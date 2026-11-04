#ifndef _MAPS_CHARACTERS_BLOODSUCKER_H
#define _MAPS_CHARACTERS_BLOODSUCKER_H

/** @brief Bloodsucker character flags. */
typedef enum _BloodsuckerFlags
{
    BloodsuckerFlag_None = 0,
    BloodsuckerFlag_0    = 1 << 0,
    BloodsuckerFlag_1    = 1 << 1
} e_BloodsuckerFlags;

/** @brief Bloodsucker character control states. */
typedef enum _BloodsuckerControl
{
    BloodsuckerControl_None = 0,
    BloodsuckerControl_1    = 1,
    BloodsuckerControl_2    = 2,
    BloodsuckerControl_3    = 3,
    BloodsuckerControl_4    = 4
} e_BloodsuckerControl;

/** @brief Bloodsucker character animation indices. */
typedef enum _BloodsuckerAnim
{
    BloodsuckerAnim_Still = 0,
    BloodsuckerAnim_1     = 1,
    BloodsuckerAnim_2     = 2,
    BloodsuckerAnim_3     = 3,
    BloodsuckerAnim_4     = 4,
    BloodsuckerAnim_5     = 5,
    BloodsuckerAnim_6     = 6,
    BloodsuckerAnim_7     = 7
} e_BloodsuckerAnim;

extern s_AnimInfo BLOODSUCKER_ANIM_INFOS[];

void Bloodsucker_Update(s_SubCharacter* bloodsucker, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

void Bloodsucker_Init(s_SubCharacter* bloodsucker);

void Bloodsucker_Control_1(s_SubCharacter* bloodsucker);

void Bloodsucker_Control_2(s_SubCharacter* bloodsucker);

void Bloodsucker_Control_3(s_SubCharacter* bloodsucker);

void Bloodsucker_Control_4(s_SubCharacter* bloodsucker);

void sharedFunc_800D0F28_3_s03(s_SubCharacter* bloodsucker, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

#endif
