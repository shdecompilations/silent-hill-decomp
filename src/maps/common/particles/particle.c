#include "inline_no_dmpsx.h"

#include "bodyprog/bodyprog.h"
#include "bodyprog/events/event_data.h"
#include "bodyprog/world/effects.h"
#include "bodyprog/math/math.h"
#include "main/rng.h"
#include "maps/common/shared.h"
#include "maps/common/particle.h"

// TODO: Find why removing these includes causes mismatch.
#if defined(MAP0_S00)
    #include "maps/map0/map0_s00.h" // TODO: Move particle-related decls to particle.h
#elif defined(MAP7_S03)
    #include "maps/map7/map7_s03.h" // TODO: Move particle-related decls to particle.h
#elif defined(MAP0_S02)
    #include "maps/map0/map0_s02.h"
#elif defined(MAP2_S00)
    #include "maps/map2/map2_s00.h"
#elif defined(MAP2_S02)
    #include "maps/map2/map2_s02.h"
#elif defined(MAP3_S00)
    #include "maps/map3/map3_s00.h"
#elif defined(MAP3_S01)
    #include "maps/map3/map3_s01.h"
#elif defined(MAP5_S01)
    #include "maps/map5/map5_s01.h"
#endif

#if defined(MAP1_S04) || defined(MAP1_S05) || defined(MAP2_S01) || defined(MAP2_S03) || defined(MAP2_S04) || defined(MAP3_S02) || defined(MAP4_S00) || defined(MAP4_S06) || defined(MAP6_S05)
// .bss data in maps we've matched .bss for
// As more maps get matched we can hopefully find proper order for these.
e_MapIdx          g_ParticleMapIdx0;
e_MapIdx          g_ParticleMapIdx1;
s32               g_Particle_SpeedX;
s32               g_Particle_SpeedZ;
s32               sharedData_800DFB6C_0_s00;
s32               sharedData_800DFB70_0_s00;
s_MapHdr_field_4C sharedData_800DFB7C_0_s00[MAP_FIELD_4C_COUNT];
s_BloodSplat      g_Effect_BloodSplats[MAP_BLOOD_SPLAT_COUNT_MAX];
s_ParticleVectors g_ParticleVectors0;
s_ParticleVectors g_ParticleVectors1;
s32               sharedData_800E0CA8_0_s00;
s32               sharedData_800E0CAC_0_s00;
u32               sharedData_800E0CB0_0_s00;
u16               sharedData_800E0CB4_0_s00;
u16               sharedData_800E0CB6_0_s00;
u16               sharedData_800E0CB8_0_s00;
s8                sharedData_800E0CBA_0_s00;
#endif

// Particle-related functions.
//
// TODO:
//  - This should be a separate split in each map overlay, but for now #including this .c works.
//  - sharedData and structs that are only used in this file need to be moved from `shared.h` into `particle.h`
//  - Some funcs here only get included in MAP0_S00, guess they're particle-related since they're between
//    other particle functions, does MAP0_S00 have some unique particle effect?

#define MAP_PARTICLE_HAS_SNOW \
    (defined(MAP0_S00) || defined(MAP0_S01) || defined(MAP0_S02) || defined(MAP1_S00) || \
    defined(MAP1_S06) || defined(MAP2_S00) || defined(MAP2_S02) || defined(MAP3_S00) || \
    defined(MAP3_S01) || defined(MAP3_S06) || defined(MAP5_S01) || defined(MAP5_S03) || \
    defined(MAP6_S00) || defined(MAP7_S03))

#define MAP_PARTICLE_HAS_RAIN \
    (defined(MAP0_S00) || defined(MAP1_S02) || defined(MAP1_S03) || defined(MAP4_S02) || \
    defined(MAP4_S03) || defined(MAP4_S04) || defined(MAP4_S05) || defined(MAP5_S00) || \
    defined(MAP6_S00) || defined(MAP6_S03))

#define MAP_USE_PARTICLES (MAP_PARTICLE_HAS_SNOW || MAP_PARTICLE_HAS_RAIN)
#define MAP_PARTICLE_HAS_800D0690 (MAP_PARTICLE_HAS_RAIN && !defined(MAP0_S00) && !defined(MAP1_S02) && !defined(MAP4_S03))

// VRAM position of the particle sprite cells and their CLUTs, which PAL moved.
#if VERSION_REGION_IS(PAL)
    #define PARTICLE_TEX_U  128
    #define PARTICLE_TEX_V  112
    #define PARTICLE_CLUT_X 944
    #define PARTICLE_CLUT_Y 496
#else
    #define PARTICLE_TEX_U  0
    #define PARTICLE_TEX_V  240
    #define PARTICLE_CLUT_X 192
    #define PARTICLE_CLUT_Y 0
#endif

/** @brief Sets the UVs of `poly` to the 16x16 particle cell at `u` pixels into the particle row.
 * The right edge stops at 255, the last cell of the PAL row ends at the edge of the texture page.
 */
#define setParticleUV4(poly, u)                                                                \
    setUV4(poly,                                                                               \
           PARTICLE_TEX_U + (u), PARTICLE_TEX_V, PARTICLE_TEX_U + (u), PARTICLE_TEX_V + 15,      \
           MIN(PARTICLE_TEX_U + (u) + 16, 255), PARTICLE_TEX_V,                                \
           MIN(PARTICLE_TEX_U + (u) + 16, 255), PARTICLE_TEX_V + 15)

#if !MAP_USE_PARTICLES

/** Barebones version of `Particle_SystemUpdate`, missing calls to `Particle_Update` and other particle-related code. */
void Particle_SystemUpdate(s32 unused, e_MapIdx mapIdx, s32 arg3)
{
    s32 temp_s0;

    WorldEnvRender_LightPositionGet(&g_ParticleVectors0.vector_0);
    g_ParticleVectors0.field_28 = WorldEnvRender_LightRotationAndIntensityGet(&g_ParticleVectors0.svec_18);

    vwGetViewPosition(&g_ParticleVectors0.viewPosition);
    vwGetViewAngle(&g_ParticleVectors0.viewRotation);

    switch (arg3)
    {
        case 0:
        case -1:
            if (arg3 == NO_VALUE)
            {
                g_SysWork.field_2349 = g_MapOverlayHdr.field_17;
                g_SysWork.field_2348 = g_MapOverlayHdr.field_17;
            }

            sharedData_800DD598_0_s00 = 0;
            g_SysWork.field_234A      = true;
            g_ParticleMapIdx0 = mapIdx;

            sharedData_800E0CB8_0_s00 = FP_FROM(sharedData_800E0CB0_0_s00, Q12_SHIFT);
            sharedData_800E0CB6_0_s00 = sharedData_800E0CB0_0_s00;
            sharedData_800E0CB4_0_s00 = sharedData_800E0CB0_0_s00;

            temp_s0 = SetSp(PSX_SCRATCH_ADDR(0x3D8));
            SetSp(temp_s0);
            break;

        default:
            sharedData_800DD584_0_s00 = g_DeltaTime == Q12(0.0f);

            g_ParticleMapIdx1 = mapIdx;

            temp_s0 = SetSp(PSX_SCRATCH_ADDR(0x3D8));
            SetSp(temp_s0);
            break;
    }

    g_ParticleVectors1 = g_ParticleVectors0;
}

#else /* MAP_USE_PARTICLES */

/** Full version of `Particle_SystemUpdate` used by most maps. */
void Particle_SystemUpdate(s32 arg1, e_MapIdx mapIdx, s32 arg3)
{
    s32 temp_a2;
    s32 temp_s0;
    s32 temp_s0_2;
    s32 temp_s0_3;
    s32 temp_v0;
    s32 i;
    s32 var_v0;
    s32 var_v0_3;
    u32 temp_t0;
    u32 temp_v1_2;
    u16 temp_v1_3;

#if defined(MAP5_S01)
    if (g_MapEventParam == 7 && g_SysWork.sysStateSteps[0] == 8)
    {
        return;
    }
#endif

#if MAP_PARTICLE_HAS_RAIN
    Particle_SoundUpdate();
#endif

    WorldEnvRender_LightPositionGet(&g_ParticleVectors0.vector_0);
    g_ParticleVectors0.field_28 = WorldEnvRender_LightRotationAndIntensityGet(&g_ParticleVectors0.svec_18);

    vwGetViewPosition(&g_ParticleVectors0.viewPosition);
    vwGetViewAngle(&g_ParticleVectors0.viewRotation);

#if defined(MAP5_S01)
    g_Particle_SpeedX = 0;
#endif

    switch (arg3)
    {
        case 0:
        case -1:
#if defined(MAP0_S00)
            sharedData_800DD591_0_s00 = 1;

            g_ParticleBox.corners[0].vx = Q12(-5.0f);
            g_ParticleBox.corners[0].vy = Q12(-4.5f);
            g_ParticleBox.corners[0].vz = Q12(211.0f);

            g_ParticleBox.corners[1].vx = Q12(15.0f);
            g_ParticleBox.corners[1].vy = Q12(-4.5f);
            g_ParticleBox.corners[1].vz = Q12(211.0f);
#elif defined(MAP0_S01)
            sharedData_800DD591_0_s00 = 1;

            g_ParticleBox.corners[0].vx = Q12(-0.30f);
            g_ParticleBox.corners[0].vz = Q12(275.0f);
            g_ParticleBox.corners[1].vx = Q12(-0.30f);
            g_ParticleBox.corners[1].vz = Q12(265.0f);
#elif defined(MAP1_S06)
            switch (g_SavegamePtr->mapRoomIdx)
            {
                case 12:
                    sharedData_800DD591_0_s00             = 1;
                    g_ParticleBox.corners[0].vx = Q12(105.5f);
                    g_ParticleBox.corners[0].vz = Q12(67.0f);
                    g_ParticleBox.corners[1].vx = Q12(105.5f);
                    g_ParticleBox.corners[1].vz = Q12(50.0f);
                    break;

                case 13:
                    sharedData_800DD591_0_s00             = 1;
                    g_ParticleBox.corners[0].vx = Q12(145.0f);
                    g_ParticleBox.corners[0].vz = Q12(24.0f);
                    g_ParticleBox.corners[1].vx = Q12(132.0f);
                    g_ParticleBox.corners[1].vz = Q12(24.0f);
                    break;

                case 9:
                    sharedData_800DD591_0_s00             = 1;
                    g_ParticleBox.corners[0].vx = Q12(55.0f);
                    g_ParticleBox.corners[0].vz = Q12(137.0f);
                    g_ParticleBox.corners[1].vx = Q12(62.0f);
                    g_ParticleBox.corners[1].vz = Q12(137.0f);
                    break;

                case 14:
                    sharedData_800DD591_0_s00             = 2;
                    g_ParticleBox.corners[0].vx = Q12(96.5f);
                    g_ParticleBox.corners[0].vz = Q12(15.0f);
                    g_ParticleBox.corners[1].vx = Q12(103.5f);
                    g_ParticleBox.corners[1].vz = Q12(15.0f);
                    break;
            }
#elif defined(MAP2_S00)
            switch (g_SavegamePtr->mapRoomIdx)
            {
                case 38:
                    sharedData_800DD591_0_s00                 = 10;
                    g_ParticleBox.corners[0].vx = Q12(120.0f);
                    g_ParticleBox.corners[0].vz = Q12(303.5f);
                    g_ParticleBox.corners[1].vx = Q12(110.0f);
                    g_ParticleBox.corners[1].vz = Q12(303.5f);
                    g_ParticleBox.corners[2].vx = Q12(128.3f);
                    g_ParticleBox.corners[2].vz = Q12(295.0f);
                    g_ParticleBox.corners[3].vx = Q12(128.3f);
                    g_ParticleBox.corners[3].vz = Q12(305.0f);
                    g_ParticleBox.corners[4].vx = Q12(111.3f);
                    g_ParticleBox.corners[4].vz = Q12(298.0f);
                    g_ParticleBox.corners[5].vx = Q12(111.3f);
                    g_ParticleBox.corners[5].vz = Q12(295.0f);
                    break;

                case 23:
                    sharedData_800DD591_0_s00                 = 1;
                    g_ParticleBox.corners[0].vx = Q12(-186.0f);
                    g_ParticleBox.corners[0].vz = Q12(301.0f);
                    g_ParticleBox.corners[1].vx = Q12(-188.0f);
                    g_ParticleBox.corners[1].vz = Q12(301.0f);
                    break;

                case 37:
                    sharedData_800DD591_0_s00                 = 9;
                    g_ParticleBox.corners[0].vx = Q12(121.0f);
                    g_ParticleBox.corners[0].vz = Q12(223.0f);
                    g_ParticleBox.corners[1].vx = Q12(114.0f);
                    g_ParticleBox.corners[1].vz = Q12(223.0f);
                    g_ParticleBox.corners[2].vx = Q12(128.3f);
                    g_ParticleBox.corners[2].vz = Q12(215.0f);
                    g_ParticleBox.corners[3].vx = Q12(128.3f);
                    g_ParticleBox.corners[3].vz = Q12(220.0f);
                    break;

                case 40:
                    sharedData_800DD591_0_s00                 = 11;
                    g_ParticleBox.corners[0].vx = Q12(-48.0f);
                    g_ParticleBox.corners[0].vz = Q12(349.0f);
                    g_ParticleBox.corners[1].vx = Q12(-48.0f);
                    g_ParticleBox.corners[1].vz = Q12(343.0f);
                    g_ParticleBox.corners[2].vx = Q12(-39.0f);
                    g_ParticleBox.corners[2].vz = Q12(343.0f);
                    g_ParticleBox.corners[3].vx = Q12(-39.0f);
                    g_ParticleBox.corners[3].vz = Q12(349.0f);
                    g_ParticleBox.corners[4].vx = Q12(-39.0f);
                    g_ParticleBox.corners[4].vz = Q12(349.0f);
                    g_ParticleBox.corners[5].vx = Q12(-48.0f);
                    g_ParticleBox.corners[5].vz = Q12(349.0f);
                    g_ParticleBox.corners[6].vx = Q12(-48.0f);
                    g_ParticleBox.corners[6].vz = Q12(343.0f);
                    g_ParticleBox.corners[7].vx = Q12(-39.0f);
                    g_ParticleBox.corners[7].vz = Q12(343.0f);
                    break;

                default:
                    sharedData_800DD591_0_s00 = 0;
                    break;
            }
#elif defined(MAP2_S02)
            sharedData_800DD591_0_s00                 = 0;
            g_ParticleBox.corners[0].vx = Q12(-31.0f);
            g_ParticleBox.corners[0].vz = Q12(-8.0f);
            g_ParticleBox.corners[1].vx = Q12(-31.0f);
            g_ParticleBox.corners[1].vz = Q12(-11.0f);
            g_ParticleBox.corners[2].vx = Q12(-25.5f);
            g_ParticleBox.corners[2].vz = Q12(-8.0f);
            g_ParticleBox.corners[3].vx = Q12(-31.0f);
            g_ParticleBox.corners[3].vz = Q12(-8.0f);
            g_ParticleBox.corners[4].vx = Q12(-25.5f);
            g_ParticleBox.corners[4].vz = Q12(-11.0f);
            g_ParticleBox.corners[5].vx = Q12(-25.5f);
            g_ParticleBox.corners[5].vz = Q12(-8.0f);
#elif defined(MAP3_S00) || defined(MAP3_S01) || defined(MAP3_S06)
            switch (g_SavegamePtr->mapRoomIdx)
            {
                case 3:
                    sharedData_800DD591_0_s00                 = 2;
                    g_ParticleBox.corners[0].vx = Q12(101.5f);
                    g_ParticleBox.corners[0].vz = Q12(100.0f);
                    g_ParticleBox.corners[1].vx = Q12(101.5f);
                    g_ParticleBox.corners[1].vz = Q12(105.0f);
                    break;

                case 5:
                    sharedData_800DD591_0_s00                 = 1;
                    g_ParticleBox.corners[0].vx = Q12(17.7f);
                    g_ParticleBox.corners[0].vz = Q12(30.0f);
                    g_ParticleBox.corners[1].vx = Q12(17.7f);
                    g_ParticleBox.corners[1].vz = Q12(17.0f);
                    g_ParticleBox.corners[2].vx = Q12(17.5f);
                    g_ParticleBox.corners[2].vz = Q12(15.0f);
                    g_ParticleBox.corners[3].vx = Q12(15.0f);
                    g_ParticleBox.corners[3].vz = Q12(15.0f);
                    break;

                case 8:
                    sharedData_800DD591_0_s00                 = 2;
                    g_ParticleBox.corners[0].vx = Q12(63.5f);
                    g_ParticleBox.corners[0].vz = Q12(57.0f);
                    g_ParticleBox.corners[1].vx = Q12(63.5f);
                    g_ParticleBox.corners[1].vz = Q12(63.0f);
                    break;

                case 9:
                    sharedData_800DD591_0_s00                 = 2;
                    g_ParticleBox.corners[0].vx = Q12(104.0f);
                    g_ParticleBox.corners[0].vz = Q12(56.0f);
                    g_ParticleBox.corners[1].vx = Q12(104.0f);
                    g_ParticleBox.corners[1].vz = Q12(61.5f);
                    break;

                case 11:
                    sharedData_800DD591_0_s00                 = 2;
                    g_ParticleBox.corners[0].vx = Q12(143.8f);
                    g_ParticleBox.corners[0].vz = Q12(18.0f);
                    g_ParticleBox.corners[1].vx = Q12(143.8f);
                    g_ParticleBox.corners[1].vz = Q12(22.0f);
                    break;

                default:
                    sharedData_800DD591_0_s00 = 0;
                    break;
            }
#elif defined(MAP4_S03)
            if (g_SavegamePtr->mapRoomIdx == 20)
            {
                sharedData_800DD591_0_s00                 = 1;
                g_ParticleBox.corners[0].vx = Q12(133.0f);
                g_ParticleBox.corners[0].vz = Q12(130.0f);
                g_ParticleBox.corners[1].vx = Q12(133.0f);
                g_ParticleBox.corners[1].vz = Q12(150.0f);
            }

#elif defined(MAP5_S03)
            sharedData_800DD591_0_s00 = 2;
#endif

#if MAP_PARTICLE_HAS_RAIN
    #if defined(MAP5_S00) || defined(MAP6_S03)
        sharedData_800E32D4_0_s00 = 3;
    #else
        sharedData_800E32D4_0_s00 = 30;
    #endif
#endif

#if !MAP_PARTICLE_HAS_RAIN || defined(MAP0_S00) || defined(MAP6_S00)
    sharedData_800DD592_0_s00 = 1;
#endif

            if (arg3 == NO_VALUE)
            {
                g_SysWork.field_2349 = g_MapOverlayHdr.field_17;
                g_SysWork.field_2348 = g_MapOverlayHdr.field_17;
            }

            sharedData_800DD598_0_s00 = 0;
#if defined(MAP7_S03)
            D_800F23D4 = 0;
#endif
            g_Particle_PrevPosition.vz = Q12(0.0f);
            g_Particle_PrevPosition.vx = Q12(0.0f);

            g_SysWork.field_234A = true;
            g_ParticleMapIdx0 = mapIdx;

            sharedData_800E0CB8_0_s00 = FP_FROM(sharedData_800E0CB0_0_s00, Q12_SHIFT);
            sharedData_800E0CB6_0_s00 = sharedData_800E0CB0_0_s00;
            sharedData_800E0CB4_0_s00 = sharedData_800E0CB0_0_s00;

            temp_s0 = SetSp(PSX_SCRATCH_ADDR(0x3D8));

            Particle_SnowInitialize(g_Particles);
#if defined(MAP0_S00)
            func_800CBFB0(&sharedData_800E34FC_0_s00, &sharedData_800E330C_0_s00, g_ParticleMapIdx0);
#endif
            SetSp(temp_s0);
            break;

        default:
#if defined(MAP7_S03)
            sharedData_800DD584_0_s00 = 0;
            D_800F23D4               += g_DeltaTime;
#else
            sharedData_800DD584_0_s00 = g_DeltaTime == Q12(0.0f);
#endif

            WorldEnv_SnowRainColorGet(&g_WorldEnv_SnowColor, &g_WorldEnv_RainColor);

            if (sharedData_800E0CB6_0_s00 != sharedData_800E0CB4_0_s00)
            {
                if (sharedData_800DD584_0_s00 == 0)
                {
                    sharedData_800DD598_0_s00++;
                }

                temp_s0_2 = Q12_MULT(Math_Sin(Q12_ANGLE(270.0f) + (sharedData_800DD598_0_s00 * 2)), (PARTICLE_COUNT_MAX / 2)) + (PARTICLE_COUNT_MAX / 2);
                temp_a2   = Q12_MULT(Math_Sin(Q12_ANGLE(180.0f) + (sharedData_800DD598_0_s00 * 2)), (PARTICLE_COUNT_MAX / 2)) + (PARTICLE_COUNT_MAX / 2);
                temp_t0 = sharedData_800E0CB6_0_s00 >> 14;

                for (i = 0; i < 2; i++)
                {
                    temp_v1_2 = (sharedData_800E0CB4_0_s00 >> ((1 - i) * 6)) & 0x3F;
                    temp_v1_3 = (sharedData_800E0CB4_0_s00 >> 6) & 0x3F;
                    temp_v0 = temp_v1_3;

                    switch (temp_v1_2)
                    {
                        case 4:
                        case 6:
                            g_ParticlesAddedCount[i] = temp_s0_2;
                            break;

                        case 5:
                        case 7:
                            var_v0 = temp_s0_2 * 2;
                            g_ParticlesAddedCount[i] = var_v0;
                            break;

                        case 32:
                        case 48:
                            g_ParticlesAddedCount[i] = temp_a2;
                            break;

                        case 37:
                        case 39:
                        case 53:
                        case 55:
                            var_v0                       = temp_s0_2 + (PARTICLE_COUNT_MAX / 2);
                            g_ParticlesAddedCount[i] = var_v0;
                            break;

                        case 40:
                        case 56:
                            var_v0 = temp_a2 * 2;
                            g_ParticlesAddedCount[i] = var_v0;
                            break;

                        case 44:
                        case 46:
                        case 60:
                        case 62:
                            var_v0                       = temp_a2 + (PARTICLE_COUNT_MAX / 2);
                            g_ParticlesAddedCount[i] = var_v0;
                            break;

                        case 0:
                            g_ParticlesAddedCount[i] = 0;
                            break;

                        case 36:
                        case 38:
                        case 52:
                        case 54:
                            var_v0                       = PARTICLE_COUNT_MAX / 2;
                            g_ParticlesAddedCount[i] = var_v0;
                            break;

                        case 45:
                        case 47:
                        case 61:
                        case 63:
                            var_v0                       = PARTICLE_COUNT_MAX;
                            g_ParticlesAddedCount[i] = var_v0;
                            break;
                    }

#if MAP_PARTICLE_HAS_SNOW
                    if (i == 0)
                    {
                        if (temp_t0 < 2)
                        {
                            if (temp_t0 != 1 || (sharedData_800E0CB4_0_s00 >> 14) < 2)
                            {
                                sharedData_800DD592_0_s00 = 1;
                                continue;
                            }
                        }

                        switch (temp_v0)
                        {
                            case 4:
                            case 6:
                            case 32:
                            case 48:
                                var_v0_3                  = g_ParticlesAddedCount[i] < ((PARTICLE_COUNT_MAX / 4) + 1);
                                sharedData_800DD592_0_s00 = var_v0_3 ^ 1;
                                break;

                            case 5:
                            case 7:
                            case 40:
                            case 56:
                                var_v0_3                  = g_ParticlesAddedCount[i] < ((PARTICLE_COUNT_MAX / 2) + 1);
                                sharedData_800DD592_0_s00 = var_v0_3 ^ 1;
                                break;

                            case 8:
                            case 9:
                            case 10:
                            case 11:
                            case 12:
                            case 13:
                            case 14:
                            case 15:
                            case 16:
                            case 17:
                            case 18:
                            case 19:
                            case 20:
                            case 21:
                            case 22:
                            case 23:
                            case 24:
                            case 25:
                            case 26:
                            case 27:
                            case 28:
                            case 29:
                            case 30:
                            case 31:
                            case 33:
                            case 34:
                            case 35:
                            case 36:
                            case 37:
                            case 38:
                            case 39:
                            case 41:
                            case 42:
                            case 43:
                            case 44:
                            case 45:
                            case 46:
                            case 47:
                            case 49:
                            case 50:
                            case 51:
                            case 52:
                            case 53:
                            case 54:
                            case 55:
                            default:
                                sharedData_800DD592_0_s00 = 1;
                                break;
                        }
                    }
#endif
                }

                if (sharedData_800DD598_0_s00 >= 0x200)
                {
                    sharedData_800DD598_0_s00 = 0;
                    g_SysWork.field_234A = true;
                    sharedData_800E0CB6_0_s00 = sharedData_800E0CB4_0_s00;

                    switch ((sharedData_800E0CB8_0_s00 & 0xF) >> 2)
                    {
                        case 0:
                            sharedData_800DFB6C_0_s00 = 0;
                            sharedData_800DFB70_0_s00 = 0;
                            break;

                        case 1:
                            sharedData_800DFB6C_0_s00 = 1;
                            sharedData_800DFB70_0_s00 = 0;
                            break;

                        case 2:
                            sharedData_800DFB6C_0_s00 = 0;
                            sharedData_800DFB70_0_s00 = 1;
                            break;
                    }
                }
            }

            g_ParticleMapIdx1 = mapIdx;
            temp_s0_3 = SetSp(PSX_SCRATCH_ADDR(0x3D8));

            Particle_Update(g_Particles);
#if defined(MAP0_S00)
            func_800CC6E8(&sharedData_800E34FC_0_s00, &sharedData_800E330C_0_s00, g_ParticleMapIdx1);
#endif
            SetSp(temp_s0_3);

            for (i = g_ParticlesAddedCount[0] + g_ParticlesAddedCount[1]; i < PARTICLE_COUNT_MAX; i++)
            {
                g_Particles[i].stateStep = 0;
            }
            break;
    }

    g_ParticleVectors1 = g_ParticleVectors0;
}

bool Particle_CameraMovedCheck(void)
{
    q19_12 dist;
    q19_12 rotY;

    dist = SquareRoot0(SQUARE(ABS_DIFF(g_Particle_Position.vx, g_Particle_PrevPosition.vx)) +
                       SQUARE(ABS_DIFF(g_Particle_Position.vz, g_Particle_PrevPosition.vz)));
    rotY = ABS_DIFF(g_Particle_PrevRotationY, g_ParticleVectors0.viewRotation.vy);
    if (dist > 10000 || rotY > Q12_ANGLE(45.0f))
    {
        return true;
    }

    return false;
}

// TODO: Refine comments and names, it's still kind of unclear.
/** @brief Spawns snow particles. Called once when the map overlay is loaded.
 *
 * Spawns and updates snow particles.
 * This avoids having all the snow particles starting in the sky at the same time.
 *
 * The spawn loop works as follows (pseudocode):
 *
 * particlesAdded = 0;
 * toAddPerTick   = 1 or 3;
 *
 * // Perform spawn tick.
 * while (particlesAdded < SNOW_SPAWN_COUNT_MAX)
 * {
 *     // Start at 0 to update all previously added particles by one step.
 *     for (i = 0; i < particlesAdded; i++)
 *     {
 *        updateParticle(i); // These particles will re-add below if they expire.
 *     }

 *     // Add new particles.
 *     for (i = 0; i < toAddPerTick; i++)
 *     {
 *         addParticle();
 *         particlesAdded++;
 *     }
 * }
 */
void Particle_SnowInitialize(s_Particle* parts)
{
    #define SNOW_COUNT_MAX       PARTICLE_COUNT_MAX
    #define SNOW_SPAWN_COUNT_MAX (PARTICLE_COUNT_MAX/2)
    #define SNOW_REST_TICKS_MAX  16

    s32         i;
    s32         j;
    s16         rand;
    s32         density;
    s32         spawnMult;
    u16         settingsState; // TODO: Rename.
    u16         snowType;
    s32         deltaTime;
    s_Particle* part;

    deltaTime = Q12(0.1f / 3.0f);

    GsInitCoordinate2(NULL, &g_SysWork.coord_22A8);

    // Reset states.
    for (i = 0, part = parts; i < SNOW_COUNT_MAX; i++, part++)
    {
        part->stateStep = 0;
    }

    // Reset counts. NOTE: One is unused.
    g_ParticlesAddedCount[1] = 0;
    g_ParticlesAddedCount[0] = 0;

    // Settings byte holds state and snow type. Can be 0-2. 1 enables snow. TODO: Move doc.
    settingsState = (sharedData_800E0CB8_0_s00 & 0xC) >> 2;
    if (((sharedData_800E0CB8_0_s00 & 0xC) >> 2) == 1)
    {
        for (i = 0, part = parts; i < SNOW_COUNT_MAX; i++, part++)
        {
            part->type = SnowType_Light;
        }

        density = (g_SysWork.field_2349 == 1) ? 1 : 3;
    }
    else
    {
        for (i = 0, part = parts; i < SNOW_COUNT_MAX; i++, part++)
        {
            part->type = settingsState;
        }

#if defined(MAP5_S00) || defined(MAP6_S03)
        density = 1;
#else
        density = 10;
#endif
    }

    if (settingsState)
    {
        snowType = sharedData_800E0CB8_0_s00 & 0x3;
        settingsState--;

        // Set wind speed on XZ plane.
        if (snowType >= SnowType_LightWindy)
        {
            g_Particle_SpeedZ = SNOW_COUNT_LIGHT_MAX * 2;
            g_Particle_SpeedX = SNOW_COUNT_LIGHT_MAX * 2;
        }
        else
        {
            g_Particle_SpeedZ = 0;
            g_Particle_SpeedX = 0;
        }

        // Set start position.
#if defined(MAP1_S06)
        g_Particle_Position.vy = Q12(-10.0f);
#else
        g_Particle_Position.vy = Q12(-6.0f);
#endif

        // Particle type determines particle multiplier for spawn loop.
        spawnMult = (snowType == SnowType_Light || snowType == SnowType_LightWindy) ? 1 : 2;

        // Spawn and update.
        for (j = 0; j < SNOW_SPAWN_COUNT_MAX; j++)
        {
            // Reset how many particles to spawn this iteration.
            g_ParticleSpawnCount = 0;

            rand                                  = Rng_Rand16();
            g_ParticlesAddedCount[settingsState] += spawnMult;

            // Clamp spawn count.
            if (snowType == SnowType_Light || snowType == SnowType_LightWindy)
            {
                limitRange(g_ParticlesAddedCount[settingsState], 0, SNOW_SPAWN_COUNT_MAX);
            }
            else
            {
                limitRange(g_ParticlesAddedCount[settingsState], 0, SNOW_COUNT_MAX);
            }

            for (i = 0, part = parts; i < g_ParticlesAddedCount[settingsState]; i++, part++)
            {
                switch (part->stateStep)
                {
                    case ParticleState_Spawn:
                        // Throttle spawning.
                        if (g_ParticleSpawnCount < density)
                        {
                            g_ParticleSpawnCount++;
                            Particle_Spawn(settingsState, part, &rand);
                        }
                        break;

                    case ParticleState_Active:
                        Particle_SpawnMovementApply(settingsState, part, &rand, &deltaTime);

                        // Step to rest state when Y position reaches 0.
                        if (part->position0.vy == 0)
                        {
#if defined(MAP1_S06)
                            part->stateStep = 0;
#else
                            part->stateStep++;
#endif
                        }
                        break;

                    default: // `ParticleState_Rest`
                        part->stateStep++;
                        if (part->stateStep >= SNOW_REST_TICKS_MAX)
                        {
                            part->stateStep = 0;
                        }
                        break;
                }
            }
        }
    }
#if MAP_PARTICLE_HAS_RAIN
    sharedData_800E32CC_0_s00 = sharedData_800DD58C_0_s00;
#endif
}

#if defined(MAP0_S00)
void func_800CBFB0(s_800E34FC* arg0, s_800E330C* arg1, s32 mapId) // 0x800CBFB0
{
    VECTOR3 vecs[4] = {
        VECTOR3(-258.0f, -1.4f, 244.1f),
        VECTOR3(-254.5f,  0.0f, 220.5f),
        VECTOR3(-249.4f,  0.0f, 219.5f),
        VECTOR3(-250.5f,  0.0f, 217.7f)
    };

    GsInitCoordinate2(NULL, &g_SysWork.coord_22F8);
    D_800DD594 = true;
    D_800DD593 = true;
    D_800E34EC = ARRAY_SIZE(sharedData_800E330C_0_s00);
    D_800E39AC = ARRAY_SIZE(sharedData_800E34FC_0_s00);
    func_800D0394(2, vecs);
}
#endif

bool Particle_Update(s_Particle* partHead)
{
    #define SNOW_REST_TICKS_UPDATE_MAX 32

#if MAP_PARTICLE_HAS_RAIN
    s32 sp64;
#endif
    MATRIX      viewMat;
    MATRIX      worldMat;
    VECTOR3     prevPos;
    s16         rand;
    s32         pass;
    s32         density;
    s32         updateCount;
    s32         updatePrev;
    s_Particle* curPart;

    density = 0;

#if MAP_PARTICLE_HAS_RAIN
    if (sharedData_800E32D0_0_s00 < 27000)
    {
        sp64 = 3;
    }
    else
    {
        if (sharedData_800E32D0_0_s00 < 135000)
        {
            sp64 = 4;
        }
        else
        {
            sp64 = 5;
        }
    }
#endif

    GsInitCoordinate2(0, &g_SysWork.coord_22A8);

    g_Particle_Position.vx = g_ParticleVectors0.viewPosition.vx + Q12_MULT(Math_Sin(g_ParticleVectors0.viewRotation.vy), Q12(3.0));
    g_Particle_Position.vz = g_ParticleVectors0.viewPosition.vz + Q12_MULT(Math_Cos(g_ParticleVectors0.viewRotation.vy), Q12(3.0));
#if defined(MAP1_S06)
    g_Particle_Position.vy = Q12(-10.0);
#else
    g_Particle_Position.vy = Q12(-6.0);
#endif

    curPart = partHead;

    prevPos = g_Particle_Position;
    if (g_Particle_PrevPosition.vx == Q12(0.0f) && g_Particle_PrevPosition.vz == Q12(0.0f))
    {
        g_Particle_PrevPosition.vx = prevPos.vx;
        g_Particle_PrevPosition.vz = prevPos.vz;
    }

    g_ParticleCameraMoved = Particle_CameraMovedCheck();

    g_SysWork.coord_22A8.coord.t[1] = Q8(0.0f);
    g_SysWork.coord_22A8.coord.t[0] = Q12_TO_Q8(g_Particle_Position.vx);
    g_SysWork.coord_22A8.coord.t[2] = Q12_TO_Q8(g_Particle_Position.vz);

    g_SysWork.coord_22A8.flg = false;
    Vw_CoordToWorldAndViewMatrices(&g_SysWork.coord_22A8, &worldMat, &viewMat);
    gte_SetRotMatrix(&viewMat);
    gte_SetTransMatrix(&viewMat);

    // Get starting random value to use later.
    rand = Rng_Rand16();
    updatePrev = 0;

    // Update wind speed.
    if (sharedData_800E0CAC_0_s00 >= 2)
    {
        // Wind is active.
        // Ramp up the speed to 200, then add some randomness once fast enough.

        // Modulate X-axis speed.
        if (g_Particle_SpeedX < 200)
        {
            g_Particle_SpeedX++;
        }
        else
        {
            g_Particle_SpeedX += Rng_GenerateInt(-8, 8);
            limitRange(g_Particle_SpeedX, 200, 800);
        }

        // Modulate Z-axis speed.
        if (g_Particle_SpeedZ < 200)
        {
            g_Particle_SpeedZ++;
        }
        else
        {
            g_Particle_SpeedZ += Rng_GenerateInt(-8, 8);
            limitRange(g_Particle_SpeedZ, 20, 800); // Might be a dev mistake? Makes more sense to be 200 like above.
        }
    }
    // Wind is disabled, ramp speed down to 0.
    else
    {
        // X.
        if (g_Particle_SpeedX != 0)
        {
            g_Particle_SpeedX--;
        }

        // Z.
        if (g_Particle_SpeedZ != 0)
        {
            g_Particle_SpeedZ--;
        }
    }

    ////////////////////////////////////////////////////////////////////////////

    // Reset how many particles to spawn this tick.
    g_ParticleSpawnCount = 0;

#if defined(MAP0_S00)
    if (g_SavegamePtr->mapRoomIdx == 3 && PLAYER_CHARA().position.vz > Q12(200.0f))
    {
        sharedData_800DD591_0_s00 = 1;
    }
    else
    {
        sharedData_800DD591_0_s00 = 0;
    }

#elif defined(MAP0_S02)
    sharedData_800DD591_0_s00 = 2;

    switch (g_SavegamePtr->mapRoomIdx)
    {
        case 2:
            g_ParticleBox.corners[0].vx = Q12(-101.5f);
            g_ParticleBox.corners[0].vz = Q12(23.8f);
            g_ParticleBox.corners[1].vx = Q12(-101.5f);
            g_ParticleBox.corners[1].vz = Q12(17.0f);
            g_ParticleBox.corners[2].vx = Q12(-98.5f);
            g_ParticleBox.corners[2].vz = Q12(17.0f);
            g_ParticleBox.corners[3].vx = Q12(-98.5f);
            g_ParticleBox.corners[3].vz = Q12(23.8f);
            g_ParticleBox.corners[4].vx = Q12(-98.5f);
            g_ParticleBox.corners[4].vz = Q12(23.8f);
            g_ParticleBox.corners[5].vx = Q12(-101.5f);
            g_ParticleBox.corners[5].vz = Q12(23.8f);
            g_ParticleBox.corners[6].vx = Q12(-101.5f);
            g_ParticleBox.corners[6].vz = Q12(17.0f);
            g_ParticleBox.corners[7].vx = Q12(-98.5f);
            g_ParticleBox.corners[7].vz = Q12(17.0f);
            break;

        case 3:
            g_ParticleBox.corners[0].vx = Q12(-145.0f);
            g_ParticleBox.corners[0].vz = Q12(15.5f);
            g_ParticleBox.corners[1].vx = Q12(-135.0f);
            g_ParticleBox.corners[1].vz = Q12(15.5f);
            break;

        case 4:
            g_ParticleBox.corners[0].vx = Q12(-184.0f);
            g_ParticleBox.corners[0].vz = Q12(23.0f);
            g_ParticleBox.corners[1].vx = Q12(-184.0f);
            g_ParticleBox.corners[1].vz = Q12(16.0f);
            break;
    }

#elif defined(MAP1_S06)
    switch (g_SavegamePtr->mapRoomIdx)
    {
        case 9:
        case 12:
        case 13:
        case 14:
            break;

        default:
            return false;
    }

#elif defined(MAP2_S00)
    switch (g_SavegamePtr->mapRoomIdx)
    {
        case 3:
            if (PLAYER_CHARA().position.vz > Q12(200.0f))
            {
                sharedData_800DD591_0_s00 = 1;
                g_ParticleBox.corners[0].vx = Q12(-5.0f);
                g_ParticleBox.corners[0].vy = Q12(-4.5f);
                g_ParticleBox.corners[0].vz = Q12(211.0f);
                g_ParticleBox.corners[1].vx = Q12(15.0f);
                g_ParticleBox.corners[1].vy = Q12(-4.5f);
                g_ParticleBox.corners[1].vz = Q12(211.0f);
            }
            else
            {
                sharedData_800DD591_0_s00 = 0;
            }
            break;

        case 7:
            if (PLAYER_CHARA().position.vx > Q12(-55.0f) &&
                PLAYER_CHARA().position.vx < Q12(-33.0f) &&
                PLAYER_CHARA().position.vz > Q12(-14.0f) &&
                PLAYER_CHARA().position.vz < Q12(0.0f))
            {
                    sharedData_800DD591_0_s00 = 10;
                    g_ParticleBox.corners[0].vx = Q12(-44.5f);
                    g_ParticleBox.corners[0].vz = Q12(-11.2f);
                    g_ParticleBox.corners[1].vx = Q12(-44.5f);
                    g_ParticleBox.corners[1].vz = Q12(-13.0f);
                    g_ParticleBox.corners[2].vx = Q12(-43.0f);
                    g_ParticleBox.corners[2].vz = Q12(-13.0f);
                    g_ParticleBox.corners[3].vx = Q12(-43.0f);
                    g_ParticleBox.corners[3].vz = Q12(-11.2f);
                    g_ParticleBox.corners[4].vx = Q12(-43.0f);
                    g_ParticleBox.corners[4].vz = Q12(-11.2f);
                    g_ParticleBox.corners[5].vx = Q12(-44.5f);
                    g_ParticleBox.corners[5].vz = Q12(-11.2f);
            }
            else if (PLAYER_CHARA().position.vx > Q12(-90.0f) &&
                     PLAYER_CHARA().position.vx < Q12(-56.0f) &&
                     PLAYER_CHARA().position.vz > Q12(-14.0f) &&
                     PLAYER_CHARA().position.vz < Q12(0.0f))
            {
                    sharedData_800DD591_0_s00 = 10;
                    g_ParticleBox.corners[0].vx = Q12(-79.0f);
                    g_ParticleBox.corners[0].vz = Q12(-10.5f);
                    g_ParticleBox.corners[1].vx = Q12(-79.0f);
                    g_ParticleBox.corners[1].vz = Q12(-13.0f);
                    g_ParticleBox.corners[2].vx = Q12(-66.5f);
                    g_ParticleBox.corners[2].vz = Q12(-13.0f);
                    g_ParticleBox.corners[3].vx = Q12(-66.5f);
                    g_ParticleBox.corners[3].vz = Q12(-10.5f);
                    g_ParticleBox.corners[4].vx = Q12(-66.5f);
                    g_ParticleBox.corners[4].vz = Q12(-10.5f);
                    g_ParticleBox.corners[5].vx = Q12(-79.0f);
                    g_ParticleBox.corners[5].vz = Q12(-10.5f);
            }
            else
            {
                sharedData_800DD591_0_s00 = 0;
            }
            break;

        case 1:
            if (PLAYER_CHARA().position.vx > Q12(-240.0f) &&
                PLAYER_CHARA().position.vx < Q12(-226.0f) &&
                PLAYER_CHARA().position.vz > Q12(136.0f))
            {
                sharedData_800DD591_0_s00 = 10;
                g_ParticleBox.corners[0].vx = Q12(-232.0f);
                g_ParticleBox.corners[0].vz = Q12(151.0f);
                g_ParticleBox.corners[1].vx = Q12(-229.0f);
                g_ParticleBox.corners[1].vz = Q12(151.0f);
                g_ParticleBox.corners[2].vx = Q12(-232.0f);
                g_ParticleBox.corners[2].vz = Q12(158.0f);
                g_ParticleBox.corners[3].vx = Q12(-232.0f);
                g_ParticleBox.corners[3].vz = Q12(151.0f);
                g_ParticleBox.corners[4].vx = Q12(-228.0f);
                g_ParticleBox.corners[4].vz = Q12(151.0f);
                g_ParticleBox.corners[5].vx = Q12(-228.0f);
                g_ParticleBox.corners[5].vz = Q12(158.0f);
            }
            else if (PLAYER_CHARA().position.vx <= Q12(-240.0f) &&
                     PLAYER_CHARA().position.vz > Q12(133.0f))
            {
                sharedData_800DD591_0_s00 = 9;
                g_ParticleBox.corners[0].vx = Q12(-251.0f);
                g_ParticleBox.corners[0].vz = Q12(142.0f);
                g_ParticleBox.corners[1].vx = Q12(-248.2f);
                g_ParticleBox.corners[1].vz = Q12(142.0f);
                g_ParticleBox.corners[2].vx = Q12(-248.2f);
                g_ParticleBox.corners[2].vz = Q12(142.0f);
                g_ParticleBox.corners[3].vx = Q12(-248.2f);
                g_ParticleBox.corners[3].vz = Q12(147.0f);
            }
            else
            {
                sharedData_800DD591_0_s00 = 0;
            }
            break;

        case 6:
            if (PLAYER_CHARA().position.vx > Q12(-225.0f) &&
                PLAYER_CHARA().position.vx < Q12(-195.0f) &&
                PLAYER_CHARA().position.vz > Q12(-100.0f) &&
                PLAYER_CHARA().position.vz < Q12(-72.0f))
            {
                sharedData_800DD591_0_s00 = 10;
                g_ParticleBox.corners[0].vx = Q12(-208.0f);
                g_ParticleBox.corners[0].vz = Q12(-93.0f);
                g_ParticleBox.corners[1].vx = Q12(-208.0f);
                g_ParticleBox.corners[1].vz = Q12(-91.0f);
                g_ParticleBox.corners[2].vx = Q12(-212.0f);
                g_ParticleBox.corners[2].vz = Q12(-91.0f);
                g_ParticleBox.corners[3].vx = Q12(-212.0f);
                g_ParticleBox.corners[3].vz = Q12(-93.0f);
                g_ParticleBox.corners[4].vx = Q12(-208.0f);
                g_ParticleBox.corners[4].vz = Q12(-91.0f);
                g_ParticleBox.corners[5].vx = Q12(-212.0f);
                g_ParticleBox.corners[5].vz = Q12(-91.0f);
            }
            else
            {
                sharedData_800DD591_0_s00 = 0;
            }
            break;

        case 27:
            if (PLAYER_CHARA().position.vx < Q12(-275.0f) &&
                PLAYER_CHARA().position.vz < Q12(-175.0f))
            {
                sharedData_800DD591_0_s00 = 10;
                g_ParticleBox.corners[0].vx = Q12(-287.0f);
                g_ParticleBox.corners[0].vz = Q12(-182.5f);
                g_ParticleBox.corners[1].vx = Q12(-285.5f);
                g_ParticleBox.corners[1].vz = Q12(-182.5f);
                g_ParticleBox.corners[2].vx = Q12(-285.5f);
                g_ParticleBox.corners[2].vz = Q12(-177.5f);
                g_ParticleBox.corners[3].vx = Q12(-287.0f);
                g_ParticleBox.corners[3].vz = Q12(-177.5f);
                g_ParticleBox.corners[4].vx = Q12(-285.5f);
                g_ParticleBox.corners[4].vz = Q12(-181.5f);
                g_ParticleBox.corners[5].vx = Q12(-285.5f);
                g_ParticleBox.corners[5].vz = Q12(-178.4f);
            }
            else
            {
                sharedData_800DD591_0_s00 = 0;
            }
            break;
    }

#elif defined(MAP2_S02)
    if (g_SavegamePtr->mapRoomIdx == 17)
    {
        return false;
    }

    sharedData_800DD591_0_s00 = 0;

#elif defined(MAP3_S00)
    switch (g_SavegamePtr->mapRoomIdx)
    {
        case 3:
        case 5:
        case 8:
        case 9:
        case 11:
            break;

        default:
            return false;
    }

#elif defined(MAP3_S01)
    switch (g_SavegamePtr->mapRoomIdx)
    {
        case 3:
        case 5:
        case 8:
        case 9:
        case 11:
            break;

        default:
            return false;
    }

#elif defined(MAP3_S06)
    switch (g_SavegamePtr->mapRoomIdx)
    {
        case 3:
        case 5:
        case 8:
        case 9:
        case 11:
            break;

        default:
            return false;
    }

#elif defined(MAP4_S02)
    if (g_SavegamePtr->mapRoomIdx == 17)
    {
        return false;
    }

#elif defined(MAP4_S03)
    if (g_SavegamePtr->mapRoomIdx != 20)
    {
        return false;
    }

#elif defined(MAP5_S01)
    sharedData_800DD591_0_s00 = 1;

    if (PLAYER_CHARA().position.vz > Q12(-33.0f) &&
        PLAYER_CHARA().position.vz < Q12(-20.0f) &&
        PLAYER_CHARA().position.vx > Q12(13.0f) &&
        PLAYER_CHARA().position.vx < Q12(44.0f))
    {
        g_ParticleBox.corners[0].vx = Q12(24.0f);
        g_ParticleBox.corners[0].vz = Q12(-19.0f);
        g_ParticleBox.corners[1].vx = Q12(24.0f);
        g_ParticleBox.corners[1].vz = Q12(-21.5f);
        g_ParticleBox.corners[2].vx = Q12(31.5f);
        g_ParticleBox.corners[2].vz = Q12(-21.5f);
        g_ParticleBox.corners[3].vx = Q12(31.5f);
        g_ParticleBox.corners[3].vz = Q12(-19.0f);
        g_ParticleBox.corners[4].vx = Q12(24.0f);
        g_ParticleBox.corners[4].vz = Q12(-21.5f);
        g_ParticleBox.corners[5].vx = Q12(31.5f);
        g_ParticleBox.corners[5].vz = Q12(-21.5f);
    }
    else if (PLAYER_CHARA().position.vz > Q12(-82.0f) &&
             PLAYER_CHARA().position.vz < Q12(-56.0f) &&
             PLAYER_CHARA().position.vx > Q12(43.0f) &&
             PLAYER_CHARA().position.vx < Q12(70.0f))
    {
        g_ParticleBox.corners[0].vx = Q12(58.7f);
        g_ParticleBox.corners[0].vz = Q12(-70.5f);
        g_ParticleBox.corners[1].vx = Q12(58.7f);
        g_ParticleBox.corners[1].vz = Q12(-68.0f);
        g_ParticleBox.corners[2].vx = Q12(54.5f);
        g_ParticleBox.corners[2].vz = Q12(-68.0f);
        g_ParticleBox.corners[3].vx = Q12(54.5f);
        g_ParticleBox.corners[3].vz = Q12(-70.5f);
        g_ParticleBox.corners[4].vx = Q12(54.5f);
        g_ParticleBox.corners[4].vz = Q12(-70.5f);
        g_ParticleBox.corners[5].vx = Q12(58.7f);
        g_ParticleBox.corners[5].vz = Q12(-70.5f);
    }
    else
    {
        sharedData_800DD591_0_s00 = 0;
    }

#elif defined(MAP5_S03)
    sharedData_800DD591_0_s00 = 2;

    switch (g_SavegamePtr->mapRoomIdx)
    {
        case 7:
            g_ParticleBox.corners[0].vx = Q12(55.5f);
            g_ParticleBox.corners[0].vz = Q12(63.0f);
            g_ParticleBox.corners[1].vx = Q12(55.5f);
            g_ParticleBox.corners[1].vz = Q12(58.0f);
            break;

        case 8:
            g_ParticleBox.corners[0].vx = Q12(100.0f);
            g_ParticleBox.corners[0].vz = Q12(56.5f);
            g_ParticleBox.corners[1].vx = Q12(104.0f);
            g_ParticleBox.corners[1].vz = Q12(56.5f);
            break;

        default:
            return false;
    }
#endif

    for (pass = 0; pass < 2; pass++)
    {
        // Set particle density.
        switch (pass)
        {
            case 0:
            #if MAP_PARTICLE_HAS_SNOW
                density = (g_SysWork.field_2349 == 1) ? 1 : 3;
            #endif
                break;

            default:
            #if MAP_PARTICLE_HAS_RAIN
                #if defined(MAP5_S00) || defined(MAP6_S03)
                    density = 1;
                #else
                    density = 10;
                #endif
            #endif
                break;
        }

        // Handle particles in this state.
        for (updateCount = updatePrev;
             updateCount < (updatePrev + g_ParticlesAddedCount[pass]);
             updateCount++, curPart++)
        {
            // Keep only the lower 4 bits.
            // NOTE: &= 0xF would be faster, but this is what they've done.
            curPart->type %= 0xF0;

            switch (pass)
            {
                case 0:
                #if MAP_PARTICLE_HAS_SNOW
                    if (curPart->type >= 2)
                    {
                        curPart->stateStep = 0;
                        curPart->type = 0;
                    }
                #endif
                    break;

                case 1:
                #if MAP_PARTICLE_HAS_RAIN
                    // Changes any non-HeavyWindow snow particles into rain?
                    if (curPart->type != ParticleType_Rain && curPart->type != SnowType_HeavyWindy)
                    {
                        curPart->stateStep = 0;
                        curPart->type = ParticleType_Rain;
                    }
                #endif
                    break;
            }

        #if defined(MAP7_S03)
            D_800F23D0 = (g_DeltaTime * 10936) / Q12(1.0f);
        #endif

            if (sharedData_800DD584_0_s00 != 0)
            {
                // NOTE: This function only has a body in `MAP07_S03` and everything else calls an empty function.
                sharedFunc_800CE954_7_s03(pass, curPart, &rand, &g_DeltaTime);
            }
            else
            {
                switch (curPart->stateStep)
                {
                    case ParticleState_Spawn:
                        if (g_ParticleSpawnCount < density)
                        {
                            g_ParticleSpawnCount++;
                            Particle_Spawn(pass, curPart, &rand);
                        }
                        break;

                    case ParticleState_Active:
                        Particle_MovementUpdate(pass, curPart, &rand, &g_DeltaTime);
                        break;

                    default: // `ParticleState_Rest`
                        curPart->stateStep++;
                        if (curPart->stateStep < SNOW_REST_TICKS_UPDATE_MAX)
                        {
                            sharedFunc_800CFFF8_0_s00(pass, curPart, &rand);
                        }
                    #if MAP_PARTICLE_HAS_800D0690
                        else
                        {
                            sharedFunc_800D0690_1_s03(pass, curPart, &rand, &g_DeltaTime);
                        }
                    #endif
                        break;
                }
            }
        }

        updatePrev += g_ParticlesAddedCount[pass];
    }

    ////////////////////////////////////////////////////////////////////////////

    curPart = partHead;
    updatePrev = 0;
    for (pass = 0; pass < 2; pass++)
    {
    #if MAP_PARTICLE_HAS_RAIN
        if (pass != 0)
        {
            sharedData_800E32D0_0_s00 += g_ParticlesAddedCount[pass];
            limitRange(sharedData_800E32D0_0_s00, 0, 135000);
        }
    #endif

        for (updateCount = updatePrev;
             updateCount < (updatePrev + g_ParticlesAddedCount[pass]);
             updateCount++, curPart++)
        {
            switch (pass)
            {
                default:
                #if MAP_PARTICLE_HAS_RAIN
                    Particle_RainDraw(curPart, sp64);
                #endif
                    break;

                case 0:
                #if MAP_PARTICLE_HAS_SNOW
                    Particle_SnowDraw(curPart);
                #endif
                    break;
            }
        }

        updatePrev += g_ParticlesAddedCount[pass];
    }

    ////////////////////////////////////////////////////////////////////////////

    // Likely previous position for next particle system update.
    // Stores XZ position and Y rotation.
    g_Particle_PrevPosition.vx = prevPos.vx;
    g_Particle_PrevPosition.vz = prevPos.vz;
    g_Particle_PrevRotationY   = g_ParticleVectors0.viewRotation.vy;
    return false;
}

#if defined(MAP0_S00)
bool func_800CC6E8(s_800E34FC* arg0, s_800E330C* arg1, s32 mapId) // 0x800CC6E8
{
    MATRIX      worldMat;
    MATRIX      viewMat;
    s_func_800CC8FC sp50;
    VECTOR3         sp90[12];
    s32         sp120[32]; // @hack Unknown type, it's passed to `func_800CC8FC` but it's an unused parameter.
    s32         temp_s4;
    s32         i;
    s_800E330C* curArg1;
    s_800E34FC* curArg0;

    if (g_SysWork.field_234B_4 == 0)
    {
        return false;
    }

    if (g_SysWork.field_234B_4 == 2 && !func_800D0600())
    {
        return false;
    }

    curArg0 = arg0;
    curArg1 = arg1;

    if (!Game_FlashlightIsOn())
    {
        g_ParticleVectors0.svec_18.vx = Q12_ANGLE(-90.0f);
    }

    temp_s4 = func_800CC8FC(&sp90, &sp120, &sp50);
    GsInitCoordinate2(NULL, &g_SysWork.coord_22F8);

    g_SysWork.coord_22F8.coord.t[2] = Q8(0.0f);
    g_SysWork.coord_22F8.coord.t[1] = Q8(0.0f);
    g_SysWork.coord_22F8.coord.t[0] = Q8(0.0f);
    g_SysWork.coord_22F8.flg        = false;

    Vw_CoordToWorldAndViewMatrices(&g_SysWork.coord_22F8, &worldMat, &viewMat);
    gte_SetRotMatrix(&viewMat);
    gte_SetTransMatrix(&viewMat);

    if (D_800DD593)
    {
        for (i = 0; i < D_800E39AC; i++, curArg0++)
        {
            func_800CE02C(i, func_800D012C(&curArg0->field_0, &sp50, temp_s4), curArg0, mapId);
        }
    }

    if (D_800DD594)
    {
        for (i = 0; i < D_800E34EC; i++, curArg1++)
        {
            func_800CD8E8(i, (s32)(Rng_Rand16() % 65) >> 6, curArg1);
        }
    }

    return false;
}

s32 func_800CC8FC(VECTOR3* arg0, s32* arg1, s_func_800CC8FC* arg2) // 0x800CC8FC
{
    SVECTOR            sp10;
    SVECTOR            offset0; // Q23.8
    SVECTOR            offset1; // Q23.8
    DVECTOR            sp28;
    DVECTOR            sp30[8];
    SVECTOR            sp50[12];
    MATRIX             mat;
    s32                temp_a0;
    s32                temp_s4;
    s32                temp_s5;
    s32                temp_s7;
    s32                j;
    s32                i;
    s32                count;
    VECTOR3*           unkPos;
    s_func_800CC8FC_0* ptr;
    DVECTOR*           dVecPtr;

    s16 D_800CA7CC[18][2] = {
        { 0x00, 0x06 },
        { 0x01, 0x07 },
        { 0x02, 0x08 },
        { 0x03, 0x09 },
        { 0x04, 0x0A },
        { 0x05, 0x0B },
        { 0x00, 0x01 },
        { 0x01, 0x02 },
        { 0x02, 0x03 },
        { 0x03, 0x04 },
        { 0x04, 0x05 },
        { 0x05, 0x00 },
        { 0x06, 0x07 },
        { 0x07, 0x08 },
        { 0x08, 0x09 },
        { 0x09, 0x0A },
        { 0x0A, 0x0B },
        { 0x0B, 0x06 },
    };

    temp_s4 = g_ParticleVectors0.field_28 >> 6;
    temp_s7 = g_ParticleVectors0.field_28 >> 3;
    temp_a0 = g_ParticleVectors0.field_28 / 20;
    temp_s5 = g_ParticleVectors0.field_28 / 12;

    for (i = 0; i < 6; i++)
    {
        sp50[i].vx = Q12_MULT(temp_a0, Math_Sin((i * Q12(60.0f)) / 360));
        sp50[i].vy = Q12_MULT(temp_a0, Math_Cos((i * Q12(60.0f)) / 360));
        sp50[i].vz = -temp_s4;
    }

    for (i = 0; i < 6; i++)
    {
        sp50[i + 6].vx = Q12_MULT(temp_s5, Math_Sin((i * Q12(60.0f)) / 360));
        sp50[i + 6].vy = Q12_MULT(temp_s5, Math_Cos((i * Q12(60.0f)) / 360));
        sp50[i + 6].vz = temp_s7;
    }

    count = 0;
    Math_RotMatrixZxyNegGte(&g_ParticleVectors0.svec_18, &mat);

    for (j = 0; j < 18; j++)
    {
        sp10 = sp50[D_800CA7CC[j][0]];

        PushMatrix();
        ApplyMatrixSV(&mat, &sp10, &offset0);
        PopMatrix();

        unkPos = &arg0[D_800CA7CC[j][0]];

        unkPos->vx = Q8_TO_Q12(offset0.vx) + (g_ParticleVectors0.vector_0.vx - PLAYER_CHARA().position.vx);
        unkPos->vy = Q8_TO_Q12(offset0.vy) + (g_ParticleVectors0.vector_0.vy - PLAYER_CHARA().position.vy);
        unkPos->vz = Q8_TO_Q12(offset0.vz) + (g_ParticleVectors0.vector_0.vz - PLAYER_CHARA().position.vz);

        offset0.vx += Q12_TO_Q8(g_ParticleVectors0.vector_0.vx - PLAYER_CHARA().position.vx);
        offset0.vy += Q12_TO_Q8(g_ParticleVectors0.vector_0.vy - PLAYER_CHARA().position.vy);
        offset0.vz += Q12_TO_Q8(g_ParticleVectors0.vector_0.vz - PLAYER_CHARA().position.vz);

        gte_ldv0(&offset0);
        gte_rtps();

        sp10 = sp50[D_800CA7CC[j][1]];

        PushMatrix();
        ApplyMatrixSV(&mat, &sp10, &offset1);
        PopMatrix();

        unkPos = &arg0[D_800CA7CC[j][1]];

        unkPos->vx = Q8_TO_Q12(offset1.vx) + (g_ParticleVectors0.vector_0.vx - PLAYER_CHARA().position.vx);
        unkPos->vy = Q8_TO_Q12(offset1.vy) + (g_ParticleVectors0.vector_0.vy - PLAYER_CHARA().position.vy);
        unkPos->vz = Q8_TO_Q12(offset1.vz) + (g_ParticleVectors0.vector_0.vz - PLAYER_CHARA().position.vz);

        offset1.vx += Q12_TO_Q8(g_ParticleVectors0.vector_0.vx - PLAYER_CHARA().position.vx);
        offset1.vy += Q12_TO_Q8(g_ParticleVectors0.vector_0.vy - PLAYER_CHARA().position.vy);
        offset1.vz += Q12_TO_Q8(g_ParticleVectors0.vector_0.vz - PLAYER_CHARA().position.vz);

        if ((offset0.vy >= Q8(0.0f) && offset1.vy <  Q8(0.0f)) ||
            (offset0.vy <  Q8(0.0f) && offset1.vy >= Q8(0.0f)))
        {
            sp30[count].vx = offset0.vx - (offset0.vy * (offset1.vx - offset0.vx)) / (offset1.vy - offset0.vy);
            sp30[count].vy = offset0.vz - (offset0.vy * (offset1.vz - offset0.vz)) / (offset1.vy - offset0.vy);
            count++;
        }
    }

    if (count == 0)
    {
        return 0;
    }

    for (i = 0; i < (count - 1); i++)
    {
        for (j = i + 1; j < count; j++)
        {
            if (sp30[i].vx > sp30[j].vx)
            {
                sp28    = sp30[i];
                sp30[i] = sp30[j];
                sp30[j] = sp28;
            }
        }
    }

    ptr = arg2->field_0;

    for (j = 0; j < 3; j++)
    {
        ptr[j].field_0 = sp30[j];
    }

    if ((ptr[1].field_0.vx - ptr[0].field_0.vx) * (ptr[2].field_0.vy - ptr[0].field_0.vy) <=
        (ptr[1].field_0.vy - ptr[0].field_0.vy) * (ptr[2].field_0.vx - ptr[0].field_0.vx))
    {
        sp28           = ptr[1].field_0;
        ptr[1].field_0 = ptr[2].field_0;
        ptr[2].field_0 = sp28;
    }

    for (j = 0; j < 3; j++)
    {
        ptr[j].field_4 = j - 1;
        ptr[j].field_5 = j + 1;
    }

    ptr[0].field_4 = 2;
    ptr[2].field_5 = 0;

    ptr = arg2->field_0;

    for (i = 3; i < count; i++)
    {
        dVecPtr = &sp10;

        j = 0;

        while ((ptr[ptr[j].field_5].field_0.vx - ptr[j].field_0.vx) * (dVecPtr[i + 8].vy - ptr[j].field_0.vy) >
               (ptr[ptr[j].field_5].field_0.vy - ptr[j].field_0.vy) * (dVecPtr[i + 8].vx - ptr[j].field_0.vx))
        {
            j = ptr[j].field_5;
        }

        ptr[i].field_7 = j;

        j = 0;

        while ((ptr[j].field_0.vx - ptr[ptr[j].field_4].field_0.vx) * (dVecPtr[i + 8].vy - ptr[ptr[j].field_4].field_0.vy) >
               (ptr[j].field_0.vy - ptr[ptr[j].field_4].field_0.vy) * (dVecPtr[i + 8].vx - ptr[ptr[j].field_4].field_0.vx))
        {
            j = ptr[j].field_5;
        }

        ptr[i].field_6 = j;

        ptr[ptr[i].field_7].field_5 = i;
        ptr[ptr[i].field_6].field_4 = i;

        ptr[i].field_5 = ptr[i].field_6;
        ptr[i].field_4 = ptr[i].field_7;

        ptr[i].field_0 = sp30[i];
    }

    D_800DFB54 = 1;

    for (i = 0; ptr[i].field_5 != 0; D_800DFB54++)
    {
        i = ptr[i].field_5;
    }

    return D_800DFB54;
}

void func_800CD1F4(s32 arg0, s32 arg1, s_800E330C* arg2) // 0x800CD1F4
{
    VECTOR3 pos; // Q23.8
    q3_12   angle0;
    q3_12   angle1;
    s32     temp_s0_3;
    s32     var_s0;

    if (sharedData_800DD584_0_s00 != 0)
    {
        return;
    }

    arg2->field_0.vx += TIMESTEP_SCALE_30_FPS(g_DeltaTime, arg2->field_C.vx);
    arg2->field_0.vy += TIMESTEP_SCALE_30_FPS(g_DeltaTime, arg2->field_C.vy);
    arg2->field_0.vz += TIMESTEP_SCALE_30_FPS(g_DeltaTime, arg2->field_C.vz);

    limitRange(arg2->field_C.vx, -0x100, 0xFF);
    limitRange(arg2->field_C.vy, -0xFF, 0xFF);
    limitRange(arg2->field_C.vz, -0xFF, 0xFF);

    switch (arg2->field_15)
    {
        case 1:
            arg2->field_C.vy += Rng_GenerateInt(-4, 4);
            limitRange(arg2->field_C.vy, -16, 16);

            arg2->field_C.vx += Q12_MULT(Math_Sin((arg2->field_16 + arg0) << 5), Q12(0.01f));
            arg2->field_C.vz += Q12_MULT(Math_Cos((arg2->field_16 + arg0) << 5), Q12(0.01f));

            var_s0 = arg2->field_16;

            if (Rng_GenerateUInt(128, 159) < var_s0)
            {
                arg2->field_15 = Rng_GenerateInt(1, 3);
                arg2->field_16 = 0;
            }
            else
            {
                arg2->field_16++;
            }

            if (arg1 != 0)
            {
                arg2->field_15 = 5;
                arg2->field_16 = 0;
            }
            break;

        case 2:
            arg2->field_C.vy += Rng_GenerateInt(-4, 4);
            limitRange(arg2->field_C.vy, -16, 16);

            arg2->field_C.vx += Q12_MULT(Math_Sin((arg2->field_16 + arg0) << 5), Q12(0.01f));
            arg2->field_C.vz += Q12_MULT(Math_Cos((arg2->field_16 + arg0) << 6), Q12(0.01f));

            var_s0 = arg2->field_16;

            if (Rng_GenerateUInt(128, 191) < var_s0)
            {
                arg2->field_15 = Rng_GenerateInt(1, 3);
                arg2->field_16 = 0;
            }
            else
            {
                arg2->field_16++;
            }

            if (arg1 != 0)
            {
                arg2->field_15 = 5;
                arg2->field_16 = 0;
            }
            break;

        case 3:
            arg2->field_C.vy += Rng_GenerateInt(-4, 4);
            limitRange(arg2->field_C.vy, -16, 16);

            if (arg2->field_16 == 0)
            {
                angle0 = Rng_GenerateInt(0, Q12_CLAMPED(1.0f));

                arg2->field_C.vx = (u32)(Math_Sin(angle0) * 5) >> 7;
                arg2->field_C.vz = (u32)(Math_Sin(angle0) * 5) >> 7;
            }

            var_s0 = arg2->field_16;

            if (Rng_GenerateUInt(100, 227) < var_s0)
            {
                arg2->field_15 = Rng_GenerateInt(1, 3);
                arg2->field_16 = 0;
            }
            else
            {
                arg2->field_16++;
            }

            if (arg1 != 0)
            {
                arg2->field_15 = 5;
                arg2->field_16 = 0;
            }
            break;

        case 5:
            if (arg0 < Rng_GenerateInt(10, 19) || g_SysWork.enablePlayerMatchAnim == 0)
            {
                pos.vx = Q12_TO_Q8(D_800E32DC->vx - arg2->field_0.vx);
                pos.vy = Q12_TO_Q8(D_800E32DC->vy - arg2->field_0.vy);
                pos.vz = Q12_TO_Q8(D_800E32DC->vz - arg2->field_0.vz);
            }
            else
            {
                pos.vx = g_SysWork.playerBoneCoords[HarryBone_RightHand].workm.t[0] - Q12_TO_Q8(arg2->field_0.vx);
                pos.vy = g_SysWork.playerBoneCoords[HarryBone_RightHand].workm.t[1] - Q12_TO_Q8(arg2->field_0.vy);
                pos.vz = g_SysWork.playerBoneCoords[HarryBone_RightHand].workm.t[2] - Q12_TO_Q8(arg2->field_0.vz);
            }

            temp_s0_3 = SquareRoot0(SQUARE(pos.vx) + SQUARE(pos.vz));
            angle0    = ratan2(pos.vx, pos.vz);
            angle1    = ratan2(pos.vy, temp_s0_3);

            arg2->field_C.vx += Q12_MULT(Math_Sin(angle0), Q12(0.02f) - 1);
            arg2->field_C.vz += Q12_MULT(Math_Cos(angle0), Q12(0.02f) - 1);
            arg2->field_C.vy += Q12_MULT(Math_Sin(angle1), Q12(0.01f));

            var_s0 = arg2->field_16;

            if (Rng_GenerateUInt(80, 143) < var_s0 || (arg1 != 0 && arg2->field_16 > 80))
            {
                arg2->field_16 = 0;
                arg2->field_15 = Rng_GenerateInt(1, 3);
            }
            else
            {
                arg2->field_16++;
            }
            break;
    }
}

void func_800CD8E8(s32 arg0, s32 arg1, s_800E330C* arg2) // 0x800CD8E8
{
    SVECTOR   pos; // Q23.8
    s32       depth;
    u8        temp_v0;
    POLY_FT4* poly;

    func_800CD1F4(arg0, arg1, arg2);

    poly = (POLY_FT4*)GsOUT_PACKET_P;

    // Set as `POLY_FT4` with `tpage = 44`.
    setPolyFT4TPage(poly, 44);

    poly->clut = getClut(PARTICLE_CLUT_X, PARTICLE_CLUT_Y);

    if (sharedData_800DD584_0_s00 != 0)
    {
        switch (arg0 & 3)
        {
            case 0:
                setParticleUV4(poly, 0);
                break;

            case 1:
                setParticleUV4(poly, 16);
                break;

            case 2:
                setParticleUV4(poly, 32);
                break;

            case 3:
                setParticleUV4(poly, 48);
                break;
        }
    }
    else
    {
        switch ((u32)g_SysWork.gameStateCounter % 8)
        {
            case 0:
            case 1:
                switch (arg0 & 3)
                {
                    case 0:
                        setParticleUV4(poly, 0);
                        break;

                    case 1:
                        setParticleUV4(poly, 16);
                        break;

                    case 2:
                        setParticleUV4(poly, 32);
                        break;

                    case 3:
                        setParticleUV4(poly, 48);
                        break;
                }
                break;

            case 2:
            case 3:
                switch (arg0 & 0x3)
                {
                    case 0:
                        setParticleUV4(poly, 16);
                        break;

                    case 1:
                        setParticleUV4(poly, 32);
                        break;

                    case 2:
                        setParticleUV4(poly, 48);
                        break;

                    case 3:
                        setParticleUV4(poly, 0);
                        break;
                }
                break;

            case 4:
            case 5:
                switch (arg0 & 0x3)
                {
                    case 0:
                        setParticleUV4(poly, 32);
                        break;

                    case 1:
                        setParticleUV4(poly, 48);
                        break;

                    case 2:
                        setParticleUV4(poly, 0);
                        break;

                    case 3:
                        setParticleUV4(poly, 16);
                        break;
                }
                break;

            case 6:
            case 7:
                switch (arg0 & 0x3)
                {
                    case 0:
                        setParticleUV4(poly, 48);
                        break;

                    case 1:
                        setParticleUV4(poly, 0);
                        break;

                    case 2:
                        setParticleUV4(poly, 16);
                        break;

                    case 3:
                        setParticleUV4(poly, 32);
                        break;
                }
                break;
        }
    }

    if (Game_FlashlightIsOn())
    {
        temp_v0 = func_80055D78(PLAYER_CHARA().position.vx + arg2->field_0.vx,
                                PLAYER_CHARA().position.vy + arg2->field_0.vy,
                                PLAYER_CHARA().position.vz + arg2->field_0.vz);

        temp_v0 = (temp_v0 != 0) ? ((temp_v0 >= 0 && temp_v0 <= 176) ? temp_v0 : 176) : 1;

        switch (D_800C39A0)
        {
            case 1:
            case 4:
            case 10:
                poly->r0 = temp_v0 + g_WorldEnv_SnowColor.r;
                poly->g0 = temp_v0 + g_WorldEnv_SnowColor.g;
                poly->b0 = temp_v0 + g_WorldEnv_SnowColor.b;
                break;

            default:
                temp_v0 = func_80055D78(PLAYER_CHARA().position.vx + arg2->field_0.vx,
                                        PLAYER_CHARA().position.vy + arg2->field_0.vy,
                                        PLAYER_CHARA().position.vz + arg2->field_0.vz);
                temp_v0 = (temp_v0 != 0) ? ((temp_v0 >= 0 && temp_v0 <= 176) ? temp_v0 : 176) : 1;

                temp_v0 >>= 3;
                poly->r0  = g_WorldEnv_SnowColor.r + temp_v0;
                poly->g0  = g_WorldEnv_SnowColor.g + temp_v0;
                poly->b0  = g_WorldEnv_SnowColor.b + temp_v0;
                break;

            case 7:
            case 9:
            case 11:
                poly->r0 = g_WorldEnv_SnowColor.r;
                poly->g0 = g_WorldEnv_SnowColor.g;
                poly->b0 = g_WorldEnv_SnowColor.b;
                break;
        }
    }
    else
    {
        switch (D_800C39A0)
        {
            case 1:
            case 4:
            case 10:
                temp_v0 = func_80055D78(PLAYER_CHARA().position.vx + arg2->field_0.vx,
                                        PLAYER_CHARA().position.vy + arg2->field_0.vy,
                                        PLAYER_CHARA().position.vz + arg2->field_0.vz);
                temp_v0 = (temp_v0 != 0) ? ((temp_v0 >= 0 && temp_v0 <= 176) ? temp_v0 : 176) : 1;

                poly->r0 = temp_v0 + g_WorldEnv_SnowColor.r;
                poly->g0 = temp_v0 + g_WorldEnv_SnowColor.g;
                poly->b0 = temp_v0 + g_WorldEnv_SnowColor.b;
                break;

            default:
                poly->r0 = g_WorldEnv_SnowColor.r;
                poly->g0 = g_WorldEnv_SnowColor.g;
                poly->b0 = g_WorldEnv_SnowColor.b;
                break;
        }
    }

    pos.vx = Q12_TO_Q8(arg2->field_0.vx);
    pos.vy = Q12_TO_Q8(arg2->field_0.vy);
    pos.vz = Q12_TO_Q8(arg2->field_0.vz);

    gte_ldv0(&pos);
    gte_rtps();
    gte_stsxy(&poly->x0);
    gte_stszotz(&depth);

    depth = depth >> 1;

    if (depth >= 1 && depth < ((ORDERING_TABLE_SIZE / 2) - 1))
    {
        u8 temp_a1;

        if ((depth >> 3) >= 13)
        {
            temp_a1 = 3;
        }
        else
        {
            temp_a1 = 16 - (depth >> 3);
        }

        if (temp_a1 < 0 || temp_a1 >= 3)
        {
            if (temp_a1 > 16)
            {
                temp_a1 = 16;
            }
        }
        else
        {
            temp_a1 = 3;
        }

        poly->x1 = poly->x0;
        poly->x2 = poly->x3 = poly->x0 + temp_a1;
        poly->y2            = poly->y0;
        poly->y1 = poly->y3 = poly->y0 + temp_a1;

        addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[depth], poly);

        GsOUT_PACKET_P = poly + 1;
    }
}

void func_800CE02C(s32 arg0, s32 arg1, s_800E34FC* pos, s32 mapId) // 0x800CE02C
{
    SVECTOR3  posQ8;
    POLY_FT4* poly;
    s32       depth;
    s32       depthDiv16;

    func_800CE544(arg0, arg1, pos);

    poly = (POLY_FT4*)GsOUT_PACKET_P;
    setPolyFT4(poly);
    setSemiTrans(poly, true);

    poly->tpage = 76;
    poly->clut  = getClut(PARTICLE_CLUT_X, PARTICLE_CLUT_Y + 1);

    if (sharedData_800DD584_0_s00 != 0)
    {
        switch (arg0 & 3)
        {
            case 0:
                setParticleUV4(poly, 64);
                break;

            case 1:
                setParticleUV4(poly, 80);
                break;

            case 2:
                setParticleUV4(poly, 96);
                break;

            case 3:
                setParticleUV4(poly, 112);
                break;
        }
    }
    else
    {
        switch ((u32)g_SysWork.gameStateCounter % 8)
        {
            case 0:
            case 1:
                switch (arg0 & 0x3)
                {
                    case 0:
                        setParticleUV4(poly, 64);
                        break;

                    case 1:
                        setParticleUV4(poly, 80);
                        break;

                    case 2:
                        setParticleUV4(poly, 96);
                        break;

                    case 3:
                        setParticleUV4(poly, 112);
                        break;
                }
                break;

            case 2:
            case 3:
                switch (arg0 & 3)
                {
                    case 0:
                        setParticleUV4(poly, 80);
                        break;

                    case 1:
                        setParticleUV4(poly, 96);
                        break;

                    case 2:
                        setParticleUV4(poly, 112);
                        break;

                    case 3:
                        setParticleUV4(poly, 64);
                        break;
                }
                break;

            case 4:
            case 5:
                switch (arg0 & 3)
                {
                    case 0:
                        setParticleUV4(poly, 96);
                        break;

                    case 1:
                        setParticleUV4(poly, 112);
                        break;

                    case 2:
                        setParticleUV4(poly, 64);
                        break;

                    case 3:
                        setParticleUV4(poly, 80);
                        break;
                }
                break;

            case 6:
            case 7:
                switch (arg0 & 3)
                {
                    case 0:
                        setParticleUV4(poly, 112);
                        break;

                    case 1:
                        setParticleUV4(poly, 64);
                        break;

                    case 2:
                        setParticleUV4(poly, 80);
                        break;

                    case 3:
                        setParticleUV4(poly, 96);
                        break;
                }
        }
    }

    poly->b0 = poly->g0 = poly->r0 = 0x80;

    posQ8.vx = Q12_TO_Q8(pos->field_0.vx);
    posQ8.vy = Q12_TO_Q8(pos->field_0.vy);
    posQ8.vz = Q12_TO_Q8(pos->field_0.vz);

    gte_ldv0(&posQ8);
    gte_rtps();
    gte_stsxy(&poly->x0);
    gte_stszotz(&depth);

    depthDiv16 = depth >> 4;
    depth      = depth >> 1;

    if (depth > 32 && depth < ((ORDERING_TABLE_SIZE / 2) - 1))
    {
        u8 var_a1;

        if (depthDiv16 >= 13)
        {
            var_a1 = 3;
        }
        else
        {
            var_a1 = 16 - depthDiv16;
        }

        if (var_a1 >= 3)
        {
            if (var_a1 >= 17)
            {
                var_a1 = 16;
            }
        }
        else
        {
            var_a1 = 3;
        }

        poly->x1 = poly->x0;
        poly->x2 = poly->x3 = poly->x0 + var_a1;
        poly->y2            = poly->y0;
        poly->y1 = poly->y3 = poly->y0 + var_a1;

        addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[depth], poly);

        GsOUT_PACKET_P = (PACKET*)&poly[1];
    }
}

void func_800CE544(s32 idx0, s32 arg1, s_800E34FC* arg2) // 0x800CE544
{
    s16     rng0;
    q3_12   angle;
    s32     someMax;
    s32     someMin;
    VECTOR3 vecCpy;

    if (sharedData_800DD584_0_s00 == 0)
    {
        arg2->field_0.vx += TIMESTEP_SCALE_30_FPS(g_DeltaTime, arg2->field_C);
        arg2->field_0.vz += TIMESTEP_SCALE_30_FPS(g_DeltaTime, arg2->field_E);

        switch (arg2->field_11)
        {
            case 1:
                arg2->field_12++;
                if (arg1)
                {
                    if (Rng_GenerateUInt(0, 1) != 0) // 1 in 2 chance.
                    {
                        arg2->field_11 = 5;
                        arg2->field_12 = 0;
                    }
                    else
                    {
                        arg2->field_11 = 6;
                    }
                }

                if (arg2->field_12 > Rng_GenerateUInt(100, 227))
                {
                    arg2->field_11 = (Rng_GenerateUInt(0, 1) * 3) + 1;
                    arg2->field_12 = 0;
                }

                arg2->field_C += Rng_GenerateInt(-4, 4);
                arg2->field_E += Rng_GenerateInt(-4, 4);
                break;

            case 2:
                arg2->field_12++;
                if (arg1)
                {
                    arg2->field_11 = 5;
                    arg2->field_12 = 0;
                }

                arg2->field_C += Rng_GenerateInt(-4, 4);
                arg2->field_E += Rng_GenerateInt(-4, 4);

                if (arg2->field_12 > Rng_GenerateUInt(100, 227))
                {
                    arg2->field_11 = Rng_GenerateUInt(3, 4);
                    arg2->field_12 = 0;

                    rng0 = Rng_GenerateInt(0, 0xFFF);
                    arg2->field_C = (u32)(Math_Sin(rng0) * 3) / 0x100;
                    arg2->field_E = (u32)(Math_Cos(rng0) * 3) / 0x100;
                }
                break;

            case 3:
                arg2->field_12++;

                if (arg2->field_12 > Rng_GenerateUInt(100, 227))
                {
                    arg2->field_11 = 2;
                    arg2->field_E = 0;
                    arg2->field_C = 0;
                    arg2->field_12 = 0;
                }

                if (arg1)
                {
                    arg2->field_11 = 5;
                    arg2->field_12 = 0;
                }
                break;

            case 4:
                arg2->field_12++;

                vecCpy.vx = D_800E32DC[(idx0 / 20) + 1].vx - arg2->field_0.vx;
                vecCpy.vz = D_800E32DC[(idx0 / 20) + 1].vz - arg2->field_0.vz;
                vecCpy.vy = 0;

                angle = ratan2(vecCpy.vx, vecCpy.vz);
                arg2->field_C = (u32)(Math_Sin(angle) * 3) / 0x100;
                arg2->field_E = (u32)(Math_Cos(angle) * 3) / 0x100;

                if (arg1)
                {
                    arg2->field_11 = Rng_GenerateUInt(5, 6);
                    arg2->field_12 = 0;
                }

                if (arg2->field_12 > 192)
                {
                    arg2->field_11 = Rng_GenerateUInt(1, 2);
                    arg2->field_E = 0;
                    arg2->field_C = 0;
                    arg2->field_12 = 0;
                }
                break;

            case 5:
                if (arg2->field_12 == 0)
                {
                    vecCpy.vx = Q12_TO_Q8(arg2->field_0.vx - PLAYER_CHARA().position.vx);
                    vecCpy.vz = Q12_TO_Q8(arg2->field_0.vz - PLAYER_CHARA().position.vz);

                    angle = ratan2(vecCpy.vx, vecCpy.vz);
                    arg2->field_C = (u32)(Math_Sin(angle) * 3) / 0x10;
                    arg2->field_E = (u32)(Math_Cos(angle) * 3) / 0x10;
                }

                if (arg1)
                {
                    arg2->field_12 = 0;
                }

                if (arg2->field_12 > 16)
                {
                    arg2->field_11 = 2;
                    arg2->field_12 = 0;
                }

                arg2->field_12++;
                break;

            case 6:
                arg2->field_12++;

                if (arg2->field_12 > Rng_GenerateUInt(20, 21))
                {
                    if (arg1)
                    {
                        arg2->field_11 = Rng_GenerateUInt(5, 6);
                        arg2->field_12 = 0;
                    }
                    else
                    {
                        arg2->field_11 = (Rng_GenerateUInt(0, 1) * 3) + 1;
                    }
                }

                arg2->field_C += Rng_GenerateInt(-4, 4);
                arg2->field_E += Rng_GenerateInt(-4, 4);
                break;
        }

        someMin = (arg2->field_11 == 5) ? -64 : -20;
        someMax = (arg2->field_11 == 5) ?  64 :  20;
        arg2->field_C = CLAMP(arg2->field_C, someMin, someMax);
        arg2->field_E = CLAMP(arg2->field_E, someMin, someMax);
    }
}
#endif

#if MAP_PARTICLE_HAS_SNOW
#if !defined(PARTICLE_CASE_COUNT)
    #define PARTICLE_CASE_COUNT 1
    #define HAS_PARTICLE_FALLBACK_CASE
#endif

/** Particle case switch handling.
 *
 * Maps define one or more HAS_PARTICLE_CASE_X flags to include their
 * respective switch cases.
 * When only a single case is present (`PARTICLE_CASE_COUNT == 1`), the
 * macro below replaces `case N:` with `default:` so that the compiler
 * collapses the switch into a direct block, matching single-case maps
 * that skip checking the case entirely.
 */
#if PARTICLE_CASE_COUNT > 1
    #define PARTICLE_CASE(id) case id
#else
    #define PARTICLE_CASE(id) default
#endif

void Particle_SnowDraw(s_Particle* part)
{
#if !defined(MAP1_S00) && !defined(MAP6_S00) && !defined(MAP7_S03)
    VECTOR3     particlePos;
#endif

#if defined(HAS_PARTICLE_CASE_1)
    VECTOR3     partCorners[2];
#endif

    SVECTOR3    particlePosQ8;
    s32         zScreenStart;

    u8          primColorG;
    u8          primColorB;
    u8          primColorR;

    s_Particle* localPart;
    POLY_FT3*   polyFt3;

    localPart = part;
    polyFt3   = (POLY_FT3*)GsOUT_PACKET_P;

    if (part->type >= 240)
    {
        part->type += 16;
        if (part->stateStep < 0 || part->stateStep >= 31)
        {
            part->stateStep = 0;
            return;
        }
    }
    else if (part->stateStep != 0)
    {
#if !defined(MAP1_S00) && !defined(MAP6_S00) && !defined(MAP7_S03)
        if (sharedData_800DD591_0_s00 != 0)
        {
            particlePos = part->position0;

            // TODO: Check value of `sharedData_800DD591_0_s00` on each map, some of the `#if defined(MAPX)` changes in the switch below could be separate cases.
            // (fallback case might also have its own value too)
            switch (sharedData_800DD591_0_s00)
            {
#ifdef HAS_PARTICLE_CASE_1
                PARTICLE_CASE(1):
                    if (part->position0.vy < g_ParticleBox.corners[0].vy)
                    {
                        Particle_BoundaryClamp(&particlePos, &g_ParticleBox.corners[0], &g_ParticleBox.corners[1], 0);
                    }
                    else
                    {
                        s32 temp_v1;
                        partCorners[0].vx = g_ParticleBox.corners[0].vx;
                        partCorners[1].vx = g_ParticleBox.corners[1].vx;
                        temp_v1           = g_ParticleBox.corners[0].vz + 0x1400;
                        temp_v1          += part->position0.vy >> 2;
                        partCorners[1].vz = temp_v1;
                        partCorners[0].vz = temp_v1;

                        Particle_BoundaryClamp(&particlePos, &partCorners[0], &partCorners[1], 0);
                    }
                    break;
#endif

#if defined(HAS_PARTICLE_CASE_2)
                PARTICLE_CASE(2):
                    Particle_BoundaryClamp(&particlePos, &g_ParticleBox.corners[0], &g_ParticleBox.corners[1], 0);
                    if (g_SavegamePtr->mapRoomIdx == 5)
                    {
                        Particle_BoundaryClamp(&particlePos, &g_ParticleBox.corners[2], &g_ParticleBox.corners[3], 0);
                    }
                    break;
#endif

#if defined(HAS_PARTICLE_CASE_9)
                PARTICLE_CASE(9):
                    Particle_BoundaryClamp(&particlePos, &g_ParticleBox.corners[0], &g_ParticleBox.corners[1], 1);
                    if (particlePos.vy == 0)
                    {
                        Particle_BoundaryClamp(&particlePos, &g_ParticleBox.corners[2], &g_ParticleBox.corners[3], 0);
                    }
                    break;
#endif

#ifdef HAS_PARTICLE_CASE_10
                PARTICLE_CASE(10):
                    Particle_BoundaryClamp(&particlePos, &g_ParticleBox.corners[0], &g_ParticleBox.corners[1], 1);
                    if (particlePos.vy == 0)
                    {
                        Particle_BoundaryClamp(&particlePos, &g_ParticleBox.corners[2], &g_ParticleBox.corners[3], 1);
                        if (particlePos.vy == 0)
                        {
                            Particle_BoundaryClamp(&particlePos, &g_ParticleBox.corners[4], &g_ParticleBox.corners[5], 0);
                        }
                    }
                    break;
#endif

#ifdef HAS_PARTICLE_CASE_11
                PARTICLE_CASE(11):
#if defined(MAP0_S02) // TODO: MAP0_S02 uses value 2 instead of 11, code used in MAP0_S02 should probably be merged to case 2 above.
                    if (PLAYER_CHARA().position.vx < Q12(-120.0f))
                    {
                        Particle_BoundaryClamp(&particlePos, &g_ParticleBox.corners[0], &g_ParticleBox.corners[1], 0);
                        break;
                    }
#endif
                    Particle_BoundaryClamp(&particlePos, &g_ParticleBox.corners[0], &g_ParticleBox.corners[1], 1);
                    if (particlePos.vy == 0)
                    {
                        Particle_BoundaryClamp(&particlePos, &g_ParticleBox.corners[2], &g_ParticleBox.corners[3], 1);
                        if (particlePos.vy == 0)
                        {
                            Particle_BoundaryClamp(&particlePos, &g_ParticleBox.corners[4], &g_ParticleBox.corners[5], 1);
                            if (particlePos.vy == 0)
                            {
                                Particle_BoundaryClamp(&particlePos, &g_ParticleBox.corners[6], &g_ParticleBox.corners[7], 0);
                            }
                        }
                    }
                    break;
#endif

#ifdef HAS_PARTICLE_FALLBACK_CASE
                default:
                    Particle_BoundaryClamp(&particlePos, &g_ParticleBox.corners[0], &g_ParticleBox.corners[1], 0);
                    break;
#endif
            }

            particlePosQ8.vx = Q12_TO_Q8(particlePos.vx);
            particlePosQ8.vy = Q12_TO_Q8(localPart->position0.vy);
            particlePosQ8.vz = Q12_TO_Q8(particlePos.vz);
        }
        else
#endif
        {
            particlePosQ8.vx = Q12_TO_Q8(localPart->position0.vx);
            particlePosQ8.vy = Q12_TO_Q8(localPart->position0.vy);
            particlePosQ8.vz = Q12_TO_Q8(localPart->position0.vz);
        }

        gte_ldv0(&particlePosQ8);
        gte_rtps();

        polyFt3->tpage = 44;
        polyFt3->u0    = 0;
        polyFt3->v0    = 112;
        polyFt3->u1    = 0;
        polyFt3->v1    = 115;
        polyFt3->u2    = 3;
        polyFt3->v2    = 112;

        setPolyFT3(polyFt3);

        gte_stsxy(&polyFt3->x0);
        gte_stszotz(&zScreenStart);

        if (D_800C39A0 == 1 || D_800C39A0 == 4)
        {
            primColorG = primColorB = primColorR = g_WorldEnv_SnowColor.r - (func_80055A50(zScreenStart) >> 2);
        }
        else
        {
            primColorR = g_WorldEnv_SnowColor.r;
            primColorG = g_WorldEnv_SnowColor.g;
            primColorB = g_WorldEnv_SnowColor.b;
        }

        zScreenStart = zScreenStart >> 1;

        if (localPart->stateStep >= 2)
        {
            if (localPart->stateStep < 31)
            {
                primColorR -= (primColorR * localPart->stateStep) >> 5;
                primColorG -= (primColorG * localPart->stateStep) >> 5;
                primColorB -= (primColorB * localPart->stateStep) >> 5;
            }
            else
            {
                localPart->stateStep = 0;
                return;
            }
        }

        polyFt3->r0 = primColorR;
        polyFt3->g0 = primColorG;
        polyFt3->b0 = primColorB;

        if (zScreenStart > 32 && zScreenStart < ORDERING_TABLE_SIZE - 1)
        {
            setSemiTrans(polyFt3, true);
            polyFt3->x1 = polyFt3->x0;
            polyFt3->y2 = polyFt3->y0;

            if (zScreenStart < 64)
            {
                if (Game_FlashlightIsOn())
                {
                    // @unused Optimized out code that makes use of `D_800C39A0`
                    // Unknown what it was actually used for.
                    if (D_800C39A0 == 1 || D_800C39A0 == 4)
                    {
                        primColorB = primColorR;
                    }
                }

                polyFt3->clut = 0x1830;
                polyFt3->u0   = 3;
                polyFt3->v0   = 112;
                polyFt3->u1   = 3;
                polyFt3->v1   = 116;
                polyFt3->u2   = 7;
                polyFt3->v2   = 112;
                polyFt3->x2   = polyFt3->x0 + 4;
                polyFt3->y1   = polyFt3->y0 + 4;
            }
            else if (zScreenStart >= 256)
            {
                if (Game_FlashlightIsOn())
                {
                    // @unused Optimized out code that makes use of `D_800C39A0`
                    // Unknown what it was actually used for.
                    if (D_800C39A0 == 1 || D_800C39A0 == 4)
                    {
                        primColorB = primColorR;
                    }
                }

                polyFt3->clut = 0x19F0;
                polyFt3->x2   = polyFt3->x0 + 1;
                polyFt3->y1   = polyFt3->y0 + 1;
            }
            else
            {
                s32 clutBase;
                if (Game_FlashlightIsOn())
                {
                    // @unused Optimized out code that makes use of `D_800C39A0`, unknown what code actually used it for.
                    if (D_800C39A0 == 1 || D_800C39A0 == 4)
                    {
                        primColorB = primColorR;
                    }
                }

                clutBase = ((zScreenStart >> 5) + 96) << 6;

                do {} while (false); // @hack

                polyFt3->clut = clutBase | 0x30;
                polyFt3->x2   = polyFt3->x0 + 3;
                polyFt3->y1   = polyFt3->y0 + 3;
            }

#if defined(MAP1_S00) || defined(MAP6_S00)
            addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[zScreenStart], polyFt3);
#elif defined(MAP7_S03)
            if (zScreenStart > 64 && D_800F23D4 > Q12(7.0f))
            {
                zScreenStart = 64;
            }
            addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[zScreenStart], polyFt3);
#else
            switch (sharedData_800DD591_0_s00)
            {
                case 2:
                    if ((zScreenStart + 256) < ORDERING_TABLE_SIZE - 1)
                    {
                        addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[zScreenStart + 256], polyFt3);
                    }
                    break;

                default:
                    addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[zScreenStart], polyFt3);
                    break;
            }
#endif
            GsOUT_PACKET_P = (PACKET*)&polyFt3[1];
        }
    }
}
#endif

#if MAP_PARTICLE_HAS_RAIN
void Particle_RainDraw(s_Particle* part, s32 arg1)
{
#if defined(MAP0_S00) || defined(MAP4_S02) || defined(MAP4_S03)
    VECTOR3 sp10;
#endif
    SVECTOR     sp20;
    SVECTOR     sp28;
    SVECTOR     sp30;
    SVECTOR     sp38;
    s32         depth;
    s32         colorComp;
    s32         test;
    u8          color;
    u32         step;
    u8          r;
    u8          g;
    u8          b;
    s32         rScaled;
    s32         gScaled;
    s32         bScaled;
    POLY_FT4*   poly;
    s_Particle* localPart;

    localPart = part;
    poly      = (POLY_FT4*)GsOUT_PACKET_P;

#if defined(MAP4_S02)
    g = 0;
    b = 0;
#endif

#if defined(MAP0_S00) || defined(MAP4_S02) || defined(MAP4_S03)
    if (localPart->stateStep != 0 && sharedData_800DD591_0_s00 != 0)
    {
        sp10    = localPart->position0;
        sp10.vy = -1;

        Particle_BoundaryClamp(&sp10, g_ParticleBox.corners, &g_ParticleBox.corners[1], 0);

        localPart->position0.vx = sp10.vx;
        localPart->position0.vz = sp10.vz;

        if (sp10.vy == 0)
        {
            localPart->stateStep = 0;
            return;
        }
    }
#endif

    switch (localPart->stateStep)
    {
        case 0:
            return;

        case 1:
            sp20.vx = Q12_TO_Q8(localPart->position1.vx);
            sp20.vy = Q12_TO_Q8(localPart->position1.vy);
            sp20.vz = Q12_TO_Q8(localPart->position1.vz);

            gte_ldv0(&sp20);
            gte_rtps();

            sp28.vx = Q12_TO_Q8(localPart->position0.vx);
            sp28.vy = Q12_TO_Q8(localPart->position0.vy);
            sp28.vz = Q12_TO_Q8(localPart->position0.vz);

            gte_stsxy(&poly->x0);
            gte_stszotz(&depth);
            gte_ldv0(&sp28);
            gte_rtps();

            if (D_800C39A0 == 1 || D_800C39A0 == 4)
            {
                color = g_WorldEnv_RainColor.r - (func_80055A50(depth) >> 5);
                r     = color;
                b     = color;
                g     = color;
            }
            else
            {
#if defined(MAP5_S00) || defined(MAP6_S03)
                r = (g_WorldEnv_RainColor.r * 3) >> 1;
                g = (g_WorldEnv_RainColor.g * 3) >> 1;
                b = (g_WorldEnv_RainColor.b * 3) >> 1;
#else
                r = g_WorldEnv_RainColor.r;
                g = g_WorldEnv_RainColor.g;
                b = g_WorldEnv_RainColor.b;
#endif
            }

            depth = depth >> 1;

            gte_stsxy(&poly->x1);

            if (depth > 32 && depth < ORDERING_TABLE_SIZE - 1)
            {
                setPolyFT4(poly);
                setSemiTrans(poly, 1);

#if !defined(MAP4_S02)
                poly->tpage = 44;
#endif

#if defined(MAP0_S00)
                if (g_SysWork.enablePlayerMatchAnim != 0)
                {
                    setRGB0(poly, 0x12, 8, 8);
                }
                else
                {
                    setRGB0(poly, r, g, b);
                }
#elif defined(MAP4_S02)
                if (localPart->movement.vx == 5)
                {
                    poly->tpage = 0x4C;
                    setRGB0(poly, (r * 2) / 3, (g * 2) / 3, b);
                }
                else
                {
                    poly->tpage = 44;
                    setRGB0(poly, r, g, b);
                }
#else
                setRGB0(poly, r, g, b);
#endif

                setUV4(poly, 10, 112, 10, 128, 13, 112, 13, 128);

                if (!(depth > 63 && depth < 256))
                {
                    poly->clut = 0x1A30;
                }
                else
                {
                    poly->clut = (((depth >> 5) + 0x68) << 6) | 0x30;
                }

                poly->x2 = poly->x0 + 1;
                poly->y2 = poly->y0;

                poly->x3 = poly->x1 + 1;
                poly->y3 = poly->y1;

                addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[depth], poly);
                GsOUT_PACKET_P = (PACKET*)(poly + 1);
            }

            return;
    }

    switch (localPart->type)
    {
        case 2:
        case 242:
#if defined(MAP4_S02)
            if (localPart->movement.vx == 5)
            {
                if (localPart->stateStep < 25)
                {
                    colorComp = 0xA3;
                }
                else
                {
                    localPart->stateStep = 0;
                    return;
                }
            }
            else
#endif
            {
                if (localPart->stateStep < 17)
                {
                    colorComp = localPart->stateStep;
                    colorComp = colorComp << arg1;

                    if (localPart->position0.vy == INT_MAX)
                    {
                        return;
                    }
                }
                else
                {
                    localPart->stateStep = 0;
                    return;
                }
            }

            sp20.vx = Q12_TO_Q8(localPart->position0.vx - colorComp);
            sp20.vy = Q12_TO_Q8(localPart->position0.vy);
            sp20.vz = Q12_TO_Q8(localPart->position0.vz - colorComp);

            gte_ldv0(&sp20);
            gte_rtps();

            sp28.vx = sp20.vx;
            sp28.vy = sp20.vy;
            sp28.vz = Q12_TO_Q8(localPart->position0.vz + colorComp);

            gte_stsxy(&poly->x0);
            gte_stszotz(&depth);

            sp30.vx = Q12_TO_Q8(localPart->position0.vx + colorComp);
            sp30.vy = sp20.vy;
            sp30.vz = sp20.vz;

            sp38.vx = sp30.vx;
            sp38.vy = sp20.vy;
            sp38.vz = sp28.vz;

            gte_ldv3(&sp28, &sp30, &sp38);
            gte_rtpt();

            depth = depth >> 1;

#if defined(MAP4_S02)
            if (localPart->movement.vx == 5)
            {
                color = localPart->stateStep;

                if (color >= 9u)
                {
                    r = 0x40 - ((color - 8) * 4);
                }
                else
                {
                    r = 0x40;
                }

                setUV4(poly, 0x20, 0x70, 0x20, 0x7F, 0x2F, 0x70, 0x2F, 0x7F);
                poly->tpage = 0x4C;
            }
            else
#endif
            {
                colorComp >>= (arg1 - 3);
                colorComp   = 0x80 - colorComp;

                if ((D_800C39A0 == 2 || D_800C39A0 == 6))
                {
                    if (Game_FlashlightIsOn())
                    {
                        r = colorComp * 2;
                    }
                    else
                    {
                        r = colorComp;
                    }

                    g = b = colorComp;
                }
                else
                {
                    r = g = b = colorComp;
                }

                step = localPart->stateStep;

                setUV4(poly, 16, 112, 16, 127, 31, 112, 31, 127);

                poly->tpage = 44;

                rScaled = r * step;
                r      -= rScaled >> 4;

                gScaled = g * step;
                g      -= gScaled >> 4;

                bScaled = b * step;
                b      -= bScaled >> 4;
            }

            gte_stsxy3(&poly->x1, &poly->x2, &poly->x3);

            if (depth > 32 && depth < ORDERING_TABLE_SIZE - 1)
            {
                setPolyFT4(poly);
                setSemiTrans(poly, 1);

#if !defined(MAP4_S02)
                if (!(depth > 0x3F && depth < 0x100))
                {
                    poly->clut = 0x1A30;
                }
                else
                {
                    poly->clut = (((depth >> 5) + 0x68) << 6) | 0x30;
                }
#endif

#if defined(MAP0_S00)
                if (g_SysWork.enablePlayerMatchAnim != 0)
                {
                    setRGB0(poly, r + 10, g, b);
                }
                else
                {
                    setRGB0(poly, r, g, b);
                }

#elif defined(MAP4_S02)
                if (localPart->movement.vx == 5)
                {
                    poly->clut = (((depth >> 5) + 0x68) << 6) | 0x30;
                    setRGB0(poly, r, r, r);
                }
                else
                {
                    if (!(depth > 0x3F && depth < 0x100))
                    {
                        poly->clut = 0x1A30;
                    }
                    else
                    {
                        poly->clut = (((depth >> 5) + 0x68) << 6) | 0x30;
                    }

                    setRGB0(poly, r, g, b);
                }
#else
                setRGB0(poly, r, g, b);
#endif

                addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[depth], poly);
                GsOUT_PACKET_P = (PACKET*)(poly + 1);
            }

            return;
    }

    if (localPart->stateStep < 31)
    {
        colorComp = localPart->stateStep;
    }
    else
    {
        colorComp = 0x20;
    }

    if (localPart->stateStep >= 49)
    {
        localPart->stateStep = 0;
        return;
    }

    sp20.vx = Q12_TO_Q8(localPart->position1.vx);
    sp20.vy = Q12_TO_Q8(localPart->position1.vy);
    sp20.vz = Q12_TO_Q8(localPart->position1.vz);

    gte_ldv0(&sp20);
    gte_rtps();

    sp28.vx = Q12_TO_Q8(localPart->position0.vx);
    sp28.vy = Q12_TO_Q8(localPart->position0.vy);
    sp28.vz = Q12_TO_Q8(localPart->position0.vz);

    gte_stsxy(&poly->x0);
    gte_stszotz(&depth);
    gte_ldv0(&sp28);
    gte_rtps();

    depth = depth >> 1;

    gte_stsxy(&poly->x1);

    if (depth > 32 && depth < ORDERING_TABLE_SIZE - 1)
    {
        setPolyFT4(poly);
        setSemiTrans(poly, 1);
        setUV4(poly, 10, 112, 10, 128, 13, 112, 13, 128);
        setRGB0(poly, colorComp, colorComp, colorComp + 0x18);

        poly->tpage = 44;

        if (!(depth > 0x3F && depth < 0x100))
        {
            poly->clut = 0x1A30;
        }
        else
        {
            poly->clut = (((depth >> 5) + 0x68) << 6) | 0x30;
        }

        poly->x2 = poly->x0 + 1;
        poly->y2 = poly->y0;
        poly->x3 = poly->x1 + 1;
        poly->y1++;
        poly->y3 = poly->y1;

        addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[depth], poly);
        GsOUT_PACKET_P = (PACKET*)(poly + 1);
    }
}
#endif

void Particle_SpawnMovementApply(s32 arg0, s_Particle* part, u16* rand, s32* deltaTime)
{
    u16      localRand;
    s16      movement;
    VECTOR3* pos;

    pos = &part->position0;

    switch (arg0)
    {
        case 0:
#if MAP_PARTICLE_HAS_SNOW
            // Random value has been generated in caller and `rand` argument.
            // Add random X offset.
            part->movement.vx += Rng_GenerateIntFromInput(*rand, -7, 7);

            // Random value for Z and Y to share and store in `rand` argument.
            *rand = (u16)Rng_Rand16();

            // Add random Z offset.
            localRand             = *rand;
            part->movement.vz += Rng_GenerateIntFromInput(localRand, -7, 7);

            // Add random Y offset.
            part->movement.vy += Rng_GenerateIntFromInput(*rand, -1, 3);

            // Apply movement to position over time.
            part->position0.vy += TIMESTEP_SCALE_30_FPS(*deltaTime, (part->movement.vy >> 1) << 2);
#endif
            break;

        case 1:
#if MAP_PARTICLE_HAS_RAIN
            part->position1.vx = pos->vx;
            part->position1.vz = pos->vz;
            part->position1.vy = pos->vy - part->movement.vy;
            part->movement.vy += sharedData_800E32D4_0_s00;

            pos->vy += TIMESTEP_SCALE_30_FPS(*deltaTime, part->movement.vy << 2);
#endif
            break;
    }

    // Clamp Y position at 0 and rest.
    if (pos->vy >= Q12(0.0f))
    {
        pos->vy = Q12(0.0f);
    }
}

// @hack Needed for match. Same as version in `rng.h` but without parenthesess around it.
// TODO: Changing `rng.h` version still doesn't let it match though?
#define Rng_GenerateIntFromInput(rand, low, high) \
    (s32)((rand) % (((high) - (low)) + 1)) + (low)

void Particle_MovementUpdate(s32 pass, s_Particle* part, u16* rand, q19_12* deltaTime)
{
    s_CollisionSurface surface;
    q19_12             deltaXCase0;
    q19_12             deltaZCase0;
    q19_12             deltaXCase1;
    q19_12             deltaZCase1;
    u16                localRand;
    s_Particle*        localPart = part;

// Value used in case 0 comparison.
#if defined(MAP1_S06)
    #define UNK_VAL 6
#elif defined(MAP7_S03)
    #define UNK_VAL 8
#else
    #define UNK_VAL 5
#endif

#if MAP_PARTICLE_HAS_SNOW
    deltaXCase0 = g_Particle_PrevPosition.vx - g_Particle_Position.vx;
    deltaZCase0 = g_Particle_PrevPosition.vz - g_Particle_Position.vz;
#elif defined(MAP5_S00) || defined(MAP6_S03)
    deltaXCase1 = g_Particle_PrevPosition.vx - g_Particle_Position.vx;
    deltaZCase1 = g_Particle_PrevPosition.vz - g_Particle_Position.vz;
#endif

    switch (pass)
    {
        case 0:
#if MAP_PARTICLE_HAS_SNOW
            if (localPart->type == ParticleType_Snow)
            {
                localPart->movement.vx += Rng_GenerateIntFromInput(*rand, -7, 7);
                *rand                   = Rng_Rand16();
                localRand               = *rand;
                localPart->movement.vz += Rng_GenerateIntFromInput(localRand, -7, 7);
                localPart->movement.vy += Rng_GenerateIntFromInput(*rand, -1, 3);
            }
            else
            {
                s32 localRand2 = Rng_Rand16();
                s32 temp       = 12;
                localPart->movement.vy += ((temp - g_Particle_PrevPosition.vy) - (g_Particle_PrevPosition.vy << 1) - (g_ParticlesAddedCount[0] >> 4)) +
                                             Rng_GenerateIntFromInput(localRand2, -2, 2);

                limitRange(localPart->movement.vy, 5, 1000);
            }

            localPart->position0.vx += TIMESTEP_SCALE_30_FPS(*deltaTime, (localPart->movement.vx << 2) + g_Particle_SpeedX + deltaXCase0);
            localPart->position0.vy += TIMESTEP_SCALE_30_FPS(*deltaTime, (localPart->movement.vy >> 1) << 2);
            localPart->position0.vz += TIMESTEP_SCALE_30_FPS(*deltaTime, (localPart->movement.vz << 2) + g_Particle_SpeedZ + deltaZCase0);

#if defined(MAP7_S03)
            localPart->position0.vz += (deltaZCase0 - D_800F23D0);
#endif

            if (ABS(localPart->position0.vx) + ABS(localPart->position0.vz) > Q12(UNK_VAL))
            {
                if (g_ParticleCameraMoved)
                {
                    Particle_PositionRandomize(rand, &part->position0, UNK_VAL);
                }
                else
                {
                    localPart->position0.vx = -localPart->position0.vx;
                    localPart->position0.vz = -localPart->position0.vz;
                    localPart->type       += 240;
                }
            }

            if (localPart->position0.vy >= Q12(0.0f))
            {
#if defined(MAP7_S03)
                localPart->position0.vy = Q12(0.0f);
                localPart->movement.vy = Q12(0.0f);
                localPart->stateStep++;
#else
                PushMatrix();
                Collision_SurfaceGet(&surface, localPart->position0.vx + g_Particle_Position.vx, localPart->position0.vz + g_Particle_Position.vz);
                PopMatrix();

                if (localPart->position0.vy >= surface.floorHeight)
                {
#if defined(MAP1_S06)
                    localPart->stateStep = 0;
#else
                    localPart->position0.vy = surface.floorHeight;
                    if (surface.floorHeight < Q12(0.0f) && surface.floorHeight > Q12(-0.2))
                    {
                        localPart->position0.vy = Q12(0.0f);
                    }

                    localPart->movement.vy = Q12(0.0f);
                    localPart->stateStep++;
#endif
                }
#endif
            }
#endif
            break;

        case 1:
#if MAP_PARTICLE_HAS_RAIN
#if !defined(MAP5_S00) && !defined(MAP6_S03)
            deltaXCase1 = g_ParticleVectors1.viewPosition.vx - g_ParticleVectors0.viewPosition.vx;
            deltaZCase1 = g_ParticleVectors1.viewPosition.vz - g_ParticleVectors0.viewPosition.vz;

            localPart->position1.vx = localPart->position0.vx;
            localPart->position1.vz = localPart->position0.vz;
            localPart->position1.vy = localPart->position0.vy - localPart->movement.vy;
#endif
            localPart->position0.vx += TIMESTEP_SCALE_30_FPS(*deltaTime, g_Particle_SpeedX + deltaXCase1);
            localPart->movement.vy += sharedData_800E32D4_0_s00;
            localPart->position0.vy += TIMESTEP_SCALE_30_FPS(*deltaTime, localPart->movement.vy << 2);

#if defined(MAP5_S00) || defined(MAP6_S03)
            localPart->position1.vy = localPart->position0.vy - Q12(0.125f);
#endif

            localPart->position0.vz += TIMESTEP_SCALE_30_FPS(*deltaTime, g_Particle_SpeedZ + deltaZCase1);

#if defined(MAP5_S00) || defined(MAP6_S03)
            localPart->position1.vx = localPart->position0.vx;
            localPart->position1.vz = localPart->position0.vz;
#endif

            if (g_ParticleCameraMoved)
            {
                localPart->position1.vx = localPart->position0.vx;
                localPart->position1.vz = localPart->position0.vz;
            }

            if (ABS(localPart->position0.vx) + ABS(localPart->position0.vz) > Q12(6.0))
            {
                if (g_ParticleCameraMoved)
                {
                    Particle_PositionRandomize(rand, &part->position0, 6);

                    localPart->position1.vx = localPart->position0.vx;
                    localPart->position1.vz = localPart->position0.vz;
                }
                else
                {
                    localPart->position1.vx -= localPart->position0.vx;
                    localPart->position1.vz -= localPart->position0.vz;

                    localPart->position0.vx = -localPart->position0.vx;
                    localPart->position0.vz = -localPart->position0.vz;

                    localPart->position1.vx += localPart->position0.vx;
                    localPart->position1.vz += localPart->position0.vz;
                }
            }

            if (localPart->position0.vy >= 0)
            {
                PushMatrix();
                Collision_SurfaceGet(&surface, localPart->position0.vx + g_Particle_Position.vx, localPart->position0.vz + g_Particle_Position.vz);
                PopMatrix();

                localPart->position0.vy = surface.floorHeight;
                localPart->movement.vx = surface.floorType;

                if (surface.floorType == FloorType_11)
                {
                    localPart->type        = SnowType_HeavyWindy;
                    localPart->position1.vx = localPart->position0.vx;
                    localPart->position1.vz = localPart->position0.vz;
                    localPart->position1.vy = localPart->position0.vy - 96; // TODO: Float.
                }

                localPart->movement.vz = Q12(0.0f);
                localPart->movement.vy = Q12(0.0f);
                localPart->stateStep++;
            }
#endif
            break;
    }
}

void sharedFunc_800CFFF8_0_s00(s32 pass, s_Particle* part, s16* rand)
{
    s32 absX;
    s32 threshold;

#if defined(MAP7_S03)
    #define PASS_ADD 8
#elif defined(MAP1_S06)
    #define PASS_ADD 6
#else
    #define PASS_ADD 5
#endif

    part->position0.vx += g_Particle_PrevPosition.vx - g_Particle_Position.vx;
    part->position0.vz += g_Particle_PrevPosition.vz - g_Particle_Position.vz;

#if defined(MAP7_S03)
    part->position0.vz += ((g_Particle_PrevPosition.vz - g_Particle_Position.vz) - D_800F23D0);
#endif

#if MAP_PARTICLE_HAS_SNOW
    if (pass == 0 && sharedData_800E0CAC_0_s00 == 3)
    {
        part->position0.vx += FP_FROM(g_Particle_SpeedX, Q4_SHIFT);
        part->position0.vz += FP_FROM(g_Particle_SpeedZ, Q4_SHIFT);
    }
#endif

#if !defined(MAP1_S00) && !defined(MAP1_S02) && !defined(MAP1_S03) && !defined(MAP4_S04) && \
    !defined(MAP4_S05) && !defined(MAP5_S00) && !defined(MAP6_S00) && !defined(MAP6_S03) && \
    !defined(MAP7_S03)
    if (sharedData_800DD591_0_s00 != 0)
    {
        return;
    }
#endif

    absX = abs(part->position0.vx);

    // This helps match the original code: `threshold = threshold = ...`
    // Also matches:
    // threshold = FP_TO(pass + 5, Q12_SHIFT); threshold = threshold += 0;
    threshold = threshold = FP_TO(pass + PASS_ADD, Q12_SHIFT);

    if (ABS_ADD(part->position0.vz, absX) > threshold)
    {
        part->stateStep = ParticleState_Spawn;
    }

    if (part->type == SnowType_HeavyWindy || part->type == (0xF0 | SnowType_HeavyWindy)) // TODO: #define 0xF0 constant when purpose is found.
    {
        part->position1.vx = part->position0.vx;
        part->position1.vz = part->position0.vz;
    }
}

#if MAP_PARTICLE_HAS_800D0690

void sharedFunc_800D0690_1_s03(s32 pass, s_Particle* part, s16* rand, q19_12* deltaTime)
{
    q19_12 x = g_Particle_PrevPosition.vx - g_Particle_Position.vx;
    q19_12 z = g_Particle_PrevPosition.vz - g_Particle_Position.vz;

    part->position1.vy  = part->position0.vy - part->movement.vy;
    part->position0.vx += TIMESTEP_SCALE_30_FPS(*deltaTime, x);
    part->movement.vy += Q12(0.001f);
    part->position0.vy += TIMESTEP_SCALE_30_FPS(*deltaTime, part->movement.vy << 2);
    part->position1.vy  = part->position0.vy - Q12(1.0f / 32.0f);
    part->position0.vz += TIMESTEP_SCALE_30_FPS(*deltaTime, z);
    part->position1.vx  = part->position0.vx;
    part->position1.vz  = part->position0.vz;

    if ((ABS(part->position0.vx) + ABS(part->position0.vz)) > Q12(6.0f))
    {
        part->stateStep = ParticleState_Spawn;
    }
}
#endif

void sharedFunc_800CE954_7_s03(s32 pass, s_Particle* part, s16* rand, q19_12* deltaTime)
{
#if defined(MAP7_S03)
    q19_12      deltaX;
    q19_12      deltaZ;
    s_Particle* ptr;

    ptr = part;

    deltaX = g_Particle_PrevPosition.vx - g_Particle_Position.vx;
    deltaZ = g_Particle_PrevPosition.vz - g_Particle_Position.vz;

    if (pass == 0)
    {
        part->position0.vx += deltaX;
        part->position0.vz += deltaZ - ((*deltaTime) * 2);

        if ((ABS(part->position0.vx) + ABS(part->position0.vz)) > Q12(8.0))
        {
            if (g_ParticleCameraMoved)
            {
                Particle_PositionRandomize(rand, &part->position0, 8);
            }
            else
            {
                ptr->position0.vx = -ptr->position0.vx;
                ptr->position0.vz = -ptr->position0.vz;
            }

            ptr->type += 240;
        }
    }
#endif
}

#if defined(MAP0_S00)
bool func_800D012C(VECTOR3* pos, s_func_800CC8FC* unused0, s32* unused1) // 0x800D012C
{
    q19_12 deltaX;
    q19_12 deltaZ;

    switch (g_SysWork.field_234B_4)
    {
        case 1:
            return true;

        case 2:
            deltaX = Q12_TO_Q8(PLAYER_CHARA().position.vx - pos->vx);
            deltaZ = Q12_TO_Q8(PLAYER_CHARA().position.vz - pos->vz);
            return SquareRoot0(SQUARE(deltaX) + SQUARE(deltaZ)) < Q8(1.0f);
    }

    return false;
}
#endif

void Particle_PositionRandomize(u16* arg0, VECTOR3* arg1, s32 arg2)
{
    s32 temp_a0;
    s32 temp_s3;

    temp_a0  = (*arg0 % FP_TO(arg2, Q12_SHIFT)) + (Rng_Rand16() % FP_TO(arg2, Q12_SHIFT));
    temp_s3  = temp_a0 - FP_TO(arg2, Q12_SHIFT);
    arg1->vx = temp_s3;
    temp_s3  = FP_TO(arg2, Q12_SHIFT) - abs(temp_s3);
    arg1->vz = ((*arg0 % temp_s3) + (Rng_Rand16() % temp_s3)) - temp_s3;
}

void Particle_Spawn(s32 arg0, s_Particle* part, u16* rand)
{
    #define SNOW_Y_START_SPEED  Q12(0.0245f)
    #define RAIN_XZ_SPAWN_RANGE 6
#if defined(MAP1_S06)
    #define SNOW_XZ_SPAWN_RANGE 6
#elif defined(MAP7_S03)
    #define SNOW_XZ_SPAWN_RANGE 8
#else
    #define SNOW_XZ_SPAWN_RANGE 5
#endif

    s_Particle* localPart;

    localPart = part;

    switch (arg0)
    {
        case 0:
#if MAP_PARTICLE_HAS_SNOW
            if (sharedData_800DD592_0_s00 != 0)
            {
                part->type = ParticleType_Snow;
            }
            else
            {
                part->type = ParticleType_Unk1;
            }

            // Set start position.
            localPart->position0.vy = g_Particle_Position.vy;

            // Set downward movement.
            localPart->movement.vz = Q12(0.0f);
            localPart->movement.vx = Q12(0.0f);
            localPart->movement.vy = SNOW_Y_START_SPEED;

            // Set random start XZ position.
            Particle_PositionRandomize(rand, part, SNOW_XZ_SPAWN_RANGE);

            // Second position unused for snow.
            localPart->position1.vz = Q12(0.0f);
            localPart->position1.vy = Q12(0.0f);
            localPart->position1.vx = Q12(0.0f);
#endif
            break;

        case 1:
#if MAP_PARTICLE_HAS_RAIN
            localPart->type = ParticleType_Rain;

            // Set start position.
            localPart->position0.vy = g_Particle_Position.vy + Q12(Rng_GenerateInt(0, 2));

#if defined(MAP5_S00) || defined(MAP6_S03)
            localPart->position1.vy = localPart->position0.vy - Q12(0.125f);
            localPart->movement.vy = Q12(0.0091f);
#else
            localPart->position1.vy = g_Particle_Position.vy;
            localPart->movement.vy = Q12(0.03675f);
#endif

            // Set random start XZ position.
            Particle_PositionRandomize(rand, part, RAIN_XZ_SPAWN_RANGE);

            // Copy random XZ to second position.
            localPart->position1.vx = localPart->position0.vx;
            localPart->position1.vz = localPart->position0.vz;
#endif
            break;
    }

    // Step to active state.
    localPart->stateStep++;
}

#endif

#if defined(MAP0_S00)
void func_800D0394(s32 arg0, VECTOR3* vecs) // 0x800D0394
{
    s32 i;

    g_SysWork.field_234B_4 = arg0;

    if (arg0)
    {
        sharedFunc_800D0A60_0_s00(D_800C39A0);

        if (arg0 == 2)
        {
            for (i = 0; i < ARRAY_SIZE(D_800E32DC); i++, vecs++)
            {
                D_800E32DC[i].vx = vecs->vx;
                D_800E32DC[i].vy = vecs->vy;
                D_800E32DC[i].vz = vecs->vz;
            }

            if (D_800DD593)
            {
                for (i = 0; i < ARRAY_SIZE(sharedData_800E34FC_0_s00); i++)
                {
                    // TODO: Is this angle math?
                    sharedData_800E34FC_0_s00[i].field_0.vx = Rng_AddGeneratedUInt(D_800E32DC[(i / 20) + 1].vx, Q12(-0.5f), Q12(0.5f) - 1);
                    sharedData_800E34FC_0_s00[i].field_0.vz = Rng_AddGeneratedUInt(D_800E32DC[(i / 20) + 1].vz, Q12(-0.5f), Q12(0.5f) - 1);
                    sharedData_800E34FC_0_s00[i].field_0.vy = D_800E32DC[(i / 20) + 1].vy;
                    sharedData_800E34FC_0_s00[i].field_11   = 1;
                    sharedData_800E34FC_0_s00[i].field_10   = 1;
                    sharedData_800E34FC_0_s00[i].field_12   = 0;
                    sharedData_800E34FC_0_s00[i].field_C    = 0;
                    sharedData_800E34FC_0_s00[i].field_E    = 0;
                }
            }

            if (D_800DD594)
            {
                for (i = 0; i < ARRAY_SIZE(sharedData_800E330C_0_s00); i++)
                {
                    sharedData_800E330C_0_s00[i].field_0.vx = D_800E32DC[0].vx + Rng_GenerateInt(Q12(-2.5f), Q12(2.5f) - 1);
                    sharedData_800E330C_0_s00[i].field_0.vy = D_800E32DC[0].vy + Rng_GenerateIntFromInput(-Rng_Rand16(), 0, Q12(4.0f) - 1); // TODO: Weird `-Rng_Rand16()`
                    sharedData_800E330C_0_s00[i].field_0.vz = D_800E32DC[0].vz + Rng_GenerateInt(Q12(-2.5f), Q12(2.5f) - 1);

                    sharedData_800E330C_0_s00[i].field_15   = 1;
                    sharedData_800E330C_0_s00[i].field_14   = 1;
                    sharedData_800E330C_0_s00[i].field_C.vx = 0;
                    sharedData_800E330C_0_s00[i].field_C.vz = 0;
                }
            }
        }
    }
}

bool func_800D0600(void) // 0x800D0600
{
    #define DIST_MAX Q12(40.0f)

    if (ABS(PLAYER_CHARA().position.vx - D_800E32DC[0].vx) +
        ABS(PLAYER_CHARA().position.vz - D_800E32DC[0].vz) < DIST_MAX ||
        ABS(PLAYER_CHARA().position.vx - D_800E32DC[1].vx) +
        ABS(PLAYER_CHARA().position.vz - D_800E32DC[1].vz) < DIST_MAX)
    {
        return true;
    }

    return false;
}
#endif

#if defined(MAP0_S00) || defined(MAP0_S01) || defined(MAP0_S02) || defined(MAP1_S06) || \
    defined(MAP2_S00) || defined(MAP2_S02) || defined(MAP3_S00) || defined(MAP3_S01) || \
    defined(MAP3_S06) || defined(MAP4_S02) || defined(MAP4_S03) || defined(MAP5_S01) || \
    defined(MAP5_S03)
void Particle_BoundaryClamp(VECTOR3* point, VECTOR3* lineStart, VECTOR3* lineEnd, s32 flag)
{
    VECTOR3 offset0;
    VECTOR3 offset1;
    VECTOR3 lineStartCpy;
    VECTOR3 lineEndCpy;
    s32     vecPToA_X;
    s32     vecPToA_Z;
    s32     vecPToB_X;
    s32     vecPToB_Z;
    s32     crossTerm1;
    s32     crossTerm2;

    // Early exit check.
    if (lineStart->vx == NO_VALUE)
    {
        point->vy = 1;
        return;
    }

    point->vx += g_Particle_Position.vx;
    offset0.vx = point->vx;

    point->vz += g_Particle_Position.vz;
    offset0.vz = point->vz;

    offset1.vx = offset0.vx;

    lineStartCpy.vx = lineStart->vx;
    lineEndCpy.vx   = lineEnd->vx;

    offset1.vy = offset0.vz;

    lineStartCpy.vy = lineStart->vz;
    lineEndCpy.vy   = lineEnd->vz;

    vecPToA_X = FP_FROM(lineStartCpy.vx - offset0.vx, Q4_SHIFT);
    vecPToB_X = FP_FROM(lineEndCpy.vx   - offset0.vx, Q4_SHIFT);
    vecPToA_Z = FP_FROM(lineStartCpy.vy - offset0.vz, Q4_SHIFT);
    vecPToB_Z = FP_FROM(lineEndCpy.vy   - offset0.vz, Q4_SHIFT);

    crossTerm1 = vecPToA_X * vecPToB_Z;
    crossTerm2 = vecPToA_Z * vecPToB_X;

    if (crossTerm2 < crossTerm1)
    {
        if (!flag)
        {
            if (lineStart->vx == lineEnd->vx)
            {
                if (lineStart->vx < offset0.vx)
                {
                    point->vx -= ABS(offset0.vx - lineStart->vx) * 2;
                }
                else
                {
                    point->vx += ABS(offset0.vx - lineStart->vx) * 2;
                }
            }
            else
            {
                if (lineStart->vz < offset0.vz)
                {
                    point->vz -= ABS(offset0.vz - lineStart->vz) * 2;
                }
                else
                {
                    point->vz += ABS(offset0.vz - lineStart->vz) * 2;
                }
            }
        }

        point->vy = 0;
    }
    else
    {
        point->vy = 1;
    }

    point->vx -= g_Particle_Position.vx;
    point->vz -= g_Particle_Position.vz;
}
#endif

void Particle_EnvironmentSet(s8 arg0, u32 arg1)
{
    s32 temp_a1;
    s32 shiftAmount;
    s32 var_s0;
    s32 var_s1;

    if (arg1 != 127)
    {
        if (arg1 == 2)
        {
            D_800C39A0 = 3;
        }
        else
        {
            D_800C39A0 = arg1;
        }
    }

    sharedData_800E0CBA_0_s00 = arg0;

#if MAP_PARTICLE_HAS_RAIN
    sharedData_800E32D0_0_s00 = 0;
#endif

    switch (arg0)
    {
        case 0:
            var_s1                    = 0;
            var_s0                    = 0;
            sharedData_800DFB6C_0_s00 = 0;
            sharedData_800DFB70_0_s00 = 0;
            break;

        case 1:
            var_s1                    = 1;
            var_s0                    = 0;
            sharedData_800DFB6C_0_s00 = 1;
            sharedData_800DFB70_0_s00 = 0;
            break;

        case 2:
            var_s1                    = 1;
            var_s0                    = 1;
            sharedData_800DFB6C_0_s00 = 1;
            sharedData_800DFB70_0_s00 = 0;
            break;

        case 5:
            var_s1                    = 2;
            var_s0                    = 0;
            sharedData_800DFB6C_0_s00 = 0;
            sharedData_800DFB70_0_s00 = 1;
#if MAP_PARTICLE_HAS_RAIN
            sharedData_800E32D0_0_s00 = 135000;
#endif
            break;

        case 6:
            var_s1                    = 2;
            var_s0                    = 1;
            sharedData_800DFB6C_0_s00 = 0;
            sharedData_800DFB70_0_s00 = 1;
#if MAP_PARTICLE_HAS_RAIN
            sharedData_800E32D0_0_s00 = 135000;
#endif
            break;

        default:
            var_s0                    = 0;
            var_s1                    = 0;
            sharedData_800DFB6C_0_s00 = 0;
            sharedData_800DFB70_0_s00 = 0;
            break;
    }

    if (var_s1 == 0)
    {
        sharedData_800E0CB0_0_s00 = var_s0 * Q12(1.0f);
    }
    else
    {
        sharedData_800E0CB0_0_s00 = (var_s1 * Q12(4.0f)) | (var_s0 * Q12(1.0f));

        temp_a1  = var_s0 + 4;
        shiftAmount = (2 - var_s1) * 6;

        sharedData_800E0CB0_0_s00 = sharedData_800E0CB0_0_s00 | (temp_a1 << shiftAmount) | (temp_a1 << (shiftAmount + 3));
    }

    sharedFunc_800D0A60_0_s00(D_800C39A0);

    sharedData_800E0CA8_0_s00 = var_s1;
    sharedData_800E0CAC_0_s00 = var_s0;
}

void sharedFunc_800D0A60_0_s00(s32 caseArg)
{
    s32 var0;
    s32 var1;
    s32 var2;

    D_800C39A0 = caseArg;

    switch (caseArg)
    {
        case 0:
        case 2:
        case 3:
        case 7:
        case 9:
        case 11:
            var0 = 1;
            var1 = 0;
            var2 = 0;
            break;

        case 4:
            var0 = 0;
            var1 = 1;
            var2 = 0;
            break;

        case 5:
        case 10:
            var0 = 1;
            var1 = 1;
            var2 = 0;
            break;

        case 6:
        case 8:
            var0 = 1;
            var1 = 0;
            var2 = 1;
            break;

        default:
            var2 = 0;
            var1 = 0;
            var0 = 0;
            break;
    }

    sharedData_800E0CB0_0_s00 = (u16)sharedData_800E0CB0_0_s00 | ((var0 | (var1 * 16) | (var2 << 8)) << 16);
    sharedData_800E0CB8_0_s00 = (sharedData_800E0CB8_0_s00 & 0xF) | ((sharedData_800E0CB0_0_s00 >> 12) & 0x1110);
}

bool sharedFunc_800D0B18_0_s00(s32 arg0)
{
    s32 temp_a0_2;
    u16 temp;

    if (g_SysWork.field_234A)
    {
        g_SysWork.field_2349 = arg0;
        g_SysWork.field_2348 = arg0;

        if (arg0 == 0)
        {
            sharedData_800E0CA8_0_s00 = 0;
        }
        else if (arg0 < 5)
        {
            sharedData_800E0CA8_0_s00 = 1;
        }
        else
        {
            sharedData_800E0CA8_0_s00 = 2;
        }

        if (sharedData_800E0CA8_0_s00 != 0)
        {
            sharedData_800E0CAC_0_s00 = (arg0 - 1) % 4u;
        }

        switch (sharedData_800E0CA8_0_s00)
        {
            case 1:
                sharedData_800DFB6C_0_s00 = 1;
                break;

            case 2:
                sharedData_800DFB70_0_s00 = 1;

#if MAP_PARTICLE_HAS_RAIN
                sharedData_800E32D0_0_s00 = 0;
#endif
                break;

            default:
                break;
        }

        sharedData_800E0CB4_0_s00 &= 0xFFF;
        temp                       = (((u16)sharedData_800E0CA8_0_s00 * 4) + sharedData_800E0CAC_0_s00) << Q12_SHIFT;
        temp_a0_2                  = sharedData_800E0CB4_0_s00 | temp;
        sharedData_800E0CB4_0_s00  = (sharedData_800E0CB4_0_s00 | temp) & 0xF000;
        temp_a0_2                  = (temp_a0_2 * 8) & 0xE38;

        if (sharedData_800E0CA8_0_s00 != 0)
        {
            sharedData_800E0CB4_0_s00 += temp_a0_2 + (((sharedData_800E0CAC_0_s00 + 4) << Q12_SHIFT) >> (sharedData_800E0CA8_0_s00 * 6));
        }
        else
        {
            sharedData_800E0CB4_0_s00 += temp_a0_2;
        }

        if (sharedData_800E0CB4_0_s00 != sharedData_800E0CB6_0_s00)
        {
            g_SysWork.field_234A = false;
        }

#if MAP_USE_PARTICLES
        if (sharedData_800E0CAC_0_s00 == 0 || sharedData_800E0CAC_0_s00 == 2)
        {
            g_Particle_PrevPosition.vy = Q12(0.0005f);
        }
        else
        {
            g_Particle_PrevPosition.vy = Q12(0.0f);
        }
#endif

        sharedData_800E0CB8_0_s00 = (sharedData_800E0CB8_0_s00 & ~0xF) + (sharedData_800E0CB4_0_s00 >> Q12_SHIFT);
        return true;
    }

    return false;
}

#if MAP_PARTICLE_HAS_RAIN
void Particle_SoundUpdate(void)
{
    u8 unkValDiv4;

    switch (g_SavegamePtr->mapIdx)
    {
        case MapIdx_MAP0_S00:
        case MapIdx_MAP0_S01: // @unused Checks for `MAP0_S01`, but map itself doesn't contain this func?
        case MapIdx_MAP1_S02:
        case MapIdx_MAP1_S03:
        case MapIdx_MAP4_S02:
            unkValDiv4 = sharedData_800E32CC_0_s00 >> 2; // `sharedData_800E32CC_0_s00 / 4`

            if ((sharedData_800E32CC_0_s00 - sharedData_800DD58C_0_s00) > 15)
            {
                sharedData_800E32CC_0_s00 -= 15;
            }
            else if ((sharedData_800DD58C_0_s00 - sharedData_800E32CC_0_s00) > 15)
            {
                sharedData_800E32CC_0_s00 += 15;
            }

            if (sharedData_800E32CC_0_s00 == 0)
            {
                g_SysWork.field_234B_0 = false;
                Sd_SfxStop(Sfx_Unk1360);
            }

            if (g_SysWork.field_234B_0)
            {
                Sfx_WithFalloffAndPitchPlay(Sfx_Unk1360, &PLAYER_CHARA().position, unkValDiv4, Q12(0.0f), 0);
            }
            else if (sharedData_800E32CC_0_s00 != 0)
            {
                SD_Call(Sfx_Unk1360);
                g_SysWork.field_234B_0 = true;
            }
    }
}
#endif

void Particle_SoundStop(void)
{
    Sd_SfxStop(Sfx_Unk1360);
}

void Particle_HyperBlasterBeamDraw(VECTOR3* vec0, q3_12* rotX, q3_12* rotY)
{
#if !defined(MAP0_S00)
    SVECTOR    startRelPos;
    SVECTOR    endRelPos;
    MATRIX     worldMat; // @unused
    MATRIX     viewMat;
    s_RayTrace trace;
    VECTOR3    beamStart;
    VECTOR3    beamOffset;
    SVECTOR    polyFt3Pos;
    s32        zScreenStart;
    s32        zScreenEnd;
    s32        beamDirX;
    s32        beamDirY;
    s32        beamDirZ;
    bool       hasHit;
    s32        i;
    s32        polyVCoord;
    s32        primCount;
    GsOT*      ot;
    POLY_GT4*  polyGt4;
    POLY_FT3*  polyFt3;

    ot = &g_OrderingTable0[g_ActiveBufferIdx];
    GsInitCoordinate2(NULL, &g_SysWork.coord_22F8);

    g_SysWork.coord_22F8.flg        = false;
    g_SysWork.coord_22F8.coord.t[0] = Q12_TO_Q8(PLAYER_CHARA().position.vx);
    g_SysWork.coord_22F8.coord.t[1] = Q12_TO_Q8(PLAYER_CHARA().position.vy);
    g_SysWork.coord_22F8.coord.t[2] = Q12_TO_Q8(PLAYER_CHARA().position.vz);

    Vw_CoordToWorldAndViewMatrices(&g_SysWork.coord_22F8, &worldMat, &viewMat);

    gte_SetRotMatrix(&viewMat);
    gte_SetTransMatrix(&viewMat);

    beamDirY      = Q12_MULT(D_800AD4C8[PLAYER_COMBAT().weaponAttack].field_0, Math_Cos(*rotY));
    beamDirX      = Q12_MULT(Q12_MULT(D_800AD4C8[PLAYER_COMBAT().weaponAttack].field_0, Math_Sin(*rotY)), Math_Sin(*rotX));
    beamDirZ      = Q12_MULT(Q12_MULT(D_800AD4C8[PLAYER_COMBAT().weaponAttack].field_0, Math_Sin(*rotY)), Math_Cos(*rotX));
    beamStart.vx  = vec0->vx;
    beamStart.vy  = vec0->vy;
    beamStart.vz  = vec0->vz;
    beamOffset.vx = beamDirX;
    beamOffset.vy = beamDirY;
    beamOffset.vz = beamDirZ;

    PushMatrix();
    hasHit = Ray_GeomAndCharaTraceGet(&trace, &beamStart, &beamOffset, &PLAYER_CHARA());
    PopMatrix();

    primCount = hasHit ? (FP_FROM(trace.hitDistance, Q12_SHIFT) + 1) : 16;

    for (i = 0; i < primCount; i++)
    {
        beamStart.vx = vec0->vx + FP_MULTIPLY(beamDirX, i, 4);
        beamStart.vy = vec0->vy + FP_MULTIPLY(beamDirY, i, 4);
        beamStart.vz = vec0->vz + FP_MULTIPLY(beamDirZ, i, 4);

        startRelPos.vx = Q12_TO_Q8(beamStart.vx - PLAYER_CHARA().position.vx);
        startRelPos.vy = Q12_TO_Q8(beamStart.vy - PLAYER_CHARA().position.vy);
        startRelPos.vz = Q12_TO_Q8(beamStart.vz - PLAYER_CHARA().position.vz);
        gte_ldv0(&startRelPos);
        gte_rtps();

        polyGt4 = (POLY_GT4*)GsOUT_PACKET_P;

        beamOffset.vx = vec0->vx + FP_MULTIPLY(beamDirX, i + 1, 4);
        beamOffset.vy = vec0->vy + FP_MULTIPLY(beamDirY, i + 1, 4);
        beamOffset.vz = vec0->vz + FP_MULTIPLY(beamDirZ, i + 1, 4);
        endRelPos.vx  = Q12_TO_Q8(beamOffset.vx - PLAYER_CHARA().position.vx);
        endRelPos.vy  = Q12_TO_Q8(beamOffset.vy - PLAYER_CHARA().position.vy);
        endRelPos.vz  = Q12_TO_Q8(beamOffset.vz - PLAYER_CHARA().position.vz);

        gte_stsxy(&polyGt4->x0);
        gte_stszotz(&zScreenStart);
        zScreenStart = zScreenStart >> 1;
        if (zScreenStart < 1 || zScreenStart >= 0xFF)
        {
            continue;
        }

        if (i == (primCount - 1))
        {
            endRelPos.vx = Q12_TO_Q8(trace.target.vx - PLAYER_CHARA().position.vx);
            endRelPos.vy = Q12_TO_Q8(trace.target.vy - PLAYER_CHARA().position.vy);
            endRelPos.vz = Q12_TO_Q8(trace.target.vz - PLAYER_CHARA().position.vz);
        }

        gte_ldv0(&endRelPos);
        gte_rtps();
        gte_stsxy(&polyGt4->x1);
        polyFt3Pos.vx = polyGt4->x1;
        polyFt3Pos.vy = polyGt4->y1;

        gte_stszotz(&zScreenEnd);
        zScreenEnd = zScreenEnd >> 1;
        if (zScreenEnd >= 1 && zScreenEnd <= 254u)
        {
            setPolyGT4(polyGt4);
            setSemiTrans(polyGt4, true);
            polyGt4->tpage = 44;

            switch (Game_HyperBlasterBeamColorGet())
            {
                case 0:
                    polyGt4->r0 = (i * -17) - 1;
                    polyGt4->g0 = 0;
                    polyGt4->b0 = 0;
                    polyGt4->r1 = (i * -17) - 18;
                    polyGt4->g1 = 0;
                    polyGt4->b1 = 0;
                    polyGt4->r2 = (i * -17) - 1;
                    polyGt4->g2 = 0;
                    polyGt4->b2 = 0;
                    polyGt4->r3 = (i * -17) - 18;
                    polyGt4->g3 = 0;
                    polyGt4->b3 = 0;
                    break;

                case 1:
                    polyGt4->r0 = (i * -17) - 1;
                    polyGt4->g0 = (i * -17) - 1;
                    polyGt4->b0 = 0;
                    polyGt4->r1 = (i * -17) - 18;
                    polyGt4->g1 = (i * -17) - 18;
                    polyGt4->b1 = 0;
                    polyGt4->r2 = (i * -17) - 1;
                    polyGt4->g2 = (i * -17) - 1;
                    polyGt4->b2 = 0;
                    polyGt4->r3 = (i * -17) - 18;
                    polyGt4->g3 = (i * -17) - 18;
                    polyGt4->b3 = 0;
                    break;

                case 2:
                    polyGt4->r0 = 0;
                    polyGt4->g0 = (i * -17) - 1;
                    polyGt4->b0 = 0;
                    polyGt4->r1 = 0;
                    polyGt4->g1 = (i * -17) - 18;
                    polyGt4->b1 = 0;
                    polyGt4->r2 = 0;
                    polyGt4->g2 = (i * -17) - 1;
                    polyGt4->b2 = 0;
                    polyGt4->r3 = 0;
                    polyGt4->g3 = (i * -17) - 18;
                    polyGt4->b3 = 0;
                    break;
            }

            polyVCoord    = 116;
            polyGt4->u0   = 10;
            polyGt4->u1   = 10;
            polyGt4->v0   = polyVCoord;
            polyGt4->v1   = polyVCoord;
            polyGt4->u2   = 13;
            polyGt4->v2   = polyVCoord;
            polyGt4->u3   = 13;
            polyGt4->v3   = polyVCoord;
            polyGt4->clut = ((((zScreenStart >> 5) + 104) << 6) | 0x30);

            if ((polyGt4->x0 < polyGt4->x1 && polyGt4->y0 < polyGt4->y1) ||
                (polyGt4->x0 > polyGt4->x1 && polyGt4->y0 > polyGt4->y1))
            {
                polyGt4->x2 = polyGt4->x0 - 1;
                polyGt4->x3 = polyGt4->x1 - 1;
            }
            else
            {
                polyGt4->x2 = polyGt4->x0 + 1;
                polyGt4->x3 = polyGt4->x1 + 1;
            }

            polyGt4->y2 = polyGt4->y0 + 1;
            polyGt4->y3 = polyGt4->y1 + 1;

            addPrim(&ot->org[zScreenStart], polyGt4);
            GsOUT_PACKET_P = (PACKET*)&polyGt4[1];
        }

        if (i == (primCount - 1))
        {
            polyFt3        = (POLY_FT3*)GsOUT_PACKET_P;
            polyFt3->tpage = 44;
            polyFt3->clut  = 0x1830;
            polyFt3->u0    = 3;
            polyFt3->v0    = 112;
            polyFt3->u1    = 3;
            polyFt3->v1    = 116;
            polyFt3->u2    = 7;
            polyFt3->v2    = 112;

            setPolyFT3(polyFt3);

            polyFt3->x0 = polyFt3Pos.vx;
            polyFt3->y0 = polyFt3Pos.vy;
            polyFt3->x1 = polyFt3->x0;
            polyFt3->y2 = polyFt3->y0;
            polyFt3->x2 = polyFt3->x0 + 4;
            polyFt3->y1 = polyFt3->y0 + 4;

            switch (Game_HyperBlasterBeamColorGet())
            {
                case 0:
                    polyFt3->r0 = 192;
                    polyFt3->g0 = 0;
                    polyFt3->b0 = 0;
                    break;

                case 1:
                    polyFt3->r0 = 192;
                    polyFt3->g0 = 192;
                    polyFt3->b0 = 0;
                    break;

                case 2:
                    polyFt3->r0 = 0;
                    polyFt3->g0 = 192;
                    polyFt3->b0 = 0;
                    break;
            }

            if (zScreenStart >= 1 && zScreenStart < 0xFF)
            {
                addPrim(&ot->org[zScreenStart], polyFt3);
                GsOUT_PACKET_P = (PACKET*)&polyFt3[1];
            }
        }
    }
#endif
}

void Particle_BeamDraw(const VECTOR3* from, const VECTOR3* to)
{
#if !defined(MAP0_S00)
    SVECTOR   fromDelta; // Q23.8
    SVECTOR   toDelta;   // Q23.8
    MATRIX    worldMat;  // @unused
    MATRIX    viewMat;
    s32       depth;
    s32       clutBase;
    GsOT*     ot;
    POLY_FT4* prim;

    ot = &g_OrderingTable0[g_ActiveBufferIdx];

    GsInitCoordinate2(NULL, &g_SysWork.coord_22F8);
    g_SysWork.coord_22F8.flg        = false;
    g_SysWork.coord_22F8.coord.t[0] = Q12_TO_Q8(PLAYER_CHARA().position.vx);
    g_SysWork.coord_22F8.coord.t[1] = Q12_TO_Q8(PLAYER_CHARA().position.vy);
    g_SysWork.coord_22F8.coord.t[2] = Q12_TO_Q8(PLAYER_CHARA().position.vz);

    Vw_CoordToWorldAndViewMatrices(&g_SysWork.coord_22F8, &worldMat, &viewMat);
    gte_SetRotMatrix(&viewMat);
    gte_SetTransMatrix(&viewMat);

    prim = (POLY_FT4*)GsOUT_PACKET_P;

    fromDelta.vx = Q12_TO_Q8(from->vx - PLAYER_CHARA().position.vx);
    fromDelta.vy = Q12_TO_Q8(from->vy - PLAYER_CHARA().position.vy);
    fromDelta.vz = Q12_TO_Q8(from->vz - PLAYER_CHARA().position.vz);

    gte_ldv0(&fromDelta);
    gte_rtps();

    toDelta.vx = Q12_TO_Q8(to->vx - PLAYER_CHARA().position.vx);
    toDelta.vy = Q12_TO_Q8(to->vy - PLAYER_CHARA().position.vy);
    toDelta.vz = Q12_TO_Q8(to->vz - PLAYER_CHARA().position.vz);

    gte_stsxy(&prim->x0);
    gte_stszotz(&depth);

    gte_ldv0(&toDelta);
    gte_rtps();
    gte_stsxy(&prim->x1);

    // Return if `depth / 2` isn't in range `[1, 158]`.
    if (DIV_FAST(depth, 2) < 1 || DIV_FAST(depth, 2) > 158)
    {
        return;
    }

    setPolyFT4TPage(prim, 0x2C);

    switch (g_MapOverlayHdr.field_16)
    {
        case 0:
            break;

        case 1:
            prim->r0 = 0xFF;
            prim->g0 = 0xFF;
            prim->b0 = 0xFF;
            break;

        case 2:
            if (WorldEnv_IsWorldLit())
            {
                prim->r0 = 0xFF;
                prim->g0 = 0xFF;
                prim->b0 = 0xFF;
            }
            else
            {
                prim->r0 = 0x30;
                prim->g0 = 0x30;
                prim->b0 = 0x30;
            }
            break;

        case 3:
            prim->r0 = 0x40;
            prim->g0 = 0x20;
            prim->b0 = 0x00;
            break;
    }

    prim->u0 = 10;
    prim->v0 = 116;
    prim->u1 = 10;
    prim->v1 = 116;
    prim->u2 = 13;
    prim->v2 = 116;
    prim->u3 = 13;
    prim->v3 = 116;

    clutBase   = ((depth >> 6) + 104) << 6;
    depth      = depth >> 1;
    prim->clut = clutBase | 0x30;

    if ((prim->x0 < prim->x1 && prim->y0 < prim->y1) ||
        (prim->x0 > prim->x1 && prim->y0 > prim->y1))
    {
        prim->x2 = prim->x0 - 1;
        prim->x3 = prim->x1 - 1;
    }
    else
    {
        prim->x2 = prim->x0 + 1;
        prim->x3 = prim->x1 + 1;
    }

    prim->y2 = prim->y0 + 1;
    prim->y3 = prim->y1 + 1;

    addPrim(&ot->org[depth], prim);

    GsOUT_PACKET_P = (PACKET*)&prim[1];
#endif
}
