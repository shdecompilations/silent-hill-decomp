#ifndef _MAPS_CHARACTERS_MONSTERCYBIL_H
#define _MAPS_CHARACTERS_MONSTERCYBIL_H

/* @brief Monster Cybil character control states. */
typedef enum _MonsterCybilControl
{
    MonsterCybilControl_None = 0,
    MonsterCybilControl_1    = 1,
    MonsterCybilControl_2    = 2,
    MonsterCybilControl_3    = 3,
    MonsterCybilControl_4    = 4,
    MonsterCybilControl_5    = 5,
    MonsterCybilControl_6    = 6,
    MonsterCybilControl_7    = 7,
    MonsterCybilControl_8    = 8,
    MonsterCybilControl_9    = 9,
    MonsterCybilControl_10   = 10,
    MonsterCybilControl_11   = 11,
    MonsterCybilControl_12   = 12,
    MonsterCybilControl_13   = 13
} e_MonsterCybilControl;

/** @brief Monster Cybil character animation indices. */
typedef enum _MonsterCybilAnim
{
    MonsterCybilAnim_Still = 0,
    MonsterCybilAnim_1     = 1,
    MonsterCybilAnim_2     = 2,
    MonsterCybilAnim_3     = 3,
    MonsterCybilAnim_4     = 4,
    MonsterCybilAnim_5     = 5,
    MonsterCybilAnim_6     = 6,
    MonsterCybilAnim_7     = 7,
    MonsterCybilAnim_8     = 8,
    MonsterCybilAnim_9     = 9,
    MonsterCybilAnim_10    = 10,
    MonsterCybilAnim_11    = 11,
    MonsterCybilAnim_12    = 12
} e_MonsterCybilAnim;

/** @brief Monster Cybil character model bone indices. */
typedef enum _MonsterCybilBone
{
    MonsterCybilBone_Root = 0
} e_MonsterCybilBone;

extern s_AnimInfo MONSTER_CYBIL_ANIM_INFOS[];

/*s_AnimInfo MONSTER_CYBIL_ANIM_INFOS[] = {
    { Anim_BlendLinear, ANIM_STATUS(0, false), false, ANIM_STATUS(0, false), { Q12(0)  }, NO_VALUE, 0  },
    { Anim_PlayLoop, ANIM_STATUS(0, true),  false, NO_VALUE,              { Q12(30) }, NO_VALUE, 1  },
    { Anim_BlendLinear, ANIM_STATUS(1, false), false, ANIM_STATUS(1, true),  { Q12(1)  }, NO_VALUE, 0  },
    { Anim_PlayLoop, ANIM_STATUS(1, true),  false, NO_VALUE,              { func_800D8898 },  0,        23 },
    { Anim_BlendLinear, ANIM_STATUS(2, false), false, ANIM_STATUS(2, true),  { Q12(5)  }, NO_VALUE, 24 }
};*/

extern s16     D_800EA776;
extern s16     D_800EA7D4;
extern s16     D_800EA7D6;
extern s16     D_800EA816;
extern s16     D_800EA836;
extern s16     D_800EA856; // Keyframe index.
extern s16     D_800EA894;
extern s16     D_800EA896; // Relative keyframe?
extern s16     D_800EBB48;
extern s16     D_800EBB4A;
extern s_Model g_MonsterCybil_ExtraModel; // Some kind of attachment or another `s_SubCharacter` instance?
extern VECTOR  D_800ED560;
extern VECTOR3 D_800ED570;

void func_800D87B0(s_SubCharacter* monsterCybil);

void func_800D8848(s_Model* modelUpper);

s32 func_800D8898(s_AnimInfo* animInfo);

void MonsterCybil_Update(s_SubCharacter* monsterCybil, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

void MonsterCybil_Init(s_SubCharacter* monsterCybil, s_Model* modelUpper);

void func_800D8A90(s_SubCharacter* monsterCybil);

void func_800D8B14(s_SubCharacter* monsterCybil, s_Model* modelUpper);

void func_800D8D7C(s_SubCharacter* monsterCybil, s_Model* modelUpper, GsCOORDINATE2* boneCoords);

void func_800D9790(s_SubCharacter* monsterCybil, s_Model* modelUpper);

void func_800D99E4(s_SubCharacter* monsterCybil, s_Model* modelUpper, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

void func_800D9AAC(s_SubCharacter* monsterCybil, s_Model* modelUpper);

void func_800D9AB4(s_SubCharacter* monsterCybil, s_Model* modelUpper, GsCOORDINATE2* boneCoords);

void func_800DA9C8(s_SubCharacter* monsterCybil, s_Model* modelUpper, GsCOORDINATE2* boneCoords);

void func_800DB4CC(s_SubCharacter* monsterCybil, s_Model* modelUpper, GsCOORDINATE2* boneCoords);

bool func_800DB6FC(VECTOR3* pos);

void func_800DB748(s_SubCharacter* monsterCybil);

bool func_800DB81C(s_SubCharacter* monsterCybil);

s32 func_800DB930(void);

s32 func_800DBA48(s_SubCharacter* monsterCybil);

bool func_800DBD64(s_SubCharacter* monsterCybil);

void func_800DBE5C(s_SubCharacter* monsterCybil);

void func_800DC018(s_SubCharacter* monsterCybil);

#endif
