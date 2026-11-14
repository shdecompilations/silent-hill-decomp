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

s32 func_8008A0E4(s32 arg0, s32 weaponAttack, s_SubCharacter* chara, VECTOR3* attackPos, s_SubCharacter* unused,
                  q3_12 rotToTargetX, q3_12 rotToTargetY) // 0x8008A0E4
{
    s32          var_t1;
    s32          temp_a1;
    s32          var_a0;
    s32          ret;
    s32          var_v0_2;
    s32          count;
    s_AnimInfo*  mapAnimInfos;
    s_AnimInfo*  baseAnimInfos;
    s_ModelAnim* modelAnim;

    var_t1    = chara->field_44.field_0;
    modelAnim = &chara->model.anim;

    if (g_DeltaTime == Q12(0.0f) || g_SysWork.sysState != SysState_Gameplay)
    {
        return NO_VALUE;
    }

    if (chara == &PLAYER_CHARA())
    {
        baseAnimInfos = D_800297B8;
        mapAnimInfos  = g_MapOverlayHdr.harryMapAnimInfos;

        modelAnim->baseAnimInfos      = baseAnimInfos;
        modelAnim->mapAnimStatusStart = ANIM_STATUS(38, false); // TODO: 38 is the first Harry anim after base anims. Make a constant?
        modelAnim->mapAnimInfos       = mapAnimInfos;

        modelAnim                     = &PLAYER_EXTRA().model.anim;
        modelAnim->baseAnimInfos      = baseAnimInfos;
        modelAnim->mapAnimInfos       = mapAnimInfos;
        modelAnim->mapAnimStatusStart = ANIM_STATUS(38, false);
    }

    if (arg0 <= 0)
    {
        var_t1          = 0;
        chara->field_44.field_0 = 0;
    }

    chara->field_44.field_2 = weaponAttack;
    if (!(modelAnim->status & (1 << 0)))
    {
        chara->field_44.field_0 = 0;
        chara->field_44.field_8 = 0;
    }
    else if (!var_t1 && arg0 > 0)
    {
        chara->field_44.field_0 = 1;
        chara->field_44.field_8 = 0;
    }

    chara->field_44.field_3  = 100;
    chara->field_44.field_C  = rotToTargetX;
    chara->field_44.field_E  = rotToTargetY;
    chara->field_44.field_18 = *attackPos;
    temp_a1         = func_8008A3E0(chara);
    ret             = NO_VALUE;

    if (chara == &PLAYER_CHARA())
    {
        count    = 6;
        var_v0_2 = 1;
    }
    else
    {
        count    = 1;
        var_v0_2 = NO_VALUE;
    }

    if (temp_a1 != 0)
    {
        var_a0 = var_v0_2;
        while (count > 0)
        {
            if (temp_a1 & var_a0)
            {
                ret = count;
                break;
            }

            var_a0 *= 2; // Or `<<= 1`.
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

u32 func_8008A270(s32 idx) // 0x8008A270
{
#if VERSION_EQUAL_OR_NEWER(USA)
    switch (idx)
    {
        case 2:
        case 5:
        case 12:
        case 15:
        case 22:
        case 25:
            if (g_SysWork.field_275C > Q12(256.0f))
            {
                return 0;
            }
            break;

        default:
            break;
    }
#else
    if (idx != 57 && idx != 58)
    {
        if (g_SysWork.field_275C > Q12(256.0f))
        {
            return 0;
        }
    }
#endif

    return g_Items_WeaponAttackInfos[idx].hitStartKeyframe;
}

u32 func_8008A2E0(s32 arg0) // 0x8008A2E0
{
    if (g_SysWork.field_275C > Q12(256.0f))
    {
        switch (arg0)
        {
            case 5:
            case 25:
                return 18;

            case 15:
                return 15;

            case 2:
            case 12:
            case 22:
                return 13;
        }
    }

    return g_Items_WeaponAttackInfos[arg0].hitKeyframeCount;
}

s32 func_8008A35C(s_AttackInfo* attackInfo, s32 arg1) // 0x8008A35C
{
    s32 var;
    s32 res;

    var = attackInfo->hitStartKeyframe + attackInfo->hitKeyframeCount;
    res = 0;
    if (arg1 >= attackInfo->hitStartKeyframe)
    {
        res = (arg1 < var);
    }

    return res;
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
        chara->field_44.field_0 = 0;
        func_8008A3E0(chara);
    }
}

s32 func_8008A3E0(s_SubCharacter* chara) // 0x8008A3E0
{
    s32                sp10;
    s32                sp14;
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
    s_SubCharacter_44* temp_v0_12;
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

    sp10 = chara->field_44.field_0;
    sp14 = (u8)chara->field_44.field_2;

    if (sp10 == 0)
    {
        chara->field_44.field_4  = 0;
        chara->field_44.field_8  = 0;
        chara->field_44.field_14 = 0;
        return 0;
    }

    if (sp10 == sp30)
    {
        chara->field_44.field_8  = 0;
        chara->field_44.field_14 = 0;
        return 0;
    }

    sp18 = 0;

    attackInfo = &g_Items_WeaponAttackInfos[sp14];

    temp_s1 = attackInfo->type;

    if (chara == &PLAYER_CHARA())
    {
        var_s0 = &((s_PlayerWork*)chara)->extra.model.anim;
    }
    else
    {
        var_s0 = &chara->model.anim;
    }

    anim  = Anim_InfoGet(var_s0);
    sp28  = var_s0->time;
    sp28 -= Q12(anim->startKeyframeIdx);

    var_s0_2 = func_8008A270(sp14);
    var_a0   = func_8008A2E0(sp14);

    i   = chara->field_44.field_14;
    sp2C     = Q12(var_s0_2 + var_a0);
    var_s0_2 = Q12(var_s0_2);

    if (sp28 < i)
    {
        sp10                   = 1;
        chara->field_44.field_0 = sp10;
    }

    if (sp10 == 1)
    {
        chara->field_44.field_4 = 0;
        sp10                   = 2;
    }

    if (i < var_s0_2)
    {
        i                  = var_s0_2;
        chara->field_44.field_14 = i;
    }
    else if (sp2C < i)
    {
        i                  = sp2C;
        chara->field_44.field_14 = i;
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

        sp20 = chara->field_44.field_C;
        sp24 = chara->field_44.field_E;

        if (temp_s1 == 3)
        {
            sp3C = chara->field_44.field_3;
            sp40 = chara->field_44.field_4;
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

                    chara->field_44.field_48[0].vx = Q12_MULT(sp54, temp_s1_4);
                    chara->field_44.field_48[0].vy = Q12_MULT(sp54, temp2);
                    chara->field_44.field_48[0].vz = Q12_MULT(sp54, temp_s0_9);

                    var_s6 += sp4C;

                    if (chara->model.charaId == Chara_Harry)
                    {
                        func_800892A4(5);
                    }

                    hasHit = Ray_GeomAndCharaTraceGet(&D_800C4728, &chara->field_44.field_18, &chara->field_44.field_48[0], chara);
                    ptr  = D_800C4728.character;
                    if (hasHit && ptr != NULL)
                    {
                        chara->field_44.field_48[1].vx = temp_s1_4;
                        chara->field_44.field_48[1].vy = temp2;
                        chara->field_44.field_48[1].vz = temp_s0_9;
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

            chara->field_44.field_14 = i;
            chara->field_44.field_4  = sp40;
            chara->field_44.field_3  = sp3C;
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

            temp_s0_10 = chara->field_44.field_12;
            temp_s1_5  = chara->field_44.field_10;

            sp60 = chara->field_44.field_24[2].vx;
            sp64 = chara->field_44.field_24[2].vy;
            sp68 = chara->field_44.field_24[2].vz;

            sp6C = chara->field_44.field_18.vx;
            sp70 = chara->field_44.field_18.vy;
            sp74 = chara->field_44.field_18.vz;

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

            chara->field_44.field_10 = chara->field_44.field_C;
            chara->field_44.field_12 = chara->field_44.field_E;

            for (var_s2 = 0, temp_s1_3 = 4; (temp_s1_3 > 0) && (var_s2 == 0); temp_s1_3--)
            {
                temp_v0_11                 = &D_800C4758[4 - temp_s1_3];
                chara->field_44.field_18.vx = temp_v0_11->vx;
                chara->field_44.field_18.vy = temp_v0_11->vy;

                temp_s0_14                 = &D_800C4748[4 - temp_s1_3];
                chara->field_44.field_18.vz = temp_v0_11->vz;

                func_8008B1DC(chara, temp_s0_14->vx, temp_s0_14->vy);

                if (func_8008BF84(chara, temp_s0_14->vx, attackInfo, sp5C) != 0)
                {
                    var_s2 = sp58;
                    sp18  |= var_s2;
                }
            }

            i     = sp28;
            temp_v0_12 = &chara->field_44;

            temp_v0_12->field_18.vx       = sp6C;
            chara->field_44.field_24[2].vx = sp6C;
            temp_v0_12->field_18.vy       = sp70;
            chara->field_44.field_24[2].vy = sp70;
            temp_v0_12->field_18.vz       = sp74;
            chara->field_44.field_24[2].vz = sp74;
        }

        if (sp18 != 0)
        {
            chara->field_44.field_0 = NO_VALUE;

            if (chara->model.charaId == Chara_Harry)
            {
                switch (sp14)
                {
                    case 32:
                    case 33:
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

                    case 34:
                        var_s0_2 = 5;
                        break;

                    case 35:
                        var_s0_2 = 4;
                        break;

                    case 2:
                    case 5:
                    case 6:
                    case 12:
                    case 15:
                    case 16:
                    case 22:
                    case 25:
                    case 26:
                        var_s0_2 = 7;
                        break;

                    case 8:
                    case 9:
                        var_s0_2 = NO_VALUE;
                        break;

                    default:
                        var_s0_2 = 6;
                        break;
                }

                if (var_s0_2 != NO_VALUE)
                {
                    func_8009151C(var_s0_2, chara->field_44.field_8 != 0, func_8007FD2C());
                }
            }
        }
        else
        {
            chara->field_44.field_0  = sp10;
            chara->field_44.field_14 = i;
        }
    }

    return chara->field_44.field_8;
}

void func_8008B15C(s_SubCharacter* chara) // 0x8008B15C
{
    s32 temp_s2;
    s32 temp_s3;
    s32 i;

    temp_s3 = chara->field_44.field_C;
    temp_s2 = chara->field_44.field_E;

    chara->field_44.field_10 = temp_s3;
    chara->field_44.field_12 = temp_s2;

    for (i = 1; i > 0; i--)
    {
        func_8008B1DC(chara, temp_s3, temp_s2);
        chara->field_44.field_24[2] = chara->field_44.field_18;
    }
}

void func_8008B1DC(s_SubCharacter* chara, q19_12 angle0, q19_12 angle1) // 0x8008B1DC
{
    s32                temp_s0;
    q19_12             sinAngle1;
    q19_12             cosAngle1;
    s32                temp_s3;
    s32                temp_v1_2;
    s32                i;
    s32                temp_s2;
    s32                idx46;
    s32                vx;
    s32                vy;
    s32                vz;
    s32                vx2;
    s32                vy2;
    s32                vz2;
    s32                vx3;
    s32                vy3;
    s32                vz3;
    s_AttackInfo*      attackInfo;
    s_SubCharacter_44* base;

    base  = &chara->field_44;
    idx46 = chara->field_44.field_2;

    for (i = 0; i >= 0; i--)
    {
        vx = base->field_48[i].vx;
        vy = base->field_48[i].vy;
        vz = base->field_48[i].vz;

        base->field_48[i + 1].vx = vx;
        base->field_48[i + 1].vy = vy;
        base->field_48[i + 1].vz = vz;

        vx2 = base->field_24[i].vx;
        vy2 = base->field_24[i].vy;
        vz2 = base->field_24[i].vz;

        base->field_24[i + 1].vx = vx2;
        base->field_24[i + 1].vy = vy2;
        base->field_24[i + 1].vz = vz2;
    }

    attackInfo = &g_Items_WeaponAttackInfos[idx46];

    temp_s2 = attackInfo->rangeMax;
    temp_s0 = attackInfo->rangeMin;

    vx3 = chara->field_44.field_18.vx;
    vy3 = chara->field_44.field_18.vy;
    vz3 = chara->field_44.field_18.vz;

    vx2 = vx3;
    vy2 = vy3;
    vz2 = vz3;

    cosAngle1 = Math_Cos(angle1);
    sinAngle1 = Math_Sin(angle1);

    temp_s3   = Q12_MULT(sinAngle1, Math_Sin(angle0));
    temp_v1_2 = Q12_MULT(sinAngle1, Math_Cos(angle0));

    temp_s2 -= temp_s0;

    vx2                          += Q12_MULT(temp_s0, temp_s3);
    chara->field_44.field_24[0].vx = vx2;

    vy2                          += Q12_MULT(temp_s0, cosAngle1);
    chara->field_44.field_24[0].vy = vy2;

    vz2                          += Q12_MULT(temp_s0, temp_v1_2);
    chara->field_44.field_24[0].vz = vz2;

    vx                            = Q12_MULT(temp_s2, temp_s3);
    chara->field_44.field_48[0].vx = vx;

    vy                            = Q12_MULT(temp_s2, cosAngle1);
    chara->field_44.field_48[0].vy = vy;

    vz                            = Q12_MULT(temp_s2, temp_v1_2);
    chara->field_44.field_48[0].vz = vz;
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

s32 func_8008B714(s_SubCharacter* attacker, s_SubCharacter* target, VECTOR3* attackPos, s32 arg3) // 0x8008B714
{
    s32           sp10;
    s32           sp14;
    s32           var_a3;
    u32           var_s0;
    q19_12        offsetX;
    q19_12        offsetY;
    q19_12        offsetZ;
    q20_12        damageAmount;
    s32           var_v1;
    u32           var_s7;
    s32           weaponAttack;
    s32           var_a2;
    s32           temp;
    s32           x0;
    s32           y0;
    s32           z0;
    s32           x1;
    s32           y1;
    s32           z1;
    s32           x2;
    s32           y2;
    s32           z2;
    s_AttackInfo* attackInfo;

    weaponAttack = (u8)attacker->field_44.field_2;
    attackInfo   = &g_Items_WeaponAttackInfos[weaponAttack];
    offsetY      = attackInfo->type;
    sp14         = attacker->field_44.field_8;
    offsetZ      = arg3;

    if (target == &PLAYER_CHARA())
    {
        sp10             = NO_VALUE;
        target->field_40 = attacker - g_SysWork.npcs; // Sets `field_40` to attacker index inside `g_SysWork.npcs`
    }
    else
    {
        sp10 = 1 << (target - g_SysWork.npcs);

        if (sp14 & sp10)
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
            switch (offsetY)
            {
                case 2:
                    func_800892A4(6);
                    break;

                case 1:
                    func_800892A4(7);
                    break;

                case 3:
                    break;
            }

            if (target->model.charaId == Chara_Padlock)
            {
                Sfx_WithFlagsPlay(Sfx_Unk1392, attackPos, Q8(0.5f), SfxFlag_None);
            }
            else
            {
                func_8008B664(attackPos, weaponAttack);
            }
        }
    }

    var_s0 = Q12(attackInfo->damage);

    switch (weaponAttack)
    {
        case WEAPON_ATTACK(AttackType_RockDrill, AttackInputType_Tap):
        case WEAPON_ATTACK(AttackType_Chainsaw,  AttackInputType_Tap):
        case WEAPON_ATTACK(AttackType_RockDrill, AttackInputType_Hold):
        case WEAPON_ATTACK(AttackType_Chainsaw,  AttackInputType_Hold):
        case WEAPON_ATTACK(AttackType_RockDrill, AttackInputType_Multitap):
        case WEAPON_ATTACK(AttackType_Chainsaw,  AttackInputType_Multitap):
            if (func_8008A35C(attackInfo, attacker->field_44.field_14) == 0)
            {
                var_s0 = 0;
            }

            if (g_SysWork.field_275C > Q12(256.0f))
            {
                var_s0 += Q12(200.0f);
            }
            break;

        case WEAPON_ATTACK(AttackType_HyperBlaster, AttackInputType_Tap):
            switch (Game_HyperBlasterBeamColorGet())
            {
                case 0:
                    break;

                case 1:
                    var_s0 += var_s0 * 2;
                    break;

                case 2:
                    var_s0 += Rng_RandQ12() * 50;
                    var_s0 *= 5;
                    break;
            }
            break;

        case WEAPON_ATTACK(AttackType_Kick,  AttackInputType_Tap):
        case WEAPON_ATTACK(AttackType_Stomp, AttackInputType_Tap):
            if (target->flags & CharaFlag_Unk2)
            {
                var_s0 = Q12(1000.0f);
            }
            break;
    }

    var_s7       = attackInfo->shock;
    damageAmount = Q12_MULT_PRECISE(var_s0, (Q12(4.5f) - Rng_RandQ12()) / 4);

    switch (weaponAttack)
    {
        case WEAPON_ATTACK(AttackType_Handgun,      AttackInputType_Tap):
        case WEAPON_ATTACK(AttackType_HuntingRifle, AttackInputType_Tap):
        case WEAPON_ATTACK(AttackType_Shotgun,      AttackInputType_Tap):
        case WEAPON_ATTACK(AttackType_HyperBlaster, AttackInputType_Tap):
        case NO_VALUE:
            offsetX = attacker->field_44.field_48[1].vx;
            offsetY   = attacker->field_44.field_48[1].vy;
            offsetZ   = attacker->field_44.field_48[1].vz;

            if (weaponAttack == WEAPON_ATTACK(AttackType_Shotgun, AttackInputType_Tap))
            {
                if (sp14 & sp10)
                {
                    var_s7 = 0;
                }

                offsetZ   = 0;
                offsetY   = 0;
                offsetX = 0;
            }
            break;

        default:
            if (target != &PLAYER_CHARA() && !(target->flags & CharaFlag_Hit))
            {
                damageAmount *= 4;
            }

            if (offsetY == 1)
            {
                damageAmount = Q12_MULT_PRECISE(damageAmount, offsetZ);
                var_s7 = Q12_MULT_PRECISE(var_s7, offsetZ);
            }

            if (weaponAttack == WEAPON_ATTACK(AttackType_Kick,  AttackInputType_Tap) ||
                weaponAttack == WEAPON_ATTACK(AttackType_Stomp, AttackInputType_Tap))
            {
                offsetX = attacker->field_44.field_48[0].vx;
                offsetY   = 0;
                offsetZ   = attacker->field_44.field_48[0].vz;
            }
            else
            {
                x0 = attacker->field_44.field_24[2].vx;
                y0 = attacker->field_44.field_24[2].vy;
                z0 = attacker->field_44.field_24[2].vz;

                x1 = attacker->field_44.field_48[2].vx;
                y1 = attacker->field_44.field_48[2].vy;
                z1 = attacker->field_44.field_48[2].vz;

                x2 = attacker->field_44.field_48[0].vx;
                y2 = attacker->field_44.field_48[0].vy;
                z2 = attacker->field_44.field_48[0].vz;

                x1 += x0;
                y1 += y0;
                z1 += z0;

                x0 = attacker->field_44.field_24[0].vx;
                y0 = attacker->field_44.field_24[0].vy;
                z0 = attacker->field_44.field_24[0].vz;

                x2 += x0;
                y2 += y0;
                z2 += z0;

                offsetX = x2 - x1;
                offsetY   = y2 - y1;
                offsetZ   = z2 - z1;
            }

            var_v1 = func_8008A058(func_80080540(offsetX, offsetY, offsetZ));
            if (var_v1 >= 64)
            {
                if (var_v1 >= Q12(4096.0f))
                {
                    var_v1 /= Q12(1.0f);
                    offsetX /= Q12(1.0f);
                    offsetY /= Q12(1.0f);
                    offsetZ /= Q12(1.0f);
                }

                var_v1  = Q12(4096.0f) / var_v1;
                offsetX = Q12_MULT_PRECISE(offsetX, var_v1);
                offsetY = Q12_MULT_PRECISE(offsetY, var_v1);
                offsetZ = Q12_MULT_PRECISE(offsetZ, var_v1);
            }
            break;
    }

    if (damageAmount != Q12(0.0f))
    {
        target->damage.amount += damageAmount;
    }

    if (var_s7 != 0)
    {
        temp                        = Q12_MULT_PRECISE(var_s7, offsetX);
        target->damage.position.vx += temp;
        temp                        = Q12_MULT_PRECISE(var_s7, offsetY);
        target->damage.position.vy += temp;
        temp                        = Q12_MULT_PRECISE(var_s7, offsetZ);
        target->damage.position.vz += temp;
    }

    target->attackReceived     = weaponAttack;
    sp14                      |= sp10;
    attacker->field_44.field_8 = sp14;

    if (damageAmount | var_s7)
    {
        var_a2 = attackInfo->impactSfxGroup;

        if (var_a2 > 0 && var_a2 < 8)
        {
            var_a3 = NO_VALUE;

            switch (target->model.charaId)
            {
                case Chara_Harry:
                    if (target->health >= Q12(0.0f))
                    {
                        var_a3 = 1;
                    }
                    break;

                case Chara_AirScreamer:
                case Chara_NightFlutter:
                    var_a3 = 2;
                    break;

                case Chara_Stalker:
                    var_a3 = 4;
                    break;

                case Chara_LarvalStalker:
                case Chara_GreyChild:
                case Chara_Mumbler:
                    var_a3 = 3;
                    break;

                case Chara_Creeper:
                    var_a3 = 5;
                    break;

                case Chara_Groaner:
                case Chara_Wormhead:
                    var_a3 = 6;
                    break;

                case Chara_SplitHead:
                    var_a3 = 7;
                    break;

                case Chara_HangedScratcher:
                    var_a3 = 8;
                    break;

                case Chara_Floatstinger:
                case Chara_Twinfeeler:
                    var_a3 = 10;
                    break;

                case Chara_Romper:
                    var_a3 = 9;
                    break;

                case Chara_PuppetNurse:
                case Chara_PuppetDoctor:
                    var_a3 = 11;
                    break;

                case Chara_Bloodsucker:
                    var_a3 = 12;
                    var_a2 = 1;
                    break;

                case Chara_Incubus:
                case Chara_Unknown23:
                case Chara_Incubator:
                    if (!(target->flags & CharaFlag_Hit))
                    {
                        if (weaponAttack == WEAPON_ATTACK(AttackType_Shotgun, AttackInputType_Tap))
                        {
                            var_a2 = 4;
                        }
                        else
                        {
                            var_a2 = 7;
                        }
                    }

                default:
                    var_a3 = 16;
                    break;

                case Chara_Padlock:
                    var_a3 = NO_VALUE;
                    break;
            }

            if (var_a3 >= 0)
            {
                func_8005F6B0(target, attackPos, var_a2, var_a3);
            }
        }
    }

    return sp10;
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

    countX = chara->field_44.field_24[0].vx;
    countY = chara->field_44.field_24[0].vy;
    countZ = chara->field_44.field_24[0].vz;

    sinAngle = Math_Sin(angle);
    cosAngle = Math_Cos(angle);

    D_800C4788[0].vx = Q12(0.0f);
    D_800C4788[0].vy = Q12(0.0f);
    D_800C4788[0].vz = Q12(0.0f);

    sp34   = chara->field_44.field_48[0].vx;
    var_fp = chara->field_44.field_48[0].vz;

    sp44 = 0;
    sp38 = 0;
    sp48 = 0;
    sp3C = 0;
    sp4C = 0;
    sp40 = 0;

    var_s7           = chara->field_44.field_48[0].vy;
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
        var_v0 = 6;
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

    chara->field_44.field_8 |= sp14;
    return sp14;
}
