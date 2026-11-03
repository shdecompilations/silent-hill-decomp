# Data Matching Guide — Silent Hill Decomp

This documents the **matched data** workflow: converting raw `data`/`rodata`
blobs in splat YAMLs into typed C that still produces a matching PSX binary.

> Matched data is distinct from matched code. `tools/data_audit.py` tracks it
> per overlay; decomp.dev tracks code. Both must be 100% for a fully
> documented, shiftable codebase.

---

## Current Status

Run:

```bash
python3 tools/data_audit.py --detail
python3 tools/data_audit.py --json | jq .categories
```

As of 2026-11-03:

| Category | Matched | Raw | % | Notes |
|----------|---------|-----|---|-------|
| `main` | 5 | 0 | 100% | Fully matched — `fileinfo`, `main`, `libsn` |
| `bodyprog` | 81 | 9 | 90% | 9 raw rodata pads (garbage bytes at 0x138, 0x694, etc.) |
| `screens` | 11 | 1 | 92% | `B_KONAMI` footer_data (0xF9C bin) |
| `maps` | 259 | 133 | 66% | Every map has 1 rodata + 2 data raw blobs |
| `EUR maps` | 60 | 276 | 18% | Not yet ported from USA |

### Per-map breakdown (worst first)

```
map1_s05  60.0%  raw 4 matched 6
map3_s06  60.0%  raw 4 matched 6
map4_s05  60.0%  raw 4 matched 6
map5_s01  60.0%  raw 4 matched 6
map0_s00  66.7%  raw 3 matched 6  ← example used below
...
```

---

## What “raw” means

In `configs/USA/maps/map0_s00.yaml`:

```yaml
segments:
  - [0x0, rodata]          # ← raw: binary blob, not in C
  - [0x4, .rodata, header] # ← matched: in src/maps/map0_s00/header.c
  - [0x1400C, data]        # ← raw: collision/terrain tables
  - [0x1653C, data]        # ← raw: blood/particle tables
```

Raw segments are emitted as `.bin` in `build/` and linked verbatim.
Matched segments are compiled from `src/` and must be **byte-identical** to pass
`checksum.sha`.

---

## Workflow to Match a Segment

### 1. Identify the struct

Check `include/maps/map0/map0_s00.h` and `include/maps/shared.h` for existing
types. Most map `data` blobs are:

- `s_MapOverlayHdr` (the `g_MapOverlayHdr` at `0x800DF310`)
- `s_MapInfo` / `s_MapPoint2d[]` / `s_CollisionTrigger[]`
- `s_AnimInfo[]` (per-map Harry anim overrides)
- `s_BloodSplat[]` / `s_Particle*`

If no struct exists, look at the map’s `header.c` — it already references
symbols like `MAP_POINTS`, `MAP_EVENTS`, `g_MapHeaderTable_38`.

### 2. Extract with `bin2c.py`

`tools/bin2c.py` reads the original overlay binary and a C struct definition,
then emits an initializer.

```bash
# Ensure assets are extracted
make extract GAME_VERSION=USA   # needs rom/image/SLUS-00707.bin present

# Generate a context file for the struct (or use an existing .i)
python3 tools/m2ctx.py src/maps/map0_s00/header.c > /tmp/ctx.c

# Run bin2c — example for the 0x1400C data blob
python3 tools/bin2c.py \
  @tools/bin2c.cfg \
  -s /tmp/ctx.c \
  -b assets/USA/VIN/MAP0_S00.BIN \
  -o 0x1400C \
  -t s_MapCollisionData \
  --use-cpp \
  -o /tmp/out.c
```

Check `tools/bin2c.cfg` for default type mappings (e.g. `q19_12 → s32` with
`Q12()` wrappers).

### 3. Move into the source tree

Paste the output into the appropriate file:

```c
// src/maps/map0_s00/header.c or a new src/maps/map0_s00/collision_data.c
#include "maps/map0/map0_s00.h"

s_MapCollisionData MAP_COLLISION_DATA = {
    // emitted initializer
};

const u8 PAD_0x1400C[0x252C] = {0}; // if padding is needed to preserve layout
```

### 4. Update the YAML

In `configs/USA/maps/map0_s00.yaml`, replace the raw entry:

```yaml
# before
- [0x1400C, data]

# after
- [0x1400C, .data, header]   # or .data, collision_data if you split the file
```

If the blob contained only padding/garbage, model it as:

```c
// src/maps/map0_s00/padding.c
const u8 MAP0_S00_PAD_0x1400C[0x252C] = {0};
```

and mark it `pad` in YAML (does not need to match if it was garbage — see
`tools/postbuild.py` MAP_PATCHES).

### 5. Rebuild and verify

```bash
make GAME_VERSION=USA generate   # regenerate linkers from YAML
make GAME_VERSION=USA build CHECKSUM=1
# should print: 46 ok (or similar) — all hashes in configs/USA/checksum.sha pass
```

If it fails, the emitted data differs by a few bytes. Common causes:

- Missing `SECTION(".data")` or wrong alignment (`subalign: 4` in YAML)
- Endianness: PSX is little-endian; bin2c defaults to LE, verify with `-e le`
- Bitfields: `s_FileInfo` uses 4/24-bit bitfields that need explicit `PACKED`
- Strings: Shift-JIS vs ASCII — use `MAP_TEXT_ENCODING` (set per version)

---

## Example: `map0_s00` Header Migration

`src/maps/map0_s00/header.c` already shows the target. It had:

```yaml
- [0x1400C, data]   # was raw
- [0x15D98, .data, header]
```

After migration:

```yaml
- [0x1400C, .data, header]  # now in C
- [0x15D98, .data, header]  # continues
```

In `header.c`, the `g_MapOverlayHdr` initializer references all major tables:

```c
const s_MapOverlayHdr g_MapOverlayHdr = {
    .mapInfo                    = &MAP_INFOS[MapType_THR],
    .mapPoints                  = MAP_POINTS,
    .mapEvents                  = MAP_EVENTS,
    .loadableItems              = LOADABLE_INVENTORY_ITEMS,
    .mapMessages                = MAP_MESSAGES,
    .harryMapAnimInfos          = HARRY_M0S00_ANIM_INFOS,
    .field_38                   = g_MapHeaderTable_38,
    .unkTable1_4C               = sharedData_800DFB7C_0_s00,
    .bloodSplats                = g_Effect_BloodSplats,
    // ...
};
```

Each pointer field is a separate `.data` object that was previously a raw
blob. The migration makes the overlay **shiftable** — `MAP_POINTS` can be
moved without manually updating every pointer.

See `pc/examples/data_migration.md` for a before/after diff.

---

## BODYPROG Raw Segments

Bodyprog’s 9 raw `rodata` pads (0x138, 0x694, etc.) are **true garbage** —
leftovers from the linker that don’t affect runtime. They are listed in
`tools/postbuild.py` as `BODYPROG_PATCHES` and can be modeled as:

```c
// src/bodyprog/padding.c
const u8 BODYPROG_PAD_0x138[0xC] = {0};
```

Mark them `pad` in YAML to exclude from matching, or keep them as `rodata`
with a bin fallback until all real data is matched.

---

## Shiftability Checklist

A file is shiftable when:

- [ ] No `INCLUDE_ASM` / `INCLUDE_RODATA` — all code and data in C
- [ ] No `SECTION(".rodata")` with hardcoded `0x800XXXXX` pointer literals
- [ ] YAML uses `auto` for `.text` addresses where possible (lets splat pack)
- [ ] `gp_value` does not need to change when adding a field
- [ ] `make build` with `NON_MATCHING=1 SKIP_ASM=1` still links

Run:

```bash
make GAME_VERSION=USA NON_MATCHING=1 SKIP_ASM=1 build
```

for a quick shiftability smoke test.

---

## Tool Reference

- `tools/data_audit.py` — overview of raw vs matched per overlay
- `tools/bin2c.py` — generate C initializers from binary blobs
- `tools/bin2c.cfg` — default mappings (Q formats, fixed-point helpers)
- `tools/naming_progress.py` — per-file naming progress (auto vs named)
- `tools/port_map.py` — port USA splits to EUR/JAP, also usable for data
- `tools/postbuild.py` — garbage patches that must be kept in sync with YAML
