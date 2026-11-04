#ifndef _MAPS_MAP6_S01_H
#define _MAPS_MAP6_S01_H

#define HAS_PlayerState_Unk51
#define HAS_PlayerState_Unk131
#define HAS_PlayerState_Reset
#define HAS_PlayerState_Unk70
#define HAS_PlayerState_Unk71
#define HAS_PlayerState_Unk132
#define HAS_PlayerState_Unk113
#define KEYFRAME_PlayerState_Unk113 700

#define HAS_Chara_Cybil
#define HAS_Chara_Dahlia
#define MAP_CHARA_COUNT 2

#define MAP_ROOM_MIN_X Q12(-200.0f)
#define MAP_ROOM_MAX_X Q12(200.0f)
#define MAP_ROOM_MIN_Z Q12(0.0f)
#define MAP_ROOM_MAX_Z Q12(120.0f)

#define MAP_FIELD_4C_COUNT 200
#define MAP_BLOOD_SPLAT_COUNT_MAX 150

#include "maps/common/shared.h"

extern q19_12 g_Cutscene_Timer;
extern u16    g_Cutscene_MapMsgAudioCmds0;
extern u16    g_Cutscene_MapMsgAudioCmds1;

extern u16 D_800D4108;

extern s_FsImageDesc D_800D410C;

extern DVECTOR D_800D4114[];

extern u8 g_Cutscene_UpdateSibyl; /** `bool` */
extern u8 g_Cutscene_UpdateDaria; /** `bool` */

extern u8 g_Cutscene_MapMsgAudioIdx1;

extern VECTOR3 D_800D4174[5];

extern q3_12 D_800D41B0;

/** Time. */
extern q19_12 D_800D41B4;

extern u8 D_800D5345; /** `bool` */

extern VECTOR3 g_Cutscene_CameraPositionTarget;
extern VECTOR3 g_Cutscene_CameraLookAtTarget;
extern u8      g_Cutscene_MapMsgAudioIdx0;

extern s_WorldObjectPose  D_800D5374;
extern s_WorldObjectPose  D_800D53A4[7];
extern s_WorldObjectModel g_CommonWorldObjects[6];
extern s_Pose  g_CommonWorldObjectPoses[3]; // 0x800D4128

/** @brief Handles a Health Drink, Handgun Bullets, or Rifle Shells item pickup event. */
void MapEvent_CommonItemTake(void);

// Map event.
void func_800D13D8(void);

// Map event.
void func_800D236C(void);

// Map event.
void func_800D2658(void);

void Map_WorldObjectsInit(void);

void Map_WorldObjectsUpdate(void);

#endif
