#ifndef _BODYPROG_CHARA_ANIM_H
#define _BODYPROG_CHARA_ANIM_H

/** @brief Character animation flags. */
typedef enum _AnimFlags
{
    AnimFlag_None     = 0,
    AnimFlag_Unlocked = 1 << 0,
    AnimFlag_Visible  = 1 << 1
} e_AnimFlags;

/** @brief Animation playback states. Returned by `Chara_AnimPlaybackStateGet`. */
typedef enum _AnimPlaybackState
{
    AnimPlaybackState_Blend   = -2,
    AnimPlaybackState_Invalid = -1, // Unsure.
    AnimPlaybackState_Active  = 0,
    AnimPlaybackState_End     = 1
} e_AnimPlaybackState;

/** @brief Constant character animation info passed to `Anim_Update` functions. */
typedef struct _AnimInfo
{
    /* 0x0 */ void (*playbackFunc)(struct _Model* model, struct _AnmHeader* anmHdr, GsCOORDINATE2* boneCoords, struct _AnimInfo* animInfo);
    /* 0x4 */ u8 status;                        /** Packed anim status. Init base? See `s_ModelAnimData::status`. */
    /* 0x5 */ s8 hasVariableDuration;           /** `bool` | Use `duration.variableFunc`: `true`, Use `duration.constant`: `false`. */
    /* 0x6 */ u8 linkStatus;                    /** Packed anim status link target. See `s_ModelAnim::status`. */
              // 1 byte of padding.
              union
              {
                  q19_12 constant;              /** Constant duration at 30 FPS. */
                  q19_12 (*variableFunc)(void); /** Variable duration at 30 FPS via a function. Allows animations to be sped up or slowed down. */
    /* 0x8 */ } duration;
    /* 0xC */ s16 startKeyframeIdx;             /** Start keyframe index. Sometimes `NO_VALUE`, unknown why. */
    /* 0xE */ s16 endKeyframeIdx;               /** End keyframe index. */
} s_AnimInfo;

/** @brief Packs an animation status containing an animation index and active flag.
 * `isActive` is `false` in the blend phase and `true` in the playback phase.
 *
 * @param animIdx Animation index.
 * @param isActive Active status. (`bool`).
 * @return Packed animation status containing the animation index and active flag.
 */
#define ANIM_STATUS(animIdx, isActive) \
    (((animIdx) << 1) | ((isActive) ? 1 : 0))

/** @brief Retrieves the animation index from a packed animation status.
 *
 * @param animStatus Packed animation status containing an animation index and active flag.
 * @return Anim index.
 */
#define ANIM_STATUS_IDX_GET(animStatus) \
    ((animStatus) >> 1)

/** @brief Checks if an animation is active.
 *
 * @param animStatus Packed animation status containing an animation index and active flag.
 * @return `true` if active, `false` otherwise.
 */
#define ANIM_STATUS_IS_ACTIVE(animStatus) \
    ((animStatus) & (1 << 0))

/** @brief Checks if an animation time is within the keyframe index range `[low, high]`.
 *
 * @param animTime Animation time to check (Q*.12).
 * @param low Low keyframe index.
 * @param high High keyframe index.
 * @return `true` if the animation time is within the keyframe index range, `false` otherwise.
 */
#define ANIM_TIME_RANGE_CHECK(animTime, low, high) \
    (FP_FROM(animTime, Q12_SHIFT) >= (low) && FP_FROM(animTime, Q12_SHIFT) <= (high))

/** @brief Gets the relative keyframe index of an animation time with a base offset.
 *
 * @param animTime Animation time (Q*.12).
 * @param baseOffset Base keyframe index offset.
 * @return Relative keyframe index.
 */
#define ANIM_TIME_REL_KEYFRAME_IDX_GET(animTime, baseOffset) \
    (FP_FROM(animTime, Q12_SHIFT) - (baseOffset))

// TODO: Move decls from `bodyprog.h`.

#endif
