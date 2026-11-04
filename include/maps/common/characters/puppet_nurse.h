#ifndef _MAPS_CHARACTERS_PUPPETNURSE_H
#define _MAPS_CHARACTERS_PUPPETNURSE_H

/** @brief Puppet Nurse or Puppet Doctor character flags. */
typedef enum _PuppetNurseFlags
{
    PuppetNurseFlag_None = 0,
    PuppetNurseFlag_0    = 1 << 0,
    PuppetNurseFlag_1    = 1 << 1
} e_PuppetNurseFlags;

/** @brief Puppet Nurse or Puppet Doctor character control states. */
typedef enum _PuppetNurseControl
{
    PuppetNurseControl_1  = 1,
    PuppetNurseControl_2  = 2,
    PuppetNurseControl_3  = 3, // Doctor-specific.
    PuppetNurseControl_4  = 4, // Nurse-specific.
    PuppetNurseControl_5  = 5,
    PuppetNurseControl_6  = 6, // Nurse-specific.
    PuppetNurseControl_7  = 7, // Doctor-specific.
    PuppetNurseControl_8  = 8,
    PuppetNurseControl_9  = 9,
    PuppetNurseControl_10 = 10,
    PuppetNurseControl_11 = 11,
    PuppetNurseControl_12 = 12,
    PuppetNurseControl_13 = 13
} e_PuppetNurseControl;

/** @brief Puppet Nurse or Puppet Doctor character animation indices. */
typedef enum _PuppetNurseAnim
{
    PuppetNurseAnim_Still = 0,
    PuppetNurseAnim_1     = 1,
    PuppetNurseAnim_2     = 2,
    PuppetNurseAnim_3     = 3,
    PuppetNurseAnim_4     = 4,
    PuppetNurseAnim_5     = 5,
    PuppetNurseAnim_6     = 6,
    PuppetNurseAnim_7     = 7, // Nurse-specific.
    PuppetNurseAnim_8     = 8, // Doctor-specific.
    PuppetNurseAnim_9     = 9,
    PuppetNurseAnim_10    = 10,
    PuppetNurseAnim_14    = 14,
    PuppetNurseAnim_15    = 15,
    PuppetNurseAnim_16    = 16,
    PuppetNurseAnim_17    = 17,
    PuppetNurseAnim_18    = 18,
    PuppetNurseAnim_19    = 19,
    PuppetNurseAnim_20    = 20,
    PuppetNurseAnim_21    = 21
} e_PuppetNurseAnim;

/** @brief Puppet Nurse or Puppet Doctor character swappable mesh IDs. */
typedef enum _PuppetNurseSwappableMesh
{
    PuppetNurseSwappableMesh_Head = 0
} e_PuppetNurseSwappableMesh;

/** @brief Puppet Nurse or Puppet Doctor character variant mesh IDs. */
typedef enum _PuppetNurseVariantMesh
{
    PuppetNurseVariantMesh_None  = 0,
    PuppetNurseVariantMesh_Head1 = 1,
    PuppetNurseVariantMesh_Head2 = 2,
    PuppetNurseVariantMesh_Head3 = 3
} e_PuppetNurseVariantMesh;

typedef struct
{
    u16 sfxId;
    u16 vol;
} s_SfxPair;

extern s32 g_PuppetNurseHurtSfxIdxs[4];
extern s_SfxPair g_NursePuppetSfxs[18];
extern u8 g_NursePuppet_AnimSfxs[580];
extern s32 g_PuppetNurse_AnimStatus0[2];
extern s32 g_PuppetNurse_AnimStatus1[2];
extern u8  g_PuppetNurse_AnimStatus2[2];
extern u8  g_PuppetNurse_AnimStatus3[4];
extern u8  g_PuppetNurse_AnimStatus4[4];

s32 PuppetNurse_HurtSfxIdGet(s_SubCharacter* nurse);

void PuppetNurse_SfxPlay(s_SubCharacter* nurse, s32 idx);

s32 PuppetNurse_AnimSfxGet(s32 idx);

void sharedFunc_800CDA88_3_s03(s_SubCharacter* nurse);

void PuppetNurse_UpdateMain(s_SubCharacter* chara, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

void PuppetNurse_Init(s_SubCharacter* nurse, bool isDoctor);

void PuppetNurse_Update(s_SubCharacter* nurse, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

void PuppetDoctor_Update(s_SubCharacter* doctor, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

/** Checks damage angle. */
bool PuppetNurse_SomeAngleCheck(s_SubCharacter* nurse);

void PuppetNurse_DamageHandle(s_SubCharacter* nurse);

void PuppetNurse_Move(s_SubCharacter* nurse);

/** Checks if some special animation is set. */
bool sharedFunc_800CE398_3_s03(s32 animStatus);

void PuppetNurse_Control1(s_SubCharacter* nurse);

void PuppetNurse_Control2(s_SubCharacter* nurse);

void PuppetNurse_Control3_4(s_SubCharacter* nurse, bool isDoctor);

bool sharedFunc_800CE7C8_3_s03(s_SubCharacter* nurse);

void PuppetNurse_Control5(s_SubCharacter* nurse);

void PuppetNurse_Control6_7(s_SubCharacter* nurse, bool isDoctor);

void PuppetNurse_Control8(s_SubCharacter* nurse);

s32 sharedFunc_800CEEAC_3_s03(void);

void sharedFunc_800CEEE0_3_s03(s_SubCharacter* nurse);

void sharedFunc_800CF0B4_3_s03(s_SubCharacter* nurse);

bool sharedFunc_800CF294_3_s03(s_SubCharacter* nurse, q19_12 dist);

void PuppetNurse_Control9(s_SubCharacter* nurse);

s32 sharedFunc_800CF600_3_s03(s_SubCharacter* nurse);

void sharedFunc_800CF7F4_3_s03(s_SubCharacter* nurse);

bool sharedFunc_800CF90C_3_s03(s_SubCharacter* nurse);

void PuppetNurse_Control12(s_SubCharacter* nurse);

void PuppetNurse_Control10(s_SubCharacter* nurse);

void PuppetNurse_Control11(s_SubCharacter* nurse);

void PuppetNurse_Control13(s_SubCharacter* nurse);

void PuppetNurse_Control(s_SubCharacter* nurse);

void sharedFunc_800D02E4_3_s03(s_SubCharacter* nurse, GsCOORDINATE2* boneCoords);

void sharedFunc_800D03E4_3_s03(s_SubCharacter* nurse);

void PuppetNurse_AnimUpdate(s_SubCharacter* nurse, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

void sharedFunc_800D0828_3_s03(s_SubCharacter* nurse, GsCOORDINATE2* boneCoords);

// TODO: `PuppetNurse_CollisionUpdate`?
void sharedFunc_800D0968_3_s03(s_SubCharacter* nurse, GsCOORDINATE2* boneCoords);

#endif
