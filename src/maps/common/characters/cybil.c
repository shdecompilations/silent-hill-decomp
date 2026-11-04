#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player/player.h"
#include "main/rng.h"
#include "maps/common/shared.h"
#include "maps/common/characters/cybil.h"

/** AI code for `Chara_Cybil`
 *
 * Included in:
 *  MAP0_S01
 *  MAP4_S01
 *  MAP6_S01
 *  MAP7_S03
 */

#define cybilProps cybil->properties.npc

/** Addresses
 * MAP0_S01: 0x800D8814
 * MAP4_S01: 0x800CFDF0
 * MAP6_S01: 0x800CE4FC
 * MAP7_S03: 0x800D1098
 */
void Cybil_Update(s_SubCharacter* cybil, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords)
{
    if (cybil->model.controlState == CybilControl_None)
    {
        Cybil_Init(cybil);
    }

    Cybil_AnimStateUpdate(cybil, boneCoords);
    Cybil_MovementUpdate(cybil, boneCoords);
    Cybil_AnimUpdate(cybil, anmHdr, boneCoords);
}

/** Addresses
 * MAP0_S01: 0x800D8888
 * MAP4_S01: 0x800CFE64
 * MAP6_S01: 0x800CE570
 * MAP7_S03: 0x800D110C
 */
void Cybil_AnimUpdate(s_SubCharacter* cybil, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords)
{
    Chara_AnimUpdate(cybil, anmHdr, boneCoords, CYBIL_ANIM_INFOS);
}

/** Addresses
 * MAP0_S01: 0x800D88D0
 * MAP4_S01: 0x800CFEAC
 * MAP6_S01: 0x800CE5B8
 * MAP7_S03: 0x800D1154
 */
void Cybil_MovementUpdate(s_SubCharacter* cybil, GsCOORDINATE2* boneCoords)
{
    VECTOR3 unused;
    VECTOR3 offset;
    q19_12  moveSpeed;
    q3_12   headingAngle;
    q19_12  moveDist;
    s32     scaleRestoreShift;
    u32     scaleReduceShift;

    unused       = cybil->position;
    moveSpeed    = cybil->moveSpeed;
    headingAngle = cybil->headingAngle;
    moveDist      = Q12_MULT_PRECISE(moveSpeed, g_DeltaTime);

    scaleRestoreShift = OVERFLOW_GUARD(moveDist);
    scaleReduceShift  = scaleRestoreShift >> 1;

    offset.vx = (u32)Q12_MULT_PRECISE(moveDist >> scaleReduceShift, Math_Sin(headingAngle) >> scaleReduceShift) << scaleRestoreShift;
    offset.vz = (u32)Q12_MULT_PRECISE(moveDist >> scaleReduceShift, Math_Cos(headingAngle) >> scaleReduceShift) << scaleRestoreShift;
    offset.vy = Q12_MULT_PRECISE(cybil->fallSpeed, g_DeltaTime);

    cybil->position.vx += offset.vx;
    cybil->position.vy  = Q12(0.0f);
    cybil->position.vz += offset.vz;

    boneCoords[CybilBone_Root].coord.t[0] = Q12_TO_Q8(cybil->position.vx);
    boneCoords[CybilBone_Root].coord.t[1] = Q12_TO_Q8(cybil->position.vy);
    boneCoords[CybilBone_Root].coord.t[2] = Q12_TO_Q8(cybil->position.vz);
}

/** Addresses
 * MAP0_S01:
 * MAP4_S01:
 * MAP6_S01:
 * MAP7_S03:
 */
void Cybil_AnimStateUpdate(s_SubCharacter* cybil, GsCOORDINATE2* boneCoords)
{
    s_CollisionSurface surface;
    e_SfxId            sfx;
    s8                 pitch0;
    s8                 pitch1;

    switch (cybilProps.controlState)
    {
        case CybilControl_None:
            if (cybilProps.moveSpeed)
            {
                cybilProps.moveSpeed -= TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f)) * 2;
                if (cybilProps.moveSpeed < Q12(0.0f))
                {
                    cybilProps.moveSpeed = Q12(0.0f);
                }
            }

            Model_AnimStatusSet(&cybil->model, CybilAnim_1, false);
            Chara_AnimStateReset(cybil);
            break;

        case CybilControl_1:
            if (cybilProps.moveSpeed > Q12(1.25f))
            {
                cybilProps.moveSpeed -= TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.5f));
                if (cybilProps.moveSpeed < Q12(1.25f))
                {
                    cybilProps.moveSpeed = Q12(1.25f);
                }
            }
            else if (cybilProps.moveSpeed < Q12(1.25f))
            {
                cybilProps.moveSpeed += TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f));
                cybilProps.moveSpeed  = CLAMP(cybilProps.moveSpeed, 0, Q12(1.25f));
            }

#if defined(MAP0_S01)
            Model_AnimStatusKeyframeSet(cybil->model, CybilAnim_3, true, CYBIL_ANIM_INFOS, 0);
#else
            Model_AnimStatusSet(&cybil->model, CybilAnim_3, false);
#endif
            Chara_AnimStateReset(cybil);
            break;

        case CybilControl_2:
            if (cybilProps.moveSpeed > Q12(4.0f))
            {
                cybilProps.moveSpeed -= TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f));
                if (cybilProps.moveSpeed < Q12(4.0f))
                {
                    cybilProps.moveSpeed = Q12(4.0f);
                }
            }
            else if (cybilProps.moveSpeed < Q12(4.0f))
            {
                cybilProps.moveSpeed += TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.75f));
                cybilProps.moveSpeed = CLAMP(cybilProps.moveSpeed, Q12(0.0f), Q12(4.0f));
            }

            // TODO: This uses `ANIM_STATUS(21, false)`, but then fetches `ANIM_STATUS(22, true)` keyframe.
            // Added `animInfosOffset` but maybe there's better way.
            Model_AnimStatusKeyframeSet(cybil->model, CybilAnim_21, false, CYBIL_ANIM_INFOS, 1);
            Chara_AnimStateReset(cybil);
            break;

        case CybilControl_3:
            if (cybilProps.moveSpeed)
            {
                cybilProps.moveSpeed -= TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f)) * 2;
                if (cybilProps.moveSpeed < Q12(0.0f))
                {
                    cybilProps.moveSpeed = Q12(0.0f);
                }
            }

            sharedData_800E2378_0_s01 = g_DeltaTime * 7;

            Model_AnimStatusSet(&cybil->model, CybilAnim_4, false);
            Chara_AnimStateReset(cybil);
            break;

        case CybilControl_4:
            if (cybilProps.moveSpeed)
            {
                cybilProps.moveSpeed -= TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f)) * 2;
                if (cybilProps.moveSpeed < Q12(0.0f))
                {
                    cybilProps.moveSpeed = Q12(0.0f);
                }
            }

            sharedData_800E2378_0_s01 = g_DeltaTime * -7;

            Model_AnimStatusSet(&cybil->model, CybilAnim_5, false);
            Chara_AnimStateReset(cybil);
            break;

        case CybilControl_5:
            if (cybilProps.moveSpeed)
            {
                cybilProps.moveSpeed -= TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f)) * 2;
                if (cybilProps.moveSpeed < Q12(0.0f))
                {
                    cybilProps.moveSpeed = Q12(0.0f);
                }
            }

            Model_AnimStatusKeyframeSet(cybil->model, CybilAnim_1, true, CYBIL_ANIM_INFOS, 0);
            Chara_AnimStateReset(cybil);
            break;

        case CybilControl_12:
            if (cybilProps.moveSpeed)
            {
                cybilProps.moveSpeed -= TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f)) * 2;
                if (cybilProps.moveSpeed < Q12(0.0f))
                {
                    cybilProps.moveSpeed = Q12(0.0f);
                }
            }

            Model_AnimStatusSet(&cybil->model, CybilAnim_1, false);
            Chara_AnimStateReset(cybil);
            break;

        case CybilControl_6:
            Model_AnimStatusKeyframeSet(cybil->model, CybilAnim_7, true, CYBIL_ANIM_INFOS, 0);
            Chara_AnimStateReset(cybil);

            if (cybil->model.anim.keyframeIdx < 91)
            {
                WorldGfx_CharaMeshSwap(Chara_Cybil, MESH_SWAP_STATUS(CybilSwappableMesh_1, CybilVariantMesh_1));
            }
            else if (cybil->model.anim.keyframeIdx < 105)
            {
                WorldGfx_CharaMeshSwap(Chara_Cybil, MESH_SWAP_STATUS(CybilSwappableMesh_1, CybilVariantMesh_2));
            }
            else
            {
                WorldGfx_CharaMeshSwap(Chara_Cybil, MESH_SWAP_STATUS(CybilSwappableMesh_2, CybilVariantMesh_1));
            }
            break;

        case CybilControl_7:
            Model_AnimStatusKeyframeSet(cybil->model, CybilAnim_8, true, CYBIL_ANIM_INFOS, 0);
            Chara_AnimStateReset(cybil);
            break;

        case CybilControl_8:
            Model_AnimStatusKeyframeSet(cybil->model, CybilAnim_9, true, CYBIL_ANIM_INFOS, 0);
            Chara_AnimStateReset(cybil);
            break;

        case CybilControl_9:
            Model_AnimStatusKeyframeSet(cybil->model, CybilAnim_10, true, CYBIL_ANIM_INFOS, 0);
            Chara_AnimStateReset(cybil);
            WorldGfx_CharaMeshSwap(Chara_Cybil, MESH_SWAP_STATUS(CybilSwappableMesh_1, CybilVariantMesh_1));
            break;

        case CybilControl_10:
            Model_AnimStatusSet(&cybil->model, CybilAnim_11, true);
            Chara_AnimStateReset(cybil);
            break;

        case CybilControl_11:
            Model_AnimStatusKeyframeSet(cybil->model, CybilAnim_12, true, CYBIL_ANIM_INFOS, 0);
            Chara_AnimStateReset(cybil);
            break;

        case CybilControl_13:
            Model_AnimStatusSet(&cybil->model, CybilAnim_13, false);
            Chara_AnimStateReset(cybil);
            break;

        case CybilControl_14:
            Model_AnimStatusSet(&cybil->model, CybilAnim_14, false);
            Chara_AnimStateReset(cybil);
            break;

        case CybilControl_15:
            Model_AnimStatusSet(&cybil->model, CybilAnim_15, false);
            Chara_AnimStateReset(cybil);
            break;

        case CybilControl_16:
            Model_AnimStatusSet(&cybil->model, CybilAnim_16, false);
            Chara_AnimStateReset(cybil);
            break;

        case CybilControl_17:
            Model_AnimStatusSet(&cybil->model, CybilAnim_17, false);
            Chara_AnimStateReset(cybil);
            break;

        case CybilControl_18:
            Model_AnimStatusSet(&cybil->model, CybilAnim_18, false);
            Chara_AnimStateReset(cybil);
            break;

        case CybilControl_19:
            Model_AnimStatusSet(&cybil->model, CybilAnim_19, false);
            Chara_AnimStateReset(cybil);
            break;

        case CybilControl_20:
            Model_AnimStatusSet(&cybil->model, CybilAnim_20, false);
            Chara_AnimStateReset(cybil);
            break;

        case CybilControl_21:
            Model_AnimStatusSet(&cybil->model, CybilAnim_26, false);
            Model_AnimStatusKeyframeSet(cybil->model, CybilAnim_26, true, CYBIL_ANIM_INFOS, 0);
            Chara_AnimStateReset(cybil);
            WorldGfx_CharaMeshSwap(Chara_EndingCybil, MESH_SWAP_STATUS(1, 2));

            if (cybil->model.anim.keyframeIdx == 295)
            {
                cybilProps.controlState = 30;
                cybil->model.stateStep = 0;
            }
            break;

        case CybilControl_22:
            Model_AnimStatusSet(&cybil->model, CybilAnim_27, false);
            Model_AnimStatusKeyframeSet(cybil->model, CybilAnim_27, true, CYBIL_ANIM_INFOS, 0);
            Chara_AnimStateReset(cybil);
            WorldGfx_CharaMeshSwap(Chara_EndingCybil, MESH_SWAP_STATUS(1, 2));
            break;

        case CybilControl_23:
            Model_AnimStatusSet(&cybil->model, CybilAnim_28, false);
            Model_AnimStatusKeyframeSet(cybil->model, CybilAnim_28, true, CYBIL_ANIM_INFOS, 0);
            Chara_AnimStateReset(cybil);
            WorldGfx_CharaMeshSwap(Chara_EndingCybil, MESH_SWAP_STATUS(1, 2));

            if (cybilProps.controlState != 23)
            {
                cybilProps.flags &= ~(1 << 13);
            }
            break;

        case CybilControl_24:
            Model_AnimStatusSet(&cybil->model, CybilAnim_29, false);
            Model_AnimStatusKeyframeSet(cybil->model, CybilAnim_29, true, CYBIL_ANIM_INFOS, 0);
            Chara_AnimStateReset(cybil);
            WorldGfx_CharaMeshSwap(Chara_EndingCybil, MESH_SWAP_STATUS(1, 2));
            break;

        case CybilControl_25:
            Model_AnimStatusKeyframeSet(cybil->model, CybilAnim_22, true, CYBIL_ANIM_INFOS, 0);
            Chara_AnimStateReset(cybil);
            break;

        case CybilControl_26:
            Model_AnimStatusKeyframeSet(cybil->model, CybilAnim_23, true, CYBIL_ANIM_INFOS, 0);
            Chara_AnimStateReset(cybil);
            break;

        case CybilControl_27:
            Model_AnimStatusSet(&cybil->model, CybilAnim_24, false);
            Chara_AnimStateReset(cybil);
            break;

        case CybilControl_29:
            Model_AnimStatusSet(&cybil->model, CybilAnim_30, false);
            Chara_AnimStateReset(cybil);
            break;

        case CybilControl_28:
            Model_AnimStatusKeyframeSet(cybil->model, CybilAnim_25, true, CYBIL_ANIM_INFOS, 0);
            Chara_AnimStateReset(cybil);
            break;

        case CybilControl_30:
            Model_AnimStatusSet(&cybil->model, CybilAnim_31, false);
            Chara_AnimStateReset(cybil);
            WorldGfx_CharaMeshSwap(Chara_EndingCybil, MESH_SWAP_STATUS(1, 2));
            break;
    }

    Collision_SurfaceGet(&surface, cybil->position.vx, cybil->position.vz);
    Player_FootstepSfxGet(surface.floorType, &sfx, &pitch0, &pitch1);

#if defined(MAP6_S01)
    sfx = Sfx_Unk1607;
#endif

    switch (cybilProps.controlState)
    {
        case CybilControl_1:
            Npc_FootstepSoundPlay(ANIM_STATUS(CybilAnim_3, true), cybil, 32, 45, sfx, pitch0);
            break;

        case CybilControl_2:
            Npc_FootstepSoundPlay(ANIM_STATUS(CybilAnim_21, true), cybil, 18, 7, sfx, pitch1);
            break;

        case CybilControl_3:
            Npc_FootstepSoundPlay(ANIM_STATUS(CybilAnim_4, true), cybil, 61, 57, sfx, pitch0);
            break;

        case CybilControl_4:
            Npc_FootstepSoundPlay(ANIM_STATUS(CybilAnim_5, true), cybil, 70, 74, sfx, pitch1);
            break;

        case CybilControl_8:
            Npc_SfxPlay(cybil->model.anim.status, cybil, 128, Sfx_Unk1368);
            break;

        case CybilControl_6:
            Npc_FootstepSoundPlay(ANIM_STATUS(CybilAnim_7, true), cybil, 83, 86, sfx, pitch0);
            Npc_SfxPlay(ANIM_STATUS(CybilAnim_7, true), cybil, 92, Sfx_Unk1369);
            break;

        case CybilControl_13:
            if (cybil->model.anim.keyframeIdx <= 185)
            {
                Npc_FootstepSoundPlay(ANIM_STATUS(CybilAnim_13, true), cybil, 185, 189, sfx, pitch0);
            }
            else if (cybil->model.anim.keyframeIdx <= 192)
            {
                Npc_FootstepSoundPlay(ANIM_STATUS(CybilAnim_13, true), cybil, 192, 189, sfx, pitch0);
            }
            else
            {
                Npc_FootstepSoundPlay(ANIM_STATUS(CybilAnim_13, true), cybil, 227, 222, sfx, pitch0);
            }
            break;

        case CybilControl_15:
            Npc_FootstepSoundPlay(ANIM_STATUS(CybilAnim_15, true), cybil, 260, 260, sfx, pitch0);
            break;

        case CybilControl_14:
            Npc_FootstepSoundPlay(ANIM_STATUS(CybilAnim_14, true), cybil, 239, 239, sfx, pitch0);
            break;

        case CybilControl_19:
            Npc_FootstepSoundPlay(ANIM_STATUS(CybilAnim_19, true), cybil, 362, 357, sfx, pitch0);
            break;

        case CybilControl_20:
            if (cybil->model.anim.keyframeIdx <= 373)
            {
                Npc_FootstepSoundPlay(ANIM_STATUS(CybilAnim_20, true), cybil, 373, 380, sfx, pitch0);
            }
            else if (cybil->model.anim.keyframeIdx <= 391)
            {
                Npc_FootstepSoundPlay(ANIM_STATUS(CybilAnim_20, true), cybil, 391, 380, sfx, pitch0);
            }
            else
            {
                Npc_FootstepSoundPlay(ANIM_STATUS(CybilAnim_20, true), cybil, 403, 398, sfx, pitch0);
            }
            break;

        case CybilControl_10:
            Npc_SfxPlay(cybil->model.anim.status, cybil, 167, Sfx_Unk1370);
            break;

        case CybilControl_25:
            if (cybil->model.anim.keyframeIdx <= 63)
            {
                if (cybil->model.anim.keyframeIdx <= 30)
                {
                    Npc_FootstepSoundPlay(ANIM_STATUS(CybilAnim_22, true), cybil, 30, 39, sfx, pitch0);
                }
                else if (cybil->model.anim.keyframeIdx <= 46)
                {
                    Npc_FootstepSoundPlay(ANIM_STATUS(CybilAnim_22, true), cybil, 46, 39, sfx, pitch0);
                }
                else
                {
                    Npc_FootstepSoundPlay(ANIM_STATUS(CybilAnim_22, true), cybil, 63, 56, sfx, pitch0);
                }
            }
            else
            {
                if (cybil->model.anim.keyframeIdx <= 81)
                {
                    Npc_FootstepSoundPlay(ANIM_STATUS(CybilAnim_22, true), cybil, 81, 74, sfx, pitch0);
                }
                else
                {
                    Npc_FootstepSoundPlay(ANIM_STATUS(CybilAnim_22, true), cybil, 97, 91, sfx, pitch0);
                }
            }
            break;

        case CybilControl_28:
            Npc_FootstepSoundPlay(ANIM_STATUS(CybilAnim_25, true), cybil, 201, 198, sfx, pitch0);

            if (Npc_SfxPlay(cybil->model.anim.status, cybil, 228, Sfx_Unk1699))
            {
                func_800892A4(6);
            }
            break;

        case CybilControl_22:
            if (sharedData_800E237C_0_s01 == 1)
            {
                Sfx_WithFlagsPlay(Sfx_Unk1673, &g_SysWork.npcs[1].position, Q8(0.75f), SfxFlag_None);
                sharedData_800E237C_0_s01 = 2;
            }

            if (cybil->model.anim.keyframeIdx == 299)
            {
                if (sharedData_800E237C_0_s01 == 0)
                {
                    func_8006342C(EquippedWeaponId_Unk63, Q12_ANGLE(90.0f), cybil->rotation.vy, &g_SysWork.npcBoneCoordBuffer[0]);
                    Sfx_WithFlagsPlay(Sfx_Unk1622, &cybil->position, Q8(0.75f), SfxFlag_None);
                    sharedData_800E237C_0_s01 = 1;
                }
            }
            break;

        case CybilControl_24:
            Npc_SfxPlay(ANIM_STATUS(CybilAnim_29, true), cybil, 327, Sfx_Unk1672);
            break;

        case CybilControl_21:
            Npc_SfxPlay(ANIM_STATUS(CybilAnim_26, true), cybil, 282, Sfx_Unk1539);
            break;
    }

    cybil->rotation.vy  = Q12_ANGLE_ABS(cybil->rotation.vy + (sharedData_800E2378_0_s01 >> 4));
    cybil->headingAngle = cybil->rotation.vy;
    cybil->moveSpeed    = cybilProps.moveSpeed;
    cybil->fallSpeed   += g_GravitySpeed;

    boneCoords->flg = false;
    Math_RotMatrixZxyNegGte(&cybil->rotation, &boneCoords[CybilBone_Root].coord);
}

/** Addresses
 * MAP0_S01: 0x800D9AD0
 * MAP4_S01: 0x800D1084
 * MAP6_S01: 0x800CF794
 * MAP7_S03: 0x800D232C
 */
void Cybil_Init(s_SubCharacter* cybil)
{
    Chara_CollisionReset(cybil);
    sharedData_800E2378_0_s01 = 0;
    sharedData_800E237C_0_s01 = 0;

    WorldGfx_CharaMeshSwap(
#ifdef MAP7_S03
                           Chara_EndingCybil,
#else
                           Chara_Cybil,
#endif
                           MESH_SWAP_STATUS(CybilSwappableMesh_1, CybilVariantMesh_1));
}
