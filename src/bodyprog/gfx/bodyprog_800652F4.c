#include "game.h"
#include "inline_no_dmpsx.h"

#include <psyq/gtemac.h>
#include <psyq/libapi.h>
#include <psyq/strings.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "bodyprog/screen/data.h"
#include "bodyprog/screen/draw.h"
#include "bodyprog/item_screens.h"
#include "bodyprog/player/player.h"
#include "bodyprog/sound/sound_system.h"
#include "main/rng.h"

s_800AE710 D_800AE710[4] = {
    {},
    { 0x4C, 0x27, 0x7F },
    {},
    {}
};

// Seems like a struct of 2 s32 based on `func_80066184`.
s32 D_800AE71C[][2] = {
    { 0x00015AE1, 0xFFFEAEB9 },
    { 0x00017B33, 0xFFFEAEB9 },
    { 0x00015AE1, 0xFFFE8E67 },
    { 0x00017B33, 0xFFFE8E67 }
};

// Some timer used in `func_80066184`.
q3_12 D_800AE73C = Q12(0.0f);

// TODO: Garbage data.
#if VERSION_IS(JAP0)
s16 D_800AE73E = 0x342C;
#elif VERSION_IS(JAP1)
s16 D_800AE73E = 0x8002;
#elif VERSION_IS(JAP2)
s16 D_800AE73E = 0x0000;
#else
s16 D_800AE73E = 0x8002;
#endif

void func_800652F4(VECTOR3* arg0, s16 arg1, s16 arg2, s16 arg3) // 0x800652F4
{
    DVECTOR          sp10[32][3];
    s32              sp190[32][3];
    s32              i;
    s32              temp_a1;
    s32              posY;
    s32              posX;
    s32              posZ;
    s32              temp_v1_3;
    s32              j;
    s32              var_s1;
    s32              var_v0_3;
    s32              temp;
    s_func_800652F4* ptr;

    ptr = PSX_SCRATCH;

    posX             = FP_FROM(g_SysWork.playerWork.player.position.vx, Q12_SHIFT);
    posY             = FP_FROM(g_SysWork.playerWork.player.position.vy, Q12_SHIFT);
    posZ             = FP_FROM(g_SysWork.playerWork.player.position.vz, Q12_SHIFT);
    ptr->field_40.vx = Q8(posX);
    ptr->field_40.vy = Q8(posY);
    ptr->field_40.vz = Q8(posZ);

    Vw_WorldScreenMatrixAtPositionGet(&ptr->field_20, Q12(posX), Q12(posY), Q12(posZ));

    gte_SetRotMatrix(&ptr->field_20);
    gte_SetTransMatrix(&ptr->field_20);
    gte_ReadGeomScreen(&ptr->field_4C);

    for (i = 0; i < 3; i++)
    {
        var_s1           = ((arg2 >> 4) + 0x80) * i;
        temp             = Q12(1.0f);
        ptr->field_54[i] = -Q12_MULT_PRECISE(CLAMP_LOW((temp - Q12_MULT(var_s1, Math_Sin(arg2 >> 2))) - Math_Cos(arg2 >> 2),
                                                          0),
                                                Q12(2.5f));
        temp             = 0x80;
        ptr->field_60[i] = Q12_MULT_PRECISE(CLAMP_LOW(arg2 - (Q12_MULT(var_s1, Math_Cos(arg2 >> 2)) - temp),
                                                         0),
                                               Q12(2.5f));
    }

    ptr->field_50 = -0x40;

    for (i = 0; i < 32; i++)
    {
        for (j = 0; j < 3; j++)
        {
            Math_SetSVectorFastSum(&ptr->field_8[j],
                                   Q12_TO_Q8((arg0->vx + FP_FROM(Q12_MULT(ptr->field_60[j], Math_Sin(ptr->field_50)) * Math_Cos(arg1) - ptr->field_54[j] * Math_Sin(arg1), Q12_SHIFT))) - (u16)ptr->field_40.vx,
                                   Q12_TO_Q8((arg0->vy + Q12_MULT(ptr->field_60[j], Math_Cos(ptr->field_50)))) - ptr->field_40.vy,
                                   Q12_TO_Q8((arg0->vz + (((Q12_MULT(ptr->field_60[j], Math_Sin(ptr->field_50)) * Math_Sin(arg1)) + (ptr->field_54[j] * Math_Cos(arg1))) >> Q12_SHIFT))) - ptr->field_40.vz);
        }

        gte_ldv3c(&ptr->field_8);
        gte_rtpt();
        gte_stsxy3c(&sp10[i]);
        gte_stsz3c(&sp190[i]);

        ptr->field_50 += 0x80;
    }

    ptr->field_0 = (POLY_G4*)GsOUT_PACKET_P;

    for (i = 0; i < 32; i++)
    {
        temp_v1_3 = i + 1;
        var_v0_3  = temp_v1_3;
        if (temp_v1_3 < 0)
        {
            var_v0_3 = i + 32;
        }

        temp_a1 = var_v0_3 >> 5;
        temp_a1 = temp_v1_3 - (temp_a1 << 5);

        for (j = 0; j < 2; j++)
        {
            setPolyG4(ptr->field_0);

            setXY0Fast(ptr->field_0, sp10[i][j].vx, sp10[i][j].vy);
            setXY1Fast(ptr->field_0, sp10[temp_a1][j].vx, sp10[temp_a1][j].vy);
            setXY2Fast(ptr->field_0, sp10[i][j + 1].vx, sp10[i][j + 1].vy);
            setXY3Fast(ptr->field_0, sp10[temp_a1][j + 1].vx, sp10[temp_a1][j + 1].vy);

            ptr->field_68 = (sp190[i][j] + sp190[temp_a1][j] + sp190[i][j + 1] + sp190[temp_a1][j + 1]) >> 2;
            setSemiTrans(ptr->field_0, true);

            ptr->field_0->r0 = ptr->field_0->r1 = CLAMP_LOW(D_800AE710[j].field_0 - (arg3 >> 4) - (arg2 >> 5), 0);
            ptr->field_0->g0 = ptr->field_0->g1 = CLAMP_LOW(D_800AE710[j].field_1 - (arg3 >> 4) - (arg2 >> 5), 0);
            ptr->field_0->b0 = ptr->field_0->b1 = CLAMP_LOW(D_800AE710[j].field_2 - (arg3 >> 4) - (arg2 >> 5), 0);

            ptr->field_0->r2 = ptr->field_0->r3 = CLAMP_LOW(D_800AE710[j + 1].field_0 - (arg3 >> 4) - (arg2 >> 5), 0);
            ptr->field_0->g2 = ptr->field_0->g3 = CLAMP_LOW(D_800AE710[j + 1].field_1 - (arg3 >> 4) - (arg2 >> 5), 0);
            ptr->field_0->b2 = ptr->field_0->b3 = CLAMP_LOW(D_800AE710[j + 1].field_2 - (arg3 >> 4) - (arg2 >> 5), 0);

            addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[ptr->field_68 >> 3], ptr->field_0);

            ptr->field_0++;
            ptr->field_4 = (DR_TPAGE*)ptr->field_0;
            setDrawTPage(ptr->field_4, 0, 1, 0x20);
            addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[ptr->field_68 >> 3], ptr->field_4);
            ptr->field_4++;
            ptr->field_0 = (POLY_G4*)ptr->field_4;
        }
    }

    GsOUT_PACKET_P = (PACKET*)ptr->field_0;
}

void func_80065B94(VECTOR3* arg0, s16 arg1) // 0x80065B94
{
    s16              temp_s0;
    s32              gridPosX;
    s32              gridPosY;
    s32              gridPosZ;
    s16              temp_fp;
    s32              i;
    s16              temp;
    s32              temp2;
    POLY_FT4*        next;
    s_func_80065B94* ptr;
    static q3_12     D_800C4428[8];
    static s16       D_800C4438[8];

    ptr = PSX_SCRATCH;

    gridPosX         = FP_FROM(g_SysWork.playerWork.player.position.vx, Q12_SHIFT);
    gridPosY         = FP_FROM(g_SysWork.playerWork.player.position.vy, Q12_SHIFT);
    gridPosZ         = FP_FROM(g_SysWork.playerWork.player.position.vz, Q12_SHIFT);
    ptr->field_2C.vx = Q8(gridPosX);
    ptr->field_2C.vy = Q8(gridPosY);
    ptr->field_2C.vz = Q8(gridPosZ);

    Vw_WorldScreenMatrixAtPositionGet(&ptr->field_C, Q12(gridPosX), Q12(gridPosY), Q12(gridPosZ));

    gte_SetRotMatrix(&ptr->field_C);
    gte_SetTransMatrix(&ptr->field_C);
    gte_ReadGeomScreen(&ptr->field_38);

    temp                 = Q12_TO_Q8(arg0->vx) - ptr->field_2C.vx;
    Math_SetSVectorFastSum(&ptr->field_4,
                           temp,
                           Q12_TO_Q8(arg0->vy) - ptr->field_2C.vy,
                           Q12_TO_Q8(arg0->vz) - ptr->field_2C.vz);

    gte_ldv0(&ptr->field_4);
    gte_rtps();
    gte_stsxy(&ptr->field_3C);
    gte_stsz(&ptr->field_40);

    if (ptr->field_40 < ptr->field_38 && (ptr->field_40 >> 3) >= ORDERING_TABLE_SIZE)
    {
        return;
    }

    ptr->field_0 = (POLY_FT4*)GsOUT_PACKET_P;
    setPolyFT4(ptr->field_0);
    *(s32*)&ptr->field_0->u0 = 0x014C4020;
    *(s32*)&ptr->field_0->u1 = 0x2C403F;
    *(u16*)&ptr->field_0->u2 = 0x5F20;
    *(u16*)&ptr->field_0->u3 = 0x5F3F;
    setSemiTrans(ptr->field_0, true);

    temp_fp = ptr->field_38 * (s32)Q12_MULT_PRECISE((arg1 >> 1) + 0x800, 0x100) / ptr->field_40;

    for (i = 0; i < 8; i++)
    {
        temp2 = arg1;
        if (arg1 == 0)
        {
            D_800C4428[i] = (i << 9) + Rng_GenerateInt(0, 255) - 128;
            D_800C4438[i] = (Math_Cos(Rng_GenerateUInt(Q12_ANGLE(0.0f), Q12_ANGLE(180.0f) - 1)) >> 1) + Q12(1.0f);
        }

        temp_s0 = Q12_MULT_PRECISE(temp_fp, D_800C4438[i]);

        ptr->field_44.vx = Q12_MULT(temp_s0, Math_Sin(D_800C4428[i]));
        ptr->field_44.vy = Q12_MULT(temp_s0, Math_Sin(D_800C4428[i] + Q12_ANGLE(90.0f)));

        ptr->field_48.vx = Q12_MULT(-temp_s0, Math_Cos(D_800C4428[i]));
        ptr->field_48.vy = Q12_MULT(-temp_s0, Math_Cos(D_800C4428[i] + Q12_ANGLE(90.0f)));

        setXY0Fast(ptr->field_0, (u16)ptr->field_3C.vx, ptr->field_3C.vy);
        setXY1Fast(ptr->field_0, (u16)ptr->field_3C.vx + (u16)ptr->field_44.vx, ptr->field_3C.vy + ptr->field_48.vx);
        setXY2Fast(ptr->field_0, (u16)ptr->field_3C.vx + (u16)ptr->field_44.vy, ptr->field_3C.vy + ptr->field_48.vy);
        setXY3Fast(ptr->field_0, (u16)ptr->field_44.vy + ((u16)ptr->field_3C.vx + (u16)ptr->field_44.vx), ptr->field_3C.vy + ptr->field_48.vx + ptr->field_48.vy);

        setRGB0Fast(ptr->field_0, 160 - ((temp2 * 5) >> 7), 128 - (temp2 >> 5), 96 - ((temp2 * 3) >> 7));

        addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[ptr->field_40 >> 3], ptr->field_0);

        next  = ptr->field_0 + 1;
        *next = *ptr->field_0;

        ptr->field_0++;
    }

    GsOUT_PACKET_P = (PACKET*)ptr->field_0;
}

void func_80066184(void) // 0x80066184
{
    s32              i;
    s_func_80066184* ptr;
    POLY_GT4*        poly;

    if (g_Controller0->buttonFlags.clicked & ControllerFlag_R3)
    {
        D_800AE73C = Q12(2.0f);
    }

    if (g_Controller0->buttonFlags.held & ControllerFlag_Cross)
    {
        D_800AE73C = ((D_800AE73C - g_DeltaTime) < Q12(0.0f)) ? Q12(0.0f) : (D_800AE73C - g_DeltaTime);
    }

    ptr = PSX_SCRATCH;

    ptr->field_3C.vx = Q12_FLOOR(g_SysWork.playerWork.player.position.vx);
    ptr->field_3C.vy = Q12_FLOOR(g_SysWork.playerWork.player.position.vy);
    ptr->field_3C.vz = Q12_FLOOR(g_SysWork.playerWork.player.position.vz);

    Vw_WorldScreenMatrixAtPositionGet(&ptr->field_4, ptr->field_3C.vx, ptr->field_3C.vy, ptr->field_3C.vz);

    gte_SetRotMatrix(&ptr->field_4);
    gte_SetTransMatrix(&ptr->field_4);
    gte_ReadGeomScreen(&ptr->field_48);

    for (i = 0; i < 4; i++)
    {
        Math_SetSVectorFastSum(&ptr->field_24[i],
                               Q12_TO_Q8(D_800AE71C[i][0] - ptr->field_3C.vx),
                               Q12_TO_Q8(-81 - ptr->field_3C.vy),
                               Q12_TO_Q8(D_800AE71C[i][1] - ptr->field_3C.vz));
    }

    gte_ldv3c(&ptr->field_24);
    gte_rtpt();
    gte_stsxy3c(&ptr->field_4C);
    gte_stsz3c(&ptr->field_5C);
    gte_ldv0(&ptr->field_3C);
    gte_rtps();
    gte_stsxy(&ptr->field_58);
    gte_stsz(&ptr->field_68);

    ptr->field_0 = (POLY_GT4*)GsOUT_PACKET_P;

    setPolyGT4(ptr->field_0);

    setXY0Fast(ptr->field_0, (u16)ptr->field_4C.vx, ptr->field_4C.vy);
    setXY1Fast(ptr->field_0, (u16)ptr->field_50.vx, ptr->field_50.vy);
    setXY2Fast(ptr->field_0, (u16)ptr->field_54.vx, ptr->field_54.vy);
    setXY3Fast(ptr->field_0, (u16)ptr->field_58.vx, ptr->field_58.vy);

    ptr->field_6C = MIN(Q12_MULT(CLAMP_MIN_THEN_LOW(D_800AE73C, Q12(0.0f), Q12(1.0f)),
                                    func_80055D78(Q12(22.7f), Q12(0.0f), Q12(-22.1f))),
                        0xFF);

    ptr->field_70 = MIN(Q12_MULT(CLAMP_MIN_THEN_LOW(D_800AE73C - Q12(0.75f), Q12(0.0f), Q12(1.0f)),
                                    func_80055D78(Q12(22.7f), Q12(0.0f), Q12(-22.1f))),
                        0xFF);

    ptr->field_74 = MIN(Q12_MULT(CLAMP_MIN_THEN_LOW(D_800AE73C, Q12(0.0f), Q12(1.0f)),
                                    func_80055D78(Q12(22.7f), Q12(0.0f), Q12(-22.1f))),
                        0xFF);

    ptr->field_78 = MIN(Q12_MULT(CLAMP_MIN_THEN_LOW(D_800AE73C - Q12(0.75f), Q12(0.0f), Q12(1.0f)),
                                    func_80055D78(Q12(22.7f), Q12(0.0f), Q12(-22.1f))),
                        0xFF);

    setRGB0Fast(ptr->field_0, ptr->field_6C, ptr->field_6C, ptr->field_6C);
    setRGB1Fast(ptr->field_0, ptr->field_70, ptr->field_70, ptr->field_70);
    setRGB2Fast(ptr->field_0, ptr->field_74, ptr->field_74, ptr->field_74);
    setRGB3Fast(ptr->field_0, ptr->field_78, ptr->field_78, ptr->field_78);

    setSemiTrans(ptr->field_0, true);

    poly  = ptr->field_0 + 1;
    *poly = *ptr->field_0;

#if VERSION_REGION_IS(PAL)
    *(u32*)&ptr->field_0->u0 = 0x6C3A0000;
#else
    *(u32*)&ptr->field_0->u0 = 0xE0000;
#endif
    
    *(u32*)&ptr->field_0->u1 = 0x2D003F;
    *(u16*)&ptr->field_0->u2 = 0x3F00;
    *(u16*)&ptr->field_0->u3 = 0x3F3F;

    addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[(ptr->field_5C + ptr->field_60 + ptr->field_64 + ptr->field_68) >> 5], ptr->field_0);

    ptr->field_0++;

#if VERSION_REGION_IS(PAL)
    *(u32*)&ptr->field_0->u0 = 0x6C3A0000;
#else
    *(u32*)&ptr->field_0->u0 = 0xE0000;
#endif
    *(u32*)&ptr->field_0->u1 = 0x4D003F;
    *(u16*)&ptr->field_0->u2 = 0x3F00;
    *(u16*)&ptr->field_0->u3 = 0x3F3F;

    ptr->field_6C = MIN(FP_MULTIPLY(CLAMP_LOW_THEN_MIN(D_800AE73C - Q12(1.0f), Q12(0.0f), Q12(1.0f)),
                                    func_80055D78(Q12(22.7f), Q12(0.0), Q12(-22.1f)), Q12_SHIFT - 1),
                        0xFF);

    ptr->field_70 = MIN(FP_MULTIPLY(CLAMP_LOW_THEN_MIN(D_800AE73C - Q12(1.0f), Q12(0.0f), Q12(1.0f)),
                                    func_80055D78(Q12(22.7f), Q12(0.0), Q12(-22.1f)), Q12_SHIFT - 1),
                        0xFF);

    ptr->field_74 = MIN(FP_MULTIPLY(CLAMP_LOW_THEN_MIN(D_800AE73C - Q12(1.0f), Q12(0.0f), Q12(1.0f)),
                                    func_80055D78(Q12(22.7f), Q12(0.0), Q12(-22.1f)), Q12_SHIFT - 1),
                        0xFF);

    ptr->field_78 = MIN(FP_MULTIPLY(CLAMP_LOW_THEN_MIN(D_800AE73C - Q12(1.0f), Q12(0.0f), Q12(1.0f)),
                                    func_80055D78(Q12(22.7f), Q12(0.0), Q12(-22.1f)), Q12_SHIFT - 1),
                        0xFF);

    setRGB0Fast(ptr->field_0, ptr->field_6C, ptr->field_6C, ptr->field_6C);
    setRGB1Fast(ptr->field_0, ptr->field_70, ptr->field_70, ptr->field_70);
    setRGB2Fast(ptr->field_0, ptr->field_74, ptr->field_74, ptr->field_74);
    setRGB3Fast(ptr->field_0, ptr->field_78, ptr->field_78, ptr->field_78);

    addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[(ptr->field_5C + ptr->field_60 + ptr->field_64 + ptr->field_68) >> 5], ptr->field_0);

    GsOUT_PACKET_P = (PACKET*)++ptr->field_0;
}
