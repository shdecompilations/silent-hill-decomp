#ifndef _BODYPROG_COLLISION_CHARA_H
#define _BODYPROG_COLLISION_CHARA_H

#include "bodyprog/chara/chara.h"

/** @brief Character collision cylinder. TODO: Too similar to `s_CharaCylinder` */
typedef struct _CharaCollisionCylinder
{
    /* 0x0  */ VECTOR3 position;     /** Q19.12 */
    /* 0xC  */ q3_12   bottomExtent;
    /* 0xE  */ q3_12   topExtent;
    /* 0x10 */ q3_12   radius;
    /* 0x14 */ s8      state;        /** `e_CharaCollisionState` */
} s_CharaCollisionCylinder;
STATIC_ASSERT_SIZEOF(s_CharaCollisionCylinder, 20);

/** @brief Character collision query. */
typedef struct _CharaCollisionQuery
{
    /* 0x0  */ s32        state;             /** `e_CharaCollisionState` */
    /* 0x4  */ bool       blockInvalidFloor;
    /* 0x8  */ q19_12     moveDistance;
    /* 0xC  */ SVECTOR    moveOffset;        /** Q23.8 */
    /* 0x14 */ DVECTOR_XZ moveDirection;     /** Q3_12 | Unit vector. */
    /* 0x18 */ q23_8      positionFromX;
    /* 0x1C */ q23_8      positionFromZ;
    /* 0x20 */ q23_8      positionToX;
    /* 0x24 */ q23_8      positionToZ;
    /* 0x28 */ q7_8       radius;
    /* 0x2A */ q7_8       top;
    /* 0x2C */ q7_8       bottom;
} s_CharaCollisionQuery;

// TODO: Rough name.
typedef struct _CharaCollisionDeltas
{
    /* 0x0  */ DVECTOR_XZ charaVertDiff;
    /* 0x4  */ DVECTOR_XZ charaMoveVertDiff;
    /* 0x8  */ u32        subChunkTransDir;   /** `e_SubChunkTransitionDirection` */
    /* 0xC  */ q23_8      radiusCollDiffDist; // Radius?
    /* 0x10 */ q7_8       collDiffDist;
    /* 0x12 */ s8         __pad_12[2];
    /* 0x14 */ DVECTOR_XZ charaMoveOffset;
} s_CharaCollisionDeltas;

bool func_80070320(void);

/** TODO: Return type uncertain. */
q19_12 func_80070360(s_SubCharacter* chara, q19_12 someDist, q3_12 arg2);

/** @brief Sets a character's interpolated collision shapes for the active animation frame.
 *
 * @param chara Character to update.
 * @param keyframe0 First keyframe.
 * @param keyframe1 Second keyframe.
 */
void Collision_CharaCollisionSet(s_SubCharacter* chara, s_CharaKeyframe* keyframe0, s_CharaKeyframe* keyframe1);

/** @brief Sets the scale of a character's model bone.
 *
 * @param boneCoords Character model bone coords.
 * @param boneIdx Index of the model bone to scale.
 * @param scaleX X scale.
 * @param scaleY Y scale.
 * @param scaleZ Z scale.
 */
void Chara_ModelBoneScaleSet(GsCOORDINATE2* boneCoords, s32 boneIdx, q19_12 scaleX, q19_12 scaleY, q19_12 scaleZ);

#endif
