#include "game.h"

#include "bodyprog/bodyprog.h"
#include "bodyprog/map/map.h"
#include "bodyprog/math/math.h"
#include "bodyprog/view/vc_util.h"
#include "bodyprog/view/vw_system.h"
#include "bodyprog/screen/screen_data.h"

#define V_BLANKS_MULT 11

VECTOR3 vcRefPosSt; /** Q19.12 | View camera reference position start. */

void vcInitCamera(s_MapOverlayHdr* map_overlay_ptr, const VECTOR3* chr_pos) // 0x8004004C
{
    g_WorldGfxWork.vcCameraInternalInfo.mv_smooth   = VC_MV_CHASE;
    g_WorldGfxWork.vcCameraInternalInfo.ev_cam_rate = Q12(0.0f);
    g_WorldGfxWork.vcCameraInternalInfo.mode        = 0;

    vcSetCameraUseWarp(chr_pos, g_SysWork.cameraAngleY);

    SetGeomScreen(SCREEN_GEOM_DIST);

    vwInitViewInfo();
    vcInitVCSystem(map_overlay_ptr->cameraPaths);
    vcStartCameraSystem();

    g_SysWork.cameraAngleZ   = Q12_ANGLE(0.0f);
    g_SysWork.cameraRadiusXz = Q12(3.0f);
    g_SysWork.cameraY        = Q12(0.0f);
}

void vcSetCameraUseWarp(const VECTOR3* chr_pos, q3_12 chr_ang_y) // 0x800400D4
{
    #define RADIUS Q12(1.5f)
    #define HEIGHT Q12(1.7f)

    VECTOR3 cam_pos; // Q19.12
    SVECTOR cam_ang; // Q3.12

    // Set rotation.
    cam_ang.vx = Q12_ANGLE(0.0f);
    cam_ang.vy = chr_ang_y;
    cam_ang.vz = Q12_ANGLE(0.0f);

    // Set position.
    cam_pos.vx = chr_pos->vx - Q12_MULT(Math_Sin(chr_ang_y), RADIUS);
    cam_pos.vy = chr_pos->vy - HEIGHT;
    cam_pos.vz = chr_pos->vz - Q12_MULT(Math_Cos(chr_ang_y), RADIUS);

    vcSetFirstCamWork(&cam_pos, chr_ang_y, g_SysWork.sysFlags & SysFlag_SetCameraRail);
    g_SysWork.sysFlags &= ~SysFlag_SetCameraRail;

    #undef RADIUS
    #undef HEIGHT
}

s32 vcRetCamMvSmoothF(void) // 0x80040190
{
    return g_WorldGfxWork.vcCameraInternalInfo.mv_smooth;
}

void Vc_CameraElevationRateLockSet(bool isUnlocked) // 0x800401A0
{
    if (isUnlocked)
    {
        g_WorldGfxWork.vcCameraInternalInfo.ev_cam_rate = Q12(1.0f);
    }
    else
    {
        g_WorldGfxWork.vcCameraInternalInfo.ev_cam_rate = Q12(0.0f);
    }
}

void vcSetEvCamRate(q3_12 ev_cam_rate) // 0x800401C0
{
    g_WorldGfxWork.vcCameraInternalInfo.ev_cam_rate = ev_cam_rate;
}

void Vc_UpdateLookAtPointSetAlt(void) // 0x800401CC
{
    Vc_UpdateLookAtPointSet();
}

void vcMoveAndSetCamera(bool in_connect_f, bool change_debug_mode, bool for_f, bool back_f, bool right_f, bool left_f, bool up_f, bool down_f) // 0x800401EC
{
    VECTOR3            first_cam_pos; // Q19.12
    VECTOR3            hr_head_pos;   // Q19.12
    s_CollisionSurface surface;
    q19_12             hero_bottom_y; // Player bottom height.
    q19_12             hero_top_y;    // Player top height.
    q19_12             grnd_y;        // Absolute floor height.
    s_SubCharacter*    hr_p;          // Player character.

    // Step to next debug mode.
    if (change_debug_mode)
    {
        g_WorldGfxWork.vcCameraInternalInfo.mode++;
    }

    // Handle debug mode.
    switch (g_WorldGfxWork.vcCameraInternalInfo.mode)
    {
        default: // `DebugCameraMode_Init`
            g_WorldGfxWork.vcCameraInternalInfo.mode = 0;

            first_cam_pos.vy = Q12(-2.2f);
            first_cam_pos.vx = g_SysWork.playerWork.player.position.vx + Q12(7.0f);
            first_cam_pos.vz = g_SysWork.playerWork.player.position.vz;

            vcSetFirstCamWork(&first_cam_pos, g_SysWork.playerWork.player.rotation.vy, false);

        case DebugCameraMode_Collide:
            hr_p = &g_SysWork.playerWork.player;

            if (in_connect_f)
            {
                grnd_y = Q12(-2.0f);

                hr_head_pos.vx = hr_p->position.vx;
                hr_head_pos.vy = hr_p->position.vy - Q12(1.9f);
                hr_head_pos.vz = hr_p->position.vz;
            }
            else
            {
                Collision_SurfaceGet(&surface, hr_p->position.vx, hr_p->position.vz);
                grnd_y = surface.floorHeight;

                vcMakeHeroHeadPos(&hr_head_pos);
            }

            hero_top_y    = hr_p->position.vy + Q12(-1.7f);
            hero_bottom_y = hr_p->position.vy + Q12_MULT(g_WorldGfxWork.vcCameraInternalInfo.ev_cam_rate, Q12(-0.5f));

            if (g_WorldGfxWork.vcCameraInternalInfo.ev_cam_rate > Q12(0.0f))
            {
                vcWorkSetFlags(VC_INHIBIT_FAR_WATCH_F, VC_NOFLAG);
            }
            else
            {
                vcWorkSetFlags(VC_NOFLAG, VC_INHIBIT_FAR_WATCH_F);
            }

            vcSetSubjChara(&hr_p->position, hero_bottom_y, hero_top_y, grnd_y,
                           &hr_head_pos,
                           hr_p->moveSpeed, hr_p->headingAngle, hr_p->rotationSpeed.vy,
                           hr_p->rotation.vy, Q12_ANGLE(120.0f), Q12(11.0f));

            g_WorldGfxWork.vcCameraInternalInfo.mv_smooth = vcExecCamera();
            break;

        case DebugCameraMode_SetReference:
            vcSetRefPosAndSysRef2CamParam(&vcRefPosSt, &g_SysWork, for_f, back_f, right_f, left_f, up_f, down_f);
            vwSetCoordRefAndEntou(NULL,
                                  vcRefPosSt.vx, vcRefPosSt.vy, vcRefPosSt.vz,
                                  g_SysWork.cameraAngleY, g_SysWork.cameraAngleZ, g_SysWork.cameraY, g_SysWork.cameraRadiusXz);
            break;

        case DebugCameraMode_AnalogStickControl:
            vcSetRefPosAndCamPosAngByPad(&vcRefPosSt, &g_SysWork);
            break;

        case DebugCameraMode_ResetReference:
            vcSetRefPosAndSysRef2CamParam(&vcRefPosSt, &g_SysWork, for_f, back_f, right_f, left_f, up_f, down_f);
            vwSetCoordRefAndEntou(&g_SysWork.playerBoneCoords[HarryBone_Head],
                                  Q12(0.0f), Q12(-0.15f), Q12(1.0f),
                                  Q12_ANGLE(165.0f), Q12_ANGLE(0.0f), Q12(-0.2f), Q12(1.0f));
            break;
    }

    vwSetViewInfo();
}

void vcMakeHeroHeadPos(VECTOR3* head_pos) // 0x8004047C
{
    #define OFFSET_Y Q12(-0.3f)

    MATRIX  neck_lwm; // Q23.8
    SVECTOR fpos;     // Q23.8
    VECTOR  vec;

    Vw_CoordHierarchyMatrixCompute(&g_SysWork.playerBoneCoords[HarryBone_Head], &neck_lwm);

    fpos.vx = Q8(0.0f);
    fpos.vy = Q8(-0.1f);
    fpos.vz = Q8(0.0f);
    ApplyMatrix(&neck_lwm, &fpos, &vec);

    head_pos->vx = Q8_TO_Q12(vec.vx + neck_lwm.t[0]);
    head_pos->vy = Q8_TO_Q12(vec.vy + neck_lwm.t[1]) + OFFSET_Y;
    head_pos->vz = Q8_TO_Q12(vec.vz + neck_lwm.t[2]);

    #undef OFFSET_Y
}

void vcAddOfsToPos(VECTOR3* out_pos, const VECTOR3* in_pos, q3_12 ofs_xz_r, q3_12 ang_y, q19_12 ofs_y) // 0x80040518
{
    out_pos->vx = in_pos->vx + Q12_MULT(ofs_xz_r, Math_Sin(ang_y));
    out_pos->vz = in_pos->vz + Q12_MULT(ofs_xz_r, Math_Cos(ang_y));
    out_pos->vy = in_pos->vy + ofs_y;
}

void vcSetRefPosAndSysRef2CamParam(VECTOR3* ref_pos, s_SysWork* sys_p,
                                   bool for_f, bool back_f, bool right_f, bool left_f, bool up_f, bool down_f) // 0x800405C4
{
    #define POS_OFFSET Q12(0.1f)
    #define RADIUS_MIN Q12(1.0f)

    if (for_f)
    {
        sys_p->cameraRadiusXz -= POS_OFFSET;
    }
    if (back_f)
    {
        sys_p->cameraRadiusXz += POS_OFFSET;
    }
    if (right_f)
    {
        sys_p->cameraAngleY -= g_VBlanks * V_BLANKS_MULT;
    }
    if (left_f)
    {
        sys_p->cameraAngleY += g_VBlanks * V_BLANKS_MULT;
    }
    if (up_f)
    {
        sys_p->cameraY -= POS_OFFSET;
    }
    if (down_f)
    {
        sys_p->cameraY += POS_OFFSET;
    }
    if (sys_p->cameraRadiusXz < RADIUS_MIN)
    {
        sys_p->cameraRadiusXz = RADIUS_MIN;
    }

    vcAddOfsToPos(ref_pos, &g_SysWork.playerWork.player.position,
                  Q12(0.5f), g_SysWork.playerWork.player.rotation.vy, Q12(-1.0f));

    #undef POS_OFFSET
    #undef RADIUS_MIN
}

void vcSetRefPosAndCamPosAngByPad(VECTOR3* ref_pos, s_SysWork* sys_p) // 0x800406D4
{
    #define MOVE_DIST Q8(0.1f)

    SVECTOR cam_ang;   // Q3.12
    VECTOR3 newCamPos; // Q23.8
    VECTOR3 cam_pos;   // Q19.12
    MATRIX  lookAtMat;
    q23_8   moveStep;

    vwGetViewPosition(&cam_pos);

    newCamPos.vx = Q12_TO_Q8(cam_pos.vx);
    newCamPos.vy = Q12_TO_Q8(cam_pos.vy);
    newCamPos.vz = Q12_TO_Q8(cam_pos.vz);

    vwGetViewAngle(&cam_ang);

    if (!(g_Controller1->buttonFlags.held & ControllerFlag_Circle))
    {
        if (g_Controller1->buttonFlags.held & ControllerFlag_LStickHighDown)
        {
            cam_ang.vx = cam_ang.vx - (g_VBlanks * V_BLANKS_MULT);
        }

        if (g_Controller1->buttonFlags.held & ControllerFlag_LStickHighUp)
        {
            cam_ang.vx = cam_ang.vx + (g_VBlanks * V_BLANKS_MULT);
        }

        if (g_Controller1->buttonFlags.held & ControllerFlag_LStickHighRight)
        {
            cam_ang.vy = cam_ang.vy + (g_VBlanks * V_BLANKS_MULT);
        }

        if (g_Controller1->buttonFlags.held & ControllerFlag_LStickHighLeft)
        {
            cam_ang.vy = cam_ang.vy - (g_VBlanks * V_BLANKS_MULT);
        }

        if (g_Controller1->buttonFlags.held & (ControllerFlag_Triangle | ControllerFlag_Cross))
        {
            moveStep = Q8(0.0f);
            if (g_Controller1->buttonFlags.held & ControllerFlag_Triangle)
            {
                moveStep = MOVE_DIST;
            }
            if (g_Controller1->buttonFlags.held & ControllerFlag_Cross)
            {
                moveStep = -MOVE_DIST - 1; // TODO: `- 1` enforces a rounded down result, but `Q8` truncates toward 0.
            }

            // @bug? Misuse of Q12 multiplication on Q8 values.
            newCamPos.vx += Q12_MULT_ALT(moveStep, Math_Sin(cam_ang.vy));
            newCamPos.vz += Q12_MULT_ALT(moveStep, Math_Cos(cam_ang.vy));
        }
    }
    else
    {
        if (g_Controller1->buttonFlags.held & ControllerFlag_LStickHighUp)
        {
            newCamPos.vy -= MOVE_DIST;
        }
        if (g_Controller1->buttonFlags.held & ControllerFlag_LStickHighDown)
        {
            newCamPos.vy += MOVE_DIST;
        }

        if (g_Controller1->buttonFlags.held & (ControllerFlag_LStickHighRight | ControllerFlag_LStickHighLeft))
        {
            moveStep = Q8(0.0f);
            if (g_Controller1->buttonFlags.held & ControllerFlag_LStickHighRight)
            {
                moveStep = MOVE_DIST;
            }
            if (g_Controller1->buttonFlags.held & ControllerFlag_LStickHighLeft)
            {
                moveStep = -MOVE_DIST - 1; // TODO: `- 1` enforces a rounded down result, but `Q8` truncates toward 0.
            }

            // @bug? Misuse of Q12 multiplication on Q8 values.
            newCamPos.vx += Q12_MULT_ALT(moveStep, Math_Sin(cam_ang.vy + Q12_ANGLE(90.0f)));
            newCamPos.vz += Q12_MULT_ALT(moveStep, Math_Cos(cam_ang.vy + Q12_ANGLE(90.0f)));
        }
    }

    Math_RotMatrixZxyNegGte(&cam_ang, &lookAtMat);

    // Set look-at matrix.
    lookAtMat.t[0] = newCamPos.vx;
    lookAtMat.t[1] = newCamPos.vy;
    lookAtMat.t[2] = newCamPos.vz;
    vwSetViewInfoDirectMatrix(NULL, &lookAtMat);

    if (g_Controller1->buttonFlags.held & (ControllerFlag_LStickHighUp    |
                                     ControllerFlag_LStickHighRight |
                                     ControllerFlag_LStickHighDown  |
                                     ControllerFlag_LStickHighLeft  |
                                     ControllerFlag_Cross       |
                                     ControllerFlag_Triangle))
    {
        SVECTOR refOffset; // Q19.12?

        // @bug? `vwAngleToVector` expects `Q12`. Possible typo.
        vwAngleToVector(&refOffset, &cam_ang, Q8(5.0f));

        ref_pos->vx = Q8_TO_Q12(newCamPos.vx + refOffset.vx);
        ref_pos->vy = Q8_TO_Q12(newCamPos.vy + refOffset.vy);
        ref_pos->vz = Q8_TO_Q12(newCamPos.vz + refOffset.vz);

        sys_p->cameraAngleY   = Math_AngleNormalize(cam_ang.vy + Q12_ANGLE(180.0f));
        sys_p->cameraY        = Q8_TO_Q12(-refOffset.vy);
        sys_p->cameraRadiusXz = Q8_TO_Q12(Math_Vector2MagCalc(refOffset.vx, refOffset.vz));
    }

    #undef MOVE_DIST
}

s8 Vc_StereoBalanceGet(const VECTOR3* soundPos) // 0x80040A64
{
    VECTOR3 camPos; // Q19.12
    VECTOR  offset; // Q25.6
    VECTOR  dir;    // Q19.12
    MATRIX  viewMat;
    s32     dot;
    s32     balance;

    // If monoural sound type, default to balance of 0.
    if (g_GameWork.config.soundType)
    {
        return 0;
    }

    // Compute direction from camera to sound.
    vwGetViewPosition(&camPos);
    offset.vx = Q12_TO_Q6(soundPos->vx - camPos.vx);
    offset.vy = Q12_TO_Q6(soundPos->vy - camPos.vy);
    offset.vz = Q12_TO_Q6(soundPos->vz - camPos.vz);
    VectorNormal(&offset, &dir);

    // Compute stereo balance.
    Vw_CoordHierarchyMatrixCompute(vwGetViewCoord(), &viewMat);
    dot     = Math_MultiplyMatrix(viewMat, dir);
    balance = CLAMP(dot, -127, 127);
    return balance;
}

void nullsub_80040B6C(void) {} // 0x80040B6C

#undef V_BLANKS_MULT
