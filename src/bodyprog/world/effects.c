#include "bodyprog/game.h"

#include <psyq/libetc.h>
#include <psyq/libpad.h>
#include <psyq/strings.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/world/effects.h"
#include "bodyprog/sys/memcard.h"
#include "bodyprog/screen/data.h"
#include "bodyprog/screen/draw.h"
#include "bodyprog/text/text_draw.h"
#include "bodyprog/math/math.h"
#include "bodyprog/sound/sd_call.h"
#include "main/fs_queue.h"
#include "main/rng.h"

extern s_WorldEnvRenderWork const g_WorldEnvRenderWork;

q3_12 D_800BCDE8[8];

static s_WorldEnvPresetIdxs D_800A9F80 = { 1, 1  };
static s_WorldEnvPresetIdxs D_800A9F84 = { 2, 2  };
static s_WorldEnvPresetIdxs D_800A9F88 = { 6, 3  };
static s_WorldEnvPresetIdxs D_800A9F8C = { 7, 4  };
static s_WorldEnvPresetIdxs D_800A9F90 = { 6, 10 };
static s_WorldEnvPresetIdxs D_800A9F94 = { 6, 5  };
static s_WorldEnvPresetIdxs D_800A9F98 = { 9, 9  };
static s_WorldEnvPresetIdxs D_800A9F9C = { 6, 6  };
static s_WorldEnvPresetIdxs D_800A9FA0 = { 3, 3  };
static s_WorldEnvPresetIdxs D_800A9FA4 = { 5, 5  };

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

#if VERSION_REGION_IS(PAL)
        poly->tpage = 45;
        poly->clut  = 4148;
#else
        poly->tpage = 44;
        poly->clut  = 4146;
#endif

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

#if VERSION_REGION_IS(PAL)
        poly->u0 = 0;
        poly->u1 = 63;
        poly->u2 = 0;
        poly->u3 = 63;
#else
        poly->u0 = 128;
        poly->u1 = 191;
        poly->u2 = 128;
        poly->u3 = 191;
#endif

        poly->v0 = 0;
        poly->v1 = 0;
        poly->v2 = 63;
        poly->v3 = 63;

        AddPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[var_s5], poly);
        GsOUT_PACKET_P = (PACKET*)poly + sizeof(POLY_FT4);
    }
}

void Game_SpotlightLoadScreenAttribsSet(void) // 0x8003EB54
{
    g_SysWork.lightIntensity     = Q12(1.0f);
    g_SysWork.lightBoneCoord     = &g_SysWork.playerBoneCoords[HarryBone_Root];
    g_SysWork.lensFlareBoneCoord = &g_SysWork.playerBoneCoords[HarryBone_Root];

    Math_Vector3Set(&g_SysWork.lightPosition, Q12(0.0f), Q12(-0.2f), Q12(-2.0f));
    Math_SVectorSet(&g_SysWork.lightRotation, Q12_ANGLE(10.0f), Q12_ANGLE(0.0f), Q12_ANGLE(0.0f));
}

void Game_FlashlightAttribsSet(void) // 0x8003EBA0
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
    s_WorldEnvPresetIdxs* presetIdxPtr;

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

    WorldEnv_PresetsSet(presetIdxPtr->presetIdx0, presetIdxPtr->presetIdx1);
}

void Game_TurnFlashlightOn(void) // 0x8003ECBC
{
    g_SysWork.worldEnv.isFlashlightOn = true;
    g_SavegamePtr->itemToggleFlags   &= ~ItemToggleFlag_FlashlightOff;
}

void Game_TurnFlashlightOff(void) // 0x8003ECE4
{
    g_SysWork.worldEnv.isFlashlightOn = false;
    g_SavegamePtr->itemToggleFlags   |= ItemToggleFlag_FlashlightOff;
}

void Game_FlashlightToggle(void) // 0x8003ED08
{
    // Awkward `isFlashlightOn` toggle.
    g_SysWork.worldEnv.isFlashlightOn ^= true;
    if (WorldEnv_IsFlashlightOn() == true)
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
    return WorldEnv_IsFlashlightOn();
}

void WorldEnv_PresetsSet(s32 idx0, s32 idx1) // 0x8003ED74
{
    WorldEnv_TransitionStart(idx0, idx1, PrimitiveType_None, NULL, 0, 0);
    WorldEnv_Update();
}

void func_8003EDA8(void) // 0x8003EDA8
{
    g_SysWork.worldEnv.flashEffect = true;
}

void WorldEnv_SnowRainColorGet(CVECTOR* snowColor, CVECTOR* rainColor) // 0x8003EDB8
{
    *snowColor = g_SysWork.worldEnv.transition[WorldEnv_IsFlashlightOn()].params.snowColor;
    *rainColor = g_SysWork.worldEnv.transition[WorldEnv_IsFlashlightOn()].params.rainColor;
}

void func_8003EE30(s32 arg0, s8* arg1, s32 arg2, s32 arg3) // 0x8003EE30
{
    g_SysWork.worldEnv.progressValue    = arg1;
    g_SysWork.worldEnv.progressType = PrimitiveType_S32;
    g_SysWork.worldEnv.progressStart    = arg2;
    g_SysWork.worldEnv.progressEnd    = arg3;

    g_SysWork.worldEnv.transitionFrom[0] = g_SysWork.worldEnv.transition[0];
    g_SysWork.worldEnv.transitionFrom[1] = g_SysWork.worldEnv.transition[1];
}

void WorldEnv_LoadScreenPresetsSet(s32 arg0, s32 arg1) // 0x8003EEDC
{
    WorldEnv_TransitionStart(arg0, arg1, PrimitiveType_None, NULL, 0, 0);
    WorldEnv_Update();
}

void WorldEnv_TransitionStart(s32 idx0, s32 idx1, e_PrimitiveType primType, void* primData, s32 arg4, s32 arg5) // 0x8003EF10
{
    WorldEnv_TransitionStartFromPresets(&WORLD_ENV_PRESETS[idx0], &WORLD_ENV_PRESETS[idx1], primType, primData, arg4, arg5);
}

void WorldEnv_TransitionStartFromPresets(const s_WorldEnvPreset* preset0, const s_WorldEnvPreset* preset1,
                                         e_PrimitiveType primType, void* primData, s32 arg4, s32 arg5) // 0x8003EF74
{
    if (preset0 == preset1)
    {
        g_SysWork.worldEnv.isFlashlightUnavailable = true;
    }
    else
    {
        g_SysWork.worldEnv.isFlashlightUnavailable = false;
    }

    g_SysWork.worldEnv.progressValue = primData;
    g_SysWork.worldEnv.progressType  = primType;
    g_SysWork.worldEnv.progressStart = arg4;
    g_SysWork.worldEnv.progressEnd   = arg5;

    g_SysWork.worldEnv.transitionFrom[0] = g_SysWork.worldEnv.transition[0];
    g_SysWork.worldEnv.transitionFrom[1] = g_SysWork.worldEnv.transition[1];

    WorldEnv_StateFromPreset(&g_SysWork.worldEnv.transitionTo[0], preset0);
    WorldEnv_StateFromPreset(&g_SysWork.worldEnv.transitionTo[1], preset1);
}

void WorldEnv_StateFromPreset(s_WorldEnvState* arg0, const s_WorldEnvPreset* preset) // 0x8003F08C
{
    arg0->params = *preset;

    if (preset->flags & WorldEnvFlag_EnableBrightness)
    {
        arg0->brightnessIntensity = Q12(1.0f);
    }
    else
    {
        arg0->brightnessIntensity = Q12(0.0f);
    }

    if (preset->flags & WorldEnvFlag_EnableLensFlare)
    {
        arg0->flashlightLensFlareIntensity = Q12(1.0f);
    }
    else
    {
        arg0->flashlightLensFlareIntensity = Q12(0.0f);
    }

    switch (preset->fogMode)
    {
        case WorldEnvFogMode_Off:
        case WorldEnvFogMode_On:
            arg0->fogDistance = preset->fogDistance;
            break;

        case WorldEnvFogMode_Zero:
            arg0->fogDistance = Q12(0.0f);
            break;

        case WorldEnvFogMode_FollowLight:
            arg0->fogDistance = preset->fogDistance;
            break;
    }
}

// https://decomp.me/scratch/XWo1G
void WorldEnv_Update(void) // 0x8003F170
{
    MATRIX           viewMat;
    VECTOR           sp48;
    SVECTOR          rot; // Q19.12
    q19_12           weight;
    u8               flags;
    q19_12           lightIntensity;
    GsCOORDINATE2*   lightBoneCoord;
    s_WorldEnvState* currentInGameGfx;
    s_WorldEnvWork*  worldEnv;

    worldEnv = &g_SysWork.worldEnv;

    if (WorldEnv_IsFlashlightOn())
    {
        g_SysWork.worldEnv.flashlightIntensity += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 4.0f);
    }
    else
    {
        g_SysWork.worldEnv.flashlightIntensity -= Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 4.0f);
    }

    g_SysWork.worldEnv.flashlightIntensity = CLAMP(g_SysWork.worldEnv.flashlightIntensity, Q12(0.0f), Q12(1.0f));

    /** @unused See `WorldEnvFogMode_FollowLight`. */
    if (g_SysWork.worldEnv.transitionTo[g_SysWork.worldEnv.flashlightIntensity != Q12(0.0f)].params.fogMode == WorldEnvFogMode_FollowLight)
    {
        Vw_CoordToViewSpaceMatrix(g_SysWork.lightBoneCoord, &viewMat);
        ApplyMatrixLV(&viewMat, &g_SysWork.lightPosition, &sp48);
        worldEnv->transitionTo[g_SysWork.worldEnv.flashlightIntensity != Q12(0.0f)].fogDistance = sp48.vz + Q8_TO_Q12(viewMat.t[2]);
    }

    if (worldEnv->progressType == PrimitiveType_None)
    {
        worldEnv->transition[0] = worldEnv->transitionTo[0];
        worldEnv->transition[1] = worldEnv->transitionTo[1];
    }
    else
    {
        weight = func_8003F6F0(func_8003F654(worldEnv), worldEnv->progressStart, worldEnv->progressEnd);

        func_8003F838(&worldEnv->transition[0], &worldEnv->transitionFrom[0], &worldEnv->transitionTo[0], weight);
        func_8003F838(&worldEnv->transition[1], &worldEnv->transitionFrom[1], &worldEnv->transitionTo[1], weight);

        if (weight >= Q12(1.0f))
        {
            worldEnv->progressType = PrimitiveType_None;
        }
    }

    func_8003F838(&worldEnv->current, &worldEnv->transition[0], &worldEnv->transition[1], worldEnv->flashlightIntensity);

    currentInGameGfx = &worldEnv->current;

    if (worldEnv->flashEffect)
    {
        flags                 = currentInGameGfx->params.flags;
        worldEnv->flashEffect = false;

        if (flags & WorldEnvFlag_Dark)
        {
            WorldEnv_StateFromPreset(currentInGameGfx, &WORLD_ENV_PRESETS[8]);
        }
        else if (flags & WorldEnvFlag_FlashlightAllowed)
        {
            currentInGameGfx->params.spotLightIntensity += Q12(0.3f);
        }
    }

    worldEnv->visibilityDistance = func_8003FEC0(&currentInGameGfx->params);
    WorldEnv_Apply(currentInGameGfx);

    lightIntensity = Q12_MULT(func_8003F4DC(&lightBoneCoord, &rot, currentInGameGfx->params.spotLightIntensity, currentInGameGfx->params.lightMode, Vc_LensFlareTypeGet(), &g_SysWork), g_SysWork.lightIntensity);

    Gfx_FlashlightPositionUpdate(lightIntensity, currentInGameGfx->flashlightLensFlareIntensity, lightBoneCoord, g_SysWork.lightBoneCoord, &rot,
                                 g_SysWork.lightPosition.vx, g_SysWork.lightPosition.vy, g_SysWork.lightPosition.vz,
                                 g_WorldGfxWork.mapInfo->waterZones);
    func_80055814(currentInGameGfx->fogDistance);

    if (worldEnv->current.params.flags & WorldEnvFlag_UseLighter)
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
static u32 func_8003F654(s_WorldEnvWork* arg0)
{
    switch (arg0->progressType)
    {
        default:
        case PrimitiveType_None:
            break;

        case PrimitiveType_S8:
            return *arg0->progressValue;

        case PrimitiveType_U8:
            return *(u8*)arg0->progressValue;

        case PrimitiveType_S16:
            return *(s16*)arg0->progressValue;

        case PrimitiveType_U16:
            return *(u16*)arg0->progressValue;

        case PrimitiveType_S32:
            return *(s32*)arg0->progressValue;
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

/** @brief Computes the weighted average of `a` and `b`.
 *
 * @param a First value.
 * @param b Second value.
 * @param weight Weight as a fixed-point alpha in Q3.12, range `[0, 4096]`.
 * @return Weighted average of `a` and `b`.
 */
static q19_12 Math_WeightedAverageGet(s32 a, s32 b, q19_12 weight) // 0x8003F7E4
{
    return Math_MulFixed(a, Q12(1.0f) - weight, Q12_SHIFT) + Math_MulFixed(b, weight, Q12_SHIFT);
}

/**
 * Scratch: https://decomp.me/scratch/asgeE
 */
static void func_8003F838(s_WorldEnvState* target, s_WorldEnvState* envSettings0, s_WorldEnvState* envSettings1, q19_12 weight) // 0x8003F838
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
        target->params.flags = envSettings0->params.flags;
    }
    else
    {
        target->params.flags = envSettings1->params.flags;
    }

    func_8003FCB0(&target->params, &envSettings0->params, &envSettings1->params, weight);

    if (envSettings0->flashlightLensFlareIntensity == Q12(0.0f))
    {
        target->flashlightLensFlareIntensity = Math_WeightedAverageGet(Q12(0.0f), envSettings1->flashlightLensFlareIntensity, weight1);
    }
    else
    {
        target->flashlightLensFlareIntensity = Math_WeightedAverageGet(envSettings0->flashlightLensFlareIntensity, envSettings1->flashlightLensFlareIntensity, weight0);
    }

    if (envSettings0->params.flags & WorldEnvFlag_Dark)
    {
        if (envSettings1->params.flags & WorldEnvFlag_Dark)
        {
            target->params.lightLevelMax = Math_WeightedAverageGet(envSettings0->params.lightLevelMax, envSettings1->params.lightLevelMax, weight);
        }
        else
        {
            target->params.lightLevelMax = Math_WeightedAverageGet(envSettings0->params.lightLevelMax, envSettings1->params.lightLevelMax, weight1);
        }
    }
    else
    {
        if (envSettings1->params.flags & WorldEnvFlag_Dark)
        {
            target->params.lightLevelMax = Math_WeightedAverageGet(envSettings0->params.lightLevelMax, envSettings1->params.lightLevelMax, weight0);
        }
        else
        {
            target->params.lightLevelMax = Math_WeightedAverageGet(envSettings0->params.lightLevelMax, envSettings1->params.lightLevelMax, weight);
        }
    }

    // Fog values adjustment.
    if (envSettings0->params.fogMode == WorldEnvFogMode_Off)
    {
        if (envSettings1->params.fogMode != WorldEnvFogMode_Off)
        {
            target->params.fogMode = envSettings1->params.fogMode;
            func_8003FD38(target, envSettings0, envSettings1, weight, weight0, weight1);
        }
        else
        {
            target->params.fogMode = temp = envSettings1->params.fogMode;
            func_8003FD38(target, envSettings0, envSettings1, weight, weight, weight);
        }
    }
    else if (envSettings1->params.fogMode == WorldEnvFogMode_Off)
    {
        if (weight1 >= Q12(1.0f))
        {
            target->params.fogMode = envSettings1->params.fogMode;
        }
        else
        {
            target->params.fogMode = envSettings0->params.fogMode;
        }

        func_8003FD38(target, envSettings0, envSettings1, weight, weight1, weight0);
    }
    else
    {
        target->params.fogMode = temp = envSettings1->params.fogMode;
        func_8003FD38(target, envSettings0, envSettings1, weight, weight, weight);
    }

    target->params.worldTintR = Math_WeightedAverageGet(envSettings0->params.worldTintR, envSettings1->params.worldTintR, weight);
    target->params.worldTintG = Math_WeightedAverageGet(envSettings0->params.worldTintG, envSettings1->params.worldTintG, weight);
    target->params.worldTintB = Math_WeightedAverageGet(envSettings0->params.worldTintB, envSettings1->params.worldTintB, weight);

    if (envSettings0->params.lightMode == WorldEnvLightMode_Spot && envSettings1->params.lightMode == WorldEnvLightMode_Directional)
    {
        if (weight < Q12(5.0f / 6.0f))
        {
            weight2                           = Q12_MULT(weight, Q12(1.2f));
            weight2                           = CLAMP(weight2, Q12(0.0f), Q12(1.0f));
            target->params.lightMode          = envSettings0->params.lightMode;
            target->params.spotLightIntensity = Math_WeightedAverageGet(envSettings0->params.spotLightIntensity, 0, weight2);
        }
        else
        {
            weight2                           = (weight - Q12(5.0f / 6.0f)) * 6;
            weight2                           = CLAMP(weight2, Q12(0.0f), Q12(1.0f));
            target->params.lightMode          = envSettings1->params.lightMode;
            weight0                           = envSettings1->params.spotLightIntensity;
            target->params.spotLightIntensity = Math_WeightedAverageGet(Q12(0.0f), weight0, weight2);
        }
    }
    else if (envSettings0->params.lightMode == WorldEnvLightMode_Directional && envSettings1->params.lightMode == WorldEnvLightMode_Spot)
    {
        if (weight < Q12(1.0f / 6.0f))
        {
            weight2                                = weight * 6;
            weight2                                = CLAMP(weight2, Q12(0.0f), Q12(1.0f));
            target->params.lightMode  = envSettings0->params.lightMode;
            target->params.spotLightIntensity = Math_WeightedAverageGet(envSettings0->params.spotLightIntensity, Q12(0.0f), weight2);
        }
        else
        {
            weight2                           = Q12_MULT(weight - Q12(1.0f / 6.0f), Q12(1.2f));
            weight2                           = CLAMP(weight2, Q12(0.0f), Q12(1.0f));
            target->params.lightMode          = envSettings1->params.lightMode;
            target->params.spotLightIntensity = Math_WeightedAverageGet(Q12(0.0f), envSettings1->params.spotLightIntensity, weight2);
        }
    }
    else
    {
        if (envSettings0->params.lightMode != WorldEnvLightMode_None && envSettings1->params.lightMode == WorldEnvLightMode_None)
        {
            if (weight >= Q12(1.0f))
            {
                target->params.lightMode = envSettings1->params.lightMode;
            }
            else
            {
                target->params.lightMode = envSettings0->params.lightMode;
            }
        }
        else
        {
            target->params.lightMode = envSettings1->params.lightMode;
        }

        target->params.spotLightIntensity = Math_WeightedAverageGet(envSettings0->params.spotLightIntensity, envSettings1->params.spotLightIntensity, weight);
    }

    if (envSettings0->params.enableTintLightOverlap == false && envSettings1->params.enableTintLightOverlap != false)
    {
        func_8003FE04(&target->params, &envSettings0->params, &envSettings1->params, weight1);
    }
    else
    {
        func_8003FE04(&target->params, &envSettings0->params, &envSettings1->params, weight);
    }
}

/**
 * Scratch: https://decomp.me/scratch/wk8iD
 */
static void func_8003FCB0(s_WorldEnvPreset* target, const s_WorldEnvPreset* envPreset0, const s_WorldEnvPreset* envPreset1, q19_12 alphaTo)
{
    q19_12 alphaFrom;

    alphaFrom = Q12(1.0f) - alphaTo;
    LoadAverageCol(&envPreset0->snowColor.r, &envPreset1->snowColor.r, alphaFrom, alphaTo, &target->snowColor.r);
    LoadAverageCol(&envPreset0->rainColor.r, &envPreset1->rainColor.r, alphaFrom, alphaTo, &target->rainColor.r);
}

/**
 * Scratch: https://decomp.me/scratch/jhfrd
 */
static void func_8003FD38(s_WorldEnvState* target, const s_WorldEnvState* envSettings0, const s_WorldEnvState* envSettings1, q19_12 weight0, q19_12 weight1, q19_12 alphaTo)
{
    if (envSettings0->brightnessIntensity != envSettings1->brightnessIntensity)
    {
        target->brightnessIntensity = Math_WeightedAverageGet(envSettings0->brightnessIntensity, envSettings1->brightnessIntensity, weight0);
    }
    else
    {
        target->brightnessIntensity = envSettings1->brightnessIntensity;
    }

    target->fogDistance                = Math_WeightedAverageGet(envSettings0->fogDistance, envSettings1->fogDistance, weight0);
    target->params.fogDistance         = Math_WeightedAverageGet(envSettings0->params.fogDistance, envSettings1->params.fogDistance, weight1);
    target->params.worldLightIntensity = Math_WeightedAverageGet(envSettings0->params.worldLightIntensity, envSettings1->params.worldLightIntensity, weight0);

    LoadAverageCol(&envSettings0->params.fogColor.r, &envSettings1->params.fogColor.r, Q12(1.0f) - alphaTo, alphaTo, &target->params.fogColor.r);
}

void func_8003FE04(const s_WorldEnvPreset* preset0, const s_WorldEnvPreset* preset1, const s_WorldEnvPreset* preset2, q19_12 alphaTo) // 0x8003FE04
{
    q19_12 alphaFrom;

    alphaFrom = Q12(1.0f) - alphaTo;
    LoadAverageCol(&preset1->pointLightTint.r, &preset2->pointLightTint.r, alphaFrom, alphaTo, &preset0->pointLightTint.r);
    LoadAverageCol(&preset1->worldTint.r, &preset2->worldTint.r, alphaFrom, alphaTo, &preset0->worldTint.r);

    if ((preset0->pointLightTint.r || preset0->pointLightTint.g || preset0->pointLightTint.b) ||
        (preset0->worldTint.r || preset0->worldTint.g || preset0->worldTint.b))
    {
        preset0->enableTintLightOverlap = true;
    }
    else
    {
        preset0->enableTintLightOverlap = false;
    }
}

s32 func_8003FEC0(const s_WorldEnvPreset* preset) // 0x8003FEC0
{
    static q19_12 Y_ARRAY[5] = {
        Q12(1.75f),
        Q12(6.0f),
        Q12(9.5f),
        Q12(12.5f),
        Q12(15.0f)
    };

    if (g_WorldEnvRenderWork.isFogEnabled)
    {
        return preset->fogDistance;
    }

    if (g_WorldEnvRenderWork.lightMode == WorldEnvLightMode_Spot)
    {
        return vwOresenHokan(Y_ARRAY, ARRAY_SIZE(Y_ARRAY), preset->spotLightIntensity, 0, Q12(2.0f));
    }

    return Q12(20.0f);
}

void WorldEnv_Apply(s_WorldEnvState* state) // 0x8003FF2C
{
    s32   fogDistCpy;
    s32   temp_v1;
    q23_8 brightness;

    temp_v1    = Q12_MULT(state->brightnessIntensity, (g_GameWork.config.brightness * 8) + 4);
    brightness = CLAMP(temp_v1, Q8_CLAMPED(0.0f), Q8_CLAMPED(1.0f));

    WorldEnvRender_WorldLightingParamSet(state->params.lightMode, state->params.worldLightIntensity, state->params.lightLevelMax, state->params.worldTintR, state->params.worldTintG, state->params.worldTintB, brightness);
    WorldEnvRender_FogParamsSet(state->params.fogMode != WorldEnvFogMode_Off, state->params.fogColor.r, state->params.fogColor.g, state->params.fogColor.b);

    fogDistCpy = state->params.fogDistance;

    WorldEnvRender_FogDistanceSet(fogDistCpy, fogDistCpy + Q12(1.0f));
    WorldEnvRender_WorldLightTintSet(state->params.enableTintLightOverlap, state->params.pointLightTint.r, state->params.pointLightTint.g, state->params.pointLightTint.b, state->params.worldTint.r, state->params.worldTint.g, state->params.worldTint.b);
}
