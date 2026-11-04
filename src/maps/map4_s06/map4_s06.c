#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "main/rng.h"
#include "maps/map4/map4_s06.h"
#include "maps/particle.h"
#include "maps/characters/player.h"

#include "../src/maps/particles/particle.c"

#include "../src/maps/characters/player.c"

#include "../src/maps/map_util.c" // 0x800CC790

#include "maps/shared/Map_RoomBgmInit_2_s02.h" // 0x800CC99C

void GameBoot_LoadScreen_StageString(void) {}

void Map_WorldObjectsInit(void) {}

void Map_WorldObjectsUpdate(void) {}

const char* MAP_MESSAGES[] = {
#include "maps/shared/map_msg_common.h"
#if VERSION_REGION_IS(NTSC)
    "\tNO_STAGE! ~E "
#elif VERSION_REGION_IS(NTSCJ)
    "\tこのステージは存在しません ~E "
#endif
};
