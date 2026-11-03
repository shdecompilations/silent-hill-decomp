# Silent Hill PC Port — Recompilation

This directory contains a **native PC recompilation** of the Silent Hill
decompilation. It consumes the same `src/` + `include/` as the PSX matching
build, but retargets them to x86_64 via CMake + SDL2 + bgfx/OpenGL.

> **Invariant:** `make GAME_VERSION=USA build` must still produce a
> byte-identical PSX binary (`checksum.sha` passes). The PC port is a
> *recompilation*, not a fork of the decompilation.

## Architecture

```
src/main/*.c ──────────┐
src/bodyprog/**/*.c ───┤
src/maps/**/ *.c ──────┤──► pc/src/core/overlay.c ──► dynamic loader (no fixed VRAM)
src/screens/**/*.c ────┤
                       │
pc/src/gpu/shim.c ─────┼──► bgfx / OpenGL (replaces libgpu / libgs / libgte)
pc/src/audio/shim.c ───┼──► SDL_mixer / miniaudio (replaces libspu / libsd)
pc/src/fs/pc_fs.c ──────┼──► POSIX file loader (replaces Fs_Queue + CD)
pc/src/core/main.c ────┼──► SDL event loop (replaces VSync / Pad)
pc/include/psyq/*.h ───┘    shims that shadow include/psyq/*.h
```

## Shiftability

The PSX build hardcodes VRAM:

- `0x80010000` — main executable
- `0x80024B60` — BODYPROG overlay (`g_OvlBodyprog`)
- `0x800C9578` — map overlays (`MAP*_S*.BIN`)
- `0x801E2600` — screen overlays

These are **not shiftable**: moving a single function breaks the map.

The PC port adds `SH_SHIFTABLE`:

- All `g_OvlDynamic` / `g_Ovl*` become heap pointers, patched at load time.
- `splat`'s `gp_value` / global VRAM assumptions are `#ifdef PC_PORT`'d out.
- Relocations in `linkers/USA/*.ld` are replaced by runtime fixups in
  `pc/src/core/reloc.c` (pointer swizzling from file offsets → heap addresses).

See `pc/include/pc/core/shiftable.h` and `docs/SHIFTABILITY.md`.

## Data Matching

Matched data is a prerequisite for shiftability — you cannot relocate a blob
you cannot type. Current status (`python3 tools/data_audit.py --detail`):

- `main` — 100% matched
- `bodyprog` — ~90% (9 raw rodata pads left, mostly padding)
- `screens` — ~92%
- `maps` — 66% (133 raw segments: 1 rodata + 2 data per map on average)

Each map has two trailing `data` segments that are still raw:

```
[0x0, rodata]          # jumptable + .rodata strings — should be .rodata in C
[0x1400C, data]        # collision/terrain tables — typed via bin2c
[0x1653C, data]        # blood splats / particle tables — typed via bin2c
```

Workflow to match one segment:

```bash
# 1. Extract the overlay's raw bytes (needs a valid ROM + `make extract`)
python3 tools/bin2c.py @tools/bin2c.cfg \
  -b assets/USA/VIN/MAP0_S00.BIN -o 0x1400C -s s_MapCollisionData \
  --use-cpp -p build/USA/src/maps/map0_s00/header.c.i

# 2. Paste the emitted initializer into src/maps/map0_s00/header.c,
#    replace the `data` entry in configs/USA/maps/map0_s00.yaml with
#    `.data, header` (or appropriate C file).

# 3. Rebuild PSX and verify checksum
make GAME_VERSION=USA build
```

An example migration is in `pc/examples/data_migration.md` and
`src/maps/map0_s00/header.c` already shows the target form.

## Building (PC)

Prereqs: CMake ≥3.20, SDL2, OpenGL, Python 3 + splat (for asset extraction).

```bash
# One-time: extract assets from retail disc (requires image)
make setup GAME_VERSION=USA   # needs rom/image/SLUS-00707.bin present

# PC build
cmake -S pc -B build-pc -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build-pc -j
./build-pc/silenthill --assets assets/USA --rom rom/USA
```

Renderer backends:

- Default: SDL2 + OpenGL 3.3 (`-DSH_USE_BGFX=OFF`)
- Optional: bgfx (Vulkan/D3D12/Metal) with `-DSH_USE_BGFX=ON`

## File Loading

On PSX, `Fs_Queue` reads sectors from `SILENT.`/`HILL.` via CD.
On PC, `pc/src/fs/pc_fs.c` maps `e_FsFile` indices to extracted files under
`assets/USA/` — same layout that `tools/silentassets/extract.py` produces.

No disc image is needed at runtime, only the extracted `assets/` tree.

## Controls

| PSX | PC |
|-----|----|
| D-Pad | WASD / Arrow keys |
| X | Space / Enter |
| O | Esc |
| Start | P |
| L1/R1 | Q/E |

Config is `~/.config/silenthill/input.ini`.

## Known gaps

- GTE (geometry transform) is CPU-emulated; no fixed-point fast path yet.
- SPU reverb is stubbed — bgm plays, but 3D panning is mono.
- Savegames use the original PS1 memory-card format under
  `~/.local/share/silenthill/save/`.

See `docs/PC_PORT_TODO.md` for full list.
