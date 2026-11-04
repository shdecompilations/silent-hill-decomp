#ifndef _MAPS_CHARACTERS_LOCKERDEADBODY_H
#define _MAPS_CHARACTERS_LOCKERDEADBODY_H

/** @brief Locker Dead Body character animation indices. */
typedef enum _LockerDeadBodyAnim
{
    LockerDeadBodyAnim_Still = 0,
    LockerDeadBodyAnim_1     = 1,
    LockerDeadBodyAnim_2     = 2,
    LockerDeadBodyAnim_3     = 3
} e_LockerDeadBodyAnim;

extern s_AnimInfo LOCKER_DEAD_BODY_ANIM_INFOS[8];

void LockerDeadBody_Update(s_SubCharacter* deadBody, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

#endif
