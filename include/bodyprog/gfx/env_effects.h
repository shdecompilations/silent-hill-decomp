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

void Map_EffectTexturesLoad(s32 mapIdx);

void func_8005E414(s32 orgIdx);

/** @brief Does something with `unkTable1_4C`, clears blood splats and initialize drawn of objects. */
void func_8005E650(s32 mapId);

void func_8005E70C(void);

s32 func_8005E7E0(s32 arg0);

// Crucial for displaying particle effects. (Not general enviroments. Blood, shoot effects)
void func_8005E89C(void);

s32 func_8005F55C(s32 arg0);

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
