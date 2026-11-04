#ifndef _BODYPROG_DMS_H
#define _BODYPROG_DMS_H

/** @brief DMS cutscene segment states. */
typedef enum _DmsSegmentState
{
    DmsSegmentState_Interpolating = 0,
    DmsSegmentState_SingleFrame   = 1,
    DmsSegmentState_Ending        = 2
} e_DmsSegmentState;

/** @brief DMS cutscene character transform keyframe. */
typedef struct _DmsKeyframeCharacter
{
    /* 0x0 */ SVECTOR3 position; /** Q7.8 */
    /* 0x6 */ SVECTOR3 rotation; /** Q3.12 */
} s_DmsKeyframeCharacter;
STATIC_ASSERT_SIZEOF(s_DmsKeyframeCharacter, 12);

/** @brief DMS cutscene camera keyframe. */
typedef struct _DmsKeyframeCamera
{
    /* 0x0 */ SVECTOR3 positionTarget;     /** Q7.8 */
    /* 0x6 */ SVECTOR3 lookAtTarget;       /** Q7.8 */
    /* 0xC */ q3_12    unusedAngle;        /** @unused Unknown angle, lerped between keyframes. */
    /* 0xE */ q3_12    projectionDistance; // TODO: Passed to `vcChangeProjectionValue`. Might be FOV related?
} s_DmsKeyframeCamera;
STATIC_ASSERT_SIZEOF(s_DmsKeyframeCamera, 16);

/** @brief Maps the inclusive playback frame range `[startFrameIdx, endFrameIdx]` to a single keyframe.
 * Used for playback frames where a character or camera has no movement and can remain set to a single keyframe.
 */
typedef struct _DmsHoldRange
{
    /* 0x0 */ s16 startFrameIdx; /** Playback frame index. */
    /* 0x2 */ s16 endFrameIdx;   /** Playback frame index. */
    /* 0x4 */ s16 keyframeIdx;   /** DMS entry keyframe index. */
} s_DmsHoldRange;
STATIC_ASSERT_SIZEOF(s_DmsHoldRange, 6);

/** @brief DMS cutscene character or camera entry. */
typedef struct _DmsEntry
{
    /* 0x0 */ s16             keyframeCount;  /** `keyframes` array size. */
    /* 0x2 */ u8              holdRangeCount; /** `holdRanges` array size. */
    /* 0x3 */ u8              __pad_3;        /** Usually 0, sometimes filled in. Likely junk data. */
    /* 0x4 */ char            name[4];        // First 4 `char`s of the name. E.g. if code checks for "DAHLIA", file is "DAHL".
    /* 0x8 */ s_DmsHoldRange* holdRanges;     /** Ranges of playback frames that map to a single keyframe, compressing repeated data. */
              union
              {
                  s_DmsKeyframeCharacter* character;
                  s_DmsKeyframeCamera*    camera;
    /* 0xC */ } keyframes;
} s_DmsEntry;
STATIC_ASSERT_SIZEOF(s_DmsEntry, 16);

/** @brief DMS cutscene segment. */
typedef struct _DmsSegment
{
    /* 0x0 */ s16 startFrameIdx; /** Playback frame index. */
    /* 0x2 */ s16 frameCount;    /** Playback frame duration at 30 FPS. */
} s_DmsSegment;
STATIC_ASSERT_SIZEOF(s_DmsSegment, 4);

/** @brief DMS cutscene header. */
typedef struct _DmsHeader
{
    /* 0x0  */ u8            isLoaded;            /** `bool` */
    /* 0x1  */ u8            characterEntryCount; /** `characters` array size. */
    /* 0x2  */ u8            segmentCount;        /** `segments` array size. */
    /* 0x3  */ u8            unused_3;            /** @unused Usually 0, but sometimes filled in. Possibly junk padding data. */
    /* 0x4  */ u32           unused_4;            /** @unused Unknown, correlates with DMS file size. */
    /* 0x8  */ s_DmsSegment* segments;
    /* 0xC  */ VECTOR3       origin;              /** Q23.8 | Cutscene origin added to relative character or camera positions. */
    /* 0x18 */ s_DmsEntry*   characterEntries;
    /* 0x1C */ s_DmsEntry    cameraEntry;
} s_DmsHeader;
STATIC_ASSERT_SIZEOF(s_DmsHeader, 44);

/** @brief Adjusts pointer addresses in a DMS header.
 *
 * @param dmsHdr DMS header to adjust.
 */
void Dms_HeaderFixOffsets(s_DmsHeader* dmsHdr);

/** @brief Adjusts pointer addresses in a DMS entry.
 *
 * @param entry DMS entry to adjust.
 * @param dmsHdr DMS header for the base address.
 */
void Dms_EntryFixOffsets(s_DmsEntry* entry, const s_DmsHeader* dmsHdr);

/** @brief @unused Gets a segment from a DMS header.
 *
 * @note `volatile` needed for match.
 *
 * @param unused @unused
 * @param segmentIdx Segment index.
 * @param dmsHeader DMS header from which to get a segment.
 * @return DMS segment.
 */
s_DmsSegment* Dms_SegmentGet(volatile s32 unused, s32 segmentIdx, s_DmsHeader* dmsHdr);

/** @brief Gets a character transformation at a given playback time from a DMS header by entry name.
 *
 * @param pos Output character position.
 * @param rot Output character rotation.
 * @param charaName DMS character entry name.
 * @param time Playback time.
 * @param dmsHdr DMS header.
 */
void Dms_CharacterTransformGet(VECTOR3* pos, SVECTOR3* rot, const char* charaName, q19_12 time, s_DmsHeader* dmsHdr);

/** @brief Gets a DMS character entry index by name.
 *
 * @param charaName Name of the DMS character entry to find.
 * @param dmsHdr DMS header.
 * @return DMS character entry index.
 */
s32 Dms_CharacterGetIdxByName(char* charaName, s_DmsHeader* dmsHdr);

/** @brief Gets a character transformation at a given playback time from a DMS header by entry index.
 *
 * @param pos Output character position.
 * @param rot Output character rotation.
 * @param charaIdx DMS character entry index.
 * @param time Playback time.
 * @param dmsHdr DMS header.
 */
void Dms_CharacterTransformGetByIdx(VECTOR3* pos, SVECTOR3* rot, s32 charaIdx, q19_12 time, const s_DmsHeader* dmsHdr);

/** @brief Linearly interpolates between two DMS character keyframes.
 *
 * @param result Interpolation result.
 * @param prevKeyframe Previous character keyframe.
 * @param nextKeyframe Next character keyframe.
 * @param alpha Interpolation alpha.
 */
void Dms_CharacterKeyframeInterpolate(s_DmsKeyframeCharacter* result,
                                      const s_DmsKeyframeCharacter* prevKeyframe, const s_DmsKeyframeCharacter* nextKeyframe,
                                      q19_12 alpha);

/** @brief @unused Computes an FOV scale.
 *
 * @param fovAngle FOV angle.
 * @return FOV scale.
 */
q3_12 Dms_FovScaleGet(q3_12 fovAngle);

/** @brief Gets the camera position and look-at targets from a DMS header.
 *
 * @param posTarget Output camera position target.
 * @param lookAtTarget Output camera look-at target.
 * @param unusedAngle @unused
 * @param time Playback time.
 * @param dmsHdr DMS header.
 * @return Projection distance.
 */
q19_12 Dms_CameraTargetsGet(VECTOR3* posTarget, VECTOR3* lookAtTarget, q3_12* unusedAngle, q19_12 time,
                            const s_DmsHeader* dmsHdr);

/** @brief @unused Checks if any axis between two rotations differs by more than 22.5 degrees (1/16th of a full
 * rotation).
 *
 * @param rot0 First rotation.
 * @param rot1 Second rotation.
 * @return `true` if the rotations differ by the epsilon, `false` otherwise.
 */
bool Dms_RotationsCompare(const SVECTOR3* rot0, const SVECTOR3* rot1);

/** @brief Linearly interpolates between two DMS camera keyframes.
 *
 * @param result Interpolation result.
 * @param prevKeyframe Previous camera keyframe.
 * @param nextKeyframe Next camera keyframe.
 * @param alpha Interpolation alpha.
 * @return Projection distance.
 */
q19_12 Dms_CameraKeyframeLerp(s_DmsKeyframeCamera* result,
                              const s_DmsKeyframeCamera* prevKeyframe, const s_DmsKeyframeCamera* nextKeyframe,
                              q19_12 alpha);

/** @brief Gets keyframe interpolation data at a given playback time from a given DMS camera entry.
 *
 * @param prevKeyframeIdx Output previous keyframe index.
 * @param nextKeyframeIdx Output next keyframe index.
 * @param alpha Output interpolation alpha.
 * @param time Playback time.
 * @param camEntry DMS camera entry.
 * @param dmsHdr DMS header.
 */
void Dms_KeyframeInterpGet(s32* prevKeyframeIdx, s32* nextKeyframeIdx, q19_12* alpha, q19_12 time,
                           const s_DmsEntry* camEntry, const s_DmsHeader* dmsHdr);

/** @brief Gets the state of a segment at a given playback time from a DMS header.
 *
 * @param time Playback time.
 * @param dmsHdr DMS header.
 * @return DMS segment state (`e_DmsSegmentState`).
 */
u32 Dms_SegmentStateGet(q19_12 time, const s_DmsHeader* dmsHdr);

/** @brief Gets a keyframe index at a given playback frame from a DMS entry.
 *
 * @param frameIdx Playback frame index.
 * @param entry DMS entry.
 */
s32 Dms_KeyframeIdxGet(s32 frameIdx, const s_DmsEntry* entry);

/** @brief Linearly interpolates between two angles.
 *
 * @param angleFrom Angle from.
 * @param angleTo Angle to.
 * @param alpha Interpolation alpha.
 */
q19_12 Dms_AngleLerp(q3_12 angleFrom, q3_12 angleTo, q19_12 alpha);

#endif
