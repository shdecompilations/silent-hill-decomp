#include "bodyprog/game.h"

#include "bodyprog/view/vw_main.h"
#include "bodyprog/view/vw_system.h"
#include "bodyprog/math/math.h"

VW_VIEW_WORK vwViewPointInfo;

void vwInitViewInfo(void) // 0x80048A38
{
    vwViewPointInfo.rview.vp.vz = Q12(0.0f);
    vwViewPointInfo.rview.vp.vy = Q12(0.0f);
    vwViewPointInfo.rview.vp.vx = Q12(0.0f);
    vwViewPointInfo.rview.vr.vx = Q12(0.0f);
    vwViewPointInfo.rview.vr.vy = Q12(0.0f);
    vwViewPointInfo.rview.vr.vz = Q12(1.0f);
    vwViewPointInfo.rview.rz    = Q12(0.0f);
    vwViewPointInfo.rview.super = &vwViewPointInfo.vwcoord;

    GsInitCoordinate2(NULL, &vwViewPointInfo.vwcoord);
    vwSetViewInfo();
}

GsCOORDINATE2* vwGetViewCoord(void) // 0x80048A90
{
    return &vwViewPointInfo.vwcoord;
}

void vwGetViewPosition(VECTOR3* pos) // 0x80048A9C
{
    *pos = vwViewPointInfo.worldpos;
}

void vwGetViewAngle(SVECTOR* ang) // 0x80048AC4
{
    *ang = vwViewPointInfo.worldang;
}

void Vw_SetLookAtMatrix(const VECTOR3* pos, const VECTOR3* lookAt) // 0x80048AF4
{
    q23_8   deltaX;
    q23_8   deltaY;
    q23_8   deltaZ;
    MATRIX  viewMat;
    SVECTOR rot; // Q3.12

    // Compute direction vector components.
    deltaX = Q12_TO_Q8(lookAt->vx - pos->vx);
    deltaY = Q12_TO_Q8(lookAt->vy - pos->vy);
    deltaZ = Q12_TO_Q8(lookAt->vz - pos->vz);

    // Compute camera rotation.
    rot.vz = Q12_ANGLE(0.0f);
    rot.vy = ratan2(deltaX, deltaZ);
    rot.vx = ratan2(-deltaY, Math_Vector2MagCalc(deltaX, deltaZ));

    // Compute view transform matrix and set global info.
    Math_RotMatrixZxyNeg(&rot, &viewMat);
    viewMat.t[0] = Q12_TO_Q8(pos->vx);
    viewMat.t[1] = Q12_TO_Q8(pos->vy);
    viewMat.t[2] = Q12_TO_Q8(pos->vz);
    vwSetViewInfoDirectMatrix(NULL, &viewMat);
}

void vwSetCoordRefAndEntou(GsCOORDINATE2* parent_p,
                           q19_12 ref_x, q19_12 ref_y, q19_12 ref_z,
                           q3_12 cam_ang_y, q3_12 cam_ang_z, q19_12 cam_y, q19_12 cam_xz_r) // 0x80048BE0
{
    SVECTOR view_ang; // Q3.12
    MATRIX* view_mtx;

    view_mtx = &vwViewPointInfo.vwcoord.coord;

    // Setup.
    vwViewPointInfo.vwcoord.flg   = false;
    vwViewPointInfo.vwcoord.super = parent_p;

    // Compute look-at rotation.
    view_ang.vy = cam_ang_y;
    view_ang.vz = cam_ang_z;
    view_ang.vx = -ratan2(-cam_y, cam_xz_r);
    view_ang.vy = Q12_ANGLE_NORM_U(view_ang.vy + Q12_ANGLE(180.0f));

    // Compute view matrix.
    Math_RotMatrixZxyNegGte(&view_ang, view_mtx);
    view_mtx->t[0] = Q12_TO_Q8(ref_x) + Q12_MULT(Q12_TO_Q8(cam_xz_r), Math_Sin(cam_ang_y));
    view_mtx->t[1] = Q12_TO_Q8(ref_y) + Q12_TO_Q8(cam_y);
    view_mtx->t[2] = Q12_TO_Q8(ref_z) + Q12_MULT(Q12_TO_Q8(cam_xz_r), Math_Cos(cam_ang_y));
}

void vwSetViewInfoDirectMatrix(GsCOORDINATE2* pcoord, const MATRIX* cammat) // 0x80048CF0
{
    vwViewPointInfo.vwcoord.flg   = false;
    vwViewPointInfo.vwcoord.super = pcoord;
    vwViewPointInfo.vwcoord.coord = *cammat;
}

/** @brief Extracts a position from a matrix, outputting the result to `pos`.
 *
 * Possible original name: `vwMatrixToPosition`.
 *
 * @param `pos` Output position (Q19.12).
 * @param `mat` Matrix to use.
 */
static inline void Math_MatrixToPosition(VECTOR3* pos, MATRIX* mat)
{
    pos->vx = Q8_TO_Q12(mat->t[0]);
    pos->vy = Q8_TO_Q12(mat->t[1]);
    pos->vz = Q8_TO_Q12(mat->t[2]);
}

void vwSetViewInfo(void) // 0x80048D48
{
    vbSetRefView(&vwViewPointInfo.rview);
    Math_MatrixToPosition(&vwViewPointInfo.worldpos, &vwViewPointInfo.vwcoord.workm);
    vwMatrixToAngleYXZ(&vwViewPointInfo.worldang, &vwViewPointInfo.vwcoord.workm);
}

void Vw_ClampAngleRange(q3_12* angleMin, q3_12* angleMax, q3_12 angleConstraintMin, q3_12 angleConstraintMax) // 0x80048DA8
{
    q19_12 prevAngleMax;
    q19_12 prevAngleMin;
    q19_12 rotToAngleMax;
    q3_12  rotToAngleConstraintMin;
    q3_12  rotToAngleConstraintMax;

    prevAngleMax = *angleMax;
    prevAngleMin = *angleMin;

    rotToAngleMax = prevAngleMax;
    rotToAngleMax = Q12_FRACT(prevAngleMax - prevAngleMin);

    rotToAngleConstraintMin = Q12_FRACT(angleConstraintMin - prevAngleMin);
    rotToAngleConstraintMax = Q12_FRACT(angleConstraintMax - prevAngleMin);

    prevAngleMax = Q8_ANGLE(0.0f);
    if (rotToAngleConstraintMin <= rotToAngleConstraintMax)
    {
        if (rotToAngleConstraintMin > prevAngleMax)
        {
            prevAngleMax = rotToAngleConstraintMin;
        }

        if (rotToAngleConstraintMax < rotToAngleMax)
        {
            rotToAngleMax = rotToAngleConstraintMax;
        }

        if (rotToAngleMax < prevAngleMax)
        {
            prevAngleMax = rotToAngleMax;
        }
    }
    else if (rotToAngleConstraintMax > prevAngleMax)
    {
        if (rotToAngleConstraintMax < rotToAngleMax)
        {
            rotToAngleMax = rotToAngleConstraintMax;
        }
    }

    *angleMin = Q12_FRACT(prevAngleMax + prevAngleMin);
    *angleMax = Q12_FRACT(rotToAngleMax + prevAngleMin);
}

q3_12 Vw_LineSegmentIntersectionCheck(s16 segmentLength, s16 segmentDir,
                                      s16 distToBound, q3_12 boundsMin, q3_12 boundsMax) // 0x80048E3C
{
    s32   temp_a0;
    q3_12 temp_lo;

    if (segmentDir > 0)
    {
        if (distToBound < 0)
        {
            return Q12(1.0f);
        }
    }
    else if (segmentDir >= 0 || distToBound > 0)
    {
        return Q12(1.0f);
    }

    if (distToBound == 0)
    {
        if (boundsMin <= 0 && boundsMax >= 0)
        {
            return Q12(0.0f);
        }

        return Q12(1.0f);
    }

    if (ABS(distToBound) > ABS(segmentDir))
    {
        return Q12(1.0f);
    }
    else
    {
        temp_lo = FP_FROM(distToBound << 16, Q4_SHIFT) / segmentDir;
        temp_a0 = Q12_MULT(segmentLength, temp_lo);

        if (temp_a0 < boundsMin || boundsMax < temp_a0)
        {
            return Q12(1.0f);
        }
        else
        {
            return temp_lo;
        }
    }
}
