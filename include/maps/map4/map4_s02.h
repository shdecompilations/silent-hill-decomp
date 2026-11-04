#ifndef _MAPS_MAP4_S02_H
#define _MAPS_MAP4_S02_H

#define MAP_ROOM_MIN_X Q12(-240.0f)
#define MAP_ROOM_MAX_X Q12(240.0f)
#define MAP_ROOM_MIN_Z Q12(-120.0f)
#define MAP_ROOM_MAX_Z Q12(240.0f)

#define MAP_FIELD_4C_COUNT 200
#define MAP_BLOOD_SPLAT_COUNT_MAX 150

#include "maps/common/shared.h"

extern s_WorldObjectPlacement g_WorldObject_Kidn04;

extern s_WorldObjectModel g_WorldObject_Fan0;

extern u8 D_800F13AC;

extern u16 D_800ED8B0[16];

void func_800EA338(void);

void Map_WorldObjectsInit(void);

void Map_WorldObjectsUpdate(void);

#endif
