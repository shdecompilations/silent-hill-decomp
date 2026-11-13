#ifndef _MAPS_MAP1_S06_H
#define _MAPS_MAP1_S06_H

#define HAS_PlayerState_Reset
#define HAS_PlayerState_Unk59
#define HAS_PlayerState_Unk60
#define KEYFRAME_PlayerState_Unk59 678
#define KEYFRAME_PlayerState_Unk60 693

#define MAP_ROOM_MIN_X Q12(-160.0f)
#define MAP_ROOM_MAX_X Q12(160.0f)
#define MAP_ROOM_MIN_Z Q12(-80.0f)
#define MAP_ROOM_MAX_Z Q12(200.0f)

#define MAP_FIELD_4C_COUNT 200
#define MAP_BLOOD_SPLAT_COUNT_MAX 150

#include "maps/common/shared.h"

extern s_BgmLayerLimits D_800D71E8;

extern u16 D_800D775C;

extern u8 D_800D7760;

extern u8 D_800D7761;

extern u8 D_800D778D;

extern u8 D_800D778E;

extern u8 D_800D778F;

extern u8 D_800D7790;

/** Time. */
extern q3_12 D_800DAF78;

extern s_WorldObjectPose D_800DAF84;

extern s_WorldObjectPose D_800DAFB4;

extern u8 D_800DAFE4;

extern s_WorldObjectPose D_800DAFF4;

extern s_WorldObjectPose D_800DB024;

/** Related to stereo sound balance. */
extern s8 D_800DB054;

/** Stereo sound balance. */
extern s8 D_800DB055;

extern s_WorldObjectPose D_800DB064;

void func_800D560C(void);

void func_800D5614(void);

void func_800D5B98(void);

// Draws scaled paper map and calls two other funcs.
void func_800D5D6C(u16 scrollX);

void func_800D5DD8(void);

void func_800D6338(void);

/** @brief Handles the ? map pickup event. */
void MapEvent_PaperMapTake(void);

void Map_WorldObjectsInit(void);

void Map_WorldObjectsUpdate(void);

void func_800D6C88(void);

#endif
