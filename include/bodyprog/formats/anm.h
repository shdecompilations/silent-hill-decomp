#ifndef _BODYPROG_FORMATS_ANM_H
#define _BODYPROG_FORMATS_ANM_H

// See: https://github.com/laura-a-n-n/silent-hill-museum/blob/main/ksy/sh1anm.ksy
typedef struct _AnmBindPose
{
    /* 0x0 */ s8 parentBone;
    /* 0x1 */ s8 rotationDataIdx;
    /* 0x2 */ s8 translationDataIdx;
    /* 0x3 */ s8 translationInitial[3];
} s_AnmBindPose;

/** @brief ANM file header. */
typedef struct _AnmHeader
{
    /* 0x0  */ u16           dataOffset;
    /* 0x2  */ u8            rotationBoneCount;
    /* 0x3  */ u8            translationBoneCount;
    /* 0x4  */ u16           keyframeDataSize; /** Size per keyframe. `(rotationBoneCount * 9) + (translationBoneCount * 3)`? */
    /* 0x6  */ u8            boneCount;        /** Size of `bindPoses`. */
    /* 0x7  */ s8            __pad_7;
    /* 0x8  */ u32           activeBoneFlags;
    /* 0xC  */ u32           fileSize;
    /* 0x10 */ u16           keyframeCount;
    /* 0x12 */ u8            scaleLog2;
    /* 0x13 */ u8            rootYOffset;
    /* 0x14 */ s_AnmBindPose bindPoses[0];
} s_AnmHeader;

#endif
