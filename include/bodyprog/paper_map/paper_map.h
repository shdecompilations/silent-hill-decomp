#ifndef _BODYPROG_PAPERMAP_PAPERMAP_H
#define _BODYPROG_PAPERMAP_PAPERMAP_H

// Paper map marking info?
typedef struct
{
    /* 0x0 */ u8 field_0;
    /* 0x1 */ u8 field_1;
    /* 0x2 */ u8 field_2;
    /* 0x3 */ u8 field_3; // Map marking sprite height?
} s_800AE8A0_0;
typedef struct // Maybe the same? Both related to `s_800AEDBC`.
{
    /* 0x0 */ s8 field_0;
    /* 0x1 */ s8 field_1;
    /* 0x2 */ u8 field_2;
    /* 0x3 */ u8 field_3;
} s_800AE8A0_4;

/** Paper map marking graphic data? */
typedef struct
{
    /* 0x0 */ POLY_FT4*    field_0;
    /* 0x4 */ s_800AE8A0_0 field_4;
    /* 0x8 */ s_800AE8A0_4 field_8;
    /* 0xC */ s32          field_C; // Count.
} s_func_80068E0C;

void GameState_PaperMapScreen_Update(void);

s32 func_80067914(s32 paperMapIdx, u16 arg1, u16 arg2, q4_12 scale);

bool func_80068CC0(s32 arg0);

/** Paper map func. Seems to draw map marking sprites. */
bool func_80068E0C(s32 arg0, s32 idx, s32 arg2, s32 shade, u16 arg4, u16 arg5, q4_12 scale);

void PaperMap_DrawScaled(u16 scrollX, u16 scrollY, q4_12 scale);

#endif
