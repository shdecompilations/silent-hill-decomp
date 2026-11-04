#ifndef _MAPS_CHARACTERS_LARVALSTALKER_H
#define _MAPS_CHARACTERS_LARVALSTALKER_H

/** @brief Larval Stalker character flags. */
typedef enum _LarvalStalkerFlags
{
    LarvalStalkerFlag_None = 0,
    LarvalStalkerFlag_1    = 1 << 1,
    LarvalStalkerFlag_2    = 1 << 2,
    LarvalStalkerFlag_3    = 1 << 3,
    LarvalStalkerFlag_4    = 1 << 4,
    LarvalStalkerFlag_5    = 1 << 5,
    LarvalStalkerFlag_6    = 1 << 6,
    LarvalStalkerFlag_7    = 1 << 7
} e_LarvalStalkerFlags;

/** @brief Larval Stalker character control states. */
typedef enum _LarvalStalkerControl
{
    LarvalStalkerControl_None        = 0,
    LarvalStalkerControl_1           = 1,
    LarvalStalkerControl_2           = 2,
    LarvalStalkerControl_3           = 3,
    LarvalStalkerControl_WalkForward = 4,
    LarvalStalkerControl_Unused5     = 5, /** @unused */
    LarvalStalkerControl_6           = 6,
    LarvalStalkerControl_Idle        = 7,
    LarvalStalkerControl_8           = 8, // Alt walk forward?
    LarvalStalkerControl_TripOver    = 9,
    LarvalStalkerControl_Stun        = 10, // Stun, get back up.
    LarvalStalkerControl_11          = 11,
    LarvalStalkerControl_12          = 12, // Recoil?
    LarvalStalkerControl_Death       = 13
} e_LarvalStalkerControl;

/** @brief Larval Stalker character animation indices. */
typedef enum _LarvalStalkerAnim
{
    LarvalStalkerAnim_Still              = 0,
    LarvalStalkerAnim_GrabAttack         = 1,
    LarvalStalkerAnim_StunBackEnd        = 2,
    LarvalStalkerAnim_DeathBackStart     = 3,
    LarvalStalkerAnim_DeathFrontStart    = 4,
    LarvalStalkerAnim_StunBackToIdle     = 5,
    LarvalStalkerAnim_StunFrontToIdle    = 6,
    LarvalStalkerAnim_StunBackStart      = 7,
    LarvalStalkerAnim_StunFrontStart     = 8,
    LarvalStalkerAnim_StunFrontEnd       = 9,
    LarvalStalkerAnim_Idle               = 10,
    LarvalStalkerAnim_WalkForward        = 11,
    LarvalStalkerAnim_WalkForwardAlt     = 12, /** Duplicate of `LarvalStalkerAnim_WalkForward`. */
    LarvalStalkerAnim_DeathFrontEnd      = 13,
    LarvalStalkerAnim_DeathBackCont      = 14, /** Bugged/unfinished death transition? */
    LarvalStalkerAnim_StunBackToIdleAlt  = 15, /** Duplicate of `LarvalStalkerAnim_StunBackToIdle`. */
    LarvalStalkerAnim_StunFrontToIdleAlt = 16, /** Duplicate of `LarvalStalkerAnim_StunFrontToIdle`. */
    LarvalStalkerAnim_StunFrontCont      = 17,
    LarvalStalkerAnim_StunBackRecoil     = 18
} e_LarvalStalkerAnim;

extern s_AnimInfo LARVAL_STALKER_ANIM_INFOS[];

void LarvalStalker_Update(s_SubCharacter* larvalStalker, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

void LarvalStalker_ControlUpdate(s_SubCharacter* larvalStalker);

void sharedFunc_800D140C_1_s00(s_SubCharacter* larvalStalker, GsCOORDINATE2* boneCoords);

void sharedFunc_800D1524_1_s00(s_SubCharacter* larvalStalker, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

void LarvalStalker_Init(s_SubCharacter* larvalStalker);

void sharedFunc_800D17BC_1_s00(s_SubCharacter* larvalStalker);

void LarvalStalker_CollisionUpdate(s_SubCharacter* larvalStalker);

#endif
