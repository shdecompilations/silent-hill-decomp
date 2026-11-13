#ifndef _MAPS_MAP5_S00_H
#define _MAPS_MAP5_S00_H

#define HAS_PlayerState_Reset
#define HAS_PlayerState_Unk59
#define HAS_PlayerState_Unk60
#define HAS_PlayerState_Unk87
#define HAS_PlayerState_Unk88
#define HAS_PlayerState_Unk114
#define KEYFRAME_PlayerState_Unk59 831
#define KEYFRAME_PlayerState_Unk60 846
#define KEYFRAME_PlayerState_Unk87 857
#define KEYFRAME_PlayerState_Unk88 894

#define MAP_ROOM_MIN_X Q12(-120.0f)
#define MAP_ROOM_MAX_X Q12(80.0f)
#define MAP_ROOM_MIN_Z Q12(-120.0f)
#define MAP_ROOM_MAX_Z Q12(80.0f)

#define MAP_FIELD_4C_COUNT 200
#define MAP_BLOOD_SPLAT_COUNT_MAX 150

#include "maps/common/shared.h"

typedef struct
{
    VECTOR3  field_0;
    s8       unk_C[20];
    SVECTOR3 field_20;
    s8       unk_26[10];
    s32      field_30;
    q19_12   field_34; // Timer?
} s_func_800D5B00_D94;

typedef struct
{
    s8                  unk_0[4];
    VECTOR              field_4;
    s8                  field_14[32];
    s8                  field_34[41];
    u8                  field_5D[81 * 41];
    s8                  field_D56[41];
    s8                  field_D7F[5];
    s32                 field_D84;
    s32                 field_D88;
    s32                 field_D8C;
    s32                 field_D90;
    s_func_800D5B00_D94 field_D94[200];
} s_func_800D5B00;

extern s32 D_800DAB78;

extern u8 D_800DAB7C[3];

extern s_WorldObjectPose  g_WorldObject_Map;
extern s_WorldObjectPose  g_WorldObject_SavePad0;
extern s_WorldObjectPose  g_WorldObject_SavePad1;
extern s_WorldObjectModel g_CommonWorldObjects[6];

extern s32 D_800DA154[];

extern s_BgmLayerLimits D_800DA570;

extern u16 D_800DA578[];

extern VECTOR3 D_800CB0CC;

extern s_Pose D_800DAAD0;
extern s_Pose D_800DAAE4;
extern s_Pose D_800DAAF8;
extern s_Pose D_800DAB0C;
extern s_Pose D_800DAB20;
extern s_Pose D_800DAB34;
extern s_Pose D_800DAB48;
extern s_Pose D_800DAB5C;

void func_800CB0D8(void);

bool func_800CB25C(POLY_FT4** poly, s32 idx);

void func_800D5B00(void);

void func_800D5CC4(s32 x, s32 y, s32 val);

void func_800D5D90(void);

s32 func_800D5EA8(s32 arg0, s32 arg1);

void func_800D5EE8(void);

void func_800D61D4(void);

void func_800D62C8(void);

void func_800D631C(VECTOR* arg0, q19_12 rotY);

s32 func_800D63DC(s32* screenXy);

void func_800D6414(void);

void func_800D6490(VECTOR3* arg0);

void func_800D6790(bool arg0);

void func_800D67F4(void);

void func_800D6888(void);

/** @brief Handles a First Aid Kit, Health Drink, Handgun Bullets, or Shotgun Shells item pickup event. */
void MapEvent_CommonItemTake(void);

void func_800D69DC(void);

/** @brief Handles the Sewer Key item pickup event. */
void MapEvent_SewerKeyTake(void);

void func_800D6B00(void);

void func_800D732C(void);

// Next 4 functions are all very similar to each other, only differ in Q12 position values and flag numbers.
void func_800D75FC(void);
void func_800D7940(void);
void func_800D7C84(void);
void func_800D7F88(void);

/** @brief Handles the ? map pickup event. */
void MapEvent_PaperMapTake(void);

void Map_WorldObjectsInit(void);

void Map_WorldObjectsUpdate(void);

void func_800D8DFC(void);

#endif
