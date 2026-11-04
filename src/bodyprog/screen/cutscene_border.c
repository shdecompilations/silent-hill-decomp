#include "game.h"

#include <libetc.h>
#include <libgs.h>

#include "main/fsqueue.h"
#include "bodyprog/screen/screen_data.h"
#include "bodyprog/screen/screen_draw.h"
#include "bodyprog/view/structs.h"
#include "bodyprog/view/vc_main.h"
#include "bodyprog/view/vc_util.h"
#include "bodyprog/math/math.h"

// ========================================
// STATIC VARIABLES
// ========================================

static DR_MODE D_800A8E98[] = {
    { 0x3000000, { 0xE1000240, 0x0 } },
    { 0x3000000, { 0xE1000240, 0x0 } }
};

#define BORDER_TOP_OUTER_Y    SCREEN_FRAMEBUFFER_EDGE_TOP
#define BORDER_TOP_INNER_Y    -96
#define BORDER_BOTTOM_OUTER_Y SCREEN_FRAMEBUFFER_EDGE_BOTTOM
#define BORDER_BOTTOM_INNER_Y (96 + (FRAMEBUFFER_HEIGHT_PROGRESSIVE % 2))

// TODO: Make a macro?
static POLY_G4 D_800A8EB0[] = {
    {
        0x8000000,
        0x0, 0x0, 0x0, 0x3A,
        -160, BORDER_TOP_OUTER_Y,
        0x0, 0x0, 0x0, 0x0,
        160, BORDER_TOP_OUTER_Y,
        0x0, 0x0, 0x0, 0x0,
        -160, BORDER_TOP_INNER_Y,
        0x0, 0x0, 0x0, 0x0,
        160, BORDER_TOP_INNER_Y
    },
    {
        0x8000000,
        0x0, 0x0, 0x0, 0x3A,
        -160, BORDER_TOP_OUTER_Y,
        0x0, 0x0, 0x0, 0x0,
        160, BORDER_TOP_OUTER_Y,
        0x0, 0x0, 0x0, 0x0,
        -160, BORDER_TOP_INNER_Y,
        0x0, 0x0, 0x0, 0x0,
        160, BORDER_TOP_INNER_Y
    },
    {
        0x8000000,
        0x0, 0x0, 0x0, 0x3A,
        -160, BORDER_BOTTOM_OUTER_Y,
        0x0, 0x0, 0x0, 0x0,
        160, BORDER_BOTTOM_OUTER_Y,
        0x0, 0x0, 0x0, 0x0,
        -160, BORDER_BOTTOM_INNER_Y,
        0x0, 0x0, 0x0, 0x0,
        160, BORDER_BOTTOM_INNER_Y
    },
    {
        0x8000000,
        0x0, 0x0, 0x0, 0x3A,
        -160, BORDER_BOTTOM_OUTER_Y,
        0x0, 0x0, 0x0, 0x0,
        160, BORDER_BOTTOM_OUTER_Y,
        0x0, 0x0, 0x0, 0x0,
        -160, BORDER_BOTTOM_INNER_Y,
        0x0, 0x0, 0x0, 0x0,
        160, BORDER_BOTTOM_INNER_Y
    }
};

static q19_12 g_Screen_BlackBorderShade = Q12(0.0f);

// ========================================
// CUTSCENE BORDERS
// ========================================

void Screen_CutsceneCameraStateUpdate(void)
{
    static void Screen_BlackBorderDraw(POLY_G4* poly, q19_12 color)
    {
        s32    i;
        q19_12 color0;
        q19_12 color1;

        color0 = color >> 4;
        color1 = color >> 5;

        if (color == Q12_CLAMPED(1.0f))
        {
            color1 = Q8_CLAMPED(1.0f);
        }

        for (i = 0; i < 2; i++)
        {
            poly[i * 2].r0 = color0;
            poly[i * 2].g0 = color0;
            poly[i * 2].b0 = color0;
            poly[i * 2].r1 = color0;
            poly[i * 2].g1 = color0;
            poly[i * 2].b1 = color0;
            poly[i * 2].r2 = color1;
            poly[i * 2].g2 = color1;
            poly[i * 2].b2 = color1;
            poly[i * 2].r3 = color1;
            poly[i * 2].g3 = color1;
            poly[i * 2].b3 = color1;
        }
    }

    GsOT*    ot;
    POLY_G4* poly;
    DR_MODE* drMode;

    drMode = &D_800A8E98[g_ActiveBufferIdx];
    poly   = &D_800A8EB0[g_ActiveBufferIdx];

    vcSetEvCamRate(g_Screen_BlackBorderShade);

    if (g_SysWork.bgmStatusFlags & BgmStatusFlag_Pause)
    {
        return;
    }

    switch (g_SysWork.cutsceneBorderState)
    {
        case CutsceneBorderState_FadeInStart:
            g_SysWork.cutsceneBorderState++;

        case CutsceneBorderState_FadingIn:
            g_Screen_BlackBorderShade += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 1.0f);
            if (g_Screen_BlackBorderShade >= Q12_CLAMPED(1.0f))
            {
                g_Screen_BlackBorderShade = Q12_CLAMPED(1.0f);
                g_SysWork.cutsceneBorderState++;
            }

            Screen_BlackBorderDraw(poly, g_Screen_BlackBorderShade);
            break;

        case CutsceneBorderState_ForceShow:
        case CutsceneBorderState_FadeOutStart:
            g_Screen_BlackBorderShade = Q12_CLAMPED(1.0f);
            g_SysWork.cutsceneBorderState++;

        case CutsceneBorderState_Shown:
            Screen_BlackBorderDraw(poly, g_Screen_BlackBorderShade);
            break;

        case CutsceneBorderState_FadingOut:
            g_Screen_BlackBorderShade -= Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 1.0f);
            if (g_Screen_BlackBorderShade <= Q12(0.0f))
            {
                g_Screen_BlackBorderShade = Q12(0.0f);
                CutsceneBorder_Reset();
                return;
            }

            Screen_BlackBorderDraw(poly, g_Screen_BlackBorderShade);
            break;

        case CutsceneBorderState_Reset:
            g_Screen_BlackBorderShade            = Q12(0.0f);
            g_SysWork.cutsceneBorderState = CutsceneBorderState_None;
            g_SysWork.sysFlags           &= ~SysFlag_CutsceneActive;
            return;

        case CutsceneBorderState_None:
            return;
    }

    ot = &g_OtTags0[g_ActiveBufferIdx][4];
    AddPrim(ot, poly);
    AddPrim(ot, &poly[2]);
    AddPrim(ot, drMode);

    if (!(g_SysWork.sysFlags & SysFlag_CutsceneActive))
    {
        vcChangeProjectionValue(SCREEN_GEOM_DIST + Q12_MULT(377 - SCREEN_GEOM_DIST, g_Screen_BlackBorderShade));
    }
}
