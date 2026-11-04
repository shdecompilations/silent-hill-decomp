#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "main/rng.h"
#include "maps/map0/map0_s02.h"
#include "maps/particle.h"
#include "maps/characters/player.h"

#include "../src/maps/particles/particle.c"

#include "../src/maps/characters/player.c"

#include "../src/maps/map_util.c" // 0x800CE7DC

#include "maps/shared/Map_RoomBgmInit_CheckCond.h" // 0x800CE884

#include "maps/shared/Map_RoomBgmInit_0_s02_CondTrue.h" // 0x800CE8B8

#include "maps/shared/Map_RoomBgmInit_0_s02_CondFalse.h" // 0x800CE934

void GameBoot_LoadScreen_StageString(void) {}
