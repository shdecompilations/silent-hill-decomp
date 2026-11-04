#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "maps/common/shared.h"
#include "maps/common/characters/ghost_child_alessa.h"

/** AI code for `Chara_GhostChildAlessa`
 *
 * Included in:
 *  MAP7_S01
 *  MAP7_S02
 */

#define ghostAlessaProps (ghostAlessa->properties.npc)

/** Addresses
 * MAP7_S01: 0x800D49B8
 * MAP7_S02: 0x800D7A68
 */
void GhostChildAlessa_Update(s_SubCharacter* ghostAlessa, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords)
{
    if (ghostAlessa->model.charaId != Chara_GhostChildAlessa)
    {
        GhostChildAlessa_Init(ghostAlessa);
    }

    GhostChildAlessa_AnimStateUpdate(ghostAlessa, boneCoords);
    GhostChildAlessa_MovementUpdate(ghostAlessa, boneCoords);
    GhostChildAlessa_AnimUpdate(ghostAlessa, anmHdr, boneCoords);
}

/** Addresses
 * MAP7_S01: 0x800D4A2C
 * MAP7_S02: 0x800D7ADC
 */
void GhostChildAlessa_AnimUpdate(s_SubCharacter* ghostAlessa, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords)
{
    Chara_AnimUpdate(ghostAlessa, anmHdr, boneCoords, GHOST_CHILD_ALESSA_ANIM_INFOS);
}

/** Addresses
 * MAP7_S01: 0x800D4A74
 * MAP7_S02: 0x800D7B24
 */
void GhostChildAlessa_MovementUpdate(s_SubCharacter* ghostAlessa, GsCOORDINATE2* boneCoords)
{
    VECTOR3 pos; // @unused
    VECTOR3 offset;
    q19_12  moveSpeed;
    q3_12   headingAngle;
    q19_12  moveDist;
    s32     scaleRestoreShift;
    u32     scaleReduceShift;

    pos          = ghostAlessa->position;
    moveSpeed    = ghostAlessa->moveSpeed;
    headingAngle = ghostAlessa->headingAngle;
    moveDist     = Q12_MULT_PRECISE(moveSpeed, g_DeltaTime);

    scaleRestoreShift = OVERFLOW_GUARD(moveDist);
    scaleReduceShift  = scaleRestoreShift >> 1;

    offset.vx = (u32)Q12_MULT_PRECISE(moveDist >> scaleReduceShift, Math_Sin(headingAngle) >> scaleReduceShift) << scaleRestoreShift;
    offset.vz = (u32)Q12_MULT_PRECISE(moveDist >> scaleReduceShift, Math_Cos(headingAngle) >> scaleReduceShift) << scaleRestoreShift;
    offset.vy = Q12_MULT_PRECISE(ghostAlessa->fallSpeed, g_DeltaTime);

    ghostAlessa->position.vx += offset.vx;
    ghostAlessa->position.vy  = Q12(0.0f);
    ghostAlessa->position.vz += offset.vz;

    boneCoords[GhostChildAlessaBone_Root].coord.t[0] = Q12_TO_Q8(ghostAlessa->position.vx);
    boneCoords[GhostChildAlessaBone_Root].coord.t[1] = Q12_TO_Q8(ghostAlessa->position.vy);
    boneCoords[GhostChildAlessaBone_Root].coord.t[2] = Q12_TO_Q8(ghostAlessa->position.vz);
}

/** Addresses
 * MAP7_S01: 0x800D4C0C
 * MAP7_S02: 0x800D7CBC
 */
void GhostChildAlessa_AnimStateUpdate(s_SubCharacter* ghostAlessa, GsCOORDINATE2* boneCoords)
{
    switch (ghostAlessaProps.controlState)
    {
        case GhostChildAlessaControl_None:
            break;

        case GhostChildAlessaControl_1:
            ghostAlessaProps.moveSpeed = ghostAlessaProps.moveDistance_124;
            Model_AnimStatusSet(&ghostAlessa->model, GhostChildAlessaAnim_1, false);
            Chara_AnimStateReset(ghostAlessa);
            break;

        case GhostChildAlessaControl_2:
            ghostAlessaProps.moveSpeed = ghostAlessaProps.moveDistance_124;
            Model_AnimStatusSet(&ghostAlessa->model, GhostChildAlessaAnim_2, false);
            Chara_AnimStateReset(ghostAlessa);
            break;

        case GhostChildAlessaControl_6:
            Model_AnimStatusSet(&ghostAlessa->model, GhostChildAlessaAnim_3, false);
            if (ghostAlessa->model.anim.keyframeIdx == 63)
            {
                ghostAlessaProps.controlState = GhostChildAlessaControl_7;
                ghostAlessa->model.stateStep  = 0;
            }
            break;

        case GhostChildAlessaControl_7:
            Model_AnimStatusSet(&ghostAlessa->model, GhostChildAlessaAnim_4, false);
            Chara_AnimStateReset(ghostAlessa);
            break;

        case GhostChildAlessaControl_8:
            Model_AnimStatusSet(&ghostAlessa->model, GhostChildAlessaAnim_5, false);
            Chara_AnimStateReset(ghostAlessa);
            break;

        case GhostChildAlessaControl_9:
            Model_AnimStatusSet(&ghostAlessa->model, GhostChildAlessaAnim_6, false);
            Chara_AnimStateReset(ghostAlessa);
            break;

        case GhostChildAlessaControl_10:
            Model_AnimStatusSet(&ghostAlessa->model, GhostChildAlessaAnim_7, false);
            Chara_AnimStateReset(ghostAlessa);
            break;

        case GhostChildAlessaControl_11:
            Model_AnimStatusSet(&ghostAlessa->model, GhostChildAlessaAnim_8, false);
            Chara_AnimStateReset(ghostAlessa);
            break;
    }

    ghostAlessa->rotation.vy  = Q12_ANGLE_ABS(ghostAlessa->rotation.vy + (sharedData_800E39D8_0_s00 >> 4));
    ghostAlessa->headingAngle = ghostAlessa->rotation.vy;
    ghostAlessa->moveSpeed    = ghostAlessaProps.moveSpeed;
    ghostAlessa->fallSpeed   += g_GravitySpeed;

    boneCoords->flg = false;
    Math_RotMatrixZxyNegGte(&ghostAlessa->rotation, &boneCoords[GhostChildAlessaBone_Root].coord);
}

/** Addresses
 * MAP7_S01: 0x800D4DD8
 * MAP7_S02: 0x800D7E88
 */
void GhostChildAlessa_Init(s_SubCharacter* ghostAlessa)
{
    Chara_CollisionReset(ghostAlessa);
    sharedData_800E39D8_0_s00 = 0;
}
