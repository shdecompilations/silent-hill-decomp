#pragma once
/**
 * Shiftability layer — makes the PSX build's hardcoded VRAM addresses
 * relocatable for PC recompilation.
 *
 * Original PSX layout (USA):
 *   0x80010000  main (SLUS_007.07)
 *   0x80024B60  BODYPROG.BIN (g_OvlBodyprog)
 *   0x800C9578  MAP*_S*.BIN (map overlays)
 *   0x801E2600  screens (B_KONAMI, STREAM, etc.)
 *
 * On PC we allocate each overlay with malloc() and patch pointers at load
 * time via overlay_fixup(). The decomp's C code must not take address-of
 * against a VRAM constant; instead it uses sh_overlay_base().
 */

#include "decomp/types.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef PC_PORT

// Overlay IDs match the linkers/ directory names
typedef enum _PcOverlayId {
    PcOverlay_Main      = 0,
    PcOverlay_Bodyprog  = 1,
    PcOverlay_BKonami   = 2,
    PcOverlay_Stream    = 3,
    PcOverlay_Options   = 4,
    PcOverlay_Saveload  = 5,
    PcOverlay_Credits   = 6,
    PcOverlay_Map0_S00  = 100, // + map index, see pc/src/core/overlay.c
    PcOverlay_Count
} e_PcOverlayId;

typedef struct _PcOverlay {
    e_PcOverlayId id;
    void*         base;      // heap base (was VRAM)
    size_t        size;
    // relocation table: array of file offsets that contain absolute pointers
    u32*          relocOffsets;
    size_t        relocCount;
} s_PcOverlay;

// Global overlay table — populated by pc_overlay_load()
extern s_PcOverlay g_PcOverlays[PcOverlay_Count];

/** Return the current base for an overlay (heap pointer on PC, VRAM on PSX). */
static inline void* sh_overlay_base(e_PcOverlayId id) {
#ifdef PC_PORT
    return g_PcOverlays[id].base;
#else
    // PSX: return the hardcoded VRAM constant the original code expects
    extern void* g_OvlBodyprog;
    extern void* g_OvlDynamic;
    switch (id) {
        case PcOverlay_Bodyprog: return g_OvlBodyprog;
        case PcOverlay_Main:     return (void*)0x80010000;
        default: break;
    }
    return g_OvlDynamic;
#endif
}

/** True if a pointer falls inside any loaded overlay's fixed VRAM window.
 *  Used to detect pointers that still need swizzling. */
bool sh_ptr_is_psx_vram(const void* p);

/** Convert a PSX VRAM pointer to a PC heap pointer (swizzle). */
void* sh_ptr_vram_to_host(const void* psxPtr);

/** Inverse: host pointer → what its PSX VRAM address would have been
 *  (for savegame compat, or for code that hashes pointers). */
void* sh_ptr_host_to_vram(const void* hostPtr);

// Relocation: patch all absolute pointers in a freshly loaded overlay
void sh_overlay_fixup(s_PcOverlay* ovl);

// Convenience for map overlays that share the same VRAM window
#define SH_MAP_VRAM_BASE  0x800C9578u
#define SH_MAP_VRAM_END   0x801E2600u

#else // !PC_PORT

#define sh_overlay_base(id) (NULL)
#define sh_ptr_is_psx_vram(p) (false)
#define sh_ptr_vram_to_host(p) ((void*)(p))
#define sh_ptr_host_to_vram(p) ((void*)(p))
#define sh_overlay_fixup(ovl) do {} while(0)

#endif

#ifdef __cplusplus
}
#endif
