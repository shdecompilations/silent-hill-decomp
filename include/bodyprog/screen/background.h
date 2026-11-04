#ifndef _BODYPROG_SCREEN_BACKGROUNDDRAW_H
#define _BODYPROG_SCREEN_BACKGROUNDDRAW_H

// ========
// GLOBALS
// ========

/** @brief Gamma intensity of background images. */
extern q0_8 g_Screen_BackgroundImgGamma;

// ==========
// FUNCTIONS
// ==========

/** @brief Draws a background image.
 *
 * Scratch: https://decomp.me/scratch/sYGPW
 *
 * @note Only applies to menus and the preview of maps when grabbing them.
 *
 * @param image Image to draw.
 */
void Screen_BackgroundImgDraw(s_FsImageDesc* image);

/** @brief Draws a transition between images.
 *
 * Scratch: https://decomp.me/scratch/1MhOK
 *
 * @param imageIn Previous image to transition from.
 * @param imageOut Image to transition into.
 * @param alpha Blend alpha to the second image.
 */
void Screen_BackgroundImgTransition(s_FsImageDesc* imageIn, s_FsImageDesc* imageOut, q3_12 alpha);

/** @brief Sets a black background and slowly fades in a background image.
 *
 * Scratch: https://decomp.me/scratch/IFpWU
 *
 * @note Most of the code is shared with `Screen_BackgroundImgTransition`.
 *
 * @param image Image to draw.
 */
void Screen_BackgroundImgDrawBlackFadeIn(s_FsImageDesc* image);

/** @brief Applies motion blur to the background from the scene.
 *
 * Scratch (USA/JAP): https://decomp.me/scratch/0T9qN
 * Scratch (EUR): https://decomp.me/scratch/juLOd
 *
 * @note Used only in the loading screen.
 *
 * @param vBlanks `e_SyncMode`.
 * @return Always returns `false`.
 */
bool Screen_BackgroundMotionBlur(s32 vBlanks);

#endif
