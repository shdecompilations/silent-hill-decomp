# Data Migration Example — `map0_s00` 0x1400C

This shows a minimal raw → typed migration. The original `configs/USA/maps/map0_s00.yaml`
had two trailing untyped blobs:

```yaml
# BEFORE
- [0x13D44, .data, map0_s00_anim_info]
- [0x1400C, data]                 # ← untyped, 0x1D8C bytes
- [0x15D98, .data, header]
- [0x16140, .data, map0_s00_2]
- [0x1653C, data]                # ← untyped, 0x3C4 bytes
```

These are the map’s `s_MapHdr_field_4C` table and `s_BloodSplat` pool.

## Step 1 — Identify

`src/maps/map0_s00/header.c` already declares:

```c
extern s_MapHdr_field_4C sharedData_800DFB7C_0_s00[MAP_FIELD_4C_COUNT]; // 200 × 12 bytes = 0x960
extern s_BloodSplat g_Effect_BloodSplats[MAP_BLOOD_SPLAT_COUNT_MAX];   // 150 × 16 bytes = 0x960
```

And `include/maps/map0/map0_s00.h` defines:

```c
#define MAP_FIELD_4C_COUNT 200
#define MAP_BLOOD_SPLAT_COUNT_MAX 150
#include "maps/shared.h" // s_MapHdr_field_4C, s_BloodSplat
```

The sizes don’t add up: `0x960 + 0x960 = 0x12C0`, but the first raw blob is
`0x15D98 - 0x1400C = 0x1D8C`. The remainder is the `s_UnkStruct3_Mo` table that
is already in `map0_s00_anim_info.c` as `g_MapHeaderTable_38`.

So the first blob is actually two arrays back-to-back.

## Step 2 — Extract with bin2c

```bash
python3 tools/bin2c.py @tools/bin2c.cfg \
  -s src/maps/map0_s00/header.c \
  -b assets/USA/VIN/MAP0_S00.BIN -o 0x1400C -t s_MapHdr_field_4C \
  --array 200 --use-cpp > /tmp/a.c

python3 tools/bin2c.py @tools/bin2c.cfg \
  -s src/maps/map0_s00/header.c \
  -b assets/USA/VIN/MAP0_S00.BIN -o 0x1496C -t s_BloodSplat \
  --array 150 --use-cpp > /tmp/b.c
```

(The 0x1496C offset is 0x1400C + 200×sizeof(s_MapHdr_field_4C).)

Output (trimmed):

```c
s_MapHdr_field_4C sharedData_800DFB7C_0_s00[200] = {
    { .flags = 0x01, .field_1 = 0x00, .field_8 = Q12(0.0f), ... },
    { .flags = 0x02, ... },
    // …
};

s_BloodSplat g_Effect_BloodSplats[150] = {
    { .pos = { Q12(-10.5f), Q12(0.0f), Q12(5.2f) }, .state = 0 },
    // …
};
```

## Step 3 — Move into source

Create `src/maps/map0_s00/map0_s00_tables.c`:

```c
#include "maps/map0/map0_s00.h"
#include "maps/shared.h"

s_MapHdr_field_4C sharedData_800DFB7C_0_s00[MAP_FIELD_4C_COUNT] = {
#include "map0_s00_field_4C.inc" // generated
};

s_BloodSplat g_Effect_BloodSplats[MAP_BLOOD_SPLAT_COUNT_MAX] = {
#include "map0_s00_blood.inc"
};
```

## Step 4 — Update YAML

```yaml
# AFTER
- [0x13D44, .data, map0_s00_anim_info]
- [0x1400C, .data, map0_s00_tables]   # ← now typed
- [0x15D98, .data, header]
- [0x16140, .data, map0_s00_2]
- [0x161DC, .data, map_events]
- [0x1653C, .data, map0_s00_tables]  # ← if it held the second table, split similarly
```

Or if `0x1653C` was actually padding:

```yaml
- [0x1653C, pad]  # exclude from matching
```

## Result

```
$ python3 tools/data_audit.py --overlay map0_s00
raw_data 1 raw_rodata 1 matched_data 6 → raw_data 0 raw_rodata 1 matched_data 8
pct 66% → 88%
```

The remaining `rodata` is the jumptable at 0x0, which is handled separately
via `jlabel` macros and does not need to be typed.

---

## Checklist for reviewers

- [ ] New `.inc` files are listed in `src/` and included with correct `Q12()` etc.
- [ ] YAML `ld_script_path` does not hardcode the old `0x1400C` size
- [ ] `make GAME_VERSION=USA CHECKSUM=1 build` passes
- [ ] `make GAME_VERSION=USA NON_MATCHING=1 SKIP_ASM=1 build` also passes (shiftable)
