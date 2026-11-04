#ifndef _MAPS_MAP6_S02_H
#define _MAPS_MAP6_S02_H

#define HAS_PlayerState_Unk51
#define HAS_PlayerState_Unk131
#define HAS_PlayerState_Reset
#define HAS_PlayerState_WalkForward
#define HAS_PlayerState_Unk71
#define HAS_PlayerState_Unk132
#define HAS_PlayerState_Unk111
#define HAS_PlayerState_Unk175
#define KEYFRAME_PlayerState_Unk111 915

#define MAP_ROOM_MIN_X Q12(-200.0f)
#define MAP_ROOM_MAX_X Q12(80.0f)
#define MAP_ROOM_MIN_Z Q12(-80.0f)
#define MAP_ROOM_MAX_Z Q12(280.0f)

#define MAP_FIELD_4C_COUNT 200
#define MAP_BLOOD_SPLAT_COUNT_MAX 150

#include "maps/common/shared.h"

#define GET_PTR(p, i, j, k) (&(&((u32*)(p))[(k) << 8])[(i)])[(j) * 16]

typedef struct
{
    s16 x;
    s16 y;
    u8  w[2];
    u8  h[2];
} s_800D3C4C;

typedef struct
{
    u32 field_0[8][16][16];
    u32 field_2000[16][16];
} s_8019F8F8_908;

typedef struct
{
    DVECTOR field_0[17][17];
    s32     field_484[17][17];
} s_8019F8F8_0;

typedef struct
{
    s_8019F8F8_0   field_0;
    s_8019F8F8_908 field_908;
} s_8019F8F8;

extern u16 D_800D3B40;

extern s_FsImageDesc D_800D3B44[5];

extern u16 D_800D3B6C;

extern s_FsImageDesc D_800D3B70;

extern VECTOR3 D_800D3B78[];

extern u16 D_800D3C2C[];

extern s_800D3C4C D_800D3C4C[8];

extern s32 D_800D3C84;

extern u8 D_800D3C88;

/** Timer. */
extern s32 D_800D3C8C;

extern s32 D_800D3C90;

extern s16 D_800D3C94;

extern s32 D_800D3C98;

extern u8 D_800D3C9C;

extern s16 D_800D3C9E;

extern s32 D_800D3CA0;

extern u8 D_800D4E08;
extern s8 D_800D4E09;

extern VECTOR3 g_Cutscene_CameraPositionTarget;
extern VECTOR3 g_Cutscene_CameraLookAtTarget;

extern q19_12 g_Cutscene_Timer;

extern s8 D_800D4E2C;

extern s8 D_800D4E2D;

extern s8 D_800D4E6C;
extern s8 D_800D4E6D;
extern u8 D_800D4E6E;

extern q3_12 D_800D4E70; // Angle.

extern s_WorldObjectPose g_WorldObject_SavePad;

extern s_FsImageDesc D_800CAB90;

extern RECT D_800CAB98;

extern u8 D_800D3C74[8];

extern s_FsImageDesc D_800D3B44[5];

// TODO: Check callers, `chara` might always be player instead.
void func_800CED74(s_SubCharacter* chara, bool arg1);

void func_800CF0B0(void);

void func_800CF0B8(void);

/** Func related to the UFO ending cutscene? */
void func_800CFC34(void);

void func_800D0500(void);

void func_800D1040(s32 arg0);

void func_800D1330(s16);

void Map_WorldObjectsInit(void);

void Map_WorldObjectsUpdate(void);

s32 func_800D1D40(void);

void func_800D2170(s32 arg0);

void func_800D2364(void);

void func_800D32D0(void);

void func_800D1AE4(void);

void func_800D1EB8(s32 arg0, s32* arg1, s32* arg2);

#endif
