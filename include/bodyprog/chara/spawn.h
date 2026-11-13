#ifndef _BODYPROG_CHARA_SPAWN_H
#define _BODYPROG_CHARA_SPAWN_H

#include "bodyprog/chara/chara.h"
#include "bodyprog/formats/lm.h"
#include "main/fsqueue.h"

/** @brief Character spawn flags. */
typedef enum _CharaSpawnFlags
{
    CharaSpawnFlag_None = 0,
    CharaSpawnFlag_0    = 1 << 0,
    CharaSpawnFlag_1    = 1 << 1,
    CharaSpawnFlag_2    = 1 << 2,
    CharaSpawnFlag_3    = 1 << 3,
    CharaSpawnFlag_4    = 1 << 4
} e_CharaSpawnFlags;

/** @brief Loads a character model.
 *
 * @param modelIdx Slot index of the moddel array to load into.
 * @param charaId ID of the character model to load.
 * @param boneCoords Character model bone coords.
 * @param forceFree Force an overwrite if the model index is taken.
 * @param lmHdr LM file header.
 * @param tex Model texture.
 */
bool Chara_Load(s32 modelIdx, s8 charaId, GsCOORDINATE2* boneCoords, s8 forceFree,
                s_LmHeader* lmHdr, s_FsImageDesc* tex);

/** @brief Processes character model loads. */
bool Chara_ProcessLoads(void);

/** @brief Initializes a character's animated bone coords.
 *
 * @param modelAnimsIdx Index into `g_CharaModelAnimsData`.
 */
void Chara_BonesInit(s32 modelAnimsIdx);

/** @brief Spawns a new NPC in the world.
 *
 * @param charaId ID of the character to spawn.
 * @param spawnFlags Spawn flags.
 * @param posX X spawn position.
 * @param posX Z spawn position.
 * @param rotY Y spawn rotation.
 * @param stateStep Character model state step to set.
 * @return @unused Spawned NPC slot index.
 */
s32 Chara_Spawn(e_CharaId charaId, s32 spawnFlags, q19_12 posX, q19_12 posZ, q3_12 rotY, u32 stateStep);

/** @brief Clears character ID in a character model.
 *
 * @param chara Character to update.
 * @param unused0 @unused
 * @param unused1 @unused
 */
void Chara_ModelCharaIdClear(s_SubCharacter* chara, s32 unused0, s32 unused1);

/** @brief Sets the spawn flags for a character ID.
 *
 * @param charaId ID of the character spawn to update.
 * @param spawnIdx Spawn index.
 * @param spawnFlags Spawn flags to set (`e_SpawnFlags`).
 */
void Chara_SpawnFlagsSet(e_CharaId charaId, s32 spawnIdx, s32 spawnFlags);

/** @brief Sets a character's spawn position.
 *
 * @param charaId ID of the character spawn to update.
 * @param spawnIdx Spawn index.
 * @param posX X position. 
 * @param posZ Z position. 
 */
void Chara_SpawnPositionSet(e_CharaId charaId, s32 spawnIdx, q19_12 posX, q19_12 posZ);

#endif
