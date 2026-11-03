#!/usr/bin/env python3
"""
Data-match audit: reports matched vs raw data per overlay.
Mirrors decomp.dev's matched_data_percent by counting splat YAML segments.

Usage:
  python3 tools/data_audit.py [--detail]
  python3 tools/data_audit.py --overlay map0_s00
"""

import glob, re, os, sys, json
import argparse

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

def analyze_yaml(path):
    text = open(path).read()
    # Count segments
    raw_data = len(re.findall(r'- \[0x[0-9A-Fa-f]+,\s*data\]', text))
    raw_rodata = len(re.findall(r'- \[0x[0-9A-Fa-f]+,\s*rodata\]', text))
    matched_data = len(re.findall(r'- \[0x[0-9A-Fa-f]+,\s*\.data,', text))
    matched_rodata = len(re.findall(r'- \[0x[0-9A-Fa-f]+,\s*\.rodata,', text))
    c_funcs = len(re.findall(r'- \[.*?, c,', text))
    o_libs = len(re.findall(r'- \[.*?, o,', text))
    # Extract segment sizes where possible
    # Section order
    # Rough size estimate: next start - current start
    segments = []
    for m in re.finditer(r'- \[(0x[0-9A-Fa-f]+|auto),?\s*([^]]*)\]', text):
        addr = m.group(1)
        rest = m.group(2)
        segments.append((addr, rest.strip()))
    return {
        'raw_data': raw_data,
        'raw_rodata': raw_rodata,
        'matched_data': matched_data,
        'matched_rodata': matched_rodata,
        'c_funcs': c_funcs,
        'o_libs': o_libs,
        'segments': segments,
    }

def category_progress(cat, files):
    total_raw = sum(analyze_yaml(f)['raw_data'] + analyze_yaml(f)['raw_rodata'] for f in files)
    total_matched = sum(analyze_yaml(f)['matched_data'] + analyze_yaml(f)['matched_rodata'] for f in files)
    # crude percent: matched / (matched+raw)
    denom = total_raw + total_matched
    pct = (total_matched / denom * 100) if denom else 100
    return pct, total_raw, total_matched

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--detail', action='store_true')
    ap.add_argument('--overlay', type=str, default=None)
    ap.add_argument('--json', action='store_true')
    args = ap.parse_args()

    configs = {
        'main': glob.glob(f'{ROOT}/configs/USA/main.yaml'),
        'bodyprog': glob.glob(f'{ROOT}/configs/USA/bodyprog.yaml'),
        'screens': glob.glob(f'{ROOT}/configs/USA/screens/*.yaml'),
        'maps': glob.glob(f'{ROOT}/configs/USA/maps/*.yaml'),
        'eur_maps': glob.glob(f'{ROOT}/configs/EUR/maps/*.yaml'),
    }

    report = {}
    for cat, files in configs.items():
        pct, raw, matched = category_progress(cat, files)
        report[cat] = {'pct': pct, 'raw': raw, 'matched': matched, 'files': len(files)}

    # Per-map detail
    per_map = {}
    for f in configs['maps']:
        name = os.path.splitext(os.path.basename(f))[0]
        info = analyze_yaml(f)
        denom = info['raw_data'] + info['raw_rodata'] + info['matched_data'] + info['matched_rodata']
        pct = (info['matched_data']+info['matched_rodata'])/denom*100 if denom else 100
        per_map[name] = {
            **info,
            'pct': pct,
            'unmatched_segments': [s for s in info['segments'] if re.match(r'data|rodata', s[1]) and '.data' not in s[1] and '.rodata' not in s[1]]
        }

    if args.overlay:
        name = args.overlay
        path = f'{ROOT}/configs/USA/maps/{name}.yaml'
        if not os.path.exists(path):
            path = f'{ROOT}/configs/USA/screens/{name}.yaml'
        if not os.path.exists(path):
            path = f'{ROOT}/configs/USA/{name}.yaml'
        if not os.path.exists(path):
            print(f"Overlay {name} not found")
            sys.exit(1)
        info = analyze_yaml(path)
        print(f"=== {name} ===")
        print(json.dumps(info, indent=2))
        # Show unmatched segments with surrounding context
        text = open(path).read()
        for line in text.splitlines():
            if re.search(r'- \[0x[0-9A-Fa-f]+,\s*(data|rodata)\]', line):
                print(line.strip())
        return

    if args.json:
        print(json.dumps({'categories': report, 'per_map': per_map}, indent=2))
        return

    print("Silent Hill Data-Match Audit")
    print("============================")
    for cat, r in report.items():
        print(f"{cat:10s} {r['pct']:5.1f}%  matched {r['matched']:3d} raw {r['raw']:3d}  files {r['files']}")

    print("\nPer-map data match (worst first):")
    sorted_maps = sorted(per_map.items(), key=lambda x: x[1]['pct'])
    for name, info in sorted_maps[:20]:
        print(f"  {name:12s} {info['pct']:5.1f}%  raw {info['raw_data']+info['raw_rodata']:2d} matched {info['matched_data']+info['matched_rodata']:2d} c {info['c_funcs']:2d}")
    if args.detail:
        print("\nFully unmatched segments (raw data/rodata) — these need migration to C:")
        for name, info in sorted_maps:
            if info['unmatched_segments']:
                print(f"\n{name}:")
                for addr, rest in info['unmatched_segments']:
                    print(f"  [{addr}, {rest}]")

        # Also check bodyprog
        print("\n--- bodyprog raw segments ---")
        for f in configs['bodyprog']:
            text = open(f).read()
            for line in text.splitlines():
                if re.search(r'- \[0x[0-9A-Fa-f]+,\s*(data|rodata|pad)\]', line):
                    # only show raw (no dot)
                    if '.data' not in line and '.rodata' not in line and ' c,' not in line and ' o,' not in line:
                        print(line.strip())

    # Recommendations
    print("\nRecommendations:")
    print("- Each 'data'/'rodata' raw segment is a binary blob not yet in C. Migrate with tools/bin2c.py")
    print("- Use `python3 tools/bin2c.py @tools/bin2c.cfg -b assets/... -o <offset> -t <struct> --use-cpp`")
    print("- For map overlays, raw segments at 0x1400C and 0x1653C are typically collision/header tables")
    print("- BODYPROG raw rodata at 0x3FDC, 0x694, 0x6D4 etc are padding; model as `const u8 PAD_xxx[SIZE]` to preserve alignment")

if __name__ == '__main__':
    main()
