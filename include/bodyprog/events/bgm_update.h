#ifndef _BODYPROG_EVENTS_BGMUPDATE_H
#define _BODYPROG_EVENTS_BGMUPDATE_H

/** @brief Main background music update.
 *
 * @note Mainly handles music layer updates. `Bgm_Update` and `Bgm_MenuUpdate` invoke the logic that updates tracks when
 * a map overlay event sets `g_MapOverlayHdr.bgmEvent`.
 */

// ==========
// CONSTANTS
// ==========

#define BGM_LAYER_COUNT 9

// ======
// ENUMS
// ======

/** @brief Background music status flags. */
typedef enum _BgmStatusFlags
{
    BgmStatusFlag_None        = 0,
    BgmStatusFlag_Pause       = 1 << 0,
    BgmStatusFlag_ApplyMute   = 1 << 1,
    BgmStatusFlag_RadioActive = 1 << 2,
    BgmStatusFlag_Duck        = 1 << 3,
    BgmStatusFlag_4           = 1 << 4, /** The only flag completely unrelated to background music.
                                         * Used exclusively by Stalker AI for movement, targeting, or player seek logic.
                                         * Stubbing or clearing this flag breaks Stalker behavior entirely.
                                         */
    BgmStatusFlag_VoiceDialog = 1 << 5,
    BgmStatusFlag_6           = 1 << 6, /** Used to re-trigger looping audio effects when returning to gameplay.
                                         * Certain looping sounds stop during game state changes and this flag is assigned
                                         * upon returning to gameplay to resume those looping sounds.
                                         *
                                         * Used in four instances:
                                         * - The wheelchair at the beginning of the game.
                                         * - The radio when the Air Screamer enters the diner.
                                         * - An event in map overlay `MAP6_02`.
                                         * - Bloodsucker logic.
                                         *
                                         * Example: In the second instance, opening the inventory menu stops the radio.
                                         * If this flag is not set when returning to gameplay, the radio sound will not resume.
                                         */
    BgmStatusFlag_RequestMute = 1 << 7
} e_BgmStatusFlags;

/** @brief Background music flags. */
typedef enum _BgmFlags
{
    BgmFlag_Layer1    = 1 << 0,
    BgmFlag_Layer2    = 1 << 1,
    BgmFlag_Layer3    = 1 << 2,
    BgmFlag_Layer4    = 1 << 3,
    BgmFlag_Layer5    = 1 << 4,
    BgmFlag_Layer6    = 1 << 5,
    BgmFlag_Layer7    = 1 << 6,
    BgmFlag_Layer8    = 1 << 7,
    BgmFlag_KeepAlive = 1 << 8,
    BgmFlag_MuteAll   = 1 << 9
} e_BgmFlags;

/** @brief Background music update commands and indices. */
typedef enum _BgmCmd
{
    BgmCmd_UpdateLayers = 0,
    BgmCmd_UpdateTrack  = 1,
    BgmCmd_Track2       = 2,
    BgmCmd_Track3       = 3,
    BgmCmd_Track4       = 4,
    BgmCmd_Track5       = 5,
    BgmCmd_Track6       = 6,
    BgmCmd_Track7       = 7,
    BgmCmd_Track8       = 8,
    BgmCmd_Track9       = 9,
    BgmCmd_Track10      = 10,
    BgmCmd_Track11      = 11,
    BgmCmd_Track12      = 12,
    BgmCmd_Track13      = 13,
    BgmCmd_Track14      = 14,
    BgmCmd_Track15      = 15,
    BgmCmd_Track16      = 16,
    BgmCmd_Track17      = 17,
    BgmCmd_Track18      = 18,
    BgmCmd_Track19      = 19,
    BgmCmd_Track20      = 20,
    BgmCmd_Track21      = 21,
    BgmCmd_Track22      = 22,
    BgmCmd_Track23      = 23,
    BgmCmd_Track24      = 24,
    BgmCmd_Track25      = 25,
    BgmCmd_Track26      = 26,
    BgmCmd_Track27      = 27,
    BgmCmd_Track28      = 28,
    BgmCmd_Track29      = 29,
    BgmCmd_Track30      = 30,
    BgmCmd_Track31      = 31,
    BgmCmd_Track32      = 32,
    BgmCmd_Track33      = 33,
    BgmCmd_Track34      = 34,
    BgmCmd_Track35      = 35,
    BgmCmd_Track36      = 36,
    BgmCmd_Track37      = 37,
    BgmCmd_Track38      = 38,
    BgmCmd_Track39      = 39,
    BgmCmd_Track40      = 40,
    BgmCmd_Track41      = 41
} e_BgmCmd;

// ========
// STRUCTS
// ========

/** @brief Background music track layer limits. */
typedef struct _BgmLayerLimits
{
    /* 0x0 */ u8 limits[8];
} s_BgmLayerLimits;

// ==========
// FUNCTIONS
// ==========

/**
 * @brief Updates the in-game background music state and invokes the map's BGM handler.
 *
 * @param updateTrack `true` when the overlay uses multiple tracks to switch the active track.
 * When passed as `false`, only the track's audio layers are updated.
 */
void Bgm_Update(bool updateTrack);

/** @brief Mutes global variables for music layer volumes. */
void Bgm_LayerGlobalVariablesMute(void);

/** @brief @unused Checks if any audio channel for music or music layer is active.
 * 
 * @return `true` if any audio channel for music or music layer is active, `false` is everything is inactive.
 */
bool Bgm_MuteCheck(void);

/** @brief Updates background music layers.
 *
 * @param bgmFlags `e_BgmStatusFlags`.
 * @param fadeSpeed Volume fade speed.
 * @param layerLimits BGM layer limits.
 */
void Bgm_LayersUpdate(e_BgmStatusFlags bgmFlags, q19_12 fadeSpeed, s_BgmLayerLimits* layerLimits);

/** @brief Updates background music state in menus and invokes a map's background music handler.
 *
 * @note This has the same behaviour as `Bgm_Update`, but additionally disables radio static.
 */
void Bgm_MenuUpdate(void);

void Bgm_SongChange(s32 bgmIdx);

// ========================================
// PLAYER ROOM INFORMATION
// ========================================

void Game_MapRoomIdxUpdate(void);

/** @unused */
s32 func_8003647C(void);

/** @unused */
s32 func_80036498(void);

// ========================================
// UNKNOWN UNUSED MATH
// ========================================

// Used in some RoomBgmInit funcs.
u32 func_800364BC(void);

#endif
