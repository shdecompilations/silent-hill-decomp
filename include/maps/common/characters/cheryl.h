#ifndef _MAPS_CHARACTERS_CHERYL_H
#define _MAPS_CHARACTERS_CHERYL_H

/** @brief Cheryl character flags. */
typedef enum _CherylFlags
{
    CherylFlag_None          = 0,
    CherylFlag_FootstepRight = 1 << 4,
    CherylFlag_FootstepLeft  = 1 << 5
} e_CherylFlags;

/** @brief Cheryl control states. */
typedef enum _CherylControl
{
    CherylControl_Idle        = 0,
    CherylControl_WalkForward = 1,
    CherylControl_RunForward  = 2,
    CherylControl_3           = 3, // Special walk forward.
    CherylControl_4           = 4  // Special walk forward.
} e_CherylControl;

/** @brief Cheryl character animation indices. */
typedef enum _CherylAnim
{
    CherylAnim_Still       = 0,
    CherylAnim_Idle        = 1,
    CherylAnim_WalkForward = 2,
    CherylAnim_RunForward  = 3
} e_CherylAnim;

/** @brief Cheryl character anim infos. */
extern s_AnimInfo CHERYL_ANIM_INFOS[];

extern s32 D_800DF1CC;

/** Timer. */
extern q19_12 D_800E3A30;

void Cheryl_Update(s_SubCharacter* cheryl, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);
void Cheryl_AnimUpdate(s_SubCharacter* cheryl, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);
void Cheryl_MovementUpdate(s_SubCharacter* cheryl, GsCOORDINATE2* boneCoords);
void Cheryl_ControlUpdate(s_SubCharacter* cheryl, GsCOORDINATE2* boneCoords);
bool Cheryl_FootstepSfxPlay(s32 animStatus, s_SubCharacter* cheryl,
                            s32 leftFootstepKeyframeIdx, s32 rightFootstepKeyframeIdx, q23_8 distToPlayerSqr, s32 pitch);
void Cheryl_Init(s_SubCharacter* cheryl);

#endif
