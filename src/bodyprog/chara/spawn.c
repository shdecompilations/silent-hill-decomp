#include "bodyprog/game.h"
#include "inline_no_dmpsx.h"

#include <psyq/libpad.h>
#include <psyq/strings.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/chara/spawn.h"
#include "bodyprog/chara/chara.h"
#include "bodyprog/formats/lm.h"
#include "bodyprog/game_boot/chara_anim.h"
#include "bodyprog/items/item_screens.h"
#include "bodyprog/math/math.h"
#include "bodyprog/screen/draw.h"
#include "bodyprog/sound/sd_call.h"
#include "main/fsqueue.h"

bool Chara_Load(s32 modelIdx, s8 charaId, GsCOORDINATE2* boneCoords, s8 forceFree,
                s_LmHeader* lmHdr, s_FsImageDesc* tex) // 0x80088C7C
{
    Fs_CharaAnimDataAlloc(modelIdx + 1, charaId, NULL, boneCoords);
    WorldGfx_CharaLmBufferAssign(forceFree);
    WorldGfx_CharaLoad(charaId, modelIdx, lmHdr, tex);
    return true;
}

bool Chara_ProcessLoads(void) // 0x80088D0C
{
    Fs_QueueWaitForEmpty();
    WorldGfx_CharaModelProcessAllLoads();
    return true;
}

void Chara_BonesInit(s32 modelAnimsIdx) // 0x80088D34
{
    modelAnimsIdx++;
    Anim_BonesInit(g_CharaModelAnimsData[modelAnimsIdx].activeAnmHdr, g_CharaModelAnimsData[modelAnimsIdx].boneCoords);
}

s32 Chara_Spawn(e_CharaId charaId, s32 spawnFlags, q19_12 posX, q19_12 posZ, q3_12 rotY, u32 stateStep) // 0x80088D78
{
    s_CollisionSurface surface;
    s32                i;
    s32                npcIdx;
    s32                activeSpawnFlags;
    s_SubCharacter*    chara;

    // TODO: Weird code, check.
    if (charaId <= CHARA_LAST_ENEMY_ID && spawnFlags < (1 << 6))
    {
        activeSpawnFlags = CharaSpawnFlag_0 | CharaSpawnFlag_1 | CharaSpawnFlag_2 | CharaSpawnFlag_3 | CharaSpawnFlag_4;
        activeSpawnFlags = spawnFlags & activeSpawnFlags;
    }
    else
    {
        activeSpawnFlags = CharaSpawnFlag_None;
    }

    if (charaId <= CHARA_LAST_ENEMY_ID)
    {
        if (HAS_FLAG(g_SysWork.field_228C, activeSpawnFlags))
        {
            for (i = 0; i < ARRAY_SIZE(g_SysWork.npcs); i++)
            {
                if (NPC_CHARA(i).field_40 == activeSpawnFlags)
                {
                    return i;
                }
            }

            return ARRAY_SIZE(g_SysWork.npcs);
        }

        npcIdx = 0;
        for (i = 0; i < ARRAY_SIZE(g_SysWork.npcs); i++)
        {
            if (NPC_CHARA(i).model.charaId != Chara_None)
            {
                npcIdx++;
            }
        }

        if (npcIdx >= g_SysWork.npcFlagId)
        {
            return 0;
        }
    }

    // Run through NPC slots.
    for (i = 0; i < ARRAY_SIZE(g_SysWork.npcs); i++)
    {
        // Find unoccupied slot.
        if (NPC_CHARA(i).model.charaId != Chara_None)
        {
            continue;
        }

        // Clear character data.
        bzero(&NPC_CHARA(i), sizeof(s_SubCharacter));

        NPC_CHARA(i).model.charaId = charaId;
        NPC_CHARA(i).field_40      = activeSpawnFlags;

        if (charaId <= CHARA_LAST_ENEMY_ID && spawnFlags < (1 << 6))
        {
            SET_FLAG(g_SysWork.field_228C, activeSpawnFlags);
        }

        // Set registered NPC flag.
        SET_FLAG(&g_SysWork.npcFlags, i);

        // Set NPC default control state and start position.
        NPC_CHARA(i).model.controlState = 0;
        NPC_CHARA(i).model.stateStep    = stateStep;
        NPC_CHARA(i).position.vx        = posX;
        Collision_SurfaceGet(&surface, posX, posZ);
        NPC_CHARA(i).position.vy        = surface.floorHeight;
        NPC_CHARA(i).position.vz        = posZ;
        NPC_CHARA(i).rotation.vy        = rotY;

        // Set visible.
        chara                    = &NPC_CHARA(i);
        chara->model.anim.flags |= AnimFlag_Visible;

        return i;
    }

    // Fall back on last NPC slot as failsafe.
    return ARRAY_SIZE(g_SysWork.npcs);
}

void Chara_ModelCharaIdClear(s_SubCharacter* chara, s32 unused0, s32 unuse1) // 0x80088F94
{
    if (chara == NULL)
    {
        return;
    }

    if (chara->model.charaId <= CHARA_LAST_ENEMY_ID)
    {
        CLEAR_FLAG(g_SysWork.field_228C, chara->field_40);
    }

    chara->model.charaId = Chara_None;
}

void Chara_SpawnFlagsSet(e_CharaId charaId, s32 spawnIdx, s32 spawnFlags) // 0x80088FF4
{
    s_SpawnInfo* spawnInfo;

    spawnInfo        = &g_MapOverlayHdr.charaSpawnInfos[g_CharaAnimDataIdxs[charaId] - 1][spawnIdx];
    spawnInfo->flags = spawnFlags;
}

void Chara_SpawnPositionSet(e_CharaId charaId, s32 spawnIdx, q19_12 posX, q19_12 posZ) // 0x80089034
{
    s_SpawnInfo* spawnInfo0;
    s_SpawnInfo* spawnInfo1;

    spawnInfo0            = &g_MapOverlayHdr.charaSpawnInfos[g_CharaAnimDataIdxs[charaId] - 1][spawnIdx];
    spawnInfo0->positionX = posX;

    spawnInfo1            = &g_MapOverlayHdr.charaSpawnInfos[g_CharaAnimDataIdxs[charaId] - 1][spawnIdx];
    spawnInfo1->positionZ = posZ;
}
