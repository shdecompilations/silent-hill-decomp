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
#include "bodyprog/events/bgm_update.h"
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
#include "bodyprog/sound/sfx_id_enum.h"
#include "bodyprog/sys/map_info.h"
#include "bodyprog/sys/settings_reset.h"
#include "bodyprog/text/map_msg.h"
#include "bodyprog/view/vw_system.h"
#include "bodyprog/world/draw.h"
#include "bodyprog/world/lighting.h"
#include "bodyprog/world/map.h"
#include "main/fsqueue.h"

#include "bodyprog/gfx/materials.h"

/** @brief Struct used by many functions involved with GTE. Kept at `PSX_SCRATCH_ADDR` (possibly only temporarily). */
typedef struct _GteScratchData
{
    /* 0x0   */ DVECTOR screenXy_0[90];
    /* 0x168 */ u16     screenZ_168[18];
    /* 0x18C */ s16     field_18C[99]; // The size changed from 150 due to the addition of `field_252`. Not sure if this is correct.
    /* 0x252 */ u8      field_252[102];
    /* 0x2B8 */ u8      field_2B8[200]; // Size likely incorrect.

                union
                {
                    MATRIX field_0;
                    struct
                    {
                        /* 0x0  */ s32     field_0;
                        /* 0x4  */ s32     field_4;
                        /* 0x8  */ CVECTOR field_8;
                        /* 0xC  */ CVECTOR field_C;
                        /* 0x10 */ u8      field_10; // `faceIdxs`.
                        /* 0x11 */ u8      field_11;
                        /* 0x12 */ u8      field_12;
                        /* 0x13 */ u8      field_13;
                        /* 0x14 */ u8      field_14;
                        /* 0x15 */ u8      field_15;
                        /* 0x16 */ u8      field_16;
                        /* 0x17 */ u8      field_17;
                        /* 0x18 */ s32     field_18;
                        /* 0x1C */ s32     field_1C;
                    } s_0;
    /* 0x380 */ } field_380;

    /* 0x3A0 */ s_Normal field_3A0;
    /* 0x3A4 */ DVECTOR  screenPos_3A4;
    /* 0x3A8 */ s32      depthP_3A8;
    /* 0x3AC */ SVECTOR  field_3AC;
    /* 0x3B4 */ u8       unk_3B4[36];
    /* 0x3D8 */ CVECTOR  field_3D8; // Written by `func_8005A42C`.
    /* 0x3DC */ DVECTOR  screenPos_3DC;
    /* 0x3E0 */ s32      depthP_3E0;
    /* 0x3E4 */ s16      rotMatrix_3E4[3][3]; // Truncated `MATRIX` without the `long t[3];` transfer vector?
} s_GteScratchData;

typedef struct
{
    /* 0x0   */ DVECTOR  screenXy_0[90];
    /* 0x168 */ s16      screenZ_168[18];
    /* 0x18C */ s16      field_18C[72];
    /* 0x21C */ s32      field_21C[39]; // Used as `VECTOR3`?
    /* 0x2B8 */ u8       field_2B8[200];
    /* 0x380 */ MATRIX   field_380;
    /* 0x3A0 */ s_Normal field_3A0;
    /* 0x3A4 */ DVECTOR  screenPos_3A4;
    /* 0x3A8 */ s32      depthP_3A8;
    /* 0x3AC */ SVECTOR  field_3AC;
    /* 0x3B4 */ u8       unk_3B4[36];
    /* 0x3D8 */ CVECTOR  field_3D8;

                 // Different functions access different data at 0x3DC onwards.
                 // Union works for it, but also possible those functions just took different `s_GteScratchData` structs.
                 union
                 {
                     struct
                     {
                         /* 0x3DC */ DVECTOR screenPos_3DC;
                         /* 0x3E0 */ s32     depthP_3E0;
                         /* 0x3E4 */ s16     rotMatrix_3E4[3][3]; // Truncated `MATRIX` without the `long t[3];` transfer vector?
                     } vertex;

                     struct
                     {
                         /* 0x3DC */ s_Normal field_3DC;
                         /* 0x3E0 */ SVECTOR  field_3E0[3];
                     } normal;

                     struct
                     {
                         /* 0x0 */ u8  field_0;
                         /* 0x1 */ u8  field_1;
                         /* 0x2 */ u8  field_2;
                         /* 0x3 */ u8  field_3;
                         /* 0x4 */ u8  field_4;
                         /* 0x5 */ u8  field_5;
                         /* 0x6 */ u8  field_6;
                         /* 0x7 */ u8  field_7;
                         /* 0x8 */ s32 field_8;
                     } s_1;
    /* 0x3DC */ } u;
} s_GteScratchData2;

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

extern const char* INV_ITEM_NAMES[];

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

/** @brief Draws a palette of colors in the frame buffer. */
void func_8003652C(void);

void func_80032D1C(void);

/** @brief Frees a character model.
 *
 * @param model Character model to release.
 */
void Chara_ModelFree(s_CharaModel* model);

// TODO: Move to `bodyprog/chara/anim.h`.
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
 * @param unused @unused
 * @param animInfo Animation info from which to get the duration.
 * @return Animation duration.
 */
q19_12 Anim_DurationGet(s_Model* unused, s_AnimInfo* animInfo);

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

/** @brief Smoothly blends between the current pose into a new target animation using a sine-based ease-out
 * without triggering a status transition.
 *
 * @unused?
 *
 * @param model Character model to animate.
 * @param anmHdr Character animation data.
 * @param boneCoords Character model bone coords.
 * @param animInfo Character animation info.
 */
void Anim_BlendEaseOut(s_Model* model, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords, s_AnimInfo* animInfo);

// =========================

/** @brief Attaches a weapon model to the player's hand.
 *
 * @param weaponAttack Packed weapon attack. See `WEAPON_ATTACK`.
 */
void Gfx_PlayerHeldItemAttach(u8 weaponAttack);

void func_8005487C(s32 arg0);

void func_80054A04(u8 arg0);

bool Gfx_PickupItemAnimate(u8 itemId);

/** @brief Computes the ammo needed to reload the equipped gun.
 *
 * @param currentAmmo Current amount of loaded ammunition.
 * @param availableAmmo Ammount of available ammunition.
 * @param gunIdx Index of the gun being reloaded (`e_AttackType`).
 */
void Items_AmmoReloadCompute(s32* currentAmmo, s32* availableAmmo, u8 gunIdx); // 0x80054FC0

// `bodyprog/gfx/bodyprog_80056D8C.h`
//======================

/** @brief Draws a 2D fog overlay quad. */
void Gfx_FogOverlayQuadDraw(s16 x0, s16 y0, s16 x1, s16 y1, s32 arg4, s32 arg5, GsOT* ot, s32 otShift);

/** Draws 3D IPD chunk models? */
void func_80057090(s_ModelInfo* modelInfo, GsOT* otTag, s32 otShift, MATRIX* viewMat, MATRIX* worldMat, u16 clutY);

s32 func_800571D0(u32 arg0);

void WorldEnvRender_LightTransform(MATRIX* worldMat, q19_12 alpha, SVECTOR* arg2, VECTOR3* arg3);

void func_80057344(s_ModelInfo* modelInfo, GsOT_TAG* otTag, s32 otShift, MATRIX* mat);

void func_800574D4(s_MeshHeader* meshHdr, s_GteScratchData* scratchData);

void func_8005759C(s_MeshHeader* meshHdr, s_GteScratchData* scratchData, s32 vertOffset, s32 normalOffset);

void func_80057658(s_MeshHeader* meshHdr, s32 offset, s_GteScratchData* scratchData, SVECTOR3* arg3, SVECTOR* arg4);

void func_80057A3C(s_MeshHeader* meshHdr, s32 offset, s_GteScratchData* scratchData, SVECTOR3* lightVec);

void func_80057B7C(s_MeshHeader* meshHdr, s32 offset, s_GteScratchData* scratchData, MATRIX* mat);

/** Main quad drawing func? */
void Gfx_MeshDraw(s_MeshHeader* meshHdr, s_GteScratchData* scratchData, GsOT_TAG* tag, s32 arg3);

/** TODO: Unknown `arg3` type. */
void func_80059D50(s32 arg0, s_ModelInfo* modelInfo, MATRIX* viewMat, s32 otShift, GsOT_TAG* tag);

void func_80059E34(u32 arg0, s_MeshHeader* meshHdr, s_GteScratchData* scratchData, s32 otShift, GsOT_TAG* tag);

void func_8005A21C(s_ModelInfo* modelInfo, GsOT_TAG* otTag, s32 otShift, MATRIX* viewMat);

/** @brief Computes a fog-shaded version of `D_800C4190` color using `arg1` as the distance factor?
 *  Stores the result at 0x3D8 into `arg0`.
 */
void func_8005A42C(s_GteScratchData* scratchData, q19_12 alpha);

void func_8005A478(s_GteScratchData* scratchData, q19_12 alpha);

/** `scratchData` is unused. */
void func_8005A838(s_GteScratchData* scratchData, q19_12 scale);

void func_8005A900(s_MeshHeader* meshHdr, s32 offset, s_GteScratchData* scratchData, MATRIX* viewMat);

u8 func_8005AA08(s_MeshHeader* meshHdr, s32 arg1, s_GteScratchData2* scratchData);

void func_8005AC50(s_MeshHeader* meshHdr, s_GteScratchData2* scratchData, GsOT_TAG* ot, bool otShift);

// events_2d_draw_utils
// =========================
void Gfx_CursorDraw(s32 x0, s16 y0, s32 x1, s16 y1, s16 u, s16 v, s16 width, s32 height, s32 tint,
                    u32 clutX, s16 clutY, s32 tPage);

/** @brief Draws a set of expanding boxes used to highlight a point on a paper map.
 *
 * @param progressAlpha Progress weight.
 * @param startX X start corner (top-left).
 * @param startY Y start corner (top-left).
 * @param startWidth Start width.
 * @param startHeight Start height.
 * @param endX X end corner (bottom-right).
 * @param endY Y end corner (bottom-right).
 * @param endWidth End width.
 * @param endHeight End height.
 */
void PaperMap_ExpandingBoxesDraw(q3_12 progressAlpha,
                                 q3_12 startX, q3_12 startY, q3_12 startWidth, q3_12 startHeight,
                                 q3_12 endX, q3_12 endY, q3_12 endWidth, q3_12 endHeight);

#endif
