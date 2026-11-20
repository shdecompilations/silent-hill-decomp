#ifndef _BODYPROG_EVENTS_NPCMAIN_H
#define _BODYPROG_EVENTS_NPCMAIN_H

/** @brief Marks an enemy character as incapable of respawning.
 * The game difficulty must be set to easy or normal. On higher difficulties, the flag has a 0.6 probability of
 * being set.
 *
 * @param chara Character to mark as incapable of respawning.
 */
void Savegame_EnemyStateUpdate(s_SubCharacter* chara);

/** @brief Updates character's damage flag to reflect if damage was taken.
 *
 * @param chara Character to update.
 */
void Chara_DamagedFlagUpdate(s_SubCharacter* chara);

void func_80037E78(s_SubCharacter* chara);

/** Responsible for loading NPCs on the map. */
void Game_NpcRoomInitSpawn(bool cond);

/** @brief Main NPC update function. Runs through each NPC and calls `g_MapOverlayHdr.charaUpdateFuncs` for them. */
void Game_NpcUpdate(void);

/** @brief Performs a 2D distance check on the XZ plane between two positions.
 *
 * @param from First position (Q19.12).
 * @param to Second position (Q19.12).
 * @param radius Intersection radius.
 * @return `true` if the 2D distance exceeds the radius, `false` otherwise.
 */
bool Math_Distance2dCheck(const VECTOR3* from, const VECTOR3* to, q19_12 radius);

#endif
