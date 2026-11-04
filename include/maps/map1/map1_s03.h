#ifndef _MAPS_MAP1_S03_H
#define _MAPS_MAP1_S03_H

#define HAS_PlayerState_Reset
#define HAS_PlayerState_Unk59
#define HAS_PlayerState_Unk60
#define HAS_PlayerState_Unk85
#define HAS_PlayerState_Unk86
#define HAS_PlayerState_Unk99
#define HAS_PlayerState_Unk100
#define HAS_PlayerState_Unk107
#define HAS_PlayerState_Unk108
#define HAS_PlayerState_Unk122
#define KEYFRAME_PlayerState_Unk59  903
#define KEYFRAME_PlayerState_Unk60  918
#define KEYFRAME_PlayerState_Unk122 1093

#define MAP_ROOM_MIN_X Q12(-160.0f)
#define MAP_ROOM_MAX_X Q12(160.0f)
#define MAP_ROOM_MIN_Z Q12(-80.0f)
#define MAP_ROOM_MAX_Z Q12(200.0f)

#define MAP_FIELD_4C_COUNT 300
#define MAP_BLOOD_SPLAT_COUNT_MAX 150

#if VERSION_REGION_IS(NTSC)
    #define MAP_MSG(idx) (idx)
#elif VERSION_REGION_IS(NTSCJ)
    #define MAP_MSG(idx) ((idx) >= 25 ? (idx) + 1 : (idx))
#endif

#include "maps/common/shared.h"

extern SVECTOR D_800E1F74;
extern u16     D_800E1F7C[];

extern VECTOR3 D_800E1F88[];
extern VECTOR3 D_800E1FC4[];

extern u16 D_800E2000[1]; // g_Cutscene_MapMsgAudioCmds
extern s_FsImageDesc D_800E2004;
extern VECTOR3 D_800E200C;
extern s16 D_800E2018;
extern q19_12 D_800E201C; // Time.
extern s_Pose  g_CommonWorldObjectPoses[10]; // 0x800E2020
extern s32 D_800E20E8; // Angle.
extern s16 D_800E20EC;
extern u8 D_800E20EE;

extern u16 D_800E20F8;
extern u16 D_800E20FA;
extern q3_12 D_800E20FC; // Angle.
extern s16 D_800E20FE;
extern u8  D_800E2100;
extern u8  D_800E2101;
extern u8 D_800E2102; // g_Cutscene_MapMsgAudioIdx

extern s_WorldObjectPose g_WorldObject5; // 0x800E6130

extern VECTOR3 g_Cutscene_CameraPositionTarget;
extern VECTOR3 g_Cutscene_CameraLookAtTarget;
extern s32     g_Cutscene_Timer;

extern s_WorldObjectPose g_WorldObject0; // 0x800E6180
extern s_WorldObjectPose g_WorldObject1; // 0x800E61B0
extern s_WorldObjectPose g_WorldObject2; // 0x800E61E0
extern s_WorldObjectPlacement g_WorldObject3; // 0x800E6210
extern s_WorldObjectPlacement g_WorldObject4; // 0x800E6240
extern s_WorldObjectPlacement g_WorldObject6; // 0x800E6270
extern s_WorldObjectPose g_WorldObject7; // 0x800E62A0
extern u8 D_800E62D0;
extern s_WorldObjectModel   g_CommonWorldObjects[6]; // 0x800E62E0

extern u8 D_800E6388;
extern q19_12 D_800E638C; // Time?

typedef struct
{
    s32   vx_0;
    s32   vz_4;
    q4_12 field_8; 
    s16   field_A;
    q3_12 field_C;
    q3_12 field_E;
    q3_12 field_10;
    s16   field_12;
    u8    idx_14;
    u8    unk_15;
    u8    unk_16;
} s_800E3A40;
extern s_800E3A40 D_800E3A40[];

void func_800CDCE0(s32 arg0, s32 arg1, s32 arg2);

void func_800CE0CC(s32 arg0);

bool func_800CE164(POLY_FT4** poly, s32 idx);

/** @brief Handles the Library Reserve Key item use event. */
void MapEvent_LibraryReserveKeyUse(void);

/** @brief Handles the Classroom Key item use event. */
void MapEvent_ClassroomKeyUse(void);

/** @brief Handles a First Aid Kit, Health Drink, Handgun Bullets, or Shotgun Shells item pickup event. */
void MapEvent_CommonItemTake(void);

/** @brief Handles the roof drain puzzle interaction event. TODO: How are the two variants used? */
void MapEvent_RoofDrainPuzzleInteract0(void);

/** @brief Handles the Roof drain puzzle interaction event. TODO: How are the two variants used? */
void MapEvent_RoofDrainPuzzleInteract1(void);

/** @brief Handles the roof drainage valve interaction event. Handles logic and water effects. */
void MapEvent_DrainageValveInteract(void);

/** @brief Handles the "The Monster Lurks" book reading event. */
void MapEvent_TheMonsterLurksBookRead(void);

/** @brief Handles the Split Head fairy tale book reading event. */
void MapEvent_SplitHeadFairyTaleBookRead(void);

/** @brief Handles the locker inspection event when the cat is no longer inside. */
void MapEvent_OpenedCatLockerInspect(void);

/** @brief Handles the locker inspection event when the cat is still inside. */
void MapEvent_UnopenedCatLockerInspect(void);

/** @brief Handles the Locker Dead Body cutscene event. */
void MapEvent_LockerDeadBodyCutscene(void);

/** @brief Handles the Library Reserve item pickup event. */
void MapEvent_LibraryReserveKeyItemTake(void);

/** @brief Handles the Rubber Ball item use event. */
void MapEvent_RubberBallUse(void);

// Some cutscene event.
void func_800DCDDC(void);

void Map_WorldObjectsInit(void);

void Map_WorldObjectsUpdate(void);

void func_800DE624(void);

void func_800DE828(void);

#endif
