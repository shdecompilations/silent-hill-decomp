#ifndef _MAPS_MAP1_S02_H
#define _MAPS_MAP1_S02_H

#define HAS_PlayerState_Reset
#define HAS_PlayerState_WalkForward
#define HAS_PlayerState_RunForward
#define HAS_PlayerState_WalkBackward
#define HAS_PlayerState_TurnRight
#define HAS_PlayerState_TurnLeft
#define HAS_PlayerState_Unk59
#define HAS_PlayerState_Unk60
#define HAS_PlayerState_Unk74
#define HAS_PlayerState_Unk94
#define HAS_PlayerState_Unk95
#define HAS_PlayerState_Unk97
#define HAS_PlayerState_Unk98
#define HAS_PlayerState_Unk106
#define KEYFRAME_PlayerState_Unk59 903
#define KEYFRAME_PlayerState_Unk60 918

#define MAP_ROOM_MIN_X Q12(-160.0f)
#define MAP_ROOM_MAX_X Q12(160.0f)
#define MAP_ROOM_MIN_Z Q12(-80.0f)
#define MAP_ROOM_MAX_Z Q12(200.0f)

#define MAP_FIELD_4C_COUNT 200
#define MAP_BLOOD_SPLAT_COUNT_MAX 150

#if VERSION_REGION_IS(NTSC)
    #define MAP_MSG(idx) (idx)
#elif VERSION_REGION_IS(NTSCJ)
    #define MAP_MSG(idx) ((idx) >= 49 ? (idx) - 1 : (idx))
#endif

#include "maps/common/shared.h"

extern u16 D_800E1EDC;

extern u8 D_800E1EE2;

extern s32 D_800E1EE4;

extern u16 D_800E1EE8;

extern s_Pose g_CommonWorldObjectPoses[11]; // 0x800E1EEC

extern s32 D_800E1FD4;

extern u8 D_800E1FD8;

extern DVECTOR D_800E1FDC; // Q19.12

extern q3_12 D_800E1FE0; // Y angle.

extern u8 D_800E1FE2;

extern u8 D_800E1FE3;

extern q3_12 D_800E1FE4;

extern s_WorldObjectPose g_WorldObject_DoorHid;
extern s_WorldObjectPose g_WorldObject_Ring1Hi;
extern s_WorldObjectPose g_WorldObject_Ring2Hi;
extern s_WorldObjectPose g_WorldObject_Door2Hi;
extern s_WorldObjectPose g_WorldObject_Door1Hi;

extern s_WorldObjectPlacement g_WorldObject_BoxHide;

extern s_WorldObjectModel g_WorldObjectC;

extern VECTOR3 g_ObjPosC[2];
extern SVECTOR g_ObjRotC[2];

extern s_WorldObjectPose g_WorldObject_GunHide;

extern s_WorldObjectPlacement  g_WorldObject_BollHid;

extern s_WorldObjectPose g_WorldObject_KeyHide;
extern s_WorldObjectPose g_WorldObject_Phone3H;
extern s_WorldObjectPose g_WorldObject_Phone2H;
extern s_WorldObjectPose g_WorldObjectB;
extern s_WorldObjectPose g_WorldObject_KeyHide1;

extern u8 D_800E5A98;
extern u8 D_800E5A99;

extern s_WorldObjectModel g_CommonWorldObjects[6];

void MapEvent_CommonItemTake(void);

/** @brief Handles the Lobby Key use event. */
void MapEvent_LobbyKeyUse(void);

void func_800DA384(void);

void func_800DA8F8(void);

void func_800DA9D4(void);

void func_800DAA00(void);

/** @brief Game demo/trial end event. Shows "COMING SOON" image. */
void MapEvent_GameTrialOver(void);

void MapEvent_DoorWithHorizontalSlotInteract(void);

void func_800DB058(void);

void func_800DB310(void);

void func_800DB33C(void);

void MapEvent_CutscenePhoneCall(void);

void func_800DBB7C(void);

void func_800DBF88(void);

void func_800DBFC8(void);

void func_800DC1E0(void);

void func_800DCF00(void);

void MapEvent_ClassroomKeyItemTake(void);

void func_800DD420(void);

void Map_WorldObjectsInit(void);

void Map_WorldObjectsUpdate(void);

void func_800DEC88(void);

#endif
