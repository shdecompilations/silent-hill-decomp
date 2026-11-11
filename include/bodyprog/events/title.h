#ifndef _BODYPROG_EVENTS_TITLE_H
#define _BODYPROG_EVENTS_TITLE_H

/** @brief Main menu entries. */
typedef enum _MainMenuEntry
{
    MainMenuEntry_Load     = 0,
    MainMenuEntry_Continue = 1,
    MainMenuEntry_Start    = 2,
    MainMenuEntry_Option   = 3,
    MainMenuEntry_Extra    = 4, /** @unused The extra options menu may have been accessible via the main menu. */

    MainMenuEntry_Count    = 5
} e_MainMenuEntry;

/** @brief Main menu states. */
typedef enum _MainMenuState
{
    MainMenuState_Start              = 0,
    MainMenuState_Main               = 1,
    MainMenuState_LoadGame           = 2,
    MainMenuState_DifficultySelector = 3,
    MainMenuState_NewGameStart       = 4
} e_MainMenuState;

/** Counts the amount of times that demos has been play in the current game session. */
extern s8 g_Demo_ReproducedCount;

/** Related to main menu fog randomization. */
extern s8* D_800BCDE0; // Type assumed.

void GameState_MainMenu_Update(void);

void MainMenu_SelectedOptionIdxReset(void);

void MainMenu_FogUpdate(void);

#endif
