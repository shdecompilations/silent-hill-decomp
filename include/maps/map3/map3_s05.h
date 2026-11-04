#ifndef _MAPS_MAP3_S05_H
#define _MAPS_MAP3_S05_H

#define HAS_PlayerState_Reset
#define HAS_PlayerState_WalkForward
#define HAS_PlayerState_RunForward
#define HAS_PlayerState_WalkBackward
#define HAS_PlayerState_TurnRight
#define HAS_PlayerState_TurnLeft
#define HAS_PlayerState_Unk59
#define HAS_PlayerState_Unk60
#define HAS_PlayerState_Unk69
#define HAS_PlayerState_Unk105
#define HAS_PlayerState_Unk81
#define HAS_PlayerState_Unk116
#define HAS_PlayerState_Unk141
#define KEYFRAME_PlayerState_Unk59 957
#define KEYFRAME_PlayerState_Unk60 972

#define MAP_ROOM_MIN_X Q12(-160.0f)
#define MAP_ROOM_MAX_X Q12(160.0f)
#define MAP_ROOM_MIN_Z Q12(-160.0f)
#define MAP_ROOM_MAX_Z Q12(160.0f)

#define MAP_FIELD_4C_COUNT 450
#define MAP_BLOOD_SPLAT_COUNT_MAX 150

#include "maps/common/shared.h"

extern u8 D_800DD528;

extern s16 D_800DD52A; // Time.

extern u16 D_800DAC70;

extern u8 D_800DAC74;

extern s16 D_800DACF0;

extern s16 D_800DACE8;

extern VECTOR3 D_800DD420;

extern VECTOR3 D_800DD430;

extern SVECTOR3 D_800DD440;

extern SVECTOR3 D_800DD448;

extern s16 D_800DD52A;

extern s32 D_800DACEC;

extern s16 D_800DD190;

extern s_WorldObjectPose      g_WorldObject0;
extern s_WorldObjectPlacement    g_WorldObject_Dr[6];
extern s_WorldObjectPose      g_WorldObject2;
extern s_WorldObjectPose      g_WorldObject3;
extern s_WorldObjectPose      g_WorldObject4;
extern s_WorldObjectModel        g_WorldObject5;
extern s_WorldObjectModel        g_WorldObject6;
extern s_WorldObjectPose      g_WorldObject7;
extern s_WorldObjectModel   g_CommonWorldObjects[6];
extern s_Pose g_CommonWorldObjectPoses[4];
extern VECTOR3 D_800DACD4;

void func_800D57E8(void);

void func_800D5934(void);

void func_800D59D4(void);

void func_800D5A98(void);

void func_800D5C98(void);

void func_800D5FC4(void);

void func_800D63C4(void);

void func_800D64E0(void);

void func_800D6BB4(void);

void func_800D6BE0(void);

void func_800D6CF0(void);

void func_800D6D1C(void);

void func_800D719C(void);

void func_800D7280(void);

void Map_WorldObjectsInit(void);

void Map_WorldObjectsUpdate(void);

void func_800D72AC(void);

#endif
