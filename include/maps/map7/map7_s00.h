#ifndef _MAPS_MAP7_S00_H
#define _MAPS_MAP7_S00_H

#define HAS_PlayerState_Reset
#define HAS_PlayerState_WalkForward
#define HAS_PlayerState_RunForward
#define HAS_PlayerState_WalkBackward
#define HAS_PlayerState_TurnRight
#define HAS_PlayerState_TurnLeft
#define HAS_PlayerState_Unk135
#define HAS_PlayerState_Unk136
#define HAS_PlayerState_Unk137
#define HAS_PlayerState_Unk138
#define HAS_PlayerState_Unk139

#define HAS_Chara_Lisa
#define MAP_CHARA_COUNT 1

// Config used by `Map_RoomIdxGet`.
#define MAP_ROOM_MIN_X Q12(-200.0f)
#define MAP_ROOM_MAX_X Q12(200.0f)
#define MAP_ROOM_MIN_Z Q12(-160.0f)
#define MAP_ROOM_MAX_Z Q12(0.f)

#define MAP_FIELD_4C_COUNT 200
#define MAP_BLOOD_SPLAT_COUNT_MAX 150

#include "maps/common/shared.h"

extern s32 D_800D4370[4]; // Z position table?

extern SVECTOR3 D_800CB61C;
extern s16 D_800D4362;

extern s_WorldObjectModel   g_WorldObject0[6];
extern s_WorldObjectPose g_WorldObject1;

extern s16 D_800D4362;
extern u8 g_Cutscene_MapMsgAudioIdx;
extern VECTOR3 g_Cutscene_CameraPosition;
extern VECTOR3 g_Cutscene_CameraLookAt;
extern q19_12 g_Cutscene_Timer;
extern s8 D_800D31D8;

extern u16 g_Cutscene_MapMsgAudioCmds[24];

extern u16 D_800D31C4;
extern s16 D_800D31DC[];

void func_800CFFE8(bool arg0);

void func_800D001C(s32 arg0);

void func_800D0050(void);

void func_800D00D0(void);

void func_800D0AC4(void);

void func_800D0B64(void);

void func_800D1604(void);

void Map_WorldObjectsInit(void);

void Map_WorldObjectsUpdate(void);

#endif
