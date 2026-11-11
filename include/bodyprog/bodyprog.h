#ifndef _BODYPROG_BODYPROG_H
#define _BODYPROG_BODYPROG_H

// TODO: Includes will have to be cleaned up once everything is organised project-wide and this header becomes
// deprecated.
#include "game.h"
#include "bodyprog/libkpad/vibration.h"
#include "bodyprog/sound/sfx.h"
#include "bodyprog/chara/anim.h"
#include "bodyprog/collision/collision.h"
#include "bodyprog/chara/model.h"
#include "bodyprog/chara/spawn.h"
#include "bodyprog/events/bgm_update.h"
#include "bodyprog/events/events_util.h"
#include "bodyprog/events/game_sys_states.h"
#include "bodyprog/events/title.h"
#include "bodyprog/formats/ipd.h"
#include "bodyprog/formats/lm.h"
#include "bodyprog/formats/model.h"
#include "bodyprog/formats/texture.h"
#include "bodyprog/formats/tmd.h"
#include "bodyprog/gfx/env_effects.h"
#include "bodyprog/gfx/water.h"
#include "bodyprog/gfx/world.h"
#include "bodyprog/gfx/world_object.h"
#include "bodyprog/map/map.h"
#include "bodyprog/map/terrain.h"
#include "bodyprog/paper_map/paper_map.h"
#include "bodyprog/player/player_control.h"
#include "bodyprog/screen/data.h"
#include "bodyprog/sound/sfx_id_enum.h"
#include "bodyprog/sys/map_info.h"
#include "bodyprog/text/map_msg.h"
#include "bodyprog/view/vw_system.h"
#include "bodyprog/world/draw.h"
#include "bodyprog/world/lighting.h"
#include "bodyprog/world/map.h"
#include "main/fsqueue.h"

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
    /* 0x3 */ s8 unused_3; /** @unused */
} s_RadioNoise;
STATIC_ASSERT_SIZEOF(s_RadioNoise, 4);

typedef struct
{
    /* 0x0 */ u8  unused_0[4];      /** @unused TODO: Looks meaningful, but unclear what it could be. */
    /* 0x4 */ s16 startKeyframeIdx;
    /* 0x6 */ s16 endKeyframeIdx;
} s_800C44F0;

typedef struct _WorldEnvPresetIdxs
{
    /* 0x0 */ u8 presetIdx0;
    /* 0x1 */ u8 presetIdx1;
} s_WorldEnvPresetIdxs;

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

// Something related to keyframes.
extern const s_800C44F0 D_800294F4[];

extern const s_AnimInfo* D_800297B8;

extern u_Filename D_8002B2CC;

extern s32 D_800A9A20;

/** X. */
extern s32 D_800A9A24;

/** Z. */
extern s32 D_800A9A28;

/** Loaded NPC types in memory and their `g_CharaModelAnimsData` indices. */
extern s8 g_CharaAnimDataIdxs[Chara_Count];

#if VERSION_REGION_IS(PAL)
    extern e_FsFile g_MapLanguageBaseIds[];
    extern e_FsFile g_ItemLanguageFileIds[];
#endif

extern s32 D_800A9EB0;

extern s_FsImageDesc D_800A9EB4;

extern s_FsImageDesc D_800A9EBC;

extern s_FsImageDesc D_800A9EC4;

/** Player anim info? */
extern s_AnimInfo D_800A998C;

/** Current player anim index store? */
extern u8 D_800A9990;

/** Relative file offset for map texture? */
extern s8 D_800A99B5;

extern void (*g_SysStateFuncs[])(void);

extern s32 D_800A9A68;

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

extern s_WorldEnvPresetIdxs D_800A9F80;

extern s_WorldEnvPresetIdxs D_800A9F84;

extern s_WorldEnvPresetIdxs D_800A9F88;

extern s_WorldEnvPresetIdxs D_800A9F8C;

extern s_WorldEnvPresetIdxs D_800A9F98;

extern u32 D_800A9FB0;

extern s32 D_800A9FB4[];

extern u8 g_Sd_SongsChannelsForLayers[41][16];

extern u8 D_800AE185;

extern u8 D_800AE186;

extern s8 __pad_bss_800BCD81[3];

extern q19_12 g_DeltaTimeCpy;

extern s32 __pad_bss_800BCD88[2];

extern s_EventData* g_ItemEvents[];

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

/** Angles. */
extern s16 D_800BCDE8[8];

extern u16 g_CollisionTriggerFlags;

extern s16 __pad_800BCE14;

extern s_WorldGfxWork g_WorldGfxWork;

extern s8* D_800C15B0;

extern s8 D_800C15B4;

extern s16 D_800C16A4;

extern s8 D_800C39A0;

/** Functions from `bodyprog_8003AB28` access `g_WorldEnvRenderWork` as constant, but some functions from `bodyprog_80055028` write to it.
 * It appears that g_WorldEnvRenderWork is intended to be defined inside `bodyprog_80055028` as writable and declared as read-only (`const`) outside of it.
 */
// extern s_WorldEnvRenderWork g_WorldEnvRenderWork;

extern GsCOORDINATE2* g_BillboardViewCoord;

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

extern s_CollisionSurface D_800C4620;

/** Q19.12 */
extern VECTOR3 D_800C4788[];

extern VECTOR3 D_800C47B8;

extern VECTOR3 D_800C47C8[];

extern VECTOR3 D_800C47E8;

extern s_RayTrace D_800C47F8;

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

/** @brief @unused Draws loading screen text.
 *
 * Just like in earlier builds of Silent Hill 2, the game was intended to show a black screen with "Now loading." text
 * in middle of the screen.
 */
void GameFs_LoadingTextDraw(void);

void func_80032D1C(void);

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
bool WorldGfx_ChunksInitCheck(void);

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

// TODO: Move to `bodyprog/chara/anim.h`.
// ========================================

void Anim_BonesInit(s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

void Anim_BonesUpdate(s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords, s32 keyframe0, s32 keyframe1, q19_12 alpha);

/** @brief Gets the active animation info of a model, accounting for overflow map animations.
 *
 * @param anim Model animation data.
 */
s_AnimInfo* Anim_InfoGet(s_ModelAnim* anim);

/** @brief Plays a character's animation using the callback function of its active animation.
 *
 * @param chara Character to update.
 * @param anmHdr Character animation data.
 * @param boneCoords Character model bone coords.
 */
void Anim_CharaUpdate(s_SubCharacter* chara, s_AnmHeader* anmHdr, GsCOORDINATE2* coords);

/** @brief Gets the duration of an animation. A duration can be constant or variable.
 *
 * @param unused @unused
 * @param animInfo Animation info from which to get the duration.
 * @return Animation duration.
 */
q19_12 Anim_DurationGet(s_Model* unused, s_AnimInfo* animInfo);

/** @brief Plays an animation once and clamps to the end keyframe, triggering a status transition to the linked
 * animation upon completion.
 *
 * @note Used for one-shot actions such as stopping and attacking.
 *
 * @param model Character model to animate.
 * @param anmHdr Character animation data.
 * @param boneCoords Character model bone coords.
 * @param animInfo Active character animation info.
 */
void Anim_PlayOnce(s_Model* model, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords, s_AnimInfo* animInfo);

/** @brief Loops an animation continuously, wrapping the time around to the start and end keyframes without
 * triggering a status transition.
 *
 * @note Used for looped actions such as running and walking.
 *
 * @param model Character model to animate.
 * @param anmHdr Character animation data.
 * @param boneCoords Character model bone coords.
 * @param animInfo Character animation info.
 */
void Anim_PlayLoop(s_Model* model, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords, s_AnimInfo* animInfo);

/** @brief Linearly interpolates between the current pose into a new target animation, triggering a status transition
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

/** @brief Smoothly blends between the current pose into a new target animation using a sine-based ease-out
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

// =========================

/** `flags` might not be a bitfield. */
void func_8004C564(u8 flags, s8 weaponAttack);

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

void WorldEnvRender_Init(void);

/** @brief Draws 2D screen effects for the flashlight's lens flare and glow. */
void WorldGfx_2dEffectsDraw(void);

/** Sets visual world parameters. */
void WorldEnvRender_WorldLightingParamSet(u8 arg0, s32 arg1, u8 arg2, s32 tintR, s32 tintG, s32 tintB, q23_8 brightness);

void WorldEnvRender_FogParamsSet(u8 isFogEnabled, u8 fogColorR, u8 fogColorG, u8 fogColorB);

void WorldEnvRender_WorldLightTintSet(u32 overlapLighting, u8 lightPosR, u8 lightPosG, u8 lightPosB, u8 worldTintR, u8 worldTintG, u8 worldTintB);

/** @brief Gets the point light position.
 *
 * @param pos Output light position.
 */
void WorldEnvRender_LightPositionGet(VECTOR3* pos);

/** @brief Gets the point light rotation and intensity.
 *
 * @param rot Output light rotation.
 * @return Light intensity.
 */
s32 WorldEnvRender_LightRotationAndIntensityGet(SVECTOR* rot);

/** @brief Gets the point light direction and intensity.
 *
 * @param rot Output light direction.
 * @return Light intensity.
 */
s32 WorldEnvRender_LightDirectionAndIntensityGet(SVECTOR* dir);

/** Light function. */
void Gfx_FlashlightPositionUpdate(q19_12 lightIntensity, q3_12 lensFlareIntensity, GsCOORDINATE2* coord0, GsCOORDINATE2* coord1,
                                  SVECTOR* rot, q19_12 posX, q19_12 posY, q19_12 posZ, s_WaterZone* waterZones);

/** Light function. */
void func_80055648(s32 lightIntensity, const SVECTOR* dir);

s32 func_800557DC(void);

void func_80055814(s32 arg0);

void WorldEnvRender_FogDistanceSet(q19_12 nearDist, q19_12 farDist);

s32 func_800559A8(s32 arg0);

u8 func_80055A50(s32 arg0);

void func_80055A90(CVECTOR* arg0, CVECTOR* arg1, u8 arg2, s32 arg3);

/** @brief Applies uniform lighting and fog shading to `color`, outputting to `result`.
 * The fog factor is derived from `arg2` via `g_WorldEnvRenderWork.fogRamp` and blends the lit color toward the far/fog color in `g_WorldEnvRenderWork.field_1C/field_1D/field_1E`,
 * with lighting strength from `g_WorldEnvRenderWork.field_20` applied equally on all axes.
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

void Bone_ModelAssign(s_Bone* bone, s_LmHeader* lmHdr, s32 modelIdx);

bool Lm_ModelFind(s_WorldObjectModel* model, s_LmHeader* lmHdr, s_WorldObjectMetadata* metadata);

void StringCopy(char* prevStr, char* newStr);

/** @brief Draws a 2D fog overlay quad. */
void Gfx_FogOverlayQuadDraw(s16 x0, s16 y0, s16 x1, s16 y1, s32 arg4, s32 arg5, GsOT* ot, s32 otShift);

/** Draws 3D IPD chunk models? */
void func_80057090(s_ModelInfo* modelInfo, GsOT* otTag, s32 otShift, MATRIX* viewMat, MATRIX* worldMat, u16 clutY);

s32 func_800571D0(u32 arg0);

void WorldEnvRender_LightTransform(MATRIX* worldMat, q19_12 alpha, SVECTOR* arg2, VECTOR3* arg3);

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

u32 func_8005C478(s16* arg0, q19_12 x0, q19_12 y0, q19_12 x1, q19_12 y1, q19_12 x2, q19_12 y2);

s32 Chara_NpcIdxGet(s_SubCharacter* chara);

void Chara_CollisionShapeOffsetsUpdate(s_CharaShapeOffsets* offsets, s_SubCharacter* chara);

s32 Chara_MovementUpdate0(s_SubCharacter* chara, s_CollisionResult* collResult);

/** Checks `floorType`. */
bool func_8005F680(s_CollisionSurface* surface);

void Game_MapRoomIdxUpdate(void);

/** @unused */
s32 func_8003647C(void);

/** @unused */
s32 func_80036498(void);

// Used in some RoomBgmInit funcs.
u32 func_800364BC(void);

void Gfx_CursorDraw(s32 x0, s16 y0, s32 x1, s16 y1, s16 u, s16 v, s16 width, s32 height, s32 tint,
                    u32 clutX, s16 clutY, s32 tPage);

// Q12 square root?
u32 func_8008A058(s32 arg0);

/** @brief @unused Returns 0. */
s32 func_8008A0CC(void);

/** @brief @unused Returns 0. */
s64 func_8008A0D4(void);

/** Anim infos setup for character. */
s32 func_8008A0E4(s32 arg0, s32 weaponAttack, s_SubCharacter* chara, VECTOR3* attackPos, s_SubCharacter* unused,
                  q3_12 rotToTargetX, q3_12 rotToTargetY);

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

/** @brief @unused Sets the debug string position.
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

s32 Chara_MovementUpdate1(s_SubCharacter* chara, s_CollisionResult* collResult, q3_12 offsetX, q3_12 offsetZ);

/** Computes something for a targeted NPC. */
bool func_8005D50C(s32* targetNpcIdx, q3_12* rotToTargetX, q3_12* rotToTargetY, const VECTOR3* attackPos,
                   u32 npcIdx, q19_12 angleConstraint);

/** @unused */
void func_800652F4(VECTOR3* arg0, s16 arg1, s16 arg2, s16 arg3);

/** @unused */
void func_80065B94(VECTOR3* arg0, s16 arg1);

void func_80066184(void);

void func_80066D90(void);

void func_80066E40(void);

void func_80066E7C(void);

void Player_FlexRotationYReset(void);

void func_8004BBF4(VbRVIEW* camView, GsCOORDINATE2* camCoord, SVECTOR* camRot);

void Settings_ScreenAndVolUpdate(void);

void Settings_RestoreDefaults(void);

void Settings_RestoreControlDefaults(s32 configIdx);

void nullsub_800334C8(void);

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

/** @brief Updates the active autosave savegame entry with the most recent savegame. */
void SysWork_AutosaveUpdate(void);

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

u32 func_8008A2E0(s32 arg0);

/** @brief Draws a set of expanding boxes used to highlight a point on a paper map.
 *
 * @param progressAlpha Progress weight.
 * @param startX X start corner (top-left).
 * @param startY Y start corner (top-left).
 * @param startWidth Start width.
 * @param startHeight Start height.
 * @param endX X end corner (bottom-right).
 * @param endY Y end corner (bottom-right).
 * @param endWidth End width.
 * @param endHeight End height.
 */
void PaperMap_ExpandingBoxesDraw(q3_12 progressAlpha,
                                 q3_12 startX, q3_12 startY, q3_12 startWidth, q3_12 startHeight,
                                 q3_12 endX, q3_12 endY, q3_12 endWidth, q3_12 endHeight);

#endif
