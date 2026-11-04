#ifndef _MAPS_MAP3_S06_H
#define _MAPS_MAP3_S06_H

#define HAS_PlayerState_Unk51
#define HAS_PlayerState_Unk131
#define HAS_PlayerState_Reset
#define HAS_PlayerState_Unk111
#define HAS_PlayerState_Unk117

#define HAS_Chara_Dahlia
#define MAP_CHARA_COUNT 1

#define HAS_PARTICLE_CASE_2
#define PARTICLE_CASE_COUNT 1

#define MAP_ROOM_MIN_X Q12(-40.0f)
#define MAP_ROOM_MAX_X Q12(160.0f)
#define MAP_ROOM_MIN_Z Q12(-80.0f)
#define MAP_ROOM_MAX_Z Q12(160.0f)

#define MAP_FIELD_4C_COUNT 200
#define MAP_BLOOD_SPLAT_COUNT_MAX 150

#include "maps/common/shared.h"

extern u16 D_800D26D0;

extern VECTOR3 D_800D26F8;

extern q19_12 g_Cutscene_Timer;

extern VECTOR3 g_Cutscene_CameraPositionTarget;

extern VECTOR3 g_Cutscene_CameraLookAtTarget;

extern q3_12 D_800D2734; // Timer. Only used for `func_800D0C14`, state 8.

extern u8 D_800D2736; // Sound index.

extern u8 D_800D2737; // `bool`

extern u8 D_800D2738;

extern s_WorldObjectPose g_WorldObject_SavePad; // D_800D5ED8;
extern s_WorldObjectPose g_WorldObject_Map; // D_800D5F08;
extern s_WorldObjectPose g_WorldObject_Map2; // D_800D5F38;
extern s_WorldObjectPose g_WorldObject_Key; // D_800D5F68;
extern s_WorldObjectPose g_WorldObject_FirstAidKit; // D_800D5F98;

void func_800D0AE4(void);

/** @brief Handles a First Aid Kit, Health Drink, Handgun Bullets, or Shotgun Shells item pickup event. */
void MapEvent_CommonItemTake(void);

void func_800D0B10(void);

void func_800D0C14(void);

/** @brief Handles the ? map pickup event. */
void MapEvent_PaperMapTake(void);

void func_800D17D8(void);

void Map_WorldObjectsInit(void);

void Map_WorldObjectsUpdate(void);

#endif
