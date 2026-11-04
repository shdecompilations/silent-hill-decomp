#ifndef _MAPS_MAP5_S01_H
#define _MAPS_MAP5_S01_H

#define MAP_ROOM_MIN_X Q12(-200.0f)
#define MAP_ROOM_MAX_X Q12(80.0f)
#define MAP_ROOM_MIN_Z Q12(-200.0f)
#define MAP_ROOM_MAX_Z Q12(40.0f)

#define HAS_PARTICLE_CASE_10
#define PARTICLE_CASE_COUNT 1

#define MAP_FIELD_4C_COUNT 200
#define MAP_BLOOD_SPLAT_COUNT_MAX 150

#include "maps/common/shared.h"

extern s_800F0158 D_800F0158[12];

extern u8 D_800F0170[4];

extern u8 D_800F0350[4];

extern s32 D_800F0354;

extern s32 D_800F0358;

extern s_WorldObjectModel   g_CommonWorldObjects[6]; // D_800F3E70
extern s_WorldObjectPlacement g_WorldObject0; // D_800F3E10
extern s_WorldObjectPose g_WorldObject1; // D_800F3E40
extern s_Pose  g_CommonWorldObjectPoses[5]; // 0x800F018C

extern u16 D_800F0174;
extern s_FsImageDesc D_800F0178;
extern VECTOR3 D_800F0180;
extern u8 D_800F035C;
extern u8 D_800F035D;
extern q3_12 D_800F035E; // Time.
extern q19_12 D_800F0360;

extern VECTOR3 g_Cutscene_CameraPositionTarget;
extern VECTOR3 g_Cutscene_CameraLookAtTarget;
extern q19_12 g_Cutscene_Timer;

extern s32           D_800ED5C8;
extern GsF_LIGHT     D_800F1450[][2];
extern GsCOORDINATE2 D_800F1510;
extern SVECTOR       D_800F1560;
extern VbRVIEW       D_800F1570;
extern s8            D_800F1598[];
extern u8            D_800F159C;
extern s32           D_800F15A0;
extern s32           D_800F1590;
extern s8            D_800F1594[];
extern s8            D_800F159D;
extern s_BgmLayerLimits D_800EFC74;
extern q19_12          D_800EFC7C; // Timer.
extern u16             D_800EFC80[];

s16 func_800CD20C(void);

// Loads number pad.
void func_800CD818(void);

void func_800CD860(void);

/** @brief Handles a First Aid Kit, Health Drink, Handgun Bullets, or Rifle Shells item pickup event. */
void MapEvent_CommonItemTake(void);

/** @brief Puzzle event? Similar to M7S01 `func_800D94DC`. */
void func_800EBA40(void);

/** @brief Handles the ? map pickup event. */
void MapEvent_PaperMapTake(void);

void func_800EBF70(void);

void func_800EC2D8(void);

void func_800EC42C(void);

void Map_WorldObjectsInit(void);

void Map_WorldObjectsUpdate(void);

void func_800CD51C(s32, s32);

void func_800CDB14(s32, s32);

void func_800CDB98();

void func_800CE180();

// Same as `func_800E6758` in MAP2_S02.
void func_800E828C(s_SubCharacter* chara);

void func_800EB6B0(void);

#endif
