#include "bodyprog/bodyprog.h"
#include "bodyprog/events/npc_main.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player/player.h"
#include "main/rng.h"
#include "maps/common/shared.h"
#include "maps/common/characters/puppet_nurse.h"
#include "maps/common/unk_m3s03.h"

// TODO:
// - Make this separate split in each map that uses it, instead of `#include`
// - Move funcdecls/structs for these out of shared.h header.

#define nurseProps      nurse->properties.puppetNurse
#define localNurseProps localNurse->properties.puppetNurse
#define playerChara     g_SysWork.playerWork.player

s32 PuppetNurse_HurtSfxIdGet(s_SubCharacter* nurse)
{
    s32 weaponAttack;
    s32 idx;

    weaponAttack = nurse->attackReceived;
    if (weaponAttack != WEAPON_ATTACK(EquippedWeaponId_Chainsaw, AttackInputType_Tap) &&
        weaponAttack != WEAPON_ATTACK(EquippedWeaponId_Chainsaw, AttackInputType_Hold) &&
        weaponAttack != WEAPON_ATTACK(EquippedWeaponId_Chainsaw, AttackInputType_Multitap) &&
        weaponAttack != WEAPON_ATTACK(EquippedWeaponId_RockDrill, AttackInputType_Tap) &&
        weaponAttack != WEAPON_ATTACK(EquippedWeaponId_RockDrill, AttackInputType_Hold) &&
        weaponAttack != WEAPON_ATTACK(EquippedWeaponId_RockDrill, AttackInputType_Multitap))
    {
        idx = D_800AD4C8[weaponAttack].field_10;
        return g_PuppetNurseHurtSfxIdxs[idx];
    }

    return NO_VALUE;
}

void PuppetNurse_SfxPlay(s_SubCharacter* nurse, s32 idx)
{
    s32        idx0;
    s_SfxPair* sfxPair;

    sfxPair = g_NursePuppetSfxs;
    idx0    = (nurseProps.field_124->idx_1C * 9) + idx;
    Sfx_WithFlagsPlay(sfxPair[idx0].sfxId, &nurse->position, sfxPair[idx0].vol, SfxFlag_None);
}

s32 PuppetNurse_AnimSfxGet(s32 animFrame)
{
    u8 idx;
    u8 sfxOffsets[4] = { 9, 6, 7, 8 };

    idx = g_NursePuppet_AnimSfxs[animFrame];
    return sfxOffsets[idx];
}

void sharedFunc_800CDA88_3_s03(s_SubCharacter* nurse)
{
    if (nurse->model.stateStep == 0)
    {
        PuppetNurse_Control(nurse);
    }
}

void PuppetNurse_UpdateMain(s_SubCharacter* nurse, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords)
{
    if (g_DeltaTime != Q12(0.0f))
    {
        PuppetNurse_DamageHandle(nurse);
        PuppetNurse_Control(nurse);
        sharedFunc_800CDA88_3_s03(nurse);
        sharedFunc_800D03E4_3_s03(nurse);
        PuppetNurse_AnimUpdate(nurse, anmHdr, boneCoords);
        sharedFunc_800D0968_3_s03(nurse, boneCoords);
        sharedFunc_800D02E4_3_s03(nurse, boneCoords);
        return;
    }
    else
    {
        PuppetNurse_AnimUpdate(nurse, anmHdr, boneCoords);
    }
}

void PuppetNurse_Init(s_SubCharacter* nurse, bool isDoctor)
{
    extern s_800D5710 sharedData_800D5710_3_s03[4]; // Likely static.

    s32             charaQuery;
    s32             charStatIdx;
    s32             charPalette;
    s32             modelVariantIdx;
    s32             stateStepDiv3;
    s_SubCharacter* localNurse;

    localNurse = nurse; // TODO: Not sure why this is needed, possibly an inline here somewhere?

    nurse->moveSpeed                      = Q12(0.0f);
    nurse->collision.state                = CharaCollisionState_Npc;
    nurse->headingAngle                   = nurse->rotation.vy;
    nurseProps.position_E8                = nurse->position;
    nurse->collision.cylinder.outerRadius = Q12(0.3f);
    nurseProps.damage.position.vx         = Q12(0.0f);
    nurseProps.damage.position.vy         = Q12(0.0f);
    nurseProps.damage.position.vz         = Q12(0.0f);
    nurseProps.field_114                  = Q12(0.0f);
    nurseProps.field_118                  = 0;

    Chara_DamageClear(nurse);

    nurseProps.flags_122 = PuppetNurseFlag_None;
    nurseProps.field_108 = nurse->position.vx;
    nurseProps.field_10C = nurse->position.vz;

    charPalette     = (nurse->model.stateStep - 1) % 3;
    stateStepDiv3   = (nurse->model.stateStep - 1) / 3;
    modelVariantIdx = stateStepDiv3 % 3;
    charaQuery      = stateStepDiv3 / 3;

    nurseProps.field_11A = Q12(0.0f);
    nurse->flags        |= CharaFlag_Hit;

    if (!isDoctor)
    {
        charStatIdx                = charPalette + 1; // Skip doctor stat at beginning.
        nurseProps.modelVariantIdx = modelVariantIdx + 1;
    }
    else
    {
        nurseProps.modelVariantIdx = 1;
        charPalette                = 0;
        charStatIdx                = 0;
    }

    localNurseProps.field_124 = &sharedData_800D5710_3_s03[charStatIdx];

    nurse->health           = localNurseProps.field_124->health_0;
    nurse->model.paletteIdx = charPalette;

    switch (charaQuery)
    {
        case 0:
            nurse->model.controlState     = PuppetNurseControl_11;
            nurse->model.stateStep        = 0;
            nurse->model.anim.time        = Q12(459.0f);
            nurse->model.anim.keyframeIdx = 459;
            break;

        case 1:
            nurse->model.controlState     = PuppetNurseControl_10;
            nurse->model.stateStep        = 0;
            nurse->model.anim.time        = Q12(459.0f);
            nurse->model.anim.keyframeIdx = 459;
            break;

        case 2:
            nurse->model.controlState     = PuppetNurseControl_9;
            nurse->model.stateStep        = 0;
            nurse->model.anim.time        = Q12(459.0f);
            nurse->model.anim.keyframeIdx = 459;
            break;
    }

    ModelAnim_AnimInfoSet(&nurse->model.anim, localNurseProps.field_124->animInfo_24);

    nurse->model.anim.status  = ANIM_STATUS(PuppetNurseAnim_17, false);
    nurse->model.anim.alpha   = Q12(0.0f);
    localNurseProps.field_120 = Q12(1.0f);
}

void PuppetNurse_Update(s_SubCharacter* nurse, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords)
{
    // Initialize.
    if (nurse->model.controlState == 0)
    {
        PuppetNurse_Init(nurse, false);
    }

    PuppetNurse_UpdateMain(nurse, anmHdr, boneCoords);
}

void PuppetDoctor_Update(s_SubCharacter* doctor, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords)
{
    // Initialize.
    if (doctor->model.controlState == 0)
    {
        PuppetNurse_Init(doctor, true);
    }

    PuppetNurse_UpdateMain(doctor, anmHdr, boneCoords);
}

bool PuppetNurse_SomeAngleCheck(s_SubCharacter* nurse)
{
    q19_12 sumSqr;
    q19_12 damageAngle;

    sumSqr = Q12_SQUARE_PRECISE(nurse->damage.position.vx) +
             Q12_SQUARE_PRECISE(nurse->damage.position.vy) +
             Q12_SQUARE_PRECISE(nurse->damage.position.vz);

    damageAngle = ratan2(nurse->damage.position.vx, nurse->damage.position.vz);
    damageAngle = ABS(Math_AngleNormalizeSigned(damageAngle - nurse->rotation.vy));
    if (sumSqr > Q12_ANGLE(360.0f) && damageAngle > Q12_ANGLE(10.0f))
    {
        return true;
    }

    return false;
}

void PuppetNurse_DamageHandle(s_SubCharacter* nurse)
{
    q19_12          newHealth;
    s32             sfxIdx;
    s_SubCharacter* localNurse;

    localNurse = nurse;

    if (nurse->damage.amount > Q12(0.0f))
    {
        sfxIdx = PuppetNurse_HurtSfxIdGet(nurse);
        if (sfxIdx != NO_VALUE)
        {
            PuppetNurse_SfxPlay(nurse, sfxIdx);
        }

        switch (nurseProps.field_118)
        {
            case 0:
                nurseProps.damage     = nurse->damage;
                nurseProps.field_114 += nurse->damage.amount;

                newHealth = nurse->health - nurse->damage.amount;
                if (newHealth < Q12(0.0f))
                {
                    newHealth = Q12(0.0f);
                }
                nurse->health = newHealth;

                if (newHealth <= Q12(120.0f))
                {
                    nurseProps.field_118++;

                    if (!PuppetNurse_SomeAngleCheck(nurse))
                    {
                        nurse->model.controlState = PuppetNurseControl_4;
                        nurse->model.stateStep    = 0;
                    }
                    else
                    {
                        nurse->model.controlState = PuppetNurseControl_3;
                        nurse->model.stateStep    = 0;
                    }

                    if (!nurse->health)
                    {
                        Savegame_EnemyStateUpdate(nurse);
                    }
                }
                else
                {
                    if (nurse->model.controlState != PuppetNurseControl_2 &&
                        ((nurseProps.field_124->field_4 < nurseProps.field_114) ||
                         (nurse->damage.amount > Q12(320.0f))))
                    {
                        nurseProps.field_114      = Q12(0.0f);
                        nurse->model.controlState = PuppetNurseControl_2;
                        nurse->model.stateStep    = 0;
                    }
                }
                break;

            case 1:
                if (nurse->health)
                {
                    PuppetNurse_SfxPlay(nurse, 1);
                    nurse->health = Q12(0.0f);
                    Savegame_EnemyStateUpdate(nurse);

                    if (nurse->model.anim.status == ANIM_STATUS(14, true))
                    {
                        nurse->model.anim.status = ANIM_STATUS(22, false);
                    }
                }

                localNurseProps.field_118++;

            case 2:
                break;
        }
    }

    Chara_DamageClear(nurse);
}

void PuppetNurse_Move(s_SubCharacter* nurse)
{
    q3_12  angleDeltaToPlayer;
    q3_12  tmpAngle;
    q3_12  absAngle;
    q19_12 distToPlayer;
    q19_12 addAngle;
    q19_12 distAbs;
    q19_12 limit;

    distToPlayer = Math_Vector2MagCalcSafeQ6(playerChara.position.vx - nurse->position.vx,
                                             playerChara.position.vz - nurse->position.vz) -
                   Q12(0.76f);
    distAbs      = ABS(distToPlayer);

    angleDeltaToPlayer = Math_AngleNormalizeSigned(Math_AngleBetweenPositionsGet(nurse->position, playerChara.position) -
                                                   nurse->rotation.vy);
    absAngle           = ABS(angleDeltaToPlayer);

    if (distAbs > Q12(0.03f))
    {
        if (distToPlayer > Q12(0.0f))
        {
            limit = Q12(0.0f);
        }
        else
        {
            limit = Q12(-0.02f);
        }

        Chara_MoveSpeedUpdate2(nurse, Q12(4.0f), limit);
    }
    else
    {
        Chara_MoveSpeedUpdate(nurse, Q12(4.0f));
    }

    if (absAngle > Q12_ANGLE(10.0f))
    {
        tmpAngle = 2;
        tmpAngle = (absAngle * tmpAngle) + Q12_ANGLE(45.0f);
        addAngle = Q12_MULT_PRECISE(g_DeltaTime, tmpAngle);
        if (angleDeltaToPlayer > Q12_ANGLE(0.0f))
        {
            nurse->rotation.vy += addAngle;
        }
        else
        {
            nurse->rotation.vy -= addAngle;
        }
    }
}

bool sharedFunc_800CE398_3_s03(s32 animStatus)
{
    return animStatus == ANIM_STATUS(PuppetNurseAnim_18, false) ||
           animStatus == ANIM_STATUS(PuppetNurseAnim_17, false) ||
           animStatus == ANIM_STATUS(PuppetNurseAnim_18, true) ||
           animStatus == ANIM_STATUS(PuppetNurseAnim_17, true);
}

void PuppetNurse_Control1(s_SubCharacter* nurse)
{
    s_SubCharacter* localNurse;

    localNurse = nurse;

    if (!nurse->model.stateStep)
    {
        if (!nurseProps.field_11E)
        {
            nurse->model.anim.status = 2;
        }
        else
        {
            if (g_SavegamePtr->gameDifficulty == GameDifficulty_Hard)
            {
                nurse->model.anim.status = ANIM_STATUS(PuppetNurseAnim_1, false);
            }
            else
            {
                nurse->model.anim.status = ANIM_STATUS(PuppetNurseAnim_21, false);
            }
        }

        nurseProps.field_11E = func_80070320();
        localNurse->model.stateStep++;
        nurseProps.field_104 = Q12(1.0f);
        return;
    }

    if (nurseProps.field_104 < Q12(0.0f) && (nurseProps.flags_122 & PuppetNurseFlag_0))
    {
        nurseProps.field_104 = Q12(100.0f);
        PuppetNurse_SfxPlay(nurse, 0);
    }

    if (localNurse->model.anim.status == ANIM_STATUS(PuppetNurseAnim_18, false))
    {
        localNurse->model.controlState = PuppetNurseControl_9;
        localNurse->model.stateStep    = 0;
    }

    if (nurseProps.field_11E && playerChara.attackReceived == NO_VALUE)
    {
        localNurse->model.controlState = PuppetNurseControl_8;
        localNurse->model.stateStep    = 0;
        return;
    }

    PuppetNurse_Move(localNurse);
    nurseProps.field_104 -= g_DeltaTime;
}

void PuppetNurse_Control2(s_SubCharacter* nurse)
{
    s32 moveSpeed;

    if (!nurse->model.stateStep)
    {
        PuppetNurse_SfxPlay(nurse, 2);
        nurse->model.anim.status = ANIM_STATUS(PuppetNurseAnim_2, false);
        nurse->model.stateStep++;
    }
    if (nurse->model.anim.status == ANIM_STATUS(PuppetNurseAnim_18, false))
    {
        nurse->model.controlState = PuppetNurseControl_9;
        nurse->model.stateStep    = 0;
    }

    Chara_MoveSpeedUpdate(nurse, Q12(4.0f));
}

void PuppetNurse_Control3_4(s_SubCharacter* nurse, bool isDoctor)
{
    s32             animStatus;
    s_SubCharacter* localNurse;

    localNurse = nurse;

    if (!nurse->model.stateStep)
    {
        if (nurse->model.anim.status == ANIM_STATUS(PuppetNurseAnim_2, true))
        {
            return;
        }

        PuppetNurse_SfxPlay(nurse, 2);

        nurse->model.anim.status = g_PuppetNurse_AnimStatus0[isDoctor];
        nurse->collision.state   = CharaCollisionState_4;
        nurse->flags            |= CharaFlag_Unk2;
        nurse->model.stateStep++;
    }

    animStatus = nurse->model.anim.status;
    if (animStatus == ANIM_STATUS(PuppetNurseAnim_14, true) || animStatus == ANIM_STATUS(PuppetNurseAnim_5, true) ||
        animStatus == ANIM_STATUS(PuppetNurseAnim_5, false) || animStatus == ANIM_STATUS(PuppetNurseAnim_15, true))
    {
        if (Chara_NpcIdxGet(nurse) != g_SysWork.targetNpcIdx && nurse->health == Q12(0.0f))
        {
            nurse->health          = NO_VALUE;
            nurse->collision.state = CharaCollisionState_Ignore;
            func_800622B8(3, nurse, g_PuppetNurse_AnimStatus1[isDoctor], 11);

            localNurseProps.flags_122 |= PuppetNurseFlag_1;
        }
        else if (nurse->model.anim.status == ANIM_STATUS(PuppetNurseAnim_15, true) && nurse->health > Q12(0.0f))
        {
            nurse->model.anim.status = ANIM_STATUS(PuppetNurseAnim_5, false);
        }
    }

    Chara_MoveSpeedUpdate(nurse, Q12(4.0f));
}

bool sharedFunc_800CE7C8_3_s03(s_SubCharacter* nurse)
{
    q19_12 dist;
    q19_12 distAbs;
    q19_12 distSubtract;
    q19_12 moveSpeed;
    q3_12  angleDeltaToPlayer;
    q3_12  headingAngleDeltaFromPlayer;
    q3_12  tmpAngle;
    q3_12  addAngle;
    q3_12  absAngleDeltaToPlayer;
    q19_12 limit;

    distSubtract          = Q12(0.78f);
    angleDeltaToPlayer    = Math_AngleNormalizeSigned(ratan2(playerChara.position.vx - nurse->position.vx,
                                                             playerChara.position.vz - nurse->position.vz) -
                                                      nurse->rotation.vy);
    absAngleDeltaToPlayer = ABS(angleDeltaToPlayer);

    headingAngleDeltaFromPlayer = Math_AngleNormalizeSigned(playerChara.rotation.vy - nurse->rotation.vy);
    headingAngleDeltaFromPlayer = ABS(headingAngleDeltaFromPlayer);

    if (headingAngleDeltaFromPlayer > Q12_ANGLE(90.0f))
    {
        distSubtract = Q12(0.85f);
    }

    dist    = Math_Vector2MagCalcSafeQ6(playerChara.position.vx - nurse->position.vx,
                                        playerChara.position.vz - nurse->position.vz) - distSubtract;
    distAbs = ABS(dist);

    if (dist > Q12(0.5f))
    {
        return false;
    }

    if (distAbs > Q12(0.2f))
    {
        limit = Q12(0.2f);
    }
    else
    {
        if (distAbs > Q12(0.05f))
        {
            if (dist > Q12(0.0f))
            {
                limit = Q12(0.6f);
            }
            else
            {
                limit = Q12(-0.6f);
            }
        }
        else
        {
            limit = Q12(0.0f);
        }
    }

    Chara_MoveSpeedUpdate2(nurse, Q12(2.0f), limit);

    if (absAngleDeltaToPlayer > Q12_ANGLE(4.0f))
    {
        tmpAngle = 2;
        tmpAngle = (absAngleDeltaToPlayer * tmpAngle) + Q12_ANGLE(45.0f);
        addAngle = Q12_MULT_PRECISE(g_DeltaTime, tmpAngle);
        if (angleDeltaToPlayer > Q12_ANGLE(0.0f))
        {
            nurse->rotation.vy += addAngle;
        }
        else
        {
            nurse->rotation.vy -= addAngle;
        }
    }

    return true;
}

void PuppetNurse_Control5(s_SubCharacter* nurse)
{
    s32 controlState;

    if (!nurse->model.stateStep)
    {
        nurse->model.anim.status = ANIM_STATUS(PuppetNurseAnim_6, false);
        nurseProps.field_104     = 0;
        nurse->model.stateStep++;
    }

    sharedFunc_800CE7C8_3_s03(nurse);

    controlState = nurse->model.stateStep;
    if (controlState == PuppetNurseControl_1)
    {
        if (nurseProps.flags_122 & PuppetNurseFlag_0)
        {
            nurse->model.stateStep = 2;
        }
        else
        {
            if (nurseProps.field_104 <= Q12(1.5f))
            {
                if (func_80070320())
                {
                    nurse->model.controlState = controlState;
                    nurse->model.stateStep    = 0;
                    nurse->model.anim.status  = ANIM_STATUS(PuppetNurseAnim_18, false);
                    return;
                }
            }
            else
            {
                nurse->model.controlState = PuppetNurseControl_13;
                nurse->model.stateStep    = 0;
                return;
            }
        }
    }
    else if (controlState == 2)
    {
        if (Chara_AttackReceivedGet(&playerChara) == NO_VALUE)
        {
            nurse->model.controlState = PuppetNurseControl_8;
            nurse->model.stateStep    = 0;
            return;
        }

        if (nurseProps.field_104 >= Q12(1.5f))
        {
            playerChara.damage.amount += Q12(D_800AD4C8[EquippedWeaponId_Unk56].field_4);
            nurseProps.field_104                       = 0;
        }
    }

    nurseProps.field_104 += g_DeltaTime;
}

void PuppetNurse_Control6_7(s_SubCharacter* nurse, bool isDoctor)
{
    if (!nurse->model.stateStep)
    {
        PuppetNurse_SfxPlay(nurse, isDoctor + 1);

        if (isDoctor)
        {
            nurse->model.anim.status = ANIM_STATUS(PuppetNurseAnim_8, false);
        }
        else
        {
            nurse->model.anim.status = ANIM_STATUS(PuppetNurseAnim_7, false);
        }

        nurse->model.stateStep++;
    }

    if (nurse->model.anim.status == ANIM_STATUS(PuppetNurseAnim_18, false))
    {
        nurse->model.controlState = PuppetNurseControl_9;
        nurse->model.stateStep    = 0;
    }

    Chara_MoveSpeedUpdate(nurse, Q12(4.0f));
}

void PuppetNurse_Control8(s_SubCharacter* nurse)
{
    s32    controlState;
    q19_12 speed;
#if !defined(M2CTX)
    register q19_12 angle asm("v1"); // @hack forced register for a match. Doesn't affect code logic.
#else
    q19_12 angle;
#endif

    switch (nurse->model.stateStep)
    {
        case 0:
            angle = Math_AngleNormalizeSigned((playerChara.rotation.vy - nurse->rotation.vy) - Q12_ANGLE(90.0f));
            if (angle < Q12_ANGLE(0.0f))
            {
                nurseProps.field_104 = -angle;
            }
            else
            {
                nurseProps.field_104 = angle;
            }

            nurse->model.stateStep++;
            break;

        case 1:
            if (nurseProps.field_104 < 0)
            {
                nurse->moveSpeed         = Q12(-1.0f);
                nurse->model.anim.status = ANIM_STATUS(10, false);
                nurse->model.stateStep   = 2;
            }

            nurseProps.field_104 -= g_DeltaTime;
            break;

        case 2:
        {
            u16 modelStates[8] = { 9, 9, 9, 6, 6, 9, 7, 7 };

            if (nurse->model.anim.status == ANIM_STATUS(18, false))
            {
                if (g_SavegamePtr->gameDifficulty == GameDifficulty_Hard)
                {
                    controlState = PuppetNurseControl_9;
                }
                else
                {
                    controlState = modelStates[(Rng_Rand16() >> 4) & 7];
                }

                nurse->model.controlState = controlState;
                nurse->model.stateStep    = 0;
            }
            else
            {
                speed = Rng_GenerateUInt(0, 0x1FF); // TODO: Combine with the +0.8f below?
                Chara_MoveSpeedUpdate(nurse, (speed + Q12(0.8f)));
            }
        }
        break;
    }
}

s32 sharedFunc_800CEEAC_3_s03(void)
{
    s32 temp_a0;

    if (!(g_SysWork.worldEnv.current.params.flags & SpecialEnvEventFlags_DarkEnvironment) && !(g_SysWork.worldEnv.current.params.flags & SpecialEnvEventFlags_FlashlightAllowed))
    {
        return 0;
    }
    else if (!(g_SysWork.worldEnv.current.params.flags & SpecialEnvEventFlags_DarkEnvironment) && (g_SysWork.worldEnv.current.params.flags & SpecialEnvEventFlags_FlashlightAllowed))
    {
        return 1;
    }
    else
    {
        return 2;
    }
}

void sharedFunc_800CEEE0_3_s03(s_SubCharacter* nurse)
{
    s32             limit;
    s16             angle;
    s_SubCharacter* localNurse;

    limit      = Q12_MULT_PRECISE(nurseProps.field_124->field_C, Q12(0.27f));
    localNurse = nurse;
    Chara_MoveSpeedUpdate2(nurse, Q12(1.4f), limit);
    limit = 0;

    if (Rng_GenerateUInt(0, 63) == 0) // 1 in 64 chance.
    {
        localNurseProps.field_11C = func_8006F99C(nurse, Q12(1.0f), nurse->rotation.vy);
    }

    angle = Math_AngleNormalizeSigned(localNurseProps.field_11C - nurse->rotation.vy);
    if (ABS(angle) > Q12_ANGLE(5.0f))
    {
        if (angle > Q12_ANGLE(0.0f))
        {
            nurse->rotation.vy += Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(90.0f));
        }
        else
        {
            nurse->rotation.vy -= Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(90.0f));
        }
    }
}

void sharedFunc_800CF0B4_3_s03(s_SubCharacter* nurse)
{
    q19_12          limit;
    q3_12           angle;
    s_SubCharacter* localNurse;

    limit      = Q12_MULT_PRECISE(nurseProps.field_124->field_C, Q12(0.27f));
    localNurse = nurse;

    Chara_MoveSpeedUpdate2(nurse, Q12(1.4f), limit);
    limit = Q12(0.0f);

    // @note Did they forget to make it an if?
    Rng_Rand16();
    localNurseProps.field_11C = Los_CharaHeadingAngleGet(nurse, Q12(1.0f),
                                                      playerChara.position.vx,
                                                      playerChara.position.vz,
                                                      Q12_ANGLE(360.0f), true);

    angle = Math_AngleNormalizeSigned(localNurseProps.field_11C - nurse->rotation.vy);
    if (ABS(angle) > Q12_ANGLE(5.0f))
    {
        if (angle > Q12_ANGLE(0.0f))
        {
            nurse->rotation.vy += Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(90.0f));
        }
        else
        {
            nurse->rotation.vy -= Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(90.0f));
        }
    }
}

bool sharedFunc_800CF294_3_s03(s_SubCharacter* nurse, q19_12 dist)
{
    s_D_800D5A8C* somePtr;

    somePtr = &sharedData_800D5A8C_3_s03[sharedFunc_800CEEAC_3_s03()];

    if (nurseProps.field_11A > Q12(0.0f))
    {
        nurseProps.field_11A -= g_DeltaTime;
    }

    if (func_80070360(nurse, dist, somePtr->field_8))
    {
        nurseProps.field_11A = Q12(1.0f);
    }

    return func_8006FD90(nurse, 1, Q12_MULT_PRECISE(somePtr->field_0, nurseProps.field_124->field_8), 
                                   Q12_MULT_PRECISE(somePtr->field_4, nurseProps.field_124->field_8)) ||
           nurseProps.field_11A > Q12(0.0f);
}

void PuppetNurse_Control9(s_SubCharacter* nurse)
{
    bool            cond;
    q19_12          dist;
    u16             rng;
    q3_12           angle;
    q3_12           angleAbs;
    s_SubCharacter* localNurse;

    localNurse = nurse;

    dist   = Math_Vector2MagCalcSafeQ6(playerChara.position.vx - nurse->position.vx,
                                       playerChara.position.vz - nurse->position.vz);

    angle    = Math_AngleNormalizeSigned(Math_AngleBetweenPositionsGet(nurse->position, playerChara.position) -
                                         nurse->rotation.vy);
    angleAbs = ABS(angle);

    if (!nurse->model.stateStep)
    {
        nurseProps.field_104 = 0;
        nurseProps.field_11C = func_8006F99C(nurse, Q12(1.0f), nurse->rotation.vy);
        nurse->model.stateStep++;
    }

    cond                    = sharedFunc_800CF294_3_s03(nurse, dist);
    nurse->field_44.field_0 = 0;

    switch (nurse->model.stateStep)
    {
        case 1:
            if (!cond)
            {
                sharedFunc_800CEEE0_3_s03(nurse);
                break;
            }
            else
            {
                nurse->model.stateStep = 2;
            }

        case 2:
            if (!cond)
            {
                nurse->model.stateStep = 1;
            }
            else
            {
                sharedFunc_800CF0B4_3_s03(nurse);

                if (dist < Q12_MULT_PRECISE(localNurseProps.field_124->field_8, Q12(3.0f)) &&
                    angleAbs < Q12_ANGLE(60.0f) && playerChara.health > Q12(0.0f))
                {
                    nurse->model.controlState = PuppetNurseControl_12;
                    nurse->model.stateStep    = 0;
                }
            }
            break;
    }

    if (nurse->model.stateStep && sharedFunc_800CE398_3_s03(nurse->model.anim.status))
    {
        if (!Rng_GenerateUInt(0, 31)) // 1 in 32 chance.
        {
            rng                       = Rng_Rand16();
            nurse->model.anim.status  = g_PuppetNurse_AnimStatus2[(rng >> 7) & 0x1];
            nurse->model.controlState = PuppetNurseControl_6;
            nurse->model.stateStep    = 0;
            return;
        }
        else
        {
            nurse->model.anim.status = g_PuppetNurse_AnimStatus3[(Rng_Rand16() >> 2) & 0x3];
        }
    }
}

s32 sharedFunc_800CF600_3_s03(s_SubCharacter* nurse)
{
    q19_12 mag;
    q19_12 speed;
    q19_12 absMag;
    q19_12 limit;

    mag    = Math_Vector2MagCalcSafeQ6(playerChara.position.vx - nurse->position.vx,
                                       playerChara.position.vz - nurse->position.vz) -
             Q12(0.76f);
    absMag = ABS(mag);
    if (absMag > Q12(0.03f) && nurse->model.anim.status != ANIM_STATUS(20, false))
    {
        if (mag > Q12(0.0f))
        {
            speed = Q12(8.0f);
            limit = Q12_MULT_PRECISE(nurseProps.field_124->field_18, Q12(1.8f));
        }
        else
        {
            limit = Q12(-0.02f);
            speed = Q12(2.0f);
        }

        Chara_MoveSpeedUpdate2(nurse, speed, limit);
    }
    else
    {
        Chara_MoveSpeedUpdate(nurse, Q12(3.0f));
    }

    return mag;
}

void sharedFunc_800CF7F4_3_s03(s_SubCharacter* nurse)
{
    int angle; // TODO: Type

    if (Rng_GenerateUInt(0, Q8(1.0f) - 1) >= Q8(0.5f))
    {
        nurseProps.field_11C = Los_CharaHeadingAngleGet(nurse, Q12(1.0f),
                                                     playerChara.position.vx,
                                                     playerChara.position.vz,
                                                     Q12_ANGLE(360.0f), true);
    }

    angle = Math_AngleNormalizeSigned(nurseProps.field_11C - nurse->rotation.vy);
    if (ABS(angle) > Q12_ANGLE(5.0f))
    {
        if (angle > Q12_ANGLE(0.0f))
        {
            nurse->rotation.vy += Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(90.0f));
        }
        else
        {
            nurse->rotation.vy -= Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(90.0f));
        }
    }
}

bool sharedFunc_800CF90C_3_s03(s_SubCharacter* nurse)
{
    s_SubCharacter* curNpc;
    s32             i;

    // Run through NPCs.
    for (i = 0; i < ARRAY_SIZE(g_SysWork.npcs); i++)
    {
        if ((g_SysWork.npcs[i].model.charaId == Chara_PuppetNurse || g_SysWork.npcs[i].model.charaId == Chara_PuppetDoctor) &&
            g_SysWork.npcs[i].health > Q12(0.0f) &&
            g_SysWork.npcs[i].field_40 != nurse->field_40 &&
            !Math_Distance2dCheck(&nurse->position, &g_SysWork.npcs[i].position, Q12(4.0f)))
        {
            curNpc = g_SysWork.npcs;

            if (!(curNpc[i].flags & CharaFlag_Unk2))
            {
                return true;
            }
        }
    }

    return false;
}

void PuppetNurse_Control12(s_SubCharacter* nurse)
{
    s32             angleDeltaToPlayer;
    s16             angleDeltaAbs;
    s32             distToPlayer;
    s32             cond;
    s32             animStatus;
    s_SubCharacter* localNurse;

    animStatus         = nurse->model.anim.status;
    angleDeltaToPlayer = Math_AngleNormalizeSigned(Math_AngleBetweenPositionsGet(nurse->position, playerChara.position) - nurse->rotation.vy);
    angleDeltaAbs      = abs(angleDeltaToPlayer);

    distToPlayer = Math_Vector2MagCalcSafeQ6((playerChara.position.vx - nurse->position.vx),
                                             (playerChara.position.vz - nurse->position.vz));

    distToPlayer = Q12_MULT_FLOAT_PRECISE(distToPlayer, 1.1f);
    cond         = sharedFunc_800CF294_3_s03(nurse, distToPlayer);

    if (playerChara.health < Q12(0.0f) || !cond)
    {
        nurse->model.controlState = 9;
        nurse->model.stateStep    = 0;
        nurse->model.anim.status  = ANIM_STATUS(18, false);
        return;
    }

    switch (nurse->model.stateStep)
    {
        case 0:
            if (angleDeltaAbs >= Q12_ANGLE(10.1f))
            {
                s32 rotAmount = Q12_ANGLE(45.0f); // @hack? Should probably be part of the `FP_MULTIPLY_PRECISE`.
                rotAmount     = Q12_MULT_PRECISE(g_DeltaTime, rotAmount);
                if (angleDeltaToPlayer > Q12_ANGLE(0.0f))
                {
                    nurse->rotation.vy += rotAmount;
                }
                else
                {
                    nurse->rotation.vy -= rotAmount;
                }

                Chara_MoveSpeedUpdate3(nurse, Q12(1.0f), Q12(0.0f));
                return;
            }

            nurse->model.stateStep = 1;

        case 1:
            if (animStatus >= ANIM_STATUS(17, false) && animStatus < ANIM_STATUS(18, false))
            {
                nurse->model.anim.status = ANIM_STATUS(9, false);
                nurse->model.stateStep++;
            }
            else if (animStatus >= ANIM_STATUS(18, false) && animStatus < ANIM_STATUS(19, false))
            {
                nurse->model.anim.status = ANIM_STATUS(20, false);
                nurse->model.stateStep++;
            }
            else
            {
                break;
            }

            sharedFunc_800CF7F4_3_s03(nurse);

        case 2:
            nurse->field_44.field_0 = 0;
            localNurse              = nurse;
            distToPlayer            = sharedFunc_800CF600_3_s03(nurse);

            if (!func_80070320() && sharedFunc_800CF90C_3_s03(nurse))
            {
                if (distToPlayer < Q12(0.1f))
                {
                    nurse->model.controlState = 5;
                    nurse->model.stateStep    = 0;
                }
            }
            else if (distToPlayer < Q12(0.15f))
            {
                if (!(playerChara.flags & 8))
                {
                    localNurseProps.field_11E = false;
                    nurse->model.controlState = 1;
                }
                else
                {
                    localNurseProps.field_11E = true;
                    nurse->model.controlState = 1;
                }

                nurse->model.stateStep   = 0;
                nurse->model.anim.status = ANIM_STATUS(18, false);
            }

            sharedFunc_800CF7F4_3_s03(nurse);
    }
}

void PuppetNurse_Control10(s_SubCharacter* nurse)
{
    u8     animStatus;
    q19_12 dist;

    if (!nurse->model.stateStep)
    {
        nurseProps.field_104     = 0;
        nurse->model.anim.status = ANIM_STATUS(19, false);
        nurse->model.stateStep++;
    }

    dist   = Math_Vector2MagCalcSafeQ6(playerChara.position.vx - nurse->position.vx,
                                       playerChara.position.vz - nurse->position.vz);

    if (sharedFunc_800CF294_3_s03(nurse, dist))
    {
        nurse->model.controlState = PuppetNurseControl_9;
        nurse->model.stateStep    = 0;

        if (Rng_Rand16() & 0x80)
        {
            animStatus = ANIM_STATUS(7, false);
        }
        else
        {
            animStatus = ANIM_STATUS(8, false);
        }

        nurse->model.anim.status = animStatus;
    }
}

void PuppetNurse_Control11(s_SubCharacter* nurse)
{
    u8              controlState;
    q19_12          dist;
    q3_12           angle;
    q19_12          tmp;
    s32             temp_s0;
    s_SubCharacter* localNurse;

    if (!nurse->model.stateStep)
    {
        nurseProps.field_104     = 0;
        nurse->model.anim.status = ANIM_STATUS(PuppetNurseAnim_17, false);
        nurseProps.field_11C     = func_8006F99C(nurse, Q12(1.0f), nurse->rotation.vy);
        nurse->model.stateStep++;
    }

    if (sharedFunc_800CE398_3_s03(nurse->model.anim.status))
    {
        if (Rng_GenerateUInt(0, 15) == 0) // 1 in 16 chance.
        {
            if (Rng_Rand16() & 0x80)
            {
                controlState = PuppetNurseControl_6;
            }
            else
            {
                controlState = PuppetNurseControl_7;
            }

            nurse->model.controlState = controlState;
            nurse->model.stateStep    = 0;
        }
        else
        {
            nurse->model.anim.status = g_PuppetNurse_AnimStatus4[(Rng_Rand16() >> 2) & 3];
        }
    }

    dist   = Math_Vector2MagCalcSafeQ6(playerChara.position.vx - nurse->position.vx,
                                       playerChara.position.vz - nurse->position.vz);

    if (sharedFunc_800CF294_3_s03(nurse, dist))
    {
        nurse->model.controlState = PuppetNurseControl_9;
        nurse->model.stateStep    = 0;
        return;
    }

    localNurse = nurse;

    Chara_MoveSpeedUpdate3(nurse, Q12(1.4f), Q12(0.27f));

    if (Rng_GenerateUInt(0, 63) == 0) // 1 in 64 chance.
    {
        tmp                       = Q12(1.5f);
        temp_s0                   = localNurseProps.field_108 + ((Rng_Rand16() % Q12(3.0f)) - tmp);
        localNurseProps.field_11C = Los_CharaHeadingAngleGet(nurse, Q12(1.0f), 
                                                          temp_s0, 
                                                          localNurseProps.field_10C + ((Rng_Rand16() % Q12(3.0f)) - tmp),
                                                          Q12_ANGLE(360.0f), true);
    }

    angle = Math_AngleNormalizeSigned(localNurseProps.field_11C - nurse->rotation.vy);
    if (ABS(angle) > Q12_ANGLE(5.0f))
    {
        if (angle > Q12_ANGLE(0.0f))
        {
            nurse->rotation.vy += Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(90.0f));
        }
        else
        {
            nurse->rotation.vy -= Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(90.0f));
        }
    }
}

void PuppetNurse_Control13(s_SubCharacter* nurse)
{
    if (nurse->model.stateStep == 0)
    {
        nurse->model.anim.status = ANIM_STATUS(PuppetNurseAnim_10, false);
        nurse->model.stateStep++;
    }

    if (nurse->model.anim.status == ANIM_STATUS(PuppetNurseAnim_18, false))
    {
        nurse->model.controlState = PuppetNurseControl_9;
        nurse->model.stateStep    = 0;
    }

    Chara_MoveSpeedUpdate3(nurse, Q12(4.0f), Q12(0.0f));
}

void PuppetNurse_Control(s_SubCharacter* nurse)
{
    // Handle control state.
    switch (nurse->model.controlState)
    {
        case PuppetNurseControl_1:
            PuppetNurse_Control1(nurse);
            break;

        case PuppetNurseControl_2:
            PuppetNurse_Control2(nurse);
            break;

        case PuppetNurseControl_3:
            PuppetNurse_Control3_4(nurse, true);
            break;

        case PuppetNurseControl_4:
            PuppetNurse_Control3_4(nurse, false);
            break;

        case PuppetNurseControl_5:
            PuppetNurse_Control5(nurse);
            break;

        case PuppetNurseControl_6:
            PuppetNurse_Control6_7(nurse, false);
            break;

        case PuppetNurseControl_7:
            PuppetNurse_Control6_7(nurse, true);
            break;

        case PuppetNurseControl_8:
            PuppetNurse_Control8(nurse);
            break;

        case PuppetNurseControl_9:
            PuppetNurse_Control9(nurse);
            break;

        case PuppetNurseControl_10:
            PuppetNurse_Control10(nurse);
            break;

        case PuppetNurseControl_11:
            PuppetNurse_Control11(nurse);
            break;

        case PuppetNurseControl_12:
            PuppetNurse_Control12(nurse);
            break;

        case PuppetNurseControl_13:
            PuppetNurse_Control13(nurse);
            break;
    }
}

void sharedFunc_800D02E4_3_s03(s_SubCharacter* nurse, GsCOORDINATE2* boneCoords)
{
    VECTOR3         pos;
    MATRIX          mat; // "Hierarchy matrix"?
    s32             weaponAttack;
    q19_12          posX;
    q19_12          posY;
    q19_12          posZ;
    s_SubCharacter* localNurse;

    localNurse            = nurse;
    nurseProps.flags_122 &= ~PuppetNurseFlag_0;

    if (localNurse->model.controlState == PuppetNurseControl_1 ||
        localNurse->model.controlState == PuppetNurseControl_5)
    {
        Vw_CoordHierarchyMatrixCompute(&boneCoords[10], &mat);
        posX = Q8_TO_Q12(mat.t[0]);
        posY = Q8_TO_Q12(mat.t[1]);
        posZ = Q8_TO_Q12(mat.t[2]);

        nurse->collision.box.field_8 = nurse->position.vy - posY;
        pos.vx                       = posX;
        pos.vy                       = posY;
        pos.vz                       = posZ;

        if (localNurse->model.controlState == PuppetNurseControl_1)
        {
            weaponAttack = localNurseProps.field_124->field_20;
        }
        else
        {
            // TODO: What's weapon attack 56?
            weaponAttack = EquippedWeaponId_Unk56;
        }

        if (func_8008A0E4(1, weaponAttack, nurse, &pos, &playerChara, nurse->rotation.vy, Q12_ANGLE(90.0f)) != NO_VALUE)
        {
            localNurseProps.flags_122 |= PuppetNurseFlag_0;
        }
    }
}

void sharedFunc_800D03E4_3_s03(s_SubCharacter* nurse)
{
    s_CollisionResult collResult;
    VECTOR            damagePos; // Q19.12
    VECTOR            dir;       // Q19.12
    VECTOR            unkPos;
    q19_12            moveSpeed;
    q19_12            damagePosComp;
    q19_12            temp_s1;
    q19_12            unkPosX;
    q19_12            unkPosY;
    q19_12            unkPosZ;

    temp_s1 = nurseProps.field_124->field_2C;

    damagePos.vx = nurseProps.damage.position.vx;
    damagePos.vz = nurseProps.damage.position.vz;
    damagePos.vy = nurseProps.damage.position.vy;

    moveSpeed            = nurse->moveSpeed;
    nurseProps.moveSpeed = moveSpeed;

    dir.vx = Math_Sin(nurse->rotation.vy);
    dir.vz = Math_Cos(nurse->rotation.vy);
    dir.vy = Q12(0.0f);

    unkPosX = Q12_MULT_PRECISE(temp_s1, damagePos.vx) + Q12_MULT_PRECISE(moveSpeed, dir.vx);
    unkPosY = Q12_MULT_PRECISE(temp_s1, damagePos.vy);
    unkPosZ = Q12_MULT_PRECISE(temp_s1, damagePos.vz) + Q12_MULT_PRECISE(moveSpeed, dir.vz);

    unkPos.vx = unkPosX;
    unkPos.vy = unkPosY;
    unkPos.vz = unkPosZ;

    nurse->moveSpeed    = SquareRoot12(Q12_MULT_PRECISE(unkPosX, unkPosX) + Q12_MULT_PRECISE(unkPosZ, unkPosZ));
    nurse->headingAngle = ratan2(unkPosX, unkPosZ);
    nurse->fallSpeed   += g_GravitySpeed;

    Chara_MovementUpdate0(nurse, &collResult);

    damagePosComp                 = nurseProps.damage.position.vx;
    nurse->moveSpeed              = nurseProps.moveSpeed;
    nurseProps.damage.position.vx = SquareRoot12(Q12_MULT_PRECISE(damagePosComp, damagePosComp) >> g_VBlanks);

    if (damagePosComp <= Q12(0.0f))
    {
        nurseProps.damage.position.vx = -nurseProps.damage.position.vx;
    }

    damagePosComp                 = nurseProps.damage.position.vy;
    nurseProps.damage.position.vy = SquareRoot12(Q12_MULT_PRECISE(damagePosComp, damagePosComp) >> g_VBlanks);

    if (damagePosComp <= Q12(0.0f))
    {
        nurseProps.damage.position.vy = -nurseProps.damage.position.vy;
    }

    damagePosComp = nurseProps.damage.position.vz;

    nurseProps.damage.position.vz = SquareRoot12(Q12_MULT_PRECISE(damagePosComp, damagePosComp) >> g_VBlanks);
    if (damagePosComp <= Q12(0.0f))
    {
        nurseProps.damage.position.vz = -nurseProps.damage.position.vz;
    }

    nurse->rotation.vy = Math_AngleNormalizeSigned(nurse->rotation.vy);
}

void PuppetNurse_AnimUpdate(s_SubCharacter* nurse, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords)
{
    s32         sfxIdx0;
    s32         sfxIdx1;
    q19_12      angle;
    s32         i;
    s32         j;
    s_AnimInfo* animInfo;
    s_AnimInfo* animInfoBase;

    animInfoBase = nurseProps.field_124->animInfo_24;
    sfxIdx0      = PuppetNurse_AnimSfxGet(FP_FROM(nurse->model.anim.time, Q12_SHIFT));

    WorldGfx_CharaMeshSwap(nurse->model.charaId, MESH_SWAP_STATUS(PuppetNurseSwappableMesh_Head, nurseProps.modelVariantIdx));
    Math_MatrixTransform(&nurse->position, &nurse->rotation, boneCoords);

    if (nurse->model.anim.status != ANIM_STATUS(PuppetNurseAnim_Still, false))
    {
        (&animInfoBase[nurse->model.anim.status])->playbackFunc(&nurse->model, anmHdr, boneCoords, &animInfoBase[nurse->model.anim.status]);
    }

    angle = nurseProps.field_124->field_18;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            boneCoords[0].coord.m[i][j] = Q12_MULT_PRECISE(angle, boneCoords[0].coord.m[i][j]);
        }
    }

    sfxIdx1 = PuppetNurse_AnimSfxGet(FP_FROM(nurse->model.anim.time, Q12_SHIFT));
    if (sfxIdx1 != sfxIdx0 && sfxIdx1 != 9)
    {
        PuppetNurse_SfxPlay(nurse, sfxIdx1);
    }
}

void sharedFunc_800D0828_3_s03(s_SubCharacter* nurse, GsCOORDINATE2* boneCoords)
{
    typedef enum _BoneMatIdx
    {
        BoneMatIdx_Torso     = 0,
        BoneMatIdx_Head      = 1,
        BoneMatIdx_RightShin = 2,
        BoneMatIdx_LeftShin  = 3,

        BoneMatIdx_Count = 4
    } e_BoneMatIdx;

    MATRIX          boneMats[BoneMatIdx_Count];
    VECTOR3         unkPos;
    q19_12          boxOffsetX;
    q19_12          boxOffsetZ;
    q19_12          cylinderRadius;
    q19_12          posY;
    q19_12          offsetPosY;
    q19_12          torsoPosY;
    q19_12          headPosY;
    q19_12          rightShinPosY;
    q19_12          leftShinPosY;
    s_SubCharacter* localNurse;

    localNurse = nurse;

    // Get torso, head, right shin, and left shin bone matrices. TODO: Not Harry.
    Vw_CoordHierarchyMatrixCompute(&boneCoords[HarryBone_Torso], &boneMats[BoneMatIdx_Torso]);
    Vw_CoordHierarchyMatrixCompute(&boneCoords[HarryBone_Head], &boneMats[BoneMatIdx_Head]);
    Vw_CoordHierarchyMatrixCompute(&boneCoords[HarryBone_RightShin], &boneMats[BoneMatIdx_RightShin]);
    Vw_CoordHierarchyMatrixCompute(&boneCoords[HarryBone_LeftShin], &boneMats[BoneMatIdx_LeftShin]);

    posY                     = localNurse->position.vy;
    rightShinPosY            = Q8_TO_Q12(boneMats[BoneMatIdx_RightShin].t[1]);
    leftShinPosY             = Q8_TO_Q12(boneMats[BoneMatIdx_LeftShin].t[1]);
    nurse->collision.box.topExtent = Q8_TO_Q12(boneMats[BoneMatIdx_Head].t[1]);

    offsetPosY                  = posY + Q12(0.25f);
    nurse->collision.box.topExtent   -= offsetPosY;
    nurse->collision.box.bottomExtent = posY;

    if (rightShinPosY >= leftShinPosY)
    {
        nurse->collision.box.height = leftShinPosY - nurse->collision.box.bottomExtent;
    }
    else
    {
        nurse->collision.box.height = rightShinPosY - nurse->collision.box.bottomExtent;
    }

    // `((torsoPosY - headPosY) / 2) - posY`
    nurse->collision.box.offsetY = ((Q8_TO_Q12(boneMats[BoneMatIdx_Head].t[1]) + Q8_TO_Q12(boneMats[BoneMatIdx_Torso].t[1])) / 2) - posY;

    cylinderRadius = sharedFunc_800CD6B0_3_s03(boneMats, ARRAY_SIZE(boneMats), &unkPos);
    boxOffsetX     = unkPos.vx - nurse->position.vx;
    boxOffsetZ     = unkPos.vz - nurse->position.vz;

    cylinderRadius                        = sharedFunc_800CD940_3_s03(cylinderRadius + Q12(0.05f), nurse->collision.cylinder.outerRadius);
    nurse->collision.cylinder.innerRadius = cylinderRadius - Q12(0.05f);
    nurse->collision.cylinder.outerRadius = cylinderRadius;
    nurse->collision.shapeOffsets.box.vx  = boxOffsetX;
    nurse->collision.shapeOffsets.box.vz  = boxOffsetZ;

    sharedFunc_800CD920_3_s03(nurse, boxOffsetX, boxOffsetZ);

    localNurseProps.position_E8.vx = nurse->position.vx;
    localNurseProps.position_E8.vz = nurse->position.vz;
}

void sharedFunc_800D0968_3_s03(s_SubCharacter* nurse, GsCOORDINATE2* boneCoords)
{
    q19_12 posY;

    if (nurse->model.anim.flags & AnimFlag_Visible)
    {
        sharedFunc_800D0828_3_s03(nurse, boneCoords);
        return;
    }

    posY = nurse->position.vy;

    // Update collision box.
    nurse->collision.box.bottomExtent  = posY;
    nurse->collision.box.height  = posY;
    nurse->collision.box.topExtent     = posY - Q12(1.7f);
    nurse->collision.box.offsetY = posY - Q12(1.0f);
}
