#ifndef _MAPS_CHARACTERS_CREEPER_H
#define _MAPS_CHARACTERS_CREEPER_H

/** @brief Creeper character flags. */
typedef enum _CreeperFlags
{
    CreeperFlag_None         = 0,
    CreeperFlag_HasAttacked  = 1 << 0,
    CreeperFlag_1            = 1 << 1, // Damaged? Doesn't get set back.
    CreeperFlag_Falling      = 1 << 2,
    CreeperFlag_3            = 1 << 3, // Doesn't get set back. Recoiling? Hit?
    CreeperFlag_Leader       = 1 << 4,
    CreeperFlag_TargetPlayer = 1 << 5,
    CreeperFlag_6            = 1 << 6, // Stop pathing?
    CreeperFlag_7            = 1 << 7,
    CreeperFlag_Alerted      = 1 << 8
} e_CreeperFlags;

/** @brief Creeper character control states. */
typedef enum _CreeperControl
{
    CreeperControl_None        = 0,
    CreeperControl_Idle        = 1,
    CreeperControl_WalkForward = 2,
    CreeperControl_Attack      = 3,
    CreeperControl_Stun        = 4,
    CreeperControl_Damage      = 5
} e_CreeperControl;

/** @brief Creeper character animation indices. */
typedef enum _CreeperAnim
{
    CreeperAnim_Still               = 0,
    CreeperAnim_AttackToWalkForward = 1,
    CreeperAnim_Attack              = 2,
    CreeperAnim_DeathStart          = 3,
    CreeperAnim_RunForwardStunCont  = 4, // Unsure.
    CreeperAnim_Recoil              = 5,
    CreeperAnim_Unused6             = 6, /** @unused Alternative idle or attack? */
    CreeperAnim_StunLoop            = 7,
    CreeperAnim_IdleToRunForward    = 8,
    CreeperAnim_Stun                = 9,
    CreeperAnim_RunForward          = 10,
    CreeperAnim_Idle                = 11,
    CreeperAnim_AttackStart         = 12,
    CreeperAnim_WalkForward         = 13,
    CreeperAnim_DeathEnd            = 14,
    CreeperAnim_RunForwardStunEnd   = 15, // Unsure.
    CreeperAnim_StunOnce            = 16,
    CreeperAnim_RunForwardStunStart = 17  // Unsure.
} e_CreeperAnim;

extern s_AnimInfo CREEPER_ANIM_INFOS[];

void Creeper_Update(s_SubCharacter* creeper, s_AnmHeader* anmHdr, GsCOORDINATE2* coords);

void Creeper_Init(s_SubCharacter* creeper); // `map5_s00` version has extra code at end of func.

void sharedFunc_800D7EE8_1_s02(s_SubCharacter* creeper);

void Creeper_ControlUpdate(s_SubCharacter* creeper);

void Creeper_ControlIdle(s_SubCharacter* creeper);

void Creeper_ControlWalkForward(s_SubCharacter* creeper);

void Creeper_ControlAttack(s_SubCharacter* creeper);

void Creeper_ControlStun(s_SubCharacter* creeper);

void Creeper_ControlDamage(s_SubCharacter* creeper);

void sharedFunc_800D983C_1_s02(s_SubCharacter* creeper);

void Creeper_AnimUpdate(s_SubCharacter* creeper, s_AnmHeader* anmHdr, GsCOORDINATE2* coords);

void Creeper_CollisionUpdate(s_SubCharacter* creeper);

#endif
