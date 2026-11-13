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
    /* 0x0   */ VECTOR3 field_0; // Q23.8 | Position.
    /* 0xC   */ MATRIX  worldToScreenMat;
    /* 0x2C  */ s32     field_2C;
    /* 0x30  */ DVECTOR field_30;
    /* 0x34  */ q3_12   field_34[24];
    /* 0x64  */ q3_12   field_64[24];
    /* 0x94  */ q3_12   field_94[24];
    /* 0xC4  */ s16     field_C4;
    /* 0xC6  */ s16     field_C6;
    /* 0xC8  */ q3_12   field_C8;
    /* 0xCA  */ q3_12   field_CA;
    /* 0xCC  */ q3_12   field_CC; // Angle?
    /* 0xCE  */ s16     field_CE;
    /* 0xD0  */ q19_12  field_D0;
    /* 0xD4  */ q19_12  field_D4;
    /* 0xD8  */ s32     field_D8;
    /* 0xDC  */ s16     field_DC[4];
    /* 0xE4  */ s16     field_E4[4];
                union
                {
                    DVECTOR field_0[4]; // Q19.12
                    s32     raw_0[4];
    /* 0xEC  */ } u_field_EC;
                union
                {
                    DVECTOR field_0[4];
                    s32     raw_0[4];
    /* 0xFC  */ } u_field_FC; // Q3.12 | Positions or offsets.
    /* 0x10C */ q19_12  field_10C[4]; // X offsets?
    /* 0x11C */ q19_12  field_11C[4]; // Z offsets?
} s_func_8005E89C;

typedef struct
{
    /* 0x0   */ s_func_8005E89C field_0;
    /* 0x12C */ DR_PRIO*        field_12C;
    /* 0x130 */ CVECTOR         field_130;
    /* 0x134 */ CVECTOR         field_134;
    /* 0x138 */ SVECTOR         field_138;
    /* 0x140 */ s32             field_140;
    /* 0x144 */ DVECTOR         field_144;
    /* 0x148 */ u16             field_148;
    /* 0x14A */ u8              unk_14A[2];
    /* 0x14C */ s32             field_14C;
    /* 0x150 */ s32             field_150;
    /* 0x154 */ s32             field_154;
    /* 0x158 */ s32             field_158;
    /* 0x15C */ s32             field_15C;
    /* 0x160 */ s32             field_160;
    /* 0x164 */ s32             field_164;
    /* 0x168 */ s32             field_168;
    /* 0x16C */ s32             field_16C;
    /* 0x170 */ s32             field_170;
    /* 0x174 */ s32             field_174;
} s_func_80060044;

typedef struct
{
    /* 0x0   */ s_func_8005E89C field_0;
    /* 0x12C */ CVECTOR         field_12C;
    /* 0x130 */ CVECTOR         field_130;
    /* 0x134 */ SVECTOR         field_134;
    /* 0x13C */ SVECTOR         field_13C;
    /* 0x144 */ SVECTOR         field_144;
    /* 0x14C */ VECTOR3         field_14C;
    /* 0x158 */ s32             field_158;
    /* 0x15C */ s32             field_15C;
    /* 0x160 */ s32             field_160;
    /* 0x164 */ s32             field_164;
    /* 0x168 */ s32             field_168;
    /* 0x16C */ DVECTOR         field_16C;
    /* 0x170 */ s32             field_170;
    /* 0x174 */ s32             field_174;
    /* 0x178 */ q19_12          field_178;
    /* 0x17C */ s32             field_17C;
    /* 0x180 */ s32             field_180;
    /* 0x184 */ s32             field_184;
    /* 0x188 */ s32             field_188;
    /* 0x18C */ s32             field_18C;
    /* 0x190 */ s32             field_190;
    /* 0x194 */ s32             field_194;
    /* 0x198 */ s32             field_198;
    /* 0x19C */ s32             field_19C;
    /* 0x1A0 */ s32             field_1A0;
} s_func_800611C0;

typedef struct
{
    /* 0x0   */ s_func_8005E89C field_0;
    /* 0x12C */ CVECTOR         field_12C;
    /* 0x130 */ CVECTOR         field_130;
    /* 0x134 */ SVECTOR         field_134[25];
    /* 0x1FC */ VECTOR3         field_1FC; // Q19.12
    /* 0x208 */ q20_12          field_208;
    /* 0x20C */ s32             field_20C;
    /* 0x210 */ s32             field_210;
    /* 0x214 */ s32             field_214[25];
    /* 0x278 */ DVECTOR         field_278[25];
    /* 0x2DC */ s32             field_2DC;
} s_func_80062708;

typedef struct
{
    /* 0x0   */ s_func_8005E89C field_0;
    /* 0x12C */ SVECTOR         field_12C;
    /* 0x138 */ s8              unk_138[8];
    /* 0x13C */ DVECTOR         field_13C;
    /* 0x140 */ s32             field_140;
    /* 0x144 */ s32             field_144;
} s_func_80064FC0;

typedef struct
{
    /* 0x0   */ s_func_8005E89C field_0;
    /* 0x12C */ MATRIX          field_12C;
    /* 0x14C */ SVECTOR         field_14C[3];
    /* 0x164 */ SVECTOR         field_164;
    /* 0x16C */ VECTOR          field_16C[3];
    /* 0x19C */ VECTOR          field_19C;
    /* 0x1AC */ VECTOR          field_1AC; // Q27.4
    /* 0x1BC */ s32             field_1BC;
    /* 0x1C0 */ s32             field_1C0;
    /* 0x1C4 */ s32             field_1C4;
    /* 0x1C8 */ s32             field_1C8;
    /* 0x1CC */ DVECTOR         field_1CC;
    /* 0x1D0 */ s16             field_1D0;
    /* 0x1D2 */ u8              unk_1D2[2];
    /* 0x1D4 */ s32             field_1D4; // Count.
    /* 0x1D8 */ s32             field_1D8;
    /* 0x1DC */ s32             field_1DC;
    /* 0x1E0 */ s32             field_1E0;
    /* 0x1E4 */ s32             field_1E4;
    /* 0x1E8 */ q19_12          field_1E8;
} s_func_80063A50;

typedef struct
{
    /* 0x0   */ s_func_8005E89C field_0;
    /* 0x12C */ DR_PRIO*        field_12C;
    /* 0x130 */ CVECTOR         field_130;
    /* 0x134 */ CVECTOR         field_134;
    /* 0x138 */ SVECTOR         field_138;
    /* 0x140 */ u8              unk_140[16];
    /* 0x150 */ s32             field_150;
    /* 0x154 */ DVECTOR         field_154;
    /* 0x158 */ u16             field_158;
    /* 0x15A */ u16             field_15A;
    /* 0x15C */ u8              field_15C;
    /* 0x160 */ s32             field_160;
    /* 0x164 */ s32             field_164;
    /* 0x168 */ s32             field_168;
} s_func_80064334;

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
