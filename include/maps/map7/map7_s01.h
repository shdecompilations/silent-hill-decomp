#ifndef _MAPS_MAP7_S01_H
#define _MAPS_MAP7_S01_H

#define HAS_PlayerState_Unk51
#define HAS_PlayerState_Unk131
#define HAS_PlayerState_Reset
#define HAS_PlayerState_WalkForward
#define HAS_PlayerState_RunForward
#define HAS_PlayerState_WalkBackward
#define HAS_PlayerState_TurnRight
#define HAS_PlayerState_TurnLeft
#define HAS_PlayerState_Unk59
#define HAS_PlayerState_Unk60
#define HAS_PlayerState_Unk72
#define HAS_PlayerState_Unk145
#define HAS_PlayerState_Unk148
#define HAS_PlayerState_Unk155
#define HAS_PlayerState_Unk156
#define HAS_PlayerState_Unk157
#define HAS_PlayerState_Unk158
#define HAS_PlayerState_Unk167

#define HAS_Chara_GhostChildAlessa
#define HAS_Chara_Lisa
#define HAS_Chara_BloodyLisa
#define MAP_CHARA_COUNT 3

// Config used by `Map_RoomIdxGet`.
#define MAP_ROOM_MIN_X    Q12(-200.0f)
#define MAP_ROOM_MAX_X    Q12(200.0f)
#define MAP_ROOM_MIN_Z    Q12(-160.0f)
#define MAP_ROOM_MAX_Z    Q12(0.f)

#define MAP_FIELD_4C_COUNT 250
#define MAP_BLOOD_SPLAT_COUNT_MAX 150

#if VERSION_REGION_IS(NTSC)
    #define MAP_MSG(idx) (idx)
#elif VERSION_REGION_IS(NTSCJ)
    #define MAP_MSG(idx) ((idx) >= 70 ? (idx) - 3 : (idx) >= 67 ? 66 : (idx) >= 39 ? (idx) - 1 : (idx))
#endif

#include "maps/common/shared.h"

typedef struct
{
    u8  field_0;
    u8  unk_1[1];
    s16 field_2;
} s_800E1690;

/** Character ID. (TODO: Move to `bodyprog.h`)*/
extern s8 D_800A9945;

extern u16 D_800E14E8;

extern u16     D_800E1500;
extern VECTOR3 D_800E1504;

extern s_800F0158 D_800E1510[26];

extern u8 D_800E1544[5];

/** Map message indices. */
extern u16 D_800E154C[];

extern u16 D_800E155C;

extern s16 D_800E1560[];

extern u8 D_800E156E;

extern u8 D_800E1688[5];

extern u8 D_800E1670;
extern u8 D_800E1671;

extern s16 D_800E1678[4];
extern u8  D_800E1680;

/** Index into `D_800E154C`. */
extern u8 D_800E168D;

// Fallng stone Y position?
extern q4_12 D_800E168E;

extern s_800E1690 D_800E1690;

extern s16 D_800E16A8[2];

extern VECTOR3 g_Cutscene_CameraPositionTarget;
extern VECTOR3 g_Cutscene_CameraLookAtTarget;

extern u8 D_800E2C64;
extern q19_12 g_Cutscene_Timer;

extern s_WorldObjectPose g_WorldObject_Door;
extern s_WorldObjectPose g_WorldObject_Stone0;

extern VECTOR3 D_800E2CD4;

/** Decrementing timer. */
extern q3_12 D_800E2CE8;

extern s_WorldObjectPlacement g_WorldObject_Dr[6];
extern s_WorldObjectPose      g_WorldObject_Key0;
extern s_WorldObjectPlacement g_WorldObject_Gofu;
extern s_WorldObjectPlacement g_WorldObject_Key1;
extern s_WorldObjectPlacement g_WorldObject_Penchi;
extern s_WorldObjectPlacement g_WorldObject_Driver;
extern s_WorldObjectPlacement g_WorldObject_Zukan;
extern s_WorldObjectPlacement g_WorldObject_Scrap;
extern s_WorldObjectModel        g_WorldObject_Key2;
extern s_WorldObjectModel        g_WorldObject_Futa[2];
extern s_Pose      g_WorldObject_UnkPose1;
extern s_WorldObjectModel        g_WorldObject_Key1_2[2];
extern s_Pose      g_WorldObject_UnkPose2;
extern s_WorldObjectPose      g_WorldObject_Camera;
extern s_WorldObjectPose      g_WorldObject_Ring;
extern s_WorldObjectPose      g_WorldObject_Gofu_B;
extern s_WorldObjectModel        g_WorldObject_KeyX1;
extern s_WorldObjectModel        g_WorldObject_KeyX2;
extern VECTOR3                g_WorldObject_UnkPos2;
extern SVECTOR3               g_WorldObject_UnkRot1;
extern SVECTOR3               g_WorldObject_UnkRot2;
extern SVECTOR3               g_WorldObject_UnkRot3;
extern SVECTOR3               g_WorldObject_UnkRot4;
extern s_WorldObjectPose      g_WorldObject_Ana;
extern s_WorldObjectPose      g_WorldObject_Stone1;
extern s_WorldObjectPose      g_WorldObject_Window;
extern s_WorldObjectPlacement g_WorldObject_Kubomi;
extern s_WorldObjectModel        g_WorldObject_KaigaR[2];
extern VECTOR3                g_WorldObject_UnkPos0;
extern s_WorldObjectModel        g_WorldObject_KaigaL[2];
extern VECTOR3                g_WorldObject_UnkPos1;
extern SVECTOR3               g_WorldObject_UnkRot0;
extern s_WorldObjectModel        g_WorldObject_SavePad;
extern s_Pose      g_WorldObject_UnkPose0;
extern s_WorldObjectPose      g_WorldObject_Diary;
extern s_WorldObjectModel        g_CommonWorldObjects[6];
extern s_Pose      g_CommonWorldObjectPoses[8]; // 0x800E15C8

extern s32 D_800E33A0;
extern s16 D_800E33A4;

extern s32 D_800E1570;
extern s32 D_800E1574;
extern u8  D_800E1578[3][3][3][2];
extern u32 D_800E1694;

void func_800D6810(s32 arg0);

void func_800D6844(s32 arg0);

void func_800D6878(void);

void func_800D68F8(void);

void func_800D725C(void);

/** Cutscene. */
void func_800D7A60(void);

void func_800D8230(void);

void func_800D82C4(void);

void func_800D8358(void);

/** @brief Handles a First Aid Kit, Health Drink, Handgun Bullets, or Shotgun Shells item pickup event. */
void MapEvent_CommonItemTake(void);

void func_800D84AC(void);

void func_800D8540(void);

void func_800D85D4(void);

/** @brief Handles the Key of Ophiel item use event. */
void MapEvent_KeyOfOphielUse(void);

/** @brief Handles the Key of Hagith item use event. */
void MapEvent_KeyOfHagithUse(void);

/** @brief Handles the Key of Phaleg item use event. */
void MapEvent_KeyOfPhalegUse(void);

void func_800D889C(void);

void func_800D8958(void);

/** @brief Handles the Bird Cage Key item use event. */
void MapEvent_BirdCageKeyUse(void);

void func_800D89D8(void);

void func_800D8A04(void);

void func_800D8A30(void);

void func_800D8A5C(void);

void func_800D8DB4(void);

void func_800D8FCC(void);

void func_800D8FF8(void);

void func_800D93BC(void);

void func_800D93E8(void);

void func_800D9414(void);

void func_800D9440(void);

void func_800D946C(void);

void func_800D94A4(void);

/** @brief Puzzle event? Similar to M5S01 `func_800EBA40`. */
void func_800D94DC(void);

void func_800D99DC(void);

void func_800D9A08(void);

void func_800D9C9C(void);

void func_800DAB64(void);

void func_800DAD7C(void);

void func_800DADA8(void);

void MapEvent_CutsceneCherylRedirect3(void);

void func_800DAE00(void);

void func_800DB13C(void);

void func_800DB1F0(void);

void func_800DB31C(void);

void func_800DB3D0(void);

void func_800DBDE0(void);

void func_800DC028(void);

void func_800DC080(void);

void func_800DCE20(void);

void func_800DD348(void* unused, s32 idx, u8 spawnFlags);

void Map_WorldObjectsInit(void);

void Map_WorldObjectsUpdate(void);

void func_800DEDA4(void);

#endif
