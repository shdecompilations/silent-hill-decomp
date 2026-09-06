#include "game.h"

#include "bodyprog/bodyprog.h"
#include "bodyprog/screen/screen_data.h"
#include "bodyprog/text/text_draw.h"
#include "bodyprog/item_screens.h"
#include "bodyprog/math/math.h"
#include "bodyprog/ranking.h"
#include "main/fsqueue.h"

static const s32 __pad_rodata_8002B2F8 = 0;

s16  g_Ranking_SavegameCount;
u16  g_Ranking_GameplayHours;
u16  g_Ranking_WalkDistanceKm;
u16  g_Ranking_WalkDistanceM;
u16  g_Ranking_RunDistanceKm;
u16  g_Ranking_RunDistanceM;
u16  g_Ranking_PickedUpItemCount;
u8   g_Ranking_GameplayMinutes;
u8   g_Ranking_GameplaySeconds;
u8   g_Ranking_ClearGameCount;
u8   g_Ranking_ClearGameEndings;
u8   g_Ranking_CurrentEndingFlags;
u8   g_Ranking_EndLocationId;
u8   g_Ranking_PickedUpSpecialItemCount;
s8   g_Ranking_Score;
u8   g_Ranking_HasUsedGasolineOnChainsaw;
u8   g_Ranking_HasUsedGasolineOnRockDrill;
u16  g_Ranking_EnemyKillsShooting;
u16  g_Ranking_EnemyKillsMelee;
bool g_Ranking_HasFiredShot;
u32  g_Ranking_CloseRangeShotPercent;
u32  g_Ranking_MidRangeShotPercent;
u32  g_Ranking_LongRangeShotPercent;
u32  g_Ranking_NoAimingShotPercent;
s8   g_Ranking_GameDifficulty;
u8   g_Ranking_ContinueCount;

s8 __pad_bss_800C48D2[14];

s_RankingTextColors g_Ranking_TextColors;

struct
{
    char f0[5];
    char f1[5];
    char f2[5];
    char f3[5];
} g_Credits_Digits = { "0.00", "0.00", "0.00", "0.00" };

void Ranking_EvaluateScore() // 0x8008F94C
{
    s16 gameplayMinutes;
    s32 var_a0_3;
    s32 var_v1;
    u16 firedShotCount;
    u16 var_v0_5;
    u8  temp_a0_2;
    u8  temp;
    s8* temp3;

    g_Ranking_Score                      = 0;
    g_Ranking_GameDifficulty             = g_SavegamePtr->gameDifficulty;
    g_Ranking_SavegameCount              = g_SavegamePtr->savegameCount;
    g_Ranking_SavegameCount              = CLAMP(g_Ranking_SavegameCount, 0, 999);
    g_Ranking_ContinueCount              = g_SavegamePtr->continueCount;
    g_Ranking_GameplayHours              = g_SavegamePtr->add290Hours * 24 + FP_FROM(g_SavegamePtr->gameplayTimer, Q12_SHIFT) / 3600;
    g_Ranking_WalkDistanceKm             = FP_FROM(g_SavegamePtr->walkDistance, Q12_SHIFT) / 1000;
    g_Ranking_WalkDistanceM              = FP_FROM(g_SavegamePtr->walkDistance, Q12_SHIFT) - (FP_FROM(g_SavegamePtr->walkDistance, Q12_SHIFT) / 1000) * 1000;
    g_Ranking_RunDistanceKm              = FP_FROM(g_SavegamePtr->runDistance, Q12_SHIFT) / 1000;
    g_Ranking_RunDistanceM               = FP_FROM(g_SavegamePtr->runDistance, Q12_SHIFT) - (FP_FROM(g_SavegamePtr->runDistance, Q12_SHIFT) / 1000) * 1000;
    g_Ranking_PickedUpItemCount          = g_SavegamePtr->pickedUpItemCount;
    g_Ranking_GameplayMinutes            = (FP_FROM(g_SavegamePtr->gameplayTimer, Q12_SHIFT) / 60) % 60;
    g_Ranking_GameplaySeconds            = FP_FROM(g_SavegamePtr->gameplayTimer, Q12_SHIFT) - (FP_FROM(g_SavegamePtr->gameplayTimer, Q12_SHIFT) / 60) * 60;
    g_Ranking_ClearGameCount             = g_SavegamePtr->clearGameCount;
    g_Ranking_ClearGameEndings           = g_SavegamePtr->clearGameEndings;
    g_Ranking_CurrentEndingFlags         = g_SavegamePtr->currentEndingFlags;
    g_Ranking_HasUsedGasolineOnChainsaw  = Savegame_EventFlagGetAlt(EventFlag_M2S00_UsedGasolineOnChainsaw);
    g_Ranking_PickedUpSpecialItemCount   = 0;
    g_Ranking_HasUsedGasolineOnRockDrill = Savegame_EventFlagGetAlt(EventFlag_M2S00_UsedGasolineOnRockDrill);
    g_Ranking_EnemyKillsShooting         = func_8009146C(1);
    g_Ranking_EnemyKillsMelee            = func_8009146C(0);

    firedShotCount = g_SavegamePtr->firedShotCount;
    g_Ranking_HasFiredShot = firedShotCount != 0;

    if (g_Ranking_HasFiredShot)
    {
        g_Ranking_CloseRangeShotPercent = g_SavegamePtr->closeRangeShotCount;
        g_Ranking_MidRangeShotPercent   = g_SavegamePtr->midRangeShotCount;
        g_Ranking_LongRangeShotPercent  = g_SavegamePtr->longRangeShotCount;
        g_Ranking_MidRangeShotPercent   = g_Ranking_MidRangeShotPercent + g_SavegamePtr->field_26C;
        g_Ranking_CloseRangeShotPercent = g_Ranking_CloseRangeShotPercent * 100 / firedShotCount;
        g_Ranking_MidRangeShotPercent   = g_Ranking_MidRangeShotPercent * 100 / firedShotCount;
        g_Ranking_LongRangeShotPercent  = g_Ranking_LongRangeShotPercent * 100 / firedShotCount;
        g_Ranking_NoAimingShotPercent   = ((100 - g_Ranking_CloseRangeShotPercent) - g_Ranking_MidRangeShotPercent) - g_Ranking_LongRangeShotPercent;

        g_Credits_Digits.f0[0] = (((g_Ranking_CloseRangeShotPercent) / 100) % 10) + '0';
        g_Credits_Digits.f0[2] = (((g_Ranking_CloseRangeShotPercent) / 10) - (((g_Ranking_CloseRangeShotPercent) / 100) * 10)) + '0';
        g_Credits_Digits.f0[3] = ((g_Ranking_CloseRangeShotPercent) % 10) + '0';

        g_Credits_Digits.f1[0] = (((g_Ranking_MidRangeShotPercent) / 100) % 10) + '0';
        g_Credits_Digits.f1[2] = (((g_Ranking_MidRangeShotPercent) / 10) - (((g_Ranking_MidRangeShotPercent) / 100) * 10)) + '0';
        g_Credits_Digits.f1[3] = ((g_Ranking_MidRangeShotPercent) % 10) + '0';

        g_Credits_Digits.f2[0] = (((g_Ranking_LongRangeShotPercent) / 100) % 10) + '0';
        g_Credits_Digits.f2[2] = (((g_Ranking_LongRangeShotPercent) / 10) - (((g_Ranking_LongRangeShotPercent) / 100) * 10)) + '0';
        g_Credits_Digits.f2[3] = ((g_Ranking_LongRangeShotPercent) % 10) + '0';

        g_Credits_Digits.f3[0] = (((g_Ranking_NoAimingShotPercent) / 100) % 10) + '0';
        g_Credits_Digits.f3[2] = (((g_Ranking_NoAimingShotPercent) / 10) - (((g_Ranking_NoAimingShotPercent) / 100) * 10)) + '0';
        g_Credits_Digits.f3[3] = ((g_Ranking_NoAimingShotPercent) % 10) + '0';
    }

    gameplayMinutes    = g_Ranking_GameplayMinutes + (g_Ranking_GameplayHours * 60);

    // @bugfix Pre-JAP1 accidentally included unlocked Hyper Blaster colors in the picked up item count.
    // JAP1 onwards fix this by ignoring the top-most bit.
    // See `pickedUpSpecialItemCount` field comment for more info.
#if VERSION_EQUAL_OR_NEWER(JAP1)
    g_Ranking_PickedUpSpecialItemCount = g_SavegamePtr->pickedUpSpecialItemCount & 0x7;
#else
    g_Ranking_PickedUpSpecialItemCount = g_SavegamePtr->pickedUpSpecialItemCount & 0xF;
#endif

    g_Ranking_EndLocationId = g_SavegamePtr->locationId;
    D_800C3E40 = 7;
    temp_a0_2  = g_GameWork.config.extraOptionsEnabled;

    if ((temp_a0_2 & 0x3) && (temp_a0_2 & 0xC))
    {
        D_800C3E40 = 0xF;
    }

    if (g_GameWork.config.extraOptionsEnabled & 1)
    {
        D_800C3E40 |= 0x10;
    }

    if (Inventory_HyperBlasterUnlockTest() == NO_VALUE || (g_Ranking_CurrentEndingFlags & GameEndingFlag_Ufo))
    {
        D_800C3E40 |= 0x20;
    }

    g_Ranking_TextColors.ending = StringColorId_White;

    switch (g_Ranking_CurrentEndingFlags)
    {
        // @bug These cases could never be used? `currentEndingFlags` only holds the current ending that was triggered.
        // Maybe this was meant to check `clearGameEndings` instead?
        case GameEndingFlag_GoodPlus + GameEndingFlag_Good:
        case GameEndingFlag_GoodPlus + GameEndingFlag_BadPlus:
        case GameEndingFlag_Good + GameEndingFlag_BadPlus:
        case GameEndingFlag_GoodPlus + GameEndingFlag_Good + GameEndingFlag_BadPlus:
        case GameEndingFlag_GoodPlus + GameEndingFlag_Bad:
        case GameEndingFlag_Good + GameEndingFlag_Bad:
        case GameEndingFlag_GoodPlus + GameEndingFlag_Good + GameEndingFlag_Bad:
        case GameEndingFlag_BadPlus + GameEndingFlag_Bad:
        case GameEndingFlag_GoodPlus + GameEndingFlag_BadPlus + GameEndingFlag_Bad:
        case GameEndingFlag_Good + GameEndingFlag_BadPlus + GameEndingFlag_Bad:
        case GameEndingFlag_GoodPlus + GameEndingFlag_Good + GameEndingFlag_BadPlus + GameEndingFlag_Bad:
            break;

        case GameEndingFlag_GoodPlus:
            g_Ranking_TextColors.ending = StringColorId_Gold;
            g_Ranking_Score += 10;
            break;

        case GameEndingFlag_Good:
            g_Ranking_Score += 5;
            break;

        case GameEndingFlag_BadPlus:
            g_Ranking_Score += 3;
            break;

        case GameEndingFlag_Bad:
            g_Ranking_Score += 1;
            break;

        case GameEndingFlag_Ufo:
            g_Ranking_Score += 0;
            break;
    }

    g_Ranking_TextColors.pickedUpItemCount = StringColorId_White;

    if ((g_Ranking_PickedUpItemCount / 15) < 0 || (g_Ranking_PickedUpItemCount / 15) >= 10)
    {
        g_Ranking_TextColors.pickedUpItemCount = StringColorId_Gold;
        g_Ranking_Score += 10;
    }
    else
    {
        g_Ranking_Score += g_Ranking_PickedUpItemCount / 15;
    }

    g_Ranking_TextColors.savegameCount = StringColorId_White;

    if (g_Ranking_SavegameCount < 3)
    {
        g_Ranking_TextColors.savegameCount = StringColorId_Gold;
        g_Ranking_Score += 5;
    }
    else if (g_Ranking_SavegameCount < 6)
    {
        g_Ranking_Score += 4;
    }
    else if (g_Ranking_SavegameCount < 11)
    {
        g_Ranking_Score += 3;
    }
    else if (g_Ranking_SavegameCount < 21)
    {
        g_Ranking_Score += 2;
    }
    else if (g_Ranking_SavegameCount < 31)
    {
        g_Ranking_Score += 1;
    }

    g_Ranking_TextColors.continueCount = StringColorId_White;

    if (g_Ranking_ContinueCount < 2)
    {
        g_Ranking_TextColors.continueCount = StringColorId_Gold;
        g_Ranking_Score += 5;
    }
    else if (g_Ranking_ContinueCount < 4)
    {
        g_Ranking_Score += 4;
    }
    else if (g_Ranking_ContinueCount < 6)
    {
        g_Ranking_Score += 3;
    }
    else if (g_Ranking_ContinueCount < 8)
    {
        g_Ranking_Score += 2;
    }
    else if (g_Ranking_ContinueCount < 10)
    {
        g_Ranking_Score += 1;
    }

    g_Ranking_TextColors.gameplayTime = StringColorId_White;

    if (gameplayMinutes < 90)
    {
        g_Ranking_TextColors.gameplayTime = StringColorId_Gold;
        g_Ranking_Score += 10;
    }
    else if (gameplayMinutes < 180)
    {
        g_Ranking_Score += 5;
    }
    else if (gameplayMinutes < 240)
    {
        g_Ranking_Score += 3;
    }
    else if (gameplayMinutes < 360)
    {
        g_Ranking_Score += 2;
    }
    else if (gameplayMinutes < 720)
    {
        g_Ranking_Score += 1;
    }

    g_Ranking_TextColors.clearGameCount = StringColorId_White;

    temp = g_Ranking_ClearGameCount;

    if (temp >= 5)
    {
        g_Ranking_TextColors.clearGameCount = StringColorId_Gold;

        g_Ranking_Score += 10;
    }
    else
    {
        g_Ranking_Score += temp * 2;
    }

    g_Ranking_TextColors.pickedUpSpecialItemCount = StringColorId_White;

    temp = g_Ranking_PickedUpSpecialItemCount;
    if (temp >= 5)
    {
        g_Ranking_TextColors.pickedUpSpecialItemCount = StringColorId_Gold;
        g_Ranking_Score    += 10;
    }
    else
    {
        g_Ranking_Score += temp * 2;
    }

    g_Ranking_TextColors.killPercentages = StringColorId_White;

    if (g_Ranking_HasFiredShot)
    {
        s32 shootingScore;
        shootingScore = ((g_Ranking_CloseRangeShotPercent + g_Ranking_MidRangeShotPercent * 2) +
                         (g_Ranking_LongRangeShotPercent * 3)) - (g_Ranking_NoAimingShotPercent * 4);
        shootingScore = shootingScore / 10;

        if (shootingScore < 0)
        {
            shootingScore = 0;
        }
        else if (shootingScore > 10)
        {
            shootingScore = 10;
        }

        g_Ranking_Score += shootingScore;

        if (shootingScore == 10)
        {
            g_Ranking_TextColors.killPercentages = StringColorId_Gold;
        }
    }
    else
    {
        g_Ranking_TextColors.killPercentages = StringColorId_Gold;
        g_Ranking_Score    += 10;
    }

    g_Ranking_TextColors.enemyKills = StringColorId_White;
    var_a0_3           = g_SavegamePtr->field_278;
    var_a0_3           = var_a0_3 - 15;
    var_a0_3           = var_a0_3 - (g_Ranking_ClearGameCount * 5);

    if (var_a0_3 < 0)
    {
        var_a0_3 = 0;
    }

    var_a0_3 += 5;

    var_v0_5 = g_Ranking_HasFiredShot ? g_Ranking_EnemyKillsShooting : 100;

    var_v1   = var_v0_5 / var_a0_3;
    var_a0_3 = g_Ranking_EnemyKillsMelee / var_a0_3;

    if (var_a0_3 < var_v1)
    {
        var_v1 /= 2;
    }
    else
    {
        var_a0_3 /= 2;
    }

    var_v1 = var_v1 + var_a0_3;

    if (var_v1 >= 31)
    {
        var_v1 = 30;
    }
    else if (var_v1 < 0)
    {
        var_v1 = 0;
    }

    temp3       = &g_Ranking_Score;
    g_Ranking_Score += var_v1;

    if (temp3 == NULL)
    {
        g_Ranking_Score += 0;
    }

    if (var_v1 == 30)
    {
        g_Ranking_TextColors.enemyKills = StringColorId_Gold;
    }

    if (g_Ranking_GameDifficulty == GameDifficulty_Easy)
    {
        g_Ranking_Score += -5;
    }

    g_Ranking_Score = CLAMP(g_Ranking_Score, 0, 100);
}

void Ranking_PrepareSavegame() // 0x800904F4
{
    u8 extraOptions;

    g_SavegamePtr->locationId         = g_Ranking_EndLocationId;
    g_SavegamePtr->clearGameCount     = g_Ranking_ClearGameCount;
    g_SavegamePtr->clearGameEndings   = g_Ranking_ClearGameEndings;
    g_SavegamePtr->currentEndingFlags = ((g_Ranking_CurrentEndingFlags & GameEndingFlag_Ufo) * 8) + GameEndingFlag_6; // Adds `GameEndingFlag_7` if `GameEndingFlag_Ufo` is set? TODO: Deoptimize.
    g_SavegamePtr->isNextFearMode     = 1;
    g_SavegamePtr->mapIdx             = MapIdx_MAP0_S01;

    if (g_Ranking_HasUsedGasolineOnChainsaw)
    {
        Savegame_EventFlagSet(EventFlag_M2S00_UsedGasolineOnChainsaw);
    }

    if (g_Ranking_HasUsedGasolineOnRockDrill)
    {
        Savegame_EventFlagSet(EventFlag_M2S00_UsedGasolineOnRockDrill);
    }

    Savegame_EventFlagSet(EventFlag_M0S02_UnlockedGasolineTank);

    extraOptions = g_GameWork.config.extraOptionsEnabled;

    if ((extraOptions & 0x3) && (extraOptions & 0xC))
    {
        Savegame_EventFlagSet(EventFlag_M2S00_UnlockedKatanaRoom);
    }

    if (g_GameWork.config.extraOptionsEnabled & (1 << 0))
    {
        Savegame_EventFlagSet(EventFlag_M0S02_UnlockedChannelingStone);
    }

    if (g_Ranking_Score >= 80)
    {
        g_SavegamePtr->pickedUpSpecialItemCount = 8;
        if (g_Ranking_Score == 100)
        {
            g_SavegamePtr->currentEndingFlags      |= GameEndingFlag_5;
            g_SavegamePtr->pickedUpSpecialItemCount = 16;
        }
    }
}

void Ranking_DisplayInfo(u32* arg0) // 0x80090664
{
    s32      temp_s1;
    s32      i;
    s32      temp_a0;
    s32      temp_v1;
    POLY_G4* poly;

    GsOT* ot = &g_OrderingTable2[g_ActiveBufferIdx];

    char* D_8002B4C0[] = {
        "GAME_RESULT",
        "Mode",
        "EASY",
        "NORMAL",
        "HARD",
        "Saves",
        "Continues",
        "Total_time",
        "h",
        "m",
        "s",
        ".",
        "km",
        "Walking_distance",
        "Running_distance",
        "Items",
        "/",
        "+",
        "Game_clear",
        "Ending",
        "GOOD+",
        "GOOD",
        "BAD+",
        "BAD",
        "UFO",
        "==Your_rank==",
        "@",
        "Defeated_\x01\x01""enemy_\x01\x01""by_\x01\x01shooting",
        "Defeated_\x01\x01""enemy_\x01\x01""by_\x01\x01""fighting",
        "Shooting_style",
        "Short_range_shots",
        "Middle_range_shots",
        "Long_range_shots",
        "No_aiming_shots"
    };

    Gfx_StringColorSet(StringColorId_White);

    if (g_GameWork.gameStateSteps[1] < 23)
    {
        Gfx_StringPositionSet(100, -84);
        Gfx_StringDraw(D_8002B4C0[0], 20);
        Gfx_StringPositionSet(24, -56);
        Gfx_StringDraw(D_8002B4C0[1], 20);

        switch (g_Ranking_GameDifficulty)
        {
            case GameDifficulty_Easy:
                Gfx_StringPositionSet(252, -56);
                Gfx_StringDraw(D_8002B4C0[2], 20);
                break;

            case GameDifficulty_Normal:
                Gfx_StringPositionSet(222, -56);
                Gfx_StringDraw(D_8002B4C0[3], 20);
                break;

            case GameDifficulty_Hard:
                Gfx_StringPositionSet(247, -56);
                Gfx_StringDraw(D_8002B4C0[4], 20);
                break;

            default:
                break;
        }

        temp_s1 = Ranking_DigitCount(g_Ranking_SavegameCount);
        Gfx_StringPositionSet(24, 7);
        Gfx_StringDraw(D_8002B4C0[5], 20);
        Gfx_StringPositionSet(286 - ((temp_s1 - 1) * 10), 7);
        Gfx_StringColorSet(g_Ranking_TextColors.savegameCount);
        Gfx_StringDrawInt(3, g_Ranking_SavegameCount);
        Gfx_StringColorSet(StringColorId_White);

        temp_s1 = Ranking_DigitCount(g_Ranking_ContinueCount);
        Gfx_StringPositionSet(24, 28);
        Gfx_StringDraw(D_8002B4C0[6], 20);
        Gfx_StringPositionSet(286 - ((temp_s1 - 1) * 10), 28);
        Gfx_StringColorSet(g_Ranking_TextColors.continueCount);
        Gfx_StringDrawInt(3, g_Ranking_ContinueCount);
        Gfx_StringColorSet(StringColorId_White);

        temp_s1 = Ranking_DigitCount(g_Ranking_GameplayHours);
        Gfx_StringPositionSet(24, 49);
        Gfx_StringDraw(D_8002B4C0[7], 20);
        Gfx_StringPositionSet(220 - (temp_s1 * 10), 49);
        Gfx_StringColorSet(g_Ranking_TextColors.gameplayTime);
        Gfx_StringDrawInt(5, g_Ranking_GameplayHours);
        Gfx_StringPositionSet(220, 49);
        Gfx_StringDraw(D_8002B4C0[8], 5);
        Gfx_StringPositionSet(254 - (Ranking_DigitCount(g_Ranking_GameplayMinutes) * 10), 49);
        Gfx_StringDrawInt(2, g_Ranking_GameplayMinutes);
        Gfx_StringPositionSet(254, 49);
        Gfx_StringDraw(D_8002B4C0[9], 5);
        Gfx_StringPositionSet(288 - (Ranking_DigitCount(g_Ranking_GameplaySeconds) * 10), 49);
        Gfx_StringDrawInt(2, g_Ranking_GameplaySeconds);
        Gfx_StringPositionSet(288, 49);
        Gfx_StringDraw(D_8002B4C0[10], 5);
        Gfx_StringColorSet(StringColorId_White);

        temp_s1 = Ranking_DigitCount(g_Ranking_WalkDistanceKm);
        Gfx_StringPositionSet(24, 70);
        Gfx_StringDraw(D_8002B4C0[13], 20);
        Gfx_StringPositionSet(229 - ((temp_s1 - 1) * 10), 70);
        Gfx_StringDrawInt(4, g_Ranking_WalkDistanceKm);
        Gfx_StringDraw(D_8002B4C0[11], 5);

        if (g_Ranking_WalkDistanceM < 100)
        {
            Gfx_StringDrawInt(1, 0);
        }

        if (g_Ranking_WalkDistanceM < 10)
        {
            Gfx_StringDrawInt(1, 0);
        }

        Gfx_StringDrawInt(3, g_Ranking_WalkDistanceM);
        Gfx_StringDraw(D_8002B4C0[12], 5);

        temp_s1 = Ranking_DigitCount(g_Ranking_RunDistanceKm);
        Gfx_StringPositionSet(24, 91);
        Gfx_StringDraw(D_8002B4C0[14], 20);
        Gfx_StringPositionSet(229 - ((temp_s1 - 1) * 10), 91);
        Gfx_StringDrawInt(4, g_Ranking_RunDistanceKm);
        Gfx_StringDraw(D_8002B4C0[11], 5);

        if (g_Ranking_RunDistanceM < 100)
        {
            Gfx_StringDrawInt(1, 0);
        }

        if (g_Ranking_RunDistanceM < 10)
        {
            Gfx_StringDrawInt(1, 0);
        }

        Gfx_StringDrawInt(3, g_Ranking_RunDistanceM);
        Gfx_StringDraw(D_8002B4C0[12], 5);

        temp_s1 = Ranking_DigitCount(g_Ranking_PickedUpItemCount);
        Gfx_StringPositionSet(24, 112);
        Gfx_StringDraw(D_8002B4C0[15], 20);
        Gfx_StringPositionSet(218 - ((temp_s1 - 1) * 10), 112);
        Gfx_StringColorSet(g_Ranking_TextColors.pickedUpItemCount);
        Gfx_StringDrawInt(3, (s32) g_Ranking_PickedUpItemCount);
        Gfx_StringDraw(D_8002B4C0[16], 5);
        Gfx_StringDrawInt(3, 204);
        Gfx_StringColorSet(g_Ranking_TextColors.pickedUpSpecialItemCount);
        Gfx_StringDraw("(", 1);

        if (g_Ranking_PickedUpSpecialItemCount != 0)
        {
            Gfx_StringDraw(D_8002B4C0[17], 5);
            Gfx_StringDrawInt(1, g_Ranking_PickedUpSpecialItemCount);
        }

        Gfx_StringPositionSet(292, 112);
        Gfx_StringDraw(")", 1);
        Gfx_StringColorSet(StringColorId_White);

        temp_s1 = Ranking_DigitCount(g_Ranking_ClearGameCount);
        Gfx_StringPositionSet(24, -35);
        Gfx_StringDraw(D_8002B4C0[18], 20);
        Gfx_StringPositionSet(286 - ((temp_s1 - 1) * 10), -35);
        Gfx_StringColorSet(g_Ranking_TextColors.clearGameCount);
        Gfx_StringDrawInt(3, g_Ranking_ClearGameCount);
        Gfx_StringColorSet(StringColorId_White);
        Gfx_StringPositionSet(24, -14);
        Gfx_StringDraw(D_8002B4C0[19], 20);
        Gfx_StringColorSet(g_Ranking_TextColors.ending);

        switch (g_Ranking_CurrentEndingFlags & 0x1F)
        {
            case GameEndingFlag_GoodPlus:
                Gfx_StringPositionSet(234, -14);
                Gfx_StringDraw(D_8002B4C0[20], 10);
                break;

            case GameEndingFlag_Good:
                Gfx_StringPositionSet(244, -14);
                Gfx_StringDraw(D_8002B4C0[21], 10);
                break;

            case GameEndingFlag_BadPlus:
                Gfx_StringPositionSet(249, -14);
                Gfx_StringDraw(D_8002B4C0[22], 10);
                break;

            case GameEndingFlag_Bad:
                Gfx_StringPositionSet(258, -14);
                Gfx_StringDraw(D_8002B4C0[23], 10);
                break;

            case GameEndingFlag_Ufo:
                Gfx_StringPositionSet(256, -14);
                Gfx_StringDraw(D_8002B4C0[24], 10);
                break;

            default:
                break;
        }

        Gfx_StringColorSet(StringColorId_White);
        Gfx_StringPositionSet(24, 154);
        Gfx_StringDraw(D_8002B4C0[27], 32);
        Gfx_StringPositionSet(296 - (Ranking_DigitCount(g_Ranking_EnemyKillsShooting) * 10), 154);
        Gfx_StringColorSet(g_Ranking_TextColors.enemyKills);
        Gfx_StringDrawInt(4, g_Ranking_EnemyKillsShooting);
        Gfx_StringColorSet(StringColorId_White);
        Gfx_StringPositionSet(24, 133);
        Gfx_StringDraw(D_8002B4C0[28], 32);
        Gfx_StringPositionSet(296 - (Ranking_DigitCount(g_Ranking_EnemyKillsMelee) * 10), 133);
        Gfx_StringColorSet(g_Ranking_TextColors.enemyKills);
        Gfx_StringDrawInt(4, g_Ranking_EnemyKillsMelee);

        if (g_Ranking_HasFiredShot)
        {
            Gfx_StringColorSet(StringColorId_White);
            Gfx_StringPositionSet(24, 175);
            Gfx_StringDraw(D_8002B4C0[29], 32);
            Gfx_StringPositionSet(72, 196);
            Gfx_StringDraw(D_8002B4C0[30], 32);
            Gfx_StringPositionSet(262, 196);
            Gfx_StringColorSet(g_Ranking_TextColors.killPercentages);
            Gfx_StringDraw(g_Credits_Digits.f0, 5);
            Gfx_StringColorSet(StringColorId_White);
            Gfx_StringPositionSet(72, 217);
            Gfx_StringDraw(D_8002B4C0[31], 32);
            Gfx_StringPositionSet(262, 217);
            Gfx_StringColorSet(g_Ranking_TextColors.killPercentages);
            Gfx_StringDraw(g_Credits_Digits.f1, 5);
            Gfx_StringColorSet(StringColorId_White);
            Gfx_StringPositionSet(72, 238);
            Gfx_StringDraw(D_8002B4C0[32], 32);
            Gfx_StringPositionSet(262, 238);
            Gfx_StringColorSet(g_Ranking_TextColors.killPercentages);
            Gfx_StringDraw(g_Credits_Digits.f2, 5);
            Gfx_StringColorSet(StringColorId_White);
            Gfx_StringPositionSet(72, 259);
            Gfx_StringDraw(D_8002B4C0[33], 32);
            Gfx_StringPositionSet(262, 259);
            Gfx_StringColorSet(g_Ranking_TextColors.killPercentages);
            Gfx_StringDraw(g_Credits_Digits.f3, 5);
        }

        Gfx_StringColorSet(StringColorId_White);
        Gfx_StringPositionSet(24, 292);
        Gfx_StringDraw(D_8002B4C0[25], 20);

        if (g_Ranking_Score >= 100)
        {
            Gfx_StringColorSet(0);
        }
        else
        {
            Gfx_StringColorSet(StringColorId_White);
        }

        temp_v1 = g_Ranking_Score / 10;
        temp_a0 = g_Ranking_Score % 10;

        if (temp_v1 != 0 && temp_a0 == 0)
        {
            Gfx_StringPositionSet(160, 198);

            g_SysWork.enableHalfHeightGlyphs = true;

            for (i = 0; i < temp_v1; i++)
            {
                Gfx_StringDraw(D_8002B4C0[26], 1);
            }

            g_SysWork.enableHalfHeightGlyphs = false;
        }
        else if (temp_v1 == 0 && temp_a0 != 0)
        {
            Gfx_StringPositionSet(160, 292);

            g_SysWork.enableHalfHeightGlyphs = false;

            for (i = 0; i < temp_a0; i++)
            {
                Gfx_StringDraw(D_8002B4C0[26], 1);
            }
        }
        else
        {
            g_SysWork.enableHalfHeightGlyphs = true;
            Gfx_StringPositionSet(160, 194);

            for (i = 0; i < temp_v1; i++)
            {
                Gfx_StringDraw(D_8002B4C0[26], 20);
            }

            g_SysWork.enableHalfHeightGlyphs = false;
            Gfx_StringPositionSet(160, 300);

            for (i = 0; i < temp_a0; i++)
            {
                Gfx_StringDraw(D_8002B4C0[26], 20);
            }
        }

        Gfx_StringColorSet(StringColorId_White);

        for (i = 0; i < 2; i++)
        {
            poly = (POLY_G4*)GsOUT_PACKET_P;
            setPolyG4(poly);

            switch (g_Ranking_GameDifficulty)
            {
                case -1:
                    setRGB1(poly, -128 - (i << 7), -64 - i * 192, 0);
                    setRGB3(poly, -128 - (i << 7), -64 - i * 192, 0);
                    setRGB0(poly, i << 7, i * 192, 0);
                    setRGB2(poly, i << 7, i * 192, 0);
                    break;

                case 0:
                    setRGB1(poly, 0, -128 - (i << 7), -64 - i * 192);
                    setRGB3(poly, 0, -128 - (i << 7), -64 - i * 192);
                    setRGB0(poly, 0, i << 7, i * 192);
                    setRGB2(poly, 0, i << 7, i * 192);
                    break;

                case 1:
                    setRGB1(poly, -128 - (i << 7), 0, -64 - i * 192);
                    setRGB3(poly, -128 - (i << 7), 0, -64 - i * 192);
                    setRGB0(poly, i << 7, 0, i * 192);
                    setRGB2(poly, i << 7, 0, i * 192);
                    break;

                default:
                    break;
            }

            setXY4(poly,
                   -64, (i * 6) - 196,
                   -64, ((i + 1) * 6) - 196,
                    66, (i * 6) - 196,
                    66, ((i + 1) * 6) - 196);

            addPrim((u8*)ot->org + 0x1C, poly);
            GsOUT_PACKET_P = (u8*)poly + 0x24;
        }
    }
}

u32 Ranking_DigitCount(u32 n) // 0x8009134C
{
    u32 digits = 1;

    while (n >= 10)
    {
        n /= 10;
        digits++;
    }

    return digits;
}

void func_80091380(void) {} // 0x80091380

void func_80091388(s32 arg0, s32 arg1, s32 arg2) // 0x80091388
{
    s32 var_a0;
    s32 var_v0;

    if (arg1 <= 0)
    {
        arg1 = 0;
    }
    else
    {
        arg0 = (arg0 - arg1) - 1;
    }

    Text_Debug_Draw(Text_Debug_IntToString(arg0, FP_FROM(arg2, Q12_SHIFT)));

    if (arg1 > 0)
    {
        Text_Debug_Draw(".");
        do
        {
            arg2  -= Q12_TRUNC(arg2);
            arg2  *= 10;
            var_a0 = arg2 % Q12(10.0f);
            Text_Debug_Draw(Text_Debug_IntToString(1, FP_FROM(var_a0, Q12_SHIFT)));
        }
        while (--arg1 > 0);
    }
}

void func_80091464(void) {} // 0x80091464

u32 func_8009146C(s32 arg0) // 0x8009146C
{
    s32 var_v0;
    u32 res;
    u32 var_v1_2;

    if (arg0 == 0)
    {
        var_v1_2 = g_SavegamePtr->meleeKillCount;
        var_v0   = (g_SavegamePtr->meleeKillCountB & 0xF) << 8;
    }
    else
    {
        var_v1_2 = g_SavegamePtr->rangedKillCount;
        var_v0 = (g_SavegamePtr->meleeKillCountB & 0xF0) * 16;
    }

    res = var_v1_2 + var_v0;
    if (res > 4000)
    {
        res = 4000;
    }
    return res;
}

void func_800914C4(s32 arg0, u32 val) // 0x800914C4
{
    if (val > 4000)
    {
        val = 4000;
    }

    if (arg0 == 0)
    {
        g_SavegamePtr->meleeKillCount  = val;
        g_SavegamePtr->meleeKillCountB = (g_SavegamePtr->meleeKillCountB & 0xF0) | ((val >> 8) & 0x0F);
    }
    else
    {
        g_SavegamePtr->rangedKillCount = val;
        g_SavegamePtr->meleeKillCountB = (g_SavegamePtr->meleeKillCountB & 0x0F) | ((val >> 4) & 0xF0);
    }
}

void func_8009151C(u32 arg0, s32 arg1, s32 arg2)
{
    switch (arg0)
    {
        case 4:
            if (g_SavegamePtr->field_278 < 50000)
            {
                g_SavegamePtr->field_278++;
            }

        case 0:
        case 1:
        case 2:
        case 3:
        case 5:
            if (g_SavegamePtr->firedShotCount < 50000)
            {
                g_SavegamePtr->firedShotCount++;

                if (arg0 != 5)
                {
                    if (arg2 > 0x8000)
                    {
                        arg2 = 0x8000;
                    }

                    g_SavegamePtr->field_260 += arg2 >> 4;
                }

                switch (arg0)
                {
                    case 0:
                        if (g_SavegamePtr->closeRangeShotCount < 50000)
                        {
                            g_SavegamePtr->closeRangeShotCount++;
                            return;
                        }
                        break;

                    case 1:
                        if (g_SavegamePtr->midRangeShotCount < 50000)
                        {
                            g_SavegamePtr->midRangeShotCount++;
                        }

                        if (arg1 != 0)
                        {
                            if (g_SavegamePtr->field_26E < 50000)
                            {
                                g_SavegamePtr->field_26E++;
                                return;
                            }
                        }
                        break;

                    case 2:
                        if (g_SavegamePtr->longRangeShotCount < 50000)
                        {
                            g_SavegamePtr->longRangeShotCount++;
                        }

                        if (arg1 != 0)
                        {
                            if (g_SavegamePtr->field_270 < 50000)
                            {
                                g_SavegamePtr->field_270++;
                                return;
                            }
                        }
                        break;

                    case 3:
                        if (g_SavegamePtr->field_26C < 50000)
                        {
                            g_SavegamePtr->field_26C++;
                        }

                        if (arg1 != 0)
                        {
                            if (g_SavegamePtr->field_272 < 50000)
                            {
                                g_SavegamePtr->field_272++;
                                return;
                            }
                        }
                        break;

                    case 4:
                    case 5:
                        break;
                }
            }
            break;

        case 7:
            if (g_SavegamePtr->field_278 < 50000)
            {
                g_SavegamePtr->field_278++;
            }

        case 6:
            if (g_SavegamePtr->field_274 < 50000)
            {
                g_SavegamePtr->field_274++;
            }

            if (arg1 != 0)
            {
                if (g_SavegamePtr->field_276 < 50000)
                {
                    g_SavegamePtr->field_276++;
                }
            }
            break;
    }
}
