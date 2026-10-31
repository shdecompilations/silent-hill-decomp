#ifndef _BODYPROG_GAMEBOOT_GAMEBOOT_H
#define _BODYPROG_GAMEBOOT_GAMEBOOT_H

// ==========
// FUNCTIONS
// ==========

/** @brief State: Loading screen. */
void GameState_LoadScreen_Update(void);

/** @brief Handles the load of the in-game state and room transitions. */
void GameBoot_InGameStartup(void);

/** @brief Initalizes environmental effects, characters information, reset player
 * state and sets some information about the currently loaded overlay.
 */
void GameBoot_InGameInit(void);

/** @brief Load save's information. */
void GameBoot_SavegameInitialize(s8 overlayId, s32 difficulty);

/** @brief Initializes the playable character data and world data. */
void GameBoot_WorldInit(void);

#if VERSION_REGION_IS(PAL)
    /** @brief Returns the language-specific map overlay ID for a given map index. */
    e_FsFile GameBoot_MapFileIdGet(s32 mapIdx);
#endif

/** Loads a map file into `g_OvlDynamic`. */
void GameBoot_MapLoad(s32 mapIdx);

#if VERSION_REGION_IS(PAL)
    /** @brief Loads an item language overlay if the loaded overlay doesn't match current language ID. */
    void GameBoot_ItemLangLoad(void);
#endif

/** @brief Sets general environmental effects such as lighting, fog, and screen effects.
 *
 * @note Weathers conditions like snow, rain, and the ending's fire rain are set by
 * code exclusively found in the map overlay.
 */
void GameBoot_WolrdEnvInit(s32 unused);

/** @brief Unused. */
void func_80035B98(void);

/** @unused and broken.
 * Intended to draw a background image when a loading screen with the string "STAGE X-X" appears.
 *
 * This loading screen will only work with the first two map overlays.
 */
void GameBoot_LoadScreen_BackgroundImg(void);

void GameBoot_LoadScreen_PlayerRun(void);


#endif
