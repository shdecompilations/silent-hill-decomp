#ifndef _BODYPROG_SCREEN_DATA_H
#define _BODYPROG_SCREEN_DATA_H

// =====================================
// SCREEN/FRAMEBUFFER-RELATED CONSTANTS
// =====================================

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240

#if VERSION_REGION_IS(PAL)
    #define FRAMEBUFFER_HEIGHT_PROGRESSIVE 255
#else
    #define FRAMEBUFFER_HEIGHT_PROGRESSIVE 224
#endif

#define FRAMEBUFFER_HEIGHT_INTERLACED  (FRAMEBUFFER_HEIGHT_PROGRESSIVE * 2)

/** @brief First VRAM row of the interlaced framebuffer, used as the Y origin when clearing or drawing it. */
#if VERSION_REGION_IS(PAL)
    #define SCREEN_FRAMEBUFFER_Y_INTERLACED 1
#else
    #define SCREEN_FRAMEBUFFER_Y_INTERLACED 32
#endif

/** @brief Y position of the display buffer passed to `GsDefDispBuff2`. */
#if VERSION_REGION_IS(PAL)
    #define SCREEN_DISPBUFF_Y_INTERLACED 0
#else
    #define SCREEN_DISPBUFF_Y_INTERLACED 32
#endif

/** @brief Y offset of the display area within the TV picture. */
#if VERSION_REGION_IS(PAL)
    #define SCREEN_DISPENV_Y_BASE 16
#else
    #define SCREEN_DISPENV_Y_BASE 8
#endif

/** @brief Perspective projection distance used with `SetGeomScreen`.
 * NTSC uses the value from `g_GameWork.gsScreenHeight`, while PAL always uses the 240 line screen height.
 */
#if VERSION_REGION_IS(PAL)
    #define SCREEN_GEOM_DIST SCREEN_HEIGHT
#else
    #define SCREEN_GEOM_DIST g_GameWork.gsScreenHeight
#endif

/** @brief Distance from the screen center to the top and bottom edges of the framebuffer. */
#define SCREEN_FRAMEBUFFER_EDGE_TOP    (-(FRAMEBUFFER_HEIGHT_PROGRESSIVE / 2))
#define SCREEN_FRAMEBUFFER_EDGE_BOTTOM ((FRAMEBUFFER_HEIGHT_PROGRESSIVE + 1) / 2)

#define ORDERING_TABLE_SIZE 2048

// =========================
// TIMING-RELATED CONSTANTS
// =========================

#define TICKS_PER_SECOND 60                                        /** Game has a variable timestep with 60 ticks max. */
#define TIMESTEP_30_FPS  Q12(1.0f / (float)(TICKS_PER_SECOND / 2))
#define TIMESTEP_60_FPS  Q12(1.0f / (float)(TICKS_PER_SECOND))

/** @brief Tick rate used in some (but not all) PAL framerate code.
 * Usually seen used by `SECONDS_60_FPS` macros, where the tick rate was changed from 60 to 50.
 * Some code may still use the normal `SECONDS_60_FPS` (with 60 tick rate).
 * Code that does change in PAL is changed to `SECONDS_TO_FRAMES`,
 * which picks the rate based on the region being built.
 */
#define TICKS_PER_SECOND_PAL 50

#if VERSION_REGION_IS(PAL)
    #define TICKS_PER_SECOND_REGIONAL TICKS_PER_SECOND_PAL
#else
    #define TICKS_PER_SECOND_REGIONAL TICKS_PER_SECOND
#endif

/** @brief Converts seconds to frames at 60 FPS.
 *
 * @param sec Seconds to convert.
 * @return Frames at 60 FPS.
 */
#define SECONDS_60_FPS(sec) \
    (s32)((sec) * TICKS_PER_SECOND)

/** @brief Converts seconds to frames at the region's full refresh rate.
 *
 * @note 60 FPS for NTSC/NTSCJ, 50 FPS for PAL.
 *
 * @param sec Seconds to convert.
 * @return Frames at 50/60 FPS.
 */
#define SECONDS_TO_FRAMES(sec) \
    (s32)((sec) * TICKS_PER_SECOND_REGIONAL)

// ========
// GLOBALS
// ========

/** @note This is defined in `draw.c` because inbetween this and
 * the previous variable (`g_Screen_PrevFadeProgressState`) there is garbage
 * data. It suggests this variable was defined in a file separate from
 * `fade.c`.
 */
extern q19_12 g_ScreenFadeTimestep;
extern s32  g_ActiveBufferIdx;
extern GsOT g_OrderingTable0[2];
extern GsOT g_OrderingTable1[2];
extern GsOT g_OrderingTable2[2];

/** Actual delta time since the previous tick. Unlike `g_DeltaTime`, this value is never overridden by demos or game code. Mostly seems used by UI code. */
extern q19_12 g_DeltaTimeRaw;

extern s32 g_IntervalVBlanks;

// ====================
// GLOBALS (BSS; Hack)
// ====================
// To match the order of the BSS segment, extern declarations
// are required in a predetermined order.
// This is done until a way to replicate `common`
// segment behavior is found.

extern s32      g_VBlanks;
extern s32      g_UncappedVBlanks;
extern s32      __pad_bss_800B5C3C;
extern GsOT_TAG g_OtTags0[2][16];

/** Main gameplay timer. Code pauses gameplay by setting this to `Q12(0.0f)`.
 * Demos override this with the time from a demo file to ensure deos remain in sync.
 *
 * Possible original name: `_shDeltaTimePerFrame`
 */
extern q19_12 g_DeltaTime;

extern GsOT_TAG g_OtTags1[2][ORDERING_TABLE_SIZE];

extern s32 __pad_bss_800B9CC4;

/** Gravity speed for this tick. */
extern q19_12 g_GravitySpeed;

/** Number of ticks since game start. */
extern s32 g_TickCount;

extern s32 g_MainLoop_FrameCount;

#endif
