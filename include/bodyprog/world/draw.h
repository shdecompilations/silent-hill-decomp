#ifndef _BODYPROG_WORLD_DRAW_H
#define _BODYPROG_WORLD_DRAW_H

// TODO: Likely multiple splits here.

s32 Map_TypeGet(void);

void Collision_TriggerFlagsLocationUpdate(const s_SubCharacter* chara);

void Collision_TriggerFlagsUpdate(void);

/** @brief Gets the bone nodes of a registered character model.
 *
 * @param charaId ID of the character for which to get the skeleton bone nodes (`e_CharaId`).
 * @return Character model  bone nodes. `NULL` if the character model is unregistered.
 */
s_BoneNode* WorldGfx_CharaModelBoneNodesGet(e_CharaId charaId);

void GameFs_BgEtcGfxLoad(void);

void GameFs_BgItemLoad(void);

/** @brief Gets the speed zone type at a given position.
 *
 * @param posX X position.
 * @param posZ Z position.
 * @return Speed zone type at the given position (`e_SpeedZoneType`).
 */
s32 Map_SpeedZoneTypeGet(q19_12 posX, q19_12 posZ);

/** @brief Initializes world and clear any previously loaded object placement.
 */
void World_Init(void);

/** Something related to player model loading. */
void WorldGfx_HeldItemModelFree(void);

/** Allocates player model? */
void WorldGfx_CharaModelsFree(void);

void Chara_ModelFree(s_CharaModel* model);

void WorldGfx_MapInit(s_MapOverlayHdr* mapHdr, s32 playerPosX, s32 playerPosZ);

/** @unused */
void Map_ActiveChunksClear(void);

void WorldGfx_MapReset(void);

void WorldGfx_IpdSamplePointStore(void);

void WorldGfx_IpdSamplePointReset(void);

/** Sets draw distance, and loads map models.
 * Breaking the function doesn't causes the game to crash, but instead
 * the world won't draw beyond what has been previously loaded. Some
 * circumstances can also cause the player to be unable to move.
 */
void WorldGfx_CloseRangeChunksInit(void);

/** @brief Loads map chunks in view and checks if they are loaded.
 *
 * @param `true` if chunks in view are loaded, `false` otherwise.
 */
bool WorldGfx_ChunksInitCheck(void);

/** @brief Draws world objects, chunks, and 2D screen effects.
 *
 * @param otShift Ordering table shift.
 */
void WorldGfx_Draw(s32 otShift);

/** @brief Sets the name of a world object model.
 *
 * @param model World object model to update.
 * @param name Name to set.
 */
void WorldObject_ModelNameSet(s_WorldObjectModel* model, char* name);

/** Submits a world object model to draw. */
void WorldObjects_Add(s_WorldObjectModel* model, const VECTOR3* pos, const SVECTOR3* rot);

/** @brief Resets the collection of world objects to draw.
 *
 * @param worldGfx World GFX workspace.
 */
void WorldObjects_Clear(s_WorldGfxWork* worldGfxWork);

/** @brief Draws all world objects submitted for the current tick.
 *
 * @param worldGfxWork World GFX workspace.
 */
void WorldObjects_DrawAllObjects(s_WorldGfxWork* worldGfxWork);

/** @brief Draws a world object, taking into account its position relative to the camera.
 *
 * @param obj World object.
 */
void WorldObjects_Draw(s_WorldObject* obj);

void WorldObjects_DrawStep(s_WorldObjectModel* model, MATRIX* viewMat, MATRIX* worldMat);

/** @brief @unused Returns held item ID.
 *
 * @return Held item ID.
 */
s32 WorldGfx_HeldItemIdGet(void);

s32 WorldGfx_PlayerPrevHeldItem(s_PlayerCombat* combat);

/** @brief Loads and sets an item held by the player.
 *
 * @param itemId ID of the held item to load (`e_InvItemId`).
 * @return Model or texture queue index.
 */
s32 WorldGfx_PlayerHeldItemSet(e_InvItemId itemId);

// Make a Harry bone invisible? TODO: Is bone 32 the light?
void func_8003D01C(void);

// Make a Harry bone visible? TODO: Is bone 32 the light?
void func_8003D03C(void);

/** Loads the model of an item held by the player? */
void WorldGfx_HeldItemDraw(void);

/** @brief Loads the Harry character model. */
void WorldGfx_HarryCharaLoad(void);

/** @brief Loads default characters in the map overlay.
 *
 * @param mapHdr Map header.
 * @return Character model queue index.
 */
s32 WorldGfx_MapInitCharaLoad(s_MapOverlayHdr* mapHdr);

/** @brief Advanced a character model LM buffer.
 *
 * @param buf Buffer to advance.
 * @param charaID  ID of the character whose model to use (`e_CharaId`).
 */
void WorldGfx_CharaLmBufferAdvance(u8** buf, e_CharaId charaId);

/** UV setup for character textures. */
void Chara_FsImageCalc(s_FsImageDesc* image, s32 groupIds, s32 modelIdx);

bool WorldGfx_IsCharaModelPresent(e_CharaId charaId);

/** @brief Makes a character transparent. */
void WorldGfx_CharaModelTransparentSet(e_CharaId charaId, bool isTransparent);

/** @brief Sets the material for a character model.
 *
 * @param charaId ID of the character whose model to update.
 * @param blendMode Material blend mode to set (`e_BlendMode`).
 */
void WorldGfx_CharaModelMaterialSet(e_CharaId charaId, s32 blendMode);

void WorldGfx_CharaLmBufferAssign(s8 forceFree);

/** @brief Frees a registered character model.
 *
 * @param model Character model to release.
 */
void WorldGfx_CharaFree(s_CharaModel* model);

/** @brief Loads a character model, setting the LM header and texture UVs as a prerequisite.
 *
 * @param charaId ID of the character whose model to load (`e_CharaId`).
 * @param modelIdx Slot index of the model array to load into.
 * @param lmHdr LM header.
 * @param tex Model texture.
 * @return Model or texture queue index.
 */
void WorldGfx_CharaLoad(e_CharaId charaId, s32 modeIdx, s_LmHeader* lmHdr, s_FsImageDesc* tex);

/** @brief Loads a character model.
 *
 * @param charaId ID of the character whose model to load (`e_CharaId`).
 * @param modelIdx Slot index of the model array to load into.
 * @param lmHdr LM header.
 * @param tex Model texture.
 * @return Model or texture queue index.
 */
s32 WorldGfx_CharaModelLoad(e_CharaId charaId, s32 modelIdx, s_LmHeader* lmHdr, s_FsImageDesc* tex);

/** Something related to animations. */
void WorldGfx_PlayerModelProcessLoad(void);

void WorldGfx_CharaModelProcessAllLoads(void);

void WorldGfx_CharaModelProcessLoad(s_CharaModel* model);

void WorldGfx_CharaDraw(e_CharaId charaId, GsCOORDINATE2* boneCoords, s32 otShift, q3_12 timer, s32 paletteIdx);

/** @brief Gets the Y position of a color palette in VRAM.
 *
 * @param charaId @unused Character ID.
 * @param paletteIdx Color palette index.
 * @return CLUT Y position in VRAM.
 */
s32 WorldGfx_CharaClutYGet(e_CharaId charaId, s32 paletteIdx);

/** @brief Handles a mesh swap for a character.
 *
 * @param charaId ID of the character on which to perform the mesh swap.
 * @param meshSwapStatus Packed mesh swap status. See `MESH_SWAP_STATUS`.
 */
void WorldGfx_CharaMeshSwap(e_CharaId charaId, s32 meshSwapStatus);

/** @brief Handles a mesh swap for a Harry character.
 *
 * @param skel Harry model skeleton.
 * @param meshSwapStatus Packed mesh swap status. See `MESH_SWAP_STATUS`.
 */
void WorldGfx_HarryMeshSwap(s_Skeleton* skel, s32 meshSwapStatus);

/** @brief Handles a mesh swap for a Cybil character.
 *
 * @param skel Cybil model skeleton.
 * @param meshSwapStatus Packed mesh swap status. See `MESH_SWAP_STATUS`.
 */
void WorldGfx_CybilMeshSwap(s_Skeleton* skel, s32 meshSwapStatus);

/** @brief Handles a mesh swap for a Monster Cybil character.
 *
 * @param skel Monster Cybil model skeleton.
 * @param meshSwapStatus Packed mesh swap status. See `MESH_SWAP_STATUS`.
 */
void WorldGfx_MonsterCybilMeshSwap(s_Skeleton* skel, s32 meshSwapStatus);

/** @brief Handles a mesh swap for a Dahlia character.
 *
 * @param skel Dahlia model skeleton.
 * @param meshSwapStatus Packed mesh swap status. See `MESH_SWAP_STATUS`.
 */
void WorldGfx_DahliaMeshSwap(s_Skeleton* skel, s32 meshSwapStatus);

/** @brief Handles a mesh swap for a Kaufmann character.
 *
 * @param skel Kaufmann model skeleton.
 * @param meshSwapStatus Packed mesh swap status. See `MESH_SWAP_STATUS`.
 */
void WorldGfx_KaufmannMeshSwap(s_Skeleton* skel, s32 meshSwapStatus);

/** @brief Handles a mesh swap for a Stalker character.
 *
 * @param skel Stalker model skeleton.
 * @param meshSwapStatus Packed mesh swap status. See `MESH_SWAP_STATUS`.
 */
void WorldGfx_StalkerMeshSwap(s_Skeleton* skel, s32 meshSwapStatus);

/** @brief Handles a mesh swap for a Split Head character.
 *
 * @param skel Split Head model skeleton.
 * @param meshSwapStatus Packed mesh swap status. See `MESH_SWAP_STATUS`.
 */
void WorldGfx_SplitHeadMeshSwap(s_Skeleton* skel, s32 meshSwapStatus);

/** @brief Handles a mesh swap for a Puppet Nurse character.
 *
 * @param skel Puppet Nurse model skeleton.
 * @param meshSwapStatus Packed mesh swap status. See `MESH_SWAP_STATUS`.
 */
void WorldGfx_PuppetNurseMeshSwap(s_Skeleton* skel, s32 meshSwapStatus);

/** @brief Handles a mesh swap for a Puppet Doctor character.
 *
 * @param skel Puppet Doctor model skeleton.
 * @param meshSwapStatus Packed mesh swap status. See `MESH_SWAP_STATUS`.
 */
void WorldGfx_PuppetDoctorMeshSwap(s_Skeleton* skel, s32 meshSwapStatus);

#endif
