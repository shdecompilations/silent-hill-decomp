#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "main/rng.h"

// TODO: .bss is emitted in order of first declaration, right now our headers
// declare externs in semi-random order though.
// For now we'll declare them before headers in the order they appear in the
// map .bss, later can hopefully improve order in headers instead.
extern e_MapIdx                g_ParticleMapIdx0;
extern e_MapIdx                g_ParticleMapIdx1;
extern s32                     g_Particle_SpeedX;
extern s32                     g_Particle_SpeedZ;
extern s32                     sharedData_800DFB6C_0_s00;
extern s32                     sharedData_800DFB70_0_s00;
extern s32                     __pad_bss_800CCC44[2];
extern s_MapHdr_field_4C       sharedData_800DFB7C_0_s00[];
extern s_BloodSplat            g_Effect_BloodSplats[];
extern s8                      __pad_bss_800CD480[12];
extern struct _ParticleVectors g_ParticleVectors0;
extern s32                     __pad_bss_800CD4B8;
extern struct _ParticleVectors g_ParticleVectors1;
extern s32                     sharedData_800E0CA8_0_s00;
extern s32                     sharedData_800E0CAC_0_s00;
extern u32                     sharedData_800E0CB0_0_s00;
extern u16                     sharedData_800E0CB4_0_s00;
extern u16                     sharedData_800E0CB6_0_s00;
extern u16                     sharedData_800E0CB8_0_s00;
extern s8                      sharedData_800E0CBA_0_s00;
extern s8                      __pad_bss_800CD4FB;
extern s32                     sharedData_800E39D8_0_s00;
extern q19_12                  g_Player_MoveSpeed;

#include "maps/map4/map4_s00.h"
#include "maps/common/particle.h"
#include "maps/common/characters/player.h"

s32 __pad_bss_800CCC44[2];
s8  __pad_bss_800CD480[12];
s32 __pad_bss_800CD4B8;
s8  __pad_bss_800CD4FB;

#include "../src/maps/common/particles/particle.c"

#include "../src/maps/common/characters/player.c"

#include "../src/maps/common/map_util.c" // 0x800CC790

#include "maps/common/shared/Map_RoomBgmInit_2_s02.h" // 0x800CC99C

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
