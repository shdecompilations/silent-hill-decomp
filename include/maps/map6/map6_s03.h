#ifndef _MAPS_MAP6_S03_H
#define _MAPS_MAP6_S03_H

#define HAS_PlayerState_Reset
#define HAS_PlayerState_Unk87
#define HAS_PlayerState_Unk88

#define MAP_FIELD_4C_COUNT 300
#define MAP_BLOOD_SPLAT_COUNT_MAX 150

#include "maps/common/shared.h"

extern s_Pose      g_CommonWorldObjectPoses[3]; // D_800DBEE4
extern s_WorldObjectModel        g_WorldObject0[2];           // D_800DFE70
extern VECTOR3                g_WorldObjectPos0;           // D_800DFEB0
extern s_WorldObjectPose      g_WorldObject1;              // D_800DFEC0
extern s_WorldObjectPlacement g_WorldObject2;              // D_800DFF00
extern s_WorldObjectModel        g_CommonWorldObjects[6];     // D_800DFF30

extern s16 D_800DFEF0;
extern s16 D_800DFEF2;
extern s8 D_800DFEF4;
extern s8 D_800DFEF5;
extern VECTOR3 D_800DBD10;
extern u16 D_800DBCE4[22]; // Flags.
extern s_BgmLayerLimits D_800DBCDC;   // Distances.

void func_800D7F98(void);

/** @brief Handles a Health Drink or Handgun Bullets item pickup event. */
void MapEvent_CommonItemTake(void);

void func_800D822C(void);

void func_800D84EC(void);

/** @brief Handles the ? map pickup event. */
void MapEvent_PaperMapTake(void);

void Map_WorldObjectsInit(void);

void Map_WorldObjectsUpdate(void);

void func_800D936C(void);

#endif
