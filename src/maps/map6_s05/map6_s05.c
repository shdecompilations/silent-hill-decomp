#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "main/rng.h"
#include "maps/map6/map6_s05.h"
#include "maps/common/particle.h"
#include "maps/common/characters/player.h"

#include "../src/maps/common/particles/particle.c"

#include "../src/maps/common/characters/player.c"

#include "../src/maps/common/map_util.c" // 0x800CC7A4

#include "maps/common/shared/Map_RoomBgmInit_6_s04.h" // 0x800CC8F8

#include "maps/common/shared/Map_RoomBgmInit_6_s04_CondTrue.h" // 0x800CC930

#include "maps/common/shared/Map_RoomBgmInit_6_s04_CondFalse.h" // 0x800CC970

// TODO: Might be part of shared block above with `map6_s04::func_800E155C`
void GameBoot_LoadScreen_StageString(void) {}

const char* MAP_MESSAGES[] = {
#include "maps/common/shared/map_msg_common.h"
#if VERSION_REGION_IS(NTSC)
    "\tNO_STAGE! ~E "
#elif VERSION_REGION_IS(NTSCJ)
    "\tこのステージは存在しません ~E "
#endif
};

