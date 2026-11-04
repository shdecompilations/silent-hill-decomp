#ifndef _MAPS_MAP2_S02_H
#define _MAPS_MAP2_S02_H

#define MAP_ROOM_MIN_X Q12(-240.0f)
#define MAP_ROOM_MAX_X Q12(240.0f)
#define MAP_ROOM_MIN_Z Q12(-120.0f)
#define MAP_ROOM_MAX_Z Q12(240.0f)

#define HAS_PARTICLE_CASE_10
#define PARTICLE_CASE_COUNT 1

#define MAP_FIELD_4C_COUNT 100
#define MAP_BLOOD_SPLAT_COUNT_MAX 50

#include "maps/common/shared.h"

extern s_Pose g_CommonWorldObjectPoses[6];
extern VECTOR3 D_800ED938;
extern q3_12 D_800F0B2C; // Time.
extern s_WorldObjectModel   g_CommonWorldObjects[6];

void func_800E3FB0(s_SubCharacter* chara);

void func_800E9D54(void);

void func_800E9E10(void);

void func_800E9EAC(void);

void func_800E9FDC(void);

void Map_WorldObjectsInit(void);

void Map_WorldObjectsUpdate(void);

void func_800EAA50(void);

#endif
