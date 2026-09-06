#ifndef _BODYPROG_BODYPROG_H
#define _BODYPROG_BODYPROG_H

#include "game.h"
#include "bodyprog/sound/sfx.h"
#include "bodyprog/collision/collision.h"
#include "bodyprog/chara/chara_model.h"
#include "bodyprog/chara/spawn.h"
#include "bodyprog/events/bgm_update.h"
#include "bodyprog/events/events_util.h"
#include "bodyprog/events/map_msg.h"
#include "bodyprog/formats/ipd.h"
#include "bodyprog/formats/lm.h"
#include "bodyprog/formats/model.h"
#include "bodyprog/formats/texture.h"
#include "bodyprog/formats/tmd.h"
#include "bodyprog/gfx/world.h"
#include "bodyprog/gfx/world_object.h"
#include "bodyprog/map/map.h"
#include "bodyprog/map/terrain.h"
#include "bodyprog/screen/screen_data.h"
#include "bodyprog/sound/sfx_id_enum.h"
#include "bodyprog/view/vw_system.h"
#include "main/fsqueue.h"

// ==========
// CONSTANTS
// ==========

#define OPT_SOUND_VOLUME_MIN   0
#define OPT_SOUND_VOLUME_MAX   128
#define OPT_VIBRATION_DISABLED 0
#define OPT_VIBRATION_ENABLED  128

// ======
// ENUMS
// ======

/** @brief SFX flags. */
typedef enum _SfxFlags
{
    SfxFlag_None          = 0,
    SfxFlag_Mono          = 1 << 0, /** Monophonic. */
    SfxFlag_NoDistAtten   = 1 << 1, /** No distance attenuation. */
    SfxFlag_UpdateAttribs = 1 << 2  /** @unused Update attributes. */
} e_SfxFlags;

// Used by `func_8006E490` and `func_8006E150`.
typedef enum _OrientationFlags
{
    OrientationFlags_None    = 0,
    OrientationFlags_InvertX = 1 << 0,
    OrientationFlags_InvertZ = 1 << 1,
    OrientationFlags_SwapXz  = 1 << 2
} e_OrientationFlags;

typedef enum _BoneHierarchy
{
    BoneHierarchy_MultiModel = -3,
    BoneHierarchy_End        = -2
} e_BoneHierarchy;

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

typedef enum _MainMenuEntry
{
    MainMenuEntry_Load     = 0,
    MainMenuEntry_Continue = 1,
    MainMenuEntry_Start    = 2,
    MainMenuEntry_Option   = 3,
    MainMenuEntry_Extra    = 4, /** @unused The extra options menu may have been accessible via the main menu. */

    MainMenuEntry_Count    = 5
} e_MainMenuEntry;

typedef enum _MainMenuState
{
    MenuState_Start              = 0,
    MenuState_Main               = 1,
    MenuState_LoadGame           = 2,
    MenuState_DifficultySelector = 3,
    MenuState_NewGameStart       = 4
} e_MainMenuState;

/** Used by `func_8003F654` to cast a specific field to the desired type. */
typedef enum _PrimitiveType
{
    PrimitiveType_None = 0,
    PrimitiveType_S8   = 1,
    PrimitiveType_U8   = 2,
    PrimitiveType_S16  = 3,
    PrimitiveType_U16  = 4,
    PrimitiveType_S32  = 5
} e_PrimitiveType;

typedef enum _LoadingScreenId
{
    LoadingScreenId_None              = 0,
    LoadingScreenId_PlayerRun         = 1,
    LoadingScreenId_BackgroundTexture = 2,
    LoadingScreenId_StageString       = 3
} e_LoadingScreenId;

// ================
// UNKNOWN STRUCTS
// ================
// TODO: Deobfuscate these and move where they belong afterwardd.

/** @brief Struct used by many functions involved with GTE. Kept at `PSX_SCRATCH_ADDR` (possibly only temporarily). */
typedef struct _GteScratchData
{
    /* 0x0   */ DVECTOR screenXy_0[90];
    /* 0x168 */ u16     screenZ_168[18];
    /* 0x18C */ s16     field_18C[99]; // The size changed from 150 due to the addition of `field_252`. Not sure if this is correct.
    /* 0x252 */ u8      field_252[102];
    /* 0x2B8 */ u8      field_2B8[200]; // Size likely incorrect.

                union
                {
                    MATRIX field_0;
                    struct
                    {
                        /* 0x0  */ s32     field_0;
                        /* 0x4  */ s32     field_4;
                        /* 0x8  */ CVECTOR field_8;
                        /* 0xC  */ CVECTOR field_C;
                        /* 0x10 */ u8      field_10; // `faceIdxs`.
                        /* 0x11 */ u8      field_11;
                        /* 0x12 */ u8      field_12;
                        /* 0x13 */ u8      field_13;
                        /* 0x14 */ u8      field_14;
                        /* 0x15 */ u8      field_15;
                        /* 0x16 */ u8      field_16;
                        /* 0x17 */ u8      field_17;
                        /* 0x18 */ s32     field_18;
                        /* 0x1C */ s32     field_1C;
                    } s_0;
    /* 0x380 */ } field_380;

    /* 0x3A0 */ s_Normal field_3A0;
    /* 0x3A4 */ DVECTOR  screenPos_3A4;
    /* 0x3A8 */ s32      depthP_3A8;
    /* 0x3AC */ SVECTOR  field_3AC;
    /* 0x3B4 */ u8       unk_3B4[36];
    /* 0x3D8 */ CVECTOR  field_3D8; // Written by `func_8005A42C`.
    /* 0x3DC */ DVECTOR  screenPos_3DC;
    /* 0x3E0 */ s32      depthP_3E0;
    /* 0x3E4 */ s16      rotMatrix_3E4[3][3]; // Truncated `MATRIX` without the `long t[3];` transfer vector?
} s_GteScratchData;

typedef struct
{
    /* 0x0 */ s8  field_0;
    /* 0x1 */ s8  field_1;
    /* 0x2 */ s8  field_2;
    /* 0x3 */ s8  field_3;
    /* 0x4 */ s32 field_4[4];
} s_800382B0;

typedef struct
{
    /* 0x0 */ s16 field_0; // Flags?
} s_8008D850;

/** Related to weapon attacks. Stats, SFX IDs, damange values, etc.? */
typedef struct
{
    /* 0x0  */ q4_12 field_0;
    /* 0x2  */ q3_12 field_2;
    /* 0x4  */ u16   field_4; // Related to damage. Multiplier?
    /* 0x6  */ s8    field_6; // Accessed by `func_8008BF84` as `u16`
    /* 0x7  */ s8    unk_7;
    /* 0x8  */ u8    field_8;   // Accessed by `func_8008BF84` as `u16`
    /* 0x9  */ u8    charaId_9; /** `e_CharaId` */
    /* 0xA  */ u8    field_A;   // Accessed by `func_8008BF84` as `u16`
    /* 0xB  */ u8    field_B;
    /* 0xC  */ q4_12 field_C;
    /* 0xE  */ u8    field_E;  // Keyframe index offset?
    /* 0xF  */ u8    field_F;  // Keyframe index offset?
    /* 0x10 */ u8    field_10; // State.
    /* 0x11 */ u8    field_11;
    /* 0x12 */ u8    field_12; // SFX ID subgroup. Uses values 0-4.
    /* 0x13 */ s8    __pad_13;
    /* 0x14 */ u32*  unk_14; // Some pointer. All entries have the same value `D_800AD4C4`.
} s_800AD4C8;
STATIC_ASSERT_SIZEOF(s_800AD4C8, 24);

/** @brief Radio noise data for nearby enemies. */
typedef struct _RadioNoise
{
    /* 0x0 */ s8 prevIdx;
    /* 0x1 */ s8 idx;
    /* 0x2 */ s8 closeNpcInfoIdx;
    /* 0x3 */ s8 unused; /** @unused */
} s_RadioNoise;
STATIC_ASSERT_SIZEOF(s_RadioNoise, 4);

typedef struct
{
    /* 0x0  */ u8      field_0; // `bool`?
    /* 0x1  */ u8      field_1; // `bool`?
    /* 0x2  */ u8      field_2; // `bool`?
    /* 0x3  */ u8      field_3;
    /* 0x4  */ s8      unk_4[4];
    /* 0x8  */ s16     field_8;
    /* 0xA  */ s16     field_A;
    /* 0xC  */ VECTOR3 field_C; // Q19.12
    /* 0x18 */ s8      unk_18[4];
    /* 0x1C */ s32     field_1C;
    /* 0x20 */ q19_12  field_20;
} s_800C4818;

typedef struct
{
    /* 0x0 */ u8  unk_0[4];
    /* 0x4 */ s16 field_4;
    /* 0x6 */ s16 field_6; // Keyframe index or time.
} s_800C44F0; // Probable size: 8 bytes.

typedef struct _MapEnvPresetIdxs
{
    /* 0x0 */ u8 presetIdx0;
    /* 0x1 */ u8 presetIdx1;
} s_MapEnvPresetIdxs;

typedef struct
{
    /* 0x0   */ DVECTOR  screenXy_0[90];
    /* 0x168 */ s16      screenZ_168[18];
    /* 0x18C */ s16      field_18C[72];
    /* 0x21C */ s32      field_21C[39]; // Used as `VECTOR3`?
    /* 0x2B8 */ u8       field_2B8[200];
    /* 0x380 */ MATRIX   field_380;
    /* 0x3A0 */ s_Normal field_3A0;
    /* 0x3A4 */ DVECTOR  screenPos_3A4;
    /* 0x3A8 */ s32      depthP_3A8;
    /* 0x3AC */ SVECTOR  field_3AC;
    /* 0x3B4 */ u8       unk_3B4[36];
    /* 0x3D8 */ CVECTOR  field_3D8;

                 // Different functions access different data at 0x3DC onwards.
                 // Union works for it, but also possible those functions just took different `s_GteScratchData` structs.
                 union
                 {
                     struct
                     {
                         /* 0x3DC */ DVECTOR screenPos_3DC;
                         /* 0x3E0 */ s32     depthP_3E0;
                         /* 0x3E4 */ s16     rotMatrix_3E4[3][3]; // Truncated `MATRIX` without the `long t[3];` transfer vector?
                     } vertex;

                     struct
                     {
                         /* 0x3DC */ s_Normal field_3DC;
                         /* 0x3E0 */ SVECTOR  field_3E0[3];
                     } normal;

                     struct
                     {
                         /* 0x0 */ u8  field_0;
                         /* 0x1 */ u8  field_1;
                         /* 0x2 */ u8  field_2;
                         /* 0x3 */ u8  field_3;
                         /* 0x4 */ u8  field_4;
                         /* 0x5 */ u8  field_5;
                         /* 0x6 */ u8  field_6;
                         /* 0x7 */ u8  field_7;
                         /* 0x8 */ s32 field_8;
                     } s_1;
    /* 0x3DC */ } u;
} s_GteScratchData2;

// Something for inventory items.
typedef struct
{
    /* 0x0  */ q3_12   positionY;
    /* 0x2  */ q3_12   field_2; // Move dist?
    /* 0x4  */ q3_12   field_4; // Angle.
    /* 0x6  */ s16     field_6;
    /* 0x8  */ s16     field_8;
    /* 0xA  */ s8      __pad_A[2];
    /* 0xC  */ SVECTOR field_C;  // Q3.12 | Offset?
    /* 0x14 */ SVECTOR position; /** Q3.12 */
} s_800AE204;

typedef struct
{
    /* 0x0 */ s_800AE204* ptr_0;
    /* 0x4 */ u8          count_4;
    /* 0x5 */ u8          unk_5;
    /* 0x6 */ q7_8        field_6;
    /* 0x8 */ u8          field_8;
    /* 0x9 */ u8          field_9;
    /* 0xA */ u8          field_A;
    /* 0xB */ u8          field_B;
} s_800AE4DC;

typedef struct
{
    /* 0x0 */ u8 field_0;
    /* 0x1 */ u8 field_1;
    /* 0x2 */ u8 field_2;
    /* 0x3 */ u8 field_3; // Map marking sprite height?
} s_800AE8A0_0;

typedef struct
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

typedef struct
{
    /* 0x0  */ POLY_FT4* field_0;
    /* 0x4  */ SVECTOR   field_4; // Q23.8
    /* 0xC  */ MATRIX    field_C;
    /* 0x2C */ VECTOR3   field_2C; // Q23.8 | Set as grid-aligned player position in `func_80065B94`.
    /* 0x38 */ s32       field_38;
    /* 0x3C */ DVECTOR   field_3C;
    /* 0x40 */ s32       field_40;
    /* 0x44 */ DVECTOR   field_44;
    /* 0x48 */ DVECTOR   field_48;
} s_func_80065B94;

typedef struct
{
    /* 0x0  */ POLY_GT4* field_0;
    /* 0x4  */ MATRIX    field_4;
    /* 0x24 */ SVECTOR   field_24[3];
    /* 0x3C */ VECTOR3   field_3C; // Q19.12
    /* 0x48 */ s32       field_48;
    /* 0x4C */ DVECTOR   field_4C;
    /* 0x50 */ DVECTOR   field_50;
    /* 0x54 */ DVECTOR   field_54;
    /* 0x58 */ DVECTOR   field_58;
    /* 0x5C */ s32       field_5C;
    /* 0x60 */ s32       field_60;
    /* 0x64 */ s32       field_64;
    /* 0x68 */ s32       field_68;
    /* 0x6C */ s32       field_6C;
    /* 0x70 */ s32       field_70;
    /* 0x74 */ s32       field_74;
    /* 0x78 */ s32       field_78;
} s_func_80066184;

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
    /* 0x12C */ PACKET*         field_12C;
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
    /* 0x0  */ POLY_G4*  field_0;
    /* 0x4  */ DR_TPAGE* field_4;
    /* 0x8  */ SVECTOR   field_8[3];
    /* 0x20 */ MATRIX    field_20;
    /* 0x40 */ VECTOR3   field_40; // Q23.8
    /* 0x4C */ s32       field_4C;
    /* 0x50 */ s16       field_50;
    /* 0x54 */ s32       field_54[3];
    /* 0x60 */ s16       field_60[3];
    /* 0x68 */ s32       field_68;
} s_func_800652F4;

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
    /* 0x12C */ PACKET*         field_12C;
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
    /* 0x0   */ s_func_8005E89C    field_0;
    /* 0x12C */ s_CollisionSurface surface;
    /* 0x138 */ MATRIX             field_138;
    /* 0x158 */ SVECTOR            field_158[4];
    /* 0x178 */ VECTOR             field_178[4];
    /* 0x1B8 */ CVECTOR            field_1B8;
    /* 0x1BC */ CVECTOR            field_1BC;
    /* 0x1C0 */ s32                field_1C0;
    /* 0x1C4 */ s32                field_1C4;
    /* 0x1C8 */ s32                field_1C8;
    /* 0x1CC */ s32                field_1CC;
    /* 0x1D0 */ s32                field_1D0;
    /* 0x1D4 */ DVECTOR            field_1D4;
} s_func_800CD1F8;

typedef struct
{
    /* 0x0  */ MATRIX  field_0; // View matrix?
    /* 0x20 */ SVECTOR field_20;
    /* 0x28 */ VECTOR  field_28; // Q27.4
    /* 0x38 */ s32     field_38;
    /* 0x3C */ s32     field_3C[5];
    /* 0x50 */ s32     field_50;
} s_func_8006342C;

// ========
// GLOBALS
// ========

extern s_FsImageDesc g_MainImg0; // 0x80022C74 - TODO: Part of main exe, move to `main/` headers?

extern char D_80028544[16];

extern RECT D_80028A20;

extern const s_AnimInfo HARRY_WEAPON_ANIM_INFOS[];

extern const s_800C44F0 D_800294F4[];

extern const s_AnimInfo* D_800297B8;

extern u_Filename D_8002B2CC;

extern s_FsImageDesc g_Font16AtlasImg;
extern s_FsImageDesc g_KonamiLogoImg;
extern s_FsImageDesc g_KcetLogoImg;
extern s_FsImageDesc g_TitleImg;
extern s_FsImageDesc g_PaperMapImg;
extern s_FsImageDesc g_PaperMapMarkingAtlasImg;
extern s_FsImageDesc g_ItemInspectionImg;
extern s_FsImageDesc g_LoadingScreenImg;
extern s_FsImageDesc g_ControllerButtonAtlasImg;
extern s_FsImageDesc g_BrightnessScreenImg0;
extern s_FsImageDesc g_BrightnessScreenImg1;
extern s_FsImageDesc g_DeathTipImg;
extern s_FsImageDesc g_HealthPortraitImg;
extern s_FsImageDesc g_InventoryKeyItemTextureImg;
extern s_FsImageDesc g_FirstAidKitItemTextureImg;

/** Some intentory item texture (`ITEM/TIM07.TIM`). */
extern s_FsImageDesc D_800A9074;

extern s_FsImageDesc D_800A907C;

extern s_FsImageDesc D_800A9084;

extern s_FsImageDesc D_800A908C;

extern s_FsImageDesc D_800A9094;

extern s_FsImageDesc g_Font24AtlasImg;

extern s_MapEffectsInfo MAP_EFFECTS_INFOS[21];

extern s_StructUnk3 D_800A952C;

extern s32 D_800A9A20;

/** X. */
extern s32 D_800A9A24;

/** Z. */
extern s32 D_800A9A28;

/** Loaded NPC types in memory and their `g_CharaModelAnimsData` indices. */
extern s8 g_CharaAnimDataIdxs[Chara_Count];

extern s32 D_800A9EB0;

extern s_FsImageDesc D_800A9EB4;

extern s_FsImageDesc D_800A9EBC;

extern s_FsImageDesc D_800A9EC4;

extern s32 D_800A9938;

extern u8 D_800A9944[];

/** Player anim info? */
extern s_AnimInfo D_800A998C;

/** Current player anim index store? */
extern u8 D_800A9990;

/** Relative file offset for map texture? */
extern s8 D_800A99B5;

extern void (*g_SysStateFuncs[])(void);

extern s32 D_800A9A68;

/** Counts the amount of times that demos has been play in the current game session. */
extern s8 g_Demo_ReproducedCount;

extern RECT D_800A9A6C; // `RECT<320, 256, 160, 240>`, only used in `SysState_Fmv_Update`?

extern s32 D_800A9ECC;

extern s32 D_800A9ED0;

extern s32 D_800A9ED4;

extern s32 D_800A9ED8;

extern s32 D_800A9EDC;

extern s32 D_800A9EE0;

extern s32 D_800A9EE4;

extern s32 D_800A9EE8;

extern s32 D_800A9EEC;

extern s32 D_800A9EF0;

extern s32 D_800A9EF4;

extern s32 D_800A9EF8;

extern s32 D_800A9EFC;

extern s32 D_800A9F00;

extern s32 D_800A9F04;

extern s32 D_800A9F08;

extern s32 D_800A9F0C;

extern s32 D_800A9F10;

extern s32 D_800A9F14;

extern s32 D_800A9F18;

extern s32 D_800A9F1C;

extern s32 D_800A9F20;

extern s32 D_800A9F28;

extern s32 D_800A9F2C;

extern s32 D_800A9F30;

extern s32 D_800A9F34;

extern s32 D_800A9F38;

extern s32 D_800A9F3C;

extern s32 D_800A9F40;

extern s32 D_800A9F44;

extern s32 D_800A9F48;

extern s32 D_800A9F4C;

extern s32 D_800A9F50;

extern s32 D_800A9F54;

extern s32 D_800A9F58;

extern s32 D_800A9F60;

extern s32 D_800A9F64;

extern s32 D_800A9F68;

extern s32 D_800A9F6C;

extern s32 D_800A9F74;

extern s32 D_800A9F78;

extern s32 D_800A9F7C;

extern s_MapEnvPresetIdxs D_800A9F80;

extern s_MapEnvPresetIdxs D_800A9F84;

extern s_MapEnvPresetIdxs D_800A9F88;

extern s_MapEnvPresetIdxs D_800A9F8C;

extern s_MapEnvPresetIdxs D_800A9F98;

extern u32 D_800A9FB0;

extern s32 D_800A9FB4[];

extern u8 g_Sd_SongsChannelsForLayers[41][16];

extern u8 D_800AE185;

extern u8 D_800AE186;

extern s8 __pad_bss_800BCD81[3];

extern q19_12 g_DeltaTimeCpy;

extern s32 __pad_bss_800BCD88[2];

extern s_EventData* g_ItemTriggerEvents[];

extern s32 __pad_bss_800BCD94[5];

extern s_RadioNoise g_RadioNoise[2];

extern s_MapPoint2d g_MapPoint;

extern s32 __pad_bss_800BCDD0;

/** Related to special item interactions. */
extern s32 g_ItemTriggerItemIds[5];

extern u8 g_MapAreaLoadCounter;

extern s8 __pad_bss_800BCDD5[3];

extern s_EventData* g_MapEventData;

extern q3_12 g_Player_FlexRotationY;

extern q3_12 g_Player_FlexRotationX;

extern u8 g_Items_GunsMaxLoadAmmo[36]; // Max loaded ammo that a weapon can hold. 0x800AD4A0

extern const char* INV_ITEM_NAMES[];

extern const char* g_ItemDescriptions[];

extern s_FsImageDesc D_800A90A4;

extern s_FsImageDesc D_800A90B4;

extern s_SubCharacter D_800BA00C; // Often passed to `Los_NpcToPlayerHitCheck`, might not be full `s_SubCharacter`?

extern u8 D_800BC74F;

extern u16 D_800BCCB0;

extern u32 D_800A999C;

extern u16 D_800BCCB2;

extern s32 g_Bgm_LayersUpdated;

extern s32 D_800BCD84;

/** Related to main menu fog randomization. */
extern s8* D_800BCDE0; // Type assumed.

/** Angles. */
extern s16 D_800BCDE8[8];

extern u16 g_CollisionTriggerFlags;

extern s16 __pad_800BCE14;

extern s_WorldGfxWork g_WorldGfxWork;

extern s8* D_800C15B0;

extern s8 D_800C15B4;

extern s16 D_800C16A4;

extern s16 D_800C391E;

extern s8 D_800C39A0;

/** Functions from `bodyprog_8003AB28` access `g_WorldEnvWork` as constant, but some functions from `bodyprog_80055028` write to it.
 * It appears that g_WorldEnvWork is intended to be defined inside `bodyprog_80055028` as writable and declared as read-only (`const`) outside of it.
 */
// extern s_WorldEnvWork g_WorldEnvWork;

extern GsCOORDINATE2* g_ViewCoord;

extern q4_12 g_Player_RotationDeltaToTargetY;
extern q4_12 g_Player_RotationDeltaToTargetX;

extern s_800C42E8 D_800C42E8[24];

extern s16 D_800C4408;

extern s16 __pad_bss_800C440A;

extern GsCOORDINATE2* D_800C440C;

extern GsCOORDINATE2* D_800C4410;

/** Flags. */
extern s8 D_800C4414;

extern s8 __pad_bss_800C4415[3];

extern s_800C4418 D_800C4418;

extern q3_12 D_800C4454;

extern s8 D_800C447A;

extern s16 D_800AEEDC[][2]; // Type assumed.

extern s16 D_800AEF78[][2]; //  Type assumed.

extern s16 D_800AF014[];

extern s16 D_800AF04C[];

extern q3_12 D_800AF070[];

extern q3_12 D_800AF1FC[];

extern u8 g_Player_AnimResetRequest; /** `bool` */

extern s_CollisionSurface D_800C4620;

/** Q19.12 */
extern VECTOR3 D_800C4788[];

extern VECTOR3 D_800C47B8;

extern VECTOR3 D_800C47C8[];

extern VECTOR3 D_800C47E8;

extern s_RayTrace D_800C47F8;

extern s_800C4818 D_800C4818;

extern s32 D_800C48F0;

extern u8 D_800AD480[24];

/** Weapon attacks. */
extern s_800AD4C8 D_800AD4C8[70];

extern const s_MapOverlayHdr g_MapOverlayHdr; // 0x800C957C

extern s16 SQRT[100];

// ==========
// FUNCTIONS
// ==========

/** @brief Draws a palette of colors in the frame buffer. */
void func_8003652C(void);

/** @brief @unused Loading screen text.
 *
 * Just like in earlier builds of Silent Hill 2, the game
 * was intended to show a black screen with
 * "Now loading" text in middle of the screen.
 */
void func_80032CE8(void);

void func_80032D1C(void);

void func_8003943C(void);

/** `SysState_Fmv` update function.
 * Movie to play is decided by `2072 - g_MapEventParam`
 * After playback, savegame gets `D_800BCDD8->eventFlagNum_2` event flag set. */
void SysState_Fmv_Update(void);

s32 Map_TypeGet(void);

void Collision_FlagsLocationUpdate(const s_SubCharacter* chara);

/** @brief Frees a character model.
 *
 * @param model Character model to release.
 */
void Chara_ModelFree(s_CharaModel* model);

void WorldGfx_MapInit(s_MapOverlayHdr* mapHdr, s32 playerPosX, s32 playerPosZ);

/** @brief Loads map chunks in view and checks if they are loaded.
 *
 * @param `true` if chunks in view are loaded, `false` otherwise.
 */
bool WorldGfx_ChunkInitCheck(void);

/** @brief Draws world objects, chunks, and 2D screen effects.
 *
 * @param otShift Ordering table shift.
 */
void WorldGfx_Draw(s32 otShift);

/** @brief Sets the name of a world object model.
 *
 * @param model World object model to update.
 * @param name Name to set.
 */
void WorldObject_ModelNameSet(s_WorldObjectModel* model, char* name);

/** Submits a world object model to draw. */
void WorldObjects_Add(s_WorldObjectModel* model, const VECTOR3* pos, const SVECTOR3* rot);

/** @brief @unused Returns held item ID.
 *
 * @return Held item ID.
 */
s32 WorldGfx_HeldItemIdGet(void);

s32 WorldGfx_PlayerPrevHeldItem(s_PlayerCombat* combat);

/** @brief Loads and sets an item held by the player.
 *
 * @param itemId ID of the held item to load (`e_InvItemId`).
 * @return Model or texture queue index.
 */
s32 WorldGfx_PlayerHeldItemSet(e_InvItemId itemId);

// Make a Harry bone invisible? TODO: Is bone 32 the light?
void func_8003D01C(void);

// Make a Harry bone visible? TODO: Is bone 32 the light?
void func_8003D03C(void);

/** Loads the model of an item held by the player? */
void WorldGfx_HeldItemDraw(void);

bool WorldGfx_IsCharaModelPresent(e_CharaId charaId);

/** @brief Sets the material for a character model.
 *
 * @param charaId ID of the character whose model to update.
 * @param blendMode Material blend mode to set (`e_BlendMode`).
 */
void WorldGfx_CharaModelMaterialSet(e_CharaId charaId, s32 blendMode);

/** @brief Makes a character transparent. */
void WorldGfx_CharaModelTransparentSet(e_CharaId charaId, bool isTransparent);

/** @brief Frees a registered character model.
 *
 * @param model Character model to release.
 */
void WorldGfx_CharaFree(s_CharaModel* model);

/** @brief Loads the Harry character model. */
void WorldGfx_HarryCharaLoad(void);

/** @brief Loads default characters in the map overlay.
 *
 * @param mapHdr Map header.
 * @return Character model queue index.
 */
s32 WorldGfx_MapInitCharaLoad(s_MapOverlayHdr* mapHdr);

void WorldGfx_CharaLmBufferAssign(s8 forceFree);

/** @brief Gets the Y position of a color palette in VRAM.
 *
 * @param charaId @unused Character ID.
 * @param paletteIdx Color palette index.
 * @return CLUT Y position in VRAM.
 */
s32 WorldGfx_CharaClutYGet(e_CharaId charaId, s32 paletteIdx);

/** @brief Handles a mesh swap for a character.
 *
 * @param charaId ID of the character on which to perform the mesh swap.
 * @param meshSwapStatus Packed mesh swap status. See `MESH_SWAP_STATUS`.
 */
void WorldGfx_CharaMeshSwap(e_CharaId charaId, s32 meshSwapStatus);

bool Chara_ModelLoadedCheck(e_CharaId charaId);

/** Related to the screen. Called by `WorldEnv_Init`. */
void func_80040BAC(void);

void WorldEnv_WorldLightTintParamSet(u8 lightPosR, u8 lightPosG, u8 lightPosB, u8 worldTintR, u8 worldTintG, u8 worldTintB);

void func_80041074(GsOT* ot, s32 arg1, SVECTOR* rot, const VECTOR3* pos);

void func_800410D8(VECTOR3* pos0, q19_12* azimuthAngle, q19_12* altitudeAngle, SVECTOR* rot, const VECTOR3* pos1);

/** @brief Computes the relative spherical rotation on the Y-axis from one direction to another,
 * derived from normalized world-space offsets.
 *
 * @param azimuthAngle Output azimuth angle (Y rotation).
 * @param altitudeAngle Output altitude angle (X rotation).
 * @param offsetFrom First world-space offset (Q19.12).
 * @param offsetTo Second world-space offset (Q19.12).
 */
void Math_RelativeRotationGet(q19_12* azimuthAngle, q19_12* altitudeAngle, const VECTOR* offsetFrom, const VECTOR* offsetTo);

void func_8004137C(VECTOR3* result, const VECTOR* offset0, const VECTOR* offset1, s32 screenDist);

void func_800414E0(GsOT* arg0, VECTOR3* arg1, s32 arg2, q19_12 angle0, q19_12 angle1);


/** @brief Initializes map data and chunks.
 *
 * @param lmHdr LM header.
 * @param ipdBuf IPD chunk buffer.
 * @param ipdBufSize IPD chunk buffer size.
 */
void WorldMap_Init(s_LmHeader* lmHdr, s_IpdHeader* ipdBuf, s32 ipdBufSize);

void WorldMap_GlobalLmInit(s_GlobalPlm* globalPlm, s_LmHeader* lmHdr);

void WorldMap_GlobalLmHeaderInit(s_LmHeader* lmHdr);

/** @brief Clears `queueIdx` in an array of chunks. */
void WorldMap_ChunkQueueClear(s_MapChunk* cellBufferPtr, s32 activeChunkCount);

void WorldMap_TexturesInit(void);

void WorldMap_CollisionDataReset(void);

/** @brief Places a chunk at given XZ chunk cell coordinates.
 *
 * @param ipdFileIdx Index of the IPD chunk file to place.
 * @param chunkX X chunk coordinate.
 * @param chunkZ Z chunk coordinate.
 */
void WorldMap_ChunkSet(s16 ipdFileIdx, s32 chunkX, s32 chunkZ);

void WorldMap_ActiveChunksClear(void);

void WorldMap_TexturesRefClear(void);

void WorldMap_Reset(void);

void WorldMap_GlobalLmReset(void);

s_Texture* WorldMap_ActiveTextureInfoGet(char* texName);

void WorldMap_InfoSet(char* mapTag, e_FsFile plmIdx, s32 activeChunkCount, bool isExterior,
                      e_FsFile ipdFileIdx, e_FsFile texFileIdx);

void WorldMap_ChunksClear(s_WorldMapWork* terrain, s32 activeCellCount);

/** @brief Locates all IPD files for a given map type.
 *
 * Example:
 * Map type THR.
 * `file 1100` is `THR0205.IPD`, `chunksGridCenter[2][5] = 1100`.
 */
void WorldMap_MakeGrid(s_WorldMapWork* terrain, char* mapTag, e_FsFile fileIdxStart);

/** @brief Converts two hex `char`s to an integer hex value.
 *
 * @param out Output hex `int`.
 * @param hex0 First hex `char`.
 * @param hex1 Second hex `char`.
 */
bool ConvertHexToS8(s32* out, char hex0, char hex1);

s_IpdCollisionData** WorldMap_ActiveChunksCollisionDataGet(s32* collDataIdx);

/** @brief Gets the collision data at a given position.
 *
 * @param posX X position.
 * @param posZ Z position.
 * @return Collision data.
 */
s_IpdCollisionData* WorldMap_CollisionDataGet(q19_12 posX, q19_12 posZ);

/** @brief Gets the location of a nearby world object model.
 *
 * @param model Model to find.
 * @param metadata TODO
 * @param posX Search X position.
 * @param posZ Search Z position.
 * @return World object model location (`e_WorldModelLocation`).
 */
s32 WorldMap_ObjectModelLocationGet(s_WorldObjectModel* model, s_WorldObjectMetadata* metadata, q19_12 posX, q19_12 posZ);

/** @brief Checks if an IPD chunk file is loaded.
 *
 * @param activeChunkIdx Active chunk index.
 * @return `true` if the IPD chunk file is loaded, `false` otherwise.
 */
bool WorldMap_ActiveChunkLoadedCheck(s32 activeChunkIdx);

/** Starts the process of loading map geometry and assigns textures when the game is set in a loading screen mode? */
void WorldMap_ChunkInit(q19_12 curPosX, q19_12 curPosZ, q19_12 projPosX, q19_12 projPosZ);

/** @brief Computes the distance from an XZ position to the edge of an XZ chunk cell boundary.
 * For exteriors, the cell boundary is expanded by `Q12(1.0f)`.
 * If the position resides inside the chunk, the distance is `Q12(0.0f)`.
 *
 * @param posX X position.
 * @param posZ Z position.
 * @param chunkX X chunk coordinate.
 * @param chunkZ Z chunk coordinate.
 * @param isExterior `true` for padded exterior, `false` for non-padded interior.
 * @return Padded distance from the XZ position to the XZ chunk cell.
 */
q19_12 WorldMap_PaddedDistanceToChunkEdgeGet(q19_12 posX, q19_12 posZ, s32 chunkX, s32 chunkZ, bool isExterior);

/** @brief Computes the distance from an XZ position to the edge of an XZ chunk cell boundary.
 * If the position resides inside the chunk cell, the distance is `Q12(0.0f)`.
 *
 * @param posX X position.
 * @param posZ Z position.
 * @param chunkX X chunk coordinate.
 * @param chunkZ Z chunk coordinate.
 * @return Distance from the XZ position to the XZ chunk cell boundary.
 */
q19_12 WorldMap_DistanceToChunkEdgeGet(q19_12 posX, q19_12 posZ, s32 chunkX, s32 chunkZ);

/** Loads geometry, sets materials and properly assigns the position of the map when loading a new room/map? */
s32 WorldMap_ChunkLoad(s_WorldMapWork* terrain, q19_12 posX0, q19_12 posZ0, q19_12 posX1, q19_12 posZ);

void WorldMap_ActiveChunksSample(s_WorldMapWork* terrain, q19_12 posX0, q19_12 posZ0, q19_12 posX1, q19_12 posZ1,
                                 bool isExterior);

/** @brief Computes the distance from a position to the nearest edge of a chunk.
 *
 * @param chunk Chunk to check.
 * @param TODO
 */
void WorldMap_DistanceToEdgeCalc(s_MapChunk* chunk, q19_12 posX0, q19_12 posZ0, q19_12 posX1, q19_12 posZ1,
                                 bool isExterior);

/** Sets materials for active chunks? */
void WorldMap_ChunkMaterialsApply(s_WorldMapWork* terrain);

/** @brief Gets the IPD chunk file index from cell coordinates.
 *
 * @param chunkX X chunk coordinate.
 * @param chunkZ Z chunk coordinate.
 * @return IPD chunk file index.
 */
s32 WorldMap_IpdChunkFileIdxGet(s32 chunkX, s32 chunkZ);

bool WorldMap_IsChunkPresentCheck(const s_MapChunk* activeChunks, s32 chunkX, s32 chunkZ);

s_MapChunk* WorldMap_FreeChunkSpaceFind(s_MapChunk* activeChunks, bool isExterior);

s32 WorldMap_ChunkLoadStart(s_MapChunk* chunk, e_FsFile fileIdx, s32 chunkX, s32 chunkZ, q19_12 posX0, q19_12 posZ0, q19_12 posX1, q19_12 posZ1, bool isExterior);

/** @brief Checks if all base map data (IPD and global LM models) have been loaded properly. */
bool WorldMap_ActiveModelsLoadStateCheck(void);

/** @brief Checks if the player is in close proximity to an unloaded ahead which should be loaded.
 *
 * @return `true` if a new chunk needs to be loaded, `false` otherwise.
 */
bool WorldMap_NextChunkLoadCheck(void);

/** Checks if a position is within the current map chunk. */
bool WorldMap_CloseChunkEdgeCheck(q19_12 posX, q19_12 posZ);

void WorldMap_ChunksDraw(GsOT* ot, s32 otShift);

bool WorldMap_ChunkPositionMatchCheck(s_MapChunk* chunk, s_WorldMapWork* terrain);

/** Checks if PLM texture is loaded? */
bool WorldMap_TextureLoadedCheck(s_IpdHeader* ipdHdr);

s_IpdCollisionData* WorldMap_HeaderCollisionDataGet(s_IpdHeader* ipdHdr);

void WorldMap_ChunkPropertiesSet(s_IpdHeader* ipdHdr, s_LmHeader** lmHdrs, s32 lmHdrCount,
                                 s_ActiveChunkTextures* fullPageActiveTexs, s_ActiveChunkTextures* halfPageActiveTexs,
                                 e_FsFile fileIdx);

void WorldMap_ChunkMaterialsLoad(s_IpdHeader* ipdHdr,
                                 s_ActiveChunkTextures* fullPageActiveTexs, s_ActiveChunkTextures* halfPageActiveTexs,
                                 e_FsFile fileIdx);

/** Checks if IPD is loaded before returning texture count? */
s32 WorldMap_ChunkHalfPageMaterialCountGet(s_IpdHeader* ipdHdr);

/** Returns inverse result of `LmFilter_IsHalfPage`. */
bool LmFilter_IsFullPage(s_Material* mat);

bool LmFilter_IsHalfPage(s_Material* mat);

void WorldMap_ChunkHeaderPtrsInit(s_IpdHeader* ipdHdr);

/** @brief Assigns `s_ModelHeader` pointers to models in `s_IpdHeader` by searching the given `s_LmHeader` array. */
void WorldMap_ModelLinkObjectLists(s_IpdHeader* ipdHdr, s_LmHeader** lmHdrs, s32 lmHdrCount);

/** @brief Searches `s_LmHeader` for objects with the given `objName`. */
s_ModelHeader* LmHeader_ModelHeaderSearch(u_Filename* modelName, s_LmHeader* lmHdr);

/** @brief Assigns `s_ModelHeader` pointers to each `s_IpdModelBuffer` in `s_IpdHeader`. */
void WorldMap_ModelBufferLinkObjectLists(s_IpdHeader* ipdHdr, s_IpdModelInfo* ipdModelInfos);

/** Sets IPD collision data chunks? */
void func_80044044(s_IpdHeader* ipd, s32 chunkX, s32 chunkZ);

/** @brief Draws an IPD chunk.
 *
 * @param ipdHdr IPD chunk data to draw.
 * @param posX X world position.
 * @param posZ Z world position.
 * @param ot Ordering table.
 * @param otShift Ordering table shift.
 */
void WorldMap_Draw(s_IpdHeader* ipdHdr, q19_12 posX, q19_12 posZ, GsOT* ot, s32 otShift);

/** @brief Checks if an IPD chunk subcell is visible.
 *
 * @param modelBuf IPD model buffer.
 * @param subcellX X subcell position.
 * @param subcellZ Z subcell position.
 * @param posX X cell position? TODO: Subcell/cell intent unclear, needs more research.
 */
bool WorldMap_SubcellVisibleCheck(s_IpdModelBuffer* modelBuf, q7_8 subcellX, q7_8 subcellZ, q23_8 posX, q23_8 posZ);

void Anim_BoneInit(s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

void Anim_BoneUpdate(s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords, s32 keyframe0, s32 keyframe1, q19_12 alpha);

s_AnimInfo* func_80044918(s_ModelAnim* anim);

void func_80044950(s_SubCharacter* chara, s_AnmHeader* anmHdr, GsCOORDINATE2* coords);

/** @brief Gets the duration of an animation. A duration can be constant or variable.
 *
 * @param unused @unused
 * @param animInfo Animation info from which to get the duration.
 * @return Animation duration.
 */
q19_12 Anim_DurationGet(s_Model* unused, s_AnimInfo* animInfo);

/** * @brief Plays an animation once and clamps to the end keyframe, triggering a status transition to the linked
 * animation upon completion.
 *
 * @note Used for one-shot actions such as stopping and attacking.
 *
 * @param model Character model to animate.
 * @param anmHdr Character animation data.
 * @param boneCoords Character model bone coords.
 * @param animInfo Active character animation info.
 */
void Anim_PlaybackOnce(s_Model* model, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords, s_AnimInfo* animInfo);

/** * @brief Loops an animation continuously, wrapping the time around to the start and end keyframes without
 * triggering a status transition.
 *
 * @note Used for looped actions such as running and walking.
 *
 * @param model Character model to animate.
 * @param anmHdr Character animation data.
 * @param boneCoords Character model bone coords.
 * @param animInfo Character animation info.
 */
void Anim_PlaybackLoop(s_Model* model, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords, s_AnimInfo* animInfo);

/** * @brief Linearly interpolates between the current pose into a new target animation, triggering a status transition
 * to the linked animation when complete.
 *
 * @note Used as the entry transition for almost every new animation status.
 *
 * @param model Character model to animate.
 * @param anmHdr Character animation data.
 * @param boneCoords Character model bone coords.
 * @param animInfo Character animation info.
 */
void Anim_BlendLinear(s_Model* model, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords, s_AnimInfo* animInfo);

/** * @brief Smoothly blends between the current pose into a new target animation using a sine-based ease-out
 * without triggering a status transition.
 *
 * @unused?
 *
 * @param model Character model to animate.
 * @param anmHdr Character animation data.
 * @param boneCoords Character model bone coords.
 * @param animInfo Character animation info.
 */
void Anim_BlendEaseOut(s_Model* model, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords, s_AnimInfo* animInfo);

/** Something related to player weapon position. Takes coords to arm bones. */
void func_80044F14(GsCOORDINATE2* coord, q3_12 rotZ, q3_12 rotX, q19_12 rotY);

s8 Bone_ModelIdxGet(s8* ptr, bool arg1);

/** Skeleton setup? Assigns bones pointer for the skeleton and resets fields. */
void Skeleton_Init(s_Skeleton* skel, s_BoneNode* boneHierarchy, u8 boneCount);

/** Clears skeleton bone flags/mask. Called by `Skeleton_Init`. */
void func_80045014(s_Skeleton* skel);

/** Anim func. Used in tandem with skeleton bone traversal. */
void func_8004506C(s_Skeleton* skel, s_LmHeader* lmHdr);

/** Anim func. */
void func_80045108(s_Skeleton* skel, s_LmHeader* lmHdr, s8* arg2, bool cond);

/** Anim func. */
void Skeleton_BoneModelAssign(s_Skeleton* skel, s_LmHeader* lmHdr, s8* arg2);

/** Anim func. Param names are rough. */
void func_80045258(s_BoneNode** boneOrd, s_BoneNode* boneHierarchy, s32 boneIdx, s_LmHeader* lmHdr);

/** Anim func. */
void func_800452EC(s_Skeleton* skel);

/** Anim func. Traverses skeleton bones for something. */
void func_80045360(s_Skeleton* skel, s8* arg1);

/** Anim func. Traverses skeleton bones to set flags/mask.
 * `cond` may actually be another `s_Skeleton` pointer.
 */
void func_800453E8(s_Skeleton* skel, bool cond);

/** Does something with skeleton bones. `arg0` is a struct pointer. */
void func_80045468(s_Skeleton* skel, s32* arg1, bool cond);

void func_80045534(s_Skeleton* skel, GsOT* ot, s32 otShift, GsCOORDINATE2* boneCoords, q3_12 arg4, u16 clutY, s_FsImageDesc* images);

/** `arg0` is probably a bit flag. */
void func_8004C564(u8 arg0, s8 weaponAttack);

// TODO: Can probably create status enums for the below funcs' return values to avoid magic,
// but other funcs using similar return patterns should be identified first if they exist.

void GameFs_UniqueItemModelLoad(u8 itemId);

void GameFs_MapItemsTextureLoad(s32 mapId);

/** @brief Attaches a weapon model to the player's hand.
 *
 * @param weaponAttack Packed weapon attack. See `WEAPON_ATTACK`.
 */
void Gfx_PlayerHeldItemAttach(u8 weaponAttack);

void func_8005487C(s32);

void func_80054A04(u8 arg0);

bool Gfx_PickupItemAnimate(u8 itemId);

/** @brief Computes the ammo needed to reload the equipped gun.
 *
 * @param currentAmmo Current amount of loaded ammunition.
 * @param availableAmmo Ammount of available ammunition.
 * @param gunIdx Index of the gun being reloaded ()`e_EquippedWeaponId`).
 */
void Items_AmmoReloadCompute(s32* currentAmmo, s32* availableAmmo, u8 gunIdx); // 0x80054FC0

void WorldEnv_Init(void);

/** @brief Draws 2D screen effects for the flashlight's lens flare and glow. */
void WorldGfx_2dEffectsDraw(void);

/** Sets visual world parameters. */
void WorldEnv_WorldLightingParamSet(u8 arg0, s32 arg1, u8 arg2, s32 tintR, s32 tintG, s32 tintB, q23_8 brightness);

void WorldEnv_FogParamsSet(u8 isFogEnabled, u8 fogColorR, u8 fogColorG, u8 fogColorB);

void WorldEnv_WorldLightTintSet(u32 overlapLighting, u8 lightPosR, u8 lightPosG, u8 lightPosB, u8 worldTintR, u8 worldTintG, u8 worldTintB);

/** @brief Gets the point light position.
 *
 * @param pos Output light position.
 */
void WorldEnv_LightPositionGet(VECTOR3* pos);

/** @brief Gets the point light rotation and intensity.
 *
 * @param rot Output light rotation.
 * @return Light intensity.
 */
s32 WorldEnv_LightRotationAndIntensityGet(SVECTOR* rot);

/** @brief Gets the point light direction and intensity.
 *
 * @param rot Output light direction.
 * @return Light intensity.
 */
s32 WorldEnv_LightDirectionAndIntensityGet(SVECTOR* dir);

/** Light function. */
void Gfx_FlashlightPositionUpdate(q19_12 lightIntensity, q3_12 lensFlareIntensity, GsCOORDINATE2* coord0, GsCOORDINATE2* coord1,
                                  SVECTOR* rot, q19_12 posX, q19_12 posY, q19_12 posZ, s_WaterZone* waterZones);

/** Light function. */
void func_80055648(s32 lightIntensity, const SVECTOR* dir);

s32 func_800557DC(void);

void func_80055814(s32 arg0);

void WorldEnv_FogDistanceSet(q19_12 nearDist, q19_12 farDist);

s32 func_800559A8(s32 arg0);

u8 func_80055A50(s32 arg0);

void func_80055A90(CVECTOR* arg0, CVECTOR* arg1, u8 arg2, s32 arg3);

/** @brief Applies uniform lighting and fog shading to `color`, outputting to `result`.
 * The fog factor is derived from `arg2` via `g_WorldEnvWork.fogRamp` and blends the lit color toward the far/fog color in `g_WorldEnvWork.field_1C/field_1D/field_1E`,
 * with lighting strength from `g_WorldEnvWork.field_20` applied equally on all axes.
 */
void func_80055B74(CVECTOR* result, CVECTOR* color, s32 arg2);

void func_80055C3C(CVECTOR* result, CVECTOR* color, s32 arg2, s32 arg3, s32 arg4, s32 arg5);

u8 func_80055D78(q19_12 posX, q19_12 posY, q19_12 posZ);

void func_80055E90(CVECTOR* color, u8 fadeAmount);

void func_80055ECC(CVECTOR* color, SVECTOR3* arg1, SVECTOR3* arg2, MATRIX* worldMat);

u8 func_80055F08(SVECTOR3* arg0, SVECTOR3* arg1, MATRIX* worldMat);

void Lm_HeaderPtrsInit(s_LmHeader* lmHdr);

void ModelHeader_FixOffsets(s_ModelHeader* modelHdr, s_LmHeader* lmHdr);

void Lm_TransparentPrimSet(s_LmHeader* lmHdr, bool isTransparent);

s32 Lm_MaterialCountGet(bool (*filterFunc)(s_Material* mat), s_LmHeader* lmHdr);

/** TODO: Unknown `arg3` type. */
void func_80059D50(s32 arg0, s_ModelInfo* modelInfo, MATRIX* viewMat, s32 otShift, GsOT_TAG* tag);

void func_80059E34(u32 arg0, s_MeshHeader* meshHdr, s_GteScratchData* scratchData, s32 otShift, GsOT_TAG* tag);

void func_8005A21C(s_ModelInfo* modelInfo, GsOT_TAG* otTag, s32 otShift, MATRIX* viewMat);

/** @brief Computes a fog-shaded version of `D_800C4190` color using `arg1` as the distance factor?
 *  Stores the result at 0x3D8 into `arg0`.
 */
void func_8005A42C(s_GteScratchData* scratchData, q19_12 alpha);

void func_8005A478(s_GteScratchData* scratchData, q19_12 alpha);

/** `scratchData` is unused. */
void func_8005A838(s_GteScratchData* scratchData, q19_12 scale);

void func_8005A900(s_MeshHeader* meshHdr, s32 offset, s_GteScratchData* scratchData, MATRIX* viewMat);

u8 func_8005AA08(s_MeshHeader* meshHdr, s32 arg1, s_GteScratchData2* scratchData);

void func_8005AC50(s_MeshHeader* meshHdr, s_GteScratchData2* scratchData, GsOT_TAG* ot, bool otShift);

void Texture_Init(s_Texture* tex, char* texName, u8 tPage0, u8 tPage1, s32 u, s32 v, s16 clutX, s16 clutY);

void Texture_RefCountReset(s_Texture* tex);

/** @unused */
void func_8005B378(s_Texture* tex, char* arg1);

void Texture_RefClear(s_Texture* tex);

void Material_TimFileNameGet(char* filename, s_Material* mat);

void func_8005B424(VECTOR3* vec0, const VECTOR3* vec1);

/** @unused No references. */
void func_800563E8(s_LmHeader* lmHdr, s32 arg1, s32 arg2, s32 arg3);

void Lm_MaterialFileIdxApply(s_LmHeader* lmHdr, e_FsFile fileIdx, s_FsImageDesc* image, s32 blendMode);

void Lm_MaterialFsImageApply1(s_LmHeader* lmHdr, char* newStr, s_FsImageDesc* image, s32 blendMode);

bool Lm_MaterialFsImageApply(s_LmHeader* lmHdr, char* fileName, s_FsImageDesc* image, s32 blendMode);

void Material_FsImageApply(s_Material* mat, s_FsImageDesc* image, s32 blendMode);

/** @brief @unused Sets a model's material blending mode. */
void Lm_MaterialBlendModeSet(s_LmHeader* lmHdr, s_FsImageDesc* images, s8 unused, s32 startIdx, s32 blendMode);

void Lm_MaterialsLoadWithFilter(s_LmHeader* lmHdr, s_ActiveChunkTextures* activeTexs, bool (*filterFunc)(s_Material* mat), e_FsFile fileIdx, s32 blendMode);

/** Checks if LM textures are loaded? */
bool Lm_IsTextureLoaded(s_LmHeader* lmHdr);

void Lm_MaterialFlagsApply(s_LmHeader* lmHdr);

void Model_MaterialFlagsApply(s_ModelHeader* modelHdr, s32 matIdx, const s_Material* mat, s32 matFlags);

void Lm_MaterialRefCountDec(s_LmHeader* lmHdr);

s32 LmHeader_ModelCountGet(s_LmHeader* lmHdr);

void Bone_ModelAssign(s_Bone* bone, s_LmHeader* lmHdr, s32 modelHdrIdx);

bool Lm_ModelFind(s_WorldObjectModel* model, s_LmHeader* lmHdr, s_WorldObjectMetadata* metadata);

void StringCopy(char* prevStr, char* newStr);

/** @brief Draws a 2D fog overlay quad. */
void Gfx_FogOverlayQuadDraw(s16 arg0, s16 arg1, s16 arg2, s16 arg3, s32 arg4, s32 arg5, GsOT* ot, s32 arg7);

/** Crucial 3D drawing function. */
void func_80057090(s_ModelInfo* modelInfo, GsOT* otTag, s32 otShift, MATRIX* viewMat, MATRIX* worldMat, u16 clutY);

s32 func_800571D0(u32 arg0);

void WorldEnv_LightTransform(MATRIX* worldMat, q19_12 alpha, SVECTOR* arg2, VECTOR3* arg3);

void func_80057344(s_ModelInfo* modelInfo, GsOT_TAG* otTag, s32 otShift, MATRIX* mat);

void func_800574D4(s_MeshHeader* meshHdr, s_GteScratchData* scratchData);

void func_8005759C(s_MeshHeader* meshHdr, s_GteScratchData* scratchData, s32 vertOffset, s32 normalOffset);

void func_80057658(s_MeshHeader* meshHdr, s32 offset, s_GteScratchData* scratchData, SVECTOR3* arg3, SVECTOR* arg4);

void func_80057A3C(s_MeshHeader* meshHdr, s32 offset, s_GteScratchData* scratchData, SVECTOR3* lightVec);

void func_80057B7C(s_MeshHeader* meshHdr, s32 offset, s_GteScratchData* scratchData, MATRIX* mat);

/** Main quad drawing func? */
void Gfx_MeshDraw(s_MeshHeader* meshHdr, s_GteScratchData* scratchData, GsOT_TAG* tag, s32 arg3);

/** `arg4` unused. */
s_Texture* Texture_Get(s_Material* mat, s_ActiveChunkTextures* activeTexs, void* fsBuf9, e_FsFile fileIdx, s32 arg4);

/** Initializes values in `D_800AE204` array. */
void func_8005B55C(GsCOORDINATE2* viewCoord);

void Gfx_BillboardDraw(s32 idx, q19_12 posX, q19_12 posY, q19_12 posZ, GsOT* ot_arg4, s32 otShift);

u32 func_8005C478(s16* arg0, q19_12 x0, q19_12 y0, q19_12 x1, q19_12 y1, q19_12 x2, q19_12 y2);

s32 Chara_NpcIdxGet(s_SubCharacter* chara);

void Chara_CollisionShapeOffsetsUpdate(s_CharaShapeOffsets* offsets, s_SubCharacter* chara);

s32 Chara_MovementUpdate0(s_SubCharacter* chara, s_CollisionResult* collResult);

/** Checks `groundType`. */
bool func_8005F680(s_CollisionSurface* surface);

/** Related to camera collision? */
void func_8005F6B0(s_SubCharacter* chara, VECTOR* pos, s32 arg2, s32 arg3);

void Map_EffectTexturesLoad(s32 mapIdx);

void func_8005E414(s32 orgIdx);

/** @brief Does something with `unkTable1_4C`, clears blood splats and initialize drawn of objects. */
void func_8005E650(s32 mapId);

void func_8005E70C(void);

s32 func_8005E7E0(s32 arg0);

// Crucial for displaying particle effects. (Not general enviroments. Blood, shoot effects)
void func_8005E89C(void);

s32 func_8005F55C(s32 arg0);

void func_8008D41C(void);

void func_8008D438(void);

void func_8008D448(void);

void func_8008D454(void);

void func_8008D464(void);

/** Light function. */
void func_8008D470(q3_12 lensFlareIntensity, SVECTOR* rot, VECTOR3* pos, s_WaterZone* waterZones);

void func_8008D5A0(VECTOR3* arg0, s16 arg1);

/** Light function. */
q19_12 func_8008D8C0(q3_12 lensFlareIntensity, q19_12 x1, q19_12 x2);

void func_8008D990(s32 arg0, q19_12 arg1, VECTOR3* arg2, s32 arg3, s32 arg4);

/** @brief Gets the water zone at a given position.
 *
 * @param posX X position.
 * @param posZ Z position.
 * @param waterZones Water zones to query.
 * @return Water zone at the given position.
 */
s_WaterZone* Map_WaterZoneGet(q27_4 posX, q27_4 posZ, s_WaterZone* waterZones);

void func_8008E5B4(void);

void func_8008E794(VECTOR3* posXz, q3_12 angle, q19_12 posY);

void func_8008EA68(SVECTOR* arg0, VECTOR3* posXz, q19_12 posY);

// ========================

void Game_MapRoomIdxUpdate(void);

/** @unused */
s32 func_8003647C(void);

/** @unused */
s32 func_80036498(void);

// Used in some RoomBgmInit funcs.
u32 func_800364BC(void);

void Gfx_CursorDraw(s32 x0, s16 y0, s32 x1, s16 y1, s16 arg4, s16 arg5, s16 arg6, s32 arg7, s32 arg8, u32 arg9, s16 argA, s32 argB);

/** Might retrun `bool`. */
void func_80089090(s32 arg0);

void func_800890B8(void);

s32 func_80089128(void);

void func_800892A4(s32 idx);

void func_800892DC(s32 idx, u8 arg1);

void func_80089314(s32 arg0);

void func_800893D0(q19_12 arg0);

void func_8008944C(void);

void func_80089470(void);

void func_80089494(void);

void func_800894B8(s32 arg0);

void func_800894DC(void);

void func_80089500(void);

/** Handles initial gamepad init. */
bool func_80089524(s_SysWork_2514* arg0, s32 padInfoMode);

s32 func_800895E4(s_SysWork_2514* arg0, s_8002AC04* arg1, u8 arg2);

bool func_80089644(s_SysWork_2514* arg0, s_func_8009ECCC* arg1, s32 arg2, u32 arg3);

bool func_80089A30(s_SysWork_2514* arg0, s_func_8009ECCC* arg1, s_8002AC04* arg2, u32* arg3);

bool func_80089BB8(s_SysWork_2514* arg0, s_func_8009ECCC* arg1, s_8002AC04* arg2, u32* arg3);

bool func_80089DF0(s_SysWork_2514* arg0, s_func_8009ECCC* arg1, s_8002AC04* arg2, u32* arg3);

bool func_8008973C(s_SysWork_2514* arg0, s_func_8009ECCC* arg1, s_8002AC04* ptr, u32* arg3);

void func_80089840(s_SysWork_2514* arg0);

void func_8008989C(s_SysWork_2514* arg0, u16 arg1, s32 (*arg2)(u16, s32));

void func_8008992C(s_SysWork_2514* arg0, u16 arg1, s32 (*arg2)(u16, s32));

void func_800899BC(s_SysWork_2514* arg0, s32 arg1);

bool func_80089D0C(s_SysWork_2514* arg0, s_func_8009ECCC* arg1, s_8002AC04* arg2, u32* arg3);

// Q12 square root?
u32 func_8008A058(s32 arg0);

/** @brief @unused Returns 0. */
s32 func_8008A0CC(void);

/** @brief @unused Returns 0. */
s64 func_8008A0D4(void);

/** Anim infos setup for character. */
s32 func_8008A0E4(s32 arg0, s32 weaponAttack, s_SubCharacter* chara, VECTOR3* attackPos, s_SubCharacter* unused, q3_12 angle0, q3_12 angle1);

u32 func_8008A270(s32 idx);

s32 func_8008A35C(s_800AD4C8* arg0, s32 arg1);

void Chara_Flag8Clear(s_SubCharacter* chara);

void Chara_Flag8Set(s_SubCharacter* chara);

/** Large function. Runs if `CharaFlag_Unk8` is set. */
void func_8008A3AC(s_SubCharacter* chara);

s32 func_8008A3E0(s_SubCharacter* chara);

void func_8008B15C(s_SubCharacter* chara);

void func_8008B1DC(s_SubCharacter* chara, q19_12 angle0, q19_12 angle1);

void func_8008B398(void);

void func_8008B3E4(q23_8 vol);

void func_8008B40C(q23_8 vol, s32 soundType);

/** `arg0` is boolean. */
void func_8008B438(s32 arg0, q23_8 vol, s32 soundType);

bool func_8008B474(s32 arg0, q23_8 vol, s32 soundType);

void func_8008B664(VECTOR3* pos, u32 caseVar);

/** Main attack handling function. */
s32 func_8008B714(s_SubCharacter* attacker, s_SubCharacter* target, VECTOR3* attackPos, s32 arg3);

s32 func_8008BF84(s_SubCharacter* chara, q19_12 angle, s_800AD4C8* arg2, s32 arg3);

s32 func_8008D850(void);

// Sets material flags?
void func_8008E4EC(s_LmHeader* lmHdr);

void func_8008D78C(void);

void func_80091380(void);

// TODO: Arrange these in address order for better insight into the original interface. -- Sezz
// ------------------------------------------------------------------

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

/** Loads `Tim00` graphic. */
void GameFs_Tim00TIMLoad(void);

void GameFs_MapItemsModelLoad(u32 mapId);

void Textures_ActiveTex_CountReset(s_ActiveChunkTextures* activeTexs);

void Textures_ActiveTex_PutTextures(s_ActiveChunkTextures* activeTexs, s_Texture* texs, s32 texIdx);

s_Texture* Textures_ActiveTex_FindTexture(char* texName, s_ActiveChunkTextures* activeTexs);

/** @brief Sets the debug string position.
 *
 * @param unused @unused
 * @param posX X screen position.
 * @param posY Y screen position.
 */
void Gfx_DebugStringPositionSet(s16 unused, s16 posX, s16 posY);

/** @brief Normalizes a signed Q3.12 fixed-point angle to the clamped signed integer range `[-2048, 2047]`.
 *
 * TODO: Appears to have the same effect as `Q12_ANGLE_NORM_S` macro. Maybe this was written by someone unaware of that?
 * Once the build is shiftable, check if calls to this can simply be switched over to the macro.
 *
 * @param angle Angle to normalize.
 * @return Normalized angle.
 */
q3_12 Math_AngleNormalizeSigned(q3_12 angle);

/** @unused */
bool func_8005BF58(s32* arg0, s32* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9);

/** @unused */
s32 func_8005C1CC(s32* arg0, s32* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);

q19_12 func_8005C7B0(q19_12 angle);

void func_800625F4(VECTOR3* arg0, q3_12 angle, s32 arg2, s32 arg3);

bool func_80062708(POLY_FT4** poly, s32 idx);

bool func_80063A50(POLY_FT4** poly, s32 idx);

// Important for drawing gun smoke effect.
bool func_80064334(POLY_FT4** poly, s32 idx);

/** Displays gunfire effects. */
void func_8006342C(s32 weaponAttack, q3_12 rotY, q3_12 rotX, GsCOORDINATE2* coord);

s32 Chara_MovementUpdate1(s_SubCharacter* chara, s_CollisionResult* collResult, q3_12 offsetX, q3_12 offsetZ);

/** Computes something for a targeted NPC. */
bool func_8005D50C(s32* targetNpcIdx, q3_12* rotToTargetX, q3_12* rotToTargetY, const VECTOR3* attackPos,
                   u32 npcIdx, q19_12 angleConstraint);

bool func_80060044(POLY_FT4** poly, s32 idx);

bool func_800611C0(POLY_FT4** poly, s32 idx);

/** Spawns blood effect? */
void func_800622B8(s32 unused, s_SubCharacter* chara, s32 animStatus, s32 arg3);

void func_80064F04(VECTOR3* arg0, s8 arg1, q3_12 arg2);

bool func_80064FC0(POLY_FT4** polys, s32 idx);

/** @unused */
void func_800652F4(VECTOR3* arg0, s16 arg1, s16 arg2, s16 arg3);

/** @unused */
void func_80065B94(VECTOR3* arg0, s16 arg1);

void func_80066184(void);

void func_80066D90(void);

void func_80066E40(void);

void func_80066E7C(void);

s32 func_80067914(s32 paperMapIdx, u16 arg1, u16 arg2, q4_12 scale);

bool func_80068CC0(s32 arg0);

/** Paper map func. Seems to draw map marking sprites. */
bool func_80068E0C(s32 arg0, s32 idx, s32 arg2, s32 shade, u16 arg4, u16 arg5, q4_12 scale);

void PaperMap_DrawScaled(u16 scrollX, u16 scrollY, q4_12 scale);

void Player_FlexRotationYReset(void);

void func_8004BBF4(VbRVIEW* arg0, GsCOORDINATE2* arg1, SVECTOR* arg2);

void Settings_ScreenAndVolUpdate(void);

void Settings_RestoreDefaults(void);

void Settings_RestoreControlDefaults(s32 configIdx);

void nullsub_800334C8(void);

/** @brief Updates the savegame buffer with current player info (position, rotation, health, event index). */
void SysWork_SavegameUpdatePlayer(void);

/** @brief Updates SysWork with player info from the savegame buffer (position, rotation, health). */
void SysWork_SavegameReadPlayer(void);

/** @brief Handles a warm game reboot. */
void Game_WarmBoot(void);

s32 func_800382B0(s32 arg0);

/** @brief Computes the distance between two positions.
 *
 * @param from First position (Q19.12).
 * @param to Second position (Q19.12).
 * @return Distance between positions.
 */
q19_12 Math_DistanceGet(const VECTOR3* from, const VECTOR3* to);

/** @brief Computes the 2D distance on the XZ plane between two positions.
 *
 * @param from First position (Q19.12).
 * @param to Second position (Q19.12).
 * @return 2D distance between positions.
 */
q19_12 Math_Distance2dGet(const VECTOR3* from, const VECTOR3* to);

void func_800803FC(VECTOR3* pos, s32 idx);

/** @brief Forces a clicked controller input status for `ControllerFlag_Select`. */
void Input_SelectClickSet(void);

void SysState_Gameplay_Update(void);

void SysState_GamePaused_Update(void);

void SysState_OptionsMenu_Update(void);

void func_8003943C(void);

void SysState_StatusMenu_Update(void);

void GameState_LoadStatusScreen_Update(void);

void SysState_MapScreen_Update(void);

void SysState_Fmv_Update(void);

void SysState_LoadArea_Update(void);

void AreaLoad_UpdatePlayerPosition(void);

/** @brief Plays door sound when transitioning areas.
 * Reduntant code. `SysState_LoadArea_Update` also performs this same code
 * in the exact way this function is doing it.
 */
void AreaLoad_TransitionSound(void);

s8 AreaLoad_TransitionFlags(void);

void SysState_ReadMessage_Update(void);

/** @brief Updates the active autosave savegame entry with the most recent savegame. */
void SysWork_AutosaveUpdate(void);

void SysState_SaveMenu_Update(void);

void SysState_EventCallback_Update(void);

void SysState_EventSetFlag_Update(void);

void SysState_EventPlaySound_Update(void);

void SysState_GameOver_Update(void);

// ====================
// Main menu functions - TODO: Maybe a split around here?
// ====================

void MainMenu_SelectedOptionIdxReset(void);

// ====================

/** @brief Gets the bone nodes of a registered character model.
 *
 * @param charaId ID of the character for which to get the skeleton bone nodes (`e_CharaId`).
 * @return Character model  bone nodes. `NULL` if the character model is unregistered.
 */
s_BoneNode* WorldGfx_CharaModelBoneNodesGet(e_CharaId charaId);

void GameFs_BgEtcGfxLoad(void);

void GameFs_BgItemLoad(void);

void GameFs_CommonItemsTextureLoad(void);

/** @brief Gets the speed zone type at a given position.
 *
 * @param posX X position.
 * @param posZ Z position.
 * @return Speed zone type at the given position (`e_SpeedZoneType`).
 */
s32 Map_SpeedZoneTypeGet(q19_12 posX, q19_12 posZ);

/** @brief Initializes world and clear any previously loaded object placement.
 */
void World_Init(void);

/** Something related to player model loading. */
void WorldGfx_HeldItemModelFree(void);

/** Allocates player model? */
void WorldGfx_CharaModelsFree(void);

void Chara_ModelFree(s_CharaModel* model);

/** @unused */
void Map_ActiveChunksClear(void);

void WorldGfx_MapReset(void);

void WorldGfx_IpdSamplePointStore(void);

void WorldGfx_IpdSamplePointReset(void);

/** Sets draw distance, and loads map models.
 * Breaking the function doesn't causes the game to crash, but instead
 * the world won't draw beyond what has been previously loaded. Some
 * circumstances can also cause the player to be unable to move.
 */
void WorldGfx_CloseRangeChunksInit(void);

/** @brief Resets the collection of world objects to draw.
 *
 * @param worldGfx World GFX workspace.
 */
void WorldObjects_Clear(s_WorldGfxWork* worldGfxWork);

/** @brief Draws all world objects submitted for the current tick.
 *
 * @param worldGfxWork World GFX workspace.
 */
void WorldObjects_DrawAllObjects(s_WorldGfxWork* worldGfxWork);

/** @brief Draws a world object, taking into account its position relative to the camera.
 *
 * @param obj World object.
 */
void WorldObjects_Draw(s_WorldObject* obj);

void WorldObjects_DrawStep(s_WorldObjectModel* model, MATRIX* viewMat, MATRIX* worldMat);

/** @brief Advanced a character model LM buffer.
 *
 * @param buf Buffer to advance.
 * @param charaID  ID of the character whose model to use (`e_CharaId`).
 */
void WorldGfx_CharaLmBufferAdvance(u8** buf, e_CharaId charaId);

/** UV setup for character textures. */
void Chara_FsImageCalc(s_FsImageDesc* image, s32 groupIds, s32 modelIdx);

/** @brief Loads a character model, setting the LM header and texture UVs as a prerequisite.
 *
 * @param charaId ID of the character whose model to load (`e_CharaId`).
 * @param modelIdx Slot index of the model array to load into.
 * @param lmHdr LM header.
 * @param tex Model texture.
 * @return Model or texture queue index.
 */
void WorldGfx_CharaLoad(e_CharaId charaId, s32 modeIdx, s_LmHeader* lmHdr, s_FsImageDesc* tex);

/** @brief Loads a character model.
 *
 * @param charaId ID of the character whose model to load (`e_CharaId`).
 * @param modelIdx Slot index of the model array to load into.
 * @param lmHdr LM header.
 * @param tex Model texture.
 * @return Model or texture queue index.
 */
s32 WorldGfx_CharaModelLoad(e_CharaId charaId, s32 modelIdx, s_LmHeader* lmHdr, s_FsImageDesc* tex);

/** Something related to animations. */
void WorldGfx_PlayerModelProcessLoad(void);

void WorldGfx_CharaModelProcessAllLoads(void);

void WorldGfx_CharaModelProcessLoad(s_CharaModel* model);

void WorldGfx_CharaDraw(e_CharaId charaId, GsCOORDINATE2* boneCoords, s32 otShift, q3_12 timer, s32 paletteIdx);

/** @brief Handles a mesh swap for a Harry character.
 *
 * @param skel Harry model skeleton.
 * @param meshSwapStatus Packed mesh swap status. See `MESH_SWAP_STATUS`.
 */
void WorldGfx_HarryMeshSwap(s_Skeleton* skel, s32 meshSwapStatus);

/** @brief Handles a mesh swap for a Cybil character.
 *
 * @param skel Cybil model skeleton.
 * @param meshSwapStatus Packed mesh swap status. See `MESH_SWAP_STATUS`.
 */
void WorldGfx_CybilMeshSwap(s_Skeleton* skel, s32 meshSwapStatus);

/** @brief Handles a mesh swap for a Monster Cybil character.
 *
 * @param skel Monster Cybil model skeleton.
 * @param meshSwapStatus Packed mesh swap status. See `MESH_SWAP_STATUS`.
 */
void WorldGfx_MonsterCybilMeshSwap(s_Skeleton* skel, s32 meshSwapStatus);

/** @brief Handles a mesh swap for a Dahlia character.
 *
 * @param skel Dahlia model skeleton.
 * @param meshSwapStatus Packed mesh swap status. See `MESH_SWAP_STATUS`.
 */
void WorldGfx_DahliaMeshSwap(s_Skeleton* skel, s32 meshSwapStatus);

/** @brief Handles a mesh swap for a Kaufmann character.
 *
 * @param skel Kaufmann model skeleton.
 * @param meshSwapStatus Packed mesh swap status. See `MESH_SWAP_STATUS`.
 */
void WorldGfx_KaufmannMeshSwap(s_Skeleton* skel, s32 meshSwapStatus);

/** @brief Handles a mesh swap for a Stalker character.
 *
 * @param skel Stalker model skeleton.
 * @param meshSwapStatus Packed mesh swap status. See `MESH_SWAP_STATUS`.
 */
void WorldGfx_StalkerMeshSwap(s_Skeleton* skel, s32 meshSwapStatus);

/** @brief Handles a mesh swap for a Split Head character.
 *
 * @param skel Split Head model skeleton.
 * @param meshSwapStatus Packed mesh swap status. See `MESH_SWAP_STATUS`.
 */
void WorldGfx_SplitHeadMeshSwap(s_Skeleton* skel, s32 meshSwapStatus);

/** @brief Handles a mesh swap for a Puppet Nurse character.
 *
 * @param skel Puppet Nurse model skeleton.
 * @param meshSwapStatus Packed mesh swap status. See `MESH_SWAP_STATUS`.
 */
void WorldGfx_PuppetNurseMeshSwap(s_Skeleton* skel, s32 meshSwapStatus);

/** @brief Handles a mesh swap for a Puppet Doctor character.
 *
 * @param skel Puppet Doctor model skeleton.
 * @param meshSwapStatus Packed mesh swap status. See `MESH_SWAP_STATUS`.
 */
void WorldGfx_PuppetDoctorMeshSwap(s_Skeleton* skel, s32 meshSwapStatus);

void func_8003ECBC(void);

void func_8003ECE4(void);

void func_8003EDA8(void);

/** Resets player info such as the inventory, health, and playtime in the savegame buffer. */
void Game_SavegameResetPlayer(void);

void Game_PlayerInfoInit(void);

void func_8007E860(void);

void func_8007E8C0(void);

/** @brief Clears NPC animation data from memory. */
void Anim_CharaTypeAnimInfoClear(void);

/** Loads player animations for a given map. Maybe for cutscenes?
 *
 * @param mapIdx Map index.
 */
void GameFs_PlayerMapAnimLoad(s32 mapIdx);

void func_80070B84(s_SubCharacter* player, q19_12 baseMoveSpeed, q19_12 topMoveSpeed, s32 keyframeIdx);

void func_80070DF0(s_PlayerExtra* extra, s_SubCharacter* player, s32 weaponAttack, s32 animStatus);

/** @brief Gets the variable animation duration for the player.
 *
 * @param model Player model.
 * @return Variable animation duration.
 */
q19_12 Player_VariableAnimDurationGet(s_Model* model);

/** Special player SFX handler for heavy breath and damage. */
bool func_80071620(u32 animStatus, s_SubCharacter* player, s32 keyframeIdx, e_SfxId sfxId);

void Player_PositionUpdate(s_SubCharacter* player, s_PlayerExtra* extra, GsCOORDINATE2* boneCoords);

void func_8007D090(s_SubCharacter* player, s_PlayerExtra* extra, GsCOORDINATE2* boneCoords);

void Player_CombatUpdate(s_SubCharacter* player, GsCOORDINATE2* boneCoords);

/** Player func. */
void func_8007E9C4(void);

/** Sound func. */
void func_8007F14C(u8 weaponAttack);

/** Resets several global variables to 0. */
void func_8007F1CC(void);

void Player_DisableDamage(u8* isPlayerDead, u8 disableDamage);

/** Some kind of player anim state check. */
bool Player_IsAttacking(void);

/** Some kind of player anim state check. */
bool Player_IsBusy(void);

/** Gets something from the player's current animation? */
s16 Player_AnimGetSomething(void);

/** @brief Computes the shortest angle between `angleFrom` and `angleTo`, outputting the result to `shortestAngle`.
 *
 * @param angleFrom First angle in Q3.12, range `[0, 4096]`.
 * @param angleTo Second angle in Q3.12, range `[0, 4096]`.
 * @param shortestAngle Shortest angle output in Q3.12, range `[0, 4096]`.
 */
void Math_ShortestAngleGet(q3_12 angleFrom, q3_12 angleTo, q3_12* shortestAngle);

/** Anim func.
 *
 * @param chara Player character.
 * @param extra Extra player character parameters.
 * @param animStatus Packed anim status.
 */
void func_8007FB94(s_SubCharacter* chara, s_PlayerExtra* extra, s32 animStatus);

/** Anim func.
 *
 * @param chara Player character.
 * @param extra Extra player character parameters.
 * @param animStatus Packed anim status.
 */
void func_8007FC48(s_SubCharacter* chara, s_PlayerExtra* extra, s32 animStatus);

/** Gets property 8 from player. */
s32 func_8007FD2C(void);

/** @unused Gets current value from the power timer for gas based weapons.
 *
 * @return Power timer.
 */
q19_12 Game_GasWeaponPowerTimerValue(void);

void func_8007FD4C(bool resetColl);

/** @brief Gets a player footstep SFX ID according to the ground type and its pitch.
 *
 * @param groundType Ground type.
 * @param sfxId Output footstep SFX ID.
 * @param pitch0 Output TODO
 * @param pitch1 Output TODO
 * @return Footstep SFX ID.
 */
void Player_FootstepSfxGet(s8 groundType, e_SfxId* sfxId, s8* pitch0, s8* pitch1);

/** Presumably, returns a heading angle from `from` to `to`. If so, rename to `Math_AngleBetweenPositionsGet`. */
q19_12 func_80080478(const VECTOR3* from, const VECTOR3* to);

/** @brief Generates a random Q12 fixed-point number in the range `[0.0f, 1.0f)`. Used for angles and multipliers.
 *
 * @return Random Q12 fixed-point number.
 */
q19_12 Rng_RandQ12(void);

// Math with XYZ values.
s32 func_80080540(q19_12 x, q19_12 y, q19_12 z);

/** Computes (abs(value) - subtractor) * copysign(value). */
s32 Math_PreservedSignSubtract(s32 val, s32 subtractor);

void func_800805BC(VECTOR3* pos, SVECTOR* rot, GsCOORDINATE2* rootCoord, s32 arg3);

bool func_800806AC(s32 arg0, s32 arg1, s32 arg2, s32 arg3); // arg3 type assumed.

/** Probably returns `bool`. */
bool func_8008074C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

/** Fills `g_CollisionPointCache` with collision data at a given 2D position.
 *
 * @param posX X position.
 * @param posZ Z position.
 */
void Collision_Fill(q19_12 posX, q19_12 posZ);

/** @brief Gets the ground height at a given 2D position.
 *
 * @param posX X position.
 * @param posZ Z position.
 * @return Ground height.
 */
q19_12 Collision_GroundHeightGet(q19_12 posX, q19_12 posZ);

/** Gets ground type from cached point collision. */
s32 func_800808AC(q19_12 posX, q19_12 posZ);

/** Returns a Q shift based on a magnitude. */
s32 Math_MagnitudeShiftGet(q19_12 mag);

u32 func_8008A2E0(s32 arg0);

/* Does the map zoom in, red lines? Argument types guessed based on f`unc_800E83C0` in MAP2_S00. */
void Map_BoxOutlineDraw(s16 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s16 arg5, s16 arg6, s16 arg7, s16 arg8);

// TODO: Move these to new headers.

void GameState_MainMenu_Update(void);
void GameState_InGame_Update(void);
void GameState_MapEvent_Update(void);
void GameState_PaperMapScreen_Update(void);
void GameState_LoadMapScreen_Update(void);

#endif
