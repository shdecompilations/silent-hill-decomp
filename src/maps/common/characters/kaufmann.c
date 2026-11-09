#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "maps/common/shared.h"
#include "maps/common/characters/kaufmann.h"

/** Code for `Chara_Kaufmann`.
 *
 * Included in:
 *  MAP3_S00
 *  MAP5_S02
 *  MAP5_S03
 *  MAP7_S02
 *  MAP7_S03
 */

#define kaufmannProps kaufmann->properties.npc

/** Addresses
 * MAP3_S00: 0x800CF21C
 * MAP5_S02: 0x800D2EA8
 * MAP5_S03: 0x800CF8DC
 * MAP7_S02: 0x800D7EAC
 * MAP7_S03: 0x800D3F58
 */
void Kaufmann_Update(s_SubCharacter* kaufmann, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords)
{
    if (kaufmann->model.controlState == 0)
    {
        Kaufmann_Init(kaufmann);
    }

    Kaufmann_ControlUpdate(kaufmann, boneCoords);
    Kaufmann_MovementUpdate(kaufmann, boneCoords);
    Kaufmann_AnimUpdate(kaufmann, anmHdr, boneCoords);
}

/** Addresses
 * MAP3_S00: 0x800CF290
 * MAP5_S02: 0x800D2F1C
 * MAP5_S03: 0x800CF950
 * MAP7_S02: 0x800D7F20
 * MAP7_S03: 0x800D3FCC
 */
void Kaufmann_AnimUpdate(s_SubCharacter* kaufmann, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords)
{
    Chara_AnimUpdate(kaufmann, anmHdr, boneCoords, KAUFMANN_ANIM_INFOS);
}

/** Addresses
 * MAP3_S00: 0x800CF2D8
 * MAP5_S02: 0x800D2F64
 * MAP5_S03: 0x800CF998
 * MAP7_S02: 0x800D7F68
 * MAP7_S03: 0x800D4014
 */
void Kaufmann_MovementUpdate(s_SubCharacter* kaufmann, GsCOORDINATE2* boneCoords)
{
    VECTOR3 pos; // Q19.12
    VECTOR3 offset;
    q19_12  moveSpeed;
    q3_12   headingAngle;
    q19_12  moveDist;
    s32     scaleRestoreShift;
    u32     scaleReduceShift;

    pos          = kaufmann->position;
    moveSpeed    = kaufmann->moveSpeed;
    headingAngle = kaufmann->headingAngle;
    moveDist     = Q12_MULT_PRECISE(moveSpeed, g_DeltaTime);

    scaleRestoreShift = OVERFLOW_GUARD(moveDist);
    scaleReduceShift  = scaleRestoreShift >> 1;

    offset.vx = (u32)Q12_MULT_PRECISE(moveDist >> scaleReduceShift, Math_Sin(headingAngle) >> scaleReduceShift) << scaleRestoreShift;
    offset.vz = (u32)Q12_MULT_PRECISE(moveDist >> scaleReduceShift, Math_Cos(headingAngle) >> scaleReduceShift) << scaleRestoreShift;
    offset.vy = Q12_MULT_PRECISE(kaufmann->fallSpeed, g_DeltaTime);

    kaufmann->position.vx += offset.vx;
    kaufmann->position.vy  = Q12(0.0f);
    kaufmann->position.vz += offset.vz;

    boneCoords[0].coord.t[0] = Q12_TO_Q8(kaufmann->position.vx);
    boneCoords[0].coord.t[1] = Q12_TO_Q8(kaufmann->position.vy);
    boneCoords[0].coord.t[2] = Q12_TO_Q8(kaufmann->position.vz);
}

/** Addresses
 * MAP3_S00: 0x800CF470
 * MAP5_S02: 0x800D30FC
 * MAP5_S03: 0x800CFB30
 * MAP7_S02: 0x800D8100
 * MAP7_S03: 0x800D41AC
 */
void Kaufmann_ControlUpdate(s_SubCharacter* kaufmann, GsCOORDINATE2* boneCoords)
{
    s_CollisionSurface surface;
    e_SfxId            sfx;
    s8                 pitch0;
    s8                 pitch1;

    switch (kaufmannProps.controlState)
    {
        case KaufmannControl_0:
            if (kaufmannProps.moveSpeed != Q12(0.0f))
            {
                kaufmannProps.moveSpeed -= TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f)) * 2;
                if (kaufmannProps.moveSpeed < Q12(0.0f))
                {
                    kaufmannProps.moveSpeed = Q12(0.0f);
                }
            }

            Model_AnimStatusSet(&kaufmann->model, KaufmannAnim_1, false);
            Chara_AnimStateReset(kaufmann);
            break;

        case KaufmannControl_1:
            if (kaufmannProps.moveSpeed > Q12(1.25f))
            {
                kaufmannProps.moveSpeed -= TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.5f));
                if (kaufmannProps.moveSpeed < Q12(1.25f))
                {
                    kaufmannProps.moveSpeed = Q12(1.25f);
                }
            }
            else if (kaufmannProps.moveSpeed < Q12(1.25f))
            {
                kaufmannProps.moveSpeed += TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f));
                kaufmannProps.moveSpeed  = CLAMP(kaufmannProps.moveSpeed, 0, Q12(1.25f));
            }

            Model_AnimStatusSet(&kaufmann->model, KaufmannAnim_3, false);
            Chara_AnimStateReset(kaufmann);
#if defined(MAP5_S03)
            WorldGfx_CharaMeshSwap(Chara_Kaufmann, MESH_SWAP_STATUS(KaufmannSwappableMesh_3, KaufmannVariantMesh_4));
#endif
            break;

        case KaufmannControl_5:
            Model_AnimStatusKeyframeSet(kaufmann->model, KaufmannAnim_1, true, KAUFMANN_ANIM_INFOS, 0);
            Chara_AnimStateReset(kaufmann);
            WorldGfx_CharaMeshSwap(Chara_Kaufmann, MESH_SWAP_STATUS(KaufmannSwappableMesh_None, KaufmannVariantMesh_1));
            break;

        case KaufmannControl_24:
            Model_AnimStatusKeyframeSet(kaufmann->model, KaufmannAnim_20, true, KAUFMANN_ANIM_INFOS, 0);
            Chara_AnimStateReset(kaufmann);
#if defined(MAP5_S03)
            WorldGfx_CharaMeshSwap(Chara_Kaufmann, MESH_SWAP_STATUS(KaufmannSwappableMesh_3, KaufmannVariantMesh_4));
#else
            WorldGfx_CharaMeshSwap(Chara_EndingKaufmann, MESH_SWAP_STATUS(3, 1));
#endif
            break;

        case KaufmannControl_6:
            Model_AnimStatusKeyframeSet(kaufmann->model, KaufmannAnim_4, true, KAUFMANN_ANIM_INFOS, 0);
            Chara_AnimStateReset(kaufmann);
#if defined(MAP3_S00)
            WorldGfx_CharaMeshSwap(Chara_Kaufmann, MESH_SWAP_STATUS(KaufmannSwappableMesh_1, KaufmannVariantMesh_3));
#endif
            break;

        case KaufmannControl_7:
            if (kaufmann->model.anim.keyframeIdx >= 95 && g_SysWork.npcs[0].properties.npc.field_120 == 0)
            {
                g_SysWork.npcs[0].properties.npc.field_120 = 1;
                func_8006342C(EquippedWeaponId_Unk70, Q12_ANGLE(0.0f), Q12_ANGLE(0.0f), &g_SysWork.npcBoneCoordBuffer[0]);
            }

            Model_AnimStatusSet(&kaufmann->model, KaufmannAnim_5, false);
            Chara_AnimStateReset(kaufmann);
#if defined(MAP3_S00)
            WorldGfx_CharaMeshSwap(Chara_Kaufmann, MESH_SWAP_STATUS(KaufmannSwappableMesh_1, KaufmannVariantMesh_3));
#endif
            break;

        case KaufmannControl_8:
            Model_AnimStatusSet(&kaufmann->model, KaufmannAnim_6, false);
            Chara_AnimStateReset(kaufmann);
#if defined(MAP3_S00)
            WorldGfx_CharaMeshSwap(Chara_Kaufmann, MESH_SWAP_STATUS(KaufmannSwappableMesh_1, KaufmannVariantMesh_3));
#endif
            break;

        case KaufmannControl_9:
            Model_AnimStatusSet(&kaufmann->model, KaufmannAnim_7, false);
            Chara_AnimStateReset(kaufmann);

            if (kaufmann->model.anim.keyframeIdx < 139 || !ANIM_STATUS_IS_ACTIVE(kaufmann->model.anim.status))
            {
                WorldGfx_CharaMeshSwap(Chara_Kaufmann, MESH_SWAP_STATUS(KaufmannSwappableMesh_1, KaufmannVariantMesh_1));
            }
            else
            {
                Savegame_EventFlagSet(EventFlag_198);
                WorldGfx_CharaMeshSwap(Chara_Kaufmann, MESH_SWAP_STATUS(KaufmannSwappableMesh_3, KaufmannVariantMesh_1));
            }
            break;

        case KaufmannControl_10:
            Model_AnimStatusSet(&kaufmann->model, KaufmannAnim_8, false);
            Chara_AnimStateReset(kaufmann);
            break;

        case KaufmannControl_11:
            Model_AnimStatusKeyframeSet(kaufmann->model, KaufmannAnim_9, true, KAUFMANN_ANIM_INFOS, 0);
            Chara_AnimStateReset(kaufmann);
#if defined(MAP5_S02)
            WorldGfx_CharaMeshSwap(Chara_Kaufmann, MESH_SWAP_STATUS(KaufmannSwappableMesh_1, KaufmannVariantMesh_1));
#endif
            break;

        case KaufmannControl_12:
            Model_AnimStatusKeyframeSet(kaufmann->model, KaufmannAnim_22, true, KAUFMANN_ANIM_INFOS, 0);
            Chara_AnimStateReset(kaufmann);
#if defined(MAP5_S02)
            WorldGfx_CharaMeshSwap(Chara_Kaufmann, MESH_SWAP_STATUS(KaufmannSwappableMesh_1, KaufmannVariantMesh_1));
#endif
            break;

        case KaufmannControl_13:
            Model_AnimStatusSet(&kaufmann->model, KaufmannAnim_10, false);
            Chara_AnimStateReset(kaufmann);
#if defined(MAP5_S02)
            WorldGfx_CharaMeshSwap(Chara_Kaufmann, MESH_SWAP_STATUS(KaufmannSwappableMesh_1, KaufmannVariantMesh_1));
#endif
            break;

        case KaufmannControl_14:
            Model_AnimStatusSet(&kaufmann->model, KaufmannAnim_11, false);
            Chara_AnimStateReset(kaufmann);
#if defined(MAP5_S02)
            WorldGfx_CharaMeshSwap(Chara_Kaufmann, MESH_SWAP_STATUS(KaufmannSwappableMesh_1, KaufmannVariantMesh_1));
#endif
            break;

        case KaufmannControl_15:
            Model_AnimStatusSet(&kaufmann->model, KaufmannAnim_12, false);
            Chara_AnimStateReset(kaufmann);

            if (kaufmann->model.anim.keyframeIdx >= 364 && ANIM_STATUS_IS_ACTIVE(kaufmann->model.anim.status))
            {
                WorldGfx_CharaMeshSwap(Chara_Kaufmann, MESH_SWAP_STATUS(KaufmannSwappableMesh_None, KaufmannVariantMesh_4));
            }
            break;

        case KaufmannControl_16:
            Model_AnimStatusSet(&kaufmann->model, KaufmannAnim_2, false);
            Chara_AnimStateReset(kaufmann);
#if defined(MAP5_S03)
            WorldGfx_CharaMeshSwap(Chara_Kaufmann, MESH_SWAP_STATUS(KaufmannSwappableMesh_3, KaufmannVariantMesh_1));
#endif
            break;

        case KaufmannControl_25:
            Model_AnimStatusSet(&kaufmann->model, KaufmannAnim_21, false);
            Chara_AnimStateReset(kaufmann);
            WorldGfx_CharaMeshSwap(Chara_EndingKaufmann, MESH_SWAP_STATUS(3, 3));
            break;

        case KaufmannControl_17:
            Model_AnimStatusSet(&kaufmann->model, KaufmannAnim_13, false);
            Chara_AnimStateReset(kaufmann);
            break;

        case KaufmannControl_18:
            Model_AnimStatusSet(&kaufmann->model, KaufmannAnim_16, false);
            Chara_AnimStateReset(kaufmann);

#if defined(MAP7_S03)
            // TODO: Invert like case 15? can't find how to match.
            if (kaufmann->model.anim.keyframeIdx < 78 || !ANIM_STATUS_IS_ACTIVE(kaufmann->model.anim.status))
            {
                WorldGfx_CharaMeshSwap(Chara_EndingKaufmann, MESH_SWAP_STATUS(3, 1));
            }
            else
            {
                WorldGfx_CharaMeshSwap(Chara_EndingKaufmann, MESH_SWAP_STATUS(3, 4));
            }
#endif
            break;

        case KaufmannControl_19:
            Model_AnimStatusSet(&kaufmann->model, KaufmannAnim_17, false);
            Chara_AnimStateReset(kaufmann);
#if defined(MAP7_S03)
            WorldGfx_CharaMeshSwap(Chara_EndingKaufmann, MESH_SWAP_STATUS(3, 4));
#endif
            break;

        case KaufmannControl_20:
            Model_AnimStatusKeyframeSet(kaufmann->model, KaufmannAnim_18, true, KAUFMANN_ANIM_INFOS, 0);
            Chara_AnimStateReset(kaufmann);

#if defined(MAP7_S03)
            if (kaufmann->model.anim.keyframeIdx < 141)
            {
                WorldGfx_CharaMeshSwap(Chara_EndingKaufmann, MESH_SWAP_STATUS(3, 4));
            }
            else
            {
                WorldGfx_CharaMeshSwap(Chara_EndingKaufmann, MESH_SWAP_STATUS(3, 1));
            }
#endif
            break;

        case KaufmannControl_21:
            Model_AnimStatusSet(&kaufmann->model, KaufmannAnim_15, false);
            Chara_AnimStateReset(kaufmann);

#if defined(MAP7_S03)
            WorldGfx_CharaMeshSwap(Chara_EndingKaufmann, MESH_SWAP_STATUS(1, 1));
#endif
            break;

        case KaufmannControl_22:
            Model_AnimStatusKeyframeSet(kaufmann->model, KaufmannAnim_19, true, KAUFMANN_ANIM_INFOS, 0);
            Chara_AnimStateReset(kaufmann);
#if defined(MAP7_S03)
            WorldGfx_CharaMeshSwap(Chara_EndingKaufmann, MESH_SWAP_STATUS(3, 1));
#endif
            break;

        case KaufmannControl_23:
            Model_AnimStatusSet(&kaufmann->model, KaufmannAnim_14, false);
            Chara_AnimStateReset(kaufmann);
            break;
    }

    Collision_SurfaceGet(&surface, kaufmann->position.vx, kaufmann->position.vz);
    Player_FootstepSfxGet(surface.floorType, &sfx, &pitch0, &pitch1);

    switch (kaufmannProps.controlState)
    {
        case KaufmannControl_1:
            Npc_FootstepSoundPlay(ANIM_STATUS(KaufmannAnim_3, true), kaufmann, 53, 41, sfx, pitch0);
            break;

        case KaufmannControl_6:
            Npc_FootstepSoundPlay(ANIM_STATUS(KaufmannAnim_4, true), kaufmann, 80, 0, sfx, pitch0);
            break;

        case KaufmannControl_7:
            Npc_SfxPlay(kaufmann->model.anim.status, kaufmann, 95, Sfx_Unk1492);
            break;

        case KaufmannControl_9:
            Npc_FootstepSoundPlay(ANIM_STATUS(KaufmannAnim_7, true), kaufmann, 132, 174, sfx, pitch0);
            break;

        case KaufmannControl_10:
            Npc_FootstepSoundPlay(ANIM_STATUS(KaufmannAnim_8, true), kaufmann, 191, 0, sfx, pitch0);
            break;

        case KaufmannControl_17:
            Npc_FootstepSoundPlay(ANIM_STATUS(KaufmannAnim_13, true), kaufmann, 393, 0, sfx, pitch0);
            break;

        case KaufmannControl_13:
            Npc_SfxPlay(ANIM_STATUS(KaufmannAnim_10, true), kaufmann, 307, Sfx_Unk1594);
            break;

        case KaufmannControl_15:
            if (kaufmann->model.anim.keyframeIdx <= 353)
            {
                Npc_FootstepSoundPlay(ANIM_STATUS(KaufmannAnim_12, true), kaufmann, 357, 353, sfx, pitch0);
            }
            else
            {
                Npc_FootstepSoundPlay(ANIM_STATUS(KaufmannAnim_12, true), kaufmann, 357, 361, sfx, pitch0);
            }
            break;

        case KaufmannControl_16:
            if (kaufmann->model.anim.keyframeIdx <= 21)
            {
                Npc_FootstepSoundPlay(ANIM_STATUS(KaufmannAnim_2, true), kaufmann, 28, 21, sfx, pitch0);
            }
            else
            {
                Npc_FootstepSoundPlay(ANIM_STATUS(KaufmannAnim_2, true), kaufmann, 28, 34, sfx, pitch0);
            }
            break;

        case KaufmannControl_25:
            if (kaufmann->model.anim.keyframeIdx <= 247)
            {
                Npc_FootstepSoundPlay(ANIM_STATUS(KaufmannAnim_21, true), kaufmann, 254, 247, sfx, pitch0);
            }
            else
            {
                Npc_FootstepSoundPlay(ANIM_STATUS(KaufmannAnim_21, true), kaufmann, 254, 260, sfx, pitch0);
            }
            break;

        case KaufmannControl_22:
            if (kaufmann->model.anim.keyframeIdx <= 211)
            {
                Npc_FootstepSoundPlay(ANIM_STATUS(KaufmannAnim_19, true), kaufmann, 217, 211, sfx, pitch0);
            }
            else
            {
                Npc_FootstepSoundPlay(ANIM_STATUS(KaufmannAnim_19, true), kaufmann, 217, 221, sfx, pitch0);
            }
            break;
    }

    kaufmann->rotation.vy  = Q12_ANGLE_ABS(kaufmann->rotation.vy + (sharedData_800D5CF4_3_s00 >> 4));
    kaufmann->headingAngle = kaufmann->rotation.vy;
    kaufmann->moveSpeed    = kaufmannProps.moveSpeed;
    kaufmann->fallSpeed   += g_GravitySpeed;

    boneCoords->flg = false;
    Math_RotMatrixZxyNegGte(&kaufmann->rotation, &boneCoords->coord);
}

/** Addresses
 * MAP3_S00: 0x800CFE48
 * MAP5_S02: 0x800D3AC0
 * MAP5_S03: 0x800D0510
 * MAP7_S02: 0x800D8AA8
 * MAP7_S03: 0x800D4C14
 */
void Kaufmann_Init(s_SubCharacter* kaufmann)
{
    Chara_CollisionReset(kaufmann);

    g_SysWork.npcs[0].properties.npc.field_120 = 0;

    sharedData_800D5CF4_3_s00 = 0;

#ifdef MAP7_S03
    WorldGfx_CharaMeshSwap(Chara_EndingKaufmann, MESH_SWAP_STATUS(1, 0)); // TODO: MAP7_S03 `Kaufmann` funcs could be `EndingKaufmann` instead, if other `Kaufmann` funcs don't match other overlays.
#else
    WorldGfx_CharaMeshSwap(Chara_Kaufmann, MESH_SWAP_STATUS(KaufmannSwappableMesh_1, KaufmannVariantMesh_None));
#endif

#ifdef MAP7_S02
    WorldGfx_CharaModelTransparentSet(Chara_Kaufmann, true);
#endif
}
