#ifndef _MAPS_MAP4_S05_H
#define _MAPS_MAP4_S05_H

#define HAS_PlayerState_Reset
#define HAS_PlayerState_WalkForward
#define HAS_PlayerState_RunForward
#define HAS_PlayerState_WalkBackward
#define HAS_PlayerState_TurnRight
#define HAS_PlayerState_TurnLeft
#define HAS_PlayerState_Unk109

#define MAP_ROOM_MIN_X Q12(-240.0f)
#define MAP_ROOM_MAX_X Q12(240.0f)
#define MAP_ROOM_MIN_Z Q12(-120.0f)
#define MAP_ROOM_MAX_Z Q12(240.0f)

#define MAP_FIELD_4C_COUNT 250
#define MAP_BLOOD_SPLAT_COUNT_MAX 150

#include "maps/common/shared.h"

extern s_AnimInfo FLOATSTINGER_ANIM_INFOS[];

extern VECTOR3 g_Cutscene_CameraPositionTarget;
extern VECTOR3 g_Cutscene_CameraLookAtTarget;

extern q19_12 g_Cutscene_Timer;

extern s_FsImageDesc D_800D7D64;

extern s32 D_800D7D6C;

extern s16 D_800D7D70;

extern u16 D_800D7D74;

extern VECTOR3 D_800D7D88;

extern u8 D_800D7D94;

extern u8 D_800D7D95;

extern s32 D_800D7D6C;

extern s16 D_800D7D80;

extern u8 D_800D7D82;

extern s_WorldObjectPlacement g_WorldObject_Kidn04;
extern s_WorldObjectPlacement g_WorldObject_Kidn05;

extern u8 D_800DB9B0;

void func_800D61A4(void);

void func_800D61AC(void);

void func_800D6800(void);

void func_800D6BC0(void);

void Map_WorldObjectsInit(void);

void Map_WorldObjectsUpdate(void);

#endif
