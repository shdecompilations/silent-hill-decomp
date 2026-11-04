#ifndef _MAPS_MAP3_S01_H
#define _MAPS_MAP3_S01_H

#define HAS_PlayerState_Reset
#define HAS_PlayerState_Unk59
#define HAS_PlayerState_Unk60
#define KEYFRAME_PlayerState_Unk59 957
#define KEYFRAME_PlayerState_Unk60 972

#define HAS_PARTICLE_CASE_2
#define PARTICLE_CASE_COUNT 1

#define MAP_ROOM_MIN_X Q12(-40.0f)
#define MAP_ROOM_MAX_X Q12(160.0f)
#define MAP_ROOM_MIN_Z Q12(-80.0f)
#define MAP_ROOM_MAX_Z Q12(160.0f)

#define MAP_FIELD_4C_COUNT 200
#define MAP_BLOOD_SPLAT_COUNT_MAX 150

#include "maps/common/shared.h"

extern s_WorldObjectPlacement    g_WorldObject_Dr[6];
extern s_WorldObjectPlacement g_WorldObject1;
extern s_WorldObjectPose      g_WorldObject2;
extern s_WorldObjectPlacement g_WorldObject3[2];
extern s_WorldObjectPose      g_WorldObject5;
extern s_WorldObjectPose      g_WorldObject6;
extern s_WorldObjectPose      g_WorldObject7;
extern s_WorldObjectPose      g_WorldObject8;
extern s_WorldObjectPose      g_WorldObject9;
extern s_WorldObjectModel        g_WorldObjectA;
extern s_Pose      g_WorldObjectAPos;
extern s_WorldObjectModel        g_CommonWorldObjects[6];

extern VECTOR3 D_800D4CE4; // Used by `func_800D2E54` only. For some reason it's not in RODATA.
extern u8 g_GeneratorMakeNoise; /** `bool` */
extern u8 D_800D8734;

void func_800D14BC(void);

void func_800D1500(void);

void func_800D1524(void);

void MapEvent_UnknownLiquidInteract(void);

/** @brief Handles the Plastic Bottle item pickup event. */
void MapEvent_PlasticBottleTake(void);

/** @brief Handles the ? map pickup event. */
void MapEvent_PaperMapTake0(void);

/** @brief Handles the ? map pickup event. */
void MapEvent_PaperMapTake1(void);

void func_800D279C(void);

void MapEvent_Generator0(void);

void func_800D29A4(void);

void func_800D2A88(void);

void Map_WorldObjectsInit(void);

void Map_WorldObjectsUpdate(void);

void MapEvent_UseBottleOnLiquid(void);

#endif
