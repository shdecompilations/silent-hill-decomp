#ifndef _BODYPROG_CHARA_BONE_H
#define _BODYPROG_CHARA_BONE_H

/** @brief Bone hierarchy types. */
typedef enum _BoneHierarchy
{
    BoneHierarchy_MultiModel = -3,
    BoneHierarchy_End        = -2
} e_BoneHierarchy;

extern s8* D_800C15B0;

/** Something related to player weapon position. Takes coords to arm bones. */
void Bone_ExtraRotationSet(GsCOORDINATE2* coord, q3_12 rotZ, q3_12 rotX, q19_12 rotY);

s8 Bone_ModelIdxIterator(s8* boneHierarchy, bool arg1);

/** Skeleton setup? Assigns bones pointer for the skeleton and resets fields. */
void Skeleton_Init(s_Skeleton* skel, s_BoneNode* boneHierarchy, u8 boneCount);

/** Clears skeleton bone flags/mask. Called by `Skeleton_Init`. */
void func_80045014(s_Skeleton* skel);

/** Anim func. Used in tandem with skeleton bone traversal. */
void func_8004506C(s_Skeleton* skel, s_LmHeader* lmHdr);

/** Anim func. */
void func_80045108(s_Skeleton* skel, s_LmHeader* lmHdr, s8* boneHierarchy, bool cond);

/** Anim func. */
void Skeleton_BoneModelAssign(s_Skeleton* skel, s_LmHeader* lmHdr, s8* boneHierarchy);

/** Anim func. Param names are rough. */
void func_80045258(s_BoneNode** boneOrd, s_BoneNode* boneHierarchy, s32 boneIdx, s_LmHeader* lmHdr);

/** Anim func. */
void func_800452EC(s_Skeleton* skel);

/** Anim func. Traverses skeleton bones for something. */
void func_80045360(s_Skeleton* skel, s8* boneHierarchy);

/** Anim func. Traverses skeleton bones to set flags/mask. */
void func_800453E8(s_Skeleton* skel, bool cond);

/** Does something with skeleton bones. */
void func_80045468(s_Skeleton* skel, s32* boneHierarchy, bool cond);

// Bone mesh texture handling?
void func_80045534(s_Skeleton* skel, GsOT* ot, s32 otShift, GsCOORDINATE2* boneCoords,
                   q3_12 arg4, u16 clutY, s_FsImageDesc* images);

#endif
