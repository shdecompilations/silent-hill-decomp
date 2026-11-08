#include "inline_no_dmpsx.h"

#include <psyq/gtemac.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "main/rng.h"

#if defined(MAP1_S05)
#include "maps/map1/map1_s05.h" // For `sharedData_800DFB7C_0_s00` size.
#elif defined(MAP3_S05)
#include "maps/map3/map3_s05.h" // For `sharedData_800DFB7C_0_s00` size.
#elif defined(MAP4_S01)
#include "maps/map4/map4_s01.h" // For `sharedData_800DFB7C_0_s00` size.
#endif

// Unknown drawing code included in M1S05, M3S05 and M4S01.
// Might be used by some kind of world object, based on M1S05 `Map_WorldObjectsInit` calling into this?
// TODO: Make this separate split in each map instead of `#include`.

void sharedFunc_800CAAD0_1_s05(void) // 0x800CC19C
{
    s32 i;
    s32 counterC;

    if (sharedData_800D8568_1_s05.flags & (1 << 0))
    {
        sharedData_800D8568_1_s05.field_1A = sharedData_800D8568_1_s05.field_1C * 2;
        sharedData_800D8568_1_s05.field_18 = sharedData_800D8568_1_s05.field_1A;
    }
    else
    {
        sharedData_800D8568_1_s05.field_1C = (sharedData_800D8568_1_s05.field_18 + sharedData_800D8568_1_s05.field_1A) >> 1;
    }

    sharedData_800D8568_1_s05.field_16 = sharedData_800D8568_1_s05.field_12 - sharedData_800D8568_1_s05.field_14;

    if (sharedData_800D8568_1_s05.field_1)
    {
        sharedData_800D8568_1_s05.field_10 = Q12(1.0f);
    }
    else
    {
        sharedData_800D8568_1_s05.field_10 = Q12(0.0f);
    }

    counterC = sharedData_800D8568_1_s05.field_C;

    for (i = ARRAY_SIZE(sharedData_800DFB7C_0_s00) - 1; i >= 0; i--)
    {
        if (sharedData_800DFB7C_0_s00[i].field_A == 0)
        {
            if (sharedData_800D8568_1_s05.field_1 == 0)
            {
                sharedFunc_800CABF8_1_s05(i, false);
            }
            else
            {
                sharedData_800DFB7C_0_s00[i].field_A = 12;
            }

            counterC--;
            if (counterC == 0)
            {
                break;
            }
        }
    }

    D_800C4414 |= 1 << 0;
}

void sharedFunc_800CABF8_1_s05(s32 idx, bool arg1)
{
    s32   rand;
    q3_12 randAngle;

    if (sharedData_800D8568_1_s05.field_1 == 1 &&
        Rng_GenerateUInt(0, 4095) < sharedData_800D8568_1_s05.field_10 &&
        Rng_GenerateUInt(0, 7) == 0) // 1 in 8 chance.
    {
        sharedData_800DFB7C_0_s00[idx].field_A = 12;
        return;
    }

    if ((sharedData_800D8568_1_s05.flags & (1 << 4)) &&
        Rng_GenerateUInt(0, 3) == 0) // 1 in 4 chance.
    {
        sharedData_800DFB7C_0_s00[idx].field_A = 12;
        return;
    }

    rand      = Rng_GenerateInt(0, (s16)sharedData_800D8568_1_s05.field_1C - 1);
    randAngle = Rng_GenerateUInt(0, Q12_ANGLE(360.0f) - 1);

    if ((sharedData_800D8568_1_s05.flags & (1 << 2)) &&
        Rng_GenerateUInt(0, 63) == 0) // 1 in 64 chance.
    {
        sharedData_800DFB7C_0_s00[idx].field_A = 10;
    }
    else
    {
        sharedData_800DFB7C_0_s00[idx].field_A = 8;
    }

    if (arg1 == true)
    {
        sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_2 = Rng_GenerateUInt(0, 15);
    }
    else
    {
        sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_2 = Rng_GenerateUInt(0, 4095);
    }

    if (sharedData_800D8568_1_s05.flags & (1 << 4))
    {
        sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_0 = MIN((((sharedData_800D8568_1_s05.field_3 << 4) * rand) / (s16)sharedData_800D8568_1_s05.field_1C) +
                                                                 (u16)sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_2 +
                                                                 sharedData_800D8568_1_s05.field_10 +
                                                                 Rng_GenerateUInt(0, 1023),
                                                                 4095);
    }
    else
    {
        sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_0 = MIN((((sharedData_800D8568_1_s05.field_3 << 4) * rand) / (s16)sharedData_800D8568_1_s05.field_1C) +
                                                                 (u16)sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_2 +
                                                                 sharedData_800D8568_1_s05.field_10,
                                                                 4095);
    }

    sharedData_800DFB7C_0_s00[idx].field_10.s_2.field_0 = (u8)Rng_Rand16() | 0x80; // TODO: Same as `Rng_Rand16() - 128`?
    sharedData_800DFB7C_0_s00[idx].field_10.s_2.field_1 = (u8)Rng_Rand16() | 0x80;

    sharedData_800DFB7C_0_s00[idx].field_10.s_0.field_2 = Rng_GenerateUInt(0, 4095);

    if (sharedData_800D8568_1_s05.flags & (1 << 0))
    {
        sharedData_800DFB7C_0_s00[idx].field_0.vx_0 = FP_FROM(rand * Math_Cos(randAngle), Q12_SHIFT);
        sharedData_800DFB7C_0_s00[idx].field_4.vz_4 = FP_FROM(rand * Math_Sin(randAngle), Q12_SHIFT);
    }
    else
    {
        sharedData_800DFB7C_0_s00[idx].field_0.vx_0 = Rng_GenerateInt(-sharedData_800D8568_1_s05.field_18, sharedData_800D8568_1_s05.field_18 - 1);
        sharedData_800DFB7C_0_s00[idx].field_4.vz_4 = Rng_GenerateInt(-sharedData_800D8568_1_s05.field_1A, sharedData_800D8568_1_s05.field_1A - 1);
    }

    sharedData_800DFB7C_0_s00[idx].vy_8    = sharedData_800D8568_1_s05.field_14 + FP_FROM((u16)sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_2 * sharedData_800D8568_1_s05.field_16, Q12_SHIFT);
    sharedData_800DFB7C_0_s00[idx].field_B = Rng_GenerateUInt(0, 1) + (Rng_GenerateInt(0, 2) * 2);
}

bool sharedFunc_800CB040_1_s05(POLY_FT4** poly, s32 idx)
{
    typedef struct
    {
        s_func_8005E89C field_0;
        SVECTOR         field_12C;
        q19_12          field_134;
        q19_12          field_138;
        s32             field_13C;
        DVECTOR         field_140;
        s16             field_144;
    } s_func_800CB040;

    q19_12           var_s1;
    s32              halfSizeCeil;
    s32              halfSizeFloor;
    s_func_800CB040* ptr;

    ptr = PSX_SCRATCH;

    if (sharedData_800DFB7C_0_s00[idx].field_A == 8)
    {
        *(s32*)&(*poly)->u0 = ((sharedData_800DFB7C_0_s00[idx].field_B >= 2) << 6) + ((sharedData_800DFB7C_0_s00[idx].field_B >= 3) ? 0x020EC000 : 0x020E8000);
        *(s32*)&(*poly)->u1 = ((sharedData_800DFB7C_0_s00[idx].field_B >= 2) ? 0x7F : 0x3F) + (sharedData_800DFB7C_0_s00[idx].field_B >= 3 ? 0x6DC000 : 0x6D8000);
        *(s16*)&(*poly)->u2 = ((sharedData_800DFB7C_0_s00[idx].field_B >= 2) << 6) | ((sharedData_800DFB7C_0_s00[idx].field_B >= 3) ? 0xFF00 : 0xBF00);
        *(s16*)&(*poly)->u3 = ((sharedData_800DFB7C_0_s00[idx].field_B >= 2) ? 0x7F : 0x3F) | ((sharedData_800DFB7C_0_s00[idx].field_B >= 3) ? 0xFF00 : 0xBF00);

        // TODO: Might be single line statements / macros like above?
        if ((u16)sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_0 < 0xE00)
        {
            POLY_FT4* poly2     = *poly;
            s32       var_v1_3  = 0xB4 + ((0xC4 - ((u16)sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_0 >> 5)) << 8) + -(((u16)sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_0 >> 7));
            s32       var_v0_6  = var_v1_3 + (MAX(0, 0xC4 - ((u16)sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_0 >> 4)) << 0x10) + 0x2E000000;
            *(s32*)&(poly2->r0) = var_v0_6;
        }
        else
        {
            POLY_FT4* poly2     = *poly;
            s32       var_a0    = MAX(0, 0x418 - ((u16)sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_0 >> 2));
            s32       var_v0_7  = var_a0 + (MAX(0, 0x3D4 - ((u16)sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_0 >> 2)) << 8) + 0x2E000000;
            *(s32*)&(poly2->r0) = var_v0_7;
        }
    }
    else
    {
        setUV0AndClutSum(*poly, 0, 120, 590);
        setUV1AndTPageSum(*poly, 7, 120, 77);
        setUV2Sum(*poly, 0, 127);
        setUV3Sum(*poly, 7, 127);
        setRGBC0(*poly, 120, 128, 128, PRIM_POLY | RECT_BLEND | RECT_TEXTURE | RECT_SIZE_1);
    }

    sharedData_800DFB7C_0_s00[idx].vy_8 = sharedData_800DFB7C_0_s00[idx].vy_8 + ptr->field_0.field_C4;

    var_s1 = (sharedData_800D8568_1_s05.field_8 * sharedData_800DFB7C_0_s00[idx].field_10.s_2.field_1) >> 8;

    ptr->field_134 = sharedData_800DFB7C_0_s00[idx].field_0.vx_0 +
                     Q12_MULT(Math_Sin(sharedData_800DFB7C_0_s00[idx].field_10.s_0.field_2), var_s1);

    ptr->field_138 = sharedData_800DFB7C_0_s00[idx].field_4.vz_4 +
                     Q12_MULT(Math_Cos(sharedData_800DFB7C_0_s00[idx].field_10.s_0.field_2), var_s1);

    if ((q4_12)sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_2 < (q4_12)ptr->field_0.field_C8)
    {
        q19_12 angle = func_8005C7B0(Q12_ANGLE(90.0f) - (((u16)sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_2 << 6) / sharedData_800D8568_1_s05.field_4));
        var_s1    = Q12_MULT((q4_12)ptr->field_0.field_CA, Math_Sin(angle)) + (q4_12)ptr->field_0.field_CC;

        ptr->field_134 = Q12_MULT(ptr->field_134, var_s1);
        ptr->field_138 = Q12_MULT(ptr->field_138, var_s1);
    }

    ptr->field_134 += ptr->field_0.field_D0;
    ptr->field_138 += ptr->field_0.field_D4;

    Math_SetSVectorFastSum(&ptr->field_12C,
                           Q12_TO_Q8(ptr->field_134)                      - (q8_8)ptr->field_0.field_0.vx,
                           Q12_TO_Q8(sharedData_800DFB7C_0_s00[idx].vy_8) - ptr->field_0.field_0.vy,
                           Q12_TO_Q8(ptr->field_138)                      - ptr->field_0.field_0.vz);

    if (sharedData_800DFB7C_0_s00[idx].field_B & (1 << 0))
    {
        sharedData_800DFB7C_0_s00[idx].field_10.s_0.field_2 += Q12_MULT_PRECISE(g_DeltaTime, ((u16)sharedData_800D8568_1_s05.field_A * sharedData_800DFB7C_0_s00[idx].field_10.s_2.field_0) >> 8);
    }
    else
    {
        sharedData_800DFB7C_0_s00[idx].field_10.s_0.field_2 -= Q12_MULT_PRECISE(g_DeltaTime, ((u16)sharedData_800D8568_1_s05.field_A * sharedData_800DFB7C_0_s00[idx].field_10.s_2.field_0) >> 8);
    }

    sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_0 += ptr->field_0.field_CE;
    sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_2 += ptr->field_0.field_CE;

    if ((u16)sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_0 >= Q12_ANGLE(360.0f))
    {
        if (sharedData_800D8568_1_s05.flags & (1 << 1) && sharedData_800DFB7C_0_s00[idx].field_A == 8)
        {
            sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_0 = (u16)sharedData_800D8568_1_s05.field_10 + Rng_GenerateUInt(0, 0xFFF);
            if ((sharedData_800D8568_1_s05.flags & (1 << 3)) &&
                Rng_GenerateUInt(0, 15) == 0) // 1 in 16 chance.
            {
                sharedData_800DFB7C_0_s00[idx].field_A = 11;
            }
            else
            {
                sharedData_800DFB7C_0_s00[idx].field_A = 9;
            }
        }
        else
        {
            if (sharedData_800D8568_1_s05.flags & (1 << 3) &&
                Rng_GenerateUInt(0, 15) == 0) // 1 in 16 chance.
            {
                s32 randVal                                        = Rng_GenerateUInt(Q12(0.5f), Q12_CLAMPED(1.0f));
                sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_0 = randVal + (sharedData_800D8568_1_s05.field_10 >> 1);
                sharedData_800DFB7C_0_s00[idx].field_A             = 11;
            }
            else
            {
                sharedFunc_800CABF8_1_s05(idx, true);
            }

            return 0;
        }
    }

    gte_ldv0(&ptr->field_12C);
    gte_rtps();
    gte_stsxy(&ptr->field_140);
    gte_stsz(&ptr->field_13C);

    if (ptr->field_13C <= 0 || (ptr->field_13C >> 3) >= ORDERING_TABLE_SIZE ||
        ABS(ptr->field_140.vx) > 200 ||
        ABS(ptr->field_140.vy) > 160)
    {
        return false;
    }

    if (sharedData_800DFB7C_0_s00[idx].field_A != 8)
    {
        var_s1 = (ptr->field_0.field_2C * 4) / ptr->field_13C;
    }
    else
    {
        var_s1 = ptr->field_0.field_D8 / ptr->field_13C;
    }

    var_s1 = MAX(var_s1, 2);

    // `ptr->field_140` contains a base X/Y position for the poly.
    // Signed half-size offsets are then added (ceil/floor) to generate
    // four corner vertices of a centered quad.
    halfSizeCeil  = (var_s1 + 1) >> 1;
    halfSizeFloor = -(var_s1 >> 1);

    *(s32*)&(*poly)->x0 = *(s32*)&ptr->field_140 + halfSizeCeil + (halfSizeCeil << 16);
    *(s32*)&(*poly)->x1 = *(s32*)&ptr->field_140 + halfSizeFloor + (halfSizeCeil << 16);
    *(s32*)&(*poly)->x2 = *(s32*)&ptr->field_140 + halfSizeCeil + (halfSizeFloor << 16);
    *(s32*)&(*poly)->x3 = *(s32*)&ptr->field_140 + halfSizeFloor + (halfSizeFloor << 16);

    addPrimFast(&g_OrderingTable0[g_ActiveBufferIdx].org[ptr->field_13C >> 3], *poly, 9);
    *poly += 1;

    return true;
}

bool sharedFunc_800CB884_1_s05(POLY_FT4** poly, s32 idx) // 0x800CCF50
{
    typedef struct
    {
        s_func_8005E89C field_0;
        SVECTOR         field_12C;
        s32             field_134;
        s32             field_138;
        s32             field_13C;
        DVECTOR         field_140;
        s16             field_144;
    } s_func_800CB884;

    s32              var_s1;
    s_func_800CB884* ptr;

    ptr = PSX_SCRATCH;

    if (sharedData_800DFB7C_0_s00[idx].field_A == 9)
    {
        *(s32*)&(*poly)->u0 = 0x024EC000;
        *(s32*)&(*poly)->u1 = 0x4DC03F;
        *(u16*)&(*poly)->u2 = 0xFF00;
        *(u16*)&(*poly)->u3 = 0xFF3F;

        ptr->field_144 = (sharedData_800DFB7C_0_s00[idx].field_C.s_2.field_0 >> 6) + 0x24;

        var_s1 = MAX(0x40 - (sharedData_800DFB7C_0_s00[idx].field_C.s_2.field_0 >> 6), 0);

        setRGBC0(*poly, var_s1, var_s1, var_s1, PRIM_POLY | RECT_BLEND | RECT_TEXTURE | RECT_SIZE_1);
    }
    else
    {
        *(s32*)&(*poly)->u0 = 0x020E7800;
        *(s32*)&(*poly)->u1 = 0xD7807;
        *(u16*)&(*poly)->u2 = 0x7F00;
        *(u16*)&(*poly)->u3 = 0x7F07;

        ptr->field_144 = 2;

        setRGBC0(*poly, 0xFF, CLAMP_LOW(0xC0 - (sharedData_800DFB7C_0_s00[idx].field_C.s_2.field_0 >> 6), 0), 0x80, PRIM_POLY | RECT_BLEND | RECT_TEXTURE | RECT_SIZE_1);
    }

    sharedData_800DFB7C_0_s00[idx].vy_8 = sharedData_800DFB7C_0_s00[idx].vy_8 + ptr->field_0.field_C4;

    var_s1 = (sharedData_800D8568_1_s05.field_8 * sharedData_800DFB7C_0_s00[idx].field_10.s_2.field_1) >> 8;

    ptr->field_134 = sharedData_800D8568_1_s05.field_20 + sharedData_800DFB7C_0_s00[idx].field_0.vx_0 +
                     Q12_MULT(Math_Sin(sharedData_800DFB7C_0_s00[idx].field_10.s_0.field_2), var_s1);

    ptr->field_138 = sharedData_800D8568_1_s05.field_24 + sharedData_800DFB7C_0_s00[idx].field_4.vz_4 +
                     Q12_MULT(Math_Cos(sharedData_800DFB7C_0_s00[idx].field_10.s_0.field_2), var_s1);

    Math_SetSVectorFastSum(&ptr->field_12C,
                           Q12_TO_Q8(ptr->field_134)                      - (q8_8)ptr->field_0.field_0.vx,
                           Q12_TO_Q8(sharedData_800DFB7C_0_s00[idx].vy_8) - ptr->field_0.field_0.vy,
                           Q12_TO_Q8(ptr->field_138)                      - ptr->field_0.field_0.vz);

    if (sharedData_800DFB7C_0_s00[idx].field_B & 1)
    {
        sharedData_800DFB7C_0_s00[idx].field_10.s_0.field_2 += Q12_MULT_PRECISE(g_DeltaTime, (ptr->field_0.field_C6 * sharedData_800DFB7C_0_s00[idx].field_10.s_2.field_0) >> 8);
    }
    else
    {
        sharedData_800DFB7C_0_s00[idx].field_10.s_0.field_2 -= Q12_MULT_PRECISE(g_DeltaTime, (ptr->field_0.field_C6 * sharedData_800DFB7C_0_s00[idx].field_10.s_2.field_0) >> 8);
    }

    sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_0 += ptr->field_0.field_CE;

    if (sharedData_800DFB7C_0_s00[idx].field_C.s_2.field_0 > Q12(1.0f))
    {
        sharedFunc_800CABF8_1_s05(idx, 1);
        return false;
    }

    gte_ldv0(&ptr->field_12C);
    gte_rtps();
    gte_stsxy(&ptr->field_140);
    gte_stsz(&ptr->field_13C);

    if (ptr->field_13C <= 0 || (ptr->field_13C >> 3) >= ORDERING_TABLE_SIZE)
    {
        return false;
    }

    if (ABS(ptr->field_140.vx) > 200)
    {
        return false;
    }

    if (ABS(ptr->field_140.vy) > 160)
    {
        return false;
    }

    var_s1 = (ptr->field_144 * ptr->field_0.field_2C) / ptr->field_13C;
    if (var_s1 != 0)
    {
        setXY0Fast(*poly, (u16)ptr->field_140.vx - var_s1, ptr->field_140.vy - var_s1);
        setXY1Fast(*poly, (u16)ptr->field_140.vx + var_s1, ptr->field_140.vy - var_s1);
        setXY2Fast(*poly, (u16)ptr->field_140.vx - var_s1, ptr->field_140.vy + var_s1);
        setXY3Fast(*poly, (u16)ptr->field_140.vx + var_s1, ptr->field_140.vy + var_s1);
    }
    else
    {
        setXY0Fast(*poly, ptr->field_140.vx, ptr->field_140.vy);
        setXY1Fast(*poly, (u16)ptr->field_140.vx + 1, ptr->field_140.vy);
        setXY2Fast(*poly, ptr->field_140.vx, ptr->field_140.vy + 1);
        setXY3Fast(*poly, (u16)ptr->field_140.vx + 1, ptr->field_140.vy + 1);
    }

    addPrimFast(&g_OrderingTable0[g_ActiveBufferIdx].org[ptr->field_13C >> 3], *poly, 9);
    *poly += 1;

    return true;
}
