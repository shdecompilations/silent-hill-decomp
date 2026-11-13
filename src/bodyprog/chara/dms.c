#include "bodyprog/game.h"
#include "inline_no_dmpsx.h"

#include <psyq/libpad.h>
#include <psyq/strings.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/chara/dms.h"
#include "bodyprog/formats/dms.h"
#include "bodyprog/items/item_screens.h"
#include "bodyprog/math/math.h"
#include "bodyprog/screen/draw.h"
#include "bodyprog/sound/sd_call.h"
#include "main/fsqueue.h"

void Dms_HeaderFixOffsets(s_DmsHeader* dmsHdr) // 0x8008C9A0
{
    s_DmsEntry* curCharaEntry;

    // Check if header is already loaded.
    if (dmsHdr->isLoaded)
    {
        return;
    }
    dmsHdr->isLoaded = true;

    // Initialize segments and character entries pointers.
    dmsHdr->segments         = PTR_OFFSET_RESOLVE(dmsHdr->segments, dmsHdr);
    dmsHdr->characterEntries = PTR_OFFSET_RESOLVE(dmsHdr->characterEntries, dmsHdr);

    // Initialize camera entry pointers.
    Dms_EntryFixOffsets(&dmsHdr->cameraEntry, dmsHdr);

    // Initialize character entry pointers.
    for (curCharaEntry = dmsHdr->characterEntries;
         curCharaEntry < &dmsHdr->characterEntries[dmsHdr->characterEntryCount];
         curCharaEntry++)
    {
        Dms_EntryFixOffsets(curCharaEntry, dmsHdr);
    }
}

void Dms_EntryFixOffsets(s_DmsEntry* entry, const s_DmsHeader* dmsHdr) // 0x8008CA44
{
    entry->keyframes.character = (u32)entry->keyframes.character + (u32)dmsHdr;
    entry->holdRanges          = (u32)entry->holdRanges          + (u32)dmsHdr;
}

s_DmsSegment* Dms_SegmentGet(volatile s32 unused, s32 segmentIdx, s_DmsHeader* dmsHdr) // 0x8008CA60
{
    return &dmsHdr->segments[segmentIdx];
}

void Dms_CharacterTransformGet(VECTOR3* pos, SVECTOR3* rot, const char* charaName, q19_12 time, s_DmsHeader* dmsHdr) // 0x8008CA74
{
    s32 charaEntryIdx;

    charaEntryIdx = Dms_CharacterGetIdxByName(charaName, dmsHdr);
    if (charaEntryIdx == NO_VALUE)
    {
        // Character not found in DMS.
        Math_Vector3Zero(pos);
        Math_SVectorZero(rot);

        Text_Debug_PositionSet(50, 90);

#if VERSION_EQUAL_OR_OLDER(PROTO_981216)
        // Code seen in 98-12-16 build.
        Text_Debug_Draw(charaName);
        Text_Debug_Draw(" doesn't exist in dms.");
#endif
    }
    else
    {
        Dms_CharacterTransformGetByIdx(pos, rot, charaEntryIdx, time, dmsHdr);
    }
}

s32 Dms_CharacterGetIdxByName(char* charaName, s_DmsHeader* dmsHdr) // 0x8008CB10
{
    s32 i;

    // Find matching name.
    for (i = 0; i < dmsHdr->characterEntryCount; i++)
    {
        if (!strncmp(charaName, dmsHdr->characterEntries[i].name, 4))
        {
            return i;
        }
    }

    return NO_VALUE;
}

void Dms_CharacterTransformGetByIdx(VECTOR3* pos, SVECTOR3* rot, s32 charaIdx, q19_12 time, const s_DmsHeader* dmsHdr) // 0x8008CB90
{
    s32                     prevKeyframeIdx;
    s32                     nextKeyframeIdx;
    q19_12                  alpha;
    s_DmsKeyframeCharacter  activeCharaKeyframe;
    s_DmsEntry*             charaEntry;
    s_DmsKeyframeCharacter* charaKeyframes;

    // Get keyframe interpolation data.
    charaEntry = &dmsHdr->characterEntries[charaIdx];
    Dms_KeyframeInterpGet(&prevKeyframeIdx, &nextKeyframeIdx, &alpha, time, charaEntry, dmsHdr);

    // Interpolate frame.
    charaKeyframes = charaEntry->keyframes.character;
    Dms_CharacterKeyframeInterpolate(&activeCharaKeyframe,
                                     &charaKeyframes[prevKeyframeIdx], &charaKeyframes[nextKeyframeIdx],
                                     alpha);

    // Set position.
    pos->vx = Q8_TO_Q12(activeCharaKeyframe.position.vx + dmsHdr->origin.vx);
    pos->vy = Q8_TO_Q12(activeCharaKeyframe.position.vy + dmsHdr->origin.vy);
    pos->vz = Q8_TO_Q12(activeCharaKeyframe.position.vz + dmsHdr->origin.vz);

    // Set rotation.
    rot->vx = activeCharaKeyframe.rotation.vx;
    rot->vy = activeCharaKeyframe.rotation.vy;
    rot->vz = activeCharaKeyframe.rotation.vz;
}

void Dms_CharacterKeyframeInterpolate(s_DmsKeyframeCharacter* result,
                                      const s_DmsKeyframeCharacter* prevKeyframe, const s_DmsKeyframeCharacter* nextKeyframe,
                                      q19_12 alpha) // 0x8008CC98
{
    // Low-precision lerp between positions.
    result->position.vx = prevKeyframe->position.vx + Q12_MULT_PRECISE(nextKeyframe->position.vx - prevKeyframe->position.vx, alpha);
    result->position.vy = prevKeyframe->position.vy + Q12_MULT_PRECISE(nextKeyframe->position.vy - prevKeyframe->position.vy, alpha);
    result->position.vz = prevKeyframe->position.vz + Q12_MULT_PRECISE(nextKeyframe->position.vz - prevKeyframe->position.vz, alpha);

    // High-precision lerp between rotations.
    result->rotation.vx = Dms_AngleLerp(prevKeyframe->rotation.vx, nextKeyframe->rotation.vx, alpha);
    result->rotation.vy = Dms_AngleLerp(prevKeyframe->rotation.vy, nextKeyframe->rotation.vy, alpha);
    result->rotation.vz = Dms_AngleLerp(prevKeyframe->rotation.vz, nextKeyframe->rotation.vz, alpha);
}

q3_12 Dms_FovScaleGet(q3_12 fovAngle) // 0x8008CDBC
{
    return (96 * Math_Cos(fovAngle / 2)) / Math_Sin(fovAngle / 2);
}

q19_12 Dms_CameraTargetsGet(VECTOR3* posTarget, VECTOR3* lookAtTarget, q3_12* unusedAngle, q19_12 time,
                            const s_DmsHeader* dmsHdr) // 0x8008CE1C
{
    s32                 prevKeyframeIdx;
    s32                 nextKeyframeIdx;
    q19_12              alpha;
    s_DmsKeyframeCamera activeCamKeyframe;
    q19_12              camProjDist;
    const s_DmsEntry*   camEntry;

    camEntry = &dmsHdr->cameraEntry;

    // Interpolate current keyframe.
    Dms_KeyframeInterpGet(&prevKeyframeIdx, &nextKeyframeIdx, &alpha, time, camEntry, dmsHdr);
    camProjDist = Dms_CameraKeyframeLerp(&activeCamKeyframe, &camEntry->keyframes.camera[prevKeyframeIdx],
                                         &camEntry->keyframes.camera[nextKeyframeIdx],
                                         alpha);

    // Set position target.
    posTarget->vx = Q8_TO_Q12(activeCamKeyframe.positionTarget.vx + dmsHdr->origin.vx);
    posTarget->vy = Q8_TO_Q12(activeCamKeyframe.positionTarget.vy + dmsHdr->origin.vy);
    posTarget->vz = Q8_TO_Q12(activeCamKeyframe.positionTarget.vz + dmsHdr->origin.vz);

    // Set look-at target.
    lookAtTarget->vx = Q8_TO_Q12(activeCamKeyframe.lookAtTarget.vx + dmsHdr->origin.vx);
    lookAtTarget->vy = Q8_TO_Q12(activeCamKeyframe.lookAtTarget.vy + dmsHdr->origin.vy);
    lookAtTarget->vz = Q8_TO_Q12(activeCamKeyframe.lookAtTarget.vz + dmsHdr->origin.vz);

    // @unused Always passed as `NULL`.
    if (unusedAngle != NULL)
    {
        *unusedAngle = activeCamKeyframe.unusedAngle;
    }

    // `camProjDist` comes from `curFrame.projectionDistance`, return value is passed to `vcChangeProjectionValue`.
    // Might be related to FOV?
    return camProjDist;
}

bool Dms_RotationsCompare(const SVECTOR3* rot0, const SVECTOR3* rot1) // 0x8008CF54
{
    #define ANGLE_EPSILON Q12_ANGLE(360.0f / 16.0f) // 22.5 degrees.

    // Check each axis.
    if (ABS(rot0->vx - rot1->vx) > ANGLE_EPSILON ||
        ABS(rot0->vy - rot1->vy) > ANGLE_EPSILON ||
        ABS(rot0->vz - rot1->vz) > ANGLE_EPSILON)
    {
        return true;
    }

    return false;
}

q19_12 Dms_CameraKeyframeLerp(s_DmsKeyframeCamera* result,
                              const s_DmsKeyframeCamera* prevKeyframe, const s_DmsKeyframeCamera* nextKeyframe,
                              q19_12 alpha) // 0x8008CFEC
{
    // Set position target.
    result->positionTarget.vx = prevKeyframe->positionTarget.vx + Q12_MULT_PRECISE(nextKeyframe->positionTarget.vx - prevKeyframe->positionTarget.vx, alpha);
    result->positionTarget.vy = prevKeyframe->positionTarget.vy + Q12_MULT_PRECISE(nextKeyframe->positionTarget.vy - prevKeyframe->positionTarget.vy, alpha);
    result->positionTarget.vz = prevKeyframe->positionTarget.vz + Q12_MULT_PRECISE(nextKeyframe->positionTarget.vz - prevKeyframe->positionTarget.vz, alpha);

    // Set look-at target.
    result->lookAtTarget.vx = prevKeyframe->lookAtTarget.vx + Q12_MULT_PRECISE(nextKeyframe->lookAtTarget.vx - prevKeyframe->lookAtTarget.vx, alpha);
    result->lookAtTarget.vy = prevKeyframe->lookAtTarget.vy + Q12_MULT_PRECISE(nextKeyframe->lookAtTarget.vy - prevKeyframe->lookAtTarget.vy, alpha);
    result->lookAtTarget.vz = prevKeyframe->lookAtTarget.vz + Q12_MULT_PRECISE(nextKeyframe->lookAtTarget.vz - prevKeyframe->lookAtTarget.vz, alpha);

    // Set unused angle and projection distance.
    result->unusedAngle        = Dms_AngleLerp(prevKeyframe->unusedAngle, nextKeyframe->unusedAngle, alpha);
    result->projectionDistance = prevKeyframe->projectionDistance + Q12_MULT_PRECISE(nextKeyframe->projectionDistance - prevKeyframe->projectionDistance, alpha);

    return result->projectionDistance;
}

void Dms_KeyframeInterpGet(s32* prevKeyframeIdx, s32* nextKeyframeIdx, q19_12* alpha, q19_12 time,
                           const s_DmsEntry* camEntry, const s_DmsHeader* dmsHdr) // 0x8008D1D0
{
    s32 prevFrameIdx;
    s32 nextFrameIdx;

    prevFrameIdx = 0;
    nextFrameIdx = 0;

    // Define playback frames and set interpolation alpha.
    switch (Dms_SegmentStateGet(time, dmsHdr))
    {
        case DmsSegmentState_Interpolating:
            prevFrameIdx = FP_FROM(time, Q12_SHIFT);
            nextFrameIdx = prevFrameIdx + 1;
            *alpha       = Q12_FRACT(time);
            break;

        case DmsSegmentState_SingleFrame:
            prevFrameIdx = FP_FROM(time, Q12_SHIFT);
            nextFrameIdx = prevFrameIdx;
            *alpha       = Q12(0.0f);
            break;

        case DmsSegmentState_Ending:
            prevFrameIdx = FP_FROM(time, Q12_SHIFT) - 1;
            nextFrameIdx = prevFrameIdx + 1;
            *alpha       = Q12_FRACT(time) + Q12(1.0f);
            break;
    }

    // Set previous and next keyframe indices.
    *prevKeyframeIdx = Dms_KeyframeIdxGet(prevFrameIdx, camEntry);
    *nextKeyframeIdx = Dms_KeyframeIdxGet(nextFrameIdx, camEntry);
}

u32 Dms_SegmentStateGet(q19_12 time, const s_DmsHeader* dmsHdr)
{
    s32                 frameIdx;
    const s_DmsSegment* curSegment;

    frameIdx = FP_FROM(time, Q12_SHIFT);

    // Run through segments.
    for (curSegment = dmsHdr->segments;
         curSegment < &dmsHdr->segments[dmsHdr->segmentCount];
         curSegment++)
    {
        // Check if at end playback frame.
        if (frameIdx != ((curSegment->startFrameIdx + curSegment->frameCount) - 1))
        {
            continue;
        }

        if (curSegment->frameCount > 1)
        {
            return DmsSegmentState_Ending;
        }

        return DmsSegmentState_SingleFrame;
    }

    return DmsSegmentState_Interpolating;
}

s32 Dms_KeyframeIdxGet(s32 frameIdx, const s_DmsEntry* entry) // 0x8008D330
{
    s32                   keyframeIdx;
    s32                   clampedKeyframeIdx;
    const s_DmsHoldRange* curHoldRange;

    keyframeIdx = frameIdx;

    // Run through ranges to freeze or offset keyframe.
    for (curHoldRange = entry->holdRanges; 
         curHoldRange < &entry->holdRanges[entry->holdRangeCount]; 
         curHoldRange++)
    {
        // Check if target playback frame falls before hold range.
        if (frameIdx < curHoldRange->startFrameIdx)
        {
            break;
        }

        // If target playback frame is within hold range, freeze at mapped keyframe.
        if (frameIdx <= curHoldRange->endFrameIdx)
        {
            keyframeIdx = curHoldRange->keyframeIdx;
            break;
        }

        // Align keyframe index.
        keyframeIdx -= curHoldRange->endFrameIdx - curHoldRange->startFrameIdx;
    }

    // Clamp keyframe index.
    clampedKeyframeIdx = CLAMP(keyframeIdx, 0, entry->keyframeCount - 1);
    return clampedKeyframeIdx;
}

q19_12 Dms_AngleLerp(q3_12 angleFrom, q3_12 angleTo, q19_12 alpha) // 0x8008D3D4
{
    return Q12_ANGLE_NORM_S((q19_12)Q12_MULT_PRECISE(Q12_ANGLE_NORM_S(angleTo - angleFrom), alpha) + angleFrom);
}
