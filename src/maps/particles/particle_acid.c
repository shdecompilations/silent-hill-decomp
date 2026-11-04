#include "inline_no_dmpsx.h"

#include <psyq/gtemac.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "main/rng.h"
#include "maps/shared.h"

void sharedFunc_800CB0A4_4_s03(VECTOR3* from, VECTOR3* to)
{
    s32 idx;

    idx = func_8005E7E0(33);
    if (idx != NO_VALUE)
    {
        sharedData_800DFB7C_0_s00[idx].field_0.vx_0         = from->vx;
        sharedData_800DFB7C_0_s00[idx].vy_8                 = from->vy;
        sharedData_800DFB7C_0_s00[idx].field_4.vz_4         = from->vz;
        sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_0  = ratan2(to->vx - from->vx, to->vz - from->vz);
        sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_2  = ratan2(to->vy - from->vy, Math_Vector2MagCalcSafeQ6(to->vx - from->vx, to->vz - from->vz));
        sharedData_800DFB7C_0_s00[idx].field_10.s_0.field_0 = D_800C4418.field_8;
        sharedData_800DFB7C_0_s00[idx].field_B              = 0;
        sharedData_800DFB7C_0_s00[idx].field_10.s_0.field_2 = Q12(1.0f);
    }
}

bool sharedFunc_800CB1B0_4_s03(POLY_FT4** poly, s32 idx)
{
    typedef struct
    {
        s_func_8005E89C field_0;
        VECTOR3         field_12C; // Q19.12
        SVECTOR         field_138;
        DVECTOR         field_140;
        s32             field_144;
        s16             field_148;
        s16             field_14A;
        s16             field_14C;
        s16             field_14E;
        s32             field_150;
        s32             field_154;
        s32             field_158;
        s32             field_15C;
    } s_func_800CB1B0;

    s_func_8005E89C    sp10;
    s_RayTrace         trace;
    s_CollisionSurface surface;
    VECTOR3            dir;
    VECTOR3            rayFrom;
    s16                temp_v0_9;
    s32                temp_a3;
    s32                temp_s1;
    s32                i;
    POLY_FT4*          next;
    s_func_800CB1B0*   ptr;
    s_SubCharacter*    sub;

    if (sharedData_800DFB7C_0_s00[idx].field_10.s_3.field_0 == 0 ||
        sharedData_800DFB7C_0_s00[idx].field_10.s_3.field_2 == 0)
    {
        sharedData_800DFB7C_0_s00[idx].field_A = 0;
        return false;
    }

    ptr = PSX_SCRATCH;

    ptr->field_148 = Math_Sin(-sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_2);
    ptr->field_14A = Math_Cos(-sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_2);
    ptr->field_14C = Math_Sin(sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_0 + Q12_ANGLE(180.0f));
    ptr->field_14E = Math_Cos(sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_0 + Q12_ANGLE(180.0f));

    ptr->field_154 = ((D_800C4418.field_C - 1) *
                      CLAMP_HIGH(Q12_MULT_PRECISE(D_800C4418.field_0, D_800C4418.field_8 - sharedData_800DFB7C_0_s00[idx].field_10.s_3.field_0),
                                 D_800C4418.field_A) /
                      D_800C4418.field_A) +
                     1;

    for (i = 0; i < ptr->field_154; i++)
    {
        ptr->field_158 = (i * D_800C4418.field_A) / D_800C4418.field_C;

        ptr->field_12C.vy = sharedData_800DFB7C_0_s00[idx].vy_8 + Q12_MULT(ptr->field_158, ptr->field_148);
        temp_s1           = Q12_MULT(ptr->field_158, ptr->field_14A);
        ptr->field_12C.vx = sharedData_800DFB7C_0_s00[idx].field_0.vx_0 + Q12_MULT(temp_s1, ptr->field_14C);
        ptr->field_12C.vz = sharedData_800DFB7C_0_s00[idx].field_4.vz_4 + Q12_MULT(temp_s1, ptr->field_14E);

        Math_SetSVectorFastSum(&ptr->field_138,
                               Q12_TO_Q8(ptr->field_12C.vx) - (u16)ptr->field_0.field_0.vx,
                               Q12_TO_Q8(ptr->field_12C.vy) - ptr->field_0.field_0.vy,
                               Q12_TO_Q8(ptr->field_12C.vz) - ptr->field_0.field_0.vz);

        gte_ldv0(&ptr->field_138);
        gte_rtps();
        gte_stsxy(&ptr->field_140);
        gte_stsz(&ptr->field_144);

        if (ptr->field_144 <= 0 || (ptr->field_144 >> 3) >= ORDERING_TABLE_SIZE)
        {
            continue;
        }

        if (ABS(ptr->field_140.vx) > 200 || ABS(ptr->field_140.vy) > 160)
        {
            continue;
        }

        temp_a3        = (i + D_800C4418.field_C) - ptr->field_154;
        ptr->field_150 = Q12_TO_Q8(((D_800C4418.field_2 * (D_800C4418.field_C - temp_a3) + D_800C4418.field_4 * temp_a3) /
                                    D_800C4418.field_C * ptr->field_0.field_2C) /
                                   ptr->field_144);

        setPolyFT4(*poly);

        setXY0Fast(*poly, (u16)ptr->field_140.vx - (u16)ptr->field_150, ptr->field_140.vy + ptr->field_150);
        setXY1Fast(*poly, (u16)ptr->field_140.vx + (u16)ptr->field_150, ptr->field_140.vy + ptr->field_150);
        setXY2Fast(*poly, (u16)ptr->field_140.vx - (u16)ptr->field_150, ptr->field_140.vy - ptr->field_150);
        setXY3Fast(*poly, (u16)ptr->field_140.vx + (u16)ptr->field_150, ptr->field_140.vy - ptr->field_150);

        ptr->field_15C = 128 - ((temp_a3 << 7) / (D_800C4418.field_C + 1));
        ptr->field_15C = (ptr->field_15C * func_80055D78(ptr->field_12C.vx, ptr->field_12C.vy, ptr->field_12C.vz)) >> 8;

        if (sharedData_800DFB7C_0_s00[idx].field_10.s_3.field_0 < (D_800C4418.field_8 >> 1))
        {
            ptr->field_15C = (ptr->field_15C * sharedData_800DFB7C_0_s00[idx].field_10.s_3.field_0) / (D_800C4418.field_8 >> 1);
        }

        ptr->field_15C = Q12_MULT_PRECISE(ptr->field_15C, sharedData_800DFB7C_0_s00[idx].field_10.s_3.field_2);

        if (ptr->field_15C == 0)
        {
            sharedData_800DFB7C_0_s00[idx].field_A = 0;
        }

        setRGB0Fast(*poly, ptr->field_15C >> 2, ptr->field_15C >> 2, ptr->field_15C >> 2);
        setSemiTrans(*poly, 1);

        *(s32*)&(*poly)->u0 = ((i & 1) << 6) + ((i & 2) ? 0xE4000 : 0xE0000);
        *(s32*)&(*poly)->u1 = ((i & 1) ? 0x7F : 0x3F) + ((i & 2) ? 0x2D4000 : 0x2D0000);
        *(u16*)&(*poly)->u2 = (!(i & 1) ? 0 : 1 << 6) + ((i & 2) ? 0x7F00 : 0x3F00);
        *(u16*)&(*poly)->u3 = ((i & 1) ? 0x7F : 0x3F) + (!(i & 2) ? 0x3F00 : 0x7F00);

        next  = *poly + 1;
        *next = *(*poly);

        addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[(ptr->field_144 - 0x20) >> 3], *poly);
        *poly += 1;

        setRGB0Fast(*poly, ptr->field_15C, ptr->field_15C, ptr->field_15C);

        (*poly)->tpage = 77;
        (*poly)->clut  = 78;

        addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[(ptr->field_144 - 0x20) >> 3], *poly);
        *poly += 1;
    }

    sp10 = ptr->field_0;

    if (sharedData_800DFB7C_0_s00[idx].field_B != 0)
    {
        temp_v0_9 = Q12_MULT_PRECISE(g_DeltaTime, D_800C4418.field_0 >> 2);
    }
    else
    {
        temp_v0_9 = Q12_MULT_PRECISE(g_DeltaTime, D_800C4418.field_0);
    }

    dir.vy = Q12_MULT(temp_v0_9, Math_Sin(sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_2));
    temp_s1  = Q12_MULT(temp_v0_9, Math_Cos(sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_2));
    dir.vx = Q12_MULT(temp_s1, Math_Sin(sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_0));
    dir.vz = Q12_MULT(temp_s1, Math_Cos(sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_0));

    sharedData_800DFB7C_0_s00[idx].field_0.vx_0 += dir.vx;
    sharedData_800DFB7C_0_s00[idx].vy_8         += dir.vy;
    sharedData_800DFB7C_0_s00[idx].field_4.vz_4 += dir.vz;

    rayFrom.vx = sharedData_800DFB7C_0_s00[idx].field_0.vx_0;
    rayFrom.vy = sharedData_800DFB7C_0_s00[idx].vy_8;
    rayFrom.vz = sharedData_800DFB7C_0_s00[idx].field_4.vz_4;

    dir.vx = ptr->field_12C.vx - sharedData_800DFB7C_0_s00[idx].field_0.vx_0;
    dir.vy = ptr->field_12C.vy - sharedData_800DFB7C_0_s00[idx].vy_8;
    dir.vz = ptr->field_12C.vz - sharedData_800DFB7C_0_s00[idx].field_4.vz_4;

    Ray_GeomAndCharaTraceGet(&trace, &rayFrom, &dir, g_SysWork.npcs);
    Collision_SurfaceGet(&surface, rayFrom.vx + dir.vx, rayFrom.vz + dir.vz);

    ptr->field_0 = sp10;

    gte_SetRotMatrix(&ptr->field_0.worldToScreenMat);
    gte_SetTransMatrix(&ptr->field_0.worldToScreenMat);

    sharedData_800DFB7C_0_s00[idx].field_10.s_0.field_0 = CLAMP_LOW(sharedData_800DFB7C_0_s00[idx].field_10.s_3.field_0 - g_DeltaTime, 0);

    if (sharedData_800DFB7C_0_s00[idx].field_B != 0)
    {
        sharedData_800DFB7C_0_s00[idx].field_10.s_0.field_2 = CLAMP_LOW(sharedData_800DFB7C_0_s00[idx].field_10.s_3.field_2 - (g_DeltaTime << 2), 0);
    }
    else if (trace.hasHit || surface.floorHeight < sharedData_800DFB7C_0_s00[idx].vy_8)
    {
        sharedData_800DFB7C_0_s00[idx].field_B++;

        if (trace.hasHit && trace.character == &g_SysWork.playerWork.player)
        {
            sub = trace.character;

            sharedFunc_800CBE54_4_s03(&trace.target, 1);
            sharedFunc_800CBE54_4_s03(&rayFrom, 1);

            if (g_SysWork.npcs[0].model.charaId == Chara_Floatstinger)
            {
                sub->attackReceived = 60;
            }
            else if (g_SysWork.npcs[0].model.charaId == Chara_Twinfeeler)
            {
                sub->attackReceived = 62;
            }
        }
        else
        {
            sharedFunc_800CBE54_4_s03(&rayFrom, 0);
        }
    }

    return true;
}

void sharedFunc_800CBE54_4_s03(VECTOR3* arg0, s32 arg1)
{
    s32 idx;
    s32 i;

    for (i = 0; i < 12; i++)
    {
        idx = func_8005E7E0(34);
        if (idx == NO_VALUE)
        {
            break;
        }

        sharedData_800DFB7C_0_s00[idx].field_0.vx_0 = Rng_AddGeneratedUInt(arg0->vx, Q12(-0.25f), Q12(0.25f) - 1);
        sharedData_800DFB7C_0_s00[idx].vy_8         = Rng_AddGeneratedUInt(arg0->vy, Q12(-0.25f), Q12(0.25f) - 1);

        sharedData_800DFB7C_0_s00[idx].field_4.vz_4        = Rng_AddGeneratedUInt(arg0->vz, Q12(-0.25f), Q12(0.25f) - 1);
        sharedData_800DFB7C_0_s00[idx].field_B             = arg1;
        sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_0 = Rng_GenerateInt(0, Q12(0.35f) - 1);

        sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_2  = Q12_MULT_PRECISE(D_800C4418.field_6, Rng_GenerateUInt(Q12(0.5f), Q12_CLAMPED(1.0f)));
        sharedData_800DFB7C_0_s00[idx].field_10.s_2.field_0 = Rng_GenerateUInt(0x40, 0x80 - 1);
    }
}

bool sharedFunc_800CC004_4_s03(POLY_FT4** poly, s32 idx)
{
    typedef struct
    {
        s_func_8005E89C field_0;
        SVECTOR         field_12C;
        DVECTOR         field_134;
        s32             field_138;
        s32             field_13C;
        s32             field_140;
    } s_func_800CC004;

    POLY_FT4*        next;
    s_func_800CC004* ptr;

    ptr = PSX_SCRATCH;

    sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_0 += g_DeltaTime;

    if (sharedData_800DFB7C_0_s00[idx].field_C.s_2.field_0 > Q12(1.4f))
    {
        sharedData_800DFB7C_0_s00[idx].field_A = 0;
        return false;
    }

    Math_SetSVectorFastSum(&ptr->field_12C,
                           Q12_TO_Q8(sharedData_800DFB7C_0_s00[idx].field_0.vx_0) - (u16)ptr->field_0.field_0.vx,
                           Q12_TO_Q8(sharedData_800DFB7C_0_s00[idx].vy_8) - ptr->field_0.field_0.vy,
                           Q12_TO_Q8(sharedData_800DFB7C_0_s00[idx].field_4.vz_4) - ptr->field_0.field_0.vz);

    gte_ldv0(&ptr->field_12C);
    gte_rtps();
    gte_stsxy(&ptr->field_134);
    gte_stsz(&ptr->field_138);

    if (ptr->field_138 <= 0 || (ptr->field_138 >> 3) >= ORDERING_TABLE_SIZE)
    {
        return false;
    }

    if (ABS(ptr->field_134.vx) > 200 || ABS(ptr->field_134.vy) > 160)
    {
        return false;
    }

    setPolyFT4(*poly);

    ptr->field_13C = (ptr->field_0.field_2C * Q12_MULT_PRECISE(sharedData_800DFB7C_0_s00[idx].field_C.s_2.field_2,
                                                               Q12(sharedData_800DFB7C_0_s00[idx].field_C.s_2.field_0 >> 1) /
                                                               5734 + 0x800) /
                                              ptr->field_138) >> 4;

    setXY0Fast(*poly, (u16)ptr->field_134.vx - (u16)ptr->field_13C, ptr->field_134.vy + ptr->field_13C);
    setXY1Fast(*poly, (u16)ptr->field_134.vx + (u16)ptr->field_13C, ptr->field_134.vy + ptr->field_13C);
    setXY2Fast(*poly, (u16)ptr->field_134.vx - (u16)ptr->field_13C, ptr->field_134.vy - ptr->field_13C);
    setXY3Fast(*poly, (u16)ptr->field_134.vx + (u16)ptr->field_13C, ptr->field_134.vy - ptr->field_13C);

    if (sharedData_800DFB7C_0_s00[idx].field_C.s_2.field_0 > Q12(0.4f))
    {
        ptr->field_140 = sharedData_800DFB7C_0_s00[idx].field_10.s_2.field_0 - (sharedData_800DFB7C_0_s00[idx].field_10.s_2.field_0 * (sharedData_800DFB7C_0_s00[idx].field_C.s_2.field_0 - Q12(0.4f))) / Q12(1.0f);
    }
    else
    {
        ptr->field_140 = (sharedData_800DFB7C_0_s00[idx].field_10.s_2.field_0 * sharedData_800DFB7C_0_s00[idx].field_C.s_2.field_0) / Q12(0.4f);
    }

    ptr->field_140 = (ptr->field_140 * (func_80055D78(sharedData_800DFB7C_0_s00[idx].field_0.vx_0, sharedData_800DFB7C_0_s00[idx].vy_8, sharedData_800DFB7C_0_s00[idx].field_4.vz_4))) >> 8;

    setRGB0Fast(*poly, ptr->field_140 >> 1, ptr->field_140 >> 1, ptr->field_140 >> 1);
    setSemiTrans(*poly, 1);

    *(s32*)&(*poly)->u0 = (((sharedData_800DFB7C_0_s00[idx].field_10.s_2.field_0 & 1) == 0) << 6) +
                          (!(sharedData_800DFB7C_0_s00[idx].field_10.s_2.field_0 & 2) ? 0xE4000 : 0xE0000);

    *(s32*)&(*poly)->u1 = (!(sharedData_800DFB7C_0_s00[idx].field_10.s_2.field_0 & 1) ? 0x7F : 0x3F) +
                          (!(sharedData_800DFB7C_0_s00[idx].field_10.s_2.field_0 & 2) ? 0x2D4000 : 0x2D0000);

    *(u16*)&(*poly)->u2 = (((sharedData_800DFB7C_0_s00[idx].field_10.s_2.field_0 & 1) == 0) << 6) |
                          (!(sharedData_800DFB7C_0_s00[idx].field_10.s_2.field_0 & 2) ? 0x7F00 : 0x3F00);

    *(u16*)&(*poly)->u3 = (!(sharedData_800DFB7C_0_s00[idx].field_10.s_2.field_0 & 1) ? 0x7F : 0x3F) |
                          (!(sharedData_800DFB7C_0_s00[idx].field_10.s_2.field_0 & 2) ? 0x7F00 : 0x3F00);

    if (sharedData_800DFB7C_0_s00[idx].field_B != 0)
    {
        ptr->field_138 -= 0x80;
    }
    else
    {
        ptr->field_138 -= 0x10;
    }

    next  = *poly + 1;
    *next = *(*poly);

    addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[ptr->field_138 >> 3], *poly);
    *poly += 1;

    setRGB0Fast(*poly, ptr->field_140, ptr->field_140, ptr->field_140);

    (*poly)->tpage = 77;
    (*poly)->clut  = 78;

    addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[ptr->field_138 >> 3], *poly);
    *poly += 1;

    return true;
}
