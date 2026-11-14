#ifndef _BODYPROG_CHARA_CHARAMODEL_H
#define _BODYPROG_CHARA_CHARAMODEL_H

#include "bodyprog/chara/anim.h"
#include "bodyprog/formats/lm.h"
#include "bodyprog/formats/model.h"
#include "main/fs_queue.h"

#define BONE_NODE_COUNT_MAX 56

/** @brief IPD skeleton bone. */
typedef struct _Bone
{
    /* 0x0  */ s_ModelInfo modelInfo;
    /* 0x10 */ s8          modelIdx;
               // 3 bytes of padding.
} s_Bone;
STATIC_ASSERT_SIZEOF(s_Bone, 20);

/** @brief IPD skeleton bone node. */
typedef struct _BoneNode
{
    /* 0x0  */ s_Bone            bone;
    /* 0x14 */ struct _BoneNode* next;
} s_BoneNode;
STATIC_ASSERT_SIZEOF(s_BoneNode, 24);

/** @brief Character skeleton. */
typedef struct _Skeleton
{
    /* 0x0 */ u8          boneCount;
    /* 0x1 */ u8          boneIdx;
    /* 0x2 */ u8          field_2;
    /* 0x3 */ s8          field_3;
    /* 0x4 */ s_BoneNode* bones_4; // Secondary bone hierarchy? Visible bones?
    /* 0x8 */ s_BoneNode* boneHierarchy;
    /* 0xC */ s_BoneNode  boneNodes[BONE_NODE_COUNT_MAX];
} s_Skeleton;
STATIC_ASSERT_SIZEOF(s_Skeleton, 1356);

/** @brief Skeletal character model. */
typedef struct _CharaModel
{
    /* 0x0  */ u8            charaId;  /** `e_CharaId` */
    /* 0x1  */ u8            isLoaded; /** `bool` */
               // 2 bytes of padding.
    /* 0x4  */ s32           queueIdx;
    /* 0x8  */ s_LmHeader*   lmHdr;
    /* 0xC  */ s_FsImageDesc texture;
    /* 0x14 */ s_Skeleton    skeleton;
} s_CharaModel;
STATIC_ASSERT_SIZEOF(s_CharaModel, 1376);

/** @brief Character model animation. */
typedef struct _ModelAnim
{
    /* 0x0  */ u8          status;             /** Is active: bit 0, Anim index: bits 1-7. */
    /* 0x1  */ u8          mapAnimStatusStart; /** Start anim status of map-specific anim infos. Only used for Harry. */
    /* 0x2  */ u16         flags;              /** `e_AnimFlags` */
    /* 0x4  */ q19_12      time;               /** Time on timeline. */
    /* 0x8  */ s16         keyframeIdx;        /** Active keyframe index. */
    /* 0xA  */ q3_12       alpha;              /** Keyframe progress alpha. */
    /* 0xC  */ s_AnimInfo* baseAnimInfos;      /** Anim infos. For Harry, used for base anims. */
    /* 0x10 */ s_AnimInfo* mapAnimInfos;       /** Map-specific anim infos. Only used for Harry. */
} s_ModelAnim;
STATIC_ASSERT_SIZEOF(s_ModelAnim, 20);

/** @brief Character model. TODO: Ambiguity with `s_CharaModel`. */
typedef struct _Model
{
    /* 0x0 */ s8          charaId;      /** `e_CharaId` */
    /* 0x1 */ u8          paletteIdx;   /** Changes the texture palette index for this model. */
    /* 0x2 */ u8          controlState; /** Active character control state. */
    /* 0x3 */ u8          stateStep;    /** Step for the current `controlState`. */ 
                                        // In `s_PlayerExtra` always 1, set to 0 for 1 tick when anim state appears to change.
                                        // Used differently in player's `s_SubCharacter`. 0: anim transitioning(?), bit 1: animated, bit 2: turning.
                                        // Sometimes holds actual anim index?
    /* 0x4 */ s_ModelAnim anim;
} s_Model;
STATIC_ASSERT_SIZEOF(s_Model, 24);

/** @brief Packs a mesh swap status containing a swappable ID and variant ID.
 *
 * @param swappableId Swappable mesh ID.
 * @param variantId Mesh variant ID.
 * @return Packed mesh swap status containing the swappable ID and variant ID.
 */
#define MESH_SWAP_STATUS(swappableId, variantId) \
    (s16)(((swappableId) << 4) | (variantId))

/** @brief Retrieves the swappable mesh ID from a packed mesh swap status.
 *
 * @note The value remains shifted left by 4.
 *
 * @param meshSwap Packed mesh swap status containing a swappable ID and variant ID.
 * @return Swappable ID.
 */
#define MESH_SWAP_STATUS_SWAPPABLE_ID_GET(meshSwap) \
    ((meshSwap) & 0xF0)

/** @brief Retrieves the variant mesh ID from a packed mesh swap status.
 *
 * @param meshSwap Packed mesh swap status containing a swappable ID and variant ID.
 * @return Variant ID.
 */
#define MESH_SWAP_STATUS_VARIANT_ID_GET(meshSwap) \
    ((meshSwap) & 0xF)

/** @brief Sets given animation flags for a model.
 *
 * @param model Model to update (`s_Model`).
 * @param flag Flags to set.
 */
#define Model_AnimFlagsSet(model, setFlags) \
    (model)->anim.flags |= (setFlags)

/** @brief Clears given animation flags for a model.
 *
 * @param model Model to update (`s_Model`).
 * @param clearFlags Flags to clear.
 */
#define Model_AnimFlagsClear(model, clearFlags) \
    (model)->anim.flags &= ~(clearFlags)

/** @brief Similar to `Model_AnimStatusSet`, but also sets `anim.time` and `anim.keyframeIdx`
 * from the `animInfos` `s_AnimInfo` array.
 *
 * @param model Model to update.
 * @param animIdx Anim index to set.
 * @param isActive Active status to set.
 * @param animInfos Reference anim infos.
 * @param animInfosOffset Anim infos offset.
 */
#define Model_AnimStatusKeyframeSet(model, animIdx, isActive, animInfos, animInfosOffset)                                 \
    if ((model).stateStep == 0)                                                                                           \
    {                                                                                                                     \
        (model).anim.status = ANIM_STATUS(animIdx, isActive);                                                             \
        (model).stateStep++;                                                                                              \
        (model).anim.time        = Q12((animInfos)[ANIM_STATUS(animIdx, isActive) + (animInfosOffset)].startKeyframeIdx); \
        (model).anim.keyframeIdx = (animInfos)[ANIM_STATUS(animIdx, (isActive) + (animInfosOffset))].startKeyframeIdx;    \
    }

/** @brief Updates a model anim if `model->stateStep` is 0.
 *
 * @param model Model to update.
 * @param animIdx Anim index to set.
 * @param isActive Active status to set.
 */
static inline void Model_AnimStatusSet(s_Model* model, s32 animIdx, bool isActive)
{
    if (model->stateStep == 0)
    {
        model->anim.status = ANIM_STATUS(animIdx, isActive);
        model->stateStep++;
    }
}

/** @brief Increments the anim status of a model anim.
 *
 * @param anim Anim to update.
 */
static inline void ModelAnim_StatusIncrement(s_ModelAnim* anim)
{
    anim->status++;
}

/** @brief Decrements the anim status of a model anim.
 *
 * @param anim Anim to update.
 */
static inline void ModelAnim_StatusDecrement(s_ModelAnim* anim)
{
    anim->status--;
}

#endif
