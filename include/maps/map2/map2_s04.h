#ifndef _MAPS_MAP2_S04_H
#define _MAPS_MAP2_S04_H

// Config used by `Map_RoomIdxGet`.
#define MAP_ROOM_MIN_X Q12(-200.0f)
#define MAP_ROOM_MAX_X Q12(200.0f)
#define MAP_ROOM_MIN_Z Q12(0.0f)
#define MAP_ROOM_MAX_Z Q12(120.0f)

#define MAP_FIELD_4C_COUNT 200
#define MAP_BLOOD_SPLAT_COUNT_MAX 150

// JAP0 merges "Coroner Seals called." into the message after it, so messages afterward are all indexed one lower than in USA.
#if VERSION_REGION_IS(NTSC)
    #define MAP_MSG(idx) (idx)
#elif VERSION_REGION_IS(NTSCJ)
    #define MAP_MSG(idx) ((idx) >= 17 ? (idx) - 1 : (idx))
#endif

#include "maps/common/shared.h"

void func_800CCB30(s32 arg0);

void MapEvent_DoorJammed(void);

void MapEvent_DoorLocked(void);

void func_800CCF9C(void);

void func_800CD050(void);

void MapEvent_PoliceChalkboardInspect(void);

/** @brief Handles the ? map pickup event. */
void MapEvent_PaperMapTake(void);

/** @brief Handles the Shotgun item pickup event. */
void MapEvent_ShotgunTake(void);

void Map_WorldObjectsInit(void);

void Map_WorldObjectsUpdate(void);

#endif
