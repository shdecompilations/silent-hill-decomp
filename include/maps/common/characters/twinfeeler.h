#ifndef _MAPS_CHARACTERS_TWINFEELER_H
#define _MAPS_CHARACTERS_TWINFEELER_H

/** @brief Twinfeeler character flags. */
typedef enum _TwinfeelerFlags
{
    TwinfeelerFlag_None        = 0,
    TwinfeelerFlag_0           = 1 << 0,
    TwinfeelerFlag_Underground = 1 << 2,
} e_TwinfeelerFlags;

/** @brief Twinfeeler character control states. */
typedef enum _TwinfeelerControl
{
    TwinfeelerControl_None        = 0,
    TwinfeelerControl_WalkForward = 1,
    TwinfeelerControl_2           = 2,
    TwinfeelerControl_3           = 3,
    TwinfeelerControl_4           = 4,
    TwinfeelerControl_5           = 5,
    TwinfeelerControl_6           = 6,
    TwinfeelerControl_Stand       = 7,
    TwinfeelerControl_Attack      = 8,
    TwinfeelerControl_9           = 9,
    TwinfeelerControl_10          = 10,
    TwinfeelerControl_Idle        = 11,
    TwinfeelerControl_12          = 12,
    TwinfeelerControl_13          = 13
} e_TwinfeelerControl;

// TODO: Unsure if anims marked as unused are actually unused. Code doesn't seem to reference them. Stuns also unclear.
/** @brief Twinfeeler character animation indices. */
typedef enum _TwinfeelerAnim
{
    TwinfeelerAnim_Still           = 0,
    TwinfeelerAnim_Resurface       = 1,
    TwinfeelerAnim_Burrow          = 2,
    TwinfeelerAnim_DeathStartAlt0  = 3,  /** @unused */
    TwinfeelerAnim_DeathStartAlt1  = 4,  /** @unused */
    TwinfeelerAnimDeathThroes      = 5,
    TwinfeelerAnim_WalkForward     = 6,
    TwinfeelerAnim_DeathStart      = 7,
    TwinfeelerAnim_InchForward     = 8,
    TwinfeelerAnim_9               = 9,  /** @unused Stun? */
    TwinfeelerAnim_10              = 10, /** @unused Recoil. Stand to idle. */
    TwinfeelerAnim_StunToIdle      = 11,
    TwinfeelerAnim_StandAcidAttack = 12,
    TwinfeelerAnim_StandRecoil     = 13,
    TwinfeelerAnim_StandRecoilAlt  = 14, /** @unused */
    TwinfeelerAnim_StandStunStart  = 15, /** @unused */
    TwinfeelerAnim_StunStart       = 16,
    TwinfeelerAnim_IdleToStand     = 17,
    TwinfeelerAnim_Idle            = 18,
    TwinfeelerAnim_DeathEndAlt     = 19,
    TwinfeelerAnim_StunIdleEnd     = 20,
    TwinfeelerAnim_StandStunEnd    = 21, /** @unused */
    TwinfeelerAnim_StunEnd         = 22,
    TwinfeelerAnim_23              = 23
} e_TwinfeelerAnim;

/** @brief Twinfeeler character model bone indices. */
typedef enum _TwinfeelerBone
{
    TwinfeelerBone_6    = 6,
    TwinfeelerBone_Head = 14
} e_TwinfeelerBone;

extern u32 D_800DF554;
extern u32 D_800DF558;
extern u32 D_800DF55C;
extern u32 D_800DF560;
extern u32 D_800DF564;
extern u32 D_800DF568;
extern u32 D_800DF56C;
extern u32 D_800DF570;

extern s32 D_800DAA44;
extern s32 D_800DAA48;
extern s32 D_800DAA4C;
extern s32 D_800DAA50;
extern s32 D_800DAA54;
extern u16 D_800DB980[];

extern q19_12 D_800E05A8; // Angle?
extern q3_12  D_800E05AC; // Angle?
extern u16    D_800E05AE;

extern u8 D_800E05E0;
extern u8 D_800E05E1;
extern u8 D_800E05E2;

extern s_800E0698 D_800E0698;

extern SVECTOR3 D_800DAE58;

// Q19.12
extern VECTOR3 D_800E08F0;

extern s32 D_800DAA58[];

extern s_SfxVolume D_800DB1F8[6];

extern SVECTOR3 D_800DAE78;

extern u8 D_800DAE80[];

extern s16 D_800DB1D8[];

extern q3_12 D_800DB1E0[]; // Twinfeeler Y positions.

extern q3_12 D_800DB1E8[]; // Twinfeeler X rotations.

extern q3_12 D_800DB1F0[]; // Twinfeeler Y rotations.

extern u8 D_800DB190[];

extern u8 D_800DB1A8[];

extern u8 D_800DB220[][3];

extern SVECTOR D_800CA788;

// Pointers to bone node indices?
extern s8 D_800DB238[][6];

// Related to anim keyframes.
extern s32 D_800DB27C[];

extern s32 D_800DB2FC[];

extern u8 D_800DB3A0[];

extern s_CharaCollisionRecord D_800E04A0;

void func_800D078C(void);

void func_800D0840(void);

void func_800D0C50(SVECTOR* rot, MATRIX* mat);

// TODO: Could be `SVECTOR3`, need to check callers.
void func_800D0CA0(q19_12 rotY, SVECTOR* pos);

// TODO: Could be `SVECTOR3`, need to check callers.
void func_800D0D6C(MATRIX* worldMat, SVECTOR* pos, q19_12 rotY);

// TODO: Could be `SVECTOR3`/VECTOR3`, need to check callers.
void func_800D0DE4(SVECTOR* out, VECTOR* in, q19_12 headingAngle, q19_12 dist);

void func_800D0EC0(u8* buf, s32 w, s32 h);

s32 func_800D0F40(s32 arg0, s32 arg1, s32 arg2);

void func_800D0FD4(s32* ord, void* arg1, u8* arg2, MATRIX* arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 abr);

void func_800D13B4(u8* arg0, s32 arg1, s32 arg2, s32 arg3);

// TODO: Could be `SVECTOR3`, need to check callers.
void func_800D1478(SVECTOR* arg0, s32 arg1, q19_12 headingAngle, s32 mode, SVECTOR* arg4);

void func_800D1604(GsOT_TAG* ot, int arg1);

void func_800D17FC(void);

void func_800D185C(s32 arg0, s32 idx);

s_800DF580* func_800D1900(VECTOR* arg0, s32 arg1);

void func_800D19AC(VECTOR3* pos);

void func_800D19CC(const VECTOR3* pos);

bool func_800D19F0(s_800DF580* arg0);

void func_800D1AFC(void);

void func_800D1C48(void);

void func_800D1D3C(GsOT_TAG* tag, SVECTOR3* arg1, MATRIX* worldMat, s32 arg3);

void func_800D1FF4(GsOT_TAG* tag);

void func_800D2120(void);

void func_800D2150(VECTOR* arg0, s32 arg1);

void func_800D21AC(GsOT_TAG* ot, MATRIX* worldMat, s32 arg2, s32 arg3);

void func_800D2684(VECTOR* pos, q19_12 headingAngle);

void func_800D26FC(VECTOR* pos, q19_12 headingAngle);

void func_800D2790(GsOT_TAG* ot, MATRIX* worldMat, s32 arg2, s32 arg3);

void func_800D2CC8(void);

void func_800D2CEC(void);

void func_800D2D28(GsOT_TAG* arg0);

void func_800D2ED0(GsOT_TAG* arg0);

// TODO: Does game ever use this texture afterward?
// After this `Twinfeeler_Init` checks `Fs_QueueChunksLoad()` before proceeding.
// Could it be some preload/warm-up before the actual enemy file loads? If so, this func should be renamed.
void Twinfeeler_TextureLoad(void);

/** @brief Translates a position in the direction of a given Y-axis heading angle along the XZ plane.
 *
 * @param newPos Output translated position (Q19.12).
 * @param pos Start position (Q19.12).
 * @param headingAngle Y-axis heading angle.
 * @param dist Translation distance.
 */
void Twinfeeler_Translate(VECTOR3* newPos, const VECTOR3* pos, q19_12 headingAngle, q19_12 dist);

void func_800D3114(void);

void func_800D326C(void);

void func_800D33D0(void);

void func_800D3428(void);

s_800E0930* func_800D344C(s_SubCharacter* twinfeeler, void (*funcptr)());

void func_800D3504(s_SubCharacter* twinfeeler);

void func_800D3528(s_SubCharacter* twinfeeler);

void func_800D354C(VECTOR3* arg0);

void func_800D35DC(SVECTOR* rot);

void func_800D3694(s_SubCharacter* twinfeeler, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

void func_800D3AE0(s_SubCharacter* chara, s32 soundIdx);

u32 func_800D3B1C(void);

void func_800D3B44(bool disableDamage);

void func_800D3B68(s_SubCharacter* twinfeeler);

void func_800D3B74(s_SubCharacter* twinfeeler);

/** @brief Teleports a Twinfeeler above floor and clears the `TwinfeelerFlag_Underground` flag.
 *
 * @param twinfeeler Twinfeeler character to update.
 */
void Twinfeeler_FloorSet(s_SubCharacter* twinfeeler);

/** @brief Teleports a Twinfeeler underground and sets the `TwinfeelerFlag_Underground` flag.
 *
 * @param twinfeeler Twinfeeler character to update.
 */
void Twinfeeler_UndergroundSet(s_SubCharacter* twinfeeler);

bool Twinfeeler_Init(s_SubCharacter* twinfeeler);

void func_800D3E18(s_SubCharacter* twinfeeler);

void func_800D3E58(s_SubCharacter* twinfeeler);

void func_800D3FB0(s_SubCharacter* twinfeeler);

void func_800D3FD8(s_SubCharacter* twinfeeler);

void func_800D4000(s_SubCharacter* twinfeeler);

void Twinfeeler_ControlIdle(s_SubCharacter* twinfeeler);

void Twinfeeler_Control_12(s_SubCharacter* twinfeeler);

void Twinfeeler_Control_13(s_SubCharacter* twinfeeler);

void func_800D43AC(s_SubCharacter* twinfeeler, s32 arg1);

q19_12 func_800D4488(VECTOR3* arg0, s32 arg1);

q19_12 func_800D4558(q19_12 headingAngle, q19_12 fromX, q19_12 fromZ);

void Twinfeeler_Control_5(s_SubCharacter* twinfeeler);

/** @brief Throws an acid projectile attack.
 *
 * @param from Start position (Q19.12).
 * @param to End position (Q19.12).
 */
void Twinfeeler_AcidThrow(VECTOR3* from, VECTOR3* to);

void Twinfeeler_Control_6(s_SubCharacter* twinfeeler, GsCOORDINATE2* boneCoords);

bool func_800D48CC(s32 arg0, s32 arg1);

bool func_800D4924(VECTOR3* vec, q19_12 angle); // Assumed return type.

void Twinfeeler_Control_10(s_SubCharacter* twinfeeler);

void Twinfeeler_ControlStand(s_SubCharacter* twinfeeler);

void Twinfeeler_ControlAttack(s_SubCharacter* twinfeeler, GsCOORDINATE2* boneCoords);

void Twinfeeler_Control_4(s_SubCharacter* twinfeeler);

bool func_800D4C0C(u32 row, s32 col);

u16 func_800D4C5C(u8* arg0, s32 arg1, s32 arg2, s32 arg3);

s32 func_800D4D80(s32 arg0, s32 arg1);

u32 func_800D4DD8(void);

u16 func_800D4E00(s32 arg0, s32 arg1, s32 arg2);

s32 func_800D4E78(s32 arg0, s32 arg1);

void Twinfeeler_Control_3(s_SubCharacter* twinfeeler);

void Twinfeeler_Control_2(s_SubCharacter* twinfeeler);

// Handles damage response?
void Twinfeeler_Control_9(s_SubCharacter* twinfeeler);

void Twinfeeler_ControlUpdate(s_SubCharacter* twinfeeler, GsCOORDINATE2* boneCoords);

void func_800D54B4(s_SubCharacter* twinfeeler, GsCOORDINATE2* boneCoords);

void func_800D55C8(s_SubCharacter* twinfeeler);

void func_800D5888(s_SubCharacter* twinfeeler, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

void func_800D5904(s_SubCharacter* twinfeeler, GsCOORDINATE2* boneCoords);

void func_800D59C0(s_BoneNode* boneNodes);

void func_800D59EC(s_SubCharacter* twinfeeler, GsCOORDINATE2* boneCoords);

void func_800D5B6C(s_SubCharacter* twinfeeler, GsCOORDINATE2* boneCoords);

void func_800D5BC8(s_SubCharacter* twinfeeler, GsCOORDINATE2* boneCoords);

bool func_800D5BF8(s32 bitIdx, s32 arg1, s32 arg2, u16* arg3);

void func_800D5C3C(s_SubCharacter* twinfeeler, GsCOORDINATE2* boneCoords);

void func_800D5DF4(s_SubCharacter* twinfeeler, GsCOORDINATE2* boneCoords);

void func_800D5E30(s_SubCharacter* twinfeeler, GsCOORDINATE2* boneCoords);

void Twinfeeler_Update(s_SubCharacter* twinfeeler, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

#endif
