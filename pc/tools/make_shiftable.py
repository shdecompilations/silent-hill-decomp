#!/usr/bin/env python3
"""
make_shiftable.py — replace hardcoded VRAM addresses with sh_overlay_base() calls.

Usage:
  python3 pc/tools/make_shiftable.py --check src/bodyprog/sys/game_main.c
  python3 pc/tools/make_shiftable.py --fix src/bodyprog/sys/game_main.c
"""
import re, sys, glob, pathlib
HARDCODED = {
    "0x80010000": "sh_overlay_base(PcOverlay_Main)",
    "0x80024B60": "sh_overlay_base(PcOverlay_Bodyprog)",
    "0x80024B6":  "sh_overlay_base(PcOverlay_Bodyprog)",  # splat short form
    "0x800C9578": "sh_overlay_base(PcOverlay_Map0_S00)", # generic map base
    "0x801E2600": "sh_overlay_base(PcOverlay_Stream)",
    "0x801E260":  "sh_overlay_base(PcOverlay_Stream)",
}
def process(text): return re.sub(r'0x800(?:10000|24B60|C9578|1E2600)\b', lambda m: HARDCODED.get(m.group(0), m.group(0)), text)
if __name__ == "__main__":
    import argparse
    ap=argparse.ArgumentParser()
    ap.add_argument("--check", action="store_true")
    ap.add_argument("--fix", action="store_true")
    ap.add_argument("files", nargs="*")
    a=ap.parse_args()
    files = a.files or glob.glob("src/**/*.c", recursive=True)
    for f in files:
        try:
            t=open(f).read()
        except: continue
        if any(k in t for k in HARDCODED):
            if a.check: print(f"{f}: hardcoded VRAM")
            if a.fix:
                open(f,"w").write(process(t))
                print(f"fixed {f}")
