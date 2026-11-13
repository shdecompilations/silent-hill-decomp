#ifndef _BODYPROG_GAMEBOOT_BACKGROUNDSOUNDINIT_H
#define _BODYPROG_GAMEBOOT_BACKGROUNDSOUNDINIT_H

/** @brief Main background music and ambient track utils.
 *
 * Handles the initialization and changes of music and environmental sound when moving in a map.
 * `Sd_BgmInit` updates music, while `Sd_AmbientSfxInit` updates environmental sound effects.
 */

// TODO: Enums for `Sd_BgmInit` and `Sd_AmbientSfxInit`.

// ====
// BGM 
// ====

/** @brief Initializes background music or updates the current track.
 *
 * @note The only unstance of this function returning `0` is used to trigger the main loading screen.
 *
 * @return `-1` if something is being loaded or audio is being played,
 * `0` if music has been muted or updated, or
 * `1` if failed.
 */
s32 Sd_BgmInit(void);

/** @brief Checks if a given background music track isn't currently playing, or if it's a command used to trigger a
 * layer update or track change.
 *
 * @param bgmIdx Index of the background music track to check.
 * @return `true` if the track is not currently playing, `false` otherwise or if it's a command.
 */
bool Sd_BgmActiveSongCheck(s32 bgmIdx);

/** @brief Updates a track.
 *
 * @note Changes tracks when moving through the map or when loading a save with a map containing multiple tracks.
 *
 * @param bgmIdx `e_BgmCmd`.
 */
void Sd_BgmSongSet(s32 bgmIdx);

/** @brief Updates a track if `g_MapOverlayHdr.bgmCmd` is set to `BgmCmd_UpdateTrack`.
 *
 * @note Changes tracks when moving through the map or when loading a save with a map containing multiple tracks.
 */
void Sd_BgmUpdateTrack(void);

/** @brief Update channels being used by songs.
 * TODO: Properly document after investigating game's channel functionallity.
 */
void Sd_BgmChannelSet(void);

// ==============
// AMBIENT SOUND 
// ==============

/** @brief Initializes enviromental audio or updates the current audio track.
 *
 * @return `-1` if something is being loaded or audio is being played,
 * `0` if nothing has been updated, or
 * `1` if audio has been updated.
 */
s32 Sd_AmbientSfxInit(void);

/** @brief Checks if a given background audio is not currently playing.
 *
 * @param ambientIdx Index of the background audio to check.
 * @return `true` if the given audio is not currently playing, `false` otherwise.
 */
bool Sd_ActiveAmbientSfxCheck(s32 ambientIdx);

/** @brief Sets a new background music track.
 *
 * @param bgmIdx Index of the background music track to set.
 */
void Sd_AmbientSfxSet(s32 bgmIdx);

#endif
