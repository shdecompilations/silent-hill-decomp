#include "bodyprog/game.h"
#include "inline_no_dmpsx.h"

#include <psyq/strings.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/gfx/billboard.h"
#include "bodyprog/gfx/bodyprog_80056D8C.h"
#include "bodyprog/world/effects.h"
#include "bodyprog/math/math.h"
#include "main/rng.h"

extern s_WorldEnvRenderWork g_WorldEnvRenderWork;

static s_BillboardInfo g_BillboardInfos[26] = {
/* 0  */ { 89,   Q12(0.025f), Q12_ANGLE(160.0f), 542,  387, 0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
/* 1  */ { 21,   306,         Q12_ANGLE(300.0f), 573,  356, 0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
/* 2  */ { 4,    306,         Q12_ANGLE(60.0f),  558,  356, 0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
/* 3  */ { -30,  289,         Q12_ANGLE(200.0f), 527,  418, 0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
/* 4  */ { -64,  255,         Q12_ANGLE(270.0f), 527,  465, 0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
/* 5  */ { -115, 306,         Q12_ANGLE(30.0f),  449,  387, 0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
/* 6  */ { -200, 289,         Q12_ANGLE(100.0f), 589,  465, 0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
/* 7  */ { -285, 221,         Q12_ANGLE(180.0f), 496,  387, 0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
/* 8  */ { -370, 272,         Q12_ANGLE(250.0f), 465,  387, 0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
/* 9  */ { -455, 255,         Q12_ANGLE(300.0f), 449,  387, 0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
/* 10 */ { -642, 170,         Q12_ANGLE(0.0f),   418,  310, 0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
/* 11 */ { -727, 136,         Q12_ANGLE(80.0f),  387,  310, 0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
/* 12 */ { 13,   30,          Q12_ANGLE(220.0f), 270,  270, 0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
/* 13 */ { -64,  60,          Q12_ANGLE(50.0f),  360,  240, 0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
/* 14 */ { -115, 100,         Q12_ANGLE(180.0f), 340,  190, 0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
/* 15 */ { -141, 30,          Q12_ANGLE(100.0f), 270,  240, 0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
/* 16 */ { -192, 70,          Q12_ANGLE(320.0f), 340,  180, 0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
/* 17 */ { 0,    0,           Q12_ANGLE(0.0f),   0,    0,   0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
/* 18 */ { 0,    0,           Q12_ANGLE(0.0f),   0,    0,   0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
/* 19 */ { 0,    0,           Q12_ANGLE(0.0f),   0,    0,   0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
/* 20 */ { 0,    0,           Q12_ANGLE(0.0f),   0,    0,   0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
/* 21 */ { 0,    0,           Q12_ANGLE(0.0f),   0,    0,   0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
/* 22 */ { 0,    0,           Q12_ANGLE(0.0f),   0,    0,   0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
/* 23 */ { 0,    0,           Q12_ANGLE(0.0f),   0,    0,   0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
/* 24 */ { 0,    0,           Q12_ANGLE(0.0f),   0,    0,   0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } },
/* 25 */ { 0,    0,           Q12_ANGLE(0.0f),   0,    0,   0, { Q12(0.0f), Q12(0.0f), Q12(0.0f) }, { Q12(0.0f), Q12(0.0f), Q12(0.0f) } }
};

// Used in `Gfx_BillboardDraw` (https://decomp.me/scratch/20BvV1)
// References subsets of `g_BillboardInfos` to determine the relevant billboard pool for a given context?
static s_BillboardWork g_BillboardWorks[3] = {
#if VERSION_REGION_IS(PAL)
    { &g_BillboardInfos[1],  3,  0, 0,      0x80, 0, 0xBF, 0x3F },
    { &g_BillboardInfos[0],  12, 0, 0,      0x80, 0, 0xBF, 0x3F },
    { &g_BillboardInfos[12], 5,  0, 0xFF67, 0x80, 0, 0xBF, 0x3F },
#else
    { &g_BillboardInfos[1],  3,  0, 0,      0, 0x80, 0x3F, 0xBF },
    { &g_BillboardInfos[0],  12, 0, 0,      0, 0x80, 0x3F, 0xBF },
    { &g_BillboardInfos[12], 5,  0, 0xFF67, 0, 0x80, 0x3F, 0xBF },
#endif
};

static SVECTOR D_800AE500[4] = {
    SVECTOR( 0.0f,    255.65f, 255.65f),
    SVECTOR(-255.65f, 0.0f,    255.65f),
    SVECTOR( 255.65f, 0.0f,    255.65f),
    SVECTOR( 0.0f,   -255.65f, 255.65f)
};

static GsCOORDINATE2* g_BillboardViewCoord;

void Gfx_BillboardInfosInit(GsCOORDINATE2* viewCoord) // 0x8005B55C
{
    s_BillboardInfo* curBillboardInfo;

    g_BillboardViewCoord = viewCoord;

    // Run through billboard sprite infos.
    for (curBillboardInfo = &g_BillboardInfos[0]; curBillboardInfo < &g_BillboardInfos[26]; curBillboardInfo++)
    {
        // Set offset direction vector (unused).
        curBillboardInfo->offsetDirection.vz = -Math_Cos(curBillboardInfo->offsetAngle);
        curBillboardInfo->offsetDirection.vx = -Math_Sin(curBillboardInfo->offsetAngle);
        curBillboardInfo->offsetDirection.vy = Q12(0.0f);

        // Set position.
        curBillboardInfo->position.vz = Q12_MULT(curBillboardInfo->offsetDistance, Math_Cos(curBillboardInfo->offsetAngle));
        curBillboardInfo->position.vx = Q12_MULT(curBillboardInfo->offsetDistance, Math_Sin(curBillboardInfo->offsetAngle));
        curBillboardInfo->position.vy = curBillboardInfo->positionY;
    }
}

void Gfx_BillboardDraw(s32 idx, q19_12 posX, q19_12 posY, q19_12 posZ, GsOT* ot, s32 otShift) // 0x8005B62C
{
    MATRIX                worldToScreenMat;
    MATRIX                worldMat;
    CVECTOR               colors[5];
    MATRIX                lightMat;
    s_GteScratchData2     sp90;
    DVECTOR               sxy;
    s32                   dummy;
    s32                   sp494;
    s32                   sp498;
    u16                   sp4A0;
    u16                   sp4A8;
    s32                   temp_a0;
    s32                   temp_a0_2;
    q19_12                temp_lo; // Light-related.
    s32                   depthScaleFactor;
    q19_12                lightMag;
    s32                   temp_v0_2;
    s32                   temp_v1;
    s32                   temp_v1_2;
    s32                   temp_v1_4;
    q19_12                lightPosX;
    q19_12                lightPosY;
    q19_12                lightPosZ;
    q19_12                fogAlpha;
    s32                   var_v0;
    s_WorldEnvRenderWork* worldEnvWork;
    s_WorldEnvRenderWork* worldEngWork1;
    s_BillboardInfo*      curBillboardInfo;
    s_BillboardWork*      billboardWork;
    s32                   temp;
    DR_PRIO*              prio0;
    DR_PRIO*              prio1;
    POLY_GT4*             poly_gt4;
    POLY_G4*              poly_g4;

    billboardWork = &g_BillboardWorks[idx];
    sp4A0   = billboardWork->field_A | (billboardWork->field_9 << 8);
    sp4A8   = billboardWork->field_8 | (billboardWork->field_B << 8);
    sp498   = ReadGeomScreen();

    temp_v1 = 0x79C << (otShift + 2);
    sp494   = g_WorldEnvRenderWork.isFogEnabled ? MIN(temp_v1, g_WorldEnvRenderWork.fog.farDistance) : temp_v1;
    Vw_WorldScreenMatrixAtPositionGet(&worldToScreenMat, posX, posY, posZ);

    // @hack Pointer needed for match, is there a way to remove this?
    // `Gfx_FogOverlayQuadDraw` `Gfx_MeshDraw` `func_8005AC50` all seem to do similar thing without needing pointer?
    worldEnvWork = &g_WorldEnvRenderWork;
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

    if (g_WorldEnvRenderWork.lightMode == WorldEnvLightMode_None)
    {
        colors[4].r = Q12_MULT(g_WorldEnvRenderWork.worldTintColor.r, g_WorldEnvRenderWork.field_20);
        colors[4].g = Q12_MULT(g_WorldEnvRenderWork.worldTintColor.g, g_WorldEnvRenderWork.field_20);
        colors[4].b = Q12_MULT(g_WorldEnvRenderWork.worldTintColor.b, g_WorldEnvRenderWork.field_20);

        if (g_WorldEnvRenderWork.isFogEnabled)
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
        Vw_CoordHierarchyMatrixCompute(g_BillboardViewCoord, &worldMat);

        // Set matrix translation.
        worldMat.t[0] = Q12_TO_Q8(posX);
        worldMat.t[1] = Q12_TO_Q8(posY) + billboardWork->offsetY;
        worldMat.t[2] = Q12_TO_Q8(posZ);

        WorldEnvRender_LightTransform(&worldMat, g_WorldEnvRenderWork.light.intensity,
                                      &g_WorldEnvRenderWork.light.direction, &g_WorldEnvRenderWork.light.position);

        switch (g_WorldEnvRenderWork.lightMode)
        {
            case WorldEnvLightMode_None:
            case WorldEnvLightMode_Spot:
                func_8005A478(&sp90, fogAlpha);
                SetColorMatrix(&g_WorldEnvRenderWork.colorMat);
                gte_lddqa(g_WorldEnvRenderWork.light.field_0);
                gte_lddqb_0();
                break;

            case WorldEnvLightMode_Directional:
                func_8005A838(&sp90, fogAlpha);
                SetColorMatrix(&g_WorldEnvRenderWork.colorMat);
                break;
        }

        ReadLightMatrix(&lightMat);

        // Get light position.
        if (g_WorldEnvRenderWork.isFogEnabled)
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

            if (g_WorldEnvRenderWork.lightMode == WorldEnvLightMode_Spot)
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
    worldEngWork1 = &g_WorldEnvRenderWork;

    SetRotMatrix(&worldToScreenMat);
    SetTransMatrix(&worldToScreenMat);

    // Run through billboard sprites to draw.
    for (curBillboardInfo = billboardWork->billboardInfos, poly_gt4 = (POLY_GT4*)GsOUT_PACKET_P;
         curBillboardInfo < &billboardWork->billboardInfos[billboardWork->billboardInfoCount];
         curBillboardInfo++)
    {
        temp_v0_2 = RotTransPers(&curBillboardInfo->position, &sxy, &dummy, &dummy);
        temp_a0   = temp_v0_2 << 2;

        if (temp_v0_2 > 32 && temp_a0 < sp494)
        {
            s32 tPage = 44;
#if VERSION_REGION_IS(PAL)
            s32 clut  = 0x7CBB;
#else
            s32 clut  = 140;
#endif
            depthScaleFactor = Q12(sp498) / temp_a0;

            *(s32*)&poly_gt4->r0 = *(s32*)&colors[0];
            *(s32*)&poly_gt4->r1 = *(s32*)&colors[1];
            *(s32*)&poly_gt4->r2 = *(s32*)&colors[2];
            temp_a0              = *(s32*)&colors[3];

            do {} while (false); // @hack

            setPolyGT4(poly_gt4);

            poly_gt4->tpage = tPage;

            poly_gt4->clut       = clut;
            *(s32*)&poly_gt4->r3 = temp_a0;
            *(s16*)&poly_gt4->u0 = *(s16*)&billboardWork->field_8;
            *(s16*)&poly_gt4->u1 = sp4A0;
            *(s16*)&poly_gt4->u2 = sp4A8;
            *(s16*)&poly_gt4->u3 = *(s16*)&billboardWork->field_A;

            temp_a0_2 = Q12_MULT(curBillboardInfo->field_8, depthScaleFactor);
            temp_v1_4 = Q12_MULT(curBillboardInfo->field_6, depthScaleFactor);

            setXY4(poly_gt4,
                   sxy.vx, sxy.vy - (temp_a0_2 * 2),
                   sxy.vx + temp_v1_4, sxy.vy - temp_a0_2,
                   sxy.vx - temp_v1_4, sxy.vy - temp_a0_2,
                   sxy.vx, sxy.vy);

            if (worldEngWork1->isFogEnabled)
            {
                prio0 = (DR_PRIO*)(poly_gt4 + 1);
                SetPriority(prio0, 0, 0);
                prio1 = prio0 + 1;
                SetPriority(prio1, 1, 1);

                poly_g4 = (POLY_G4*)(prio1 + 1);

                temp = (func_80055A50(temp_v0_2 << 6) * 16) + worldEngWork1->fog.intensity;

                gte_lddp(Q12(1.0f) - MIN(temp, Q12(1.0f)));
                gte_ldrgb(&g_WorldEnvRenderWork.fog.color);
                gte_dpcs();
                gte_strgb((CVECTOR*)&poly_g4->r0);

                *(s32*)&poly_g4->r1 = *(s32*)&poly_g4->r2 = *(s32*)&poly_g4->r3 = *(s32*)&poly_g4->r0;

                *(s32*)&poly_g4->x0 = *(s32*)&poly_gt4->x0;
                *(s32*)&poly_g4->x1 = *(s32*)&poly_gt4->x1;
                *(s32*)&poly_g4->x2 = *(s32*)&poly_gt4->x2;
                *(s32*)&poly_g4->x3 = *(s32*)&poly_gt4->x3;

                setPolyG4(poly_g4);
                setSemiTrans(poly_g4, 1);

                addPrim(&ot->org[temp_v0_2 >> otShift], prio0);
                addPrim(&ot->org[temp_v0_2 >> otShift], poly_g4);
                addPrim(&ot->org[temp_v0_2 >> otShift], prio1);
                addPrim(&ot->org[temp_v0_2 >> otShift], poly_gt4);
                poly_g4++;
                poly_gt4 = (POLY_GT4*)poly_g4;
            }
            else
            {
                addPrim(&ot->org[temp_v0_2 >> otShift], poly_gt4);
                poly_gt4++;
            }
        }
    }

    GsOUT_PACKET_P = (PACKET*)poly_gt4;
}

void Gfx_DebugStringPositionSet(s16 unused, s16 posX, s16 posY) // 0x8005BF0C
{
    Text_Debug_PositionSet(posX, posY);
}
