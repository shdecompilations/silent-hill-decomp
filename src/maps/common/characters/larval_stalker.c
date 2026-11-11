#include "bodyprog/bodyprog.h"
#include "bodyprog/events/npc_main.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player/player.h"
#include "main/rng.h"
#include "maps/common/shared.h"
#include "maps/common/characters/larval_stalker.h"

#define larvalStalkerProps larvalStalker->properties.larvalStalker

void LarvalStalker_Update(s_SubCharacter* larvalStalker, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords)
{
    s8* mapOverlayPtr;

    // Initialize.
    if (larvalStalker->model.controlState == LarvalStalkerControl_None)
    {
        LarvalStalker_Init(larvalStalker);
    }

    sharedFunc_800D17BC_1_s00(larvalStalker);
    LarvalStalker_ControlUpdate(larvalStalker);
    sharedFunc_800D140C_1_s00(larvalStalker, boneCoords);
    sharedFunc_800D1524_1_s00(larvalStalker, anmHdr, boneCoords);
    LarvalStalker_CollisionUpdate(larvalStalker);

    if (larvalStalkerProps.timer_10A < Q12(3.5f))
    {
        return;
    }

    larvalStalker->timer_C6 += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.25f);
    if (larvalStalker->timer_C6 <= Q12(1.0f))
    {
        return;
    }

    // TODO: Weird hack, or a drunk developer.
    mapOverlayPtr = &g_SavegamePtr->mapIdx;

    larvalStalker->timer_C6               = Q12(1.0f);
    larvalStalker->model.controlState = LarvalStalkerControl_1;

    if (*mapOverlayPtr == 37 || !Rng_GenerateInt(0, 3)) // 1 in 4 chance.
    {
        Savegame_EnemyStateUpdate(larvalStalker);
    }
}

#define Chara_TurnModulate(angle, limit, mult)                                    \
    angleDeltaToPlayer = angle;                                                   \
                                                                                  \
    if (limit <= ABS(angleDeltaToPlayer))                                         \
    {                                                                             \
        if ((angleDeltaToPlayer) > Q12_ANGLE(0.0f))                               \
        {                                                                         \
            larvalStalker->rotation.vy += Q12_MULT_PRECISE(g_DeltaTime, mult); \
        }                                                                         \
        else                                                                      \
        {                                                                         \
            larvalStalker->rotation.vy -= Q12_MULT_PRECISE(g_DeltaTime, mult); \
        }                                                                         \
    }

#define Chara_TurnModulate_REV(angle, limit, mult)                                \
    if (limit <= ABS(angle))                                                      \
    {                                                                             \
        if ((angle) < Q12_ANGLE(0.0f))                                            \
        {                                                                         \
            larvalStalker->rotation.vy += Q12_MULT_PRECISE(g_DeltaTime, mult); \
        }                                                                         \
        else                                                                      \
        {                                                                         \
            larvalStalker->rotation.vy -= Q12_MULT_PRECISE(g_DeltaTime, mult); \
        }                                                                         \
    }

#define ANIM_STUFF(mult, div)                                                                                                     \
    temp = Q12_MULT_PRECISE(LARVAL_STALKER_ANIM_INFOS[larvalStalker->model.anim.status].duration.constant, g_DeltaTime) * \
           mult / div;                                                                                                            \
    larvalStalkerProps.angle_100 = FP_FROM(temp * Math_Sin(larvalStalker->rotation.vy), Q12_SHIFT);                            \
    larvalStalkerProps.angle_102 = FP_FROM(temp * Math_Cos(larvalStalker->rotation.vy), Q12_SHIFT);

void LarvalStalker_ControlUpdate(s_SubCharacter* larvalStalker)
{
    s32     temp;
    VECTOR3 pos;
    q19_12  distToTarget;
    bool    hasLos;
    s32     idxInfo;
    q19_12  distStep;
    q19_12  baseDistMax;
    q19_12  angleDeltaToPlayer;
    q19_12  sinRotY;
    q19_12  cosRotY;
    q19_12  targetX;
    q19_12  targetZ;

    angleDeltaToPlayer = Math_AngleNormalizeSigned(Math_AngleBetweenPositionsGet(larvalStalker->position, PLAYER_CHARA().position) -
                                       larvalStalker->rotation.vy);
    distToTarget       = Math_Vector2MagCalcSafeQ6(larvalStalkerProps.targetPositionX - larvalStalker->position.vx,
                                             larvalStalkerProps.targetPositionZ - larvalStalker->position.vz);

    larvalStalker->rotation.vy = Math_AngleNormalizeSigned(larvalStalker->rotation.vy);

    if (ANIM_STATUS_IDX_GET(larvalStalker->model.anim.status) != LarvalStalkerAnim_GrabAttack)
    {
        larvalStalker->field_44.field_0 = 0;
    }

    if (!WorldEnv_IsInDarkness() && !WorldEnv_IsFlashlightAllowed())
    {
        baseDistMax = Q12(2.5f);
        distStep = Q12(8.0f);
    }
    else if (!WorldEnv_IsInDarkness() && WorldEnv_IsFlashlightAllowed())
    {
        baseDistMax = Q12(3.5f);
        distStep = Q12(10.0f);
    }
    else
    {
        baseDistMax = Q12(0.5f);
        distStep = Q12(1.5f);
    }

    if (Math_Vector2MagCalcSafeQ6(PLAYER_CHARA().position.vx - larvalStalker->position.vx,
                            PLAYER_CHARA().position.vz - larvalStalker->position.vz) < ((baseDistMax * 2) + distStep))
    {
        larvalStalkerProps.targetPositionX = PLAYER_CHARA().position.vx;
        larvalStalkerProps.targetPositionZ = PLAYER_CHARA().position.vz;
    }

    // Handle control state.
    switch (larvalStalker->model.controlState)
    {
        case LarvalStalkerControl_1:
            larvalStalker->moveSpeed = Q12(0.0f);

            if (!Rng_GenerateInt(0, 31)) // 1 in 32 chance.
            {
                larvalStalker->health = NO_VALUE;
            }
            break;

        case LarvalStalkerControl_2:
            Chara_MoveSpeedUpdate3(larvalStalker, Q12(1.5f), Q12(0.3f));

            if (Los_CharaToCharaDistCheck(larvalStalker, Q12(1.0f), larvalStalker->rotation.vy) || !Rng_GenerateInt(0, 63)) // 1 in 64 chance.
            {
                larvalStalker->model.anim.status = ANIM_STATUS(LarvalStalkerAnim_Idle, false);
                larvalStalker->model.controlState = LarvalStalkerControl_3;
            }

            if (func_8006FD90(larvalStalker, 3, baseDistMax, distStep))
            {
                larvalStalker->model.controlState = LarvalStalkerControl_Idle;
                larvalStalker->model.anim.status = ANIM_STATUS(LarvalStalkerAnim_Idle, false);
                larvalStalkerProps.flags_E8 |= LarvalStalkerFlag_2;
            }
            break;

        case LarvalStalkerControl_3:
            Chara_MoveSpeedUpdate(larvalStalker, Q12(1.5f));

            if (larvalStalker->model.anim.status == ANIM_STATUS(LarvalStalkerAnim_WalkForward, true))
            {
                if (larvalStalkerProps.angle_108 > Q12_ANGLE(0.0f))
                {
                    larvalStalker->rotation.vy += Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(30.0f));
                    larvalStalkerProps.angle_108  -= Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(30.0f));

                    if ((larvalStalkerProps.angle_108) <= Q12_ANGLE(0.0f))
                    {
                        larvalStalkerProps.angle_108 = Q12_ANGLE(0.0f);
                        larvalStalker->model.anim.status = ANIM_STATUS(LarvalStalkerAnim_Idle, false);
                    }
                }
                else if (larvalStalkerProps.angle_108 < Q12_ANGLE(0.0f))
                {
                    larvalStalker->rotation.vy -= Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(30.0f));
                    larvalStalkerProps.angle_108  += Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(30.0f));

                    if (!(larvalStalkerProps.angle_108 < Q12_ANGLE(0.0f)))
                    {
                        larvalStalkerProps.angle_108 = Q12_ANGLE(0.0f);
                        larvalStalker->model.anim.status = ANIM_STATUS(LarvalStalkerAnim_Idle, false);
                    }
                }
                else
                {
                    larvalStalker->model.anim.status = ANIM_STATUS(LarvalStalkerAnim_Idle, false);
                }
            }

            if (larvalStalker->model.anim.status == ANIM_STATUS(LarvalStalkerAnim_Idle, true))
            {
                hasLos = Los_CharaToCharaDistCheck(larvalStalker, Q12(0.6f), larvalStalker->rotation.vy);
                if ((hasLos && !Rng_GenerateInt(0, 3)) || // 1 in 4 chance.
                    !Rng_GenerateInt(0, 31))                 // 1 in 32 chance.
                {
                    larvalStalkerProps.angle_108           = Rng_GenerateUInt(Q12_ANGLE(-90.0f), Q12_ANGLE(90.0f) - 1); // -90 >< 90 angle.
                    larvalStalker->model.anim.status = ANIM_STATUS(LarvalStalkerAnim_WalkForward, false);
                }
                else if (!Rng_GenerateInt(0, 15)) // 1 in 16 chance.
                {
                    if (hasLos == false)
                    {
                        larvalStalker->model.controlState = LarvalStalkerControl_2;
                        larvalStalker->model.anim.status = ANIM_STATUS(LarvalStalkerAnim_WalkForward, false);
                    }
                }
            }

            if (func_8006FD90(larvalStalker, 3, baseDistMax, distStep))
            {
                larvalStalker->model.controlState = LarvalStalkerControl_Idle;
                larvalStalker->model.anim.status = ANIM_STATUS(LarvalStalkerAnim_Idle, false);
                larvalStalkerProps.angle_108 = Q12_ANGLE(0.0f);
                larvalStalkerProps.flags_E8 |= LarvalStalkerFlag_2;
            }
            break;

        case LarvalStalkerControl_WalkForward:
            Chara_MoveSpeedUpdate3(larvalStalker, Q12(1.5f), Q12(0.3f));

            if ((larvalStalkerProps.flags_E8 & LarvalStalkerFlag_2) &&
                ANIM_STATUS_IDX_GET(larvalStalker->model.anim.status) == LarvalStalkerAnim_WalkForward &&
                ((distToTarget < (Q12(3.5f) - FP_TO(larvalStalkerProps.field_EA, Q12_SHIFT))) || !Rng_GenerateInt(0, 31))) // 1 in 32 chance.
            {
                larvalStalker->model.controlState = LarvalStalkerControl_Idle;
                larvalStalker->model.anim.status = ANIM_STATUS(LarvalStalkerAnim_Idle, false);
                larvalStalkerProps.flags_E8 &= ~LarvalStalkerFlag_3;
                break;
            }

            if (distToTarget >= (FP_TO(larvalStalkerProps.field_EA, Q12_SHIFT) + Q12(2.0f)) ||
                PLAYER_CHARA().moveSpeed <= ((larvalStalkerProps.field_EA * Q12(1.5f) )+ Q12(0.5f)))
            {
                if (Q12_ANGLE(45.0f) > ABS(angleDeltaToPlayer) &&
                    (distToTarget < ((larvalStalker->collision.cylinder.outerRadius + Q12(0.05f)) +
                                     PLAYER_CHARA().collision.cylinder.outerRadius)))
                {
                    if (!Rng_GenerateInt(0, 7)) // 1 in 8 chance.
                    {
                        targetX = larvalStalkerProps.targetPositionX + Rng_GenerateInt(0, Q12(1.5f) - 1);
                        targetZ = larvalStalkerProps.targetPositionZ + Rng_GenerateInt(0, Q12(1.5f) - 1);
                        larvalStalkerProps.angle_108 = Los_CharaHeadingAngleGet(larvalStalker, Q12(2.0f), targetX, targetZ, Q12(1.0f), true);
                        Chara_TurnModulate(Math_AngleNormalizeSigned(larvalStalkerProps.angle_108 - larvalStalker->rotation.vy), Q12_ANGLE(1.5f), Q12_ANGLE(30.0f));
                    }
                    else
                    {
                        Chara_TurnModulate_REV(angleDeltaToPlayer, Q12_ANGLE(1.5f), Q12_ANGLE(30.0f));
                    }
                    break;
                }

                if ((distStep >> 1) < distToTarget)
                {
                    if (!Rng_GenerateInt(0, 31)) // 1 in 32 chance.
                    {
                        larvalStalkerProps.angle_108 = Los_CharaHeadingAngleGet(larvalStalker, Q12(2.0f), larvalStalkerProps.targetPositionX, larvalStalkerProps.targetPositionZ, Q12(1.0f), true);
                    }

                    Chara_TurnModulate(Math_AngleNormalizeSigned(larvalStalkerProps.angle_108 - larvalStalker->rotation.vy), Q12_ANGLE(1.5f), Q12_ANGLE(30.0f));
                    break;
                }

                if (larvalStalkerProps.flags_E8 & LarvalStalkerFlag_3)
                {
                    Chara_TurnModulate(Math_AngleNormalizeSigned(larvalStalkerProps.angle_108 - larvalStalker->rotation.vy), Q12_ANGLE(1.5f), Q12_ANGLE(30.0f));
                    if (!Rng_GenerateInt(0, 15)) // 1 in 16 chance.
                    {
                        larvalStalkerProps.flags_E8 &= ~LarvalStalkerFlag_3;
                    }
                    break;
                }

                if (!(g_SysWork.charaGroupFlags[3] & CharaGroupFlag_0))
                {
                    Chara_TurnModulate(angleDeltaToPlayer, Q12_ANGLE(1.5f), Q12_ANGLE(30.0f));
                }
                else
                {
                    larvalStalker->model.controlState = LarvalStalkerControl_Idle;
                    larvalStalker->model.anim.status = ANIM_STATUS(LarvalStalkerAnim_Idle, false);
                }
            }
            else
            {
                larvalStalker->model.controlState = LarvalStalkerControl_8;

                Sfx_WithFlagsPlay(Sfx_Unk1429, &larvalStalker->position, Q8(0.5f), SfxFlag_None);

                larvalStalkerProps.timer_EC = Q12(0.0f);
                larvalStalker->model.anim.status = ANIM_STATUS(LarvalStalkerAnim_WalkForwardAlt, false);
                larvalStalkerProps.flags_E8 &= ~LarvalStalkerFlag_3;

            }
            break;

        case LarvalStalkerControl_Unused5:
            Chara_TurnModulate(angleDeltaToPlayer, Q12_ANGLE(1.5f), Q12_ANGLE(30.0f));

            if (larvalStalkerProps.flags_E8 & LarvalStalkerFlag_6)
            {
                larvalStalkerProps.animTime_104 = larvalStalker->model.anim.time;
                larvalStalker->moveSpeed = Q12(0.0f);
                larvalStalkerProps.timer_EC    += g_DeltaTime;

                if (larvalStalkerProps.timer_EC > Q12(1.8f))
                {
                    if (!Rng_GenerateInt(0, 7)) // 1 in 8 chance.
                    {
                        Sfx_WithFlagsPlay(0, &larvalStalker->position, Q8(0.5f), SfxFlag_None);
                        larvalStalkerProps.timer_EC = Q12(0.0f);

                        if (PLAYER_CHARA().field_40 == NO_VALUE)
                        {
                            PLAYER_CHARA().field_40 = Chara_NpcIdxGet(larvalStalker);
                        }

                        Chara_AttackReceivedSet(&PLAYER_CHARA(), WEAPON_ATTACK(EquippedWeaponId_Handgun, AttackInputType_Multitap));
                        PLAYER_CHARA().damage.amount += (FP_TO(D_800AD4C8[WEAPON_ATTACK(EquippedWeaponId_Handgun, AttackInputType_Multitap)].field_4, Q12_SHIFT) * ((Rng_Rand16() & 0x1F) + 0x55)) / 100;
                    }
                }

                angleDeltaToPlayer = Math_AngleNormalizeSigned(PLAYER_CHARA().rotation.vy - larvalStalker->rotation.vy);
                if (Q12_ANGLE(90.0f) < ABS(angleDeltaToPlayer))
                {
                    if (distToTarget >= Q12(0.29004f))
                    {
                        sinRotY = Math_Sin(larvalStalker->rotation.vy);
                        larvalStalkerProps.angle_100 = ((distToTarget - Q12(0.28f)) <= Q12(0.02f)) ?
                                                       Q12_MULT(distToTarget - Q12(0.28f), sinRotY) :
                                                       Q12_MULT(Q12(0.02f), sinRotY);

                        cosRotY = Math_Cos(larvalStalker->rotation.vy);
                        larvalStalkerProps.angle_102 = ((distToTarget - Q12(0.28f)) <= Q12(0.02f)) ?
                                                       Q12_MULT(distToTarget - Q12(0.28f), cosRotY) :
                                                       Q12_MULT(Q12(0.02f), cosRotY);
                    }

                    if (distToTarget < Q12(0.27f))
                    {
                        larvalStalkerProps.angle_100 = Q12_MULT(((distToTarget - Q12(0.28f)) < Q12(-0.02f)) ? Q12(-0.02f) : (distToTarget - Q12(0.28f)), Math_Sin(larvalStalker->rotation.vy));
                        larvalStalkerProps.angle_102 = Q12_MULT(((distToTarget - Q12(0.28f)) < Q12(-0.02f)) ? Q12(-0.02f) : (distToTarget - Q12(0.28f)), Math_Cos(larvalStalker->rotation.vy));
                    }
                }
                else
                {
                    if (distToTarget > Q12(0.18f))
                    {
                        sinRotY = Math_Sin(larvalStalker->rotation.vy);
                        larvalStalkerProps.angle_100 = ((distToTarget - Q12(0.17f)) <= Q12(0.02f)) ?
                                                       Q12_MULT(distToTarget - Q12(0.17f), sinRotY) :
                                                       Q12_MULT(Q12(0.02f), sinRotY);

                        sinRotY = Math_Cos(larvalStalker->rotation.vy);
                        larvalStalkerProps.angle_102 = ((distToTarget - Q12(0.17f)) <= Q12(0.02f)) ?
                                                       Q12_MULT(distToTarget - Q12(0.17f), sinRotY) :
                                                       Q12_MULT(Q12(0.02f), sinRotY);
                    }

                    if (distToTarget < Q12(0.16f))
                    {
                        larvalStalkerProps.angle_100 = Q12_MULT(((distToTarget - Q12(0.17f)) < Q12(-0.02f)) ? Q12(-0.02f) : (distToTarget - Q12(0.17f)), Math_Sin(larvalStalker->rotation.vy));
                        larvalStalkerProps.angle_102 = Q12_MULT(((distToTarget - Q12(0.17f)) < Q12(-0.02f)) ? Q12(-0.02f) : (distToTarget - Q12(0.17f)), Math_Cos(larvalStalker->rotation.vy));
                    }
                }

                if (Chara_AttackReceivedGet(&PLAYER_CHARA()) == NO_VALUE)
                {
                    g_SysWork.charaGroupFlags[3]     &= ~CharaGroupFlag_0;
                    larvalStalker->model.anim.status  = ANIM_STATUS(LarvalStalkerAnim_StunBackToIdle, false);
                    larvalStalker->model.controlState = LarvalStalkerControl_Stun;
                    larvalStalkerProps.flags_E8      &= ~LarvalStalkerFlag_6;
                    larvalStalker->collision.state    = CharaCollisionState_Npc;
                    larvalStalkerProps.keyframeIdx_F4 = 0;

                    Sfx_WithFlagsPlay(Sfx_Unk1429, &larvalStalker->position, Q8(0.5f), SfxFlag_None);
                }
                else
                {
                    larvalStalker->collision.state = CharaCollisionState_Player;
                }
            }
            else
            {
                if (distToTarget < ((larvalStalker->collision.cylinder.outerRadius + Q12(0.02f)) +
                                    PLAYER_CHARA().collision.cylinder.outerRadius))
                {
                    Chara_MoveSpeedUpdate(larvalStalker, Q12(1.5f));
                }

                pos     = larvalStalker->position;
                pos.vy += larvalStalker->collision.box.field_8;

                if (func_8008A0E4(1, WEAPON_ATTACK(EquippedWeaponId_Unk31, AttackInputType_Multitap), larvalStalker, &pos, &PLAYER_CHARA(), larvalStalker->rotation.vy, Q12_ANGLE(90.0f)) != NO_VALUE)
                {
                    larvalStalkerProps.timer_EC = Q12(0.0f);
                    larvalStalkerProps.flags_E8 |= LarvalStalkerFlag_6;
                }
                else if (distToTarget > Q12(0.5f))
                {
                    larvalStalker->model.anim.status  = ANIM_STATUS(LarvalStalkerAnim_WalkForward, false);
                    larvalStalker->model.controlState = LarvalStalkerControl_WalkForward;
                }

            }
            break;

        case LarvalStalkerControl_6:
            Chara_MoveSpeedUpdate3(larvalStalker, Q12(1.5f), Q12(1.0f));

            if (larvalStalkerProps.timer_EC == Q12(0.0f))
            {
                larvalStalkerProps.angle_108 = Los_CharaHeadingAngleGet(larvalStalker, Q12(2.0f), larvalStalkerProps.targetPositionX, larvalStalkerProps.targetPositionZ, Q12(1.0f), false);
                if (larvalStalkerProps.angle_108 == Q12(1.0f))
                {
                    larvalStalkerProps.angle_108 = Rng_GenerateUInt(0, Q12_ANGLE(360.0f) - 1);
                }
            }

            larvalStalkerProps.timer_EC += g_DeltaTime;
            if (larvalStalkerProps.timer_EC > Q12(0.8f))
            {
                larvalStalkerProps.timer_EC = Q12(0.0f);
            }

            Chara_TurnModulate(Math_AngleNormalizeSigned(larvalStalkerProps.angle_108 - larvalStalker->rotation.vy), 33, 682);

            if (larvalStalker->moveSpeed >= Q12(0.9004f) && !Rng_GenerateUInt(0, 255))
            {
                larvalStalker->model.anim.status = ANIM_STATUS(LarvalStalkerAnim_StunFrontToIdle, false);
                larvalStalkerProps.timer_EC = Q12(0.0f);
                larvalStalkerProps.keyframeIdx_F4 = 0;
                larvalStalker->model.controlState = LarvalStalkerControl_TripOver;

                Sfx_WithFlagsPlay(Sfx_Unk1429, &larvalStalker->position, Q8(0.5f), SfxFlag_None);
            }
            else
            {
                if ((Q12(4.8f) - (larvalStalkerProps.field_EA * Q12(1.2f))) < distToTarget &&
                    !Rng_GenerateInt(0, 31)) // 1 in 32 chance.
                {
                    larvalStalker->model.controlState = LarvalStalkerControl_Idle;
                    larvalStalker->model.anim.status = ANIM_STATUS(LarvalStalkerAnim_Idle, false);
                    larvalStalkerProps.timer_EC = Q12(0.0f);
                    larvalStalkerProps.angle_108 = Q12(0.0f);
                    larvalStalkerProps.flags_E8 |= LarvalStalkerFlag_2;
                }
            }
            break;

        case LarvalStalkerControl_Idle:
            if (Los_NpcToPlayerHitCheck(larvalStalker, &PLAYER_CHARA()) || (distStep * 2) < distToTarget)
            {
                larvalStalker->model.controlState = LarvalStalkerControl_3;

                if (ANIM_STATUS_IDX_GET(larvalStalker->model.anim.status) != LarvalStalkerAnim_Idle)
                {
                    larvalStalker->model.anim.status = ANIM_STATUS(LarvalStalkerAnim_Idle, false);
                }
            }
            else
            {
                if (Q12_ANGLE(1.5f) <= ABS(angleDeltaToPlayer) &&
                    ANIM_STATUS_IDX_GET(larvalStalker->model.anim.status) != LarvalStalkerAnim_Idle)
                {
                    Chara_MoveSpeedUpdate3(larvalStalker, Q12(0.375f), Q12(0.0f));

                    if (ANIM_STATUS_IDX_GET(larvalStalker->model.anim.status) != ANIM_STATUS(LarvalStalkerAnim_StunBackToIdle, true))
                    {
                        larvalStalker->model.anim.status = ANIM_STATUS(LarvalStalkerAnim_WalkForward, false);
                    }

                    if (angleDeltaToPlayer > Q12_ANGLE(0.0f))
                    {
                        larvalStalker->rotation.vy += Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(30.0f));
                    }
                    else
                    {
                        larvalStalker->rotation.vy -= Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(30.0f));
                    }
                }
                else
                {
                    Chara_MoveSpeedUpdate(larvalStalker, Q12(1.5f));

                    if (ANIM_STATUS_IDX_GET(larvalStalker->model.anim.status) != LarvalStalkerAnim_Idle)
                    {
                        larvalStalker->model.anim.status = ANIM_STATUS(LarvalStalkerAnim_Idle, false);
                    }
                }

                larvalStalkerProps.timer_EC += g_DeltaTime;

                if (!(g_SysWork.charaGroupFlags[3] & CharaGroupFlag_0) &&
                    (Q12(5.0f) - FP_TO(larvalStalkerProps.field_EA, Q12_SHIFT)) < larvalStalkerProps.timer_EC)
                {
                    larvalStalker->model.controlState = LarvalStalkerControl_WalkForward;
                    larvalStalker->model.anim.status  = ANIM_STATUS(LarvalStalkerAnim_WalkForward, false);
                    larvalStalkerProps.timer_EC = Q12(0.0f);

                    larvalStalkerProps.flags_E8 &= ~LarvalStalkerFlag_2;
                    if (larvalStalkerProps.field_EA < 2)
                    {
                        larvalStalkerProps.field_EA++;
                    }
                }
                else
                {
                    if (distToTarget < (FP_TO(larvalStalkerProps.field_EA, Q12_SHIFT) + Q12(2.0f)) &&
                        PLAYER_CHARA().moveSpeed > ((larvalStalkerProps.field_EA * Q12(1.5f)) + Q12(0.5f)))
                    {
                        larvalStalker->model.controlState = LarvalStalkerControl_8;

                        Sfx_WithFlagsPlay(Sfx_Unk1429, &larvalStalker->position, Q8(0.5f), SfxFlag_None);

                        larvalStalkerProps.timer_EC = Q12(0.0f);
                        larvalStalker->model.anim.status = ANIM_STATUS(LarvalStalkerAnim_WalkForwardAlt, false);
                        larvalStalkerProps.flags_E8 &= ~LarvalStalkerFlag_3;
                    }
                }
            }
            break;

        case LarvalStalkerControl_8:
            larvalStalkerProps.timer_10A += Q12_MULT_PRECISE(g_DeltaTime, Rng_Rand16() & 0xFFF);
            if (larvalStalkerProps.timer_10A > Q12(3.5f))
            {
                larvalStalkerProps.timer_10A = Q12(3.5f);
            }

            Chara_MoveSpeedUpdate3(larvalStalker, Q12(1.5f), Q12(1.0f));

            if (larvalStalkerProps.timer_EC == Q12(0.0f))
            {
                larvalStalkerProps.angle_108 = Los_CharaHeadingAngleGet(larvalStalker,
                                                                     Q12(1.5f),
                                                                     larvalStalkerProps.targetPositionX,
                                                                     larvalStalkerProps.targetPositionZ,
                                                                     Q12(1.0f),
                                                                     false);
                if (larvalStalkerProps.angle_108 == Q12(1.0f))
                {
                    larvalStalkerProps.angle_108 = Rng_Rand16() & 0xFFF;
                }
            }

            larvalStalkerProps.timer_EC += g_DeltaTime;
            if (larvalStalkerProps.timer_EC > Q12(0.8f))
            {
                larvalStalkerProps.timer_EC = Q12(0.0f);
            }

            Chara_TurnModulate(Math_AngleNormalizeSigned(larvalStalkerProps.angle_108 - larvalStalker->rotation.vy), Q12_ANGLE(1.5f), 682);

            if ((larvalStalker->moveSpeed >= Q12(0.9004f)) && !Rng_GenerateInt(0, 63)) // 1 in 64 chance.
            {
                larvalStalker->model.anim.status = ANIM_STATUS(LarvalStalkerAnim_StunFrontToIdle, false);
                larvalStalkerProps.timer_EC = Q12(0.0f);
                larvalStalkerProps.keyframeIdx_F4 = 0;
                larvalStalker->model.controlState = LarvalStalkerControl_TripOver;

                Sfx_WithFlagsPlay(Sfx_Unk1429, &larvalStalker->position, Q8(0.5f), SfxFlag_None);
            }

            if (!(larvalStalkerProps.flags_E8 & LarvalStalkerFlag_7))
            {
                larvalStalkerProps.timer_F0 += g_DeltaTime;
                if (larvalStalkerProps.timer_F0 > Q12(5.0f) && !Rng_GenerateInt(0, 63)) // 1 in 64 chance.
                {
                    larvalStalkerProps.timer_F0 = Q12(0.0f);
                    larvalStalkerProps.timer_EC = Q12(0.0f);
                    larvalStalkerProps.angle_108 = Q12(0.0f);
                    larvalStalker->model.controlState = LarvalStalkerControl_6;
                }
            }
            break;

        case LarvalStalkerControl_TripOver:
            larvalStalkerProps.timer_10A += Q12_MULT_PRECISE(g_DeltaTime, (Rng_Rand16() & 0x7FF) + Q12(0.5f));
            if (larvalStalkerProps.timer_10A > Q12(3.5f))
            {
                larvalStalkerProps.timer_10A = Q12(3.5f);
            }

            Chara_MoveSpeedUpdate(larvalStalker, Q12(0.5f));

            if (ANIM_TIME_RANGE_CHECK(larvalStalker->model.anim.time, 86, 92))
            {
                ANIM_STUFF(Q12(0.15f), Q12(6.0f));
            }

            if (ANIM_TIME_RANGE_CHECK(larvalStalker->model.anim.time, 95, 105))
            {
                larvalStalker->moveSpeed = 0;
            }

            if (ANIM_STATUS_IDX_GET(larvalStalker->model.anim.status) == LarvalStalkerAnim_Idle)
            {
                larvalStalker->model.anim.status = ANIM_STATUS(LarvalStalkerAnim_WalkForwardAlt, false);
                larvalStalker->model.controlState = LarvalStalkerControl_8;
            }
            break;

        case LarvalStalkerControl_Stun:
            Chara_MoveSpeedUpdate(larvalStalker, Q12(1.5f));

            if (ANIM_TIME_RANGE_CHECK(larvalStalker->model.anim.time, 86, 92))
            {
                ANIM_STUFF(Q12(0.15f), Q12(6.0f));
            }

            if (ANIM_TIME_RANGE_CHECK(larvalStalker->model.anim.time, 41, 49))
            {
                temp = Q12_MULT_PRECISE(LARVAL_STALKER_ANIM_INFOS[larvalStalker->model.anim.status].duration.constant,
                                           g_DeltaTime) * Q12(0.3f) / Q12(9.0f);
                larvalStalkerProps.angle_100 = FP_FROM(temp * Math_Sin(larvalStalker->rotation.vy + Q12_ANGLE(180.0f)), Q12_SHIFT);
                larvalStalkerProps.angle_102 = FP_FROM(temp * Math_Cos(larvalStalker->rotation.vy + Q12_ANGLE(180.0f)), Q12_SHIFT);
            }

            if (ANIM_TIME_RANGE_CHECK(larvalStalker->model.anim.time, 95, 96) && larvalStalker->moveSpeed != Q12(0.0f))
            {
                larvalStalker->model.anim.status = ANIM_STATUS(LarvalStalkerAnim_StunFrontToIdleAlt, true);
            }

            if (larvalStalker->model.anim.status == ANIM_STATUS(LarvalStalkerAnim_StunFrontToIdleAlt, true) &&
                larvalStalker->moveSpeed == Q12(0.0f) && !Rng_GenerateInt(0, 7)) // 1 in 8 chance.
            {
                larvalStalker->model.anim.status = ANIM_STATUS(LarvalStalkerAnim_StunFrontToIdle, true);
            }

            if (ANIM_TIME_RANGE_CHECK(larvalStalker->model.anim.time, 61, 62) && larvalStalker->moveSpeed != Q12(0.0f))
            {
                larvalStalker->model.anim.status = ANIM_STATUS(LarvalStalkerAnim_StunBackToIdleAlt, true);
            }

            if (larvalStalker->model.anim.status == ANIM_STATUS(LarvalStalkerAnim_StunBackToIdleAlt, true) &&
                larvalStalker->moveSpeed == Q12(0.0f) && !Rng_GenerateInt(0, 7)) // 1 in 8 chance.
            {
                larvalStalker->model.anim.status = ANIM_STATUS(LarvalStalkerAnim_StunBackToIdle, true);
            }

            if (ANIM_STATUS_IDX_GET(larvalStalker->model.anim.status) == LarvalStalkerAnim_Idle &&
                !Rng_GenerateInt(0, 7)) // 1 in 8 chance.
            {
                larvalStalker->model.anim.status = ANIM_STATUS(LarvalStalkerAnim_WalkForwardAlt, false);
                larvalStalker->model.controlState = LarvalStalkerControl_8;
                larvalStalkerProps.flags_E8 &= ~LarvalStalkerFlag_1;
            }
            break;

        case LarvalStalkerControl_11:
            if (ANIM_TIME_RANGE_CHECK(larvalStalker->model.anim.time, 143, 151))
            {
                ANIM_STUFF(Q12(0.3f), Q12(9.0f));
            }

            if (larvalStalker->health == Q12(0.0f) && ANIM_STATUS_IDX_GET(larvalStalker->model.anim.status) == LarvalStalkerAnim_StunFrontEnd)
            {
                larvalStalker->model.controlState = LarvalStalkerControl_Death;
                larvalStalker->model.anim.status = ANIM_STATUS(LarvalStalkerAnim_DeathFrontStart, false);
                Savegame_EnemyStateUpdate(larvalStalker);
            }

            if (larvalStalkerProps.flags_E8 & LarvalStalkerFlag_1)
            {
                Chara_MoveSpeedUpdate(larvalStalker, Q12(1.5f));

                if (larvalStalker->moveSpeed == Q12(0.0f))
                {
                    larvalStalkerProps.flags_E8 &= ~LarvalStalkerFlag_1;
                }
                break;
            }

            Chara_MoveSpeedUpdate3(larvalStalker, Q12(1.5f), Q12(0.1f));

            if (larvalStalkerProps.timer_EC == Q12(0.0f))
            {
                larvalStalkerProps.angle_108 = Los_CharaHeadingAngleGet(larvalStalker, Q12_ANGLE(180.0f), larvalStalkerProps.targetPositionX, larvalStalkerProps.targetPositionZ, Q12(1.0f), false);
            }

            larvalStalkerProps.timer_EC += g_DeltaTime;
            if (larvalStalkerProps.timer_EC > Q12(1.0f))
            {
                larvalStalkerProps.timer_EC = Q12(0.0f);
            }

            Chara_TurnModulate(Math_AngleNormalizeSigned(larvalStalkerProps.angle_108 - larvalStalker->rotation.vy), Q12_ANGLE(1.5f), 170);
            break;

        case LarvalStalkerControl_12:
            if (ANIM_TIME_RANGE_CHECK(larvalStalker->model.anim.time, 121, 127))
            {
                temp = (Q12_MULT_PRECISE(LARVAL_STALKER_ANIM_INFOS[larvalStalker->model.anim.status].duration.constant,
                                            g_DeltaTime) * Q12(0.15f)) / Q12(6.0f);
                larvalStalkerProps.angle_100 = FP_FROM(temp * Math_Sin(larvalStalker->rotation.vy + Q12_ANGLE(180.0f)), Q12_SHIFT);
                larvalStalkerProps.angle_102 = FP_FROM(temp * Math_Cos(larvalStalker->rotation.vy + Q12_ANGLE(180.0f)), Q12_SHIFT);
            }

            larvalStalker->flags |= CharaFlag_Unk2;

            if (larvalStalker->health <= Q12(0.0f) &&
                ANIM_STATUS_IDX_GET(larvalStalker->model.anim.status) == LarvalStalkerAnim_StunBackEnd)
            {
                larvalStalker->flags &= ~CharaFlag_Unk2;
                larvalStalker->model.controlState = LarvalStalkerControl_Death;
                larvalStalker->model.anim.status = ANIM_STATUS(LarvalStalkerAnim_DeathBackStart, false);
                Savegame_EnemyStateUpdate(larvalStalker);
            }

            Chara_MoveSpeedUpdate3(larvalStalker, Q12(1.5f), Q12(0.0f));
            break;

        case LarvalStalkerControl_Death:
            larvalStalker->flags &= ~CharaFlag_Unk2;

            if (larvalStalker->health <= Q12(0.0f) && Chara_NpcIdxGet(larvalStalker) != g_SysWork.targetNpcIdx)
            {
                larvalStalker->health          = NO_VALUE;
                larvalStalker->collision.state = CharaCollisionState_Ignore;
            }

            // TODO: Weird property cast. Originally, maybe a wrong properties struct was mistakenly accessed?
            if (larvalStalker->moveSpeed == Q12(0.0f) && !(*(s32*)&larvalStalkerProps.flags_E8 & (LarvalStalkerFlag_4 | LarvalStalkerFlag_5)))
            {
                if (ANIM_STATUS_IDX_GET(larvalStalker->model.anim.status) == LarvalStalkerAnim_DeathFrontEnd)
                {
                    func_800622B8(3, larvalStalker, 3, 3);
                    larvalStalkerProps.flags_E8 |= LarvalStalkerFlag_4;
                }
                else if (ANIM_STATUS_IDX_GET(larvalStalker->model.anim.status) == LarvalStalkerAnim_DeathBackCont)
                {
                    func_800622B8(3, larvalStalker, 4, 3);
                    larvalStalkerProps.flags_E8 |= LarvalStalkerFlag_4;
                }
            }

            Chara_MoveSpeedUpdate3(larvalStalker, Q12(1.5f), Q12(0.0f));
            break;
    }

    Chara_DamageClear(larvalStalker);
}

void sharedFunc_800D140C_1_s00(s_SubCharacter* larvalStalker, GsCOORDINATE2* boneCoords)
{
    s_CollisionResult sp18;

    larvalStalker->fallSpeed += g_GravitySpeed >> 1;

    if (!(larvalStalkerProps.flags_E8 & LarvalStalkerFlag_1))
    {
        larvalStalker->headingAngle = larvalStalker->rotation.vy;
    }

    if (g_DeltaTime != Q12(0.0f))
    {
        Chara_MovementUpdate1(larvalStalker, &sp18, larvalStalkerProps.angle_100, larvalStalkerProps.angle_102);
    }

    larvalStalkerProps.angle_102 = Q12_ANGLE(0.0f);
    larvalStalkerProps.angle_100 = Q12_ANGLE(0.0f);

    if (larvalStalker->fallSpeed != Q12(0.0f))
    {
        larvalStalkerProps.flags_E8 |= LarvalStalkerFlag_5;
    }
    else
    {
        larvalStalkerProps.flags_E8 &= ~LarvalStalkerFlag_5;
    }

    if (larvalStalker->moveSpeed > Q12(0.0f))
    {
        if (sp18.offset.vx == Q12(0.0f) &&
            sp18.offset.vz == Q12(0.0f))
        {
            larvalStalkerProps.angle_108 = Los_CharaHeadingAngleGet(larvalStalker,
                                                                          Q12(1.0f),
                                                                          larvalStalkerProps.targetPositionX,
                                                                          larvalStalkerProps.targetPositionZ,
                                                                          Q12(1.0f),
                                                                          false);
            larvalStalkerProps.flags_E8 |= LarvalStalkerFlag_3;
        }
    }

    Math_MatrixTransform(&larvalStalker->position, &larvalStalker->rotation, boneCoords);
}

void sharedFunc_800D1524_1_s00(s_SubCharacter* larvalStalker, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords)
{
    s_AnimInfo* animInfo;

    #define activeAnimInfo LARVAL_STALKER_ANIM_INFOS[larvalStalker->model.anim.status]

    switch (larvalStalker->model.anim.status)
    {
        case ANIM_STATUS(LarvalStalkerAnim_StunBackToIdle, false):
            activeAnimInfo.endKeyframeIdx = larvalStalkerProps.keyframeIdx_F4 + 41;
            break;

        case ANIM_STATUS(LarvalStalkerAnim_StunBackToIdle, true):
            activeAnimInfo.startKeyframeIdx = larvalStalkerProps.keyframeIdx_F4 + 41;
            break;

        case ANIM_STATUS(LarvalStalkerAnim_StunBackStart, false):
            activeAnimInfo.endKeyframeIdx = larvalStalkerProps.keyframeIdx_F4 + 121;
            break;

        case ANIM_STATUS(LarvalStalkerAnim_StunBackStart, true):
            activeAnimInfo.startKeyframeIdx = larvalStalkerProps.keyframeIdx_F4 + 121;
            break;

        case ANIM_STATUS(LarvalStalkerAnim_StunFrontToIdle, false):
            activeAnimInfo.endKeyframeIdx = larvalStalkerProps.keyframeIdx_F4 + 86;
            break;

        case ANIM_STATUS(LarvalStalkerAnim_StunFrontToIdle, true):
            activeAnimInfo.startKeyframeIdx = larvalStalkerProps.keyframeIdx_F4 + 86;
            break;

        case ANIM_STATUS(LarvalStalkerAnim_StunFrontStart, false):
            activeAnimInfo.endKeyframeIdx = larvalStalkerProps.keyframeIdx_F4 + 143;
            break;

        case ANIM_STATUS(LarvalStalkerAnim_StunFrontStart, true):
            activeAnimInfo.startKeyframeIdx = larvalStalkerProps.keyframeIdx_F4 + 143;
            break;

        case ANIM_STATUS(LarvalStalkerAnim_WalkForwardAlt, true):
            activeAnimInfo.duration.constant = Q12_MULT_FLOAT_PRECISE(larvalStalker->moveSpeed, 32.0f);
            break;
    }

    animInfo = &LARVAL_STALKER_ANIM_INFOS[larvalStalker->model.anim.status];
    animInfo->playbackFunc(&larvalStalker->model, anmHdr, boneCoords, animInfo);

    #undef activeAnimInfo
}

void LarvalStalker_Init(s_SubCharacter* larvalStalker)
{
    s32 i;

    larvalStalker->model.controlState = LarvalStalkerControl_3;
    larvalStalker->model.anim.time    = Q12(0.0f);
    Chara_AnimSet(larvalStalker, ANIM_STATUS(LarvalStalkerAnim_Idle, false), 162);

    larvalStalkerProps.flags_E8 = LarvalStalkerFlag_None;
    larvalStalker->model.anim.alpha = Q12(0.0f);
    larvalStalker->model.stateStep  = 0;
    ModelAnim_AnimInfoSet(&larvalStalker->model.anim, LARVAL_STALKER_ANIM_INFOS);

    larvalStalker->health    = Q12(300.0f);
    larvalStalker->fallSpeed = Q12(0.0f);
    larvalStalker->moveSpeed = Q12(0.0f);

    Chara_DamageClear(larvalStalker);
    larvalStalker->collision.state = CharaCollisionState_Ignore;
    larvalStalker->headingAngle    = larvalStalker->rotation.vy;

    Chara_PropsClear(larvalStalker);
    larvalStalkerProps.targetPositionX = PLAYER_CHARA().position.vx;
    larvalStalkerProps.targetPositionZ = PLAYER_CHARA().position.vz;
}

void sharedFunc_800D17BC_1_s00(s_SubCharacter* larvalStalker)
{
    q19_12 angle;
    s32    keyframe;
    s32    keyframe2;
    s16    keyframeOffset;

    if (larvalStalker->damage.amount <= Q12(0.0f) || larvalStalker->health <= Q12(0.0f))
    {
        return;
    }

    Sfx_WithFlagsPlay(0, &larvalStalker->position, Q8(0.5f), SfxFlag_None);

    larvalStalkerProps.flags_E8 |= LarvalStalkerFlag_7;

    larvalStalker->health                = MAX(larvalStalker->health - larvalStalker->damage.amount, Q12(0.0f));
    larvalStalker->damage.position.vx += FP_FROM(larvalStalker->moveSpeed * Math_Sin(larvalStalker->headingAngle), Q12_SHIFT);
    larvalStalker->damage.position.vz += FP_FROM(larvalStalker->moveSpeed * Math_Cos(larvalStalker->headingAngle), Q12_SHIFT);
    larvalStalker->moveSpeed             = FP_TO(Math_Vector2MagCalcSafeQ6(larvalStalker->damage.position.vx, larvalStalker->damage.position.vz), Q12_SHIFT) / Q12(2.4f);
    larvalStalker->fallSpeed             = FP_TO(larvalStalker->damage.position.vy, Q12_SHIFT) / Q12(2.4f);
    larvalStalker->headingAngle          = ratan2(larvalStalker->damage.position.vx, larvalStalker->damage.position.vz);

    larvalStalkerProps.flags_E8       |= LarvalStalkerFlag_1;
    larvalStalkerProps.targetPositionX = PLAYER_CHARA().position.vx;
    larvalStalkerProps.targetPositionZ = PLAYER_CHARA().position.vz;

    if (larvalStalker->model.controlState == LarvalStalkerControl_11)
    {
        if (ANIM_STATUS_IDX_GET(larvalStalker->model.anim.status) == LarvalStalkerAnim_StunFrontEnd)
        {
            larvalStalkerProps.keyframeIdx_F6                         = NO_VALUE;
            larvalStalker->model.anim.status = ANIM_STATUS(LarvalStalkerAnim_StunFrontCont, false);
        }
    }
    else if (larvalStalker->model.controlState == LarvalStalkerControl_12)
    {
        if (ANIM_STATUS_IDX_GET(larvalStalker->model.anim.status) == LarvalStalkerAnim_StunBackEnd)
        {
            larvalStalkerProps.keyframeIdx_F6                         = NO_VALUE;
            larvalStalker->model.anim.status = ANIM_STATUS(LarvalStalkerAnim_StunBackRecoil, false);
        }
    }
    else
    {
        if (Math_AngleNormalizeSigned(Math_AngleBetweenPositionsGet(larvalStalker->position, PLAYER_CHARA().position) -
                          larvalStalker->rotation.vy) < Q12_ANGLE(0.0f))
        {
            angle = -Math_AngleNormalizeSigned(Math_AngleBetweenPositionsGet(larvalStalker->position, PLAYER_CHARA().position) -
                                   larvalStalker->rotation.vy);
        }
        else
        {
            angle = Math_AngleNormalizeSigned(Math_AngleBetweenPositionsGet(larvalStalker->position, PLAYER_CHARA().position) -
                                  larvalStalker->rotation.vy);
        }

        if (larvalStalker->health > Q12(100.0f))
        {
            larvalStalker->model.controlState = LarvalStalkerControl_Stun;

            if (angle < Q12_ANGLE(90.0f))
            {
                keyframe2      = FP_FROM(larvalStalker->model.anim.time, Q12_SHIFT);
                keyframeOffset = keyframe2 - 41;
                if (ANIM_TIME_RANGE_CHECK(larvalStalker->model.anim.time, 41, 65))
                {
                    // No-op.
                }
                else if (ANIM_TIME_RANGE_CHECK(larvalStalker->model.anim.time, 66, 72))
                {
                    larvalStalkerProps.keyframeIdx_F6 = keyframeOffset;
                    if (keyframeOffset == 30)
                    {
                        larvalStalker->model.anim.time -= Q12(1.0f);
                    }

                    larvalStalker->model.anim.status                       = ANIM_STATUS(LarvalStalkerAnim_StunBackToIdle, false);
                    larvalStalkerProps.keyframeIdx_F4 = 76 - FP_FROM(larvalStalker->model.anim.time, Q12_SHIFT);
                }
                else if (ANIM_TIME_RANGE_CHECK(larvalStalker->model.anim.time, 73, 74))
                {
                    larvalStalkerProps.keyframeIdx_F6                                               = keyframeOffset;
                    larvalStalker->model.anim.status                       = ANIM_STATUS(LarvalStalkerAnim_StunBackToIdle, false);
                    larvalStalkerProps.keyframeIdx_F4 = 5;
                }
                else if (ANIM_TIME_RANGE_CHECK(larvalStalker->model.anim.time, 75, 78))
                {
                    larvalStalkerProps.keyframeIdx_F6 = keyframeOffset;
                    if (keyframeOffset == 36)
                    {
                        larvalStalker->model.anim.time -= Q12(1.0f);
                    }
                    larvalStalker->model.anim.status = ANIM_STATUS(LarvalStalkerAnim_StunBackToIdle, false);
                    larvalStalkerProps.keyframeIdx_F4 = 79 - FP_FROM(larvalStalker->model.anim.time, Q12_SHIFT);
                }
                else
                {
                    larvalStalkerProps.keyframeIdx_F4 = 0;
                    larvalStalker->model.anim.status = ANIM_STATUS(LarvalStalkerAnim_StunBackToIdle, false);
                }
            }
            else
            {
                keyframe2      = FP_FROM(larvalStalker->model.anim.time, Q12_SHIFT);
                keyframeOffset = keyframe2 - 86;
                if (ANIM_TIME_RANGE_CHECK(larvalStalker->model.anim.time, 86, 104))
                {
                    // No-op.
                }
                else if (ANIM_TIME_RANGE_CHECK(larvalStalker->model.anim.time, 105, 118))
                {
                    larvalStalker->model.anim.status = ANIM_STATUS(LarvalStalkerAnim_StunFrontToIdle, false);
                    larvalStalkerProps.keyframeIdx_F6 = keyframeOffset;
                    larvalStalkerProps.keyframeIdx_F4 = 7 - ((FP_FROM(larvalStalker->model.anim.time, Q12_SHIFT) - 104) >> 1);
                }
                else
                {
                    larvalStalkerProps.keyframeIdx_F4 = 0;
                    larvalStalker->model.anim.status = ANIM_STATUS(LarvalStalkerAnim_StunFrontToIdle, false);
                }
            }
        }
        else
        {
            if (Rng_GenerateInt(0, 7) == 0) // 1 in 8 chance.
            {
                Savegame_EnemyStateUpdate(larvalStalker);
            }

            if (angle < Q12_ANGLE(90.0f))
            {
                keyframe       = FP_FROM(larvalStalker->model.anim.time, Q12_SHIFT);
                keyframeOffset = keyframe - 41;
                if (ANIM_TIME_RANGE_CHECK(larvalStalker->model.anim.time, 41, 51))
                {
                    larvalStalker->model.anim.status = ANIM_STATUS(LarvalStalkerAnim_StunBackStart, false);
                    keyframeOffset = FP_FROM(larvalStalker->model.anim.time, Q12_SHIFT) - 41;
                    larvalStalkerProps.keyframeIdx_F4 = keyframeOffset;
                    larvalStalkerProps.keyframeIdx_F6 = keyframeOffset;
                }
                else if (ANIM_TIME_RANGE_CHECK(larvalStalker->model.anim.time, 52, 65))
                {
                    larvalStalkerProps.keyframeIdx_F6 = keyframeOffset;
                    larvalStalker->model.anim.status = ANIM_STATUS(LarvalStalkerAnim_StunBackEnd, false);
                }
                else if (ANIM_TIME_RANGE_CHECK(larvalStalker->model.anim.time, 66, 72))
                {
                    larvalStalkerProps.keyframeIdx_F6 = keyframeOffset;
                    if (keyframeOffset == 30)
                    {
                        larvalStalker->model.anim.time -= Q12(1.0f);
                    }
                    larvalStalker->model.anim.status = ANIM_STATUS(LarvalStalkerAnim_StunBackStart, false);
                    larvalStalkerProps.keyframeIdx_F4 = 76 - FP_FROM(larvalStalker->model.anim.time, Q12_SHIFT);
                }
                else if (ANIM_TIME_RANGE_CHECK(larvalStalker->model.anim.time, 73, 74))
                {
                    larvalStalker->model.anim.status = ANIM_STATUS(LarvalStalkerAnim_StunBackStart, false);
                    larvalStalkerProps.keyframeIdx_F6 = keyframeOffset;
                    larvalStalker->model.anim.time = 5;
                }
                else if (ANIM_TIME_RANGE_CHECK(larvalStalker->model.anim.time, 75, 78))
                {
                    larvalStalkerProps.keyframeIdx_F6 = keyframeOffset;
                    if (keyframeOffset == 36)
                    {
                        larvalStalker->model.anim.time -= Q12(1.0f);
                    }
                    larvalStalker->model.anim.status = ANIM_STATUS(LarvalStalkerAnim_StunBackStart, false);
                    larvalStalker->model.anim.time   = 79 - FP_FROM(larvalStalker->model.anim.time, Q12_SHIFT);
                }
                else
                {
                    if (ANIM_STATUS_IDX_GET(larvalStalker->model.anim.status) != LarvalStalkerAnim_StunBackStart)
                    {
                        larvalStalkerProps.keyframeIdx_F4 = 0;
                        larvalStalker->model.anim.status = ANIM_STATUS(LarvalStalkerAnim_StunBackStart, false);
                    }
                }

                larvalStalker->model.controlState = LarvalStalkerControl_12;
            }
            else
            {
                keyframe2 = FP_FROM(larvalStalker->model.anim.time, Q12_SHIFT);
                keyframeOffset = keyframe2 - 86;
                if (ANIM_TIME_RANGE_CHECK(larvalStalker->model.anim.time, 86, 96))
                {
                    larvalStalker->model.anim.status = ANIM_STATUS(LarvalStalkerAnim_StunFrontStart, false);
                    keyframeOffset = FP_FROM(larvalStalker->model.anim.time, Q12_SHIFT) - 86;
                    larvalStalkerProps.keyframeIdx_F4 = keyframeOffset;
                    larvalStalkerProps.keyframeIdx_F6 = keyframeOffset;
                }
                else if (ANIM_TIME_RANGE_CHECK(larvalStalker->model.anim.time, 97, 104))
                {
                    larvalStalkerProps.keyframeIdx_F6 = keyframeOffset;
                    larvalStalker->model.anim.status = ANIM_STATUS(LarvalStalkerAnim_StunFrontEnd, false);
                }
                else if (ANIM_TIME_RANGE_CHECK(larvalStalker->model.anim.time, 105, 118))
                {
                    larvalStalker->model.anim.status = ANIM_STATUS(LarvalStalkerAnim_StunFrontStart, false);
                    larvalStalkerProps.keyframeIdx_F6 = keyframeOffset;
                    larvalStalkerProps.keyframeIdx_F4 = 7 - ((FP_FROM(larvalStalker->model.anim.time, Q12_SHIFT) - 104) >> 1);
                }
                else
                {
                    if (ANIM_STATUS_IDX_GET(larvalStalker->model.anim.status) != LarvalStalkerAnim_StunFrontStart)
                    {
                        larvalStalkerProps.keyframeIdx_F4 = 0;
                    }

                    larvalStalker->model.anim.status = ANIM_STATUS(LarvalStalkerAnim_StunFrontStart, false);
                }

                larvalStalker->model.controlState = LarvalStalkerControl_11;
            }
        }
    }

    larvalStalkerProps.angle_108 = 0;
    larvalStalkerProps.timer_EC = Q12(0.0f);
}

#include "bodyprog/bodyprog.h"

extern s_CharaKeyframe sharedData_800DA928_1_s00[];
extern s_CharaKeyframe sharedData_800DA93C_1_s00;
extern s_CharaKeyframe sharedData_800DA950_1_s00;
extern s_CharaKeyframe sharedData_800DA964_1_s00[];

extern s_CharaKeyframe sharedData_800DAC34_1_s00[];
extern s_CharaKeyframe sharedData_800DAE28_1_s00[];

extern s_CharaKeyframe sharedData_800DAF54_1_s00;
extern s_CharaKeyframe sharedData_800DAF68_1_s00[];

extern s_CharaKeyframe sharedData_800DAFF4_1_s00;

extern s_CharaKeyframe sharedData_800DB008_1_s00;
extern s_CharaKeyframe sharedData_800DB01C_1_s00;

extern s_CharaKeyframe sharedData_800DB030_1_s00;

void LarvalStalker_CollisionUpdate(s_SubCharacter* larvalStalker)
{
    s32 keyframeIdx0;
    s32 keyframeIdx1;
    s32 keyframeIdx2;
    s32 activeKeyframeIdx0;
    s32 relKeyframeIdx;
    s32 activeKeyframeIdx1;
    s32 keyframeIdx3;

    switch (larvalStalker->model.anim.status)
    {
        case ANIM_STATUS(LarvalStalkerAnim_StunBackEnd, false):
            keyframeIdx1 = larvalStalkerProps.keyframeIdx_F6;
            if (keyframeIdx1 != -1)
            {
                if (keyframeIdx1 == -2)
                {
                    Collision_CharaCollisionSet(larvalStalker, &sharedData_800DAF54_1_s00, &sharedData_800DA93C_1_s00);
                    break;
                }

                relKeyframeIdx = keyframeIdx1;
                if (relKeyframeIdx < 15)
                {
                    keyframeIdx0 = relKeyframeIdx - (relKeyframeIdx >= 11);
                }
                else
                {
                    if ((relKeyframeIdx - 24) >= 0)
                    {
                        keyframeIdx0 = relKeyframeIdx - 10;
                    }
                    else
                    {
                        keyframeIdx0 = 14;
                    }
                }

                Collision_CharaCollisionSet(larvalStalker, &sharedData_800DA964_1_s00[keyframeIdx0], &sharedData_800DA93C_1_s00);
                break;
            }

            Chara_CollisionSet(larvalStalker, sharedData_800DA928_1_s00[1]);
            break;

        case ANIM_STATUS(LarvalStalkerAnim_DeathBackStart, true):
        case ANIM_STATUS(LarvalStalkerAnim_StunBackRecoil, true):
            keyframeIdx0 = (FP_FROM(larvalStalker->model.anim.time, Q12_SHIFT) > 24 && FP_FROM(larvalStalker->model.anim.time, Q12_SHIFT) < 30) ? 0 : 1;
            keyframeIdx1 = (FP_FROM(larvalStalker->model.anim.time, Q12_SHIFT) > 23 && FP_FROM(larvalStalker->model.anim.time, Q12_SHIFT) < 29) ? 0 : 1;
            Collision_CharaCollisionSet(larvalStalker, &sharedData_800DA928_1_s00[keyframeIdx0], &sharedData_800DA928_1_s00[keyframeIdx1]);
            break;

        case ANIM_STATUS(LarvalStalkerAnim_StunBackEnd, true):
        case ANIM_STATUS(LarvalStalkerAnim_DeathBackStart, false):
        case ANIM_STATUS(LarvalStalkerAnim_DeathBackCont, false):
        case ANIM_STATUS(LarvalStalkerAnim_DeathBackCont, true):
        case ANIM_STATUS(LarvalStalkerAnim_StunBackRecoil, false):
            Chara_CollisionSet(larvalStalker, sharedData_800DA928_1_s00[1]);
            break;

        case ANIM_STATUS(LarvalStalkerAnim_DeathFrontStart, false):
            Collision_CharaCollisionSet(larvalStalker, &sharedData_800DB008_1_s00, &sharedData_800DA950_1_s00);
            break;

        case ANIM_STATUS(LarvalStalkerAnim_DeathFrontStart, true):
        case ANIM_STATUS(LarvalStalkerAnim_DeathFrontEnd, false):
        case ANIM_STATUS(LarvalStalkerAnim_DeathFrontEnd, true):
            Chara_CollisionSet(larvalStalker, sharedData_800DA950_1_s00);
            break;

        case ANIM_STATUS(LarvalStalkerAnim_StunBackToIdle, false):
            keyframeIdx3 = larvalStalkerProps.keyframeIdx_F4;
            if (keyframeIdx3 == 0)
            {
                Chara_CollisionSet(larvalStalker, sharedData_800DA964_1_s00[0]);
                break;
            }

            keyframeIdx1 = larvalStalkerProps.keyframeIdx_F6;
            if ((keyframeIdx1 - 24) >= 0)
            {
                keyframeIdx0 = keyframeIdx1 - 10;
            }
            else
            {
                keyframeIdx0 = 14;
            }

            Collision_CharaCollisionSet(larvalStalker, &sharedData_800DA964_1_s00[keyframeIdx0], &sharedData_800DA964_1_s00[keyframeIdx3]);
            break;

        case ANIM_STATUS(LarvalStalkerAnim_StunBackToIdle, true):
        case ANIM_STATUS(LarvalStalkerAnim_StunBackToIdleAlt, true):
            activeKeyframeIdx0 = FP_FROM(larvalStalker->model.anim.time, Q12_SHIFT);
            relKeyframeIdx = activeKeyframeIdx0 - 41;
            if (relKeyframeIdx < 15)
            {
                keyframeIdx0 = relKeyframeIdx - (relKeyframeIdx >= 11);
                keyframeIdx1 = (activeKeyframeIdx0 - 40) - (relKeyframeIdx >= 10);
            }
            else
            {
                if ((activeKeyframeIdx0 - 65) >= 0)
                {
                    keyframeIdx0 = activeKeyframeIdx0 - 51;
                }
                else
                {
                    keyframeIdx0 = 14;
                }

                if ((activeKeyframeIdx0 - 64) >= 0)
                {
                    keyframeIdx1 = activeKeyframeIdx0 - 50;
                }
                else
                {
                    keyframeIdx1 = 14;
                }
            }

            Collision_CharaCollisionSet(larvalStalker, &sharedData_800DA964_1_s00[keyframeIdx0], &sharedData_800DA964_1_s00[keyframeIdx1]);
            break;

        case ANIM_STATUS(LarvalStalkerAnim_StunFrontToIdle, false):
            keyframeIdx2 = larvalStalkerProps.keyframeIdx_F4;
            if (keyframeIdx2 == 0)
            {
                Chara_CollisionSet(larvalStalker, sharedData_800DAC34_1_s00[0]);
                break;
            }

            relKeyframeIdx = larvalStalkerProps.keyframeIdx_F6;
            keyframeIdx0 = ((relKeyframeIdx - 7) - (relKeyframeIdx >= 23)) - (relKeyframeIdx >= 24);
            Collision_CharaCollisionSet(larvalStalker, &sharedData_800DAC34_1_s00[keyframeIdx0], &sharedData_800DAC34_1_s00[keyframeIdx2]);
            break;

        case ANIM_STATUS(LarvalStalkerAnim_StunFrontToIdle, true):
        case ANIM_STATUS(LarvalStalkerAnim_StunFrontToIdleAlt, true):
            activeKeyframeIdx1 = FP_FROM(larvalStalker->model.anim.time, Q12_SHIFT);
            relKeyframeIdx = activeKeyframeIdx1 - 86;
            if (relKeyframeIdx < 15)
            {
                keyframeIdx0 = MIN(8, relKeyframeIdx);
                keyframeIdx1 = MIN(8, activeKeyframeIdx1 - 85);
            }
            else
            {
                keyframeIdx0 = (activeKeyframeIdx1 - 93) - (relKeyframeIdx >= 23) - (relKeyframeIdx >= 24);
                keyframeIdx1 = (activeKeyframeIdx1 - 92) - (relKeyframeIdx >= 22) - (relKeyframeIdx >= 23) - (relKeyframeIdx >= 33);
            }

            Collision_CharaCollisionSet(larvalStalker, &sharedData_800DAC34_1_s00[keyframeIdx0], &sharedData_800DAC34_1_s00[keyframeIdx1]);
            break;

        case ANIM_STATUS(LarvalStalkerAnim_StunBackStart, false):
            if (larvalStalkerProps.keyframeIdx_F4 == 0)
            {
                Chara_CollisionSet(larvalStalker, sharedData_800DAE28_1_s00[0]);
                break;
            }

            relKeyframeIdx = larvalStalkerProps.keyframeIdx_F6;
            if (relKeyframeIdx < 15)
            {
                keyframeIdx0 = relKeyframeIdx - (relKeyframeIdx >= 11);
            }
            else
            {
                if ((relKeyframeIdx - 24) >= 0)
                {
                    keyframeIdx0 = relKeyframeIdx - 10;
                }
                else
                {
                    keyframeIdx0 = 14;
                }
            }

            keyframeIdx1 = larvalStalkerProps.keyframeIdx_F4;
            Collision_CharaCollisionSet(larvalStalker, &sharedData_800DA964_1_s00[keyframeIdx0], &sharedData_800DAE28_1_s00[keyframeIdx1]);
            break;

        case ANIM_STATUS(LarvalStalkerAnim_StunBackStart, true):
            larvalStalkerProps.keyframeIdx_F6 = -2;
            keyframeIdx1 = FP_FROM(larvalStalker->model.anim.time, Q12_SHIFT);

            relKeyframeIdx = keyframeIdx1 - 121;
            keyframeIdx0   = relKeyframeIdx - (relKeyframeIdx > 10) < 16 ? relKeyframeIdx - (relKeyframeIdx > 10) : 15;

            keyframeIdx1 = relKeyframeIdx + 1;
            keyframeIdx1 = relKeyframeIdx + (relKeyframeIdx < 10) < 16 ? relKeyframeIdx + (relKeyframeIdx < 10) : 15;

            Collision_CharaCollisionSet(larvalStalker, &sharedData_800DAE28_1_s00[keyframeIdx0], &sharedData_800DAE28_1_s00[keyframeIdx1]);
            break;

        case ANIM_STATUS(LarvalStalkerAnim_StunFrontStart, false):
            if (larvalStalkerProps.keyframeIdx_F4 == 0)
            {
                Chara_CollisionSet(larvalStalker, sharedData_800DAF68_1_s00[0]);
                break;
            }

            relKeyframeIdx = larvalStalkerProps.keyframeIdx_F6;
            if (relKeyframeIdx < 15)
            {
                keyframeIdx0 = MIN(8, relKeyframeIdx);
            }
            else
            {
                keyframeIdx0 = ((relKeyframeIdx - 7) - (relKeyframeIdx >= 23)) - (relKeyframeIdx >= 24);
            }

            keyframeIdx1 = larvalStalkerProps.keyframeIdx_F4;
            Collision_CharaCollisionSet(larvalStalker, &sharedData_800DAC34_1_s00[keyframeIdx0], &sharedData_800DAF68_1_s00[keyframeIdx1]);
            break;

        case ANIM_STATUS(LarvalStalkerAnim_StunFrontStart, true):
            larvalStalkerProps.keyframeIdx_F6 = -2;
            activeKeyframeIdx0 = FP_FROM(larvalStalker->model.anim.time, Q12_SHIFT);
            relKeyframeIdx = activeKeyframeIdx0 - 143;

            keyframeIdx0 = MIN(7, relKeyframeIdx);
            keyframeIdx1 = MIN(7, activeKeyframeIdx0 - 142);
            Collision_CharaCollisionSet(larvalStalker, &sharedData_800DAF68_1_s00[keyframeIdx0], &sharedData_800DAF68_1_s00[keyframeIdx1]);
            break;

        case ANIM_STATUS(LarvalStalkerAnim_StunFrontEnd, false):
            keyframeIdx1 = larvalStalkerProps.keyframeIdx_F6;
            if (keyframeIdx1 == -1)
            {
                Chara_CollisionSet(larvalStalker, sharedData_800DB008_1_s00);
                break;
            }
            if (keyframeIdx1 == -2)
            {
                Collision_CharaCollisionSet(larvalStalker, &sharedData_800DAFF4_1_s00, &sharedData_800DB008_1_s00);
                break;
            }

            relKeyframeIdx = keyframeIdx1;
            if (relKeyframeIdx < 15)
            {
                keyframeIdx0 = MIN(8, relKeyframeIdx);
            }
            else
            {
                keyframeIdx0 = relKeyframeIdx - 7;
            }

            Collision_CharaCollisionSet(larvalStalker, &sharedData_800DAC34_1_s00[keyframeIdx0], &sharedData_800DB008_1_s00);
            break;

        case ANIM_STATUS(LarvalStalkerAnim_StunFrontEnd, true):
        case ANIM_STATUS(LarvalStalkerAnim_StunFrontCont, false):
        case ANIM_STATUS(LarvalStalkerAnim_StunFrontCont, true):
            Chara_CollisionSet(larvalStalker, sharedData_800DB008_1_s00);
            break;

        case ANIM_STATUS(LarvalStalkerAnim_Idle, false):
        case ANIM_STATUS(LarvalStalkerAnim_Idle, true):
            Chara_CollisionSet(larvalStalker, sharedData_800DB01C_1_s00);
            break;

        case ANIM_STATUS(LarvalStalkerAnim_WalkForward, false):
        case ANIM_STATUS(LarvalStalkerAnim_WalkForward, true):
        case ANIM_STATUS(LarvalStalkerAnim_WalkForwardAlt, false):
        case ANIM_STATUS(LarvalStalkerAnim_WalkForwardAlt, true):
            Chara_CollisionSet(larvalStalker, sharedData_800DB030_1_s00);
            break;

        case ANIM_STATUS(LarvalStalkerAnim_GrabAttack, false):
        case ANIM_STATUS(LarvalStalkerAnim_GrabAttack, true):
            larvalStalker->collision.box.topExtent            = Q12(-0.72f);
            larvalStalker->collision.box.height               = Q12(-0.2f);
            larvalStalker->collision.box.offsetY              = Q12(-0.66f);
            larvalStalker->collision.cylinder.outerRadius     = Q12(0.12f);
            larvalStalker->collision.shapeOffsets.box.vz      = Q12(0.02f);
            larvalStalker->collision.cylinder.innerRadius     = Q12(0.11f);
            larvalStalker->collision.box.bottomExtent         = Q12(0.0f);
            larvalStalker->collision.shapeOffsets.cylinder.vx = Q12(0.0f);
            larvalStalker->collision.shapeOffsets.cylinder.vz = Q12(0.0f);
            larvalStalker->collision.shapeOffsets.box.vx      = Q12(0.0f);
            larvalStalker->collision.box.field_8              = Q12(-0.59f);
            break;
    }

    Chara_CollisionShapeOffsetsUpdate(&larvalStalker->collision.shapeOffsets, larvalStalker);
}
