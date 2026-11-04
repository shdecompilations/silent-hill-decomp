#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "main/rng.h"
#include "maps/common/shared.h"
#include "maps/common/characters/alessa.h"

/** AI code for `Chara_Alessa`
 *
 * Included in:
 *  MAP3_S02
 *  MAP6_S02
 *  MAP6_S04
 *  MAP7_S03
 */

#define alessaProps alessa->properties.npc

/** Addresses
 * MAP3_S02: 0x800CEC88
 * MAP6_S02: 0x800CD734
 * MAP6_S04: 0x800DC2B4
 * MAP7_S03: 0x800D2F08
 */
void Alessa_Update(s_SubCharacter* alessa, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords)
{
    if (alessa->model.controlState == AlessaControl_None)
    {
        Alessa_Init(alessa);
    }

    Alessa_AnimStateUpdate(alessa, boneCoords);
    Alessa_MovementUpdate(alessa, boneCoords);
    Alessa_AnimUpdate(alessa, anmHdr, boneCoords);
}

/** Addresses
 * MAP3_S02: 0x800CECFC
 * MAP6_S02: 0x800CD7A8
 * MAP6_S04: 0x800DC328
 * MAP7_S03: 0x800D2F7C
 */
void Alessa_AnimUpdate(s_SubCharacter* alessa, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords)
{
    Chara_AnimUpdate(alessa, anmHdr, boneCoords, ALESSA_ANIM_INFOS);
}

/** Addresses
 * MAP3_S02: 0x800CED44
 * MAP6_S02: 0x800CD7F0
 * MAP6_S04: 0x800DC370
 * MAP7_S03: 0x800D2FC4
 */
void Alessa_MovementUpdate(s_SubCharacter* alessa, GsCOORDINATE2* boneCoords)
{
    VECTOR3 unused;
    VECTOR3 offset;
    q19_12  moveSpeed;
    q3_12   headingAngle;
    q19_12  moveDist;
    s32     scaleRestoreShift;
    u32     scaleReduceShift;

    unused       = alessa->position;
    moveSpeed    = alessa->moveSpeed;
    headingAngle = alessa->headingAngle;
    moveDist     = Q12_MULT_PRECISE(moveSpeed, g_DeltaTime);

    scaleRestoreShift = OVERFLOW_GUARD(moveDist);
    scaleReduceShift  = scaleRestoreShift >> 1;

    offset.vx = (u32)Q12_MULT_PRECISE(moveDist >> scaleReduceShift, Math_Sin(headingAngle) >> scaleReduceShift) << scaleRestoreShift;
    offset.vz = (u32)Q12_MULT_PRECISE(moveDist >> scaleReduceShift, Math_Cos(headingAngle) >> scaleReduceShift) << scaleRestoreShift;
    offset.vy = Q12_MULT_PRECISE(alessa->fallSpeed, g_DeltaTime);

    alessa->position.vx += offset.vx;
    alessa->position.vy =  Q12(0.0f);
    alessa->position.vz += offset.vz;

    boneCoords[AlessaBone_Root].coord.t[0] = Q12_TO_Q8(alessa->position.vx);
    boneCoords[AlessaBone_Root].coord.t[1] = Q12_TO_Q8(alessa->position.vy);
    boneCoords[AlessaBone_Root].coord.t[2] = Q12_TO_Q8(alessa->position.vz);
}

/** Addresses
 * MAP3_S02: 0x800CEEDC
 * MAP6_S02: 0x800CD988
 * MAP6_S04: 0x800DC508
 * MAP7_S03: 0x800D315C
 */
void Alessa_AnimStateUpdate(s_SubCharacter* alessa, GsCOORDINATE2* boneCoords)
{
    s_CollisionSurface surface;
    e_SfxId            sfxId;
    s8                 pitch0;
    s8                 pitch1;

    switch (alessaProps.controlState)
    {
        case AlessaControl_None:
            break;

        case AlessaControl_2:
            if (alessaProps.moveSpeed > Q12(1.25f))
            {
                alessaProps.moveSpeed -= TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.5f));
                if (alessaProps.moveSpeed < Q12(1.25f))
                {
                    alessaProps.moveSpeed = Q12(1.25f);
                }
            }
            else if (alessaProps.moveSpeed < Q12(1.25f))
            {
                alessaProps.moveSpeed += TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f));
                alessaProps.moveSpeed  = CLAMP(alessaProps.moveSpeed, Q12(0.0f), Q12(1.25f));
            }

            Model_AnimStatusSet(&alessa->model, AlessaAnim_WalkForward, false);
            Chara_AnimStateReset(alessa);
            break;

        case AlessaControl_1:
            if (alessaProps.moveSpeed != Q12(0.0f))
            {
                alessaProps.moveSpeed -= TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f)) * 2;
                if (alessaProps.moveSpeed < Q12(0.0f))
                {
                    alessaProps.moveSpeed = Q12(0.0f);
                }
            }

            Model_AnimStatusKeyframeSet(alessa->model, AlessaAnim_StandIdle, true, ALESSA_ANIM_INFOS, 0);
            Chara_AnimStateReset(alessa);
            break;

        case AlessaControl_4:
            Model_AnimStatusSet(&alessa->model, AlessaAnim_ForcePush, false);
            Chara_AnimStateReset(alessa);
            break;

        case AlessaControl_3:
            Model_AnimStatusSet(&alessa->model, AlessaAnim_WalkForwardTurnStumble, false);
            Chara_AnimStateReset(alessa);
            break;

        case AlessaControl_5:
            Model_AnimStatusSet(&alessa->model, AlessaAnim_StumbleForwardCrumple, false);
            Chara_AnimStateReset(alessa);
            break;

        case 6:
            Model_AnimStatusSet(&alessa->model, AlessaAnim_CrumpleLookUp, false);
            Chara_AnimStateReset(alessa);
            break;

        case AlessaControl_7:
            Model_AnimStatusSet(&alessa->model, AlessaAnim_CrumpleShakeHead, false);
            Chara_AnimStateReset(alessa);
            break;

        case AlessaControl_8:
            Model_AnimStatusSet(&alessa->model, AlessaAnim_StandLookRight, false);
            Chara_AnimStateReset(alessa);
            break;

        case AlessaControl_9:
            Model_AnimStatusSet(&alessa->model, AlessaAnim_CrumpleIdle, false);
            Chara_AnimStateReset(alessa);
            break;

        case AlessaControl_10:
            Model_AnimStatusSet(&alessa->model, AlessaAnim_Kneel, false);
            Chara_AnimStateReset(alessa);
            break;
    }

    Collision_SurfaceGet(&surface, alessa->position.vx, alessa->position.vz);
    Player_FootstepSfxGet(surface.floorType, &sfxId, &pitch0, &pitch1);

    switch (alessaProps.controlState)
    {
        case AlessaControl_2:
            Npc_FootstepSoundPlay(ANIM_STATUS(AlessaAnim_WalkForward, true), alessa, 24, 37, sfxId, pitch0);
            break;

        case AlessaControl_3:
            if (alessa->model.anim.keyframeIdx < 90)
            {
                if (alessa->model.anim.keyframeIdx <= 60)
                {
                    Npc_FootstepSoundPlay(ANIM_STATUS(AlessaAnim_WalkForwardTurnStumble, true), alessa, 60, 71, sfxId, pitch0);
                }
                else if (alessa->model.anim.keyframeIdx <= 81)
                {
                    Npc_FootstepSoundPlay(ANIM_STATUS(AlessaAnim_WalkForwardTurnStumble, true), alessa, 81, 71, sfxId, pitch0);
                }
                else
                {
                    Npc_FootstepSoundPlay(ANIM_STATUS(AlessaAnim_WalkForwardTurnStumble, true), alessa, 89, 89, sfxId, pitch0);
                }
            }
            else
            {
                if (alessa->model.anim.keyframeIdx <= 111)
                {
                    Npc_FootstepSoundPlay(ANIM_STATUS(AlessaAnim_WalkForwardTurnStumble, true), alessa, 121, 111, sfxId, pitch1);
                }
                else
                {
                    Npc_FootstepSoundPlay(ANIM_STATUS(AlessaAnim_WalkForwardTurnStumble, true), alessa, 121, 128, sfxId, pitch1);
                }
            }
            break;

        case AlessaControl_5:
            Npc_SfxPlay(alessa->model.anim.status, alessa, 210, Sfx_Unk1638);

            if (alessa->model.anim.keyframeIdx <= 173)
            {
                Npc_FootstepSoundPlay(ANIM_STATUS(AlessaAnim_StumbleForwardCrumple, true), alessa, 182, 173, sfxId, pitch0);
            }
            else
            {
                Npc_FootstepSoundPlay(ANIM_STATUS(AlessaAnim_StumbleForwardCrumple, true), alessa, 182, 199, sfxId, pitch0);
            }
            break;

        case AlessaControl_8:
            Npc_FootstepSoundPlay(ANIM_STATUS(AlessaAnim_StandLookRight, true), alessa, 298, 298, sfxId, pitch0);
            break;
    }

    alessa->rotation.vy  = Q12_ANGLE_ABS(alessa->rotation.vy + (sharedData_800D3150_3_s02 >> 4));
    alessa->headingAngle = alessa->rotation.vy;
    alessa->moveSpeed    = alessaProps.moveSpeed;
    alessa->fallSpeed   += g_GravitySpeed;

    boneCoords->flg = false;
    Math_RotMatrixZxyNegGte(&alessa->rotation, &boneCoords[AlessaBone_Root].coord);
}

/** Addresses
 * MAP3_S02: 0x800CF3E0
 * MAP6_S02: 0x800CDE8C
 * MAP6_S04: 0x800DCA0C
 * MAP7_S03: 0x800D3660
 */
void Alessa_Init(s_SubCharacter* alessa)
{
    Chara_CollisionReset(alessa);
    sharedData_800D3150_3_s02 = 0;
}

#undef darliaProps
