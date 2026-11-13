#include "bodyprog/game.h"
#include "inline_no_dmpsx.h"

#include <psyq/gtemac.h>
#include <psyq/libapi.h>
#include <psyq/strings.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/paper_map/paper_map.h"
#include "bodyprog/events/event_data.h"
#include "bodyprog/events/radio.h"
#include "bodyprog/math/math.h"
#include "bodyprog/screen/data.h"
#include "bodyprog/screen/draw.h"
#include "bodyprog/items/item_screens.h"
#include "bodyprog/player/player.h"
#include "bodyprog/sound/sd_call.h"
#include "main/rng.h"

// Used by `func_80068CC0` and `GameState_PaperMapScreen_Update`.
// I think this is save file flags related to
// placing the markings on the map.
/** `[1]` contains relative paper map file indices. */
static u8 D_800AE740[][2] = {
    { 0x00, 0x00 },
    { 0x00, 0x00 },
    { 0x00, 0x00 },
    { 0x00, 0x00 },
    { 0x00, 0x00 },
    { 0x06, 0x00 },
    { 0x07, 0x05 },
    { 0x08, 0x06 },
    { 0x00, 0x07 },
    { 0x0A, 0x00 },
    { 0x0B, 0x09 },
    { 0x0C, 0x0A },
    { 0x00, 0x0B },
    { 0x0E, 0x00 },
    { 0x00, 0x0D },
    { 0x00, 0x00 },
    { 0x11, 0x00 },
    { 0x12, 0x10 },
    { 0x13, 0x11 },
    { 0x00, 0x12 },
    { 0x15, 0x00 },
    { 0x16, 0x14 },
    { 0x17, 0x15 },
    { 0x00, 0x16 }
};

static bool D_800AE770 = false; // Used by `GameState_PaperMapScreen_Update`.

// `D_800AE774`. `D_800AE7E4`, `D_800AE820` are used by `func_80067914`. TODO: Use `DVECTOR` for these?
static s8 D_800AE774[8][7][2] = {
    { { 0, 0 }, { 0, 0 }, { -43, 90 }, { -48, 72 }, { -51, 42 }, { -48, -32 }, { -47, 68 } },
    { { 0, 0 }, { 0, 0 }, { -1, 82 }, { -33, -2 }, { -8, -80 }, { 64, -80 }, { 0, 0 } },
    { { 0, 0 }, { -36, -74 }, { 72, 80 }, { 37, 1 }, { 28, -80 }, { 104, -75 }, { 0, 0 } },
    { { 31, -3 }, { 33, -43 }, { 32, 56 }, { 112, 32 }, { 112, -32 }, { 0, 0 }, { 54, 1 } },
    { { 66, 100 }, { 65, 65 }, { -48, 88 }, { -48, 72 }, { -43, 37 }, { -48, -32 }, { 0, 0 } },
    { { 64, 65 }, { 111, 77 }, { 4, 83 }, { -28, -2 }, { 0, 0 }, { 68, 84 }, { 0, 0 } },
    { { 0, 0 }, { 0, 0 }, { 32, 80 }, { 47, 1 }, { -4, -82 }, { 32, -80 }, { 0, 0 } },
    { { 0, 0 }, { 0, 0 }, { 32, 56 }, { 107, 32 }, { 117, -32 }, { 89, -80 }, { 64, 1 } }
};

static s8 D_800AE7E4[5][6][2] = {
    { { 0, 0 }, { 60, -83 }, { 87, -81 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
    { { 0, 0 }, { 98, -36 }, { 68, -19 }, { 102, -1 }, { 0, 0 }, { -53, -25 } },
    { { 0, 0 }, { 59, -40 }, { 111, -84 }, { 104, 41 }, { 32, 11 }, { 0, 0 } },
    { { 0, 0 }, { 0, 0 }, { 102, -49 }, { 102, 69 }, { 1, 6 }, { 0, 0 } },
    { { 0, 0 }, { 104, -25 }, { 104, -24 }, { 0, 0 }, { -10, -41 }, { 0, 0 } }
};

static s8 D_800AE820[8][8][2] = {
    { { 0, 0 }, { 32, 11 }, { -40, 48 }, { -5, 72 }, { 62, 71 }, { -39, -62 }, { 68, -66 }, { 0, 0 } },
    { { 0, 0 }, { 68, -66 }, { -40, -66 }, { 0, 0 }, { 98, 79 }, { -35, -3 }, { 60, 1 }, { 0, 0 } },
    { { 0, 0 }, { 68, 38 }, { -40, -12 }, { 56, 78 }, { 110, 24 }, { 0, 0 }, { 14, -13 }, { 0, 0 } },
    { { 0, 0 }, { -4, -17 }, { -40, 13 }, { 104, -18 }, { 109, -53 }, { 0, 0 }, { -46, 66 }, { 32, 12 } },
    { { 0, 0 }, { 0, 0 }, { 31, -66 }, { 32, 12 }, { -10, 48 }, { 110, -83 }, { 0, 0 }, { 31, 10 } },
    { { 0, 0 }, { 0, 0 }, { 59, -39 }, { 32, -24 }, { 0, 0 }, { 116, -48 }, { 0, 0 }, { 1, 7 } },
    { { 0, 0 }, { 0, 0 }, { 98, -37 }, { 0, 0 }, { 0, 0 }, { 104, -24 }, { 68, 71 }, { -52, -24 } },
    { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 103, -23 }, { 68, -24 }, { 101, -5 }, { 41, 86 }, { 0, 0 } }
};

static u32 D_800AE8A0[327] = {
    0x00000000, 0x1F0F0000, 0x1F0F0010, 0x1F0F0020,
    0x1F0F0030, 0x1F0F0040, 0x1F0F0050, 0x1F0F0060,
    0x2F1F7050, 0x1F1F2020, 0x1F1F2040, 0x3F1F4000,
    0x2F0F4020, 0x5F4F8000, 0x2F1F4050, 0x1F1F2060,
    0x5F0F4070, 0x3F1FA050, 0x00018E07, 0x0004E20A,
    0x0005E90C, 0x000EBEF8, 0x0001BDDF, 0x0001ADCA,
    0x0001DDC1, 0x00016ACA, 0x00015CDD, 0x00015C00,
    0x00013606, 0x00014324, 0x000D4703, 0x00010F24,
    0x00010E07, 0x0001BC37, 0x000120DE, 0x00011AE8,
    0x000122EF, 0x000137E8, 0x000B20E2, 0x0001F9DD,
    0x0009DFDB, 0x000944DB, 0x000FBB25, 0x0010C715,
    0x0011001F, 0x0601203C, 0x000A1339, 0x03022B0D,
    0x000029C4, 0x03024EBD, 0x000844DB, 0x0007E0DF,
    0x00000000, 0x1F3F8000, 0x3F1F4030, 0x1F1F4050,
    0x3F1F4000, 0x1F0F0000, 0x1F0F0010, 0x1F0F0020,
    0x1F0F0040, 0x0001962C, 0x00065BF0, 0x0002CCE4,
    0x000369E4, 0x0002F232, 0x00024131, 0x00038820,
    0x0001962C, 0x00065BF0, 0x0002CAE4, 0x000369E4,
    0x00038820, 0x0005D124, 0x00052224, 0x00051C06,
    0x00023FF9, 0x0002F245, 0x0002EFCE, 0x0002A13C,
    0x00050442, 0x000088B0, 0x00023EE8, 0x00059710,
    0x0002AC2C, 0x000509F1, 0x00050FEA, 0x0002EDC0,
    0x0005F300, 0x07064DE1, 0x0008EAC8, 0x00000000,
    0x1F0F0000, 0x1F0F0010, 0x1F0F0020, 0x1F0F0030,
    0x1F0F0040, 0x1F0F0050, 0x1F0F0060, 0x1F1F2000,
    0x1F1F2020, 0x1F0F2050, 0x1F0F2040, 0x1F0F2060,
    0x1F1F4000, 0x04025224, 0x04003824, 0x0300580C,
    0x0300380C, 0x040B30F4, 0x030144F6, 0x03003817,
    0x0301200C, 0x040B3024, 0x040BC024, 0x0001B826,
    0x0402BE2C, 0x0402F22C, 0x0402FE2C, 0x0402322C,
    0x0300F839, 0x0300D00C, 0x0001B817, 0x0300B80C,
    0x0001B8F9, 0x0400C0F4, 0x0001980C, 0x0002BDEC,
    0x0002F1EC, 0x0002FEEC, 0x000231EC, 0x040044EC,
    0x040054EC, 0x00073CFE, 0x00061C1E, 0x000230F4,
    0x0002C024, 0x0001580C, 0x0300380C, 0x040B30F4,
    0x030044F6, 0x03003816, 0x030A200C, 0x04003024,
    0x040BC024, 0x0300B826, 0x0400BE2C, 0x0400F22C,
    0x0402FE2C, 0x0400322C, 0x0300F839, 0x0300D00C,
    0x0300B817, 0x0300B80C, 0x0300B8F9, 0x040BC0F4,
    0x0400BDEC, 0x0002F1EC, 0x0400FEEC, 0x040031EC,
    0x040044EC, 0x040054EC, 0x0300F8DF, 0x0001F8F0,
    0x0001F828, 0x030A3828, 0x0007F80C, 0x00059C0C,
    0x040B4EF4, 0x000C3928, 0x040B30F4, 0x030038F7,
    0x03003817, 0x04023024, 0x0402C024, 0x0301B827,
    0x0402BE2C, 0x0402F22C, 0x0402FE2C, 0x0402322C,
    0x0300F83A, 0x030AB821, 0x0300B800, 0x0300B8FA,
    0x0400C0F4, 0x0400BEEC, 0x0400F2EC, 0x0400FEEC,
    0x040032EC, 0x0300F8DF, 0x040044EC, 0x040054EC,
    0x04009E24, 0x0009AF08, 0x000854F8, 0x0007441E,
    0x040030F4, 0x000138F7, 0x03003817, 0x040B3024,
    0x040BC024, 0x0300B828, 0x040BBE2C, 0x040BF22C,
    0x0400FE2C, 0x0400322C, 0x0300F83A, 0x030AB821,
    0x0001B801, 0x0001B8FB, 0x0400C0F4, 0x0400BEEC,
    0x0400F2EC, 0x0400FEEC, 0x040032EC, 0x0300F8DF,
    0x040044EC, 0x040054EC, 0x0001F8F0, 0x000138F0,
    0x0001F828, 0x04009E24, 0x0402A0F3, 0x000DA922,
    0x00000000, 0x0F070040, 0x0F070038, 0x0F070010,
    0x0F070018, 0x2F1F1000, 0x0301240C, 0x0402C32E,
    0x0402B72E, 0x0301CB2F, 0x04022336, 0x04022322,
    0x040224FE, 0x04022DE6, 0x000509DC, 0x0005E528,
    0x00000000, 0x1F0F0000, 0x1F0F0010, 0x1F0F0020,
    0x1F0F0030, 0x1F0F0040, 0x1F0F0050, 0x1F0F0060,
    0x1F0F2050, 0x1F0F2040, 0x1F1F2000, 0x000110E1,
    0x0400F3E8, 0x0300C8F7, 0x0300E1EB, 0x0400FF00,
    0x03001014, 0x00012812, 0x0001281E, 0x0300342D,
    0x04021C24, 0x0402FE24, 0x0402E524, 0x0409C324,
    0x0002C324, 0x00021D0C, 0x0402A018, 0x000110E1,
    0x0400F3E8, 0x0001E1EB, 0x0409FF00, 0x03011014,
    0x03002812, 0x0300281E, 0x00021C24, 0x0400FE24,
    0x0400E524, 0x0400C324, 0x04001D0C, 0x0002A018,
    0x000128FC, 0x00012804, 0x0400CD24, 0x0002B018,
    0x0002DB18, 0x0400E624, 0x0002FE24, 0x0400CD24,
    0x0400B018, 0x0409DB18, 0x0400E624, 0x0002FE24,
    0x0001BC1E, 0x0308BC1E, 0x0400C324, 0x0002E624,
    0x04000D24, 0x03002818, 0x04004024, 0x03081015,
    0x04091D0C, 0x00012804, 0x000128FC, 0x04001DF4,
    0x040008F4, 0x0002EFF4, 0x0002DCF4, 0x0400C0F4,
    0x0400FF00, 0x0001BC1E, 0x0308BC1E, 0x0002C324,
    0x0400DC24, 0x00020C24, 0x03002818, 0x00024024,
    0x03001015, 0x04001C0C, 0x030028FC, 0x00012803,
    0x04001DF4, 0x040009F4, 0x0400F1F4, 0x0002DCF4,
    0x0400C0F4, 0x04000000, 0x000AE5E0
};

// Used by `func_80068E0C`. Contains pointers to the table above.
static s_800AEDBC D_800AEDBC[] = {
    {},
    { &D_800AE8A0[0],   &D_800AE8A0[18],  0x0363, 0x03A8 },
    { &D_800AE8A0[52],  &D_800AE8A0[61],  0x03AF, 0x03BE },
    { &D_800AE8A0[52],  &D_800AE8A0[68],  0x03D1, 0x03F0 },
    { &D_800AE8A0[52],  &D_800AE8A0[83],  0x03F3, 0x0404 },
    { &D_800AE8A0[91],  &D_800AE8A0[105], 0x0415, 0x041A },
    { &D_800AE8A0[91],  &D_800AE8A0[107], 0x0429, 0x0466 },
    { &D_800AE8A0[91],  &D_800AE8A0[170], 0x04C5, 0x04FA },
    { &D_800AE8A0[91],  &D_800AE8A0[222], 0x0549, 0x054E },
    { &D_800AE8A0[91],  &D_800AE8A0[105], 0x041F, 0x0424 },
    { &D_800AE8A0[91],  &D_800AE8A0[137], 0x0477, 0x04BA },
    { &D_800AE8A0[91],  &D_800AE8A0[196], 0x0507, 0x053C },
    { &D_800AE8A0[91],  &D_800AE8A0[222], 0x0551, 0x0556 },
    { &D_800AE8A0[224], &D_800AE8A0[230], 0x0559, 0x0562 },
    { &D_800AE8A0[224], &D_800AE8A0[234], 0x056D, 0x0578 },
    { &D_800AE8A0[224], &D_800AE8A0[239], 0x0585, 0x0588 },
    { &D_800AE8A0[240], &D_800AE8A0[282], 0x05E7, 0x05F2 },
    { &D_800AE8A0[240], &D_800AE8A0[251], 0x058F, 0x05B0 },
    { &D_800AE8A0[240], &D_800AE8A0[292], 0x060F, 0x0612 },
    { &D_800AE8A0[240], &D_800AE8A0[309], 0x0615, 0x0618 },
    { &D_800AE8A0[240], &D_800AE8A0[287], 0x05FB, 0x0606 },
    { &D_800AE8A0[240], &D_800AE8A0[267], 0x05BB, 0x05DA },
    { &D_800AE8A0[240], &D_800AE8A0[293], 0x061B, 0x063C },
    { &D_800AE8A0[240], &D_800AE8A0[310], 0x0649, 0x066C }
};

void GameState_PaperMapScreen_Update(void) // 0x80066EB0
{
    s32    temp_s0_2;
    q19_12 scale;
    s8     markingFileIdx;
    u16    scrollY;
    u16    scrollX;
    q20_12 scrollPosX;
    q20_12 scrollPosY;
    u32    temp3;
    u32    temp4;

    static u8     paperMapIdx;
    static u8     markingIdx;
    static s8     activeMarkingFileIdx;
    static s8     __pad_bss_800C444B;
    static q19_12 screenPosX;
    static q19_12 screenPosY;

    Bgm_MenuUpdate();
    Game_TimerUpdate();

    switch (g_GameWork.gameStateSteps[0])
    {
        case 0:
            Screen_Refresh(SCREEN_WIDTH, true);

            activeMarkingFileIdx = g_PaperMapMarkingFileIdxs[g_SavegamePtr->paperMapIdx];
            paperMapIdx = g_SavegamePtr->paperMapIdx;
            screenPosX = NO_VALUE;
            D_800C4454 = Q12(1.0f);
            D_800AE770 = false;

            Game_RadioSoundStop();
            SD_Call(Sfx_MenuMap);
            func_80066E40();
            Fs_QueueStartReadTim(FILE_TIM_MP_0TOWN_TIM + g_PaperMapFileIdxs[paperMapIdx], FS_BUFFER_2, &g_PaperMapImg);
            Fs_QueueWaitForEmpty();

            g_IntervalVBlanks = 1;
            Screen_FadeStart(true, true, false);

            Game_StateStepSet(0, 2);
            break;

        case 2:
            // Compute paper map XY scroll values.
            if (screenPosX < Q12(0.0f))
            {
                scrollY = 0;
                scrollX = 0;
            }
            else
            {
                scrollPosX = screenPosX - Q12_MULT_PRECISE(D_800C4454, screenPosX);
                scrollX    = FP_FROM(scrollPosX, Q12_SHIFT);

                scrollPosY = screenPosY - Q12_MULT_PRECISE(D_800C4454, screenPosY);
                scrollY    = FP_FROM(scrollPosY, Q12_SHIFT);
            }

            // Compute paper map scale.
            scale = (D_800C4454 >> 1) + Q12(0.5f);

            temp_s0_2 = func_80067914(paperMapIdx, scrollX, scrollY, scale);
            func_80068E0C(1, paperMapIdx, 0, 0, scrollX, scrollY, scale);

            if (Screen_IsFadeNone())
            {
                if (!D_800AE770 && D_800C4454 == Q12(1.0f))
                {
                    func_80068CC0(paperMapIdx);
                }
            }

            PaperMap_DrawScaled(scrollX, scrollY, scale);

            if ((g_GameWork.gameStatePrev == GameState_InventoryScreen && Pad_ButtonClickedBind(cancel)) ||
                (g_GameWork.gameStatePrev != GameState_InventoryScreen &&
                 Pad_ButtonClicked(Pad_BindingGet(cancel) | Pad_BindingGet(map))))
            {
                SD_Call(Sfx_MenuMap);

                if (g_GameWork.gameStatePrev == GameState_InventoryScreen)
                {
                    GsDrawOt(&g_OrderingTable0[g_ActiveBufferIdx]);
                    VSync(SyncMode_Wait);
                    GsDrawOt(&g_OrderingTable0[g_ActiveBufferIdx]);

                    func_80066E7C();

#if VERSION_REGION_IS(PAL)
                    Fs_QueueStartReadTim(FILE_ITEM_HEROPIC2_TIM, FS_BUFFER_1, &g_HealthPortraitImg);
#endif

                    GameFs_MapItemsTextureLoad(g_SavegamePtr->mapIdx);
                    func_80066D90();
                    Screen_FadeTimestepReset();
                }
                else
                {
                    Screen_FadeStart(true, false, false);
                }

                Game_StateStepSet(0, 4);
                break;
            }

            if (Pad_ButtonClickedBind(enter))
            {
                if (D_800AE770)
                {
                    D_800AE770 = false;
                }
                else
                {
                    D_800AE770 = true;

                    if (screenPosX < Q12(0.0f))
                    {
                        screenPosX = Q12(CLAMP_LOW_THEN_MIN((s16)temp_s0_2    + 80, 0, SCREEN_WIDTH  / 2));
                        screenPosY = Q12(CLAMP_LOW_THEN_MIN((temp_s0_2 >> 16) + 60, 0, SCREEN_HEIGHT / 2));
                    }
                }
            }

            if (!D_800AE770)
            {
                if (D_800C4454 == Q12(1.0f))
                {
                    if (Pad_ButtonClicked(ControllerFlag_LStickHighUp))
                    {
                        if (HAS_PAPER_MAP(D_800AE740[paperMapIdx][0]))
                        {
                            markingIdx = D_800AE740[paperMapIdx][0];

                            Fs_QueueStartSeek(FILE_TIM_MP_0TOWN_TIM + g_PaperMapFileIdxs[markingIdx]);
                            Screen_FadeStart(true, false, false);

                            screenPosX                   = NO_VALUE;
                            
                            Game_StateStepSet(0, 3);
                            break;
                        }
                    }

                    if (Pad_ButtonClicked(ControllerFlag_LStickHighDown))
                    {
                        if (HAS_PAPER_MAP(D_800AE740[paperMapIdx][1]))
                        {
                            markingIdx = D_800AE740[paperMapIdx][1];

                            Fs_QueueStartSeek(FILE_TIM_MP_0TOWN_TIM + g_PaperMapFileIdxs[markingIdx]);
                            Screen_FadeStart(true, false, false);

                            screenPosX                   = NO_VALUE;

                            Game_StateStepSet(0, 3);
                            break;
                        }
                    }
                    break;
                }

                D_800C4454 = MIN(D_800C4454 + 196, Q12(1.0f));
                break;
            }

            D_800C4454  = CLAMP_LOW(D_800C4454 - 196, 0);
            screenPosX += (g_Controller0->normalizedSticks.sticks.leftX * ((SHRT_MAX / 2) + 1)) / 75;
            screenPosX  = CLAMP_RANGE(screenPosX, Q12(0.0f), Q12(SCREEN_WIDTH / 2));
            screenPosY += (g_Controller0->normalizedSticks.sticks.leftY * ((SHRT_MAX / 2) + 1)) / 75;
            screenPosY  = CLAMP_RANGE(screenPosY, Q12(0.0f), Q12(SCREEN_HEIGHT / 2));
            break;

        case 3:
            if (Screen_IsFadeFinished())
            {
                paperMapIdx = markingIdx;

                SD_Call(Sfx_MenuMap);

                Game_StateStepSet(0, 1);
                break;
            }

            func_80067914(paperMapIdx, 0, 0, Q12(1.0f));
            func_80068E0C(1, paperMapIdx, 0, 0, 0, 0, Q12(1.0f));
            PaperMap_DrawScaled(0, 0, Q12(1.0f));
            break;

        case 1:
            Fs_QueueStartReadTim(FILE_TIM_MP_0TOWN_TIM + g_PaperMapFileIdxs[paperMapIdx], FS_BUFFER_2, &g_PaperMapImg);

            markingFileIdx = g_PaperMapMarkingFileIdxs[paperMapIdx];
            if (markingFileIdx != activeMarkingFileIdx && markingFileIdx != NO_VALUE)
            {
                activeMarkingFileIdx = g_PaperMapMarkingFileIdxs[paperMapIdx];
                Fs_QueueStartReadTim(FILE_TIM_MR_0TOWN_TIM + g_PaperMapMarkingFileIdxs[paperMapIdx], FS_BUFFER_1, &g_PaperMapMarkingAtlasImg);
            }

            Fs_QueueWaitForEmpty();
            Screen_FadeStart(true, true, false);

            Game_StateStepSet(0, 2);
            break;

        case 4:
            temp3   = screenPosX - Q12_MULT_PRECISE(D_800C4454, screenPosX);
            scrollX = FP_FROM(temp3, Q12_SHIFT);
            temp4   = screenPosY - Q12_MULT_PRECISE(D_800C4454, screenPosY);
            scale   = (D_800C4454 >> 1) + Q12(0.5f);
            scrollY = FP_FROM(temp4, Q12_SHIFT);

            func_80067914(paperMapIdx, scrollX, scrollY, scale);
            func_80068E0C(1, paperMapIdx, 0, 0, scrollX, scrollY, scale);
            PaperMap_DrawScaled(scrollX, scrollY, scale);

            if (Screen_IsFadeFinished())
            {
                if (g_GameWork.gameStatePrev == GameState_InGame || g_GameWork.gameStatePrev == GameState_LoadMapScreen)
                {
                    func_80066E7C();
                    Screen_Init(SCREEN_WIDTH, false);
                    g_GameWork.gameStatePrev = GameState_InGame;
                }

                Game_StateSetPrevious();
            }
            break;
    }
}

s32 func_80067914(s32 paperMapIdx, u16 arg1, u16 arg2, q4_12 scale) // 0x80067914
{
    // Switch key for a rounded-down chunk `cellX`/`cellZ`: converted to `MAP_CHUNK_IDX` numbering, offset by 20
    // to keep it positive (similar to `MAP_CHUNK_CELL`?), then packed as `X * 100 + Z`.
    #define MAP_OFFSET(coord) \
        (MAP_CHUNK_IDX_FROM_GRID(coord) + 20)

    // Rounded-down chunk index of `pos`, except positions exactly on a chunk edge at or below 0 round one lower.
    #define CELL_GET(pos) \
        (((pos) <= Q12(0.0f)) ? (((pos) - Q12(CHUNK_SIZE)) / Q12(CHUNK_SIZE)) : ((pos) / Q12(CHUNK_SIZE)))

    #define MAP_IDX(x, z) \
        ((MAP_OFFSET(x) * 100) + MAP_OFFSET(z))

    s32      sp10[6];
    s16      temp_s1;
    s16      temp_s2;
    q3_12    angle1;
    s32      temp_s4;
    s32      cellX;
    s32      cellZ;
    q3_12    angle;
    u32      temp_a0;
    s16      mapCoordIdxX;
    s16      mapCoordIdxZ;
    s32      temp;
    s32      temp2;
    s32      temp3;
    s32      temp4;
    LINE_F4* line;
    POLY_G3* poly;

    // Check if paper map is valid.
    if (g_SavegamePtr->paperMapIdx != paperMapIdx)
    {
        return 0;
    }

    // Compute player cell.
    cellX = CELL_GET(PLAYER_CHARA().position.vx);
    cellZ = CELL_GET(PLAYER_CHARA().position.vz);

    mapCoordIdxX = SHRT_MAX;
    angle        = PLAYER_CHARA().rotation.vy;
    mapCoordIdxZ = SHRT_MAX;

    switch (paperMapIdx)
    {
        case PaperMapIdx_OldTown:
            switch (g_SavegamePtr->mapIdx)
            {
                case MapIdx_MAP2_S00:
                    if (cellZ < 4 || (cellZ < 6 && (cellX >= -1 && cellX < 1)))
                    {
                        mapCoordIdxX = (PLAYER_CHARA().position.vx / Q12(2.0f)) + 19;
                        mapCoordIdxZ = (-PLAYER_CHARA().position.vz / Q12(2.0f)) + 1;
                        break;
                    }

                    switch (MAP_IDX(cellX, cellZ))
                    {
                        case MAP_IDX(-5, 9):
                            mapCoordIdxX = 43;
                            mapCoordIdxZ = -34;
                            break;

                        case MAP_IDX(-5, 7):
                        case MAP_IDX(-3, 5):
                        case MAP_IDX(-3, 6):
                            mapCoordIdxX = -11;
                            mapCoordIdxZ = -19;
                            break;

                        case MAP_IDX(1, 9):
                            mapCoordIdxX = -63;
                            mapCoordIdxZ = 73;
                            break;

                        case MAP_IDX(3, 9):
                            mapCoordIdxX = -63;
                            mapCoordIdxZ = -28;
                            break;

                        case MAP_IDX(2, 7):
                        case MAP_IDX(3, 7):
                            mapCoordIdxX = -58;
                            mapCoordIdxZ = 73;
                            break;

                        case MAP_IDX(2, 5):
                        case MAP_IDX(3, 5):
                            mapCoordIdxX = -58;
                            mapCoordIdxZ = -28;
                            break;

                        case MAP_IDX(5, 5):
                            mapCoordIdxX = -112;
                            mapCoordIdxZ = 51;
                            break;

                        case MAP_IDX(-2, 8):
                        case MAP_IDX(5, 8):
                            mapCoordIdxX = 116;
                            mapCoordIdxZ = 47;
                            break;
                    }
                    break;

                case MapIdx_MAP0_S02:
                    switch (MAP_IDX(cellX, cellZ))
                    {
                        case MAP_IDX(-3, 0):
                            mapCoordIdxX = -103;
                            mapCoordIdxZ = 103;
                            break;

                        case MAP_IDX(-4, 0):
                            mapCoordIdxX = 36;
                            mapCoordIdxZ = -76;
                            angle       += Q12_ANGLE(90.0f);
                            break;

                        case MAP_IDX(-5, 0):
                            mapCoordIdxX = 62;
                            mapCoordIdxZ = 50;
                            angle       += Q12_ANGLE(180.0f);
                            break;

#if VERSION_EQUAL_OR_NEWER(JAP1) // @bugfix
                        case MAP_IDX(-4, 1):
                            mapCoordIdxX = -58;
                            mapCoordIdxZ = -28;
                            break;
#endif
                    }
                    break;

                case MapIdx_MAP2_S01:
                    mapCoordIdxX = 38;
                    mapCoordIdxZ = 50;
                    angle       += Q12_ANGLE(180.0f);
                    break;

                case MapIdx_MAP0_S01:
                    mapCoordIdxX = 33;
                    mapCoordIdxZ = -28;
                    break;

                case MapIdx_MAP1_S06:
                    mapCoordIdxX = -123;
                    mapCoordIdxZ = 81;
                    angle       -= Q12_ANGLE(90.0f);
                    break;
            }
            break;

        case PaperMapIdx_FogSchoolBF:
        case PaperMapIdx_FogSchool1F:
        case PaperMapIdx_FogSchool2F:
        case PaperMapIdx_FogSchoolRF:
        case PaperMapIdx_AltSchoolBF:
        case PaperMapIdx_AltSchool1F:
        case PaperMapIdx_AltSchool2F:
        case PaperMapIdx_AltSchoolRF:
            if (cellX == -2 && cellZ == 0)
            {
                angle       += Q12_ANGLE(180.0f);
                mapCoordIdxX = D_800AE774[2][2][0] + (((PLAYER_CHARA().position.vx - (cellX * Q12(CHUNK_SIZE))) - (Q12(CHUNK_SIZE) / 2)) / Q12(-0.2f));
                mapCoordIdxZ = D_800AE774[2][2][1] + (((PLAYER_CHARA().position.vz - (cellZ * Q12(CHUNK_SIZE))) - (Q12(CHUNK_SIZE) / 2)) / Q12(0.2f));
            }
            else
            {
                mapCoordIdxX = D_800AE774[cellX + 4][cellZ + 2][0] + (((PLAYER_CHARA().position.vx - (cellX * Q12(CHUNK_SIZE))) - (Q12(CHUNK_SIZE) / 2)) / Q12(0.2f));
                mapCoordIdxZ = D_800AE774[cellX + 4][cellZ + 2][1] + (((PLAYER_CHARA().position.vz - (cellZ * Q12(CHUNK_SIZE))) - (Q12(CHUNK_SIZE) / 2)) / Q12(-0.2f));
            }
            break;

        case PaperMapIdx_FogHospitalBF:
        case PaperMapIdx_FogHospital1F:
        case PaperMapIdx_FogHospital2F:
        case PaperMapIdx_FogHospital3F:
            mapCoordIdxX = D_800AE7E4[cellX + 1][cellZ + 2][0] + (((PLAYER_CHARA().position.vx - (cellX * Q12(CHUNK_SIZE))) - (Q12(CHUNK_SIZE) / 2)) / 0x222); // TODO: Demagic into clean float.
            mapCoordIdxZ = D_800AE7E4[cellX + 1][cellZ + 2][1] + ((((PLAYER_CHARA().position.vz - (cellZ * Q12(CHUNK_SIZE))) - (Q12(CHUNK_SIZE) / 2)) * 6) / Q12(-0.8f));
            break;

        case PaperMapIdx_AltHospitalBF:
        case PaperMapIdx_AltHospital1F:
        case PaperMapIdx_AltHospital2F:
        case PaperMapIdx_AltHospital3F:
            mapCoordIdxX = D_800AE820[cellX + 4][cellZ + 4][0] + (((PLAYER_CHARA().position.vx -  (cellX * Q12(CHUNK_SIZE))) - (Q12(CHUNK_SIZE) / 2)) / 0x222);
            mapCoordIdxZ = D_800AE820[cellX + 4][cellZ + 4][1] + ((((PLAYER_CHARA().position.vz - (cellZ * Q12(CHUNK_SIZE))) - (Q12(CHUNK_SIZE) / 2)) * 6) / Q12(-0.8f));
            break;

        case PaperMapIdx_ResortTown:
            switch (g_SavegamePtr->mapIdx)
            {
                case MapIdx_MAP5_S01:
                    mapCoordIdxX = (PLAYER_CHARA().position.vx / Q12(2.0f)) + 60;
                    mapCoordIdxZ = (-PLAYER_CHARA().position.vz / Q12(2.0f)) - 85;

                    if (PLAYER_IN_MAP_CHUNK(-4, -1) || PLAYER_IN_MAP_CHUNK(-5, -1))
                    {
                        mapCoordIdxX += 87;
                        mapCoordIdxZ += 61;
                    }
                    break;

                case MapIdx_MAP5_S02:
                    if (PLAYER_IN_MAP_CHUNK(3, 1))
                    {
                        mapCoordIdxX = 87;
                        mapCoordIdxZ = -56;
                        angle       += Q12_ANGLE(90.0f);
                    }
                    else if (PLAYER_IN_MAP_CHUNK(3, 0))
                    {
                        mapCoordIdxX = 54;
                        mapCoordIdxZ = -53;
                        angle       += Q12_ANGLE(90.0f);
                    }
                    break;

                case MapIdx_MAP5_S03:
                    switch (MAP_IDX(cellX, cellZ))
                    {
                        case MAP_IDX(2, 1):
                            mapCoordIdxX = 69;
                            mapCoordIdxZ = -19;
                            break;

                        case MAP_IDX(2, 0):
                            mapCoordIdxX = 74;
                            mapCoordIdxZ = -12;
                            angle       -= Q12_ANGLE(90.0f);
                            break;

                        case MAP_IDX(1, 0):
                            mapCoordIdxX = 74;
                            mapCoordIdxZ = -14;
                            break;

                        case MAP_IDX(1, 1):
                            mapCoordIdxX = 73;
                            mapCoordIdxZ = -14;
                            break;
                    }
                    break;

                case MapIdx_MAP6_S00:
                    mapCoordIdxX = (PLAYER_CHARA().position.vx / Q12(2.0f)) + 0;
                    mapCoordIdxZ = (-PLAYER_CHARA().position.vz / Q12(2.0f)) + 55;
                    break;

                case MapIdx_MAP6_S01:
                    if (PLAYER_IN_MAP_CHUNK(-2, 1))
                    {
                        mapCoordIdxZ = 43;
                    }
                    else
                    {
                        mapCoordIdxZ = 46;
                    }

                    mapCoordIdxX = -77;
                    break;

                case MapIdx_MAP6_S02:
                    mapCoordIdxX = -55;
                    mapCoordIdxZ =  84;

                    if (!PLAYER_IN_MAP_CHUNK_X(1))
                    {
                        angle += Q12_ANGLE(90.0f);
                    }
                    else if (!PLAYER_IN_MAP_CHUNK_Z(-1))
                    {
                        angle += Q12_ANGLE(90.0f);
                    }
                    break;
            }
            break;

        case PaperMapIdx_FogCentralTown:
        case PaperMapIdx_AltCentralTown:
            switch (g_SavegamePtr->mapIdx)
            {
                case MapIdx_MAP2_S02:
                case MapIdx_MAP4_S02:
                case MapIdx_MAP4_S05:
                    switch (MAP_IDX(cellX, cellZ))
                    {
                        case MAP_IDX(-6, 2):
                            mapCoordIdxX =  94;
                            mapCoordIdxZ = -103;
                            break;

                        case MAP_IDX(-3, 2):
                        case MAP_IDX(-4, 2):
                            mapCoordIdxX = -12;
                            mapCoordIdxZ =  63;
                            angle       += Q12_ANGLE(180.0f);
                            break;

                        case MAP_IDX(3, -1):
                            mapCoordIdxX = -16;
                            mapCoordIdxZ =  91;
                            break;

                        case MAP_IDX(-3, 4):
                        case MAP_IDX(-3, 5):
                            mapCoordIdxX = (PLAYER_CHARA().position.vx / Q12(1.0f)) + 80;
                            mapCoordIdxZ = ((Q12(280.0f) - PLAYER_CHARA().position.vz) / Q12(1.0f)) + 0;
                            break;

                        default:
                            mapCoordIdxX = (PLAYER_CHARA().position.vx / Q12(1.0f)) + 80;
                            mapCoordIdxZ = (-PLAYER_CHARA().position.vz / Q12(1.0f)) + 0;
                            break;
                    }
                    break;

                case MapIdx_MAP4_S01:
                    mapCoordIdxX =  95;
                    mapCoordIdxZ = -103;
                    break;

                case MapIdx_MAP4_S03:
                    switch (MAP_IDX(cellX, cellZ))
                    {
                        case MAP_IDX(3, 1):
                        case MAP_IDX(4, 1):
                            mapCoordIdxX =  100;
                            mapCoordIdxZ = -61;
                            break;

                        case MAP_IDX(2, -3):
                        case MAP_IDX(3, -3):
                            mapCoordIdxX =  115;
                            mapCoordIdxZ = -61;
                            break;

                        case MAP_IDX(5, -1):
                            mapCoordIdxX =  115;
                            mapCoordIdxZ = -63;
                            angle       += Q12_ANGLE(90.0f);
                            break;

                        case MAP_IDX(2, 3):
                        case MAP_IDX(3, 3):
                            mapCoordIdxX =  125;
                            mapCoordIdxZ = -60;
                            break;
                    }
                    break;

                case MapIdx_MAP2_S04:
                    switch (MAP_IDX(cellX, cellZ))
                    {
                        case MAP_IDX(-3, 1):
                        case MAP_IDX(-1, 0):
                            mapCoordIdxX = -19;
                            mapCoordIdxZ = -14;
                            break;

                        case MAP_IDX(-2, 0):
                            mapCoordIdxX = -21;
                            mapCoordIdxZ = -14;
                            break;
                    }
                    break;
            }
            break;

        case PaperMapIdx_FogSewer1F:
            mapCoordIdxX = (-PLAYER_CHARA().position.vz / Q12(0.5f)) + 0;
            mapCoordIdxZ = (-PLAYER_CHARA().position.vx / Q12(0.5f)) + 40;
            angle       -= Q12_ANGLE(90.0f);
            break;

        case PaperMapIdx_FogSewer2F:
            mapCoordIdxX = (-PLAYER_CHARA().position.vz / Q12(0.5f)) - 80;
            mapCoordIdxZ = (-PLAYER_CHARA().position.vx / Q12(0.5f)) - 140;
            angle       -= Q12_ANGLE(90.0f);
            break;

        case PaperMapIdx_AltSewer:
            mapCoordIdxX = (-PLAYER_CHARA().position.vx / Q12(0.5f)) + 55;
            mapCoordIdxZ = (PLAYER_CHARA().position.vz / Q12(0.5f)) - 80;
            angle       += Q12_ANGLE(180.0f);
            break;

        default:
            return 0;
    }

    if (mapCoordIdxX == SHRT_MAX)
    {
        return 0;
    }

    temp_s4 = (mapCoordIdxZ << 16) + mapCoordIdxX;
    if (Pad_ButtonHeld(ControllerFlag_L1 | ControllerFlag_R1))
    {
        return temp_s4;
    }

    temp_a0 = SCREEN_WIDTH / 2;
    temp3   = ((mapCoordIdxX - (arg1 - temp_a0)) * SCREEN_WIDTH);
    temp    = Q12_MULT_PRECISE(scale, SCREEN_WIDTH);
    temp_s2 = (temp3 / temp) - (SCREEN_WIDTH / 2);

    temp_a0 = SCREEN_HEIGHT / 2;
    temp4   = ((mapCoordIdxZ - (arg2 - temp_a0)) * SCREEN_HEIGHT);
    temp2   = Q12_MULT_PRECISE(scale, SCREEN_HEIGHT);
    temp_s1 = (temp4 / temp2) - (SCREEN_HEIGHT / 2);

    angle1 = Math_AngleNormalizeSigned(angle);

    sp10[0] = temp_s2 + FP_FROM(Math_Sin(angle1) * 6, Q12_SHIFT);
    sp10[1] = (temp_s1 + FP_FROM(Math_Cos(angle1) * -6, Q12_SHIFT)) * 2;
    sp10[2] = temp_s2 + FP_FROM(Math_Cos(angle1) * 4 - Math_Sin(angle1) * 6, Q12_SHIFT);
    sp10[3] = (temp_s1 + FP_FROM(Math_Sin(angle1) * 4 + Math_Cos(angle1) * 6, Q12_SHIFT)) * 2;
    sp10[4] = temp_s2 + FP_FROM(Math_Cos(angle1) * -4 - Math_Sin(angle1) * 6, Q12_SHIFT);
    sp10[5] = (temp_s1 + FP_FROM(Math_Sin(angle1) * -4 + Math_Cos(angle1) * 6, Q12_SHIFT)) * 2;

    line = (LINE_F4*)GsOUT_PACKET_P;
    setLineF4(line);
    setXY0Fast(line, sp10[0], sp10[1]);
    setXY1Fast(line, sp10[2], sp10[3]);
    setXY2Fast(line, sp10[4], sp10[5]);
    setXY3Fast(line, sp10[0], sp10[1]);
    setRGB0Fast(line, 0x10, 0x10, 0x10);

    addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[2], line);

    poly = (POLY_G3*)(++line);
    setPolyG3(poly);
    setXY0Fast(poly, sp10[0], sp10[1]);
    setXY1Fast(poly, sp10[2], sp10[3]);
    setXY2Fast(poly, sp10[4], sp10[5]);
    setRGB0Fast(poly, 0x00, 0x00, 0xFF);
    setRGB1Fast(poly, 0x00, 0xFF, 0x00);
    setRGB2Fast(poly, 0x00, 0xFF, 0x00);

    addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[2], poly);
    GsOUT_PACKET_P = (PACKET*)poly + sizeof(POLY_G3);
    return temp_s4;
}

bool func_80068CC0(s32 arg0) // 0x80068CC0
{
    s32      i;
    POLY_G3* poly;

    for (i = 0; i < 2; i++)
    {
        if (HAS_PAPER_MAP(D_800AE740[arg0][i]))
        {
            poly = GsOUT_PACKET_P;
            setPolyG3(poly);

            if (i != 0)
            {
                setXY0Fast(poly,  0, 216);
                setXY1Fast(poly, -8, 200);
                setXY2Fast(poly,  8, 200);
            }
            else
            {
                setXY0Fast(poly,  0, -216);
                setXY1Fast(poly, -8, -200);
                setXY2Fast(poly,  8, -200);
            }

            setRGB0Fast(poly, 0x10, 0x10, 0xC4);
            setRGB1Fast(poly, 0x80, 0x80, 0xC4);
            setRGB2Fast(poly, 0x80, 0x80, 0xC4);

            addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[2], poly);
            GsOUT_PACKET_P = ++poly;
        }
    }

    return true;
}

bool func_80068E0C(s32 arg0, s32 idx, s32 arg2, s32 shade, u16 arg4, u16 arg5, q4_12 scale) // 0x80068E0C
{
    s32              sp0;
    u16              sp4;
    s16              y0;
    s32              x0;
    s32              x1;
    s32              y1;
    s32              j;
    s32              i;
    s32              temp;
    s32              temp2;
    s16              temp3;
    s32              temp4;
    s32              temp6;
    s_func_80068E0C* ptr;

    ptr = PSX_SCRATCH;

    if (g_PaperMapMarkingFileIdxs[idx] == NO_VALUE)
    {
        return false;
    }

    if (D_800AEDBC[idx].ptr_0 == NULL)
    {
        return false;
    }

    if (D_800AEDBC[idx].ptr_4 == NULL)
    {
        return false;
    }

    ptr->field_0 = (POLY_FT4*)GsOUT_PACKET_P;
    i            = 0;

    if (arg0 == 2)
    {
        i            = (arg2 - D_800AEDBC[idx].field_8) >> 1;
        ptr->field_C = i + 1;
    }
    else
    {
        shade        = 128;
        ptr->field_C = (D_800AEDBC[idx].field_A - D_800AEDBC[idx].field_8) >> 1;
    }

    for (; i < ptr->field_C; i++)
    {
        for (j = 0; j < 2; j++)
        {
            if (arg0 == 1 && !Savegame_EventFlagGet(((D_800AEDBC[idx].field_8 + (i * 2)) + j) + 1))
            {
                continue;
            }

            if (arg0 == 2 && (arg2 & (1 << 0)) != j)
            {
                continue;
            }

            ptr->field_8 = *(s_800AE8A0_4*)&D_800AEDBC[idx].ptr_4[i];
            if (j != 0)
            {
                if (ptr->field_8.field_3)
                {
                    ptr->field_4 = *(s_800AE8A0_0*)&D_800AEDBC[idx].ptr_0[ptr->field_8.field_3];
                }
                else
                {
                    continue;
                }
            }
            else if (ptr->field_8.field_2)
            {
                ptr->field_4 = *(s_800AE8A0_0*)&D_800AEDBC[idx].ptr_0[ptr->field_8.field_2];
            }
            else
            {
                continue;
            }

            sp4   = SCREEN_WIDTH / 2;
            temp6 = (ptr->field_8.field_0 << 1) - (arg4 - sp4);
            x0    = (Q12(temp6) / scale) - (SCREEN_WIDTH / 2);

            sp4   = SCREEN_HEIGHT;
            temp4 = Q12((ptr->field_8.field_1 << 1) - ((arg5 * 2) - sp4));
            y0    = (temp4 / scale) - SCREEN_HEIGHT;

            x1    = x0 + (Q12(ptr->field_4.field_2) / scale);
            temp3 = Q12(ptr->field_4.field_3) / scale;
            y1    = y0 + temp3;

            // Set polygon vertices.
            setPolyFT4(ptr->field_0);
            setXY0Fast(ptr->field_0, x0, y0);
            setXY1Fast(ptr->field_0, x1, y0);
            setXY2Fast(ptr->field_0, x0, y1);
            setXY3Fast(ptr->field_0, x1, y1);

            // Set polygon UVs.
            temp                       = (ptr->field_4.field_1 << 8) + 0x80;
            *((s32*)&ptr->field_0->u0) = ptr->field_4.field_0 + temp + (getClut(g_PaperMapMarkingAtlasImg.clutX, g_PaperMapMarkingAtlasImg.clutY) << 16);
            temp2                      = ptr->field_4.field_2 + 0x80;
            *((s32*)&ptr->field_0->u1) = ptr->field_4.field_0 + temp2 + (ptr->field_4.field_1 << 8) + 0x470000;
            sp0                        = ((ptr->field_4.field_1 + ptr->field_4.field_3) << 8) + 0x80;
            *((s16*)&ptr->field_0->u2) = ptr->field_4.field_0 + sp0;
            *((s16*)&ptr->field_0->u3) = ptr->field_4.field_2 + (ptr->field_4.field_0 + 0x80) + ((ptr->field_4.field_1 + ptr->field_4.field_3) << 8);

            // Set polygon color.
            setRGBC0(ptr->field_0, shade, shade, shade, PRIM_POLY | RECT_BLEND | RECT_TEXTURE | RECT_SIZE_1);

            addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[3], ptr->field_0);
            ptr->field_0++;
        }
    }

    GsOUT_PACKET_P = (PACKET*)ptr->field_0;
    return true;
}

void PaperMap_DrawScaled(u16 scrollX, u16 scrollY, q4_12 scale) // 0x800692A4
{
    u16       rightEdge;
    u16       bottomEdge;
    s16       clampedLeft;
    s16       clampedWidth;
    s16       clampedTop;
    s16       clampedHeight;
    s32       tileU;
    s32       tileV;
    s16       tileU1;
    s16       tileVBottom;
    s32       tileX;
    s32       tileY;
    s16       hackVar;
    s16       hackVar2;
    s16       hackVar3;
    s16       hackVar4;
    POLY_FT4* poly;
#if VERSION_REGION_IS(PAL)
    TILE*     tile;
#endif

    // Accumulated screen-space X edges per tile column (center-relative coords).
    s16 tileScreenEdgesX[] = { -160, -160, -160, -160 };

    // Accumulated screen-space Y edges per tile row.
    s16 tileScreenEdgesY[] = { -SCREEN_HEIGHT, -SCREEN_HEIGHT, -SCREEN_HEIGHT };

    // Compute right/bottom edges of scaled 320x240 image.
    rightEdge  = (Q12_MULT_PRECISE(scale, SCREEN_WIDTH)  + scrollX) - 1;
    bottomEdge = (Q12_MULT_PRECISE(scale, SCREEN_HEIGHT) + scrollY) - 1;

    poly = GsOUT_PACKET_P;

    // Iterate 3x2 grid of 128x128 VRAM tiles covering paper map image.
    for (tileX = 0; tileX < 3; tileX++)
    {
        for (tileY = 0; tileY < 2; tileY++)
        {
            // Skip if tile column is entirely outside scaled image horizontally.
            if ((((tileX + 1) * 128) < scrollX) ||
                (tileX * 128) > rightEdge)
            {
                // @hack Needed for regalloc match later in the function.
                // TODO: Maybe params to macros below are actually meant to be locals? No luck changing them though.
                hackVar  =
                hackVar2 =
                hackVar3 =
                hackVar4 = 0;
                continue;
            }

            // Skip if tile row is entirely outside scaled image vertically.
            if ((((tileY + 1) * 128) < (scrollY + (g_PaperMapImg.v / 2))) ||
                ((tileY * 128) > (bottomEdge + (g_PaperMapImg.v / 2))))
            {
                continue;
            }

            // Clamp tile left edge to image left edge.
            clampedLeft = CLAMP_LOW(scrollX, tileX * 128);

            // Compute visible tile width (clamped to image right edge).
            clampedWidth = MIN_EXTENT_OFFSET((tileX + 1) * 128, rightEdge, clampedLeft);

            // Clamp tile top edge to image top edge.
            clampedTop = CLAMP_LOW(scrollY + (g_PaperMapImg.v / 2), tileY * 128);

            // Compute visible tile height (clamped to image bottom edge).
            clampedHeight = MIN_EXTENT_OFFSET((tileY + 1) * 128, bottomEdge + (g_PaperMapImg.v / 2), clampedTop);

            // Advance accumulated screen edges by inverse-scaled tile extent.
            tileScreenEdgesX[tileX + 1] = tileScreenEdgesX[tileX] + (Q12(clampedWidth)        / scale);
            tileScreenEdgesY[tileY + 1] = tileScreenEdgesY[tileY] + ((Q12(clampedHeight) * 2) / scale);

            setPolyFT4(poly);

            // Set quad screen positions from accumulated edge arrays.
            setXY0Fast(poly, tileScreenEdgesX[tileX],     tileScreenEdgesY[tileY]);
            setXY1Fast(poly, tileScreenEdgesX[tileX + 1], tileScreenEdgesY[tileY]);
            setXY2Fast(poly, tileScreenEdgesX[tileX],     tileScreenEdgesY[tileY + 1]);
            setXY3Fast(poly, tileScreenEdgesX[tileX + 1], tileScreenEdgesY[tileY + 1]);

            // Compute tile's UV offset into VRAM region.
            // @note These are `int`s, but `tileU1` and `tileVBottom` are `short`s.
            tileU = clampedLeft - (tileX * 128);
            tileV = clampedTop  - (tileY * 128);

            setUV0AndClutSum(poly, tileU, tileV * 2, getClut(g_PaperMapImg.clutX, g_PaperMapImg.clutY));

            // @hack Regalloc fixes. `hackVar` usages compile to nothing (GCC knows `hackVar == 0`), but the 4 vars
            // pad the stack frame to match.
            // @hack Two `tileScreenEdgesX[tileX] & hackVar` loads bump the array pointer's ref count so it wins `t8`.
            setUV1AndTPageSum(poly, tileU + clampedWidth, tileV * 2,
                              getTPage(g_PaperMapImg.tPage[0],
                                       (hackVar << 5) | hackVar2 | hackVar3 | hackVar4 | (tileScreenEdgesX[tileX] & hackVar),
                                       (g_PaperMapImg.tPage[1] + tileX + (tileY * 16)) << 6,
                                       (((g_PaperMapImg.tPage[1] + (tileY * 16)) >> 4) & 1) << 8));

            tileU1      =  clampedLeft - (tileX * 128);
            tileVBottom = ((clampedTop - (tileY * 128)) << 1) - 1;

            // @hack Similar to above, `tileScreenEdgesX[tileX] & hackVar` is needed to fix regalloc.
            setUV2Sum(poly, tileU1 + (tileScreenEdgesX[tileX] & hackVar), tileVBottom + (clampedHeight << 1));
            setUV3Sum(poly, clampedWidth + tileU1, tileVBottom + (clampedHeight << 1));

            setSemiTrans(poly, 0);
            setRGB0Fast(poly, 128, 128, 128);

            addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[4], poly);
            poly++;
        }
    }

#if VERSION_REGION_IS(PAL)
    // Black bars covering the PAL screen area above and below the 480-line paper map image.
    tile = (TILE*)poly;
    setTile(tile);
    setXY0(tile, -(SCREEN_WIDTH / 2), -256);
    setWH(tile, SCREEN_WIDTH, 16);
    setRGB0Fast(tile, 0, 0, 0);
    setSemiTrans(tile, 0);
    addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[0], tile);
    tile++;

    setTile(tile);
    setXY0(tile, -(SCREEN_WIDTH / 2), 236);
    setWH(tile, SCREEN_WIDTH, 20);
    setRGB0Fast(tile, 0, 0, 0);
    setSemiTrans(tile, 0);
    addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[0], tile);
    tile++;

    g_SysWork.bgmStatusFlags |= BgmStatusFlag_Pause;
    GsOUT_PACKET_P            = tile;
#else
    GsOUT_PACKET_P            = poly;
    g_SysWork.bgmStatusFlags |= BgmStatusFlag_Pause;
#endif
}
