#ifndef _MAPS_MAP2_S00_H
#define _MAPS_MAP2_S00_H

#define HAS_PlayerState_Reset
#define HAS_PlayerState_Unk59
#define HAS_PlayerState_Unk60
#define KEYFRAME_PlayerState_Unk59 946
#define KEYFRAME_PlayerState_Unk60 961

#define HAS_PARTICLE_CASE_1
#define HAS_PARTICLE_CASE_9
#define HAS_PARTICLE_CASE_10
#define HAS_PARTICLE_CASE_11
#define PARTICLE_CASE_COUNT 4

#define MAP_ROOM_MIN_X Q12(-320.0f)
#define MAP_ROOM_MAX_X Q12(240.0f)
#define MAP_ROOM_MIN_Z Q12(-240.0f)
#define MAP_ROOM_MAX_Z Q12(400.0f)
#define MAP_HAS_SECONDARY_GRID

#define MAP_FIELD_4C_COUNT 100
#define MAP_BLOOD_SPLAT_COUNT_MAX 50

// JAP0 merges the messages 52 and 53 into one, so messages afterward are all indexed one lower than in USA.
#if VERSION_REGION_IS(NTSC)
    #define MAP_MSG(idx) (idx)
#elif VERSION_REGION_IS(NTSCJ)
    #define MAP_MSG(idx) ((idx) >= 53 ? (idx) - 1 : (idx))
#endif

#include "maps/common/shared.h"

extern u16 g_Cutscene_MapMsgAudioCmds[2]; // 0x800F1A10

extern s16 g_Gfx_LockTimFileIdxs[8]; // 0x800F1A14

extern s32 D_800F1A24;

extern s_Pose  g_CommonWorldObjectPoses[32]; // 0x800F1A28

extern s_MapPoint2d D_800F1CAC[3][32];

extern u8 D_800F1CA8[3]; // = { 0, 1, 2 }

/** Alpha value? */
extern q3_12 D_800F228C;

/** `bool` */
extern u8 D_800F228E;

/** Paper map marking fade alpha. Used to fade in the marking sprite to half opacity. */
extern s32 g_Gfx_PaperMapMarkingAlpha; // 0x800F2290

extern u8 g_Cutscene_MapMsgAudioIdx; // 0x800F2294

extern u8 D_800F2295;

extern s32 D_800F2298;

/** Paper map zoom counter or timer. */
extern s32 D_800F229C;

extern s_FsImageDesc* D_800F22A0;
extern s_FsImageDesc* D_800F22A4;
extern s_FsImageDesc* D_800F22A8;

extern s16 g_DoorOfEclypse_MapMsgIdx; // 0x800F22AC

extern s16 D_800F22AE;

/* TODO: Rename these with sequental names (not really essential, but would be nice). */
extern SVECTOR3 D_800F5344;
extern s8       D_800F534C;

/** Time. */
extern q3_12 D_800F534E;

extern s16 D_800F5350;

extern s_WorldObjectModel        D_800F535C;
extern VECTOR3                D_800F537C;
extern SVECTOR3               D_800F538C;
extern s_WorldObjectPose      g_WorldObject_IronPipe;
extern s_WorldObjectPose      g_WorldObject1;
extern s_WorldObjectPose      g_WorldObject2;
extern s_WorldObjectPose      g_WorldObject3;
extern s_WorldObjectPlacement g_WorldObjectB[3];
extern s_WorldObjectPose      g_WorldObject4[3];
extern s_WorldObjectModel        g_WorldObject5[4];
extern VECTOR3                D_800F55DC[2];
extern s_WorldObjectModel        g_WorldObject6[3];
extern s_WorldObjectModel        g_WorldObject7[4];
extern VECTOR3                D_800F56CC[2];
extern u8                     D_800F56E4;
extern s_WorldObjectModel        D_800F56EC;
extern s_Pose      D_800F570C;
extern s_Pose      D_800F572C;
extern s_Pose      D_800F574C;
extern s_WorldObjectPose      g_WorldObject8;
extern s_WorldObjectPose      g_WorldObject_RockDrill;
extern s_WorldObjectPose      g_WorldObject_Chainsaw;
extern s_WorldObjectModel        g_CommonWorldObjects[6];

void func_800D756C(s_SubCharacter* chara);

void func_800D794C(s_SubCharacter* chara);

void func_800DD588(s_SubCharacter* chara);

void func_800DD834(s_SubCharacter* chara);

void func_800E7B74(void);

void func_800E7D54(void);

void MapEvent_AtWaterWorks(void);

void func_800E816C(void);

void func_800E8198(void);

/** @brief Handles the ? map pickup event. */
void MapEvent_PaperMapTake(void);

void MapEvent_SteelPipeTake(void);

void MapEvent_CutsceneExitCafe(void);

void MapEvent_CherylsSketchbook(void);

void func_800E9470(void);

void MapEvent_DoghouseNote(void);

void MapEvent_DoghouseKeyTake(void);

void func_800E9A0C(void);

void func_800E9A74(void);

void func_800E9CB4(void);

/** @brief Handles the House Key item use event. */
void MapEvent_HouseKeyUse(void);

void func_800E9DD8(void);

void MapEvent_DoorOfEclipseEnter(void);

void func_800EA6E0(void);

void MapEvent_KGordonKeyUse(void);

void func_800EA960(void);

void func_800EAD2C(void);

void func_800EB090(void);

void func_800EB174(void);

void func_800EB3F4(void);

void func_800EB758(void);

void func_800EB79C(void);

void func_800EB7E0(void);

void func_800EB824(s32);

void Map_WorldObjectsInit(void);

void Map_WorldObjectsUpdate(void);

/** Index getter. */
s32 func_800EE518(void);

s32 func_800EE5A8(u32 arg0);

void func_800EE5D0(void);

void func_800EE660(void);

#endif
