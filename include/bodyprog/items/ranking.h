#ifndef _BODYPROG_RANKING_H
#define _BODYPROG_RANKING_H

/** @brief Ranking system. */

// ========
// STRUCTS
// ========

typedef struct _RankingTextColors
{
    /* 0x0 */ u8 ending;
    /* 0x1 */ u8 clearGameCount;
    /* 0x2 */ u8 savegameCount;
    /* 0x3 */ u8 continueCount;
    /* 0x4 */ u8 gameplayTime;
    /* 0x5 */ u8 pickedUpItemCount;
    /* 0x6 */ u8 pickedUpSpecialItemCount;
    /* 0x7 */ u8 enemyKills;
    /* 0x8 */ u8 killPercentages;
} s_RankingTextColors;

// ========
// GLOBALS
// ========

// Data below is likely part of a collected results data struct.

//extern s_800AFDF4 D_800AFDF4;

/** Savegame count. */
extern s16 g_Ranking_SavegameCount;

/** Gameplay time. */
extern u16 g_Ranking_GameplayHours;

extern u16 g_Ranking_WalkDistanceKm;

extern u16 g_Ranking_WalkDistanceM;

extern u16 g_Ranking_RunDistanceKm;

extern u16 g_Ranking_RunDistanceM;

/** Picked up item count. */
extern u16 g_Ranking_PickedUpItemCount;

extern u8 g_Ranking_GameplayMinutes;

extern u8 g_Ranking_GameplaySeconds;

/** Clear game count. */
extern u8 g_Ranking_ClearGameCount;

/** Clear game endings. */
extern u8 g_Ranking_ClearGameEndings;

extern u8 g_Ranking_CurrentEndingFlags;

/** Location ID. */
extern u8 g_Ranking_EndLocationId;

extern u8 g_Ranking_PickedUpSpecialItemCount;

extern s8 g_Ranking_Score;

extern u8 g_Ranking_HasUsedGasolineOnChainsaw;

extern u8 g_Ranking_HasUsedGasolineOnRockDrill;

extern u16 g_Ranking_EnemyKillsShooting;

extern u16 g_Ranking_EnemyKillsMelee;

/** Has fired shots. */
extern bool g_Ranking_HasFiredShot;

/** Close range shot count. */
extern u32 g_Ranking_CloseRangeShotPercent;

/** Mid range shot count. */
extern u32 g_Ranking_MidRangeShotPercent;

/** Long range shot count. */
extern u32 g_Ranking_LongRangeShotPercent;

extern u32 g_Ranking_NoAimingShotPercent;

/** Game difficulty. */
extern s8 g_Ranking_GameDifficulty;

/** Continue count. */
extern u8 g_Ranking_ContinueCount;

extern s8 __pad_bss_800C48D2[14];

extern s_RankingTextColors g_Ranking_TextColors;

// ==========
// FUNCTIONS
// ==========

/** Processes and collects results data. */
void Ranking_EvaluateScore(void);

void Ranking_PrepareSavegame(void);

void Ranking_DisplayInfo(u32* arg0); // 0x80090664

void func_80090664(void);

u32 Ranking_DigitCount(u32 n);

void func_80091380(void);

void func_80091388(s32 arg0, s32 arg1, s32 arg2);

void func_80091464(void);

u32 func_8009146C(s32 arg0);

void func_800914C4(s32 arg0, u32 val);

void GameFs_StfRollBinLoad(void);

void func_800917C0(void* arg0, s32 arg1, s_FsImageDesc* img);

void func_8009185C(s16 arg0, s16 arg1, s32 arg2, q19_12 arg3, q19_12 arg4, q19_12 arg5, q19_12 arg6, q19_12 arg7, s32 arg8, s32 arg9);

#endif
