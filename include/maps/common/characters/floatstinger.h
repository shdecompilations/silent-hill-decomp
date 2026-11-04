#ifndef _MAPS_CHARACTERS_FLOATSTINGER_H
#define _MAPS_CHARACTERS_FLOATSTINGER_H

/** @brief Floatstinger character flags. */
typedef enum _FloatstingerFlags
{
    FloatstingerFlag_None = 0,
    FloatstingerFlag_0    = 1 << 0,
    FloatstingerFlag_1    = 1 << 1,
    FloatstingerFlag_2    = 1 << 2,
    FloatstingerFlag_3    = 1 << 3,
    FloatstingerFlag_4    = 1 << 4
} e_FloatstingerFlags;

/** @brief Floatstinger character control states. */
typedef enum _FloatstingerControl
{
    FloatstingerControl_None = 0,

    FloatstingerControl_2    = 2,
    FloatstingerControl_3    = 3,

    FloatstingerControl_5    = 5,
    FloatstingerControl_6    = 6
} e_FloatstingerControl;

/** @brief Floatstinger character animation indices. */
typedef enum _FloatstingerAnim
{
    FloatstingerAnim_Still = 0,
    FloatstingerAnim_1     = 1,
    FloatstingerAnim_2     = 2,
    FloatstingerAnim_3     = 3,
    FloatstingerAnim_4     = 4,
    FloatstingerAnim_5     = 5,
    FloatstingerAnim_6     = 6,
    FloatstingerAnim_7     = 7,
    FloatstingerAnim_8     = 8,
    FloatstingerAnim_9     = 9,
    FloatstingerAnim_10    = 10,
    FloatstingerAnim_11    = 11,
    FloatstingerAnim_12    = 12
} e_FloatstingerAnim;

typedef struct
{
    s32 field_0[2];
    s32 field_8[2];
    s32 field_10;
} s_func_800D4458;

/*s_AnimInfo FLOATSTINGER_ANIM_INFOS[] = {
    { Anim_BlendLinear, NO_VALUE, false, ANIM_STATUS(FloatstingerAnim_Still, false), { Q12(0) }, NO_VALUE, 0 },
    { NULL, ANIM_STATUS(FloatstingerAnim_Still, false), false, ANIM_STATUS(FloatstingerAnim_Still, false), { Q12(0) }, 0, 0 },
    { Anim_BlendLinear, ANIM_STATUS(FloatstingerAnim_1, false), false, ANIM_STATUS(FloatstingerAnim_1, true), { Q12(4) }, NO_VALUE, 1 },
    { Anim_PlayOnce, ANIM_STATUS(FloatstingerAnim_1, true), false, ANIM_STATUS(FloatstingerAnim_9, false), { Q12(32) }, 1, 31 },
    { Anim_BlendLinear, ANIM_STATUS(FloatstingerAnim_2, false), false, ANIM_STATUS(FloatstingerAnim_2, true), { Q12(5) }, NO_VALUE, 32 },
    { Anim_PlayOnce, ANIM_STATUS(FloatstingerAnim_2, true), false, ANIM_STATUS(FloatstingerAnim_9, false), { Q12(20) }, 32, 57 },
    { Anim_BlendLinear, ANIM_STATUS(FloatstingerAnim_3, false), false, ANIM_STATUS(FloatstingerAnim_3, true), { Q12(10) }, NO_VALUE, 58 },
    { Anim_PlayOnce, ANIM_STATUS(FloatstingerAnim_3, true), false, ANIM_STATUS(FloatstingerAnim_11, false), { Q12(20) }, 58, 78 },
    { Anim_BlendLinear, ANIM_STATUS(FloatstingerAnim_4, false), false, ANIM_STATUS(FloatstingerAnim_4, true), { Q12(20) }, NO_VALUE, 116 },
    { Anim_PlayOnce, ANIM_STATUS(FloatstingerAnim_4, true), false, ANIM_STATUS(FloatstingerAnim_7, false), { Q12(0) }, 116, 136 },
    { Anim_BlendLinear, ANIM_STATUS(FloatstingerAnim_5, false), false, ANIM_STATUS(FloatstingerAnim_5, true), { Q12(2) }, NO_VALUE, 85 },
    { Anim_PlayOnce, ANIM_STATUS(FloatstingerAnim_5, true), false, ANIM_STATUS(FloatstingerAnim_3, false), { Q12(4) }, 85, 115 },
    { Anim_BlendLinear, ANIM_STATUS(FloatstingerAnim_6, false), false, ANIM_STATUS(FloatstingerAnim_6, true), { Q12(20) }, NO_VALUE, 116 },
    { Anim_PlayLoop, ANIM_STATUS(FloatstingerAnim_6, true), false, ANIM_STATUS(FloatstingerAnim_11, false), { Q12(0) }, 116, 136 },
    { Anim_BlendLinear, ANIM_STATUS(FloatstingerAnim_7, false), false, ANIM_STATUS(FloatstingerAnim_7, true), { Q12(20) }, NO_VALUE, 137 },
    { Anim_PlayLoop, ANIM_STATUS(FloatstingerAnim_7, true), false, NO_VALUE, { Q12(0) }, 137, 158 },
    { Anim_BlendLinear, ANIM_STATUS(FloatstingerAnim_8, false), false, ANIM_STATUS(FloatstingerAnim_8, true), { Q12(20) }, NO_VALUE, 159 },
    { Anim_PlayLoop, ANIM_STATUS(FloatstingerAnim_8, true), false, NO_VALUE, { Q12(20) }, 159, 190 },
    { Anim_BlendLinear, ANIM_STATUS(FloatstingerAnim_9, false), false, ANIM_STATUS(FloatstingerAnim_9, true), { Q12(5) }, NO_VALUE, 191 },
    { Anim_PlayLoop, ANIM_STATUS(FloatstingerAnim_9, true), false, NO_VALUE, { Q12(0) }, 191, 222 },
    { Anim_BlendLinear, ANIM_STATUS(FloatstingerAnim_10, false), false, ANIM_STATUS(FloatstingerAnim_10, true), { Q12(20) }, NO_VALUE, 223 },
    { Anim_PlayLoop, ANIM_STATUS(FloatstingerAnim_10, true), false, NO_VALUE, { Q12(20) }, 223, 254 },
    { Anim_BlendLinear, ANIM_STATUS(FloatstingerAnim_11, false), false, ANIM_STATUS(FloatstingerAnim_11, true), { Q12(20) }, NO_VALUE, 78 },
    { Anim_PlayLoop, ANIM_STATUS(FloatstingerAnim_11, true), false, NO_VALUE, { Q12(0) }, 78, 79 },
    { Anim_BlendLinear, ANIM_STATUS(FloatstingerAnim_12, false), false, ANIM_STATUS(FloatstingerAnim_12, true), { Q12(4) }, NO_VALUE, 1 },
    { Anim_PlayOnce, ANIM_STATUS(FloatstingerAnim_12, true), false, ANIM_STATUS(FloatstingerAnim_9, false), { Q12(32) }, 1, 31 }
};*/

extern s_func_800D4458 D_800D780C[];
extern u8              D_800D7848[];
extern q19_12          D_800D7858;
extern q3_12           D_800D785C;
extern s32             D_800D7860;
extern s32             D_800D799C;
extern void          (*D_800D7A04[])(void);
extern SVECTOR         D_800D7A20[];
extern SVECTOR         D_800D7A30;
extern s16             D_800D7A38[];
extern u8              D_800D7A58[];
extern SVECTOR         D_800D7A5C[];
extern GsCOORDINATE2*  D_800DB928;
extern q3_12           D_800DB898;
extern s16             D_800DB89A;
extern q3_12           D_800DB89C;
extern s16             D_800DB8A8[];
extern VECTOR3         D_800DB8C8[];

void Floatstinger_Update(s_SubCharacter* floatstinger, s_AnmHeader* anmHdr, GsCOORDINATE2* coords);

void Floatstinger_Init(s_SubCharacter* floatstinger);

void func_800D1968(s_SubCharacter* floatstinger);

void Floatstinger_ControlUpdate(s_SubCharacter* floatstinger);

void func_800D1BF8(s_SubCharacter* floatstinger);

void func_800D2B90(s_SubCharacter* floatstinger);

void func_800D341C(void);

void func_800D3424(s_SubCharacter* floatstinger);

void func_800D3564(s_SubCharacter* floatstinger);

void func_800D35F0(s_SubCharacter* floatstinger);

void func_800D37E8(s_SubCharacter* floatstinger, s_AnmHeader* anmHdr);

void func_800D3AD4(s_SubCharacter* floatstinger);

void func_800D3F84(VECTOR3* floatstinger, s32 arg1, s32 arg2);

void func_800D41F0(s_SubCharacter* floatstinger);

bool func_800D4458(s_SubCharacter* floatstinger, VECTOR3* arg1);

// Returns angle.
q19_12 func_800D48A4(s_SubCharacter* floatstinger, q3_12 dist, q3_12 headingAngle);

void func_800D4A3C(s_SubCharacter* floatstinger, VECTOR3* pos, q3_12 angle);

#endif
