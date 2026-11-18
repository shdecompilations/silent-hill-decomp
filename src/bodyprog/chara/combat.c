#include "bodyprog/game.h"
#include "inline_no_dmpsx.h"

#include <psyq/libpad.h>
#include <psyq/strings.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/chara/combat.h"
#include "bodyprog/screen/data.h"
#include "bodyprog/screen/draw.h"
#include "bodyprog/world/effects.h"
#include "bodyprog/items/item_screens.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player/player_control.h"
#include "bodyprog/sound/sd_call.h"
#include "main/fs_queue.h"

// Used by `Combat_AttackUpdate`.
q3_12 D_800AFD1C[] =
{
    Q12_ANGLE(360.0f), Q12_ANGLE(360.0f), Q12_ANGLE(180.0f), Q12_ANGLE(120.0f),
    Q12_ANGLE(90.0f),  Q12_ANGLE(72.0f),  Q12_ANGLE(60.0f),  Q12_ANGLE(51.5f),
    Q12_ANGLE(45.0f),  Q12_ANGLE(40.0f),  Q12_ANGLE(36.0f),  Q12_ANGLE(32.7f),
    Q12_ANGLE(30.0f),  Q12_ANGLE(27.7f),  Q12_ANGLE(25.7f),  Q12_ANGLE(24.0f)
};

// ========================================
// COMBAT RELATED?
// ========================================

u32 Math_SquareRootQ12(s32 x) // 0x8008A058
{
    // @hack
    register s32  temp_t0 asm("t0");
    register u32  var_t1  asm("t1");
    register u32  var_t2  asm("t2");
    register s16* ptr     asm("t3");

    var_t1 = 0;

    if (x)
    {
        gte_ldlzc(x);

        var_t2  = var_t1;
        ptr = SQRT;

        if (x > 0)
        {
            gte_stlzcr(var_t2);
        }

        var_t1 = x;

        var_t2 &= 0x1E;
        temp_t0 = var_t2 - 24;

        var_t2 = -temp_t0;

        if (temp_t0 >= 0)
        {
            var_t2 = var_t1 << temp_t0;
        }
        else
        {
            var_t2 = var_t1 >> var_t2;
        }

        var_t2 -= 0x40;

        ptr += var_t2;

        var_t2 = *ptr;

        temp_t0  += 6;
        temp_t0 >>= 1;

        var_t1 = -temp_t0;

        if (temp_t0 >= 0)
        {
            var_t1 = var_t2 >> temp_t0;
        }
        else
        {
            var_t1 = var_t2 << var_t1;
        }
    }

    return var_t1;
}

s32 func_8008A0CC(void) // 0x8008A0CC
{
    return 0;
}

s64 func_8008A0D4(void) // 0x8008A0D4
{
    return 0;
}

s32 Combat_AttackPerform(e_CharaAttackState request, s32 weaponAttack, s_SubCharacter* chara, VECTOR3* attackPos,
                         s_SubCharacter* unused, q3_12 aimYaw, q3_12 aimPitch) // 0x8008A0E4
{
    #define DEFAULT_SHOT_COUNT 100

    e_CharaAttackState prevState;
    s32                hitFlags;
    s32                ret;
    s32                firstNpcBit;
    s32                count;
    s_ModelAnim*       modelAnim;

    prevState = chara->attack.state;
    modelAnim = &chara->model.anim;

    if (g_DeltaTime == Q12(0.0f) || g_SysWork.sysState != SysState_Gameplay)
    {
        return NO_VALUE;
    }

    if (chara == &PLAYER_CHARA())
    {
        s_AnimInfo* baseAnimInfos = D_800297B8;
        s_AnimInfo* mapAnimInfos  = g_MapOverlayHdr.harryMapAnimInfos;

        modelAnim->baseAnimInfos      = baseAnimInfos;
        modelAnim->mapAnimStatusStart = ANIM_STATUS(38, false); // TODO: 38 is the first Harry anim after base anims. Make a constant?
        modelAnim->mapAnimInfos       = mapAnimInfos;

        modelAnim                     = &PLAYER_EXTRA().model.anim;
        modelAnim->baseAnimInfos      = baseAnimInfos;
        modelAnim->mapAnimInfos       = mapAnimInfos;
        modelAnim->mapAnimStatusStart = ANIM_STATUS(38, false);
    }

    if (request <= CharaAttackState_Idle)
    {
        prevState           = CharaAttackState_Idle;
        chara->attack.state = CharaAttackState_Idle;
    }

    chara->attack.weaponAttack = weaponAttack;

    if (!(modelAnim->status & (1 << 0)))
    {
        chara->attack.state    = CharaAttackState_Idle;
        chara->attack.hitFlags = 0;
    }
    else if (prevState == CharaAttackState_Idle && request > CharaAttackState_Idle)
    {
        chara->attack.state    = CharaAttackState_Requested;
        chara->attack.hitFlags = 0;
    }

    chara->attack.shotsLeft = DEFAULT_SHOT_COUNT;
    chara->attack.aimYaw    = aimYaw;
    chara->attack.aimPitch  = aimPitch;
    chara->attack.origin    = *attackPos;
    hitFlags                = Combat_AttackUpdate(chara);
    ret                     = NO_VALUE;

    if (chara == &PLAYER_CHARA())
    {
        count       = NPC_COUNT_MAX;
        firstNpcBit = 1;
    }
    else
    {
        count       = 1;
        firstNpcBit = NO_VALUE;
    }

    if (hitFlags != 0)
    {
        s32 npcBit = firstNpcBit;

        while (count > 0)
        {
            if (hitFlags & npcBit)
            {
                ret = count;
                break;
            }

            npcBit <<= 1;
            count--;
        }
    }

    if (chara->model.charaId != Chara_Harry)
    {
        if (PLAYER_CHARA().attackReceived != NO_VALUE)
        {
            ret = 0;
        }
        else
        {
            ret = NO_VALUE;
        }
    }
    return ret;

    #undef DEFAULT_SHOT_COUNT
}

u32 Combat_AttackStartKeyframeGet(s32 weaponAttack) // 0x8008A270
{
#if VERSION_EQUAL_OR_NEWER(USA)
    switch (weaponAttack)
    {
        case WEAPON_ATTACK(AttackType_RockDrill, AttackInputType_Tap):
        case WEAPON_ATTACK(AttackType_Chainsaw,  AttackInputType_Tap):
        case WEAPON_ATTACK(AttackType_RockDrill, AttackInputType_Hold):
        case WEAPON_ATTACK(AttackType_Chainsaw,  AttackInputType_Hold):
        case WEAPON_ATTACK(AttackType_RockDrill, AttackInputType_Multitap):
        case WEAPON_ATTACK(AttackType_Chainsaw,  AttackInputType_Multitap):
            if (g_SysWork.gasWeaponEngineSfxLevel > Q12(256.0f))
            {
                return 0;
            }
            break;

        default:
            break;
    }
#else
    if (weaponAttack != WEAPON_ATTACK(AttackType_PuppetNurse,  AttackInputType_Tap) &&
        weaponAttack != WEAPON_ATTACK(AttackType_PuppetDoctor, AttackInputType_Tap))
    {
        if (g_SysWork.gasWeaponEngineSfxLevel > Q12(256.0f))
        {
            return 0;
        }
    }
#endif

    return g_Items_WeaponAttackInfos[weaponAttack].hitStartKeyframe;
}

u32 Combat_AttackKeyframeCountGet(s32 weaponAttack) // 0x8008A2E0
{
    if (g_SysWork.gasWeaponEngineSfxLevel > Q12(256.0f))
    {
        switch (weaponAttack)
        {
            case WEAPON_ATTACK(AttackType_Chainsaw, AttackInputType_Tap):
            case WEAPON_ATTACK(AttackType_Chainsaw, AttackInputType_Multitap):
                return 18;

            case WEAPON_ATTACK(AttackType_Chainsaw, AttackInputType_Hold):
                return 15;

            case WEAPON_ATTACK(AttackType_RockDrill, AttackInputType_Tap):
            case WEAPON_ATTACK(AttackType_RockDrill, AttackInputType_Hold):
            case WEAPON_ATTACK(AttackType_RockDrill, AttackInputType_Multitap):
                return 13;
        }
    }

    return g_Items_WeaponAttackInfos[weaponAttack].hitKeyframeCount;
}

bool Combat_AttackHitWindowCheck(s_AttackInfo* attackInfo, s32 keyframe) // 0x8008A35C
{
    s32  hitEnd;
    bool result;

    hitEnd = attackInfo->hitStartKeyframe + attackInfo->hitKeyframeCount;
    result = false;
    if (keyframe >= attackInfo->hitStartKeyframe)
    {
        result = (keyframe < hitEnd);
    }

    return result;
}

void Combat_CharaAttackTickedClear(s_SubCharacter* chara) // 0x8008A384
{
    chara->flags &= ~CharaFlag_AttackTicked;
}

void Combat_CharaAttackTickedSet(s_SubCharacter* chara) // 0x8008A398
{
    chara->flags |= CharaFlag_AttackTicked;
}

void Combat_AttackCleanup(s_SubCharacter* chara) // 0x8008A3AC
{
    if (!(chara->flags & CharaFlag_AttackTicked))
    {
        chara->attack.state = CharaAttackState_Idle;
        Combat_AttackUpdate(chara);
    }
}

s32 Combat_AttackUpdate(s_SubCharacter* attacker) // 0x8008A3E0
{
    s32            state;
    u32            attackType;
    s32            weaponAttack;
    bool           isDone;
    s_AttackInfo*  attackInfo;
    s_ModelAnim*   modelAnim;
    s_AnimInfo*    animInfo;
    q19_12         aimYaw;
    q19_12         aimPitch;
    q19_12         hitWindowStart;
    s32            hitKeyframeCount;
    q19_12         hitCheckTime;
    q19_12         attackTime;
    q19_12         hitWindowEnd;
    s32            targetDistLevel;
    q19_12         shotInterval;
    u32            shotCountMax;
    s32            shotsLeft;
    u32            shotsFired;
    s32            pelletCount;
    q19_12         inaccuracy;
    s_CharaAttack* attack;

    static s_RayTrace shotTrace;
    static DVECTOR    stepAims[4];    // Yaw (`vx`) and pitch (`vy`) for each hit check step.
    static VECTOR3    stepOrigins[4]; // Q19.12 | Attack origin for each hit check step.

    targetDistLevel = NO_VALUE;
    Combat_CharaAttackTickedSet(attacker);

    state        = attacker->attack.state;
    weaponAttack = attacker->attack.weaponAttack;

    if (state == CharaAttackState_Idle)
    {
        attacker->attack.shotsFired   = 0;
        attacker->attack.hitFlags     = 0;
        attacker->attack.hitCheckTime = 0;
        return 0;
    }

    if (state == CharaAttackState_Finished)
    {
        attacker->attack.hitFlags     = 0;
        attacker->attack.hitCheckTime = 0;
        return 0;
    }

    isDone = false;

    attackInfo = &g_Items_WeaponAttackInfos[weaponAttack];
    attackType = attackInfo->type;

    if (attacker == &PLAYER_CHARA())
    {
        modelAnim = &PLAYER_EXTRA().model.anim;
    }
    else
    {
        modelAnim = &attacker->model.anim;
    }

    animInfo    = Anim_InfoGet(modelAnim);
    attackTime  = modelAnim->time;
    attackTime -= Q12(animInfo->startKeyframeIdx);

    hitWindowStart   = Combat_AttackStartKeyframeGet(weaponAttack);
    hitKeyframeCount = Combat_AttackKeyframeCountGet(weaponAttack);
    hitWindowEnd     = Q12(hitWindowStart + hitKeyframeCount);
    hitWindowStart   = Q12(hitWindowStart);

    hitCheckTime = attacker->attack.hitCheckTime;

    // Anim time went backwards, so the attack anim must have restarted.
    if (attackTime < hitCheckTime)
    {
        state                  = CharaAttackState_Requested;
        attacker->attack.state = state;
    }

    if (state == CharaAttackState_Requested)
    {
        attacker->attack.shotsFired = 0;
        state                       = CharaAttackState_Started;
    }

    // `hitCheckTime` is how far hit checks have got, keep it within window.
    if (hitCheckTime < hitWindowStart)
    {
        hitCheckTime                  = hitWindowStart;
        attacker->attack.hitCheckTime = hitCheckTime;
    }
    else if (hitWindowEnd < hitCheckTime)
    {
        hitCheckTime                  = hitWindowEnd;
        attacker->attack.hitCheckTime = hitCheckTime;
    }

    // Window hasn't opened yet.
    if (attackTime < hitWindowStart)
    {
        return 0;
    }

    // Paused.
    if (g_DeltaTime == Q12(0.0f))
    {
        return attacker->attack.hitFlags;
    }

    if (attackTime >= hitWindowEnd)
    {
        attackTime = hitWindowEnd;
        isDone     = true;
    }

    aimYaw   = attacker->attack.aimYaw;
    aimPitch = attacker->attack.aimPitch;

    if (attackType == AttackInfoType_Gunshot)
    {
        q19_12 pelletAngleStep;
        q19_12 ringAngle;
        q19_12 rangeMax;
        q19_12 pelletAngle;

        shotsLeft    = attacker->attack.shotsLeft;
        shotsFired   = attacker->attack.shotsFired;
        shotInterval = attackInfo->hitCheck.shot.interval;
        rangeMax     = attackInfo->rangeMax;

        // Only weapons with a shot interval get more than one shot per attack.
        if (shotInterval != 0)
        {
            shotCountMax = attackInfo->hitCheck.shot.shotCountMax;
            if (shotCountMax == 0)
            {
                shotCountMax = 1;
            }
        }
        else
        {
            shotCountMax = 1;
        }

        inaccuracy  = attackInfo->hitCheck.shot.inaccuracy;
        pelletCount = attackInfo->hitCheck.shot.pelletCount;

        // Upper 4 bits of `spread` are the pellet step count (0 means 16), lower 4 pick the ring angle.
        ringAngle       = attackInfo->hitCheck.shot.spread;
        pelletAngleStep = ringAngle >> 4;
        pelletAngleStep--;
        pelletAngleStep &= 0xF;
        ringAngle        = D_800AFD1C[ringAngle & 0xF];
        pelletAngleStep++;
        pelletAngleStep = pelletAngleStep * ringAngle;

        if (shotInterval == 0 && pelletCount == 1)
        {
            q19_12 aimTime;
            q19_12 spreadRadius;
            q19_12 rangeThird;

            // Single shot weapons get tighter the longer the aim is held, capped at 15 degrees.
            // Darkness makes it worse.
            aimTime = (attacker->model.charaId == Chara_Harry) ? func_8007FD2C() : Q12(2.0f);
            if (aimTime < Q12(0.5f))
            {
                aimTime = Q12(0.5f);
            }
            else if (aimTime > Q12(2.0f))
            {
                aimTime = Q12(2.0f);
            }

            spreadRadius = Q12_MULT_PRECISE(Q12(aimTime - Q12(0.5f)) / Q12(1.5f), Q12(-1.5f)) + Q12(1.75f);
            spreadRadius = Q12_MULT_PRECISE(spreadRadius, inaccuracy);

            if (spreadRadius < Q12_ANGLE(15.0f))
            {
                if (WorldEnv_IsInDarkness())
                {
                    spreadRadius = (spreadRadius + Q12_ANGLE(15.0f)) / 2;
                }
            }
            else
            {
                spreadRadius = Q12_ANGLE(15.0f);
            }

            rangeThird      = rangeMax / 3;
            targetDistLevel = NO_VALUE;

            if (g_SysWork.targetNpcIdx >= 0)
            {
                q19_12          distSqr;
                s_SubCharacter* target;

                target = &NPC_CHARA(g_SysWork.targetNpcIdx);

                do
                {
                    if (target->model.charaId < Chara_AirScreamer)
                    {
                        break;
                    }

                    if (target->model.charaId > CHARA_LAST_ENEMY_ID)
                    {
                        break;
                    }

                    distSqr = Q12_SQUARE_PRECISE(attacker->position.vx - target->position.vx) +
                              Q12_SQUARE_PRECISE(attacker->position.vy - target->position.vy) +
                              Q12_SQUARE_PRECISE(attacker->position.vz - target->position.vz);
                    targetDistLevel = SquareRoot12(distSqr);

                    if (target->flags & CharaFlag_Unk2)
                    {
                        distSqr -= Q12(0.5f);
                        if (distSqr < 0)
                        {
                            spreadRadius = 0;
                        }
                        else if (distSqr < Q12(16.0f))
                        {
                            spreadRadius = Q12_MULT_PRECISE(spreadRadius, Q12_DIV(distSqr, Q12(16.0f)));
                        }
                    }

                    // Make closer targets easier to hit. AAlso works out the range stat for ranking.
                    if (WorldEnv_IsInDarkness())
                    {
                        targetDistLevel = 3;
                    }
                    else if (rangeThird < targetDistLevel)
                    {
                        targetDistLevel = 2;
                    }
                    else if (targetDistLevel > Q12(2.0f))
                    {
                        targetDistLevel = 1;
                        spreadRadius   -= spreadRadius / 2;
                    }
                    else
                    {
                        targetDistLevel = 0;
                        spreadRadius    = 0;
                    }
                } while (0);
            }

            inaccuracy = Q12_MULT_PRECISE(spreadRadius, Rng_RandQ12());
        }

        pelletAngle = Q12_MULT(pelletAngleStep, state - CharaAttackState_Started);

        for (hitCheckTime += shotInterval;
             shotsLeft != 0 && shotsFired < shotCountMax && attackTime >= hitCheckTime;
             hitCheckTime += shotInterval, shotsFired++, shotsLeft--)
        {
            s32 pelletsLeft;

            for (pelletsLeft = pelletCount; pelletsLeft > 0; pelletsLeft--)
            {
                q19_12          pelletOffset;
                q19_12          ringOffsetAngle;
                q19_12          pelletYaw;
                q19_12          pelletPitch;
                q19_12          sinPelletPitch;
                q19_12          pelletDirX;
                q19_12          pelletDirY;
                q19_12          pelletDirZ;
                bool            hasHit;
                s_SubCharacter* hitChara;

                // Random offset from the aim, in a random direction around the pellet's spot on the ring.
                pelletOffset    = Q12_MULT_PRECISE(inaccuracy, Rng_RandQ12());
                ringOffsetAngle = pelletAngle + Q12_MULT_PRECISE(ringAngle, Rng_RandQ12() - Q12_ANGLE(180.0f));

                pelletYaw      = aimYaw + Q12_MULT(pelletOffset, Math_Sin(ringOffsetAngle));
                pelletPitch    = aimPitch + Q12_MULT(pelletOffset, Math_Cos(ringOffsetAngle));
                pelletDirY     = Math_Cos(pelletPitch);
                sinPelletPitch = Math_Sin(pelletPitch);
                pelletDirX     = Q12_MULT(sinPelletPitch, Math_Sin(pelletYaw));
                pelletDirZ     = Q12_MULT(sinPelletPitch, Math_Cos(pelletYaw));

                attacker->attack.hitLineVec[0].vx = Q12_MULT(rangeMax, pelletDirX);
                attacker->attack.hitLineVec[0].vy = Q12_MULT(rangeMax, pelletDirY);
                attacker->attack.hitLineVec[0].vz = Q12_MULT(rangeMax, pelletDirZ);

                pelletAngle += pelletAngleStep;

                if (attacker->model.charaId == Chara_Harry)
                {
                    func_800892A4(5);
                }

                hasHit   = Ray_GeomAndCharaTraceGet(&shotTrace, &attacker->attack.origin, &attacker->attack.hitLineVec[0], attacker);
                hitChara = shotTrace.character;
                if (hasHit && hitChara != NULL)
                {
                    attacker->attack.hitLineVec[1].vx = pelletDirX;
                    attacker->attack.hitLineVec[1].vy = pelletDirY;
                    attacker->attack.hitLineVec[1].vz = pelletDirZ;
                    Combat_HitApply(attacker, hitChara, &shotTrace.target, 0);
                }
            }
        }

        // Rewind to the last shot that went off. Next tick carries on from there.
        if (hitCheckTime >= hitWindowEnd)
        {
            isDone = true;
        }
        else
        {
            hitCheckTime -= shotInterval;
        }

        // Out of shots for this attack.
        if (shotsLeft == 0)
        {
            hitCheckTime = attackTime;
            isDone       = true;
        }

        if (shotsFired >= shotCountMax)
        {
            hitCheckTime = attackTime;
            isDone       = true;
        }

        attacker->attack.hitCheckTime = hitCheckTime;
        attacker->attack.shotsFired   = shotsFired;
        attacker->attack.shotsLeft    = shotsLeft;
    }
    else
    {
        bool   stopOnHit;
        q19_12 damageScale;
        q19_12 prevAimYaw;
        q19_12 prevAimPitch;
        q19_12 prevOriginX;
        q19_12 prevOriginY;
        q19_12 prevOriginZ;
        q19_12 originX;
        q19_12 originY;
        q19_12 originZ;
        q19_12 prevDirX;
        q19_12 prevDirY;
        q19_12 prevDirZ;
        q19_12 dirX;
        q19_12 dirY;
        q19_12 dirZ;
        s32    stepWeight;
        q19_12 prevOriginSumX;
        q19_12 prevOriginSumZ;
        q19_12 prevOriginSumY;
        q19_12 prevDirSumX;
        q19_12 prevDirSumY;
        q19_12 prevDirSumZ;
        s32    stepsLeft;
        bool   stopped;

        if (hitKeyframeCount <= 0)
        {
            hitKeyframeCount = 1;
        }

        // Slash attacks spread their damage over the whole window, so each step only gets its share of
        // the time since last tick.
        if (attackType == AttackInfoType_Slash)
        {
            stopOnHit = false;
            if (hitCheckTime < attackTime)
            {
                damageScale = (attackTime - hitCheckTime) / (hitKeyframeCount * 4);
            }
            else
            {
                damageScale = Q12(0.0f);
            }
        }
        else
        {
            stopOnHit   = true;
            damageScale = Q12(1.0f);
        }

        if (state == CharaAttackState_Started)
        {
            Combat_AttackHitLineInit(attacker);
            state = CharaAttackState_Active;
        }

        prevAimYaw   = attacker->attack.prevAimYaw;
        prevAimPitch = attacker->attack.prevAimPitch;

        prevOriginX = attacker->attack.prevOrigin.vx;
        prevOriginY = attacker->attack.prevOrigin.vy;
        prevOriginZ = attacker->attack.prevOrigin.vz;

        originX = attacker->attack.origin.vx;
        originY = attacker->attack.origin.vy;
        originZ = attacker->attack.origin.vz;

        prevDirY = Math_Cos(prevAimPitch);
        prevDirZ = Math_Sin(prevAimPitch);
        prevDirX = Q12_MULT(prevDirZ, Math_Sin(prevAimYaw));
        prevDirZ = Q12_MULT(prevDirZ, Math_Cos(prevAimYaw));

        dirY = Math_Cos(aimPitch);
        dirZ = Math_Sin(aimPitch);
        dirX = Q12_MULT(dirZ, Math_Sin(aimYaw));
        dirZ = Q12_MULT(dirZ, Math_Cos(aimYaw));

        // Split the movement since last tick into 4 steps so fast swings don't skip over anything.
        // Last step [3] is this tick.
        for (stepWeight = 4, prevOriginSumZ = 0, prevOriginSumY = 0, prevOriginSumX = 0,
             prevDirSumZ = 0, prevDirSumY = 0, prevDirSumX = 0;
             stepWeight > 0;
             prevOriginSumZ += prevOriginZ,
             prevOriginSumY += prevOriginY,
             prevOriginSumX += prevOriginX,
             prevDirSumZ    += prevDirZ,
             prevDirSumY    += prevDirY,
             prevDirSumX    += prevDirX,
             stepWeight--)
        {
            q19_12 stepYaw;
            q19_12 stepDirX;
            q19_12 stepDirY;
            q19_12 stepDirZ;

            stepDirX = ((dirX * stepWeight) + prevDirSumX) / 4;
            stepDirY = ((dirY * stepWeight) + prevDirSumY) / 4;
            stepDirZ = ((dirZ * stepWeight) + prevDirSumZ) / 4;

            stepYaw = ratan2(stepDirX, stepDirZ);

            stepAims[stepWeight - 1].vy = ratan2(Math_Vector2MagCalc(stepDirX, stepDirZ), stepDirY);
            stepAims[stepWeight - 1].vx = stepYaw;

            stepOrigins[stepWeight - 1].vx = ((originX * stepWeight) + prevOriginSumX) / 4;
            stepOrigins[stepWeight - 1].vy = ((originY * stepWeight) + prevOriginSumY) / 4;
            stepOrigins[stepWeight - 1].vz = ((originZ * stepWeight) + prevOriginSumZ) / 4;
        }

        attacker->attack.prevAimYaw   = attacker->attack.aimYaw;
        attacker->attack.prevAimPitch = attacker->attack.aimPitch;

        for (stopped = false, stepsLeft = 4; stepsLeft > 0 && !stopped; stepsLeft--)
        {
            VECTOR3* stepOrigin;
            DVECTOR* stepAngles;

            stepOrigin                 = &stepOrigins[4 - stepsLeft];
            stepAngles                 = &stepAims[4 - stepsLeft];
            attacker->attack.origin.vx = stepOrigin->vx;
            attacker->attack.origin.vy = stepOrigin->vy;
            attacker->attack.origin.vz = stepOrigin->vz;

            Combat_AttackHitLineUpdate(attacker, stepAngles->vx, stepAngles->vy);

            if (Combat_MeleeHitCheck(attacker, stepAngles->vx, attackInfo, damageScale) != 0)
            {
                stopped = stopOnHit;
                isDone |= stopped;
            }
        }

        hitCheckTime = attackTime;

        // Steps above moved the origin around, put it back and save it for next tick.
        attack = &attacker->attack;

        attack->origin.vx              = originX;
        attacker->attack.prevOrigin.vx = originX;
        attack->origin.vy              = originY;
        attacker->attack.prevOrigin.vy = originY;
        attack->origin.vz              = originZ;
        attacker->attack.prevOrigin.vz = originZ;
    }

    if (isDone)
    {
        attacker->attack.state = CharaAttackState_Finished;

        // Log the attack for the end of game ranking.
        if (attacker->model.charaId == Chara_Harry)
        {
            s32 rankingShotType;

            switch (weaponAttack)
            {
                case WEAPON_ATTACK(AttackType_Handgun,      AttackInputType_Tap):
                case WEAPON_ATTACK(AttackType_HuntingRifle, AttackInputType_Tap):
                    if (targetDistLevel != NO_VALUE)
                    {
                        if (WorldEnv_IsInDarkness())
                        {
                            rankingShotType = 3;
                        }
                        else
                        {
                            rankingShotType = targetDistLevel;
                        }
                    }
                    else
                    {
                        rankingShotType = 5;
                    }
                    break;

                case WEAPON_ATTACK(AttackType_Shotgun, AttackInputType_Tap):
                    rankingShotType = 5;
                    break;

                case WEAPON_ATTACK(AttackType_HyperBlaster, AttackInputType_Tap):
                    rankingShotType = 4;
                    break;

                case WEAPON_ATTACK(AttackType_RockDrill, AttackInputType_Tap):
                case WEAPON_ATTACK(AttackType_Chainsaw,  AttackInputType_Tap):
                case WEAPON_ATTACK(AttackType_Katana,    AttackInputType_Tap):
                case WEAPON_ATTACK(AttackType_RockDrill, AttackInputType_Hold):
                case WEAPON_ATTACK(AttackType_Chainsaw,  AttackInputType_Hold):
                case WEAPON_ATTACK(AttackType_Katana,    AttackInputType_Hold):
                case WEAPON_ATTACK(AttackType_RockDrill, AttackInputType_Multitap):
                case WEAPON_ATTACK(AttackType_Chainsaw,  AttackInputType_Multitap):
                case WEAPON_ATTACK(AttackType_Katana,    AttackInputType_Multitap):
                    rankingShotType = 7;
                    break;

                case WEAPON_ATTACK(AttackType_Kick,  AttackInputType_Tap):
                case WEAPON_ATTACK(AttackType_Stomp, AttackInputType_Tap):
                    rankingShotType = NO_VALUE;
                    break;

                default:
                    rankingShotType = 6;
                    break;
            }

            if (rankingShotType != NO_VALUE)
            {
                func_8009151C(rankingShotType, attacker->attack.hitFlags != 0, func_8007FD2C());
            }
        }
    }
    else
    {
        attacker->attack.state        = state;
        attacker->attack.hitCheckTime = hitCheckTime;
    }

    return attacker->attack.hitFlags;
}

void Combat_AttackHitLineInit(s_SubCharacter* attacker) // 0x8008B15C
{
    s32 aimPitch;
    s32 aimYaw;
    s32 i;

    aimYaw   = attacker->attack.aimYaw;
    aimPitch = attacker->attack.aimPitch;

    attacker->attack.prevAimYaw   = aimYaw;
    attacker->attack.prevAimPitch = aimPitch;

    // @hack Loop is required for match, might be based on `attack->hitLineVec` ARRAY_SIZE but not totally sure.
    // Code in this loop only runs once.
    for (i = ARRAY_SIZE(attacker->attack.hitLineVec) - 1; i > 0; i--)
    {
        Combat_AttackHitLineUpdate(attacker, aimYaw, aimPitch);
        attacker->attack.prevOrigin = attacker->attack.origin;
    }
}

void Combat_AttackHitLineUpdate(s_SubCharacter* attacker, q19_12 aimYaw, q19_12 aimPitch) // 0x8008B1DC
{
    s32            rangeMin;
    q19_12         sinPitch;
    q19_12         dirY;
    s32            dirX;
    s32            dirZ;
    s32            i;
    s32            hitLineLength;
    s32            weaponAttack;
    s32            hitLineVecX;
    s32            hitLineVecY;
    s32            hitLineVecZ;
    s32            hitLineStartX;
    s32            hitLineStartY;
    s32            hitLineStartZ;
    s32            originX;
    s32            originY;
    s32            originZ;
    s_AttackInfo*  attackInfo;
    s_CharaAttack* attack;

    attack       = &attacker->attack;
    weaponAttack = attacker->attack.weaponAttack;

    // Shift hit line history.
    // @hack Loop is required for match, might be based on `attack->hitLineVec` ARRAY_SIZE but not totally sure.
    // Code in this loop only runs once.
    for (i = ARRAY_SIZE(attack->hitLineVec) - 2; i >= 0; i--)
    {
        hitLineVecX = attack->hitLineVec[i].vx;
        hitLineVecY = attack->hitLineVec[i].vy;
        hitLineVecZ = attack->hitLineVec[i].vz;

        attack->hitLineVec[i + 1].vx = hitLineVecX;
        attack->hitLineVec[i + 1].vy = hitLineVecY;
        attack->hitLineVec[i + 1].vz = hitLineVecZ;

        hitLineStartX = attack->hitLineStart[i].vx;
        hitLineStartY = attack->hitLineStart[i].vy;
        hitLineStartZ = attack->hitLineStart[i].vz;

        attack->hitLineStart[i + 1].vx = hitLineStartX;
        attack->hitLineStart[i + 1].vy = hitLineStartY;
        attack->hitLineStart[i + 1].vz = hitLineStartZ;
    }

    attackInfo = &g_Items_WeaponAttackInfos[weaponAttack];

    hitLineLength = attackInfo->rangeMax;
    rangeMin      = attackInfo->rangeMin;

    originX = attacker->attack.origin.vx;
    originY = attacker->attack.origin.vy;
    originZ = attacker->attack.origin.vz;

    hitLineStartX = originX;
    hitLineStartY = originY;
    hitLineStartZ = originZ;

    dirY     = Math_Cos(aimPitch);
    sinPitch = Math_Sin(aimPitch);
    dirX     = Q12_MULT(sinPitch, Math_Sin(aimYaw));
    dirZ     = Q12_MULT(sinPitch, Math_Cos(aimYaw));

    hitLineLength -= rangeMin;

    hitLineStartX                      += Q12_MULT(rangeMin, dirX);
    attacker->attack.hitLineStart[0].vx = hitLineStartX;

    hitLineStartY                      += Q12_MULT(rangeMin, dirY);
    attacker->attack.hitLineStart[0].vy = hitLineStartY;

    hitLineStartZ                      += Q12_MULT(rangeMin, dirZ);
    attacker->attack.hitLineStart[0].vz = hitLineStartZ;

    hitLineVecX                       = Q12_MULT(hitLineLength, dirX);
    attacker->attack.hitLineVec[0].vx = hitLineVecX;

    hitLineVecY                       = Q12_MULT(hitLineLength, dirY);
    attacker->attack.hitLineVec[0].vy = hitLineVecY;

    hitLineVecZ                       = Q12_MULT(hitLineLength, dirZ);
    attacker->attack.hitLineVec[0].vz = hitLineVecZ;
}

void Combat_GasWeaponSfxAllStop(void) // 0x8008B398
{
    Combat_GasWeaponContactSfxUpdate(Q8(0.0f));
    Combat_GasWeaponEngineSfxUpdate(0, 0, 0);
    Combat_GasWeaponEngineSfxUpdate(1, 0, 0);
    Combat_RockDrillRunSfxUpdate(0, 0);
}

void Combat_GasWeaponContactSfxUpdate(q23_8 vol) // 0x8008B3E4
{
    Combat_GasWeaponSfxUpdate(GasWeaponSfx_Contact, vol, 0);
}

void Combat_RockDrillRunSfxUpdate(q23_8 vol, s32 pitch) // 0x8008B40C
{
    Combat_GasWeaponSfxUpdate(GasWeaponSfx_RockDrillRun, vol, pitch);
}

void Combat_GasWeaponEngineSfxUpdate(bool isChainsaw, q23_8 vol, s32 pitch) // 0x8008B438
{
    bool chainsaw;

    chainsaw = isChainsaw != 0;

    // `isChainsaw`: `GasWeaponSfx_ChainsawRun`, `!isChainsaw`: `GasWeaponSfx_RockDrillStart`.
    if (Combat_GasWeaponSfxUpdate(GasWeaponSfx_RockDrillStart - chainsaw, vol, pitch))
    {
        func_80089314(chainsaw);
    }
}

bool Combat_GasWeaponSfxUpdate(e_GasWeaponSfx sfxIdx, q23_8 vol, s32 pitch) // 0x8008B474
{
    q19_12 level;
    s32    wasStopped;
    u16    sfxId;

    level = 0;
    sfxId = 0;
    switch (sfxIdx)
    {
        case GasWeaponSfx_Contact:
            sfxId = Sfx_GasWeaponContact;
            level = g_SysWork.gasWeaponContactSfxLevel;
            pitch = 0;
            break;

        case GasWeaponSfx_ChainsawRun:
            sfxId = Sfx_ChainsawRun;
            level = g_SysWork.gasWeaponEngineSfxLevel;
            break;

        case GasWeaponSfx_RockDrillStart:
            sfxId = Sfx_RockDrillStart;
            level = g_SysWork.gasWeaponEngineSfxLevel;
            break;

        case GasWeaponSfx_RockDrillRun:
            sfxId = Sfx_RockDrillRun;
            level = g_SysWork.rockDrillRunSfxLevel;
            break;
    }

    wasStopped = level == Q12(0.0f);

    if (g_DeltaTime == Q12(0.0f))
    {
        vol = Q8(0.0f);
    }

    if (vol >= Q8(0.0f))
    {
        if (vol != Q8(0.0f))
        {
            if (vol > Q8_CLAMPED(1.0f))
            {
                level = Q12(511.0f);
            }
            else
            {
                level = Q12(vol) + Q12(256.0f);
            }
        }
        else
        {
            level = 0;
        }
    }
    else
    {
        level -= g_DeltaTime * Q8(2.0f); // TODO: Mixed Q formats?
        if (level < 0)
        {
            level = 0;
        }
    }

    if (level == 0)
    {
        Sd_SfxStop(sfxId);
    }
    else if (wasStopped)
    {
        // `pitch` is passed as `sfxFlags` here, but sounds are only ever started with a pitch of 0.
        if (level > Q12(256.0f))
        {
            Sfx_WithFlagsPlay(sfxId, &PLAYER_CHARA().position, FP_FROM(level - Q12(256.0f), Q12_SHIFT), pitch);
        }
        else
        {
            Sfx_WithFlagsPlay(sfxId, &PLAYER_CHARA().position, Q8_CLAMPED(0.0f), pitch);
        }
    }
    else
    {
        if (level > Q12(256.0f))
        {
            Sfx_WithFlagsAndPitchPlay(sfxId, &PLAYER_CHARA().position, FP_FROM(level - Q12(256.0f), Q12_SHIFT), SfxFlag_UpdateAttribs, pitch);
        }
        else
        {
            Sfx_WithFlagsAndPitchPlay(sfxId, &PLAYER_CHARA().position, 0, SfxFlag_UpdateAttribs, pitch);
        }
    }

    switch (sfxIdx)
    {
        case GasWeaponSfx_Contact:
            g_SysWork.gasWeaponContactSfxLevel = level;
            break;

        case GasWeaponSfx_ChainsawRun:
        case GasWeaponSfx_RockDrillStart:
            g_SysWork.gasWeaponEngineSfxLevel = level;
            break;

        case GasWeaponSfx_RockDrillRun:
            g_SysWork.rockDrillRunSfxLevel = level;
    }

    return level > Q12(256.0f);
}

void func_8008B664(VECTOR3* pos, u32 weaponAttack) // 0x8008B664
{
    s32 sfxId;

    sfxId = 0;
    switch (weaponAttack)
    {
        case WEAPON_ATTACK(AttackType_KitchenKnife, AttackInputType_Tap):
        case WEAPON_ATTACK(AttackType_Katana,       AttackInputType_Tap):
        case WEAPON_ATTACK(AttackType_KitchenKnife, AttackInputType_Hold):
        case WEAPON_ATTACK(AttackType_Katana,       AttackInputType_Hold):
        case WEAPON_ATTACK(AttackType_KitchenKnife, AttackInputType_Multitap):
        case WEAPON_ATTACK(AttackType_Katana,       AttackInputType_Multitap):
            sfxId = Sfx_Unk1297;
            break;

        case WEAPON_ATTACK(AttackType_SteelPipe, AttackInputType_Tap):
        case WEAPON_ATTACK(AttackType_Hammer,    AttackInputType_Tap):
        case WEAPON_ATTACK(AttackType_Axe,       AttackInputType_Tap):
        case WEAPON_ATTACK(AttackType_SteelPipe, AttackInputType_Hold):
        case WEAPON_ATTACK(AttackType_Hammer,    AttackInputType_Hold):
        case WEAPON_ATTACK(AttackType_Axe,       AttackInputType_Hold):
        case WEAPON_ATTACK(AttackType_SteelPipe, AttackInputType_Multitap):
        case WEAPON_ATTACK(AttackType_Hammer,    AttackInputType_Multitap):
        case WEAPON_ATTACK(AttackType_Axe,       AttackInputType_Multitap):
            sfxId = Sfx_Unk1296;
            break;

        case WEAPON_ATTACK(AttackType_Handgun,      AttackInputType_Tap):
        case WEAPON_ATTACK(AttackType_HuntingRifle, AttackInputType_Tap):
        case WEAPON_ATTACK(AttackType_Shotgun,      AttackInputType_Tap):
            sfxId = Sfx_Unk1286;
            break;

        case WEAPON_ATTACK(AttackType_RockDrill, AttackInputType_Tap):
        case WEAPON_ATTACK(AttackType_Chainsaw,  AttackInputType_Tap):
        case WEAPON_ATTACK(AttackType_Chainsaw,  AttackInputType_Hold):
        case WEAPON_ATTACK(AttackType_Chainsaw,  AttackInputType_Multitap):
            if (g_SysWork.gasWeaponEngineSfxLevel > Q12(256.0f))
            {
                Combat_GasWeaponContactSfxUpdate(Q8(0.5f));
                break;
            }

        case WEAPON_ATTACK(AttackType_Kick,  AttackInputType_Tap):
        case WEAPON_ATTACK(AttackType_Stomp, AttackInputType_Tap):
            sfxId = Sfx_Unk1316;
            break;

        case WEAPON_ATTACK(AttackType_HyperBlaster, AttackInputType_Tap): // No impact sound.
    }

    if (sfxId != 0)
    {
        Sfx_WithFlagsPlay(sfxId, pos, Q8(0.75f), SfxFlag_None);
    }
}

s32 Combat_HitApply(s_SubCharacter* attacker, s_SubCharacter* target, VECTOR3* hitPos, q19_12 damageScale) // 0x8008B714
{
    s32           weaponAttack;
    s_AttackInfo* attackInfo;
    s32           attackType;
    s32           targetBit;
    s32           hitFlags;
    u32           damage;
    q19_12        pushX;
    q19_12        pushY;
    q19_12        pushZ;
    q20_12        finalDamage;
    u32           shock;

    weaponAttack = attacker->attack.weaponAttack;
    attackInfo   = &g_Items_WeaponAttackInfos[weaponAttack];
    attackType   = attackInfo->type;
    hitFlags     = attacker->attack.hitFlags;

    if (target == &PLAYER_CHARA())
    {
        targetBit               = NO_VALUE;
        target->attackingNpcIdx = attacker - g_SysWork.npcs; // Obtuse way of setting NPC index.
    }
    else
    {
        targetBit = 1 << (target - g_SysWork.npcs);

        if (hitFlags & targetBit)
        {
            switch (weaponAttack)
            {
                case WEAPON_ATTACK(AttackType_Chainsaw,  AttackInputType_Tap):
                case WEAPON_ATTACK(AttackType_Chainsaw,  AttackInputType_Hold):
                case WEAPON_ATTACK(AttackType_Chainsaw,  AttackInputType_Multitap):
                case WEAPON_ATTACK(AttackType_RockDrill, AttackInputType_Tap):
                    if (g_SysWork.gasWeaponEngineSfxLevel > Q12(256.0f))
                    {
                        Combat_GasWeaponContactSfxUpdate(Q8(0.5f));
                    }
                    break;
            }
        }
        else
        {
            switch (attackType)
            {
                case AttackInfoType_Strike:
                    func_800892A4(6);
                    break;

                case AttackInfoType_Slash:
                    func_800892A4(7);
                    break;

                case AttackInfoType_Gunshot:
                    break;
            }

            if (target->model.charaId == Chara_Padlock)
            {
                Sfx_WithFlagsPlay(Sfx_Unk1392, hitPos, Q8(0.5f), SfxFlag_None);
            }
            else
            {
                func_8008B664(hitPos, weaponAttack);
            }
        }
    }

    damage = Q12(attackInfo->damage);

    switch (weaponAttack)
    {
        case WEAPON_ATTACK(AttackType_RockDrill, AttackInputType_Tap):
        case WEAPON_ATTACK(AttackType_Chainsaw,  AttackInputType_Tap):
        case WEAPON_ATTACK(AttackType_RockDrill, AttackInputType_Hold):
        case WEAPON_ATTACK(AttackType_Chainsaw,  AttackInputType_Hold):
        case WEAPON_ATTACK(AttackType_RockDrill, AttackInputType_Multitap):
        case WEAPON_ATTACK(AttackType_Chainsaw,  AttackInputType_Multitap):
            // @bug `hitCheckTime` is Q12 but `Combat_AttackHitWindowCheck` compares whole keyframes?
            if (!Combat_AttackHitWindowCheck(attackInfo, attacker->attack.hitCheckTime))
            {
                damage = 0;
            }

            if (g_SysWork.gasWeaponEngineSfxLevel > Q12(256.0f))
            {
                damage += Q12(200.0f);
            }
            break;

        case WEAPON_ATTACK(AttackType_HyperBlaster, AttackInputType_Tap):
            switch (Game_HyperBlasterBeamColorGet())
            {
                case 0:
                    break;

                case 1:
                    damage *= 3;
                    break;

                case 2:
                    damage += Rng_RandQ12() * 50;
                    damage *= 5;
                    break;
            }
            break;

        case WEAPON_ATTACK(AttackType_Kick,  AttackInputType_Tap):
        case WEAPON_ATTACK(AttackType_Stomp, AttackInputType_Tap):
            if (target->flags & CharaFlag_Unk2)
            {
                damage = Q12(1000.0f);
            }
            break;
    }

    // Random spread of 87.5% to 112.5%.
    shock       = attackInfo->shock;
    finalDamage = Q12_MULT_PRECISE(damage, (Q12(4.5f) - Rng_RandQ12()) / 4);

    switch (weaponAttack)
    {
        case WEAPON_ATTACK(AttackType_Handgun,      AttackInputType_Tap):
        case WEAPON_ATTACK(AttackType_HuntingRifle, AttackInputType_Tap):
        case WEAPON_ATTACK(AttackType_Shotgun,      AttackInputType_Tap):
        case WEAPON_ATTACK(AttackType_HyperBlaster, AttackInputType_Tap):
        case NO_VALUE: // @bug This `NO_VALUE` check could never trigger here, `weaponAttack` is zero-extended to `s32` from a `u8`, 0xFF would read as 255 and not `NO_VALUE`.
                       // Possible this was another Gunshot type attack at one point, which they removed from here by changing to NO_VALUE?
            // Push along the pellet direction. Shotgun pellets don't push.
            pushX = attacker->attack.hitLineVec[1].vx;
            pushY = attacker->attack.hitLineVec[1].vy;
            pushZ = attacker->attack.hitLineVec[1].vz;

            if (weaponAttack == WEAPON_ATTACK(AttackType_Shotgun, AttackInputType_Tap))
            {
                if (hitFlags & targetBit)
                {
                    shock = 0;
                }

                pushZ = 0;
                pushY = 0;
                pushX = 0;
            }
            break;

        default:
        {
            s32 pushLength;

            if (target != &PLAYER_CHARA() && !(target->flags & CharaFlag_Hit))
            {
                finalDamage *= 4;
            }

            if (attackType == AttackInfoType_Slash)
            {
                finalDamage = Q12_MULT_PRECISE(finalDamage, damageScale);
                shock       = Q12_MULT_PRECISE(shock, damageScale);
            }

            if (weaponAttack == WEAPON_ATTACK(AttackType_Kick,  AttackInputType_Tap) ||
                weaponAttack == WEAPON_ATTACK(AttackType_Stomp, AttackInputType_Tap))
            {
                pushX = attacker->attack.hitLineVec[0].vx;
                pushY = 0;
                pushZ = attacker->attack.hitLineVec[0].vz;
            }
            else
            {
                // Push along the swing, from last tick's hit line end to this step's.
                // @bug `prevTip` is never written, so last tick's end is just `prevOrigin`.
                s32 baseX;
                s32 baseY;
                s32 baseZ;
                s32 prevTipX;
                s32 prevTipY;
                s32 prevTipZ;
                s32 tipX;
                s32 tipY;
                s32 tipZ;

                baseX = attacker->attack.prevOrigin.vx;
                baseY = attacker->attack.prevOrigin.vy;
                baseZ = attacker->attack.prevOrigin.vz;

                prevTipX = attacker->attack.prevTip.vx;
                prevTipY = attacker->attack.prevTip.vy;
                prevTipZ = attacker->attack.prevTip.vz;

                tipX = attacker->attack.hitLineVec[0].vx;
                tipY = attacker->attack.hitLineVec[0].vy;
                tipZ = attacker->attack.hitLineVec[0].vz;

                prevTipX += baseX;
                prevTipY += baseY;
                prevTipZ += baseZ;

                baseX = attacker->attack.hitLineStart[0].vx;
                baseY = attacker->attack.hitLineStart[0].vy;
                baseZ = attacker->attack.hitLineStart[0].vz;

                tipX += baseX;
                tipY += baseY;
                tipZ += baseZ;

                pushX = tipX - prevTipX;
                pushY = tipY - prevTipY;
                pushZ = tipZ - prevTipZ;
            }

            // Normalize push to Q12 unit length.
            pushLength = Math_SquareRootQ12(func_80080540(pushX, pushY, pushZ));
            if (pushLength >= 64)
            {
                if (pushLength >= Q12(4096.0f))
                {
                    pushLength /= Q12(1.0f);
                    pushX      /= Q12(1.0f);
                    pushY      /= Q12(1.0f);
                    pushZ      /= Q12(1.0f);
                }

                pushLength = Q12(4096.0f) / pushLength;
                pushX      = Q12_MULT_PRECISE(pushX, pushLength);
                pushY      = Q12_MULT_PRECISE(pushY, pushLength);
                pushZ      = Q12_MULT_PRECISE(pushZ, pushLength);
            }
            break;
        }
    }

    if (finalDamage != Q12(0.0f))
    {
        target->damage.amount += finalDamage;
    }

    if (shock != 0)
    {
        s32 pushDelta;

        pushDelta                   = Q12_MULT_PRECISE(shock, pushX);
        target->damage.position.vx += pushDelta;
        pushDelta                   = Q12_MULT_PRECISE(shock, pushY);
        target->damage.position.vy += pushDelta;
        pushDelta                   = Q12_MULT_PRECISE(shock, pushZ);
        target->damage.position.vz += pushDelta;
    }

    target->attackReceived    = weaponAttack;
    hitFlags                 |= targetBit;
    attacker->attack.hitFlags = hitFlags;

    if (finalDamage | shock)
    {
        s32 bloodSize = attackInfo->bloodSize;

        if (bloodSize > 0 && bloodSize < 8)
        {
            s32 bloodType = NO_VALUE; // TODO: Enum for blood types.

            switch (target->model.charaId)
            {
                case Chara_Harry:
                    if (target->health >= Q12(0.0f))
                    {
                        bloodType = 1;
                    }
                    break;

                case Chara_AirScreamer:
                case Chara_NightFlutter:
                    bloodType = 2;
                    break;

                case Chara_Stalker:
                    bloodType = 4;
                    break;

                case Chara_LarvalStalker:
                case Chara_GreyChild:
                case Chara_Mumbler:
                    bloodType = 3;
                    break;

                case Chara_Creeper:
                    bloodType = 5;
                    break;

                case Chara_Groaner:
                case Chara_Wormhead:
                    bloodType = 6;
                    break;

                case Chara_SplitHead:
                    bloodType = 7;
                    break;

                case Chara_HangedScratcher:
                    bloodType = 8;
                    break;

                case Chara_Floatstinger:
                case Chara_Twinfeeler:
                    bloodType = 10;
                    break;

                case Chara_Romper:
                    bloodType = 9;
                    break;

                case Chara_PuppetNurse:
                case Chara_PuppetDoctor:
                    bloodType = 11;
                    break;

                case Chara_Bloodsucker:
                    bloodType = 12;
                    bloodSize = 1;
                    break;

                case Chara_Incubus:
                case Chara_Unknown23:
                case Chara_Incubator:
                    if (!(target->flags & CharaFlag_Hit))
                    {
                        if (weaponAttack == WEAPON_ATTACK(AttackType_Shotgun, AttackInputType_Tap))
                        {
                            bloodSize = 4;
                        }
                        else
                        {
                            bloodSize = 7;
                        }
                    }
                    bloodType = 16;
                    break;

                default:
                    bloodType = 16;
                    break;

                case Chara_Padlock:
                    bloodType = NO_VALUE;
                    break;
            }

            if (bloodType >= 0)
            {
                func_8005F6B0(target, hitPos, bloodSize, bloodType);
            }
        }
    }

    return targetBit;
}

s32 Combat_MeleeHitCheck(s_SubCharacter* attacker, q19_12 yaw, s_AttackInfo* attackInfo, q19_12 damageScale) // 0x8008BF84
{
    s_SubCharacter* other;
    s_SubCharacter* firstChara;
    s32             charaCount;
    s32             hitFlags;
    s32             charaBit;
    s32             firstCharaBit;
    s32             i;
    q19_12          sinYaw;
    q19_12          cosYaw;
    q19_12          hitLineStartX;
    q19_12          hitLineStartY;
    q19_12          hitLineStartZ;
    q19_12          hitLineVecX;
    q19_12          hitLineVecY;
    q19_12          hitLineVecZ;
    q19_12          localHitLineX;
    q19_12          localHitLineY;
    q19_12          localHitLineZ;
    q19_12          boxMaxX;
    q19_12          boxMaxY;
    q19_12          boxMaxZ;
    q19_12          boxMinX;
    q19_12          boxMinY;
    q19_12          boxMinZ;
    q19_12          boxSizeX;
    q19_12          boxSizeY;
    q19_12          boxSizeZ;
    q19_12          hitSizeY;
    q19_12          hitSizeZ;
    q19_12          rayX;
    q19_12          rayFromY;
    q19_12          rayZ;

    static VECTOR3    localHitLinePoints[4]; // Q19.12 | Hit line in attacker-local space. Written but never read.
    static VECTOR3    hitPos;
    static s32        __pad_bss_800C47C4;
    static VECTOR3    rayPoints[2];          // Line-of-sight ray from the attacker to each candidate.
    static s32        __pad_bss_800C47E0[2];
    static VECTOR3    otherCenter;
    static s32        __pad_bss_800C47F4;
    static s_RayTrace rayTrace;

    hitLineStartX = attacker->attack.hitLineStart[0].vx;
    hitLineStartY = attacker->attack.hitLineStart[0].vy;
    hitLineStartZ = attacker->attack.hitLineStart[0].vz;

    sinYaw = Math_Sin(yaw);
    cosYaw = Math_Cos(yaw);

    localHitLinePoints[0].vx = Q12(0.0f);
    localHitLinePoints[0].vy = Q12(0.0f);
    localHitLinePoints[0].vz = Q12(0.0f);

    hitLineVecX = attacker->attack.hitLineVec[0].vx;
    hitLineVecZ = attacker->attack.hitLineVec[0].vz;

    boxMinX = 0;
    boxMaxX = 0;
    boxMinY = 0;
    boxMaxY = 0;
    boxMinZ = 0;
    boxMaxZ = 0;

    hitLineVecY              = attacker->attack.hitLineVec[0].vy;
    localHitLinePoints[1].vy = hitLineVecY;

    localHitLineX            = Q12_MULT_PRECISE(hitLineVecX, cosYaw) - Q12_MULT_PRECISE(hitLineVecZ, sinYaw);
    localHitLinePoints[1].vx = localHitLineX;

    localHitLineZ            = Q12_MULT_PRECISE(hitLineVecX, sinYaw) + Q12_MULT_PRECISE(hitLineVecZ, cosYaw);
    localHitLinePoints[1].vz = localHitLineZ;

    localHitLineY = hitLineVecY;

    if (localHitLineX > Q12(0.0f))
    {
        boxMaxX = localHitLineX;
    }
    else if (localHitLineX < Q12(0.0f))
    {
        boxMinX = localHitLineX;
    }

    if (boxMaxY < localHitLineY)
    {
        boxMaxY = localHitLineY;
    }
    else if (localHitLineY < boxMinY)
    {
        boxMinY = localHitLineY;
    }

    if (boxMaxZ < localHitLineZ)
    {
        boxMaxZ = localHitLineZ;
    }
    else if (localHitLineZ < boxMinZ)
    {
        boxMinZ = localHitLineZ;
    }

    hitSizeY = attackInfo->hitCheck.fight.sizeY;
    hitSizeZ = attackInfo->hitCheck.fight.sizeZ;

    boxSizeX = (boxMaxX - boxMinX);
    boxSizeY = (boxMaxY - boxMinY);
    boxSizeZ = (boxMaxZ - boxMinZ);

    if (boxSizeX < attackInfo->hitCheck.fight.sizeX)
    {
        boxMaxX += DIV_FAST(attackInfo->hitCheck.fight.sizeX, 2);
        boxMinX -= DIV_FAST(attackInfo->hitCheck.fight.sizeX, 2);
    }

    if (boxSizeY < hitSizeY)
    {
        boxMaxY += DIV_FAST(attackInfo->hitCheck.fight.sizeY, 2);
        boxMinY -= DIV_FAST(attackInfo->hitCheck.fight.sizeY, 2);
    }

    if (boxSizeZ < hitSizeZ)
    {
        boxMaxZ += DIV_FAST(attackInfo->hitCheck.fight.sizeZ, 2);
        boxMinZ -= DIV_FAST(attackInfo->hitCheck.fight.sizeZ, 2);
    }

    if (attacker == &PLAYER_CHARA())
    {
        firstChara    = &NPC_CHARA(0);
        charaCount    = NPC_COUNT_MAX;
        firstCharaBit = 1;
    }
    else
    {
        firstChara    = &PLAYER_CHARA();
        charaCount    = 1;
        firstCharaBit = -1;
    }

    charaBit = firstCharaBit;
    other    = firstChara;
    hitFlags = 0;

    rayX = attacker->position.vx;
    rayZ = attacker->position.vz;

    rayFromY  = attacker->position.vy;
    rayFromY += attacker->collision.box.topExtent;

    rayPoints[0].vx = rayX;
    rayPoints[0].vy = rayFromY;
    rayPoints[0].vz = rayZ;

    for (i = charaCount; i > 0; i--, other++, charaBit <<= 1)
    {
        q19_12 targetX;
        q19_12 rayToY;
        q19_12 targetZ;
        q19_12 offsetX;
        q19_12 innerRadius;
        q19_12 angleToOther;
        q19_12 cosAngleToOther;
        bool   isBlocked;

        if (g_SysWork.sysState == SysState_GameOver ||
            PLAYER_CHARA().health <= Q12(0.0f) ||
            other == attacker ||
            other->model.charaId == Chara_None ||
            other->health < Q12(0.0f) ||
            other->collision.state == CharaCollisionState_Ignore)
        {
            continue;
        }

        otherCenter.vx = other->position.vx + other->collision.shapeOffsets.box.vx;
        otherCenter.vy = other->position.vy;
        otherCenter.vz = other->position.vz + other->collision.shapeOffsets.box.vz;
        if (Math_Distance2dGet(&attacker->position, &otherCenter) > Q12(3.0f))
        {
            continue;
        }

        targetX = other->position.vx;
        rayToY  = other->position.vy;
        targetZ = other->position.vz;

        // @hack Box offsets loaded through `offsetX` and `innerRadius` required for match.
        offsetX     = other->collision.shapeOffsets.box.vx;
        innerRadius = other->collision.shapeOffsets.box.vz;

        targetX += offsetX;
        rayToY  += other->collision.box.topExtent;
        targetZ += innerRadius;

        rayPoints[1].vy = rayToY;
        innerRadius      = other->collision.cylinder.innerRadius;

        // @bug `rayX` and `rayZ` are meant to stay at the ray start, but are overwritten with each candidate's ray end
        // below. From the second candidate on, this angle is measured from the previous candidate's ray end instead.
        angleToOther    = ratan2(targetX - rayX, targetZ - rayZ);
        offsetX         = Q12_MULT_PRECISE(innerRadius, Math_Sin(angleToOther));
        cosAngleToOther = Math_Cos(angleToOther);

        rayX = targetX - offsetX;
        rayZ = targetZ - Q12_MULT_PRECISE(innerRadius, cosAngleToOther);

        rayPoints[1].vx = rayX;
        rayPoints[1].vz = rayZ;

        isBlocked = Ray_GeomTraceGet(&rayTrace, &rayPoints[0], &rayPoints[1]);
        if (isBlocked != false)
        {
            isBlocked = true; // @hack Redundant normalize required for match.
        }

        if (isBlocked != true)
        {
            q19_12 otherCenterX;
            q19_12 otherPosY;
            q19_12 otherCenterZ;
            q19_12 otherHeight;
            q19_12 otherTopExtent;
            q19_12 otherMidY;
            q19_12 relX;
            q19_12 relY;
            q19_12 relZ;
            q19_12 localX;
            q19_12 localZ;
            q19_12 outsideX;
            q19_12 outsideZ;
            q19_12 hitOffsetZ;
            q19_12 hitOffsetY;
            q19_12 hitOffsetX;
            q19_12 hitOffsetXZ;
            q19_12 halfHeight;
            s32    j;

            otherCenterX  = other->position.vx;
            otherCenterX += other->collision.shapeOffsets.box.vx;
            otherCenterZ  = other->position.vz;
            otherCenterZ += other->collision.shapeOffsets.box.vz;

            relX = otherCenterX - hitLineStartX;
            relZ = otherCenterZ - hitLineStartZ;

            otherPosY      = other->position.vy;
            otherHeight    = other->collision.box.height;
            otherTopExtent = other->collision.box.topExtent;

            relY   = otherPosY - hitLineStartY;
            localX = Q12_MULT_PRECISE(relX, cosYaw) - Q12_MULT_PRECISE(relZ, sinYaw);
            localZ = Q12_MULT_PRECISE(relX, sinYaw) + Q12_MULT_PRECISE(relZ, cosYaw);

            if ((relY < (boxMinY - otherHeight)) || ((boxMaxY - otherTopExtent) < relY))
            {
                continue;
            }

            innerRadius = other->collision.cylinder.innerRadius;
            outsideX    = Q12(0.0f);

            if (localX < boxMinX)
            {
                outsideX = boxMinX - localX;
            }
            else if (boxMaxX < localX)
            {
                outsideX = localX - boxMaxX;
            }

            outsideZ = Q12(0.0f);

            if (localZ < boxMinZ)
            {
                outsideZ = boxMinZ - localZ;
            }
            else if (boxMaxZ < localZ)
            {
                outsideZ = localZ - boxMaxZ;
            }

            if (outsideX | outsideZ)
            {
                if (outsideX == Q12(0.0f))
                {
                    if (innerRadius < outsideZ)
                    {
                        continue;
                    }
                }
                else if (outsideZ == Q12(0.0f))
                {
                    if (innerRadius < outsideX)
                    {
                        continue;
                    }
                }
                else
                {
                    q19_12 outsideDistSqr;

                    outsideDistSqr = Q12_SQUARE_PRECISE(outsideX) + Q12_SQUARE_PRECISE(outsideZ);
                    if (Q12_SQUARE_PRECISE(innerRadius) < outsideDistSqr)
                    {
                        continue;
                    }
                }
            }

            // @bug Overwrites the current `hitLineVec` with the point 3/4 along the hit line.
            // Each later candidate that reaches here converts the already converted point again.
            hitLineVecX -= hitLineVecX / 4;
            hitLineVecY -= hitLineVecY / 4;
            hitLineVecZ -= hitLineVecZ / 4;

            hitLineVecX += hitLineStartX;
            hitLineVecY += hitLineStartY;
            hitLineVecZ += hitLineStartZ;

            otherMidY = (otherTopExtent + otherHeight) / 2;

            hitOffsetX = hitLineVecX - otherCenterX;
            hitOffsetZ = hitLineVecZ - otherCenterZ;
            hitOffsetY = hitLineVecY - (otherPosY + otherMidY);

            hitOffsetXZ = Math_SquareRootQ12(func_80080540(hitOffsetX, Q12(0.0f), hitOffsetZ));
            halfHeight  = CLAMP_LOW((otherHeight - otherTopExtent) / 2, 0);

            for (j = 2;
                 (halfHeight < hitOffsetY || hitOffsetY < -halfHeight || innerRadius < hitOffsetXZ) && j > 0;
                 j--)
            {
                s32 clampScale;

                if (innerRadius < hitOffsetXZ)
                {
                    clampScale  = Q12_DIV(innerRadius, hitOffsetXZ);
                    hitOffsetXZ = innerRadius;
                    hitOffsetX  = Q12_MULT_PRECISE(clampScale, hitOffsetX);
                    hitOffsetY  = Q12_MULT_PRECISE(clampScale, hitOffsetY);
                    hitOffsetZ  = Q12_MULT_PRECISE(clampScale, hitOffsetZ);
                }
                else if (halfHeight < hitOffsetY)
                {
                    clampScale  = Q12_DIV(halfHeight, hitOffsetY);
                    hitOffsetY  = halfHeight;
                    hitOffsetXZ = Q12_MULT_PRECISE(clampScale, hitOffsetXZ);
                    hitOffsetX  = Q12_MULT_PRECISE(clampScale, hitOffsetX);
                    hitOffsetZ  = Q12_MULT_PRECISE(clampScale, hitOffsetZ);
                }
                else if (hitOffsetY < -halfHeight)
                {
                    clampScale  = Q12_DIV(-halfHeight, hitOffsetY);
                    hitOffsetY  = -halfHeight;
                    hitOffsetXZ = Q12_MULT_PRECISE(clampScale, hitOffsetXZ);
                    hitOffsetX  = Q12_MULT_PRECISE(clampScale, hitOffsetX);
                    hitOffsetZ  = Q12_MULT_PRECISE(clampScale, hitOffsetZ);
                }
            }

            hitPos.vx = otherCenterX + hitOffsetX;
            hitPos.vy = (otherPosY + otherMidY) + hitOffsetY;
            hitPos.vz = otherCenterZ + hitOffsetZ;

            if (Combat_HitApply(attacker, other, &hitPos, damageScale))
            {
                hitFlags |= charaBit;
            }
        }
    }

    attacker->attack.hitFlags |= hitFlags;
    return hitFlags;
}
