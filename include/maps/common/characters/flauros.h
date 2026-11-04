#ifndef _MAPS_CHARACTERS_FLAUROS_H
#define _MAPS_CHARACTERS_FLAUROS_H

/** @brief Flauros character control states. */
typedef enum _FlaurosControl
{
    FlaurosControl_None = 0,
    FlaurosControl_1    = 1
} e_FlaurosControl;

/** @brief Flauros character animation indices. */
typedef enum _FlaurosAnim
{
    FlaurosAnim_Still = 0,
    FlaurosAnim_1     = 1
} e_FlaurosAnim;

extern s_AnimInfo FLAUROS_ANIM_INFOS[];

void Flauros_Update(s_SubCharacter* flauros, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

#endif
