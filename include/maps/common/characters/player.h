#ifndef _MAPS_CHARACTERS_PLAYER_H
#define _MAPS_CHARACTERS_PLAYER_H

extern q19_12 g_Player_MoveSpeed; // `sharedFunc_800CDAA8_0_s02`

void sharedFunc_800CDAA8_0_s02(s_SubCharacter* player, s_PlayerExtra* extra, GsCOORDINATE2* boneCoords);

void sharedFunc_800D1C38_0_s00(s_SubCharacter* player, s_PlayerExtra* extra, GsCOORDINATE2* boneCoords);

void Player_CutsceneWeaponUnequip(void);

void Player_MatchArmAnimDisable(void);

void Player_ControlFreeze(void);

void Player_ControlUnfreeze(bool setIdle);

bool Player_PathWaypointExecute(s32 playerExtraState, VECTOR3* vec, q3_12 angle, s32 vecCount);

void Player_AnimStateSet(s32 playerExtraState);

// Requests a player animation reset. TODO: Unsure about the name.
void Player_AnimReset(void);

void Player_AnimLock(void);

s32 Player_AnimIsLocked(void);

void Player_AnimUnlock(void);

/** Player anim func. Checks if the active keyframe is at the start or end of the anim's range. */
s32 Player_AnimPlaybackStateGet(void);

/** Triggers at specific points after the start of a cutscene or dialogue. */
bool Player_MoveSpeedIsZero(void);

void Player_MoveSpeedClear(void);

// High-level held item handler for player?
void sharedFunc_800D2E6C_0_s00(void);

void Player_FallBackward(void);

void Player_Stub_F8(void);

void Player_DamageFeetFront(void);

void sharedFunc_800D2E8C_0_s00(q19_12 posX, q19_12 posZ, VECTOR3* vec);

// Handle damage? Lots of hardcoded character cases.
bool sharedFunc_800D2E94_0_s00(void);

// Something with a player collision offset.
void sharedFunc_800D2E9C_0_s00(q19_12* offsetX, q19_12* offsetZ, q3_12* angle);

s32 Player_PropertyField10DGet(void);

void Player_EmptyWeaponHandSet(void);

void Player_WeaponAttackRestore(void);

#endif
