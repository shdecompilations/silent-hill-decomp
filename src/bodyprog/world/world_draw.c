#include "game.h"

#include <psyq/libetc.h>
#include <psyq/libpad.h>
#include <psyq/strings.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/gfx/map_effects.h"
#include "bodyprog/math/math.h"
#include "bodyprog/memcard.h"
#include "bodyprog/screen/screen_data.h"
#include "bodyprog/screen/screen_draw.h"
#include "bodyprog/sound/sound_system.h"
#include "bodyprog/text/text_draw.h"
#include "main/fsqueue.h"
#include "maps/characters/cybil.h"
#include "maps/characters/dahlia.h"
#include "maps/characters/kaufmann.h"
#include "maps/characters/puppet_nurse.h"
#include "maps/characters/split_head.h"
#include "maps/characters/stalker.h"

#ifndef PAD_HACK_IGNORE
    const s32 __pad_rodata_80025524 = 0;
    s16 __pad_800BCE14;
#endif

u16 g_CollisionTriggerFlags;
s_WorldGfxWork g_WorldGfxWork;


s32 Map_TypeGet(void) // 0x8003BD2C
{
    return g_WorldGfxWork.mapInfo - MAP_INFOS;
}

void Collision_FlagsLocationUpdate(const s_SubCharacter* chara) // 0x8003BD48
{
    u16 collFlags;

    g_CollisionTriggerFlags = Collision_FlagsGet();
    collFlags               = g_CollisionTriggerFlags;

    switch (Map_TypeGet())
    {
        case MapType_THR:
            if (chara->position.vx >= Q12(191.6f) && chara->position.vx <= Q12(198.8f) &&
                chara->position.vz >= Q12(-96.0f) && chara->position.vz <= Q12(-90.3f))
            {
                collFlags = (collFlags & ~CollisionTriggerFlag_Objects) | CollisionTriggerFlag_2;
            }
            break;

        case MapType_SPR:
        case MapType_SPU:
            if (chara->position.vx >= Q12(-100.0f) && chara->position.vx <= Q12(-94.5f) &&
                chara->position.vz >= Q12(-70.3f)  && chara->position.vz <= Q12(-62.0f))
            {
                collFlags = (collFlags & ~CollisionTriggerFlag_Objects) | CollisionTriggerFlag_2;
            }
    }

    Collision_FlagsSet(collFlags);
}

void Collision_FlagsUpdate(void) // 0x8003BE28
{
    Collision_FlagsSet(g_CollisionTriggerFlags);
}

// ========================================
// WORLD INITALIZATION
// ========================================

s_BoneNode* WorldGfx_CharaModelBoneNodesGet(e_CharaId charaId)
{
    return g_WorldGfxWork.registeredCharaModels[charaId]->skeleton.boneNodes;
}

void GameFs_BgEtcGfxLoad(void)
{
    static s_FsImageDesc IMAGE = { .tPage = { 0, 12 }, .clutX = 192 };

    Fs_QueueStartReadTim(FILE_TIM_BG_ETC_TIM, FS_BUFFER_1, &IMAGE);
}

void GameFs_BgItemLoad(void)
{
    g_WorldGfxWork.itemLmQueueIdx = Fs_QueueStartRead(FILE_BG_BG_ITEM_PLM, &g_WorldGfxWork.itemLmHdr);
}

static void GameFs_CommonItemsTextureLoad(void)
{
    static s_FsImageDesc IMAGE_TIM = {
        .tPage = { 0, 15 },
        .u     = 0,
        .v     = 0,
        .clutX = 176,
        .clutY = 0
    };

    static s_FsImageDesc IMAGE_ETC = {
        .tPage = { 0, 12 },
        .u     = 0,
        .v     = 192,
        .clutX = 192,
        .clutY = 0
    };

    s_LmHeader* itemLmHdr;

    itemLmHdr = &g_WorldGfxWork.itemLmHdr;

    if (!Fs_QueueIsEntryLoaded(g_WorldGfxWork.itemLmQueueIdx) || itemLmHdr->isLoaded)
    {
        return;
    }

    Lm_HeaderPtrsInit(&g_WorldGfxWork.itemLmHdr);
    Lm_MaterialFsImageApply1(&g_WorldGfxWork.itemLmHdr, "TIM00", &IMAGE_TIM, 1);
    Lm_MaterialFsImageApply1(&g_WorldGfxWork.itemLmHdr, "BG_ETC", &IMAGE_ETC, 1);
    Lm_MaterialFlagsApply(&g_WorldGfxWork.itemLmHdr);
}

s32 Map_SpeedZoneTypeGet(q19_12 posX, q19_12 posZ) // 0x8003BF60
{
    s32                zoneType;
    const s_SpeedZone* curZone;

    zoneType = SpeedZoneType_Slow;

    if (g_SavegamePtr->mapIdx == MapIdx_MAP0_S00)
    {
        return SpeedZoneType_Normal;
    }

    if (g_WorldGfxWork.mapInfo->speedZones != NULL)
    {
        curZone = g_WorldGfxWork.mapInfo->speedZones;
        while (curZone->type != NO_VALUE)
        {
            if (posX >= Q4_TO_Q12(curZone->minX) && Q4_TO_Q12(curZone->maxX) >= posX &&
                posZ >= Q4_TO_Q12(curZone->minZ) && Q4_TO_Q12(curZone->maxZ) >= posZ &&
                zoneType < curZone->type)
            {
                zoneType = curZone->type;
            }

            curZone++;
        }
    }

    return zoneType;
}

void World_Init(void) // 0x8003C048
{
    WorldEnv_Init();

    g_WorldGfxWork.useStoredPoint = false;

    WorldMap_Init(GLOBAL_LM_BUFFER, IPD_BUFFER, 0x2C000);
    Collision_Init();

    g_SysWork.lightIntensity = Q12(1.0f);

    Game_FlashlightAttributesFix();
    func_8005B55C(vwGetViewCoord());
    WorldObjects_Clear(&g_WorldGfxWork);
}

void WorldGfx_HeldItemModelFree(void) // 0x8003C0C0
{
    s_HeldItem* heldItem;

    heldItem = &g_WorldGfxWork.heldItem;

    heldItem->itemId                  = NO_VALUE;
    heldItem->lmHdr                   = HELD_ITEM_LM_BUFFER;
    heldItem->bone.modelInfo.field_0  = 0;
    heldItem->bone.modelInfo.coord    = NULL;
    heldItem->bone.modelInfo.modelHdr = NULL;
}

void WorldGfx_CharaModelsFree(void) // 0x8003C110
{
    s32           i;
    s_CharaModel* curModel;

    for (i = 0; i < Chara_Count; i++)
    {
        if (i != Chara_Harry)
        {
            g_WorldGfxWork.registeredCharaModels[i] = NULL;
        }
    }

    g_WorldGfxWork.charaLmBuffer = MAP_CHARA_LM_BUFFER;
    for (curModel = &g_WorldGfxWork.charaModels[0]; curModel < &g_WorldGfxWork.charaModels[4]; curModel++)
    {
        Chara_ModelFree(curModel);
    }
}

void Chara_ModelFree(s_CharaModel* model) // 0x8003C1AC
{
    s_FsImageDesc image = { 0 };

    model->charaId  = Chara_None;
    model->isLoaded = false;
    model->queueIdx = 0;
    model->lmHdr    = MAP_CHARA_LM_BUFFER;
    model->texture  = image;
}

// ========================================
// IPD INITALIZATION & MAP GRAPHIC DRAW
// ========================================

extern s_WorldEnvWork const g_WorldEnvWork;

void WorldGfx_MapInit(s_MapOverlayHdr* mapHdr, s32 playerPosX, s32 playerPosZ) // 0x8003C220
{
    s32        activeIpdCount;
    u8         flags;
    s_MapInfo* mapInfo;

    g_WorldGfxWork.mapInfo = mapHdr->mapInfo;

    flags = mapHdr->mapInfo->flags;
    if (flags & MapFlag_OneActiveChunk)
    {
        activeIpdCount = 1;
    }
    else if (flags & MapFlag_TwoActiveChunks)
    {
        activeIpdCount = 2;
    }
    else
    {
        activeIpdCount = 4;
    }

    mapInfo = mapHdr->mapInfo;
    WorldMap_InfoSet(mapInfo->tag, mapInfo->plmFileIdx, activeIpdCount, CHECK_FLAG(mapInfo->flags, MapFlag_Interior, false), 0, 0);

    if (mapHdr->mapInfo == &MAP_INFOS[MapType_THR])
    {
        WorldMap_ChunkSet(FILE_BG_THR05FD_IPD, -1, 8);
    }

    WorldMap_ChunkInit(playerPosX, playerPosZ, playerPosX, playerPosZ);
}

void Map_ActiveChunksClear(void) // 0x8003C2EC
{
    WorldMap_ActiveChunksClear();
}

void WorldGfx_MapReset(void) // 0x8003C30C
{
    u8 mapFlags;

    mapFlags = g_WorldGfxWork.mapInfo->flags;
    if ((mapFlags & MapFlag_Interior) && (mapFlags & (MapFlag_OneActiveChunk | MapFlag_TwoActiveChunks)))
    {
        WorldMap_Reset();
        return;
    }

    WorldMap_ActiveChunksClear();
    WorldMap_TexturesRefClear();
}

void WorldGfx_IpdSamplePointStore(void) // 0x8003C368
{
    g_WorldGfxWork.useStoredPoint = true;
    g_WorldGfxWork.ipdSamplePoint = g_SysWork.playerWork.player.position;
}

void WorldGfx_IpdSamplePointReset(void) // 0x8003C3A0
{
    g_WorldGfxWork.useStoredPoint = false;
}

void WorldGfx_CloseRangeChunksInit(void) // 0x8003C3AC
{
    #define HALF_CELL_SIZE Q12(2.5f) // TODO: 5x5 subcell thing, check this.
    #define PROJ_DIST_NEAR Q12(6.0f)
    #define PROJ_DIST_FAR  Q12(14.0f)

    VECTOR3         samplePos;
    VECTOR3         projPos;
    SVECTOR         camRot;  // Q19.12
    s32             alignedPosX;
    s32             alignedPosZ;
    q19_12          moveDist;
    q19_12          projOffsetX;
    q19_12          projOffsetZ;
    q19_12          stepsX;
    q19_12          stepsZ;
    q19_12          projDist;
    u8              mapFlags;
    s_SubCharacter* player;

    player = &g_SysWork.playerWork.player;

    if (g_WorldGfxWork.useStoredPoint)
    {
        samplePos = g_WorldGfxWork.ipdSamplePoint;
    }
    else
    {
        samplePos = player->position;
    }

    // Offset sample position.
    moveDist      = (player->moveSpeed * Q12(5.5f)) / Q12(3.91f);
    moveDist      = CLAMP(moveDist, Q12(0.0f), Q12(5.5f));
    samplePos.vx += Q12_MULT_PRECISE(moveDist, Math_Sin(player->headingAngle));
    samplePos.vz += Q12_MULT_PRECISE(moveDist, Math_Cos(player->headingAngle));

    // Special case for THR map.
    if (g_WorldGfxWork.mapInfo == &MAP_INFOS[MapType_THR] &&
        player->position.vx >= Q12(-40.0f) && player->position.vx <= Q12(40.0f) &&
        player->position.vz >= Q12(200.0f) && player->position.vz <= Q12(240.0f))
    {
        samplePos.vz = Q12(200.0f);
    }

    if (g_WorldEnvWork.isFogEnabled)
    {
        vwGetViewPosition(&projPos);
        vwGetViewAngle(&camRot);

        // Compute base projected distance based on map visibility.
        mapFlags = g_WorldGfxWork.mapInfo->flags;
        if (!(mapFlags & MapFlag_Interior) || !(mapFlags & (MapFlag_OneActiveChunk | MapFlag_TwoActiveChunks)))
        {
            projDist = Q12_MULT(Math_Cos(camRot.vx), Q12(9.0f));
        }
        else
        {
            projDist = Q12(0.0f);
        }

        // Compute projected position.
        projOffsetX = Q12_MULT(projDist, Math_Sin(camRot.vy));
        projOffsetX = CLAMP(projOffsetX, -PROJ_DIST_NEAR, PROJ_DIST_NEAR);
        projOffsetZ = Q12_MULT(projDist, Math_Cos(camRot.vy));
        projOffsetZ = CLAMP(projOffsetZ, -PROJ_DIST_NEAR, PROJ_DIST_NEAR);
        projPos.vx += projOffsetX;
        projPos.vz += projOffsetZ;

        // Limit projection if too far away.
        if (Vc_VectorMagnitudeCalc(projPos.vx - player->position.vx,
                                   Q12(0.0f),
                                   projPos.vz - player->position.vz) > Q12(16.0f))
        {
            projDist   = PROJ_DIST_FAR;
            projPos.vx = player->position.vx + Q12_MULT(Math_Sin(camRot.vy), projDist);
            projPos.vz = player->position.vz + Q12_MULT(Math_Cos(camRot.vy), projDist);
        }
    }
    else
    {
        projPos     = player->position;
        projPos.vx += FP_FROM(Q12(Math_Sin(player->rotation.vy)), Q12_SHIFT);
        projPos.vz += FP_FROM(Q12(Math_Cos(player->rotation.vy)), Q12_SHIFT);
    }

    mapFlags = g_WorldGfxWork.mapInfo->flags;
    if ((mapFlags & MapFlag_Interior) && (mapFlags & (MapFlag_OneActiveChunk | MapFlag_TwoActiveChunks)))
    {
        // Compute aligned X position.
        stepsX = player->position.vx / HALF_CELL_SIZE;
        if (player->position.vx < Q12(0.0f))
        {
            stepsX--;
        }

        // Compute aligned Z position.
        stepsZ      = player->position.vz / HALF_CELL_SIZE;
        alignedPosX = stepsX * HALF_CELL_SIZE;
        if (player->position.vz < Q12(0.0f))
        {
            stepsZ--;
        }

        alignedPosZ = stepsZ * HALF_CELL_SIZE;

        samplePos.vx = CLAMP(samplePos.vx, alignedPosX + 1, alignedPosX + (HALF_CELL_SIZE - 1));
        samplePos.vz = CLAMP(samplePos.vz, alignedPosZ + 1, alignedPosZ + (HALF_CELL_SIZE - 1));
        projPos.vx   = CLAMP(projPos.vx,   alignedPosX + 1, alignedPosX + (HALF_CELL_SIZE - 1));
        projPos.vz   = CLAMP(projPos.vz,   alignedPosZ + 1, alignedPosZ + (HALF_CELL_SIZE - 1));
    }

    WorldMap_ChunkInit(samplePos.vx, samplePos.vz, projPos.vx, projPos.vz);

    #undef HALF_CELL_SIZE
    #undef PROJ_DIST_NEAR
    #undef PROJ_DIST_FAR
}

bool WorldGfx_ChunkInitCheck(void) // 0x8003C850
{
    WorldGfx_CloseRangeChunksInit();
    return WorldMap_ActiveModelsLoadStateCheck();
}

void WorldGfx_Draw(s32 otShift) // 0x8003C878
{
    // Draw world objects.
    WorldObjects_DrawAllObjects(&g_WorldGfxWork);

    // Load and draw active chunks.
    while (WorldMap_NextChunkLoadCheck())
    {
        WorldGfx_CloseRangeChunksInit();
        Fs_QueueWaitForEmpty();
    }
    WorldMap_ChunksDraw(&g_OrderingTable0[g_ActiveBufferIdx], otShift);

    // Draw 2D screen effects.
    WorldGfx_2dEffectsDraw();
}

// ========================================
// OBJECTS DRAW
// ========================================

void WorldObject_ModelNameSet(s_WorldObjectModel* model, char* name) // 0x8003C8F8
{
    model->metadata.modelLocation = WorldModelLocation_None;
    model->modelInfo.field_0      = 0;

    StringCopy(model->metadata.name.str, name);

    model->metadata.field_8 = 0;
}

void WorldObjects_Add(s_WorldObjectModel* model, const VECTOR3* pos, const SVECTOR3* rot) // 0x8003C92C
{
    q23_8          posX;
    q23_8          posY;
    q23_8          posZ;
    q21_10         rotX;
    q19_12         rotZ;
    q21_10         rotY;
    s32            i;
    s32            modelLoc;
    s_WorldObject* obj;

    // Submit world object if a slot is available.
    if (g_WorldGfxWork.objectCount < ARRAY_SIZE(g_WorldGfxWork.objects))
    {
        if (model->metadata.modelLocation == WorldModelLocation_None)
        {
            GameFs_CommonItemsTextureLoad();

            modelLoc = WorldMap_ObjectModelLocationGet(model, &model->metadata,
                                                       g_SysWork.playerWork.player.position.vx,
                                                       g_SysWork.playerWork.player.position.vz);
            if (modelLoc == WorldModelLocation_None)
            {
                if (Lm_ModelFind(model, &g_WorldGfxWork.itemLmHdr, &model->metadata))
                {
                    modelLoc = WorldModelLocation_Global;
                }
                else
                {
                    return;
                }
            }

            model->metadata.modelLocation = modelLoc;
        }

        // Compute geometry position and rotation.
        posX = Q12_TO_Q8(pos->vx);
        posY = Q12_TO_Q8(pos->vy);
        posZ = Q12_TO_Q8(pos->vz);
        rotX = Q12_TO_Q10(rot->vx);
        rotZ = Q12_TO_Q10(rot->vz);
        rotY = rot->vy;

        // Check if world object to draw was already submitted?
        for (i = 0; i < g_WorldGfxWork.objectCount; i++)
        {
            obj = &g_WorldGfxWork.objects[i];

            if (model == obj->model     &&
                posX  == obj->positionX &&
                posZ  == obj->positionZ &&
                posY  == obj->positionY &&
                rotX  == obj->rotationX &&
                rotY  == obj->rotationY &&
                rotZ  == obj->rotationZ)
            {
                return;
            }
        }

        // Submit new world object to draw.
        obj            = &g_WorldGfxWork.objects[g_WorldGfxWork.objectCount];
        obj->rotationX = rotX;
        obj->rotationY = rotY;

        if (obj->positionZ) {} // @hack Required for match.

        obj->rotationZ = rotZ;
        obj->model     = model;
        obj->positionX = posX;
        obj->positionY = posY;
        obj->positionZ = posZ;

        g_WorldGfxWork.objectCount++;
    }
}

void WorldObjects_Clear(s_WorldGfxWork* worldGfxWork) // 0x8003CB3C
{
    worldGfxWork->objectCount = 0;
}

void WorldObjects_DrawAllObjects(s_WorldGfxWork* worldGfxWork) // 0x8003CB44
{
    s_WorldObject* curObj;

    // Run through world objects to draw.
    for (curObj = &worldGfxWork->objects[0]; curObj < &worldGfxWork->objects[worldGfxWork->objectCount]; curObj++)
    {
        WorldObjects_Draw(curObj);
    }

    worldGfxWork->objectCount = 0;
}

void WorldObjects_Draw(s_WorldObject* obj) // 0x8003CBA4
{
    GsCOORDINATE2 coord;
    SVECTOR       rot; // Q3.12
    MATRIX        viewMat;
    MATRIX        worldMat;

    coord.flg   = false;
    coord.super = NULL;

    // Set geometry position.
    coord.coord.t[0] = obj->positionX;
    coord.coord.t[1] = obj->positionY;
    coord.coord.t[2] = obj->positionZ;

    // Compute world rotation.
    rot.vx = Q10_TO_Q12(obj->rotationX);
    rot.vy = obj->rotationY;
    rot.vz = Q10_TO_Q12(obj->rotationZ);

    Math_RotMatrixZxyNeg(&rot, &coord.coord);
    Vw_CoordToWorldAndViewMatrices(&coord, &worldMat, &viewMat);
    WorldObjects_DrawStep(obj->model, &viewMat, &worldMat);
}

void WorldObjects_DrawStep(s_WorldObjectModel* model, MATRIX* viewMat, MATRIX* worldMat) // 0x8003CC7C
{
    s8                     modelLoc;
    s_WorldObjectMetadata* objMetaCpy;
    s_ModelHeader*         modelHdr;

    modelLoc = model->metadata.modelLocation;
    if (modelLoc == WorldModelLocation_None)
    {
        return;
    }

    modelHdr   = model->modelInfo.modelHdr;
    objMetaCpy = &model->metadata;

    if (modelLoc >= WorldModelLocation_Chunk2 && modelLoc <= WorldModelLocation_Unk6)
    {
        if (WorldMap_ActiveChunkLoadedCheck(modelLoc - 3) == WorldModelLocation_None)
        {
            model->metadata.modelLocation = WorldModelLocation_None;
        }
    }

    if (COMPARE_FILENAMES(&objMetaCpy->name, &modelHdr->name))
    {
        model->metadata.modelLocation = WorldModelLocation_None;
        return;
    }

    func_80057090(&model->modelInfo, &g_OrderingTable0[g_ActiveBufferIdx], 1, viewMat, worldMat, 0);
}

// ========================================
// HELD ITEM DRAW
// ========================================

s32 WorldGfx_HeldItemIdGet(void) // 0x8003CD5C
{
    return g_WorldGfxWork.heldItem.itemId;
}

s32 WorldGfx_PlayerPrevHeldItem(s_PlayerCombat* combat) // 0x8003CD6C
{
    s32 itemId;
    s8  weaponAttack;

    weaponAttack = combat->weaponAttack;
    itemId       = NO_VALUE;
    if (weaponAttack != NO_VALUE)
    {
        itemId = weaponAttack + InvItemId_KitchenKnife;
    }

    return WorldGfx_PlayerHeldItemSet(itemId);
}

s32 WorldGfx_PlayerHeldItemSet(e_InvItemId itemId) // 0x8003CDA0
{
    s32         fileIdx;
    s_HeldItem* heldItem;

    // Check if requested item is already being held.
    heldItem = &g_WorldGfxWork.heldItem;
    if (heldItem->itemId == itemId)
    {
        return 0;
    }

    // Define texture files to load.
    heldItem->itemId = itemId;
    switch (itemId)
    {
        default:
            fileIdx = NO_VALUE;
            break;

        case NO_VALUE:
        case InvItemId_KitchenKnife:
        case InvItemId_Hammer:
        case InvItemId_Axe:
        case InvItemId_Handgun:
        case InvItemId_HuntingRifle:
        case InvItemId_Shotgun:
            fileIdx               = NO_VALUE;
            heldItem->textureName = "HERO";
            break;

        case InvItemId_SteelPipe:
            fileIdx               = FILE_ITEM_PIPE_TIM;
            heldItem->textureName = "PIPE";
            break;

        case InvItemId_CutscenePhone:
            fileIdx               = FILE_ITEM_PHONE_TIM;
            heldItem->textureName = "PHONE";
            break;

        case InvItemId_CutsceneFlauros:
            fileIdx               = FILE_ITEM_FLAUROS_TIM;
            heldItem->textureName = "FLAUROS";
            break;

        case InvItemId_CutsceneAglaophotis:
            fileIdx               = FILE_ITEM_AGLA_TIM;
            heldItem->textureName = "AGLA";
            break;

        case InvItemId_CutscenePlasticBottle:
            fileIdx               = FILE_ITEM_BOTL_TIM;
            heldItem->textureName = "BOTL";
            break;

        case InvItemId_CutsceneBaby:
            fileIdx               = FILE_ITEM_BABY_TIM;
            heldItem->textureName = "BABY";
            break;

        case InvItemId_CutsceneBloodPack:
            fileIdx               = FILE_ITEM_BLOOD_TIM;
            heldItem->textureName = "BLOOD";
            break;

        case InvItemId_Chainsaw:
            fileIdx               = FILE_ITEM_CSAW_TIM;
            heldItem->textureName = "CSAW";
            break;

        case InvItemId_HyperBlaster:
            fileIdx               = FILE_ITEM_HPRGUN_TIM;
            heldItem->textureName = "HPRGUN";
            break;

        case InvItemId_RockDrill:
            fileIdx               = FILE_ITEM_DRILL_TIM;
            heldItem->textureName = "DRILL";
            break;

        case InvItemId_Katana:
            fileIdx               = FILE_ITEM_KATANA_TIM;
            heldItem->textureName = "KATANA";
            break;
    }

    // Set texture UVs.
    if (fileIdx == NO_VALUE)
    {
        heldItem->imageDesc.tPage[1] = 27;
        heldItem->imageDesc.tPage[0] = 0;
        heldItem->imageDesc.u        = 0;
        heldItem->imageDesc.v        = 0;
        heldItem->imageDesc.clutX    = 736;
        heldItem->imageDesc.clutY    = 480;
    }
    else
    {
        heldItem->imageDesc.tPage[1] = 27;
        heldItem->imageDesc.tPage[0] = 0;
        heldItem->imageDesc.u        = 48;
        heldItem->imageDesc.v        = 224;
        heldItem->imageDesc.clutX    = 736;
        heldItem->imageDesc.clutY    = 498;
    }

    // Load texture.
    if (fileIdx != NO_VALUE)
    {
        heldItem->queueIdx = Fs_QueueStartReadTim(fileIdx, FS_BUFFER_10, &heldItem->imageDesc);
    }

    // Define index of model file to load.
    switch (itemId)
    {
        case NO_VALUE:
        default:
            fileIdx = NO_VALUE;
            break;

        case InvItemId_KitchenKnife:
            fileIdx = FILE_ITEM_KNIFE_PLM;
            break;

        case InvItemId_SteelPipe:
            fileIdx = FILE_ITEM_PIPE_PLM;
            break;

        case InvItemId_Hammer:
            fileIdx = FILE_ITEM_HAMMER_PLM;
            break;

        case InvItemId_Axe:
            fileIdx = FILE_ITEM_AXE_PLM;
            break;

        case InvItemId_Handgun:
            fileIdx = FILE_ITEM_HANDGUN_PLM;
            break;

        case InvItemId_HuntingRifle:
            fileIdx = FILE_ITEM_RIFLE_PLM;
            break;

        case InvItemId_Shotgun:
            fileIdx = FILE_ITEM_SHOTGUN_PLM;
            break;

        case InvItemId_CutscenePhone:
            fileIdx = FILE_ITEM_PHONE_PLM;
            break;

        case InvItemId_CutsceneFlauros:
            fileIdx = FILE_ITEM_FLAUROS_PLM;
            break;

        case InvItemId_CutsceneAglaophotis:
            fileIdx = FILE_ITEM_AGLA_PLM;
            break;

        case InvItemId_CutscenePlasticBottle:
            fileIdx = FILE_ITEM_BOTL_PLM;
            break;

        case InvItemId_CutsceneBaby:
            fileIdx = FILE_ITEM_BABY_PLM;
            break;

        case InvItemId_CutsceneBloodPack:
            fileIdx = FILE_ITEM_BLOOD_PLM;
            break;

        case InvItemId_Chainsaw:
            fileIdx = FILE_ITEM_CSAW_PLM;
            break;

        case InvItemId_HyperBlaster:
            fileIdx = FILE_ITEM_HPRGUN_PLM;
            break;

        case InvItemId_RockDrill:
            fileIdx = FILE_ITEM_DRILL_PLM;
            break;

        case InvItemId_Katana:
            fileIdx = FILE_ITEM_KATANA_PLM;
            break;
    }

    // Load model.
    if (fileIdx != NO_VALUE)
    {
        heldItem->queueIdx = Fs_QueueStartRead(fileIdx, heldItem->lmHdr);
        return heldItem->queueIdx;
    }

    return 0;
}

void func_8003D01C(void) // 0x8003D01C
{
    g_WorldGfxWork.heldItem.bone.modelInfo.field_0 &= ~(1 << 31);
}

void func_8003D03C(void) // 0x8003D03C
{
    g_WorldGfxWork.heldItem.bone.modelInfo.field_0 |= 1 << 31;
}

void WorldGfx_HeldItemDraw(void) // 0x8003D058
{
    MATRIX         viewMat;
    MATRIX         worldMat;
    GsCOORDINATE2* coord;
    s_HeldItem*    heldItem;
    s_LmHeader*    lmHdr;

    // Check if held item is valid.
    heldItem = &g_WorldGfxWork.heldItem;
    if (heldItem->itemId == NO_VALUE)
    {
        return;
    }

    // Distinguish between left-handed and right-handed items.
    if (heldItem->itemId == InvItemId_CutscenePhone)
    {
        coord = &g_SysWork.playerBoneCoords[HarryBone_LeftHand];
    }
    else
    {
        coord = &g_SysWork.playerBoneCoords[HarryBone_RightHand];
    }

    if (Fs_QueueIsEntryLoaded(heldItem->queueIdx))
    {
        lmHdr = heldItem->lmHdr;
        if (!lmHdr->isLoaded)
        {
            Lm_HeaderPtrsInit(lmHdr);
            Lm_MaterialFsImageApply1(lmHdr, heldItem->textureName, &heldItem->imageDesc, BlendMode_Additive);
            Lm_MaterialFlagsApply(lmHdr);
            Bone_ModelAssign(&heldItem->bone, heldItem->lmHdr, 0);
        }

        Vw_CoordToWorldAndViewMatrices(coord, &worldMat, &viewMat);
        func_80057090(&heldItem->bone.modelInfo, &g_OrderingTable0[g_ActiveBufferIdx], 1, &viewMat, &worldMat, 0);
    }
}

// ========================================
// CHARACTER LOAD AND DRAW
// ========================================

void WorldGfx_HarryCharaLoad(void) // 0x8003D160
{
    s_FsImageDesc   image;
    s32             queueIdx;
    s_WorldGfxWork* worldGfxWork;
    s_CharaModel*   harryModel;
    s_LmHeader*     harryLmHdr;

    harryLmHdr = HARRY_LM_BUFFER;

    Chara_FsImageCalc(&image, Chara_Harry, 0);

    worldGfxWork                                      = &g_WorldGfxWork;
    harryModel                                        = &worldGfxWork->harryModel;
    g_WorldGfxWork.registeredCharaModels[Chara_Harry] = harryModel;

    Fs_QueueStartRead(CHARA_FILE_INFOS[Chara_Harry].modelFileIdx, harryLmHdr);
    queueIdx = Fs_QueueStartReadTim(CHARA_FILE_INFOS[Chara_Harry].textureFileIdx, FS_BUFFER_1, &image);

    g_WorldGfxWork.harryModel.charaId = Chara_Harry;
    harryModel->isLoaded              = false;
    harryModel->queueIdx              = queueIdx;
    harryModel->lmHdr                 = harryLmHdr;
    g_WorldGfxWork.harryModel.texture = image;
}

s32 WorldGfx_MapInitCharaLoad(s_MapOverlayHdr* mapHdr) // 0x8003D21C
{
    s_FsImageDesc image;
    bool          cond;
    s32           i;
    s32           j;
    s32           queueIdx;
    s32           curCharaId;
    s_CharaModel* curModel;

    for (queueIdx = 0, i = 0, g_WorldGfxWork.charaLmBuffer = MAP_CHARA_LM_BUFFER, cond = false;
         i < CHARA_GROUP_COUNT;
         i++)
    {
        curCharaId = mapHdr->charaGroupIds[i];
        curModel   = &g_WorldGfxWork.charaModels[i];

        if (curCharaId != Chara_None)
        {
            if (!cond)
            {
                if (curCharaId != curModel->charaId)
                {
                    cond = true;
                    for (j = i; j < ARRAY_SIZE(g_WorldGfxWork.charaModels); j++)
                    {
                        g_WorldGfxWork.charaModels[j].charaId = Chara_None;
                    }
                }
            }

            if (cond)
            {
                Chara_FsImageCalc(&image, curCharaId, i);
                queueIdx = WorldGfx_CharaModelLoad(curCharaId, i, (s_LmHeader*)g_WorldGfxWork.charaLmBuffer, &image);
            }

            WorldGfx_CharaLmBufferAdvance(&g_WorldGfxWork.charaLmBuffer, curCharaId);
        }
    }

    return queueIdx;
}

void WorldGfx_CharaLmBufferAdvance(u8** buf, e_CharaId charaId) // 0x8003D354
{
    s16 modelFileIdx;
    s32 fileSize;

    modelFileIdx = CHARA_FILE_INFOS[charaId].modelFileIdx;
    fileSize     = Fs_GetFileSize(modelFileIdx);

    Fs_GetFileSectorAlignedSize(modelFileIdx);
    *buf += (fileSize + 3) & ~0x3;
}

void Chara_FsImageCalc(s_FsImageDesc* image, s32 charaId, s32 modelIdx) // 0x8003D3BC
{
    s16 clutX;
    s16 clutY;
    s8  tPage;
    s8  v;
    s8  u;

    // TODO: Deoptimise.
    v = charaId < Chara_AirScreamer;
    if (charaId >= Chara_None && v)
    {
        tPage = 27;
        v     = 0;
        u     = 0;
        clutX = 736;
        clutY = 480;
    }
    else
    {
        clutY = 464;
        clutX = (modelIdx * 16) + 704;

        switch (modelIdx)
        {
            default:
                modelIdx = 0;

            case 0:
            case 1:
                tPage = 28;
                u     = 0;
                v     = modelIdx << 7;
                break;

            case 2:
            case 3:
                tPage = 29;
                u     = 0;
                v     = (modelIdx - 2) << 7;
                break;
        }
    }

    image->tPage[0] = 0;
    image->tPage[1] = tPage;
    image->u        = u;
    image->v        = v;
    image->clutX    = clutX;
    image->clutY    = clutY;
}

bool WorldGfx_IsCharaModelPresent(e_CharaId charaId) // 0x8003D444
{
    return g_WorldGfxWork.registeredCharaModels[charaId] != NULL;
}

void func_8003D460(void) {} // 0x8003D460

void WorldGfx_CharaModelTransparentSet(e_CharaId charaId, bool isTransparent) // 0x8003D468
{
    s16           data[256];
    RECT          rect;
    s32           x;
    s32           i;
    s32           y;
    s_CharaModel* model;

    model = g_WorldGfxWork.registeredCharaModels[charaId];
    Lm_TransparentPrimSet(model->lmHdr, isTransparent);

    rect.x = model->texture.clutX;
    rect.y = model->texture.clutY;
    rect.w = 16;
    rect.h = 16;

    DrawSync(SyncMode_Wait);
    StoreImage(&rect, &data);

    for (y = 0, i = 0; y < 16; y++)
    {
        for (x = 0; x < 16; x++, i++)
        {
            if (!isTransparent)
            {
                data[i] &= SHRT_MAX;
            }
            else
            {
                data[i] |= 1 << 15;
            }
        }
    }

    LoadImage(&rect, &data);
}

void WorldGfx_CharaModelMaterialSet(s32 charaId, s32 blendMode) // 0x8003D550
{
    s_CharaModel* model;

    model = g_WorldGfxWork.registeredCharaModels[charaId];
    Lm_MaterialFileIdxApply(model->lmHdr, CHARA_FILE_INFOS[charaId].textureFileIdx, &model->texture, blendMode);
    Lm_MaterialFlagsApply(model->lmHdr);
}

void WorldGfx_CharaLmBufferAssign(s8 forceFree) // 0x8003D5B4
{
    u8            charaId;
    s32           i;
    u32           lmData;
    s_CharaModel* curModel;

    for (i = 0; i < ARRAY_SIZE(g_WorldGfxWork.charaModels); i++)
    {
        curModel = &g_WorldGfxWork.charaModels[i];
        if ((forceFree >> i) & (1 << 0))
        {
            WorldGfx_CharaFree(curModel);
        }
    }

    i = 0;
    g_WorldGfxWork.charaLmBuffer = MAP_CHARA_LM_BUFFER;
    for (; i < ARRAY_SIZE(g_WorldGfxWork.charaModels); i++)
    {
        curModel = &g_WorldGfxWork.charaModels[i];

        charaId = curModel->charaId;
        if (charaId != Chara_None)
        {
            lmData = (s32)curModel->lmHdr + Fs_GetFileSize(CHARA_FILE_INFOS[charaId].modelFileIdx);
            if (g_WorldGfxWork.charaLmBuffer < lmData)
            {
                g_WorldGfxWork.charaLmBuffer = lmData;
            }
        }
    }
}

void WorldGfx_CharaFree(s_CharaModel* model) // 0x8003D6A4
{
    if (model->charaId != Chara_None)
    {
        g_WorldGfxWork.registeredCharaModels[model->charaId] = NULL;
        Chara_ModelFree(model);
    }
}

void WorldGfx_CharaLoad(e_CharaId charaId, s32 modeIdx, s_LmHeader* lmHdr, s_FsImageDesc* tex) // 0x8003D6E0
{
    s_FsImageDesc image;
    s_LmHeader*   lmHdrBuf;

    // Set LM header.
    if (lmHdr != NULL)
    {
        lmHdrBuf = lmHdr;
    }
    else if (g_WorldGfxWork.charaModels[modeIdx].charaId != Chara_None)
    {
        lmHdrBuf = g_WorldGfxWork.charaModels[modeIdx].lmHdr;
    }
    else
    {
        lmHdrBuf = (s_LmHeader*)g_WorldGfxWork.charaLmBuffer;
        WorldGfx_CharaLmBufferAdvance(&g_WorldGfxWork.charaLmBuffer, charaId);
    }

    // Set texture UVs.
    if (tex != NULL)
    {
        image = *tex;
    }
    else
    {
        Chara_FsImageCalc(&image, charaId, modeIdx);
    }

    WorldGfx_CharaModelLoad(charaId, modeIdx, lmHdrBuf, &image);
}

s32 WorldGfx_CharaModelLoad(e_CharaId charaId, s32 modelIdx, s_LmHeader* lmHdr, s_FsImageDesc* tex) // 0x8003D7D4
{
    s32            queueIdx;
    s32            modelCharaId; // `e_CharaId`
    s_CharaModel*  model;
    s_FsImageDesc* modelTex;

    model        = &g_WorldGfxWork.charaModels[modelIdx];
    modelCharaId = model->charaId;
    modelTex     = &model->texture;

    // If character is invalid, set model as unused.
    if (charaId == Chara_None)
    {
        g_WorldGfxWork.registeredCharaModels[modelCharaId] = NULL;
        return 0;
    }

    // Register character model.
    if (modelCharaId != Chara_None)
    {
        if (charaId == modelCharaId)
        {
            if (lmHdr == model->lmHdr && memcmp(tex, modelTex, sizeof(s_FsImageDesc)) == 0)
            {
                return 0;
            }
        }

        g_WorldGfxWork.registeredCharaModels[modelCharaId] = NULL;
    }
    g_WorldGfxWork.registeredCharaModels[charaId] = model;

    // Load model and texture files.
    queueIdx = Fs_QueueStartRead(CHARA_FILE_INFOS[charaId].modelFileIdx, lmHdr);
    if (CHARA_FILE_INFOS[charaId].textureFileIdx != NO_VALUE)
    {
        queueIdx = Fs_QueueStartReadTim(CHARA_FILE_INFOS[charaId].textureFileIdx, FS_BUFFER_1, tex);
    }

    // Set loaded model data.
    model->charaId  = charaId;
    model->isLoaded = false;
    model->queueIdx = queueIdx;
    model->lmHdr    = lmHdr;
    model->texture  = *tex;
    return queueIdx;
}

void WorldGfx_PlayerModelProcessLoad(void) // 0x8003D938
{
    WorldGfx_CharaModelProcessLoad(&g_WorldGfxWork.harryModel);
}

void WorldGfx_CharaModelProcessAllLoads(void) // 0x8003D95C
{
    s32           i;
    s_CharaModel* model;

    for (i = 0; i < Chara_Count; i++)
    {
        if (i != Chara_Harry)
        {
            model = g_WorldGfxWork.registeredCharaModels[i];
            if (model != NULL)
            {
                WorldGfx_CharaModelProcessLoad(model);
            }
        }
    }
}

void WorldGfx_CharaModelProcessLoad(s_CharaModel* model) // 0x8003D9C8
{
    s_Skeleton* skel;

    if (!model->isLoaded && model->charaId != Chara_None && Fs_QueueIsEntryLoaded(model->queueIdx))
    {
        model->isLoaded = true;

        Lm_HeaderPtrsInit(model->lmHdr);
        Lm_MaterialFileIdxApply(model->lmHdr, CHARA_FILE_INFOS[model->charaId].textureFileIdx, &model->texture, CHARA_FILE_INFOS[model->charaId].materialBlendMode % 4);

        skel = &model->skeleton;

        Lm_MaterialFlagsApply(model->lmHdr);
        Skeleton_Init(skel, model->skeleton.boneNodes, BONE_NODE_COUNT_MAX);
        func_8004506C(skel, model->lmHdr);
        func_800452EC(skel);
        func_800453E8(skel, true);
    }
}

void WorldGfx_CharaDraw(e_CharaId charaId, GsCOORDINATE2* boneCoords, s32 otShift, q3_12 timer, s32 paletteIdx) // 0x8003DA9C
{
    CVECTOR tintColor = { 0 };
    s16     clutY;

    // Check if character is invalid.
    if (charaId == Chara_None)
    {
        return;
    }

    timer = CLAMP(timer, Q12(0.0f), Q12(1.0f));

    // Something to do with items held by player.
    if (charaId == Chara_Harry)
    {
        WorldGfx_HeldItemDraw();
    }

    clutY = WorldGfx_CharaClutYGet(charaId, paletteIdx);

    if (timer != Q12(0.0f))
    {
        tintColor = g_WorldEnvWork.worldTintColor;
        WorldEnv_WorldLightingParamSet(g_WorldEnvWork.field_0, g_WorldEnvWork.field_20, g_WorldEnvWork.field_3,
                      Q12_MULT_PRECISE(Q12(1.0f) - timer, g_WorldEnvWork.worldTintColor.r) << 5,
                      Q12_MULT_PRECISE(Q12(1.0f) - timer, g_WorldEnvWork.worldTintColor.g) << 5,
                      Q12_MULT_PRECISE(Q12(1.0f) - timer, g_WorldEnvWork.worldTintColor.b) << 5,
                      g_WorldEnvWork.screenBrightness);
    }

    func_80045534(&g_WorldGfxWork.registeredCharaModels[charaId]->skeleton, &g_OrderingTable0[g_ActiveBufferIdx], otShift,
                  boneCoords, Q8_TO_Q12(CHARA_FILE_INFOS[charaId].field_6), clutY, CHARA_FILE_INFOS[charaId].field_8);

    if (timer != Q12(0.0f))
    {
        WorldEnv_WorldLightingParamSet(g_WorldEnvWork.field_0, g_WorldEnvWork.field_20, g_WorldEnvWork.field_3,
                      tintColor.r << 5,
                      tintColor.g << 5,
                      tintColor.b << 5,
                      g_WorldEnvWork.screenBrightness);
    }
}

s32 WorldGfx_CharaClutYGet(e_CharaId charaId, s32 paletteIdx) // 0x8003DD74
{
    return (paletteIdx << 10) & 0xFC00;
}

// ========================================
// MESH SWAP
// ========================================

void WorldGfx_CharaMeshSwap(e_CharaId charaId, s32 meshSwapStatus) // 0x8003DD80
{
    s_CharaModel* model;

    model = g_WorldGfxWork.registeredCharaModels[charaId];

    switch (charaId)
    {
        case Chara_Harry:
            WorldGfx_HarryMeshSwap(&model->skeleton, meshSwapStatus);
            break;

        case Chara_Stalker:
            WorldGfx_StalkerMeshSwap(&model->skeleton, meshSwapStatus);
            break;

        case Chara_Cybil:
        case Chara_EndingCybil:
            WorldGfx_CybilMeshSwap(&model->skeleton, meshSwapStatus);
            break;

        case Chara_MonsterCybil:
            WorldGfx_MonsterCybilMeshSwap(&model->skeleton, meshSwapStatus);
            break;

        case Chara_Dahlia:
        case Chara_EndingDahlia:
            WorldGfx_DahliaMeshSwap(&model->skeleton, meshSwapStatus);
            break;

        case Chara_Kaufmann:
        case Chara_EndingKaufmann:
            WorldGfx_KaufmannMeshSwap(&model->skeleton, meshSwapStatus);
            break;

        case Chara_SplitHead:
            WorldGfx_SplitHeadMeshSwap(&model->skeleton, meshSwapStatus);
            break;

        case Chara_PuppetNurse:
            WorldGfx_PuppetNurseMeshSwap(&model->skeleton, meshSwapStatus);
            break;

        case Chara_PuppetDoctor:
            WorldGfx_PuppetDoctorMeshSwap(&model->skeleton, meshSwapStatus);
            break;
    }
}

void WorldGfx_HarryMeshSwap(s_Skeleton* skel, s32 meshSwapStatus) // 0x8003DE60
{
    s32 variantId;
    s32 swappableId;

    static s32 D_800A9ECC = 0xFE16FD13;
    static s32 D_800A9ED0 = 0x0000FE13;
    static s32 D_800A9ED4 = 0x0000FE14;
    static s32 D_800A9ED8 = 0x0000FE15;
    static s32 D_800A9EDC = 0x0000FE16;
    static s32 D_800A9EE0 = 0x000000FE;
    static s32 D_800A9EE4 = 0x00FE1211;
    static s32 D_800A9EE8 = 0x0000FE11;
    static s32 D_800A9EEC = 0x0000FE12;

    // Process variant mesh.
    variantId = MESH_SWAP_STATUS_VARIANT_ID_GET(meshSwapStatus);
    if (variantId != HarryVariantMesh_None)
    {
        func_80045468(skel, &D_800A9ECC, false);

        switch (variantId)
        {
            case HarryVariantMesh_RightHandEmpty:
                func_80045468(skel, &D_800A9ED0, true);
                break;

            case HarryVariantMesh_HandMelee:
                func_80045468(skel, &D_800A9ED4, true);
                break;

            case HarryVariantMesh_RightHandGun:
                func_80045468(skel, &D_800A9ED8, true);
                break;

            case HarryVariantMesh_RightHandRifle:
                func_80045468(skel, &D_800A9EDC, true);
                break;

            case HarryVariantMesh_5:
                func_80045468(skel, &D_800A9EE0, true);
                break;
        }
    }

    // Process swappable mesh.
    swappableId = MESH_SWAP_STATUS_SWAPPABLE_ID_GET(meshSwapStatus);
    if (swappableId != HarrySwappableMesh_None)
    {
        func_80045468(skel, &D_800A9EE4, false);

        switch (swappableId)
        {
            case MESH_SWAP_STATUS(HarrySwappableMesh_RightHand, HarrySwappableMesh_None):
                func_80045468(skel, &D_800A9EE8, true);
                break;

            case MESH_SWAP_STATUS(HarrySwappableMesh_LeftHand, HarrySwappableMesh_None):
                func_80045468(skel, &D_800A9EEC, true);
                break;
        }
    }
}

void WorldGfx_CybilMeshSwap(s_Skeleton* skel, s32 meshSwapStatus) // 0x8003DF84
{
    s32 variantId;
    s32 swappableId;

    static s32 D_800A9EF0 = 0x0000FE14;
    static s32 D_800A9EF4 = 0x00FE1514;
    static s32 D_800A9EF8 = 0x0000FE12;
    static s32 D_800A9EFC = 0x00FE1312;

    // Process variant mesh.
    variantId = MESH_SWAP_STATUS_VARIANT_ID_GET(meshSwapStatus);
    if (variantId != CybilVariantMesh_None)
    {
        switch (variantId)
        {
            case CybilVariantMesh_1:
                func_80045468(skel, &D_800A9EF4, false);
                func_80045468(skel, &D_800A9EF0, true);
                break;

            case CybilVariantMesh_2:
                func_80045468(skel, &D_800A9EF0, false);
                func_80045468(skel, &D_800A9EF4, true);
                break;
        }
    }

    // Process swappable mesh.
    variantId = MESH_SWAP_STATUS_SWAPPABLE_ID_GET(meshSwapStatus);
    if (variantId != CybilSwappableMesh_None)
    {
        switch (variantId)
        {
            case MESH_SWAP_STATUS(CybilSwappableMesh_1, CybilVariantMesh_None):
                func_80045468(skel, &D_800A9EFC, false);
                func_80045468(skel, &D_800A9EF8, true);
                break;

            case MESH_SWAP_STATUS(CybilSwappableMesh_2, CybilVariantMesh_None):
                func_80045468(skel, &D_800A9EF8, false);
                func_80045468(skel, &D_800A9EFC, true);
                break;
        }
    }
}

void WorldGfx_MonsterCybilMeshSwap(s_Skeleton* skel, s32 meshSwapStatus) // 0x8003E08C
{
    s32 variantId;

    static s32 D_800A9F00 = 0x0000FE02;
    static s32 D_800A9F04 = 0x0000FE03;
    static s32 D_800A9F08 = 0x0000FE00;
    static s32 D_800A9F0C = 0x00FE0100;

    // Process variant mesh.
    variantId = MESH_SWAP_STATUS_VARIANT_ID_GET(meshSwapStatus);
    if (variantId != CybilVariantMesh_None)
    {
        switch (variantId)
        {
            case CybilVariantMesh_1:
                func_80045468(skel, &D_800A9F04, false);
                func_80045468(skel, &D_800A9F00, true);
                break;

            case CybilVariantMesh_2:
                func_80045468(skel, &D_800A9F00, false);
                func_80045468(skel, &D_800A9F04, true);
                break;
        }
    }

    // Process swappable mesh.
    variantId = MESH_SWAP_STATUS_SWAPPABLE_ID_GET(meshSwapStatus);
    if (variantId != CybilSwappableMesh_None)
    {
        switch (variantId)
        {
            case MESH_SWAP_STATUS(CybilSwappableMesh_1, CybilVariantMesh_None):
                func_80045468(skel, &D_800A9F0C, false);
                func_80045468(skel, &D_800A9F08, true);
                break;

            case MESH_SWAP_STATUS(CybilSwappableMesh_2, CybilVariantMesh_None):
                func_80045468(skel, &D_800A9F08, false);
                func_80045468(skel, &D_800A9F0C, true);
                break;
        }
    }
}

void WorldGfx_DahliaMeshSwap(s_Skeleton* skel, s32 meshSwapStatus) // 0x8003E194
{
    s32 variantId;

    static s32 D_800A9F10 = 0xFE03FD00;
    static s32 D_800A9F14 = 0x0000FE00;
    static s32 D_800A9F18 = 0x00FE0201;
    static s32 D_800A9F1C = 0x0000FE03;

    // Process variant mesh.
    variantId = MESH_SWAP_STATUS_VARIANT_ID_GET(meshSwapStatus);
    if (variantId != DahliaVariantMesh_None)
    {
        func_80045468(skel, &D_800A9F10, false);

        switch (variantId)
        {
            case DahliaVariantMesh_1:
                func_80045468(skel, &D_800A9F14, true);
                break;

            case DahliaVariantMesh_2:
                func_80045468(skel, &D_800A9F18, true);
                break;

            case DahliaVariantMesh_3:
                func_80045468(skel, &D_800A9F1C, true);
                break;
        }
    }
}

void WorldGfx_KaufmannMeshSwap(s_Skeleton* skel, s32 meshSwapStatus) // 0x8003E238
{
    s32 variantId;
    s32 swappableId;

    static s32 D_800A9F20 = 0x06050403;
    static s32 D_800A9F24 = 0x000000FE; // @unused
    static s32 D_800A9F28 = 0x0000FE03;
    static s32 D_800A9F2C = 0x0000FE04;
    static s32 D_800A9F30 = 0x00FE0504;
    static s32 D_800A9F34 = 0x00FE0603;
    static s32 D_800A9F38 = 0xFE020100;
    static s32 D_800A9F3C = 0x0000FE00;
    static s32 D_800A9F40 = 0x0000FE01;
    static s32 D_800A9F44 = 0x00FE0201;

    // Process variant mesh.
    variantId = MESH_SWAP_STATUS_VARIANT_ID_GET(meshSwapStatus);
    if (variantId != KaufmannVariantMesh_None)
    {
        func_80045468(skel, &D_800A9F20, false);

        switch (variantId)
        {
            case KaufmannVariantMesh_1:
                func_80045468(skel, &D_800A9F28, true);
                break;

            case KaufmannVariantMesh_2:
                func_80045468(skel, &D_800A9F2C, true);
                break;

            case KaufmannVariantMesh_3:
                func_80045468(skel, &D_800A9F30, true);
                break;

            case KaufmannVariantMesh_4:
                func_80045468(skel, &D_800A9F34, true);
                break;
        }
    }

    // Process swappable mesh.
    swappableId = MESH_SWAP_STATUS_SWAPPABLE_ID_GET(meshSwapStatus);
    if (swappableId != KaufmannSwappableMesh_None)
    {
        func_80045468(skel, &D_800A9F38, false);

        switch (swappableId)
        {
            case MESH_SWAP_STATUS(KaufmannSwappableMesh_1, KaufmannVariantMesh_None):
                func_80045468(skel, &D_800A9F3C, true);
                break;

            case MESH_SWAP_STATUS(KaufmannSwappableMesh_2, KaufmannVariantMesh_None):
                func_80045468(skel, &D_800A9F40, true);
                break;

            case MESH_SWAP_STATUS(KaufmannSwappableMesh_3, KaufmannVariantMesh_None):
                func_80045468(skel, &D_800A9F44, true);
                break;
        }
    }
}

void WorldGfx_StalkerMeshSwap(s_Skeleton* skel, s32 meshSwapStatus) // 0x8003E388
{
    s32 variantId;

    static s32 D_800A9F48 = 0x0000FE05;
    static s32 D_800A9F4C = 0x0000FE06;

    // Process variant mesh.
    variantId = MESH_SWAP_STATUS_VARIANT_ID_GET(meshSwapStatus);
    if (variantId != StalkerVariantMesh_None)
    {
        switch (variantId)
        {
            case StalkerVariantMesh_1:
                func_80045468(skel, &D_800A9F4C, false);
                func_80045468(skel, &D_800A9F48, true);
                break;

            case StalkerVariantMesh_2:
                func_80045468(skel, &D_800A9F48, false);
                func_80045468(skel, &D_800A9F4C, true);
                break;
        }
    }
}

void WorldGfx_SplitHeadMeshSwap(s_Skeleton* skel, s32 meshSwapStatus) // 0x8003E414
{
    s32 variantId;

    static s32 D_800A9F50 = 0xFE19FD11;
    static s32 D_800A9F54 = 0xFE22FD1A;

    // @note Unique disuse of `MESH_SWAP_STATUS_VARIANT_ID_GET`. This masks bits 0-1 instead of 0-3.
    variantId = meshSwapStatus & 0x3;
    if (variantId != SplitHeadVariantMesh_None)
    {
        switch (variantId)
        {
            case SplitHeadVariantMesh_1:
                func_80045468(skel, &D_800A9F50, false);
                func_80045468(skel, &D_800A9F54, true);
                break;

            case SplitHeadVariantMesh_2:
                func_80045468(skel, &D_800A9F54, false);
                func_80045468(skel, &D_800A9F50, true);
                break;
        }
    }
}

void WorldGfx_PuppetNurseMeshSwap(s_Skeleton* skel, s32 meshSwapStatus) // 0x8003E4A0
{
    s32 variantId;

    static s32 D_800A9F58 = 0x05040302;
    static s32 D_800A9F5C = 0x00FE0706; // @unused
    static s32 D_800A9F60 = 0x00FE0502;
    static s32 D_800A9F64 = 0x00FE0603;
    static s32 D_800A9F68 = 0x00FE0704;

    // Process variant mesh.
    variantId = MESH_SWAP_STATUS_VARIANT_ID_GET(meshSwapStatus);
    if (variantId != PuppetNurseVariantMesh_None)
    {
        func_80045468(skel, &D_800A9F58, false);

        switch (variantId)
        {
            case PuppetNurseVariantMesh_Head1:
                func_80045468(skel, &D_800A9F60, true);
                break;

            case PuppetNurseVariantMesh_Head2:
                func_80045468(skel, &D_800A9F64, true);
                break;

            case PuppetNurseVariantMesh_Head3:
                func_80045468(skel, &D_800A9F68, true);
                break;
        }
    }
}

void WorldGfx_PuppetDoctorMeshSwap(s_Skeleton* skel, s32 meshSwapStatus) // 0x8003E544
{
    s32 variantId;

    static s32 D_800A9F6C = 0x05040302;
    static s32 D_800A9F70 = 0x00FE0706; // @unused
    static s32 D_800A9F74 = 0x00FE0502;
    static s32 D_800A9F78 = 0x00FE0603;
    static s32 D_800A9F7C = 0x00FE0704;

    // Process variant mesh.
    variantId = MESH_SWAP_STATUS_VARIANT_ID_GET(meshSwapStatus);
    if (variantId != PuppetNurseVariantMesh_None)
    {
        func_80045468(skel, &D_800A9F6C, false);

        switch (variantId)
        {
            case PuppetNurseVariantMesh_Head1:
                func_80045468(skel, &D_800A9F74, true);
                break;

            case PuppetNurseVariantMesh_Head2:
                func_80045468(skel, &D_800A9F78, true);
                break;

            case PuppetNurseVariantMesh_Head3:
                func_80045468(skel, &D_800A9F7C, true);
                break;
        }
    }
}

const s32 __pad_rodata_80025BB0 = 0;
