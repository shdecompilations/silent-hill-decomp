#ifndef _BODYPROG_CHARA_DMS_H
#define _BODYPROG_CHARA_DMS_H

#include "bodyprog/formats/dms.h"

/** @brief DMS cutscene segment states. */
typedef enum _DmsSegmentState
{
    DmsSegmentState_Interpolating = 0,
    DmsSegmentState_SingleFrame   = 1,
    DmsSegmentState_Ending        = 2
} e_DmsSegmentState;

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
