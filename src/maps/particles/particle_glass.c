#include "inline_no_dmpsx.h"

#include <psyq/gtemac.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "main/rng.h"
#include "maps/shared.h"

// Glass-breaking particle effect? Included in M0S01 and M7S01
// Called by cutscene event code.
// TODO: Make this separate split in each map instead of `#include`.

void sharedFunc_800CCB8C_0_s01(VECTOR* arg0, VECTOR* arg1, s16 arg2, s32 arg3, s16 arg4, s16 arg5, s32 arg6, s32 arg7)
{
    typedef struct
    {
        VECTOR3 field_0;
        MATRIX  field_C;
        SVECTOR field_2C;
        VECTOR  field_34;
        s32     field_44;
        s16     field_48;
        s16     field_4A;
        s32     field_4C;
    } s_sharedFunc_800CCB8C_0_s01;

    s32                          sp20;
    s32                          sp24;
    s32                          var_s5;
    s32                          var_s7;
    s32                          temp_lo;
    s32                          temp_lo_2;
    s32                          temp_s0_2;
    s32                          temp_v0_8;
    s16                          var_s1;
    s32                          var_v0_2;
    s_sharedFunc_800CCB8C_0_s01* ptr;

    ptr = PSX_SCRATCH;

    sharedData_800DFB10_0_s01[arg7].vx = arg0->vx;
    sharedData_800DFB10_0_s01[arg7].vy = arg0->vz;
    sharedData_800DFB10_0_s01[arg7].vz = arg3;

    var_s1 = arg5;

    if (var_s1 != 0)
    {
        ptr->field_4C = 0;
    }
    else
    {
        ptr->field_4C = 1;
        var_s1        = arg4;
    }

    ptr->field_0.vx = arg0->vx - arg1->vx;
    ptr->field_0.vy = arg0->vy - arg1->vy;
    ptr->field_0.vz = arg0->vz - arg1->vz;

    ptr->field_48 = ratan2(ptr->field_0.vy, ptr->field_0.vz);
    ptr->field_4A = ratan2(ptr->field_0.vx, ptr->field_0.vz);

    Math_SetSVectorFastSum(&ptr->field_2C, (u16)ptr->field_48, ptr->field_4A, 0);

    Math_RotMatrixZxyNegGte(&ptr->field_2C, &ptr->field_C);
    TransMatrix(&ptr->field_C, &VECTOR_Zero);

    gte_SetRotMatrix(&ptr->field_C);
    gte_SetTransMatrix(&ptr->field_C);

    ptr->field_44 = Math_Vector2MagCalcSafeQ6(var_s1, arg4) + 1;

    for (var_s7 = -arg4; arg4 >= var_s7;)
    {
        sp24 = arg6 * 3;

        for (var_s5 = -var_s1; var_s1 >= var_s5;)
        {
            sp20 = SQUARE(var_s7 >> 6);

            if (ptr->field_4C != 0 && SQUARE(arg4 >> 6) < sp20 + SQUARE(var_s5 >> 6))
            {
                continue;
            }

            if ((Rng_Rand16() % (0x20 - (((SquareRoot0(sp20 + SQUARE(var_s5 >> 6)) << 6) * 24) / ptr->field_44))) == 0)
            {
                continue;
            }

            temp_v0_8 = func_8005E7E0(arg7 + 0x17);
            if (temp_v0_8 == NO_VALUE)
            {
                return;
            }

            Math_SetSVectorFastSum(&ptr->field_2C,
                                   (var_s7 + (Rng_Rand16() % (arg6 >> 1))) - (arg6 >> 2),
                                   (var_s5 + (Rng_Rand16() % (arg6 >> 1))) - (arg6 >> 2),
                                   0);

            gte_ldv0(&ptr->field_2C);
            gte_rt();
            gte_stlvnl(&ptr->field_34);

            sharedData_800DFB7C_0_s00[temp_v0_8].field_0.s_0.field_0 = ptr->field_34.vx;
            sharedData_800DFB7C_0_s00[temp_v0_8].vy_8                = ptr->field_34.vy + arg0->vy;
            sharedData_800DFB7C_0_s00[temp_v0_8].field_4.s_0.field_0 = ptr->field_34.vz;

            temp_s0_2 = Math_Vector3MagCalcSafe(ptr->field_34.vx, ptr->field_34.vy, ptr->field_34.vz);

            sharedData_800DFB7C_0_s00[temp_v0_8].field_10.s_0.field_2 = MIN(temp_s0_2 / (arg2 >> 10), Q12(1.0f));

            sharedData_800DFB7C_0_s00[temp_v0_8].field_B = ((Rng_Rand16() % MIN(((temp_s0_2 * 6) / ptr->field_44) + 3, 8)) * 16) - 0x80 + Rng_GenerateUInt(0, 15);

            ptr->field_34.vx += ptr->field_0.vx;
            ptr->field_34.vy += ptr->field_0.vy;
            ptr->field_34.vz += ptr->field_0.vz;

            temp_s0_2 = Math_Vector3MagCalcSafe(ptr->field_34.vx, ptr->field_34.vy, ptr->field_34.vz);

            sharedData_800DFB7C_0_s00[temp_v0_8].field_C.s_0.field_0  = (arg2 * Rng_GenerateUInt(64, 191) >> 7) * ptr->field_34.vx / temp_s0_2;
            sharedData_800DFB7C_0_s00[temp_v0_8].field_C.s_0.field_2  = (arg2 * Rng_GenerateUInt(64, 191) >> 7) * ptr->field_34.vy / temp_s0_2;
            sharedData_800DFB7C_0_s00[temp_v0_8].field_10.s_0.field_0 = (arg2 * Rng_GenerateUInt(64, 191) >> 7) * ptr->field_34.vz / temp_s0_2;

            sharedData_800DFB7C_0_s00[temp_v0_8].field_0.s_0.field_2 = Rng_AddGeneratedUInt(ptr->field_48, -255, 256);
            sharedData_800DFB7C_0_s00[temp_v0_8].field_4.s_0.field_2 = Rng_AddGeneratedUInt(ptr->field_4A, -255, 256);

            temp_lo  = (var_s5 * arg6) / var_s1;
            var_v0_2 = var_s5 + ((ABS(temp_lo) + sp24) >> 2);
            var_s5   = var_v0_2;
        }

        temp_lo_2 = (var_s7 * arg6) / arg4;
        var_v0_2  = var_s7 + ((ABS(temp_lo_2) + sp24) >> 2);
        var_s7    = var_v0_2;
    }
}

bool sharedFunc_800CD1F8_0_s01(POLY_FT4** poly, s32 idx)
{
    s32                temp_s2;
    s32                temp_s1;
    s32                temp_s1_2;
    s32                temp_s1_3;
    s32                temp_s2_2;
    s32                temp_s3;
    s32                temp_v0_6;
    s32                i;
    s32                var_v0_8;
    s32                var_v1_2;
    u8                 temp_v0_23;
    u8                 temp_v0_24;
    u8                 temp_v0_25;
    u8                 temp_v1_4;
    s_MapHdr_field_4C* mapHdrPtr;
    s_func_800CD1F8*   ptr;

    ptr = PSX_SCRATCH;

    if (sharedData_800DFB7C_0_s00[idx].field_B & 0x80)
    {
        sharedData_800DFB7C_0_s00[idx].field_10.s_3.field_2 = CLAMP_LOW(sharedData_800DFB7C_0_s00[idx].field_10.s_3.field_2 - g_DeltaTime, 0);

        ptr->field_1C0 = SQUARE((sharedData_800DFB7C_0_s00[idx].field_B - 0x80) >> 4);

        if (sharedData_800DFB7C_0_s00[idx].field_10.s_3.field_2 == 0)
        {
            sharedData_800DFB7C_0_s00[idx].field_B += 0x80;

            mapHdrPtr = &sharedData_800DFB7C_0_s00[idx];

            temp_s2 = ((ratan2(sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_2, sharedData_800DFB7C_0_s00[idx].field_10.s_0.field_0) - sharedData_800DFB7C_0_s00[idx].field_0.s_0.field_2) > 0) << 7;

            mapHdrPtr->field_10.s_0.field_2 = temp_s2 |
                                              ((ratan2(sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_0, sharedData_800DFB7C_0_s00[idx].field_10.s_0.field_0) - sharedData_800DFB7C_0_s00[idx].field_4.s_0.field_2) > 0 ? 0x8000 : 0);

            temp_s2_2 = Math_Vector2MagCalcSafeQ6(sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_2,
                                                sharedData_800DFB7C_0_s00[idx].field_10.s_0.field_0);
            temp_s1   = temp_s2_2 >> 2;
            temp_s3   = temp_s2_2 >> 3;

            sharedData_800DFB7C_0_s00[idx].field_10.s_3.field_2 += CLAMP_HIGH(((temp_s2_2 + (Rng_Rand16() % temp_s1)) - temp_s3), 0x7FFF) / 256;

            temp_s2_2 = Math_Vector2MagCalcSafeQ6(sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_0,
                                                sharedData_800DFB7C_0_s00[idx].field_10.s_0.field_0);
            temp_s1_2 = temp_s2_2 >> 2;
            temp_s3   = temp_s2_2 >> 3;

            sharedData_800DFB7C_0_s00[idx].field_10.s_3.field_2 += (CLAMP_HIGH(((temp_s2_2 + (Rng_Rand16() % temp_s1_2)) - temp_s3), 0x7FFF) / 256) << 8;
        }
    }
    else
    {
        temp_v0_6      = ratan2(sharedData_800DFB7C_0_s00[idx].field_0.s_0.field_0, sharedData_800DFB7C_0_s00[idx].field_4.s_0.field_0);
        ptr->field_1C0 = SQUARE(sharedData_800DFB7C_0_s00[idx].field_B >> 4);

        temp_s2_2 = ((ptr->field_1C0 + 7) * Q12(1.5f)) / 28;

        if (sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_0 > 0)
        {
            sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_0 = CLAMP_LOW(sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_0 - Q12_MULT_PRECISE(g_DeltaTime, Q12_MULT(temp_s2_2, Math_Sin(temp_v0_6))), 0);
        }
        else if (sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_0 < 0)
        {
            sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_0 = MIN(sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_0 - Q12_MULT_PRECISE(g_DeltaTime, Q12_MULT(temp_s2_2, Math_Sin(temp_v0_6))), 0);
        }

        if (sharedData_800DFB7C_0_s00[idx].field_10.s_0.field_0 > 0)
        {
            sharedData_800DFB7C_0_s00[idx].field_10.s_0.field_0 = CLAMP_LOW(sharedData_800DFB7C_0_s00[idx].field_10.s_0.field_0 - Q12_MULT_PRECISE(g_DeltaTime, Q12_MULT(temp_s2_2, Math_Cos(temp_v0_6))), 0);
        }
        else if (sharedData_800DFB7C_0_s00[idx].field_10.s_0.field_0 < 0)
        {
            sharedData_800DFB7C_0_s00[idx].field_10.s_0.field_0 = MIN(sharedData_800DFB7C_0_s00[idx].field_10.s_0.field_0 - Q12_MULT_PRECISE(g_DeltaTime, Q12_MULT(temp_s2_2, Math_Cos(temp_v0_6))), 0);
        }

        temp_s2_2 = (g_GravitySpeed * ((sharedData_800DFB7C_0_s00[idx].field_B >> 4) + 9)) >> 4;

        sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_2  = CLAMP_HIGH(sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_2 + temp_s2_2, 0x7FFF);
        sharedData_800DFB7C_0_s00[idx].field_0.s_0.field_0 += Q12_MULT_PRECISE(g_DeltaTime, sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_0);
        sharedData_800DFB7C_0_s00[idx].vy_8                += Q12_MULT_PRECISE(g_DeltaTime, sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_2);
        sharedData_800DFB7C_0_s00[idx].field_4.s_0.field_0 += Q12_MULT_PRECISE(g_DeltaTime, sharedData_800DFB7C_0_s00[idx].field_10.s_0.field_0);

        if (sharedData_800DFB7C_0_s00[idx].field_10.s_3.field_2 & 0x80)
        {
            sharedData_800DFB7C_0_s00[idx].field_0.s_0.field_2 += Q12_MULT_PRECISE(g_DeltaTime, (sharedData_800DFB7C_0_s00[idx].field_10.s_3.field_2 & 0x7F) << 7);
        }
        else
        {
            sharedData_800DFB7C_0_s00[idx].field_0.s_0.field_2 -= Q12_MULT_PRECISE(g_DeltaTime, (sharedData_800DFB7C_0_s00[idx].field_10.s_3.field_2 & 0x7F) << 7);
        }

        if (sharedData_800DFB7C_0_s00[idx].field_10.s_3.field_2 & 0x8000)
        {
            sharedData_800DFB7C_0_s00[idx].field_4.s_0.field_2 += Q12_MULT_PRECISE(g_DeltaTime, (sharedData_800DFB7C_0_s00[idx].field_10.s_3.field_2 >> 1) & 0x3F80);
        }
        else
        {
            sharedData_800DFB7C_0_s00[idx].field_4.s_0.field_2 -= Q12_MULT_PRECISE(g_DeltaTime, (sharedData_800DFB7C_0_s00[idx].field_10.s_3.field_2 >> 1) & 0x3F80);
        }

        Collision_SurfaceGet(&ptr->surface, sharedData_800DFB7C_0_s00[idx].field_0.s_0.field_0, sharedData_800DFB7C_0_s00[idx].field_4.s_0.field_0);

        if ((ptr->surface.floorType == FloorType_Default &&
             sharedData_800DFB7C_0_s00[idx].vy_8 > 0) ||
            ptr->surface.floorHeight < sharedData_800DFB7C_0_s00[idx].vy_8)
        {
            sharedData_800DFB7C_0_s00[idx].field_A = 0;
        }
    }

    temp_v1_4           = sharedData_800DFB7C_0_s00[idx].field_B;
    *(s32*)&(*poly)->u0 = ((temp_v1_4 & 3) << 4) + ((temp_v1_4 << 0xA) & 0x3000) + 0xE0000;
    temp_v0_23          = sharedData_800DFB7C_0_s00[idx].field_B;
    *(s32*)&(*poly)->u1 = ((temp_v0_23 & 3) << 4) + 15 + (((temp_v0_23 << 0xA) & 0x3000)) + 0x2D0000;
    temp_v0_24          = sharedData_800DFB7C_0_s00[idx].field_B;
    *(u16*)&(*poly)->u2 = ((temp_v0_24 & 3) << 4) + ((((temp_v0_24 * 4) & 0x30) + 15) << 8);
    temp_v0_25          = sharedData_800DFB7C_0_s00[idx].field_B;
    *(u16*)&(*poly)->u3 = ((temp_v0_25 & 3) << 4) + 15 + ((((temp_v0_25 * 4) & 0x30) + 15) << 8);

    temp_s1_3 = sharedData_800DFB7C_0_s00[idx].field_A - 23;

    *(s32*)&ptr->field_158[0].vx = (u16)sharedData_800DFB7C_0_s00[idx].field_0.s_0.field_2 + (sharedData_800DFB7C_0_s00[idx].field_4.s_0.field_2 << 16);
    ptr->field_158[0].vz         = 0;

    Math_RotMatrixZxyNegGte(&ptr->field_158[0], &ptr->field_138);

    ptr->field_178[0].vx = sharedData_800DFB10_0_s01[temp_s1_3].vx;
    ptr->field_178[0].vy = 0;
    ptr->field_178[0].vz = sharedData_800DFB10_0_s01[temp_s1_3].vy;

    TransMatrix(&ptr->field_138, &ptr->field_178[0]);

    gte_SetRotMatrix(&ptr->field_138);
    gte_SetTransMatrix(&ptr->field_138);

    temp_s2_2 = ((s16)sharedData_800DFB10_0_s01[temp_s1_3].vz * (ptr->field_1C0 + 4)) >> 4;

    for (i = 0; i < 4; i++)
    {
        if (!(i & 1))
        {
            var_v1_2 = -temp_s2_2 & 0xFFFF;
        }
        else
        {
            var_v1_2 = temp_s2_2 & 0xFFFF;
        }

        if (i >= 2)
        {
            var_v0_8 = var_v1_2 + (-temp_s2_2 << 16);
        }
        else
        {
            var_v0_8 = var_v1_2 + (temp_s2_2 << 16);
        }

        *(s32*)&ptr->field_158[0].vx = var_v0_8;
        ptr->field_158[0].vz         = 0;

        gte_ldv0(&ptr->field_158[0]);
        gte_rt();
        gte_stlvnl(&ptr->field_178[i]);
    }

    gte_SetRotMatrix(&ptr->field_0.worldToScreenMat);
    gte_SetTransMatrix(&ptr->field_0.worldToScreenMat);

    for (i = 0; i < ARRAY_SIZE(ptr->field_158); i++)
    {
        Math_SetSVectorFastSum(&ptr->field_158[i],
                               (Q12_TO_Q8(sharedData_800DFB7C_0_s00[idx].field_0.s_0.field_0 + ptr->field_178[i].vx)) - (q8_8)ptr->field_0.field_0.vx,
                               (Q12_TO_Q8(sharedData_800DFB7C_0_s00[idx].vy_8 + ptr->field_178[i].vy))                - ptr->field_0.field_0.vy,
                               Q12_TO_Q8((sharedData_800DFB7C_0_s00[idx].field_4.s_0.field_0 + ptr->field_178[i].vz)) - ptr->field_0.field_0.vz);
    }

    gte_ldv3c(&ptr->field_158[0]);
    gte_rtpt();
    gte_stsxy3_g3(*poly);
    gte_stsz3c(&ptr->field_1C4);
    gte_ldv0(&ptr->field_158[3]);
    gte_rtps();
    gte_stsxy(&ptr->field_1D4);
    gte_stsz(&ptr->field_1D0);

    ptr->field_1C4 = (ptr->field_1C4 + ptr->field_1C8 + ptr->field_1CC + ptr->field_1D0) >> 2;

    if (ptr->field_1C4 <= 0 || (ptr->field_1C4 >> 3) >= ORDERING_TABLE_SIZE ||
        ABS(ptr->field_1D4.vx) > 200 ||
        ABS(ptr->field_1D4.vy) > 160)
    {
        return false;
    }

    func_80055A90(&ptr->field_1B8, &ptr->field_1BC, 0x80, ptr->field_1C4 * 0x10);

    *(s32*)&(*poly)->r0 = (ptr->field_1BC.r + (ptr->field_1BC.g << 8) + (ptr->field_1BC.b << 16) + 0x2E000000);
    *(s32*)&(*poly)->x3 = *(s32*)&ptr->field_1D4.vx;

    addPrimFast(&g_OrderingTable0[g_ActiveBufferIdx].org[((ptr->field_1C4 - Q12(1.5f)) >= Q12(0.0f)) ? (ptr->field_1C4 - Q12(1.5f)) >> 3 : 0], *poly, 9);

    *poly += 1;

    return true;
}
