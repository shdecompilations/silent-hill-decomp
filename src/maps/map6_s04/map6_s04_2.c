#include "inline_no_dmpsx.h"

#include <psyq/gtemac.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/dms.h"
#include "bodyprog/events/event_data.h"
#include "bodyprog/gfx/map_effects.h"
#include "bodyprog/map/terrain.h"
#include "bodyprog/math/math.h"
#include "bodyprog/item_screens.h"
#include "bodyprog/player.h"
#include "bodyprog/sound/sound_system.h"
#include "main/rng.h"
#include "maps/map6/map6_s04.h"
#include "maps/characters/alessa.h"
#include "maps/characters/dahlia.h"
#include "maps/characters/monster_cybil.h"
#include "maps/particle.h"
#include "maps/characters/larval_stalker.h"
#include "maps/characters/stalker.h"
#include "maps/characters/player.h"

#include "../src/maps/unk_draw_m1s01.c"

#include "../src/maps/particle.c"

#include "../src/maps/characters/player.c"

#include "../src/maps/characters/larval_stalker.c" // 0x800D00B8
#include "../src/maps/characters/stalker.c" // 0x800D3560
#include "../src/maps/characters/monster_cybil.c" // 0x800D87B0
#include "../src/maps/characters/alessa.c"
#include "../src/maps/characters/dahlia.c"

#include "../src/maps/chara_util.c" // 0x800DD5B8

// TODO: Move to separate Flauros split.
#include "../src/maps/characters/flauros.c" // 0x800DE0C4

// TODO: Move to separate Parasite split.
#include "../src/maps/characters/parasite.c" // 0x800DE1CC

void func_800DE26C(void) {}

void func_800DE274(void) // 0x800DE274
{
    s32              diff;
    s32              j;
    s32              i;
    s32              range;
    s32              square;
    s32              temp;
    u8*              curPtr;
    s_func_800DE274* ptr;

    ptr = UNK_FS_BUFFER_6;

    range  = 64;
    curPtr = UNK_FS_BUFFER_6->field_0;
    for (i = -range; i <= range; i++)
    {
        square = SQUARE(i);

        for (j = -range; j <= range; j++, curPtr++)
        {
            diff = range - SquareRoot0(square + SQUARE(j));
            if (diff > 0)
            {
                temp    = (diff << 8) / range;
                temp    = MIN(255, temp);
                *curPtr = temp;
            }
            else
            {
                *curPtr = 0;
            }
        }
    }

    ptr->field_4318 = 0;
}

s32 func_800DE350(s32 arg0) // 0x800DE350
{
    s32 idx;

    idx = UNK_FS_BUFFER_6->field_4101[arg0];

    return D_800EAF20[idx] | 0x3A000000;
}

void* func_800DE380(GsOT_TAG* arg0, PACKET* arg1) // 0x800DE380
{
    s32       i;
    s32       temp_s0;
    s32       temp_s4;
    s32       var_a1;
    s32       j;
    s32       var_s7;
    POLY_G4*  poly;
    DR_TPAGE* tPage;

    poly = arg1;

    for (i = 1; i < 19; i++)
    {
        var_s7 = func_800DE350(28 * (i - 1));
        var_a1 = func_800DE350(28 * i);

        for (j = 1; j < 28; j++)
        {
            temp_s4 = var_a1;
            temp_s0 = var_s7;

            var_s7 = func_800DE350(j + (28 * (i - 1)));
            var_a1 = func_800DE350(j + (28 * i));

            setPolyG4(poly);

            poly->x0 = -178 + (12 * j);
            poly->y0 = ((i - 1) * 12) - 112;
            poly->x1 = -166 + 12 * j;
            poly->y1 = ((i - 1) * 12) - 112;
            poly->x2 = -178 + (12 * j);
            poly->y2 = ((i - 1) * 12) - 100;
            poly->x3 = -166 + (12 * j);
            poly->y3 = ((i - 1) * 12) - 100;

            *(s32*)&poly->r0 = temp_s0;
            *(s32*)&poly->r1 = var_s7;
            *(s32*)&poly->r2 = temp_s4;
            *(s32*)&poly->r3 = var_a1;

            if ((temp_s0 << 8) != 0 || (var_s7 << 8) != 0 || (temp_s4 << 8) != 0 || (var_a1 << 8) != 0)
            {
                addPrim(arg0, poly);
            }

            poly++;
        }
    }

    tPage = (DR_TPAGE*)poly;
    setDrawTPage(tPage, 0, 1, getTPageN(0, 1, 5, 0));
    addPrim(arg0, tPage);

    return (PACKET*)(tPage + 1);
}

void func_800DE5CC(void) // 0x800DE5CC
{
    GsOUT_PACKET_P = func_800DE380(&g_OrderingTable0[g_ActiveBufferIdx].org[UNK_FS_BUFFER_6->field_4318], GsOUT_PACKET_P);
}

void func_800DE62C(void) // 0x800DE62C
{
    memset(UNK_FS_BUFFER_6->field_4101, 0, ARRAY_SIZE(UNK_FS_BUFFER_6->field_4101));
}

void func_800DE658(s_func_800DE274* arg0, q23_8 arg1, q23_8 arg2, q23_8 arg3, q23_8 arg4) // 0x800DE658
{
    s32 temp_a3;
    s32 temp_t9;
    s32 temp_v1;
    s32 var_a0_2;
    s32 j;
    s32 var_t0;
    s32 var_t2;
    s32 i;
    s32 var_t7;
    s32 var_t8;
    u8* ptr1;
    s32 idx;
    u8* ptr0;
    s32 temp_s2;
    s32 temp_s0;
    s32 temp;

    ptr0 = UNK_FS_BUFFER_6->field_4101;

    arg1 = arg1 + Q8(0.65f);
    arg2 = arg2 + 112;

    if (arg3 == Q8(0.0f))
    {
        return;
    }

    arg3   = Q12_MULT(arg3, arg4);
    var_t0 = arg2 - arg3;

    if (var_t0 > Q8(7.0f / 8.0f))
    {
        return;
    }

    if (var_t0 < 0)
    {
        var_a0_2 = -var_t0;
        var_t2   = (var_a0_2 * 64) / arg3;
        var_t2   = FP_TO(var_t2, Q12_SHIFT);
        var_t0   = 0;
    }
    else
    {
        var_t2 = ((-var_t0 % 12) << 6) / arg3;
        var_t2 = FP_TO(var_t2, Q12_SHIFT);
    }

    temp   = var_t0 / 12;
    var_t0 = arg2 + arg3;

    if (var_t0 < 0)
    {
        return;
    }

    temp_t9 = MIN(var_t0, Q8(7.0f / 8.0f)) / 12;
    var_t0  = arg1 - arg3;

    if (var_t0 > Q8(1.3f))
    {
        return;
    }

    if (var_t0 < 0)
    {
        var_a0_2 = -var_t0;
        var_t7   = (var_a0_2 * 64) / arg3;
        var_t7   = FP_TO(var_t7, Q12_SHIFT);
        var_t0   = 0;
    }
    else
    {
        var_t7 = ((((var_t0 / 12) * 12) - var_t0) * 64) / arg3;
        var_t7 = FP_TO(var_t7, Q12_SHIFT);
    }

    temp_s2 = var_t0 / 12;
    var_t0  = arg1 + arg3;

    if (var_t0 < 0)
    {
        return;
    }

    var_a0_2 = MIN(var_t0, Q8(1.3f));
    temp_a3  = 0x300000 / arg3;
    var_t8   = var_t2;
    temp_v1  = var_a0_2 / 12;
    temp_s0  = temp_a3;

    for (i = temp; i <= temp_t9; i++)
    {
        var_t0  = MAX(FP_FROM(var_t8, Q12_SHIFT), 0);
        var_t8 += temp_s0;
        ptr1    = &arg0->field_0[var_t0 * 129];
        var_t2  = var_t7;

        for (j = temp_s2; j <= temp_v1; j++)
        {
            idx       = j + i * 0x1C;
            arg1      = ptr0[idx];
            var_t0    = MAX(FP_FROM(var_t2, Q12_SHIFT), 0);
            var_a0_2  = ptr1[var_t0];
            var_a0_2  = Q12_MULT(var_a0_2, arg4);
            var_t2   += temp_a3;
            arg1     += var_a0_2;
            var_a0_2  = MIN(arg1, 0xFF);
            ptr0[idx] = var_a0_2;
        }
    }
}

void func_800DE8F0(MATRIX* mat, SVECTOR* arg1, SVECTOR3* result) // 0x800DE8F0
{
    s32 sxy;
    s32 unused;

    SetRotMatrix(mat);
    SetTransMatrix(mat);

    result->vz = RotTransPers(arg1, &sxy, &unused, &unused);
    result->vx = sxy;
    result->vy = sxy >> 16;
}

void func_800DE95C(void) // 0x800DE95C
{
    SVECTOR3         sp18;
    s32              temp_a0;
    s32              i;
    s_800ED848*      ptr0;
    s_func_800DE274* ptr1;

    ptr0 = D_800ED848;
    ptr1 = UNK_FS_BUFFER_6;

    func_800DE62C();

    for (i = 0; i < 16; i++, ptr0++)
    {
        if (ptr0->field_0 != 0 && (ptr0->field_24 == NULL || ptr0->field_24(ptr0)))
        {
            func_800DE8F0(&ptr0->field_28, &ptr0->field_1C, &sp18);
            temp_a0 = Q12(1.0f) - (sp18.vz * 4);
            func_800DE658(ptr1, sp18.vx, sp18.vy, Q12_MULT_PRECISE(ptr0->field_8 >> 8, temp_a0) >> 4, ptr0->field_10);
        }
    }
}

s_800ED848* func_800DEA4C(void) // 0x800DEA4C
{
    s_800ED848* ptr;
    s32         i;

    ptr = &D_800ED848;

    for (i = 0; i < 16; i++)
    {
        if (ptr->field_0 == 0)
        {
            ptr->field_0  = 1;
            ptr->field_5C = Q12(1.0f);
            return ptr;
        }

        ptr++;
    }

    return NULL;
}

void func_800DEA94(s_800ED848* arg0) // 0x800DEA94
{
    if (arg0 != NULL)
    {
        arg0->field_0 = 0;
    }
}

void func_800DEAA8(s32 arg0) // 0x800DEAA8
{
    s32         i;
    s_800ED848* ptr;

    ptr = &D_800ED848;

    for (i = 0; i < 16; i++)
    {
        if (ptr->field_0 != 0 && ptr->field_4 == arg0)
        {
            func_800DEA94(ptr);
        }

        ptr++;
    }
}

s32 func_800DEB1C(s_800ED848* arg0) // 0x800DEB1C
{
    SVECTOR sp10;
    VECTOR  sp18;

    sp10.vx = Q12_TO_Q8(arg0->field_14->vx);
    sp10.vy = Q12_TO_Q8(arg0->field_14->vy);
    sp10.vz = Q12_TO_Q8(arg0->field_14->vz);

    SetRotMatrix(&GsWSMATRIX);
    SetTransMatrix(&GsWSMATRIX);
    ApplyRotMatrix(&sp10, &sp18);

    sp18.vx += GsWSMATRIX.t[0];
    sp18.vy += GsWSMATRIX.t[1];
    sp18.vz += GsWSMATRIX.t[2];

    TransMatrix(&arg0->field_28, &sp18);
    SetTransMatrix(&arg0->field_28);
    Math_RotMatrixZxyNeg(arg0->field_18, &arg0->field_28);
    MulRotMatrix(&arg0->field_28);

    arg0->field_8 += Q12(1.0f);
    arg0->field_8  = CLAMP_HIGH(arg0->field_C, arg0->field_8);

    return 1;
}

void func_800DEC3C(VECTOR3* arg0, SVECTOR3* arg1) // 0x800DEC3C
{
    s_800CB69C  sp10[1];
    s32         i;
    s_800ED848* ptr;

    sp10[0] = D_800CB69C;

    for (i = 0; i <= 0; i++)
    {
        ptr = func_800DEA4C();

        if (ptr != NULL)
        {
            ptr->field_14 = arg0;
            ptr->field_18 = arg1;
            ptr->field_1C = sp10[i].field_0;
            ptr->field_8  = Q12(4.0f);
            ptr->field_C  = sp10[i].field_8;
            ptr->field_10 = sp10[i].field_C;
            ptr->field_24 = &func_800DEB1C;
            ptr->field_4  = 0;
        }
    }
}

void func_800DED30(void) // 0x800DED30
{
    func_800DEAA8(0);
}

void func_800DED50(MATRIX* outMat, GsCOORDINATE2* rootCoord, s32 scale) // 0x800DED50
{
    MATRIX transformMat;
    VECTOR scaleVec;

    Vw_CoordHierarchyMatrixCompute(rootCoord, &transformMat);

    scaleVec.vx = scale;
    scaleVec.vy = scale;
    scaleVec.vz = scale;

    transformMat.t[0] -= D_800C3868.t[0];
    transformMat.t[1] -= D_800C3868.t[1];
    transformMat.t[2] -= D_800C3868.t[2];

    ScaleMatrix(&transformMat, &scaleVec);
    Vw_MultiplyAndTransformMatrix(&VbWvsMatrix, &transformMat, outMat);
}

bool func_800DEDEC(s_800ED848* arg0) // 0x800DEDEC
{
    switch (arg0->field_48)
    {
        case 0:
            func_800DED50(&arg0->field_28, &arg0->field_58[1], arg0->field_5C);

            arg0->field_5C += 80;
            arg0->field_8   = arg0->field_60 + Q12_MULT_PRECISE(arg0->field_5C, arg0->field_C);
            arg0->field_10 += 30;
            arg0->field_10  = CLAMP_HIGH(arg0->field_64, arg0->field_10);

            if (*arg0->field_4C > Q12(350.0f))
            {
                arg0->field_48++;
            }
            break;

        case 1:
            func_800DED50(&arg0->field_28, &arg0->field_58[1], arg0->field_5C);

            arg0->field_5C -= 200;
            arg0->field_5C  = MAX(arg0->field_5C, 0);
            arg0->field_8   = arg0->field_60 + Q12_MULT_PRECISE(arg0->field_5C, arg0->field_C);
            arg0->field_10 -= 30;
            arg0->field_10  = MAX(arg0->field_10, 0);
            break;
    }

    return true;
}

void func_800DEF50(VECTOR3* arg0, GsCOORDINATE2* arg1, s32* arg2) // 0x800DEF50
{
    s_800CB6AC       sp10[6];
    s32              idx;
    s_800ED848*      ptr;
    s_func_800DE274* ptr1;
    s32              i;

    memcpy(&sp10, &D_800CB6AC, sizeof(D_800CB6AC));

    ptr1 = UNK_FS_BUFFER_6;

    for (i = 0; i < ARRAY_SIZE(sp10); i++)
    {
        ptr = func_800DEA4C();

        if (ptr != NULL)
        {
            ptr->field_14 = arg0;
            ptr->field_58 = arg1;

            idx = sp10[i].field_0;

            if (idx != 0)
            {
                // TODO: Not Q12?
                ptr->field_1C.vx = Q12_MULT_PRECISE(arg1[idx].coord.t[0], Q12(3.0f));
                ptr->field_1C.vy = Q12_MULT_PRECISE(arg1[idx].coord.t[1], Q12(3.0f));
                ptr->field_1C.vz = Q12_MULT_PRECISE(arg1[idx].coord.t[2], Q12(3.0f));
                ptr->field_60    = Q12(40.0f);
            }
            else
            {
                ptr->field_1C.vx = 0;
                ptr->field_1C.vy = 0;
                ptr->field_1C.vz = 0;
                ptr->field_60    = Q12(100.0f);
            }

            ptr->field_C     = sp10[i].field_4;
            ptr->field_8     = ptr->field_C;
            ptr->field_10    = sp10[i].field_8;
            ptr->field_64    = sp10[i].field_C;
            ptr->field_24    = &func_800DEDEC;
            ptr->field_4     = 1;
            ptr1->field_4318 = 0;
            ptr->field_5C    = 0;
            ptr->field_4C    = arg2;
            ptr->field_48    = 0;
        }
    }
}

void func_800DF134(void) // 0x800DF134
{
    func_800DEAA8(1);
    D_800ED588++;
}

void func_800DF160(MATRIX* arg0, VECTOR3* arg1, VECTOR3* arg2) // 0x800DF160
{
    MATRIX sp10;
    MATRIX sp30;
    s32    temp_s1;
    s32    temp_s2;
    s32    temp_s3;
    s32    temp_s4;
    s32    temp_s5;
    s32    temp_s6;
    s32    temp_v0;
    s32    temp_v0_2;
    s32    var_a0;
    s32    var_a1;
    s32    var_a2;
    s32    var_s1;
    s32    var_s2;

    temp_s2 = arg2->vx - arg1->vx;
    temp_s3 = arg2->vy - arg1->vy;
    temp_s1 = arg2->vz - arg1->vz;

    temp_s4 = Q12_MULT(temp_s2, temp_s2);
    temp_s5 = Q12_MULT(temp_s1, temp_s1);
    temp_s6 = Q12_MULT(temp_s3, temp_s3);

    temp_v0 = SquareRoot12(temp_s4 + temp_s5);

    if (temp_v0 != 0)
    {
        var_s2 = Q12_DIV(temp_s2, temp_v0);
        var_s1 = Q12_DIV(temp_s1, temp_v0);
    }
    else
    {
        var_s2 = 0;
        var_s1 = Q12(1.0f);
    }

    var_a0    = temp_s6 + temp_s4;
    temp_v0_2 = SquareRoot12(var_a0 + temp_s5);

    if (temp_v0_2 != 0)
    {
        var_a2 = -FP_TO(temp_s3, Q12_SHIFT) / temp_v0_2; // TODO: `Q12_DIV` doesn't match.
        var_a1 = Q12_DIV(temp_v0, temp_v0_2);
    }
    else
    {
        var_a2 = Q12(0.0f);
        var_a1 = Q12(1.0f);
    }

    sp30.m[0][0] = var_s1;
    sp30.m[0][1] = 0;
    sp30.m[0][2] = var_s2;
    sp30.m[1][0] = 0;
    sp30.m[1][1] = Q12(1.0f);
    sp30.m[1][2] = 0;
    sp30.m[2][0] = -var_s2;
    sp30.m[2][1] = 0;
    sp30.m[2][2] = var_s1;

    sp10.m[0][0] = Q12(1.0f);
    sp10.m[0][1] = 0;
    sp10.m[0][2] = 0;
    sp10.m[1][0] = 0;
    sp10.m[1][2] = -var_a2;
    sp10.m[2][0] = 0;
    sp10.m[1][1] = var_a1;
    sp10.m[2][2] = var_a1;
    sp10.m[2][1] = var_a2;

    MulMatrix0(&sp30, &sp10, arg0);
}

void func_800DF2F0(MATRIX* arg0, VECTOR3* arg1, VECTOR3* arg2) // 0x800DF2F0
{
    SVECTOR rot;
    VECTOR  sp18;
    MATRIX  sp28;

    rot.vx = Q12_TO_Q8(arg1->vx);
    rot.vy = Q12_TO_Q8(arg1->vy);
    rot.vz = Q12_TO_Q8(arg1->vz);

    SetRotMatrix(&GsWSMATRIX);
    SetTransMatrix(&GsWSMATRIX);
    ApplyRotMatrix(&rot, &sp18);

    sp18.vx += GsWSMATRIX.t[0];
    sp18.vy += GsWSMATRIX.t[1];
    sp18.vz += GsWSMATRIX.t[2];

    TransMatrix(arg0, &sp18);
    func_800DF160(arg0, arg1, arg2);

    rot.vx = Q12_ANGLE(0.0f);
    rot.vy = Q12_ANGLE(0.0f);
    rot.vz = Q12_ANGLE(75.0f);

    Math_RotMatrixXyz(&rot, &sp28);
    MulMatrix(arg0, &sp28);
    SetRotMatrix(&GsWSMATRIX);
    MulRotMatrix(arg0);
    SetRotMatrix(arg0);
    SetTransMatrix(arg0);
}

bool func_800DF41C(s_800ED848* arg0) // 0x800DF41C
{
    s32              i;
    s32              var_v1;
    MATRIX*          mat;
    s_func_800DE274* ptr0;

    ptr0 = UNK_FS_BUFFER_6;

    func_800DF2F0(&arg0->field_28, arg0->field_14, arg0->field_68);
    SetRotMatrix(&arg0->field_28);
    SetTransMatrix(&arg0->field_28);

    arg0->field_8 += 30;

    for (i = 0; i < 50; i++)
    {
        var_v1 = 332;
        func_800DE658(ptr0, ((Rng_Rand16() % 332) / 4) + (arg0->field_8 - var_v1), ((Rng_Rand16() % 224) / 2) - 56, (Rng_Rand16() % 64) + 10, Rng_Rand16() % 2048);
    }

    return false;
}

void func_800DF5B0(VECTOR3* arg0, VECTOR3* arg1) // 0x800DF5B0
{
    s_800ED848* ptr;

    ptr = func_800DEA4C();

    if (ptr != NULL)
    {
        ptr->field_24 = &func_800DF41C;
        ptr->field_14 = arg0;
        ptr->field_68 = arg1;
        ptr->field_8  = 0;
        ptr->field_4  = 2;

        func_800DF2F0(&ptr->field_28, ptr->field_14, ptr->field_68);
    }
}

void func_800DF618(void) // 0x800DF618
{
    func_800DEAA8(2);
    func_800DE274();

    D_800ED588++;
}

void func_800DF64C(void) // 0x800DF64C
{
    s32         i;
    s_800ED848* ptr;

    ptr = D_800ED848;

    for (i = 15; i >= 0; i--)
    {
        ptr->field_0 = 0;
        ptr++;
    }
}

s32 func_800DF670(s32 arg0) // 0x800DF670
{
    s32 temp_a0;
    s32 var_a0;

    var_a0  = UNK_FS_BUFFER_6->field_4101[arg0];
    temp_a0 = (var_a0 > 100) ? (200 - var_a0) : var_a0;
    var_a0  = MAX(temp_a0, 0);

    return D_800EAF20[var_a0] | 0x3A000000;
}

PACKET* func_800DF6C4(GsOT_TAG* ot, PACKET* packet, MATRIX* mat) // 0x800DF6C4
{
    SVECTOR   sp28;
    SVECTOR   sp30;
    SVECTOR   sp38;
    SVECTOR   sp40;
    DVECTOR   sp48[4];
    s32       sp58;
    s32       sp5C;
    s32       temp_s0;
    s32       temp_s1;
    s32       j;
    s32       var_s5;
    s32       var_s6;
    s32       i;
    GsOT_TAG* var_fp;
    POLY_G4*  poly;
    DR_TPAGE* tPage;
    DR_TPAGE* tPage2;

    poly   = packet;
    var_fp = ot;

    for (i = 1; i < 19; i++)
    {
        if (i >= 10)
        {
            var_fp = &ot[0x12C];
        }

        var_s6 = func_800DF670(28 * (i - 1));
        var_s5 = func_800DF670(28 * i);

        for (j = 1; j < 28; j++)
        {
            sp28.vx = (j * D_800EB320) - (D_800EB320 * 14);
            sp28.vy = (i * D_800EB324) - ((D_800EB324 * 19) / 2);
            sp28.vz = 0;

            sp30.vx = ((j + 1) * D_800EB320) - (D_800EB320 * 14);
            sp30.vy = sp28.vy;
            sp30.vz = 0;

            sp38.vx = sp28.vx;
            sp38.vy = ((i + 1) * D_800EB324) - ((D_800EB324 * 19) / 2);
            sp38.vz = 0;

            sp40.vx = sp30.vx;
            sp40.vy = sp38.vy;
            sp40.vz = 0;

            RotTransPers4(&sp28, &sp30, &sp38, &sp40, &sp48[0], &sp48[1], &sp48[2], &sp48[3], &sp58, &sp5C);

            temp_s0 = var_s6;

            var_s6  = func_800DF670(j + ((i - 1) * 28));
            temp_s1 = var_s5;
            var_s5  = func_800DF670(j + (28 * i));

            setPolyG4(poly);

            *(s32*)&poly->x0 = *(s32*)&sp48[0];
            *(s32*)&poly->x1 = *(s32*)&sp48[1];
            *(s32*)&poly->x2 = *(s32*)&sp48[2];
            *(s32*)&poly->x3 = *(s32*)&sp48[3];

            *(s32*)&poly->r0 = temp_s0;
            *(s32*)&poly->r1 = var_s6;
            *(s32*)&poly->r2 = temp_s1;
            *(s32*)&poly->r3 = var_s5;

            if ((temp_s0 << 8) != 0 ||
                (var_s6 << 8) != 0 ||
                (temp_s1 << 8) != 0 ||
                (var_s5 << 8) != 0)
            {
                addPrim(var_fp, poly);
            }

            poly++;
        }
    }

    tPage  = (DR_TPAGE*)poly;
    tPage2 = tPage + 1;

    setDrawTPage(tPage, 0, 1, getTPageN(0, 2, 5, 0));
    addPrim(ot, tPage);

    setDrawTPage(tPage2, 0, 1, getTPageN(0, 2, 5, 0));
    addPrim(var_fp, tPage2);

    return (PACKET*)(tPage + 2);
}

void func_800DFA38(MATRIX* rotMat) // 0x800DFA38
{
    SVECTOR rot;
    VECTOR  sp18;

    rot = D_800EB328;

    SetRotMatrix(&GsWSMATRIX);
    SetTransMatrix(&GsWSMATRIX);
    ApplyRotMatrix(&rot, &sp18);

    sp18.vx += GsWSMATRIX.t[0];
    sp18.vy += GsWSMATRIX.t[1];
    sp18.vz += GsWSMATRIX.t[2];

    TransMatrix(rotMat, &sp18);
    SetTransMatrix(rotMat);

    rot = D_800EB330;

    Math_RotMatrixZxyNeg(&rot, rotMat);
    SetMulRotMatrix(rotMat);
}

void func_800DFB44(void) // 0x800DFB44
{
    MATRIX           mat;
    s_func_800DE274* base;

    base = UNK_FS_BUFFER_6;

    func_800DFA38(&mat);
    GsOUT_PACKET_P = func_800DF6C4(&g_OrderingTable0[g_ActiveBufferIdx].org[base->field_4318], GsOUT_PACKET_P, &mat);
}

bool func_800DFBB0(s_800ED848* arg0) // 0x800DFBB0
{
    s_func_800DE274* ptr;

    ptr = UNK_FS_BUFFER_6;

    switch (arg0->field_48)
    {
        case 0:
            if (*arg0->field_4C > Q12(118.0f))
            {
                Sfx_WithFlagsPlay(Sfx_Unk1637, NULL, 0xFF, SfxFlag_Mono | SfxFlag_NoDistAtten);
                func_800892A4(8);

                D_800EBB4C = 10;
                D_800EBB50 = Q12(3.0f);
                arg0->field_48++;
            }
            break;

        case 1:
            func_800DE658(ptr, -108, -20, D_800EBB4C, Q12(1.0f));

            D_800EBB4C += FP_FROM(D_800EBB50, Q12_SHIFT);
            D_800EBB50 += Q12(1.5f);
            break;

        case 2:
            break;
    }

    return false;
}

void func_800DFC94(s32* arg0) // 0x800DFC94
{
    s_800ED848* ptr;

    ptr = func_800DEA4C();

    func_800DE274();

    if (ptr != NULL)
    {
        ptr->field_8  = Q12(4.0f);
        ptr->field_10 = Q12(0.4f);
        ptr->field_24 = func_800DFBB0;
        ptr->field_48 = 0;
        ptr->field_4C = arg0;
        ptr->field_4  = 4;
    }

    D_800ED588++;
}

void func_800DFD08(void) // 0x800DFD08
{
    func_800DEAA8(4);
    func_800DE274();
    D_800ED588++;
}

void* func_800DFD3C(GsOT_TAG* ot, PACKET* packet, MATRIX* mat, s32 arg3, s32 arg4) // 0x800DFD3C
{
    SVECTOR   sp28;
    SVECTOR   sp30;
    SVECTOR   sp38;
    SVECTOR   sp40;
    DVECTOR   sp48[4];
    s32       sp58;
    s32       sp5C;
    s32       i;
    s32       temp_s0;
    s32       temp_s1;
    s32       j;
    s32       var_s5;
    s32       var_s6;
    GsOT_TAG* sp64;
    POLY_G4*  poly;
    DR_TPAGE* tPage;
    DR_TPAGE* tPage2;

    poly = packet;
    sp64 = ot;

    for (i = 1; i < 19; i++)
    {
        if (i > 9)
        {
            sp64 = &ot[300];
        }

        j = 1;

        var_s6 = func_800DE350((i - 1) * 28);
        var_s5 = func_800DE350(i * 28);

        sp28.vx = (arg4 * i) - ((arg4 * 19) / 2);
        sp28.vy = 0;

        sp30.vx = sp28.vx;
        sp30.vy = 0;

        sp38.vx = (arg4 * (i + 1)) - ((arg4 * 19) / 2);
        sp38.vy = 0;

        sp40.vx = sp38.vx;
        sp40.vy = 0;

        for (; j < 28; j++)
        {
            sp28.vz = (arg3 * j) - (arg3 * 7);
            sp30.vz = (arg3 * (j + 1)) - (arg3 * 7);
            sp40.vz = sp30.vz;
            sp38.vz = sp28.vz;

            RotTransPers4(&sp28, &sp30, &sp38, &sp40, &sp48[0], &sp48[1], &sp48[2], &sp48[3], &sp58, &sp5C);

            temp_s0 = var_s6;
            var_s6  = func_800DE350(j + ((i - 1) * 28));
            temp_s1 = var_s5;
            var_s5  = func_800DE350(j + (i * 28));

            setPolyG4(poly);

            *(s32*)&poly->x0 = *(s32*)&sp48[0];
            *(s32*)&poly->x1 = *(s32*)&sp48[1];
            *(s32*)&poly->x2 = *(s32*)&sp48[2];
            *(s32*)&poly->x3 = *(s32*)&sp48[3];

            *(s32*)&poly->r0 = temp_s0;
            *(s32*)&poly->r1 = var_s6;
            *(s32*)&poly->r2 = temp_s1;
            *(s32*)&poly->r3 = var_s5;

            if ((temp_s0 << 8) != 0 ||
                (var_s6 << 8) != 0 ||
                (temp_s1 << 8) != 0 ||
                (var_s5 << 8) != 0)
            {
                addPrim(sp64, poly);
            }

            poly++;
        }
    }

    tPage  = (DR_TPAGE*)poly;
    tPage2 = tPage + 1;

    setDrawTPage(tPage, 0, 1, getTPageN(0, 1, 5, 0));
    addPrim(ot, tPage);

    setDrawTPage(tPage2, 0, 1, getTPageN(0, 1, 5, 0));
    addPrim(sp64, tPage2);

    return (PACKET*)(tPage + 2);
}

void func_800E00F4(void) // 0x800E00F4
{
    MATRIX mat;

    GsOUT_PACKET_P = func_800DFD3C(&g_OrderingTable0[g_ActiveBufferIdx].org[UNK_FS_BUFFER_6->field_4318], GsOUT_PACKET_P, &mat, 0x100, 7);
}

void func_800E0164(void) // 0x800E0164
{
    s32 val;
    s32 i;
    s32 temp2;
    u8* ptr;
    s32 temp;

    ptr = UNK_FS_BUFFER_6->field_4101;

    for (i = 0; i < ARRAY_SIZE(UNK_FS_BUFFER_6->field_4101); i++)
    {
        val = *ptr;

        if (val != 0)
        {
            temp = Rng_Rand16();

            temp2 = val - 2;
            val   = temp2 + ((temp & 0x30) >> 4);

            if (val < 0)
            {
                val = 0;
            }

            if (val < 0x100)
            {
                temp2 = val;
            }
            else
            {
                temp2 = 0xFF;
            }

            *ptr = temp2;
        }

        ptr++;
    }
}

void func_800E01F4(void) // 0x800E01F4
{
    D_800ED588 = 6;
}

const s_800CB69C D_800CB69C = { { 0, 0, 0, 0 }, Q12(110.0f), Q12(0.5f) };

const s_800CB6AC D_800CB6AC[6] = {
    { 0,  Q12(180.0f), Q12(0.5f),  Q12(0.7f)  },
    { 6,  Q12(180.0f), Q12(0.05f), Q12(0.25f) },
    { 7,  Q12(180.0f), Q12(0.05f), Q12(0.25f) },
    { 8,  Q12(180.0f), Q12(0.05f), Q12(0.25f) },
    { 9,  Q12(180.0f), Q12(0.05f), Q12(0.25f) },
    { 10, Q12(180.0f), Q12(0.05f), Q12(0.25f) },
};

void func_800E0204(void) // 0x800E0204
{
    if (D_800ED588 == 0)
    {
        if (!Fs_QueueChunksLoad())
        {
            return;
        }

        D_800ED588++;
        func_800DE274();
    }

    switch (D_800ED588)
    {
        case 1:
        case 5:
        case 6:
            break;

        case 2:
            func_800DE95C();
            func_800E0164();
            func_800DFB44();
            break;

        case 3:
            func_800DE95C();
            func_800E0164();
            func_800DE5CC();
            break;

        case 4:
            func_800DE95C();
            func_800E0164();
            func_800E00F4();
            break;
    }
}

void func_800E02E0(void) // 0x800E02E0
{
    D_800ED588 = 0;

    func_800DE26C();
    func_800DF64C();
}

s_func_800E030C* func_800E030C(void) // 0x800E030C
{
    s32              i;
    s_func_800E030C* ptr;

    ptr = (s_func_800E030C*)((u8*)FS_BUFFER_1 + 0x494);

    for (i = 0; i < 800; i++, ptr++)
    {
        if (!ptr->isSlotInUse_24)
        {
            ptr->isSlotInUse_24 = true;
            return ptr;
        }
    }

    return NULL;
}

void func_800E0358(s_func_800E030C* arg0) // 0x800E0358
{
    switch (arg0->field_34)
    {
        case 0:
            if (arg0->field_0 < 0)
            {
                arg0->field_0   = 0;
                arg0->vec_14.vx = 0;
                arg0->vec_14.vy = 0;
                arg0->vec_14.vz = 0;
                arg0->field_34++;
            }
            break;

        case 1:
            if (arg0->field_0 < 0)
            {
                arg0->isSlotInUse_24 = false;
            }
            break;
    }
}

void func_800E03C4(const VECTOR3* from, const VECTOR3* to, q19_12 dist, s32 arg3) // 0x800E03C4
{
    VECTOR           disp; // Q19.12
    VECTOR           dir;  // Q19.12
    s_func_800E030C* ptr;

    ptr = func_800E030C();
    if (ptr != NULL)
    {
        ptr->vec_4.vx = from->vx;
        ptr->vec_4.vy = from->vy;
        ptr->vec_4.vz = from->vz;

        // Compute direction.
        disp.vx = to->vx - from->vx;
        disp.vy = to->vy - from->vy;
        disp.vz = to->vz - from->vz;
        VectorNormal(&disp, &dir);

        ptr->vec_14.vx = Q12_MULT_PRECISE(dir.vx, dist);
        ptr->vec_14.vy = Q12_MULT_PRECISE(dir.vy, dist);
        ptr->vec_14.vz = Q12_MULT_PRECISE(dir.vz, dist);

        ptr->field_0    = Q12_DIV(SquareRoot12(Q12_SQUARE_PRECISE(disp.vx) +
                                               Q12_SQUARE_PRECISE(disp.vy) +
                                               Q12_SQUARE_PRECISE(disp.vz)),
                                  dist);
        ptr->field_28   = Q12(255.0f);
        ptr->field_2C   = arg3 << 8;
        ptr->funcPtr_30 = func_800E0358;
        ptr->field_34   = 0;
    }
}

void func_800E05C8(s32 x, s32 y, s32 val) // 0x800E05C8
{
    s16              col;
    s16              row;
    s_func_800E05C8* ptr;
    u8*              buf;

    ptr = FS_BUFFER_1;

    row = y + 96;
    col = x + 160;

    val >>= 12;

    col += (col > 0) ? 4 : -4;
    col /= 8;

    row += (row > 0) ? 4 : -4;
    row /= 8;

    if (col > 0 && col < 41 && row > 0 && row < 25)
    {
        buf  = ptr->field_5D + (col + (41 * row));
        *buf = val;
    }
}

void func_800E068C(void) // 0x800E068C
{
    SVECTOR          sp10;
    DVECTOR          sp18[2];
    s32              sp20;
    s32              i;
    s_func_800E030C* ptr;
    s_func_800E05C8* base;

    base = FS_BUFFER_1;
    ptr  = base->field_494;

    for (i = 0; i < 800; i++, ptr++)
    {
        if (ptr->isSlotInUse_24)
        {
            if (ptr->funcPtr_30 != NULL)
            {
                ptr->funcPtr_30(ptr);
            }

            sp10.vx = Q12_TO_Q8(ptr->vec_4.vx);
            sp10.vy = Q12_TO_Q8(ptr->vec_4.vy);
            sp10.vz = Q12_TO_Q8(ptr->vec_4.vz);

            RotTransPers(&sp10, &sp18[0], &sp18[1], &sp20);
            func_800E05C8(sp18[0].vx, sp18[0].vy, ptr->field_28);

            ptr->vec_4.vx += Q12_MULT_PRECISE(ptr->vec_14.vx, g_DeltaTime);
            ptr->vec_4.vy += Q12_MULT_PRECISE(ptr->vec_14.vy, g_DeltaTime);
            ptr->vec_4.vz += Q12_MULT_PRECISE(ptr->vec_14.vz, g_DeltaTime);
            ptr->field_28 += Q12_MULT_PRECISE(ptr->field_2C, g_DeltaTime);
            ptr->field_28  = MAX(ptr->field_28, 0);
            ptr->field_28  = MIN(ptr->field_28, 0xFF000);
            ptr->field_0  -= g_DeltaTime;
        }
    }
}

s32 func_800E0878(s32 arg0, s32 arg1) // 0x800E0878
{
    s_func_800E05C8* buf;

    buf = (s_func_800E05C8*)FS_BUFFER_1;

    return D_800EB338[buf->field_5D[(arg1 * 41) + arg0]];
}

void func_800E08B8(void) // 0x800E08B8
{
    u32              sp18;
    s_func_800E05C8* ptr;
    GsOT_TAG*        ot;
    s32              color3;
    s32              color2;
    s32              color1;
    s32              i;
    s32              j;
    s32              color0;
    POLY_G4*         poly;
    DR_MODE*         mode;
    PACKET*          packet;
    s32              col;
    s32              index;
    s32              code;

    ptr = (s_func_800E05C8*)FS_BUFFER_1;

    packet = GsOUT_PACKET_P;
    poly   = packet;

    index = g_ActiveBufferIdx;
    ot    = g_OrderingTable0[index].org;
    ot    = &ot[ptr->field_48C >> 1];

    col  = 0x3A000000;
    code = 0x3A;

    for (i = 1; i < 25; i++)
    {
        color0 = func_800E0878(0, i - 1);
        color1 = func_800E0878(0, i);

        for (j = 1; j < 41; j++)
        {
            sp18 = ptr->field_5D[(i * 41) + j];

            color2 = color1;
            color3 = color0;

            color0 = func_800E0878(j, i - 1);
            color1 = func_800E0878(j, i);

            if (col == color3 &&
                color3 == color0 &&
                color3 == color2 &&
                color2 == color1)
            {
                continue;
            }

            if ((j + i) & 1)
            {
                poly->x0 = -160 + (j * 8);
                poly->y0 = -96 + (i * 8);
                poly->x1 = -152 + (j * 8);
                poly->y1 = -96 + (i * 8);
                poly->x2 = -160 + (j * 8);
                poly->y2 = -88 + (i * 8);
                poly->x3 = -152 + (j * 8);
                poly->y3 = -88 + (i * 8);

                *(s32*)&poly->r0 = color3;
                *(s32*)&poly->r1 = color0;
                *(s32*)&poly->r2 = color2;
                *(s32*)&poly->r3 = color1;
            }
            else
            {
                poly->x1 = -160 + (j * 8);
                poly->y1 = -96 + (i * 8);
                poly->x0 = -152 + (j * 8);
                poly->y0 = -96 + (i * 8);
                poly->x3 = -160 + (j * 8);
                poly->y3 = -88 + (i * 8);
                poly->x2 = -152 + (j * 8);
                poly->y2 = -88 + (i * 8);

                *(s32*)&poly->r0 = color0;
                *(s32*)&poly->r1 = color3;
                *(s32*)&poly->r2 = color1;
                *(s32*)&poly->r3 = color2;
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

void func_800E0BB0(void) // 0x800E0BB0
{
    s32              j;
    s32              i;
    s32              val;
    s_func_800E05C8* buf;
    u8*              tab;

    buf = FS_BUFFER_1;

    for (i = 0; i < 25; i++)
    {
        tab = &buf->field_5D[i * 41];

        for (j = 0; j < 41; j++)
        {
            val = tab[j - 41];

            if (j == 0)
            {
                val = 0;
            }
            else
            {
                val += tab[j - 1];
            }

            if (j == 40)
            {
                val = 0;
            }
            else
            {
                val = val + tab[j + 1];
            }

            val  += tab[j + 41];
            val >>= 2;
            val  -= 1;

            if (val <= 0)
            {
                tab[j] = 0;
            }
            else
            {
                tab[j] = val;
            }
        }
    }
}

void func_800E0C58(void) // 0x800E0C58
{
    s32              i;
    s_func_800E030C* ptr;
    s_func_800E05C8* base;

    base = FS_BUFFER_1;
    ptr  = base->field_494;

    memset(base->field_34, 0, sizeof(base->field_34));
    memset(base->field_5D, 0, sizeof(base->field_5D));
    memset(base->field_45E, 0, sizeof(base->field_45E));

    for (i = 799; i >= 0; i--)
    {
        ptr->isSlotInUse_24 = 0;
        ptr++;
    }
}

void func_800E0CCC(VECTOR* arg0, s32 arg1) // 0x800E0CCC
{
    MATRIX  sp10;
    VECTOR  sp30;
    SVECTOR sp40;

    SetRotMatrix(&GsWSMATRIX);
    SetTransMatrix(&GsWSMATRIX);
    ApplyRotMatrixLV(arg0, &sp30);

    sp30.vx += GsWSMATRIX.t[0];
    sp30.vy += GsWSMATRIX.t[1];
    sp30.vz += GsWSMATRIX.t[2];

    TransMatrix(&sp10, &sp30);
    SetTransMatrix(&sp10);

    sp40.vx = 0;
    sp40.vy = arg1;
    sp40.vz = 0;

    Math_RotMatrixZxyNeg(&sp40, &sp10);
    SetMulRotMatrix(&sp10);
}

void func_800E0D8C(VECTOR3* arg0) // 0x800E0D8C
{
    s_func_800E05C8* buf = FS_BUFFER_1;
    buf->field_4.vx      = arg0->vx >> 4;
    buf->field_4.vy      = arg0->vy >> 4;
    buf->field_4.vz      = arg0->vz >> 4;
    buf->field_490       = 0;
}

void func_800E0DC4(s32 arg0, s32 arg1) // 0x800E0DC4
{
    VECTOR3 sp10;
    VECTOR3 sp20;
    s32     temp_s0;
    s32     temp_s3;
    s32     temp_s4;
    s32     temp_v0;

    temp_v0 = Rng_Rand16();
    temp_s4 = Math_Sin(temp_v0);
    temp_s3 = Math_Cos(temp_v0);
    temp_v0 = Rng_Rand16();
    temp_s0 = Q12_MULT_PRECISE(temp_s3, Math_Sin(temp_v0));
    temp_v0 = Q12_MULT_PRECISE(temp_s3, Math_Cos(temp_v0));

    sp10.vx = 0;
    sp10.vy = 0;
    sp10.vz = 0;

    sp20.vx = Q12_MULT_PRECISE(temp_s0, arg0);
    sp20.vy = Q12_MULT_PRECISE(temp_s4, arg0);
    sp20.vz = Q12_MULT_PRECISE(temp_v0, arg0);

    func_800E03C4(&sp20, &sp10, arg1, 0);
}

s32 func_800E0F28(q19_12 val0, q19_12 val1) // 0x800E0F28
{
    s32 integer;
    s32 fraction;

    integer  = Q12_MULT_PRECISE(val0, val1) / Q12(1.0f);
    fraction = Q12_MULT_PRECISE(val0, val1) % Q12(1.0f);

    if ((Rng_Rand16() / 8) < fraction)
    {
        integer++;
    }

    return integer;
}

const VECTOR3 D_800CB728 = { Q12(2.0f), Q12(-0.3f), Q12(-30.5f) };

void func_800E0FAC(s32 arg0) // 0x800E0FAC
{
    VECTOR3          sp10;
    s32              temp_v0;
    s32              i;
    s_func_800E05C8* ptr;

    ptr = FS_BUFFER_1;

    switch (D_800ED58C)
    {
        case 0:
            if (arg0 > Q12(100.0f))
            {
                sp10 = D_800CB728;
                D_800ED58C++;

                func_800E0C58();
                func_800E0D8C(&sp10);

                D_800EBB54 = 0;

                Sfx_WithFlagsPlay(Sfx_Unk1636, NULL, 0xFF, SfxFlag_Mono | SfxFlag_NoDistAtten);
                break;
            }
            return;

        case 1:
            D_800EBB54 += Q12_MULT_PRECISE(g_DeltaTime, Q12(30.0f));
            temp_v0     = func_800E0F28(D_800EBB54, g_DeltaTime);

            for (i = 0; i < temp_v0; i++)
            {
                func_800E0DC4(Q12(3.0f), Q12(4.0f));
            }
            break;

        case 2:
            break;
    }

    func_800E0CCC(&ptr->field_4, ptr->field_490);

    ptr->field_48C = 0;

    func_800E068C();
    func_800E0BB0();
    func_800E08B8();
}

void func_800E10F8(void) // 0x800E10F8
{
    D_800ED58C = 0;
}

#include "../src/maps/map_util.c" // 0x800E1104

#include "maps/shared/Map_RoomBgmInit_6_s04.h" // 0x800E1258

#include "maps/shared/Map_RoomBgmInit_6_s04_CondTrue.h" // 0x800E1290

#include "maps/shared/Map_RoomBgmInit_6_s04_CondFalse.h" // 0x800E12D0

// TODO: Might be part of shared block above with `map6_s05::func_800CCBFC`
void GameBoot_LoadScreen_StageString(void) {}

#include "maps/shared/Event_CutsceneTimerAdvance.h" // 0x800E1564

void func_800E15FC(s_SubCharacter* player, s_SubCharacter* npc, bool arg2) // 0x800E15FC
{
    VECTOR3 sp18;
    VECTOR3 sp28;
    VECTOR3 sp38;
    q3_12   angle0;
    q19_12  angle5;
    s32     temp_a0_2;
    q19_12  newPosX;
    s32     temp_s0_2;
    s32     temp_s0_3;
    q19_12  temp_s1_2;
    q19_12  angle1;
    q19_12  angle2;
    u32     temp_s5;
    q19_12  angle7;
    u32     temp_v1_2;
    q19_12  angle4;
    q19_12  angle3;
    q19_12  newPosZ;
    s32     temp2;
    u32     temp3;
    q19_12  angle6;

    temp3   = vcWork.flags;
    temp3   = temp3 >> 9;
    temp3   = temp3 & 1;
    temp_s5 = temp3 ^ (g_GameWorkConst->config.extraViewCtrl != false);

    Vc_VectorMagnitudeCalc(player->position.vx - npc->position.vx, Q12(0.0f), player->position.vz - npc->position.vz);
    angle2 = ratan2(player->position.vx - Q12(20.0f), player->position.vz - Q12(100.0f));
    angle1 = angle2 + Q12_ANGLE(90.0f);

    if (temp_s5 != 0)
    {
        angle0 = angle2 + Q12_MULT(Math_Cos(player->rotation.vy - angle1), Q12_ANGLE(-10.0f));
    }
    else
    {
        if (arg2 || ((D_800EBB5A - angle2) << 20) > 0)
        {
            angle3 = Q12_MULT(Math_Cos(player->rotation.vy - angle1), Q12_ANGLE(22.5f)) + Q12_ANGLE(37.5f);
        }
        else
        {
            angle3 = Q12_MULT(Math_Cos(player->rotation.vy - angle1), Q12_ANGLE(22.5f)) - Q12_ANGLE(37.5f);
        }

        angle0 = angle2 + angle3;
    }

    if (arg2)
    {
        D_800EBB58.field_0 = Q12_ANGLE(0.0f);
        D_800EBB58.field_2 = angle0;
    }
    else
    {
        D_800EBB58.field_0  = vwRetNewAngSpdToTargetAng(D_800EBB58.field_0, D_800EBB58.field_2, angle0,
                                                        Q12(0.5f), Q12(0.7f), Q12(5.0f));
        D_800EBB58.field_2 += Q12_MULT_PRECISE(D_800EBB58.field_0, g_DeltaTime);
    }

    sp28.vx = Q12_MULT(Math_Sin(D_800EBB58.field_2), Q12(5.5f)) + Q12(20.0f);
    sp28.vz = Q12_MULT(Math_Cos(D_800EBB58.field_2), Q12(5.5f)) + Q12(100.0f);
    sp28.vy = Q12(-2.4f);

    if (arg2)
    {
        sp38 = sp28;
    }
    else
    {
        vwGetViewPosition(&sp38);
    }

    if (temp_s5 != 0)
    {
        newPosX = player->position.vx + Q12_MULT(Math_Sin(player->rotation.vy), Q12(8.0f));
        newPosZ = player->position.vz + Q12_MULT(Math_Cos(player->rotation.vy), Q12(8.0f));
        angle4  = ratan2(newPosX - sp38.vx, newPosZ - sp38.vz);
    }
    else
    {
        sp38 = sp28;

        temp_s1_2 = Q12(0.4f);
        temp_s1_2 = FP_FROM(FP_TO(Math_Cos(player->rotation.vy - angle2), Q12_SHIFT), Q12_SHIFT) +
                    (Vc_VectorMagnitudeCalc(player->position.vx - Q12(20.0f), Q12(0.0f), player->position.vz - Q12(100.0f)) - temp_s1_2);

        temp_s0_2 = angle2 + Q12_MULT(Math_Cos(player->rotation.vy - angle1), Q12_ANGLE(25.0f));

        temp_s0_3 = Q12_MULT(temp_s1_2, Math_Sin(temp_s0_2)) + Q12(20.0f);

        temp_s0_3 = temp_s0_3 - sp38.vx;
        temp2     = Q12_MULT(temp_s1_2, Math_Cos(temp_s0_2)) + Q12(100.0f);

        angle4 = ratan2(temp_s0_3, temp2 - sp38.vz);

        angle5 = ratan2(sp38.vx - Q12(20.0f), sp38.vz - Q12(100.0f));
        angle6   = angle5 + Q12_ANGLE(90.0f);
        angle7 = Q12_ANGLE_NORM_S(angle4 - angle6);

        if (((D_800EBB5A - angle2) << 20) > 0)
        {
            if (((angle7 - Q12_ANGLE(135.0f)) << 20) < 0)
            {
                angle4 = angle5 + Q12_ANGLE(225.0f);
            }
        }
        else if (((angle7 - Q12_ANGLE(45.0f)) << 20) > 0)
        {
            angle4 = angle5 + Q12_ANGLE(135.0f);
        }
    }

    if (arg2)
    {
        D_800EBB58.field_6 = Q12_ANGLE(0.0f);
        D_800EBB58.field_4 = angle4;
    }
    else
    {
        D_800EBB58.field_6  = vwRetNewAngSpdToTargetAng(D_800EBB58.field_6, D_800EBB58.field_4, angle4,
                                                        Q12(2.0f), Q12(2.0f), Q12(8.0f));
        D_800EBB58.field_4 += Q12_MULT_PRECISE(D_800EBB58.field_6, g_DeltaTime);
    }

    ratan2(player->position.vy + Q12(0.9f), Vc_VectorMagnitudeCalc(player->position.vx - sp28.vx, Q12(0.0f), player->position.vz - sp28.vz));

    sp18.vx = sp28.vx + Q12_MULT(Math_Sin(D_800EBB58.field_4), Q12(3.0f));
    sp18.vz = sp28.vz + Q12_MULT(Math_Cos(D_800EBB58.field_4), Q12(3.0f));

    sp18.vy = -Math_Sin(Q12_ANGLE(-20.0f)) * Q12(3.0f) / Math_Cos(Q12_ANGLE(-20.0f)) - Q12(2.4f);

    if (temp_s5 != 0 && g_GameWorkConst->config.extraViewMode)
    {
        temp_v1_2 = vcWork.flags >> 9;
        temp_v1_2 = temp_v1_2 & 1;

        if (( g_GameWorkConst->config.extraViewCtrl && (temp_v1_2 ^ 1) != 0) ||
            (!g_GameWorkConst->config.extraViewCtrl && temp_v1_2 != 0))
        {
            temp_a0_2 = vcWork.flags >> 10;
            temp_a0_2 = temp_a0_2 & 0x1;

            if (( g_GameWorkConst->config.extraViewCtrl && (temp_a0_2 ^ 1) == 0) ||
                (!g_GameWorkConst->config.extraViewCtrl && temp_a0_2 == 0))
            {
                vcReturnPreAutoCamWork(true);
                return;
            }
        }

        vcReturnPreAutoCamWork(false);
        return;
    }

    vcUserCamTarget(&sp28, NULL, true);
    vcUserWatchTarget(&sp18, NULL, true);
}

void func_800E1CA0(void) // 0x800E1CA0
{
    s32 i;

    for (i = 0; i < 8; i++)
    {
        g_MapOverlayHdr.harryMapAnimInfos[i + 16] = D_800CC424[i];
    }

    for (i = 0; i < 4; i++)
    {
        g_MapOverlayHdr.field_38[i + 8] = D_800CC4A4[i];
    }
}

void func_800E1D48(void) {}

const char* MAP_MESSAGES[] = {
    #include "maps/shared/map_msg_common.h"
#if VERSION_REGION_IS(NTSC)
    /* 15 */ "\tGotta'_go_find_Cybil! ~N\n\tNo_time_to_go_back. ~E ",
    /* 16 */ "~J0(10)\t~E ",
    /* 17 */ "~J0(10)\tCybil! ~E ",
    /* 18 */ "~J0(2.3)\tWake_up!_Snap_out_of_it! ~E ",
    /* 19 */ "~J0(1.0)\tCybil! ~E ",
    /* 20 */ "~J0(4.7)\t...Harry? ~N\n\t\t\t...What...happened? ",
    /* 21 */ "~J0(3.3)\tDon't_talk. ~N\n\t\t\tI'll_take_care_of_you. ~E ",
    /* 22 */ "~J1(4.8)\tHarry... ~N\n\t\t\tWhy_did_they_take_your_daughter? ",
    /* 23 */ "~J1(2.3)\tWhy_her? ",
    /* 24 */ "~J1(3.0)\tI'm_not_sure,_myself. ",
    /* 25 */ "~J1(4.3)\tBut_you_know,_Cheryl_isn't ~N\n\t\t\tmy_biological_daughter. ",
    /* 26 */ "~J1(3.0)\tI_actually_haven't_told_her_yet. ",
    /* 27 */ "~J1(3.3)\tShe_probably_already_knows ~N\n\t\t\tanyway,_though. ~E ",
    /* 28 */ "~J1(3.9)\tWe_found_her_abandoned ~N\n\t\t\ton_the_side_of_the_highway. ",
    /* 29 */ "~J1(3.3)\tNobody_knew ~N\n\t\t\twhere_she_came_from. ",
    /* 30 */ "~J1(3.3)\tWe_didn't_have_any_kids ~N\n\t\t\tof_our_own. ",
    /* 31 */ "~J1(2.4)\tMy_wife_was_sick, ",
    /* 32 */ "~J1(3.4)\tand_it_didn't_look_like ~N\n\t\t\tshe_was_getting_any_better. ",
    /* 33 */ "~J1(2.7)\tSo_we_took_Cheryl_in. ~E ",
    /* 34 */ "~J1(3.1)\tSo_in_that_case... ",
    /* 35 */ "~J1(5.2)\tThere_might_be_some_connection ~N\n\t\t\tbetween_Cheryl_and_this_town. ~E ",
    /* 36 */ "~J1(3.4)\tSo_what_do_you_do_now? ",
    /* 37 */ "~J1(3.5)\tCheryl_is_my_daughter. ~N\n\t\t\tI_will_save_her. ",
    /* 38 */ "~J1(4.5)\tNo_matter_what. ~E ",
    /* 39 */ "~J0(7.3)\t~E ",
    /* 40 */ "~J0(7.0)\t~E ",
    /* 41 */ "~J0(6.0)\tWhat_in_the_hell... ~N\n\t\t\tHow_could_this_have_happened? ~E ",
    /* 42 */ "~J1(1.7)\tI_figured_you'd_show. ",
    /* 43 */ "~J1(1.6)\tHold_it_right_there. ",
    /* 44 */ "~J1(4.0)\tI_don't_know_who_you_are, ~N\n\t\t\twhat_you_are_trying_to_do, ~N\n\t\t\tand_I_don't_care. ",
    /* 45 */ "~J1(1.6)\tJust_one_thing. ",
    /* 46 */ "~J1(3.1)\tLet_Cheryl_go. ~N\n\t\t\tThat's_all_I_ask. ~E ",
    /* 47 */ "~J0(1.0)\tWhat? ~E ",
    /* 48 */ "~J0(1.0)\tDamn! ~E ",
    /* 49 */ "~J0(1.2)\tHuh? ~E ",
    /* 50 */ "~J0(1.5)\tWhat_is_this? ~E ",
    /* 51 */ "~J0(1.4)\t~E ",
    /* 52 */ "~J1(1.6)\tWhere's_Cheryl? ",
    /* 53 */ "~J1(2.0)\tGive_me_back_my_daughter. ~E ",
    /* 54 */ "~J0(3.2)\tWe_meet_at_last,_Alessa. ",
    /* 55 */ "~J0(1.2)\t.......! ~E ",
    /* 56 */ "~J1(1.8)\tDahlia_Gillespie...... ",
    /* 57 */ "~J1(2.0)\tWhere's_Cheryl? ~N\n\t\t\tWhere_is_she!? ~E ",
    /* 58 */ "~J1(1.2)\tAlessa. ",
    /* 59 */ "~J1(4.0)\tThis_is_the_end_of ~N\n\t\t\tyour_little_game. ",
    /* 60 */ "~J1(1.2)\tMama... ",
    /* 61 */ "~J1(2.7)\tHuh? ~N\n\t\t\tCould_she_be...? ~E ",
    /* 62 */ "~J1(4.6)\tYou've_been_a_ghastly_little_pest, ~N\n\t\t\thaven't_you?__Alessa? ",
    /* 63 */ "~J1(2.0)\tI_was_careless. ",
    /* 64 */ "~J1(3.5)\tThinking_you_couldn't_escape ~N\n\t\t\tfrom_our_spell. ",
    /* 65 */ "~J1(4.5)\tBut_Mommy_didn't_know ~N\n\t\t\thow_much_you'd_grown. ",
    /* 66 */ "~J1(4.3)\tThat's_why_I_couldn't_catch_you ~N\n\t\t\tall_by_myself. ",
    /* 67 */ "~J1(2.8)\tBut_what_a_pity,_yes? ",
    /* 68 */ "~J1(5.0)\tNow_you're_half_indebted ~N\n\t\t\tto_this_man_for_his_help. ",
    /* 69 */ "~J1(2.7)\tHey! ~N\n\t\t\tWhat_are_you_talking_about? ",
    /* 70 */ "~J1(6.9)\tAlessa._My_dear_little_girl... ~N\n\t\t\tThere_is_one_thing_left ~N\n\t\t\tI_need_you_to_do_for_me. ~E ",
    /* 71 */ "~J0(2.3)\tNo. ~N\n\t\t\tGet_away_from_me. ",
    /* 72 */ "~J0(1.8)\tBad_girl. ",
    /* 73 */ "~J0(5.1)\tEverything_is_ready. ~N\n\t\t\tLet's_go_home_now. ~E ",
    /* 74 */ "~J0(1.7)\tWhat's_going_on_here!? ~E ",
    /* 75 */ "~J0(1.2)\t~E "
#elif VERSION_REGION_IS(NTSCJ)
    /* 15 */ "\tシビルを探しに行かなくては！ ~E ",
    /* 16 */ "~J0(10)\t~E ",
    /* 17 */ "~J0(10)\tシビル！ ~E ",
    /* 18 */ "~J0(2.3)\tしっかりしろ！ ~E ",
    /* 19 */ "~J0(1.0)\tシビル！ ~E ",
    /* 20 */ "~J0(4.7)\t……ハリー？ ~N\n\t\t\tあたし……どうしたのかしら…… ",
    /* 21 */ "~J0(3.3)\tしゃべるな　すぐに傷の手当てをする ~E ",
    /* 22 */ "~J1(4.8)\tハリー……　あなたの娘は ~N\n\t\t\tなぜ連れ去られたのかしら…… ",
    /* 23 */ "~J1(2.3)\t何があるというの……？ ",
    /* 24 */ "~J1(3.0)\tそれはわたしにもわからない ",
    /* 25 */ "~J1(4.3)\tただ……わたしとシェリルは ~N\n\t\t\t血がつながっていないんだ ",
    /* 26 */ "~J1(3.0)\tシェリルにはまだ教えていない…… ",
    /* 27 */ "~J1(3.3)\tいや　もう知っているかもしれないな ~E ",
    /* 28 */ "~J1(3.9)\tハイウェイの近くで見つけたんだ ",
    /* 29 */ "~J1(3.3)\t身元はまったくわからなかった ",
    /* 30 */ "~J1(3.3)\tわたしたちには子供がいなかった ",
    /* 31 */ "~J1(2.4)\t妻の持病が悪化しつつあって…… ",
    /* 32 */ "~J1(3.4)\t子供を産めるような状態では ~N\n\t\t\tなかったからだ ",
    /* 33 */ "~J1(2.7)\tそしてわたしたちはシェリルを引き取った ~E ",
    /* 34 */ "~J1(3.1)\tそれじゃ…… ",
    /* 35 */ "~J1(5.2)\tシェリルと　この町で起こったことと ~N\n\t\t\t何かつながりがあるのかもしれない ~E ",
    /* 36 */ "~J1(3.4)\tどうするの……？ ",
    /* 37 */ "~J1(3.5)\tシェリルはわたしの娘だ　絶対に助け出す ",
    /* 38 */ "~J1(4.5)\tそれだけだ ~E ",
    /* 39 */ "~J0(7.3)\t~E ",
    /* 40 */ "~J0(7.0)\t~E ",
    /* 41 */ "~J0(6.0)\t何ということだ…… ~N\n\t\t\tどうしてこんなことになってしまうんだ…… ~E ",
    /* 42 */ "~J1(1.7)\tやはり　現れたな…… ",
    /* 43 */ "~J1(1.6)\tここを通すわけにはいかない ",
    /* 44 */ "~J1(4.0)\tおまえが何者なのか ~N\n\t\t\t何をするつもりなのか ~N\n\t\t\tそんなことはどうでもいいんだ ",
    /* 45 */ "~J1(1.6)\t一つだけだ ",
    /* 46 */ "~J1(3.1)\tシェリルを返してくれ ~N\n\t\t\tそれだけでいい ~E ",
    /* 47 */ "~J0(1.0)\t何だ？ ~E ",
    /* 48 */ "~J0(1.0)\tくそ！ ~E ",
    /* 49 */ "~J0(1.2)\t何？ ~E ",
    /* 50 */ "~J0(1.5)\tこれは……？ ~E ",
    /* 51 */ "~J0(1.4) ~E ",
    /* 52 */ "~J1(1.6)\tシェリルを…… ",
    /* 53 */ "~J1(2.0)\t娘を返してくれ…… ~E ",
    /* 54 */ "~J0(3.2)\tやっと会えたわね　アレッサ ",
    /* 55 */ "~J0(1.2)\t…………！ ~E ",
    /* 56 */ "~J1(1.8)\tダリア・ギレスピー…… ",
    /* 57 */ "~J1(2.0)\tこれでシェリルに会えるんだろう！ ~N\n\t\t\tどこにいるんだ！？ ~E ",
    /* 58 */ "~J1(1.2)\tアレッサ ",
    /* 59 */ "~J1(4.0)\tこれで悪ふざけはおしまいよ ",
    /* 60 */ "~J1(1.2)\tママ ",
    /* 61 */ "~J1(2.7)\t何だって？ ~N\n\t\t\tどういうことだ？ ~E ",
    /* 62 */ "~J1(4.6)\tずいぶん　小ざかしいことをしてくれたわね ~N\n\t\t\tアレッサ？ ",
    /* 63 */ "~J1(2.0)\t油断していたわ ",
    /* 64 */ "~J1(3.5)\tあたしたちの呪縛から ~N\n\t\t\t逃げ出せるとは思っていなかったわ ",
    /* 65 */ "~J1(4.5)\tあなたがそんなに成長していたなんて ~N\n\t\t\tママ　知らなかったの ",
    /* 66 */ "~J1(4.3)\tおかげで　あたしだけでは ~N\n\t\t\t捕まえることができなかったわ ",
    /* 67 */ "~J1(2.8)\tでも　残念だったわね ",
    /* 68 */ "~J1(5.0)\tあなたの半分がこの男に ~N\n\t\t\t助けを求めてしまって ",
    /* 69 */ "~J1(2.7)\tおい　何を言ってるんだ？ ",
    /* 70 */ "~J1(6.9)\tアレッサ　あたしのかわいい娘…… ~N\n\t\t\tあなたにはまだやってもらわなくては ~N\n\t\t\tいけないことが残っているのよ ~E ",
    /* 71 */ "~J0(2.3)\tいや！ ~N\n\t\t\t近寄らないで ",
    /* 72 */ "~J0(1.8)\t悪い子ね ",
    /* 73 */ "~J0(5.1)\t準備はできているのよ ~N\n\t\t\tさあ　帰りましょう ~E ",
    /* 74 */ "~J0(1.7)\t何だ！？ ~E ",
    /* 75 */ "~J0(1.2)\t~E "
#endif
};

const s_AnimInfo D_800CC424[8] = {
    { Anim_BlendLinear, ANIM_STATUS(46, false), false, ANIM_STATUS(46, true), { Q12(10) }, NO_VALUE, 805 },
    { Anim_PlayOnce, ANIM_STATUS(46, true), false, ANIM_STATUS(46, true), { Q12(18) }, 805, 819 },
    { Anim_BlendLinear, ANIM_STATUS(47, false), false, ANIM_STATUS(47, true), { Q12(10) }, NO_VALUE, 820 },
    { Anim_PlayOnce, ANIM_STATUS(47, true), false, ANIM_STATUS(47, true), { Q12(18) }, 820, 834 },
    { Anim_BlendLinear, ANIM_STATUS(48, false), false, ANIM_STATUS(48, true), { Q12(10) }, NO_VALUE, 835 },
    { Anim_PlayOnce, ANIM_STATUS(48, true), false, ANIM_STATUS(48, true), { Q12(30) }, 835, 868 },
    { Anim_BlendLinear, ANIM_STATUS(49, false), false, ANIM_STATUS(49, true), { Q12(10) }, NO_VALUE, 869 },
    { Anim_PlayOnce, ANIM_STATUS(49, true), false, ANIM_STATUS(49, true), { Q12(30) }, 869, 902 },
};

const s_UnkStruct3_Mo D_800CC4A4[4] = {
    { 0x5D5C, 0x00D6, 0x0325, 0x0333 },
    { 0x5F5E, 0x00D7, 0x0334, 0x0342 },
    { 0x6160, 0x00F0, 0x0343, 0x0364 },
    { 0x6362, 0x00F4, 0x0365, 0x0386 },
};

void func_800E1D50(void) // 0x800E1D50
{
    typedef enum _EventState
    {
        EventState_Skip = 3
    } e_EventState;

    VECTOR3  lintPos;
    SVECTOR3 lintRot;

    // Skip.
    if ((g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.skip) &&
        g_SysWork.sysStateSteps[0] > 0 && g_SysWork.sysStateSteps[0] < EventState_Skip)
    {
        SysWork_StateStepSet(0, EventState_Skip);
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();

            D_800ED5AC = 0;
            g_Cutscene_Timer = Q12(0.0f);
            g_SysWork.sysFlags |= SysFlag_NoEnemySpawn;

            Game_TurnFlashlightOn();
            func_800E1CA0();
            func_8007E860();
            Fs_QueueStartRead(FILE_ANIM_HBM6_S4B_ANM, FS_BUFFER_4);
            Fs_QueueStartRead(FILE_ANIM_PARK_DMS, FS_BUFFER_14);

            D_800A9938 = 0x38630;

            Fs_QueueWaitForEmpty();
            Dms_HeaderFixOffsets((s_DmsHeader*)FS_BUFFER_14);

            g_SysWork.lightBoneCoord     = NULL;
            g_SysWork.lensFlareBoneCoord = NULL;
            g_SysWork.lightIntensity     = Q12(1.0f);

            Anim_CharaTypeAnimInfoClear();
            Chara_Load(0, Chara_MonsterCybil, g_SysWork.npcBoneCoordBuffer, -1, NULL, NULL);

            CutsceneBorder_ForceShow();
            g_SysWork.sysFlags |= SysFlag_CutsceneActive;

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 2, Q12(0.0f), false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 51, false);
            SysWork_StateStepIncrement(0);

        case 1:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 113, false);
            SysWork_StateStepIncrement(0);
            break;

        case 2:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(0.0f), Q12(19.0f), false, true);
            break;

        case 3:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        default:
            g_Cutscene_Timer = NO_VALUE;

            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            vcReturnPreAutoCamWork(true);
            Chara_ProcessLoads();
            Chara_Spawn(Chara_MonsterCybil, 0, Q12(26.5f), Q12(108.5f), Q12(1.0f / 32.0f), 3);
            func_800E15FC(&g_SysWork.playerWork.player, &g_SysWork.npcs[0], true);
            Savegame_EventFlagSet(EventFlag_441);
            break;
    }

    if (g_Cutscene_Timer >= Q12(0.0f))
    {
        Dms_CharacterTransformGet(&g_SysWork.playerWork.player.position, &g_SysWork.playerWork.player.rotation, "HERO", g_Cutscene_Timer, (s_DmsHeader*)FS_BUFFER_14);
        vcChangeProjectionValue(Dms_CameraTargetsGet(&g_Cutscene_CameraPositionTarget, &g_Cutscene_CameraLookAtTarget, NULL, g_Cutscene_Timer, (s_DmsHeader*)FS_BUFFER_14));
        vcUserCamTarget(&g_Cutscene_CameraPositionTarget, NULL, true);
        vcUserWatchTarget(&g_Cutscene_CameraLookAtTarget, NULL, true);
        Dms_CharacterTransformGet(&g_SysWork.lightPosition, &lintRot, "LIGHT", g_Cutscene_Timer, (s_DmsHeader*)FS_BUFFER_14);
        Dms_CharacterTransformGet(&lintPos, &lintRot, "L_INT", g_Cutscene_Timer, (s_DmsHeader*)FS_BUFFER_14);

        g_SysWork.lightRotation.vx = -ratan2(lintPos.vy - g_SysWork.lightPosition.vy, Math_Vector2MagCalcSafeQ6(lintPos.vx - g_SysWork.lightPosition.vx, lintPos.vz - g_SysWork.lightPosition.vz));
        g_SysWork.lightRotation.vy = ratan2(lintPos.vx - g_SysWork.lightPosition.vx, lintPos.vz - g_SysWork.lightPosition.vz);
        g_SysWork.lightRotation.vz = Q12_ANGLE(0.0f);
    }
}

void func_800E219C(void) // 0x800E219C
{
    VECTOR3  lightIntPos;
    SVECTOR3 unused;

    #define playerChara g_SysWork.playerWork.player

    if ((g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.skip) &&
        g_SysWork.sysStateSteps[0] >= 2 && g_SysWork.sysStateSteps[0] < 8)
    {
        SysWork_StateStepReset();
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Game_TurnFlashlightOn();

            D_800ED5AC = 0;
            Savegame_EventFlagSet(EventFlag_461);
            g_Cutscene_Timer = Q12(21.0f);

            SysWork_StateStepIncrement(0);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, 0, false);

            g_SysWork.lightBoneCoord = NULL;
            g_SysWork.lensFlareBoneCoord = NULL;

        case 1:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &playerChara, 168, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 9, false);
            SysWork_StateStepIncrement(0);

        case 2:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(21.0f), Q12(68.0f), false, true);
            break;

        case 3:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 10, false);
            Model_AnimFlagsClear(&playerChara.model, 2);
            SysWork_StateStepIncrement(0);

        case 4:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(69.0f), Q12(80.0f), false, true);
            break;

        case 5:
            Event_WaitTimer(Q12(2.0f), false);
            break;

        case 6:
            Model_AnimFlagsSet(&playerChara.model, 2);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &playerChara, 167, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 11, false);
            SysWork_StateStepIncrement(0);

        case 7:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(81.0f), Q12(95.0f), true, true);
            break;

        case 8:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, 0, false);
            break;

        default:
            g_Cutscene_Timer = NO_VALUE;

            // Warp player.
            playerChara.position.vx = Q12(25.0f);
            playerChara.position.vz = Q12(100.0f);
            playerChara.rotation.vy = Q12_ANGLE(337.5f);

            // Warp NPC.
            g_SysWork.npcs[0].position.vx = Q12(24.0f);
            g_SysWork.npcs[0].position.vz = Q12(102.0f);
            g_SysWork.npcs[0].rotation.vy = Q12_ANGLE(157.5f);

            g_SysWork.npcs[0].properties.npc.controlState = 0;
            g_SysWork.npcs[0].properties.npc.field_EC     = 1;
            g_SysWork.npcs[0].properties.npc.field_EE     = 1;

            vcReturnPreAutoCamWork(true);
            func_800E15FC(&playerChara, &g_SysWork.npcs[0], true);
            Player_ControlUnfreeze(false);

            SysWork_StateSetNext(SysState_Gameplay);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, 0, false);

            func_8008D448();
            Game_FlashlightAttributesFix();

            g_SysWork.lightIntensity = Q12(1.0f);

            Savegame_EventFlagSet(EventFlag_443);

            Model_AnimFlagsSet(&playerChara.model, 2);
            func_800D8A90(&g_SysWork.npcs[0]);
            func_8003A16C();

            Savegame_EventFlagSet(EventFlag_467);
            break;
    }

    if (g_Cutscene_Timer >= Q12(0.0f))
    {
        // Get "HERO" position.
        Dms_CharacterTransformGet(&playerChara.position, &playerChara.rotation, "HERO", g_Cutscene_Timer, FS_BUFFER_14);

        // Get "MSB" position
        Dms_CharacterTransformGet(&g_SysWork.npcs[0].position, &g_SysWork.npcs[0].rotation, "MSB", g_Cutscene_Timer, FS_BUFFER_14);

        vcChangeProjectionValue(Dms_CameraTargetsGet(&g_Cutscene_CameraPositionTarget, &g_Cutscene_CameraLookAtTarget, NULL, g_Cutscene_Timer, FS_BUFFER_14));
        vcUserCamTarget(&g_Cutscene_CameraPositionTarget, NULL, true);
        vcUserWatchTarget(&g_Cutscene_CameraLookAtTarget, NULL, true);

        // "LIGHT", cutscene light position?
        Dms_CharacterTransformGet(&g_SysWork.lightPosition, &unused, "LIGHT", g_Cutscene_Timer, FS_BUFFER_14);

        // "L_INT", interior light or intersection point?
        Dms_CharacterTransformGet(&lightIntPos, &unused, "L_INT", g_Cutscene_Timer, FS_BUFFER_14);

        // Set light rotation.
        g_SysWork.lightRotation.vx = -ratan2(lightIntPos.vy - g_SysWork.lightPosition.vy, Math_Vector2MagCalcSafeQ6(lightIntPos.vx - g_SysWork.lightPosition.vx, lightIntPos.vz - g_SysWork.lightPosition.vz));
        g_SysWork.lightRotation.vy =  ratan2(lightIntPos.vx - g_SysWork.lightPosition.vx, lightIntPos.vz - g_SysWork.lightPosition.vz);
        g_SysWork.lightRotation.vz = Q12_ANGLE(0.0f);
    }

    #undef playerChara
}

void func_800E2724(void) // 0x800E2724
{
    s32 curStateStep;

    #define playerChara g_SysWork.playerWork.player

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            ScreenFade_ResetTimestep();
            g_SysWork.sysFlags |= SysFlag_NoEnemySpawn;

            func_800E1CA0();
            func_8007E860();
            Fs_QueueStartRead(FILE_ANIM_HBM6_S4B_ANM, FS_BUFFER_4);

            D_800A9938 = 0x38630;

#if VERSION_EQUAL_OR_NEWER(JAP1) // @bugfix? PARK.DMS gets loaded in `func_800E1D50` above, maybe it had chance of that load being skipped?
            Fs_QueueStartRead(FILE_ANIM_PARK_DMS, FS_BUFFER_14);
            Fs_QueueWaitForEmpty();
            Dms_HeaderFixOffsets(FS_BUFFER_14);
#endif

            Anim_CharaTypeAnimInfoClear();

            // Load Monster Cybil character.
            Chara_Load(0, Chara_MonsterCybil, &g_SysWork.npcBoneCoordBuffer[0], CHARA_FORCE_FREE_ALL, NULL, NULL);
            Chara_ProcessLoads();
            Chara_Spawn(Chara_MonsterCybil, 0, Q12(26.5f), Q12(108.5f), Q12_ANGLE(11.3f), 3);

            // Warp camera to start position.
            Event_CameraPositionSet(NULL, Q12(25.27f), Q12(-2.4f), Q12(98.43f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(NULL, Q12(23.78f), Q12(-1.04f), Q12(101.88f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);

            // Warp player to start position.
            playerChara.position.vx = Q12(25.0f);
            playerChara.position.vz = Q12(100.0f);
            playerChara.rotation.vy = Q12_ANGLE(337.5f);

            // Warp Monster Cybil character to start position.
            g_SysWork.npcs[0].position.vx = Q12(24.0f);
            g_SysWork.npcs[0].position.vz = Q12(102.0f);
            g_SysWork.npcs[0].rotation.vy = Q12_ANGLE(157.5f);

            SysWork_StateStepIncrement(0);
            break;

        case 1:
            curStateStep = g_SysWork.sysStateSteps[0];

            // TODO: `Chara_MonsterCybil` properties
            g_SysWork.npcs[0].properties.npc.controlState = Q12(0.0f);
            g_SysWork.npcs[0].properties.npc.field_EC     = curStateStep;
            g_SysWork.npcs[0].properties.npc.field_EE     = curStateStep;
            func_800D8A90(&g_SysWork.npcs[0]);

            Savegame_EventFlagSet(EventFlag_467);

            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);

            vcReturnPreAutoCamWork(true);
            func_800E15FC(&playerChara, &g_SysWork.npcs[0], true);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            break;
    }

    #undef playerChara
}

void func_800E2950(void) // 0x800E2950
{
    VECTOR3 camPos;
    VECTOR3 camLookAt;

    #define playerChara g_SysWork.playerWork.player

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();

            func_8007FD4C(true);
            Chara_MovementReset(&g_SysWork.npcs[0]);

            CutsceneBorder_ForceShow();

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 2, Q12(0.0f), false);
            Game_TurnFlashlightOn();

            // Warp player.
            playerChara.position.vx = Q12(23.0f);
            playerChara.position.vz = Q12(103.0f);
            playerChara.rotation.vy = Q12_ANGLE(135.0f);

            // Warp NPC.
            g_SysWork.npcs[0].position.vx = Q12(18.0f);
            g_SysWork.npcs[0].position.vz = Q12(96.0f);
            g_SysWork.npcs[0].rotation.vy = Q12_ANGLE(135.0f);

            // Warp camera.
            Math_Vector3Set(&camPos, Q12(18.0f), Q12(-1.5f), Q12(94.5f));
            Math_Vector3Set(&camLookAt, Q12(18.0f), Q12(-1.3f), Q12(96.0f));
            Event_CameraPositionSet(&camPos, Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(&camLookAt, Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            func_8008D438();

            g_SysWork.lensFlareBoneCoord = NULL;

            g_SysWork.lightRotation.vx = -ratan2(camLookAt.vy - camPos.vy, Math_Vector2MagCalcSafeQ6(camLookAt.vx - camPos.vx, camLookAt.vz - camPos.vz));
            g_SysWork.lightRotation.vy = ratan2(camLookAt.vx - camPos.vx, camLookAt.vz - camPos.vz);
            g_SysWork.lightRotation.vz = Q12_ANGLE(0.0f);

            g_SysWork.lightBoneCoord = NULL;

            g_SysWork.lightPosition.vx = camPos.vx;
            g_SysWork.lightPosition.vy = MAX(Q12(-1.5f), camPos.vy);
            g_SysWork.lightPosition.vz = camPos.vz;

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 4, false);

            SysWork_StateStepIncrement(0);

        case 1:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimLock, &g_SysWork.npcs[0], 0, false);
            Event_WaitTimer(Q12(1.0f), false);
            break;

        case 2:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimUnlock, &g_SysWork.npcs[0], 0, false);
            SysWork_StateStepIncrement(0);
            break;

        case 3:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AwaitAnimEnd, &g_SysWork.npcs[0], 0, false);
            break;

        default:
            vcReturnPreAutoCamWork(true);
            func_800E15FC(&playerChara, &g_SysWork.npcs[0], true);
            Player_ControlUnfreeze(false);

            SysWork_StateSetNext(SysState_Gameplay);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);

            func_8008D448();
            Game_FlashlightAttributesFix();

            g_SysWork.lightIntensity = Q12(1.0f);
            Savegame_EventFlagSet(EventFlag_445);
            break;
    }

    #undef playerChara
}

void MapEvent_CutsceneCybilDeath(void) // 0x800E2CA0
{
    VECTOR3         lightIntPos;
    SVECTOR3        unused;
    s_SubCharacter* player;

    #define playerChara g_SysWork.playerWork.player

    // Skip.
    if ((g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.skip) &&
        g_SysWork.sysStateSteps[0] == 7)
    {
        SysWork_StateStepSet(0, 8);
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            D_800EBA30 = 0;
            D_800ED5B6 = 0;

            Game_TurnFlashlightOn();
            func_8003D03C();
            Sd_SfxStop(Sfx_Unk1617);
            Player_ControlFreeze();

            g_SysWork.npcs[0].health = NO_VALUE;

            D_800ED5AC                   = 0;
            g_Cutscene_Timer             = Q12(96.0f);
            g_SysWork.lightBoneCoord     = NULL;
            g_SysWork.lensFlareBoneCoord = NULL;
            g_SysWork.lightIntensity     = Q12(1.0f);

            SysWork_StateStepIncrement(0);

        case 1:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 2:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.0f), false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 2, Q12(0.0f), false);
            SysWork_StateStepIncrement(0);
            break;

        case 3:
            SD_Call(16);
            SD_Call(19);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &playerChara, 162, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 7, false);
            SysWork_StateStepIncrement(0);

        case 4:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(96.0f), Q12(131.0f), false, false);

            // Possible inline?
            player = &playerChara;
            if (player->model.anim.keyframeIdx > 1282)
            {
                Event_CharaAnimCmdExecute(CharaAnimCmd_AnimLock, player, 0, false);
            }

            SysWork_StateStepIncrement(0);

        case 5:
            // Possible inline?
            player = &playerChara;
            if (player->model.anim.keyframeIdx > 1282)
            {
                Event_CharaAnimCmdExecute(CharaAnimCmd_AnimLock, player, 0, false);
            }

            SysWork_StateStepIncrement(0);

        case 6:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimUnlock, &playerChara, 0, false);
            SysWork_StateStepIncrement(0);

        case 7:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(131.0f), Q12(195.0f), false, false);
            Event_DisplayMapMsg(false, 41, 0, 0, 0, false);

            if (g_SysWork.sysStateSteps[0] != 7)
            {
                Savegame_EventFlagSet(EventFlag_463);
            }
            break;

        case 8:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.5f), false);
            break;

        default:
            g_Cutscene_Timer = NO_VALUE;
            Player_ControlUnfreeze(false);

            SysWork_StateSetNext(SysState_Gameplay);

            Savegame_EventFlagSet(EventFlag_447);
            Savegame_EventFlagSet(EventFlag_450);

            vcReturnPreAutoCamWork(true);
            func_8008D448();
            Game_FlashlightAttributesFix();
            g_SysWork.lightIntensity = Q12(1.0f);
            SD_Call(19);
            Chara_ModelCharaIdClear(&g_SysWork.npcs[0], 0, 0);
            break;
    }

    if (g_Cutscene_Timer > Q12(96.0f))
    {
        // Get "HERO" position.
        Dms_CharacterTransformGet(&playerChara.position, &playerChara.rotation, "HERO", g_Cutscene_Timer, FS_BUFFER_14);

        // Get "MSB" position
        Dms_CharacterTransformGet(&g_SysWork.npcs[0].position, &g_SysWork.npcs[0].rotation, "MSB", g_Cutscene_Timer, FS_BUFFER_14);

        vcChangeProjectionValue(Dms_CameraTargetsGet(&g_Cutscene_CameraPositionTarget, &g_Cutscene_CameraLookAtTarget, NULL, g_Cutscene_Timer, FS_BUFFER_14));
        vcUserCamTarget(&g_Cutscene_CameraPositionTarget, NULL, true);
        vcUserWatchTarget(&g_Cutscene_CameraLookAtTarget, NULL, true);

        // "LIGHT", cutscene light position?
        Dms_CharacterTransformGet(&g_SysWork.lightPosition, &unused, "LIGHT", g_Cutscene_Timer, FS_BUFFER_14);

        // "L_INT", interior light or intersection point?
        Dms_CharacterTransformGet(&lightIntPos, &unused, "L_INT", g_Cutscene_Timer, FS_BUFFER_14);

        // Set light rotation.
        g_SysWork.lightRotation.vx = -ratan2(lightIntPos.vy - g_SysWork.lightPosition.vy, Math_Vector2MagCalcSafeQ6(lightIntPos.vx - g_SysWork.lightPosition.vx, lightIntPos.vz - g_SysWork.lightPosition.vz));
        g_SysWork.lightRotation.vy =  ratan2(lightIntPos.vx - g_SysWork.lightPosition.vx, lightIntPos.vz - g_SysWork.lightPosition.vz);
        g_SysWork.lightRotation.vz = Q12_ANGLE(0.0f);
    }
}

void func_800E3244(void) // 0x800E3244
{
    typedef enum _EventState
    {
        EventState_Skip = 28
    } e_EventState;

    VECTOR3  lintPos;
    SVECTOR3 lintRot;
    q19_12   posX;
    q19_12   lookAtX;

    #define playerChara g_SysWork.playerWork.player

    // Skip.
    if ((g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.skip) &&
        g_SysWork.sysStateSteps[0] >= 2 && g_SysWork.sysStateSteps[0] < 27)
    {
        if (g_SysWork.sysStateSteps[0] != 20 &&
            g_SysWork.sysStateSteps[0] != 22)
        {
            SysWork_StateStepSet(0, EventState_Skip);
        }
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            D_800EBA30 = 0;
            D_800ED5B6 = 0;

            Sd_SfxStop(Sfx_Unk1617);
            Player_ControlFreeze();
            Chara_MovementReset(&g_SysWork.npcs[0]);

            g_SysWork.npcs[0].health = NO_VALUE;

            Game_TurnFlashlightOn();
            g_Cutscene_Timer = Q12(0.0f);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 2, Q12(0.0f), false);
            D_800ED5AC = 0;

            Fs_QueueStartRead(FILE_ANIM_AMUSE1_DMS, FS_BUFFER_14);
            Fs_QueueWaitForEmpty();
            Dms_HeaderFixOffsets((s_DmsHeader*)FS_BUFFER_14);

            g_SysWork.lightBoneCoord = NULL;
            g_SysWork.lensFlareBoneCoord = NULL;
            g_SysWork.lightIntensity = Q12(0.5f);

            Chara_Load(1, Chara_Parasite, &g_SysWork.npcBoneCoordBuffer[30], 0, NULL, NULL);

            CutsceneBorder_ForceShow();
            g_SysWork.sysFlags |= SysFlag_CutsceneActive;

            func_8003D03C();
            Player_EmptyWeaponHandSet();
            WorldGfx_PlayerHeldItemSet(InvItemId_CutscenePlasticBottle);
            Fs_QueueWaitForEmpty();
            SysWork_StateStepIncrement(0);

        case 1:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &playerChara, 170, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 8, false);
            SysWork_StateStepIncrement(0);

        case 2:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(0.0f), Q12(24.0f), false, true);
            func_800E73B4(g_Cutscene_Timer);
            break;

        case 3:
            SysWork_StateStepIncrement(0);

        case 4:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &playerChara, 185, false);
            SysWork_StateStepIncrement(0);

        case 5:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(25.0f), Q12(58.0f), false, true);
            break;

        case 6:
            g_Cutscene_Timer = Q12(59.0f);

            Chara_ProcessLoads();
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &playerChara, 160, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 5, false);
            Savegame_EventFlagSet(EventFlag_462);

            func_800E7944(0);
            Event_DisplayMapMsgWithAudio(39, &D_800ED5AC, D_800EBA34);
            SysWork_StateStepIncrement(0);

        case 7:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(59.0f), Q12(139.0f), false, true);
            func_800E7944(2);
            break;

        case 8:
            Chara_Spawn(Chara_Parasite, 0, Q12(20.0f), Q12(100.0f), 0, 3);
            Sfx_WithFlagsPlay(Sfx_Unk1625, &g_SysWork.npcs[1].position, 32, SfxFlag_None);
            Sfx_WithFlagsPlay(Sfx_Unk1624, &g_SysWork.npcs[1].position, 32, SfxFlag_None);
            func_800E7944(1);
            SysWork_StateStepIncrement(0);

        case 9:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(140.0f), Q12(183.0f), false, true);
            Sfx_WithFalloffAndPitchPlay(Sfx_Unk1624, &g_SysWork.npcs[1].position, 32, Q12(16.0f), 0);
            func_800E7944(2);
            break;

        case 10:
            SD_Call(19);

            g_Cutscene_Timer = Q12(186.0f);

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &playerChara, 163, false);
            func_800E7944(3);
            SysWork_StateStepIncrement(0);

        case 11:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(5.0f), Q12(186.0f), Q12(193.0f), false, true);
            Sfx_WithFalloffAndPitchPlay(Sfx_Unk1624, &g_SysWork.npcs[1].position, 32, Q12(16.0f), 0);
            break;

        case 12:
            g_Cutscene_Timer = Q12(195.0f);

            ModelAnim_StatusIncrement(&g_SysWork.npcs[1].model.anim);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &playerChara, 151, false);
            SysWork_StateStepIncrement(0);

        case 13:
            if (g_Cutscene_Timer < Q12(202.0f))
            {
                Sfx_WithFalloffAndPitchPlay(Sfx_Unk1624, &g_SysWork.npcs[1].position, 32, Q12(16.0f), 0);
            }
            else
            {
                Sd_SfxStop(Sfx_Unk1624);
            }

            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(195.0f), Q12(213.0f), false, true);
            break;

        case 14:
            g_Cutscene_Timer = Q12(216.0f);

            Chara_ModelCharaIdClear(&g_SysWork.npcs[1], 0, 0);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &playerChara, 161, false);
            SysWork_StateStepIncrement(0);

        case 15:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(216.0f), Q12(243.0f), false, true);

            if (g_Cutscene_Timer >= Q12(219.0f))
            {
                Event_DisplayMapMsgWithAudio(17, &D_800ED5AC, D_800EBA34);
            }
            break;

        case 16:
            SysWork_StateStepIncrement(0);

        case 17:
            Event_DisplayMapMsgWithAudio(18, &D_800ED5AC, D_800EBA34); // "Wake up! Snap out of it!"
            break;

        case 18:
            Event_DisplayMapMsgWithAudio(19, &D_800ED5AC, D_800EBA34); // "Cybil!"

            if (g_SysWork.sysStateSteps[0] != 18)
            {
                Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 6, false);
            }
            break;

        case 19:
            Event_DisplayMapMsgWithAudio(20, &D_800ED5AC, D_800EBA34); // "...Harry? What... happened?"
            break;

        case 20:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(1.0f), false);
            break;

        case 21:
            D_800ED5B4 = -0x7A;

            Model_AnimFlagsClear(&playerChara.model, (1<<1));
            Chara_ModelCharaIdClear(&g_SysWork.npcs[0], 0, 0);

            g_Cutscene_Timer = NO_VALUE;

            func_8008D438();

            g_SysWork.lightIntensity = Q12(0.3594f);
            D_800ED5B8 = 1;

            SysWork_StateStepIncrement(0);

        case 22:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(1.0f), false);
            break;

        case 23:
            Event_DisplayMapMsgWithAudio(22, &D_800ED5AC, D_800EBA34);
            break;

        case 24:
            Event_DisplayMapMsgWithAudio(28, &D_800ED5AC, D_800EBA34);
            break;

        case 25:
            Event_DisplayMapMsgWithAudio(34, &D_800ED5AC, D_800EBA34);
            break;

        case 26:
            Event_DisplayMapMsgWithAudio(36, &D_800ED5AC, D_800EBA34);
            break;

        case 27:
            SD_Call(19);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.8f), false);
            break;

        case EventState_Skip:
            SD_Call(19);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        default:
            func_800E7944(3);

            g_Cutscene_Timer = NO_VALUE;

            Player_WeaponAttackRestore();
            WorldGfx_PlayerPrevHeldItem(&g_SysWork.playerCombat);
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);

            Chara_AnimReset(&g_SysWork.npcs[0], false);
            vcReturnPreAutoCamWork(true);
            func_8008D448();
            Game_FlashlightAttributesFix();

            g_SysWork.lightIntensity = Q12(1.0f);

            Savegame_EventFlagSet(EventFlag_449);
            Savegame_EventFlagSet(EventFlag_450);

            Chara_ModelCharaIdClear(&g_SysWork.npcs[0], 0, 0);
            Chara_ModelCharaIdClear(&g_SysWork.npcs[1], 0, 0);

            Sd_SfxStop(Sfx_Unk1625);
            Sd_SfxStop(Sfx_Unk1624);
            Model_AnimFlagsSet(&playerChara.model, (1<<1));
            break;
    }

    if (g_SysWork.sysStateSteps[0] >= 22 && g_SysWork.sysStateSteps[0] <= 27)
    {
        posX = FP_FROM(Math_Sin(D_800ED5B6) * Q12(5.2f), Q12_SHIFT) + Q12(20.0f);
        Event_CameraPositionSet(NULL, posX, Q12(-2.0f), FP_FROM(Math_Cos(D_800ED5B6) * Q12(5.2f), Q12_SHIFT) + Q12(100.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);

        lookAtX = FP_FROM(Math_Sin(D_800ED5B6 - Q12(0.172f)) * Q12(3.7f), Q12_SHIFT) + Q12(20.0f);
        Event_CameraLookAtSet(NULL, lookAtX, Q12(-1.0f), FP_FROM(Math_Cos(D_800ED5B6 - 704) * Q12(3.7f), Q12_SHIFT) + Q12(100.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);

        Math_Vector3Set(&g_SysWork.lightPosition, FP_FROM(Math_Sin(D_800ED5B6 - Q12(0.2188f)) * Q12(4.8f), Q12_SHIFT) + Q12(20.0f),
                        Q12(-1.0f),
                        FP_FROM(Math_Cos(D_800ED5B6 - Q12(0.2188f)) * Q12(4.8f), Q12_SHIFT) + Q12(100.0f));
        Math_SetSVectorFast(&g_SysWork.lightRotation, Q12_ANGLE(-39.4f), (D_800ED5B6 + Q12_ANGLE(182.9f)), Q12_ANGLE(0.0f));
        func_800E74C4();
    }

    if (g_Cutscene_Timer > Q12(0.0f))
    {
        Dms_CharacterTransformGet(&playerChara.position, &playerChara.rotation, "HERO", g_Cutscene_Timer, (s_DmsHeader*)FS_BUFFER_14);
        Dms_CharacterTransformGet(&g_SysWork.npcs[0].position, &g_SysWork.npcs[0].rotation, "MSB", g_Cutscene_Timer, (s_DmsHeader*)FS_BUFFER_14);
        Dms_CharacterTransformGet(&g_SysWork.npcs[1].position, &g_SysWork.npcs[1].rotation, "ICU", g_Cutscene_Timer, (s_DmsHeader*)FS_BUFFER_14);

        vcChangeProjectionValue(Dms_CameraTargetsGet(&g_Cutscene_CameraPositionTarget, &g_Cutscene_CameraLookAtTarget, NULL, g_Cutscene_Timer, (s_DmsHeader*)FS_BUFFER_14));
        vcUserCamTarget(&g_Cutscene_CameraPositionTarget, NULL, true);
        vcUserWatchTarget(&g_Cutscene_CameraLookAtTarget, NULL, true);

        Dms_CharacterTransformGet(&g_SysWork.lightPosition, &lintRot, "LIGHT", g_Cutscene_Timer, (s_DmsHeader*)FS_BUFFER_14);
        Dms_CharacterTransformGet(&lintPos, &lintRot, "L_INT", g_Cutscene_Timer, (s_DmsHeader*)FS_BUFFER_14);

        g_SysWork.lightRotation.vx = -ratan2(lintPos.vy - g_SysWork.lightPosition.vy, Math_Vector2MagCalcSafeQ6(lintPos.vx - g_SysWork.lightPosition.vx, lintPos.vz - g_SysWork.lightPosition.vz));
        g_SysWork.lightRotation.vy = ratan2(lintPos.vx - g_SysWork.lightPosition.vx, lintPos.vz - g_SysWork.lightPosition.vz);
        g_SysWork.lightRotation.vz = Q12_ANGLE(0.0f);
    }

    #undef playerChara
}

void func_800E3EF4(void) // 0x800E3EF4
{
    typedef struct
    {
        SPRT*     sprt_0;
        DR_TPAGE* tpage_4;
        DR_STP*   stp_8;
        u8        unk_C[4];
        u8        unk_10[4];
        s32       activeBufferIdx_14;
    } g_GteScratchData_func_800DD2D4;

    g_GteScratchData_func_800DD2D4* scratchData;

    VECTOR3       lightIntPos;
    SVECTOR3      unused;
    s_FsImageDesc sp30;
    s32           i;

    #define playerChara g_SysWork.playerWork.player

    scratchData = PSX_SCRATCH_ADDR(0);

    if ((g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.skip) &&
        g_SysWork.sysStateSteps[0] >= 4 && g_SysWork.sysStateSteps[0] < 63)
    {
        func_800E01F4();
        SysWork_StateStepSet(0, 65);
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            D_800ED5AD = 1;
            D_800ED5AC = 0;

            Fs_QueueStartRead(FILE_ANIM_AMUSE2_DMS, FS_BUFFER_14);
            Fs_QueueWaitForEmpty();
            Dms_HeaderFixOffsets(FS_BUFFER_14);
            WorldObject_ModelNameSet(&g_WorldObject_SFlauros.object, "SFLAU_HI");

            g_Cutscene_Timer = Q12(0.0f);
            D_800EBB60 = false;
            D_800EBB61 = 0;

            CutsceneBorder_ForceShow();
            ScreenFade_ResetTimestep();
            g_SysWork.sysFlags |= SysFlag_CutsceneActive;

            func_8008D438();

            g_SysWork.lightBoneCoord     = NULL;
            g_SysWork.lensFlareBoneCoord = NULL;
            g_SysWork.lightIntensity     = Q12(1.0f);

            Player_EmptyWeaponHandSet();
            func_8003D03C();
            SysWork_StateStepIncrement(0);
            break;

        case 1:
            Bgm_CrossfadeToTrack(BgmCmd_Track36);
            break;

        case 2:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &playerChara, 51, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 1, false);

            g_SysWork.lightIntensity = Q12(1.0f);

            SysWork_StateStepIncrement(0);

        case 3:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.0f), false);
            break;

        case 4:
            Event_DisplayMapMsgWithAudio(42, &D_800ED5AC, &D_800EBA64);
            break;

        case 5:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 4, false);
            SysWork_StateStepIncrement(0);

        case 6:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(1.0f), Q12(24.0f), false, true);
            break;

        case 7:
            Event_DisplayMapMsgWithAudio(47, &D_800ED5AC, &D_800EBA64);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(1.0f), Q12(39.0f), false, true);
            break;

        case 8:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &playerChara, 149, false);

            D_800EBAA8 = true;

            func_800892A4(8);
            SysWork_StateStepIncrement(0);

        case 9:
            Event_DisplayMapMsgWithAudio(47, &D_800ED5AC, &D_800EBA64);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(1.0f), Q12(59.0f), false, false);
            break;

        case 10:
            Chara_Load(1, Chara_Flauros, &g_SysWork.npcBoneCoordBuffer[30], 0, NULL, NULL);
            D_800EBAA8 = false;
            SysWork_StateStepIncrement(0);

        case 11:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(1.0f), Q12(59.0f), false, true);
            break;

        case 12:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 8, false);
            SysWork_StateStepIncrement(0);

        case 13:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(1.0f), Q12(79.0f), true, true);
            break;

        case 14:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(80.0f), Q12(104.0f), true, true);
            g_SysWork.lightIntensity = Q12(2.0f);
            Savegame_EventFlagSet(EventFlag_452);
            break;

        case 15:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &playerChara, 150, false);
            func_800DFC94(&g_Cutscene_Timer);

            g_SysWork.lightIntensity = Q12(0.8f);

            SysWork_StateStepIncrement(0);

        case 16:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(105.0f), Q12(114.0f), false, true);
            break;

        case 17:
            Event_DisplayMapMsgWithAudio(48, &D_800ED5AC, &D_800EBA64);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(105.0f), Q12(162.0f), true, false);
            break;

        case 18:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(105.0f), Q12(162.0f), true, true);
            break;

        case 19:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &playerChara, 152, false);
            func_800DFD08();
            func_800DEC3C(&g_WorldObject_SFlauros.position, &g_WorldObject_SFlauros.rotation);

            D_800EBB61 = 3;
            Savegame_EventFlagSet(EventFlag_464);
            g_SysWork.lightIntensity = Q12(0.7f);

            SysWork_StateStepIncrement(0);

        case 20:
            Event_DisplayMapMsgWithAudio(49, &D_800ED5AC, &D_800EBA64);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(163.0f), Q12(199.0f), true, false);
            break;

        case 21:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(163.0f), Q12(199.0f), true, true);
            break;

        case 22:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &playerChara, 188, false);

            g_SysWork.lightIntensity = Q12(0.6f);
            D_800EBB61           = 1;

            Sfx_WithFlagsPlay(Sfx_Unk1635, NULL, Q8_CLAMPED(1.0f), SfxFlag_Mono | SfxFlag_NoDistAtten);
            SysWork_StateStepIncrement(0);

        case 23:
            Event_DisplayMapMsgWithAudio(50, &D_800ED5AC, &D_800EBA64);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(200.0f), Q12(229.0f), true, false);
            break;

        case 24:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(200.0f), Q12(229.0f), true, true);
            break;

        case 25:
            Chara_ProcessLoads();

            g_SysWork.lightIntensity = Q12(1.0f);

            Sfx_WithFlagsPlay(Sfx_Unk1636, NULL, Q8_CLAMPED(1.0f), SfxFlag_Mono | SfxFlag_NoDistAtten);
            func_800DED30();
            func_800DEF50(&g_SysWork.npcs[1].position, &g_SysWork.npcBoneCoordBuffer[30], &g_Cutscene_Timer);
            Chara_Spawn(Chara_Flauros, 0, playerChara.position.vx + Q12(1.0f), playerChara.position.vz, Q8(0.5f), 3);

            D_800EBB61 = 2;

            SysWork_StateStepIncrement(0);

        case 26:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(230.0f), Q12(299.0f), true, true);
            break;

        case 27:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 3, false);
            SysWork_StateStepIncrement(0);

        case 28:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(300.0f), Q12(310.0f), true, true);
            break;

        case 29:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &playerChara, 51, false);
            Chara_ModelCharaIdClear(&g_SysWork.npcs[1], 0, 0);

            D_800EBB61 = 1;

            SysWork_StateStepIncrement(0);

        case 30:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(311.0f), Q12(357.0f), false, true);
            break;

        case 31:
            Sd_SfxStop(Sfx_Unk1636);
            Sd_SfxStop(Sfx_Unk1635);
            Sfx_WithFlagsPlay(Sfx_Unk1634, NULL, Q8_CLAMPED(1.0f), SfxFlag_Mono | SfxFlag_NoDistAtten);
            SysWork_StateStepIncrement(0);

        case 32:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(311.0f), Q12(365.0f), false, true);
            break;

        case 33:
            Event_DisplayMapMsgWithAudio(51, &D_800ED5AC, &D_800EBA64);
            func_800DF134();

            D_800EBB70.vx = g_SysWork.npcs[0].position.vx;
            D_800EBB70.vy = g_SysWork.npcs[0].position.vy - Q12(1.0f);
            D_800EBB70.vz = g_SysWork.npcs[0].position.vz;

            func_800DF5B0(&g_WorldObject_SFlauros.position, &D_800EBB70);
            SysWork_StateStepIncrement(0);

        case 34:
            Event_WaitTimer(Q12(0.2f), false);
            break;

        case 35:
            Sfx_WithFlagsPlay(Sfx_Unk1633, NULL, Q8_CLAMPED(1.0f), SfxFlag_Mono | SfxFlag_NoDistAtten);
            SysWork_StateStepIncrement(0);

        case 36:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AwaitAnimEnd, &g_SysWork.npcs[0], 0, false);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(311.0f), Q12(386.0f), true, false);
            break;

        case 37:
            SysWork_StateStepIncrement(0);

        case 38:
            Event_CharaAnimPlayToEnd(&g_SysWork.npcs[0], 5);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(387.0f), Q12(431.0f), true, false);
            break;

        case 39:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &playerChara, 53, false);
            func_800DF618();
            D_800EBB61 = 0;
            SysWork_StateStepIncrement(0);

        case 40:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(432.0f), Q12(454.0f), true, true);
            break;

        case 41:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &playerChara, 51, false);
            g_SysWork.lightIntensity = Q12(0.6f);
            SysWork_StateStepIncrement(0);

        case 42:
            Event_DisplayMapMsgWithAudio(52, &D_800ED5AC, &D_800EBA64);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(455.0f), Q12(455.0f), true, false);

            if (g_SysWork.sysStateSteps[0] != 42)
            {
                SD_Call(19);
            }
            break;

        case 43:
            if (Sd_AudioStreamingCheck() == AudioStreamingState_None)
            {
                g_Cutscene_Timer = NO_VALUE;

                Chara_FsImageCalc(&sp30, Chara_Dahlia, 2);
                Chara_Load(1, Chara_Dahlia, &g_SysWork.npcBoneCoordBuffer[30], 2, NULL, &sp30);
                Fs_QueueStartRead(FILE_ANIM_AMUSE2B_DMS, FS_BUFFER_14);
                SysWork_StateStepIncrement(0);
            }
            else
            {
                break;
            }

        case 44:
            if (Fs_QueueChunksLoad())
            {
                SysWork_StateStepIncrement(0);
            }
            break;

        case 45:
            Dms_HeaderFixOffsets(FS_BUFFER_14);
            Chara_ProcessLoads();
            Chara_Spawn(Chara_Dahlia, 0, Q12(-40.0f), Q12(-40.0f), Q12_ANGLE(0.0f), 3);
            Savegame_EventFlagClear(EventFlag_452);
            Savegame_EventFlagSet(EventFlag_465);

            g_SysWork.lightIntensity = Q12(0.6f);

            SysWork_StateStepIncrement(0);
            break;

        case 46:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[1], 14, false);
            D_800EBB60 = true;
            SysWork_StateStepIncrement(0);

        case 47:
            Event_DisplayMapMsgWithAudio(54, &D_800ED5AC, &D_800EBA64);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(0.0f), Q12(10.0f), false, true);
            break;

        case 48:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 6, false);
            SysWork_StateStepIncrement(0);

        case 49:
            Event_DisplayMapMsgWithAudio(54, &D_800ED5AC, &D_800EBA64);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(0.0f), Q12(30.0f), true, false);
            break;

        case 50:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &playerChara, 51, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 9, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[1], 12, false);

            g_Cutscene_Timer = Q12(31.0f);

            SysWork_StateStepIncrement(0);

        case 51:
            Event_DisplayMapMsgWithAudio(56, &D_800ED5AC, &D_800EBA64);
            break;

        case 52:
            Event_DisplayMapMsgWithAudio(58, &D_800ED5AC, &D_800EBA64);
            break;

        case 53:
            Event_DisplayMapMsgWithAudio(62, &D_800ED5AC, &D_800EBA64);
            break;

        case 54:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[1], 13, false);
            g_SysWork.lightIntensity = Q12(0.7f);
            SysWork_StateStepIncrement(0);

        case 55:
            Event_DisplayMapMsgWithAudio(71, &D_800ED5AC, &D_800EBA64);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(32.0f), Q12(80.0f), false, true);
            break;

        case 56:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 7, false);
            SysWork_StateStepIncrement(0);

        case 57:
            Event_DisplayMapMsgWithAudio(71, &D_800ED5AC, &D_800EBA64);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(32.0f), Q12(110.0f), true, false);
            break;

        case 58:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(32.0f), Q12(110.0f), true, true);
            break;

        case 59:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &playerChara, 123, false);
            g_SysWork.lightIntensity = Q12(1.0f);
            SysWork_StateStepIncrement(0);

        case 60:
            Event_DisplayMapMsgWithAudio(74, &D_800ED5AC, &D_800EBA64);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(111.0f), Q12(145.0f), true, false);
            break;

        case 61:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 0, Q12(0.5f), false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            SysWork_StateStepIncrement(0);

        case 62:
            Event_DisplayMapMsgWithAudio(75, &D_800ED5AC, &D_800EBA64);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(111.0f), Q12(145.0f), true, false);
            break;

        case 63:
            SysWork_StateStepIncrement(0);
            Sd_SfxStop(Sfx_Unk1636);

        case 64:
            SysWork_StateStepIncrement(0);

        case 65:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 66:
            if (g_SysWork.npcs[1].model.charaId == Chara_Flauros)
            {
                Chara_ModelCharaIdClear(&g_SysWork.npcs[1], 0, 0);
            }

            SysWork_StateStepIncrement(0);

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            ScreenFade_ResetTimestep();

            Chara_ModelCharaIdClear(&g_SysWork.npcs[0], 0, 0);
            Chara_ModelCharaIdClear(&g_SysWork.npcs[1], 0, 0);

            Savegame_EventFlagSet(EventFlag_451);

            Player_ItemRemove(InvItemId_Flauros, 1);

            func_8003D01C();
            Player_WeaponAttackRestore();
            D_800A9938 = 190000;

            SD_Call(19);
            func_8008D448();
            Game_FlashlightAttributesFix();

            g_SysWork.lightIntensity = Q12(1.0f);
            g_SysWork.sysFlags      &= ~SysFlag_NoEnemySpawn;
            break;
    }

    if (D_800EBAA8)
    {
        scratchData->activeBufferIdx_14 = g_ActiveBufferIdx;
        scratchData->sprt_0             = (SPRT*)GsOUT_PACKET_P;
        for (i = 0; i < 2; i++)
        {
            setCodeWord(scratchData->sprt_0, PRIM_RECT | RECT_BLEND | RECT_TEXTURE, COLOR_RGBC(128, 128, 128, 0));
            setXY0Fast(scratchData->sprt_0, ((i << 8) - 160), -112);
            scratchData->sprt_0->u0 = 0;
            scratchData->sprt_0->v0 = (scratchData->activeBufferIdx_14 == 0) ? 32 : 0;
            setWH(scratchData->sprt_0, (i == 0) ? 256 : 64, FRAMEBUFFER_HEIGHT_PROGRESSIVE);
            addPrimFast(&g_OrderingTable2[g_ActiveBufferIdx].org[15], scratchData->sprt_0, 4);

            scratchData->sprt_0++;
            scratchData->tpage_4 = (DR_TPAGE*)scratchData->sprt_0;
            setDrawTPage(scratchData->tpage_4, 0, 0, getTPage(2, 0, ((scratchData->activeBufferIdx_14 << 4) + (i << 2)) << 6, (((scratchData->activeBufferIdx_14 << 4) >> 4) & 1) << 8));

            AddPrim(&g_OrderingTable2[g_ActiveBufferIdx].org[15], scratchData->tpage_4);
            scratchData->tpage_4++;
            scratchData->sprt_0 = (SPRT*)scratchData->tpage_4;
        }

        scratchData->stp_8 = (DR_STP*)scratchData->sprt_0;
        SetDrawStp(scratchData->stp_8, 1);
        addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[ORDERING_TABLE_SIZE - 1], scratchData->stp_8);
        scratchData->stp_8++;
        SetDrawStp(scratchData->stp_8, 0);
        addPrim(&g_OrderingTable2[g_ActiveBufferIdx].org[0], scratchData->stp_8);
        scratchData->stp_8++;
        GsOUT_PACKET_P = (PACKET*)scratchData->stp_8;
    }

    if (g_Cutscene_Timer >= Q12(0.0f))
    {
        Dms_CharacterTransformGet(&playerChara.position, &playerChara.rotation, "HERO", g_Cutscene_Timer, FS_BUFFER_14);
        vcChangeProjectionValue(Dms_CameraTargetsGet(&g_Cutscene_CameraPositionTarget, &g_Cutscene_CameraLookAtTarget, NULL, g_Cutscene_Timer, FS_BUFFER_14));
        vcUserCamTarget(&g_Cutscene_CameraPositionTarget, NULL, true);
        vcUserWatchTarget(&g_Cutscene_CameraLookAtTarget, NULL, true);
        Dms_CharacterTransformGet(&g_SysWork.lightPosition, &unused, "LIGHT", g_Cutscene_Timer, FS_BUFFER_14);
        Dms_CharacterTransformGet(&lightIntPos, &unused, "L_INT", g_Cutscene_Timer, FS_BUFFER_14);

        // Set light rotation.
        g_SysWork.lightRotation.vx = -ratan2(lightIntPos.vy - g_SysWork.lightPosition.vy,
                                                  Math_Vector2MagCalcSafeQ6(lightIntPos.vx - g_SysWork.lightPosition.vx,
                                                                      lightIntPos.vz - g_SysWork.lightPosition.vz));
        g_SysWork.lightRotation.vy =  ratan2(lightIntPos.vx - g_SysWork.lightPosition.vx,
                                                  lightIntPos.vz - g_SysWork.lightPosition.vz);
        g_SysWork.lightRotation.vz = Q12_ANGLE(0.0f);

        Dms_CharacterTransformGet(&g_SysWork.npcs[0].position, &g_SysWork.npcs[0].rotation, "ARSIA", g_Cutscene_Timer, FS_BUFFER_14);

        if (D_800EBB60)
        {
            Dms_CharacterTransformGet(&g_SysWork.npcs[1].position, &g_SysWork.npcs[1].rotation, "DARIA", g_Cutscene_Timer, FS_BUFFER_14);
        }

        if (D_800EBB61 == 3)
        {
            Dms_CharacterTransformGet(&g_WorldObject_SFlauros.position, &g_WorldObject_SFlauros.rotation, "SFLAU", g_Cutscene_Timer, FS_BUFFER_14);
        }
        else if (D_800EBB61 == 1)
        {
            Dms_CharacterTransformGet(&g_WorldObject_SFlauros.position, &g_WorldObject_SFlauros.rotation, "SFLAU", g_Cutscene_Timer, FS_BUFFER_14);
            Savegame_EventFlagSet(EventFlag_454);
        }
        else
        {
            Savegame_EventFlagClear(EventFlag_454);
        }

        if (D_800EBB61 == 2)
        {
            Dms_CharacterTransformGet(&g_SysWork.npcs[1].position, &g_SysWork.npcs[1].rotation, "BFLAU", g_Cutscene_Timer, FS_BUFFER_14);
        }
    }

    if (g_SysWork.sysStateSteps[0] >= 48)
    {
        func_800E0FAC(g_Cutscene_Timer);
    }

    #undef playerChara
}

void func_800E558C(void) // 0x800E558C
{
    typedef struct
    {
        SPRT*     sprt_0;
        DR_TPAGE* tpage_4;
        DR_STP*   stp_8;
        u8        unk_C[4];
        u8        unk_10[4];
        s32       activeBufferIdx_14;
    } g_GteScratchData_func_800DD2D4;

    g_GteScratchData_func_800DD2D4* scratchData;
    s32                             i;

    #define playerChara g_SysWork.playerWork.player

    scratchData = PSX_SCRATCH_ADDR(0);

    if (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.skip &&
        g_SysWork.sysStateSteps[0] >= 2 && g_SysWork.sysStateSteps[0] < 6)
    {
        SysWork_StateStepSet(0, 6);
    }

    D_800ED5F0 += Q12_MULT_PRECISE(g_DeltaTime, 64);
    if (D_800ED5F0 >= 128)
    {
        D_800ED5F0 = 128;
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Savegame_EventFlagSet(EventFlag_453);

            Fs_QueueStartRead(FILE_ANIM_HBM6_S4C_ANM, FS_BUFFER_4);
            Fs_QueueStartRead(FILE_ANIM_LOOKAR_DMS, FS_BUFFER_14);
            Fs_QueueWaitForEmpty();

            Dms_HeaderFixOffsets(FS_BUFFER_14);

            ScreenFade_ResetTimestep();
            CutsceneBorder_ForceShow();
            
            g_Cutscene_Timer            = NO_VALUE;
            g_SysWork.sysFlags |= SysFlag_CutsceneActive;

            func_8003D03C();
            Player_EmptyWeaponHandSet();
            Anim_CharaTypeAnimInfoClear();
            Chara_Load(0, Chara_Alessa, &g_SysWork.npcBoneCoordBuffer[0], CHARA_FORCE_FREE_ALL, NULL, NULL);

            D_800ED5AD = 2;

            SysWork_StateStepIncrement(0);
            break;

        case 1:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &playerChara, 53, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, 0, false);
            SysWork_StateStepIncrement(0);

        case 2:
            g_DeltaTime = Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.65f);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(0.0f), Q12(22.0f), true, true);
            break;

        case 3:
            Chara_ProcessLoads();
            Chara_Spawn(Chara_Alessa, 0, playerChara.position.vx + Q12(1.0f), playerChara.position.vz, Q12_ANGLE(11.3f), 3);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &playerChara, 70, false);

            g_SysWork.lightIntensity = Q12(2.5f);

            SysWork_StateStepIncrement(0);

        case 4:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(3.5f), Q12(23.0f), Q12(33.0f), true, true);
            break;

        case 5:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(1.0f), false);
            break;

        case 6:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 7:
            if (g_SysWork.npcs[0].model.charaId != Chara_Alessa)
            {
                Chara_ProcessLoads();
                Chara_Spawn(Chara_Alessa, 0, playerChara.position.vx + Q12(1.0f), playerChara.position.vz, Q12_ANGLE(11.3f), 3);
            }

            SysWork_StateStepIncrement(0);

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            Savegame_EventFlagSet(EventFlag_468);
            func_8003D01C();
            Player_WeaponAttackRestore();

            D_800ED5F0 = 0;
            break;
    }

    if (g_SysWork.sysStateSteps[0] >= 4)
    {
        scratchData->activeBufferIdx_14 = g_ActiveBufferIdx;
        scratchData->sprt_0             = (SPRT*)GsOUT_PACKET_P;
        for (i = 0; i < 2; i++)
        {
            setCodeWord(scratchData->sprt_0, PRIM_RECT | RECT_BLEND | RECT_TEXTURE, COLOR_RGBC(128, 128, 128, 0));
            setXY0Fast(scratchData->sprt_0, ((i << 8) - 160), -112);
            scratchData->sprt_0->u0 = 0;
            scratchData->sprt_0->v0 = (scratchData->activeBufferIdx_14 == 0) ? 32 : 0;
            setWH(scratchData->sprt_0, (i == 0) ? 256 : 64, FRAMEBUFFER_HEIGHT_PROGRESSIVE);
            addPrimFast(&g_OrderingTable2[g_ActiveBufferIdx].org[15], scratchData->sprt_0, 4);

            scratchData->sprt_0++;
            scratchData->tpage_4 = (DR_TPAGE*)scratchData->sprt_0;
            setDrawTPage(scratchData->tpage_4, 0, 0, getTPage(2, 0, ((scratchData->activeBufferIdx_14 << 4) + (i << 2)) << 6, (((scratchData->activeBufferIdx_14 << 4) >> 4) & 1) << 8));

            AddPrim(&g_OrderingTable2[g_ActiveBufferIdx].org[15], scratchData->tpage_4);
            scratchData->tpage_4++;
            scratchData->sprt_0 = (SPRT*)scratchData->tpage_4;
        }

        scratchData->stp_8 = (DR_STP*)scratchData->sprt_0;
        SetDrawStp(scratchData->stp_8, 1);
        addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[ORDERING_TABLE_SIZE - 1], scratchData->stp_8);
        scratchData->stp_8++;
        SetDrawStp(scratchData->stp_8, 0);
        addPrim(&g_OrderingTable2[g_ActiveBufferIdx].org[0], scratchData->stp_8);
        scratchData->stp_8++;
        GsOUT_PACKET_P = (PACKET*)scratchData->stp_8;
    }

    if (g_Cutscene_Timer >= Q12(0.0f))
    {
        vcChangeProjectionValue(Dms_CameraTargetsGet(&g_Cutscene_CameraPositionTarget, &g_Cutscene_CameraLookAtTarget, NULL, g_Cutscene_Timer, FS_BUFFER_14));
        vcUserCamTarget(&g_Cutscene_CameraPositionTarget, NULL, true);
        vcUserWatchTarget(&g_Cutscene_CameraLookAtTarget, NULL, true);

        if (g_SysWork.sysStateSteps[0] >= 4)
        {
            Dms_CharacterTransformGet(&g_SysWork.npcs[0].position, &g_SysWork.npcs[0].rotation, "ARSIA", g_Cutscene_Timer, FS_BUFFER_14);
            Dms_CharacterTransformGet(&playerChara.position, &playerChara.rotation, "HERO", g_Cutscene_Timer, FS_BUFFER_14);
            playerChara.rotation.vy += Q12_ANGLE(22.5f);
        }
        else
        {
            Dms_CharacterTransformGet(&playerChara.position, &playerChara.rotation, "HERO", g_Cutscene_Timer, FS_BUFFER_14);
        }
    }
}

void Map_WorldObjectsInit(void) // 0x800E5D44
{
    D_800ED841 = 0;
    D_800ED5F0 = 0;
    D_800ED840 = 0;
    D_800ED5AD = 0;
    D_800ED5B4 = 0;
    D_800ED5B8 = 0;
    D_800ED740.vx = Q12(20.0f);
    D_800ED740.vy = Q12(0.0f);
    D_800ED740.vz = Q12(100.0f);
    D_800ED5B6 = 0;

    WorldObject_ModelNameSet(&g_WorldObject0[0], "UMA1_HID");
    WorldObject_ModelNameSet(&g_WorldObject0[1], "UMA2_HID");
    WorldObject_ModelNameSet(&g_WorldObject1[0], "MGR_KOYA");
    WorldObject_ModelNameSet(&g_WorldObject1[1], "MGR_STR_");
    WorldObject_ModelNameSet(&g_WorldObject1[2], "SAKU2_BA");
    WorldObject_ModelNameSet(&g_WorldObject2,    "SAK_POL_");
    WorldObject_ModelNameSet(&g_WorldObject3,    "ROT_POL2");
    WorldObject_ModelNameSet(&g_WorldObject4[0], "MGR_DRL_");
    WorldObject_ModelNameSet(&g_WorldObject4[1], "DRL2_ILI");
    WorldObject_ModelNameSet(&g_WorldObject5,    "CHAIR_HI");
    WorldObject_ModelNameSet(&g_WorldObject6[0], "FW1_BACK");
    WorldObject_ModelNameSet(&g_WorldObject6[1], "FW2_BACK");
    WorldObject_ModelNameSet(&g_WorldObject6[2], "FW3WIR_H");
    WorldObject_ModelNameSet(&g_WorldObject7[0], "FWBOX1_B");
    WorldObject_ModelNameSet(&g_WorldObject7[1], "FWBOX2_B");
    WorldObject_ModelNameSet(&g_WorldObject8,    "WHEEL_BA");

    WorldObject_Init(&g_WorldObject9, g_CommonWorldObjectNames[1], -13.6f, -1.0f, -28.3f, 0.0f, -165.0f, 0.0f);
    func_800E02E0();
    func_800E10F8();

    g_SysWork.npcFlagId = 3;
}

// rodata at the bottom of this file. TODO: Inline once all users are decomped.
extern VECTOR3 D_800CC7C0;
extern VECTOR3 D_800CC7CC;

void Map_WorldObjectsUpdate(void) // 0x800E5F54
{
    s32 cellZ0;
    s32 cellX0;
    s32 projCellZ0;
    s32 projCellX0;
    u16 triggerFlags;

    #define playerChara g_SysWork.playerWork.player

    triggerFlags = CollisionTriggerFlag_None;

    if (!Savegame_EventFlagGet(EventFlag_466))
    {
        Savegame_EventFlagSet(EventFlag_466);
        func_8003A16C();
    }

    cellZ0 = playerChara.position.vz / Q12(CHUNK_SIZE);
    cellX0 = playerChara.position.vx / Q12(CHUNK_SIZE);

    if (playerChara.position.vx > Q12(0.0f))
    {
        projCellX0 = cellX0 + 17;
    }
    else
    {
        projCellX0 = cellX0 + 15;
    }

    if (playerChara.position.vz > Q12(0.0f))
    {
        projCellZ0 = cellZ0 + 17;
    }
    else
    {
        projCellZ0 = cellZ0 + 15;
    }

    switch (CELL_XZ(projCellX0, projCellZ0))
    {
        case CELL_XZ(17, 19):
            func_800E636C();

            if (Savegame_EventFlagGet(EventFlag_443))
            {
                triggerFlags = CollisionTriggerFlag_Objects;

                if (g_SysWork.sysState != SysState_EventCallback)
                {
                    func_800E15FC(&playerChara, &g_SysWork.npcs[0], false);
                }
            }

            if (D_800ED840)
            {
                Sd_SfxStop(Sfx_Unk1616);
                Sd_SfxStop(Sfx_Unk1615);
                D_800ED840 = 0;
            }
            break;

        case CELL_XZ(15, 15):
            WorldObjects_Add(&g_WorldObject9.object, &g_WorldObject9.position, &g_WorldObject9.rotation);

        default:
            if (!Savegame_EventFlagGet(EventFlag_452) &&
                PLAYER_NEAR_POS(vx, 8.0f, 20.0f) && PLAYER_NEAR_POS(vz, -28.0f, 22.0f) &&
                Vw_AabbVisibleInScreenCheck(Q12(7.0f), Q12(11.0f), Q12(-14.0f), Q12(0.0f), Q12(-36.0f), Q12(-20.0f)))
            {
                func_800E6CB8();
            }
            else if (!Savegame_EventFlagGet(EventFlag_453))
            {
                if (PLAYER_NEAR_POS(vx, -16.0f, 20.0f) && PLAYER_NEAR_POS(vz, -64.0f, 20.0f))
                {
                    if (Vw_AabbVisibleInScreenCheck(Q12(-22.0f), Q12(-9.0f), Q12(-8.0f), Q12(0.0f), Q12(-69.0f), Q12(-58.0f)))
                    {
                        func_800E7204();
                    }
                }
            }

            if (Savegame_EventFlagGet(EventFlag_454))
            {
                WorldObjects_Add(&g_WorldObject_SFlauros.object, &g_WorldObject_SFlauros.position, &g_WorldObject_SFlauros.rotation);
            }

            if (D_800ED840 == 0)
            {
                SD_Call(Sfx_Unk1616);
                SD_Call(Sfx_Unk1615);
                D_800ED840++;
            }

            func_800E75B8(Sfx_Unk1616, &D_800CC7C0, D_800ED5F0, Q12(32.0f), D_800ED5AD);
            Sfx_WithFalloffAndPitchPlay(Sfx_Unk1615, &D_800CC7CC, Q8(0.5f), Q12(32.0f), 0);
    }

    Collision_TriggerFlagBitsClear(CollisionTriggerFlag_All);
    Collision_TriggerFlagBitsSet(triggerFlags);
    func_800E0204();
}

void func_800E636C(void) // 0x800E636C
{
    VECTOR3  pos; // Q19.12
    SVECTOR3 rot; // Q19.12
    s32      sp30[8];
    s32      sp50[8];
    q19_12   temp_s0;
    s32      temp_v0_4;
    q19_12   temp_v0_6;
    s32      var_a0_2;
    s32      var_s0;
    s32      i;
    q19_12   curAngle;
    s32      temp_t1;

    #define playerChara g_SysWork.playerWork.player

    if (!Savegame_EventFlagGet(EventFlag_443) || Savegame_EventFlagGet(EventFlag_467))
    {
        temp_t1 = D_800EBA30;

        D_800ED5B6 += Q12_MULT_PRECISE(g_DeltaTime, D_800EBA30);
        D_800ED5B6  = (Q12_MULT_PRECISE(g_DeltaTime, D_800ED5B4) + D_800ED5B6) & 0xFFF;

        if (g_SysWork.npcs[0].properties.npc.controlState == 1)
        {
            if (D_800EBA30 == 0 && g_SysWork.npcs[0].health > Q12(0.0f))
            {
                SD_Call(Sfx_Unk1617);
            }

            if (g_SysWork.npcs[0].health > 0)
            {
                D_800EBA30 -= 50;
            }
            else
            {
                D_800EBA30 += 50;
            }

            D_800EBA30 = CLAMP(D_800EBA30, -455, 0);
        }
        else
        {
            if (g_SysWork.npcs[0].properties.npc.field_EC == 12)
            {
                D_800EBA30 -= 50;

                if (temp_t1 == 0 && D_800EBA30 < 0)
                {
                    if (playerChara.properties.player.moveSpeed > Q12(3.0f))
                    {
                        Player_DamageFeetFront();
                    }

                    SD_Call(Sfx_Unk1617);
                    Savegame_EventFlagSet(EventFlag_455);
                }
            }
            else
            {
                D_800EBA30 += 50;

                if (temp_t1 < 0 && D_800EBA30 >= 0)
                {
                    if (playerChara.properties.player.moveSpeed > Q12(3.0f))
                    {
                        Player_DamageFeetFront();
                    }

                    Sd_SfxStop(Sfx_Unk1617);
                    Savegame_EventFlagClear(EventFlag_455);
                }
            }

            D_800EBA30 = CLAMP(D_800EBA30, -227, 0);
        }

        if (D_800EBA30 != 0)
        {
            pos.vx = Q12(20.0f);
            pos.vy = Q12(1.0f);
            pos.vz = Q12(100.0f);

            Sfx_WithFalloffAndPitchPlay(Sfx_Unk1617, &pos, ABS(D_800EBA30) >> 2, Q12(16.0f), ABS(D_800EBA30) >> 2);
        }

        for (i = 0; i < 10; i++)
        {
            pos.vx = D_800EBAAC[i] + D_800ED740.vx;
            pos.vz = D_800EBAD4[i] + D_800ED740.vz;

            if (Vw_AabbVisibleInScreenCheck(pos.vx - Q12(1.0f), pos.vx + Q12(1.0f), Q12(-2.0f), 0, pos.vz - Q12(1.0f), pos.vz + Q12(1.0f)))
            {
                if (i < 6)
                {
                    temp_v0_4 = Math_Sin((D_800ED5B6 * 4) + ((Q12(1.0f) * i) / 3));
                    var_a0_2  = 1;
                    pos.vy   = (Q12(-1.0f) - temp_v0_4) / 5;
                }
                else
                {
                    var_s0    = Q12(1.0f) * i;
                    temp_v0_4 = Math_Sin((D_800ED5B6 * 4) + (var_s0 / 2) + Q12(0.5f));
                    var_a0_2  = 0;
                    pos.vy   = (Q12(-1.0f) - temp_v0_4) / 5;
                }

                Math_SetSVectorFastSum(&rot, Q12_ANGLE(0.0f), D_800EBAFC[i], Q12_ANGLE(0.0f));
                WorldObjects_Add(&g_WorldObject0[var_a0_2], &pos, &rot);

                if (i == g_SysWork.npcs[0].properties.npc.field_FE)
                {
                    g_SysWork.npcs[0].properties.npc.field_100 = pos.vy;
                }
            }
        }

        pos.vy = Q12(1.0f);
        pos.vx = D_800ED740.vx;
        pos.vz = D_800ED740.vz;

        temp_s0   = Q12_MULT(Math_Sin(D_800ED5B6 + Q12_ANGLE(180.0f)), Q12(8.0f)) + pos.vx;
        temp_v0_6 = Q12_MULT(Math_Cos(D_800ED5B6 + Q12_ANGLE(180.0f)), Q12(8.0f)) + pos.vz;

        if (Vw_AabbVisibleInScreenCheck(temp_s0 - Q12(4.5f), temp_s0 + Q12(4.5f), Q12(-2.0f), 0, temp_v0_6 - Q12(4.5f), temp_v0_6 + Q12(4.5f)))
        {
            Math_SetSVectorFastSum(&rot, Q12_ANGLE(0.0f), D_800ED5B6, Q12_ANGLE(0.0f));
            WorldObjects_Add(g_WorldObject1, &pos, &rot);

            rot.vy += Q12_ANGLE(11.25f);

            WorldObjects_Add(&g_WorldObject1[1], &pos, &rot);
        }

        Math_SetSVectorFastSum(&rot, 0, D_800ED5B6 + Q12_ANGLE(90.0f), 0);
        pos.vy = Q12(-0.3f);

        for (i = 0, curAngle = Q12_ANGLE(90.0f); i < 8; i++)
        {
            sp30[i] = Q12_MULT(Math_Sin(rot.vy + curAngle), Q12(7.5f)) + pos.vx;
            sp50[i] = Q12_MULT(Math_Cos(rot.vy + curAngle), Q12(7.5f)) + pos.vz;

            curAngle += Q12_ANGLE(45.0f);
        }

        for (i = 0; i < 8; i++)
        {
            if (Vw_AabbVisibleInScreenCheck(CLAMP_HIGH(sp30[i], sp30[(i + 1) & 0x7]) - Q12(0.2f),
                                            MAX(sp30[i], sp30[(i + 1) & 0x7]) + Q12(0.2f), Q12(-2.0f), Q12(0.0f),
                                            CLAMP_HIGH(sp50[i], sp50[(i + 1) & 0x7]) - Q12(0.2f),
                                            MAX(sp50[i], sp50[(i + 1) & 0x7]) + Q12(0.2f)))
            {
                if (i != 0)
                {
                    WorldObjects_Add(&g_WorldObject2, &pos, &rot);
                }
                else
                {
                    WorldObjects_Add(&g_WorldObject1[2], &pos, &rot);
                }
            }

            pos.vy  = Q12(0.0f);
            rot.vy += Q12_ANGLE(45.0f);
        }

        Math_SetSVectorFastSum(&rot, Q12_ANGLE(0.0f), D_800ED5B6, Q12_ANGLE(0.0f));

        for (i = 0, curAngle = Q12_ANGLE(90.0f); i < 4; i++)
        {
            sp30[i]   = Q12_MULT(Math_Sin(rot.vy + curAngle), Q12(0.8f)) + pos.vx;
            sp50[i]   = Q12_MULT(Math_Cos(rot.vy + curAngle), Q12(0.8f)) + pos.vz;
            curAngle += Q12_ANGLE(90.0f);
        }

        for (i = 0; i < 4; i++)
        {
            if (Vw_AabbVisibleInScreenCheck(CLAMP_HIGH(sp30[i], sp30[(i + 1) & 0x3]) - Q12(0.4f),
                                            MAX(sp30[i], sp30[(i + 1) & 0x3]) + Q12(0.4f), Q12(-2.0f), Q12(0.0f),
                                            CLAMP_HIGH(sp50[i], sp50[(i + 1) & 0x3]) - Q12(0.4f),
                                            MAX(sp50[i], sp50[(i + 1) & 0x3]) + Q12(0.4f)))
            {
                WorldObjects_Add(g_WorldObject4, &D_800ED740, &rot);
                WorldObjects_Add(&g_WorldObject4[1], &D_800ED740, &rot);
            }

            rot.vy += Q12_ANGLE(90.0f);
        }

        pos.vx = Q12(22.8f);
        pos.vy = Q12(0.0f);
        pos.vz = Q12(96.6f);

        Math_SetSVectorFastSum(&rot, 0, 0xFE1F, 0);

        if (D_800ED5B8 == 0)
        {
            WorldObjects_Add(&g_WorldObject5, &pos, &rot);
        }
    }
}

void func_800E6CB8(void) // 0x800E6CB8
{
    VECTOR3  sp18;
    SVECTOR3 sp28;
    VECTOR3  sp30;
    SVECTOR3 sp40;
    q19_12   time;
    s32      j;
    s32      i;
    s32      var_s3;

    time        = Q12_MULT_PRECISE(g_DeltaTime, Q12(45.511f));
    D_800EBB30 += time;

    if (D_800EBB30 > Q12(307.2f))
    {
        D_800EBB30 -= Q12(307.2f);
        D_800EBB34++;

        for (i = 3; i > 0; i--)
        {
            D_800EBB10[i] = D_800EBB10[i - 1];
            D_800EBB20[i] = D_800EBB20[i - 1];
        }

        D_800EBB20[0] = 0;
        D_800EBB10[0] = 0;
    }

    for (i = 0; i < 4; i++)
    {
        if (D_800EBB20[i] == 0)
        {
            D_800EBB10[i] += time;
            if (D_800EBB10[i] > Q12(45.511f) && ((Rng_Rand16() % Q12(45.511f)) + Q12(45.511f)) < D_800EBB10[i])
            {
                D_800EBB20[i]--;
            }
        }
        else if (D_800EBB10[i] > 0)
        {
            D_800EBB20[i] += Q12_MULT_PRECISE(g_DeltaTime, Q12(-68.2667f));
            D_800EBB20[i]  = MAX(D_800EBB20[i], Q12(-34.1334f));
            D_800EBB10[i] += Q12_MULT_PRECISE(g_DeltaTime, D_800EBB20[i]);

            if (D_800EBB10[i] < 0)
            {
                if (D_800EBB20[i] > Q12(-22.7554f))
                {
                    D_800EBB20[i] = 0;
                    D_800EBB10[i] = 0;
                }
                else
                {
                    D_800EBB20[i] = D_800EBB20[i] >> 1;
                }
            }
        }
        else
        {
            D_800EBB20[i] += Q12_MULT_PRECISE(g_DeltaTime, Q12(68.2667f));
            D_800EBB20[i]  = CLAMP_HIGH(D_800EBB20[i], Q12(34.1334f));
            D_800EBB10[i] += Q12_MULT_PRECISE(g_DeltaTime, D_800EBB20[i]);

            if (D_800EBB10[i] > 0)
            {
                if (D_800EBB20[i] < Q12(22.7554f))
                {
                    D_800EBB20[i] = 0;
                    D_800EBB10[i] = 0;
                }
                else
                {
                    D_800EBB20[i] = D_800EBB20[i] >> 1;
                }
            }
        }
    }

    Math_SetSVectorFastSum(&sp28, Q12_ANGLE(0.0f), Q12_ANGLE(0.0f), Q12_ANGLE(0.0f));
    Math_SetSVectorFastSum(&sp40, Q12_ANGLE(0.0f), Q12_ANGLE(0.0f), Q12_ANGLE(0.0f));

    for (i = 0, var_s3 = Q12(-65.0f); i < 5; i++)
    {
        sp18.vx = Q12(8.7f);
        sp28.vx = FP_FROM(D_800EBB30, Q12_SHIFT) + (var_s3 / 360);
        sp18.vy = Q12_MULT(Math_Cos(sp28.vx), Q12(10.0f)) - Q12(13.8f);
        sp18.vz = Q12_MULT(Math_Sin(sp28.vx), Q12(10.0f)) - Q12(28.0f);

        for (j = 0; j < 3; j++)
        {
            WorldObjects_Add(&g_WorldObject6[j], &sp18, &sp28);
        }

        sp30.vx = Q12(8.7f);
        sp30.vy = Q12_MULT(Math_Cos(sp28.vx), Q12(11.25f)) - Q12(13.8f);
        sp30.vz = Q12_MULT(Math_Sin(sp28.vx), Q12(11.25f)) - Q12(28.0f);
        sp40.vx = FP_FROM(D_800EBB10[i], Q12_SHIFT);

        if (Vw_AabbVisibleInScreenCheck(MIN_OFFSET(sp30.vx, Q12(1.0f), Q12(1.0f)),
                                        MAX_OFFSET(sp30.vx, Q12(1.0f), Q12(1.0f)),
                                        MIN_OFFSET(sp30.vy, Q12(-2.0f), Q12(-0.2f)),
                                        MAX_OFFSET(sp30.vy, Q12(-2.0f), Q12(-0.2f)),
                                        MIN_OFFSET(sp30.vz, Q12(1.0f), Q12(1.0f)),
                                        MAX_OFFSET(sp30.vz, Q12(1.0f), Q12(1.0f))))
        {
            WorldObjects_Add(&g_WorldObject7[(D_800EBB34 + i) & 1], &sp30, &sp40);
        }

        var_s3 += Q12(27.0f);
    }
}

void func_800E7204(void) // 0x800E7204
{
    VECTOR3  pos; // 19.12
    SVECTOR3 rot; // 19.12
    q3_12    angle;
    q19_12   temp_s0;
    s32      i;

    D_800EBB36 += Q12_MULT_PRECISE(g_DeltaTime, Q12(0.197f) + 1);
    D_800EBB38 += Q12_MULT_PRECISE(g_DeltaTime, Q12(0.275f));

    for (i = 0; i < 12; i++)
    {
        angle = Math_Sin(D_800EBB36 + (i << 10)) / 30;
        pos.vy = Q12_MULT(Math_Sin(angle), Q12(4.0f)) - Q12(1.5f);

        temp_s0 = Q12_MULT(Math_Cos(angle), Q12(4.0f));

        rot.vy = D_800EBB38 + ((i << 12) / 12);

        pos.vx = Q12_MULT(temp_s0, Math_Sin(rot.vy)) - Q12(16.0f);
        pos.vz = Q12_MULT(temp_s0, Math_Cos(rot.vy)) - Q12(64.0f);
        rot.vx = -angle;
        rot.vz = Q12_ANGLE(0.0f);

        WorldObjects_Add(&g_WorldObject8, &pos, &rot);
    }
}

const VECTOR3 D_800CC7C0 = { Q12(8.7f), Q12(-2.5f), Q12(-28.0f) };

const VECTOR3 D_800CC7CC = { Q12(-16.0f), Q12(-1.5f), Q12(-64.0f) };

void func_800E73B4(q19_12 arg0) // 0x800E73B4
{
    MATRIX  sp10;
    SVECTOR sp30 = { 10, -17, 30 };
    VECTOR  sp38;
    VECTOR  sp48;

    if (arg0 > Q12(6.0f) && arg0 < Q12(8.0f))
    {
        Vw_CoordHierarchyMatrixCompute(&g_SysWork.playerBoneCoords[HarryBone_RightHand], &sp10);

        gte_SetRotMatrix(&sp10);
        gte_SetTransMatrix(&sp10);
        gte_ldv0(&sp30);
        gte_rt();
        gte_stlvnl(&sp38);

        sp48.vx = Q8_TO_Q12(sp38.vx);
        sp48.vy = Q8_TO_Q12(sp38.vy);
        sp48.vz = Q8_TO_Q12(sp38.vz);

#if VERSION_EQUAL_OR_NEWER(USA) // @bugfix? TODO: figure out what the changed 3rd param affects.
        func_8005F6B0(&g_SysWork.npcs[0], &sp48, 9, 15);
#else
        func_8005F6B0(&g_SysWork.npcs[0], &sp48, 9, 1);
#endif
    }
}

void func_800E74C4(void) // 0x800E74C4
{
    TILE*     tile;
    DR_TPAGE* tpage;

    tile = GsOUT_PACKET_P;

    setRGBC0(tile, 0, 8, 48, PRIM_RECT | RECT_BLEND);
    setXY0Fast(tile, 0xFF60, 0xFF90);
    setWH(tile, SCREEN_WIDTH, FRAMEBUFFER_HEIGHT_PROGRESSIVE);
    addPrimFast(&g_OrderingTable0[g_ActiveBufferIdx].org[2], tile, 3);
    tile++;

    tpage = tile;
    setDrawTPage(tpage, 0, 1, getTPageN(0, 2, 0, 0));
    AddPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[2], tpage);
    tpage++;

    GsOUT_PACKET_P = tpage;
}

void func_800E75B8(s32 arg0, const VECTOR3* pos, s32 arg2, s32 arg3, s32 arg4) // 0x800E75B8
{
    #define CLAMP_CUSTOM(a, b, min, max, x) \
        ((((a) - ((b) - (max))) >= (max) || ((a) - ((b) - (max))) >= (min)) ? (((a) - ((b) - (max))) < (max) ? ((a) - ((b) + (x))) : (max)) : (min))

    s32 max;
    s32 balance;
    int new_var;

    #define playerChara g_SysWork.playerWork.player

    // Compute audio balance.
    balance = Vc_StereoBalanceGet(pos);
    if (arg4 == 2)
    {
        balance = 0;
    }
    else if (arg4 == 1)
    {
        balance >>= 1;

        if (D_800ED841 < balance)
        {
            D_800ED841 = CLAMP_HIGH(balance, Q12_MULT_PRECISE(g_DeltaTime, 0xC0) + D_800ED841);
        }

        if (balance < D_800ED841)
        {
            D_800ED841 = MAX(balance, D_800ED841 - Q12_MULT_PRECISE(g_DeltaTime, 0xC0));
        }

        balance = D_800ED841;
    }

    D_800ED841 = balance;

    if (arg2 == 0)
    {
        arg2 = 0x80;
    }

    if (arg4 != 0)
    {
        arg2 = arg2 >> 1;
    }

    max     = UCHAR_MAX;
    new_var = 1;

    Sd_SfxAttributesUpdate(arg0, balance, CLAMP_CUSTOM((arg2 * Math_Vector3MagCalcSafe(playerChara.position.vx - pos->vx,
                                                                                  playerChara.position.vy - pos->vy,
                                                                                  playerChara.position.vz - pos->vz)) /
                                                        arg3, arg2, 0, max, new_var), 0);

    #undef playerChara
}

void func_800E7944(u32 arg0) // 0x800E7944
{
    typedef struct
    {
        MATRIX  field_0;
        SVECTOR field_20;
        VECTOR  field_28;
    } s_func_800E7944;

    s32              i;
    s32              temp_a3;
    s_func_800E7944* ptr;

    ptr = PSX_SCRATCH;

    if (arg0 != 3)
    {
        if (D_800EBB7C != 0)
        {
            Math_SetSVectorFastSum(&ptr->field_20, 0, 0xFFF2, 20);
            Vw_CoordHierarchyMatrixCompute(&g_SysWork.npcBoneCoordBuffer[0x1F], &ptr->field_0);
        }
        else
        {
            Math_SetSVectorFastSum(&ptr->field_20, 0, 0xFFD7, -21);
            Vw_CoordHierarchyMatrixCompute(&g_SysWork.npcBoneCoordBuffer[1], &ptr->field_0);
        }

        gte_SetRotMatrix(&ptr->field_0);
        gte_SetTransMatrix(&ptr->field_0);
        gte_ldv0(&ptr->field_20);
        gte_rt();
        gte_stlvnl(&ptr->field_28);

        sharedData_800DEE50_1_s01.field_14 = ptr->field_28.vx * 16;
        sharedData_800DEE50_1_s01.field_18 = ptr->field_28.vz * 16;
        temp_a3                            = (sharedData_800DEE50_1_s01.field_8 - (ptr->field_28.vy * 16)) >> 1;

        for (i = 0; i < 0x12C; i++)
        {
            if (sharedData_800DFB7C_0_s00[i].field_A == 14)
            {
                sharedData_800DFB7C_0_s00[i].vy_8 -= temp_a3;
            }
        }

        sharedData_800DEE50_1_s01.field_8 = (u16)ptr->field_28.vy * 16;
        sharedData_800DEE50_1_s01.field_6 = sharedData_800DEE50_1_s01.field_8 - Q12(0.2f);
    }

    // TODO: De-hexing.
    if (arg0 == 0)
    {
        sharedData_800DEE50_1_s01.field_0  = 1;
        sharedData_800DEE50_1_s01.field_2  = 0;
        sharedData_800DEE50_1_s01.field_4  = 0x1C;
        sharedData_800DEE50_1_s01.field_A  = 0x266;
        sharedData_800DEE50_1_s01.field_1  = 0x1C;
        sharedData_800DEE50_1_s01.field_C  = -0x199;
        sharedData_800DEE50_1_s01.field_E  = 0x51;
        sharedData_800DEE50_1_s01.field_1C = 0x8000;
        sharedData_800DEE50_1_s01.field_12 = -0x80;

        sharedFunc_800CB7F4_1_s01();
        D_800EBB7C = 0;
    }
    else if (arg0 == 1)
    {
        sharedData_800DEE50_1_s01.field_A  = 0x11E;
        sharedData_800DEE50_1_s01.field_1  = 0x10;
        sharedData_800DEE50_1_s01.field_12 = 0;
        D_800EBB7C                         = arg0;
    }
    else if (arg0 == 3)
    {
        D_800C4414 &= 0xFD;

        for (i = 0; i < 300; i++)
        {
            if (sharedData_800DFB7C_0_s00[i].field_A == 13 || sharedData_800DFB7C_0_s00[i].field_A == 14)
            {
                sharedData_800DFB7C_0_s00[i].field_A = 0;
            }
        }
    }
}
