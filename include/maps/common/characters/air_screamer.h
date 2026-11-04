#ifndef _MAPS_CHARACTERS_AIRSCREAMER_H
#define _MAPS_CHARACTERS_AIRSCREAMER_H

/** @brief Air Screamer or Night Flutter character flags. */
typedef enum _AirScreamerFlags
{
    AirScreamerFlag_None = 0,
    AirScreamerFlag_0    = 1 << 0,
    AirScreamerFlag_1    = 1 << 1,
    AirScreamerFlag_2    = 1 << 2,
    AirScreamerFlag_3    = 1 << 3,
    AirScreamerFlag_4    = 1 << 4,
    AirScreamerFlag_5    = 1 << 5,
    AirScreamerFlag_6    = 1 << 6,
    AirScreamerFlag_7    = 1 << 7,
    AirScreamerFlag_8    = 1 << 8,
    AirScreamerFlag_9    = 1 << 9,
    AirScreamerFlag_10   = 1 << 10,
    AirScreamerFlag_11   = 1 << 11,
    AirScreamerFlag_12   = 1 << 12,
    AirScreamerFlag_13   = 1 << 13, // SFX active? Mirrors similar player flag?
    AirScreamerFlag_14   = 1 << 14,
    AirScreamerFlag_15   = 1 << 15,
    AirScreamerFlag_16   = 1 << 16,
    AirScreamerFlag_17   = 1 << 17,
    AirScreamerFlag_18   = 1 << 18,
    AirScreamerFlag_19   = 1 << 19,
    AirScreamerFlag_20   = 1 << 20,
    AirScreamerFlag_21   = 1 << 21,
    AirScreamerFlag_22   = 1 << 22,
    AirScreamerFlag_23   = 1 << 23,
    AirScreamerFlag_24   = 1 << 24,
    AirScreamerFlag_25   = 1 << 25,
    AirScreamerFlag_26   = 1 << 26,
    AirScreamerFlag_27   = 1 << 27,
    AirScreamerFlag_28   = 1 << 28,
    AirScreamerFlag_29   = 1 << 29,
    AirScreamerFlag_30   = 1 << 30,

    AirScreamerFlag_31   = 1 << 31
} e_AirScreamerFlags;

/** @brief Air Screamer or Night Flutter character hit types. */
typedef enum _AirScreamerHit
{
    AirScreamerHit_None   = 0,
    AirScreamerHit_Recoil = 1,
    AirScreamerHit_Rear   = 2,
    AirScreamerHit_Stun   = 3,
    AirScreamerHit_Kill   = 4
} e_AirScreamerHit;

/** @brief Air Screamer or Night Flutter character control states. */
typedef enum _AirScreamerControl
{
    AirScreamerControl_None      = 0,
    AirScreamerControl_1         = 1,
    AirScreamerControl_2         = 2,
    AirScreamerControl_StandIdle = 3,
    AirScreamerControl_4         = 4, // } Walk
    AirScreamerControl_5         = 5, // }
    AirScreamerControl_6         = 6,
    AirScreamerControl_7         = 7,
    AirScreamerControl_8         = 8,
    AirScreamerControl_9         = 9,
    AirScreamerControl_10        = 10,
    AirScreamerControl_11        = 11,
    AirScreamerControl_12        = 12,
    AirScreamerControl_13        = 13,
    AirScreamerControl_14        = 14,
    AirScreamerControl_15        = 15,
    AirScreamerControl_Recoil    = 16,
    AirScreamerControl_Stun      = 17,
    AirScreamerControl_18        = 18, // } Variable duration hover?
    AirScreamerControl_19        = 19, // }
    AirScreamerControl_20        = 20,
    AirScreamerControl_21        = 21,
    AirScreamerControl_22        = 22,
    AirScreamerControl_23        = 23,
    AirScreamerControl_24        = 24,
    AirScreamerControl_25        = 25,
    AirScreamerControl_26        = 26,
    AirScreamerControl_27        = 27,
    AirScreamerControl_28        = 28,
    AirScreamerControl_29        = 29,
    AirScreamerControl_30        = 30,
    AirScreamerControl_31        = 31,
    AirScreamerControl_32        = 32,
    AirScreamerControl_Glide     = 33,
    AirScreamerControl_34        = 34,
    AirScreamerControl_35        = 35,
    AirScreamerControl_36        = 36,
    AirScreamerControl_37        = 37,
    AirScreamerControl_38        = 38,
    AirScreamerControl_39        = 39,
    AirScreamerControl_40        = 40,
    AirScreamerControl_41        = 41,
    AirScreamerControl_42        = 42,
    AirScreamerControl_43        = 43,
    AirScreamerControl_44        = 44,
    AirScreamerControl_45        = 45,
    AirScreamerControl_46        = 46,
    AirScreamerControl_47        = 47,
    AirScreamerControl_48        = 48,
    AirScreamerControl_49        = 49,
    AirScreamerControl_50        = 50,
    AirScreamerControl_51        = 51,

    AirScreamerControl_Count = 52
} e_AirScreamerControl;

/** @brief Air Screamer character animation indices. */
typedef enum _AirScreamerAnim
{
    AirScreamerAnim_Still                  = 0,
    AirScreamerAnim_StandBiteAttack        = 1,
    AirScreamerAnim_HoverBiteAttack        = 2,
    AirScreamerAnim_GlideScratchAttack     = 3,
    AirScreamerAnim_StandToStun            = 4,
    AirScreamerAnim_Death                  = 5,
    AirScreamerAnim_StandRecoil            = 6,
    AirScreamerAnim_HoverRecoil            = 7,
    AirScreamerAnim_GlideRecoil            = 8,
    AirScreamerAnim_HoverInjuredBiteAttack = 9,
    AirScreamerAnim_HoverToStun            = 10,
    AirScreamerAnim_GlideToStun            = 11,
    AirScreamerAnim_DeathThroes            = 12,
    AirScreamerAnim_StandBiteAttackStart   = 13,
    AirScreamerAnim_StandBiteAttackEnd     = 14,
    AirScreamerAnim_GlideToHover           = 15,
    AirScreamerAnim_GlideToWalkForward     = 16,
    AirScreamerAnim_Glide                  = 17,
    AirScreamerAnim_HoverToGlide           = 18,
    AirScreamerAnim_HoverVariable          = 19,
    AirScreamerAnim_HoverInjuredToStun     = 20,
    AirScreamerAnim_HoverInjured           = 21,
    AirScreamerAnim_StandIdleToHover       = 22,
    AirScreamerAnim_StandIdle              = 23,
    AirScreamerAnim_StandIdleToGlide       = 24,
    AirScreamerAnim_WalkForward            = 25,
    AirScreamerAnim_Stun                   = 26,
    AirScreamerAnim_HoverConstant          = 27,
    AirScreamerAnim_28                     = 28
} e_AirScreamerAnim;

extern VECTOR3 sharedData_800DE1A0_0_s01; /** Q19.12 | Used for LOS offset. */
extern VECTOR3 g_AirScreamer_TargetPosition2; /** Q19.12 */
extern VECTOR3 sharedData_800DE1C0_0_s01; /** Q19.12 | Used for LOS offset. */
extern VECTOR3 sharedData_800DE1E0_0_s01; /** Q19.12 | Used for wall collision offset. */
extern VECTOR3 sharedData_800DE1F0_0_s01; // Q19.12 | Used for collision.
extern s_CollisionResult g_AirScreamer_CollisionResult; // Cached result?

/** Air Screamer or Night Flutter control functions. Indexed using `e_AirScreamerControl`. */
extern void (*g_AirScreamer_ControlFuncs[AirScreamerControl_Count])(s_SubCharacter* airScreamer);

/** @brief Updates an active Air Screamer or Night Flutter character.
 *
 * @param airScreamer Air Screamer or Night Flutter character to update.
 * @param animHdr Character animation data.
 * @param boneCoords Character model bone coords.
 */
void AirScreamer_Update(s_SubCharacter* airScreamer, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

bool sharedFunc_800D21E4_0_s01(s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

/** Probes collision and sets `airScreamerProps.floorHeight`. */
void sharedFunc_800D2200_0_s01(s_SubCharacter* airScreamer);

bool sharedFunc_800D2274_0_s01(s_SubCharacter* airScreamer);

void sharedFunc_800D2364_0_s01(s_SubCharacter* airScreamer);

bool sharedFunc_800D2390_0_s01(s_SubCharacter* airScreamer);

/** @brief Initializes an Air Screamer or Night Flutter character.
 *
 * @param airScreamer Air Screamer or Night Flutter character to initialize.
 */
bool AirScreamer_Init(s_SubCharacter* airScreamer);

void sharedFunc_800D2B00_0_s01(s_SubCharacter* airScreamer);

void sharedFunc_800D2B10_0_s01(s_SubCharacter* airScreamer);

void sharedFunc_800D2B28_0_s01(s_SubCharacter* airScreamer);

void sharedFunc_800D2B4C_0_s01(s_SubCharacter* airScreamer);

/** @brief Warps an Air Screamer or Night Flutter character to the floor.
 *
 * @param airScreamer Air Screamer or Night Flutter character to warp.
 */
void AirScreamer_FloorWarp(s_SubCharacter* airScreamer);

void sharedFunc_800D2BE4_0_s01(s_SubCharacter* airScreamer);

void sharedFunc_800D2BF4_0_s01(s_SubCharacter* airScreamer);

/** @brief Applies damage to an Air Screamer character and returns a hit type corresponding to the damage context.
 *
 * @param airScreamer Air Screamer character to damage.
 * @param mult Damage multiplier.
 * @return Hit type (`e_AirScreamerHit`).
 */
s32 AirScreamer_DamageTake(s_SubCharacter* airScreamer, q19_12 mult);

bool sharedFunc_800D2E04_0_s01(s_SubCharacter* airScreamer, VECTOR3* inVec, q19_12* outDist, q19_12* outAngle);

bool sharedFunc_800D3430_0_s01(s_SubCharacter* airScreamer, q19_12* dist, q19_12* angle);

/** Pecking distance getter? */
s32 sharedFunc_800D3814_0_s01(s_SubCharacter* airScreamer);

bool sharedFunc_800D31D0_0_s01(s_SubCharacter* airScreamer, const VECTOR3* pos, s32 arg2);

bool AirScreamer_ControlUpdate(s_SubCharacter* airScreamer);

void AirScreamer_Control_0(s_SubCharacter* airScreamer);

void AirScreamer_Control_1(s_SubCharacter* airScreamer);

/** Probability query. */
void sharedFunc_800D3DFC_0_s01(s_SubCharacter* airScreamer);

bool sharedFunc_800D62D8_0_s01(s_SubCharacter* airScreamer);

/** Returns anim-related index. */
s32 sharedFunc_800D4A80_0_s01(s_SubCharacter* airScreamer);

/** Gets player Y position with an offset. */
q19_12 sharedFunc_800D5274_0_s01(void);

void sharedFunc_800D633C_0_s01(s_SubCharacter* airScreamer);
void sharedFunc_800D63A4_0_s01(s_SubCharacter* airScreamer);
void sharedFunc_800D6600_0_s01(s_SubCharacter* airScreamer);

q19_12 sharedFunc_800D7120_0_s01(q19_12 moveSpeed, s32 arg1, s32 arg2);
q19_12 sharedFunc_800D71F0_0_s01(q19_12 moveSpeed, s32 arg1, s32 arg2, s32 arg3);

s32 sharedFunc_800D7440_0_s01(s_CollisionResult* collResult, VECTOR* offset, s_SubCharacter* airScreamer);

void sharedFunc_800D7560_0_s01(s_SubCharacter* airScreamer);

void sharedFunc_800D76A0_0_s01(s_SubCharacter* airScreamer);

// Unused speed getter?
q19_12 sharedFunc_800D7714_0_s01(s_SubCharacter* airScreamer);

q19_12 sharedFunc_800D77D0_0_s01(s_SubCharacter* airScreamer);

bool sharedFunc_800D7AB0_0_s01(s_SubCharacter* airScreamer);

/** @brief Applies a multi-stage bend deformation to several model coordinates.
 * Builds three rotation matrices using scaled fractions of `chara->angleToTarget` and
 * multiplies them into `boneCoords[10]`, `boneCoords[9]`, `boneCoords[1]`, and `boneCoords[2]`.
 * Used when the animation flag bit 0 is active to drive a chained bending motion.
 */
void sharedFunc_800D7B14_0_s01(s_SubCharacter* airScreamer, GsCOORDINATE2* boneCoords);

q19_12 Model_AnimDurationGet(s_Model* model);

bool sharedFunc_800D7EBC_0_s01(s_SubCharacter* airScreamer);

bool sharedFunc_800D81B0_0_s01(s_SubCharacter* airScreamer);

void sharedFunc_800D81D0_0_s01(s_SubCharacter* airScreamer);

void sharedFunc_800D8244_0_s01(s_SubCharacter* airScreamer);

void sharedFunc_800D82B8_0_s01(s_SubCharacter* airScreamer);

void sharedFunc_800D8714_0_s01(s_SubCharacter* airScreamer, q19_12 moveSpeed, q19_12 headingAngle);

void sharedFunc_800D87FC_0_s01(s_SubCharacter* airScreamer);

// NOTE: Following are only present in M0S01
void func_800D39F4(s_SubCharacter* airScreamer);

void func_800D3A3C(s_SubCharacter* airScreamer);

void func_800D3AC0(s_SubCharacter* airScreamer);

// TODO: Sort these properly.

/* TODO: This function is almost identical to `sharedFunc_800D57C8_0_s01`.
 * Both are used in map0_s01 so ifdefs cannot be used based on the map name.
 * Sections under `#if 0` are what is missing compared to `sharedFunc_800D57C8_0_s01`.
 */
void sharedFunc_800D598C_0_s01(s_SubCharacter* airScreamer);

// Probably all Air Screamer section below. Unsure about functions above.

/* TODO: This function is almost identical to `sharedFunc_800D57C8_0_s01`.
 * Also look at `sharedFunc_800D598C_0_s01`.
 */
void sharedFunc_800D5B10_0_s01(s_SubCharacter* airScreamer);

void sharedFunc_800D5C90_0_s01(s_SubCharacter* airScreamer);

void sharedFunc_800D5D80_0_s01(s_SubCharacter* airScreamer);

void sharedFunc_800D5E14_0_s01(s_SubCharacter* airScreamer);

void AirScreamer_Control_2(s_SubCharacter* airScreamer);

void AirScreamer_ControlStandIdle(s_SubCharacter* airScreamer);

void AirScreamer_Control_4(s_SubCharacter* airScreamer);

void AirScreamer_Control_5(s_SubCharacter* airScreamer);

void AirScreamer_Control_6(s_SubCharacter* airScreamer);

void AirScreamer_Control_7(s_SubCharacter* airScreamer);

bool sharedFunc_800D4AEC_0_s01(s_SubCharacter* airScreamer, VECTOR3* arg1, VECTOR3* arg2, VECTOR3* arg3);

void AirScreamer_Control_8(s_SubCharacter* airScreamer);

void AirScreamer_Control_9(s_SubCharacter* airScreamer);

void AirScreamer_Control_11(s_SubCharacter* airScreamer);

void AirScreamer_Control_12(s_SubCharacter* airScreamer);

void AirScreamer_Control_13(s_SubCharacter* airScreamer);

void AirScreamer_Control_14(s_SubCharacter* airScreamer);

void AirScreamer_Control_15(s_SubCharacter* airScreamer);

// Very similar to `AirScreamer_Control_44`, changes here should be copied there.
void AirScreamer_ControlRecoil(s_SubCharacter* airScreamer);

void AirScreamer_ControlStun(s_SubCharacter* airScreamer);

void AirScreamer_Control_18(s_SubCharacter* airScreamer);

void AirScreamer_Control_19(s_SubCharacter* airScreamer);

void AirScreamer_Control_20(s_SubCharacter* airScreamer);

void AirScreamer_Control_21(s_SubCharacter* airScreamer);

void AirScreamer_Control_22(s_SubCharacter* airScreamer);

void AirScreamer_Control_23(s_SubCharacter* airScreamer);

void AirScreamer_Control_24(s_SubCharacter* airScreamer);

void AirScreamer_Control_25(s_SubCharacter* airScreamer);

void AirScreamer_Control_26(s_SubCharacter* airScreamer);

void AirScreamer_Control_27(s_SubCharacter* airScreamer);

void AirScreamer_Control_28(s_SubCharacter* airScreamer);

void AirScreamer_Control_30(s_SubCharacter* airScreamer);

void AirScreamer_Control_31(s_SubCharacter* airScreamer);

void AirScreamer_Control_32(s_SubCharacter* airScreamer);

void AirScreamer_ControlGlide(s_SubCharacter* airScreamer);

void AirScreamer_Control_34(s_SubCharacter* airScreamer);

void AirScreamer_Control_36(s_SubCharacter* airScreamer);

void AirScreamer_Control_37(s_SubCharacter* airScreamer);

void AirScreamer_Control_40(s_SubCharacter* airScreamer);

void AirScreamer_Control_41(s_SubCharacter* airScreamer);

void AirScreamer_Control_42(s_SubCharacter* airScreamer);

void AirScreamer_Control_43(s_SubCharacter* airScreamer);

// Very similar to `AirScreamer_ControlRecoil`, changes here should be copied there.
void AirScreamer_Control_44(s_SubCharacter* airScreamer);

void AirScreamer_Control_45(s_SubCharacter* airScreamer);

bool sharedFunc_800DBF88_2_s00(s_SubCharacter* airScreamer, q19_12 arg1);

/** @brief Checks if an Air Screamer is within a thredhold of 2 meters above the floor.
 *
 * @param airScreamer Air Screamer character to check.
 * @param `true` if the character is floored, `false` otherwise.
 */
bool AirScreamer_IsFlooredCheck(s_SubCharacter* airScreamer);

bool sharedFunc_800DC0E4_2_s00(s_SubCharacter* airScreamer, q19_12 moveSpeedMult);

bool sharedFunc_800DC200_2_s00(s_SubCharacter* airScreamer);

bool sharedFunc_800DC30C_2_s00(s_SubCharacter* airScreamer);

/** Checks if an Air Screamer is near the floor? */
bool sharedFunc_800DC3BC_2_s00(s_SubCharacter* airScreamer);

/** State step getter. */
s32 sharedFunc_800DC438_2_s00(s_SubCharacter* airScreamer);

bool AirScreamer_HasLandedCheck(s_SubCharacter* airScreamer);

s32 sharedFunc_800DC598_2_s00(s_SubCharacter* airScreamer);

bool sharedFunc_800DC67C_2_s00(s_SubCharacter* airScreamer);

/** Chance getter? */
q19_12 sharedFunc_800DC6E4_2_s00(s_SubCharacter* airScreamer, q19_12 arg1);

/** Chance getter? */
q19_12 sharedFunc_800DC894_2_s00(s_SubCharacter* airScreamer, q19_12 dist);

void sharedFunc_800DD13C_2_s00(s_SubCharacter* airScreamer, s32 npcSlot, q19_12 spawnChance);

void sharedFunc_800DD2C4_2_s00(s_SubCharacter* airScreamer, s32 arg1, s32 arg2);

void sharedFunc_800DD4A4_2_s00(s_SubCharacter* airScreamer);

void sharedFunc_800DD4EC_2_s00(s_SubCharacter* airScreamer);

void sharedFunc_800DD534_2_s00(s_SubCharacter* airScreamer);

void sharedFunc_800DD588_2_s00(s_SubCharacter* airScreamer);

void sharedFunc_800DD834_2_s00(s_SubCharacter* airScreamer);

void sharedFunc_800DDA80_2_s00(s_SubCharacter* airScreamer);

void sharedFunc_800DDE14_2_s00(s_SubCharacter* airScreamer);

void sharedFunc_800DDF74_2_s00(s_SubCharacter* airScreamer, q19_12 dist, q19_12 headingAngle);

void sharedFunc_800DE034_2_s00(s_SubCharacter* airScreamer, VECTOR3* vec, q19_12 dist);

void sharedFunc_800DE11C_2_s00(s_SubCharacter* airScreamer);

bool sharedFunc_800DE250_2_s00(s_SubCharacter* airScreamer);

bool sharedFunc_800DE578_2_s00(s_SubCharacter* airScreamer);

void sharedFunc_800DE6A8_2_s00(s_SubCharacter* airScreamer, VECTOR3* vec, q19_12 dist);

void sharedFunc_800DE7E0_2_s00(s_SubCharacter* airScreamer);

void sharedFunc_800DEBCC_2_s00(s_SubCharacter* airScreamer);

void sharedFunc_800DECC4_2_s00(s_SubCharacter* airScreamer);

q19_12 sharedFunc_800DEE24_2_s00(s_SubCharacter* airScreamer);

void sharedFunc_800DF358_2_s00(s_SubCharacter* airScreamer);

/** Condition is probably to differentiate between an Air Screamer and Night Flutter. */
void sharedFunc_800DF448_2_s00(s_SubCharacter* airScreamer, bool cond);

void sharedFunc_800DF60C_2_s00(s_SubCharacter* airScreamer);

void sharedFunc_800DF710_2_s00(s_SubCharacter* airScreamer);

void sharedFunc_800DF80C_2_s00(s_SubCharacter* airScreamer);

void sharedFunc_800DF8A0_2_s00(s_SubCharacter* airScreamer);

void sharedFunc_800E012C_2_s00(s_SubCharacter* airScreamer);

void sharedFunc_800E021C_2_s00(s_SubCharacter* airScreamer, s32 arg1, s32 arg2);

void sharedFunc_800E041C_2_s00(s_SubCharacter* airScreamer);

void sharedFunc_800E04B0_2_s00(s_SubCharacter* airScreamer);

void sharedFunc_800E0514_2_s00(s_SubCharacter* airScreamer);

/** Air Screamer state step getter. `const` is required for match. */
bool sharedFunc_800D5F00_0_s01(s_SubCharacter* const airScreamer);

bool sharedFunc_800D3508_0_s01(s_SubCharacter* airScreamer, q19_12* dist);

bool sharedFunc_800D3630_0_s01(s_SubCharacter* airScreamer, q19_12* dist);

/** Returns flags? Specific to Air Screamer? */
s32 sharedFunc_800D3758_0_s01(s_SubCharacter* airScreamer, q19_12* outDist, q19_12* outAngle, q19_12 dist, s32* arg4);

s32 sharedFunc_800D569C_0_s01(s_SubCharacter* airScreamer, q19_12 posY, q19_12 dist);

void sharedFunc_800D57C8_0_s01(s_SubCharacter* airScreamer);

void sharedFunc_800D5E78_0_s01(s_SubCharacter* airScreamer, q19_12 angle);

s32 sharedFunc_800D6A60_0_s01(VECTOR3* offset, VECTOR3* vec1, s_SubCharacter* airScreamer, s32 arg3, VECTOR3* vec2);

void sharedFunc_800D6C7C_0_s01(VECTOR* arg0, s_SubCharacter* airScreamer, s32 arg2, VECTOR3* arg3);

/** Plays SFX. */
void sharedFunc_800D2364_0_s01(s_SubCharacter* airScreamer);

void sharedFunc_800D5638_0_s01(s_SubCharacter* airScreamer);

void sharedFunc_800DF2D0_2_s00(s_SubCharacter* airScreamer);

void sharedFunc_800D4E84_0_s01(s_SubCharacter* airScreamer);

void sharedFunc_800D529C_0_s01(s_SubCharacter* airScreamer, q19_12 radius, q19_12 headingAngle);

void sharedFunc_800D53AC_0_s01(s_SubCharacter* airScreamer);

void sharedFunc_800D6EC4_0_s01(s_SubCharacter* airScreamer);

void sharedFunc_800D7B14_0_s01(s_SubCharacter* airScreamer, GsCOORDINATE2* boneCoords);

void sharedFunc_800DE1F8_2_s00(s_SubCharacter* airScreamer);

bool sharedFunc_800DEC64_2_s00(s_SubCharacter* airScreamer);

void sharedFunc_800DEC84_2_s00(s_SubCharacter* airScreamer, q19_12 radius, q19_12 angle);

void sharedFunc_800DECA4_2_s00(s_SubCharacter* airScreamer, VECTOR3* vec, q19_12 dist);

void sharedFunc_800DF22C_2_s00(s_SubCharacter* airScreamer);

bool sharedFunc_800DF24C_2_s00(s_SubCharacter* airScreamer);

bool sharedFunc_800D2FB4_0_s01(s_SubCharacter* airScreamer, VECTOR3* playerPos, s32 playerField10C);

void sharedFunc_800D72E8_0_s01(s_SubCharacter* airScreamer, q19_12 angle0, q19_12 rotSpeedY);

void sharedFunc_800D6EC4_0_s01(s_SubCharacter* airScreamer);

void sharedFunc_800D8804_0_s01(void);

#endif
