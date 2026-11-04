#ifndef _MAPS_CHARACTERS_GHOSTCHILDALESSA_H
#define _MAPS_CHARACTERS_GHOSTCHILDALESSA_H

/** @brief Ghost Child Alessa character control states. */
typedef enum _GhostChildAlessaControl
{
    GhostChildAlessaControl_None = 0,
    GhostChildAlessaControl_1    = 1,
    GhostChildAlessaControl_2    = 2,
    GhostChildAlessaControl_3    = 3,
    GhostChildAlessaControl_4    = 4,
    GhostChildAlessaControl_5    = 5,
    GhostChildAlessaControl_6    = 6,
    GhostChildAlessaControl_7    = 7,
    GhostChildAlessaControl_8    = 8,
    GhostChildAlessaControl_9    = 9,
    GhostChildAlessaControl_10   = 10,
    GhostChildAlessaControl_11   = 11
} e_GhostChildAlessaControl;

/** @brief Ghost Child Alessa character animation indices. */
typedef enum _GhostChildAlessaAnim
{
    GhostChildAlessaAnim_Still = 0,
    GhostChildAlessaAnim_1     = 1,
    GhostChildAlessaAnim_2     = 2,
    GhostChildAlessaAnim_3     = 3,
    GhostChildAlessaAnim_4     = 4,
    GhostChildAlessaAnim_5     = 5,
    GhostChildAlessaAnim_6     = 6,
    GhostChildAlessaAnim_7     = 7,
    GhostChildAlessaAnim_8     = 8
} e_GhostChildAlessaAnim;

/** @brief Ghost Child Alessa model bone indices. */
typedef enum _GhostChildAlessaBone
{
    GhostChildAlessaBone_Root = 0
} e_GhostChildAlessaBone;

/** @brief Ghost Child Alessa character anim infos. */
extern s_AnimInfo GHOST_CHILD_ALESSA_ANIM_INFOS[];

/*s_AnimInfo GHOST_CHILD_ALESSA_ANIM_INFOS[] = {
    { Anim_BlendLinear, ANIM_STATUS(GhostChildAlessaAnim_Still, false), false, ANIM_STATUS(GhostChildAlessaAnim_Still, false), { Q12(0.0f) }, NO_VALUE, 0 },
    { Anim_PlayLoop, ANIM_STATUS(GhostChildAlessaAnim_Still, true), false, NO_VALUE, { Q12(30.0f) }, NO_VALUE, 1 },
    { Anim_BlendLinear, ANIM_STATUS(GhostChildAlessaAnim_1, false), false, ANIM_STATUS(GhostChildAlessaAnim_1, true), { Q12(5.0f) }, NO_VALUE, 0 },
    { Anim_PlayLoop, ANIM_STATUS(GhostChildAlessaAnim_1, true), false, NO_VALUE, { Q12(15.0f) }, 0, 27 },
    { Anim_BlendLinear, ANIM_STATUS(GhostChildAlessaAnim_2, false), false, ANIM_STATUS(2, true), { Q12(5.0f) }, NO_VALUE, 28 },
    { Anim_PlayLoop, ANIM_STATUS(GhostChildAlessaAnim_2, true), false, NO_VALUE, { Q12(15.0f) }, 28, 49 },
    { Anim_BlendLinear, ANIM_STATUS(GhostChildAlessaAnim_3, false), false, ANIM_STATUS(GhostChildAlessaAnim_3, true), { Q12(10.0f) }, NO_VALUE, 50 },
    { Anim_PlayOnce, ANIM_STATUS(GhostChildAlessaAnim_3, true), false, ANIM_STATUS(GhostChildAlessaAnim_3, true), { Q12(10.0f) }, 50, 63 },
    { Anim_BlendLinear, ANIM_STATUS(GhostChildAlessaAnim_4, false), false, ANIM_STATUS(GhostChildAlessaAnim_4, true), { Q12(10.0f) }, NO_VALUE, 64 },
    { Anim_PlayLoop, ANIM_STATUS(GhostChildAlessaAnim_4, true), false, NO_VALUE, { Q12(10.0f) }, 64, 83 },
    { Anim_BlendLinear, ANIM_STATUS(GhostChildAlessaAnim_5, false), false, ANIM_STATUS(GhostChildAlessaAnim_5, true), { Q12(10.0f) }, NO_VALUE, 84 },
    { Anim_PlayOnce, ANIM_STATUS(GhostChildAlessaAnim_5, true), false, ANIM_STATUS(GhostChildAlessaAnim_5, true), { Q12(10.0f) }, 84, 109 },
    { Anim_BlendLinear, ANIM_STATUS(GhostChildAlessaAnim_6, false), false, ANIM_STATUS(GhostChildAlessaAnim_6, true), { Q12(5.0f) }, NO_VALUE, 110 },
    { Anim_PlayLoop, ANIM_STATUS(GhostChildAlessaAnim_6, true), false, NO_VALUE, { Q12(5.0f) }, 110, 111 },
    { Anim_BlendLinear, ANIM_STATUS(GhostChildAlessaAnim_7, false), false, ANIM_STATUS(GhostChildAlessaAnim_7, true), { Q12(5.0f) }, NO_VALUE, 112 },
    { Anim_PlayLoop, ANIM_STATUS(GhostChildAlessaAnim_7, true), false, NO_VALUE, { Q12(5.0f) }, 112, 127 },
    { Anim_BlendLinear, ANIM_STATUS(GhostChildAlessaAnim_8, false), false, ANIM_STATUS(GhostChildAlessaAnim_8, true), { Q12(5.0f) }, NO_VALUE, 128 },
    { Anim_PlayLoop, ANIM_STATUS(GhostChildAlessaAnim_8, true), false, NO_VALUE, { Q12(5.0f) }, 128, 137 }
};*/

void GhostChildAlessa_Update(s_SubCharacter* ghostAlessa, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);
void GhostChildAlessa_AnimUpdate(s_SubCharacter* ghostAlessa, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);
void GhostChildAlessa_MovementUpdate(s_SubCharacter* ghostAlessa, GsCOORDINATE2* boneCoords);
void GhostChildAlessa_AnimStateUpdate(s_SubCharacter* ghostAlessa, GsCOORDINATE2* boneCoords);
void GhostChildAlessa_Init(s_SubCharacter* ghostAlessa);

#endif
