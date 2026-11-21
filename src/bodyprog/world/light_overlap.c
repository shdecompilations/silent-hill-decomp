#include "bodyprog/game.h"
#include "inline_no_dmpsx.h"

#include <psyq/strings.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/world/light_overlap.h"
#include "bodyprog/chara/chara.h"
#include "bodyprog/screen/data.h"
#include "bodyprog/screen/draw.h"
#include "bodyprog/math/math.h"
#include "main/fs_queue.h"

/** @note This code (except the first function) is used to handle an overlapping light.
 *
 * This system is used only at one specific moment in the game. The
 * only time this system is active is when an environment preset from 
 * `WORLD_ENV_PRESETS` has `enableTintLightOverlap` enabled, or when both 
 * `pointLightTint` and `worldTint` have defined values. The code does not 
 * redefine these variables again.
 *
 * This system replaces or overlaps the ambient and volumetric 
 * lighting of the flashlight, but does not affect the color 
 * of the light applied to surfaces.
 */

bool WorldGfx_CharaModelAssignedCheck(e_CharaId charaId) // 0x80040B74
{
    s32 i;

    // Run through character models.
    for (i = 0; i < ARRAY_SIZE(g_WorldGfxWork.charaModels); i++)
    {
        if (g_WorldGfxWork.charaModels[i].charaId == charaId)
        {
            return true;
        }
    }

    return false;
}

static s_LightOverlapPrims g_LightOverlapPrims[2];

void WorldEnvRender_LightOverlapInit(void) // 0x80040BAC
{
    DVECTOR              edgePoints[17];
    POLY_G4*             ring;
    POLY_F4*             edge;
    s_LightOverlapPrims* prims;
    POLY_G3*             fan;
    s32                  i;
    s32                  ringIdx;
    s32                  bufferIdx;
    s32*                 edgeXy;

    // Screen border points starting at the right edge midpoint and running in the same direction as the ring
    // points, so edge quad N joins ring point N to border point N. Point 16 closes the loop.
    for (i = 0; i < ARRAY_SIZE(edgePoints); i++)
    {
        if (i < 2)
        {
            edgePoints[i].vx = g_GameWork.gsScreenWidth / 2;
            edgePoints[i].vy = (g_GameWork.gsScreenHeight / 4) * i;
        }
        else if (i < 6)
        {
            edgePoints[i].vx = (g_GameWork.gsScreenWidth / 2) - (((g_GameWork.gsScreenWidth / 2) >> 1) * (i - 2));
            edgePoints[i].vy = g_GameWork.gsScreenHeight / 2;
        }
        else if (i < 10)
        {
            edgePoints[i].vx = -g_GameWork.gsScreenWidth / 2;
            edgePoints[i].vy = (g_GameWork.gsScreenHeight / 2) - (((g_GameWork.gsScreenHeight / 2) >> 1) * (i - 6));
        }
        else if (i < 14)
        {
            edgePoints[i].vx = (-g_GameWork.gsScreenWidth / 2) + ((g_GameWork.gsScreenWidth / 4) * (i - 10));
            edgePoints[i].vy = -g_GameWork.gsScreenHeight / 2;
        }
        else
        {
            edgePoints[i].vx = g_GameWork.gsScreenWidth / 2;
            edgePoints[i].vy = (-g_GameWork.gsScreenHeight / 2) + ((g_GameWork.gsScreenHeight / 4) * (i - 14));
        }
    }

    for (bufferIdx = 0, edgeXy = (s32*)edgePoints, prims = g_LightOverlapPrims; bufferIdx < 2; bufferIdx++, prims++)
    {
        SetDrawTPage(&prims->tpages[0], 1, 1, getTPageN(0, BlendMode_Subtractive, 0, 0));
        SetDrawTPage(&prims->tpages[1], 1, 1, getTPageN(0, BlendMode_Additive, 0, 0));

        fan  = prims->fan;
        edge = prims->edge;

        for (i = 0; i < 16; i++, fan++, edge++)
        {
            SetPolyG3(fan);
            setSemiTrans(fan, true);
            SetPolyF4(edge);
            setSemiTrans(edge, true);

            *(s32*)&edge->x2 = edgeXy[i % 16];
            *(s32*)&edge->x3 = edgeXy[(i % 16) + 1];
        }

        ring = prims->rings;

        for (ringIdx = 0; ringIdx < 3; ringIdx++)
        {
            for (i = 0; i < 16; i++, ring++)
            {
                SetPolyG4(ring);
                setSemiTrans(ring, true);
            }
        }
    }
}

void WorldEnvRender_WorldLightTintParamSet(u8 lightColorR, u8 lightColorG, u8 lightColorB,
                                           u8 worldTintR, u8 worldTintG, u8 worldTintB) // 0x80040E7C
{
    u32                  ringColors[4];
    s32                  i;
    s32                  bufferIdx;
    s32                  ringIdx;
    u32                  lightColor;
    POLY_G3*             fan;
    POLY_F4*             edge;
    POLY_G4*             ring;
    s_LightOverlapPrims* prims;

    lightColor = COLOR_RGB(lightColorR, lightColorG, lightColorB);

    ringColors[0] = COLOR_RGB(0, 0, 0);
    ringColors[1] = COLOR_RGB(worldTintR / 3, worldTintG / 3, worldTintB / 3);
    ringColors[2] = COLOR_RGB((worldTintR * 2) / 3, (worldTintG * 2) / 3, (worldTintB * 2) / 3);
    ringColors[3] = COLOR_RGB(worldTintR, worldTintG, worldTintB);

    for (bufferIdx = 0, prims = g_LightOverlapPrims; bufferIdx < 2; bufferIdx++, prims++)
    {
        fan  = prims->fan;
        edge = prims->edge;

        for (i = 0; i < 16; i++, fan++, edge++)
        {
            *(s32*)&fan->r0 = lightColor + (fan->code << 24);
            *(s32*)&fan->r1 = ringColors[0];
            *(s32*)&fan->r2 = ringColors[0];
            *(s32*)&edge->r0 = ringColors[3] + (edge->code << 24);
        }

        ring = prims->rings;

        for (ringIdx = 0; ringIdx < 3; ringIdx++)
        {
            for (i = 0; i < 16; i++, ring++)
            {
                *(s32*)&ring->r0 = ringColors[ringIdx] + (ring->code << 24);
                *(s32*)&ring->r1 = ringColors[ringIdx];
                *(s32*)&ring->r2 = ringColors[ringIdx + 1];
                *(s32*)&ring->r3 = ringColors[ringIdx + 1];
            }
        }
    }
}

void WorldEnvRender_LightOverlapDraw(GsOT* ot, q19_12 intensity, SVECTOR* world_light_vec, const VECTOR3* world_light_pos) // 0x80041074
{
    VECTOR3 scr_l_pos;
    q19_12  scr_l_ang_xy;
    q19_12  scr_l_ang_z;

    WorldEnvRender_LightScreenInfoGet(&scr_l_pos, &scr_l_ang_xy, &scr_l_ang_z, world_light_vec, world_light_pos);
    WorldEnvRender_LightOverlapPolysDraw(ot, &scr_l_pos, intensity, scr_l_ang_xy, scr_l_ang_z);
}

void WorldEnvRender_LightScreenInfoGet(VECTOR3* scr_l_pos, q19_12* scr_l_ang_xy, q19_12* scr_l_ang_z,
                                       SVECTOR* world_light_vec, const VECTOR3* world_light_pos) // 0x800410D8
{
    MATRIX        transformMat;
    SVECTOR       vec0;
    GsCOORDINATE2 coord;
    VECTOR        ws_l_pos; // Q19.12
    VECTOR        ws_l_vec; // Q19.12
    s32           flag;

    memset(&vec0, 0, sizeof(SVECTOR));

    // Compute coord.
    coord.super      = NULL;
    coord.workm      = GsIDMATRIX;
    coord.workm.t[0] = Q12_TO_Q8(world_light_pos->vx);
    coord.workm.t[1] = Q12_TO_Q8(world_light_pos->vy);
    coord.workm.t[2] = Q12_TO_Q8(world_light_pos->vz);
    coord.flg        = true;

    Vw_CoordToViewSpaceMatrix(&coord, &transformMat);
    SetRotMatrix(&transformMat);
    SetTransMatrix(&transformMat);
    RotTrans(&vec0, &ws_l_pos, (long*)&flag);

    ApplyRotMatrix(world_light_vec, &ws_l_vec);

    WorldEnvRender_LightScreenAngleGet(scr_l_ang_xy, scr_l_ang_z, &ws_l_pos, &ws_l_vec);
    WorldEnvRender_LightScreenPositionGet(scr_l_pos, &ws_l_pos, &ws_l_vec, ReadGeomScreen());
}

void WorldEnvRender_LightScreenAngleGet(q19_12* scr_l_ang_xy, q19_12* scr_l_ang_z,
                                        const VECTOR* ws_l_sxyz_p, const VECTOR* ws_l_vec_p) // 0x8004122C
{
    VECTOR  one_pos;    // Q19.12
    VECTOR  one_l_vec;  // Q19.12
    VECTOR  one_op_vec; // Q19.12
    SVECTOR cos_z;
    SVECTOR sin_z;
    VECTOR  vec;        // Q19.12

    // Normal of plane containing eye-to-light ray and light direction.
    VectorNormal(ws_l_sxyz_p, &one_pos);
    VectorNormal(ws_l_vec_p, &one_l_vec);
    OuterProduct12(&one_pos, &one_l_vec, &one_op_vec);
    VectorNormal(&one_op_vec, &one_op_vec);

    // Screen-space direction of that plane.
    *scr_l_ang_xy = Q12_ANGLE_NORM_U(ratan2(one_op_vec.vy, one_op_vec.vx) - Q12_ANGLE(90.0f));

    // Angle between eye-to-light ray and light direction, measured within that plane.
    cos_z.vx = FP_FROM((one_pos.vx * one_l_vec.vx) +
                       (one_pos.vy * one_l_vec.vy) +
                       (one_pos.vz * one_l_vec.vz), Q12_SHIFT);
    OuterProduct12(&one_op_vec, &one_pos, &vec);
    sin_z.vx = FP_FROM((one_l_vec.vx * vec.vx) +
                       (one_l_vec.vy * vec.vy) +
                       (one_l_vec.vz * vec.vz), Q12_SHIFT);

    *scr_l_ang_z = Q12_ANGLE_NORM_U(ratan2(sin_z.vx, cos_z.vx));
}

void WorldEnvRender_LightScreenPositionGet(VECTOR3* scr_l_pos,
                                           const VECTOR* ws_l_sxyz_p, const VECTOR* ws_l_vec_p, s32 scr_z) // 0x8004137C
{
    VECTOR tmp_pos;
    s32    geom_x;
    s32    geom_y;
    s32    scr_z_half;
    s32    tmp_vec_z;

    scr_z_half = scr_z / 2;

    if (scr_z_half < ws_l_sxyz_p->vz)
    {
        tmp_pos = *ws_l_sxyz_p;
    }
    else
    {
        tmp_vec_z = 1;
        if (ws_l_vec_p->vz != 0)
        {
            tmp_vec_z = ws_l_vec_p->vz;
        }

        tmp_pos.vz = scr_z_half;
        tmp_pos.vx = (((scr_z_half - ws_l_sxyz_p->vz) * ws_l_vec_p->vx) / tmp_vec_z) + ws_l_sxyz_p->vx;
        tmp_pos.vy = (((scr_z_half - ws_l_sxyz_p->vz) * ws_l_vec_p->vy) / tmp_vec_z) + ws_l_sxyz_p->vy;
    }

    ReadGeomOffset((long*)&geom_x, (long*)&geom_y);

    scr_l_pos->vz = tmp_pos.vz;
    scr_l_pos->vx = ((tmp_pos.vx * scr_z) / tmp_pos.vz) + geom_x;
    scr_l_pos->vy = ((tmp_pos.vy * scr_z) / tmp_pos.vz) + geom_y;
}

void WorldEnvRender_LightOverlapPolysDraw(GsOT* ot, const VECTOR3* scr_l_pos, q19_12 intensity,
                                          q19_12 scr_l_ang_xy, q19_12 scr_l_ang_z) // 0x800414E0
{
    s32      ringRadii[4];
    DVECTOR  ringCenters[4];
    s32      angZSin;
    s32      angXyCos;
    s32      angXySin;
    s32      radius;
    s32      depthScale;
    s32      i;
    s32      j;
    u32*     ringXy;
    DVECTOR* ringPoints;
    POLY_G3* fan;
    POLY_G4* ring;
    POLY_F4* edge;

    if (scr_l_pos->vz < Q12(0.25f))
    {
        depthScale = Q12(1.0f);
    }
    else
    {
        depthScale = Q12(1024.0f) / scr_l_pos->vz;
    }

    // Halves in size as the light turns away from the camera, `depthScale` fades that out with distance.
    radius = (Q12_MULT_ALT(intensity, 500) << 10) / scr_l_pos->vz;
    radius = (radius * (Q12(3.0f) - Q12_MULT(depthScale, Math_Cos(scr_l_ang_z)))) / Q12(4.0f);

    ringRadii[0] = (radius * 2) / 10;
    ringRadii[1] = (radius * 4) / 10;
    ringRadii[2] = (radius * 7) / 10;
    ringRadii[3] = radius;

    angZSin  = Math_Sin(scr_l_ang_z);
    angXyCos = Math_Cos(scr_l_ang_xy);
    angXySin = Math_Sin(scr_l_ang_xy);

    // Shift each ring along the light's screen direction by `sin(angle z)`
    // so the spot stretches when the beam is seen side-on.
    for (i = 0; i < ARRAY_SIZE(ringRadii); i++)
    {
        s32 centerScale;

        centerScale       = Q12(radius - (ringRadii[i] >> 1)) / radius;
        ringCenters[i].vx = scr_l_pos->vx + Q12_MULT_ALT(Q12_MULT_ALT(Q12_MULT_ALT(ringRadii[i], centerScale), angXyCos), angZSin);
        ringCenters[i].vy = scr_l_pos->vy + Q12_MULT_ALT(Q12_MULT_ALT(Q12_MULT_ALT(ringRadii[i], centerScale), angXySin), angZSin);
    }

    // 4 rings of 17 points in scratchpad. Point 16 repeats point 0 to close the ring.
    ringPoints = PSX_SCRATCH;
    for (i = 0; i < ARRAY_SIZE(ringRadii); i++)
    {
        DVECTOR* point;

        point = ringPoints + (i * 17);
        for (j = 0; j < 17; j++, point++)
        {
            s32 pointCos;
            s32 pointSin;

            pointCos = Math_Cos(j * Q12_ANGLE(22.5f));
            pointSin = Math_Sin(j * Q12_ANGLE(22.5f));

            // Clamped to the GPU's 11-bit signed vertex range.
            point->vx = Q12_MULT_ALT(ringRadii[i], pointCos) + ringCenters[i].vx;
            point->vx = CLAMP(point->vx, -1024, 1023);

            point->vy = Q12_MULT_ALT(ringRadii[i], pointSin) + ringCenters[i].vy;
            point->vy = CLAMP(point->vy, -1024, 1023);
        }
    }

    ringXy = (u32*)PSX_SCRATCH;

    fan  = g_LightOverlapPrims[g_ActiveBufferIdx].fan;
    edge = g_LightOverlapPrims[g_ActiveBufferIdx].edge;

    for (j = 0; j < 16; j++, fan++, edge++)
    {
        fan->x0         = scr_l_pos->vx;
        fan->y0         = scr_l_pos->vy;
        *(s32*)&fan->x1 = ringXy[j];
        *(s32*)&fan->x2 = ringXy[j + 1];

        addPrim(&ot->org[0], fan);

        *(s32*)&edge->x0 = ringXy[(3 * 17) + j];
        *(s32*)&edge->x1 = ringXy[(3 * 17) + j + 1];

        addPrim(&ot->org[1], edge);
    }

    ring = g_LightOverlapPrims[g_ActiveBufferIdx].rings;

    for (i = 0; i < 3; i++)
    {
        u32* rowXy;

        rowXy = ringXy + (i * 17);

        for (j = 0; j < 16; j++, ring++)
        {
            *(s32*)&ring->x0 = rowXy[j];
            *(s32*)&ring->x1 = rowXy[j + 1];
            *(s32*)&ring->x2 = rowXy[17 + j];
            *(s32*)&ring->x3 = rowXy[(17 + j) + 1];

            addPrim(&ot->org[1], ring);
        }
    }

    AddPrim(&ot->org[0], &g_LightOverlapPrims[g_ActiveBufferIdx].tpages[1]);
    AddPrim(&ot->org[1], &g_LightOverlapPrims[g_ActiveBufferIdx].tpages[0]);
}
