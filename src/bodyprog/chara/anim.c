#include "game.h"
#include "inline_no_dmpsx.h"

#include "bodyprog/bodyprog.h"
#include "bodyprog/chara/anim.h"
#include "bodyprog/screen/data.h"
#include "bodyprog/screen/draw.h"
#include "bodyprog/math/math.h"
#include "main/fsqueue.h"
#include "types.h"

/** @brief Computes the timestep of the target animation for the current tick.
 *
 * @param model Character model.
 * @param animInfo Animation info.
 * @return Animation timestep for the current tick.
 */
static inline q19_12 Anim_TimestepGet(s_Model* model, s_AnimInfo* animInfo)
{
    q19_12 duration;

    // Check if animation is unlocked.
    if (model->anim.flags & AnimFlag_Unlocked)
    {
        duration = Anim_DurationGet(model, animInfo);
        return Q12_MULT_PRECISE(duration, g_DeltaTime);
    }

    return Q12(0.0f);
}

void Anim_BonesInit(s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords) // 0x800445A4
{
    s32            boneIdx;
    s32            i;
    s32            j;
    s_AnmBindPose* curBindPose;
    GsCOORDINATE2* curBoneCoord;

    GsInitCoordinate2(NULL, boneCoords);

    // Run through bones.
    for (boneIdx = 1, curBindPose = &anmHdr->bindPoses[1], curBoneCoord = &boneCoords[1];
         boneIdx < anmHdr->boneCount;
         boneIdx++, curBindPose++, curBoneCoord++)
    {
        curBoneCoord->super = &boneCoords[anmHdr->bindPoses[boneIdx].parentBone];

        // If no translation for this bone, copy over `translationInitial`.
        if (curBindPose->translationDataIdx < 0)
        {
            for (i = 0; i < 3; i++)
            {
                curBoneCoord->coord.t[i] = anmHdr->bindPoses[boneIdx].translationInitial[i] << anmHdr->scaleLog2;
            }
        }

        // If no rotation for this bone, create identity matrix.
        if (curBindPose->rotationDataIdx < 0)
        {
            for (i = 0; i < 3; i++)
            {
                for (j = 0; j < 3; j++)
                {
                    curBoneCoord->coord.m[i][j] = ((j == i) ? Q12_ANGLE(360.0f) : Q12_ANGLE(0.0f));
                }
            }
        }
    }
}

void Anim_BonesUpdate(s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords, s32 keyframe0, s32 keyframe1, q19_12 alpha) // 0x800446D8
{
    s32            boneCount;
    bool           isPlayer;
    u32            activeBoneFlags;
    s32            boneIdx;
    s32            scaleLog2;
    s32            boneTranslationDataIdx;
    s32            boneRotationDataIdx;
    s32            j;
    s32            i;
    s8*            frame0TranslationData;
    s8*            frame1TranslationData;
    s8*            frame0RotationData;
    s8*            frame1RotationData;
    void*          frame0Data;
    void*          frame0RotData;
    void*          frame1Data;
    void*          frame1RotData;
    GsCOORDINATE2* curBoneCoord;
    s_AnmBindPose* curBindPose;

    boneCount     = anmHdr->boneCount;
    frame0Data    = ((u8*)anmHdr + anmHdr->dataOffset) + (anmHdr->keyframeDataSize * keyframe0);
    frame0RotData = frame0Data + (anmHdr->translationBoneCount * 3);
    frame1Data    = ((u8*)anmHdr + anmHdr->dataOffset) + (anmHdr->keyframeDataSize * keyframe1);
    frame1RotData = frame1Data + (anmHdr->translationBoneCount * 3);

    // For player, use inverted mask of `extra.disabledAnimBoneFlags` to facilitate masking of upper and lower body.
    isPlayer = boneCoords == &g_SysWork.playerBoneCoords[HarryBone_Root];
    if (isPlayer)
    {
        activeBoneFlags = ~PLAYER_EXTRA().disabledAnimBoneFlags;
    }
    else
    {
        activeBoneFlags = anmHdr->activeBoneFlags;
    }

    // Skip root bone (index 0) and start processing from bone 1.
    boneCoords  = &boneCoords[1];
    curBindPose = &anmHdr->bindPoses[1];

    for (boneIdx = 1, curBoneCoord = boneCoords;
         boneIdx < boneCount;
         boneIdx++, curBindPose++, curBoneCoord++)
    {
        // Process bones flagged active.
        if (activeBoneFlags & (1 << boneIdx))
        {
            curBoneCoord->flg = false;
            scaleLog2         = anmHdr->scaleLog2;

            boneTranslationDataIdx = curBindPose->translationDataIdx;
            if (boneTranslationDataIdx >= 0)
            {
                // 3-byte vector translation.
                frame0TranslationData = frame0Data + (boneTranslationDataIdx * 3);
                frame1TranslationData = frame1Data + (boneTranslationDataIdx * 3);

                // Interpolate XYZ translation components.
                for (i = 0; i < 3; i++)
                {
                    // Linear interpolation with scaling: `frame0 + (frame1 - frame0) * alpha`.
                    curBoneCoord->coord.t[i] = (*frame0TranslationData << scaleLog2) +
                                               (((*frame1TranslationData - *frame0TranslationData) * alpha) >> (Q12_SHIFT - scaleLog2));

                    frame0TranslationData++;
                    frame1TranslationData++;
                }

                if (boneTranslationDataIdx == 0)
                {
                    curBoneCoord->coord.t[1] -= anmHdr->rootYOffset; // TODO: Not sure of purpose of this yet.
                }
            }

            boneRotationDataIdx = curBindPose->rotationDataIdx;
            if (boneRotationDataIdx >= 0)
            {
                // 9-byte rotation matrix.
                frame0RotationData = frame0RotData + (boneRotationDataIdx * 9);
                frame1RotationData = frame1RotData + (boneRotationDataIdx * 9);

                // Interpolate rotation matrix.
                for (i = 0; i < 3; i++)
                {
                    for (j = 0; j < 3; j++)
                    {
                        curBoneCoord->coord.m[i][j] = (*frame0RotationData << 5) +
                                                   (((*frame1RotationData - *frame0RotationData) * alpha) >> 7);

                        frame0RotationData++;
                        frame1RotationData++;
                    }
                }
            }
        }
    }

    // Copy player hips translation to torso.
    if (isPlayer)
    {
        for (i = 0; i < 3; i++)
        {
            g_SysWork.playerBoneCoords[HarryBone_Torso].coord.t[i] = g_SysWork.playerBoneCoords[HarryBone_Hips].coord.t[i];
        }
    }
}

s_AnimInfo* Anim_InfoGet(s_ModelAnim* anim) // 0x80044918
{
    // `field_C`/`field_10` usually points to data at `0x800AF228` which contains funcptrs and other stuff.

    s_AnimInfo* animInfos;
    s_AnimInfo* mapAnimInfos;
    u8          animStatus;
    s32         mapAnimStatusStart;

    animInfos          = anim->baseAnimInfos;
    mapAnimInfos       = anim->mapAnimInfos;
    animStatus         = anim->status;
    mapAnimStatusStart = anim->mapAnimStatusStart;

    // Fetch from existing map anim infos if status overflows from base anim infos.
    if (mapAnimInfos != NULL && animStatus >= mapAnimStatusStart)
    {
        animInfos  = mapAnimInfos;
        animInfos -= mapAnimStatusStart;
    }

    return &animInfos[animStatus];
}

void Anim_CharaUpdate(s_SubCharacter* chara, s_AnmHeader* anmHdr, GsCOORDINATE2* coords) // 0x80044950
{
    s_AnimInfo* animInfo;

    animInfo = Anim_InfoGet(&chara->model.anim);
    animInfo->playbackFunc(&chara->model, anmHdr, coords, animInfo);
}

q19_12 Anim_DurationGet(s_Model* unused, s_AnimInfo* animInfo) // 0x800449AC
{
    if (!animInfo->hasVariableDuration)
    {
        return animInfo->duration.constant;
    }

    return animInfo->duration.variableFunc();
}

void Anim_PlayOnce(s_Model* model, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords, s_AnimInfo* animInfo) // 0x800449F0
{
    bool   setNewAnimStatus;
    q19_12 timestep;
    q19_12 newTime;
    s32    newKeyframeIdx;
    q19_12 startTime;
    q19_12 endTime;
    q19_12 alpha;

    setNewAnimStatus = false;

    // Get timestep.
    timestep = Anim_TimestepGet(model, animInfo);

    // Compute new time and keyframe index.
    newTime        = model->anim.time;
    newKeyframeIdx = FP_FROM(newTime, Q12_SHIFT);
    if (timestep != Q12(0.0f))
    {
        newTime += timestep;

        // Clamp new time to valid keyframe range.
        endTime = Q12(animInfo->endKeyframeIdx);
        if (newTime >= endTime)
        {
            newTime          = endTime;
            setNewAnimStatus = true;
        }
        else
        {
            startTime = Q12(animInfo->startKeyframeIdx);
            if (newTime <= startTime)
            {
                newTime          = startTime;
                setNewAnimStatus = true;
            }
        }

        newKeyframeIdx = FP_FROM(newTime, Q12_SHIFT);
    }

    // Update skeleton.
    alpha = Q12_FRACT(newTime);
    if ((model->anim.flags & AnimFlag_Unlocked) || (model->anim.flags & AnimFlag_Visible))
    {
        Anim_BonesUpdate(anmHdr, boneCoords, newKeyframeIdx, newKeyframeIdx + 1, alpha);
    }

    // Update frame data.
    model->anim.time        = newTime;
    model->anim.keyframeIdx = newKeyframeIdx;
    model->anim.alpha       = Q12(0.0f);

    // Link to new anim status.
    if (setNewAnimStatus)
    {
        model->anim.status = animInfo->linkStatus;
    }
}

void Anim_PlayLoop(s_Model* model, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords, s_AnimInfo* animInfo) // 0x80044B38
{
    s32    startKeyframeIdx;
    s32    endKeyframeIdx;
    s32    nextStartKeyframeIdx;
    s32    keyframeCount;
    q19_12 startTime;
    q19_12 nextStartTime;
    q19_12 duration;
    q19_12 timestep;
    q19_12 newTime;
    s32    newKeyframeIdx0;
    s32    newKeyframeIdx1;
    q19_12 alpha;

    startKeyframeIdx     = animInfo->startKeyframeIdx;
    endKeyframeIdx       = animInfo->endKeyframeIdx;
    nextStartKeyframeIdx = endKeyframeIdx + 1;
    keyframeCount        = nextStartKeyframeIdx - startKeyframeIdx;

    startTime     = Q12(startKeyframeIdx);
    nextStartTime = Q12(nextStartKeyframeIdx);
    duration      = Q12(keyframeCount);

    // Get timestep.
    timestep = Anim_TimestepGet(model, animInfo);

    // Wrap new time to valid keyframe range.
    newTime = model->anim.time + timestep;
    while (newTime < startTime)
    {
        newTime += duration;
    }
    while (newTime >= nextStartTime)
    {
        newTime -= duration;
    }

    // Compute new keyframe 1. Wrap to start to facilitate loop.
    newKeyframeIdx0 = FP_FROM(newTime, Q12_SHIFT);
    newKeyframeIdx1 = newKeyframeIdx0 + 1;
    if (newKeyframeIdx1 == nextStartKeyframeIdx)
    {
        newKeyframeIdx1 = startKeyframeIdx;
    }

    // Update skeleton.
    alpha = Q12_FRACT(newTime);
    if ((model->anim.flags & AnimFlag_Unlocked) || (model->anim.flags & AnimFlag_Visible))
    {
        Anim_BonesUpdate(anmHdr, boneCoords, newKeyframeIdx0, newKeyframeIdx1, alpha);
    }

    // Update frame data.
    model->anim.time        = newTime;
    model->anim.keyframeIdx = newKeyframeIdx0;
    model->anim.alpha       = Q12(0.0f);
}

void Anim_BlendLinear(s_Model* model, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords, s_AnimInfo* animInfo) // 0x80044CA4
{
    bool   setNewAnimStatus;
    s32    startKeyframeIdx;
    s32    endKeyframeIdx;
    q19_12 timestep;
    q19_12 alpha;

    setNewAnimStatus = false;
    startKeyframeIdx = animInfo->startKeyframeIdx;
    endKeyframeIdx   = animInfo->endKeyframeIdx;

    // If no start keyframe exists, default to active keyframe.
    if (startKeyframeIdx == NO_VALUE)
    {
        startKeyframeIdx = model->anim.keyframeIdx;
    }

    // Get timestep.
    timestep = Anim_TimestepGet(model, animInfo);

    // Update time to start or end keyframe, whichever is closest.
    alpha  = model->anim.alpha;
    alpha += timestep;
    if (alpha >= Q12(0.5f))
    {
        model->anim.time = Q12(endKeyframeIdx);
    }
    else
    {
        model->anim.time = Q12(startKeyframeIdx);
    }

    // Update frame data.
    if (alpha >= Q12(1.0f))
    {
        startKeyframeIdx        = endKeyframeIdx;
        model->anim.keyframeIdx = endKeyframeIdx;

        alpha            = Q12(0.0f);
        setNewAnimStatus = true;
    }

    // Update skeleton.
    if ((model->anim.flags & AnimFlag_Unlocked) || (model->anim.flags & AnimFlag_Visible))
    {
        Anim_BonesUpdate(anmHdr, boneCoords, startKeyframeIdx, endKeyframeIdx, alpha);
    }

    // Update alpha.
    model->anim.alpha = alpha;

    // Link to new anim status.
    if (setNewAnimStatus)
    {
        model->anim.status = animInfo->linkStatus;
    }
}

void Anim_BlendEaseOut(s_Model* model, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords, s_AnimInfo* animInfo) // 0x80044DF0
{
    s32    startKeyframeIdx;
    s32    endKeyframeIdx;
    q19_12 duration;
    q19_12 timestep;
    q19_12 alpha;
    q19_12 sinVal;
    q19_12 newTime;
    q19_12 newAlpha;

    startKeyframeIdx = animInfo->startKeyframeIdx;
    endKeyframeIdx   = animInfo->endKeyframeIdx;

    // Compute timestep. TODO: Can't call `Anim_TimestepGet` inline due to register constraints.
    if (model->anim.flags & AnimFlag_Unlocked)
    {
        duration = Anim_DurationGet(model, animInfo);
        timestep = Q12_MULT_PRECISE(duration, g_DeltaTime);
    }
    else
    {
        timestep = Q12(0.0f);
    }

    // Update alpha.
    newAlpha          = model->anim.alpha;
    alpha             = newAlpha + timestep;
    model->anim.alpha = alpha;

    // Compute ease-out alpha.
    sinVal   = Math_Sin((alpha / 2) - Q12(0.25f));
    newAlpha = (sinVal / 2) + Q12(0.5f);

    // Update time to start or end keyframe, whichever is closest.
    if (newAlpha >= Q12(0.5f))
    {
        newTime = Q12(startKeyframeIdx);
    }
    else
    {
        newTime = Q12(endKeyframeIdx);
    }
    alpha = newAlpha;

    // Update time.
    model->anim.time = newTime;

    // Update skeleton.
    if ((model->anim.flags & AnimFlag_Unlocked) || (model->anim.flags & AnimFlag_Visible))
    {
        Anim_BonesUpdate(anmHdr, boneCoords, startKeyframeIdx, endKeyframeIdx, alpha);
    }

    // Update active keyframe.
    model->anim.keyframeIdx = FP_FROM(newTime, Q12_SHIFT);
}
