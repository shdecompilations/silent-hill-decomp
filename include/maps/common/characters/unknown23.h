#ifndef _MAPS_CHARACTERS_UNKNOWN23_H
#define _MAPS_CHARACTERS_UNKNOWN23_H

typedef struct
{
    s32     field_0;
    s32     field_4;
    VECTOR  field_8;
    u8      unk_18[16];
    SVECTOR field_28;
    u8      unk_30[8];
    s32     field_38;
} s_func_800DF1D4_58C;

typedef struct
{
    s8                  unk_0[4];
    VECTOR              field_4;
    s8                  unk_10[32];
    s8                  field_34[41];
    u8                  field_5D[31 * 41];
    u8                  field_554[41];
    s8                  unk_57D[7];
    s32                 field_584;
    s8                  unk_588[4];
    s_func_800DF1D4_58C field_58C[100];
} s_func_800DF1D4;

typedef struct
{
    u8      unk_0[4];
    VECTOR3 field_4;
    u8      unk_10[0x574];
    s32     field_584;
    s32     field_588;
} s_func_800DFA48;

extern s_AnimInfo UNKKOWN_23_ANIM_INFOS[];

extern s32 D_800ECA50[];

extern q19_12 D_800F3DB8; // Timer.

void func_800DF1D4(void);

void func_800DF288(s32 x, s32 y, s32 val);

void func_800DF348(void);

s32 func_800DF418(s32 arg0, s32 arg1);

void func_800DF458(void);

void func_800DF750(void);

void func_800DF7F8(void);

void func_800DF84C(VECTOR* arg0, s32 arg1);

s32 func_800DF90C(void);

void func_800DF944(void);

void func_800DFA14(void);

void func_800DFA48(VECTOR3* arg0, VECTOR3* arg1);

s32 func_800DFB04(void);

void func_800DFB2C(bool disableDamage);

void func_800DFB50(s_SubCharacter* chara);

void func_800DFB74(s_SubCharacter* chara);

bool Unknown23_Init(s_SubCharacter* chara, GsCOORDINATE2* boneCoords);

void func_800DFCE4(s_SubCharacter* chara);

void func_800DFE10(s_SubCharacter* chara);

void func_800DFEF0(s_SubCharacter* chara);

void func_800DFF28(s_SubCharacter* chara);

void func_800DFF44(s_SubCharacter* chara);

void func_800DFF60(s_SubCharacter* chara, GsCOORDINATE2* boneCoords);

q19_12 func_800E04C4(void);

void func_800E0528(s_SubCharacter* chara);

void func_800E05DC(s_SubCharacter* chara, GsCOORDINATE2* boneCoords);

void func_800E0670(s_SubCharacter* chara, GsCOORDINATE2* boneCoords);

void func_800E0728(s_SubCharacter* chara);

void func_800E0774(s_SubCharacter* chara, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

void func_800E07F0(s_SubCharacter* chara, GsCOORDINATE2* boneCoords);

void func_800E0888(s_SubCharacter* chara, GsCOORDINATE2* boneCoords);

void func_800E08E4(s_SubCharacter* chara, GsCOORDINATE2* boneCoords);

void func_800E0914(s_SubCharacter* chara);

void Unknown23_Update(s_SubCharacter* chara, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

#endif
