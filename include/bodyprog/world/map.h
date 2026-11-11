#ifndef _BODYPROG_WORLD_MAP_H
#define _BODYPROG_WORLD_MAP_H

/** @brief Initializes map data and chunks.
 *
 * @param lmHdr LM header.
 * @param ipdBuf IPD chunk file buffer.
 * @param ipdBufSize IPD chunk file buffer size.
 */
void WorldMap_Init(s_LmHeader* lmHdr, s_IpdHeader* ipdBuf, s32 ipdBufSize);

void WorldMap_GlobalLmInit(s_GlobalPlm* globalPlm, s_LmHeader* lmHdr);

void WorldMap_GlobalLmHeaderInit(s_LmHeader* lmHdr);

/** @brief Clears the `queueIdx` for each active chunk in an array.
 *
 * @param activeChunks Active chunk array.
 * @param activeChunkCount Number of active chunks.
 */
void WorldMap_ChunkQueueClear(s_MapChunk* activeChunks, s32 activeChunkCount);

void WorldMap_TexturesInit(void);

void WorldMap_CollisionDataReset(void);

/** @brief Places a chunk at given XZ chunk coordinates.
 *
 * @param ipdFileIdx Index of the IPD chunk file to place.
 * @param chunkX X chunk coordinate.
 * @param chunkZ Z chunk coordinate.
 */
void WorldMap_ChunkSet(s16 ipdFileIdx, s32 chunkX, s32 chunkZ);

void WorldMap_ActiveChunksClear(void);

void WorldMap_TexturesRefClear(void);

void WorldMap_Reset(void);

void WorldMap_GlobalLmReset(void);

s_Texture* WorldMap_ActiveTextureInfoGet(char* texName);

void WorldMap_InfoSet(char* mapTag, e_FsFile plmIdx, s32 activeChunkCount, bool isExterior,
                      e_FsFile ipdFileIdx, e_FsFile texFileIdx);

void WorldMap_ChunksClear(s_WorldMapWork* terrain, s32 activeChunkCount);

/** @brief Locates all IPD files for a given map type.
 *
 * Example:
 * Map type THR.
 * `file 1100` is `THR0205.IPD`, `chunksGridCenter[2][5] = 1100`.
 */
void WorldMap_MakeGrid(s_WorldMapWork* terrain, char* mapTag, e_FsFile fileIdxStart);

/** @brief Converts two hex `char`s to an integer hex value.
 *
 * @param out Output hex `int`.
 * @param hex0 First hex `char`.
 * @param hex1 Second hex `char`.
 */
bool ConvertHexToS8(s32* out, char hex0, char hex1);

s_IpdCollisionData** WorldMap_ActiveChunksCollisionDataGet(s32* collDataIdx);

/** @brief Gets the collision data at a given position.
 *
 * @param posX X search position.
 * @param posZ Z search position.
 * @return Collision data.
 */
s_IpdCollisionData* WorldMap_CollisionDataGet(q19_12 posX, q19_12 posZ);

/** @brief Gets the location of a nearby world object model.
 *
 * @param model Model to find.
 * @param metadata Model metadata.
 * @param posX X search position.
 * @param posZ Z search position.
 * @return World object model location (`e_WorldModelLocation`).
 */
s32 WorldMap_ObjectModelLocationGet(s_WorldObjectModel* model, s_WorldObjectMetadata* metadata, q19_12 posX, q19_12 posZ);

/** @brief Checks if an IPD chunk file is loaded.
 *
 * @param activeChunkIdx Active chunk index.
 * @return `true` if the IPD chunk file is loaded, `false` otherwise.
 */
bool WorldMap_ActiveChunkLoadedCheck(s32 activeChunkIdx);

/** @brief Starts the process of loading map geometry and assigns textures when the game is set in a loading screen mode? */
void WorldMap_ChunkInit(q19_12 posX, q19_12 posZ, q19_12 projPosX, q19_12 projPosZ);

/** @brief Computes the padded distance from an XZ position to the a chunk edge.
 * For exteriors, the chunk edge is expanded by `Q12(1.0f)`.
 *
 * @note If the position resides inside the chunk, the distance is `Q12(0.0f)`.
 *
 * @param posX X position.
 * @param posZ Z position.
 * @param chunkX X chunk coordinate.
 * @param chunkZ Z chunk coordinate.
 * @param isExterior `true` for padded exterior, `false` for non-padded interior.
 * @return Padded distance from the XZ position to the chunk edge.
 */
q19_12 WorldMap_PaddedDistanceToChunkEdgeGet(q19_12 posX, q19_12 posZ, s32 chunkX, s32 chunkZ, bool isExterior);

/** @brief Computes the distance from an XZ position to a chunk edge.
 *
 * @note If the position resides inside the chunk, the distance is `Q12(0.0f)`.
 *
 * @param posX X position.
 * @param posZ Z position.
 * @param chunkX X chunk coordinate.
 * @param chunkZ Z chunk coordinate.
 * @return Distance from the XZ position to the chunk edge.
 */
q19_12 WorldMap_DistanceToChunkEdgeGet(q19_12 posX, q19_12 posZ, s32 chunkX, s32 chunkZ);

/** Loads geometry, sets materials and properly assigns the position of the map when loading a new room/map? */
s32 WorldMap_ChunkLoad(s_WorldMapWork* terrain, q19_12 posX, q19_12 posZ, q19_12 projPosX, q19_12 projPosZ);

void WorldMap_ActiveChunksSample(s_WorldMapWork* terrain, q19_12 posX, q19_12 posZ, q19_12 projPosX, q19_12 projPosZ,
                                 bool isExterior);

/** @brief Computes the distance from current and projected positions to the closest edge of a chunk.
 *
 * @note The results are stored in the chunk.
 *
 * @param chunk Chunk to check.
 * @param posX Current X position.
 * @param posZ Current Z position.
 * @param projX Projected X position.
 * @param projZ Projected Z position.
 * @param isExterior `true` for exterior, `false` for interior.
 */
void WorldMap_DistanceToEdgeCalc(s_MapChunk* chunk, q19_12 posX, q19_12 posZ, q19_12 projPosX, q19_12 projPosZ,
                                 bool isExterior);

/** Sets materials for active chunks? */
void WorldMap_ChunkMaterialsApply(s_WorldMapWork* terrain);

/** @brief Gets the IPD chunk file index for a chunk at given coordinates.
 *
 * @param chunkX X chunk coordinate.
 * @param chunkZ Z chunk coordinate.
 * @return IPD chunk file index.
 */
s32 WorldMap_IpdChunkFileIdxGet(s32 chunkX, s32 chunkZ);

/** @brief Checks if an active chunk exists at given coordinates.
 *
 * @param activeChunks Active chunkcs to check.
 * @param chunkX X chunk coordinate.
 * @param chunkZ Z chunk coordinate.
 * @return `true` if an active chunk exists at the coordinate, `false` otherwise.
 */
bool WorldMap_IsChunkPresentCheck(const s_MapChunk* activeChunks, s32 chunkX, s32 chunkZ);

/** @brief Finds a free slot for a new chunk.
 *
 * @param activeChunks Active chunks to search.
 * @param isExterior `true` for exterior, `false` for interior.
 */
s_MapChunk* WorldMap_FreeChunkGet(s_MapChunk* activeChunks, bool isExterior);

/** @brief Loads a new chunk from an IPD file.
 *
 * @param chunk Chunk slot to load into.
 * @param fileId IPD chunk file index.
 * @param chunkX X chunk coordinate.
 * @param chunkZ Z chunk coordinate.
 * @param posX Current X position from which to compute the distance to the chunk edge.
 * @param posZ Current Z position from which to compute the distance to the chunk edge.
 * @param projX Projected X position from which to compute the distance to the chunk edge.
 * @param projZ Projected Z position from which to compute the distance to the chunk edge.
 * @param isExterior `true` for exterior, `false` for interior.
 */
s32 WorldMap_ChunkLoadStart(s_MapChunk* chunk, e_FsFile fileIdx, s32 chunkX, s32 chunkZ,
                            q19_12 posX, q19_12 posZ, q19_12 projPosX, q19_12 projPosZ, bool isExterior);

/** @brief Checks if all base map data (IPD and global LM models) have been loaded properly. */
bool WorldMap_ActiveModelsLoadStateCheck(void);

/** @brief Checks if the player is in close proximity to an unloaded ahead which should be loaded.
 *
 * @return `true` if a new chunk needs to be loaded, `false` otherwise.
 */
bool WorldMap_NextChunkLoadCheck(void);

/** Checks if a position is within the current map chunk. */
bool WorldMap_CloseChunkEdgeCheck(q19_12 posX, q19_12 posZ);

void WorldMap_ChunksDraw(GsOT* ot, s32 otShift);

bool WorldMap_ChunkPositionMatchCheck(s_MapChunk* chunk, s_WorldMapWork* terrain);

/** Checks if PLM texture is loaded? */
bool WorldMap_TextureLoadedCheck(s_IpdHeader* ipdHdr);

s_IpdCollisionData* WorldMap_HeaderCollisionDataGet(s_IpdHeader* ipdHdr);

void WorldMap_ChunkPropertiesSet(s_IpdHeader* ipdHdr, s_LmHeader** lmHdrs, s32 lmHdrCount,
                                 s_ActiveChunkTextures* fullPageActiveTexs, s_ActiveChunkTextures* halfPageActiveTexs,
                                 e_FsFile fileIdx);

void WorldMap_ChunkMaterialsLoad(s_IpdHeader* ipdHdr,
                                 s_ActiveChunkTextures* fullPageActiveTexs, s_ActiveChunkTextures* halfPageActiveTexs,
                                 e_FsFile fileIdx);

/** Checks if IPD is loaded before returning texture count? */
s32 WorldMap_ChunkHalfPageMaterialCountGet(s_IpdHeader* ipdHdr);

/** Returns inverse result of `LmFilter_IsHalfPage`. */
bool LmFilter_IsFullPage(s_Material* mat);

bool LmFilter_IsHalfPage(s_Material* mat);

void WorldMap_ChunkHeaderPtrsInit(s_IpdHeader* ipdHdr);

/** @brief Assigns `s_ModelHeader` pointers to models in `s_IpdHeader` by searching the given `s_LmHeader` array. */
void WorldMap_ModelLinkObjectLists(s_IpdHeader* ipdHdr, s_LmHeader** externalLmHdrs, s32 externalLmHdrCount);

/** @brief Searches `s_LmHeader` for objects with the given `objName`. */
s_ModelHeader* LmHeader_ModelHeaderSearch(u_Filename* modelName, s_LmHeader* lmHdr);

/** @brief Assigns `s_ModelHeader` pointers to each `s_IpdModelBuffer` in `s_IpdHeader`. */
void WorldMap_ModelBufferLinkObjectLists(s_IpdHeader* ipdHdr, s_IpdModelInfo* ipdModelInfos);

/** Sets IPD collision data chunks? */
void func_80044044(s_IpdHeader* ipd, s32 chunkX, s32 chunkZ);

/** @brief Draws an IPD chunk.
 *
 * @param ipdHdr IPD chunk data to draw.
 * @param posX X world position.
 * @param posZ Z world position.
 * @param ot Ordering table.
 * @param otShift Ordering table shift.
 */
void WorldMap_Draw(s_IpdHeader* ipdHdr, q19_12 posX, q19_12 posZ, GsOT* ot, s32 otShift);

/** @brief Checks if an IPD subchunk is visible.
 *
 * @param modelBuf IPD model buffer.
 * @param subchunkX X subchunk position.
 * @param subchunkZ Z subchunk position.
 * @param chunkEdgeX X chunk edge position.
 * @param chunkEdgeZ Z chunk edge position.
 */
bool WorldMap_SubchunkVisibleCheck(s_IpdModelBuffer* modelBuf,
                                   q7_8 subchunkX, q7_8 subchunkZ, q23_8 chunkEdgeX, q23_8 chunkEdgeZ);

#endif
