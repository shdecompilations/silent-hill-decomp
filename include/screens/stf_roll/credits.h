#ifndef _SCREENS_CREDITS_H
#define _SCREENS_CREDITS_H

#include "common.h"

#define CREDITS_CODE_OFFSET_POS_1   '\x10'
#define CREDITS_CODE_OFFSET_POS_2   '\x11'
#define CREDITS_CODE_OFFSET_POS_4   '\x12'
#define CREDITS_CODE_OFFSET_POS_8   '\x13'
#define CREDITS_CODE_OFFSET_POS_16  '\x14'
#define CREDITS_CODE_OFFSET_POS_32  '\x15'
#define CREDITS_CODE_OFFSET_POS_64  '\x16'
#define CREDITS_CODE_OFFSET_POS_128 '\x17'
#define CREDITS_CODE_OFFSET_NEG_1   '\x18'
#define CREDITS_CODE_OFFSET_NEG_2   '\x19'
#define CREDITS_CODE_OFFSET_NEG_4   '\x1A'
#define CREDITS_CODE_OFFSET_NEG_8   '\x1B'
#define CREDITS_CODE_OFFSET_NEG_16  '\x1C'
#define CREDITS_CODE_OFFSET_NEG_32  '\x1D'
#define CREDITS_CODE_OFFSET_NEG_64  '\x1E'
#define CREDITS_CODE_OFFSET_NEG_128 '\x1F'

/** @brief Used for normal credits screen. */
typedef struct
{
    /* 0x0  */ s16  textX;      /** Prim vertex X. */
    /* 0x2  */ s16  textY;      /** Prim vertex Y. */
    /* 0x4  */ s16  leftMargin; /** Width? */
    /* 0x6  */ s8   lineHeight; /** Height? */
    /* 0x7  */ s8   semiTrans;  /** `bool` | Blend flag. */
    /* 0x8  */ s32  color;      /** Rgb24 color + code. */
    /* 0xC  */ s16* widthTable; /** Points to 0x801E5C24, data size 400. */
    /* 0x10 */ s32* colorTable; /** Points to 0x801E5E24, data size 28, rgb24 + code. */
    /* 0x14 */ u16  tpage;      /** Tpage for `setDrawTPage`, calculated from `uv` with `Credits_TextBlendSet`. */
    /* 0x16 */ s16  clut;       /** Clut XY. */
    /* 0x18 */ u32  uv;         /** Texture UV, tpageXY might be more accurate?. */
} s_CreditTextState;
STATIC_ASSERT_SIZEOF(s_CreditTextState, 28);

/** @brief Used for UFO ending credits screen. */
typedef struct
{
    /* 0x0  */ s_CreditTextState sub_0;
    /* 0x1C */ s32        field_1C;
    /* 0x20 */ s16        field_20;
    /* 0x22 */ s16        field_22;
    /* 0x24 */ s32        field_24;
    /* 0x28 */ s32        field_28;
    /* 0x2C */ s32        field_2C;
    /* 0x30 */ s32        field_30;
    /* 0x34 */ q19_12     field_34;
    /* 0x38 */ q19_12     field_38;
    /* 0x3C */ q19_12     field_3C;
    /* 0x40 */ q19_12     field_40;
    /* 0x44 */ q19_12     field_44;
    /* 0x48 */ q19_12     field_48;
    /* 0x4C */ q19_12     field_4C;
    /* 0x50 */ q19_12     field_50;
    /* 0x54 */ q19_12     field_54;
} s_CreditText3dState;
STATIC_ASSERT_SIZEOF(s_CreditText3dState, 0x58);

// Used by `func_801E2E28`.
typedef struct
{
    s16 xaSfxId;     /** SFX ID. */
    s16 scrollDelay; /** Initial value for the scrolling text timer, negative values cause delay before text is shown. */
    s16 field_4;     /** Text draw duration? Unsure. */
} s_801E5558;
STATIC_ASSERT_SIZEOF(s_801E5558, 6);

extern s_801E5558 D_801E5558[];

extern char* D_801E5590[]; // Credits string table.

extern char D_801E5BD0[]; // KCET string.

extern s32 D_801E5C20;

extern s16 D_801E5C24[256];

extern s32 g_CreditsColorTable[7];

extern s32 g_CreditsColorTable3d[7];

extern u8 D_801E5FF8[];

extern u8 D_801E5E5C[];

extern u8 D_801E5E6C[];

extern s32 D_801E5E74;

extern s32 D_801E5E78;

extern s32 D_801E5E7C;

extern s32 D_801E5E80;

extern s32 D_801E5E84;

extern s32 D_801E5E88;

extern s32 D_801E5E8C; // Index for some array used by most recent `func_801E2E28` call. `func_801E2ED8` accesses it.

extern u8 D_801E5E90[];

extern u8 D_801E5FD0[];

extern u32 D_801E600C; // Random 16-bit number.

void func_801E2E28(s32 idx);

bool func_801E2ED8(void);

bool func_801E2FC0(void);

void GameState_Credits_Update(void);

/** Draw some image on the screen. */
bool func_801E3124(void);

bool func_801E342C(void);

bool func_801E3304(void);

bool func_801E3684(void);

bool func_801E3970(void);

void func_801E386C(void);

void func_801E3DD0(void);

s32 func_801E3DF8(u8 arg0);

void Credits_TextPositionSet(s32 x, s32 y);

/** @brief Sets the current RGB+command color for `g_CreditTextState` struct.
 *
 * Sets the packed RGB+command color for `g_CreditTextState`,
 * with the command component hard-set to `PRIM_RECT | RECT_TEXTURE`.
 *
 * @param r Red component.
 * @param g Green component.
 * @param b Blue component.
 */
void Credits_TextColorSet(s32 r, s32 g, s32 b);

void Credits_TextLineHeightSet(s8 arg0);

void Credits_TextBlendSet(bool semiTrans, u32 arg1);

void Credits_TextDraw(char* str);

void Credits_Text3dPositionSet(s32 arg0, s32 arg1);

/** @brief Sets the current RGB+command color for `D_800AFE2C`.
 *
 * Sets the packed RGB+command color for `D_800AFE10`,
 * with the command component hard-set to `GPU_COM_TF4`.
 *
 * @param r Red component.
 * @param g Green component.
 * @param b Blue component.
 */
void Credits_Text3dColorSet(s32 r, s32 g, s32 b);

void Credits_Text3dLineHeightSet(s8 arg0);

void Credits_Text3dBlendSet(bool semiTrans, u32 arg1);

void Credits_Text3dDraw(u8* str);

#endif
