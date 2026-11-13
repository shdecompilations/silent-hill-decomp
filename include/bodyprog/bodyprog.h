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
#include "bodyprog/world/lighting.h"
#include "bodyprog/world/map.h"
#include "main/fsqueue.h"

#include "bodyprog/items/item_screens.h"
#include "bodyprog/gfx/materials.h"

extern s_FsImageDesc g_MainImg0; // 0x80022C74 - TODO: Part of main exe, move to `main/` headers?

extern char D_80028544[16];

extern u_Filename D_8002B2CC;

#if VERSION_REGION_IS(PAL)
    extern e_FsFile g_MapLanguageBaseIds[];
    extern e_FsFile g_ItemLanguageFileIds[];
#endif

/** Relative file offset for map texture? */
extern s8 D_800A99B5;

extern void (*g_SysStateFuncs[])(void);

extern RECT D_800A9A6C; // `RECT<320, 256, 160, 240>`, only used in `SysState_Fmv_Update`?

extern u32 D_800A9FB0;

extern s32 D_800A9FB4[];

extern u8 g_Sd_SongsChannelsForLayers[41][16];

extern u8 D_800AE185;

extern u8 D_800AE186;

extern const char* g_ItemDescriptions[];

extern s_FsImageDesc D_800A90A4;

extern s_FsImageDesc D_800A90B4;

extern s_SubCharacter D_800BA00C; // Often passed to `Los_NpcToPlayerHitCheck`, might not be full `s_SubCharacter`?

extern u32 D_800A999C;

extern s32 g_Bgm_LayersUpdated;

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

extern s32 D_800C48F0;

extern const s_MapOverlayHdr g_MapOverlayHdr;

extern s16 SQRT[100];

// TODO: Move to `bodyprog/chara/anim.h`. Getting weird errors.
// ========================================

void Anim_BonesInit(s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

void Anim_BonesUpdate(s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords, s32 keyframe0, s32 keyframe1, q19_12 alpha);

/** @brief Gets the active animation info of a model, accounting for overflow map animations.
 *
 * @param anim Model animation data.
 */
s_AnimInfo* Anim_InfoGet(s_ModelAnim* anim);

/** @brief Plays a character's animation using the callback function of its active animation.
 *
 * @param chara Character to update.
 * @param anmHdr Character animation data.
 * @param boneCoords Character model bone coords.
 */
void Anim_CharaUpdate(s_SubCharacter* chara, s_AnmHeader* anmHdr, GsCOORDINATE2* coords);

/** @brief Gets the duration of an animation. A duration can be constant or variable.
 *
 * @param model Model passed to `duration.variableFunc`.
 * @param animInfo Animation info from which to get the duration.
 * @return Animation duration.
 */
q19_12 Anim_DurationGet(s_Model* model, s_AnimInfo* animInfo);

/** @brief Plays an animation once and clamps to the end keyframe, triggering a status transition to the linked
 * animation upon completion.
 *
 * @note Used for one-shot actions such as stopping and attacking.
 *
 * @param model Character model to animate.
 * @param anmHdr Character animation data.
 * @param boneCoords Character model bone coords.
 * @param animInfo Active character animation info.
 */
void Anim_PlayOnce(s_Model* model, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords, s_AnimInfo* animInfo);

/** @brief Loops an animation continuously, wrapping the time around to the start and end keyframes without
 * triggering a status transition.
 *
 * @note Used for looped actions such as running and walking.
 *
 * @param model Character model to animate.
 * @param anmHdr Character animation data.
 * @param boneCoords Character model bone coords.
 * @param animInfo Character animation info.
 */
void Anim_PlayLoop(s_Model* model, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords, s_AnimInfo* animInfo);

/** @brief Linearly interpolates between the current pose into a new target animation, triggering a status transition
 * to the linked animation when complete.
 *
 * @note Used as the entry transition for almost every new animation status.
 *
 * @param model Character model to animate.
 * @param anmHdr Character animation data.
 * @param boneCoords Character model bone coords.
 * @param animInfo Character animation info.
 */
void Anim_BlendLinear(s_Model* model, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords, s_AnimInfo* animInfo);

/** @brief @unused? Smoothly blends between the current pose into a new target animation using a sine-based ease-out
 * without triggering a status transition.
 *
 * @param model Character model to animate.
 * @param anmHdr Character animation data.
 * @param boneCoords Character model bone coords.
 * @param animInfo Character animation info.
 */
void Anim_BlendEaseOut(s_Model* model, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords, s_AnimInfo* animInfo);

#endif
