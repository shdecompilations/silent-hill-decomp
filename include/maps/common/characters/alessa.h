#ifndef _MAPS_CHARACTERS_ALESSA_H
#define _MAPS_CHARACTERS_ALESSA_H

/** @brief Alessa character control states. */
typedef enum _AlessaControl
{
    AlessaControl_None = 0,
    AlessaControl_1    = 1,
    AlessaControl_2    = 2,
    AlessaControl_3    = 3,
    AlessaControl_4    = 4,
    AlessaControl_5    = 5,
    AlessaControl_6    = 6,
    AlessaControl_7    = 7,
    AlessaControl_8    = 8,
    AlessaControl_9    = 9,
    AlessaControl_10   = 10
} e_AlessaControl;

/** @brief Alessa character animation indices. */
typedef enum _AlessaAnim
{
    AlessaAnim_Still                  = 0,
    AlessaAnim_StandIdle              = 1,
    AlessaAnim_WalkForward            = 2,
    AlessaAnim_WalkForwardTurnStumble = 3,
    AlessaAnim_ForcePush              = 4,
    AlessaAnim_StumbleForwardCrumple  = 5,
    AlessaAnim_CrumpleLookUp          = 6,
    AlessaAnim_CrumpleShakeHead       = 7,
    AlessaAnim_StandLookRight         = 8,
    AlessaAnim_CrumpleIdle            = 9,
    AlessaAnim_Kneel                  = 10
} e_AlessaAnim;

/** @brief Alessa character bone indices. */
typedef enum _AlessaBone
{
    AlessaBone_Root = 0
} e_AlessaBone;

/** @brief Alessa character anim infos. */
extern s_AnimInfo ALESSA_ANIM_INFOS[];

/*s_AnimInfo ALESSA_ANIM_INFOS[] = {
    { Anim_BlendLinear, ANIM_STATUS(AlessaAnim_Still,                  false), false, ANIM_STATUS(AlessaAnim_Still,                  false), { Q12(0.0f)  }, NO_VALUE, 0   },
    { Anim_PlayLoop, ANIM_STATUS(AlessaAnim_Still,                  true),  false, NO_VALUE,                                              { Q12(30.0f) }, NO_VALUE, 1   },
    { Anim_BlendLinear, ANIM_STATUS(AlessaAnim_StandIdle,              false), false, ANIM_STATUS(AlessaAnim_Kneel,                  true),  { Q12(5.0f)  }, NO_VALUE, 0   },
    { Anim_PlayLoop, ANIM_STATUS(AlessaAnim_StandIdle,              true),  false, NO_VALUE,                                              { Q12(5.0f)  }, 0,        15  },
    { Anim_BlendLinear, ANIM_STATUS(AlessaAnim_WalkForward,            false), false, ANIM_STATUS(AlessaAnim_Kneel,                  true),  { Q12(5.0f)  }, NO_VALUE, 16  },
    { Anim_PlayLoop, ANIM_STATUS(AlessaAnim_WalkForward,            true),  false, NO_VALUE,                                              { Q12(15.0f) }, 16,       44  },
    { Anim_BlendLinear, ANIM_STATUS(AlessaAnim_WalkForwardTurnStumble, false), false, ANIM_STATUS(AlessaAnim_WalkForwardTurnStumble, true),  { Q12(10.0f) }, NO_VALUE, 45  },
    { Anim_PlayOnce, ANIM_STATUS(AlessaAnim_WalkForwardTurnStumble, true),  false, ANIM_STATUS(AlessaAnim_WalkForwardTurnStumble, true),  { Q12(10.0f) }, 45,       131 },
    { Anim_BlendLinear, ANIM_STATUS(AlessaAnim_ForcePush,              false), false, ANIM_STATUS(AlessaAnim_ForcePush,              true),  { Q12(20.0f) }, NO_VALUE, 132 },
    { Anim_PlayOnce, ANIM_STATUS(AlessaAnim_ForcePush,              true),  false, ANIM_STATUS(AlessaAnim_ForcePush,              true),  { Q12(10.0f) }, 132,      170 },
    { Anim_BlendLinear, ANIM_STATUS(AlessaAnim_StumbleForwardCrumple,  false), false, ANIM_STATUS(AlessaAnim_StumbleForwardCrumple,  true),  { Q12(10.0f) }, NO_VALUE, 171 },
    { Anim_PlayOnce, ANIM_STATUS(AlessaAnim_StumbleForwardCrumple,  true),  false, ANIM_STATUS(AlessaAnim_StumbleForwardCrumple,  true),  { Q12(10.0f) }, 171,      216 },
    { Anim_BlendLinear, ANIM_STATUS(AlessaAnim_CrumpleLookUp,          false), false, ANIM_STATUS(AlessaAnim_CrumpleLookUp,          true),  { Q12(1.0f)  }, NO_VALUE, 217 },
    { Anim_PlayOnce, ANIM_STATUS(AlessaAnim_CrumpleLookUp,          true),  false, ANIM_STATUS(AlessaAnim_CrumpleLookUp,          true),  { Q12(10.0f) }, 217,      237 },
    { Anim_BlendLinear, ANIM_STATUS(AlessaAnim_CrumpleShakeHead,       true),  false, ANIM_STATUS(AlessaAnim_CrumpleShakeHead,       true),  { Q12(1.0f)  }, NO_VALUE, 238 },
    { Anim_PlayOnce, ANIM_STATUS(AlessaAnim_CrumpleShakeHead,       true),  false, ANIM_STATUS(AlessaAnim_CrumpleShakeHead,       true),  { Q12(10.0f) }, 238,      288 },
    { Anim_BlendLinear, ANIM_STATUS(AlessaAnim_StandLookRight,         false), false, ANIM_STATUS(AlessaAnim_StandLookRight,         true),  { Q12(10.0f) }, NO_VALUE, 289 },
    { Anim_PlayOnce, ANIM_STATUS(AlessaAnim_StandLookRight,         true),  false, ANIM_STATUS(AlessaAnim_StandLookRight,         true),  { Q12(10.0f) }, 289,      309 },
    { Anim_BlendLinear, ANIM_STATUS(AlessaAnim_CrumpleIdle,            false), false, ANIM_STATUS(AlessaAnim_CrumpleIdle,            true),  { Q12(5.0f)  }, NO_VALUE, 310 },
    { Anim_PlayLoop, ANIM_STATUS(AlessaAnim_CrumpleIdle,            true),  false, NO_VALUE,                                              { Q12(3.0f)  }, 310,      325 },
    { Anim_BlendLinear, ANIM_STATUS(AlessaAnim_Kneel,                  false), false, ANIM_STATUS(AlessaAnim_Kneel,                  true),  { Q12(5.0f)  }, NO_VALUE, 326 },
    { Anim_PlayLoop, ANIM_STATUS(AlessaAnim_Kneel,                  true),  false, NO_VALUE,                                              { Q12(3.0f)  }, 326,      341 }
};*/

extern s32 sharedData_800D3150_3_s02;

void Alessa_Update(s_SubCharacter* alessa, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

void Alessa_AnimUpdate(s_SubCharacter* alessa, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

void Alessa_MovementUpdate(s_SubCharacter* alessa, GsCOORDINATE2* boneCoords);

void Alessa_AnimStateUpdate(s_SubCharacter* alessa, GsCOORDINATE2* boneCoords);

/** @brief Initializes an Alessa character.
 *
 * @param alessa Alessa character.
 */
void Alessa_Init(s_SubCharacter* alessa);

#endif
