#ifndef _BODYPROG_COLLISION_RAY_H
#define _BODYPROG_COLLISION_RAY_H

#include "bodyprog/chara/chara.h"
#include "bodyprog/formats/ipd.h"

typedef struct
{
    s8         unk_0[8];
    DVECTOR    field_8;
    s16        field_C;
    s16        field_E;
    q19_12     field_10; // } X
    q19_12     field_14; // } Z
    s16        field_18;
    s8         unk_1A[2];
    s32        field_1C; // Index into `field_20`.
    DVECTOR_XZ field_20[2];
} s_func_8006E490;

// Something with a projected character position.
typedef struct
{
    /* 0x0 */ q23_8 positionX;
    /* 0x4 */ q23_8 positionZ;
              union
              {
                  struct
                  {
                      /* 0x8 */ q7_8 floorHeight;
                      /* 0xA */ q7_8 topHeight; // Not top box/cylinder height but something else?
                      /* 0xC */ q7_8 radius;
                  } shape;
                  struct
                  {
                      /* 0x8 */ DVECTOR_XZ offset;   /** Q7.8 */
                      /* 0xC */ DVECTOR_XZ offset_C; /** Q7.8 */
                  } unk;
              } u; // TODO: Temp name.
} s_RayQuery_6C;

// Something related to cell ranges.
typedef struct
{
    s16 field_0;
    s16 field_2;
} s_RayQuery_8C;

/** @brief In-progress ray trace query. Contains pointers to active characters among other things. */
typedef struct _RayQuery
{
    /* 0x0  */ bool             ignoreFloors; // TODO: Unsure, probably more of a high-level query path flag.
    /* 0x4  */ s16              triggerFlags; /** `e_CollisionTriggerFlags` */
    /* 0x6  */ q7_8             radius;
    /* 0x8  */ q7_8             hitDistance;
    /* 0xA  */ s8               __pad_A[2];
    /* 0xC  */ VECTOR3          target; // Q23.8
    /* 0x18 */ s8               unk_18[4];
    /* 0x1C */ q7_8             floorHeight;
    /* 0x1E */ s8               __pad_1E[2];
    /* 0x20 */ s_SubCharacter*  character;
    /* 0x24 */ q7_8             offsetX;
    /* 0x26 */ q7_8             offsetZ;
    /* 0x28 */ s32              floorType; /** `e_FloorType` */
    /* 0x2C */ VECTOR3          from;       // Q23.8
    /* 0x38 */ s8               unk_38[4];
    /* 0x3C */ s32              field_3C; // X  } Q23.8 `VECTOR3`?
    /* 0x40 */ s32              field_40; // Y? }
    /* 0x44 */ s32              field_44; // Z  }
    /* 0x48 */ s8               unk_48[4];
    /* 0x4C */ q7_8             field_4C; // X?
    /* 0x4E */ q7_8             field_4E; // Height offset.
    /* 0x50 */ SVECTOR          offset;   // Q23.8
    /* 0x58 */ u16              field_58; // X } Movement direction vector?
    /* 0x5A */ s16              field_5A; // Z }
    /* 0x5C */ q7_8             rayDistance;
    /* 0x5E */ s16              field_5E;
    /* 0x60 */ s16              field_60;
    /* 0x62 */ s8               __pad_62[2];
    /* 0x64 */ s_SubCharacter** collidableCharas;
    /* 0x68 */ s32              collidableCharaCount; /** `collidableCharas` array size. */
    /* 0x6C */ s_RayQuery_6C    field_6C;
    /* 0x7C */ s32              cellCountX;
    /* 0x80 */ s32              cellCountZ;
    /* 0x84 */ q8_8             cellSize;
    /* 0x86 */ u8               __pad_86[2];
    /* 0x88 */ s32              field_88;    // `field_8C` array size.
    /* 0x8C */ s_RayQuery_8C    field_8C[1]; // Unknown buffer size.
} s_RayQuery;

/** @brief Collision ray trace. */
typedef struct _RayTrace
{
    /* 0x0  */ s8              hasHit;     /** `bool` */
    /* 0x1  */ u8              floorType;  /** `e_FloorType` */
    /* 0x2  */ s8              __pad_2[2];
    /* 0x4  */ VECTOR3         target;     /** Q19.12 */
    /* 0x10 */ s_SubCharacter* character;
    /* 0x14 */ q19_12          hitDistance;
    /* 0x18 */ q19_12          floorHeight;
    /* 0x1C */ q3_12           headingAngle;
} s_RayTrace;

/** @brief Gets a collision ray trace with level geometry.
 *
 * @param trace Output ray trace.
 * @param from Ray origin (Q19.12).
 * @param to Ray target (Q19.12).
 * @return `true` if the ray hit an obstacle, `false` otherwise.
 */
bool Ray_GeomTraceGet(s_RayTrace* trace, const VECTOR3* from, const VECTOR3* to);

/** @brief Gets a collision ray trace with level geometry and characters, optionally ignoring a specified character.
 *
 * @param trace Output ray trace.
 * @param from Ray origin (Q19.12).
 * @param offset Ray target offset from the origin (Q19.12).
 * @param excludedChara Character to ignore.
 * @return `true` if the ray hit an obstacle, `false` otherwise.
 */
bool Ray_GeomAndCharaTraceGet(s_RayTrace* trace, const VECTOR3* from, const VECTOR3* offset,
                              const s_SubCharacter* excludedChara);

/** @brief Sets a missed collision ray trace.
 *
 * @param trace Ray trace to update.
 * @param from Ray origin (Q19.12).
 * @param offset Ray target offset from the origin (Q19.12).
 * @param hitDist Hit distance to set.
 */
void Ray_MissSet(s_RayTrace* trace, const VECTOR3* from, const VECTOR3* offset, q23_8 hitDist);

/** @brief Checks if a collision ray's line of sight hit level geometry or a character, optionally ignoring a
 * specified character.
 *
 * @param trace Ray trace.
 * @param from Ray origin (Q19.12).
 * @param offset Ray target offset from the origin (Q19.12).
 * @param excludedChara Character to ignore.
 * @return `true` if there is an obstacle, `false` otherwise.
 */
bool Ray_GeomAndCharaTraceGetAlt(s_RayTrace* trace, const VECTOR3* from, const VECTOR3* offset,
                                 s_SubCharacter* excludedChara);

bool func_8006DC18(s_RayTrace* trace, const VECTOR3* from, const VECTOR3* offset);

bool Ray_TraceSetup(s_RayQuery* query, bool ignoreFloors, q7_8 radiusOffset, const VECTOR3* from, const VECTOR3* offset,
                    q19_12 unusedX, q19_12 unusedZ,
                    s_SubCharacter** collCharas, s32 collCharaCount);

bool Ray_TraceRun(s_RayTrace* trace, s_RayQuery* query);

// Fills `query` with info.
void func_8006E0AC(s_RayQuery* query, const s_IpdCollisionData* collData);

void func_8006E150(s_func_8006E490* arg0, DVECTOR arg1, DVECTOR arg2);

void func_8006E490(s_func_8006E490* arg0, u32 flags, q19_12 posX, q19_12 posZ);

void func_8006E53C(s_RayQuery* query, s_IpdShapeAlloc* shapeAlloc, s_IpdCollisionData* collData);

void func_8006E78C(s_RayQuery* query, s_IpdPolygon* polygon, SVECTOR3* splitVerts, s_IpdSurface* surfaces, bool hasSurface);

void func_8006EB8C(s_RayQuery* query, s_IpdCylinder* cylinder);

// Fills ray hit data?
void func_8006EE0C(s_RayQuery_6C* arg0, bool ignoreFloors, const s_SubCharacter* chara);

// Sets hit character ray query?
void func_8006EEB8(s_RayQuery* query, s_SubCharacter* chara);

#endif
