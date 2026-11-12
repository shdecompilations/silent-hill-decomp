#ifndef _BODYPROG_PLAYER_PLAYERCONTROL_H
#define _BODYPROG_PLAYER_PLAYERCONTROL_H

/** @brief Gets the variable animation duration for the player.
 *
 * @param model Player model.
 * @return Variable animation duration.
 */
q19_12 Player_VariableAnimDurationGet(s_Model* model);

/** Special player SFX handler for heavy breath and damage. */
bool func_80071620(u32 animStatus, s_SubCharacter* player, s32 keyframeIdx, e_SfxId sfxId);

void Player_PositionUpdate(s_SubCharacter* player, s_PlayerExtra* extra, GsCOORDINATE2* boneCoords);

void func_8007D090(s_SubCharacter* player, s_PlayerExtra* extra, GsCOORDINATE2* boneCoords);

void Player_CombatUpdate(s_SubCharacter* player, GsCOORDINATE2* boneCoords);

/** Player func. */
void func_8007E9C4(void);

/** Sound func. */
void func_8007F14C(u8 weaponAttack);

/** Resets several global variables to 0. */
void func_8007F1CC(void);

void Player_DisableDamage(u8* isPlayerDead, u8 disableDamage);

/** Some kind of player anim state check. */
bool Player_IsAttacking(void);

/** Some kind of player anim state check. */
bool Player_IsBusy(void);

/** Gets something from the player's current animation? */
s16 Player_AnimGetSomething(void);

/** @brief Computes the shortest angle between `angleFrom` and `angleTo`, outputting the result to `shortestAngle`.
 *
 * @param angleFrom First angle in Q3.12, range `[0, 4096]`.
 * @param angleTo Second angle in Q3.12, range `[0, 4096]`.
 * @param shortestAngle Shortest angle output in Q3.12, range `[0, 4096]`.
 */
void Math_ShortestAngleGet(q3_12 angleFrom, q3_12 angleTo, q3_12* shortestAngle);

/** Anim func.
 *
 * @param chara Player character.
 * @param extra Extra player character parameters.
 * @param animStatus Packed anim status.
 */
void func_8007FB94(s_SubCharacter* chara, s_PlayerExtra* extra, s32 animStatus);

/** Anim func.
 *
 * @param chara Player character.
 * @param extra Extra player character parameters.
 * @param animStatus Packed anim status.
 */
void func_8007FC48(s_SubCharacter* chara, s_PlayerExtra* extra, s32 animStatus);

/** Gets property 8 from player. */
s32 func_8007FD2C(void);

/** @unused Gets current value from the power timer for gas based weapons.
 *
 * @return Power timer.
 */
q19_12 Game_GasWeaponPowerTimerValue(void);

void func_8007FD4C(bool resetColl);

/** @brief Gets a player footstep SFX ID according to the floor type and its pitch.
 *
 * @param floorType Floor type.
 * @param sfxId Output footstep SFX ID.
 * @param pitch0 Output TODO
 * @param pitch1 Output TODO
 * @return Footstep SFX ID.
 */
void Player_FootstepSfxGet(s8 floorType, e_SfxId* sfxId, s8* pitch0, s8* pitch1);

/** Presumably, returns a heading angle from `from` to `to`. If so, rename to `Math_AngleBetweenPositionsGet`. */
q19_12 func_80080478(const VECTOR3* from, const VECTOR3* to);

/** @brief Generates a random normalized Q19.12 fixed-point number, integer range `[0, 4096)`.
 * Used for angles and multipliers.
 *
 * @return Random Q19.12 fixed-point number.
 */
q19_12 Rng_RandQ12(void);

// Math with XYZ values.
s32 func_80080540(q19_12 x, q19_12 y, q19_12 z);

/** Computes (abs(value) - subtractor) * copysign(value). */
s32 Math_PreservedSignSubtract(s32 val, s32 subtractor);

void func_800805BC(VECTOR3* pos, SVECTOR* rot, GsCOORDINATE2* rootCoord, s32 arg3);

/** Called by `func_8008074C` which is @unused. */
bool func_800806AC(s32 arg0, q19_12 posX, q19_12 posY, q19_12 posZ);

/** @unused */
bool func_8008074C(s32 arg0, q19_12 posX, q19_12 posY, q19_12 posZ);

/** @brief Fills `g_CollisionPointCache` with collision data at a given 2D position.
 *
 * @param posX X position.
 * @param posZ Z position.
 */
void Collision_PointCacheSet(q19_12 posX, q19_12 posZ);

/** @brief Gets the floor height at a given 2D position.
 *
 * @param posX X position.
 * @param posZ Z position.
 * @return Floor height.
 */
q19_12 Collision_FloorHeightGet(q19_12 posX, q19_12 posZ);

/** @brief Gets the floor type at a given 2D position.
 *
 * @param posX X position.
 * @param posZ Z position.
 * @return Floor type (`e_FloorType`).
 */
s32 Collision_FloorTypeGet(q19_12 posX, q19_12 posZ);

/** Returns a Q shift based on a magnitude. */
s32 Math_MagnitudeShiftGet(q19_12 mag);

#endif
