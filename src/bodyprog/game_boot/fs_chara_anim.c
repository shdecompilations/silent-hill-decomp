#include "game.h"

#include <psyq/libetc.h>
#include <psyq/libpad.h>
#include <psyq/strings.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/game_boot/fs_chara_anim.h"
#include "main/fsqueue.h"
#include "bodyprog/demo.h"
#include "bodyprog/game_boot/game_boot.h"
#include "bodyprog/screen/screen_draw.h"
#include "bodyprog/text/text_draw.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player.h"
#include "bodyprog/ranking.h"
#include "bodyprog/sound/sound_system.h"

// ========================================
// GLOBAL VARIABLES
// ========================================

s8 g_CharaAnimDataIdxs[Chara_Count] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    // 3 bytes of padding.
};

s_CharaAnimData g_CharaModelAnimsData[CHARA_GROUP_COUNT] = {
    {
        .allocCharaId  = Chara_Harry,
        .activeCharaId = Chara_Harry,
        .allocAddr     = (s_AnmHeader*)FS_BUFFER_0,
        .activeAnmHdr  = (s_AnmHeader*)FS_BUFFER_0,
        .allocSize     = 0x2E630,
        .activeSize    = 0x2E630,
        .boneCoords    = NULL
    }
};

#if VERSION_IS(EUR)
    // File index tables, TODO: Should probably live in game_boot.c next to code that uses them?
    e_FsFile g_MapLanguageBaseIds[5] =
    {
        FILE_VIN_MAP0_S00_BIN,
        FILE_VIN2_MAP0_S10_BIN,
        FILE_VIN3_MAP0_S20_BIN,
        FILE_VIN4_MAP0_S30_BIN,
        FILE_VIN5_MAP0_S40_BIN
    };
    e_FsFile g_ItemLanguageFileIds[5] =
    {
        FILE_VIN_ITEM_ENG_BIN,
        FILE_VIN_ITEM_GER_BIN,
        FILE_VIN_ITEM_FRN_BIN,
        FILE_VIN_ITEM_SPN_BIN,
        FILE_VIN_ITEM_ITL_BIN
    };
#endif

s_AnimInfo D_800A998C = {
    .playbackFunc        = Anim_PlayLoop,
    .status              = 0,
    .hasVariableDuration = false,
    .linkStatus          = ANIM_STATUS(0, false),
    .duration            = { Q12(8.0f) },
    .startKeyframeIdx    = 26,
    .endKeyframeIdx      = 44
};

// ========================================
// ANIMATION FILE STREAMING
// ========================================

/** @brief Checks if the raw file streaming memory allocations of two character animation data slots overlap.
 *
 * @param animDataIdx0 First animation data index.
 * @param animDataIdx1 Second animation data index.
 * @return `true` if the animation data overlaps, `false` otherwise.
 */
static bool Fs_CharaAnimDataOverlapCheck(s32 animDataIdx0, s32 animDataIdx1)
{
    u32              animBufferAddr1;
    u32              animBufferAddr0;
    s_CharaAnimData* animData0;
    s_CharaAnimData* animData1;

    animData0       = &g_CharaModelAnimsData[animDataIdx0];
    animData1       = &g_CharaModelAnimsData[animDataIdx1];
    animBufferAddr0 = animData0->allocAddr;
    animBufferAddr1 = animData1->activeAnmHdr;

    if (animBufferAddr0 >= (animBufferAddr1 + animData1->activeSize) ||
        animBufferAddr1 >= (animBufferAddr0 + animData0->allocSize))
    {
        return false;
    }

    return true;
}

/** @brief Finds the character animation data slot index.
 * If not found, it returns index 0.
 *
 * @param charaId ID of the character for which to find the animation data.
 * @return Animation data index.
 */
static s32 Fs_CharaAnimDataIdxGet(e_CharaId charaId)
{
    s32 i;

    for (i = 1; (i < CHARA_GROUP_COUNT); i++)
    {
        if (g_CharaModelAnimsData[i].activeCharaId == charaId)
        {
            return i;
        }
    }

    return 0;
}

void Fs_CharaAnimDataAlloc(s32 animDataIdx, e_CharaId charaId, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords) // 0x80035338
{
    s32              i;
    s_AnmHeader*     localAnmHdr;
    s_CharaAnimData* initAnimData;
    s_CharaAnimData* npcAnimData;

    localAnmHdr  = anmHdr;
    initAnimData = &g_CharaModelAnimsData[animDataIdx];

    if (charaId == Chara_None)
    {
        return;
    }

    // Estimate pointer to ANM data by summing buffer size and current pointer position.
    for (npcAnimData = &initAnimData[-1]; localAnmHdr == NULL; npcAnimData--)
    {
        localAnmHdr = npcAnimData->allocAddr + npcAnimData->allocSize;
    }

    // If the target character ID matches with the selected element from `g_CharaModelAnimsData`,
    // it ensures the animation buffer pointer matches with the previously estimated one.
    // If the estimated pointer is in a position behind of the currently saved one, it moves
    // data to the position of the estimated pointer.
    //
    // If any of the previous checks fail, values previously assigned at the index are cleared. If
    // the character hasn't been loaded in a different `g_InitializedCharaAnimInfo` slot, the
    // animation file is loaded.
    if (initAnimData->activeCharaId == charaId)
    {
        if (animDataIdx == 1 || localAnmHdr == initAnimData->activeAnmHdr)
        {
            Fs_CharaAnimDataUpdate(animDataIdx, charaId, initAnimData->activeAnmHdr, boneCoords);
            return;
        }
        else if (localAnmHdr < initAnimData->activeAnmHdr)
        {
            initAnimData->allocAddr = localAnmHdr;

            Mem_Move32(localAnmHdr,
                       g_CharaModelAnimsData[animDataIdx].activeAnmHdr,
                       g_CharaModelAnimsData[animDataIdx].activeSize);
            Fs_CharaAnimDataUpdate(animDataIdx, charaId, localAnmHdr, boneCoords);
            return;
        }
    }

    initAnimData->boneCoords    = &g_SysWork.npcBoneCoordBuffer[0];
    initAnimData->activeCharaId = Chara_None;
    initAnimData->activeAnmHdr  = NULL;
    initAnimData->activeSize    = 0;
    initAnimData->allocCharaId  = charaId;
    initAnimData->allocAddr     = localAnmHdr;
    initAnimData->allocSize     = Fs_GetFileSectorAlignedSize(CHARA_FILE_INFOS[charaId].animFileIdx);

    i = Fs_CharaAnimDataIdxGet(charaId);

    if (i > 0)
    {
        Mem_Move32(g_CharaModelAnimsData[animDataIdx].allocAddr,
                   g_CharaModelAnimsData[i].activeAnmHdr,
                   g_CharaModelAnimsData[i].activeSize);
        Fs_CharaAnimDataUpdate(animDataIdx, charaId, initAnimData->allocAddr, boneCoords);
    }
    else
    {
        Fs_QueueStartReadAnm(animDataIdx, charaId, localAnmHdr, boneCoords);
    }

    // Run through character group.
    for (i = 1; i < CHARA_GROUP_COUNT; i++)
    {
        if (i != animDataIdx && g_CharaModelAnimsData[i].activeCharaId != Chara_None &&
            Fs_CharaAnimDataOverlapCheck(animDataIdx, i) != false)
        {
            bzero(&g_CharaModelAnimsData[i], sizeof(s_CharaAnimData));
        }
    }
}

void Fs_CharaAnimDataUpdate(s32 animDataIdx, e_CharaId charaId, s_AnmHeader* animFile, GsCOORDINATE2* boneCoords) // 0x80035560
{
    s32              boneCount;
    GsCOORDINATE2*   localBoneCoords;
    s_CharaAnimData* animData;

    localBoneCoords = boneCoords;
    animData        = &g_CharaModelAnimsData[animDataIdx];

    if (localBoneCoords == NULL)
    {
        if (animDataIdx == 1)
        {
            localBoneCoords = &g_SysWork.npcBoneCoordBuffer[0];
        }
        else if (animDataIdx >= 2)
        {
            boneCount        = g_CharaModelAnimsData[animDataIdx - 1].activeAnmHdr->boneCount;
            localBoneCoords  = g_CharaModelAnimsData[animDataIdx - 1].boneCoords;
            localBoneCoords += boneCount + 1;

            // Check for end of `g_SysWork.npcBoneCoordBuffer`.
            // @note Since `g_MapOverlayHdr.npcBoneCoordBuffer` is always assigned to
            // `g_SysWork.npcBoneCoordBuffer[0]`, this effectively forms a ring buffer.
            if ((&localBoneCoords[animFile->boneCount] + 1) >= &g_SysWork.npcBoneCoordBuffer[NPC_BONE_COUNT_MAX])
            {
                localBoneCoords = g_MapOverlayHdr.npcBoneCoordBuffer;
            }
        }
    }

    animData->activeCharaId = charaId;
    animData->activeAnmHdr  = animFile;
    animData->activeSize    = Fs_GetFileSectorAlignedSize(CHARA_FILE_INFOS[charaId].animFileIdx);
    animData->boneCoords    = localBoneCoords;

    Anim_BonesInit(animFile, localBoneCoords);

    g_CharaAnimDataIdxs[charaId] = animDataIdx;
}

void Fs_CharaAnimBoneInfoSet(void) // 0x8003569C
{
    s32            i;
    GsCOORDINATE2* boneCoords;
    s_AnmHeader*   anmHdr;

    // Run through character group.
    for (i = 1; i < (CHARA_GROUP_COUNT - 1); i++)
    {
        // Skip if no valid character model.
        if (g_MapOverlayHdr.charaGroupIds[i] == Chara_None)
        {
            continue;
        }

        boneCoords  = g_CharaModelAnimsData[i].boneCoords;
        anmHdr      = g_CharaModelAnimsData[i + 1].activeAnmHdr;
        boneCoords += g_CharaModelAnimsData[i].activeAnmHdr->boneCount + 1;

        // Check for end of `g_SysWork.npcBoneCoordBuffer`.
        // @note Since `g_MapOverlayHdr.npcBoneCoordBuffer` is always assigned to `g_SysWork.npcBoneCoordBuffer[0]`,
        // this effectively forms a ring buffer.
        if ((&boneCoords[anmHdr->boneCount] + 1) >= &g_SysWork.npcBoneCoordBuffer[NPC_BONE_COUNT_MAX])
        {
            boneCoords = g_MapOverlayHdr.npcBoneCoordBuffer;
        }

        g_CharaModelAnimsData[i + 1].boneCoords = boneCoords;
        Anim_BonesInit(anmHdr, boneCoords);
    }
}
