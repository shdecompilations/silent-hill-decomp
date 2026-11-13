#ifndef _MAPS_MAP1_S00_H
#define _MAPS_MAP1_S00_H

#define HAS_PlayerState_Reset
#define HAS_PlayerState_Unk87
#define HAS_PlayerState_Unk88

#define MAP_ROOM_MIN_X Q12(-160.0f)
#define MAP_ROOM_MAX_X Q12(160.0f)
#define MAP_ROOM_MIN_Z Q12(-80.0f)
#define MAP_ROOM_MAX_Z Q12(200.0f)

#define MAP_FIELD_4C_COUNT 200
#define MAP_BLOOD_SPLAT_COUNT_MAX 150

// JAP0 merges "A picture of a door." and the message after it into one.
#if VERSION_REGION_IS(NTSC)
    #define MAP_MSG(idx) (idx)
#elif VERSION_REGION_IS(NTSCJ)
    #define MAP_MSG(idx) ((idx) >= 28 ? (idx) - 1 : (idx))
#endif

#include "maps/common/shared.h"

extern s_BgmLayerLimits D_800DCC4C;

extern u16 D_800DCC54[42];

extern s_MapPoint2d MAP_POINTS[]; // 0x800DCCB8

extern s32 D_800DD718;

extern s_Pose           g_CommonWorldObjectPoses[5]; // 0x800DD71C

extern s_WorldObjectPlacement g_WorldObject0; // 0x800E0EF0

extern s_WorldObjectPlacement g_WorldObject1[6]; // 0x800E0F20

extern s_WorldObjectPlacement g_WorldObject2; // 0x800E1010

extern s_WorldObjectPlacement g_WorldObject3; // 0x800E1040

extern s_WorldObjectPlacement g_WorldObject4; // 0x800E1070

extern s_WorldObjectPlacement g_WorldObject5; // 0x800E10A0

extern s_WorldObjectPlacement g_WorldObject6[2]; // 0x800E10D0

extern s_WorldObjectPose       g_WorldObject7; // 0x800E1120

extern s_WorldObjectPose       g_WorldObject8; // 0x800E1150

extern u8 D_800E1180;

extern s16 D_800E1182;

extern s_WorldObjectPose g_WorldObject9; // 0x800E1190

extern s_WorldObjectModel   g_CommonWorldObjects[6]; // 0x800E11C0

void MapEvent_CommonItemTake(void);

void MapEvent_GoldenSunEmptyReceptableInspect(void);

void MapEvent_GoldenSunFilledReceptacleInspect(void);

void MapEvent_GoldenSunItemUse(void);

void MapEvent_SilverMoonEmptyReceptableInspect(void);

void MapEvent_SilverMoonFilledReceptacleInspect(void);

void MapEvent_SilverMoonItemUse(void);

void MapEvent_ClockTowerInspect(void);

void func_800D8354(void);

void func_800D853C(void);

void func_800D8570(void);

void func_800D85A4(void);

void func_800D85D8(void);

void func_800D8948(void);

void func_800D8CC4(void);

/** @brief Handles the town map pickup event. */
void MapEvent_TownMapTake(void);

void func_800D9254(void);

void func_800D928C(void);

void func_800DA3FC(void);

void MapEvent_Boiler0(void);

void MapEvent_Boiler1(void);

void MapEvent_Boiler2(void);

void Map_WorldObjectsInit(void);

void Map_WorldObjectsUpdate(void);

#endif
