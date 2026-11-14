#ifndef _BODYPROG_CHARA_COMBAT_H
#define _BODYPROG_CHARA_COMBAT_H

#include "bodyprog/chara/chara.h"
#include "bodyprog/items/weapon_data.h"

// Q12 square root?
u32 func_8008A058(s32 arg0);

/** @brief @unused Returns 0. */
s32 func_8008A0CC(void);

/** @brief @unused Returns 0. */
s64 func_8008A0D4(void);

/** @brief Starts, continues or cancels an attack and runs its hit checks for this tick.
 *
 * @param request `> 0` starts the attack if idle, `<= 0` cancels.
 * @return Player: `NPC_COUNT_MAX - (index of the lowest NPC hit)`, `NO_VALUE` if none.
 *         Others: 0 if the player has an `attackReceived` set, otherwise `NO_VALUE`.
 */
s32 func_8008A0E4(s32 request, s32 weaponAttack, s_SubCharacter* chara, VECTOR3* attackPos, s_SubCharacter* unused,
                  q3_12 aimYaw, q3_12 aimPitch);

/** @brief Gets the first keyframe of an attack's hit window. */
u32 func_8008A270(s32 weaponAttack);

/** @brief Gets the keyframe count of an attack's hit window. */
u32 func_8008A2E0(s32 weaponAttack);

/** @brief Checks if `keyframe` is inside an attack's hit window. */
s32 func_8008A35C(s_AttackInfo* attackInfo, s32 keyframe);

void Chara_Flag8Clear(s_SubCharacter* chara);

void Chara_Flag8Set(s_SubCharacter* chara);

/** Large function. Runs if `CharaFlag_Unk8` is set. */
void func_8008A3AC(s_SubCharacter* chara);

s32 func_8008A3E0(s_SubCharacter* chara);

void func_8008B15C(s_SubCharacter* chara);

void func_8008B1DC(s_SubCharacter* chara, q19_12 aimYaw, q19_12 aimPitch);

void func_8008B398(void);

void func_8008B3E4(q23_8 vol);

void func_8008B40C(q23_8 vol, s32 soundType);

/** `arg0` is boolean. */
void func_8008B438(s32 arg0, q23_8 vol, s32 soundType);

bool func_8008B474(s32 arg0, q23_8 vol, s32 soundType);

void func_8008B664(VECTOR3* pos, u32 caseVar);

/** Main attack handling function. */
s32 func_8008B714(s_SubCharacter* attacker, s_SubCharacter* target, VECTOR3* hitPos, q19_12 damageScale);

s32 func_8008BF84(s_SubCharacter* chara, q19_12 angle, s_AttackInfo* attackInfo, s32 arg3);

#endif
