#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "main/rng.h"
#include "maps/map0/map0_s02.h"
#include "maps/common/particle.h"
#include "maps/common/characters/player.h"

#include "../src/maps/common/particles/particle.c"

#include "../src/maps/common/characters/player.c"

#include "../src/maps/common/map_util.c" // 0x800CE7DC

#include "maps/common/shared/Map_RoomBgmInit_CheckCond.h" // 0x800CE884

#include "maps/common/shared/Map_RoomBgmInit_0_s02_CondTrue.h" // 0x800CE8B8

#include "maps/common/shared/Map_RoomBgmInit_0_s02_CondFalse.h" // 0x800CE934

void GameBoot_LoadScreen_StageString(void) {}
