#!/usr/bin/env python3

# Finds data symbols that start inside another symbol's declared C size, e.g. a struct at 0x10 of
# size 0x10 with a separate symbol at 0x14. Such inner symbols should be field/element accesses
# of the outer one, or the outer declaration is too large.
#
# Sizes come from sizeof() on file-scope declarations in each preprocessed TU (build/<ver>/src/**/*.i),
# compiled with the project's own cc1 so struct layout matches the target. Symbol addresses come from
# the linked ELFs, so a build is required first.
#
# --write-sym adds `size:` to sym.txt entries that lack one, skipping symbols that overlap others or
# have conflicting sizes between TUs. A `regenerate` + build is needed afterwards.

import argparse
import collections
import glob
import multiprocessing
import os
import re
import subprocess
import tempfile

CC1 = "tools/gcc-2.8.1-psx/cc1"
IDENT = re.compile(r"[A-Za-z_]\w*")
# Linker-defined and compiler marker symbols, plus `.NON_MATCHING` aliases.
SKIP_SYM = re.compile(r"(_START|_END|_VRAM|_start|_end)$|^_binary_|^gcc2_compiled|^__gnu_compiled|\.")
# Covers the whole overlay load area, so everything in an overlay lands inside it.
SKIP_OUTER = {"g_MapOverlayHdr"}
SYM_LINE = re.compile(r"^(\s*)(\w+)(\s*=\s*)(0x[0-9A-Fa-f]+)(\s*;)(.*)$")


def elf_syms(elf):
    out = subprocess.run(["readelf", "-sW", elf], capture_output=True, text=True, check=True).stdout
    syms = []
    for line in out.splitlines():
        p = line.split()
        if len(p) < 8 or not p[0][:-1].isdigit() or p[3] in ("FILE", "SECTION") or p[6] == "UND":
            continue
        size = int(p[2], 16) if p[2].startswith("0x") else int(p[2])
        syms.append((int(p[1], 16), size, p[3], p[7]))
    return syms


def owner(ver, i_path):
    """ELF basename whose own TUs define `i_path`, or SHARED for main/bodyprog headers."""
    rel = os.path.relpath(i_path, f"build/{ver}/src").split(os.sep)
    if rel[0] in ("maps", "screens"):
        return rel[1].upper() + ".BIN.elf"
    return "SHARED"


def probe_sizes(args):
    """Returns {name: size} for every candidate that `sizeof` accepts in this TU."""
    i_path, data_names, tmp_dir = args
    src = open(i_path, errors="replace").read()
    cands = sorted(set(IDENT.findall(src)) & data_names)
    base = src.count("\n") + 1
    fd, fn = tempfile.mkstemp(suffix=".i", dir=tmp_dir)
    os.close(fd)
    result = {}
    try:
        # Undeclared names and incomplete types fail to compile; drop the failing lines and retry.
        for _ in range(20):
            if not cands:
                break
            probe = src + "\n" + "".join(f"char __sz__{n}[sizeof({n})];\n" for n in cands)
            open(fn, "w").write(probe)
            r = subprocess.run([CC1, "-quiet", "-O0", "-G0", "-funsigned-char", "-fcommon", fn, "-o", fn + ".s"],
                               capture_output=True, text=True)
            if r.returncode == 0:
                for m in re.finditer(r"\.l?comm\s+__sz__(\w+),(\d+)", open(fn + ".s").read()):
                    result[m.group(1)] = int(m.group(2))
                break
            bad = set()
            for m in re.finditer(re.escape(os.path.basename(fn)) + r":(\d+):", r.stderr):
                idx = int(m.group(1)) - base - 1
                if 0 <= idx < len(cands):
                    bad.add(cands[idx])
            if not bad:
                print(f"# skipped {i_path}: {r.stderr.strip().splitlines()[:1]}")
                break
            cands = [c for c in cands if c not in bad]
    finally:
        for f in (fn, fn + ".s"):
            if os.path.exists(f):
                os.remove(f)
    return i_path, result


def sym_file_elf(ver, sym_path, elfs):
    """Maps configs/<ver>/[maps|screens/]sym.<x>.txt to the ELF it describes."""
    name = os.path.basename(sym_path)[len("sym."):-len(".txt")]
    if name == "main":
        main = [e for e in elfs if os.path.dirname(e) == f"build/{ver}/out"]
        return main[0] if main else None
    for e in elfs:
        if os.path.basename(e) == name.upper() + ".BIN.elf":
            return e
    return None


def write_sym(ver, elfs, sized):
    """Adds `size:` to sym.txt entries that lack one. `sized` maps ELF path -> {name: size}."""
    total = 0
    for sym_path in sorted(glob.glob(f"configs/{ver}/**/sym.*.txt", recursive=True)):
        elf = sym_file_elf(ver, sym_path, elfs)
        if elf is None or elf not in sized:
            continue
        lines = open(sym_path).read().split("\n")
        changed = 0
        for i, line in enumerate(lines):
            m = SYM_LINE.match(line)
            if not m or m.group(2) not in sized[elf]:
                continue
            size = sized[elf][m.group(2)]
            existing = re.search(r"size:(0x[0-9A-Fa-f]+|\d+)", m.group(6))
            if existing:
                if int(existing.group(1), 0) != size:
                    print(f"SYM SIZE {sym_path}: {m.group(2)}: sym.txt 0x{int(existing.group(1), 0):X}, C 0x{size:X}")
                continue
            comment = m.group(6).rstrip()
            comment = f"{comment} size:0x{size:X}" if "//" in comment else f" // size:0x{size:X}"
            lines[i] = "".join(m.group(1, 2, 3, 4, 5)) + comment
            changed += 1
        if changed:
            open(sym_path, "w").write("\n".join(lines))
            print(f"# {sym_path}: {changed} sizes added")
            total += changed
    print(f"# {total} sizes added")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("version", nargs="?", default="USA")
    parser.add_argument("--write-sym", action="store_true", help="add size: to sym.txt entries without one")
    parser.add_argument("-j", type=int, default=os.cpu_count())
    args = parser.parse_args()
    ver = args.version

    elfs = {e: elf_syms(e) for e in sorted(glob.glob(f"build/{ver}/out/**/*.elf", recursive=True))}
    funcs = {n for s in elfs.values() for _, _, t, n in s if t == "FUNC"}
    data_names = {n for s in elfs.values() for _, _, t, n in s if t != "FUNC"} - funcs

    i_paths = [p for p in glob.glob(f"build/{ver}/src/**/*.i", recursive=True) if not p.endswith(".sjis.i")]
    sizes = collections.defaultdict(lambda: collections.defaultdict(dict))  # owner -> name -> {tu: size}
    with tempfile.TemporaryDirectory() as tmp_dir, multiprocessing.Pool(args.j) as pool:
        for i_path, result in pool.imap_unordered(probe_sizes, [(p, data_names, tmp_dir) for p in i_paths]):
            for n, sz in result.items():
                sizes[owner(ver, i_path)][n][i_path] = sz

    # Overlays reference main/bodyprog addresses that those ELFs never define themselves, so symbols
    # in main/bodyprog are checked against every ELF's symbols.
    all_data = {(a, n) for syms in elfs.values() for a, _, t, n in syms if t != "FUNC" and not SKIP_SYM.search(n)}
    # lib_externs.ld is linked into every ELF; its overlay-range entries only apply to one map each.
    lib_externs = set()
    if os.path.exists(f"configs/{ver}/lib_externs.ld"):
        lib_externs = set(re.findall(r"^\s*(\w+)\s*=", open(f"configs/{ver}/lib_externs.ld").read(), re.M))
    sized = {}
    for elf, syms in elfs.items():
        own, shared = sizes.get(os.path.basename(elf), {}), sizes["SHARED"]
        own_data = sorted({(a, n) for a, _, t, n in syms if t != "FUNC" and not SKIP_SYM.search(n)})
        if os.path.dirname(elf) == f"build/{ver}/out" or os.path.basename(elf) == "BODYPROG.BIN.elf":
            at = sorted(all_data)
        else:
            at = [(a, n) for a, n in own_data if n not in lib_externs]
        ok = {}
        for a, n in own_data:
            tus = own.get(n) or shared.get(n)
            if not tus or n in SKIP_OUTER:
                continue
            elf_name = os.path.basename(elf)
            if len(set(tus.values())) > 1:
                print(f"CONFLICT {elf_name}: {n}: " +
                      ", ".join(f"0x{z:X} ({os.path.basename(t)})" for t, z in sorted(tus.items())))
                continue
            sz = next(iter(tus.values()))
            inner = sorted({(b, m) for b, m in at if a < b < a + sz and m != n})
            if inner:
                print(f"{elf_name}: {n} @{a:08X} size 0x{sz:X}: " +
                      ", ".join(f"{m}(+0x{b - a:X})" for b, m in inner))
            else:
                ok[n] = sz
        sized[elf] = ok

    if args.write_sym:
        write_sym(ver, list(elfs), sized)


if __name__ == "__main__":
    main()
