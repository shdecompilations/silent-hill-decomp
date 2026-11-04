#ifndef _MAPS_CHARACTERS_INCUBUS_H
#define _MAPS_CHARACTERS_INCUBUS_H

/** @brief Incubus character control states. */
typedef enum _IncubusControl
{
    IncubusControl_None = 0,
    IncubusControl_1    = 1,
    IncubusControl_2    = 2,
    IncubusControl_3    = 3,
    IncubusControl_4    = 4,
    IncubusControl_5    = 5,
    IncubusControl_6    = 6,
    IncubusControl_7    = 7,
    IncubusControl_8    = 8,
    IncubusControl_9    = 9,
    IncubusControl_10   = 10,
    IncubusControl_11   = 11,
    IncubusControl_12   = 12,
    IncubusControl_13   = 13
} e_IncubusControl;

/** @brief Incubus character animation indices. */
typedef enum _IncubusAnim
{
    IncubusAnim_Still = 0,
    IncubusAnim_1     = 1,
    IncubusAnim_2     = 2,
    IncubusAnim_3     = 3,
    IncubusAnim_4     = 4,
    IncubusAnim_5     = 5
} e_IncubusAnim;

/** @brief Incubus character model bone indices. */
typedef enum _IncubusBone
{
    IncubusBone_2 = 2
} e_IncubusBone;

extern s_AnimInfo INCUBUS_ANIM_INFOS[];

extern s_SfxVolume D_800EC8C8[13];

extern u8 D_800EC8FC[340]; // Related to Incubus keyframes.

bool func_800DD964(void);

void func_800DD98C(bool disableDamage);

void func_800DD9B0(s_SubCharacter* chara);

void func_800DD9D4(s_SubCharacter* chara);

void func_800DD9F8(s_SubCharacter* chara);

bool Incubus_Init(s_SubCharacter* incubus, GsCOORDINATE2* boneCoords); // 0x800DDA1C

void func_800DDB3C(s_SubCharacter* chara, GsCOORDINATE2* boneCoords);

void func_800DDB68(s_SubCharacter* chara, s32 soundIdx);

/** Incubus SFX indices related to keyframes. */
s32 func_800DDBA4(s32 idx);

void func_800DDBBC(s_SubCharacter* incubus);

void func_800DDCC4(s_SubCharacter* incubus);

void func_800DDDB0(s_SubCharacter* incubus);

void func_800DDDD8(s_SubCharacter* incubus);

void func_800DDEEC(s_SubCharacter* incubus);

void func_800DDF14(s_SubCharacter* incubus);

void func_800DDF3C(s_SubCharacter* incubus, GsCOORDINATE2* boneCoords);

void func_800DE2A4(s_SubCharacter* incubus, GsCOORDINATE2* boneCoords);

void func_800DE68C(s_SubCharacter* incubus, GsCOORDINATE2* boneCoords);

void func_800DEA54(s_SubCharacter* incubus, GsCOORDINATE2* boneCoords);

q19_12 func_800DEA90(void);

void func_800DEAF4(s_SubCharacter* incubus);

void func_800DEBA8(s_SubCharacter* incubus);

void func_800DEC38(s_SubCharacter* incubus);

void func_800DEC74(s_SubCharacter* incubus, GsCOORDINATE2* boneCoords);

void func_800DED68(s_SubCharacter* incubus, GsCOORDINATE2* boneCoords);

// TODO: Rename to `Incubus_MovementUpdate`.
void func_800DEE44(s_SubCharacter* incubus);

// Plays footstep SFX?
void func_800DEE90(s_SubCharacter* incubus, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

void func_800DEF50(s_SubCharacter* incubus, GsCOORDINATE2* boneCoords);

void func_800DEFE8(s_SubCharacter* incubus, GsCOORDINATE2* boneCoords);

void func_800DF044(s_SubCharacter* incubus, GsCOORDINATE2* boneCoords);

void func_800DF074(s_SubCharacter* incubus);

void Incubus_Update(s_SubCharacter* incubus, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

#endif
