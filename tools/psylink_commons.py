#!/usr/bin/env python3

# Places uninitialised COMMON globals in .bss into the linker script directly, for the 
# targets in TARGET_COMMONS (Makefile).
#
# In the original PsyQ build-env the assembler placed each object's statics (`.lcomm`),
# aligned to 16 bytes. Globals were left as COMMON symbols (`.comm`) for the psylink 
# linker, which started each object's .bss on an 8-byte boundary and placed its
# globals right after the object's statics, aligned to 8 bytes.
#
# psylink ordered those by a hash of the name (its length plus the sum of its characters,
# mod 512), while GNU ld allocates COMMON symbols in a separate order of its own instead.
#
# maspsx can handle statics for us, but GNU ld can't place globals like psylink did, so 
# these targets assemble globals as COMMON symbols (`--use-comm-section`) and this script
# then handles placing them into the linker script, preventing GNU ld from reordering them.
#
# In the linker script from splat, after each object's `(.bss)` line, we assign every
# global of that object an address, ordered by its original address in the target's
# symbol files. The original names psylink hashed aren't known, so the symbol files
# stand in for that order.

import argparse
import re
import sys

import yaml
from elftools.elf.elffile import ELFFile

BSS_LINE = re.compile(r"^(\s*)(\S+\.o)\(\.bss\);\s*$")
BSS_START_LINE = re.compile(r"^\s*(\w+_BSS_START) = \.;")
SYM_LINE = re.compile(r"^\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+)\s*;")


def alignment(size):
    for a in (8, 4, 2):
        if size >= a:
            return a
    return 1


def read_object(path):
    """Returns the object's .bss size and {name: size} for its COMMON symbols."""
    commons = {}
    with open(path, "rb") as f:
        elf = ELFFile(f)
        bss = elf.get_section_by_name(".bss")
        for sym in elf.get_section_by_name(".symtab").iter_symbols():
            if sym["st_shndx"] == "SHN_COMMON":
                commons[sym.name] = sym["st_size"]
    return (bss["sh_size"] if bss else 0), commons


def read_addresses(yaml_path):
    """Returns {name: [addresses]} from the symbol files a splat yaml lists."""
    with open(yaml_path) as f:
        paths = yaml.safe_load(f)["options"]["symbol_addrs_path"]
    if isinstance(paths, str):
        paths = [paths]
    addrs = {}
    for path in paths:
        for line in open(path):
            m = SYM_LINE.match(line)
            if m:
                addrs.setdefault(m.group(1), set()).add(int(m.group(2), 16))
    return addrs


def main():
    ap = argparse.ArgumentParser(description="Write a linker script placing COMMON symbols.")
    ap.add_argument("input", help="splat linker script")
    ap.add_argument("output")
    ap.add_argument("objects", nargs="+", help="objects assembled with --use-comm-section")
    ap.add_argument("--yaml", required=True, help="splat yaml whose symbol files give the order")
    args = ap.parse_args()

    addrs = read_addresses(args.yaml)
    errors = []

    placed = set()
    out = []
    bss_start = None
    first = False
    for line in open(args.input):
        m = BSS_START_LINE.match(line)
        if m:
            bss_start, first = m.group(1), True
        m = BSS_LINE.match(line)
        if not m or m.group(2) not in args.objects:
            out.append(line)
            first = first and not m
            continue
        indent, path = m.groups()
        bss_size, commons = read_object(path)
        commons = {n: s for n, s in commons.items() if n not in placed}
        if not first and (bss_size or commons):
            out.append(f"{indent}. = {bss_start} + ALIGN(. - {bss_start}, 8);\n")
        first = False
        out.append(line)
        for name in commons:
            if len(addrs.get(name, ())) != 1:
                why = "no symbol file entry" if name not in addrs else "several addresses"
                errors.append(f"{path}: `{name}` has {why}")
        known = [n for n in commons if len(addrs.get(n, ())) == 1]
        for name in sorted(known, key=lambda n: min(addrs[n])):
            size = commons[name]
            out.append(f"{indent}{name} = ALIGN({alignment(size)}); . = {name} + {size:#x};\n")
            placed.add(name)
    if errors:
        sys.exit("\n".join(errors))
    with open(args.output, "w") as f:
        f.writelines(out)


if __name__ == "__main__":
    main()
