#ifndef _MAPS_MAP4_S04_H
#define _MAPS_MAP4_S04_H

#define HAS_PlayerState_Unk51
#define HAS_PlayerState_Unk131
#define HAS_PlayerState_Reset
#define HAS_PlayerState_WalkForward
#define HAS_PlayerState_Unk117
#define HAS_PlayerState_Unk134
#define HAS_PlayerState_Unk135
#define KEYFRAME_PlayerState_Unk117 678

#define HAS_Chara_Lisa
#define MAP_CHARA_COUNT 1

#define MAP_ROOM_MIN_X Q12(-160.0f)
#define MAP_ROOM_MAX_X Q12(160.0f)
#define MAP_ROOM_MIN_Z Q12(-160.0f)
#define MAP_ROOM_MAX_Z Q12(160.0f)

#define MAP_FIELD_4C_COUNT 200
#define MAP_BLOOD_SPLAT_COUNT_MAX 150

#include "maps/common/shared.h"

extern RECT D_800D3710[2];

extern RECT D_800D3720;

extern DVECTOR D_800D3728[2];

extern DVECTOR D_800D3730;

extern u16 D_800D3734;

extern u16 D_800D3778;

extern u8 D_800D37C0;

extern u8 D_800D37C1;

extern VECTOR3 g_Cutscene_CameraPositionTarget;

extern VECTOR3 g_Cutscene_CameraLookAtTarget;

extern q19_12 g_Cutscene_Timer;

extern u8 D_800D6EF8;

extern s_WorldObjectPose      g_WorldObjectSavepad; // 0x800D6F38
extern s_WorldObjectPlacement g_WorldObject0;       // 0x800D6F08

void func_800D1870(void);

void func_800D1910(void);

void func_800D23E4(void);

void Map_WorldObjectsInit(void);

void Map_WorldObjectsUpdate(void);

#endif
