#include "bodyprog/bodyprog.h"
#include "bodyprog/events/npc_main.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player/player.h"
#include "main/rng.h"
#include "maps/common/shared.h"
#include "maps/common/characters/creeper.h"

#if defined(MAP3_S02)
// .bss data in maps we've matched .bss for
s8 sharedData_800E57CC_1_s02;
#endif

#define creeperProps creeper->properties.creeper

void Creeper_Update(s_SubCharacter* creeper, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords)
{
    // Initialize.
    if (creeper->model.controlState == CreeperControl_None)
    {
        Creeper_Init(creeper);
    }

    if (g_DeltaTime != Q12(0.0f))
    {
        sharedFunc_800D7EE8_1_s02(creeper);
        Creeper_ControlUpdate(creeper);
        sharedFunc_800D983C_1_s02(creeper);
    }

    Creeper_AnimUpdate(creeper, anmHdr, boneCoords);
    Creeper_CollisionUpdate(creeper);
}

// This inline allows getting rid of some ugly gotos, couldn't find a different way to handle it.
static inline void Creeper_PropsUpdateFromStep(s_SubCharacter* creeper)
{
    s32 stateStep;

    stateStep = creeper->model.stateStep;
    if (creeper->model.stateStep != 3)
    {
        if (stateStep < 3)
        {
            return;
        }

        if (stateStep >= 14)
        {
            return;
        }

        if (stateStep < 12)
        {
            return;
        }

        creeperProps.flags         |= CreeperFlag_Leader;
        creeperProps.homePositionX = creeper->position.vx;
        creeperProps.homePositionZ = creeper->position.vz;
    }

    if (creeper->model.stateStep == 13 || creeper->model.stateStep == 3)
    {
        creeperProps.flags |= CreeperFlag_6;
    }

    creeper->model.controlState = 1;
    Chara_AnimSet(creeper, ANIM_STATUS(CreeperAnim_Idle, true), 94);
}

void Creeper_Init(s_SubCharacter* creeper)
{
    // TODO: Values used in the `Rng_Rand16` calls at the end. TODO: Not sure of the actual purpose yet.
    #define BASE_EASY_VAL   0.7f
    #define BASE_NORMAL_VAL 0.9f
    #define BASE_HARD_VAL   1.2f
    #define RAND_EASY_MAX   0.2f
    #define RAND_NORMAL_MAX 0.2f
    #define RAND_HARD_MAX   0.4f

    s32 i;

    // Check if any other Creeper NPCs are present, making sure to skip this `s_SubCharacter` instance in the NPC array.
    i = 0;
    do
    {
        if (creeper == &NPC_CHARA(i) || NPC_CHARA(i).model.charaId != Chara_Creeper)
        {
            i++;
            continue;
        }

        break;
    }
    while (i < ARRAY_SIZE(g_SysWork.npcs));

    if (i == ARRAY_SIZE(g_SysWork.npcs))
    {
        // No other Creepers present.
        sharedData_800E57CC_1_s02 = 0;
    }

    creeper->health           = Q12(200.0f);
    creeperProps.flags        = CreeperFlag_None;
    creeper->model.anim.alpha = Q12(0.0f);
    creeper->moveSpeed        = Q12(0.0f);
    creeper->headingAngle     = creeper->rotation.vy;
    creeper->collision.state  = CharaCollisionState_2;

    Chara_PropsClear(creeper);
    Creeper_PropsUpdateFromStep(creeper);
    ModelAnim_AnimInfoSet(&creeper->model.anim, CREEPER_ANIM_INFOS);
    Chara_DamageClear(creeper);

    creeperProps.targetPositionX = creeper->position.vx;
    creeperProps.targetPositionZ = creeper->position.vz;

    if (g_SavegamePtr->gameDifficulty == GameDifficulty_Easy)
    {
        creeperProps.moveSpeed = Q12_MULT_FLOAT_PRECISE((Q12(BASE_EASY_VAL) + (Rng_Rand16() % Q12(RAND_EASY_MAX))), 2.0f);
    }
    else if (g_SavegamePtr->gameDifficulty == GameDifficulty_Normal)
    {
        creeperProps.moveSpeed = Q12_MULT_FLOAT_PRECISE((Q12(BASE_NORMAL_VAL) + (Rng_Rand16() % Q12(RAND_NORMAL_MAX))), 2.0f);
    }
    else
    {
        creeperProps.moveSpeed = Q12_MULT_FLOAT_PRECISE((Q12(BASE_HARD_VAL) + (Rng_Rand16() % Q12(RAND_HARD_MAX))), 2.0f);
    }

#ifdef MAP5_S00
    creeper->flags |= CharaFlag_NoRadioStatic;
#endif

    #undef BASE_EASY_VAL
    #undef BASE_NORMAL_VAL
    #undef BASE_HARD_VAL
    #undef RAND_EASY_MAX
    #undef RAND_NORMAL_MAX
    #undef RAND_HARD_MAX
}

void sharedFunc_800D7EE8_1_s02(s_SubCharacter* creeper)
{
    // Creeper has been hit.
    if (creeper->damage.amount > Q12(0.0f) && creeper->health > Q12(0.0f))
    {
        Sfx_WithFlagsPlay(Sfx_Unk1425, &creeper->position, Q8(0.5f), SfxFlag_None);
        creeperProps.chirpTimer = Q12(0.0f);

        // TODO: Weird position scaling?
        creeper->damage.position.vx += (creeper->moveSpeed * Math_Sin(creeper->headingAngle)) >> 15;
        creeper->damage.position.vz += (creeper->moveSpeed * Math_Cos(creeper->headingAngle)) >> 15;

        creeper->moveSpeed = FP_TO(Math_Vector2MagCalcSafeQ6(creeper->damage.position.vx, creeper->damage.position.vz), Q12_SHIFT) /
                             Q12(0.8f);

        creeper->fallSpeed    = Q12_DIV(creeper->damage.position.vy, Q12(0.8f));
        creeper->headingAngle = Rng_AddGeneratedUInt(ratan2(creeper->damage.position.vx, creeper->damage.position.vz), -128, 127);

        creeperProps.flags |= CreeperFlag_1;

        if (ANIM_STATUS_IDX_GET(creeper->model.anim.status) == CreeperAnim_StunLoop)
        {
            creeper->health = Q12(0.0f);

            creeperProps.animStatus_10A = ANIM_STATUS(CreeperAnim_StunOnce, true);
            if (creeper->model.anim.status == ANIM_STATUS(CreeperAnim_StunLoop, true))
            {
                creeper->model.anim.status = ANIM_STATUS(CreeperAnim_StunOnce, true);
            }
            else
            {
                creeper->model.anim.status = ANIM_STATUS(CreeperAnim_StunOnce, false);
            }
        }
        else if (ANIM_STATUS_IDX_GET(creeper->model.anim.status) == CreeperAnim_IdleToRunForward)
        {
            creeper->health = Q12(0.0f);

            creeperProps.animStatus_10A = ANIM_STATUS(CreeperAnim_RunForwardStunStart, true);
            if (creeper->model.anim.status == ANIM_STATUS(CreeperAnim_IdleToRunForward, true))
            {
                creeper->model.anim.status = ANIM_STATUS(CreeperAnim_RunForwardStunStart, true);
            }
            else
            {
                creeper->model.anim.status = ANIM_STATUS(CreeperAnim_RunForwardStunStart, false);
            }
        }
        else if (ANIM_STATUS_IDX_GET(creeper->model.anim.status) == CreeperAnim_Stun)
        {
            creeper->health             = Q12(0.0f);
            creeper->model.anim.status  = ANIM_STATUS(CreeperAnim_DeathStart, false);
            creeper->model.controlState = CreeperControl_Damage;
        }
        else if (ANIM_STATUS_IDX_GET(creeper->model.anim.status) == CreeperAnim_RunForward)
        {
            creeper->health             = Q12(0.0f);
            creeper->model.anim.status  = ANIM_STATUS(CreeperAnim_RunForwardStunCont, false);
            creeper->model.controlState = CreeperControl_Damage;
        }
        else
        {
            if (creeper->model.controlState == CreeperControl_Attack)
            {
                g_SysWork.charaGroupFlags[3] &= ~CharaGroupFlag_1;
            }

            creeper->model.controlState = CreeperControl_Stun;
            creeper->health             = Q12(20.0f);
            creeper->flags             |= CharaFlag_Unk2;

            // Stun if heading toward player.
            if (ABS(Math_AngleNormalizeSigned(Math_AngleBetweenPositionsGet(creeper->position, PLAYER_CHARA().position) -
                                              creeper->rotation.vy)) < Q12_ANGLE(60.0f))
            {
                creeper->model.anim.status = ANIM_STATUS(CreeperAnim_StunLoop, false);
            }
            // Keep moving if heading away from player.
            else
            {
                creeper->model.anim.status = ANIM_STATUS(CreeperAnim_IdleToRunForward, false);
            }
        }
    }

    // Clear damage.
    if (creeper->damage.amount < Q12(200.0f) || creeper->health <= Q12(0.0f))
    {
        Chara_DamageClear(creeper);
    }
}

void Creeper_ControlUpdate(s_SubCharacter* creeper)
{
    // Handle control state.
    switch (creeper->model.controlState)
    {
        case CreeperControl_Idle:
            Creeper_ControlIdle(creeper);
            break;

        case CreeperControl_WalkForward:
            Creeper_ControlWalkForward(creeper);
            break;

        case CreeperControl_Attack:
            Creeper_ControlAttack(creeper);
            break;

        case CreeperControl_Stun:
            Creeper_ControlStun(creeper);
            break;

        case CreeperControl_Damage:
            Creeper_ControlDamage(creeper);
            break;
    }

    // Reset alerted flag.
    creeperProps.flags &= ~CreeperFlag_Alerted;
}

void Creeper_ControlIdle(s_SubCharacter* creeper)
{
    bool cond;

    Chara_MoveSpeedUpdate3(creeper, Q12(4.0f), Q12(0.0f));
    creeperProps.chirpTimer += g_DeltaTime;

    if (!WorldEnv_IsInDarkness() && WorldEnv_IsFlashlightAllowed())
    {
        cond = func_8006FD90(creeper, 2, Q12(12.0f), Q12(8.0f));
    }
    else if (WorldEnv_IsFlashlightAllowed() && WorldEnv_IsInDarkness())
    {
        cond = func_8006FD90(creeper, 2, Q12(0.8f), Q12(0.4f));
    }
    else
    {
        cond = func_8006FD90(creeper, 2, Q12(4.0f), Q12(12.0f));
    }

    cond |= func_80070360(creeper, Q12(0.0f), Q12(0.5f));
    if (cond)
    {
        creeper->flags |= CharaFlag_Hit;

        if (Rng_GenerateUInt(0, 7) == 0 && // 1 in 8 chance.
            creeper->model.anim.status == ANIM_STATUS(CreeperAnim_Idle, true))
        {
            creeper->model.controlState = CreeperControl_WalkForward;
            creeper->model.anim.status  = ANIM_STATUS(CreeperAnim_WalkForward, false);
            creeperProps.attackTimer    = Q12(0.5f);
            creeperProps.chirpTimer     = Q12(0.0f);
            return;
        }
    }

    // Check if Creeper was alerted.
    if (creeperProps.flags & CreeperFlag_Alerted)
    {
        creeper->model.controlState = CreeperControl_WalkForward;
        creeper->model.anim.status  = ANIM_STATUS(CreeperAnim_WalkForward, false);
        creeperProps.attackTimer    = Q12(0.5f);
        creeperProps.chirpTimer     = Q12(0.0f);
    }
    else if (creeperProps.flags & CreeperFlag_6)
    {
        if (creeperProps.flags & CreeperFlag_7)
        {
            if (!Rng_GenerateUInt(0, 7)) // 1 in 8 chance.
            {
                if (creeperProps.chirpTimer > Q12(0.5f))
                {
                    if (creeperProps.chirpTimer < Q12(2.5f))
                    {
                        Sfx_WithFlagsPlay(Sfx_Unk1427, &creeper->position, Q8(0.5f), SfxFlag_None);

                        creeperProps.chirpTimer = Q12(0.0f);
                        sharedData_800E57CC_1_s02--;
                        creeperProps.flags &= ~CreeperFlag_7;
                    }
                }
            }
        }
        else if (sharedData_800E57CC_1_s02 == 0)
        {
            if (!Rng_GenerateUInt(0, 511) && creeperProps.chirpTimer > Q12(4.0f)) // 1 in 512 chance.
            {
                s32 i;

                Sfx_WithFlagsPlay(Sfx_Unk1427, &creeper->position, Q8(0.5f), SfxFlag_None);
                creeperProps.chirpTimer = Q12(0.0f);

                // Update other Creepers.
                for (i = 0; i < ARRAY_SIZE(g_SysWork.npcs); i++)
                {
                    #define curNpc NPC_CHARA(i)

                    // Check if NPC is a Creeper.
                    if (curNpc.model.charaId != Chara_Creeper)
                    {
                        continue;
                    }

                    if (curNpc.field_40 != creeper->field_40 &&
                        (curNpc.properties.creeper.flags & CreeperFlag_6) &&
                        curNpc.model.controlState == CreeperControl_Idle &&
                        !Math_Distance2dCheck(&creeper->position, &curNpc.position, Q12(16.0f)))
                    {
                        curNpc.properties.creeper.chirpTimer = Q12(0.0f);
                        sharedData_800E57CC_1_s02++;
                        curNpc.properties.creeper.flags |= CreeperFlag_7;
                    }

                    #undef curNpc
                }
            }
            else if (!Rng_GenerateUInt(0, 63) && creeperProps.chirpTimer > Q12(2.0f)) // 1 in 64 chance.
            {
                Sfx_WithFlagsPlay(Sfx_Unk1426, &creeper->position, Q8(0.5f), SfxFlag_None);
                creeperProps.chirpTimer = Rng_GenerateInt(Q12(0.8f), Q12(1.8f) - 1);
            }
        }
    }
}

void Creeper_ControlWalkForward(s_SubCharacter* creeper)
{
    q3_12  unkAngleDelta;
    q3_12  angleDeltaToPlayer;
    q19_12 distToPlayer;
    q19_12 distToTarget;

    angleDeltaToPlayer = Math_AngleNormalizeSigned(Math_AngleBetweenPositionsGet(creeper->position, PLAYER_CHARA().position) -
                                                   creeper->rotation.vy);

    if (((!WorldEnv_IsInDarkness() && WorldEnv_IsFlashlightAllowed()) && func_8006FD90(creeper, 0, Q12(12.0f), Q12(8.0f))) ||
        (WorldEnv_IsFlashlightAllowed() && WorldEnv_IsInDarkness() && func_8006FD90(creeper, 2, Q12(0.8f), Q12(0.4f))) ||
        ((!WorldEnv_IsInDarkness() && !WorldEnv_IsFlashlightAllowed()) && func_8006FD90(creeper, 1, Q12(4.0f), Q12(12.0f))) ||
        (func_80070360(creeper, 0, Q12(0.5f)) || creeperProps.flags & CreeperFlag_Alerted))
    {
        creeperProps.targetPositionX = PLAYER_CHARA().position.vx;
        creeperProps.targetPositionZ = PLAYER_CHARA().position.vz;
        creeperProps.flags          &= ~CreeperFlag_TargetPlayer;
    }

    distToPlayer = Math_Vector2MagCalcSafeQ6(PLAYER_CHARA().position.vx - creeper->position.vx,
                                       PLAYER_CHARA().position.vz - creeper->position.vz);
    distToTarget = Math_Vector2MagCalcSafeQ6(creeperProps.targetPositionX - creeper->position.vx,
                                       creeperProps.targetPositionZ - creeper->position.vz);

    if (distToTarget < Q12(1.2f) && !(creeperProps.flags & CreeperFlag_HasAttacked) &&
        !Los_NpcToPlayerHitCheck(creeper, &PLAYER_CHARA()))
    {
        if (distToPlayer > Q12(1.2f))
        {
            if ((*(s32*)&creeperProps.flags & (CreeperFlag_Leader | CreeperFlag_TargetPlayer)) == CreeperFlag_Leader)
            {
                creeperProps.flags          |= CreeperFlag_TargetPlayer;
                creeperProps.targetPositionX = creeperProps.homePositionX;
                creeperProps.targetPositionZ = creeperProps.homePositionZ;
            }
            else
            {
                creeper->model.controlState = CreeperControl_Idle;
                creeper->model.anim.status  = ANIM_STATUS(CreeperAnim_Idle, false);
                creeperProps.attackTimer    = Q12(0.0f);
            }
        }
        else if (!(g_SysWork.charaGroupFlags[3] & CharaGroupFlag_1) && !Chara_HasFlag(&PLAYER_CHARA(), CharaFlag_Unk4) &&
                 distToPlayer < Q12(0.5f) && ABS(angleDeltaToPlayer) < Q12_ANGLE(10.0f) &&
                 PLAYER_CHARA().health > Q12(0.0f))
        {
            creeper->model.controlState   = CreeperControl_Attack;
            creeper->model.anim.status    = ANIM_STATUS(CreeperAnim_AttackStart, false);
            creeperProps.attackTimer      = Q12(0.0f);
            g_SysWork.charaGroupFlags[3] |= CharaGroupFlag_1;
        }
        else
        {
            if (ABS(angleDeltaToPlayer) >= Q12_ANGLE(90.0f))
            {
                Chara_MoveSpeedUpdate(creeper, Q12(4.0f));
            }
            else
            {
                Chara_MoveSpeedUpdate3(creeper, Q12(1.5f), (creeperProps.moveSpeed * (Q12_ANGLE(90.0f) - ABS(angleDeltaToPlayer))) >> 10);
            }

            if (TIMESTEP_ANGLE(1, 2) < ABS(angleDeltaToPlayer) && Rng_GenerateInt(0, 7)) // 1 in 8 chance.
            {
                if (angleDeltaToPlayer > Q12_ANGLE(0.0f))
                {
                    creeper->rotation.vy += Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(180.0f));
                }
                else
                {
                    creeper->rotation.vy -= Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(180.0f));
                }
            }
        }
    }
    else
    {
        Chara_MoveSpeedUpdate4(creeper, Q12(1.5f), creeperProps.moveSpeed);

        creeperProps.attackTimer += g_DeltaTime;

        if ((ABS(angleDeltaToPlayer) > Q12_ANGLE(10.0f) && Los_CharaToCharaDistCheck(creeper, Q12(1.0f), creeperProps.angleToTarget)) ||
            (!Rng_GenerateInt(0, 7) && // 1 in 8 chance.
             ((!(creeperProps.flags & CreeperFlag_HasAttacked) && creeperProps.attackTimer > Q12(0.5f)) ||
              ( (creeperProps.flags & CreeperFlag_HasAttacked) && creeperProps.attackTimer > Q12(2.0f)))))
        {
            if (creeperProps.attackTimer > Q12(2.0f))
            {
                creeperProps.flags &= ~CreeperFlag_HasAttacked;
            }

            creeperProps.angleToTarget = Los_CharaHeadingAngleGet(creeper, Q12(1.2f),
                                                               creeperProps.targetPositionX,
                                                               creeperProps.targetPositionZ,
                                                               Q12_ANGLE(360.0f), true);
            if (creeperProps.angleToTarget == Q12_ANGLE(360.0f))
            {
                creeperProps.angleToTarget = creeper->rotation.vy;
            }
            else
            {
                creeperProps.attackTimer = Q12(0.0f);
            }
        }

        unkAngleDelta = Math_AngleNormalizeSigned(creeperProps.angleToTarget - creeper->rotation.vy);
        if (TIMESTEP_ANGLE(1, 2) < ABS(unkAngleDelta))
        {
            if (unkAngleDelta > Q12_ANGLE(0.0f))
            {
                creeper->rotation.vy += Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(180.0f));
            }
            else
            {
                creeper->rotation.vy -= Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(180.0f));
            }
        }
    }
}

void Creeper_ControlAttack(s_SubCharacter* creeper)
{
    VECTOR3 creeperPos;
    s32     i;
    q19_12  moveSpeed0;
    q19_12  distToPlayer;

    if (Los_NpcToPlayerHitCheck(creeper, &PLAYER_CHARA()))
    {
        g_SysWork.charaGroupFlags[3] &= ~CharaGroupFlag_1;
        creeper->model.controlState   = CreeperControl_WalkForward;
        creeper->model.anim.status    = ANIM_STATUS(CreeperAnim_WalkForward, false);
        return;
    }

    if (ANIM_STATUS_IDX_GET(creeper->model.anim.status) == CreeperAnim_AttackStart ||
        creeper->model.anim.status == ANIM_STATUS(CreeperAnim_Attack, false) ||
        ANIM_TIME_RANGE_CHECK(creeper->model.anim.time, 4, 7))
    {
        distToPlayer = Math_Vector2MagCalcSafeQ6(PLAYER_CHARA().position.vx - creeper->position.vx,
                                           PLAYER_CHARA().position.vz - creeper->position.vz);
        if (distToPlayer < Q12(0.4f))
        {
            Chara_MoveSpeedUpdate3(creeper, Q12(16.0f), Q12(0.0f));
        }
        else
        {
            moveSpeed0 = creeperProps.moveSpeed / 2;

            Chara_MoveSpeedUpdate2(creeper, Q12(6.0f), moveSpeed0);
        }

        if (TIMESTEP_ANGLE(1, 2) < ABS(Math_AngleNormalizeSigned((Math_AngleBetweenPositionsGet(creeper->position, PLAYER_CHARA().position) - creeper->rotation.vy))))
        {
            if ((Math_AngleNormalizeSigned((Math_AngleBetweenPositionsGet(creeper->position, PLAYER_CHARA().position) - creeper->rotation.vy)) << 16) > 0)
            {
                creeper->rotation.vy += Q12_MULT_PRECISE(g_DeltaTime, Q12(0.5f));
            }
            else
            {
                creeper->rotation.vy -= Q12_MULT_PRECISE(g_DeltaTime, Q12(0.5f));
            }
        }
        else
        {
            creeper->rotation.vy = Math_AngleBetweenPositionsGet(creeper->position, PLAYER_CHARA().position);
        }

        creeper->field_44.field_0 = 1;
        return;
    }

    Chara_MoveSpeedUpdate3(creeper, Q12(2.0f), Q12(0.0f));

    if (ANIM_TIME_RANGE_CHECK(creeper->model.anim.time, 9, 10))
    {
        creeperPos     = creeper->position;
        creeperPos.vy += creeper->collision.box.field_8;

        func_8008A0E4(1, WEAPON_ATTACK(AttackType_Creeper, AttackInputType_Tap),
                      creeper, &creeperPos, &PLAYER_CHARA(), creeper->rotation.vy,
                      ratan2(Q12(0.4f),
                             (PLAYER_CHARA().position.vy + PLAYER_CHARA().collision.box.bottomExtent) -
                             (creeper->position.vy + creeper->collision.box.bottomExtent)));

        if (!(creeperProps.flags & CreeperFlag_HasAttacked))
        {
            Sfx_WithFlagsPlay(Sfx_Unk1424, &creeper->position, Q8(0.5f), SfxFlag_None);
        }

        creeperProps.flags |= CreeperFlag_HasAttacked;

        // Alert other Creepers.
        for (i = 0; i < ARRAY_SIZE(g_SysWork.npcs); i++)
        {
            if (creeper->model.charaId == Chara_Creeper &&
                !Math_Distance2dCheck(&creeper->position, &NPC_CHARA(i).position, Q12(16.0f)))
            {
                NPC_CHARA(i).properties.creeper.flags |= CreeperFlag_Alerted;
            }
        }
    }
    else if (ANIM_STATUS_IDX_GET(creeper->model.anim.status) == CreeperAnim_WalkForward)
    {
        g_SysWork.charaGroupFlags[3] &= ~CharaGroupFlag_1;
        creeper->model.controlState   = CreeperControl_WalkForward;
        creeperProps.attackTimer      = Q12(0.0f);
        creeperProps.angleToTarget    = Los_CharaHeadingAngleGet(creeper, Q12(4.8f),
                                                              PLAYER_CHARA().position.vx, PLAYER_CHARA().position.vz,
                                                              Q12_ANGLE(360.0f), false);
        creeper->field_44.field_0     = 0;
    }
}

void Creeper_ControlStun(s_SubCharacter* creeper)
{
    q19_12 dist;

    if (creeper->model.anim.status == ANIM_STATUS(CreeperAnim_StunLoop, true))
    {
        if (creeper->rotation.vy != Math_AngleNormalizeSigned(creeper->headingAngle + Q12_ANGLE(180.0f)))
        {
            if (Math_AngleNormalizeSigned((creeper->headingAngle + Q12_ANGLE(180.0f)) - creeper->rotation.vy) > Q12_ANGLE(0.0f))
            {
                creeper->rotation.vy += Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(30.0f));
            }
            else
            {
                creeper->rotation.vy -= Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(30.0f));
            }
        }

        if (ANIM_TIME_RANGE_CHECK(creeper->model.anim.time, 50, 59))
        {
            dist = Q12_MULT_PRECISE(CREEPER_ANIM_INFOS[creeper->model.anim.status].duration.constant, g_DeltaTime) / 9;
        }
        else
        {
            q19_12 baseDist = Q12_MULT_PRECISE(CREEPER_ANIM_INFOS[creeper->model.anim.status].duration.constant, g_DeltaTime) * Q12(0.1f);
            dist            = baseDist / Q12(4.0f);
        }

        creeperProps.collisionOffsetX = Q12_MULT(dist, Math_Sin(creeper->rotation.vy + Q12_ANGLE(180.0f)));
        creeperProps.collisionOffsetZ = Q12_MULT(dist, Math_Cos(creeper->rotation.vy + Q12_ANGLE(180.0f)));
    }

    if (ANIM_STATUS_IDX_GET(creeper->model.anim.status) == CreeperAnim_DeathStart         ||
        ANIM_STATUS_IDX_GET(creeper->model.anim.status) == CreeperAnim_RunForwardStunCont ||
        ANIM_STATUS_IDX_GET(creeper->model.anim.status) == CreeperAnim_DeathEnd           ||
        ANIM_STATUS_IDX_GET(creeper->model.anim.status) == CreeperAnim_RunForwardStunEnd)
    {
        creeper->model.controlState = CreeperControl_Damage;
    }
}

void Creeper_ControlDamage(s_SubCharacter* creeper)
{
    creeper->flags &= ~CharaFlag_Unk2;

    if (creeper->health == Q12(0.0f) && Chara_NpcIdxGet(creeper) != g_SysWork.targetNpcIdx)
    {
        creeper->health          = NO_VALUE;
        creeper->collision.state = CharaCollisionState_Ignore;
    }

    if (creeper->moveSpeed == Q12(0.0f) && !(*(s32*)&creeperProps.flags & (CreeperFlag_Falling | CreeperFlag_3)))
    {
        creeper->properties.creeper.flags |= CreeperFlag_3;
        Savegame_EnemyStateUpdate(creeper);

        if (creeper->model.anim.status == ANIM_STATUS(CreeperAnim_DeathEnd, true))
        {
            func_800622B8(3, creeper, ANIM_STATUS(CreeperAnim_Recoil, false), 5);
        }
        else
        {
            func_800622B8(3, creeper, ANIM_STATUS(CreeperAnim_Recoil, true), 5);
        }
    }
}

void sharedFunc_800D983C_1_s02(s_SubCharacter* creeper)
{
    s_CollisionResult collResult;

    creeper->fallSpeed += g_GravitySpeed >> 2;

    if (creeperProps.flags & CreeperFlag_1)
    {
        Chara_MoveSpeedUpdate(creeper, Q12(4.0f));
    }
    else
    {
        creeper->headingAngle = creeper->rotation.vy;
    }

    Chara_MovementUpdate1(creeper, &collResult, creeperProps.collisionOffsetX, creeperProps.collisionOffsetZ);
    creeperProps.collisionOffsetZ = Q12(0.0f);
    creeperProps.collisionOffsetX = Q12(0.0f);

    if (creeper->fallSpeed != Q12(0.0f))
    {
        creeperProps.flags |= CreeperFlag_Falling;
    }
    else
    {
        creeperProps.flags &= ~CreeperFlag_Falling;
    }

    creeper->rotation.vy = Math_AngleNormalizeSigned(creeper->rotation.vy);
}

void Creeper_AnimUpdate(s_SubCharacter* creeper, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords)
{
    s_AnimInfo* animInfo;

    Math_MatrixTransform(&creeper->position, &creeper->rotation, boneCoords);

    animInfo = &CREEPER_ANIM_INFOS[creeper->model.anim.status];
    animInfo->playbackFunc(&creeper->model, anmHdr, boneCoords, animInfo);
}

extern s_CharaKeyframe sharedData_800E0F78_1_s02[];
extern s_CharaKeyframe sharedData_800E0FC8_1_s02;
extern s_CharaKeyframe sharedData_800E0FDC_1_s02;
extern s_CharaKeyframe sharedData_800E0FF0_1_s02;
extern s_CharaKeyframe sharedData_800E1004_1_s02[]; // Creeper keyframe data?
extern s_CharaKeyframe sharedData_800E10CC_1_s02;
extern s_CharaKeyframe sharedData_800E10E0_1_s02[];
extern s_CharaKeyframe sharedData_800E1158_1_s02;
extern s_CharaKeyframe sharedData_800E116C_1_s02;
extern s_CharaKeyframe sharedData_800E1180_1_s02;
extern s_CharaKeyframe sharedData_800E1194_1_s02;
extern s_CharaKeyframe sharedData_800E11A8_1_s02;
extern s_CharaKeyframe sharedData_800E11BC_1_s02;

void Creeper_CollisionUpdate(s_SubCharacter* creeper)
{
    s32 keyframeIdx0;
    s32 keyframeIdx1;
    s32 keyframeIdx2;
    s32 keyframeIdx3;

    switch (creeper->model.anim.status)
    {
        case ANIM_STATUS(CreeperAnim_AttackToWalkForward, false):
            Collision_CharaCollisionSet(creeper, &sharedData_800E0FC8_1_s02, &sharedData_800E0F78_1_s02[0]);
            break;

        case ANIM_STATUS(CreeperAnim_AttackToWalkForward, true):
            creeper->properties.creeper.animStatus_10A = ANIM_STATUS(CreeperAnim_AttackToWalkForward, true);
            keyframeIdx0 = FP_FROM(creeper->model.anim.time, Q12_SHIFT);
            keyframeIdx1 = keyframeIdx0 + 1;
            Collision_CharaCollisionSet(creeper, &sharedData_800E0F78_1_s02[keyframeIdx0], &sharedData_800E0F78_1_s02[keyframeIdx1]);
            break;

        case ANIM_STATUS(CreeperAnim_Attack, false):
            Collision_CharaCollisionSet(creeper, &sharedData_800E11A8_1_s02, &sharedData_800E0FC8_1_s02);
            break;

        case ANIM_STATUS(CreeperAnim_Attack, true):
            Chara_CollisionSet(creeper, sharedData_800E0FC8_1_s02);
            creeper->collision.box.field_8 = Q12(-0.16f);
            break;

        case ANIM_STATUS(CreeperAnim_DeathStart, false):
            if (creeper->properties.creeper.animStatus_10A == ANIM_STATUS(CreeperAnim_StunOnce, true))
            {
                Collision_CharaCollisionSet(creeper, &sharedData_800E10CC_1_s02, &sharedData_800E0FDC_1_s02);
            }
            else
            {
                Collision_CharaCollisionSet(creeper, &sharedData_800E116C_1_s02, &sharedData_800E0FDC_1_s02);
            }
            break;

        case ANIM_STATUS(CreeperAnim_DeathStart, true):
        case ANIM_STATUS(CreeperAnim_DeathEnd,  false):
        case ANIM_STATUS(CreeperAnim_DeathEnd,  true):
            Chara_CollisionSet(creeper, sharedData_800E0FDC_1_s02);
            break;

        case ANIM_STATUS(CreeperAnim_RunForwardStunCont, false):
            if (creeper->properties.creeper.animStatus_10A == ANIM_STATUS(CreeperAnim_RunForwardStunStart, true))
            {
                Collision_CharaCollisionSet(creeper, &sharedData_800E1158_1_s02, &sharedData_800E0FF0_1_s02);
            }
            else
            {
                Collision_CharaCollisionSet(creeper, &sharedData_800E1180_1_s02, &sharedData_800E0FF0_1_s02);
            }
            break;

        case ANIM_STATUS(CreeperAnim_RunForwardStunCont, true):
        case ANIM_STATUS(CreeperAnim_RunForwardStunEnd, false):
        case ANIM_STATUS(CreeperAnim_RunForwardStunEnd, true):
            Chara_CollisionSet(creeper, sharedData_800E0FF0_1_s02);
            break;

        case ANIM_STATUS(CreeperAnim_StunLoop, false):
        case ANIM_STATUS(CreeperAnim_StunOnce, false):
            Chara_CollisionSet(creeper, sharedData_800E1004_1_s02[0]);
            break;

        case ANIM_STATUS(CreeperAnim_StunLoop, true):
        case ANIM_STATUS(CreeperAnim_StunOnce, true):
            keyframeIdx2 = FP_FROM(creeper->model.anim.time, Q12_SHIFT) - 50;
            keyframeIdx3 = keyframeIdx2 - !(keyframeIdx2 < 10) - !(keyframeIdx2 < 11);

            keyframeIdx0 = keyframeIdx3 - !(keyframeIdx2 < 12);
            keyframeIdx1 = (keyframeIdx2 + 1) - !(keyframeIdx2 < 9) - !(keyframeIdx2 < 10) - !(keyframeIdx2 < 11);
            Collision_CharaCollisionSet(creeper, &sharedData_800E1004_1_s02[keyframeIdx0], &sharedData_800E1004_1_s02[keyframeIdx1]);
            break;

        case ANIM_STATUS(CreeperAnim_IdleToRunForward, false):
        case ANIM_STATUS(CreeperAnim_RunForwardStunStart, false):
            Chara_CollisionSet(creeper, sharedData_800E10E0_1_s02[0]);
            break;

        case ANIM_STATUS(CreeperAnim_IdleToRunForward, true):
        case ANIM_STATUS(CreeperAnim_RunForwardStunStart, true):
            keyframeIdx0 = FP_FROM(creeper->model.anim.time, Q12_SHIFT) - 64;
            keyframeIdx1 = keyframeIdx0 + 1;
            Collision_CharaCollisionSet(creeper, &sharedData_800E10E0_1_s02[keyframeIdx0], &sharedData_800E10E0_1_s02[keyframeIdx1]);
            break;

        case ANIM_STATUS(CreeperAnim_Stun, false):
            Collision_CharaCollisionSet(creeper, &sharedData_800E10CC_1_s02, &sharedData_800E116C_1_s02);
            break;

        case ANIM_STATUS(CreeperAnim_Stun, true):
            Chara_CollisionSet(creeper, sharedData_800E116C_1_s02);
            break;

        case ANIM_STATUS(CreeperAnim_RunForward, false):
            Collision_CharaCollisionSet(creeper, &sharedData_800E1158_1_s02, &sharedData_800E1180_1_s02);
            break;

        case ANIM_STATUS(CreeperAnim_RunForward, true):
            Chara_CollisionSet(creeper, sharedData_800E1180_1_s02);
            break;

        case ANIM_STATUS(CreeperAnim_Idle, false):
            Collision_CharaCollisionSet(creeper, &sharedData_800E11BC_1_s02, &sharedData_800E1194_1_s02);
            break;

        case ANIM_STATUS(CreeperAnim_Idle, true):
            creeper->properties.creeper.animStatus_10A = ANIM_STATUS(CreeperAnim_Idle, true);
            Chara_CollisionSet(creeper, sharedData_800E1194_1_s02);
            break;

        case ANIM_STATUS(CreeperAnim_AttackStart, false):
            Collision_CharaCollisionSet(creeper, &sharedData_800E11BC_1_s02, &sharedData_800E11A8_1_s02);
            break;

        case ANIM_STATUS(CreeperAnim_AttackStart, true):
            Chara_CollisionSet(creeper, sharedData_800E11A8_1_s02);
            break;

        case ANIM_STATUS(CreeperAnim_WalkForward, false):
            if (creeper->properties.creeper.animStatus_10A == ANIM_STATUS(CreeperAnim_Idle, true))
            {
                Collision_CharaCollisionSet(creeper, &sharedData_800E1194_1_s02, &sharedData_800E11BC_1_s02);
            }
            else if (creeper->properties.creeper.animStatus_10A == ANIM_STATUS(CreeperAnim_AttackToWalkForward, true))
            {
                Collision_CharaCollisionSet(creeper, &sharedData_800E0F78_1_s02[0], &sharedData_800E11BC_1_s02);
            }
            else
            {
                Chara_CollisionSet(creeper, sharedData_800E11BC_1_s02);
            }
            break;

        case ANIM_STATUS(CreeperAnim_WalkForward, true):
            Chara_CollisionSet(creeper, sharedData_800E11BC_1_s02);
            break;
    }

    Chara_CollisionShapeOffsetsUpdate(&creeper->collision.shapeOffsets, creeper);
}
