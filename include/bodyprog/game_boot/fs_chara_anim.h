#ifndef _BODYPROG_GAMEBOOT_FSCHARAANIM_H
#define _BODYPROG_GAMEBOOT_FSCHARAANIM_H

#include "bodyprog/chara/chara.h"
#include "bodyprog/formats/anm.h"

// ========
// STRUCTS
// ========

/** @brief Character model animation data. */
typedef struct _CharaAnimData
{
    /* 0x0  */ s8             allocCharaId;  /** `e_CharaId` */
    /* 0x1  */ s8             activeCharaId; /** `e_CharaId` */
               // 2 bytes of padding.
    /* 0x4  */ s32            allocAddr; // TODO: Should be `s_AnmHeader*` but doesn't match.
    /* 0x8  */ s_AnmHeader*   activeAnmHdr;
    /* 0xC  */ s32            allocSize;
    /* 0x10 */ s32            activeSize;
    /* 0x14 */ GsCOORDINATE2* boneCoords;
} s_CharaAnimData;

// ========
// GLOBALS
// ========

/** Loaded NPC types in memory and their `g_CharaModelAnimsData` indices. */
extern s8 g_CharaAnimDataIdxs[Chara_Count];

/** @brief Animation data for loaded character models. */
extern s_CharaAnimData g_CharaModelAnimsData[CHARA_GROUP_COUNT];

/** Player anim info? */
extern s_AnimInfo D_800A998C;

// ==========
// FUNCTIONS
// ==========

/** @brief Allocates memory for character animation data and bone coords.
 *
 * Handles three critical scenarios:
 * 1. If the data is already at the slot, it ensures the memory layout hasn't drifted.
 * 2. If the data is loaded in a different slot, it copies the data over to reuse it.
 * 3. If the data is new, it calls `Fs_QueueStartReadAnm` to read data from the disk.
 *
 * As a final step, it performs an overlap sweep and zeroes out any existing slots that the new allocation collided
 * with.
 *
 * @param animDataIdx Index of the animation data to allocate.
 * @param charaId ID of the character associated with the animation.
 * @param anmHdr Character animation data.
 * @param boneCoords Character bone coords.
 */
void Fs_CharaAnimDataAlloc(s32 animDataIdx, e_CharaId charaId, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

/** @brief Assigns character animation data to a free slot and initializes character model bones.
 * Called by `Fs_QueuePostLoadAnm`.
 *
 * @param animDataIdx Index of the animation data to update.
 * @param charaId ID of the character associated with the animation.
 * @param anmHdr Character animation data.
 * @param boneCoords Character bone coords.
 */
void Fs_CharaAnimDataUpdate(s32 animDataIdx, e_CharaId charaId, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

/** @brief Dynamically allocates where the next character animation data bone coords should be stored in
 * `g_SysWork.npcBoneCoordBuffer` based on the space consumed by the current character. */
void Fs_CharaAnimBoneInfoSet(void);

#endif
