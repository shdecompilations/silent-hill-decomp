#include "game.h"
#include "inline_no_dmpsx.h"

#include <psyq/libapi.h>
#include <psyq/strings.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/player/player_control.h"
#include "bodyprog/chara/bone.h"
#include "bodyprog/events/npc_main.h"
#include "bodyprog/world/effects.h"
#include "bodyprog/items/item_screens.h"
#include "bodyprog/screen/data.h"
#include "bodyprog/screen/draw.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player/player.h"
#include "bodyprog/sound/sound_system.h"
#include "bodyprog/sys/joy.h"
#include "main/rng.h"

// ========================================
// GLOBAL VARIABLES
// ========================================

// Collision box top extent + Y offset. TODO: Use a struct?
static q3_12 g_Harry_FallForwardAnimBoxOffsets[][2] = {
    { Q12(-1.6f),  Q12(-1.1f)  },
    { Q12(-1.6f),  Q12(-1.1f)  },
    { Q12(-1.6f),  Q12(-1.1f)  },
    { Q12(-1.6f),  Q12(-1.1f)  },
    { Q12(-1.6f),  Q12(-1.1f)  },
    { Q12(-1.6f),  Q12(-1.1f)  },
    { Q12(-1.6f),  Q12(-1.1f)  },
    { Q12(-1.58f), Q12(-1.1f)  },
    { Q12(-1.56f), Q12(-1.08f) },
    { Q12(-1.54f), Q12(-1.08f) },
    { Q12(-1.52f), Q12(-1.08f) },
    { Q12(-1.5f),  Q12(-1.02f) },
    { Q12(-1.44f), Q12(-0.98f) },
    { Q12(-1.37f), Q12(-0.98f) },
    { Q12(-1.3f),  Q12(-0.93f) },
    { Q12(-1.05f), Q12(-0.65f) },
    { Q12(-0.94f), Q12(-0.6f)  },
    { Q12(-0.72f), Q12(-0.5f)  },
    { Q12(-0.66f), Q12(-0.46f) },
    { Q12(-0.64f), Q12(-0.42f) },
    { Q12(-0.62f), Q12(-0.4f)  },
    { Q12(-0.57f), Q12(-0.36f) },
    { Q12(-0.55f), Q12(-0.32f) },
    { Q12(-0.55f), Q12(-0.36f) },
    { Q12(-0.62f), Q12(-0.43f) },
    { Q12(-0.65f), Q12(-0.5f)  },
    { Q12(-0.73f), Q12(-0.56f) },
    { Q12(-0.76f), Q12(-0.6f)  },
    { Q12(-0.82f), Q12(-0.64f) },
    { Q12(-0.82f), Q12(-0.64f) },
    { Q12(-0.86f), Q12(-0.71f) },
    { Q12(-0.92f), Q12(-0.74f) },
    { Q12(-1.0f),  Q12(-0.8f)  },
    { Q12(-1.16f), Q12(-0.87f) },
    { Q12(-1.34f), Q12(-0.94f) },
    { Q12(-1.44f), Q12(-0.97f) },
    { Q12(-1.52f), Q12(-1.01f) },
    { Q12(-1.57f), Q12(-1.03f) },
    { Q12(-1.6f),  Q12(-1.1f)  }
};

// Collision box top extent + Y offset. TODO: Use a struct?
static q3_12 g_Harry_FallBackwardAnimBoxOffsets[][2] = {
    { Q12(-1.6f),  Q12(-1.1f)  },
    { Q12(-1.6f),  Q12(-1.1f)  },
    { Q12(-1.58f), Q12(-1.07f) },
    { Q12(-1.42f), Q12(-0.99f) },
    { Q12(-1.42f), Q12(-0.98f) },
    { Q12(-1.49f), Q12(-1.03f) },
    { Q12(-1.4f),  Q12(-1.02f) },
    { Q12(-1.38f), Q12(-0.98f) },
    { Q12(-1.18f), Q12(-0.75f) },
    { Q12(-0.94f), Q12(-0.49f) },
    { Q12(-0.82f), Q12(-0.4f)  },
    { Q12(-0.72f), Q12(-0.37f) },
    { Q12(-0.66f), Q12(-0.34f) },
    { Q12(-0.66f), Q12(-0.34f) },
    { Q12(-0.54f), Q12(-0.3f)  },
    { Q12(-0.54f), Q12(-0.28f) },
    { Q12(-0.54f), Q12(-0.28f) },
    { Q12(-0.5f),  Q12(-0.25f) },
    { Q12(-0.48f), Q12(-0.25f) },
    { Q12(-0.48f), Q12(-0.25f) },
    { Q12(-0.6f),  Q12(-0.26f) },
    { Q12(-0.69f), Q12(-0.28f) },
    { Q12(-0.76f), Q12(-0.33f) },
    { Q12(-0.84f), Q12(-0.37f) },
    { Q12(-0.87f), Q12(-0.37f) },
    { Q12(-0.92f), Q12(-0.39f) },
    { Q12(-0.97f), Q12(-0.48f) },
    { Q12(-1.13f), Q12(-0.67f) },
    { Q12(-1.22f), Q12(-0.75f) },
    { Q12(-1.43f), Q12(-0.94f) },
    { Q12(-1.56f), Q12(-1.01f) },
    { Q12(-1.58f), Q12(-1.08f) },
    { Q12(-1.6f),  Q12(-1.1f)  },
    { Q12(-1.6f),  Q12(-1.1f)  },
    { Q12(-1.6f),  Q12(-1.1f)  },
    { Q12(-1.6f),  Q12(-1.1f)  },
    { Q12(-1.6f),  Q12(-1.1f)  },
    { Q12(-1.6f),  Q12(-1.1f)  },
    { Q12(-1.6f),  Q12(-1.1f)  }
};

static q3_12 g_Harry_KickAnimBoxOffsets[] = {
    Q12(0.0f),   Q12(0.0f),   Q12(-0.01f), Q12(-0.02f),
    Q12(-0.03f), Q12(-0.04f), Q12(-0.06f), Q12(-0.08f),
    Q12(-0.1f),  Q12(-0.12f), Q12(-0.14f), Q12(-0.16f),
    Q12(-0.18f), Q12(-0.2f),  Q12(-0.22f), Q12(-0.2f),
    Q12(-0.18f), Q12(-0.16f), Q12(-0.14f), Q12(-0.1f),
    Q12(-0.08f), Q12(-0.06f), Q12(-0.04f), Q12(-0.03f),
    Q12(-0.02f), Q12(-0.01f), Q12(0.0f),   Q12(0.0f)
};

static q3_12 g_Harry_StompAnimBoxOffsets[] = {
    Q12(0.0f),   Q12(0.0f),   Q12(-0.01f), Q12(-0.03f),
    Q12(-0.05f), Q12(-0.07f), Q12(-0.09f), Q12(-0.11f),
    Q12(-0.13f), Q12(-0.15f), Q12(-0.17f), Q12(-0.18f),
    Q12(-0.17f), Q12(-0.15f), Q12(-0.12f), Q12(-0.08f),
    Q12(-0.04f), Q12(0.0f)
};

static q3_12 g_Harry_DeathAnimCylinderOffsets[] = {
    Q12(0.0f),   Q12(0.04f),  Q12(0.08f),  Q12(0.12f),
    Q12(0.16f),  Q12(0.20f),  Q12(0.24f),  Q12(0.28f),
    Q12(0.32f),  Q12(0.36f),  Q12(0.40f),  Q12(0.44f),
    Q12(0.48f),  Q12(0.52f),  Q12(0.56f),  Q12(0.60f),
    Q12(0.64f),  Q12(0.68f),  Q12(0.72f),  Q12(0.76f),
    Q12(0.80f),  Q12(0.84f),  Q12(0.88f),  Q12(0.92f),
    Q12(0.96f),  Q12(1.00f),  Q12(1.04f),  Q12(1.08f),
    Q12(1.12f),  Q12(1.16f),  Q12(1.20f),  Q12(1.24f),
    Q12(1.28f),  Q12(1.32f),  Q12(1.36f),  Q12(1.40f),
    Q12(0.0f),   Q12(0.0f),   Q12(0.0f),   Q12(0.0f),
    Q12(0.0f),   Q12(0.0f),   Q12(0.0f),   Q12(0.0f),
    Q12(0.0f),   Q12(-0.01f), Q12(-0.08f), Q12(-0.15f),
    Q12(-0.20f), Q12(-0.23f), Q12(-0.26f), Q12(-0.29f),
    Q12(-0.29f), Q12(-0.29f), Q12(-0.29f), Q12(-0.29f),
    Q12(-0.29f), Q12(-0.29f), Q12(-0.29f), Q12(-0.29f),
    Q12(-0.29f), Q12(-0.29f), Q12(-0.29f), Q12(-0.29f),
    Q12(-0.29f), Q12(-0.29f), Q12(-0.34f), Q12(-0.35f),
    Q12(-0.35f), Q12(-0.34f), Q12(-0.34f), Q12(-0.33f),
    Q12(-0.33f), Q12(-0.33f), Q12(-0.32f), Q12(-0.32f),
    Q12(-0.31f), Q12(-0.29f), Q12(0.0f),   Q12(0.0f),
    Q12(0.0f),   Q12(0.0f),   Q12(0.0f),   Q12(0.0f),
    Q12(0.0f),   Q12(0.0f),   Q12(0.0f),   Q12(0.0f),
    Q12(0.0f),   Q12(0.0f),   Q12(0.0f),   Q12(0.0f),
    Q12(0.0f),   Q12(0.0f),   Q12(0.0f),   Q12(0.0f),
    Q12(0.0f),   Q12(0.0f),   Q12(0.0f),   Q12(0.0f),
    Q12(-0.05f), Q12(-0.11f), Q12(-0.15f), Q12(-0.20f),
    Q12(-0.25f), Q12(-0.30f), Q12(-0.30f), Q12(-0.27f),
    Q12(-0.24f), Q12(-0.21f), Q12(-0.18f), Q12(-0.15f),
    Q12(-0.12f), Q12(-0.09f), Q12(-0.06f), Q12(-0.03f),
    Q12(0.0f),   Q12(0.03f),  Q12(0.07f),  Q12(0.0f),
    Q12(-0.02f), Q12(-0.03f), Q12(-0.03f), Q12(-0.02f),
    Q12(-0.01f), Q12(0.0f),   Q12(0.0f),   Q12(0.0f),
    Q12(0.0f),   Q12(0.0f),   Q12(0.0f),   Q12(0.0f),
    Q12(0.0f),   Q12(0.0f),   Q12(0.0f),   Q12(0.0f),
    Q12(0.0f),   Q12(0.0f),   Q12(0.0f),   Q12(0.0f),
    Q12(0.0f),   Q12(0.0f),   Q12(0.0f),   Q12(0.0f),
    Q12(0.30f),  Q12(0.30f),  Q12(0.30f),  Q12(0.30f),
    Q12(0.30f),  Q12(0.30f),  Q12(0.30f),  Q12(0.30f),
    Q12(0.30f),  Q12(0.30f),  Q12(0.30f),  Q12(0.30f),
    Q12(0.30f),  Q12(0.30f),  Q12(0.30f),  Q12(0.30f),
    Q12(0.32f),  Q12(0.33f),  Q12(0.34f),  Q12(0.35f),
    Q12(0.36f),  Q12(0.37f),  Q12(0.38f),  Q12(0.39f),
    Q12(0.40f),  Q12(0.40f),  Q12(0.40f),  Q12(0.40f),
    Q12(0.40f),  Q12(0.40f),  Q12(0.40f),  Q12(0.40f),
    Q12(0.40f),  Q12(0.40f),  Q12(0.40f),  Q12(0.40f),
    Q12(0.40f),  Q12(0.40f),  Q12(0.40f),  Q12(0.40f),
    Q12(0.40f),  Q12(0.40f),  Q12(0.30f),  Q12(0.30f),
    Q12(0.30f),  Q12(0.30f),  Q12(0.30f),  Q12(0.30f),
    Q12(0.30f),  Q12(0.30f),  Q12(0.30f),  Q12(0.30f),
    Q12(0.30f),  Q12(0.0f)
};

static q3_12 g_Harry_PinnedAnimCylinderOffsets[] = {
    Q12(0.08f), Q12(0.16f), Q12(0.24f), Q12(0.32f),
    Q12(0.4f),  Q12(0.48f), Q12(0.48f), Q12(0.0f)
};

q19_12 g_Player_GrabReleaseInputTimer   = Q12(0.0f);
q3_12  g_Player_FlexRotationY           = Q12_ANGLE(0.0f);
q3_12  g_Player_FlexRotationX           = Q12_ANGLE(0.0f);
u8     g_Player_IsInWalkToRunTransition = false;
u8     g_Player_DisableControl          = false;
q1_7   g_Player_MoveStickMag            = Q7(0.0f);
s8     g_Player_RockDrill_AttackType    = RockDrillAttackType_Center;
u32    D_800AF218                       = 0; // Unused, maybe padding.
s32    g_Player_TargetNpcIdx            = NO_VALUE;

/** Related to player. */
u8 D_800AF220 = 0; // Keyframe index in map overlay header struct.

// 3 bytes of padding.

/** @brief Previous selected weapon. Set to `g_SysWork::PLAYER_COMBAT()::weaponAttack` and used to load the required
 * animation data.
 */
s32 g_Player_PrevSelectedWeapon = NO_VALUE;

// TODO: First element is modified at runtime. `func_8007E8C0` Suggests size is 76?
s_AnimInfo HARRY_BASE_ANIM_INFOS[57] = {
    { Anim_BlendLinear, ANIM_STATUS(HarryAnim_Still,                   false), false, ANIM_STATUS(HarryAnim_Still,                   true), { Q12(10.0f)                     }, NO_VALUE, 0   },
    { Anim_PlayLoop,    ANIM_STATUS(HarryAnim_Still,                   true),  false, NO_VALUE,                                             { Q12(30.0f)                     }, NO_VALUE, 1   },
    { Anim_BlendLinear, ANIM_STATUS(HarryAnim_TransitionToStill,       false), false, ANIM_STATUS(HarryAnim_TransitionToStill,       true), { Q12(10.0f)                     }, NO_VALUE, 0   },
    { Anim_PlayLoop,    ANIM_STATUS(HarryAnim_TransitionToStill,       true),  false, NO_VALUE,                                             { Q12(0.0f)                      }, 0,        0   },
    { Anim_BlendLinear, ANIM_STATUS(HarryAnim_WalkForward,             false), false, ANIM_STATUS(HarryAnim_WalkForward,             true), { Q12(50.0f)                     }, NO_VALUE, 1   },
    { Anim_PlayLoop,    ANIM_STATUS(HarryAnim_WalkForward,             true),  true,  NO_VALUE,                                             { Player_VariableAnimDurationGet }, 1,        23  },
    { Anim_BlendLinear, ANIM_STATUS(HarryAnim_RunForward,              false), false, ANIM_STATUS(HarryAnim_RunForward,              true), { Q12(20.0f)                     }, NO_VALUE, 26  },
    { Anim_PlayLoop,    ANIM_STATUS(HarryAnim_RunForward,              true),  true,  NO_VALUE,                                             { Player_VariableAnimDurationGet }, 26,       45  },
    { Anim_BlendLinear, ANIM_STATUS(HarryAnim_WalkBackward,            false), false, ANIM_STATUS(HarryAnim_WalkBackward,            true), { Q12(20.0f)                     }, NO_VALUE, 46  },
    { Anim_PlayLoop,    ANIM_STATUS(HarryAnim_WalkBackward,            true),  true,  NO_VALUE,                                             { Player_VariableAnimDurationGet }, 46,       69  },
    { Anim_BlendLinear, ANIM_STATUS(HarryAnim_SidestepLeft,            false), false, ANIM_STATUS(HarryAnim_SidestepLeft,            true), { Q12(50.0f)                     }, NO_VALUE, 70  },
    { Anim_PlayLoop,    ANIM_STATUS(HarryAnim_SidestepLeft,            true),  false, NO_VALUE,                                             { Q12(30.0f)                     }, 70,       94  },
    { Anim_BlendLinear, ANIM_STATUS(HarryAnim_SidestepRight,           false), false, ANIM_STATUS(HarryAnim_SidestepRight,           true), { Q12(50.0f)                     }, NO_VALUE, 95  },
    { Anim_PlayLoop,    ANIM_STATUS(HarryAnim_SidestepRight,           true),  false, NO_VALUE,                                             { Q12(30.0f)                     }, 95,       119 },
    { Anim_BlendLinear, ANIM_STATUS(HarryAnim_RunLeft,                 false), false, ANIM_STATUS(HarryAnim_RunLeft,                 true), { Q12(20.0f)                     }, NO_VALUE, 121 },
    { Anim_PlayLoop,    ANIM_STATUS(HarryAnim_RunLeft,                 true),  false, NO_VALUE,                                             { Q12(20.0f)                     }, 121,      133 },
    { Anim_BlendLinear, ANIM_STATUS(HarryAnim_RunRight,                false), false, ANIM_STATUS(HarryAnim_RunRight,                true), { Q12(20.0f)                     }, NO_VALUE, 136 },
    { Anim_PlayLoop,    ANIM_STATUS(HarryAnim_RunRight,                true),  false, NO_VALUE,                                             { Q12(20.0f)                     }, 136,      148 },
    { Anim_BlendLinear, ANIM_STATUS(HarryAnim_RunForwardWallStopRight, false), false, ANIM_STATUS(HarryAnim_RunForwardWallStopRight, true), { Q12(50.0f)                     }, NO_VALUE, 150 },
    { Anim_PlayOnce,    ANIM_STATUS(HarryAnim_RunForwardWallStopRight, true),  false, ANIM_STATUS(HarryAnim_RunForwardWallStopRight, true), { Q12(15.0f)                     }, 150,      159 },
    { Anim_BlendLinear, ANIM_STATUS(HarryAnim_RunForwardWallStopLeft,  false), false, ANIM_STATUS(HarryAnim_RunForwardWallStopLeft,  true), { Q12(50.0f)                     }, NO_VALUE, 160 },
    { Anim_PlayOnce,    ANIM_STATUS(HarryAnim_RunForwardWallStopLeft,  true),  false, ANIM_STATUS(HarryAnim_RunForwardWallStopLeft,  true), { Q12(15.0f)                     }, 160,      169 },
    { Anim_BlendLinear, ANIM_STATUS(HarryAnim_RunForwardStumble,       false), false, ANIM_STATUS(HarryAnim_RunForwardStumble,       true), { Q12(50.0f)                     }, NO_VALUE, 170 },
    { Anim_PlayOnce,    ANIM_STATUS(HarryAnim_RunForwardStumble,       true),  false, ANIM_STATUS(HarryAnim_RunForwardStumble,       true), { Q12(15.0f)                     }, 170,      179 },
    { Anim_BlendLinear, ANIM_STATUS(HarryAnim_TurnLeft,                false), false, ANIM_STATUS(HarryAnim_TurnLeft,                true), { Q12(50.0f)                     }, NO_VALUE, 180 },
    { Anim_PlayLoop,    ANIM_STATUS(HarryAnim_TurnLeft,                true),  false, NO_VALUE,                                             { Q12(15.0f)                     }, 180,      192 },
    { Anim_BlendLinear, ANIM_STATUS(HarryAnim_TurnRight,               false), false, ANIM_STATUS(HarryAnim_TurnRight,               true), { Q12(50.0f)                     }, NO_VALUE, 193 },
    { Anim_PlayLoop,    ANIM_STATUS(HarryAnim_TurnRight,               true),  false, NO_VALUE,                                             { Q12(15.0f)                     }, 193,      205 },
    { Anim_BlendLinear, ANIM_STATUS(HarryAnim_QuickTurnRight,          false), false, ANIM_STATUS(HarryAnim_QuickTurnRight,          true), { Q12(50.0f)                     }, NO_VALUE, 206 },
    { Anim_PlayOnce,    ANIM_STATUS(HarryAnim_QuickTurnRight,          true),  false, ANIM_STATUS(HarryAnim_QuickTurnRight,          true), { Q12(28.0f)                     }, 206,      217 },
    { Anim_BlendLinear, ANIM_STATUS(HarryAnim_QuickTurnLeft,           false), false, ANIM_STATUS(HarryAnim_QuickTurnLeft,           true), { Q12(50.0f)                     }, NO_VALUE, 219 },
    { Anim_PlayOnce,    ANIM_STATUS(HarryAnim_QuickTurnLeft,           true),  false, ANIM_STATUS(HarryAnim_QuickTurnLeft,           true), { Q12(28.0f)                     }, 219,      230 },
    { Anim_BlendLinear, ANIM_STATUS(HarryAnim_JumpBackward,            false), false, ANIM_STATUS(HarryAnim_JumpBackward,            true), { Q12(10.0f)                     }, NO_VALUE, 240 },
    { Anim_PlayOnce,    ANIM_STATUS(HarryAnim_JumpBackward,            true),  false, ANIM_STATUS(HarryAnim_JumpBackward,            true), { Q12(10.0f)                     }, 240,      247 },
    { Anim_BlendLinear, ANIM_STATUS(HarryAnim_LookAround,              false), false, ANIM_STATUS(HarryAnim_LookAround,              true), { Q12(50.0f)                     }, NO_VALUE, 248 },
    { Anim_PlayLoop,    ANIM_STATUS(HarryAnim_LookAround,              true),  false, NO_VALUE,                                             { Q12(7.0f)                      }, 248,      320 },
    { Anim_BlendLinear, ANIM_STATUS(HarryAnim_RunLeftWallStop,         false), false, ANIM_STATUS(HarryAnim_RunLeftWallStop,         true), { Q12(50.0f)                     }, NO_VALUE, 321 },
    { Anim_PlayOnce,    ANIM_STATUS(HarryAnim_RunLeftWallStop,         true),  false, ANIM_STATUS(HarryAnim_RunLeftWallStop,         true), { Q12(20.0f)                     }, 321,      336 },
    { Anim_BlendLinear, ANIM_STATUS(HarryAnim_RunLeftStumble,          false), false, ANIM_STATUS(HarryAnim_RunLeftStumble,          true), { Q12(50.0f)                     }, NO_VALUE, 337 },
    { Anim_PlayOnce,    ANIM_STATUS(HarryAnim_RunLeftStumble,          true),  false, ANIM_STATUS(HarryAnim_RunLeftStumble,          true), { Q12(15.0f)                     }, 337,      349 },
    { Anim_BlendLinear, ANIM_STATUS(HarryAnim_RunRightWallStop,        false), false, ANIM_STATUS(HarryAnim_RunRightWallStop,        true), { Q12(50.0f)                     }, NO_VALUE, 350 },
    { Anim_PlayOnce,    ANIM_STATUS(HarryAnim_RunRightWallStop,        true),  false, ANIM_STATUS(HarryAnim_RunRightWallStop,        true), { Q12(20.0f)                     }, 350,      365 },
    { Anim_BlendLinear, ANIM_STATUS(HarryAnim_RunRightStumble,         false), false, ANIM_STATUS(HarryAnim_RunRightStumble,         true), { Q12(50.0f)                     }, NO_VALUE, 366 },
    { Anim_PlayOnce,    ANIM_STATUS(HarryAnim_RunRightStumble,         true),  false, ANIM_STATUS(HarryAnim_RunRightStumble,         true), { Q12(15.0f)                     }, 366,      378 },
    { Anim_BlendLinear, ANIM_STATUS(HarryAnim_FallForward,             false), false, ANIM_STATUS(HarryAnim_FallForward,             true), { Q12(30.0f)                     }, NO_VALUE, 387 },
    { Anim_PlayOnce,    ANIM_STATUS(HarryAnim_FallForward,             true),  false, ANIM_STATUS(HarryAnim_FallForward,             true), { Q12(20.0f)                     }, 387,      417 },
    { Anim_BlendLinear, ANIM_STATUS(HarryAnim_FallBackward,            false), false, ANIM_STATUS(HarryAnim_FallBackward,            true), { Q12(30.0f)                     }, NO_VALUE, 418 },
    { Anim_PlayOnce,    ANIM_STATUS(HarryAnim_FallBackward,            true),  false, ANIM_STATUS(HarryAnim_FallBackward,            true), { Q12(20.0f)                     }, 418,      456 },
    { Anim_BlendLinear, ANIM_STATUS(HarryAnim_Kick,                    false), false, ANIM_STATUS(HarryAnim_Kick,                    true), { Q12(5.0f)                      }, NO_VALUE, 457 },
    { Anim_PlayOnce,    ANIM_STATUS(HarryAnim_Kick,                    true),  false, ANIM_STATUS(HarryAnim_Kick,                    true), { Q12(20.0f)                     }, 457,      484 },
    { Anim_BlendLinear, ANIM_STATUS(HarryAnim_Stomp,                   false), false, ANIM_STATUS(HarryAnim_Stomp,                   true), { Q12(5.0f)                      }, NO_VALUE, 485 },
    { Anim_PlayOnce,    ANIM_STATUS(HarryAnim_Stomp,                   true),  false, ANIM_STATUS(HarryAnim_Stomp,                   true), { Q12(20.0f)                     }, 485,      502 },
    { Anim_BlendLinear, ANIM_STATUS(HarryAnim_Idle,                    false), false, ANIM_STATUS(HarryAnim_Idle,                    true), { Q12(5.0f)                      }, NO_VALUE, 503 },
    { Anim_PlayLoop,    ANIM_STATUS(HarryAnim_Idle,                    true),  false, NO_VALUE,                                             { Q12(10.0f)                     }, 503,      542 },
    { Anim_BlendLinear, ANIM_STATUS(HarryAnim_IdleExhausted,           false), false, ANIM_STATUS(HarryAnim_IdleExhausted,           true), { Q12(5.0f)                      }, NO_VALUE, 543 },
    { Anim_PlayLoop,    ANIM_STATUS(HarryAnim_IdleExhausted,           true),  true,  NO_VALUE,                                             { Player_VariableAnimDurationGet }, 543,      567 },
    {}
};
// TODO: `GameFs_WeaponInfoUpdate` indicates there should be more.

u8 __pad1[14] = {};

s16 D_800AF5C6 = 0;
u32 D_800AF5C8[23] = {}; // Padding?
s16 D_800AF624 = 0;
s16 D_800AF626 = 0; /** Keyframe index. */
u32 D_800AF628[368] = {};

s32 g_Player_AttackAnimIdx    = 0;
s32 g_Player_MeleeAttackType  = 0;
s32 g_Player_IsMultiTapAttack = 0;

s_EquippedWeaponInfo g_Player_EquippedWeaponInfos[11] = {
    {},
    { Sfx_Unk1293, 0,           0,           ANIM_STATUS(HarryAnim_Unk32, false), ANIM_STATUS(HarryAnim_HandgunRecoil, false), ANIM_STATUS(HarryAnim_HandgunRecoil, true), 0,  52, 0 },
    { Sfx_Unk1294, 0,           0,           ANIM_STATUS(HarryAnim_Unk32, false), ANIM_STATUS(HarryAnim_HandgunRecoil, false), ANIM_STATUS(HarryAnim_HandgunRecoil, true), 0,  52, 0 },
    { Sfx_Unk1295, 0,           0,           ANIM_STATUS(HarryAnim_Unk32, false), ANIM_STATUS(HarryAnim_HandgunRecoil, false), ANIM_STATUS(HarryAnim_HandgunRecoil, true), 0,  52, 0 },
    { Sfx_Unk1301, 0,           0,           ANIM_STATUS(HarryAnim_Unk32, false), ANIM_STATUS(HarryAnim_HandgunRecoil, false), ANIM_STATUS(HarryAnim_HandgunRecoil, true), 0,  52, 0 },
    { Sfx_Unk1303, 0,           0,           ANIM_STATUS(HarryAnim_Unk32, false), ANIM_STATUS(HarryAnim_Unk29,         false), ANIM_STATUS(HarryAnim_Unk29,         true), 0,  20, 0 },
    { Sfx_Unk1283, Sfx_Unk1284, Sfx_Unk1285, ANIM_STATUS(HarryAnim_Unk33, false), ANIM_STATUS(HarryAnim_Unk36,         false), ANIM_STATUS(HarryAnim_Unk30,         true), 37, 37, 0 },
    { Sfx_Unk1290, Sfx_Unk1291, Sfx_Unk1292, ANIM_STATUS(HarryAnim_Unk33, false), ANIM_STATUS(HarryAnim_Unk36,         false), ANIM_STATUS(HarryAnim_Unk30,         true), 24, 37, 0 },
    { Sfx_Unk1287, Sfx_Unk1288, Sfx_Unk1289, ANIM_STATUS(HarryAnim_Unk33, false), ANIM_STATUS(HarryAnim_Unk36,         false), ANIM_STATUS(HarryAnim_Unk30,         true), 23, 37, 0 },
    { Sfx_Unk1299, 0,           0,           ANIM_STATUS(HarryAnim_Unk33, false), ANIM_STATUS(HarryAnim_Unk36,         false), ANIM_STATUS(HarryAnim_Unk30,         true), 0,  37, 0 },
    { Sfx_Unk1298, 0,           0,           ANIM_STATUS(HarryAnim_Unk32, false), ANIM_STATUS(HarryAnim_HandgunRecoil, false), ANIM_STATUS(HarryAnim_HandgunRecoil, true), 0,  52, 0 }
};

/** @brief Cached collision point data. Generic collision global used by high-level getters. */
s_CollisionPoint g_CollisionPointCache = {
    .position  = {},
    .surface   = {},
    .floorType = NO_VALUE
};

s_800C44F0 D_800C44F0[10];
VECTOR3    g_Player_TargetEnemyPosition;
q19_12     D_800C454C;
q19_12     g_Player_PrevMoveSpeed;
q3_12      g_Player_RotationToTargetX;
q3_12      g_Player_RotationToTargetY;
s32        g_Player_HasMoveInput;
s32        g_Player_HasActionInput;
s8         g_Player_PrevAttackReceived;
u8         g_Player_IsDead;
u8         g_Player_DisableDamage;

u8 __pad_bss_800C4563[13];

s_EquippedWeaponInfo g_Player_EquippedWeaponInfo;
u8                   g_Player_CutsceneState;

u8 __pad_bss_800C457D;

u16        g_Player_IsAiming;
u16        g_Player_IsSteppingLeftTap;
u16        g_Player_IsSteppingRightTap;
u16        g_Player_IsTurningLeft;
u16        g_Player_IsTurningRight;

u8         D_800C4588;

s8 __pad_bss_800C4589[7];

s_CollisionResult g_Player_CollisionResult;

u16        g_Player_IsSteppingLeftHold;
u16        g_Player_IsSteppingRightHold;
VECTOR3    D_800C45B0;
u16        g_Player_IsHoldAttack;
u16        g_Player_IsAttacking;
u16        g_Player_IsShooting;

s8         __pad_bss_800C45C2[6];
s_800C45C8 D_800C45C8;
s8         __pad_bss_800C45E0[8];
u16        g_Player_IsMovingForward;
s8         __pad_bss_800C45EA[2];
q19_12     D_800C45EC;
u16        g_Player_IsMovingBackward;
s8         __pad_bss_800C45F2[6];
VECTOR3    g_Player_PrevPosition;
u16        g_Player_IsRunning;
u8         g_Player_AnimResetRequest;
s8         __pad_bss_800C4607;
q19_12     g_Player_HeadingAngle;
s32        __pad_bss_800C460C;
VECTOR3    D_800C4610;

q19_12 Player_VariableAnimDurationGet(s_Model* model) // 0x800706E4
{
    q19_12 duration;

    duration = Q12(0.0f);

    switch (PLAYER_EXTRA().state)
    {
        case PlayerState_EnemyGrabPinnedFront:
        case PlayerState_EnemyGrabPinnedBack:
            switch (g_SavegamePtr->mapIdx)
            {
                case MapIdx_MAP2_S00:
                case MapIdx_MAP2_S02:
                case MapIdx_MAP4_S02:
                case MapIdx_MAP5_S01:
                case MapIdx_MAP6_S00:
                case MapIdx_MAP6_S02:
                    if (g_MapOverlayHdr.field_38[D_800AF220].status_2 == ANIM_STATUS(HarryAnim_Unk128, false) ||
                        g_MapOverlayHdr.field_38[D_800AF220].status_2 == ANIM_STATUS(HarryAnim_Unk129, false))
                    {
                        if (PLAYER_CHARA().health <= Q12(0.0f))
                        {
                            PLAYER_PROPS().afkTimer -= g_DeltaTime;
                            if (PLAYER_PROPS().afkTimer >= Q12(0.0f))
                            {
                                PLAYER_PROPS().afkTimer -= g_DeltaTime;
                                duration = PLAYER_PROPS().afkTimer;
                            }
                            else
                            {
                                duration = Q12(0.0f);
                            }
                            break;
                        }
                    }

                    duration = Q12(15.0f);
            }
            break;

        case PlayerState_OnFloorFront:
        case PlayerState_OnFloorBehind:
            if (g_SavegamePtr->mapIdx == MapIdx_MAP6_S04)
            {
                if (g_MapOverlayHdr.field_38[D_800AF220].status_2 == ANIM_STATUS(HarryAnim_Unk132, true) ||
                    g_MapOverlayHdr.field_38[D_800AF220].status_2 == ANIM_STATUS(HarryAnim_Unk133, false))
                {
                    if (PLAYER_CHARA().health <= Q12(0.0f))
                    {
                        PLAYER_PROPS().afkTimer -= g_DeltaTime * 2;
                        if (PLAYER_PROPS().afkTimer >= Q12(0.0f))
                        {
                            PLAYER_PROPS().afkTimer -= g_DeltaTime * 2;

                            duration = PLAYER_PROPS().afkTimer;
                        }
                        else
                        {
                            duration = Q12(0.0f);
                        }
                        break;
                    }
                }

                duration = Q12(10.0f);
            }
            break;

        default:
            switch (model->anim.status)
            {
                case ANIM_STATUS(HarryAnim_WalkForward, true):
                    if (g_Controller0->rawSticks.sticks.leftY <= -STICK_DEADZONE)
                    {
                        duration = ((ABS(g_Controller0->rawSticks.sticks.leftY + STICK_DEADZONE) * Q12(0.65f) / STICK_DEADZONE) * 16) + Q12(12.0f);
                    }
                    else if (g_Player_MoveStickMag != Q7(0.0f))
                    {
                        duration = ((ABS(g_Player_MoveStickMag - STICK_DEADZONE) * Q12(0.65f) / STICK_DEADZONE) * 16) + Q12(12.0f);
                    }
                    else
                    {
                        duration = Q12(22.0f);
                    }
                    break;

                case ANIM_STATUS(HarryAnim_RunForward, true):
                    if (g_Controller0->rawSticks.sticks.leftY <= -STICK_DEADZONE)
                    {
                        if ((model->anim.keyframeIdx >= 40 && model->anim.keyframeIdx < 46) ||
                            (model->anim.keyframeIdx >= 30 && model->anim.keyframeIdx < 36))
                        {
                            duration = (ABS(g_Controller0->rawSticks.sticks.leftY + STICK_DEADZONE) * Q12(0.25f)) + Q12(16.0f);
                        }
                        else
                        {
                            duration = Q12(32.0f);
                        }
                    }
                    else if (g_Player_MoveStickMag != Q7(0.0f))
                    {
                        duration = (ABS(g_Player_MoveStickMag - STICK_DEADZONE) * Q12(0.25f)) + Q12(16.0f);
                    }
                    else
                    {
                        duration = Q12(30.0f);
                    }
                    break;

                case ANIM_STATUS(HarryAnim_WalkBackward, true):
                    if (g_Controller0->rawSticks.sticks.leftY >= STICK_DEADZONE)
                    {
                        duration = (((ABS(g_Controller0->rawSticks.sticks.leftY - STICK_DEADZONE) * Q12(0.4f) / STICK_DEADZONE) * Q12(1.0f)) / 200) + Q12(15.36f);
                    }
                    else if (g_Player_MoveStickMag != Q7(0.0f))
                    {
                        duration = (((ABS(g_Player_MoveStickMag - STICK_DEADZONE) * Q12(0.4f) / STICK_DEADZONE) * Q12(1.0f)) / 200) + Q12(15.36f);
                    }
                    else
                    {
                        duration = Q12(23.0f);
                    }
                    break;

                case ANIM_STATUS(HarryAnim_IdleExhausted, true):
                    if (PLAYER_CHARA().health < Q12(30.0f))
                    {
                        duration = Q12(40.0f) - PLAYER_CHARA().health;
                    }
                    else
                    {
                        duration = Q12(FP_FROM(PLAYER_PROPS().exhaustionTimer, Q12_SHIFT));
                    }
                    break;
            }
    }

    return duration;
}

const s_AnimInfo* const D_800297B8 = HARRY_BASE_ANIM_INFOS;

void func_80070B84(s_SubCharacter* player, q19_12 baseMoveSpeed, q19_12 topMoveSpeed, s32 keyframeIdx) // 0x80070B84
{
    q3_12  unkMoveDist;
    q24_7  stickY;
    q3_12* moveDist;

    if (g_Player_MoveStickMag == Q7(0.0f))
    {
        stickY = ABS(g_Controller0->rawSticks.sticks.leftY);
    }
    else
    {
        stickY = g_Player_MoveStickMag;
    }

    baseMoveSpeed += ((topMoveSpeed - baseMoveSpeed) * (stickY - STICK_DEADZONE)) / STICK_DEADZONE;

    // @hack Wrapping in loop required for match.
    do
    {
        if (baseMoveSpeed < PLAYER_PROPS().moveSpeed)
        {
            unkMoveDist           = PLAYER_PROPS().moveSpeed - ((TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f))) * 2);
            PLAYER_PROPS().moveSpeed = unkMoveDist;
            if (unkMoveDist < baseMoveSpeed)
            {
                PLAYER_PROPS().moveSpeed = baseMoveSpeed;
            }
        }
        else if (PLAYER_PROPS().moveSpeed < baseMoveSpeed)
        {
            moveDist = &PLAYER_PROPS().moveSpeed;
            if (player->model.anim.keyframeIdx >= keyframeIdx)
            {
                PLAYER_PROPS().moveSpeed = *moveDist + TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f));
            }

            PLAYER_PROPS().moveSpeed = CLAMP(*moveDist, Q12(0.0f), baseMoveSpeed);
        }
    }
    while (false); // @hack Required for match.
}

void func_80070CF0(s_SubCharacter* player, q19_12 arg1, q19_12 moveDistMax, q19_12 moveDistForward, q19_12 moveDistBack) // 0x80070CF0
{
    q24_7  stickY;
    q3_12* moveDist;

    do
    {
        if ((player->model.anim.keyframeIdx >= 40 && player->model.anim.keyframeIdx < 46) ||
            (player->model.anim.keyframeIdx >= 30 && player->model.anim.keyframeIdx < 36))
        {
            stickY      = g_Player_MoveStickMag ? g_Player_MoveStickMag : ABS(g_Controller0->rawSticks.sticks.leftY);
            moveDistMax = arg1 + (((moveDistMax - arg1) * (stickY - STICK_DEADZONE)) / STICK_DEADZONE);
        }
    }
    while (false); // @hack Required for match.

    if (moveDistMax < PLAYER_PROPS().moveSpeed)
    {
        PLAYER_PROPS().moveSpeed -= moveDistBack;
        if (PLAYER_PROPS().moveSpeed < moveDistMax)
        {
            PLAYER_PROPS().moveSpeed = moveDistMax;
        }
    }
    else
    {
        moveDist = &PLAYER_PROPS().moveSpeed;
        if (PLAYER_PROPS().moveSpeed < moveDistMax)
        {
            PLAYER_PROPS().moveSpeed += moveDistForward;
            PLAYER_PROPS().moveSpeed  = CLAMP(*moveDist, Q12(0.0f), moveDistMax);
        }
    }
}

void func_80070DF0(s_PlayerExtra* extra, s_SubCharacter* player, s32 weaponAttack, s32 animStatus)  // 0x80070DF0
{
    q3_12 shortestAngle;
    q3_12 angleTo;

    if (extra->model.stateStep == 0)
    {
        extra->model.anim.status = animStatus - 1;
        extra->model.stateStep++;
    }

    if (player->model.stateStep == 0)
    {
        player->model.anim.status = animStatus - 1;
        player->model.stateStep++;
    }

    angleTo = Q12_FRACT(ratan2((NPC_CHARA(g_SysWork.targetNpcIdx).position.vx + NPC_CHARA(g_SysWork.targetNpcIdx).collision.shapeOffsets.box.vx) - PLAYER_COMBAT().attackPosition.vx,
                               (NPC_CHARA(g_SysWork.targetNpcIdx).position.vz + NPC_CHARA(g_SysWork.targetNpcIdx).collision.shapeOffsets.box.vz) - PLAYER_COMBAT().attackPosition.vz) +
                          Q12_ANGLE(360.0f));
    player->angleToTarget = angleTo;
    Math_ShortestAngleGet(player->rotation.vy, angleTo, &shortestAngle);

    if (ABS(shortestAngle) >= Q12_ANGLE(8.5f))
    {
        if (shortestAngle < 0)
        {
            player->rotation.vy -= Q12_ANGLE(8.5f);
        }
        else
        {
            player->rotation.vy += Q12_ANGLE(8.5f);
        }
    }

    if (extra->model.anim.keyframeIdx >= (HARRY_BASE_ANIM_INFOS[animStatus].startKeyframeIdx + D_800AD4C8[weaponAttack].field_E) &&
        ((HARRY_BASE_ANIM_INFOS[animStatus].startKeyframeIdx + D_800AD4C8[weaponAttack].field_E) + D_800AD4C8[weaponAttack].field_F) >= extra->model.anim.keyframeIdx)
    {
        PLAYER_COMBAT().weaponAttack = weaponAttack;

        if (!(PLAYER_PROPS().flags & PlayerFlag_Unk2))
        {
            player->field_44.field_0 = 1;
            PLAYER_PROPS().flags |= PlayerFlag_Unk2;
        }
    }

    if (animStatus == ANIM_STATUS(HarryAnim_Kick, true) && ANIM_STATUS_IS_ACTIVE(player->model.anim.status))
    {
        PLAYER_CHARA().collision.shapeOffsets.box.vx      =  Q12_MULT(g_Harry_KickAnimBoxOffsets[player->model.anim.keyframeIdx - 457], Math_Cos(player->rotation.vy));
        PLAYER_CHARA().collision.shapeOffsets.box.vz      = -Q12_MULT(g_Harry_KickAnimBoxOffsets[player->model.anim.keyframeIdx - 457], Math_Sin(player->rotation.vy));
        PLAYER_CHARA().collision.shapeOffsets.cylinder.vx =  Q12(0.0f);
        PLAYER_CHARA().collision.shapeOffsets.cylinder.vz =  Q12(0.0f);
    }

    if (animStatus == ANIM_STATUS(HarryAnim_Stomp, true) && ANIM_STATUS_IS_ACTIVE(player->model.anim.status))
    {
        PLAYER_CHARA().collision.shapeOffsets.box.vx      =  Q12_MULT(g_Harry_StompAnimBoxOffsets[player->model.anim.keyframeIdx - 485], Math_Cos(player->rotation.vy));
        PLAYER_CHARA().collision.shapeOffsets.box.vz      = -Q12_MULT(g_Harry_StompAnimBoxOffsets[player->model.anim.keyframeIdx - 485], Math_Sin(player->rotation.vy));
        PLAYER_CHARA().collision.shapeOffsets.cylinder.vx =  Q12(0.0f);
        PLAYER_CHARA().collision.shapeOffsets.cylinder.vz =  Q12(0.0f);
    }

    if (player->model.anim.status == animStatus &&
        player->model.anim.keyframeIdx == HARRY_BASE_ANIM_INFOS[animStatus].endKeyframeIdx)
    {
        PLAYER_PROPS().flags &= ~PlayerFlag_Unk2;

        Player_ExtraStateSet(player, extra, PlayerState_None);

        PLAYER_CHARA().collision.shapeOffsets.cylinder.vz = Q12(0.0f);
        PLAYER_CHARA().collision.shapeOffsets.cylinder.vx = Q12(0.0f);
        PLAYER_CHARA().collision.shapeOffsets.box.vz      = Q12(0.0f);
        PLAYER_CHARA().collision.shapeOffsets.box.vx      = Q12(0.0f);

        PLAYER_COMBAT().weaponAttack = (g_SavegamePtr->equippedWeapon == InvItemId_Unequipped) ? NO_VALUE :
                                                                                              (g_SavegamePtr->equippedWeapon + InvItemId_KitchenKnife);
        g_SysWork.targetNpcIdx    = NO_VALUE;
        PLAYER_COMBAT().isAiming     = false;
    }
}

void Player_CharaTurn_0(s_SubCharacter* player, e_PlayerLowerBodyState curState) // 0x800711C4
{
    if (PLAYER_EXTRA().upperBodyState == PlayerUpperBodyState_Attack ||
        !g_Player_IsSteppingLeftTap || !g_Player_IsSteppingRightTap)
    {
        return;
    }

    if (g_Player_IsTurningLeft)
    {
        PLAYER_EXTRA().lowerBodyState = curState + PlayerLowerBodyState_QuickTurnLeft;
    }
    else
    {
        PLAYER_EXTRA().lowerBodyState = curState + PlayerLowerBodyState_QuickTurnRight;
    }
}

void Player_CharaTurn_1(s_SubCharacter* player, e_PlayerLowerBodyState curState) // 0x80071224
{
    if (PLAYER_EXTRA().upperBodyState == PlayerUpperBodyState_Attack ||
        !g_Player_IsSteppingLeftTap || !g_Player_IsSteppingRightTap)
    {
        return;
    }

    if (g_Player_IsTurningRight)
    {
        PLAYER_EXTRA().lowerBodyState = curState + PlayerLowerBodyState_QuickTurnRight;
    }
    else
    {
        PLAYER_EXTRA().lowerBodyState = curState + PlayerLowerBodyState_QuickTurnLeft;
    }
}

void Player_CharaRotate(s32 speed) // 0x80071284
{
    if (g_GameWork.config.extraRetreatTurn)
    {
        if (g_Player_IsTurningRight)
        {
            D_800C454C = ((speed * g_DeltaTime) * g_Player_IsTurningRight) >> 6; // `>> 6` = `/ 64`.
        }
        else if (g_Player_IsTurningLeft)
        {
            D_800C454C = ((-speed * g_DeltaTime) * g_Player_IsTurningLeft) >> 6;
        }
    }
    else if (PLAYER_EXTRA().lowerBodyState == PlayerLowerBodyState_WalkBackward ||
             PLAYER_EXTRA().lowerBodyState == PlayerLowerBodyState_AimWalkBackward)
    {
        if (g_Player_IsTurningRight)
        {
            D_800C454C = ((-speed * g_DeltaTime) * g_Player_IsTurningRight) >> 6;
        }
        else if (g_Player_IsTurningLeft)
        {
            D_800C454C = ((speed * g_DeltaTime) * g_Player_IsTurningLeft) >> 6;
        }
    }
    else
    {
        if (g_Player_IsTurningRight)
        {
            D_800C454C = ((speed * g_DeltaTime) * g_Player_IsTurningRight) >> 6;
        }
        else if (g_Player_IsTurningLeft)
        {
            D_800C454C = ((-speed * g_DeltaTime) * g_Player_IsTurningLeft) >> 6;
        }
    }
}

void Player_MovementStateReset(s_SubCharacter* player, e_PlayerLowerBodyState lowerBodyState) // 0x800713B4
{
    if (PLAYER_EXTRA().lowerBodyState != lowerBodyState)
    {
        player->model.stateStep                 = 0;
        player->model.controlState              = 0;
        player->properties.player.runStepSfxCount = 0;
        player->properties.player.afkTimer      = Q12(0.0f);
        g_SysWork.playerStopFlags               = PlayerStopFlag_None;
    }
}

bool Player_FootstepSfxPlay(s32 animStatus, s_SubCharacter* player, s32 keyframe0, s32 keyframe1, s32 sfx, s8 pitch)
{
    if (player->model.anim.status != animStatus)
    {
        return false;
    }

    if (player->model.anim.keyframeIdx >= keyframe1)
    {
        if (!(PLAYER_PROPS().flags & PlayerFlag_Unk4))
        {
            if (pitch < 0x20)
            {
                switch (animStatus)
                {
                    case ANIM_STATUS(HarryAnim_SidestepRight, true):
                    case ANIM_STATUS(HarryAnim_SidestepLeft, true):
                    case ANIM_STATUS(HarryAnim_TurnLeft, true):
                    case ANIM_STATUS(HarryAnim_TurnRight, true):
                        Sfx_WithPitchPlay(sfx, &player->position, Q8_CLAMPED(0.095f), pitch);
                        player->properties.player.field_10C = pitch;
                        break;

                    default:
                        Sfx_WithPitchPlay(sfx, &player->position, Q8(0.25f), pitch);
                        player->properties.player.field_10C = pitch + 0x10;
                        break;
                }
            }
            else
            {
                Sfx_WithPitchPlay(sfx, &player->position, Q8(0.5f), pitch);
                player->properties.player.field_10C = pitch + 0x40;
            }

            PLAYER_PROPS().flags |= PlayerFlag_Unk4;
            return true;
        }
    }
    else
    {
        PLAYER_PROPS().flags &= ~PlayerFlag_Unk4;
    }

    if (player->model.anim.keyframeIdx >= keyframe0)
    {
        if (!(PLAYER_PROPS().flags & PlayerFlag_Unk5))
        {
            if (pitch < 32)
            {
                switch (animStatus)
                {
                    case ANIM_STATUS(HarryAnim_SidestepRight, true):
                    case ANIM_STATUS(HarryAnim_SidestepLeft, true):
                    case ANIM_STATUS(HarryAnim_TurnLeft, true):
                    case ANIM_STATUS(HarryAnim_TurnRight, true):
                        Sfx_WithPitchPlay(sfx, &player->position, Q8_CLAMPED(0.095f), pitch);
                        player->properties.player.field_10C = pitch;
                        break;

                    default:
                        Sfx_WithPitchPlay(sfx, &player->position, Q8(0.25f), pitch);
                        player->properties.player.field_10C = pitch + 16;
                        break;
                }
            }
            else
            {
                Sfx_WithPitchPlay(sfx, &player->position, Q8(0.5f), pitch);
                player->properties.player.field_10C = pitch + 64;
            }

            PLAYER_PROPS().flags |= PlayerFlag_Unk5;
            return true;
        }
    }
    else
    {
        PLAYER_PROPS().flags &= ~PlayerFlag_Unk5;
    }

    return false;
}

bool func_80071620(u32 animStatus, s_SubCharacter* player, s32 keyframeIdx, e_SfxId sfxId) // 0x80071620
{
    if (player->model.anim.status != animStatus)
    {
        return false;
    }

    if (player->model.anim.keyframeIdx >= keyframeIdx)
    {
        if (PLAYER_PROPS().flags & PlayerFlag_SfxActive)
        {
            return false;
        }

        switch (sfxId)
        {
            case Sfx_Stumble0:
            case Sfx_Unk1316:
            case Sfx_Unk1318:
            case Sfx_Unk1319:
            case Sfx_Stumble1:
                Sfx_WithFlagsPlay(sfxId, &player->position, 0x80, SfxFlag_None);
                player->properties.player.field_10C = 0x40;
                break;

            case Sfx_Unk1283:
                Sfx_WithFlagsPlay(sfxId, &player->position, 0xC8, SfxFlag_NoDistAtten);
                player->properties.player.field_10C = 0;
                break;

            case Sfx_Unk1628:
                Sfx_WithFlagsPlay(sfxId, &player->position, 0x40, SfxFlag_Mono);
                break;

            case Sfx_Unk1626:
                Sfx_WithFlagsPlay(sfxId, &player->position, 0xFF, SfxFlag_Mono);
                break;

            case Sfx_Unk1638:
                Sfx_WithFlagsPlay(sfxId, &player->position, 0xFF, SfxFlag_NoDistAtten);
                break;

            default:
            case Sfx_HarryHeavyBreath:
            case Sfx_DoorJammed:
                Sfx_WithFlagsPlay(sfxId, &player->position, 0x40, SfxFlag_NoDistAtten);
                player->properties.player.field_10C = 0;
                break;
        }

        PLAYER_PROPS().flags |= PlayerFlag_SfxActive;
        return true;
    }
    else
    {
        PLAYER_PROPS().flags &= ~PlayerFlag_SfxActive;

        do {} while (false); // @hack Required for match.

        return false;
    }
}

void Player_Update(s_SubCharacter* player, s_AnmHeader* anmHdr, GsCOORDINATE2* coords) // 0x800717D0
{
    s_PlayerExtra* extra;

    extra = &PLAYER_EXTRA();

    if (g_DeltaTime != Q12(0.0f))
    {
        Player_ReceiveDamage(player, extra);

        if (g_Player_IsInWalkToRunTransition)
        {
            g_Player_HasActionInput      = false;
            g_Player_HasMoveInput        = false;
            g_Player_IsShooting          = false;
            g_Player_IsAttacking         = false;
            g_Player_IsHoldAttack        = false;
            g_Player_IsAiming            = false;
            g_Player_IsRunning           = false;
            g_Player_IsMovingBackward    = false;
            g_Player_IsMovingForward     = false;
            g_Player_IsSteppingRightTap  = false;
            g_Player_IsSteppingRightHold = false;
            g_Player_IsTurningRight      = false;
            g_Player_IsSteppingLeftTap   = false;
            g_Player_IsSteppingLeftHold  = false;
            g_Player_IsTurningLeft       = false;
        }

        if (!g_Player_DisableControl)
        {
            Player_LogicUpdate(player, extra, coords);
        }
        else
        {
            g_MapOverlayHdr.func_B8(player, extra, coords);
        }

        if (!g_Player_DisableControl)
        {
            Player_PositionUpdate(player, extra, coords);
        }
        else
        {
            g_MapOverlayHdr.func_BC(player, extra, coords);
        }

        Player_AnimUpdate(player, extra, anmHdr, coords);
        func_8007D090(player, extra, coords);
    }

    D_800C45B0.vx = 0;
    D_800C45B0.vz = 0;
}

static inline void func_80071968_Switch0(void)
{
    if (PLAYER_COMBAT().weaponAttack != NO_VALUE)
    {
        switch (PLAYER_COMBAT().weaponAttack)
        {
            case WEAPON_ATTACK(EquippedWeaponId_KitchenKnife, AttackInputType_Tap):
            case WEAPON_ATTACK(EquippedWeaponId_SteelPipe,    AttackInputType_Tap):
            case WEAPON_ATTACK(EquippedWeaponId_RockDrill,    AttackInputType_Tap):
            case WEAPON_ATTACK(EquippedWeaponId_Hammer,       AttackInputType_Tap):
            case WEAPON_ATTACK(EquippedWeaponId_Chainsaw,     AttackInputType_Tap):
            case WEAPON_ATTACK(EquippedWeaponId_Katana,       AttackInputType_Tap):
            case WEAPON_ATTACK(EquippedWeaponId_Axe,          AttackInputType_Tap):
            case WEAPON_ATTACK(EquippedWeaponId_KitchenKnife, AttackInputType_Hold):
            case WEAPON_ATTACK(EquippedWeaponId_SteelPipe,    AttackInputType_Hold):
            case WEAPON_ATTACK(EquippedWeaponId_RockDrill,    AttackInputType_Hold):
            case WEAPON_ATTACK(EquippedWeaponId_Hammer,       AttackInputType_Hold):
            case WEAPON_ATTACK(EquippedWeaponId_Chainsaw,     AttackInputType_Hold):
            case WEAPON_ATTACK(EquippedWeaponId_Katana,       AttackInputType_Hold):
            case WEAPON_ATTACK(EquippedWeaponId_Axe,          AttackInputType_Hold):
            case WEAPON_ATTACK(EquippedWeaponId_KitchenKnife, AttackInputType_Multitap):
            case WEAPON_ATTACK(EquippedWeaponId_SteelPipe,    AttackInputType_Multitap):
            case WEAPON_ATTACK(EquippedWeaponId_RockDrill,    AttackInputType_Multitap):
            case WEAPON_ATTACK(EquippedWeaponId_Hammer,       AttackInputType_Multitap):
            case WEAPON_ATTACK(EquippedWeaponId_Chainsaw,     AttackInputType_Multitap):
            case WEAPON_ATTACK(EquippedWeaponId_Katana,       AttackInputType_Multitap):
            case WEAPON_ATTACK(EquippedWeaponId_Axe,          AttackInputType_Multitap):
                WorldGfx_CharaMeshSwap(Chara_Harry, MESH_SWAP_STATUS(HarrySwappableMesh_LeftHand, HarryVariantMesh_HandMelee));
                break;

            case WEAPON_ATTACK(EquippedWeaponId_Handgun,      AttackInputType_Tap):
            case WEAPON_ATTACK(EquippedWeaponId_Shotgun,      AttackInputType_Tap):
            case WEAPON_ATTACK(EquippedWeaponId_HyperBlaster, AttackInputType_Tap):
                WorldGfx_CharaMeshSwap(Chara_Harry, MESH_SWAP_STATUS(HarrySwappableMesh_LeftHand, HarryVariantMesh_RightHandGun));
                break;

            case WEAPON_ATTACK(EquippedWeaponId_HuntingRifle, AttackInputType_Tap):
                WorldGfx_CharaMeshSwap(Chara_Harry, MESH_SWAP_STATUS(HarrySwappableMesh_LeftHand, HarryVariantMesh_RightHandRifle));
                break;

            case WEAPON_ATTACK(EquippedWeaponId_Unk3, AttackInputType_Tap):
            case WEAPON_ATTACK(EquippedWeaponId_Kick, AttackInputType_Tap):
            case WEAPON_ATTACK(EquippedWeaponId_Stomp, AttackInputType_Tap):
            case WEAPON_ATTACK(EquippedWeaponId_Unk3, AttackInputType_Hold):
            case WEAPON_ATTACK(EquippedWeaponId_Kick, AttackInputType_Hold):
            case WEAPON_ATTACK(EquippedWeaponId_Stomp, AttackInputType_Hold):
            case WEAPON_ATTACK(EquippedWeaponId_Unk3, AttackInputType_Multitap):
            case WEAPON_ATTACK(EquippedWeaponId_Kick, AttackInputType_Multitap):
            case WEAPON_ATTACK(EquippedWeaponId_Stomp, AttackInputType_Multitap):
            case WEAPON_ATTACK(EquippedWeaponId_Unk31,AttackInputType_Tap):
                break;
        }
    }
    else
    {
        WorldGfx_CharaMeshSwap(Chara_Harry, MESH_SWAP_STATUS(HarrySwappableMesh_LeftHand, HarryVariantMesh_HandMelee));
    }
}

static inline void func_80071968_Switch1(void)
{
    if (PLAYER_COMBAT().weaponAttack != NO_VALUE)
    {
        switch (PLAYER_COMBAT().weaponAttack)
        {
            case WEAPON_ATTACK(EquippedWeaponId_KitchenKnife, AttackInputType_Tap):
            case WEAPON_ATTACK(EquippedWeaponId_SteelPipe,    AttackInputType_Tap):
            case WEAPON_ATTACK(EquippedWeaponId_RockDrill,    AttackInputType_Tap):
            case WEAPON_ATTACK(EquippedWeaponId_Hammer,       AttackInputType_Tap):
            case WEAPON_ATTACK(EquippedWeaponId_Chainsaw,     AttackInputType_Tap):
            case WEAPON_ATTACK(EquippedWeaponId_Katana,       AttackInputType_Tap):
            case WEAPON_ATTACK(EquippedWeaponId_Axe,          AttackInputType_Tap):
            case WEAPON_ATTACK(EquippedWeaponId_KitchenKnife, AttackInputType_Hold):
            case WEAPON_ATTACK(EquippedWeaponId_SteelPipe,    AttackInputType_Hold):
            case WEAPON_ATTACK(EquippedWeaponId_RockDrill,    AttackInputType_Hold):
            case WEAPON_ATTACK(EquippedWeaponId_Hammer,       AttackInputType_Hold):
            case WEAPON_ATTACK(EquippedWeaponId_Chainsaw,     AttackInputType_Hold):
            case WEAPON_ATTACK(EquippedWeaponId_Katana,       AttackInputType_Hold):
            case WEAPON_ATTACK(EquippedWeaponId_Axe,          AttackInputType_Hold):
            case WEAPON_ATTACK(EquippedWeaponId_KitchenKnife, AttackInputType_Multitap):
            case WEAPON_ATTACK(EquippedWeaponId_SteelPipe,    AttackInputType_Multitap):
            case WEAPON_ATTACK(EquippedWeaponId_RockDrill,    AttackInputType_Multitap):
            case WEAPON_ATTACK(EquippedWeaponId_Hammer,       AttackInputType_Multitap):
            case WEAPON_ATTACK(EquippedWeaponId_Chainsaw,     AttackInputType_Multitap):
            case WEAPON_ATTACK(EquippedWeaponId_Katana,       AttackInputType_Multitap):
            case WEAPON_ATTACK(EquippedWeaponId_Axe,          AttackInputType_Multitap):
                WorldGfx_CharaMeshSwap(Chara_Harry, MESH_SWAP_STATUS(HarrySwappableMesh_RightHand, HarryVariantMesh_HandMelee));
                break;

            case WEAPON_ATTACK(EquippedWeaponId_Handgun,      AttackInputType_Tap):
            case WEAPON_ATTACK(EquippedWeaponId_Shotgun,      AttackInputType_Tap):
            case WEAPON_ATTACK(EquippedWeaponId_HyperBlaster, AttackInputType_Tap):
                WorldGfx_CharaMeshSwap(Chara_Harry, MESH_SWAP_STATUS(HarrySwappableMesh_RightHand, HarryVariantMesh_RightHandGun));
                break;

            case WEAPON_ATTACK(EquippedWeaponId_HuntingRifle, AttackInputType_Tap):
                WorldGfx_CharaMeshSwap(Chara_Harry, MESH_SWAP_STATUS(HarrySwappableMesh_RightHand, HarryVariantMesh_RightHandRifle));
                break;

            case WEAPON_ATTACK(EquippedWeaponId_Unk3,  AttackInputType_Tap):
            case WEAPON_ATTACK(EquippedWeaponId_Kick,  AttackInputType_Tap):
            case WEAPON_ATTACK(EquippedWeaponId_Stomp, AttackInputType_Tap):
            case WEAPON_ATTACK(EquippedWeaponId_Unk3,  AttackInputType_Hold):
            case WEAPON_ATTACK(EquippedWeaponId_Kick,  AttackInputType_Hold):
            case WEAPON_ATTACK(EquippedWeaponId_Stomp, AttackInputType_Hold):
            case WEAPON_ATTACK(EquippedWeaponId_Unk3,  AttackInputType_Multitap):
            case WEAPON_ATTACK(EquippedWeaponId_Kick,  AttackInputType_Multitap):
            case WEAPON_ATTACK(EquippedWeaponId_Stomp, AttackInputType_Multitap):
            case WEAPON_ATTACK(EquippedWeaponId_Unk31, AttackInputType_Tap):
                break;
        }
    }
    else
    {
        WorldGfx_CharaMeshSwap(Chara_Harry,
                               g_SysWork.enablePlayerMatchAnim ? MESH_SWAP_STATUS(HarrySwappableMesh_RightHand, HarryVariantMesh_HandMelee) :
                                                                 MESH_SWAP_STATUS(HarrySwappableMesh_RightHand, HarryVariantMesh_RightHandEmpty));
    }
}

void Player_AnimUpdate(s_SubCharacter* player, s_PlayerExtra* extra, s_AnmHeader* anmHdr, GsCOORDINATE2* coords) // 0x80071968
{
    s_AnimInfo* animInfo;

    switch (PLAYER_EXTRA().state)
    {
        case PlayerState_Unk61:
        case PlayerState_Unk62:
        case PlayerState_Unk63:
        case PlayerState_Unk64:
        case PlayerState_Unk65:
        case PlayerState_Unk66:
        case PlayerState_Unk67:
        case PlayerState_Unk68:
        case PlayerState_Unk69:
        case PlayerState_Unk76:
        case PlayerState_Unk77:
        case PlayerState_Unk78:
        case PlayerState_Unk79:
        case PlayerState_Unk80:
        case PlayerState_Unk83:
        case PlayerState_Unk84:
        case PlayerState_Unk85:
        case PlayerState_Unk86:
        case PlayerState_Unk87:
        case PlayerState_Unk88:
        case PlayerState_Unk93:
        case PlayerState_Unk94:
        case PlayerState_Unk95:
        case PlayerState_Unk97:
        case PlayerState_Unk98:
        case PlayerState_Unk99:
        case PlayerState_Unk100:
        case PlayerState_Unk102:
        case PlayerState_Unk103:
        case PlayerState_Unk104:
        case PlayerState_Unk105: // Moving an object. (Hospital Basement Otherworld and Motel)
        case PlayerState_Unk107:
        case PlayerState_Unk108:
        case PlayerState_Unk111:
        case PlayerState_Unk112:
        case PlayerState_Unk118:
        case PlayerState_Unk119:
        case PlayerState_Unk122:
        case PlayerState_Unk134:
        case PlayerState_Unk136:
        case PlayerState_Unk137:
        case PlayerState_Unk138:
        case PlayerState_Unk139:
        case PlayerState_Unk141: // Throwing disinfective alcohol scene. (Hospital Basement Otherworld)
        case PlayerState_Unk142:
        case PlayerState_Unk143:
        case PlayerState_Unk144:
        case PlayerState_Unk145:
        case PlayerState_Unk146:
        case PlayerState_Unk147:
        case PlayerState_Unk148:
        case PlayerState_Unk152:
        case PlayerState_Unk162:
            break;

        case PlayerState_RunForward:
            func_80071968_Switch0();
            break;

        case PlayerState_None:
            switch (PLAYER_EXTRA().upperBodyState)
            {
                case PlayerUpperBodyState_RunForward:
                case PlayerUpperBodyState_RunRight:
                case PlayerUpperBodyState_RunLeft:
                    func_80071968_Switch0();
                    break;

                default:
                    func_80071968_Switch1();
                    break;
            }
            break;

        default:
        case PlayerState_Combat:
        case PlayerState_Idle:
        case PlayerState_FallForward:
        case PlayerState_FallBackward:
        case PlayerState_KickEnemy:
        case PlayerState_StompEnemy:
        case PlayerState_Unk7:
        case PlayerState_Death:
        case PlayerState_InstantDeath:
        case PlayerState_EnemyGrabTorsoFront:
        case PlayerState_Unk11:
        case PlayerState_Unk12:
        case PlayerState_EnemyGrabTorsoBack:
        case PlayerState_EnemyGrabLegsFront:
        case PlayerState_EnemyGrabLegsBack:
        case PlayerState_EnemyReleaseUpperFront:
        case PlayerState_Unk17:
        case PlayerState_Unk18:
        case PlayerState_DamageHead:
        case PlayerState_EnemyReleaseUpperBack:
        case PlayerState_EnemyReleaseLowerFront:
        case PlayerState_EnemyReleaseLowerBack:
        case PlayerState_DamageTorsoBack:
        case PlayerState_DamageTorsoFront:
        case PlayerState_DamageTorsoRight:
        case PlayerState_DamageTorsoLeft:
        case PlayerState_DamageFeetFront:
        case PlayerState_DamageFeetBack:
        case PlayerState_DamagePushBack:
        case PlayerState_DamagePushFront:
        case PlayerState_Unk31:
        case PlayerState_EnemyGrabNeckFront:
        case PlayerState_EnemyGrabNeckBack:
        case PlayerState_Unk34:
        case PlayerState_Unk35:
        case PlayerState_Unk36:
        case PlayerState_EnemyGrabPinnedFrontStart:
        case PlayerState_EnemyGrabPinnedBackStart:
        case PlayerState_EnemyGrabPinnedFront:
        case PlayerState_EnemyGrabPinnedBack:
        case PlayerState_EnemyReleasePinnedFront:
        case PlayerState_EnemyReleasePinnedBack:
        case PlayerState_Unk43:
        case PlayerState_Unk44:
        case PlayerState_DamageThrownFront:
        case PlayerState_DamageThrownBack:
        case PlayerState_OnFloorFront:
        case PlayerState_OnFloorBehind:
        case PlayerState_GetUpFront:
        case PlayerState_GetUpBack:
        case PlayerState_Unk51:
        case PlayerState_Reset:
        case PlayerState_WalkForward:
        case PlayerState_WalkBackward:
        case PlayerState_TurnRight:
        case PlayerState_TurnLeft:
        case PlayerState_Unk58:
        case PlayerState_Unk59: // Interacting with vines. (Hospital Basement Otherworld)
        case PlayerState_Unk60: // Interacting with vines without disinfective alcohol applied. (Hospital Basement Otherworld)
        case PlayerState_Unk70:
        case PlayerState_Unk71:
        case PlayerState_Unk72:
        case PlayerState_Unk73:
        case PlayerState_Unk74:
        case PlayerState_Unk75:
        case PlayerState_Unk81: // Burning vines scene. (Hospital Basement Otherworld)
        case PlayerState_Unk82:
        case PlayerState_Unk89:
        case PlayerState_Unk90:
        case PlayerState_Unk91:
        case PlayerState_Unk92:
        case PlayerState_Unk96:
        case PlayerState_Unk101:
        case PlayerState_Unk106:
        case PlayerState_Unk109:
        case PlayerState_Unk110:
        case PlayerState_Unk113:
        case PlayerState_Unk114:
        case PlayerState_Unk115:
        case PlayerState_Unk116:
        case PlayerState_Unk117:
        case PlayerState_Unk120:
        case PlayerState_Unk121:
        case PlayerState_Unk123:
        case PlayerState_Unk124:
        case PlayerState_Unk125:
        case PlayerState_Unk126:
        case PlayerState_Unk127:
        case PlayerState_Unk128:
        case PlayerState_Unk129:
        case PlayerState_Unk130:
        case PlayerState_Unk131:
        case PlayerState_Unk132:
        case PlayerState_Unk133:
        case PlayerState_Unk135:
        case PlayerState_Unk140:
        case PlayerState_Unk149:
        case PlayerState_Unk150:
        case PlayerState_Unk151:
        case PlayerState_Unk153:
        case PlayerState_Unk154:
        case PlayerState_Unk155:
        case PlayerState_Unk156:
        case PlayerState_Unk157:
        case PlayerState_Unk158:
        case PlayerState_Unk159:
        case PlayerState_Unk160:
        case PlayerState_Unk161:
            func_80071968_Switch1();
            break;
    }

    if (!g_Player_IsInWalkToRunTransition)
    {
        // Disable upper body bones before playing anim.
        PLAYER_EXTRA().disabledAnimBoneFlags = HARRY_UPPER_BODY_BONE_MASK;

        animInfo = &HARRY_BASE_ANIM_INFOS[player->model.anim.status];
        animInfo->playbackFunc(&player->model, anmHdr, coords, animInfo);

        // Re-enable upper body bones, disable lower body bones.
        PLAYER_EXTRA().disabledAnimBoneFlags = HARRY_LOWER_BODY_BONE_MASK;

        animInfo = &HARRY_BASE_ANIM_INFOS[extra->model.anim.status];
        animInfo->playbackFunc(&extra->model, anmHdr, coords, animInfo);
        return;
    }

    // Disable upper body bones before playing anim.
    PLAYER_EXTRA().disabledAnimBoneFlags = HARRY_UPPER_BODY_BONE_MASK;
    player->model.anim.status                     = ANIM_STATUS(HarryAnim_Still, false);

    animInfo = &HARRY_BASE_ANIM_INFOS[ANIM_STATUS(HarryAnim_Still, false)];
    animInfo->playbackFunc(&player->model, anmHdr, coords, animInfo);

    // Re-enable upper body bones, disable lower body bones.
    PLAYER_EXTRA().disabledAnimBoneFlags = HARRY_LOWER_BODY_BONE_MASK;

    animInfo = &HARRY_BASE_ANIM_INFOS[extra->model.anim.status];
    animInfo->playbackFunc(&extra->model, anmHdr, coords, animInfo);

    if (player->model.anim.status == HARRY_BASE_ANIM_INFOS[ANIM_STATUS(HarryAnim_Still, false)].linkStatus)
    {
        g_Player_IsInWalkToRunTransition = false;
    }
}

void Player_LogicUpdate(s_SubCharacter* player, s_PlayerExtra* extra, GsCOORDINATE2* coords) // 0x80071CE8
{
    SVECTOR       playerAngles;
    q3_12         headingAngle0;
    q3_12         headingAngle1;
    q3_12         angle;
    s16           sp1E;
    q19_12        temp_a2;
    q19_12        cylinderOffset0;
    q19_12        var_v1_5;
    s32           cylinderOffset1;
    q19_12        deltaPosX;
    q19_12        deltaPosZ;
    s32           temp_v1_12;
    s32           temp_v1_13;
    e_PlayerState thrownState;
    s32           grabFreeInputCount;
    e_PlayerState romperAttackState;
    e_PlayerState enemyGrabReleaseState;
    q3_12         unkDistThreshold;
    q3_12         npcDist;
    s32           npcIdx;
    s32           animStatus;
    s32           temp;
    s_Model**     models; // Maybe model pointer array?
    s_Model*      model;

    animStatus = ANIM_STATUS(HarryAnim_Still, false);

    Game_TimerUpdate();

    g_Player_PrevMoveSpeed                = Q12(0.0f);
    D_800C454C                            = Q12(0.0f);
    player->properties.player.field_10C >>= 1;

    if (player->flags & CharaFlag_Unk4)
    {
        player->properties.player.timer_110 += g_DeltaTime;
    }

    if (player->properties.player.timer_110 > D_800C45EC)
    {
        player->properties.player.timer_110 = Q12(0.0f);
        player->flags &= ~CharaFlag_Unk4;
    }

    if (PLAYER_PROPS().gasWeaponPowerTimer != Q12(0.0f))
    {
        PLAYER_PROPS().gasWeaponPowerTimer -= g_DeltaTime;
    }

    PLAYER_PROPS().gasWeaponPowerTimer = CLAMP(PLAYER_PROPS().gasWeaponPowerTimer, Q12(0.0f), Q12(60.0f));

    if (PLAYER_COMBAT().weaponAttack == WEAPON_ATTACK(EquippedWeaponId_Chainsaw,  AttackInputType_Tap) ||
        PLAYER_COMBAT().weaponAttack == WEAPON_ATTACK(EquippedWeaponId_RockDrill, AttackInputType_Tap))
    {
        func_8004C564(PLAYER_COMBAT().weaponAttack, (PLAYER_PROPS().gasWeaponPowerTimer != 0) ? 4 : 2);
    }

    g_SavegamePtr->healthSaturation -= g_DeltaTime;
    g_SavegamePtr->healthSaturation = CLAMP(g_SavegamePtr->healthSaturation, Q12(0.0f), Q12(300.0f));

    if (g_SavegamePtr->healthSaturation != Q12(0.0f))
    {
        PLAYER_CHARA().health += g_DeltaTime;
        PLAYER_CHARA().health  = CLAMP(PLAYER_CHARA().health, Q12(0.0f), Q12(100.0f));
    }

    if (g_SavegamePtr->mapIdx == MapIdx_MAP2_S00)
    {
        g_MapOverlayHdr.func_108();
    }

    if (g_DeltaTime != Q12(0.0f))
    {
        Player_Controller();
    }

    switch (PLAYER_EXTRA().state)
    {
        case PlayerState_Idle:
            PLAYER_PROPS().moveSpeed = Q12(0.0f);
            WorldEnvRender_LightRotationAndIntensityGet(&playerAngles);
            PLAYER_PROPS().quickTurnHeadingAngle = playerAngles.vy;

            if (extra->model.stateStep == 0)
            {
                extra->model.anim.status = ANIM_STATUS(HarryAnim_LookAround, false);
                extra->model.stateStep++;
            }

            if (player->model.stateStep == 0)
            {
                player->model.anim.status = ANIM_STATUS(HarryAnim_LookAround, false);
                player->model.stateStep++;
            }

            // If player is not performing a movement.
            if (!(g_Player_HasMoveInput | g_Player_HasActionInput))
            {
                break;
            }

            Player_ExtraStateSet(player, extra, PlayerState_None);

        case PlayerState_None:
        case PlayerState_Combat:
            Player_LowerBodyUpdate(player, extra);

            if (PLAYER_EXTRA().state < (u32)PlayerState_Idle)
            {
                Player_UpperBodyUpdate(player, extra);
            }
            break;

        case PlayerState_Unk7:
            func_8007FB94(player, extra, ANIM_STATUS(100, false));

            if (PLAYER_PROPS().moveSpeed != Q12(0.0f))
            {
                PLAYER_PROPS().moveSpeed -= TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.5f));

                if (PLAYER_PROPS().moveSpeed < Q12(0.0f))
                {
                    PLAYER_PROPS().moveSpeed = Q12(0.0f);
                }
            }

            if (player->attackReceived != WEAPON_ATTACK(EquippedWeaponId_Unk68, AttackInputType_Tap) && 
                player->attackReceived != WEAPON_ATTACK(EquippedWeaponId_Unk69, AttackInputType_Tap))
            {
                g_Player_HeadingAngle = ratan2(player->damage.position.vx, player->damage.position.vz) - player->rotation.vy;
                PLAYER_PROPS().moveSpeed = SQUARE(player->damage.position.vx) + SQUARE(player->damage.position.vz) + SQUARE(player->damage.position.vy);
            }

            if (extra->model.anim.keyframeIdx == g_MapOverlayHdr.field_38[D_800AF220].keyframeIdx_6)
            {
                player->attackReceived = NO_VALUE;

                g_SysWork.targetNpcIdx = NO_VALUE;
                PLAYER_PROPS().flags &= ~PlayerFlag_DamageReceived;

                Player_ExtraStateSet(player, extra, PlayerState_None);

                PLAYER_PROPS().moveSpeed = Q12(0.0f);
            }

            g_Player_PrevMoveSpeed = PLAYER_PROPS().moveSpeed;
            player->flags         |= CharaFlag_Unk4;
            player->attackReceived = NO_VALUE;
            break;

        case PlayerState_DamageThrownFront:
        case PlayerState_DamageThrownBack:
            thrownState = PlayerState_None;

            switch (PLAYER_EXTRA().state)
            {
                case PlayerState_DamageThrownFront:
                    animStatus  = ANIM_STATUS(HarryAnim_Unk131, true);
                    thrownState = PlayerState_OnFloorFront;
                    break;

                case PlayerState_DamageThrownBack:
                    animStatus  = ANIM_STATUS(HarryAnim_Unk132, false);
                    thrownState = PlayerState_OnFloorBehind;
                    break;
            }

            func_8007FB94(player, extra, animStatus);

            if (player->model.anim.keyframeIdx == g_MapOverlayHdr.field_38[D_800AF220].keyframeIdx_6)
            {
                Player_ExtraStateSet(player, extra, thrownState);
                player->properties.player.afkTimer = Q12(10.0f);
            }

            if (PLAYER_PROPS().moveSpeed != Q12(0.0f))
            {
                PLAYER_PROPS().moveSpeed -= TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.5f)) >> 3;

                if (PLAYER_PROPS().moveSpeed < Q12(0.0f))
                {
                    PLAYER_PROPS().moveSpeed = Q12(0.0f);
                }
            }

            g_Player_PrevMoveSpeed = PLAYER_PROPS().moveSpeed;
            break;

        case PlayerState_EnemyGrabPinnedFrontStart:
        case PlayerState_EnemyGrabPinnedBackStart:
            romperAttackState = PlayerState_None;
            npcIdx            = 0;

            switch (PLAYER_EXTRA().state)
            {
                case PlayerState_EnemyGrabPinnedFrontStart:
                    animStatus        = ANIM_STATUS(HarryAnim_Unk127, true);
                    romperAttackState = PlayerState_EnemyGrabPinnedFront;
                    npcIdx            = g_SysWork.npcIdxs[0];
                    Math_ShortestAngleGet(player->rotation.vy, Q12_ANGLE_NORM_U(NPC_CHARA(npcIdx).rotation.vy + Q12_ANGLE(180.0f)), &headingAngle0);
                    break;

                case PlayerState_EnemyGrabPinnedBackStart:
                    animStatus        = ANIM_STATUS(HarryAnim_Unk128, true);
                    romperAttackState = PlayerState_EnemyGrabPinnedBack;
                    npcIdx            = g_SysWork.npcIdxs[1];
                    Math_ShortestAngleGet(player->rotation.vy, Q12_ANGLE_NORM_U(NPC_CHARA(npcIdx).rotation.vy + Q12_ANGLE(360.0f)), &headingAngle0);
                    break;
            }

            PLAYER_PROPS().moveSpeed = Q12(0.0f);
            func_8007FB94(player, extra, animStatus);
            player->collision.cylinder.outerRadius = Q12(0.25f);
            player->collision.cylinder.innerRadius = Q12(0.0f);

            if (ANIM_STATUS_IS_ACTIVE(player->model.anim.status))
            {
                cylinderOffset0                                = -g_Harry_PinnedAnimCylinderOffsets[player->model.anim.keyframeIdx - g_MapOverlayHdr.field_38[D_800AF220].time];
                PLAYER_CHARA().collision.shapeOffsets.box.vx      = Q12(0.0f);
                PLAYER_CHARA().collision.shapeOffsets.box.vz      = Q12(0.0f);
                PLAYER_CHARA().collision.shapeOffsets.cylinder.vx = Q12_MULT(cylinderOffset0, Math_Sin(player->rotation.vy));
                PLAYER_CHARA().collision.shapeOffsets.cylinder.vz = Q12_MULT(cylinderOffset0, Math_Cos(player->rotation.vy));
            }

            if (ABS(headingAngle0) < Q12_ANGLE(11.25f))
            {
                if (PLAYER_EXTRA().state == PlayerState_EnemyGrabPinnedFrontStart)
                {
                    player->rotation.vy = NPC_CHARA(npcIdx).rotation.vy + Q12_ANGLE(180.0f);
                }
                else
                {
                    player->rotation.vy = NPC_CHARA(npcIdx).rotation.vy;
                }
            }
            else
            {
                if (headingAngle0 > Q12_ANGLE(0.0f))
                {
                    player->rotation.vy += Q12_ANGLE(11.25f);
                }
                else
                {
                    player->rotation.vy -= Q12_ANGLE(11.25f);
                }
            }

            Math_ShortestAngleGet(player->rotation.vy, Q12_ANGLE_NORM_U(NPC_CHARA(npcIdx).rotation.vy + Q12_ANGLE(360.0f)), &headingAngle0);

            model = &NPC_CHARA(npcIdx).model;

            do {} while (false); // @hack Required for match.

            g_Player_HeadingAngle =
            temp                  = headingAngle0;

            if ((*models) != NULL) // @hack Required for match.
            {
                g_Player_HeadingAngle += Q12_ANGLE(0.0f);
            }

            if (player->model.anim.keyframeIdx == g_MapOverlayHdr.field_38[D_800AF220].keyframeIdx_6)
            {
                Player_ExtraStateSet(player, extra, romperAttackState);
                player->properties.player.afkTimer = Q12(15.0f);
            }

            if (ANIM_STATUS_IS_ACTIVE(player->model.anim.status))
            {
                if (PLAYER_EXTRA().state >= PlayerState_EnemyGrabPinnedFrontStart &&
                    PLAYER_EXTRA().state <  PlayerState_EnemyGrabPinnedFront)
                {
                    temp = Q12(-8.0f);
                    extra->model.anim.time         = (Q12(g_MapOverlayHdr.harryMapAnimInfos[player->model.anim.status - 76].startKeyframeIdx) + model->anim.time) + temp;
                    player->model.anim.time        = (Q12(g_MapOverlayHdr.harryMapAnimInfos[player->model.anim.status - 76].startKeyframeIdx) + model->anim.time) + temp;
                    player->model.anim.keyframeIdx = FP_FROM(player->model.anim.time, Q12_SHIFT);
                    extra->model.anim.keyframeIdx  = FP_FROM(extra->model.anim.time, Q12_SHIFT);
                }
            }

            if (ABS(player->position.vx - D_800C4610.vx) <= Q12(0.05f))
            {
                player->position.vx = D_800C4610.vx;
            }
            else
            {
                if (player->position.vx >= D_800C4610.vx)
                {
                    player->position.vx -= Q12(0.05f) + 1;
                }
                else
                {
                    player->position.vx += Q12(0.05f) + 1;
                }
            }

            if (ABS(player->position.vz - D_800C4610.vz) <= Q12(0.05f))
            {
                player->position.vz = D_800C4610.vz;
            }
            else
            {
                if (player->position.vz < D_800C4610.vz)
                {
                    player->position.vz += Q12(0.05f) + 1;
                }
                else
                {
                    player->position.vz -= Q12(0.05f) + 1;
                }
            }
            break;

        case PlayerState_EnemyGrabTorsoFront:
        case PlayerState_EnemyGrabTorsoBack:
        case PlayerState_EnemyGrabLegsFront:
        case PlayerState_EnemyGrabLegsBack:
        case PlayerState_EnemyGrabNeckFront:
        case PlayerState_EnemyGrabNeckBack:
        case PlayerState_EnemyGrabPinnedFront:
        case PlayerState_EnemyGrabPinnedBack:
        case PlayerState_OnFloorFront:
        case PlayerState_OnFloorBehind:
            grabFreeInputCount    = 0;
            enemyGrabReleaseState = PlayerState_None;
            unkDistThreshold      = Q12(0.0f);
            PLAYER_PROPS().moveSpeed = Q12(0.0f);
            npcDist               = Q12(0.0f);

            // Accommodates player position (for pinned enemy gram and Romper attack) and establishes required input count to get free.
            switch (PLAYER_EXTRA().state)
            {
                case PlayerState_OnFloorFront:
                case PlayerState_OnFloorBehind:
                    if (g_SavegamePtr->gameDifficulty == GameDifficulty_Easy)
                    {
                        grabFreeInputCount = 1600;
                    }
                    else if (g_SavegamePtr->gameDifficulty == GameDifficulty_Hard)
                    {
                        grabFreeInputCount = 4800;
                    }
                    else
                    {
                        grabFreeInputCount = 3200;
                    }

                    switch (PLAYER_EXTRA().state)
                    {
                        case PlayerState_OnFloorFront:
                            animStatus            = ANIM_STATUS(HarryAnim_Unk132, true);
                            enemyGrabReleaseState = PlayerState_GetUpFront;
                            break;

                        case PlayerState_OnFloorBehind:
                            animStatus            = ANIM_STATUS(HarryAnim_Unk133, false);
                            enemyGrabReleaseState = PlayerState_GetUpBack;
                            break;
                    }

                    player->collision.cylinder.innerRadius += Q12_MULT_PRECISE(g_DeltaTime, Q12(0.27f));
                    player->collision.box.topExtent    += Q12_MULT_PRECISE(g_DeltaTime, Q12(1.2f));
                    player->collision.box.offsetY      += Q12_MULT_PRECISE(g_DeltaTime, Q12(0.9f));
                    player->collision.cylinder.innerRadius  = CLAMP(player->collision.cylinder.innerRadius, DEFAULT_PLAYER_CYLINDER_INNER_RADIUS, Q12(0.5f));
                    player->collision.box.topExtent     = CLAMP(player->collision.box.topExtent, DEFAULT_PLAYER_BOX_TOP, Q12(-0.4));
                    player->collision.box.offsetY       = CLAMP(player->collision.box.offsetY, DEFAULT_PLAYER_BOX_OFFSET_Y, Q12(-0.2f));

                    if (player->health <= Q12(0.0f) && player->properties.player.afkTimer <= Q12(0.0f))
                    {
                        g_MapOverlayHdr.playerAnimLock();
                        SysWork_StateSetNext(SysState_GameOver);

                        player->health                  = Q12(100.0f);
                        PLAYER_PROPS().gasWeaponPowerTimer = Q12(0.0f);
                        return;
                    }
                    break;

                case PlayerState_EnemyGrabPinnedFront:
                case PlayerState_EnemyGrabPinnedBack:
                    unkDistThreshold = Q12(0.65f);

                    switch (PLAYER_EXTRA().state)
                    {
                        case PlayerState_EnemyGrabPinnedFront:
                            if (g_SavegamePtr->gameDifficulty == GameDifficulty_Easy)
                            {
                                grabFreeInputCount = 800;
                            }
                            else if (g_SavegamePtr->gameDifficulty == GameDifficulty_Hard)
                            {
                                grabFreeInputCount = 2400;
                            }
                            else
                            {
                                grabFreeInputCount = 1600;
                            }

                            animStatus            = ANIM_STATUS(HarryAnim_Unk128, false);
                            enemyGrabReleaseState = PlayerState_EnemyReleasePinnedFront;
                            break;

                        case PlayerState_EnemyGrabPinnedBack:
                            if (g_SavegamePtr->gameDifficulty == GameDifficulty_Easy)
                            {
                                grabFreeInputCount = 1200;
                            }
                            else if (g_SavegamePtr->gameDifficulty == GameDifficulty_Hard)
                            {
                                grabFreeInputCount = 3600;
                            }
                            else
                            {
                                grabFreeInputCount = 2400;
                            }

                            animStatus          = ANIM_STATUS(HarryAnim_Unk129, false);
                            enemyGrabReleaseState = PlayerState_EnemyReleasePinnedBack;
                            break;
                    }

                    player->collision.cylinder.outerRadius         = Q12(0.0f);
                    PLAYER_CHARA().collision.shapeOffsets.cylinder.vz = Q12(0.0f);
                    PLAYER_CHARA().collision.shapeOffsets.cylinder.vx = Q12(0.0f);
                    PLAYER_CHARA().collision.shapeOffsets.box.vz      = Q12(0.0f);
                    PLAYER_CHARA().collision.shapeOffsets.box.vx      = Q12(0.0f);

                    if (ABS(player->position.vx - D_800C4610.vx) <= Q12(0.05f))
                    {
                        player->position.vx = D_800C4610.vx;
                    }
                    else
                    {
                        if (player->position.vx >= D_800C4610.vx)
                        {
                            player->position.vx -= (Q12(0.05f) + 1);
                        }
                        else
                        {
                            player->position.vx += (Q12(0.05f) + 1);
                        }
                    }

                    if (ABS(player->position.vz - D_800C4610.vz) <= Q12(0.05f))
                    {
                        player->position.vz = D_800C4610.vz;
                    }
                    else
                    {
                        if (player->position.vz >= D_800C4610.vz)
                        {
                            player->position.vz -= (Q12(0.05f) + 1);
                        }
                        else
                        {
                            player->position.vz += (Q12(0.05f) + 1);
                        }
                    }

                    if (player->health <= Q12(0.0f) && player->properties.player.afkTimer <= Q12(0.0f))
                    {
                        g_MapOverlayHdr.playerAnimLock();

                        SysWork_StateSetNext(SysState_GameOver);

                        player->health                  = Q12(100.0f);
                        PLAYER_PROPS().gasWeaponPowerTimer = Q12(0.0f);
                        return;
                    }
                    break;

                case PlayerState_EnemyGrabTorsoFront:
                    unkDistThreshold = Q12(1.0f);

                    if (g_SavegamePtr->gameDifficulty == GameDifficulty_Easy)
                    {
                        grabFreeInputCount = 800;
                    }
                    else if (g_SavegamePtr->gameDifficulty == GameDifficulty_Hard)
                    {
                        grabFreeInputCount = 2400;
                    }
                    else
                    {
                        grabFreeInputCount = 1600;
                    }

                    animStatus            = ANIM_STATUS(HarryAnim_Unk115, false);
                    enemyGrabReleaseState = PlayerState_EnemyReleaseUpperFront;
                    break;

                default:
                    break;

                case PlayerState_EnemyGrabTorsoBack:
                    unkDistThreshold = Q12(1.0f);

                    if (g_SavegamePtr->gameDifficulty == GameDifficulty_Easy)
                    {
                        grabFreeInputCount = 1000;
                    }
                    else if (g_SavegamePtr->gameDifficulty == GameDifficulty_Hard)
                    {
                        grabFreeInputCount = 3000;
                    }
                    else
                    {
                        grabFreeInputCount = 2000;
                    }

                    animStatus            = ANIM_STATUS(HarryAnim_Unk117, false);
                    enemyGrabReleaseState = PlayerState_EnemyReleaseUpperBack;
                    break;

                case PlayerState_EnemyGrabLegsFront:
                    unkDistThreshold = Q12(0.8f);

                    if (g_SavegamePtr->gameDifficulty == GameDifficulty_Easy)
                    {
                        grabFreeInputCount = 700;
                    }
                    else if (g_SavegamePtr->gameDifficulty == GameDifficulty_Hard)
                    {
                        grabFreeInputCount = 2100;
                    }
                    else
                    {
                        grabFreeInputCount = 1400;
                    }

                    animStatus            = ANIM_STATUS(HarryAnim_Unk117, true);
                    enemyGrabReleaseState = PlayerState_EnemyReleaseLowerFront;
                    break;

                case PlayerState_EnemyGrabLegsBack:
                    unkDistThreshold = Q12(0.8f);

                    if (g_SavegamePtr->gameDifficulty == GameDifficulty_Easy)
                    {
                        grabFreeInputCount = 800;
                    }
                    else if (g_SavegamePtr->gameDifficulty == GameDifficulty_Hard)
                    {
                        grabFreeInputCount = 2400;
                    }
                    else
                    {
                        grabFreeInputCount = 1600;
                    }

                    animStatus            = ANIM_STATUS(HarryAnim_Unk118, false);
                    enemyGrabReleaseState = PlayerState_EnemyReleaseLowerBack;
                    break;

                case PlayerState_EnemyGrabNeckFront:
                    unkDistThreshold = Q12(1.5f);

                    if (g_SavegamePtr->gameDifficulty == GameDifficulty_Easy)
                    {
                        grabFreeInputCount = 3600;
                    }
                    else if (g_SavegamePtr->gameDifficulty == GameDifficulty_Hard)
                    {
                        grabFreeInputCount = 10800;
                    }
                    else
                    {
                        grabFreeInputCount = 7200;
                    }

                    animStatus            = ANIM_STATUS(HarryAnim_Unk125, true);
                    enemyGrabReleaseState = PlayerState_EnemyReleaseUpperFront;
                    break;

                case PlayerState_EnemyGrabNeckBack:
                    unkDistThreshold = Q12(1.5f);

                    if (g_SavegamePtr->gameDifficulty == GameDifficulty_Easy)
                    {
                        grabFreeInputCount = 3600;
                    }
                    else if (g_SavegamePtr->gameDifficulty == GameDifficulty_Hard)
                    {
                        grabFreeInputCount = 10800;
                    }
                    else
                    {
                        grabFreeInputCount = 7200;
                    }

                    animStatus            = ANIM_STATUS(HarryAnim_Unk125, true);
                    enemyGrabReleaseState = PlayerState_EnemyReleaseUpperBack;
                    break;
            }

            // Accommodates position of player and enemy?
            switch (PLAYER_EXTRA().state)
            {
                case PlayerState_EnemyGrabTorsoFront:
                case PlayerState_EnemyGrabLegsFront:
                case PlayerState_EnemyGrabNeckFront:
                case PlayerState_EnemyGrabPinnedFront:
                    deltaPosX = player->position.vx - NPC_CHARA(g_SysWork.npcIdxs[0]).position.vx;
                    deltaPosZ = player->position.vz - NPC_CHARA(g_SysWork.npcIdxs[0]).position.vz;
                    npcDist   = Math_Vector2MagCalc(deltaPosX, deltaPosZ);
                    Math_ShortestAngleGet(player->rotation.vy, Q12_ANGLE_NORM_U(NPC_CHARA(g_SysWork.npcIdxs[0]).rotation.vy + Q12_ANGLE(180.0f)), &headingAngle1);

                    if (ABS(headingAngle1) < Q12_ANGLE(11.25f))
                    {
                        player->rotation.vy = NPC_CHARA(g_SysWork.npcIdxs[0]).rotation.vy + Q12_ANGLE(180.0f);
                    }
                    else
                    {
                        if (headingAngle1 > Q12_ANGLE(0.0f))
                        {
                            player->rotation.vy += Q12_ANGLE(11.25f);
                        }
                        else
                        {
                            player->rotation.vy -= Q12_ANGLE(11.25f);
                        }
                    }
                    break;

                case PlayerState_EnemyGrabTorsoBack:
                case PlayerState_EnemyGrabLegsBack:
                case PlayerState_EnemyGrabNeckBack:
                case PlayerState_EnemyGrabPinnedBack:
                    temp_v1_12 = player->position.vx - NPC_CHARA(g_SysWork.npcIdxs[1]).position.vx;
                    temp_v1_13 = player->position.vz - NPC_CHARA(g_SysWork.npcIdxs[1]).position.vz;
                    npcDist     = Math_Vector2MagCalc(temp_v1_12, temp_v1_13);
                    Math_ShortestAngleGet(player->rotation.vy, Q12_ANGLE_NORM_U(NPC_CHARA(g_SysWork.npcIdxs[1]).rotation.vy + Q12_ANGLE(360.0f)), &headingAngle1);

                    if (ABS(headingAngle1) < Q12_ANGLE(11.25f))
                    {
                        player->rotation.vy = NPC_CHARA(g_SysWork.npcIdxs[1]).rotation.vy;
                    }
                    else
                    {
                        if (headingAngle1 > Q12_ANGLE(0.0f))
                        {
                            player->rotation.vy += Q12_ANGLE(11.25f);
                        }
                        else
                        {
                            player->rotation.vy -= Q12_ANGLE(11.25f);
                        }
                    }
                    break;
            }

            switch (PLAYER_EXTRA().state)
            {
                case PlayerState_EnemyGrabPinnedFront:
                    Math_ShortestAngleGet(player->rotation.vy, Q12_ANGLE_NORM_U(NPC_CHARA(g_SysWork.npcIdxs[0]).rotation.vy + Q12_ANGLE(360.0f)), &headingAngle1);

                case PlayerState_EnemyGrabPinnedBack:
                    Math_ShortestAngleGet(player->rotation.vy, Q12_ANGLE_NORM_U(NPC_CHARA(g_SysWork.npcIdxs[1]).rotation.vy + Q12_ANGLE(360.0f)), &headingAngle1);
                    break;
            }

            g_Player_HeadingAngle = headingAngle1;
            func_8007FB94(player, extra, animStatus);

            if (player->health > Q12(0.0f) && (g_Player_HasMoveInput | g_Player_HasActionInput))
            {
                g_Player_GrabReleaseInputTimer += g_DeltaTime;
            }

            // If player isn't thrown to floor (Cybil shoot attack).
            if (!(PLAYER_EXTRA().state >= PlayerState_OnFloorFront &&
                  PLAYER_EXTRA().state <  PlayerState_GetUpFront))
            {
                if (unkDistThreshold < npcDist)
                {
                    g_Player_GrabReleaseInputTimer = grabFreeInputCount;
                    if (PLAYER_EXTRA().state == PlayerState_EnemyGrabPinnedFront)
                    {
                        NPC_CHARA(g_SysWork.npcIdxs[0]).moveSpeed = Q12(0.0f);
                    }

                    if (PLAYER_EXTRA().state == PlayerState_EnemyGrabPinnedBack)
                    {
                        NPC_CHARA(g_SysWork.npcIdxs[1]).moveSpeed = Q12(0.0f);
                    }
                }
            }

            if (g_Player_GrabReleaseInputTimer >= grabFreeInputCount)
            {
                func_8007FD4C(false);
                Player_ExtraStateSet(player, extra, enemyGrabReleaseState);

                player->flags |= CharaFlag_Unk4;
            }
            break;

        case PlayerState_FallForward:
        case PlayerState_FallBackward:
        case PlayerState_EnemyReleaseUpperFront:
        case PlayerState_Unk17:
        case PlayerState_Unk18:
        case PlayerState_DamageHead:
        case PlayerState_EnemyReleaseUpperBack:
        case PlayerState_EnemyReleaseLowerFront:
        case PlayerState_EnemyReleaseLowerBack:
        case PlayerState_EnemyReleasePinnedFront:
        case PlayerState_EnemyReleasePinnedBack:
        case PlayerState_GetUpFront:
        case PlayerState_GetUpBack:
            if (PLAYER_EXTRA().state != PlayerState_FallBackward)
            {
                if (PLAYER_PROPS().moveSpeed != Q12(0.0f))
                {
                    PLAYER_PROPS().moveSpeed -= DIV_FAST(TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f)), 2);
                    if ((PLAYER_PROPS().moveSpeed >> 16) & 1)
                    {
                        PLAYER_PROPS().moveSpeed = Q12(0.0f);
                    }
                }
            }
            else if (PLAYER_PROPS().moveSpeed != Q12(0.0f))
            {
                PLAYER_PROPS().moveSpeed -= DIV_FAST(TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f)), 4);
                if ((PLAYER_PROPS().moveSpeed >> 16) & 1)
                {
                    PLAYER_PROPS().moveSpeed = Q12(0.0f);
                }
            }

            switch (PLAYER_EXTRA().state)
            {
                case PlayerState_GetUpFront:
                    animStatus = ANIM_STATUS(HarryAnim_Unk133, true);
                    break;

                case PlayerState_GetUpBack:
                    animStatus = ANIM_STATUS(HarryAnim_Unk134, false);
                    break;

                case PlayerState_EnemyReleasePinnedFront:
                    animStatus = ANIM_STATUS(HarryAnim_Unk129, true);
                    break;

                case PlayerState_EnemyReleasePinnedBack:
                    animStatus = ANIM_STATUS(HarryAnim_Unk130, false);
                    break;

                case PlayerState_EnemyReleaseUpperFront:
                    animStatus = ANIM_STATUS(HarryAnim_Unk120, false);
                    break;

                case PlayerState_EnemyReleaseUpperBack:
                    animStatus = ANIM_STATUS(HarryAnim_Unk122, false);
                    break;

                case PlayerState_Unk17:
                    animStatus = ANIM_STATUS(HarryAnim_Unk120, true);
                    break;

                case PlayerState_Unk18:
                    animStatus = ANIM_STATUS(HarryAnim_Unk121, true);
                    break;

                case PlayerState_EnemyReleaseLowerFront:
                    animStatus = ANIM_STATUS(HarryAnim_Unk122, true);
                    break;

                case PlayerState_EnemyReleaseLowerBack:
                    animStatus = ANIM_STATUS(HarryAnim_Unk123, false);
                    break;

                case PlayerState_DamageHead:
                    animStatus = ANIM_STATUS(HarryAnim_Unk121, false);
                    break;

                case PlayerState_FallForward:
                    animStatus = ANIM_STATUS(HarryAnim_FallForward, false);
                    break;

                case PlayerState_FallBackward:
                    animStatus = ANIM_STATUS(HarryAnim_FallBackward, false);
                    break;
            }

            if (!(PLAYER_EXTRA().state >= PlayerState_FallForward && PLAYER_EXTRA().state < PlayerState_KickEnemy))
            {
                func_8007FB94(player, extra, animStatus);
            }

            g_Player_PrevMoveSpeed = PLAYER_PROPS().moveSpeed;
            player->flags |= CharaFlag_Unk4;

            switch (PLAYER_EXTRA().state)
            {
                case PlayerState_GetUpFront:
                case PlayerState_GetUpBack:
                    player->damage.amount              = Q12(0.0f);
                    player->properties.player.afkTimer = Q12(0.0f);

                    if (player->model.anim.keyframeIdx == g_MapOverlayHdr.field_38[D_800AF220].keyframeIdx_6)
                    {
                        PLAYER_PROPS().flags &= ~PlayerFlag_DamageReceived;

                        Player_ExtraStateSet(player, extra, PlayerState_None);

                        PLAYER_CHARA().collision.cylinder.outerRadius     = Q12(0.3f);
                        PLAYER_CHARA().collision.cylinder.innerRadius     = DEFAULT_PLAYER_CYLINDER_INNER_RADIUS;
                        PLAYER_CHARA().collision.box.topExtent            = DEFAULT_PLAYER_BOX_TOP;
                        PLAYER_CHARA().collision.shapeOffsets.cylinder.vz = Q12(0.0f);
                        PLAYER_CHARA().collision.shapeOffsets.cylinder.vx = Q12(0.0f);
                        PLAYER_CHARA().collision.shapeOffsets.box.vz      = Q12(0.0f);
                        PLAYER_CHARA().collision.shapeOffsets.box.vx      = Q12(0.0f);
                        PLAYER_CHARA().collision.box.bottomExtent         = Q12(0.0f);
                        PLAYER_CHARA().collision.box.offsetY              = DEFAULT_PLAYER_BOX_OFFSET_Y;
                    }

                    player->attackReceived = NO_VALUE;

                default:
                    break;

                case PlayerState_EnemyReleasePinnedFront:
                case PlayerState_EnemyReleasePinnedBack:
                    player->properties.player.afkTimer             = Q12(0.0f);
                    PLAYER_CHARA().collision.shapeOffsets.cylinder.vz = Q12(0.0f);
                    PLAYER_CHARA().collision.shapeOffsets.cylinder.vx = Q12(0.0f);
                    PLAYER_CHARA().collision.shapeOffsets.box.vz      = Q12(0.0f);
                    PLAYER_CHARA().collision.shapeOffsets.box.vx      = Q12(0.0f);

                    if (ANIM_STATUS_IS_ACTIVE(player->model.anim.status))
                    {
                        player->collision.cylinder.outerRadius = ((player->model.anim.keyframeIdx - g_MapOverlayHdr.field_38[D_800AF220].time) * Q12(0.3f)) / 21;
                    }
                    else
                    {
                        player->collision.cylinder.outerRadius = Q12(0.0f);
                    }

                    if (player->model.anim.keyframeIdx == g_MapOverlayHdr.field_38[D_800AF220].keyframeIdx_6)
                    {
                        PLAYER_PROPS().flags &= ~PlayerFlag_DamageReceived;
                        switch (PLAYER_EXTRA().state)
                        {
                            case PlayerState_EnemyReleasePinnedFront:
                                Player_ExtraStateSet(player, extra, PlayerState_Unk43);
                                break;

                            case PlayerState_EnemyReleasePinnedBack:
                                Player_ExtraStateSet(player, extra, PlayerState_Unk44);
                                break;
                        }

                        PLAYER_CHARA().collision.cylinder.outerRadius     = Q12(0.3f);
                        PLAYER_CHARA().collision.shapeOffsets.cylinder.vz = Q12(0.0f);
                        PLAYER_CHARA().collision.shapeOffsets.cylinder.vx = Q12(0.0f);
                        PLAYER_CHARA().collision.shapeOffsets.box.vz      = Q12(0.0f);
                        PLAYER_CHARA().collision.shapeOffsets.box.vx      = Q12(0.0f);

                        player->attackReceived = NO_VALUE;
                    }
                    break;

                case PlayerState_EnemyReleaseUpperFront:
                case PlayerState_Unk17:
                case PlayerState_Unk18:
                case PlayerState_DamageHead:
                case PlayerState_EnemyReleaseUpperBack:
                case PlayerState_EnemyReleaseLowerFront:
                case PlayerState_EnemyReleaseLowerBack:
                    if (player->model.anim.keyframeIdx == (g_MapOverlayHdr.field_38[D_800AF220].time + Q12(0.001f)))
                    {
                        player->attackReceived = NO_VALUE;
                    }

                    if (player->model.anim.keyframeIdx == g_MapOverlayHdr.field_38[D_800AF220].keyframeIdx_6)
                    {
                        PLAYER_PROPS().flags &= ~PlayerFlag_DamageReceived;

                        Player_ExtraStateSet(player, extra, PlayerState_None);

                        player->collision.cylinder.outerRadius = Q12(0.3f);

                        PLAYER_CHARA().collision.shapeOffsets.cylinder.vz = Q12(0.0f);
                        PLAYER_CHARA().collision.shapeOffsets.cylinder.vx = Q12(0.0f);
                        PLAYER_CHARA().collision.shapeOffsets.box.vz      = Q12(0.0f);
                        PLAYER_CHARA().collision.shapeOffsets.box.vx      = Q12(0.0f);
                    }
                    break;

                case PlayerState_FallForward:
                case PlayerState_FallBackward:
                    if (extra->model.stateStep == 0)
                    {
                        extra->model.anim.status = animStatus;
                        extra->model.stateStep++;
                    }

                    if (player->model.stateStep == 0)
                    {
                        player->model.anim.status = animStatus;
                        player->model.stateStep++;
                    }

                    if (extra->model.controlState == 0 && player->position.vy >= player->properties.player.floorHeight)
                    {
                        extra->model.controlState++;
                        Sfx_WithFlagsPlay(Sfx_HarryFallGasp, &player->position, Q8(1.0f / 8.0f), SfxFlag_None);
                        player->properties.player.field_10C = 128;
                        func_80089470();
                    }

                    if (PLAYER_EXTRA().state == PlayerState_FallForward)
                    {
                        player->properties.player.field_10D = 0;
                        if (ANIM_STATUS_IS_ACTIVE(player->model.anim.status))
                        {
                            PLAYER_CHARA().collision.box.topExtent = g_Harry_FallForwardAnimBoxOffsets[player->model.anim.keyframeIdx - 379][0];
                            PLAYER_CHARA().collision.box.offsetY   = g_Harry_FallForwardAnimBoxOffsets[player->model.anim.keyframeIdx - 379][1];
                        }

                        if (player->model.anim.keyframeIdx == HARRY_BASE_ANIM_INFOS[45].endKeyframeIdx)
                        {
                            if (player->position.vy > Q12(6.5f))
                            {
                                Player_ExtraStateSet(player, extra, PlayerState_Death);
                            }
                            else
                            {
                                Player_ExtraStateSet(player, extra, PlayerState_None);
                            }

                            PLAYER_CHARA().collision.box.topExtent    = DEFAULT_PLAYER_BOX_TOP;
                            PLAYER_CHARA().collision.box.bottomExtent = Q12(0.0f);
                            PLAYER_CHARA().collision.box.offsetY      = DEFAULT_PLAYER_BOX_OFFSET_Y;
                            player->collision.cylinder.outerRadius = Q12(0.3f);
                        }
                    }
                    else
                    {
                        player->properties.player.field_10D = 1;

                        if (ANIM_STATUS_IS_ACTIVE(player->model.anim.status))
                        {
                            PLAYER_CHARA().collision.box.topExtent = g_Harry_FallBackwardAnimBoxOffsets[player->model.anim.keyframeIdx - 418][0];
                            PLAYER_CHARA().collision.box.offsetY   = g_Harry_FallBackwardAnimBoxOffsets[player->model.anim.keyframeIdx - 418][1];
                        }

                        if (player->model.anim.keyframeIdx == HARRY_BASE_ANIM_INFOS[47].endKeyframeIdx)
                        {
                            if (player->position.vy > Q12(6.5f))
                            {
                                Player_ExtraStateSet(player, extra, PlayerState_Death);
                            }
                            else
                            {
                                Player_ExtraStateSet(player, extra, PlayerState_None);
                            }

                            PLAYER_CHARA().collision.box.topExtent    = DEFAULT_PLAYER_BOX_TOP;
                            PLAYER_CHARA().collision.box.bottomExtent = Q12(0.0f);
                            PLAYER_CHARA().collision.box.offsetY      = DEFAULT_PLAYER_BOX_OFFSET_Y;
                            player->collision.cylinder.outerRadius = Q12(0.3f);
                        }
                    }
                    break;
            }

            player->attackReceived = NO_VALUE;
            break;

        case PlayerState_Unk43:
            func_8007FB94(player, extra, ANIM_STATUS(130, true));

            if (player->model.anim.keyframeIdx == g_MapOverlayHdr.field_38[D_800AF220].keyframeIdx_6)
            {
                Player_ExtraStateSet(player, extra, PlayerState_None);
                player->collision.cylinder.innerRadius = DEFAULT_PLAYER_CYLINDER_INNER_RADIUS;
            }
            break;

        case PlayerState_Unk44:
            func_8007FB94(player, extra, ANIM_STATUS(131, false));

            if (player->model.anim.keyframeIdx == g_MapOverlayHdr.field_38[D_800AF220].keyframeIdx_6)
            {
                Player_ExtraStateSet(player, extra, PlayerState_None);
                player->collision.cylinder.innerRadius = DEFAULT_PLAYER_CYLINDER_INNER_RADIUS;
            }
            break;

        case PlayerState_Unk36:
            player->attackReceived = NO_VALUE;
            func_8007FB94(player, extra, ANIM_STATUS(126, false));

            if (ANIM_STATUS_IS_ACTIVE(player->model.anim.status))
            {
                if ((g_MapOverlayHdr.field_38[D_800AF220].time + 12) >= player->model.anim.keyframeIdx)
                {
                    func_80071620(player->model.anim.status, player, g_MapOverlayHdr.field_38[D_800AF220].time + 12, Sfx_Unk1318);
                }
                else
                {
                    func_80071620(player->model.anim.status, player, g_MapOverlayHdr.field_38[D_800AF220].time + 30, Sfx_Unk1319);
                }
            }

            if (player->model.anim.keyframeIdx == g_MapOverlayHdr.field_38[D_800AF220].keyframeIdx_6)
            {
                g_MapOverlayHdr.playerAnimLock();

                SysWork_StateSetNext(SysState_GameOver);
                func_8007E9C4();

                extra->model.controlState++;
                player->health = Q12(100.0f);
                player->model.controlState++;
                PLAYER_PROPS().gasWeaponPowerTimer = Q12(0.0f);
                return;
            }
            break;

        case PlayerState_Death:
            player->attackReceived = NO_VALUE;
            func_8007FB94(player, extra, ANIM_STATUS(101, false));
            player->collision.cylinder.innerRadius = Q12(0.0f);

            if (ANIM_STATUS_IS_ACTIVE(player->model.anim.status))
            {
                if ((g_MapOverlayHdr.field_38[D_800AF220].time + 12) >= player->model.anim.keyframeIdx)
                {
                    func_80071620(player->model.anim.status, player, g_MapOverlayHdr.field_38[D_800AF220].time + 12, Sfx_Unk1318);
                }
                else
                {
                    func_80071620(player->model.anim.status, player, g_MapOverlayHdr.field_38[D_800AF220].time + 32, Sfx_Unk1319);
                }

                temp_a2 = g_Harry_DeathAnimCylinderOffsets[player->model.anim.keyframeIdx - g_MapOverlayHdr.field_38[D_800AF220].time];
                if (player->model.anim.keyframeIdx != g_MapOverlayHdr.field_38[D_800AF220].keyframeIdx_6)
                {
                    var_v1_5 = g_Harry_DeathAnimCylinderOffsets[(player->model.anim.keyframeIdx + 1) - g_MapOverlayHdr.field_38[D_800AF220].time];
                }
                else
                {
                    var_v1_5 = temp_a2;
                }

                cylinderOffset1                                = temp_a2 + Q12_MULT(var_v1_5 - temp_a2, Q12_FRACT(player->model.anim.time));
                PLAYER_CHARA().collision.shapeOffsets.box.vx      = Q12(0.0f);
                PLAYER_CHARA().collision.shapeOffsets.box.vz      = Q12(0.0f);
                PLAYER_CHARA().collision.shapeOffsets.cylinder.vx = Q12_MULT(cylinderOffset1, Math_Sin(player->rotation.vy));
                PLAYER_CHARA().collision.shapeOffsets.cylinder.vz = Q12_MULT(cylinderOffset1, Math_Cos(player->rotation.vy));
                player->collision.cylinder.outerRadius         = Q12(0.3f);
            }

            if (player->model.anim.keyframeIdx == g_MapOverlayHdr.field_38[D_800AF220].keyframeIdx_6)
            {
                if (g_SavegamePtr->mapIdx == MapIdx_MAP0_S00)
                {
                    g_MapOverlayHdr.playerAnimLock();
                    Savegame_EventFlagSet(EventFlag_25);

                    func_8007E9C4();

                    extra->model.controlState++;
                    player->health = Q12(100.0f);
                    player->model.controlState++;
                    PLAYER_PROPS().gasWeaponPowerTimer = Q12(0.0f);
                    return;
                }

                g_MapOverlayHdr.playerAnimLock();

                SysWork_StateSetNext(SysState_GameOver);

                func_8007E9C4();

                extra->model.controlState++;
                player->health = Q12(100.0f);
                player->model.controlState++;
                PLAYER_PROPS().gasWeaponPowerTimer = Q12(0.0f);
                return;
            }
            break;

        case PlayerState_InstantDeath:
            if (extra->model.controlState == 0)
            {
                SD_Call(4731);
            }

            func_8007FB94(player, extra, ANIM_STATUS(101, true));
            player->collision.cylinder.innerRadius = Q12(0.0f);

            if (player->model.anim.keyframeIdx == (g_MapOverlayHdr.field_38[D_800AF220].keyframeIdx_6 - 25))
            {
                g_MapOverlayHdr.playerAnimLock();

                SysWork_StateSetNext(SysState_GameOver);

                func_8007E9C4();

                extra->model.controlState++;
                player->health = Q12(100.0f);
                player->model.controlState++;
                return;
            }
            break;

        case PlayerState_DamageTorsoBack:
        case PlayerState_DamageTorsoFront:
        case PlayerState_DamageTorsoRight:
        case PlayerState_DamageTorsoLeft:
        case PlayerState_DamageFeetFront:
        case PlayerState_DamageFeetBack:
        case PlayerState_DamagePushBack:
        case PlayerState_DamagePushFront:
            switch (PLAYER_EXTRA().state)
            {
                case PlayerState_DamageTorsoBack:
                    func_8007FB94(player, extra, ANIM_STATUS(105, true));
                    break;

                case PlayerState_DamageTorsoFront:
                    func_8007FB94(player, extra, ANIM_STATUS(105, false));
                    break;

                case PlayerState_DamageTorsoRight:
                    func_8007FB94(player, extra, ANIM_STATUS(106, false));
                    break;

                case PlayerState_DamageTorsoLeft:
                    func_8007FB94(player, extra, ANIM_STATUS(106, true));
                    break;

                case PlayerState_DamageFeetFront:
                    func_8007FB94(player, extra, ANIM_STATUS(107, true));
                    break;

                case PlayerState_DamageFeetBack:
                    func_8007FB94(player, extra, ANIM_STATUS(107, false));
                    break;

                case PlayerState_DamagePushBack:
                    func_8007FB94(player, extra, ANIM_STATUS(123, true));
                    break;

                case PlayerState_DamagePushFront:
                    func_8007FB94(player, extra, ANIM_STATUS(124, false));
                    break;
            }

            switch (PLAYER_EXTRA().state)
            {
                case PlayerState_DamagePushBack:
                    Math_ShortestAngleGet(player->rotation.vy, PLAYER_PROPS().field_118, &angle);

                    if (ABS(angle) >= Q12_ANGLE(90.0f))
                    {
                        break;
                    }

                    if (ABS(angle) < Q12_ANGLE(5.7f))
                    {
                        player->rotation.vy = PLAYER_PROPS().field_118;
                    }
                    else
                    {
                        player->rotation.vy += (angle / ABS(angle)) << 6;
                    }
                    break;

                case PlayerState_DamagePushFront:
                    Math_ShortestAngleGet(player->rotation.vy, PLAYER_PROPS().field_118, &sp1E);

                    if (ABS(sp1E) < Q12_ANGLE(90.0f))
                    {
                        break;
                    }

                    if (ABS(sp1E) >= Q12_ANGLE(174.4f))
                    {
                        player->rotation.vy = Q12_ANGLE_NORM_U(PLAYER_PROPS().field_118 + Q12_ANGLE(180.0f));
                    }
                    else
                    {
                        player->rotation.vy -= (sp1E / ABS(sp1E)) << 6;
                    }
                    break;
            }

            if (PLAYER_PROPS().moveSpeed != Q12(0.0f))
            {
                PLAYER_PROPS().moveSpeed -= TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.5f)) >> 2;
                if ((PLAYER_PROPS().moveSpeed >> 16) & 0x1)
                {
                    PLAYER_PROPS().moveSpeed = Q12(0.0f);
                }
            }

            if (player->attackReceived != WEAPON_ATTACK(EquippedWeaponId_Unk68, AttackInputType_Tap) && 
                player->attackReceived != WEAPON_ATTACK(EquippedWeaponId_Unk69, AttackInputType_Tap))
            {
                g_Player_HeadingAngle = ratan2(player->damage.position.vx, player->damage.position.vz) -
                                        player->rotation.vy;
                PLAYER_PROPS().moveSpeed = SQUARE(player->damage.position.vx) +
                                        SQUARE(player->damage.position.vz) +
                                        SQUARE(player->damage.position.vy);
            }

            if (extra->model.anim.keyframeIdx == g_MapOverlayHdr.field_38[D_800AF220].keyframeIdx_6)
            {
                player->attackReceived = NO_VALUE;
                g_SysWork.targetNpcIdx = NO_VALUE;
                PLAYER_PROPS().flags     &= ~PlayerFlag_DamageReceived;

                Player_ExtraStateSet(player, extra, PlayerState_None);

                PLAYER_PROPS().moveSpeed = Q12(0.0f);
            }

            g_Player_PrevMoveSpeed     = PLAYER_PROPS().moveSpeed;
            player->flags |= CharaFlag_Unk4;
            break;

        case PlayerState_KickEnemy:
            func_80070DF0(extra, player, WEAPON_ATTACK(EquippedWeaponId_Kick, AttackInputType_Tap), ANIM_STATUS(24, true));
            break;

        case PlayerState_StompEnemy:
            func_80070DF0(extra, player, WEAPON_ATTACK(EquippedWeaponId_Stomp, AttackInputType_Tap), ANIM_STATUS(25, true));
            break;
    }

    player->rotation.vy      = Q12_ANGLE_NORM_U(player->rotation.vy + (D_800C454C >> 4) + Q12_ANGLE(360.0f));
    player->headingAngle     = Q12_ANGLE_NORM_U((player->rotation.vy + g_Player_HeadingAngle) + Q12_ANGLE(360.0f));
    player->moveSpeed        = g_Player_PrevMoveSpeed;
    player->fallSpeed       += g_GravitySpeed;
    player->rotationSpeed.vy = (D_800C454C << 8) / g_DeltaTime;
    coords->flg              = false;

    Math_RotMatrixZxyNegGte(&player->rotation, &coords->coord);
}

void Player_UpperBodyStateUpdate(s_PlayerExtra* extra, e_PlayerUpperBodyState upperState, s32 unused, s32 arg3) // 0x80073FC0
{
    e_PlayerUpperBodyState prevState;
    s_Model*               charaModel;

    prevState  = PLAYER_EXTRA().upperBodyState;
    charaModel = &PLAYER_CHARA().model;

    switch (PLAYER_EXTRA().lowerBodyState)
    {
        case PlayerLowerBodyState_WalkForward:
            PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_WalkForward;

        default:
            break;

        case PlayerLowerBodyState_RunForward:
            PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_RunForward;
            break;

        case PlayerLowerBodyState_WalkBackward:
            PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_WalkBackward;
            break;

        case PlayerLowerBodyState_SidestepRight:
            PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_SidestepRight;
            break;

        case PlayerLowerBodyState_SidestepLeft:
            PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_SidestepLeft;
            break;

        case PlayerLowerBodyState_RunRight:
            PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_RunRight;
            break;

        case PlayerLowerBodyState_RunLeft:
            PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_RunLeft;
            break;

        case PlayerLowerBodyState_QuickTurnRight:
            PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_QuickTurnRight;
            break;

        case PlayerLowerBodyState_QuickTurnLeft:
            PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_QuickTurnLeft;
            break;

        case PlayerLowerBodyState_JumpBackward:
            PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_RunJumpBackward;
            break;

        case PlayerLowerBodyState_RunForwardWallStop:
            PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_RunWallStop;
            break;

        case PlayerLowerBodyState_Stumble:
            PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_LowerBodyStumble;
            break;

        case PlayerLowerBodyState_RunLeftWallStop:
            PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_RunLeftWallStop;
            break;

        case PlayerLowerBodyState_RunRightWallStop:
            PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_RunRightWallStop;
            break;

        case PlayerLowerBodyState_RunLeftStumble:
            PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_RunLeftStumble;
            break;

        case PlayerLowerBodyState_RunRightStumble:
            PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_SidestepRightStumble;
            break;

        case PlayerLowerBodyState_None:
            switch (arg3)
            {
                case 0:
                case 2:
                    PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_None;
                    break;

                case 1:
                    if (!g_Player_IsTurningRight)
                    {
                        if (g_Player_IsTurningLeft)
                        {
                            PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_TurnLeft;
                        }
                    }
                    else
                    {
                        PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_TurnRight;
                    }
                    break;

                case 3:
                    if (g_Player_IsTurningLeft)
                    {
                        PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_TurnLeft;
                        break;
                    }

                    if (!g_Player_IsTurningRight)
                    {
                        PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_None;
                    }
                    break;

                case 4:
                    if (g_Player_IsTurningRight)
                    {
                        PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_TurnRight;
                        break;
                    }

                    if (!g_Player_IsTurningLeft)
                    {
                        PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_None;
                    }
                    break;
            }
            break;
    }

    if (PLAYER_EXTRA().upperBodyState != upperState)
    {
        extra->model.stateStep    = 0;
        extra->model.controlState = 0;
    }

    switch (prevState)
    {
        case PlayerUpperBodyState_WalkForward:
            if (PLAYER_EXTRA().upperBodyState != PlayerUpperBodyState_RunForward)
            {
                break;
            }

            extra->model.anim.status      = charaModel->anim.status;
            extra->model.anim.keyframeIdx = charaModel->anim.keyframeIdx;
            extra->model.anim.time        = charaModel->anim.time;
            extra->model.stateStep++;
            break;

        case PlayerUpperBodyState_RunForward:
            if (PLAYER_EXTRA().upperBodyState != PlayerUpperBodyState_WalkForward)
            {
                break;
            }

            extra->model.anim.status      = charaModel->anim.status;
            extra->model.anim.keyframeIdx = charaModel->anim.keyframeIdx;
            extra->model.anim.time        = charaModel->anim.time;
            extra->model.stateStep++;
            break;
    }
}

void Player_UpperBodyUpdate(s_SubCharacter* player, s_PlayerExtra* extra) // 0x80074254
{
    s32 stumbleSfxId;

    if (Player_UpperBodyMainUpdate(player, extra))
    {
        return;
    }

    stumbleSfxId = (D_800C45C8.floorType == FloorType_10) ? Sfx_Stumble1 : Sfx_Stumble0;

    switch (PLAYER_EXTRA().upperBodyState)
    {
        case PlayerUpperBodyState_None:
            func_80071620(55, player, 551, Sfx_HarryHeavyBreath);
            break;

        case PlayerUpperBodyState_LowerBodyStumble:
            if (func_80071620(23, player, 173, stumbleSfxId))
            {
                func_8008944C();
            }
            break;

        case PlayerUpperBodyState_RunLeftStumble:
            if (func_80071620(39, player, 340, stumbleSfxId))
            {
                func_8008944C();
            }
            break;

        case PlayerUpperBodyState_SidestepRightStumble:
            if (func_80071620(43, player, 369, stumbleSfxId))
            {
                func_8008944C();
            }
            break;
    }

    Player_CombatStateUpdate(player, extra);
}

bool Player_UpperBodyMainUpdate(s_SubCharacter* player, s_PlayerExtra* extra) // 0x80075504
{
    s32        enemyAttackedIdx;
    s16        sp20;
    s16        sp22;
    s32        currentAmmoVar;
    s32        totalAmmoVar;
    s32        temp_s1_2;
    s16        temp_v0_3;
    s16        temp_v1_3;
    s32        i;
    s16        var_s0;
    s32        playerTurn;
    static s32 D_800C44D0;
    static s32 D_800C44D4;

    bool Player_CombatAnimUpdate(void) // 0x80074350
    {
        s16 ssp20;
        s16 temp_a1;
        s32 keyframeIdx0;
        s32 keyframeIdx1;
        u8  weaponAttack;

        weaponAttack = PLAYER_COMBAT().weaponAttack;

        switch (PLAYER_COMBAT().weaponAttack)
        {
            case WEAPON_ATTACK(EquippedWeaponId_KitchenKnife, AttackInputType_Tap):
                keyframeIdx0 = 619;
                keyframeIdx1 = 613;
                break;

            case WEAPON_ATTACK(EquippedWeaponId_Chainsaw, AttackInputType_Tap):
                keyframeIdx0 = 630;
                keyframeIdx1 = 624;
                break;

            case WEAPON_ATTACK(EquippedWeaponId_RockDrill, AttackInputType_Tap):
                keyframeIdx0 = 568;
                keyframeIdx1 = 568;
                break;

            case WEAPON_ATTACK(EquippedWeaponId_Axe, AttackInputType_Tap):
                keyframeIdx0 = 625;
                keyframeIdx1 = 618;
                break;

            case WEAPON_ATTACK(EquippedWeaponId_SteelPipe, AttackInputType_Tap):
            case WEAPON_ATTACK(EquippedWeaponId_Hammer,    AttackInputType_Tap):
                keyframeIdx0 = 648;
                keyframeIdx1 = 642;
                break;

            case WEAPON_ATTACK(EquippedWeaponId_Katana, AttackInputType_Tap):
                keyframeIdx0 = 619;
                keyframeIdx1 = 612;
                break;

            default:
                keyframeIdx1 = 0;
                keyframeIdx0 = 0;
                break;
        }

        if (PLAYER_EXTRA().state == PlayerState_Combat)
        {
            PLAYER_PROPS().field_104 += g_DeltaTime;

            if (!g_GameWork.config.extraWeaponCtrl)
            {
                g_Player_HasActionInput      = false;
                g_Player_HasMoveInput        = false;
                g_Player_IsShooting          = false;
                g_Player_IsAttacking         = false;
                g_Player_IsHoldAttack        = false;
                g_Player_IsAiming            = false;
                g_Player_IsRunning           = false;
                g_Player_IsMovingBackward    = false;
                g_Player_IsMovingForward     = false;
                g_Player_IsSteppingRightTap  = false;
                g_Player_IsSteppingRightHold = false;
                g_Player_IsTurningRight      = false;
                g_Player_IsSteppingLeftTap   = false;
                g_Player_IsSteppingLeftHold  = false;
                g_Player_IsTurningLeft       = false;
            }
        }

        // Attack type (except melee multitap) and animation.
        if (extra->model.controlState == 0)
        {
            g_Player_MeleeAttackType  = 0;
            g_Player_IsMultiTapAttack = 0;

            PLAYER_PROPS().flags &= ~PlayerFlag_Shooting;
            PLAYER_PROPS().flags &= ~PlayerFlag_Unk6;

            if (PLAYER_COMBAT().weaponAttack >= WEAPON_ATTACK(EquippedWeaponId_Handgun, AttackInputType_Tap))
            {
                g_Player_MeleeAttackType = 0;
                g_Player_AttackAnimIdx   = g_Player_EquippedWeaponInfo.animAttackHold;
                D_800AF220               = g_Player_EquippedWeaponInfo.field_A >> 4;
            }
            else if (g_Player_IsAttacking && PLAYER_COMBAT().weaponAttack != WEAPON_ATTACK(EquippedWeaponId_RockDrill, AttackInputType_Tap))
            {
                g_Player_MeleeAttackType = 1;
                g_Player_AttackAnimIdx   = g_Player_EquippedWeaponInfo.animAttackHold - 4;
                D_800AF220               = (g_Player_EquippedWeaponInfo.field_A >> 4) - 2;
            }
            else
            {
                g_Player_MeleeAttackType = 0;

                // Handle Rock Drill animation.
                if (PLAYER_COMBAT().weaponAttack != WEAPON_ATTACK(EquippedWeaponId_RockDrill, AttackInputType_Tap) ||
                    g_Player_RockDrill_AttackType == RockDrillAttackType_Center)
                {
                    g_Player_AttackAnimIdx = g_Player_EquippedWeaponInfo.animAttackHold;
                    D_800AF220             = g_Player_EquippedWeaponInfo.field_A >> 4;
                }
                else if (g_Player_RockDrill_AttackType == RockDrillAttackType_Down)
                {
                    g_Player_AttackAnimIdx = g_Player_EquippedWeaponInfo.animAttackHold + 4;
                    D_800AF220             = (g_Player_EquippedWeaponInfo.field_A >> 4) + 2;
                }
                else
                {
                    g_Player_AttackAnimIdx = g_Player_EquippedWeaponInfo.animAttackHold + 2;
                    D_800AF220             = (g_Player_EquippedWeaponInfo.field_A >> 4) + 1;
                }
            }

            extra->model.controlState++;

            if (PLAYER_COMBAT().weaponAttack < WEAPON_ATTACK(EquippedWeaponId_Handgun, AttackInputType_Tap))
            {
                PLAYER_COMBAT().weaponAttack = WEAPON_ATTACK(WEAPON_ATTACK_ID_GET(PLAYER_COMBAT().weaponAttack), AttackInputType_Tap);
            }

            PLAYER_COMBAT().weaponAttack = WEAPON_ATTACK(PLAYER_COMBAT().weaponAttack, g_Player_MeleeAttackType);

            D_800C44D0 = HARRY_BASE_ANIM_INFOS[g_Player_AttackAnimIdx].startKeyframeIdx + D_800AD4C8[PLAYER_COMBAT().weaponAttack].field_E;
            D_800C44D4 = HARRY_BASE_ANIM_INFOS[g_Player_AttackAnimIdx].startKeyframeIdx + D_800AD4C8[PLAYER_COMBAT().weaponAttack].field_E +
                         D_800AD4C8[PLAYER_COMBAT().weaponAttack].field_F;
        }

        // Used for make continuos/hold shooting smoother?
        if (g_SysWork.targetNpcIdx != NO_VALUE &&
            PLAYER_COMBAT().weaponAttack >= WEAPON_ATTACK(EquippedWeaponId_Handgun, AttackInputType_Tap))
        {
            if (!g_GameWork.config.extraAutoAiming)
            {
                if (!WorldEnv_IsInDarkness())
                {
                    func_8005CD38(&enemyAttackedIdx, &PLAYER_PROPS().field_122, &PLAYER_COMBAT().attackPosition, Q12_ANGLE(50.0f), Q12(10.0f), 0);
                    func_8005D50C(&g_Player_TargetNpcIdx, &g_Player_RotationToTargetX, &g_Player_RotationToTargetY, &PLAYER_COMBAT().attackPosition, enemyAttackedIdx, Q12_ANGLE(20.0f));
                }
                else
                {
                    func_8005CD38(&enemyAttackedIdx, &PLAYER_PROPS().field_122, &PLAYER_COMBAT().attackPosition, Q12_ANGLE(50.0f), Q12(3.0f), 0);
                    func_8005D50C(&g_Player_TargetNpcIdx, &g_Player_RotationToTargetX, &g_Player_RotationToTargetY, &PLAYER_COMBAT().attackPosition, enemyAttackedIdx, Q12_ANGLE(20.0f));
                }
            }
            else
            {
                enemyAttackedIdx = g_SysWork.targetNpcIdx;
            }

            if (enemyAttackedIdx == NO_VALUE && enemyAttackedIdx == g_Player_TargetNpcIdx)
            {
                g_Player_RotationToTargetY = NO_VALUE;
                g_Player_RotationToTargetX = NO_VALUE;
            }

            if (enemyAttackedIdx == g_SysWork.targetNpcIdx)
            {
                player->angleToTarget = Q12_FRACT(ratan2((NPC_CHARA(enemyAttackedIdx).position.vx + NPC_CHARA(enemyAttackedIdx).collision.shapeOffsets.box.vx) - PLAYER_CHARA().position.vx,
                                                         (NPC_CHARA(enemyAttackedIdx).position.vz + NPC_CHARA(enemyAttackedIdx).collision.shapeOffsets.box.vz) - PLAYER_CHARA().position.vz) +
                                                  Q12_ANGLE(360.0f));
            }
            else
            {
                player->angleToTarget = player->rotation.vy;
            }

            if (extra->model.stateStep == 0)
            {
                extra->model.anim.status = ANIM_STATUS(HarryAnim_Unk30, false);
                extra->model.stateStep++;
            }
        }
        else
        {
            if (g_SysWork.targetNpcIdx != NO_VALUE && !g_GameWork.config.extraAutoAiming)
            {
                if (!WorldEnv_IsInDarkness())
                {
                    func_8005CD38(&enemyAttackedIdx, &PLAYER_PROPS().field_122, &PLAYER_COMBAT().attackPosition, Q12(3.0f), Q12(3.0f), 5);
                }
                else
                {
                    func_8005CD38(&enemyAttackedIdx, &PLAYER_PROPS().field_122, &PLAYER_COMBAT().attackPosition, Q12(1.0f), Q12(1.0f), 5);
                }

                if (enemyAttackedIdx == g_SysWork.targetNpcIdx)
                {
                    temp_a1 = Q12_FRACT(ratan2((NPC_CHARA(enemyAttackedIdx).position.vx + NPC_CHARA(enemyAttackedIdx).collision.shapeOffsets.box.vx) - PLAYER_CHARA().position.vx,
                                               (NPC_CHARA(enemyAttackedIdx).position.vz + NPC_CHARA(enemyAttackedIdx).collision.shapeOffsets.box.vz) - PLAYER_CHARA().position.vz) + Q12(1.0f));

                    Math_ShortestAngleGet(player->rotation.vy, temp_a1, &ssp20);
                    D_800C454C = g_DeltaTime * 15;

                    if (ABS(ssp20) >= 0x80)
                    {
                        if (ssp20 < 0)
                        {
                            D_800C454C = -D_800C454C;
                        }
                    }
                    else
                    {
                        player->angleToTarget = 
                        player->rotation.vy   = temp_a1;
                        D_800C454C            = Q12(0.0f);
                    }
                }
            }
            else
            {
                enemyAttackedIdx = NO_VALUE;
                PLAYER_PROPS().field_122 = Q12_ANGLE(90.0f);
                player->angleToTarget = player->rotation.vy;
            }

            if (PLAYER_COMBAT().weaponAttack >= WEAPON_ATTACK(EquippedWeaponId_Handgun, AttackInputType_Tap))
            {
                if (PLAYER_PROPS().flags & PlayerFlag_Unk11)
                {
                    if (extra->model.stateStep == 0)
                    {
                        extra->model.anim.status = g_Player_EquippedWeaponInfo.animAttack - 12;
                        extra->model.stateStep++;
                    }
                }
                else
                {
                    if (extra->model.stateStep == 0)
                    {
                        extra->model.anim.status = g_Player_EquippedWeaponInfo.animAttack;
                        extra->model.stateStep++;
                    }
                }
            }
            else if (g_Player_IsAttacking && PLAYER_COMBAT().weaponAttack != WEAPON_ATTACK(EquippedWeaponId_RockDrill, AttackInputType_Tap))
            {
                if (extra->model.stateStep == 0)
                {
                    extra->model.anim.status = g_Player_EquippedWeaponInfo.animAttack - 4;
                    extra->model.stateStep++;
                }
            }
            else if (PLAYER_COMBAT().weaponAttack != WEAPON_ATTACK(EquippedWeaponId_RockDrill, AttackInputType_Tap) ||
                     g_Player_RockDrill_AttackType == RockDrillAttackType_Center)
            {
                if (extra->model.stateStep == 0)
                {
                    extra->model.anim.status = g_Player_EquippedWeaponInfo.animAttack;
                    extra->model.stateStep++;
                }
            }
            else if (g_Player_RockDrill_AttackType == RockDrillAttackType_Down)
            {
                if (extra->model.stateStep == 0)
                {
                    extra->model.anim.status = g_Player_EquippedWeaponInfo.animAttack + 4;
                    extra->model.stateStep++;
                }
            }
            else
            {
                if (extra->model.stateStep == 0)
                {
                    extra->model.anim.status = g_Player_EquippedWeaponInfo.animAttack + 2;
                    extra->model.stateStep++;
                }
            }
        }

        // Audio effects for attack animations, no ammo audio and removes ammo value.
        // Additionally trigger some special state for the Rock Drill.
        if (PLAYER_COMBAT().weaponAttack < WEAPON_ATTACK(EquippedWeaponId_Handgun, AttackInputType_Tap))
        {
            if (WEAPON_ATTACK_ID_GET(PLAYER_COMBAT().weaponAttack) != WEAPON_ATTACK(EquippedWeaponId_Chainsaw,  AttackInputType_Tap) &&
                WEAPON_ATTACK_ID_GET(PLAYER_COMBAT().weaponAttack) != WEAPON_ATTACK(EquippedWeaponId_RockDrill, AttackInputType_Tap))
            {
                if (extra->model.anim.keyframeIdx >= D_800C44D0 && D_800C44D4 >= extra->model.anim.keyframeIdx)
                {
                    if (!(PLAYER_PROPS().flags & PlayerFlag_Unk2))
                    {
                        player->field_44.field_0 = 1;

                        Sfx_WithFlagsPlay(g_Player_EquippedWeaponInfo.attackSfx, &player->position, Q8(0.5f), SfxFlag_None);

                        player->properties.player.field_10C = 0x40;
                        PLAYER_PROPS().flags                  |= PlayerFlag_Unk2;
                    }
                }
                else if (D_800C44D4 < extra->model.anim.keyframeIdx)
                {
                    PLAYER_PROPS().flags &= ~PlayerFlag_Unk2;
                }
            }
            else if (PLAYER_PROPS().gasWeaponPowerTimer == Q12(0.0f))
            {
                if (extra->model.anim.keyframeIdx >= D_800C44D0 && D_800C44D4 >= extra->model.anim.keyframeIdx &&
                    !(PLAYER_PROPS().flags & PlayerFlag_Unk2))
                {
                    player->field_44.field_0 = 1;
                    PLAYER_PROPS().flags       |= PlayerFlag_Unk2;
                }
            }
            else
            {
                if (player->field_44.field_0 <= 0)
                {
                    player->field_44.field_0 = 1;
                }

                player->properties.player.field_10C = 0x40;
            }
        }
        else
        {
            if (extra->model.anim.keyframeIdx >= D_800C44D0 && D_800C44D4 >= extra->model.anim.keyframeIdx &&
                !(PLAYER_PROPS().flags & PlayerFlag_Shooting))
            {
                PLAYER_PROPS().flags |= PlayerFlag_Shooting;

                if (PLAYER_COMBAT().currentWeaponAmmo != 0)
                {
                    player->field_44.field_0 = 1;

                    if (PLAYER_COMBAT().weaponAttack != WEAPON_ATTACK(EquippedWeaponId_HyperBlaster, AttackInputType_Tap))
                    {
                        PLAYER_COMBAT().currentWeaponAmmo--;
                        g_SavegamePtr->items[PLAYER_COMBAT().weaponInventoryIdx].count--;

                        Sfx_WithFlagsPlay(g_Player_EquippedWeaponInfo.attackSfx, &player->position, Q8(0.5f), SfxFlag_None);
                    }
                    else
                    {
                        Sfx_WithFlagsPlay(g_Player_EquippedWeaponInfo.attackSfx, &player->position, Q8_CLAMPED(0.19f), SfxFlag_None);
                    }

                    player->properties.player.field_10C = 200;
                }
                else
                {
                    Sfx_WithFlagsPlay(g_Player_EquippedWeaponInfo.outOfAmmoSfx, &player->position, Q8(0.5f), SfxFlag_None);

                    player->properties.player.field_10C = 32;
                    extra->model.anim.keyframeIdx = D_800C44F0[D_800AF220].endKeyframeIdx - 3;
                    extra->model.anim.time        = Q12(D_800C44F0[D_800AF220].endKeyframeIdx - 3);

                    if (PLAYER_EXTRA().lowerBodyState == PlayerLowerBodyState_Aim)
                    {
                        player->model.anim.keyframeIdx = D_800C44F0[D_800AF220].endKeyframeIdx - 3;
                        player->model.anim.time        = Q12(D_800C44F0[D_800AF220].endKeyframeIdx - 3);
                    }
                }
            }
        }

        // Finish attack animation.
        // Though more context about `D_800AF220` and `D_800C44F0` is required,
        // they likely indicate if an attack animation has finished.
        if (PLAYER_COMBAT().weaponAttack < WEAPON_ATTACK(EquippedWeaponId_Handgun, AttackInputType_Tap))
        {
            // Attack anim.
            if (extra->model.anim.status == ANIM_STATUS(HarryAnim_Unk29, true) ||
                extra->model.anim.status == ANIM_STATUS(HarryAnim_Unk30, true) ||
                extra->model.anim.status == ANIM_STATUS(HarryAnim_HandgunRecoil, true) ||
                extra->model.anim.status == ANIM_STATUS(HarryAnim_Unk29, true) ||
                extra->model.anim.status == ANIM_STATUS(HarryAnim_Unk30, true) ||
                extra->model.anim.status == ANIM_STATUS(HarryAnim_HandgunRecoil, true) ||
                extra->model.anim.status == ANIM_STATUS(HarryAnim_Unk29, true) ||
                extra->model.anim.status == ANIM_STATUS(HarryAnim_Unk30, true) ||
                extra->model.anim.status == ANIM_STATUS(HarryAnim_HandgunRecoil, true))
            {
                if (extra->model.anim.keyframeIdx == D_800C44F0[D_800AF220].endKeyframeIdx)
                {
                    extra->model.anim.status      = ANIM_STATUS(HarryAnim_HandgunAim, true);
                    extra->model.anim.keyframeIdx = D_800C44F0[0].endKeyframeIdx;
                    extra->model.anim.time        = Q12(extra->model.anim.keyframeIdx);

                    if (PLAYER_PROPS().flags & PlayerFlag_Unk0)
                    {
                        PLAYER_EXTRA().state          = PlayerState_Combat;
                        PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_AimTargetLock;

                        if (PLAYER_EXTRA().lowerBodyState == PlayerLowerBodyState_Attack)
                        {
                            PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_Aim;
                        }
                    }
                    else
                    {
                        PLAYER_EXTRA().state          = PlayerState_None;
                        PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_Aim;
                        extra->model.controlState  = 
                        extra->model.stateStep     = 0;

                        if (PLAYER_EXTRA().lowerBodyState == PlayerLowerBodyState_Attack)
                        {
                            PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_Aim;
                            PLAYER_PROPS().flags         &= ~PlayerFlag_Unk10;
                        }
                    }

                    PLAYER_PROPS().field_104     = 0;
                    PLAYER_PROPS().flags        &= ~PlayerFlag_Unk2;
                    PLAYER_PROPS().flags        &= ~PlayerFlag_Shooting;
                    PLAYER_COMBAT().weaponAttack = WEAPON_ATTACK(WEAPON_ATTACK_ID_GET(PLAYER_COMBAT().weaponAttack), AttackInputType_Tap);
                    return true;
                }
            }
        }
        // Attack anim.
        else if ((extra->model.anim.status == ANIM_STATUS(HarryAnim_Unk30, true) ||
                  extra->model.anim.status == ANIM_STATUS(HarryAnim_Unk36, true)) &&
                 extra->model.anim.keyframeIdx == D_800C44F0[D_800AF220].endKeyframeIdx)
        {
            if (PLAYER_PROPS().flags & PlayerFlag_Unk0)
            {
                PLAYER_EXTRA().state          = PlayerState_Combat;
                PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_AimTargetLock;

                if (PLAYER_EXTRA().lowerBodyState == PlayerLowerBodyState_Attack)
                {
                    PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_Aim;
                }
            }
            else
            {
                PLAYER_EXTRA().state          = PlayerState_None;
                PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_Aim;
                extra->model.controlState  = extra->model.stateStep = 0;

                if (PLAYER_EXTRA().lowerBodyState == PlayerLowerBodyState_Attack)
                {
                    PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_Aim;
                    PLAYER_PROPS().flags         &= ~PlayerFlag_Unk10;
                    player->model.controlState = 
                    player->model.stateStep    = 0;
                }
            }

            g_Player_RotationToTargetY            = NO_VALUE;
            g_Player_RotationToTargetX            = NO_VALUE;
            PLAYER_PROPS().field_104 = 0;
            PLAYER_PROPS().flags    &= ~PlayerFlag_Shooting;
            return true;
        }

        PLAYER_PROPS().flags |= PlayerFlag_Unk6;

        // Handles multitap attack.
        if (PLAYER_COMBAT().weaponAttack < WEAPON_ATTACK(EquippedWeaponId_Handgun, AttackInputType_Tap))
        {
            if (g_Player_MeleeAttackType == 0 && PLAYER_COMBAT().weaponAttack != WEAPON_ATTACK(EquippedWeaponId_RockDrill, AttackInputType_Tap))
            {
                if (extra->model.anim.keyframeIdx >= keyframeIdx1 &&
                    extra->model.anim.keyframeIdx < keyframeIdx0 &&
                    extra->model.anim.status == ANIM_STATUS(HarryAnim_HandgunRecoil, true) &&
                    (g_Player_IsAttacking || g_Player_IsShooting))
                {
                    g_Player_IsMultiTapAttack = true;
                }
            }
        }

        if (g_Player_IsMultiTapAttack)
        {
            if (extra->model.anim.status == ANIM_STATUS(HarryAnim_HandgunRecoil, true) &&
                extra->model.anim.keyframeIdx >= keyframeIdx0)
            {
                extra->model.stateStep = 0;

                if (PLAYER_EXTRA().lowerBodyState == PlayerLowerBodyState_Attack)
                {
                    player->model.stateStep = 0;
                }

                g_Player_AttackAnimIdx              = g_Player_EquippedWeaponInfo.animAttackHold - 2;
                D_800AF220                          = (g_Player_EquippedWeaponInfo.field_A >> 4) - 1;
                g_Player_MeleeAttackType            = 2;
                PLAYER_COMBAT().weaponAttack = WEAPON_ATTACK(WEAPON_ATTACK_ID_GET(weaponAttack), AttackInputType_Multitap);

                if (extra->model.stateStep == 0)
                {
                    extra->model.anim.status = g_Player_EquippedWeaponInfo.animAttack - 2;
                    extra->model.stateStep++;
                }

                D_800C44D0 = HARRY_BASE_ANIM_INFOS[g_Player_AttackAnimIdx].startKeyframeIdx + D_800AD4C8[PLAYER_COMBAT().weaponAttack].field_E;
                D_800C44D4 = HARRY_BASE_ANIM_INFOS[g_Player_AttackAnimIdx].startKeyframeIdx + D_800AD4C8[PLAYER_COMBAT().weaponAttack].field_E +
                             D_800AD4C8[PLAYER_COMBAT().weaponAttack].field_F;
                g_Player_IsMultiTapAttack = 0;

                PLAYER_PROPS().flags &= ~PlayerFlag_Unk2;
            }
        }

        return false;
    }

    enemyAttackedIdx = NO_VALUE;

    if (PLAYER_EXTRA().upperBodyState != PlayerUpperBodyState_AimTargetLock && PLAYER_EXTRA().upperBodyState != PlayerUpperBodyState_Attack)
    {
        PLAYER_PROPS().field_104 = 0;
    }

    switch (PLAYER_EXTRA().upperBodyState)
    {
        case PlayerUpperBodyState_None:
            if ((extra->model.anim.status == ANIM_STATUS(HarryAnim_WalkForward, true) ||
                 extra->model.anim.status == ANIM_STATUS(HarryAnim_RunForward, true)) &&
                extra->model.stateStep != 0)
            {
                extra->model.stateStep = 0;
            }

            // Set idle animation.
            if (player->properties.player.exhaustionTimer < Q12(10.0f) && player->health >= Q12(30.0f))
            {
                if (extra->model.stateStep == 0)
                {
                    extra->model.anim.status = ANIM_STATUS(HarryAnim_Idle, false);
                    extra->model.stateStep++;
                }

                Player_UpperBodyStateUpdate(extra, PlayerUpperBodyState_None, 53, 1);
            }
            else
            {
                player->properties.player.afkTimer = Q12(0.0f);

                // If not normal idle anim, set it and update `upperBodyState`.
                if (extra->model.anim.status != ANIM_STATUS(HarryAnim_Idle, true))
                {
                    if (extra->model.stateStep == 0)
                    {
                        extra->model.anim.status = ANIM_STATUS(HarryAnim_Idle, false);
                        extra->model.stateStep++;
                    }

                    Player_UpperBodyStateUpdate(extra, PlayerUpperBodyState_None, 55, 1);
                }
                else
                {
                    extra->model.stateStep = 0;
                    if (extra->model.stateStep == 0)
                    {
                        extra->model.anim.status = ANIM_STATUS(HarryAnim_IdleExhausted, false);
                        extra->model.stateStep++;
                    }
                }
            }

            if (PLAYER_EXTRA().upperBodyState != PlayerUpperBodyState_None)
            {
                player->properties.player.afkTimer = Q12(0.0f);
            }

            player->angleToTarget = player->rotation.vy;

            if (PLAYER_EXTRA().upperBodyState == PlayerUpperBodyState_None)
            {
                player->properties.player.afkTimer++;

                if (player->properties.player.afkTimer >= 300)
                {
                    if (player->health >= Q12(60.0f))
                    {
                        player->properties.player.afkTimer        = Q12(0.0f);
                          // TODO: `Player_ExtraStateSet` doesn't match?
                        PLAYER_EXTRA().state          = PlayerState_Idle;
                        player->model.controlState                = 
                        player->model.stateStep                   = 0;
                        extra->model.controlState                 = 
                        extra->model.stateStep                    = 0;
                        PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_None;
                        PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_None;
                        return true;
                    }
                }
            }
            break;

        default:
            break;

        case PlayerUpperBodyState_WalkForward:
            if (extra->model.stateStep == 0)
            {
                extra->model.anim.status = ANIM_STATUS(HarryAnim_WalkForward, false);
                extra->model.stateStep++;
            }

            if (extra->model.anim.status == ANIM_STATUS(HarryAnim_WalkForward, true))
            {
                extra->model.anim.time = player->model.anim.time;
            }

            Player_UpperBodyStateUpdate(extra, PlayerUpperBodyState_WalkForward, 5, 0);
            player->angleToTarget = player->rotation.vy;
            break;

        case PlayerUpperBodyState_RunForward:
            if (extra->model.stateStep == 0)
            {
                extra->model.anim.status = ANIM_STATUS(HarryAnim_RunForward, false);
                extra->model.stateStep++;
            }

            if (extra->model.anim.status == ANIM_STATUS(HarryAnim_RunForward, true))
            {
                extra->model.anim.time = player->model.anim.time;
            }

            Player_UpperBodyStateUpdate(extra, PlayerUpperBodyState_RunForward, 7, 2);
            player->angleToTarget = player->rotation.vy;
            break;

        case PlayerUpperBodyState_RunWallStop:
            if (PLAYER_PROPS().flags & PlayerFlag_WallStopRight)
            {
                if (extra->model.stateStep == 0)
                {
                    extra->model.anim.status = ANIM_STATUS(HarryAnim_RunForwardWallStopRight, false);
                    extra->model.stateStep++;
                }
            }
            else if (extra->model.stateStep == 0)
            {
                extra->model.anim.status = ANIM_STATUS(HarryAnim_RunForwardWallStopLeft, false);
                extra->model.stateStep++;
            }

            Player_UpperBodyStateUpdate(extra, PlayerUpperBodyState_RunWallStop, 19, 0);
            Player_UpperBodyStateUpdate(extra, PlayerUpperBodyState_RunWallStop, 21, 0);
            player->angleToTarget = player->rotation.vy;
            break;

        case PlayerUpperBodyState_SidestepRight:
            if (extra->model.stateStep == 0)
            {
                extra->model.anim.status = ANIM_STATUS(HarryAnim_SidestepRight, false);
                extra->model.stateStep++;
            }

            Player_UpperBodyStateUpdate(extra, PlayerUpperBodyState_SidestepRight, 13, 0);
            player->angleToTarget = player->rotation.vy;
            break;

        case PlayerUpperBodyState_SidestepLeft:
            if (extra->model.stateStep == 0)
            {
                extra->model.anim.status = ANIM_STATUS(HarryAnim_SidestepLeft, false);
                extra->model.stateStep++;
            }

            Player_UpperBodyStateUpdate(extra, PlayerUpperBodyState_SidestepLeft, 11, 0);
            player->angleToTarget = player->rotation.vy;
            break;

        case PlayerUpperBodyState_RunRight:
            if (extra->model.stateStep == 0)
            {
                extra->model.anim.status = ANIM_STATUS(HarryAnim_RunRight, false);
                extra->model.stateStep++;
            }

            Player_UpperBodyStateUpdate(extra, PlayerUpperBodyState_RunRight, 17, 0);
            player->angleToTarget = player->rotation.vy;

            if (extra->model.anim.status == ANIM_STATUS(HarryAnim_RunRight, true))
            {
                extra->model.anim.time = player->model.anim.time;
            }
            break;

        case PlayerUpperBodyState_RunLeft:
            if (extra->model.stateStep == 0)
            {
                extra->model.anim.status = ANIM_STATUS(HarryAnim_RunLeft, false);
                extra->model.stateStep++;
            }

            Player_UpperBodyStateUpdate(extra, PlayerUpperBodyState_RunLeft, 15, 0);
            player->angleToTarget = player->rotation.vy;

            if (extra->model.anim.status == ANIM_STATUS(HarryAnim_RunLeft, true))
            {
                extra->model.anim.time = player->model.anim.time;
            }
            break;

        case PlayerUpperBodyState_WalkBackward:
            if (extra->model.stateStep == 0)
            {
                extra->model.anim.status = ANIM_STATUS(HarryAnim_WalkBackward, false);
                extra->model.stateStep++;
            }

            Player_UpperBodyStateUpdate(extra, PlayerUpperBodyState_WalkBackward, 9, 0);
            player->angleToTarget = player->rotation.vy;
            break;

        case PlayerUpperBodyState_QuickTurnRight:
            if (extra->model.stateStep == 0)
            {
                extra->model.anim.status = ANIM_STATUS(HarryAnim_QuickTurnRight, false);
                extra->model.stateStep++;
            }

            Player_UpperBodyStateUpdate(extra, PlayerUpperBodyState_QuickTurnRight, 29, 0);

            if (extra->model.anim.status == ANIM_STATUS(HarryAnim_QuickTurnRight, true))
            {
                extra->model.anim.time = player->model.anim.time;
            }

            player->angleToTarget = player->rotation.vy;
            break;

        case PlayerUpperBodyState_QuickTurnLeft:
            if (extra->model.stateStep == 0)
            {
                extra->model.anim.status = ANIM_STATUS(HarryAnim_QuickTurnLeft, false);
                extra->model.stateStep++;
            }

            Player_UpperBodyStateUpdate(extra, PlayerUpperBodyState_QuickTurnLeft, 31, 0);

            if (extra->model.anim.status == ANIM_STATUS(HarryAnim_QuickTurnLeft, true))
            {
                extra->model.anim.time = player->model.anim.time;
            }

            player->angleToTarget = player->rotation.vy;
            break;

        case PlayerUpperBodyState_TurnRight:
            if (extra->model.stateStep == 0)
            {
                extra->model.anim.status = ANIM_STATUS(HarryAnim_TurnRight, false);
                extra->model.stateStep++;
            }

            Player_UpperBodyStateUpdate(extra, PlayerUpperBodyState_TurnRight, 27, 3);
            player->angleToTarget = player->rotation.vy;
            break;

        case PlayerUpperBodyState_TurnLeft:
            if (extra->model.stateStep == 0)
            {
                extra->model.anim.status = ANIM_STATUS(HarryAnim_TurnLeft, false);
                extra->model.stateStep++;
            }

            Player_UpperBodyStateUpdate(extra, PlayerUpperBodyState_TurnLeft, 25, 4);
            player->angleToTarget = player->rotation.vy;
            break;

        case PlayerUpperBodyState_RunJumpBackward:
            if (extra->model.stateStep == 0)
            {
                extra->model.anim.status = ANIM_STATUS(HarryAnim_JumpBackward, false);
                extra->model.stateStep++;
            }

            if (extra->model.anim.status == ANIM_STATUS(HarryAnim_JumpBackward, true))
            {
                extra->model.anim.time = player->model.anim.time;
            }

            Player_UpperBodyStateUpdate(extra, PlayerUpperBodyState_RunJumpBackward, 33, 0);
            player->angleToTarget = player->rotation.vy;
            break;

        case PlayerUpperBodyState_LowerBodyStumble:
            if (extra->model.stateStep == 0)
            {
                extra->model.anim.status = ANIM_STATUS(HarryAnim_RunForwardStumble, false);
                extra->model.stateStep++;
            }

            if (extra->model.anim.status == ANIM_STATUS(HarryAnim_RunForwardStumble, true))
            {
                extra->model.anim.time = player->model.anim.time;
            }

            Player_UpperBodyStateUpdate(extra, PlayerUpperBodyState_LowerBodyStumble, 23, 0);
            player->angleToTarget = player->rotation.vy;
            break;

        case PlayerUpperBodyState_RunLeftWallStop:
            if (extra->model.stateStep == 0)
            {
                extra->model.anim.status = ANIM_STATUS(HarryAnim_RunLeftWallStop, false);
                extra->model.stateStep++;
            }

            Player_UpperBodyStateUpdate(extra, PlayerUpperBodyState_RunLeftWallStop, 0x25, 0);
            player->angleToTarget = player->rotation.vy;
            break;

        case PlayerUpperBodyState_RunRightWallStop:
            if (extra->model.stateStep == 0)
            {
                extra->model.anim.status = ANIM_STATUS(HarryAnim_RunRightWallStop, false);
                extra->model.stateStep++;
            }

            Player_UpperBodyStateUpdate(extra, PlayerUpperBodyState_RunRightWallStop, 41, 0);
            player->angleToTarget = player->rotation.vy;
            break;

        case PlayerUpperBodyState_RunLeftStumble:
            if (extra->model.stateStep == 0)
            {
                extra->model.anim.status = ANIM_STATUS(HarryAnim_RunLeftStumble, false);
                extra->model.stateStep++;
            }

            if (extra->model.anim.status == ANIM_STATUS(HarryAnim_RunLeftStumble, true))
            {
                extra->model.anim.time = player->model.anim.time;
            }

            Player_UpperBodyStateUpdate(extra, PlayerUpperBodyState_RunLeftStumble, 39, 0);
            player->angleToTarget = player->rotation.vy;
            break;

        case PlayerUpperBodyState_SidestepRightStumble:
            if (extra->model.stateStep == 0)
            {
                extra->model.anim.status = ANIM_STATUS(HarryAnim_RunRightStumble, false);
                extra->model.stateStep++;
            }

            if (extra->model.anim.status == ANIM_STATUS(HarryAnim_RunRightStumble, true))
            {
                extra->model.anim.time = player->model.anim.time;
            }

            Player_UpperBodyStateUpdate(extra, PlayerUpperBodyState_SidestepRightStumble, 43, 0);
            player->angleToTarget = player->rotation.vy;
            break;

        case PlayerUpperBodyState_Aim:
            g_SysWork.targetNpcIdx = NO_VALUE;

            if (PLAYER_COMBAT().weaponAttack == WEAPON_ATTACK(EquippedWeaponId_Chainsaw,  AttackInputType_Tap) ||
                PLAYER_COMBAT().weaponAttack == WEAPON_ATTACK(EquippedWeaponId_RockDrill, AttackInputType_Tap))
            {
                if (PLAYER_PROPS().gasWeaponPowerTimer != Q12(0.0f))
                {
                    if (player->field_44.field_0 <= 0)
                    {
                        player->field_44.field_0 = 1;
                    }

                    if (extra->model.stateStep == 0)
                    {
                        extra->model.anim.status = ANIM_STATUS(HarryAnim_Unk34, false);
                        extra->model.stateStep++;
                    }
                }
                else
                {
                    extra->model.anim.status      = ANIM_STATUS(HarryAnim_HandgunAim, true);
                    extra->model.anim.keyframeIdx = D_800AF5C6;
                    extra->model.anim.time        = D_800AF5C6 << 12;
                }
            }

            PLAYER_PROPS().flags &= ~PlayerFlag_Unk6;
            player->angleToTarget = player->rotation.vy;

            if (PLAYER_COMBAT().weaponAttack >= WEAPON_ATTACK(EquippedWeaponId_Handgun, AttackInputType_Tap))
            {
                // Aim.
                if (extra->model.anim.status != ANIM_STATUS(HarryAnim_HandgunAim, true) &&
                    extra->model.anim.status != ANIM_STATUS(HarryAnim_Unk34, true))
                {
                    if (g_Player_IsAttacking || g_Player_IsShooting)
                    {
                        PLAYER_PROPS().flags |= PlayerFlag_Unk11;
                    }
                    else
                    {
                        PLAYER_PROPS().flags &= ~PlayerFlag_Unk11;

                        if (extra->model.stateStep == 0)
                        {
                            extra->model.anim.status = ANIM_STATUS(HarryAnim_Unk34, false);
                            extra->model.stateStep++;
                        }
                    }
                }
                else
                {
                    PLAYER_PROPS().flags &= ~PlayerFlag_Unk11;
                }
            }
            break;

        case PlayerUpperBodyState_AimTargetLock:
            PLAYER_PROPS().field_104 += g_DeltaTime;
            PLAYER_PROPS().flags &= ~PlayerFlag_Unk6;

            if (g_Player_IsTurningRight)
            {
                playerTurn = 1;
            }
            else
            {
                playerTurn = (g_Player_IsTurningLeft != false) * 2;
            }

            if ((extra->model.anim.status != ANIM_STATUS(HarryAnim_Unk29, true) || extra->model.anim.keyframeIdx != D_800C44F0[1].endKeyframeIdx) &&
                (extra->model.anim.status != ANIM_STATUS(HarryAnim_Unk30, true) || extra->model.anim.keyframeIdx != D_800C44F0[2].endKeyframeIdx) &&
                (extra->model.anim.status != ANIM_STATUS(HarryAnim_Unk32, true) || extra->model.anim.keyframeIdx != D_800C44F0[4].endKeyframeIdx))
            {
                playerTurn = 0;
                player->properties.player.field_100++;
            }
            else
            {
                player->properties.player.field_100 = 0;
            }

            if (playerTurn != 0)
            {
                PLAYER_PROPS().flags                 &= ~PlayerFlag_Unk9;
                player->properties.player.field_F4 = g_Player_FlexRotationX;

                if (!WorldEnv_IsInDarkness())
                {
                    func_8005CD38(&g_Player_TargetNpcIdx, &PLAYER_PROPS().field_122, &PLAYER_COMBAT().attackPosition, Q12(2.0f / 3.0f), Q12(10.0f), playerTurn);
                }
                else
                {
                    func_8005CD38(&g_Player_TargetNpcIdx, &PLAYER_PROPS().field_122, &PLAYER_COMBAT().attackPosition, Q12(1.0f / 3.0f), Q12(3.0f), playerTurn);
                }

                if (g_Player_TargetNpcIdx == NO_VALUE)
                {
                    PLAYER_PROPS().flags                        &= ~PlayerFlag_Unk12;
                    player->model.stateStep                   = 0;
                    PLAYER_PROPS().field_122                     = Q12_ANGLE(90.0f);
                    PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_Aim;
                    g_Player_IsShooting                       = false;
                    PLAYER_EXTRA().state          = PlayerState_None;
                    g_Player_IsAttacking                      = false;
                    extra->model.controlState                 = extra->model.stateStep = 0;

                    if (PLAYER_EXTRA().lowerBodyState == PlayerLowerBodyState_Attack)
                    {
                        PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_Aim;
                        player->model.controlState =
                        player->model.stateStep    = 0;
                    }
                }
                else
                {
                    PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_AimTargetLockSwitch;
                    PLAYER_PROPS().flags         |= PlayerFlag_Unk12;
                    extra->model.controlState  =
                    extra->model.stateStep     = 0;
                }

                g_SysWork.targetNpcIdx = g_Player_TargetNpcIdx;
            }
            else
            {
                if (extra->model.controlState != 0)
                {
                    if (g_Player_TargetEnemyPosition.vx != NPC_CHARA(g_SysWork.targetNpcIdx).position.vx ||
                        g_Player_TargetEnemyPosition.vy != NPC_CHARA(g_SysWork.targetNpcIdx).position.vy ||
                        g_Player_TargetEnemyPosition.vz != NPC_CHARA(g_SysWork.targetNpcIdx).position.vz ||
                        g_Player_PrevPosition.vx != PLAYER_CHARA().position.vx ||
                        g_Player_PrevPosition.vy != PLAYER_CHARA().position.vy ||
                        g_Player_PrevPosition.vz != PLAYER_CHARA().position.vz)
                    {
                        if (!WorldEnv_IsInDarkness())
                        {
                            func_8005CD38(&enemyAttackedIdx, &PLAYER_PROPS().field_122, &PLAYER_COMBAT().attackPosition, Q12_ANGLE(50.0f), Q12(10.0f), 0);
                        }
                        else
                        {
                            func_8005CD38(&enemyAttackedIdx, &PLAYER_PROPS().field_122, &PLAYER_COMBAT().attackPosition, Q12_ANGLE(25.0f), Q12(3.0f), 0);
                        }

                        g_Player_TargetEnemyPosition = NPC_CHARA(g_SysWork.targetNpcIdx).position;
                    }
                    else
                    {
                        enemyAttackedIdx = g_SysWork.targetNpcIdx;
                    }
                }
                else
                {
                    enemyAttackedIdx      = g_SysWork.targetNpcIdx;
                    g_Player_TargetEnemyPosition = NPC_CHARA(g_SysWork.targetNpcIdx).position;
                }

                if (enemyAttackedIdx == g_SysWork.targetNpcIdx && enemyAttackedIdx != NO_VALUE)
                {
                    player->angleToTarget = Q12_FRACT(ratan2((NPC_CHARA(enemyAttackedIdx).position.vx + NPC_CHARA(enemyAttackedIdx).collision.shapeOffsets.box.vx) - PLAYER_CHARA().position.vx,
                                                       (NPC_CHARA(enemyAttackedIdx).position.vz + NPC_CHARA(enemyAttackedIdx).collision.shapeOffsets.box.vz) - PLAYER_CHARA().position.vz) +
                                                Q12_ANGLE(360.0f));
                }
                else
                {
                    PLAYER_PROPS().flags &= ~PlayerFlag_Unk12;
                    player->model.stateStep                                  = 0;
                    PLAYER_PROPS().field_122  = Q12_ANGLE(90.0f);
                    g_SysWork.targetNpcIdx                                 = NO_VALUE;
                    PLAYER_EXTRA().state                      = PlayerState_None;
                    PLAYER_EXTRA().upperBodyState             = PlayerUpperBodyState_Aim;
                    extra->model.controlState                                      = extra->model.stateStep = 0;
                }
            }

            if (extra->model.controlState == 0)
            {
                extra->model.controlState++;
            }
            break;

        case PlayerUpperBodyState_AimStart:
            player->angleToTarget = player->rotation.vy;

            if (PLAYER_COMBAT().weaponAttack == WEAPON_ATTACK(EquippedWeaponId_Chainsaw,  AttackInputType_Tap) ||
                PLAYER_COMBAT().weaponAttack == WEAPON_ATTACK(EquippedWeaponId_RockDrill, AttackInputType_Tap))
            {
                if (PLAYER_PROPS().gasWeaponPowerTimer != Q12(0.0f))
                {
                    if (extra->model.stateStep == 0)
                    {
                        extra->model.anim.status = ANIM_STATUS(HarryAnim_Unk33, false);
                        extra->model.stateStep++;
                    }
                }
                else if (extra->model.stateStep == 0)
                {
                    extra->model.anim.status = ANIM_STATUS(HarryAnim_HandgunAim, false);
                    extra->model.stateStep++;
                }

                if (((PLAYER_COMBAT().weaponAttack == WEAPON_ATTACK(EquippedWeaponId_Chainsaw, AttackInputType_Tap) &&
                      extra->model.anim.status == ANIM_STATUS(HarryAnim_HandgunAim, true) &&
                      extra->model.anim.keyframeIdx >= (D_800C44F0[0].startKeyframeIdx + 5)) ||
                     (PLAYER_COMBAT().weaponAttack == WEAPON_ATTACK(EquippedWeaponId_RockDrill, AttackInputType_Tap) &&
                      extra->model.anim.status == ANIM_STATUS(HarryAnim_HandgunAim, true) &&
                      extra->model.anim.keyframeIdx >= (D_800C44F0[0].startKeyframeIdx + 9))) &&
                    !(PLAYER_PROPS().flags & PlayerFlag_Unk2))
                {
                    PLAYER_PROPS().gasWeaponPowerTimer = Q12(60.0f);

                    func_8004C564(PLAYER_COMBAT().weaponAttack, 0);

                    player->properties.player.field_10C                       = 0x40;
                    PLAYER_PROPS().flags |= PlayerFlag_Unk2;
                }
            }
            else if (PLAYER_COMBAT().weaponAttack == WEAPON_ATTACK(EquippedWeaponId_HuntingRifle, AttackInputType_Tap))
            {
                if (extra->model.stateStep == 0)
                {
                    extra->model.anim.status = ANIM_STATUS(HarryAnim_Unk29, false);
                    extra->model.stateStep++;
                }
            }
            else
            {
                if (extra->model.stateStep == 0)
                {
                    extra->model.anim.status = ANIM_STATUS(HarryAnim_HandgunAim, false);
                    extra->model.stateStep++;
                }
            }

            if (PLAYER_COMBAT().weaponAttack == WEAPON_ATTACK(EquippedWeaponId_Chainsaw,  AttackInputType_Tap) ||
                PLAYER_COMBAT().weaponAttack == WEAPON_ATTACK(EquippedWeaponId_RockDrill, AttackInputType_Tap))
            {
                if (extra->model.anim.keyframeIdx == D_800C44F0[0].endKeyframeIdx ||
                    extra->model.anim.keyframeIdx == D_800C44F0[5].endKeyframeIdx)
                {
                    if (extra->model.anim.keyframeIdx == D_800C44F0[0].endKeyframeIdx)
                    {
                        func_8004C564(PLAYER_COMBAT().weaponAttack, 1);
                    }

                    PLAYER_EXTRA().upperBodyState             = PlayerUpperBodyState_Aim;
                    extra->model.controlState                                      = extra->model.stateStep = 0;
                    PLAYER_PROPS().flags &= ~PlayerFlag_Unk2;

                    if (PLAYER_PROPS().gasWeaponPowerTimer != Q12(0.0f))
                    {
                        player->field_44.field_0 = 1;
                    }
                }
            }
            else
            {
                if (extra->model.anim.keyframeIdx == D_800C44F0[0].endKeyframeIdx ||
                    extra->model.anim.keyframeIdx == D_800C44F0[1].endKeyframeIdx)
                {
                    PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_Aim;
                }
            }
            break;

        case PlayerUpperBodyState_AimStartTargetLock:
        case PlayerUpperBodyState_AimTargetLockSwitch:
            sp22 = 0;

            if (PLAYER_EXTRA().upperBodyState == PlayerUpperBodyState_AimStartTargetLock)
            {
                if (g_GameWork.config.extraAutoAiming)
                {
                    if (!WorldEnv_IsInDarkness())
                    {
                        func_8005CD38(&g_Player_TargetNpcIdx, &PLAYER_PROPS().field_122, &PLAYER_COMBAT().attackPosition, Q12_ANGLE(50.0f), Q12(10.0f), 0);
                    }
                    else
                    {
                        func_8005CD38(&g_Player_TargetNpcIdx, &PLAYER_PROPS().field_122, &PLAYER_COMBAT().attackPosition, Q12_ANGLE(50.0f), Q12(3.0f), 0);
                    }
                }
                else if (!WorldEnv_IsInDarkness())
                {
                    func_8005CD38(&g_Player_TargetNpcIdx, &PLAYER_PROPS().field_122, &PLAYER_COMBAT().attackPosition, Q12(3.0f), Q12(7.0f), 4);
                }
                else
                {
                    func_8005CD38(&g_Player_TargetNpcIdx, &PLAYER_PROPS().field_122, &PLAYER_COMBAT().attackPosition, Q12(0.9f), Q12(2.1f), 4);
                }

                g_SysWork.targetNpcIdx = g_Player_TargetNpcIdx;
            }

            D_800AF220 = 1;
            player->properties.player.field_100++;

            if (!g_GameWork.config.extraWeaponCtrl)
            {
                g_Player_HasActionInput      = false;
                g_Player_HasMoveInput        = false;
                g_Player_IsShooting          = false;
                g_Player_IsAttacking         = false;
                g_Player_IsHoldAttack        = false;
                g_Player_IsAiming            = false;
                g_Player_IsRunning           = false;
                g_Player_IsMovingBackward    = false;
                g_Player_IsMovingForward     = false;
                g_Player_IsSteppingRightTap  = false;
                g_Player_IsSteppingRightHold = false;
                g_Player_IsTurningRight      = false;
                g_Player_IsSteppingLeftTap   = false;
                g_Player_IsSteppingLeftHold  = false;
                g_Player_IsTurningLeft       = false;
            }

            extra->model.controlState++;

            if (PLAYER_EXTRA().upperBodyState == PlayerUpperBodyState_AimStartTargetLock)
            {
                if (extra->model.stateStep == 0)
                {
                    extra->model.anim.status = ANIM_STATUS(HarryAnim_Unk32, false);
                    extra->model.stateStep++;
                }
            }
            else if (extra->model.stateStep == 0)
            {
                extra->model.anim.status = ANIM_STATUS(HarryAnim_Unk29, false);
                extra->model.stateStep++;
            }

            if (g_SysWork.targetNpcIdx == NO_VALUE)
            {
                PLAYER_PROPS().field_122  = Q12_ANGLE(90.0f);
                PLAYER_EXTRA().upperBodyState             = PlayerUpperBodyState_Aim;
                PLAYER_EXTRA().state                      = PlayerState_None;
                PLAYER_PROPS().flags &= ~PlayerFlag_Unk12;
                extra->model.controlState                                      = extra->model.stateStep = 0;
                break;
            }

            if (!g_GameWork.config.extraAutoAiming)
            {
                temp_v0_3 = ratan2((NPC_CHARA(g_SysWork.targetNpcIdx).position.vx + NPC_CHARA(g_SysWork.targetNpcIdx).collision.shapeOffsets.box.vx) - PLAYER_CHARA().position.vx,
                                   (NPC_CHARA(g_SysWork.targetNpcIdx).position.vz + NPC_CHARA(g_SysWork.targetNpcIdx).collision.shapeOffsets.box.vz) - PLAYER_CHARA().position.vz);

                temp_s1_2 = Q12_ANGLE_NORM_U(temp_v0_3 + Q12_ANGLE(360.0f));

                switch (extra->model.anim.status)
                {
                    case ANIM_STATUS(HarryAnim_Unk29, true):
                    case ANIM_STATUS(HarryAnim_Unk32, true):
                        if (extra->model.anim.keyframeIdx == D_800C44F0[D_800AF220].endKeyframeIdx)
                        {
                            player->rotation.vy = temp_s1_2;
                        }
                        break;
                }

                Math_ShortestAngleGet(player->rotation.vy, temp_s1_2, &sp20);

                D_800C454C = ((extra->model.controlState * 3) + 12) * g_DeltaTime;
                D_800C454C = CLAMP(D_800C454C, 0, 0xFFF);

                if (ABS(sp20) >= Q12_ANGLE(11.25f))
                {
                    if (sp20 < Q12_ANGLE(0.0f))
                    {
                        D_800C454C = -D_800C454C;
                    }
                }
                else
                {
                    player->rotation.vy   = temp_s1_2;
                    D_800C454C            = Q12(0.0f);
                    player->angleToTarget = temp_s1_2;

                    if (PLAYER_EXTRA().upperBodyState == PlayerUpperBodyState_AimStartTargetLock)
                    {
                        PLAYER_PROPS().flags &= ~PlayerFlag_Unk8;
                    }
                    else
                    {
                        PLAYER_PROPS().flags |= PlayerFlag_Unk8;
                    }

                    if (PLAYER_PROPS().flags & PlayerFlag_Unk9)
                    {
                        if (extra->model.anim.keyframeIdx == D_800C44F0[4].endKeyframeIdx)
                        {
                            PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_Attack;

                            if (PLAYER_EXTRA().lowerBodyState == PlayerLowerBodyState_Aim)
                            {
                                PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_Attack;
                                player->model.controlState                          =
                                player->model.stateStep                      = 0;
                            }

                            PLAYER_PROPS().flags &= ~PlayerFlag_Unk9;
                            extra->model.controlState                                      =
                            extra->model.stateStep                                  = 0;
                        }
                    }
                    else if (extra->model.anim.keyframeIdx == D_800C44F0[4].endKeyframeIdx)
                    {
                        PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_AimTargetLock;
                        extra->model.controlState                          =
                        extra->model.stateStep                      = 0;
                    }
                }

                player->angleToTarget = player->rotation.vy;
                break;
            }

            temp_v0_3 = ratan2((NPC_CHARA(g_SysWork.targetNpcIdx).position.vx +
                                NPC_CHARA(g_SysWork.targetNpcIdx).collision.shapeOffsets.box.vx) -
                               PLAYER_CHARA().position.vx,
                                (NPC_CHARA(g_SysWork.targetNpcIdx).position.vz +
                                 NPC_CHARA(g_SysWork.targetNpcIdx).collision.shapeOffsets.box.vz) -
                                PLAYER_CHARA().position.vz);

            temp_s1_2 = Q12_ANGLE_NORM_U(temp_v0_3 + Q12_ANGLE(360.0f));

            Math_ShortestAngleGet(player->rotation.vy, temp_s1_2, &sp20);

            sp20 = CLAMP(sp20, Q12_ANGLE(-33.75f), Q12_ANGLE(33.75f));

            temp_v1_3 = g_DeltaTime * 0xF;
            temp_v1_3 = CLAMP(temp_v1_3, 0, 0xFFF);
            var_s0    = temp_v1_3;

            Math_ShortestAngleGet(player->angleToTarget, temp_s1_2, &sp22);

            if (ABS(sp22) > Q12_ANGLE(11.25f))
            {
                if (sp22 < Q12_ANGLE(0.0f))
                {
                    var_s0 = -var_s0;
                }

                player->angleToTarget = Q12_ANGLE_NORM_U((player->angleToTarget + (var_s0 >> 4)) + Q12_ANGLE(360.0f));
            }
            else
            {
                player->angleToTarget = player->rotation.vy + sp20;

                if (PLAYER_EXTRA().upperBodyState == PlayerUpperBodyState_AimStartTargetLock)
                {
                    PLAYER_PROPS().flags &= ~PlayerFlag_Unk8;
                }
                else
                {
                    PLAYER_PROPS().flags |= PlayerFlag_Unk8;
                }

                if (PLAYER_PROPS().flags & PlayerFlag_Unk9)
                {
                    if (extra->model.anim.keyframeIdx == D_800C44F0[4].endKeyframeIdx)
                    {
                        PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_Attack;

                        if (PLAYER_EXTRA().lowerBodyState == PlayerLowerBodyState_Aim)
                        {
                            PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_Attack;
                            player->model.controlState                = player->model.stateStep = 0;
                        }

                        PLAYER_PROPS().flags        &= ~PlayerFlag_Unk9;
                        extra->model.controlState = extra->model.stateStep = 0;
                    }
                }
                else if (extra->model.anim.keyframeIdx == D_800C44F0[4].endKeyframeIdx)
                {
                    PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_AimTargetLock;
                    extra->model.controlState = extra->model.stateStep = 0;
                }
            }
            break;

        case PlayerUpperBodyState_AimStop:
            D_800AF220 = g_Player_EquippedWeaponInfo.field_A & 0xF;

            if (extra->model.stateStep == 0)
            {
                extra->model.anim.status = g_Player_EquippedWeaponInfo.animStopAiming;
                extra->model.stateStep++;
            }

            if (extra->model.anim.keyframeIdx == D_800C44F0[D_800AF220].startKeyframeIdx ||
                ((PLAYER_EXTRA().lowerBodyState == PlayerLowerBodyState_RunForward || PLAYER_EXTRA().lowerBodyState == PlayerLowerBodyState_RunRight ||
                  PLAYER_EXTRA().lowerBodyState == PlayerLowerBodyState_RunLeft) &&
                 (extra->model.anim.keyframeIdx <= D_800C44F0[D_800AF220].endKeyframeIdx)))
            {
                switch (PLAYER_EXTRA().lowerBodyState)
                {
                    case PlayerLowerBodyState_RunForward:
                        PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_RunForward;
                        break;

                    case PlayerLowerBodyState_RunRight:
                        PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_RunRight;
                        break;

                    case PlayerLowerBodyState_AimRunLeft:
                        PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_RunLeft;
                        break;

                    default:
                        PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_None;
                        break;
                }

                extra->model.controlState = extra->model.stateStep = 0;
                if (PLAYER_EXTRA().lowerBodyState == PlayerLowerBodyState_None)
                {
                    player->model.controlState = player->model.stateStep = 0;
                }

                g_SysWork.targetNpcIdx = NO_VALUE;
            }

            player->angleToTarget = player->rotation.vy;
            break;

        case PlayerUpperBodyState_Attack:
            if (Player_CombatAnimUpdate())
            {
                return true;
            }
            break;

        case PlayerUpperBodyState_Reload:
            if (extra->model.stateStep == 0)
            {
                extra->model.anim.status = ANIM_STATUS(HarryAnim_HandgunRecoil, false);
                extra->model.stateStep++;
            }

            if ((D_800AF624 + g_Player_EquippedWeaponInfo.field_9) <= extra->model.anim.keyframeIdx &&
                !(PLAYER_PROPS().flags & PlayerFlag_Unk2))
            {
                Sfx_WithFlagsPlay(g_Player_EquippedWeaponInfo.reloadSfx, &player->position, Q8(0.5f), SfxFlag_None);

                player->properties.player.field_10C = 0x20;
                PLAYER_PROPS().flags                  |= PlayerFlag_Unk2;
            }

            if (extra->model.anim.keyframeIdx == D_800AF626)
            {
                g_Player_TargetNpcIdx                     = NO_VALUE;
                PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_Aim;
                g_SysWork.targetNpcIdx                    = NO_VALUE;
                PLAYER_EXTRA().state          = PlayerState_None;
                PLAYER_PROPS().flags                        &= ~PlayerFlag_Unk2;
                extra->model.anim.status                  = ANIM_STATUS(HarryAnim_HandgunAim, true);
                extra->model.anim.keyframeIdx             = 588;
                extra->model.anim.time                    = Q12(588.0f);

                if (PLAYER_EXTRA().lowerBodyState == PlayerLowerBodyState_Reload)
                {
                    PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_Aim;
                }

                if (PLAYER_COMBAT().totalWeaponAmmo != 0)
                {
                    currentAmmoVar = PLAYER_COMBAT().currentWeaponAmmo;
                    totalAmmoVar   = PLAYER_COMBAT().totalWeaponAmmo;

                    Items_AmmoReloadCompute(&currentAmmoVar, &totalAmmoVar, PLAYER_COMBAT().weaponAttack);

                    PLAYER_COMBAT().currentWeaponAmmo = currentAmmoVar;
                    PLAYER_COMBAT().totalWeaponAmmo   = totalAmmoVar;

                    for (i = 0; i < INV_ITEM_COUNT_MAX; i++)
                    {
                        if (g_SavegamePtr->items[i].id == (PLAYER_COMBAT().weaponAttack + InvItemId_KitchenKnife))
                        {
                            g_SavegamePtr->items[i].count = PLAYER_COMBAT().currentWeaponAmmo;
                        }
                        if (g_SavegamePtr->items[i].id == (PLAYER_COMBAT().weaponAttack + InvItemId_Handgun))
                        {
                            g_SavegamePtr->items[i].count = PLAYER_COMBAT().totalWeaponAmmo;
                        }
                    }
                }
            }
            break;
    }

    return false;
}

void Player_CombatStateUpdate(s_SubCharacter* player, s_PlayerExtra* extra) // 0x800771BC
{
    s32 currentAmmoVar;
    s32 totalAmmoVar;
    s32 i;

    // Lock player view onto enemy.
    switch (PLAYER_EXTRA().upperBodyState)
    {
        case PlayerUpperBodyState_None:
        case PlayerUpperBodyState_WalkForward:
        case PlayerUpperBodyState_SidestepRight:
        case PlayerUpperBodyState_SidestepLeft:
        case PlayerUpperBodyState_WalkBackward:
        case PlayerUpperBodyState_TurnRight:
        case PlayerUpperBodyState_TurnLeft:
            if (!g_Player_IsInWalkToRunTransition)
            {
                if ((g_Player_IsAiming && PLAYER_COMBAT().weaponAttack >= WEAPON_ATTACK(EquippedWeaponId_KitchenKnife, AttackInputType_Tap)) ||
                    PLAYER_COMBAT().isAiming)
                {
                    PLAYER_COMBAT().isAiming = true;

                    if (PLAYER_COMBAT().weaponAttack < WEAPON_ATTACK(EquippedWeaponId_Handgun, AttackInputType_Tap))
                    {
                        g_Player_TargetNpcIdx = NO_VALUE;
                    }
                    else
                    {
                        if (g_GameWork.config.extraAutoAiming)
                        {
                            if (!WorldEnv_IsInDarkness())
                            {
                                func_8005CD38(&g_Player_TargetNpcIdx, &PLAYER_PROPS().field_122, &PLAYER_COMBAT().attackPosition, Q12_ANGLE(50.0f), Q12(10.0f), 0);
                            }
                            else
                            {
                                func_8005CD38(&g_Player_TargetNpcIdx, &PLAYER_PROPS().field_122, &PLAYER_COMBAT().attackPosition, Q12_ANGLE(50.0f), Q12(3.0f), 0);
                            }
                        }
                        else if (!WorldEnv_IsInDarkness())
                        {
                            func_8005CD38(&g_Player_TargetNpcIdx, &PLAYER_PROPS().field_122, &PLAYER_COMBAT().attackPosition, Q12(3.0f), Q12(7.0f), 4);
                        }
                        else
                        {
                            func_8005CD38(&g_Player_TargetNpcIdx, &PLAYER_PROPS().field_122, &PLAYER_COMBAT().attackPosition, Q12(0.9f), Q12(2.1f), 4);
                        }
                    }

                    g_SysWork.targetNpcIdx = g_Player_TargetNpcIdx;
                    if (g_SysWork.targetNpcIdx == NO_VALUE)
                    {
                        PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_AimStart;
                        PLAYER_PROPS().field_122                     = Q12_ANGLE(90.0f);
                    }
                    else
                    {
                        PLAYER_EXTRA().state          = PlayerState_Combat;
                        PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_AimStartTargetLock;
                    }

                    if (PLAYER_EXTRA().lowerBodyState == PlayerLowerBodyState_None)
                    {
                        PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_Aim;
                        player->model.stateStep    = 0;
                        player->model.controlState = 0;
                    }
                    else if (PLAYER_EXTRA().lowerBodyState < PlayerLowerBodyState_Aim)
                    {
                        PLAYER_EXTRA().lowerBodyState += PlayerLowerBodyState_Aim;
                    }

                    extra->model.stateStep              = 0;
                    extra->model.controlState           = 0;
                    player->properties.player.field_100 = 0;

                    if (PLAYER_COMBAT().totalWeaponAmmo != 0)
                    {
                        if (PLAYER_COMBAT().weaponAttack >= WEAPON_ATTACK(EquippedWeaponId_Handgun, AttackInputType_Tap))
                        {
                            currentAmmoVar = PLAYER_COMBAT().currentWeaponAmmo;
                            totalAmmoVar   = PLAYER_COMBAT().totalWeaponAmmo;

                            Items_AmmoReloadCompute(&currentAmmoVar, &totalAmmoVar, PLAYER_COMBAT().weaponAttack);

                            PLAYER_COMBAT().currentWeaponAmmo = currentAmmoVar;
                            PLAYER_COMBAT().totalWeaponAmmo   = totalAmmoVar;
                        }
                    }

                    if (PLAYER_COMBAT().weaponAttack >= WEAPON_ATTACK(EquippedWeaponId_Handgun, AttackInputType_Tap))
                    {
                        for (i = 0; i < INV_ITEM_COUNT_MAX; i++)
                        {
                            if (g_SavegamePtr->items[i].id == (PLAYER_COMBAT().weaponAttack + InvItemId_KitchenKnife))
                            {
                                g_SavegamePtr->items[i].count = PLAYER_COMBAT().currentWeaponAmmo;
                            }
                            if (g_SavegamePtr->items[i].id == (PLAYER_COMBAT().weaponAttack + InvItemId_Handgun))
                            {
                                g_SavegamePtr->items[i].count = PLAYER_COMBAT().totalWeaponAmmo;
                            }
                        }
                    }
                }
            }
            break;
    }

    // Execute finishing move on knocked enemies.
    switch (PLAYER_EXTRA().upperBodyState)
    {
        case PlayerUpperBodyState_None:
        case PlayerUpperBodyState_WalkForward:
        case PlayerUpperBodyState_SidestepRight:
        case PlayerUpperBodyState_SidestepLeft:
        case PlayerUpperBodyState_WalkBackward:
        case PlayerUpperBodyState_TurnRight:
        case PlayerUpperBodyState_TurnLeft:
        case PlayerUpperBodyState_Aim:
            if (func_8007F95C())
            {
                if (g_Player_IsAttacking)
                {
                    Player_ExtraStateSet(player, extra, PlayerState_StompEnemy);
                    return;
                }

                if (g_Player_IsShooting)
                {
                    Player_ExtraStateSet(player, extra, PlayerState_KickEnemy);
                    return;
                }
            }
            break;
    }

    // Handle aim state.
    switch (PLAYER_EXTRA().upperBodyState)
    {
        case PlayerUpperBodyState_Aim:
        case PlayerUpperBodyState_AimTargetLock:
            // Stop aiming.
            if (( g_GameWork.config.extraWeaponCtrl && !g_Player_IsAiming) ||
                (!g_GameWork.config.extraWeaponCtrl &&  g_Player_IsAiming))
            {
                player->properties.player.field_F4 = 0;
                PLAYER_EXTRA().upperBodyState         = PlayerUpperBodyState_AimStop;
                g_SysWork.targetNpcIdx             = NO_VALUE;
                PLAYER_PROPS().flags                 &= ~PlayerFlag_Unk0;
                PLAYER_EXTRA().state                  = PlayerState_None;
                PLAYER_COMBAT().isAiming              = false;
                PLAYER_PROPS().flags                 &= ~PlayerFlag_Unk9;

                if (PLAYER_EXTRA().lowerBodyState == PlayerLowerBodyState_Aim ||
                    PLAYER_EXTRA().lowerBodyState == PlayerLowerBodyState_Attack)
                {
                    PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_None;
                    player->model.stateStep                   = 0;
                    player->model.controlState                = 0;
                    extra->model.stateStep                    = 0;
                    extra->model.controlState                 = 0;
                    break;
                }

                if (PLAYER_EXTRA().lowerBodyState >= PlayerLowerBodyState_AimWalkForward)
                {
                    PLAYER_EXTRA().lowerBodyState -= PlayerLowerBodyState_Aim;
                }

                extra->model.stateStep    = 0;
                extra->model.controlState = 0;
                break;
            }

            if ((g_Player_IsAttacking || g_Player_IsShooting) &&
                PLAYER_EXTRA().lowerBodyState != PlayerLowerBodyState_AimQuickTurnRight &&
                PLAYER_EXTRA().lowerBodyState != PlayerLowerBodyState_AimQuickTurnLeft)
            {
                if (PLAYER_COMBAT().weaponAttack < WEAPON_ATTACK(EquippedWeaponId_Handgun, AttackInputType_Tap))
                {
                    if (PLAYER_COMBAT().weaponAttack == WEAPON_ATTACK(EquippedWeaponId_Chainsaw,  AttackInputType_Tap) ||
                        PLAYER_COMBAT().weaponAttack == WEAPON_ATTACK(EquippedWeaponId_RockDrill, AttackInputType_Tap))
                    {
                        if ((extra->model.anim.status != ANIM_STATUS(HarryAnim_HandgunAim, true) ||
                             extra->model.anim.keyframeIdx != D_800C44F0[0].endKeyframeIdx) &&
                            (extra->model.anim.status != ANIM_STATUS(HarryAnim_Unk29, true) ||
                             extra->model.anim.keyframeIdx != D_800C44F0[1].endKeyframeIdx) &&
                            (extra->model.anim.status != ANIM_STATUS(HarryAnim_Unk34, true) ||
                             extra->model.anim.keyframeIdx < D_800C44F0[6].startKeyframeIdx ||
                             D_800C44F0[6].endKeyframeIdx < extra->model.anim.keyframeIdx) &&
                            (extra->model.anim.status != ANIM_STATUS(HarryAnim_Unk30, true) ||
                             extra->model.anim.keyframeIdx != D_800C44F0[2].endKeyframeIdx) &&
                            (extra->model.anim.status != ANIM_STATUS(HarryAnim_HandgunRecoil, true) ||
                             extra->model.anim.keyframeIdx != D_800C44F0[3].endKeyframeIdx) &&
                            (extra->model.anim.status != ANIM_STATUS(HarryAnim_Unk33, true) ||
                             extra->model.anim.keyframeIdx != D_800C44F0[5].endKeyframeIdx))
                        {
                            break;
                        }
                    }
                    else
                    {
                        if ((extra->model.anim.status != ANIM_STATUS(HarryAnim_HandgunAim, true) ||
                             extra->model.anim.keyframeIdx != D_800C44F0[0].endKeyframeIdx) &&
                            (extra->model.anim.status != ANIM_STATUS(HarryAnim_Unk29, true) ||
                             extra->model.anim.keyframeIdx != D_800C44F0[1].endKeyframeIdx) &&
                            (extra->model.anim.status != ANIM_STATUS(HarryAnim_Unk30, true) ||
                             extra->model.anim.keyframeIdx != D_800C44F0[2].endKeyframeIdx) &&
                            (extra->model.anim.status != ANIM_STATUS(HarryAnim_HandgunRecoil, true) ||
                             extra->model.anim.keyframeIdx != D_800C44F0[3].endKeyframeIdx))
                        {
                            break;
                        }
                    }
                }
                else
                {
                    if ((extra->model.anim.status != ANIM_STATUS(HarryAnim_HandgunAim, true) ||
                         extra->model.anim.keyframeIdx != D_800C44F0[0].endKeyframeIdx) &&
                        (extra->model.anim.status != ANIM_STATUS(HarryAnim_Unk29, true) ||
                         extra->model.anim.keyframeIdx != D_800C44F0[1].endKeyframeIdx) &&
                        (extra->model.anim.status != ANIM_STATUS(HarryAnim_Unk30, true) ||
                         extra->model.anim.keyframeIdx != D_800C44F0[2].endKeyframeIdx) &&
                        (extra->model.anim.status != ANIM_STATUS(HarryAnim_HandgunRecoil, true) ||
                         extra->model.anim.keyframeIdx != D_800C44F0[3].endKeyframeIdx) &&
                        (extra->model.anim.status != ANIM_STATUS(HarryAnim_Unk32, true) ||
                         extra->model.anim.keyframeIdx != D_800C44F0[4].endKeyframeIdx) &&
                        (extra->model.anim.status != ANIM_STATUS(HarryAnim_Unk36, true) ||
                         extra->model.anim.keyframeIdx != D_800C44F0[8].endKeyframeIdx) &&
                        (extra->model.anim.status != ANIM_STATUS(HarryAnim_Unk34, true) ||
                         extra->model.anim.keyframeIdx != D_800C44F0[6].startKeyframeIdx))
                    {
                        break;
                    }
                }

                PLAYER_PROPS().flags &= ~PlayerFlag_Unk0;

                if (PLAYER_COMBAT().weaponAttack < WEAPON_ATTACK(EquippedWeaponId_Handgun, AttackInputType_Tap))
                {
                    if (!WorldEnv_IsInDarkness())
                    {
                        func_8005CD38(&g_Player_TargetNpcIdx, &PLAYER_PROPS().field_122, &PLAYER_COMBAT().attackPosition, Q12(3.0f), Q12(3.0f), 5);
                    }
                    else
                    {
                        func_8005CD38(&g_Player_TargetNpcIdx, &PLAYER_PROPS().field_122, &PLAYER_COMBAT().attackPosition, Q12(1.0f), Q12(1.0f), 5);
                    }

                    g_SysWork.targetNpcIdx = g_Player_TargetNpcIdx;
                }
                else
                {
                    if (!WorldEnv_IsInDarkness())
                    {
                        func_8005CD38(&g_Player_TargetNpcIdx, &PLAYER_PROPS().field_122, &PLAYER_COMBAT().attackPosition, Q12(7.0f), Q12(7.0f), 5);
                    }
                    else
                    {
                        func_8005CD38(&g_Player_TargetNpcIdx, &PLAYER_PROPS().field_122, &PLAYER_COMBAT().attackPosition, Q12(2.1f), Q12(2.1f), 5);
                    }
                }

                switch (g_Player_TargetNpcIdx)
                {
                    default:
                        if (PLAYER_COMBAT().weaponAttack >= WEAPON_ATTACK(EquippedWeaponId_Handgun, AttackInputType_Tap))
                        {
                            PLAYER_EXTRA().state = PlayerState_Combat;
                            PLAYER_PROPS().flags               |= PlayerFlag_Unk0 | PlayerFlag_Unk9;

                            if (g_SysWork.targetNpcIdx != g_Player_TargetNpcIdx)
                            {
                                PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_AimTargetLockSwitch;
                            }
                            else
                            {
                                PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_Attack;

                                if (PLAYER_EXTRA().lowerBodyState == PlayerLowerBodyState_Aim)
                                {
                                    PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_Attack;
                                    player->model.stateStep                   = 0;
                                    player->model.controlState                = 0;
                                }
                            }

                            g_SysWork.targetNpcIdx = g_Player_TargetNpcIdx;
                            break;
                        }

                        PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_Attack;

                        if (PLAYER_EXTRA().lowerBodyState == PlayerLowerBodyState_Aim)
                        {
                            PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_Attack;
                            player->model.stateStep                   = 0;
                            player->model.controlState                = 0;
                        }

                    case NO_VALUE:
                        PLAYER_PROPS().field_122  = Q12_ANGLE(90.0f);
                        PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_Attack;
                        PLAYER_PROPS().flags                        &= ~PlayerFlag_Unk9;

                        if (PLAYER_EXTRA().lowerBodyState == PlayerLowerBodyState_Aim)
                        {
                            PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_Attack;
                            player->model.stateStep                   = 0;
                            player->model.controlState                = 0;
                        }
                        break;
                }

                if (PLAYER_COMBAT().weaponAttack >= WEAPON_ATTACK(EquippedWeaponId_Handgun, AttackInputType_Tap))
                {
                    if (PLAYER_COMBAT().currentWeaponAmmo == 0 &&
                        INV_ITEM_GROUP(g_SavegamePtr->equippedWeapon) == InvItemGroup_GunWeapons &&
                        PLAYER_COMBAT().totalWeaponAmmo != 0)
                    {
                        PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_Reload;
                        PLAYER_PROPS().flags                        &= ~PlayerFlag_Unk9;

                        if (PLAYER_EXTRA().lowerBodyState == PlayerLowerBodyState_Aim ||
                            PLAYER_EXTRA().lowerBodyState == PlayerLowerBodyState_Attack)
                        {
                            PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_Reload;
                            player->model.stateStep                   = 0;
                            player->model.controlState                = 0;
                        }
                    }
                }
                else
                {
                    PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_Attack;

                    if (PLAYER_EXTRA().lowerBodyState == PlayerLowerBodyState_Aim ||
                        WEAPON_ATTACK_ID_GET(PLAYER_COMBAT().weaponAttack) == EquippedWeaponId_SteelPipe ||
                        WEAPON_ATTACK_ID_GET(PLAYER_COMBAT().weaponAttack) == EquippedWeaponId_Hammer    ||
                        WEAPON_ATTACK_ID_GET(PLAYER_COMBAT().weaponAttack) == EquippedWeaponId_RockDrill ||
                        WEAPON_ATTACK_ID_GET(PLAYER_COMBAT().weaponAttack) == EquippedWeaponId_Katana)
                    {
                        PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_Attack;
                        player->model.stateStep                   = 0;
                        player->model.controlState                = 0;
                    }
                }

                extra->model.stateStep    = 0;
                extra->model.controlState = 0;
            }
            else
            {
                PLAYER_PROPS().flags &= ~PlayerFlag_Unk9;
            }
            break;
    }
}

void Player_StepWallStop_MovementCancel(s_SubCharacter* player, s32 animStatus0, s32 animStatus1, s32 keyframeIdx, e_PlayerLowerBodyState lowerBodyState, s32 headingAngle, s32 aimState) // 0x80077BB8
{
    q3_12 headingAngleCpy;

    if (PLAYER_PROPS().moveSpeed != Q12(0.0f))
    {
        PLAYER_PROPS().moveSpeed -= (TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f))) >> 1;
        if ((PLAYER_PROPS().moveSpeed >> 16) & (1 << 0))
        {
            PLAYER_PROPS().moveSpeed = Q12(0.0f);
        }
    }

    if (player->model.stateStep == 0)
    {
        player->model.anim.status = animStatus0;
        player->model.stateStep++;
    }

    if (PLAYER_EXTRA().upperBodyState != PlayerUpperBodyState_AimStartTargetLock)
    {
        if (player->model.anim.status == animStatus1 && player->model.anim.keyframeIdx >= keyframeIdx)
        {
            PLAYER_EXTRA().lowerBodyState = aimState;
            Player_MovementStateReset(player, lowerBodyState);
        }

        Player_CharaRotate(2);
    }

    if (PLAYER_EXTRA().lowerBodyState == PlayerLowerBodyState_None)
    {
        PLAYER_PROPS().headingAngle = Q12_ANGLE(0.0f);
        g_Player_HeadingAngle    = Q12_ANGLE(0.0f);
    }
    else
    {
        headingAngleCpy          = headingAngle;
        PLAYER_PROPS().headingAngle = headingAngleCpy;
        g_Player_HeadingAngle    = headingAngleCpy;
    }
}

void Player_LowerBodyUpdate(s_SubCharacter* player, s_PlayerExtra* extra) // 0x80077D00
{
    #define MOVE_DIST_MAX            Q12(1000000.0f)
    #define MOVE_DIST_MIN            1
    #define RUN_STUMBLE_TRIGGER_DIST (u32)Q12(10.0f)

    // Used for `player.moveSpeed`.
    #define GET_MOVE_SPEED(zoneType)                      \
        (((zoneType) == SpeedZoneType_Fast) ? Q12(5.0f) : \
                                              (((zoneType) == SpeedZoneType_Slow) ? Q12(3.5f) : Q12(4.0f)))

    // Used for `player.moveSpeed`.
    #define GET_VAL(val) \
        (((val) < Q12(3.5f)) ? (((g_DeltaTime) * Q12(0.75f)) / TIMESTEP_30_FPS) : (((g_DeltaTime) + (((g_DeltaTime) < 0) ? 3 : 0)) >> 2))

    q19_12                 moveOffsetX;
    q19_12                 moveOffsetZ;
    q19_12                 moveDist;
    s32                    speedZoneType; // `e_SpeedZoneType`
    e_PlayerLowerBodyState lowerBodyMoveState;
    s32                    moveDistForward;
    s32                    aimState;

    #define stillAnim HARRY_BASE_ANIM_INFOS[ANIM_STATUS(HarryAnim_Still, false)]

    if (PLAYER_EXTRA().lowerBodyState < PlayerLowerBodyState_Aim)
    {
        aimState = 0;
    }
    else
    {
        aimState = 20;
    }

    // Compute move distance step.
    lowerBodyMoveState = Player_LowerBodyMoveStateGet(player, &D_800C45C8);
    speedZoneType      = Map_SpeedZoneTypeGet(player->position.vx, player->position.vz);
    moveOffsetX        = SQUARE(player->position.vx - g_Player_PrevPosition.vx);
    moveOffsetZ        = SQUARE(player->position.vz - g_Player_PrevPosition.vz);
    moveDist           = SquareRoot0(moveOffsetX + moveOffsetZ);

    // Track travel distances.
    switch (PLAYER_EXTRA().lowerBodyState)
    {
        case PlayerLowerBodyState_None:
        case PlayerLowerBodyState_Aim:
        case PlayerLowerBodyState_Attack:
            break;

        case PlayerLowerBodyState_WalkForward:
        case PlayerLowerBodyState_WalkBackward:
        case PlayerLowerBodyState_SidestepRight:
        case PlayerLowerBodyState_SidestepLeft:
        case PlayerLowerBodyState_AimWalkForward:
        case PlayerLowerBodyState_AimWalkBackward:
        case PlayerLowerBodyState_AimSidestepRight:
        case PlayerLowerBodyState_AimSidestepLeft:
            g_SavegamePtr->walkDistance += moveDist;
            g_SavegamePtr->walkDistance  = CLAMP(g_SavegamePtr->walkDistance, MOVE_DIST_MIN, MOVE_DIST_MAX);
            break;

        default:
            g_SavegamePtr->runDistance += moveDist;
            g_SavegamePtr->runDistance  = CLAMP(g_SavegamePtr->runDistance, MOVE_DIST_MIN, MOVE_DIST_MAX);
            break;
    }

    switch (PLAYER_EXTRA().lowerBodyState)
    {
        case PlayerLowerBodyState_None:
        case PlayerLowerBodyState_Aim:
            if (player->model.anim.status == ANIM_STATUS(HarryAnim_WalkForward, true))
            {
                player->model.stateStep = 0;
            }

            // Check if player is aiming.
            if (aimState != 0)
            {
                if (PLAYER_PROPS().moveSpeed != Q12(0.0f))
                {
                    PLAYER_PROPS().moveSpeed -= TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f));
                    if ((PLAYER_PROPS().moveSpeed >> 16) & 1)
                    {
                        PLAYER_PROPS().moveSpeed = Q12(0.0f);
                    }
                }
            }
            else if (PLAYER_PROPS().moveSpeed != Q12(0.0f))
            {
                PLAYER_PROPS().moveSpeed -= (TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f))) * 2;
                if ((PLAYER_PROPS().moveSpeed >> 16) & 1)
                {
                    PLAYER_PROPS().moveSpeed = Q12(0.0f);
                }
            }

            // Sets animations during specific idle states while aiming or standing.
            if (PLAYER_EXTRA().lowerBodyState == PlayerLowerBodyState_None)
            {
                // Aim to idle.
                if (PLAYER_EXTRA().upperBodyState == PlayerUpperBodyState_AimStop)
                {
                    if (!PLAYER_COMBAT().isAiming && player->model.stateStep == 0)
                    {
                        player->model.anim.status = g_Player_EquippedWeaponInfo.animStopAiming;
                        player->model.stateStep++;
                    }
                }
                // Check if player has >= 30% or < 10% health to determine level of exertion.
                else if (player->properties.player.exhaustionTimer < Q12(10.0f) && player->health >= Q12(30.0f))
                {
                    if (player->model.stateStep == 0)
                    {
                        player->model.anim.status = ANIM_STATUS(HarryAnim_Idle, false);
                        player->model.stateStep++;
                    }
                }
                else if (player->model.stateStep == 0)
                {
                    player->model.anim.status = ANIM_STATUS(HarryAnim_IdleExhausted, false);
                    player->model.stateStep++;
                }
            }
            else
            {
                if (PLAYER_EXTRA().upperBodyState == PlayerUpperBodyState_AimStop)
                {
                    if (!PLAYER_COMBAT().isAiming && player->model.stateStep == 0)
                    {
                        player->model.anim.status = g_Player_EquippedWeaponInfo.animStopAiming;
                        player->model.stateStep++;
                    }
                }
                // Melee weapon.
                else if (PLAYER_COMBAT().weaponAttack < WEAPON_ATTACK(EquippedWeaponId_Handgun, AttackInputType_Tap))
                {
                    if ((PLAYER_COMBAT().weaponAttack == WEAPON_ATTACK(EquippedWeaponId_Chainsaw,  AttackInputType_Tap) ||
                         PLAYER_COMBAT().weaponAttack == WEAPON_ATTACK(EquippedWeaponId_RockDrill, AttackInputType_Tap)) &&
                        PLAYER_PROPS().gasWeaponPowerTimer != Q12(0.0f))
                    {
                        if (player->model.stateStep == 0)
                        {
                            player->model.anim.status = ANIM_STATUS(HarryAnim_Unk33, false);
                            player->model.stateStep++;
                        }
                    }
                    else if (player->model.stateStep == 0)
                    {
                        player->model.anim.status = ANIM_STATUS(HarryAnim_HandgunAim, false);
                        player->model.stateStep++;
                    }
                }
                else if (PLAYER_PROPS().flags & PlayerFlag_Unk6)
                {
                    if (player->model.stateStep == 0)
                    {
                        player->model.anim.status = ANIM_STATUS(HarryAnim_Unk34, false);
                        player->model.stateStep++;
                    }
                }
                else if (PLAYER_EXTRA().upperBodyState == PlayerUpperBodyState_AimStartTargetLock)
                {
                    if (player->model.stateStep == 0)
                    {
                        player->model.anim.status = ANIM_STATUS(HarryAnim_Unk32, false);
                        player->model.stateStep++;
                    }
                }
                else if (PLAYER_COMBAT().weaponAttack == WEAPON_ATTACK(EquippedWeaponId_HuntingRifle, AttackInputType_Tap))
                {
                    if (player->model.stateStep == 0)
                    {
                        player->model.anim.status = ANIM_STATUS(HarryAnim_Unk29, false);
                        player->model.stateStep++;
                    }
                }
                else
                {
                    if (player->model.stateStep == 0)
                    {
                        player->model.anim.status = ANIM_STATUS(HarryAnim_HandgunAim, false);
                        player->model.stateStep++;
                    }
                }

                PLAYER_PROPS().flags &= ~PlayerFlag_Unk6;
            }

            // Set idle to move depending on user input.
            if (PLAYER_EXTRA().state == PlayerState_Combat) // Aiming at or shooting enemy.
            {
                if (ANIM_STATUS_IS_ACTIVE(player->model.anim.status) &&
                    ANIM_STATUS_IS_ACTIVE(extra->model.anim.status))
                {
                    if (player->model.anim.status >= ANIM_STATUS(HarryAnim_Unk29, false) ||
                        player->model.anim.keyframeIdx == D_800C44F0[0].endKeyframeIdx ||
                        player->model.anim.keyframeIdx == D_800C44F0[5].endKeyframeIdx)
                    {
                        if (g_Player_IsMovingForward)
                        {
                            PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_AimWalkForward;
                        }
                        else if (g_Player_IsMovingBackward)
                        {
                            PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_AimWalkBackward;
                        }
                    }
                }

                Player_MovementStateReset(player, aimState);
            }
            // Aiming at nothing, or shooting at nothing, or idle.
            else
            {
                if (ANIM_STATUS_IS_ACTIVE(player->model.anim.status) &&
                    ANIM_STATUS_IS_ACTIVE(extra->model.anim.status))
                {
                    if ((aimState == 0 && PLAYER_PROPS().moveSpeed == Q12(0.0f))||
                        player->model.anim.status >= ANIM_STATUS(HarryAnim_Unk29, false) ||
                        player->model.anim.keyframeIdx == D_800C44F0[0].endKeyframeIdx)
                    {
                        if (g_Player_IsMovingForward)
                        {
                            // Restrict aiming when going from idle to run.
                            if ((g_Player_IsRunning && lowerBodyMoveState == PlayerLowerBodyState_None) &&
                                (aimState == 0 || (( g_GameWork.config.extraWeaponCtrl && !g_Player_IsAiming) ||
                                                   (!g_GameWork.config.extraWeaponCtrl &&  g_Player_IsAiming)) &&
                                 WEAPON_ATTACK_ID_GET(PLAYER_COMBAT().weaponAttack) == EquippedWeaponId_SteelPipe))
                            {
                                PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_RunForward;
                            }
                            else
                            {
                                PLAYER_EXTRA().lowerBodyState = aimState + PlayerLowerBodyState_WalkForward;
                            }
                        }
                        else if (g_Player_IsMovingBackward)
                        {
                            PLAYER_EXTRA().lowerBodyState = aimState + PlayerLowerBodyState_WalkBackward;
                        }
                        else if (g_Player_IsSteppingRightHold)
                        {
                            player->headingAngle = player->headingAngle + Q12_ANGLE(90.0f);
                            lowerBodyMoveState                = Player_LowerBodyMoveStateGet(player, &D_800C45C8);

                            if (g_Player_IsRunning && aimState == 0 &&
                                lowerBodyMoveState == PlayerLowerBodyState_None)
                            {
                                PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_RunRight;
                            }
                            else
                            {
                                PLAYER_EXTRA().lowerBodyState = aimState + PlayerLowerBodyState_SidestepRight;
                            }
                        }
                        else if (g_Player_IsSteppingLeftHold)
                        {
                            player->headingAngle -= Q12_ANGLE(90.0f);
                            lowerBodyMoveState    = Player_LowerBodyMoveStateGet(player, &D_800C45C8);

                            if (g_Player_IsRunning && aimState == 0 &&
                                lowerBodyMoveState == PlayerLowerBodyState_None)
                            {
                                PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_RunLeft;
                            }
                            else
                            {
                                PLAYER_EXTRA().lowerBodyState = aimState + PlayerLowerBodyState_SidestepLeft;
                            }
                        }

                        if (aimState == 0 && !PLAYER_COMBAT().isAiming)
                        {
                            if (( g_GameWork.config.extraWalkRunCtrl && !g_Player_IsRunning) ||
                                (!g_GameWork.config.extraWalkRunCtrl &&  g_Player_IsRunning))
                            {
                                if (g_Player_IsMovingBackward)
                                {
                                    PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_JumpBackward;
                                    PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_RunJumpBackward;
                                    extra->model.stateStep     = 0;
                                    extra->model.controlState  = 0;
                                }
                            }
                        }

                        if (PLAYER_EXTRA().lowerBodyState == aimState && !g_Player_IsInWalkToRunTransition)
                        {
                            Player_CharaTurn_0(player, aimState);
                        }
                    }
                }

                Player_MovementStateReset(player, aimState);

                if (g_Player_IsRunning)
                {
                    Player_CharaRotate(10);
                }
                else
                {
                    Player_CharaRotate(7);
                }

                if (aimState != 0 && PLAYER_EXTRA().upperBodyState == PlayerUpperBodyState_Aim)
                {
                    if (D_800C454C != Q12(0.0f))
                    {
                        // TODO: Convert hex to clean floats. Must have been a fraction based on a constant?
                        // Determine speed if using certain weapons while moving?
                        switch (PLAYER_COMBAT().weaponAttack)
                        {
                            case WEAPON_ATTACK(EquippedWeaponId_KitchenKnife, AttackInputType_Tap):
                                PLAYER_PROPS().moveSpeed = (u32)(D_800C454C * 0x465) >> 9;
                                break;

                            case WEAPON_ATTACK(EquippedWeaponId_Chainsaw, AttackInputType_Tap):
                            case WEAPON_ATTACK(EquippedWeaponId_Katana,   AttackInputType_Tap):
                            case WEAPON_ATTACK(EquippedWeaponId_Axe,      AttackInputType_Tap):
                                PLAYER_PROPS().moveSpeed = (u32)(D_800C454C * 0x15F9) >> 11;
                                break;

                            case WEAPON_ATTACK(EquippedWeaponId_SteelPipe, AttackInputType_Tap):
                            case WEAPON_ATTACK(EquippedWeaponId_Hammer,    AttackInputType_Tap):
                                PLAYER_PROPS().moveSpeed = ((u32)(D_800C454C * 0xD2F) >> 10);
                                break;

                            case WEAPON_ATTACK(EquippedWeaponId_RockDrill,    AttackInputType_Tap):
                            case WEAPON_ATTACK(EquippedWeaponId_Handgun,      AttackInputType_Tap):
                            case WEAPON_ATTACK(EquippedWeaponId_HuntingRifle, AttackInputType_Tap):
                            case WEAPON_ATTACK(EquippedWeaponId_Shotgun,      AttackInputType_Tap):
                            case WEAPON_ATTACK(EquippedWeaponId_HyperBlaster, AttackInputType_Tap):
                                PLAYER_PROPS().moveSpeed = (-(D_800C454C * 0x87F0) >> 14);
                                break;
                        }

                        if (g_DeltaTime != Q12(0.0f))
                        {
                            PLAYER_PROPS().moveSpeed = ((PLAYER_PROPS().moveSpeed * 0x88) / g_DeltaTime);
                        }

                        // Restart timer for idle animation.
                        if (D_800C454C != Q12(0.0f))
                        {
                            player->properties.player.afkTimer = Q12(0.0f);
                        }
                    }
                }
                // Move without aiming.
                else if (D_800C454C != Q12(0.0f))
                {
                    player->properties.player.afkTimer = Q12(0.0f);
                }

                // Turn if idle.
                if (g_Player_IsTurningLeft && player->model.stateStep == 1 &&
                    (player->model.anim.status == ANIM_STATUS(HarryAnim_Idle, true) ||
                     player->model.anim.status == ANIM_STATUS(HarryAnim_IdleExhausted, true)))
                {
                    player->model.stateStep   = 2;
                    player->model.anim.status = ANIM_STATUS(HarryAnim_TurnLeft, false);
                }
                else if (g_Player_IsTurningRight && player->model.stateStep == 1 &&
                         (player->model.anim.status == ANIM_STATUS(HarryAnim_Idle, true) ||
                          player->model.anim.status == ANIM_STATUS(HarryAnim_IdleExhausted, true)))
                {
                    player->model.stateStep   = 2;
                    player->model.anim.status = ANIM_STATUS(HarryAnim_TurnRight, false);
                }

                if (!g_Player_IsTurningLeft && !g_Player_IsTurningRight && player->model.stateStep == 2 &&
                    (player->model.anim.status == ANIM_STATUS(HarryAnim_TurnRight, true) ||
                     player->model.anim.status == ANIM_STATUS(HarryAnim_TurnLeft, true)))
                {
                    player->model.anim.status = ANIM_STATUS(HarryAnim_Idle, false);
                    player->model.stateStep   = 0;
                }
            }

            if (PLAYER_PROPS().moveSpeed == Q12(0.0f) ||
                 g_Player_IsTurningLeft || g_Player_IsTurningRight)
            {
                PLAYER_PROPS().headingAngle = Q12_ANGLE(0.0f);
                g_Player_HeadingAngle    = Q12_ANGLE(0.0f);
            }
            break;

        case PlayerLowerBodyState_WalkForward:
        case PlayerLowerBodyState_AimWalkForward:
            if (!g_Player_IsMovingForward)
            {
                g_SysWork.playerStopFlags |= PlayerStopFlag_StopWalking;
            }

            if ((g_SysWork.playerStopFlags & PlayerStopFlag_StopWalking) &&
                PLAYER_EXTRA().lowerBodyState < PlayerLowerBodyState_Aim &&
                PLAYER_EXTRA().upperBodyState != PlayerUpperBodyState_AimStop)
            {
                if (PLAYER_PROPS().moveSpeed != Q12(0.0f))
                {
                    PLAYER_PROPS().moveSpeed -= (TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f))) * 2;

                    if ((PLAYER_PROPS().moveSpeed >> 16) & (1 << 0))
                    {
                        PLAYER_PROPS().moveSpeed = Q12(0.0f);
                    }
                }
            }
            // Walking.
            else
            {
                if (g_Controller0->rawSticks.sticks.leftY <= -STICK_DEADZONE)
                {
                    g_Player_MoveStickMag = ABS(g_Controller0->rawSticks.sticks.leftY);
                    func_80070B84(player, Q12(0.75f), Q12(1.4f), 2);
                }
                // Stopped walking.
                else
                {
                    if (g_Player_MoveStickMag != Q7(0.0f))
                    {
                        func_80070B84(player, Q12(0.75f), Q12(1.4f), 2);
                    }
                    // Reduce speed if going too fast while walking.
                    else if (PLAYER_PROPS().moveSpeed > Q12(1.4f))
                    {
                        PLAYER_PROPS().moveSpeed -= (TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f))) * 2;
                        if (PLAYER_PROPS().moveSpeed < Q12(1.4f))
                        {
                            PLAYER_PROPS().moveSpeed = Q12(1.4f);
                        }
                    }
                    else if (PLAYER_PROPS().moveSpeed < Q12(1.4f))
                    {
                        if (player->model.anim.keyframeIdx >= 2)
                        {
                            PLAYER_PROPS().moveSpeed += TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f));
                        }

                        PLAYER_PROPS().moveSpeed = CLAMP(PLAYER_PROPS().moveSpeed, Q12(0.0f), Q12(1.4f));
                    }

                    if (g_Controller0->buttonFlags.held & ControllerFlag_LStickHighUp)
                    {
                        g_Player_MoveStickMag = Q7(0.0f);
                    }
                }
            }

            if (player->model.stateStep == 0)
            {
                player->model.anim.status = ANIM_STATUS(HarryAnim_WalkForward, false);
                player->model.stateStep++;
            }

            // Something related to anim and states when aiming or attacking while moving.
            if (PLAYER_EXTRA().state == PlayerState_Combat)
            {
                if (g_SysWork.playerStopFlags & PlayerStopFlag_StopWalking)
                {
                    if ((PLAYER_EXTRA().lowerBodyState < PlayerLowerBodyState_Aim &&
                         PLAYER_EXTRA().upperBodyState != PlayerUpperBodyState_AimStop) ||
                         (player->model.anim.keyframeIdx >= 10 && player->model.anim.keyframeIdx <= 11) ||
                          player->model.anim.keyframeIdx == 22 || player->model.anim.keyframeIdx == 21)
                    {
                        PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_Aim;
                    }
                }

                Player_MovementStateReset(player, aimState | (1 << 0));

                if (PLAYER_COMBAT().weaponAttack != WEAPON_ATTACK(EquippedWeaponId_HyperBlaster, AttackInputType_Tap))
                {
                    Player_CharaRotate(5);
                }
            }
            else
            {
                if (!(g_SysWork.playerStopFlags & PlayerStopFlag_StopWalking))
                {
                    // Code to change the player's state to running.
                    if (g_Player_IsRunning)
                    {
                        if (aimState == 0 && lowerBodyMoveState == PlayerLowerBodyState_None &&
                            (PLAYER_EXTRA().upperBodyState == PlayerUpperBodyState_WalkForward ||
                             PLAYER_EXTRA().upperBodyState == PlayerUpperBodyState_AimStop))
                        {
                            if (player->model.anim.keyframeIdx >= 10 && player->model.anim.keyframeIdx <= 11)
                            {
                                PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_RunForward;
                                stillAnim.endKeyframeIdx   = 36;
                                stillAnim.linkStatus       = ANIM_STATUS(HarryAnim_RunForward, true);
                                PLAYER_PROPS().flags         |= PlayerFlag_Unk5;
                            }
                            else if (player->model.anim.keyframeIdx >= 21 && player->model.anim.keyframeIdx <= 22)
                            {
                                PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_RunForward;
                                stillAnim.endKeyframeIdx   = 26;
                                stillAnim.linkStatus       = ANIM_STATUS(HarryAnim_RunForward, true);
                            }
                        }
                    }
                }
                else if ((PLAYER_EXTRA().lowerBodyState < PlayerLowerBodyState_Aim &&
                          PLAYER_EXTRA().upperBodyState != PlayerUpperBodyState_AimStop) ||
                         (player->model.anim.keyframeIdx >= 10 && player->model.anim.keyframeIdx <= 11) ||
                          player->model.anim.keyframeIdx == 22 || player->model.anim.keyframeIdx == 21)
                {
                    // Aparently, code intended to change player's state if the player stop walking while either aiming or attacking.
                    if (PLAYER_COMBAT().weaponAttack < WEAPON_ATTACK(EquippedWeaponId_Handgun, AttackInputType_Tap) &&
                        aimState != 0)
                    {
                        if (((extra->model.anim.status == ANIM_STATUS(HarryAnim_Unk29, true) ||
                              extra->model.anim.status == ANIM_STATUS(HarryAnim_Unk30, true)) &&
                            (PLAYER_COMBAT().weaponAttack != WEAPON_ATTACK(EquippedWeaponId_Chainsaw,  AttackInputType_Tap) &&
                             PLAYER_COMBAT().weaponAttack != WEAPON_ATTACK(EquippedWeaponId_RockDrill, AttackInputType_Tap))) ||
                            extra->model.anim.status == ANIM_STATUS(HarryAnim_HandgunRecoil, true))
                        {
                            PLAYER_PROPS().flags         |= PlayerFlag_Unk10;
                            player->model.stateStep    = 0;
                            player->model.controlState = 0;
                            PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_Attack;
                        }
                        else
                        {
                            PLAYER_EXTRA().lowerBodyState = aimState;
                            PLAYER_PROPS().flags         &= ~PlayerFlag_Unk10;
                        }
                    }
                    else
                    {
                        PLAYER_EXTRA().lowerBodyState = aimState;
                        PLAYER_PROPS().flags         &= ~PlayerFlag_Unk10;
                    }
                }

                if (PLAYER_EXTRA().lowerBodyState == (aimState + PlayerLowerBodyState_WalkForward) &&
                    !g_Player_IsInWalkToRunTransition)
                {
                    Player_CharaTurn_0(player, aimState);
                }

                Player_MovementStateReset(player, aimState | (1 << 0));
                Player_CharaRotate(5);
            }

            PLAYER_PROPS().headingAngle = Q12_ANGLE(0.0f);
            g_Player_HeadingAngle    = Q12_ANGLE(0.0f);

            if (PLAYER_EXTRA().lowerBodyState == PlayerLowerBodyState_RunForward)
            {
                player->model.anim.status = ANIM_STATUS(HarryAnim_Still, false);
                player->model.stateStep++;
                g_Player_IsInWalkToRunTransition = true;
            }
            break;

        case PlayerLowerBodyState_RunForward:
            player->properties.player.exhaustionTimer += g_DeltaTime;

            if (g_Controller0->rawSticks.sticks.leftY <= -STICK_DEADZONE)
            {
                g_Player_MoveStickMag = ABS(g_Controller0->rawSticks.sticks.leftY);

                moveOffsetX = GET_MOVE_SPEED(speedZoneType);

                if (PLAYER_PROPS().moveSpeed < Q12(3.5f))
                {
                    moveDistForward = TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.75f));
                }
                else
                {
                    moveOffsetZ     = g_DeltaTime;
                    moveOffsetZ    += (moveOffsetZ < 0) ? 3 : 0;
                    moveDistForward = DIV_FAST(moveOffsetZ, 4);
                }

                func_80070CF0(player, Q12(2.0f), moveOffsetX, moveDistForward, TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f)));
            }
            // Stopped running.
            else
            {
                if (g_Player_MoveStickMag != Q7(0.0f))
                {
                    moveOffsetX = GET_MOVE_SPEED(speedZoneType);

                    if (PLAYER_PROPS().moveSpeed < Q12(3.5f))
                    {
                        moveDistForward = TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.75f));
                    }
                    else
                    {
                        moveOffsetZ     = g_DeltaTime;
                        moveOffsetZ    += (moveOffsetZ < Q12(0.0f)) ? 3 : Q12(0.0f);
                        moveDistForward = DIV_FAST(moveOffsetZ, 4);
                    }

                    func_80070CF0(player, Q12(2.0f), moveOffsetX, moveDistForward, TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f)));
                }
                else if (PLAYER_PROPS().moveSpeed > GET_MOVE_SPEED(speedZoneType))
                {
                    PLAYER_PROPS().moveSpeed -= TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f));
                    if (PLAYER_PROPS().moveSpeed < GET_MOVE_SPEED(speedZoneType))
                    {
                        PLAYER_PROPS().moveSpeed = GET_MOVE_SPEED(speedZoneType);
                    }
                }
                else
                {
                    if (PLAYER_PROPS().moveSpeed < GET_MOVE_SPEED(speedZoneType))
                    {
                        PLAYER_PROPS().moveSpeed += GET_VAL(PLAYER_PROPS().moveSpeed);
                        PLAYER_PROPS().moveSpeed  = CLAMP(PLAYER_PROPS().moveSpeed, 0, GET_MOVE_SPEED(speedZoneType));
                    }
                }

                if (g_Controller0->buttonFlags.held & ControllerFlag_LStickHighUp)
                {
                    g_Player_MoveStickMag = Q7(0.0f);
                }
            }

            if (player->model.stateStep == 0)
            {
                player->model.anim.status = ANIM_STATUS(HarryAnim_RunForward, false);
                player->model.stateStep++;
            }

            if ((player->model.anim.keyframeIdx == 43 ||
                 player->model.anim.keyframeIdx == 33) &&
                player->position.vy == player->properties.player.floorHeight)
            {
                player->fallSpeed = Q12(-1.25f);
            }

            // Running.
            if (PLAYER_EXTRA().upperBodyState != PlayerUpperBodyState_AimStartTargetLock &&
                player->model.anim.status == ANIM_STATUS(HarryAnim_RunForward, true))
            {
                switch (lowerBodyMoveState)
                {
                    case PlayerLowerBodyState_WalkForward:
                        if (player->properties.player.runDistance >= RUN_STUMBLE_TRIGGER_DIST)
                        {
                            PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_Stumble;
                        }
                        else if (player->model.anim.keyframeIdx >= 30 &&
                                 player->model.anim.keyframeIdx <= 31)
                        {
                            PLAYER_EXTRA().lowerBodyState = lowerBodyMoveState;
                            stillAnim.endKeyframeIdx   = 8;
                            stillAnim.linkStatus       = ANIM_STATUS(HarryAnim_WalkForward, true);
                        }
                        else if (player->model.anim.keyframeIdx >= 41 &&
                                 player->model.anim.keyframeIdx <= 42)
                        {
                            PLAYER_EXTRA().lowerBodyState = lowerBodyMoveState;
                            stillAnim.endKeyframeIdx   = 20;
                            stillAnim.linkStatus       = ANIM_STATUS(HarryAnim_WalkForward, true);
                        }
                        break;

                    case PlayerLowerBodyState_RunForward:
                        if (player->properties.player.runDistance >= RUN_STUMBLE_TRIGGER_DIST)
                        {
                            PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_RunForwardWallStop;
                        }
                        else if (player->model.anim.keyframeIdx >= 30 &&
                                 player->model.anim.keyframeIdx <= 31)
                        {
                            PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_WalkForward;
                            stillAnim.endKeyframeIdx   = 8;
                            stillAnim.linkStatus       = ANIM_STATUS(HarryAnim_WalkForward, true);
                        }
                        else if (player->model.anim.keyframeIdx >= 41 &&
                                 player->model.anim.keyframeIdx <= 42)
                        {
                            PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_WalkForward;
                            stillAnim.endKeyframeIdx   = 20;
                            stillAnim.linkStatus       = ANIM_STATUS(HarryAnim_WalkForward, true);
                        }
                        break;

                    default:
                        if (!g_Player_IsRunning || !g_Player_IsMovingForward)
                        {
                            // Change state from running to walking.
                            if (g_Player_IsMovingForward)
                            {
                                if (player->model.anim.keyframeIdx >= 30 &&
                                    player->model.anim.keyframeIdx <= 31)
                                {
                                    PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_WalkForward;
                                    stillAnim.endKeyframeIdx   = 8;
                                    stillAnim.linkStatus       = ANIM_STATUS(HarryAnim_WalkForward, true);
                                }
                                else if (player->model.anim.keyframeIdx >= 41 &&
                                         player->model.anim.keyframeIdx <= 42)
                                {
                                    PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_WalkForward;
                                    stillAnim.endKeyframeIdx   = 20;
                                    stillAnim.linkStatus       = ANIM_STATUS(HarryAnim_WalkForward, true);
                                }
                            }
                            // Set stumble anim if crashed into a wall.
                            else if (player->properties.player.runStepSfxCount >= 5 &&
                                     PLAYER_PROPS().moveSpeed >= Q12(3.125f))
                            {
                                if (player->model.anim.keyframeIdx >= 33 &&
                                    player->model.anim.keyframeIdx <= 34)
                                {
                                    PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_RunForwardWallStop;
                                    PLAYER_PROPS().flags         &= ~PlayerFlag_WallStopRight;
                                }
                                else if (player->model.anim.keyframeIdx >= 43 &&
                                         player->model.anim.keyframeIdx <= 44)
                                {
                                    PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_RunForwardWallStop;
                                    PLAYER_PROPS().flags         |= PlayerFlag_WallStopRight;
                                }
                            }
                            // Change state from run to walk. Difference with first conditional is this only triggers if
                            // a walk is interrupted and a wall crash anim was not triggered.
                            // In-game, appears as though player goes directly to idle. Mechanically, it goes through this state, then to idle.
                            else
                            {
                                if (player->model.anim.keyframeIdx >= 30 &&
                                    player->model.anim.keyframeIdx <= 31)
                                {
                                    PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_WalkForward;
                                    stillAnim.endKeyframeIdx   = 8;
                                    stillAnim.linkStatus       = ANIM_STATUS(HarryAnim_WalkForward, true);
                                }
                                else if (player->model.anim.keyframeIdx >= 41 &&
                                         player->model.anim.keyframeIdx <= 42)
                                {
                                    PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_WalkForward;
                                    stillAnim.endKeyframeIdx   = 20;
                                    stillAnim.linkStatus       = ANIM_STATUS(HarryAnim_WalkForward, true);
                                }
                            }
                        }
                        break;
                }
            }

            if (PLAYER_EXTRA().lowerBodyState != PlayerLowerBodyState_RunForward)
            {
                Player_MovementStateReset(player, PlayerLowerBodyState_RunForward);
            }

            Player_CharaRotate(4);

            g_Player_HeadingAngle    = Q12_ANGLE(0.0f);
            PLAYER_PROPS().headingAngle = Q12_ANGLE(0.0f);

            if (PLAYER_EXTRA().lowerBodyState == PlayerLowerBodyState_WalkForward)
            {
                player->model.anim.status = ANIM_STATUS(HarryAnim_Still, false);
                player->model.stateStep++;
                g_Player_IsInWalkToRunTransition = true;
            }
            break;

        case PlayerLowerBodyState_RunForwardWallStop:
            if (PLAYER_PROPS().moveSpeed != Q12(0.0f))
            {
                PLAYER_PROPS().moveSpeed -= (TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f))) >> 1;
                if ((PLAYER_PROPS().moveSpeed >> 16) & (1 << 0))
                {
                    PLAYER_PROPS().moveSpeed = Q12(0.0f);
                }
            }

            // Depending on frame of gait cycle, set left or right wall stop anim variant.
            if (PLAYER_PROPS().flags & PlayerFlag_WallStopRight)
            {
                if (player->model.stateStep == 0)
                {
                    player->model.anim.status = ANIM_STATUS(HarryAnim_RunForwardWallStopRight, false);
                    player->model.stateStep++;
                }
            }
            else if (player->model.stateStep == 0)
            {
                player->model.anim.status = ANIM_STATUS(HarryAnim_RunForwardWallStopLeft, false);
                player->model.stateStep++;
            }

            if (PLAYER_EXTRA().upperBodyState != PlayerUpperBodyState_AimStartTargetLock)
            {
                if (player->model.anim.status == ANIM_STATUS(HarryAnim_RunForwardWallStopLeft, true) &&
                    player->model.anim.keyframeIdx >= 168 ||
                    player->model.anim.status == ANIM_STATUS(HarryAnim_RunForwardWallStopRight, true) &&
                    player->model.anim.keyframeIdx >= 158)
                {
                    PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_None;
                    Player_MovementStateReset(player, PlayerLowerBodyState_RunForwardWallStop);
                }

                Player_CharaRotate(4);
            }
            break;

        case PlayerLowerBodyState_WalkBackward:
        case PlayerLowerBodyState_AimWalkBackward:
            if (!g_Player_IsMovingBackward)
            {
                g_SysWork.playerStopFlags |= PlayerStopFlag_StopRunning;
            }

            if ((g_SysWork.playerStopFlags & PlayerStopFlag_StopRunning) &&
                PLAYER_EXTRA().lowerBodyState < PlayerLowerBodyState_Aim &&
                PLAYER_EXTRA().upperBodyState != PlayerUpperBodyState_AimStop)
            {
                if (PLAYER_PROPS().moveSpeed != Q12(0.0f))
                {
                    PLAYER_PROPS().moveSpeed -= ((TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f))) * 2);
                    if ((PLAYER_PROPS().moveSpeed >> 16) & (1 << 0))
                    {
                        PLAYER_PROPS().moveSpeed = Q12(0.0f);
                    }
                }
            }
            // Walking backward.
            else if (g_Controller0->rawSticks.sticks.leftY >= STICK_DEADZONE)
            {
                g_Player_MoveStickMag = ABS(g_Controller0->rawSticks.sticks.leftY);
                func_80070B84(player, Q12(0.75f), Q12(1.15f), 2);
            }
            // Stop walking backward.
            else
            {
                if (g_Player_MoveStickMag != Q7(0.0f))
                {
                    func_80070B84(player, Q12(0.75f), Q12(1.15f), 2);
                }
                else if (PLAYER_PROPS().moveSpeed > Q12(1.15f))
                {
                    PLAYER_PROPS().moveSpeed -= (TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f))) * 2;
                    if (PLAYER_PROPS().moveSpeed < Q12(1.15f))
                    {
                        PLAYER_PROPS().moveSpeed = Q12(1.15f);
                    }
                }
                else if (PLAYER_PROPS().moveSpeed < Q12(1.15f))
                {
                    if (player->model.anim.keyframeIdx >= 2)
                    {
                        PLAYER_PROPS().moveSpeed += TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f));
                    }

                    PLAYER_PROPS().moveSpeed = CLAMP(PLAYER_PROPS().moveSpeed, Q12(0.0f), Q12(1.15f));
                }

                if (g_Controller0->buttonFlags.held & ControllerFlag_LStickHighDown)
                {
                    g_Player_MoveStickMag = Q7(0.0f);
                }
            }

            if (player->model.stateStep == 0)
            {
                player->model.anim.status = ANIM_STATUS(HarryAnim_WalkBackward, false);
                player->model.stateStep++;
            }

            if (PLAYER_EXTRA().state == PlayerState_Combat)
            {
                if (g_SysWork.playerStopFlags & PlayerStopFlag_StopRunning)
                {
                    if ((PLAYER_EXTRA().lowerBodyState < PlayerLowerBodyState_Aim &&
                         PLAYER_EXTRA().upperBodyState != PlayerUpperBodyState_AimStop) ||
                         (player->model.anim.keyframeIdx >= 56 &&
                          player->model.anim.keyframeIdx <= 57) ||
                         player->model.anim.keyframeIdx == 67 ||
                         player->model.anim.keyframeIdx == 66)
                    {
                        PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_Aim;
                    }
                }

                Player_MovementStateReset(player, aimState + PlayerLowerBodyState_WalkBackward);

                if (PLAYER_COMBAT().weaponAttack != WEAPON_ATTACK(EquippedWeaponId_HyperBlaster, AttackInputType_Tap))
                {
                    Player_CharaRotate(5);
                }
            }
            else
            {
                if (!(g_SysWork.playerStopFlags & PlayerStopFlag_StopRunning))
                {
                    if (((player->model.anim.keyframeIdx >= 66 &&
                          player->model.anim.keyframeIdx <= 67) ||
                         player->model.anim.keyframeIdx == 46 ||
                         player->model.anim.keyframeIdx == 47) &&
                        aimState == 0 && PLAYER_EXTRA().upperBodyState != PlayerUpperBodyState_AimStop)
                    {
                        if (( g_GameWork.config.extraWalkRunCtrl && !g_Player_IsRunning) ||
                            (!g_GameWork.config.extraWalkRunCtrl &&  g_Player_IsRunning))
                        {
                            if (g_Player_IsMovingBackward)
                            {
                                PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_JumpBackward;
                                PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_RunJumpBackward;
                                extra->model.stateStep     = 0;
                                extra->model.controlState  = 0;
                            }
                        }
                    }
                }
                else if ((PLAYER_EXTRA().lowerBodyState < PlayerLowerBodyState_Aim &&
                          PLAYER_EXTRA().upperBodyState != PlayerUpperBodyState_AimStop) ||
                         (player->model.anim.keyframeIdx >= 56 && player->model.anim.keyframeIdx <= 57) ||
                          player->model.anim.keyframeIdx == 67 || player->model.anim.keyframeIdx == 66)
                {
                    if (PLAYER_COMBAT().weaponAttack < WEAPON_ATTACK(EquippedWeaponId_Handgun, AttackInputType_Tap) &&
                        aimState != 0)
                    {
                        if (((extra->model.anim.status == ANIM_STATUS(HarryAnim_Unk29, true) ||
                              extra->model.anim.status == ANIM_STATUS(HarryAnim_Unk30, true)) &&
                             (PLAYER_COMBAT().weaponAttack != WEAPON_ATTACK(EquippedWeaponId_Chainsaw,  AttackInputType_Tap) &&
                              PLAYER_COMBAT().weaponAttack != WEAPON_ATTACK(EquippedWeaponId_RockDrill, AttackInputType_Tap))) ||
                            extra->model.anim.status == ANIM_STATUS(HarryAnim_HandgunRecoil, true))
                        {
                            PLAYER_PROPS().flags         |= PlayerFlag_Unk10;
                            player->model.stateStep    = 0;
                            player->model.controlState = 0;
                            PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_Attack;
                        }
                        else
                        {
                            PLAYER_EXTRA().lowerBodyState = aimState;
                            PLAYER_PROPS().flags         &= ~PlayerFlag_Unk10;
                        }
                    }
                    else
                    {
                        PLAYER_EXTRA().lowerBodyState = aimState;
                        PLAYER_PROPS().flags         &= ~PlayerFlag_Unk10;
                    }
                }

                if (PLAYER_EXTRA().lowerBodyState == (aimState + PlayerLowerBodyState_WalkBackward) &&
                    !g_Player_IsInWalkToRunTransition)
                {
                    Player_CharaTurn_0(player, aimState);
                }

                Player_MovementStateReset(player, aimState + PlayerLowerBodyState_WalkBackward);
                Player_CharaRotate(4);
            }

            PLAYER_PROPS().headingAngle = Q12_ANGLE(180.0f);
            g_Player_HeadingAngle    = Q12_ANGLE(180.0f);
            break;

        case PlayerLowerBodyState_SidestepRight:
        case PlayerLowerBodyState_AimSidestepRight:
            if (PLAYER_PROPS().moveSpeed > Q12(1.25f))
            {
                PLAYER_PROPS().moveSpeed -= (TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f)));
                if (PLAYER_PROPS().moveSpeed < Q12(1.25f))
                {
                    PLAYER_PROPS().moveSpeed = Q12(1.25f);
                }
            }
            else
            {
                if (player->model.anim.keyframeIdx >= 100 &&
                    player->model.anim.keyframeIdx <= 111)
                {
                    PLAYER_PROPS().moveSpeed += TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f));
                }
                else if (player->model.anim.keyframeIdx >= 112)
                {
                    PLAYER_PROPS().moveSpeed -= TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f));
                }

                PLAYER_PROPS().moveSpeed = CLAMP(PLAYER_PROPS().moveSpeed, Q12(0.0f), Q12(1.25f));
            }

            if (player->model.stateStep == 0)
            {
                player->model.anim.status = ANIM_STATUS(HarryAnim_SidestepRight, false);
                player->model.stateStep++;
            }

            if (player->model.anim.status == ANIM_STATUS(HarryAnim_SidestepRight, true) &&
                player->model.anim.keyframeIdx >= 117)
            {
                // Stopped sidestepping while attacking.
                // If attacking with gun, dispatches to idle aim state instead of attack state.
                if (!g_Player_IsSteppingRightHold)
                {
                    if (PLAYER_COMBAT().weaponAttack < WEAPON_ATTACK(EquippedWeaponId_Handgun, AttackInputType_Tap) &&
                        aimState != 0)
                    {
                        // Some melee weapons allow attack while sidestepping.
                        if (((extra->model.anim.status == ANIM_STATUS(HarryAnim_Unk29, true) ||
                              extra->model.anim.status == ANIM_STATUS(HarryAnim_Unk30, true)) &&
                             (PLAYER_COMBAT().weaponAttack != WEAPON_ATTACK(EquippedWeaponId_Chainsaw,  AttackInputType_Tap) &&
                              PLAYER_COMBAT().weaponAttack != WEAPON_ATTACK(EquippedWeaponId_RockDrill, AttackInputType_Tap))) ||
                              extra->model.anim.status == ANIM_STATUS(HarryAnim_HandgunRecoil, true))
                        {
                            PLAYER_PROPS().flags         |= PlayerFlag_Unk10;
                            player->model.stateStep    = 0;
                            player->model.controlState = 0;
                            PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_Attack;
                        }
                        else
                        {
                            PLAYER_EXTRA().lowerBodyState = aimState;
                        }
                    }
                    else
                    {
                        PLAYER_EXTRA().lowerBodyState = aimState;
                    }
                }
                else if (g_Player_IsRunning && aimState == 0 && lowerBodyMoveState == PlayerLowerBodyState_None)
                {
                    if (PLAYER_EXTRA().upperBodyState != PlayerUpperBodyState_AimStop)
                    {
                        PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_RunRight;
                    }
                }
            }

            Player_CharaTurn_0(player, aimState);
            Player_MovementStateReset(player, aimState + PlayerLowerBodyState_SidestepRight);
            Player_CharaRotate(3);

            PLAYER_PROPS().headingAngle = Q12_ANGLE(90.0f);
            g_Player_HeadingAngle    = Q12_ANGLE(90.0f);
            break;

        case PlayerLowerBodyState_SidestepLeft:
        case PlayerLowerBodyState_AimSidestepLeft:
            if (PLAYER_PROPS().moveSpeed > Q12(1.25f))
            {
                PLAYER_PROPS().moveSpeed -= TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f));
                if (PLAYER_PROPS().moveSpeed < Q12(1.25f))
                {
                    PLAYER_PROPS().moveSpeed = Q12(1.25f);
                }
            }
            else
            {
                if (player->model.anim.keyframeIdx >= 75 &&
                    player->model.anim.keyframeIdx <= 86)
                {
                    PLAYER_PROPS().moveSpeed += TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f));
                }
                else if (player->model.anim.keyframeIdx >= 87)
                {
                    PLAYER_PROPS().moveSpeed -= TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f));
                }

                PLAYER_PROPS().moveSpeed = CLAMP(PLAYER_PROPS().moveSpeed, Q12(0.0f), Q12(1.25f));
            }

            if (player->model.stateStep == 0)
            {
                player->model.anim.status = ANIM_STATUS(HarryAnim_SidestepLeft, false);
                player->model.stateStep++;
            }

            if (player->model.anim.status == ANIM_STATUS(HarryAnim_SidestepLeft, true) &&
                player->model.anim.keyframeIdx >= 92)
            {
                // Stopped stepping while attacking.
                // If attacking with gun, dispatches to idle aim state instead of attack state.
                if (!g_Player_IsSteppingLeftHold)
                {
                    if (PLAYER_COMBAT().weaponAttack < WEAPON_ATTACK(EquippedWeaponId_Handgun, AttackInputType_Tap) &&
                        aimState != 0)
                    {
                        if (((extra->model.anim.status == ANIM_STATUS(HarryAnim_Unk29, true) ||
                              extra->model.anim.status == ANIM_STATUS(HarryAnim_Unk30, true)) &&
                             (PLAYER_COMBAT().weaponAttack != WEAPON_ATTACK(EquippedWeaponId_Chainsaw,  AttackInputType_Tap) &&
                              PLAYER_COMBAT().weaponAttack != WEAPON_ATTACK(EquippedWeaponId_RockDrill, AttackInputType_Tap))) ||
                             extra->model.anim.status == ANIM_STATUS(HarryAnim_HandgunRecoil, true))
                        {
                            PLAYER_PROPS().flags         |= PlayerFlag_Unk10;
                            player->model.stateStep    = 0;
                            player->model.controlState = 0;
                            PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_Attack;
                        }
                        else
                        {
                            PLAYER_EXTRA().lowerBodyState = aimState;
                        }
                    }
                    else
                    {
                        PLAYER_EXTRA().lowerBodyState = aimState;
                    }
                }
                else if (g_Player_IsRunning && aimState == 0 && lowerBodyMoveState == PlayerLowerBodyState_None)
                {
                    if (PLAYER_EXTRA().upperBodyState != PlayerUpperBodyState_AimStop)
                    {
                        PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_RunLeft;
                    }
                }
            }

            Player_CharaTurn_1(player, aimState);
            Player_MovementStateReset(player, aimState + PlayerLowerBodyState_SidestepLeft);
            Player_CharaRotate(3);

            PLAYER_PROPS().headingAngle = Q12_ANGLE(-90.0f);
            g_Player_HeadingAngle    = Q12_ANGLE(-90.0f);
            break;

        case PlayerLowerBodyState_RunRight:
            player->properties.player.exhaustionTimer += g_DeltaTime;
            if (PLAYER_PROPS().moveSpeed > Q12(3.1739f))
            {
                PLAYER_PROPS().moveSpeed -= TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f));
                if (PLAYER_PROPS().moveSpeed < Q12(3.1739f))
                {
                    PLAYER_PROPS().moveSpeed = Q12(3.1739f);
                }
            }
            else if (PLAYER_PROPS().moveSpeed < Q12(3.1739f))
            {
                PLAYER_PROPS().moveSpeed += TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.75f));
                PLAYER_PROPS().moveSpeed  = CLAMP(PLAYER_PROPS().moveSpeed, Q12(0.0f), Q12(3.1739f));
            }

            if (player->model.stateStep == 0)
            {
                player->model.anim.status = ANIM_STATUS(HarryAnim_RunRight, false);
                player->model.stateStep++;
            }

            if ((player->model.anim.keyframeIdx == 139 ||
                 player->model.anim.keyframeIdx == 145) &&
                player->position.vy == player->properties.player.floorHeight)
            {
                player->fallSpeed = Q12(-1.0f);
            }

            if (PLAYER_EXTRA().upperBodyState != PlayerUpperBodyState_AimStartTargetLock)
            {
                switch (lowerBodyMoveState)
                {
                    case PlayerLowerBodyState_WalkForward:
                        if (player->properties.player.runDistance >= RUN_STUMBLE_TRIGGER_DIST)
                        {
                            PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_RunRightStumble;
                        }
                        else
                        {
                            if (player->model.anim.status == ANIM_STATUS(HarryAnim_RunRight, true) &&
                                player->model.anim.keyframeIdx >= 147)
                            {
                                PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_None;
                            }
                        }
                        break;

                    case PlayerLowerBodyState_RunForward:
                        if (player->properties.player.runDistance >= RUN_STUMBLE_TRIGGER_DIST)
                        {
                            PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_RunRightWallStop;
                        }
                        else
                        {
                            if (player->model.anim.status == ANIM_STATUS(HarryAnim_RunRight, true) &&
                                player->model.anim.keyframeIdx >= 147)
                            {
                                PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_None;
                            }
                        }
                        break;

                    default:
                        if (player->properties.player.runStepSfxCount >= 5 &&
                            PLAYER_PROPS().moveSpeed >= Q12(3.125f))
                        {
                            if (player->model.anim.keyframeIdx >= 144 && (!g_Player_IsRunning || !g_Player_IsSteppingRightHold))
                            {
                                PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_RunRightWallStop;
                            }
                        }
                        else if (player->model.anim.status == ANIM_STATUS(HarryAnim_RunRight, true) &&
                                 player->model.anim.keyframeIdx >= 147 &&
                                 (!g_Player_IsRunning || !g_Player_IsSteppingRightHold))
                        {
                            PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_None;
                        }
                        break;
                }
            }

            Player_MovementStateReset(player, PlayerLowerBodyState_RunRight);
            Player_CharaRotate(4);

            PLAYER_PROPS().headingAngle = Q12_ANGLE(90.0f);
            g_Player_HeadingAngle    = Q12_ANGLE(90.0f);
            break;

        case PlayerLowerBodyState_RunLeft:
            player->properties.player.exhaustionTimer += g_DeltaTime;
            if (PLAYER_PROPS().moveSpeed > Q12(3.1739f))
            {
                PLAYER_PROPS().moveSpeed -= TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f));
                if (PLAYER_PROPS().moveSpeed < Q12(3.1739f))
                {
                    PLAYER_PROPS().moveSpeed = Q12(3.1739f);
                }
            }
            else if (PLAYER_PROPS().moveSpeed < Q12(3.1739f))
            {
                PLAYER_PROPS().moveSpeed += TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.75f));
                PLAYER_PROPS().moveSpeed  = CLAMP(PLAYER_PROPS().moveSpeed, Q12(0.0f), Q12(3.1739f));
            }

            if (player->model.stateStep == 0)
            {
                player->model.anim.status = ANIM_STATUS(HarryAnim_RunLeft, false);
                player->model.stateStep++;
            }

            if ((player->model.anim.keyframeIdx == 125 ||
                 player->model.anim.keyframeIdx == 132) &&
                player->position.vy == player->properties.player.floorHeight)
            {
                player->fallSpeed = Q12(-1.0f);
            }

            if (PLAYER_EXTRA().upperBodyState != PlayerUpperBodyState_AimStartTargetLock)
            {
                switch (lowerBodyMoveState)
                {
                    case PlayerLowerBodyState_WalkForward:
                        if (player->properties.player.runDistance >= RUN_STUMBLE_TRIGGER_DIST)
                        {
                            PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_RunLeftStumble;
                        }
                        else
                        {
                            if (player->model.anim.status == ANIM_STATUS(HarryAnim_RunLeft, true) &&
                                player->model.anim.keyframeIdx >= 132)
                            {
                                PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_None;
                            }
                        }
                        break;

                    case PlayerLowerBodyState_RunForward:
                        if (player->properties.player.runDistance >= RUN_STUMBLE_TRIGGER_DIST)
                        {
                            PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_RunLeftWallStop;
                        }
                        else
                        {
                            if (player->model.anim.status == ANIM_STATUS(HarryAnim_RunLeft, true) &&
                                player->model.anim.keyframeIdx >= 132)
                            {
                                PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_None;
                            }
                        }
                        break;

                    default:
                        if (player->properties.player.runStepSfxCount >= 5 &&
                            PLAYER_PROPS().moveSpeed >= Q12(3.125f))
                        {
                            if (player->model.anim.keyframeIdx > 128 &&
                                (!g_Player_IsRunning || !g_Player_IsSteppingLeftHold))
                            {
                                PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_RunLeftWallStop;
                            }
                        }
                        else if (player->model.anim.status == ANIM_STATUS(HarryAnim_RunLeft, true) &&
                                 player->model.anim.keyframeIdx >= 132 &&
                                 (!g_Player_IsRunning || !g_Player_IsSteppingLeftHold))
                        {
                            PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_None;
                        }
                        break;
                }
            }

            Player_MovementStateReset(player, PlayerLowerBodyState_RunLeft);
            Player_CharaRotate(4);

            PLAYER_PROPS().headingAngle = Q12_ANGLE(-90.0f);
            g_Player_HeadingAngle    = Q12_ANGLE(-90.0f);
            break;

        case PlayerLowerBodyState_QuickTurnRight:
        case PlayerLowerBodyState_AimQuickTurnRight:
            g_Player_HeadingAngle = Q12_ANGLE(0.0f);

            if (PLAYER_PROPS().moveSpeed != Q12(0.0f))
            {
                PLAYER_PROPS().moveSpeed -= TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.5f));
                if ((PLAYER_PROPS().moveSpeed >> 16) & (1 << 0))
                {
                    PLAYER_PROPS().moveSpeed = Q12(0.0f);
                }
            }

            if (player->model.controlState == 0)
            {
                PLAYER_PROPS().quickTurnHeadingAngle = player->rotation.vy;
            }

            if (player->model.stateStep == 0)
            {
                player->model.anim.status = ANIM_STATUS(HarryAnim_QuickTurnRight, false);
                player->model.stateStep++;
            }

            if (player->model.controlState == 0)
            {
                player->model.controlState++;
            }

            if (player->model.anim.status == ANIM_STATUS(HarryAnim_QuickTurnRight, true) &&
                player->model.anim.keyframeIdx >= 206)
            {
                D_800C454C = g_DeltaTime * 24;
            }
            else
            {
                D_800C454C = Q12(0.0f);
            }

            if (ABS_DIFF(PLAYER_PROPS().quickTurnHeadingAngle, player->rotation.vy) > (Q12_ANGLE(180.0f) - ((s32)(g_DeltaTime * 24) >> 4)))
            {
                if (ABS_DIFF(PLAYER_PROPS().quickTurnHeadingAngle, player->rotation.vy) < (((g_DeltaTime * 24) >> 4) + Q12_ANGLE(180.0f)))
                {
                    player->rotation.vy   = PLAYER_PROPS().quickTurnHeadingAngle + Q12_ANGLE(180.0f);
                    PLAYER_PROPS().moveSpeed = Q12(1.4f);
                    D_800C454C            = Q12(0.0f);

                    // State change.
                    if (player->model.anim.keyframeIdx >= 213)
                    {
                        if (g_Player_IsMovingForward)
                        {
                            if (g_Player_IsRunning && aimState == 0)
                            {
                                PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_RunForward;
                            }
                            else
                            {
                                PLAYER_EXTRA().lowerBodyState = aimState + PlayerLowerBodyState_WalkForward;
                            }

                            player->model.stateStep    = 0;
                            player->model.controlState = 0;
                        }
                        else if (g_Player_IsMovingBackward)
                        {
                            if (g_Player_IsRunning && aimState == 0)
                            {
                                PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_JumpBackward;
                            }
                            else
                            {
                                PLAYER_EXTRA().lowerBodyState = aimState + PlayerLowerBodyState_WalkBackward;
                            }

                            player->model.stateStep    = 0;
                            player->model.controlState = 0;
                        }
                        else if (g_Player_IsSteppingRightHold)
                        {
                            player->headingAngle += Q12_ANGLE(90.0f);

                            if (g_Player_IsRunning && aimState == 0)
                            {
                                PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_RunRight;
                            }
                            else
                            {
                                PLAYER_EXTRA().lowerBodyState = aimState + PlayerLowerBodyState_SidestepRight;
                            }

                            player->model.stateStep    = 0;
                            player->model.controlState = 0;
                        }
                        else if (g_Player_IsSteppingLeftHold)
                        {
                            player->headingAngle -= Q12_ANGLE(90.0f);

                            if (g_Player_IsRunning && aimState == 0)
                            {
                                PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_RunLeft;
                            }
                            else
                            {
                                PLAYER_EXTRA().lowerBodyState = aimState + PlayerLowerBodyState_SidestepLeft;
                            }

                            player->model.stateStep    = 0;
                            player->model.controlState = 0;
                        }
                        else if (player->model.anim.keyframeIdx >= 216)
                        {
                            PLAYER_EXTRA().lowerBodyState = aimState;
                            player->model.stateStep    = 0;
                            player->model.controlState = 0;
                        }
                    }
                }
            }

            PLAYER_PROPS().headingAngle = Q12_ANGLE(0.0f);
            g_Player_HeadingAngle    = Q12_ANGLE(0.0f);
            break;

        case PlayerLowerBodyState_QuickTurnLeft:
        case PlayerLowerBodyState_AimQuickTurnLeft:
            g_Player_HeadingAngle = Q12_ANGLE(0.0f);

            if (PLAYER_PROPS().moveSpeed != Q12(0.0f))
            {
                PLAYER_PROPS().moveSpeed -= TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.5f));
                if ((PLAYER_PROPS().moveSpeed >> 16) & (1 << 0))
                {
                    PLAYER_PROPS().moveSpeed = Q12(0.0f);
                }
            }

            if (player->model.controlState == 0)
            {
                PLAYER_PROPS().quickTurnHeadingAngle = player->rotation.vy;
            }

            if (player->model.stateStep == 0)
            {
                player->model.anim.status = ANIM_STATUS(HarryAnim_QuickTurnLeft, false);
                player->model.stateStep++;
            }

            if (player->model.controlState == 0)
            {
                player->model.controlState++;
            }

            if (player->model.anim.status == ANIM_STATUS(HarryAnim_QuickTurnLeft, true) &&
                player->model.anim.keyframeIdx >= 219)
            {
                D_800C454C = -(g_DeltaTime * 24);
            }
            else
            {
                D_800C454C = Q12(0.0f);
            }

            if (ABS_DIFF(PLAYER_PROPS().quickTurnHeadingAngle, player->rotation.vy) > (Q12_ANGLE(180.0f) - ((g_DeltaTime * 24) >> 4)))
            {
                if (ABS_DIFF(PLAYER_PROPS().quickTurnHeadingAngle, player->rotation.vy) < (((g_DeltaTime * 24) >> 4) + Q12_ANGLE(180.0f)))
                {
                    player->rotation.vy   = PLAYER_PROPS().quickTurnHeadingAngle + Q12_ANGLE(180.0f);
                    PLAYER_PROPS().moveSpeed = Q12(1.4f);
                    D_800C454C            = Q12(0.0f);

                    // State change.
                    if (player->model.anim.keyframeIdx >= 226)
                    {
                        if (g_Player_IsMovingForward)
                        {
                            if (g_Player_IsRunning && aimState == 0)
                            {
                                PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_RunForward;
                            }
                            else
                            {
                                PLAYER_EXTRA().lowerBodyState = aimState + PlayerLowerBodyState_WalkForward;
                            }

                            player->model.stateStep    = 0;
                            player->model.controlState = 0;
                        }
                        else if (g_Player_IsMovingBackward)
                        {
                            if (g_Player_IsRunning && aimState == 0)
                            {
                                PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_JumpBackward;
                            }
                            else
                            {
                                PLAYER_EXTRA().lowerBodyState = aimState + PlayerLowerBodyState_WalkBackward;
                            }

                            player->model.stateStep    = 0;
                            player->model.controlState = 0;
                        }
                        else if (g_Player_IsSteppingRightHold)
                        {
                            player->headingAngle += Q12_ANGLE(90.0f);

                            if (g_Player_IsRunning && aimState == 0)
                            {
                                PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_RunRight;
                            }
                            else
                            {
                                PLAYER_EXTRA().lowerBodyState = aimState + PlayerLowerBodyState_SidestepRight;
                            }

                            player->model.stateStep    = 0;
                            player->model.controlState = 0;
                        }
                        else if (g_Player_IsSteppingLeftHold)
                        {
                            player->headingAngle -= Q12_ANGLE(90.0f);

                            if (g_Player_IsRunning && aimState == 0)
                            {
                                PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_RunLeft;
                            }
                            else
                            {
                                PLAYER_EXTRA().lowerBodyState = aimState + PlayerLowerBodyState_SidestepLeft;
                            }

                            player->model.stateStep    = 0;
                            player->model.controlState = 0;
                        }
                        else if (player->model.anim.keyframeIdx >= 229)
                        {
                            PLAYER_EXTRA().lowerBodyState = aimState;
                            player->model.stateStep    = 0;
                            player->model.controlState = 0;
                        }
                    }
                }
            }

            PLAYER_PROPS().headingAngle = Q12_ANGLE(0.0f);
            g_Player_HeadingAngle    = Q12_ANGLE(0.0f);
            break;

        case PlayerLowerBodyState_JumpBackward:
            if (player->model.stateStep == 0)
            {
                player->model.anim.status = ANIM_STATUS(HarryAnim_JumpBackward, false);
                player->model.stateStep++;
            }

            // Jump backward.
            if ((player->model.anim.status >= ANIM_STATUS(HarryAnim_JumpBackward, false) &&
                 player->model.anim.status <= ANIM_STATUS(HarryAnim_JumpBackward, true)) &&
                player->model.anim.keyframeIdx < 245)
            {
                if (player->model.controlState == 0)
                {
                    player->fallSpeed = Q12(-2.0f);
                }

                player->model.controlState++;
                PLAYER_PROPS().moveSpeed  = Q12(2.25f);
                g_Player_PrevMoveSpeed = Q12(2.25f);
            }
            else
            {
                if (PLAYER_PROPS().moveSpeed != Q12(0.0f))
                {
                    PLAYER_PROPS().moveSpeed -= ((TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f))) * 2);
                    if ((PLAYER_PROPS().moveSpeed >> 16) & (1 << 0))
                    {
                        PLAYER_PROPS().moveSpeed = Q12(0.0f);
                    }
                }

                g_Player_PrevMoveSpeed = PLAYER_PROPS().moveSpeed;
            }

            if (player->model.anim.status == ANIM_STATUS(HarryAnim_JumpBackward, true) &&
                player->model.anim.keyframeIdx == 246)
            {
                if (player->position.vy < player->properties.player.floorHeight)
                {
                    Player_ExtraStateSet(player, extra, PlayerState_FallBackward);

                    PLAYER_PROPS().moveSpeed = Q12(1.25f);
                }
                else
                {
                    PLAYER_EXTRA().lowerBodyState = aimState;
                    player->model.stateStep    = 0;
                    player->model.controlState = 0;
                    player->fallSpeed          = Q12(0.0f);
                }
            }

            PLAYER_PROPS().headingAngle = Q12_ANGLE(180.0f);
            g_Player_HeadingAngle    = Q12_ANGLE(180.0f);
            break;

        case PlayerLowerBodyState_Stumble:
            if (PLAYER_PROPS().moveSpeed != Q12(0.0f))
            {
                PLAYER_PROPS().moveSpeed -= ((TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f))) * 2) / 3;
                if ((PLAYER_PROPS().moveSpeed >> 16) & (1 << 0))
                {
                    PLAYER_PROPS().moveSpeed = Q12(0.0f);
                }
            }

            if (D_800C45C8.field_14 <= Q12(0.5f) &&
                PLAYER_PROPS().moveSpeed != Q12(0.0f))
            {
                PLAYER_PROPS().moveSpeed -= (TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f))) * 4;
                if ((PLAYER_PROPS().moveSpeed >> 16) & (1 << 0))
                {
                    PLAYER_PROPS().moveSpeed = Q12(0.0f);
                }
            }

            if (player->model.stateStep == 0)
            {
                player->model.anim.status = ANIM_STATUS(HarryAnim_RunForwardStumble, false);
                player->model.stateStep++;
            }

            if (player->model.anim.status == ANIM_STATUS(HarryAnim_RunForwardStumble, true) &&
                player->model.anim.keyframeIdx == 179)
            {
                PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_None;
                Player_MovementStateReset(player, PlayerLowerBodyState_Stumble);
            }
            break;

        case PlayerLowerBodyState_RunLeftWallStop:
            Player_StepWallStop_MovementCancel(player, 36, 37, 335, PlayerLowerBodyState_RunLeftWallStop, Q12_ANGLE(-90.0f), aimState);
            break;

        case PlayerLowerBodyState_RunRightWallStop:
            Player_StepWallStop_MovementCancel(player, 40, 41, 364, PlayerLowerBodyState_RunRightWallStop, Q12_ANGLE(90.0f), aimState);
            break;

        case PlayerLowerBodyState_RunLeftStumble:
            if (PLAYER_PROPS().moveSpeed != Q12(0.0f))
            {
                PLAYER_PROPS().moveSpeed -= (TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f))) >> 2;
                if ((PLAYER_PROPS().moveSpeed >> 16) & (1 << 0))
                {
                    PLAYER_PROPS().moveSpeed = Q12(0.0f);
                }
            }

            if (D_800C45C8.field_14 < Q12(0.3401f))
            {
                PLAYER_PROPS().moveSpeed = Q12(0.0f);
            }

            if (player->model.stateStep == 0)
            {
                player->model.anim.status = ANIM_STATUS(HarryAnim_RunLeftStumble, false);
                player->model.stateStep++;
            }

            if (player->model.anim.status == ANIM_STATUS(HarryAnim_RunLeftStumble, true) &&
                player->model.anim.keyframeIdx == 349)
            {
                PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_None;
                Player_MovementStateReset(player, 15);
            }

            if (PLAYER_EXTRA().lowerBodyState != PlayerLowerBodyState_None)
            {
                PLAYER_PROPS().headingAngle = Q12_ANGLE(-90.0f);
                g_Player_HeadingAngle    = Q12_ANGLE(-90.0f);
                break;
            }

            PLAYER_PROPS().headingAngle = Q12_ANGLE(0.0f);
            g_Player_HeadingAngle    = Q12_ANGLE(0.0f);
            break;

        case PlayerLowerBodyState_RunRightStumble:
            if (PLAYER_PROPS().moveSpeed != Q12(0.0f))
            {
                PLAYER_PROPS().moveSpeed -= (TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f))) >> 2;
                if ((PLAYER_PROPS().moveSpeed >> 16) & (1 << 0))
                {
                    PLAYER_PROPS().moveSpeed = Q12(0.0f);
                }
            }

            if (D_800C45C8.field_14 < Q12(0.3401f))
            {
                PLAYER_PROPS().moveSpeed = Q12(0.0f);
            }

            if (player->model.stateStep == 0)
            {
                player->model.anim.status = ANIM_STATUS(HarryAnim_RunRightStumble, false);
                player->model.stateStep++;
            }

            if (player->model.anim.status == ANIM_STATUS(HarryAnim_RunRightStumble, true) &&
                player->model.anim.keyframeIdx == 378)
            {
                PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_None;
                Player_MovementStateReset(player, 16);
            }

            if (PLAYER_EXTRA().lowerBodyState == PlayerLowerBodyState_None)
            {
                PLAYER_PROPS().headingAngle = Q12_ANGLE(0.0f);
                g_Player_HeadingAngle    = Q12_ANGLE(0.0f);
                break;
            }

            PLAYER_PROPS().headingAngle = Q12_ANGLE(90.0f);
            g_Player_HeadingAngle    = Q12_ANGLE(90.0f);
            break;

        case PlayerLowerBodyState_Attack:
            // If weapon is katana.
            if (PLAYER_COMBAT().weaponAttack < WEAPON_ATTACK(EquippedWeaponId_Handgun, AttackInputType_Tap) &&
                WEAPON_ATTACK_ID_GET(PLAYER_COMBAT().weaponAttack) == EquippedWeaponId_Katana)
            {
                if (PLAYER_COMBAT().weaponAttack == WEAPON_ATTACK(EquippedWeaponId_Katana, AttackInputType_Hold))
                {
                    if (PLAYER_PROPS().moveSpeed == Q12(0.0f) &&
                        (extra->model.anim.keyframeIdx >= D_800C44F0[D_800AF220].startKeyframeIdx + 7))
                    {
                        PLAYER_PROPS().moveSpeed = Q12(5.0f);
                        g_Player_HeadingAngle = Q12_ANGLE(0.0f);
                    }
                }
                else if (player->model.stateStep == 0 && !g_Player_IsAttacking)
                {
                    PLAYER_PROPS().moveSpeed = Q12(5.0f);
                    g_Player_HeadingAngle = Q12_ANGLE(0.0f);
                }
            }

            if (PLAYER_COMBAT().weaponAttack < WEAPON_ATTACK(EquippedWeaponId_Handgun, AttackInputType_Tap) &&
                WEAPON_ATTACK_ID_GET(PLAYER_COMBAT().weaponAttack) == EquippedWeaponId_Katana)
            {
                if (PLAYER_PROPS().moveSpeed != Q12(0.0f))
                {
                    PLAYER_PROPS().moveSpeed -= TIMESTEP_SCALE_30_FPS(g_DeltaTime, 0x444);
                    if ((PLAYER_PROPS().moveSpeed >> 16) & (1 << 0))
                    {
                        PLAYER_PROPS().moveSpeed = Q12(0.0f);
                    }
                }
            }
            else
            {
                if (PLAYER_PROPS().moveSpeed != Q12(0.0f))
                {
                    PLAYER_PROPS().moveSpeed -= TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f));
                    if ((PLAYER_PROPS().moveSpeed >> 16) & (1 << 0))
                    {
                        PLAYER_PROPS().moveSpeed = Q12(0.0f);
                    }
                }
            }

            if (g_SysWork.targetNpcIdx == NO_VALUE ||
                PLAYER_COMBAT().weaponAttack < WEAPON_ATTACK(EquippedWeaponId_Handgun, AttackInputType_Tap))
            {
                if (PLAYER_COMBAT().weaponAttack >= WEAPON_ATTACK(EquippedWeaponId_Handgun, AttackInputType_Tap))
                {
                    if (PLAYER_PROPS().flags & PlayerFlag_Unk11)
                    {
                        if (player->model.stateStep == 0)
                        {
                            player->model.anim.status = g_Player_EquippedWeaponInfo.animAttack - 12;
                            player->model.stateStep++;
                        }
                    }
                    else
                    {
                        if (player->model.stateStep == 0)
                        {
                            player->model.anim.status = g_Player_EquippedWeaponInfo.animAttack;
                            player->model.stateStep++;
                        }
                    }
                }
                else if (PLAYER_PROPS().flags & PlayerFlag_Unk10)
                {
                    if (PLAYER_COMBAT().weaponAttack == WEAPON_ATTACK(EquippedWeaponId_Chainsaw,  AttackInputType_Tap) ||
                        PLAYER_COMBAT().weaponAttack == WEAPON_ATTACK(EquippedWeaponId_RockDrill, AttackInputType_Tap))
                    {
                        if (player->model.stateStep == 0)
                        {
                            player->model.anim.status = g_Player_EquippedWeaponInfo.animAttack;
                            player->model.stateStep++;
                        }
                    }
                    else if (player->model.stateStep == 0)
                    {
                        player->model.anim.status = ANIM_STATUS(HarryAnim_HandgunAim, false);
                        player->model.stateStep++;
                    }

                    if (player->model.anim.keyframeIdx == D_800C44F0[0].endKeyframeIdx ||
                        player->model.anim.keyframeIdx == D_800C44F0[5].endKeyframeIdx)
                    {
                        player->model.anim.status      = extra->model.anim.status;
                        player->model.anim.keyframeIdx = extra->model.anim.keyframeIdx;
                        player->model.anim.time        = extra->model.anim.time;
                        player->model.stateStep++;
                    }
                }
                else if (PLAYER_COMBAT().weaponAttack == WEAPON_ATTACK(EquippedWeaponId_RockDrill, AttackInputType_Tap))
                {
                    if (g_Player_RockDrill_AttackType == RockDrillAttackType_Up)
                    {
                        if (player->model.stateStep == 0)
                        {
                            player->model.anim.status = g_Player_EquippedWeaponInfo.animAttack + 2;
                            player->model.stateStep++;
                        }
                    }
                    else if (g_Player_RockDrill_AttackType == RockDrillAttackType_Down)
                    {
                        if (player->model.stateStep == 0)
                        {
                            player->model.anim.status = g_Player_EquippedWeaponInfo.animAttack + 4;
                            player->model.stateStep++;
                        }
                    }
                    else
                    {
                        if (player->model.stateStep == 0)
                        {
                            player->model.anim.status = g_Player_EquippedWeaponInfo.animAttack;
                            player->model.stateStep++;
                        }
                    }
                }
                else
                {
                    if (extra->model.anim.status == ANIM_STATUS(HarryAnim_Unk30, true))
                    {
                        player->model.anim.status      = extra->model.anim.status;
                        player->model.anim.keyframeIdx = extra->model.anim.keyframeIdx;
                        player->model.anim.time        = extra->model.anim.time;
                        player->model.stateStep++;
                    }
                    else if (g_Player_IsAttacking || extra->model.anim.status == ANIM_STATUS(HarryAnim_Unk29, true))
                    {
                        if (player->model.stateStep == 0)
                        {
                            player->model.anim.status = g_Player_EquippedWeaponInfo.animAttack - 4;
                            player->model.stateStep++;
                        }
                    }
                    else if (g_Player_IsShooting || extra->model.anim.status == ANIM_STATUS(HarryAnim_HandgunRecoil, true))
                    {
                        if (player->model.stateStep == 0)
                        {
                            player->model.anim.status = g_Player_EquippedWeaponInfo.animAttack;
                            player->model.stateStep++;
                        }
                    }
                }
            }
            else if (player->model.stateStep == 0)
            {
                player->model.anim.status = g_Player_EquippedWeaponInfo.animAttackHold - 1;
                player->model.stateStep++;
            }

            if (PLAYER_COMBAT().weaponAttack >= WEAPON_ATTACK(EquippedWeaponId_Handgun, AttackInputType_Tap) ||
                (WEAPON_ATTACK_ID_GET(PLAYER_COMBAT().weaponAttack) != EquippedWeaponId_SteelPipe &&
                 WEAPON_ATTACK_ID_GET(PLAYER_COMBAT().weaponAttack) != EquippedWeaponId_Hammer    &&
                 WEAPON_ATTACK_ID_GET(PLAYER_COMBAT().weaponAttack) != EquippedWeaponId_RockDrill &&
                 WEAPON_ATTACK_ID_GET(PLAYER_COMBAT().weaponAttack) != EquippedWeaponId_Katana))
            {
                if (ANIM_STATUS_IS_ACTIVE(player->model.anim.status) &&
                    ANIM_STATUS_IS_ACTIVE(extra->model.anim.status)  &&
                    (player->model.anim.status >= ANIM_STATUS(HarryAnim_Unk29, false) ||
                     player->model.anim.keyframeIdx == D_800C44F0[0].endKeyframeIdx))
                {
                    if (!g_Player_IsMovingForward)
                    {
                        if (g_Player_IsMovingBackward)
                        {
                            if (PLAYER_PROPS().moveSpeed == Q12(0.0f))
                            {
                                PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_AimWalkBackward;
                            }
                        }
                        else if (PLAYER_EXTRA().state != PlayerState_Combat)
                        {
                            if (!g_Player_IsSteppingRightHold)
                            {
                                if (g_Player_IsSteppingLeftHold)
                                {
                                    PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_AimSidestepLeft;
                                }
                            }
                            else
                            {
                                PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_AimSidestepRight;
                            }
                        }
                    }
                    else
                    {
                        PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_AimWalkForward;
                    }

                    if (PLAYER_EXTRA().lowerBodyState != PlayerLowerBodyState_Attack)
                    {
                        Player_MovementStateReset(player, PlayerLowerBodyState_Aim);
                        break;
                    }
                }
            }
            break;

        case PlayerLowerBodyState_Reload:
            if (player->model.stateStep == 0)
            {
                player->model.anim.status = ANIM_STATUS(HarryAnim_HandgunRecoil, false);
                player->model.stateStep++;
            }

            if (ANIM_STATUS_IS_ACTIVE(player->model.anim.status) &&
                ANIM_STATUS_IS_ACTIVE(extra->model.anim.status)  &&
                (player->model.anim.status >= ANIM_STATUS(HarryAnim_Unk29, false) ||
                 player->model.anim.keyframeIdx == D_800C44F0[0].endKeyframeIdx))
            {
                if (g_Player_IsMovingForward)
                {
                    PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_AimWalkForward;
                }
                else if (g_Player_IsMovingBackward)
                {
                    if (PLAYER_PROPS().moveSpeed == Q12(0.0f))
                    {
                        PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_AimWalkBackward;
                    }
                }
                else if (PLAYER_EXTRA().state != PlayerState_Combat)
                {
                    if (g_Player_IsSteppingRightHold)
                    {
                        PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_AimSidestepRight;
                    }
                    else if (g_Player_IsSteppingLeftHold)
                    {
                        PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_AimSidestepLeft;
                    }
                }

                if (PLAYER_EXTRA().lowerBodyState != PlayerLowerBodyState_Reload)
                {
                    Player_MovementStateReset(player, PlayerLowerBodyState_Aim);
                }
            }
            break;
    }

    func_8007B924(player, extra);

    #undef stillAnim
}

void func_8007B924(s_SubCharacter* player, s_PlayerExtra* extra) // 0x8007B924
{
    e_SfxId sfxId;
    s8      pitch0;
    s8      pitch1;

    Player_FootstepSfxGet(g_Player_CollisionResult.surface.floorType, &sfxId, &pitch0, &pitch1);

    // This entire conditional is the reason why movement stop working when removing this function call.
    if (PLAYER_EXTRA().lowerBodyState != PlayerLowerBodyState_JumpBackward &&
        PLAYER_EXTRA().lowerBodyState != PlayerLowerBodyState_Reload)
    {
        g_Player_PrevMoveSpeed = PLAYER_PROPS().moveSpeed;
    }

    switch (PLAYER_EXTRA().lowerBodyState)
    {
        case PlayerLowerBodyState_RunForward:
        case PlayerLowerBodyState_RunRight:
        case PlayerLowerBodyState_RunLeft:
            if (ANIM_STATUS_IS_ACTIVE(player->model.anim.status) &&
                player->model.anim.status >= ANIM_STATUS(HarryAnim_RunForward, true))
            {
                player->properties.player.exhaustionTimer += g_DeltaTime;
            }
            break;

        case PlayerLowerBodyState_None:
        case PlayerLowerBodyState_RunForwardWallStop:
        case PlayerLowerBodyState_Stumble:
        case PlayerLowerBodyState_RunLeftStumble:
        case PlayerLowerBodyState_RunRightStumble:
        case PlayerLowerBodyState_Aim:
            player->properties.player.exhaustionTimer -= g_DeltaTime * 2;
            break;

        default:
            player->properties.player.exhaustionTimer -= g_DeltaTime;
            break;
    }

    player->properties.player.exhaustionTimer = CLAMP(player->properties.player.exhaustionTimer, Q12(0.0f), Q12(35.0f));

    // Check if player has >=30% or <10% of health to determine exertion level.
    if (player->model.anim.status == ANIM_STATUS(HarryAnim_IdleExhausted, true))
    {
        if (player->properties.player.exhaustionTimer < Q12(10.0f) &&
            player->health >= Q12(30.0f))
        {
            player->model.stateStep    = 0;
            player->model.controlState = 0;
            extra->model.stateStep     = 0;
            extra->model.controlState  = 0;
        }
    }

    // Plays movement sounds.
    switch (PLAYER_EXTRA().lowerBodyState)
    {
        case PlayerLowerBodyState_None:
        case PlayerLowerBodyState_Aim:
            // Turn right.
            if (PLAYER_EXTRA().upperBodyState == PlayerUpperBodyState_TurnRight)
            {
                Player_FootstepSfxPlay(ANIM_STATUS(HarryAnim_TurnRight, true), player, 204, 200, sfxId, pitch0);
            }
            // Turn left.
            else if (PLAYER_EXTRA().upperBodyState == PlayerUpperBodyState_TurnLeft)
            {
                Player_FootstepSfxPlay(ANIM_STATUS(HarryAnim_TurnLeft, true), player, 187, 191, sfxId, pitch0);
            }

            if ((PLAYER_PROPS().flags & PlayerFlag_Moving) &&
                ((player->model.anim.status >= ANIM_STATUS(HarryAnim_Idle, true) &&
                  player->model.anim.status <= ANIM_STATUS(HarryAnim_IdleExhausted, false)) ||
                 player->model.anim.status == ANIM_STATUS(HarryAnim_HandgunAim, true)))
            {
                Sfx_WithPitchPlay(sfxId, &player->position, Q8_CLAMPED(0.095f), pitch0);

                player->properties.player.field_10C                        = pitch0 + 0x10;
                PLAYER_PROPS().flags &= ~PlayerFlag_Moving;
            }

            if (player->model.anim.keyframeIdx == 246 &&
                !(PLAYER_PROPS().flags & PlayerFlag_Unk5))
            {
                Sfx_WithPitchPlay(sfxId, &player->position, Q8(0.5f), pitch1);

                player->properties.player.field_10C                       = pitch1 + 0x20;
                PLAYER_PROPS().flags |= PlayerFlag_Unk5;
            }
            break;

        default:
            break;

        case PlayerLowerBodyState_WalkBackward:
        case PlayerLowerBodyState_AimWalkBackward:
            Player_FootstepSfxPlay(ANIM_STATUS(HarryAnim_WalkBackward, true), player, 52, 63, sfxId, pitch0);
            PLAYER_PROPS().flags |= PlayerFlag_Moving;
            break;

        case PlayerLowerBodyState_AimWalkForward:
        case PlayerLowerBodyState_WalkForward:
            Player_FootstepSfxPlay(ANIM_STATUS(HarryAnim_WalkForward, true), player, 18, 6, sfxId, pitch0);
            PLAYER_PROPS().flags |= PlayerFlag_Moving;
            break;

        case PlayerLowerBodyState_RunForward:
            if (Player_FootstepSfxPlay(ANIM_STATUS(HarryAnim_RunForward, true), player, 31, 41, sfxId, pitch1))
            {
                player->properties.player.runStepSfxCount++;
            }

            PLAYER_PROPS().flags |= PlayerFlag_Moving;
            break;

        case PlayerLowerBodyState_SidestepRight:
            Player_FootstepSfxPlay(ANIM_STATUS(HarryAnim_SidestepRight, true), player, 118, 108, sfxId, pitch0);
            PLAYER_PROPS().flags |= PlayerFlag_Moving;
            break;

        case PlayerLowerBodyState_SidestepLeft:
            Player_FootstepSfxPlay(ANIM_STATUS(HarryAnim_SidestepLeft, true), player, 93, 83, sfxId, pitch0);
            PLAYER_PROPS().flags |= PlayerFlag_Moving;
            break;

        case PlayerLowerBodyState_RunRight:
            if (Player_FootstepSfxPlay(ANIM_STATUS(HarryAnim_RunRight, true), player, 145, 139, sfxId, pitch1))
            {
                player->properties.player.runStepSfxCount++;
            }

            PLAYER_PROPS().flags |= PlayerFlag_Moving;
            break;

        case PlayerLowerBodyState_RunLeft:
            if (Player_FootstepSfxPlay(ANIM_STATUS(HarryAnim_RunLeft, true), player, 131, 125, sfxId, pitch1))
            {
                player->properties.player.runStepSfxCount++;
            }

            PLAYER_PROPS().flags |= PlayerFlag_Moving;
            break;

        case PlayerLowerBodyState_RunForwardWallStop:
            if (PLAYER_PROPS().flags & PlayerFlag_WallStopRight)
            {
                if (player->model.anim.keyframeIdx < 152)
                {
                    Player_FootstepSfxPlay(ANIM_STATUS(HarryAnim_RunForwardWallStopRight, true), player, 151, 154, sfxId, pitch1);
                }
                else
                {
                    Player_FootstepSfxPlay(ANIM_STATUS(HarryAnim_RunForwardWallStopRight, true), player, 156, 154, sfxId, pitch1);
                }
            }
            else
            {
                if (player->model.anim.keyframeIdx < 162)
                {
                    Player_FootstepSfxPlay(ANIM_STATUS(HarryAnim_RunForwardWallStopLeft, true), player, 164, 161, sfxId, pitch1);
                }
                else
                {
                    Player_FootstepSfxPlay(ANIM_STATUS(HarryAnim_RunForwardWallStopLeft, true), player, 164, 166, sfxId, pitch1);
                }
            }

            PLAYER_PROPS().flags &= ~PlayerFlag_Moving;
            break;

        case PlayerLowerBodyState_RunLeftWallStop:
            if (player->model.anim.keyframeIdx < 323)
            {
                Player_FootstepSfxPlay(ANIM_STATUS(HarryAnim_RunLeftWallStop, true), player, 322, 324, sfxId, pitch1);
            }
            else
            {
                Player_FootstepSfxPlay(ANIM_STATUS(HarryAnim_RunLeftWallStop, true), player, 327, 324, sfxId, pitch1);
            }

            PLAYER_PROPS().flags &= ~PlayerFlag_Moving;
            break;

        case PlayerLowerBodyState_RunRightWallStop:
            if (player->model.anim.keyframeIdx < 352)
            {
                Player_FootstepSfxPlay(ANIM_STATUS(HarryAnim_RunRightWallStop, true), player, 353, 351, sfxId, pitch1);
            }
            else
            {
                Player_FootstepSfxPlay(ANIM_STATUS(HarryAnim_RunRightWallStop, true), player, 353, 356, sfxId, pitch1);
            }

            PLAYER_PROPS().flags &= ~PlayerFlag_Moving;
            break;

        case PlayerLowerBodyState_Stumble:
            if (player->model.anim.keyframeIdx < 172)
            {
                Player_FootstepSfxPlay(ANIM_STATUS(HarryAnim_RunForwardStumble, true), player, 171, 174, sfxId, pitch1);
            }
            else
            {
                Player_FootstepSfxPlay(ANIM_STATUS(HarryAnim_RunForwardStumble, true), player, 176, 174, sfxId, pitch1);
            }

            PLAYER_PROPS().flags &= ~PlayerFlag_Moving;
            break;

        case PlayerLowerBodyState_RunLeftStumble:
            if (player->model.anim.keyframeIdx < 338)
            {
                Player_FootstepSfxPlay(ANIM_STATUS(HarryAnim_RunLeftStumble, true), player, 337, 341, sfxId, pitch0);
            }
            else if (player->model.anim.keyframeIdx < 344)
            {
                Player_FootstepSfxPlay(ANIM_STATUS(HarryAnim_RunLeftStumble, true), player, 343, 341, sfxId, pitch0);
            }
            else
            {
                Player_FootstepSfxPlay(ANIM_STATUS(HarryAnim_RunLeftStumble, true), player, 356, 346, sfxId, pitch1);
            }

            PLAYER_PROPS().flags &= ~PlayerFlag_Moving;
            break;

        case PlayerLowerBodyState_RunRightStumble:
            if (player->model.anim.keyframeIdx < 367)
            {
                Player_FootstepSfxPlay(ANIM_STATUS(HarryAnim_RunRightStumble, true), player, 366, 370, sfxId, pitch0);
            }
            else if (player->model.anim.keyframeIdx < 373)
            {
                Player_FootstepSfxPlay(ANIM_STATUS(HarryAnim_RunRightStumble, true), player, 372, 370, sfxId, pitch0);
            }
            else
            {
                Player_FootstepSfxPlay(ANIM_STATUS(HarryAnim_RunRightStumble, true), player, 385, 375, sfxId, pitch1);
            }

            PLAYER_PROPS().flags &= ~PlayerFlag_Moving;
            break;

        case PlayerLowerBodyState_QuickTurnLeft:
        case PlayerLowerBodyState_AimQuickTurnLeft:
            Player_FootstepSfxPlay(ANIM_STATUS(HarryAnim_QuickTurnLeft, true), player, 222, 224, sfxId, pitch0);
            PLAYER_PROPS().flags &= ~PlayerFlag_Moving;
            break;

        case PlayerLowerBodyState_QuickTurnRight:
        case PlayerLowerBodyState_AimQuickTurnRight:
            Player_FootstepSfxPlay(ANIM_STATUS(HarryAnim_QuickTurnRight, true), player, 209, 211, sfxId, pitch0);
            PLAYER_PROPS().flags &= ~PlayerFlag_Moving;
            break;

        case PlayerLowerBodyState_JumpBackward:
        case PlayerLowerBodyState_Unk31:
            if (player->model.anim.keyframeIdx < 243)
            {
                PLAYER_PROPS().flags &= ~PlayerFlag_Unk5;
            }

            if (player->position.vy == g_Player_CollisionResult.surface.floorHeight)
            {
                Player_FootstepSfxPlay(ANIM_STATUS(HarryAnim_JumpBackward, true), player, 243, 245, sfxId, pitch1);
            }

            PLAYER_PROPS().flags &= ~PlayerFlag_Moving;
            break;
    }
}

void Player_PositionUpdate(s_SubCharacter* player, s_PlayerExtra* extra, GsCOORDINATE2* boneCoords)
{
    s_CollisionSurface surface;
    VECTOR3            offset;
    VECTOR3            sp30; // Q19.12
    VECTOR3            sp40; // Q19.12
    q19_12             moveOffsetX;
    q19_12             moveOffsetZ;
    q3_12              moveOffsetAlphaX;
    q3_12              moveOffsetAlphaZ;
    q3_12              adjMoveOffsetX;
    q3_12              adjMoveOffsetZ;
    q19_12             adjMoveSpeed;
    s32                temp_s2_2;
    s32                temp_s3_2;
    q19_12             headingAngle;
    q19_12             posY;
    u32                temp;

    g_Player_PrevPosition = player->position;

    // TODO: Similar block found in `sharedFunc_800D1C38_0_s00`. Maybe an inline?

    Collision_SurfaceGet(&surface, player->position.vx, player->position.vz);

    // Compute speed displacement.
    moveOffsetX = Q12_MULT(player->moveSpeed, Math_Sin(player->headingAngle));
    moveOffsetZ = Q12_MULT(player->moveSpeed, Math_Cos(player->headingAngle));

    // Compute displacement alpha from floor slope.
    moveOffsetAlphaX = Math_Cos(DIV_FAST(ABS(surface.slopeGradientX), 8));
    moveOffsetAlphaZ = Math_Cos(DIV_FAST(ABS(surface.slopeGradientZ), 8));

    // Compute adjusted displacement.
    adjMoveOffsetX = Q12_MULT(Q12_MULT(moveOffsetX, moveOffsetAlphaX), moveOffsetAlphaX);
    adjMoveOffsetZ = Q12_MULT(Q12_MULT(moveOffsetZ, moveOffsetAlphaZ), moveOffsetAlphaZ);

    // Adjust speed according to displacement.
    if (player->moveSpeed >= Q12(0.0f))
    {
        player->moveSpeed = Math_Vector2MagCalc(adjMoveOffsetX, adjMoveOffsetZ);
    }
    else
    {
        player->moveSpeed = -Math_Vector2MagCalc(adjMoveOffsetX, adjMoveOffsetZ);
    }
    adjMoveSpeed = Q12_MULT_PRECISE(player->moveSpeed, g_DeltaTime);

    headingAngle = player->headingAngle;
    temp      = adjMoveSpeed + Q12(8.0f) - 1;
    temp_s2_2 = (temp > (Q12(16.0f) - 2)) * 4;
    temp_s3_2 = temp_s2_2 >> 1;

    offset.vx = Q12_MULT_PRECISE(adjMoveSpeed >> temp_s3_2, Math_Sin(headingAngle) >> temp_s3_2);
    offset.vx <<= temp_s2_2;

    offset.vz = Q12_MULT_PRECISE(adjMoveSpeed >> temp_s3_2, Math_Cos(headingAngle) >> temp_s3_2);
    offset.vz <<= temp_s2_2;

    offset.vy = Q12_MULT_PRECISE(player->fallSpeed, g_DeltaTime);

    // Special condition for school boss.
    if (g_SavegamePtr->mapIdx == MapIdx_MAP1_S05)
    {
        offset.vx += D_800C45B0.vx;
        sp30.vx    = offset.vx;
        offset.vz += D_800C45B0.vz;
        sp30.vz    = offset.vz;
    }

    Collision_WallDetect(&g_Player_CollisionResult, &offset, player);

    // Special condition for school boss.
    if (g_SavegamePtr->mapIdx == MapIdx_MAP1_S05)
    {
        if (D_800C45B0.vx != 0 &&
            (DIFF_SIGN(sp30.vx, g_Player_CollisionResult.offset.vx) ||
             abs(sp30.vx) >= ABS(g_Player_CollisionResult.offset.vx)))
        {
            sp40.vx = sp30.vx - g_Player_CollisionResult.offset.vx;
        }
        else
        {
            sp40.vx = Q12(0.0f);
        }

        if (D_800C45B0.vz != 0 &&
            (DIFF_SIGN(sp30.vz, g_Player_CollisionResult.offset.vz) ||
             abs(sp30.vz) >= ABS(g_Player_CollisionResult.offset.vz)))
        {
            sp40.vz = sp30.vz - g_Player_CollisionResult.offset.vz;
        }
        else
        {
            sp40.vz = Q12(0.0f);
        }

        g_MapOverlayHdr.splitHeadMovementOffsetSet(-sp40.vx, -sp40.vz);
    }

    player->position.vx += g_Player_CollisionResult.offset.vx;
    player->position.vy += g_Player_CollisionResult.offset.vy;
    player->position.vz += g_Player_CollisionResult.offset.vz;

    if (PLAYER_EXTRA().upperBodyState == PlayerUpperBodyState_RunForward ||
        PLAYER_EXTRA().upperBodyState == PlayerUpperBodyState_RunRight   ||
        PLAYER_EXTRA().upperBodyState == PlayerUpperBodyState_RunLeft)
    {
        player->properties.player.runDistance += Math_Vector3MagCalc(g_Player_CollisionResult.offset.vx,
                                                                     g_Player_CollisionResult.offset.vy,
                                                                     g_Player_CollisionResult.offset.vz);
    }
    else
    {
        player->properties.player.runDistance = Q12(0.0f);
    }

    if (g_SavegamePtr->mapIdx     == MapIdx_MAP1_S00 &&
        g_SavegamePtr->mapRoomIdx == 13)
    {
        g_Player_CollisionResult.surface.floorHeight = Q12(0.0f);
    }

    // Set collision floor height.
    if (g_Player_CollisionResult.surface.floorType == FloorType_Default)
    {
        g_Player_CollisionResult.surface.floorHeight = player->properties.player.floorHeight;
    }

    // Warp to floor if submerged.
    if (player->position.vy > g_Player_CollisionResult.surface.floorHeight)
    {
        player->position.vy = g_Player_CollisionResult.surface.floorHeight;
        player->fallSpeed   = Q12(0.0f);
    }

    adjMoveOffsetZ = Q12_ANGLE_NORM_U(ratan2(player->position.vx - g_Player_PrevPosition.vx,
                                             player->position.vz - g_Player_PrevPosition.vz) +
                                      Q12_ANGLE(360.0f));

    if (!(PLAYER_EXTRA().state >= PlayerState_FallForward &&
          PLAYER_EXTRA().state <  PlayerState_KickEnemy))
    {
        if (!g_Player_IsInWalkToRunTransition)
        {
            posY = player->position.vy;
            if ((g_Player_CollisionResult.surface.floorHeight - posY) >= Q12(0.65f))
            {
                if (ABS_DIFF(player->rotation.vy, adjMoveOffsetZ) >= Q12_ANGLE(90.0f) &&
                    ABS_DIFF(player->rotation.vy, adjMoveOffsetZ) <  Q12_ANGLE(270.0f))
                {
                    if (PLAYER_EXTRA().lowerBodyState != PlayerLowerBodyState_JumpBackward)
                    {
                        Player_ExtraStateSet(player, extra, PlayerState_FallBackward);
                    }
                }
                else
                {
                    Player_ExtraStateSet(player, extra, PlayerState_FallForward);
                }

                PLAYER_COMBAT().isAiming = false;
            }
        }
    }

    // Set model position.
    player->properties.player.floorHeight = g_Player_CollisionResult.surface.floorHeight;
    boneCoords[HarryBone_Root].coord.t[0]  = Q12_TO_Q8(player->position.vx);
    boneCoords[HarryBone_Root].coord.t[1]  = Q12_TO_Q8(player->position.vy);
    boneCoords[HarryBone_Root].coord.t[2]  = Q12_TO_Q8(player->position.vz);
}

void Player_ReceiveDamage(s_SubCharacter* player, s_PlayerExtra* extra) // 0x8007C800
{
    q3_12 headingAngle;
    q4_12 enemyRotY;
    s32   i;
    s32   sfxId;
    s32   angleState;

    #define attacker NPC_CHARA(0)

    // Set damage SFX according to something.
    sfxId = Sfx_Unk1326;
    if (player->attackReceived != NO_VALUE)
    {
        switch (D_800AD4C8[player->attackReceived].field_11)
        {
            case 2:
                sfxId = Sfx_Unk1327;
                break;

            case 4:
                sfxId = Sfx_Unk1328;
                break;

            case 5:
                sfxId = Sfx_Unk1329;
                break;

            case 0:
                break;
        }
    }

    if (g_Player_DisableControl || g_Player_DisableDamage)
    {
        player->damage.amount = Q12(0.0f);
        return;
    }

    switch (PLAYER_EXTRA().state)
    {
        case PlayerState_FallForward:
        case PlayerState_FallBackward:
        case PlayerState_EnemyReleasePinnedFront:
        case PlayerState_EnemyReleasePinnedBack:
        case PlayerState_DamageThrownFront:
        case PlayerState_DamageThrownBack:
        case PlayerState_GetUpFront:
        case PlayerState_GetUpBack:
            break;

        case PlayerState_Death:
        case PlayerState_InstantDeath:
            return;

        // Grab states.
        case PlayerState_EnemyGrabTorsoFront:
        case PlayerState_EnemyGrabTorsoBack:
        case PlayerState_EnemyGrabLegsFront:
        case PlayerState_EnemyGrabLegsBack:
        case PlayerState_EnemyGrabNeckFront:
        case PlayerState_EnemyGrabNeckBack:
        case PlayerState_EnemyGrabPinnedFront:
        case PlayerState_EnemyGrabPinnedBack:
        case PlayerState_OnFloorFront:
        case PlayerState_OnFloorBehind:
            // Related to enemy grabbing.
            if (player->damage.amount != Q12(0.0f) && !(PLAYER_PROPS().flags & PlayerFlag_DamageReceived))
            {
                PLAYER_PROPS().flags |= PlayerFlag_DamageReceived;
                Sfx_WithFlagsPlay(sfxId, &player->position, Q8(1.0f / 8.0f), SfxFlag_None);
                player->properties.player.field_10C = 64;
            }

            if (player->damage.amount == Q12(0.0f))
            {
                PLAYER_PROPS().flags &= ~PlayerFlag_DamageReceived;
            }

            func_80089494();
            break;

        case PlayerState_Unk7:
        case PlayerState_EnemyReleaseUpperFront:
        case PlayerState_Unk17:
        case PlayerState_Unk18:
        case PlayerState_DamageHead:
        case PlayerState_EnemyReleaseUpperBack:
        case PlayerState_EnemyReleaseLowerFront:
        case PlayerState_EnemyReleaseLowerBack:
        case PlayerState_DamageTorsoBack:
        case PlayerState_DamageTorsoFront:
        case PlayerState_DamageTorsoRight:
        case PlayerState_DamageTorsoLeft:
        case PlayerState_DamageFeetFront:
        case PlayerState_DamageFeetBack:
        case PlayerState_Unk35:
        case PlayerState_EnemyGrabPinnedFrontStart:
        case PlayerState_EnemyGrabPinnedBackStart:
            player->damage.position.vz = Q12(0.0f);
            player->damage.position.vy = Q12(0.0f);
            player->damage.position.vx = Q12(0.0f);

            if (player->attackReceived == WEAPON_ATTACK(EquippedWeaponId_Unk37, AttackInputType_Hold))
            {
                PLAYER_CHARA().collision.cylinder.innerRadius = Q12(0.0f);
                Player_ExtraStateSet(player, extra, PlayerState_InstantDeath);
                return;
            }

            if (player->attackReceived == WEAPON_ATTACK(EquippedWeaponId_Unk68, AttackInputType_Tap) ||
                player->attackReceived == WEAPON_ATTACK(EquippedWeaponId_Unk69, AttackInputType_Tap))
            {
                player->damage.amount = Q12(0.0f);
            }
            break;

        default:
            if (g_Player_IsInWalkToRunTransition)
            {
                g_Player_PrevAttackReceived = player->attackReceived;
                return;
            }

            if (g_Player_PrevAttackReceived != NO_VALUE)
            {
                player->attackReceived = g_Player_PrevAttackReceived;
                g_Player_PrevAttackReceived = NO_VALUE;
            }

            if (player->attackReceived <= 0)
            {
                break;
            }

            g_SysWork.targetNpcIdx    = NO_VALUE;
            PLAYER_COMBAT().weaponAttack = (g_SavegamePtr->equippedWeapon == InvItemId_Unequipped) ? NO_VALUE :
                                                                                                  (g_SavegamePtr->equippedWeapon - InvItemId_KitchenKnife);

            if (PLAYER_COMBAT().weaponAttack == WEAPON_ATTACK(EquippedWeaponId_RockDrill, AttackInputType_Tap))
            {
                func_8004C564(2, 3);
            }

            if (PLAYER_EXTRA().state >= PlayerState_FallForward &&
                PLAYER_EXTRA().state <  PlayerState_Unk7)
            {
                Player_CollisionReset();
            }

            enemyRotY = NPC_CHARA(player->field_40).rotation.vy;
            if (player->attackReceived == WEAPON_ATTACK(EquippedWeaponId_HandgunBullets, AttackInputType_Tap) ||
                player->attackReceived == WEAPON_ATTACK(EquippedWeaponId_RifleShells, AttackInputType_Tap))
            {
                enemyRotY -= Q12_ANGLE(90.0f);
            }
            else if (player->attackReceived == WEAPON_ATTACK(EquippedWeaponId_Unk69, AttackInputType_Tap))
            {
                enemyRotY = Q12_ANGLE(90.0f);
            }
            else if (player->attackReceived == WEAPON_ATTACK(EquippedWeaponId_Unk68, AttackInputType_Tap))
            {
                enemyRotY = player->damage.position.vy;
            }

            enemyRotY = Q12_ANGLE_NORM_U((enemyRotY - player->rotation.vy) + Q12_ANGLE(360.0f));

            switch (player->attackReceived)
            {
                case 67:
                    Player_ExtraStateSet(player, extra, PlayerState_Unk7);
                    break;

                case 63:
                    PLAYER_PROPS().moveSpeed = Q12(1.5f);
                    Math_ShortestAngleGet(player->rotation.vy, attacker.rotation.vy, &headingAngle);
                    g_Player_HeadingAngle = headingAngle;

                    if (enemyRotY >= Q12_ANGLE(90.0f) && enemyRotY < Q12_ANGLE(270.0f))
                    {
                        Player_ExtraStateSet(player, extra, PlayerState_DamageThrownFront);
                    }
                    else
                    {
                        Player_ExtraStateSet(player, extra, PlayerState_DamageThrownBack);
                    }
                    break;

                case 60:
                case 62:
                    player->damage.amount = Q12(10.0f);
                    Player_ExtraStateSet(player, extra, PlayerState_DamageHead);
                    break;

                case 41:
                case 42:
                    Player_ExtraStateSet(player, extra, PlayerState_DamageHead);
                    break;

                case 49: // Leg grab.
                case 54: // Romper grab.
                case 56: // Torso grab.
                case 66:
                    if (enemyRotY >= Q12_ANGLE(90.0f) &&
                        enemyRotY <  Q12_ANGLE(270.0f))
                    {
                        g_SysWork.npcIdxs[0] = player->field_40;

                        switch (player->attackReceived)
                        {
                            case 54:
                                Player_ExtraStateSet(player, extra, PlayerState_EnemyGrabPinnedFrontStart);
                                break;

                            case 45:
                            case 56:
                                Player_ExtraStateSet(player, extra, PlayerState_EnemyGrabTorsoFront);
                                break;

                            case 49:
                                Player_ExtraStateSet(player, extra, PlayerState_EnemyGrabLegsFront);
                                break;

                            case 66:
                                Player_ExtraStateSet(player, extra, PlayerState_EnemyGrabNeckFront);
                                break;

                        }
                    }
                    else
                    {
                        g_SysWork.npcIdxs[1] = player->field_40;

                        switch (player->attackReceived)
                        {
                            case 54:
                                Player_ExtraStateSet(player, extra, PlayerState_EnemyGrabPinnedBackStart);
                                break;

                            case 45:
                            case 56:
                                Player_ExtraStateSet(player, extra, PlayerState_EnemyGrabTorsoBack);
                                break;

                            case 49:
                                Player_ExtraStateSet(player, extra, PlayerState_EnemyGrabLegsBack);
                                break;

                            case 66:
                                Player_ExtraStateSet(player, extra, PlayerState_EnemyGrabNeckBack);
                                break;
                        }
                    }
                    break;

                case 47:
                    player->health                     = NO_VALUE;
                    player->collision.cylinder.innerRadius = Q12(0.0f);
                    Player_ExtraStateSet(player, extra, PlayerState_InstantDeath);
                    return;

                case 69:
                    PLAYER_PROPS().moveSpeed = Q12(1.6f);
                    Math_ShortestAngleGet(player->rotation.vy, Q12_ANGLE(90.0f), &headingAngle);
                    g_Player_HeadingAngle = headingAngle;

                case 68:
                    if (player->attackReceived != WEAPON_ATTACK(EquippedWeaponId_Unk69, AttackInputType_Tap))
                    {
                        PLAYER_PROPS().moveSpeed = Q12(4.0f);
                        Math_ShortestAngleGet(player->rotation.vy, (s16)player->damage.position.vy, &headingAngle);
                        g_Player_HeadingAngle = headingAngle;
                    }

                case 40:
                case 43:
                case 44:
                case 45:
                case 46:
                case 48:
                case 50:
                case 52:
                case 57:
                case 58:
                case 59:
                case 61:
                case 64:
                case 65:
                    // Left harm animation.
                    if (enemyRotY >= Q12_ANGLE(45.0f) && enemyRotY < Q12_ANGLE(135.0f))
                    {
                        angleState = PlayerState_DamageTorsoLeft;
                    }
                    // Front harm animation.
                    else if (enemyRotY >= Q12_ANGLE(135.0f) && enemyRotY < Q12_ANGLE(225.0f))
                    {
                        angleState = PlayerState_DamageTorsoFront;
                    }
                    // Right harm animation.
                    else if (enemyRotY >= Q12_ANGLE(225.0f) && enemyRotY < Q12_ANGLE(315.0f))
                    {
                        angleState = PlayerState_DamageTorsoRight;
                    }
                    // Back harm animation.
                    else
                    {
                        angleState = PlayerState_DamageTorsoBack;
                    }

                    Player_ExtraStateSet(player, extra, angleState);
                    break;

                case 53:
                    if (enemyRotY < Q12_ANGLE(179.95f))
                    {
                        angleState = PlayerState_DamageFeetFront;
                    }
                    else
                    {
                        angleState = PlayerState_DamageFeetBack;
                    }

                    Player_ExtraStateSet(player, extra, angleState);
                    break;
            }

            if ((u32)PLAYER_EXTRA().state >= PlayerState_FallForward)
            {
                player->properties.player.afkTimer = Q12(0.0f);
                player->properties.player.field_F4 = 0;
                PLAYER_PROPS().flags                 &= ~PlayerFlag_Unk12;
                PLAYER_COMBAT().isAiming              = false;
                PLAYER_PROPS().flags                 &= ~PlayerFlag_Unk9;
                player->field_44.field_0           = NO_VALUE;
            }
            break;
    }

    // Clear damage if dead and return early.
    if (PLAYER_EXTRA().state == PlayerState_Death)
    {
        Chara_DamageClear(player);
        return;
    }

    // Apply damage.
    if (player->damage.amount != Q12(0.0f))
    {
        PLAYER_PROPS().flags &= ~PlayerFlag_Unk2;
        if (!(PLAYER_PROPS().flags & PlayerFlag_DamageReceived))
        {
            Sfx_WithFlagsPlay(sfxId, &player->position, Q8(1.0f / 8.0f), SfxFlag_None);
            PLAYER_PROPS().flags |= PlayerFlag_DamageReceived;
            player->properties.player.field_10C = 0x40;
        }

        if (g_SavegamePtr->mapIdx == MapIdx_MAP0_S00)
        {
            player->health -= player->damage.amount * 2;
        }
        else
        {
            switch (g_SavegamePtr->gameDifficulty)
            {
                case GameDifficulty_Easy:
                    player->damage.amount = DIV_FAST(player->damage.amount * 3, 4);
                    break;

                case GameDifficulty_Hard:
                    player->damage.amount = DIV_FAST(player->damage.amount * 6, 4);
                    break;
            }

            player->health -= player->damage.amount;
        }

        if (player->health < Q12(0.0f))
        {
            player->health = NO_VALUE;
            g_Player_IsDead  = true;
        }

        func_800893D0(player->damage.amount);
        player->damage.amount = Q12(0.0f);
    }

    if (player->health <= Q12(0.0f) &&
        PLAYER_EXTRA().state != PlayerState_Death                &&
        PLAYER_EXTRA().state != PlayerState_Unk36                &&
        PLAYER_EXTRA().state != PlayerState_EnemyGrabPinnedFront &&
        PLAYER_EXTRA().state != PlayerState_EnemyGrabPinnedBack  &&
        PLAYER_EXTRA().state != PlayerState_OnFloorFront         &&
        PLAYER_EXTRA().state != PlayerState_OnFloorBehind        &&
        !g_Player_IsInWalkToRunTransition)
    {
        player->field_40                     = NO_VALUE;
        g_SavegamePtr->healthSaturation = Q12(0.0f);

        for (i = 0; i < 4; i++)
        {
            g_SysWork.npcIdxs[i] = NO_VALUE;
        }

        if (player->attackReceived == WEAPON_ATTACK(EquippedWeaponId_ShotgunShells, AttackInputType_Tap))
        {
            Player_ExtraStateSet(player, extra, PlayerState_Unk36);
        }
        else
        {
            Player_ExtraStateSet(player, extra, PlayerState_Death);
        }
    }

    #undef attacker
}

void func_8007D090(s_SubCharacter* player, s_PlayerExtra* extra, GsCOORDINATE2* boneCoords) // 0x8007D090
{
    #define FLEX_ROT_X_RANGE Q12_ANGLE(56.25f)
    #define FLEX_ROT_Y_RANGE Q12_ANGLE(33.75f)

    q19_12 flexRotStep;
    s32    temp_v0;
    q19_12 flexRotMax;
    q19_12 var_a2;
    s32    var_a3;
    q19_12 var_v1;

    switch (PLAYER_EXTRA().state)
    {
        case PlayerState_Combat:
            switch (PLAYER_COMBAT().weaponAttack)
            {
                case WEAPON_ATTACK(EquippedWeaponId_Handgun, AttackInputType_Tap):
                    var_a2 = 20;
                    var_a3 = 2;
                    break;

                case WEAPON_ATTACK(EquippedWeaponId_HuntingRifle, AttackInputType_Tap):
                    var_a2 = 18;
                    var_a3 = 6;
                    break;

                case WEAPON_ATTACK(EquippedWeaponId_Shotgun, AttackInputType_Tap):
                    var_a2 = 26;
                    var_a3 = 3;
                    break;

                case WEAPON_ATTACK(EquippedWeaponId_HyperBlaster, AttackInputType_Tap):
                    var_a2 = 30;
                    var_a3 = 0;
                    break;

                default:
                    var_a2 = 12;
                    var_a3 = 6;
                    break;
            }

            if (PLAYER_EXTRA().upperBodyState == PlayerUpperBodyState_Reload ||
                PLAYER_COMBAT().weaponAttack < WEAPON_ATTACK(EquippedWeaponId_Handgun, AttackInputType_Tap))
            {
                g_Player_FlexRotationX = Q12_ANGLE(0.0f);
                g_Player_FlexRotationY = Q12_ANGLE(0.0f);
            }
            else
            {
                g_Player_FlexRotationX = PLAYER_PROPS().field_122 - Q12_ANGLE(90.0f);

                if (ABS(player->angleToTarget - player->rotation.vy) > Q12_ANGLE(180.0f))
                {
                    if (player->angleToTarget > player->rotation.vy)
                    {
                        g_Player_FlexRotationY = -Q12_ANGLE_NORM_U((player->rotation.vy + Q12_ANGLE(360.0f)) - player->angleToTarget);
                    }
                    else
                    {
                        g_Player_FlexRotationY = Q12_ANGLE_NORM_U((player->angleToTarget + Q12_ANGLE(360.0f)) - player->rotation.vy);
                    }
                }
                else
                {
                    g_Player_FlexRotationY = player->angleToTarget - player->rotation.vy;
                }

                if (player->properties.player.field_100 != 0 ||
                    PLAYER_EXTRA().upperBodyState == PlayerUpperBodyState_AimStartTargetLock ||
                    PLAYER_EXTRA().upperBodyState == PlayerUpperBodyState_AimTargetLockSwitch)
                {
                    if (PLAYER_PROPS().flags & PlayerFlag_Unk8)
                    {
                        var_v1 = Q12_ANGLE(0.0f);
                    }
                    else
                    {
                        var_v1 = var_a3;
                    }

                    if (player->properties.player.field_F4 != 0)
                    {
                        flexRotStep = player->properties.player.field_F4 - g_Player_FlexRotationX;
                        if (g_Player_FlexRotationX < player->properties.player.field_F4)
                        {
                            flexRotMax = player->properties.player.field_100 * TIMESTEP_SCALE_30_FPS(g_DeltaTime, var_a2 + player->properties.player.field_100);
                            if (flexRotMax < flexRotStep)
                            {
                                g_Player_FlexRotationX = player->properties.player.field_F4 - flexRotMax;
                            }
                        }
                        else
                        {
                            flexRotMax = -(player->properties.player.field_100 * TIMESTEP_SCALE_30_FPS(g_DeltaTime, var_a2 + player->properties.player.field_100));
                            if (flexRotStep < flexRotMax)
                            {
                                g_Player_FlexRotationX = player->properties.player.field_F4 - flexRotMax;
                            }
                        }
                    }
                    else
                    {
                        if (player->properties.player.field_100 < var_v1)
                        {
                            g_Player_FlexRotationY = Q12_ANGLE(0.0f);
                            g_Player_FlexRotationX = Q12_ANGLE(0.0f);
                        }
                        else
                        {
                            temp_v0     = player->properties.player.field_100 + 1;
                            flexRotStep = temp_v0 - var_v1;
                            flexRotMax  = Q12_ANGLE(0.4f);
                            flexRotMax  = flexRotStep * TIMESTEP_SCALE_30_FPS(g_DeltaTime, var_a2 + ((flexRotStep * 2) + flexRotMax));

                            // Clamp X-axis flex angle.
                            if (g_Player_FlexRotationX > Q12_ANGLE(0.0f))
                            {
                                if (flexRotMax < g_Player_FlexRotationX)
                                {
                                    g_Player_FlexRotationX = flexRotMax;
                                }
                            }
                            else if (g_Player_FlexRotationX < Q12_ANGLE(0.0f))
                            {
                                if (g_Player_FlexRotationX < -flexRotMax)
                                {
                                    g_Player_FlexRotationX = -flexRotMax;
                                }
                            }

                            // Clamp Y-axis flex angle.
                            if (g_Player_FlexRotationY > Q12_ANGLE(0.0f))
                            {
                                if (flexRotMax < g_Player_FlexRotationY)
                                {
                                    g_Player_FlexRotationY = flexRotMax;
                                }
                            }
                            else if (g_Player_FlexRotationY < Q12_ANGLE(0.0f))
                            {
                                if (g_Player_FlexRotationY < -flexRotMax)
                                {
                                    g_Player_FlexRotationY = -flexRotMax;
                                }
                            }
                        }
                    }
                }

                g_Player_FlexRotationX = CLAMP(g_Player_FlexRotationX, -FLEX_ROT_X_RANGE, FLEX_ROT_X_RANGE);
                g_Player_FlexRotationY = CLAMP(g_Player_FlexRotationY, -FLEX_ROT_Y_RANGE, FLEX_ROT_Y_RANGE);

                // Apply flex rotation to torso and arms.
                Bone_ExtraRotationSet(&boneCoords[HarryBone_Torso], Q12_ANGLE(0.0f), DIV_FAST(g_Player_FlexRotationX, 2), g_Player_FlexRotationY);
                Math_RotMatrixZ(DIV_FAST(g_Player_FlexRotationX, 2), &boneCoords[HarryBone_LeftUpperArm].coord);
                Math_RotMatrixZ(DIV_FAST(g_Player_FlexRotationX, 2), &boneCoords[HarryBone_RightUpperArm].coord);
            }
            break;

        case PlayerState_None:
            // Pre-modulate X-axis flex angle.
            if (g_Player_FlexRotationX > Q12_ANGLE(0.0f))
            {
                g_Player_FlexRotationX -= Q12_ANGLE(2.9f);
            }
            else if (g_Player_FlexRotationX < Q12_ANGLE(0.0f))
            {
                g_Player_FlexRotationX += Q12_ANGLE(2.9f);
            }

            // Compute flex rotation step for current frame.
            flexRotStep = TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12_ANGLE(2.15f));

            // Modulate X-axis flex angle.
            if (g_Player_FlexRotationX > Q12_ANGLE(0.0f))
            {
                g_Player_FlexRotationX -= flexRotStep;
                if (g_Player_FlexRotationX < Q12_ANGLE(0.0f))
                {
                    g_Player_FlexRotationX = Q12_ANGLE(0.0f);
                }
            }
            else if (g_Player_FlexRotationX < Q12_ANGLE(0.0f))
            {
                g_Player_FlexRotationX += flexRotStep;
                if (g_Player_FlexRotationX > Q12_ANGLE(0.0f))
                {
                    g_Player_FlexRotationX = Q12_ANGLE(0.0f);
                }
            }

            // Modulate Y-axis flex angle.
            if (g_Player_FlexRotationY > Q12_ANGLE(0.0f))
            {
                g_Player_FlexRotationY -= flexRotStep;
                if (g_Player_FlexRotationY < Q12_ANGLE(0.0f))
                {
                    g_Player_FlexRotationY = Q12_ANGLE(0.0f);
                }
            }
            else if (g_Player_FlexRotationY < Q12_ANGLE(0.0f))
            {
                g_Player_FlexRotationY += flexRotStep;
                if (g_Player_FlexRotationY > Q12_ANGLE(0.0f))
                {
                    g_Player_FlexRotationY = Q12_ANGLE(0.0f);
                }
            }

            // Apply flex rotation to torso and arms.
            Bone_ExtraRotationSet(&boneCoords[HarryBone_Torso], Q12_ANGLE(0.0f), DIV_FAST(g_Player_FlexRotationX, 2), g_Player_FlexRotationY);
            Math_RotMatrixZ(DIV_FAST(g_Player_FlexRotationX, 2), &boneCoords[HarryBone_LeftUpperArm].coord);
            Math_RotMatrixZ(DIV_FAST(g_Player_FlexRotationX, 2), &boneCoords[HarryBone_RightUpperArm].coord);
            break;

        case PlayerState_Unk180:
            if (g_Player_FlexRotationY != Q12_ANGLE(0.0f))
            {
                Bone_ExtraRotationSet(&boneCoords[HarryBone_Torso], Q12_ANGLE(0.0f), Q12_ANGLE(0.0f),  Q12_ANGLE(16.9f));
                Bone_ExtraRotationSet(&boneCoords[HarryBone_Head],  Q12_ANGLE(0.0f), Q12_ANGLE(28.2f), Q12_ANGLE(19.7f));
            }
            break;

        default:
            if (PLAYER_EXTRA().state >= PlayerState_Reset &&
                PLAYER_EXTRA().state <  PlayerState_Unk59)
            {
                Bone_ExtraRotationSet(&boneCoords[HarryBone_Head], Q12_ANGLE(0.0f), Q12_ANGLE(0.0f), g_Player_FlexRotationY);
            }
            else
            {
                g_Player_FlexRotationY = Q12_ANGLE(0.0f);
                g_Player_FlexRotationX = Q12_ANGLE(0.0f);
            }
            break;
    }

    if (g_SysWork.enablePlayerMatchAnim && PLAYER_EXTRA().state < PlayerState_Unk58)
    {
        Bone_ExtraRotationSet(&g_SysWork.playerBoneCoords[HarryBone_RightUpperArm], Q12_ANGLE(0.0f),   Q12_ANGLE(63.3f), Q12_ANGLE(-8.8f));
        Bone_ExtraRotationSet(&g_SysWork.playerBoneCoords[HarryBone_RightForearm],  Q12_ANGLE(-14.1f), Q12_ANGLE(22.5f), Q12_ANGLE(-30.8f));
        Bone_ExtraRotationSet(&g_SysWork.playerBoneCoords[HarryBone_RightHand],     Q12_ANGLE(13.2f),  Q12_ANGLE(0.0f),  Q12_ANGLE(0.0f));
    }
}

void Player_FlexRotationYReset(void) // 0x8007D6E0
{
    g_Player_FlexRotationY = Q12_ANGLE(0.1f);
}

s32 Player_LowerBodyMoveStateGet(s_SubCharacter* player, s_800C45C8* arg1) // 0x8007D6F0
{
    s_RayTrace traces[2];
    VECTOR3    vecs[4];
    bool       traceHits[2];
    s32        moveSpeed0;
    s32        moveSpeed1;
    s32        offsetZ0;
    s32        offsetX0;
    q19_12     offsetX1;
    q19_12     offsetZ1;
    q3_12      angle;
    q4_12      angleDelta;

    moveSpeed0  = DIV_FAST(PLAYER_PROPS().moveSpeed, 8);
    moveSpeed0 += Q12(0.75f);
    moveSpeed1  = Q12(-0.6f);
    moveSpeed1 -= DIV_FAST(PLAYER_PROPS().moveSpeed, 16);

    offsetX0 = Q12_MULT(Math_Cos(player->headingAngle), Q12(0.2f));
    offsetZ0 = Q12_MULT(Math_Sin(player->headingAngle), Q12(0.2f));
    offsetX1 = Q12_MULT(moveSpeed0, Math_Sin(player->headingAngle));
    offsetZ1 = Q12_MULT(moveSpeed0, Math_Cos(player->headingAngle));

    moveSpeed1 -= Q12(0.4f);

    vecs[0].vy = player->position.vy + moveSpeed1;
    vecs[0].vx = (player->position.vx + offsetX0) + offsetX1;

    vecs[0].vz = (player->position.vz - offsetZ0) + offsetZ1;
    vecs[2].vy = player->position.vy - Q12(0.4f);
    vecs[2].vx = player->position.vx + offsetX0;
    vecs[2].vz = player->position.vz - offsetZ0;

    traceHits[0] = Ray_GeomTraceGet(&traces[0], &vecs[2], &vecs[0]);

    if (traceHits[0])
    {
        vecs[1].vy = vecs[0].vy;
        vecs[1].vx = (player->position.vx - offsetX0) + offsetX1;
        vecs[1].vz = (player->position.vz + offsetZ0) + offsetZ1;
        vecs[3].vy = vecs[2].vy;
        vecs[3].vx = player->position.vx - offsetX0;
        vecs[3].vz = player->position.vz + offsetZ0;

        traceHits[1] = Ray_GeomTraceGet(&traces[1], &vecs[3], &vecs[1]);

        if (traceHits[1])
        {
            arg1->field_14   = DIV_FAST(traces[0].hitDistance + traces[1].hitDistance, 2);
            arg1->floorType = traces[0].floorType;

            angle      = Q12_ANGLE_NORM_U(Q12_ANGLE_ABS(DIV_FAST(traces[0].headingAngle + traces[1].headingAngle, 2)));
            angleDelta = ABS_DIFF(angle, player->headingAngle);
            if (angleDelta > Q12_ANGLE(160.0f) && angleDelta < Q12_ANGLE(200.0f))
            {
                if ((player->position.vy - Q12(1.3f)) < traces[0].floorHeight ||
                    traces[0].floorType == FloorType_Default ||
                    traces[0].floorType == FloorType_None)
                {
                    if ((player->position.vy - Q12(0.3f)) >= traces[0].floorHeight)
                    {
                        return PlayerLowerBodyState_RunForward;
                    }
                }
                else
                {
                    return PlayerLowerBodyState_WalkForward;
                }
            }
        }
    }

    return PlayerLowerBodyState_None;
}

void Player_CombatUpdate(s_SubCharacter* player, GsCOORDINATE2* boneCoords) // 0x8007D970
{
    VECTOR                blasterBeamFrom; // Q19.12
    VECTOR                partBeamFrom;    // Q19.12
    VECTOR                partBeamTo;      // Q19.12
    MATRIX                handTransformMat;
    VECTOR                handPos;      // Q23.8
    VECTOR                partBeamToQ8; // Q23.8
    SVECTOR               localHandPos;
    DVECTOR               rotDeltaToTarget;
    s32                   distToAttackPosSqr;
    q23_8                 offsetToAttackPosX;
    q23_8                 offsetToAttackPosZ;
    q3_12                 angleDeltaToTargetY;
    VECTOR*               attackPos0; // Q19.12
    VECTOR*               attackPos1; // Q19.12
    VECTOR*               attackPos2; // Q19.12
    s_Model*              model;
    static s32            __pad_bss_800C44D8[2];
    static VECTOR3        attackPos;
    static s32            __pad_bss_800C44EC;

    #define targetNpc NPC_CHARA(g_Player_TargetNpcIdx)
    #define unused    NPC_CHARA(0)

    model = &PLAYER_EXTRA().model;

    // Ranged attack.
    if (PLAYER_EXTRA().lowerBodyState < PlayerLowerBodyState_Aim)
    {
        attackPos0     = &PLAYER_COMBAT().attackPosition;
        attackPos0->vx = Q8_TO_Q12(g_SysWork.playerBoneCoords[HarryBone_RightFoot].workm.t[0]);
        attackPos0->vy = Q8_TO_Q12(g_SysWork.playerBoneCoords[HarryBone_RightFoot].workm.t[1]);
        attackPos0->vz = Q8_TO_Q12(g_SysWork.playerBoneCoords[HarryBone_RightFoot].workm.t[2]);
    }
    // Melee attack.
    else
    {
        switch (PLAYER_COMBAT().weaponAttack)
        {
            case NO_VALUE:
            case WEAPON_ATTACK(EquippedWeaponId_Kick,  AttackInputType_Tap):
            case WEAPON_ATTACK(EquippedWeaponId_Stomp, AttackInputType_Tap):
                attackPos1     = &PLAYER_COMBAT().attackPosition;
                attackPos1->vx = Q8_TO_Q12(g_SysWork.playerBoneCoords[HarryBone_RightFoot].workm.t[0]);
                attackPos1->vy = Q8_TO_Q12(g_SysWork.playerBoneCoords[HarryBone_RightFoot].workm.t[1]);
                attackPos1->vz = Q8_TO_Q12(g_SysWork.playerBoneCoords[HarryBone_RightFoot].workm.t[2]);
                break;

            default:
                attackPos2     = &PLAYER_COMBAT().attackPosition;
                attackPos2->vx = Q8_TO_Q12(g_SysWork.playerBoneCoords[HarryBone_RightHand].workm.t[0]);
                attackPos2->vy = Q8_TO_Q12(g_SysWork.playerBoneCoords[HarryBone_RightHand].workm.t[1]);
                attackPos2->vz = Q8_TO_Q12(g_SysWork.playerBoneCoords[HarryBone_RightHand].workm.t[2]);
                break;
        }
    }

    if (PLAYER_PROPS().gasWeaponPowerTimer != Q12(0.0f))
    {
        g_SysWork.sysStateStepData[1]++;

        if (PLAYER_PROPS().moveSpeed >= Q12(3.1739f) || (g_SysWork.sysStateStepData[1] % 2))
        {
            func_8006342C(g_SavegamePtr->equippedWeapon - InvItemId_KitchenKnife,
                          Q12_ANGLE(0.0f), Q12_ANGLE(0.0f), boneCoords);
        }
    }

    if (!(PLAYER_EXTRA().state >= PlayerState_Unk7 && PLAYER_EXTRA().state < PlayerState_Unk51) &&
        ((PLAYER_EXTRA().state >= PlayerState_None && PLAYER_EXTRA().state < PlayerState_Idle) ||
         PLAYER_EXTRA().state == PlayerState_KickEnemy ||
         PLAYER_EXTRA().state == PlayerState_StompEnemy))
    {
        if (PLAYER_COMBAT().weaponAttack >= EquippedWeaponId_Handgun &&
            PLAYER_EXTRA().lowerBodyState >= PlayerLowerBodyState_Aim)
        {
            if (PLAYER_EXTRA().state == PlayerState_Combat && g_Player_TargetNpcIdx != NO_VALUE)
            {
                rotDeltaToTarget.vx = ratan2((targetNpc.position.vx + targetNpc.collision.shapeOffsets.box.vx) - PLAYER_COMBAT().attackPosition.vx,
                                           (targetNpc.position.vz + targetNpc.collision.shapeOffsets.box.vz) - PLAYER_COMBAT().attackPosition.vz);
            }
            else
            {
                // @hack Required for match.
                do { player->angleToTarget = player->rotation.vy; } while (false);

                rotDeltaToTarget.vx = player->angleToTarget;
            }

            rotDeltaToTarget.vy   = PLAYER_PROPS().field_122;
            angleDeltaToTargetY = rotDeltaToTarget.vy;
            if (angleDeltaToTargetY >= Q12_ANGLE(33.75f))
            {
                if (angleDeltaToTargetY > Q12_ANGLE(146.25f))
                {
                    angleDeltaToTargetY = Q12_ANGLE(146.25f);
                }
            }
            else
            {
                angleDeltaToTargetY = Q12_ANGLE(33.75f);
            }

            if (player->field_44.field_0 > 0)
            {
                func_8006342C(PLAYER_COMBAT().weaponAttack, angleDeltaToTargetY, rotDeltaToTarget.vx, boneCoords);
            }
        }
        else
        {
            switch (PLAYER_COMBAT().weaponAttack)
            {
                case NO_VALUE:
                case EquippedWeaponId_Kick:
                case EquippedWeaponId_Stomp:
                    // Compute transformation matrix for right foot.
                    Math_SetSVectorFast(&localHandPos, 0, 60, 134);
                    Vw_CoordHierarchyMatrixCompute(&boneCoords[HarryBone_RightFoot], &handTransformMat);
                    break;

                default:
                    if (PLAYER_EXTRA().lowerBodyState < PlayerLowerBodyState_Aim)
                    {
                        // Compute transformation matrix for right foot.
                        Math_SetSVectorFast(&localHandPos, 0, 60, 134);
                        Vw_CoordHierarchyMatrixCompute(&boneCoords[HarryBone_RightFoot], &handTransformMat);
                    }
                    else
                    {
                        switch (WEAPON_ATTACK_ID_GET(PLAYER_COMBAT().weaponAttack))
                        {
                            case EquippedWeaponId_KitchenKnife:
                                Math_SetSVectorFastSum(&localHandPos, Q12_MULT(D_800AD4C8[PLAYER_COMBAT().weaponAttack].field_0, 0xF),
                                                       -FP_MULTIPLY(D_800AD4C8[PLAYER_COMBAT().weaponAttack].field_0, Q12(0.0185f), Q12_SHIFT - 1),
                                                        Q12_MULT(D_800AD4C8[PLAYER_COMBAT().weaponAttack].field_0, Q12(0.0185f)) >> 1);
                                break;

                            case EquippedWeaponId_SteelPipe:
                                Math_SetSVectorFastSum(&localHandPos, Q12_MULT(D_800AD4C8[PLAYER_COMBAT().weaponAttack].field_0, 0xF),
                                                       -(Q12_MULT(D_800AD4C8[PLAYER_COMBAT().weaponAttack].field_0, Q12(0.055f)) >> 1),
                                                        FP_MULTIPLY(D_800AD4C8[PLAYER_COMBAT().weaponAttack].field_0, Q12(0.011f), Q12_SHIFT - 2));
                                break;

                            case EquippedWeaponId_Chainsaw:
                                Math_SetSVectorFastSum(&localHandPos,
                                                        Q12_MULT(D_800AD4C8[PLAYER_COMBAT().weaponAttack].field_0, 0xF) >> 1,
                                                       -(Q12_MULT(D_800AD4C8[PLAYER_COMBAT().weaponAttack].field_0, Q12(0.033f)) >> 1),
                                                        (Q12_MULT(D_800AD4C8[PLAYER_COMBAT().weaponAttack].field_0, Q12(0.121f)) >> 1));
                                break;

                            case EquippedWeaponId_RockDrill:
                                Math_SetSVectorFastSum(&localHandPos,
                                                        Q12(0.0f),
                                                       -(Q12_MULT(D_800AD4C8[PLAYER_COMBAT().weaponAttack].field_0, Q12(0.011f))),
                                                        FP_MULTIPLY(D_800AD4C8[PLAYER_COMBAT().weaponAttack].field_0, Q12(0.011f), Q12_SHIFT - 2));
                                break;

                            case EquippedWeaponId_Axe:
                                Math_SetSVectorFastSum(&localHandPos,
                                                         Q12(0.0f),
                                                        -(Q12_MULT(D_800AD4C8[PLAYER_COMBAT().weaponAttack].field_0, 0x2C1) >> 1),
                                                         Q12_MULT((u32)D_800AD4C8[PLAYER_COMBAT().weaponAttack].field_0, 0xC3));
                                break;

                            case EquippedWeaponId_Hammer:
                                Math_SetSVectorFastSum(&localHandPos, (Q12_MULT(D_800AD4C8[PLAYER_COMBAT().weaponAttack].field_0, 0xF) >> 1),
                                                       -(Q12_MULT(D_800AD4C8[PLAYER_COMBAT().weaponAttack].field_0, 0x69)),
                                                        Q12_MULT(D_800AD4C8[PLAYER_COMBAT().weaponAttack].field_0, 0x13B) >> 1);
                                break;

                            case EquippedWeaponId_Katana:
                                Math_SetSVectorFastSum(&localHandPos, (Q12_MULT(D_800AD4C8[PLAYER_COMBAT().weaponAttack].field_0, 0xF) >> 1),
                                                       -(Q12_MULT(D_800AD4C8[PLAYER_COMBAT().weaponAttack].field_0, 0x13B) >> 1),
                                                        Q12_MULT(D_800AD4C8[PLAYER_COMBAT().weaponAttack].field_0, 0xF));
                                break;
                        }

                        // Compute transformation matrix for right hand.
                        Vw_CoordHierarchyMatrixCompute(&boneCoords[HarryBone_RightHand], &handTransformMat);
                    }
                    break;
            }

            // Compute rotation from hand to attack position.
            gte_SetRotMatrix(&handTransformMat);
            gte_SetTransMatrix(&handTransformMat);
            gte_ldv0(&localHandPos);
            gte_rt();
            gte_stlvnl(&handPos);
            offsetToAttackPosX = Q12_TO_Q8(PLAYER_COMBAT().attackPosition.vx) - handPos.vx;
            offsetToAttackPosZ = Q12_TO_Q8(PLAYER_COMBAT().attackPosition.vz) - handPos.vz;
            distToAttackPosSqr = Math_Vector2MagCalc(offsetToAttackPosX, offsetToAttackPosZ);
            rotDeltaToTarget.vx  = ratan2(handPos.vx - Q12_TO_Q8(PLAYER_COMBAT().attackPosition.vx),
                                        handPos.vz - Q12_TO_Q8(PLAYER_COMBAT().attackPosition.vz));
            rotDeltaToTarget.vy  = ratan2(distToAttackPosSqr, handPos.vy - Q12_TO_Q8(PLAYER_COMBAT().attackPosition.vy));
        }

        // Draw Hyper Blaster beam.
        if (PLAYER_COMBAT().weaponAttack == WEAPON_ATTACK(EquippedWeaponId_HyperBlaster, AttackInputType_Tap) &&
            PLAYER_COMBAT().isAiming &&
            model->anim.status >= ANIM_STATUS(HarryAnim_HandgunAim, true) &&
            model->anim.keyframeIdx >= 574)
        {
            if (PLAYER_EXTRA().state < PlayerState_Idle)
            {
                if (PLAYER_EXTRA().state == PlayerState_None && g_SysWork.targetNpcIdx != NO_VALUE)
                {
                    g_SysWork.targetNpcIdx = NO_VALUE;
                }

                Math_SetSVectorFast(&localHandPos, Q8(0.0f), Q8(-0.156f), Q8(0.34f));
                localHandPos.vz = Q8(0.34f);

                // Compute beam start position.
                Vw_CoordHierarchyMatrixCompute(&boneCoords[HarryBone_RightHand], &handTransformMat);
                gte_SetRotMatrix(&handTransformMat);
                gte_SetTransMatrix(&handTransformMat);
                gte_ldv0(&localHandPos);
                gte_rt();
                gte_stlvnl(&partBeamToQ8);
                blasterBeamFrom.vx = Q8_TO_Q12(partBeamToQ8.vx);
                blasterBeamFrom.vy = Q8_TO_Q12(partBeamToQ8.vy);
                blasterBeamFrom.vz = Q8_TO_Q12(partBeamToQ8.vz);

                // Set beam rotation.
                if (g_GameWork.config.extraAutoAiming)
                {
                    rotDeltaToTarget.vx = player->angleToTarget;
                }

                // Draw beam.
                g_MapOverlayHdr.particleHyperBlasterBeamDraw(&blasterBeamFrom, &rotDeltaToTarget.vx, &rotDeltaToTarget.vy);
            }
        }

        // Draw particle beam.
        if (PLAYER_EXTRA().state < PlayerState_Idle)
        {
            if ((PLAYER_COMBAT().weaponAttack == WEAPON_ATTACK(EquippedWeaponId_Chainsaw, AttackInputType_Tap) &&
                 model->anim.keyframeIdx >= 572 && model->anim.keyframeIdx < 584) ||
                (PLAYER_COMBAT().weaponAttack == WEAPON_ATTACK(EquippedWeaponId_RockDrill, AttackInputType_Tap) &&
                 player->model.anim.keyframeIdx >= 577 && model->anim.keyframeIdx < 583))
            {
                // Compute beam start position.
                Math_SetSVectorFast(&localHandPos, Q8(0.0f), Q8(0.0f), Q8(0.0f));
                Vw_CoordHierarchyMatrixCompute(&boneCoords[HarryBone_RightHand], &handTransformMat);
                gte_SetRotMatrix(&handTransformMat);
                gte_SetTransMatrix(&handTransformMat);
                gte_ldv0(&localHandPos);
                gte_rt();
                gte_stlvnl(&partBeamToQ8);
                Math_SetSVectorFast(&localHandPos, Q8(0.0f), Q8(0.0f), Q8(0.0f));
                partBeamFrom.vx = Q8_TO_Q12(partBeamToQ8.vx);
                partBeamFrom.vy = Q8_TO_Q12(partBeamToQ8.vy);
                partBeamFrom.vz = Q8_TO_Q12(partBeamToQ8.vz);

                // Compute beam end position.
                Vw_CoordHierarchyMatrixCompute(&boneCoords[HarryBone_LeftHand], &handTransformMat);
                gte_SetRotMatrix(&handTransformMat);
                gte_SetTransMatrix(&handTransformMat);
                gte_ldv0(&localHandPos);
                gte_rt();
                gte_stlvnl(&partBeamToQ8);
                partBeamTo.vx = Q8_TO_Q12(partBeamToQ8.vx);
                partBeamTo.vy = Q8_TO_Q12(partBeamToQ8.vy);
                partBeamTo.vz = Q8_TO_Q12(partBeamToQ8.vz);

                // Draw beam.
                g_MapOverlayHdr.particleBeamDraw(&partBeamFrom, &partBeamTo);
            }
        }

        if (PLAYER_EXTRA().upperBodyState != PlayerUpperBodyState_AimStop)
        {
            if (PLAYER_COMBAT().weaponAttack >= WEAPON_ATTACK(EquippedWeaponId_Handgun, AttackInputType_Tap))
            {
                if (g_Player_RotationToTargetX != NO_VALUE || g_Player_RotationToTargetY != g_Player_RotationToTargetX)
                {
                    func_8008A0E4(player->field_44.field_0, PLAYER_COMBAT().weaponAttack, player, &attackPos, &unused, g_Player_RotationToTargetY, g_Player_RotationToTargetX);
                }
                else
                {
                    func_8008A0E4(player->field_44.field_0, PLAYER_COMBAT().weaponAttack, player, &attackPos, &unused, rotDeltaToTarget.vx, rotDeltaToTarget.vy);
                }
            }
            else
            {
                func_8008A0E4(player->field_44.field_0, PLAYER_COMBAT().weaponAttack, player, &PLAYER_COMBAT().attackPosition, &unused, rotDeltaToTarget.vx, rotDeltaToTarget.vy);
            }

            g_Player_RotationDeltaToTargetX = rotDeltaToTarget.vx;
            g_Player_RotationDeltaToTargetY = rotDeltaToTarget.vy;
        }
    }

    attackPos = PLAYER_COMBAT().attackPosition;

    #undef targetNpc
    #undef unused
}

void Game_SavegameResetPlayer(void) // 0x8007E530
{
    #define DEFAULT_INV_SLOT_COUNT 8

    s32 i;

    g_SavegamePtr->invSlotCount = DEFAULT_INV_SLOT_COUNT;

    for (i = 0; i < INV_ITEM_COUNT_MAX; i++)
    {
        g_SavegamePtr->items[i].id    = NO_VALUE;
        g_SavegamePtr->items[i].count = 0;
    }

    g_SavegamePtr->playerHealth      = Q12(100.0f);
    g_SavegamePtr->field_A0          = 0;
    g_SavegamePtr->equippedWeapon    = InvItemId_Unequipped;
    g_SavegamePtr->healthSaturation  = Q12(0.0f);
    g_SavegamePtr->gameplayTimer     = Q12(0.0f);
    g_SavegamePtr->runDistance       = Q12(0.0f);
    g_SavegamePtr->walkDistance      = Q12(0.0f);
    g_SavegamePtr->pickedUpItemCount = 0;
    g_SavegamePtr->clearGameCount    = 0;
    g_SavegamePtr->add290Hours       = 0;

    #undef DEFAULT_INV_SLOT_COUNT
}

void Game_PlayerInfoInit(void) // 0x8007E5AC
{
    s32      i;
    u32      itemGroupId;
    s_Model* model;
    s_Model* extraModel;

    SysWork_SavegameReadPlayer();

    PLAYER_CHARA().model.charaId                  = Chara_Harry;
    PLAYER_EXTRA().model.charaId                                  = Chara_Harry;
    PLAYER_CHARA().collision.cylinder.outerRadius = Q12(0.3f);
    PLAYER_CHARA().collision.cylinder.innerRadius = DEFAULT_PLAYER_CYLINDER_INNER_RADIUS;

    extraModel = &PLAYER_CHARA().model;
    model      = &PLAYER_EXTRA().model;

    g_SysWork.enablePlayerMatchAnim = false;

    extraModel->anim.flags |= AnimFlag_Unlocked | AnimFlag_Visible;
    model->anim.flags      |= AnimFlag_Unlocked | AnimFlag_Visible;

    PLAYER_CHARA().collision.state = CharaCollisionState_Npc;
    g_Inventory_EquippedItem                    = g_SavegamePtr->equippedWeapon;

    itemGroupId = INV_ITEM_GROUP(g_SavegamePtr->equippedWeapon);

    // Assign weapon that the player was holding when saving.
    if (itemGroupId == InvItemGroup_MeleeWeapons || itemGroupId == InvItemGroup_GunWeapons)
    {
        for (i = 0; g_SavegamePtr->items[i].id != g_SavegamePtr->equippedWeapon && i < INV_ITEM_COUNT_MAX; i++);

        PLAYER_COMBAT().weaponAttack       = g_SavegamePtr->equippedWeapon + InvItemId_KitchenKnife;
        PLAYER_COMBAT().currentWeaponAmmo  = g_SavegamePtr->items[i].count;
        PLAYER_COMBAT().weaponInventoryIdx = i;

        if (itemGroupId == InvItemGroup_MeleeWeapons)
        {
            PLAYER_COMBAT().totalWeaponAmmo = 0;
        }
        else
        {
            for (i = 0;
                 g_SavegamePtr->items[i].id != (g_SavegamePtr->equippedWeapon + InvItemId_HealthDrink) && i < INV_ITEM_COUNT_MAX;
                 i++);

            if (i == INV_ITEM_COUNT_MAX)
            {
                PLAYER_COMBAT().totalWeaponAmmo = 0;
            }
            else
            {
                PLAYER_COMBAT().totalWeaponAmmo = (s8)g_SavegamePtr->items[i].count;
            }
        }
    }
    else
    {
        PLAYER_COMBAT().weaponAttack       = NO_VALUE;
        PLAYER_COMBAT().currentWeaponAmmo  = 0;
        PLAYER_COMBAT().totalWeaponAmmo    = 0;
        PLAYER_COMBAT().weaponInventoryIdx = NO_VALUE;
    }

    PLAYER_COMBAT().isAiming          = false;
    g_Player_GrabReleaseInputTimer = Q12(0.0f);
    D_800C4588                     = 0;
    g_Player_CutsceneState             = PlayerCutsceneState_RunForward;
    g_Player_DisableControl        = false;

    switch (g_SavegamePtr->gameDifficulty)
    {
        case GameDifficulty_Easy:
            D_800C45EC = Q12(5.0f);
            break;

        case GameDifficulty_Normal:
            D_800C45EC = Q12(2.5f);
            break;

        case GameDifficulty_Hard:
            D_800C45EC = Q12(1.8f);
            break;
    }

    g_Player_PrevSelectedWeapon = NO_VALUE;
    g_GameWork.mapAnimIdx       = NO_VALUE;

    g_SavegamePtr->invSlotCount  = CLAMP(g_SavegamePtr->invSlotCount, INV_ITEM_COUNT_MAX / 5, INV_ITEM_COUNT_MAX);
    PLAYER_CHARA().health = CLAMP(PLAYER_CHARA().health, 1, Q12(100.0f));
}

void func_8007E860(void) // 0x8007E860
{
    s32 i;
    s32 startIdx;

    for (i = 0; i < 8; i++)
    {
        startIdx                            = 92;
        HARRY_BASE_ANIM_INFOS[startIdx + i] = g_MapOverlayHdr.harryMapAnimInfos[i + 16];
    }
}

void func_8007E8C0(void) // 0x8007E8C0
{
    s32             i;
    s_AnimInfo*     animInfos;
    s_SubCharacter* player;

    player    = &PLAYER_CHARA();
    animInfos = g_MapOverlayHdr.harryMapAnimInfos;

    for (i = 76; animInfos->playbackFunc != NULL; i++, animInfos++)
    {
        HARRY_BASE_ANIM_INFOS[i] = g_MapOverlayHdr.harryMapAnimInfos[i - 76];
    }

    if (g_SavegamePtr->mapIdx == MapIdx_MAP0_S01)
    {
        g_SysWork.enablePlayerMatchAnim = false;
    }

    player->properties.player.exhaustionTimer = Q12(0.0f);

    Player_CollisionReset();
    player->collision.cylinder.outerRadius = Q12(0.3f);
    player->collision.cylinder.innerRadius = DEFAULT_PLAYER_CYLINDER_INNER_RADIUS;
    g_GameWork.mapAnimIdx                  = NO_VALUE;

    func_8007E9C4();
}

void func_8007E9C4(void) // 0x8007E9C4
{
    s_SubCharacter* player;

    player = &PLAYER_CHARA();

    g_Player_IsInWalkToRunTransition = false;
    PLAYER_EXTRA().state                = PlayerState_None;
    PLAYER_EXTRA().upperBodyState       = PlayerUpperBodyState_None;
    PLAYER_EXTRA().lowerBodyState       = PlayerLowerBodyState_None;
    PLAYER_EXTRA().model.stateStep      = 0;
    PLAYER_EXTRA().model.controlState   = 0;

    player->model.stateStep     = 0;
    player->model.controlState  = 0;
    g_SysWork.playerStopFlags   = PlayerStopFlag_None;
    g_Player_FlexRotationY      = Q12_ANGLE(0.0f);
    g_Player_FlexRotationX      = Q12_ANGLE(0.0f);
    g_Player_PrevAttackReceived = NO_VALUE;
    PLAYER_COMBAT().isAiming       = false;

    func_8004C564(0, NO_VALUE);

    player->angleToTarget   = Q12_ANGLE(90.0f);
    g_Player_IsDead         = false;
    g_Player_DisableDamage  = false;
    g_Player_HasActionInput = false;
    g_Player_HasMoveInput   = false;
    g_Player_IsShooting     = false;
    g_Player_IsAttacking    = false;

    player->properties.player.afkTimer        = Q12(0.0f);
    player->properties.player.field_F4        = 0;
    player->properties.player.runStepSfxCount = 0;
    player->properties.player.field_100       = 0;
    player->properties.player.field_104       = 0;
    player->properties.player.runDistance     = Q12(0.0f);
    player->properties.player.timer_110       = 0;
    player->properties.player.flags           = 0;
    player->properties.player.moveSpeed       = 0;

    Chara_DamageClear(player);

    g_Player_IsHoldAttack  = false;
    player->flags         &= ~CharaFlag_Unk4;
    g_Player_PrevPosition  = player->position;
    g_SysWork.targetNpcIdx = NO_VALUE;
    player->field_40       = NO_VALUE;
    player->attackReceived = NO_VALUE;

    g_SysWork.npcIdxs[3]               = NO_VALUE;
    g_SysWork.npcIdxs[2]               = NO_VALUE;
    g_SysWork.npcIdxs[1]               = NO_VALUE;
    g_SysWork.npcIdxs[0]               = NO_VALUE;
    player->collision.cylinder.innerRadius = DEFAULT_PLAYER_CYLINDER_INNER_RADIUS;

    g_Player_IsAiming            = false;
    g_Player_IsRunning           = false;
    g_Player_IsMovingBackward    = false;
    g_Player_IsMovingForward     = false;
    g_Player_IsSteppingRightTap  = false;
    g_Player_IsSteppingRightHold = false;
    g_Player_IsTurningRight      = false;
    g_Player_IsSteppingLeftTap   = false;
    g_Player_IsSteppingLeftHold  = false;
    g_Player_IsTurningLeft       = false;
}

void GameFs_PlayerMapAnimLoad(s32 mapIdx) // 0x8007EB64
{
    #define BASE_FILE_IDX FILE_ANIM_HB_M0S00_ANM

    if (g_GameWork.mapAnimIdx != mapIdx ||
        mapIdx == (FILE_ANIM_HB_M6S04_ANM - BASE_FILE_IDX) ||
        mapIdx == (FILE_ANIM_HB_M7S01_ANM - BASE_FILE_IDX) ||
        mapIdx == (FILE_ANIM_HB_M7S02_ANM - BASE_FILE_IDX))
    {
        g_GameWork.mapAnimIdx = mapIdx;
        Fs_QueueStartRead(BASE_FILE_IDX + mapIdx, FS_BUFFER_4);
    }

    #undef BASE_FILE_IDX
}

void GameFs_WeaponInfoUpdate(void) // 0x8007EBBC
{
    s32 relAnimInfoIdx;
    s32 relKeyframeIdx;
    s32 i;

    relAnimInfoIdx = 0;
    relKeyframeIdx = 0;

    g_SysWork.targetNpcIdx = NO_VALUE;

    switch (PLAYER_COMBAT().weaponAttack)
    {
        case NO_VALUE:
            g_Player_EquippedWeaponInfo = g_Player_EquippedWeaponInfos[0];
            return;

        case WEAPON_ATTACK(EquippedWeaponId_KitchenKnife, AttackInputType_Tap):
            relAnimInfoIdx                                    = 30;
            relKeyframeIdx                                    = 15;
            g_Player_EquippedWeaponInfo                       = g_Player_EquippedWeaponInfos[1];
            PLAYER_CHARA().collision.box.field_8 = -0x1030;
            break;

        default:
            return;

        case WEAPON_ATTACK(EquippedWeaponId_Axe, AttackInputType_Tap):
            relAnimInfoIdx                                    = 0;
            relKeyframeIdx                                    = 0;
            g_Player_EquippedWeaponInfo                       = g_Player_EquippedWeaponInfos[3];
            PLAYER_CHARA().collision.box.field_8 = -0xFD0;
            break;

        case WEAPON_ATTACK(EquippedWeaponId_SteelPipe, AttackInputType_Tap):
        case WEAPON_ATTACK(EquippedWeaponId_Hammer, AttackInputType_Tap):
            g_Player_EquippedWeaponInfo = g_Player_EquippedWeaponInfos[2];
            switch (PLAYER_COMBAT().weaponAttack)
            {
                case WEAPON_ATTACK(EquippedWeaponId_Hammer, AttackInputType_Tap):
                    relAnimInfoIdx = 10;
                    relKeyframeIdx = 5;
                    break;

                case WEAPON_ATTACK(EquippedWeaponId_SteelPipe, AttackInputType_Tap):
                    relAnimInfoIdx = 20;
                    relKeyframeIdx = 10;
                    break;
            }

            PLAYER_CHARA().collision.box.field_8 = -0xEC0;
            break;

        case WEAPON_ATTACK(EquippedWeaponId_Chainsaw, AttackInputType_Tap):
            relAnimInfoIdx                                    = 50;
            relKeyframeIdx                                    = 25;
            g_Player_EquippedWeaponInfo                       = g_Player_EquippedWeaponInfos[4];
            PLAYER_CHARA().collision.box.field_8 = -0xE90;
            break;

        case WEAPON_ATTACK(EquippedWeaponId_RockDrill, AttackInputType_Tap):
            relAnimInfoIdx                                    = 64;
            relKeyframeIdx                                    = 32;
            g_Player_EquippedWeaponInfo                       = g_Player_EquippedWeaponInfos[5];
            PLAYER_CHARA().collision.box.field_8 = -0x12E0;
            break;

        case WEAPON_ATTACK(EquippedWeaponId_Katana, AttackInputType_Tap):
            relAnimInfoIdx                                    = 40;
            relKeyframeIdx                                    = 20;
            g_Player_EquippedWeaponInfo                       = g_Player_EquippedWeaponInfos[10];
            PLAYER_CHARA().collision.box.field_8 = -0xF20;
            break;

        case WEAPON_ATTACK(EquippedWeaponId_Handgun, AttackInputType_Tap):
            relAnimInfoIdx                                    = 78;
            relKeyframeIdx                                    = 39;
            g_Player_EquippedWeaponInfo                       = g_Player_EquippedWeaponInfos[6];
            PLAYER_CHARA().collision.box.field_8 = -0x1600;
            break;

        case WEAPON_ATTACK(EquippedWeaponId_HuntingRifle, AttackInputType_Tap):
            relAnimInfoIdx                                    = 96;
            relKeyframeIdx                                    = 48;
            g_Player_EquippedWeaponInfo                       = g_Player_EquippedWeaponInfos[7];
            PLAYER_CHARA().collision.box.field_8 = -0x1180;
            break;

        case WEAPON_ATTACK(EquippedWeaponId_Shotgun, AttackInputType_Tap):
            relAnimInfoIdx                                    = 114;
            relKeyframeIdx                                    = 57;
            g_Player_EquippedWeaponInfo                       = g_Player_EquippedWeaponInfos[8];
            PLAYER_CHARA().collision.box.field_8 = -0x1600;
            break;

        case WEAPON_ATTACK(EquippedWeaponId_HyperBlaster, AttackInputType_Tap):
            relAnimInfoIdx                                    = 132;
            relKeyframeIdx                                    = 66;
            g_Player_EquippedWeaponInfo                       = g_Player_EquippedWeaponInfos[9];
            PLAYER_CHARA().collision.box.field_8 = -0x1610;
            break;
    }

    for (i = 56; i < 76; i++)
    {
        HARRY_BASE_ANIM_INFOS[i] = HARRY_WEAPON_ANIM_INFOS[(i - 56) + relAnimInfoIdx];
    }

    for (i = 0; i < 10; i++)
    {
        D_800C44F0[i] = D_800294F4[i + relKeyframeIdx];
    }

    if (PLAYER_COMBAT().weaponAttack != NO_VALUE && g_Player_PrevSelectedWeapon != PLAYER_COMBAT().weaponAttack)
    {
        g_Player_PrevSelectedWeapon = PLAYER_COMBAT().weaponAttack;
        func_8007F14C(PLAYER_COMBAT().weaponAttack);

        switch (PLAYER_COMBAT().weaponAttack)
        {
            case EquippedWeaponId_KitchenKnife:
                Fs_QueueStartRead(FILE_ANIM_HB_WEP3_ANM, FS_BUFFER_12);
                break;

            case EquippedWeaponId_Axe:
                Fs_QueueStartRead(FILE_ANIM_HB_WEP1_ANM, FS_BUFFER_12);
                break;

            case EquippedWeaponId_SteelPipe:
            case EquippedWeaponId_Hammer:
                Fs_QueueStartRead(FILE_ANIM_HB_WEP2_ANM, FS_BUFFER_12);
                break;

            case EquippedWeaponId_Chainsaw:
                Fs_QueueStartRead(FILE_ANIM_HB_WEP6_ANM, FS_BUFFER_12);
                break;

            case EquippedWeaponId_RockDrill:
                Fs_QueueStartRead(FILE_ANIM_HB_WEP8_ANM, FS_BUFFER_12);
                break;

            case EquippedWeaponId_Katana:
                Fs_QueueStartRead(FILE_ANIM_HB_WEP9_ANM, FS_BUFFER_12);
                break;

            case EquippedWeaponId_Handgun:
                Fs_QueueStartRead(FILE_ANIM_HB_WEP4_ANM, FS_BUFFER_12);
                break;

            case EquippedWeaponId_HuntingRifle:
                Fs_QueueStartRead(FILE_ANIM_HB_WEP51_ANM, FS_BUFFER_12);
                break;

            case EquippedWeaponId_Shotgun:
                Fs_QueueStartRead(FILE_ANIM_HB_WEP52_ANM, FS_BUFFER_12);
                break;

            case EquippedWeaponId_HyperBlaster:
                Fs_QueueStartRead(FILE_ANIM_HB_WEP53_ANM, FS_BUFFER_12);
                break;
        }
    }
}

void func_8007F14C(u8 weaponAttack) // 0x8007F14C
{
    switch (weaponAttack)
    {
        case WEAPON_ATTACK(EquippedWeaponId_KitchenKnife, AttackInputType_Tap):
        case WEAPON_ATTACK(EquippedWeaponId_SteelPipe,    AttackInputType_Tap):
        case WEAPON_ATTACK(EquippedWeaponId_Hammer,       AttackInputType_Tap):
        case WEAPON_ATTACK(EquippedWeaponId_Katana,       AttackInputType_Tap):
        case WEAPON_ATTACK(EquippedWeaponId_Axe,          AttackInputType_Tap):
            SD_Call(164);
            break;

        case WEAPON_ATTACK(EquippedWeaponId_Chainsaw, AttackInputType_Tap):
            SD_Call(169);
            break;

        case WEAPON_ATTACK(EquippedWeaponId_RockDrill, AttackInputType_Tap):
            SD_Call(163);
            break;

        case WEAPON_ATTACK(EquippedWeaponId_Handgun, AttackInputType_Tap):
            SD_Call(166);
            break;

        case WEAPON_ATTACK(EquippedWeaponId_HuntingRifle, AttackInputType_Tap):
            SD_Call(167);
            break;

        case WEAPON_ATTACK(EquippedWeaponId_Shotgun, AttackInputType_Tap):
            SD_Call(168);
            break;

        case WEAPON_ATTACK(EquippedWeaponId_HyperBlaster, AttackInputType_Tap):
            SD_Call(165);
            break;
    }
}

void Game_PlayerMovementsReset(void) // 0x8007F1CC
{
    g_Player_HasActionInput          = false;
    g_Player_HasMoveInput            = false;
    g_Player_IsShooting              = false;
    g_Player_IsAttacking             = false;
    g_Player_IsHoldAttack            = false;
    g_Player_IsAiming                = false;
    g_Player_IsRunning               = false;
    g_Player_IsMovingBackward        = false;
    g_Player_IsMovingForward         = false;
    g_Player_IsSteppingRightTap      = false;
    g_Player_IsSteppingRightHold     = false;
    g_Player_IsTurningRight          = false;
    g_Player_IsSteppingLeftTap       = false;
    g_Player_IsSteppingLeftHold      = false;
    g_Player_IsTurningLeft           = false;
    g_Player_IsInWalkToRunTransition = false;
}

void Player_DisableDamage(u8* isPlayerDead, u8 disableDamage) // 0x8007F250
{
    *isPlayerDead          = g_Player_IsDead;
    g_Player_DisableDamage = disableDamage;
}

bool Player_IsAttacking(void) // 0x8007F26C
{
    if (PLAYER_EXTRA().upperBodyState == PlayerUpperBodyState_Attack ||
        PLAYER_EXTRA().state == PlayerState_KickEnemy ||
        PLAYER_EXTRA().state == PlayerState_StompEnemy)
    {
        return true;
    }

    return false;
}

bool Player_IsBusy(void) // 0x8007F2AC
{
    if (PLAYER_CHARA().health <= Q12(0.0f) ||
        PLAYER_COMBAT().isAiming ||
        PLAYER_EXTRA().state == PlayerState_KickEnemy ||
        PLAYER_EXTRA().state == PlayerState_StompEnemy ||
        (PLAYER_EXTRA().state >= PlayerState_Unk7 &&
         PLAYER_EXTRA().state <= PlayerState_GetUpBack))
    {
        return true;
    }

    return false;
}

s16 Player_AnimGetSomething(void) // 0x8007F308
{
    return HARRY_BASE_ANIM_INFOS[PLAYER_CHARA().model.anim.status].startKeyframeIdx;
}

void Player_Controller(void) // 0x8007F32C
{
    s32 heldAttackActionFlag;

    g_Player_IsMovingForward    = (g_Player_IsMovingForward    * 2) & 0x3;
    g_Player_IsSteppingLeftTap  = (g_Player_IsSteppingLeftTap  * 2) & 0x3F;
    g_Player_IsSteppingRightTap = (g_Player_IsSteppingRightTap * 2) & 0x3F;

    if (g_Controller0->rawSticks.sticks.leftY < -STICK_DEADZONE || g_Controller0->rawSticks.sticks.leftY >= STICK_DEADZONE ||
        g_Controller0->rawSticks.sticks.leftX < -STICK_DEADZONE || g_Controller0->rawSticks.sticks.leftX >= STICK_DEADZONE)
    {
        g_Player_IsTurningLeft    = g_Controller0->rawSticks.sticks.leftX < -STICK_DEADZONE ? ABS(g_Controller0->rawSticks.sticks.leftX + STICK_DEADZONE) : 0;
        g_Player_IsTurningRight   = g_Controller0->rawSticks.sticks.leftX >= STICK_DEADZONE ? (g_Controller0->rawSticks.sticks.leftX - (STICK_DEADZONE - 1)) : 0;
        g_Player_IsMovingForward |= g_Controller0->rawSticks.sticks.leftY < -STICK_DEADZONE;
        g_Player_IsMovingBackward = g_Controller0->rawSticks.sticks.leftY >= STICK_DEADZONE;
        g_Player_HasMoveInput     = g_Controller0->buttonFlags.clicked & (g_GameWorkPtr->config.controllerConfig.stepLeft |
                                                                          (ControllerFlag_LStickLowUp    |
                                                                           ControllerFlag_LStickLowRight |
                                                                           ControllerFlag_LStickLowDown  |
                                                                           ControllerFlag_LStickLowLeft) |
                                                                          g_GameWorkPtr->config.controllerConfig.stepRight |
                                                                          g_GameWorkPtr->config.controllerConfig.aim);
    }
    else
    {
        g_Player_IsTurningLeft    = ((g_Controller0->buttonFlags.held & (ControllerFlag_LStickHighRight | ControllerFlag_LStickHighLeft)) == ControllerFlag_LStickHighLeft) << 6;
        g_Player_IsTurningRight   = ((g_Controller0->buttonFlags.held & (ControllerFlag_LStickHighRight | ControllerFlag_LStickHighLeft)) == ControllerFlag_LStickHighRight) << 6;
        g_Player_IsMovingForward |= (g_Controller0->buttonFlags.held & (ControllerFlag_LStickHighUp | ControllerFlag_LStickHighDown)) == ControllerFlag_LStickHighUp;
        g_Player_IsMovingBackward = (g_Controller0->buttonFlags.held & (ControllerFlag_LStickHighUp | ControllerFlag_LStickHighDown)) == ControllerFlag_LStickHighDown;
        g_Player_HasMoveInput     = g_Controller0->buttonFlags.clicked & (g_GameWorkPtr->config.controllerConfig.stepLeft |
                                                                          (ControllerFlag_LStickHighUp    |
                                                                           ControllerFlag_LStickHighRight |
                                                                           ControllerFlag_LStickHighDown  |
                                                                           ControllerFlag_LStickHighLeft) |
                                                                          g_GameWorkPtr->config.controllerConfig.stepRight |
                                                                          g_GameWorkPtr->config.controllerConfig.aim);
    }

    g_Player_IsSteppingLeftHold  = (g_Controller0->buttonFlags.held & g_GameWorkPtr->config.controllerConfig.stepLeft) &&
                                  !(g_Controller0->buttonFlags.held & g_GameWorkPtr->config.controllerConfig.stepRight);

    g_Player_IsSteppingRightHold = (g_Controller0->buttonFlags.held & g_GameWorkPtr->config.controllerConfig.stepRight) &&
                                  !(g_Controller0->buttonFlags.held & g_GameWorkPtr->config.controllerConfig.stepLeft);

    g_Player_IsSteppingLeftTap  |= (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.stepLeft)  != false;
    g_Player_IsSteppingRightTap |= (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.stepRight) != false;

    if (g_GameWork.config.extraWalkRunCtrl)
    {
        g_Player_IsRunning = !(g_Controller0->buttonFlags.held & g_GameWorkPtr->config.controllerConfig.run);
    }
    else
    {
        g_Player_IsRunning = g_Controller0->buttonFlags.held & g_GameWorkPtr->config.controllerConfig.run;
    }

    if (g_GameWork.config.extraWeaponCtrl)
    {
        g_Player_IsAiming = g_Controller0->buttonFlags.held & g_GameWorkPtr->config.controllerConfig.aim;
    }
    else
    {
        g_Player_IsAiming = g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.aim;
    }

    if (PLAYER_COMBAT().weaponAttack >= WEAPON_ATTACK(EquippedWeaponId_Handgun, AttackInputType_Tap) &&
        PLAYER_EXTRA().lowerBodyState >= PlayerLowerBodyState_Aim)
    {
        g_Player_IsShooting  = g_Controller0->buttonFlags.held & g_GameWorkPtr->config.controllerConfig.action;
        g_Player_IsAttacking = g_Player_IsShooting;
    }
    else
    {
        heldAttackActionFlag = g_Controller0->buttonFlags.held & g_GameWorkPtr->config.controllerConfig.action;

        g_Player_IsHoldAttack = (g_Player_IsHoldAttack * 2) & 0x1F;
        g_Player_IsAttacking  = (g_Player_IsAttacking * 2) & 0x3;
        g_Player_IsShooting   = (g_Player_IsShooting * 2) & 0x3;

        g_Player_IsHoldAttack |= (heldAttackActionFlag & 0xFFFF) != false;
        g_Player_IsAttacking  |= (g_Player_IsHoldAttack & 0xF) == 0xF;

        g_Player_IsShooting |= g_Player_IsHoldAttack != false && !(g_Player_IsHoldAttack & 0x11);

        if (g_Player_IsShooting)
        {
            g_Player_IsHoldAttack = false;
        }
    }

    g_Player_HasActionInput = g_Controller0->buttonFlags.clicked & (g_GameWorkPtr->config.controllerConfig.run |
                                                                    g_GameWorkPtr->config.controllerConfig.action);

    if (g_SysWork.sysState != SysState_Gameplay)
    {
        g_Player_IsShooting   = false;
        g_Player_IsAttacking  = false;
        g_Player_IsHoldAttack = false;
    }

    if (PLAYER_COMBAT().weaponAttack == WEAPON_ATTACK(EquippedWeaponId_HyperBlaster, AttackInputType_Tap))
    {
        switch (Inventory_HyperBlasterFunctionalTest())
        {
            case 0: // Player has weapon (not unlocked).
                g_Player_IsAiming = false;
                break;

            case 1: // Konami gun controller.
                g_Player_IsAiming    = g_Controller1->buttonFlags.held & ControllerFlag_Cross;
                g_Player_IsShooting  = g_Controller1->buttonFlags.held & ControllerFlag_Square;
                g_Player_IsAttacking = g_Player_IsShooting;
                break;

            case 2: // Player has weapon (unlocked).
                break;
        }
    }

    // This conditional makes it impossible to move when aiming with specific weapons.
    if (PLAYER_COMBAT().isAiming && (PLAYER_COMBAT().weaponAttack == WEAPON_ATTACK(EquippedWeaponId_HuntingRifle, AttackInputType_Tap) ||
                                            (PLAYER_COMBAT().weaponAttack < WEAPON_ATTACK(EquippedWeaponId_Handgun, AttackInputType_Tap) &&
                                             (WEAPON_ATTACK_ID_GET(PLAYER_COMBAT().weaponAttack) == EquippedWeaponId_Hammer    ||
                                              WEAPON_ATTACK_ID_GET(PLAYER_COMBAT().weaponAttack) == EquippedWeaponId_RockDrill ||
                                              WEAPON_ATTACK_ID_GET(PLAYER_COMBAT().weaponAttack) == EquippedWeaponId_Katana))))
    {
        if (g_Player_IsMovingForward)
        {
            g_Player_RockDrill_AttackType = RockDrillAttackType_Up;
        }
        else if (g_Player_IsMovingBackward)
        {
            g_Player_RockDrill_AttackType = RockDrillAttackType_Down;
        }
        else
        {
            g_Player_RockDrill_AttackType = RockDrillAttackType_Center;
        }

        g_Player_IsRunning           = false;
        g_Player_IsSteppingRightTap  = false;
        g_Player_IsSteppingRightHold = false;
        g_Player_IsSteppingLeftTap   = false;
        g_Player_IsSteppingLeftHold  = false;
        g_Player_IsMovingBackward    = false;
        g_Player_IsMovingForward     = false;
    }
}

bool func_8007F95C(void) // 0x8007F95C
{
    VECTOR3         pos0;
    VECTOR3         pos1;
    u16             sp30;
    s32             i;
    u16             temp;
    q19_12          radius;
    s_SubCharacter* curNpc1;
    s_SubCharacter* curNpc0;

    if (g_Player_IsInWalkToRunTransition)
    {
        return false;
    }

    pos0.vx = PLAYER_CHARA().position.vx;
    pos0.vy = PLAYER_CHARA().position.vy;
    pos0.vz = PLAYER_CHARA().position.vz;

    if (!PLAYER_COMBAT().isAiming ||
        PLAYER_COMBAT().weaponAttack < WEAPON_ATTACK(EquippedWeaponId_Handgun, AttackInputType_Tap))
    {
        for (i = 0, curNpc0 = g_SysWork.npcs, curNpc1 = g_SysWork.npcs;
             i < ARRAY_SIZE(g_SysWork.npcs);
             i++, curNpc0++, curNpc1++)
        {
            if (curNpc1->model.charaId != Chara_None)
            {
                if (curNpc1->model.charaId == Chara_AirScreamer ||
                    curNpc1->model.charaId == Chara_NightFlutter)
                {
                    radius = Q12(1.2f);
                }
                else
                {
                    if (curNpc1->model.charaId == Chara_Creeper)
                    {
                        radius = Q12(0.65f);
                    }
                    else
                    {
                        radius = Q12(0.85f);
                    }
                }

                pos1.vx = curNpc0->position.vx + curNpc0->collision.shapeOffsets.box.vx;
                pos1.vy = curNpc0->position.vy;
                pos1.vz = curNpc0->position.vz + curNpc0->collision.shapeOffsets.box.vz;

                if (!Math_Distance2dCheck(&pos0, &pos1, radius) && ABS(pos1.vy - pos0.vy) < Q12(0.3f) &&
                    curNpc0->health > Q12(0.0f) && (curNpc0->flags & CharaFlag_Unk2))
                {
                    Math_ShortestAngleGet(PLAYER_CHARA().rotation.vy,
                                          Q12_ANGLE_NORM_U(ratan2(pos1.vx - pos0.vx, pos1.vz - pos0.vz) + Q12_ANGLE(360.0f)),
                                          &sp30);

                    temp = sp30 + Q12_ANGLE(89.98f);
                    if (temp < Q12_ANGLE(202.49f))
                    {
                        g_SysWork.targetNpcIdx = i;
                        return true;
                    }
                }
            }
        }
    }

    g_SysWork.targetNpcIdx = NO_VALUE;
    return false;
}

void Math_ShortestAngleGet(q3_12 angleFrom, q3_12 angleTo, q3_12* shortestAngle) // 0x8007FB34
{
    q3_12 adjAngle;

    if (angleTo > angleFrom)
    {
        if ((angleTo - angleFrom) < Q12_ANGLE(180.0f))
        {
            *shortestAngle = angleTo - angleFrom;
        }
        else
        {
            adjAngle       = angleTo  - Q12_ANGLE(360.0f);
            *shortestAngle = adjAngle - angleFrom;
        }
    }
    else
    {
        if ((angleFrom - angleTo) < Q12_ANGLE(180.0f))
        {
            *shortestAngle = angleTo - angleFrom;
        }
        else
        {
            adjAngle       = angleFrom - Q12_ANGLE(360.0f);
            *shortestAngle = angleTo   - adjAngle;
        }
    }
}

void func_8007FB94(s_SubCharacter* chara, s_PlayerExtra* extra, s32 animStatus) // 0x8007FB94
{
    s32 i;

    if (extra->model.controlState != 0)
    {
        return;
    }

    for (i = 0; i < 40; i++)
    {
        if (g_MapOverlayHdr.field_38[i].status_2 != animStatus)
        {
            continue;
        }

        if (extra->model.stateStep == 0)
        {
            extra->model.anim.status = g_MapOverlayHdr.field_38[i].status;
            extra->model.stateStep++;
        }

        if (chara->model.stateStep == 0)
        {
            chara->model.anim.status = g_MapOverlayHdr.field_38[i].status;
            chara->model.stateStep++;
        }

        D_800AF220 = i;
        i          = 41;

        extra->model.controlState++;
    }
}

void func_8007FC48(s_SubCharacter* chara, s_PlayerExtra* extra, s32 animStatus) // 0x8007FC48
{
    s32 i;

    if (extra->model.controlState != 0)
    {
        return;
    }

    // TODO: 40 of what?
    for (i = 0; i < 40; i++)
    {
        if (g_MapOverlayHdr.field_38[i].status_2 != animStatus)
        {
            continue;
        }

        // Set active anim index.
        extra->model.anim.status = g_MapOverlayHdr.field_38[i].status + 1; // TODO: There's a macro for anim status++.
        chara->model.anim.status = g_MapOverlayHdr.field_38[i].status + 1;

        // Increment state step.
        extra->model.stateStep++;
        chara->model.stateStep++;

        // Set anim time.
        extra->model.anim.time = Q12(g_MapOverlayHdr.field_38[i].time);
        D_800AF220             = i;
        chara->model.anim.time = Q12(g_MapOverlayHdr.field_38[i].time);
        i                      = 41;

        // Increment state.
        extra->model.controlState++;
    }
}

s32 func_8007FD2C(void) // 0x8007FD2C
{
    return PLAYER_PROPS().field_104;
}

q19_12 Game_GasWeaponPowerTimerValue(void) // 0x8007FD3C
{
    return PLAYER_PROPS().gasWeaponPowerTimer;
}

void func_8007FD4C(bool resetColl) // 0x8007FD4C
{
    s32             i;
    s_SubCharacter* player;

    player = &PLAYER_CHARA();

    g_Player_GrabReleaseInputTimer = Q12(0.0f);
    player->field_40               = NO_VALUE;

    PLAYER_PROPS().flags &= ~PlayerFlag_DamageReceived;

    for (i = 0; i < ARRAY_SIZE(g_SysWork.npcIdxs); i++)
    {
        g_SysWork.npcIdxs[i] = NO_VALUE;
    }

    if (resetColl)
    {
        PLAYER_CHARA().collision.cylinder.outerRadius = Q12(0.3f);
        PLAYER_CHARA().collision.cylinder.innerRadius = DEFAULT_PLAYER_CYLINDER_INNER_RADIUS;

        // TODO: Doesn't match?
        //Player_CollisionReset();
        PLAYER_CHARA().collision.box.topExtent            = DEFAULT_PLAYER_BOX_TOP;
        PLAYER_CHARA().collision.shapeOffsets.cylinder.vz = Q12(0.0f);
        PLAYER_CHARA().collision.shapeOffsets.cylinder.vx = Q12(0.0f);
        PLAYER_CHARA().collision.shapeOffsets.box.vz      = Q12(0.0f);
        PLAYER_CHARA().collision.shapeOffsets.box.vx      = Q12(0.0f);
        PLAYER_CHARA().collision.box.bottomExtent         = Q12(0.0f);
        PLAYER_CHARA().collision.box.offsetY              = DEFAULT_PLAYER_BOX_OFFSET_Y;
    }
}

void Player_FootstepSfxGet(s8 floorType, e_SfxId* sfxId, s8* pitch0, s8* pitch1) // 0x8007FDE0
{
    switch (floorType)
    {
        case FloorType_8:
            // @hack Redundant switch needed for match. This allows emitting a dupe `g_SavegamePtr->mapIdx`
            // load in the ASM that goes unused, before setting `*sfxId = Sfx_FootstepUnk1330`.
            // MapIdx cases are just guesses that allow the dupe to be emitted.
            // Unknown whether original code might have had something similar (maybe at one point would set
            // different sfxId values, but was later changed to same one)
            switch (g_SavegamePtr->mapIdx)
            {
                case MapIdx_MAP0_S01:
                case MapIdx_MAP0_S02:
                    *sfxId = Sfx_FootstepUnk1330;
                    break;

                default:
                    *sfxId = Sfx_FootstepUnk1330;
                    break;
            }

            switch (g_SavegamePtr->mapIdx)
            {
                case MapIdx_MAP2_S00:
                    if (PLAYER_CHARA().position.vx >= Q12(95.0f)  && PLAYER_CHARA().position.vx <= Q12(105.0f) &&
                        PLAYER_CHARA().position.vz >= Q12(-33.0f) && PLAYER_CHARA().position.vz <= Q12(-28.0f))
                    {
                        *sfxId = Sfx_Unk1389;
                    }
                    break;
            }
            break;

        case FloorType_Grass:
            *sfxId = Sfx_FootstepGrass;
            break;

        case FloorType_4:
            *sfxId = Sfx_Unk1313;
            break;

        case FloorType_5:
            if (g_SavegamePtr->mapIdx == MapIdx_MAP4_S02)
            {
                *sfxId = Sfx_Unk1543;
            }
            else
            {
                *sfxId = Sfx_Unk1557;
            }
            break;

        case FloorType_6:
        case FloorType_10:
        case FloorType_11:
            *sfxId = Sfx_FootstepMetal;
            break;

        case FloorType_9:
            if (g_SavegamePtr->mapIdx == MapIdx_MAP0_S02)
            {
                *sfxId = Sfx_Unk1388;
            }
            else
            {
                *sfxId = Sfx_FootstepUnk1331;
            }
            break;

        case FloorType_2:
            *sfxId = Sfx_Unk1389;
            break;

        default:
        case FloorType_Default:
        case FloorType_1:
        case FloorType_7:
            *sfxId = Sfx_FootstepConcrete;
            break;
    }

    switch (g_SavegamePtr->mapIdx)
    {
        case MapIdx_MAP6_S02:
            switch (g_SavegamePtr->mapRoomIdx)
            {
                case 20:
                    if (PLAYER_CHARA().position.vy > Q12(0.0f))
                    {
                        *sfxId = Sfx_FootstepMetal1;
                    }
                    break;

                case 21:
                    if (floorType != FloorType_1)
                    {
                        *sfxId = Sfx_FootstepMetal1;
                    }
                    break;
            }
            break;

        case MapIdx_MAP4_S03:
            if ((PLAYER_CHARA().position.vx >= Q12(165.0f)   &&
                 PLAYER_CHARA().position.vz >= Q12(58.5f)    && PLAYER_CHARA().position.vz <= Q12(61.5f)) ||
                (PLAYER_CHARA().position.vx <= Q12(112.1f)   &&
                 PLAYER_CHARA().position.vz >= Q12(-101.45f) && PLAYER_CHARA().position.vz <= Q12(-98.5f)))
            {
                *sfxId = Sfx_Unk1565;
            }

        case MapIdx_MAP6_S00:
            if ((PLAYER_CHARA().position.vx >= Q12(-160.1f)  && PLAYER_CHARA().position.vx <= Q12(-158.5f)  &&
                 PLAYER_CHARA().position.vz >= Q12(26.8f)    && PLAYER_CHARA().position.vz <= Q12(27.4f))   ||
                (PLAYER_CHARA().position.vx >= Q12(-160.1f)  && PLAYER_CHARA().position.vx <= Q12(-158.5f)  &&
                 PLAYER_CHARA().position.vz >= Q12(16.8f)    && PLAYER_CHARA().position.vz <= Q12(17.5f))   ||
                (PLAYER_CHARA().position.vx >= Q12(-170.0f)  && PLAYER_CHARA().position.vx <= Q12(-165.8f)  &&
                 PLAYER_CHARA().position.vz >= Q12(-16.4f)   && PLAYER_CHARA().position.vz <= Q12(-14.35f)) ||
                (PLAYER_CHARA().position.vx >= Q12(-172.7f)  && PLAYER_CHARA().position.vx <= Q12(-170.9f)  &&
                 PLAYER_CHARA().position.vz >= Q12(-24.9f)   && PLAYER_CHARA().position.vz <= Q12(-21.25f)) ||
                (PLAYER_CHARA().position.vx >= Q12(-170.28f) && PLAYER_CHARA().position.vx <= Q12(-165.85f) &&
                 PLAYER_CHARA().position.vz >= Q12(-35.4f)   && PLAYER_CHARA().position.vz <= Q12(-34.35f)))
            {
                *sfxId = Sfx_Unk1600;
            }
            break;

        case MapIdx_MAP6_S01:
            if (g_SavegamePtr->mapRoomIdx == 18)
            {
                *sfxId = Sfx_Unk1608;
            }
            break;

        case MapIdx_MAP6_S04:
            *sfxId = Sfx_FootstepMetal;
            break;
    }

    switch (floorType)
    {
        case FloorType_5:
        case FloorType_6:
        case FloorType_10:
        case FloorType_11:
            *pitch0 = Rng_GenerateInt(-4, 3);
            *pitch1 = Rng_GenerateInt(56, 71);
            break;

        default:
            *pitch0 = Rng_GenerateInt(-16, 15);
            *pitch1 = Rng_GenerateInt(32, 95);
            break;
    }
}

q19_12 Math_DistanceGet(const VECTOR3* from, const VECTOR3* to) // 0x800802CC
{
    q19_12 deltaX;
    q19_12 deltaY;
    q19_12 deltaZ;

    deltaX = to->vx - from->vx;
    deltaY = to->vy - from->vy;
    deltaZ = to->vz - from->vz;
    return SquareRoot12(Q12_MULT_PRECISE(deltaX, deltaX) +
                        Q12_MULT_PRECISE(deltaY, deltaY) +
                        Q12_MULT_PRECISE(deltaZ, deltaZ));
}

q19_12 Math_Distance2dGet(const VECTOR3* from, const VECTOR3* to) // 0x8008037C
{
    q19_12 deltaX;
    q19_12 deltaZ;

    deltaX = to->vx - from->vx;
    deltaZ = to->vz - from->vz;
    return SquareRoot12(Q12_MULT_PRECISE(deltaX, deltaX) +
                        Q12_MULT_PRECISE(deltaZ, deltaZ));
}

void func_800803FC(VECTOR3* pos, s32 idx) // 0x800803FC
{
    q19_12 posX;
    q19_12 posZ;

    posX = g_MapOverlayHdr.charaSpawnInfos[0][idx].positionX;
    posZ = g_MapOverlayHdr.charaSpawnInfos[0][idx].positionZ;

    pos->vx = posX;
    pos->vy = Collision_FloorHeightGet(posX, posZ);
    pos->vz = posZ;
}

void Input_SelectClickSet(void) // 0x80080458
{
    g_Controller1->buttonFlags.clicked |= ControllerFlag_Select;
}

q19_12 func_80080478(const VECTOR3* pos0, const VECTOR3* pos1) // 0x80080478
{
    q19_12 x0;
    q19_12 x1;
    q19_12 y1;
    q19_12 y0;
    q19_12 z0;
    q19_12 z1;
    q19_12 deltaX;
    q19_12 deltaZ;
    u16    atan2Delta;
    s32    unk;

    x0 = pos0->vx;
    x1 = pos1->vx;
    y0 = pos0->vy;
    y1 = pos1->vy;
    z0 = pos0->vz;
    z1 = pos1->vz;

    deltaX     = x1 - x0;
    deltaZ     = z1 - z0;
    atan2Delta = ratan2(deltaX, deltaZ);

    unk = func_8008A058(func_80080540(deltaX, Q12(0.0f), deltaZ));
    return (ratan2(unk, y1 - y0) << 16) | atan2Delta;
}

q19_12 Rng_RandQ12(void) // 0x80080514
{
    s32 rand16;

    rand16 = Rng_Rand16();
    return Q12_FRACT(((rand16 * 2) ^ rand16) >> 3);
}

s32 func_80080540(q19_12 x, q19_12 y, q19_12 z) // 0x80080540
{
    s32 v0;

    __asm__ volatile(
        "mult %0, %0\n"
        "mflo $4\n"
        "mfhi $3\n"
        "srl  $4, $4, 0xc\n"
        "sll  $3, $3, 0x14\n"

        "mult %1, %1\n"
        "or   %3, $3, %0\n"
        "mflo $4\n"
        "mfhi $3\n"
        "srl  $4, $4, 0xc\n"
        "sll  $3, $3, 0x14\n"

        "mult %2, %2\n"
        "or   %1, $3, %0\n"
        "mflo $4\n"
        "mfhi $3\n"
        "srl  $4, $4, 0xc\n"
        "sll  $3, $3, 0x14\n"
        "or   %2, $3, $4\n"

        : "r="(x), "r="(y), "r="(z), "r="(v0)
        : "r"(x), "r"(y), "r"(z));

    return v0 + y + z;
}

s32 Math_PreservedSignSubtract(s32 val, s32 subtractor) // 0x80080594
{
    s32 signBit;
    s32 absDiff;

    signBit = val >> 31;
    absDiff = ((val ^ signBit) - signBit) - subtractor;
    return ((absDiff & ~(absDiff >> 31)) ^ signBit) - signBit;
}

void func_800805BC(VECTOR3* pos, SVECTOR* rot, GsCOORDINATE2* rootCoord, s32 arg3) // 0x800805BC
{
    MATRIX mat;
    VECTOR vec;

    Vw_CoordHierarchyMatrixCompute(rootCoord, &mat);
    gte_SetRotMatrix(&mat);
    gte_SetTransMatrix(&mat);

    while (arg3 > 0)
    {
        gte_ldv0(rot);
        gte_rt();
        gte_stlvnl(&vec);

        pos->vx = Q8_TO_Q12(vec.vx);
        pos->vy = Q8_TO_Q12(vec.vy);
        pos->vz = Q8_TO_Q12(vec.vz);

        arg3--;
        rot++;
        pos++;
    }
}

bool func_800806AC(s32 arg0, q19_12 posX, q19_12 posY, q19_12 posZ) // 0x800806AC
{
    bool result;
    //static s_Collision D_800C4620;

    result = arg0 != 0;
    if (!result)
    {
        return result;
    }

    result = arg0 == NO_VALUE;
    if (result)
    {
        return result;
    }

    Collision_SurfaceGet(&D_800C4620, posX, posZ);

    result = posY < D_800C4620.floorHeight;
    if (result)
    {
        result = D_800C4620.floorType != NO_VALUE;
        if (result)
        {
            result = arg0 & (1 << D_800C4620.floorType);
            return result != false;
        }
    }

    return result;
}

bool func_8008074C(s32 arg0, q19_12 posX, q19_12 posY, q19_12 posZ) // 0x8008074C
{
    return func_800806AC(arg0, posX, INT_MAX + 1, posZ);
}

void Collision_PointCacheSet(q19_12 posX, q19_12 posZ) // 0x8008076C
{
    q19_12              floorHeight;
    s32                 floorType; // `e_FloorType`
    q19_12              collX;
    q19_12              collZ;
    s_CollisionSurface* surface;

    surface = &g_CollisionPointCache.surface;

    collX = g_CollisionPointCache.position.vx;
    collZ = g_CollisionPointCache.position.vz;
    if (g_CollisionPointCache.floorType != NO_VALUE &&
        collX == posX &&
        collZ == posZ)
    {
        return;
    }

    Collision_SurfaceGet(surface, posX, posZ);
    g_CollisionPointCache.position.vx = posX;
    g_CollisionPointCache.position.vz = posZ;

    floorType = surface->floorType;
    switch (surface->floorType)
    {
        case FloorType_Default:
            floorHeight = Q12(8.0f);

            switch (g_SavegamePtr->mapIdx)
            {
                case MapIdx_MAP5_S01:
#if VERSION_EQUAL_OR_NEWER(USA)
                    if (posZ <= Q12(0.0f))
#endif
                    {
                        floorHeight = Q12(4.0f);
#if VERSION_EQUAL_OR_NEWER(USA)
                        floorType = FloorType_7;
#else
                        surface->floorType = FloorType_7;
#endif
                    }
                    break;

                case MapIdx_MAP6_S00:
                    floorHeight = Q12(4.0f);
#if VERSION_EQUAL_OR_NEWER(USA)
                    floorType = FloorType_7;
#else
                    surface->floorType = FloorType_7;
#endif
                    break;
            }
            break;

        case FloorType_None:
            floorHeight = Q12(8.0f);

            switch (g_SavegamePtr->mapIdx)
            {
                case MapIdx_MAP6_S00:
                    floorHeight = Q12(4.0f);
#if VERSION_EQUAL_OR_NEWER(USA)
                    floorType = FloorType_7;
#else
                    surface->floorType = FloorType_7;
#endif
                    break;
            }
            break;

        default:
            floorHeight = surface->floorHeight;
            break;
    }

    g_CollisionPointCache.position.vy = floorHeight;
    g_CollisionPointCache.floorType  = floorType;
}

q19_12 Collision_FloorHeightGet(q19_12 posX, q19_12 posZ) // 0x80080884
{
    Collision_PointCacheSet(posX, posZ);
    return g_CollisionPointCache.position.vy;
}

s32 Collision_FloorTypeGet(q19_12 posX, q19_12 posZ) // 0x800808AC
{
    Collision_PointCacheSet(posX, posZ);
    return g_CollisionPointCache.floorType;
}

s32 Math_MulFixed(s32 val0, s32 val1, s32 shift) // 0x800808D4
{
    u32 lo;

    // Use inline asm to fetch high/low parts of mult.
    // Only method found to allow C to keep same insn/reg order so far.
    __asm__ volatile(
        "mult %0, %1\n" // Multiply `val0` and `val1`.
        "mfhi %0\n"     // Move high result back into `val0`?
        "mflo %2\n"     // Move low result to lo.
        : "=r"(val0), "=r"(val1), "=r"(lo)
        : "0"(val0), "1"(val1));

#if 0
    // Equivalent C version of above (non-matching).
    s64 res = (s64)val0 * (s64)val1;
    val0    = (u32)(res >> 32);
    lo      = (u32)res;
#endif

    return (val0 << (32 - shift)) | (lo >> shift);
}

s32 Math_MagnitudeShiftGet(q19_12 mag) // 0x800808F8
{
    s32 shift;

    if (mag < Q12(4.0f))
    {
        return 0;
    }

    if (mag > (Q12(64.0f) - 1))
    {
        if (mag > (Q12(1024.0f) - 1))
        {
            return Q12_SHIFT;
        }

        shift = Q8_SHIFT;
        return shift;
    }

    shift = Q4_SHIFT;
    return shift;
}
