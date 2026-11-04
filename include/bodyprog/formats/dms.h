#ifndef _BODYPROG_FORMATS_DMS_H
#define _BODYPROG_FORMATS_DMS_H

/** @brief DMS cutscene character transform keyframe. */
typedef struct _DmsKeyframeCharacter
{
    /* 0x0 */ SVECTOR3 position; /** Q7.8 */
    /* 0x6 */ SVECTOR3 rotation; /** Q3.12 */
} s_DmsKeyframeCharacter;

/** @brief DMS cutscene camera keyframe. */
typedef struct _DmsKeyframeCamera
{
    /* 0x0 */ SVECTOR3 positionTarget;     /** Q7.8 */
    /* 0x6 */ SVECTOR3 lookAtTarget;       /** Q7.8 */
    /* 0xC */ q3_12    unusedAngle;        /** @unused Unknown angle, lerped between keyframes. */
    /* 0xE */ q3_12    projectionDistance; // TODO: Passed to `vcChangeProjectionValue`. Might be FOV related?
} s_DmsKeyframeCamera;

/** @brief Maps the inclusive playback frame range `[startFrameIdx, endFrameIdx]` to a single keyframe.
 * Used for playback frames where a character or camera has no movement and can remain set to a single keyframe.
 */
typedef struct _DmsHoldRange
{
    /* 0x0 */ s16 startFrameIdx; /** Playback frame index. */
    /* 0x2 */ s16 endFrameIdx;   /** Playback frame index. */
    /* 0x4 */ s16 keyframeIdx;   /** DMS entry keyframe index. */
} s_DmsHoldRange;

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

/** @brief DMS cutscene segment. */
typedef struct _DmsSegment
{
    /* 0x0 */ s16 startFrameIdx; /** Playback frame index. */
    /* 0x2 */ s16 frameCount;    /** Playback frame duration at 30 FPS. */
} s_DmsSegment;

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

#endif
