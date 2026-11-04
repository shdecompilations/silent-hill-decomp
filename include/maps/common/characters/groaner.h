#ifndef _MAPS_CHARACTERS_GROANER_H
#define _MAPS_CHARACTERS_GROANER_H

/** @brief Groaner character flags. */
typedef enum _GroanerFlags
{
    GroanerFlag_None     = 0,
    GroanerFlag_0        = 1 << 0,
    GroanerFlag_1        = 1 << 1,
    GroanerFlag_Airborne = 1 << 2, // Unsure.
    GroanerFlag_3        = 1 << 3,
    GroanerFlag_4        = 1 << 4,
    GroanerFlag_5        = 1 << 5,
    GroanerFlag_6        = 1 << 6,
    GroanerFlag_7        = 1 << 7,
    GroanerFlag_8        = 1 << 8,
    GroanerFlag_Jumping  = 1 << 9,
    GroanerFlag_10       = 1 << 10,
    GroanerFlag_11       = 1 << 11,
    GroanerFlag_12       = 1 << 12
} e_GroanerFlags;

/** @brief Groaner character control states. */
typedef enum _GroanerControl
{
    GroanerControl_None               = 0,
    GroanerControl_WalkForward        = 1,
    GroanerControl_RunForward         = 2,
    GroanerControl_JumpAttack         = 3,
    GroanerControl_4                  = 4, // TODO: Unsure. Related to run forward?
    GroanerControl_StandRecoil        = 5,
    GroanerControl_StunFromJump       = 6,
    GroanerControl_StunFromStandRight = 7,
    GroanerControl_StunFromStandLeft  = 8,
    GroanerControl_Death              = 9
} e_GroanerControl;

/** @brief Groaner character animation indices. */
typedef enum _GroanerAnim
{
    GroanerAnim_Still                        = 0,
    GroanerAnim_StunFromJumpDeathStart       = 1,
    GroanerAnim_StandRecoilFront             = 2,
    GroanerAnim_JumpToStun                   = 3,
    GroanerAnim_StunFromJump                 = 4,
    GroanerAnim_StunFromStandRightDeathStart = 5,
    GroanerAnim_StandRecoilRight             = 6,
    GroanerAnim_StandToStunRight             = 7,
    GroanerAnim_StunFromStandRight           = 8,
    GroanerAnim_StandIdle                    = 9,
    GroanerAnim_JumpAttack                   = 10,
    GroanerAnim_StunFromStandLeftDeathStart  = 11,
    GroanerAnim_StandRecoilLeft              = 12,
    GroanerAnim_StandToStunLeft              = 13,
    GroanerAnim_StunFromStandLeft            = 14,
    GroanerAnim_StandToJumpAttack            = 15,
    GroanerAnim_RunForward                   = 16,
    GroanerAnim_WalkForward                  = 17,
    GroanerAnim_StunFromJumpDeathEnd         = 18,
    GroanerAnim_StunFromStandRightDeathEnd   = 19,
    GroanerAnim_StunFromStandLeftDeathEnd    = 20,
    GroanerAnim_StunFromJumpRecoil           = 21,
    GroanerAnim_StunFromStandRightRecoil     = 22,
    GroanerAnim_StunFromStandLeftRecoil      = 23
} e_GroanerAnim;

/** @brief Groaner character model bone indices. */
typedef enum _GroanerBone
{
    GroanerBone_Hips = 1,
    GroanerBone_Head = 3,
    GroanerBone_Jaw  = 4
} e_GroanerBone;

extern s_AnimInfo GROANER_ANIM_INFOS[];

extern s_CharaKeyframe sharedData_800EEE54_2_s00[];
extern s_CharaKeyframe sharedData_800EF0AC_2_s00[];
extern s_CharaKeyframe sharedData_800EF19C_2_s00;
extern s_CharaKeyframe sharedData_800EF1B0_2_s00[];
extern s_CharaKeyframe sharedData_800EF250_2_s00[];
extern s_CharaKeyframe sharedData_800EF28C_2_s00[];
extern s_CharaKeyframe sharedData_800EF368_2_s00[];
extern s_CharaKeyframe sharedData_800EF4BC_2_s00;
extern s_CharaKeyframe sharedData_800EF4D0_2_s00[];
extern s_CharaKeyframe sharedData_800EF624_2_s00;
extern s_CharaKeyframe sharedData_800EF638_2_s00[];
extern s_CharaKeyframe sharedData_800EF7A0_2_s00;
extern s_CharaKeyframe sharedData_800EF7B4_2_s00[];
extern s_CharaKeyframe sharedData_800EFA84_2_s00[];
extern s_CharaKeyframe sharedData_800EFCDC_2_s00[];
extern s_CharaKeyframe sharedData_800EFF48_2_s00[];
extern s_CharaKeyframe sharedData_800F0038_2_s00[];
extern s_CharaKeyframe sharedData_800F00EC_2_s00[];
extern s_CharaKeyframe sharedData_800F0268_2_s00[];
extern s_CharaKeyframe sharedData_800F03A8_2_s00[];
extern s_CharaKeyframe sharedData_800F04C0_2_s00[];

#define ANIM_INDEX_FROM_KEYFRAME(offset, keyframe, max, count)                             \
    ((((offset) - ((keyframe) > (count))) < (max)) ? ((offset) - ((keyframe) > (count))) : \
                                                     ((max) - 1))

void Groaner_Update(s_SubCharacter* groaner, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

void Groaner_Init(s_SubCharacter* groaner); // `map2_s00` has extra block at end of func compared to other maps.

void Groaner_ControlStandRecoil(s_SubCharacter* groaner);

void Groaner_ControlStun(s_SubCharacter* groaner);

void sharedFunc_800E33DC_2_s00(s_SubCharacter* groaner);

void sharedFunc_800E384C_2_s00(s_SubCharacter* groaner);

void Groaner_ControlWalkForward(s_SubCharacter* groaner);

void Groaner_ControlRunForward(s_SubCharacter* groaner);

void Groaner_ControlJumpAttack(s_SubCharacter* groaner);

void Groaner_Control4(s_SubCharacter* groaner);

void Groaner_ControlDeath(s_SubCharacter* groaner);

void sharedFunc_800E5AA4_2_s00(s_SubCharacter* groaner);

void Groaner_AnimUpdate(s_SubCharacter* groaner, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

void Groaner_CollisionUpdate(s_SubCharacter* groaner);

// Plays SFX.
void sharedFunc_800E71E8_2_s00(s_SubCharacter* groaner);

/*s_AnimInfo GROANER_ANIM_INFOS[] = {
    { Anim_BlendLinear, NO_VALUE, false, ANIM_STATUS(0, false), { Q12(0) }, NO_VALUE, 0 },
    { NULL, ANIM_STATUS(0, false), false, ANIM_STATUS(0, false), { Q12(0) }, 0, 0 },
    { Anim_BlendLinear, ANIM_STATUS(1, false), false, ANIM_STATUS(1, true), { Q12(2) }, NO_VALUE, 0 },
    { Anim_PlayOnce, ANIM_STATUS(1, true), false, ANIM_STATUS(18, false), { Q12(3) }, 0, 15 },
    { Anim_BlendLinear, ANIM_STATUS(2, false), false, ANIM_STATUS(2, true), { Q12(16) }, NO_VALUE, 19 },
    { Anim_PlayOnce, ANIM_STATUS(2, true), false, ANIM_STATUS(9, false), { Q12(24.8f) }, 19, 36 },
    { Anim_BlendLinear, ANIM_STATUS(3, false), false, ANIM_STATUS(3, true), { Q12(16) }, NO_VALUE, 39 },
    { Anim_PlayOnce, ANIM_STATUS(3, true), false, ANIM_STATUS(4, false), { Q12(0) }, 39, 77 },
    { Anim_BlendLinear, ANIM_STATUS(4, false), false, ANIM_STATUS(4, true), { Q12(1) }, NO_VALUE, 78 },
    { Anim_PlayLoop, ANIM_STATUS(4, true), false, NO_VALUE, { Q12(4) }, 78, 93 },
    { Anim_BlendLinear, ANIM_STATUS(5, false), false, ANIM_STATUS(5, true), { Q12(2) }, NO_VALUE, 94 },
    { Anim_PlayOnce, ANIM_STATUS(5, true), false, ANIM_STATUS(19, false), { Q12(3.5f) }, 94, 110 },
    { Anim_BlendLinear, ANIM_STATUS(6, false), false, ANIM_STATUS(6, true), { Q12(16) }, NO_VALUE, 114 },
    { Anim_PlayOnce, ANIM_STATUS(6, true), false, ANIM_STATUS(9, false), { Q12(24.2f) }, 114, 130 },
    { Anim_BlendLinear, ANIM_STATUS(7, false), false, ANIM_STATUS(7, true), { Q12(16) }, NO_VALUE, 134 },
    { Anim_PlayOnce, ANIM_STATUS(7, true), false, ANIM_STATUS(8, false), { Q12(0) }, 134, 164 },
    { Anim_BlendLinear, ANIM_STATUS(8, false), false, ANIM_STATUS(8, true), { Q12(1) }, NO_VALUE, 165 },
    { Anim_PlayLoop, ANIM_STATUS(8, true), false, NO_VALUE, { Q12(6) }, 165, 190 },
    { Anim_BlendLinear, ANIM_STATUS(9, false), false, ANIM_STATUS(9, true), { Q12(2) }, NO_VALUE, 191 },
    { Anim_PlayLoop, ANIM_STATUS(9, true), false, NO_VALUE, { Q12(7) }, 191, 206 },
    { Anim_BlendLinear, ANIM_STATUS(10, false), false, ANIM_STATUS(10, true), { Q12(20) }, NO_VALUE, 214 },
    { Anim_PlayOnce, ANIM_STATUS(10, true), false, ANIM_STATUS(16, false), { Q12(0) }, 214, 243 },
    { Anim_BlendLinear, ANIM_STATUS(11, false), false, ANIM_STATUS(11, true), { Q12(2) }, NO_VALUE, 244 },
    { Anim_PlayOnce, ANIM_STATUS(11, true), false, ANIM_STATUS(20, false), { Q12(3.5f) }, 244, 264 },
    { Anim_BlendLinear, ANIM_STATUS(12, false), false, ANIM_STATUS(12, true), { Q12(16) }, NO_VALUE, 268 },
    { Anim_PlayOnce, ANIM_STATUS(12, true), false, ANIM_STATUS(9, false), { Q12(24.2f) }, 268, 284 },
    { Anim_BlendLinear, ANIM_STATUS(13, false), false, ANIM_STATUS(13, true), { Q12(32) }, NO_VALUE, 288 },
    { Anim_PlayOnce, ANIM_STATUS(13, true), false, ANIM_STATUS(14, false), { Q12(0) }, 288, 318 },
    { Anim_BlendLinear, ANIM_STATUS(14, false), false, ANIM_STATUS(14, true), { Q12(1) }, NO_VALUE, 319 },
    { Anim_PlayLoop, ANIM_STATUS(14, true), false, NO_VALUE, { Q12(6) }, 319, 345 },
    { Anim_BlendLinear, ANIM_STATUS(15, false), false, ANIM_STATUS(15, true), { Q12(32) }, NO_VALUE, 346 },
    { Anim_PlayOnce, ANIM_STATUS(15, true), false, ANIM_STATUS(10, false), { Q12(54) }, 346, 362 },
    { Anim_BlendLinear, ANIM_STATUS(16, false), false, ANIM_STATUS(16, true), { Q12(16) }, NO_VALUE, 363 },
    { Anim_PlayLoop, ANIM_STATUS(16, true), false, NO_VALUE, { Q12(0) }, 363, 370 },
    { Anim_BlendLinear, ANIM_STATUS(17, false), false, ANIM_STATUS(17, true), { Q12(2) }, NO_VALUE, 371 },
    { Anim_PlayLoop, ANIM_STATUS(17, true), false, NO_VALUE, { Q12(11.5f) }, 371, 396 },
    { Anim_BlendLinear, ANIM_STATUS(18, false), false, ANIM_STATUS(18, true), { Q12(16) }, NO_VALUE, 15 },
    { Anim_PlayLoop, ANIM_STATUS(18, true), false, NO_VALUE, { Q12(0) }, 15, 16 },
    { Anim_BlendLinear, ANIM_STATUS(19, false), false, ANIM_STATUS(19, true), { Q12(16) }, NO_VALUE, 110 },
    { Anim_PlayLoop, ANIM_STATUS(19, true), false, NO_VALUE, { Q12(0) }, 110, 111 },
    { Anim_BlendLinear, ANIM_STATUS(20, false), false, ANIM_STATUS(20, true), { Q12(16) }, NO_VALUE, 264 },
    { Anim_PlayLoop, ANIM_STATUS(20, true), false, NO_VALUE, { Q12(0) }, 264, 265 },
    { Anim_BlendLinear, ANIM_STATUS(21, false), false, ANIM_STATUS(21, true), { Q12(16) }, NO_VALUE, 0 },
    { Anim_PlayOnce, ANIM_STATUS(21, true), false, ANIM_STATUS(4, false), { Q12(12) }, 0, 15 },
    { Anim_BlendLinear, ANIM_STATUS(22, false), false, ANIM_STATUS(22, true), { Q12(16) }, NO_VALUE, 94 },
    { Anim_PlayOnce, ANIM_STATUS(22, true), false, ANIM_STATUS(8, false), { Q12(12) }, 94, 110 },
    { Anim_BlendLinear, ANIM_STATUS(23, false), false, ANIM_STATUS(23, true), { Q12(16) }, NO_VALUE, 244 },
    { Anim_PlayOnce, ANIM_STATUS(23, true), false, ANIM_STATUS(14, false), { Q12(12) }, 244, 264 }
};*/

#endif
