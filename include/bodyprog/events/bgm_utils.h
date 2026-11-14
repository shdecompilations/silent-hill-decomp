#ifndef _BODYPROG_EVENTS_BGMUTILS_H
#define _BODYPROG_EVENTS_BGMUTILS_H

/** @brief Background music song handling and effects.
 *
 * @note Most of these functions are are unused, and the only used one is only rarely called in final maps.
 */
 
/** @brief Plays a new background music song. If the song is already playing, the request is ignored.
 *
 * @param bgmIdx Index of the background song to play.
 */
void Bgm_NewSongPlay(s32 bgmIdx);

/** @brief Handles a multi-step crossfade to a new background music song.
 *
 * @param bgmIdx Index of the background song to play.
 */
void Bgm_CrossfadeToTrack(s32 bgmIdx);

/** @brief Crossfades the currently playing background song to silence. */
void Bgm_CrossfadeToSilence(void);

/** @brief @unused Immediately stops the active background music song. */
void Bgm_SongStop(void);

/** @brief @unused Fades out the active background music song to a stop.
 *
 * @param useSlowFade Use slow fade.
 */
void Bgm_SongFadeOut(bool useSlowFade);

#endif
