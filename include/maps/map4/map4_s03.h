#ifndef _MAPS_MAP4_S03_H
#define _MAPS_MAP4_S03_H

#define HAS_PlayerState_Reset
#define HAS_PlayerState_Unk123
#define HAS_PlayerState_Unk128
#define HAS_PlayerState_Unk129
#define KEYFRAME_PlayerState_Unk123 877

#define MAP_ROOM_MIN_X Q12(-240.0f)
#define MAP_ROOM_MAX_X Q12(240.0f)
#define MAP_ROOM_MIN_Z Q12(-120.0f)
#define MAP_ROOM_MAX_Z Q12(240.0f)

#define MAP_FIELD_4C_COUNT        250
#define MAP_BLOOD_SPLAT_COUNT_MAX 150

#include "maps/common/shared.h"

#define COPY_GT4_DATA(poly, idx, ptr0, ptr1, ptr2, n) \
{                                                     \
    u16* ptr4 = &(ptr0)[(idx)];                       \
    s32* ptr5 = &(ptr1)[(idx)];                       \
    s32* ptr6 = &(ptr2)[(idx)];                       \
                                                      \
    *(u16*)&(poly)->u##n = *ptr4;                     \
    *(s32*)&(poly)->x##n = *ptr5;                     \
    *(s32*)&(poly)->r##n = *ptr6;                     \
}

typedef struct
{
    SVECTOR field_0; // } Note: Not sure if these are actually `SVECTOR`.
    SVECTOR field_8; // } That struct fits and works with some copy code, but could be a custom struct of 4 `u16`s.
} s_800DB7D4;
STATIC_ASSERT_SIZEOF(s_800DB7D4, 16);

typedef struct
{
    SVECTOR field_0[4];
    u8      field_20;
    u8      field_21;
    u8      field_22;
    u8      field_23;
    s32     field_24;
    s32     field_28;
    s32     field_2C;
    s32     field_30;
    s16     field_34;
    s16     field_36;
} s_800E06A0;
STATIC_ASSERT_SIZEOF(s_800E06A0, 56);

typedef struct
{
    s32                field_0;
    s32                field_4;
    s_800E06A0         field_8[10];
    s_WorldObjectModel objRef_238;
    s8                 unk_254[4];
    VECTOR3            field_258;
} s_800E0698;
STATIC_ASSERT_SIZEOF(s_800E0698, 612);

// Used by Twinfeeler for random spawn locations when resurfacing?
typedef struct _800E0930
{
    /* 0x0  */ s32             stateStep;
    /* 0x4  */ q19_12          timer;
    /* 0x8  */ s_SubCharacter* chara;
    /* 0xC  */ VECTOR3         position;
    /* 0x18 */ void            (*funcptr_18)(struct _800E0930*);
} s_800E0930;

typedef struct
{
    q19_12 x_0;
    q19_12 z_4;
    MATRIX world_8;
} s_800E0988;

typedef struct _800DF580
{
    /* 0x0  */ s32     field_0;
    /* 0x4  */ s32     field_4;
    /* 0x8  */ q19_12  field_8;
    /* 0xC  */ s16     field_C;
    /* 0xE  */ s16     field_E;
    /* 0x10 */ s16     field_10;
    /* 0x12 */ s8      unk_12[2];
    /* 0x14 */ SVECTOR field_14;
    /* 0x1C */ MATRIX  worldMat;
    /* 0x3C */ s32     field_3C;
    /* 0x40 */ u8      field_40[133]; // Type assumed.
    /* 0xC8 */ bool    (*field_C8)(struct _800DF580*);
    /* 0xCC */ q19_12  field_CC;
    /* 0xD0 */ s32     field_D0;
    /* 0xD4 */ s32     field_D4;
} s_800DF580;

typedef struct
{
    q19_12  timer_0;
    q19_12  field_4;
    SVECTOR field_8;
    s8      unk_10[4];
} s_800E0300;

typedef struct
{
    s32     field_0;
    SVECTOR field_4;
    s32     field_C;
    s32     field_10;
    s32     field_14;
    s8      unk_18[4];
} s_800E0440;

typedef struct
{
    q19_12  field_0;
    SVECTOR field_4;
    q19_12  field_C;
    q19_12  field_10;
    s8      unk_14[4];
} s_800E0900;

typedef struct
{
    s32 field_0;
    s32 field_4;
    s32 field_8;
    s32 field_C;
} s_800DB210;

typedef struct
{
    u8  field_0;
    s8  unk_1;
    u16 field_2;
    u8  field_4;
    u8  field_5;
    u8  field_6;
    u8  field_7;
    u8  field_8;
    u8  field_9;
} s_800DB874;

extern s_800DB874 D_800DB874[];

extern s_800DB210 D_800DB210;

extern SVECTOR3 D_800DAE60;

extern u8 D_800DAE68[];

extern s_800DF580 D_800DF580[];

extern s_800E0300 D_800E0300[16];

extern s_800E0440 D_800E0440[3];

extern s_800E0900 D_800E0900[2];

extern s_800E0930 D_800E0930[3];

extern s_800E0988 D_800E0988[];

extern s_800DB7D4 D_800DB7D4;
extern s_800DB7D4 D_800DB7E4[3][3];

extern s_AnimInfo TWINFEELER_ANIM_INFOS[];

extern s32 D_800DB914; // } RNG related values?
extern s32 D_800DB918; // }

extern s_FsImageDesc D_800DB91C;

extern u16 D_800DB92C;

extern u16 D_800DB9E0;
extern u8  D_800DB9E2;

extern s_WorldObjectPose  g_WorldObject_Fence;
extern s_WorldObjectPose  g_WorldObject_Mal5_21;
extern s_WorldObjectModel g_WorldObject_Mal6[2];
extern VECTOR3            g_WorldObject_UnkPos;
extern SVECTOR3           g_WorldObject_UnkRot;
extern s_WorldObjectPose  g_WorldObject_SavePad;
extern s_WorldObjectModel g_CommonWorldObjects[6];
extern s_WorldObjectModel D_800E08D0;
extern s_Pose  g_CommonWorldObjectPoses[3]; // 0x800DB930
extern VECTOR3 D_800DB7C8;
extern SVECTOR D_800DB924;

/** @brief Handles a First Aid Kit or Rifle Shells item pickup event. */
void MapEvent_CommonItemTake(void);

void func_800D6704(void);

void func_800D6774(void);

void func_800D6F24(void);

s16 func_800D7394(void);

void func_800D7408(void);

void func_800D7450(void);

void func_800D7548(void);

void func_800D761C(s_800E06A0* arg0, s_800DB7D4* arg1, s8 arg2, s32 arg3);

void func_800D76BC(s32 arg0);

bool func_800D76E8(void); // Assumed return type.

void func_800D7718(void);

void func_800D7808(s_800E06A0* arg0, s32 arg1); // Assumed types.

bool func_800D78D4(s_800E06A0* arg0);

s32 func_800D78F4(s_800E06A0* arg0, s32 arg1);

s32 func_800D7AE0(s_800E06A0* arg0);

void func_800D7F1C(s_800E06A0* arg0);

bool func_800D81FC(s_800E06A0* arg0, s32 arg1);

bool func_800D8230(s_800E06A0* arg0, s32 arg1, s32 arg2);

bool func_800D826C(s_800E06A0* arg0, s32 arg1, s32 arg2);

bool func_800D8354(s_800E06A0* arg0, s32 arg1, s32 arg2);

bool func_800D84C0(s_800E06A0* arg0, s32 arg1, s32 arg2);

bool func_800D85E4(s_800E06A0* arg0, s32 arg1);

bool func_800D8620(s_800E06A0* arg0, s32 arg1, s32 arg2);

s32 func_800D87AC(s_800E06A0* arg0);

q19_12 func_800D8874(void);

void func_800D88C8(s_800E06A0*, u8); // Assumed types.

void func_800D8FC0(void);

void func_800D960C(void);

void func_800D9824(void);

void Map_WorldObjectsInit(void);

void Map_WorldObjectsUpdate(void);

void func_800DA3E0(void);

void func_800DA718(void);

#endif
