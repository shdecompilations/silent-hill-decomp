#ifndef _MAPS_MAP0_S02_H
#define _MAPS_MAP0_S02_H

#define HAS_PARTICLE_CASE_11
#define PARTICLE_CASE_COUNT 1

// Config used by `Map_RoomIdxGet`.
#define MAP_ROOM_MIN_X Q12(-200.0f)
#define MAP_ROOM_MAX_X Q12(200.0f)
#define MAP_ROOM_MIN_Z Q12(0.0f)
#define MAP_ROOM_MAX_Z Q12(120.0f)

#define MAP_FIELD_4C_COUNT 100
#define MAP_BLOOD_SPLAT_COUNT_MAX 50

#include "maps/common/shared.h"

/** @brief Handles a First Aid Kit, Health Drink, or Handgun Bullets item pickup event. */
void MapEvent_CommonItemTake(void);

void MapEvent_EmptyFunction(void);

/** @brief Handles the Gasoline Tank item pickup event. */
void MapEvent_GasolineTankItemTake(void);

/** @brief Handles the Katana item pickup event. */
void MapEvent_KatanaItemTake(void);

/** @brief Handles the Channeling Stone item pickup event. */
void MapEvent_ChannelingStoneItemTake(void);

void Map_WorldObjectsInit(void);

void Map_WorldObjectsUpdate(void);

#endif
