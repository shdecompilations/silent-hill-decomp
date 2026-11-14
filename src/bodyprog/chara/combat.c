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
            if (g_SysWork.field_275C > Q12(256.0f))
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
        if (g_SysWork.field_275C > Q12(256.0f))
        {
            return 0;
        }
    }
#endif

    return g_Items_WeaponAttackInfos[weaponAttack].hitStartKeyframe;
}

u32 func_8008A2E0(s32 weaponAttack) // 0x8008A2E0
{
    if (g_SysWork.field_275C > Q12(256.0f))
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

        if (temp_s1 == 3)
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

            if (temp_s1 == 1)
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

                if (func_8008BF84(chara, temp_s0_14->vx, attackInfo, sp5C) != 0)
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

void func_8008B398(void) // 0x8008B398
{
    func_8008B3E4(Q8(0.0f));
    func_8008B438(0, 0, 0);
    func_8008B438(1, 0, 0);
    func_8008B40C(0, 0);
}

void func_8008B3E4(q23_8 vol) // 0x8008B3E4
{
    func_8008B474(0, vol, 0);
}

void func_8008B40C(q23_8 vol, s32 soundType) // 0x8008B40C
{
    func_8008B474(3, vol, soundType);
}

void func_8008B438(s32 arg0, q23_8 vol, s32 soundType) // 0x8008B438
{
    bool cond;

    cond = arg0 != 0;
    if (func_8008B474(2 - cond, vol, soundType))
    {
        func_80089314(cond);
    }
}

bool func_8008B474(s32 arg0, q23_8 vol, s32 soundType) // 0x8008B474
{
    s32 unkVol;
    s32 cond;
    u16 sfxId;

    unkVol = 0;
    sfxId = 0;
    switch (arg0)
    {
        case 0:
            sfxId = Sfx_Unk1300;
            unkVol = g_SysWork.field_2760;
            soundType = 0;
            break;

        case 1:
            sfxId = Sfx_Unk1301;
            unkVol = g_SysWork.field_275C;
            break;

        case 2:
            sfxId = Sfx_Unk1302;
            unkVol = g_SysWork.field_275C;
            break;

        case 3:
            sfxId = Sfx_Unk1303;
            unkVol = g_SysWork.field_2764;
            break;
    }

    cond = unkVol == Q12(0.0f);

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
                unkVol = Q12(511.0f);
            }
            else
            {
                unkVol = Q12(vol) + Q12(256.0f);
            }
        }
        else
        {
            unkVol = 0;
        }
    }
    else
    {
        unkVol -= g_DeltaTime << 9;
        if (unkVol < 0)
        {
            unkVol = 0;
        }
    }

    if (unkVol == 0)
    {
        Sd_SfxStop(sfxId);
    }
    else if (cond)
    {
        // NOTE: Sfx_WithFlagsPlay calls Sfx_WithFlagsAndPitchPlay. `soundType` is `pitch` when calling `Sfx_WithFlagsAndPitchPlay` directly.
        if (unkVol > Q12(256.0f))
        {
            Sfx_WithFlagsPlay(sfxId, &PLAYER_CHARA().position, FP_FROM(unkVol - Q12(256.0f), Q12_SHIFT), soundType);
        }
        else
        {
            Sfx_WithFlagsPlay(sfxId, &PLAYER_CHARA().position, Q8_CLAMPED(0.0f), soundType);
        }
    }
    else
    {
        if (unkVol > Q12(256.0f))
        {
            Sfx_WithFlagsAndPitchPlay(sfxId, &PLAYER_CHARA().position, FP_FROM(unkVol - Q12(256.0f), Q12_SHIFT), 4, soundType);
        }
        else
        {
            Sfx_WithFlagsAndPitchPlay(sfxId, &PLAYER_CHARA().position, 0, 4, soundType);
        }
    }

    switch (arg0)
    {
        case 0:
            g_SysWork.field_2760 = unkVol;
            break;

        case 1:
        case 2:
            g_SysWork.field_275C = unkVol;
            break;

        case 3:
            g_SysWork.field_2764 = unkVol;
    }

    return unkVol > Q12(256.0f);
}

void func_8008B664(VECTOR3* pos, u32 caseVar) // 0x8008B664
{
    s32 sfxId;

    sfxId = 0;
    switch (caseVar)
    {
        case 0:
        case 6:
        case 10:
        case 16:
        case 20:
        case 26:
            sfxId = Sfx_Unk1297;
            break;

        case 1:
        case 4:
        case 7:
        case 11:
        case 14:
        case 17:
        case 21:
        case 24:
        case 27:
            sfxId = Sfx_Unk1296;
            break;

        case 32:
        case 33:
        case 34:
            sfxId = Sfx_Unk1286;
            break;

        case 2:
        case 5:
        case 15:
        case 25:
            if (g_SysWork.field_275C > Q12(256.0f))
            {
                func_8008B3E4(Q8(0.5f));
                break;
            }

        case 8:
        case 9:
            sfxId = Sfx_Unk1316;
            break;

        case 35:
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
        targetBit        = NO_VALUE;
        target->field_40 = attacker - g_SysWork.npcs; // Sets `field_40` to attacker index inside `g_SysWork.npcs`
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
                    if (g_SysWork.field_275C > Q12(256.0f))
                    {
                        func_8008B3E4(Q8(0.5f));
                    }
                    break;
            }
        }
        else
        {
            switch (attackType)
            {
                case AttackInfoType_Melee1Hit:
                    func_800892A4(6);
                    break;

                case AttackInfoType_Melee:
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

            if (g_SysWork.field_275C > Q12(256.0f))
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

            if (attackType == AttackInfoType_Melee)
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

s32 func_8008BF84(s_SubCharacter* chara, q19_12 angle, s_AttackInfo* attackInfo, s32 arg3) // 0x8008BF84
{
    s_SubCharacter* chara1;
    s32             sp14;
    s32             sp18;
    s32             i;
    q19_12          sinAngle;
    q19_12          cosAngle;
    s32             countX;
    s32             countY;
    s32             countZ;
    q19_12          sp34;
    s32             sp38;
    s32             sp3C;
    s32             sp40;
    s32             sp44;
    s32             sp48;
    s32             sp4C;
    q19_12          posX;
    q19_12          posZ;
    q19_12          sp58;
    s32             sp5C;
    s32             temp_s5;
    s32             temp_s6;
    q19_12          temp_a0;
    s32             temp_a0_2;
    q19_12          temp_a0_3;
    s32             temp_lo_5;
    s32             temp_s0;
    s32             temp_s1;
    q19_12          unkX;
    q19_12          innerRadius;
    q19_12          temp_t2;
    s32             j;
    q19_12          temp_t4;
    q19_12          temp_t4_2;
    q19_12          temp_t5;
    s32             angle1;
    s32             temp_v0_6;
    s32             temp_v0_7;
    q19_12          temp_v0_8;
    q19_12          var_a1_2;
    s32             var_fp;
    q19_12          unkZ;
    q19_12          var_s7;
    s32             var_t1;
    s32             var_t2;
    s32             var_v0;
    q19_12          var_v0_3;
    s32             var_v0_6;
    q19_12          var_v1;
    s32             temp_a2;
    u16             temp_v1;
    q19_12          posY;
    s32             temp4;
    s_SubCharacter* chara0;

    static VECTOR3    D_800C4788[4]; // Q19.12
    static VECTOR3    D_800C47B8;
    static s32        __pad_bss_800C47C4;
    static VECTOR3    D_800C47C8[2];
    static s32        __pad_bss_800C4E0[2];
    static VECTOR3    D_800C47E8;
    static s32        __pad_bss_800C4F4;
    static s_RayTrace D_800C47F8;

    countX = chara->attack.segStart[0].vx;
    countY = chara->attack.segStart[0].vy;
    countZ = chara->attack.segStart[0].vz;

    sinAngle = Math_Sin(angle);
    cosAngle = Math_Cos(angle);

    D_800C4788[0].vx = Q12(0.0f);
    D_800C4788[0].vy = Q12(0.0f);
    D_800C4788[0].vz = Q12(0.0f);

    sp34   = chara->attack.segVec[0].vx;
    var_fp = chara->attack.segVec[0].vz;

    sp44 = 0;
    sp38 = 0;
    sp48 = 0;
    sp3C = 0;
    sp4C = 0;
    sp40 = 0;

    var_s7           = chara->attack.segVec[0].vy;
    D_800C4788[1].vy = var_s7;

    temp_t4          = Q12_MULT_PRECISE(sp34, cosAngle) - Q12_MULT_PRECISE(var_fp, sinAngle);
    D_800C4788[1].vx = temp_t4;

    temp_a0          = Q12_MULT_PRECISE(sp34, sinAngle) + Q12_MULT_PRECISE(var_fp, cosAngle);
    D_800C4788[1].vz = temp_a0;

    temp4 = var_s7;

    if (temp_t4 > 0)
    {
        sp38 = temp_t4;
    }
    else if (temp_t4 < 0)
    {
        sp44 = temp_t4;
    }

    if (sp3C < temp4)
    {
        sp3C = temp4;
    }
    else if (temp4 < sp48)
    {
        sp48 = temp4;
    }

    if (sp40 < temp_a0)
    {
        sp40 = temp_a0;
    }
    else if (temp_a0 < sp4C)
    {
        sp4C = temp_a0;
    }

    temp_v1   = attackInfo->hitCheck.fight.sizeX;
    temp_a0_2 = attackInfo->hitCheck.fight.sizeY;
    temp_a2   = attackInfo->hitCheck.fight.sizeZ;

    var_v0    = (sp38 - sp44);
    temp_v0_7 = (sp3C - sp48);
    var_v0_6  = (sp40 - sp4C);

    if (var_v0 < temp_v1)
    {
        sp38 += DIV_FAST(attackInfo->hitCheck.fight.sizeX, 2);
        sp44 -= DIV_FAST(attackInfo->hitCheck.fight.sizeX, 2);
    }

    if (temp_v0_7 < temp_a0_2)
    {
        sp3C += DIV_FAST(attackInfo->hitCheck.fight.sizeY, 2);
        sp48 -= DIV_FAST(attackInfo->hitCheck.fight.sizeY, 2);
    }

    if (var_v0_6 < temp_a2)
    {
        sp40 += DIV_FAST(attackInfo->hitCheck.fight.sizeZ, 2);
        sp4C -= DIV_FAST(attackInfo->hitCheck.fight.sizeZ, 2);
    }

    if (chara == &PLAYER_CHARA())
    {
        chara0 = &NPC_CHARA(0);
        var_v0 = NPC_COUNT_MAX;
        var_v1 = 1;
    }
    else
    {
        chara0 = &PLAYER_CHARA();
        var_v0 = 1;
        var_v1 = -1;
    }

    sp18 = var_v1;
    chara1 = chara0;
    sp14 = 0;

    posX = chara->position.vx;
    posZ = chara->position.vz;

    posY  = chara->position.vy;
    posY += chara->collision.box.topExtent;

    D_800C47C8[0].vx = posX;
    D_800C47C8[0].vy = posY;
    D_800C47C8[0].vz = posZ;

    for (i = var_v0; i > 0; i--, chara1++, sp18 *= 2)
    {
        if (g_SysWork.sysState == SysState_GameOver ||
            PLAYER_CHARA().health <= Q12(0.0f) ||
            chara1 == chara ||
            chara1->model.charaId == Chara_None ||
            chara1->health < Q12(0.0f) ||
            chara1->collision.state == CharaCollisionState_Ignore)
        {
            continue;
        }

        D_800C47E8.vx = chara1->position.vx + chara1->collision.shapeOffsets.box.vx;
        D_800C47E8.vy = chara1->position.vy;
        D_800C47E8.vz = chara1->position.vz + chara1->collision.shapeOffsets.box.vz;
        if (Math_Distance2dGet(&chara->position, &D_800C47E8) > Q12(3.0f))
        {
            continue;
        }

        temp_s1 = chara1->position.vx;
        var_v1  = chara1->position.vy;
        unkX = chara1->position.vz;

        temp_s0 = chara1->collision.shapeOffsets.box.vx;
        innerRadius = chara1->collision.shapeOffsets.box.vz;

        temp_s1 += temp_s0;

        var_v1  += chara1->collision.box.topExtent;
        unkX += innerRadius;

        D_800C47C8[1].vy = var_v1;
        innerRadius          = chara1->collision.cylinder.innerRadius;

        angle1 = ratan2(temp_s1 - posX, unkX - posZ);
        temp_v0_6 = Math_Sin(angle1);
        temp_s0   = Q12_MULT_PRECISE(innerRadius, temp_v0_6);

        unkZ = Math_Cos(angle1);

        posX = temp_s1 - temp_s0;

        D_800C47C8[1].vx = posX;
        posZ             = unkX - Q12_MULT_PRECISE(innerRadius, unkZ);

        D_800C47C8[1].vz = posZ;
        var_v1           = Ray_GeomTraceGet(&D_800C47F8, &D_800C47C8[0], &D_800C47C8[1]);

        if (var_v1 != false)
        {
            var_v1 = true;
        }

        if (var_v1 == true)
        {
            continue;
        }

        temp_t4_2  = chara1->position.vx;
        temp_t4_2 += chara1->collision.shapeOffsets.box.vx;

        var_v1   = temp_t4_2 - countX;
        temp_t5  = chara1->position.vz;
        temp_t5 += chara1->collision.shapeOffsets.box.vz;

        temp_v0_8 = temp_t5 - countZ;

        sp58    = chara1->position.vy;
        temp_s6 = chara1->collision.box.height;
        temp_s5 = chara1->collision.box.topExtent;

        j   = sp58 - countY;
        temp_t2   = Q12_MULT_PRECISE(var_v1, cosAngle) - Q12_MULT_PRECISE(temp_v0_8, sinAngle);
        temp_a0_3 = Q12_MULT_PRECISE(var_v1, sinAngle) + Q12_MULT_PRECISE(temp_v0_8, cosAngle);

        if ((j < (sp48 - temp_s6)) || ((sp3C - temp_s5) < j))
        {
            continue;
        }

        innerRadius  = chara1->collision.cylinder.innerRadius;
        var_a1_2 = Q12(0.0f);

        if (temp_t2 < sp44)
        {
            var_a1_2 = sp44 - temp_t2;
        }
        else if (sp38 < temp_t2)
        {
            var_a1_2 = temp_t2 - sp38;
        }

        var_v1 = Q12(0.0f);

        if (temp_a0_3 < sp4C)
        {
            var_v1 = sp4C - temp_a0_3;
        }
        else if (sp40 < temp_a0_3)
        {
            var_v1 = temp_a0_3 - sp40;
        }

        if (var_a1_2 | var_v1)
        {
            if (var_a1_2 == Q12(0.0f))
            {
                if (innerRadius < var_v1)
                {
                    continue;
                }
            }
            else
            {
                if (var_v1 == Q12(0.0f))
                {
                    if (innerRadius < var_a1_2)
                    {
                        continue;
                    }
                }
                else
                {
                    var_v0_3 = Q12_MULT_PRECISE(var_a1_2, var_a1_2) + Q12_MULT_PRECISE(var_v1, var_v1);
                    if (Q12_MULT_PRECISE(innerRadius, innerRadius) < var_v0_3)
                    {
                        continue;
                    }
                }
            }
        }

        sp34   -= sp34 / 4;
        var_s7 -= var_s7 / 4;
        var_fp -= var_fp / 4;

        sp34   += countX;
        var_s7 += countY;
        var_fp += countZ;

        sp5C = (temp_s5 + temp_s6) / 2;

        unkX = sp34 - temp_t4_2;
        unkZ  = var_fp - temp_t5;
        temp_s0 = var_s7 - (sp58 + sp5C);

        var_t1 = func_8008A058(func_80080540(unkX, Q12(0.0f), unkZ));
        var_t2 = (temp_s6 - temp_s5) / 2;

        if (var_t2 < 0)
        {
            var_t2 = 0;
        }

        for (j = 2; (var_t2 < temp_s0 || temp_s0 < -var_t2 || innerRadius < var_t1) && j > 0; j--)
        {
            if (innerRadius < var_t1)
            {
                temp_lo_5 = Q12(innerRadius) / var_t1;
                var_t1    = innerRadius;
                unkX   = Q12_MULT_PRECISE(temp_lo_5, unkX);
                temp_s0   = Q12_MULT_PRECISE(temp_lo_5, temp_s0);
                unkZ    = Q12_MULT_PRECISE(temp_lo_5, unkZ);
            }
            else if (var_t2 < temp_s0)
            {
                temp_lo_5 = Q12(var_t2) / temp_s0;
                temp_s0   = var_t2;
                var_t1    = Q12_MULT_PRECISE(temp_lo_5, var_t1);
                unkX   = Q12_MULT_PRECISE(temp_lo_5, unkX);
                unkZ    = Q12_MULT_PRECISE(temp_lo_5, unkZ);
            }
            else if (temp_s0 < -var_t2)
            {
                temp_lo_5 = Q12(-var_t2) / temp_s0;
                temp_s0   = -var_t2;
                var_t1    = Q12_MULT_PRECISE(temp_lo_5, var_t1);
                unkX   = Q12_MULT_PRECISE(temp_lo_5, unkX);
                unkZ    = Q12_MULT_PRECISE(temp_lo_5, unkZ);
            }
        }

        D_800C47B8.vx = temp_t4_2 + unkX;
        D_800C47B8.vy = (sp58 + sp5C) + temp_s0;
        D_800C47B8.vz = temp_t5 + unkZ;

        if (func_8008B714(chara, chara1, &D_800C47B8, arg3))
        {
            sp14 |= sp18;
        }
    }

    chara->attack.hitFlags |= sp14;
    return sp14;
}
