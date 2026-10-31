#ifndef _BODYPROG_FORMATS_IPD_H
#define _BODYPROG_FORMATS_IPD_H

#include "bodyprog/formats/lm.h"
#include "bodyprog/formats/model.h"

/** @brief GLOSSARY OF TERMS
 *
 * Chunk:    40x40 meter area. 1 per IPD file, usually contain 20x20 cells.
 * Subchunk: 8x8 meter area. 25 per chunk.
 * Cell:     Variable-size collision grid unit, usually 2x2 meters.
 * Polygon:  Collision shape with up to 2 surfaces divided by a split.
 * Surface:  Polygon split containing fine-grained collision attributes.
 * Cylinder: Collision shape.
 */

#define IPD_HEADER_MAGIC 20

#define CHUNK_SIZE    40.0f /** 40x40 meters. */
#define SUBCHUNK_SIZE 8.0f  /** 8x8 meters. */

/** @brief Floor material types. TODO: SH2 suggests "material"? */
typedef enum _FloorType
{
    FloorType_Default = 0,
    FloorType_1       = 1,  // Uses concrete sound.
    FloorType_2       = 2,
    FloorType_Grass   = 3,
    FloorType_4       = 4,
    FloorType_5       = 5,
    FloorType_6       = 6,  // Uses metal sound.
    FloorType_7       = 7,  // Uses concrete sound.
    FloorType_8       = 8,
    FloorType_9       = 9,
    FloorType_10      = 10, // Uses metal sound.
    FloorType_11      = 11, // Uses metal sound.
    FloorType_None    = 12
} e_FloorType;

/** @brief IPD collision surface. */
typedef struct _IpdSurface
{
    /* 0x0    */ SVECTOR3 offset;              /** Q7.8 */
    /* 0x6+0  */ u16      floorType       : 5; /** `e_FloorType` */
    /* 0x6+5  */ u16      disableHeight   : 3; /** `bool` */
    /* 0x6+8  */ u16      surfaceCacheIdx : 3; /** Index into `s_CollisionQuery::shape.surfaceCache`. */
    /* 0x6+11 */ u16      triggerFlagIdx  : 4; /** `e_CollisionTriggerFlags` index. */
    /* 0x8    */ q7_8     slopeGradientX;
    /* 0xA    */ q7_8     slopeGradientZ;
} s_IpdSurface;

/** @brief IPD collision polygon shape, composed of up to 2 surfaces split by a line. */
typedef struct _IpdPolygon
{
    /* 0x0+0  */ q7_8 offsetX            : 14;
    /* 0x0+14 */ u16  triggerFlagIdxHigh : 2;  /** `e_CollisionTriggerFlags` index | Combined with `triggerFlagIdxLow` for complete field. */
    /* 0x2+0  */ q7_8 offsetY            : 14;
    /* 0x2+14 */ u16  triggerFlagIdxLow  : 2;  /** `e_CollisionTriggerFlags` index | Combined with `triggerFlagIdxHigh` for complete field. */
    /* 0x4    */ q7_8 offsetZ;
    /* 0x6    */ u8   splitVertexIdx0;         /** `s_IpdCollisionData::polygonSplitVertices` index. */
    /* 0x7    */ u8   splitVertexIdx1;         /** `s_IpdCollisionData::polygonSplitVertices` index. */
    /* 0x8    */ u8   surfaceIdx0;             /** `s_IpdCollisionData::polygonSurfaces` index. */
    /* 0x9    */ u8   surfaceIdx1;             /** `s_IpdCollisionData::polygonSurfaces` index. */
} s_IpdPolygon;

/** @brief IPD collision cylinder shape. */
typedef struct _IpdCylinder
{
    /* 0x0+0 */ u16      floorType      : 5; /** `e_FloorType` */
    /* 0x0+5 */ u16      disableHeight  : 3; /** `bool` */
    /* 0x0+8 */ u16      triggerFlagIdx : 4; /** `e_CollisionTriggerFlags` index. */
    /* 0x2   */ SVECTOR3 offset;             /** Q7.8 */
    /* 0x8   */ q7_8     radius;
} s_IpdCylinder;

/** @brief IPD collision shapes allocation for a cell (represented by pair). */
typedef struct _IpdShapeAlloc
{
    /* 0x0 */ s16 shapeLutIdx;   /** Index into `s_IpdCollisionData::shapeLut`. */
    /* 0x2 */ s16 surfaceLutIdx; /** Index into `s_IpdCollisionData::surfaceLut`. */
} s_IpdShapeAlloc;

/** @brief IPD collision data. */
typedef struct _IpdCollisionData
{
    /* 0x0    */ q23_8            positionX;                   /** } World position. */
    /* 0x4    */ q23_8            positionZ;                   /** } */
    /* 0x8+0  */ u32              polygonSplitVertexCount : 8; /** `polygonSplitVertices` array size. */
    /* 0x8+8  */ u32              polygonSurfaceCount     : 8; /** `polygonSurfaces` array size. */
    /* 0x8+16 */ u32              polygonCount            : 8; /** `polygons` array size. */
    /* 0x8+24 */ u32              cylinderCount           : 8; /** `cylinders` array size. */
    /* 0xC    */ SVECTOR3*        polygonSplitVertices;        /** Q7.8 | Pairs define a polygon line split. */
    /* 0x10   */ s_IpdSurface*    polygonSurfaces;             /** Unique collision surfaces. */
    /* 0x14   */ s_IpdPolygon*    polygons;                    /** Unique collision polygons. */
    /* 0x18   */ s_IpdCylinder*   cylinders;                   /** Unique collision cylinders. */
    /* 0x1C   */ q7_8             cellSize;                    /** `Q8(2.0f)` in most IPD files, sometimes `Q8(0.0f)`. */
    /* 0x1E   */ u8               cellCountX;                  /** 20 in most IPD files, sometimes 0. */
    /* 0x1F   */ u8               cellCountZ;                  /** 20 in most IPD files, sometimes 0. */
    /* 0x20   */ s_IpdShapeAlloc* shapeAllocs;                 /** Shape allocations per cell. Pairs of elements define index ranges. */
    /* 0x24   */ u16              shapeLutSize;                /** `shapeLut` array size. */
    /* 0x26   */ u16              surfaceLutSize;              /** `surfaceLut` array size. */
    /* 0x28   */ u8*              shapeLut;                    /** Indices into `polygons` + `cylinders`. */
    /* 0x2C   */ u8*              surfaceLut;                  /** Indices into `polygonSurfaces`. */
    /* 0x30   */ u8               shapePassCounter;            /** Active shape pass. Stamps below this value, i.e. from a previous pass, mark unprocessed shapes. */
    /* 0x31   */ s8               __pad_31[3];
    /* 0x34   */ u8               shapePassStampBuffer[256];   /** Tracks visited shapes per query pass. */
} s_IpdCollisionData;

/** @brief IPD model info. */
typedef struct _IpdModelInfo
{
    /* 0x0 */ u8             isGlobalPlm; /** `true` if loaded from `BG/*_GLB.PLM` file, otherwise loaded from IPD itself. */ 
    /* 0x1 */ s8             __pad_1[3];
    /* 0x4 */ u_Filename     name;
    /* 0xC */ s_ModelHeader* modelHdr;
} s_IpdModelInfo;

/** @brief IPD transformed model instance. */
typedef struct _IpdModelInstance
{
    /* 0x0 */ s_ModelHeader* modelHdr;
    /* 0x4 */ MATRIX         transformMat; /** Translation is relative to the chunk. */
} s_IpdModelInstance;

/** @brief IPD transformed billboard sprite instance. */
typedef struct _IpdBillboardInstance
{
    /* 0x0 */ SVECTOR3 offset; /** Q7.8 | Offset in chunk. */
    /* 0x6 */ s8       idx;    // Index of drawable billboard in memory? Seems each IPD can only have 2 unique billboards.
    /* 0x8 */ s8       __pad_8;
} s_IpdBillboardInstance;

// TODO: Name uncertain. Used as (relative?) 2D visibility AABB, but sizes are weirdly inconsistent. What are they encompassing?
typedef struct _SubchunkRange
{
    /* 0x0 */ q7_8 minX;
    /* 0x2 */ q7_8 maxX;
    /* 0x4 */ q7_8 minZ;
    /* 0x6 */ q7_8 maxZ;
} s_SubchunkRange;

// Just props?
/** @brief IPD model buffer of multiple models and billboards. */
typedef struct _IpdModelBuffer
{
    /* 0x0  */ u8                      modelInstanceCount;     /** `modelInstances` array size. */
    /* 0x1  */ u8                      billboardInstanceCount; /** `billboardInstances` array size. */
    /* 0x2  */ u8                      subchunkCount;
    /* 0x3  */ s8                      __pad_3;
    /* 0x4  */ q7_8                    minX; // } `s_SubchunkRange`?
    /* 0x6  */ q7_8                    maxX; // }
    /* 0x8  */ q7_8                    minZ; // }
    /* 0xA  */ q7_8                    maxZ; // }
    /* 0xC  */ s_IpdModelInstance*     modelInstances;
    /* 0x10 */ s_IpdBillboardInstance* billboardInstances;
    /* 0x14 */ s_SubchunkRange*        subchunkRanges;
} s_IpdModelBuffer;

/** @brief IPD header. */
typedef struct _IpdHeader
{
    /* 0x0  */ u8                 magic;
    /* 0x1  */ u8                 isLoaded;         /** `bool` */
    /* 0x3  */ s8                 chunkX;           /** } Position in chunk grid. */
    /* 0x3  */ s8                 chunkZ;           /** } */
    /* 0x4  */ s_LmHeader*        lmHdr;
    /* 0x8  */ u8                 modelInfoCount;   /** `modelInfos` array size. */
    /* 0x9  */ u8                 modelBufferCount; /** `modelBuffers` array size. */
    /* 0xA  */ u8                 modelOrderSize;   /** `modelOrder` array size. */
    /* 0xB  */ s8                 __pad_B[9];
    /* 0x14 */ s_IpdModelInfo*    modelInfos;   // Unique models.
    /* 0x18 */ s_IpdModelBuffer*  modelBuffers; // Chunk geom groups?
    /* 0x1C */ u8                 textureCount; // TODO: Should it be `u32`? Referenced once, likely not texture count.
                                                // "`u8` - Relative pointer to textures list"
                                                // "`u32` - Relative pointer to object order"
    /* 0x1D */ s8                 unk_1D[51];   // TODO: Actual texture data array? Does this contain billboards?
    /* 0x50 */ u8*                modelOrder;   /** Indices into `modelBuffers`. */
    /* 0x54 */ s_IpdCollisionData collisionData;
} s_IpdHeader;

#endif
