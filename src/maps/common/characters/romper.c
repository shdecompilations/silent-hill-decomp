#include "bodyprog/bodyprog.h"
#include "bodyprog/events/npc_main.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player/player.h"
#include "main/rng.h"
#include "maps/common/shared.h"
#include "maps/common/characters/romper.h"

#define romperProps romper->properties.romper

void Romper_Update(s_SubCharacter* romper, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords)
{
    if (romper->model.controlState == 0)
    {
        Romper_Init(romper);
    }
    else if (g_DeltaTime != Q12(0.0f))
    {
        sharedFunc_800E6420_2_s02(romper);
        Romper_ControlUpdate(romper);
        sharedFunc_800E8730_2_s02(romper);
        Romper_CollisionUpdate(romper);
    }

    sharedFunc_800E8A40_2_s02(romper, anmHdr, boneCoords);
    sharedFunc_800E60FC_2_s02(romper);
}

void Romper_Init(s_SubCharacter* romper)
{
    #define HEALTH_BASE      Q12(450.0f)
    #define HEALTH_BONUS_MAX Q12(112.5f)

    s32 i;
    s32 temp_a0;
    s32 var_v0;

    romperProps.flags           = RomperFlag_None;
    romper->model.anim.alpha = Q12(0.0f);

    if (g_SavegamePtr->gameDifficulty == GameDifficulty_Easy)
    {
        romper->health = HEALTH_BASE - ((s32)Rng_Rand16() % HEALTH_BONUS_MAX);
    }
    else
    {
        if (g_SavegamePtr->gameDifficulty == GameDifficulty_Normal)
        {
            romper->health = HEALTH_BASE + ((s32)Rng_Rand16() % HEALTH_BONUS_MAX);
        }
        else
        {
            var_v0 = (s32)Rng_Rand16();
            temp_a0 = var_v0;
            if (temp_a0 < Q12(0.0f))
            {
                var_v0 = temp_a0 + (Q12(0.5f) - 1);
            }

            // TODO: Shifts are equivalent to `var_v0_2 % Q12(0.5f)`.
            romper->health = Q12_MULT_PRECISE(((temp_a0 - ((var_v0 >> 11) << 11)) + Q12(1.5f)), HEALTH_BASE);
        }
    }

    romper->moveSpeed       = Q12(0.0f);
    romper->collision.state = CharaCollisionState_Npc;
    romper->headingAngle    = romper->rotation.vy;
    Chara_PropsClear(romper);

    romper->model.controlState = RomperControl_WalkForward;
    Chara_AnimSet(romper, ANIM_STATUS(RomperAnim_WalkForward, true), 147);

    romperProps.field_F0     = 6;
    romperProps.rotationY_F2 = romper->rotation.vy;
    romper->moveSpeed = Q12(0.02175f);

    ModelAnim_AnimInfoSet(&romper->model.anim, ROMPER_ANIM_INFOS);
    Chara_DamageClear(romper);

    romperProps.targetPositionX_FC  =
    romperProps.positionZ_110       = romper->position.vx;
    romperProps.targetPositionZ_100 =
    romperProps.positionX_108       = romper->position.vz;

    #undef HEALTH_BASE
    #undef HEALTH_BONUS_MAX
}

void Romper_FootstepSfxPlay(s_SubCharacter* romper, s16 arg1, s16 arg2, u8* arg3)
{
    q19_12 keyframeIdx;
    q19_12 arg1Plus2;
    q23_8  pitch;
    q23_8  vol;

    keyframeIdx = FP_FROM(romper->model.anim.time, Q12_SHIFT);
    arg1Plus2   = arg1 + arg2;
    pitch       = Q8(0.0f);
    vol         = Q8(0.0f);

    if (keyframeIdx >= arg1Plus2 &&
        keyframeIdx < (arg1Plus2 + 2))
    {
        if (*arg3 == 0)
        {
            switch (arg1)
            {
                case 2:
                    pitch = Q8(0.125f);
                    vol   = Q8(0.765625f);
                    break;

                case 8:
                case 15:
                case 93:
                    pitch = Q8(-0.125f);
                    vol   = Q8(0.5f);
                    break;

                case 109:
                    pitch = Q8(0.0f);
                    vol   = Q8(0.5f);
                    break;

                case 110:
                default:
                    pitch = -Q8(0.25f);
                    vol   = Q8(0.125f);
                    break;
            }

            Sfx_WithPitchPlay(Sfx_RomperFootstep, &romper->position, vol, pitch + Rng_GenerateUInt(-7, 8));
            *arg3 = 1;
        }
    }
    else
    {
        *arg3 = 0;
    }
}

void sharedFunc_800E60FC_2_s02(s_SubCharacter* romper)
{
    q19_12 var_s1;

    switch (romper->model.anim.status)
    {
        case ANIM_STATUS(RomperAnim_RunToJump, true):
            Romper_FootstepSfxPlay(romper, 2, 2, &romperProps.field_114);
            break;

        case ANIM_STATUS(RomperAnim_JumpToIdleStart, true):
            Romper_FootstepSfxPlay(romper, 8, 5, &romperProps.field_114);
            break;

        case ANIM_STATUS(RomperAnim_JumpToRunForward, true):
            Romper_FootstepSfxPlay(romper, 15, 2, &romperProps.field_114);
            break;

        case ANIM_STATUS(RomperAnim_JumpStunStart, true):
            Romper_FootstepSfxPlay(romper, 93, 2, &romperProps.field_115);
            Romper_FootstepSfxPlay(romper, 93, 3, &romperProps.field_114);
            break;

        case ANIM_STATUS(RomperAnim_RunForwardLoop, true):
            Romper_FootstepSfxPlay(romper, 109, 7, &romperProps.field_115);
            Romper_FootstepSfxPlay(romper, 109, 20, &romperProps.field_114);
            break;

        case ANIM_STATUS(RomperAnim_CreepForward, true):
            Romper_FootstepSfxPlay(romper, 131, 8, &romperProps.field_115);
            Romper_FootstepSfxPlay(romper, 131, 15, &romperProps.field_114);
            break;

        default:
            romperProps.field_115 = 0;
            romperProps.field_114 = 0;
            break;
    }

    if (romperProps.field_116 != Q12(0.0f))
    {
        romperProps.field_116 -= g_DeltaTime;
        if (romperProps.field_116 < Q12(0.0f))
        {
            romperProps.field_116 = Q12(0.0f);
        }
    }
    else if (romperProps.flags & RomperFlag_7)
    {
        romperProps.field_116 = Q12(0.8f);
        Sfx_WithFlagsPlay(Sfx_Unk1400, &romper->position, Q8(0.5f), SfxFlag_None);
    }

    romperProps.flags &= ~RomperFlag_7;

    if (romper->model.anim.status == ANIM_STATUS(RomperAnim_JumpToIdleEnd, true))
    {
        if (romperProps.timer_118 == Q12(0.0f))
        {
            Sfx_WithFlagsPlay(Sfx_Unk1401, &romper->position, Q8(0.5f), SfxFlag_None);
            romperProps.timer_118 = Q12(3.2f);
        }
        else
        {
            romperProps.timer_118 -= g_DeltaTime;
            if (romperProps.timer_118 < Q12(0.0f))
            {
                romperProps.timer_118 = Q12(0.0f);
            }
        }
    }
    else if (romperProps.timer_118 > Q12(0.0f))
    {
        romperProps.timer_118 = Q12(0.0f);
        Sd_SfxStop(Sfx_Unk1401);
    }

    if (romperProps.field_11A != 0)
    {
        Sfx_WithFlagsPlay(Sfx_Unk1402, &romper->position, Rng_GenerateUInt(120, 135), SfxFlag_None);
        romperProps.field_11A = 0;
    }

    if (romper->model.anim.status != ANIM_STATUS(RomperAnim_JumpToIdleEnd, true) &&
        romperProps.field_116 == 0 &&
        romper->health > Q12(0.0f))
    {
        if (romperProps.timer_11C != Q12(0.0f))
        {
            romperProps.timer_11C -= g_DeltaTime;
            if (romperProps.timer_11C < Q12(0.0f))
            {
                romperProps.timer_11C = Q12(0.0f);
            }
        }
        else
        {
            // TODO: Is this Q6?
            if (romper->health <= Q12(225.0f))
            {
                var_s1 = Q12_TO_Q6(Q12_DIV(romper->health, Q12(450.0f))) + Q6(0.875f);
            }
            else
            {
                var_s1 = Q6(1.875f);
            }

            Sfx_WithFlagsPlay(Sfx_Unk1404, &romper->position, Rng_AddGeneratedUInt(var_s1, 0, 15) >> 1, SfxFlag_None);
            romperProps.timer_11C = Rng_GenerateInt(Q12(4.5f), Q12(6.5f) - 1);
        }
    }
}

void sharedFunc_800E6420_2_s02(s_SubCharacter* romper)
{
    s_CollisionSurface surface;
    s32                keyframeIdx;
    q19_12             unkHealth;
    s32                prevControlState;

    if (romper->health > Q12(0.0f))
    {
        Collision_SurfaceGet(&surface, romper->position.vx, romper->position.vz);

        if (surface.floorType == FloorType_None)
        {
            if (Rng_GenerateInt(Q12(4.0f), Q12(8.0f) - 1) < romper->position.vy)
            {
                romper->damage.amount = Q12(3000.0f);
            }
        }
    }

    if (romper->damage.amount == Q12(0.0f) || romper->health <= Q12(0.0f))
    {
        Chara_DamageClear(romper);
        return;
    }

    prevControlState = romper->model.controlState;

    romper->health = MAX(romper->health - romper->damage.amount, Q12(0.0f));
    romperProps.flags |= RomperFlag_7;

    if (g_SavegamePtr->gameDifficulty == GameDifficulty_Hard)
    {
        unkHealth = Q12(400.0f);
    }
    else
    {
        unkHealth = Q12(200.0f);
    }

    if (unkHealth < romper->health)
    {
        romperProps.targetPositionX_FC  = PLAYER_CHARA().position.vx;
        romperProps.targetPositionZ_100 = PLAYER_CHARA().position.vz;
        romperProps.field_10E     = 0;

        romperProps.rotationY_F2 = ratan2(PLAYER_CHARA().position.vx - romper->position.vx,
                                          PLAYER_CHARA().position.vz - romper->position.vz);

        switch (romper->model.anim.status)
        {
            case ANIM_STATUS(RomperAnim_RunForwardLoop, false):
                romper->model.anim.time = Q12(109.0f);

            case ANIM_STATUS(RomperAnim_RunForwardLoop, true):
                keyframeIdx = FP_FROM(romper->model.anim.time, Q12_SHIFT) - 109;
                if (keyframeIdx > 0)
                {
                    if (keyframeIdx < 7)
                    {
                        romper->model.anim.status = ANIM_STATUS(RomperAnim_WalkToRunForwardLeftStart, true);
                    }
                    else if (keyframeIdx < 11)
                    {
                        romper->model.anim.status = ANIM_STATUS(RomperAnim_WalkToRunForwardLeftEnd, false);
                    }
                    else if (keyframeIdx > 17)
                    {
                        romper->model.anim.status = ANIM_STATUS(RomperAnim_WalkToRunForwardRightEnd, false);
                    }
                    else
                    {
                        romper->model.anim.status = ANIM_STATUS(RomperAnim_WalkToRunForwardRightStart, true);
                    }
                }
                else
                {
                    romper->model.anim.status = ANIM_STATUS(RomperAnim_WalkToRunForwardRightEnd, false);
                }

                if (!ANIM_STATUS_IS_ACTIVE(romper->model.anim.status))
                {
                    romper->model.controlState = RomperControl_6;
                }
                break;

            case ANIM_STATUS(RomperAnim_WalkForward, false):
                romper->model.anim.time = Q12(147.0f);

            case ANIM_STATUS(RomperAnim_WalkForward, true):
                keyframeIdx = FP_FROM(romper->model.anim.time, Q12_SHIFT) - 147;
                if (keyframeIdx < 2)
                {
                    romper->model.anim.status = ANIM_STATUS(RomperAnim_WalkToRunForwardRightEnd, false);
                }
                else if (keyframeIdx >= 10)
                {
                    romper->model.anim.status = ANIM_STATUS(RomperAnim_WalkToRunForwardRightEnd, false);
                }
                else
                {
                    romper->model.anim.status = ANIM_STATUS(RomperAnim_WalkToRunForwardLeftEnd, false);
                }

                romper->model.controlState = RomperControl_6;
                break;

            case ANIM_STATUS(RomperAnim_GrabAttack, false):
            case ANIM_STATUS(RomperAnim_GrabAttack, true):
            case ANIM_STATUS(RomperAnim_CreepForward, false):
            case ANIM_STATUS(RomperAnim_CreepForward, true):
                romper->model.controlState = RomperControl_6;
                romper->model.anim.status  = ANIM_STATUS(RomperAnim_WalkToRunForwardRightEnd, false);
                break;
        }
    }
    else if (!(romperProps.flags & RomperFlag_2))
    {
        romper->model.controlState = RomperControl_7;
        romper->model.anim.status  = ANIM_STATUS(RomperAnim_JumpStunStart, false);
        romperProps.flags         |= RomperFlag_2;
    }

    romperProps.field_10E    = 0;
    romperProps.rotationY_F2 = Q12_ANGLE(360.0f);

    Chara_DamageClear(romper);

    if (prevControlState != romper->model.controlState)
    {
        if (romperProps.flags & RomperFlag_11)
        {
            romperProps.flags            &= ~RomperFlag_11;
            g_SysWork.charaGroupFlags[3] &= ~(CharaGroupFlag_0 | CharaGroupFlag_1);
        }
    }
}

void Romper_ControlUpdate(s_SubCharacter* romper)
{
    u8 controlState;

    if (romper->model.controlState != RomperControl_Jump)
    {
        romper->field_44.field_0 = 0;
    }

    // Handle control state.
    controlState = romper->model.controlState;
    g_Romper_ControlFuncs[controlState](romper);
    if (romper->model.controlState != controlState)
    {
        romperProps.distance_120 = Q12(0.0f);
    }
}

void Romper_Control_1(s_SubCharacter* romper)
{
    s32    flags;
    q19_12 newMoveSpeed;
    s32    var;

    if (!WorldEnv_IsInDarkness() && !WorldEnv_IsFlashlightAllowed())
    {
        var = func_8006FD90(romper, 1, Q12(1.8f), Q12(4.0f));
    }
    else if (!WorldEnv_IsInDarkness() && WorldEnv_IsFlashlightAllowed())
    {
        var = func_8006FD90(romper, 1, Q12(3.0f), Q12(7.0f));
    }
    else
    {
        var = func_8006FD90(romper, 1, Q12(1.0f), Q12(1.0f));
    }

    if (g_SavegamePtr->gameDifficulty != GameDifficulty_Easy)
    {
        var += func_80070360(romper, Q12(0.0f), Q12(0.4f));
    }

    newMoveSpeed      = romper->moveSpeed - Q12_MULT_PRECISE(g_DeltaTime, Q12(15.0f));
    romper->moveSpeed = MAX(newMoveSpeed, Q12(0.0f));

    if (romper->model.anim.status == ANIM_STATUS(RomperAnim_GrabAttack, true) &&
        (Rng_GenerateUInt(0, 4095) == 0 || var)) // 1 in 4096 chance.
    {
        romper->model.controlState = RomperControl_WalkForward;
        romper->model.anim.status  = ANIM_STATUS(RomperAnim_WalkForward, false);
    }
}

void Romper_ControlWalkForward(s_SubCharacter* romper)
{
    q3_12  moveDist;
    q3_12  angleDeltaToTarget;
    q19_12 headingAngle;
    s32    flags;
    bool   startRunning;

    angleDeltaToTarget = Math_AngleNormalizeSigned(romperProps.rotationY_F2 - romper->rotation.vy);
    if (TIMESTEP_ANGLE(3, 3) < ABS(angleDeltaToTarget))
    {
        if (angleDeltaToTarget > Q12_ANGLE(0.0f))
        {
            romper->rotation.vy += Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(30.0f));
        }
        else
        {
            romper->rotation.vy -= Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(30.0f));
        }
    }
    else
    {
        romper->rotation.vy = romperProps.rotationY_F2;
    }

    if (romper->rotation.vy == romperProps.rotationY_F2 || Los_CharaHitCheck(romper, Q12(1.0f), romper->rotation.vy))
    {
        angleDeltaToTarget = Math_AngleNormalizeSigned(romper->rotation.vy - ratan2(romperProps.targetPositionX_FC  - romper->position.vx,
                                                                           romperProps.targetPositionZ_100 - romper->position.vz));
        if (ABS(angleDeltaToTarget) > Q12_ANGLE(15.0f))
        {
            if (!Rng_GenerateInt(0, 15)) // 1 in 16 chance.
            {
                romperProps.rotationY_F2 = Los_CharaHeadingAngleGet(romper, Q12(1.0f), romperProps.targetPositionX_FC,
                                                                                  romperProps.targetPositionZ_100, Q12_ANGLE(360.0f), true);
            }
        }
    }

    if (Math_Vector2MagCalcSafeQ6(romper->position.vx - romperProps.targetPositionX_FC,
                            romper->position.vz - romperProps.targetPositionZ_100) < Q12(1.0f))
    {
        moveDist                        = Rng_GenerateInt(Q12(0.0f), Q12(5.0f) - 1);
        headingAngle                    = Rng_GenerateUInt(0, Q12_ANGLE(360.0f) - 1);
        romperProps.targetPositionX_FC  = romperProps.positionZ_110 + Q12_MULT(moveDist, Math_Sin(headingAngle));
        romperProps.targetPositionZ_100 = romperProps.positionX_108 + Q12_MULT(moveDist, Math_Cos(headingAngle));
    }

    romperProps.field_F0 += sharedFunc_800E939C_2_s02(romper);
    romper->moveSpeed += romperProps.field_F0;

    if (!WorldEnv_IsInDarkness() && !WorldEnv_IsFlashlightAllowed())
    {
        startRunning = func_8006FD90(romper, 1, Q12(1.8f), Q12(4.0f));
    }
    else if (!WorldEnv_IsInDarkness() && WorldEnv_IsFlashlightAllowed())
    {
        startRunning = func_8006FD90(romper, 1, Q12(3.0f), Q12(7.0f));
    }
    else
    {
        startRunning = func_8006FD90(romper, 1, Q12(1.0f), Q12(1.0f));
    }

    if (g_SavegamePtr->gameDifficulty != GameDifficulty_Easy)
    {
        startRunning += func_80070360(romper, Q12(0.0f), Q12(0.15f));
    }

    if (startRunning && romper->model.anim.status == ANIM_STATUS(RomperAnim_WalkForward, true))
    {
        romper->model.anim.status = ANIM_STATUS(RomperAnim_RunForwardStart, false);

        if (FP_FROM(romper->model.anim.time, Q12_SHIFT) > 148 &&
            FP_FROM(romper->model.anim.time, Q12_SHIFT) < 157)
        {
            romperProps.field_10C = 7;
        }
        else
        {
            romperProps.field_10C = 18;
        }

        romper->flags |= RomperFlag_2;
    }
}

void Romper_Control_3(s_SubCharacter* romper)
{
    q3_12           angleDeltaToTarget;
    s32             temp_s0;
    bool            cond;
    q19_12          distToTarget;
    s32             temp_v1_2;
    s32             unkDist1;
    s32             var_s0;
    s32             i;
    q3_12           angleToTarget;
    q19_12          unkDist;
    s_SubCharacter* player;

    romperProps.field_F0 += sharedFunc_800E94B4_2_s02(romper);
    romper->moveSpeed    += romperProps.field_F0;

    if (!WorldEnv_IsInDarkness() && !WorldEnv_IsFlashlightAllowed())
    {
        var_s0 = func_8006FD90(romper, 0, Q12(2.5f), Q12(6.0f));
    }
    else if (!WorldEnv_IsInDarkness() && WorldEnv_IsFlashlightAllowed())
    {
        var_s0 = func_8006FD90(romper, 0, Q12(3.0f), Q12(8.0f));
    }
    else
    {
        var_s0 = func_8006FD90(romper, 0, Q12(0.8f), Q12(2.0f));
    }

    if (g_SavegamePtr->gameDifficulty != GameDifficulty_Easy)
    {
        var_s0 |= func_80070360(romper, Q12(0.0f), Q12(0.2f));
    }

    if (var_s0)
    {
        romperProps.targetPositionX_FC = PLAYER_CHARA().position.vx +
                                                    (Q12_MULT(PLAYER_CHARA().moveSpeed, Math_Sin(PLAYER_CHARA().headingAngle)) >> 1);

        romperProps.targetPositionZ_100 = PLAYER_CHARA().position.vz +
                                                     (Q12_MULT(PLAYER_CHARA().moveSpeed, Math_Cos(PLAYER_CHARA().headingAngle)) >> 1);
        romperProps.field_10E = 0;
    }
    else
    {
        romperProps.field_10E += g_DeltaTime;
    }

    if (ANIM_TIME_RANGE_CHECK(romper->model.anim.time, 112, 114) ||
        ANIM_TIME_RANGE_CHECK(romper->model.anim.time, 121, 126))
    {
        romperProps.flags &= ~RomperFlag_3;
        romperProps.flags &= ~RomperFlag_8;
        return;
    }

    if (!(romperProps.flags & RomperFlag_8))
    {
        romperProps.flags |= RomperFlag_8;
        return;
    }

    distToTarget = Math_Vector2MagCalcSafeQ6(romper->position.vx - romperProps.targetPositionX_FC,
                                       romper->position.vz - romperProps.targetPositionZ_100);

    cond = Los_NpcToPlayerHitCheck(romper, &PLAYER_CHARA());

    if (!(romperProps.flags & RomperFlag_3))
    {
        romperProps.flags |= RomperFlag_3;

        angleToTarget = ratan2(romperProps.targetPositionX_FC - romper->position.vx,
                               romperProps.targetPositionZ_100 - romper->position.vz);
        angleDeltaToTarget = ABS(Math_AngleNormalizeSigned(angleToTarget - romper->rotation.vy));
        unkDist   = Q12_MULT_PRECISE(angleDeltaToTarget * 2, Q12(4.0f));

        if (!(romperProps.flags & RomperFlag_4) && unkDist < distToTarget)
        {
            if (angleDeltaToTarget < Q12_ANGLE(60.0f) && distToTarget < Q12(6.0f) && !cond)
            {
                romperProps.rotationY_F2 = angleToTarget;
            }
            else
            {
                unkDist1 = Q12_MULT_PRECISE(Rng_GenerateUInt(0xF00, 0x1100 - 1), CLAMP_MIN_THEN_LOW(distToTarget - Q12(0.2f), Q12(0.2f), Q12(2.5f)));

                romperProps.rotationY_F2 = Los_CharaHeadingAngleGet(romper, unkDist1, romperProps.targetPositionX_FC, romperProps.targetPositionZ_100, Q12_ANGLE(360.0f), true);
            }
        }
        else
        {
            unkDist1 = Q12_MULT_PRECISE(Rng_GenerateUInt(0xF00, 0x1100 - 1), Q12(2.5f));
            romperProps.rotationY_F2 = Los_CharaHeadingAngleGet(romper, unkDist1, romperProps.targetPositionX_FC, romperProps.targetPositionZ_100, Q12_ANGLE(360.0f), false);
        }

        if (romperProps.rotationY_F2 == angleToTarget)
        {
            temp_s0 = Los_CharaHitCheck(romper, CLAMP_HIGH(distToTarget, 0x1AAA), romper->rotation.vy);
        }
        else
        {
            temp_s0 = Los_CharaHitCheck(romper, 0x1AAA, romper->rotation.vy);
        }

        if (romperProps.field_10E > Q12(1.0f) && !Rng_GenerateUInt(0, 15))
        {
            romper->model.controlState = RomperControl_1;
            romper->model.anim.status  = ANIM_STATUS(RomperAnim_GrabAttack, false);
            romperProps.rotationY_F2   = romper->rotation.vy;
        }
        else if (temp_s0 != 0 || romperProps.rotationY_F2 == Q12_ANGLE(360.0f) ||
                 distToTarget < Q12_MULT_PRECISE(Q12(1.0f) - Math_Cos(angleDeltaToTarget >> 1), Q12(2.5f)))
        {
            if (romperProps.rotationY_F2 == Q12_ANGLE(360.0f))
            {
                romperProps.rotationY_F2 = romper->rotation.vy + Q12_ANGLE(180.0f);
            }

            if (!ANIM_TIME_RANGE_CHECK(romper->model.anim.time, 115, 120))
            {
                romper->model.controlState = RomperControl_4;
                romper->model.anim.status  = ANIM_STATUS(RomperAnim_CreepForward, false);
                romperProps.field_10C      = 0;
                romper->moveSpeed          = Q12(0.0f);
                romperProps.distance_120   = Q12(0.0f);
            }
        }
    }

    for (i = 0; i < 3; i++)
    {
        angleToTarget = Math_AngleNormalizeSigned(romperProps.rotationY_F2 - romper->rotation.vy);

        if (TIMESTEP_ANGLE(1, 3) < ABS(angleToTarget))
        {
            if (angleToTarget > Q12_ANGLE(0.0f))
            {
                romper->rotation.vy += Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(90.0f));
            }
            else
            {
                romper->rotation.vy -= Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(90.0f));
            }
        }
        else
        {
            romper->rotation.vy = romperProps.rotationY_F2;
            break;
        }
    }

    if (romperProps.flags & RomperFlag_4)
    {
        romperProps.distance_120 += g_DeltaTime;
        if (distToTarget > Q12(1.8f) || romperProps.distance_120 > Q12(1.8f))
        {
            romperProps.distance_120 = 0;
            romperProps.flags &= ~RomperFlag_4;
        }
    }

    // @hack Required for match.
    for (i = 0; i < ARRAY_SIZE(g_SysWork.npcs); i++)
    {
        if (&NPC_CHARA(i) == romper)
        {
            break;
        }
    }

    if (PLAYER_CHARA().health <= Q12(0.0f))
    {
        return;
    }

    player = &PLAYER_CHARA();

    if (player->flags & RomperFlag_3)
    {
        return;
    }

    if (g_SysWork.sysState != SysState_Gameplay ||
        (g_SysWork.charaGroupFlags[3] & (CharaGroupFlag_0 | CharaGroupFlag_1)))
    {
        return;
    }

    if (romper->fallSpeed != Q12(0.0f) || distToTarget <= Q12(0.3f) || distToTarget >= Q12(3.2f) ||
        (romper->position.vy - PLAYER_CHARA().position.vy) >= Q12(1.0f) ||
        (romper->position.vy - PLAYER_CHARA().position.vy) <= Q12(-1.2f))
    {
        return;
    }

    if (romperProps.flags & RomperFlag_4)
    {
        return;
    }

    if (ABS(Math_AngleNormalizeSigned(ratan2(PLAYER_CHARA().position.vx - romper->position.vx,
                                             PLAYER_CHARA().position.vz - romper->position.vz) -
                                      romper->rotation.vy)) < Q12_ANGLE(30.0f))
    {
        romper->model.controlState    = RomperControl_Jump;
        romper->model.anim.status     = ANIM_STATUS(RomperAnim_RunToJump, false);
        romper->field_44.field_0      = 1;
        g_SysWork.charaGroupFlags[3] |= CharaGroupFlag_0 | CharaGroupFlag_1;
        romperProps.flags            |= RomperFlag_4 | RomperFlag_11;
    }
}

void Romper_Control_4(s_SubCharacter* romper)
{
    q19_12 angle0;
    s32    temp_s0;
    s32    flags;
    s32    var_s0;
    s32    i;
    q19_12 angle1;

    if (!WorldEnv_IsInDarkness() && !WorldEnv_IsFlashlightAllowed())
    {
        var_s0 = func_8006FD90(romper, 0, Q12(2.5f), Q12(6.0f));
    }
    else if (!WorldEnv_IsInDarkness() && WorldEnv_IsFlashlightAllowed())
    {
        var_s0 = func_8006FD90(romper, 0, Q12(3.0f), Q12(8.0f));
    }
    else
    {
        var_s0 = func_8006FD90(romper, 0, Q12(0.8f), Q12(2.0f));
    }

    if (g_SavegamePtr->gameDifficulty == GameDifficulty_Easy)
    {
        var_s0 |= func_80070360(romper, Q12(0.0f), Q12(0.2f));
    }

    if (var_s0 != 0)
    {
        romperProps.targetPositionX_FC  = PLAYER_CHARA().position.vx;
        romperProps.targetPositionZ_100 = PLAYER_CHARA().position.vz;
        romperProps.field_10E     = 0;
    }
    else
    {
        romperProps.field_10E += g_DeltaTime;
    }

    temp_s0 = Los_CharaHitCheck(romper, Q12(2.5f), romper->rotation.vy);
    if (romperProps.distance_120 == Q12(0.0f) ||
        (temp_s0 != 0 && romperProps.distance_120 >= Q12(0.0f)))
    {
        romperProps.rotationY_F2 = Q12_ANGLE(0.0f);

        if (Math_Vector2MagCalcSafeQ6(PLAYER_CHARA().position.vx - romper->position.vx,
                                PLAYER_CHARA().position.vz - romper->position.vz) >= Q12_ANGLE(180.0f))
        {
            romperProps.rotationY_F2 = Los_CharaHeadingAngleGet(romper, Q12(2.5f), romperProps.targetPositionX_FC,
                                                                              romperProps.targetPositionZ_100, Q12(1.0f), true);
        }
        else
        {
            romperProps.rotationY_F2 = Los_CharaHeadingAngleGet(romper, Q12(2.5f), romperProps.targetPositionX_FC,
                                                                              romperProps.targetPositionZ_100, Q12(1.0f), false);
        }

        if (romperProps.rotationY_F2 == Q12_ANGLE(360.0f))
        {
            romperProps.rotationY_F2 = func_8006F99C(romper, Q12(2.5f), romper->rotation.vy);
        }

        if (temp_s0 != 0)
        {
            romperProps.distance_120 = -((Rng_Rand16() % Q12(1.0f)) + Q12(1.0f));
        }
        else
        {
            romperProps.distance_120 = (Rng_Rand16() % Q12(2.0f)) + Q12(1.0f);
        }
    }

    if (romperProps.distance_120 > Q12(0.0f))
    {
        romperProps.distance_120 -= g_DeltaTime;
        if (romperProps.distance_120 < Q12(0.0f))
        {
            romperProps.distance_120 = Q12(0.0f);
        }
    }
    else
    {
        romperProps.distance_120 += g_DeltaTime;
        if (romperProps.distance_120 > Q12(0.0f))
        {
            romperProps.distance_120 = Q12(0.0f);
        }
    }

    for (i = 0; i < 3; i++)
    {
        angle0 = Math_AngleNormalizeSigned(romperProps.rotationY_F2 - romper->rotation.vy);
        if (TIMESTEP_ANGLE(1, 4) < ABS(angle0))
        {
            if (angle0 > Q12_ANGLE(0.0f))
            {
                romper->rotation.vy += Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(45.0f));
            }
            else
            {
                romper->rotation.vy -= Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(45.0f));
            }
        }
        else
        {
            romper->rotation.vy = romperProps.rotationY_F2;
        }
    }

    // TODO: Animation 20 doesn't exist?
    if (!Los_CharaHitCheck(romper, Q12(2.5f), romper->rotation.vy) && ANIM_TIME_RANGE_CHECK(romper->model.anim.time, 131, 132))
    {
        romper->model.anim.status = ANIM_STATUS(RomperAnim_20, false);
        romper->moveSpeed         = Q12(0.0f);
        return;
    }

    angle0 = Math_AngleNormalizeSigned(romperProps.rotationY_F2 - romper->rotation.vy);
    if (angle0 >= Q12_ANGLE(0.0f))
    {
        angle1 = Q12_ANGLE(180.0f) - angle0;
    }
    else
    {
        angle1 = angle0 + Q12_ANGLE(180.0f);
    }

    Chara_MoveSpeedUpdate4(romper, Q12(1.2f), Q12_MULT_PRECISE(angle1 * 2, Q12(2.4f)));
}

void Romper_ControlJump(s_SubCharacter* romper)
{
    s_CollisionSurface surface;
    VECTOR3            pos; // Q19.12
    s16                temp_v0_4;
    q3_12              unkAngle1;
    s16                var_s3;
    s32                temp_a1;
    q19_12             unkAngle;
    s32                temp_v0;
    s32                temp_v0_3;
    s32                temp_v1_5;
    s16                var_a0_2;
    s32                i;
    s16                temp;

    if (romper->model.anim.status == ANIM_STATUS(RomperAnim_RunToJump, true))
    {
        if (romper->model.anim.time > Q12(5.0f))
        {
            var_s3 = Q12(0.0f);
        }
        else
        {
            var_s3 = Q12(0.5f) - (Q12_DIV(romper->model.anim.time - Q12(2.0f), Q12(3.0f)) >> 1);
        }
    }
    else
    {
        var_s3 = Q12(0.5f);
    }

    temp_v0                                   = Q12_MULT(PLAYER_CHARA().moveSpeed, Math_Sin(PLAYER_CHARA().headingAngle));
    romperProps.targetPositionX_FC = PLAYER_CHARA().position.vx + Q12_MULT_PRECISE(temp_v0, var_s3);

    temp_a1                                    = Q12_MULT(PLAYER_CHARA().moveSpeed, Math_Cos(PLAYER_CHARA().headingAngle));
    romperProps.targetPositionZ_100 = PLAYER_CHARA().position.vz + Q12_MULT_PRECISE(temp_a1, var_s3);

    temp_v0_3 = ratan2(romperProps.targetPositionX_FC - romper->position.vx, romperProps.targetPositionZ_100 - romper->position.vz);
    unkAngle   = temp_v0_3;

    if (romper->model.anim.status == ANIM_STATUS(RomperAnim_RunToJump, false))
    {
        romper->moveSpeed = MAX(romper->moveSpeed - Q12_MULT_PRECISE(g_DeltaTime, Q12(15.0f)), Q12(0.0f));

        for (i = 0; i < 4; i++)
        {
            temp_v0_4 = Math_AngleNormalizeSigned(unkAngle - romper->rotation.vy);

            if ((((g_DeltaTime >> 3) + 1) >> 1) < ABS(temp_v0_4))
            {
                if (temp_v0_4 > 0)
                {
                    romper->rotation.vy += Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(90.0f)) >> 1;
                }
                else
                {
                    romper->rotation.vy -= Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(90.0f)) >> 1;
                }
            }
            else
            {
                romper->rotation.vy = unkAngle;
            }
        }
    }
    else if (romper->model.anim.status == ANIM_STATUS(RomperAnim_RunToJump, true))
    {
        unkAngle1 = Math_AngleNormalizeSigned(temp_v0_3 - romper->rotation.vy);
        if (TIMESTEP_ANGLE(1, 4) < ABS(unkAngle1))
        {
            if (unkAngle1 > Q12_ANGLE(0.0f))
            {
                romper->rotation.vy += Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(45.0f));
            }
            else
            {
                romper->rotation.vy -= Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(45.0f));
            }
        }

        if (romper->model.anim.time == Q12(2.0f))
        {
            temp_v1_5 = Math_Vector2MagCalcSafeQ6(romperProps.targetPositionX_FC - romper->position.vx,
                                            romperProps.targetPositionZ_100 - romper->position.vz);

            var_a0_2          = CLAMP_LOW(temp_v1_5, Q12(0.3f));
            romper->moveSpeed = var_a0_2 << 1;

            Collision_SurfaceGet(&surface, PLAYER_CHARA().position.vx, PLAYER_CHARA().position.vz);
            temp = surface.floorHeight - Q12(0.8f);

            romper->fallSpeed  = (temp << 1) - Q12(2.45f);
            romperProps.flags &= ~RomperFlag_9;
        }
        else if (FP_FROM(romper->model.anim.time, Q12_SHIFT) == 5 ||
                 FP_FROM(romper->model.anim.time, Q12_SHIFT) == 6)
        {
            if (!(romperProps.flags & RomperFlag_9))
            {
                Sfx_WithFlagsPlay(Sfx_Unk1403, &romper->position, Q8(0.5f), SfxFlag_None);
                romperProps.flags |= RomperFlag_9;
            }

            pos.vx = romper->position.vx;
            pos.vy = romper->position.vy;
            pos.vz = romper->position.vz;

            if (func_8008A0E4(1, WEAPON_ATTACK(EquippedWeaponId_Shotgun, AttackInputType_Multitap), romper, &pos, &PLAYER_CHARA(), romper->rotation.vy, Q12_ANGLE(90.0f)) != NO_VALUE)
            {
                romper->model.anim.status = ANIM_STATUS(RomperAnim_JumpToIdleStart, false);
                romper->model.controlState  = RomperControl_10;

                sharedFunc_800E9714_2_s02(romper);
                romperProps.flags |= RomperFlag_10;
            }
        }
    }
    else
    {
        if (romperProps.flags & RomperFlag_Falling)
        {
            Chara_MoveSpeedUpdate(romper, Q12_MULT_PRECISE(romper->moveSpeed, Q12(3.0f)) + Q12(3.0f));
        }
        else
        {
            romper->moveSpeed = MAX(romper->moveSpeed - Q12_MULT_PRECISE(g_DeltaTime, Q12(15.0f)), Q12(0.0f));
        }
    }
}

void Romper_Control_6(s_SubCharacter* romper)
{
    q3_12 moveDist;

    romper->moveSpeed = MAX(romper->moveSpeed - Q12_MULT_PRECISE(g_DeltaTime, Q12(15.0f)), Q12(0.0f));

    if (ANIM_STATUS_IDX_GET(romper->model.anim.status) == RomperAnim_WalkToRunForwardRightEnd ||
        ANIM_STATUS_IDX_GET(romper->model.anim.status) == RomperAnim_WalkToRunForwardLeftEnd)
    {
        moveDist                    = (Q12_MULT_PRECISE(sharedData_800ECA4C_2_s02, g_DeltaTime) * Q12(0.6f)) / Q12(10.0f);
        romperProps.movementOffsetX = Q12_MULT(moveDist, Math_Sin(romper->rotation.vy));
        romperProps.movementOffsetZ = Q12_MULT(moveDist, Math_Cos(romper->rotation.vy));
    }
}

void Romper_Control_7(s_SubCharacter* romper)
{
    q3_12 moveDist;

    if (romperProps.flags & RomperFlag_Falling)
    {
        Chara_MoveSpeedUpdate(romper, Q12_MULT_PRECISE(romper->moveSpeed, Q12(3.0f)) + Q12(3.0f));
    }
    else
    {
        romper->moveSpeed = MAX(romper->moveSpeed - Q12_MULT_PRECISE(g_DeltaTime, Q12(15.0f)), Q12(0.0f));
    }

    if (romper->model.anim.status == ANIM_STATUS(RomperAnim_JumpStunStart, false) ||
        ANIM_TIME_RANGE_CHECK(romper->model.anim.time, 93, 96))
    {
        moveDist                    = (Q12_MULT_PRECISE(sharedData_800ECACC_2_s02, g_DeltaTime) * Q12(1.4f)) / Q12(7.0f);
        romperProps.movementOffsetX = Q12_MULT(moveDist, Math_Sin(romper->rotation.vy));
        romperProps.movementOffsetZ = Q12_MULT(moveDist, Math_Cos(romper->rotation.vy));
    }

    if (ANIM_STATUS_IDX_GET(romper->model.anim.status) == RomperAnim_JumpStunEnd)
    {
        romper->flags |= RomperFlag_1;
    }

    if (romper->health == Q12(0.0f) && romper->model.anim.status == ANIM_STATUS(RomperAnim_JumpStunEnd, true))
    {
        romper->model.controlState = RomperControl_8;
        romper->model.anim.status  = ANIM_STATUS(RomperAnim_StunToIdleStart, false);
        romper->flags             &= ~RomperFlag_1;

        Savegame_EnemyStateUpdate(romper);
    }
}

void Romper_Control_8(s_SubCharacter* romper)
{
    if (romper->health == Q12(0.0f))
    {
        if (g_SysWork.targetNpcIdx != Chara_NpcIdxGet(romper))
        {
            romper->health          = NO_VALUE;
            romper->collision.state = CharaCollisionState_Ignore;
        }
    }

    if (romper->moveSpeed == Q12(0.0f) &&
        !(romperProps.flags & (RomperFlag_Falling | RomperFlag_1)))
    {
        func_800622B8(3, romper, ANIM_STATUS(RomperAnim_StunToIdleStart, true), 9);
        romperProps.flags |= RomperFlag_1;
    }
}

void Romper_Control_9(s_SubCharacter* romper) {}

void Romper_Control_10(s_SubCharacter* romper)
{
    VECTOR          sp10;
    s_SubCharacter* player;

    if (romper->position.vx != romperProps.targetPositionX_FC ||
        romper->position.vz != romperProps.targetPositionZ_100)
    {
        romperProps.rotationY_F2 = ratan2(romperProps.targetPositionX_FC - romper->position.vx,
                                          romperProps.targetPositionZ_100 - romper->position.vz);
    }

    if (romper->model.anim.status == ANIM_STATUS(RomperAnim_JumpToIdleEnd, true))
    {
        romper->moveSpeed = MAX(romper->moveSpeed - Q12_MULT_PRECISE(g_DeltaTime, romperProps.field_124 * 4), Q12(0.0f));
    }
    else
    {
        romper->moveSpeed = MAX(romper->moveSpeed - Q12_MULT_PRECISE(g_DeltaTime, romperProps.field_124), Q12(0.0f));
    }

    romper->collision.state = CharaCollisionState_Ignore;
    if (romper->model.anim.status == ANIM_STATUS(RomperAnim_JumpToIdleEnd, true))
    {
        romperProps.field_104 = romper->model.anim.time - Q12(20.0f);
    }
    else
    {
        romperProps.field_104 = 0;
    }

    player = &PLAYER_CHARA();

    if (player->attackReceived == NO_VALUE)
    {
        romper->model.anim.status  = ANIM_STATUS(RomperAnim_9, false);
        romper->model.controlState = RomperControl_GrabAttack;
        romper->collision.state    = CharaCollisionState_Ignore;
        romper->collision.state    = CharaCollisionState_Npc;
    }
    else if (romper->model.anim.time < Q12(24.0f))
    {
        romperProps.flags |= RomperFlag_6;
    }
    else if (romperProps.flags & RomperFlag_6)
    {
        romperProps.flags &= ~RomperFlag_6;

        PLAYER_CHARA().damage.amount += (FP_TO(D_800AD4C8[55].damage, Q12_SHIFT) *
                                                               Rng_GenerateUInt(85, 116)) / 100;

        sp10.vx = romper->position.vx + FP_FROM(FP_TO(Math_Sin(romper->rotation.vy) >> 1, Q12_SHIFT), Q12_SHIFT);
        sp10.vy = romper->position.vy - Q12(0.1f);
        sp10.vz = romper->position.vz + FP_FROM(FP_TO(Math_Cos(romper->rotation.vy) >> 1, Q12_SHIFT), Q12_SHIFT);

        func_8005F6B0(&PLAYER_CHARA(), &sp10, 4, 9);

        romperProps.field_11A = 1;
    }
}

void Romper_ControlGrabAttack(s_SubCharacter* romper)
{
    if (romper->model.anim.status == ANIM_STATUS(RomperAnim_GrabAttack, true) &&
        !Rng_GenerateInt(0, 7)) // 1 in 8 chance.
    {
        g_SysWork.charaGroupFlags[3] &= ~(CharaGroupFlag_0 | CharaGroupFlag_1);
        romper->model.controlState    = RomperControl_1;
        romperProps.flags            &= ~RomperFlag_11;
    }
}

void sharedFunc_800E8730_2_s02(s_SubCharacter* romper)
{
    s_CollisionResult collResult;
    VECTOR3           pos; // Q19.12
    q3_12             headingAngle;
    s32               temp_s0;
    s32               temp_s2;
    s32               temp_s3;
    q19_12            sinHeadingAngle;

    romper->fallSpeed += g_GravitySpeed;

    if (ANIM_STATUS_IDX_GET(romper->model.anim.status) == RomperAnim_JumpToIdleStart ||
        ANIM_STATUS_IDX_GET(romper->model.anim.status) == RomperAnim_JumpToIdleEnd)
    {
        romper->headingAngle = romperProps.rotationY_F2;
    }
    else
    {
        romper->headingAngle = romper->rotation.vy;
    }

    if (g_DeltaTime != Q12(0.0f))
    {
        headingAngle = romper->headingAngle;
        temp_s0 = Q12_MULT_PRECISE(g_DeltaTime, romper->moveSpeed);
        temp_s2 = (temp_s0 < -0x7FFF || temp_s0 > 0x7FFF) * 4;
        sinHeadingAngle = Math_Sin(headingAngle);
        temp_s3 = temp_s2 >> 1;
        temp_s0 = temp_s0 >> temp_s3;

        pos.vx = Q12_MULT_PRECISE(temp_s0, sinHeadingAngle >> temp_s3) << temp_s2;
        pos.vz = Q12_MULT_PRECISE(temp_s0, Math_Cos(headingAngle) >> temp_s3) << temp_s2;
        pos.vy = Q12_MULT_PRECISE(g_DeltaTime, romper->fallSpeed);

        pos.vx += romperProps.movementOffsetX;
        pos.vz += romperProps.movementOffsetZ;

        Collision_WallDetect(&collResult, &pos, romper);

        romper->position.vx += collResult.offset.vx;
        romper->position.vz += collResult.offset.vz;

        if (romperProps.flags & RomperFlag_10)
        {
            if (romper->model.controlState != RomperControl_10 && romper->position.vy <= collResult.surface.floorHeight)
            {
                romperProps.flags &= ~RomperFlag_10;
            }

            if (romperProps.flags & RomperFlag_10)
            {
                if (romper->model.controlState == RomperControl_10)
                {
                    romper->position.vy += collResult.offset.vy;
                    if (PLAYER_CHARA().position.vy < romper->position.vy)
                    {
                        romper->position.vy = PLAYER_CHARA().position.vy;
                        romper->fallSpeed   = Q12(0.0f);
                    }
                }
                else
                {
                    romper->position.vy -= Q12_MULT_PRECISE(g_DeltaTime, Q12(0.15f));
                    romper->fallSpeed    = Q12(0.0f);
                    if (collResult.surface.floorHeight >= romper->position.vy)
                    {
                        romper->position.vy = collResult.surface.floorHeight;
                    }
                }
            }
            else
            {
                romper->position.vy += collResult.offset.vy;
                if (collResult.surface.floorHeight < romper->position.vy)
                {
                    romper->position.vy = collResult.surface.floorHeight;
                    romper->fallSpeed   = Q12(0.0f);
                }
            }
        }
        else
        {
            romper->position.vy += collResult.offset.vy;
            if (collResult.surface.floorHeight < romper->position.vy)
            {
                romper->position.vy = collResult.surface.floorHeight;
                romper->fallSpeed   = Q12(0.0f);
            }
        }

        romperProps.movementOffsetZ = Q12(0.0f);
        romperProps.movementOffsetX = Q12(0.0f);
    }

    romper->rotation.vy = Math_AngleNormalizeSigned(romper->rotation.vy);

    if (romper->fallSpeed != Q12(0.0f))
    {
        romperProps.flags |= RomperFlag_Falling;
    }
    else
    {
        romperProps.flags &= ~RomperFlag_Falling;
    }
}

void sharedFunc_800E8A40_2_s02(s_SubCharacter* romper, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords)
{
    typedef struct
    {
        MATRIX  field_0;
        SVECTOR field_20;
    } s_sharedFunc_800E8A40_2_s02;

    q19_12                       angleDeltaToTarget;
    q19_12                       unkAngle;
    s32                          temp_v0_4;
    s_AnimInfo*                  animInfo;
    s_sharedFunc_800E8A40_2_s02* ptr;

    ptr = PSX_SCRATCH;

    switch (romper->model.anim.status)
    {
        case ANIM_STATUS(RomperAnim_RunForwardLoop, true):
        case ANIM_STATUS(RomperAnim_WalkToRunForwardRightStart, true):
        case ANIM_STATUS(RomperAnim_WalkToRunForwardLeftStart, true):
            ROMPER_ANIM_INFOS[romper->model.anim.status].duration.constant = sharedData_800EC950_2_s02;
            break;

        case ANIM_STATUS(RomperAnim_CreepForward, false):
            sharedData_800ECB22_2_s02 = romperProps.field_10C + 131;
            break;

        case ANIM_STATUS(RomperAnim_CreepForward, true):
            ROMPER_ANIM_INFOS[29].startKeyframeIdx = romperProps.field_10C + 131;
            romperProps.field_10C                  = 0;
            ROMPER_ANIM_INFOS[romper->model.anim.status].duration.constant = Q12_MULT_PRECISE(MAX(romper->moveSpeed, Q12(1.5f)), Q12(8.32f));
            break;

        case ANIM_STATUS(RomperAnim_RunForwardStart, false):
            sharedData_800ECBC2_2_s02 = romperProps.field_10C + 109;
            break;

        case ANIM_STATUS(RomperAnim_RunForwardStart, true):
            sharedData_800ECBD0_2_s02 = romperProps.field_10C + 109;
            break;
    }

    Math_MatrixTransform(&romper->position, (SVECTOR*)&romper->rotation, boneCoords);

    animInfo = &ROMPER_ANIM_INFOS[romper->model.anim.status];
    animInfo->playbackFunc(&romper->model, anmHdr, boneCoords, animInfo);

    switch (romper->model.anim.status)
    {
        case ANIM_STATUS(RomperAnim_WalkToRunForwardRightEnd, false):
            romper->model.controlState = RomperControl_6;
            if (romper->model.anim.time == Q12(127.0f))
            {
                romper->model.anim.status = ANIM_STATUS(RomperAnim_WalkToRunForwardRightEnd, true);
                romper->model.anim.time   = Q12(39.0f);
            }
            break;

        case ANIM_STATUS(RomperAnim_WalkToRunForwardLeftEnd, false):
            romper->model.controlState = RomperControl_6;
            if (romper->model.anim.time == Q12(116.0f))
            {
                romper->model.anim.status = ANIM_STATUS(RomperAnim_WalkToRunForwardLeftEnd, true);
                romper->model.anim.time   = Q12(50.0f);
            }
            break;

        case ANIM_STATUS(RomperAnim_RunForwardLoop, false):
            if (romper->model.controlState == RomperControl_Jump)
            {
                romperProps.flags            &= ~RomperFlag_11;
                g_SysWork.charaGroupFlags[3] &= ~(CharaGroupFlag_0 | CharaGroupFlag_1);
            }

            romper->model.controlState        = RomperControl_3;
            romperProps.field_F4 = Q12(22.0f) - 1;

            if (romper->model.anim.time == Q12(49.0f))
            {
                romper->model.anim.status       = ANIM_STATUS(RomperAnim_RunForwardLoop, true);
                romperProps.field_F4 = Q12_CLAMPED(1.0f);
                romper->model.anim.time         = Q12(110.0f);
                romperProps.field_F0 = 1143;
                romper->moveSpeed                  = Q12(0.0f);
            }
            else if (romper->model.anim.time == Q12(60.0f))
            {
                romper->model.anim.status       = ANIM_STATUS(RomperAnim_RunForwardLoop, true);
                romperProps.field_F4 = Q12(11.0f) - 1;
                romperProps.field_F0 = 121;
                romper->model.anim.time = Q12(120.0f);
                romper->moveSpeed       = Q12(1.2f);
            }
            break;

        case ANIM_STATUS(RomperAnim_RunForwardStart, true):
        case ANIM_STATUS(RomperAnim_20, true):
            romper->model.controlState = RomperControl_3;
            romper->model.anim.status  = ANIM_STATUS(RomperAnim_RunForwardLoop, true);
            romperProps.field_F4 = romper->model.anim.time - (Q12(109.0f) + 1);
            break;
    }

    angleDeltaToTarget = Math_AngleNormalizeSigned(romperProps.angle_EC - romper->rotation.vy);
    if (angleDeltaToTarget == Q12_ANGLE(0.0f))
    {
        unkAngle = Q12_ANGLE(0.0f);
    }
    else
    {
        unkAngle = Q12_ANGLE(17.25f);
    }

    if (angleDeltaToTarget < Q12_ANGLE(0.0f))
    {
        unkAngle = -unkAngle;
    }

    temp_v0_4 = Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 1.0f / 16.0f);
    if (unkAngle < romperProps.field_EE)
    {
        romperProps.field_EE = MAX(unkAngle, romperProps.field_EE - temp_v0_4);
    }
    else
    {
        romperProps.field_EE = CLAMP_HIGH(unkAngle, romperProps.field_EE + temp_v0_4);
    }

    *(s32*)&ptr->field_20 = romperProps.field_EE << 16;
    ptr->field_20.vz      = 0;

    Math_RotMatrixZxyNegGte(&ptr->field_20, &ptr->field_0);
    MulMatrix2(&ptr->field_0, &boneCoords[2].coord);

    romperProps.angle_EC = romper->rotation.vy;
}

extern s_CharaKeyframe sharedData_800ECC44_2_s02;
extern s_CharaKeyframe sharedData_800ECC58_2_s02[];
extern s_CharaKeyframe sharedData_800ECCBC_2_s02;
extern s_CharaKeyframe sharedData_800ECCD0_2_s02[];
extern s_CharaKeyframe sharedData_800ECD34_2_s02;
extern s_CharaKeyframe sharedData_800ECD48_2_s02[];
extern s_CharaKeyframe sharedData_800ECE24_2_s02[];
extern s_CharaKeyframe sharedData_800ECF00_2_s02[];
extern s_CharaKeyframe sharedData_800ECF64_2_s02[];
extern s_CharaKeyframe sharedData_800ED018_2_s02[];
extern s_CharaKeyframe sharedData_800ED1D0_2_s02[];
extern s_CharaKeyframe sharedData_800ED2C0_2_s02;

void Romper_CollisionUpdate(s_SubCharacter* romper)
{
    s32 keyframeIdxs[2];
    s32 keyframeIdx;
    s32 i;

    switch (romper->model.anim.status)
    {
        case ANIM_STATUS(RomperAnim_RunToJump, true):
            for (i = 0, keyframeIdx = FP_FROM(romper->model.anim.time, Q12_SHIFT) - 2; i < 2; i++, keyframeIdx++)
            {
                keyframeIdxs[i] = keyframeIdx - !(keyframeIdx < 5);
            }

            Collision_CharaCollisionSet(romper, &sharedData_800ECC58_2_s02[keyframeIdxs[0]], &sharedData_800ECC58_2_s02[keyframeIdxs[1]]);
            break;

        case ANIM_STATUS(RomperAnim_JumpToIdleStart, false):
        case ANIM_STATUS(RomperAnim_JumpToIdleStart, true):
        case ANIM_STATUS(RomperAnim_JumpToIdleEnd, false):
        case ANIM_STATUS(RomperAnim_JumpToIdleEnd, true):
            Chara_CollisionSet(romper, sharedData_800ECCBC_2_s02);
            break;

        case ANIM_STATUS(RomperAnim_JumpToRunForward, true):
            for (i = 0, keyframeIdx = FP_FROM(romper->model.anim.time, Q12_SHIFT) - 15;
                 i < 2;
                 i++, keyframeIdx++)
            {
                keyframeIdxs[i] = keyframeIdx;
            }

            Collision_CharaCollisionSet(romper, &sharedData_800ECCD0_2_s02[keyframeIdxs[0]], &sharedData_800ECCD0_2_s02[keyframeIdxs[1]]);
            break;

        case ANIM_STATUS(RomperAnim_WalkToRunForwardRightEnd, true):
            for (i = 0, keyframeIdx = FP_FROM(romper->model.anim.time, Q12_SHIFT) - 39;
                 i < 2;
                 i++, keyframeIdx++)
            {
                keyframeIdxs[i] = keyframeIdx;
            }

            Collision_CharaCollisionSet(romper, &sharedData_800ECD48_2_s02[keyframeIdxs[0]], &sharedData_800ECD48_2_s02[keyframeIdxs[1]]);
            break;

        case ANIM_STATUS(RomperAnim_WalkToRunForwardLeftEnd, true):
            for (i = 0, keyframeIdx = FP_FROM(romper->model.anim.time, Q12_SHIFT) - 50;
                 i < 2;
                 i++, keyframeIdx++)
            {
                keyframeIdxs[i] = keyframeIdx;
            }

            Collision_CharaCollisionSet(romper, &sharedData_800ECE24_2_s02[keyframeIdxs[0]], &sharedData_800ECE24_2_s02[keyframeIdxs[1]]);
            break;

        case ANIM_STATUS(RomperAnim_StunToIdleStart, false):
        case ANIM_STATUS(RomperAnim_StunToIdleStart, true):
        case ANIM_STATUS(RomperAnim_JumpStunEnd, false):
        case ANIM_STATUS(RomperAnim_JumpStunEnd, true):
        case ANIM_STATUS(RomperAnim_StunToIdleEnd, false):
        case ANIM_STATUS(RomperAnim_StunToIdleEnd, true):
            Chara_CollisionSet(romper, sharedData_800ECF00_2_s02[4]);
            break;

        case ANIM_STATUS(RomperAnim_JumpStunStart, false):
            Chara_CollisionSet(romper, sharedData_800ECF00_2_s02[0]);
            break;

        case ANIM_STATUS(RomperAnim_JumpStunStart, true):
            for (i = 0, keyframeIdx = FP_FROM(romper->model.anim.time, Q12_SHIFT) - 93;
                 i < 2;
                 i++, keyframeIdx++)
            {
                keyframeIdxs[i] = keyframeIdx >= 4 ? 4 : keyframeIdx;
            }

            Collision_CharaCollisionSet(romper, &sharedData_800ECF00_2_s02[keyframeIdxs[0]], &sharedData_800ECF00_2_s02[keyframeIdxs[1]]);
            break;

        case ANIM_STATUS(RomperAnim_GrabAttack, true):
            for (i = 0, keyframeIdx = FP_FROM(romper->model.anim.time, Q12_SHIFT) - 100;
                 i < 2;
                 i++, keyframeIdx = (keyframeIdx + 1) % 9)
            {
                keyframeIdxs[i] = keyframeIdx;
            }

            Collision_CharaCollisionSet(romper, &sharedData_800ECF64_2_s02[keyframeIdxs[0]], &sharedData_800ECF64_2_s02[keyframeIdxs[1]]);
            break;

        case ANIM_STATUS(RomperAnim_RunForwardLoop, true):
            for (i = 0, keyframeIdx = FP_FROM(romper->model.anim.time, Q12_SHIFT) - 109;
                 i < 2;
                 i++, keyframeIdx = (keyframeIdx + 1) % 22)
            {
                keyframeIdxs[i] = keyframeIdx;
            }

            Collision_CharaCollisionSet(romper, &sharedData_800ED018_2_s02[keyframeIdxs[0]], &sharedData_800ED018_2_s02[keyframeIdxs[1]]);
            break;

        case ANIM_STATUS(RomperAnim_CreepForward, true):
            Chara_CollisionSet(romper, sharedData_800ED2C0_2_s02);
            break;

        case ANIM_STATUS(RomperAnim_WalkForward, true):
            for (i = 0, keyframeIdx = FP_FROM(romper->model.anim.time, Q12_SHIFT) - 147;
                 i < 2;
                 i++, keyframeIdx = (keyframeIdx + 1) % 16)
            {
                keyframeIdxs[i] = keyframeIdx - (keyframeIdx > 0) - !(keyframeIdx < 4) - !(keyframeIdx < 8) - !(keyframeIdx < 12);
            }

            Collision_CharaCollisionSet(romper, &sharedData_800ED1D0_2_s02[keyframeIdxs[0]], &sharedData_800ED1D0_2_s02[keyframeIdxs[1]]);
            break;

        case ANIM_STATUS(RomperAnim_9, true):
            Chara_CollisionSet(romper, sharedData_800ECD34_2_s02);
            romper->collision.cylinder.outerRadius = (romper->model.anim.time - Q12(61.0f)) / 100;
            break;

        default:
            Chara_CollisionSet(romper, sharedData_800ECC44_2_s02);
            break;
    }

    Chara_CollisionShapeOffsetsUpdate(&romper->collision.shapeOffsets, romper);

    if (ANIM_STATUS_IDX_GET(romper->model.anim.status) != RomperAnim_RunToJump)
    {
        romper->field_44.field_0 = 0;
    }
}

s32 sharedFunc_800E939C_2_s02(s_SubCharacter* romper)
{
    s32    i;
    q19_12 relAnimTime;
    u32    var_a3;

    if (romper->model.anim.time <  Q12(147.0f) ||
        romper->model.anim.time >= Q12(163.0f))
    {
        return 0;
    }

    relAnimTime = romper->model.anim.time - Q12(147.0f);
    if (relAnimTime < romperProps.field_F4)
    {
        romperProps.field_F0 = 6;
        romperProps.field_F4 = Q12(0.0f);
        romper->moveSpeed = Q12(1.0f / 46.0f);
    }

    var_a3 = 0;
    for (i = 0; i < 7; i++)
    {
        if (sharedData_800ED2D4_2_s02[i].field_0 > romperProps.field_F4 ||
            romperProps.field_F4 >= sharedData_800ED2D4_2_s02[i + 1].field_0)
        {
            continue;
        }

        if (relAnimTime < sharedData_800ED2D4_2_s02[i + 1].field_0)
        {
            var_a3 += Q12_MULT_PRECISE(relAnimTime - romperProps.field_F4, sharedData_800ED2D4_2_s02[i].field_4);
            break;
        }

        var_a3 += Q12_MULT_PRECISE(sharedData_800ED2D4_2_s02[i + 1].field_0 - romperProps.field_F4, sharedData_800ED2D4_2_s02[i].field_4);
        romperProps.field_F4 = sharedData_800ED2D4_2_s02[i + 1].field_0;
    }

    romperProps.field_F4 = relAnimTime;
    return (var_a3 >> 31) + var_a3;
}

s32 sharedFunc_800E94B4_2_s02(s_SubCharacter* romper)
{
    s32 i;
    s32 relAnimTime;
    u32 var_t4;

    // TODO: Demagic hex values in this func. Some of the resulting floats are awkward.

    if (romper->model.anim.time <  Q12(109.0f) ||
        romper->model.anim.time >= Q12(131.0f))
    {
        return 0;
    }

    relAnimTime = romper->model.anim.time - Q12(109.0f);
    if (relAnimTime < romperProps.field_F4)
    {
        romperProps.field_F0 = -404;
        romperProps.field_F4 = Q12(0.0f);
        romper->moveSpeed                  = Q12(-0.256f);
    }

    var_t4 = 0;
    for (i = 0; i < 22; i++)
    {
        if (romperProps.field_F4 < FP_TO(i, Q12_SHIFT) ||
            romperProps.field_F4 >= FP_TO(i + 1, Q12_SHIFT))
        {
            continue;
        }

        if (relAnimTime >= FP_TO(i + 1, Q12_SHIFT))
        {
            switch (i + 1)
            {
                case 1:
                    if (romper->fallSpeed == 0)
                    {
                        romper->fallSpeed = -0x2A77;
                    }
                    break;

                case 11:
                    if (romper->fallSpeed == 0)
                    {
                        romper->fallSpeed = -0x30FF;
                    }
                    break;

                case 7:
                    var_t4                                = 0;
                    romperProps.field_F0 = 0x112;
                    romper->moveSpeed                  = Q12(1.268f);
                    break;

                case 18:
                    var_t4                                = 0;
                    romperProps.field_F0 = 0x3F;
                    romper->moveSpeed                  = 0xBF0;
                    break;

                default:
                    var_t4               += Q12_MULT_PRECISE(FP_TO(i + 1, Q12_SHIFT) - romperProps.field_F4, sharedData_800ED314_2_s02[i].field_0);
                    romper->moveSpeed += Q12_MULT_PRECISE(FP_TO(i + 1, Q12_SHIFT) - romperProps.field_F4, sharedData_800ED314_2_s02[i].field_4);
                    break;
            }
            romperProps.field_F4 = FP_TO(i + 1, Q12_SHIFT);
        }
        else
        {
            var_t4               += Q12_MULT_PRECISE(relAnimTime - romperProps.field_F4, sharedData_800ED314_2_s02[i].field_0);
            romper->moveSpeed += Q12_MULT_PRECISE(relAnimTime - romperProps.field_F4, sharedData_800ED314_2_s02[i].field_4);
            break;
        }
    }

    romperProps.field_F4 = relAnimTime;

    if ((relAnimTime >= Q12(1.0f) && relAnimTime < Q12(7.0f)) ||
        (relAnimTime >= Q12(11.0f) && relAnimTime < Q12(18.0f)))
    {
        sharedData_800EC950_2_s02 = Q12(12.0f);
    }
    else
    {
        sharedData_800EC950_2_s02 = Q12(24.0f);
    }

    return (var_t4 >> 31) + var_t4;
}

void sharedFunc_800E9714_2_s02(s_SubCharacter* romper)
{
    q19_12 newPosX;
    q19_12 newPosZ;
    q3_12  headingAngle;
    q19_12 moveSpeed;
    q19_12 temp_v0;
    q19_12 temp_v0_4;
    q19_12 moveDist;
    q19_12 moveSpeedStep;

    // TODO: Document this.
    moveSpeed     = romper->moveSpeed;
    moveSpeedStep = Q12(8.0f);
    temp_v0       = Q12_DIV(moveSpeed, moveSpeedStep);
    temp_v0_4     = moveSpeed - Q12_MULT_PRECISE(temp_v0, Q12(4.0f));
    headingAngle  = romper->rotation.vy;
    moveDist      = Q12_MULT_PRECISE(temp_v0_4, temp_v0);

    if (moveDist >= Math_Vector2MagCalcSafeQ6(PLAYER_CHARA().position.vx - romper->position.vx,
                                        PLAYER_CHARA().position.vz - romper->position.vz))
    {
        newPosX = romper->position.vx + Q12_MULT(moveDist, Math_Sin(headingAngle));
        newPosZ = romper->position.vz + Q12_MULT(moveDist, Math_Cos(headingAngle));
    }
    else
    {
        newPosX = PLAYER_CHARA().position.vx;
        newPosZ = PLAYER_CHARA().position.vz;
    }

    sharedFunc_800D2E9C_0_s00(&newPosX, &newPosZ, &headingAngle);

    temp_v0 = (FP_TO(Math_Vector2MagCalcSafeQ6(newPosX - romper->position.vx, newPosZ - romper->position.vz), Q12_SHIFT) << 1) / moveSpeed;
    moveSpeedStep = Q12_DIV(moveSpeed, temp_v0);

    if (moveSpeedStep < Q12(0.0f))
    {
        moveSpeedStep = Q12(4.0f);
    }

    if (moveSpeedStep > Q12(32.0f))
    {
        moveSpeedStep = Q12(32.0f);
    }

    romperProps.rotationY_F2  = ratan2(PLAYER_CHARA().position.vx - romper->position.vx,
                                       PLAYER_CHARA().position.vz - romper->position.vz);
    romperProps.field_124     = moveSpeedStep;
    romperProps.targetPositionX_FC  = newPosX;
    romperProps.targetPositionZ_100 = newPosZ;
}

#undef romperProps
