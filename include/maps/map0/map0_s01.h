#ifndef _MAPS_MAP0_S01_H
#define _MAPS_MAP0_S01_H

#define HAS_PlayerState_Unk51
#define HAS_PlayerState_Unk131
#define HAS_PlayerState_Reset
#define HAS_PlayerState_WalkForward
#define HAS_PlayerState_Unk70
#define HAS_PlayerState_Unk71
#define HAS_PlayerState_Unk132
#define HAS_PlayerState_Unk76
#define HAS_PlayerState_Unk77
#define HAS_PlayerState_Unk78
#define HAS_PlayerState_Unk89
#define HAS_PlayerState_Unk90
#define HAS_PlayerState_Unk91
#define HAS_PlayerState_Unk92
#define HAS_PlayerState_Unk93
#define HAS_PlayerState_Unk110

#define HAS_Chara_Cybil
#define MAP_CHARA_COUNT 1

#define MAP_ROOM_MIN_X Q12(-320.0f)
#define MAP_ROOM_MAX_X Q12(240.0f)
#define MAP_ROOM_MIN_Z Q12(-240.0f)
#define MAP_ROOM_MAX_Z Q12(400.0f)
#define MAP_HAS_SECONDARY_GRID

#define MAP_FIELD_4C_COUNT 300
#define MAP_BLOOD_SPLAT_COUNT_MAX 150

#include "maps/common/shared.h"

extern u16 g_Cutscene_MapMsgAudioCmds[];

extern u16 D_800DE124;

extern u16 D_800DE128;

extern s_Pose D_800DE12C;

extern s_Pose D_800DE140;

extern VECTOR3 D_800DE154;

extern u8 D_800DE250;

extern u8 D_800DE251;

extern VECTOR3 g_Cutscene_CameraPositionTarget;
extern VECTOR3 g_Cutscene_CameraLookAtTarget;
extern q19_12  g_Cutscene_Timer;
extern u8      g_Cutscene_MapMsgAudioIdx;

extern u8 D_800E23A1; // Radio static SFX falloff.

// Breaking glass.
extern s_WorldObjectModel D_800E23B0[1];
extern s_WorldObjectModel D_800E23D0[1];
extern s_WorldObjectModel D_800E23F0[3];
extern s_WorldObjectModel D_800E2450[2];

extern VECTOR3 g_CutscenePosition;
extern s_WorldObjectPose g_WorldObject_KitchenKnife;
extern s_WorldObjectPose g_WorldObject_Flashlight;
extern s_WorldObjectPose g_WorldObject_Map;
extern s_WorldObjectPose g_WorldObject_PocketRadio;

extern q3_12 D_800E2560;

extern s_WorldObjectModel g_CommonWorldObjects[];

void MapEvent_CafeCutscene(void);

/** @brief @unused Handles the demo end screen event after the Air Screamer breaks through the cafe window. Seen in demo builds. */
void MapEvent_ToBeContinued(void);

/** @brief Handles the Air Screamer cutscene in the cafe. */
void MapEvent_AirScreamerIntroCutscene(void);

/** @brief Handles the Pocket Radio item pickup event. */
void MapEvent_PocketRadioItemTake(void);

/** @brief Handles the Flashlight item pickup event. */
void MapEvent_FlashlightItemTake(void);

/** @brief Handles the Silent Hill town map pickup event in the cafe.
 *
 * Also responsible for air screamer scene setup.
 */
void MapEvent_MapItemTake(void);

/** @brief Handles the Kitchen Knife item pickup event. */
void MapEvent_KitchenKnifeItemTake(void);

void MapEvent_EmptyFunction(void);

/** @brief Handles a First Aid Kit or Health Drink item pickup event. */
void MapEvent_CommonItemTake(void);

/** @brief Handles the Air Screamer death event. */
void MapEvent_AirScreamerDeath(void);

void Map_WorldObjectsInit(void);

void Map_WorldObjectsUpdate(void);

void CutsceneObjects_Add(s32 arg0);

#endif
