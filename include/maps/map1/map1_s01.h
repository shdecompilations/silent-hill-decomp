#ifndef _MAPS_MAP1_S01_H
#define _MAPS_MAP1_S01_H

#define HAS_PlayerState_Reset
#define HAS_PlayerState_Unk59
#define HAS_PlayerState_Unk60
#define HAS_PlayerState_Unk83
#define HAS_PlayerState_Unk85
#define HAS_PlayerState_Unk122
#define KEYFRAME_PlayerState_Unk59 873
#define KEYFRAME_PlayerState_Unk60 888

#define MAP_ROOM_MIN_X Q12(-160.0f)
#define MAP_ROOM_MAX_X Q12(160.0f)
#define MAP_ROOM_MIN_Z Q12(-80.0f)
#define MAP_ROOM_MAX_Z Q12(200.0f)

#define MAP_FIELD_4C_COUNT 300
#define MAP_BLOOD_SPLAT_COUNT_MAX 150

// JAP0 has a different set of messages from 22 up (one dropped, two merged).
#if VERSION_REGION_IS(NTSC)
    #define MAP_MSG(idx) (idx)
#elif VERSION_REGION_IS(NTSCJ)
    #define MAP_MSG(idx) ((idx) >= 29 ? (idx) - 2 : (idx) >= 23 ? (idx) - 1 : (idx))
#endif

#include "maps/common/shared.h"

extern u8  D_800DC9FC;
extern u16 D_800DCA04[42];

extern s32 D_800DD4FC;

extern s8 g_PianoKeys[11];

extern s8 g_PianoKeySequence[5];

extern s_Pose g_CommonWorldObjectPoses[5];

extern s32 D_800DD57C;

extern q19_12 g_PianoCursorX;

extern q19_12 g_PianoCursorY;

extern s32 g_PianoKeyCounter;

extern s32 D_800DD594;

extern s_WorldObjectPose g_WorldObject_SilverH;
extern s_WorldObjectPose g_WorldObject_Tobira00;
extern s_WorldObjectPose g_WorldObject_Tobira01;
extern s_WorldObjectPose g_WorldObject_HakoHid;

extern VECTOR3 D_800DF000;
extern VECTOR3 D_800DF010;

extern s_WorldObjectModel g_WorldObject_HandHid;
extern s_WorldObjectModel g_WorldObject_Hand2Hi;
extern s_WorldObjectModel g_WorldObject_PanelgH;

extern VECTOR3  D_800DF080;

extern s_WorldObjectPose g_WorldObject_AcidHid;
extern s_WorldObjectPose g_WorldObject_Futa1Hi;
extern s_WorldObjectPose g_WorldObject_Futa2Hi;

extern s_WorldObjectModel g_CommonWorldObjects[6];

void MapEvent_DoorJammed(void);

void MapEvent_DoorLocked(void);

void MapEvent_DoorUnlocked(void);

void MapEvent_CommonItemTake(void);

void MapEvent_HandStatueInspect(void);

void MapEvent_ChemicalItemTake(void);

void func_800D7308(void);

void MapEvent_GoldMedallionItemPickup(void);

void func_800D7830(void);

void MapEvent_PianoPuzzleInteract(void);

void func_800D7EEC(void);

/** @brief Handles the piano puzzle event.
 *
 * @param playNote Play a note.
 */
void PianoPuzzle_Control(bool playNote);

void func_800D857C(void);

void func_800D8794(void);

void func_800D87C0(void);

void Map_WorldObjectsInit(void);

void Map_WorldObjectsUpdate(void);

void func_800D9DDC(void);

void func_800D9EC4(void);

#endif
