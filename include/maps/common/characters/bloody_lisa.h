#ifndef _MAPS_CHARACTERS_BLOODYLISA_H
#define _MAPS_CHARACTERS_BLOODYLISA_H

/** @brief Bloody Lisa character control states. */
typedef enum _BloodyLisaControl
{
    BloodyLisaControl_None = 0,
    BloodyLisaControl_1    = 1,
    BloodyLisaControl_2    = 2,
    BloodyLisaControl_3    = 3,
    BloodyLisaControl_4    = 4,
    BloodyLisaControl_5    = 5,
    BloodyLisaControl_6    = 6
} e_BloodyLisaControl;

/** @brief Bloody Lisa character animation indices. */
typedef enum _BloodyLisaAnim
{
    BloodyLisaAnim_Still = 0,
    BloodyLisaAnim_Walk  = 1
} e_BloodyLisaAnim;

/*s_AnimInfo BLOODY_LISA_ANIM_INFOS[] = {
    { Anim_BlendLinear, ANIM_STATUS(BloodyLisaAnim_Still, false), false, ANIM_STATUS(BloodyLisaAnim_Still, false), { Q12(0.0f)  }, NO_VALUE, 0   },
    { Anim_PlayLoop, ANIM_STATUS(BloodyLisaAnim_Still, true),  false, NO_VALUE,                                 { Q12(30.0f) }, NO_VALUE, 1   },
    { Anim_BlendLinear, ANIM_STATUS(BloodyLisaAnim_Walk,  false), false, ANIM_STATUS(BloodyLisaAnim_Walk,  true),  { Q12(10.0f) }, NO_VALUE, 0   },
    { Anim_PlayOnce, ANIM_STATUS(BloodyLisaAnim_Walk,  true),  false, ANIM_STATUS(BloodyLisaAnim_Walk,  true),  { Q12(10.0f) }, 0,        100 }
};*/

/** @brief Bloody Lisa character anim infos. */
extern s_AnimInfo BLOODY_LISA_ANIM_INFOS[];

extern s32 sharedData_800E2C38_7_s01;

void BloodyLisa_Update(s_SubCharacter* bloodyLisa, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);
void BloodyLisa_AnimUpdate(s_SubCharacter* bloodyLisa, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);
void BloodyLisa_MovementUpdate(s_SubCharacter* bloodyLisa, GsCOORDINATE2* boneCoords);
void BloodyLisa_AnimStateUpdate(s_SubCharacter* bloodyLisa, GsCOORDINATE2* boneCoords);
void BloodyLisa_Init(s_SubCharacter* bloodyLisa);

#endif
