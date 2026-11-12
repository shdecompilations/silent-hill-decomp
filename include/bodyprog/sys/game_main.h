#ifndef _BODYPROG_SYS_GAMEMAIN_H
#define _BODYPROG_SYS_GAMEMAIN_H

// ======
// ENUMS
// ======

/** @brief Reset types used in `MainLoop_ShouldWarmReset`. */
typedef enum _ResetType
{
    ResetType_None     = 0,
    ResetType_WarmBoot = 2
} e_ResetType;

// ========
// GLOBALS
// ========

extern s32 g_WarmBootTimer;

// ==========
// FUNCTIONS
// ==========

void GameState_Init_Update(void);

/** Bodyprog entrypoint. Called by `main`. */
void MainLoop(void);

#endif
