#include "bodyprog/bodyprog.h"
#include "bodyprog/events/npc_main.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player/player.h"
#include "main/rng.h"
#include "maps/common/shared.h"
#include "maps/common/characters/hanged_scratcher.h"

#define scratcherProps scratcher->properties.hangedScratcher

void HangedScratcher_Update(s_SubCharacter* scratcher, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords)
{
    // Initialize.
    if (scratcher->model.controlState == HangedScratcherControl_None)
    {
        HangedScratcher_Init(scratcher);
    }

    sharedFunc_800CFF74_5_s00(scratcher);
    HangedScratcher_ControlUpdate(scratcher);
    sharedFunc_800D26D8_5_s00(scratcher);
    sharedFunc_800D2844_5_s00(scratcher, anmHdr, boneCoords);
    HangedScratcher_CollisionUpdate(scratcher);
    sharedFunc_800D3300_5_s00(scratcher);
}

void HangedScratcher_Init(s_SubCharacter* scratcher)
{
    s32    i;
    q19_12 radiusMax;
    q19_12 radiusMin;

    scratcherProps.flags = 0;
    Chara_PropsClear(scratcher);

    scratcher->health = Q12(350.0f);
    if (g_SavegamePtr->gameDifficulty == GameDifficulty_Hard)
    {
        scratcher->health = Q12_MULT_PRECISE(scratcher->health, Q12(1.35f)); // 472.44873f
    }

    scratcher->moveSpeed    = Q12(0.0f);
    scratcher->fallSpeed    = Q12(0.0f);
    scratcher->headingAngle = scratcher->rotation.vy;
    scratcher->rotation.vx  = Q12_ANGLE(0.0f);

    ModelAnim_AnimInfoSet(&scratcher->model.anim, HANGED_SCRATCHER_ANIM_INFOS);
    Chara_DamageClear(scratcher);

    // Handle state step.
    switch (scratcher->model.stateStep)
    {
        case 3:
        case 9:
            if (scratcher->model.stateStep == 3)
            {
                scratcher->model.controlState = HangedScratcherControl_2;
            }
            else if (scratcher->model.stateStep == 7) // @unused This will never run? Meant to be `9`?
            {
                scratcher->model.controlState = HangedScratcherControl_16;
            }
            else
            {
                scratcher->model.controlState = HangedScratcherControl_1;
            }

            Chara_AnimSet(scratcher, ANIM_STATUS(HangedScratcherAnim_14, true), 222);
            scratcher->position.vy = Q12(-3.6f);
            scratcher->rotation.vz = Q12_ANGLE(180.0f);

        default:
            break;

        case 7:
        case 14:
            scratcher->model.controlState = HangedScratcherControl_5;
            Chara_AnimSet(scratcher, ANIM_STATUS(HangedScratcherAnim_15, true), 232);
            scratcher->position.vy = Q12(0.0f);
            scratcher->rotation.vz = Q12_ANGLE(0.0f);
            break;

        case 17:
            scratcherProps.timer_EA = Q12(1.2f);

        case 6:
            scratcher->model.controlState = HangedScratcherControl_6;
            Chara_AnimSet(scratcher, ANIM_STATUS(HangedScratcherAnim_15, true), 232);
            scratcher->position.vy = Q12(0.0f);
            scratcher->rotation.vz = Q12_ANGLE(0.0f);
            break;

        case 18:
        case 19:
        case 20:
            scratcher->model.controlState = HangedScratcherControl_14;
            scratcher->position.vy        = Q12(0.0f);
            scratcher->rotation.vz        = Q12_ANGLE(0.0f);

            switch (scratcher->model.stateStep)
            {
                case 18:
                    Chara_AnimSet(scratcher, ANIM_STATUS(HangedScratcherAnim_JumpDownFromWall, true), 0);
                    break;

                case 19:
                    Chara_AnimSet(scratcher, ANIM_STATUS(HangedScratcherAnim_EmergeFromWater, true), 51);
                    break;

                case 20:
                    Chara_AnimSet(scratcher, ANIM_STATUS(HangedScratcherAnim_JumpDownFromCeiling, true), 87);
                    break;
            }
            break;
    }

    scratcherProps.positionX_F4 = scratcher->position.vx;
    scratcherProps.positionZ_F8 = scratcher->position.vz;

    if (g_SavegamePtr->gameDifficulty == GameDifficulty_Easy)
    {
        radiusMax = Q12(2.7f);
        radiusMin = Q12(2.4f);
    }
    else if (g_SavegamePtr->gameDifficulty == GameDifficulty_Normal)
    {
        radiusMax = Q12(3.0f);
        radiusMin = Q12(2.8f);
    }
    else
    {
        radiusMax = Q12(3.8f);
        radiusMin = Q12(3.6f);
    }

    // TODO: Use `Rng_GenerateInt`.
    scratcherProps.radiusMax_10C = (Rng_Rand16() % (radiusMax >> 2)) + ((radiusMax * 3) >> 2);
    scratcherProps.radiusMin_10E = (Rng_Rand16() % (radiusMin >> 2)) + ((radiusMin * 3) >> 2);
    scratcher->flags         |= CharaFlag_NoRadioStatic | CharaFlag_Hit;
}

void sharedFunc_800CFF74_5_s00(s_SubCharacter* scratcher)
{
    s_CollisionSurface surface;
    q19_12             prevMoveSpeed;
    q3_12              prevHeadingAngle;

    if (scratcher->damage.amount > Q12(0.0f) && scratcher->health > Q12(0.0f))
    {
        scratcher->health              = MAX(Q12(0.0f), scratcher->health - scratcher->damage.amount);
        scratcher->damage.position.vx += (scratcher->moveSpeed * Math_Sin(scratcher->headingAngle)) >> 14;  // TODO: Weird shift?
        scratcher->damage.position.vz += (scratcher->moveSpeed * Math_Cos(scratcher->headingAngle)) >> 14;

        prevMoveSpeed           = scratcher->moveSpeed;
        scratcher->moveSpeed    = Q12_DIV(Math_Vector2MagCalcSafeQ6(scratcher->damage.position.vx, scratcher->damage.position.vz), Q12(4.0f));
        prevHeadingAngle        = scratcher->headingAngle;
        scratcher->headingAngle = ratan2(scratcher->damage.position.vx, scratcher->damage.position.vz);

        switch (scratcher->model.controlState)
        {
            case HangedScratcherControl_4:
                g_SysWork.charaGroupFlags[3] &= ~CharaGroupFlag_1;

            case HangedScratcherControl_1:
            case HangedScratcherControl_2:
            case HangedScratcherControl_RunForward:
            case HangedScratcherControl_9:
                Collision_SurfaceGet(&surface, scratcher->position.vx, scratcher->position.vz);

                if (scratcher->health == Q12(0.0f) && surface.floorType == FloorType_7)
                {
                    scratcher->health = 1;

                    if (Rng_GenerateUInt(0, 15) < 12)
                    {
                        scratcher->moveSpeed    = prevMoveSpeed;
                        scratcher->headingAngle = prevHeadingAngle;

                        Chara_DamageClear(scratcher);
                        return;
                    }
                }

                if (ABS(scratcher->position.vx - PLAYER_CHARA().position.vx) > Q12(0.2f) &&
                    ABS(scratcher->position.vz - PLAYER_CHARA().position.vz) > Q12(0.2f))
                {
                    if (surface.floorType != FloorType_7 && (Rng_Rand16() & 0xF) < 4)
                    {
                        scratcherProps.flags |= HangedScratcherFlag_1;
                    }
                }

                scratcher->model.controlState  = HangedScratcherControl_9;
                scratcher->model.anim.status = ANIM_STATUS(HangedScratcherAnim_8, false);
                break;

            case HangedScratcherControl_15:
                g_SysWork.charaGroupFlags[3] &= ~CharaGroupFlag_1;

            case HangedScratcherControl_5:
            case HangedScratcherControl_6:
            case HangedScratcherControl_WalkForward:
            case HangedScratcherControl_10:
                if (scratcher->health > Q12(100.0f))
                {
                    scratcher->model.controlState  = HangedScratcherControl_10;
                    scratcher->model.anim.status = ANIM_STATUS(HangedScratcherAnim_9, false);
                }
                else
                {
                    scratcher->model.controlState  = HangedScratcherControl_12;
                    scratcher->model.anim.status = ANIM_STATUS(HangedScratcherAnim_23, false);
                }

                break;

            case HangedScratcherControl_11:
                if (scratcher->model.anim.status == ANIM_STATUS(HangedScratcherAnim_12, true))
                {
                    scratcher->model.anim.status = ANIM_STATUS(HangedScratcherAnim_21, false);
                }

            case HangedScratcherControl_8:
                scratcher->fallSpeed += Q12_DIV(scratcher->damage.position.vy, Q12(4.0f));
                break;

            case HangedScratcherControl_12:
                if (scratcher->model.anim.status == ANIM_STATUS(HangedScratcherAnim_13, true))
                {
                    scratcher->model.anim.status = ANIM_STATUS(HangedScratcherAnim_22, false);
                }
        }

        scratcherProps.timer_EA  = 0;
        scratcherProps.flags |= HangedScratcherFlag_4;
    }

    Chara_DamageClear(scratcher);
}

void HangedScratcher_ControlUpdate(s_SubCharacter* scratcher)
{
    s32 animStatus;

    animStatus = scratcher->model.anim.status;
    if (animStatus != ANIM_STATUS(HangedScratcherAnim_4, true) &&
        animStatus != ANIM_STATUS(HangedScratcherAnim_5, true))
    {
        scratcher->field_44.field_0 = 0;
    }

    // Handle control state.
    switch (scratcher->model.controlState)
    {
        case HangedScratcherControl_None:
            break;

        case HangedScratcherControl_1:
            HangedScratcher_Control_1(scratcher);
            break;

        case HangedScratcherControl_2:
            HangedScratcher_ControlRunForward(scratcher);
            break;

        case HangedScratcherControl_RunForward:
            HangedScratcher_Control_3(scratcher);
            break;

        case HangedScratcherControl_4:
            HangedScratcher_Control_4(scratcher);
            break;

        case HangedScratcherControl_5:
            HangedScratcher_Control_5(scratcher);
            break;

        case HangedScratcherControl_6:
            HangedScratcher_Control_6(scratcher);
            break;

        case HangedScratcherControl_WalkForward:
            HangedScratcher_ControlWalkForward(scratcher);
            break;

        case HangedScratcherControl_8:
            HangedScratcher_Control_8(scratcher);
            break;

        case HangedScratcherControl_9:
            HangedScratcher_Control_9(scratcher);
            break;

        case HangedScratcherControl_10:
            HangedScratcher_Control_10(scratcher);
            break;

        case HangedScratcherControl_11:
            HangedScratcher_Control_11(scratcher);
            break;

        case HangedScratcherControl_12:
            HangedScratcher_Control_12(scratcher);
            break;

        case HangedScratcherControl_13:
            HangedScratcher_Control_13(scratcher);
            break;

        case HangedScratcherControl_14:
            HangedScratcher_Control_14(scratcher);
            break;

        case HangedScratcherControl_15:
            HangedScratcher_Control_15(scratcher);
            break;

        case HangedScratcherControl_16:
            HangedScratcher_Control_16(scratcher);
            break;
    }
}

void HangedScratcher_Control_16(s_SubCharacter* scratcher)
{
    if (scratcherProps.field_104 != 0)
    {
        scratcher->model.controlState = HangedScratcherControl_WalkForward;
        scratcher->model.anim.status  = ANIM_STATUS(HangedScratcherAnim_WalkForward, false);
    }
}

void HangedScratcher_Control_1(s_SubCharacter* scratcher)
{
    q19_12 distToPlayer;

    distToPlayer = Math_Vector2MagCalcSafeQ6(PLAYER_CHARA().position.vx - scratcher->position.vx,
                                       PLAYER_CHARA().position.vz - scratcher->position.vz);

    // TODO: Invert to return early instead?
    if (distToPlayer < Q12(1.0f) &&
        !Los_NpcToPlayerHitCheck(scratcher, &PLAYER_CHARA()) &&
        (((!WorldEnv_IsInDarkness() && WorldEnv_IsFlashlightAllowed()) && func_8006FD90(scratcher, 1, Q12(2.0f), Q12(1.5f))) ||
         (WorldEnv_IsFlashlightAllowed() && WorldEnv_IsInDarkness() &&
          func_8006FD90(scratcher, 1, Q12(1.5f), Q12(0.3f)))))
    {
        scratcher->model.controlState = HangedScratcherControl_RunForward;
        scratcher->model.anim.status  = ANIM_STATUS(HangedScratcherAnim_RunForward, false);
    }
}

void HangedScratcher_ControlRunForward(s_SubCharacter* chara)
{
    q19_12 distToPlayer;

    distToPlayer = Math_Vector2MagCalcSafeQ6(PLAYER_CHARA().position.vx - chara->position.vx,
                                             PLAYER_CHARA().position.vz - chara->position.vz);

    Chara_MoveSpeedUpdate(chara, Q12(4.0f));

    if (distToPlayer < Q12(5.0f) || (distToPlayer < Q12(20.0f) && Rng_Rand16() == 0))
    {
        chara->model.controlState = HangedScratcherControl_RunForward;
        chara->model.anim.status  = ANIM_STATUS(HangedScratcherAnim_RunForward, false);
    }
}

void HangedScratcher_Control_3(s_SubCharacter* scratcher)
{
    q19_12 distToPlayer;
    q3_12  angleDeltaToPlayer;
    q3_12  targetRotDelta;
    q19_12 playerPosYDelta;

    distToPlayer       = Math_Vector2MagCalcSafeQ6(PLAYER_CHARA().position.vx - scratcher->position.vx,
                                             PLAYER_CHARA().position.vz - scratcher->position.vz);
    angleDeltaToPlayer = Math_AngleNormalizeSigned(Math_AngleBetweenPositionsGet(scratcher->position, PLAYER_CHARA().position) -
                                                   scratcher->rotation.vy);
    angleDeltaToPlayer = ABS(angleDeltaToPlayer);

    if (angleDeltaToPlayer < TIMESTEP_ANGLE(1, 2))
    {
        Chara_MoveSpeedUpdate4(scratcher, Q12(4.0f), scratcherProps.radiusMax_10C);

        scratcherProps.timer_EA = Q12(0.0f);
    }
    else
    {
        if (angleDeltaToPlayer < Q12_ANGLE(60.0f) && distToPlayer > Q12(2.0f))
        {
            Chara_MoveSpeedUpdate4(scratcher, Q12(4.0f), scratcherProps.radiusMax_10C);
        }
        else if (angleDeltaToPlayer < Q12_ANGLE(120.0f) && distToPlayer > Q12(4.0f))
        {
            Chara_MoveSpeedUpdate4(scratcher, Q12(4.0f), scratcherProps.radiusMax_10C / 2);
        }
        else
        {
            Chara_MoveSpeedUpdate(scratcher, Q12(4.0f));
        }

        scratcherProps.timer_EA += g_DeltaTime;
        if (distToPlayer > Q12(7.5f) && scratcherProps.timer_EA > Q12(1.8f))
        {
            scratcherProps.targetHeadingAngle_EC = Los_CharaHeadingAngleGet(scratcher,
                                                                         Q12(5.0f),
                                                                         PLAYER_CHARA().position.vx,
                                                                         PLAYER_CHARA().position.vz,
                                                                         Q12_ANGLE(360.0f),
                                                                         true);
            if (scratcherProps.targetHeadingAngle_EC == Q12_ANGLE(360.0f))
            {
                scratcherProps.targetHeadingAngle_EC = scratcher->rotation.vy;
            }

            scratcherProps.timer_EA = Q12(0.0f);
        }
        else if (distToPlayer > Q12(3.0f) && scratcherProps.timer_EA > Q12(0.6f))
        {
            scratcherProps.targetHeadingAngle_EC = Los_CharaHeadingAngleGet(scratcher,
                                                                         Q12(1.5f),
                                                                         PLAYER_CHARA().position.vx,
                                                                         PLAYER_CHARA().position.vz,
                                                                         Q12_ANGLE(360.0f),
                                                                         true);
            if (scratcherProps.targetHeadingAngle_EC == Q12_ANGLE(360.0f))
            {
                scratcherProps.targetHeadingAngle_EC = scratcher->rotation.vy;
            }

            scratcherProps.timer_EA = Q12(0.0f);
        }
        else
        {
            scratcherProps.targetHeadingAngle_EC = Math_AngleBetweenPositionsGet(scratcher->position, PLAYER_CHARA().position);
            scratcherProps.timer_EA              = Q12(0.0f);
        }

        targetRotDelta = Math_AngleNormalizeSigned(scratcherProps.targetHeadingAngle_EC - scratcher->rotation.vy);
        if (ABS(targetRotDelta) > TIMESTEP_ANGLE(1, 2))
        {
            if (targetRotDelta > Q12_ANGLE(0.0f))
            {
                scratcher->rotation.vy += Q12_MULT_PRECISE(g_DeltaTime, Q12(0.5f));
            }
            else
            {
                scratcher->rotation.vy -= Q12_MULT_PRECISE(g_DeltaTime, Q12(0.5f));
            }
        }
    }

    if (sharedFunc_800D3214_5_s00(scratcher))
    {
        scratcher->model.controlState  = HangedScratcherControl_8;
        scratcher->model.anim.status = ANIM_STATUS(HangedScratcherAnim_16, false);
        scratcher->moveSpeed            = Q12(0.0f);

        scratcherProps.flags |= HangedScratcherFlag_1;
    }
    else
    {
        if (!Chara_HasFlag(&PLAYER_CHARA(), 8) && !(g_SysWork.charaGroupFlags[3] & CharaGroupFlag_1))
        {
            if (PLAYER_CHARA().health > Q12(0.0f) &&
                distToPlayer < Q12(1.0f) && angleDeltaToPlayer < Q12_ANGLE(30.0f))
            {
                playerPosYDelta = scratcher->position.vy - PLAYER_CHARA().position.vy;
                if (playerPosYDelta < Q12(-3.5f))
                {
                    if (playerPosYDelta > Q12(-3.7f))
                    {
                        scratcher->model.controlState  = HangedScratcherControl_4;
                        scratcherProps.timer_EA            = Q12(0.0f);
                        scratcher->model.anim.status = ANIM_STATUS(HangedScratcherAnim_4, false);
                        g_SysWork.charaGroupFlags[3]           |= HangedScratcherFlag_1;
                        return;
                    }
                }
            }
        }

        if (distToPlayer > Q12(20.0f) && !Rng_GenerateInt(0, 31)) // 1 in 32 chance.
        {
            scratcher->model.controlState  = HangedScratcherControl_2;
            scratcher->model.anim.status = ANIM_STATUS(HangedScratcherAnim_14, false);
        }
    }
}

void HangedScratcher_Control_4(s_SubCharacter* scratcher)
{
    VECTOR3 attackPos; // Q19.12
    q19_12  distToPlayer;
    q3_12   angleDeltaToPlayer;
    q3_12   angleDeltaToPlayerAbs;

    distToPlayer          = Math_Vector2MagCalcSafeQ6(PLAYER_CHARA().position.vx - scratcher->position.vx,
                                                PLAYER_CHARA().position.vz - scratcher->position.vz);
    angleDeltaToPlayer    = Math_AngleNormalizeSigned(Math_AngleBetweenPositionsGet(scratcher->position, PLAYER_CHARA().position) -
                                          scratcher->rotation.vy);
    angleDeltaToPlayerAbs = ABS(angleDeltaToPlayer);

    if (ANIM_TIME_RANGE_CHECK(scratcher->model.anim.time, 98, 103))
    {
        if (angleDeltaToPlayerAbs > TIMESTEP_ANGLE(1, 2))
        {
            if (angleDeltaToPlayer > Q12_ANGLE(0.0f))
            {
                scratcher->rotation.vy += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.5f);
            }
            else
            {
                scratcher->rotation.vy -= Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.5f);
            }
        }

        if (distToPlayer > Q12(0.2f))
        {
            scratcher->moveSpeed = Q12(1.0f);
        }
        else
        {
            scratcher->moveSpeed = Q12(0.0f);
        }

        scratcher->field_44.field_0 = 1;
    }
    else
    {
        scratcher->moveSpeed = Q12(0.0f);

        if (ANIM_TIME_RANGE_CHECK(scratcher->model.anim.time, 104, 105))
        {
            attackPos.vx             = scratcher->position.vx;
            attackPos.vy             = Q12(-1.45f);
            attackPos.vz             = scratcher->position.vz;
            scratcherProps.flags |= HangedScratcherFlag_5;

            func_8008A0E4(1, WEAPON_ATTACK(EquippedWeaponId_Unk44, AttackInputType_Tap), scratcher, &attackPos, &PLAYER_CHARA(), scratcher->rotation.vy,
                          ratan2(distToPlayer, (PLAYER_CHARA().position.vy + PLAYER_CHARA().collision.box.offsetY) - attackPos.vy));
        }
        else if (ANIM_STATUS_IDX_GET(scratcher->model.anim.status) == HangedScratcherAnim_14)
        {
            if (distToPlayer > Q12(6.0f) && !Rng_GenerateInt(0, 15)) // 1 in 16 chance.
            {
                scratcher->model.controlState  = HangedScratcherControl_RunForward;
                scratcherProps.timer_EA            = Q12(0.0f);
                scratcher->model.anim.status = ANIM_STATUS(HangedScratcherAnim_RunForward, false);
                g_SysWork.charaGroupFlags[3]           &= ~CharaGroupFlag_1;
            }
            else
            {
                scratcherProps.timer_EA += g_DeltaTime;

                if (scratcherProps.timer_EA > Q12(3.0f) && !Rng_GenerateInt(0, 7))
                {
                    scratcherProps.timer_EA           = Q12(0.0f);
                    scratcher->model.controlState = HangedScratcherControl_2;
                    g_SysWork.charaGroupFlags[3]          &= ~CharaGroupFlag_1;
                }
            }
        }
    }
}

void HangedScratcher_Control_5(s_SubCharacter* scratcher)
{
    if (Vw_AabbVisibleInScreenCheck(MIN_OFFSET(scratcher->position.vx, Q12(0.5f), Q12(0.5f)),
                                    MAX_OFFSET(scratcher->position.vx, Q12(0.5f), Q12(0.5f)),
                                    MIN_OFFSET(scratcher->position.vy, Q12(1.0f), Q12(0.0f)),
                                    MAX_OFFSET(scratcher->position.vy, Q12(1.0f), Q12(0.0f)),
                                    MIN_OFFSET(scratcher->position.vz, Q12(0.5f), Q12(0.5f)),
                                    MAX_OFFSET(scratcher->position.vz, Q12(0.5f), Q12(0.5f))) ||
        Math_Vector2MagCalcSafeQ6(PLAYER_CHARA().position.vx - scratcher->position.vx,
                            PLAYER_CHARA().position.vz - scratcher->position.vz) < Q12(1.0f))
    {
        if (Los_NpcToPlayerHitCheck(scratcher, &PLAYER_CHARA()))
        {
            return;
        }

        if (((!WorldEnv_IsInDarkness() && WorldEnv_IsFlashlightAllowed()) && func_8006FD90(scratcher, 1, Q12(1.5f), Q12(0.5f))) ||
            (WorldEnv_IsFlashlightAllowed() && WorldEnv_IsInDarkness() && func_8006FD90(scratcher, 1, Q12(1.2f), Q12(0.3f))))
        {
            scratcher->model.controlState = HangedScratcherControl_WalkForward;
            scratcher->model.anim.status  = ANIM_STATUS(HangedScratcherAnim_WalkForward, false);
        }
    }
}

void HangedScratcher_Control_6(s_SubCharacter* scratcher)
{
    q19_12 distToPlayer;
    u16    temp_v1;

    distToPlayer = Math_Vector2MagCalcSafeQ6(PLAYER_CHARA().position.vx - scratcher->position.vx,
                                       PLAYER_CHARA().position.vz - scratcher->position.vz);

    Chara_MoveSpeedUpdate(scratcher, Q12(4.0f));

    if (scratcherProps.timer_EA == Q12(0.0f) &&
        scratcher->model.anim.status == ANIM_STATUS(HangedScratcherAnim_15, true))
    {
        if (distToPlayer < Q12(12.0f) && !Rng_GenerateInt(0, 127)) // 1 in 128 chance.
        {
            scratcher->model.controlState = HangedScratcherControl_WalkForward;
            scratcher->model.anim.status  = ANIM_STATUS(HangedScratcherAnim_WalkForward, false);
        }
        else if (distToPlayer < Q12(6.0f) && !Rng_GenerateInt(0, 15)) // 1 in 16 chance.
        {
            scratcher->model.controlState = HangedScratcherControl_WalkForward;
            scratcher->model.anim.status  = ANIM_STATUS(HangedScratcherAnim_WalkForward, false);
        }
    }

    // @bug Weird function pointer check, code will never run?
    if (&sharedFunc_800D3214_5_s00 == NULL && distToPlayer > Q12(24.0f))
    {
        scratcher->model.controlState = HangedScratcherControl_None;
        scratcher->model.stateStep    = 3;
    }

    temp_v1                 = scratcherProps.timer_EA;
    scratcherProps.timer_EA = MAX(Q12(0.0f), temp_v1 - g_DeltaTime);
}

void HangedScratcher_ControlWalkForward(s_SubCharacter* scratcher)
{
    q19_12 distToPlayer;
    q3_12  angleDeltaToPlayer;
    q3_12  angleDeltaToPlayerAbs;
    q19_12 angleToPlayer;
    bool   hasLosToPlayer;
    q3_12  targetRotDelta;

    distToPlayer          = Math_Vector2MagCalcSafeQ6(PLAYER_CHARA().position.vx - scratcher->position.vx,
                                                      PLAYER_CHARA().position.vz - scratcher->position.vz);
    angleDeltaToPlayer    = Math_AngleNormalizeSigned(Math_AngleBetweenPositionsGet(scratcher->position, PLAYER_CHARA().position) -
                                                      scratcher->rotation.vy);
    angleDeltaToPlayerAbs = ABS(angleDeltaToPlayer);
    angleToPlayer         = Math_AngleBetweenPositionsGet(scratcher->position, PLAYER_CHARA().position);

    if (distToPlayer > Q12(16.0f))
    {
        scratcher->model.controlState = HangedScratcherControl_6;
        scratcherProps.timer_EA       = Q12(0.5f);
        scratcher->model.anim.status  = ANIM_STATUS(HangedScratcherAnim_15, false);
        scratcherProps.field_EE       = 37;
        return;
    }

    hasLosToPlayer = Los_NpcToPlayerHitCheck(scratcher, &PLAYER_CHARA());
    if (angleDeltaToPlayerAbs < TIMESTEP_ANGLE(1, 3) && !hasLosToPlayer)
    {
        Chara_MoveSpeedUpdate4(scratcher, Q12(2.0f), scratcherProps.radiusMin_10E);
        scratcherProps.timer_EA = Q12(0.0f);
    }
    else
    {
        if (!hasLosToPlayer)
        {
            if (angleDeltaToPlayerAbs < Q12_ANGLE(45.0f) && distToPlayer > Q12(2.0f))
            {
                Chara_MoveSpeedUpdate4(scratcher, Q12(2.0f), scratcherProps.radiusMin_10E);
            }
            else if (angleDeltaToPlayerAbs < Q12_ANGLE(90.0f) && distToPlayer > Q12(4.0f))
            {
                Chara_MoveSpeedUpdate4(scratcher, Q12(2.0f), scratcherProps.radiusMin_10E / 2);
            }
            else
            {
                Chara_MoveSpeedUpdate(scratcher, Q12(4.0f));
            }
        }
        else
        {
            Chara_MoveSpeedUpdate4(scratcher, Q12(2.0f), scratcherProps.radiusMin_10E);
        }

        scratcherProps.timer_EA += g_DeltaTime;

        if (distToPlayer < Q12(2.0f) && !hasLosToPlayer)
        {
            scratcherProps.timer_EA              = Q12(0.0f);
            scratcherProps.targetHeadingAngle_EC = angleToPlayer;
        }
        else if (scratcherProps.timer_EA > Q12(0.6f) ||
                 (scratcherProps.timer_EA > Q12(0.3f) &&
                  hasLosToPlayer && !Rng_GenerateInt(0, 7))) // 1 in 8 chance.
        {
            scratcherProps.timer_EA = Q12(0.0f);

            scratcherProps.targetHeadingAngle_EC = Los_CharaHeadingAngleGet(scratcher, Q12(1.0f), PLAYER_CHARA().position.vx, PLAYER_CHARA().position.vz, Q12_ANGLE(360.0f), true);
            if (scratcherProps.targetHeadingAngle_EC == Q12_ANGLE(360.0f))
            {
                scratcherProps.targetHeadingAngle_EC = scratcher->rotation.vy - Q12_ANGLE(180.0f);
            }
        }

        targetRotDelta = Math_AngleNormalizeSigned(scratcherProps.targetHeadingAngle_EC - scratcher->rotation.vy);
        if (ABS(targetRotDelta) > TIMESTEP_ANGLE(1, 3))
        {
            if (targetRotDelta > Q12_ANGLE(0.0f))
            {
                scratcher->rotation.vy += Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(90.0f));
            }
            else
            {
                scratcher->rotation.vy -= Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(90.0f));
            }
        }
        else
        {
            scratcher->rotation.vy = scratcherProps.targetHeadingAngle_EC;
        }
    }

    if (!hasLosToPlayer && !(g_SysWork.charaGroupFlags[3] & CharaGroupFlag_1) &&
        !(PLAYER_CHARA().flags & CharaFlag_Unk4) &&
        PLAYER_CHARA().health > Q12(0.0f))
    {
        if (Math_Vector2MagCalcSafeQ6(PLAYER_CHARA().position.vx - scratcher->position.vx,
                                PLAYER_CHARA().position.vz - scratcher->position.vz) < Q12(1.2f))
        {
            if (ABS(Math_AngleNormalizeSigned(Math_AngleBetweenPositionsGet(scratcher->position, PLAYER_CHARA().position) -
                                  scratcher->rotation.vy)) < Q12_ANGLE(20.0f))
            {
                scratcher->model.controlState = HangedScratcherControl_15;
                g_SysWork.charaGroupFlags[3] |= CharaGroupFlag_1;
                scratcherProps.timer_EA       = Q12(0.0f);
                scratcher->model.anim.status  = ANIM_STATUS(HangedScratcherAnim_5, false);
            }
        }
    }
}

void HangedScratcher_Control_8(s_SubCharacter* scratcher)
{
    if (scratcherProps.flags & HangedScratcherFlag_Airborne)
    {
        scratcher->fallSpeed += g_GravitySpeed >> 1;

        if (FP_FROM(scratcher->model.anim.time, Q12_SHIFT) == 259)
        {
            sharedData_800DE28C_5_s00 = Q12(0.0f);
        }
        else
        {
            sharedData_800DE28C_5_s00 = Q12(15.0f);
        }
    }
    else
    {
        sharedData_800DE28C_5_s00 = Q12(12.0f);
    }

    if (ANIM_TIME_RANGE_CHECK(scratcher->model.anim.time, 245, 256))
    {
        // Misuse of `TIMESTEP_SCALE_60_FPS`, should be `TIMESTEP_SCALE_30(g_DeltaTime, Q12(0.122f))`.
        scratcher->rotation.vz = MAX(Q12(0.0f), scratcher->rotation.vz - TIMESTEP_SCALE_60_FPS(g_DeltaTime, Q12(0.011f)));
    }

    if (ANIM_STATUS_IDX_GET(scratcher->model.anim.status) == HangedScratcherAnim_15)
    {
        scratcher->moveSpeed = Q12(0.0f);

        if (scratcher->health > Q12(100.0f))
        {
            scratcher->model.controlState = HangedScratcherControl_6;
            scratcherProps.timer_EA           = Q12(0.5f);
        }
        else
        {
            scratcher->model.controlState = HangedScratcherControl_12;
            scratcher->model.anim.status  = ANIM_STATUS(HangedScratcherAnim_11, false);
            scratcherProps.field_EE            = 31;
        }
    }
}

void HangedScratcher_Control_9(s_SubCharacter* scratcher)
{
    s_CollisionSurface surface;

    Chara_MoveSpeedUpdate(scratcher, Q12(4.0f));

    if (ANIM_STATUS_IDX_GET(scratcher->model.anim.status) == HangedScratcherAnim_14 &&
        scratcher->moveSpeed == Q12(0.0f))
    {
        Collision_SurfaceGet(&surface, scratcher->position.vx, scratcher->position.vz);
        if (scratcher->health > Q12(100.0f) || surface.floorType == FloorType_7)
        {
            if (scratcherProps.flags & HangedScratcherFlag_1)
            {
                scratcher->model.controlState = HangedScratcherControl_8;
                scratcher->model.anim.status  = ANIM_STATUS(HangedScratcherAnim_16, false);
            }
            else if (!Rng_GenerateInt(0, 7)) // 1 in 8 chance.
            {
                scratcher->model.controlState = HangedScratcherControl_RunForward;
                scratcher->model.anim.status  = ANIM_STATUS(HangedScratcherAnim_RunForward, false);
            }
        }
        else
        {
            scratcher->model.controlState = HangedScratcherControl_11;
            scratcher->model.anim.status  = ANIM_STATUS(HangedScratcherAnim_10, false);
        }
    }
}

void HangedScratcher_Control_10(s_SubCharacter* scratcher)
{
    Chara_MoveSpeedUpdate(scratcher, Q12(4.0f));

    if (ANIM_STATUS_IDX_GET(scratcher->model.anim.status) == HangedScratcherAnim_15 &&
        scratcher->moveSpeed == Q12(0.0f) &&
        Rng_GenerateInt(0, 7) == 0) // 1 in 8 chance.
    {
        scratcher->model.controlState = HangedScratcherControl_WalkForward;
        scratcher->model.anim.status  = ANIM_STATUS(HangedScratcherAnim_WalkForward, false);
    }
}

void HangedScratcher_Control_11(s_SubCharacter* scratcher)
{
    if (scratcherProps.flags & HangedScratcherFlag_Airborne)
    {
        scratcher->fallSpeed += g_GravitySpeed >> 1;

        if (ANIM_TIME_RANGE_CHECK(scratcher->model.anim.time, 176, 184))
        {
            sharedData_800DE28C_5_s00 = Q12(10.0f);
        }
        else
        {
            sharedData_800DE28C_5_s00 = Q12(0.0f);
        }
    }
    else
    {
        Chara_MoveSpeedUpdate(scratcher, Q12(4.0f));

        if (ANIM_STATUS_IDX_GET(scratcher->model.anim.status) == HangedScratcherAnim_10)
        {
            if (ANIM_TIME_RANGE_CHECK(scratcher->model.anim.time, 176, 185))
            {
                scratcher->model.anim.time        = Q12(186.0f);
                scratcher->model.anim.keyframeIdx = 186;
            }

            sharedData_800DE28C_5_s00 = Q12(8.0f);
        }
    }

    if (ANIM_TIME_RANGE_CHECK(scratcher->model.anim.time, 191, 193))
    {
        Chara_AnimSet(scratcher, ANIM_STATUS(HangedScratcherAnim_12, true), 202);
        scratcher->rotation.vz = Q12_ANGLE(0.0f);
        return;
    }

    if (scratcher->model.anim.status == ANIM_STATUS(HangedScratcherAnim_12, true))
    {
        scratcher->flags |= CharaFlag_Unk2;

        if (scratcher->health <= Q12(0.0f))
        {
            scratcher->model.controlState = HangedScratcherControl_13;
            scratcher->model.anim.status  = ANIM_STATUS(HangedScratcherAnim_6, false);
        }
    }
}

void HangedScratcher_Control_12(s_SubCharacter* scratcher)
{
    q19_12 duration;
    q19_12 durationScaled;

    Chara_MoveSpeedUpdate(scratcher, Q12(4.0f));

    if (ANIM_TIME_RANGE_CHECK(scratcher->model.anim.time, 192, 199))
    {
        duration       = HANGED_SCRATCHER_ANIM_INFOS[scratcher->model.anim.status].duration.constant;
        durationScaled = (Q12_MULT_PRECISE(duration, g_DeltaTime) * Q12(0.7f)) / Q12(7.0f);

        scratcherProps.offsetX_F0 += FP_FROM(durationScaled * Math_Sin(scratcher->rotation.vy), Q12_SHIFT);
        scratcherProps.offsetZ_F2 += FP_FROM(durationScaled * Math_Cos(scratcher->rotation.vy), Q12_SHIFT);
    }

    if (scratcher->model.anim.status == ANIM_STATUS(HangedScratcherAnim_13, true))
    {
        scratcher->flags |= CharaFlag_Unk2;

        if (scratcher->health <= Q12(0.0f))
        {
            scratcher->model.controlState  = HangedScratcherControl_13;
            scratcher->model.anim.status = ANIM_STATUS(HangedScratcherAnim_7, false);
        }
    }
}

void HangedScratcher_Control_13(s_SubCharacter* scratcher)
{
    Chara_MoveSpeedUpdate(scratcher, Q12(4.0f));

    if (scratcher->health == Q12(0.0f))
    {
        if (g_SysWork.targetNpcIdx != Chara_NpcIdxGet(scratcher))
        {
            scratcher->health          = NO_VALUE;
            scratcher->flags          &= ~CharaFlag_Unk2;
            scratcher->collision.state = CharaCollisionState_Ignore;
            scratcherProps.flags      |= HangedScratcherFlag_9;
        }
    }

    // TODO: Weird 32-bit flags access, maybe `if` is checking both `flags` and `field_EA`?
    if (scratcher->moveSpeed == Q12(0.0f) &&
        !(*(u32*)&scratcherProps.flags & (HangedScratcherFlag_Airborne | HangedScratcherFlag_2)))
    {
        func_800622B8(3, scratcher, ANIM_STATUS(HangedScratcherAnim_4, false), 8);
        scratcherProps.flags |= HangedScratcherFlag_2;
        Savegame_EnemyStateUpdate(scratcher);
    }
}

void HangedScratcher_Control_14(s_SubCharacter* scratcher)
{
    s_CollisionSurface surface;
    VECTOR3            sfxPos;

    if ((scratcherProps.flags & HangedScratcherFlag_7) &&
        scratcher->model.anim.status < ANIM_STATUS(HangedScratcherAnim_4, false))
    {
        HANGED_SCRATCHER_ANIM_INFOS[scratcher->model.anim.status].duration.constant = Q12(9.6f);
    }
    else
    {
        HANGED_SCRATCHER_ANIM_INFOS[scratcher->model.anim.status].duration.constant = Q12(0.0f);
    }

    if (FP_FROM(scratcher->model.anim.time, Q12_SHIFT) == 51)
    {
        scratcher->model.anim.flags &= ~AnimFlag_Visible;
    }
    else
    {
        scratcher->model.anim.flags |= AnimFlag_Visible;
    }

    if (!(scratcherProps.flags & HangedScratcherFlag_6))
    {
        if (ANIM_TIME_RANGE_CHECK(scratcher->model.anim.time, 42, 44))
        {
            scratcherProps.flags |= HangedScratcherFlag_6;

            Collision_SurfaceGet(&surface, scratcher->position.vx, scratcher->position.vz);
            if (surface.floorType == FloorType_11)
            {
                Sfx_WithFlagsPlay(Sfx_Unk1582, &scratcher->position, Q8(0.5f), SfxFlag_None);
            }
            else
            {
                Sfx_WithFlagsPlay(Sfx_Unk1581, &scratcher->position, Q8(0.5f), SfxFlag_None);
            }
        }

        if (ANIM_TIME_RANGE_CHECK(scratcher->model.anim.time, 84, 86))
        {
            scratcherProps.flags |= HangedScratcherFlag_6;

            sfxPos.vx = scratcher->position.vx;
            sfxPos.vy = scratcher->position.vy;
            sfxPos.vz = scratcher->position.vz + Q12(0.6f);

            Collision_SurfaceGet(&surface, sfxPos.vx, sfxPos.vz);
            if (surface.floorType == FloorType_11)
            {
                Sfx_WithFlagsPlay(Sfx_Unk1582, &sfxPos, Q8(0.5f), SfxFlag_None);
            }
            else
            {
                Sfx_WithFlagsPlay(Sfx_Unk1581, &sfxPos, Q8(0.5f), SfxFlag_None);
            }
        }

        if (ANIM_TIME_RANGE_CHECK(scratcher->model.anim.time, 94, 96))
        {
            scratcherProps.flags |= HangedScratcherFlag_6;

            sfxPos.vx = scratcher->position.vx;
            sfxPos.vy = scratcher->position.vy;
            sfxPos.vz = scratcher->position.vz - Q12(1.0f);

            Collision_SurfaceGet(&surface, sfxPos.vx, sfxPos.vz);
            if (surface.floorType == FloorType_11)
            {
                Sfx_WithFlagsPlay(Sfx_Unk1582, &sfxPos, Q8(0.5f), SfxFlag_None);
            }
            else
            {
                Sfx_WithFlagsPlay(Sfx_Unk1581, &sfxPos, Q8(0.5f), SfxFlag_None);
            }
        }
    }

    if (!(scratcherProps.flags & HangedScratcherFlag_8) &&
        ANIM_TIME_RANGE_CHECK(scratcher->model.anim.time, 52, 54))
    {
        scratcherProps.flags |= HangedScratcherFlag_8;

        sfxPos.vx = scratcher->position.vx;
        sfxPos.vy = scratcher->position.vy + Q12(0.5f);
        sfxPos.vz = scratcher->position.vz + Q12(1.5f);
        Sfx_WithFlagsPlay(Sfx_Unk1577, &sfxPos, Q8(0.5f), SfxFlag_None);
    }
}

void HangedScratcher_Control_15(s_SubCharacter* scratcher)
{
    VECTOR3 attackPos;
    q19_12  distToPlayer;
    q3_12   angleDeltaToPlayer;

    distToPlayer       = Math_Vector2MagCalcSafeQ6(PLAYER_CHARA().position.vx - scratcher->position.vx,
                                                   PLAYER_CHARA().position.vz - scratcher->position.vz);
    angleDeltaToPlayer = Math_AngleNormalizeSigned(Math_AngleBetweenPositionsGet(scratcher->position, PLAYER_CHARA().position) -
                                                   scratcher->rotation.vy);

    if (ANIM_TIME_RANGE_CHECK(scratcher->model.anim.time, 116, 123) &&
        distToPlayer > Q12(1.0f) && angleDeltaToPlayer < Q12_ANGLE(20.0f))
    {
        scratcher->moveSpeed = Q12(1.0f);
    }
    else
    {
        scratcher->moveSpeed = Q12(0.0f);
    }

    if (ANIM_TIME_RANGE_CHECK(scratcher->model.anim.time, 116, 128))
    {
        if (ABS(angleDeltaToPlayer) > TIMESTEP_ANGLE(1, 4))
        {
            if (angleDeltaToPlayer > Q12_ANGLE(0.0f))
            {
                scratcher->rotation.vy += Q12_MULT_PRECISE(g_DeltaTime, Q12(0.125f));
            }
            else
            {
                scratcher->rotation.vy -= Q12_MULT_PRECISE(g_DeltaTime, Q12(0.125f));
            }
        }
        else
        {
            scratcher->rotation.vy = ratan2(PLAYER_CHARA().position.vx - scratcher->position.vx,
                                            PLAYER_CHARA().position.vz - scratcher->position.vz);
        }
    }

    if (ANIM_TIME_RANGE_CHECK(scratcher->model.anim.time, 124, 130))
    {
        if (scratcher->field_44.field_0 == 0)
        {
            scratcher->field_44.field_0 = 1;
        }

        attackPos.vx = scratcher->position.vx;
        attackPos.vy = scratcher->position.vy - Q12(0.8f);
        attackPos.vz = scratcher->position.vz;

        scratcherProps.flags |= HangedScratcherFlag_5;

        func_8008A0E4(1, WEAPON_ATTACK(EquippedWeaponId_Unk45, AttackInputType_Tap), scratcher, &attackPos, &PLAYER_CHARA(), scratcher->rotation.vy, Q12_ANGLE(90.0f));
    }

    if (scratcher->model.anim.status == ANIM_STATUS(HangedScratcherAnim_15, true))
    {
        if (scratcherProps.timer_EA == Q12(0.0f))
        {
            g_SysWork.charaGroupFlags[3] &= ~CharaGroupFlag_1;
        }

        scratcherProps.timer_EA += g_DeltaTime;

        if (scratcherProps.timer_EA > Q12(2.5f) ||
            (scratcherProps.timer_EA > Q12(0.8f) &&
             !Rng_GenerateInt(0, 31))) // 1 in 32 chance.
        {
            scratcher->model.controlState = HangedScratcherControl_WalkForward;
            scratcher->model.anim.status  = ANIM_STATUS(HangedScratcherAnim_WalkForward, false);
        }
    }
    else
    {
        scratcherProps.timer_EA = Q12(0.0f);
    }
}

void sharedFunc_800D26D8_5_s00(s_SubCharacter* scratcher)
{
    s_CollisionResult  collResult;
    s_CollisionSurface surface;
    u8                 flag1Val;

    scratcher->rotation.vy  = Math_AngleNormalizeSigned(scratcher->rotation.vy);
    scratcher->headingAngle = scratcher->rotation.vy;

    // Fall.
    switch (scratcher->model.controlState)
    {
        case HangedScratcherControl_5:
        case HangedScratcherControl_6:
        case HangedScratcherControl_WalkForward:
        case HangedScratcherControl_10:
        case HangedScratcherControl_12:
        case HangedScratcherControl_13:
            scratcher->fallSpeed += g_GravitySpeed;
            break;
    }

    Chara_MovementUpdate1(scratcher, &collResult, scratcherProps.offsetX_F0, scratcherProps.offsetZ_F2);

    scratcherProps.offsetZ_F2 = Q12(0.0f);
    scratcherProps.offsetX_F0 = Q12(0.0f);

    flag1Val = scratcherProps.flags & HangedScratcherFlag_Airborne;

    if (scratcher->position.vy == collResult.surface.floorHeight)
    {
        scratcherProps.flags &= ~HangedScratcherFlag_Airborne;

        if (flag1Val)
        {
            if (scratcher->model.anim.status == ANIM_STATUS(HangedScratcherAnim_10, true))
            {
                Collision_SurfaceGet(&surface, scratcher->position.vx, scratcher->position.vz);

                if (surface.floorType == FloorType_11)
                {
                    Sfx_WithFlagsPlay(Sfx_Unk1582, &scratcher->position, Q8(0.5f), SfxFlag_None);
                }
                else
                {
                    Sfx_WithFlagsPlay(Sfx_Unk1581, &scratcher->position, Q8(0.5f), SfxFlag_None);
                }
            }
            else if (scratcher->model.anim.status == ANIM_STATUS(HangedScratcherAnim_16, true))
            {
                Collision_SurfaceGet(&surface, scratcher->position.vx, scratcher->position.vz);

                if (surface.floorType == FloorType_11)
                {
                    Sfx_WithFlagsPlay(Sfx_Unk1582, &scratcher->position, Q8(0.25f), SfxFlag_None);
                }
                else
                {
                    Sfx_WithFlagsPlay(Sfx_Unk1581, &scratcher->position, Q8(0.25f), SfxFlag_None);
                }
            }
        }
    }
    else
    {
        scratcherProps.flags |= HangedScratcherFlag_Airborne;
    }
}

void sharedFunc_800D2844_5_s00(s_SubCharacter* scratcher, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords)
{
    s_AnimInfo* animInfo;
    q19_12      newAnimDur;

    #define curAnimInfo HANGED_SCRATCHER_ANIM_INFOS[scratcher->model.anim.status]

    scratcher->rotation.vy = Math_AngleNormalizeSigned(scratcher->rotation.vy);

    switch (scratcher->model.anim.status)
    {
        case ANIM_STATUS(HangedScratcherAnim_4, true):
            if (g_SavegamePtr->gameDifficulty == GameDifficulty_Normal)
            {
                curAnimInfo.duration.constant = Q12(19.5f);
            }
            else if (g_SavegamePtr->gameDifficulty == GameDifficulty_Easy)
            {
                curAnimInfo.duration.constant = Q12(19.5f);
            }
            else
            {
                curAnimInfo.duration.constant = Q12(26.0f);
            }
            break;

        case ANIM_STATUS(HangedScratcherAnim_5, false):
            if (g_SavegamePtr->gameDifficulty == GameDifficulty_Normal)
            {
                curAnimInfo.duration.constant = Q12(12.0f);
            }
            else if (g_SavegamePtr->gameDifficulty == GameDifficulty_Easy)
            {
                curAnimInfo.duration.constant = Q12(8.0f);
            }
            else
            {
                curAnimInfo.duration.constant = Q12(18.0f);
            }
            break;

        case ANIM_STATUS(HangedScratcherAnim_5, true):
            if (g_SavegamePtr->gameDifficulty == GameDifficulty_Normal)
            {
                curAnimInfo.duration.constant = Q12(23.0f);
            }
            else if (g_SavegamePtr->gameDifficulty == GameDifficulty_Easy)
            {
                curAnimInfo.duration.constant = Q12(15.0f);
            }
            else
            {
                curAnimInfo.duration.constant = Q12(34.0f);
            }
            break;

        case ANIM_STATUS(HangedScratcherAnim_9, true):
            if (FP_FROM(scratcher->model.anim.time, Q12_SHIFT) != 173)
            {
                curAnimInfo.duration.constant = Q12(12.0f);
                break;
            }

            curAnimInfo.duration.constant = Q12(4.5f);
            break;

        case ANIM_STATUS(HangedScratcherAnim_23, true):
            if (FP_FROM(scratcher->model.anim.time, Q12_SHIFT) == 173)
            {
                curAnimInfo.duration.constant = Q12(3.0f);
            }
            else
            {
                curAnimInfo.duration.constant = Q12(9.0f);
            }
            break;

        case ANIM_STATUS(HangedScratcherAnim_11, true):
            if (ANIM_TIME_RANGE_CHECK(scratcher->model.anim.time, 192, 193))
            {
                curAnimInfo.duration.constant = Q12(2.5f);
            }
            else
            {
                curAnimInfo.duration.constant = Q12(4.5f);
            }
            break;

        case ANIM_STATUS(HangedScratcherAnim_10, true):
        case ANIM_STATUS(HangedScratcherAnim_16, true):
            curAnimInfo.duration.constant = sharedData_800DE28C_5_s00;
            break;

        case ANIM_STATUS(HangedScratcherAnim_14, true):
        case ANIM_STATUS(HangedScratcherAnim_15, true):
            switch (scratcher->model.controlState)
            {
                case 1:
                    curAnimInfo.duration.constant = Q12(1.2f);
                    break;

                case 5:
                    curAnimInfo.duration.constant = Q12(2.4f);
                    break;

                default:
                    curAnimInfo.duration.constant = Q12(3.5f);
                    break;
            }
            break;

        case ANIM_STATUS(HangedScratcherAnim_RunForward, true):
            curAnimInfo.duration.constant = MAX(Q12(2.0f), Q12_MULT_PRECISE(scratcher->moveSpeed, Q12(4.4f)));
            break;

        case ANIM_STATUS(HangedScratcherAnim_WalkForward, true):
            newAnimDur = Q12_MULT_PRECISE(scratcher->moveSpeed, Q12(6.0f));
            if (scratcher->rotation.vy != scratcherProps.field_FC)
            {
                newAnimDur = MAX(Q12(9.0f), newAnimDur);
            }

            curAnimInfo.duration.constant = newAnimDur;
            break;
    }

    Math_MatrixTransform(&scratcher->position, &scratcher->rotation, boneCoords);

    animInfo = &curAnimInfo;
    animInfo->playbackFunc(&scratcher->model, anmHdr, boneCoords, animInfo);
    scratcherProps.field_FC = scratcher->rotation.vy;

    #undef curAnimInfo
}

// Likely internal to this func, can keep in .c instead of .h.
extern s_CharaKeyframe sharedData_800D9500_5_s00[];
extern s_CharaKeyframe sharedData_800D9654_5_s00;
extern s_CharaKeyframe sharedData_800D9668_5_s00;
extern s_CharaKeyframe sharedData_800D967C_5_s00[];
extern s_CharaKeyframe sharedData_800D96F4_5_s00;
extern s_CharaKeyframe sharedData_800D9708_5_s00[];
extern s_CharaKeyframe sharedData_800D97F8_5_s00;
extern s_CharaKeyframe sharedData_800D980C_5_s00;
extern s_CharaKeyframe sharedData_800D9820_5_s00[];
extern s_CharaKeyframe sharedData_800D99D8_5_s00;
extern s_CharaKeyframe sharedData_800D99EC_5_s00;
extern s_CharaKeyframe sharedData_800D9A00_5_s00[];
extern s_CharaKeyframe sharedData_800D9A64_5_s00;
extern s_CharaKeyframe sharedData_800D9A78_5_s00[];
extern s_CharaKeyframe sharedData_800D9AF0_5_s00;
extern s_CharaKeyframe sharedData_800D9B04_5_s00;
extern s_CharaKeyframe sharedData_800D9B18_5_s00;
extern s_CharaKeyframe sharedData_800D9B2C_5_s00[];

void HangedScratcher_CollisionUpdate(s_SubCharacter* scratcher)
{
    s32 relKeyframeIdx;
    s32 keyframeIdx0;
    s32 keyframeIdx1;

    #define animKeyframeIdx FP_FROM(scratcher->model.anim.time, Q12_SHIFT)

    // Handle animation status.
    switch (scratcher->model.anim.status)
    {
        case ANIM_STATUS(HangedScratcherAnim_4, false):
            Collision_CharaCollisionSet(scratcher, &sharedData_800D980C_5_s00, sharedData_800D9500_5_s00);
            break;

        case ANIM_STATUS(HangedScratcherAnim_4, true):
            scratcherProps.field_EE = 9;

            relKeyframeIdx = animKeyframeIdx - 98;
            Collision_CharaCollisionSet(scratcher, &sharedData_800D9500_5_s00[relKeyframeIdx], &sharedData_800D9500_5_s00[relKeyframeIdx + 1]);
            break;

        case ANIM_STATUS(HangedScratcherAnim_10, false):
            Collision_CharaCollisionSet(scratcher, &sharedData_800D980C_5_s00, sharedData_800D9708_5_s00);
            break;

        case ANIM_STATUS(HangedScratcherAnim_10, true):
            scratcherProps.field_EE = 21;

            relKeyframeIdx = animKeyframeIdx - 176;
            keyframeIdx0   = relKeyframeIdx - ((relKeyframeIdx >= 12) ? 1 : 0) - ((relKeyframeIdx >= 14) ? 1 : 0);
            keyframeIdx1   = (relKeyframeIdx + 1) - ((relKeyframeIdx >= 11) ? 1 : 0) - ((relKeyframeIdx >= 13) ? 1 : 0) - ((relKeyframeIdx >= 14) ? 1 : 0);

            Collision_CharaCollisionSet(scratcher, &sharedData_800D9708_5_s00[keyframeIdx0], &sharedData_800D9708_5_s00[keyframeIdx1]);
            break;

        case ANIM_STATUS(HangedScratcherAnim_12, false):
            if (scratcherProps.field_EE == 21)
            {
                Collision_CharaCollisionSet(scratcher, &sharedData_800D97F8_5_s00, &sharedData_800D9668_5_s00);
            }
            else
            {
                Chara_CollisionSet(scratcher, sharedData_800D9668_5_s00);
            }
            break;

        case ANIM_STATUS(HangedScratcherAnim_21, true):
            scratcherProps.field_EE = 43;

        case ANIM_STATUS(HangedScratcherAnim_6, false):
        case ANIM_STATUS(HangedScratcherAnim_6, true):
        case ANIM_STATUS(HangedScratcherAnim_12, true):
        case ANIM_STATUS(HangedScratcherAnim_19, false):
        case ANIM_STATUS(HangedScratcherAnim_19, true):
        case ANIM_STATUS(HangedScratcherAnim_21, false):
            Chara_CollisionSet(scratcher, sharedData_800D9668_5_s00);
            break;

        case ANIM_STATUS(HangedScratcherAnim_8, false):
            Chara_CollisionSet(scratcher, sharedData_800D967C_5_s00[0]);
            break;

        case ANIM_STATUS(HangedScratcherAnim_8, true):
            scratcherProps.field_EE = 17;

            relKeyframeIdx = animKeyframeIdx - 154;

            // TODO: Could these `if`s be flattened?
            if (relKeyframeIdx < 7)
            {
                if (relKeyframeIdx < 3)
                {
                    keyframeIdx0 = (relKeyframeIdx == 2 ? 1 : 0);
                }
                else
                {
                    keyframeIdx0 = 2;
                }
            }
            else
            {
                keyframeIdx0 = animKeyframeIdx - 158;
            }

            if (relKeyframeIdx < 6)
            {
                if (relKeyframeIdx >= 3)
                {
                    keyframeIdx1 = 2;
                }
                else
                {
                    keyframeIdx1 = relKeyframeIdx;
                }
            }
            else
            {
                keyframeIdx1 = (relKeyframeIdx - 3) - ((relKeyframeIdx == 10) ? 1 : 0);
            }

            Collision_CharaCollisionSet(scratcher, &sharedData_800D967C_5_s00[keyframeIdx0], &sharedData_800D967C_5_s00[keyframeIdx1]);
            break;

        case ANIM_STATUS(HangedScratcherAnim_14, false):
            if (scratcherProps.field_EE == 9)
            {
                Collision_CharaCollisionSet(scratcher, &sharedData_800D9654_5_s00, &sharedData_800D980C_5_s00);
            }
            else if (scratcherProps.field_EE == 17)
            {
                Collision_CharaCollisionSet(scratcher, &sharedData_800D96F4_5_s00, &sharedData_800D980C_5_s00);
            }
            else
            {
                Chara_CollisionSet(scratcher, sharedData_800D980C_5_s00);
            }
            break;

        case ANIM_STATUS(HangedScratcherAnim_RunForward, true):
            scratcherProps.field_EE = 35;

        case ANIM_STATUS(HangedScratcherAnim_14, true):
        case ANIM_STATUS(HangedScratcherAnim_RunForward, false):
            Chara_CollisionSet(scratcher, sharedData_800D980C_5_s00);
            break;

        case ANIM_STATUS(HangedScratcherAnim_16, false):
            Collision_CharaCollisionSet(scratcher, &sharedData_800D980C_5_s00, sharedData_800D9820_5_s00);
            break;

        case ANIM_STATUS(HangedScratcherAnim_16, true):
            scratcherProps.field_EE = 33;

            relKeyframeIdx = animKeyframeIdx - 244;
            Collision_CharaCollisionSet(scratcher, &sharedData_800D9820_5_s00[relKeyframeIdx], &sharedData_800D9820_5_s00[relKeyframeIdx + 1]);
            break;

        case ANIM_STATUS(HangedScratcherAnim_13, false):
            if (scratcherProps.field_EE == 23)
            {
                Collision_CharaCollisionSet(scratcher, &sharedData_800D9AF0_5_s00, &sharedData_800D99EC_5_s00);
            }
            else
            {
                Chara_CollisionSet(scratcher, sharedData_800D99EC_5_s00);
            }
            break;

        case ANIM_STATUS(HangedScratcherAnim_22, true):
            scratcherProps.field_EE = 45;

        case ANIM_STATUS(HangedScratcherAnim_7, false):
        case ANIM_STATUS(HangedScratcherAnim_7, true):
        case ANIM_STATUS(HangedScratcherAnim_13, true):
        case ANIM_STATUS(HangedScratcherAnim_20, false):
        case ANIM_STATUS(HangedScratcherAnim_20, true):
        case ANIM_STATUS(HangedScratcherAnim_22, false):
            Chara_CollisionSet(scratcher, sharedData_800D99EC_5_s00);
            break;

        case ANIM_STATUS(HangedScratcherAnim_9, false):
        case ANIM_STATUS(HangedScratcherAnim_23, false):
            Chara_CollisionSet(scratcher, sharedData_800D9A00_5_s00[0]);
            break;

        case ANIM_STATUS(HangedScratcherAnim_9, true):
        case ANIM_STATUS(HangedScratcherAnim_23, true):
            scratcherProps.field_EE = 19;

            relKeyframeIdx = animKeyframeIdx - 166;
            keyframeIdx0   = relKeyframeIdx - ((relKeyframeIdx > 0) ? 1 : 0) - ((relKeyframeIdx >= 4) ? 1 : 0) - ((relKeyframeIdx >= 5) ? 1 : 0);
            keyframeIdx1   = relKeyframeIdx - ((relKeyframeIdx >= 3) ? 1 : 0) - ((relKeyframeIdx >= 4) ? 1 : 0) - ((relKeyframeIdx >= 8) ? 1 : 0);

            Collision_CharaCollisionSet(scratcher, &sharedData_800D9A00_5_s00[keyframeIdx0], &sharedData_800D9A00_5_s00[keyframeIdx1]);
            break;

        case ANIM_STATUS(HangedScratcherAnim_11, false):
            if (scratcherProps.field_EE == 31)
            {
                Collision_CharaCollisionSet(scratcher, &sharedData_800D9B04_5_s00, sharedData_800D9A78_5_s00);
            }
            else
            {
                Collision_CharaCollisionSet(scratcher, &sharedData_800D9A64_5_s00, sharedData_800D9A78_5_s00);
            }
            break;

        case ANIM_STATUS(HangedScratcherAnim_11, true):
            scratcherProps.field_EE = 23;

            relKeyframeIdx = animKeyframeIdx - 192;
            keyframeIdx0   = relKeyframeIdx - ((relKeyframeIdx >= 2) ? 1 : 0) - ((relKeyframeIdx >= 8) ? 1 : 0);
            keyframeIdx1   = (relKeyframeIdx + 1) - ((relKeyframeIdx > 0) ? 1 : 0) - ((relKeyframeIdx >= 7) ? 1 : 0) - ((relKeyframeIdx >= 8) ? 1 : 0);

            Collision_CharaCollisionSet(scratcher, &sharedData_800D9A78_5_s00[keyframeIdx0], &sharedData_800D9A78_5_s00[keyframeIdx1]);
            break;

        case ANIM_STATUS(HangedScratcherAnim_15, false):
            if (scratcherProps.field_EE == 33)
            {
                Collision_CharaCollisionSet(scratcher, &sharedData_800D99D8_5_s00, &sharedData_800D9B04_5_s00);
            }
            else if (scratcherProps.field_EE == 19)
            {
                Collision_CharaCollisionSet(scratcher, &sharedData_800D9A64_5_s00, &sharedData_800D9B04_5_s00);
            }
            else
            {
                Chara_CollisionSet(scratcher, sharedData_800D9B04_5_s00);
            }
            break;

        case ANIM_STATUS(HangedScratcherAnim_15, true):
        case ANIM_STATUS(HangedScratcherAnim_WalkForward, false):
        case ANIM_STATUS(HangedScratcherAnim_WalkForward, true):
            Chara_CollisionSet(scratcher, sharedData_800D9B04_5_s00);
            break;

        case ANIM_STATUS(HangedScratcherAnim_JumpDownFromWall, false):
        case ANIM_STATUS(HangedScratcherAnim_JumpDownFromWall, true):
        case ANIM_STATUS(HangedScratcherAnim_EmergeFromWater, false):
        case ANIM_STATUS(HangedScratcherAnim_EmergeFromWater, true):
        case ANIM_STATUS(HangedScratcherAnim_JumpDownFromCeiling, false):
        case ANIM_STATUS(HangedScratcherAnim_JumpDownFromCeiling, true):
        case ANIM_STATUS(HangedScratcherAnim_24, false):
        case ANIM_STATUS(HangedScratcherAnim_24, true):
        case ANIM_STATUS(HangedScratcherAnim_25, false):
        case ANIM_STATUS(HangedScratcherAnim_25, true):
        case ANIM_STATUS(HangedScratcherAnim_26, false):
        case ANIM_STATUS(HangedScratcherAnim_26, true):
            Chara_CollisionSet(scratcher, sharedData_800D9B18_5_s00);
            break;

        case ANIM_STATUS(HangedScratcherAnim_5, false):
            Chara_CollisionSet(scratcher, sharedData_800D9B2C_5_s00[0]);
            break;

        case ANIM_STATUS(HangedScratcherAnim_5, true):
            relKeyframeIdx = animKeyframeIdx - 116;
            Collision_CharaCollisionSet(scratcher, &sharedData_800D9B2C_5_s00[relKeyframeIdx], &sharedData_800D9B2C_5_s00[relKeyframeIdx + 1]);
            break;
    }

    Chara_CollisionShapeOffsetsUpdate(&scratcher->collision.shapeOffsets, scratcher);

    if (!(scratcherProps.flags & HangedScratcherFlag_9))
    {
        if (!(scratcher->flags & (1 << 1)) && scratcher->health > Q12(0.0f) &&
            (scratcherProps.flags & HangedScratcherFlag_1) && (scratcherProps.flags & HangedScratcherFlag_Airborne))
        {
            scratcher->collision.state = CharaCollisionState_4;
        }
        else
        {
            scratcher->collision.state = CharaCollisionState_2;
        }
    }

    #undef animKeyframeIdx
}

bool sharedFunc_800D3214_5_s00(s_SubCharacter* scratcher)
{
    s_CollisionSurface surface;

    Collision_SurfaceGet(&surface, scratcher->position.vx, scratcher->position.vz);

#ifdef MAP5_S00
    if (surface.floorType != FloorType_7)
    {
        if (scratcher->position.vx > Q12(-40.0f))
        {
            return scratcher->position.vz > Q12(-9.0f);
        }
        else if (scratcher->position.vx > Q12(-80.0f))
        {
            if (scratcher->position.vx <= Q12(-57.0f))
            {
                if (scratcher->position.vx >= Q12(-62.0f))
                {
                    if (scratcher->position.vz > Q12(-10.0f))
                    {
                        return true;
                    }

                    return scratcher->position.vz < Q12(-14.0f);
                }
            }
        }
        else if (scratcher->position.vz <= Q12(-89.0f))
        {
            return (scratcher->position.vx >= Q12(-89.0f));
        }
    }

    return false;
#else
    // M6S03 version.
    if (surface.floorType != FloorType_7)
    {
        if (scratcher->position.vx < 0)
        {
            if (scratcher->position.vz < Q12(38.0f))
            {
                return true;
            }

            if (scratcher->position.vz < Q12(57.0f))
            {
                return false;
            }

            if (scratcher->position.vz <= Q12(63.0f))
            {
                if (scratcher->position.vx >= Q12(-22.0f))
                {
                    return scratcher->position.vx > Q12(-18.0f);
                }
            }
            else
            {
                return false;
            }
        }
        else if (scratcher->position.vz > Q12(40.0f))
        {
            if (scratcher->position.vz <= Q12(63.0f))
            {
                if (scratcher->position.vz >= Q12(57.0f))
                {
                    if (scratcher->position.vx >= Q12(18.0f))
                    {
                        return scratcher->position.vx > Q12(22.0f);
                    }

                    return true;
                }
            }

            return false;
        }
        else if (scratcher->position.vz <= Q12(23.0f))
        {
            if (scratcher->position.vz >= Q12(17.0f))
            {
                if (scratcher->position.vx >= Q12(18.0f))
                {
                    return scratcher->position.vx > Q12(22.0f);
                }

                return true;
            }

            return false;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    return true;
#endif
}

void sharedFunc_800D3300_5_s00(s_SubCharacter* scratcher)
{
    #define animKeyframeIdx FP_FROM(scratcher->model.anim.time, Q12_SHIFT)

    if (scratcherProps.timer_100 != Q12(0.0f))
    {
        if (scratcherProps.timer_100 > Q12(0.0f))
        {
            scratcherProps.timer_100 -= g_DeltaTime;
            if (scratcherProps.timer_100 <= Q12(0.0f))
            {
                scratcherProps.timer_100 = Q12(0.0f);
            }
        }
        else
        {
            scratcherProps.timer_100 += g_DeltaTime;
            if (scratcherProps.timer_100 >= Q12(0.0f))
            {
                scratcherProps.timer_100 = Q12(0.75f);
                Sfx_WithFlagsPlay(Sfx_Unk1580, &scratcher->position, Q8(0.5f), SfxFlag_None);
            }
        }
    }
    else
    {
        if (scratcherProps.flags & HangedScratcherFlag_5)
        {
            if (scratcherProps.flags & HangedScratcherFlag_Airborne)
            {
                Sfx_WithFlagsPlay(Sfx_Unk1584, &scratcher->position, Q8(0.5f), SfxFlag_None);
            }
            else
            {
                Sfx_WithFlagsPlay(Sfx_Unk1579, &scratcher->position, Q8(0.5f), SfxFlag_None);
            }

            scratcherProps.timer_100 = Q12(0.5f);
        }
        else
        {
            if (scratcherProps.flags & HangedScratcherFlag_4)
            {
                scratcherProps.timer_100 = Q12(-0.3f);
            }
        }

        switch (scratcher->model.controlState)
        {
            case 1:
            case 16:
                if (scratcherProps.timer_108 == Q12(0.0f))
                {
                    if (!Rng_GenerateInt(0, 511)) // 1 in 512 chance.
                    {
                        if (Math_Vector2MagCalcSafeQ6(PLAYER_CHARA().position.vx - scratcher->position.vx,
                                                PLAYER_CHARA().position.vz - scratcher->position.vz) > Q12(6.5f))
                        {
                            scratcherProps.timer_108 = Rng_GenerateInt(Q12(3.5f), Q12(8.0f) - 1);
                            Sfx_WithFlagsPlay(Sfx_Unk1583, &scratcher->position, Q8(0.5f), SfxFlag_None);
                        }
                    }
                }
                else
                {
                    scratcherProps.timer_108 -= g_DeltaTime;
                    if (scratcherProps.timer_108 < Q12(0.0f))
                    {
                        scratcherProps.timer_108 = Q12(0.0f);
                    }
                }
                break;

            case 2:
            case 3:
            case 5:
            case 6:
            case 7:
                if (scratcherProps.timer_108 == Q12(0.0f))
                {
                    if (!Rng_GenerateUInt(0, 255)) // 1 in 256 chance.
                    {
                        if (Math_Vector2MagCalcSafeQ6(PLAYER_CHARA().position.vx - scratcher->position.vx,
                                                PLAYER_CHARA().position.vz - scratcher->position.vz) > Q12(4.5f))
                        {
                            scratcherProps.timer_108 = Rng_GenerateInt(Q12(3.5f), Q12(7.5f) - 1);
                            Sfx_WithFlagsPlay(Sfx_Unk1583, &scratcher->position, Q8(0.5f), SfxFlag_None);
                        }
                    }
                }
                else
                {
                    scratcherProps.timer_108 -= g_DeltaTime;
                    if (scratcherProps.timer_108 < Q12(0.0f))
                    {
                        scratcherProps.timer_108 = Q12(0.0f);
                    }
                }
                break;
        }
    }

    scratcherProps.flags &= ~(HangedScratcherFlag_4 | HangedScratcherFlag_5);

    // TODO: Can the `+ 88` etc below be added into the Rng macro calls? Couldn't match with that yet.
    if (scratcher->model.anim.status == ANIM_STATUS(HangedScratcherAnim_RunForward, true))
    {
        if (ANIM_TIME_RANGE_CHECK(scratcher->model.anim.time, 272, 273))
        {
            if (scratcherProps.field_103 == 0)
            {
                Sfx_WithPitchPlay(Sfx_Unk1578, &scratcher->position, Q8(0.125f),
                                  Rng_GenerateUInt(0, 15) + (Q12_DIV(scratcher->moveSpeed, Q12(3.0f)) >> 5) + 88);
                scratcherProps.field_103 = 1;
            }
        }
        else
        {
            scratcherProps.field_103 = 0;
        }

        if (animKeyframeIdx == 276 || animKeyframeIdx == 267)
        {
            if (scratcherProps.field_102 == 0)
            {
                Sfx_WithPitchPlay(Sfx_Unk1578, &scratcher->position, Q8(0.125f),
                                  Rng_GenerateUInt(0, 15) + (Q12_DIV(scratcher->moveSpeed, Q12(3.0f)) >> 5) + 88);
                scratcherProps.field_102 = 1;
            }
        }
        else
        {
            scratcherProps.field_102 = 0;
        }
    }
    else if (scratcher->model.anim.status == ANIM_STATUS(HangedScratcherAnim_WalkForward, true))
    {
        if (ANIM_TIME_RANGE_CHECK(scratcher->model.anim.time, 285, 286))
        {
            if (scratcherProps.field_103 == 0)
            {
                Sfx_WithPitchPlay(Sfx_Unk1578, &scratcher->position, Q8(0.5f),
                                  Rng_GenerateUInt(0, 15) + (Q12_DIV(scratcher->moveSpeed, Q12(2.8f)) >> 5) + 120);
                scratcherProps.field_103 = 1;
            }
        }
        else
        {
            scratcherProps.field_103 = 0;
        }

        if (ANIM_TIME_RANGE_CHECK(scratcher->model.anim.time, 277, 278))
        {
            if (scratcherProps.field_102 == 0)
            {
                Sfx_WithPitchPlay(Sfx_Unk1578, &scratcher->position, Q8(0.5f),
                                  Rng_GenerateUInt(0, 15) + (Q12_DIV(scratcher->moveSpeed, Q12(2.8f)) >> 5) + 120);
                scratcherProps.field_102 = 1;
            }
        }
        else
        {
            scratcherProps.field_102 = 0;
        }
    }
    else if (scratcher->model.anim.status == ANIM_STATUS(HangedScratcherAnim_JumpDownFromWall, true))
    {
        if (scratcherProps.field_103 < 3)
        {
            s32 temp_v1 = (scratcherProps.field_103 + 1) * 11;
            if (animKeyframeIdx >= temp_v1 && (animKeyframeIdx < temp_v1 + 2))
            {
                Sfx_WithPitchPlay(Sfx_Unk1578, &scratcher->position, Q8(0.5f), -16);
                scratcherProps.field_103++;
            }
        }
    }
    else
    {
        scratcherProps.field_102 = 0;
        scratcherProps.field_103 = 0;
    }

    #undef animKeyframeIdx
}

#undef scratcherProps
