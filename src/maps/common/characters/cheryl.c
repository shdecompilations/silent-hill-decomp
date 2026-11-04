#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player/player.h"
#include "maps/common/shared.h"
#include "maps/common/characters/cheryl.h"

/** AI code for `Chara_Cheryl`
 *
 * Included in:
 *  MAP0_S00
 */

#define cherylProps cheryl->properties.npc

void Cheryl_Update(s_SubCharacter* cheryl, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords) // 0x800D7FB8
{
    if (cheryl->model.charaId != Chara_Cheryl)
    {
        Cheryl_Init(cheryl);
    }

    Cheryl_ControlUpdate(cheryl, boneCoords);
    Cheryl_MovementUpdate(cheryl, boneCoords);
    Cheryl_AnimUpdate(cheryl, anmHdr, boneCoords);
}

void Cheryl_AnimUpdate(s_SubCharacter* cheryl, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords) // 0x800D802C
{
    q19_12      moveSpeed;
    q19_12      duration;
    s_AnimInfo* animInfo;

    if (cherylProps.controlState == 1)
    {
        D_800DF1CC = Q12_MULT_PRECISE(cheryl->moveSpeed, Q12(30.2f));
    }

    moveSpeed         = MIN(cheryl->moveSpeed, Q12(2.5f));
    cheryl->moveSpeed = moveSpeed;

    if (cherylProps.controlState == 2)
    {
        animInfo = CHERYL_ANIM_INFOS;

        if (moveSpeed <= Q12(1.5f))
        {
            duration = Q12_MULT_PRECISE(moveSpeed, Q12(18.6f));
        }
        else
        {
            duration = Q12(27.9f) - 1;
        }

        animInfo[ANIM_STATUS(CherylAnim_RunForward, true)].duration.constant = duration;
    }

    Chara_AnimUpdate(cheryl, anmHdr, boneCoords, CHERYL_ANIM_INFOS);
}

void Cheryl_MovementUpdate(s_SubCharacter* cheryl, GsCOORDINATE2* boneCoords) // 0x800D8124
{
    VECTOR3 pos; // @hack Unused but required for match.
    VECTOR3 offset;
    q19_12  moveSpeed;
    q3_12   headingAngle;
    q19_12  moveDist;
    s32     scaleRestoreShift;
    u32     scaleReduceShift;

    pos          = cheryl->position;
    moveSpeed    = cheryl->moveSpeed;
    headingAngle = cheryl->headingAngle;
    moveDist     = Q12_MULT_PRECISE(moveSpeed, g_DeltaTime);

    scaleRestoreShift = OVERFLOW_GUARD(moveDist);
    scaleReduceShift  = scaleRestoreShift >> 1;

    offset.vx = (u32)Q12_MULT_PRECISE(moveDist >> scaleReduceShift, Math_Sin(headingAngle) >> scaleReduceShift) << scaleRestoreShift;
    offset.vz = (u32)Q12_MULT_PRECISE(moveDist >> scaleReduceShift, Math_Cos(headingAngle) >> scaleReduceShift) << scaleRestoreShift;
    offset.vy = Q12_MULT_PRECISE(cheryl->fallSpeed, g_DeltaTime);

    Collision_WallDetect(&sharedData_800E39BC_0_s00, &offset, cheryl);

    cheryl->position.vx += offset.vx;
    cheryl->position.vy += sharedData_800E39BC_0_s00.offset.vy;
    cheryl->position.vz += offset.vz;

    if (cheryl->position.vy > sharedData_800E39BC_0_s00.surface.floorHeight)
    {
        cheryl->position.vy = sharedData_800E39BC_0_s00.surface.floorHeight;
        cheryl->fallSpeed   = Q12(0.0f);
    }

    boneCoords[0].coord.t[0] = Q12_TO_Q8(cheryl->position.vx);
    boneCoords[0].coord.t[1] = Q12_TO_Q8(cheryl->position.vy);
    boneCoords[0].coord.t[2] = Q12_TO_Q8(cheryl->position.vz);
}

void Cheryl_ControlUpdate(s_SubCharacter* cheryl, GsCOORDINATE2* boneCoords) // 0x800D8310
{
    s_CollisionSurface surface;
    e_SfxId            sfx;
    s8                 pitch0;
    s8                 pitch1;
    q23_8              distToPlayerSqr;

    #define playerChara g_SysWork.playerWork.player
    #define playerProps playerChara.properties.player

    D_800E3A30 = Q12(0.0f);

    switch (cherylProps.controlState)
    {
        case CherylControl_Idle:
            if (cherylProps.moveSpeed != Q12(0.0f))
            {
                cherylProps.moveSpeed -= TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f)) * 2;
                if (cherylProps.moveSpeed < Q12(0.0f))
                {
                    cherylProps.moveSpeed = Q12(0.0f);
                }
            }

            Model_AnimStatusSet(&cheryl->model, CherylAnim_Idle, false);
            Chara_AnimStateReset(cheryl);

            cherylProps.moveDistance_124 = Q12(0.0f);
            break;

        case CherylControl_WalkForward:
            cherylProps.moveSpeed = cherylProps.moveDistance_124;

            Model_AnimStatusSet(&cheryl->model, CherylAnim_WalkForward, false);
            Chara_AnimStateReset(cheryl);
            break;

        case CherylControl_RunForward:
            cherylProps.moveSpeed = cherylProps.moveDistance_124;

            Model_AnimStatusSet(&cheryl->model, CherylAnim_RunForward, false);
            Chara_AnimStateReset(cheryl);
            break;

        case CherylControl_3:
            if (cherylProps.moveSpeed != Q12(0.0f))
            {
                cherylProps.moveSpeed -= TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f)) * 2;
                if (cherylProps.moveSpeed < Q12(0.0f))
                {
                    cherylProps.moveSpeed = Q12(0.0f);
                }
            }

            D_800E3A30 = g_DeltaTime * 7;

            Model_AnimStatusSet(&cheryl->model, CherylAnim_WalkForward, false);
            Chara_AnimStateReset(cheryl);

            playerProps.headingAngle = Q12_ANGLE(0.0f);
            cherylProps.moveDistance_124 = Q12(0.0f);
            break;

        case CherylControl_4:
            if (cherylProps.moveSpeed != Q12(0.0f))
            {
                cherylProps.moveSpeed -= TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f)) * 2;
                if (cherylProps.moveSpeed < Q12(0.0f))
                {
                    cherylProps.moveSpeed = Q12(0.0f);
                }
            }

            D_800E3A30 = g_DeltaTime * -7;

            Model_AnimStatusSet(&cheryl->model, CherylAnim_WalkForward, false);
            Chara_AnimStateReset(cheryl);

            playerProps.headingAngle = Q12_ANGLE(0.0f);
            cherylProps.moveDistance_124 = Q12(0.0f);
            break;
    }

    Collision_SurfaceGet(&surface, cheryl->position.vx, cheryl->position.vz);
    Player_FootstepSfxGet(surface.floorType, &sfx, &pitch0, &pitch1);

    distToPlayerSqr = SQUARE(Q12_TO_Q8(cheryl->position.vx - playerChara.position.vx)) +
                      SQUARE(Q12_TO_Q8(cheryl->position.vz - playerChara.position.vz));

    if (g_Player_DisableControl)
    {
        switch (cherylProps.controlState)
        {
            case CherylControl_WalkForward:
                Npc_FootstepSoundPlay(ANIM_STATUS(CherylAnim_WalkForward, true), cheryl, 16, 28, Sfx_CherylFootstep, pitch0);
                break;

            case CherylControl_RunForward:
                Npc_FootstepSoundPlay(ANIM_STATUS(CherylAnim_RunForward, true), cheryl, 53, 42, Sfx_CherylFootstep, pitch1);
                break;
        }
    }
    else
    {
        switch (cherylProps.controlState)
        {
            case CherylControl_WalkForward:
                Cheryl_FootstepSfxPlay(ANIM_STATUS(CherylAnim_WalkForward, true), cheryl, 16, 28, distToPlayerSqr, pitch0);
                break;

            case CherylControl_RunForward:
                Cheryl_FootstepSfxPlay(ANIM_STATUS(CherylAnim_RunForward, true), cheryl, 53, 42, distToPlayerSqr, pitch1);
                break;
        }
    }

    cheryl->rotation.vy  = Q12_ANGLE_ABS(cheryl->rotation.vy + (D_800E3A30 >> 4));
    cheryl->headingAngle = cheryl->rotation.vy;
    cheryl->moveSpeed    = cherylProps.moveDistance_124;
    cheryl->fallSpeed   += g_GravitySpeed;

    boneCoords->flg = false;
    Math_RotMatrixZxyNegGte(&cheryl->rotation, &boneCoords->coord);
}

bool Cheryl_FootstepSfxPlay(s32 animStatus, s_SubCharacter* cheryl,
                            s32 leftFootstepKeyframeIdx, s32 rightFootstepKeyframeIdx, q23_8 distToPlayerSqr, s32 pitch) // 0x800D8748
{
    q24_8 distSqr;
    u32   vol;

    if (cheryl->model.anim.status == animStatus)
    {
        // Compute volume.
        if (distToPlayerSqr >= SQUARE(2000))
        {
            if (distToPlayerSqr <= 12000000)
            {
                distSqr = distToPlayerSqr;
            }
            else
            {
                distSqr = 12000000;
            }
        }
        else
        {
            distSqr = SQUARE(2000);
        }
        vol = (12000000 - distSqr) >> 16;

        // Play right footstep SFX.
        if (cheryl->model.anim.keyframeIdx >= rightFootstepKeyframeIdx)
        {
            if (!(cherylProps.flags & CherylFlag_FootstepRight))
            {
                Sfx_WithPitchPlay(Sfx_CherylFootstep, &cheryl->position, Q8_FRACT(vol), pitch);
                cherylProps.flags |= CherylFlag_FootstepRight;
                return true;
            }
        }
        else
        {
            cherylProps.flags &= ~CherylFlag_FootstepRight;
        }

        // Play left footstep SFX.
        if (cheryl->model.anim.keyframeIdx >= leftFootstepKeyframeIdx)
        {
            if (!(cherylProps.flags & CherylFlag_FootstepLeft))
            {
                Sfx_WithPitchPlay(Sfx_CherylFootstep, &cheryl->position, Q8_FRACT(vol), pitch);
                cherylProps.flags |= CherylFlag_FootstepLeft;
                return true;
            }
        }
        else
        {
            cherylProps.flags &= ~CherylFlag_FootstepLeft;
        }
    }

    return false;
}

void Cheryl_Init(s_SubCharacter* cheryl) // 0x800D8888
{
    Chara_CollisionReset(cheryl);
    D_800E3A30 = Q12(0.0f);
}
