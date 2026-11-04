#ifndef _MAPS_CHARACTERS_LITTLEINCUBUS_H
#define _MAPS_CHARACTERS_LITTLEINCUBUS_H

/** @brief Little Incubus character control states. */
typedef enum _LitteIncubusControl
{
    LitteIncubusControl_None = 0,
    LitteIncubusControl_1    = 1
} e_LitteIncubusControl;

/** @brief Little Incubus character animation indices. */
typedef enum _LitteIncubusAnim
{
    LitteIncubusAnim_Still = 0,
    LitteIncubusAnim_1     = 1
} e_LitteIncubusAnim;

/** @brief Little Incubus character model bone indices. */
typedef enum _LittleIncubusBone
{
    LittleIncubusBone_1 = 1,
    LittleIncubusBone_7 = 7
} e_LittleIncubusBone;

extern s_AnimInfo LITTLE_INCUBUS_ANIM_INFOS[];

extern s32 D_800EDA00;

void LittleIncubus_Update(s_SubCharacter* incubus, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

#endif
