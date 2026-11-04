#ifndef _MAPS_CHARACTERS_HANGEDSCRATCHER_H
#define _MAPS_CHARACTERS_HANGEDSCRATCHER_H

/** @brief Hanged Scratcher character flags */
typedef enum _HangedScratcherFlags
{
    HangedScratcherFlag_Airborne = 1 << 0,
    HangedScratcherFlag_1        = 1 << 1,
    HangedScratcherFlag_2        = 1 << 2,
    HangedScratcherFlag_4        = 1 << 4,
    HangedScratcherFlag_5        = 1 << 5,
    HangedScratcherFlag_6        = 1 << 6,
    HangedScratcherFlag_7        = 1 << 7,
    HangedScratcherFlag_8        = 1 << 8,
    HangedScratcherFlag_9        = 1 << 9
} e_HangedScratcherFlags;

/** @brief Hanged Scratcher character control states. */
typedef enum _HangedScratcherControl
{
    HangedScratcherControl_None        = 0,
    HangedScratcherControl_1           = 1,
    HangedScratcherControl_2           = 2,
    HangedScratcherControl_RunForward  = 3,
    HangedScratcherControl_4           = 4,
    HangedScratcherControl_5           = 5,
    HangedScratcherControl_6           = 6,
    HangedScratcherControl_WalkForward = 7,
    HangedScratcherControl_8           = 8,
    HangedScratcherControl_9           = 9,
    HangedScratcherControl_10          = 10,
    HangedScratcherControl_11          = 11,
    HangedScratcherControl_12          = 12,
    HangedScratcherControl_13          = 13,
    HangedScratcherControl_14          = 14, // Entrance?
    HangedScratcherControl_15          = 15,
    HangedScratcherControl_16          = 16
} e_HangedScratcherControl;

/** @brief Hanged Scratcher character animation indices. */
typedef enum _HangedScratcherAnim
{
    HangedScratcherAnim_Still               = 0,
    HangedScratcherAnim_JumpDownFromWall    = 1,
    HangedScratcherAnim_EmergeFromWater     = 2,
    HangedScratcherAnim_JumpDownFromCeiling = 3,
    HangedScratcherAnim_4                   = 4,
    HangedScratcherAnim_5                   = 5,
    HangedScratcherAnim_6                   = 6,
    HangedScratcherAnim_7                   = 7,
    HangedScratcherAnim_8                   = 8,
    HangedScratcherAnim_9                   = 9,
    HangedScratcherAnim_10                  = 10,
    HangedScratcherAnim_11                  = 11,
    HangedScratcherAnim_12                  = 12,
    HangedScratcherAnim_13                  = 13,
    HangedScratcherAnim_14                  = 14,
    HangedScratcherAnim_15                  = 15,
    HangedScratcherAnim_16                  = 16,
    HangedScratcherAnim_RunForward          = 17,
    HangedScratcherAnim_WalkForward         = 18,
    HangedScratcherAnim_19                  = 19,
    HangedScratcherAnim_20                  = 20,
    HangedScratcherAnim_21                  = 21,
    HangedScratcherAnim_22                  = 22,
    HangedScratcherAnim_23                  = 23,
    HangedScratcherAnim_24                  = 24,
    HangedScratcherAnim_25                  = 25,
    HangedScratcherAnim_26                  = 26
} e_HangedScratcherAnim;

/*s_AnimInfo HANGED_SCRATCHER_ANIM_INFOS[] = {
    { Anim_BlendLinear, NO_VALUE, false, ANIM_STATUS(0, false), { Q12(0) }, NO_VALUE, 0 },
    { NULL, ANIM_STATUS(0, false), false, ANIM_STATUS(0, false), { Q12(0) }, 0, 0 },
    { Anim_BlendLinear, ANIM_STATUS(1, false), false, ANIM_STATUS(1, true), { Q12(0) }, NO_VALUE, 0 },
    { Anim_PlayOnce, ANIM_STATUS(1, true), false, ANIM_STATUS(24, false), { Q12(0) }, 0, 50 },
    { Anim_BlendLinear, ANIM_STATUS(2, false), false, ANIM_STATUS(2, true), { Q12(0) }, NO_VALUE, 51 },
    { Anim_PlayOnce, ANIM_STATUS(2, true), false, ANIM_STATUS(25, false), { Q12(0) }, 51, 86 },
    { Anim_BlendLinear, ANIM_STATUS(3, false), false, ANIM_STATUS(3, true), { Q12(0) }, NO_VALUE, 87 },
    { Anim_PlayOnce, ANIM_STATUS(3, true), false, ANIM_STATUS(26, false), { Q12(0) }, 87, 97 },
    { Anim_BlendLinear, ANIM_STATUS(4, false), false, ANIM_STATUS(4, true), { Q12(18) }, NO_VALUE, 98 },
    { Anim_PlayOnce, ANIM_STATUS(4, true), false, ANIM_STATUS(14, false), { Q12(19.5f) }, 98, 115 },
    { Anim_BlendLinear, ANIM_STATUS(5, false), false, ANIM_STATUS(5, true), { Q12(12) }, NO_VALUE, 116 },
    { Anim_PlayOnce, ANIM_STATUS(5, true), false, ANIM_STATUS(15, false), { Q12(23) }, 116, 135 },
    { Anim_BlendLinear, ANIM_STATUS(6, false), false, ANIM_STATUS(6, true), { Q12(4) }, NO_VALUE, 136 },
    { Anim_PlayOnce, ANIM_STATUS(6, true), false, ANIM_STATUS(19, false), { Q12(2) }, 136, 147 },
    { Anim_BlendLinear, ANIM_STATUS(7, false), false, ANIM_STATUS(7, true), { Q12(4) }, NO_VALUE, 148 },
    { Anim_PlayOnce, ANIM_STATUS(7, true), false, ANIM_STATUS(20, false), { Q12(1) }, 148, 153 },
    { Anim_BlendLinear, ANIM_STATUS(8, false), false, ANIM_STATUS(8, true), { Q12(4) }, NO_VALUE, 154 },
    { Anim_PlayOnce, ANIM_STATUS(8, true), false, ANIM_STATUS(14, false), { Q12(14) }, 154, 165 },
    { Anim_BlendLinear, ANIM_STATUS(9, false), false, ANIM_STATUS(9, true), { Q12(4) }, NO_VALUE, 168 },
    { Anim_PlayOnce, ANIM_STATUS(9, true), false, ANIM_STATUS(15, false), { Q12(0) }, 168, 175 },
    { Anim_BlendLinear, ANIM_STATUS(10, false), false, ANIM_STATUS(10, true), { Q12(16) }, NO_VALUE, 176 },
    { Anim_PlayOnce, ANIM_STATUS(10, true), false, ANIM_STATUS(12, false), { Q12(0) }, 176, 191 },
    { Anim_BlendLinear, ANIM_STATUS(11, false), false, ANIM_STATUS(11, true), { Q12(2) }, NO_VALUE, 192 },
    { Anim_PlayOnce, ANIM_STATUS(11, true), false, ANIM_STATUS(13, false), { Q12(0) }, 192, 201 },
    { Anim_BlendLinear, ANIM_STATUS(12, false), false, ANIM_STATUS(12, true), { Q12(16) }, NO_VALUE, 202 },
    { Anim_PlayLoop, ANIM_STATUS(12, true), false, NO_VALUE, { Q12(3) }, 202, 215 },
    { Anim_BlendLinear, ANIM_STATUS(13, false), false, ANIM_STATUS(13, true), { Q12(2) }, NO_VALUE, 216 },
    { Anim_PlayLoop, ANIM_STATUS(13, true), false, NO_VALUE, { Q12(3) }, 216, 221 },
    { Anim_BlendLinear, ANIM_STATUS(14, false), false, ANIM_STATUS(14, true), { Q12(2) }, NO_VALUE, 222 },
    { Anim_PlayLoop, ANIM_STATUS(14, true), false, NO_VALUE, { Q12(0) }, 222, 231 },
    { Anim_BlendLinear, ANIM_STATUS(15, false), false, ANIM_STATUS(15, true), { Q12(2) }, NO_VALUE, 232 },
    { Anim_PlayLoop, ANIM_STATUS(15, true), false, NO_VALUE, { Q12(0) }, 232, 243 },
    { Anim_BlendLinear, ANIM_STATUS(16, false), false, ANIM_STATUS(16, true), { Q12(16) }, NO_VALUE, 244 },
    { Anim_PlayOnce, ANIM_STATUS(16, true), false, ANIM_STATUS(15, false), { Q12(0) }, 244, 266 },
    { Anim_BlendLinear, ANIM_STATUS(17, false), false, ANIM_STATUS(17, true), { Q12(8) }, NO_VALUE, 267 },
    { Anim_PlayLoop, ANIM_STATUS(17, true), false, NO_VALUE, { Q12(0) }, 267, 276 },
    { Anim_BlendLinear, ANIM_STATUS(18, false), false, ANIM_STATUS(18, true), { Q12(4) }, NO_VALUE, 277 },
    { Anim_PlayLoop, ANIM_STATUS(18, true), false, NO_VALUE, { Q12(0) }, 277, 292 },
    { Anim_BlendLinear, ANIM_STATUS(19, false), false, ANIM_STATUS(19, true), { Q12(16) }, NO_VALUE, 147 },
    { Anim_PlayLoop, ANIM_STATUS(19, true), false, NO_VALUE, { Q12(0) }, 147, 148 },
    { Anim_BlendLinear, ANIM_STATUS(20, false), false, ANIM_STATUS(20, true), { Q12(16) }, NO_VALUE, 153 },
    { Anim_PlayLoop, ANIM_STATUS(20, true), false, NO_VALUE, { Q12(0) }, 153, 154 },
    { Anim_BlendLinear, ANIM_STATUS(21, false), false, ANIM_STATUS(21, true), { Q12(16) }, NO_VALUE, 136 },
    { Anim_PlayOnce, ANIM_STATUS(21, true), false, ANIM_STATUS(12, false), { Q12(16) }, 136, 147 },
    { Anim_BlendLinear, ANIM_STATUS(22, false), false, ANIM_STATUS(22, true), { Q12(16) }, NO_VALUE, 148 },
    { Anim_PlayOnce, ANIM_STATUS(22, true), false, ANIM_STATUS(13, false), { Q12(8) }, 148, 153 },
    { Anim_BlendLinear, ANIM_STATUS(23, false), false, ANIM_STATUS(23, true), { Q12(4) }, NO_VALUE, 168 },
    { Anim_PlayOnce, ANIM_STATUS(23, true), false, ANIM_STATUS(11, false), { Q12(0) }, 168, 174 },
    { Anim_BlendLinear, ANIM_STATUS(24, false), false, ANIM_STATUS(24, true), { Q12(0) }, NO_VALUE, 50 },
    { Anim_PlayLoop, ANIM_STATUS(24, true), false, NO_VALUE, { Q12(0) }, 50, 51 },
    { Anim_BlendLinear, ANIM_STATUS(25, false), false, ANIM_STATUS(25, true), { Q12(0) }, NO_VALUE, 86 },
    { Anim_PlayLoop, ANIM_STATUS(25, true), false, NO_VALUE, { Q12(0) }, 86, 87 },
    { Anim_BlendLinear, ANIM_STATUS(26, false), false, ANIM_STATUS(26, true), { Q12(0) }, NO_VALUE, 97 },
    { Anim_PlayLoop, ANIM_STATUS(26, true), false, NO_VALUE, { Q12(0) }, 97, 98 }
};*/

extern s_AnimInfo HANGED_SCRATCHER_ANIM_INFOS[];

extern q19_12 sharedData_800DE28C_5_s00;

void HangedScratcher_Update(s_SubCharacter* scratcher, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

void HangedScratcher_Init(s_SubCharacter* scratcher);

void sharedFunc_800CFF74_5_s00(s_SubCharacter* scratcher);

void HangedScratcher_ControlUpdate(s_SubCharacter* scratcher);

void HangedScratcher_Control_16(s_SubCharacter* scratcher);

void HangedScratcher_Control_1(s_SubCharacter* scratcher);

void HangedScratcher_ControlRunForward(s_SubCharacter* scratcher);

void HangedScratcher_Control_3(s_SubCharacter* scratcher);

void HangedScratcher_Control_4(s_SubCharacter* scratcher);

void HangedScratcher_Control_5(s_SubCharacter* scratcher);

void HangedScratcher_Control_6(s_SubCharacter* scratcher);

void HangedScratcher_ControlWalkForward(s_SubCharacter* scratcher);

void HangedScratcher_Control_8(s_SubCharacter* scratcher);

void HangedScratcher_Control_9(s_SubCharacter* scratcher);

void HangedScratcher_Control_10(s_SubCharacter* scratcher);

void HangedScratcher_Control_11(s_SubCharacter* scratcher);

void HangedScratcher_Control_12(s_SubCharacter* scratcher);

void HangedScratcher_Control_13(s_SubCharacter* scratcher);

void HangedScratcher_Control_14(s_SubCharacter* scratcher);

void HangedScratcher_Control_15(s_SubCharacter* scratcher);

void sharedFunc_800D26D8_5_s00(s_SubCharacter* scratcher);

void sharedFunc_800D2844_5_s00(s_SubCharacter* scratcher, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

void HangedScratcher_CollisionUpdate(s_SubCharacter* scratcher);

bool sharedFunc_800D3214_5_s00(s_SubCharacter* scratcher);

void sharedFunc_800D3300_5_s00(s_SubCharacter* scratcher);

#endif
