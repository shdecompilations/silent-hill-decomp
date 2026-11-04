#include "game.h"
#include "inline_no_dmpsx.h"

#include <psyq/gtemac.h>
#include <psyq/libapi.h>
#include <psyq/strings.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/collision/collision.h"
#include "bodyprog/math/math.h"
#include "bodyprog/item_screens.h"
#include "bodyprog/player/player.h"
#include "bodyprog/screen/screen_data.h"
#include "bodyprog/screen/screen_draw.h"
#include "bodyprog/sound/sound_system.h"
#include "main/rng.h"

#define PROXIMITY_BUFFER Q8(1.0f / 32.0f)

s_ActiveCollisionTriggers g_ActiveCollisionTriggers;

void Collision_Init(void) // 0x800697EC
{
    Collision_TriggerFlagsSet(CollisionTriggerFlag_Map);
    g_ActiveCollisionTriggers.collisionTriggerCount = 0;
}

u16 Collision_TriggerFlagsGet(void) // 0x80069810
{
    return g_ActiveCollisionTriggers.flags;
}

void Collision_TriggerFlagsSet(u16 triggerFlags) // 0x80069820
{
    g_ActiveCollisionTriggers.flags = triggerFlags;
}

void Collision_TriggerFlagBitsSet(u16 triggerFlags) // 0x8006982C
{
    g_ActiveCollisionTriggers.flags |= triggerFlags;
}

void Collision_TriggerFlagBitsClear(s32 triggerFlags) // 0x80069844
{
    g_ActiveCollisionTriggers.flags = (g_ActiveCollisionTriggers.flags & ~triggerFlags) | CollisionTriggerFlag_Map;
}

void Collision_NearbyTriggersGet(q19_12 posX, q19_12 posZ, const s_CollisionTrigger* triggers) // 0x80069860
{
    #define AABB_BUFFER Q12(16.0f)

    const s_CollisionTrigger* curTrigger;
    q19_12                    minX;
    q19_12                    maxX;
    q19_12                    minZ;
    q19_12                    maxZ;

    g_ActiveCollisionTriggers.collisionTriggerCount = 0;

    // Run through triggers.
    for (curTrigger = &triggers[0]; !curTrigger->isEndOfArray; curTrigger++)
    {
        // Define AABB bounds.
        minX = FP_TO(curTrigger->positionX, Q12_SHIFT);
        maxX = FP_TO(curTrigger->positionX + curTrigger->sizeX, Q12_SHIFT);
        minZ = FP_TO(curTrigger->positionZ, Q12_SHIFT);
        maxZ = FP_TO(curTrigger->positionZ + curTrigger->sizeZ, Q12_SHIFT);

        // Expand AABB.
        minX -= AABB_BUFFER;
        maxX += AABB_BUFFER;
        minZ -= AABB_BUFFER;
        maxZ += AABB_BUFFER;

        // Collect collided trigger.
        if (posX >= minX && maxX >= posX &&
            posZ >= minZ && maxZ >= posZ)
        {
            g_ActiveCollisionTriggers.collisionTriggers[g_ActiveCollisionTriggers.collisionTriggerCount] = curTrigger;
            g_ActiveCollisionTriggers.collisionTriggerCount++;
        }
    }

    #undef AABB_BUFFER
}

void Collision_ChunkHeaderPtrsInit(s_IpdCollisionData* collData) // 0x8006993C
{
    collData->polygonSplitVertices = (u8*)collData->polygonSplitVertices + (u32)collData;
    collData->polygonSurfaces      = (u8*)collData->polygonSurfaces      + (u32)collData;
    collData->polygons             = (u8*)collData->polygons             + (u32)collData;
    collData->cylinders            = (u8*)collData->cylinders            + (u32)collData;
    collData->shapeAllocs          = (u8*)collData->shapeAllocs          + (u32)collData;
    collData->shapeLut             = (u8*)collData->shapeLut             + (u32)collData;
    collData->surfaceLut           = (u8*)collData->surfaceLut           + (u32)collData;
}

void Collision_ShapePassReset(s_IpdCollisionData* collData) // 0x80069994
{
    s32* curStamp;

    // Increment shape pass counter.
    collData->shapePassCounter++;

    // Reset stamps when pass counter exceeds reasonable threshold.
    if (collData->shapePassCounter > (sizeof(collData->shapePassStampBuffer) - 4))
    {
        // Clear shape pass counter.
        collData->shapePassCounter = 0;

        // Clear all shape stamps. TODO: Is this `memset`/`bzero`?
        for (curStamp = &collData->shapePassStampBuffer[0];
             curStamp < &collData->shapePassStampBuffer[sizeof(collData->shapePassStampBuffer)];
             curStamp++)
        {
            *curStamp = 0;
        }
    }
}

void Collision_ShapePassUpdate(s_IpdCollisionData* collData) // 0x800699E4
{
    collData->shapePassCounter++;
}

void Collision_SurfaceGet(s_CollisionSurface* surface, q19_12 posX, q19_12 posZ) // 0x800699F8
{
    s_CharaCollisionCylinder cylinder;
    VECTOR3             pos;
    s_CollisionQuery    query;
    s_IpdCollisionData* collData;

    pos.vx = Q12(0.0f);
    pos.vy = Q12(0.0f);
    pos.vz = Q12(0.0f);

    // Get collision data at world position.
    collData = WorldMap_CollisionDataGet(posX, posZ);
    if (collData == NULL)
    {
        surface->floorHeight    = Q12(8.0f);
        surface->slopeGradientZ = Q8(0.0f);
        surface->slopeGradientX = Q8(0.0f);
        surface->floorType      = FloorType_Default;
        return;
    }

    // Define cylinder.
    cylinder.position.vx  = posX;
    cylinder.position.vy  = Q12(0.0f);
    cylinder.position.vz  = posZ;
    cylinder.bottomExtent = Q12(0.0f);
    cylinder.topExtent    = Q12(0.0f);
    cylinder.radius       = Q12(0.0f);

    Collision_CollStateInit(&query, &pos, &cylinder, false);

    query.isCharaMoving = false;
    query.enablePush    = false;
    query.field_0_10    = true;
    Collision_CharaCollisionHandle(&query, collData);

    if (query.disableHeight == true)
    {
        surface->floorType   = FloorType_Default;
        surface->floorHeight = Q12(8.0f);
    }
    else
    {
        surface->floorType   = query.floorType;
        surface->floorHeight = Q8_TO_Q12(Ipd_FloorHeightGet(query.charaState.positionFromX,
                                                            query.charaState.positionFromZ,
                                                            &query));
    }

    surface->slopeGradientX = query.slopeGradientX;
    surface->slopeGradientZ = query.slopeGradientZ;
}

s32 Collision_WallDetect(s_CollisionResult* collResult, const VECTOR3* moveOffset, s_SubCharacter* chara) // 0x80069B24
{
    s32 stackPtr;
    s32 response;

    stackPtr = SetSp(PSX_SCRATCH_ADDR(0x3D8));
    response = Collision_WallResponse(collResult, moveOffset, chara,
                                      Collision_CharaCollisionSetup(collResult, moveOffset, chara));
    SetSp(stackPtr);
    return response;
}

s32 Collision_WallResponse(s_CollisionResult* collResult, const VECTOR3* moveOffset, s_SubCharacter* chara, s32 response) // 0x80069BA8
{
    #define POINT_COUNT          9
    #define ANGLE_STEP           Q12_ANGLE(370.0f / POINT_COUNT) // Maybe @bug, or `370.0f` is used instead of `360.0f`
                                                                 // as a rudimentary way of increasing point density in front. 
    #define WALL_COUNT_THRESHOLD 3                               // Minimum wall-height floors to detect before a wall is acknowledged.
    #define WALL_HEIGHT          Q12(0.5f)                       // Floor height to treat as a wall.

    s_CollisionSurface surface;
    e_CollisionType    collType;
    q19_12             floorHeight;
    q19_12             wallHeightBound;
    s32                i;
    s32                wallCount;
    s32                floorType; // `e_FloorType`

    if (response == NO_VALUE)
    {
        response = true;

        // Push player away from drop.
        if (chara == &g_SysWork.playerWork.player && chara->health > Q12(0.0f))
        {
            Collision_WallPush(collResult, &chara->position, chara->position.vy, chara->rotation.vy);
        }
    }

    switch (chara->model.charaId)
    {
        case Chara_Harry:
        case Chara_Groaner:
        case Chara_Wormhead:
        case Chara_LarvalStalker:
        case Chara_Stalker:
        case Chara_GreyChild:
        case Chara_Mumbler:
        case Chara_Creeper:
        case Chara_Romper:
        case Chara_PuppetNurse:
        case Chara_PuppetDoctor:
            wallHeightBound = chara->position.vy - WALL_HEIGHT;

            switch (collResult->surface.floorType)
            {
                case FloorType_None:
                    collType = CollisionType_Floor;
                    break;

                default:
                    collType = (collResult->surface.floorHeight < wallHeightBound) ? CollisionType_Wall :
                                                                                      CollisionType_None;
                    break;
            }

            wallCount = 0;
            if (collType == CollisionType_None)
            {
                break;
            }

            // Run through floor heights around character position to detect walls.
            for (i = 0, floorType = FloorType_None; i < POINT_COUNT; i++)
            {
                Collision_SurfaceGet(&surface,
                                     chara->position.vx + Q12_MULT(Math_Sin(i * ANGLE_STEP), Q12(0.2f)),
                                     chara->position.vz + Q12_MULT(Math_Cos(i * ANGLE_STEP), Q12(0.2f)));

                switch (collType)
                {
                    case CollisionType_Wall:
                        // Track wall-height floors.
                        if (surface.floorHeight < wallHeightBound)
                        {
                            wallCount++;
                        }
                        break;

                    case CollisionType_Floor:
                        if (surface.floorType != FloorType_None)
                        {
                            floorType   = surface.floorType;
                            floorHeight = surface.floorHeight;
                        }
                        break;
                }
            }

            switch (collType)
            {
                case CollisionType_Wall:
                    // Ignore wall if too few wall-height floors detected.
                    if (wallCount < WALL_COUNT_THRESHOLD)
                    {
                        collResult->surface.floorHeight = chara->position.vy;
                    }
                    break;

                case CollisionType_Floor:
                    if (floorType == FloorType_None)
                    {
                        break;
                    }

                    collResult->surface.floorHeight = floorHeight;
                    collResult->surface.floorType   = FloorType_None;
                    break;
            }
            break;
    }

    return response;

    #undef POINT_COUNT
    #undef ANGLE_STEP
    #undef WALL_COUNT_THRESHOLD
    #undef WALL_HEIGHT
}

void Collision_WallPush(s_CollisionResult* collResult, const VECTOR3* pos, q19_12 floorHeight, q19_12 headingAngle) // 0x80069DF0
{
    #define POINT_COUNT 16
    #define ANGLE_STEP  Q12_ANGLE(360.0f / POINT_COUNT)

    q19_12             floorHeights[POINT_COUNT];
    s_CollisionSurface surface;
    q19_12             pushAngle;
    q19_12             baseFloorHeight;
    s32                rightLowestFloorHeightIdx;
    s32                leftLowestFloorHeightIdx;
    s32                i;
    q19_12             floorHeightMax;
    q19_12             floorHeightMin;
    s32                lowestFloorHeightIdx;

    // Set base parameters.
    floorHeightMin       = Q12(-DEFAULT_FLOOR_HEIGHT);
    floorHeightMax       = Q12(DEFAULT_FLOOR_HEIGHT);
    lowestFloorHeightIdx = 0;

    // Run through floor heights around position.
    for (i = 0; i < POINT_COUNT; i++)
    {
        // Collect height.
        Collision_SurfaceGet(&surface,
                             pos->vx + Math_Sin((headingAngle & 0xF) + (i * ANGLE_STEP)),
                             pos->vz + Math_Cos((headingAngle & 0xF) + (i * ANGLE_STEP)));
        floorHeights[i] = surface.floorHeight;

        // Track min height.
        if (floorHeightMin < surface.floorHeight)
        {
            floorHeightMin       = surface.floorHeight;
            lowestFloorHeightIdx = i;
        }

        // Track max height.
        if (surface.floorHeight < floorHeightMax)
        {
            floorHeightMax = surface.floorHeight;
        }
    }

    // Cap base floor height.
    baseFloorHeight = DIV_FAST(floorHeightMin + floorHeightMax, 2);
    if (baseFloorHeight < (floorHeight - INTERSECTION_BUFFER))
    {
        baseFloorHeight = floorHeight - INTERSECTION_BUFFER;
    }

    // Search clockwise from lowest point for wall start.
    for (i = lowestFloorHeightIdx + 1, rightLowestFloorHeightIdx = lowestFloorHeightIdx;
         i < (lowestFloorHeightIdx + POINT_COUNT) && baseFloorHeight < floorHeights[i & 0xF];
         i++)
    {
        rightLowestFloorHeightIdx = i;
    }

    // Search counter-clockwise from lowest point for wall start.
    for (i = lowestFloorHeightIdx - 1, leftLowestFloorHeightIdx = lowestFloorHeightIdx;
         i < (lowestFloorHeightIdx - POINT_COUNT) && baseFloorHeight < floorHeights[i & 0xF];
         i--)
    {
        leftLowestFloorHeightIdx = i;
    }

    // Compute offset away from wall.
    pushAngle             = DIV_FAST(Q8(leftLowestFloorHeightIdx + rightLowestFloorHeightIdx), 2);
    collResult->offset.vx = Q12_MULT_PRECISE(Math_Sin(pushAngle), Q12(1.0f / 16.0f));
    collResult->offset.vz = Q12_MULT_PRECISE(Math_Cos(pushAngle), Q12(1.0f / 16.0f));

    #undef POINT_COUNT
    #undef ANGLE_STEP
}

bool Collision_CharaCollisionSetup(s_CollisionResult* collResult, const VECTOR3* moveOffset, s_SubCharacter* chara) // 0x80069FFC
{
    s_CharaCollisionCylinder cylinder;
    VECTOR3                  offsetCpy;
    s32                      collDataIdx;
    s32                      charaCount;
    bool                     cond;

    // Set collision cylinder position.
    cylinder.position.vx = chara->position.vx + chara->collision.shapeOffsets.cylinder.vx;
    cylinder.position.vy = chara->position.vy - Q12(0.02f);
    cylinder.position.vz = chara->position.vz + chara->collision.shapeOffsets.cylinder.vz;

    // Check if collision data at current position is valid.
    if (WorldMap_CollisionDataGet(chara->position.vx, chara->position.vz) == NULL)
    {
        Collision_DefaultResultSet(collResult, Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(8.0f));
        return true;
    }

    // Set collision cylinder query.
    cylinder.topExtent    = chara->collision.box.topExtent;
    cylinder.bottomExtent = chara->collision.box.bottomExtent;
    cylinder.radius       = chara->collision.cylinder.outerRadius;
    cylinder.state        = chara->collision.state;

    offsetCpy = *moveOffset;

    // TODO: Condition related to ceiling detection, exact purpose unclear. See `s_CollisionCharaState::field_4`.
    switch (chara->model.charaId)
    {
        case Chara_Harry:
        case Chara_Groaner:
        case Chara_Wormhead:
        case Chara_Romper:
            cond = true;
            break;

        default:
            cond = false;
            break;
    }

    return func_8006A4A8(collResult, &offsetCpy, &cylinder, cond,
                         WorldMap_ActiveChunksCollisionDataGet(&collDataIdx), collDataIdx, NULL, 0,
                         Collision_CollidableCharasGet(&charaCount, chara, true), charaCount);
}

void Collision_DefaultResultSet(s_CollisionResult* collResult,
                                q19_12 offsetX, q19_12 offsetY, q19_12 offsetZ, q19_12 floorHeight) // 0x8006A178
{
    collResult->offset.vx              = offsetX;
    collResult->offset.vy              = offsetY;
    collResult->offset.vz              = offsetZ;
    collResult->surface.slopeGradientZ = Q8(0.0f);
    collResult->surface.slopeGradientX = Q8(0.0f);
    collResult->surface.floorType      = FloorType_Default;
    collResult->ceilingHeight          = Q12(DEFAULT_CEILING_HEIGHT);
    collResult->surface.floorHeight    = floorHeight;
}

s_SubCharacter** Collision_CollidableCharasGet(s32* collCharaCount, const s_SubCharacter* excludedChara,
                                               bool includePlayer) // 0x8006A1A4
{
    s_SubCharacter* curChara;

    static s_SubCharacter*  collCharas[NPC_COUNT_MAX + 1]; // Enough for all NPCs + player.
    static s_SubCharacter** curCollChara;                  // Active characters.

    // Filter invalid case.
    if (excludedChara != NULL)
    {
        if (excludedChara->model.charaId == Chara_None ||
            excludedChara->collision.state == CharaCollisionState_Ignore ||
            (excludedChara->collision.state == CharaCollisionState_Player && includePlayer == true))
        {
            *collCharaCount = 0;
            return &collCharas;
        }
    }

    *collCharaCount = 0;
    curCollChara    = &collCharas;

    // Collect collidable NPCs.
    for (curChara = &g_SysWork.npcs[0]; curChara < &g_SysWork.npcs[ARRAY_SIZE(g_SysWork.npcs)]; curChara++)
    {
        if (curChara->model.charaId != Chara_None)
        {
            if (curChara->collision.state != CharaCollisionState_Ignore &&
                (curChara->collision.state != CharaCollisionState_Player || includePlayer != true) &&
                curChara != excludedChara &&
                (includePlayer != true || excludedChara == NULL ||
                 excludedChara->collision.state != CharaCollisionState_4 ||
                 curChara->collision.state >= excludedChara->collision.state))
            {
                *collCharaCount              += 1;
                *curCollChara                 = curChara;
                curCollChara++;
                curChara->collision.passStamp = 0;
            }
        }
    }

    // Collect collidable player.
    curChara = &g_SysWork.playerWork.player;
    if (curChara->model.charaId != Chara_None)
    {
        if (curChara->collision.state != CharaCollisionState_Ignore &&
            (curChara->collision.state != CharaCollisionState_Player || includePlayer != true) &&
            curChara != excludedChara &&
            (includePlayer != true || excludedChara == NULL ||
             excludedChara->collision.state != CharaCollisionState_4 ||
             curChara->collision.state >= excludedChara->collision.state))
        {
            *collCharaCount              += 1;
            *curCollChara                 = curChara;
            curCollChara++;
            curChara->collision.passStamp = 0;
        }
    }

    return &collCharas;
}

s32 Collision_OffsetCheck(s_CollisionResult* collResult, VECTOR* offset, const s_CharaCollisionCylinder* cylinder) // 0x8006A3B4
{
    s32 stackPtr;
    s32 var1;

    stackPtr = SetSp(PSX_SCRATCH_ADDR(0x3D8));
    var1 = func_8006A42C(collResult, offset, cylinder);
    SetSp(stackPtr);

    if (var1 == NO_VALUE)
    {
        var1 = 1;
    }
    return var1;
}

s32 func_8006A42C(s_CollisionResult* collResult, const VECTOR3* offset, const s_CharaCollisionCylinder* cylinder) // 0x8006A42C
{
    VECTOR3 offsetCpy;
    s32     collDataIdx;

    offsetCpy = *offset;

    return func_8006A4A8(collResult, &offsetCpy, cylinder, false,
                         WorldMap_ActiveChunksCollisionDataGet(&collDataIdx), collDataIdx, NULL, 0, NULL, 0);
}

s32 func_8006A4A8(s_CollisionResult* collResult, VECTOR3* moveOffset, const s_CharaCollisionCylinder* cylinder, bool arg3,
                  s_IpdCollisionData** collDataPtrs, s32 collDataIdx, s_CharaCollisionRecord* charaRecords, s32 charaRecordCount,
                  s_SubCharacter** charas, s32 charaCount) // 0x8006A4A8
{
    s_CollisionQuery     query;
    VECTOR3              extraOffset; // Q19.12
    VECTOR3              moveOffsetCpy1;
    VECTOR3              moveOffsetCpy0;
    q23_8                radius;
    s32                  i;
    bool                 cond;
    q19_12               floorHeight;
    s32                  count;
    s_SubCharacter**     curChara;
    s_IpdCollisionData** curCollData;
    s_SubCharacter*      chara;

    cond = false;

    // Set default collision result.
    if (cylinder->state == CharaCollisionState_Default)
    {
        Collision_DefaultResultSet(collResult, moveOffset->vx, moveOffset->vy, moveOffset->vz, cylinder->position.vy);
        return 0;
    }

    // Dampen movement offset.
    Collision_MoveOffsetDampen(moveOffset, cylinder, charas, charaCount);

    moveOffsetCpy0            = *moveOffset;
    collResult->ceilingHeight = Collision_TriggerCeilingHeightGet(&moveOffsetCpy0, cylinder, cylinder->radius, cylinder->topExtent);
    Collision_CollStateInit(&query, &moveOffsetCpy0, cylinder, arg3);

    // Apply movement offset.
    moveOffsetCpy1        = moveOffsetCpy0;
    collResult->offset.vz = Q12(0.0f);
    collResult->offset.vx = Q12(0.0f);
    collResult->offset.vy = moveOffsetCpy0.vy;

    while (true)
    {
        if (query.offsetX)
        {
            query.isCharaMoving = query.charaState.moveDistance != Q12(0.0f);
            query.enablePush    = false;
            query.field_0_10    = false;
        }
        else
        {
            query.isCharaMoving = query.charaState.moveDistance != Q12(0.0f);
            query.enablePush    = true;
            query.field_0_10    = true;
        }

        // Run through collision data.
        for (curCollData = collDataPtrs; curCollData < &collDataPtrs[collDataIdx]; curCollData++)
        {
            Collision_CharaCollisionHandle(&query, *curCollData);
        }

        if (query.obstacleContext.hardPushConstraint.shapeCount &&
            query.obstacleContext.hardPushConstraint.angleMin == query.obstacleContext.hardPushConstraint.angleMax)
        {
            cond |= true;
        }

        func_8006CF18(&query, charaRecords, charaRecordCount);

        // Run through characters.
        for (curChara = charas; curChara < &charas[charaCount]; curChara++)
        {
            chara  = *curChara;

            radius = Q12_TO_Q8(chara->collision.cylinder.outerRadius) + query.charaState.radius;
            if (chara->collision.state < (u32)query.charaState.state)
            {
                radius -= Q8(0.06f);
            }

            query.chunkOffsetFrom.vx = Q12_TO_Q8(chara->position.vx + chara->collision.shapeOffsets.cylinder.vx);
            query.chunkOffsetTo.vz   = Q12_TO_Q8(chara->position.vz + chara->collision.shapeOffsets.cylinder.vz);

            query.context.chara.bottom    = Q12_TO_Q8(chara->collision.box.topExtent    + chara->position.vy);
            query.context.chara.top       = Q12_TO_Q8(chara->collision.box.bottomExtent + chara->position.vy);
            query.context.chara.radius    = radius;
            query.context.chara.state     = chara->collision.state;
            query.context.chara.passStamp = &chara->collision.passStamp;

            if (!query.offsetX)
            {
                chara->collision.passStamp = 0;
            }

            func_8006CC9C(&query);
            func_8006CF18(&query, chara->collision.charaRecords, chara->collision.charaRecordCount);
        }

        func_8006D01C(&extraOffset, &moveOffsetCpy1, Collision_OffsetAlphaGet(&query), &query);

        collResult->offset.vx += extraOffset.vx;
        collResult->offset.vz += extraOffset.vz;

        if (query.offsetX)
        {
            break;
        }

        if (query.obstacleContext.hardPushConstraint.shapeCount  != 0||
            query.obstacleContext.charaPushConstraint.shapeCount != 0)
        {
            count = ARRAY_SIZE(query.obstacleContext.shapePassStamps);
            if (query.obstacleContext.softPushConstraint.shapeCount < 9)
            {
                count = query.obstacleContext.softPushConstraint.shapeCount;
            }

            for (i = 0; i < count; i++)
            {
                *query.obstacleContext.shapePassStamps[i] += 1;
            }
        }
        else if (query.field_34 != CollisionContactType_None)
        {
            *query.passStamp += 1;
        }
        else
        {
            break;
        }

        query.offsetX = true;
        func_8006D774(&query, &extraOffset, &moveOffsetCpy1);
    }

    if (query.disableHeight == true)
    {
        floorHeight                   = Q12(8.0f);
        collResult->surface.floorType = FloorType_Default;
    }
    else
    {
        collResult->surface.floorType = query.floorType;
        floorHeight                   = Q8_TO_Q12(Ipd_FloorHeightGet(query.charaState.positionFromX + Q12_TO_Q8(extraOffset.vx),
                                                                     query.charaState.positionFromZ + Q12_TO_Q8(extraOffset.vz), &query));
    }

    collResult->surface.floorHeight    = floorHeight;
    collResult->surface.slopeGradientX = query.slopeGradientX;
    collResult->surface.slopeGradientZ = query.slopeGradientZ;

    if (cond)
    {
        return NO_VALUE;
    }

    return query.offsetX != false;
}

void Collision_MoveOffsetDampen(VECTOR3* moveOffset, const s_CharaCollisionCylinder* cylinder,
                                s_SubCharacter** charas, s32 charaCount) // 0x8006A940
{
    #define SLOWDOWN_MAX Q12(0.4f)
    
    q19_12          headingAngle;
    q19_12          cylinderOffsetZ;
    q19_12          cylinderOffsetX;
    q19_12          collAngleDelta;
    s32             i;
    q19_12          slowdownAlpha;
    q19_12          slowdownAlphaReduction;
    s32             dist;
    q19_12          curCharaTop;
    q19_12          curCharaBottom;
    q19_12          otherCharaBottom;
    q19_12          otherCharaTop;
    s_SubCharacter* curChara;

    slowdownAlpha = Q12(1.0f);
    headingAngle  = ratan2(moveOffset->vx, moveOffset->vz);

    // Run through characters to collide.
    for (i = 0; i < charaCount; i++)
    {
        curChara = charas[i];

        // Check if current character is collidable.
        if (curChara->collision.state == CharaCollisionState_Ignore ||
            curChara->collision.state == CharaCollisionState_Player ||
            curChara->collision.state >= cylinder->state)
        {
            continue;
        }

        // Check if cylinders collide on vertical axis using box top and bottom.
        curCharaTop      = curChara->collision.box.topExtent    + curChara->position.vy;
        curCharaBottom   = curChara->collision.box.bottomExtent + curChara->position.vy;
        otherCharaTop    = cylinder->topExtent                  + cylinder->position.vy;
        otherCharaBottom = cylinder->bottomExtent               + cylinder->position.vy;
        if (curCharaTop    > otherCharaBottom ||
            curCharaBottom < otherCharaTop)
        {
            continue;
        }

        cylinderOffsetX = (curChara->position.vx + curChara->collision.shapeOffsets.cylinder.vx) - cylinder->position.vx;
        cylinderOffsetZ = (curChara->position.vz + curChara->collision.shapeOffsets.cylinder.vz) - cylinder->position.vz;
        
        // Check if cylinders collide on XZ plane.
        dist = Vc_VectorMagnitudeCalc(cylinderOffsetX, Q12(0.0f), cylinderOffsetZ);
        if (((curChara->collision.cylinder.outerRadius + cylinder->radius) + INTERSECTION_BUFFER) < dist)
        {
            continue;
        }

        // Compute directional alignment.
        collAngleDelta = Q12_MULT(Math_Cos(ratan2(cylinderOffsetX, cylinderOffsetZ) - headingAngle), Q12(1.5f));

        // Clamp to produce no slowdown when moving away or sideways.
        slowdownAlphaReduction = MAX(collAngleDelta, Q12(0.0f));

        // Cap maximum slowdown per character collision with unique case for Hanged Scratcher.
        collAngleDelta = slowdownAlphaReduction; // @hack From here, `collAngleDelta` used as `slowdownAlphaReduction` for match.
        if (curChara->model.charaId == Chara_HangedScratcher)
        {
            collAngleDelta = MIN(collAngleDelta, Q12(0.6f));
        }
        else
        {
            collAngleDelta = MIN(collAngleDelta, Q12(0.4f));
        }

        // Adjust slowdown alpha.
        slowdownAlpha -= collAngleDelta;
    }

    // Apply slowdown to displacement offset.
    slowdownAlpha  = MAX(slowdownAlpha, Q12(0.4f));
    moveOffset->vx = Q12_MULT(slowdownAlpha, moveOffset->vx);
    moveOffset->vz = Q12_MULT(slowdownAlpha, moveOffset->vz);
}

void Collision_CollStateInit(s_CollisionQuery* state, VECTOR3* moveOffset,
                             const s_CharaCollisionCylinder* cylinder, bool blockInvalidFloor) // 0x8006AB50
{
    state->offsetX                      = false;
    state->triggerFlags                 = g_ActiveCollisionTriggers.flags;
    state->charaState.blockInvalidFloor = blockInvalidFloor;

    Collision_MoveDirectionCalc(&state->charaState, moveOffset, cylinder);

    state->slopedFloorHeight = Q8(DEFAULT_FLOOR_HEIGHT);
    state->field_34 = CollisionContactType_None;
    
    state->obstacleContext.hardPushConstraint.shapeCount  = 0;
    state->obstacleContext.hardIntersectDepth             = Q8(0.0f);
    state->obstacleContext.softPushConstraint.shapeCount  = 0;
    state->obstacleContext.charaIntersectDepth            = Q8(0.0f);
    state->obstacleContext.charaPushConstraint.shapeCount = 0;
    
    state->slopeGradientZ = Q8(0.0f);
    state->slopeGradientX = Q8(0.0f);
    state->disableHeight  = true;
    state->floorType      = FloorType_Default;
}

void Collision_MoveDirectionCalc(s_CollisionCharaState* charaState,
                                 const VECTOR3* moveOffset, const s_CharaCollisionCylinder* cylinder) // 0x8006ABC0
{
    q3_12 headingAngle;

    charaState->moveOffset.vx = Q12_TO_Q8(moveOffset->vx);
    charaState->moveOffset.vy = Q12_TO_Q8(moveOffset->vy);
    charaState->moveOffset.vz = Q12_TO_Q8(moveOffset->vz);

    charaState->moveDistance = Math_Vector2MagCalc(charaState->moveOffset.vx, charaState->moveOffset.vz);
    if (charaState->moveDistance != Q12(0.0f))
    {
        // @unused
        charaState->moveDirection.vx = Q12(charaState->moveOffset.vx) / charaState->moveDistance;
        charaState->moveDirection.vz = Q12(charaState->moveOffset.vz) / charaState->moveDistance;

        headingAngle                 = ratan2(charaState->moveOffset.vz, charaState->moveOffset.vx);
        charaState->moveDirection.vx = Math_Cos(headingAngle);
        charaState->moveDirection.vz = Math_Sin(headingAngle);
    }
    else
    {
        charaState->moveDirection.vx = Q12(1.0f);
        charaState->moveDirection.vz = Q12(0.0f);
    }

    charaState->radius        = Q12_TO_Q8(cylinder->radius);
    charaState->positionFromX = Q12_TO_Q8(cylinder->position.vx);
    charaState->positionFromZ = Q12_TO_Q8(cylinder->position.vz);
    charaState->positionToX   = charaState->positionFromX + charaState->moveOffset.vx;
    charaState->positionToZ   = charaState->positionFromZ + charaState->moveOffset.vz;
    charaState->top           = Q12_TO_Q8(cylinder->topExtent    + cylinder->position.vy);
    charaState->bottom        = Q12_TO_Q8(cylinder->bottomExtent + cylinder->position.vy);
    charaState->state         = cylinder->state;
}

void Collision_CharaCollisionHandle(s_CollisionQuery* state, s_IpdCollisionData* collData) // 0x8006AD44
{
    s32              endCellX;
    s32              startCellX;
    s32              curCellZ;
    s32              curCellX;
    s_IpdShapeAlloc* shapeAlloc;

    // Check if IPD collision contains necessary information and current cell is accessible.
    if ((collData->polygonSurfaceCount == 0 &&
         collData->polygonCount        == 0 &&
         collData->cylinderCount       == 0) ||
        !Collision_CellInit(state, collData))
    {
        return;
    }

    if (!state->offsetX)
    {
        Collision_ShapePassReset(collData);
    }

    // Run through neighboring cells.
    startCellX = state->context.shape.cellX;
    endCellX   = (state->context.shape.cellX + state->context.shape.cellSpanX) - 1;
    for (curCellZ = state->context.shape.cellZ;
         curCellZ < (state->context.shape.cellZ + state->context.shape.cellSpanZ);
         curCellZ++)
    {
        shapeAlloc = &collData->shapeAllocs[(curCellZ * collData->cellCountX) + startCellX];

        for (curCellX = startCellX;
             curCellX <= endCellX;
             curCellX++, shapeAlloc++)
        {
            func_8006B1C8(state, collData, shapeAlloc);
        }
    }

    if (!state->offsetX)
    {
        Collision_ShapePassUpdate(collData);
    }

    // TODO: Something that handles height and movement on slopes.
    if (state->field_0_10)
    {
        func_8006C838(state, collData);
        if (state->context.shape.shapeAlloc != NULL)
        {
            func_8006CA18(state, collData, state->context.shape.shapeAlloc);
        }
    }
}

bool Collision_CellInit(s_CollisionQuery* state, const s_IpdCollisionData* collData) // 0x8006AEAC
{
    s_CollisionCachedSurface* curCachedSurface;

    if (!Collision_CellIdxGet(state, collData))
    {
        return false;
    }

    state->chunkOffsetFrom.offset.vx = state->charaState.positionFromX - collData->positionX;
    state->chunkOffsetFrom.offset.vz = state->charaState.positionFromZ - collData->positionZ;
    state->chunkOffsetTo.offset.vx   = state->charaState.positionToX   - collData->positionX;
    state->chunkOffsetTo.offset.vz   = state->charaState.positionToZ   - collData->positionZ;
    
    if ((state->chunkOffsetFrom.offset.vx / collData->cellSize) <  0                    ||
        (state->chunkOffsetFrom.offset.vx / collData->cellSize) >= collData->cellCountX ||
        (state->chunkOffsetFrom.offset.vz / collData->cellSize) <  0                    ||
        (state->chunkOffsetFrom.offset.vz / collData->cellSize) >= collData->cellCountZ)
    {
        state->context.shape.shapeAlloc = NULL;
    }
    else
    {
        state->context.shape.shapeAlloc = &collData->shapeAllocs[((state->chunkOffsetFrom.offset.vz / collData->cellSize) * collData->cellCountX) +
                                                                    (state->chunkOffsetFrom.offset.vx / collData->cellSize)];
        state->shapeIdx                    = NO_VALUE;

        // Run through cached surfaces.
        for (curCachedSurface = &state->context.shape.surfaceCache[0];
             curCachedSurface < &state->context.shape.surfaceCache[4];
             curCachedSurface++)
        {
            curCachedSurface->subChunkTransDir   = SubChunkTransitionDirection_None;
            curCachedSurface->surfaceIdx         = NO_VALUE;
            curCachedSurface->radiusCollDiffDist = INT_MAX;
        }
    }

    return true;
}

bool Collision_CellIdxGet(s_CollisionQuery* state, const s_IpdCollisionData* collData) // 0x8006B004
{
    q23_8 charaCollDiffZClosest;
    q23_8 charaCollDiffXClosest;
    q23_8 cellXSize;
    q23_8 cellZSize;
    q23_8 cellXLimit;
    q23_8 cellZLimit;
    q23_8 charaCollDiffZFarthest;
    q23_8 charaCollDiffXFarthest;
    
    cellXSize  = collData->cellSize * collData->cellCountX;
    cellXLimit = cellXSize - 1;
    cellZSize  = collData->cellSize * collData->cellCountZ;
    cellZLimit = cellZSize - 1;

    // Getting X axis differences.
    charaCollDiffXClosest  = state->charaState.positionFromX - collData->positionX;
    charaCollDiffXFarthest = state->charaState.positionToX   - collData->positionX;
    if (charaCollDiffXFarthest < charaCollDiffXClosest)
    {
        charaCollDiffXFarthest ^= charaCollDiffXClosest;
        charaCollDiffXClosest  ^= charaCollDiffXFarthest;
        charaCollDiffXFarthest ^= charaCollDiffXClosest;
    }
    charaCollDiffXClosest  -= state->charaState.radius;
    charaCollDiffXFarthest += state->charaState.radius;

    // Getting Z axis differences.
    charaCollDiffZClosest  = state->charaState.positionFromZ - collData->positionZ;
    charaCollDiffZFarthest = state->charaState.positionToZ   - collData->positionZ;
    if (charaCollDiffZFarthest < charaCollDiffZClosest)
    {
        charaCollDiffZFarthest ^= charaCollDiffZClosest;
        charaCollDiffZClosest  ^= charaCollDiffZFarthest;
        charaCollDiffZFarthest ^= charaCollDiffZClosest;
    }
    charaCollDiffZClosest  -= state->charaState.radius;
    charaCollDiffZFarthest += state->charaState.radius;

    if (cellXSize < charaCollDiffXClosest ||
        cellZSize < charaCollDiffZClosest ||
        charaCollDiffXFarthest < Q8(0.0f) ||
        charaCollDiffZFarthest < Q8(0.0f))
    {
        return false;
    }

    charaCollDiffXClosest  = limitRange(charaCollDiffXClosest,  Q8(0.0f), cellXLimit);
    charaCollDiffZClosest  = limitRange(charaCollDiffZClosest,  Q8(0.0f), cellZLimit);
    charaCollDiffXFarthest = limitRange(charaCollDiffXFarthest, Q8(0.0f), cellXLimit);
    charaCollDiffZFarthest = limitRange(charaCollDiffZFarthest, Q8(0.0f), cellZLimit);

    state->context.shape.cellX     = charaCollDiffXClosest / collData->cellSize;
    state->context.shape.cellZ     = charaCollDiffZClosest / collData->cellSize;
    state->context.shape.cellSpanX = ((charaCollDiffXFarthest / collData->cellSize) -
                                      state->context.shape.cellX) + 1;
    state->context.shape.cellSpanZ = ((charaCollDiffZFarthest / collData->cellSize) -
                                      state->context.shape.cellZ) + 1;

    return true;
}

void func_8006B1C8(s_CollisionQuery* state, s_IpdCollisionData* collData, s_IpdShapeAlloc* shapeAlloc) // 0x8006B1C8
{
    s32 polygonCount;
    s32 i;
    u8  shapeIdx;

    // Run through allocated cell shapes.
    for (i = shapeAlloc[0].shapeLutIdx; i < shapeAlloc[1].shapeLutIdx; i++)
    {
        shapeIdx = collData->shapeLut[i];

        // Check if shape was already analyzed.
        if (collData->shapePassStampBuffer[shapeIdx] > collData->shapePassCounter)
        {
            continue;
        }
        collData->shapePassStampBuffer[shapeIdx] = collData->shapePassCounter + 1;

        // Handle polygon.
        polygonCount = collData->polygonCount;
        if (shapeIdx < polygonCount)
        {
            if (func_8006B318(state, collData, shapeIdx))
            {
                if (state->field_0_10)
                {
                    func_8006B6E8(state, shapeAlloc);
                }

                if (state->isCharaMoving || state->enablePush)
                {
                    if (state->point.shapeAttribs.polygon.surfaceIdx1 == UCHAR_MAX)
                    {
                        func_8006B9C8(state);
                    }

                    if (state->point.shapeAttribs.polygon.surfaceIdx0 == UCHAR_MAX)
                    {
                        func_8006B8F8(&state->point);
                        func_8006B9C8(state);
                    }
                }
            }
        }
        // Handle cylinder.
        else if (func_8006C3D4(state, collData, shapeIdx))
        {
            func_8006C45C(state);
        }
    }
}

bool func_8006B318(s_CollisionQuery* state, const s_IpdCollisionData* collData, s32 shapeIdx) // 0x8006B318
{
    q23_8         charaVertDiffZ;
    q23_8         charaVertDiffX;
    s32           packedXz;
    s_IpdSurface* curSurface;
    s_IpdPolygon* polygon;

    polygon = &collData->polygons[shapeIdx];
    
    // Alternative collision enabling?
    // As noted briefly in `e_CollisionTriggerFlags` the game contains two flags (three, but one is unused)
    // which specifically enable certain collisions under specific game events, see for example the school
    // boss valve puzzle which relays in this flag system in order to enable and disable the collision that
    // blocks the player from progressing until the puzzle is solved.
    if (CHECK_FLAG_IDX(state->triggerFlags, (polygon->triggerFlagIdxHigh << 2) | polygon->triggerFlagIdxLow, false))
    {
        return false;
    }

    state->point.ipdCollisionData = collData;
    state->point.shapeIdx         = shapeIdx;

    state->point.offset.vx = polygon->offsetX;
    state->point.offset.vy = polygon->offsetY;
    state->point.offset.vz = polygon->offsetZ;

    state->point.shapeAttribs.polygon.polygonSplitVertex0 = collData->polygonSplitVertices[polygon->splitVertexIdx1];
    state->point.shapeAttribs.polygon.polygonSplitVertex1 = collData->polygonSplitVertices[polygon->splitVertexIdx0];

    state->point.shapeAttribs.polygon.surfaceIdx0 = polygon->surfaceIdx0;
    if (state->point.shapeAttribs.polygon.surfaceIdx0 != UCHAR_MAX)
    {
        curSurface                                              = &collData->polygonSurfaces[state->point.shapeAttribs.polygon.surfaceIdx0];
        state->point.shapeAttribs.polygon.surfaceCacheIdx0      = curSurface->surfaceCacheIdx;
        state->point.shapeAttribs.polygon.disableSurfaceHeight0 = curSurface->disableHeight;

        if (state->charaState.blockInvalidFloor &&
            (curSurface->disableHeight == true || curSurface->floorType == FloorType_None))
        {
            state->point.shapeAttribs.polygon.polygonSplitVertex0.vy += Q8(DEFAULT_CEILING_HEIGHT);
            state->point.shapeAttribs.polygon.polygonSplitVertex1.vy += Q8(DEFAULT_CEILING_HEIGHT);
        }
    }

    state->point.shapeAttribs.polygon.surfaceIdx1 = polygon->surfaceIdx1;
    if (state->point.shapeAttribs.polygon.surfaceIdx1 != UCHAR_MAX)
    {
        curSurface                                              = &collData->polygonSurfaces[state->point.shapeAttribs.polygon.surfaceIdx1];
        state->point.shapeAttribs.polygon.surfaceCacheIdx1      = curSurface->surfaceCacheIdx;
        state->point.shapeAttribs.polygon.disableSurfaceHeight1 = curSurface->disableHeight;

        if (state->charaState.blockInvalidFloor &&
            (curSurface->disableHeight == true || curSurface->floorType == FloorType_None))
        {
            state->point.shapeAttribs.polygon.polygonSplitVertex0.vy = Q8(DEFAULT_CEILING_HEIGHT);
            state->point.shapeAttribs.polygon.polygonSplitVertex1.vy = Q8(DEFAULT_CEILING_HEIGHT);
        }
    }

    /* Will - 1: Important for dealing with heights or cells transition. If changing any value given to `surfaceCache`,
       Harry starts freaking out when going through cells that have different heights or cells that have two
       with one having a different alture as he may suddenly appear way above of them, this is more something done to handle
       the transition between cells because onces the player is in the cell with different height to the previous one they
       can walk normally.
    */
    packedXz = PACKED_XZ16(polygon->offsetX, polygon->offsetY);
    gte_ldR11R12(packedXz);
    gte_ldR13R21(packedXz);
    
    packedXz = PACKED_XZ16(state->charaState.moveOffset.vx, state->charaState.moveOffset.vz);
    gte_ldvxy0(packedXz);
    gte_ldvz0();
    gte_rtv0();
    gte_stMAC12(&state->point.charaDeltas.charaMoveOffset);
    /* Will - 1: ^ These values are normally set to 0 when standing still and are only modified when moving.
    
       Will - 2: I think this a distance, the value when set isn't too big and also I think `charaState.moveOffset` is the
       character movement offset, like the equivalent of doing `chunkOffsetFrom.offset.vx - chunkOffsetTo.offset.vx`
       that may explain why it's 0 when standing still.
    */
    
    packedXz = PACKED_XZ16(state->chunkOffsetFrom.offset.vx - state->point.shapeAttribs.polygon.polygonSplitVertex0.vx,
                           state->chunkOffsetFrom.offset.vz - state->point.shapeAttribs.polygon.polygonSplitVertex0.vz);
    gte_ldvxy0(packedXz);
    gte_ldvz0();
    gte_rtv0();
    gte_stMAC12(&state->point.charaDeltas.charaVertDiff);

    charaVertDiffX                                = state->point.charaDeltas.charaVertDiff.vx;
    charaVertDiffZ                                = state->point.charaDeltas.charaVertDiff.vz;
    state->point.charaDeltas.charaMoveVertDiff.vx = state->point.charaDeltas.charaMoveOffset.vx + charaVertDiffX;
    state->point.charaDeltas.charaMoveVertDiff.vz = state->point.charaDeltas.charaMoveOffset.vz + charaVertDiffZ;

    if (charaVertDiffX < Q8(0.0f))
    {
        state->point.charaDeltas.subChunkTransDir   = SubChunkTransitionDirection_X;
        state->point.charaDeltas.radiusCollDiffDist = Math_Vector2MagCalc(charaVertDiffX, charaVertDiffZ);
        state->point.charaDeltas.collDiffDist       = -charaVertDiffX;
    }
    else if (state->point.offset.vz < charaVertDiffX)
    {
        state->point.charaDeltas.subChunkTransDir   = SubChunkTransitionDirection_X;
        state->point.charaDeltas.radiusCollDiffDist = Math_Vector2MagCalc(charaVertDiffX - state->point.offset.vz, charaVertDiffZ);
        state->point.charaDeltas.collDiffDist       = charaVertDiffX - state->point.offset.vz;
    }
    else
    {
        if (charaVertDiffZ < Q8(0.0f))
        {
            state->point.charaDeltas.radiusCollDiffDist = -charaVertDiffZ;
        }
        else
        {
            state->point.charaDeltas.radiusCollDiffDist = charaVertDiffZ;
        }

        state->point.charaDeltas.subChunkTransDir = SubChunkTransitionDirection_Z;
    }

    return true;
}

void func_8006B6E8(s_CollisionQuery* state, s_IpdShapeAlloc* shapeAlloc) // 0x8006B6E8
{
    s32                       surfaceCacheIdx;
    s32                       surfaceIdx0;
    s32                       surfaceIdx1;
    bool                      disableSurfaceHeight0;
    bool                      disableSurfaceHeight1;
    s_CollisionCachedSurface* cachedSurface;

    surfaceIdx0           = state->point.shapeAttribs.polygon.surfaceIdx0;
    surfaceIdx1           = state->point.shapeAttribs.polygon.surfaceIdx1;
    disableSurfaceHeight0 = state->point.shapeAttribs.polygon.disableSurfaceHeight0;
    disableSurfaceHeight1 = state->point.shapeAttribs.polygon.disableSurfaceHeight1;

    if (surfaceIdx0 == UCHAR_MAX)
    {
        if (surfaceIdx1 == UCHAR_MAX)
        {
            return;
        }

        surfaceCacheIdx = state->point.shapeAttribs.polygon.surfaceCacheIdx1;
    }
    else
    {
        surfaceCacheIdx = state->point.shapeAttribs.polygon.surfaceCacheIdx0;
    }

    /* Will - 1: checks if the value at whatever the struct is `temp_s0`
       pointing have defined values. It first checks if `s_CollisionCachedSurface::field_0`
       is 0 or not, need to mention that this value is defined by default as `-1`
       in `Collision_CellInit` so any other value means that the values where defined
       already, but if `s_CollisionCachedSurface::field_0` is not zero then it checks
       if the value stored in `state->point.charaDeltas.field_8` is
       defined with any value other than 0, if not then it comes back here and continues
       if it is then it proceed to do some other checks that will return true.
       
       Likely if any of those cases fails the check then it will redefine whatever
       is supposed to be stored there.
    */
    cachedSurface = &state->context.shape.surfaceCache[surfaceCacheIdx];
    if (!func_8006B7E0(cachedSurface, &state->point.charaDeltas))
    {
        return;
    }

    cachedSurface->subChunkTransDir   = state->point.charaDeltas.subChunkTransDir;
    cachedSurface->radiusCollDiffDist = state->point.charaDeltas.radiusCollDiffDist;
    cachedSurface->collDiffDist       = state->point.charaDeltas.collDiffDist;

    if (state->point.charaDeltas.charaVertDiff.vz >= Q8(0.0f))
    {
        if (disableSurfaceHeight0 == true)
        {
            cachedSurface->surfaceIdx = NO_VALUE;
        }
        else
        {
            cachedSurface->surfaceIdx = surfaceIdx0;
        }
    }
    else
    {
        if (disableSurfaceHeight1 == true)
        {
            cachedSurface->surfaceIdx = NO_VALUE;
        }
        else
        {
            cachedSurface->surfaceIdx = surfaceIdx1;
        }
    }
}

bool func_8006B7E0(s_CollisionCachedSurface* cachedSurface, s_CharaCollisionDeltas* charaDeltas) // 0x8006B7E0
{
    #define UNK_THRESHOLD Q8(0.025f)

    if (cachedSurface->subChunkTransDir == SubChunkTransitionDirection_None)
    {
        return true;
    }

    switch (charaDeltas->subChunkTransDir)
    {
        case SubChunkTransitionDirection_None:
            return false;

        case SubChunkTransitionDirection_Z:
            switch (cachedSurface->subChunkTransDir)
            {
                case 1:
                    if (charaDeltas->radiusCollDiffDist < cachedSurface->radiusCollDiffDist)
                    {
                        return true;
                    }
                    break;

                case 2:
                    if (charaDeltas->radiusCollDiffDist < (cachedSurface->radiusCollDiffDist + UNK_THRESHOLD))
                    {
                        return true;
                    }
                    break;
            }
            break;

        case SubChunkTransitionDirection_X:
            switch (cachedSurface->subChunkTransDir)
            {
                case SubChunkTransitionDirection_Z:
                    if (charaDeltas->radiusCollDiffDist < (cachedSurface->radiusCollDiffDist - UNK_THRESHOLD))
                    {
                        return true;
                    }
                    break;

                case SubChunkTransitionDirection_X:
                    if (charaDeltas->radiusCollDiffDist < (cachedSurface->radiusCollDiffDist - UNK_THRESHOLD))
                    {
                        return true;
                    }

                    if (charaDeltas->radiusCollDiffDist < (cachedSurface->radiusCollDiffDist + UNK_THRESHOLD) &&
                        charaDeltas->collDiffDist < cachedSurface->collDiffDist)
                    {
                        return true;
                    }
                    break;
            }
    }

    return false;

    #undef UNK_THRESHOLD
}

void func_8006B8F8(s_CollisionShapePoint* cellPoint) // 0x8006B8F8
{
    s_CharaCollisionDeltas* ptr;
    s32                     temp_a1;
    s32                     temp_a2;
    s32                     temp;

    temp                                                   = (u16)cellPoint->shapeAttribs.polygon.polygonSplitVertex0.vx;
    temp_a1                                                = (u16)cellPoint->shapeAttribs.polygon.polygonSplitVertex1.vx;
    temp_a2                                                = (u16)cellPoint->shapeAttribs.polygon.polygonSplitVertex1.vz;
    cellPoint->shapeAttribs.polygon.polygonSplitVertex1.vx = temp;
    temp                                                   = (u16)cellPoint->shapeAttribs.polygon.polygonSplitVertex0.vy;
    cellPoint->shapeAttribs.polygon.polygonSplitVertex0.vy = cellPoint->shapeAttribs.polygon.polygonSplitVertex1.vy;
    cellPoint->shapeAttribs.polygon.polygonSplitVertex0.vx = temp_a1;
    cellPoint->offset.vx                                   = -cellPoint->offset.vx;
    cellPoint->shapeAttribs.polygon.polygonSplitVertex1.vy = temp;
    cellPoint->offset.vy                                   = -cellPoint->offset.vy;
    temp                                                   = (u16)cellPoint->shapeAttribs.polygon.polygonSplitVertex0.vz;
    cellPoint->shapeAttribs.polygon.polygonSplitVertex1.vz = temp;
    temp                                                   = cellPoint->shapeAttribs.polygon.surfaceIdx0;
    cellPoint->charaDeltas.charaMoveOffset.vx                 = -cellPoint->charaDeltas.charaMoveOffset.vx;

    cellPoint->shapeAttribs.polygon.polygonSplitVertex0.vz = temp_a2;
    cellPoint->shapeAttribs.polygon.surfaceIdx0            = cellPoint->shapeAttribs.polygon.surfaceIdx1;

    ptr = &cellPoint->charaDeltas;

    cellPoint->shapeAttribs.polygon.surfaceIdx1      = temp;
    temp                                             = cellPoint->shapeAttribs.polygon.surfaceCacheIdx0;
    cellPoint->charaDeltas.charaMoveOffset.vz        = -cellPoint->charaDeltas.charaMoveOffset.vz;
    cellPoint->shapeAttribs.polygon.surfaceCacheIdx0 = cellPoint->shapeAttribs.polygon.surfaceCacheIdx1;
    cellPoint->shapeAttribs.polygon.surfaceCacheIdx1 = temp;

    ptr->charaVertDiff.vz     = -ptr->charaVertDiff.vz;
    ptr->charaVertDiff.vx     = (cellPoint->offset.vz - ptr->charaVertDiff.vx);
    ptr->charaMoveVertDiff.vz = -ptr->charaMoveVertDiff.vz;
    ptr->charaMoveVertDiff.vx = (cellPoint->offset.vz - ptr->charaMoveVertDiff.vx);
}

void func_8006B9C8(s_CollisionQuery* state) // 0x8006B9C8
{
    q23_8 charaRadius;

    if (state->point.shapeAttribs.polygon.surfaceIdx1 == UCHAR_MAX &&
        state->point.charaDeltas.charaVertDiff.vz < Q8(0.0f) &&
        (state->charaState.bottom >= state->point.shapeAttribs.polygon.polygonSplitVertex0.vy ||
         state->charaState.bottom >= state->point.shapeAttribs.polygon.polygonSplitVertex1.vy))
    {
        if (state->enablePush && state->point.charaDeltas.radiusCollDiffDist < state->charaState.radius)
        {
            func_8006BB50(state, CollisionPushType_Hard);
            return;
        }

        charaRadius = state->charaState.radius;
        if (state->enablePush && state->point.charaDeltas.radiusCollDiffDist < (charaRadius + PROXIMITY_BUFFER))
        {
            func_8006BB50(state, CollisionPushType_Soft);
        }

        if (state->isCharaMoving)
        {
            if (!state->obstacleContext.hardPushConstraint.shapeCount &&
                (-charaRadius < state->point.charaDeltas.charaVertDiff.vz ||
                 -charaRadius < state->point.charaDeltas.charaMoveVertDiff.vz) &&
                (-charaRadius < state->point.charaDeltas.charaVertDiff.vx ||
                 -charaRadius < state->point.charaDeltas.charaMoveVertDiff.vx) &&
                (state->point.charaDeltas.charaVertDiff.vx     < (charaRadius + state->point.offset.vz) ||
                 state->point.charaDeltas.charaMoveVertDiff.vx < (charaRadius + state->point.offset.vz)))
            {
                func_8006BE40(state);
            }
        }
    }
}

void func_8006BB50(s_CollisionQuery* state, s32 pushType) // 0x8006BB50
{
    q23_8 charaCollDistX;
    q23_8 charaCollDistZ;
    q7_8  temp2;

    if (func_8006BC34(state) < Q8(0.0f))
    {
        return;
    }

    if (state->point.charaDeltas.charaVertDiff.vx < Q8(0.0f))
    {
        charaCollDistX = state->chunkOffsetFrom.offset.vx - state->point.shapeAttribs.polygon.polygonSplitVertex0.vx;
        charaCollDistZ = state->chunkOffsetFrom.offset.vz - state->point.shapeAttribs.polygon.polygonSplitVertex0.vz;
    }
    else if (state->point.offset.vz < state->point.charaDeltas.charaVertDiff.vx)
    {
        charaCollDistX = state->chunkOffsetFrom.offset.vx - state->point.shapeAttribs.polygon.polygonSplitVertex1.vx;
        charaCollDistZ = state->chunkOffsetFrom.offset.vz - state->point.shapeAttribs.polygon.polygonSplitVertex1.vz;
    }
    else
    {
        charaCollDistX =  state->point.offset.vy;
        charaCollDistZ = -state->point.offset.vx;
    }

    temp2 = state->charaState.radius - state->point.charaDeltas.radiusCollDiffDist;
    func_8006BCC4(&state->obstacleContext, &state->point.ipdCollisionData->shapePassStampBuffer[state->point.shapeIdx],
                  pushType, charaCollDistX, charaCollDistZ, temp2);
}

q23_8 func_8006BC34(s_CollisionQuery* state)
{
    q7_8 someZ;
    q7_8 height0;
    q7_8 height1;
    q7_8 someX;
    q7_8 height2;
    q7_8 floorHeight; // Unsure.

    someX = state->point.charaDeltas.charaVertDiff.vx;
    if (someX < Q8(0.0f))
    {
        floorHeight = state->point.shapeAttribs.polygon.polygonSplitVertex0.vy;
    }
    else
    {
        someZ = state->point.offset.vz;
        if (someZ < someX)
        {
            floorHeight = state->point.shapeAttribs.polygon.polygonSplitVertex1.vy;
        }
        else
        {
            height0 = state->point.shapeAttribs.polygon.polygonSplitVertex0.vy;
            height1 = state->point.shapeAttribs.polygon.polygonSplitVertex1.vy;
            height2 = state->point.shapeAttribs.polygon.polygonSplitVertex0.vy;

            if (height0 == height1)
            {
                floorHeight = height2;
            }
            else
            {
                floorHeight = height2 + ((q23_8)((height1 - height0) * someX) / someZ);
            }
        }
    }

    return state->charaState.bottom - floorHeight;
}

void func_8006BCC4(s_CollisionObstacleContext* obstacleContext, u8* passStamp,
                   u32 pushType, q7_8 distX, q7_8 distZ, q7_8 intersectDepth) // 0x8006BCC4
{
    q7_8 angleMin;
    q7_8 angleMax;

    // TODO: This are not Q12 values nor angles. However the equivalent of both `Q12_ANGLE` in Q8 is weird.
    angleMin = Q12_FRACT(ratan2(distZ, distX) - Q12_ANGLE(89.0f));
    angleMax = Q12_FRACT(angleMin + Q12_ANGLE(178.0f));

    switch (pushType)
    {
        case CollisionPushType_Hard:
            *passStamp += 1;
            Collision_PushConstraintShapeAdd(&obstacleContext->hardPushConstraint, angleMin, angleMax);

            if (obstacleContext->hardIntersectDepth < intersectDepth)
            {
                obstacleContext->hardIntersectDepth = intersectDepth;
            }
            break;

        case CollisionPushType_Soft:
            Collision_PushConstraintShapeAdd(&obstacleContext->softPushConstraint, angleMin, angleMax);

            if (obstacleContext->softPushConstraint.shapeCount < 9)
            {
                obstacleContext->shapePassStamps[obstacleContext->softPushConstraint.shapeCount - 1] = passStamp;
            }
            break;
        
        case CollisionPushType_Chara:
            *passStamp += 1;
            Collision_PushConstraintShapeAdd(&obstacleContext->charaPushConstraint, angleMin, angleMax);

            if (obstacleContext->charaIntersectDepth < intersectDepth)
            {
                obstacleContext->charaIntersectDepth = intersectDepth;
            }
            break;
    }
}

void Collision_PushConstraintShapeAdd(s_CollisionPushConstraint* pushConstraint,
                                      q3_12 angleConstraintMin, q3_12 angleConstraintMax) // 0x8006BDDC
{
    if (pushConstraint->shapeCount == 0)
    {
        pushConstraint->shapeCount = 1;
        pushConstraint->angleMin   = angleConstraintMin;
        pushConstraint->angleMax   = angleConstraintMax;
        return;
    }

    pushConstraint->shapeCount++;
    Vw_ClampAngleRange(&pushConstraint->angleMin, &pushConstraint->angleMax, angleConstraintMin, angleConstraintMax);
}

void func_8006BE40(s_CollisionQuery* state) // 0x8006BE40
{
    q23_8  negRadius;
    q23_8  unkDist;
    q19_12 distAlpha;
    u32    unkSwitchType;
    q19_12 dist;

    distAlpha = Q12(0.0f);
    negRadius = -(state->charaState.radius);
    unkDist   = Q8(0.0f);

    if (state->point.charaDeltas.charaVertDiff.vz >= negRadius)
    {
        if (state->point.charaDeltas.charaVertDiff.vx >= Q8(0.0f))
        {
            if (state->point.offset.vz >= state->point.charaDeltas.charaVertDiff.vx)
            {
                unkDist = state->point.charaDeltas.charaVertDiff.vx;
                unkSwitchType = 2;
            }
            else
            {
                unkSwitchType = 1;
            }
        }
        else
        {
            unkSwitchType = 0;
        }
    }
    else
    {
        if (state->point.charaDeltas.charaMoveOffset.vz == 0)
        {
            if (state->point.charaDeltas.charaVertDiff.vx > 0 &&
                state->point.charaDeltas.charaVertDiff.vx < state->point.offset.vz)
            {
                distAlpha = Q12(0.0f);
                unkDist = state->point.charaDeltas.charaVertDiff.vx;
            }
        }
        else
        {
            distAlpha = FP_TO(negRadius - state->point.charaDeltas.charaVertDiff.vz, Q12_SHIFT) /
                        state->point.charaDeltas.charaMoveOffset.vz;
            dist      = state->point.charaDeltas.charaMoveOffset.vx * distAlpha;
            dist      = FP_FROM(dist, Q12_SHIFT);
            unkDist   = dist + state->point.charaDeltas.charaVertDiff.vx;
        }

        if (unkDist < Q8(0.0F))
        {
            unkSwitchType = 0;
        }
        else if (state->point.offset.vz < unkDist)
        {
            unkSwitchType = 1;
        }
        else
        {
            unkSwitchType = 2;
        }
    }

    switch (unkSwitchType)
    {
        case 0: // Vertex 0.
            func_8006BF88(state, &state->point.shapeAttribs.polygon.polygonSplitVertex0);
            break;

        case 1: // Vertex 1.
            func_8006BF88(state, &state->point.shapeAttribs.polygon.polygonSplitVertex1);
            break;

        case 2: // Non-split?
            func_8006C0C8(state, distAlpha, unkDist);
            break;
    }
}

void func_8006BF88(s_CollisionQuery* state, const SVECTOR3* splitVert) // 0x8006BF88
{
    q3_12 hitAlpha;
    s32   temp2;
    s32   temp3;

    // TODO: Can actual struct be passed?
    hitAlpha = func_8006C248(*(s32*)&state->charaState.moveDirection, state->charaState.moveDistance,
                             splitVert->vx - state->chunkOffsetFrom.offset.vx,
                             splitVert->vz - state->chunkOffsetFrom.offset.vz,
                             state->charaState.radius);
    if (hitAlpha != NO_VALUE && func_8006C1B8(2, hitAlpha, state) && state->charaState.bottom > splitVert->vy)
    {
        state->hitAlpha  = hitAlpha;
        state->field_34  = CollisionContactType_2;
        temp2            = state->chunkOffsetFrom.offset.vx + Q12_MULT(state->charaState.moveOffset.vx, hitAlpha);
        state->hitDistance  = Q12_TO_Q4(state->charaState.moveDistance * hitAlpha);
        state->passStamp = &state->point.ipdCollisionData->shapePassStampBuffer[state->point.shapeIdx];

        state->field_3C = temp2 - splitVert->vx;
        temp3           = state->chunkOffsetFrom.offset.vz + Q12_MULT(state->charaState.moveOffset.vz, hitAlpha);
        state->field_3E = temp3 - splitVert->vz;
    }
}

void func_8006C0C8(s_CollisionQuery* state, q3_12 hitAlpha, q7_8 arg2) // 0x8006C0C8
{
    q23_8 temp;

    if (!func_8006C1B8(1, hitAlpha, state) || state->point.charaDeltas.charaMoveOffset.vz < 0)
    {
        return;
    }

    temp = ((state->point.shapeAttribs.polygon.polygonSplitVertex1.vy -
             state->point.shapeAttribs.polygon.polygonSplitVertex0.vy) * arg2) / state->point.offset.vz;
    if (temp + state->point.shapeAttribs.polygon.polygonSplitVertex0.vy < state->charaState.bottom)
    {
        state->passStamp = &state->point.ipdCollisionData->shapePassStampBuffer[state->point.shapeIdx];
        state->field_34  = CollisionContactType_1;
        state->hitAlpha  = hitAlpha;
        state->hitDistance  = Q12_TO_Q4(state->charaState.moveDistance * hitAlpha);
        state->field_3C  = state->point.offset.vy;
        state->field_3E  = -state->point.offset.vx;
    }
}

bool func_8006C1B8(u32 contactType, q3_12 hitAlpha, s_CollisionQuery* state) // 0x8006C1B8
{
    q27_4 hitDist;

    hitDist = Q12_TO_Q4(state->charaState.moveDistance * hitAlpha);
    switch (contactType)
    {
        default:
        case CollisionContactType_None:
            return false;

        case CollisionContactType_2:
            if (state->field_34 != CollisionContactType_1)
            {
                if (state->field_34 == CollisionContactType_None)
                {
                   return true;
                }

                break;
            }

            hitDist += Q4(6.0f);
            break;

        case CollisionContactType_1:
            if (state->field_34 != contactType)
            {
                if (state->field_34 != CollisionContactType_None)
                {
                    if (state->field_34 == CollisionContactType_2)
                    {
                        hitDist -= Q4(6.0f);
                    }

                    break;
                }

                return true;
            }
            break;
    }

    return hitDist < state->hitDistance;
}

q3_12 func_8006C248(s32 packedDir, q3_12 moveDist, q3_12 deltaX, q7_8 deltaZ, q7_8 radius) // 0x8006C248
{
    DVECTOR sp10; // Q19.12
    s16     temp_v0;
    q7_8    dist;
    q3_12   alpha;

    gte_ldR11R12(packedDir);
    gte_ldR13R21(packedDir);
    gte_ldvxy0(PACKED_XZ16(deltaX, deltaZ));
    gte_ldvz0();
    gte_rtv0();
    gte_stMAC12(&sp10.vx);

    if (sp10.vx < Q12(0.0f))
    {
        dist = Math_Vector2MagCalc(sp10.vx, sp10.vy);
    }
    else if (moveDist < sp10.vx)
    {
        temp_v0 = sp10.vx - moveDist;
        dist  = Math_Vector2MagCalc(temp_v0, sp10.vy);
    }
    else
    {
        dist = ABS(sp10.vy);
    }

    if (moveDist == Q12(0.0f))
    {
        alpha = NO_VALUE;
        if (dist >= radius)
        {
            return alpha;
        }
        else
        {
            return Q12(0.0f);
        }
    }

    if (dist >= radius)
    {
        return NO_VALUE;
    }

    alpha = Q12_DIV(sp10.vx - SquareRoot0(SQUARE(radius) - SQUARE(sp10.vy)), moveDist);
    alpha = CLAMP(alpha, Q12(0.0f), Q12(1.0f));

    return alpha;
}

bool func_8006C3D4(s_CollisionQuery* state, s_IpdCollisionData* collData, s32 shapeIdx) // 0x8006C3D4
{
    s_IpdCylinder* cylinder;

    state->point.ipdCollisionData = collData;
    state->point.shapeIdx         = shapeIdx;
    cylinder                      = &collData->cylinders[shapeIdx - collData->polygonCount];

    if (CHECK_FLAG_IDX(state->triggerFlags, cylinder->triggerFlagIdx, false))
    {
        return false;
    }

    state->point.disableHeight                = cylinder->disableHeight;
    state->point.offset.vx                    = cylinder->offset.vx;
    state->point.offset.vy                    = cylinder->offset.vy;
    state->point.offset.vz                    = cylinder->offset.vz;
    state->point.shapeAttribs.cylinder.radius = cylinder->radius;
    return true;
}

void func_8006C45C(s_CollisionQuery* state) // 0x8006C45C
{
    q7_8  distMax;
    q7_8  dist;
    q3_12 hitAlpha;
    q23_8 bound;
    q23_8 distX;
    q23_8 distZ;
    s32   temp;
    s32   temp2;

    distMax = state->charaState.radius + state->point.shapeAttribs.cylinder.radius;
    bound   = distMax + PROXIMITY_BUFFER;

    if (state->chunkOffsetFrom.offset.vx < (state->point.offset.vx - bound) &&
        state->chunkOffsetTo.offset.vx   < (state->point.offset.vx - bound))
    {
        return;
    }

    if ((state->point.offset.vx + bound) < state->chunkOffsetFrom.offset.vx &&
        (state->point.offset.vx + bound) < state->chunkOffsetTo.offset.vx)
    {
        return;
    }

    if (state->chunkOffsetFrom.offset.vz < (state->point.offset.vz - bound) &&
        state->chunkOffsetTo.offset.vz   < (state->point.offset.vz - bound))
    {
        return;
    }

    if ((state->point.offset.vz + bound) < state->chunkOffsetFrom.offset.vz &&
        (state->point.offset.vz + bound) < state->chunkOffsetTo.offset.vz)
    {
        return;
    }

    distX = state->chunkOffsetFrom.offset.vx - state->point.offset.vx;
    distZ = state->chunkOffsetFrom.offset.vz - state->point.offset.vz;
    dist  = Math_Vector2MagCalc(distX, distZ);

    if (dist < state->point.shapeAttribs.cylinder.radius && state->point.disableHeight != true &&
        (state->shapeIdx == UCHAR_MAX || state->point.offset.vy < state->floorHeight))
    {
        state->shapeIdx    = state->point.shapeIdx;
        state->floorHeight = state->point.offset.vy;
    }

    if ((!state->isCharaMoving && !state->enablePush) || dist < state->point.shapeAttribs.cylinder.radius)
    {
        return;
    }

    if (dist < distMax && state->enablePush)
    {
        func_8006C794(state, CollisionPushType_Hard, dist);
        return;
    }

    if (dist < (distMax + PROXIMITY_BUFFER) && state->enablePush)
    {
        func_8006C794(state, CollisionPushType_Soft, dist);
    }

    if (!state->isCharaMoving || state->obstacleContext.hardPushConstraint.shapeCount)
    {
        return;
    }

    hitAlpha = func_8006C248(*(s32*)&state->charaState.moveDirection, state->charaState.moveDistance,
                              state->point.offset.vx - state->chunkOffsetFrom.offset.vx,
                              state->point.offset.vz - state->chunkOffsetFrom.offset.vz,
                              distMax);

    if (hitAlpha == NO_VALUE)
    {
        return;
    }

    if (hitAlpha < Q12(0.0f))
    {
        hitAlpha = Q12(0.0f);
    }

    if (func_8006C1B8(1, hitAlpha, state) && state->charaState.bottom >= state->point.offset.vy)
    {
        state->hitAlpha  = hitAlpha;
        state->field_34  = CollisionContactType_1;
        temp             = state->chunkOffsetFrom.offset.vx + Q12_MULT(state->charaState.moveOffset.vx, hitAlpha);
        state->hitDistance  = Q12_TO_Q4(state->charaState.moveDistance * hitAlpha);
        state->passStamp = &state->point.ipdCollisionData->shapePassStampBuffer[state->point.shapeIdx];
        state->field_3C  = temp - state->point.offset.vx;
        temp2            = state->chunkOffsetFrom.offset.vz + Q12_MULT(state->charaState.moveOffset.vz, hitAlpha);
        state->field_3E  = temp2 - state->point.offset.vz;
    }
}

void func_8006C794(s_CollisionQuery* state, s32 pushType, q23_8 dist) // 0x8006C794
{
    if (state->charaState.bottom >= (state->point.offset.vy + (dist - state->point.shapeAttribs.cylinder.radius)))
    {
        func_8006BCC4(&state->obstacleContext,
                      &state->point.ipdCollisionData->shapePassStampBuffer[state->point.shapeIdx],
                      pushType,
                      state->chunkOffsetFrom.offset.vx - state->point.offset.vx,
                      state->chunkOffsetFrom.offset.vz - state->point.offset.vz,
                      (state->charaState.radius + state->point.shapeAttribs.cylinder.radius) - dist);
    }
}

void func_8006C838(s_CollisionQuery* state, s_IpdCollisionData* collData) // 0x8006C838
{
    q19_12                    slopedFloorHeight;
    s_CollisionCachedSurface* curCachedSurface;
    s_IpdSurface*             curSurface;
    s_IpdCylinder*            cylinder;

    if (state->context.shape.shapeAlloc == NULL)
    {
        return;
    }

    if (state->shapeIdx != UCHAR_MAX && state->floorHeight < state->slopedFloorHeight)
    {
        cylinder                 = &collData->cylinders[state->shapeIdx - collData->polygonCount];
        state->slopedFloorHeight = state->floorHeight;
        state->positionX         = state->chunkOffsetFrom.offset.vx + collData->positionX;
        state->positionZ         = state->chunkOffsetFrom.offset.vz + collData->positionZ;
        state->slopeGradientX    = Q8(0.0f);
        state->slopeGradientZ    = Q8(0.0f);
        state->disableHeight     = cylinder->disableHeight;
        state->floorType         = cylinder->floorType;
    }

    // Run through cached surfaces.
    for (curCachedSurface = &state->context.shape.surfaceCache[0];
         curCachedSurface < &state->context.shape.surfaceCache[4];
         curCachedSurface++)
    {
        if (curCachedSurface->surfaceIdx != UCHAR_MAX)
        {
            curSurface = &collData->polygonSurfaces[curCachedSurface->surfaceIdx];

            // Compute sloped floor height.
            slopedFloorHeight = curSurface->offset.vy;
            if (curSurface->slopeGradientX != Q8(0.0f))
            {
                slopedFloorHeight += Q12_MULT(curSurface->slopeGradientX,
                                              state->chunkOffsetFrom.offset.vx - curSurface->offset.vx);
            }
            if (curSurface->slopeGradientZ != Q8(0.0f))
            {
                slopedFloorHeight += Q12_MULT(curSurface->slopeGradientZ,
                                              state->chunkOffsetFrom.offset.vz - curSurface->offset.vz);
            }

            if (slopedFloorHeight < state->slopedFloorHeight)
            {
                state->slopedFloorHeight = slopedFloorHeight;
                state->positionX         = state->chunkOffsetFrom.offset.vx + collData->positionX;
                state->positionZ         = state->chunkOffsetFrom.offset.vz + collData->positionZ;
                state->slopeGradientX    = curSurface->slopeGradientX;
                state->slopeGradientZ    = curSurface->slopeGradientZ;
                state->disableHeight     = curSurface->disableHeight;
                state->floorType         = curSurface->floorType;
            }
        }
    }
}

void func_8006CA18(s_CollisionQuery* state, s_IpdCollisionData* collData, s_IpdShapeAlloc* shapeAlloc) // 0x8006CA18
{
    s32           surfaceLutStartIdx;
    s32           surfaceLutEndIdx;
    q23_8         slopedFloorHeight;
    u8*           curSurfaceIdx;
    s_IpdSurface* curSurface;

    surfaceLutStartIdx = shapeAlloc[0].surfaceLutIdx;
    surfaceLutEndIdx   = shapeAlloc[1].surfaceLutIdx;

    // Check for invalid surface LUT range.
    if (surfaceLutStartIdx == surfaceLutEndIdx)
    {
        return;
    }

    // Run through allocated cell surfaces.
    for (curSurfaceIdx = &collData->surfaceLut[surfaceLutStartIdx];
         curSurfaceIdx < &collData->surfaceLut[surfaceLutEndIdx];
         curSurfaceIdx++)
    {
        // Check if surface is collidable.
        curSurface = &collData->polygonSurfaces[*curSurfaceIdx];
        if (CHECK_FLAG_IDX(state->triggerFlags, curSurface->triggerFlagIdx, false) ||
            curSurface->disableHeight == true)
        {
            continue;
        }

        // Compute sloped floor height.
        slopedFloorHeight = curSurface->offset.vy;
        if (curSurface->slopeGradientX != Q8(0.0f))
        {
            slopedFloorHeight += Q12_MULT(curSurface->slopeGradientX, state->chunkOffsetFrom.offset.vx - curSurface->offset.vx);
        }
        if (curSurface->slopeGradientZ != Q8(0.0f))
        {
            slopedFloorHeight += Q12_MULT(curSurface->slopeGradientZ, state->chunkOffsetFrom.offset.vz - curSurface->offset.vz);
        }

        if (slopedFloorHeight < state->slopedFloorHeight)
        {
            state->slopedFloorHeight = slopedFloorHeight;
            state->positionX  = state->chunkOffsetFrom.offset.vx + collData->positionX;
            state->positionZ  = state->chunkOffsetFrom.offset.vz + collData->positionZ;
            state->slopeGradientX    = curSurface->slopeGradientX;
            state->slopeGradientZ    = curSurface->slopeGradientZ;
            state->disableHeight     = curSurface->disableHeight;
            state->floorType         = curSurface->floorType;
        }
    }
}

q3_12 Collision_OffsetAlphaGet(s_CollisionQuery* state) // 0x8006CB90
{
    q23_8 floorHeight;

    if (state->slopedFloorHeight == Q8(DEFAULT_FLOOR_HEIGHT))
    {
        return Q12(1.0f);
    }

    floorHeight = Ipd_FloorHeightGet(state->charaState.positionToX, state->charaState.positionToZ, state);
    if ((state->charaState.bottom + state->charaState.moveOffset.vy) < floorHeight ||
        floorHeight == state->slopedFloorHeight)
    {
        return Q12(1.0f);
    }

    return Q12_DIV(state->charaState.moveDistance, Math_Vector2MagCalc(state->charaState.moveDistance,
                                                                   floorHeight - state->charaState.bottom));
}

q23_8 Ipd_FloorHeightGet(q23_8 posX, q23_8 posZ, const s_CollisionQuery* state) // 0x8006CC44
{
    if (state->floorType != FloorType_None)
    {
        return Q12_MULT(state->slopeGradientX, posX - state->positionX) +
               Q12_MULT(state->slopeGradientZ, posZ - state->positionZ) +
               state->slopedFloorHeight;
    }

    return Q8(8.0f);
}

void func_8006CC9C(s_CollisionQuery* state) // 0x8006CC9C
{
    q23_8  deltaX;
    q23_8  deltaZ;
    q19_12 hitAlpha;
    q23_8  temp_s4;
    q23_8  temp;
    q23_8  temp2;
    q23_8  radiusOffset;
    q23_8  temp4;
    q23_8  targetCharaBottom;

    if (state->context.chara.state < CharaCollisionState_2 ||
        *state->context.chara.passStamp != 0)
    {
        return;
    }

    if (state->charaState.positionFromX > (state->chunkOffsetFrom.vx + (state->context.chara.radius + state->charaState.moveDistance)) ||
        state->charaState.positionToX   < (state->chunkOffsetFrom.vx - (state->context.chara.radius + state->charaState.moveDistance)))
    {
        return;
    }

    if (state->charaState.positionFromZ > (state->chunkOffsetTo.vz + (state->context.chara.radius + state->charaState.moveDistance)) ||
        state->charaState.positionToZ   < (state->chunkOffsetTo.vz - (state->context.chara.radius + state->charaState.moveDistance)) ||
        state->charaState.top > state->context.chara.top)
    {
        return;
    }

    deltaX = (state->charaState.positionFromX - state->chunkOffsetFrom.vx);

    if (state->charaState.bottom < state->context.chara.bottom != Q8(0.0f))
    {
        return;
    }

    deltaZ = state->charaState.positionFromZ - state->chunkOffsetTo.vz;
    temp_s4 = Math_Vector2MagCalc(deltaX, deltaZ);

    hitAlpha = func_8006C248(*(s32*)&state->charaState.moveDirection, state->charaState.moveDistance,
                              state->chunkOffsetFrom.vx - state->charaState.positionFromX,
                              state->chunkOffsetTo.vz   - state->charaState.positionFromZ,
                              state->context.chara.radius);
    if (hitAlpha == NO_VALUE)
    {
        return;
    }

    if (hitAlpha == Q12(0.0f))
    {
        if (state->enablePush)
        {
            radiusOffset = state->context.chara.radius - temp_s4;
            func_8006BCC4(&state->obstacleContext, state->context.chara.passStamp,
                          CollisionPushType_Chara, deltaX, deltaZ, radiusOffset);
        }
    }
    else if (state->isCharaMoving && state->obstacleContext.hardPushConstraint.shapeCount == 0 &&
             func_8006C1B8(1, hitAlpha, state))
    {
        temp2 = (state->charaState.positionFromZ - state->chunkOffsetTo.vz);
        targetCharaBottom = Q12_MULT(hitAlpha, state->charaState.moveOffset.vz);

        state->passStamp = state->context.chara.passStamp;
        state->hitAlpha  = hitAlpha;

        state->field_34 = CollisionContactType_1;

        temp  = (state->charaState.positionFromX - state->chunkOffsetFrom.vx);
        temp4 = Q12_MULT(hitAlpha, state->charaState.moveOffset.vx);

        state->hitDistance = Q12_TO_Q4(state->charaState.moveDistance * hitAlpha);
        state->field_3C = temp + temp4;
        state->field_3E = temp2 + targetCharaBottom;
    }
}

void func_8006CF18(s_CollisionQuery* state, s_CharaCollisionRecord* charaRecords, s32 charaRecordCount) // 0x8006CF18
{
    q23_8                   radius;
    s_CharaCollisionRecord* curCharaRecord;

    // Run through character records.
    for (curCharaRecord = charaRecords; curCharaRecord < &charaRecords[charaRecordCount]; curCharaRecord++)
    {
        radius = Q12_TO_Q8(curCharaRecord->radius) + state->charaState.radius;
        if (curCharaRecord->state < (u32)state->charaState.state)
        {
            radius -= Q8(0.06f);
        }

        state->chunkOffsetFrom.vx = Q12_TO_Q8(curCharaRecord->position.vx);
        state->chunkOffsetTo.vz   = Q12_TO_Q8(curCharaRecord->position.vz);

        state->context.chara.bottom    = Q12_TO_Q8(curCharaRecord->topExtent    + curCharaRecord->position.vy);
        state->context.chara.top       = Q12_TO_Q8(curCharaRecord->bottomExtent + curCharaRecord->position.vy);
        state->context.chara.radius    = radius;
        state->context.chara.state     = curCharaRecord->state;
        state->context.chara.passStamp = &curCharaRecord->passStamp;

        if (!state->offsetX)
        {
            curCharaRecord->passStamp = 0;
        }

        func_8006CC9C(state);
    }
}

void func_8006D01C(VECTOR3* extraOffset, VECTOR3* offset, q3_12 offsetAlpha, s_CollisionQuery* state) // 0x8006D01C
{
    VECTOR3 adjOffset;
    s32     temp_s0;
    s32     temp_s1;
    s32     temp_a0;
    s32     temp_v0;

    adjOffset.vx = Q12_MULT(offset->vx, offsetAlpha);
    adjOffset.vz = Q12_MULT(offset->vz, offsetAlpha);

    if (state->obstacleContext.hardPushConstraint.shapeCount  != 0||
        state->obstacleContext.charaPushConstraint.shapeCount != 0)
    {
        extraOffset->vx = Q12(0.0f);
        extraOffset->vz = Q12(0.0f);
        *offset         = adjOffset;
        func_8006D2B4(offset, &state->obstacleContext);
        return;
    }

    if (state->field_34 == CollisionContactType_None)
    {
        *extraOffset = adjOffset;
        offset->vz   = Q12(0.0f);
        offset->vx   = Q12(0.0f);
        return;
    }

    if (offsetAlpha < state->hitAlpha)
    {
        state->field_34 = CollisionContactType_None;
        *extraOffset    = adjOffset;
        offset->vz      = Q12(0.0f);
        offset->vx      = Q12(0.0f);
        return;
    }

    extraOffset->vx = Q12_MULT(offset->vx, state->hitAlpha);
    extraOffset->vz = Q12_MULT(offset->vz, state->hitAlpha);
    offset->vx      = adjOffset.vx - extraOffset->vx;
    offset->vz      = adjOffset.vz - extraOffset->vz;

    temp_s0 = state->field_3C;
    temp_s1 = state->field_3E;
    temp_a0 = SQUARE(temp_s0) + SQUARE(temp_s1);

    if (temp_a0 < 256)
    {
        temp_a0 = SquareRoot0(temp_a0 * 256);
        temp_s0 = (temp_s0 << 16) / temp_a0;
        temp_s1 = (temp_s1 << 16) / temp_a0;
    }
    else
    {
        temp_a0 = SquareRoot0(temp_a0);
        temp_s0 = (temp_s0 << 12) / temp_a0; // `Q12_SHIFT`?
        temp_s1 = (temp_s1 << 12) / temp_a0;
    }

    temp_v0  = FP_FROM((offset->vx * temp_s1) + (offset->vz * -temp_s0), Q12_SHIFT);
    offset->vx = Q12_MULT(temp_v0, temp_s1);
    offset->vz = Q12_MULT(temp_v0, -temp_s0);

    if (temp_s0 > Q12(1.0f / 3.0f))
    {
        extraOffset->vx += Q12(1.0f / 256.0f);
    }
    else if (temp_s0 < Q12(-1.0f / 3.0f))
    {
        extraOffset->vx -= Q12(1.0f / 256.0f);
    }

    if (temp_s1 > Q12(1.0f / 3.0f))
    {
        extraOffset->vz += Q12(1.0f / 256.0f);
    }
    else if (temp_s1 < Q12(-1.0f / 3.0f))
    {
        extraOffset->vz -= Q12(1.0f / 256.0f);
    }
}

void func_8006D2B4(VECTOR3* offset, s_CollisionObstacleContext* obstacleContext) // 0x8006D2B4
{
    #define PUSH_BUFFER Q8(1.0f / 128.0f)

    q3_12  angleMin1;
    q3_12  angleMax1;
    q3_12  angleMin;
    q3_12  angleMax;
    q3_12  angle1;
    q19_12 unkAngleMax;
    bool   cond;
    bool   var_s1;
    q3_12  angle;
    q19_12 unkAngleMin;
    q7_8   dist;
    q7_8   intersectDepth;
    q3_12  angle2;
    q3_12  angle3;

    if (obstacleContext->charaPushConstraint.shapeCount != 0)
    {
        var_s1 = false;
        if (obstacleContext->hardPushConstraint.shapeCount != 0)
        {
            angleMin1 = obstacleContext->hardPushConstraint.angleMin;
            angleMax1 = obstacleContext->hardPushConstraint.angleMax;
            if (obstacleContext->softPushConstraint.shapeCount != 0)
            {
                Vw_ClampAngleRange(&angleMin1, &angleMax1,
                                   obstacleContext->softPushConstraint.angleMin,
                                   obstacleContext->softPushConstraint.angleMax);
            }
        }
        else if (obstacleContext->softPushConstraint.shapeCount != 0)
        {
            angleMin1 = obstacleContext->softPushConstraint.angleMin;
            angleMax1 = obstacleContext->softPushConstraint.angleMax;
        }
        else
        {
            var_s1 = true;
        }

        if (var_s1)
        {
            cond = true;
        }
        else
        {
            angle1  = ratan2(offset->vz, offset->vx);
            angle = Q12_ANGLE(90.0f);
            angle2  = Q12_ANGLE_NORM_S(angleMax1 - (angle1 - angle));
            angle3  = Q12_ANGLE_NORM_S(angleMin1 - (angle1 + angle));
            if (angle2 < Q12_ANGLE(0.0f) && angle3 > Q12_ANGLE(0.0f))
            {
                cond = false;
            }
            else
            {
                cond = true;
            }
        }

        if (cond)
        {
            if (obstacleContext->hardPushConstraint.shapeCount == 0)
            {
                obstacleContext->hardPushConstraint = obstacleContext->charaPushConstraint;
                obstacleContext->hardIntersectDepth = obstacleContext->charaIntersectDepth;
            }
            else
            {
                obstacleContext->hardPushConstraint.shapeCount += obstacleContext->charaPushConstraint.shapeCount;

                Vw_ClampAngleRange(&obstacleContext->hardPushConstraint.angleMin, &obstacleContext->hardPushConstraint.angleMax,
                                   obstacleContext->charaPushConstraint.angleMin, obstacleContext->charaPushConstraint.angleMax);

                intersectDepth = obstacleContext->hardIntersectDepth;
                if (obstacleContext->hardIntersectDepth < obstacleContext->charaIntersectDepth)
                {
                    intersectDepth = obstacleContext->charaIntersectDepth;
                }
                obstacleContext->hardIntersectDepth = intersectDepth;
            }
        }

        if (obstacleContext->softPushConstraint.shapeCount != 0)
        {
            if (obstacleContext->hardPushConstraint.shapeCount == 0)
            {
                obstacleContext->hardPushConstraint = obstacleContext->softPushConstraint;
                obstacleContext->hardIntersectDepth = Q8(0.0f);
            }
            else
            {
                obstacleContext->hardPushConstraint.shapeCount += obstacleContext->softPushConstraint.shapeCount;
                Vw_ClampAngleRange(&obstacleContext->hardPushConstraint.angleMin, &obstacleContext->hardPushConstraint.angleMax,
                                   obstacleContext->softPushConstraint.angleMin, obstacleContext->softPushConstraint.angleMax);
            }
        }
    }

    if (obstacleContext->hardPushConstraint.shapeCount != 0)
    {
        if (obstacleContext->hardPushConstraint.angleMin == obstacleContext->hardPushConstraint.angleMax)
        {
            offset->vx = Q12(0.0f);
            offset->vz = Q12(0.0f);
            return;
        }

        angle = DIV_FAST(obstacleContext->hardPushConstraint.angleMin + obstacleContext->hardPushConstraint.angleMax, 2);
        if (obstacleContext->hardPushConstraint.angleMax < obstacleContext->hardPushConstraint.angleMin)
        {
            angle = Q12_FRACT(angle + Q12_ANGLE(180.0f));
        }

        if (obstacleContext->softPushConstraint.shapeCount == 0)
        {
            angleMin = obstacleContext->hardPushConstraint.angleMin;
            angleMax = obstacleContext->hardPushConstraint.angleMax;
        }
        else
        {
            angleMin = obstacleContext->hardPushConstraint.angleMin;
            angleMax = obstacleContext->hardPushConstraint.angleMax;

            Vw_ClampAngleRange(&angleMin, &angleMax,
                               obstacleContext->softPushConstraint.angleMin, obstacleContext->softPushConstraint.angleMax);

            angleMin = Q12_FRACT(angleMin);
            angleMax = Q12_FRACT(angleMax);
            if (angleMin == angleMax)
            {
                angleMin = obstacleContext->hardPushConstraint.angleMin;
                angleMax = obstacleContext->hardPushConstraint.angleMax;
            }

            if (angleMin != angleMax)
            {
                unkAngleMin = Q12_ANGLE_NORM_S(angleMin - angle);
                unkAngleMax = Q12_ANGLE_NORM_S(angleMax - angle);
                if (unkAngleMin >= Q12_ANGLE(0.0f) || unkAngleMax <= Q12_ANGLE(0.0f))
                {
                    if (ABS(unkAngleMin) < ABS(unkAngleMax))
                    {
                        angle = angleMin;
                    }
                    else
                    {
                        angle = angleMax;
                    }
                }
            }
        }

        dist = Q8_TO_Q12(MIN(obstacleContext->hardIntersectDepth + PUSH_BUFFER, 16));
        func_8006D600(offset, angle, angleMin, angleMax, dist);
    }

    #undef PUSH_BUFFER
}

void func_8006D600(VECTOR3* pos, q19_12 angle, q19_12 angleMin, q19_12 angleMax, q19_12 dist) // 0x8006D600
{
    q19_12 angle3;
    q3_12  normAngle;
    q3_12  normAngleMin;
    q3_12  normAngleMax;
    q19_12 sinAngle2;
    q19_12 deltaX;
    q19_12 deltaZ;
    q19_12 z;
    q19_12 x;
    q19_12 cosAngle2;
    q19_12 angle2;
    q19_12 dist0;
    q19_12 angle1;

    normAngle    = Q12_FRACT(angle);
    normAngleMin = Q12_FRACT(angleMin);
    normAngleMax = Q12_FRACT(angleMax);

    if (dist > Q12(1.0f / 16.0f))
    {
        dist = Q12(1.0f / 16.0f);
    }

    x      = Q12_MULT(dist, Math_Cos(normAngle));
    angle2 = normAngleMax;
    z      = Q12_MULT(dist, Math_Sin(normAngle));
    deltaX = pos->vx - x;
    deltaZ = pos->vz - z;
    angle1 = Q12_FRACT(ratan2(deltaZ, deltaX));

    if (angle1 < normAngleMin)
    {
        angle1 += Q12_ANGLE(360.0f);
    }

    if (angle2 < normAngleMin)
    {
        angle2 += Q12_ANGLE(360.0f);
    }

    angle3 = normAngleMin + Q12_ANGLE(360.0f);
    if (angle2 < angle1)
    {
        if (((angle2 + angle3) >> 1) < angle1)
        {
            angle2 = angle3;
        }

        sinAngle2 = Math_Sin(angle2);
        cosAngle2 = Math_Cos(angle2);

        dist0 = Q12_MULT(deltaX, cosAngle2) + Q12_MULT(deltaZ, sinAngle2);
        if (dist0 < Q12(0.0f))
        {
            dist0 = Q12(0.0f);
        }

        pos->vx = x + Q12_MULT(dist0, cosAngle2);
        pos->vz = z + Q12_MULT(dist0, sinAngle2);
    }
}

void func_8006D774(s_CollisionQuery* state, const VECTOR3* offset0, const VECTOR3* offset1) // 0x8006D774
{
    SVECTOR q8Offset0; // Q7.8
    SVECTOR q8Offset1; // Q7.8

    q8Offset0.vx = Q12_TO_Q8(offset0->vx);
    q8Offset0.vy = Q12_TO_Q8(offset0->vz);
    q8Offset1.vx = Q12_TO_Q8(offset1->vx);
    q8Offset1.vy = Q12_TO_Q8(offset1->vz);

    state->field_34                                       = CollisionContactType_None;
    state->obstacleContext.hardPushConstraint.shapeCount  = 0;
    state->obstacleContext.hardIntersectDepth             = Q8(0.0f);
    state->obstacleContext.softPushConstraint.shapeCount  = 0;
    state->obstacleContext.charaIntersectDepth            = Q8(0.0f);
    state->obstacleContext.charaPushConstraint.shapeCount = 0;

    func_8006D7EC(&state->charaState, &q8Offset0, &q8Offset1);
}

void func_8006D7EC(s_CollisionCharaState* charaState, const SVECTOR* offset0, const SVECTOR* offset1) // 0x8006D7EC
{
    q3_12  headingAngle;
    q19_12 moveDist;
    q7_8   offsetY;

    charaState->moveOffset.vx = offset1->vx;
    offsetY                   = offset1->vy;
    charaState->moveOffset.vz = offset1->vy;

    moveDist                 = Math_Vector2MagCalc(charaState->moveOffset.vx, offsetY);
    charaState->moveDistance = moveDist;

    if (moveDist != Q12(0.0f))
    {
        charaState->moveDirection.vx = Q12_DIV(charaState->moveOffset.vx, moveDist);
        charaState->moveDirection.vz = Q12_DIV(charaState->moveOffset.vz, charaState->moveDistance);

        headingAngle             = ratan2(charaState->moveOffset.vz, charaState->moveOffset.vx);
        charaState->moveDirection.vx = Math_Cos(headingAngle);
        charaState->moveDirection.vz = Math_Sin(headingAngle);
    }
    else
    {
        charaState->moveDirection.vx = Q12(1.0f);
        charaState->moveDirection.vz = Q12(0.0f);
    }

    charaState->positionFromX = charaState->positionFromX + offset0->vx;
    charaState->positionFromZ = charaState->positionFromZ + offset0->vy;
    charaState->positionToX   = charaState->positionFromX + charaState->moveOffset.vx;
    charaState->positionToZ   = charaState->positionFromZ + charaState->moveOffset.vz;
}

// Padding with garbage data.
#if VERSION_IS(USA)
    static const u8 hack_D_80028B8C_fix[] = { 0x00, 0x42, 0x05, 0x80, 0x00, 0x00, 0x00, 0x00 };
#elif VERSION_IS(JAP0)
    static const u8 hack_D_80028B8C_fix[] = { 0x00, 0x20, 0x3D, 0x20, 0x00, 0x00, 0x00, 0x00 };
#elif VERSION_IS(JAP1)
    static const u8 hack_D_80028B8C_fix[] = { 0x00, 0x5E, 0x05, 0x80, 0x00, 0x00, 0x00, 0x00 };
#elif VERSION_IS(JAP2)
    static const u8 hack_D_80028B8C_fix[] = { 0x00, 0x00, 0x42, 0x24, 0x00, 0x00, 0x00, 0x00 };
#elif VERSION_IS(EUR)
    static const u8 hack_D_80028B8C_fix[] = { 0x00, 0x3C, 0x05, 0x80, 0x00, 0x00, 0x00, 0x00 };
#endif
