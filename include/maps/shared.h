#ifndef _MAPS_SHARED_H
#define _MAPS_SHARED_H

#include "game.h"

#include "bodyprog/events/bgm_update.h"
#include "bodyprog/game_boot/game_boot.h"
#include "bodyprog/screen/background_draw.h"
#include "bodyprog/screen/screen_draw.h"
#include "maps/map_util.h"

/** @brief Heading angle quadrants. TODO: Order uncertain. North/South and East/West could be swapped. */
typedef enum _Quadrant
{
    Quadrant_South = 0,
    Quadrant_North = 1,
    Quadrant_West  = 2,
    Quadrant_East  = 3
} e_Quadrant;

typedef struct
{
    char unk_0[3420];
    u16  field_D5C;
    s16  field_D5E;
} s_func_800D8714;

/** @brief Stores SFX ID and volume.
 *  Volume union required becase one function reads the value as `u16`, another as `u8`.
 *  Actual value fits into `u8` (8 bit volume control).
 */
typedef struct _SfxVolume
{
    u16 id;
    union
    {
        u8  val8;
        u16 val16;
    } volume_2;
} s_SfxVolume;

typedef struct
{
    /* 0x0   */ s_AnimInfo  animInfo_0[56];
    /* 0x380 */ s32         field_380[301][2]; // Unknown size, biggest constant index used is 35. Associated with anims.
    /* 0xCE8 */ s_SfxVolume sfxVolumes_CE8[11];
    /* 0xD14 */ u_Property  properties_D14[4];
    /* 0xD24 */ u16         field_D24[2][3][2];
    /* 0xD3C */ u8          field_D3C[2][6];
    /* 0xD48 */ SVECTOR*    ptr_D48[5];
    /* 0xD5C */ s16         field_D5C[4][2]; // Related to animations. Move offset distances?
    /* 0xD6C */ s8          unk_D6C[4];
    /* 0xD70 */ s16         field_D70[2][2];
} s_func_800D2E04;

typedef struct
{
    /* 0x0   */ s32                 flags;
    /* 0x4   */ s_AnmHeader*        anmHdr_4;
    /* 0x8   */ GsCOORDINATE2*      coords_8;
    /* 0xC   */ VECTOR3             matrices_C[1]; // Unknown size.
    /* 0x10  */ s8                  unk_10[132];
    /* 0x94  */ s_CharaBox          box;
    /* 0xA0  */ s_CharaCylinder     cylinder;
    /* 0xA4  */ s_CharaShapeOffsets shapeOffsets;
    /* 0xAC  */ s32                 field_B4[6][4]; // [*][3] is angle.
    /* 0x114 */ s32                 field_114;
    /* 0x118 */ s32                 field_118;
    /* 0x11C */ VECTOR3             field_11C; // Q19.12
    /* 0x128 */ VECTOR3             field_128;
    /* 0x134 */ VECTOR3             field_134;
    /* 0x140 */ VECTOR3             field_140;
    union
    {
        struct
        {
            u8  field_14C_0 : 1;
            u8  field_14C_1 : 1;
            u8  field_14C_2 : 1;
            u8  field_14C_3 : 1;
            u32 field_14C_4 : 28; // Unsure if used.
        } bits;
        // @hack Some functions only match when bits are defined as `u32`, but that breaks other functions.
        struct
        {
            u32 field_14C_0 : 1;
            u32 field_14C_1 : 1;
            u32 field_14C_2 : 1;
            u32 field_14C_3 : 1;
            u32 field_14C_4 : 28;
        } bits32;
        u32 flags;
    } field_14C;
    q19_12 distance_150;
    q19_12 angle_154;
    q19_12 field_158; // Displacement distance.
    s32    field_15C;
} s_sharedData_800E21D0_0_s01;

// Used by puzzle in M5S01 and M7S01.
typedef struct
{
    u8 field_0; // Q12
    u8 field_1;
} s_800F0158;

extern s_MapOverlayHdr_94 sharedData_800E30C8_1_s02;

#if defined(MAP_FIELD_4C_COUNT)
    extern s_MapHdr_field_4C sharedData_800DFB7C_0_s00[MAP_FIELD_4C_COUNT];
#else
    extern s_MapHdr_field_4C sharedData_800DFB7C_0_s00[];
#endif

#if defined(MAP_BLOOD_SPLAT_COUNT_MAX)
    extern s_BloodSplat g_Effect_BloodSplats[MAP_BLOOD_SPLAT_COUNT_MAX];
#else
    extern s_BloodSplat g_Effect_BloodSplats[];
#endif

/** Air Screamer or Night Flutter control functions. Indexed using `e_AirScreamerControl`. */
extern void (*g_AirScreamer_ControlFuncs[52])(s_SubCharacter* airScreamer); // TODO: Use `AirScreamerControl_Count`.

extern s_MapPoint2d MAP_POINTS[];
extern s_EventData  MAP_EVENTS[];
extern void (*g_LoadScreenFuncs[])();
extern void (*g_MapEventFuncs[])();
extern u8 LOADABLE_INVENTORY_ITEMS[];
extern s_UnkStruct3_Mo g_MapHeaderTable_38[];

/** Scale. */
extern VECTOR sharedData_800DD870_0_s01;

/** Scale. */
extern VECTOR sharedData_800DD880_0_s01;

/** Could be a padded `VECTOR`, but the ASM for the function `vec = vec` assignments copies only the first 3 fields. */
extern VECTOR3 sharedData_800DE170_0_s01;
extern VECTOR3 g_AirScreamer_HeadTargetPosition;
extern VECTOR3 sharedData_800DE190_0_s01;

extern s_RayTrace sharedData_800E2330_0_s01;

/** Packed weapon attack. `See `WEAPON_ATTACK`. */
extern s8 g_Player_PrevWeaponAttack;

extern s32 sharedData_800DF1F4_0_s00;
extern s16 sharedData_800DF1F8_0_s00;
extern s16 sharedData_800DF1FA_0_s00;

extern u8 sharedData_800EEAC4_2_s00[][4][4];
extern VECTOR3 sharedData_800F216C_2_s00;
extern VECTOR3 sharedData_800F217C_2_s00;
extern VECTOR3 sharedData_800F21BC_2_s00;
extern VECTOR3 sharedData_800F21CC_2_s00;
extern VECTOR3 sharedData_800F21DC_2_s00;
extern VECTOR3 sharedData_800F21EC_2_s00;
extern VECTOR3 sharedData_800F21FC_2_s00;
extern VECTOR sharedData_800DE200_0_s01;
extern VECTOR3 sharedData_800DE210_0_s01;
extern SVECTOR sharedData_800DE220_0_s01;
extern MATRIX sharedData_800DE230_0_s01;

/** Something related to rotation. */
extern s32 sharedData_800E39D8_0_s00;

// Used by `Stalker`/`LarvalStalker`.
extern u8     sharedData_800DD5A6_0_s00;
extern s32    sharedData_800E39E4_0_s00;
extern s32    sharedData_800E39E8_0_s00;
extern s32    sharedData_800E39EC_0_s00[];
extern u16    sharedData_800E3A0C_0_s00[]; // Angles?
extern q19_12 g_Stalker_TargetPositionX;
extern q19_12 g_Stalker_TargetPositionZ;

// Two variables used by `Creeper_Init`.
extern s8 sharedData_800E57CC_1_s02;

extern s_sharedData_800E21D0_0_s01 sharedData_800E21D0_0_s01;

extern s_func_800D2E04 const sharedData_800CAA98_0_s01; // Used by `AirScreamer_Init` / `sharedFunc_800D2BF4_0_s01`

extern u8  sharedData_800D16E4_2_s01;

extern u8 sharedData_800DD591_0_s00;
extern u8 sharedData_800DD584_0_s00;

extern s32 sharedData_800DFB6C_0_s00;
extern s32 sharedData_800DFB70_0_s00;

extern s32 sharedData_800DD58C_0_s00;
extern s32 sharedData_800E32CC_0_s00;

extern u16 sharedData_800D21E8_3_s00[];

extern u16 sharedData_800E32D4_0_s00;

extern s32 sharedData_800DD598_0_s00;

extern CVECTOR sharedData_800E3258_0_s00;

extern CVECTOR sharedData_800E325C_0_s00;

extern q19_12 sharedData_800E2370_0_s01[2]; // Air Screamer ground collision heights?

// Each of these holds 8 bytes, passed to `Bgm_LayersUpdate` which loops through each byte.
// TODO: Should use `s_BgmLayerLimits`?
extern u8 sharedData_800CFB14_0_s02[8];
extern u8 sharedData_800CFB1C_0_s02[8];
extern u8 sharedData_800CFB24_0_s02[8];
extern u8 sharedData_800CFB2C_0_s02[8];
extern u8 sharedData_800CFB34_0_s02[8];
extern u8 sharedData_800CFB3C_0_s02[8];
extern u8 sharedData_800CFB44_0_s02[8];
extern u8 sharedData_800CFB4C_0_s02[8];
extern u8 sharedData_800CFB54_0_s02[8];

// Cached collision result for Cheryl, Dahlia, and the player?
extern s_CollisionResult sharedData_800E39BC_0_s00;

extern s32 sharedData_800DD5A0_0_s00;
extern s16 sharedData_800DD5A4_0_s00; // Used as index into array of vectors.
extern s16 sharedData_800E39E0_0_s00;
extern s16 sharedData_800E39E2_0_s00;

extern u8      sharedData_800EB738_6_s04;
extern u8      sharedData_800EB740_6_s04[2];
extern u8      sharedData_800EB748_6_s04;
extern s8      sharedData_800EB74A_6_s04;
extern VECTOR3 sharedData_800EB750_6_s04;

extern s16 sharedData_800D5880_1_s05;

extern u8 sharedData_800E1208_1_s02[8];
extern u16 sharedData_800E1210_1_s02[42]; // Flags.

typedef struct
{
    VECTOR3 field_0; // Rotation?
    s16     field_C;
    s16     field_E;
    s8      field_10;
    u8      field_11;
    u8      field_12;
    s8      field_13;
} s_800E34FC;
STATIC_ASSERT_SIZEOF(s_800E34FC, 20);

typedef struct
{
    VECTOR3  field_0;
    SVECTOR3 field_C; // 2D position or offset.
    u8       unk_12[2];
    u8       field_14;
    u8       field_15;
    u8       field_16; // Packed angle?
} s_800E330C;
STATIC_ASSERT_SIZEOF(s_800E330C, 24);

typedef struct
{
    s32   field_0;
    s32   field_4;
    q3_12 field_8; // Y offset.
    s16   field_A;
} s_D_800D5A8C;
STATIC_ASSERT_SIZEOF(s_D_800D5A8C, 12);

typedef struct _MapHeader_field_5C
{
    u8    flags;
    u8    field_1;
    u8    field_2;
    u8    field_3;
    u8    field_4;
    u8    field_5;
    u8    field_6;
    u8    field_7;
    u16   field_8;
    s16   field_A;
    u16   field_C;
    s16   field_E;
    q3_12 field_10;
    s16   field_12;
    s16   field_14;
    s16   field_16;
    s16   field_18;
    s16   field_1A;
    u16   field_1C;
    s16   field_1E;
    s32   field_20;
    s32   field_24;
} s_MapHeader_field_5C;

typedef struct
{
    s32 vx;
    s32 vy;
    s16 vz;
} s_sharedData_800DFB10_0_s01;

typedef struct
{
    q19_12 field_0; // Relative anim time?
    s32    field_4;
} s_sharedData_800ED2D4_2_s02;

extern s_MapHeader_field_5C sharedData_800D8568_1_s05;

extern s_800E330C sharedData_800E330C_0_s00[20];

extern s_800E34FC sharedData_800E34FC_0_s00[60];

extern const char* MAP_MESSAGES[];

extern s_MapOverlayHdr_7C sharedData_800DEE50_1_s01;

extern u8      sharedData_800D4CD4_3_s01[][2];
extern DVECTOR sharedData_800D4D0C_3_s01;
extern VECTOR3 sharedData_800CB088_3_s01;
extern VECTOR3 sharedData_800CB094_3_s01;
extern VECTOR3 sharedData_800CB0A0_3_s01;
extern s32     sharedData_800D4D10_3_s01;
extern s32     sharedData_800D4D14_3_s01;
extern s8      sharedData_800D4D18_3_s01;

extern s_D_800D5A8C sharedData_800D5A8C_3_s03[3];

extern u8 sharedData_800D1D14_3_s02[8];

/** Room flags? */
extern u16 sharedData_800D1D1C_3_s02[60];

extern s_sharedData_800DFB10_0_s01 sharedData_800DFB10_0_s01[];

extern u8 sharedData_800ED430_2_s02[];

extern u8 sharedData_800ED43C_2_s02[];

extern u8  sharedData_800EFCFC_6_s00[8];
extern s32 sharedData_800EFD04_6_s00;
extern u16 sharedData_800EFD08_6_s00[];
extern u16 sharedData_800EFD34_6_s00[];

extern u8 sharedData_800ED418_4_s02[8];
extern s32 sharedData_800ED420_4_s02;   // `e_BgmFlags`
extern u8 sharedData_800ED424_4_s02[8];
extern u16 sharedData_800ED42C_4_s02[]; // `e_BgmFlags` | Map room index used for access.
extern u8 sharedData_800ED458_4_s02[8];

extern s_BgmLayerLimits sharedData_800D2F18_7_s00;
extern s_BgmLayerLimits sharedData_800D2F20_7_s00;
extern u16          sharedData_800D2F28_7_s00[38]; // `e_BgmFlags`
extern s_BgmLayerLimits sharedData_800D2F74_7_s00;
extern s_BgmLayerLimits sharedData_800D2F7C_7_s00;
extern s_BgmLayerLimits sharedData_800D2F84_7_s00;

extern s32 sharedData_800E1570_7_s01;
extern s32 sharedData_800E1574_7_s01;
extern u8  sharedData_800E1578_7_s01[3][3][3][2];
extern u32 sharedData_800E1694_7_s01;
extern s32 sharedData_800E2CA8_7_s01;
extern s32 sharedData_800E2CAC_7_s01;

void sharedFunc_800DB60C_7_s01(void);

bool sharedFunc_800CE688_1_s03(POLY_FT4** poly, s32 idx);

bool sharedFunc_800CD1F8_0_s01(POLY_FT4** poly, s32 idx);

void Map_RoomBgmInit(bool arg0);

void Map_RoomBgmInit_CondTrue(void);

void Map_RoomBgmInit_CondFalse(void);

void sharedFunc_800CAAD0_1_s05(void);

void sharedFunc_800CABF8_1_s05(s32 idx, bool arg1);

bool sharedFunc_800CB040_1_s05(POLY_FT4** poly, s32 idx);

bool sharedFunc_800CB884_1_s05(POLY_FT4** poly, s32 idx);

void sharedFunc_800CBE7C_1_s05(q23_8 posX, q23_8 posY, q23_8 posZ, s32 arg3, s32 arg4);

void sharedFunc_800CCDD4_1_s05(POLY_FT4** poly, s32 idx);

void sharedFunc_800CCE2C_1_s05(s32 arg0, s32 arg1,
                               q19_12 posX0, q19_12 posY0, q19_12 posZ0,
                               q19_12 posX1, q19_12 posY1, q19_12 posZ1);

void sharedFunc_800D4408_1_s05(VECTOR3* pos, s32 idx, q23_8 posX, q23_8 posY, q23_8 posZ);

/** NPC-only. Among other things, sets the NPC's anim to anim 3 (which might actually be flags if the field packs more data). */
void Chara_AnimReset(s_SubCharacter* npc, bool cond);

/** NPC-only, sets `controlState` in properties. */
void Chara_AnimStateSet(s_SubCharacter* npc, s32 controlState);

/** NPC-only, sets `resetControlState` in properties. */
void Chara_ControlStateReset(s_SubCharacter* npc);

/** Locks a character's animation. */
void Chara_AnimLock(s_SubCharacter* chara);

/** Unlocked flag getter. */
s32 Chara_AnimIsLocked(s_SubCharacter* chara);

/** Unlocks a character's animation. */
void Chara_AnimUnlock(s_SubCharacter* chara);

// Seems like this function is intended to be called repeatedly, swapping between states.
// Might be called many times in a single logic update or over multiple updates.
// Very similar to `Player_PathWaypointExecute`.
bool Chara_PathWaypointExecute(s_SubCharacter* chara, s32 arg1, VECTOR3* arg2In, q19_12 angleIn, s32 arg4);

void Chara_VisibleSet(s_SubCharacter* chara);

void Chara_InvisibleSet(s_SubCharacter* chara);

s32 Chara_AnimStartKeyframeIdxGet(s_SubCharacter* chara);

/** Humanoid init function? */
void Chara_CollisionReset(s_SubCharacter* chara);

void Event_CutsceneTimerAdvance(q19_12* timer, q19_12 rate, q19_12 startTime, q19_12 endTime,
                                bool clampToEnd, bool autoAdvance);

void sharedFunc_800CB7F4_1_s01(void);

void sharedFunc_800CB8A0_1_s01(s32 idx);

bool sharedFunc_800CBA38_1_s01(s32 idx);

bool sharedFunc_800CBB30_1_s01(POLY_FT4** poly, s32 idx);

bool sharedFunc_800CBDA8_1_s02(POLY_FT4** poly, s32 idx);

void sharedFunc_800CCE60_1_s02(void);

bool sharedFunc_800CBF74_1_s05(POLY_FT4** poly, s32 idx);

bool sharedFunc_800CC618_1_s05(POLY_FT4** poly, s32 idx);

bool sharedFunc_800CCF30_1_s05(POLY_FT4** poly, s32 idx);

// Name uncertain.
void Chara_MovementReset(s_SubCharacter* npc);

/** @brief Gets the playback state of a character's active animation.
 *
 * @param chara Character to check.
 * @return Playback state (`e_AnimPlaybackState`);
 */
s32 Chara_AnimPlaybackStateGet(s_SubCharacter* chara);

bool Npc_SfxPlay(s32 animStatus, s_SubCharacter* chara, s32 keyframeIdx, s32 sfxId);

/** Character SFX func. Suggests there's a shared property struct for humanoid NPCs. */
bool Npc_FootstepSoundPlay(s32 animStatus, s_SubCharacter* chara, s32 keyframeIdx0, s32 keyframeIdx1, s32 sfxId, s32 pitch);

void sharedFunc_800CCB8C_0_s01(VECTOR* arg0, VECTOR* arg1, s16 arg2, s32 arg3, s16 arg4, s16 arg5, s32 arg6, s32 arg7);

void sharedFunc_800CE5D4_1_s03(VECTOR3* arg0, u16 arg1, u16 arg2, u32 arg3);

void sharedFunc_800CBA4C_1_s02(s32 idx, s32 arg1, s32 arg2);

void sharedFunc_800CBC98_1_s02(void);

void sharedFunc_800CBD58_1_s02(void);

/** @brief Handles the jammed door event. */
void MapEvent_DoorJammed(void);

/** @brief Handles the locked door event. */
void MapEvent_DoorLocked(void);

void sharedFunc_800D15F0_3_s01(void);

/** @brief Handles the unlocked door event. */
void MapEvent_DoorUnlocked(void);

void GameBoot_LoadScreen_StageString(void);

void sharedFunc_800CBE54_4_s03(VECTOR3* arg0, s32 arg1);

bool sharedFunc_800CC004_4_s03(POLY_FT4** poly, s32 idx);

bool sharedFunc_800CB1B0_4_s03(POLY_FT4** poly, s32 idx);

// Twinfeeler and Floatstinger acid particle effect. Included in M4S03 and M4S05
// TODO: Make this separate split in each map instead of `#include`.
void sharedFunc_800CB0A4_4_s03(VECTOR3* from, VECTOR3* to);

void sharedFunc_800D0110_7_s00(void);

static inline void ModelAnim_AnimInfoSet(s_ModelAnim* anim, s_AnimInfo* animInfo)
{
    anim->baseAnimInfos = animInfo;
    anim->mapAnimInfos  = NULL;
}

/** @brief World object description with a pose (position and rotation). */
typedef struct _WorldObjectPose
{
    /* 0x0  */ s_WorldObjectModel object;
    /* 0x1C */ VECTOR3            position; /** Q19.12 */
    /* 0x28 */ SVECTOR3           rotation; /** Q19.12 */
} s_WorldObjectPose;
STATIC_ASSERT_SIZEOF(s_WorldObjectPose, 48);

/** @brief World object description with a position. */
typedef struct _WorldObjectPlacement
{
    /* 0x0  */ s_WorldObjectModel object;
    /* 0x1C */ VECTOR3            position; /** Q19.12 */
} s_WorldObjectPlacement;
STATIC_ASSERT_SIZEOF(s_WorldObjectPlacement, 40);

#define WorldObject_PoseInit(objPose, posX, posY, posZ, rotX, rotY, rotZ) \
    WorldObject_PoseSet(objPose,                                          \
                        Q12(posX), Q12(posY), Q12(posZ),                  \
                        Q12_ANGLE(rotX), Q12_ANGLE(rotY), Q12_ANGLE(rotZ))
#define WorldObject_PoseSet(objPose, posX, posY, posZ, rotX, rotY, rotZ) \
{                                                                        \
    Math_Vector3Set(&(objPose)->position, posX, posY, posZ);             \
    Math_SetSVectorFast(&(objPose)->rotation, rotX, rotY, rotZ);         \
}

#define WorldObject_Init(objPose, name, posX, posY, posZ, rotX, rotY, rotZ) \
    WorldObject_Set(objPose, name,                                          \
                    Q12(posX), Q12(posY), Q12(posZ),                        \
                    Q12_ANGLE(rotX), Q12_ANGLE(rotY), Q12_ANGLE(rotZ))
#define WorldObject_Set(objPose, name, posX, posY, posZ, rotX, rotY, rotZ) \
{                                                                          \
    Math_Vector3Set(&(objPose)->position, posX, posY, posZ);     \
    Math_SetSVectorFast(&(objPose)->rotation, rotX, rotY, rotZ); \
    WorldObject_ModelNameSet(&(objPose)->object, (name));        \
}

#define WorldObject_PosePositionInit(objPose, name, posX, posY, posZ) \
    WorldObject_PosePositionSet(objPose, name, Q12(posX), Q12(posY), Q12(posZ))
#define WorldObject_PosePositionSet(objPose, name, posX, posY, posZ) \
{                                                                    \
    Math_Vector3Set(&(objPose)->position, posX, posY, posZ);         \
    WorldObject_ModelNameSet(&(objPose)->object, (name));            \
}

#define WorldObject_PlacementInit(objPlacement, name, posX, posY, posZ) \
    WorldObject_PlacementSet(objPlacement, name, Q12(posX), Q12(posY), Q12(posZ))
#define WorldObject_PlacementSet(objPose, name, posX, posY, posZ) \
{                                                                 \
    Math_Vector3Set(&(objPose)->position, posX, posY, posZ);      \
    WorldObject_ModelNameSet(&(objPose)->object, (name));         \
}

#define APPROACH(current, target, step) \
    ((current) > (target) ? MAX((current) - (step), (target)) : MIN((target), (current) + (step)))

#define APPROACH_ALT(current, target, step) \
    ((current) > (target) ? MAX((current) - (step), (target)) : CLAMP_HIGH((current) + (step), (target)))

#define APPROACH_ALT2(current, target, step) \
    ((current) > (target) ? CLAMP_LOW((current) - (step), (target)) : CLAMP_HIGH((target), (current) + (step)))

/** @brief Updates the move speed of a character.
 *
 * @param chara Character to update.
 * @param speed Move speed (Q*.12).
 */
#define Chara_MoveSpeedUpdate(chara, speed) \
    chara->moveSpeed = APPROACH(chara->moveSpeed, Q12(0.0f), Q12_MULT_PRECISE(g_DeltaTime, speed))

// TODO: Is it possible to merge these macros?
#define Chara_MoveSpeedUpdate2(chara, speed, limit)                      \
{                                                                        \
    q19_12 moveSpeed;                                                    \
    q19_12 newSpeed;                                                     \
    q19_12 newMoveSpeed;                                                 \
                                                                         \
    moveSpeed = chara->moveSpeed;                                        \
    if (moveSpeed > limit)                                               \
    {                                                                    \
        newMoveSpeed = limit;                                            \
        limit        = Q12(0.0f);                                        \
        newSpeed     = moveSpeed - Q12_MULT_PRECISE(g_DeltaTime, speed); \
        if (newMoveSpeed < newSpeed)                                     \
        {                                                                \
            newMoveSpeed = newSpeed;                                     \
        }                                                                \
    }                                                                    \
    else                                                                 \
    {                                                                    \
        newMoveSpeed = limit;                                            \
        newSpeed     = moveSpeed + Q12_MULT_PRECISE(g_DeltaTime, speed); \
        if (newMoveSpeed >= newSpeed)                                    \
        {                                                                \
            newMoveSpeed = newSpeed;                                     \
        }                                                                \
    }                                                                    \
    chara->moveSpeed = newMoveSpeed;                                     \
}

#define Chara_MoveSpeedUpdate3(chara, speed, limit) \
    chara->moveSpeed = APPROACH(chara->moveSpeed, limit, Q12_MULT_PRECISE(g_DeltaTime, speed))

#define Chara_MoveSpeedUpdate4(chara, speed, limit) \
    chara->moveSpeed = APPROACH_ALT(chara->moveSpeed, limit, Q12_MULT_PRECISE(g_DeltaTime, speed))

#endif
