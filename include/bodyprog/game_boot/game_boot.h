#ifndef _BODYPROG_GAMEBOOT_GAMEBOOT_H
#define _BODYPROG_GAMEBOOT_GAMEBOOT_H

/** @brief Loading screen IDs. */
typedef enum _LoadingScreenId
{
    LoadingScreenId_None              = 0,
    LoadingScreenId_PlayerRun         = 1,
    LoadingScreenId_BackgroundTexture = 2,
    LoadingScreenId_StageString       = 3
} e_LoadingScreenId;

/** @brief Handles the loading screen game state. */
void GameState_LoadScreen_Update(void);

/** @brief Handles the load of the in-game state and room transitions. */
void GameBoot_InGameStartup(void);

/** @brief Initalizes environmental effects and character info. Additionally, it resets the player state and sets some
 * info about the currently loaded overlay.
 */
void GameBoot_InGameInit(void);

/** @brief Load save's information. */
void GameBoot_SavegameInitialize(s8 overlayId, s32 difficulty);

/** @brief Initializes player and world data. */
void GameBoot_WorldInit(void);

#if VERSION_REGION_IS(PAL)
    /** @brief Returns the language-specific map overlay ID for a given map index.
     *
     * @param mapIdx Map index.
     * @return Map overlay file ID.
     */
    e_FsFile GameBoot_MapFileIdGet(s32 mapIdx);
#endif

/** @brief Loads a map file into `g_OvlDynamic`.
 *
 * @param mapIdx Index of the map to load.
 */
void GameBoot_MapLoad(s32 mapIdx);

#if VERSION_REGION_IS(PAL)
    /** @brief Loads an item language overlay if the loaded overlay doesn't match the current language ID. */
    void GameBoot_ItemLangLoad(void);
#endif

// TODO: The below don't belong here. Move to load_screen.h

/** @brief Sets general environmental effects such as lighting, fog, and screen effects.
 *
 * @note Weathers conditions like snow, rain, and the ending's fire rain are set by
 * code exclusively found in the map overlay.
 */
void GameBoot_WorldEnvInit(s32 unused);

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
