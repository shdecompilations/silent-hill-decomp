#ifndef _BODYPROG_MAP_TERRAIN_H
#define _BODYPROG_MAP_TERRAIN_H

#include "bodyprog/formats/ipd.h"
#include "bodyprog/formats/lm.h"

#define ACTIVE_CHUNK_COUNT_MAX            4
#define ACTIVE_CHUNK_TEXTURE_COUNT_MAX    10
#define HALF_PAGE_CHUNK_TEXTURE_COUNT_MAX 2
#define FULL_PAGE_CHUNK_TEXTURE_COUNT_MAX 8

typedef enum _MapModelLoadState
{
    MapModelLoadState_Invalid   = 0,
    MapModelLoadState_Unloaded  = 1,
    MapModelLoadState_Corrupted = 2, // Maybe wrong name for this.
    MapModelLoadState_Loaded    = 3
} e_MapModelLoadState;

/** @brief IPD chunk file load states.
 *
 * See `Map_ChunkLoadStateGet`.
 */
typedef enum _WorldMapLoadState
{
    WorldMapLoadState_Invalid  = 0, /** Entry index is `NO_VALUE`. */
    WorldMapLoadState_Unloaded = 1, /** Not currently loaded. */
    WorldMapLoadState_Loaded   = 2  /** Currently loaded. */
} e_WorldMapLoadState;

/** @brief Map chunk. */
typedef struct _MapChunk
{
    /* 0x0  */ s_IpdHeader* ipdHdr;
    /* 0x4  */ s32          queueIdx;
    /* 0x8  */ s16          chunkX;
    /* 0xA  */ s16          chunkZ;
    /* 0xC  */ q19_12       paddedDistanceToEdge;
    /* 0x10 */ q19_12       projPaddedDistanceToEdge;
    /* 0x14 */ u8           materialCount;
             // 3 bytes of padding.
    /* 0x18 */ s32          outsideCount;
} s_MapChunk;
STATIC_ASSERT_SIZEOF(s_MapChunk, 28);

/** @brief Map chunk column.
 * TODO: Or row?
 * Will: Or it should be a 2D array. Also this may actually be cells, not chunks.
         Based in `WorldMap_MakeGrid`, the 2D array dimensions should be 19x16 and possibly some Q8 value.
 */
typedef struct _ChunkColumn
{
    /* 0x0 */ s16 idx[16];
} s_ChunkColumn;
STATIC_ASSERT_SIZEOF(s_ChunkColumn, 32);

typedef struct _ActiveChunkTextures
{
    /* 0x0 */ s32        count;
    /* 0x4 */ s_Texture* textures[ACTIVE_CHUNK_TEXTURE_COUNT_MAX];
} s_ActiveChunkTextures;

/** @brief Texture data associated with chunks. */
typedef struct _ChunkTextures
{
    /* 0x0   */ s_ActiveChunkTextures fullPage;
    /* 0x2C  */ s_ActiveChunkTextures halfPage;
    /* 0x58  */ s_Texture             fullPageTextures[FULL_PAGE_CHUNK_TEXTURE_COUNT_MAX];
    /* 0x118 */ s_Texture             halfPageTextures[HALF_PAGE_CHUNK_TEXTURE_COUNT_MAX];
} s_ChunkTextures;
STATIC_ASSERT_SIZEOF(s_ChunkTextures, 328);

/** @brief Global PLM model. */
typedef struct _GlobalPlm
{
    /* 0x0 */ s_LmHeader* lmHdr; // TODO: Should be an array according to `WorldMap_ChunkPropertiesSet` call?
    /* 0x4 */ s32         fileIdx;
    /* 0x8 */ s32         queueIdx;
} s_GlobalPlm;

/** @brief Map data and layout. */
typedef struct _WorldMapWork
{
    /* 0x0   */ s_IpdCollisionData collisionData; // Default chunk collision data?
    /* 0x134 */ s32                textureFileIdx;
    /* 0x138 */ s_GlobalPlm        globalPlm;
    /* 0x144 */ char               mapTag[4];
    /* 0x148 */ s32                mapTagSize;
    /* 0x14C */ s32                ipdFileIdx;
    /* 0x150 */ s_IpdHeader*       chunkBuffer;
    /* 0x154 */ s32                chunkBufferSize;
    /* 0x158 */ s32                activeChunkCount;
    /* 0x15C */ s_MapChunk         activeChunks[ACTIVE_CHUNK_COUNT_MAX];
    /* 0x1CC */ s_ChunkColumn      chunksGrid[19];
    /* 0x42C */ s_ChunkColumn*     chunksGridCenter; // TODO: All access to this variable relies on hacks.
    /* 0x430 */ s_ChunkTextures    chunkTextures;
    /* 0x578 */ q19_12             positionX;
    /* 0x57C */ q19_12             positionZ;
    /* 0x580 */ s32                chunkX;
    /* 0x584 */ s32                chunkZ;
    /* 0x588 */ bool               isExterior;
} s_WorldMapWork;
STATIC_ASSERT_SIZEOF(s_WorldMapWork, 1420);

/** @brief Packs XZ cell coordinates in a single value.
 *
 * @param x X cell coordinate.
 * @param z Z cell coordinate.
 * @return Packed XZ cell coordinates.
 */
#define CELL_XZ(x, z) \
    ((x) + ((z) << 8))

#endif
