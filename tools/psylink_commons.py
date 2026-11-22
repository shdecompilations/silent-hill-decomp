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
# global of that object an address in declaration order (the order that our decomp/gcc
# emits them, header externs included). The original names psylink hashed aren't known,
# so this lets us control order using our declarations in the source instead.

import argparse
import re
import sys

from elftools.elf.elffile import ELFFile

BSS_LINE = re.compile(r"^(\s*)(\S+\.o)\(\.bss\);\s*$")
BSS_START_LINE = re.compile(r"^\s*(\w+_BSS_START) = \.;")
COMM_LINE = re.compile(r"^\s*\.comm\s+([\w.$]+)\s*,\s*(\d+)")


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


def declaration_order(path, commons):
    """Orders an object's COMMON symbols as gcc emitted them in its .c.s."""
    asm = re.sub(r"\.o$", ".s", path)
    order = []
    for line in open(asm):
        m = COMM_LINE.match(line)
        if m and m.group(1) in commons and m.group(1) not in order:
            order.append(m.group(1))
    missing = set(commons) - set(order)
    if missing:
        sys.exit(f"{asm}: no .comm line for {', '.join(sorted(missing))}")
    return order


def main():
    ap = argparse.ArgumentParser(description="Write a linker script placing COMMON symbols.")
    ap.add_argument("input", help="splat linker script")
    ap.add_argument("output")
    ap.add_argument("objects", nargs="+", help="objects assembled with --use-comm-section")
    args = ap.parse_args()

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
        for name in declaration_order(path, commons):
            size = commons[name]
            out.append(f"{indent}{name} = ALIGN({alignment(size)}); . = {name} + {size:#x};\n")
            placed.add(name)
    with open(args.output, "w") as f:
        f.writelines(out)


if __name__ == "__main__":
    main()
