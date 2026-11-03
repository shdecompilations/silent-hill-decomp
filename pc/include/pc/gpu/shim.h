#pragma once
/**
 * GPU shim — replaces libgpu / libgs / libgte on PC.
 *
 * The decomp calls PSX GPU macros like setPolyFT4(), setXY0(), DrawPrim(),
 * PutDrawEnv(), etc. On PC we translate these into bgfx/OpenGL draw calls.
 *
 * Minimal faithful shim: we preserve the ordering table (OT) concept but
 * execute it immediately via modern rendering.
 */

#include "decomp/types.h"
#include "psyq/libgpu.h"
#include "psyq/libgte.h"

#ifdef PC_PORT

// ---------------------------------------------------------------------------
// Framebuffer / display — shadows PSX VRAM layout
// ---------------------------------------------------------------------------
#define SH_SCREEN_WIDTH   320
#define SH_SCREEN_HEIGHT  240
#define SH_FB_WIDTH       320
#define SH_FB_HEIGHT      480  // double-buffered

typedef struct _PcFrameBuffer {
    u32 width, height;
    u32* pixels; // RGBA8
} s_PcFrameBuffer;

extern s_PcFrameBuffer g_PcFrontBuffer;
extern s_PcFrameBuffer g_PcBackBuffer;

// ---------------------------------------------------------------------------
// Ordering table — on PSX this is a linked list in scratchpad
// We keep the same API but back it with a vector of draw commands.
// ---------------------------------------------------------------------------
#define SH_OT_SIZE  4096

typedef struct _PcOtEntry {
    u8  primType;   // POLY_FT4, SPRT, TILE, etc.
    u8  abr;        // semi-transparency
    u8  tpage;      // texture page
    u8  clut;       // CLUT index
    // vertices (screen space, already projected)
    s16 x[4], y[4];
    s16 u[4], v[4];
    CVECTOR rgb[4];
    u16 z;          // OT depth (lower = farther)
} s_PcOtEntry;

extern s_PcOtEntry g_PcOt[SH_OT_SIZE];
extern int g_PcOtCount;

void PcGfx_Init(int width, int height);
void PcGfx_Shutdown(void);
void PcGfx_BeginFrame(void);
void PcGfx_EndFrame(void);

// Translate libgpu calls
void PcGfx_DrawPrim(const void* prim);
void PcGfx_PutDrawEnv(const DRAWENV* env);
void PcGfx_PutDispEnv(const DISPENV* env);
void PcGfx_ClearImage(const RECT* rect, u8 r, u8 g, u8 b);
void PcGfx_LoadImage(const RECT* rect, const u32* data);
void PcGfx_StoreImage(const RECT* rect, u32* data);
void PcGfx_MoveImage(const RECT* rect, int x, int y);

// GTE — CPU emulation (fixed-point)
void PcGte_Init(void);
void PcGte_SetRotMatrix(const MATRIX* m);
void PcGte_SetTransMatrix(const MATRIX* m);
void PcGte_RotTransPers(const VECTOR* v, long* sxy, long* p, long* flag);
void PcGte_RotTransPers4(
    const VECTOR* v0, const VECTOR* v1, const VECTOR* v2, const VECTOR* v3,
    long* sxy0, long* sxy1, long* sxy2, long* sxy3,
    long* p0, long* p1, long* p2, long* p3, long* flag
);

// Texture cache — TIM → GPU texture
typedef struct _PcTexture {
    u32 id;
    int width, height;
    int tpage, clut;
} s_PcTexture;

s_PcTexture* PcTexture_FromTim(const void* timData);
void PcTexture_Bind(const s_PcTexture* tex);

#else
// On PSX, include original headers directly
#endif
