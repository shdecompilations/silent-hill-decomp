#ifndef _MAPS_MAP3_S04_H
#define _MAPS_MAP3_S04_H

#define HAS_PlayerState_Unk51
#define HAS_PlayerState_Unk131
#define HAS_PlayerState_Reset
#define HAS_PlayerState_Unk59
#define HAS_PlayerState_Unk60
#define HAS_PlayerState_Unk125
#define KEYFRAME_PlayerState_Unk59 957
#define KEYFRAME_PlayerState_Unk60 972

#define HAS_Chara_Lisa
#define MAP_CHARA_COUNT 1

#define MAP_ROOM_MIN_X Q12(-160.0f)
#define MAP_ROOM_MAX_X Q12(160.0f)
#define MAP_ROOM_MIN_Z Q12(-160.0f)
#define MAP_ROOM_MAX_Z Q12(160.0f)

#define MAP_FIELD_4C_COUNT 200
#define MAP_BLOOD_SPLAT_COUNT_MAX 150

#include "maps/common/shared.h"

extern s_WorldObjectPlacement g_WorldObject_Dr[6];
extern s_WorldObjectPlacement g_WorldObject_Plate;
extern s_WorldObjectPose      g_WorldObject_SavePad;
extern s_WorldObjectModel     g_CommonWorldObjects[6];
extern s_Pose      g_CommonWorldObjectPoses[2];

extern u16 D_800D599C;
extern VECTOR3 g_Cutscene_CameraPositionTarget;
extern VECTOR3 g_Cutscene_CameraLookAtTarget;
extern u8 D_800D5A3C;
extern q19_12 g_Cutscene_Timer;
extern s16 D_800D5A48[3];
extern s16 D_800D5A4E;

void func_800D21C4(void);

void func_800D2470(void);

void func_800D250C(void);

void func_800D2668(void);

void func_800D2E58(void);

void Map_WorldObjectsInit(void);

void Map_WorldObjectsUpdate(void);

void func_800D43B8(void);

#endif
