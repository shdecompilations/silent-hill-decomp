#ifndef _BODYPROG_GAMEBOOT_CHARAANIM_H
#define _BODYPROG_GAMEBOOT_CHARAANIM_H

enum   _CharaId;
struct _AnmHeader;
struct _AnimInfo;

// ========
// STRUCTS
// ========

/** @brief Character model animation data. */
typedef struct _CharaAnimData
{
    /* 0x0  */ s8                 allocCharaId;  /** `enum _CharaId` */
    /* 0x1  */ s8                 activeCharaId; /** `enum _CharaId` */
               // 2 bytes of padding.
    /* 0x4  */ s32                allocAddr; // TODO: Should be `struct _AnmHeader*` but doesn't match.
    /* 0x8  */ struct _AnmHeader* activeAnmHdr;
    /* 0xC  */ s32                allocSize;
    /* 0x10 */ s32                activeSize;
    /* 0x14 */ GsCOORDINATE2*     boneCoords;
} s_CharaAnimData;

// ========
// GLOBALS
// ========

/** @brief Loaded NPC types in memory and their `g_CharaModelAnimsData` indices. Written to by `Fs_CharaAnimDataUpdate`. */
extern s8 g_CharaAnimDataIdxs[Chara_Count];

/** @brief Animation data for loaded character models. */
extern s_CharaAnimData g_CharaModelAnimsData[CHARA_GROUP_COUNT];

#if VERSION_REGION_IS(PAL)
    extern e_FsFile g_MapLanguageBaseIds[];
    extern e_FsFile g_ItemLanguageFileIds[];
#endif

/** @brief Player load screen run anim info. */
extern struct _AnimInfo g_PlayerLoadScreenAnimInfo;

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
void Fs_CharaAnimDataAlloc(s32 animDataIdx, enum _CharaId charaId, struct _AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

/** @brief Assigns character animation data to a free slot and initializes character model bones.
 * Called by `Fs_QueuePostLoadAnm`.
 *
 * @param animDataIdx Index of the animation data to update.
 * @param charaId ID of the character associated with the animation.
 * @param anmHdr Character animation data.
 * @param boneCoords Character bone coords.
 */
void Fs_CharaAnimDataUpdate(s32 animDataIdx, enum _CharaId charaId, struct _AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

/** @brief Dynamically allocates where the next character animation data bone coords should be stored in
 * `g_SysWork.npcBoneCoordBuffer` based on the space consumed by the current character.
 */
void Fs_CharaAnimBoneInfoSet(void);

#endif
