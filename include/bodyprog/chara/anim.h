#ifndef _BODYPROG_CHARA_ANIM_H
#define _BODYPROG_CHARA_ANIM_H

struct _AnmHeader;
struct _Model;
struct _ModelAnim;
struct _SubCharacter;

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
    /* 0x4 */ u8 status;              /** Packed anim status. Init base? See `struct _ModelAnimData::status`. */
    /* 0x5 */ s8 hasVariableDuration; /** `bool` | Use `duration.variableFunc`: `true`, Use `duration.constant`: `false`. */
    /* 0x6 */ u8 linkStatus;          /** Packed anim status link target. See `struct _ModelAnim::status`. */
              // 1 byte of padding.
              union
              {
                  q19_12 constant;    /** Constant duration at 30 FPS. */
                  q19_12 (*variableFunc)(struct _Model* model); /** Variable duration at 30 FPS via a function. Allows animations to be sped up or slowed down. */
    /* 0x8 */ } duration;
    /* 0xC */ s16 startKeyframeIdx;   /** Start keyframe index. Sometimes `NO_VALUE`, unknown why. */
    /* 0xE */ s16 endKeyframeIdx;     /** End keyframe index. */
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

void Anim_BonesInit(struct _AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

void Anim_BonesUpdate(struct _AnmHeader* anmHdr, GsCOORDINATE2* boneCoords, s32 keyframe0, s32 keyframe1, q19_12 alpha);

/** @brief Gets the active animation info of a model, accounting for overflow map animations.
 *
 * @param anim Model animation data.
 */
s_AnimInfo* Anim_InfoGet(struct _ModelAnim* anim);

/** @brief Plays a character's animation using the callback function of its active animation.
 *
 * @param chara Character to update.
 * @param anmHdr Character animation data.
 * @param boneCoords Character model bone coords.
 */
void Anim_CharaUpdate(struct _SubCharacter* chara, struct _AnmHeader* anmHdr, GsCOORDINATE2* coords);

/** @brief Gets the duration of an animation. A duration can be constant or variable.
 *
 * @param model Model passed to `duration.variableFunc`.
 * @param animInfo Animation info from which to get the duration.
 * @return Animation duration.
 */
q19_12 Anim_DurationGet(struct _Model* model, s_AnimInfo* animInfo);

/** @brief Plays an animation once and clamps to the end keyframe, triggering a status transition to the linked
 * animation upon completion.
 *
 * @note Used for one-shot actions such as stopping and attacking.
 *
 * @param model Character model to animate.
 * @param anmHdr Character animation data.
 * @param boneCoords Character model bone coords.
 * @param animInfo Active character animation info.
 */
void Anim_PlayOnce(struct _Model* model, struct _AnmHeader* anmHdr, GsCOORDINATE2* boneCoords, s_AnimInfo* animInfo);

/** @brief Loops an animation continuously, wrapping the time around to the start and end keyframes without
 * triggering a status transition.
 *
 * @note Used for looped actions such as running and walking.
 *
 * @param model Character model to animate.
 * @param anmHdr Character animation data.
 * @param boneCoords Character model bone coords.
 * @param animInfo Character animation info.
 */
void Anim_PlayLoop(struct _Model* model, struct _AnmHeader* anmHdr, GsCOORDINATE2* boneCoords, s_AnimInfo* animInfo);

/** @brief Linearly interpolates between the current pose into a new target animation, triggering a status transition
 * to the linked animation when complete.
 *
 * @note Used as the entry transition for almost every new animation status.
 *
 * @param model Character model to animate.
 * @param anmHdr Character animation data.
 * @param boneCoords Character model bone coords.
 * @param animInfo Character animation info.
 */
void Anim_BlendLinear(struct _Model* model, struct _AnmHeader* anmHdr, GsCOORDINATE2* boneCoords, s_AnimInfo* animInfo);

/** @brief @unused? Smoothly blends between the current pose into a new target animation using a sine-based ease-out
 * without triggering a status transition.
 *
 * @param model Character model to animate.
 * @param anmHdr Character animation data.
 * @param boneCoords Character model bone coords.
 * @param animInfo Character animation info.
 */
void Anim_BlendEaseOut(struct _Model* model, struct _AnmHeader* anmHdr, GsCOORDINATE2* boneCoords, s_AnimInfo* animInfo);

#endif
