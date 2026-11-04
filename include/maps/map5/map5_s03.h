#ifndef _MAPS_MAP5_S03_H
#define _MAPS_MAP5_S03_H

#define HAS_PlayerState_Unk51
#define HAS_PlayerState_Unk131
#define HAS_PlayerState_Reset
#define HAS_PlayerState_Unk59
#define HAS_PlayerState_Unk60
#define HAS_PlayerState_Unk69
#define HAS_PlayerState_Unk105
#define HAS_PlayerState_Unk159
#define HAS_PlayerState_Unk169
#define HAS_PlayerState_Unk187
#define KEYFRAME_PlayerState_Unk59 801
#define KEYFRAME_PlayerState_Unk60 816
#define KEYFRAME_PlayerState_Unk69 828

#define HAS_Chara_Kaufmann
#define MAP_CHARA_COUNT 1

#define MAP_ROOM_MIN_X Q12(-200.0f)
#define MAP_ROOM_MAX_X Q12(200.0f)
#define MAP_ROOM_MIN_Z Q12(0.0f)
#define MAP_ROOM_MAX_Z Q12(120.0f)

#define MAP_FIELD_4C_COUNT 150
#define MAP_BLOOD_SPLAT_COUNT_MAX 50

#if VERSION_REGION_IS(NTSC)
    #define MAP_MSG(idx) (idx)
#elif VERSION_REGION_IS(NTSCJ)
    #define MAP_MSG(idx) ((idx) >= 58 ? (idx) - 2 : (idx) >= 20 ? (idx) - 1 : (idx))
#endif

#include "maps/common/shared.h"

extern s_Pose      g_WorldObjectPose_HealthDrink0;
extern s_Pose      g_WorldObjectPose_ShotgunShells;
extern s_Pose      g_WorldObjectPose_HealthDrink1;
extern s_WorldObjectPlacement g_WorldObject_Movaches;
extern s32                    D_800D6F8C;
extern s_WorldObjectModel     g_WorldObject_SavePad;
extern s_Pose      g_WorldObjectPose_SavePad[2];
extern s_WorldObjectPose      g_WorldObject_Mag;
extern s_WorldObjectModel     g_CommonWorldObjects[6];
extern s_WorldObjectModel     g_WorldObject_HealthDrink;
extern s_WorldObjectModel     g_WorldObject_ShotgunShells;

extern u16 D_800D3BDC;

extern s32 D_800D3C40;

extern q3_12 D_800D3C44; // Time.

extern VECTOR3 g_Cutscene_CameraPositionTarget;
extern VECTOR3 g_Cutscene_CameraLookAtTarget;

extern u8 D_800D6F54;

extern q19_12 g_Cutscene_Timer;

void func_800D1004(s32 arg0);

/** @brief Handles a Health Drink or Shotgun Shells item pickup event. */
void MapEvent_CommonItemTake(void);

void func_800D1628(void);

void func_800D1904(void);

void func_800D1A84(void);

void func_800D1ACC(void);

void func_800D1AF8(void);

void func_800D2640(void);

void func_800D2674(void);

void Map_WorldObjectsInit(void);

void Map_WorldObjectsUpdate(void);

#endif
