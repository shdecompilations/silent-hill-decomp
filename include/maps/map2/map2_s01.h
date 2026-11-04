#ifndef _MAPS_MAP2_S01_H
#define _MAPS_MAP2_S01_H

#define HAS_PlayerState_Unk51
#define HAS_PlayerState_Unk131
#define HAS_PlayerState_Reset
#define HAS_PlayerState_Unk111
#define HAS_PlayerState_Unk113
#define KEYFRAME_PlayerState_Unk113 723

#define HAS_Chara_Dahlia
#define MAP_CHARA_COUNT 1

// Config used by `Map_RoomIdxGet`.
#define MAP_ROOM_MIN_X    Q12(-200.0f)
#define MAP_ROOM_MAX_X    Q12(200.0f)
#define MAP_ROOM_MIN_Z    Q12(0.0f)
#define MAP_ROOM_MAX_Z    Q12(120.0f)

#define MAP_FIELD_4C_COUNT 200
#define MAP_BLOOD_SPLAT_COUNT_MAX 150

// JAP0 merges "It won't open." into the message before it, so messages afterward are all indexed one lower than in USA.
#if VERSION_REGION_IS(NTSC)
    #define MAP_MSG(idx) (idx)
#elif VERSION_REGION_IS(NTSCJ)
    #define MAP_MSG(idx) ((idx) >= 39 ? (idx) - 1 : (idx))
#endif

#include "maps/common/shared.h"

extern u16 g_Cutscene_MapMsgAudioCmds[20];
extern q19_12 g_Cutscene_Timer;
extern VECTOR3 g_CameraPositionTarget;
extern VECTOR3 g_CameraLookAtTarget;
extern u8 g_Cutscene_MapMsgAudioIdx;

void func_800CB010(s32 arg0, s32 arg1, s32 arg2);

void func_800CE858(s32 arg0);

void func_800CE88C(void);

void func_800CEB9C(void);

void func_800CEC30(void);

void func_800CED44(void);

void func_800CED88(void);

void func_800CF798(void);

void func_800CF7C4(void);

void Map_WorldObjectsInit(void);

void Map_WorldObjectsUpdate(void);

#endif
