#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "main/rng.h"
#include "maps/map2/map2_s03.h"
#include "maps/particle.h"
#include "maps/characters/player.h"

#include "../src/maps/particles/particle.c"

#include "../src/maps/characters/player.c"

#include "../src/maps/map_util.c" // 0x800CC7C0

#include "maps/shared/Map_RoomBgmInit_2_s00.h" // 0x800CCA1C

void GameBoot_LoadScreen_StageString(void) {} // 0x800CCB24

void Map_WorldObjectsInit(void) {} // 0x800CCB2C

void Map_WorldObjectsUpdate(void) {} // 0x800CCB34

const char* MAP_MESSAGES[] = {
#include "maps/shared/map_msg_common.h"
#if VERSION_REGION_IS(NTSC)
    "\tNO_STAGE! ~E "
#elif VERSION_REGION_IS(NTSCJ)
    "\tこのステージは存在しません ~E "
#endif
};

