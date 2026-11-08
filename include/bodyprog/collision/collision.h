#ifndef _BODYPROG_COLLISION_COLLISION_H
#define _BODYPROG_COLLISION_COLLISION_H

#include "bodyprog/chara/chara.h"
#include "bodyprog/collision/chara.h"
#include "bodyprog/collision/los.h"
#include "bodyprog/collision/ray.h"
#include "bodyprog/collision/trigger.h"
#include "bodyprog/formats/ipd.h"
#include "bodyprog/map/map.h"

#define DEFAULT_FLOOR_HEIGHT          30.0f
#define DEFAULT_CEILING_HEIGHT        -16.0f
#define INTERSECTION_BUFFER           0.1f
#define SOFT_OBSTACLE_SHAPE_COUNT_MAX 8
#define CACHED_SURFACE_COUNT_MAX      4

/** @brief Collision types. */
typedef enum _CollisionType
{
    CollisionType_None  = 0,
    CollisionType_Wall  = 1,
    CollisionType_Floor = 2 // TODO: Unsure.
} e_CollisionType;

// TODO: Unsure about what these actually are.
typedef enum _CollisionContactType
{
    CollisionContactType_None = 0,
    CollisionContactType_1    = 1,
    CollisionContactType_2    = 2
} s_CollisionContactType;

/** @brief Collision push types. */
typedef enum _CollisionPushType
{
    CollisionPushType_Hard  = 0, /** Hard inner shape proximity. */
    CollisionPushType_Soft  = 1, /** Soft outer shape proximity. */
    CollisionPushType_Chara = 2  /** Character-to-character cylinder. */
} s_CollisionPushType;

// TODO: "Subchunk" term may be misused here, need to check.
typedef enum _SubChunkTransitionDirection
{
    SubChunkTransitionDirection_None = 0,
    SubChunkTransitionDirection_Z    = 1,
    SubChunkTransitionDirection_X    = 2
} e_SubChunkTransitionDirection;

/** @brief Collision point surface data. */
typedef struct _CollisionSurface
{
    /* 0x0 */ q19_12 floorHeight;
    /* 0x4 */ q7_8   slopeGradientX;
    /* 0x6 */ q7_8   slopeGradientZ;
    /* 0x8 */ s8     floorType; /** `e_FloorType` */
              // 3 bytes of padding.
} s_CollisionSurface;
STATIC_ASSERT_SIZEOF(s_CollisionSurface, 12);

/** @brief Collision point data. */
typedef struct _CollisionPoint
{
    /* 0x0  */ VECTOR3            position;   /** Q19.12 */
    /* 0xC  */ s_CollisionSurface surface;
    /* 0x18 */ s32                floorType; /** `e_FloorType` */
} s_CollisionPoint;

/** @brief Collision point probe result. */
typedef struct _CollisionResult
{
    /* 0x0  */ VECTOR3            offset; /** Q19.12 */
    /* 0xC  */ s_CollisionSurface surface;
    /* 0x18 */ q19_12             ceilingHeight;
} s_CollisionResult;

/** @brief Cached collision surface. */
typedef struct _CollisionCachedSurface
{
    /* 0x0 */ u8    subChunkTransDir; /**`e_SubChunkTransitionDirection` */
    /* 0x1 */ u8    surfaceIdx;       // Index for `s_IpdCollisionData::polygonSurfaces`.
    /* 0x2 */ q7_8  collDiffDist;
    /* 0x4 */ q23_8 radiusCollDiffDist;
} s_CollisionCachedSurface;

/** @brief Collision push constraint. */
typedef struct _CollisionPushConstraint
{
    /* 0x0 */ s8    shapeCount; /** Number of soft-collided shapes. */
    /* 0x2 */ q3_12 angleMin;
    /* 0x4 */ q3_12 angleMax;
} s_CollisionPushConstraint;

/** @brief Collision obstacle context for collided shapes. */
typedef struct _CollisionObstacleContext
{
    /* 0x0  */ s_CollisionPushConstraint hardPushConstraint;
    /* 0x6  */ q7_8                      hardIntersectDepth;
    /* 0x8  */ s_CollisionPushConstraint softPushConstraint;
    /* 0xE  */ s8                        __pad_E[2];
    /* 0x10 */ s8*                       shapePassStamps[SOFT_OBSTACLE_SHAPE_COUNT_MAX];
    /* 0x30 */ s_CollisionPushConstraint charaPushConstraint;
    /* 0x36 */ q7_8                      charaIntersectDepth;
} s_CollisionObstacleContext;

/** @brief Copied collision polygon or cylinder attributes. */
typedef union _CollisionShapeAttribs
{
              struct
              {
                  /* 0x0 */ q7_8 radius;
    /* 0x0 */ } cylinder;
              struct
              {
                  /* 0x0 */ u8       surfaceIdx0;
                  /* 0x1 */ u8       surfaceIdx1;
                  /* 0x2 */ u8       surfaceCacheIdx0;
                  /* 0x3 */ u8       surfaceCacheIdx1;
                  /* 0x4 */ u8       disableSurfaceHeight0; /** `bool` */
                  /* 0x5 */ u8       disableSurfaceHeight1; /** `bool` */
                  /* 0x8 */ SVECTOR3 polygonSplitVertex0;   /** Q7.8 | Vertex from `s_IpdCollisionData::polygonSplitVertices`. */
                  /* 0xE */ SVECTOR3 polygonSplitVertex1;   /** Q7.8 | Vertex from `s_IpdCollisionData::polygonSplitVertices`. */
    /* 0x0 */ } polygon;
} s_CollisionShapeAttribs;

/** @brief Collision shape point data. TODO: Come up with better name. */
typedef struct _CollisionShapePoint
{
    /* 0x0  */ s_IpdCollisionData*     ipdCollisionData;
    /* 0x4  */ u8                      shapeIdx; // `s_IpdCollisionData::polygons` index.
    /* 0x5  */ u8                      disableHeight;
    /* 0x6  */ SVECTOR3                offset; // Q7.8 | Probe position inside cell/cell? Y is maybe Z, likely wrong struct.
    /* 0xC  */ s_CollisionShapeAttribs shapeAttribs;
    /* 0x20 */ s_CharaCollisionDeltas  charaDeltas;
} s_CollisionShapePoint;
STATIC_ASSERT_SIZEOF(s_CollisionShapePoint, 56);

/** @brief Collision query shape context TODO: Cell lookup or character to check? */
typedef union _CollisionQueryContext
{
              struct
              {
                  /* 0x0 */ u8                       cellX;
                  /* 0x1 */ u8                       cellZ;
                  /* 0x2 */ u8                       cellSpanX;
                  /* 0x3 */ u8                       cellSpanZ;
                  /* 0x4 */ s_IpdShapeAlloc*         shapeAlloc;
                  /* 0x8 */ s_CollisionCachedSurface surfaceCache[CACHED_SURFACE_COUNT_MAX];
    /* 0x0 */ } shape;
              struct
              {
                  /* 0x0 */ q7_8 bottom;
                  /* 0x2 */ q7_8 top;
                  /* 0x4 */ q7_8 radius;
                  /* 0x6 */ u8   state;     /** `e_CharaCollisionState` */
                  /* 0x8 */ u8*  passStamp; /** Tracks visited characters per query pass. */
    /* 0x0 */ } chara;
} s_CollisionQueryContext;

/** @brief In-progress collision query. */
typedef struct _CollisionQuery
{
    /* 0x0+0  */ u8                         offsetX       : 8;  /** `bool` | Related to `Collision_ShapePassReset`. Maybe name `hasCellChecks`? */
    /* 0x0+8  */ s8                         isCharaMoving : 1;  /** `bool` */
    /* 0x0+9  */ s8                         enablePush    : 1;  /** `bool` | Push away from obstacles. */
    /* 0x0+10 */ s8                         field_0_10    : 1;  /** `bool` | Enable floor check? */
    /* 0x2    */ u16                        triggerFlags  : 16; /** `e_CollisionTriggerFlags` */
    /* 0x4    */ s_CharaCollisionQuery      charaQuery;
    /* 0x34   */ s32                        contactType; /** `s_CollisionContactType` */
    /* 0x38   */ s16                        hitAlpha;    /** Closest hit alpha for a given movement distance. */
    /* 0x3A   */ q11_4                      hitDistance; /** Closest hit distance. */
    /* 0x3C   */ q7_8                       hitOffsetX;  // } Shape center to hit position's offset?
    /* 0x3E   */ q7_8                       hitOffsetZ;  // }
    /* 0x40   */ u8*                        passStamp;   /** Tracks visited shapes/characters per query pass. */
    /* 0x44   */ s_CollisionObstacleContext obstacleContext;
    /* 0x7C   */ q23_8                      slopedFloorHeight; // TODO: How is `floorHeight` different?
    /* 0x80   */ q23_8                      positionX;         /** Set to `chunkOffsetFrom.offset.vx + s_IpdCollisioNData::positionX`. */
    /* 0x84   */ q23_8                      positionZ;         /** Set to `chunkOffsetFrom.offset.vz + s_IpdCollisioNData::positionZ`. */
    /* 0x88   */ q23_8                      slopeGradientX;
    /* 0x8C   */ q23_8                      slopeGradientZ;
    /* 0x90   */ bool                       disableHeight; /** `bool` | TODO: Does disabled height indicate a wall or pit? */
    /* 0x94   */ s32                        floorType;     /** `e_FloorType` */

                 union
                 {
                     DVECTOR_XZ offset; /** Q23.8 */
                     q23_8      vx;
    /* 0x98   */ } chunkOffsetFrom; /** Offset from chunk position. */
                 union
                 {
                     DVECTOR_XZ offset; /** Q23.8 */
                     q23_8      vz;
    /* 0x9C   */ } chunkOffsetTo; /** Offset from chunk position. */

    /* 0xA0   */ s_CollisionQueryContext context;
    /* 0xC8   */ u8                      shapeIdx; /** Secondary. Used only if detecting a cell after not finding one. */
    /* 0xCA   */ q7_8                    floorHeight; // TODO: Base floor height with slope unaccounted for?
    /* 0xCC   */ s_CollisionShapePoint   point;
} s_CollisionQuery;

// emoose: Also works: `extern u16 g_ActiveCollisionTriggers[];`, `arg0->field_4 = g_ActiveCollisionTriggers[0];`.
// Didn't see any array accesses in Ghidra though, struct might be more likely.
extern s_ActiveCollisionTriggers g_ActiveCollisionTriggers;

#define PACKED_MOVE_DIRECTION(moveDir) \
    *(s32*)&(moveDir)

// ========================================
// @split? Collision init, flags, getters.
// ========================================

/** @brief Initializes the collision subsystem, resetting flags and clearing the trigger zone count. */
void Collision_Init(void);

/** @brief Gets the active collision trigger flags.
 *
 * @return Active collision trigger flags.
 */
u16 Collision_TriggerFlagsGet(void);

/** @brief Sets the active collision trigger flags.
 *
 * @param triggerFlags New collision trigger flags.
 */
void Collision_TriggerFlagsSet(u16 triggerFlags);

/** @brief Sets additional active collision trigger flags.
 *
 * @param triggerFlags Additional collision trigger flags to set.
 */
void Collision_TriggerFlagBitsSet(u16 triggerFlags);

/** @brief Clears active collision trigger flags. `CollisionTriggerFlag_Map` is excepted.
 *
 * @param triggerFlags Collision trigger flags to clear.
 */
void Collision_TriggerFlagBitsClear(s32 triggerFlags);

/** @brief Runs through collision triggers and collects those containing the given XZ position.
 *
 * @param posX Query X position.
 * @param posZ Query Z position.
 * @param triggers Array of collision triggers.
 */
void Collision_NearbyTriggersGet(q19_12 posX, q19_12 posZ, const s_CollisionTrigger* triggers);

// ========================================
// @split? IPD functions.
// ========================================

void Collision_ChunkHeaderPtrsInit(s_IpdCollisionData* collData);

/** @brief Resets the shape query pass system, clearing the counter and stamps to 0 in collision data..
 *
 * @param collData Collision data to update.
 */
void Collision_ShapePassReset(s_IpdCollisionData* collData);

/** @brief Increments the shape query pass counter in collision data.
 *
 * @param collData Collision data to update.
 */
void Collision_ShapePassUpdate(s_IpdCollisionData* collData);

// ========================================
// @split? IPD/floor collision detection.
// ========================================

/** @brief Gets the collision surface at a given 2D world position.
 *
 * @param surface Output collision surface.
 * @param posX X world position.
 * @param posZ Z world position.
 */
void Collision_SurfaceGet(s_CollisionSurface* surface, q19_12 posX, q19_12 posZ);

/** @brief Detects a wall collision using the scratchpad for performance.
 *
 * @param collResult Output collision result.
 * @param moveOffset Movement offset.
 * @param chara Character to check.
 * @return Wall response code.
 */
s32 Collision_WallDetect(s_CollisionResult* collResult, const VECTOR3* moveOffset, s_SubCharacter* chara);

/** @brief Handles a wall collision response by sampling the floor at 9 points around a character.
 *
 * @param collResult Output collision result.
 * @param moveOffset Movement offset (unused).
 * @param chara Character to check.
 * @param response Initial collision pass response.
 * @return Wall response code.
 */
s32 Collision_WallResponse(s_CollisionResult* collResult, const VECTOR3* moveOffset, s_SubCharacter* chara, s32 response);

/** @brief Probes floor heights at 16 radial points to push away from a wall.
 * Called in `Collision_WallResponse` and used exclusively for the player.
 *
 * @note Since Y+ is down, the meanings of min/max and lowest/highest are inverted.
 *
 * @param collResult Output collision result with the XZ offset vector adjusted to push away from a wall.
 * @param pos Center position.
 * @param floorHeight Base floor height.
 * @param headingAngle Start heading angle on the XZ plane for the probe circle.
 */
void Collision_WallPush(s_CollisionResult* collResult, const VECTOR3* pos, q19_12 floorHeight, q19_12 headingAngle);

/** @brief Applies collision detection for a character's movement offset.
 *
 * @param collResult Output collision result.
 * @param moveOffset Movement offset.
 * @param chara Character performing the movement.
 * @return Collision result response.
 */
bool Collision_CharaCollisionSetup(s_CollisionResult* collResult, const VECTOR3* moveOffset, s_SubCharacter* chara);

/** @brief Initializes a default collision result with a position and floor height.
 *
 * @param result Collision result to initialize.
 * @param offsetX X offset.
 * @param offsetY Y offset.
 * @param offsetZ Z offset.
 * @param floorHeight Floor height.
 */
void Collision_DefaultResultSet(s_CollisionResult* collResult,
                                q19_12 offsetX, q19_12 offsetY, q19_12 offsetZ, q19_12 floorHeight);

/** @brief Gets an array of collidable characters for collision testing.
 *
 * @param collCharaCount Output number of collidable characters.
 * @param excludedChara Character to exclude, typically the one being tested.
 * @param includePlayer Filter out the player.
 * @return Collidable characters.
 */
s_SubCharacter** Collision_CollidableCharasGet(s32* collCharaCount, const s_SubCharacter* excludedChara,
                                               bool includePlayer);

/** @brief Checks a movement offset against a collision result.
 *
 * @param collResult Collision result.
 * @param offset Movement offset to test.
 * @param cylinder Collision cylinder.
 * @return `true` if movement is possible, `false` otherwise.
 */
s32 Collision_OffsetCheck(s_CollisionResult* collResult, VECTOR* offset, const s_CharaCollisionCylinder* cylinder);

s32 func_8006A42C(s_CollisionResult* collResult, const VECTOR3* offset, const s_CharaCollisionCylinder* cylinder);

// Something for character movement.
// Returns 0, 1, or `NO_VALUE`.
s32 func_8006A4A8(s_CollisionResult* collResult, VECTOR3* moveOffset, const s_CharaCollisionCylinder* cylinder, bool arg3,
                  s_IpdCollisionData** collDataPtrs, s32 collDataIdx, s_CharaCollisionRecord* charaRecords, s32 charaRecordCount,
                  s_SubCharacter** charas, s32 charaCount);

/** @brief Dampens a character's movement offset according to relational cylinder collisions with other characters.
 *
 * @param moveOffset Move offset to dampen.
 * @param cylinder Collision cylinder.
 * @param charas Characters to collide.
 * @param charaCount Number of characters.
 */
void Collision_MoveOffsetDampen(VECTOR3* moveOffset, const s_CharaCollisionCylinder* cylinder,
                                s_SubCharacter** charas, s32 charaCount);

/** @brief Initializes a collision query for a new pass.
 *
 * @param query Collision query to initialize.
 * @param moveOffset Movement offset.
 * @param cylinder Collision cylinder.
 * @param blockInvalidFloor `true` to treat invalid floors as walls, `false` otherwise.
 */
void Collision_CollStateInit(s_CollisionQuery* query, VECTOR3* moveOffset,
                             const s_CharaCollisionCylinder* cylinder, bool blockInvalidFloor);

// ========================================
// @split? Collision query fill.
// ========================================

/** @brief Calculates the movement direction vector and distance from a position offset.
 *
 * @param query Character collision query to update.
 * @param moveOffset Movement offset.
 * @param cylinder Collision cylinder.
 */
void Collision_MoveDirectionCalc(s_CharaCollisionQuery* query,
                                 const VECTOR3* moveOffset, const s_CharaCollisionCylinder* cylinder);

void Collision_CharaCollisionHandle(s_CollisionQuery* query, s_IpdCollisionData* collData);

bool Collision_CellInit(s_CollisionQuery* query, const s_IpdCollisionData* collData);

bool Collision_CellIdxGet(s_CollisionQuery* query, const s_IpdCollisionData* collData);

// Related to polygons.
void func_8006B1C8(s_CollisionQuery* query, s_IpdCollisionData* collData, s_IpdShapeAlloc* shapeAlloc);

// Related to polygons.
bool func_8006B318(s_CollisionQuery* query, const s_IpdCollisionData* collData, s32 shapeIdx);

// Related to polygons.
/** `arg1` is unused, but `func_8006B1C8` passes second arg to this. */
void func_8006B6E8(s_CollisionQuery* query, s_IpdShapeAlloc* shapeAlloc);

// Related to polygons.
bool func_8006B7E0(s_CollisionCachedSurface* cachedSurface, s_CharaCollisionDeltas* charaDeltas);

// Related to polygons.
void func_8006B8F8(s_CollisionShapePoint* cellPoint);

// Related to polygons.
void func_8006B9C8(s_CollisionQuery* query);

// Related to polygons.
void func_8006BB50(s_CollisionQuery* query, s32 pushType);

/** @brief Gets the relative height from the bottom of a character to the floor polygon.
 * Called by `func_8006BB50`.
 *
 * @param query Reference collision query.
 */
q23_8 Collision_CharaDistanceToFloorGet(const s_CollisionQuery* query);

/** @brief Sets the shape intersection depth of a given push type in an obstacle context.
 *
 * @param obstacleContext Obstacle context to update.
 * @param passStamp Shape pass stamp.
 * @param pushType Push type.
 * @param collOffsetX X collision offset.
 * @param collOffsetZ Z collision offset.
 * @param intersectDepth Shape intersection depth.
 */
void Collision_ObstacleIntersectDepthSet(s_CollisionObstacleContext* obstacleContext, u8* passStamp,
                                         u32 pushType, q7_8 collOffsetX, q7_8 collOffsetZ, q7_8 intersectDepth);

void Collision_PushConstraintShapeAdd(s_CollisionPushConstraint* pushConstraint,
                                      q3_12 angleConstraintMin, q3_12 angleConstraintMax);

// Related to polygons.
void func_8006BE40(s_CollisionQuery* query);

void func_8006BF88(s_CollisionQuery* query, const SVECTOR3* splitVert);

void func_8006C0C8(s_CollisionQuery* query, q3_12 hitAlpha, q7_8 arg2);

bool func_8006C1B8(u32 contactType, q3_12 hitAlpha, s_CollisionQuery* query);

/** @brief Computes an obstacle intersection hit distance alpha along the provided movement distance.
 *
 * @param packedDir Packed intersection direction.
 * @param moveDist Movement offset.
 * @param deltaX X offset to the obstacle.
 * @param deltaZ Z offset to the obstacle.
 * @param radius Intersection radius.
 * @return Hit alpha along `moveDist`, or `NO_VALUE` if no intersection.
 */
q3_12 Collision_HitAlphaGet(s32 packedDir, q3_12 moveDist, q3_12 offsetX, q7_8 offsetZ, q7_8 radius);

// Related to cylinders.
bool func_8006C3D4(s_CollisionQuery* query, s_IpdCollisionData* collData, s32 shapeIdx);

// Related to cylinders.
void func_8006C45C(s_CollisionQuery* query);

// Related to cylinders.
void Collision_CylinderObstacleIntersectDepthSet(s_CollisionQuery* query, s32 pushType, q23_8 distToCylinder);

void func_8006C838(s_CollisionQuery* query, s_IpdCollisionData* collData);

// Accounts for slopes in floor height?
void func_8006CA18(s_CollisionQuery* query, s_IpdCollisionData* collData, s_IpdShapeAlloc* shapeAlloc);

// TODO: Computes alpha related to the floor slope?
q3_12 Collision_OffsetAlphaGet(s_CollisionQuery* query);

// Gets sloped floor height, not the base height.
q23_8 Ipd_FloorHeightGet(q23_8 posX, q23_8 posZ, const s_CollisionQuery* query);

void func_8006CC9C(s_CollisionQuery* query);

// Runs through character collision records.
void func_8006CF18(s_CollisionQuery* query, s_CharaCollisionRecord* charaRecords, s32 charaRecordCount);

void func_8006D01C(VECTOR3* extraOffset, VECTOR3* offset, q3_12 offsetAlpha, s_CollisionQuery* query);

void func_8006D2B4(VECTOR3* offset, s_CollisionObstacleContext* obstacleContext);

void func_8006D600(VECTOR3* pos, q19_12 angle, q19_12 rotX, q19_12 rotY, q19_12 hardIntersectDepth);

/** @brief Sets raw character movement parameters and clears all shape contact pushes.
 * Called by `func_8006A4A8`.
 *
 * @param query Collision query to update.
 * @param extraOffset Extra start offset (Q19.12).
 * @param moveOffset Movement offset (Q19.12).
 */
void Collision_CharaRawMovementSet(s_CollisionQuery* query, const VECTOR3* extraOffset, const VECTOR3* moveOffset);

/** @brief Sets raw character movement parameters.
 * Called by `Collision_CharaRawMovementSet`.
 *
 * @param query Character collision query to update.
 * @param extraOffset Extra start offset (Q23.8).
 * @param moveOffset Movement offset (Q23.8).
 */
void Collision_CharaMovementSet(s_CharaCollisionQuery* query, const SVECTOR* extraOffset, const SVECTOR* moveOffset);

#endif
