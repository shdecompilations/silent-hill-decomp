#ifndef _BODYPROG_BODYPROG_H
#define _BODYPROG_BODYPROG_H

// TODO: Includes will have to be cleaned up once everything is organised project-wide and this header becomes
// deprecated.
#include "bodyprog/game.h"
#include "bodyprog/sound/sfx.h"
#include "bodyprog/bodyprog_math_8005BF38.h"
#include "bodyprog/chara/anim.h"
#include "bodyprog/chara/combat.h"
#include "bodyprog/chara/model.h"
#include "bodyprog/chara/spawn.h"
#include "bodyprog/collision/collision.h"
#include "bodyprog/events/events_2d_draw_utils.h"
#include "bodyprog/events/events_util.h"
#include "bodyprog/events/game_sys_states.h"
#include "bodyprog/events/title.h"
#include "bodyprog/formats/dms.h" // Needed here ATM for some maps.
#include "bodyprog/formats/ipd.h"
#include "bodyprog/formats/lm.h"
#include "bodyprog/formats/model.h"
#include "bodyprog/formats/texture.h"
#include "bodyprog/formats/tmd.h"
#include "bodyprog/gfx/bodyprog_80055028.h"
#include "bodyprog/gfx/bodyprog_80056D8C.h"
#include "bodyprog/gfx/env_effects.h"
#include "bodyprog/gfx/lens_flare.h"
#include "bodyprog/gfx/texture_utils.h"
#include "bodyprog/gfx/world.h"
#include "bodyprog/gfx/world_object.h"
#include "bodyprog/items/utils.h"
#include "bodyprog/items/weapon_data.h"
#include "bodyprog/libkpad/vibration.h"
#include "bodyprog/map/map.h"
#include "bodyprog/map/terrain.h"
#include "bodyprog/paper_map/paper_map.h"
#include "bodyprog/paper_map/paper_map_image.h"
#include "bodyprog/player/player_control.h"
#include "bodyprog/player/weapon_anim_data.h"
#include "bodyprog/screen/data.h"
#include "bodyprog/sound/sfx_ids.h"
#include "bodyprog/sys/map_info.h"
#include "bodyprog/sys/settings_reset.h"
#include "bodyprog/text/map_msg.h"
#include "bodyprog/view/vw_system.h"
#include "bodyprog/world/draw.h"
#include "bodyprog/world/light_overlap.h"
#include "bodyprog/world/map.h"
#include "main/fs_queue.h"

#include "bodyprog/items/item_screens.h"
#include "bodyprog/gfx/materials.h"

// TODO: Remaining externs here are all unmigrated data.

extern char D_80028544[16];

extern u_Filename D_8002B2CC;

extern s8 D_800C15B4;

extern s8 D_800C39A0;

/** Functions from `bodyprog_8003AB28` access `g_WorldEnvRenderWork` as constant, but some functions from `bodyprog_80055028` write to it.
 * It appears that g_WorldEnvRenderWork is intended to be defined inside `bodyprog_80055028` as writable and declared as read-only (`const`) outside of it.
 */
// extern s_WorldEnvRenderWork g_WorldEnvRenderWork;

extern q4_12 g_Player_RotationDeltaToTargetY;
extern q4_12 g_Player_RotationDeltaToTargetX;

extern q3_12 D_800C4454;

extern s_CollisionSurface D_800C4620;

extern s16 SQRT[100];

// TODO: Defined per map.
extern const s_MapOverlayHdr g_MapOverlayHdr;

#endif
