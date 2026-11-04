#ifndef _MAPS_MAP6_S00_H
#define _MAPS_MAP6_S00_H

#define HAS_PlayerState_Reset
#define HAS_PlayerState_WalkForward
#define HAS_PlayerState_Unk114
#define HAS_PlayerState_Unk126
#define HAS_PlayerState_Unk127
#define HAS_PlayerState_Unk133
#define KEYFRAME_PlayerState_Unk114 956

#define MAP_ROOM_MIN_X Q12(-200.0f)
#define MAP_ROOM_MAX_X Q12(80.0f)
#define MAP_ROOM_MIN_Z Q12(-80.0f)
#define MAP_ROOM_MAX_Z Q12(280.0f)

#define MAP_FIELD_4C_COUNT 200
#define MAP_BLOOD_SPLAT_COUNT_MAX 150

#include "maps/common/shared.h"

typedef struct
{
    POLY_GT4* field_0;
    MATRIX    field_4;
    MATRIX    field_24;
    SVECTOR   field_44[3];
    VECTOR3   field_5C;
    SVECTOR   field_68; // Q3.12
    CVECTOR   field_70;
    CVECTOR   field_74[4];
    s32       field_84;
    s32       field_88;
    s16       field_8C;
    s16       field_8E;
    u8        field_90[17][17];
    s32       field_1B4;
    s32       field_1B8;
    s32       field_1BC;
    s32       field_1C0;
    s32       field_1C4;
} s_func_800EC4B4;

typedef struct
{
    DVECTOR field_0[17][17];
    s32     field_484[17][17];
    CVECTOR field_908[17][17][2];
} s_func_800EC4B4_2;

extern u16     D_800F0038;
extern q19_12  g_Cutscene_Timer;
extern s32     D_800F0044;
extern DVECTOR D_800F0084[17][17];
extern VECTOR3 g_Cutscene_CameraPositionTarget;
extern VECTOR3 g_Cutscene_CameraLookAtTarget;
extern u8      D_800F0684;
extern s16     D_800F0686;

extern s_WorldObjectPlacement g_WorldObject_Window;
extern s_WorldObjectPlacement g_WorldObject_DrOpen1;
extern s_WorldObjectPlacement g_WorldObject_DrOpen2;
extern s_WorldObjectPlacement g_WorldObject_DrClose;
extern s_WorldObjectModel     g_CommonWorldObjects[6];
extern s_Pose      g_CommonWorldObjectPoses[3];

/** @brief Handles a First Aid Kit, Health Drink, or Shotgun Shells item pickup event. */
void MapEvent_CommonItemTake(void);

void func_800EB11C(void);

void Map_WorldObjectsInit(void);

void Map_WorldObjectsUpdate(void);

void func_800EC4B4(s32 arg0);

#endif
