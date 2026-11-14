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

/** Anim infos setup for character. */
s32 func_8008A0E4(s32 arg0, s32 weaponAttack, s_SubCharacter* chara, VECTOR3* attackPos, s_SubCharacter* unused,
                  q3_12 rotToTargetX, q3_12 rotToTargetY);

u32 func_8008A270(s32 idx);

u32 func_8008A2E0(s32 arg0);

s32 func_8008A35C(s_AttackInfo* attackInfo, s32 arg1);

void Chara_Flag8Clear(s_SubCharacter* chara);

void Chara_Flag8Set(s_SubCharacter* chara);

/** Large function. Runs if `CharaFlag_Unk8` is set. */
void func_8008A3AC(s_SubCharacter* chara);

s32 func_8008A3E0(s_SubCharacter* chara);

void func_8008B15C(s_SubCharacter* chara);

void func_8008B1DC(s_SubCharacter* chara, q19_12 angle0, q19_12 angle1);

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
