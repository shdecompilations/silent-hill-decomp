#ifndef _BODYPROG_SOUND_SFX_H
#define _BODYPROG_SOUND_SFX_H

#include "bodyprog/sound/sfx_id_enum.h"

/** @brief SFX flags. */
typedef enum _SfxFlags
{
    SfxFlag_None          = 0,
    SfxFlag_Mono          = 1 << 0, /** Monophonic. */
    SfxFlag_NoDistAtten   = 1 << 1, /** No distance attenuation. */
    SfxFlag_UpdateAttribs = 1 << 2  /** @unused Update attributes. */
} e_SfxFlags;

/** @brief SFX pair indices. Used for `SFX_PAIRS`. */
typedef enum _SfxPairIdx
{
    SfxPairIdx_0  = 0,
    SfxPairIdx_1  = 1,
    SfxPairIdx_2  = 2,
    SfxPairIdx_3  = 3,
    SfxPairIdx_4  = 4,
    SfxPairIdx_5  = 5,
    SfxPairIdx_6  = 6,
    SfxPairIdx_7  = 7,
    SfxPairIdx_8  = 8,
    SfxPairIdx_9  = 9,
    SfxPairIdx_10 = 10,
    SfxPairIdx_11 = 11,
    SfxPairIdx_12 = 12,
    SfxPairIdx_13 = 13,
    SfxPairIdx_14 = 14,
    SfxPairIdx_15 = 15,
    SfxPairIdx_16 = 16,
    SfxPairIdx_17 = 17,
    SfxPairIdx_18 = 18,
    SfxPairIdx_19 = 19,
    SfxPairIdx_20 = 20,
    SfxPairIdx_21 = 21,
    SfxPairIdx_22 = 22,
    SfxPairIdx_23 = 23,
    SfxPairIdx_24 = 24
} e_SfxPairIdx;

/** @brief Computes 2^(-x) using table interpolation.
 *
 * @param x Input value in Q19.12.
 * @return 2^(-x) in Q12. Returns 0 if x >= 12, INT_MAX if x < -20.
 */
q19_12 Math_Pow2Neg(q19_12 x);

/** @brief Computes 2^(-x) clamped to [0, Q12(4.0)].
 *
 * @param x Input value in Q19.12.
 * @return 2^(-x) in Q12, clamped.
 */
q19_12 Math_Pow2NegClamped(q19_12 x);

/** @brief Computes the distance-attenuated SFX volume from a source position.
 *
 * @param pos SFX position.
 * @param vol SFX volume to attenuate.
 * @return Distance-attenuated SFX volume.
 */
q23_8 Sfx_DistanceAttenuatedVolumeGet(const VECTOR3* pos, q23_8 vol);

/** @brief Plays SFX with flags.
 *
 * @param sfxId ID of the SFX to play.
 * @param pos SFX position.
 * @param vol SFX volume.
 * @param sfxFlags SFX flags.
 */
void Sfx_WithFlagsPlay(e_SfxId sfxId, const VECTOR3* pos, q23_8 vol, s32 sfxFlags);

/** @brief Plays SFX with flags and a pitch.
 *
 * @param sfxId ID of the SFX to play.
 * @param pos SFX position.
 * @param vol SFX volume.
 * @param sfxFlags SFX flags.
 * @param pitch SFX pitch.
 */
void Sfx_WithFlagsAndPitchPlay(e_SfxId sfxId, const VECTOR3* pos, q23_8 vol, s32 sfxFlags, s32 pitch);

/** @brief Plays SFX with a pitch.
 *
 * @param sfxId ID of the SFX to play.
 * @param pos SFX position.
 * @param vol SFX volume.
 * @param pitch SFX pitch.
 */
void Sfx_WithPitchPlay(e_SfxId sfxId, const VECTOR3* pos, q23_8 vol, s8 pitch);

/** @brief Plays SFX with a falloff and pitch.
 *
 * @param sfxId ID of the SFX to play.
 * @param pos SFX position.
 * @param vol SFX volume.
 * @param sfxFlags SFX falloff.
 * @param pitch SFX pitch.
 */
void Sfx_WithFalloffAndPitchPlay(e_SfxId sfxId, VECTOR3* pos, s32 vol, q19_12 falloff, s8 pitch);

#endif
