#ifndef _MAPS_CHARACTERS_INCUBATOR_H
#define _MAPS_CHARACTERS_INCUBATOR_H

/** @brief Incubator character control states. */
typedef enum _IncubatorControl
{
    IncubatorControl_None = 0,
    IncubatorControl_1    = 1,
    IncubatorControl_2    = 2,
    IncubatorControl_3    = 3
} e_IncubatorControl;

/** @brief Incubator character animation indices. */
typedef enum _IncubatorAnim
{
    IncubatorAnim_Still = 0,
    IncubatorAnim_1     = 1,
    IncubatorAnim_2     = 2,
    IncubatorAnim_3     = 3
} e_IncubatorAnim;

/** @brief Incubator character model bone indices. */
typedef enum _IncubatorBone
{
    IncubatorBone_Root = 0
} e_IncubatorBone;

/*s_AnimInfo INCUBATOR_ANIM_INFOS[] = {
    { Anim_BlendLinear, ANIM_STATUS(0, false), false, ANIM_STATUS(0, false), { Q12(0)    }, NO_VALUE, 0   },
    { Anim_PlayLoop, ANIM_STATUS(0, true),  false, NO_VALUE,              { Q12(30)   }, NO_VALUE, 1   },
    { Anim_BlendLinear, ANIM_STATUS(1, false), false, ANIM_STATUS(1, true),  { Q12(10)   }, NO_VALUE, 0   },
    { Anim_PlayOnce, ANIM_STATUS(1, true),  false, ANIM_STATUS(1, true),  { Q12(6.1f) }, 0,        74  },
    { Anim_BlendLinear, ANIM_STATUS(2, false), false, ANIM_STATUS(2, true),  { Q12(10)   }, NO_VALUE, 75  },
    { Anim_PlayOnce, ANIM_STATUS(2, true),  false, ANIM_STATUS(2, true),  { Q12(10)   }, 75,       115 },
    { Anim_BlendLinear, ANIM_STATUS(3, false), false, ANIM_STATUS(3, true),  { Q12(10)   }, NO_VALUE, 116 },
    { Anim_PlayLoop, ANIM_STATUS(3, true),  false, NO_VALUE,              { Q12(10)   }, 116,      132 }
};*/

/** @brief Bloody Incubator character anim infos. */
extern s_AnimInfo INCUBATOR_ANIM_INFOS[];

void Incubator_Update(s_SubCharacter* incubator, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

void Incubator_AnimUpdate(s_SubCharacter* incubator, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

// TODO: Rename to `Incubatro_MovementUpdate`
void func_800D3C80(s_SubCharacter* incubator, GsCOORDINATE2* boneCoords);

// TODO: Rename to `Incubatro_ControlUpdate`
void func_800D3E18(s_SubCharacter* incubator, GsCOORDINATE2* boneCoords);

void Incubator_Init(s_SubCharacter* incubator);

#endif
