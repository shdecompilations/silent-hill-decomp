#include "bodyprog/bodyprog.h"
#include "bodyprog/events/npc_main.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player/player.h"
#include "main/rng.h"
#include "maps/common/shared.h"
#include "maps/common/characters/stalker.h"

#define stalkerProps stalker->properties.stalker

void Stalker_Update(s_SubCharacter* stalker, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords)
{
    if (g_SavegamePtr->gameDifficulty == GameDifficulty_Normal)
    {
        sharedData_800E3A20_0_s00 = Q12(350.0f);
        sharedData_800E3A24_0_s00 = Q12(100.0f);
        sharedData_800E3A28_0_s00 = Q12(3.0f);
        sharedData_800E3A2C_0_s00 = Q12(50.0f);
    }
    else if (g_SavegamePtr->gameDifficulty == GameDifficulty_Easy)
    {
        sharedData_800E3A20_0_s00 = Q12(350.0f);
        sharedData_800E3A24_0_s00 = Q12(100.0f);
        sharedData_800E3A28_0_s00 = Q12(0.0f);
        sharedData_800E3A2C_0_s00 = Q12(35.0f);
    }
    else
    {
        sharedData_800E3A20_0_s00 = Q12(600.0f);
        sharedData_800E3A24_0_s00 = Q12(180.0f);
        sharedData_800E3A28_0_s00 = Q12(6.0f);
        sharedData_800E3A2C_0_s00 = Q12(100.0f);
    }

    // Initialize.
    if (stalker->model.controlState == StalkerControl_None)
    {
        Stalker_Init(stalker);
    }

    if (stalker->model.controlState != StalkerControl_1)
    {
        if (g_DeltaTime != Q12(0.0f))
        {
            sharedFunc_800D3308_0_s00(stalker);
            Stalker_ControlUpdate(stalker);
            sharedFunc_800D67FC_0_s00(stalker);
        }

        sharedFunc_800D6970_0_s00(stalker, anmHdr, boneCoords);
        Stalker_CollisionUpdate(stalker);

        if (g_DeltaTime != Q12(0.0f))
        {
            sharedFunc_800D7BE8_0_s00(stalker);
        }

        stalkerProps.flags &= ~StalkerFlag_WarpRotation;
    }
}

void Stalker_Init(s_SubCharacter* stalker)
{
    s32 i;

    stalkerProps.flags = StalkerFlag_None;
    Chara_PropsClear(stalker);
    stalker->health = sharedData_800E3A20_0_s00;

#ifdef MAP6_S03
    // Set health to 1.5x when not on easy difficulty.
    if (g_SavegamePtr->gameDifficulty != GameDifficulty_Easy)
    {
        stalker->health = Q12_MULT_FLOAT_PRECISE(sharedData_800E3A20_0_s00, 1.5f);
    }
#endif

    stalker->moveSpeed   = Q12(0.0f);
    stalker->fallSpeed   = Q12(0.0f);
    stalker->rotation.vz = Q12_ANGLE(0.0f);
    stalker->rotation.vx = Q12_ANGLE(0.0f);
    Chara_AnimSet(stalker, ANIM_STATUS(StalkerAnim_30, true), 477);
    stalker->headingAngle = stalker->rotation.vy;

    // Handle state step.
    switch (stalker->model.stateStep)
    {
        case 11:
            stalkerProps.flags |= StalkerFlag_2;

        case 3:
            stalkerProps.timer_F8 = Q12(2.5f);
            stalkerProps.flags   |= StalkerFlag_0;

        case 8:
            stalker->model.controlState = StalkerControl_4;
            stalker->collision.state    = CharaCollisionState_Npc;
            break;

        case 7:
            stalker->model.controlState = StalkerControl_4;
            stalkerProps.flags         |= StalkerFlag_11;
            stalker->collision.state    = CharaCollisionState_Npc;
            break;

        case 6:
            stalker->model.controlState = StalkerControl_2;
            stalker->flags             |= CharaFlag_Hit;
            stalkerProps.timer_F8       = Q12(1.0f);
            stalkerProps.flags         |= StalkerFlag_0 | StalkerFlag_1;
            stalker->collision.state    = CharaCollisionState_Npc;
            stalker->flags             &= ~CharaFlag_Unk5;
            break;

        case 5:
            stalker->model.controlState = StalkerControl_1;
            stalker->collision.state    = CharaCollisionState_Ignore;
            stalker->flags             |= CharaFlag_Unk5;
            break;

        case 9:
            stalker->model.controlState = StalkerControl_12;
            Chara_AnimSet(stalker, ANIM_STATUS(StalkerAnim_27, true), 427);
            stalkerProps.flags         |= StalkerFlag_0;
            stalker->collision.state    = CharaCollisionState_Npc;
            break;

        case 10:
            stalker->model.controlState = StalkerControl_11;
            Chara_AnimSet(stalker, ANIM_STATUS(StalkerAnim_28, true), 443);
            stalker->collision.state    = 3;
            break;

        case 17:
            stalker->model.controlState = StalkerControl_13;
            Chara_AnimSet(stalker, ANIM_STATUS(StalkerAnim_14, true), 176);
            stalker->collision.state    = CharaCollisionState_Ignore;
            break;
    }

    stalker->model.stateStep    = 0;
    stalkerProps.sfxId_102          = NO_VALUE;
    stalkerProps.targetPositionX = stalker->position.vx;
    stalkerProps.targetPositionZ = stalker->position.vz;
    stalkerProps.health_110         = stalker->health;

    ModelAnim_AnimInfoSet(&stalker->model.anim, STALKER_ANIM_INFOS);
    Chara_DamageClear(stalker);

#ifdef MAP6_S03
    stalker->flags |= CharaFlag_NoRadioStatic;
#endif
}

void sharedFunc_800D3308_0_s00(s_SubCharacter* stalker)
{
    q3_12  angle;
    u16    keyframeIdx;
    q19_12 mag;
    q19_12 var_a0;
    q20_12 newHealth;

    var_a0                 = stalkerProps.timer_10C - Q12_MULT_PRECISE(g_DeltaTime, Q12(20.0f));
    stalkerProps.timer_10C = MAX(var_a0, Q12(0.0f));

    if (stalker->damage.amount > Q12(0.0f) && stalker->health > Q12(0.0f))
    {
        sharedFunc_800D7E04_0_s00(stalker, Sfx_Unk1365);
    }

    if (stalker->model.anim.status == ANIM_STATUS(StalkerAnim_34, true) ||
        stalker->model.anim.status == ANIM_STATUS(StalkerAnim_35, true))
    {
        stalker->damage.amount = Q12(0.0f);
    }

    if (stalker->health > Q12(0.0f))
    {
        newHealth          = stalker->health + Q12_MULT_PRECISE(g_DeltaTime, sharedData_800E3A28_0_s00);
        stalker->health = MIN(newHealth, stalkerProps.health_110);
    }

    if (stalker->damage.amount > Q12(0.0f))
    {
        if (stalker->health > Q12(0.0f))
        {
            stalker->health       = MAX(stalker->health - stalker->damage.amount, Q12(0.0f));
            stalkerProps.health_110 -= DIV_FAST(stalker->damage.amount, 4);
            stalkerProps.flags   |= StalkerFlag_0 | StalkerFlag_10;

            stalker->damage.position.vx += DIV_FAST(Q12_MULT(stalker->moveSpeed, Math_Sin(stalker->headingAngle)), 8);
            stalker->damage.position.vz += DIV_FAST(Q12_MULT(stalker->moveSpeed, Math_Cos(stalker->headingAngle)), 8);

            if (stalkerProps.flags & StalkerFlag_6)
            {
                mag          = Math_Vector2MagCalcSafeQ6(stalker->damage.position.vx, stalker->damage.position.vz);
                stalker->moveSpeed = Q12_DIV(mag, Q12(12.0f));
            }
            else
            {
                mag          = Math_Vector2MagCalcSafeQ6(stalker->damage.position.vx, stalker->damage.position.vz);
                stalker->moveSpeed = Q12_DIV(mag, Q12(1.5f));
            }

            stalker->headingAngle              = ratan2(stalker->damage.position.vx, stalker->damage.position.vz);
            stalkerProps.flags                |= StalkerFlag_Moving;
            stalkerProps.timer_10C += stalker->damage.amount;

            if (stalkerProps.timer_10C >= sharedData_800E3A2C_0_s00 ||
                (!(stalkerProps.flags & StalkerFlag_6) && stalker->health <= sharedData_800E3A24_0_s00))
            {
                stalkerProps.timer_10C = Q12(0.0f);

                angle = ABS(Math_AngleNormalizeSigned(Math_AngleBetweenPositionsGet(stalker->position, g_SysWork.playerWork.player.position) -
                                          stalker->rotation.vy));

                if (stalkerProps.flags & StalkerFlag_6)
                {
                    if (stalker->model.anim.status == ANIM_STATUS(StalkerAnim_26, true))
                    {
                        stalkerProps.keyframeIdx_FC = NO_VALUE;
                        stalker->model.anim.status                       = ANIM_STATUS(StalkerAnim_38, false);
                    }
                    else if (stalker->model.anim.status == ANIM_STATUS(StalkerAnim_17, true))
                    {
                        stalkerProps.keyframeIdx_FC = NO_VALUE;
                        stalker->model.anim.status                       = ANIM_STATUS(StalkerAnim_39, false);
                    }
                    else if (stalker->model.anim.status == ANIM_STATUS(StalkerAnim_18, true))
                    {
                        stalkerProps.keyframeIdx_FC = NO_VALUE;
                        stalker->model.anim.status                       = ANIM_STATUS(StalkerAnim_40, false);
                    }
                }
                else
                {
                    if (stalker->health > sharedData_800E3A24_0_s00 && !(stalkerProps.flags & StalkerFlag_7))
                    {
                        if (stalker->model.controlState == StalkerControl_5)
                        {
                            g_SysWork.charaGroupFlags[3] &= ~CharaGroupFlag_1;
                        }

                        stalker->model.controlState = StalkerControl_7;

                        if (ANIM_STATUS_IDX_GET(stalker->model.anim.status) < StalkerAnim_22 ||
                            ANIM_STATUS_IDX_GET(stalker->model.anim.status) > StalkerAnim_23)
                        {
                            if (angle >= Q12_ANGLE(60.0f))
                            {
                                stalker->model.anim.status = ANIM_STATUS(StalkerAnim_23, false);
                            }
                            else
                            {
                                stalker->model.anim.status = ANIM_STATUS(StalkerAnim_22, false);
                            }
                        }
                    }
                    else if (stalkerProps.flags & StalkerFlag_7)
                    {
                        stalker->model.controlState = StalkerControl_9;

                        if (ANIM_STATUS_IDX_GET(stalker->model.anim.status) == StalkerAnim_11)
                        {
                            if (stalker->model.anim.status == ANIM_STATUS(StalkerAnim_11, false))
                            {
                                stalkerProps.keyframeIdx_FC = 0;
                                stalker->model.anim.status                               = ANIM_STATUS(StalkerAnim_38, false);
                            }
                            else if (FP_FROM(stalker->model.anim.time, Q12_SHIFT) > 106 &&
                                     FP_FROM(stalker->model.anim.time, Q12_SHIFT) < 109)
                            {
                                stalkerProps.keyframeIdx_FC = FP_FROM(stalker->model.anim.time, Q12_SHIFT) - 107;
                                stalker->model.anim.status                       = ANIM_STATUS(StalkerAnim_38, false);
                            }
                            else if (FP_FROM(stalker->model.anim.time, Q12_SHIFT) > 108 &&
                                     FP_FROM(stalker->model.anim.time, Q12_SHIFT) < 113)
                            {
                                if (angle >= Q12_ANGLE(60.0f))
                                {
                                    stalker->model.anim.status = ANIM_STATUS(StalkerAnim_42, false);
                                }
                                else
                                {
                                    stalker->model.anim.status = ANIM_STATUS(StalkerAnim_1, false);
                                }

                                stalkerProps.keyframeIdx_FC = FP_FROM(stalker->model.anim.time, Q12_SHIFT) - 107;

                                if (stalkerProps.keyframeIdx_FC == 2)
                                {
                                    stalkerProps.relKeyframeIdx_FE = 2;
                                }
                                else if (stalkerProps.keyframeIdx_FC == 3)
                                {
                                    stalkerProps.relKeyframeIdx_FE = 1;
                                }
                                else
                                {
                                    stalkerProps.relKeyframeIdx_FE = 0;
                                }
                            }
                            else
                            {
                                if (FP_FROM(stalker->model.anim.time, Q12_SHIFT) > 112 &&
                                    FP_FROM(stalker->model.anim.time, Q12_SHIFT) < 118)
                                {
                                    if (angle >= Q12_ANGLE(60.0f))
                                    {
                                        stalker->model.anim.status = ANIM_STATUS(StalkerAnim_43, false);
                                    }
                                    else
                                    {
                                        stalker->model.anim.status = ANIM_STATUS(StalkerAnim_3, false);
                                    }

                                    keyframeIdx = FP_FROM(stalker->model.anim.time, Q12_SHIFT);

                                    stalkerProps.keyframeIdx_FC = keyframeIdx - 107;

                                    if (keyframeIdx == 116 || keyframeIdx == 117)
                                    {
                                        stalkerProps.relKeyframeIdx_FE = 1;
                                    }
                                    else
                                    {
                                        stalkerProps.relKeyframeIdx_FE = 2;
                                    }
                                }
                                else
                                {
                                    if (angle >= Q12_ANGLE(60.0f))
                                    {
                                        stalker->model.anim.status = ANIM_STATUS(StalkerAnim_44, false);
                                    }
                                    else
                                    {
                                        stalker->model.anim.status = ANIM_STATUS(StalkerAnim_2, false);
                                    }

                                    stalkerProps.keyframeIdx_FC = FP_FROM(stalker->model.anim.time, Q12_SHIFT) - 107;
                                }
                            }
                        }
                        else if (ANIM_STATUS_IDX_GET(stalker->model.anim.status) == StalkerAnim_12)
                        {
                            if (stalker->model.anim.status == ANIM_STATUS(StalkerAnim_12, false))
                            {
                                stalkerProps.keyframeIdx_FC = 0;
                            }
                            else
                            {
                                stalkerProps.keyframeIdx_FC = FP_FROM(stalker->model.anim.time, Q12_SHIFT) - 129;
                            }

                            if (stalker->model.anim.status == ANIM_STATUS(StalkerAnim_12, false) ||
                                (FP_FROM(stalker->model.anim.time, Q12_SHIFT) > 128 &&
                                 FP_FROM(stalker->model.anim.time, Q12_SHIFT) < 136))
                            {
                                stalker->model.anim.status = ANIM_STATUS(StalkerAnim_39, false);
                            }
                            else if (FP_FROM(stalker->model.anim.time, Q12_SHIFT) > 135 &&
                                     FP_FROM(stalker->model.anim.time, Q12_SHIFT) < 142)
                            {
                                stalker->model.anim.status                               = ANIM_STATUS(StalkerAnim_4, false);
                                stalkerProps.relKeyframeIdx_FE = (12 - stalkerProps.keyframeIdx_FC) >> 1;
                            }
                            else if (FP_FROM(stalker->model.anim.time, Q12_SHIFT) > 141 &&
                                     FP_FROM(stalker->model.anim.time, Q12_SHIFT) < 149)
                            {
                                stalker->model.anim.status = ANIM_STATUS(StalkerAnim_5, false);

                                switch (stalkerProps.keyframeIdx_FC)
                                {
                                    case 13:
                                        stalkerProps.relKeyframeIdx_FE = 5;
                                        break;

                                    case 14:
                                    case 15:
                                        stalkerProps.relKeyframeIdx_FE = 4;
                                        break;

                                    case 16:
                                    case 17:
                                        stalkerProps.relKeyframeIdx_FE = 3;
                                        break;

                                    default:
                                        stalkerProps.relKeyframeIdx_FE = 2;
                                        break;
                                }
                            }
                            else
                            {
                                stalker->model.anim.status                       = ANIM_STATUS(StalkerAnim_5, false);
                                stalkerProps.relKeyframeIdx_FE = 1;
                            }
                        }
                        else
                        {
                            if (stalker->model.anim.status == ANIM_STATUS(StalkerAnim_13, false))
                            {
                                stalkerProps.keyframeIdx_FC = 0;
                                stalker->model.anim.status                       = ANIM_STATUS(StalkerAnim_40, false);
                            }
                            else
                            {
                                stalkerProps.keyframeIdx_FC = FP_FROM(stalker->model.anim.time, Q12_SHIFT) - 158;

                                if (FP_FROM(stalker->model.anim.time, Q12_SHIFT) == 158 ||
                                    FP_FROM(stalker->model.anim.time, Q12_SHIFT) == 159)
                                {
                                    stalker->model.anim.status = ANIM_STATUS(StalkerAnim_40, false);
                                }
                                else if (!ANIM_TIME_RANGE_CHECK(stalker->model.anim.time, 160, 168))
                                {
                                    stalkerProps.relKeyframeIdx_FE = 0;
                                    stalker->model.anim.status = ANIM_STATUS(StalkerAnim_5, false);
                                }
                                else
                                {
                                    stalker->model.anim.status = ANIM_STATUS(StalkerAnim_45, true);
                                }
                            }
                        }

                        stalkerProps.flags &= ~StalkerFlag_7;
                    }
                    else
                    {
                        stalker->model.controlState = StalkerControl_9;

                        if (angle < Q12_ANGLE(60.0f))
                        {
                            if (FP_FROM(stalker->model.anim.time, Q12_SHIFT) > 361 &&
                                FP_FROM(stalker->model.anim.time, Q12_SHIFT) < 373)
                            {
                                if (ANIM_TIME_RANGE_CHECK(stalker->model.anim.time, 364, 367))
                                {
                                    stalkerProps.relKeyframeIdx_FE = 2;
                                }
                                else
                                {
                                    stalkerProps.relKeyframeIdx_FE = 1;
                                }

                                stalkerProps.keyframeIdx_FC = FP_FROM(stalker->model.anim.time, Q12_SHIFT) - 362;
                            }
                            else
                            {
                                stalkerProps.relKeyframeIdx_FE = 0;
                            }

                            stalker->model.anim.status = ANIM_STATUS(StalkerAnim_24, false);
                        }
                        else
                        {
                            if (stalker->model.anim.status == ANIM_STATUS(StalkerAnim_11, true))
                            {
                                stalkerProps.relKeyframeIdx_FE = 23;
                                stalker->model.anim.status                       = ANIM_STATUS(StalkerAnim_25, false);
                                stalkerProps.keyframeIdx_FC = FP_FROM(stalker->model.anim.time, Q12_SHIFT) - 107;
                            }
                            else if (stalker->model.anim.status == ANIM_STATUS(StalkerAnim_12, true))
                            {
                                stalkerProps.relKeyframeIdx_FE = 25;
                                stalker->model.anim.status                       = ANIM_STATUS(StalkerAnim_25, false);
                                stalkerProps.keyframeIdx_FC = FP_FROM(stalker->model.anim.time, Q12_SHIFT) - 129;
                            }
                            else if (stalker->model.anim.status == ANIM_STATUS(StalkerAnim_13, true))
                            {
                                stalkerProps.relKeyframeIdx_FE = 27;
                                stalker->model.anim.status                       = ANIM_STATUS(StalkerAnim_25, false);
                                stalkerProps.keyframeIdx_FC = FP_FROM(stalker->model.anim.time, Q12_SHIFT) - 158;
                            }
                            else
                            {
                                stalkerProps.relKeyframeIdx_FE = 83;
                                stalker->model.anim.status = ANIM_STATUS(StalkerAnim_41, false);
                            }
                        }
                    }
                }

                stalkerProps.timer_F8  = Q12(0.0f);
                stalkerProps.flags &= 0xF7E7; // TODO: Many flags here.
            }
        }
    }

    Chara_DamageClear(stalker);
}

void Stalker_ControlUpdate(s_SubCharacter* stalker)
{
    q3_12 angle;
    s32   temp_s1;
    s32   i;
    s32   angle1;
    s32   unkAnimStatus;
    s32   animStatus;

    if (stalkerProps.flags & StalkerFlag_10)
    {
        stalker->flags |= CharaFlag_Hit;
    }

    if (stalker->model.anim.status != ANIM_STATUS(StalkerAnim_34, true))
    {
        animStatus = stalker->model.anim.status;

        if (animStatus > ANIM_STATUS(StalkerAnim_34, true))
        {
            unkAnimStatus = ANIM_STATUS(StalkerAnim_35, true);
            unkAnimStatus++; // } TODO: Hack.
            unkAnimStatus--; // }
        }
        else
        {
            unkAnimStatus = ANIM_STATUS(StalkerAnim_7, true);
        }

        if (animStatus != unkAnimStatus)
        {
            stalker->field_44.field_0 = 0;
        }
    }

    if (!(g_SysWork.bgmStatusFlags & BgmStatusFlag_4))
    {
        g_SysWork.bgmStatusFlags |= BgmStatusFlag_4;

        for (i = 4; i > 0; i--)
        {
            sharedData_800E39EC_0_s00[i] = sharedData_800E39EC_0_s00[i - 1];
            sharedData_800E3A0C_0_s00[i] = sharedData_800E3A0C_0_s00[i - 1];
        }

        sharedData_800E39EC_0_s00[0] = FP_TO(Math_Vector2MagCalcSafeQ6(g_SysWork.playerWork.player.position.vx - sharedData_800E39E4_0_s00,
                                                                       g_SysWork.playerWork.player.position.vz - sharedData_800E39E8_0_s00), Q12_SHIFT) / g_DeltaTime;
        sharedData_800E3A0C_0_s00[0] = ratan2(g_SysWork.playerWork.player.position.vx - sharedData_800E39E4_0_s00, g_SysWork.playerWork.player.position.vz - sharedData_800E39E8_0_s00);
        sharedData_800E39E4_0_s00    = g_SysWork.playerWork.player.position.vx;
        sharedData_800E39E8_0_s00    = g_SysWork.playerWork.player.position.vz;

        if (sharedData_800DD5A6_0_s00 != 5)
        {
            sharedData_800DD5A6_0_s00++;
            g_Stalker_TargetPositionX = g_SysWork.playerWork.player.position.vx;
            g_Stalker_TargetPositionZ = g_SysWork.playerWork.player.position.vz;
        }
        else
        {
            angle1 = Q12_ANGLE(0.0f);
            for (i = 0; i < 4; i++)
            {
                angle1 += Math_AngleNormalizeSigned(sharedData_800E3A0C_0_s00[i] - sharedData_800E3A0C_0_s00[i + 1]);
            }

            angle = sharedData_800E3A0C_0_s00[0] + (angle1 >> 2);

            angle1 = 0;
            for (i = 0; i < 4; i++)
            {
                angle1 += sharedData_800E39EC_0_s00[i] - sharedData_800E39EC_0_s00[i + 1];
            }

            temp_s1                   = sharedData_800E39EC_0_s00[0] + (angle1 >> 2);
            g_Stalker_TargetPositionX = ((temp_s1 * Math_Sin(angle)) >> 14) + g_SysWork.playerWork.player.position.vx;
            g_Stalker_TargetPositionZ = ((temp_s1 * Math_Cos(angle)) >> 14) + g_SysWork.playerWork.player.position.vz;
        }
    }

    // Handle control state.
    switch (stalker->model.controlState)
    {
        case StalkerControl_None:
            break;

        case StalkerControl_2:
            Stalker_Control_2(stalker);
            break;

        case StalkerControl_3:
            Stalker_Control_3(stalker);
            break;

        case StalkerControl_8:
            Stalker_Control_8(stalker);
            break;

        case StalkerControl_4:
            Stalker_Control_4(stalker);
            break;

        case StalkerControl_5:
            Stalker_Control_5(stalker);
            break;

        case StalkerControl_6:
            Stalker_Control_6(stalker);
            break;

        case StalkerControl_7:
            Stalker_Control_7(stalker);
            break;

        case StalkerControl_9:
            Stalker_Control_9(stalker);
            break;

        case StalkerControl_10:
            Stalker_Control_10(stalker);
            break;

        case StalkerControl_11:
            Stalker_Control_11(stalker);
            break;

        case StalkerControl_12:
            Stalker_Control_12(stalker);
            break;

        case StalkerControl_13:
            Stalker_Control_13(stalker);
            break;
    }
}

void Stalker_Control_2(s_SubCharacter* stalker)
{
#if defined(MAP0_S00)
    q3_12  angleDeltaToTargetHeading;
    q3_12  grabAngleDelta;
    q3_12  angleDeltaToPlayer;
    q19_12 distToPlayer;
    s32    i;

    // @hack Separate `Q6_TO_Q12` needed for match, `Math_Vector2MagCalcSafeQ6` swaps two saved registers.
    distToPlayer       = Math_Vector2MagCalc(Q12_TO_Q6(g_Stalker_TargetPositionX - stalker->position.vx),
                                             Q12_TO_Q6(g_Stalker_TargetPositionZ - stalker->position.vz));
    distToPlayer       = Q6_TO_Q12(distToPlayer);
    angleDeltaToPlayer = Math_AngleNormalizeSigned(ratan2(g_Stalker_TargetPositionX - stalker->position.vx,
                                              g_Stalker_TargetPositionZ - stalker->position.vz) -
                                       stalker->rotation.vy);

    if (distToPlayer > Q12(8.0f) && (stalkerProps.flags & StalkerFlag_0) && !(g_SysWork.sysFlags & SysFlag_MenuActive))
    {
        if (!Vw_AabbVisibleInScreenCheck(stalker->position.vx - Q12(1.0f), stalker->position.vx + Q12(1.0f),
                                         stalker->position.vy - Q12(1.8f), stalker->position.vy,
                                         stalker->position.vz - Q12(1.0f), stalker->position.vz - Q12(1.0f)))
        {
            stalkerProps.flags |= StalkerFlag_WarpRotation;
        }
    }

    if (stalker->model.anim.status == ANIM_STATUS(StalkerAnim_30, true) &&
        !Los_NpcToPlayerHitCheck(stalker, &g_SysWork.playerWork.player))
    {
        stalkerProps.timer_F8 = Q12(0.0f);

        if (stalkerProps.flags & StalkerFlag_WarpRotation)
        {
            stalker->rotation.vy = Math_AngleBetweenPositionsGet(stalker->position, g_SysWork.playerWork.player.position);
        }
        else
        {
            if (ABS(angleDeltaToPlayer) > Q12_ANGLE(90.0f))
            {
                Chara_MoveSpeedUpdate(stalker, Q12(1.5f));
            }
            else
            {
                Chara_MoveSpeedUpdate3(stalker, Q12_ANGLE(180.0f), (Q12_ANGLE(90.0f) - ABS(angleDeltaToPlayer)) * 4);
            }
        }

        for (i = 3 + ((ABS(angleDeltaToPlayer) > Q12_ANGLE(90.0f)) ? 1 : 0); i > 0; i--)
        {
            if (TIMESTEP_ANGLE(1, 2) < ABS(angleDeltaToPlayer))
            {
                if (angleDeltaToPlayer > Q12_ANGLE(0.0f))
                {
                    stalker->rotation.vy += Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(180.0f));
                }
                else
                {
                    stalker->rotation.vy -= Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(180.0f));
                }

                angleDeltaToPlayer = Math_AngleNormalizeSigned(ratan2(g_Stalker_TargetPositionX - stalker->position.vx,
                                                          g_Stalker_TargetPositionZ - stalker->position.vz) -
                                                   stalker->rotation.vy);
            }

            if (ABS(angleDeltaToPlayer) < TIMESTEP_ANGLE(1, 2) || distToPlayer > Q12(2.0f))
            {
                break;
            }
        }

        if ((g_SysWork.charaGroupFlags[3] & CharaGroupFlag_0) ||
            ( (stalkerProps.flags & StalkerFlag_2) && Rng_GenerateInt(0, 3) != 0) || // 1 in 4 chance.
            (!(stalkerProps.flags & StalkerFlag_2) && Rng_GenerateInt(0, 1) != 0))   // 1 in 2 chance.
        {
            if ( distToPlayer < Q12(0.9f) &&
                (distToPlayer > Q12(0.7f) || !Rng_GenerateInt(0, 3))) // 1 in 4 chance.
            {
                if (ABS(angleDeltaToPlayer) < Q12_ANGLE(90.0f) && g_SysWork.playerWork.player.health > Q12(0.0f))
                {
                    stalker->model.controlState = StalkerControl_5;
                    g_SysWork.charaGroupFlags[3] |= CharaGroupFlag_1;

                    sharedFunc_800D7E04_0_s00(stalker, Sfx_Unk1364);

                    if (ANIM_TIME_RANGE_CHECK(stalker->model.anim.time, 493, 504))
                    {
                        stalker->model.anim.status = ANIM_STATUS(StalkerAnim_37, true);
                        stalkerProps.keyframeIdx_FC      = ANIM_STATUS(StalkerAnim_14, true);
                        stalkerProps.relKeyframeIdx_FE   = 0;
                        return;
                    }

                    stalker->model.anim.status = ANIM_STATUS(StalkerAnim_7, false);
                    stalkerProps.keyframeIdx_FC = ANIM_TIME_REL_KEYFRAME_IDX_GET(stalker->model.anim.time, 477);

                    if (ANIM_TIME_RANGE_CHECK(stalker->model.anim.time, 478, 481))
                    {
                        stalkerProps.relKeyframeIdx_FE = 1;
                    }
                    else if (ANIM_TIME_RANGE_CHECK(stalker->model.anim.time, 482, 487))
                    {
                        stalkerProps.relKeyframeIdx_FE = 2;
                    }
                    else if (ANIM_TIME_RANGE_CHECK(stalker->model.anim.time, 488, 490))
                    {
                        stalkerProps.relKeyframeIdx_FE = 3;
                    }
                    else if (ANIM_TIME_RANGE_CHECK(stalker->model.anim.time, 491, 492))
                    {
                        stalkerProps.relKeyframeIdx_FE = 4;
                    }
                    else
                    {
                        stalkerProps.relKeyframeIdx_FE = 0;
                    }
                    return;
                }
            }
        }
        // Attack player if player is alive.
        else if (distToPlayer > Q12(0.5f) && distToPlayer < Q12(1.0f))
        {
            if (ABS(angleDeltaToPlayer) < Q12_ANGLE(60.0f))
            {
                if (g_SysWork.playerWork.player.health > Q12(0.0f))
                {
                    stalker->model.controlState = StalkerControl_6;

                    // Grab player in front or behind.
                    grabAngleDelta = Math_AngleNormalizeSigned(g_SysWork.playerWork.player.rotation.vy - stalker->rotation.vy);
                    if (ABS(grabAngleDelta) > Q12_ANGLE(90.0f))
                    {
                        stalker->model.anim.status = ANIM_STATUS(StalkerAnim_34, false);
                    }
                    else
                    {
                        stalker->model.anim.status = ANIM_STATUS(StalkerAnim_35, false);
                    }

                    stalkerProps.keyframeIdx_FC = ANIM_TIME_REL_KEYFRAME_IDX_GET(stalker->model.anim.time, 477);
                    g_SysWork.charaGroupFlags[3]    |= CharaGroupFlag_0 | CharaGroupFlag_1;
                    return;
                }
            }
        }

        if (distToPlayer < Q12(0.7f))
        {
            stalker->model.controlState = StalkerControl_4;
        }
    }
    else
    {
        Chara_MoveSpeedUpdate3(stalker, Q12(0.5f), Q12(1.0f));

        // Set target heading angle.
        if (stalkerProps.timer_F8 == Q12(0.0f))
        {
            stalkerProps.targetHeadingAngle = Los_CharaHeadingAngleGet(stalker,
                                                                        Q12(2.0f),
                                                                        g_SysWork.playerWork.player.position.vx,
                                                                        g_SysWork.playerWork.player.position.vz,
                                                                        Q12(1.0f),
                                                                        true);
            if (stalkerProps.targetHeadingAngle == Q12_ANGLE(360.0f))
            {
                stalkerProps.targetHeadingAngle = stalker->rotation.vy - Q12_ANGLE(180.0f);
            }
        }

        // Update timer.
        stalkerProps.timer_F8 += g_DeltaTime;
        if (stalkerProps.timer_F8 > Q12(2.5f) && Rng_GenerateInt(0, 15) == 0) // 1 in 16 chance.
        {
            stalkerProps.timer_F8 = Q12(0.0f);
        }

        // Turn toward target heading angle.
        if (stalkerProps.flags & StalkerFlag_WarpRotation)
        {
            stalker->rotation.vy = stalkerProps.targetHeadingAngle;
        }
        else
        {
            angleDeltaToTargetHeading = Math_AngleNormalizeSigned(stalkerProps.targetHeadingAngle - stalker->rotation.vy);
            if (TIMESTEP_ANGLE(1, 2) < ABS(angleDeltaToTargetHeading))
            {
                if (angleDeltaToTargetHeading > Q12_ANGLE(0.0f))
                {
                    stalker->rotation.vy += Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(180.0f));
                }
                else
                {
                    stalker->rotation.vy -= Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(180.0f));
                }
            }
        }
    }
#endif
}

void Stalker_Control_3(s_SubCharacter* stalker)
{
#if !defined(MAP0_S00)
    q3_12  grabAngleDelta;
    s16    angleDeltaToTargetHeading;
    q3_12  angleDeltaToPlayer;
    q19_12 distToPlayer;
    q19_12 radiusMax;
    q19_12 radiusMin;
    s32    i;
    bool   cond1;
    q25_6  temp;
    q19_12 temp2;

    distToPlayer  = Math_Vector2MagCalcSafeQ6(g_Stalker_TargetPositionX - stalker->position.vx,
                                        g_Stalker_TargetPositionZ - stalker->position.vz);
    angleDeltaToPlayer = Math_AngleNormalizeSigned(ratan2(g_Stalker_TargetPositionX - stalker->position.vx,
                                              g_Stalker_TargetPositionZ - stalker->position.vz) -
                                       stalker->rotation.vy);

    if (!(g_SysWork.field_2388.field_154.effectsInfo.flags.field_0 & (SpecialEnvEventFlags_DarkEnvironment | SpecialEnvEventFlags_FlashlightAllowed)))
    {
        radiusMax = Q12(15.0f);
        radiusMin = Q12(5.5f);
        cond1  = func_8006FD90(stalker, 0, Q12(7.5f), Q12(7.5f));
    }
    else if ((g_SysWork.field_2388.field_154.effectsInfo.flags.field_0 & (SpecialEnvEventFlags_DarkEnvironment | SpecialEnvEventFlags_FlashlightAllowed)) == SpecialEnvEventFlags_FlashlightAllowed)
    {
#if defined(MAP1_S02) || defined(MAP1_S03) || defined(MAP5_S02) || defined(MAP6_S03) || defined(MAP6_S04) || defined(MAP7_S02)
        radiusMax = Q12(16.0f);
        radiusMin = Q12(6.0f);
        cond1  = func_8006FD90(stalker, 0, Q12(8.0f), Q12(8.0f));
#else
        radiusMax = Q12(12.0f);
        radiusMin = Q12(4.5f);
        cond1  = func_8006FD90(stalker, 0, Q12(6.0f), Q12(6.0f));
#endif
    }
    else
    {
        radiusMax = Q12(0.8f);
#if defined(MAP1_S02) || defined(MAP1_S03) || defined(MAP5_S02) || defined(MAP6_S03) || defined(MAP6_S04) || defined(MAP7_S02)
        radiusMin = Q12(1.2f);
#else
        radiusMin = Q12(1.0f);
#endif
        cond1 = func_8006FD90(stalker, 1, Q12(0.4f), Q12(0.4f));
    }

    if (cond1 || func_80070360(stalker, distToPlayer, Q12(1.0f)))
    {
        stalkerProps.targetPositionX = g_Stalker_TargetPositionX;
        stalkerProps.targetPositionZ = g_Stalker_TargetPositionZ;

        if (!(stalkerProps.flags & StalkerFlag_10))
        {
            stalkerProps.flags |= StalkerFlag_10;
            sharedFunc_800D7E04_0_s00(stalker, Sfx_Unk1363);
        }

        stalkerProps.timer_116 = Q12(0.0f);
    }
    else if (stalkerProps.flags & StalkerFlag_10)
    {
        stalkerProps.timer_116 += g_DeltaTime;

        if (!Rng_GenerateUInt(0, 511) || stalkerProps.timer_116 > Q12(1.2f))
        {
            stalkerProps.timer_116 = Q12(0.0f);
            stalkerProps.flags &= ~StalkerFlag_10;
        }
    }

    if (distToPlayer > Q12(8.0f))
    {
        if ((stalkerProps.flags & StalkerFlag_0) &&
            (stalkerProps.flags & StalkerFlag_10) &&
            !(g_SysWork.sysFlags & SysFlag_MenuActive))
        {
            if (!Vw_AabbVisibleInScreenCheck(stalker->position.vx - Q12(1.0f), stalker->position.vx + Q12(1.0f),
                                             stalker->position.vy - Q12(1.8f), stalker->position.vy,
                                             stalker->position.vz - Q12(1.0f), stalker->position.vz - Q12(1.0f)))
            {
                stalkerProps.flags |= StalkerFlag_WarpRotation;
            }
        }
    }

    if (stalker->model.anim.status == ANIM_STATUS(StalkerAnim_30, true) &&
        !(stalkerProps.flags & StalkerFlag_Falling) &&
        !Los_NpcToPlayerHitCheck(stalker, &g_SysWork.playerWork.player))
    {
        stalkerProps.timer_F8 = Q12(0.0f);

        if (!(stalkerProps.flags & StalkerFlag_Moving))
        {
            Chara_MoveSpeedUpdate3(stalker, Q12_ANGLE(180.0f), (Q12_ANGLE(180.0f) - ABS(angleDeltaToPlayer)) * 2);
        }

        if (stalkerProps.flags & StalkerFlag_WarpRotation)
        {
            stalker->rotation.vy = ratan2(stalkerProps.targetPositionX - stalker->position.vx,
                                             stalkerProps.targetPositionZ - stalker->position.vz);
        }
        else
        {
            for (i = 3 + ((ABS(angleDeltaToPlayer) > Q12_ANGLE(90.0f)) ? 1 : 0); i > 0; i--)
            {
                if (TIMESTEP_ANGLE(1, 3) < ABS(angleDeltaToPlayer))
                {
                    if (angleDeltaToPlayer > Q12_ANGLE(0.0f))
                    {
                        stalker->rotation.vy += Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(90.0f));
                    }
                    else
                    {
                        stalker->rotation.vy -= Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(90.0f));
                    }

                    angleDeltaToPlayer = Math_AngleNormalizeSigned(ratan2(g_Stalker_TargetPositionX - stalker->position.vx,
                                                              g_Stalker_TargetPositionZ - stalker->position.vz) -
                                                       stalker->rotation.vy);
                }

                if (ABS(angleDeltaToPlayer) < TIMESTEP_ANGLE(1, 3) || distToPlayer > (radiusMin >> 2))
                {
                    break;
                }
            }
        }

        if ((g_SysWork.charaGroupFlags[3] & CharaGroupFlag_0) || func_80070320() ||
            ABS(stalker->position.vy - g_SysWork.playerWork.player.position.vy) > Q12(0.3f) ||
            ( (stalkerProps.flags & StalkerFlag_2) &&  Rng_GenerateInt(0, 3)) || // 3 in 4 chance?
            (!(stalkerProps.flags & StalkerFlag_2) && !Rng_GenerateInt(0, 3)))   // 1 in 4 chance.
        {
            if (!(g_SysWork.playerWork.player.flags & CharaFlag_Unk4) &&
                distToPlayer < Q12(0.9f) && (distToPlayer > Q12(0.7f) || !Rng_GenerateInt(0, 3)) && // 1 in 4 chance.
                ABS(angleDeltaToPlayer) < Q12_ANGLE(90.0f) && g_SysWork.playerWork.player.health > Q12(0.0f))
            {
                stalker->model.controlState = StalkerControl_5;
                g_SysWork.charaGroupFlags[3] |= CharaGroupFlag_1;

                sharedFunc_800D7E04_0_s00(stalker, Sfx_Unk1364);

                if (ANIM_TIME_RANGE_CHECK(stalker->model.anim.time, 493, 504))
                {
                    stalker->model.anim.status = ANIM_STATUS(StalkerAnim_37, true);
                    stalkerProps.keyframeIdx_FC      = ANIM_STATUS(StalkerAnim_14, true);
                    stalkerProps.relKeyframeIdx_FE   = 0;
                }
                else
                {
                    stalker->model.anim.status = ANIM_STATUS(StalkerAnim_7, false);
                    stalkerProps.keyframeIdx_FC = ANIM_TIME_REL_KEYFRAME_IDX_GET(stalker->model.anim.time, 477);

                    if (ANIM_TIME_RANGE_CHECK(stalker->model.anim.time, 478, 481))
                    {
                        stalkerProps.relKeyframeIdx_FE = 1;
                    }
                    else if (ANIM_TIME_RANGE_CHECK(stalker->model.anim.time, 482, 487))
                    {
                        stalkerProps.relKeyframeIdx_FE = 2;
                    }
                    else if (ANIM_TIME_RANGE_CHECK(stalker->model.anim.time, 488, 490))
                    {
                        stalkerProps.relKeyframeIdx_FE = 3;
                    }
                    else if (ANIM_TIME_RANGE_CHECK(stalker->model.anim.time, 491, 492))
                    {
                        stalkerProps.relKeyframeIdx_FE = 4;
                    }
                    else
                    {
                        stalkerProps.relKeyframeIdx_FE = 0;
                    }
                }
            }
            else if (distToPlayer < Q12(0.7f))
            {
                stalker->model.controlState = StalkerControl_4;
            }
        }
        else if (!(g_SysWork.playerWork.player.flags & CharaFlag_Unk4) &&
                 distToPlayer < Q12(1.0f) && distToPlayer > Q12(0.5f) &&
                 ABS(angleDeltaToPlayer) < Q12_ANGLE(60.0f) && g_SysWork.playerWork.player.health > Q12(0.0f))
        {
            stalker->model.controlState = StalkerControl_6;

            // Grab player in front or behind.
            grabAngleDelta = Math_AngleNormalizeSigned(g_SysWork.playerWork.player.rotation.vy - stalker->rotation.vy);
            if (ABS(grabAngleDelta) > Q12_ANGLE(90.0f))
            {
                stalker->model.anim.status = ANIM_STATUS(StalkerAnim_34, false);
            }
            else
            {
                stalker->model.anim.status = ANIM_STATUS(StalkerAnim_35, false);
            }

            stalkerProps.keyframeIdx_FC = ANIM_TIME_REL_KEYFRAME_IDX_GET(stalker->model.anim.time, 477);
            g_SysWork.charaGroupFlags[3]    |= CharaGroupFlag_0 | CharaGroupFlag_1;
            stalker->field_44.field_0   = 1;
        }
        else if (distToPlayer < Q12(0.7f))
        {
            stalker->model.controlState = StalkerControl_4;
        }
    }
    else
    {
        if (!(stalkerProps.flags & StalkerFlag_Moving))
        {
            Chara_MoveSpeedUpdate3(stalker, Q12(0.5f), Q12(0.7f));
        }

        if (stalkerProps.timer_F8 == Q12(0.0f))
        {
            stalkerProps.targetHeadingAngle = Los_CharaHeadingAngleGet(stalker,
                                                                        Q12(2.5f),
                                                                        stalkerProps.targetPositionX,
                                                                        stalkerProps.targetPositionZ,
                                                                        Q12(1.0f),
                                                                        true);
            if (stalkerProps.targetHeadingAngle == Q12_ANGLE(360.0f))
            {
                stalkerProps.targetHeadingAngle = stalker->rotation.vy - Q12_ANGLE(180.0f);
            }
        }

        stalkerProps.timer_F8 += g_DeltaTime;
        if (stalkerProps.timer_F8 > Q12(3.0f) &&
            !Rng_GenerateInt(0, 15)) // 1 in 16 chance.
        {
            stalkerProps.timer_F8 = Q12(0.0f);
        }

        if (stalkerProps.flags & StalkerFlag_WarpRotation)
        {
            stalker->rotation.vy = stalkerProps.targetHeadingAngle;
        }
        else
        {
            angleDeltaToTargetHeading = Math_AngleNormalizeSigned(stalkerProps.targetHeadingAngle - stalker->rotation.vy);
            if (((g_DeltaTime >> 3) + 1) < ABS(angleDeltaToTargetHeading))
            {
                if (angleDeltaToTargetHeading > Q12_ANGLE(0.0f))
                {
                    stalker->rotation.vy += Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(90.0f));
                }
                else
                {
                    stalker->rotation.vy -= Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(90.0f));
                }
            }
        }
    }

    if (stalker->model.anim.status == ANIM_STATUS(StalkerAnim_30, true) &&
        (radiusMax * 2) < distToPlayer)
    {
        temp  = Math_Vector2MagCalc(Q12_TO_Q6(stalker->position.vx - stalkerProps.targetPositionX),
                                    Q12_TO_Q6(stalker->position.vz - stalkerProps.targetPositionZ));
        temp2 = radiusMin >> 1;
        if (Q6_TO_Q12(temp) < temp2)
        {
            stalker->model.controlState = StalkerControl_4;
        }
    }
#endif
}

void Stalker_Control_4(s_SubCharacter* stalker)
{
    q3_12  angle;
    q19_12 offsetX;
    q19_12 offsetZ;
    bool   condCombo;
    q19_12 distToPlayer;
    q19_12 distToTarget;
    q19_12 dist1;
    q19_12 deltaX;
    q19_12 deltaZ;

    distToPlayer = Math_Vector2MagCalcSafeQ6(g_SysWork.playerWork.player.position.vx - stalker->position.vx,
                                       g_SysWork.playerWork.player.position.vz - stalker->position.vz);

    if (!(g_SysWork.field_2388.field_154.effectsInfo.flags.field_0 & (SpecialEnvEventFlags_DarkEnvironment | SpecialEnvEventFlags_FlashlightAllowed)))
    {
        dist1     = Q12(5.5f);
        condCombo = func_8006FD90(stalker, 0, Q12(7.5f), Q12(7.5f));
    }
    else if ((g_SysWork.field_2388.field_154.effectsInfo.flags.field_0 & (SpecialEnvEventFlags_DarkEnvironment | SpecialEnvEventFlags_FlashlightAllowed)) == SpecialEnvEventFlags_FlashlightAllowed)
    {
#if defined(MAP1_S00) || defined(MAP1_S01)
        dist1     = Q12(4.5f);
        condCombo = func_8006FD90(stalker, 0, Q12(12.f), Q12(0.0f));
#else
        dist1     = Q12(6.0f);
        condCombo = func_8006FD90(stalker, 0, Q12(16.0f), Q12(0.0f));
#endif
    }
    else
    {
#if defined(MAP1_S00) || defined(MAP1_S01)
        dist1 = Q12(1.0f);
#else
        dist1 = Q12(1.2f);
#endif
        condCombo = func_8006FD90(stalker, 1, Q12(0.4f), Q12(0.4f));
    }

    condCombo |= func_80070360(stalker, distToPlayer, Q12(0.8f));

    if (!(stalkerProps.flags & StalkerFlag_Moving))
    {
        if (stalkerProps.flags & StalkerFlag_11)
        {
            Chara_MoveSpeedUpdate3(stalker, Q12(0.5f), Q12(0.35f));
        }
        else
        {
            Chara_MoveSpeedUpdate3(stalker, Q12(0.5f), Q12(0.7f));
        }
    }

    offsetX      = stalker->position.vx - stalkerProps.targetPositionX;
    offsetZ      = stalker->position.vz - stalkerProps.targetPositionZ;
    distToTarget = MAX(ABS(offsetX), ABS(offsetZ));

    if (!(*(s32*)&stalkerProps.flags & 0x18))
    {
        if (!(Rng_GenerateInt(0, 511) && distToTarget <= Q12(4.0f)) || // 1 in 512 chance.
            (!Rng_GenerateInt(0, 127) &&                               // 1 in 128 chance.
             Rng_GenerateUInt(0, Q12_CLAMPED(1.0f)) < Q12_DIV(distToTarget, Q12(4.0f))))
        {
            if (stalkerProps.flags & StalkerFlag_11)
            {
                deltaX = Rng_GenerateInt(Q12(-0.5f), Q12(0.5f) - 1) - offsetX;
                deltaZ = Rng_GenerateInt(Q12(-0.5f), Q12(0.5f) - 1) - offsetZ;
                stalkerProps.targetHeadingAngle = ratan2(deltaX, deltaZ);
            }
            else
            {
                deltaX = Rng_GenerateInt(Q12(-4.0f), Q12(4.0f) - 1) - offsetX;
                deltaZ = Rng_GenerateInt(Q12(-4.0f), Q12(4.0f) - 1) - offsetZ;
                stalkerProps.targetHeadingAngle = ratan2(deltaX, deltaZ);
            }

            stalkerProps.flags |= StalkerFlag_3;
        }
    }

    if (!(stalkerProps.flags & StalkerFlag_4) && Los_CharaHitCheck(stalker, Q12(1.0f), stalker->rotation.vy))
    {
        stalkerProps.targetHeadingAngle = func_8006F99C(stalker, Q12(3.5f), stalker->rotation.vy);
        if (stalkerProps.targetHeadingAngle == StalkerFlag_WarpRotation)
        {
            stalkerProps.targetHeadingAngle = stalker->rotation.vy + Q12_ANGLE(180.0f);
        }

        stalkerProps.flags |= StalkerFlag_4;
    }

    // Smoothly rotate toward target direction
    angle = Math_AngleNormalizeSigned(stalkerProps.targetHeadingAngle - stalker->rotation.vy);

    if (TIMESTEP_ANGLE(1, 4) < ABS(angle))
    {
        if (angle > Q12_ANGLE(0.0f))
        {
            stalker->rotation.vy += Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(45.0f));
        }
        else
        {
            stalker->rotation.vy -= Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(45.0f));
        }
    }
    else
    {
        stalkerProps.flags &= ~(StalkerFlag_3 | StalkerFlag_4);
    }

    // Handle aggressive/chase mode.
    if (stalkerProps.flags & StalkerFlag_11)
    {
        if (!Rng_GenerateInt(0, 1023)) // 1 in 1024 chance.
        {
            stalkerProps.flags &= ~StalkerFlag_3;
        }

        if (!Rng_GenerateInt(0, 1023)) // 1 in 1024 chance.
        {
            stalkerProps.flags &= ~StalkerFlag_4;
        }

        if (distToPlayer < Q12(2.0f) && distToPlayer < dist1 && !Los_NpcToPlayerHitCheck(stalker, &g_SysWork.playerWork.player))
        {
            stalkerProps.timer_F8  = Q12(5.0f);
            stalkerProps.flags |= StalkerFlag_0;
            stalkerProps.flags &= ~StalkerFlag_11;
        }
    }

    if ((stalkerProps.flags & StalkerFlag_0))
    {
        if ((stalkerProps.flags & StalkerFlag_10) || condCombo)
        {
            if (condCombo)
            {
                stalkerProps.targetPositionX = g_SysWork.playerWork.player.position.vx;
                stalkerProps.targetPositionZ = g_SysWork.playerWork.player.position.vz;
                stalkerProps.timer_116          = Q12(0.0f);
            }

            if (!(stalkerProps.flags & StalkerFlag_10))
            {
                stalkerProps.flags |= StalkerFlag_10;
                sharedFunc_800D7E04_0_s00(stalker, Sfx_Unk1363);
            }

            if (((!Rng_GenerateInt(0, 15) && stalkerProps.timer_F8 > Q12(1.0f)) && // 1 in 16 chance.
                 (stalkerProps.flags & StalkerFlag_1)) ||
                (!Rng_GenerateInt(0, 15) && stalkerProps.timer_F8 > Q12(2.5f)) ||  // 1 in 16 chance.
                (!Rng_GenerateInt(0, 31) && ((dist1 >> 1) < distToPlayer)))        // 1 in 32 chance.
            {
                if (stalkerProps.flags & StalkerFlag_1)
                {
                    stalker->model.controlState = StalkerControl_2;
                }
                else
                {
                    stalker->model.controlState = StalkerControl_3;
                }

                stalkerProps.flags &= ~(StalkerFlag_3 | StalkerFlag_4);
            }
            else
            {
                stalkerProps.timer_F8 += g_DeltaTime;
            }
        }
    }

    if (!condCombo)
    {
        if (stalkerProps.flags & StalkerFlag_10)
        {
            stalkerProps.timer_116 += g_DeltaTime;

            if (!Rng_GenerateInt(0, 4095) || stalkerProps.timer_116 > Q12(1.2f)) // 1 in 4096 chance.
            {
                stalkerProps.timer_116 = Q12(0.0f);
                stalkerProps.flags &= ~StalkerFlag_10;
            }
        }
    }
}

void Stalker_Control_5(s_SubCharacter* stalker)
{
    #define DIST_TO_PLAYER_MAX Q12(0.9f)
    #define MOVE_SPEED_MAX     Q12(0.7f)

    q3_12  angleDeltaToPlayer;
    q19_12 distToPlayer;
    q19_12 angle1;
    s32    i;

    if (ANIM_STATUS_IDX_GET(stalker->model.anim.status) == StalkerAnim_30)
    {
        stalker->model.controlState = StalkerControl_4;
        g_SysWork.charaGroupFlags[3] &= ~CharaGroupFlag_1;
        return;
    }

    if (ANIM_TIME_RANGE_CHECK(stalker->model.anim.time, 62, 64))
    {
        stalker->moveSpeed = Q12(0.0f);
    }
    else if (ANIM_TIME_RANGE_CHECK(stalker->model.anim.time, 50, 61))
    {
        distToPlayer = Math_Vector2MagCalcSafeQ6(g_SysWork.playerWork.player.position.vx - stalker->position.vx,
                                           g_SysWork.playerWork.player.position.vz - stalker->position.vz);

        for (i = 0; i < 6; i++)
        {
            angleDeltaToPlayer = Math_AngleNormalizeSigned(Math_AngleBetweenPositionsGet(stalker->position, g_SysWork.playerWork.player.position) -
                                               stalker->rotation.vy);
            angle1             = TIMESTEP_ANGLE(3, 3); // @hack `(g_DeltaTime / 3) >> 3` should be same as `g_DeltaTime / 24`, but that doesn't match?
            if ((angleDeltaToPlayer >= Q12_ANGLE(0.0f) && angle1 <  angleDeltaToPlayer) ||
                (angleDeltaToPlayer <  Q12_ANGLE(0.0f) && angle1 < -angleDeltaToPlayer))
            {
                if (angleDeltaToPlayer > Q12_ANGLE(0.0f))
                {
                    stalker->rotation.vy = Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.0833f) + stalker->rotation.vy;
                }
                else
                {
                    stalker->rotation.vy = stalker->rotation.vy - Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.0833f);
                }
            }
        }

        if (!(stalkerProps.flags & StalkerFlag_Moving))
        {
            if (distToPlayer > DIST_TO_PLAYER_MAX)
            {
                Chara_MoveSpeedUpdate3(stalker, Q12(1.0f), MOVE_SPEED_MAX);
                return;
            }

            if (distToPlayer < MOVE_SPEED_MAX)
            {
                Chara_MoveSpeedUpdate3(stalker, Q12(1.0f), -MOVE_SPEED_MAX);
                return;
            }

            Chara_MoveSpeedUpdate(stalker, Q12(6.0f));
        }
    }
    else
    {
        Chara_MoveSpeedUpdate(stalker, Q12(6.0f));
    }
}

void Stalker_Control_6(s_SubCharacter* stalker)
{
    VECTOR3 vec0;
    q3_12   angleDeltaToPlayer;
    q3_12   moveSpeed;
    q19_12  distToPlayer;
    s32     i;
    u32     animStatus;

    stalker->collision.state = CharaCollisionState_Npc;
    Chara_MoveSpeedUpdate(stalker, Q12(1.5f));

    distToPlayer = Math_Vector2MagCalcSafeQ6(g_SysWork.playerWork.player.position.vx - stalker->position.vx,
                               g_SysWork.playerWork.player.position.vz - stalker->position.vz);

    moveSpeed = Q12(0.0f);
    angleDeltaToPlayer = Math_AngleNormalizeSigned(ratan2(g_SysWork.playerWork.player.position.vx - stalker->position.vx,
                                              g_SysWork.playerWork.player.position.vz - stalker->position.vz) -
                                       stalker->rotation.vy);

    if (ABS(angleDeltaToPlayer) < Q12_ANGLE(45.0f))
    {
        if (stalker->model.anim.status == ANIM_STATUS(StalkerAnim_34, true) && distToPlayer > Q12(0.53f))
        {
            moveSpeed = Q12_MULT_PRECISE(g_DeltaTime, Q12(1.0f));
        }
        else if (stalker->model.anim.status == ANIM_STATUS(StalkerAnim_35, true) && distToPlayer > Q12(0.38f))
        {
            moveSpeed = Q12_MULT_PRECISE(g_DeltaTime, Q12(1.0f));
        }
    }

    for (i = 0; i < 3; i++)
    {
        if (TIMESTEP_ANGLE(1, 4) < ABS(angleDeltaToPlayer))
        {
            if (angleDeltaToPlayer > Q12_ANGLE(0.0f))
            {
                stalker->rotation.vy += Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(45.0f));
            }
            else
            {
                stalker->rotation.vy -= Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(45.0f));
            }
        }

        angleDeltaToPlayer = Math_AngleNormalizeSigned(ratan2(g_SysWork.playerWork.player.position.vx - stalker->position.vx,
                                                  g_SysWork.playerWork.player.position.vz - stalker->position.vz) -
                                           stalker->rotation.vy);
    }

    if (stalkerProps.flags & StalkerFlag_5)
    {
        if (stalker->model.anim.status == ANIM_STATUS(StalkerAnim_9, true))
        {
            if (distToPlayer < Q12(0.52f) || distToPlayer > Q12(0.54f))
            {
                moveSpeed = CLAMP_HIGH(Q12_MULT_PRECISE(g_DeltaTime, Q12(1.0f)), distToPlayer - Q12(0.53f));
            }

            stalkerProps.relAnimTime_104 = stalker->model.anim.time - Q12(83.0f);
        }
        else if (stalker->model.anim.status == ANIM_STATUS(StalkerAnim_8, true))
        {
            if (distToPlayer < Q12(0.37f) || distToPlayer > Q12(0.39f))
            {
                moveSpeed = CLAMP_HIGH(Q12_MULT_PRECISE(g_DeltaTime, Q12(1.0f)), distToPlayer - Q12(0.38f));
            }

            stalkerProps.relAnimTime_104 = stalker->model.anim.time - Q12(70.0f);
        }
        else
        {
            stalkerProps.relAnimTime_104 = Q12(0.0f);
        }

        stalker->rotation.vy = Math_AngleBetweenPositionsGet(stalker->position, g_SysWork.playerWork.player.position);

        if (g_SysWork.playerWork.player.attackReceived == NO_VALUE)
        {
            g_SysWork.charaGroupFlags[3] &= ~CharaGroupFlag_0;
            animStatus                    = stalker->model.anim.status;
            stalker->model.controlState   = StalkerControl_9;
            stalker->model.anim.status    = ANIM_STATUS(StalkerAnim_10, false);
            stalkerProps.keyframeIdx_FC   = animStatus;
            stalker->collision.state      = CharaCollisionState_Ignore;
            stalker->collision.state      = CharaCollisionState_Npc;

            stalkerProps.flags |= StalkerFlag_6;
            stalkerProps.flags &= ~StalkerFlag_5;
        }
        else
        {
            stalkerProps.timer_F8 += g_DeltaTime;
            if (stalkerProps.timer_F8 > Q12(1.5f))
            {
                g_SysWork.playerWork.player.damage.amount += (FP_TO(D_800AD4C8[50].field_4, Q12_SHIFT) * Rng_GenerateUInt(85, 116)) / 100;
                stalkerProps.timer_F8              = Rng_Rand16() % Q12(0.5f);
            }
        }
    }
    else if (ANIM_TIME_RANGE_CHECK(stalker->model.anim.time, 472, 476))
    {
        vec0.vx = stalker->position.vx;
        vec0.vy = stalker->position.vy + stalker->collision.box.field_8;
        vec0.vz = stalker->position.vz;

        // TODO: What's weapon attack 49?
        if (func_8008A0E4(1, WEAPON_ATTACK(EquippedWeaponId_Unk49, AttackInputType_Tap), stalker, &vec0, &g_SysWork.playerWork, stalker->rotation.vy, Q12_ANGLE(90.0f)) != NO_VALUE)
        {
            stalkerProps.flags |= StalkerFlag_5;
            g_SysWork.charaGroupFlags[3] &= ~CharaGroupFlag_1;

            sharedFunc_800D7E04_0_s00(stalker, Sfx_Unk1366);

            stalkerProps.timer_F8 = Q12(1.0f);
        }
    }
    else if (stalker->model.anim.status == ANIM_STATUS(StalkerAnim_9, true) ||
             stalker->model.anim.status == ANIM_STATUS(StalkerAnim_8, true) ||
             distToPlayer > Q12(2.0f))
    {
        g_SysWork.charaGroupFlags[3] &= ~(CharaGroupFlag_0 | CharaGroupFlag_1);
        stalker->model.controlState = StalkerControl_9;

        if (stalker->model.anim.status == ANIM_STATUS(StalkerAnim_34, true) ||
            stalker->model.anim.status == ANIM_STATUS(StalkerAnim_9,  false))
        {
            stalkerProps.keyframeIdx_FC = ANIM_STATUS(StalkerAnim_9, true);
        }
        else if (stalker->model.anim.status == ANIM_STATUS(StalkerAnim_35, true) ||
                 stalker->model.anim.status == ANIM_STATUS(StalkerAnim_8,  false))
        {
            stalkerProps.keyframeIdx_FC = ANIM_STATUS(StalkerAnim_8, true);
        }
        else
        {
            stalkerProps.keyframeIdx_FC = stalker->model.anim.status;
        }

        stalkerProps.flags        |= StalkerFlag_6;
        stalker->model.anim.status = ANIM_STATUS(StalkerAnim_36, false);
        stalker->collision.state   = CharaCollisionState_Npc;
    }

    if (g_DeltaTime != Q12(0.0f))
    {
        stalkerProps.offset_EC = Q12_MULT(moveSpeed, Math_Sin(stalker->rotation.vy));
        stalkerProps.offset_EE = Q12_MULT(moveSpeed, Math_Cos(stalker->rotation.vy));
    }
}

void Stalker_Control_7(s_SubCharacter* stalker)
{
    u16 flags;

    if (!(stalkerProps.flags & StalkerFlag_Moving))
    {
        Chara_MoveSpeedUpdate(stalker, Q12(1.5f));
    }

    if (stalker->model.anim.status == ANIM_STATUS(StalkerAnim_27, true) &&
        !Rng_GenerateInt(0, 3)) // 1 in 4 chance.
    {
        flags = stalkerProps.flags;
        if (!(flags & StalkerFlag_Moving))
        {
            stalker->model.controlState  = (flags & StalkerFlag_1) ? StalkerControl_2 : StalkerControl_3;
            stalker->model.anim.status = ANIM_STATUS(StalkerAnim_30, false);
            stalkerProps.keyframeIdx_FC      = ANIM_STATUS(StalkerAnim_27, true);
            stalkerProps.relKeyframeIdx_FE   = ANIM_TIME_REL_KEYFRAME_IDX_GET(stalker->model.anim.time, 427);
        }
    }
}

void Stalker_Control_8(s_SubCharacter* stalker)
{
#if defined(MAP1_S00) || defined(MAP1_S01)
    #define BASE_DIST_MAX Q12(6.0f)
#else
    #define BASE_DIST_MAX Q12(8.0f)
#endif

    u32    activeKeyframeIdx;
    s32    animDivTmp;
    s32    animDiv;
    s32    step;
    q19_12 animMult;
    s32    frameIdx;
    s32    flags;
    s32    cond;
    q19_12 duration;

    stalker->flags &= ~CharaFlag_Unk2;
    if (!(stalkerProps.flags & StalkerFlag_Moving))
    {
        Chara_MoveSpeedUpdate(stalker, Q12(1.5f));
    }

    Chara_MoveSpeedUpdate(stalker, Q12(1.5f));

    activeKeyframeIdx = FP_FROM(stalker->model.anim.time, Q12_SHIFT);
    animDivTmp = 0;
    animMult = Q12(0.0f);

    if (activeKeyframeIdx >= 110 && activeKeyframeIdx < 117)
    {
        animMult = Q12(0.3f);
        animDivTmp = 7;
    }
    else if (activeKeyframeIdx >= 117 && activeKeyframeIdx < 121)
    {
        animMult = Q12(0.3f);
        animDivTmp = 4;
    }
    else if (activeKeyframeIdx >= 121 && activeKeyframeIdx < 124)
    {
        animMult = Q12(0.1f);
        animDivTmp = 4;
    }

    if (animMult != Q12(0.0f))
    {
        animDiv  = FP_TO(animDivTmp, Q12_SHIFT);
        duration = STALKER_ANIM_INFOS[stalker->model.anim.status].duration.constant;
        step     = (Q12_MULT_PRECISE(duration, g_DeltaTime) * animMult) / animDiv;

        stalkerProps.offset_EC = Q12_MULT(step, Math_Sin(stalker->rotation.vy));
        stalkerProps.offset_EE = Q12_MULT(step, Math_Cos(stalker->rotation.vy));
    }

    if (stalkerProps.flags & StalkerFlag_7)
    {
        frameIdx = FP_FROM(stalker->model.anim.time, Q12_SHIFT);
        if ((frameIdx >= 121 && frameIdx < 129) ||
            (frameIdx >= 149 && frameIdx < 158) ||
            (frameIdx >= 171 && frameIdx < 176))
        {
            stalkerProps.flags &= ~StalkerFlag_7;
        }
    }

    if (stalker->model.anim.status == ANIM_STATUS(StalkerAnim_27, true) && !Rng_GenerateInt(0, 3)) // 1 in 4 chance.
    {
        if (stalkerProps.flags & StalkerFlag_1)
        {
            stalker->model.controlState = StalkerControl_2;
        }
        else
        {
            stalker->model.controlState = StalkerControl_3;
        }

        stalker->model.anim.status = ANIM_STATUS(StalkerAnim_30, false);
        stalkerProps.keyframeIdx_FC      = ANIM_STATUS(StalkerAnim_27, true);
        stalkerProps.relKeyframeIdx_FE   = ANIM_TIME_REL_KEYFRAME_IDX_GET(stalker->model.anim.time, 427);
    }

    // @hack `animDiv` has to be used for certain stuff for a match, weird.
    animDiv = g_SysWork.field_2388.field_154.effectsInfo.flags.field_0;
    flags = animDiv & (SpecialEnvEventFlags_DarkEnvironment | SpecialEnvEventFlags_FlashlightAllowed);
    if (flags == SpecialEnvEventFlags_None)
    {
        cond = func_8006FD90(stalker, 0, Q12(7.5f), Q12(0.0f));
    }
    else if (flags == SpecialEnvEventFlags_FlashlightAllowed)
    {
        cond = func_8006FD90(stalker, 0, BASE_DIST_MAX, Q12(0.0f));
    }
    else
    {
        cond = func_8006FD90(stalker, 1, Q12(0.4f), Q12(0.0f));
    }

    if (cond || func_80070360(stalker, Q12(0.0f), Q12(1.0f)))
    {
        stalkerProps.targetPositionX = g_SysWork.playerWork.player.position.vx;
        stalkerProps.targetPositionZ = g_SysWork.playerWork.player.position.vz;
    }

    #undef BASE_DIST_MAX
}

void Stalker_Control_9(s_SubCharacter* stalker)
{
    #define FRICTION_PER_FRAME 1.5f // TODO: Or is it per tick?

    q19_12 dist;
    s32    distDiv;
    q19_12 angleOffset;

    if (stalker->health < sharedData_800E3A24_0_s00)
    {
        stalker->flags |= CharaFlag_Unk2;
    }

    stalkerProps.flags |= StalkerFlag_6;
    if (!(stalkerProps.flags & StalkerFlag_Moving))
    {
        Chara_MoveSpeedUpdate(stalker, Q12(FRICTION_PER_FRAME));
    }

    distDiv = 0;
    angleOffset = Q12_ANGLE(0.0f);
    dist = Q12(0.0f);

    if (stalker->model.anim.status == ANIM_STATUS(StalkerAnim_24, false))
    {
        dist = Q12(0.15f);
        distDiv = 1;
        angleOffset = Q12_ANGLE(180.0f);
    }
    else if (ANIM_TIME_RANGE_CHECK(stalker->model.anim.time, 395, 402))
    {
        dist = Q12(1.0f);
        distDiv = 8 - stalkerProps.relKeyframeIdx_FE;
        angleOffset = Q12_ANGLE(180.0f);
    }
    else if (ANIM_TIME_RANGE_CHECK(stalker->model.anim.time, 407, 412))
    {
        dist = Q12(0.5f);
        distDiv = 5;
        angleOffset = Q12_ANGLE(0.0f);
    }
    else if (ANIM_TIME_RANGE_CHECK(stalker->model.anim.time, 0, 3))
    {
        dist = Q12(0.1f);
        distDiv = 4;
        angleOffset = Q12_ANGLE(180.0f);
    }
    else if (ANIM_TIME_RANGE_CHECK(stalker->model.anim.time, 9, 17))
    {
        dist = Q12(0.3f);
        distDiv = 9;
        angleOffset = Q12_ANGLE(180.0f);
    }
    else if (ANIM_TIME_RANGE_CHECK(stalker->model.anim.time, 4, 6))
    {
        dist = Q12(0.4f);
        distDiv = 3;
        angleOffset = Q12_ANGLE(180.0f);
    }
    else if (ANIM_TIME_RANGE_CHECK(stalker->model.anim.time, 20, 23))
    {
        dist = Q12(0.1f);
        distDiv = 5;
        angleOffset = Q12_ANGLE(0.0f);
    }
    else if (ANIM_TIME_RANGE_CHECK(stalker->model.anim.time, 27, 32))
    {
        dist = Q12(0.3f);
        distDiv = 6;
        angleOffset = Q12_ANGLE(0.0f);
    }
    else if (stalker->model.anim.status == ANIM_STATUS(StalkerAnim_45, true) &&
             ANIM_TIME_RANGE_CHECK(stalker->model.anim.time, 159, 167))
    {
        dist = Q12(0.3f);
        distDiv = 8;
        angleOffset = Q12_ANGLE(180.0f);
    }
    else if (stalker->model.anim.status == ANIM_STATUS(StalkerAnim_10, true) &&
             ANIM_TIME_RANGE_CHECK(stalker->model.anim.time, 96, 100))
    {
        dist = Q12(0.2f);
        distDiv = 5;
        angleOffset = Q12_ANGLE(180.0f);
    }

    if (dist != Q12(0.0f))
    {
        dist = (s32)(dist * (u32)Q12_MULT_PRECISE(STALKER_ANIM_INFOS[stalker->model.anim.status].duration.constant, g_DeltaTime)) /
               FP_TO(distDiv, Q12_SHIFT);

        stalkerProps.offset_EC = Q12_MULT(dist, Math_Sin(stalker->rotation.vy + angleOffset));
        stalkerProps.offset_EE = Q12_MULT(dist, Math_Cos(stalker->rotation.vy + angleOffset));
    }

    if (stalker->model.anim.status == ANIM_STATUS(StalkerAnim_26, true) ||
        stalker->model.anim.status == ANIM_STATUS(StalkerAnim_17, true) ||
        stalker->model.anim.status == ANIM_STATUS(StalkerAnim_18, true))
    {
        if (stalker->health == Q12(0.0f) && stalker->moveSpeed == Q12(0.0f))
        {
            if (stalker->model.anim.status == ANIM_STATUS(StalkerAnim_26, true))
            {
                stalker->model.anim.status = ANIM_STATUS(StalkerAnim_21, false);
            }
            if (stalker->model.anim.status == ANIM_STATUS(StalkerAnim_17, true))
            {
                stalker->model.anim.status = ANIM_STATUS(StalkerAnim_19, false);
            }
            if (stalker->model.anim.status == ANIM_STATUS(StalkerAnim_18, true))
            {
                stalker->model.anim.status = ANIM_STATUS(StalkerAnim_20, false);
            }

            stalker->model.controlState = StalkerControl_10;
            Savegame_EnemyStateUpdate(stalker);
        }

        if (stalker->health > sharedData_800E3A24_0_s00 &&
            !Rng_GenerateInt(0, 7) && // 1 in 8 chance.
            !(stalkerProps.flags & StalkerFlag_Moving))
        {
            if (stalker->model.anim.status == ANIM_STATUS(StalkerAnim_26, true))
            {
                stalker->model.anim.status = ANIM_STATUS(StalkerAnim_11, false);
            }
            if (stalker->model.anim.status == ANIM_STATUS(StalkerAnim_17, true))
            {
                stalker->model.anim.status = ANIM_STATUS(StalkerAnim_12, false);
            }
            if (stalker->model.anim.status == ANIM_STATUS(StalkerAnim_18, true))
            {
                stalker->model.anim.status = ANIM_STATUS(StalkerAnim_13, false);
            }

            stalker->model.controlState = StalkerControl_8;
            stalkerProps.flags |= StalkerFlag_7;
            stalkerProps.flags &= ~StalkerFlag_6;
        }
    }
}

void Stalker_Control_10(s_SubCharacter* stalker)
{
    s32 newAnimStatus;

    if (!(stalkerProps.flags & StalkerFlag_Moving))
    {
        Chara_MoveSpeedUpdate(stalker, Q12(1.5f));
    }

    if (stalker->health == Q12(0.0f))
    {
        stalker->flags &= ~CharaFlag_Unk2;

        if (g_SysWork.targetNpcIdx != Chara_NpcIdxGet(stalker))
        {
            stalker->health          = NO_VALUE;
            stalker->collision.state = CharaCollisionState_Ignore;
        }
    }

    // @hack Odd `stalkerProps.flags` aaccess.
    if (stalker->moveSpeed == Q12(0.0f) && !(*(s32*)&stalkerProps.flags & (StalkerFlag_Falling |StalkerFlag_9)))
    {
        newAnimStatus = (stalker->model.anim.status == ANIM_STATUS(StalkerAnim_31, true)) ? ANIM_STATUS(StalkerAnim_Still, true) :
                                                                                            ANIM_STATUS(StalkerAnim_Still, false);

        if (stalker->model.anim.status == ANIM_STATUS(StalkerAnim_32, true))
        {
            newAnimStatus = ANIM_STATUS(StalkerAnim_1, false);
        }

        if (stalker->model.anim.status == ANIM_STATUS(StalkerAnim_33, true))
        {
            newAnimStatus = ANIM_STATUS(StalkerAnim_6, false);
        }

        if (newAnimStatus != ANIM_STATUS(StalkerAnim_Still, false))
        {
#if !defined(MAP5_S02) && !defined(MAP7_S02)
            func_800622B8(3, stalker, newAnimStatus, 3);
#endif
            stalkerProps.flags |= StalkerFlag_9;
        }
    }

#if defined(MAP5_S02) || defined(MAP7_S02)
    if (stalkerProps.flags & StalkerFlag_9)
    {
        stalker->timer_C6 += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.25f);
        if (stalker->timer_C6 > Q12(1.0f))
        {
            stalker->model.charaId = Chara_None;
        }
    }
#endif
}

void Stalker_Control_11(s_SubCharacter* stalker) {}

void Stalker_Control_12(s_SubCharacter* stalker)
{
#if defined(MAP1_S00) || defined(MAP1_S01)
    #define BASE_DIST_MAX Q12(12.0f)
#else
    #define BASE_DIST_MAX Q12(16.0f)
#endif

    q19_12 distToPlayer;
    q3_12  angleDeltaToPlayer;
    s32    cond; // Not `bool`?

    distToPlayer       = Math_Vector2MagCalcSafeQ6(g_SysWork.playerWork.player.position.vx - stalker->position.vx,
                                             g_SysWork.playerWork.player.position.vz - stalker->position.vz);
    angleDeltaToPlayer = Math_AngleNormalizeSigned(Math_AngleBetweenPositionsGet(stalker->position, g_SysWork.playerWork.player.position) -
                                       stalker->rotation.vy);

    if (!(g_SysWork.field_2388.field_154.effectsInfo.flags.field_0 & (SpecialEnvEventFlags_DarkEnvironment | SpecialEnvEventFlags_FlashlightAllowed)))
    {
        cond = func_8006FD90(stalker, 0, Q12(7.5f), Q12(7.5f));
    }
    else if ((g_SysWork.field_2388.field_154.effectsInfo.flags.field_0 & (SpecialEnvEventFlags_DarkEnvironment | SpecialEnvEventFlags_FlashlightAllowed)) == SpecialEnvEventFlags_FlashlightAllowed)
    {
        cond = func_8006FD90(stalker, 0, BASE_DIST_MAX, Q12(0.0f));
    }
    else
    {
        cond = func_8006FD90(stalker, 1, Q12(0.4f), Q12(0.8f));
    }

    cond |= func_80070360(stalker, distToPlayer, Q12(1.0f));
    if (!cond)
    {
        return;
    }

    stalkerProps.targetPositionX = g_SysWork.playerWork.player.position.vx;
    stalkerProps.targetPositionZ = g_SysWork.playerWork.player.position.vz;

    if (distToPlayer < Q12(1.2f) && angleDeltaToPlayer < Q12_ANGLE(180.0f))
    {
        if (!(stalkerProps.flags & StalkerFlag_1))
        {
            stalker->model.controlState = StalkerControl_3;
        }
        else
        {
            stalker->model.controlState = StalkerControl_2;
        }

        stalker->model.anim.status = ANIM_STATUS(StalkerAnim_30, false);

        stalkerProps.keyframeIdx_FC    = 55;
        stalkerProps.relKeyframeIdx_FE = ANIM_TIME_REL_KEYFRAME_IDX_GET(stalker->model.anim.time, 427);
        stalkerProps.flags         |= StalkerFlag_10;

        sharedFunc_800D7E04_0_s00(stalker, Sfx_Unk1363);
    }

    #undef BASE_DIST_MAX
}

void Stalker_Control_13(s_SubCharacter* stalker)
{
    if (stalker->model.stateStep == 1)
    {
        stalker->model.stateStep++;
        stalker->model.anim.status = ANIM_STATUS(StalkerAnim_15, false);
    }

    if (!(stalkerProps.flags & StalkerFlag_14) &&
        stalker->model.anim.time > Q12(211.0f))
    {
        sharedFunc_800D7E04_0_s00(stalker, Sfx_Unk1365);

        stalker->health     = NO_VALUE;
        stalkerProps.flags |= StalkerFlag_14;
    }

    if (stalker->model.stateStep == 3)
    {
        stalker->model.stateStep++;
        Chara_AnimSet(stalker, ANIM_STATUS(StalkerAnim_47, true), 294);

        if (stalker->health != NO_VALUE)
        {
            func_8005F6B0(&g_SysWork.npcs[1], &Q12_VECTOR3(140.39f, -0.55f, 22.76f), 3, 3);

            stalker->health     = NO_VALUE;
            stalkerProps.flags |= StalkerFlag_14;
        }
    }
}

void sharedFunc_800D67FC_0_s00(s_SubCharacter* stalker)
{
    s_CollisionResult* collResults[7];
    q3_12              newHeadingAngle;
    q19_12             prevMoveSpeed;
    u16                newFlags;

    stalker->fallSpeed   = stalker->fallSpeed + g_GravitySpeed;
    newHeadingAngle      = Math_AngleNormalizeSigned(stalker->rotation.vy);
    stalker->rotation.vy = newHeadingAngle;

    if (stalkerProps.flags & StalkerFlag_Moving)
    {
        Chara_MoveSpeedUpdate(stalker, Q12(1.5f));
        if (stalker->moveSpeed == Q12(0.0f))
        {
            stalkerProps.flags &= ~StalkerFlag_Moving;
        }
    }
    else
    {
        stalker->headingAngle = newHeadingAngle;
    }

    prevMoveSpeed = stalker->moveSpeed;
    if (stalkerProps.flags & StalkerFlag_WarpRotation)
    {
        if (stalker->model.controlState == StalkerControl_2)
        {
            stalker->moveSpeed *= 8;
        }
        else
        {
            stalker->moveSpeed *= 2;
        }
    }

    if (g_DeltaTime != Q12(0.0f))
    {
        Chara_MovementUpdate1(stalker, &collResults, stalkerProps.offset_EC, stalkerProps.offset_EE);
    }

    stalkerProps.offset_EE = Q12(0.0f);
    stalkerProps.offset_EC = Q12(0.0f);
    stalker->moveSpeed  = prevMoveSpeed;

    // Update falling flag.
    if (stalker->fallSpeed != Q12(0.0f))
    {
        newFlags = stalkerProps.flags | StalkerFlag_Falling;
    }
    else
    {
        newFlags = stalkerProps.flags & ~StalkerFlag_Falling;
    }
    stalkerProps.flags = newFlags;
}

void sharedFunc_800D6970_0_s00(s_SubCharacter* stalker, s_AnmHeader* animHdr, GsCOORDINATE2* coord)
{
    typedef struct
    {
        MATRIX  field_0;
        VECTOR  field_20; // Q23.8, then Q10.12.
        SVECTOR field_30;
        VECTOR3 position_38;
        q3_12   angle_44;
        q3_12   angle_46;
    } s_sharedFunc_800D6970_0_s00;

    s32                          temp;
    s_AnimInfo*                  animInfo;
    s_sharedFunc_800D6970_0_s00* ptr;

    WorldGfx_CharaMeshSwap(stalker->model.charaId, MESH_SWAP_STATUS(StalkerSwappableMesh_Head, StalkerVariantMesh_2));

    switch (stalker->model.anim.status)
    {
        case ANIM_STATUS(StalkerAnim_7, false):
            STALKER_ANIM_INFOS[ANIM_STATUS(StalkerAnim_7, false)].endKeyframeIdx = stalkerProps.relKeyframeIdx_FE + 50;
            break;

        case ANIM_STATUS(StalkerAnim_7, true):
            STALKER_ANIM_INFOS[ANIM_STATUS(StalkerAnim_7, true)].startKeyframeIdx = (stalkerProps.relKeyframeIdx_FE + 50);

            if (ANIM_TIME_RANGE_CHECK(stalker->model.anim.time, 50, 58))
            {
                STALKER_ANIM_INFOS[stalker->model.anim.status].duration.constant = Q12(24.0f);
            }
#if VERSION_EQUAL_OR_NEWER(USA)
            else if (ANIM_TIME_RANGE_CHECK(stalker->model.anim.time, 59, 65))
            {
                STALKER_ANIM_INFOS[stalker->model.anim.status].duration.constant = Q12(32.0f);
            }
#else // JAP0
            else if (ANIM_TIME_RANGE_CHECK(stalker->model.anim.time, 59, 64))
            {
                STALKER_ANIM_INFOS[stalker->model.anim.status].duration.constant = Q12(36.0f);
            }
#endif
            else
            {
                STALKER_ANIM_INFOS[stalker->model.anim.status].duration.constant = Q12_MULT_PRECISE(MAX(stalker->moveSpeed, Q12(0.1f)), Q12(30.67f));
            }
            break;

        case ANIM_STATUS(StalkerAnim_37, true):
            STALKER_ANIM_INFOS[stalker->model.anim.status].duration.constant = Q12_MULT_PRECISE(MAX(stalker->moveSpeed, Q12(1.5f)), Q12(30.67f));
            break;

        case ANIM_STATUS(StalkerAnim_30, true):
            STALKER_ANIM_INFOS[stalker->model.anim.status].duration.constant = Q12_MULT_PRECISE(MAX(stalker->moveSpeed, Q12(0.1f)), Q12(30.67f));
            break;

        case ANIM_STATUS(StalkerAnim_1, false):
        case ANIM_STATUS(StalkerAnim_42, false):
            STALKER_ANIM_INFOS[stalker->model.anim.status].endKeyframeIdx = stalkerProps.relKeyframeIdx_FE;
            break;

        case ANIM_STATUS(StalkerAnim_1, true):
        case ANIM_STATUS(StalkerAnim_42, true):
            STALKER_ANIM_INFOS[stalker->model.anim.status].startKeyframeIdx = stalkerProps.relKeyframeIdx_FE;
            break;

        case ANIM_STATUS(StalkerAnim_3, false):
        case ANIM_STATUS(StalkerAnim_43, false):
            STALKER_ANIM_INFOS[stalker->model.anim.status].endKeyframeIdx = stalkerProps.relKeyframeIdx_FE + 9;
            break;

        case ANIM_STATUS(StalkerAnim_3, true):
        case ANIM_STATUS(StalkerAnim_43, true):
            STALKER_ANIM_INFOS[stalker->model.anim.status].startKeyframeIdx = stalkerProps.relKeyframeIdx_FE + 9;
            break;

        case ANIM_STATUS(StalkerAnim_4, false):
            STALKER_ANIM_INFOS[stalker->model.anim.status].endKeyframeIdx = stalkerProps.relKeyframeIdx_FE + 20;
            break;

        case ANIM_STATUS(StalkerAnim_4, true):
            STALKER_ANIM_INFOS[stalker->model.anim.status].startKeyframeIdx = stalkerProps.relKeyframeIdx_FE + 20;
            break;

        case ANIM_STATUS(StalkerAnim_5, false):
            STALKER_ANIM_INFOS[stalker->model.anim.status].endKeyframeIdx = stalkerProps.relKeyframeIdx_FE + 25;
            break;

        case ANIM_STATUS(StalkerAnim_5, true):
            STALKER_ANIM_INFOS[stalker->model.anim.status].startKeyframeIdx = stalkerProps.relKeyframeIdx_FE + 25;
            break;

        case ANIM_STATUS(StalkerAnim_24, false):
            STALKER_ANIM_INFOS[stalker->model.anim.status].endKeyframeIdx = stalkerProps.relKeyframeIdx_FE + 395;
            break;

        case ANIM_STATUS(StalkerAnim_24, true):
            STALKER_ANIM_INFOS[stalker->model.anim.status].startKeyframeIdx = stalkerProps.relKeyframeIdx_FE + 395;
            break;
    }

    Math_MatrixTransform(&stalker->position, &stalker->rotation, coord);

    animInfo = &STALKER_ANIM_INFOS[stalker->model.anim.status];
    animInfo->playbackFunc(&stalker->model, animHdr, coord, animInfo);

    ptr = PSX_SCRATCH;

#if VERSION_EQUAL_OR_NEWER(USA)
    if (ANIM_TIME_RANGE_CHECK(stalker->model.anim.time, 60, 65))
#else // JAP0
    if (ANIM_TIME_RANGE_CHECK(stalker->model.anim.time, 60, 66))
#endif
    {
        Math_SetSVectorFastSum(&ptr->field_30, -5, 33, 37);
        Vw_CoordHierarchyMatrixCompute(&coord[6], &ptr->field_0);

        gte_SetRotMatrix(&ptr->field_0);
        gte_SetTransMatrix(&ptr->field_0);
        gte_ldv0(&ptr->field_30);
        gte_rt();
        gte_stlvnl(&ptr->field_20);

        // TODO: Q conversion. From Q8 to Q12?
        ptr->field_20.vx = Q8_TO_Q12(ptr->field_20.vx);
        ptr->field_20.vy = Q8_TO_Q12(ptr->field_20.vy);
        ptr->field_20.vz = Q8_TO_Q12(ptr->field_20.vz);

        ptr->position_38.vx = stalker->position.vx;
        ptr->position_38.vy = stalker->position.vy + stalker->collision.box.field_8;
        ptr->position_38.vz = stalker->position.vz;

        // TODO: What's weapon attack 48?
        func_8008A0E4(1, WEAPON_ATTACK(EquippedWeaponId_Unk48, AttackInputType_Tap), stalker, &ptr->position_38, &g_SysWork.playerWork.player,
                      ratan2(ptr->field_20.vx - stalker->position.vx, ptr->field_20.vz - stalker->position.vz),
                      ratan2(Math_Vector2MagCalcSafeQ6(stalker->position.vx - ptr->field_20.vx,
                                                 stalker->position.vz - ptr->field_20.vz),
                             ptr->field_20.vy - (stalker->position.vy + stalker->collision.box.field_8)));
    }

    ptr->angle_44 = stalkerProps.angle_114;

    if (stalker->model.controlState == StalkerControl_2 ||
        stalker->model.controlState == StalkerControl_3)
    {
        ptr->angle_46 = Math_AngleNormalizeSigned(Math_AngleBetweenPositionsGet(stalker->position, g_SysWork.playerWork.player.position) -
                                      stalker->rotation.vy);
        temp          = Math_Vector2MagCalcSafeQ6(g_SysWork.playerWork.player.position.vx - stalker->position.vx,
                                            g_SysWork.playerWork.player.position.vz - stalker->position.vz);

        if (temp < Q12(3.0f) && ABS(ptr->angle_46) < Q12_ANGLE(45.0f))
        {
            if (g_Stalker_TargetPositionX == stalkerProps.targetPositionX &&
                g_Stalker_TargetPositionZ == stalkerProps.targetPositionZ)
            {
                if (ptr->angle_46 > Q12_ANGLE(0.0f))
                {
                    ptr->angle_44 = CLAMP_HIGH(ptr->angle_46, ptr->angle_44 + Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(45.0f)));
                }
                else
                {
                    ptr->angle_44 = MAX(ptr->angle_46, ptr->angle_44 - Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(45.0f)));
                }
            }
        }
        else
        {
            if (ptr->angle_44 > Q12_ANGLE(0.0f))
            {
                ptr->angle_44 = CLAMP_LOW(ptr->angle_44 - Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(45.0f)), Q12_ANGLE(0.0f));
            }
            else
            {
                ptr->angle_44 = MIN(ptr->angle_44 + Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(45.0f)), Q12_ANGLE(0.0f));
            }
        }
    }
    else
    {
        if (ptr->angle_44 > Q12_ANGLE(0.0f))
        {
            ptr->angle_44 = CLAMP_LOW(ptr->angle_44 - Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(45.0f)), Q12_ANGLE(0.0f));
        }
        else
        {
            ptr->angle_44 = MIN(ptr->angle_44 + Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(45.0f)), Q12_ANGLE(0.0f));
        }
    }

    Math_SetSVectorFastSum(&ptr->field_30, 0, ptr->angle_44, 0);
    Math_RotMatrixZxyNegGte(&ptr->field_30, &ptr->field_0);
    MulMatrix(&coord[3].coord, &ptr->field_0);

    stalkerProps.angle_114 = ptr->angle_44;
}

// TODO: Size of arrays may be incorrect. One big array doesn't match.
extern s_CharaKeyframe sharedData_800DDBA8_0_s00;
extern s_CharaKeyframe sharedData_800DDBBC_0_s00[9];
extern s_CharaKeyframe sharedData_800DDC70_0_s00[7];
extern s_CharaKeyframe sharedData_800DDCFC_0_s00[7];
extern s_CharaKeyframe sharedData_800DDD88_0_s00[16];
extern s_CharaKeyframe sharedData_800DDEC8_0_s00[5];
extern s_CharaKeyframe sharedData_800DDF2C_0_s00[11];
extern s_CharaKeyframe sharedData_800DE008_0_s00[11];
extern s_CharaKeyframe sharedData_800DE0E4_0_s00[13];
extern s_CharaKeyframe sharedData_800DE1E8_0_s00[11];
extern s_CharaKeyframe sharedData_800DE2C4_0_s00;
extern s_CharaKeyframe sharedData_800DE2D8_0_s00;
extern s_CharaKeyframe sharedData_800DE2EC_0_s00;
extern s_CharaKeyframe sharedData_800DE300_0_s00[16];
extern s_CharaKeyframe sharedData_800DE440_0_s00[16];
extern s_CharaKeyframe sharedData_800DE580_0_s00[42]; // Stalker anim data?
extern s_CharaKeyframe sharedData_800DE8C8_0_s00[29];
extern s_CharaKeyframe sharedData_800DEB0C_0_s00[18]; // Stalker anim data? Index is keyframe index.
extern s_CharaKeyframe sharedData_800DEC74_0_s00[3];
extern s_CharaKeyframe sharedData_800DECB0_0_s00[12];
extern s_CharaKeyframe sharedData_800DEDA0_0_s00[5];
extern s_CharaKeyframe sharedData_800DEE04_0_s00[3];
extern s_CharaKeyframe sharedData_800DEE40_0_s00[2];
extern s_CharaKeyframe sharedData_800DEE68_0_s00[9];
extern s_CharaKeyframe sharedData_800DEF1C_0_s00[30];

void Stalker_CollisionUpdate(s_SubCharacter* stalker)
{
    s32 keyframeIdx0;
    s32 keyframeIdx1;
    s32 keyframeIdx2;

    switch (stalker->model.anim.status)
    {
        case ANIM_STATUS(StalkerAnim_8, false):
        case ANIM_STATUS(StalkerAnim_9, false):
            Collision_CharaCollisionSet(stalker, &sharedData_800DE440_0_s00[15], &sharedData_800DDBA8_0_s00);
            break;

        case ANIM_STATUS(StalkerAnim_8, true):
        case ANIM_STATUS(StalkerAnim_9, true):
            Chara_CollisionSet(stalker, sharedData_800DDBA8_0_s00);
            break;

        case ANIM_STATUS(StalkerAnim_38, false):
            if (stalkerProps.keyframeIdx_FC == NO_VALUE)
            {
                Collision_CharaCollisionSet(stalker, &sharedData_800DE2C4_0_s00, &sharedData_800DDBBC_0_s00[0]);
            }
            else
            {
                Collision_CharaCollisionSet(stalker, &sharedData_800DE580_0_s00[20], &sharedData_800DDBBC_0_s00[0]);
            }
            break;

        case ANIM_STATUS(StalkerAnim_21, false):
            Collision_CharaCollisionSet(stalker, &sharedData_800DE2C4_0_s00, &sharedData_800DDBBC_0_s00[0]);
            break;

        case ANIM_STATUS(StalkerAnim_21, true):
        case ANIM_STATUS(StalkerAnim_38, true):
            keyframeIdx0 = ANIM_TIME_REL_KEYFRAME_IDX_GET(stalker->model.anim.time, 353);
            keyframeIdx1 = ANIM_TIME_REL_KEYFRAME_IDX_GET(stalker->model.anim.time, 352);
            Collision_CharaCollisionSet(stalker, &sharedData_800DDBBC_0_s00[keyframeIdx0], &sharedData_800DDBBC_0_s00[keyframeIdx1]);
            break;

        case ANIM_STATUS(StalkerAnim_31, false):
        case ANIM_STATUS(StalkerAnim_31, true):
            Chara_CollisionSet(stalker, sharedData_800DDBBC_0_s00[8]);
            break;

        case ANIM_STATUS(StalkerAnim_19, false):
            Collision_CharaCollisionSet(stalker, &sharedData_800DE2D8_0_s00, &sharedData_800DDC70_0_s00[0]);
            break;

        case ANIM_STATUS(StalkerAnim_39, false):
            if (stalkerProps.keyframeIdx_FC == NO_VALUE)
            {
                Collision_CharaCollisionSet(stalker, &sharedData_800DE2D8_0_s00, &sharedData_800DDC70_0_s00[0]);
            }
            else
            {
                Collision_CharaCollisionSet(stalker, &sharedData_800DE8C8_0_s00[stalkerProps.keyframeIdx_FC], &sharedData_800DDC70_0_s00[0]);
            }
            break;

        case ANIM_STATUS(StalkerAnim_19, true):
        case ANIM_STATUS(StalkerAnim_39, true):
            keyframeIdx0 = ANIM_TIME_REL_KEYFRAME_IDX_GET(stalker->model.anim.time, 339);
            keyframeIdx1 = ANIM_TIME_REL_KEYFRAME_IDX_GET(stalker->model.anim.time, 338);
            Collision_CharaCollisionSet(stalker, &sharedData_800DDC70_0_s00[keyframeIdx0], &sharedData_800DDC70_0_s00[keyframeIdx1]);
            break;

        case ANIM_STATUS(StalkerAnim_32, false):
        case ANIM_STATUS(StalkerAnim_32, true):
            Chara_CollisionSet(stalker, sharedData_800DDC70_0_s00[0]);
            break;

        case ANIM_STATUS(StalkerAnim_20, false):
            Collision_CharaCollisionSet(stalker, &sharedData_800DE2EC_0_s00, &sharedData_800DDCFC_0_s00[0]);
            break;

        case ANIM_STATUS(StalkerAnim_40, false):
            if (stalkerProps.keyframeIdx_FC == NO_VALUE)
            {
                Collision_CharaCollisionSet(stalker, &sharedData_800DE2EC_0_s00, &sharedData_800DDCFC_0_s00[0]);
            }
            else
            {
                Collision_CharaCollisionSet(stalker, &sharedData_800DEB0C_0_s00[stalkerProps.keyframeIdx_FC], &sharedData_800DDCFC_0_s00[0]);
            }
            break;

        case ANIM_STATUS(StalkerAnim_20, true):
        case ANIM_STATUS(StalkerAnim_40, true):
            keyframeIdx0 = ANIM_TIME_REL_KEYFRAME_IDX_GET(stalker->model.anim.time, 346);
            keyframeIdx1 = ANIM_TIME_REL_KEYFRAME_IDX_GET(stalker->model.anim.time, 345);
            Collision_CharaCollisionSet(stalker, &sharedData_800DDCFC_0_s00[keyframeIdx0], &sharedData_800DDCFC_0_s00[keyframeIdx1]);
            break;

        case ANIM_STATUS(StalkerAnim_33, false):
        case ANIM_STATUS(StalkerAnim_33, true):
            Chara_CollisionSet(stalker, sharedData_800DDCFC_0_s00[6]);
            break;

        case ANIM_STATUS(StalkerAnim_22, false):
            Chara_CollisionSet(stalker, sharedData_800DDD88_0_s00[0]);
            break;

        case ANIM_STATUS(StalkerAnim_22, true):
            keyframeIdx0 = ANIM_TIME_REL_KEYFRAME_IDX_GET(stalker->model.anim.time, 362);
            keyframeIdx1 = ANIM_TIME_REL_KEYFRAME_IDX_GET(stalker->model.anim.time, 361);
            Collision_CharaCollisionSet(stalker, &sharedData_800DDD88_0_s00[keyframeIdx0], &sharedData_800DDD88_0_s00[keyframeIdx1]);
            break;

        case ANIM_STATUS(StalkerAnim_23, false):
        case ANIM_STATUS(StalkerAnim_41, false):
            Chara_CollisionSet(stalker, sharedData_800DDEC8_0_s00[0]);
            break;

        case ANIM_STATUS(StalkerAnim_23, true):
        case ANIM_STATUS(StalkerAnim_41, true):
            keyframeIdx0 = ANIM_TIME_REL_KEYFRAME_IDX_GET(stalker->model.anim.time, 378);
            keyframeIdx1 = ANIM_TIME_REL_KEYFRAME_IDX_GET(stalker->model.anim.time, 377);
            Collision_CharaCollisionSet(stalker, &sharedData_800DDEC8_0_s00[keyframeIdx0], &sharedData_800DDEC8_0_s00[keyframeIdx1]);
            break;

        case ANIM_STATUS(StalkerAnim_36, false):
            Collision_CharaCollisionSet(stalker, &sharedData_800DDBA8_0_s00, &sharedData_800DE008_0_s00[6]);
            break;

        case ANIM_STATUS(StalkerAnim_10, false):
            Collision_CharaCollisionSet(stalker, &sharedData_800DDBA8_0_s00, &sharedData_800DE008_0_s00[0]);
            break;

        case ANIM_STATUS(StalkerAnim_10, true):
        case ANIM_STATUS(StalkerAnim_36, true):
            keyframeIdx0 = ANIM_TIME_REL_KEYFRAME_IDX_GET(stalker->model.anim.time, 96);
            keyframeIdx1 = ANIM_TIME_REL_KEYFRAME_IDX_GET(stalker->model.anim.time, 95);
            Collision_CharaCollisionSet(stalker, &sharedData_800DE008_0_s00[keyframeIdx0], &sharedData_800DE008_0_s00[keyframeIdx1]);
            break;

        case ANIM_STATUS(StalkerAnim_24, false):
            if (stalkerProps.relKeyframeIdx_FE != 0)
            {
                Collision_CharaCollisionSet(stalker,
                              &sharedData_800DDD88_0_s00[stalkerProps.keyframeIdx_FC],
                              &sharedData_800DE0E4_0_s00[stalkerProps.relKeyframeIdx_FE]);
            }
            else
            {
                Chara_CollisionSet(stalker, sharedData_800DE0E4_0_s00[0]);
            }
            break;

        case ANIM_STATUS(StalkerAnim_24, true):
            keyframeIdx0 = ANIM_TIME_REL_KEYFRAME_IDX_GET(stalker->model.anim.time, 394);
            keyframeIdx1 = ANIM_TIME_REL_KEYFRAME_IDX_GET(stalker->model.anim.time, 393);
            Collision_CharaCollisionSet(stalker, &sharedData_800DE0E4_0_s00[keyframeIdx0], &sharedData_800DE0E4_0_s00[keyframeIdx1]);
            break;

        case ANIM_STATUS(StalkerAnim_25, false):
            if (stalkerProps.relKeyframeIdx_FE == 83)
            {
                Collision_CharaCollisionSet(stalker, &sharedData_800DDF2C_0_s00[0], &sharedData_800DE1E8_0_s00[1]);
            }
            else
            {
                keyframeIdx0 = stalkerProps.keyframeIdx_FC;
                if (stalkerProps.relKeyframeIdx_FE == 23)
                {
                    Collision_CharaCollisionSet(stalker, &sharedData_800DE580_0_s00[keyframeIdx0 + 20], &sharedData_800DE1E8_0_s00[1]);
                }
                else if (stalkerProps.relKeyframeIdx_FE == 25)
                {
                    Collision_CharaCollisionSet(stalker, &sharedData_800DE8C8_0_s00[keyframeIdx0], &sharedData_800DE1E8_0_s00[1]);
                }
                else if (stalkerProps.relKeyframeIdx_FE == 27)
                {
                    Collision_CharaCollisionSet(stalker, &sharedData_800DEB0C_0_s00[keyframeIdx0], &sharedData_800DE1E8_0_s00[1]);
                }
            }
            break;

        case ANIM_STATUS(StalkerAnim_25, true):
            keyframeIdx0 = ANIM_TIME_REL_KEYFRAME_IDX_GET(stalker->model.anim.time, 407);
            keyframeIdx1 = ANIM_TIME_REL_KEYFRAME_IDX_GET(stalker->model.anim.time, 406);
            Collision_CharaCollisionSet(stalker, &sharedData_800DE1E8_0_s00[keyframeIdx0], &sharedData_800DE1E8_0_s00[keyframeIdx1]);
            break;

        case ANIM_STATUS(StalkerAnim_26, false):
        case ANIM_STATUS(StalkerAnim_26, true):
            Chara_CollisionSet(stalker, sharedData_800DE2C4_0_s00);
            break;

        case ANIM_STATUS(StalkerAnim_17, false):
        case ANIM_STATUS(StalkerAnim_17, true):
            Chara_CollisionSet(stalker, sharedData_800DE2D8_0_s00);
            break;

        case ANIM_STATUS(StalkerAnim_18, false):
        case ANIM_STATUS(StalkerAnim_18, true):
            Chara_CollisionSet(stalker, sharedData_800DE2EC_0_s00);
            break;

        case ANIM_STATUS(StalkerAnim_27, false):
            Chara_CollisionSet(stalker, sharedData_800DE300_0_s00[0]);
            break;

        case ANIM_STATUS(StalkerAnim_27, true):
            keyframeIdx0 = ANIM_TIME_REL_KEYFRAME_IDX_GET(stalker->model.anim.time, 427);
            if (keyframeIdx0 != 15)
            {
                keyframeIdx1 = ANIM_TIME_REL_KEYFRAME_IDX_GET(stalker->model.anim.time, 426);
            }
            else
            {
                keyframeIdx1 = 0;
            }

            Collision_CharaCollisionSet(stalker, &sharedData_800DE300_0_s00[keyframeIdx0], &sharedData_800DE300_0_s00[keyframeIdx1]);
            break;

        case ANIM_STATUS(StalkerAnim_34, false):
        case ANIM_STATUS(StalkerAnim_35, false):
            keyframeIdx0 = stalkerProps.keyframeIdx_FC;
            keyframeIdx1 = 0;
            Collision_CharaCollisionSet(stalker, &sharedData_800DEF1C_0_s00[keyframeIdx0], &sharedData_800DE440_0_s00[keyframeIdx1]);
            break;

        case ANIM_STATUS(StalkerAnim_34, true):
        case ANIM_STATUS(StalkerAnim_35, true):
            keyframeIdx0 = ANIM_TIME_REL_KEYFRAME_IDX_GET(stalker->model.anim.time, 461);
            keyframeIdx1 = ANIM_TIME_REL_KEYFRAME_IDX_GET(stalker->model.anim.time, 460);
            Collision_CharaCollisionSet(stalker, &sharedData_800DE440_0_s00[keyframeIdx0], &sharedData_800DE440_0_s00[keyframeIdx1]);

            stalker->collision.box.field_8 = stalker->collision.box.offsetY;
            break;

        case ANIM_STATUS(StalkerAnim_11, false):
            Collision_CharaCollisionSet(stalker, &sharedData_800DE2C4_0_s00, &sharedData_800DE580_0_s00[20]);
            break;

        case ANIM_STATUS(StalkerAnim_11, true):
            keyframeIdx0 = ANIM_TIME_REL_KEYFRAME_IDX_GET(stalker->model.anim.time, 107);
            keyframeIdx1 = ANIM_TIME_REL_KEYFRAME_IDX_GET(stalker->model.anim.time, 106);
            Collision_CharaCollisionSet(stalker, &sharedData_800DE580_0_s00[keyframeIdx0 + 20], &sharedData_800DE580_0_s00[keyframeIdx1 + 20]);
            break;

        case ANIM_STATUS(StalkerAnim_12, false):
            Collision_CharaCollisionSet(stalker, &sharedData_800DE2D8_0_s00, &sharedData_800DE8C8_0_s00[0]);
            break;

        case ANIM_STATUS(StalkerAnim_12, true):
            keyframeIdx0 = ANIM_TIME_REL_KEYFRAME_IDX_GET(stalker->model.anim.time, 129);
            keyframeIdx1 = ANIM_TIME_REL_KEYFRAME_IDX_GET(stalker->model.anim.time, 128);
            Collision_CharaCollisionSet(stalker, &sharedData_800DE8C8_0_s00[keyframeIdx0], &sharedData_800DE8C8_0_s00[keyframeIdx1]);
            break;

        case ANIM_STATUS(StalkerAnim_13, false):
            Collision_CharaCollisionSet(stalker, &sharedData_800DE2EC_0_s00, &sharedData_800DEB0C_0_s00[0]);
            break;

        case ANIM_STATUS(StalkerAnim_13, true):
            keyframeIdx0 = ANIM_TIME_REL_KEYFRAME_IDX_GET(stalker->model.anim.time, 158);
            keyframeIdx1 = ANIM_TIME_REL_KEYFRAME_IDX_GET(stalker->model.anim.time, 157);
            Collision_CharaCollisionSet(stalker, &sharedData_800DEB0C_0_s00[keyframeIdx0], &sharedData_800DEB0C_0_s00[keyframeIdx1]);
            break;

        case ANIM_STATUS(StalkerAnim_7, false):
            keyframeIdx0 = stalkerProps.keyframeIdx_FC;
            keyframeIdx1 = stalkerProps.relKeyframeIdx_FE;
            Collision_CharaCollisionSet(stalker, &sharedData_800DEF1C_0_s00[keyframeIdx0], &sharedData_800DE580_0_s00[keyframeIdx1]);
            break;

        case ANIM_STATUS(StalkerAnim_7, true):
            keyframeIdx0 = ANIM_TIME_REL_KEYFRAME_IDX_GET(stalker->model.anim.time, 50);
            keyframeIdx1 = ANIM_TIME_REL_KEYFRAME_IDX_GET(stalker->model.anim.time, 49);
            Collision_CharaCollisionSet(stalker, &sharedData_800DE580_0_s00[keyframeIdx0], &sharedData_800DE580_0_s00[keyframeIdx1]);

            stalker->collision.box.field_8 = Q12(-0.7f);
            stalkerProps.keyframeIdx_FC = 15;
            break;

        case ANIM_STATUS(StalkerAnim_1, false):
        case ANIM_STATUS(StalkerAnim_42, false):
            Collision_CharaCollisionSet(stalker,
                          &sharedData_800DE580_0_s00[19 + stalkerProps.keyframeIdx_FC],
                          &sharedData_800DEC74_0_s00[stalkerProps.relKeyframeIdx_FE]);
            break;

        case ANIM_STATUS(StalkerAnim_1, true):
        case ANIM_STATUS(StalkerAnim_42, true):
            keyframeIdx0 = FP_FROM(stalker->model.anim.time, Q12_SHIFT);
            keyframeIdx1 = FP_FROM(stalker->model.anim.time, Q12_SHIFT) + 1;
            Collision_CharaCollisionSet(stalker, &sharedData_800DEC74_0_s00[keyframeIdx0], &sharedData_800DEC74_0_s00[keyframeIdx1]);
            break;

        case ANIM_STATUS(StalkerAnim_3, false):
        case ANIM_STATUS(StalkerAnim_43, false):
            Collision_CharaCollisionSet(stalker,
                          &sharedData_800DE580_0_s00[19 + stalkerProps.keyframeIdx_FC],
                          &sharedData_800DECB0_0_s00[stalkerProps.relKeyframeIdx_FE]);
            break;

        case ANIM_STATUS(StalkerAnim_3, true):
        case ANIM_STATUS(StalkerAnim_43, true):
            keyframeIdx0 = ANIM_TIME_REL_KEYFRAME_IDX_GET(stalker->model.anim.time, 10);
            keyframeIdx1 = ANIM_TIME_REL_KEYFRAME_IDX_GET(stalker->model.anim.time, 9);
            Collision_CharaCollisionSet(stalker, &sharedData_800DECB0_0_s00[keyframeIdx0 + 1], &sharedData_800DECB0_0_s00[keyframeIdx1 + 1]);
            break;

        case ANIM_STATUS(StalkerAnim_2, false):
        case ANIM_STATUS(StalkerAnim_44, false):
            keyframeIdx2 = stalkerProps.keyframeIdx_FC;
            keyframeIdx2--;
            keyframeIdx1 = 0;
            Collision_CharaCollisionSet(stalker, &sharedData_800DE580_0_s00[keyframeIdx2 + 20], &sharedData_800DEDA0_0_s00[keyframeIdx1]);
            break;

        case ANIM_STATUS(StalkerAnim_2, true):
        case ANIM_STATUS(StalkerAnim_44, true):
            keyframeIdx0 = ANIM_TIME_REL_KEYFRAME_IDX_GET(stalker->model.anim.time, 4);
            keyframeIdx1 = ANIM_TIME_REL_KEYFRAME_IDX_GET(stalker->model.anim.time, 3);
            Collision_CharaCollisionSet(stalker, &sharedData_800DEDA0_0_s00[keyframeIdx0], &sharedData_800DEDA0_0_s00[keyframeIdx1]);
            break;

        case ANIM_STATUS(StalkerAnim_4, false):
            Collision_CharaCollisionSet(stalker,
                          &sharedData_800DE8C8_0_s00[stalkerProps.keyframeIdx_FC],
                          &sharedData_800DEE04_0_s00[stalkerProps.relKeyframeIdx_FE]);
            break;

        case ANIM_STATUS(StalkerAnim_4, true):
            keyframeIdx0 = ANIM_TIME_REL_KEYFRAME_IDX_GET(stalker->model.anim.time, 20);
            keyframeIdx1 = ANIM_TIME_REL_KEYFRAME_IDX_GET(stalker->model.anim.time, 19);
            Collision_CharaCollisionSet(stalker, &sharedData_800DEE04_0_s00[keyframeIdx0], &sharedData_800DEE04_0_s00[keyframeIdx1]);
            break;

        case ANIM_STATUS(StalkerAnim_5, false):
            if (stalkerProps.relKeyframeIdx_FE >= 2)
            {
                Collision_CharaCollisionSet(stalker,
                              &sharedData_800DE8C8_0_s00[stalkerProps.keyframeIdx_FC],
                              &sharedData_800DEE40_0_s00[stalkerProps.relKeyframeIdx_FE]);
            }
            else
            {
                Chara_CollisionSet(stalker, sharedData_800DEE68_0_s00[0]);
            }
            break;

        case ANIM_STATUS(StalkerAnim_5, true):
            keyframeIdx0 = ANIM_TIME_REL_KEYFRAME_IDX_GET(stalker->model.anim.time, 25);
            Collision_CharaCollisionSet(stalker, &sharedData_800DEE68_0_s00[keyframeIdx0], &sharedData_800DEE68_0_s00[keyframeIdx0 + 1]);
            break;

        case ANIM_STATUS(StalkerAnim_45, true):
            keyframeIdx0 = ANIM_TIME_REL_KEYFRAME_IDX_GET(stalker->model.anim.time, 158);
            Collision_CharaCollisionSet(stalker, &sharedData_800DEB0C_0_s00[keyframeIdx0 + 1], &sharedData_800DEB0C_0_s00[keyframeIdx0]);
            break;

        case ANIM_STATUS(StalkerAnim_30, false):
            if (stalkerProps.keyframeIdx_FC == 15)
            {
                Collision_CharaCollisionSet(stalker, &sharedData_800DE580_0_s00[19], &sharedData_800DEF1C_0_s00[0]);
            }
            else if (stalkerProps.keyframeIdx_FC == 55)
            {
                Collision_CharaCollisionSet(stalker, &sharedData_800DE300_0_s00[stalkerProps.relKeyframeIdx_FE], &sharedData_800DEF1C_0_s00[0]);
            }
            break;

        case ANIM_STATUS(StalkerAnim_30, true):
        case ANIM_STATUS(StalkerAnim_37, true):
            keyframeIdx0 = ANIM_TIME_REL_KEYFRAME_IDX_GET(stalker->model.anim.time, 477);
            if (keyframeIdx0 != 29)
            {
                keyframeIdx1 = ANIM_TIME_REL_KEYFRAME_IDX_GET(stalker->model.anim.time, 476);
            }
            else
            {
                keyframeIdx1 = 0;
            }

            Collision_CharaCollisionSet(stalker, &sharedData_800DEF1C_0_s00[keyframeIdx0], &sharedData_800DEF1C_0_s00[keyframeIdx1]);
            break;
    }

    Chara_CollisionShapeOffsetsUpdate(&stalker->collision.shapeOffsets, stalker);
}

void sharedFunc_800D7BE8_0_s00(s_SubCharacter* stalker)
{
    if (stalkerProps.field_10A == 0)
    {
        if (!(stalkerProps.flags & StalkerFlag_10))
        {
            if (!(stalkerProps.flags & StalkerFlag_6) && stalker->model.controlState != 12)
            {
                stalkerProps.timer_108 += g_DeltaTime;
                Rng_Rand16();
            }
        }
    }
    else
    {
        switch (stalkerProps.sfxId_102)
        {
            case Sfx_Unk1365:
                if (stalkerProps.timer_108 > Q12(0.2f) &&
                    stalkerProps.timer_108 < Q12(1.2f))
                {
                    Sfx_WithFlagsPlay(Sfx_Unk1365, &stalker->position, Q8(0.5f), SfxFlag_None);
                    stalkerProps.timer_108 += Q12(1.0f);
                }
                else if (stalkerProps.timer_108 < Q12(0.0f) ||
                         stalkerProps.timer_108 > Q12(1.8f))
                {
                    stalkerProps.field_10A = 0;
                }

                stalkerProps.timer_108 += g_DeltaTime;
                break;

            case Sfx_Unk1364:
                stalkerProps.timer_108 += g_DeltaTime;
                if (stalkerProps.timer_108 <= Q12(0.6f) ||
                    stalkerProps.timer_108 >= Q12(1.6f))
                {
                    if (stalkerProps.timer_108 < Q12(0.0f) ||
                        stalkerProps.timer_108 > Q12(2.8f))
                    {
                        stalkerProps.field_10A = 0;
                    }
                    break;
                }

                Sfx_WithFlagsPlay(stalkerProps.sfxId_102, &stalker->position, Q8(0.5f), SfxFlag_None);
                stalkerProps.timer_108 += Q12(1.0f);
                break;

            case Sfx_Unk1363:
                stalkerProps.timer_108 += g_DeltaTime;
                if (stalkerProps.timer_108 < Q12(0.0f) ||
                    stalkerProps.timer_108 > Q12(0.5f))
                {
                    if (Rng_GenerateInt(0, 7)) // 7 in 8 chance?
                    {
                        Sfx_WithFlagsPlay(Sfx_Unk1363, &stalker->position, Q8(0.5f), SfxFlag_None);
                    }

                    stalkerProps.field_10A = 0;
                }
                break;

            case Sfx_Unk1366:
                stalkerProps.timer_108 += g_DeltaTime;
                if (stalkerProps.timer_108 > Q12(0.2f) &&
                    stalkerProps.timer_108 < Q12(1.2f))
                {
                    Sfx_WithFlagsPlay(stalkerProps.sfxId_102, &stalker->position, Q8(0.5f), SfxFlag_None);
                    stalkerProps.timer_108 += Q12(1.0f);
                }
                else if (stalkerProps.timer_108 < Q12(0.0f) ||
                         stalkerProps.timer_108 > Q12(3.0f))
                {
                    stalkerProps.field_10A = 0;
                }
                break;
        }
    }
}

void sharedFunc_800D7E04_0_s00(s_SubCharacter* stalker, s32 sfxId)
{
    s32 newSfxId;

    newSfxId = NO_VALUE;
    switch (sfxId)
    {
        case Sfx_Unk1365:
            if (stalkerProps.field_10A < 3)
            {
                newSfxId                                 = sfxId;
                stalkerProps.field_10A = 3;
                stalkerProps.timer_108 = Rng_GenerateInt(Q12(0.0f), Q12(0.2f) - 1);
            }
            break;

        case Sfx_Unk1364:
            if (stalkerProps.field_10A < 2)
            {
                newSfxId                                 = sfxId;
                stalkerProps.field_10A = 2;
                stalkerProps.timer_108 = Rng_GenerateInt(Q12(0.0f), Q12(0.3f) - 1);
            }
            break;

        case Sfx_Unk1363:
            if (stalkerProps.field_10A < 2)
            {
                newSfxId                                 = sfxId;
                stalkerProps.field_10A = 2;
                stalkerProps.timer_108 = Rng_GenerateInt(Q12(0.0f), Q12(0.3f) - 1);
            }
            break;

        case Sfx_Unk1366:
            if (stalkerProps.field_10A < 2)
            {
                newSfxId                                 = sfxId;
                stalkerProps.field_10A = 2;
                stalkerProps.timer_108 = Rng_GenerateInt(Q12(0.0f), Q12(0.3f) - 1);
            }
            break;
    }

    if (newSfxId != NO_VALUE)
    {
        Sd_SfxStop((u16)stalkerProps.sfxId_102);
        stalkerProps.sfxId_102 = newSfxId;
    }
}
