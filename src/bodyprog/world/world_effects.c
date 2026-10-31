#include "game.h"

#include <psyq/libetc.h>
#include <psyq/libpad.h>
#include <psyq/strings.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/gfx/map_effects.h"
#include "bodyprog/memcard.h"
#include "bodyprog/screen/screen_data.h"
#include "bodyprog/screen/screen_draw.h"
#include "bodyprog/text/text_draw.h"
#include "bodyprog/math/math.h"
#include "bodyprog/sound/sound_system.h"
#include "main/fsqueue.h"
#include "main/rng.h"

extern s_WorldEnvWork const g_WorldEnvWork;

s16 D_800BCDE8[8];

static s_MapEnvPresetIdxs D_800A9F80 = { 1, 1  };
static s_MapEnvPresetIdxs D_800A9F84 = { 2, 2  };
static s_MapEnvPresetIdxs D_800A9F88 = { 6, 3  };
static s_MapEnvPresetIdxs D_800A9F8C = { 7, 4  };
static s_MapEnvPresetIdxs D_800A9F90 = { 6, 10 };
static s_MapEnvPresetIdxs D_800A9F94 = { 6, 5  };
static s_MapEnvPresetIdxs D_800A9F98 = { 9, 9  };
static s_MapEnvPresetIdxs D_800A9F9C = { 6, 6  };
static s_MapEnvPresetIdxs D_800A9FA0 = { 3, 3  };
static s_MapEnvPresetIdxs D_800A9FA4 = { 5, 5  };

// ========================================
// EFFECTS (FOG AND LIGHT)
// ========================================

void GameFs_FlameGfxLoad(void) // 0x8003E710
{
#if VERSION_REGION_IS(PAL)
    static s_FsImageDesc IMG_FLAME = {
        .tPage = { 0, 13 },
        .u     = 0,
        .v     = 0,
        .clutX = 832,
        .clutY = 64
    };
#else
    static s_FsImageDesc IMG_FLAME = {
        .tPage = { 0, 12 },
        .u     = 32,
        .v     = 0,
        .clutX = 800,
        .clutY = 64
    };
#endif

    Fs_QueueStartReadTim(FILE_TIM_FLAME_TIM, FS_BUFFER_1, &IMG_FLAME);
}

// Attach the lighter effect at the beginning of the game.
#if VERSION_IS(EUR)
// @hack Statics of the included function below, remove once decompiled.
u32 D_800A9FB0_0 __asm__("D_800A9FB0.0") = 0;
INCLUDE_ASM("bodyprog/nonmatchings/world/world_effects", func_8003E740);
#else
void func_8003E740(void) // 0x8003E740
{
    DVECTOR   sp10;
    MATRIX    viewMat;
    SVECTOR   sp38;
    s32       sp40[4];
    SVECTOR   sp50;
    DVECTOR   sp58;
    s32       depthZ;
    s32       temp_a0;
    s32       temp_s6;
    s32       i;
    s32       var_s5;
    s16*      var_a0;
    POLY_FT4* poly;
    s32       idx = 0;

    static u32 D_800A9FB0 = 0;

    if (g_DeltaTime != Q12(0.0f))
    {
        D_800A9FB0 += 8;
        for (i = 0; i < 8; i++)
        {
            D_800BCDE8[i] = Rng_Rand16();
        }
    }

    sp38.vx = 1;
    sp38.vy = -7;
    sp38.vz = 33;
    sp38.vx = Q12_MULT(Math_AngleNormalize(D_800BCDE8[idx++]), 5) + 1;
    sp38.vz = Q12_MULT(Math_AngleNormalize(D_800BCDE8[idx++]), 5) + 33;

    poly = (POLY_FT4*)GsOUT_PACKET_P;

    Vw_CoordToViewSpaceMatrix(&g_SysWork.playerBoneCoords[HarryBone_RightHand], &viewMat);
    SetRotMatrix(&viewMat);
    SetTransMatrix(&viewMat);

    var_s5 = RotTransPers(&sp38, &sp10, &depthZ, &depthZ);

    temp_s6  = var_s5 * 4;
    var_s5 >>= 1;
    var_s5  -= 2;

    if (var_s5 < 0)
    {
        var_s5 = 0;
    }

    if (temp_s6 > 128 && var_s5 < ORDERING_TABLE_SIZE - 1)
    {
        SetPolyFT4(poly);
        setSemiTrans(poly, true);

        temp_a0 = D_800BCDE8[idx++];
        if ((temp_a0 & 0xFFF) >= 3482) // TODO: `> Q12(0.85f)` also matches, but this gets used for `setRGB0` color?
        {
            D_800A9FB0 -= 16 + (temp_a0 & 0xF);
        }

        if (D_800A9FB0 >= 33)
        {
            D_800A9FB0 = 0;
        }

        setRGB0(poly, D_800A9FB0 + 48, D_800A9FB0 + 48, D_800A9FB0 + 48);
        poly->tpage = 44;
        poly->clut  = 4146;

        var_a0 = &D_800BCDE8[idx++];

        for (i = 0; i < 4; i++)
        {
            sp40[i] = (var_a0[i] & 0xF) - 8;
        }

        SetRotMatrix(&GsIDMATRIX);
        SetTransMatrix(&GsIDMATRIX);

        sp50.vz = temp_s6;
        sp50.vx = sp40[0] - 51;
        sp50.vy = sp40[2] - 51;

        RotTransPers(&sp50, &sp58, &depthZ, &depthZ);

        poly->x0 = sp10.vx + sp58.vx;
        poly->y0 = sp10.vy + sp58.vy;
        sp50.vx  = sp40[1] + 51;
        sp50.vy  = sp40[3] - 51;

        RotTransPers(&sp50, &sp58, &depthZ, &depthZ);

        poly->x1 = sp10.vx + sp58.vx;
        poly->y1 = sp10.vy + sp58.vy;
        sp50.vx  = -51 - sp40[1];
        sp50.vy  = 51 - sp40[3];

        RotTransPers(&sp50, &sp58, &depthZ, &depthZ);

        poly->x2 = sp10.vx + sp58.vx;
        poly->y2 = sp10.vy + sp58.vy;
        sp50.vx  = 51 - sp40[0];
        sp50.vy  = 51 - sp40[2];

        RotTransPers(&sp50, &sp58, &depthZ, &depthZ);

        poly->x3 = sp10.vx + sp58.vx;
        poly->y3 = sp10.vy + sp58.vy;

        poly->u0 = 128;
        poly->u1 = 191;
        poly->u2 = 128;
        poly->u3 = 191;

        poly->v0 = 0;
        poly->v1 = 0;
        poly->v2 = 63;
        poly->v3 = 63;

        AddPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[var_s5], poly);
        GsOUT_PACKET_P = (PACKET*)poly + sizeof(POLY_FT4);
    }
}
#endif

void Game_SpotlightLoadScreenAttribsFix(void) // 0x8003EB54
{
    g_SysWork.lightIntensity     = Q12(1.0f);
    g_SysWork.lightBoneCoord     = &g_SysWork.playerBoneCoords[HarryBone_Root];
    g_SysWork.lensFlareBoneCoord = &g_SysWork.playerBoneCoords[HarryBone_Root];

    Math_Vector3Set(&g_SysWork.lightPosition, Q12(0.0f), Q12(-0.2f), Q12(-2.0f));
    Math_SVectorSet(&g_SysWork.lightRotation, Q12_ANGLE(10.0f), Q12_ANGLE(0.0f), Q12_ANGLE(0.0f));
}

void Game_FlashlightAttributesFix(void) // 0x8003EBA0
{
    g_SysWork.lightIntensity     = Q12(1.0f);
    g_SysWork.lightBoneCoord     = &g_SysWork.playerBoneCoords[HarryBone_Torso];
    g_SysWork.lensFlareBoneCoord = &g_SysWork.playerBoneCoords[HarryBone_Root];

    Math_Vector3Set(&g_SysWork.lightPosition, Q12(-0.08f), Q12(-0.28f), Q12(0.12f));
    Math_SVectorSet(&g_SysWork.lightRotation, Q12_ANGLE(-15.0f), Q12_ANGLE(0.0f), Q12_ANGLE(0.0f));
}

void WorldEnv_MapPresetSet(s_MapOverlayHdr* mapHdr) // 0x8003EBF4
{
    bool                hasActiveChunk;
    u8                  flags;
    s_MapEnvPresetIdxs* presetIdxPtr;

    flags          = mapHdr->mapInfo->flags;
    hasActiveChunk = false;
    if (flags & MapFlag_Interior)
    {
        hasActiveChunk = (flags & (MapFlag_OneActiveChunk | MapFlag_TwoActiveChunks)) > 0;
    }

    switch (mapHdr->field_16)
    {
        case 1:
            if (hasActiveChunk)
            {
                presetIdxPtr = &D_800A9F84;
            }
            else
            {
                presetIdxPtr = &D_800A9F80;
            }
            break;

        case 2:
            if (hasActiveChunk)
            {
                presetIdxPtr = &D_800A9F8C;
            }
            else
            {
                presetIdxPtr = &D_800A9F88;
            }
            break;

        case 3:
            presetIdxPtr = &D_800A9F98;
            break;

        default:
            presetIdxPtr = &D_800A9F80;
            break;
    }

    Gfx_MapEnvSet(presetIdxPtr->presetIdx0, presetIdxPtr->presetIdx1);
}

void Game_TurnFlashlightOn(void) // 0x8003ECBC
{
    g_SysWork.field_2388.isFlashlightOn = true;
    g_SavegamePtr->itemToggleFlags        &= ~ItemToggleFlag_FlashlightOff;
}

void Game_TurnFlashlightOff(void) // 0x8003ECE4
{
    g_SysWork.field_2388.isFlashlightOn = false;
    g_SavegamePtr->itemToggleFlags     |= ItemToggleFlag_FlashlightOff;
}

void Game_FlashlightToggle(void) // 0x8003ED08
{
    // Awkward `isFlashlightOn` toggle.
    g_SysWork.field_2388.isFlashlightOn ^= true;
    if (g_SysWork.field_2388.isFlashlightOn == true)
    {
        g_SavegamePtr->itemToggleFlags &= ~ItemToggleFlag_FlashlightOff;
    }
    else
    {
        g_SavegamePtr->itemToggleFlags |= ItemToggleFlag_FlashlightOff;
    }
}

bool Game_FlashlightIsOn(void) // 0x8003ED64
{
    return g_SysWork.field_2388.isFlashlightOn;
}

void Gfx_MapEnvSet(s32 idx0, s32 idx1) // 0x8003ED74
{
    Gfx_MapEnvUpdate(idx0, idx1, PrimitiveType_None, NULL, 0, 0);
    Gfx_EffectsUpdate();
}

void func_8003EDA8(void) // 0x8003EDA8
{
    g_SysWork.field_2388.flashEffect = true;
}

void func_8003EDB8(CVECTOR* color0, CVECTOR* color1) // 0x8003EDB8
{
    *color0 = g_SysWork.field_2388.field_1C[g_SysWork.field_2388.isFlashlightOn].effectsInfo.field_21;
    *color1 = g_SysWork.field_2388.field_1C[g_SysWork.field_2388.isFlashlightOn].effectsInfo.field_25;
}

void func_8003EE30(s32 arg0, s8* arg1, s32 arg2, s32 arg3) // 0x8003EE30
{
    g_SysWork.field_2388.field_4    = arg1;
    g_SysWork.field_2388.primitiveType = PrimitiveType_S32;
    g_SysWork.field_2388.field_8    = arg2;
    g_SysWork.field_2388.field_C    = arg3;

    g_SysWork.field_2388.field_EC[0] = g_SysWork.field_2388.field_1C[0];
    g_SysWork.field_2388.field_EC[1] = g_SysWork.field_2388.field_1C[1];
}

void Gfx_LoadScreenMapEffectsUpdate(s32 arg0, s32 arg1) // 0x8003EEDC
{
    Gfx_MapEnvUpdate(arg0, arg1, PrimitiveType_None, NULL, 0, 0);
    Gfx_EffectsUpdate();
}

void Gfx_MapEnvUpdate(s32 idx0, s32 idx1, e_PrimitiveType primType, void* primData, s32 arg4, s32 arg5) // 0x8003EF10
{
    Gfx_MapEnvStepUpdate(&MAP_EFFECTS_INFOS[idx0], &MAP_EFFECTS_INFOS[idx1], primType, primData, arg4, arg5);
}

void Gfx_MapEnvStepUpdate(const s_MapEffectsInfo* preset0, const s_MapEffectsInfo* preset1,
                                 e_PrimitiveType primType, void* primData, s32 arg4, s32 arg5) // 0x8003EF74
{
    if (preset0 == preset1)
    {
        g_SysWork.field_2388.isFlashlightUnavailable = true;
    }
    else
    {
        g_SysWork.field_2388.isFlashlightUnavailable = false;
    }

    g_SysWork.field_2388.field_4       = primData;
    g_SysWork.field_2388.primitiveType = primType;
    g_SysWork.field_2388.field_8       = arg4;
    g_SysWork.field_2388.field_C       = arg5;

    g_SysWork.field_2388.field_EC[0] = g_SysWork.field_2388.field_1C[0];
    g_SysWork.field_2388.field_EC[1] = g_SysWork.field_2388.field_1C[1];

    Gfx_FogParametersSet(&g_SysWork.field_2388.field_84[0], preset0);
    Gfx_FogParametersSet(&g_SysWork.field_2388.field_84[1], preset1);
}

void Gfx_FogParametersSet(s_StructUnk3* arg0, const s_MapEffectsInfo* effectsInfo) // 0x8003F08C
{
    arg0->effectsInfo = *effectsInfo;

    if (effectsInfo->flags.field_00[0] & SpecialEnvEventFlags_EnableBrightness)
    {
        arg0->brightnessIntensity = Q12(1.0f);
    }
    else
    {
        arg0->brightnessIntensity = Q12(0.0f);
    }

    if (effectsInfo->flags.field_00[0] & SpecialEnvEventFlags_EnableLensflare)
    {
        arg0->flashlightLensFlareIntensity = Q12(1.0f);
    }
    else
    {
        arg0->flashlightLensFlareIntensity = Q12(0.0f);
    }

    switch (effectsInfo->field_E)
    {
        case 0:
        case 1:
            arg0->fogDistance = effectsInfo->fogDistance;
            break;

        case 2:
            arg0->fogDistance = Q12(0.0f);
            break;

        case 3:
            arg0->fogDistance = effectsInfo->fogDistance;
            break;
    }
}

void Gfx_EffectsUpdate(void) // 0x8003F170
{
    MATRIX          viewMat;
    VECTOR          sp48;
    SVECTOR         rot; // Q19.12
    q19_12          weight;
    u8              flags;
    q19_12          lightIntensity;
    GsCOORDINATE2*  lightBoneCoord;
    s_StructUnk3*   currentInGameGfx;
    s_SysWork_2388* ptr;

    ptr = &g_SysWork.field_2388;

    if (g_SysWork.field_2388.isFlashlightOn)
    {
        g_SysWork.field_2388.flashlightIntensity += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 4.0f);
    }
    else
    {
        g_SysWork.field_2388.flashlightIntensity -= Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 4.0f);
    }

    g_SysWork.field_2388.flashlightIntensity = CLAMP(g_SysWork.field_2388.flashlightIntensity, Q12(0.0f), Q12(1.0f));

    /** @unused See `s_MapEffectsInfo::field_E`. */
    if (g_SysWork.field_2388.field_84[g_SysWork.field_2388.flashlightIntensity != Q12(0.0f)].effectsInfo.field_E == 3)
    {
        Vw_CoordToViewSpaceMatrix(g_SysWork.lightBoneCoord, &viewMat);
        ApplyMatrixLV(&viewMat, &g_SysWork.lightPosition, &sp48);
        ptr->field_84[g_SysWork.field_2388.flashlightIntensity != Q12(0.0f)].fogDistance = sp48.vz + Q8_TO_Q12(viewMat.t[2]);
    }

    if (ptr->primitiveType == PrimitiveType_None)
    {
        ptr->field_1C[0] = ptr->field_84[0];
        ptr->field_1C[1] = ptr->field_84[1];
    }
    else
    {
        weight = func_8003F6F0(func_8003F654(ptr), ptr->field_8, ptr->field_C);

        func_8003F838(&ptr->field_1C[0], &ptr->field_EC[0], &ptr->field_84[0], weight);
        func_8003F838(&ptr->field_1C[1], &ptr->field_EC[1], &ptr->field_84[1], weight);

        if (weight >= Q12(1.0f))
        {
            ptr->primitiveType = PrimitiveType_None;
        }
    }

    func_8003F838(&ptr->field_154, &ptr->field_1C[0], &ptr->field_1C[1], ptr->flashlightIntensity);

    currentInGameGfx = &ptr->field_154;

    if (ptr->flashEffect)
    {
        flags            = currentInGameGfx->effectsInfo.flags.field_00[0];
        ptr->flashEffect = false;

        if (flags & SpecialEnvEventFlags_DarkEnvironment)
        {
            Gfx_FogParametersSet(currentInGameGfx, &MAP_EFFECTS_INFOS[8]);
        }
        else if (flags & SpecialEnvEventFlags_FlashlightAllowed)
        {
            currentInGameGfx->effectsInfo.spotLightIntensity += Q12(0.3f);
        }
    }

    ptr->field_10 = func_8003FEC0(&currentInGameGfx->effectsInfo);
    WorldEnv_FogLightingParamsUpdate(currentInGameGfx);

    lightIntensity = Q12_MULT(func_8003F4DC(&lightBoneCoord, &rot, currentInGameGfx->effectsInfo.spotLightIntensity, currentInGameGfx->effectsInfo.flags.field_00[2], Vc_LensFlareTypeGet(), &g_SysWork), g_SysWork.lightIntensity);

    Gfx_FlashlightPositionUpdate(lightIntensity, currentInGameGfx->flashlightLensFlareIntensity, lightBoneCoord, g_SysWork.lightBoneCoord, &rot,
                                 g_SysWork.lightPosition.vx, g_SysWork.lightPosition.vy, g_SysWork.lightPosition.vz,
                                 g_WorldGfxWork.mapInfo->waterZones);
    func_80055814(currentInGameGfx->fogDistance);

    if (ptr->field_154.effectsInfo.flags.field_00[0] & SpecialEnvEventFlags_UseLighter)
    {
        func_8003E740();
    }
}

/** Adjust spotlight/flashlight lighting atributes.
 * Scratch: https://decomp.me/scratch/Crnh4
 */
static q19_12 func_8003F4DC(GsCOORDINATE2** lightBoneCoord, SVECTOR* rot, q19_12 lightWeight, s32 arg3, u32 lensFlare, s_SysWork* sysWork) // 0x8003F4DC
{
    s32     temp;
    q19_12  lightWeightCpy;
    SVECTOR rot0;

    // TODO: `arg4` is the value from `VC_ROAD_DATA::field_15`.

    if (arg3 != (1 << 1))
    {
        lensFlare = LensFlareType_Custom;
    }

    lightWeightCpy = lightWeight;
    if (lensFlare == LensFlareType_Default)
    {
        lightWeightCpy = Q12(0.0f);
    }

    switch (lensFlare)
    {
        default:
        case LensFlareType_Custom:
            *lightBoneCoord = sysWork->lensFlareBoneCoord;
            break;

        case LensFlareType_Default:
        case LensFlareType_Preset1:
        case LensFlareType_Preset2:
        case LensFlareType_Preset3:
        case LensFlareType_Preset4:
            *lightBoneCoord = NULL;
            break;
    }

    switch (lensFlare)
    {
        default:
        case LensFlareType_Custom:
            rot0 = sysWork->lightRotation;
            break;

        case LensFlareType_Default:
            rot0.vx = Q12_ANGLE(0.0f);
            rot0.vy = Q12_ANGLE(-90.0f);
            rot0.vz = Q12_ANGLE(0.0f);
            break;

        case LensFlareType_Preset1:
            rot0.vx = Q12_ANGLE(-20.0f);
            rot0.vy = Q12_ANGLE(195.0f);
            rot0.vz = Q12_ANGLE(0.0f);
            break;

        case LensFlareType_Preset2:
            rot0.vx = Q12_ANGLE(-20.0f);
            rot0.vy = Q12_ANGLE(-75.0f);
            rot0.vz = Q12_ANGLE(0.0f);
            break;

        case LensFlareType_Preset3:
            rot0.vx = Q12_ANGLE(-20.0f);
            rot0.vy = Q12_ANGLE(15.0f);
            rot0.vz = Q12_ANGLE(0.0f);
            break;

        case LensFlareType_Preset4:
            rot0.vx = Q12_ANGLE(-20.0f);
            rot0.vy = Q12_ANGLE(105.0f);
            rot0.vz = Q12_ANGLE(0.0f);
            break;
    }

    rot->vy = -Math_Sin(rot0.vx);
    temp    =  Math_Cos(rot0.vx);
    rot->vz = Q12_MULT(temp, Math_Cos(rot0.vy));
    rot->vx = Q12_MULT(temp, Math_Sin(rot0.vy));
    return lightWeightCpy;
}

/**
 *
 * Scratch: https://decomp.me/scratch/Tbwyz
 *
 * @return Likely q20_12?
 */
static u32 func_8003F654(s_SysWork_2388* arg0)
{
    switch (arg0->primitiveType)
    {
        default:
        case PrimitiveType_None:
            break;

        case PrimitiveType_S8:
            return *arg0->field_4;

        case PrimitiveType_U8:
            return *(u8*)arg0->field_4;

        case PrimitiveType_S16:
            return *(s16*)arg0->field_4;

        case PrimitiveType_U16:
            return *(u16*)arg0->field_4;

        case PrimitiveType_S32:
            return *(s32*)arg0->field_4;
    }

    return 0;
}

/** @brief Computes the normalized progress alpha in the range `[0.0f, 1.0f]`.
 *
 * Scratch: https://decomp.me/scratch/XdPoR
 *
 * @param val Current value.
 * @param min Minumum range.
 * @param max Maximum range.
 * @return Normalized progress alpha.
 */
static q19_12 func_8003F6F0(s32 val, s32 min, s32 max)
{
    #define Q12_BITS     32
    #define Q12_VAL_BITS 31
    #define Q12_INT_BITS 19

    s32 leadingZeros;
    s32 shift;

    if (min < max)
    {
        val = CLAMP(val, min, max);
    }
    else if (max < min)
    {
        val = CLAMP(val, max, min);
    }
    else
    {
        return Q12(1.0f);
    }

    leadingZeros = Q12_BITS - Lzc(max - min);
    shift        = 0;

    if ((leadingZeros + Q12_SHIFT) >= Q12_VAL_BITS)
    {
        shift = leadingZeros - Q12_INT_BITS;
    }
    shift = CLAMP(shift, 0, Q12_SHIFT);

    return ((val - min) << (Q12_SHIFT - shift)) / ((max - min) >> shift);

    #undef Q12_BITS
    #undef Q12_VAL_BITS
    #undef Q12_INT_BITS
}

q19_12 Math_WeightedAverageGet(s32 a, s32 b, q19_12 weight) // 0x8003F7E4
{
    return Math_MulFixed(a, Q12(1.0f) - weight, Q12_SHIFT) + Math_MulFixed(b, weight, Q12_SHIFT);
}

/**
 * Scratch: https://decomp.me/scratch/asgeE
 */
static void func_8003F838(s_StructUnk3* target, s_StructUnk3* envSettings0, s_StructUnk3* envSettings1, q19_12 weight) // 0x8003F838
{
    q19_12 weight0;
    q19_12 weight1;
    q19_12 weight2;
    u32    temp;

    weight0 = weight * 2;
    weight0 = CLAMP(weight0, Q12(0.0f), Q12(1.0f));
    
    weight1 = (weight - Q12(0.5f)) * 2;
    weight1 = CLAMP(weight1, Q12(0.0f), Q12(1.0f));

    // Copy enviroment flags.
    if (weight < Q12(0.5f))
    {
        target->effectsInfo.flags.field_00[0] = envSettings0->effectsInfo.flags.field_00[0];
    }
    else
    {
        target->effectsInfo.flags.field_00[0] = envSettings1->effectsInfo.flags.field_00[0];
    }

    func_8003FCB0(&target->effectsInfo, &envSettings0->effectsInfo, &envSettings1->effectsInfo, weight);

    if (envSettings0->flashlightLensFlareIntensity == Q12(0.0f))
    {
        target->flashlightLensFlareIntensity = Math_WeightedAverageGet(Q12(0.0f), envSettings1->flashlightLensFlareIntensity, weight1);
    }
    else
    {
        target->flashlightLensFlareIntensity = Math_WeightedAverageGet(envSettings0->flashlightLensFlareIntensity, envSettings1->flashlightLensFlareIntensity, weight0);
    }

    if (envSettings0->effectsInfo.flags.field_00[0] & SpecialEnvEventFlags_DarkEnvironment)
    {
        if (envSettings1->effectsInfo.flags.field_00[0] & SpecialEnvEventFlags_DarkEnvironment)
        {
            target->effectsInfo.flags.field_00[1] = Math_WeightedAverageGet(envSettings0->effectsInfo.flags.field_00[1], envSettings1->effectsInfo.flags.field_00[1], weight);
        }
        else
        {
            target->effectsInfo.flags.field_00[1] = Math_WeightedAverageGet(envSettings0->effectsInfo.flags.field_00[1], envSettings1->effectsInfo.flags.field_00[1], weight1);
        }
    }
    else
    {
        if (envSettings1->effectsInfo.flags.field_00[0] & SpecialEnvEventFlags_DarkEnvironment)
        {
            target->effectsInfo.flags.field_00[1] = Math_WeightedAverageGet(envSettings0->effectsInfo.flags.field_00[1], envSettings1->effectsInfo.flags.field_00[1], weight0);
        }
        else
        {
            target->effectsInfo.flags.field_00[1] = Math_WeightedAverageGet(envSettings0->effectsInfo.flags.field_00[1], envSettings1->effectsInfo.flags.field_00[1], weight);
        }
    }

    // Fog values adjustment.
    if (envSettings0->effectsInfo.field_E == 0)
    {
        if (envSettings1->effectsInfo.field_E != 0)
        {
            target->effectsInfo.field_E = envSettings1->effectsInfo.field_E;
            func_8003FD38(target, envSettings0, envSettings1, weight, weight0, weight1);
        }
        else
        {
            target->effectsInfo.field_E = temp = envSettings1->effectsInfo.field_E;
            func_8003FD38(target, envSettings0, envSettings1, weight, weight, weight);
        }
    }
    else if (envSettings1->effectsInfo.field_E == 0)
    {
        if (weight1 >= Q12(1.0f))
        {
            target->effectsInfo.field_E = envSettings1->effectsInfo.field_E;
        }
        else
        {
            target->effectsInfo.field_E = envSettings0->effectsInfo.field_E;
        }

        func_8003FD38(target, envSettings0, envSettings1, weight, weight1, weight0);
    }
    else
    {
        target->effectsInfo.field_E = temp = envSettings1->effectsInfo.field_E;
        func_8003FD38(target, envSettings0, envSettings1, weight, weight, weight);
    }

    target->effectsInfo.worldTintR = Math_WeightedAverageGet(envSettings0->effectsInfo.worldTintR, envSettings1->effectsInfo.worldTintR, weight);
    target->effectsInfo.worldTintG = Math_WeightedAverageGet(envSettings0->effectsInfo.worldTintG, envSettings1->effectsInfo.worldTintG, weight);
    target->effectsInfo.worldTintB = Math_WeightedAverageGet(envSettings0->effectsInfo.worldTintB, envSettings1->effectsInfo.worldTintB, weight);

    if (envSettings0->effectsInfo.flags.field_00[2] == UnkGfxEnum_1 && envSettings1->effectsInfo.flags.field_00[2] == UnkGfxEnum_2)
    {
        if (weight < Q12(5.0f / 6.0f))
        {
            weight2                                = Q12_MULT(weight, Q12(1.2f));
            weight2                                = CLAMP(weight2, Q12(0.0f), Q12(1.0f));
            target->effectsInfo.flags.field_00[2]  = envSettings0->effectsInfo.flags.field_00[2];
            target->effectsInfo.spotLightIntensity = Math_WeightedAverageGet(envSettings0->effectsInfo.spotLightIntensity, 0, weight2);
        }
        else
        {
            weight2                                = (weight - Q12(5.0f / 6.0f)) * 6;
            weight2                                = CLAMP(weight2, Q12(0.0f), Q12(1.0f));
            target->effectsInfo.flags.field_00[2]  = envSettings1->effectsInfo.flags.field_00[2];
            weight0                                = envSettings1->effectsInfo.spotLightIntensity;
            target->effectsInfo.spotLightIntensity = Math_WeightedAverageGet(Q12(0.0f), weight0, weight2);
        }
    }
    else if (envSettings0->effectsInfo.flags.field_00[2] == UnkGfxEnum_2 && envSettings1->effectsInfo.flags.field_00[2] == UnkGfxEnum_1)
    {
        if (weight < Q12(1.0f / 6.0f))
        {
            weight2                                = weight * 6;
            weight2                                = CLAMP(weight2, Q12(0.0f), Q12(1.0f));
            target->effectsInfo.flags.field_00[2]  = envSettings0->effectsInfo.flags.field_00[2];
            target->effectsInfo.spotLightIntensity = Math_WeightedAverageGet(envSettings0->effectsInfo.spotLightIntensity, Q12(0.0f), weight2);
        }
        else
        {
            weight2                                = Q12_MULT(weight - Q12(1.0f / 6.0f), Q12(1.2f));
            weight2                                = CLAMP(weight2, Q12(0.0f), Q12(1.0f));
            target->effectsInfo.flags.field_00[2]  = envSettings1->effectsInfo.flags.field_00[2];
            target->effectsInfo.spotLightIntensity = Math_WeightedAverageGet(Q12(0.0f), envSettings1->effectsInfo.spotLightIntensity, weight2);
        }
    }
    else
    {
        if (envSettings0->effectsInfo.flags.field_00[2] != UnkGfxEnum_0 && envSettings1->effectsInfo.flags.field_00[2] == UnkGfxEnum_0)
        {
            if (weight >= Q12(1.0f))
            {
                target->effectsInfo.flags.field_00[2] = envSettings1->effectsInfo.flags.field_00[2];
            }
            else
            {
                target->effectsInfo.flags.field_00[2] = envSettings0->effectsInfo.flags.field_00[2];
            }
        }
        else
        {
            target->effectsInfo.flags.field_00[2] = envSettings1->effectsInfo.flags.field_00[2];
        }

        target->effectsInfo.spotLightIntensity = Math_WeightedAverageGet(envSettings0->effectsInfo.spotLightIntensity, envSettings1->effectsInfo.spotLightIntensity, weight);
    }

    if (envSettings0->effectsInfo.enableTintLightOverlap == false && envSettings1->effectsInfo.enableTintLightOverlap != false)
    {
        func_8003FE04(&target->effectsInfo, &envSettings0->effectsInfo, &envSettings1->effectsInfo, weight1);
    }
    else
    {
        func_8003FE04(&target->effectsInfo, &envSettings0->effectsInfo, &envSettings1->effectsInfo, weight);
    }
}

/**
 * Scratch: https://decomp.me/scratch/wk8iD
 */
static void func_8003FCB0(s_MapEffectsInfo* target, const s_MapEffectsInfo* envSettings0, const s_MapEffectsInfo* envSettings1, q19_12 alphaTo)
{
    q19_12 alphaFrom;

    alphaFrom = Q12(1.0f) - alphaTo;
    LoadAverageCol(&envSettings0->field_21.r, &envSettings1->field_21.r, alphaFrom, alphaTo, &target->field_21.r);
    LoadAverageCol(&envSettings0->field_25.r, &envSettings1->field_25.r, alphaFrom, alphaTo, &target->field_25.r);
}

/**
 * Scratch: https://decomp.me/scratch/jhfrd
 */
static void func_8003FD38(s_StructUnk3* target, const s_StructUnk3* envSettings0, const s_StructUnk3* envSettings1, q19_12 weight0, q19_12 weight1, q19_12 alphaTo)
{
    if (envSettings0->brightnessIntensity != envSettings1->brightnessIntensity)
    {
        target->brightnessIntensity = Math_WeightedAverageGet(envSettings0->brightnessIntensity, envSettings1->brightnessIntensity, weight0);
    }
    else
    {
        target->brightnessIntensity = envSettings1->brightnessIntensity;
    }

    target->fogDistance                    = Math_WeightedAverageGet(envSettings0->fogDistance, envSettings1->fogDistance, weight0);
    target->effectsInfo.fogDistance        = Math_WeightedAverageGet(envSettings0->effectsInfo.fogDistance, envSettings1->effectsInfo.fogDistance, weight1);
    target->effectsInfo.worldLightIntensity = Math_WeightedAverageGet(envSettings0->effectsInfo.worldLightIntensity, envSettings1->effectsInfo.worldLightIntensity, weight0);

    LoadAverageCol(&envSettings0->effectsInfo.fogColor.r, &envSettings1->effectsInfo.fogColor.r, Q12(1.0f) - alphaTo, alphaTo, &target->effectsInfo.fogColor.r);
}

void func_8003FE04(const s_MapEffectsInfo* arg0, const s_MapEffectsInfo* arg1, const s_MapEffectsInfo* arg2, q19_12 alphaTo) // 0x8003FE04
{
    q19_12 alphaFrom;

    alphaFrom = Q12(1.0f) - alphaTo;
    LoadAverageCol(&arg1->pointLightTint.r, &arg2->pointLightTint.r, alphaFrom, alphaTo, &arg0->pointLightTint.r);
    LoadAverageCol(&arg1->worldTint.r, &arg2->worldTint.r, alphaFrom, alphaTo, &arg0->worldTint.r);

    if ((arg0->pointLightTint.r || arg0->pointLightTint.g || arg0->pointLightTint.b) ||
        (arg0->worldTint.r || arg0->worldTint.g || arg0->worldTint.b))
    {
        arg0->enableTintLightOverlap = true;
    }
    else
    {
        arg0->enableTintLightOverlap = false;
    }
}

s32 func_8003FEC0(const s_MapEffectsInfo* arg0) // 0x8003FEC0
{
    static q19_12 Y_ARRAY[5] = {
        Q12(1.75f),
        Q12(6.0f),
        Q12(9.5f),
        Q12(12.5f),
        Q12(15.0f)
    };

    if (g_WorldEnvWork.isFogEnabled)
    {
        return arg0->fogDistance;
    }

    if (g_WorldEnvWork.field_0 == UnkGfxEnum_1)
    {
        return vwOresenHokan(Y_ARRAY, ARRAY_SIZE(Y_ARRAY), arg0->spotLightIntensity, 0, Q12(2.0f));
    }

    return Q12(20.0f);
}

void WorldEnv_FogLightingParamsUpdate(s_StructUnk3* arg0) // 0x8003FF2C
{
    s32   fogDistCpy;
    s32   temp_v1;
    q23_8 brightness;

    temp_v1    = Q12_MULT(arg0->brightnessIntensity, (g_GameWork.config.brightness * 8) + 4);
    brightness = CLAMP(temp_v1, Q8_CLAMPED(0.0f), Q8_CLAMPED(1.0f));

    WorldEnv_WorldLightingParamSet(arg0->effectsInfo.flags.field_00[2], arg0->effectsInfo.worldLightIntensity, arg0->effectsInfo.flags.field_00[1], arg0->effectsInfo.worldTintR, arg0->effectsInfo.worldTintG, arg0->effectsInfo.worldTintB, brightness);
    WorldEnv_FogParamsSet(arg0->effectsInfo.field_E != 0, arg0->effectsInfo.fogColor.r, arg0->effectsInfo.fogColor.g, arg0->effectsInfo.fogColor.b);

    fogDistCpy = arg0->effectsInfo.fogDistance;

    WorldEnv_FogDistanceSet(fogDistCpy, fogDistCpy + Q12(1.0f));
    WorldEnv_WorldLightTintSet(arg0->effectsInfo.enableTintLightOverlap, arg0->effectsInfo.pointLightTint.r, arg0->effectsInfo.pointLightTint.g, arg0->effectsInfo.pointLightTint.b, arg0->effectsInfo.worldTint.r, arg0->effectsInfo.worldTint.g, arg0->effectsInfo.worldTint.b);
}
