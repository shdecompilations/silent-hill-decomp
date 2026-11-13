#include "bodyprog/game.h"
#include "inline_no_dmpsx.h"

#include <psyq/gtemac.h>
#include <psyq/libapi.h>
#include <psyq/strings.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/world/effects.h"
#include "bodyprog/items/item_screens.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player/player.h"
#include "bodyprog/screen/data.h"
#include "bodyprog/screen/draw.h"
#include "bodyprog/sound/sound_system.h"
#include "main/rng.h"

VECTOR3 D_800AE1B4[1] = { Q12(0.0f), Q12(0.05f), Q12(-0.076f) };

// Referenced only by `func_80055648` (https://decomp.me/scratch/joGmE)
// Doesn't look like `VECTOR3` tbh. Each word is larger than the previous.
s32 D_800AE1C0[] = {
    0,
    0,
    0x26,
    0x4C,
    0x5E,
    0x70,
    0x85,
    0x99,
    0xB0,
    0xBD,
    0xD1,
    0xDC,
    0xE6,
    0xF0,
    0xF5,
    0xFA,
    0xFF
};

// ========================================
// ENVIRONMENT AND SCREEN GFX 1
// ========================================

s_WorldEnvRenderWork g_WorldEnvRenderWork;

void WorldEnvRender_Init(void) // 0x80055028
{
    func_80040BAC();
    shLensFlareInit();

    g_WorldEnvRenderWork.lightMode = WorldEnvLightMode_None;
    g_WorldEnvRenderWork.field_20  = Q12(1.0f);

    g_WorldEnvRenderWork.worldTintColor.r = 128;
    g_WorldEnvRenderWork.worldTintColor.g = 128;
    g_WorldEnvRenderWork.worldTintColor.b = 128;

    g_WorldEnvRenderWork.isFogEnabled           = false;
    g_WorldEnvRenderWork.enableTintLightOverlap = false;

    g_WorldEnvRenderWork.fog.color.r = 255;
    g_WorldEnvRenderWork.fog.color.g = 255;
    g_WorldEnvRenderWork.fog.color.b = 255;

    g_WorldEnvRenderWork.light.field_0            = Q4(0.0f);
    g_WorldEnvRenderWork.light.lensFlareIntensity = Q12(0.0f);
    g_WorldEnvRenderWork.waterZones               = NULL;
    g_WorldEnvRenderWork.fog.intensity            = 0;

    gte_SetFarColor(0, 0, 0);

    WorldEnvRender_FogDistanceSet(Q12(32.0f), Q12(34.0f));
}

void WorldGfx_2dEffectsDraw(void) // 0x800550D0
{
    s32      color0;
    s32      color2;
    PACKET*  packet;
    PACKET*  packet2;
    DR_MODE* mode;
    POLY_G4* poly;
    GsOT*    ot;

    ot = &g_OrderingTable0[g_ActiveBufferIdx];

    if (g_WorldEnvRenderWork.enableTintLightOverlap != false)
    {
        func_80041074(ot, g_WorldEnvRenderWork.light.intensity, &g_WorldEnvRenderWork.light.direction, &g_WorldEnvRenderWork.light.position);
    }

    if (g_WorldEnvRenderWork.lightMode == WorldEnvLightMode_Spot && g_WorldEnvRenderWork.light.lensFlareIntensity != Q12(0.0f))
    {
        shLensFlareExec(g_WorldEnvRenderWork.light.lensFlareIntensity, &g_WorldEnvRenderWork.light.direction,
                        &g_WorldEnvRenderWork.light.position, g_WorldEnvRenderWork.waterZones);
    }

    if (g_WorldEnvRenderWork.screenBrightness > Q12(0.0f))
    {
        poly            = (POLY_G4*)GsOUT_PACKET_P;
        mode            = (DR_MODE*)(GsOUT_PACKET_P + sizeof(POLY_G4));
        GsOUT_PACKET_P += sizeof(POLY_G4) + sizeof(DR_MODE);

        color0           = (g_WorldEnvRenderWork.screenBrightness + (g_WorldEnvRenderWork.screenBrightness << 8)) + (g_WorldEnvRenderWork.screenBrightness << 16);
        *(s32*)&poly->r3 = color0;
        *(s32*)&poly->r2 = color0;
        *(s32*)&poly->r1 = color0;
        *(s32*)&poly->r0 = color0;

        setPolyG4(poly);
        setSemiTrans(poly, true);
        
#if VERSION_REGION_IS(PAL)
        setXY4(poly,
               -180, -g_GameWork.gsScreenHeight / 2,
                180, -g_GameWork.gsScreenHeight / 2,
               -180,  g_GameWork.gsScreenHeight / 2,
                180,  g_GameWork.gsScreenHeight / 2);
#else
        setXY4(poly,
               -180, -120,
                180, -120,
               -180,  120,
                180,  120);
#endif

        AddPrim(ot->org, poly);
        SetDrawMode(mode, 0, 1, 0x20, NULL);
        AddPrim(ot->org, mode);
    }

    packet2 = GsOUT_PACKET_P;
    packet  = packet2;

    SetPriority(packet2, 0, 0);
    AddPrim(&ot->org[ORDERING_TABLE_SIZE - 1], packet2);

    poly           = (POLY_G4*)(packet + 0xC);
    GsOUT_PACKET_P = packet + 0x30;

    color2           = *(s32*)&g_WorldEnvRenderWork.fog.color;
    *(s32*)&poly->r3 = color2;
    *(s32*)&poly->r2 = color2;
    *(s32*)&poly->r1 = color2;
    *(s32*)&poly->r0 = color2;

    SetPolyG4(poly);

#if VERSION_REGION_IS(PAL)
    setXY4(poly,
           -180, -g_GameWork.gsScreenHeight / 2,
            180, -g_GameWork.gsScreenHeight / 2,
           -180,  g_GameWork.gsScreenHeight / 2,
            180,  g_GameWork.gsScreenHeight / 2);
#else
    setXY4(poly,
           -180, -120,
            180, -120,
           -180,  120,
            180,  120);
#endif

    AddPrim(&ot->org[ORDERING_TABLE_SIZE - 1], poly);
    packet = GsOUT_PACKET_P;
    SetPriority(packet, 0, 1);
    AddPrim(&ot->org[ORDERING_TABLE_SIZE - 1], packet);

    GsOUT_PACKET_P = packet + sizeof(DR_MODE);
    mode           = (DR_MODE*)GsOUT_PACKET_P;

    SetDrawMode(mode, 0, 1, 32, NULL);
    AddPrim(&ot->org[ORDERING_TABLE_SIZE - 1], mode);

    GsOUT_PACKET_P = packet + 24;
}

void WorldEnvRender_WorldLightingParamSet(u8 arg0, q19_12 arg1, u8 arg2, s32 tintR, s32 tintG, s32 tintB, q23_8 brightness) // 0x80055330
{
    g_WorldEnvRenderWork.lightMode        = arg0;
    g_WorldEnvRenderWork.field_20         = arg1;
    g_WorldEnvRenderWork.lightLevelMax    = arg2;
    g_WorldEnvRenderWork.worldTintColor.r = tintR >> 5;
    g_WorldEnvRenderWork.colorMat.m[0][2] = tintR;
    g_WorldEnvRenderWork.colorMat.m[0][1] = tintR;
    g_WorldEnvRenderWork.colorMat.m[0][0] = tintR;
    g_WorldEnvRenderWork.worldTintColor.g = tintG >> 5;
    g_WorldEnvRenderWork.screenBrightness = brightness;
    g_WorldEnvRenderWork.worldTintColor.b = tintB >> 5;
    g_WorldEnvRenderWork.colorMat.m[1][2] = (s16)tintG;
    g_WorldEnvRenderWork.colorMat.m[1][1] = (s16)tintG;
    g_WorldEnvRenderWork.colorMat.m[1][0] = (s16)tintG;
    g_WorldEnvRenderWork.colorMat.m[2][2] = (s16)tintB;
    g_WorldEnvRenderWork.colorMat.m[2][1] = (s16)tintB;
    g_WorldEnvRenderWork.colorMat.m[2][0] = (s16)tintB;
    g_WorldEnvRenderWork.field_24.r       = (tintR * arg1) >> 17;
    g_WorldEnvRenderWork.field_24.g       = (tintG * arg1) >> 17;
    g_WorldEnvRenderWork.field_24.b       = (tintB * arg1) >> 17;
}

void WorldEnvRender_FogParamsSet(u8 isFogEnabled, u8 fogColorR, u8 fogColorG, u8 fogColorB) // 0x800553C4
{
    g_WorldEnvRenderWork.isFogEnabled = isFogEnabled;
    g_WorldEnvRenderWork.fog.color.r  = fogColorR;
    g_WorldEnvRenderWork.fog.color.g  = fogColorG;
    g_WorldEnvRenderWork.fog.color.b  = fogColorB;
}

void WorldEnvRender_WorldLightTintSet(u32 overlapLighting, u8 lightPosR, u8 lightPosG, u8 lightPosB, u8 worldTintR, u8 worldTintG, u8 worldTintB) // 0x800553E0
{
    g_WorldEnvRenderWork.enableTintLightOverlap = overlapLighting;

    if (overlapLighting != false)
    {
        WorldEnvRender_WorldLightTintParamSet(lightPosR, lightPosG, lightPosB, worldTintR, worldTintG, worldTintB);
    }
}

void WorldEnvRender_LightPositionGet(VECTOR3* pos) // 0x80055434
{
    *pos = g_WorldEnvRenderWork.light.position;
}

s32 WorldEnvRender_LightRotationAndIntensityGet(SVECTOR* rot) // 0x8005545C
{
    *rot = g_WorldEnvRenderWork.light.rotation;
    return g_WorldEnvRenderWork.light.intensity;
}

s32 WorldEnvRender_LightDirectionAndIntensityGet(SVECTOR* dir) // 0x80055490
{
    *dir = g_WorldEnvRenderWork.light.direction;
    return g_WorldEnvRenderWork.light.intensity;
}

void Gfx_FlashlightPositionUpdate(q19_12 lightIntensity, q3_12 lensFlareIntensity, GsCOORDINATE2* coord0, GsCOORDINATE2* coord1,
                                  SVECTOR* rot, q19_12 posX, q19_12 posY, q19_12 posZ, s_WaterZone* waterZones) // 0x800554C4
{
    MATRIX   mat;
    SVECTOR  tempSvec;
    VECTOR   vec;
    VECTOR3* lightPos;            // Q19.12
    VECTOR3* transformedLightPos; // Q19.12

    g_WorldEnvRenderWork.light.intensity          = lightIntensity;
    g_WorldEnvRenderWork.light.lensFlareIntensity = lensFlareIntensity;
    g_WorldEnvRenderWork.waterZones               = waterZones;

    if (coord0 == NULL)
    {
        g_WorldEnvRenderWork.light.direction = *rot;
    }
    else
    {
        Vw_CoordHierarchyMatrixCompute(coord0, &mat);
        ApplyMatrixSV(&mat, rot, &g_WorldEnvRenderWork.light.direction);
    }

    if (coord1 == NULL)
    {
        lightPos     = &g_WorldEnvRenderWork.light.position;
        lightPos->vx = posX;
        lightPos->vy = posY;
        lightPos->vz = posZ;
    }
    else
    {
        Vw_CoordHierarchyMatrixCompute(coord1, &mat);

        tempSvec.vx = Q12_TO_Q8(posX);
        tempSvec.vy = Q12_TO_Q8(posY);
        tempSvec.vz = Q12_TO_Q8(posZ);

        ApplyMatrix(&mat, &tempSvec, &vec);

        transformedLightPos     = &g_WorldEnvRenderWork.light.position;
        transformedLightPos->vx = Q8_TO_Q12(vec.vx + mat.t[0]);
        transformedLightPos->vy = Q8_TO_Q12(vec.vy + mat.t[1]);
        transformedLightPos->vz = Q8_TO_Q12(vec.vz + mat.t[2]);
    }

    vwVectorToAngle(&g_WorldEnvRenderWork.light.rotation, &g_WorldEnvRenderWork.light.direction);
    g_WorldEnvRenderWork.light.field_0 = Q12_TO_Q4(lightIntensity);
    func_80055648(lightIntensity, &g_WorldEnvRenderWork.light.direction);
}

void func_80055648(s32 intensity, const SVECTOR* dir) // 0x80055648
{
    s32                var_a2;
    s32                temp_lo;
    s32                temp_t1;
    s32                temp_v0;
    s32                temp_v1;
    s32                j;
    s32                i;
    s_WorldEnvRenderWork_84* curPtr;

    for (i = 0, curPtr = g_WorldEnvRenderWork.light.field_38;
         i < 3;
         i++, curPtr++)
    {
        switch (i)
        {
            case 0:
                var_a2 = dir->vx;
                break;

            case 1:
                var_a2 = dir->vy;
                break;

            case 2:
                var_a2 = dir->vz;
                break;

            default:
                var_a2 = Q12(0.0f);
                break;
        }

        temp_t1 = Q12(var_a2 + Q12(1.3f)) / Q12(2.3f);
        temp_v1 = Q12(Q12(1.3f) - var_a2) / Q12(2.3f);
        temp_v0 = Q12(1024.0f) / temp_t1;
        temp_lo = Q12(1024.0f) / temp_v1;

        for (j = 0; j < ARRAY_SIZE(D_800AE1B4); j++)
        {
            curPtr->field_0[0][j].vy =
            curPtr->field_0[1][j].vy = Q12_MULT(D_800AE1B4[j].vy, intensity);
            curPtr->field_0[0][j].vx = Q12_MULT(D_800AE1B4[j].vx, temp_t1);
            curPtr->field_0[1][j].vx = Q12_MULT(D_800AE1B4[j].vx, temp_v1);
            curPtr->field_0[0][j].vz = Q12_MULT(FP_MULTIPLY(D_800AE1B4[j].vz, temp_v0, Q12_SHIFT - 2), intensity);
            curPtr->field_0[1][j].vz = Q12_MULT(FP_MULTIPLY(D_800AE1B4[j].vz, temp_lo, Q12_SHIFT - 2), intensity);
        }
    }
}

s32 func_800557DC(void) // 0x800557DC
{
    MATRIX mat;

    Vw_WorldScreenMatrixAtPositionGet(&mat, g_WorldEnvRenderWork.light.position.vx, g_WorldEnvRenderWork.light.position.vy, g_WorldEnvRenderWork.light.position.vz);
    return Q8_TO_Q12(mat.t[2]);
}

void func_80055814(s32 arg0) // 0x80055814
{
    g_WorldEnvRenderWork.fog.intensity = Q12(1.0f) - func_800559A8(arg0);
}

void WorldEnvRender_FogDistanceSet(q19_12 nearDist, q19_12 farDist) // 0x80055840
{
    s32 temp_lo;
    s32 temp_v1_2;
    s32 var_a0;
    s32 var_a1;
    s32 var_a2;
    s32 var_a3;
    s32 var_t0;
    s32 var_v1;
    s32 temp;

    nearDist = Q12_TO_Q8(nearDist);

    g_WorldEnvRenderWork.fog.nearDistance = nearDist;
    g_WorldEnvRenderWork.fog.farDistance  = Q12_TO_Q8(farDist);
    g_WorldEnvRenderWork.fog.depthShift   = 0x20 - Lzc(nearDist - 1);

    temp = (0x10 << (g_WorldEnvRenderWork.fog.depthShift + 1)) / nearDist;
    for (var_t0 = 0, var_a3 = 0; var_t0 < Q12(1.0f) && var_a3 < 0x80; var_t0 += temp, var_a3++)
    {
        temp_v1_2 = var_t0 >> 8;

        if (temp_v1_2 == 0)
        {
            var_a1 = 0x80;
            var_a2 = var_t0 & 0x7F;
            if (var_t0 & 0x80)
            {
                var_v1 = D_800AE1C0[0];
                var_a0 = 0;
            }
            else
            {
                var_a0 = D_800AE1C0[0];
                var_v1 = D_800AE1C0[1];
            }
        }
        else
        {
            var_a1 = 0x100;
            var_a2 = var_t0 & 0xFF;
            var_a0 = D_800AE1C0[temp_v1_2];
            var_v1 = D_800AE1C0[temp_v1_2 + 1];
        }

        temp_lo = (var_a0 * (var_a1 - var_a2) + var_v1 * var_a2) / var_a1;

        limitRange(temp_lo, 0, 0xFF);

        g_WorldEnvRenderWork.fogRamp[var_a3] = (u8)temp_lo;
    }

    for (; var_a3 < 0x80; var_a3++)
    {
        g_WorldEnvRenderWork.fogRamp[var_a3] = 0xFF;
    }
}

s32 func_800559A8(s32 arg0) // 0x800559A8
{
    s32 temp_a2;
    s32 temp_lo;
    s32 temp_v0;
    s32 var_a1;
    s32 var_v0;

    temp_v0 = 1 << g_WorldEnvRenderWork.fog.depthShift;
    var_a1  = temp_v0 >> 7;

    if (temp_v0 < 0)
    {
        var_a1 = (temp_v0 + 0x7F) >> 7;
    }

    temp_v0 = arg0 >> 4;
    temp_lo = temp_v0 / var_a1;

    if (temp_lo < 0)
    {
        return 0;
    }

    if (temp_lo >= 0x80)
    {
        return Q12(1.0f);
    }

    temp_a2 = g_WorldEnvRenderWork.fogRamp[temp_lo] * 16;
    var_v0  = temp_v0 % var_a1;

    if (temp_lo == 0x7F)
    {
        temp_v0 = Q12(1.0f);
    }
    else
    {
        temp_v0 = g_WorldEnvRenderWork.fogRamp[temp_lo + 1] * 16;
    }

    return temp_a2 + ((temp_v0 - temp_a2) * var_v0) / var_a1;
}

u8 func_80055A50(s32 arg0) // 0x80055A50
{
    s32 temp;

    temp = arg0 >> 4;

    if (temp >= (1 << g_WorldEnvRenderWork.fog.depthShift))
    {
        return 255;
    }

    return g_WorldEnvRenderWork.fogRamp[(temp << 7) >> g_WorldEnvRenderWork.fog.depthShift];
}

void func_80055A90(CVECTOR* arg0, CVECTOR* arg1, u8 arg2, s32 arg3) // 0x80055A90
{
    s32 var_v1;

    // TODO: Maybe Q8 to Q12 and back again.
    arg3 = arg3 >> 4;
    if (arg3 < 0)
    {
        arg3 = 0;
    }

    if (arg3 < (1 << g_WorldEnvRenderWork.fog.depthShift))
    {
        arg3 = g_WorldEnvRenderWork.fogRamp[(arg3 << 7) >> g_WorldEnvRenderWork.fog.depthShift] << 4;
    }
    else
    {
        arg3 = 0xFF << 4;
    }

    var_v1 = Q12(1.0f) - (g_WorldEnvRenderWork.fog.intensity + arg3);
    if (var_v1 < 0)
    {
        var_v1 = 0;
    }

    gte_lddp(var_v1);
    gte_ldrgb(&g_WorldEnvRenderWork.fog.color);

    gte_SetFarColor(0, 0, 0);

    gte_dpcs();

    gte_strgb(arg0);

    gte_lddp(arg3);
    gte_ldrgb(&g_WorldEnvRenderWork.worldTintColor);

    gte_ldsv_(arg2 << 5);

    gte_dpcl();

    gte_strgb(arg1);
}

void func_80055B74(CVECTOR* result, CVECTOR* color, s32 arg2) // 0x80055B74
{
    s32 var_v0;
    s32 var_t0;

    arg2 >>= 4;

    var_t0 = g_WorldEnvRenderWork.field_20 >> 5;

    if (arg2 < (1 << g_WorldEnvRenderWork.fog.depthShift))
    {
        var_v0 = g_WorldEnvRenderWork.fogRamp[(arg2 << 7) >> g_WorldEnvRenderWork.fog.depthShift];
    }
    else
    {
        var_v0 = 255;
    }

    var_v0 <<= 4;

    gte_lddp(var_v0);
    gte_ldrgb(color);
    gte_SetFarColor(g_WorldEnvRenderWork.fog.color.r, g_WorldEnvRenderWork.fog.color.g, g_WorldEnvRenderWork.fog.color.b);
    gte_ldsv_(var_t0 << 5);
    gte_dpcl();
    gte_strgb(result);
    gte_SetFarColor(0, 0, 0);
}

void func_80055C3C(CVECTOR* result, CVECTOR* color, s32 arg2, s32 arg3, s32 arg4, s32 arg5) // 0x80055C3C
{
    s32 temp_a1;
    s32 var_v0;
    s32 var_s0;

    var_s0  = arg5 >> 4;
    temp_a1 = func_80055D78(arg2, arg3, arg4);

    if (g_WorldEnvRenderWork.isFogEnabled)
    {
        if (var_s0 < (1 << g_WorldEnvRenderWork.fog.depthShift))
        {
            var_v0 = g_WorldEnvRenderWork.fogRamp[(var_s0 << 7) >> g_WorldEnvRenderWork.fog.depthShift];
        }
        else
        {
            var_v0 = 255;
        }

        gte_lddp(var_v0 << 4);
        gte_ldrgb(color);
        gte_SetFarColor(g_WorldEnvRenderWork.fog.color.r, g_WorldEnvRenderWork.fog.color.g, g_WorldEnvRenderWork.fog.color.b);
        gte_ldsv_(temp_a1 << 5);

        gte_dpcl();
        gte_strgb(result);

        gte_SetFarColor(0, 0, 0);
    }
    else
    {
        gte_lddp(Q12(1.0f) - (temp_a1 << 5));
        gte_ldrgb(color);
        gte_dpcs();
        gte_strgb(result);
    }
}

u8 func_80055D78(q19_12 posX, q19_12 posY, q19_12 posZ) // 0x80055D78
{
    q23_8    pos[3]; // XYZ
    s32      temp_v1;
    q23_8    tempX;
    s32      var_a3;
    s32      i;
    VECTOR3* ptr0;
    VECTOR3* ptr1;

    pos[0] = Q12_TO_Q8(posX) - Q12_TO_Q8(g_WorldEnvRenderWork.light.position.vx);
    pos[1] = Q12_TO_Q8(posY) - Q12_TO_Q8(g_WorldEnvRenderWork.light.position.vy);
    pos[2] = Q12_TO_Q8(posZ) - Q12_TO_Q8(g_WorldEnvRenderWork.light.position.vz);

    if (g_WorldEnvRenderWork.lightMode != WorldEnvLightMode_None)
    {
        ptr1 = &g_WorldEnvRenderWork.light.field_38;
        for (i = 0, ptr0 = ptr1, var_a3 = 0xFF;
             i < ARRAY_SIZE(pos);
             i++, ptr0 += 2)
        {
            tempX = pos[i];
            ptr1  = ptr0;
            if (tempX < 0)
            {
                tempX = -tempX;
                ptr1++;
            }

            temp_v1 = ptr1->vy + Q12_MULT(ptr1->vz, tempX - ptr1->vx);
            if (temp_v1 < var_a3)
            {
                var_a3 = temp_v1;
            }
        }
    }
    else
    {
        var_a3 = 0;
    }

    var_a3 += g_WorldEnvRenderWork.field_20 >> 5;

    limitRange(var_a3, 0, g_WorldEnvRenderWork.lightLevelMax);
    return var_a3;
}

void func_80055E90(CVECTOR* color, u8 fadeAmount) // 0x80055E90
{
    q19_12 alpha;
    u8     prev_cd;

    alpha = Q12(1.0f) - (fadeAmount * 32);

    // Works similar to `gte_DpqColor` macro, but `gte_lddp`/`gte_ldrgb` are in wrong order?

    gte_lddp(alpha);
    gte_ldrgb(&g_WorldEnvRenderWork.worldTintColor);
    gte_dpcs();

    prev_cd = color->cd;
    gte_strgb(color);
    color->cd = prev_cd;
}

// Used for the transition world effect during Central Silent Hill.
void func_80055ECC(CVECTOR* color, SVECTOR3* arg1, SVECTOR3* arg2, MATRIX* worldMat) // 0x80055ECC
{
    func_80055E90(color, func_80055F08(arg1, arg2, worldMat));
}

u8 func_80055F08(SVECTOR3* arg0, SVECTOR3* arg1, MATRIX* worldMat) // 0x80055F08
{
    MATRIX  rotMat;
    DVECTOR screenPos;
    s32     geomOffsetX;
    s32     geomOffsetY;
    s32     depthP;
    s32     geomScreen;
    s32     ret;
    s32     var_v1;
    u8      field_3;
    s32     field_20;

    WorldEnvRender_LightTransform(worldMat, g_WorldEnvRenderWork.light.intensity, &g_WorldEnvRenderWork.light.direction, &g_WorldEnvRenderWork.light.position);

    gte_lddqa(g_WorldEnvRenderWork.light.field_0);
    gte_lddqb_0();
    gte_ldtr_0();

    ReadGeomOffset(&geomOffsetX, &geomOffsetY);
    geomScreen = ReadGeomScreen();
    SetGeomOffset(-1024, -1024);
    SetGeomScreen(16);

    rotMat.m[0][0] = g_WorldEnvRenderWork.light.field_28.vx;
    rotMat.m[0][1] = g_WorldEnvRenderWork.light.field_28.vy;
    rotMat.m[0][2] = g_WorldEnvRenderWork.light.field_28.vz;
    rotMat.m[1][0] = -arg1->vx;
    rotMat.m[1][1] = -arg1->vy;
    rotMat.m[1][2] = -arg1->vz;
    rotMat.m[2][0] = arg0->vx - g_WorldEnvRenderWork.light.field_30.vx;
    rotMat.m[2][1] = arg0->vy - g_WorldEnvRenderWork.light.field_30.vy;
    rotMat.m[2][2] = arg0->vz - g_WorldEnvRenderWork.light.field_30.vz;

    field_3  = g_WorldEnvRenderWork.lightLevelMax;
    field_20 = g_WorldEnvRenderWork.field_20 >> 5;

    SetRotMatrix(&rotMat);

    gte_ldv0(&rotMat.m[2][0]);
    gte_rtps();
    gte_stsxy(&screenPos);
    gte_stdp(&depthP);

    screenPos.vx += 1024;
    screenPos.vy += 1024;

    ret   = (screenPos.vx * screenPos.vy) + (screenPos.vy * (depthP >> 4));
    ret >>= 5;
    ret  -= 16;
    if (ret < 0)
    {
        ret = 0;
    }

    ret += field_20;

    var_v1 = (screenPos.vx >> 1) + (depthP >> 2);
    if (var_v1 > 48)
    {
        var_v1 = 48;
    }

    ret = ret + var_v1;
    if (field_3 < ret)
    {
        ret = field_3;
    }

    SetGeomOffset(geomOffsetX, geomOffsetY);
    SetGeomScreen(geomScreen);

    return ret;
}

void Lm_HeaderPtrsInit(s_LmHeader* lmHdr) // 0x800560FC
{
    s32 i;

    if (lmHdr->isLoaded == true)
    {
        return;
    }
    lmHdr->isLoaded = true;

    // Add memory address of header to pointer fields.
    lmHdr->materials  = (u8*)lmHdr->materials  + (u32)lmHdr;
    lmHdr->modelHdrs  = (u8*)lmHdr->modelHdrs  + (u32)lmHdr;
    lmHdr->modelOrder = (u8*)lmHdr->modelOrder + (u32)lmHdr;

    for (i = 0; i < lmHdr->modelCount; i++)
    {
        if (lmHdr->magic == LM_HEADER_MAGIC)
        {
            ModelHeader_FixOffsets(&lmHdr->modelHdrs[i], lmHdr);
        }
    }
}

void ModelHeader_FixOffsets(s_ModelHeader* modelHdr, s_LmHeader* lmHdr) // 0x800561A4
{
    s_MeshHeader* curMeshHdr;

    modelHdr->meshHdrs = (u8*)modelHdr->meshHdrs + (u32)lmHdr;

    for (curMeshHdr = &modelHdr->meshHdrs[0]; curMeshHdr < &modelHdr->meshHdrs[modelHdr->meshCount]; curMeshHdr++)
    {
        curMeshHdr->primitives = (u8*)curMeshHdr->primitives + (u32)lmHdr;
        curMeshHdr->verticesXy = (u8*)curMeshHdr->verticesXy + (u32)lmHdr;
        curMeshHdr->verticesZ  = (u8*)curMeshHdr->verticesZ  + (u32)lmHdr;
        curMeshHdr->normals    = (u8*)curMeshHdr->normals    + (u32)lmHdr;
        curMeshHdr->unkPtr_14  = (u8*)curMeshHdr->unkPtr_14  + (u32)lmHdr;
    }
}
