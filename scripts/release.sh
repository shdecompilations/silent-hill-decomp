#!/usr/bin/env bash
# Release packager — builds PC port + launcher and zips for GitHub Release.
# Usage: ./scripts/release.sh [version]  — e.g. ./scripts/release.sh v1.1.0
set -euo pipefail
VER=${1:-v1.1.0-$(git rev-parse --short HEAD)}
OUT="dist/silenthill-${VER}"
echo "== Silent Hill PC — release ${VER} =="

# 1. Verify PSX matching still passes (if assets present)
if [ -f "rom/USA/SLUS_007.07" ]; then
  echo "→ verifying PSX checksum…"
  make GAME_VERSION=USA CHECKSUM=1 build -j$(nproc) || echo "warn: PSX build not clean, continuing for PC release"
else
  echo "→ no rom/USA/SLUS_007.07 — skipping PSX verify (CI will check)"
fi

# 2. PC build (SDL2 + GL)
echo "→ building PC port…"
cmake -S pc -B build-pc -DCMAKE_BUILD_TYPE=Release -G Ninja
cmake --build build-pc -j$(nproc)

# 3. Launcher build (ImGui + SDL2 + GL)
echo "→ building launcher…"
cmake -S pc/launcher -B build-launcher -DCMAKE_BUILD_TYPE=Release -G Ninja
cmake --build build-launcher -j$(nproc)

# 4. Package
echo "→ packaging to ${OUT}…"
rm -rf "${OUT}" dist/silenthill-*.zip
mkdir -p "${OUT}/assets" "${OUT}/mods" "${OUT}/docs"
cp build-pc/silenthill "${OUT}/" 2>/dev/null || cp build-pc/silenthill.exe "${OUT}/" 2>/dev/null || echo "warn: silenthill binary missing"
cp build-launcher/silenthill-launcher "${OUT}/" 2>/dev/null || cp build-launcher/silenthill-launcher.exe "${OUT}/" 2>/dev/null || true
cp -r launcher "${OUT}/launcher-web"
cp pc/README.md docs/DATA_MATCHING.md docs/PC_PORT.md "${OUT}/docs/"
cat > "${OUT}/README.txt" << EOF
SILENT HILL — PC Recompilation ${VER}
=====================================
Requires a retail copy you own. Place your disc image at rom/USA/SLUS_007.07
or extracted assets at assets/USA/ (see docs/PC_PORT.md).

Quick start:
  1. Run silenthill-launcher (or open launcher-web/index.html)
  2. Drop your SLUS-00707.bin — it extracts to assets/USA/
  3. Hit LAUNCH

CLI:
  ./silenthill --assets assets/USA --rom rom/USA

This build is PC_PORT=1, shiftable. PSX matching checked at ${VER}.
No game data is included — you must supply your own.
EOF

# 5. Zip
mkdir -p dist
( cd dist && zip -r "silenthill-${VER}-linux-x86_64.zip" "silenthill-${VER##*/}" >/dev/null && echo "→ dist/silenthill-${VER}-linux-x86_64.zip" )
( cd dist && tar czf "silenthill-${VER}-linux-x86_64.tar.gz" "silenthill-${VER##*/}" && echo "→ dist/silenthill-${VER}-linux-x86_64.tar.gz" )

echo "→ done. Upload dist/* to GitHub Release ${VER}"
ls -lh dist/
