#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "main/rng.h"

// TODO: .bss is emitted in order of first declaration, right now our headers
// declare externs in semi-random order though.
// For now we'll declare them before headers in the order they appear in the
// map .bss, later can hopefully improve order in headers instead.
extern e_MapIdx                g_WeatherParticle_MapIdx0;
extern e_MapIdx                g_WeatherParticle_MapIdx1;
extern s32                     g_WeatherParticle_SpeedX;
extern s32                     g_WeatherParticle_SpeedZ;
extern s32                     sharedData_800DFB6C_0_s00;
extern s32                     sharedData_800DFB70_0_s00;
extern s_Particle       g_Particle_Buffer[];
extern s_BloodSplat            g_Particle_BloodSplats[];
extern struct _WeatherParticleVectors g_WeatherParticle_Vectors0;
extern struct _WeatherParticleVectors g_WeatherParticle_Vectors1;
extern s32                     sharedData_800E0CA8_0_s00;
extern s32                     sharedData_800E0CAC_0_s00;
extern u32                     sharedData_800E0CB0_0_s00;
extern u16                     sharedData_800E0CB4_0_s00;
extern u16                     sharedData_800E0CB6_0_s00;
extern u16                     sharedData_800E0CB8_0_s00;
extern s8                      sharedData_800E0CBA_0_s00;
extern s32                     sharedData_800E39D8_0_s00;
extern q19_12                  g_Player_MoveSpeed;

#include "maps/map4/map4_s06.h"
#include "maps/common/particle.h"
#include "maps/common/characters/player.h"

#include "../src/maps/common/particles/particle.c"

#include "../src/maps/common/characters/player.c"

#include "../src/maps/common/map_util.c" // 0x800CC790

#include "../src/maps/common/bgm/bgm_SPR.c" // 0x800CC99C

void GameBoot_LoadScreen_StageString(void) {}

void Map_WorldObjectsInit(void) {}

void Map_WorldObjectsUpdate(void) {}

const char* MAP_MESSAGES[] = {
#include "maps/common/shared/map_msg_common.inc"
#if VERSION_REGION_IS(NTSC)
    "\tNO_STAGE! ~E "
#elif VERSION_REGION_IS(NTSCJ)
    "\tこのステージは存在しません ~E "
#endif
};

s_EventData MAP_EVENTS[] = {
#include "map_events.h"
};
