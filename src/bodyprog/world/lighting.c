#include "bodyprog/game.h"
#include "inline_no_dmpsx.h"

#include <psyq/strings.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/screen/data.h"
#include "bodyprog/screen/draw.h"
#include "bodyprog/math/math.h"
#include "main/fsqueue.h"

/** @note This code (except the first and possibly second functions) is used to handle an overlapping lighting system.
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
 *
 * The second function is used as an initialization process for 2D graphics, likely related to this same system.
 */

// ========================================
// CHARACTERS
// ========================================

bool Chara_ModelLoadedCheck(e_CharaId charaId) // 0x80040B74
{
    s32 i;

    for (i = 0; i < ARRAY_SIZE(g_WorldGfxWork.charaModels); i++)
    {
        if (g_WorldGfxWork.charaModels[i].charaId == charaId)
        {
            return true;
        }
    }

    return false;
}

// ========================================
// WORLD RENDERING
// ========================================

static PACKET g_Map_GfxPackets[2][0xA10];

void func_80040BAC(void) // 0x80040BAC
{
    DVECTOR   posTable[17];
    POLY_G4*  poly_g4;
    POLY_F4*  poly_f4;
    PACKET*   packet;
    POLY_G3*  poly_g3;
    DR_TPAGE* page;
    s32       i;
    s32       k;
    s32       j;
    s32*      ptr;

    for (i = 0; i < ARRAY_SIZE(posTable); i++)
    {
        if (i < 2)
        {
            posTable[i].vx = g_GameWork.gsScreenWidth / 2;
            posTable[i].vy = (g_GameWork.gsScreenHeight / 4) * i;
        }
        else if (i < 6)
        {
            posTable[i].vx = (g_GameWork.gsScreenWidth / 2) - (((g_GameWork.gsScreenWidth / 2) >> 1) * (i - 2));
            posTable[i].vy = g_GameWork.gsScreenHeight / 2;
        }
        else if (i < 10)
        {
            posTable[i].vx = -g_GameWork.gsScreenWidth / 2;
            posTable[i].vy = (g_GameWork.gsScreenHeight / 2) - (((g_GameWork.gsScreenHeight / 2) >> 1) * (i - 6));
        }
        else if (i < 14)
        {
            posTable[i].vx = (-g_GameWork.gsScreenWidth / 2) + ((g_GameWork.gsScreenWidth / 4) * (i - 10));
            posTable[i].vy = -g_GameWork.gsScreenHeight / 2;
        }
        else
        {
            posTable[i].vx = g_GameWork.gsScreenWidth / 2;
            posTable[i].vy = (-g_GameWork.gsScreenHeight / 2) + ((g_GameWork.gsScreenHeight / 4) * (i - 14));
        }
    }

    for (j = 0, ptr = &posTable[0], packet = &g_Map_GfxPackets; j < 2; j++,
        packet += (sizeof(DR_TPAGE) * 2)       +
                  ((sizeof(POLY_G4) * 16) * 3) +
                  (sizeof(POLY_G3) * 16)       +
                  (sizeof(POLY_F4) * 16))
    {
        page = (DR_TPAGE*)packet;

        SetDrawTPage(page, 1, 1, 0x40);
        page++;
        SetDrawTPage(page, 1, 1, 0x20);

        poly_g3 = packet + sizeof(DR_TPAGE) * 2;
        poly_f4 = packet + (sizeof(DR_TPAGE) * 2)       +
                           ((sizeof(POLY_G4) * 16) * 3) +
                           (sizeof(POLY_G3) * 16);

        for (i = 0; i < 16; i++, poly_g3++, poly_f4++)
        {
            SetPolyG3(poly_g3);
            setSemiTrans(poly_g3, true);
            SetPolyF4(poly_f4);
            setSemiTrans(poly_f4, true);

            *(s32*)&poly_f4->x2 = ptr[i % 16];
            *(s32*)&poly_f4->x3 = ptr[(i % 16) + 1];
        }

        poly_g4 = packet + (sizeof(DR_TPAGE) * 2) +
                           (sizeof(POLY_G3) * 16);

        for (k = 0; k < 3; k++)
        {
            for (i = 0; i < 16; i++, poly_g4++)
            {
                SetPolyG4(poly_g4);
                setSemiTrans(poly_g4, true);
            }
        }
    }
}

void WorldEnvRender_WorldLightTintParamSet(u8 lightPosR, u8 lightPosG, u8 lightPosB,
                                           u8 worldTintR, u8 worldTintG, u8 worldTintB) // 0x80040E7C
{
    u32      colors[4];
    s32      j;
    s32      i;
    s32      k;
    u32      lightPointColor;
    POLY_G3* poly_g3;
    POLY_F4* poly_f4;
    POLY_G4* poly_g4;
    PACKET*  packet;

    lightPointColor = COLOR_RGB(lightPosR, lightPosG, lightPosB);

    packet = &g_Map_GfxPackets;

    colors[0] = COLOR_RGB(0, 0, 0);
    colors[1] = COLOR_RGB(worldTintR / 3, worldTintG / 3, worldTintB / 3);
    colors[2] = COLOR_RGB((worldTintR * 2) / 3, (worldTintG * 2) / 3, (worldTintB * 2) / 3);
    colors[3] = COLOR_RGB(worldTintR, worldTintG, worldTintB);

    for (i = 0; i < 2; i++,
        packet += (sizeof(DR_TPAGE) * 2)       +
                  ((sizeof(POLY_G4) * 16) * 3) +
                  (sizeof(POLY_G3) * 16)       +
                  (sizeof(POLY_F4) * 16))
    {
        poly_g3 = packet + (sizeof(DR_TPAGE) * 2);
        poly_f4 = packet + (sizeof(DR_TPAGE) * 2)       +
                           ((sizeof(POLY_G4) * 16) * 3) +
                           (sizeof(POLY_G3) * 16);

        for (j = 0; j < 16; j++, poly_g3++, poly_f4++)
        {
            *(s32*)&poly_g3->r0 = lightPointColor + (poly_g3->code << 24);
            *(s32*)&poly_g3->r1 = colors[0];
            *(s32*)&poly_g3->r2 = colors[0];
            *(s32*)&poly_f4->r0 = colors[3] + (poly_f4->code << 24);
        }

        poly_g4 = packet + (sizeof(DR_TPAGE) * 2) +
                           (sizeof(POLY_G3) * 16);

        for (k = 0; k < 3; k++)
        {
            for (j = 0; j < 16; j++, poly_g4++)
            {
                *(s32*)&poly_g4->r0 = colors[k] + (poly_g4->code << 24);
                *(s32*)&poly_g4->r1 = colors[k];
                *(s32*)&poly_g4->r2 = colors[k + 1];
                *(s32*)&poly_g4->r3 = colors[k + 1];
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

void WorldEnvRender_LightScreenInfoGet(VECTOR3* scr_l_pos, q19_12* scr_l_ang_xy, q19_12* scr_l_ang_z, SVECTOR* world_light_vec, const VECTOR3* world_light_pos) // 0x800410D8
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
    RotTrans(&vec0, &ws_l_pos, &flag);

    ApplyRotMatrix(world_light_vec, &ws_l_vec);

    WorldEnvRender_LightScreenAngleGet(scr_l_ang_xy, scr_l_ang_z, &ws_l_pos, &ws_l_vec);
    WorldEnvRender_LightScreenPosGet(scr_l_pos, &ws_l_pos, &ws_l_vec, ReadGeomScreen());
}

void WorldEnvRender_LightScreenAngleGet(q19_12* scr_l_ang_xy, q19_12* scr_l_ang_z, const VECTOR* ws_l_sxyz_p, const VECTOR* ws_l_vec_p) // 0x8004122C
{
    VECTOR  one_pos;        // Q19.12
    VECTOR  one_l_vec;        // Q19.12
    VECTOR  one_op_vec; // Q19.12
    SVECTOR cos_z;
    SVECTOR sin_z;
    VECTOR  vec;    // Q19.12

    // Normal of the plane containing the eye-to-light ray and the light direction.
    VectorNormal(ws_l_sxyz_p, &one_pos);
    VectorNormal(ws_l_vec_p, &one_l_vec);
    OuterProduct12(&one_pos, &one_l_vec, &one_op_vec);
    VectorNormal(&one_op_vec, &one_op_vec);

    // Screen-space direction of that plane.
    *scr_l_ang_xy = Q12_ANGLE_NORM_U(ratan2(one_op_vec.vy, one_op_vec.vx) - Q12_ANGLE(90.0f));

    // Angle between the eye-to-light ray and the light direction, measured within that plane.
    cos_z.vx = FP_FROM((one_pos.vx * one_l_vec.vx) +
                          (one_pos.vy * one_l_vec.vy) +
                          (one_pos.vz * one_l_vec.vz), Q12_SHIFT);
    OuterProduct12(&one_op_vec, &one_pos, &vec);
    sin_z.vx = FP_FROM((one_l_vec.vx * vec.vx) +
                          (one_l_vec.vy * vec.vy) +
                          (one_l_vec.vz * vec.vz), Q12_SHIFT);

    *scr_l_ang_z = Q12_ANGLE_NORM_U(ratan2(sin_z.vx, cos_z.vx));
}

void WorldEnvRender_LightScreenPosGet(VECTOR3* scr_l_pos, const VECTOR* ws_l_sxyz_p, const VECTOR* ws_l_vec_p, s32 scr_z) // 0x8004137C
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

    ReadGeomOffset(&geom_x, &geom_y);

    scr_l_pos->vz = tmp_pos.vz;
    scr_l_pos->vx = ((tmp_pos.vx * scr_z) / tmp_pos.vz) + geom_x;
    scr_l_pos->vy = ((tmp_pos.vy * scr_z) / tmp_pos.vz) + geom_y;
}

void WorldEnvRender_LightOverlapPolysDraw(GsOT* ot, const VECTOR3* scr_l_pos, q19_12 intensity, q19_12 scr_l_ang_xy, q19_12 scr_l_ang_z) // 0x800414E0
{
    s32      sp10[4];
    DVECTOR  sp20[4];
    s32      temp_a1;
    s32      temp_a3;
    s32      temp_lo;
    s32      temp_s0_3;
    s32      temp_s1;
    s32      temp_s2;
    s32      var_s0;
    s32      var_s1;
    s32      i;
    s32      j;
    s32      var_v1;
    DVECTOR* var_s1_2;
    u32*     var_a1_3;
    u32*     var_t0;
    u32*     var_t1;
    u32*     var_t4;
    POLY_G3* poly_g3;
    POLY_G4* poly_g4;
    POLY_F4* poly_f4;

    if (scr_l_pos->vz < Q12(0.25f))
    {
        var_s1 = Q12(1.0f);
    }
    else
    {
        var_s1 = Q12(1024.0f) / scr_l_pos->vz;
    }

    var_s0 = (Q12_MULT_ALT(intensity, 500) << 10) / scr_l_pos->vz;
    var_v1 = var_s0 * (Q12(3.0f) - Q12_MULT(var_s1, Math_Cos(scr_l_ang_z)));
    var_s0 = var_v1 / ((SHRT_MAX / 2) + 1);

    sp10[0] = (var_s0 / 5);
    sp10[1] = (var_s0 * 4) / 10;
    sp10[2] = (var_s0 * 7) / 10;
    sp10[3] = var_s0;

    temp_s1 = Math_Sin(scr_l_ang_z);
    temp_s2 = Math_Cos(scr_l_ang_xy);
    temp_a3 = Math_Sin(scr_l_ang_xy);

    for (i = 0; i < ARRAY_SIZE(sp10); i++)
    {
        temp_lo = Q12(var_s0 - (sp10[i] >> 1)) / var_s0;
        sp20[i].vx = scr_l_pos->vx + Q12_MULT_ALT(Q12_MULT_ALT(Q12_MULT_ALT(sp10[i], temp_lo), temp_s2), temp_s1);
        sp20[i].vy = scr_l_pos->vy + Q12_MULT_ALT(Q12_MULT_ALT(Q12_MULT_ALT(sp10[i], temp_lo), temp_a3), temp_s1);
    }

    var_t4 = PSX_SCRATCH;
    for (i = 0; i < 4; i++)
    {
        var_s1_2 = var_t4 + (i * 17);
        for (j = 0; j < 17; j++, var_s1_2++)
        {
            temp_s0_3 = Math_Cos(j << 8);
            temp_a1   = Math_Sin(j << 8);

            var_s1_2->vx = Q12_MULT_ALT(sp10[i], temp_s0_3) + sp20[i].vx;
            var_s1_2->vx = CLAMP(var_s1_2->vx, Q12(-0.25f), Q12(0.25f) - 1);

            var_s1_2->vy = Q12_MULT_ALT(sp10[i], temp_a1) + sp20[i].vy;
            var_s1_2->vy = CLAMP(var_s1_2->vy, Q12(-0.25f), Q12(0.25f) - 1);
        }
    }

    var_t0 = (u32*)PSX_SCRATCH;

    poly_g3 = &g_Map_GfxPackets[g_ActiveBufferIdx][sizeof(DR_TPAGE) * 2];
    poly_f4 = &g_Map_GfxPackets[g_ActiveBufferIdx][(sizeof(DR_TPAGE) * 2) +
                                             ((sizeof(POLY_G4) * 16) * 3) +
                                             (sizeof(POLY_G3) * 16)];

    for (j = 0; j < 16; j++, poly_g3++, poly_f4++)
    {
        poly_g3->x0         = scr_l_pos->vx;
        poly_g3->y0         = scr_l_pos->vy;
        *(s32*)&poly_g3->x1 = var_t0[j];
        *(s32*)&poly_g3->x2 = var_t0[j + 1];

        addPrim(ot->org, poly_g3);

        *(s32*)&poly_f4->x0 = var_t0[j + 51];
        *(s32*)&poly_f4->x1 = var_t0[j + 52];

        addPrim(&ot->org[1], poly_f4);
    }

    var_t1  = (u32*)PSX_SCRATCH;
    poly_g4 = &g_Map_GfxPackets[g_ActiveBufferIdx][(sizeof(DR_TPAGE) * 2) + (sizeof(POLY_G3) * 16)];

    for (i = 0; i < 3; i++)
    {
        var_a1_3 = var_t1 + (i * 17);

        for (j = 0; j < 16; j++, poly_g4++)
        {
            *(s32*)&poly_g4->x0 = var_a1_3[j];
            *(s32*)&poly_g4->x1 = var_a1_3[j + 1];
            *(s32*)&poly_g4->x2 = var_a1_3[17 + j];
            *(s32*)&poly_g4->x3 = var_a1_3[(17 + j) + 1];

            addPrim(&ot->org[1], poly_g4);
        }
    }

    AddPrim(ot->org, &g_Map_GfxPackets[g_ActiveBufferIdx][8]);
    AddPrim(&ot->org[1], &g_Map_GfxPackets[g_ActiveBufferIdx]);
}
