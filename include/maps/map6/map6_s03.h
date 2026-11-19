#ifndef _MAPS_MAP6_S03_H
#define _MAPS_MAP6_S03_H

#define HAS_PlayerState_Reset
#define HAS_PlayerState_Unk87
#define HAS_PlayerState_Unk88

#define MAP_PARTICLE_COUNT_MAX 300
#define MAP_BLOOD_SPLAT_COUNT_MAX 150

#include "maps/common/shared.h"

extern s_Pose                 g_CommonWorldObjectPoses[3];
extern s_WorldObjectModel     g_WorldObject0[2];
extern VECTOR3                g_WorldObjectPos0;
extern s_WorldObjectPose      g_WorldObject1;
extern s_WorldObjectPlacement g_WorldObject2;
extern s_WorldObjectModel     g_CommonWorldObjects[6];

extern s16 D_800DFEF0;
extern s16 D_800DFEF2;
extern s8 D_800DFEF4;
extern s8 D_800DFEF5;
extern VECTOR3 D_800DBD10;
extern u16 D_800DBCE4[22]; // BGM flags for each room.
extern s_BgmLayerLimits g_BgmDRU_LayerLimits0;

/** @brief Handles a Health Drink or Handgun Bullets item pickup event. */
void MapEvent_CommonItemTake(void);

void func_800D822C(void);

void func_800D84EC(void);

/** @brief Handles the ? paper map pickup event. */
void MapEvent_PaperMapTake(void);

void Map_WorldObjectsInit(void);

void Map_WorldObjectsUpdate(void);

void func_800D936C(void);

#endif
