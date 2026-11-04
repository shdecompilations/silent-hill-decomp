#ifndef _MAPS_CHARACTERS_LISA_H
#define _MAPS_CHARACTERS_LISA_H

/** @brief Lisa character control states. */
typedef enum _LisaControl
{
    LisaControl_None = 0,
    LisaControl_1    = 1,
    LisaControl_2    = 2,
    LisaControl_3    = 3,
    LisaControl_4    = 4,
    LisaControl_5    = 5,
    LisaControl_6    = 6,
    LisaControl_7    = 7,
    LisaControl_8    = 8,
    LisaControl_9    = 9,
    LisaControl_10   = 10,
    LisaControl_11   = 11,
    LisaControl_12   = 12,
    LisaControl_13   = 13,
    LisaControl_14   = 14,
    LisaControl_15   = 15,
    LisaControl_16   = 16,
    LisaControl_17   = 17,
    LisaControl_18   = 18,
    LisaControl_19   = 19,
    LisaControl_20   = 20,
    LisaControl_21   = 21
} e_LisaControl;

/** @brief Lisa character animation indices. */
typedef enum _LisaAnim
{
    LisaAnim_Still = 0,
    LisaAnim_1     = 1,
    LisaAnim_2     = 2,
    LisaAnim_3     = 3,
    LisaAnim_4     = 4,
    LisaAnim_5     = 5,
    LisaAnim_6     = 6,
    LisaAnim_7     = 7,
    LisaAnim_8     = 8,
    LisaAnim_9     = 9,
    LisaAnim_10    = 10,
    LisaAnim_11    = 11,
    LisaAnim_12    = 12,
    LisaAnim_13    = 13,
    LisaAnim_14    = 14,
    LisaAnim_15    = 15,
    LisaAnim_16    = 16,
    LisaAnim_17    = 17
} e_LisaAnim;

/** @brief Lisa character model bone indices. */
typedef enum _LisaBone
{
    LisaBone_Root = 0
} e_LisaBone;

/** @brief Lisa character anim infos. */
extern s_AnimInfo LISA_ANIM_INFOS[];

/*s_AnimInfo LISA_ANIM_INFOS[] = {
    { Anim_BlendLinear, ANIM_STATUS(LisaAnim_0,  false), false, ANIM_STATUS(LisaAnim_0,  false), { Q12(0.0f)    }, NO_VALUE, 0   },
    { Anim_PlayLoop, ANIM_STATUS(LisaAnim_0,  true),  false, NO_VALUE,               { Q12(30.0f)   }, NO_VALUE, 1   },
    { Anim_BlendLinear, ANIM_STATUS(LisaAnim_1,  false), false, ANIM_STATUS(LisaAnim_1,  true),  { Q12(2.0f)    }, NO_VALUE, 0   },
    { Anim_PlayLoop, ANIM_STATUS(LisaAnim_1,  true),  false, NO_VALUE,               { Q12(5.0f)    }, 0,        9   },
    { Anim_BlendLinear, ANIM_STATUS(LisaAnim_2,  false), false, ANIM_STATUS(LisaAnim_2,  true),  { Q12(5.0f)    }, NO_VALUE, 10  },
    { Anim_PlayLoop, ANIM_STATUS(LisaAnim_2,  true),  false, NO_VALUE,               { Q12(20.0f)   }, 10,       37  },
    { Anim_BlendLinear, ANIM_STATUS(LisaAnim_3,  false), false, ANIM_STATUS(LisaAnim_3,  true),  { Q12(10.0f)   }, NO_VALUE, 38  },
    { Anim_PlayOnce, ANIM_STATUS(LisaAnim_3,  true),  false, ANIM_STATUS(LisaAnim_3,  true),  { Q12(1.5f)    }, 38,       58  },
    { Anim_BlendLinear, ANIM_STATUS(LisaAnim_4,  false), false, ANIM_STATUS(LisaAnim_4,  true),  { Q12(10.0f)   }, NO_VALUE, 59  },
    { Anim_PlayOnce, ANIM_STATUS(LisaAnim_4,  true),  false, ANIM_STATUS(LisaAnim_4,  true),  { Q12(5.0f)    }, 59,       72  },
    { Anim_BlendLinear, ANIM_STATUS(LisaAnim_5,  false), false, ANIM_STATUS(LisaAnim_5,  true),  { Q12(10.0f)   }, NO_VALUE, 73  },
    { Anim_PlayOnce, ANIM_STATUS(LisaAnim_5,  true),  false, ANIM_STATUS(LisaAnim_5,  true),  { Q12(9.0f)    }, 73,       189 },
    { Anim_BlendLinear, ANIM_STATUS(LisaAnim_6,  false), false, ANIM_STATUS(LisaAnim_6,  true),  { Q12(5.0f)    }, NO_VALUE, 190 },
    { Anim_PlayLoop, ANIM_STATUS(LisaAnim_6,  true),  false, NO_VALUE,               { Q12(5.0f)    }, 190,      205 },
    { Anim_BlendLinear, ANIM_STATUS(LisaAnim_7,  false), false, ANIM_STATUS(LisaAnim_7,  true),  { Q12(10.0f)   }, NO_VALUE, 206 },
    { Anim_PlayOnce, ANIM_STATUS(LisaAnim_7,  true),  false, ANIM_STATUS(LisaAnim_7,  true),  { Q12(10.0f)   }, 206,      227 },
    { Anim_BlendLinear, ANIM_STATUS(LisaAnim_8,  false), false, ANIM_STATUS(LisaAnim_8,  true),  { Q12(10.0f)   }, NO_VALUE, 228 },
    { Anim_PlayOnce, ANIM_STATUS(LisaAnim_8,  true),  false, ANIM_STATUS(LisaAnim_8,  true),  { Q12(10.0f)   }, 228,      248 },
    { Anim_BlendLinear, ANIM_STATUS(LisaAnim_9,  false), false, ANIM_STATUS(LisaAnim_9,  true),  { Q12(10.0f)   }, NO_VALUE, 249 },
    { Anim_PlayOnce, ANIM_STATUS(LisaAnim_9,  true),  false, ANIM_STATUS(LisaAnim_9,  true),  { Q12(8.0f)    }, 249,      283 },
    { Anim_BlendLinear, ANIM_STATUS(LisaAnim_10, false), false, ANIM_STATUS(LisaAnim_10, true),  { Q12(10.0f)   }, NO_VALUE, 284 },
    { Anim_PlayOnce, ANIM_STATUS(LisaAnim_10, true),  false, ANIM_STATUS(LisaAnim_10, true),  { Q12(10.0f)   }, 284,      353 },
    { Anim_BlendLinear, ANIM_STATUS(LisaAnim_11, false), false, ANIM_STATUS(LisaAnim_11, true),  { Q12(10.0f)   }, NO_VALUE, 354 },
    { Anim_PlayOnce, ANIM_STATUS(LisaAnim_11, true),  false, ANIM_STATUS(LisaAnim_11, true),  { Q12(3.5f)    }, 354,      403 },
    { Anim_BlendLinear, ANIM_STATUS(LisaAnim_12, false), false, ANIM_STATUS(LisaAnim_12, true),  { Q12(10.0f)   }, NO_VALUE, 404 },
    { Anim_PlayOnce, ANIM_STATUS(LisaAnim_12, true),  false, ANIM_STATUS(LisaAnim_12, true),  { Q12(8.0f)    }, 404,      415 },
    { Anim_BlendLinear, ANIM_STATUS(LisaAnim_13, false), false, ANIM_STATUS(LisaAnim_13, true),  { Q12(8.0f)    }, NO_VALUE, 416 },
    { Anim_PlayOnce, ANIM_STATUS(LisaAnim_13, true),  false, ANIM_STATUS(LisaAnim_13, true),  { Q12(7.5f)    }, 416,      436 },
    { Anim_BlendLinear, ANIM_STATUS(LisaAnim_14, false), false, ANIM_STATUS(LisaAnim_14, true),  { Q12(8.0f)    }, NO_VALUE, 437 },
    { Anim_PlayOnce, ANIM_STATUS(LisaAnim_14, true),  false, ANIM_STATUS(LisaAnim_14, true),  { Q12(10.0f)   }, 437,      547 },
    { Anim_BlendLinear, ANIM_STATUS(LisaAnim_15, false), false, ANIM_STATUS(LisaAnim_15, true),  { Q12(10.0f)   }, NO_VALUE, 548 },
    { Anim_PlayOnce, ANIM_STATUS(LisaAnim_15, true),  false, ANIM_STATUS(LisaAnim_15, true),  { Q12(10.0f)   }, 548,      572 },
    { Anim_BlendLinear, ANIM_STATUS(LisaAnim_16, false), false, ANIM_STATUS(LisaAnim_16, true),  { Q12(10.0f)   }, NO_VALUE, 573 },
    { Anim_PlayOnce, ANIM_STATUS(LisaAnim_16, true),  false, ANIM_STATUS(LisaAnim_16, true),  { Q12(10.0f)   }, 573,      602 },
    { Anim_BlendLinear, ANIM_STATUS(LisaAnim_17, false), false, ANIM_STATUS(LisaAnim_17, true),  { Q12(2.0f)    }, NO_VALUE, 603 },
    { Anim_PlayLoop, ANIM_STATUS(LisaAnim_17, true),  false, NO_VALUE,               { Q12(5.0f)    }, 603,      618 }
};*/

extern s32 sharedData_800D6BB8_3_s04;

void Lisa_Update(s_SubCharacter* lisa, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);
void Lisa_AnimUpdate(s_SubCharacter* lisa, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);
void Lisa_MovementUpdate(s_SubCharacter* lisa, GsCOORDINATE2* boneCoords);
void Lisa_AnimStateUpdate(s_SubCharacter* lisa, GsCOORDINATE2* boneCoords);
void Lisa_Init(s_SubCharacter* lisa);

#endif
