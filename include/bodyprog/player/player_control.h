#ifndef _BODYPROG_PLAYER_PLAYERCONTROL_H
#define _BODYPROG_PLAYER_PLAYERCONTROL_H

#include "bodyprog/player/player.h"

/** @brief Counts the number inputs performed by the user to release an enemy grab. */
extern s32 g_Player_GrabReleaseInputTimer;

extern q3_12 g_Player_FlexRotationY;

extern q3_12 g_Player_FlexRotationX;

/** @brief `bool` | Determines if the player is transitioning from a walk to a run and vice-versa. */
extern u8 g_Player_IsInWalkToRunTransition;

/** `bool` */
extern u8 g_Player_DisableControl;

extern q1_7 g_Player_MoveStickMag;

/** @brief `e_RockDrillAttackType` | Rock Drill attack type based on D-Pad input. */
extern s8 g_Player_RockDrill_AttackType;

/** Another variable that saves the index of the enemy being attacked. */
extern s32 g_Player_TargetNpcIdx;

extern const s_AnimInfo* D_800297B8;

/** @brief Gets the variable animation duration for the player.
 *
 * @param model Player model.
 * @return Variable animation duration.
 */
q19_12 Player_VariableAnimDurationGet(s_Model* model);

void func_80070B84(s_SubCharacter* player, q19_12 baseMoveSpeed, q19_12 topMoveSpeed, s32 keyframeIdx);

/** Used for player run displacement.
 *
 * @param chara Player character.
 */
void func_80070CF0(s_SubCharacter* player, q19_12 arg1, q19_12 moveDistMax, q19_12 moveDistForward, q19_12 moveDistBack);

void func_80070DF0(s_PlayerExtra* extra, s_SubCharacter* player, s32 weaponAttack, s32 animStatus);

void Player_CharaTurn_0(s_SubCharacter* player, e_PlayerLowerBodyState curState);

void Player_CharaTurn_1(s_SubCharacter* player, e_PlayerLowerBodyState curState);

void Player_CharaRotate(s32 speed);

/** @brief Clears movement variables and reset Harry's lower body part animation states in case
 * of switching the player state.
 */
void Player_MovementStateReset(s_SubCharacter* player, e_PlayerLowerBodyState lowerBodyState);

/** Audio-related. */
bool Player_FootstepSfxPlay(s32 animStatus, s_SubCharacter* player, s32 keyframe0, s32 keyframe1, s32 sfx, s8 pitch);

/** Special player SFX handler for heavy breath and damage. */
bool func_80071620(u32 animStatus, s_SubCharacter* player, s32 keyframeIdx, e_SfxId sfxId);

/** @brief Main update function for player functionality.
 *
 * @note SH2 symbols have a similar function named `PlayerFunction`, but code-wise they
 * doesn't share many similarities.
 */
void Player_Update(s_SubCharacter* player, s_AnmHeader* anmHdr, GsCOORDINATE2* coords);

/** Player anim handler? */
void Player_AnimUpdate(s_SubCharacter* player, s_PlayerExtra* extra, s_AnmHeader* anmHdr, GsCOORDINATE2* coords);

/** @brief Main update function for player logic. */
void Player_LogicUpdate(s_SubCharacter* player, s_PlayerExtra* extra, GsCOORDINATE2* coords);

/** @brief Updates `PLAYER_EXTRA().upperBodyState` and prepares
 * transitions between running to and from walking animations.
 *
 * @param extra Pointer to `s_PlayerExtra` struct.
 * @param upperState Check if `PLAYER_EXTRA().upperBodyState` is not the same being input in order to clear animation status.
 * @param unused @unused Possibly animation index.
 * @param arg3 Player turn state. Only affects turn animations.
 * * 0 and 2: Idle state.
 * *       1: Any turn state.
 * *       3: Left turn state.
 * *       4: Right turn state.
 */
void Player_UpperBodyStateUpdate(s_PlayerExtra* extra, e_PlayerUpperBodyState upperState, s32 unused, s32 arg3);

/** @brief Triggers animations and actions performed by the player's upper body. */
void Player_UpperBodyUpdate(s_SubCharacter* player, s_PlayerExtra* extra);

/** @brief Main animation and state handler for the player's upper body.
 * Returns `true` if combat animations where completed or if the player's health is >60%.
 */
bool Player_UpperBodyMainUpdate(s_SubCharacter* player, s_PlayerExtra* extra);

/** @brief Updates upper body states related to combat. */
void Player_CombatStateUpdate(s_SubCharacter* player, s_PlayerExtra* extra);

/** @brief Cancels movement when the player abruptly stops running sideways. */
void Player_StepWallStop_MovementCancel(s_SubCharacter* player, s32 arg1, s32 animStatus, s32 keyframeIdx, e_PlayerLowerBodyState lowerBodyState, s32 headingAngle, s32 aimState);

/** @brief Main player movement function. */
void Player_LowerBodyUpdate(s_SubCharacter* player, s_PlayerExtra* extra);

/** @brief Multiple purposes.
 * * Assigns the value used by a global that handles player movement.
 * * Deals with the exhaustion variable (When Harry has been running for too long).
 * * Restarts the animation state if the player is not longer exhausted or has recovered health.
 * * Handles audio related to Harry's movements.
 */
void func_8007B924(s_SubCharacter* player, s_PlayerExtra* extra);

void Player_PositionUpdate(s_SubCharacter* player, s_PlayerExtra* extra, GsCOORDINATE2* boneCoords);

/** @brief Checks for damage received by the player and handles the animations
 * for getting hurt or grabbed by monsters.
 *
 * @note SH2 symbols have a similar function named `PlayerCheckDamage`, but code-wise it has
 * no similarities.
 */
void Player_ReceiveDamage(s_SubCharacter* player, s_PlayerExtra* extra);

void func_8007D090(s_SubCharacter* player, s_PlayerExtra* extra, GsCOORDINATE2* boneCoords);

void Player_FlexRotationYReset(void);

// TODO: Temporary placement, belongs in `player_control.h` but there's a decl issue. 
s32 Player_LowerBodyMoveStateGet(s_SubCharacter* player, s_800C45C8* arg1);

void Player_CombatUpdate(s_SubCharacter* player, GsCOORDINATE2* boneCoords);

/** Resets player info such as the inventory, health, and playtime in the savegame buffer. */
void Game_SavegameResetPlayer(void);

// TODO: Decl issue, placed in `player.h` for now.
// Returns player lower body state.
//s32 Player_LowerBodyMoveStateGet(s_SubCharacter* player, s_800C45C8* arg1);

void Game_PlayerInfoInit(void);

void func_8007E860(void);

void func_8007E8C0(void);

/** Player func. */
void func_8007E9C4(void);

/** Loads player animations for a given map. Maybe for cutscenes?
 *
 * @param mapIdx Map index.
 */
void GameFs_PlayerMapAnimLoad(s32 mapIdx);

void GameFs_WeaponInfoUpdate(void);

/** Sound func. */
void func_8007F14C(u8 weaponAttack);

void Game_PlayerMovementsReset(void);

void Player_DisableDamage(u8* isPlayerDead, u8 disableDamage);

/** Some kind of player anim state check. */
bool Player_IsAttacking(void);

/** Some kind of player anim state check. */
bool Player_IsBusy(void);

/** Gets something from the player's current animation? */
s16 Player_AnimGetSomething(void);

void Player_Controller(void);

/** @brief Determines if the player can stomp or kick a knocked enemy. */
bool func_8007F95C(void);

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

/** @brief Computes the distance between two positions.
 *
 * @param from First position (Q19.12).
 * @param to Second position (Q19.12).
 * @return Distance between positions.
 */
q19_12 Math_DistanceGet(const VECTOR3* from, const VECTOR3* to);

/** @brief Computes the 2D distance on the XZ plane between two positions.
 *
 * @param from First position (Q19.12).
 * @param to Second position (Q19.12).
 * @return 2D distance between positions.
 */
q19_12 Math_Distance2dGet(const VECTOR3* from, const VECTOR3* to);

void func_800803FC(VECTOR3* pos, s32 idx);

/** @brief Forces a clicked controller input status for `ControllerFlag_Select`. */
void Input_SelectClickSet(void);

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

/** @brief Multiplies two integers in a fixed-point Q format.
 *
 * @param a First fixed-point factor.
 * @param b Second fixed-point factor.
 * @param shift Fixed-point shift.
 * @return Fixed-point product of `a` and `b`.
 */
s32 Math_MulFixed(s32 a, s32 b, s32 shift);

/** Returns a Q shift based on a magnitude. */
s32 Math_MagnitudeShiftGet(q19_12 mag);

#endif
