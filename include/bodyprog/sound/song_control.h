#ifndef _BODYPROG_SOUND_SONGCONTROL_H
#define _BODYPROG_SOUND_SONGCONTROL_H

/** @brief Main background music and ambience song utils.
 *
 * Handles the initialization and changes of music and environmental sound when moving in a map.
 * `Sd_BgmControl` updates music, while `Sd_AmbienceControl` updates environmental sound effects.
 */

// TODO: Enums for `Sd_BgmControl` and `Sd_AmbienceControl`.

// =================
// BACKGROUND MUSIC
// =================

/** @brief Controls the active background music song.
 *
 * @note The only instance of this function returning `0` is when triggering the main loading screen.
 *
 * @return `-1` if something is being loaded or audio is being played,
 * `0` if music has been muted or updated, or
 * `1` if failed.
 */
s32 Sd_BgmControl(void);

/** @brief Checks if a given background music song isn't currently playing, or if it's a command used to trigger a
 * layer update or song change.
 *
 * @param bgmIdx Index of the background music song to check.
 * @return `true` if the song is not currently playing, `false` otherwise or if it's a command.
 */
bool Sd_ActiveBgmCheck(s32 bgmIdx);

/** @brief Sets a new backgrund music song.
 *
 * @note This changes songs when moving through the map or loading a savegame with a map containing multiple songs.
 *
 * @param bgmIdx Index of the new background music song to set.
 */
void Sd_BgmSet(s32 bgmIdx);

/** @brief Updates the backgrund music song if `g_MapOverlayHdr.bgmCmd` is set to `BgmCmd_UpdateTrack`.
 *
 * @note This changes songs when moving through the map or loading a savegame with a map containing multiple songs.
 */
void Sd_BgmUpdate(void);

/** @brief Update channels being used by backgrund music songs.
 * TODO: Properly document after investigating game's channel functionallity.
 */
void Sd_BgmChannelSet(void);

// =========
// AMBIENCE
// =========

/** @brief Controls the active ambience song.
 *
 * @return `-1` if something is being loaded or audio is being played,
 * `0` if nothing has been updated, or
 * `1` if audio has been updated.
 */
s32 Sd_AmbienceControl(void);

/** @brief Checks if a given ambience song isn't currently playing.
 *
 * @param ambienceIdx Index of the ambience song to check.
 * @return `true` if the ambience song isn't currently playing, `false` otherwise.
 */
bool Sd_ActiveAmbienceCheck(s32 ambienceIdx);

/** @brief Sets a new ambience song.
 *
 * @param ambienceIdx Index of the ambience song to set.
 */
void Sd_AmbienceSet(s32 ambienceIdx);

#endif
