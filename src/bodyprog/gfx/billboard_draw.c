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

extern s_WorldEnvWork g_WorldEnvWork;

s_800AE204 D_800AE204[26] = {
    { 89,   102,  Q12_ANGLE(160.0f), 542,  387, 0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
    { 21,   306,  Q12_ANGLE(300.0f), 573,  356, 0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
    { 4,    306,  Q12_ANGLE(60.0f),  558,  356, 0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
    { -30,  289,  Q12_ANGLE(200.0f), 527,  418, 0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
    { -64,  255,  Q12_ANGLE(270.0f), 527,  465, 0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
    { -115, 306,  Q12_ANGLE(30.0f),  449,  387, 0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
    { -200, 289,  Q12_ANGLE(100.0f), 589,  465, 0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
    { -285, 221,  Q12_ANGLE(180.0f), 496,  387, 0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
    { -370, 272,  Q12_ANGLE(250.0f), 465,  387, 0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
    { -455, 255,  Q12_ANGLE(300.0f), 449,  387, 0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
    { -642, 170,  Q12_ANGLE(0.0f),   418,  310, 0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
    { -727, 136,  Q12_ANGLE(80.0f),  387,  310, 0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
    { 13,   30,   Q12_ANGLE(220.0f), 270,  270, 0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
    { -64,  60,   Q12_ANGLE(50.0f),  360,  240, 0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
    { -115, 100,  Q12_ANGLE(180.0f), 340,  190, 0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
    { -141, 30,   Q12_ANGLE(100.0f), 270,  240, 0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
    { -192, 70,   Q12_ANGLE(320.0f), 340,  180, 0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
    { 0,    0,    Q12_ANGLE(0.0f),   0,    0,   0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
    { 0,    0,    Q12_ANGLE(0.0f),   0,    0,   0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
    { 0,    0,    Q12_ANGLE(0.0f),   0,    0,   0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
    { 0,    0,    Q12_ANGLE(0.0f),   0,    0,   0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
    { 0,    0,    Q12_ANGLE(0.0f),   0,    0,   0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
    { 0,    0,    Q12_ANGLE(0.0f),   0,    0,   0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
    { 0,    0,    Q12_ANGLE(0.0f),   0,    0,   0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
    { 0,    0,    Q12_ANGLE(0.0f),   0,    0,   0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
    { 0,    0,    Q12_ANGLE(0.0f),   0,    0,   0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } }
};

// Used in `Gfx_BillboardDraw` (https://decomp.me/scratch/2BvV1)
s_800AE4DC D_800AE4DC[3] = {
#if VERSION_REGION_IS(PAL)
    { &D_800AE204[1],  3,  0, 0,      0x80, 0, 0xBF, 0x3F },
    { &D_800AE204[0],  12, 0, 0,      0x80, 0, 0xBF, 0x3F },
    { &D_800AE204[12], 5,  0, 0xFF67, 0x80, 0, 0xBF, 0x3F },
#else
    { &D_800AE204[1],  3,  0, 0,      0, 0x80, 0x3F, 0xBF },
    { &D_800AE204[0],  12, 0, 0,      0, 0x80, 0x3F, 0xBF },
    { &D_800AE204[12], 5,  0, 0xFF67, 0, 0x80, 0x3F, 0xBF },
#endif
};

SVECTOR D_800AE500[4] = {
    SVECTOR( 0.0f,    255.65f, 255.65f),
    SVECTOR(-255.65f, 0.0f,    255.65f),
    SVECTOR( 255.65f, 0.0f,    255.65f),
    SVECTOR( 0.0f,   -255.65f, 255.65f)
};

static GsCOORDINATE2* g_ViewCoord;

void func_8005B55C(GsCOORDINATE2* viewCoord) // 0x8005B55C
{
    s_800AE204* curPtr;

    g_ViewCoord = viewCoord;

    for (curPtr = &D_800AE204[0]; curPtr < &D_800AE204[26]; curPtr++)
    {
        curPtr->field_C.vz  = -Math_Cos(curPtr->field_4);
        curPtr->field_C.vx  = -Math_Sin(curPtr->field_4);
        curPtr->field_C.vy  = Q12(0.0f);
        curPtr->position.vz = Q12_MULT(curPtr->field_2, Math_Cos(curPtr->field_4));
        curPtr->position.vx = Q12_MULT(curPtr->field_2, Math_Sin(curPtr->field_4));
        curPtr->position.vy = curPtr->positionY;
    }
}

void Gfx_BillboardDraw(s32 idx, q19_12 posX, q19_12 posY, q19_12 posZ, GsOT* ot_arg4, s32 otShift) // 0x8005B62C
{
    MATRIX            worldToScreenMat;
    MATRIX            worldMat;
    CVECTOR           colors[5];
    MATRIX            lightMat;
    s_GteScratchData2 sp90;
    DVECTOR           sxy;
    s32               dummy;
    s32               sp494;
    s32               sp498;
    u16               sp4A0;
    u16               sp4A8;
    s_WorldEnvWork*   worldEnvWork;
    s_WorldEnvWork*   worldEngWork1;
    s32               temp_a0;
    s32               temp_a0_2;
    q19_12            temp_lo; // Light-related.
    s32               temp_lo_2;
    q19_12            lightMag;
    s32               temp_v0_2;
    s32               temp_v1;
    s32               temp_v1_2;
    s32               temp_v1_4;
    q19_12            lightPosX;
    q19_12            lightPosY;
    q19_12            lightPosZ;
    q19_12            fogAlpha;
    s32               var_v0;
    s_800AE204*       curPtr;
    s_800AE4DC*       temp_fp;
    s32               temp;
    s32               i;
    PACKET*           packet;
    PACKET*           packet2;
    POLY_GT4*         poly_gt4;
    POLY_G4*          poly_g4;

    temp_fp = &D_800AE4DC[idx];
    sp4A0   = temp_fp->field_A | (temp_fp->field_9 << 8);
    sp4A8   = temp_fp->field_8 | (temp_fp->field_B << 8);
    sp498   = ReadGeomScreen();

    temp_v1 = 0x79C << (otShift + 2);
    sp494   = g_WorldEnvWork.isFogEnabled ? MIN(temp_v1, g_WorldEnvWork.fog.farDistance) : temp_v1;
    Vw_WorldScreenMatrixAtPositionGet(&worldToScreenMat, posX, posY, posZ);

    // @hack Pointer needed for match, is there a way to remove this?
    // `Gfx_FogOverlayQuadDraw` `Gfx_MeshDraw` `func_8005AC50` all seem to do similar thing without needing pointer?
    worldEnvWork = &g_WorldEnvWork;
    if (!worldEnvWork->isFogEnabled)
    {
        fogAlpha = Q12(1.0f);
    }
    else
    {
        fogAlpha = Q12(1.0f) - Q8_TO_Q12(MIN(func_80055A50(Q8_TO_Q12(worldToScreenMat.t[2])), Q12(1.0f)));
    }

    // @hack Make sure compiler doesn't optimize out `new_var` pointer.
    if (worldEnvWork != NULL)
    {
        u8 x = -x;
    }

    if (g_WorldEnvWork.field_0 == UnkGfxEnum_0)
    {
        colors[4].r = Q12_MULT(g_WorldEnvWork.worldTintColor.r, g_WorldEnvWork.field_20);
        colors[4].g = Q12_MULT(g_WorldEnvWork.worldTintColor.g, g_WorldEnvWork.field_20);
        colors[4].b = Q12_MULT(g_WorldEnvWork.worldTintColor.b, g_WorldEnvWork.field_20);

        if (g_WorldEnvWork.isFogEnabled)
        {
            colors[4].r = Q12_MULT(colors[4].r, fogAlpha);
            colors[4].g = Q12_MULT(colors[4].g, fogAlpha);
            colors[4].b = Q12_MULT(colors[4].b, fogAlpha);
        }

        *(s32*)&colors[3] = *(s32*)&colors[4];
        *(s32*)&colors[2] = *(s32*)&colors[4];
        *(s32*)&colors[1] = *(s32*)&colors[4];
        *(s32*)&colors[0] = *(s32*)&colors[4];
    }
    else
    {
        Vw_CoordHierarchyMatrixCompute(g_ViewCoord, &worldMat);

        worldMat.t[0] = Q12_TO_Q8(posX);
        worldMat.t[1] = Q12_TO_Q8(posY) + temp_fp->field_6;
        worldMat.t[2] = Q12_TO_Q8(posZ);

        WorldEnv_LightTransform(&worldMat, g_WorldEnvWork.light.intensity,
                                &g_WorldEnvWork.light.direction, &g_WorldEnvWork.light.position);

        switch (g_WorldEnvWork.field_0)
        {
            case UnkGfxEnum_0:
            case UnkGfxEnum_1:
                func_8005A478(&sp90, fogAlpha);
                SetColorMatrix(&g_WorldEnvWork.colorMat);
                gte_lddqa(g_WorldEnvWork.light.field_0);
                gte_lddqb_0();
                break;

            case UnkGfxEnum_2:
                func_8005A838(&sp90, fogAlpha);
                SetColorMatrix(&g_WorldEnvWork.colorMat);
                break;
        }

        ReadLightMatrix(&lightMat);

        // Get light position.
        if (g_WorldEnvWork.isFogEnabled)
        {
            lightPosX = Q12_MULT(lightMat.m[0][0], fogAlpha);
            lightPosY = Q12_MULT(lightMat.m[0][1], fogAlpha);
            lightPosZ = Q12_MULT(lightMat.m[0][2], fogAlpha);
        }
        else
        {
            lightPosX = lightMat.m[0][0];
            lightPosY = lightMat.m[0][1];
            lightPosZ = lightMat.m[0][2];
        }

        lightMag = Math_Vector3MagCalc(lightPosX, lightPosY, lightPosZ);
        if (lightMag > Q12(0.25f))
        {
            temp_v1_2 = Q12_MULT(lightMag - Q12(0.25f), Q12(0.3f)) + Q12(0.25f);
            var_v0 = Q12(MIN(temp_v1_2, Q12(1.0f)));
            temp_lo = var_v0 / lightMag;

            lightPosX = Q12_MULT(lightPosX, temp_lo);
            lightPosY = Q12_MULT(lightPosY, temp_lo);
            lightPosZ = Q12_MULT(lightPosZ, temp_lo);

            if (g_WorldEnvWork.field_0 == UnkGfxEnum_1)
            {
                gte_lddp(temp_lo);
                gte_ldir_stbk();
                gte_gpf12();
                gte_ldmac_stir();
            }
        }

        // Set light position.
        lightMat.m[0][0] = lightPosX;
        lightMat.m[0][1] = lightPosY;
        lightMat.m[0][2] = lightPosZ;

        lightMat.m[1][0] = Q12_TO_Q8(-lightPosZ);
        lightMat.m[1][1] = Q12_TO_Q8(lightPosY);
        lightMat.m[1][2] = Q12_TO_Q8(lightPosX);

        lightMat.m[2][0] = Q12_TO_Q8(lightPosZ);
        lightMat.m[2][1] = Q12_TO_Q8(lightPosY);
        lightMat.m[2][2] = Q12_TO_Q8(-lightPosX);

        SetLightMatrix(&lightMat);
        NormalColor3(&D_800AE500[0], &D_800AE500[1], &D_800AE500[2], &colors[0], &colors[1], &colors[2]);
        NormalColor(&D_800AE500[3], &colors[3]);
    }

    // @hack Needed to get right reg order for `poly_gt4`.
    worldEngWork1 = &g_WorldEnvWork;

    SetRotMatrix(&worldToScreenMat);
    SetTransMatrix(&worldToScreenMat);

    for (curPtr = temp_fp->ptr_0, poly_gt4 = GsOUT_PACKET_P;
         curPtr < &temp_fp->ptr_0[temp_fp->count_4];
         curPtr++)
    {
        temp_v0_2 = RotTransPers(&curPtr->position, &sxy, &dummy, &dummy);
        temp_a0   = temp_v0_2 << 2;

        if (temp_v0_2 > 32 && temp_a0 < sp494)
        {
            s32 tPage = 44;
#if VERSION_REGION_IS(PAL)
            s32 clut  = 0x7CBB;
#else
            s32 clut  = 140;
#endif
            temp_lo_2 = Q12(sp498) / temp_a0;

            *(s32*)&poly_gt4->r0 = *(s32*)&colors[0];
            *(s32*)&poly_gt4->r1 = *(s32*)&colors[1];
            *(s32*)&poly_gt4->r2 = *(s32*)&colors[2];
            temp_a0              = *(s32*)&colors[3];

            do {} while (false); // @hack

            setPolyGT4(poly_gt4);

            poly_gt4->tpage = tPage;

            poly_gt4->clut       = clut;
            *(s32*)&poly_gt4->r3 = temp_a0;
            *(s16*)&poly_gt4->u0 = *(s16*)&temp_fp->field_8;
            *(s16*)&poly_gt4->u1 = sp4A0;
            *(s16*)&poly_gt4->u2 = sp4A8;
            *(s16*)&poly_gt4->u3 = *(s16*)&temp_fp->field_A;

            temp_a0_2 = Q12_MULT(curPtr->field_8, temp_lo_2);
            temp_v1_4 = Q12_MULT(curPtr->field_6, temp_lo_2);

            setXY4(poly_gt4,
                   sxy.vx, sxy.vy - (temp_a0_2 * 2),
                   sxy.vx + temp_v1_4, sxy.vy - temp_a0_2,
                   sxy.vx - temp_v1_4, sxy.vy - temp_a0_2,
                   sxy.vx, sxy.vy);

            if (worldEngWork1->isFogEnabled)
            {
                packet = poly_gt4 + 1;
                SetPriority(packet, 0, 0);
                packet2 = packet + 12; // TODO: `sizeof`?
                SetPriority(packet2, 1, 1);

                poly_g4 = packet2 + 12;

                temp = (func_80055A50(temp_v0_2 << 6) * 16) + worldEngWork1->fog.intensity;

                gte_lddp(Q12(1.0f) - MIN(temp, Q12(1.0f)));
                gte_ldrgb(&g_WorldEnvWork.fog.color);
                gte_dpcs();
                gte_strgb((CVECTOR*)&poly_g4->r0);

                *(s32*)&poly_g4->r1 = *(s32*)&poly_g4->r2 = *(s32*)&poly_g4->r3 = *(s32*)&poly_g4->r0;

                *(s32*)&poly_g4->x0 = *(s32*)&poly_gt4->x0;
                *(s32*)&poly_g4->x1 = *(s32*)&poly_gt4->x1;
                *(s32*)&poly_g4->x2 = *(s32*)&poly_gt4->x2;
                *(s32*)&poly_g4->x3 = *(s32*)&poly_gt4->x3;

                setPolyG4(poly_g4);
                setSemiTrans(poly_g4, 1);

                addPrim(&ot_arg4->org[temp_v0_2 >> otShift], packet);
                addPrim(&ot_arg4->org[temp_v0_2 >> otShift], poly_g4);
                addPrim(&ot_arg4->org[temp_v0_2 >> otShift], packet2);
                addPrim(&ot_arg4->org[temp_v0_2 >> otShift], poly_gt4);
                poly_g4++;
                poly_gt4 = poly_g4;
            }
            else
            {
                addPrim(&ot_arg4->org[temp_v0_2 >> otShift], poly_gt4);
                poly_gt4++;
            }
        }
    }

    GsOUT_PACKET_P = poly_gt4;
}

// ========================================
// UNUSED
// ========================================
// Split from something related to a cut debug feature?

void Gfx_DebugStringPositionSet(s16 unused, s16 posX, s16 posY) // 0x8005BF0C
{
    Text_Debug_PositionSet(posX, posY);
}
