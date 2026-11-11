#include "inline_no_dmpsx.h"

#include <psyq/gtemac.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "main/rng.h"
#include "maps/common/shared.h"

// Split Head drool particles included in M1S05 and M1S06.
// Handles drops that fall and splat on the floor, and strands when Split Head mouth is open.

void sharedFunc_800CBE7C_1_s05(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    s32 i;

    i = func_8005E7E0(7);
    if (i != NO_VALUE)
    {
        sharedData_800DFB7C_0_s00[i].field_0.vx_0         = Rng_AddGeneratedUInt(arg0, -32, 31) * 16;
        sharedData_800DFB7C_0_s00[i].vy_8                 = Rng_AddGeneratedUInt(arg1, 0, 63) * 16;
        sharedData_800DFB7C_0_s00[i].field_4.vz_4         = Rng_AddGeneratedUInt(arg2, -32, 31) * 16;
        sharedData_800DFB7C_0_s00[i].field_B              = 0;
        sharedData_800DFB7C_0_s00[i].field_C.s_0.field_0  = 0;
        sharedData_800DFB7C_0_s00[i].field_C.s_0.field_2  = arg3;
        sharedData_800DFB7C_0_s00[i].field_10.s_1.field_0 = arg4;
        sharedData_800DFB7C_0_s00[i].field_10.s_1.field_2 = Rng_GenerateUInt(16, 47);
        sharedData_800DFB7C_0_s00[i].field_10.s_1.field_3 = Rng_GenerateUInt(0, 15);
    }
}

bool sharedFunc_800CBF74_1_s05(POLY_FT4** poly, s32 idx)
{
    typedef struct
    {
        /* 0x0   */ s_func_8005E89C    field_0;
        /* 0x12C */ s_CollisionSurface surface;
        /* 0x138 */ SVECTOR            cameraRotation; // Q3.12
        /* 0x140 */ s32                field_140;
        /* 0x144 */ DVECTOR            field_144;
        /* 0x148 */ s32                field_148;
        /* 0x14C */ s32                field_14C;
    } s_sharedFunc_800CBF74_1_s05;

    VECTOR3                      sfxPos; // Q19.12
    s32                          temp_v0;
    q19_12                       temp_v0_2;
    s32                          var_v1;
    s_sharedFunc_800CBF74_1_s05* ptr;

    ptr = PSX_SCRATCH;

    temp_v0 = Q12_MULT(sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_2, Math_Sin(sharedData_800DFB7C_0_s00[idx].field_10.s_0.field_0));

    sharedData_800DFB7C_0_s00[idx].vy_8         += Q12_MULT_PRECISE(g_DeltaTime, sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_0);
    sharedData_800DFB7C_0_s00[idx].field_0.vx_0 += Q12_MULT_PRECISE(g_DeltaTime, temp_v0);

    temp_v0_2 = Q12_MULT(sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_2, Math_Cos(sharedData_800DFB7C_0_s00[idx].field_10.s_0.field_0));

    sharedData_800DFB7C_0_s00[idx].field_4.vz_4        += Q12_MULT_PRECISE(g_DeltaTime, temp_v0_2);
    sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_0 += g_GravitySpeed >> 1;
    sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_2  = CLAMP_LOW(sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_2 - Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(180.0f)), Q12_ANGLE(0.0f));

    Collision_SurfaceGet(&ptr->surface, sharedData_800DFB7C_0_s00[idx].field_0.vx_0, sharedData_800DFB7C_0_s00[idx].field_4.vz_4);

    if (ptr->surface.floorHeight < sharedData_800DFB7C_0_s00[idx].vy_8 || sharedData_800DFB7C_0_s00[idx].vy_8 > Q12(0.0f))
    {
        sharedData_800DFB7C_0_s00[idx].vy_8                = MIN(ptr->surface.floorHeight, 0);
        sharedData_800DFB7C_0_s00[idx].field_B             = 1;
        sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_0 = Q12_ANGLE(0.0f);

        sfxPos.vx = sharedData_800DFB7C_0_s00[idx].field_0.vx_0;
        sfxPos.vy = sharedData_800DFB7C_0_s00[idx].vy_8;
        sfxPos.vz = sharedData_800DFB7C_0_s00[idx].field_4.vz_4;

        Sfx_WithFlagsPlay(Sfx_Unk1476, &sfxPos, Q8(0.5f), SfxFlag_None);
    }

    gte_SetRotMatrix(&ptr->field_0.worldToScreenMat);
    gte_SetTransMatrix(&ptr->field_0.worldToScreenMat);

    if (ABS(PLAYER_CHARA().position.vx - sharedData_800DFB7C_0_s00[idx].field_0.vx_0) +
        ABS(PLAYER_CHARA().position.vz - sharedData_800DFB7C_0_s00[idx].field_4.vz_4) >
        Q12(20.0f))
    {
        return false;
    }

    Math_SetSVectorFastSum(&ptr->cameraRotation,
                           Q12_TO_Q8(sharedData_800DFB7C_0_s00[idx].field_0.vx_0) - (u16)ptr->field_0.field_0.vx,
                           Q12_TO_Q8(sharedData_800DFB7C_0_s00[idx].vy_8) - ptr->field_0.field_0.vy,
                           Q12_TO_Q8(sharedData_800DFB7C_0_s00[idx].field_4.vz_4) - ptr->field_0.field_0.vz);

    gte_ldv0(&ptr->cameraRotation);
    gte_rtps();
    gte_stsxy(&ptr->field_144);
    gte_stsz(&ptr->field_140);

    if ((ptr->field_140 - 16) <= 0 || ((ptr->field_140 - 16) >> 3) >= ORDERING_TABLE_SIZE ||
        ABS(ptr->field_144.vx) > 200 || ABS(ptr->field_144.vy) > 160)
    {
        return false;
    }

    vwGetViewAngle(&ptr->cameraRotation);

    ptr->field_148 = ptr->field_0.field_2C * 4 / ptr->field_140;
    ptr->field_14C = ((((ptr->cameraRotation.vx + 0x400) >> 9) + 7) * ptr->field_0.field_2C) / ptr->field_140;

    setXY0Fast(*poly, (u16)ptr->field_144.vx - (u16)ptr->field_148, ptr->field_144.vy - ptr->field_14C);
    setXY1Fast(*poly, (u16)ptr->field_144.vx + (u16)ptr->field_148, ptr->field_144.vy - ptr->field_14C);
    setXY2Fast(*poly, (u16)ptr->field_144.vx - (u16)ptr->field_148, ptr->field_144.vy + ptr->field_14C);
    setXY3Fast(*poly, (u16)ptr->field_144.vx + (u16)ptr->field_148, ptr->field_144.vy + ptr->field_14C);

    *(u32*)&(*poly)->u0 = 0x530000;
    *(u32*)&(*poly)->u1 = 0x6B001F;
    *(u16*)&(*poly)->u2 = 0x1F00;
    *(u16*)&(*poly)->u3 = 0x1F1F;

    var_v1 = ((u32)func_80055D78(sharedData_800DFB7C_0_s00[idx].field_0.vx_0, sharedData_800DFB7C_0_s00[idx].vy_8, sharedData_800DFB7C_0_s00[idx].field_4.vz_4) * 0x31) >> 5;
    var_v1 = MIN(var_v1, 0xC4);

    setRGBC0(*poly, var_v1, var_v1, var_v1, PRIM_POLY | RECT_BLEND | RECT_TEXTURE | RECT_SIZE_1);
    addPrimFast(&g_OrderingTable0[g_ActiveBufferIdx].org[(ptr->field_140 - 8) >> 3], *poly, 9);
    *poly += 1;

    return true;
}

bool sharedFunc_800CC618_1_s05(POLY_FT4** poly, s32 idx)
{
    typedef struct
    {
        s_func_8005E89C field_0;
        SVECTOR         collision;
        SVECTOR         field_134;
        SVECTOR         field_13C;
        SVECTOR         field_144;
        VECTOR3         field_14C;
        s32             field_158;
        DVECTOR         field_15C;
        s32             field_160;
        s32             field_164;
        s32             field_168;
        s32             field_16C;
        s32             field_170;
        s32             field_174;
        s32             field_178;
        s32             field_17C;
        s32             field_180;
        s32             field_184;
        s32             field_188;
        s32             field_18C;
        s32             field_190;
    } s_sharedFunc_800CC618_1_s05;

    s32                          temp_v1_7;
    s32                          temp_v1_8;
    s32                          var_v0_4;
    s32                          temp_a0_3;
    u32                          temp_v1_6;
    s32                          temp;
    s32                          temp2;
    s32                          temp3;
    s_sharedFunc_800CC618_1_s05* ptr;

    ptr = PSX_SCRATCH;

    ptr->field_160 = MAX(0xFF - ((sharedData_800DFB7C_0_s00[idx].field_C.s_2.field_0 * 16) / Q12(0.3f)), 0);

    sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_0 += g_DeltaTime;
    if (sharedData_800DFB7C_0_s00[idx].field_C.s_2.field_0 > 0x4CC0) // TODO: Demagic.
    {
        sharedData_800DFB7C_0_s00[idx].field_A = 0;
    }

    if (ABS(PLAYER_CHARA().position.vx - sharedData_800DFB7C_0_s00[idx].field_0.vx_0) +
            ABS(PLAYER_CHARA().position.vz - sharedData_800DFB7C_0_s00[idx].field_4.vz_4) >
        Q12(20.0f))
    {
        return false;
    }

    ptr->field_168 = CLAMP_HIGH(sharedData_800DFB7C_0_s00[idx].field_10.s_2.field_2, ((sharedData_800DFB7C_0_s00[idx].field_C.s_2.field_0 * sharedData_800DFB7C_0_s00[idx].field_10.s_2.field_2) / 2456) + 8);

    Math_SetSVectorFastSum(&ptr->collision,
                           (Q12_TO_Q8(sharedData_800DFB7C_0_s00[idx].field_0.vx_0) - (u16)ptr->field_0.field_0.vx) - (u16)ptr->field_168,
                           Q12_TO_Q8(sharedData_800DFB7C_0_s00[idx].vy_8) - ptr->field_0.field_0.vy,
                           Q12_TO_Q8(sharedData_800DFB7C_0_s00[idx].field_4.vz_4) - ptr->field_0.field_0.vz + (u16)ptr->field_168);

    // TODO: Use `Math_SetSVectorFastSum`.
    *(s32*)&ptr->field_134 = (((u16)ptr->field_168 + (Q12_TO_Q8(sharedData_800DFB7C_0_s00[idx].field_0.vx_0) - (u16)ptr->field_0.field_0.vx)) & 0xFFFF) +
                             ((Q12_TO_Q8(sharedData_800DFB7C_0_s00[idx].vy_8) - ptr->field_0.field_0.vy) << 16);
    temp              = (u16)ptr->field_168;
    ptr->field_134.vz = temp + (Q12_TO_Q8(sharedData_800DFB7C_0_s00[idx].field_4.vz_4) - ptr->field_0.field_0.vz);

    // TODO: Use `Math_SetSVectorFastSum`.
    *(s32*)&ptr->field_13C = (((Q12_TO_Q8(sharedData_800DFB7C_0_s00[idx].field_0.vx_0) - (u16)ptr->field_0.field_0.vx) - (u16)ptr->field_168) & 0xFFFF) +
                             ((Q12_TO_Q8(sharedData_800DFB7C_0_s00[idx].vy_8) - ptr->field_0.field_0.vy) << 16);
    temp2             = (u16)ptr->field_168;
    ptr->field_13C.vz = (Q12_TO_Q8(sharedData_800DFB7C_0_s00[idx].field_4.vz_4) - ptr->field_0.field_0.vz) - temp2;

    // TODO: Use `Math_SetSVectorFastSum`.
    *(s32*)&ptr->field_144 = (((u16)ptr->field_168 + (Q12_TO_Q8(sharedData_800DFB7C_0_s00[idx].field_0.vx_0) - (u16)ptr->field_0.field_0.vx)) & 0xFFFF) +
                             ((Q12_TO_Q8(sharedData_800DFB7C_0_s00[idx].vy_8) - ptr->field_0.field_0.vy) << 16);
    temp3             = (u16)ptr->field_168;
    ptr->field_144.vz = (Q12_TO_Q8(sharedData_800DFB7C_0_s00[idx].field_4.vz_4) - ptr->field_0.field_0.vz) - temp3;

    gte_ldv3c(&ptr->collision);
    gte_rtpt();
    gte_stsxy3_g3(*poly);
    gte_stsz3c(&ptr->field_14C);
    gte_ldv0(&ptr->field_144);
    gte_rtps();
    gte_stsxy(&ptr->field_15C);
    gte_stsz(&ptr->field_158);

    ptr->field_14C.vx = (ptr->field_14C.vx + ptr->field_14C.vy + ptr->field_14C.vz + ptr->field_158) >> 2;

    if ((ptr->field_14C.vx - 16) <= 0 || ((ptr->field_14C.vx - 16) >> 3) >= ORDERING_TABLE_SIZE ||
        ABS(ptr->field_15C.vx) > 200 || ABS(ptr->field_15C.vy) > 160)
    {
        return false;
    }

    *(s32*)&(*poly)->x3 = *(s32*)&ptr->field_15C;

    ptr->field_164 = (ptr->field_160 * (func_80055D78(sharedData_800DFB7C_0_s00[idx].field_0.vx_0, sharedData_800DFB7C_0_s00[idx].vy_8, sharedData_800DFB7C_0_s00[idx].field_4.vz_4))) >> 7;
    ptr->field_160 = CLAMP_HIGH(ptr->field_160, ptr->field_164);

    temp_a0_3      = sharedData_800DFB7C_0_s00[idx].field_10.s_1.field_3 & 0x7;
    ptr->field_16C = (!(sharedData_800DFB7C_0_s00[idx].field_10.s_1.field_3 & 0x8)) << 5;
    ptr->field_180 = MIN(sharedData_800DFB7C_0_s00[idx].field_C.s_2.field_0 / 2456, 7) << 5;

    temp_v1_7 = temp_a0_3 ^ 0x1;
    temp_v1_7 = temp_v1_7 & 0x1;

    temp_v1_8 = (u32)temp_a0_3 >> 1;
    temp_v1_8 = temp_v1_8 ^ 0x1;
    temp_v1_8 = temp_v1_8 & 0x1;

    temp_v1_8 = temp_v1_7 ^ temp_v1_8;
    temp_v1_7 = (((temp_a0_3 >> 1) ^ 1) & 1) ^ ((temp_a0_3 ^ 1) & 1);

    if ((!(temp_a0_3 & 4) && (temp_v1_8 ^ 1) != 0) || ((temp_a0_3 & 4) && (temp_v1_8) != 0))
    {
        var_v0_4 = 0;
    }
    else
    {
        var_v0_4 = 0x1F;
    }
    ptr->field_170 = var_v0_4;

    if (temp_a0_3 >= 4)
    {
        ptr->field_184 = 0x1F;
    }
    else
    {
        ptr->field_184 = 0;
    }

    temp_v1_8 = temp_a0_3 ^ 0x1;
    temp_v1_8 = temp_v1_8 & 0x1;

    if ((!(temp_a0_3 & 4) && (temp_v1_8 ^ 1) != 0) || ((temp_a0_3 & 4) && (temp_v1_8) != 0))
    {
        var_v0_4 = 0;
    }
    else
    {
        var_v0_4 = 0x1F;
    }
    ptr->field_174 = var_v0_4;

    temp_v1_8 = (u32)temp_a0_3 >> 1;
    temp_v1_8 = temp_v1_8 ^ 1;
    temp_v1_8 = temp_v1_8 & 1;

    if ((!(temp_a0_3 & 0x4) && (temp_v1_8 ^ 0x1) != 0) || ((temp_a0_3 & 0x4) && temp_v1_8 != 0))
    {
        var_v0_4 = 0x1F;
    }
    else
    {
        var_v0_4 = 0;
    }
    ptr->field_188 = var_v0_4;

    temp_v1_8 = temp_a0_3 ^ 0x1;
    temp_v1_8 = temp_v1_8 & 0x1;

    if ((!(temp_a0_3 & 0x4) && (temp_v1_8 ^ 0x1) != 0) || ((temp_a0_3 & 0x4) && temp_v1_8 != 0))
    {
        var_v0_4 = 0x1F;
    }
    else
    {
        var_v0_4 = 0;
    }
    ptr->field_178 = var_v0_4;

    temp_v1_8 = (u32)temp_a0_3 >> 1;
    temp_v1_8 = temp_v1_8 ^ 1;
    temp_v1_8 = temp_v1_8 & 1;

    if ((!(temp_a0_3 & 0x4) && (temp_v1_8 ^ 0x1) != 0) || ((temp_a0_3 & 0x4) && temp_v1_8 != 0))
    {
        var_v0_4 = 0;
    }
    else
    {
        var_v0_4 = 0x1F;
    }
    ptr->field_18C = var_v0_4;

    temp_v1_7 = temp_a0_3 ^ 0x1;
    temp_v1_7 = temp_v1_7 & 0x1;

    temp_v1_8 = (u32)temp_a0_3 >> 1;
    temp_v1_8 = temp_v1_8 ^ 0x1;
    temp_v1_8 = temp_v1_8 & 0x1;

    temp_v1_8 = temp_v1_7 ^ temp_v1_8;

    if ((!(temp_a0_3 & 0x4) && (temp_v1_8 ^ 0x1) != 0) || ((temp_a0_3 & 0x4) && temp_v1_8 != 0))
    {
        var_v0_4 = 0x1F;
    }
    else
    {
        var_v0_4 = 0;
    }
    ptr->field_17C = var_v0_4;

    if (temp_a0_3 < 4)
    {
        ptr->field_190 = 0x1F;
    }
    else
    {
        ptr->field_190 = 0;
    }

    *(u32*)&(*poly)->u0 = ptr->field_16C + ptr->field_170 + ((ptr->field_180 + ptr->field_184) << 8) + 0x530000;
    *(u32*)&(*poly)->u1 = ptr->field_16C + ptr->field_174 + ((ptr->field_180 + ptr->field_188) << 8) + 0x6B0000;
    *(u16*)&(*poly)->u2 = ptr->field_16C + ptr->field_178 + ((ptr->field_180 + ptr->field_18C) << 8);
    *(u16*)&(*poly)->u3 = ptr->field_16C + ptr->field_17C + ((ptr->field_180 + ptr->field_190) << 8);

    setRGBC0(*poly, ptr->field_160, ptr->field_160, ptr->field_160, PRIM_POLY | RECT_BLEND | RECT_TEXTURE | RECT_SIZE_1);
    addPrimFast(&g_OrderingTable0[g_ActiveBufferIdx].org[(ptr->field_14C.vx - 16) >> 3], *poly, 9);
    *poly += 1;

    return true;
}

void sharedFunc_800CCDD4_1_s05(POLY_FT4** poly, s32 idx)
{
    if (sharedData_800DFB7C_0_s00[idx].field_B == 0)
    {
        sharedFunc_800CBF74_1_s05(poly, idx);
    }
    else
    {
        sharedFunc_800CC618_1_s05(poly, idx);
    }
}

void sharedFunc_800CCE2C_1_s05(s32 arg0, s32 arg1,
                               q19_12 posX0, q19_12 posY0, q19_12 posZ0,
                               q19_12 posX1, q19_12 posY1, q19_12 posZ1)
{
    s32 idx0;
    s32 idx1;

    idx0 = func_8005E7E0(5);
    if (idx0 == NO_VALUE)
    {
        return;
    }

    idx1 = func_8005E7E0(6);
    if (idx1 == NO_VALUE)
    {
        sharedData_800DFB7C_0_s00[idx0].field_A = 0;
        return;
    }

    sharedData_800DFB7C_0_s00[idx0].field_B             = arg0;
    sharedData_800DFB7C_0_s00[idx0].field_C.s_0.field_0 = idx1;
    sharedData_800DFB7C_0_s00[idx0].field_C.s_1.field_2 = arg1;
    sharedData_800DFB7C_0_s00[idx0].field_0.vx_0        = posX0;
    sharedData_800DFB7C_0_s00[idx0].vy_8                = posY0;
    sharedData_800DFB7C_0_s00[idx0].field_4.vz_4        = posZ0;

    sharedData_800DFB7C_0_s00[idx1].field_0.vx_0 = posX1;
    sharedData_800DFB7C_0_s00[idx1].vy_8         = posY1;
    sharedData_800DFB7C_0_s00[idx1].field_4.vz_4 = posZ1;
}

bool sharedFunc_800CCF30_1_s05(POLY_FT4** poly, s32 idx)
{
    typedef struct
    {
        s_func_8005E89C field_0;
        SVECTOR         collision;
        s32             field_134;
        s32             cameraRotation;
        DVECTOR         field_13C;
        DVECTOR         field_140;
        DVECTOR         field_144;
        s16             field_148;
        s32             field_14C;
    } s_sharedFunc_800CCF30_1_s05;

    s32                          temp_v1_3;
    s32                          var_a1;
    s32                          var_v0_4;
    s32                          temp_s3;
    s32                          var_a0;
    s32                          temp;
    s_sharedFunc_800CCF30_1_s05* ptr;

    ptr = PSX_SCRATCH;

    temp_s3 = sharedData_800DFB7C_0_s00[idx].field_C.s_2.field_0;

    sharedData_800DFB7C_0_s00[idx].field_A     = 0;
    sharedData_800DFB7C_0_s00[temp_s3].field_A = 0;

    Math_SetSVectorFastSum(&ptr->collision,
                           sharedData_800DFB7C_0_s00[idx].field_0.s_1.field_0 - (u16)ptr->field_0.field_0.vx,
                           sharedData_800DFB7C_0_s00[idx].vy_8 - ptr->field_0.field_0.vy,
                           sharedData_800DFB7C_0_s00[idx].field_4.s_0.field_0 - ptr->field_0.field_0.vz);

    gte_ldv0(&ptr->collision);
    gte_rtps();
    gte_stsxy(&ptr->field_13C);
    gte_stsz(&ptr->field_134);

    if (ABS(ptr->field_13C.vx) > 320 || ABS(ptr->field_13C.vy) > 240)
    {
        return false;
    }

    Math_SetSVectorFastSum(&ptr->collision,
                           sharedData_800DFB7C_0_s00[temp_s3].field_0.s_1.field_0 - (u16)ptr->field_0.field_0.vx,
                           sharedData_800DFB7C_0_s00[temp_s3].vy_8 - ptr->field_0.field_0.vy,
                           sharedData_800DFB7C_0_s00[temp_s3].field_4.s_0.field_0 - ptr->field_0.field_0.vz);

    gte_ldv0(&ptr->collision);
    gte_rtps();
    gte_stsxy(&ptr->field_140);
    gte_stsz(&ptr->cameraRotation);

    var_a1 = MAX(ptr->field_134, ptr->cameraRotation);

    if (ptr->cameraRotation >= ptr->field_134)
    {
        var_v0_4 = ((ptr->field_134 * 3) + var_a1) >> 2;
    }
    else
    {
        var_v0_4 = ((ptr->cameraRotation * 3) + var_a1) >> 2;
    }

    ptr->field_134 = var_v0_4;

    if (ptr->field_134 <= 0 || (ptr->field_134 >> 3) >= ORDERING_TABLE_SIZE)
    {
        return false;
    }

    ptr->field_148 = ratan2(ptr->field_140.vy - ptr->field_13C.vy, ptr->field_140.vx - ptr->field_13C.vx);

    ptr->field_14C = CLAMP_LOW(
        Q12_MULT_PRECISE(Q12(1.0f) - Math_Vector2MagCalcSafeQ6(sharedData_800DFB7C_0_s00[idx].field_0.vx_0 - sharedData_800DFB7C_0_s00[temp_s3].field_0.vx_0,
                                                            sharedData_800DFB7C_0_s00[idx].field_4.vz_4 - sharedData_800DFB7C_0_s00[temp_s3].field_4.vz_4),
                            24) * ptr->field_0.field_2C / ptr->field_134,
        1);

    ptr->field_144.vx = Q12_MULT(ptr->field_14C, Math_Sin(ptr->field_148));
    ptr->field_144.vy = Q12_MULT(ptr->field_14C, Math_Cos(ptr->field_148));

    setXY0Fast(*poly, (u16)ptr->field_13C.vx - (u16)ptr->field_144.vx, ptr->field_13C.vy - ptr->field_144.vy);
    setXY1Fast(*poly, (u16)ptr->field_140.vx - (u16)ptr->field_144.vx, ptr->field_140.vy - ptr->field_144.vy);
    setXY2Fast(*poly, (u16)ptr->field_13C.vx + (u16)ptr->field_144.vx, ptr->field_13C.vy + ptr->field_144.vy);
    setXY3Fast(*poly, (u16)ptr->field_140.vx + (u16)ptr->field_144.vx, ptr->field_140.vy + ptr->field_144.vy);

    *(u32*)&(*poly)->u0 = (sharedData_800DFB7C_0_s00[idx].field_B * 0xC00) + 0x010E4000;
    *(u32*)&(*poly)->u1 = (sharedData_800DFB7C_0_s00[idx].field_B * 0xC00) + 0x6D407F;
    *(u16*)&(*poly)->u2 = (sharedData_800DFB7C_0_s00[idx].field_B * 0xC00) + 0x4B00;
    *(u16*)&(*poly)->u3 = (sharedData_800DFB7C_0_s00[idx].field_B * 0xC00) + 0x4B7F;

    temp = func_80055D78(((sharedData_800DFB7C_0_s00[idx].field_0.vx_0 + sharedData_800DFB7C_0_s00[temp_s3].field_0.vx_0) * 16) >> 1,
                         (sharedData_800DFB7C_0_s00[idx].vy_8 + sharedData_800DFB7C_0_s00[temp_s3].vy_8) * 8,
                         ((sharedData_800DFB7C_0_s00[idx].field_4.vz_4 + sharedData_800DFB7C_0_s00[temp_s3].field_4.vz_4) * 16) >> 1);

    temp_v1_3 = (sharedData_800DFB7C_0_s00[idx].field_C.s_1.field_2 * temp) >> 6;

    if (sharedData_800DFB7C_0_s00[idx].field_C.s_1.field_2 >= temp_v1_3)
    {
        var_a0 = temp_v1_3;
    }
    else
    {
        var_a0 = sharedData_800DFB7C_0_s00[idx].field_C.s_1.field_2;
    }
    temp_v1_3 = var_a0;

    setRGBC0(*poly, temp_v1_3, temp_v1_3, temp_v1_3, PRIM_POLY | RECT_BLEND | RECT_TEXTURE | RECT_SIZE_1);
    addPrimFast(&g_OrderingTable0[g_ActiveBufferIdx].org[ptr->field_134 >> 3], *poly, 9);
    *poly += 1;

    return true;
}
