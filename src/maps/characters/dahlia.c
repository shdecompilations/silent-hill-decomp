#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player/player.h"
#include "main/rng.h"
#include "maps/shared.h"
#include "maps/characters/dahlia.h"

/** AI code for `Chara_Dahlia`
 *
 * Included in:
 *  MAP2_S01
 *  MAP3_S06
 *  MAP6_S01
 *  MAP6_S04
 *  MAP7_S02
 *  MAP7_S03
 */

#define dahliaProps dahlia->properties.npc

/** Addresses
 * MAP2_S01: 0x800CD20C
 * MAP3_S06: 0x800CF0F8
 * MAP6_S01: 0x800CF7D0
 * MAP6_S04: 0x800DCA30
 * MAP7_S02: 0x800D6ED4
 * MAP7_S03: 0x800D2368
 */
void Dahlia_Update(s_SubCharacter* dahlia, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords)
{
    if (dahlia->model.controlState == DahliaControl_None)
    {
        Dahlia_Init(dahlia);
    }

    Dahlia_ControlUpdate(dahlia, boneCoords);
    Dahlia_MovementUpdate(dahlia, boneCoords);
    Dahlia_AnimUpdate(dahlia, anmHdr, boneCoords);
}

/** Addresses
 * MAP2_S01: 0x800CD280
 * MAP3_S06: 0x800CF16C
 * MAP6_S01: 0x800CF844
 * MAP6_S04: 0x800DCAA4
 * MAP7_S02: 0x800D6F48
 * MAP7_S03: 0x800D23DC
 */
void Dahlia_AnimUpdate(s_SubCharacter* dahlia, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords)
{
    Chara_AnimUpdate(dahlia, anmHdr, boneCoords, DAHLIA_ANIM_INFOS);
}

/** Addresses
 * MAP2_S01: 0x800CD2C8
 * MAP3_S06: 0x800CF1B4
 * MAP6_S01: 0x800CF88C
 * MAP6_S04: 0x800DCAEC
 * MAP7_S02: 0x800D6F90
 * MAP7_S03: 0x800D2424
 */
void Dahlia_MovementUpdate(s_SubCharacter* dahlia, GsCOORDINATE2* boneCoords)
{
    VECTOR3 pos; // @unused
    VECTOR3 offset;
    q19_12  moveSpeed;
    q3_12   headingAngle;
    q19_12  moveDist;
    s32     scaleRestoreShift;
    u32     scaleReduceShift;

    pos          = dahlia->position;
    moveSpeed    = dahlia->moveSpeed;
    headingAngle = dahlia->headingAngle;
    moveDist     = Q12_MULT_PRECISE(moveSpeed, g_DeltaTime);

    scaleRestoreShift = OVERFLOW_GUARD(moveDist);
    scaleReduceShift  = scaleRestoreShift >> 1;

    // Compute movement offset.
    offset.vx = (u32)Q12_MULT_PRECISE(moveDist >> scaleReduceShift, Math_Sin(headingAngle) >> scaleReduceShift) << scaleRestoreShift;
    offset.vz = (u32)Q12_MULT_PRECISE(moveDist >> scaleReduceShift, Math_Cos(headingAngle) >> scaleReduceShift) << scaleRestoreShift;
    offset.vy = Q12_MULT_PRECISE(dahlia->fallSpeed, g_DeltaTime);

    Collision_WallDetect(&sharedData_800E39BC_0_s00, &offset, dahlia);

    // Update position.
    dahlia->position.vx += offset.vx;
    dahlia->position.vy  = Q12(0.0f);
    dahlia->position.vz += offset.vz;

    // Warp to floor.
    if (dahlia->position.vy > sharedData_800E39BC_0_s00.surface.floorHeight)
    {
        dahlia->position.vy = sharedData_800E39BC_0_s00.surface.floorHeight;
        dahlia->fallSpeed   = Q12(0.0f);
    }

    // Update root bone coord.
    boneCoords[0].coord.t[0] = Q12_TO_Q8(dahlia->position.vx);
    boneCoords[0].coord.t[1] = Q12_TO_Q8(dahlia->position.vy);
    boneCoords[0].coord.t[2] = Q12_TO_Q8(dahlia->position.vz);
}

/** Addresses
 * MAP2_S01: 0x800CD4A0
 * MAP3_S06: 0x800CF38C
 * MAP6_S01: 0x800CFA64
 * MAP6_S04: 0x800DCCC4
 * MAP7_S02: 0x800D7168
 * MAP7_S03: 0x800D25FC
 */
void Dahlia_ControlUpdate(s_SubCharacter* dahlia, GsCOORDINATE2* boneCoords)
{
    s_CollisionSurface surface;
    e_SfxId            sfxId;
    s8                 pitch0;
    s8                 pitch1;

    switch (dahliaProps.controlState)
    {
        case DahliaControl_None:
        case DahliaControl_5:
            if (dahliaProps.moveSpeed)
            {
                dahliaProps.moveSpeed -= TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f)) * 2;
                if (dahliaProps.moveSpeed < 0)
                {
                    dahliaProps.moveSpeed = Q12(0.0f);
                }
            }

            Model_AnimStatusSet(&dahlia->model, DahliaAnim_1, false);
            Chara_AnimStateReset(dahlia);
            break;

        case DahliaControl_1:
            if (dahliaProps.moveSpeed > Q12(1.25f))
            {
                dahliaProps.moveSpeed -= TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.5f));
                if (dahliaProps.moveSpeed < Q12(1.25f))
                {
                    dahliaProps.moveSpeed = Q12(1.25f);
                }
            }
            else if (dahliaProps.moveSpeed < Q12(1.25f))
            {
                dahliaProps.moveSpeed += TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f));
                dahliaProps.moveSpeed  = CLAMP(dahliaProps.moveSpeed, 0, Q12(1.25f));
            }

            Model_AnimStatusSet(&dahlia->model, DahliaAnim_2, false);
            Chara_AnimStateReset(dahlia);
            break;

        case DahliaControl_6:
            Model_AnimStatusSet(&dahlia->model, DahliaAnim_14, false);
#if defined(MAP7_S03)
            WorldGfx_CharaMeshSwap(Chara_EndingDahlia, MESH_SWAP_STATUS(0, 1));
#endif
            Chara_AnimStateReset(dahlia);
            break;

        case DahliaControl_7:
            Model_AnimStatusSet(&dahlia->model, DahliaAnim_3, false);
            Chara_AnimStateReset(dahlia);
            break;

        case DahliaControl_8:
            Model_AnimStatusSet(&dahlia->model, DahliaAnim_4, false);
            Chara_AnimStateReset(dahlia);
            break;

        case DahliaControl_11:
#if defined(MAP3_S06) || defined(MAP6_S01) || defined(MAP6_S04) || defined(MAP7_S02) || defined(MAP7_S03)
            Model_AnimStatusKeyframeSet(dahlia->model, DahliaAnim_7, true, DAHLIA_ANIM_INFOS, 0);
#else
            Model_AnimStatusSet(&dahlia->model, DahliaAnim_7, false);
#endif
            Chara_AnimStateReset(dahlia);
            break;

        case DahliaControl_9:
            Model_AnimStatusSet(&dahlia->model, DahliaAnim_5, false);
            Chara_AnimStateReset(dahlia);

            if (dahlia->model.anim.keyframeIdx < 144)
            {
                WorldGfx_CharaMeshSwap(Chara_Dahlia, MESH_SWAP_STATUS(DahliaSwappableMesh_Head, DahliaVariantMesh_1));
            }
            else
            {
#if !defined(MAP6_S01) && !defined(MAP6_S04) && !defined(MAP7_S02) && !defined(MAP7_S03)
#if defined(MAP3_S06)
                WorldGfx_CharaMeshSwap(Chara_Dahlia, MESH_SWAP_STATUS(DahliaSwappableMesh_Head, DahliaVariantMesh_2));
#else
                WorldGfx_CharaMeshSwap(Chara_Dahlia, MESH_SWAP_STATUS(DahliaSwappableMesh_Head, DahliaVariantMesh_3));
#endif
#endif
            }
            break;

        case DahliaControl_10:
#if !defined(MAP6_S01) && !defined(MAP6_S04) && !defined(MAP7_S02) && !defined(MAP7_S03)
#if defined(MAP3_S06)
            Model_AnimStatusKeyframeSet(dahlia->model, DahliaAnim_6, true, DAHLIA_ANIM_INFOS, 0);
#else
            Model_AnimStatusSet(&dahlia->model, DahliaAnim_6, false);
#endif
#endif
            Chara_AnimStateReset(dahlia);

            if (dahlia->model.anim.keyframeIdx < 198)
            {
#if !defined(MAP6_S01) && !defined(MAP6_S04) && !defined(MAP7_S02) && !defined(MAP7_S03)
#if defined(MAP3_S06)
                WorldGfx_CharaMeshSwap(Chara_Dahlia, MESH_SWAP_STATUS(DahliaSwappableMesh_Head, DahliaVariantMesh_2));
#else
                WorldGfx_CharaMeshSwap(Chara_Dahlia, MESH_SWAP_STATUS(DahliaSwappableMesh_Head, DahliaVariantMesh_3));
#endif
#endif
            }
            else
            {
                WorldGfx_CharaMeshSwap(Chara_Dahlia, MESH_SWAP_STATUS(DahliaSwappableMesh_Head, DahliaVariantMesh_1));
            }
            break;

        case DahliaControl_12:
            Model_AnimStatusKeyframeSet(dahlia->model, DahliaAnim_8, true, DAHLIA_ANIM_INFOS, 0);
            Chara_AnimStateReset(dahlia);
            break;

        case DahliaControl_13:
            Model_AnimStatusSet(&dahlia->model, DahliaAnim_9, false);
            Chara_AnimStateReset(dahlia);
            break;

        case DahliaControl_14:
            Model_AnimStatusSet(&dahlia->model, DahliaAnim_10, false);
            Chara_AnimStateReset(dahlia);
            break;

        case DahliaControl_15:
            Model_AnimStatusSet(&dahlia->model, DahliaAnim_15, false);
            Chara_AnimStateReset(dahlia);
            break;

        case DahliaControl_16:
            Model_AnimStatusSet(&dahlia->model, DahliaAnim_16, false);
            Chara_AnimStateReset(dahlia);

            if (dahlia->model.anim.keyframeIdx == 103)
            {
                dahliaProps.controlState = DahliaControl_5;
                dahlia->model.stateStep  = 0;
            }
            break;

        case DahliaControl_17:
            Model_AnimStatusSet(&dahlia->model, DahliaAnim_17, false);
            Chara_AnimStateReset(dahlia);
            break;

        case DahliaControl_18:
            Model_AnimStatusSet(&dahlia->model, DahliaAnim_18, false);
            Chara_AnimStateReset(dahlia);
            break;

        case DahliaControl_19:
            if (dahlia->model.anim.keyframeIdx >= 40 && !sharedData_800D16E4_2_s01)
            {
                func_8005F6B0(&g_SysWork.npcs[1], &Q12_VECTOR3(138.3f, -1.1f, -100.8f), 3, 1);
                sharedData_800D16E4_2_s01 = 1;
            }

            Model_AnimStatusKeyframeSet(dahlia->model, DahliaAnim_19, true, DAHLIA_ANIM_INFOS, 0);
            Chara_AnimStateReset(dahlia);

            if (dahliaProps.controlState != DahliaControl_19)
            {
                sharedData_800D16E4_2_s01 = 0;
            }
            break;

        case DahliaControl_20:
            Model_AnimStatusSet(&dahlia->model, DahliaAnim_20, false);
            Chara_AnimStateReset(dahlia);
            break;

        case DahliaControl_21:
            Model_AnimStatusKeyframeSet(dahlia->model, DahliaAnim_21, true, DAHLIA_ANIM_INFOS, 0);
            Chara_AnimStateReset(dahlia);
            break;

        case DahliaControl_22:
            Model_AnimStatusKeyframeSet(dahlia->model, DahliaAnim_22, true, DAHLIA_ANIM_INFOS, 0);
            Chara_AnimStateReset(dahlia);
            break;

        case DahliaControl_23:
            Model_AnimStatusSet(&dahlia->model, DahliaAnim_23, false);
            Chara_AnimStateReset(dahlia);
            break;

        case DahliaControl_24:
            Model_AnimStatusSet(&dahlia->model, DahliaAnim_24, false);
            Chara_AnimStateReset(dahlia);
            break;

        case DahliaControl_25:
            Model_AnimStatusSet(&dahlia->model, DahliaAnim_11, false);
            WorldGfx_CharaMeshSwap(Chara_Dahlia, MESH_SWAP_STATUS(DahliaSwappableMesh_Head, DahliaVariantMesh_1));

            if (dahlia->model.anim.keyframeIdx == 349)
            {
                dahliaProps.controlState = DahliaControl_26;
                dahlia->model.stateStep  = 0;
                break;
            }
            break;

        case DahliaControl_26:
            Model_AnimStatusSet(&dahlia->model, DahliaAnim_12, false);
            Chara_AnimStateReset(dahlia);
            break;

        case DahliaControl_27:
            Model_AnimStatusSet(&dahlia->model, DahliaAnim_13, false);
            Chara_AnimStateReset(dahlia);
            break;

        case DahliaControl_28:
            Model_AnimStatusSet(&dahlia->model, DahliaAnim_25, false);
            Chara_AnimStateReset(dahlia);
            break;
    }

    Collision_SurfaceGet(&surface, dahlia->position.vx, dahlia->position.vz);

    Player_FootstepSfxGet(surface.floorType, &sfxId, &pitch0, &pitch1);
#if defined(MAP6_S01)
    sfxId = Sfx_Unk1606;
#else
    sfxId = Sfx_Unk1489;
#endif

    switch (dahliaProps.controlState)
    {
        case DahliaControl_1:
            Npc_FootstepSoundPlay(ANIM_STATUS(DahliaAnim_2, true), dahlia, 24, 37, sfxId, pitch0);
            break;

        case DahliaControl_8:
            Npc_FootstepSoundPlay(ANIM_STATUS(DahliaAnim_4, true), dahlia, 107, 98, sfxId, pitch0);
            break;

        case DahliaControl_11:
            Npc_FootstepSoundPlay(ANIM_STATUS(DahliaAnim_7, true), dahlia, 235, 245, sfxId, pitch0);
            break;

        case DahliaControl_7:
            if (dahlia->model.anim.keyframeIdx < 47)
            {
                Npc_SfxPlay(dahlia->model.anim.status, dahlia, 46, Sfx_Unk1674);
            }
            else
            {
                Npc_SfxPlay(dahlia->model.anim.status, dahlia, 47, Sfx_Unk1675);
            }
            break;

        default:
            break;

        case DahliaControl_17:
            if (dahlia->model.anim.keyframeIdx < 127)
            {
                Npc_SfxPlay(ANIM_STATUS(DahliaAnim_17, true), dahlia, 120, Sfx_Unk1674);
            }
            else
            {
                Npc_SfxPlay(ANIM_STATUS(DahliaAnim_17, true), dahlia, 127, Sfx_Unk1675);
            }
            break;

        case DahliaControl_10:
            if (g_SavegamePtr->mapIdx == MapIdx_MAP2_S01)
            {
                Npc_SfxPlay(dahlia->model.anim.status, dahlia, 193, Sfx_Unk1488);
            }
            else
            {
                Npc_SfxPlay(dahlia->model.anim.status, dahlia, 193, Sfx_Unk1496);
            }
            break;

        case DahliaControl_14:
            if (dahlia->model.anim.keyframeIdx < 310)
            {
                Npc_FootstepSoundPlay(dahlia->model.anim.status, dahlia, 319, 309, sfxId, pitch0);
            }
            else
            {
                Npc_FootstepSoundPlay(dahlia->model.anim.status, dahlia, 319, 329, sfxId, pitch0);
            }
            break;

        case DahliaControl_19:
            if (dahlia->model.anim.keyframeIdx < 175)
            {

                Npc_SfxPlay(ANIM_STATUS(DahliaAnim_19, true), dahlia, 170, Sfx_Unk1674);
            }
            else
            {
                Npc_SfxPlay(ANIM_STATUS(DahliaAnim_19, true), dahlia, 177, Sfx_Unk1675);
            }
            break;
    }

    dahlia->rotation.vy  = Q12_ANGLE_ABS(dahlia->rotation.vy + Q8_TO_Q4(sharedData_800D16E0_2_s01));
    dahlia->headingAngle = dahlia->rotation.vy;
    dahlia->moveSpeed    = dahliaProps.moveSpeed;
    dahlia->fallSpeed   += g_GravitySpeed;

    boneCoords->flg = false;
    Math_RotMatrixZxyNegGte(&dahlia->rotation, &boneCoords->coord);
}

/** Addresses
 * MAP2_S01: 0x800CDD64
 * MAP3_S06: 0x800CFC9C
 * MAP6_S01: 0x800D031C
 * MAP6_S04: 0x800DD57C
 * MAP7_S02: 0x800D7A20
 * MAP7_S03: 0x800D2ECC
 */
void Dahlia_Init(s_SubCharacter* dahlia)
{
    Chara_CollisionReset(dahlia);

    sharedData_800D16E0_2_s01 = 0;
    sharedData_800D16E4_2_s01 = 0;

#ifdef MAP7_S03
    WorldGfx_CharaMeshSwap(Chara_EndingDahlia, MESH_SWAP_STATUS(0, 1)); // TODO: MAP7_S03 Dahlia funcs could be EndingDahlia instead, if other Dahlia funcs don't match other overlays.
#else
    WorldGfx_CharaMeshSwap(Chara_Dahlia, MESH_SWAP_STATUS(DahliaSwappableMesh_Head, DahliaVariantMesh_1));
#endif

#ifdef MAP7_S02
    WorldGfx_CharaModelTransparentSet(Chara_Dahlia, true);
#endif
}
