#ifndef _MAPS_MAP7_S03_H
#define _MAPS_MAP7_S03_H

#include "bodyprog/game.h"

#include <psyq/libgs.h>

#include "bodyprog/chara/dms.h"

#define HAS_PlayerState_Unk51
#define HAS_PlayerState_Unk131
#define HAS_PlayerState_Reset
#define HAS_PlayerState_Unk153
#define HAS_PlayerState_Unk186
#define HAS_PlayerState_Unk162
#define HAS_PlayerState_Unk171
#define HAS_PlayerState_Unk172
#define HAS_PlayerState_Unk173
#define HAS_PlayerState_Unk174
#define HAS_PlayerState_Unk176
#define HAS_PlayerState_Unk177
#define HAS_PlayerState_Unk184
#define HAS_PlayerState_Unk178
#define HAS_PlayerState_Unk179
#define HAS_PlayerState_Unk180
#define HAS_PlayerState_Unk181
#define HAS_PlayerState_Unk182
#define HAS_PlayerState_Unk183
#define KEYFRAME_PlayerState_Unk153 816
#define KEYFRAME_PlayerState_Unk162 837

#define HAS_Chara_EndingCybil
#define HAS_Chara_EndingDahlia
#define HAS_Chara_Alessa
#define HAS_Chara_EndingKaufmann
#define HAS_Chara_BloodyLisa
#define HAS_Chara_Incubator
#define HAS_Chara_BloodyIncubator
#define MAP_CHARA_COUNT 7

// Config used by `Map_RoomIdxGet`.
#define MAP_ROOM_MIN_X    Q12(-200.0f)
#define MAP_ROOM_MAX_X    Q12(200.0f)
#define MAP_ROOM_MIN_Z    Q12(-160.0f)
#define MAP_ROOM_MAX_Z    Q12(0.0f)

#define MAP_FIELD_4C_COUNT 200
#define MAP_BLOOD_SPLAT_COUNT_MAX 150

#include "maps/common/shared.h"

typedef struct
{
    VECTOR3 field_0;
    q19_12  field_C;
    q19_12  field_10;
    q19_12  field_14;
    q19_12  field_18;
    q19_12  field_1C;
    q19_12  field_20;
    s32     field_24;
    s32     field_28;
    q19_12  field_2C;
    s32     field_30;
    q19_12  field_34;
    s32     flags_38;
} s_800ED7E0_ptr;
STATIC_ASSERT_SIZEOF(s_800ED7E0_ptr, 60);

typedef struct
{
    s16 field_0;
    s16 field_2;
    s16 field_4;
    s16 field_6;
    s16 field_8;
    s16 field_A;
    u16 field_C;
    u16 field_E;
} s_800F3D48_0_0;

struct _800F3D48;
struct _800F3D48_0;

typedef struct
{
    s32     field_0;
    s16     field_4;
    s16     field_6;
    s32     field_8;
    s16     field_C; // Used for switch case in `func_800D9DF8`.
    s16     field_E;
    q19_12  field_10; // Timer.
    s32     field_14;
    VECTOR  field_18;
    VECTOR  field_28; // Q19.12 | Offset.
    s32     field_38;
    s32     field_3C;
    s32     field_40;
    struct _800F3D48_0* (*field_44)(struct _800F3D48*);
    void                (*field_48)(struct _800F3D48*, s_800F3D48_0_0*);
} s_800F3D48_4;
STATIC_ASSERT_SIZEOF(s_800F3D48_4, 76);

typedef struct _800F3D48_0
{
    s32                 field_0;
    struct _800F3D48_0* next_4;
    s32                 field_8; // Some kind of enum? 0 / 1 / 2
} s_800F3D48_0;
STATIC_ASSERT_SIZEOF(s_800F3D48_0, 12);

typedef struct _800F3D48
{
    s_800F3D48_0* ptr_0;
    s_800F3D48_4  field_4;
} s_800F3D48;
STATIC_ASSERT_SIZEOF(s_800F3D48, 80);

typedef struct _800F3DAC
{
    s32    field_0;
    u8     unk_4[4];
    q19_12 timer;
    q19_12 timer_C;
    s32    field_10;
    q19_12 field_14; // Values seem like `Q12_ANGLE`, but haven't confirmed how it's used yet.
    s32    field_18;
    s32    field_1C;
    s32    field_20;
    u8     unk_24[4];
    s32    field_28[29];
    u8     unk_9C[4];
    s32    field_A0[29];
    u8     unk_114[4];
    MATRIX mat_118[29];
    u8     unk_4B8[32];
    q19_12 rotationZ_4D8; // Passed as `z` to `ApplyRotMatrix`.
    u8     unk_4DC[4];
    s32    field_4E0;
    s32    field_4E4;
    s32    field_4E8;
    s32    coordIdx_4EC;
    void   (*field_4F0)(struct _800F3DAC*);
    s32    field_4F4;
} s_800F3DAC;
STATIC_ASSERT_SIZEOF(s_800F3DAC, 0x4F8);

// Note: Beginning is similar to M4S03 `s_800E0988`, which could be a substruct used here.
typedef struct
{
    q19_12          positionX;
    q19_12          positionZ;
    MATRIX          mat_8;
    s_SubCharacter* field_28;
    GsCOORDINATE2*  coords_2C; // Bone coords.
    VECTOR3         playerPosition_30;
    q19_12          velocityX_3C; // } Some kind of velocity based on `PLAYER_CHARA().position` - `this.playerPosition_30`
    q19_12          velocityZ_40; // }
    s32             field_44;
    s32             field_48;
} s_D_800F48A8;
STATIC_ASSERT_SIZEOF(s_D_800F48A8, 0x4C); // Assumed size.

typedef struct
{
    VECTOR3  field_0; // Q19.12
    s8       unk_C[20];
    SVECTOR3 field_20;
    s8       unk_28[8];
    s32      field_30;
    s32      field_34;
    s8       unk_38[4];
} s_func_800D5EC0_A60;

typedef struct
{
    s8                  unk_0[4];
    VECTOR              field_4;
    s8                  unk_10[32];
    s8                  field_34[41];
    u8                  field_5D[61 * 41];
    s8                  field_A22[41];
    s8                  field_A4B[5];
    s32                 field_A50;
    s32                 field_A54;
    q19_12              field_A58; // Angle.
    s32                 field_A5C;
    s_func_800D5EC0_A60 field_A60[20];
} s_func_800D5EC0;

typedef struct
{
    VECTOR3  field_0;
    s8       unk_C[20];
    SVECTOR3 field_20; // Q3.12 | Position?
    s8       unk_26[10];
    s32      field_30;
} s_func_800D68C4_A08;

typedef struct
{
    s8                  unk_0[4];
    VECTOR              field_4;
    s8                  unk_14[32];
    s8                  field_34[49];
    u8                  field_65[49 * 49];
    s8                  field_9C6[49];
    s8                  unk_9F7[5];
    s32                 field_9FC;
    s32                 field_A00;
    s32                 field_A04;
    s_func_800D68C4_A08 field_A08[100];
} s_func_800D68C4;

typedef struct
{
    VECTOR field_0;
    VECTOR field_10;
    s32    field_20;
    s8     unk_24[4];
    s32    field_28;
    s8     unk_1C[4];
    s32    field_30;
    s32    field_34;
} s_func_800D71B0_498;

typedef struct
{
    s8                  unk_0[4];
    VECTOR              field_4;
    s8                  unk_10[32];
    s8                  field_34[41];
    u8                  field_5D[41 * 25];
    s8                  field_45E[41];
    s8                  unk_487[5];
    s32                 field_48C;
    s32                 field_490;
    s32                 field_494;
    s_func_800D71B0_498 field_498[200];
} s_func_800D71B0;

typedef struct
{
    u16 sfxId_0;  /** `e_SfxId` */
    u16 sfxVol_2;
    u16 field_4;
    u16 field_6;
} s_800EC770;

typedef struct
{
    VECTOR3* vec_0;
    q19_12   field_4;
    VECTOR3* vec_8;
    q19_12   field_C;
    s16      pos_10;
    s16      field_12;
    s16      field_14;
    s16      field_16;
    s16      total_max_spd_18; // Guessed name.
    s16      field_1A;
    s16      field_1C;
    s16      field_1E;
} s_800F4B40_1C;

typedef struct
{
    s16 now_spd_0; // Guessed name.
    s16 field_2;
    s16 field_4;
    s16 field_6;
    s16 field_8;
} s_800F4B40_A4;

typedef struct
{
    q3_12       x_0;
    q3_12       y_2;
    s_PrimColor color_4;
} s_800F4B40_118;
STATIC_ASSERT_SIZEOF(s_800F4B40_118, 8);

typedef struct
{
    s32            field_0;
    s32            field_4;
    q19_12         timer;
    s32            field_C;
    q19_12         timer_10;
    u8             unk_14[4];
    u8             field_18[3];
    u8             unk_1B[1];
    s_800F4B40_1C  field_1C[2];
    s_800F4B40_1C* field_5C;
    s_800F4B40_1C* field_60;
    s_800F4B40_1C  field_64[2];
    s_800F4B40_A4  field_A4[2];
    VECTOR3        field_B8[8];  // Guessed type based on size, likely incorrect.
    s_800F4B40_118 field_118[6][16];
} s_800F4B40;

typedef struct
{
    VECTOR3 field_0;
    u8      unk_C[4];
    MATRIX  mat_10;
} s_800F3D58;

typedef struct
{
    s32           field_0;
    s_800F4B40_1C field_4[2];
} s_func_800E1FE0;

typedef struct
{
    s_800F3D48_0* field_0;
    s32    field_4;
    s32    field_8;
    VECTOR field_C; // Q19.12 | Bone offset?
    s32    field_1C;
    s8     unk_20[4];
    s32    field_24;
    s32    field_28;
} s_func_800DAD54;

extern s32 D_800F2438; // Length of `D_800F3D48` array.

extern s_800F3D48* D_800F3D48;

extern s_800F3DAC* D_800F3DAC; // Pointer to `s_800F3DAC[30]`.

extern s32 D_800F3DB0;

extern q19_12 D_800F3DB4; // Timer.

extern s_D_800F48A8 D_800F48A8;

extern s32 D_800ED73C; // } RNG related values?
extern s32 D_800ED740; // }

extern s32 D_800ED744[9];

extern u16 D_800ED768;

extern u16 g_Cutscene_MapMsgAudioCmds[];

extern u16 D_800ED7B4;

extern s_800ED7E0_ptr* D_800ED7E0[];

extern u16 D_800ED898;

extern s32 D_800ED8AC;

extern s_800ED7E0_ptr* D_800ED8B0[];

extern s32 D_800ED8E8;

extern s_800ED7E0_ptr* D_800ED8EC[];

extern u16 D_800ED88C;

extern s32 D_800ED980;

extern s_800ED7E0_ptr* D_800ED984[];

extern u16 D_800ED9B4;

extern s32 D_800ED9B8;

extern s_800ED7E0_ptr* D_800ED9BC[];

extern s32 D_800EDA04;

extern u8 g_Cutscene_PrevPlayerPlaybackState; /** `e_AnimPlaybackState` */

extern s32 D_800EDA0C;

extern s16 D_800F23D0;

extern s32 D_800F23D4;

extern s32 D_800F2430;
extern s32 D_800F2434;

extern s32 D_800F3D8C;

extern s_WorldObjectPose g_WorldObject_Bin;

extern s_WorldObjectPose g_WorldObject_Nu;

extern s_WorldObjectPose g_WorldObject_Baby;

extern GsCOORDINATE2 g_NpcBoneCoords[HarryBone_Count];

extern q19_12 g_Cutscene_Timer; // Count of objects to load?

extern VECTOR3 g_WorldObject_UnkPos;

extern u8 g_Cutscene_MapMsgAudioIdx;

extern u8 D_800F4805;

extern u8 D_800F4806; // Index for `D_800ED230`.

extern u8 g_Cutscene_UpdateHero; // `bool`
extern u8 g_Cutscene_UpdateSibyl; // `bool`
extern u8 g_Cutscene_UpdateDaria; // `bool`
extern u8 g_Cutscene_UpdateArsia; // `bool`
extern u8 g_Cutscene_UpdateKau; // `bool`
extern u8 g_Cutscene_UpdateMar; // `bool`
extern u8 g_Cutscene_UpdateBar; // `bool`
extern u8 g_Cutscene_UpdateLitl; // `bool`
extern u8 g_Cutscene_UpdateBos; // `bool`
extern u8 g_Cutscene_UpdateBin; // `bool`
extern u8 g_Cutscene_UpdateNu; // `bool`
extern u8 g_Cutscene_UpdateBaby; // `bool`
extern u8 D_800F4813;
extern u8 D_800F4814;
extern u8 D_800F4815; // `bool`
extern u8 D_800F4816; // `bool`
extern u8 D_800F4817; // `bool`
extern u8 D_800F4818; // `bool`
extern u8 D_800F4819;
extern u8 D_800F481A;
extern u8 D_800F481B;

extern q20_12 D_800F3D98[5]; // Time.

extern q19_12 D_800F4820; // Time.

extern s32 D_800F4824;

extern s32 D_800F4828;

extern q3_12 D_800F482C; // Rotation for "BIN" world object.

extern s32 D_800F4830;

extern s32 D_800F4834;

extern s32 D_800F4838;

extern s32 D_800F483C;

extern s_WorldObjectModel g_WorldObject_Real;
extern s_WorldObjectModel g_WorldObject_Ura;
extern s_WorldObjectModel g_WorldObject_Under;

extern s32 D_800F48A4;

extern s32 D_800F2418[];

extern s32 D_800EB008;
extern s32 D_800EB00C;

extern s32 D_800EB010[];

extern s32 D_800EB410[];

extern s32 D_800EB810;

extern s32 D_800EB814[];

extern s32 D_800EBC14;

extern s32 D_800EBC18[]; // Possibly array of pointers.

extern VECTOR D_800EC018[];

extern s_800EC770 D_800EC770[];

extern s_800F3D58 D_800F3D58;

extern s_800F4B40 D_800F4B40;

extern s_800F3D48_0 D_800EC3EC;
extern s_800F3D48_0 D_800EC34C;
extern s_800F3D48_0 D_800EC680[];
extern s_800F3D48_0 D_800EC53C;
extern s_800F3D48_0 D_800EC614;
extern s_800F3D48_0 D_800EC5A8;

extern s_800F3D48 D_800F2448;
extern s32        D_800F3D90;

extern s_800F3D48_0  D_800EC6EC;
extern s_800F3D48_0* const D_800CAE30[3];
extern s_800F3D48_0  D_800EC1B8;

extern s32 D_800EC758[];
extern s32 D_800EC764[];

extern VECTOR3 D_800F4B30;

extern s_FsImageDesc D_800ED218;
extern s_FsImageDesc D_800ED220;
extern s_FsImageDesc D_800ED228;

extern s_DmsHeader* D_800ED230[];

extern VECTOR3 D_800ED244;
extern VECTOR3 D_800ED250;

extern s_func_800E1FE0 D_800ED274[18];

extern VECTOR3 g_Cutscene_CameraPositionTarget;
extern VECTOR3 g_Cutscene_CameraLookAtTarget;

extern VECTOR3 D_800F47D8;
extern SVECTOR3 D_800F47E8;

extern VECTOR D_800EC798[];

void func_800D663C(void);

s32 func_800D6750(s32* sxy);

void func_800D6788(void); // Assumed return type.

void func_800D6804(const VECTOR3* from, const VECTOR3* to);

void func_800D6FA0(void);

s32 func_800D70B4(s32* sxy);

void func_800D70EC(void);

void func_800D7144(VECTOR3*);

void func_800D71A4(s32 arg0);

void func_800D74F4(s32 x, s32 y, s32 val);

s32 func_800D7CB4(s32* sxy);

void func_800D7CEC(void);

void func_800D7CF8(void);

void func_800D7D74(s32);

void func_800D7E50(VECTOR3*);

void func_800D7ED0(void);

void func_800D7EF0(void);

PACKET* func_800D7F10(void);

void func_800D7F20(u8* arg0);

void func_800D7F2C(GsOT_TAG* ot, s32 arg1, q19_12 angle, q19_12 dist0, q19_12 dist1, s16 x1, s16 y1, s32 arg7);

s32 func_800D822C(SVECTOR* worldPos, s16* outScreenX, s16* outScreenY);

void func_800D82AC(GsOT_TAG* ot, s32 arg1, s32 arg2, s32 arg3);

s32 func_800D8438(s32 idx);

void func_800D8738(void* ot, s32 abr);

void func_800D87D4(void* ot);

void func_800D8858(s32 arg0);

void func_800D88C4(void);

void func_800D88CC(void);

void func_800D88D8(void);

s_800F3D48_0* func_800D88E8(s_800F3D48* arg0);

s_800F3D48* func_800D905C(void);

void func_800D90C8(void);

void func_800D9114(s_800F3D48* arg0);

void func_800D9394(void);

void func_800D947C(void);

void func_800D952C(void);

s_800F3D48_0* func_800D95D4(s_800F3D48* arg0);

s_800F3D48_0* func_800D9740(s_800F3D48* arg0);

q19_12 func_800DA08C(q19_12 arg0, q19_12 arg1, q19_12 arg2);

q19_12 func_800DA420(VECTOR* result);

void func_800DA4B4(VECTOR* arg0, VECTOR* arg1);

s32 func_800DA4EC(s32 min, s32 max);

void func_800DA774(s_800ED7E0_ptr*);

s_800F3D48_0* func_800DA884(s_800F3D48* arg0);

void func_800DACFC(VECTOR3* vec, s32 arg1, s32 arg2);

void func_800DAD54(void);

void func_800DB154(GsCOORDINATE2*);

void func_800DB288(void);

void func_800DB608(void);

void func_800DBABC(void);

void func_800DBAE8(const VECTOR3* pos, s32 idx);

void func_800DBBA0(void);

void func_800DBBD8(MATRIX* mat);

void func_800DBC18(q19_12 damageAmount);

bool func_800DBCA4(MATRIX* mat, VECTOR3* outVec);

void func_800DC3EC(s_800F3DAC* arg0);

void func_800DC49C(s_800F3DAC* arg0);

void func_800DC544(GsOT_TAG* ot);

/** @brief Checks if the distance between two positions is within a given maximum threshold.
 *
 * @param from First position (Q19.12).
 * @param to Second world position (Q19.12).
 * @param distMax Maximum distance between positions.
 * @return `true` if the positions are within the maximum distance of each other, `false` otherwise.
 */
bool Math_DistanceCheck(const VECTOR3* from, const VECTOR3* to, q19_12 distMax);

bool func_800DC6DC(VECTOR* arg0, VECTOR* arg1, VECTOR3* arg2, s_800F3DAC* arg3);

void func_800DCD94(MATRIX* mat, VECTOR3* pos);

void func_800DCDDC(s_800F3DAC* arg0, const VECTOR3* arg1, const VECTOR3* arg2);

void func_800DCF94(void);

void func_800DCFF8(void);

void func_800DCF94(void);

void func_800DD044(void);

s_800F3DAC* func_800DD090(void);

void func_800DD0EC(const VECTOR3* pos, s32 coordIdx);

void func_800DD240(const VECTOR3* pos);

void func_800DD260(const VECTOR3* arg0, const VECTOR3* arg1);

void func_800DD2C8(const VECTOR3* arg0, const VECTOR3* arg1);

void func_800DD32C(const VECTOR3* arg0, const VECTOR3* arg1);

void func_800DD3D4(void* arg0, q19_12 scaleX, q19_12 scaleY, q19_12 scaleZ);

void func_800DD464(const VECTOR3* arg0);

void func_800DD4CC(s_800F3DAC* arg0);

void func_800DD594(const VECTOR3* pos, s_SubCharacter* chara, GsCOORDINATE2* boneCoords, s32 arg3);

void func_800DD62C(const VECTOR3* pos, s_SubCharacter* chara, GsCOORDINATE2* boneCoords);

void func_800DD67C(const VECTOR3* pos, s_SubCharacter* chara, GsCOORDINATE2* boneCoords);

void func_800DD6CC(void);
void func_800DD738(const VECTOR3* pos0, const VECTOR3* pos1, q19_12 rotZ, q19_12 timer);
void func_800DD7D0(VECTOR3* charaPos);
void func_800DD868(void);
void func_800DD8CC(VECTOR3* charaPos);

void func_800E0A34(void);
void func_800E0B28(s32 arg0);
void func_800E0B5C(s32 arg0);

void func_800E0B90(void);
void func_800E0C10(void);

/** Camera function. */
void func_800E14DC(s_SubCharacter* player, s_SubCharacter* otherChara, bool warpCamera);

void func_800E16FC(void);

void func_800E1788(s32 arg0);

/** Deep copy of data from `src` into `dest`, preserving `dest` vector pointers. */
void func_800E17B8(s_800F4B40_1C* dest, s_800F4B40_1C* src);

void func_800E1854(void);

void func_800E1FE0(s_func_800E1FE0* arg0);

void func_800E20A4(s_800F4B40_1C* arg0, s_800F4B40_A4* arg1, s_800F4B40_1C* arg2, s_800F4B40_1C* arg3);

void func_800E22AC(VECTOR3* arg0, s32* arg1, s16* mv_pos, s16* now_spd, VECTOR3* arg4, s32 arg5, VECTOR3* arg6, s32 arg7, s16 total_max_spd);

q19_12 func_800E2444(q19_12 dampingRate, q19_12 current, q19_12 target);

void func_800E24A0(s_800F4B40_1C* arg0);

void func_800E2664(s32 arg0, s16 arg1);

void func_800E27D0(s32 arg0, s16 arg1, s32 arg2, VECTOR3* pos);

// Some kind of RNG? Constants don't seem well known?
q3_12 func_800E28F4(void);

void func_800E2968(s_800F4B40_118* arg0, s32 colCount, s32 rowCount, DVECTOR* arg3, DVECTOR* arg4,
                   s32 arg5, s32 arg6, q19_12 angle0, q19_12 angle1, s_PrimColor* color, s32 arg10, u8 arg11);

void func_800E2C28(s_800F4B40_118* arg0, s32 colCount, s32 rowCount, s32 zDepth, s32 arg4);

void Map_CutsceneObjectsUpdate(void);

void func_800E3390(void);

void func_800E3B6C(void);

void func_800E3C48(void);

void func_800E3D18(void);

void func_800E3E84(void);

/** Ending cutscene event? */
void func_800E3F30(void);

void func_800E4714(void);

void func_800E514C(void);

void func_800E62CC(void);

void func_800E70F0(void);

void func_800E7380(void);

void func_800E787C(void);

void func_800E86BC(void);

void func_800E8D20(void);

void func_800E9260(e_CharaId charaId, s32 arg1);

void func_800E941C(void);

void func_800E9444(e_CharaId charaId, s_SubCharacter* chara);

void func_800E9490(s_SubCharacter* chara);

void func_800E9498(void);

void func_800E94AC(void);

void func_800E94C0(void);

void func_800E94F4(void);

void Map_WorldObjectsInit(void);

void Map_WorldObjectsUpdate(void);

void func_800E9874(void);

void func_800E98EC(void);

void func_800E9AC8(void);

void func_800E9C28(void);


// TODO: Alphabetise these.



void func_800D5D24(void);

bool func_800D5D48(void);

void func_800D5E78(void);

void func_800D5EC0(void);

void func_800D600C(s32 x, s32 y, s32 val);

void func_800D60E4(void);

s32 func_800D621C(s32 arg0, s32 arg1);

void func_800D625C(void);

void func_800D6548(void);

void func_800D6690(VECTOR* arg0, q19_12 rotY);

void func_800D6788(void);

void func_800D68C4(void);

void func_800D6A10(s32 x, s32 y, s32 val);

void func_800D6ADC(void);

s32 func_800D6BCC(s32 arg0, s32 arg1);

void func_800D6C0C(void);

void func_800D6FF4(VECTOR* arg0, q19_12 rotY);

void func_800D70EC(void);

void func_800D7144(VECTOR3* arg0);

void func_800D71B0(VECTOR3* pos, s32 arg1);

void func_800D75D0(void);

s32 func_800D77A4(s32 arg0, s32 arg1);

void func_800D77E4(void);

void func_800D7AD0(void);

void func_800D7B78(void);

void func_800D7BF4(VECTOR* arg0, q19_12 rotY);

void func_800D7D08(void);

void func_800D7D74(s32 arg0);

void func_800D7E50(VECTOR3* arg0);

void func_800D8454(s32* arg0, s32 x, s32 y, s32 s);

void func_800D947C(void);

void func_800D952C(void);

void func_800D917C(void);

void func_800D8954(s_800F3D48* arg0, s_800F3D48_0_0* arg1);

void func_800D8CD4(q19_12 rotZ, SVECTOR* rot);

void func_800D8D90(s_800F3D48* arg0, s_800F3D48_0_0* arg1);

void func_800D982C(s_800F3D48* arg0);

void func_800D997C(s_800F3D48* arg0);

void func_800D9AA0(s_800F3D48* arg0);

void func_800D9B90(s_800F3D48* arg0);

s_800F3D48_0* func_800D9C20(s_800F3D48* arg0);

s_800F3D48_0* func_800D9DF8(s_800F3D48* arg0);

void func_800DA04C(void);

/** Does some kind of timer computation. */
q19_12 func_800DA178(q19_12 arg0, q19_12 arg1, q19_12 arg2, q19_12 arg3, q19_12 arg4);

s32 func_800DA1F4(VECTOR3* arg0, VECTOR* arg1, q19_12 arg2, s32 arg3, s32 arg4);

void func_800DA550(s_800ED7E0_ptr* arg0);

void func_800DA774(s_800ED7E0_ptr* arg0);

void func_800DA9F8(VECTOR3* arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

void func_800DAB18(VECTOR3* arg0, s32 arg1, s32 arg2, s32 arg3);

void func_800DAC04(VECTOR3* arg0, void* arg1, s32 arg2);

// Does something wth character bones. Likely Harry-specific.
void func_800DAD54(void);

// Does something wth character bones. Likely Harry-specific.
s_func_800DAD54* func_800DAD90(void);

void func_800DADE0(s_func_800DAD54* arg0, s_800F3D48_0_0* arg1);

// Does something wth character bones. Likely Harry-specific.
void func_800DB154(GsCOORDINATE2* boneCoords);

void func_800DB288(void);

void func_800DB6D0(MATRIX* arg0, const VECTOR* rot0, const VECTOR* rot1, const MATRIX* mat, s32 arg4);

s32 func_800DBA08(SVECTOR* arg0, s32* arg1, s32* arg2);

void func_800DBD94(s_800F3DAC* arg0, GsOT_TAG* ot);

#endif
