#include "game.h"

#include <psyq/libetc.h>
#include <psyq/libpad.h>
#include <psyq/strings.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/sys/demo.h"
#include "bodyprog/events/event_data.h"
#include "bodyprog/events/npc_main.h"
#include "bodyprog/events/radio.h"
#include "bodyprog/game_boot/fs_chara_anim.h"
#include "bodyprog/world/effects.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player/player.h"
#include "bodyprog/screen/data.h"
#include "bodyprog/sound/sound_system.h"
#include "main/fsqueue.h"

void Savegame_EnemyStateUpdate(s_SubCharacter* chara) // 0x80037DC4
{
    if (g_SavegamePtr->gameDifficulty <= GameDifficulty_Normal || Rng_RandQ12() >= Q12(0.3f))
    {
        g_SavegamePtr->mapEnemyStates[g_SavegamePtr->mapIdx] &= ~(1 << chara->field_40);
    }
}

void Chara_DamagedFlagUpdate(s_SubCharacter* chara) // 0x80037E40
{
    if (chara->damage.amount > Q12(0.0f))
    {
        chara->flags |= CharaFlag_Damaged;
    }
    else
    {
        chara->flags &= ~CharaFlag_Damaged;
    }
}

void func_80037E78(s_SubCharacter* chara) // 0x80037E78
{
    s8  idx;
    s32 cond;

    // @hack Odd check for `(chara->flags & (CharaFlag_Damaged | CharaFlag_Dead)) == CharaFlag_Damaged`.
    if (chara->health <= Q12(0.0f) &&
        (*(s32*)&chara->headingAngle & ((CharaFlag_Damaged | CharaFlag_Dead) << 16)) == (CharaFlag_Damaged << 16))
    {
        idx = chara->attackReceived;
        if (idx < 39) // TODO: What weapon attack?
        {
            cond = D_800AD4C8[idx].field_10 == 3;
            func_800914C4(cond, func_8009146C(cond) + 1);
        }

        chara->flags |= CharaFlag_Dead;
    }
}

void Game_NpcRoomInitSpawn(bool cond) // 0x80037F24
{
    #define GET_NPC_FLAG(flagId) \
        ((1 << (flagId)) - 1)

    s_CollisionSurface surface;
    s32                groupCharaId0;
    s32                groupCharaId1;
    s32                npcIdx;
    s32                i;
    s32*               ovlEnemiesStatePtr;
    s_SpawnInfo*       curCharaSpawn;
    s_SubCharacter*    chara;
    VECTOR3*           pos;

    npcIdx             = 0;
    curCharaSpawn      = g_MapOverlayHdr.charaSpawnInfos[0];
    ovlEnemiesStatePtr = &g_SavegamePtr->mapEnemyStates[g_SavegamePtr->mapIdx];

    if (cond == false)
    {
        Game_RadioNoiseReset();

        if (g_MapOverlayHdr.npcSpawnEvent != NULL)
        {
            g_MapOverlayHdr.npcSpawnEvent();
        }
    }

    groupCharaId0 = g_MapOverlayHdr.charaGroupIds[0];
    groupCharaId1 = g_MapOverlayHdr.charaGroupIds[1];

    for (i = 0; i < 32 && g_VBlanks < 4; i++, curCharaSpawn++)
    {
        if (g_SysWork.npcFlags == GET_NPC_FLAG(g_SysWork.npcFlagId))
        {
            break;
        }

        pos = (VECTOR3*)curCharaSpawn;

        if (!(g_SysWork.sysFlags & SysFlag_NoEnemySpawn) &&
            HAS_FLAG(ovlEnemiesStatePtr, i) && !HAS_FLAG(g_SysWork.field_228C, i) &&
            curCharaSpawn->flags != CharaSpawnFlag_None &&
            g_SavegamePtr->gameDifficulty >= curCharaSpawn->gameDifficultyMin &&
            func_8008F914(curCharaSpawn->positionX, curCharaSpawn->positionZ) &&
            !Math_Distance2dCheck(&PLAYER_CHARA().position, pos, Q12(22.0f)) &&
            (!cond || Math_Distance2dCheck(&PLAYER_CHARA().position, pos, Q12(20.0f))))
        {
            while (HAS_FLAG(&g_SysWork.npcFlags, npcIdx))
            {
                npcIdx++;
            }

            bzero(&NPC_CHARA(npcIdx), sizeof(s_SubCharacter));

            if (curCharaSpawn->charaId > Chara_None)
            {
                NPC_CHARA(npcIdx).model.charaId = curCharaSpawn->charaId;
            }
            else
            {
                NPC_CHARA(npcIdx).model.charaId = (i < 16) ? groupCharaId0 : groupCharaId1;
            }

            NPC_CHARA(npcIdx).field_40           = i;
            NPC_CHARA(npcIdx).model.controlState = 0;
            NPC_CHARA(npcIdx).model.stateStep    = curCharaSpawn->flags;
            NPC_CHARA(npcIdx).position.vx        = curCharaSpawn->positionX;
            NPC_CHARA(npcIdx).position.vz        = curCharaSpawn->positionZ;
            Collision_SurfaceGet(&surface, curCharaSpawn->positionX, curCharaSpawn->positionZ);
            NPC_CHARA(npcIdx).position.vy        = surface.floorHeight;
            NPC_CHARA(npcIdx).rotation.vy        = Q8_TO_Q12(curCharaSpawn->rotationY);

            SET_FLAG(&g_SysWork.npcFlags, npcIdx);
            SET_FLAG(g_SysWork.field_228C, i);

            chara                    = &NPC_CHARA(npcIdx);
            chara->model.anim.flags |= AnimFlag_Visible;
        }
    }

    #undef GET_NPC_FLAG
}

void Game_NpcUpdate(void) // 0x80038354
{
    /** @brief Close NPC info for radio interference. */
    typedef struct _CloseNpcInfo
    {
        /* 0x0 */ s8      bitIdx;
                  // 3 bytes of padding.
        /* 0x4 */ s32     distanceToNpc; /** Squared integer. */
        /* 0x8 */ VECTOR3 position;      /** Q19.12 */
    } s_CloseNpcInfo;

    s_CloseNpcInfo  closestNpcInfos[3];
    u32             idxBits;
    s32             posZShift6;
    s32             posXShift6;
    s32             m;
    u8              vol;
    s32             j;
    bool            isExterior;
    s32             k;
    bool            isLowVisInterior;
    s32             closeNpcInfoIdx0;
    q27_4           distToNpc;
    s32             closeNpcInfoIdx1;
    s32             balance;
    s8              idx;
    s32             closeNpcInfoIdx;
    s32             bitIdx;
    u32             curDistToNpc;    // Squared integer.
    u32             curDistToNpcCpy; // Squared integer.
    s32             l;
    s32             animDataIdx;
    s32             temp2;
    GsCOORDINATE2*  boneCoords;
    s_SubCharacter* curNpc;
    s_CloseNpcInfo* closeNpcInfo;

    s32 func_800382B0(s32 bitIdx)
    {
        s32 i;

        for (i = 0; i < (ARRAY_SIZE(closestNpcInfos) - 1); i++)
        {
            if (bitIdx == closestNpcInfos[i].bitIdx)
            {
                return i;
            }
        }

        return NO_VALUE;
    }

    // Gets close NPC info index.
    static s32 func_800382EC()
    {
        s32 i;

        for (i = 0; i < (ARRAY_SIZE(closestNpcInfos) - 1); i++)
        {
            if (closestNpcInfos[i].bitIdx == NO_VALUE)
            {
                break;
            }

            if ((idxBits & (1 << closestNpcInfos[i].bitIdx)) == 0)
            {
                idxBits |= 1 << closestNpcInfos[i].bitIdx;
                return i;
            }
        }

        return NO_VALUE;
    }

    posXShift6 = Q12_TO_Q6(PLAYER_CHARA().position.vx);
    posZShift6 = Q12_TO_Q6(PLAYER_CHARA().position.vz);

    Demo_DemoRandSeedBackup();
    Demo_DemoRandSeedRestore();

    for (j = 0; j < ARRAY_SIZE(closestNpcInfos); j++)
    {
        closestNpcInfos[j].bitIdx        = NO_VALUE;
        closestNpcInfos[j].distanceToNpc = SQUARE(32);
        closestNpcInfos[j].position.vy   = Q12(0.0f);
    }

    // Run through NPCs.
    for (k = 0, curNpc = g_SysWork.npcs;
         k < ARRAY_SIZE(g_SysWork.npcs);
         k++, curNpc++)
    {
        // Ignore invalid or special NPC.
        if (curNpc->model.charaId == Chara_None ||
            curNpc->model.charaId == Chara_Padlock)
        {
            continue;
        }

        // Only process enemy NPC for radio interference.
        if (curNpc->model.charaId <= CHARA_LAST_ENEMY_ID)
        {
            // Compute square integer distance from player to NPC.
            curDistToNpc = Q12_SQUARE_PRECISE(Q12_TO_Q6(curNpc->position.vx) - posXShift6) +
                           Q12_SQUARE_PRECISE(Q12_TO_Q6(curNpc->position.vz) - posZShift6);

            // Check if map is low-visibility interior.
            isLowVisInterior = false;
            if (g_MapOverlayHdr.mapInfo->flags & MapFlag_Interior)
            {
                isLowVisInterior = (g_MapOverlayHdr.mapInfo->flags & (MapFlag_OneActiveChunk |
                                                                      MapFlag_TwoActiveChunks)) > 0;
            }

            // Run through ???
            for (j = 0; j < 3; j++)
            {
                // Check if NPC is detectable by radio.
                if (curNpc->health <= Q12(0.0f)             ||
                    curNpc->flags & CharaFlag_NoRadioStatic ||
                    curDistToNpc >= closestNpcInfos[j].distanceToNpc)
                {
                    continue;
                }

                // Check if player and NPC occupy the same 4x4 chunk region in low-visibility interior map.
                if (isLowVisInterior)
                {
                    // Check X axis.
                    s32 playerChunk = (PLAYER_CHARA().position.vx + (Q12(CHUNK_SIZE) * 4)) / Q12(CHUNK_SIZE);
                    s32 npcChunk    = (curNpc->position.vx                     + (Q12(CHUNK_SIZE) * 4)) / Q12(CHUNK_SIZE);
                    if (npcChunk != playerChunk)
                    {
                        continue;
                    }

                    // Check Z axis.
                    playerChunk = (PLAYER_CHARA().position.vz + (Q12(CHUNK_SIZE) * 4)) / Q12(CHUNK_SIZE);
                    npcChunk    = (curNpc->position.vz                     + (Q12(CHUNK_SIZE) * 4)) / Q12(CHUNK_SIZE);
                    if (npcChunk != playerChunk)
                    {
                        continue;
                    }
                }

                for (m = ARRAY_SIZE(closestNpcInfos) - 1; j < m; m--)
                {
                    closestNpcInfos[m].bitIdx        = closestNpcInfos[m - 1].bitIdx;
                    closestNpcInfos[m].distanceToNpc = closestNpcInfos[m - 1].distanceToNpc;
                    closestNpcInfos[m].position.vx   = closestNpcInfos[m - 1].position.vx;
                    closestNpcInfos[m].position.vz   = closestNpcInfos[m - 1].position.vz;
                }

                closestNpcInfos[j].bitIdx        = curNpc - g_SysWork.npcs;
                closestNpcInfos[j].distanceToNpc = curDistToNpc;
                closestNpcInfos[j].position.vx   = curNpc->position.vx;
                closestNpcInfos[j].position.vz   = curNpc->position.vz;
                break;
            }

            // Unload distant NPC to avoid drawing.
            curDistToNpcCpy = curDistToNpc;
            if (curDistToNpcCpy > ((!isLowVisInterior && curNpc->health < Q12(0.0f)) ? SQUARE(24) : SQUARE(40)))
            {
                curNpc->model.charaId = Chara_None;

                SysWork_NpcFlagClear(k);
                CLEAR_FLAG(g_SysWork.field_228C, curNpc->field_40);
                continue;
            }

            // Set NPC visibility.
            if ((WorldEnv_IsFlashlightAllowed() && curDistToNpc > SQUARE(15)) ||
                (!WorldEnv_IsFlashlightAllowed() && Camera_Distance2dGet(&curNpc->position) > SQUARE(15)))
            {
                curNpc->model.anim.flags &= ~AnimFlag_Visible;
            }
            else
            {

                curNpc->model.anim.flags |= AnimFlag_Visible;
            }
        }

        curNpc->model.anim.flags |= AnimFlag_Unlocked;

        animDataIdx = g_CharaAnimDataIdxs[curNpc->model.charaId];
        boneCoords  = g_CharaModelAnimsData[animDataIdx].boneCoords;

        Chara_Flag8Clear(curNpc);
        Chara_DamagedFlagUpdate(curNpc);
        Collision_TriggerFlagsLocationUpdate(curNpc);

        g_MapOverlayHdr.charaUpdateFuncs[curNpc->model.charaId](curNpc, g_CharaModelAnimsData[animDataIdx].activeAnmHdr, boneCoords);

        Collision_TriggerFlagsUpdate();
        func_80037E78(curNpc);
        func_8008A3AC(curNpc);

        if (curNpc->model.anim.flags & AnimFlag_Visible)
        {
            WorldGfx_CharaDraw(curNpc->model.charaId, boneCoords, 1, curNpc->timer_C6, (s8)curNpc->model.paletteIdx);
        }
    }

    for (k = 2; k >= 0; k--)
    {
        if (closestNpcInfos[k].bitIdx != NO_VALUE)
        {
            break;
        }
    }

    g_RadioPitchState = k + 1;

    if (!(g_SavegamePtr->itemToggleFlags & ItemToggleFlag_RadioOn))
    {
        return;
    }

    idxBits = 0;

    for (l = 0; l < ARRAY_SIZE(g_RadioNoise); l++)
    {
        closeNpcInfoIdx1 = g_RadioNoise[l].idx;
        if (closeNpcInfoIdx1 == NO_VALUE)
        {
            closeNpcInfoIdx0 = NO_VALUE;
        }
        else
        {
            closeNpcInfoIdx0 = func_800382B0(closeNpcInfoIdx1);
        }

        if (closeNpcInfoIdx0 >= 0)
        {
            g_RadioNoise[l].closeNpcInfoIdx = closeNpcInfoIdx0;
            idxBits                        |= 1 << closeNpcInfoIdx1;
        }
        else
        {
            g_RadioNoise[l].idx = NO_VALUE;
        }
    }

    for (l = 0; l < ARRAY_SIZE(g_RadioNoise); l++)
    {
        idx = g_RadioNoise[l].idx;
        if (idx == NO_VALUE)
        {
            closeNpcInfoIdx = func_800382EC();
            if (closeNpcInfoIdx != idx)
            {
                bitIdx = closestNpcInfos[closeNpcInfoIdx].bitIdx;
            }
            else
            {
                bitIdx = NO_VALUE;
            }

            g_RadioNoise[l].closeNpcInfoIdx = closeNpcInfoIdx;
            g_RadioNoise[l].idx             = bitIdx;
        }
    }

    for (l = 0; l < ARRAY_SIZE(g_RadioNoise); l++)
    {
        if (g_RadioNoise[l].prevIdx == NO_VALUE)
        {
            if (g_RadioNoise[l].idx >= 0)
            {
                SD_Call((u16)(Sfx_RadioInterferenceLoop + l));
            }
        }
        else
        {
            isExterior = false;
            if (!(g_MapOverlayHdr.mapInfo->flags & MapFlag_Interior) ||
                !(g_MapOverlayHdr.mapInfo->flags & (MapFlag_OneActiveChunk | MapFlag_TwoActiveChunks)))
            {
                isExterior = true;
            }

            if (g_RadioNoise[l].idx >= 0)
            {
                closeNpcInfo = &closestNpcInfos[g_RadioNoise[l].closeNpcInfoIdx];
                balance      = Vc_StereoBalanceGet(&closeNpcInfo->position);

                // Compute volume from distance.
                distToNpc = Q12_TO_Q4(SquareRoot12(Q12(closeNpcInfo->distanceToNpc)));
                if (isExterior)
                {
                    distToNpc = DIV_FAST(distToNpc, 2);
                }

                // Compute clamped volume from Q4 distance. TODO: Is it Q4 here?
                vol = CLAMP(distToNpc, Q8(0.0f), Q8_CLAMPED(1.0f));

                Sd_SfxAttributesUpdate(Sfx_RadioInterferenceLoop + l, balance, vol, 0);
            }
            else
            {
                Sd_SfxStop(Sfx_RadioInterferenceLoop + l);
            }
        }

        g_RadioNoise[l].prevIdx = g_RadioNoise[l].idx;
    }
}

bool Math_Distance2dCheck(const VECTOR3* from, const VECTOR3* to, q19_12 radius) // 0x80038A6C
{
    q19_12 deltaX;
    q19_12 deltaZ;
    q19_12 radiusSqr;
    q19_12 sum;

    // Check rough radius intersection on X axis.
    deltaX = from->vx - to->vx;
    if (radius < deltaX)
    {
        return true;
    }
    if (radius < -deltaX)
    {
        return true;
    }

    // Check rough radius intersection on Z axis.
    deltaZ = from->vz - to->vz;
    if (radius < deltaZ)
    {
        return true;
    }
    if (radius < -deltaZ)
    {
        return true;
    }

    // Check distance.
    sum       = Q12_MULT_PRECISE(deltaX, deltaX) + Q12_MULT_PRECISE(deltaZ, deltaZ);
    radiusSqr = Q12_MULT_PRECISE(radius, radius);
    return sum > radiusSqr;
}

/** @brief Computes the squared 2D distance on the XZ plane from the reference position to the camera.
 *
 * @param pos Reference position (Q19.12).
 * @return 2D squared integer distance to the camera.
 */
static s32 Camera_Distance2dGet(const VECTOR3* pos) // 0x80038B44
{
    VECTOR3 camPos; // Q19.12
    q25_6   deltaX;
    q25_6   deltaZ;

    vwGetViewPosition(&camPos);
    deltaX = Q12_TO_Q6(camPos.vx - pos->vx);
    deltaZ = Q12_TO_Q6(camPos.vz - pos->vz);
    return Q12_MULT_PRECISE(deltaX, deltaX) + Q12_MULT_PRECISE(deltaZ, deltaZ);
}
