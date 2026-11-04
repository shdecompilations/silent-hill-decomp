#ifndef _BODYPROG_PLAYER_H
#define _BODYPROG_PLAYER_H

/** @note The game handles the upper and lower part of the player's body independently.
 *
 * SH2 symbols indicate a similar system with 4 enums used depending on the control scheme:
 * `lower_*_2D`
 * `lower_*_3D`
 * `upper_*_2D`
 * `upper_*_3D`
 */

// ==========
// CONSTANTS
// ==========

#define DEFAULT_PLAYER_BOX_TOP               Q12(-1.6f)
#define DEFAULT_PLAYER_BOX_OFFSET_Y          Q12(-1.1f)
#define DEFAULT_PLAYER_CYLINDER_INNER_RADIUS Q12(0.23f)

#define HARRY_UPPER_BODY_BONE_MASK BITMASK_RANGE(HarryBone_Root, HarryBone_RightHand)
#define HARRY_LOWER_BODY_BONE_MASK BITMASK_RANGE(HarryBone_Hips, HarryBone_RightFoot)

// ======
// ENUMS
// ======

/** @brief Player control flags. */
typedef enum _PlayerFlags
{
    PlayerFlag_None           = 0,
    PlayerFlag_Unk0           = 1 << 0,
    PlayerFlag_Shooting       = 1 << 1,
    PlayerFlag_Unk2           = 1 << 2,
    PlayerFlag_WallStopRight  = 1 << 3, /** Use right leg for wall stop. */
    PlayerFlag_Unk4           = 1 << 4,
    PlayerFlag_Unk5           = 1 << 5, // `PlayerFlag_MoveBackward`?
    PlayerFlag_Unk6           = 1 << 6,
    PlayerFlag_Unk7           = 1 << 7,
    PlayerFlag_Unk8           = 1 << 8,
    PlayerFlag_Unk9           = 1 << 9,
    PlayerFlag_Unk10          = 1 << 10, // `PlayerFlag_MeleeAttack`?
    PlayerFlag_Unk11          = 1 << 11, // `PlayerFlag_GunAttack`?
    PlayerFlag_Unk12          = 1 << 12,
    PlayerFlag_SfxActive      = 1 << 13,
    PlayerFlag_DamageReceived = 1 << 14,
    PlayerFlag_Moving         = 1 << 15,
    PlayerFlag_Unk16          = 1 << 16,
    PlayerFlag_Unk17          = 1 << 17,
    PlayerFlag_Unk18          = 1 << 18,
    PlayerFlag_Unk19          = 1 << 19,
    PlayerFlag_Unk20          = 1 << 20,

    PlayerFlag_Unk28          = 1 << 28,
    PlayerFlag_Unk29          = 1 << 29,
    PlayerFlag_Unk30          = 1 << 30,
    PlayerFlag_Unk31          = 1 << 31
} e_PlayerFlags;

typedef enum _PlayerStopFlags
{
    PlayerStopFlag_None        = 0,
    PlayerStopFlag_StopWalking = 1 << 0,
    PlayerStopFlag_StopRunning = 1 << 1
} e_PlayerStopFlags;

/** @brief Player states. Used by `s_PlayerExtra::state`. */
typedef enum _PlayerState
{
    PlayerState_None                      = 0,
    PlayerState_Combat                    = 1,
    PlayerState_Idle                      = 2,
    PlayerState_FallForward               = 3,
    PlayerState_FallBackward              = 4,
    PlayerState_KickEnemy                 = 5,
    PlayerState_StompEnemy                = 6,
    PlayerState_Unk7                      = 7, // Lighting bolts at the end of the game?
    PlayerState_Death                     = 8,
    PlayerState_InstantDeath              = 9, // Split Head boss.
    PlayerState_EnemyGrabTorsoFront       = 10,
    PlayerState_Unk11                     = 11,
    PlayerState_Unk12                     = 12,
    PlayerState_EnemyGrabTorsoBack        = 13,
    PlayerState_EnemyGrabLegsFront        = 14,
    PlayerState_EnemyGrabLegsBack         = 15,
    PlayerState_EnemyReleaseUpperFront    = 16,
    PlayerState_Unk17                     = 17,
    PlayerState_Unk18                     = 18,
    PlayerState_DamageHead                = 19,
    PlayerState_EnemyReleaseUpperBack     = 20,
    PlayerState_EnemyReleaseLowerFront    = 21,
    PlayerState_EnemyReleaseLowerBack     = 22,
    PlayerState_DamageTorsoBack           = 23,
    PlayerState_DamageTorsoFront          = 24,
    PlayerState_DamageTorsoRight          = 25,
    PlayerState_DamageTorsoLeft           = 26,
    PlayerState_DamageFeetFront           = 27, // Creeper attack.
    PlayerState_DamageFeetBack            = 28, // Creeper attack.
    PlayerState_DamagePushBack            = 29, // Split Head boss.
    PlayerState_DamagePushFront           = 30, // Split Head boss.
    PlayerState_Unk31                     = 31,
    PlayerState_EnemyGrabNeckFront        = 32, // Cybil boss.
    PlayerState_EnemyGrabNeckBack         = 33, // Cybil boss.
    PlayerState_Unk34                     = 34,
    PlayerState_Unk35                     = 35,
    PlayerState_Unk36                     = 36,
    PlayerState_EnemyGrabPinnedFrontStart = 37, // Romper attack.
    PlayerState_EnemyGrabPinnedBackStart  = 38, // Romper attack.
    PlayerState_EnemyGrabPinnedFront      = 39, // Romper attack.
    PlayerState_EnemyGrabPinnedBack       = 40, // Romper attack.
    PlayerState_EnemyReleasePinnedFront   = 41, // Romper attack.
    PlayerState_EnemyReleasePinnedBack    = 42, // Romper attack.
    PlayerState_Unk43                     = 43,
    PlayerState_Unk44                     = 44,
    PlayerState_DamageThrownFront         = 45,
    PlayerState_DamageThrownBack          = 46,
    PlayerState_OnFloorFront              = 47, // Counts as grab anim.
    PlayerState_OnFloorBehind             = 48, // Counts as grab anim.
    PlayerState_GetUpFront                = 49,
    PlayerState_GetUpBack                 = 50,
    PlayerState_Unk51                     = 51, // Cutscenes
    PlayerState_Reset                     = 52, // Unsure, maybe a forced stop. Seems to reset the animation to a standing pose.
    PlayerState_WalkForward               = 53, // Cutscene-only?
    PlayerState_RunForward                = 54, // Cutscene-only?
    PlayerState_WalkBackward              = 55, // Cutscene-only? Possibly @unused
    PlayerState_TurnRight                 = 56,
    PlayerState_TurnLeft                  = 57,
    PlayerState_Unk58                     = 58,
    PlayerState_Unk59                     = 59, // Crouching in cutscenes/interactions?
    PlayerState_Unk60                     = 60, // Get up from crouching in cutscenes/interactions?
    PlayerState_Unk61                     = 61,
    PlayerState_Unk62                     = 62,
    PlayerState_Unk63                     = 63,
    PlayerState_Unk64                     = 64,
    PlayerState_Unk65                     = 65,
    PlayerState_Unk66                     = 66,
    PlayerState_Unk67                     = 67,
    PlayerState_Unk68                     = 68,
    PlayerState_Unk69                     = 69,
    PlayerState_Unk70                     = 70,
    PlayerState_Unk71                     = 71,
    PlayerState_Unk72                     = 72,
    PlayerState_Unk73                     = 73,
    PlayerState_Unk74                     = 74,
    PlayerState_Unk75                     = 75,
    PlayerState_Unk76                     = 76,
    PlayerState_Unk77                     = 77,
    PlayerState_Unk78                     = 78,
    PlayerState_Unk79                     = 79,
    PlayerState_Unk80                     = 80,
    PlayerState_Unk81                     = 81,
    PlayerState_Unk82                     = 82,
    PlayerState_Unk83                     = 83,
    PlayerState_Unk84                     = 84,
    PlayerState_Unk85                     = 85,
    PlayerState_Unk86                     = 86,
    PlayerState_Unk87                     = 87,
    PlayerState_Unk88                     = 88,
    PlayerState_Unk89                     = 89,
    PlayerState_Unk90                     = 90,
    PlayerState_Unk91                     = 91,
    PlayerState_Unk92                     = 92,
    PlayerState_Unk93                     = 93,
    PlayerState_Unk94                     = 94,
    PlayerState_Unk95                     = 95,
    PlayerState_Unk96                     = 96,
    PlayerState_Unk97                     = 97,
    PlayerState_Unk98                     = 98,
    PlayerState_Unk99                     = 99,
    PlayerState_Unk100                    = 100,
    PlayerState_Unk101                    = 101,
    PlayerState_Unk102                    = 102,
    PlayerState_Unk103                    = 103,
    PlayerState_Unk104                    = 104,
    PlayerState_Unk105                    = 105,
    PlayerState_Unk106                    = 106,
    PlayerState_Unk107                    = 107,
    PlayerState_Unk108                    = 108,
    PlayerState_Unk109                    = 109,
    PlayerState_Unk110                    = 110,
    PlayerState_Unk111                    = 111,
    PlayerState_Unk112                    = 112,
    PlayerState_Unk113                    = 113,
    PlayerState_Unk114                    = 114,
    PlayerState_Unk115                    = 115,
    PlayerState_Unk116                    = 116,
    PlayerState_Unk117                    = 117,
    PlayerState_Unk118                    = 118,
    PlayerState_Unk119                    = 119,
    PlayerState_Unk120                    = 120,
    PlayerState_Unk121                    = 121,
    PlayerState_Unk122                    = 122,
    PlayerState_Unk123                    = 123,
    PlayerState_Unk124                    = 124,
    PlayerState_Unk125                    = 125,
    PlayerState_Unk126                    = 126,
    PlayerState_Unk127                    = 127,
    PlayerState_Unk128                    = 128,
    PlayerState_Unk129                    = 129,
    PlayerState_Unk130                    = 130,
    PlayerState_Unk131                    = 131,
    PlayerState_Unk132                    = 132,
    PlayerState_Unk133                    = 133,
    PlayerState_Unk134                    = 134,
    PlayerState_Unk135                    = 135,
    PlayerState_Unk136                    = 136,
    PlayerState_Unk137                    = 137,
    PlayerState_Unk138                    = 138,
    PlayerState_Unk139                    = 139,
    PlayerState_Unk140                    = 140,
    PlayerState_Unk141                    = 141,
    PlayerState_Unk142                    = 142,
    PlayerState_Unk143                    = 143,
    PlayerState_Unk144                    = 144,
    PlayerState_Unk145                    = 145,
    PlayerState_Unk146                    = 146,
    PlayerState_Unk147                    = 147,
    PlayerState_Unk148                    = 148,
    PlayerState_Unk149                    = 149,
    PlayerState_Unk150                    = 150,
    PlayerState_Unk151                    = 151,
    PlayerState_Unk152                    = 152,
    PlayerState_Unk153                    = 153,
    PlayerState_Unk154                    = 154,
    PlayerState_Unk155                    = 155,
    PlayerState_Unk156                    = 156,
    PlayerState_Unk157                    = 157,
    PlayerState_Unk158                    = 158,
    PlayerState_Unk159                    = 159,
    PlayerState_Unk160                    = 160,
    PlayerState_Unk161                    = 161,
    PlayerState_Unk162                    = 162,
    PlayerState_Unk163                    = 163,
    PlayerState_Unk164                    = 164,
    PlayerState_Unk165                    = 165,
    PlayerState_Unk166                    = 166,
    PlayerState_Unk167                    = 167,
    PlayerState_Unk168                    = 168,
    PlayerState_Unk169                    = 169,
    PlayerState_Unk170                    = 170,
    PlayerState_Unk171                    = 171,
    PlayerState_Unk172                    = 172,
    PlayerState_Unk173                    = 173,
    PlayerState_Unk174                    = 174,
    PlayerState_Unk175                    = 175,
    PlayerState_Unk176                    = 176,
    PlayerState_Unk177                    = 177,
    PlayerState_Unk178                    = 178,
    PlayerState_Unk179                    = 179,
    PlayerState_Unk180                    = 180,
    PlayerState_Unk181                    = 181,
    PlayerState_Unk182                    = 182,
    PlayerState_Unk183                    = 183,
    PlayerState_Unk184                    = 184,
    PlayerState_Unk185                    = 185,
    PlayerState_Unk186                    = 186,
    PlayerState_Unk187                    = 187,
    PlayerState_Unk188                    = 188,
    PlayerState_Unk189                    = 189
} e_PlayerState;

/** @brief Upper body player states. Used by `s_PlayerExtra::upperBodyState`. */
typedef enum _PlayerUpperBodyState
{
    PlayerUpperBodyState_None                 = 0,
    PlayerUpperBodyState_WalkForward          = 1,
    PlayerUpperBodyState_RunForward           = 2,
    PlayerUpperBodyState_RunWallStop          = 3,
    PlayerUpperBodyState_SidestepRight        = 4,
    PlayerUpperBodyState_SidestepLeft         = 5,
    PlayerUpperBodyState_RunRight             = 6,
    PlayerUpperBodyState_RunLeft              = 7,
    PlayerUpperBodyState_WalkBackward         = 8,
    PlayerUpperBodyState_QuickTurnRight       = 9,
    PlayerUpperBodyState_QuickTurnLeft        = 10,
    PlayerUpperBodyState_TurnRight            = 11,
    PlayerUpperBodyState_TurnLeft             = 12,
    PlayerUpperBodyState_RunJumpBackward      = 13,
    PlayerUpperBodyState_LowerBodyStumble     = 14,
    PlayerUpperBodyState_RunLeftWallStop      = 15,
    PlayerUpperBodyState_RunRightWallStop     = 16,
    PlayerUpperBodyState_RunLeftStumble       = 17,
    PlayerUpperBodyState_SidestepRightStumble = 18,
    PlayerUpperBodyState_Aim                  = 19,
    PlayerUpperBodyState_AimTargetLock        = 20,
    PlayerUpperBodyState_AimStart             = 21,
    PlayerUpperBodyState_AimStartTargetLock   = 22,
    PlayerUpperBodyState_AimTargetLockSwitch  = 23,
    PlayerUpperBodyState_AimStop              = 24,
    PlayerUpperBodyState_Attack               = 25,
    PlayerUpperBodyState_Reload               = 26
} e_PlayerUpperBodyState;

/** @brief Lower body player states. Used by `s_PlayerExtra::lowerBodyState`. */
typedef enum _PlayerLowerBodyState
{
    PlayerLowerBodyState_None               = 0,
    PlayerLowerBodyState_WalkForward        = 1,
    PlayerLowerBodyState_RunForward         = 2,
    PlayerLowerBodyState_RunForwardWallStop = 3,
    PlayerLowerBodyState_WalkBackward       = 4,
    PlayerLowerBodyState_SidestepRight      = 5,
    PlayerLowerBodyState_SidestepLeft       = 6,
    PlayerLowerBodyState_RunRight           = 7, // } TODO: Check. Could be swapped.
    PlayerLowerBodyState_RunLeft            = 8, // }
    PlayerLowerBodyState_QuickTurnRight     = 9,
    PlayerLowerBodyState_QuickTurnLeft      = 10,
    PlayerLowerBodyState_JumpBackward       = 11,
    PlayerLowerBodyState_Stumble            = 12,
    PlayerLowerBodyState_RunLeftWallStop    = 13,
    PlayerLowerBodyState_RunRightWallStop   = 14,
    PlayerLowerBodyState_RunRightStumble    = 16,
    PlayerLowerBodyState_RunLeftStumble     = 15,
    PlayerLowerBodyState_Unk17              = 17,
    PlayerLowerBodyState_Unk18              = 18,
    PlayerLowerBodyState_Unk19              = 19,
    PlayerLowerBodyState_Aim                = 20,
    PlayerLowerBodyState_AimWalkForward     = 21,
    PlayerLowerBodyState_Unk22              = 22, // @unused `PlayerLowerBodyState_AimRunForward`?
    PlayerLowerBodyState_Unk23              = 23, // @unused `PlayerLowerBodyState_AimRunWallStop`?
    PlayerLowerBodyState_AimWalkBackward    = 24,
    PlayerLowerBodyState_AimSidestepRight   = 25,
    PlayerLowerBodyState_AimSidestepLeft    = 26,
    PlayerLowerBodyState_AimRunRight        = 27, // @unused
    PlayerLowerBodyState_AimRunLeft         = 28, // @unused
    PlayerLowerBodyState_AimQuickTurnRight  = 29,
    PlayerLowerBodyState_AimQuickTurnLeft   = 30,
    PlayerLowerBodyState_Unk31              = 31, // Something backward?
    PlayerLowerBodyState_Unk32              = 32,
    PlayerLowerBodyState_Unk33              = 33,
    PlayerLowerBodyState_Attack             = 34,
    PlayerLowerBodyState_Reload             = 35
} e_PlayerLowerBodyState;

/** @brief Player cutscene states. Used by `g_Player_CutsceneState`. */
typedef enum _PlayerCutsceneState
{
    PlayerCutsceneState_RunForward  = 0,
    PlayerCutsceneState_WalkForward = 1,
    PlayerCutsceneState_Unused2     = 2, /** @unused Likely for walk backward. */
    PlayerCutsceneState_TurnRight   = 3,
    PlayerCutsceneState_TurnLeft    = 4
} e_PlayerCutsceneState;

/** @brief Rock Drill weapon attack types. */
typedef enum _RockDrillAttackType
{
    RockDrillAttackType_Down   = -1, /** Down input. */
    RockDrillAttackType_Center = 0,  /** No input. */
    RockDrillAttackType_Up     = 1   /** Up input */
} e_RockDrillAttackType;

// ========
// STRUCTS
// ========

/** TODO: Maybe `s_PlayerAttack` fits.
 * - `s_WeaponDataInfo` fits better as this struct stores the information of the data
 * used by weapons.
 */
typedef struct _800AFBF4
{
    /* 0x0 */ s16 attackSfx;
    /* 0x2 */ s16 reloadSfx;
    /* 0x4 */ s16 outOfAmmoSfx;
    /* 0x6 */ u8  animStopAiming; /** Packed anim status. See `s_ModelAnim::status`. */
    /* 0x7 */ u8  animAttack;     /** Packed anim status. See `s_ModelAnim::status`. */
    /* 0x8 */ u8  animAttackHold;
    /* 0x9 */ u8  field_9;
    /* 0xA */ u8  field_A; // Attack type?
    /* 0xB */ s8  __pad_B;
} s_800AFBF4;
STATIC_ASSERT_SIZEOF(s_800AFBF4, 12);

// Used in player lower body state handling.
typedef struct
{
    /* 0x0  */ u8     unk_0;
    /* 0x1  */ u8     floorType; /** `e_FloorType` */
    /* 0x2  */ u8     unk_2[18];
    /* 0x14 */ q19_12 field_14; // Related to hit distance
} s_800C45C8;

// ========
// GLOBALS
// ========

/** @brief `bool` | Determines if the player is transitioning from a walk to a run and vice-versa. */
extern u8 g_Player_IsInWalkToRunTransition;

/** `bool` */
extern u8 g_Player_DisableControl;

extern u8 g_Player_MoveStickMag;

/** @brief `e_RockDrillAttackType` | Rock Drill attack type based on D-Pad input. */
extern s8 g_Player_RockDrill_AttackType;

/** Another variable that saves the index of the enemy being attacked. */
extern s32 g_Player_TargetNpcIdx;

/** @brief Counts the number inputs performed by the user to release an enemy grab. */
extern s32 g_Player_GrabReleaseInputTimer;

/** @brief Current attack animation. */
extern s32 g_Player_AttackAnimIdx;

/** @brief Indicates if a multiple button taps attack is being perfomed. */
extern s32 g_Player_IsMultiTapAttack;

/** @brief `e_AttackInputType` | Attack type performed by the player. */
extern s32 g_Player_MeleeAttackType;

extern s_800AFBF4 D_800AFBF4[11];

extern q19_12 D_800C45DC;

// ====================
// GLOBALS (BSS; Hack)
// ====================
// To match the order of the BSS segment, extern declarations
// are required in a predetermined order.
// This is done until a way to replicate `common`
// segment behavior is found.

extern s_800C44F0 D_800C44F0[10];               /** Table of player keyframe indices. Purpose unknown. */
extern VECTOR3    g_Player_TargetEnemyPosition; /** @brief Q19.12 */
extern q19_12     D_800C454C;                   /** @brief Player rotation speed. More context is required for a name. */
extern q19_12     g_Player_PrevMoveSpeed;       // TODO: Unsure if it should be called a "previous" move speed.
extern s16        D_800C4554;                   // Timer?
extern s16        D_800C4556;                   // Timer?
extern s32        g_Player_HasMoveInput;        /** Determines if input is detected from a movement button or the right analog stick. */
extern s32        g_Player_HasActionInput;      /** Cancels the idle state if the Action or Run buttons are tapped. */
extern s8         g_Player_PrevAttackReceived;
extern u8         g_Player_IsDead;              /** `bool` */

/** `bool`
 *
 * @note Possibly inaccurrate name or buggy functionality. If enemies grab the
 * player with this set to `true`, the user will be unable to interact with anything.
 */
extern u8 g_Player_DisableDamage;

extern u8 __pad_bss_800C4563[13];

extern s_800AFBF4 g_Player_EquippedWeaponInfo;

/** `g_Player_CutsceneState` could be related to animations that play during cutscenes.
 *
 * Called by:
 * - 'func_800D2C7C' in map0_s00.c
 * - 'func_800D2D2C' in map0_s00.c
 *
 * The value changes during cutscenes when the player does any of the following
 * actions:
 * * 0 - Nothing
 * * 1 - Walking
 * * 3 - Moves to the right?
 * * 4 - Moves to the left
 *
 * This behaviour is irregular. After the first in-game dialogue
 * with Harry, the value is kept as 1 (even when he is not walking or in
 * a cutscene) until the next dialogue triggers (when Harry sees
 * Cheryl and the second FMV plays). Something similar happens on the
 * next overlay where the value is 1 during the first
 * cutscene and doesn't change until the player makes an input.
 */
extern u8 g_Player_CutsceneState;

extern u8 __pad_bss_800C457D;

extern u16 g_Player_IsAiming;           /** Determines if the player is aiming a weapon. */
extern u16 g_Player_IsSteppingLeftTap;  /** `bool` */
extern u16 g_Player_IsSteppingRightTap; /** `bool` */
extern u16 g_Player_IsTurningLeft;      /** `bool` */
extern u16 g_Player_IsTurningRight;     /** `bool` */

extern u8 D_800C4588;

extern s8 __pad_bss_800C4589[7];

/** Player instance of this struct. */
extern s_CollisionResult g_Player_CollisionResult;

/** `bool` */
extern u16 g_Player_IsSteppingLeftHold;

/** `bool` */
extern u16 g_Player_IsSteppingRightHold;

/** Q19.12 | Position. */
extern VECTOR3 D_800C45B0; // Assumed type

/** `bool` */
extern u16 g_Player_IsHoldAttack;

/** `bool` */
extern u16 g_Player_IsAttacking;

/** `bool` */
extern u16 g_Player_IsShooting;

extern s8 __pad_bss_800C45C2[6];

extern s_800C45C8 D_800C45C8;

extern s8 __pad_bss_800C45E0[8];

/** `bool` */
extern u16 g_Player_IsMovingForward;

extern s8 __pad_bss_800C45EA[2];

/** Related to game difficulty, used as max for `player.timer_110`. See `Player_LogicUpdate`. */
extern q19_12 D_800C45EC;

/** `bool` */
extern u16 g_Player_IsMovingBackward;

extern s8 __pad_bss_800C45F2[6];

/** XZ player position copy. */
extern VECTOR3 g_Player_PrevPosition;

/** @brief Defines if the player is triggering the running action. */
extern u16 g_Player_IsRunning;

extern s16 __pad_bss_800C4606;

extern q19_12 g_Player_HeadingAngle;

extern s32 __pad_bss_800C460C;

/** Displacement offset. */
extern VECTOR3 D_800C4610;


// ==========
// FUNCTIONS
// ==========

/** Used for enemy target locking. Arg at index 3 is an angle? */
void func_8005CD38(s32* out0, q3_12* out1, const VECTOR3* pos, q3_12 angleMin, q19_12 distMin, s32 mode);

/** Used for player run displacement.
 *
 * @param chara Player character.
 */
void func_80070CF0(s_SubCharacter* player, q19_12 arg1, q19_12 moveDistMax, q19_12 moveDistForward, q19_12 modeDistBack);

void Player_CharaTurn_0(s_SubCharacter* player, e_PlayerLowerBodyState curState);

void Player_CharaTurn_1(s_SubCharacter* player, e_PlayerLowerBodyState curState);

void Player_CharaRotate(s32 speed);

/** @brief Clears movement variables and reset Harry's lower body part animation states in case
 * of switching the player state.
 */
void Player_MovementStateReset(s_SubCharacter* player, e_PlayerLowerBodyState lowerBodyState);

/** Audio-related. */
bool Player_FootstepSfxPlay(s32 animStatus, s_SubCharacter* player, s32 keyframe0, s32 keyframe1, s32 sfx, s8 pitch);

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

/** @brief Updates `g_SysWork.playerWork.extra.upperBodyState` and prepares
 * transitions between running to and from walking animations.
 *
 * @param extra Pointer to `s_PlayerExtra` struct.
 * @param upperState Check if `g_SysWork.playerWork.extra.upperBodyState` is not the same being input in order to clear animation status.
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

/** @brief Updates animations and status related to player attacks.
 * Returns `true` if animations where succefully finished and if performing a multitap attack.
 */
bool Player_CombatAnimUpdate(void);

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

/** @brief Checks for damage received by the player and handles the animations
 * for getting hurt or grabbed by monsters.
 *
 * @note SH2 symbols have a similar function named `PlayerCheckDamage`, but code-wise it has
 * no similarities.
 */
void Player_ReceiveDamage(s_SubCharacter* player, s_PlayerExtra* extra);

// Returns player lower body state.
s32 Player_LowerBodyMoveStateGet(s_SubCharacter* player, s_800C45C8* arg1);

void GameFs_WeaponInfoUpdate(void);

void Game_PlayerMovementsReset(void);

void Player_Controller(void);

/** @brief Determines if the player can stomp or kick a knocked enemy. */
bool func_8007F95C(void);

#define Player_ExtraStateSet(playerChara, playerExtra, playerState)        \
{                                                                          \
    g_SysWork.playerWork.extra.state          = (playerState);             \
    (playerChara)->model.stateStep            = 0;                         \
    (playerChara)->model.controlState         = 0;  \
    (playerExtra)->model.stateStep            = 0;                         \
    (playerExtra)->model.controlState         = 0;  \
    g_SysWork.playerWork.extra.upperBodyState = PlayerUpperBodyState_None; \
    g_SysWork.playerWork.extra.lowerBodyState = PlayerLowerBodyState_None; \
}

/** @brief Sets the given animation flag for both the player character data and extra player data.
 *
 * @param flags Animation flags to set.
 */
static inline void Player_AnimFlagsSet(u32 flags)
{
    s_SubCharacter* chara;
    s_PlayerExtra*  extra;

    extra = &g_SysWork.playerWork.extra;
    chara = &g_SysWork.playerWork.player;

    extra->model.anim.flags |= flags;
    chara->model.anim.flags |= flags;
}

/** @brief Clears the given animation flags for both the player character data and extra player data.
 *
 * @param clearFlags Animation flags to clear.
 */
#define Player_AnimFlagsClear(clearFlags)                      \
{                                                              \
    s_PlayerExtra*  playerExtra = &g_SysWork.playerWork.extra; \
    s_SubCharacter* playerChara = &g_SysWork.playerWork.player;\
                                                               \
    playerExtra->model.anim.flags &= ~(clearFlags);            \
    playerChara->model.anim.flags &= ~(clearFlags);            \
}

/** @brief Resets the player character's collision shapes. */
static inline void Player_CollisionReset()
{
    #define playerChara g_SysWork.playerWork.player

    playerChara.collision.box.topExtent            = DEFAULT_PLAYER_BOX_TOP;
    playerChara.collision.box.bottomExtent         = Q12(0.0f);
    playerChara.collision.box.offsetY              = DEFAULT_PLAYER_BOX_OFFSET_Y;
    playerChara.collision.shapeOffsets.cylinder.vz = Q12(0.0f);
    playerChara.collision.shapeOffsets.cylinder.vx = Q12(0.0f);
    playerChara.collision.shapeOffsets.box.vz      = Q12(0.0f);
    playerChara.collision.shapeOffsets.box.vx      = Q12(0.0f);

    #undef playerChara
}

/** @brief Resets the extra player parameters state if `g_Player_AnimResetRequest` is set.
 *
 * @note This works similar to `Chara_AnimStateReset`.
 *
 * @param player Player character to update.
 * @param extra Extra player  parameters to update.
 */
static inline void Player_AnimStateReset(s_SubCharacter* player, s_PlayerExtra* extra)
{
    if (g_Player_AnimResetRequest)
    {
        Player_ExtraStateSet(player, extra, PlayerState_Reset);
        g_Player_AnimResetRequest = false;
    }
}

#endif
