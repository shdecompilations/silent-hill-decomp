#include "game.h"
#include "inline_no_dmpsx.h"

#include <psyq/libapi.h>
#include <psyq/strings.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/collision/collision.h"
#include "bodyprog/math/math.h"
#include "main/rng.h"

#define PROXIMITY_BUFFER                  (1.0f / 32.0f)
#define CHARA_RADIUS_BUFFER               0.06f
#define HARD_SHAPE_INTERSECT_DEPTH_BUFFER (1.0f / 16.0f)

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
        surface->floorHeight = Q8_TO_Q12(Ipd_FloorHeightGet(query.charaQuery.positionFromX,
                                                            query.charaQuery.positionFromZ,
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
        if (chara == &PLAYER_CHARA() && chara->health > Q12(0.0f))
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
    if (baseFloorHeight < (floorHeight - Q12(INTERSECTION_BUFFER)))
    {
        baseFloorHeight = floorHeight - Q12(INTERSECTION_BUFFER);
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

    // TODO: Condition related to ceiling detection, exact purpose unclear. See `s_CharaCollisionQuery::blockInvalidFloor`.
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
    for (curChara = &NPC_CHARA(0); curChara < &NPC_CHARA(ARRAY_SIZE(g_SysWork.npcs)); curChara++)
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
    curChara = &PLAYER_CHARA();
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

    // Accumulate pushes?
    while (true)
    {
        if (query.offsetX)
        {
            query.isCharaMoving = query.charaQuery.moveDistance != Q12(0.0f);
            query.enablePush    = false;
            query.field_0_10    = false;
        }
        else
        {
            query.isCharaMoving = query.charaQuery.moveDistance != Q12(0.0f);
            query.enablePush    = true;
            query.field_0_10    = true;
        }

        // Run through collision data.
        for (curCollData = collDataPtrs; curCollData < &collDataPtrs[collDataIdx]; curCollData++)
        {
            Collision_CharaCollisionHandle(&query, *curCollData);
        }

        if (query.obstacleContext.hardPushConstraint.shapeCount != 0 &&
            query.obstacleContext.hardPushConstraint.angleMin == query.obstacleContext.hardPushConstraint.angleMax)
        {
            cond |= true;
        }

        func_8006CF18(&query, charaRecords, charaRecordCount);

        // Run through characters.
        for (curChara = charas; curChara < &charas[charaCount]; curChara++)
        {
            chara = *curChara;

            radius = Q12_TO_Q8(chara->collision.cylinder.outerRadius) + query.charaQuery.radius;
            if (chara->collision.state < (u32)query.charaQuery.state)
            {
                radius -= Q8(CHARA_RADIUS_BUFFER);
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
            if (query.obstacleContext.softPushConstraint.shapeCount <= ARRAY_SIZE(query.obstacleContext.shapePassStamps))
            {
                count = query.obstacleContext.softPushConstraint.shapeCount;
            }

            for (i = 0; i < count; i++)
            {
                *query.obstacleContext.shapePassStamps[i] += 1;
            }
        }
        else if (query.contactType != CollisionContactType_None)
        {
            *query.passStamp += 1;
        }
        else
        {
            break;
        }

        query.offsetX = true;
        Collision_CharaRawMovementSet(&query, &extraOffset, &moveOffsetCpy1);
    }

    if (query.disableHeight == true)
    {
        floorHeight                   = Q12(8.0f);
        collResult->surface.floorType = FloorType_Default;
    }
    else
    {
        collResult->surface.floorType = query.floorType;
        floorHeight                   = Q8_TO_Q12(Ipd_FloorHeightGet(query.charaQuery.positionFromX + Q12_TO_Q8(extraOffset.vx),
                                                                     query.charaQuery.positionFromZ + Q12_TO_Q8(extraOffset.vz), &query));
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
    #define DEFAULT_SLOWDOWN_MAX          Q12(0.4f)
    #define HANGED_SCRATCHER_SLOWDOWN_MAX Q12(0.6f)
    
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
        if (((curChara->collision.cylinder.outerRadius + cylinder->radius) + Q12(INTERSECTION_BUFFER)) < dist)
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
            collAngleDelta = MIN(collAngleDelta, HANGED_SCRATCHER_SLOWDOWN_MAX);
        }
        else
        {
            collAngleDelta = MIN(collAngleDelta, DEFAULT_SLOWDOWN_MAX);
        }

        // Adjust slowdown alpha.
        slowdownAlpha -= collAngleDelta;
    }

    // Apply slowdown to displacement offset.
    slowdownAlpha  = MAX(slowdownAlpha, DEFAULT_SLOWDOWN_MAX);
    moveOffset->vx = Q12_MULT(slowdownAlpha, moveOffset->vx);
    moveOffset->vz = Q12_MULT(slowdownAlpha, moveOffset->vz);

    #undef DEFAULT_SLOWDOWN_MAX
    #undef HANGED_SCRATCHER_SLOWDOWN_MAX
}

void Collision_CollStateInit(s_CollisionQuery* query, VECTOR3* moveOffset,
                             const s_CharaCollisionCylinder* cylinder, bool blockInvalidFloor) // 0x8006AB50
{
    query->offsetX                      = false;
    query->triggerFlags                 = g_ActiveCollisionTriggers.flags;
    query->charaQuery.blockInvalidFloor = blockInvalidFloor;

    Collision_MoveDirectionCalc(&query->charaQuery, moveOffset, cylinder);

    query->slopedFloorHeight = Q8(DEFAULT_FLOOR_HEIGHT);
    query->contactType       = CollisionContactType_None;
    
    query->obstacleContext.hardPushConstraint.shapeCount  = 0;
    query->obstacleContext.hardIntersectDepth             = Q8(0.0f);
    query->obstacleContext.softPushConstraint.shapeCount  = 0;
    query->obstacleContext.charaIntersectDepth            = Q8(0.0f);
    query->obstacleContext.charaPushConstraint.shapeCount = 0;
    
    query->slopeGradientZ = Q8(0.0f);
    query->slopeGradientX = Q8(0.0f);
    query->disableHeight  = true;
    query->floorType      = FloorType_Default;
}

void Collision_MoveDirectionCalc(s_CharaCollisionQuery* query,
                                 const VECTOR3* moveOffset, const s_CharaCollisionCylinder* cylinder) // 0x8006ABC0
{
    q3_12 headingAngle;

    query->moveOffset.vx = Q12_TO_Q8(moveOffset->vx);
    query->moveOffset.vy = Q12_TO_Q8(moveOffset->vy);
    query->moveOffset.vz = Q12_TO_Q8(moveOffset->vz);

    query->moveDistance = Math_Vector2MagCalc(query->moveOffset.vx, query->moveOffset.vz);
    if (query->moveDistance != Q12(0.0f))
    {
        // @unused
        query->moveDirection.vx = Q12(query->moveOffset.vx) / query->moveDistance;
        query->moveDirection.vz = Q12(query->moveOffset.vz) / query->moveDistance;

        headingAngle                 = ratan2(query->moveOffset.vz, query->moveOffset.vx);
        query->moveDirection.vx = Math_Cos(headingAngle);
        query->moveDirection.vz = Math_Sin(headingAngle);
    }
    else
    {
        query->moveDirection.vx = Q12(1.0f);
        query->moveDirection.vz = Q12(0.0f);
    }

    query->radius        = Q12_TO_Q8(cylinder->radius);
    query->positionFromX = Q12_TO_Q8(cylinder->position.vx);
    query->positionFromZ = Q12_TO_Q8(cylinder->position.vz);
    query->positionToX   = query->positionFromX + query->moveOffset.vx;
    query->positionToZ   = query->positionFromZ + query->moveOffset.vz;
    query->top           = Q12_TO_Q8(cylinder->topExtent    + cylinder->position.vy);
    query->bottom        = Q12_TO_Q8(cylinder->bottomExtent + cylinder->position.vy);
    query->state         = cylinder->state;
}

void Collision_CharaCollisionHandle(s_CollisionQuery* query, s_IpdCollisionData* collData) // 0x8006AD44
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
        !Collision_CellInit(query, collData))
    {
        return;
    }

    if (!query->offsetX)
    {
        Collision_ShapePassReset(collData);
    }

    // Run through neighboring cells.
    startCellX = query->context.shape.cellX;
    endCellX   = (query->context.shape.cellX + query->context.shape.cellSpanX) - 1;
    for (curCellZ = query->context.shape.cellZ;
         curCellZ < (query->context.shape.cellZ + query->context.shape.cellSpanZ);
         curCellZ++)
    {
        shapeAlloc = &collData->shapeAllocs[(curCellZ * collData->cellCountX) + startCellX];

        for (curCellX = startCellX;
             curCellX <= endCellX;
             curCellX++, shapeAlloc++)
        {
            func_8006B1C8(query, collData, shapeAlloc);
        }
    }

    if (!query->offsetX)
    {
        Collision_ShapePassUpdate(collData);
    }

    // TODO: Something that handles height and movement on slopes.
    if (query->field_0_10)
    {
        func_8006C838(query, collData);
        if (query->context.shape.shapeAlloc != NULL)
        {
            func_8006CA18(query, collData, query->context.shape.shapeAlloc);
        }
    }
}

bool Collision_CellInit(s_CollisionQuery* query, const s_IpdCollisionData* collData) // 0x8006AEAC
{
    s_CollisionCachedSurface* curCachedSurface;

    if (!Collision_CellIdxGet(query, collData))
    {
        return false;
    }

    query->chunkOffsetFrom.offset.vx = query->charaQuery.positionFromX - collData->positionX;
    query->chunkOffsetFrom.offset.vz = query->charaQuery.positionFromZ - collData->positionZ;
    query->chunkOffsetTo.offset.vx   = query->charaQuery.positionToX   - collData->positionX;
    query->chunkOffsetTo.offset.vz   = query->charaQuery.positionToZ   - collData->positionZ;
    
    if ((query->chunkOffsetFrom.offset.vx / collData->cellSize) <  0                    ||
        (query->chunkOffsetFrom.offset.vx / collData->cellSize) >= collData->cellCountX ||
        (query->chunkOffsetFrom.offset.vz / collData->cellSize) <  0                    ||
        (query->chunkOffsetFrom.offset.vz / collData->cellSize) >= collData->cellCountZ)
    {
        query->context.shape.shapeAlloc = NULL;
    }
    else
    {
        query->context.shape.shapeAlloc = &collData->shapeAllocs[((query->chunkOffsetFrom.offset.vz / collData->cellSize) * collData->cellCountX) +
                                                                    (query->chunkOffsetFrom.offset.vx / collData->cellSize)];
        query->shapeIdx                 = NO_VALUE;

        // Run through cached surfaces.
        for (curCachedSurface = &query->context.shape.surfaceCache[0];
             curCachedSurface < &query->context.shape.surfaceCache[4];
             curCachedSurface++)
        {
            curCachedSurface->subChunkTransDir   = SubChunkTransitionDirection_None;
            curCachedSurface->surfaceIdx         = NO_VALUE;
            curCachedSurface->radiusCollDiffDist = INT_MAX;
        }
    }

    return true;
}

bool Collision_CellIdxGet(s_CollisionQuery* query, const s_IpdCollisionData* collData) // 0x8006B004
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
    charaCollDiffXClosest  = query->charaQuery.positionFromX - collData->positionX;
    charaCollDiffXFarthest = query->charaQuery.positionToX   - collData->positionX;
    if (charaCollDiffXFarthest < charaCollDiffXClosest)
    {
        charaCollDiffXFarthest ^= charaCollDiffXClosest;
        charaCollDiffXClosest  ^= charaCollDiffXFarthest;
        charaCollDiffXFarthest ^= charaCollDiffXClosest;
    }
    charaCollDiffXClosest  -= query->charaQuery.radius;
    charaCollDiffXFarthest += query->charaQuery.radius;

    // Getting Z axis differences.
    charaCollDiffZClosest  = query->charaQuery.positionFromZ - collData->positionZ;
    charaCollDiffZFarthest = query->charaQuery.positionToZ   - collData->positionZ;
    if (charaCollDiffZFarthest < charaCollDiffZClosest)
    {
        charaCollDiffZFarthest ^= charaCollDiffZClosest;
        charaCollDiffZClosest  ^= charaCollDiffZFarthest;
        charaCollDiffZFarthest ^= charaCollDiffZClosest;
    }
    charaCollDiffZClosest  -= query->charaQuery.radius;
    charaCollDiffZFarthest += query->charaQuery.radius;

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

    query->context.shape.cellX     = charaCollDiffXClosest / collData->cellSize;
    query->context.shape.cellZ     = charaCollDiffZClosest / collData->cellSize;
    query->context.shape.cellSpanX = ((charaCollDiffXFarthest / collData->cellSize) -
                                      query->context.shape.cellX) + 1;
    query->context.shape.cellSpanZ = ((charaCollDiffZFarthest / collData->cellSize) -
                                      query->context.shape.cellZ) + 1;

    return true;
}

void func_8006B1C8(s_CollisionQuery* query, s_IpdCollisionData* collData, s_IpdShapeAlloc* shapeAlloc) // 0x8006B1C8
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
            if (func_8006B318(query, collData, shapeIdx))
            {
                if (query->field_0_10)
                {
                    func_8006B6E8(query, shapeAlloc);
                }

                if (query->isCharaMoving || query->enablePush)
                {
                    if (query->point.shapeAttribs.polygon.surfaceIdx1 == UCHAR_MAX)
                    {
                        func_8006B9C8(query);
                    }

                    if (query->point.shapeAttribs.polygon.surfaceIdx0 == UCHAR_MAX)
                    {
                        func_8006B8F8(&query->point);
                        func_8006B9C8(query);
                    }
                }
            }
        }
        // Handle cylinder.
        else if (func_8006C3D4(query, collData, shapeIdx))
        {
            func_8006C45C(query);
        }
    }
}

bool func_8006B318(s_CollisionQuery* query, const s_IpdCollisionData* collData, s32 shapeIdx) // 0x8006B318
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
    if (CHECK_FLAG_IDX(query->triggerFlags, (polygon->triggerFlagIdxHigh << 2) | polygon->triggerFlagIdxLow, false))
    {
        return false;
    }

    query->point.ipdCollisionData = collData;
    query->point.shapeIdx         = shapeIdx;

    query->point.offset.vx = polygon->offsetX;
    query->point.offset.vy = polygon->offsetY;
    query->point.offset.vz = polygon->offsetZ;

    query->point.shapeAttribs.polygon.polygonSplitVertex0 = collData->polygonSplitVertices[polygon->splitVertexIdx1];
    query->point.shapeAttribs.polygon.polygonSplitVertex1 = collData->polygonSplitVertices[polygon->splitVertexIdx0];

    query->point.shapeAttribs.polygon.surfaceIdx0 = polygon->surfaceIdx0;
    if (query->point.shapeAttribs.polygon.surfaceIdx0 != UCHAR_MAX)
    {
        curSurface                                              = &collData->polygonSurfaces[query->point.shapeAttribs.polygon.surfaceIdx0];
        query->point.shapeAttribs.polygon.surfaceCacheIdx0      = curSurface->surfaceCacheIdx;
        query->point.shapeAttribs.polygon.disableSurfaceHeight0 = curSurface->disableHeight;

        if (query->charaQuery.blockInvalidFloor &&
            (curSurface->disableHeight == true || curSurface->floorType == FloorType_None))
        {
            query->point.shapeAttribs.polygon.polygonSplitVertex0.vy += Q8(DEFAULT_CEILING_HEIGHT);
            query->point.shapeAttribs.polygon.polygonSplitVertex1.vy += Q8(DEFAULT_CEILING_HEIGHT);
        }
    }

    query->point.shapeAttribs.polygon.surfaceIdx1 = polygon->surfaceIdx1;
    if (query->point.shapeAttribs.polygon.surfaceIdx1 != UCHAR_MAX)
    {
        curSurface                                              = &collData->polygonSurfaces[query->point.shapeAttribs.polygon.surfaceIdx1];
        query->point.shapeAttribs.polygon.surfaceCacheIdx1      = curSurface->surfaceCacheIdx;
        query->point.shapeAttribs.polygon.disableSurfaceHeight1 = curSurface->disableHeight;

        if (query->charaQuery.blockInvalidFloor &&
            (curSurface->disableHeight == true || curSurface->floorType == FloorType_None))
        {
            query->point.shapeAttribs.polygon.polygonSplitVertex0.vy = Q8(DEFAULT_CEILING_HEIGHT);
            query->point.shapeAttribs.polygon.polygonSplitVertex1.vy = Q8(DEFAULT_CEILING_HEIGHT);
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
    
    packedXz = PACKED_XZ16(query->charaQuery.moveOffset.vx, query->charaQuery.moveOffset.vz);
    gte_ldvxy0(packedXz);
    gte_ldvz0();
    gte_rtv0();
    gte_stMAC12(&query->point.charaDeltas.charaMoveOffset);
    /* Will - 1: ^ These values are normally set to 0 when standing still and are only modified when moving.
    
       Will - 2: I think this a distance, the value when set isn't too big and also I think `charaQuery.moveOffset` is the
       character movement offset, like the equivalent of doing `chunkOffsetFrom.offset.vx - chunkOffsetTo.offset.vx`
       that may explain why it's 0 when standing still.
    */
    
    packedXz = PACKED_XZ16(query->chunkOffsetFrom.offset.vx - query->point.shapeAttribs.polygon.polygonSplitVertex0.vx,
                           query->chunkOffsetFrom.offset.vz - query->point.shapeAttribs.polygon.polygonSplitVertex0.vz);
    gte_ldvxy0(packedXz);
    gte_ldvz0();
    gte_rtv0();
    gte_stMAC12(&query->point.charaDeltas.charaVertDiff);

    charaVertDiffX                                = query->point.charaDeltas.charaVertDiff.vx;
    charaVertDiffZ                                = query->point.charaDeltas.charaVertDiff.vz;
    query->point.charaDeltas.charaMoveVertDiff.vx = query->point.charaDeltas.charaMoveOffset.vx + charaVertDiffX;
    query->point.charaDeltas.charaMoveVertDiff.vz = query->point.charaDeltas.charaMoveOffset.vz + charaVertDiffZ;

    if (charaVertDiffX < Q8(0.0f))
    {
        query->point.charaDeltas.subChunkTransDir   = SubChunkTransitionDirection_X;
        query->point.charaDeltas.radiusCollDiffDist = Math_Vector2MagCalc(charaVertDiffX, charaVertDiffZ);
        query->point.charaDeltas.collDiffDist       = -charaVertDiffX;
    }
    else if (query->point.offset.vz < charaVertDiffX)
    {
        query->point.charaDeltas.subChunkTransDir   = SubChunkTransitionDirection_X;
        query->point.charaDeltas.radiusCollDiffDist = Math_Vector2MagCalc(charaVertDiffX - query->point.offset.vz, charaVertDiffZ);
        query->point.charaDeltas.collDiffDist       = charaVertDiffX - query->point.offset.vz;
    }
    else
    {
        if (charaVertDiffZ < Q8(0.0f))
        {
            query->point.charaDeltas.radiusCollDiffDist = -charaVertDiffZ;
        }
        else
        {
            query->point.charaDeltas.radiusCollDiffDist = charaVertDiffZ;
        }

        query->point.charaDeltas.subChunkTransDir = SubChunkTransitionDirection_Z;
    }

    return true;
}

void func_8006B6E8(s_CollisionQuery* query, s_IpdShapeAlloc* shapeAlloc) // 0x8006B6E8
{
    s32                       surfaceCacheIdx;
    s32                       surfaceIdx0;
    s32                       surfaceIdx1;
    bool                      disableSurfaceHeight0;
    bool                      disableSurfaceHeight1;
    s_CollisionCachedSurface* cachedSurface;

    surfaceIdx0           = query->point.shapeAttribs.polygon.surfaceIdx0;
    surfaceIdx1           = query->point.shapeAttribs.polygon.surfaceIdx1;
    disableSurfaceHeight0 = query->point.shapeAttribs.polygon.disableSurfaceHeight0;
    disableSurfaceHeight1 = query->point.shapeAttribs.polygon.disableSurfaceHeight1;

    if (surfaceIdx0 == UCHAR_MAX)
    {
        if (surfaceIdx1 == UCHAR_MAX)
        {
            return;
        }

        surfaceCacheIdx = query->point.shapeAttribs.polygon.surfaceCacheIdx1;
    }
    else
    {
        surfaceCacheIdx = query->point.shapeAttribs.polygon.surfaceCacheIdx0;
    }

    /* Will - 1: checks if the value at whatever the struct is `temp_s0`
       pointing have defined values. It first checks if `s_CollisionCachedSurface::field_0`
       is 0 or not, need to mention that this value is defined by default as `-1`
       in `Collision_CellInit` so any other value means that the values where defined
       already, but if `s_CollisionCachedSurface::field_0` is not zero then it checks
       if the value stored in `query->point.charaDeltas.field_8` is
       defined with any value other than 0, if not then it comes back here and continues
       if it is then it proceed to do some other checks that will return true.
       
       Likely if any of those cases fails the check then it will redefine whatever
       is supposed to be stored there.
    */
    cachedSurface = &query->context.shape.surfaceCache[surfaceCacheIdx];
    if (!func_8006B7E0(cachedSurface, &query->point.charaDeltas))
    {
        return;
    }

    cachedSurface->subChunkTransDir   = query->point.charaDeltas.subChunkTransDir;
    cachedSurface->radiusCollDiffDist = query->point.charaDeltas.radiusCollDiffDist;
    cachedSurface->collDiffDist       = query->point.charaDeltas.collDiffDist;

    if (query->point.charaDeltas.charaVertDiff.vz >= Q8(0.0f))
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
                case SubChunkTransitionDirection_Z:
                    if (charaDeltas->radiusCollDiffDist < cachedSurface->radiusCollDiffDist)
                    {
                        return true;
                    }
                    break;

                case SubChunkTransitionDirection_X:
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
    cellPoint->charaDeltas.charaMoveOffset.vx              = -cellPoint->charaDeltas.charaMoveOffset.vx;

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

void func_8006B9C8(s_CollisionQuery* query) // 0x8006B9C8
{
    q23_8 charaRadius;

    if (query->point.shapeAttribs.polygon.surfaceIdx1 == UCHAR_MAX &&
        query->point.charaDeltas.charaVertDiff.vz < Q8(0.0f) &&
        (query->charaQuery.bottom >= query->point.shapeAttribs.polygon.polygonSplitVertex0.vy ||
         query->charaQuery.bottom >= query->point.shapeAttribs.polygon.polygonSplitVertex1.vy))
    {
        if (query->enablePush && query->point.charaDeltas.radiusCollDiffDist < query->charaQuery.radius)
        {
            func_8006BB50(query, CollisionPushType_Hard);
            return;
        }

        charaRadius = query->charaQuery.radius;
        if (query->enablePush && query->point.charaDeltas.radiusCollDiffDist < (charaRadius + Q8(PROXIMITY_BUFFER)))
        {
            func_8006BB50(query, CollisionPushType_Soft);
        }

        if (query->isCharaMoving)
        {
            if (!query->obstacleContext.hardPushConstraint.shapeCount &&
                (-charaRadius < query->point.charaDeltas.charaVertDiff.vz ||
                 -charaRadius < query->point.charaDeltas.charaMoveVertDiff.vz) &&
                (-charaRadius < query->point.charaDeltas.charaVertDiff.vx ||
                 -charaRadius < query->point.charaDeltas.charaMoveVertDiff.vx) &&
                (query->point.charaDeltas.charaVertDiff.vx     < (charaRadius + query->point.offset.vz) ||
                 query->point.charaDeltas.charaMoveVertDiff.vx < (charaRadius + query->point.offset.vz)))
            {
                func_8006BE40(query);
            }
        }
    }
}

void func_8006BB50(s_CollisionQuery* query, s32 pushType) // 0x8006BB50
{
    q23_8 collOffsetX;
    q23_8 collOffsetZ;
    q7_8  intersectDepth;

    // Return early if character is above floor.
    if (Collision_CharaDistanceToFloorGet(query) < Q8(0.0f))
    {
        return;
    }

    if (query->point.charaDeltas.charaVertDiff.vx < Q8(0.0f))
    {
        collOffsetX = query->chunkOffsetFrom.offset.vx - query->point.shapeAttribs.polygon.polygonSplitVertex0.vx;
        collOffsetZ = query->chunkOffsetFrom.offset.vz - query->point.shapeAttribs.polygon.polygonSplitVertex0.vz;
    }
    else if (query->point.offset.vz < query->point.charaDeltas.charaVertDiff.vx)
    {
        collOffsetX = query->chunkOffsetFrom.offset.vx - query->point.shapeAttribs.polygon.polygonSplitVertex1.vx;
        collOffsetZ = query->chunkOffsetFrom.offset.vz - query->point.shapeAttribs.polygon.polygonSplitVertex1.vz;
    }
    else
    {
        collOffsetX =  query->point.offset.vy;
        collOffsetZ = -query->point.offset.vx;
    }

    intersectDepth = query->charaQuery.radius - query->point.charaDeltas.radiusCollDiffDist;
    Collision_ObstacleIntersectDepthSet(&query->obstacleContext,
                                        &query->point.ipdCollisionData->shapePassStampBuffer[query->point.shapeIdx],
                                        pushType, collOffsetX, collOffsetZ, intersectDepth);
}

q23_8 Collision_CharaDistanceToFloorGet(const s_CollisionQuery* query) // 0x8006BC34
{
    q7_8 relZ;
    q7_8 height0;
    q7_8 height1;
    q7_8 relX;
    q7_8 baseHeight;
    q7_8 floorHeight;

    relX = query->point.charaDeltas.charaVertDiff.vx;
    if (relX < Q8(0.0f))
    {
        floorHeight = query->point.shapeAttribs.polygon.polygonSplitVertex0.vy;
    }
    else
    {
        relZ = query->point.offset.vz;
        if (relZ < relX)
        {
            floorHeight = query->point.shapeAttribs.polygon.polygonSplitVertex1.vy;
        }
        else
        {
            // Get polygon split vertex heights.
            height0    = query->point.shapeAttribs.polygon.polygonSplitVertex0.vy;
            height1    = query->point.shapeAttribs.polygon.polygonSplitVertex1.vy;
            baseHeight = query->point.shapeAttribs.polygon.polygonSplitVertex0.vy;

            // Set flat height.
            if (height0 == height1)
            {
                floorHeight = baseHeight;
            }
            // Compute height on slope.
            else
            {
                floorHeight = baseHeight + ((q23_8)((height1 - height0) * relX) / relZ);
            }
        }
    }

    return query->charaQuery.bottom - floorHeight;
}

void Collision_ObstacleIntersectDepthSet(s_CollisionObstacleContext* obstacleContext, u8* passStamp,
                                         u32 pushType, q7_8 collOffsetX, q7_8 collOffsetZ, q7_8 intersectDepth) // 0x8006BCC4
{
    q3_12 angleMin;
    q3_12 angleMax;

    angleMin = Q12_FRACT(ratan2(collOffsetZ, collOffsetX) - Q12_ANGLE(89.0f));
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

            if (obstacleContext->softPushConstraint.shapeCount <= ARRAY_SIZE(obstacleContext->shapePassStamps))
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

void func_8006BE40(s_CollisionQuery* query) // 0x8006BE40
{
    q23_8  negRadius;
    q23_8  unkDist;
    q19_12 distAlpha;
    u32    unkSwitchType;
    q19_12 dist;

    distAlpha = Q12(0.0f);
    negRadius = -(query->charaQuery.radius);
    unkDist   = Q8(0.0f);

    if (query->point.charaDeltas.charaVertDiff.vz >= negRadius)
    {
        if (query->point.charaDeltas.charaVertDiff.vx >= Q8(0.0f))
        {
            if (query->point.offset.vz >= query->point.charaDeltas.charaVertDiff.vx)
            {
                unkDist = query->point.charaDeltas.charaVertDiff.vx;
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
        if (query->point.charaDeltas.charaMoveOffset.vz == 0)
        {
            if (query->point.charaDeltas.charaVertDiff.vx > 0 &&
                query->point.charaDeltas.charaVertDiff.vx < query->point.offset.vz)
            {
                distAlpha = Q12(0.0f);
                unkDist = query->point.charaDeltas.charaVertDiff.vx;
            }
        }
        else
        {
            distAlpha = FP_TO(negRadius - query->point.charaDeltas.charaVertDiff.vz, Q12_SHIFT) /
                        query->point.charaDeltas.charaMoveOffset.vz;
            dist      = query->point.charaDeltas.charaMoveOffset.vx * distAlpha;
            dist      = FP_FROM(dist, Q12_SHIFT);
            unkDist   = dist + query->point.charaDeltas.charaVertDiff.vx;
        }

        if (unkDist < Q8(0.0f))
        {
            unkSwitchType = 0;
        }
        else if (query->point.offset.vz < unkDist)
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
            func_8006BF88(query, &query->point.shapeAttribs.polygon.polygonSplitVertex0);
            break;

        case 1: // Vertex 1.
            func_8006BF88(query, &query->point.shapeAttribs.polygon.polygonSplitVertex1);
            break;

        case 2: // Non-split?
            func_8006C0C8(query, distAlpha, unkDist);
            break;
    }
}

void func_8006BF88(s_CollisionQuery* query, const SVECTOR3* splitVert) // 0x8006BF88
{
    q3_12 hitAlpha;
    s32   temp2;
    s32   temp3;

    // Compute hit alpha.
    hitAlpha = Collision_HitAlphaGet(PACKED_MOVE_DIRECTION(query->charaQuery.moveDirection),
                                     query->charaQuery.moveDistance,
                                     splitVert->vx - query->chunkOffsetFrom.offset.vx,
                                     splitVert->vz - query->chunkOffsetFrom.offset.vz,
                                     query->charaQuery.radius);

    if (hitAlpha != NO_VALUE && func_8006C1B8(CollisionContactType_2, hitAlpha, query) &&
        query->charaQuery.bottom > splitVert->vy)
    {
        query->hitAlpha    = hitAlpha;
        query->contactType = CollisionContactType_2;
        temp2              = query->chunkOffsetFrom.offset.vx + Q12_MULT(query->charaQuery.moveOffset.vx, hitAlpha);
        query->hitDistance = Q12_TO_Q4(query->charaQuery.moveDistance * hitAlpha);
        query->passStamp   = &query->point.ipdCollisionData->shapePassStampBuffer[query->point.shapeIdx];
        query->hitOffsetX  = temp2 - splitVert->vx;
        temp3              = query->chunkOffsetFrom.offset.vz + Q12_MULT(query->charaQuery.moveOffset.vz, hitAlpha);
        query->hitOffsetZ  = temp3 - splitVert->vz;
    }
}

void func_8006C0C8(s_CollisionQuery* query, q3_12 hitAlpha, q7_8 arg2) // 0x8006C0C8
{
    q23_8 temp;

    if (!func_8006C1B8(CollisionContactType_1, hitAlpha, query) || query->point.charaDeltas.charaMoveOffset.vz < 0)
    {
        return;
    }

    temp = ((query->point.shapeAttribs.polygon.polygonSplitVertex1.vy -
             query->point.shapeAttribs.polygon.polygonSplitVertex0.vy) * arg2) / query->point.offset.vz;
    if ((temp + query->point.shapeAttribs.polygon.polygonSplitVertex0.vy) < query->charaQuery.bottom)
    {
        query->passStamp   = &query->point.ipdCollisionData->shapePassStampBuffer[query->point.shapeIdx];
        query->contactType = CollisionContactType_1;
        query->hitAlpha    = hitAlpha;
        query->hitDistance = Q12_TO_Q4(query->charaQuery.moveDistance * hitAlpha);
        query->hitOffsetX  = query->point.offset.vy;
        query->hitOffsetZ  = -query->point.offset.vx;
    }
}

bool func_8006C1B8(u32 contactType, q3_12 hitAlpha, s_CollisionQuery* query) // 0x8006C1B8
{
    #define HIT_BUFFER Q4(6.0f)

    q27_4 hitDist;

    hitDist = Q12_TO_Q4(query->charaQuery.moveDistance * hitAlpha);
    switch (contactType)
    {
        default:
        case CollisionContactType_None:
            return false;

        case CollisionContactType_2:
            if (query->contactType != CollisionContactType_1)
            {
                if (query->contactType == CollisionContactType_None)
                {
                   return true;
                }

                break;
            }

            hitDist += HIT_BUFFER;
            break;

        case CollisionContactType_1:
            if (query->contactType != contactType)
            {
                if (query->contactType != CollisionContactType_None)
                {
                    if (query->contactType == CollisionContactType_2)
                    {
                        hitDist -= HIT_BUFFER;
                    }

                    break;
                }

                return true;
            }
            break;
    }

    return hitDist < query->hitDistance;

    #undef HIT_BUFFER
}

q3_12 Collision_HitAlphaGet(s32 packedDir, q3_12 moveDist, q3_12 offsetX, q7_8 offsetZ, q7_8 radius) // 0x8006C248
{
    DVECTOR sp10; // Q19.12
    s16     temp_v0;
    q7_8    dist;
    q3_12   alpha;

    gte_ldR11R12(packedDir);
    gte_ldR13R21(packedDir);
    gte_ldvxy0(PACKED_XZ16(offsetX, offsetZ));
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

    // No intersection.
    if (dist >= radius)
    {
        return NO_VALUE;
    }

    alpha = Q12_DIV(sp10.vx - SquareRoot0(SQUARE(radius) - SQUARE(sp10.vy)), moveDist);
    alpha = CLAMP(alpha, Q12(0.0f), Q12(1.0f));
    return alpha;
}

bool func_8006C3D4(s_CollisionQuery* query, s_IpdCollisionData* collData, s32 shapeIdx) // 0x8006C3D4
{
    s_IpdCylinder* cylinder;

    query->point.ipdCollisionData = collData;
    query->point.shapeIdx         = shapeIdx;
    cylinder                      = &collData->cylinders[shapeIdx - collData->polygonCount];

    if (CHECK_FLAG_IDX(query->triggerFlags, cylinder->triggerFlagIdx, false))
    {
        return false;
    }

    query->point.disableHeight                = cylinder->disableHeight;
    query->point.offset.vx                    = cylinder->offset.vx;
    query->point.offset.vy                    = cylinder->offset.vy;
    query->point.offset.vz                    = cylinder->offset.vz;
    query->point.shapeAttribs.cylinder.radius = cylinder->radius;
    return true;
}

void func_8006C45C(s_CollisionQuery* query) // 0x8006C45C
{
    q7_8  distToCylinderMax;
    q7_8  distToCylinder;
    q3_12 hitAlpha;
    q23_8 distToCylinderBound;
    q23_8 distToCylinderX;
    q23_8 distToCylinderZ;
    s32   temp;
    s32   temp2;

    distToCylinderMax   = query->charaQuery.radius + query->point.shapeAttribs.cylinder.radius;
    distToCylinderBound = distToCylinderMax + Q8(PROXIMITY_BUFFER);

    if (query->chunkOffsetFrom.offset.vx < (query->point.offset.vx - distToCylinderBound) &&
        query->chunkOffsetTo.offset.vx   < (query->point.offset.vx - distToCylinderBound))
    {
        return;
    }

    if ((query->point.offset.vx + distToCylinderBound) < query->chunkOffsetFrom.offset.vx &&
        (query->point.offset.vx + distToCylinderBound) < query->chunkOffsetTo.offset.vx)
    {
        return;
    }

    if (query->chunkOffsetFrom.offset.vz < (query->point.offset.vz - distToCylinderBound) &&
        query->chunkOffsetTo.offset.vz   < (query->point.offset.vz - distToCylinderBound))
    {
        return;
    }

    if ((query->point.offset.vz + distToCylinderBound) < query->chunkOffsetFrom.offset.vz &&
        (query->point.offset.vz + distToCylinderBound) < query->chunkOffsetTo.offset.vz)
    {
        return;
    }

    distToCylinderX = query->chunkOffsetFrom.offset.vx - query->point.offset.vx;
    distToCylinderZ = query->chunkOffsetFrom.offset.vz - query->point.offset.vz;
    distToCylinder  = Math_Vector2MagCalc(distToCylinderX, distToCylinderZ);
    if (distToCylinder < query->point.shapeAttribs.cylinder.radius && query->point.disableHeight != true &&
        (query->shapeIdx == UCHAR_MAX || query->point.offset.vy < query->floorHeight))
    {
        query->shapeIdx    = query->point.shapeIdx;
        query->floorHeight = query->point.offset.vy;
    }

    if ((!query->isCharaMoving && !query->enablePush) || distToCylinder < query->point.shapeAttribs.cylinder.radius)
    {
        return;
    }

    // Set hard intersection if below proximity buffer zone.
    if (distToCylinder < distToCylinderMax && query->enablePush)
    {
        Collision_CylinderObstacleIntersectDepthSet(query, CollisionPushType_Hard, distToCylinder);
        return;
    }

    // Set soft intersection if within proximity buffer zone.
    if (distToCylinder < (distToCylinderMax + Q8(PROXIMITY_BUFFER)) && query->enablePush)
    {
        Collision_CylinderObstacleIntersectDepthSet(query, CollisionPushType_Soft, distToCylinder);
    }

    // Return early if character is stopped or hard shapes have been collided with.
    if (!query->isCharaMoving || query->obstacleContext.hardPushConstraint.shapeCount != 0)
    {
        return;
    }

    // Compute hit alpha.
    hitAlpha = Collision_HitAlphaGet(PACKED_MOVE_DIRECTION(query->charaQuery.moveDirection),
                                     query->charaQuery.moveDistance,
                                     query->point.offset.vx - query->chunkOffsetFrom.offset.vx,
                                     query->point.offset.vz - query->chunkOffsetFrom.offset.vz,
                                     distToCylinderMax);
    if (hitAlpha == NO_VALUE)
    {
        return;
    }

    // Clamp hit alpha.
    if (hitAlpha < Q12(0.0f))
    {
        hitAlpha = Q12(0.0f);
    }

    if (func_8006C1B8(CollisionContactType_1, hitAlpha, query) && query->charaQuery.bottom >= query->point.offset.vy)
    {
        query->hitAlpha    = hitAlpha;
        query->contactType = CollisionContactType_1;
        temp               = query->chunkOffsetFrom.offset.vx + Q12_MULT(query->charaQuery.moveOffset.vx, hitAlpha);
        query->hitDistance = Q12_TO_Q4(query->charaQuery.moveDistance * hitAlpha);
        query->passStamp   = &query->point.ipdCollisionData->shapePassStampBuffer[query->point.shapeIdx];
        query->hitOffsetX  = temp - query->point.offset.vx;
        temp2              = query->chunkOffsetFrom.offset.vz + Q12_MULT(query->charaQuery.moveOffset.vz, hitAlpha);
        query->hitOffsetZ  = temp2 - query->point.offset.vz;
    }
}

void Collision_CylinderObstacleIntersectDepthSet(s_CollisionQuery* query, s32 pushType, q23_8 distToCylinder) // 0x8006C794
{
    if (query->charaQuery.bottom >= (query->point.offset.vy + (distToCylinder - query->point.shapeAttribs.cylinder.radius)))
    {
        Collision_ObstacleIntersectDepthSet(&query->obstacleContext,
                                            &query->point.ipdCollisionData->shapePassStampBuffer[query->point.shapeIdx],
                                            pushType,
                                            query->chunkOffsetFrom.offset.vx - query->point.offset.vx,
                                            query->chunkOffsetFrom.offset.vz - query->point.offset.vz,
                                            (query->charaQuery.radius + query->point.shapeAttribs.cylinder.radius) - distToCylinder);
    }
}

void func_8006C838(s_CollisionQuery* query, s_IpdCollisionData* collData) // 0x8006C838
{
    q19_12                    slopedFloorHeight;
    s_CollisionCachedSurface* curCachedSurface;
    s_IpdSurface*             curSurface;
    s_IpdCylinder*            cylinder;

    if (query->context.shape.shapeAlloc == NULL)
    {
        return;
    }

    if (query->shapeIdx != UCHAR_MAX && query->floorHeight < query->slopedFloorHeight)
    {
        cylinder                 = &collData->cylinders[query->shapeIdx - collData->polygonCount];
        query->slopedFloorHeight = query->floorHeight;
        query->positionX         = query->chunkOffsetFrom.offset.vx + collData->positionX;
        query->positionZ         = query->chunkOffsetFrom.offset.vz + collData->positionZ;
        query->slopeGradientX    = Q8(0.0f);
        query->slopeGradientZ    = Q8(0.0f);
        query->disableHeight     = cylinder->disableHeight;
        query->floorType         = cylinder->floorType;
    }

    // Run through cached surfaces.
    for (curCachedSurface = &query->context.shape.surfaceCache[0];
         curCachedSurface < &query->context.shape.surfaceCache[4];
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
                                              query->chunkOffsetFrom.offset.vx - curSurface->offset.vx);
            }
            if (curSurface->slopeGradientZ != Q8(0.0f))
            {
                slopedFloorHeight += Q12_MULT(curSurface->slopeGradientZ,
                                              query->chunkOffsetFrom.offset.vz - curSurface->offset.vz);
            }

            if (slopedFloorHeight < query->slopedFloorHeight)
            {
                query->slopedFloorHeight = slopedFloorHeight;
                query->positionX         = query->chunkOffsetFrom.offset.vx + collData->positionX;
                query->positionZ         = query->chunkOffsetFrom.offset.vz + collData->positionZ;
                query->slopeGradientX    = curSurface->slopeGradientX;
                query->slopeGradientZ    = curSurface->slopeGradientZ;
                query->disableHeight     = curSurface->disableHeight;
                query->floorType         = curSurface->floorType;
            }
        }
    }
}

void func_8006CA18(s_CollisionQuery* query, s_IpdCollisionData* collData, s_IpdShapeAlloc* shapeAlloc) // 0x8006CA18
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
        if (CHECK_FLAG_IDX(query->triggerFlags, curSurface->triggerFlagIdx, false) ||
            curSurface->disableHeight == true)
        {
            continue;
        }

        // Compute sloped floor height.
        slopedFloorHeight = curSurface->offset.vy;
        if (curSurface->slopeGradientX != Q8(0.0f))
        {
            slopedFloorHeight += Q12_MULT(curSurface->slopeGradientX, query->chunkOffsetFrom.offset.vx - curSurface->offset.vx);
        }
        if (curSurface->slopeGradientZ != Q8(0.0f))
        {
            slopedFloorHeight += Q12_MULT(curSurface->slopeGradientZ, query->chunkOffsetFrom.offset.vz - curSurface->offset.vz);
        }

        if (slopedFloorHeight < query->slopedFloorHeight)
        {
            query->slopedFloorHeight = slopedFloorHeight;
            query->positionX  = query->chunkOffsetFrom.offset.vx + collData->positionX;
            query->positionZ  = query->chunkOffsetFrom.offset.vz + collData->positionZ;
            query->slopeGradientX    = curSurface->slopeGradientX;
            query->slopeGradientZ    = curSurface->slopeGradientZ;
            query->disableHeight     = curSurface->disableHeight;
            query->floorType         = curSurface->floorType;
        }
    }
}

q3_12 Collision_OffsetAlphaGet(s_CollisionQuery* query) // 0x8006CB90
{
    q23_8 floorHeight;

    if (query->slopedFloorHeight == Q8(DEFAULT_FLOOR_HEIGHT))
    {
        return Q12(1.0f);
    }

    floorHeight = Ipd_FloorHeightGet(query->charaQuery.positionToX, query->charaQuery.positionToZ, query);
    if ((query->charaQuery.bottom + query->charaQuery.moveOffset.vy) < floorHeight ||
        floorHeight == query->slopedFloorHeight)
    {
        return Q12(1.0f);
    }

    return Q12_DIV(query->charaQuery.moveDistance, Math_Vector2MagCalc(query->charaQuery.moveDistance,
                                                                   floorHeight - query->charaQuery.bottom));
}

q23_8 Ipd_FloorHeightGet(q23_8 posX, q23_8 posZ, const s_CollisionQuery* query) // 0x8006CC44
{
    if (query->floorType != FloorType_None)
    {
        return Q12_MULT(query->slopeGradientX, posX - query->positionX) +
               Q12_MULT(query->slopeGradientZ, posZ - query->positionZ) +
               query->slopedFloorHeight;
    }

    return Q8(8.0f);
}

void func_8006CC9C(s_CollisionQuery* query) // 0x8006CC9C
{
    q23_8  collOffsetX;
    q23_8  collOffsetZ;
    q19_12 hitAlpha;
    q23_8  collDist;
    q23_8  temp;
    q23_8  temp2;
    q23_8  intersectDepth;
    q23_8  temp4;
    q23_8  targetCharaBottom;

    if (query->context.chara.state < CharaCollisionState_2 ||
        *query->context.chara.passStamp != 0)
    {
        return;
    }

    if (query->charaQuery.positionFromX > (query->chunkOffsetFrom.vx + (query->context.chara.radius + query->charaQuery.moveDistance)) ||
        query->charaQuery.positionToX   < (query->chunkOffsetFrom.vx - (query->context.chara.radius + query->charaQuery.moveDistance)))
    {
        return;
    }

    if (query->charaQuery.positionFromZ > (query->chunkOffsetTo.vz + (query->context.chara.radius + query->charaQuery.moveDistance)) ||
        query->charaQuery.positionToZ   < (query->chunkOffsetTo.vz - (query->context.chara.radius + query->charaQuery.moveDistance)) ||
        query->charaQuery.top > query->context.chara.top)
    {
        return;
    }

    collOffsetX = (query->charaQuery.positionFromX - query->chunkOffsetFrom.vx);

    if (query->charaQuery.bottom < query->context.chara.bottom != Q8(0.0f))
    {
        return;
    }

    collOffsetZ = query->charaQuery.positionFromZ - query->chunkOffsetTo.vz;
    collDist    = Math_Vector2MagCalc(collOffsetX, collOffsetZ);

    // Compute hit alpha.
    hitAlpha = Collision_HitAlphaGet(PACKED_MOVE_DIRECTION(query->charaQuery.moveDirection),
                                     query->charaQuery.moveDistance,
                                     query->chunkOffsetFrom.vx - query->charaQuery.positionFromX,
                                     query->chunkOffsetTo.vz   - query->charaQuery.positionFromZ,
                                     query->context.chara.radius);
    if (hitAlpha == NO_VALUE)
    {
        return;
    }

    if (hitAlpha == Q12(0.0f))
    {
        if (query->enablePush)
        {
            intersectDepth = query->context.chara.radius - collDist;
            Collision_ObstacleIntersectDepthSet(&query->obstacleContext, query->context.chara.passStamp,
                                                CollisionPushType_Chara, collOffsetX, collOffsetZ, intersectDepth);
        }
    }
    else if (query->isCharaMoving && query->obstacleContext.hardPushConstraint.shapeCount == 0 &&
             func_8006C1B8(CollisionContactType_1, hitAlpha, query))
    {
        temp2 = (query->charaQuery.positionFromZ - query->chunkOffsetTo.vz);
        targetCharaBottom = Q12_MULT(hitAlpha, query->charaQuery.moveOffset.vz);

        query->passStamp   = query->context.chara.passStamp;
        query->hitAlpha    = hitAlpha;
        query->contactType = CollisionContactType_1;

        temp  = (query->charaQuery.positionFromX - query->chunkOffsetFrom.vx);
        temp4 = Q12_MULT(hitAlpha, query->charaQuery.moveOffset.vx);

        query->hitDistance = Q12_TO_Q4(query->charaQuery.moveDistance * hitAlpha);
        query->hitOffsetX  = temp + temp4;
        query->hitOffsetZ  = temp2 + targetCharaBottom;
    }
}

void func_8006CF18(s_CollisionQuery* query, s_CharaCollisionRecord* charaRecords, s32 charaRecordCount) // 0x8006CF18
{
    q23_8                   radius;
    s_CharaCollisionRecord* curCharaRecord;

    // Run through character records.
    for (curCharaRecord = charaRecords; curCharaRecord < &charaRecords[charaRecordCount]; curCharaRecord++)
    {
        radius = Q12_TO_Q8(curCharaRecord->radius) + query->charaQuery.radius;
        if (curCharaRecord->state < (u32)query->charaQuery.state)
        {
            radius -= Q8(CHARA_RADIUS_BUFFER);
        }

        query->chunkOffsetFrom.vx = Q12_TO_Q8(curCharaRecord->position.vx);
        query->chunkOffsetTo.vz   = Q12_TO_Q8(curCharaRecord->position.vz);

        query->context.chara.bottom    = Q12_TO_Q8(curCharaRecord->topExtent    + curCharaRecord->position.vy);
        query->context.chara.top       = Q12_TO_Q8(curCharaRecord->bottomExtent + curCharaRecord->position.vy);
        query->context.chara.radius    = radius;
        query->context.chara.state     = curCharaRecord->state;
        query->context.chara.passStamp = &curCharaRecord->passStamp;

        if (!query->offsetX)
        {
            curCharaRecord->passStamp = 0;
        }

        func_8006CC9C(query);
    }
}

void func_8006D01C(VECTOR3* extraOffset, VECTOR3* offset, q3_12 offsetAlpha, s_CollisionQuery* query) // 0x8006D01C
{
    #define LARGE_OFFSET_BOUND  Q12(1.0f / 3.0f)
    #define EXTRA_OFFSET_BUFFER Q12(1.0f / 256.0f)

    VECTOR3 adjOffset;
    q23_8   hitOffsetX;
    q23_8   hitOffsetZ;
    s32     offsetDistSqr;
    s32     temp_v0;

    adjOffset.vx = Q12_MULT(offset->vx, offsetAlpha);
    adjOffset.vz = Q12_MULT(offset->vz, offsetAlpha);

    if (query->obstacleContext.hardPushConstraint.shapeCount  != 0||
        query->obstacleContext.charaPushConstraint.shapeCount != 0)
    {
        extraOffset->vx = Q12(0.0f);
        extraOffset->vz = Q12(0.0f);
        *offset         = adjOffset;
        func_8006D2B4(offset, &query->obstacleContext);
        return;
    }

    if (query->contactType == CollisionContactType_None)
    {
        *extraOffset = adjOffset;
        offset->vz   = Q12(0.0f);
        offset->vx   = Q12(0.0f);
        return;
    }

    if (offsetAlpha < query->hitAlpha)
    {
        query->contactType = CollisionContactType_None;
        *extraOffset       = adjOffset;
        offset->vz         = Q12(0.0f);
        offset->vx         = Q12(0.0f);
        return;
    }

    extraOffset->vx = Q12_MULT(offset->vx, query->hitAlpha);
    extraOffset->vz = Q12_MULT(offset->vz, query->hitAlpha);
    offset->vx      = adjOffset.vx - extraOffset->vx;
    offset->vz      = adjOffset.vz - extraOffset->vz;

    hitOffsetX    = query->hitOffsetX;
    hitOffsetZ    = query->hitOffsetZ;
    offsetDistSqr = SQUARE(hitOffsetX) + SQUARE(hitOffsetZ);
    if (offsetDistSqr < 256)
    {
        offsetDistSqr = SquareRoot0(offsetDistSqr * 256);
        hitOffsetX = (hitOffsetX << 16) / offsetDistSqr; // TODO: Fixed-point division? `Q12_DIV` wouldn't fit, why `<< 16`?
        hitOffsetZ = (hitOffsetZ << 16) / offsetDistSqr;
    }
    else
    {
        offsetDistSqr = SquareRoot0(offsetDistSqr);
        hitOffsetX = Q12_DIV(hitOffsetX, offsetDistSqr);
        hitOffsetZ = Q12_DIV(hitOffsetZ, offsetDistSqr);
    }

    // TODO: `Q12_SHIFT` doesn't seem correct, what's this doing?
    temp_v0  = FP_FROM((offset->vx * hitOffsetZ) + (offset->vz * -hitOffsetX), Q12_SHIFT);
    offset->vx = Q12_MULT(temp_v0, hitOffsetZ);
    offset->vz = Q12_MULT(temp_v0, -hitOffsetX);

    if (hitOffsetX > LARGE_OFFSET_BOUND)
    {
        extraOffset->vx += EXTRA_OFFSET_BUFFER;
    }
    else if (hitOffsetX < -LARGE_OFFSET_BOUND)
    {
        extraOffset->vx -= EXTRA_OFFSET_BUFFER;
    }

    if (hitOffsetZ > LARGE_OFFSET_BOUND)
    {
        extraOffset->vz += EXTRA_OFFSET_BUFFER;
    }
    else if (hitOffsetZ < -LARGE_OFFSET_BOUND)
    {
        extraOffset->vz -= EXTRA_OFFSET_BUFFER;
    }

    #undef LARGE_OFFSET_BOUND
    #undef EXTRA_OFFSET_BUFFER
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
    bool   noShapePush;
    q3_12  angle;
    q19_12 unkAngleMin;
    q7_8   dist;
    q7_8   intersectDepth;
    q3_12  angle2;
    q3_12  angle3;

    if (obstacleContext->charaPushConstraint.shapeCount != 0)
    {
        noShapePush = false;
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
            noShapePush = true;
        }

        if (noShapePush)
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

        dist = Q8_TO_Q12(MIN(obstacleContext->hardIntersectDepth + PUSH_BUFFER, Q8(HARD_SHAPE_INTERSECT_DEPTH_BUFFER)));
        func_8006D600(offset, angle, angleMin, angleMax, dist);
    }

    #undef PUSH_BUFFER
}

void func_8006D600(VECTOR3* pos, q19_12 angle, q19_12 angleMin, q19_12 angleMax, q19_12 hardIntersectDepth) // 0x8006D600
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

    if (hardIntersectDepth > Q12(HARD_SHAPE_INTERSECT_DEPTH_BUFFER))
    {
        hardIntersectDepth = Q12(HARD_SHAPE_INTERSECT_DEPTH_BUFFER);
    }

    x      = Q12_MULT(hardIntersectDepth, Math_Cos(normAngle));
    angle2 = normAngleMax;
    z      = Q12_MULT(hardIntersectDepth, Math_Sin(normAngle));
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
        if (DIV_FAST(angle2 + angle3, 2) < angle1)
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

void Collision_CharaRawMovementSet(s_CollisionQuery* query, const VECTOR3* extraOffset, const VECTOR3* moveOffset) // 0x8006D774
{
    SVECTOR locExtraOffset; // Q7.8
    SVECTOR locMoveOffset;  // Q7.8

    // Convert offsets to Q8.
    locExtraOffset.vx = Q12_TO_Q8(extraOffset->vx);
    locExtraOffset.vy = Q12_TO_Q8(extraOffset->vz);
    locMoveOffset.vx  = Q12_TO_Q8(moveOffset->vx);
    locMoveOffset.vy  = Q12_TO_Q8(moveOffset->vz);

    // Clear shape contact pushes.
    query->contactType                                    = CollisionContactType_None;
    query->obstacleContext.hardPushConstraint.shapeCount  = 0;
    query->obstacleContext.hardIntersectDepth             = Q8(0.0f);
    query->obstacleContext.softPushConstraint.shapeCount  = 0;
    query->obstacleContext.charaIntersectDepth            = Q8(0.0f);
    query->obstacleContext.charaPushConstraint.shapeCount = 0;

    // Set movement.
    Collision_CharaMovementSet(&query->charaQuery, &locExtraOffset, &locMoveOffset);
}

void Collision_CharaMovementSet(s_CharaCollisionQuery* query, const SVECTOR* extraOffset, const SVECTOR* moveOffset) // 0x8006D7EC
{
    q3_12  headingAngle;
    q19_12 moveDist;
    q7_8   moveOffsetY;

    // Set movement offset.
    query->moveOffset.vx = moveOffset->vx;
    moveOffsetY          = moveOffset->vy;
    query->moveOffset.vz = moveOffset->vy;

    // Compute movement distance.
    moveDist            = Math_Vector2MagCalc(query->moveOffset.vx, moveOffsetY);
    query->moveDistance = moveDist;

    // Compute movement direction.
    if (moveDist != Q12(0.0f))
    {
        query->moveDirection.vx = Q12_DIV(query->moveOffset.vx, moveDist);
        query->moveDirection.vz = Q12_DIV(query->moveOffset.vz, query->moveDistance);

        headingAngle            = ratan2(query->moveOffset.vz, query->moveOffset.vx);
        query->moveDirection.vx = Math_Cos(headingAngle);
        query->moveDirection.vz = Math_Sin(headingAngle);
    }
    else
    {
        query->moveDirection.vx = Q12(1.0f);
        query->moveDirection.vz = Q12(0.0f);
    }

    // Compute positions.
    query->positionFromX = query->positionFromX + extraOffset->vx;
    query->positionFromZ = query->positionFromZ + extraOffset->vy;
    query->positionToX   = query->positionFromX + query->moveOffset.vx;
    query->positionToZ   = query->positionFromZ + query->moveOffset.vz;
}
