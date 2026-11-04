#ifndef _MAPS_CHARACTERS_SPLITHEAD_H
#define _MAPS_CHARACTERS_SPLITHEAD_H

/** @brief Split Head character flags. */
typedef enum _SplitHeadFlags
{
    SplitHeadFlag_None = 0,
    SplitHeadFlag_0    = 1 << 0,
    SplitHeadFlag_1    = 1 << 1,
    SplitHeadFlag_2    = 1 << 2,
    SplitHeadFlag_3    = 1 << 3,
    SplitHeadFlag_4    = 1 << 4,
    SplitHeadFlag_5    = 1 << 5,
    SplitHeadFlag_6    = 1 << 6,
    SplitHeadFlag_7    = 1 << 7,
    SplitHeadFlag_8    = 1 << 8,
    SplitHeadFlag_9    = 1 << 9
} e_SplitHeadFlags;

/** @brief Split Head character control states. */
typedef enum _SplitHeadControl
{
    SplitHeadControl_None = 0,
    SplitHeadControl_1    = 1,
    SplitHeadControl_2    = 2,
    SplitHeadControl_3    = 3,
    SplitHeadControl_4    = 4,
    SplitHeadControl_5    = 5,
    SplitHeadControl_6    = 6,
    SplitHeadControl_7    = 7,
    SplitHeadControl_8    = 8
} e_SplitHeadControl;

/** @brief Split Head character animation indices. */
typedef enum _SplitHeadAnim
{
    SplitHeadAnim_Still       = 0,
    SplitHeadAnim_1           = 1,
    SplitHeadAnim_BiteAttack  = 2,
    SplitHeadAnim_3           = 3, // something to idle.
    SplitHeadAnim_4           = 4, // something to idle.
    SplitHeadAnim_5           = 5, // hit left?
    SplitHeadAnim_6           = 6, // hit right?
    SplitHeadAnim_7           = 7, // die
    SplitHeadAnim_8           = 8, // splayed? to 13
    SplitHeadAnim_9           = 9, // walk forward, head shaking?
    SplitHeadAnim_StandIdle   = 10,
    SplitHeadAnim_WalkForward = 11,
    SplitHeadAnim_12          = 12, // laying down
    SplitHeadAnim_13          = 13, // splayed to idle?
    SplitHeadAnim_14          = 14  // something idle?
} e_SplitHeadAnim;

/** @brief Split Head character swappable mesh IDs. */
typedef enum _SplitHeadSwappableMesh
{
    SplitHeadSwappableMesh_Head = 0
} e_SplitHeadSwappableMesh;

/** @brief Split Head character variant mesh IDs. */
typedef enum _SplitHeadVariantMesh
{
    SplitHeadVariantMesh_None = 0,
    SplitHeadVariantMesh_1    = 1, // } Open and closed heads?
    SplitHeadVariantMesh_2    = 2  // }
} e_SplitHeadVariantMesh;

typedef struct
{
    s32 field_0; // Distance/Magnitude?
    s32 field_4; // X
    s32 field_8; // Z
    u8  field_C; // 0 / 1 / 2
    u8  field_D; // `bool`
    u8  field_E;
    s8  unk_F;
} s_sharedFunc_800D4594_1_s05;

typedef struct
{
    SVECTOR3 vec;
    s16      idx;
} s_sharedData_800D5AB0_1_s05;

/*s_AnimInfo SPLIT_HEAD_ANIM_INFOS[] = {
    { Anim_BlendLinear, NO_VALUE, false, ANIM_STATUS(0, false), { Q12(0) }, NO_VALUE, 0 },
    { NULL, ANIM_STATUS(0, false), false, ANIM_STATUS(0, false), { Q12(0) }, 0, 0 },
    { Anim_BlendLinear, ANIM_STATUS(1, false), false, ANIM_STATUS(1, true), { Q12(2) }, NO_VALUE, 0 },
    { Anim_PlayLoop, ANIM_STATUS(1, true), false, NO_VALUE, { Q12(3.5f) }, 0, 13 },
    { Anim_BlendLinear, ANIM_STATUS(2, false), false, ANIM_STATUS(2, true), { Q12(8) }, NO_VALUE, 14 },
    { Anim_PlayOnce, ANIM_STATUS(2, true), false, ANIM_STATUS(10, false), { Q12(0) }, 14, 40 },
    { Anim_BlendLinear, ANIM_STATUS(3, false), false, ANIM_STATUS(3, true), { Q12(18) }, NO_VALUE, 41 },
    { Anim_PlayOnce, ANIM_STATUS(3, true), false, ANIM_STATUS(10, false), { Q12(12) }, 41, 50 },
    { Anim_BlendLinear, ANIM_STATUS(4, false), false, ANIM_STATUS(4, true), { Q12(12) }, NO_VALUE, 51 },
    { Anim_PlayOnce, ANIM_STATUS(4, true), false, ANIM_STATUS(10, false), { Q12(12) }, 51, 60 },
    { Anim_BlendLinear, ANIM_STATUS(5, false), false, ANIM_STATUS(5, true), { Q12(12) }, NO_VALUE, 61 },
    { Anim_PlayOnce, ANIM_STATUS(5, true), false, ANIM_STATUS(6, false), { Q12(12) }, 61, 63 },
    { Anim_BlendLinear, ANIM_STATUS(6, false), false, ANIM_STATUS(6, true), { Q12(12) }, NO_VALUE, 78 },
    { Anim_PlayOnce, ANIM_STATUS(6, true), false, ANIM_STATUS(10, false), { Q12(12) }, 78, 80 },
    { Anim_BlendLinear, ANIM_STATUS(7, false), false, ANIM_STATUS(7, true), { Q12(4) }, NO_VALUE, 94 },
    { Anim_PlayOnce, ANIM_STATUS(7, true), false, ANIM_STATUS(12, false), { Q12(2) }, 94, 114 },
    { Anim_BlendLinear, ANIM_STATUS(8, false), false, ANIM_STATUS(8, true), { Q12(0.5f) }, NO_VALUE, 115 },
    { Anim_PlayOnce, ANIM_STATUS(8, true), false, ANIM_STATUS(13, false), { Q12(1.5f) }, 115, 131 },
    { Anim_BlendLinear, ANIM_STATUS(9, false), false, ANIM_STATUS(9, true), { Q12(4) }, NO_VALUE, 132 },
    { Anim_PlayOnce, ANIM_STATUS(9, true), false, ANIM_STATUS(1, false), { Q12(0) }, 132, 161 },
    { Anim_BlendLinear, ANIM_STATUS(10, false), false, ANIM_STATUS(10, true), { Q12(8) }, NO_VALUE, 162 },
    { Anim_PlayLoop, ANIM_STATUS(10, true), false, NO_VALUE, { Q12(6) }, 162, 171 },
    { Anim_BlendLinear, ANIM_STATUS(11, false), false, ANIM_STATUS(11, true), { Q12(4) }, NO_VALUE, 172 },
    { Anim_PlayLoop, ANIM_STATUS(11, true), false, NO_VALUE, { Q12(0) }, 172, 201 },
    { Anim_BlendLinear, ANIM_STATUS(12, false), false, ANIM_STATUS(12, true), { Q12(8) }, NO_VALUE, 114 },
    { Anim_PlayLoop, ANIM_STATUS(12, true), false, NO_VALUE, { Q12(0) }, 114, 115 },
    { Anim_BlendLinear, ANIM_STATUS(13, false), false, ANIM_STATUS(13, true), { Q12(8) }, NO_VALUE, 131 },
    { Anim_PlayLoop, ANIM_STATUS(13, true), false, NO_VALUE, { Q12(0) }, 131, 132 },
    { Anim_BlendLinear, ANIM_STATUS(14, false), false, ANIM_STATUS(14, true), { Q12(2) }, NO_VALUE, 34 },
    { Anim_PlayLoop, ANIM_STATUS(14, true), false, NO_VALUE, { Q12(2) }, 34, 36 }
};*/

extern s_AnimInfo SPLIT_HEAD_ANIM_INFOS[];

/** Set to Split Head bone coords. */
extern GsCOORDINATE2* sharedData_800D8610_1_s05;

extern q3_12 g_SplitHead_MovementOffsetX;
extern q3_12 g_SplitHead_MovementOffsetZ;

extern q19_12 g_SplitHead_MovementAnimDuration;

extern s16 sharedData_800D5A8C_1_s05;

extern u8 sharedData_800D5AAE_1_s05;
extern u8 sharedData_800D5AAF_1_s05;

extern u8       sharedData_800D5CF8_1_s05[9];
extern u8       sharedData_800D5D08_1_s05[];
extern VECTOR3  sharedData_800D8618_1_s05[]; // Size 9. Effects positions??
extern SVECTOR3 sharedData_800D5A90_1_s05[]; // Q7.8

extern s_sharedData_800D5AB0_1_s05 sharedData_800D5AB0_1_s05[];
extern s16                         sharedData_800D5BE0_1_s05[6][3];

extern q19_12 sharedData_800D8684_1_s05; // Offset X for player target.
extern q19_12 sharedData_800D8688_1_s05; // Offset Z for player target.

/** @brief Updates an active Split Head character.
 *
 * @param splitHead Split Head character to update.
 * @param anmHdr Character animation data.
 * @param boneCoords Character bone coords.
 */
void SplitHead_Update(s_SubCharacter* splitHead, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

void SplitHead_Init(s_SubCharacter* splitHead);

void sharedFunc_800CF990_1_s05(s_SubCharacter* splitHead);

void SplitHead_DamageTake(s_SubCharacter* splitHead);

void SplitHead_ControlUpdate(s_SubCharacter* splitHead);

void SplitHead_Control_1(s_SubCharacter* splitHead);

void SplitHead_Control_2(s_SubCharacter* splitHead);

void SplitHead_Control_3(s_SubCharacter* splitHead);

void SplitHead_MovementUpdate(s_SubCharacter* splitHead);

void SplitHead_AnimUpdate(s_SubCharacter* splitHead, s_AnmHeader* anmHdr);

void sharedFunc_800D2D74_1_s05(s_SubCharacter* splitHead);

// Updates something to do with a player target position.
void sharedFunc_800D3388_1_s05(s_SubCharacter* splitHead, q19_12* offsetX, q19_12* offsetZ);

void SplitHead_Control_4(s_SubCharacter* splitHead);

void SplitHead_Control_5(s_SubCharacter* splitHead);

void SplitHead_Control_6(s_SubCharacter* splitHead);

void SplitHead_Control_7(s_SubCharacter* splitHead);

void SplitHead_Control_8(s_SubCharacter* splitHead);

void sharedFunc_800D3AFC_1_s05(s_SubCharacter* splitHead);

void sharedFunc_800D3B30_1_s05(s_SubCharacter* splitHead);

void sharedFunc_800D4070_1_s05(s_SubCharacter* splitHead);

/** Accumulates an unknown XZ offset. */
void SplitHead_MovementOffsetSet(q19_12 offsetX, q19_12 offsetZ);

bool sharedFunc_800D4530_1_s05(s_SubCharacter* splitHead);

void sharedFunc_800D4594_1_s05(s_sharedFunc_800D4594_1_s05* arg0, q19_12 posX, q19_12 posZ, VECTOR3* vec0, VECTOR3* vec1);

#endif
