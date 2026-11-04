#ifndef _MAPS_CHARACTERS_CAT_H
#define _MAPS_CHARACTERS_CAT_H

/** @brief Cat character control states. */
typedef enum _CatControl
{
    CatControl_None = 0,
    CatControl_1    = 1,
    CatControl_2    = 2,
    CatControl_3    = 3
} e_CatControl;

/** @brief Cat character animation indices. */
typedef enum _CatAnim
{
    CatAnim_Still      = 0,
    CatAnim_Jump       = 1,
    CatAnim_Run        = 2,
    CatAnim_IdleToJump = 3,
    CatAnim_JumpToRun  = 4
} e_CatAnim;

/** @brief Cat character anim infos.
 * @unused Frames `[1, 6]`.
 */
/*s_AnimInfo CAT_ANIM_INFOS[] = {
    { Anim_BlendLinear,  NO_VALUE,                               false, ANIM_STATUS(CatAnim_Still, false),     { Q12(0.0f)  }, NO_VALUE, 0  },
    { NULL,              ANIM_STATUS(CatAnim_Still, false),      false, ANIM_STATUS(CatAnim_Still, false),     { Q12(0.0f)  }, 0,        0  },
    { Anim_BlendLinear,  ANIM_STATUS(CatAnim_Jump, false),       false, ANIM_STATUS(CatAnim_Jump, true),       { Q12(64.0f) }, NO_VALUE, 7  },
    { Anim_PlayOnce, ANIM_STATUS(CatAnim_Jump, true),        false, ANIM_STATUS(CatAnim_JumpToRun, false), { Q12(15.8f) }, 7,        22 },
    { Anim_BlendLinear,  ANIM_STATUS(CatAnim_Run, false),        false, ANIM_STATUS(CatAnim_Run, true),        { Q12(64.0f) }, NO_VALUE, 23 },
    { Anim_PlayLoop, ANIM_STATUS(CatAnim_Run, true),         false, NO_VALUE,                              { Q12(35.0f) }, 23,       43 },
    { Anim_BlendLinear,  ANIM_STATUS(CatAnim_IdleToJump, false), false, ANIM_STATUS(CatAnim_IdleToJump, true), { Q12(0.0f)  }, NO_VALUE, 7  },
    { Anim_PlayLoop, ANIM_STATUS(CatAnim_IdleToJump, true),  false, NO_VALUE,                              { Q12(0.0f)  }, 7,        8  },
    { Anim_BlendLinear,  ANIM_STATUS(CatAnim_JumpToRun, false),  false, ANIM_STATUS(CatAnim_JumpToRun, true),  { Q12(0.0f)  }, NO_VALUE, 22 },
    { Anim_PlayLoop, ANIM_STATUS(CatAnim_JumpToRun, true),   false, NO_VALUE,                              { Q12(0.0f)  }, 22,       23 }
};*/

extern s_AnimInfo CAT_ANIM_INFOS[]; // 0x800DC924

/** @brief Updates an active Cat Doctor character.
 *
 * @param cat Cat character to update.
 * @param anmHdr Character animation data.
 * @param boneCoords Character model bone coords.
 */
void Cat_Update(s_SubCharacter* cat, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

#endif
