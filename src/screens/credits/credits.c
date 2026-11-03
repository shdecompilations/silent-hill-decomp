#include "game.h"

#include <psyq/libetc.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "bodyprog/ranking.h"
#include "bodyprog/screen/screen_data.h"
#include "bodyprog/screen/screen_draw.h"
#include "bodyprog/sound/sound_system.h"
#include "main/rng.h"
#include "screens/credits/credits.h"

extern s_CreditTextState   g_CreditTextState;
extern s_CreditText3dState g_CreditText3dState;

// Seems to be params for each of the games endings.
// Only looks like index 1 - 4 are used by `map7_s03` though? (which accounts for the 4 different normal endings)
// TODO: is UFO ending also part of this?
s_801E5558 D_801E5558[6] = {
    /** `GameEnding_0`, never used? */
    {
        .xaSfxId     = Sfx_XaAudio725,
        .scrollDelay = SECONDS_TO_FRAMES(0),
        .field_4     = SECONDS_TO_FRAMES(145),
    },
    /** `GameEnding_GoodPlus` */
    {
        .xaSfxId     = Sfx_XaAudio723,
        .scrollDelay = -SECONDS_TO_FRAMES(40),
        .field_4     = SECONDS_TO_FRAMES(140),
    },
    /** `GameEnding_Good` */
    {
        .xaSfxId     = Sfx_XaAudio724,
        .scrollDelay = -SECONDS_TO_FRAMES(15),
        .field_4     = SECONDS_TO_FRAMES(145),
    },
    /** `GameEnding_BadPlus` */
    {
        .xaSfxId     = Sfx_XaAudio725,
        .scrollDelay = -SECONDS_TO_FRAMES(3),
        .field_4     = SECONDS_TO_FRAMES(142),
    },
    /** `GameEnding_Bad` */
    {
        .xaSfxId     = Sfx_XaAudio726,
        .scrollDelay = -SECONDS_TO_FRAMES(3),
        .field_4     = SECONDS_TO_FRAMES(147),
    },
    /** `GameEnding_Ufo`, never used? */
    {
        .xaSfxId     = Sfx_XaAudio726,
        .scrollDelay = -SECONDS_TO_FRAMES(2),
        .field_4     = SECONDS_TO_FRAMES(140)
    }
};

// VRAM regions saved/restored with StoreImage/LoadImage.
RECT g_CreditsVramSaveRects[2] = {
    { 320, 256, 160, 240 },
    { 480, 16, 32, 480 }
};

void func_801E2E28(s32 idx) // 0x801E2E28
{
    s32 var0;
    s32 var1;

    D_801E5E8C = idx;

    Sd_XaPreLoadAudioPreTaskAdd(D_801E5558[idx].xaSfxId);
    var1 = (D_801E5558[idx].field_4 * 2) - 504;
    var0 = var1 / D_801E5C20;

    D_801E5E7C = var1;
    D_801E5E88 = 1;
    D_801E5E7C = var0;
    D_801E5E84 = (var0 * D_801E5C20) + 480;
    D_801E5E80 = 0x10000 / var0;
}

bool func_801E2ED8(void) // 0x801E2ED8
{
    switch (D_801E5E88)
    {
        case 0:
            break;

        case 1:
            SD_Call((u16)D_801E5558[D_801E5E8C].xaSfxId);
            D_801E5E88++;
            break;

        case 2:
            switch (Sd_AudioStreamingCheck())
            {
                case AudioStreamingState_None:
                    return false;

                case AudioStreamingState_XaPlaying:
                    D_800C48F0 = D_801E5558[D_801E5E8C].scrollDelay;
                    return true;

                default:
                    break;
            }

        default:
            break;
    }

    return false;
}

bool func_801E2FC0(void) // 0x801E2FC0
{
    switch (g_ScreenFade_Status)
    {
        case SCREEN_FADE_STATUS(ScreenFadeState_ResetTimestep,   false):
        case SCREEN_FADE_STATUS(ScreenFadeState_FadeOutComplete, false):
        case SCREEN_FADE_STATUS(ScreenFadeState_ResetTimestep,   true):
        case SCREEN_FADE_STATUS(ScreenFadeState_FadeOutComplete, true):
            if (Fs_QueueGetLength() != 0)
            {
                break;
            }

            Game_StateSetNext(GameState_Credits);
            return true;

        case SCREEN_FADE_STATUS(ScreenFadeState_FadeOutStart, false):
        case SCREEN_FADE_STATUS(ScreenFadeState_FadeOutSteps, false):
        case SCREEN_FADE_STATUS(ScreenFadeState_FadeOutStart, true):
        case SCREEN_FADE_STATUS(ScreenFadeState_FadeOutSteps, true):
            break;

        default:
            ScreenFade_Start(true, false, true);
            g_ScreenFadeTimestep = Q12(1.0f);
            break;
    }

    return false;
}

void GameState_Credits_Update(void) // 0x801E3094
{
    bool (*funcs[3])() = {
        func_801E3124,
        func_801E342C,
        func_801E3304
    };

    D_800C48F0 += g_VBlanks;
    if (funcs[g_GameWork.gameStateSteps[0]]())
    {
        Game_StateStepIncrement(0);
    }
}

bool func_801E3124(void) // 0x801E3124
{
    switch (g_GameWork.gameStateSteps[1])
    {
        case 0:
            switch (g_ScreenFade_Status)
            {
                case SCREEN_FADE_STATUS(ScreenFadeState_FadeOutComplete, true):
                    g_GameWork.background2dColor.r = 255;
                    g_GameWork.background2dColor.g = 255;
                    g_GameWork.background2dColor.b = 255;
                    break;

                case SCREEN_FADE_STATUS(ScreenFadeState_FadeOutComplete, false):
                    g_GameWork.background2dColor.r = 0;
                    g_GameWork.background2dColor.g = 0;
                    g_GameWork.background2dColor.b = 0;
                    break;
            }

            SD_Call(18);
            SD_Call(16);
            g_GameWork.gameStateSteps[1]++;
            break;

        case 1:
            StoreImage(&g_CreditsVramSaveRects[0], (u_long*)IMAGE_BUFFER_0);
            StoreImage(&g_CreditsVramSaveRects[1], (u_long*)IMAGE_BUFFER_1);
            DrawSync(SyncMode_Wait);

            switch (g_ScreenFade_Status)
            {
#if VERSION_REGION_IS(PAL)
                case SCREEN_FADE_STATUS(ScreenFadeState_FadeOutComplete, true):
                    Screen_RectInterlacedClear(0, 0, 512, 510, 255, 255, 255);
                    Screen_RectInterlacedClear(0, 510, 512, 2, 0, 0, 0);
                    break;

                case SCREEN_FADE_STATUS(ScreenFadeState_FadeOutComplete, false):
                    Screen_RectInterlacedClear(0, 0, 512, 512, 0, 0, 0);
                    break;
#else
                case SCREEN_FADE_STATUS(ScreenFadeState_FadeOutComplete, true):
                    Screen_RectInterlacedClear(0, 32, 512, 448, 255, 255, 255);
                    break;

                case SCREEN_FADE_STATUS(ScreenFadeState_FadeOutComplete, false):
                    Screen_RectInterlacedClear(0, 32, 512, 448, 0, 0, 0);
                    break;
#endif
            }

            g_GameWork.gameStateSteps[1]++;
            break;

        case 2:
            Screen_Init(512, true);

            g_IntervalVBlanks = 1;
            D_801E5E74        = SECONDS_TO_FRAMES(1);
            g_GameWork.gameStateSteps[1]++;
            break;

        case 3:
            D_801E5E74--;

            if (D_801E5E74 <= 0)
            {
                u8 audioState = Sd_AudioStreamingCheck();
                if (audioState == AudioStreamingState_None || audioState == AudioStreamingState_XaPlaying)
                {
                    g_GameWork.background2dColor.r = 0;
                    g_GameWork.background2dColor.g = 0;
                    g_GameWork.background2dColor.b = 0;
                    return true;
                }
            }

            break;
    }

    return false;
}

bool func_801E3304(void) // 0x801E3304
{
    if (g_GameWork.gameStatePrev == GameState_InGame)
    {
        if (g_GameWork.gameStateSteps[1] == 0)
        {
            Screen_Init(SCREEN_WIDTH, false);

            ScreenFade_Reset();
            g_GameWork.gameStateSteps[1]++;
        }
        else if (g_GameWork.gameStateSteps[1] != 10)
        {
            g_GameWork.gameStateSteps[1]++;
        }
        else
        {
#if VERSION_REGION_IS(PAL)
            Screen_RectInterlacedClear(0, 1, 320, 510, 0, 0, 0);
#else
            Screen_RectInterlacedClear(0, 32, 320, 448, 0, 0, 0);
#endif
            LoadImage(&g_CreditsVramSaveRects[0], IMAGE_BUFFER_0);
            LoadImage(&g_CreditsVramSaveRects[1], IMAGE_BUFFER_1);
            DrawSync(SyncMode_Wait);
            VSync(SyncMode_Wait2);

            Game_StateSetPrevious();
        }

        return false;
    }

    return false;
}

bool func_801E342C(void) // 0x801E342C
{
    s32   temp;
    GsOT* ot;
    TILE* tile;

    if (((g_GameWork.config.extraOptionsEnabled >> (D_801E5E8C - 1)) & (1 << 0)) &&
        (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.skip))
    {
        D_800C48F0 = D_801E5558[D_801E5E8C].field_4 + (D_801E5E84 / 2);
        SD_Call(19);
    }

    ot   = (GsOT*)&g_OtTags1[g_ActiveBufferIdx + 1][0];
    tile = (TILE*)GsOUT_PACKET_P;

    addPrimFast(ot, tile, 3);
    setCodeWord(tile, PRIM_RECT, 0);
#if VERSION_REGION_IS(PAL)
    setXY0Fast(tile, -256, -256);
    setWHFast(tile, 512, 4);
#else
    setXY0Fast(tile, -256, -FRAMEBUFFER_HEIGHT_PROGRESSIVE);
    setWHFast(tile, 512, 2);
#endif

    GsOUT_PACKET_P += sizeof(TILE);
    temp = func_801E3684();

    switch (g_GameWork.gameStateSteps[1])
    {
        case 0:
            switch (g_ScreenFade_Status)
            {
                case SCREEN_FADE_STATUS(ScreenFadeState_FadeOutComplete, true):
                    ScreenFade_Start(true, true, true);
                    break;

                case SCREEN_FADE_STATUS(ScreenFadeState_FadeOutComplete, false):
                    ScreenFade_Start(true, true, false);
                    break;
            }

            g_ScreenFadeTimestep = Q12(1.0f);
            g_GameWork.gameStateSteps[1]++;
            D_801E5E78 = SECONDS_TO_FRAMES(3);
            break;

        case 1:
            if (temp == 0)
            {
                break;
            }

            D_801E5E78--;
            if (Sd_AudioStreamingCheck() == AudioStreamingState_None)
            {
                g_GameWork.gameStateSteps[1]++;
            }
            break;

        case 2:
            D_801E5E78--;
            if (D_801E5E78 <= 0)
            {
                g_ScreenFade_Status          = g_GameWork.gameStateSteps[1];
                g_GameWork.gameStateSteps[1] = 3;
            }
            break;

        case 3:
            if (g_ScreenFade_Status == SCREEN_FADE_STATUS(ScreenFadeState_FadeOutComplete, false))
            {
                g_GameWork.gameStateSteps[1] = 4;
                return true;
            }
            break;

        default:
            break;
    }

    return false;
}

#include "stringtable.h"

/** Unknown Rodata values.
 * The types are assumed. It is unknown where
 * are this applied, they could be also some
 * sort of value defined with an macro.
*/
static const s16 D_801E2E1E = 0x50;    // 80
static const s32 D_801E2E20 = 0x1800;  // 6144
static const s32 D_801E2E24 = 0x21700; // 136960

s32 D_801E5C20 = 400;

#include "widthtable.h"

s32 g_CreditsColorTable[7] = {
    0x64FF0000,
    0x640000FF,
    0x64FF00FF,
    0x6400FF00,
    0x64FFFF00,
    0x6400FFFF,
    0x64FFFFFF,
};

s32 g_CreditsColorTable3d[7] = {
    0x2CFF0000,
    0x2C0000FF,
    0x2CFF00FF,
    0x2C00FF00,
    0x2CFFFF00,
    0x2C00FFFF,
    0x2CFFFFFF
};

u8 D_801E5E5C[16] = {
    '\x7F', '1', '9', 'A',
    'C', 'D', 'E', 'G',
    'H', 'I', 'K', 'L',
    'M', 'N', 'O', 'P'
};

u8 D_801E5E6C[8] = {
    '.', 'Y', 'V', 'U',
    'T', 'S', 'R',

    // TODO: Garbage padding, maybe can be moved to postbuild.
#if VERSION_REGION_IS(NTSC)
    '\x20'
#elif VERSION_IS(JAP0)
    '\x8D'
#elif VERSION_IS(JAP1)
    '\x04'
#elif VERSION_IS(JAP2)
    '\x10'
#endif
};

s32 D_801E5E74 = 0;

s32 D_801E5E78 = 0;

s32 D_801E5E7C = 0;

s32 D_801E5E80 = 0;

s32 D_801E5E84 = 0;

s32 D_801E5E88 = 0;

s32 D_801E5E8C = 0;

u8 D_801E5E90[320] = { 0 };

u8 D_801E5FD0[40] = { 0 };

u8 D_801E5FF8[20] = { 0 };

u32 D_801E600C = 0;

bool func_801E3684(void) // 0x801E3684
{
    s32  sp10;
    s32  sp14;
    s32  sp18;
    s32  temp_fp;
    s32  temp_v1;
    s32  var_s0;
    s32  var_s1;
    s32  var_s3;
    s32  var_s4;
    s32  var_s5;
    s32  var_s6;
    s32  var_v1;
    bool result;
    s8*  temp_s0;
    s32* var_s2;

    temp_v1 = D_800C48F0 * 2;
    var_s0  = (s32)((temp_v1 - 504) * D_801E5E80);
    var_s0  = var_s0 >> 16;
    var_s6  = D_801E5E84;
    sp10    = D_801E5E7C;

    if (var_s0 < 0)
    {
        var_s0 = 0;
    }

    var_v1 = (s32)(temp_v1 * D_801E5E80);
    var_v1 = var_v1 >> 16;
    if (var_v1 >= D_801E5C20)
    {
        var_v1 = D_801E5C20 - 1;
    }

    sp14 = ((var_s0 * sp10) - temp_v1) + SCREEN_HEIGHT;
    sp18 = (var_v1 - var_s0) + 1;

    var_s6  = (var_s6 - temp_v1);
    var_s6  = var_s6 + SCREEN_HEIGHT;
    temp_fp = var_s6 < SCREEN_HEIGHT;

    if (temp_fp != 0)
    {
        result    = var_s6 < -23;
        var_s4 = -1;

        if (result)
        {
            var_s6 = -24;
        }
    }
    else
    {
        result    = false;
        var_s4 = -1;
    }

    Credits_TextBlendSet(true, 1);
    Credits_TextColorSet(Q8_COLOR(0.19f), Q8_COLOR(0.19f), Q8_COLOR(0.19f));

    for (var_s5 = 2; var_s5 >= 0; var_s5--, var_s4 += 2)
    {
        if (var_s5 == 0)
        {
            var_s4 = 0;
            Credits_TextBlendSet(false, 0);
            Credits_TextColorSet(Q8_COLOR(0.2525f), Q8_COLOR(0.2525f), Q8_COLOR(0.2525f));
        }

        var_s3 = sp14 + var_s4;
        var_s2 = &g_CreditList[var_s0];

        for (var_s1 = sp18; var_s1 > 0; var_s1--, var_s2++, var_s3 += sp10)
        {
            temp_s0 = *var_s2;
            Credits_TextPositionSet(0, var_s3);
            Credits_TextDraw(temp_s0);
        }

        if (temp_fp != 0)
        {
            Credits_TextPositionSet(0, var_s6 + var_s4);
            Credits_TextDraw(D_801E5BD0);
        }
    }

    return result;
}

void func_801E386C(void) // 0x801E386C
{
    s_801E5558* ptr;
    s32         var0;
    s64         var1;
    s32         var2;

    var2 = 5;

    func_8009185C(0, 0, SCREEN_HEIGHT, 0x1000, 0, 0x22000, 0xF0000, 0xB33, 899, -0xF9C);
    ptr  = D_801E5558;
    ptr  = &ptr[5];
    var0 = (u16)ptr->field_4;

    var1 = (s16)var0;
    var1 = FP_TO(var1, Q12_SHIFT);
    var1 = FP_FROM(var1, Q12_SHIFT);

    D_801E5E8C = var2;
    D_801E5E7C = var1 - 264;
    D_800C48F0 = (s32)ptr->scrollDelay;
    D_801E5E7C = D_801E5E7C / D_801E5C20;
    D_801E5E84 = (D_801E5E7C * D_801E5C20) + SCREEN_HEIGHT;
    D_801E5E80 = 0x10000 / D_801E5E7C;
}

bool func_801E3970(void) // 0x801E3970
{
    bool   showKcet;
    bool   animateKcet;
    bool   isFinished;
    char*  lineStrPtr;
    char** currentLinePtr;
    q19_12 angle0;
    s32    temp_a2;
    s32    currentLinePosY;
    s32    lineHeight;
    s32    temp_v0;
    s32    temp_v0_4;
    s32    vBlank;
    s32    var_a0;
    s32    var_fp;
    s32    lineIdx;
    s32    i;
    s32    lineY;
    s32    angleInt;
    q19_12 sin;
    q19_12 cos;
    s32    linesToDraw;
    s32    var_t3;
    s32    skipTo;
    s32    skipToInt;

    showKcet    = false;
    animateKcet = false;
    isFinished  = false;

    vBlank = Q12_MULT_PRECISE(D_800C48F0, Q12(1.0f));

    temp_a2    = D_801E5E84;
    lineHeight = D_801E5E7C;

    lineIdx = (vBlank - 408) * D_801E5E80;
    lineIdx = lineIdx >> 16;
    if (lineIdx < 0)
    {
        lineIdx = 0;
    }

    var_a0 = vBlank * D_801E5E80;
    var_a0 = var_a0 >> 16;
    if (var_a0 >= D_801E5C20)
    {
        var_a0 = D_801E5C20 - 1;
    }

    temp_a2 = (temp_a2 - vBlank);
    var_fp  = temp_a2 + 0x78;

    temp_a2 = var_fp;
    angleInt  = 0;

    showKcet = var_fp < 0x78;

    var_t3          = lineIdx * lineHeight;
    temp_v0         = var_t3 - vBlank;
    currentLinePosY = temp_v0 + 0x78;

    temp_v0     = (var_a0 - lineIdx);
    linesToDraw = temp_v0 + 1;

    if (showKcet)
    {
        animateKcet = var_fp < -0x2F;
        if (animateKcet)
        {
            var_fp = -0x30;
            angleInt = -0x30 - temp_a2;
        }
    }
    else
    {
        animateKcet = false;
    }

    Credits_TextColorSet(Q8_COLOR(0.6275f), Q8_COLOR(0.6275f), Q8_COLOR(0.6275f));
    Credits_TextBlendSet(false, 0);

    lineY          = currentLinePosY;
    currentLinePtr = &g_CreditList[lineIdx];

    for (i = linesToDraw; i > 0; i--, lineY += lineHeight, currentLinePtr++)
    {
        lineStrPtr = *currentLinePtr;
        Credits_Text3dPositionSet(0, lineY);
        Credits_Text3dDraw(lineStrPtr);
    }

    if (animateKcet)
    {
        angle0 = angleInt;

        // Rotate.
        if (angleInt < SECONDS_TO_FRAMES(2)) // TODO: Called angle in this func but the values change based on 50/60Hz?
        {
            angle0 = (angleInt * Q12(1.0f)) / SECONDS_TO_FRAMES(2);
            angle0 = Q12_MULT_PRECISE((Q12_ANGLE(360.0f) - angle0), Q12_ANGLE(77.3f));
            sin     = -Math_Sin(angle0);
            cos     = Math_Cos(angle0);
            func_8009185C(0, 0, 240, Q12(1.0f), 0, Q12(34.0f), Q12(240.0f), Q12(0.7f), cos, sin);
        }
        // Zoom in.
        else if (angleInt < SECONDS_TO_FRAMES(3))
        {
            angle0   = angleInt - SECONDS_TO_FRAMES(2);
            angle0   = (angle0 * Q12(1.0f)) / SECONDS_TO_FRAMES(1);
            temp_v0_4 = Q12_MULT_PRECISE((Q12(1.0f) - angle0), Q12(30.0f)) + Q12(210.0f);
            func_8009185C(0, 0, 240, Q12(1.0f), 0, Q12(34.0f), temp_v0_4, Q12(0.7f), Q12(1.0f), 0);
        }
        else if (angleInt >= SECONDS_TO_FRAMES(6))
        {
            // Zoom out.
            if (angleInt < SECONDS_TO_FRAMES(8))
            {
                angle0   = angleInt - SECONDS_TO_FRAMES(6);
                angle0   = (angle0 * Q12(1.0f)) / SECONDS_TO_FRAMES(2);
                temp_v0_4 = Q12_MULT_PRECISE((Q12(1.0f) - angle0), Q12(-3630.0f)) + Q12(3840.0f);
                func_8009185C(0, 0, 240, Q12(1.0f), 0, Q12(34.0f), temp_v0_4, Q12(0.7f), Q12(1.0f), 0);
            }
            // Hide and finish.
            else
            {
                isFinished = true;
                showKcet   = false;
            }
        }
    }
    else if (((g_GameWork.config.extraOptionsEnabled >> (D_801E5E8C - 1)) & (1 << 0)) &&
             (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.skip))
    {
        skipTo = D_801E5E84 + 168;
        skipTo = Q12_DIV(skipTo, Q12(1.0f));
        D_800C48F0 = skipTo;
        SD_Call(19);
    }

    if (showKcet)
    {
        Credits_Text3dPositionSet(0, var_fp);
        Credits_Text3dDraw(D_801E5BD0);
    }

    return isFinished;
}

void func_801E3DD0(void) // 0x801E3DD0
{
    D_801E600C = Rng_Rand16();
}

s32 func_801E3DF8(u8 arg0) // 0x801E3DF8
{
    return ((u32)(arg0 * 0x31) >> 6) | 0x38;
}

void func_801E3E18(s32 arg0, s32 arg1) // 0x801E3E18
{
    s32      j;
    s32      i;
    PACKET*  packet;
    s32*     addr;
    u8*      bufA;
    u8*      bufB;
    POLY_G4* poly;
    u32      x0;
    u16      x1;
    u16      x2;
    u16      x3;
    u32      color0;
    u32      color1;
    u32      color2;
    u32      color3;
    u8*      ptr;
    u8*      ptr2;
    s32      temp_t6;
    s32      temp_t1;
    u32      sp1C;
    u32      sp20;
    u32      sp24;
    u32      sp28;
    u32      sp2C;
    u32      temp_v1_2;
    s32      var_s4;
    s32*     var_a0;
    s32*     var_a1_2;
    u32      temp_a2;
    u32      temp_v1;
    u32      var_v1;
    u8       temp_a0;
    u32      var_a1;
    u32      var_a2;
    u32      var_a3;
    s32*     var_a0_2;
    u8*      var_s1_2;
    u8*      var_s2_2;
    u8*      var_t0;
    s32*     temp_t2;

    packet  = GsOUT_PACKET_P;
    temp_t2 = &g_OtTags0[g_ActiveBufferIdx][6];
    addr    = temp_t2 + 1;
    temp_a2 = D_801E600C;

    bufA = &D_801E5FD0[0];

    for (i = 1; i < 17; i++)
    {
        temp_a2 = temp_a2 * 0x19660D + 0x3C6EF35F;
        temp_v1 = (temp_a2 >> 20);
        temp_v1 -= 0x200;
        if (temp_v1 >= 0xC00)
        {
            var_v1 = 0xFF;
        }
        else if (temp_v1 >= 0xA00)
        {
            temp_v1 = (temp_v1 - 0xE00) >> 3;
            var_v1  = temp_v1 + 192;
        }
        else if (temp_v1 < 0x200u)
        {
            var_v1 = temp_v1 >> 3;
        }
        else
        {
            temp_v1 = (temp_v1 - 0x200) >> 4;
            var_v1  = temp_v1 + 64;
        }

        D_801E5FD0[i] = var_v1;
    }

    D_801E600C     = temp_a2;
    D_801E5FD0[0]  = D_801E5FD0[16];
    D_801E5FD0[17] = D_801E5FD0[1];

    bufA = &D_801E5FD0[0];
    bufB = &D_801E5FD0[20];

    j       = 0;
    var_s4  = 0;
    temp_t1 = arg0 + 480;
    sp1C    = 0xFFFFFF;
    temp_t6 =
    sp28    = temp_t1 << 16;
    sp2C    = temp_t6 + 0x200000;

    for (;
         j < 16;
         j++, var_s4 -= 16, sp28 += 0xFFE00000, sp2C += 0xFFE00000, temp_t1 -= 32)
    {
        ptr2 = D_801E5FF8;

        ptr  = bufB;
        bufB = bufA;
        bufA = ptr;

        ptr = D_801E5E90 + (j * 20);

        var_a1_2 = (s32*)D_801E5FF8;
        var_a0   = (s32*)ptr;

        for (i = 0; i < 20; i += 4)
        {
            var_a1_2[i * 5] = var_a0[i * 5];
        }

        var_s2_2 = ptr2;
        var_a1   = *var_s2_2++;
        var_a3   = *var_s2_2++;

        var_t0 = bufA + 1;

        var_s1_2 = bufB;
        var_a2   = *bufB;
        var_s1_2++;

        for (i = 16; i > 0; i--)
        {
            q3_12 var = Q12(1.0f);

            temp_v1_2 = var_a1;

            temp_a0 = var_a2;
            var_a1  = var_a3;
            temp_v1_2 += temp_a0;

            var_a3 = *var_s2_2;
            var_a2 = *var_s1_2;
            var_a3 += var_a2;

            var_s2_2++;
            var_s1_2++;

            var_a1 = var_a1 + (((temp_v1_2 * (var - arg1)) + (var_a3 * arg1)) >> 13);
            var_a1 = var_a1 + ((temp_a0 + var_a2) >> 1);
            var_a1 = (((var_a1 + var_a2 + temp_a0) * 2) + (var_a3 + temp_v1_2)) >> 1;
            var_a1--;

            *var_t0 = var_a1;
            var_t0++;
        }

        bufA[0]  = bufA[16];
        bufA[17] = bufA[1];

        var_a1_2 = (s32*)ptr;
        var_a0_2 = (s32*)bufA;

        for (i = 0; i < 20; i += 4)
        {
            var_a1_2[i * 5] = var_a0_2[i * 5];
        }

        if (temp_t1 >= 480)
        {
            continue;
        }

        var_s2_2 = bufA;
        color1   = func_801E3DF8(*var_s2_2++);

        var_s1_2 = bufB;
        color3   = func_801E3DF8(*var_s1_2++);

        sp20 = sp2C;
        sp24 = sp28;

        for (i = 0; i < 16; i++)
        {
            color0 = color1;
            color1 = func_801E3DF8(*var_s2_2++);
            var_s2_2++;

            color2 = color3;
            color3 = func_801E3DF8(*var_s1_2++);
            var_s1_2++;

            var_v1 = 0x200;
            var_a1 = var_v1;
            x0     = var_s4 + var_a1;

            if ((var_s4 + 48) > 0)
            {
                x0 = var_s4;
                if (var_s4 < 0)
                {
                    poly = (POLY_G4*)packet;
                    // addPrimFast(addr, poly, 8);
                    poly->tag = ((*addr & sp1C) | 0x08000000);
                    *addr     = (*addr & 0xFF000000) | ((u32)poly & sp1C);

                    x0 = var_s4 + 0x200;
                    x1 = var_s4 + 0x220;
                    x2 = var_s4 + 0x210;
                    x3 = var_s4 + 0x230;

                    *(u32*)(&poly->x0) = (((x0) & 0xFFFF) + (sp28));
                    *(u32*)(&poly->x1) = (((x1) & 0xFFFF) + (sp28));
                    *(u32*)(&poly->x2) = (((x2) & 0xFFFF) + (sp20));
                    *(u32*)(&poly->x3) = (((x3) & 0xFFFF) + (sp20));

                    setRGBC0Word(poly, color0);
                    setRGBC1Word(poly, color1);
                    setRGBC2Word(poly, color2);
                    setRGBC3Word(poly, color3);

                    packet += sizeof(POLY_G4);

                    x0 = var_s4;
                }
            }

            x1 = x0 + 32;
            x2 = x0 + 16;
            x3 = x0 + 48;

            poly = (POLY_G4*)packet;
            // addPrimFast(addr, poly, 8);
            poly->tag = ((*addr & sp1C) | 0x08000000);
            *addr     = (*addr & 0xFF000000) | ((u32)poly & sp1C);

            *(u32*)(&poly->x0) = (((x0) & 0xFFFF) + (sp24));
            *(u32*)(&poly->x1) = (((x1) & 0xFFFF) + (sp24));
            *(u32*)(&poly->x2) = (((x2) & 0xFFFF) + (sp20));
            *(u32*)(&poly->x3) = (((x3) & 0xFFFF) + (sp20));

            setRGBC0Word(poly, color0);
            setRGBC1Word(poly, color1);
            setRGBC2Word(poly, color2);
            setRGBC3Word(poly, color3);

            packet += sizeof(POLY_G4);
        }
    }
    GsOUT_PACKET_P = packet;
}

void Credits_TextPositionSet(s32 x, s32 y) // 0x801E42F8
{
    g_CreditTextState.textX = x;
    g_CreditTextState.textY = y;
    g_CreditTextState.leftMargin = x;
}

void Credits_TextColorSet(s32 r, s32 g, s32 b) // 0x801E4310
{
    g_CreditTextState.color = COLOR_RGBC(r, g, b, PRIM_RECT | RECT_TEXTURE);
}

void Credits_TextLineHeightSet(s8 arg0) // 0x801E4340
{
    g_CreditTextState.lineHeight = arg0;
}

void Credits_TextBlendSet(bool semiTrans, u32 arg1) // 0x801E434C
{
    u32 shiftedArg1;
    u32 shiftedField_18;
    u32 maskedField_18;

    g_CreditTextState.semiTrans = semiTrans != 0;

    if (arg1 < 4)
    {
        // TODO: Might be a `getTPage` macro?
        maskedField_18  = g_CreditTextState.uv;
        shiftedField_18 = (maskedField_18 << 4) & (1 << 8);

        maskedField_18   &= 0xF;
        shiftedField_18 >>= 4;

        shiftedArg1   = arg1 & 3;
        shiftedArg1 <<= 5;
        shiftedArg1  |= shiftedField_18;
        shiftedArg1  |= (u8)maskedField_18;

        g_CreditTextState.tpage = shiftedArg1;
    }
}

void Credits_TextDraw(char* str) // 0x801E4394
{
    s32       textX;
    s32       textY;
    s32       leftMargin;
    s32       lineHeight;
    u32       blendFlag;
    u32       charCode;
    u32       colorCode;
    u32       clut;
    s32       charWidth;
    s32       widthSum;
    s32       idx;
    u32       temp;
    u32       nextChar;
    u8*       strPtr;
    u8*       scanPtr;
    s16*      widthTable;
    s32*      colorTable;
    PACKET*   packet;
    SPRT*     sprite;
    SPRT_16*  sprite16;
    DR_TPAGE* tPage;
    GsOT*     ot;

    strPtr     = str;
    textX      = g_CreditTextState.textX;
    textY      = g_CreditTextState.textY;
    leftMargin = g_CreditTextState.leftMargin;
    lineHeight = g_CreditTextState.lineHeight;
    widthTable = g_CreditTextState.widthTable;
    colorTable = g_CreditTextState.colorTable;
    packet     = GsOUT_PACKET_P;
    blendFlag  = (u8)g_CreditTextState.semiTrans;
    charCode   = *strPtr;
    colorCode  = g_CreditTextState.color | (blendFlag << 25);  // RBG + code + semi-transparency flag.
    clut       = (u16)g_CreditTextState.clut;                  // Clut Y, clut Y.
    ot         = &g_OtTags0[g_ActiveBufferIdx][6];

    while (charCode != 0)
    {
        charWidth = widthTable[charCode];

        // Normal font.
        if (charCode >= '!' && charCode < 133)
        {
            sprite = (SPRT*)packet;
            addPrimFast(ot, sprite, 4);
            setRGBC0Word(sprite, colorCode);
            setXY0Fast(sprite, textX, textY);

            idx                   = charCode - '!';
            temp                  = idx / 10;
            idx                   = idx % 10;
            idx                   = idx * 24;
            *(u32*)(&sprite->u0)  = idx + ((temp * 24) * 256);
            *(u32*)(&sprite->u0) += clut << 16;

            setWHFast(sprite, 24, 24);
            packet += sizeof(SPRT);
        }
        else
        {
            sprite16 = (SPRT_16*)packet;

            // KCET font.
            if (charCode >= 160 && charCode < 176)
            {
                addPrimFast(ot, sprite16, 3);
                setRGBC0Word(sprite16, colorCode ^ (24 << 24)); // GP0(7Ch) - Textured Rectangle, 16x16, opaque, texture-blending.
                setXY0Fast(sprite16, textX, textY + 4);

                idx                     = charCode - 160;
                *(u32*)(&sprite16->u0)  = ((idx * 256 )* 16) + (16 * (16 - 1));
                *(u32*)(&sprite16->u0) += clut << 16;

                packet += sizeof(SPRT_16);
            }
            // KCET font, .YVUTSR.
            else if (charCode >= 184 && charCode < 191)
            {
                addPrimFast(ot, sprite16, 3);
                setRGBC0Word(sprite16, colorCode ^ (24 << 24)); // GP0(7Ch) - Textured Rectangle, 16x16, opaque, texture-blending.
                setXY0Fast(sprite16, textX, textY + 4);

                idx                     = charCode - 184;
                *(u32*)(&sprite16->u0)  = (idx * 16) + (((256 * 16) * (16 - 1)) + (16 * 8));
                *(u32*)(&sprite16->u0) += clut << 16;

                packet += sizeof(SPRT_16);
            }
            // Change font color.
            else if (charCode >= 1 && charCode < 8)
            {
                colorCode = colorTable[charCode - 1] | ((u8)g_CreditTextState.semiTrans << 25);
            }
            // Change font width table?
            else if (charCode >= 208 && charCode < 216)
            {
                scanPtr  = strPtr + 1;
                widthSum = 0;
                while (1)
                {
                    s32 var  = 8;
                    nextChar = *scanPtr++;
                    if (nextChar == (charCode + 8) ||
                        (nextChar == charCode || nextChar == 10) ||
                        nextChar == 13 ||
                        nextChar == 12)
                    {
                        break;
                    }

                    if ((nextChar - 240) >= var &&
                        (nextChar - 248) > 7 &&
                        (nextChar - 224) > 7 &&
                        (nextChar - 232) > 7)
                    {
                        widthSum += widthTable[nextChar];
                    }
                }

                (&widthTable[charCode])[16] = widthSum / 2;
                (&widthTable[charCode])[32] = -(&widthTable[charCode])[16];
                (&widthTable[charCode])[24] = widthSum;
                (&widthTable[charCode])[40] = -(&widthTable[charCode])[24];
            }
        }

        // Offsets.
        switch ((s32)charCode)
        {
            // Newline.
            case '\n':
                textX  = leftMargin;
                textY += lineHeight;
                break;

            // Vertical tab.
            case '\v':
                textY += lineHeight;
                break;

            // Carriage return.
            case '\r':
                textX = leftMargin;
                break;

            // Form feed.
            case '\f':
                textX = g_CreditTextState.textX;
                textY = g_CreditTextState.textY;
                break;

            default:
                textX += charWidth;
                break;
        }

        strPtr++;
        charCode = *strPtr;
    }

    tPage = (DR_TPAGE*)packet;
    setDrawTPage(tPage, 0, 1, g_CreditTextState.tpage);
    addPrim(ot, tPage);

    g_CreditTextState.textX = textX;
    g_CreditTextState.textY = textY;
    GsOUT_PACKET_P          = packet + sizeof(DR_TPAGE);
}

void Credits_Text3dPositionSet(s32 arg0, s32 arg1) // 0x801E47E0
{
    s32  temp_a2;
    s32  temp_lo;
    s32  temp_lo_11;
    s32  temp_lo_12;
    s32  temp_lo_15;
    s32  temp_lo_3;
    s32  temp_lo_4;
    s32  temp_lo_6;
    s32  temp_lo_7;
    s32  temp_t0;
    s32  temp_t1;
    s32  temp_t2;
    s32  temp_v0;
    s32  temp_v0_2;
    s32  temp_v0_3;
    s32  temp_v0_4;
    bool check = g_CreditText3dState.sub_0.textX != arg0;

    if (g_CreditText3dState.sub_0.textY != arg1)
    {
        temp_lo   = arg1 * g_CreditText3dState.field_3C;
        temp_t1   = g_CreditText3dState.field_24 << 0x16;
        temp_lo_3 = (g_CreditText3dState.field_30 + (s32)Q12_MULT_PRECISE(g_CreditText3dState.field_1C, temp_lo)) >> 2;
        temp_lo_3 = temp_t1 / temp_lo_3;

        temp_t2   = arg1 + 24;
        temp_lo_4 = temp_t2 * g_CreditText3dState.field_3C;
        temp_lo_6 = (g_CreditText3dState.field_30 + (s32)Q12_MULT_PRECISE(g_CreditText3dState.field_1C, temp_lo_4)) >> 2;
        temp_lo_6 = temp_t1 / temp_lo_6;

        temp_lo_7 = arg0 * g_CreditText3dState.field_34;
        temp_v0   = g_CreditText3dState.field_28 + Q12_MULT_PRECISE(g_CreditText3dState.field_1C, temp_lo_7);
        temp_t0   = Q12_MULT_PRECISE(temp_lo_3, temp_v0);
        temp_v0_2 = g_CreditText3dState.field_28 + Q12_MULT_PRECISE(g_CreditText3dState.field_1C, g_CreditText3dState.field_34);

        g_CreditText3dState.sub_0.textX  = arg0;
        g_CreditText3dState.sub_0.textY  = arg1;
        g_CreditText3dState.sub_0.leftMargin  = arg0;
        g_CreditText3dState.field_40 = temp_t0;
        g_CreditText3dState.field_44 = Q12_MULT_PRECISE(temp_lo_6, temp_v0);
        g_CreditText3dState.field_48 = Q12_MULT_PRECISE(temp_lo_3, temp_v0_2) - Q12_MULT_PRECISE(temp_lo_3, g_CreditText3dState.field_28);
        g_CreditText3dState.field_4C = Q12_MULT_PRECISE(temp_lo_6, temp_v0_2) - Q12_MULT_PRECISE(temp_lo_6, g_CreditText3dState.field_28);
        temp_lo_11          = arg1 * g_CreditText3dState.field_38;
        temp_v0_3           = g_CreditText3dState.field_2C + Q12_MULT_PRECISE(g_CreditText3dState.field_1C, temp_lo_11);
        g_CreditText3dState.field_50 = Q12_MULT_PRECISE(temp_lo_3, temp_v0_3);

        temp_lo_12          = temp_t2 * g_CreditText3dState.field_38;
        temp_t0             = g_CreditText3dState.field_2C + Q12_MULT_PRECISE(g_CreditText3dState.field_1C, temp_lo_12);
        g_CreditText3dState.field_54 = Q12_MULT_PRECISE(temp_lo_6, temp_t0);
    }
    else if (check)
    {
        temp_v0_4            = arg0 - g_CreditText3dState.sub_0.textX;
        temp_a2              = temp_v0_4 * g_CreditText3dState.field_48;
        temp_lo_15           = temp_v0_4 * g_CreditText3dState.field_4C;
        g_CreditText3dState.sub_0.textX   = arg0;
        g_CreditText3dState.field_40 += temp_a2;
        g_CreditText3dState.field_44 += temp_lo_15;
    }
}

void Credits_Text3dColorSet(s32 r, s32 g, s32 b) // 0x801E4B98
{
    g_CreditText3dState.sub_0.color = (r & 0xFF) | ((g & 0xFF) << 8) | ((b & 0xFF) << 16) | (GPU_COM_TF4 << 24);
}

void Credits_Text3dLineHeightSet(s8 arg0) // 0x801E4BC8
{
    g_CreditText3dState.sub_0.lineHeight = arg0;
}

void Credits_Text3dBlendSet(bool semiTrans, u32 arg1) // 0x801E4BD4
{
    u32 shiftedArg1;
    u32 shiftedField_18;
    u32 maskedField_18;

    g_CreditText3dState.sub_0.semiTrans = (semiTrans != 0);

    if (arg1 < 4)
    {
        maskedField_18  = g_CreditText3dState.sub_0.uv;
        shiftedField_18 = (maskedField_18 << 4) & 0x100;

        maskedField_18   &= 0xF;
        shiftedField_18 >>= 4;

        shiftedArg1   = arg1 & 0x3;
        shiftedArg1 <<= 5;
        shiftedArg1  |= shiftedField_18;
        shiftedArg1  |= (u8)maskedField_18;

        g_CreditText3dState.sub_0.tpage = shiftedArg1;
    }
}

void Credits_Text3dDraw(u8* str) // 0x801E4C1C
{
    PACKET* packet;
    GsOT*   ot;

    s32 textX      = g_CreditText3dState.sub_0.textX;
    s32 textY      = g_CreditText3dState.sub_0.textY;
    s32 leftMargin = g_CreditText3dState.sub_0.leftMargin;
    s32 lineHeight = g_CreditText3dState.sub_0.lineHeight;
    u32 colorCode;

    q19_12 var_a3 = g_CreditText3dState.field_40;
    q19_12 var_t3 = g_CreditText3dState.field_44;
    q19_12 var_t7 = g_CreditText3dState.field_48;
    q19_12 var_t6 = g_CreditText3dState.field_4C;
    q19_12 var_t4 = g_CreditText3dState.field_50;
    q19_12 var_t5 = g_CreditText3dState.field_54;

    u32       clut       = (u16)g_CreditText3dState.sub_0.clut;
    u32       tPage      = g_CreditText3dState.sub_0.tpage;
    s16*      widthTable = g_CreditText3dState.sub_0.widthTable;
    s32*      colorTable = g_CreditText3dState.sub_0.colorTable;
    u32       blendFlag  = (u8)g_CreditText3dState.sub_0.semiTrans << 25;
    s32       charWidth;
    bool      textXChanged;
    bool      textYChanged;
    u32       nextChar;
    u8*       scanPtr;
    POLY_FT4* poly;
    s32       widthSum;
    u32       charCode;

    s32 temp_a0_4;
    s32 temp_a1_2;
    s32 temp_lo_3;
    s32 temp_lo_6;
    s32 temp_v0_5;
    s32 temp_v0_7;

    s32 var_a1;
    s32 var_a2;
    s32 var_t0;
    s32 var_t0_2;

    u32 temp_v1;
    u32 temp;
    s32 idx;

    packet = GsOUT_PACKET_P;
    ot     = &g_OtTags0[g_ActiveBufferIdx][6];

    charCode  = *str;
    colorCode = g_CreditText3dState.sub_0.color | blendFlag; // RGB + code + semi-transparency flag.

    while (charCode != 0)
    {
        poly = (POLY_FT4*)packet;

        var_t0 = (charCode - 160) < 0x10u | ((charCode - 184) < 7u) << 1;

        // Map KCET font to normal.
        if (var_t0 != 0)
        {
            if (var_t0 == 1)
            {
                charCode = D_801E5E5C[charCode - 160];
            }
            else
            {
                charCode = D_801E5E6C[charCode - 184];
            }
        }

        charWidth = widthTable[charCode];

        // Normal font.
        if (charCode >= 33 && charCode < 133)
        {
            s32 offset         = 24 * 256;
            idx                = charCode - 33;
            temp               = idx / 10;
            idx                = idx % 10;
            idx                = idx * 24;
            temp               = temp * 24;
            *(u32*)(&poly->u0) = idx + (temp * 256);
            *(u32*)(&poly->u0) += clut << 16;
            *(u32*)(&poly->u1) = (idx + charWidth) + (temp * 256);
            *(u32*)(&poly->u1) += tPage << 16;
            temp_v1            = temp * (u64)256; // From permuter, not sure why this works.
            *(u32*)(&poly->u2) = idx + (temp_v1 + offset);
            *(u32*)(&poly->u3) = (idx + charWidth) + (temp_v1 + offset);

            if (var_t0 != 0)
            {
                charWidth = (charWidth * 2) / 3;
                var_a1    = var_t7 * charWidth;
                var_a2    = var_t6 * charWidth;
                var_a2    = ((var_a2 - var_a1) * 2) / 3 + var_a1;
                var_t0_2  = ((var_t5 - var_t4) * 2) / 3 + var_t4;
            }
            else
            {
                var_a1   = var_t7 * charWidth;
                var_t0_2 = var_t5;
                var_a2   = var_t6 * charWidth;
            }

            addPrimFast(ot, (POLY_FT4*)packet, 9);
            setRGBC0Word(poly, colorCode); // GP0(2Ch) - Textured four-point polygon, opaque, texture-blending.
            setXY0Fast(poly, FP_FROM(var_a3, Q12_SHIFT), FP_FROM(var_t4, Q12_SHIFT));
            setXY1Fast(poly, FP_FROM(var_a3 + var_a1, Q12_SHIFT), FP_FROM(var_t4, Q12_SHIFT));
            setXY2Fast(poly, FP_FROM(var_t3, Q12_SHIFT), FP_FROM(var_t0_2, Q12_SHIFT));
            setXY3Fast(poly, FP_FROM(var_t3 + var_a2, Q12_SHIFT), FP_FROM(var_t0_2, Q12_SHIFT));
            packet += sizeof(POLY_FT4);
        }
        // Change font color.
        else if (charCode >= 1 && charCode < 8)
        {
            colorCode = colorTable[charCode - 1] | ((u8)g_CreditText3dState.sub_0.semiTrans << 25);
        }
        // Change font width table?
        else if (charCode >= 208 && charCode < 216)
        {
            widthSum = 0;
            scanPtr  = str + 1;
            while (1)
            {
                s32 var  = 8;
                nextChar = *scanPtr++;
                if (nextChar == (charCode + 8) || nextChar == charCode || nextChar == 10 || nextChar == 13 || nextChar == 12)
                {
                    break;
                }

                if ((nextChar - 240) >= var && (nextChar - 248) >= 8u && (nextChar - 224) >= 8u && (nextChar - 232) >= 8u)
                {
                    var_t0 = (nextChar >= 160 && nextChar < 176) | (nextChar >= 184 && nextChar < 191) << 1;
                    if (var_t0 != 0) // Map KCET font to normal.
                    {
                        if (var_t0 == 1)
                        {
                            nextChar = D_801E5E5C[nextChar - 160];
                        }
                        else
                        {
                            nextChar = D_801E5E6C[nextChar - 184];
                        }
                        widthSum += (widthTable[nextChar] * 2) / 3;
                    }
                    else
                    {
                        widthSum += widthTable[nextChar];
                    }
                }
            }

            (&widthTable[charCode])[16] = widthSum / 2;
            (&widthTable[charCode])[32] = -(&widthTable[charCode])[16];
            (&widthTable[charCode])[24] = widthSum;
            (&widthTable[charCode])[40] = -(&widthTable[charCode])[24];
        }

        textXChanged = false;
        textYChanged = false;

        // Offsets.
        switch ((s32)charCode)
        {
            // Newline.
            case '\n':
                textX        = leftMargin;
                textY       += lineHeight;
                textYChanged = true;
                break;

            // Vertical tab.
            case '\v':
                textY       += lineHeight;
                textYChanged = true;
                break;

            // Carriage return.
            case '\r':
                charWidth    = textX - leftMargin;
                textX        = leftMargin;
                textXChanged = true;
                break;

            // Form feed.
            case '\f':
                textX        = g_CreditText3dState.sub_0.textX;
                textY        = g_CreditText3dState.sub_0.textY;
                textYChanged = true;
                break;

            default:
                textX       += charWidth;
                textXChanged = true;
                break;
        }

        if (textYChanged)
        {
            temp_a1_2 = g_CreditText3dState.field_24 << 22;

            temp_lo_3 = (g_CreditText3dState.field_30 + (s32)Q12_MULT_PRECISE(g_CreditText3dState.field_1C, textY * g_CreditText3dState.field_3C)) >> 2;
            temp_lo_3 = temp_a1_2 / temp_lo_3;

            temp_lo_6 = (g_CreditText3dState.field_30 + (s32)Q12_MULT_PRECISE(g_CreditText3dState.field_1C, (textY + 24) * g_CreditText3dState.field_3C)) >> 2;
            temp_lo_6 = temp_a1_2 / temp_lo_6;

            temp_v0_5 = g_CreditText3dState.field_28 + Q12_MULT_PRECISE(g_CreditText3dState.field_1C, textX * g_CreditText3dState.field_34);

            var_a3 = Q12_MULT_PRECISE(temp_lo_3, temp_v0_5);
            var_t3 = Q12_MULT_PRECISE(temp_lo_6, temp_v0_5);

            temp_v0_5 = g_CreditText3dState.field_28 + Q12_MULT_PRECISE(g_CreditText3dState.field_1C, g_CreditText3dState.field_34);

            var_t7 = Q12_MULT_PRECISE(temp_lo_3, temp_v0_5) - Q12_MULT_PRECISE(temp_lo_3, g_CreditText3dState.field_28);
            var_t6 = Q12_MULT_PRECISE(temp_lo_6, temp_v0_5) - Q12_MULT_PRECISE(temp_lo_6, g_CreditText3dState.field_28);

            temp_v0_7 = g_CreditText3dState.field_2C + Q12_MULT_PRECISE(g_CreditText3dState.field_1C, textY * g_CreditText3dState.field_38);
            var_t4    = Q12_MULT_PRECISE(temp_lo_3, temp_v0_7);

            temp_a0_4 = g_CreditText3dState.field_2C + Q12_MULT_PRECISE(g_CreditText3dState.field_1C, (textY + 24) * g_CreditText3dState.field_38);
            var_t5    = Q12_MULT_PRECISE(temp_lo_6, temp_a0_4);
        }
        else if (textXChanged)
        {
            var_a3 += charWidth * var_t7;
            var_t3 += charWidth * var_t6;
        }

        str++;
        charCode = *str;
    }

    GsOUT_PACKET_P                  = packet;
    g_CreditText3dState.sub_0.textX = textX;
    g_CreditText3dState.sub_0.textY = textY;
    g_CreditText3dState.field_40    = var_a3;
    g_CreditText3dState.field_44    = var_t3;
    g_CreditText3dState.field_48    = var_t7;
    g_CreditText3dState.field_4C    = var_t6;
    g_CreditText3dState.field_50    = var_t4;
    g_CreditText3dState.field_54    = var_t5;
}
