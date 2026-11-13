#ifndef _BODYPROG_SYS_FSSCREENS_H
#define _BODYPROG_SYS_FSSCREENS_H

/** Seeks for the English title screen background graphic. */
void GameFs_TitleGfxSeek(void);

/** Loads the English title screen background graphic. */
void GameFs_TitleGfxLoad(void);

/** Seeks for the stream overlay. */
void GameFs_StreamBinSeek(void);

/** Loads the stream overlay. */
void GameFs_StreamBinLoad(void);

/** Loads the options background graphic and overlay. */
void GameFs_OptionBinLoad(void);

/** Loads the save/load background graphic and overlay. */
void GameFs_SaveLoadBinLoad(void);

/** @brief @unused Draws loading screen text.
 *
 * Just like in earlier builds of Silent Hill 2, the game was intended to show a black screen with "Now loading." text
 * in middle of the screen.
 */
void GameFs_LoadingTextDraw(void);

#endif
