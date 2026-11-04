#ifndef _MAPS_MAP4_S06_H
#define _MAPS_MAP4_S06_H

#define MAP_ROOM_MIN_X Q12(-240.0f)
#define MAP_ROOM_MAX_X Q12(240.0f)
#define MAP_ROOM_MIN_Z Q12(-120.0f)
#define MAP_ROOM_MAX_Z Q12(240.0f)

#define MAP_FIELD_4C_COUNT 100
#define MAP_BLOOD_SPLAT_COUNT_MAX 50

#include "maps/common/shared.h"

void Map_WorldObjectsInit(void);

void Map_WorldObjectsUpdate(void);

#endif
