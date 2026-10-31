#include "game.h"
#include "inline_no_dmpsx.h"

#include <psyq/gtemac.h>
#include <psyq/libapi.h>
#include <psyq/strings.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "bodyprog/screen/screen_data.h"
#include "bodyprog/screen/screen_draw.h"
#include "bodyprog/item_screens.h"
#include "bodyprog/player.h"
#include "bodyprog/sound/sound_system.h"
#include "main/rng.h"

// ========================================
// ENVIRONMENT AND SCREEN GFX 2 (Drawing?)
// ========================================

extern s_WorldEnvWork g_WorldEnvWork;

void Gfx_FogOverlayQuadDraw(s16 x0, s16 y0, s16 x1, s16 y1, s32 arg4, s32 arg5, GsOT* ot, s32 otShift) // 0x80056D8C
{
    s16       var_a3;
    s16       var_a3_2;
    s16       var_v1;
    s16       var_v1_2;
    q23_8     temp_a0_2;
    q19_12    var_s0;
    s32       var_t1;
    q23_8     var_v0;
    q23_8     var_v1_3;
    s32       var_v1_4;
    POLY_G4*  poly;
    DR_MODE*  mode;
    PACKET*   packet;
    PACKET*   packet2;
    GsOT_TAG* tag;
    s16       temp_s6;
    s16       temp_s5;

    var_t1  = arg4;
    temp_s5 = y1;

    if (!g_WorldEnvWork.isFogEnabled)
    {
        return;
    }

    var_a3 = MAX(x0, ~(g_GameWork.gsScreenWidth  >> 1));
    var_v1 = MAX(y0, ~(g_GameWork.gsScreenHeight >> 1));

    var_v1_2 = CLAMP_HIGH(x1, (g_GameWork.gsScreenWidth >> 1) + 1);
    temp_s6  = var_v1_2;

    var_a3_2 = CLAMP_HIGH(temp_s5, (g_GameWork.gsScreenHeight >> 1) + 1);
    temp_s5  = var_a3_2;

    temp_a0_2 = 0x79C << (otShift + 2);

    if (g_WorldEnvWork.isFogEnabled)
    {
        var_v1_3 = MIN(temp_a0_2, g_WorldEnvWork.fog.farDistance);
    }
    else
    {
        var_v1_3 = temp_a0_2;
    }

    var_v0 = var_t1 >> 4;
    if (var_v0 < (var_v1_3 + Q8(3.0f)))
    {
        if (var_t1 < 0)
        {
            var_t1 = 0;
        }

        var_s0 = (func_80055A50(var_t1) * 16) + g_WorldEnvWork.fog.intensity;
        var_s0 = MIN(var_s0, Q12(1.0f));

        var_v1_4 = MAX(arg5 >> 7, 1);

        packet = GsOUT_PACKET_P;
        tag    = &ot->org[var_v1_4];

        SetPriority(packet, 0, 0);
        AddPrim(tag, packet);

        poly           = (POLY_G4*)(packet + 12);
        GsOUT_PACKET_P = (PACKET*)poly;

        // TODO: Use macro.
        *(u32*)&poly->r0 =
        *(u32*)&poly->r1 =
        *(u32*)&poly->r2 =
        *(u32*)&poly->r3 = Q12_MULT(g_WorldEnvWork.fog.color.r, var_s0)       +
                          (Q12_MULT(g_WorldEnvWork.fog.color.g, var_s0) << 8) +
                          (Q12_MULT(g_WorldEnvWork.fog.color.b, var_s0) << 16);

        SetPolyG4(poly);

        // TODO: Use macro.
        *(s32*)&poly->x0 = var_a3 + (var_v1 << 16);
        *(s32*)&poly->x1 = temp_s6 + (var_v1 << 16);
        *(s32*)&poly->x2 = var_a3 + (temp_s5 << 16);
        *(s32*)&poly->x3 = temp_s6 + (temp_s5 << 16);

        setSemiTrans(poly, true);
        AddPrim(tag, poly);

        mode           = (DR_MODE*)(packet + 48);
        GsOUT_PACKET_P = (PACKET*)mode;
        SetDrawMode(mode, 0, 1, 32, NULL);
        AddPrim(tag, mode);

        packet2        = packet + 60;
        GsOUT_PACKET_P = packet2;
        SetPriority(packet2, 1, 1);
        AddPrim(tag, packet2);

        GsOUT_PACKET_P = packet + 72;
    }
}

void func_80057090(s_ModelInfo* modelInfo, GsOT* arg1, s32 otShift, MATRIX* viewMat, MATRIX* worldMat, u16 clutY) // 0x80057090
{
    s32            temp_a0;
    GsOT_TAG*      otTag;
    s_ModelHeader* modelHdr;

    modelHdr = modelInfo->modelHdr;

    if (modelInfo->flags < ModelFlag_None)
    {
        return;
    }

    otTag = &arg1->org[func_800571D0(modelHdr->field_B_1)];
    temp_a0 = modelHdr->field_B_4;
    if ((temp_a0 & 0xFF) && temp_a0 >= 0 && temp_a0 < 4) // TODO: `& 0xFF` needed for match.
    {
        func_80059D50(temp_a0, modelInfo, viewMat, otShift, otTag);
    }
    else
    {
        if (worldMat != NULL && g_WorldEnvWork.field_0 != UnkGfxEnum_0)
        {
            WorldEnv_LightTransform(worldMat, g_WorldEnvWork.light.intensity, &g_WorldEnvWork.light.direction, &g_WorldEnvWork.light.position);
        }

        if (modelHdr->field_B_0)
        {
            g_WorldEnvWork.clutY = clutY;
            func_8005A21C(modelInfo, otTag, otShift, viewMat);
        }
        else
        {
            func_80057344(modelInfo, otTag, otShift, viewMat);
        }
    }
}

s32 func_800571D0(u32 arg0) // 0x800571D0
{
    switch (arg0)
    {
        default:
        case 0:
            return 2;

        case 1:
            return 0;

        case 2:
            return 4;

        case 3:
            return 33;

        case 4:
            return 66;

        case 5:
            return 99;
    }
}

void WorldEnv_LightTransform(MATRIX* worldMat, q19_12 alpha, SVECTOR* arg2, VECTOR3* arg3) // 0x80057228
{
    q23_8 posX;
    q23_8 posY;
    q23_8 posZ;

    gte_SetRotMatrix_custom(worldMat);

    gte_ldv0(arg2);
    gte_rtv0();
    gte_lddp(alpha);
    gte_gpf12();
    gte_stsv(&g_WorldEnvWork.light.field_28);

    // Divide `arg3` by 16 and subtract matrix translation.
    posX = Q12_TO_Q8(arg3->vx) - worldMat->t[0];
    posY = Q12_TO_Q8(arg3->vy) - worldMat->t[1];
    posZ = Q12_TO_Q8(arg3->vz) - worldMat->t[2];

    gte_LoadVector0_XYZ(posX, posY, posZ);
    gte_rtv0();
    gte_stsv(&g_WorldEnvWork.light.field_30);
}

void func_80057344(s_ModelInfo* modelInfo, GsOT_TAG* otTag, s32 otShift, MATRIX* mat) // 0x80057344
{
    u32               normalOffset;
    u32               vertOffset;
    s_MeshHeader*     curMeshHdr;
    s_ModelHeader*    modelHdr;
    s_GteScratchData* scratchData;

    scratchData = PSX_SCRATCH_ADDR(0);

    modelHdr     = modelInfo->modelHdr;
    vertOffset   = modelHdr->vertexOffset;
    normalOffset = modelHdr->normalOffset;

    gte_lddqa(g_WorldEnvWork.light.field_0);
    gte_lddqb_0();

    // Run through meshes.
    for (curMeshHdr = modelHdr->meshHdrs; curMeshHdr < &modelHdr->meshHdrs[modelHdr->meshCount]; curMeshHdr++)
    {
        if (vertOffset != 0 || normalOffset != 0)
        {
            func_8005759C(curMeshHdr, scratchData, vertOffset, normalOffset);
        }
        else
        {
            func_800574D4(curMeshHdr, scratchData);
        }

        switch (g_WorldEnvWork.field_0)
        {
            case UnkGfxEnum_0:
                break;

            case UnkGfxEnum_1:
                func_80057658(curMeshHdr, normalOffset, scratchData, &g_WorldEnvWork.light.field_28, &g_WorldEnvWork.light.field_30);
                break;

            case UnkGfxEnum_2:
                func_80057A3C(curMeshHdr, normalOffset, scratchData, &g_WorldEnvWork.light.field_28);
                break;
        }

        func_80057B7C(curMeshHdr, vertOffset, scratchData, mat);
        Gfx_MeshDraw(curMeshHdr, scratchData, otTag, otShift);
    }
}

void func_800574D4(s_MeshHeader* meshHdr, s_GteScratchData* scratchData) // 0x800574D4
{
    DVECTOR* vertexXy;
    s16*     vertexZ;
    DVECTOR* screenXy;
    s16*     var_a2;
    u8*      unkPtr;
    u8*      unkPtrDest;

    screenXy = &scratchData->screenXy_0[0];
    var_a2   = &scratchData->field_18C[0]; // `screenZ`? There's already an earlier struct field though.
    vertexXy = &meshHdr->verticesXy[0];
    vertexZ  = &meshHdr->verticesZ[0];

    while (var_a2 < &scratchData->field_18C[meshHdr->vertexCount])
    {
        *(u32*)screenXy++ = *(u32*)vertexXy++;

        *(u32*)var_a2 = *(u32*)vertexZ;
        vertexZ += 2;
        var_a2 += 2;
    }

    while (screenXy < &scratchData->screenXy_0[meshHdr->vertexCount])
    {
        *(u32*)screenXy++ = *(u32*)vertexXy++;
    }

    unkPtr     = &meshHdr->unkPtr_14[0];
    unkPtrDest = &scratchData->field_2B8[0];

    while (unkPtrDest < &scratchData->field_2B8[meshHdr->unkCount_3])
    {
        *(u32*)unkPtrDest = *(u32*)unkPtr;
        unkPtr += 4;
        unkPtrDest += 4;
    }
}

void func_8005759C(s_MeshHeader* meshHdr, s_GteScratchData* scratchData, s32 vertOffset, s32 normalOffset) // 0x8005759C
{
    s16* vertexZPtr;
    s16* field_18CPtr;
    s32* vertexXyPtr;
    s32* screenXyPtr;
    u8*  field_2B8Ptr;
    u8*  field_14Ptr;

    // Should be loop? Tried but no luck.
    screenXyPtr  = &scratchData->screenXy_0[vertOffset];
    field_18CPtr = &scratchData->field_18C[vertOffset];
    vertexXyPtr  = meshHdr->verticesXy;
    vertexZPtr   = meshHdr->verticesZ;
    while (vertexXyPtr < &meshHdr->verticesXy[meshHdr->vertexCount])
    {
        *screenXyPtr++  = *vertexXyPtr++;
        *field_18CPtr++ = *vertexZPtr++;
    }

    field_14Ptr  = meshHdr->unkPtr_14;
    field_2B8Ptr = &scratchData->field_2B8[normalOffset];
    while (field_14Ptr < &meshHdr->unkPtr_14[meshHdr->unkCount_3])
    {
        *field_2B8Ptr++ = *field_14Ptr++;
    }
}

void func_80057658(s_MeshHeader* meshHdr, s32 offset, s_GteScratchData* scratchData, SVECTOR3* arg3, SVECTOR* arg4) // 0x80057658
{
    s32       geomOffsetX;
    s32       geomOffsetY;
    s32       geomScreen;
    s32       temp_t9;
    s32       var_a1;
    s32       var_v1;
    s32       temp_t2;
    u8        temp_v1;
    u8*       end;
    u8*       var_t0;
    s16*      temp_t8;
    s32*      depthP;
    MATRIX*   mat;
    DVECTOR*  screenPos;
    s_Normal* curNormal;

    scratchData->field_3AC = *arg4; // 3AC changed to `SVECTOR`.

    scratchData->field_380.field_0.m[0][0] = arg3->vx;
    scratchData->field_380.field_0.m[0][1] = arg3->vy;
    scratchData->field_380.field_0.m[0][2] = arg3->vz;

    gte_ldtr_0();

    ReadGeomOffset(&geomOffsetX, &geomOffsetY);
    geomScreen = ReadGeomScreen();
    SetGeomOffset(-1024, -1024);
    SetGeomScreen(16);

    temp_t9 = g_WorldEnvWork.field_20 >> 5;
    temp_v1 = g_WorldEnvWork.field_3;

    var_t0 = &scratchData->field_2B8[offset];
    mat    = &scratchData->field_380.field_0;

    for (curNormal = meshHdr->normals; curNormal < &meshHdr->normals[meshHdr->normalCount]; curNormal++)
    {
        temp_t8   = &scratchData->field_380.field_0.m[2][0];
        screenPos = &scratchData->screenPos_3A4;
        depthP    = &scratchData->depthP_3A8;
        temp_t2   = temp_v1;

        *(u32*)&scratchData->field_3A0 = *(u32*)curNormal;

        mat->m[1][0] = scratchData->field_3A0.nx << 5;
        mat->m[1][1] = scratchData->field_3A0.ny << 5;
        mat->m[1][2] = scratchData->field_3A0.nz << 5;
        gte_SetRotMatrix_Row0_1(mat->m);

        end = &var_t0[scratchData->field_3A0.count];

        mat->m[2][0] = scratchData->screenXy_0[*var_t0].vx - scratchData->field_3AC.vx;
        mat->m[2][1] = scratchData->screenXy_0[*var_t0].vy - scratchData->field_3AC.vy;
        mat->m[2][2] = scratchData->field_18C[*var_t0] - scratchData->field_3AC.vz;
        gte_SetRotMatrix_Row2(mat->m);

        gte_ldv0(temp_t8);
        gte_rtps();
        gte_stsxy(screenPos);
        gte_stdp(depthP);

        while (++var_t0 < end)
        {
            mat->m[2][0] = scratchData->screenXy_0[*var_t0].vx - scratchData->field_3AC.vx;
            mat->m[2][1] = scratchData->screenXy_0[*var_t0].vy - scratchData->field_3AC.vy;
            mat->m[2][2] = scratchData->field_18C[*var_t0] - scratchData->field_3AC.vz;

            gte_SetRotMatrix_Row2(mat->m);
            gte_ldv0(temp_t8);
            gte_rtps();

            scratchData->screenPos_3A4.vx += 1024;
            scratchData->screenPos_3A4.vy += 1024;

            var_a1   = (scratchData->screenPos_3A4.vx * scratchData->screenPos_3A4.vy) + (scratchData->screenPos_3A4.vy * (scratchData->depthP_3A8 >> 4));
            var_a1 >>= 5;
            var_a1  -= 16;
            if (var_a1 < 0)
            {
                var_a1 = 0;
            }

            var_v1 = (scratchData->screenPos_3A4.vx >> 1) + (scratchData->depthP_3A8 >> 2);
            var_a1 += temp_t9;
            if (var_v1 > 48)
            {
                var_v1 = 48;
            }
            var_a1 += var_v1;

            if (temp_t2 < var_a1)
            {
                var_a1 = temp_t2;
            }

            *(var_t0 - 1) = var_a1;

            gte_stsxy(screenPos);
            gte_stdp(depthP);
        }

        scratchData->screenPos_3A4.vx += 1024;
        scratchData->screenPos_3A4.vy += 1024;

        var_a1   = (scratchData->screenPos_3A4.vx * scratchData->screenPos_3A4.vy) + (scratchData->screenPos_3A4.vy * (scratchData->depthP_3A8 >> 4));
        var_a1 >>= 5;
        var_a1  -= 16;
        if (var_a1 < 0)
        {
            var_a1 = 0;
        }

        var_v1  = (scratchData->screenPos_3A4.vx >> 1) + (scratchData->depthP_3A8 >> 2);
        var_a1 += temp_t9;
        if (var_v1 > 48)
        {
            var_v1 = 48;
        }
        var_a1 += var_v1;

        if (temp_t2 < var_a1)
        {
            var_a1 = temp_t2;
        }

        *(var_t0 - 1) = var_a1;
    }

    SetGeomOffset(geomOffsetX, geomOffsetY);
    SetGeomScreen(geomScreen);
}

void func_80057A3C(s_MeshHeader* meshHdr, s32 offset, s_GteScratchData* scratchData, SVECTOR3* lightVec) // 0x80057A3C
{
    s32       var_v1;
    s32       temp_t2;
    u8*       var_a3;
    void*     endPtr;
    s_Normal* normal;

    scratchData->field_380.field_0.m[0][0] = lightVec->vx;
    scratchData->field_380.field_0.m[0][1] = lightVec->vy;
    scratchData->field_380.field_0.m[0][2] = lightVec->vz;
    gte_SetLightMatrix(&scratchData->field_380.field_0);

    var_a3  = &scratchData->field_2B8[offset];
    temp_t2 = g_WorldEnvWork.field_20;

    for (normal = meshHdr->normals; normal < &meshHdr->normals[meshHdr->normalCount]; normal++)
    {
        *(u32*)&scratchData->field_3A0 = *(u32*)normal;

        scratchData->field_3AC.vx = scratchData->field_3A0.nx << 5;
        scratchData->field_3AC.vy = scratchData->field_3A0.ny << 5;
        scratchData->field_3AC.vz = scratchData->field_3A0.nz << 5;

        gte_ldv0(&scratchData->field_3AC);
        gte_ll();

        var_v1   = gte_stIR1();
        var_v1  += temp_t2;
        var_v1 >>= 5;

        if (var_v1 > 0xFF)
        {
            var_v1 = 0xFF;
        }

        endPtr = &var_a3[scratchData->field_3A0.count];
        for (; var_a3 < endPtr; var_a3++)
        {
            *var_a3 = var_v1;
        }
    }
}

void func_80057B7C(s_MeshHeader* meshHdr, s32 offset, s_GteScratchData* scratchData, MATRIX* mat) // 0x80057B7C
{
    DVECTOR* screenXy;
    s16*     temp_a2;
    s32      temp_s2;
    u8*      var_t1;

    temp_s2 = g_WorldEnvWork.fog.depthShift;

    SetRotMatrix(mat);
    SetTransMatrix(mat);

    screenXy = &scratchData->screenXy_0[offset];

    *(s32*)&scratchData->field_380.field_0.m[0][0] = *(s32*)&screenXy[0];
    *(s32*)&scratchData->field_380.field_0.m[1][1] = *(s32*)&screenXy[1];
    *(s32*)&scratchData->field_380.field_0.m[2][2] = *(s32*)&screenXy[2];

    temp_a2 = &scratchData->field_18C[offset];

    scratchData->field_380.field_0.m[0][2]      = temp_a2[0];
    scratchData->field_380.field_0.m[2][0]      = temp_a2[1];
    *(s16*)&scratchData->field_380.field_0.t[0] = temp_a2[2];

    var_t1 = &scratchData->field_252[offset];

    gte_ldv3c(&scratchData->field_380.field_0);
    gte_rtpt();
    gte_stsxy3c(screenXy);
    gte_stsz3(&scratchData->field_380.field_0.m[0][2], &scratchData->field_380.field_0.m[2][0], &scratchData->field_380.field_0.t[0]);

    temp_a2[0] = scratchData->field_380.field_0.m[0][2];
    temp_a2[1] = scratchData->field_380.field_0.m[2][0];
    temp_a2[2] = scratchData->field_380.field_0.t[0];

    screenXy += 3;
    var_t1  += 3;
    temp_a2 += 3;

    if (g_WorldEnvWork.isFogEnabled)
    {
        for (;
             screenXy < &scratchData->screenXy_0[meshHdr->vertexCount + offset];
             screenXy += 3, temp_a2 += 3, var_t1 += 3)
        {
            *(s32*)&scratchData->field_380.field_0.m[0][0] = *(s32*)&screenXy[0];
            *(s32*)&scratchData->field_380.field_0.m[1][1] = *(s32*)&screenXy[1];
            *(s32*)&scratchData->field_380.field_0.m[2][2] = *(s32*)&screenXy[2];

            scratchData->field_380.field_0.m[0][2]      = temp_a2[0];
            scratchData->field_380.field_0.m[2][0]      = temp_a2[1];
            *(s16*)&scratchData->field_380.field_0.t[0] = temp_a2[2];

            gte_ldv3c(&scratchData->field_380.field_0);
            gte_rtpt();

            var_t1[-3] = temp_a2[-3] < (1 << temp_s2) ? g_WorldEnvWork.fogRamp[(temp_a2[-3] << 7) >> temp_s2] : 0xFF;
            var_t1[-2] = temp_a2[-2] < (1 << temp_s2) ? g_WorldEnvWork.fogRamp[(temp_a2[-2] << 7) >> temp_s2] : 0xFF;
            var_t1[-1] = temp_a2[-1] < (1 << temp_s2) ? g_WorldEnvWork.fogRamp[(temp_a2[-1] << 7) >> temp_s2] : 0xFF;

            gte_stsxy3c(screenXy);
            gte_stsz3(&scratchData->field_380.field_0.m[0][2], &scratchData->field_380.field_0.m[2][0], &scratchData->field_380.field_0.t[0]);

            temp_a2[0] = scratchData->field_380.field_0.m[0][2];
            temp_a2[1] = scratchData->field_380.field_0.m[2][0];
            temp_a2[2] = scratchData->field_380.field_0.t[0];
        }

        var_t1[-3] = temp_a2[-3] < (1 << temp_s2) ? g_WorldEnvWork.fogRamp[(temp_a2[-3] << 7) >> temp_s2] : 0xFF;
        var_t1[-2] = temp_a2[-2] < (1 << temp_s2) ? g_WorldEnvWork.fogRamp[(temp_a2[-2] << 7) >> temp_s2] : 0xFF;
        var_t1[-1] = temp_a2[-1] < (1 << temp_s2) ? g_WorldEnvWork.fogRamp[(temp_a2[-1] << 7) >> temp_s2] : 0xFF;
    }
    else
    {
        for (; screenXy < &scratchData->screenXy_0[meshHdr->vertexCount + offset]; screenXy += 3, temp_a2 += 3)
        {
            *(s32*)&scratchData->field_380.field_0.m[0][0] = *(s32*)&screenXy[0];
            *(s32*)&scratchData->field_380.field_0.m[1][1] = *(s32*)&screenXy[1];
            *(s32*)&scratchData->field_380.field_0.m[2][2] = *(s32*)&screenXy[2];

            scratchData->field_380.field_0.m[0][2]      = temp_a2[0];
            scratchData->field_380.field_0.m[2][0]      = temp_a2[1];
            *(s16*)&scratchData->field_380.field_0.t[0] = temp_a2[2];

            gte_ldv3c(&scratchData->field_380.field_0);
            gte_rtpt();
            gte_stsxy3c(screenXy);
            gte_stsz3(&scratchData->field_380.field_0.m[0][2], &scratchData->field_380.field_0.m[2][0], &scratchData->field_380.field_0.t[0]);

            temp_a2[0] = scratchData->field_380.field_0.m[0][2];
            temp_a2[1] = scratchData->field_380.field_0.m[2][0];
            temp_a2[2] = scratchData->field_380.field_0.t[0];
        }
    }
}

void Gfx_MeshDraw(s_MeshHeader* meshHdr, s_GteScratchData* scratchData, GsOT_TAG* tag, s32 otShift) // 0x8005801C
{
    s32          sp10;
    s32          sp14;
    s32          sp18;
    s32          sp1C;
    s32          sp20;
    s32          var_t3;
    s32          var_t3_2;
    s32          temp_a2;
    s32          temp_a2_3;
    s32          temp_a2_4;
    s32          temp_a2_5;
    s32          temp_a2_7;
    s32          temp_a0;
    s32          temp_a0_13;
    s32          temp_a0_5;
    s32          temp_a0_7;
    s32          temp_a0_9;
    s32          temp_a1;
    s32          temp_a1_2;
    s32          temp_a1_3;
    s32          temp_a1_4;
    s32          temp_a1_5;
    s32          temp_a2_2;
    s32          temp_a2_6;
    s32          temp_a3;
    s32          temp_a3_2;
    s32          temp_a3_3;
    s32          temp_a3_4;
    s32          temp_a3_5;
    s32          temp_v1;
    s32          temp_v1_11;
    s32          temp_v1_16;
    s32          temp_v1_21;
    s32          temp_v1_27;
    s32          temp_v1_5;
    u32          temp_t0;
    u32          temp_t0_2;
    u32          temp_t0_3;
    u32          temp_t0_4;
    u32          temp_t0_5;
    s32          temp;
    s32          temp2;
    s32          temp3;
    s32          temp4;
    s_Primitive* prim;
    PACKET*      packet0;
    PACKET*      packet1;
    POLY_GT4*    poly0;
    POLY_G4*     poly1;
    POLY_G4*     poly2;
    POLY_GT4*    poly3;
    POLY_FT4*    poly4;

    temp_v1 = 0x79C << (otShift + 2);

    if (!g_WorldEnvWork.isFogEnabled)
    {
        scratchData->field_380.s_0.field_1C = temp_v1;
    }
    else
    {
        scratchData->field_380.s_0.field_1C = g_WorldEnvWork.fog.farDistance;

        if (temp_v1 < scratchData->field_380.s_0.field_1C)
        {
            scratchData->field_380.s_0.field_1C = temp_v1;
        }
    }

    scratchData->field_380.s_0.field_0    = g_GameWork.gsScreenWidth >> 1;
    scratchData->field_380.s_0.field_4    = g_WorldEnvWork.fog.intensity;
    scratchData->field_380.s_0.field_8    = g_WorldEnvWork.worldTintColor;
    scratchData->field_380.s_0.field_8.cd = 60;

    if (g_WorldEnvWork.field_0 == UnkGfxEnum_0)
    {
        gte_lddp(Q12(1.0f) - g_WorldEnvWork.field_20);
        gte_ldrgb(&scratchData->field_380.s_0.field_8);
        gte_dpcs();
        gte_strgb(&scratchData->field_380.s_0.field_8);
    }

    scratchData->field_380.s_0.field_C    = g_WorldEnvWork.fog.color;
    scratchData->field_380.s_0.field_C.cd = 56;

    SetBackColor(0, 0, 0);

    prim = meshHdr->primitives;

    if (g_WorldEnvWork.field_0 != UnkGfxEnum_0)
    {
        if (g_WorldEnvWork.isFogEnabled)
        {
            if (*(s32*)&scratchData->field_380.s_0.field_C & 0xFFFFFF)
            {
                poly3 = GsOUT_PACKET_P;
                poly1  = poly3 + 1;

                for (; prim < &meshHdr->primitives[meshHdr->primitiveCount]; prim++)
                {
                    *(s32*)&scratchData->field_380.s_0.field_10 = *(s32*)&prim->faceIdxs;

                    scratchData->field_380.s_0.field_18 = scratchData->field_18C[scratchData->field_380.s_0.field_10];
                    if (scratchData->field_380.s_0.field_18 < scratchData->field_18C[scratchData->field_380.s_0.field_11])
                    {
                        scratchData->field_380.s_0.field_18 = scratchData->field_18C[scratchData->field_380.s_0.field_11];
                    }
                    if (scratchData->field_380.s_0.field_18 < scratchData->field_18C[scratchData->field_380.s_0.field_12])
                    {
                        scratchData->field_380.s_0.field_18 = scratchData->field_18C[scratchData->field_380.s_0.field_12];
                    }
                    if (scratchData->field_380.s_0.field_18 < scratchData->field_18C[scratchData->field_380.s_0.field_13])
                    {
                        scratchData->field_380.s_0.field_18 = scratchData->field_18C[scratchData->field_380.s_0.field_13];
                    }

                    if (scratchData->field_380.s_0.field_18 <= 0)
                    {
                        continue;
                    }

                    if (scratchData->field_380.s_0.field_18 <= 32)
                    {
                        scratchData->field_380.s_0.field_18 = 32;
                    }

                    if (scratchData->field_380.s_0.field_18 > scratchData->field_380.s_0.field_1C)
                    {
                        continue;
                    }

                    gte_NormalClip(*(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_10],
                                   *(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_11],
                                   *(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_12], &sp10);

                    if (sp10 <= 0)
                    {
                        gte_ldsxy0(*(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_13]);
                        gte_nclip();
                        gte_stopz(&sp10);

                        if (sp10 >= 0)
                        {
                            continue;
                        }
                    }

                    temp_a3 = scratchData->field_380.s_0.field_0;
                    temp_a2 = *(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_10];

                    temp_a1   = *(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_11];
                    temp_a0   = *(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_12];
                    temp_v1_5 = *(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_13];
                    temp_t0   = temp_a3 * 2;
                    temp_a2_2 = temp_a2;

                    if ((s16)temp_a2 + temp_a3 < temp_t0 || (s16)temp_a1 + temp_a3 < temp_t0 ||
                        (s16)temp_a0 + temp_a3 < temp_t0 || (s16)temp_v1_5 + temp_a3 < temp_t0)
                    {
                        *(s32*)&poly3->x0 = temp_a2_2;
                        *(s32*)&poly1->x0  = temp_a2_2;
                        *(s32*)&poly3->x1 = temp_a1;
                        *(s32*)&poly1->x1  = temp_a1;
                        *(s32*)&poly3->x2 = temp_a0;
                        *(s32*)&poly1->x2  = temp_a0;
                        *(s32*)&poly3->x3 = temp_v1_5;
                        *(s32*)&poly1->x3  = temp_v1_5;

                        *(s32*)&scratchData->field_380.s_0.field_14 = *(s32*)&prim->field_10;

                        var_t3  = Q12(1.0f) - scratchData->field_252[scratchData->field_380.s_0.field_10] * 16;
                        var_t3 -= scratchData->field_380.s_0.field_4;
                        if (var_t3 < 0)
                        {
                            var_t3 = 0;
                        }

                        gte_lddp(var_t3);
                        gte_ldrgb(&scratchData->field_380.s_0.field_C);
                        gte_dpcs();
                        gte_strgb(&poly1->r0);
                        gte_lddp(scratchData->field_252[scratchData->field_380.s_0.field_10] << 4);
                        gte_ldsv_(scratchData->field_2B8[scratchData->field_380.s_0.field_14] << 5);
                        gte_ldrgb(&scratchData->field_380.s_0.field_8);
                        gte_dpcl();
                        gte_strgb(&poly3->r0);

                        var_t3  = Q12(1.0f) - scratchData->field_252[scratchData->field_380.s_0.field_11] * 16;
                        var_t3 -= scratchData->field_380.s_0.field_4;
                        if (var_t3 < 0)
                        {
                            var_t3 = 0;
                        }

                        gte_lddp(var_t3);
                        gte_ldrgb(&scratchData->field_380.s_0.field_C);
                        gte_dpcs();
                        gte_strgb(&poly1->r1);
                        gte_lddp(scratchData->field_252[scratchData->field_380.s_0.field_11] << 4);
                        gte_ldsv_(scratchData->field_2B8[scratchData->field_380.s_0.field_15] << 5);
                        gte_ldrgb(&scratchData->field_380.s_0.field_8);
                        gte_dpcl();
                        gte_strgb(&poly3->r1);

                        var_t3  = Q12(1.0f) - scratchData->field_252[scratchData->field_380.s_0.field_12] * 0x10;
                        var_t3 -= scratchData->field_380.s_0.field_4;
                        if (var_t3 < 0)
                        {
                            var_t3 = 0;
                        }

                        gte_lddp(var_t3);
                        gte_ldrgb(&scratchData->field_380.s_0.field_C);
                        gte_dpcs();
                        gte_strgb(&poly1->r2);
                        gte_lddp(scratchData->field_252[scratchData->field_380.s_0.field_12] << 4);
                        gte_ldsv_(scratchData->field_2B8[scratchData->field_380.s_0.field_16] << 5);
                        gte_ldrgb(&scratchData->field_380.s_0.field_8);
                        gte_dpcl();
                        gte_strgb(&poly3->r2);

                        var_t3  = Q12(1.0f) - scratchData->field_252[scratchData->field_380.s_0.field_13] * 0x10;
                        var_t3 -= scratchData->field_380.s_0.field_4;
                        if (var_t3 < 0)
                        {
                            var_t3 = 0;
                        }

                        gte_lddp(var_t3);
                        gte_ldrgb(&scratchData->field_380.s_0.field_C);
                        gte_dpcs();
                        gte_strgb(&poly1->r3);
                        gte_lddp(scratchData->field_252[scratchData->field_380.s_0.field_13] << 4);
                        gte_ldsv_(scratchData->field_2B8[scratchData->field_380.s_0.field_17] << 5);
                        gte_ldrgb(&scratchData->field_380.s_0.field_8);
                        gte_dpcl();
                        gte_strgb(&poly3->r3);

                        *(s32*)&poly3->u0 = *(s32*)&prim->u0;
                        *(s32*)&poly3->u1 = *(s32*)&prim->u1 & 0xFFFFFF;
                        *(u16*)&poly3->u2 = *(u16*)&prim->u2;
                        *(u16*)&poly3->u3 = *(u16*)&prim->u3;

                        setlen(poly3, 12);
                        setlen(poly1, 8);

                        if (prim->bits1.flags & (1 << 15))
                        {
                            packet1 = poly1 + 1;

                            SetPriority(packet1, 0, 0);
                            addPrim(&tag[(scratchData->field_380.s_0.field_18 >> otShift) >> 2], packet1);

                            setSemiTrans(poly1, 1);
                            addPrim(&tag[(scratchData->field_380.s_0.field_18 >> otShift) >> 2], poly1);

                            packet1 = (PACKET*)(poly1 + 1) + 12;
                            SetPriority(packet1, 1, 1);
                            addPrim(&tag[(scratchData->field_380.s_0.field_18 >> otShift) >> 2], packet1);

                            addPrim(&tag[(scratchData->field_380.s_0.field_18 >> otShift) >> 2], poly3);

                            poly3 = (PACKET*)(poly1 + 1) + 12 + 12;
                            poly1  = poly3 + 1;
                        }
                        else
                        {
                            setSemiTrans(poly3, 1);

                            addPrim(&tag[(scratchData->field_380.s_0.field_18 >> otShift) >> 2], poly3);
                            addPrim(&tag[(scratchData->field_380.s_0.field_18 >> otShift) >> 2], poly1);

                            poly3 = poly1 + 1;
                            poly1  = poly3 + 1;
                        }
                    }
                }

                GsOUT_PACKET_P = poly1; // @bug? Should be `poly_gt4`
                return;
            }

            poly3 = GsOUT_PACKET_P;

            for (; prim < &meshHdr->primitives[meshHdr->primitiveCount]; prim++)
            {
                *(s32*)&scratchData->field_380.s_0.field_10 = *(s32*)&prim->faceIdxs;

                scratchData->field_380.s_0.field_18 = scratchData->field_18C[scratchData->field_380.s_0.field_10];

                if (scratchData->field_380.s_0.field_18 < scratchData->field_18C[scratchData->field_380.s_0.field_11])
                {
                    scratchData->field_380.s_0.field_18 = scratchData->field_18C[scratchData->field_380.s_0.field_11];
                }

                if (scratchData->field_380.s_0.field_18 < scratchData->field_18C[scratchData->field_380.s_0.field_12])
                {
                    scratchData->field_380.s_0.field_18 = scratchData->field_18C[scratchData->field_380.s_0.field_12];
                }

                if (scratchData->field_380.s_0.field_18 < scratchData->field_18C[scratchData->field_380.s_0.field_13])
                {
                    scratchData->field_380.s_0.field_18 = scratchData->field_18C[scratchData->field_380.s_0.field_13];
                }

                if (scratchData->field_380.s_0.field_18 <= 0)
                {
                    continue;
                }

                if (scratchData->field_380.s_0.field_18 <= 32)
                {
                    scratchData->field_380.s_0.field_18 = 32;
                }

                if (scratchData->field_380.s_0.field_18 > scratchData->field_380.s_0.field_1C)
                {
                    continue;
                }

                gte_ldsxy3(*(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_10],
                           *(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_11],
                           *(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_12]);
                gte_nclip();
                gte_stopz(&sp14);

                if (sp14 <= 0)
                {
                    gte_ldsxy0(*(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_13]);
                    gte_nclip();
                    gte_stopz(&sp14);

                    if (sp14 >= 0)
                    {
                        continue;
                    }
                }

                temp_a3_2 = scratchData->field_380.s_0.field_0;
                temp_a2_3 = *(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_10];

                temp_a1_2  = *(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_11];
                temp_a0_5  = *(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_12];
                temp_v1_11 = *(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_13];
                temp_t0_2  = temp_a3_2 * 2;
                temp2      = temp_a2_3;

                if ((s16)temp_a2_3 + temp_a3_2 < temp_t0_2 || (s16)temp_a1_2 + temp_a3_2 < temp_t0_2 ||
                    (s16)temp_a0_5 + temp_a3_2 < temp_t0_2 || (s16)temp_v1_11 + temp_a3_2 < temp_t0_2)
                {
                    *(s32*)&poly3->x0 = temp2;
                    *(s32*)&poly3->x1 = temp_a1_2;
                    *(s32*)&poly3->x2 = temp_a0_5;
                    *(s32*)&poly3->x3 = temp_v1_11;

                    *(s32*)&scratchData->field_380.s_0.field_14 = *(s32*)&prim->field_10;

                    gte_lddp(scratchData->field_252[scratchData->field_380.s_0.field_10] << 4);
                    gte_ldsv_(scratchData->field_2B8[scratchData->field_380.s_0.field_14] << 5);
                    gte_ldrgb(&scratchData->field_380.s_0.field_8);
                    gte_dpcl();
                    gte_strgb(&poly3->r0);

                    gte_lddp(scratchData->field_252[scratchData->field_380.s_0.field_11] << 4);
                    gte_ldsv_(scratchData->field_2B8[scratchData->field_380.s_0.field_15] << 5);
                    gte_ldrgb(&scratchData->field_380.s_0.field_8);
                    gte_dpcl();
                    gte_strgb(&poly3->r1);

                    gte_lddp(scratchData->field_252[scratchData->field_380.s_0.field_12] << 4);
                    gte_ldsv_(scratchData->field_2B8[scratchData->field_380.s_0.field_16] << 5);
                    gte_ldrgb(&scratchData->field_380.s_0.field_8);
                    gte_dpcl();
                    gte_strgb(&poly3->r2);

                    gte_lddp(scratchData->field_252[scratchData->field_380.s_0.field_13] << 4);
                    gte_ldsv_(scratchData->field_2B8[scratchData->field_380.s_0.field_17] << 5);
                    gte_ldrgb(&scratchData->field_380.s_0.field_8);
                    gte_dpcl();
                    gte_strgb(&poly3->r3);

                    *(s32*)&poly3->u0 = *(s32*)&prim->u0;
                    *(s32*)&poly3->u1 = *(s32*)&prim->u1 & 0xFFFFFF;
                    *(u16*)&poly3->u2 = *(u16*)&prim->u2;
                    *(u16*)&poly3->u3 = *(u16*)&prim->u3;

                    setlen(poly3, 12);

                    addPrim(&tag[(scratchData->field_380.s_0.field_18 >> otShift) >> 2], poly3);
                    poly3++;
                }
            }

            GsOUT_PACKET_P = poly3;
            return;
        }
        else
        {
            goto __block1530;
        }
    }

    if (g_WorldEnvWork.isFogEnabled != 0)
    {
        poly3  = GsOUT_PACKET_P;
        poly2 = poly3 + 1;

        for (; prim < &meshHdr->primitives[meshHdr->primitiveCount]; prim++)
        {
            *(s32*)&scratchData->field_380.s_0.field_10 = *(s32*)&prim->faceIdxs;

            scratchData->field_380.s_0.field_18 = scratchData->field_18C[scratchData->field_380.s_0.field_10];

            if (scratchData->field_380.s_0.field_18 < scratchData->field_18C[scratchData->field_380.s_0.field_11])
            {
                scratchData->field_380.s_0.field_18 = scratchData->field_18C[scratchData->field_380.s_0.field_11];
            }

            if (scratchData->field_380.s_0.field_18 < scratchData->field_18C[scratchData->field_380.s_0.field_12])
            {
                scratchData->field_380.s_0.field_18 = scratchData->field_18C[scratchData->field_380.s_0.field_12];
            }

            if (scratchData->field_380.s_0.field_18 < scratchData->field_18C[scratchData->field_380.s_0.field_13])
            {
                scratchData->field_380.s_0.field_18 = scratchData->field_18C[scratchData->field_380.s_0.field_13];
            }

            if (scratchData->field_380.s_0.field_18 <= 0)
            {
                continue;
            }

            if (scratchData->field_380.s_0.field_18 <= 32)
            {
                scratchData->field_380.s_0.field_18 = 32;
            }

            if (scratchData->field_380.s_0.field_18 > scratchData->field_380.s_0.field_1C)
            {
                continue;
            }

            gte_ldsxy3(*(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_10],
                       *(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_11],
                       *(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_12]);
            gte_nclip();
            gte_stopz(&sp18);

            if (sp18 <= 0)
            {
                gte_ldsxy0(*(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_13]);
                gte_nclip();
                gte_stopz(&sp18);

                if (sp18 >= 0)
                {
                    continue;
                }
            }

            temp_a3_4 = scratchData->field_380.s_0.field_0;
            temp_a2_5 = *(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_10];

            temp_a1_4  = *(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_11];
            temp_a0_9  = *(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_12];
            temp_v1_21 = *(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_13];
            temp_t0_4  = temp_a3_4 * 2;
            temp_a2_6  = temp_a2_5;

            if (((s16)temp_a2_5 + temp_a3_4) < temp_t0_4 || ((s16)temp_a1_4 + temp_a3_4) < temp_t0_4 ||
                ((s16)temp_a0_9 + temp_a3_4) < temp_t0_4 || ((s16)temp_v1_21 + temp_a3_4) < temp_t0_4)
            {
                *(s32*)&poly3->x0 = temp_a2_6;
                *(s32*)&poly2->x0 = temp_a2_6;
                *(s32*)&poly3->x1 = temp_a1_4;
                *(s32*)&poly2->x1 = temp_a1_4;
                *(s32*)&poly3->x2 = temp_a0_9;
                *(s32*)&poly2->x2 = temp_a0_9;
                *(s32*)&poly3->x3 = temp_v1_21;
                *(s32*)&poly2->x3 = temp_v1_21;

                temp4    = Q12(1.0f) - scratchData->field_252[scratchData->field_380.s_0.field_10] * 16;
                var_t3_2 = temp4 - scratchData->field_380.s_0.field_4;
                if (var_t3_2 < 0)
                {
                    var_t3_2 = 0;
                }

                gte_lddp(var_t3_2);
                gte_ldrgb(&scratchData->field_380.s_0.field_C);
                gte_dpcs();
                gte_strgb(&poly2->r0);
                gte_lddp(Q12(1.0f) - var_t3_2);
                gte_ldrgb(&scratchData->field_380.s_0.field_8);
                gte_dpcs();
                gte_strgb(&poly3->r0);

                temp4    = Q12(1.0f) - scratchData->field_252[scratchData->field_380.s_0.field_11] * 16;
                var_t3_2 = temp4 - scratchData->field_380.s_0.field_4;
                if (var_t3_2 < 0)
                {
                    var_t3_2 = 0;
                }

                gte_lddp(var_t3_2);
                gte_ldrgb(&scratchData->field_380.s_0.field_C);
                gte_dpcs();
                gte_strgb(&poly2->r1);
                gte_lddp(Q12(1.0f) - var_t3_2);
                gte_ldrgb(&scratchData->field_380.s_0.field_8);
                gte_dpcs();
                gte_strgb(&poly3->r1);

                temp4    = Q12(1.0f) - scratchData->field_252[scratchData->field_380.s_0.field_12] * 16;
                var_t3_2 = temp4 - scratchData->field_380.s_0.field_4;
                if (var_t3_2 < 0)
                {
                    var_t3_2 = 0;
                }

                gte_lddp(var_t3_2);
                gte_ldrgb(&scratchData->field_380.s_0.field_C);
                gte_dpcs();
                gte_strgb(&poly2->r2);
                gte_lddp(Q12(1.0f) - var_t3_2);
                gte_ldrgb(&scratchData->field_380.s_0.field_8);
                gte_dpcs();
                gte_strgb(&poly3->r2);

                temp4    = Q12(1.0f) - scratchData->field_252[scratchData->field_380.s_0.field_13] * 16;
                var_t3_2 = temp4 - scratchData->field_380.s_0.field_4;
                if (var_t3_2 < 0)
                {
                    var_t3_2 = 0;
                }

                gte_lddp(var_t3_2);
                gte_ldrgb(&scratchData->field_380.s_0.field_C);
                gte_dpcs();
                gte_strgb(&poly2->r3);
                gte_lddp(Q12(1.0f) - var_t3_2);
                gte_ldrgb(&scratchData->field_380.s_0.field_8);
                gte_dpcs();
                gte_strgb(&poly3->r3);

                *(s32*)&poly3->u0 = *(s32*)&prim->u0;
                *(s32*)&poly3->u1 = *(s32*)&prim->u1 & 0xFFFFFF;
                *(u16*)&poly3->u2 = *(u16*)&prim->u2;
                *(u16*)&poly3->u3 = *(u16*)&prim->u3;

                setlen(poly3, 12);
                setlen(poly2, 8);

                if (prim->bits1.flags & (1 << 15))
                {
                    packet0 = poly2 + 1;

                    SetPriority(packet0, 0, 0);

                    addPrim(&tag[(scratchData->field_380.s_0.field_18 >> otShift) >> 2], packet0);
                    setSemiTrans(poly2, 1);
                    addPrim(&tag[(scratchData->field_380.s_0.field_18 >> otShift) >> 2], poly2);

                    packet0 = (PACKET*)(poly2 + 1) + 12;
                    SetPriority(packet0, 1, 1);
                    addPrim(&tag[(scratchData->field_380.s_0.field_18 >> otShift) >> 2], packet0);
                    addPrim(&tag[(scratchData->field_380.s_0.field_18 >> otShift) >> 2], poly3);

                    poly3  = (PACKET*)(poly2 + 1) + 12 + 12;
                    poly2 = poly3 + 1;
                }
                else
                {
                    setSemiTrans(poly3, 1);
                    addPrim(&tag[(scratchData->field_380.s_0.field_18 >> otShift) >> 2], poly3);
                    addPrim(&tag[(scratchData->field_380.s_0.field_18 >> otShift) >> 2], poly2);

                    poly3  = poly2 + 1;
                    poly2 = poly3 + 1;
                }
            }
        }

        GsOUT_PACKET_P = poly2; // @bug? Should be `poly_gt4`.
        return;
    }
    else
    {
        goto __block19CC;
    }

__block1530:
{
    poly0 = GsOUT_PACKET_P;

    for (; prim < &meshHdr->primitives[meshHdr->primitiveCount]; prim++)
    {
        *(s32*)&scratchData->field_380.s_0.field_10 = *(s32*)&prim->faceIdxs;

        scratchData->field_380.s_0.field_18 = scratchData->field_18C[scratchData->field_380.s_0.field_10];

        if (scratchData->field_380.s_0.field_18 < scratchData->field_18C[scratchData->field_380.s_0.field_11])
        {
            scratchData->field_380.s_0.field_18 = scratchData->field_18C[scratchData->field_380.s_0.field_11];
        }

        if (scratchData->field_380.s_0.field_18 < scratchData->field_18C[scratchData->field_380.s_0.field_12])
        {
            scratchData->field_380.s_0.field_18 = scratchData->field_18C[scratchData->field_380.s_0.field_12];
        }

        if (scratchData->field_380.s_0.field_18 < scratchData->field_18C[scratchData->field_380.s_0.field_13])
        {
            scratchData->field_380.s_0.field_18 = scratchData->field_18C[scratchData->field_380.s_0.field_13];
        }

        if (scratchData->field_380.s_0.field_18 <= 0)
        {
            continue;
        }

        if (scratchData->field_380.s_0.field_18 <= 32)
        {
            scratchData->field_380.s_0.field_18 = 32;
        }

        if (scratchData->field_380.s_0.field_18 > scratchData->field_380.s_0.field_1C)
        {
            continue;
        }

        gte_ldsxy3(*(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_10],
                   *(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_11],
                   *(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_12]);
        gte_nclip();
        gte_stopz(&sp1C);

        if (sp1C <= 0)
        {
            gte_ldsxy0(*(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_13]);
            gte_nclip();
            gte_stopz(&sp1C);

            if (sp1C >= 0)
            {
                continue;
            }
        }

        temp_a3_3 = scratchData->field_380.s_0.field_0;
        temp_a2_4 = *(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_10];

        temp_a1_3  = *(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_11];
        temp_a0_7  = *(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_12];
        temp_v1_16 = *(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_13];
        temp_t0_3  = temp_a3_3 * 2;
        temp3      = temp_a2_4;

        if (((s16)temp_a2_4 + temp_a3_3) < temp_t0_3 || ((s16)temp_a1_3 + temp_a3_3) < temp_t0_3 ||
            ((s16)temp_a0_7 + temp_a3_3) < temp_t0_3 || ((s16)temp_v1_16 + temp_a3_3) < temp_t0_3)
        {
            *(s32*)&poly0->x0 = temp3;
            *(s32*)&poly0->x1 = temp_a1_3;
            *(s32*)&poly0->x2 = temp_a0_7;
            *(s32*)&poly0->x3 = temp_v1_16;

            *(s32*)&scratchData->field_380.s_0.field_14 = *(s32*)&prim->field_10;

            if (scratchData->field_2B8[scratchData->field_380.s_0.field_14] >= 8)
            {
                gte_lddp(Q12(1.0f) - (scratchData->field_2B8[scratchData->field_380.s_0.field_14] << 5));
                gte_ldrgb(&scratchData->field_380.s_0.field_8);
                gte_dpcs();
                gte_strgb(&poly0->r0);
            }
            else
            {
                *(s32*)&poly0->r0 = 0x3C000000;
            }

            if (scratchData->field_2B8[scratchData->field_380.s_0.field_15] >= 8)
            {
                gte_lddp(Q12(1.0f) - (scratchData->field_2B8[scratchData->field_380.s_0.field_15] << 5));
                gte_ldrgb(&scratchData->field_380.s_0.field_8);
                gte_dpcs();
                gte_strgb(&poly0->r1);
            }
            else
            {
                *(s32*)&poly0->r1 = 0x3C000000;
            }

            if (scratchData->field_2B8[scratchData->field_380.s_0.field_16] >= 8)
            {
                gte_lddp(Q12(1.0f) - (scratchData->field_2B8[scratchData->field_380.s_0.field_16] << 5));
                gte_ldrgb(&scratchData->field_380.s_0.field_8);
                gte_dpcs();
                gte_strgb(&poly0->r2);
            }
            else
            {
                *(s32*)&poly0->r2 = 0x3C000000;
            }

            if (scratchData->field_2B8[scratchData->field_380.s_0.field_17] >= 8)
            {
                gte_lddp(Q12(1.0f) - (scratchData->field_2B8[scratchData->field_380.s_0.field_17] << 5));
                gte_ldrgb(&scratchData->field_380.s_0.field_8);
                gte_dpcs();
                gte_strgb(&poly0->r3);
            }
            else
            {
                *(s32*)&poly0->r3 = 0x3C000000;
            }

            *(s32*)&poly0->u0 = *(s32*)&prim->u0;
            *(s32*)&poly0->u1 = *(s32*)&prim->u1 & 0xFFFFFF;
            *(u16*)&poly0->u2 = *(u16*)&prim->u2;
            *(u16*)&poly0->u3 = *(u16*)&prim->u3;

            setlen(poly0, 12);

            addPrim(&tag[(scratchData->field_380.s_0.field_18 >> otShift) >> 2], poly0);
            poly0++;
        }
    }

    GsOUT_PACKET_P = poly0;
    return;
}

__block19CC:
    scratchData->field_380.s_0.field_8.cd = 44;
    poly4                       = GsOUT_PACKET_P;

    for (prim = meshHdr->primitives; prim < &meshHdr->primitives[meshHdr->primitiveCount]; prim++)
    {
        *(s32*)&scratchData->field_380.s_0.field_10 = *(s32*)&prim->faceIdxs;
        scratchData->field_380.s_0.field_18         = scratchData->field_18C[scratchData->field_380.s_0.field_10];

        if (scratchData->field_380.s_0.field_18 < scratchData->field_18C[scratchData->field_380.s_0.field_11])
        {
            scratchData->field_380.s_0.field_18 = scratchData->field_18C[scratchData->field_380.s_0.field_11];
        }

        if (scratchData->field_380.s_0.field_18 < scratchData->field_18C[scratchData->field_380.s_0.field_12])
        {
            scratchData->field_380.s_0.field_18 = scratchData->field_18C[scratchData->field_380.s_0.field_12];
        }

        if (scratchData->field_380.s_0.field_18 < scratchData->field_18C[scratchData->field_380.s_0.field_13])
        {
            scratchData->field_380.s_0.field_18 = scratchData->field_18C[scratchData->field_380.s_0.field_13];
        }

        if (scratchData->field_380.s_0.field_18 <= 0)
        {
            continue;
        }

        if (scratchData->field_380.s_0.field_18 <= 32)
        {
            scratchData->field_380.s_0.field_18 = 32;
        }

        if (scratchData->field_380.s_0.field_18 > scratchData->field_380.s_0.field_1C)
        {
            continue;
        }

        gte_ldsxy3(*(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_10],
                   *(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_11],
                   *(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_12]);
        gte_nclip();
        gte_stopz(&sp20);

        if (sp20 <= 0)
        {
            gte_ldsxy0(*(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_13]);
            gte_nclip();
            gte_stopz(&sp20);

            if (sp20 >= 0)
            {
                continue;
            }
        }

        temp_a3_5 = scratchData->field_380.s_0.field_0;
        temp_a2_7 = *(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_10];

        temp_a1_5  = *(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_11];
        temp_a0_13 = *(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_12];
        temp_v1_27 = *(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_13];
        temp_t0_5  = temp_a3_5 * 2;
        temp       = temp_a2_7;

        if (((s16)temp_a2_7 + temp_a3_5) < temp_t0_5 || ((s16)temp_a1_5 + temp_a3_5) < temp_t0_5 ||
            ((s16)temp_a0_13 + temp_a3_5) < temp_t0_5 || ((s16)temp_v1_27 + temp_a3_5) < temp_t0_5)
        {
            *(s32*)&poly4->x0 = temp;
            *(s32*)&poly4->x1 = temp_a1_5;
            *(s32*)&poly4->x2 = temp_a0_13;
            *(s32*)&poly4->x3 = temp_v1_27;

            *(s32*)&poly4->r0 = *(s32*)&scratchData->field_380.s_0.field_8;

            *(s32*)&poly4->u0 = *(s32*)&prim->u0;
            *(s32*)&poly4->u1 = *(s32*)&prim->u1 & 0xFFFFFF;
            *(u16*)&poly4->u2 = *(u16*)&prim->u2;
            *(u16*)&poly4->u3 = *(u16*)&prim->u3;

            setlen(poly4, 9);

            addPrim(&tag[(scratchData->field_380.s_0.field_18 >> otShift) >> 2], poly4);

            poly4++;
        }
    }

    GsOUT_PACKET_P = poly4;
}

void func_80059D50(s32 arg0, s_ModelInfo* modelInfo, MATRIX* viewMat, s32 otShift, GsOT_TAG* tag) // 0x80059D50
{
    s_GteScratchData* scratchData;
    s_MeshHeader*     curMeshHdr;
    s_ModelHeader*    modelHdr;

    scratchData = PSX_SCRATCH_ADDR(0);

    modelHdr = modelInfo->modelHdr;

    for (curMeshHdr = &modelHdr->meshHdrs[0]; curMeshHdr < &modelHdr->meshHdrs[modelHdr->meshCount]; curMeshHdr++)
    {
        func_800574D4(curMeshHdr, scratchData);
        func_80057B7C(curMeshHdr, 0, scratchData, viewMat);
        func_80059E34(arg0, curMeshHdr, scratchData, otShift, tag);
    }
}

void func_80059E34(u32 arg0, s_MeshHeader* meshHdr, s_GteScratchData* scratchData, s32 otShift, GsOT_TAG* tag) // 0x80059E34
{
    s32          sp0;
    s32          var_t2;
    s32          x2;
    s32          x1;
    s32          temp_v1;
    s32          x3;
    u16          var_a2;
    s32          packedColor;
    s32          var_t9;
    u32          temp_t0;
    u32          temp_t1;
    s32          temp;
    s32          x0;
    GsOT_TAG*    tag0;
    GsOT_TAG*    tag1;
    s_Primitive* prim;
    POLY_FT4*    poly;

    tag1 = &g_OrderingTable0[g_ActiveBufferIdx].org[640];

    switch (arg0)
    {
        case 0:
        default:
            return;

        case 1:
        case 3:
            packedColor = 0x2E383838;
            var_a2 = 0x140;
            break;

        case 2:
            packedColor = 0x2E808080;
            var_a2 = 0x20;
            break;
    }

    temp_v1 = Q8(7.61f) << (otShift + 2);
    var_t9  = g_WorldEnvWork.isFogEnabled ? MIN(temp_v1, g_WorldEnvWork.fog.farDistance) : temp_v1;

    poly                        = (POLY_FT4*)GsOUT_PACKET_P;
    scratchData->field_380.s_0.field_0 = g_GameWork.gsScreenWidth >> 1;

    for (prim = meshHdr->primitives; prim < &meshHdr->primitives[meshHdr->primitiveCount]; prim++)
    {
        *(s32*)&scratchData->field_380.s_0.field_10 = *(s32*)&prim->faceIdxs;

        var_t2 = scratchData->field_18C[scratchData->field_380.s_0.field_10];
        var_t2 = MAX(scratchData->field_18C[scratchData->field_380.s_0.field_11], var_t2);
        var_t2 = MAX(scratchData->field_18C[scratchData->field_380.s_0.field_12], var_t2);
        var_t2 = MAX(scratchData->field_18C[scratchData->field_380.s_0.field_13], var_t2);

        if (var_t2 <= 0)
        {
            continue;
        }

        if (var_t2 <= 32)
        {
            var_t2 = 32;
        }

        if (var_t9 < var_t2)
        {
            continue;
        }

        gte_NormalClip(*(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_10],
                       *(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_11],
                       *(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_12],
                       &sp0);

        if (sp0 <= 0)
        {
            gte_ldsxy0(*(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_13]);
            gte_nclip();
            gte_stopz(&sp0);

            if (sp0 >= 0)
            {
                continue;
            }
        }

        temp = *(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_10];
        x1 = *(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_11];
        x2 = *(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_12];
        x3 = *(s32*)&scratchData->screenXy_0[scratchData->field_380.s_0.field_13];

        temp_t0 = scratchData->field_380.s_0.field_0;
        temp_t1 = temp_t0 * 2;
        x0 = temp;

        if ((scratchData->screenXy_0[scratchData->field_380.s_0.field_10].vx + temp_t0) >= temp_t1 &&
            ((s16)x1 + temp_t0) >= temp_t1 &&
            ((s16)x2 + temp_t0) >= temp_t1 &&
            ((s16)x3 + temp_t0) >= temp_t1)
        {
            continue;
        }

        *(s32*)&poly->x0 = x0;
        *(s32*)&poly->x1 = x1;
        *(s32*)&poly->x2 = x2;
        *(s32*)&poly->x3 = x3;

        *(s32*)&poly->r0 = packedColor;
        *(s32*)&poly->u0 = *(s32*)&prim->u0;
        *(s32*)&poly->u1 = ((*(u32*)&prim->u1 & 0x1FFFFF) | (var_a2 << 16)); // Maybe `field_4` is bitfield.
        *(u16*)&poly->u2 = *(u16*)&prim->u2;
        *(u16*)&poly->u3 = *(u16*)&prim->u3;

        setlen(poly, 9);

        if (arg0 == 1)
        {
            tag0 = tag1;
        }
        else
        {
            tag0 = &tag[var_t2 >> (otShift + 2)];
        }

        addPrim(tag0, poly);
        poly++;
    }

    GsOUT_PACKET_P = (PACKET*)poly;
}

void func_8005A21C(s_ModelInfo* modelInfo, GsOT_TAG* otTag, s32 otShift, MATRIX* viewMat) // 0x8005A21C
{
    s16               var_v1;
    u32               normalOffset;
    u32               vertOffset;
    s_ModelHeader*    modelHdr;
    s_MeshHeader*     curMeshHdr;
    s_GteScratchData* scratchData;

    scratchData = PSX_SCRATCH_ADDR(0);

    if (g_WorldEnvWork.isFogEnabled)
    {
        if (viewMat->t[2] < (1 << g_WorldEnvWork.fog.depthShift))
        {
            var_v1 = Q12(1.0f) - (g_WorldEnvWork.fogRamp[(s32)(viewMat->t[2] << 7) >> g_WorldEnvWork.fog.depthShift] << 4);
        }
        else
        {
            var_v1 = 1 << 4;
        }
    }
    else
    {
        var_v1 = 256 << 4;
    }

    switch (g_WorldEnvWork.field_0)
    {
        case UnkGfxEnum_0:
            func_8005A42C(scratchData, var_v1);
            break;

        case UnkGfxEnum_1:
            func_8005A478(scratchData, var_v1);
            SetColorMatrix(&g_WorldEnvWork.colorMat);
            gte_lddqa(g_WorldEnvWork.light.field_0);
            gte_lddqb_0();
            break;

        case UnkGfxEnum_2:
            func_8005A838(scratchData, var_v1);
            SetColorMatrix(&g_WorldEnvWork.colorMat);
            gte_lddqa(g_WorldEnvWork.light.field_0);
            gte_lddqb_0();
            break;
    }

    modelHdr     = modelInfo->modelHdr;
    vertOffset   = modelHdr->vertexOffset;
    normalOffset = modelHdr->normalOffset;

    for (curMeshHdr = modelHdr->meshHdrs; curMeshHdr < &modelHdr->meshHdrs[modelHdr->meshCount]; curMeshHdr++)
    {
        func_8005A900(curMeshHdr, vertOffset, scratchData, viewMat);

        if (g_WorldEnvWork.field_0 != UnkGfxEnum_0)
        {
            func_8005AA08(curMeshHdr, normalOffset, scratchData);
        }

        func_8005AC50(curMeshHdr, scratchData, otTag, otShift);
    }
}

void func_8005A42C(s_GteScratchData* scratchData, q19_12 alpha) // 0x8005A42C
{
    q19_12 invAlpha = Q12(1.0f) - Q12_MULT(alpha, g_WorldEnvWork.field_20);

    gte_lddp(invAlpha);
    gte_ldrgb(&g_WorldEnvWork.worldTintColor);
    gte_dpcs();
    gte_strgb(&scratchData->field_3D8);
}

void func_8005A478(s_GteScratchData* scratchData, q19_12 alpha) // 0x8005A478
{
    s32 geomOffsetX;
    s32 geomOffsetY;
    s32 offsetX;
    s32 offsetY;
    s32 offsetZ;
    s32 temp_a0;
    s16 geomScreen;
    s32 temp_v1;
    s32 var_a1;
    s32 var_a2;
    s32 var_s0;
    s32 var_t1;
    s32 var_v1;
    s32 negOffsetX;
    s32 negOffsetY;
    s32 negOffsetZ;

    ReadGeomOffset(&geomOffsetX, &geomOffsetY);
    geomScreen = ReadGeomScreen();
    SetGeomOffset(Q8(-4.0f), Q8(-4.0f));
    SetGeomScreen(16);

    offsetX = g_WorldEnvWork.light.field_30.vx;
    offsetY = g_WorldEnvWork.light.field_30.vy;
    offsetZ = g_WorldEnvWork.light.field_30.vz;

    var_t1 = Math_Vector3MagCalc(offsetX, offsetY, offsetZ);
    if (var_t1 == 0)
    {
        var_t1 = 1;
    }

    negOffsetX = -offsetX;
    negOffsetY = -offsetY;
    negOffsetZ = -offsetZ;

    *(u32*)&scratchData->rotMatrix_3E4[0][0] = *(u32*)&g_WorldEnvWork.light.field_28;
    scratchData->rotMatrix_3E4[0][2]         = g_WorldEnvWork.light.field_28.vz;

    scratchData->rotMatrix_3E4[1][0] = Q12(negOffsetX) / var_t1;
    scratchData->rotMatrix_3E4[1][1] = Q12(negOffsetY) / var_t1;
    scratchData->rotMatrix_3E4[1][2] = Q12(negOffsetZ) / var_t1;

    scratchData->rotMatrix_3E4[2][0] = negOffsetX;
    scratchData->rotMatrix_3E4[2][1] = negOffsetY;
    scratchData->rotMatrix_3E4[2][2] = negOffsetZ;

    gte_SetRotMatrix(scratchData->rotMatrix_3E4);
    gte_SetVector0(&scratchData->rotMatrix_3E4[2][0]);
    gte_ldtr_0();
    gte_rtps();
    gte_stsxy(&scratchData->screenPos_3DC);
    gte_stdp(&scratchData->depthP_3E0);

    scratchData->screenPos_3DC.vx += 1024; // TODO: Q8?
    scratchData->screenPos_3DC.vy += 1024;

    var_s0  = (scratchData->screenPos_3DC.vx * scratchData->screenPos_3DC.vy) +
              (scratchData->screenPos_3DC.vy * (scratchData->depthP_3E0 >> 4));
    var_s0 >>= 5;
    var_s0  -= 16;
    if (var_s0 < 0)
    {
        var_s0 = 0;
    }

    var_v1 = (scratchData->screenPos_3DC.vx >> 1) + (scratchData->depthP_3E0 >> 2);
    if (var_v1 > 48)
    {
        var_v1 = 48;
    }

    var_s0  += var_v1;
    var_s0 >>= 1;
    if (g_WorldEnvWork.field_3 < var_s0)
    {
        var_s0 = g_WorldEnvWork.field_3;
    }

    var_s0 = Q12_MULT(var_s0, alpha);

    if (var_s0 < 0)
    {
        var_s0 = 0;
    }

    if ((var_s0 >> 2) > 40)
    {
        var_a2 = var_s0 - 40;
    }
    else
    {
        var_a2 = var_s0 - (var_s0 >> 2);
    }

    temp_a0 = (scratchData->rotMatrix_3E4[1][0] * var_a2) >> 7;
    temp_a0 = CLAMP(temp_a0, Q12(-1.4f), Q12(1.4f));

    temp_v1 = (scratchData->rotMatrix_3E4[1][1] * var_a2) >> 7;
    temp_v1 = CLAMP(temp_v1, Q12(-1.4f), Q12(1.4f));

    gte_SetLightSourceXY(temp_a0, temp_v1);

    temp_a0 = (scratchData->rotMatrix_3E4[1][2] * var_a2) >> 7;
    temp_a0 = CLAMP(temp_a0, Q12(-1.4f), Q12(1.4f));

    gte_SetLightSourceZ(temp_a0);

    SetGeomOffset(geomOffsetX, geomOffsetY);
    SetGeomScreen(geomScreen);

    var_a1 = 40;

    if ((var_s0 >> 2) <= 40)
    {
        var_a1 = (var_s0 >> 2);
    }

    if (var_a1 > 64)
    {
        var_a1 = 64;
    }

    SetBackColor(Q12_MULT(g_WorldEnvWork.field_24.r + ((g_WorldEnvWork.worldTintColor.r * var_a1) >> 7), alpha),
                 Q12_MULT(g_WorldEnvWork.field_24.g + ((g_WorldEnvWork.worldTintColor.g * var_a1) >> 7), alpha),
                 Q12_MULT(g_WorldEnvWork.field_24.b + ((g_WorldEnvWork.worldTintColor.b * var_a1) >> 7), alpha));
}

void func_8005A838(s_GteScratchData* scratchData, q19_12 scale) // 0x8005A838
{
    SVECTOR3 color;

    color.vx = DIV_FAST(Q12_MULT(g_WorldEnvWork.light.field_28.vx, scale), 2);
    color.vy = DIV_FAST(Q12_MULT(g_WorldEnvWork.light.field_28.vy, scale), 2);
    color.vz = DIV_FAST(Q12_MULT(g_WorldEnvWork.light.field_28.vz, scale), 2);
    gte_SetLightSVector(&color);

    SetBackColor(Q12_MULT(g_WorldEnvWork.field_24.r, scale),
                 Q12_MULT(g_WorldEnvWork.field_24.g, scale),
                 Q12_MULT(g_WorldEnvWork.field_24.b, scale));
}

void func_8005A900(s_MeshHeader* meshHdr, s32 offset, s_GteScratchData* scratchData, MATRIX* viewMat) // 0x8005A900
{
    DVECTOR* inXy;  // Model-space XY input
    u16*     inZ;   // Model-space Z input
    DVECTOR* outXy; // Projected XY output buffer
    u16*     outZ;  // Projected Z output buffer

    if (meshHdr->vertexCount == 0)
    {
        return;
    }

    SetRotMatrix(viewMat);
    SetTransMatrix(viewMat);

    outXy = &scratchData->screenXy_0[offset];
    outZ  = &scratchData->screenZ_168[offset];

    inXy = meshHdr->verticesXy;
    inZ  = meshHdr->verticesZ;

    while (outXy < &scratchData->screenXy_0[meshHdr->vertexCount + offset])
    {
        // Nearly same as `gte_RotTransPers3`, processes 3 vertices per iteration.
        gte_LoadVector0_1_2_XYZ(inXy, inZ);
        gte_rtpt();
        gte_FetchScreen0_1_2_XYZ(outXy, outZ);

        outXy += 3;
        outZ  += 3;
        inXy  += 3;
        inZ   += 3;
    }
}

u8 func_8005AA08(s_MeshHeader* meshHdr, s32 arg1, s_GteScratchData2* scratchData) // 0x8005AA08
{
    // Same as `gte_strgb3`, but takes `VECTOR3` pointer to store results.
    // Not sure why this was needed, the func that uses it also ends up calling the normal `gte_strgb3` too.
    #define gte_strgb3_vec( r0 ) __asm__ volatile ( \
        "swc2    $20, 0( %0 );"                     \
        "swc2    $21, 4( %0 );"                     \
        "swc2    $22, 8( %0 )"                      \
        :                                           \
        : "r"( r0 )                                 \
        : "memory" )

    CVECTOR   color;
    s_Normal* normals;
    VECTOR3*  points;

    if (meshHdr->normalCount == 0)
    {
        return;
    }

    color.cd = 0;
    gte_ldrgb(&color);

    normals = meshHdr->normals;
    *(u32*)&scratchData->u.normal.field_3DC = *(u32*)&normals[0];
    scratchData->u.normal.field_3E0[0].vx = scratchData->u.normal.field_3DC.nx << 5;
    scratchData->u.normal.field_3E0[0].vy = scratchData->u.normal.field_3DC.ny << 5;
    scratchData->u.normal.field_3E0[0].vz = scratchData->u.normal.field_3DC.nz << 5;

    *(u32*)&scratchData->u.normal.field_3DC = *(u32*)&normals[1];
    scratchData->u.normal.field_3E0[1].vx = scratchData->u.normal.field_3DC.nx << 5;
    scratchData->u.normal.field_3E0[1].vy = scratchData->u.normal.field_3DC.ny << 5;
    scratchData->u.normal.field_3E0[1].vz = scratchData->u.normal.field_3DC.nz << 5;

    *(u32*)&scratchData->u.normal.field_3DC = *(u32*)&normals[2];
    normals += 3;
    scratchData->u.normal.field_3E0[2].vx = scratchData->u.normal.field_3DC.nx << 5;
    scratchData->u.normal.field_3E0[2].vy = scratchData->u.normal.field_3DC.ny << 5;
    scratchData->u.normal.field_3E0[2].vz = scratchData->u.normal.field_3DC.nz << 5;

    points = &scratchData->field_21C[arg1];

    gte_ldv3c(scratchData->u.normal.field_3E0);
    gte_nct();

    while (normals < &meshHdr->normals[meshHdr->normalCount])
    {
        *(u32*)&scratchData->u.normal.field_3DC = *(u32*)&normals[0];
        scratchData->u.normal.field_3E0[0].vx = scratchData->u.normal.field_3DC.nx << 5;
        scratchData->u.normal.field_3E0[0].vy = scratchData->u.normal.field_3DC.ny << 5;
        scratchData->u.normal.field_3E0[0].vz = scratchData->u.normal.field_3DC.nz << 5;

        *(u32*)&scratchData->u.normal.field_3DC = *(u32*)&normals[1];
        scratchData->u.normal.field_3E0[1].vx = scratchData->u.normal.field_3DC.nx << 5;
        scratchData->u.normal.field_3E0[1].vy = scratchData->u.normal.field_3DC.ny << 5;
        scratchData->u.normal.field_3E0[1].vz = scratchData->u.normal.field_3DC.nz << 5;

        *(u32*)&scratchData->u.normal.field_3DC = *(u32*)&normals[2];
        normals += 3;
        scratchData->u.normal.field_3E0[2].vx = scratchData->u.normal.field_3DC.nx << 5;
        scratchData->u.normal.field_3E0[2].vy = scratchData->u.normal.field_3DC.ny << 5;
        scratchData->u.normal.field_3E0[2].vz = scratchData->u.normal.field_3DC.nz << 5;

        gte_strgb3_vec(points); // Store result of previous `gte_nct` call.
        points++;
        gte_ldv3c(scratchData->u.normal.field_3E0);
        gte_nct();
    }

    gte_strgb3(&points->vx, &points->vy, &points->vz); // Store result from final `gte_nct`.
}

void func_8005AC50(s_MeshHeader* meshHdr, s_GteScratchData2* scratchData, GsOT_TAG* ot, bool otShift) // 0x8005AC50
{
    typedef union
    {
        POLY_GT3* gt3;
        POLY_GT4* gt4;
        PACKET*   packet;
    } u_poly;

    s32          r4;
    s32          sp4;
    s16          temp_v1;
    s32          temp_a0;
    s32          temp_t4;
    s32          temp_t4_2;
    s32          temp_v0;
    s32          temp_v0_2;
    s32          var_t9;
    s32          var_v0;
    s32          var_v1;
    s32          var_a3;
    GsOT_TAG*    localOt;
    s_Primitive* prim;
    u16*         var_t5;
    u_poly       poly;

    var_a3              = g_WorldEnvWork.field_0;
    scratchData->u.s_1.field_8 = g_WorldEnvWork.clutY << 16;

    temp_a0 = 0x79C << (otShift + 2);
    var_t9  = g_WorldEnvWork.isFogEnabled ? MIN(temp_a0, g_WorldEnvWork.fog.farDistance) : temp_a0;

    for (prim = meshHdr->primitives, poly.packet = GsOUT_PACKET_P; prim < &meshHdr->primitives[meshHdr->primitiveCount]; prim++)
    {
        *(s32*)&scratchData->u.s_1.field_0 = *(s32*)&prim->faceIdxs;
        *(s32*)&scratchData->u.s_1.field_4 = *(s32*)&prim->field_10;

        if (scratchData->u.s_1.field_3 == 0xFF)
        {
            temp_t4 = (scratchData->screenZ_168[scratchData->u.s_1.field_0] + scratchData->screenZ_168[scratchData->u.s_1.field_1] +
                       scratchData->screenZ_168[scratchData->u.s_1.field_2] + scratchData->screenZ_168[scratchData->u.s_1.field_2]) >> 2;

            if (temp_t4 <= 0 || var_t9 < temp_t4)
            {
                continue;
            }

            gte_NormalClip(*(s32*)&scratchData->screenXy_0[scratchData->u.s_1.field_0],
                           *(s32*)&scratchData->screenXy_0[scratchData->u.s_1.field_1],
                           *(s32*)&scratchData->screenXy_0[scratchData->u.s_1.field_2],
                           &r4);

            if (r4 <= 0)
            {
                continue;
            }

            *(s32*)&poly.gt3->x0 = *(s32*)&scratchData->screenXy_0[scratchData->u.s_1.field_0];
            *(s32*)&poly.gt3->x1 = *(s32*)&scratchData->screenXy_0[scratchData->u.s_1.field_1];
            *(s32*)&poly.gt3->x2 = *(s32*)&scratchData->screenXy_0[scratchData->u.s_1.field_2];

            if (var_a3 != UnkGfxEnum_0)
            {
                *(s32*)&poly.gt3->r0 = *(s32*)&scratchData->field_21C[scratchData->u.s_1.field_4];
                *(s32*)&poly.gt3->r1 = *(s32*)&scratchData->field_21C[scratchData->u.s_1.field_5];
                *(s32*)&poly.gt3->r2 = *(s32*)&scratchData->field_21C[scratchData->u.s_1.field_6];
            }
            else
            {
                *(s32*)&poly.gt3->r0 = *(s32*)&poly.gt3->r1 = *(s32*)&poly.gt3->r2 = *(s32*)&scratchData->field_3D8;
            }

            poly.gt3->code = ((prim->bits1.flags >> 15) * 2) | 0x34;

            *(s32*)&poly.gt3->u0 = *(s32*)&prim->u0 + scratchData->u.s_1.field_8;
            *(s32*)&poly.gt3->u1 = *(s32*)&prim->u1 & 0xFFFFFF;
            *(u16*)&poly.gt3->u2 = *(u16*)&prim->u2;

            setlen(poly.gt3, 9);

            addPrim(&ot[(temp_t4 >> otShift) >> 2], poly.gt3);
            poly.gt3++;
        }
        else
        {
            temp_t4 = (scratchData->screenZ_168[scratchData->u.s_1.field_0] + scratchData->screenZ_168[scratchData->u.s_1.field_1] +
                       scratchData->screenZ_168[scratchData->u.s_1.field_2] + scratchData->screenZ_168[scratchData->u.s_1.field_3]) >> 2;

            if (temp_t4 <= 0 || var_t9 < temp_t4)
            {
                continue;
            }

            gte_ldsxy3(*(s32*)&scratchData->screenXy_0[scratchData->u.s_1.field_0],
                       *(s32*)&scratchData->screenXy_0[scratchData->u.s_1.field_1],
                       *(s32*)&scratchData->screenXy_0[scratchData->u.s_1.field_2]);
            gte_nclip();
            gte_stopz(&sp4);

            if (sp4 <= 0)
            {
                gte_ldsxy0(*(s32*)&scratchData->screenXy_0[scratchData->u.s_1.field_3]);
                gte_nclip();
                gte_stopz(&sp4);

                if (sp4 >= 0)
                {
                    continue;
                }
            }

            *(s32*)&poly.gt4->x0 = *(s32*)&scratchData->screenXy_0[scratchData->u.s_1.field_0];
            *(s32*)&poly.gt4->x1 = *(s32*)&scratchData->screenXy_0[scratchData->u.s_1.field_1];
            *(s32*)&poly.gt4->x2 = *(s32*)&scratchData->screenXy_0[scratchData->u.s_1.field_2];
            *(s32*)&poly.gt4->x3 = *(s32*)&scratchData->screenXy_0[scratchData->u.s_1.field_3];

            if (var_a3 != UnkGfxEnum_0)
            {
                *(s32*)&poly.gt4->r0 = *(s32*)&scratchData->field_21C[scratchData->u.s_1.field_4];
                *(s32*)&poly.gt4->r1 = *(s32*)&scratchData->field_21C[scratchData->u.s_1.field_5];
                *(s32*)&poly.gt4->r2 = *(s32*)&scratchData->field_21C[scratchData->u.s_1.field_6];
                *(s32*)&poly.gt4->r3 = *(s32*)&scratchData->field_21C[scratchData->u.s_1.field_7];
            }
            else
            {
                *(s32*)&poly.gt4->r0 =
                *(s32*)&poly.gt4->r1 =
                *(s32*)&poly.gt4->r2 =
                *(s32*)&poly.gt4->r3 = *(s32*)&scratchData->field_3D8;
            }

            poly.gt4->code = ((prim->bits1.flags >> 15) * 2) | 0x3C;

            *(s32*)&poly.gt4->u0 = *(s32*)&prim->u0 + scratchData->u.s_1.field_8;
            *(s32*)&poly.gt4->u1 = *(s32*)&prim->u1 & 0xFFFFFF;
            *(u16*)&poly.gt4->u2 = *(u16*)&prim->u2;
            *(u16*)&poly.gt4->u3 = *(u16*)&prim->u3;

            setlen(poly.gt4, 12);

            addPrim(&ot[(temp_t4 >> otShift) >> 2], poly.gt4);
            poly.gt4++;
        }
    }

    GsOUT_PACKET_P = poly.packet;
}
