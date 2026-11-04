#ifndef _MAPS_CHARACTERS_PARASITE_H
#define _MAPS_CHARACTERS_PARASITE_H

/** @brief Parasite character control states. */
typedef enum _ParasiteControl
{
    ParasiteControl_None = 0,
    ParasiteControl_1    = 1
} e_ParasiteControl;

extern s_AnimInfo PARASITE_ANIM_INFOS[];

void Parasite_Update(s_SubCharacter* parasite, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

#endif
