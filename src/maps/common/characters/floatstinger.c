#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "maps/common/characters/floatstinger.h"

#define floatstingerProps floatstinger->properties.floatstinger

void Floatstinger_Update(s_SubCharacter* floatstinger, s_AnmHeader* anmHdr, GsCOORDINATE2* coords) // 0x800D16FC
{
    D_800DB928 = coords;

    if (floatstinger->model.controlState == 0)
    {
        Floatstinger_Init(floatstinger);
    }

    if (g_DeltaTime != Q12(0.0f))
    {
        func_800D1968(floatstinger);
        Floatstinger_ControlUpdate(floatstinger);
        func_800D35F0(floatstinger);
    }

    func_800D37E8(floatstinger, anmHdr);
    func_800D3AD4(floatstinger);

    floatstingerProps.field_EC = floatstinger->rotation.vy;
}

void Floatstinger_Init(s_SubCharacter* floatstinger) // 0x800D1790
{
    s32 i;

    floatstingerProps.flags_E8     = 0;
    floatstinger->model.anim.alpha = Q12(0.0f);

    // Set health.
    floatstinger->health = Q12(4000.0f);
    if (g_SavegamePtr->gameDifficulty == GameDifficulty_Hard)
    {
        floatstinger->health = Q12(6000.0f);
    }

    floatstinger->moveSpeed       = Q12(0.0f);
    floatstinger->fallSpeed       = Q12(0.0f);
    floatstinger->collision.state = CharaCollisionState_4;
    Chara_PropsClear(floatstinger);

    switch (floatstinger->model.stateStep)
    {
        case 3:
            floatstinger->model.controlState = FloatstingerControl_5;
            Chara_AnimSet(floatstinger, ANIM_STATUS(FloatstingerAnim_9, true), 191);

            floatstingerProps.field_106 = -0x90; // Could be `Q12_ANGLE(-12.7f)`?
            break;

        case 4:
            floatstinger->position.vx = Q12(-114.5f);
            floatstinger->position.vy = Q12(2.0f);
            floatstinger->position.vz = Q12(108.0f);

            floatstinger->model.controlState = FloatstingerControl_2;
            Chara_AnimSet(floatstinger, ANIM_STATUS(FloatstingerAnim_9, true), 191);
            floatstinger->rotation.vy = Q12_ANGLE(-90.0f);
            break;
    }

    floatstinger->model.stateStep = 0;
    ModelAnim_AnimInfoSet(&floatstinger->model.anim, FLOATSTINGER_ANIM_INFOS);
    Chara_DamageClear(floatstinger);

    D_800DB89C                 = 0;
    D_800DB898                 = 0;
    floatstinger->headingAngle = floatstinger->rotation.vy;

    for (i = 0; i < 15; i++)
    {
        D_800DB8A8[i] = 0;
    }

    floatstingerProps.field_EC = floatstinger->rotation.vy;
    floatstinger->flags       |= CharaFlag_NoRadioStatic | CharaFlag_Hit;

    D_800D7858 = 0;

    if (g_SavegamePtr->gameDifficulty == GameDifficulty_Hard)
    {
        D_800D785C                  = Q12(2.4f);
        floatstingerProps.flags_E8 |= FloatstingerFlag_0;
    }
    else if (g_SavegamePtr->gameDifficulty == GameDifficulty_Easy)
    {
        D_800D785C = Q12(4.0f);
    }
    else
    {
        D_800D785C = Q12(3.2f);
    }
}

void func_800D1968(s_SubCharacter* floatstinger) // 0x800D1968
{
    VECTOR3 sp10;
    u8      sp20;
    s32     var_a1;
    q19_12  newHealth;

    floatstingerProps.field_108 = MAX(floatstingerProps.field_108 -
                                                             Q12_MULT_PRECISE(g_DeltaTime, Q12(150.0f)),
                                                             Q12(0.0f));

    if (floatstinger->damage.amount > Q12(0.0f))
    {
        if (g_SavegamePtr->gameDifficulty == GameDifficulty_Easy)
        {
            var_a1 = Q12(500.0f);
        }
        else
        {
            var_a1 = Q12(300.0f);
        }

        floatstingerProps.field_108 += floatstinger->damage.amount;

        if (var_a1 < floatstingerProps.field_108)
        {
            floatstinger->damage.amount -= floatstingerProps.field_108 - var_a1;
            floatstingerProps.field_108  = var_a1;
            if (floatstinger->damage.amount < Q12(0.0f))
            {
                floatstinger->damage.amount = Q12(0.0f);
            }
        }

        Player_DisableDamage(&sp20, false);

        newHealth = floatstinger->health - floatstinger->damage.amount;
        if (sp20 != 0)
        {
            if (newHealth <= Q12(0.0f))
            {
                newHealth = 1;
            }
        }
        else if (newHealth < Q12(0.0f))
        {
            newHealth = Q12(0.0f);
        }
        floatstinger->health = newHealth;

        floatstingerProps.field_F8 += FP_FROM(floatstinger->damage.amount, Q12_SHIFT);
        floatstingerProps.field_FA += FP_FROM(floatstinger->damage.amount, Q12_SHIFT);

        Sfx_WithFlagsPlay(Sfx_Unk1570, &sp10, Q8(0.5f), SfxFlag_None);
        
 // @bugfix USA onwards always runs this block if `health == 0`, in JAP0 controlState/anim.status also had to pass the conditions.
#if VERSION_EQUAL_OR_NEWER(USA)
        if (floatstinger->health == Q12(0.0f) ||
            ((floatstingerProps.field_F8 > 1000 || floatstingerProps.field_FA > 1200) &&
             floatstinger->model.controlState == FloatstingerControl_2 &&
             floatstinger->model.anim.status == ANIM_STATUS(FloatstingerAnim_9, true)))
#else
        if ((floatstinger->health == Q12(0.0f) || floatstingerProps.field_F8 > 1000 || floatstingerProps.field_FA > 1200) &&
            floatstinger->model.controlState == FloatstingerControl_2 &&
            floatstinger->model.anim.status == ANIM_STATUS(FloatstingerAnim_9, true))
#endif
        {
            floatstingerProps.field_F8       = 0;
            floatstinger->model.controlState = FloatstingerControl_3;

            if (floatstingerProps.field_FA > 1200)
            {
                floatstingerProps.field_FA  = 0;
                floatstingerProps.flags_E8 |= FloatstingerFlag_1;
            }
        }
    }

    if (floatstingerProps.field_FA > 1200 && floatstinger->model.controlState == 2 &&
        floatstinger->model.anim.status == ANIM_STATUS(FloatstingerAnim_9, true))
    {
        floatstingerProps.field_F8       = 0;
        floatstingerProps.field_FA       = 0;
        floatstinger->model.controlState = FloatstingerControl_3;
        floatstingerProps.flags_E8      |= FloatstingerFlag_1;
    }

    Chara_DamageClear(floatstinger);

    Player_DisableDamage(&sp20, floatstinger->health == Q12(0.0f));
}

void Floatstinger_ControlUpdate(s_SubCharacter* floatstinger) // 0x800D1B98
{
    if (floatstinger->model.anim.status != ANIM_STATUS(FloatstingerAnim_1, true))
    {
        floatstinger->field_44.field_0 = 0;
        floatstingerProps.flags_E8    &= ~FloatstingerFlag_2;
    }

    // Handle control state.
    if (D_800D7A04[floatstinger->model.controlState] != NULL)
    {
        D_800D7A04[floatstinger->model.controlState]();
    }
}

void func_800D1BF8(s_SubCharacter* floatstinger) // 0x800D1BF8
{
    typedef struct
    {
        MATRIX field_0;
        VECTOR field_20[2]; // Q23.8
    } s_func_800D1BF8;

    VECTOR3          sp20[3];
    VECTOR3          sp48;
    VECTOR3          sp58;
    VECTOR3          sp68;
    s16              temp_v0_4;
    s32              temp_v0_14;
    s32              var_v1;
    s32              temp_v1_12;
    q19_12           sinCosAngle;
    s32              temp_a1;
    s32              temp_a2;
    s32              temp_a2_2;
    s32              temp_s0_3;
    s32              temp_s0_4;
    s16              temp_s1;
    s32              temp_s3_2;
    s32              i;
    s32              var_s5;
    q3_12            angle;
    s32              temp2;
    s32              temp;
    s16              temp3;
    s_func_800D1BF8* ptr;

    if (PLAYER_CHARA().position.vx > Q12(-120.0f) && PLAYER_CHARA().position.vx < Q12(-115.0f) &&
        (PLAYER_CHARA().position.vz > Q12(96.0f) && PLAYER_CHARA().position.vz < Q12(103.0f)))
    {
        var_s5     = 0;
        temp_v0_14 = Math_Vector2MagCalcSafeQ6(PLAYER_CHARA().position.vx + Q12(117.5f),
                                               PLAYER_CHARA().position.vz - Q12(100.0f));

        angle = ratan2(PLAYER_CHARA().position.vx + Q12(117.5f),
                       PLAYER_CHARA().position.vz - Q12(100.0f));

        sinCosAngle = Math_Sin(angle);
        sp58.vx     = (floatstinger->model.anim.status == ANIM_STATUS(FloatstingerAnim_1, true)) ? (Q12_MULT(temp_v0_14 + Q12(0.6f), sinCosAngle) - Q12(117.5f)) : 
                                                                                                   (Q12_MULT(temp_v0_14 + Q12(1.2f), sinCosAngle) - Q12(117.5f));
        sp58.vy     = Q12(-1.0f);

        sinCosAngle = Math_Cos(angle);
        sp58.vz     = (floatstinger->model.anim.status == ANIM_STATUS(FloatstingerAnim_1, true)) ? (Q12_MULT(temp_v0_14 + Q12(0.6f), sinCosAngle) + Q12(100.0f)) : 
                                                                                                   (Q12_MULT(temp_v0_14 + Q12(1.2f), sinCosAngle) + Q12(100.0f));
    }
    else if (PLAYER_CHARA().position.vx > Q12(-121.0f) && PLAYER_CHARA().position.vx < Q12(-120.0f) &&
             (PLAYER_CHARA().position.vz > Q12(96.0f) && PLAYER_CHARA().position.vz < Q12(103.0f)))
    {
        var_s5  = 1;
        sp58.vx = PLAYER_CHARA().position.vx - Q12(2.5f);
        sp58.vz = PLAYER_CHARA().position.vz;
        sp58.vy = PLAYER_CHARA().position.vy + Q12(0.8f);
    }
    else
    {
        var_s5 = 2;
        angle  = ratan2(floatstinger->position.vx - PLAYER_CHARA().position.vx,
                        floatstinger->position.vz - PLAYER_CHARA().position.vz);

        sinCosAngle = Math_Sin(angle);
        temp_a1     = PLAYER_CHARA().position.vx;

        sp48.vx = (floatstinger->model.anim.status == ANIM_STATUS(FloatstingerAnim_1, true)) ? (temp_a1 + Q12_MULT(sinCosAngle * 39, 63)) :
                                                                                               (temp_a1 + Q12_MULT(sinCosAngle, Q12(1.2f)));

        sp48.vy = Q12(2.0f);

        temp2   = Math_Cos(angle);
        temp_a2 = PLAYER_CHARA().position.vz;

        sp48.vz = (floatstinger->model.anim.status == ANIM_STATUS(FloatstingerAnim_1, true)) ? (temp_a2 + Q12_MULT(temp2 * 39, 63)) :
                                                                                               (temp_a2 + Q12_MULT(temp2, Q12(1.2f)));

        func_800D4458(floatstinger, &sp48);

        sp58.vx = sp48.vx + D_800DB898;
        sp58.vy = sp48.vy;
        sp58.vz = sp48.vz + D_800DB89C;
    }

    temp_a2_2 = ratan2(PLAYER_CHARA().position.vx - sp58.vx,
                       PLAYER_CHARA().position.vz - sp58.vz);

    if (floatstinger->model.anim.status == ANIM_STATUS(FloatstingerAnim_9, true))
    {
        func_800D4A3C(floatstinger, &sp58, temp_a2_2);

        floatstingerProps.field_100 -= g_DeltaTime;
        if (floatstingerProps.field_100 < Q12(0.0f))
        {
            floatstingerProps.field_100 = Q12(0.0f);
        }

        if (floatstingerProps.flags_E8 & FloatstingerFlag_3)
        {
            floatstingerProps.field_FC = MAX(0, floatstingerProps.field_FC - g_DeltaTime);

            if (!floatstingerProps.field_FC)
            {
                floatstingerProps.flags_E8 &= ~FloatstingerFlag_3;
            }
        }

        angle = ratan2(PLAYER_CHARA().position.vx - floatstinger->position.vx,
                       PLAYER_CHARA().position.vz - floatstinger->position.vz);

        if (ABS(PLAYER_CHARA().position.vy - floatstinger->position.vy) < Q12(0.05f) &&
            Math_Vector2MagCalcSafeQ6(PLAYER_CHARA().position.vx - floatstinger->position.vx,
                                      PLAYER_CHARA().position.vz - floatstinger->position.vz) < Q12(1.5f) &&
            ((var_s5 == 0 && Rng_GenerateUInt(0, 3)) || (var_s5 == 2 && Rng_GenerateUInt(0, 3)))) // 1 in 4 chance.
        {
            if (PLAYER_CHARA().health > Q12(0.0f) &&
                ABS(Math_AngleNormalizeSigned(angle - floatstinger->headingAngle)) < Q12_ANGLE(90.0f) &&
                ABS(Math_AngleNormalizeSigned(angle - floatstinger->rotation.vy)) < Q12_ANGLE(30.0f) &&
                !(PLAYER_CHARA().flags & (1 << 3)) &&
                !Rng_GenerateUInt(0, 7)) // 1 in 8 chance.
            {
                floatstinger->model.anim.status = ANIM_STATUS(FloatstingerAnim_1, false);
                floatstingerProps.flags_E8     &= ~FloatstingerFlag_4;
            }
        }
        else if (!(floatstingerProps.flags_E8 & FloatstingerFlag_3) &&
                 Math_Vector2MagCalcSafeQ6(PLAYER_CHARA().position.vx - floatstinger->position.vx,
                                           PLAYER_CHARA().position.vz - floatstinger->position.vz) < Q12(6.5f))
        {
            if (ABS(Math_AngleNormalizeSigned(angle - floatstinger->headingAngle)) < Q12_ANGLE(90.0f) &&
                ABS(Math_AngleNormalizeSigned(angle - floatstinger->rotation.vy)) < Q12_ANGLE(15.0f))
            {
                var_v1 = ABS(floatstinger->position.vy);

                if (var_v1 < Q12(8.0f) && !(PLAYER_CHARA().flags & (1 << 3)) &&
                    !Rng_GenerateUInt(0, 3)) // 1 in 4 chance.
                {
                    floatstinger->model.anim.status = ANIM_STATUS(FloatstingerAnim_12, false);
                }
            }
        }
    }
    else if (floatstinger->model.anim.status == ANIM_STATUS(FloatstingerAnim_1, true))
    {
        if (FP_FROM(floatstinger->model.anim.time, Q12_SHIFT) > 0 &&
            FP_FROM(floatstinger->model.anim.time, Q12_SHIFT) < 18)
        {
            func_800D4A3C(floatstinger, &sp58, temp_a2_2);
            floatstingerProps.field_104 = false;
        }
        else if (FP_FROM(floatstinger->model.anim.time, Q12_SHIFT) > 17 &&
                 FP_FROM(floatstinger->model.anim.time, Q12_SHIFT) < 25)
        {
            floatstinger->moveSpeed = Q12(0.0f);
            if (!(floatstingerProps.flags_E8 & FloatstingerFlag_2))
            {
                floatstinger->field_44.field_0++;
                floatstingerProps.flags_E8 |= FloatstingerFlag_2;
            }

            for (i = 0; i < 2; i++)
            {
                ptr = PSX_SCRATCH;

                Vw_CoordHierarchyMatrixCompute(&D_800DB928[5], &ptr->field_0);
                gte_SetRotMatrix(&ptr->field_0);
                gte_SetTransMatrix(&ptr->field_0);
                gte_ldv0(&D_800D7A20[i]);
                gte_rt();
                gte_stlvnl(&ptr->field_20[i]);

                sp20[i].vx = Q8_TO_Q12(ptr->field_20[i].vx);
                sp20[i].vy = Q8_TO_Q12(ptr->field_20[i].vy);
                sp20[i].vz = Q8_TO_Q12(ptr->field_20[i].vz);
            }

            if (!floatstingerProps.field_104)
            {
                sp68.vx = floatstinger->position.vx;
                sp68.vy = floatstinger->position.vy - Q12(1.0f);
                sp68.vz = floatstinger->position.vz;

                Sfx_WithFlagsPlay(Sfx_Unk1571, &sp68, Q8(0.5f), SfxFlag_None);
                floatstingerProps.field_104++;
            }

            func_8008A0E4(1, WEAPON_ATTACK(AttackType_Floatstinger, AttackInputType_Tap), floatstinger, &sp20[0], &PLAYER_CHARA(), ratan2(sp20[1].vx - sp20[0].vx, sp20[1].vz - sp20[0].vz),
                          ratan2(Math_Vector2MagCalcSafeQ6(sp20[1].vx - sp20[0].vx, sp20[1].vz - sp20[0].vz), sp20[1].vy - sp20[0].vy));

            if (g_SavegamePtr->gameDifficulty != 1 && floatstingerProps.field_100 == 0)
            {
                floatstingerProps.flags_E8 &= ~FloatstingerFlag_0;
            }
        }
    }
    else if (floatstinger->model.anim.status == ANIM_STATUS(FloatstingerAnim_12, true))
    {
        if (FP_FROM(floatstinger->model.anim.time, Q12_SHIFT) > 0 &&
            FP_FROM(floatstinger->model.anim.time, Q12_SHIFT) < 18)
        {
            func_800D4A3C(floatstinger, &sp58, temp_a2_2);
        }
        else
        {
            floatstinger->moveSpeed = Q12(0.0f);

            if (!(floatstingerProps.flags_E8 & FloatstingerFlag_3))
            {
                ptr = PSX_SCRATCH;
                Vw_CoordHierarchyMatrixCompute(&D_800DB928[9], &ptr->field_0);
                gte_SetRotMatrix(&ptr->field_0);
                gte_SetTransMatrix(&ptr->field_0);
                gte_ldv0(&D_800D7A30);
                gte_rt();
                gte_stlvnl(&ptr->field_20[0]);

                sp20[0].vx = Q8_TO_Q12(ptr->field_20[0].vx);
                sp20[0].vy = Q8_TO_Q12(ptr->field_20[0].vy);
                sp20[0].vz = Q8_TO_Q12(ptr->field_20[0].vz);

                temp_s1 = ratan2(PLAYER_CHARA().position.vx - D_800DB8C8[7].vx,
                                 PLAYER_CHARA().position.vz - D_800DB8C8[7].vz);

                temp_s0_3 = SquareRoot0(SQUARE((D_800DB8C8[7].vx - PLAYER_CHARA().position.vx) >> 6) +
                                        SQUARE((D_800DB8C8[7].vz - PLAYER_CHARA().position.vz) >> 6));

                temp       = Rng_GenerateUInt(Q12(1.0f), Q12(2.0f) - 1);
                temp_s0_3  = temp_s0_3 << 6;
                temp_v0_14 = Q12_MULT_PRECISE(temp_s0_3, temp);

                sp20[1].vx = PLAYER_CHARA().position.vx + PLAYER_CHARA().collision.shapeOffsets.box.vx +
                             Q12_MULT(temp_v0_14, Math_Sin(temp_s1));
                sp20[1].vz = PLAYER_CHARA().position.vz + PLAYER_CHARA().collision.shapeOffsets.box.vz +
                             Q12_MULT(temp_v0_14, Math_Cos(temp_s1));

                temp_v0_4 = Q12_MULT_PRECISE(PLAYER_CHARA().position.vy - D_800DB8C8[7].vy, Rng_GenerateUInt(Q12(1.0f), Q12(2.0f) - 1));

                temp_a2_2 = PLAYER_CHARA().collision.box.offsetY;

                sp20[1].vy = MAX(PLAYER_CHARA().position.vy + temp_v0_4, Q12(-1.0f)) <= Q12(2.0f) ? (temp_a2_2 + MAX(PLAYER_CHARA().position.vy + temp_v0_4, Q12(-1.0f))) : 
                                                                                                                 (temp_a2_2 + Q12(2.0f));

                sp68.vx = floatstinger->position.vx;
                sp68.vy = floatstinger->position.vy - Q12(3.5f);
                sp68.vz = floatstinger->position.vz;

                Sfx_WithFlagsPlay(Sfx_Unk1572, &sp68, Q8(0.5f), SfxFlag_None);
                sharedFunc_800CB0A4_4_s03(&sp20[0], &sp20[1]);

                if (floatstingerProps.flags_E8 & FloatstingerFlag_0)
                {
                    for (i = 0; i < 2; i++)
                    {
                        temp_s3_2 = Rng_GenerateUInt(0, Q12_ANGLE(180.0f) - 1);
                        angle     = Rng_GenerateUInt(0, Q12_ANGLE(360.0f) - 1);
                        temp_s0_4 = Math_Sin(angle);

                        sp20[2].vx = sp20[1].vx + Q12_MULT(temp_s3_2, temp_s0_4) +
                                     Math_Sin(floatstinger->rotation.vy + ((i != 0) ? Q12_ANGLE(90.0f) : Q12_ANGLE(-90.0f)));

                        sinCosAngle = angle;
                        temp_s0_4   = Math_Cos(sinCosAngle);

                        sp20[2].vz = sp20[1].vz + Q12_MULT(temp_s3_2, temp_s0_4) +
                                     Math_Cos(floatstinger->rotation.vy + ((i != 0) ? Q12_ANGLE(90.0f) : Q12_ANGLE(-90.0f)));

                        temp3      = Rng_GenerateUInt(Q12(-0.25f), Q12(0.25f) - 1);
                        temp_v1_12 = sp20[1].vy + temp3;

                        temp2 = PLAYER_CHARA().collision.box.offsetY;

                        sp20[2].vy = PLAYER_CHARA().collision.box.offsetY < MAX(temp_v1_12, temp2 - Q12(3.0f)) ?
                                                                                         PLAYER_CHARA().collision.box.offsetY :
                                                                                         MAX(temp_v1_12, PLAYER_CHARA().collision.box.offsetY - Q12(3.0f));

                        sharedFunc_800CB0A4_4_s03(&sp20[0], &sp20[2]);
                    }
                }

                floatstingerProps.field_FC  = Q12_MULT_PRECISE(D_800D785C, Rng_GenerateUInt(Q12(0.75f), Q12_CLAMPED(1.0f)));
                floatstingerProps.flags_E8 |= FloatstingerFlag_3;

                if (g_SavegamePtr->gameDifficulty != GameDifficulty_Hard && floatstingerProps.field_100 == 0)
                {
                    floatstingerProps.flags_E8 &= ~FloatstingerFlag_0;
                }
            }
        }
    }

    for (i = 6; i >= 0; i--)
    {
        D_800DB8C8[i + 1].vx = D_800DB8C8[i].vx;
        D_800DB8C8[i + 1].vy = D_800DB8C8[i].vy;
        D_800DB8C8[i + 1].vz = D_800DB8C8[i].vz;
    }

    D_800DB8C8[0].vx = PLAYER_CHARA().position.vx;
    D_800DB8C8[0].vy = PLAYER_CHARA().position.vy;
    D_800DB8C8[0].vz = PLAYER_CHARA().position.vz;
}

void func_800D2B90(s_SubCharacter* floatstinger) // 0x800D2B90
{
    s32    temp_a0_6;
    q19_12 var_a1;
    s32    temp_s0;
    q19_12 temp_v0;
    s32    i;
    s32    temp_v0_7;

    if (floatstinger->health == Q12(0.0f))
    {
        Savegame_EventFlagSet(EventFlag_350);
    }

    D_800D7860 = Q12(13.0f);
    var_a1     = (floatstingerProps.flags_E8 & FloatstingerFlag_1) ? Q12(0.6f) : Q12(0.4f);
    temp_v0    = D_800D7858;

    if (((floatstingerProps.flags_E8 & FloatstingerFlag_1) && (var_a1 + Q12(1.0f)) < temp_v0) ||
        (!(floatstingerProps.flags_E8 & FloatstingerFlag_1) && (var_a1 + Q12(0.7f)) < temp_v0))
    {
        D_800D7858 = 0;
        for (i = 14; i >= 0; i--)
        {
            D_800DB8A8[i] = 0;
        }

        if (floatstingerProps.flags_E8 & FloatstingerFlag_1)
        {
            floatstingerProps.field_100 = Rng_GenerateInt(Q12(24.0f), Q12(40.0f) - 1);
            floatstingerProps.flags_E8 &= ~FloatstingerFlag_1;
            floatstingerProps.flags_E8 |= FloatstingerFlag_0;
        }

        if (floatstinger->health != Q12(0.0f))
        {
            floatstinger->model.controlState = FloatstingerControl_2;
        }
        else
        {
            floatstinger->model.controlState = FloatstingerControl_6;
        }
    }
    else
    {
        D_800D7858 += g_DeltaTime;

        floatstinger->moveSpeed = CLAMP_LOW(floatstinger->moveSpeed - Q12_MULT_PRECISE(g_DeltaTime, (floatstingerProps.flags_E8 & FloatstingerFlag_0) ? Q12(2.5f) : Q12(1.5f)), Q12(0.0f));

        if (floatstinger->fallSpeed > Q12(0.0f))
        {
            floatstinger->fallSpeed = CLAMP_LOW(floatstinger->fallSpeed - Q12_MULT_PRECISE(g_DeltaTime, (floatstingerProps.flags_E8 & FloatstingerFlag_0) ? Q12(4.5f) : Q12(1.8f)), Q12(0.0f));
        }
        else
        {
            floatstinger->fallSpeed = MIN(floatstinger->fallSpeed - Q12_MULT_PRECISE(g_DeltaTime, (floatstingerProps.flags_E8 & FloatstingerFlag_0) ? Q12(-3.0f) : Q12(-0.8f)), Q12(0.0f));
        }

        temp_v0 = D_800D7858;

        if (((floatstingerProps.flags_E8 & FloatstingerFlag_1) && temp_v0 < Q12(0.6f)) ||
            (!(floatstingerProps.flags_E8 & FloatstingerFlag_1) && temp_v0 < Q12(0.4f)))
        {
            for (i = 0; i < (temp_v0 = 15); i++) // @hack
            {
                temp_s0 = D_800D7A38[i] * D_800D7858;
                var_a1  = D_800D7858 << 10;

                D_800DB8A8[i] = (((floatstingerProps.flags_E8 & FloatstingerFlag_1) ? (temp_s0 / 2457) : (temp_s0 / 1638)) +
                                 Q12_MULT(D_800D7A38[i], Q12(1.0f) - Math_Cos((floatstingerProps.flags_E8 & FloatstingerFlag_1) ? (var_a1 / 2457) : (var_a1 / 1638)))) >> 1;
            }
        }
        else
        {
            for (i = 0; i < (temp_v0 = 15); i++) // @hack
            {
                temp_s0 = D_800D7A38[i] * ((floatstingerProps.flags_E8 & FloatstingerFlag_1) ? (D_800D7858 - Q12(0.6f)) : (D_800D7858 - Q12(0.4f)));

                var_a1        = ((floatstingerProps.flags_E8 & FloatstingerFlag_1) ? (D_800D7858 - Q12(0.6f)) : (D_800D7858 - Q12(0.4f))) << 10;
                var_a1        = Q12_MULT(D_800D7A38[i], Q12(1.0f) - Math_Cos((floatstingerProps.flags_E8 & FloatstingerFlag_1) ? (var_a1 / Q12(1.0f)) : (var_a1 / 2867)));
                temp_v0_7     = D_800D7A38[i];
                temp_a0_6     = (((floatstingerProps.flags_E8 & FloatstingerFlag_1) ? (temp_s0 / Q12(1.0f)) : (temp_s0 / 2867)) + var_a1) >> 1;
                D_800DB8A8[i] = temp_v0_7 - temp_a0_6;
            }
        }
    }
}

void func_800D341C(void) {}

void func_800D3424(s_SubCharacter* floatstinger) // 0x800D3424
{
    VECTOR3 sp10;

    switch (floatstinger->model.stateStep)
    {
        case 0:
            if (Savegame_EventFlagGet(EventFlag_349))
            {
                floatstinger->position.vx        = Q12(-114.5f);
                floatstinger->position.vy        = Q12(2.0f);
                floatstinger->position.vz        = Q12(108.0f);
                floatstinger->model.stateStep    = 4;
                floatstinger->model.controlState = FloatstingerControl_None;
                floatstingerProps.field_106      = 64;
            }
            else
            {
                floatstinger->position.vx       = Q12(-113.5f);
                floatstinger->position.vy       = Q12(8.0f);
                floatstinger->position.vz       = Q12(108.0f);
                floatstinger->rotation.vy       = Q12_ANGLE(-90.0f);
                floatstinger->model.anim.flags &= ~(1 << 1);
            }
            break;

        case 2:
            floatstinger->model.controlState = FloatstingerControl_None;
            floatstinger->model.stateStep    = 4;

        case 1:
            floatstinger->model.anim.flags |= 1 << 1;
            sp10.vx                         = floatstinger->position.vx;
            sp10.vy                         = Q12(2.0f);
            sp10.vz                         = floatstinger->position.vz;
            func_800D4A3C(floatstinger, &sp10, Q12_ANGLE(-90.0f));
            break;
    }

    floatstingerProps.field_106++;
    if (floatstingerProps.field_106 > 64)
    {
        floatstingerProps.field_106 = 64;
    }
}

void func_800D3564(s_SubCharacter* floatstinger) // 0x800D3564
{
    VECTOR3 sp10;

    D_800D7860                 = Q12(13.0f);
    floatstingerProps.field_EE = 0;
    floatstinger->moveSpeed    = 0;

    if (ANIM_STATUS_IDX_GET(floatstinger->model.anim.status) != FloatstingerAnim_5 && (s32)g_SavegamePtr->eventFlags[10] < 0)
    {
        floatstinger->model.anim.status = ANIM_STATUS(FloatstingerAnim_5, false);
        sp10.vx                         = floatstinger->position.vx;
        sp10.vy                         = floatstinger->position.vy - Q12(3.5f);
        sp10.vz                         = floatstinger->position.vz;

        Sfx_WithFlagsPlay(Sfx_Unk1573, &sp10, Q8(0.5f), SfxFlag_None);
    }
}

void func_800D35F0(s_SubCharacter* floatstinger) // 0x800D35F0
{
    s_CollisionResult sp10;
    VECTOR3           sp30;
    q3_12             headingAngle;
    s32               temp_s0;
    s32               temp_s2;
    s32               temp_s3;
    q19_12            offsetY;

    headingAngle = floatstinger->headingAngle;
    temp_s0      = Q12_MULT_PRECISE(g_DeltaTime, floatstinger->moveSpeed);
    temp_s2      = (temp_s0 <= Q12(-8.0f) || temp_s0 >= Q12(8.0f)) * 4;
    temp_s3      = temp_s2 >> 1;

    sp30.vx = Q12_MULT_PRECISE(temp_s0 >> temp_s3, Math_Sin(headingAngle) >> temp_s3) << temp_s2;
    sp30.vz = Q12_MULT_PRECISE(temp_s0 >> temp_s3, Math_Cos(headingAngle) >> temp_s3) << temp_s2;
    sp30.vy = Q12_MULT_PRECISE(g_DeltaTime, floatstinger->fallSpeed);

    Collision_WallDetect(&sp10, &sp30, floatstinger);

    offsetY                    = sp10.offset.vy;
    floatstinger->position.vx += sp10.offset.vx;
    floatstinger->position.vz += sp10.offset.vz;

    func_800D4458(floatstinger, &floatstinger->position);

    sp30.vy = Q12(0.0f);
    sp30.vx = D_800DB898;
    sp30.vz = D_800DB89C;

    Collision_WallDetect(&sp10, &sp30, floatstinger);
    floatstinger->position.vx += sp10.offset.vx;
    floatstinger->position.vz += sp10.offset.vz;

    sp30.vx = floatstinger->position.vx;
    sp30.vy = floatstinger->position.vy + offsetY;
    sp30.vz = floatstinger->position.vz;

    if (!func_800D4458(floatstinger, &sp30))
    {
        floatstinger->position.vy += offsetY;
    }
}

void func_800D37E8(s_SubCharacter* floatstinger, s_AnmHeader* anmHdr) // 0x800D37E8
{
    typedef struct
    {
        MATRIX  field_0;
        SVECTOR field_20;
    } s_func_800D37E8;

    VECTOR3          vec;
    s32              i;
    s_AnimInfo*      anim;
    s_func_800D37E8* ptr;
    q19_12*          time;

    switch (floatstinger->model.anim.status)
    {
        case ANIM_STATUS(FloatstingerAnim_1, true):
            if (FP_FROM(floatstinger->model.anim.time, Q12_SHIFT) > 0 &&
                FP_FROM(floatstinger->model.anim.time, Q12_SHIFT) < 4)
            {
                if (!(floatstingerProps.flags_E8 & FloatstingerFlag_4))
                {
                    vec.vx = floatstinger->position.vx;
                    vec.vy = floatstinger->position.vy - Q12(4.0f);
                    vec.vz = floatstinger->position.vz;

                    Sfx_WithFlagsPlay(Sfx_Unk1569, &vec, 0x20, SfxFlag_None);
                    floatstingerProps.flags_E8 |= FloatstingerFlag_4;
                }
            }
            else
            {
                floatstingerProps.flags_E8 &= ~FloatstingerFlag_4;
            }

            if (g_SavegamePtr->gameDifficulty == GameDifficulty_Easy)
            {
                FLOATSTINGER_ANIM_INFOS[floatstinger->model.anim.status].duration.constant = Q12(26.0f);
            }
            else if (g_SavegamePtr->gameDifficulty == GameDifficulty_Hard)
            {
                FLOATSTINGER_ANIM_INFOS[floatstinger->model.anim.status].duration.constant = Q12(38.0f);
            }
            else
            {
                FLOATSTINGER_ANIM_INFOS[floatstinger->model.anim.status].duration.constant = Q12(32.0f);
            }
            break;

        case ANIM_STATUS(FloatstingerAnim_9, true):
            D_800D799C = D_800D7860;

            time = &floatstinger->model.anim.time;

            if ((FP_FROM(*time, Q12_SHIFT) > 190 && FP_FROM(*time, Q12_SHIFT) < 194) ||
                (FP_FROM(*time, Q12_SHIFT) > 206 && FP_FROM(*time, Q12_SHIFT) < 210))
            {
                if (!(floatstingerProps.flags_E8 & FloatstingerFlag_4))
                {
                    if (floatstingerProps.field_106 > 0)
                    {
                        vec.vx = floatstinger->position.vx;
                        vec.vy = floatstinger->position.vy - Q12(4.0f);
                        vec.vz = floatstinger->position.vz;

                        Sfx_WithFlagsPlay(Sfx_Unk1568, &vec, floatstingerProps.field_106 >> 1, SfxFlag_None);
                        floatstingerProps.flags_E8 |= FloatstingerFlag_4;
                    }
                }
            }
            else
            {
                floatstingerProps.flags_E8 &= ~FloatstingerFlag_4;
            }
            break;
    }

    Math_MatrixTransform(&floatstinger->position, (SVECTOR*)&floatstinger->rotation, D_800DB928);

    anim = &FLOATSTINGER_ANIM_INFOS[floatstinger->model.anim.status];
    anim->playbackFunc(&floatstinger->model, anmHdr, D_800DB928, anim);

    ptr = PSX_SCRATCH;

    for (i = 0; i < 15; i++)
    {
        if (i < 3)
        {
            Math_SetSVectorFastSum(&ptr->field_20, 0, 0, D_800DB8A8[i]);
        }
        else if (i & 1)
        {
            Math_SetSVectorFastSum(&ptr->field_20, D_800DB8A8[i], 0, 0);
        }
        else
        {
            Math_SetSVectorFastSum(&ptr->field_20, 0, D_800DB8A8[i], 0);
        }

        Math_RotMatrixZxyNegGte(&ptr->field_20, &ptr->field_0);
        MulMatrix(&D_800DB928[D_800D7848[i]].coord, &ptr->field_0);
    }
}

void func_800D3AD4(s_SubCharacter* floatstinger) // 0x800D3AD4
{
    typedef struct
    {
        MATRIX  field_0;
        VECTOR  field_20[3];
        VECTOR3 field_50[3];
    } s_func_800D3AD4;

    s32              i;
    s16              var_v1;
    s_func_800D3AD4* ptr;

    ptr = PSX_SCRATCH;

    for (i = 0; i < 3; i++)
    {
        Vw_CoordHierarchyMatrixCompute(&D_800DB928[D_800D7A58[i]], &ptr->field_0);

        gte_SetRotMatrix(&ptr->field_0);
        gte_SetTransMatrix(&ptr->field_0);
        gte_ldv0(&D_800D7A5C[i]);
        gte_rt();
        gte_stlvnl(&ptr->field_20[i]);

        ptr->field_50[i].vx = Q8_TO_Q12(ptr->field_20[i].vx);
        ptr->field_50[i].vy = Q8_TO_Q12(ptr->field_20[i].vy);
        ptr->field_50[i].vz = Q8_TO_Q12(ptr->field_20[i].vz);
    }

    if ((PLAYER_CHARA().position.vy - Q12(1.35f)) < ptr->field_50[0].vy)
    {
        floatstinger->collision.box.topExtent            = ptr->field_50[1].vy - floatstinger->position.vy;
        floatstinger->collision.box.height               = ptr->field_50[0].vy - floatstinger->position.vy;
        floatstinger->collision.cylinder.outerRadius     = Q12(0.4f);
        floatstinger->collision.cylinder.innerRadius     = Q12(0.4f);
        floatstinger->collision.shapeOffsets.box.vx      = ((ptr->field_50[0].vx + ptr->field_50[1].vx) >> 1) - floatstinger->position.vx;
        floatstinger->collision.shapeOffsets.cylinder.vx = floatstinger->collision.shapeOffsets.box.vx;
        floatstinger->collision.shapeOffsets.box.vz      = (ptr->field_50[0].vz + ptr->field_50[1].vz >> 1) - floatstinger->position.vz;
        floatstinger->collision.shapeOffsets.cylinder.vz = floatstinger->collision.shapeOffsets.box.vz;
    }
    else
    {
        floatstinger->collision.box.topExtent            = ptr->field_50[0].vy - floatstinger->position.vy;
        floatstinger->collision.box.height               = ptr->field_50[2].vy - floatstinger->position.vy;
        floatstinger->collision.cylinder.outerRadius     = Q12(0.5f);
        floatstinger->collision.cylinder.innerRadius     = Q12(0.5f);
        floatstinger->collision.shapeOffsets.box.vx      = ((ptr->field_50[0].vx + ptr->field_50[2].vx) >> 1) - floatstinger->position.vx;
        floatstinger->collision.shapeOffsets.cylinder.vx = floatstinger->collision.shapeOffsets.box.vx;
        floatstinger->collision.shapeOffsets.box.vz      = (ptr->field_50[0].vz + ptr->field_50[2].vz >> 1) - floatstinger->position.vz;
        floatstinger->collision.shapeOffsets.cylinder.vz = floatstinger->collision.shapeOffsets.box.vz;
    }

    if (((ptr->field_50[0].vy + ptr->field_50[1].vy) >> 1) > (PLAYER_CHARA().position.vy - Q12(1.4f)))
    {
        floatstinger->collision.box.offsetY = ((ptr->field_50[0].vy + ptr->field_50[1].vy) >> 1) - floatstinger->position.vy;
    }
    else if (((ptr->field_50[0].vy + ptr->field_50[2].vy) >> 1) < (PLAYER_CHARA().position.vy - Q12(1.4f)))
    {
        floatstinger->collision.box.offsetY = ((ptr->field_50[0].vy + ptr->field_50[2].vy) >> 1) - floatstinger->position.vy;
    }
    else
    {
        var_v1                              = floatstinger->position.vy + Q12(1.4f);
        floatstinger->collision.box.offsetY = PLAYER_CHARA().position.vy - var_v1;
    }

    if (floatstinger->model.anim.status == ANIM_STATUS(FloatstingerAnim_1, true))
    {
        if (FP_FROM(floatstinger->model.anim.time, Q12_SHIFT) > 15 &&
            FP_FROM(floatstinger->model.anim.time, Q12_SHIFT) < 27)
        {
            floatstinger->collision.cylinder.outerRadius = Q12_MULT_PRECISE(floatstinger->collision.cylinder.outerRadius, Q12(0.3f));
        }
        else if (FP_FROM(floatstinger->model.anim.time, Q12_SHIFT) > 11 &&
                 FP_FROM(floatstinger->model.anim.time, Q12_SHIFT) < 16)
        {
            floatstinger->collision.cylinder.outerRadius = Q12_MULT_PRECISE(floatstinger->collision.cylinder.outerRadius, Q12(1.0f) - Q12_MULT_PRECISE((floatstinger->model.anim.time - Q12(12.0f)) >> 2, Q12(0.7f)));
        }
        else if (FP_FROM(floatstinger->model.anim.time, Q12_SHIFT) > 26 &&
                 FP_FROM(floatstinger->model.anim.time, Q12_SHIFT) < 31)
        {
            floatstinger->collision.cylinder.outerRadius = Q12_MULT_PRECISE(floatstinger->collision.cylinder.outerRadius, Q12_MULT_PRECISE((Q12(31.0f) - floatstinger->model.anim.time) >> 2, Q12(0.7f)) + Q12(0.3f));
        }
    }

    if (PLAYER_CHARA().health <= Q12(0.0f))
    {
        floatstinger->collision.box.bottomExtent = Q12(-2.0f);
    }
    else
    {
        floatstinger->collision.box.bottomExtent = Q12(-1.46f);
    }
}

void func_800D3F84(VECTOR3* floatstinger, s32 arg1, s32 arg2) // 0x800D3F84
{
    VECTOR  sp10;
    MATRIX  sp20;
    SVECTOR sp40;
    VECTOR  sp48;
    s32     sp58;
    s16     colR;
    s16     colRG;
    TILE*   tile;

    sp10.vx = Q12(FP_FROM(PLAYER_CHARA().position.vx, Q12_SHIFT));
    sp10.vy = Q12(FP_FROM(PLAYER_CHARA().position.vy, Q12_SHIFT));
    sp10.vz = Q12(FP_FROM(PLAYER_CHARA().position.vz, Q12_SHIFT));

    Vw_WorldScreenMatrixAtPositionGet(&sp20, sp10.vx, sp10.vy, sp10.vz);

    gte_SetRotMatrix(&sp20);
    gte_SetTransMatrix(&sp20);
    gte_ReadGeomScreen(&sp58);

    tile = GsOUT_PACKET_P;

    Math_SetSVectorFastSum(&sp40, Q12_TO_Q8(floatstinger->vx - sp10.vx), Q12_TO_Q8(floatstinger->vy - sp10.vy), Q12_TO_Q8(floatstinger->vz - sp10.vz));
    gte_ldv0(&sp40);
    gte_rt();
    gte_stlvnl(&sp48);

    if (sp48.vz >= sp58)
    {
        s32 x = (sp48.vx * sp58) / sp48.vz;
        s32 y = (sp48.vy * sp58) / sp48.vz;
        setXY0(tile, x, y);

        // `setTile(tile)` in reverse order?
        setcode(tile, 0x60);
        setlen(tile, 3);

        setSemiTrans(tile, false);

        setRGB0Fast(tile, (arg1 & 4) ? 0xC4 : 0, (arg1 & 2) ? 0xC4 : 0, (arg1 & 1) ? 0xC4 : 0);

        tile->w = arg2;
        tile->h = arg2;

        addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[12], tile);
        tile++;
        GsOUT_PACKET_P = tile;
    }
}

void func_800D41F0(s_SubCharacter* floatstinger) // 0x800D41F0
{
    s16 temp_v1;
    s16 temp_s0;
    s32 temp_t0;
    s16 temp_v0;
    s32 var_a1;
    s32 var_v0;
    s16 temp_s1;

    temp_s0 = Q12_MULT(floatstinger->moveSpeed, Math_Sin(floatstinger->headingAngle));
    temp_s1 = floatstinger->fallSpeed;
    temp_v0 = Q12_MULT(floatstinger->moveSpeed, Math_Cos(floatstinger->headingAngle));
    temp_v1 = Math_Vector3MagCalcSafe(temp_s0, temp_s1, temp_v0);
    temp_t0 = (Q12_MULT_PRECISE(temp_v1, Q12(15.0f) - temp_v1) >> 1) + Q12(13.0f);

    if (floatstingerProps.field_EE == 0)
    {
        var_v0 = Q12(13.0f);
    }
    else if (floatstingerProps.field_EE >= 0)
    {
        var_v0 = Q12_MULT_PRECISE(floatstingerProps.field_EE, Q12(24.0f)) + Q12(21.0f);
    }
    else
    {
        var_v0 = Q12_MULT_PRECISE(-floatstingerProps.field_EE, Q12(24.0f)) + Q12(21.0f);
    }

    if (var_v0 < temp_t0)
    {
        var_a1 = temp_t0;
    }
    else
    {
        var_a1 = var_v0;
    }

    if (var_a1 < D_800D7860)
    {
        D_800D7860 = CLAMP_HIGH(var_a1, D_800D7860 + Q12_MULT_PRECISE(g_DeltaTime, Q12(6.0f)));
    }
    else
    {
        D_800D7860 = MAX(var_a1, D_800D7860 - Q12_MULT_PRECISE(g_DeltaTime, Q12(6.0f)));
    }
}

bool func_800D4458(s_SubCharacter* floatstinger, VECTOR3* arg1) // 0x800D4458
{
    q19_12 angle0;
    q19_12 angle1;
    q19_12 angle2;
    s32    i;
    s32    var_s0;
    s32    var_s3;
    s32    caseVar;

    var_s3 = 0;
    angle0 = 0;

    D_800DB898 = 0;
    D_800DB89A = 0;
    D_800DB89C = 0;

    for (i = 0; i < 3; i++)
    {
        if (arg1->vy + floatstinger->collision.box.bottomExtent >= D_800D780C[i].field_10)
        {
            break;
        }
    }

    if (i == 3)
    {
        return false;
    }

    D_800DB89A = D_800D780C[i].field_10 - (arg1->vy + floatstinger->collision.box.bottomExtent);

    if (D_800D780C[i].field_0[0] <= arg1->vx)
    {
        if (D_800D780C[i].field_0[1] < arg1->vx)
        {
            caseVar = 2;
        }
        else
        {
            caseVar = 1;
        }
    }
    else
    {
        caseVar = 0;
    }

    if (D_800D780C[i].field_8[0] <= arg1->vz)
    {
        if (D_800D780C[i].field_8[1] < arg1->vz)
        {
            caseVar += 6;
        }
        else
        {
            caseVar += 3;
        }
    }

    switch (caseVar)
    {
        case 0:
        case 2:
        case 6:
        case 8:
            var_s3 = Math_Vector2MagCalcSafeQ6(arg1->vx - D_800D780C[i].field_0[(caseVar % 6) != 0],
                                               arg1->vz - D_800D780C[i].field_8[caseVar > 2]);

            angle0 = ratan2(arg1->vx - D_800D780C[i].field_0[(caseVar % 6) != 0],
                            arg1->vz - D_800D780C[i].field_8[caseVar > 2]);
            break;

        case 1:
            angle0 = Q12_ANGLE(180.0f);
            var_s3 = D_800D780C[i].field_8[0] - arg1->vz;
            break;

        case 3:
            angle0 = Q12_ANGLE(270.0f);
            var_s3 = D_800D780C[i].field_0[0] - arg1->vx;
            break;

        case 5:
            angle0 = Q12_ANGLE(90.0f);
            var_s3 = arg1->vx - D_800D780C[i].field_0[1];
            break;

        case 7:
            angle0 = Q12_ANGLE(0.0f);
            var_s3 = arg1->vz - D_800D780C[i].field_8[1];
            break;
    }

    angle1 = ABS(Math_AngleNormalizeSigned(angle0 - floatstinger->rotation.vy));
    if ((Q12_ANGLE(90.0f) - angle1) < Q12_ANGLE(0.0f))
    {
        angle2 = Q12_ANGLE(180.0f) - angle1;
    }
    else
    {
        angle2 = angle1;
    }

    if (angle1 > Q12_ANGLE(90.0f))
    {
        var_s0 = ((angle2 << 13) >> 10) + Q12(1.0f);
    }
    else
    {
        var_s0 = ((angle2 << 12) >> 10) + Q12(2.0f);
    }

    if (var_s3 < var_s0)
    {
        D_800DB898 = Q12_MULT(var_s0 - var_s3, Math_Sin(angle0));
        D_800DB89C = Q12_MULT(var_s0 - var_s3, Math_Cos(angle0));
        return true;
    }
    else
    {
        return false;
    }
}

q19_12 func_800D48A4(s_SubCharacter* floatstinger, q3_12 dist, q3_12 headingAngle) // 0x800D48A4
{
    s_RayTrace trace;
    VECTOR3    from; // Q19.12
    VECTOR3    dir;  // Q19.12
    q3_12      unkAngle0;
    q19_12     unkAngle1;
    s32        count;
    s32        i;

    count = Rng_Rand16() & 0xFF;
    count = !(count < 0x80);

    from.vx = floatstinger->position.vx;
    from.vy = floatstinger->position.vy + floatstinger->collision.box.bottomExtent;
    from.vz = floatstinger->position.vz;

    for (i = count; i < (count + 8); i++)
    {
        if (i & 1)
        {
            unkAngle0 = (Q12_ANGLE(22.5f) << ((i + 1) >> 1)) + (Rng_Rand16() & 0x3F);
        }
        else
        {
            unkAngle0 = -((Q12_ANGLE(22.5f) << ((i + 2) >> 1)) + (Rng_Rand16() & 0x3F));
        }

        unkAngle1 = unkAngle0 + headingAngle;

        dir.vx = Q12_MULT(dist, Math_Sin(unkAngle1));
        dir.vy = Q12(0.0f);
        dir.vz = Q12_MULT(dist, Math_Cos(unkAngle1));

        if (Ray_GeomAndCharaTraceGet(&trace, &from, &dir, floatstinger) == false)
        {
            return Math_AngleNormalizeSigned(headingAngle + unkAngle0);
        }
    }

    return headingAngle;
}

void func_800D4A3C(s_SubCharacter* floatstinger, VECTOR3* pos, q3_12 newRotY) // 0x800D4A3C
{
    VECTOR3 newPos;
    q19_12  dist;
    q3_12   angle1;
    q3_12   rotY;
    q19_12  dist0;
    s16     var_s3;
    s32     temp_t2;
    s32     temp_v0_13;
    s32     temp_v0_14;
    q3_12   unkAngle;
    s16     var_s4;
    s32     var_v0_11;
    s32     var_v1_4;

    dist     = Math_Vector2MagCalcSafeQ6(pos->vx - floatstinger->position.vx, pos->vz - floatstinger->position.vz);
    unkAngle = ratan2(pos->vx - floatstinger->position.vx, pos->vz - floatstinger->position.vz);

    if (dist > Q12(6.5f))
    {
        rotY = floatstinger->rotation.vy;
        rotY = unkAngle - rotY;
    }
    else
    {
        rotY = floatstinger->rotation.vy;
        rotY = newRotY - rotY;
    }

    angle1 = Math_AngleNormalizeSigned(rotY);
    if (ABS(angle1) > ((floatstingerProps.flags_E8 & FloatstingerFlag_0) ? ABS(((Q12_MULT_PRECISE(floatstingerProps.field_EE, g_DeltaTime) * 3) >> 4) + 1) : ABS((Q12_MULT_PRECISE(floatstingerProps.field_EE, g_DeltaTime) >> 1) + 1)))
    {
        if (angle1 > Q12_ANGLE(0.0f))
        {
            floatstingerProps.field_EE = CLAMP_HIGH((floatstingerProps.flags_E8 & FloatstingerFlag_0) ? Q12_ANGLE(135.0f) : Q12_ANGLE(90.0f),
                                                    floatstingerProps.field_EE + ((floatstingerProps.flags_E8 & FloatstingerFlag_0) ? Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(135.0f)) :
                                                                                                                                      Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(90.0f))));
        }
        else
        {
            floatstingerProps.field_EE = MAX((floatstingerProps.flags_E8 & FloatstingerFlag_0) ? Q12_ANGLE(-135.0f) : Q12_ANGLE(-90.0f),
                                             floatstingerProps.field_EE - ((floatstingerProps.flags_E8 & FloatstingerFlag_0) ? Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(135.0f)) :
                                                                                                                               Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(90.0f))));
        }

        floatstinger->rotation.vy = Math_AngleNormalizeSigned(Q12_MULT_PRECISE(g_DeltaTime, floatstingerProps.field_EE) + floatstinger->rotation.vy);
    }
    else
    {
        floatstinger->rotation.vy  = newRotY;
        floatstingerProps.field_EE = APPROACH_ALT2(floatstingerProps.field_EE, Q12_ANGLE(0.0f),
                                                   (floatstingerProps.flags_E8 & FloatstingerFlag_0) ? (Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(135.0f)) * 2) :
                                                                                                       (Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(90.0f)) * 2));
    }

    if (floatstingerProps.field_F0 == Q12(0.0f) || dist < Q12(4.0f))
    {
        dist0 = MIN(dist, Q12(3.5f));

        newPos.vx = floatstinger->position.vx + Q12_MULT(dist0, Math_Sin(unkAngle));
        newPos.vy = floatstinger->position.vy;
        newPos.vz = floatstinger->position.vz + Q12_MULT(dist0, Math_Cos(unkAngle));

        if (func_800D4458(floatstinger, &newPos))
        {
            if (floatstingerProps.field_10E != Q12(0.0f))
            {
                floatstingerProps.field_F4  = floatstingerProps.field_10C;
                floatstingerProps.field_10E = CLAMP_LOW(floatstingerProps.field_10E - g_DeltaTime, Q12(0.0f));
            }
            else
            {
                floatstingerProps.field_F4  = func_800D48A4(floatstinger, dist0, unkAngle);
                floatstingerProps.field_10C = floatstingerProps.field_F4;
                floatstingerProps.field_10E = Rng_GenerateInt(Q12(0.4f), Q12(1.0f) - 2);
            }
        }
        else
        {
            floatstingerProps.field_F4  = unkAngle;
            floatstingerProps.field_10E = Q12(0.0f);
        }
    }
    else
    {
        floatstingerProps.field_F0 += g_DeltaTime;
        if (floatstingerProps.field_F0 > Q12(0.8f) &&
            !Rng_GenerateUInt(0, 15)) // 1 in 16 chance.
        {
            floatstingerProps.field_F0 = Q12(0.0f);
        }
    }

    var_s4 = Q12_MULT(Math_Sin(floatstingerProps.field_F4),
                      Q12_MULT_PRECISE(g_DeltaTime, (floatstingerProps.flags_E8 & FloatstingerFlag_0 ? Q12(2.5f) : Q12(1.5f))));

    var_s3 = Q12_MULT(Math_Cos(floatstingerProps.field_F4),
                      Q12_MULT_PRECISE(g_DeltaTime, (floatstingerProps.flags_E8 & FloatstingerFlag_0 ? Q12(2.5f) : Q12(1.5f))));

    if ((var_s4 * Math_Sin(floatstinger->headingAngle)) > Q12(0.0f))
    {
        var_s4 += Q12_MULT(floatstinger->moveSpeed, Math_Sin(floatstinger->headingAngle));
    }
    else
    {
        var_v0_11 = Q12_MULT(floatstinger->moveSpeed, Math_Sin(floatstinger->headingAngle));
        var_s4   += (floatstingerProps.flags_E8 & FloatstingerFlag_0) ? (var_v0_11 >> 3) : (var_v0_11 >> 2);
    }

    if ((var_s3 * Math_Cos(floatstinger->headingAngle)) > Q12(0.0f))
    {
        var_s3 += Q12_MULT(floatstinger->moveSpeed, Math_Cos(floatstinger->headingAngle));
    }
    else
    {
        temp_v0_13 = Q12_MULT(floatstinger->moveSpeed, Math_Cos(floatstinger->headingAngle));
        var_s3    += (floatstingerProps.flags_E8 & FloatstingerFlag_0) ? (temp_v0_13 >> 3) : (temp_v0_13 >> 2);
    }

    floatstinger->headingAngle = ratan2(var_s4, var_s3);

    temp_v0_14 = Math_Vector2MagCalcSafeQ6(var_s4, var_s3);

    if (((floatstingerProps.flags_E8 & FloatstingerFlag_0) && temp_v0_14 > Q12(3.5f)) ||
        (!(floatstingerProps.flags_E8 & FloatstingerFlag_0) && temp_v0_14 > Q12(2.0f)))
    {
        var_v1_4 = Math_Vector2MagCalcSafeQ6(var_s4, var_s3);
    }
    else
    {
        if (floatstingerProps.flags_E8 & FloatstingerFlag_0)
        {
            var_v1_4 = Q12(3.5f);
        }
        else
        {
            var_v1_4 = Q12(2.0f);
        }
    }

    floatstinger->moveSpeed = var_v1_4;

    if (dist < Q12(4.0f))
    {
        if (dist < Q12(0.05f))
        {
            floatstinger->moveSpeed    = Q12(0.0f);
            floatstinger->headingAngle = floatstinger->rotation.vy;
            floatstinger->position.vx  = pos->vx;
            floatstinger->position.vz  = pos->vz;
        }
        else
        {
            floatstinger->moveSpeed = (floatstinger->moveSpeed + ((floatstinger->moveSpeed * dist) / Q12(4.0f))) >> 1;

            if (ABS(Math_AngleNormalizeSigned(unkAngle - floatstinger->headingAngle)) > Q12_ANGLE(45.0f))
            {
                floatstinger->moveSpeed = Q12_MULT_PRECISE(floatstinger->moveSpeed, (ABS(Math_AngleNormalizeSigned(unkAngle - floatstinger->headingAngle)) << 1) + Q12_ANGLE(45.0f));
            }
        }
    }

    temp_t2 = pos->vy - floatstinger->position.vy;

    if (temp_t2 < 0)
    {
        if (ABS(temp_t2) > 81)
        {
            floatstinger->fallSpeed = MAX((floatstingerProps.flags_E8 & FloatstingerFlag_0) ? Q12(-5.0f) : Q12(-1.0f),
                                          floatstinger->fallSpeed + Q12_MULT_PRECISE(g_DeltaTime, floatstingerProps.flags_E8 & 1 ? Q12(-3.0f) : Q12(-0.8f)));

            if (ABS(temp_t2) < Q12(1.0f))
            {
                floatstinger->fallSpeed = floatstinger->fallSpeed + ((floatstinger->fallSpeed * ABS(temp_t2)) / Q12(1.0f)) >> 1;
            }
        }
        else
        {
            floatstinger->fallSpeed   = Q12(0.0f);
            floatstinger->position.vy = pos->vy;
        }
    }
    else
    {
        if (ABS(temp_t2) > 81)
        {
            floatstinger->fallSpeed = CLAMP_HIGH((floatstingerProps.flags_E8 & FloatstingerFlag_0) ? Q12(9.0f) : Q12(3.0f),
                                                 floatstinger->fallSpeed + Q12_MULT_PRECISE(g_DeltaTime, floatstingerProps.flags_E8 & 1 ? 0x4800 : 0x1CCC));

            if (ABS(temp_t2) < Q12(1.0f))
            {
                floatstinger->fallSpeed = floatstinger->fallSpeed + (floatstinger->fallSpeed * ABS(temp_t2) / Q12(1.0f)) >> 1;
            }
        }
        else
        {
            floatstinger->fallSpeed   = Q12(0.0f);
            floatstinger->position.vy = pos->vy;
        }
    }

    func_800D41F0(floatstinger);
}
