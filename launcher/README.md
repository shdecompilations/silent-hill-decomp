# Silent Hill — PC Recompilation Launcher

Web launcher for the PC port. Themed to Silent Hill’s fog/static, not a generic settings dialog.

## Run

```bash
# serve locally (any static server)
python3 -m http.server 8000 --directory launcher
# open http://localhost:8000
```

Or via Docker / GitHub Pages — it’s static HTML/CSS/JS, no backend required.

## What it does

- **Play** — select version (USA/EUR/JAP) + entry (boot / specific map / debug), verify PSX checksum (`46/46 MATCH`), and launch `./build-pc/silenthill --assets assets/USA --rom rom/USA`.
- **Assets** — drag-and-drop retail image (`SLUS-00707.bin/.cue`) → `dumpsxiso + silentassets` extraction to `assets/USA/` + `rom/USA/`. Shows required vs generated files.
- **Settings** — renderer (GL 3.3 default, bgfx Vulkan/D3D12), internal resolution, VSync, dithering, widescreen culling, audio/input, build mode (Matching / Shiftable / PC_PORT).
- **Mods** — `mods/` toggling (requires `SH_SHIFTABLE` build).
- **Tools** — one-click `data_audit.py`, `naming_progress.py`, `make check`, `make_shiftable.py --check`.

All buttons are wired to console log simulation; replace `src/app.js`’s `fakeProgress`/`log` with real IPC when packaging as Electron/Tauri.

## Packaging as native (Tauri/Electron)

The launcher is designed to be wrapped:

```bash
# Tauri (Rust backend can call `make extract` etc. directly)
npm create tauri-app launcher-native
# copy launcher/* into src, wire `window.__TAURI__.invoke('extract', {path})`
```

Native build uses `pc/CMakeLists.txt` for the game binary; the launcher is the entry point that ensures assets exist before exec’ing `silenthill`.

## Native C++ launcher (`pc/launcher/`)

For a no-runtime launcher, see `pc/launcher/` — SDL2 + Dear ImGui implementation with the same fog aesthetic, built via CMake. It shares `pc/include/pc/` shims.
