#ifndef _MAPS_CHARACTERS_ROMPER_H
#define _MAPS_CHARACTERS_ROMPER_H

/** @brief Romper character flags. */
typedef enum _RomperFlags
{
    RomperFlag_None    = 0,
    RomperFlag_Falling = 1 << 0,
    RomperFlag_1       = 1 << 1,
    RomperFlag_2       = 1 << 2,
    RomperFlag_3       = 1 << 3,
    RomperFlag_4       = 1 << 4, // Jumping?
    RomperFlag_5       = 1 << 5,
    RomperFlag_6       = 1 << 6,
    RomperFlag_7       = 1 << 7,
    RomperFlag_8       = 1 << 8,
    RomperFlag_9       = 1 << 9,
    RomperFlag_10      = 1 << 10,
    RomperFlag_11      = 1 << 11 // Jumping?
} e_RomperFlags;

/** @brief Romper character control states. */
typedef enum _RomperControl
{
    RomperControl_None        = 0, // Assigned function is `NULL`, different from other charas. Check.
    RomperControl_1           = 1,
    RomperControl_WalkForward = 2,
    RomperControl_3           = 3,
    RomperControl_4           = 4,
    RomperControl_Jump        = 5, // Unsure.
    RomperControl_6           = 6,
    RomperControl_7           = 7, // Jump/attack?
    RomperControl_8           = 8,
    RomperControl_9           = 9,
    RomperControl_10          = 10,
    RomperControl_GrabAttack  = 11
} e_RomperControl;

/** @brief Romper character animation indices. */
typedef enum _RomperAnim
{
    RomperAnim_Invalid                    = 0, // } Unsure. Both seemingly unused, only a difference in links.
    RomperAnim_Still                      = 1, // }
    RomperAnim_RunToJump                  = 2,
    RomperAnim_JumpToIdleStart            = 3,
    RomperAnim_JumpToRunForward           = 4,
    RomperAnim_JumpToIdleEnd              = 5,
    RomperAnim_StunToIdleStart            = 6,
    RomperAnim_WalkToRunForwardRightEnd   = 7,
    RomperAnim_WalkToRunForwardLeftEnd    = 8,
    RomperAnim_9                          = 9,
    RomperAnim_JumpStunEnd                = 10,
    RomperAnim_JumpStunStart              = 11,
    RomperAnim_GrabAttack                 = 12, // Unsure.
    RomperAnim_RunForwardLoop             = 13,
    RomperAnim_CreepForward               = 14,
    RomperAnim_WalkForward                = 15,
    RomperAnim_StunToIdleEnd              = 16,
    RomperAnim_WalkToRunForwardRightStart = 17,
    RomperAnim_WalkToRunForwardLeftStart  = 18,
    RomperAnim_RunForwardStart            = 19,
    RomperAnim_20                         = 20  // TODO: This animation is referenced but doesn't actually exist.
} e_RomperAnim;

extern s_AnimInfo ROMPER_ANIM_INFOS[];

extern void (*g_Romper_ControlFuncs[])(s_SubCharacter* romper);

extern s_sharedData_800ED2D4_2_s02 sharedData_800ED2D4_2_s02[];

extern s32 sharedData_800EC950_2_s02;

extern s32 sharedData_800ECA4C_2_s02;

extern s32 sharedData_800ECACC_2_s02;

extern s16 sharedData_800ECB22_2_s02;

extern s16 sharedData_800ECBC2_2_s02;

extern s16 sharedData_800ECBD0_2_s02;

extern s_sharedData_800ED2D4_2_s02 sharedData_800ED314_2_s02[];

void Romper_Update(s_SubCharacter* romper, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

void Romper_Init(s_SubCharacter* romper);

/** Romper footstep SFX. */
void Romper_FootstepSfxPlay(s_SubCharacter* romper, s16 arg1, s16 arg2, u8* arg3);

void sharedFunc_800E60FC_2_s02(s_SubCharacter* romper);

void sharedFunc_800E6420_2_s02(s_SubCharacter* romper);

void Romper_ControlUpdate(s_SubCharacter* romper);

void Romper_Control_1(s_SubCharacter* romper);

void Romper_Control_8(s_SubCharacter* romper);

void Romper_Control_9(s_SubCharacter* romper);

void Romper_ControlGrabAttack(s_SubCharacter* romper);

void sharedFunc_800E8730_2_s02(s_SubCharacter* romper);

void sharedFunc_800E8A40_2_s02(s_SubCharacter* romper, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

void Romper_CollisionUpdate(s_SubCharacter* romper);

s32 sharedFunc_800E939C_2_s02(s_SubCharacter* romper);

s32 sharedFunc_800E94B4_2_s02(s_SubCharacter* romper);

void sharedFunc_800E9714_2_s02(s_SubCharacter* romper);

/*s_AnimInfo ROMPER_ANIM_INFOS[] = {
    { Anim_BlendLinear,  NO_VALUE,               false, ANIM_STATUS(0, false),  { Q12(0.0f) }, NO_VALUE, 0 },
    { NULL,              ANIM_STATUS(0, false),  false, ANIM_STATUS(0, false),  { Q12(0.0f) }, 0, 0 },
    { Anim_BlendLinear,  ANIM_STATUS(1, false),  false, ANIM_STATUS(1, true),   { Q12(20.0f) }, NO_VALUE, 0 },
    { Anim_PlayLoop, ANIM_STATUS(1, true),   false, NO_VALUE,               { Q12(10.0f) }, 0, 1 },
    { Anim_BlendLinear,  ANIM_STATUS(2, false),  false, ANIM_STATUS(2, true),   { Q12(18.0f) }, NO_VALUE, 2 },
    { Anim_PlayOnce, ANIM_STATUS(2, true),   false, ANIM_STATUS(4, false),  { Q12(12.0f) }, 2, 7 },
    { Anim_BlendLinear,  ANIM_STATUS(3, false),  false, ANIM_STATUS(3, true),   { Q12(15.0f) }, NO_VALUE, 8 },
    { Anim_PlayOnce, ANIM_STATUS(3, true),   false, ANIM_STATUS(5, false),  { Q12(15.0f) }, 8, 14 },
    { Anim_BlendLinear,  ANIM_STATUS(4, false),  false, ANIM_STATUS(4, true),   { Q12(20.0f) }, NO_VALUE, 15 },
    { Anim_PlayOnce, ANIM_STATUS(4, true),   false, ANIM_STATUS(13, false), { Q12(12.0f) }, 15, 19 },
    { Anim_BlendLinear,  ANIM_STATUS(5, false),  false, ANIM_STATUS(5, true),   { Q12(15.0f) }, NO_VALUE, 20 },
    { Anim_PlayLoop, ANIM_STATUS(5, true),   false, NO_VALUE,               { Q12(15.0f) }, 20, 33 },
    { Anim_BlendLinear,  ANIM_STATUS(6, false),  false, ANIM_STATUS(6, true),   { Q12(20.0f) }, NO_VALUE, 34 },
    { Anim_PlayOnce, ANIM_STATUS(6, true),   false, ANIM_STATUS(16, false), { Q12(5.0f) }, 34, 38 },
    { Anim_BlendLinear,  ANIM_STATUS(7, false),  false, ANIM_STATUS(7, true),   { Q12(10.0f) }, NO_VALUE, 39 },
    { Anim_PlayOnce, ANIM_STATUS(7, true),   false, ANIM_STATUS(13, false), { Q12(8.0f) }, 39, 49 },
    { Anim_BlendLinear,  ANIM_STATUS(8, false),  false, ANIM_STATUS(8, true),   { Q12(10.0f) }, NO_VALUE, 50 },
    { Anim_PlayOnce, ANIM_STATUS(8, true),   false, ANIM_STATUS(13, false), { Q12(8.0f) }, 50, 60 },
    { Anim_BlendLinear,  ANIM_STATUS(9, false),  false, ANIM_STATUS(9, true),   { Q12(10.0f) }, NO_VALUE, 61 },
    { Anim_PlayOnce, ANIM_STATUS(9, true),   false, ANIM_STATUS(21, false), { Q12(10.0f) }, 61, 87 },
    { Anim_BlendLinear,  ANIM_STATUS(10, false), false, ANIM_STATUS(10, true),  { Q12(20.0f) }, NO_VALUE, 88 },
    { Anim_PlayLoop, ANIM_STATUS(10, true),  false, NO_VALUE,               { Q12(5.0f) }, 88, 92 },
    { Anim_BlendLinear,  ANIM_STATUS(11, false), false, ANIM_STATUS(11, true),  { Q12(4.0f) }, NO_VALUE, 93 },
    { Anim_PlayOnce, ANIM_STATUS(11, true),  false, ANIM_STATUS(10, false), { Q12(6.5f) }, 93, 99 },
    { Anim_BlendLinear,  ANIM_STATUS(12, false), false, ANIM_STATUS(12, true),  { Q12(10.0f) }, NO_VALUE, 100 },
    { Anim_PlayLoop, ANIM_STATUS(12, true),  false, NO_VALUE,               { Q12(6.0f) }, 100, 108 },
    { Anim_BlendLinear,  ANIM_STATUS(13, false), false, ANIM_STATUS(13, true),  { Q12(20.0f) }, NO_VALUE, 109 },
    { Anim_PlayLoop, ANIM_STATUS(13, true),  false, NO_VALUE,               { Q12(0.0f) }, 109, 130 },
    { Anim_BlendLinear,  ANIM_STATUS(14, false), false, ANIM_STATUS(14, true),  { Q12(6.0f) }, NO_VALUE, 131 },
    { Anim_PlayLoop, ANIM_STATUS(14, true),  false, NO_VALUE,               { Q12(0.0f) }, 131, 146 },
    { Anim_BlendLinear,  ANIM_STATUS(15, false), false, ANIM_STATUS(15, true),  { Q12(16.0f) }, NO_VALUE, 147 },
    { Anim_PlayLoop, ANIM_STATUS(15, true),  false, NO_VALUE,               { Q12(4.0f) }, 147, 162 },
    { Anim_BlendLinear,  ANIM_STATUS(16, false), false, ANIM_STATUS(16, true),  { Q12(16.0f) }, NO_VALUE, 38 },
    { Anim_PlayLoop, ANIM_STATUS(16, true),  false, NO_VALUE,               { Q12(0.0f) }, 38, 39 },
    { Anim_BlendLinear,  ANIM_STATUS(17, false), false, ANIM_STATUS(17, true),  { Q12(0.0f) }, NO_VALUE, 120 },
    { Anim_PlayOnce, ANIM_STATUS(17, true),  false, ANIM_STATUS(7, false),  { Q12(0.0f) }, 120, 127 },
    { Anim_BlendLinear,  ANIM_STATUS(18, false), false, ANIM_STATUS(18, true),  { Q12(0.0f) }, NO_VALUE, 110 },
    { Anim_PlayOnce, ANIM_STATUS(18, true),  false, ANIM_STATUS(8, false),  { Q12(0.0f) }, 110, 116 },
    { Anim_BlendLinear,  ANIM_STATUS(19, false), false, ANIM_STATUS(19, true),  { Q12(12.0f) }, NO_VALUE, 109 },
    { Anim_PlayOnce, ANIM_STATUS(19, true),  false, ANIM_STATUS(13, false), { Q12(0.0f) }, 109, 130 }
};*/

#endif
