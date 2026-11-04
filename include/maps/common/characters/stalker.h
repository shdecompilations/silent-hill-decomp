#ifndef _MAPS_CHARACTERS_STALKER_H
#define _MAPS_CHARACTERS_STALKER_H

/** @brief Stalker character flags. */
typedef enum _StalkerFlags
{
    StalkerFlag_None         = 0,
    StalkerFlag_0            = 1 << 0,
    StalkerFlag_1            = 1 << 1,
    StalkerFlag_2            = 1 << 2,
    StalkerFlag_3            = 1 << 3,
    StalkerFlag_4            = 1 << 4,
    StalkerFlag_5            = 1 << 5,
    StalkerFlag_6            = 1 << 6,
    StalkerFlag_7            = 1 << 7,
    StalkerFlag_8            = 1 << 8,
    StalkerFlag_9            = 1 << 9,
    StalkerFlag_10           = 1 << 10,
    StalkerFlag_11           = 1 << 11,
    StalkerFlag_WarpRotation = 1 << 12, /** Warps to target heading angle. */
    StalkerFlag_13           = 1 << 13,
    StalkerFlag_14           = 1 << 14
} s_StalkerFlags;

/** @brief Stalker character control states. */
typedef enum _StalkerControl
{
    StalkerControl_None = 0,
    StalkerControl_1    = 1,
    StalkerControl_2    = 2,
    StalkerControl_3    = 3,
    StalkerControl_4    = 4,
    StalkerControl_5    = 5,
    StalkerControl_6    = 6,
    StalkerControl_7    = 7,
    StalkerControl_8    = 8,
    StalkerControl_9    = 9,
    StalkerControl_10   = 10,
    StalkerControl_11   = 11,
    StalkerControl_12   = 12,
    StalkerControl_13   = 13
} e_StalkerControl;

/** @brief Stalker character animation indices. */
typedef enum _StalkerAnim
{
    StalkerAnim_Still = 0,
    StalkerAnim_1     = 1,
    StalkerAnim_2     = 2,
    StalkerAnim_3     = 3,
    StalkerAnim_4     = 4,
    StalkerAnim_5     = 5,
    StalkerAnim_6     = 6,
    StalkerAnim_7     = 7,
    StalkerAnim_8     = 8,
    StalkerAnim_9     = 9,
    StalkerAnim_10    = 10,
    StalkerAnim_11    = 11,
    StalkerAnim_12    = 12,
    StalkerAnim_13    = 13,
    StalkerAnim_14    = 14,
    StalkerAnim_15    = 15,
    StalkerAnim_16    = 16,
    StalkerAnim_17    = 17,
    StalkerAnim_18    = 18,
    StalkerAnim_19    = 19,
    StalkerAnim_20    = 20,
    StalkerAnim_21    = 21,
    StalkerAnim_22    = 22,
    StalkerAnim_23    = 23,
    StalkerAnim_24    = 24,
    StalkerAnim_25    = 25,
    StalkerAnim_26    = 26,
    StalkerAnim_27    = 27,
    StalkerAnim_28    = 28,
    StalkerAnim_29    = 29,
    StalkerAnim_30    = 30,
    StalkerAnim_31    = 31,
    StalkerAnim_32    = 32,
    StalkerAnim_33    = 33,
    StalkerAnim_34    = 34, // } Grab player front/back.
    StalkerAnim_35    = 35, // }
    StalkerAnim_36    = 36,
    StalkerAnim_37    = 37,
    StalkerAnim_38    = 38,
    StalkerAnim_39    = 39,
    StalkerAnim_40    = 40,
    StalkerAnim_41    = 41,
    StalkerAnim_42    = 42,
    StalkerAnim_43    = 43,
    StalkerAnim_44    = 44,
    StalkerAnim_45    = 45,
    StalkerAnim_46    = 46,
    StalkerAnim_47    = 47
} e_StalkerAnim;

/** @brief Stalker character swappable mesh IDs. */
typedef enum _StalkerSwappableMesh
{
    StalkerSwappableMesh_Head = 0
} e_StalkerSwappableMesh;

/** @brief Stalker character variant mesh IDs. */
typedef enum _StalkerVariantMesh
{
    StalkerVariantMesh_None = 0,
    StalkerVariantMesh_1    = 1, // @unused?
    StalkerVariantMesh_2    = 2  // Head variant?
} e_StalkerVariantMesh;

/*s_AnimInfo STALKER_ANIM_INFOS[] = {
    { Anim_BlendLinear, NO_VALUE, false, ANIM_STATUS(0, false), { Q12(0) }, NO_VALUE, 0 },
    { NULL, ANIM_STATUS(0, false), false, ANIM_STATUS(0, false), { Q12(0) }, 0, 0 },
    { Anim_BlendLinear, ANIM_STATUS(1, false), false, ANIM_STATUS(1, true), { Q12(5) }, NO_VALUE, 0 },
    { Anim_PlayOnce, ANIM_STATUS(1, true), false, ANIM_STATUS(26, false), { Q12(12) }, 0, 3 },
    { Anim_BlendLinear, ANIM_STATUS(2, false), false, ANIM_STATUS(2, true), { Q12(5) }, NO_VALUE, 4 },
    { Anim_PlayOnce, ANIM_STATUS(2, true), false, ANIM_STATUS(26, false), { Q12(6) }, 4, 8 },
    { Anim_BlendLinear, ANIM_STATUS(3, false), false, ANIM_STATUS(3, true), { Q12(5) }, NO_VALUE, 9 },
    { Anim_PlayOnce, ANIM_STATUS(3, true), false, ANIM_STATUS(26, false), { Q12(6) }, 9, 19 },
    { Anim_BlendLinear, ANIM_STATUS(4, false), false, ANIM_STATUS(4, true), { Q12(4) }, NO_VALUE, 20 },
    { Anim_PlayOnce, ANIM_STATUS(4, true), false, ANIM_STATUS(17, false), { Q12(6) }, 20, 24 },
    { Anim_BlendLinear, ANIM_STATUS(5, false), false, ANIM_STATUS(5, true), { Q12(4) }, NO_VALUE, 25 },
    { Anim_PlayOnce, ANIM_STATUS(5, true), false, ANIM_STATUS(17, false), { Q12(6) }, 25, 33 },
    { Anim_BlendLinear, ANIM_STATUS(6, false), false, ANIM_STATUS(6, true), { Q12(0) }, NO_VALUE, 34 },
    { NULL, ANIM_STATUS(6, true), false, ANIM_STATUS(0, false), { Q12(0) }, 34, 49 },
    { Anim_BlendLinear, ANIM_STATUS(7, false), false, ANIM_STATUS(7, true), { Q12(16) }, NO_VALUE, 50 },
    { Anim_PlayOnce, ANIM_STATUS(7, true), false, ANIM_STATUS(30, false), { Q12(0) }, 50, 69 },
    { Anim_BlendLinear, ANIM_STATUS(8, false), false, ANIM_STATUS(8, true), { Q12(32) }, NO_VALUE, 70 },
    { Anim_PlayLoop, ANIM_STATUS(8, true), false, NO_VALUE, { Q12(13) }, 70, 82 },
    { Anim_BlendLinear, ANIM_STATUS(9, false), false, ANIM_STATUS(9, true), { Q12(32) }, NO_VALUE, 83 },
    { Anim_PlayLoop, ANIM_STATUS(9, true), false, NO_VALUE, { Q12(13) }, 83, 95 },
    { Anim_BlendLinear, ANIM_STATUS(10, false), false, ANIM_STATUS(10, true), { Q12(16) }, NO_VALUE, 96 },
    { Anim_PlayOnce, ANIM_STATUS(10, true), false, ANIM_STATUS(18, false), { Q12(8) }, 96, 106 },
    { Anim_BlendLinear, ANIM_STATUS(11, false), false, ANIM_STATUS(11, true), { Q12(4) }, NO_VALUE, 107 },
    { Anim_PlayOnce, ANIM_STATUS(11, true), false, ANIM_STATUS(27, false), { Q12(8) }, 107, 128 },
    { Anim_BlendLinear, ANIM_STATUS(12, false), false, ANIM_STATUS(12, true), { Q12(4) }, NO_VALUE, 129 },
    { Anim_PlayOnce, ANIM_STATUS(12, true), false, ANIM_STATUS(27, false), { Q12(8) }, 129, 157 },
    { Anim_BlendLinear, ANIM_STATUS(13, false), false, ANIM_STATUS(13, true), { Q12(4) }, NO_VALUE, 158 },
    { Anim_PlayOnce, ANIM_STATUS(13, true), false, ANIM_STATUS(27, false), { Q12(10) }, 158, 175 },
    { Anim_BlendLinear, ANIM_STATUS(14, false), false, ANIM_STATUS(14, true), { Q12(10) }, NO_VALUE, 176 },
    { Anim_PlayOnce, ANIM_STATUS(14, true), false, ANIM_STATUS(46, false), { Q12(10) }, 176, 263 },
    { Anim_BlendLinear, ANIM_STATUS(15, false), false, ANIM_STATUS(15, true), { Q12(5) }, NO_VALUE, 264 },
    { Anim_PlayOnce, ANIM_STATUS(15, true), false, ANIM_STATUS(47, false), { Q12(10) }, 264, 294 },
    { Anim_BlendLinear, ANIM_STATUS(16, false), false, ANIM_STATUS(16, true), { Q12(0) }, NO_VALUE, 295 },
    { NULL, ANIM_STATUS(16, true), false, ANIM_STATUS(0, false), { Q12(0) }, 295, 322 },
    { Anim_BlendLinear, ANIM_STATUS(17, false), false, ANIM_STATUS(17, true), { Q12(16) }, NO_VALUE, 323 },
    { Anim_PlayLoop, ANIM_STATUS(17, true), false, NO_VALUE, { Q12(4) }, 323, 332 },
    { Anim_BlendLinear, ANIM_STATUS(18, false), false, ANIM_STATUS(18, true), { Q12(16) }, NO_VALUE, 333 },
    { Anim_PlayLoop, ANIM_STATUS(18, true), false, NO_VALUE, { Q12(4) }, 333, 338 },
    { Anim_BlendLinear, ANIM_STATUS(19, false), false, ANIM_STATUS(19, true), { Q12(8) }, NO_VALUE, 339 },
    { Anim_PlayOnce, ANIM_STATUS(19, true), false, ANIM_STATUS(32, false), { Q12(1.5) }, 339, 344 },
    { Anim_BlendLinear, ANIM_STATUS(20, false), false, ANIM_STATUS(20, true), { Q12(8) }, NO_VALUE, 346 },
    { Anim_PlayOnce, ANIM_STATUS(20, true), false, ANIM_STATUS(33, false), { Q12(1.5) }, 346, 351 },
    { Anim_BlendLinear, ANIM_STATUS(21, false), false, ANIM_STATUS(21, true), { Q12(8) }, NO_VALUE, 353 },
    { Anim_PlayOnce, ANIM_STATUS(21, true), false, ANIM_STATUS(31, false), { Q12(1.5) }, 353, 360 },
    { Anim_BlendLinear, ANIM_STATUS(22, false), false, ANIM_STATUS(22, true), { Q12(16) }, NO_VALUE, 362 },
    { Anim_PlayOnce, ANIM_STATUS(22, true), false, ANIM_STATUS(27, false), { Q12(12) }, 362, 377 },
    { Anim_BlendLinear, ANIM_STATUS(23, false), false, ANIM_STATUS(23, true), { Q12(16) }, NO_VALUE, 378 },
    { Anim_PlayOnce, ANIM_STATUS(23, true), false, ANIM_STATUS(27, false), { Q12(12) }, 378, 393 },
    { Anim_BlendLinear, ANIM_STATUS(24, false), false, ANIM_STATUS(24, true), { Q12(16) }, NO_VALUE, 395 },
    { Anim_PlayOnce, ANIM_STATUS(24, true), false, ANIM_STATUS(26, false), { Q12(15) }, 395, 406 },
    { Anim_BlendLinear, ANIM_STATUS(25, false), false, ANIM_STATUS(25, true), { Q12(4) }, NO_VALUE, 408 },
    { Anim_PlayOnce, ANIM_STATUS(25, true), false, ANIM_STATUS(17, false), { Q12(10) }, 408, 417 },
    { Anim_BlendLinear, ANIM_STATUS(26, false), false, ANIM_STATUS(26, true), { Q12(16) }, NO_VALUE, 418 },
    { Anim_PlayLoop, ANIM_STATUS(26, true), false, NO_VALUE, { Q12(8) }, 418, 426 },
    { Anim_BlendLinear, ANIM_STATUS(27, false), false, ANIM_STATUS(27, true), { Q12(16) }, NO_VALUE, 427 },
    { Anim_PlayLoop, ANIM_STATUS(27, true), false, NO_VALUE, { Q12(4) }, 427, 442 },
    { Anim_BlendLinear, ANIM_STATUS(28, false), false, ANIM_STATUS(28, true), { Q12(20) }, NO_VALUE, 443 },
    { Anim_PlayLoop, ANIM_STATUS(28, true), false, NO_VALUE, { Q12(20) }, 443, 460 },
    { Anim_BlendLinear, ANIM_STATUS(29, false), false, ANIM_STATUS(29, true), { Q12(0) }, NO_VALUE, 461 },
    { Anim_PlayOnce, ANIM_STATUS(29, true), false, ANIM_STATUS(0, false), { Q12(0) }, 461, 476 },
    { Anim_BlendLinear, ANIM_STATUS(30, false), false, ANIM_STATUS(30, true), { Q12(4) }, NO_VALUE, 477 },
    { Anim_PlayLoop, ANIM_STATUS(30, true), false, NO_VALUE, { Q12(0) }, 477, 506 },
    { Anim_BlendLinear, ANIM_STATUS(31, false), false, ANIM_STATUS(31, true), { Q12(16) }, NO_VALUE, 360 },
    { Anim_PlayLoop, ANIM_STATUS(31, true), false, NO_VALUE, { Q12(0) }, 360, 361 },
    { Anim_BlendLinear, ANIM_STATUS(32, false), false, ANIM_STATUS(32, true), { Q12(16) }, NO_VALUE, 344 },
    { Anim_PlayLoop, ANIM_STATUS(32, true), false, NO_VALUE, { Q12(0) }, 344, 345 },
    { Anim_BlendLinear, ANIM_STATUS(33, false), false, ANIM_STATUS(33, true), { Q12(16) }, NO_VALUE, 351 },
    { Anim_PlayLoop, ANIM_STATUS(33, true), false, NO_VALUE, { Q12(0) }, 351, 352 },
    { Anim_BlendLinear, ANIM_STATUS(34, false), false, ANIM_STATUS(34, true), { Q12(48) }, NO_VALUE, 461 },
    { Anim_PlayOnce, ANIM_STATUS(34, true), false, ANIM_STATUS(9, false), { Q12(48) }, 461, 476 },
    { Anim_BlendLinear, ANIM_STATUS(35, false), false, ANIM_STATUS(35, true), { Q12(48) }, NO_VALUE, 461 },
    { Anim_PlayOnce, ANIM_STATUS(35, true), false, ANIM_STATUS(8, false), { Q12(48) }, 461, 476 },
    { Anim_BlendLinear, ANIM_STATUS(36, false), false, ANIM_STATUS(36, true), { Q12(32) }, NO_VALUE, 102 },
    { Anim_PlayOnce, ANIM_STATUS(36, true), false, ANIM_STATUS(18, false), { Q12(6) }, 102, 106 },
    { Anim_BlendLinear, ANIM_STATUS(37, false), false, ANIM_STATUS(37, true), { Q12(0) }, NO_VALUE, 477 },
    { Anim_PlayOnce, ANIM_STATUS(37, true), false, ANIM_STATUS(7, false), { Q12(0) }, 477, 506 },
    { Anim_BlendLinear, ANIM_STATUS(38, false), false, ANIM_STATUS(38, true), { Q12(12) }, NO_VALUE, 353 },
    { Anim_PlayOnce, ANIM_STATUS(38, true), false, ANIM_STATUS(26, false), { Q12(12) }, 353, 361 },
    { Anim_BlendLinear, ANIM_STATUS(39, false), false, ANIM_STATUS(39, true), { Q12(12) }, NO_VALUE, 339 },
    { Anim_PlayOnce, ANIM_STATUS(39, true), false, ANIM_STATUS(17, false), { Q12(12) }, 339, 345 },
    { Anim_BlendLinear, ANIM_STATUS(40, false), false, ANIM_STATUS(40, true), { Q12(12) }, NO_VALUE, 346 },
    { Anim_PlayOnce, ANIM_STATUS(40, true), false, ANIM_STATUS(18, false), { Q12(12) }, 346, 352 },
    { Anim_BlendLinear, ANIM_STATUS(41, false), false, ANIM_STATUS(41, true), { Q12(16) }, NO_VALUE, 378 },
    { Anim_PlayOnce, ANIM_STATUS(41, true), false, ANIM_STATUS(25, false), { Q12(6) }, 378, 383 },
    { Anim_BlendLinear, ANIM_STATUS(42, false), false, ANIM_STATUS(42, true), { Q12(4) }, NO_VALUE, 0 },
    { Anim_PlayOnce, ANIM_STATUS(42, true), false, ANIM_STATUS(26, false), { Q12(8) }, 0, 3 },
    { Anim_BlendLinear, ANIM_STATUS(43, false), false, ANIM_STATUS(43, true), { Q12(4) }, NO_VALUE, 9 },
    { Anim_PlayOnce, ANIM_STATUS(43, true), false, ANIM_STATUS(26, false), { Q12(4) }, 9, 19 },
    { Anim_BlendLinear, ANIM_STATUS(44, false), false, ANIM_STATUS(44, true), { Q12(4) }, NO_VALUE, 4 },
    { Anim_PlayOnce, ANIM_STATUS(44, true), false, ANIM_STATUS(26, false), { Q12(4) }, 4, 8 },
    { Anim_BlendLinear, ANIM_STATUS(45, false), false, ANIM_STATUS(45, true), { Q12(8) }, NO_VALUE, 158 },
    { Anim_PlayOnce, ANIM_STATUS(45, true), false, ANIM_STATUS(18, false), { Q12(1048564) }, 158, 175 }, // TODO: Unsure. Check data, the FFFF could be something else.
    { Anim_BlendLinear, ANIM_STATUS(46, false), false, ANIM_STATUS(46, true), { Q12(0) }, NO_VALUE, 263 },
    { Anim_PlayLoop, ANIM_STATUS(46, true), false, ANIM_STATUS(15, false), { Q12(0) }, 263, 264 },
    { Anim_BlendLinear, ANIM_STATUS(47, false), false, ANIM_STATUS(47, true), { Q12(0) }, NO_VALUE, 294 },
    { Anim_PlayLoop, ANIM_STATUS(47, true), false, ANIM_STATUS(15, false), { Q12(0) }, 294, 295 }
};*/

extern s_AnimInfo STALKER_ANIM_INFOS[];

// Next 4 set by `Stalker_Update`.
extern q19_12 sharedData_800E3A20_0_s00; // Health.
extern q19_12 sharedData_800E3A24_0_s00; // Health.
extern q19_12 sharedData_800E3A28_0_s00;
extern q19_12 sharedData_800E3A2C_0_s00;

void Stalker_Control_2(s_SubCharacter* stalker);

void Stalker_Control_3(s_SubCharacter* stalker);

void Stalker_Control_4(s_SubCharacter* stalker);

void Stalker_Control_5(s_SubCharacter* stalker);

void Stalker_Control_6(s_SubCharacter* stalker);

void Stalker_Control_7(s_SubCharacter* stalker);

void Stalker_Control_8(s_SubCharacter* stalker);

void Stalker_Control_9(s_SubCharacter* stalker);

void Stalker_Control_10(s_SubCharacter* stalker);

void Stalker_Control_11(s_SubCharacter* stalker);

void Stalker_Control_12(s_SubCharacter* stalker);

void Stalker_Control_13(s_SubCharacter* stalker);

void sharedFunc_800D67FC_0_s00(s_SubCharacter* stalker);

void sharedFunc_800D7E04_0_s00(s_SubCharacter* stalker, s32 sfxId);

void Stalker_Update(s_SubCharacter* stalker, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

void Stalker_Init(s_SubCharacter* stalker); // Matches across all maps except `map6_s03`.

void sharedFunc_800D3308_0_s00(s_SubCharacter* stalker);

void Stalker_ControlUpdate(s_SubCharacter* stalker);

void sharedFunc_800D6970_0_s00(s_SubCharacter* stalker, s_AnmHeader* animHdr, GsCOORDINATE2* boneCoords);

void Stalker_CollisionUpdate(s_SubCharacter* stalker);

void sharedFunc_800D7BE8_0_s00(s_SubCharacter* stalker);

#endif
