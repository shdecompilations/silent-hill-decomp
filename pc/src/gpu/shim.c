#include "pc/gpu/shim.h"

#ifdef PC_PORT

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

s_PcFrameBuffer g_PcFrontBuffer = {0};
s_PcFrameBuffer g_PcBackBuffer = {0};
s_PcOtEntry g_PcOt[SH_OT_SIZE] = {0};
int g_PcOtCount = 0;

static SDL_Renderer* s_Renderer = NULL;
static SDL_Window* s_WindowRef = NULL;

void PcGfx_Init(int width, int height) {
    extern SDL_Window* s_Window;
    s_WindowRef = s_Window;

    // Try accelerated renderer, fallback to software
    s_Renderer = SDL_CreateRenderer(s_WindowRef, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!s_Renderer) {
        s_Renderer = SDL_CreateRenderer(s_WindowRef, -1, SDL_RENDERER_SOFTWARE);
    }
    printf("PcGfx: renderer %s (%dx%d)\n", s_Renderer ? SDL_GetRendererInfo(s_Renderer, &(SDL_RendererInfo){0}) == 0 ? "ok" : "unknown" : "null", width, height);

    g_PcFrontBuffer.width = width;
    g_PcFrontBuffer.height = height;
    g_PcFrontBuffer.pixels = calloc(width * height, sizeof(u32));

    g_PcBackBuffer.width = width;
    g_PcBackBuffer.height = height;
    g_PcBackBuffer.pixels = calloc(width * height, sizeof(u32));
}

void PcGfx_Shutdown(void) {
    free(g_PcFrontBuffer.pixels);
    free(g_PcBackBuffer.pixels);
    if (s_Renderer) SDL_DestroyRenderer(s_Renderer);
}

void PcGfx_BeginFrame(void) {
    g_PcOtCount = 0;
    if (s_Renderer) {
        SDL_SetRenderDrawColor(s_Renderer, 0, 0, 0, 255);
        SDL_RenderClear(s_Renderer);
    }
}

void PcGfx_EndFrame(void) {
    // Flush OT: in a real bgfx backend this would submit draw calls.
    // For now we just present the SDL renderer.
    if (s_Renderer) SDL_RenderPresent(s_Renderer);

    // Swap buffers
    s_PcFrameBuffer tmp = g_PcFrontBuffer;
    g_PcFrontBuffer = g_PcBackBuffer;
    g_PcBackBuffer = tmp;
}

void PcGfx_DrawPrim(const void* prim) {
    if (g_PcOtCount >= SH_OT_SIZE) return;
    // Decode prim header: first byte is primitive type + flags
    // This is a stub that logs; full implementation decodes
    // POLY_FT4 / SPRT / TILE etc. via libgpu.h structs.
    const u8* p = (const u8*)prim;
    s_PcOtEntry* e = &g_PcOt[g_PcOtCount++];
    e->primType = p[0];
    // Real decoder would memcpy verts, UVs, colors etc.
}

void PcGfx_PutDrawEnv(const DRAWENV* env) {
    // Set scissor / offset for subsequent DrawPrims
    (void)env;
}

void PcGfx_PutDispEnv(const DISPENV* env) {
    (void)env;
}

void PcGfx_ClearImage(const RECT* rect, u8 r, u8 g, u8 b) {
    if (s_Renderer) {
        SDL_SetRenderDrawColor(s_Renderer, r, g, b, 255);
        SDL_Rect rc = { rect->x, rect->y, rect->w, rect->h };
        SDL_RenderFillRect(s_Renderer, &rc);
    }
}

void PcGfx_LoadImage(const RECT* rect, const u32* data) {
    (void)rect; (void)data;
    // TIM upload: PcTexture_FromTim() is the real path
}

void PcGfx_StoreImage(const RECT* rect, u32* data) {
    (void)rect; (void)data;
}

void PcGfx_MoveImage(const RECT* rect, int x, int y) {
    (void)rect; (void)x; (void)y;
}

// ---------------------------------------------------------------------------
// GTE — fixed-point geometry transform, CPU fallback
// ---------------------------------------------------------------------------
static MATRIX s_GteRot = {0};
static MATRIX s_GteTrans = {0};

void PcGte_Init(void) {
    // No HW GTE on PC; we use integer math matching the PSX pipeline.
}

void PcGte_SetRotMatrix(const MATRIX* m) { s_GteRot = *m; }
void PcGte_SetTransMatrix(const MATRIX* m) { s_GteTrans = *m; }

void PcGte_RotTransPers(const VECTOR* v, long* sxy, long* p, long* flag) {
    // Simplified: apply rotation + translation, then perspective divide.
    // Full PSX GTE does 16-bit saturating + flag handling; we stub
    // enough for menu/map screens to render at correct positions.
    long x = (s_GteRot.m[0][0] * v->vx + s_GteRot.m[0][1] * v->vy + s_GteRot.m[0][2] * v->vz) / 4096 + s_GteTrans.t[0];
    long y = (s_GteRot.m[1][0] * v->vx + s_GteRot.m[1][1] * v->vy + s_GteRot.m[1][2] * v->vz) / 4096 + s_GteTrans.t[1];
    long z = (s_GteRot.m[2][0] * v->vx + s_GteRot.m[2][1] * v->vy + s_GteRot.m[2][2] * v->vz) / 4096 + s_GteTrans.t[2];
    if (z == 0) z = 1;
    // PSX projection: sxy = (x * proj + center) / z ; we use 320x240 center
    *sxy = ((x * 256) / z) + (160 << 16) | (((y * 256) / z + 120) << 16);
    if (p) *p = z;
    if (flag) *flag = 0;
}

void PcGte_RotTransPers4(const VECTOR* v0, const VECTOR* v1, const VECTOR* v2, const VECTOR* v3,
                         long* sxy0, long* sxy1, long* sxy2, long* sxy3,
                         long* p0, long* p1, long* p2, long* p3, long* flag) {
    PcGte_RotTransPers(v0, sxy0, p0, flag);
    PcGte_RotTransPers(v1, sxy1, p1, flag);
    PcGte_RotTransPers(v2, sxy2, p2, flag);
    PcGte_RotTransPers(v3, sxy3, p3, flag);
}

s_PcTexture* PcTexture_FromTim(const void* timData) {
    (void)timData;
    static s_PcTexture dummy = {0};
    return &dummy;
}

void PcTexture_Bind(const s_PcTexture* tex) { (void)tex; }

#endif // PC_PORT
