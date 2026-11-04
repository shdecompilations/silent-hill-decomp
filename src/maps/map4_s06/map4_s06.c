#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "main/rng.h"
#include "maps/map4/map4_s06.h"
#include "maps/common/particle.h"
#include "maps/common/characters/player.h"

#include "../src/maps/common/particles/particle.c"

#include "../src/maps/common/characters/player.c"

#include "../src/maps/common/map_util.c" // 0x800CC790

#include "maps/common/shared/Map_RoomBgmInit_2_s02.h" // 0x800CC99C

void GameBoot_LoadScreen_StageString(void) {}

void Map_WorldObjectsInit(void) {}

void Map_WorldObjectsUpdate(void) {}

const char* MAP_MESSAGES[] = {
#include "maps/common/shared/map_msg_common.h"
#if VERSION_REGION_IS(NTSC)
    "\tNO_STAGE! ~E "
#elif VERSION_REGION_IS(NTSCJ)
    "\tこのステージは存在しません ~E "
#endif
};
