#ifndef _MAPS_MAP1_S05_H
#define _MAPS_MAP1_S05_H

#define MAP_ROOM_MIN_X Q12(-160.0f)
#define MAP_ROOM_MAX_X Q12(160.0f)
#define MAP_ROOM_MIN_Z Q12(-80.0f)
#define MAP_ROOM_MAX_Z Q12(200.0f)

#define MAP_FIELD_4C_COUNT 500
#define MAP_BLOOD_SPLAT_COUNT_MAX 150

#include "maps/common/shared.h"

extern s32 D_800D587C;

extern s_BgmLayerLimits D_800D5C3C;

extern u8 D_800D5D11;

extern s_WorldObjectPose g_WorldObject0; // 0x800D8698
extern s_WorldObjectPose g_WorldObject1; // 0x800D86C8

extern s16 D_800D86F8[2];
extern u8  D_800D86FC[2];
extern u8  D_800D86FE;

void func_800D49AC(void);

void func_800D4D1C(void);

void Map_WorldObjectsInit(void);

void Map_WorldObjectsUpdate(void);

#endif
