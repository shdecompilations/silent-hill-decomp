#include "game.h"

#include <psyq/libetc.h>
#include <psyq/libgs.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/screen/data.h"
#include "bodyprog/screen/draw.h"
#include "bodyprog/math/math.h"
#include "main/fsqueue.h"

#if VERSION_REGION_IS(PAL)
    #define FADE_TILE_HEIGHT 255
#else
    #define FADE_TILE_HEIGHT SCREEN_HEIGHT
#endif

static q19_12 g_Screen_PrevFadeProgressState;

static DR_MODE D_800A8E5C[] = {
    { 0x3000000, { 0xE1000240, 0x0 } },
    { 0x3000000, { 0xE1000240, 0x0 } }
};

static TILE D_800A8E74[] = {
    { 0x3000000, 255, 0, 0, 0x62, -SCREEN_WIDTH, -FADE_TILE_HEIGHT, SCREEN_WIDTH * 2, FADE_TILE_HEIGHT * 2 },
    { 0x3000000, 255, 0, 0, 0x62, -SCREEN_WIDTH, -FADE_TILE_HEIGHT, SCREEN_WIDTH * 2, FADE_TILE_HEIGHT * 2 }
};

static q19_12 g_Screen_FadeProgress = Q12(0.0f);

/** @brief Sets the screen fade effect to black or white according to the current status. */
static void Screen_FadeDrawModeSet(DR_MODE* drMode) // 0x800325A4
{
    if (IS_SCREEN_FADE_WHITE(g_Screen_FadeStatus))
    {
        SetDrawMode(drMode, 0, 1, 32, NULL);
    }
    else
    {
        SetDrawMode(drMode, 0, 1, 64, NULL);
    }
}

q19_12 Screen_FadeProgressGet(void) // 0x800325F8
{
    return Q12(1.0f) - g_Screen_PrevFadeProgressState;
}

void Screen_FadeUpdate(void) // 0x8003260C
{
    s32      queueLength;
    q19_12   timestep;
    GsOT*    ot;
    TILE*    tile;
    DR_MODE* drMode;

    drMode                         = &D_800A8E5C[g_ActiveBufferIdx];
    tile                           = &D_800A8E74[g_ActiveBufferIdx];
    g_Screen_PrevFadeProgressState = g_Screen_FadeProgress;

    switch (g_Screen_FadeStatus)
    {
        case SCREEN_FADE_STATUS(ScreenFadeState_FadeOutStart, false):
        case SCREEN_FADE_STATUS(ScreenFadeState_FadeOutStart, true):
            g_Screen_FadeProgress = Q12(0.0f);
            g_Screen_FadeStatus++;

        case SCREEN_FADE_STATUS(ScreenFadeState_FadeOutSteps, false):
        case SCREEN_FADE_STATUS(ScreenFadeState_FadeOutSteps, true):
            Screen_FadeDrawModeSet(drMode);
            queueLength = Fs_QueueGetLength();

            if (g_ScreenFadeTimestep > Q12(0.0f))
            {
                timestep = g_ScreenFadeTimestep;
            }
            else
            {
                timestep = Q12(3.0f) / (queueLength + 1);
            }

            g_Screen_FadeProgress += Q12_MULT_PRECISE(timestep, g_DeltaTimeRaw);
            if (g_Screen_FadeProgress >= Q12_CLAMPED(1.0f))
            {
                g_Screen_FadeProgress = Q12_CLAMPED(1.0f);
                g_Screen_FadeStatus++;
            }

            tile->r0 = Q12_TO_Q8(g_Screen_FadeProgress);
            tile->g0 = Q12_TO_Q8(g_Screen_FadeProgress);
            tile->b0 = Q12_TO_Q8(g_Screen_FadeProgress);
            break;

        case SCREEN_FADE_STATUS(ScreenFadeState_ResetTimestep, false):
        case SCREEN_FADE_STATUS(ScreenFadeState_ResetTimestep, true):
            g_ScreenFadeTimestep = Q12(0.0f);

        case SCREEN_FADE_STATUS(ScreenFadeState_FadeInStart, false):
        case SCREEN_FADE_STATUS(ScreenFadeState_FadeInStart, true):
            g_Screen_FadeProgress = Q12_CLAMPED(1.0f);
            g_Screen_FadeStatus++;

        case SCREEN_FADE_STATUS(ScreenFadeState_FadeOutComplete, false):
        case SCREEN_FADE_STATUS(ScreenFadeState_FadeOutComplete, true):
            Screen_FadeDrawModeSet(drMode);
            tile->r0 = Q12_TO_Q8(g_Screen_FadeProgress);
            tile->g0 = Q12_TO_Q8(g_Screen_FadeProgress);
            tile->b0 = Q12_TO_Q8(g_Screen_FadeProgress);
            break;

        case SCREEN_FADE_STATUS(ScreenFadeState_FadeInSteps, false):
        case SCREEN_FADE_STATUS(ScreenFadeState_FadeInSteps, true):
            Screen_FadeDrawModeSet(drMode);

            if (g_ScreenFadeTimestep > Q12(0.0f))
            {
                timestep = g_ScreenFadeTimestep;
            }
            else
            {
                timestep = Q12(3.0f);
            }

            g_Screen_FadeProgress -= Q12_MULT_PRECISE(timestep, g_DeltaTimeRaw);
            if (g_Screen_FadeProgress <= Q12(0.0f))
            {
                g_Screen_FadeProgress = Q12(0.0f);
                Screen_FadeReset();
                return;
            }

            tile->r0 = Q12_TO_Q8(g_Screen_FadeProgress);
            tile->g0 = Q12_TO_Q8(g_Screen_FadeProgress);
            tile->b0 = Q12_TO_Q8(g_Screen_FadeProgress);
            break;

        case SCREEN_FADE_STATUS(ScreenFadeState_Reset, false):
            g_ScreenFadeTimestep  = Q12(0.0f);
            g_Screen_FadeProgress = Q12(0.0f);
            g_Screen_FadeStatus   = SCREEN_FADE_STATUS(ScreenFadeState_None, false);
            return;

        case SCREEN_FADE_STATUS(ScreenFadeState_None, false):
            return;
    }

    ot = &g_OtTags0[g_ActiveBufferIdx][5];
    AddPrim(ot, tile);
    AddPrim(ot, drMode);
}
