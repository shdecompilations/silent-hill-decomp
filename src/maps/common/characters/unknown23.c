#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "main/rng.h"
#include "maps/common/characters/unknown23.h"
#include "maps/map7/map7_s03.h"
#include "maps/common/unk_m3s03.h"

// Unknown23: Incubator transforms into this based on `map7_s03::func_800E9498`?
// Seems to include parts of Incubus code here exactly, maybe some half-way chara before Incubus spawns?

#define unknown23Props      chara->properties.incubus
#define localUnknown23Props localChara->properties.incubus

void func_800DF1D4(void) // 0x800DF1D4
{
    s32    i;
    q19_12 randAngle;
    q19_12 sinARandAngle;
    q19_12 cosRandAngle;

    s_func_800DF1D4_58C* curPtr = ((s_func_800DF1D4*)FS_BUFFER_26)->field_58C;
    for (i = 0; i < 100; i++, curPtr++)
    {
        Math_Vector3Set(&curPtr->field_8, 0, 0, 0);

        curPtr->field_38 = 255;

        randAngle     = Rng_Rand16();
        sinARandAngle = Math_Sin(randAngle);
        cosRandAngle  = Math_Cos(randAngle);

        Math_SVectorSet(&curPtr->field_28, cosRandAngle, sinARandAngle, 0);

        curPtr->field_0 = 0;
        curPtr->field_4 = Q12(1.0f);
    }
}

void func_800DF288(s32 x, s32 y, s32 val) // 0x800DF288
{
    s16              row;
    s16              col;
    s_func_800DF1D4* ptr;
    u8*              buf;

    ptr = FS_BUFFER_26;

    col = x + 160;
    row = y + 120;

    col += (col > 0) ? 4 : -4;
    col /= 8;

    row += (row > 0) ? 4 : -4;
    row /= 8;

    if (col > 0 && col < 41 && row > 0 && row < 31)
    {
        buf  = ptr->field_5D + (col + (41 * row));
        *buf = val;
    }
}

void func_800DF348(void) // 0x800DF348
{
    SVECTOR              sp10;
    DVECTOR              sp18[2];
    s32                  sp20;
    s32                  i;
    s_func_800DF1D4_58C* curPtr;
    s_func_800DF1D4*     base;

    base = (s_func_800DF1D4*)FS_BUFFER_26;

    curPtr = base->field_58C;
    for (i = 0; i < 100; i++, curPtr++)
    {
        sp10.vx = curPtr->field_8.vx >> 7;
        sp10.vy = curPtr->field_8.vy >> 7;
        sp10.vz = 0;

        RotTransPers(&sp10, &sp18[0], &sp18[1], &sp20);
        func_800DF288(sp18[0].vx, sp18[0].vy, curPtr->field_38);

        curPtr->field_8.vx += curPtr->field_28.vx;
        curPtr->field_8.vy += curPtr->field_28.vy;
        curPtr->field_8.vz += curPtr->field_28.vz;
        curPtr->field_38    = curPtr->field_38 - 8;
        curPtr->field_38    = MAX(curPtr->field_38, 0);
    }
}

s32 func_800DF418(s32 arg0, s32 arg1) // 0x800DF418
{
    s_func_800DF1D4* ptr = (s_func_800DF1D4*)FS_BUFFER_26;

    return D_800ECA50[ptr->field_5D[(arg1 * 41) + arg0]];
}

void func_800DF458(void) // 0x800DF458
{
    u32              sp18;
    s_func_800DF1D4* ptr;
    GsOT_TAG*        ot;
    s32              col0;
    s32              col2;
    s32              col3;
    s32              i;
    s32              j;
    s32              col1;
    POLY_G4*         poly;
    DR_MODE*         mode;
    PACKET*          packet;
    s32              col;
    s32              idx;
    int              code;

    ptr = FS_BUFFER_26;

    idx = g_ActiveBufferIdx;
    ot  = g_OrderingTable0[idx].org;
    ot  = &ot[ptr->field_584 >> 1];

    packet = GsOUT_PACKET_P;
    poly   = packet;

    col  = 0x3A000000;
    code = 0x3A;

    for (i = 1; i < 31; i++)
    {
        col1 = func_800DF418(0, i - 1);
        col3 = func_800DF418(0, i);

        for (j = 1; j < 41; j++)
        {
            sp18 = ptr->field_5D[(i * 41) + j];

            col0 = col1;
            col2 = col3;
            col1 = func_800DF418(j, i - 1);

            col3 = func_800DF418(j, i);

            if (col == col0 &&
                col0 == col1 &&
                col0 == col2 &&
                col2 == col3)
            {
                continue;
            }

            if ((j + i) & 0x1)
            {
                poly->x0 = -168 + (j * 8);
                poly->y0 = -120 + (i * 8);

                poly->x1 = (-168 + (j * 8)) + 8;
                poly->y1 = -120 + (i * 8);

                poly->x2 = -168 + (j * 8);
                poly->y2 = (-120 + (i * 8)) + 8;

                poly->x3 = (-168 + (j * 8)) + 8;
                poly->y3 = (-120 + (i * 8)) + 8;

                *(s32*)&poly->r0 = col0;
                *(s32*)&poly->r1 = col1;
                *(s32*)&poly->r2 = col2;
                *(s32*)&poly->r3 = col3;
            }
            else
            {
                poly->x1 = -168 + (j * 8);
                poly->y1 = -120 + (i * 8);

                poly->x0 = (-168 + (j * 8)) + 8;
                poly->y0 = -120 + (i * 8);

                poly->x3 = -168 + (j * 8);
                poly->y3 = (-120 + (i * 8)) + 8;

                poly->x2 = (-168 + (j * 8)) + 8;
                poly->y2 = (-120 + (i * 8)) + 8;

                *(s32*)&poly->r0 = col1;
                *(s32*)&poly->r1 = col0;
                *(s32*)&poly->r2 = col3;
                *(s32*)&poly->r3 = col2;
            }

            setPolyG4(poly);
            poly->code = (float)sp18; // @hack
            poly->code = code;

            addPrim(ot, poly);
            poly++;
        }
    }

    packet = poly;
    mode   = packet;

    SetDrawMode(mode, 0, 1, 42, NULL);
    addPrim(ot, mode);
    packet         = mode + 1;
    GsOUT_PACKET_P = packet;
}

void func_800DF750(void) // 0x800DF750
{
    #define ROW_COUNT 31
    #define COL_COUNT 41

    s32 row;
    s32 col;
    s32 val;

    s_func_800DF1D4* base = (s_func_800DF1D4*)FS_BUFFER_26;

    for (row = 0; row < ROW_COUNT; row++)
    {
        for (col = 0; col < COL_COUNT; col++)
        {
            // TODO: Maybe `field_5D` actually begins at 0x34, but the first row (0x34 to 0x5D) only gets used indirectly?
            // 0x5D is weird offset to start at. No luck using multi-dimensional array yet either.
            u8* ptr = &base->field_5D[row * COL_COUNT];

            val = ptr[col - COL_COUNT];

            if (col == 0)
            {
                // Add same column from previous row.
                val = 0;
            }
            else
            {
                // Add value from previous column.
                val += ptr[col - 1];
            }

            if (col == (COL_COUNT - 1))
            {
                val = 0;
            }
            else
            {
                // Add value from next column.
                val += ptr[col + 1];
            }

            // Add column from next row.
            val += ptr[col + COL_COUNT];

            val >>= 2;
            val--;

            if (val <= 0)
            {
                ptr[col] = 0;
            }
            else
            {
                ptr[col] = val;
            }
        }
    }

    #undef ROW_COUNT
    #undef COL_COUNT
}

void func_800DF7F8(void) // 0x800DF7F8
{
    s_func_800DF1D4* buf = FS_BUFFER_26;

    memset(buf->field_34, 0, sizeof(buf->field_34));
    memset(buf->field_5D, 0, sizeof(buf->field_5D));
    memset(buf->field_554, 0, sizeof(buf->field_554));
}

void func_800DF84C(VECTOR* arg0, s32 arg1) // 0x800DF84C
{
    MATRIX  mat;
    VECTOR  vec;
    SVECTOR svec;

    SetRotMatrix(&GsWSMATRIX);
    SetTransMatrix(&GsWSMATRIX);

    ApplyRotMatrixLV(arg0, &vec);

    vec.vx += GsWSMATRIX.t[0];
    vec.vy += GsWSMATRIX.t[1];
    vec.vz += GsWSMATRIX.t[2];
    TransMatrix(&mat, &vec);
    SetTransMatrix(&mat);

    Math_SVectorSet(&svec, 0, arg1, 0);
    Math_RotMatrixZxyNeg(&svec, &mat);
    SetMulRotMatrix(&mat);
}

s32 func_800DF90C(void) // 0x800DF90C
{
    SVECTOR sp10;
    s32     sp18;

    sp10.vx = Q8(0.0f);
    sp10.vy = Q8(0.0f);
    sp10.vz = Q8(0.0f);
    return RotTransPers(&sp10, &sp18, &sp18, &sp18);
}

void func_800DF944(void) // 0x800DF944
{
    s_func_800DFA48* ptr;

    ptr = FS_BUFFER_26;

    switch (D_800F3DB8)
    {
        case 0:
            break;

        case 1:
            func_800DF7F8();
            func_800DF1D4();
            D_800F3DB8++;

        default:
            func_800DF84C(&ptr->field_4, ptr->field_588);
            ptr->field_584 = func_800DF90C();

            if (g_DeltaTime != Q12(0.0f))
            {
                func_800DF348();
                func_800DF750();
            }

            func_800DF458();

            D_800F3DB8 += g_DeltaTime;
            if (D_800F3DB8 > Q12(1.5f))
            {
                D_800F3DB8 = 0;
            }
            break;
    }
}

void func_800DFA14(void) // 0x800DFA14
{
    D_800F3DB8 = 0;
    memset(FS_BUFFER_26, 0xA7, 0x1CFC);
}

void func_800DFA48(VECTOR3* arg0, VECTOR3* arg1) // 0x800DFA48
{
    s_func_800DFA48* ptr;
    q19_12           angle;

    ptr = FS_BUFFER_26;

    // TODO: Decode into `WEAPON_ATTACK` macro.
    Chara_AttackReceivedSet(&g_SysWork.playerWork.player, 68);

    angle                                          = ratan2(arg0->vx - arg1->vx, arg0->vz - arg1->vz);
    g_SysWork.playerWork.player.damage.amount      = 1;
    g_SysWork.playerWork.player.damage.position.vy = angle;
    ptr->field_588                                 = angle;

    D_800F3DB8 = 1;

    ptr->field_4.vx = Q12_TO_Q8(arg0->vx);
    ptr->field_4.vy = Q12_TO_Q8(arg0->vy) - Q8(1.3f);
    ptr->field_4.vz = Q12_TO_Q8(arg0->vz);

    Sfx_WithFlagsPlay(Sfx_Unk1673, &g_SysWork.playerWork.player.position, Q8(0.5f), SfxFlag_None);
}

s32 func_800DFB04(void) // 0x800DFB04
{
    u8 sp10;

    Player_DisableDamage(&sp10, false);
    return sp10;
}

void func_800DFB2C(bool disableDamage) // 0x800DFB2C
{
    u8 sp10;

    Player_DisableDamage(&sp10, disableDamage);
}

void func_800DFB50(s_SubCharacter* chara) // 0x800DFB50
{
    if (chara->model.controlState != 0)
    {
        chara->model.controlState = 4;
        chara->model.stateStep    = 0;
    }
    else
    {
        chara->model.stateStep = 4;
    }
}

void func_800DFB74(s_SubCharacter* chara) // 0x800DFB74
{
    if (chara->model.controlState != 0)
    {
        chara->model.controlState = 2;
        chara->model.stateStep    = 0;
    }
    else
    {
        chara->model.stateStep = 2;
    }
}

bool Unknown23_Init(s_SubCharacter* chara, GsCOORDINATE2* boneCoords) // 0x800DFB98
{
    s_SubCharacter* localChara; // TODO: Not sure why this is needed here, might be an inline in this func.

    localChara = chara;

    chara->model.anim.alpha = Q12(0.0f);

    if (g_SavegamePtr->gameDifficulty == GameDifficulty_Easy)
    {
        chara->health = Q12(17995.605f); // TODO: Some percentage taken from 30000?
    }
    else
    {
        chara->health = Q12(30000.0f);
    }

    chara->moveSpeed                          = Q12(0.0f);
    chara->collision.cylinder.outerRadius     = Q12(0.3f);
    chara->collision.shapeOffsets.cylinder.vx = Q12(0.0f);
    chara->collision.shapeOffsets.cylinder.vz = Q12(0.0f);
    chara->collision.state                    = CharaCollisionState_4;
    chara->headingAngle                       = chara->rotation.vy;
    chara->flags                             |= CharaFlag_Hit;

    localUnknown23Props.someState_F0 = 0;

    if (chara->model.stateStep == 0)
    {
        chara->model.controlState = 1;
        chara->model.stateStep    = 0;
    }
    else
    {
        chara->model.controlState = chara->model.stateStep;
        chara->model.stateStep    = 0;
    }

    Chara_AnimSet(chara, ANIM_STATUS(2, false), 115);
    ModelAnim_AnimInfoSet(&chara->model.anim, UNKKOWN_23_ANIM_INFOS);

    Chara_DamageClear(chara);

    if (g_SavegamePtr->gameDifficulty == GameDifficulty_Hard)
    {
        localUnknown23Props.bossFightTimer_F4 = Q12(300.0f);
    }
    else
    {
        localUnknown23Props.bossFightTimer_F4 = Q12(30.0f);
    }

    func_800DD67C(&chara->position, chara, boneCoords);
    func_800DFA14();

    chara->flags |= CharaFlag_NoRadioStatic;
    return true;
}

void func_800DFCE4(s_SubCharacter* chara) // 0x800DFCE4
{
    if (unknown23Props.someState_F0 == 0)
    {
        if (unknown23Props.bossFightTimer_F4 < Q12(0.0f))
        {
            chara->health        = 0;
            chara->damage.amount = 1;
        }

        if (!Inventory_GunsAvailableCheck(false))
        {
            unknown23Props.bossFightTimer_F4 -= g_DeltaTime;
        }

        if (!(chara->flags & CharaFlag_Hit))
        {
            chara->damage.amount *= 10;
        }

        if (chara->damage.amount > Q12(0.0f))
        {
            chara->health = MAX(Q12(0.0f), chara->health - chara->damage.amount);
            if (chara->health <= Q12(0.0f) && func_800DFB04() == 0)
            {
                Savegame_EventFlagSet(EventFlag_582);
                chara->model.controlState = 5;
                chara->model.stateStep    = 0;
                chara->health             = NO_VALUE;
                chara->flags             |= CharaFlag_Unk2;
                unknown23Props.someState_F0++;
            }
        }
    }

    Chara_DamageClear(chara);
}

void func_800DFE10(s_SubCharacter* chara) // 0x800DFE10
{
    s16 temp_v0;
    s16 var_v0;
    s16 var_v0_2;
    s32 tmp;

    temp_v0 = Math_AngleNormalizeSigned(ratan2(g_SysWork.playerWork.player.position.vx - chara->position.vx,
                                               g_SysWork.playerWork.player.position.vz - chara->position.vz) -
                                        chara->rotation.vy);
    var_v0  = ABS(temp_v0);

    if (var_v0 > Q12_ANGLE(10.0f))
    {
        var_v0_2 = Q12_ANGLE(90.0f);
        tmp      = Q12_MULT_PRECISE(g_DeltaTime, var_v0_2);
        if (temp_v0 > 0)
        {
            chara->rotation.vy += tmp;
        }
        else
        {
            chara->rotation.vy -= tmp;
        }
    }
}

void func_800DFEF0(s_SubCharacter* chara) // 0x800DFEF0
{
    if (chara->model.stateStep == 0)
    {
        chara->model.stateStep++;
        return;
    }

    if (chara->model.anim.status == 8)
    {
        chara->model.controlState = 2;
        chara->model.stateStep    = 0;
    }
}

void func_800DFF28(s_SubCharacter* chara) // 0x800DFF28
{
    if (chara->model.stateStep == 0)
    {
        chara->model.stateStep++;
    }
}

void func_800DFF44(s_SubCharacter* chara) // 0x800DFF44
{
    if (chara->model.stateStep == 0)
    {
        chara->model.stateStep++;
    }
}

void func_800DFF60(s_SubCharacter* chara, GsCOORDINATE2* boneCoords) // 0x800DFF60
{
    MATRIX          transformMat;
    VECTOR3         pos;
    q3_12           angleDeltaToPlayer;
    s_SubCharacter* localChara;

    localChara = chara;

    if (chara->model.stateStep == 0)
    {
        chara->model.stateStep++;
        unknown23Props.timer_E8 = Q12(1.5f);
        chara->flags           &= ~CharaFlag_Hit;
        return;
    }

    if (boneCoords == NULL)
    {
        func_800DFE10(chara);
    }

    switch (chara->model.stateStep)
    {
        case 1:
            if (boneCoords)
            {
                Vw_CoordHierarchyMatrixCompute(&boneCoords[6], &transformMat);
                pos.vx = Q8_TO_Q12(transformMat.t[0]);
                pos.vy = Q8_TO_Q12(transformMat.t[1]);
                pos.vz = Q8_TO_Q12(transformMat.t[2]);

                if (!(Rng_Rand16() & 0xF0))
                {
                    func_800DD0EC(&pos, 6);
                }

                Vw_CoordHierarchyMatrixCompute(&boneCoords[10], &transformMat);
                pos.vx = Q8_TO_Q12(transformMat.t[0]);
                pos.vy = Q8_TO_Q12(transformMat.t[1]);
                pos.vz = Q8_TO_Q12(transformMat.t[2]);

                if (!(Rng_Rand16() & 0xF0))
                {
                    func_800DD0EC(&pos, 10);
                }

                if (localUnknown23Props.timer_E8 < Q12(0.0f))
                {
                    if (g_SavegamePtr->gameDifficulty == GameDifficulty_Hard)
                    {
                        localUnknown23Props.timer_E8 = Q12(2.0f);
                    }
                    else
                    {
                        localUnknown23Props.timer_E8 = Q12(2.5f);
                    }

                    chara->model.stateStep++;
                }

                localUnknown23Props.timer_E8 -= g_DeltaTime;
            }
            break;

        case 2:
            if (boneCoords)
            {
                Vw_CoordHierarchyMatrixCompute(&boneCoords[6], &transformMat);
                pos.vx = Q8_TO_Q12(transformMat.t[0]);
                pos.vy = Q8_TO_Q12(transformMat.t[1]);
                pos.vz = Q8_TO_Q12(transformMat.t[2]);

                if (!(Rng_Rand16() & 0xFC))
                {
                    func_800DD0EC(&pos, 6);
                }

                if (!(Rng_Rand16() & 0x30))
                {
                    func_800DD260(&pos, &pos);
                }

                Vw_CoordHierarchyMatrixCompute(&boneCoords[10], &transformMat);
                pos.vx = Q8_TO_Q12(transformMat.t[0]);
                pos.vy = Q8_TO_Q12(transformMat.t[1]);
                pos.vz = Q8_TO_Q12(transformMat.t[2]);

                if (!(Rng_Rand16() & 0xFC))
                {
                    func_800DD0EC(&pos, 10);
                }

                if (!(Rng_Rand16() & 0x30))
                {
                    func_800DD260(&pos, &pos);
                }

                if (!(Rng_Rand16() & 0xFC))
                {
                    func_800DD464(&pos);
                }

                angleDeltaToPlayer = abs(Math_AngleNormalizeSigned(Math_AngleBetweenPositionsGet(chara->position, g_SysWork.playerWork.player.position) -
                                                       chara->rotation.vy));

                if (localUnknown23Props.timer_E8 < Q12(0.0f) && angleDeltaToPlayer < Q12_ANGLE(12.0f))
                {
                    localUnknown23Props.timer_E8 = Q12(0.3f);
                    chara->model.stateStep++;
                    break;
                }

                localUnknown23Props.timer_E8 -= g_DeltaTime;
            }
            break;

        case 3:
            if (boneCoords)
            {
                Vw_CoordHierarchyMatrixCompute(&boneCoords[6], &transformMat);
                pos.vx = Q8_TO_Q12(transformMat.t[0]);
                pos.vy = Q8_TO_Q12(transformMat.t[1]);
                pos.vz = Q8_TO_Q12(transformMat.t[2]);

                if (!(Rng_Rand16() & 0xFC))
                {
                    func_800DD0EC(&pos, 6);
                }

                if (!(Rng_Rand16() & 0x30))
                {
                    func_800DD3D4(&pos, D_800F48A8.velocityX_3C, 0, D_800F48A8.velocityZ_40);
                }

                Vw_CoordHierarchyMatrixCompute(&boneCoords[10], &transformMat);
                pos.vx = Q8_TO_Q12(transformMat.t[0]);
                pos.vy = Q8_TO_Q12(transformMat.t[1]);
                pos.vz = Q8_TO_Q12(transformMat.t[2]);

                if (!(Rng_Rand16() & 0xFC))
                {
                    func_800DD0EC(&pos, 10);
                }

                if (!(Rng_Rand16() & 0x30))
                {
                    func_800DD3D4(&pos, D_800F48A8.velocityX_3C, 0, D_800F48A8.velocityZ_40);
                }

                if (!(Rng_Rand16() & 0xFC))
                {
                    func_800DD464(&pos);
                }

                if (localUnknown23Props.timer_E8 < Q12(0.0f))
                {
                    chara->model.stateStep++;

                    if (g_SavegamePtr->gameDifficulty == GameDifficulty_Hard)
                    {
                        localUnknown23Props.timer_E8 = Q12(2.5f);
                    }
                    else
                    {
                        localUnknown23Props.timer_E8 = Q12(1.5f);
                    }
                }

                localUnknown23Props.timer_E8 -= g_DeltaTime;
            }
            break;

        case 4:
            if (boneCoords)
            {
                Vw_CoordHierarchyMatrixCompute(&boneCoords[6], &transformMat);
                pos.vx = Q8_TO_Q12(transformMat.t[0]);
                pos.vy = Q8_TO_Q12(transformMat.t[1]);
                pos.vz = Q8_TO_Q12(transformMat.t[2]);

                if (!(Rng_Rand16() & 0x70))
                {
                    func_800DD3D4(&pos, D_800F48A8.velocityX_3C, 0, D_800F48A8.velocityZ_40);
                }

                Vw_CoordHierarchyMatrixCompute(&boneCoords[10], &transformMat);
                pos.vx = Q8_TO_Q12(transformMat.t[0]);
                pos.vy = Q8_TO_Q12(transformMat.t[1]);
                pos.vz = Q8_TO_Q12(transformMat.t[2]);

                if (!(Rng_Rand16() & 0x70))
                {
                    func_800DD3D4(&pos, D_800F48A8.velocityX_3C, 0, D_800F48A8.velocityZ_40);
                }

                if (!(Rng_Rand16() & 0xFC))
                {
                    func_800DD464(&pos);
                }

                if (localUnknown23Props.timer_E8 < Q12(0.0f))
                {
                    chara->model.stateStep++;
                }

                localUnknown23Props.timer_E8 -= g_DeltaTime;
            }
            break;

        case 5:
            chara->model.controlState = 2;
            chara->model.stateStep    = 0;
            chara->flags             |= CharaFlag_Hit;
            break;
    }
}

q19_12 func_800E04C4(void) // 0x800E04C4
{
    q19_12 base;

    if (g_SavegamePtr->gameDifficulty == GameDifficulty_Hard)
    {
        base = Q12(2.0f);
    }
    else
    {
        base = Q12(5.0f);
    }

    return base + Rng_GenerateInt(0, Q12(2.0f) - 1);
}

void func_800E0528(s_SubCharacter* chara) // 0x800E0528
{
    if (chara->model.stateStep == 0)
    {
        unknown23Props.timer_E8 = func_800E04C4();
        chara->model.stateStep++;
        return;
    }

    func_800DFE10(chara);

    switch (chara->model.stateStep)
    {
        case 1:
            if (unknown23Props.timer_E8 <= Q12(0.0f))
            {
                chara->model.stateStep = 2;
            }
            break;

        case 2:
            chara->model.controlState = 3;
            chara->model.stateStep    = 0;
            break;
    }

    unknown23Props.timer_E8 -= g_DeltaTime;
}

void func_800E05DC(s_SubCharacter* chara, GsCOORDINATE2* boneCoords) // 0x800E05DC
{
    switch (chara->model.controlState)
    {
        case 4:
            func_800DFEF0(chara);
            break;

        case 5:
            func_800DFF28(chara);
            break;

        case 6:
            func_800DFF44(chara);
            break;

        case 3:
            func_800DFF60(chara, 0);
            break;

        case 2:
            func_800E0528(chara);
            break;

        case 1:
            break;
    }
}

void func_800E0670(s_SubCharacter* chara, GsCOORDINATE2* boneCoords) // 0x800E0670
{
    s32 dist;

    func_800DB608();

    if (chara->model.controlState == 3)
    {
        func_800DFF60(chara, boneCoords);
    }

    dist = Math_Vector2MagCalcSafeQ6(g_SysWork.playerWork.player.position.vx - chara->position.vx,
                               g_SysWork.playerWork.player.position.vz - chara->position.vz);
    if (dist < Q12(2.5f) && func_800DFB04() == 0)
    {
        func_800DFA48(&g_SysWork.playerWork.player.position, &chara->position);
    }
}

void func_800E0728(s_SubCharacter* chara) // 0x800E0728
{
    s_CollisionResult collResult;

    chara->fallSpeed += g_GravitySpeed;
    Chara_MovementUpdate0(chara, &collResult);
    chara->rotation.vy = Math_AngleNormalizeSigned(chara->rotation.vy);
}

void func_800E0774(s_SubCharacter* chara, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords) // 0x800E0774
{
    s_AnimInfo* animInfo;

    Math_MatrixTransform(&chara->position, &chara->rotation, boneCoords);

    if (chara->model.anim.status != ANIM_STATUS(0, false))
    {
        animInfo = &UNKKOWN_23_ANIM_INFOS[chara->model.anim.status];
        animInfo->playbackFunc(&chara->model, anmHdr, boneCoords, animInfo);
    }
}

void func_800E07F0(s_SubCharacter* chara, GsCOORDINATE2* boneCoords) // 0x800E07F0
{
    MATRIX transformMat;
    q19_12 posY;
    q19_12 posX;
    q19_12 posZ;

    Vw_CoordHierarchyMatrixCompute(&boneCoords[2], &transformMat);
    posX = Q8_TO_Q12(transformMat.t[0]) - chara->position.vx;
    posY = Q8_TO_Q12(transformMat.t[1]) - chara->position.vy;
    posZ = Q8_TO_Q12(transformMat.t[2]) - chara->position.vz;

    chara->collision.cylinder.outerRadius = Q12(0.5f);
    chara->collision.cylinder.innerRadius = Q12(0.5f);
    chara->collision.box.topExtent        = posY - Q12(0.25f);
    chara->collision.box.bottomExtent     = posY;
    chara->collision.box.height           = posY + Q12(0.25f);
    chara->collision.box.offsetY          = posY;

    sharedFunc_800CD920_3_s03(chara, posX, posZ);

    chara->collision.shapeOffsets.box.vx = chara->collision.shapeOffsets.cylinder.vx;
    chara->collision.shapeOffsets.box.vz = chara->collision.shapeOffsets.cylinder.vz;
}

void func_800E0888(s_SubCharacter* chara, GsCOORDINATE2* boneCoords) // 0x800E0888
{
    q19_12 posY;

    if (chara->model.anim.flags & AnimFlag_Visible)
    {
        func_800E07F0(chara, boneCoords);
        return;
    }

    posY            = chara->position.vy;
    chara->collision.box.bottomExtent = posY;
    chara->collision.box.height = posY;
    chara->collision.box.topExtent = posY - Q12(1.0f);
    chara->collision.box.offsetY = posY - Q12(0.5f);
}

void func_800E08E4(s_SubCharacter* chara, GsCOORDINATE2* boneCoords) // 0x800E08E4
{
    if (chara->model.stateStep == 0)
    {
        func_800E05DC(chara, boneCoords);
    }
}

void func_800E0914(s_SubCharacter* chara) // 0x800E0914
{
    func_800DD6CC();
    func_800DF944();
}

void Unknown23_Update(s_SubCharacter* chara, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords) // 0x800E093C
{
    if (chara->model.controlState == 0)
    {
        Unknown23_Init(chara, boneCoords);
    }

    if (chara->model.controlState != 1)
    {
        if (g_DeltaTime != Q12(0.0f))
        {
            func_800DFCE4(chara);
            func_800E05DC(chara, boneCoords);
            func_800E08E4(chara, boneCoords);
            func_800E0728(chara);
            func_800E0774(chara, anmHdr, boneCoords);
            func_800E0888(chara, boneCoords);
            func_800E0670(chara, boneCoords);
            func_800E0914(chara);
            func_800DFB2C(chara->flags & CharaFlag_Unk2);
        }
        else
        {
            func_800DB608();
            func_800E0914(chara);
            func_800E0774(chara, anmHdr, boneCoords);
        }
    }
}
