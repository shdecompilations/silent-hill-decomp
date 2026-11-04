#ifndef _BODYPROG_SCREEN_SCREENDRAW_H
#define _BODYPROG_SCREEN_SCREENDRAW_H

#include "bodyprog/screen/fade.h"
#include "bodyprog/screen/cutscene_border.h"

// ==========
// FUNCTIONS
// ==========

void Screen_RectInterlacedClear(s16 x, s16 y, s16 w, s16 h, u8 r, u8 g, u8 b);

void Screen_Refresh(s32 screenWidth, bool isInterlaced);

void Screen_Init(s32 screenWidth, bool isInterlaced);

void Screen_XyPositionSet(s32 x, s32 y);

void Screen_DisplayEnvXySet(DISPENV* displayEnv, s32 x, s32 y);

#endif
