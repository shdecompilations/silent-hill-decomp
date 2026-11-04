#ifndef _MAPS_MAP3_S03_H
#define _MAPS_MAP3_S03_H

#define HAS_PlayerState_Reset
#define HAS_PlayerState_Unk59
#define HAS_PlayerState_Unk60
#define HAS_PlayerState_Unk146
#define HAS_PlayerState_Unk147
#define HAS_PlayerState_Unk154
#define KEYFRAME_PlayerState_Unk146 954
#define KEYFRAME_PlayerState_Unk147 988

#define MAP_ROOM_MIN_X Q12(-160.0f)
#define MAP_ROOM_MAX_X Q12(160.0f)
#define MAP_ROOM_MIN_Z Q12(-160.0f)
#define MAP_ROOM_MAX_Z Q12(160.0f)

#define MAP_FIELD_4C_COUNT 250
#define MAP_BLOOD_SPLAT_COUNT_MAX 150

#if VERSION_REGION_IS(NTSC)
    #define MAP_MSG(idx) (idx)
#elif VERSION_REGION_IS(NTSCJ)
    #define MAP_MSG(idx) ((idx) >= 34 ? (idx) - 2 : (idx) >= 31 ? (idx) - 1 : (idx))
#endif

#include "maps/common/shared.h"

extern u8 D_800D6B40[][2];

extern u8 D_800D6B50[];

extern u16 D_800D6B54;

extern s32 D_800D6BD0;

extern s32 D_800D6BD4;

extern q3_12 D_800D6BD8;

extern u8 D_800D6BDA;

extern u8 D_800D8140[];

/** Relative inventory item ID since `InvItemId_PlateOfTurtle`. */
extern u8 D_800D8144;

extern u8 D_800D8145;

extern s_WorldObjectPlacement g_WorldObject_Dr[6];
extern s_WorldObjectPose      g_WorldObject_Plate0;
extern s_WorldObjectPlacement g_WorldObject_Plate1;
extern s_WorldObjectPose      g_WorldObject_Panel;
extern s_WorldObjectPose      g_WorldObject_Blood0;
extern s_WorldObjectPose      g_WorldObject_Lighter;
extern s_WorldObjectPlacement g_WorldObject_Alcohol;
extern s_WorldObjectPlacement g_WorldObject_Key;
extern s_WorldObjectModel     g_WorldObject_Colors[4];
extern s_WorldObjectPose      g_WorldObject_SavePad;
extern u8                     D_800D8428;
extern u8                     D_800D8490;
extern s_WorldObjectPose      g_WorldObject_Blood1;
extern s_WorldObjectPlacement g_WorldObject_Blood2;
extern s_WorldObjectModel     g_CommonWorldObjects[6];
extern s_Pose      g_CommonWorldObjectPoses[5];

void func_800D151C(void);

void func_800D1774(void);

void func_800D17A0(void);

void func_800D17CC(void);

void func_800D17F8(s32 arg0, s32 arg1);

void func_800D1A58(void);

void func_800D26D0(void);

void func_800D2708(void);

void func_800D2740(void);

void func_800D2778(void);

void func_800D27F4(void);

void func_800D2C2C(void);

void func_800D2C58(void);

void func_800D2C84(void);

void func_800D2CB0(void);

void func_800D2CDC(void);

void func_800D30FC(void);

void func_800D3128(void);

void Map_WorldObjectsInit(void);

void Map_WorldObjectsUpdate(void);

#endif
