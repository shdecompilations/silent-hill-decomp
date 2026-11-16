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

// Used by `func_8008A3E0`. TODO: Are they really angles?
q3_12 D_800AFD1C[] =
{
    Q12_ANGLE(360.0f), Q12_ANGLE(360.0f), Q12_ANGLE(180.0f), Q12_ANGLE(120.0f),
    Q12_ANGLE(90.0f),  Q12_ANGLE(72.0f),  Q12_ANGLE(60.0f),  Q12_ANGLE(51.5f),
    Q12_ANGLE(45.0f),  Q12_ANGLE(40.0f),  Q12_ANGLE(36.0f),  0x174,
    Q12_ANGLE(30.0f),  0x13B,             0x124,             Q12_ANGLE(24.0f)
};

// ========================================
// COMBAT RELATED?
// ========================================
// Will: Possibly combat? I only found that stubbing `func_8008A3E0` and `func_8008B714`
// makes enemies unable to perform any attack. Also there are some sound related functions,
// they seem to be specifically done for something related to weapons in the inventory.

u32 func_8008A058(s32 arg0) // 0x8008A058
{
    // @hack
    register s32  temp_t0 asm("t0");
    register u32  var_t1  asm("t1");
    register u32  var_t2  asm("t2");
    register s16* ptr     asm("t3");

    var_t1 = 0;

    if (arg0)
    {
        gte_ldlzc(arg0);

        var_t2  = var_t1;
        ptr = SQRT;

        if (arg0 > 0)
        {
            gte_stlzcr(var_t2);
        }

        var_t1 = arg0;

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

s32 func_8008A0E4(s32 request, s32 weaponAttack, s_SubCharacter* chara, VECTOR3* attackPos, s_SubCharacter* unused,
                  q3_12 aimYaw, q3_12 aimPitch) // 0x8008A0E4
{
    s32          prevState;
    s32          hitFlags;
    s32          ret;
    s32          firstNpcBit;
    s32          count;
    s_ModelAnim* modelAnim;

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

    if (request <= 0)
    {
        prevState           = 0;
        chara->attack.state = 0;
    }

    chara->attack.weaponAttack = weaponAttack;

    if (!(modelAnim->status & (1 << 0)))
    {
        chara->attack.state    = 0;
        chara->attack.hitFlags = 0;
    }
    else if (!prevState && request > 0)
    {
        chara->attack.state    = 1;
        chara->attack.hitFlags = 0;
    }

    chara->attack.shotsLeft = 100;
    chara->attack.aimYaw    = aimYaw;
    chara->attack.aimPitch  = aimPitch;
    chara->attack.origin    = *attackPos;
    hitFlags                = func_8008A3E0(chara);
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
}

u32 func_8008A270(s32 weaponAttack) // 0x8008A270
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

u32 func_8008A2E0(s32 weaponAttack) // 0x8008A2E0
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

s32 func_8008A35C(s_AttackInfo* attackInfo, s32 keyframe) // 0x8008A35C
{
    s32 hitEnd;
    s32 result;

    hitEnd = attackInfo->hitStartKeyframe + attackInfo->hitKeyframeCount;
    result = 0;
    if (keyframe >= attackInfo->hitStartKeyframe)
    {
        result = (keyframe < hitEnd);
    }

    return result;
}

void Chara_Flag8Clear(s_SubCharacter* chara) // 0x8008A384
{
    chara->flags &= ~CharaFlag_Unk8;
}

void Chara_Flag8Set(s_SubCharacter* chara) // 0x8008A398
{
    chara->flags |= CharaFlag_Unk8;
}

void func_8008A3AC(s_SubCharacter* chara) // 0x8008A3AC
{
    if (!(chara->flags & CharaFlag_Unk8))
    {
        chara->attack.state = 0;
        func_8008A3E0(chara);
    }
}

s32 func_8008A3E0(s_SubCharacter* chara) // 0x8008A3E0
{
    s32                sp10;
    s32                weaponAttack;
    s32                sp18;
    s_AttackInfo*      attackInfo;
    s32                sp20;
    s32                sp24;
    s32                sp28;
    s32                sp2C;
    s32                sp30;
    s32                sp34;
    u32                shotCountMax;
    s32                sp3C;
    u32                sp40;
    s32                pelletCount;
    s32                sp48;
    s32                sp4C;
    s32                sp50;
    s32                sp54;
    s32                sp58;
    s32                sp5C;
    s32                sp60;
    s32                sp64;
    s32                sp68;
    s32                sp6C;
    s32                sp70;
    s32                sp74;
    q19_12             sp78;
    s32                sp7C;
    q19_12             sp80;
    s32                sp84;
    q19_12             sp88;
    q19_12             sp8C;
    s32                sp90;
    s32                sp94;
    DVECTOR*           temp_s0_14;
    VECTOR3*           temp_a0;
    VECTOR3*           temp_v0_11;
    s_CharaAttack*     temp_v0_12;
    s32                temp_s0;
    s32                temp_s0_11;
    q19_12             angle;
    s32                temp_s0_7;
    s32                temp_s0_8;
    s32                temp_s0_9;
    q19_12             temp_s0_10;
    s32                var_s0_2;
    s32                temp_s1_3;
    s32                temp_s1_4;
    q19_12             temp_s1_5;
    s32                temp_s2_2;
    s32                var_s2;
    s32                temp_v0_10;
    s32                scaledDistSqr;
    s32                temp_v1;
    s32                var_a0_2;
    s32                var_a0_3;
    s32                temp_a1;
    s32                var_a1;
    s32                var_fp;
    s32                var_s3;
    s32                j;
    s32                var_s6;
    s32                i;
    s32                var_v1;
    s_ModelAnim*       var_s0;
    s_SubCharacter*    temp_s1_2;
    u32                temp_s1;
    s32                var_a0;
    s_AnimInfo*        anim;
    s_SubCharacter*    ptr;
    bool               hasHit;
    s32                temp_t3_2;
    s32                temp2;
    s32                temp3;
    s32                temp4;
    s32                temp5;
    static s_RayTrace  D_800C4728;
    static DVECTOR     D_800C4748[4];
    static VECTOR3     D_800C4758[4];

    sp30 = NO_VALUE;
    Chara_Flag8Set(chara);

    sp10         = chara->attack.state;
    weaponAttack = chara->attack.weaponAttack;

    if (sp10 == 0)
    {
        chara->attack.shotsFired  = 0;
        chara->attack.hitFlags  = 0;
        chara->attack.hitCheckTime = 0;
        return 0;
    }

    if (sp10 == sp30)
    {
        chara->attack.hitFlags  = 0;
        chara->attack.hitCheckTime = 0;
        return 0;
    }

    sp18 = 0;

    attackInfo = &g_Items_WeaponAttackInfos[weaponAttack];

    temp_s1 = attackInfo->type;

    if (chara == &PLAYER_CHARA())
    {
        var_s0 = &PLAYER_EXTRA().model.anim;
    }
    else
    {
        var_s0 = &chara->model.anim;
    }

    anim  = Anim_InfoGet(var_s0);
    sp28  = var_s0->time;
    sp28 -= Q12(anim->startKeyframeIdx);

    var_s0_2 = func_8008A270(weaponAttack);
    var_a0   = func_8008A2E0(weaponAttack);

    i   = chara->attack.hitCheckTime;
    sp2C     = Q12(var_s0_2 + var_a0);
    var_s0_2 = Q12(var_s0_2);

    if (sp28 < i)
    {
        sp10                   = 1;
        chara->attack.state = sp10;
    }

    if (sp10 == 1)
    {
        chara->attack.shotsFired = 0;
        sp10                   = 2;
    }

    if (i < var_s0_2)
    {
        i                  = var_s0_2;
        chara->attack.hitCheckTime = i;
    }
    else if (sp2C < i)
    {
        i                  = sp2C;
        chara->attack.hitCheckTime = i;
    }

    if (sp28 < var_s0_2)
    {
        return 0;
    }

    if (g_DeltaTime != Q12(0.0f))
    {
        if (sp28 >= sp2C)
        {
            sp28 = sp2C;
            sp18 = 1;
        }

        sp20 = chara->attack.aimYaw;
        sp24 = chara->attack.aimPitch;

        if (temp_s1 == AttackInfoType_Gunshot)
        {
            sp3C = chara->attack.shotsLeft;
            sp40 = chara->attack.shotsFired;
            sp34 = attackInfo->hitCheck.shot.interval;

            sp54 = attackInfo->rangeMax;

            if (sp34 != 0)
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

            sp50 = attackInfo->hitCheck.shot.spread;
            sp4C = sp50 >> 4;
            sp48 = attackInfo->hitCheck.shot.inaccuracy;
            sp4C--;
            sp4C &= 0xF;
            sp50  = D_800AFD1C[sp50 & 0xF];
            sp4C++;
            pelletCount = attackInfo->hitCheck.shot.pelletCount;
            sp4C = sp4C * sp50;

            if (sp34 == 0 && pelletCount == 1)
            {
                if (chara->model.charaId == Chara_Harry)
                {
                    var_a0_2 = func_8007FD2C();
                }
                else
                {
                    var_a0_2 = Q12(2.0f);
                }

                if (var_a0_2 < Q12(0.5f))
                {
                    var_a0_2 = Q12(0.5f);
                }
                else if (var_a0_2 > Q12(2.0f))
                {
                    var_a0_2 = Q12(2.0f);
                }

                temp_v1 = Q12(var_a0_2 - Q12(0.5f)) / Q12(1.5f);
                var_s2  = Q12_MULT_PRECISE(temp_v1, Q12(-1.5f)) + Q12(1.75f);
                var_s2  = Q12_MULT_PRECISE(var_s2, sp48);

                if (var_s2 < 170)
                {
                    if (WorldEnv_IsInDarkness())
                    {
                        var_s2 = (var_s2 + 170) / 2;
                    }
                }
                else
                {
                    var_s2 = 170;
                }

                temp_t3_2 = sp54 / 3;
                sp30      = NO_VALUE;

                if (g_SysWork.targetNpcIdx >= 0)
                {
                    temp_s1_2 = &NPC_CHARA(g_SysWork.targetNpcIdx);

                    if (temp_s1_2->model.charaId >= Chara_AirScreamer)
                    {
                        do
                        {
                            if (temp_s1_2->model.charaId <= CHARA_LAST_ENEMY_ID)
                            {
                                temp_s0 = Q12_SQUARE_PRECISE(chara->position.vx - temp_s1_2->position.vx) +
                                          Q12_SQUARE_PRECISE(chara->position.vy - temp_s1_2->position.vy) +
                                          Q12_SQUARE_PRECISE(chara->position.vz - temp_s1_2->position.vz);
                                sp30    = SquareRoot12(temp_s0);

                                if (temp_s1_2->flags & CharaFlag_Unk2)
                                {
                                    temp_s0 = temp_s0 - 0x800;
                                    if (temp_s0 < 0)
                                    {
                                        var_s2 = 0;
                                    }
                                    else if (temp_s0 <= 0xFFFF)
                                    {
                                        scaledDistSqr = Q12(temp_s0);
                                        var_s2        = Q12_MULT_PRECISE(var_s2, scaledDistSqr / 0x10000);
                                    }
                                }

                                if (WorldEnv_IsInDarkness())
                                {
                                    sp30 = 3;
                                }
                                else if (temp_t3_2 < sp30)
                                {
                                    sp30 = 2;
                                }
                                else if (sp30 > Q12(2.0f))
                                {
                                    sp30    = 1;
                                    var_s2 -= var_s2 / 2;
                                }
                                else
                                {
                                    sp30   = 0;
                                    var_s2 = 0;
                                }
                            }
                        } while (0); // @hack
                    }
                }

                sp48 = Q12_MULT_PRECISE(var_s2, Rng_RandQ12());
            }

            var_s6 = Q12_MULT(sp4C, sp10 - 2);

            for (i = i + sp34;
                 sp3C != 0 && sp40 < shotCountMax && sp28 >= i;
                 i += sp34, sp40++, sp3C--)
            {
                for (j = pelletCount; j > 0; j--)
                {
                    temp_v0_10 = Rng_RandQ12();
                    temp_s2_2  = Q12_MULT_PRECISE(sp48, temp_v0_10);

                    temp_v0_10 = Rng_RandQ12() - Q12_ANGLE(180.0f);
                    angle  = var_s6 + Q12_MULT_PRECISE(sp50, temp_v0_10);

                    temp3     = sp20 + Q12_MULT(temp_s2_2, Math_Sin(angle));
                    temp_s0_7 = sp24 + Q12_MULT(temp_s2_2, Math_Cos(angle));
                    temp2     = Math_Cos(temp_s0_7);
                    temp_s0_8 = Math_Sin(temp_s0_7);
                    temp_s1_4 = Q12_MULT(temp_s0_8, Math_Sin(temp3));
                    temp_s0_9 = Q12_MULT(temp_s0_8, Math_Cos(temp3));

                    chara->attack.segVec[0].vx = Q12_MULT(sp54, temp_s1_4);
                    chara->attack.segVec[0].vy = Q12_MULT(sp54, temp2);
                    chara->attack.segVec[0].vz = Q12_MULT(sp54, temp_s0_9);

                    var_s6 += sp4C;

                    if (chara->model.charaId == Chara_Harry)
                    {
                        func_800892A4(5);
                    }

                    hasHit = Ray_GeomAndCharaTraceGet(&D_800C4728, &chara->attack.origin, &chara->attack.segVec[0], chara);
                    ptr  = D_800C4728.character;
                    if (hasHit && ptr != NULL)
                    {
                        chara->attack.segVec[1].vx = temp_s1_4;
                        chara->attack.segVec[1].vy = temp2;
                        chara->attack.segVec[1].vz = temp_s0_9;
                        func_8008B714(chara, ptr, &D_800C4728.target, 0);
                    }
                }
            }

            if (i >= sp2C)
            {
                sp18 = 1;
            }
            else
            {
                i -= sp34;
            }

            if (sp3C == 0)
            {
                i = sp28;
                sp18   = 1;
            }

            if (sp40 >= shotCountMax)
            {
                i = sp28;
                sp18   = 1;
            }

            chara->attack.hitCheckTime = i;
            chara->attack.shotsFired  = sp40;
            chara->attack.shotsLeft  = sp3C;
        }
        else
        {
            if (var_a0 <= 0)
            {
                var_a0 = 1;
            }

            if (temp_s1 == AttackInfoType_Slash)
            {
                sp58 = 0;
                if (i < sp28)
                {
                    sp5C = (sp28 - i) / (var_a0 * 4);
                }
                else
                {
                    sp5C = Q12(0.0f);
                }
            }
            else
            {
                sp58 = 1;
                sp5C = Q12(1.0f);
            }

            if (sp10 == 2)
            {
                func_8008B15C(chara);
                sp10 = 3;
            }

            temp_s0_10 = chara->attack.prevAimPitch;
            temp_s1_5  = chara->attack.prevAimYaw;

            sp60 = chara->attack.prevOrigin.vx;
            sp64 = chara->attack.prevOrigin.vy;
            sp68 = chara->attack.prevOrigin.vz;

            sp6C = chara->attack.origin.vx;
            sp70 = chara->attack.origin.vy;
            sp74 = chara->attack.origin.vz;

            sp7C = Math_Cos(temp_s0_10);
            sp80 = Math_Sin(temp_s0_10);

            sp78 = Q12_MULT(sp80, Math_Sin(temp_s1_5));
            sp80 = Q12_MULT(sp80, Math_Cos(temp_s1_5));

            sp88 = Math_Cos(sp24);
            sp8C = Math_Sin(sp24);

            sp84 = Q12_MULT(sp8C, Math_Sin(sp20));
            sp8C = Q12_MULT(sp8C, Math_Cos(sp20));
            sp90 = 0;
            sp94 = 0;

            for (var_s2 = 4, var_fp = 0, var_s3 = 0, i = 0, var_s6 = 0;
                 var_s2 > 0;
                 sp90  += sp68,
                 sp94   += sp64,
                 var_fp += sp60,
                 var_s3 += sp80,
                 i += sp7C,
                 var_s6 += sp78,
                 var_s2--)
            {
                var_a0_3 = (sp84 * var_s2) + var_s6;
                temp5 = var_a0_3 / 4;

                var_a1 = (sp88 * var_s2) + i;
                temp4 = var_a1 / 4;

                temp_s0_11 = ((sp8C * var_s2) + var_s3) / 4;

                temp_s0 = ratan2(temp5, temp_s0_11);

                D_800C4748[var_s2 - 1].vy = ratan2(Math_Vector2MagCalc(temp5, temp_s0_11), temp4);
                D_800C4748[var_s2 - 1].vx = temp_s0;

                temp_a1 = var_s2 - 1;
                temp_a0 = &D_800C4758[temp_a1];

                var_v1 = (sp6C * var_s2) + var_fp;
                temp_a0->vx = var_v1 / 4;

                temp_a0->vy = ((sp70 * var_s2) + sp94) / 4;

                temp_v0_10 = (sp74 * var_s2) + sp90;
                temp_a0->vz = temp_v0_10 / 4;
            }

            chara->attack.prevAimYaw = chara->attack.aimYaw;
            chara->attack.prevAimPitch = chara->attack.aimPitch;

            for (var_s2 = 0, temp_s1_3 = 4; (temp_s1_3 > 0) && (var_s2 == 0); temp_s1_3--)
            {
                temp_v0_11                 = &D_800C4758[4 - temp_s1_3];
                chara->attack.origin.vx = temp_v0_11->vx;
                chara->attack.origin.vy = temp_v0_11->vy;

                temp_s0_14                 = &D_800C4748[4 - temp_s1_3];
                chara->attack.origin.vz = temp_v0_11->vz;

                func_8008B1DC(chara, temp_s0_14->vx, temp_s0_14->vy);

                if (Combat_MeleeHitCheck(chara, temp_s0_14->vx, attackInfo, sp5C) != 0)
                {
                    var_s2 = sp58;
                    sp18  |= var_s2;
                }
            }

            i     = sp28;
            temp_v0_12 = &chara->attack;

            temp_v0_12->origin.vx       = sp6C;
            chara->attack.prevOrigin.vx = sp6C;
            temp_v0_12->origin.vy       = sp70;
            chara->attack.prevOrigin.vy = sp70;
            temp_v0_12->origin.vz       = sp74;
            chara->attack.prevOrigin.vz = sp74;
        }

        if (sp18 != 0)
        {
            chara->attack.state = NO_VALUE;

            if (chara->model.charaId == Chara_Harry)
            {
                switch (weaponAttack)
                {
                    case WEAPON_ATTACK(AttackType_Handgun,      AttackInputType_Tap):
                    case WEAPON_ATTACK(AttackType_HuntingRifle, AttackInputType_Tap):
                        if (sp30 != NO_VALUE)
                        {
                            if (WorldEnv_IsInDarkness())
                            {
                                var_s0_2 = 3;
                            }
                            else
                            {
                                var_s0_2 = sp30;
                            }
                        }
                        else
                        {
                            var_s0_2 = 5;
                        }
                        break;

                    case WEAPON_ATTACK(AttackType_Shotgun, AttackInputType_Tap):
                        var_s0_2 = 5;
                        break;

                    case WEAPON_ATTACK(AttackType_HyperBlaster, AttackInputType_Tap):
                        var_s0_2 = 4;
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
                        var_s0_2 = 7;
                        break;

                    case WEAPON_ATTACK(AttackType_Kick,  AttackInputType_Tap):
                    case WEAPON_ATTACK(AttackType_Stomp, AttackInputType_Tap):
                        var_s0_2 = NO_VALUE;
                        break;

                    default:
                        var_s0_2 = 6;
                        break;
                }

                if (var_s0_2 != NO_VALUE)
                {
                    func_8009151C(var_s0_2, chara->attack.hitFlags != 0, func_8007FD2C());
                }
            }
        }
        else
        {
            chara->attack.state  = sp10;
            chara->attack.hitCheckTime = i;
        }
    }

    return chara->attack.hitFlags;
}

void func_8008B15C(s_SubCharacter* chara) // 0x8008B15C
{
    s32 aimPitch;
    s32 aimYaw;
    s32 i;

    aimYaw   = chara->attack.aimYaw;
    aimPitch = chara->attack.aimPitch;

    chara->attack.prevAimYaw   = aimYaw;
    chara->attack.prevAimPitch = aimPitch;

    // @hack Loop is required for match, might be based on `attack->segVec` ARRAY_SIZE but not totally sure.
    // Code in this loop only runs once.
    for (i = ARRAY_SIZE(chara->attack.segVec) - 1; i > 0; i--)
    {
        func_8008B1DC(chara, aimYaw, aimPitch);
        chara->attack.prevOrigin = chara->attack.origin;
    }
}

void func_8008B1DC(s_SubCharacter* chara, q19_12 aimYaw, q19_12 aimPitch) // 0x8008B1DC
{
    s32                rangeMin;
    q19_12             sinPitch;
    q19_12             cosPitch;
    s32                dirX;
    s32                dirZ;
    s32                i;
    s32                segLength;
    s32                weaponAttack;
    s32                segVecX;
    s32                segVecY;
    s32                segVecZ;
    s32                segStartX;
    s32                segStartY;
    s32                segStartZ;
    s32                originX;
    s32                originY;
    s32                originZ;
    s_AttackInfo*      attackInfo;
    s_CharaAttack*     attack;

    attack       = &chara->attack;
    weaponAttack = chara->attack.weaponAttack;

    // Shift segment history.
    // @hack Loop is required for match, might be based on `attack->segVec` ARRAY_SIZE but not totally sure.
    // Code in this loop only runs once.
    for (i = ARRAY_SIZE(attack->segVec) - 2; i >= 0; i--)
    {
        segVecX = attack->segVec[i].vx;
        segVecY = attack->segVec[i].vy;
        segVecZ = attack->segVec[i].vz;

        attack->segVec[i + 1].vx = segVecX;
        attack->segVec[i + 1].vy = segVecY;
        attack->segVec[i + 1].vz = segVecZ;

        segStartX = attack->segStart[i].vx;
        segStartY = attack->segStart[i].vy;
        segStartZ = attack->segStart[i].vz;

        attack->segStart[i + 1].vx = segStartX;
        attack->segStart[i + 1].vy = segStartY;
        attack->segStart[i + 1].vz = segStartZ;
    }

    attackInfo = &g_Items_WeaponAttackInfos[weaponAttack];

    segLength = attackInfo->rangeMax;
    rangeMin  = attackInfo->rangeMin;

    originX = chara->attack.origin.vx;
    originY = chara->attack.origin.vy;
    originZ = chara->attack.origin.vz;

    segStartX = originX;
    segStartY = originY;
    segStartZ = originZ;

    cosPitch = Math_Cos(aimPitch);
    sinPitch = Math_Sin(aimPitch);

    dirX = Q12_MULT(sinPitch, Math_Sin(aimYaw));
    dirZ = Q12_MULT(sinPitch, Math_Cos(aimYaw));

    segLength -= rangeMin;

    segStartX                   += Q12_MULT(rangeMin, dirX);
    chara->attack.segStart[0].vx = segStartX;

    segStartY                   += Q12_MULT(rangeMin, cosPitch);
    chara->attack.segStart[0].vy = segStartY;

    segStartZ                   += Q12_MULT(rangeMin, dirZ);
    chara->attack.segStart[0].vz = segStartZ;

    segVecX                    = Q12_MULT(segLength, dirX);
    chara->attack.segVec[0].vx = segVecX;

    segVecY                    = Q12_MULT(segLength, cosPitch);
    chara->attack.segVec[0].vy = segVecY;

    segVecZ                    = Q12_MULT(segLength, dirZ);
    chara->attack.segVec[0].vz = segVecZ;
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
        vol = 0;
    }

    if (vol >= 0)
    {
        if (vol != 0)
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

s32 func_8008B714(s_SubCharacter* attacker, s_SubCharacter* target, VECTOR3* hitPos, q19_12 damageScale) // 0x8008B714
{
    s32           targetBit;
    s32           hitFlags;
    u32           damage;
    q19_12        pushX;
    q19_12        pushY;
    q19_12        pushZ;
    s32           attackType;
    q20_12        damageAmount;
    s32           pushLength;
    u32           shock;
    s32           weaponAttack;
    s_AttackInfo* attackInfo;

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
            // @bug `hitCheckTime` is Q12 but `func_8008A35C` compares whole keyframes?
            if (func_8008A35C(attackInfo, attacker->attack.hitCheckTime) == 0)
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
                    damage += damage * 2;
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

    shock        = attackInfo->shock;
    damageAmount = Q12_MULT_PRECISE(damage, (Q12(4.5f) - Rng_RandQ12()) / 4);

    switch (weaponAttack)
    {
        case WEAPON_ATTACK(AttackType_Handgun,      AttackInputType_Tap):
        case WEAPON_ATTACK(AttackType_HuntingRifle, AttackInputType_Tap):
        case WEAPON_ATTACK(AttackType_Shotgun,      AttackInputType_Tap):
        case WEAPON_ATTACK(AttackType_HyperBlaster, AttackInputType_Tap):
        case NO_VALUE:
            pushX = attacker->attack.segVec[1].vx;
            pushY = attacker->attack.segVec[1].vy;
            pushZ = attacker->attack.segVec[1].vz;

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
            if (target != &PLAYER_CHARA() && !(target->flags & CharaFlag_Hit))
            {
                damageAmount *= 4;
            }

            if (attackType == AttackInfoType_Slash)
            {
                damageAmount = Q12_MULT_PRECISE(damageAmount, damageScale);
                shock        = Q12_MULT_PRECISE(shock, damageScale);
            }

            if (weaponAttack == WEAPON_ATTACK(AttackType_Kick,  AttackInputType_Tap) ||
                weaponAttack == WEAPON_ATTACK(AttackType_Stomp, AttackInputType_Tap))
            {
                pushX = attacker->attack.segVec[0].vx;
                pushY = 0;
                pushZ = attacker->attack.segVec[0].vz;
            }
            else
            {
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

                prevTipX = attacker->attack.field_60.vx;
                prevTipY = attacker->attack.field_60.vy;
                prevTipZ = attacker->attack.field_60.vz;

                tipX = attacker->attack.segVec[0].vx;
                tipY = attacker->attack.segVec[0].vy;
                tipZ = attacker->attack.segVec[0].vz;

                prevTipX += baseX;
                prevTipY += baseY;
                prevTipZ += baseZ;

                baseX = attacker->attack.segStart[0].vx;
                baseY = attacker->attack.segStart[0].vy;
                baseZ = attacker->attack.segStart[0].vz;

                tipX += baseX;
                tipY += baseY;
                tipZ += baseZ;

                pushX = tipX - prevTipX;
                pushY = tipY - prevTipY;
                pushZ = tipZ - prevTipZ;
            }

            pushLength = func_8008A058(func_80080540(pushX, pushY, pushZ));
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

    if (damageAmount != Q12(0.0f))
    {
        target->damage.amount += damageAmount;
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

    if (damageAmount | shock)
    {
        s32 impactSfxGroup = attackInfo->impactSfxGroup;

        if (impactSfxGroup > 0 && impactSfxGroup < 8)
        {
            s32 impactCharaType = NO_VALUE; // TODO: Enum for these chara types.

            switch (target->model.charaId)
            {
                case Chara_Harry:
                    if (target->health >= Q12(0.0f))
                    {
                        impactCharaType = 1;
                    }
                    break;

                case Chara_AirScreamer:
                case Chara_NightFlutter:
                    impactCharaType = 2;
                    break;

                case Chara_Stalker:
                    impactCharaType = 4;
                    break;

                case Chara_LarvalStalker:
                case Chara_GreyChild:
                case Chara_Mumbler:
                    impactCharaType = 3;
                    break;

                case Chara_Creeper:
                    impactCharaType = 5;
                    break;

                case Chara_Groaner:
                case Chara_Wormhead:
                    impactCharaType = 6;
                    break;

                case Chara_SplitHead:
                    impactCharaType = 7;
                    break;

                case Chara_HangedScratcher:
                    impactCharaType = 8;
                    break;

                case Chara_Floatstinger:
                case Chara_Twinfeeler:
                    impactCharaType = 10;
                    break;

                case Chara_Romper:
                    impactCharaType = 9;
                    break;

                case Chara_PuppetNurse:
                case Chara_PuppetDoctor:
                    impactCharaType = 11;
                    break;

                case Chara_Bloodsucker:
                    impactCharaType = 12;
                    impactSfxGroup  = 1;
                    break;

                case Chara_Incubus:
                case Chara_Unknown23:
                case Chara_Incubator:
                    if (!(target->flags & CharaFlag_Hit))
                    {
                        if (weaponAttack == WEAPON_ATTACK(AttackType_Shotgun, AttackInputType_Tap))
                        {
                            impactSfxGroup = 4;
                        }
                        else
                        {
                            impactSfxGroup = 7;
                        }
                    }

                default:
                    impactCharaType = 16;
                    break;

                case Chara_Padlock:
                    impactCharaType = NO_VALUE;
                    break;
            }

            if (impactCharaType >= 0)
            {
                func_8005F6B0(target, hitPos, impactSfxGroup, impactCharaType);
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
    q19_12          sinAngle;
    q19_12          cosAngle;
    q19_12          segStartX;
    q19_12          segStartY;
    q19_12          segStartZ;
    q19_12          segVecX;
    q19_12          segVecY;
    q19_12          segVecZ;
    q19_12          localSegX;
    q19_12          localSegY;
    q19_12          localSegZ;
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

    static VECTOR3    localSegPoints[4]; // Q19.12 | Hit segment in attacker-local space. Written but never read.
    static VECTOR3    hitPos;
    static s32        __pad_bss_800C47C4;
    static VECTOR3    rayPoints[2];      // Line-of-sight ray from the attacker to each candidate.
    static s32        __pad_bss_800C47E0[2];
    static VECTOR3    otherCenter;
    static s32        __pad_bss_800C47F4;
    static s_RayTrace rayTrace;

    segStartX = attacker->attack.segStart[0].vx;
    segStartY = attacker->attack.segStart[0].vy;
    segStartZ = attacker->attack.segStart[0].vz;

    sinAngle = Math_Sin(yaw);
    cosAngle = Math_Cos(yaw);

    localSegPoints[0].vx = Q12(0.0f);
    localSegPoints[0].vy = Q12(0.0f);
    localSegPoints[0].vz = Q12(0.0f);

    segVecX = attacker->attack.segVec[0].vx;
    segVecZ = attacker->attack.segVec[0].vz;

    boxMinX = 0;
    boxMaxX = 0;
    boxMinY = 0;
    boxMaxY = 0;
    boxMinZ = 0;
    boxMaxZ = 0;

    segVecY          = attacker->attack.segVec[0].vy;
    localSegPoints[1].vy = segVecY;

    localSegX        = Q12_MULT_PRECISE(segVecX, cosAngle) - Q12_MULT_PRECISE(segVecZ, sinAngle);
    localSegPoints[1].vx = localSegX;

    localSegZ        = Q12_MULT_PRECISE(segVecX, sinAngle) + Q12_MULT_PRECISE(segVecZ, cosAngle);
    localSegPoints[1].vz = localSegZ;

    localSegY = segVecY;

    if (localSegX > 0)
    {
        boxMaxX = localSegX;
    }
    else if (localSegX < 0)
    {
        boxMinX = localSegX;
    }

    if (boxMaxY < localSegY)
    {
        boxMaxY = localSegY;
    }
    else if (localSegY < boxMinY)
    {
        boxMinY = localSegY;
    }

    if (boxMaxZ < localSegZ)
    {
        boxMaxZ = localSegZ;
    }
    else if (localSegZ < boxMinZ)
    {
        boxMinZ = localSegZ;
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

        // @hack Box offsets loaded through `offsetX` and `innerRadius`, required for match.
        offsetX     = other->collision.shapeOffsets.box.vx;
        innerRadius = other->collision.shapeOffsets.box.vz;

        targetX += offsetX;
        rayToY  += other->collision.box.topExtent;
        targetZ += innerRadius;

        rayPoints[1].vy = rayToY;
        innerRadius      = other->collision.cylinder.innerRadius;

        // @bug `rayX` and `rayZ` are meant to stay at the ray start, but are overwritten with each candidate's ray end
        // below. From the second candidate on, this angle is measured from the previous candidate's ray end instead.
        angleToOther = ratan2(targetX - rayX, targetZ - rayZ);
        offsetX      = Q12_MULT_PRECISE(innerRadius, Math_Sin(angleToOther));
        cosAngleToOther = Math_Cos(angleToOther);

        rayX = targetX - offsetX;
        rayZ = targetZ - Q12_MULT_PRECISE(innerRadius, cosAngleToOther);

        rayPoints[1].vx = rayX;
        rayPoints[1].vz = rayZ;

        isBlocked = Ray_GeomTraceGet(&rayTrace, &rayPoints[0], &rayPoints[1]);
        if (isBlocked != false)
        {
            isBlocked = true; // @hack Redundant normalize, required for match.
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

            relX = otherCenterX - segStartX;
            relZ = otherCenterZ - segStartZ;

            otherPosY      = other->position.vy;
            otherHeight    = other->collision.box.height;
            otherTopExtent = other->collision.box.topExtent;

            relY   = otherPosY - segStartY;
            localX = Q12_MULT_PRECISE(relX, cosAngle) - Q12_MULT_PRECISE(relZ, sinAngle);
            localZ = Q12_MULT_PRECISE(relX, sinAngle) + Q12_MULT_PRECISE(relZ, cosAngle);

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

            // @bug Overwrites the current `segVec` with the point 3/4 along the segment, each later
            // candidate that reaches here converts the already converted point again.
            segVecX -= segVecX / 4;
            segVecY -= segVecY / 4;
            segVecZ -= segVecZ / 4;

            segVecX += segStartX;
            segVecY += segStartY;
            segVecZ += segStartZ;

            otherMidY = (otherTopExtent + otherHeight) / 2;

            hitOffsetX = segVecX - otherCenterX;
            hitOffsetZ = segVecZ - otherCenterZ;
            hitOffsetY = segVecY - (otherPosY + otherMidY);

            hitOffsetXZ = func_8008A058(func_80080540(hitOffsetX, Q12(0.0f), hitOffsetZ));
            halfHeight  = CLAMP_LOW((otherHeight - otherTopExtent) / 2, 0);

            for (j = 2; (halfHeight < hitOffsetY || hitOffsetY < -halfHeight || innerRadius < hitOffsetXZ) && j > 0; j--)
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

            if (func_8008B714(attacker, other, &hitPos, damageScale))
            {
                hitFlags |= charaBit;
            }
        }
    }

    attacker->attack.hitFlags |= hitFlags;
    return hitFlags;
}
