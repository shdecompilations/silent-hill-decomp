#include "inline_no_dmpsx.h"

#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player/player.h"
#include "main/rng.h"
#include "maps/common/particle.h"

// TODO:
// - Make this separate split in each map that uses it instead of `#include`.
//   This file depends on some MAP_X macros from each mapX_sXX.h file though, which the maps .c currently handles including.
// - Move func decls and structs for these out of `shared.h`.

/** Character case switch handling.
 *
 * Maps define one or more `HAS_Chara_X` flags to include their respective switch cases.
 * When only a single case is present (`MAP_CHARA_COUNT == 1`), the
 * macro below replaces `case N:` with `default:` so that the compiler
 * collapses the switch into a direct block, matching single-case maps
 * that skip checking the case entirely.
 *
 * Used by both `Chara_AnimPlaybackStateGet` and `Chara_AnimStartKeyframeIdxGet` in this file.
 */
#if defined(MAP_CHARA_COUNT) && MAP_CHARA_COUNT > 1
    #define CHARA_CASE(id) case id
#else
    #define CHARA_CASE(id) default
#endif

void Chara_MovementReset(s_SubCharacter* npc)
{
    npc->properties.npc.field_F4         = 0;
    npc->properties.npc.freeze           = false;
    npc->properties.npcPath.pathStep     = 0;
    npc->properties.npc.moveSpeed        = Q12(0.0f);
}

void Chara_AnimReset(s_SubCharacter* npc, bool cond)
{
    npc->properties.npc.field_F4 = 1;

    if (cond)
    {
        npc->model.anim.status      = ANIM_STATUS(1, true);
        npc->model.anim.keyframeIdx = 0;
        npc->model.anim.time        = Q12(0.0f);
        npc->model.anim.alpha       = Q12(1.0f);
    }
    else
    {
        npc->properties.npc.controlState = 0;
        npc->model.stateStep             = 0;
    }

    npc->model.anim.flags |= AnimFlag_Unlocked;
}

void Chara_AnimStateSet(s_SubCharacter* npc, s32 controlState)
{
    npc->properties.npc.moveSpeed         = Q12(0.0f);
    npc->properties.npc.resetControlState = false;
    npc->properties.npc.freeze            = false;
    npc->properties.npc.controlState      = controlState;
    npc->model.stateStep                  = 0;
}

void Chara_ControlStateReset(s_SubCharacter* npc)
{
    npc->properties.npc.resetControlState = true;
}

void Chara_AnimLock(s_SubCharacter* chara)
{
    chara->model.anim.flags &= ~AnimFlag_Unlocked;
}

s32 Chara_AnimIsLocked(s_SubCharacter* chara)
{
    return ~(chara->model.anim.flags & AnimFlag_Unlocked);
}

void Chara_AnimUnlock(s_SubCharacter* chara)
{
    chara->model.anim.flags |= AnimFlag_Unlocked;
}

s32 Chara_AnimPlaybackStateGet(s_SubCharacter* chara)
{
    s_AnimInfo* animInfo = NULL;

    switch (chara->model.charaId)
    {
#ifdef HAS_Chara_EndingCybil
        CHARA_CASE(Chara_Cybil):
        CHARA_CASE(Chara_EndingCybil):
            animInfo = &CYBIL_ANIM_INFOS[chara->model.anim.status];
            break;
#endif

#ifdef HAS_Chara_Cybil
        CHARA_CASE(Chara_Cybil):
            animInfo = &CYBIL_ANIM_INFOS[chara->model.anim.status];
            break;
#endif

#ifdef HAS_Chara_MonsterCybil
        CHARA_CASE(Chara_MonsterCybil):
            animInfo = &MONSTER_CYBIL_ANIM_INFOS[chara->model.anim.status];
            break;
#endif

#ifdef HAS_Chara_EndingDahlia
        CHARA_CASE(Chara_Dahlia):
        CHARA_CASE(Chara_EndingDahlia):
            animInfo = &DAHLIA_ANIM_INFOS[chara->model.anim.status];
            break;
#endif

#ifdef HAS_Chara_Dahlia
    #if defined(MAP6_S01) // Some reason Dahlia is default case in M6S01?
        default:
    #else
        CHARA_CASE(Chara_Dahlia):
    #endif
            animInfo = &DAHLIA_ANIM_INFOS[chara->model.anim.status];
            break;
#endif

#ifdef HAS_Chara_Alessa
        CHARA_CASE(Chara_Alessa):
            animInfo = &ALESSA_ANIM_INFOS[chara->model.anim.status];
            break;
#endif

#ifdef HAS_Chara_GhostChildAlessa
        CHARA_CASE(Chara_GhostChildAlessa):
            animInfo = &GHOST_CHILD_ALESSA_ANIM_INFOS[chara->model.anim.status];
            break;
#endif

#ifdef HAS_Chara_EndingKaufmann
        CHARA_CASE(Chara_Kaufmann):
        CHARA_CASE(Chara_EndingKaufmann):
            animInfo = &KAUFMANN_ANIM_INFOS[chara->model.anim.status];
            break;
#endif

#ifdef HAS_Chara_Kaufmann
        CHARA_CASE(Chara_Kaufmann):
            animInfo = &KAUFMANN_ANIM_INFOS[chara->model.anim.status];
            break;
#endif

#ifdef HAS_Chara_Lisa
        CHARA_CASE(Chara_Lisa):
            animInfo = &LISA_ANIM_INFOS[chara->model.anim.status];
            break;
#endif

#ifdef HAS_Chara_BloodyLisa
    #ifdef MAP7_S03 // In M7S03 this checks for Lisa instead of BloodyLisa (while Chara_AnimStartKeyframeIdxGet checks for BloodyLisa instead?)
        CHARA_CASE(Chara_Lisa):
    #else
        CHARA_CASE(Chara_BloodyLisa):
    #endif
            animInfo = &BLOODY_LISA_ANIM_INFOS[chara->model.anim.status];
            break;
#endif

#ifdef HAS_Chara_GhostDoctor
        CHARA_CASE(Chara_GhostDoctor):
            animInfo = &GHOST_DOCTOR_ANIM_INFOS[chara->model.anim.status];
            break;
#endif

#ifdef HAS_Chara_Incubator
        CHARA_CASE(Chara_Incubator):
            animInfo = &INCUBATOR_ANIM_INFOS[chara->model.anim.status];
            break;
#endif

#ifdef HAS_Chara_BloodyIncubator
        CHARA_CASE(Chara_BloodyIncubator):
            animInfo = &BLOODY_INCUBATOR_ANIM_INFOS[chara->model.anim.status];
            break;
#endif

#ifdef HAS_Chara_Cheryl
        CHARA_CASE(Chara_Cheryl):
            animInfo = &CHERYL_ANIM_INFOS[chara->model.anim.status];
            break;
#endif
    }

    if (animInfo->playbackFunc == Anim_PlayOnce)
    {
        if (Anim_DurationGet(chara, animInfo) > Q12(0.0f))
        {
            return (chara->model.anim.keyframeIdx == animInfo->endKeyframeIdx) ? AnimPlaybackState_End :
                                                                                 AnimPlaybackState_Active;
        }
        else
        {
            return (chara->model.anim.keyframeIdx == animInfo->startKeyframeIdx) ? AnimPlaybackState_End :
                                                                                   AnimPlaybackState_Active;
        }
    }

    if (animInfo->playbackFunc == Anim_BlendLinear)
    {
        return AnimPlaybackState_Blend;
    }

    return AnimPlaybackState_Invalid;
}

bool Chara_PathWaypointExecute(s_SubCharacter* chara, s32 arg1, VECTOR3* arg2In, q19_12 angleIn, s32 arg4)
{
    // TODO: Figure out which characters get passed to this function.
    #define ANGLE_THRESHOLD (Q12_ANGLE(360.0) >> 6) // 360 / 64 = 5.625 degrees.

    q7_8     shortestAngle;
    q19_12   angleStep;
    q23_8    dist;
    VECTOR3* arg2; // Q19.12

    angleIn = Q12_ANGLE_ABS(angleIn);
    arg2 = arg2In;

    switch (chara->properties.npcPath.pathStep)
    {
        case 0:
            sharedData_800DF1F8_0_s00 = 0;
            chara->properties.npcPath.pathStep = 1;
            break;

        case 1:
            if (arg2[0].vx == chara->position.vx && arg2[0].vz == chara->position.vz)
            {
                chara->properties.npcPath.pathStep = 6;
            }
            else
            {
                sharedData_800DF1FA_0_s00 = Q12_ANGLE_ABS(ratan2(arg2[0].vx - chara->position.vx,
                                                                 arg2[0].vz - chara->position.vz));
                Math_ShortestAngleGet(chara->rotation.vy, sharedData_800DF1FA_0_s00, &shortestAngle);

                if (ABS(shortestAngle) < ANGLE_THRESHOLD)
                {
                    chara->rotation.vy = sharedData_800DF1FA_0_s00;
                    chara->properties.npcPath.pathStep = 3;
                }
                else
                {
                    if (shortestAngle < Q12_ANGLE(0.0f))
                    {
                        chara->properties.npcPath.field_FC = 4;
                        chara->properties.npcPath.controlState = 4;
                        chara->model.stateStep = 0;
                    }
                    else
                    {
                        chara->properties.npcPath.field_FC = 3;
                        chara->properties.npcPath.controlState = 3;
                        chara->model.stateStep = 0;
                    }

                    chara->properties.npcPath.pathStep = 2;
                }
            }
            break;

        case 2:
            Math_ShortestAngleGet(chara->rotation.vy, sharedData_800DF1FA_0_s00, &shortestAngle);
            if (ABS(shortestAngle) < ANGLE_THRESHOLD)
            {
                chara->rotation.vy = sharedData_800DF1FA_0_s00;
                chara->properties.npcPath.pathStep = 3;
            }
            break;

        case 3:
            dist = SquareRoot0(Q12_2D_DISTANCE_SQR(arg2[0], chara->position));

            if (ABS((int)dist) < Q8(0.15f)) // @hack Needs to be `int` for `ABS` to match?
            {
                chara->properties.npcPath.controlState = 0;
                chara->model.stateStep = 0;
                chara->properties.npcPath.pathStep = 8;
            }
            else
            {
                chara->properties.npcPath.controlState = arg1;
                chara->model.stateStep = 0;
                chara->properties.npcPath.field_FC = arg1;
                chara->properties.npcPath.pathStep = 4;
            }
            break;

        case 4:
            sharedData_800DF1FA_0_s00 = Q12_ANGLE_ABS(ratan2(arg2[sharedData_800DF1F8_0_s00].vx - chara->position.vx,
                                                      arg2[sharedData_800DF1F8_0_s00].vz - chara->position.vz));
            sharedData_800DF1F4_0_s00 = SquareRoot0(Q12_2D_DISTANCE_SQR(arg2[sharedData_800DF1F8_0_s00], chara->position));

            chara->properties.npcPath.pathStep = 5;
            break;

        case 5:
            // Test against next `VECTOR3` unless last one has been reached.
            if ((sharedData_800DF1F8_0_s00 + 1) < arg4)
            {
                sharedData_800DF1FA_0_s00 = ratan2(arg2[sharedData_800DF1F8_0_s00 + 1].vx - chara->position.vx,
                                                   arg2[sharedData_800DF1F8_0_s00 + 1].vz - chara->position.vz);
            }
            else
            {
                sharedData_800DF1FA_0_s00 = ratan2(arg2[sharedData_800DF1F8_0_s00].vx - chara->position.vx,
                                                   arg2[sharedData_800DF1F8_0_s00].vz - chara->position.vz);
            }

            sharedData_800DF1FA_0_s00 = Q12_ANGLE_ABS(sharedData_800DF1FA_0_s00);

            Math_ShortestAngleGet(chara->rotation.vy, sharedData_800DF1FA_0_s00, &shortestAngle);

            // Turn toward.
            angleStep = TIMESTEP_SCALE_30_FPS(g_DeltaTime, (shortestAngle < Q12_ANGLE(45.0f)) ? Q12_ANGLE(2.9f) :
                                                                                                Q12_ANGLE(22.5f));

            if (ABS(shortestAngle) < ANGLE_THRESHOLD)
            {
                chara->rotation.vy = sharedData_800DF1FA_0_s00;
            }
            else if (shortestAngle < Q12_ANGLE(0.0f))
            {
                chara->rotation.vy -= angleStep;
            }
            else
            {
                chara->rotation.vy += angleStep;
            }

            chara->rotation.vy = Q12_ANGLE_ABS(chara->rotation.vy);

            dist = SquareRoot0(Q12_2D_DISTANCE_SQR(arg2[sharedData_800DF1F8_0_s00], chara->position));

            // TODO: Should be `arg1 * Q12(0.15f)`, but compiler splits out one distance for some reason.
            if (dist < (((arg1 - 1) * Q12(0.15f)) + Q12(0.15f)) >> 4 ||
                (sharedData_800DF1F4_0_s00 < dist && sharedData_800DF1F8_0_s00 != arg4 - 1))
            {
                sharedData_800DF1F8_0_s00++;
                dist = SquareRoot0(Q12_2D_DISTANCE_SQR(arg2[sharedData_800DF1F8_0_s00], chara->position));

                if (sharedData_800DF1F8_0_s00 == arg4)
                {
                    chara->properties.npcPath.pathStep = 6;
                }
                else
                {
                    chara->properties.npcPath.pathStep = 4;
                }
            }

            sharedData_800DF1F4_0_s00 = dist;
            break;

        case 6:
            Math_ShortestAngleGet(chara->rotation.vy, angleIn, &shortestAngle);
            if (ABS(shortestAngle) < ANGLE_THRESHOLD)
            {
                chara->rotation.vy = angleIn;
                chara->properties.npcPath.pathStep = 8;
            }
            else
            {
                if (shortestAngle < Q12_ANGLE(0.0f))
                {
                    chara->properties.npcPath.field_FC = 4;
                    chara->properties.npcPath.controlState = 4;
                    chara->model.stateStep = 0;
                }
                else
                {
                    chara->properties.npcPath.field_FC = 3;
                    chara->properties.npcPath.controlState = 3;
                    chara->model.stateStep = 0;
                }

                chara->properties.npcPath.pathStep = 7;
            }
            break;

        case 7:
            Math_ShortestAngleGet(chara->rotation.vy, angleIn, &shortestAngle);
            if (ABS(shortestAngle) < ANGLE_THRESHOLD)
            {
                chara->properties.npcPath.field_FC = arg1 - 1;
                chara->rotation.vy = angleIn;
                chara->properties.npcPath.controlState = 0;
                chara->model.stateStep = 0;
                chara->properties.npcPath.pathStep = 8;
            }
            break;

        case 8:
            chara->properties.npcPath.field_F4 = 1;
            sharedData_800DF1FA_0_s00 = 0;
            chara->properties.npcPath.pathStep = 0;
            chara->properties.npcPath.field_124 = 0;
            return true;
    }

    return false;
}

void Chara_VisibleSet(s_SubCharacter* chara)
{
    chara->model.anim.flags |= AnimFlag_Visible;
}

void Chara_InvisibleSet(s_SubCharacter* chara)
{
    chara->model.anim.flags &= ~AnimFlag_Visible;
}

bool Npc_FootstepSoundPlay(s32 animStatus, s_SubCharacter* chara, s32 keyframeIdx0, s32 keyframeIdx1, s32 sfxId, s32 pitch)
{
    // TODO: Seems each of the s_PropsNpcs characters get passed to this func.
    // May share the same `flags` values based on code below?

    if (chara->model.anim.status != animStatus)
    {
        return false;
    }

    if (chara->model.anim.keyframeIdx >= keyframeIdx1)
    {
        if (!(chara->properties.npc.flags & (1 << 4)))
        {
            Sfx_WithPitchPlay(sfxId, &chara->position, Q8(0.5f), pitch);
            chara->properties.npc.flags |= 1 << 4;
            return true;
        }
    }
    else
    {
        chara->properties.npc.flags &= ~(1 << 4);
    }

    if (chara->model.anim.keyframeIdx >= keyframeIdx0)
    {
        if (!(chara->properties.npc.flags & (1 << 5)))
        {
            Sfx_WithPitchPlay(sfxId, &chara->position, Q8(0.5f), pitch);
            chara->properties.npc.flags |= 1 << 5;
            return true;
        }
    }
    else
    {
        chara->properties.npc.flags &= ~(1 << 5);
    }

    return false;
}

bool Npc_SfxPlay(s32 animStatus, s_SubCharacter* chara, s32 keyframeIdx, s32 sfxId)
{
    if (chara->model.anim.status == animStatus)
    {
        if (chara->model.anim.keyframeIdx >= keyframeIdx)
        {
            if (!(chara->properties.npc.flags & PlayerFlag_SfxActive))
            {
                switch (sfxId)
                {
#if defined(MAP6_S04)
                    case Sfx_Unk1620:
                        Sfx_WithFlagsPlay(sfxId, &chara->position, Q8(0.25f), SfxFlag_None);
                        break;

                    case Sfx_Unk1621:
                        Sfx_WithFlagsPlay(sfxId, &chara->position, Q8(0.25f), SfxFlag_None);
                        break;

                    case Sfx_Unk1622:
                    case Sfx_Unk1638:
                        Sfx_WithFlagsPlay(sfxId, &chara->position, Q8(0.75f), SfxFlag_None);
                        break;

                    case Sfx_Unk1631:
                    case Sfx_Unk1632:
                        Sfx_WithFlagsPlay(sfxId, &chara->position, Q8(0.25f), SfxFlag_None);
                        break;

#elif defined(MAP3_S00) || defined(MAP5_S02) || defined(MAP5_S03)
                    case Sfx_Unk1492:
                        Sfx_WithFlagsPlay(sfxId, &chara->position, Q8_CLAMPED(1.0f), SfxFlag_NoDistAtten);
                        break;

                    case Sfx_Unk1594:
                        Sfx_WithFlagsPlay(sfxId, &chara->position, Q8(0.5f), SfxFlag_None);
                        break;

#elif defined(MAP3_S04) || defined(MAP4_S04) || defined(MAP7_S00)
                    case Sfx_Unk1639:
                        Sfx_WithFlagsPlay(sfxId, &chara->position, Q8(0.25f), SfxFlag_None);
                        break;

#elif defined(MAP0_S01) || defined(MAP4_S01)
                    case Sfx_Unk1368:
                    case Sfx_Unk1369:
                    case Sfx_Unk1370:
                        Sfx_WithFlagsPlay(sfxId, &chara->position, Q8(0.25f), SfxFlag_None);
                        break;

#elif defined(MAP2_S01) || defined(MAP3_S06)
                    case Sfx_Unk1488:
                    case Sfx_Unk1496:
                        Sfx_WithFlagsPlay(sfxId, &chara->position, Q8(0.25f), SfxFlag_None);
                        break;

#elif defined(MAP6_S01)
                    // TODO: This is made of two blocks above, maybe can be merged somehow.
                    case Sfx_Unk1488:
                    case Sfx_Unk1496:
                        Sfx_WithFlagsPlay(sfxId, &chara->position, Q8(0.25f), SfxFlag_None);
                        break;

                    case Sfx_Unk1368:
                    case Sfx_Unk1369:
                    case Sfx_Unk1370:
                        Sfx_WithFlagsPlay(sfxId, &chara->position, Q8(0.25f), SfxFlag_None);
                        break;

#elif defined(MAP7_S03)
                    case Sfx_Unk1539:
                    case Sfx_Unk1674:
                    case Sfx_Unk1675:
                        Sfx_WithFlagsPlay(sfxId, &chara->position, Q8(1.0f / 8.0f), SfxFlag_None);
                        break;

                    case Sfx_Unk1622:
                        Sfx_WithFlagsPlay(sfxId, &chara->position, Q8(0.75f), SfxFlag_None);
                        break;

#endif
                    default:
                        Sfx_WithFlagsPlay(sfxId, &chara->position, Q8(0.5f), SfxFlag_None);
                        break;
                }

                chara->properties.npc.flags |= PlayerFlag_SfxActive;
                return true;
            }
        }
        else
        {
            chara->properties.npc.flags &= ~PlayerFlag_SfxActive;
        }
    }

    return false;
}

s32 Chara_AnimStartKeyframeIdxGet(s_SubCharacter* chara)
{
    s_AnimInfo* animInfo = NULL;

    switch (chara->model.charaId)
    {
#if defined(MAP5_S02) || defined(MAP5_S03)
        // `animInfo` left as `NULL`.
#else

#ifdef HAS_Chara_EndingCybil
        CHARA_CASE(Chara_Cybil):
        CHARA_CASE(Chara_EndingCybil):
            animInfo = &CYBIL_ANIM_INFOS[chara->model.anim.status];
            break;
#endif

#ifdef HAS_Chara_Cybil
        CHARA_CASE(Chara_Cybil):
            animInfo = &CYBIL_ANIM_INFOS[chara->model.anim.status];
            break;
#endif

#ifdef HAS_Chara_MonsterCybil
        CHARA_CASE(Chara_MonsterCybil):
            animInfo = &MONSTER_CYBIL_ANIM_INFOS[chara->model.anim.status];
            break;
#endif

#ifdef HAS_Chara_EndingDahlia
        CHARA_CASE(Chara_Dahlia):
        CHARA_CASE(Chara_EndingDahlia):
            animInfo = &DAHLIA_ANIM_INFOS[chara->model.anim.status];
            break;
#endif

#ifdef HAS_Chara_Dahlia
    #if defined(MAP6_S01) // Some reason Dahlia is default case in M6S01?
        default:
    #else
        CHARA_CASE(Chara_Dahlia):
    #endif
            animInfo = &DAHLIA_ANIM_INFOS[chara->model.anim.status];
            break;
#endif

#ifdef HAS_Chara_Alessa
        CHARA_CASE(Chara_Alessa):
            animInfo = &ALESSA_ANIM_INFOS[chara->model.anim.status];
            break;
#endif

#ifdef HAS_Chara_GhostChildAlessa
        CHARA_CASE(Chara_GhostChildAlessa):
            animInfo = &GHOST_CHILD_ALESSA_ANIM_INFOS[chara->model.anim.status];
            break;
#endif

#ifdef HAS_Chara_EndingKaufmann
        CHARA_CASE(Chara_Kaufmann):
        CHARA_CASE(Chara_EndingKaufmann):
            animInfo = &KAUFMANN_ANIM_INFOS[chara->model.anim.status];
            break;
#endif

#ifdef HAS_Chara_Kaufmann
        CHARA_CASE(Chara_Kaufmann):
            animInfo = &KAUFMANN_ANIM_INFOS[chara->model.anim.status];
            break;
#endif

// M7S02 uses Chara_GhostDoctor in Chara_AnimPlaybackStateGet, but includes Lisa/BloodyLisa here?
#if defined(HAS_Chara_Lisa) || defined(HAS_Chara_GhostDoctor)
        CHARA_CASE(Chara_Lisa):
            animInfo = &LISA_ANIM_INFOS[chara->model.anim.status];
            break;
#endif

#if defined(HAS_Chara_BloodyLisa) || defined(HAS_Chara_GhostDoctor)
        CHARA_CASE(Chara_BloodyLisa):
            animInfo = &BLOODY_LISA_ANIM_INFOS[chara->model.anim.status];
            break;
#endif

#ifdef HAS_Chara_Incubator
        CHARA_CASE(Chara_Incubator):
            animInfo = &INCUBATOR_ANIM_INFOS[chara->model.anim.status];
            break;
#endif

#ifdef HAS_Chara_BloodyIncubator
        CHARA_CASE(Chara_BloodyIncubator):
            animInfo = &BLOODY_INCUBATOR_ANIM_INFOS[chara->model.anim.status];
            break;
#endif

#ifdef HAS_Chara_Cheryl
        CHARA_CASE(Chara_Cheryl):
            animInfo = &CHERYL_ANIM_INFOS[chara->model.anim.status];
            break;
#endif
#endif
    }

    return animInfo->startKeyframeIdx;
}

void Chara_CollisionReset(s_SubCharacter* chara)
{
    chara->model.stateStep = 0;

    chara->collision.box.topExtent            = Q12(0.0f);
    chara->collision.box.bottomExtent         = Q12(0.0f);
    chara->collision.box.offsetY              = Q12(0.0f);
    chara->collision.cylinder.outerRadius     = Q12(0.0f);
    chara->collision.cylinder.innerRadius     = Q12(0.0f);
    chara->collision.shapeOffsets.cylinder.vz = Q12(0.0f);
    chara->collision.shapeOffsets.cylinder.vx = Q12(0.0f);
    chara->collision.shapeOffsets.box.vz      = Q12(0.0f);
    chara->collision.shapeOffsets.box.vx      = Q12(0.0f);

    chara->properties.player.floorHeight = Q12(0.0f);
    chara->properties.player.afkTimer    = Q12(0.0f);
    chara->properties.player.floorHeight = Q12(0.0f);
    chara->properties.player.field_F0    = 0;
    chara->properties.player.field_F4    = Q12_ANGLE(0.0f);

    chara->model.controlState++;
    chara->collision.state = CharaCollisionState_Npc;
}

#undef CHARA_CASE
