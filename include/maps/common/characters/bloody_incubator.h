#ifndef _MAPS_CHARACTERS_BLOODYINCUBATOR_H
#define _MAPS_CHARACTERS_BLOODYINCUBATOR_H

/** @brief Bloody Incubator character control states. */
typedef enum _BloodyIncubatorControl
{
    BloodyIncubatorControl_None = 0,
    BloodyIncubatorControl_1    = 1,
    BloodyIncubatorControl_2    = 2,
    BloodyIncubatorControl_3    = 3,
    BloodyIncubatorControl_4    = 4,
    BloodyIncubatorControl_5    = 5,
    BloodyIncubatorControl_6    = 6,
    BloodyIncubatorControl_7    = 7,
    BloodyIncubatorControl_8    = 8,
    BloodyIncubatorControl_9    = 9,
    BloodyIncubatorControl_10   = 10,
    BloodyIncubatorControl_11   = 11
} e_BloodyIncubatorControl;

/** @brief Bloody Incubator character animation indices. */
typedef enum _BloodyIncubatorAnim
{
    BloodyIncubatorAnim_Still = 0,
    BloodyIncubatorAnim_1     = 1,
    BloodyIncubatorAnim_2     = 2,
    BloodyIncubatorAnim_3     = 3,
    BloodyIncubatorAnim_4     = 4,
    BloodyIncubatorAnim_5     = 5,
    BloodyIncubatorAnim_6     = 6,
    BloodyIncubatorAnim_7     = 7,
    BloodyIncubatorAnim_8     = 8,
    BloodyIncubatorAnim_9     = 9,
    BloodyIncubatorAnim_10    = 10,
    BloodyIncubatorAnim_11    = 11
} e_BloodyIncubatorAnim;

/** @brief Bloody Incubator character model bone indices. */
typedef enum _BloodyIncubatorBone
{
    BloodyIncubatorBone_Root = 0
} e_BloodyIncubatorBone;

/*s_AnimInfo BLOODY_INCUBATOR_ANIM_INFOS[] = {
    { Anim_BlendLinear, ANIM_STATUS(0,  false), false, ANIM_STATUS(0, false), { Q12(0) }, NO_VALUE, 0 },
    { Anim_PlayLoop, ANIM_STATUS(0,  true),  false, NO_VALUE, { Q12(30) }, NO_VALUE, 1 },
    { Anim_BlendLinear, ANIM_STATUS(1,  false), false, ANIM_STATUS(1, true), { Q12(10) }, NO_VALUE, 0 },
    { Anim_PlayOnce, ANIM_STATUS(1,  true),  false, ANIM_STATUS(1, true), { Q12(10) }, 0, 54 },
    { Anim_BlendLinear, ANIM_STATUS(2,  false), false, ANIM_STATUS(2, true), { Q12(10) }, NO_VALUE, 55 },
    { Anim_PlayOnce, ANIM_STATUS(2,  true),  false, ANIM_STATUS(2, true), { Q12(10) }, 55, 117 },
    { Anim_BlendLinear, ANIM_STATUS(3,  false), false, ANIM_STATUS(3, true), { Q12(10) }, NO_VALUE, 118 },
    { Anim_PlayOnce, ANIM_STATUS(3,  true),  false, ANIM_STATUS(3, true), { Q12(10) }, 118, 152 },
    { Anim_BlendLinear, ANIM_STATUS(4,  false), false, ANIM_STATUS(4, true), { Q12(10) }, NO_VALUE, 153 },
    { Anim_PlayOnce, ANIM_STATUS(4,  true),  false, ANIM_STATUS(4, true), { Q12(10) }, 153, 213 },
    { Anim_BlendLinear, ANIM_STATUS(5,  false), false, ANIM_STATUS(5, true), { Q12(10) }, NO_VALUE, 214 },
    { Anim_PlayLoop, ANIM_STATUS(5,  true),  false, NO_VALUE, { Q12(10) }, 214, 229 },
    { Anim_BlendLinear, ANIM_STATUS(6,  false), false, ANIM_STATUS(6, true), { Q12(10) }, NO_VALUE, 230 },
    { Anim_PlayOnce, ANIM_STATUS(6,  true),  false, ANIM_STATUS(6, true), { Q12(10) }, 230, 260 },
    { Anim_BlendLinear, ANIM_STATUS(7,  false), false, ANIM_STATUS(7, true), { Q12(10) }, NO_VALUE, 261 },
    { Anim_PlayLoop, ANIM_STATUS(7,  true),  false, NO_VALUE, { Q12(10) }, 261, 276 },
    { Anim_BlendLinear, ANIM_STATUS(8,  false), false, ANIM_STATUS(8, true), { Q12(10) }, NO_VALUE, 277 },
    { Anim_PlayOnce, ANIM_STATUS(8,  true),  false, ANIM_STATUS(8, true), { Q12(10) }, 277, 295 },
    { Anim_BlendLinear, ANIM_STATUS(9,  false), false, ANIM_STATUS(9, true), { Q12(10) }, NO_VALUE, 296 },
    { Anim_PlayOnce, ANIM_STATUS(9,  true),  false, ANIM_STATUS(9, true), { Q12(8) }, 296, 365 },
    { Anim_BlendLinear, ANIM_STATUS(10, false), false, ANIM_STATUS(10, true), { Q12(10) }, NO_VALUE, 366 },
    { Anim_PlayLoop, ANIM_STATUS(10, true),  false, NO_VALUE, { Q12(10) }, 366, 382 },
    { Anim_BlendLinear, ANIM_STATUS(11, false), false, ANIM_STATUS(11, true), { Q12(10) }, NO_VALUE, 295 },
    { Anim_PlayOnce, ANIM_STATUS(11, true),  false, ANIM_STATUS(11, true), { Q12(10) }, 295, 295 }
};*/

/** @brief Bloody Incubator character anim infos. */
extern s_AnimInfo BLOODY_INCUBATOR_ANIM_INFOS[];

void BloodyIncubator_Update(s_SubCharacter* bloodyIncubator, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

void BloodyIncubator_AnimUpdate(s_SubCharacter* bloodyIncubator, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

void func_800D3740(s_SubCharacter* bloodyIncubator, GsCOORDINATE2* boneCoords);

void func_800D38D8(s_SubCharacter* bloodyIncubator, GsCOORDINATE2* boneCoords);

void BloodyIncubator_Init(s_SubCharacter* bloodyIncubator);

#endif
