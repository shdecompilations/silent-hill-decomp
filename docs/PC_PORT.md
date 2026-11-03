# PC Port — Design & Remaining Work

This is the companion to `pc/README.md`. It details the recompilation
architecture and what is still TODO.

## Why a separate `pc/` directory?

The PSX build is **matching**: `make build` must produce a byte-identical
`SLUS_007.07` / `BODYPROG.BIN` / `MAP*.BIN`. That build uses `mips-linux-gnu`
+ PSY-Q 4.4 + hard VRAM `0x800C9578` and cannot be made portable without
breaking the checksum.

The PC port is **recompilation**: same `src/` + `include/` but compiled with
host `gcc`/`clang`, `PC_PORT=1`, and PSX lib shims. It lives in `pc/` so:

- `Makefile` and `ninja_config.py` are untouched — PSX matching is not at risk.
- `pc/CMakeLists.txt` is the only build entry for PC.
- PSX-specific files under `lib/` (assembly `libapi.a` etc.) are never linked
  on PC; `pc/src/gpu/*.c` provides replacements.

Invariant: `make GAME_VERSION=USA CHECKSUM=1 build` still passes after any PC
change. CI enforces this (`pc-port.yml` runs both builds).

## Module Map

```
include/           # shared headers — must compile on both PSX and PC
  game.h           # s_SysWork, s_GameWork — already portable
  bodyprog/*.h     # most engine types are portable
  maps/shared.h    # map overlay ABI — the contract pc/ must honor

pc/include/
  pc_compat.h          # -include forced: SECTION() → empty, PSX_SCRATCH → 0
  pc/core/shiftable.h  # g_PcOverlays[PcOverlay_Count], sh_ptr_vram_to_host()
  pc/gpu/shim.h        # PcGfx_* — translates DrawPrim → bgfx/OpenGL
  pc/audio/shim.h      # PcAudio_* — replaces SPU
  pc/fs/pc_fs.h        # PcFs_* — replaces Fs_Queue + CD

pc/src/
  core/main.c          # SDL main loop — calls MainLoop() from game_main.c
  core/overlay.c       # PcOverlay_Load() + sh_overlay_fixup() (reloc)
  core/input.c         # SDL keyboard/pad → s_ControllerData
  gpu/shim.c           # GTE CPU emu + OT flushing
  audio/shim.c         # SDL_mixer stub
  fs/pc_fs.c           # assets/USA/ loader

tools/
  data_audit.py        # matched_data audit (66% → 100% roadmap)
  bin2c.py             # binary → C struct initializer
  make_shiftable.py    # replace 0x800XXXXX with sh_overlay_base()
```

## Rendering — SDL2 + bgfx

The PSX GPU is immediate-mode: game builds an ordering table (OT) of
`POLY_FT4`/`SPRT`/`TILE` in scratchpad, then `DrawOTag()` flushes it.

On PC we keep the OT API but back it with a vector (`g_PcOt[4096]`) and
flush in `PcGfx_EndFrame()`:

- **bgfx path** (`-DSH_USE_BGFX=ON`): each OT entry becomes a bgfx
  `bgfx::TransientVertexBuffer` draw with a shader that emulates PSX
  semi-transparency (`abr`) and CLUT/TPage.
- **GL path** (default): `SDL_Renderer` for menus + raw GL for 3D.
  TIM → `s_PcTexture` cache keyed by `(tpage, clut)`.

GTE (cop2) is the hard part: the PSX has a fixed-point geometry coprocessor;
all vertex transforms go through `gte_ldv0`/`gte_rtps`. On PC we CPU-emulate
with the same 12-bit fractional math (`q19_12`, `q3_12`) so that fixed-point
behavior matches. See `pc/src/gpu/shim.c: PcGte_RotTransPers()`.

## Audio — libspu / libsd → SDL

- VAB (sample banks like `1ST/BASE.VAB`) are loaded via `PcVab_Load()` and
  decoded to PCM.
- KDT (music) is parsed and fed to SDL_mixer as MIDI + soundfont, or to
  miniaudio. The SFX ID enum (`sfx_id_enum.h`) is unchanged.
- XA (FMV audio) is streamed from `HILL.` sectors — on PC we read the
  extracted `XA/*.xa` files.

Reverb is stubbed; 3D panning uses SDL’s stereo panner for now.

## File I/O — Fs_Queue → PcFs

PSX: `g_FileTable` (generated into `src/main/filetable.c.USA.inc` by
`silentassets`) maps `e_FsFile` → `(startSector, blockCount, pathIdx)`.
`Fs_QueueStartRead(FILE_1ST_BODYPROG_BIN, FS_BUFFER_0)` does `CdRead()`.

PC: `PcFs_LoadFile(FILE_1ST_BODYPROG_BIN)` does `fopen("assets/USA/1ST/BODYPROG.BIN")`.
The mapping is the same — `fileenum.h.USA.inc` is shared. Async queue is
synchronous for now; a thread pool can be added.

Savegames: PSX writes `BISLPM-86192` card images; PC writes the same format
to `~/.local/share/silenthill/save/` so saves are interchangeable.

## Shiftability

See `pc/include/pc/core/shiftable.h`.

Hardcoded VRAM is the blocker for modding. Example:

```c
// PSX — breaks if you add a field to s_MapOverlayHdr
void* SECTION(".rodata") g_OvlDynamic = (void*)0x800C9578;

// PC — relocatable
void* g_OvlDynamic = sh_overlay_base(PcOverlay_Map0_S00);
```

Tool to find remaining hardcodes:

```bash
python3 pc/tools/make_shiftable.py --check
python3 pc/tools/make_shiftable.py --fix src/main/main.c
```

After all `0x800XXXXX` are gone, YAML `auto` addresses let splat pack
segments without manual `0x13D44` offsets.

## TODO

### Must-have for boot to menu

- [ ] Finish `Fs_Queue` → `PcFs` for all `FILE_*` used in `GameState_Init`
      (`1ST/BODYPROG.BIN`, `1ST/B_KONAMI.BIN`, `TIM/*`)
- [ ] Implement `DrawPrim` decoder for `POLY_FT4`/`SPRT` used by `Screen_Init`
- [ ] Keyboard → pad mapping passes `MainLoop_ShouldWarmReset()` (L1+R1+Start)

### Must-have for in-game

- [ ] Map overlay loader: `PcOverlay_Load(PcOverlay_Map0_S00, "assets/USA/VIN/MAP0_S00.BIN")`
      + `sh_overlay_fixup()` for every pointer in `s_MapOverlayHdr`
- [ ] IPD/LM model loader: PSX reads `BG/*.IPD` into scratchpad; PC uploads to VBO
- [ ] Collision: `s_IpdCollisionData` pointer fixups are already in `collision.c`
      but need PC-side `Collision_ChunkHeaderPtrsInit()` that uses host pointers
- [ ] Save/load: `libcard` → POSIX file

### Nice-to-have

- [ ] bgfx Vulkan backend, HDR
- [ ] Widescreen (patch `worldGfx` culling that assumes 320×240)
- [ ] Mod loader: replace `assets/USA/VIN/*.BIN` with rebuilt C overlays

## Testing

```bash
# PSX matching must still pass
make GAME_VERSION=USA CHECKSUM=1 build

# Shiftability smoke
make GAME_VERSION=USA NON_MATCHING=1 SKIP_ASM=1 build

# PC build (no ROM needed — stubs load)
cmake -S pc -B build-pc && cmake --build build-pc -j
./build-pc/silenthill --assets assets/USA

# With ROM present, boot should reach Konami logo
./build-pc/silenthill --assets assets/USA --rom rom/USA
```
