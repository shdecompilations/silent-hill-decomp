#ifndef _MAPS_MAP7_S02_H
#define _MAPS_MAP7_S02_H

#define HAS_PlayerState_Unk51
#define HAS_PlayerState_Unk131
#define HAS_PlayerState_Reset
#define HAS_PlayerState_Unk59
#define HAS_PlayerState_Unk60
#define HAS_PlayerState_Unk72
#define HAS_PlayerState_Unk143
#define HAS_PlayerState_Unk144
#define HAS_PlayerState_Unk146
#define HAS_PlayerState_Unk147
#define HAS_PlayerState_Unk148
#define HAS_PlayerState_Unk155
#define HAS_PlayerState_Unk156
#define HAS_PlayerState_Unk157
#define HAS_PlayerState_Unk158
#define KEYFRAME_PlayerState_Unk59 862
#define KEYFRAME_PlayerState_Unk60 877

#define HAS_Chara_Dahlia
#define HAS_Chara_GhostChildAlessa
#define HAS_Chara_Kaufmann
#define HAS_Chara_GhostDoctor
#define MAP_CHARA_COUNT 4

// Config used by `Map_RoomIdxGet`.
#define MAP_ROOM_MIN_X    Q12(-200.0f)
#define MAP_ROOM_MAX_X    Q12(200.0f)
#define MAP_ROOM_MIN_Z    Q12(-160.0f)
#define MAP_ROOM_MAX_Z    Q12(0.f)

#define MAP_FIELD_4C_COUNT 200
#define MAP_BLOOD_SPLAT_COUNT_MAX 150

#if VERSION_REGION_IS(NTSC)
    #define MAP_MSG(idx) (idx)
#elif VERSION_REGION_IS(NTSCJ)
    #define MAP_MSG(idx) ((idx) >= 39 ? (idx) - 3 : (idx) >= 36 ? (idx) - 2 : (idx) >= 24 ? (idx) - 1 : (idx))
#endif

#include "maps/common/shared.h"

typedef struct
{
    u8  field_0;
    u8  unk_1[1];
    s16 field_2;
} s_800EA494;

extern s_MapPoint2d MAP_POINTS[];

extern u16 D_800E9CE4;

extern u16 D_800E9CFC;

/** Some SFX position. */
extern VECTOR3 D_800E9D00;

extern u16 D_800E9D50;

/** Map message indices. */
extern u16 D_800E9D6C[];

extern u16 D_800E9D7C;

extern s16 D_800E9D80[];

extern u8 D_800E9D8E;

extern s8 D_800E9ECC;
extern u8 D_800E9ECD;

extern q19_12 D_800E9ED0;

extern s16 D_800E9ED4;
extern s16 D_800E9ED6;
extern q3_12 D_800E9ED8; // Time value.
extern s16 D_800E9EDA;

/** Index into `D_800E9D6C`. */
extern u8 D_800EA490;
extern u16 D_800EA492;

extern s_800EA494 D_800EA494;

extern u8 D_800EA4A9;

extern VECTOR3 g_Cutscene_CameraPositionTarget;
extern VECTOR3 g_Cutscene_CameraLookAtTarget;

extern u8     D_800EB6B0;
extern q19_12 g_Cutscene_Timer; // Timer.

extern s_WorldObjectPose      g_WorldObject_Door;
extern s_WorldObjectPose      g_WorldObject_Beans[16];
extern s16                    D_800EB9F4[16];
extern s16                    D_800EBA14[16];
extern s_WorldObjectPose      g_WorldObject_Dor[4];
extern u8                     D_800EBAF4;
extern s_WorldObjectPose      g_WorldObject_Bed3;
extern s_WorldObjectPlacement g_WorldObject_Cover;
extern s_WorldObjectPose      D_800EBB64;

/** Timer. */
extern q3_12 D_800EBB94;

extern s_WorldObjectPlacement g_WorldObject_Dr[6];
extern s_WorldObjectPlacement g_WorldObject_Ank;
extern s_WorldObjectPlacement g_WorldObject_Ref[2];
extern s_WorldObjectPlacement g_WorldObject_Chain1[2];
extern s_WorldObjectPlacement g_WorldObject_Sword;
extern s_WorldObjectPose      g_WorldObject_Jelly;
extern s_WorldObjectPose      g_WorldObject_Key0;
extern s_WorldObjectPlacement g_WorldObject_Door9;
extern s_WorldObjectPlacement g_WorldObject_Wall9;
extern s_WorldObjectPlacement g_WorldObject_Gofu;
extern s_WorldObjectPlacement g_WorldObject_Driver;
extern s_WorldObjectPlacement g_WorldObject_Zukan;
extern s_WorldObjectPlacement g_WorldObject_Scrap;
extern s_WorldObjectModel        g_WorldObject_Key1;
extern s_WorldObjectModel        g_WorldObject_Futa[2];
extern s_Pose      g_WorldObject_UnkPose2;
extern s_WorldObjectPose      g_WorldObject_Camera;
extern s_WorldObjectPose      g_WorldObject_Ring;
extern s_WorldObjectPose      g_WorldObject_Gofu_B;
extern s_WorldObjectPlacement g_WorldObject_Mov1;
extern s_WorldObjectPlacement g_WorldObject_Key2;
extern s_WorldObjectPose      g_WorldObject_GofuA[5];
extern s_WorldObjectPose      g_WorldObject_KeyX2;
extern s_WorldObjectPose      g_WorldObject_Stone;
extern s_WorldObjectPose      g_WorldObject_Window;
extern s_WorldObjectModel        g_WorldObject_KaigaR[2];
extern VECTOR3                g_WorldObject_UnkPos0;
extern s_WorldObjectModel        g_WorldObject_KaigaL[2];
extern VECTOR3                g_WorldObject_UnkPos1;
extern SVECTOR3               g_WorldObject_UnkRot0;
extern s_WorldObjectModel        g_WorldObject_SavePad;
extern s_Pose      g_WorldObject_UnkPose0;
extern s_Pose      g_WorldObject_UnkPose1;
extern s_WorldObjectPose      g_WorldObject_Diary;
extern s_WorldObjectPlacement g_WorldObject_Kubomi;
extern s_WorldObjectModel        g_CommonWorldObjects[6];
extern s_Pose         g_CommonWorldObjectPoses[8];

extern u8 D_800E9DE8[26][2];
extern u8 D_800EA4AC[5];
extern u8 D_800E9E1C[5];

extern VECTOR3       D_800EA484;
extern s16           D_800E9EDC;
extern GsCOORDINATE2 D_800E9EE4;
extern RECT          D_800E9D1C;
extern u16           D_800E9D30;
extern RECT          D_800E9D0C[];
extern u16           D_800E9D24[];
extern u16           D_800E9D2C;

void func_800D97FC(bool arg0);

void func_800D9830(s32 arg0);

void func_800D9864(void);

void func_800D98E4(void);

void func_800DA248(void);

/** Cutscene. */
void func_800DAA4C(void);

/** @brief Handles a First Aid Kit, Health Drink, Handgun Bullets, or Shotgun Shells item pickup event. */
void MapEvent_CommonItemTake(void);

void func_800DB498(void);

void func_800DB52C(void);

/** @brief Handles the Key of Bethor item use event. */
void MapEvent_KeyOfBethorUse(void);

/** @brief Handles the Key of Aratron item use event. */
void MapEvent_KeyOfAratronUse(void);

void func_800DB738(void);

void func_800DBB34(void);

/** @brief Handles the Ankh item pickup event. */
void MapEvent_AnkhTake(void);

void func_800DC14C(void);

void func_800DC778(void);

void func_800DC94C(void);

void func_800DC954(void);

void func_800DCD00(void);

void func_800DD2D4(void);

void func_800DD9E8(void);

void func_800DDEC8(void);

void func_800DE1FC(void);

void func_800DF1E8(void);

/** @brief Handles the Disk of Ouroboros item pickup event. */
void MapEvent_DiskOfOuroborosTake(void);

void func_800DF21C(void);

void func_800DFA54(void);

/** @brief Handles the Screwdriver item pickup event. */
void MapEvent_ScrewdriverTake(void);

void func_800DFAAC(void);

void func_800DFAE4(void);

/** @brief Handles the Amulet of Solomon item pickup event. */
void MapEvent_AmuletOfSolomonTake(void);

void func_800DFB48(void);

/** @brief Earlier version of `map7_s01::func_800D9C9C` using unfinished sfx IDs, possibly unused here. */
void func_800DFDDC(void);

/** @brief Handles the Crest of Mercury item pickup event. */
void MapEvent_CrestOfMercuryTake(void);

/** @brief Handles the Ring of Contract item pickup event. */
void MapEvent_RingOfContractTake(void);

/** @brief Handles the Camera item pickup event. */
void MapEvent_CameraTake(void);

void func_800E0CB4(void);

void func_800E0FF0(void);

void func_800E12E4(void);

void func_800E1398(void);

void func_800E1DAC(void);

void func_800E1FF4(void);

void func_800E2DC0(void);

void func_800E2DEC(void);

void func_800E32E0(void);

void Map_WorldObjectsInit(void);

void Map_WorldObjectsUpdate(void);

void func_800E5628(void);

#endif
