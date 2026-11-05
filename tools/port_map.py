#!/usr/bin/env python3
"""
Helpers for porting a USA map overlay (C splits) to another game version.

The USA map overlays are split and decompiled, other versions are still a single
asm blob. Most map code is identical between versions, so the USA splits and symbol
names can be carried over by aligning the two overlay binaries.

Needs a built USA tree (`make build`), and `make GAME_VERSION=<ver> regenerate`
having been run at least once so the version's linker files exist.

Subcommands:
    port MAP        Write <ver> yaml + sym files for MAP from the USA ones.
    syms NAME...    Add USA bodyprog symbols to <ver>'s sym.bodyprog.txt.
    check MAP       Rebuild MAP for <ver>, report undefined symbols, section
                    size mismatches, function-level diffs and garbage bytes.
    msgs MAP        Show / generate the NTSCJ MAP_MESSAGES block for MAP.
                    EUR: write the messages of all five languages to map_msg_pal.h.
    wrap MAP [N]    Wrap message indices >= N in MAP_MSG(). EUR always uses 3.

Typical flow:
    tools/port_map.py port map1_s05
    tools/port_map.py check map1_s05
    (fix whatever `check` reports: missing symbols with `syms`, VERSION_*
     differences in the C, garbage bytes in tools/postbuild.py MAP_PATCHES)

EUR has every map once per language (map1_s05 English, s15 German, s25 French,
s35 Spanish, s45 Italian), built from the English map's source with LANG_<XXX> set:
    tools/port_map.py --version EUR port map1_s05      (from USA, as above)
    tools/port_map.py --version EUR msgs map1_s05      (text of all languages)
    tools/port_map.py --version EUR wrap map1_s05
    tools/port_map.py --version EUR check map1_s05
    tools/port_map.py --version EUR port map1_s15      (from the EUR English map)
    tools/port_map.py --version EUR check map1_s15     (likewise s25, s35, s45)

Usage:
    python3 tools/port_map.py [--version JAP0] <subcommand> ...
"""
import argparse
import bisect
import difflib
import glob
import os
import re
import struct
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
os.chdir(ROOT)

BODYPROG_BASE = 0x80024B60
ENTRY = re.compile(r'^(\s+- \[)(0x[0-9A-Fa-f]+)(\s*,?\s*.*)$')
SYM = re.compile(r'^(\w[\w.]*)\s*=\s*0x([0-9A-Fa-f]+);\s*(//.*)?')


def read(path, mode='r'):
    with open(path, mode) as f:
        return f.read()


def write(path, data):
    with open(path, 'w') as f:
        f.write(data)


def yaml_vram(path):
    return int(re.search(r'vram:\s*(0x[0-9A-Fa-f]+)', read(path)).group(1), 16)


def yaml_target(path):
    return re.search(r'target_path:\s*(\S+)', read(path)).group(1)


# PAL has every map overlay once per language: the digit after `_s` is the language (map0_s20 is the
# French map0_s00). They are built from the English map's source with LANG_<XXX> set.
PAL_LANGS = ('eng', 'ger', 'frn', 'spn', 'itl')
# PAL inserts an extra message at index 3 of the common messages every map starts with.
PAL_COMMON_MSG_COUNT = 16


def pal_lang(m):
    """(language index, English map) for a PAL map overlay name, e.g. map0_s20 -> (2, 'map0_s00')."""
    x = re.fullmatch(r'(map\d_s)(\d)(\d)', m)
    if not x or int(x.group(2)) >= len(PAL_LANGS):
        return 0, m
    return int(x.group(2)), '%s0%s' % (x.group(1), x.group(3))


def pal_lang_map(base, lang):
    return '%s%d%s' % (base[:-2], lang, base[-1])


def overlay_bin(version, m):
    """Path of the original overlay binary, from the yaml (language overlays live in VIN2..VIN5)."""
    y = 'configs/%s/maps/%s.yaml' % (version, m)
    if os.path.exists(y):
        return yaml_target(y)
    return 'assets/%s/VIN/%s.BIN' % (version, m.upper())


def built_bin(version, m):
    return 'build/%s/out/%s' % (version, overlay_bin(version, m).split('/', 2)[2])


# ---------------------------------------------------------------------------
# Binary alignment
# ---------------------------------------------------------------------------

def normalize(data):
    """Words of `data` with anything that depends on where it is linked masked out."""
    out = []
    for i in range(0, len(data) - 3, 4):
        w = struct.unpack_from('<I', data, i)[0]
        op = w >> 26
        if op in (2, 3):                       # j / jal target
            w &= ~0x3ffffff
        elif op == 0x0f:                       # lui
            w &= ~0xffff
        elif op in (0x09, 0x0d, 0x20, 0x21, 0x23, 0x24, 0x25, 0x28, 0x29, 0x2b, 0x0c, 0x0a, 0x0b, 0x08):
            if ((w >> 21) & 31) != 29:         # not sp-relative
                w &= ~0xffff
        elif 0x80000000 <= w < 0x80200000:     # pointer
            w = 0xDEAD
        out.append(w)
    return out


class Aligner:
    """Maps offsets in the USA overlay to offsets in the other version's overlay."""

    def __init__(self, usa, other):
        sm = difflib.SequenceMatcher(None, normalize(usa), normalize(other), autojunk=False)
        self.blocks = [(b.a * 4, b.b * 4, b.size * 4) for b in sm.get_matching_blocks() if b.size >= 3]
        self.starts = [b[0] for b in self.blocks]
        self.coverage = sum(b[2] for b in self.blocks) / max(len(usa), 1)
        self.approx = []

    def __call__(self, off, what=''):
        i = bisect.bisect_right(self.starts, off) - 1
        if i < 0:
            self.approx.append((off, what))
            return off + (self.blocks[0][1] - self.blocks[0][0])
        a, b, size = self.blocks[i]
        if off >= a + size:
            self.approx.append((off, what))
        return off - a + b


# ---------------------------------------------------------------------------
# Symbol helpers
# ---------------------------------------------------------------------------

def sym_names(*paths):
    names = set()
    for p in paths:
        if os.path.exists(p):
            for line in read(p).split('\n'):
                m = SYM.match(line)
                if m:
                    names.add(m.group(1))
    return names


def parse_ld_map(path):
    """Returns ({symbol: (addr, section)}, {(obj, section): (addr, size)}) from a ld map."""
    syms, sections = {}, {}
    lines = read(path).split('\n')
    cur = None
    for i, line in enumerate(lines):
        m = re.match(r'^ (\.\w+)\s+0x([0-9a-f]+)\s+0x([0-9a-f]+)\s+(\S+\.o)$', line)
        if m:
            cur = (m.group(4), m.group(1))
            if int(m.group(2), 16) >= 0x80000000:
                sections[cur] = (int(m.group(2), 16), int(m.group(3), 16))
            else:
                cur = None
            continue
        if re.match(r'^ (\.\w+)\s*$', line) and i + 1 < len(lines):
            m2 = re.match(r'^\s+0x([0-9a-f]+)\s+0x([0-9a-f]+)\s+(\S+\.o)$', lines[i + 1])
            if m2 and int(m2.group(1), 16) >= 0x80000000:
                cur = (m2.group(3), line.strip())
                sections[cur] = (int(m2.group(1), 16), int(m2.group(2), 16))
                continue
        m = re.match(r'^\s+0x([0-9a-f]+)\s+(\w+)$', line)
        if m and cur:
            syms.setdefault(m.group(2), (int(m.group(1), 16), cur[1], cur[0]))
        elif line.startswith(' .') or line.startswith('LOAD'):
            cur = None
    return syms, sections


def obj_key(obj):
    return obj.split('src/bodyprog/')[-1] if 'src/bodyprog/' in obj else obj.split('/')[-1]


def cached_bodyprog_map(version):
    """build/<ver> gets wiped by `regenerate`, so keep a copy of the bodyprog map."""
    cache = 'build/port-cache/%s.bodyprog.map' % version
    live = 'build/%s/out/1ST/BODYPROG.BIN.map' % version
    if os.path.exists(live):
        os.makedirs('build/port-cache', exist_ok=True)
        write(cache, read(live))
    if not os.path.exists(cache):
        sys.exit('No %s. Run `make GAME_VERSION=%s build` first.' % (live, version))
    return cache


# ---------------------------------------------------------------------------
# port
# ---------------------------------------------------------------------------

def overlay_extern(addr, version):
    """Address in another version of something a map calls in a screen overlay (loaded at 0x801E....).

    Several screen overlays load at the same addresses, so the owner is found from the source
    (`// 0x801E386C` comments in src/screens/<overlay>/).
    """
    hit = subprocess.run(['grep', '-rl', '0x%08X' % addr, 'src/screens'], capture_output=True, text=True).stdout.split()
    names = {p.split('/')[2] for p in hit if p.count('/') >= 3}
    for name in names:
        y = 'configs/USA/screens/%s.yaml' % name
        if not os.path.exists(y):
            continue
        text = read(y)
        tp = re.search(r'target_path:\s*(\S+)', text)
        other_path = tp.group(1).replace('assets/USA/', 'assets/%s/' % version)
        other_y = y.replace('configs/USA/', 'configs/%s/' % version)
        if not (os.path.exists(other_path) and os.path.exists(other_y)):
            continue
        base = yaml_vram(y)
        usa = read(tp.group(1), 'rb')
        if base <= addr < base + len(usa):
            al = Aligner(usa, read(other_path, 'rb'))
            return yaml_vram(other_y) + al(addr - base, 'extern')
    return None


def port_pal_language(args):
    """Port a PAL language overlay (e.g. map0_s20) from its already ported English map (map0_s00).

    The code is the same in every language, only the text and what follows it moves. The English
    yaml/sym are remapped onto the language binary, and the yaml builds the English map's source
    with the language set (src_path stays the English map, build_path is the language's directory).
    """
    M = args.map
    lang, base = pal_lang(M)
    base_y = 'configs/EUR/maps/%s.yaml' % base
    if not re.search(r'^\s+- \[\w+,\s*c,', read(base_y), re.M):
        sys.exit('%s has no C splits yet, port %s first.' % (base_y, base))
    own_y = read('configs/EUR/maps/%s.yaml' % M)
    eng, other = read(overlay_bin('EUR', base), 'rb'), read(overlay_bin('EUR', M), 'rb')
    align = Aligner(eng, other)

    out = []
    in_subsegments = False
    for l in read(base_y).split('\n'):
        key = re.match(r'^\s*(\w+):', l)
        k = key.group(1) if key else None
        if k in ('name', 'sha1', 'basename', 'target_path') and not l.startswith('  -'):
            l = next(x for x in own_y.split('\n') if re.match(r'^\s*%s:' % k, x) and not x.startswith('- ') and not x.startswith('  -'))
            l = re.sub(r'^\s*', '  ' if k not in ('name', 'sha1') else '', l)
        elif k == 'build_path':
            l = '  build_path: build/EUR/lang/%s' % PAL_LANGS[lang]
        elif k == 'src_path':
            pass
        elif re.match(r'^\s+subsegments:', l):
            in_subsegments = True
        elif in_subsegments:
            e = ENTRY.match(l)
            if e and l.startswith('      - ['):
                o = int(e.group(2), 16)
                l = '%s0x%X%s' % (e.group(1), 0 if o == 0 else align(o, l.strip()[:40]), e.group(3))
            elif e:
                l = '  - [0x%X]' % len(other)
        else:
            # paths of generated/per-overlay files (asm, linker script, sym/rel files, segment name)
            l = l.replace(base, M)
        out.append(l)
    write('configs/EUR/maps/%s.yaml' % M, '\n'.join(out))

    eb, ob = yaml_vram(base_y), yaml_vram('configs/EUR/maps/%s.yaml' % M)
    rows = []
    for line in read('configs/EUR/maps/sym.%s.txt' % base).split('\n'):
        m = SYM.match(line)
        if m:
            addr = int(m.group(2), 16)
            if eb <= addr < eb + len(eng):
                addr = ob + align(addr - eb, m.group(1))
            rows.append((addr, m.group(1), m.group(3) or ''))
    rows.sort()
    write('configs/EUR/maps/sym.%s.txt' % M, '\n'.join('%-44s = 0x%08X;%s' % (n, a, (' ' + ann) if ann else '') for a, n, ann in rows) + '\n')
    rel = 'configs/EUR/maps/rel.%s.txt' % M
    if not os.path.exists(rel):
        write(rel, '')
    print('%s (%s %s): %.1f%% aligned with %s, %d symbols, %d approximated offsets' % (
        M, PAL_LANGS[lang], 'language overlay', align.coverage * 100, base, len(rows), len(align.approx)))
    for off, what in align.approx[:20]:
        print('  approximated 0x%X (%s)' % (off, what))
    print('Next: tools/port_map.py --version EUR msgs %s, make GAME_VERSION=EUR regenerate, then check %s' % (base, M))


def cmd_port(args):
    M, V = args.map, args.version
    U = M.upper()
    if V == 'EUR' and pal_lang(M)[0] > 0:
        return port_pal_language(args)
    usa = read('assets/USA/VIN/%s.BIN' % U, 'rb')
    other = read(overlay_bin(V, M), 'rb')
    ub = yaml_vram('configs/USA/maps/%s.yaml' % M)
    jb = yaml_vram('configs/%s/maps/%s.yaml' % (V, M))
    align = Aligner(usa, other)

    # yaml: keep the version's header, remap the USA subsegments
    usa_y = read('configs/USA/maps/%s.yaml' % M).split('\n')
    old_y = read('configs/%s/maps/%s.yaml' % (V, M)).split('\n')
    us = next(i for i, l in enumerate(usa_y) if re.match(r'^\s+subsegments:', l))
    os_ = next(i for i, l in enumerate(old_y) if re.match(r'^\s+subsegments:', l))
    out = old_y[:os_ + 1]
    for l in usa_y[us + 1:]:
        e = ENTRY.match(l)
        if e and l.startswith('      - ['):
            o = int(e.group(2), 16)
            l = '%s0x%X%s' % (e.group(1), 0 if o == 0 else align(o, l.strip()[:40]), e.group(3))
        elif e:
            l = '  - [0x%X]' % len(other)
        out.append(l)
    yaml_text = '\n'.join(out)
    if not os.path.exists('configs/%s/sym.main.txt' % V):
        yaml_text = yaml_text.replace('configs/%s/sym.main.txt' % V, 'configs/USA/sym.main.txt')

    # symbols: everything the USA link knows about in this overlay, remapped
    syms = {}
    extern_rows = []
    usa_map = 'build/USA/out/VIN/%s.BIN.map' % U
    if not os.path.exists(usa_map):
        sys.exit('Missing %s, run `make build` for USA first.' % usa_map)
    ld_syms, _ = parse_ld_map(usa_map)
    for name, (addr, sec, obj) in ld_syms.items():
        if sec in ('.text', '.rodata', '.data', '.bss') and ub <= addr < ub + len(usa) + 0x100000:
            syms[name] = (addr, sec)
    auto = 'linkers/USA/maps/undefined_syms_auto.%s.txt' % M
    for path in (auto, 'configs/USA/maps/sym.%s.txt' % M):
        if os.path.exists(path):
            for line in read(path).split('\n'):
                m = SYM.match(line) or re.match(r'^(\w+)\s*=\s*0x([0-9A-Fa-f]+);', line)
                if m and ub <= int(m.group(2), 16) < ub + len(usa) and m.group(1) not in syms:
                    syms[m.group(1)] = (int(m.group(2), 16), None)
    for f in glob.glob('asm/USA/maps/%s/nonmatchings/**/*.s' % M, recursive=True):
        n = os.path.basename(f)[:-2]
        m = re.search(r'_([0-9A-F]{8})(?:_|$)', n)
        if m and n not in syms and ub <= int(m.group(1), 16) < ub + len(usa):
            syms[n] = (int(m.group(1), 16), '.text' if n.startswith('func_') else None)
    annotations = {}
    if os.path.exists('configs/USA/maps/sym.%s.txt' % M):
        for line in read('configs/USA/maps/sym.%s.txt' % M).split('\n'):
            m = SYM.match(line)
            if m:
                annotations[m.group(1)] = m.group(3) or ''
    # functions/data the map uses from the screen overlays (stf_roll etc.), which load at 0x801E....
    for kind, ext in (('funcs', '.text'), ('syms', None)):
        path = 'linkers/USA/maps/undefined_%s_auto.%s.txt' % (kind, M)
        if not os.path.exists(path):
            continue
        for line in read(path).split('\n'):
            m = re.match(r'^(\w+)\s*=\s*0x([0-9A-Fa-f]+);', line)
            if not m or int(m.group(2), 16) < 0x801E0000:
                continue
            tr = overlay_extern(int(m.group(2), 16), V)
            if tr is not None and m.group(1) not in syms:
                extern_rows.append((tr, m.group(1), '// type:func' if kind == 'funcs' else ''))
    known = sym_names('configs/%s/sym.bodyprog.txt' % V, 'configs/%s/sym.main.txt' % V, 'configs/USA/sym.main.txt')
    rows = []
    for name, (addr, sec) in syms.items():
        if name in known or addr - ub > len(usa):
            continue
        ann = annotations.get(name, '')
        if not ann and sec == '.text':
            ann = '// type:func'
        rows.append((jb + align(addr - ub, name), name, ann))
    rows += extern_rows
    rows.sort()
    sym_text = '\n'.join('%-44s = 0x%08X;%s' % (n, a, (' ' + ann) if ann else '') for a, n, ann in rows) + '\n'

    ydst = args.out and os.path.join(args.out, M + '.yaml') or 'configs/%s/maps/%s.yaml' % (V, M)
    sdst = args.out and os.path.join(args.out, 'sym.%s.txt' % M) or 'configs/%s/maps/sym.%s.txt' % (V, M)
    if args.out:
        os.makedirs(args.out, exist_ok=True)
    write(ydst, yaml_text)
    write(sdst, sym_text)
    print('%s: %.1f%% of the overlay aligned, %d symbols, %d approximated offsets -> %s' % (
        M, align.coverage * 100, len(rows), len(align.approx), os.path.dirname(ydst)))
    for off, what in align.approx[:20]:
        print('  approximated 0x%X (%s)' % (off, what))
    cached_bodyprog_map('USA')
    if not args.out:
        print('Next: make GAME_VERSION=%s regenerate, then tools/port_map.py check %s' % (V, M))


# ---------------------------------------------------------------------------
# syms
# ---------------------------------------------------------------------------

def usa_bodyprog_syms():
    out = {}
    for line in read('configs/USA/sym.bodyprog.txt').split('\n'):
        m = SYM.match(line)
        if m:
            out[m.group(1)] = (int(m.group(2), 16), m.group(3) or '')
    return out


def translate_bodyprog_syms(names, version):
    """name -> 'name = 0xADDR; ann' line for `version`, by position inside the same object file."""
    usa_syms, usa_secs = parse_ld_map(cached_bodyprog_map('USA'))
    _, oth_secs = parse_ld_map(cached_bodyprog_map(version))
    usa_by_key = {(obj_key(o), s): v for (o, s), v in usa_secs.items() if v[1]}
    oth_by_key = {(obj_key(o), s): v for (o, s), v in oth_secs.items() if v[1]}
    usym = usa_bodyprog_syms()
    lines, problems = [], []
    for n in names:
        if n in usym:
            addr, ann = usym[n]
        else:
            m = re.fullmatch(r'D_([0-9A-F]{8})', n)
            if not m:
                problems.append((n, 'not in USA sym.bodyprog.txt'))
                continue
            addr, ann = int(m.group(1), 16), ''
        hit = next((k for k, (a, z) in usa_by_key.items() if a <= addr < a + z and k in oth_by_key), None)
        if not hit:
            # libkmath is assembled from asm (no object in the map), position it relative to Math_Sin.
            osym = {}
            for line in read('configs/%s/sym.bodyprog.txt' % version).split('\n'):
                x = SYM.match(line)
                if x:
                    osym[x.group(1)] = int(x.group(2), 16)
            anchor = usym.get('Math_Sin')
            if anchor and 'Math_Sin' in osym and anchor[0] <= addr < anchor[0] + 0x2000:
                lines.append(('%-44s = 0x%08X;%s' % (n, osym['Math_Sin'] + addr - anchor[0], (' ' + ann) if ann else ''), '  # libkmath, relative to Math_Sin'))
                continue
            problems.append((n, 'no matching object (asm library?), add by hand'))
            continue
        ua, uz = usa_by_key[hit]
        ja, jz = oth_by_key[hit]
        note = '' if uz == jz else '  # object size differs, check this one'
        lines.append(('%-44s = 0x%08X;%s' % (n, ja + addr - ua, (' ' + ann) if ann else ''), note))
    return lines, problems


def insert_sym_lines(path, new_lines):
    L = read(path).split('\n')
    have = {SYM.match(l).group(1) for l in L if SYM.match(l)}
    for line in new_lines:
        m = SYM.match(line)
        if m.group(1) in have:
            print('  already there:', m.group(1))
            continue
        addr = int(m.group(2), 16)
        best = None
        for i, l in enumerate(L):
            e = SYM.match(l)
            if e and int(e.group(2), 16) <= addr and (best is None or int(e.group(2), 16) >= int(SYM.match(L[best]).group(2), 16)):
                best = i
        L.insert(best + 1 if best is not None else len(L), line)
        have.add(m.group(1))
        print('  added', line.split()[0])
    write(path, '\n'.join(L))


def sync_annotations(version):
    """Copy `// type:.. size:..` comments from the USA bodyprog sym to the version's, where it has none.

    Without a size spimdisasm can't tie `sym + offset` accesses to the symbol, so the asm of a
    ported map ends up without the reference (and the C code then fails to link against it).
    """
    usa = {}
    for line in read('configs/USA/sym.bodyprog.txt').split('\n'):
        m = SYM.match(line)
        if m and m.group(3):
            usa[m.group(1)] = m.group(3)
    path = 'configs/%s/sym.bodyprog.txt' % version
    out, n = [], 0
    for line in read(path).split('\n'):
        m = SYM.match(line)
        if m and not m.group(3) and m.group(1) in usa:
            line = line.rstrip() + ' ' + usa[m.group(1)]
            n += 1
        out.append(line)
    write(path, '\n'.join(out))
    print('Copied %d annotations into %s' % (n, path))


def cmd_syms(args):
    if args.sync:
        sync_annotations(args.version)
        return
    lines, problems = translate_bodyprog_syms(args.names, args.version)
    insert_sym_lines('configs/%s/sym.bodyprog.txt' % args.version, [l for l, _ in lines])
    for l, note in lines:
        if note:
            print('  note:', l.split()[0], note)
    for n, why in problems:
        print('  !!', n, why)
    print('Run `make GAME_VERSION=%s regenerate` before building, the linker only gets symbols the generated asm refers to.' % args.version)


# ---------------------------------------------------------------------------
# check
# ---------------------------------------------------------------------------

def run(cmd, log):
    with open(log, 'w') as f:
        subprocess.run(cmd, shell=True, stdout=f, stderr=subprocess.STDOUT, stdin=subprocess.DEVNULL)
    return read(log)


def disassemble(path, start, stop):
    out = subprocess.run(['mips-linux-gnu-objdump', '-D', '-b', 'binary', '-m', 'mips:3000', '-EL',
                          '--start-address=0x%x' % start, '--stop-address=0x%x' % stop, path],
                         capture_output=True, text=True).stdout
    return [re.sub(r'^\s*[0-9a-f]+:\t[0-9a-f ]+\t', '', l) for l in out.split('\n')[7:] if l.strip()]


def add_map_patch(version, m, entries):
    text = read('tools/postbuild.py')
    block = '        "%s": [\n%s\n        ]' % (m, ',\n'.join("            [0x%X, b'%s']" % e for e in entries))
    table = text.split('MAP_PATCHES = {')[1].split('def apply_map_patches')[0]
    own = table.split('    "%s": {' % version)[1].split('\n    },')[0] if '    "%s": {' % version in table else ''
    if '"%s": [' % m in own:
        print('  %s already has patches for %s in tools/postbuild.py, edit them by hand' % (version, m))
    else:
        head = '    "%s": {\n' % version
        if head in text:
            text = text.replace(head, head + block + ',\n', 1)
        else:
            text = text.replace('MAP_PATCHES = {\n', 'MAP_PATCHES = {\n    "%s": {\n%s\n    },\n' % (version, block), 1)
        write('tools/postbuild.py', text)
        print('  added to MAP_PATCHES in tools/postbuild.py')
    mk = read('Makefile')
    if 'maps/%s' % m not in mk.split('TARGET_POSTBUILD :=')[1].split('\n')[0].split():
        mk = mk.replace('TARGET_POSTBUILD :=', 'TARGET_POSTBUILD := maps/%s' % m, 1) if False else re.sub(r'(TARGET_POSTBUILD :=[^\n]*)', r'\1 maps/%s' % m, mk, count=1)
        write('Makefile', mk)
        print('  added maps/%s to TARGET_POSTBUILD in the Makefile' % m)


def cmd_check(args):
    M, V = args.map, args.version
    U = M.upper()
    log = 'build/port-cache/%s.%s.build.log' % (M, V)
    os.makedirs('build/port-cache', exist_ok=True)
    text = run('. .venv/bin/activate 2>/dev/null; make -k -j8 GAME_VERSION=%s %s' % (V, built_bin(V, M)), log)
    undefined = {}
    for m in re.finditer(r"undefined reference to `([^']+)'", text):
        undefined[m.group(1)] = undefined.get(m.group(1), 0) + 1
    errors = sorted({m.group(0)[:160] for m in re.finditer(r'.*(?:error:|undeclared|No rule to make|can\'t open).*', text)})
    if undefined:
        print('Undefined symbols (try `port_map.py syms NAME...` for bodyprog ones):')
        for n, c in sorted(undefined.items(), key=lambda x: -x[1]):
            print('  %-40s x%d' % (n, c))
    for e in errors[:8]:
        print('  ' + e)
    built = built_bin(V, M)
    if not os.path.exists(built):
        print('No %s was produced, see %s' % (built, log))
        return
    # postbuild garbage bytes, if the map is in the postbuild list
    subprocess.run([sys.executable, 'tools/postbuild.py', 'maps/' + M], capture_output=True)

    # section sizes vs the yaml
    _, secs = parse_ld_map(built + '.map')
    ents = []
    for l in read('configs/%s/maps/%s.yaml' % (V, M)).split('\n'):
        x = re.match(r'^\s+- \[(0x[0-9A-Fa-f]+)\s*,?\s*(\.?\w+)?(?:,\s*([\w/]+))?', l)
        if x and l.startswith('      - ['):
            ents.append((int(x.group(1), 16), x.group(2), x.group(3)))
    print('Section size mismatches (small negatives are alignment padding):')
    for i, (off, t, n) in enumerate(ents[:-1]):
        sec = {'c': '.text', '.rodata': '.rodata', '.data': '.data', '.bss': '.bss'}.get(t)
        if not sec or not n:
            continue
        exp = ents[i + 1][0] - off
        for (obj, s), (a, z) in secs.items():
            if obj.endswith('/%s.c.o' % n) and 'src/maps/%s/' % pal_lang(M)[1] in obj and s == sec:
                if z != exp and not (z == 0 and exp == 0):
                    print('  %-8s 0x%-6x %-24s built=0x%x expected=0x%x' % (sec, off, n, z, exp))
    # function-level diff of the text
    b, o = read(built, 'rb'), read(overlay_bin(V, M), 'rb')
    print('Sizes: built %d, original %d' % (len(b), len(o)))
    base = yaml_vram('configs/%s/maps/%s.yaml' % (V, M))
    ld_syms, _ = parse_ld_map(built + '.map')
    funcs = sorted((a - base, n) for n, (a, s, ob) in ld_syms.items() if s == '.text' and a >= base)
    keys = [f[0] for f in funcs]

    def where(off):
        i = bisect.bisect_right(keys, off) - 1
        return '%s+0x%x' % (funcs[i][1], off - funcs[i][0]) if i >= 0 else '?'
    sm = difflib.SequenceMatcher(None, normalize(b), normalize(o), autojunk=False)
    diffs = [op for op in sm.get_opcodes() if op[0] != 'equal']
    if diffs:
        print('Code differences (built vs original, ignoring relocations):')
        for t, a1, a2, b1, b2 in diffs[:25]:
            print('  %-7s built 0x%x-0x%x (%s)  original 0x%x-0x%x' % (t, a1 * 4, a2 * 4, where(a1 * 4), b1 * 4, b2 * 4))
        if len(diffs) > 25:
            print('  ... %d more' % (len(diffs) - 25))
        print('Disassemble a function with: mips-linux-gnu-objdump -D -b binary -m mips:3000 -EL --start-address=.. --stop-address=.. <file>')
    # exact byte differences
    n = min(len(b), len(o))
    words = sorted({i // 4 * 4 for i in range(n) if b[i] != o[i]})
    groups = []
    for w in words:
        if groups and w - groups[-1][1] <= 8:
            groups[-1][1] = w
        else:
            groups.append([w, w])
    if not groups:
        print('MATCH: no byte differences.')
        return
    base_ = yaml_vram('configs/%s/maps/%s.yaml' % (V, M))

    def in_object(lo, hi):
        # Only padding can be patched, never bytes a section owns. Section tails are the exception:
        # the last bytes of a .rodata/.data section are alignment padding after the final string/value.
        for (_, sec), (a, z) in secs.items():
            if not z or a < base_ or sec not in ('.text', '.rodata', '.data'):
                continue
            start, end = a - base_, a - base_ + z
            if start < hi and lo < end:
                tail = sec in ('.rodata', '.data') and lo >= end - 3 and b[end - 4:lo].count(0) > 0
                if not tail:
                    return True
        return False
    def trimmed(g):
        lo = g[0]
        while lo < g[1] + 4 and b[lo] == o[lo]:
            lo += 1
        hi = g[1] + 4
        while hi > lo and b[hi - 1] == o[hi - 1]:
            hi -= 1
        return lo, hi
    small = [g for g in groups if g[1] - g[0] <= 4 and not in_object(*trimmed(g))]
    # Words that only differ in the low 16 bits (address operands): a constant offset means the
    # symbols they refer to are misplaced in the ported sym file (raw data blobs are only approximated).
    shifts = {}
    for w in words:
        wb, wo = struct.unpack_from('<I', b, w)[0], struct.unpack_from('<I', o, w)[0]
        if (wb >> 26) == (wo >> 26) or (wb >> 16) == (wo >> 16):
            d = (wb & 0xffff) - (wo & 0xffff)
            if d and abs(d) <= 0x40 and d % 4 == 0:
                lowest, count = shifts.get(d, (0xffff, 0))
                shifts[d] = (min(lowest, wo & 0xffff), count + 1)
    for d, (lowest, count) in sorted(shifts.items(), key=lambda x: -x[1][1]):
        if count >= 3:
            print('Hint: %d words use an address %+d off the original, from low16 0x%04X upwards. Symbols in '
                  'configs/%s/maps/sym.%s.txt from there on are probably misplaced.' % (count, d, lowest, V, M))
    print('%d differing byte regions, %d look like garbage bytes in padding.' % (len(groups), len(small)))
    if small:
        entries = []
        for g in small:
            s = g[0]
            while s < g[1] + 4 and b[s] == o[s]:
                s += 1
            e = g[1] + 4
            while e > s and b[e - 1] == o[e - 1]:
                e -= 1
            entries.append((s, ''.join('\\x%02X' % x for x in o[s:e])))
        if args.patch:
            add_map_patch(V, M, entries)
        else:
            print('Garbage bytes for MAP_PATCHES in tools/postbuild.py (re-run with --patch to add them,')
            print('this also adds the map to TARGET_POSTBUILD in the Makefile):')
            for off, data in entries:
                print("    [0x%X, b'%s']," % (off, data))


# ---------------------------------------------------------------------------
# msgs
# ---------------------------------------------------------------------------

# cp932 maps a few characters to different code points than the Shift-JIS conversion the build uses.
CP932_TO_SJIS = {'\uff0d': '\u2212', '\uff5e': '\u301c', '\u2225': '\u2016', '\uffe0': '\u00a2',
                 '\uffe1': '\u00a3', '\uffe2': '\u00ac', '\u2015': '\u2014'}


def sjis_text(text):
    return ''.join(CP932_TO_SJIS.get(c, c) for c in text)


def jp_message_table(version, m):
    """Strings of the version's MAP_MESSAGES table (index -> text), read from its pointer array.

    Reading the table (instead of the strings in rodata) keeps duplicated messages, which the
    compiler stores only once, at their own indices.
    """
    sym = 'configs/%s/maps/sym.%s.txt' % (version, m)
    addr = None
    for line in read(sym).split('\n'):
        x = SYM.match(line)
        if x and x.group(1) == 'MAP_MESSAGES':
            addr = int(x.group(2), 16)
    if addr is None:
        return None
    base = yaml_vram('configs/%s/maps/%s.yaml' % (version, m))
    data = read(overlay_bin(version, m), 'rb')
    out, off = [], addr - base
    while off + 4 <= len(data):
        p = struct.unpack_from('<I', data, off)[0] - base
        if not (0 <= p < len(data)):
            break
        end = data.index(b'\0', p)
        out.append(sjis_text(data[p:end].decode('cp932', errors='replace')))
        off += 4
    return out


def pal_message_table(m):
    """Raw strings of a PAL overlay's MAP_MESSAGES table, found through the map header's `mapMessages` (0x30)."""
    data = read(overlay_bin('EUR', m), 'rb')
    base = yaml_vram('configs/EUR/maps/%s.yaml' % m)
    table = struct.unpack_from('<I', data, 4 + 0x30)[0] - base
    out, off = [], table
    while off + 4 <= len(data):
        p = struct.unpack_from('<I', data, off)[0] - base
        # The first string of .rodata can follow non-zero bytes, but strings are word aligned.
        if not (0 <= p < table) or (p % 4 and data[p - 1] != 0):
            break
        out.append(data[p:data.index(b'\0', p)])
        off += 4
    return out


def pal_c_string(raw):
    """C literal for PAL text. Latin-1 letters are written as UTF-8, the build converts them back."""
    out = []
    for c in raw:
        if c == 0x0A:
            out.append('\\n')
        elif c == 0x09:
            out.append('\\t')
        elif c in (0x22, 0x5C):
            out.append('\\' + chr(c))
        elif 0x20 <= c < 0x7F or c >= 0xA0:
            out.append(chr(c))
        else:
            out.append('\\%03o' % c)
    return '"%s"' % ''.join(out)


def pal_msg_block(tables, first, width):
    lines = []
    for lang, table in enumerate(tables):
        lines.append('#%s LANGUAGE_IS(%s)' % ('if' if lang == 0 else 'elif', PAL_LANGS[lang].upper()))
        for i in range(first, len(table)):
            idx = ('%d' % i).ljust(width)
            lines.append('    /* %s */ %s,' % (idx, pal_c_string(table[i])))
    lines.append('#endif')
    return '\n'.join(lines) + '\n'


def cmd_msgs_pal(args):
    """Write the PAL messages of a map (all five languages) into map_msg_pal.h next to its source."""
    base = pal_lang(args.map)[1]
    tables = [pal_message_table(pal_lang_map(base, lang)) for lang in range(len(PAL_LANGS))]
    for lang, table in enumerate(tables):
        print('  %s %s: %d messages' % (pal_lang_map(base, lang), PAL_LANGS[lang], len(table)))
    if len({len(t) for t in tables}) != 1:
        print('  !! the languages have different message counts, check the tables above')

    common = 'include/maps/shared/map_msg_common_pal.h'
    text = ('// Messages every PAL map starts with, the same in all maps of a language.\n'
            + pal_msg_block([t[:PAL_COMMON_MSG_COUNT] for t in tables], 0, 2))
    if not os.path.exists(common):
        write(common, text)
        print('Wrote', common)
    elif read(common) != text:
        print('  !! %s differs from the common messages of %s, left as is' % (common, base))

    own = 'src/maps/%s/map_msg_pal.h' % base
    write(own, '// PAL messages of this map after the common ones, the language overlays differ only by these.\n'
               + pal_msg_block(tables, PAL_COMMON_MSG_COUNT, 2))
    print('Wrote', own)

    files = [f for f in glob.glob('src/maps/%s/*.c' % base) if 'const char* MAP_MESSAGES[]' in read(f)]
    if len(files) != 1:
        print('No single file defines MAP_MESSAGES for %s, include map_msg_pal.h by hand' % base)
        return
    f = files[0]
    text = read(f)
    body = text.split('MAP_MESSAGES[]')[1].split('};')[0]
    if '"map_msg_pal.h"' in body:
        return
    if '#endif' not in body:
        print('%s: MAP_MESSAGES has no version blocks, add `#elif VERSION_REGION_IS(PAL)` + the include by hand' % f)
        return
    i = body.rindex('#endif')
    new = body[:i] + '#elif VERSION_REGION_IS(PAL)\n    #include "map_msg_pal.h"\n' + body[i:]
    write(f, text.replace(body, new, 1))
    print('Wrote', f)


def cmd_msgs(args):
    M, V = args.map, args.version
    if V == 'EUR':
        return cmd_msgs_pal(args)
    files = [f for f in glob.glob('src/maps/%s/*.c' % M) if 'const char* MAP_MESSAGES[]' in read(f)]
    if len(files) != 1:
        sys.exit('No single file defines MAP_MESSAGES for %s' % M)
    f = files[0]
    text = read(f)
    body = text.split('MAP_MESSAGES[]')[1].split('};')[0]
    if 'VERSION_REGION_IS(NTSCJ)' in body:
        sys.exit('%s already has an NTSCJ block' % f)
    table = jp_message_table(V, M)
    if table is None:
        sys.exit('MAP_MESSAGES is not in configs/%s/maps/sym.%s.txt (run `port` first)' % (V, M))
    jp = table[15:]
    if 'NO_STAGE' in body and not re.search(r'/\* 15 ', body):
        # Maps that only have the "no stage" placeholder message.
        line = [l for l in body.split('\n') if 'NO_STAGE' in l][0]
        text_ = jp[-1].replace('\\', '\\\\').replace('"', '\\"').replace('\t', '\\t').replace('\n', '\\n')
        print('%s: NO_STAGE placeholder -> "%s"' % (f, jp[-1].strip()))
        if args.apply:
            new = '#if VERSION_REGION_IS(NTSC)\n' + line.rstrip() + '\n#elif VERSION_REGION_IS(NTSCJ)\n    "' + text_ + '"\n#endif'
            write(f, text.replace(body, body.replace(line, new, 1), 1))
            print('Wrote', f)
        return
    usa = [(int(i), s) for i, s in re.findall(r'^\s*/\* (\d+)\s*\*/ (".*?")\s*,?\s*$', body, re.M) if int(i) >= 15]
    if not usa:
        print('%s has no custom messages' % f)
        return
    equal = len(jp) == len(usa)
    print('%s: %d custom USA messages, %d in the %s table' % (f, len(usa), len(jp), V))
    usa_by_idx = dict(usa)
    for k in range(max(len(jp), len(usa))):
        idx = 15 + k
        u = usa_by_idx.get(idx, '')
        j = jp[k] if k < len(jp) else ''
        print('  %-3d %-44s | %s' % (idx, u[:42], j[:36].replace('\n', '\\n').replace('\t', '\\t')))
    if not equal and not args.force:
        print('Counts differ: the JP table merges/drops messages, so indices used by the event\n'
              'tables and map code shift too. Compare the two columns above (rows are the same\n'
              'index in each version), then re-run with --apply --force to write the JP block and\n'
              'add a MAP_MSG(idx) macro for the shifted indices (see map2_s00.h).')
        return
    if not args.apply:
        print('Counts match. Re-run with --apply to wrap the USA block in #if and add the NTSCJ one.')
        return
    first = next(l for l in body.split('\n') if re.match(r'^\s*/\* (\d+)', l) and int(re.match(r'^\s*/\* (\d+)', l).group(1)) >= 15)
    last = [l for l in body.split('\n') if re.match(r'^\s*/\* (\d+)', l)][-1]
    a, b = body.index(first), body.rindex(last) + len(last)
    block = ',\n'.join('    /* %d */ "%s"' % (15 + k, s.replace('\\', '\\\\').replace('"', '\\"').replace('\t', '\\t').replace('\n', '\\n'))
                       for k, s in enumerate(jp))
    new = body[:a] + '#if VERSION_REGION_IS(NTSC)\n' + body[a:b] + '\n#elif VERSION_REGION_IS(NTSCJ)\n' + block + '\n#endif' + body[b:]
    write(f, text.replace(body, new, 1))
    print('Wrote', f)


MSG_CALLS = {      # function -> argument positions holding a message index (negative: from the end)
    'Event_DisplayMapMsg': [1],
    'Event_DisplayMapMsgWithBg': [3],
    'Event_DisplayMapMsgWithAudio': [0],
    'Event_DisplayMapMsgWithSfx': [0],
    'Event_DisplayMapMsgWithDimmedBg': [3, 4],
    'Event_ItemTake': [-1],
    'Event_PaperMapTake': [-1],
}


def split_args(text, start):
    """Top-level argument spans of the call whose '(' is at text[start]."""
    depth, spans, a = 0, [], start + 1
    for i in range(start, len(text)):
        c = text[i]
        if c == '(':
            depth += 1
        elif c == ')':
            depth -= 1
            if depth == 0:
                spans.append((a, i))
                return spans, i
        elif c == ',' and depth == 1:
            spans.append((a, i))
            a = i + 1
    return spans, len(text)


def cmd_wrap(args):
    """Wrap message indices >= THRESH in MAP_MSG(), and add the macro to the map's header."""
    M, V = args.map, args.version
    if V == 'EUR':
        # Same for every PAL map: the extra common message at index 3 moves everything after it up by one.
        thresh = 3 if args.thresh is None else args.thresh
        macro = args.macro or '((idx) >= %d ? (idx) + 1 : (idx))' % thresh
    else:
        if args.thresh is None:
            sys.exit('wrap needs THRESH for %s' % V)
        thresh = args.thresh
        macro = args.macro or '((idx) >= %d ? (idx) - %d : (idx))' % (thresh, args.shift)
    n = 0
    for f in glob.glob('src/maps/%s/*.c' % M):
        text = read(f)
        orig = text
        # event table entries that show a message
        text = re.sub(r'(\.sysState\s*=\s*SysState_ReadMessage,\n\s*\.eventParam\s*=\s*)(\d+),',
                      lambda m: m.group(1) + ('MAP_MSG(%s),' % m.group(2) if int(m.group(2)) >= thresh else m.group(2) + ','), text)
        # calls that take a message index
        for fn, positions in MSG_CALLS.items():
            pos = 0
            while True:
                m = re.compile(r'\b%s\(' % fn).search(text, pos)
                if not m:
                    break
                spans, end = split_args(text, m.end() - 1)
                edits = []
                for p in positions:
                    if -len(spans) <= p < len(spans):
                        a, b = spans[p]
                        arg = text[a:b]
                        if re.fullmatch(r'\s*\d+\s*', arg) and int(arg) >= thresh:
                            edits.append((a, b, arg.replace(arg.strip(), 'MAP_MSG(%s)' % arg.strip())))
                for a, b, new in sorted(edits, reverse=True):
                    text = text[:a] + new + text[b:]
                    n += 1
                pos = m.end()
        if text != orig:
            if '#include "maps/' not in text.split('MAP_MSG')[0].split('\n\n')[0] and f.endswith('events_data.c'):
                pass
            write(f, text)
    hdrs = glob.glob('include/maps/*/%s.h' % M)
    if not hdrs:
        sys.exit('no header include/maps/*/%s.h' % M)
    h = hdrs[0]
    htext = read(h)
    pal_note = '// PAL has an extra common message at index 3, so every message after it is one higher than in USA.\n'
    if V == 'EUR' and 'define MAP_MSG' not in htext:
        block = (pal_note + '#if VERSION_REGION_IS(PAL)\n    #define MAP_MSG(idx) %s\n#else\n'
                 '    #define MAP_MSG(idx) (idx)\n#endif\n' % macro)
        inc = '#include "maps/shared.h"'
        i = htext.index(inc) if inc in htext else htext.rindex('#endif')
        write(h, htext[:i] + block + '\n' + htext[i:])
    elif V == 'EUR' and 'VERSION_REGION_IS(PAL)' not in htext:
        # Add a PAL branch to the existing NTSCJ one.
        d = htext.index('#define MAP_MSG')
        s = htext.rindex('#if VERSION_REGION_IS', 0, d)
        e = htext.index('#endif', d)
        htext = htext[:e] + '#elif VERSION_REGION_IS(PAL)\n    #define MAP_MSG(idx) %s\n' % macro + htext[e:]
        write(h, htext[:s] + pal_note + htext[s:])
    elif 'define MAP_MSG' not in htext:
        block = ('\n// JAP0 merges, drops or adds messages compared to USA, so message indices in code differ.\n'
                 '#if VERSION_REGION_IS(NTSC)\n    #define MAP_MSG(idx) (idx)\n#elif VERSION_REGION_IS(NTSCJ)\n'
                 '    #define MAP_MSG(idx) %s\n#endif\n' % macro)
        # Goes above the shared.h include, like the other per-map defines.
        inc = '#include "maps/shared.h"'
        i = htext.index(inc) if inc in htext else htext.rindex('#endif')
        write(h, htext[:i] + block.lstrip('\n') + '\n' + htext[i:])
    for f in glob.glob('src/maps/%s/*events_data.c' % M):
        t = read(f)
        inc = '#include "%s"' % h.replace('include/', '')
        if 'MAP_MSG' in t and inc not in t:
            write(f, t.replace('#include "bodyprog/bodyprog.h"\n', '#include "bodyprog/bodyprog.h"\n' + inc + '\n', 1))
    print('Wrapped %d call arguments plus the event table; macro in %s' % (n, h))
    print('Check the remaining uses by hand (assignments like `...MsgIdx = N`) with: grep -n "MapMsg" src/maps/%s/*.c' % M)


def cmd_validate(args):
    """Compare the sym file with the symbols the C objects of the last build actually define.

    Splat only uses the sym file to name things in the asm it generates, the C build never reads those
    addresses back, so a wrong address there can go unnoticed. Where a symbol is defined by a compiled
    .c.o its linked address is the truth, so any difference is an error in the sym file.
    Symbols defined only by generated asm (raw data blobs) take their address from the sym file itself
    and can't be checked this way; `check` catches those when the overlay stops matching.
    """
    M, V = args.map, args.version
    if M == 'bodyprog':
        map_path, sym_path = 'build/%s/out/1ST/BODYPROG.BIN.map' % V, 'configs/%s/sym.bodyprog.txt' % V
    else:
        map_path, sym_path = built_bin(V, M) + '.map', 'configs/%s/maps/sym.%s.txt' % (V, M)
    if not os.path.exists(map_path):
        sys.exit('No %s, build %s for %s first.' % (map_path, M, V))
    linked, _ = parse_ld_map(map_path)
    sym_lines = read(sym_path).split('\n')
    ok = bad = unchecked = 0
    problems = []
    for i, line in enumerate(sym_lines):
        m = SYM.match(line)
        if not m:
            continue
        name, addr = m.group(1), int(m.group(2), 16)
        hit = linked.get(name)
        if not hit or not hit[2].endswith('.c.o'):
            unchecked += 1
            continue
        if hit[0] == addr:
            ok += 1
        else:
            bad += 1
            problems.append((i, name, addr, hit[0], hit[2].split('/')[-1]))
    print('%s (%s): %d symbols match the linked C objects, %d differ, %d not defined by C (not checked)' % (M, V, ok, bad, unchecked))
    for i, name, addr, real, obj in problems[:40]:
        print('  %-40s sym.txt 0x%08X  linked 0x%08X (%+d)  in %s' % (name, addr, real, real - addr, obj))
    if len(problems) > 40:
        print('  ... %d more' % (len(problems) - 40))
    if args.fix and problems:
        for i, name, addr, real, obj in problems:
            sym_lines[i] = re.sub(r'0x[0-9A-Fa-f]+;', '0x%08X;' % real, sym_lines[i], count=1)
        write(sym_path, '\n'.join(sym_lines))
        print('Rewrote %d addresses in %s' % (len(problems), sym_path))
    return bad


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--version', default='JAP0')
    sub = ap.add_subparsers(dest='cmd', required=True)
    p = sub.add_parser('port')
    p.add_argument('map')
    p.add_argument('--out', help='write the yaml/sym here instead of into configs/')
    p.set_defaults(fn=cmd_port)
    p = sub.add_parser('syms')
    p.add_argument('names', nargs='*')
    p.add_argument('--sync', action='store_true', help='copy type/size annotations from the USA sym file')
    p.set_defaults(fn=cmd_syms)
    p = sub.add_parser('check')
    p.add_argument('map')
    p.add_argument('--patch', action='store_true', help='add suggested garbage-byte patches to postbuild.py / Makefile')
    p.set_defaults(fn=cmd_check)
    p = sub.add_parser('validate')
    p.add_argument('map', help="map name, or 'bodyprog'")
    p.add_argument('--fix', action='store_true', help='rewrite the sym file addresses to the linked ones')
    p.set_defaults(fn=cmd_validate)
    p = sub.add_parser('wrap')
    p.add_argument('map')
    p.add_argument('thresh', type=int, nargs='?', help='first message index that is shifted (JAP0), PAL always uses 3')
    p.add_argument('--shift', type=int, default=1)
    p.add_argument('--macro', help='full MAP_MSG(idx) body for non-uniform shifts')
    p.set_defaults(fn=cmd_wrap)
    p = sub.add_parser('msgs')
    p.add_argument('map')
    p.add_argument('--apply', action='store_true')
    p.add_argument('--force', action='store_true', help='write the JP block even if the message counts differ')
    p.set_defaults(fn=cmd_msgs)
    args = ap.parse_args()
    args.fn(args)


if __name__ == '__main__':
    main()
