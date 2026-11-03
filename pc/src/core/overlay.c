#include "pc/core/shiftable.h"

#ifdef PC_PORT

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

bool sh_ptr_is_psx_vram(const void* p) {
    uintptr_t v = (uintptr_t)p;
    return (v >= 0x80010000 && v < 0x80200000);
}

void* sh_ptr_vram_to_host(const void* psxPtr) {
    if (!psxPtr) return NULL;
    uintptr_t v = (uintptr_t)psxPtr;
    // Map VRAM windows to heap bases
    if (v >= 0x80010000 && v < 0x80024B60) {
        // main
        if (g_PcOverlays[PcOverlay_Main].base)
            return (u8*)g_PcOverlays[PcOverlay_Main].base + (v - 0x80010000);
    } else if (v >= 0x80024B60 && v < 0x800C9578) {
        if (g_PcOverlays[PcOverlay_Bodyprog].base)
            return (u8*)g_PcOverlays[PcOverlay_Bodyprog].base + (v - 0x80024B60);
    } else if (v >= SH_MAP_VRAM_BASE && v < SH_MAP_VRAM_END) {
        // Map overlays share the 0x800C9578 window — find which is loaded
        // For now we keep a single active map
        for (int i = PcOverlay_Map0_S00; i < PcOverlay_Count; i++) {
            if (g_PcOverlays[i].base) {
                return (u8*)g_PcOverlays[i].base + (v - SH_MAP_VRAM_BASE);
            }
        }
    }
    // Not a VRAM pointer — return as-is (might be scratchpad 0x1F800000 or heap)
    return (void*)psxPtr;
}

void* sh_ptr_host_to_vram(const void* hostPtr) {
    if (!hostPtr) return NULL;
    uintptr_t h = (uintptr_t)hostPtr;
    for (int i = 0; i < PcOverlay_Count; i++) {
        s_PcOverlay* o = &g_PcOverlays[i];
        if (!o->base || o->size == 0) continue;
        uintptr_t base = (uintptr_t)o->base;
        if (h >= base && h < base + o->size) {
            uintptr_t vramBase = 0;
            switch (i) {
                case PcOverlay_Main: vramBase = 0x80010000; break;
                case PcOverlay_Bodyprog: vramBase = 0x80024B60; break;
                default:
                    if (i >= PcOverlay_Map0_S00) vramBase = SH_MAP_VRAM_BASE;
                    else vramBase = 0x801E2600;
                    break;
            }
            return (void*)(vramBase + (h - base));
        }
    }
    return (void*)hostPtr;
}

void sh_overlay_fixup(s_PcOverlay* ovl) {
    if (!ovl || !ovl->base || !ovl->relocOffsets) return;
    // Reloc table is generated at extraction: each entry is a file offset
    // of a 32-bit word that was an absolute pointer in VRAM.
    for (size_t i = 0; i < ovl->relocCount; i++) {
        u32 off = ovl->relocOffsets[i];
        if (off + 4 > ovl->size) continue;
        u32* slot = (u32*)((u8*)ovl->base + off);
        u32 vramPtr = *slot;
        if (vramPtr >= 0x80010000 && vramPtr < 0x80200000) {
            void* host = sh_ptr_vram_to_host((void*)(uintptr_t)vramPtr);
            *slot = (u32)(uintptr_t)host;
        }
    }
    printf("sh_overlay_fixup: %s %zu relocs patched\n",
           ovl->id == PcOverlay_Bodyprog ? "BODYPROG" : "overlay",
           ovl->relocCount);
}

// Helper to load an overlay from disk and install it
bool PcOverlay_Load(e_PcOverlayId id, const char* path) {
    FILE* f = fopen(path, "rb");
    if (!f) {
        fprintf(stderr, "PcOverlay_Load: cannot open %s\n", path);
        return false;
    }
    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    fseek(f, 0, SEEK_SET);
    void* mem = malloc(sz);
    if (fread(mem, 1, sz, f) != (size_t)sz) {
        free(mem); fclose(f); return false;
    }
    fclose(f);

    s_PcOverlay* o = &g_PcOverlays[id];
    if (o->base) free(o->base);
    o->id = id;
    o->base = mem;
    o->size = sz;
    // Reloc table is side-car: <path>.reloc
    char relocPath[512];
    snprintf(relocPath, sizeof(relocPath), "%s.reloc", path);
    FILE* rf = fopen(relocPath, "rb");
    if (rf) {
        fseek(rf, 0, SEEK_END);
        long rsz = ftell(rf);
        fseek(rf, 0, SEEK_SET);
        o->relocCount = rsz / 4;
        o->relocOffsets = malloc(rsz);
        fread(o->relocOffsets, 1, rsz, rf);
        fclose(rf);
    } else {
        o->relocCount = 0;
        o->relocOffsets = NULL;
    }

    sh_overlay_fixup(o);
    return true;
}

void PcOverlay_Unload(e_PcOverlayId id) {
    s_PcOverlay* o = &g_PcOverlays[id];
    if (o->base) free(o->base);
    if (o->relocOffsets) free(o->relocOffsets);
    memset(o, 0, sizeof(*o));
}

#endif
