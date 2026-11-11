#ifndef _MAPS_MAP3_S02_H
#define _MAPS_MAP3_S02_H

#define MAP_ROOM_MIN_X Q12(-160.0f)
#define MAP_ROOM_MAX_X Q12(160.0f)
#define MAP_ROOM_MIN_Z Q12(-160.0f)
#define MAP_ROOM_MAX_Z Q12(160.0f)

#define MAP_FIELD_4C_COUNT 200
#define MAP_BLOOD_SPLAT_COUNT_MAX 150

#include "maps/common/shared.h"

extern s32 D_800D1FEC;

extern u8 D_800D3154;

void func_800D02AC(void);

void func_800D02B4(void);

void func_800D03FC(void);

void func_800D0608(void);

void Map_WorldObjectsInit(void);

void Map_WorldObjectsUpdate(void);

s16 func_800D1354(void);

#endif
