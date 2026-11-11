#include "game.h"

#include "bodyprog/bodyprog.h"
#include "bodyprog/sound/sfx.h"
#include "bodyprog/sound/sound_system.h"

/** @brief Lookup table for `2^(-i / 32)` in Q12, i = 0..32.
 * Used by `Math_Pow2Neg` for exponential decay via table interpolation.
 *
 * @bug Entry [1] (0.9876) deviates from the expected value of ~0.9785.
 * Possibly a data entry error (switched 7 and 8).
 * Others match closely to `2^(-i / 32)`
 */
q3_12 g_Pow2NegFracTable[] = {
    Q12(1.0f),    Q12(0.9876f), Q12(0.9576f), Q12(0.9371f),
    Q12(0.917f),  Q12(0.8973f), Q12(0.878f),  Q12(0.8592f),
    Q12(0.8406f), Q12(0.8228f), Q12(0.8052f), Q12(0.788f),
    Q12(0.771f),  Q12(0.7544f), Q12(0.7383f), Q12(0.7225f),
    Q12(0.7071f), Q12(0.692f),  Q12(0.6771f), Q12(0.6626f),
    Q12(0.6482f), Q12(0.6343f), Q12(0.621f),  Q12(0.6075f),
    Q12(0.5945f), Q12(0.582f),  Q12(0.5694f), Q12(0.5572f),
    Q12(0.5452f), Q12(0.5335f), Q12(0.522f),  Q12(0.5108f),
    Q12(0.5f)
};

static VECTOR3  g_Sfx_CameraPosition; // Q19.12
static VECTOR3* g_Sfx_PlayerPosition; // Q19.12

q19_12 Math_Pow2Neg(q19_12 x) // 0x8005D86C
{
    q19_12 fracPart;
    s32    intPart;
    q19_12 alpha;
    s32    idx;
    s32    tableVal;
    s32    nextTableVal;

    fracPart = Q12_FRACT(x);
    intPart  = FP_FROM(x, Q12_SHIFT);

    if (intPart >= 12)
    {
        return 0;
    }
    if (intPart < -20)
    {
        return INT_MAX;
    }

    alpha = (x & 0x7F) << 5;
    idx = fracPart >> 7;

    tableVal = g_Pow2NegFracTable[idx];
    if (intPart > 0)
    {
        tableVal >>= intPart;
    }
    else if (intPart < 0)
    {
        tableVal <<= -intPart;
    }

    if (alpha != 0)
    {
        nextTableVal = g_Pow2NegFracTable[idx + 1];
        if (intPart > 0)
        {
            nextTableVal >>= intPart;
        }
        else if (intPart < 0)
        {
            nextTableVal <<= -intPart;
        }

        tableVal = Q12_MULT_PRECISE(tableVal, Q12(1.0f) - alpha) + Q12_MULT_PRECISE(nextTableVal, alpha);
    }

    return tableVal;
}

q19_12 Math_Pow2NegClamped(q19_12 x) // 0x8005D974
{
    q19_12 val;

    val = Math_Pow2Neg(x);
    if (val > Q12(4.0f))
    {
        val = Q12(4.0f);
    }
    else if (val < Q12(0.0f))
    {
        val = Q12(0.0f);
    }

    return val;
}

q23_8 Sfx_DistanceAttenuatedVolumeGet(const VECTOR3* pos, q23_8 vol) // 0x8005D9B8
{
    q19_12 offsetX;
    q19_12 offsetY;
    q19_12 offsetZ;
    q19_12 camDistAtten; // Camera-to-player distance attenuation
    q19_12 srcDistAtten; // Source-to-player distance attenuation
    q23_8  adjVol;

    vwGetViewPosition(&g_Sfx_CameraPosition);
    g_Sfx_PlayerPosition = &PLAYER_CHARA().position;

    offsetX      = g_Sfx_CameraPosition.vx - g_Sfx_PlayerPosition->vx;
    offsetY      = g_Sfx_CameraPosition.vy - g_Sfx_PlayerPosition->vy;
    offsetZ      = g_Sfx_CameraPosition.vz - g_Sfx_PlayerPosition->vz;
    camDistAtten = Math_Pow2NegClamped((SquareRoot12(Q12_SQUARE_PRECISE(offsetX) +
                                                     Q12_SQUARE_PRECISE(offsetY) +
                                                     Q12_SQUARE_PRECISE(offsetZ)) - Q12(2.5f)) / 10);
    if (camDistAtten > Q12(1.0f))
    {
        camDistAtten = Q12(1.0f);
    }

    offsetX      = g_Sfx_PlayerPosition->vx - pos->vx;
    offsetY      = g_Sfx_PlayerPosition->vy - pos->vy;
    offsetZ      = g_Sfx_PlayerPosition->vz - pos->vz;
    srcDistAtten = Math_Pow2NegClamped((SquareRoot12(Q12_SQUARE_PRECISE(offsetX) +
                                                     Q12_SQUARE_PRECISE(offsetY) +
                                                     Q12_SQUARE_PRECISE(offsetZ)) - Q12(6.0f)) / 4);

    adjVol = Q12_MULT_PRECISE(camDistAtten, srcDistAtten);
    if (adjVol > Q8(32.0f))
    {
        adjVol = Q8(32.0f);
    }
    else if (adjVol < Q8(0.0f))
    {
        adjVol = Q8(0.0f);
    }

    adjVol = Q12_MULT_PRECISE(vol, adjVol);
    if (adjVol > Q8_CLAMPED(1.0f))
    {
        adjVol = Q8_CLAMPED(1.0f);
    }
    else if (adjVol < Q8(0.0f))
    {
        adjVol = Q8(0.0f);
    }

    return adjVol;
}

void Sfx_WithFlagsPlay(e_SfxId sfxId, const VECTOR3* pos, q23_8 vol, s32 sfxFlags) // 0x8005DC1C
{
    Sfx_WithFlagsAndPitchPlay(sfxId, pos, vol, sfxFlags, 0);
}

void Sfx_WithFlagsAndPitchPlay(e_SfxId sfxId, const VECTOR3* pos, q23_8 vol, s32 sfxFlags, s32 pitch) // 0x8005DC3C
{
    q23_8 adjVol;
    q24_7 balance;

    // Get stereo balance.
    if (sfxFlags & SfxFlag_Mono || g_GameWork.config.soundType)
    {
        balance = Q7(0.0f);
    }
    else
    {
        balance = Vc_StereoBalanceGet(pos);
    }

    // Clamp volume.
    if (vol > Q8_CLAMPED(1.0f))
    {
        vol = Q8_CLAMPED(1.0f);
    }
    else if (vol < Q8_CLAMPED(0.0f))
    {
        vol = Q8_CLAMPED(0.0f);
    }

    if (sfxFlags & SfxFlag_NoDistAtten)
    {
        adjVol = vol;
    }
    else
    {
        adjVol = Sfx_DistanceAttenuatedVolumeGet(pos, vol);
    }

    if (adjVol > Q8_CLAMPED(1.0f))
    {
        adjVol = Q8_CLAMPED(1.0f);
    }

    // TODO: `~*` converts volumes?
    if (sfxFlags & SfxFlag_UpdateAttribs)
    {
        Sd_SfxAttributesUpdate(sfxId, balance, ~adjVol, pitch);
    }
    else
    {
        Sd_SfxPlay(sfxId, balance, ~adjVol);
    }
}

void Sfx_WithPitchPlay(e_SfxId sfxId, const VECTOR3* pos, q23_8 vol, s8 pitch) // 0x8005DD44
{
    q23_8 volCpy;
    s32   balance;

    // Get stereo balance.
    if (g_GameWork.config.soundType)
    {
        balance = 0;
    }
    else
    {
        balance = Vc_StereoBalanceGet(pos);
    }

    // Clamp volume.
    if (vol > Q8_CLAMPED(1.0f))
    {
        vol = Q8_CLAMPED(1.0f);
    }
    else if (vol < Q8_CLAMPED(0.0f))
    {
        vol = Q8_CLAMPED(0.0f);
    }

    volCpy = Sfx_DistanceAttenuatedVolumeGet(pos, vol);
    if (volCpy > Q8_CLAMPED(1.0f))
    {
        volCpy = Q8_CLAMPED(1.0f);
    }

    Sd_SfxWithPitchPlay(sfxId, balance, ~volCpy, pitch);
}

static inline q23_8 AttenuationCalc(s32 vol, VECTOR3* pos, q19_12 falloff)
{
    q19_12 dist;

    dist = Math_Vector3MagCalcSafe(PLAYER_CHARA().position.vx - pos->vx,
                                   PLAYER_CHARA().position.vy - pos->vy,
                                   PLAYER_CHARA().position.vz - pos->vz);
    return (vol * dist) / falloff;
}

void Sfx_WithFalloffAndPitchPlay(e_SfxId sfxId, VECTOR3* pos, s32 vol, q19_12 falloff, s8 pitch)
{
    s32 balance;
    u16 adjVol;
    s32 unkVol;
    s32 atten0;
    u8  atten1;
    s32 atten2;

    if (g_GameWork.config.soundType)
    {
        balance = 0;
    }
    else
    {
        balance = Vc_StereoBalanceGet(pos);
    }

    if (vol > Q8_CLAMPED(1.0f))
    {
        vol = Q8_CLAMPED(1.0f);
    }

    if (vol < Q8(0.0f))
    {
        return;
    }

    atten0 = AttenuationCalc(vol, pos, falloff);
    unkVol = vol - Q8_CLAMPED(1.0f);
    if ((atten0 - unkVol) >= Q8_CLAMPED(1.0f) || (AttenuationCalc(vol, pos, falloff) - unkVol) >= 0)
    {
        atten2 = AttenuationCalc(vol, pos, falloff) - unkVol;
        adjVol = Q8_CLAMPED(1.0f);
        if (atten2 < Q8_CLAMPED(1.0f))
        {
            atten1 = AttenuationCalc(vol, pos, falloff) - (vol + 1);
            adjVol = atten1;
        }
    }
    else
    {
        adjVol = Q8(0.0f);
    }

    Sd_SfxAttributesUpdate(sfxId, balance, adjVol, pitch);
}
