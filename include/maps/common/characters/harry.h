#ifndef _MAPS_CHARACTERS_HARRY_H
#define _MAPS_CHARACTERS_HARRY_H

/** @brief Harry character animation indices. */
typedef enum _HarryAnim
{
    HarryAnim_Still                   = 0,
    HarryAnim_TransitionToStill       = 1,
    HarryAnim_WalkForward             = 2,
    HarryAnim_RunForward              = 3,
    HarryAnim_WalkBackward            = 4,
    HarryAnim_SidestepLeft            = 5,
    HarryAnim_SidestepRight           = 6,
    HarryAnim_RunLeft                 = 7,
    HarryAnim_RunRight                = 8,
    HarryAnim_RunForwardWallStopRight = 9,
    HarryAnim_RunForwardWallStopLeft  = 10,
    HarryAnim_RunForwardStumble       = 11,
    HarryAnim_TurnLeft                = 12,
    HarryAnim_TurnRight               = 13,
    HarryAnim_QuickTurnRight          = 14,
    HarryAnim_QuickTurnLeft           = 15,
    HarryAnim_JumpBackward            = 16,
    HarryAnim_LookAround              = 17,
    HarryAnim_RunLeftWallStop         = 18,
    HarryAnim_RunLeftStumble          = 19,
    HarryAnim_RunRightWallStop        = 20,
    HarryAnim_RunRightStumble         = 21,
    HarryAnim_FallForward             = 22,
    HarryAnim_FallBackward            = 23,
    HarryAnim_Kick                    = 24,
    HarryAnim_Stomp                   = 25,
    HarryAnim_Idle                    = 26,
    HarryAnim_IdleExhausted           = 27,
    HarryAnim_HandgunAim              = 28,
    HarryAnim_Unk29                   = 29, // } Handgun.
    HarryAnim_Unk30                   = 30, // }
    HarryAnim_HandgunRecoil           = 31,
    HarryAnim_Unk32                   = 32,
    HarryAnim_Unk33                   = 33,
    HarryAnim_Unk34                   = 34,
    HarryAnim_Unk35                   = 35,
    HarryAnim_Unk36                   = 36,
    HarryAnim_Unk37                   = 37,

    // NOTE: Anims 38 and above are per-map. Will need separate enums or could do `#ifdef` magic here if it works.

    HarryAnim_Death                   = 40,

    // 56 - duplicate stomp?

    // Probably combat anims and many cutscene anims here.

    // TODO: Anims ahead not entirely clear. Need to confirm before naming definitively. Some grab/release anims might be split in two, hence the gaps.

    HarryAnim_Unk115                  = 115, // Enemy grab torso front?

    HarryAnim_Unk117                  = 117, // Enemy grab torso back?
    HarryAnim_Unk118                  = 118, // Enemy grab legs back?

    HarryAnim_Unk120                  = 120, // Enemy release upper front?
    HarryAnim_Unk121                  = 121, // Damage head?
    HarryAnim_Unk122                  = 122, // Enemy release upper front?
    HarryAnim_Unk123                  = 123, // Enemy release lower back?

    HarryAnim_Unk125                  = 125, // Enemy grab neck front and back?

    HarryAnim_Unk127                  = 127, // Enemy grab pinned front?
    HarryAnim_Unk128                  = 128, // Enemy grab pinned back?
    HarryAnim_Unk129                  = 129, // Enemy release pinned front?
    HarryAnim_Unk130                  = 130, // Enemy release pinned back?
    HarryAnim_Unk131                  = 131, // Damage thrown front? Also used to recover from pinned grab.
    HarryAnim_Unk132                  = 132, // Damage thrown back? Also used to recover from pinned grab.
    HarryAnim_Unk133                  = 133, // Get up front?
    HarryAnim_Unk134                  = 134  // Get up back?
} s_HarryAnim;

/** @brief Harry character model bone indices. */
typedef enum _HarryBone
{
    HarryBone_Root          = 0,
    HarryBone_Torso         = 1,
    HarryBone_Head          = 2,
    HarryBone_LeftShoulder  = 3,
    HarryBone_LeftUpperArm  = 4,
    HarryBone_LeftForearm   = 5,
    HarryBone_LeftHand      = 6,
    HarryBone_RightShoulder = 7,
    HarryBone_RightUpperArm = 8,
    HarryBone_RightForearm  = 9,
    HarryBone_RightHand     = 10,
    HarryBone_Hips          = 11,
    HarryBone_LeftThigh     = 12,
    HarryBone_LeftShin      = 13,
    HarryBone_LeftFoot      = 14,
    HarryBone_RightThigh    = 15,
    HarryBone_RightShin     = 16,
    HarryBone_RightFoot     = 17,

    HarryBone_Count         = 18
} e_HarryBone;

/** @brief Harry character swappable mesh IDs. */
typedef enum _HarrySwappableMesh
{
    HarrySwappableMesh_None      = 0,
    HarrySwappableMesh_RightHand = 1,
    HarrySwappableMesh_LeftHand  = 2
} e_HarrySwappableMesh;

/** @brief Harry character variant mesh IDs. */
typedef enum _HarryVariantMesh
{
    HarryVariantMesh_None           = 0,
    HarryVariantMesh_RightHandEmpty = 1,
    HarryVariantMesh_HandMelee      = 2, // Used for both hands?
    HarryVariantMesh_RightHandGun   = 3,
    HarryVariantMesh_RightHandRifle = 4,
    HarryVariantMesh_5              = 5  // Unsure, only used in `sharedFunc_800CDAA8_0_s02_Switch_Unk85`.
} e_HarryVariantMesh;

#endif
