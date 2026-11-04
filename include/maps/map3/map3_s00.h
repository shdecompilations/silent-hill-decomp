#ifndef _MAPS_MAP3_S00_H
#define _MAPS_MAP3_S00_H

#define HAS_PlayerState_Unk51
#define HAS_PlayerState_Unk131
#define HAS_PlayerState_Reset
#define HAS_PlayerState_Unk72
#define HAS_PlayerState_Unk115

#define HAS_Chara_Kaufmann
#define MAP_CHARA_COUNT 1

#define HAS_PARTICLE_CASE_2
#define PARTICLE_CASE_COUNT 1

#define MAP_ROOM_MIN_X Q12(-40.0f)
#define MAP_ROOM_MAX_X Q12(160.0f)
#define MAP_ROOM_MIN_Z Q12(-80.0f)
#define MAP_ROOM_MAX_Z Q12(160.0f)

#define MAP_FIELD_4C_COUNT 200
#define MAP_BLOOD_SPLAT_COUNT_MAX 150

#include "maps/common/shared.h"

extern s_WorldObjectPlacement g_WorldObject0;
extern s_WorldObjectPlacement g_WorldObject1[2];
extern s_WorldObjectPose      g_WorldObject2;
extern s_WorldObjectPose      g_WorldObject3;
extern s_WorldObjectPose      g_WorldObject4;
extern s_WorldObjectPose      g_WorldObject5;

extern VECTOR3 D_800CB2E4;
extern VECTOR3 D_800CB2F0;
extern VECTOR3 D_800CB2FC;
extern VECTOR3 D_800CB308;
extern u16 D_800D24F0;
extern VECTOR3 D_800D2530;

extern q19_12  g_Cutscene_Timer;
extern VECTOR3 g_Cutscene_CameraPositionTarget;
extern VECTOR3 g_Cutscene_CameraLookAtTarget;

extern u8 D_800D256C;

void func_800CCE88();

void func_800D0994();

void func_800D09D4();

void func_800D0CA4();

/** @brief First Aid Kit item pickup event. */
void MapEvent_FirstAidKitTake(void);

/** @brief Handles the ? map pickup event. */
void MapEvent_PaperMapTake(void);

void func_800D0CF8(void);

void Map_WorldObjectsInit(void);

void Map_WorldObjectsUpdate(void);

#endif
