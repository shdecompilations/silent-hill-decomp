#ifndef _BODYPROG_GFX_ENVEFFECTS_H
#define _BODYPROG_GFX_ENVEFFECTS_H

typedef enum _EffectTextureFlags
{
    EffectTextureFlag_None         = 0,
    EffectTextureFlag_Glass        = 1 << 1, /** Broken glass in the cafe Air Screamer cutscene. */
    EffectTextureFlag_WaterRefract = 1 << 2, /** Water waves and light reflection in the sewer. */
    EffectTextureFlag_Water        = 1 << 3,
    EffectTextureFlag_Fire         = 1 << 4,
    EffectTextureFlag_Ef           = 1 << 5, // TODO: Rename. Looks like stringy flesh?
    EffectTextureFlag_Blood        = 1 << 6,
    EffectTextureFlag_WarmTest     = 1 << 7  /** @unused Test lens flare? */
} e_EffectTextureFlags;

typedef struct
{
    /* 0x0  */ MATRIX  field_0; // View matrix?
    /* 0x20 */ SVECTOR field_20;
    /* 0x28 */ VECTOR  field_28; // Q27.4
    /* 0x38 */ s32     field_38;
    /* 0x3C */ s32     field_3C[5];
    /* 0x50 */ s32     field_50;
} s_func_8006342C;

typedef struct
{
    /* 0x0 */ u8  field_0;
    /* 0x1 */ u8  field_1;
    /* 0x2 */ s16 field_2; // XYZ? X and Y swapped?
    /* 0x4 */ s32 field_4; //
    /* 0x8 */ s32 field_8; //
} s_800C42E8;

typedef struct
{
    /* 0x0 */ s16 field_0;
    /* 0x2 */ s16 field_2;
    /* 0x4 */ s16 field_4;
    /* 0x6 */ s16 field_6;
    /* 0x8 */ u16 field_8;
    /* 0xA */ u16 field_A;
    /* 0xC */ u8  field_C;
} s_800C4418;

extern s_800C42E8 D_800C42E8[24];

extern s16 D_800C4408;

extern s16 __pad_bss_800C440A;

extern GsCOORDINATE2* D_800C440C;

extern GsCOORDINATE2* D_800C4410;

/** Flags. */
extern s8 D_800C4414;

extern s_800C4418 D_800C4418;

void Map_EffectTexturesLoad(s32 mapIdx);

void func_8005E414(s32 orgIdx);

/** @brief Does something with `unkTable1_4C`, clears blood splats and initialize drawn of objects. */
void func_8005E650(s32 mapId);

void func_8005E70C(void);

s32 func_8005E7E0(s32 arg0);

// Crucial for displaying particle effects. (Not general enviroments. Blood, shoot effects)
void func_8005E89C(void);

s32 func_8005F55C(s32 arg0);

/** Checks `floorType`. */
bool func_8005F680(s_CollisionSurface* surface);

/** Related to camera collision? */
void func_8005F6B0(s_SubCharacter* chara, VECTOR* pos, s32 arg2, s32 arg3);

bool func_80060044(POLY_FT4** poly, s32 idx);

bool func_800611C0(POLY_FT4** poly, s32 idx);

/** Spawns blood effect? */
void func_800622B8(s32 unused, s_SubCharacter* chara, s32 animStatus, s32 arg3);

void func_800625F4(VECTOR3* arg0, q3_12 angle, s32 arg2, s32 arg3);

bool func_80062708(POLY_FT4** poly, s32 idx);

/** Displays gunfire effects. */
void func_8006342C(s32 weaponAttack, q3_12 rotY, q3_12 rotX, GsCOORDINATE2* coord);

bool func_80063A50(POLY_FT4** poly, s32 idx);

// Important for drawing gun smoke effect.
bool func_80064334(POLY_FT4** poly, s32 idx);

void func_80064F04(VECTOR3* arg0, s8 arg1, q3_12 arg2);

bool func_80064FC0(POLY_FT4** polys, s32 idx);

#endif
