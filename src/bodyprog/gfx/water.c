#include "bodyprog/game.h"
#include "inline_no_dmpsx.h"

#include <psyq/libpad.h>
#include <psyq/strings.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/gfx/water.h"
#include "bodyprog/screen/data.h"
#include "bodyprog/screen/draw.h"
#include "bodyprog/items/item_screens.h"
#include "bodyprog/math/math.h"
#include "bodyprog/sound/sound_system.h"
#include "main/fsqueue.h"

// VRAM position of the CLUT rows used by `func_8008E5B4` and `func_8008E794`.
#if VERSION_REGION_IS(PAL)
    #define WATER_CLUT_X 928
    #define WATER_CLUT_Y 432
#else
    #define WATER_CLUT_X 224
    #define WATER_CLUT_Y 0
#endif

LensFlareWork light_flare_work;

void func_8008D41C(void) // 0x8008D41C
{
    light_flare_work.flare_inhibit_f = 0;
    light_flare_work.water_inhibit_f = 0;
    light_flare_work.lfl_sync_draw_func_exec_f = 0;
    light_flare_work.now_l_eff_rate = 0;
}

void func_8008D438(void) // 0x8008D438
{
    light_flare_work.flare_inhibit_f = 1;
}

void func_8008D448(void) // 0x8008D448
{
    light_flare_work.flare_inhibit_f = 0;
}

void func_8008D454(void) // 0x8008D454
{
    light_flare_work.water_inhibit_f = 1;
}

void func_8008D464(void) // 0x8008D464
{
    light_flare_work.water_inhibit_f = 0;
}

void func_8008D470(q3_12 light_intensity, SVECTOR* rot, VECTOR3* pos, s_WaterZone* waterZones) // 0x8008D470
{
    s32          posY;
    s_WaterZone* waterZone;

    if (light_flare_work.flare_inhibit_f == 0)
    {
        func_800410D8(&light_flare_work.scr_l_pos, &light_flare_work.scr_l_ang_xy, &light_flare_work.scr_l_ang_z, rot, pos);

        if ((ReadGeomScreen() >> 1) < light_flare_work.scr_l_pos.vz)
        {
            light_flare_work.lfl_sync_draw_func_exec_f = 1;
            light_flare_work.tgt_l_eff_rate = shLensFlareMakeEffectTargetRate(light_intensity, light_flare_work.scr_l_pos.vz, light_flare_work.scr_l_ang_z);
            func_8008D5A0(&light_flare_work.scr_l_pos, light_flare_work.scr_l_ang_z);
        }
        else
        {
            return;
        }
    }

    if (light_flare_work.water_inhibit_f == 0)
    {
        waterZone = Map_WaterZoneGet(Q12_TO_Q4(pos->vx), Q12_TO_Q4(pos->vz), waterZones);
        if (waterZone != NULL)
        {
            func_8008E5B4();

            if (waterZone->isEnabled == true)
            {
                posY = Q4_TO_Q12(waterZone->illumination);
                func_8008E794(pos, light_flare_work.scr_l_ang_z, posY);
                func_8008EA68(rot, pos, posY);
            }
        }
    }
}

void func_8008D5A0(VECTOR3* arg0, s16 arg1) // 0x8008D5A0
{
    RECT      sp10;
    bool      cond;
    DR_MOVE*  packet;
    TILE*     tile;
    GsOT_TAG* ot;

    packet         = GsOUT_PACKET_P;
    tile           = GsOUT_PACKET_P + 24;
    GsOUT_PACKET_P = GsOUT_PACKET_P + 40;
    ot             = &g_OrderingTable0[g_ActiveBufferIdx].org[(arg0->vz + 2) >> 3];

    cond = false;
    if (arg0->vx >= (-g_GameWork.gsScreenWidth >> 1) && (g_GameWork.gsScreenWidth >> 1) >= arg0->vx)
    {
        if (arg0->vy >= (-g_GameWork.gsScreenHeight >> 1) && (g_GameWork.gsScreenHeight >> 1) >= arg0->vy &&
            arg0->vz >= 0)
        {
            cond = arg1 > 0x354;
        }
    }

    if (cond)
    {
        SetTile(tile);
        tile->r0 = 0xFF;
        tile->g0 = 0xFF;
        tile->b0 = 0xFF;
        tile->x0 = arg0->vx;
        tile->y0 = arg0->vy;
        tile->w  = 1;
        tile->h  = 1;
        AddPrim(ot, tile);

        sp10.x = arg0->vx + (g_GameWork.gsScreenWidth / 2);
        sp10.y = arg0->vy + (g_GameWork.gsScreenHeight / 2);

#if VERSION_REGION_IS(PAL) 
        if (g_ActiveBufferIdx != 1)
        {
            sp10.y += 256;
        }
#else
        if (g_ActiveBufferIdx == 1)
        {
            sp10.y += 32;
        }
        else
        {
            sp10.y += 256;
        }
#endif
    }
    else
    {
        sp10.x = 784;
        sp10.y = 113;
    }

    sp10.h = 8;
    sp10.w = 8;

    SetDrawMove(packet, &sp10, (g_ActiveBufferIdx * 8) + 784, 112);
    AddPrim(g_OrderingTable0[g_ActiveBufferIdx].org + 5, packet);
}

void shLensFlareDraw(void) // 0x8008D78C
{
    s32 var0;
    s32 var1;

    if (light_flare_work.lfl_sync_draw_func_exec_f == 0)
    {
        return;
    }

    light_flare_work.lfl_sync_draw_func_exec_f = 0;

    if (light_flare_work.flare_inhibit_f != 0)
    {
        return;
    }

    var1 = shLensFlareLightCenterIsVisible();
    if (var1 != 0)
    {
        var0 = light_flare_work.tgt_l_eff_rate - light_flare_work.now_l_eff_rate;
    }
    else
    {
        var0 = -light_flare_work.now_l_eff_rate;
    }

    light_flare_work.now_l_eff_rate += var0 >> 1;

    if (vcRetCamMvSmoothF() == VC_MV_CHASE)
    {
        light_flare_work.now_l_eff_rate = 0;
        var1               = 0;
    }

    shLensFlareEffect_Draw(var1, light_flare_work.now_l_eff_rate, &light_flare_work.scr_l_pos, light_flare_work.scr_l_ang_xy, light_flare_work.scr_l_ang_z);
}

s32 shLensFlareLightCenterIsVisible(void) // 0x8008D850
{
    s16  rectX;
    RECT rect;
    u16  pixels[2]; // `StoreImage2` copies whole 32-bit words, so the 1 pixel needed comes with a 2nd.

    rectX = 784;
    if (g_ActiveBufferIdx == 0)
    {
        rectX = 792;
    }

    rect.y = 112;
    rect.w = 2;
    rect.x = rectX;
    rect.h = 1;

    DrawSync(SyncMode_Wait);
    StoreImage2(&rect, (u_long*)pixels);
    DrawSync(SyncMode_Wait);

    // Still white (ignoring the mask bit) if nothing was drawn over the light center.
    return (pixels[0] & 0x7FFF) == 0x7FFF;
}

q19_12 shLensFlareMakeEffectTargetRate(q3_12 light_eff_pow, q19_12 x1, q19_12 x2) // 0x8008D8C0
{
    q19_12 temp0;
    q19_12 temp1;
    q19_12 res;

    static s32 Y_ARRAY_0[2] = {
        Q12(0.0f),
        Q12(1.0f)
    };

    static s32 Y_ARRAY_1[7] = {
        Q12(4.0f),
        Q12(1.3f),
        Q12(0.9f),
        Q12(0.7f),
        Q12(0.5f),
        Q12(0.35f),
        Q12(0.2f)
    };

    static s32 Y_ARRAY_2[7] = {
        Q12(0.15f),
        Q12(0.2f),
        Q12(0.25f),
        Q12(0.3f),
        Q12(0.35f),
        Q12(0.5f),
        Q12(1.3f)
    };

    temp0 = vwOresenHokan(&Y_ARRAY_0, ARRAY_SIZE(Y_ARRAY_0), light_eff_pow, 0, Q8(16.0f));
    temp1 = vwOresenHokan(&Y_ARRAY_1, ARRAY_SIZE(Y_ARRAY_1), x1, Q8(0.8f), Q8(13.0f));
    res   = Q12_MULT(vwOresenHokan(&Y_ARRAY_2, ARRAY_SIZE(Y_ARRAY_2), x2, Q8(3.335f), Q8(7.425f)), // TODO: Yucky floats, maybe these aren't distances?
                     Q12_MULT(temp0, temp1));

    return (res > Q8(24.0f)) ? Q8(24.0f) : res;
}

// Angles used by `shLensFlareEffect_Draw`.
q3_12 D_800AFD7C[] = {
    Q12_ANGLE(-175.0f), 0xF889, 0xFA39, 0xFAE4,
    0xFD56, 0xFDC8, 0xFF56, 0xFFEA,
    Q12_ANGLE(5.0f), Q12_ANGLE(35.0f), Q12_ANGLE(40.0f), Q12_ANGLE(60.0f),
    Q12_ANGLE(110.0f), Q12_ANGLE(142.0f), Q12_ANGLE(147.0f), Q12_ANGLE(155.0f)
};

s_FsImageDesc img0 = { .tPage = { 0, 13 } }; // 0x800AFD9C

void shLensFlareEffect_Draw(s32 center_visible_f, q19_12 arg1, VECTOR3* arg2, s32 arg3, s32 arg4) // 0x8008D990
{
    // VRAM position of the CLUT rows used below.
#if VERSION_REGION_IS(PAL)
    #define CLUT_X 944
    #define CLUT_Y 496
#else
    #define CLUT_X 192
    #define CLUT_Y 0
#endif

    s32       sp20;
    s32       sp24;
    s32       sp28;
    s32       sp2C;
    q3_12     curAngle;
    s32       temp_a0;
    s32       temp_a0_4;
    s32       temp_a0_5;
    s32       temp_a1;
    s32       temp_a1_2;
    s32       temp_a1_5;
    s32       temp_a2;
    s32       temp_a2_3;
    s32       temp_a3_2;
    q3_12     cosCurAngle;
    s32       temp_s0_6;
    q3_12     sinCurAngle;
    s32       temp_s2_2;
    s32       temp_t0;
    s32       temp_v0_11;
    s32       temp_v0_13;
    s32       temp_v0_3;
    s32       temp_v0_5;
    s32       temp_v0_9;
    s32       temp_v1_10;
    s32       temp_v1_12;
    s32       temp_v1_4;
    s32       temp_v1_6;
    s32       temp_v1_8;
    s16       var_s3;
    s32       i;
    POLY_FT4* poly;
    GsOT_TAG* temp_s2;
    GsOT_TAG* temp_s7;

    DVECTOR D_8002B2BC[] = {
        { -4, 0 },
        { 43, -47 },
        { 43, 47 },
        { 90, 0 }
    };

    // TODO: 512 is probably a screen constant.
    if (arg2->vx < ((-g_GameWork.gsScreenWidth  >> 1) - 512) || ((g_GameWork.gsScreenWidth  >> 1) + 512) < arg2->vx ||
        arg2->vy < ((-g_GameWork.gsScreenHeight >> 1) - 512) || ((g_GameWork.gsScreenHeight >> 1) + 512) < arg2->vy)
    {
        return;
    }

    sinCurAngle   = Q12_MULT(Math_Sin(arg4), Math_Cos(arg3));
    cosCurAngle = Q12_MULT(Math_Sin(arg4), Math_Sin(arg3));

    poly = (POLY_FT4*)GsOUT_PACKET_P;

    temp_s7 = &g_OrderingTable0[g_ActiveBufferIdx].org[1];
    temp_s2 = &g_OrderingTable0[g_ActiveBufferIdx].org[arg2->vz >> 3];

    SetPolyFT4(poly);

    poly->tpage = 0x2C;
    poly->clut  = getClut(CLUT_X, CLUT_Y + 6);

    setUV4(poly, 0, 0, 63, 0, 0, 63, 63, 63);

    temp_a1 = (arg1 + Q12(3.0f)) >> 2;

    setRGB0(poly, Q12_MULT(temp_a1, 40), Q12_MULT(temp_a1, 40), Q12_MULT(temp_a1, 40));
    setSemiTrans(poly, 1);

    temp_a2   = arg2->vx + Q12_MULT(sinCurAngle, 5);
    temp_a0   = Q12_MULT(temp_a1, 30);
    temp_a1_2 = arg2->vy + Q12_MULT(cosCurAngle, 5);

    setXY4(poly,
           temp_a2 - temp_a0, temp_a1_2 - temp_a0,
           temp_a2 + temp_a0, temp_a1_2 - temp_a0,
           temp_a2 - temp_a0, temp_a1_2 + temp_a0,
           temp_a2 + temp_a0, temp_a1_2 + temp_a0);

    if (center_visible_f)
    {
        AddPrim(temp_s7, poly);
    }
    else
    {
        if (arg4 > 0x400)
        {
            AddPrim(temp_s2, poly);
        }
        else
        {
            AddPrim(temp_s2 + 7, poly);
        }
    }

    poly++;

    SetPolyFT4(poly);

    poly->tpage = 44;
    poly->clut  = getClut(CLUT_X, CLUT_Y + 6);

    setUV4(poly, 0, 0, 63, 0, 0, 63, 63, 63);
    setRGB0(poly, 0x10, 0x10, 0x10);
    setSemiTrans(poly, 1);

    temp_v0_3 = arg2->vx;
    temp_v1_4 = arg2->vy;

    setXY4(poly,
           temp_v0_3 - 11, temp_v1_4 - 11,
           temp_v0_3 + 11, temp_v1_4 - 11,
           temp_v0_3 - 11, temp_v1_4 + 11,
           temp_v0_3 + 11, temp_v1_4 + 11);

    if (center_visible_f)
    {
        AddPrim(temp_s7, poly);
    }
    else
    {
        if (arg4 > 0x400)
        {
            AddPrim(temp_s2, poly);
        }
        else
        {
            AddPrim(temp_s2 + 7, poly);
        }
    }

    poly++;

    SetPolyFT4(poly);
    setSemiTrans(poly, 1);

    temp_v0_5 = Q12_MULT(MIN(arg1 * 2, Q12(1.0f)), 48);
    setRGB0(poly, temp_v0_5, temp_v0_5, temp_v0_5);

    poly->tpage = 44;
    poly->clut  = getClut(CLUT_X, CLUT_Y + 6);

    setUV4(poly, 0, 0, 63, 0, 0, 63, 63, 63);

    temp_t0   = arg2->vx + FP_MULTIPLY(sinCurAngle, 9, Q12_SHIFT - 1);
    temp_a2_3 = Q12_MULT((arg1 + Q12(3.0f)) >> 2, 48);
    temp_a3_2 = arg2->vy + FP_MULTIPLY(cosCurAngle, 9, Q12_SHIFT - 1);

    setXY4(poly,
           temp_t0 - temp_a2_3, temp_a3_2 - temp_a2_3,
           temp_t0 + temp_a2_3, temp_a3_2 - temp_a2_3,
           temp_t0 - temp_a2_3, temp_a3_2 + temp_a2_3,
           temp_t0 + temp_a2_3, temp_a3_2 + temp_a2_3);

    AddPrim(temp_s7, poly);
    poly++;

    if (!arg1)
    {
        GsOUT_PACKET_P = (PACKET*)poly;
        return;
    }

    sp20 = ((-arg4 >> 1) + ((arg2->vx + arg2->vy) * 4)) + 0x2AA;
    sp24 = (arg2->vz * 4) + ((arg2->vx + arg2->vy) * 16);

    for (i = 0; i < 16; i++)
    {
        curAngle = D_800AFD7C[i] + sp20;

        var_s3 = Q12_MULT((Math_Cos(curAngle - arg3) + Q12(1.7f)), (Math_Cos((curAngle * 12) + sp24) + Q12(1.0f)) >> 1);
        var_s3 = MIN(var_s3, Q12(1.5f));

        sinCurAngle = Math_Sin(curAngle);
        cosCurAngle = Math_Cos(curAngle);

        SetPolyFT4(poly);

        poly->tpage = 0x2C;
        poly->clut  = getClut(CLUT_X, CLUT_Y + 5);

        setUV4(poly, 0x20, 0x40, 0x3F, 0x40, 0x20, 0x5F, 0x3F, 0x5F);
        setSemiTrans(poly, 1);

        temp_v0_9 = (arg1 * var_s3) >> 18;
        setRGB0(poly, temp_v0_9, temp_v0_9, temp_v0_9);

        temp_a0_4 = arg2->vy;
        temp_v1_6 = arg2->vx;

        setXY4(poly,
               temp_v1_6 + FP_FROM(D_8002B2BC[0].vx * cosCurAngle - D_8002B2BC[0].vy * sinCurAngle, Q12_SHIFT),
               temp_a0_4 + FP_FROM(D_8002B2BC[0].vx * sinCurAngle + D_8002B2BC[0].vy * cosCurAngle, Q12_SHIFT),
               temp_v1_6 + FP_FROM(D_8002B2BC[1].vx * cosCurAngle - D_8002B2BC[1].vy * sinCurAngle, Q12_SHIFT),
               temp_a0_4 + FP_FROM(D_8002B2BC[1].vx * sinCurAngle + D_8002B2BC[1].vy * cosCurAngle, Q12_SHIFT),
               temp_v1_6 + FP_FROM(D_8002B2BC[2].vx * cosCurAngle - D_8002B2BC[2].vy * sinCurAngle, Q12_SHIFT),
               temp_a0_4 + FP_FROM(D_8002B2BC[2].vx * sinCurAngle + D_8002B2BC[2].vy * cosCurAngle, Q12_SHIFT),
               temp_v1_6 + FP_FROM(D_8002B2BC[3].vx * cosCurAngle - D_8002B2BC[3].vy * sinCurAngle, Q12_SHIFT),
               temp_a0_4 + FP_FROM(D_8002B2BC[3].vx * sinCurAngle + D_8002B2BC[3].vy * cosCurAngle, Q12_SHIFT));

        addPrim(temp_s7, poly);
        poly++;
    }

    sp28 = 0x3F;
    sp2C = 0x40;

    SetPolyFT4(poly);

    setRGB0(poly, Q12_MULT(arg1, 32), Q12_MULT(arg1, 0x30), Q12_MULT(arg1, 32));

    poly->tpage = 44;
    poly->clut  = getClut(CLUT_X, CLUT_Y + 7);

    setUV4(poly, 0x40, 0, 0x7F, 0, 0x40, 0x3F, 0x7F, 0x3F);
    setSemiTrans(poly, 1);

    temp_s2_2 = (arg1 + 0x3000) >> 2;
    temp_a1_5 = Q12_MULT(temp_s2_2, 0x10);
    temp_a0_5 = Q12_MULT(arg2->vx, 0x333);
    setSemiTrans(poly, 1);
    temp_v1_8 = Q12_MULT(arg2->vy, 0x333);

    setXY4(poly,
           temp_a0_5 - temp_a1_5, temp_v1_8 - temp_a1_5,
           temp_a0_5 + temp_a1_5, temp_v1_8 - temp_a1_5,
           temp_a0_5 - temp_a1_5, temp_v1_8 + temp_a1_5,
           temp_a0_5 + temp_a1_5, temp_v1_8 + temp_a1_5);

    addPrim(temp_s7, poly);
    poly++;

    SetPolyFT4(poly);

    temp_s0_6 = Q12_MULT(arg1, 0x18);
    setRGB0(poly, ((u32)arg1 * 7) >> 9, ((u32)arg1 * 0x13) >> 0xB, temp_s0_6);

    poly->tpage = 44;
    poly->clut  = getClut(CLUT_X, CLUT_Y + 7);

    setUV4(poly, 0x40, 0, 0x7F, 0, 0x40, 0x3F, 0x7F, 0x3F);
    setSemiTrans(poly, 1);

    temp_v1_10 = Q12_MULT(arg2->vx, -0x4CC);
    temp_v0_11 = Q12_MULT(arg2->vy, -0x4CC);

    setXY4(poly,
           temp_v1_10 - Q12_MULT(temp_s2_2, 0x1C), temp_v0_11 - Q12_MULT(temp_s2_2, 0x1C),
           temp_v1_10 + Q12_MULT(temp_s2_2, 0x1C), temp_v0_11 - Q12_MULT(temp_s2_2, 0x1C),
           temp_v1_10 - Q12_MULT(temp_s2_2, 0x1C), temp_v0_11 + Q12_MULT(temp_s2_2, 0x1C),
           temp_v1_10 + Q12_MULT(temp_s2_2, 0x1C), temp_v0_11 + Q12_MULT(temp_s2_2, 0x1C));

    addPrim(temp_s7, poly);
    poly++;

    SetPolyFT4(poly);

    setRGB0(poly, (u32)arg1 >> 6, temp_s0_6, ((u32)arg1 * 0x2F) >> 11);

    poly->tpage = 76;
    poly->clut  = getClut(CLUT_X, CLUT_Y + 6);

    setUV4(poly, 0, 0, 0x3F, 0, 0, 0x3F, 0x3F, 0x3F);
    setSemiTrans(poly, 1);

    temp_v0_13 = arg2->vx;
    temp_v1_12 = arg2->vy;

    setXY4(poly,
           temp_v0_13 - 10, temp_v1_12 - 10,
           temp_v0_13 + 10, temp_v1_12 - 10,
           temp_v0_13 - 10, temp_v1_12 + 10,
           temp_v0_13 + 10, temp_v1_12 + 10);

    addPrim(temp_s7, poly);
    poly++;
    GsOUT_PACKET_P = (PACKET*)poly;

    #undef CLUT_X
    #undef CLUT_Y
}

void func_8008E4EC(s_LmHeader* lmHdr) // 0x8008E4EC
{
    Lm_MaterialFsImageApply1(lmHdr, D_8002B2CC.str, &img0, 1);
}

s_WaterZone* Map_WaterZoneGet(q27_4 posX, q27_4 posZ, s_WaterZone* waterZones)
{
    s_WaterZone* curWaterZone;

    if (waterZones == NULL)
    {
        return NULL;
    }

    for (curWaterZone = waterZones; curWaterZone->isEnabled; curWaterZone++)
    {
        if (posX >= curWaterZone->minX && posX < curWaterZone->maxX &&
            posZ >= curWaterZone->minZ && posZ < curWaterZone->maxZ)
        {
            return curWaterZone;
        }
    }

    return NULL;
}

void func_8008E5B4(void) // 0x8008E5B4
{
    typedef struct
    {
        GsOT_TAG  tag_0;
        DR_AREA   area_4;
        DR_OFFSET offset_10;
        POLY_FT4  poly_1C[2];
        DR_ENV    env_6C;
    } s_func_8008E5B4;

    static RECT D_800AFDA4 = { 832, 224, 32, 32 };
    static s32  D_800AFDAC = 0;

    DRAWENV          drawEnv;
    s32              temp_v0;
    s_func_8008E5B4* packet;

    GetDrawEnv(&drawEnv);
    packet         = GsOUT_PACKET_P;
    GsOUT_PACKET_P = &packet[1];

    setlen(packet, 0);
    TermPrim(packet);

    SetDrawArea(&packet->area_4, &D_800AFDA4);
    SetDrawOffset(&packet->offset_10, (u16*)&D_800AFDA4);
    SetDrawEnv(&packet->env_6C, &drawEnv);

    if (g_DeltaTime != Q12(0.0f))
    {
        D_800AFDAC++;
    }

    setRGBC0(&packet->poly_1C[1], 128, 128, 128, 0x0);
    setRGBC0(&packet->poly_1C[0], 128, 128, 128, 0x0);

    setPolyFT4(&packet->poly_1C[0]);
    setPolyFT4(&packet->poly_1C[1]);
    setSemiTrans(&packet->poly_1C[1], true);

    packet->poly_1C[1].tpage = 45;
    packet->poly_1C[0].tpage = 45;

    setXY0Fast(&packet->poly_1C[1], 0, 0);
    setXY0Fast(&packet->poly_1C[0], 0, 0);
    setXY1Fast(&packet->poly_1C[1], 32, 0);
    setXY1Fast(&packet->poly_1C[0], 32, 0);
    setXY2Fast(&packet->poly_1C[1], 0, 32);
    setXY2Fast(&packet->poly_1C[0], 0, 32);
    setXY3Fast(&packet->poly_1C[1], 32, 32);
    setXY3Fast(&packet->poly_1C[0], 32, 32);

    packet->poly_1C[1].clut = getClut(WATER_CLUT_X, WATER_CLUT_Y);
    packet->poly_1C[0].clut = getClut(WATER_CLUT_X, WATER_CLUT_Y);

    temp_v0               = (D_800AFDAC >> 1) & 0x1F;
    packet->poly_1C[0].u0 = 0;
    packet->poly_1C[0].v0 = temp_v0;
    packet->poly_1C[0].u1 = 32;
    packet->poly_1C[0].v1 = temp_v0;
    packet->poly_1C[0].u2 = 0;
    packet->poly_1C[0].v2 = temp_v0 + 32;
    packet->poly_1C[0].u3 = 32;
    packet->poly_1C[0].v3 = temp_v0 + 32;

    temp_v0               = ((D_800AFDAC + 1) >> 2) & 0x1F;
    packet->poly_1C[1].u0 = temp_v0;
    packet->poly_1C[1].v0 = 8;
    packet->poly_1C[1].u1 = temp_v0 + 32;
    packet->poly_1C[1].v1 = 8;
    packet->poly_1C[1].u2 = temp_v0;
    packet->poly_1C[1].v2 = 40;
    packet->poly_1C[1].u3 = temp_v0 + 32;
    packet->poly_1C[1].v3 = 40;

    AddPrim(packet, &packet->env_6C);
    AddPrim(packet, &packet->poly_1C[1]);
    AddPrim(packet, &packet->poly_1C[0]);
    AddPrim(packet, &packet->area_4);
    AddPrim(packet, &packet->offset_10);

    DrawOTag((u32*)packet);
}

void func_8008E794(VECTOR3* posXz, q3_12 angle, q19_12 posY) // 0x8008E794
{
    VECTOR    sp10;
    VECTOR    sp20; // Q23.8
    MATRIX    sp30;
    s32       sp50;
    q19_12    angle0;
    q20_12    sinAngle0;
    POLY_FT4* poly;

    static SVECTOR svec0 = {};

    memset(&sp20, 0, 16);
    sp20.vx = Q12_TO_Q8(posXz->vx);
    sp20.vy = Q12_TO_Q8(posY * 2) - Q12_TO_Q8(posXz->vy);
    sp20.vz = Q12_TO_Q8(posXz->vz);
    sp10    = sp20;

    sp30 = GsWSMATRIX;
    ApplyMatrixLV(&GsWSMATRIX, &sp10, &sp30.t);
    sp30.t[0] += GsWSMATRIX.t[0];
    sp30.t[1] += GsWSMATRIX.t[1];
    sp30.t[2] += GsWSMATRIX.t[2];
    SetTransMatrix(&sp30);

    if ((RotTransPers(&svec0, &sp20, &sp50, &sp50) * 4) >= 128)
    {
        poly = GsOUT_PACKET_P;
        SetPolyFT4(poly);
        setSemiTrans(poly, true);

        angle0 = angle - Q12_ANGLE(40.0f);
        if (angle0 > Q12_ANGLE(90.0f))
        {
            angle0 = Q12_ANGLE(90.0f);
        }

        if (Math_Sin(angle0) >= Q12(0.0f))
        {
            sinAngle0 = Math_Sin(angle0);
        }
        else
        {
            sinAngle0 = Q12(0.0f);
        }

        poly->r0    = sinAngle0 >> 6;
        poly->g0    = (sinAngle0 * 7) >> 9;
        poly->b0    = (sinAngle0 * 5) >> 9;
        poly->tpage = 45;
        poly->clut  = getClut(WATER_CLUT_X, WATER_CLUT_Y + 1);
        poly->u0    = 64;
        poly->u1    = 64;
        poly->v0    = 31;
        poly->v1    = 0;
        poly->u2    = 127;
        poly->v2    = 31;
        poly->u3    = 127;
        poly->v3    = 0;
        poly->x0    = sp20.vx - 24;
        poly->y0    = ((u32)sp20.vx >> 16) - 48; // Are these hacks? Can't think of any other solutions.
        poly->x1    = sp20.vx + 24;
        poly->y1    = ((u32)sp20.vx >> 16) - 48;
        poly->x2    = sp20.vx - 24;
        poly->y2    = ((u32)sp20.vx >> 16) + 48;
        poly->x3    = sp20.vx + 24;
        poly->y3    = ((u32)sp20.vx >> 16) + 48;

        AddPrim(g_OrderingTable0[g_ActiveBufferIdx].org + 641, poly);
        GsOUT_PACKET_P = poly + 1;
    }
}

void func_8008EA68(SVECTOR* arg0, VECTOR3* posXz, q19_12 posY) // 0x8008EA68
{
    typedef struct
    {
        POLY_G4 g4[2];
        POLY_G3 g3[2];
    } s_func_8008EA68;

    SVECTOR          sp28[5]; // Q7.8
    GsCOORDINATE2    sp50;
    MATRIX           spA0;
    SVECTOR          spC0; // Q3.8 | Rotation?
    s32              packedXy;
    s32              spCC;
    s16              angle1;
    q3_12            curAngle;
    q19_12           angle0;
    s32              temp_s0_2;
    s16              temp_s1;
    s32              temp_s2;
    s32              temp_v0_2;
    s32              temp_v0_3;
    s32              temp_v1_2;
    s32              i;
    s32              temp;
    PACKET*          packet;
    s_func_8008EA68* poly;
    GsOT_TAG*        spD0;
    GsOT_TAG*        spD4;
    GsOT_TAG*        ot;

    ot         = g_OrderingTable0[g_ActiveBufferIdx].org;
    sp50.flg   = false;
    sp50.coord = GsIDMATRIX;

    spD0 = &ot[641];
    spD4 = &ot[639];

    angle1  = ratan2(arg0->vx, arg0->vz);
    sp50.coord.m[0][0] =  Math_Cos(angle1);
    sp50.coord.m[2][2] =  Math_Cos(angle1);
    sp50.coord.m[0][2] =  Math_Sin(angle1);
    sp50.coord.m[2][0] = -Math_Sin(angle1);

    sp50.super      = NULL;
    sp50.coord.t[0] = Q12_TO_Q8(posXz->vx);
    sp50.coord.t[1] = Q12_TO_Q8(posY);
    sp50.coord.t[2] = Q12_TO_Q8(posXz->vz);

    Vw_CoordToViewSpaceMatrix(&sp50, &spA0);
    SetRotMatrix(&spA0);
    SetTransMatrix(&spA0);

    spC0.vx = Q8(0.0f);
    spC0.vy = Q8(0.0f);
    spC0.vz = Q8(0.7f);
    RotTransPers(&spC0.vx, &packedXy, &spCC, &spCC);

    packet     = GsOUT_PACKET_P;
    sp28[1].vy = 0;
    sp28[0].vy = 0;
    sp28[3].vy = Q8(0.2f);
    sp28[2].vy = Q8(0.2f);

    for (i = 0, curAngle = 0; i < 8; i++, curAngle += Q12_ANGLE(45.0f))
    {
        temp_s1   = Math_Sin(curAngle);
        angle0    = curAngle + Q12_ANGLE(45.0f);
        temp_s2   = Math_Cos(curAngle);
        temp_s0_2 = Math_Sin(angle0);
        temp_v0_2 = Math_Cos(angle0);

        sp28[0].vx = FP_MULTIPLY((q7_8)temp_s1, Q8(0.2f), 10);
        sp28[0].vz = FP_MULTIPLY((q7_8)temp_s2, Q8(0.2f), 10) + Q8(1.2f);
        sp28[1].vx = FP_MULTIPLY((q7_8)temp_s0_2, Q8(0.2f), 10);
        sp28[1].vz = FP_MULTIPLY((q7_8)temp_v0_2, Q8(0.2f), 10) + Q8(1.2f);
        sp28[2].vx = Q12_MULT((q7_8)temp_s1, Q8(2.2f));
        sp28[2].vz = Q12_MULT((q7_8)temp_s2, Q8(2.2f)) + Q8(1.5);
        sp28[3].vx = Q12_MULT((q7_8)temp_s0_2, Q8(2.2f));
        sp28[3].vz = Q12_MULT((q7_8)temp_v0_2, Q8(2.2f)) + Q8(1.5);

        poly = packet;

        temp = 0;

        setRGBC0(&poly->g4[0], 20, 18, 12, 0x0);
        setRGBC1(&poly->g4[0], 20, 18, 12, 0x0);
        setRGBC3(&poly->g4[0], 4, 4, 4, 0x0);
        setRGBC2(&poly->g4[0], 4, 4, 4, 0x0);

        setRGBC0(&poly->g3[0], 24, 22, 12, 0x0);
        setRGBC2(&poly->g3[0], 20, 18, 12, 0x0);
        setRGBC1(&poly->g3[0], 20, 18, 12, 0x0);

        setRGBC0(&poly->g4[1], 4, 8, 4, 0x0);
        setRGBC1(&poly->g4[1], 4, 8, 4, 0x0);
        setRGBC3(&poly->g4[1], 0, 0, 0, 0x0);
        setRGBC2(&poly->g4[1], 0, 0, 0, 0x0);

        setRGBC0(&poly->g3[1], 8, 8, 4, 0x0);
        setRGBC2(&poly->g3[1], 4, 8, 4, 0x0);
        setRGBC1(&poly->g3[1], 4, 8, 4, 0x0);

        setPolyG4(&poly->g4[0]);
        setPolyG3(&poly->g3[0]);
        setPolyG4(&poly->g4[1]);
        setSemiTrans(&poly->g4[1], true);
        setPolyG3(&poly->g3[1]);
        setSemiTrans(&poly->g3[1], true);

        *(s32*)&poly->g3[1].x0 = *(s32*)&packedXy;
        *(s32*)&poly->g3[0].x0 = *(s32*)&packedXy;

        RotTransPers4(&sp28[0], &sp28[1], &sp28[2], &sp28[3], &poly->g4[0].x0, &poly->g4[0].x1, &poly->g4[0].x2, &poly->g4[0].x3, &spCC, &spCC);

        temp_v0_3 = *(s32*)&poly->g4[0].x0;
        temp_v1_2 = *(s32*)&poly->g4[0].x1;

        *(s32*)&poly->g3[1].x1 = temp_v0_3;
        *(s32*)&poly->g3[0].x1 = temp_v0_3;
        *(s32*)&poly->g4[1].x0 = temp_v0_3;

        *(s32*)&poly->g3[1].x2 = temp_v1_2;
        *(s32*)&poly->g3[0].x2 = temp_v1_2;
        *(s32*)&poly->g4[1].x1 = temp_v1_2;

        *(s32*)&poly->g4[1].x2 = *(s32*)&poly->g4[0].x2;
        *(s32*)&poly->g4[1].x3 = *(s32*)&poly->g4[0].x3;

        AddPrim(spD0, &poly->g4[0]);
        AddPrim(spD0, &poly->g3[0]);
        AddPrim(spD4, &poly->g4[1]);
        AddPrim(spD4, &poly->g3[1]);

        poly++;
        packet = poly;
    }

    SetDrawTPage((DR_TPAGE*)packet, temp, 1, 32);
    AddPrim(spD4, packet);
    packet        += sizeof(DR_TPAGE);
    GsOUT_PACKET_P = packet;
}

void func_8008EF18(void) {} // 0x8008EF18
