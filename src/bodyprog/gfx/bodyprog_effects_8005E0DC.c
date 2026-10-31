#include "game.h"
#include "inline_no_dmpsx.h"

#include <psyq/gtemac.h>
#include <psyq/libapi.h>
#include <psyq/strings.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/gfx/map_effects.h"
#include "bodyprog/math/math.h"
#include "bodyprog/screen/screen_data.h"
#include "bodyprog/screen/screen_draw.h"
#include "bodyprog/item_screens.h"
#include "bodyprog/player.h"
#include "bodyprog/sound/sound_system.h"
#include "main/rng.h"

#if VERSION_REGION_IS(PAL)
RECT D_800AE5A8[4] = {
    { 0x0, 0x1, 0x140, 0xFF },
    { 0x0, 0x100, 0x140, 0xFF },
    { 0x140, 0x100, 0x140, 0xFF },
    { 0xA0, 0x81, 0xA0, 0x17F }
};

u16 D_800AE5C8[2] = { 0x01E0, 0x017F };
#else
RECT D_800AE5A8[4] = {
    { 0x0, 0x20, 0x140, 0xE0 },
    { 0x0, 0x100, 0x140, 0xE0 },
    { 0x140, 0x100, 0x140, 0xE0 },
    { 0xA0, 0x90, 0xA0, 0x170 }
};

u16 D_800AE5C8[2] = { 0x01E0, 0x0170 };
#endif // Passed to `SetDrawOffset` and is accessed as part of a RECT with data from `D_800AE5A8`.

// https://decomp.me/scratch/HNL4n something to do with animations?
// `var_s7 = *(((animStatus + 1) * 2) + &D_800AE5CC) - *((animStatus * 2) + &D_800AE5CC);`
s16 D_800AE5CC[18] =
{
    0x0,  0x1,  0x4,  0x7,
    0x09, 0xB,  0xD,  0xF,
    0x11, 0x14, 0x17, 0x18,
    0x19, 0x1C, 0x1E, 0x20,
    0x22, 0x0
};

// https://decomp.me/scratch/HNL4n Referenced under `if (g_GameWork.config.extraBloodColor != 0xE)`
// Similar access pattern to the table above.
s16 D_800AE5F0[136] = {
    0x0001, 0x0000, 0x0001, 0x0000,
    0x0360, 0x00C0, 0x03E0, 0xFD90,
    0x03E0, 0xFC20, 0x0320, 0xFE50,
    0x09C0, 0xF950, 0x0100, 0xFFE0,
    0x0160, 0x05E0, 0x01C0, 0xFE30,
    0x0360, 0x00F0, 0x02C0, 0xFDB0,
    0x0360, 0xFD70, 0x02C0, 0xFE40,
    0x0640, 0x0130, 0x0220, 0xFF40,
    0x04E0, 0xFFA0, 0x02A0, 0xFEC0,
    0x04E0, 0xFAC0, 0x0280, 0xFE70,
    0x05DC, 0xFCAE, 0x00C8, 0xFF9C,
    0x0300, 0xFCB0, 0x02C0, 0x00C0,
    0x06E0, 0xFFC0, 0x01E0, 0xFF70,
    0x0340, 0x00A0, 0x0360, 0xFF70,
    0x06E0, 0xFAA0, 0x0160, 0xFFC0,
    0x03A0, 0x0070, 0x0320, 0xFD10,
    0x07A0, 0xF9F0, 0x0160, 0xFEF0,
    0x0CC0, 0xFD90, 0x0100, 0xFFF0,
    0x04C0, 0x0080, 0x01C0, 0xFF00,
    0x0400, 0xFD40, 0x0260, 0xFEA0,
    0x01C0, 0x0D40, 0x0180, 0x0070,
    0x05A0, 0x01F0, 0x0B32, 0xFAD0,
    0x0AA0, 0xFAB0, 0x0200, 0xFF10,
    0x05A0, 0xFE90, 0x0160, 0xFF20,
    0x0620, 0xFC80, 0x0140, 0xFF30,
    0x0620, 0x0120, 0x0240, 0xFE70,
    0x0A20, 0x0090, 0x00E0, 0xFF70,
    0x0140, 0x0920, 0x00E0, 0xFF70,
    0x0580, 0x0180, 0x0220, 0xFDF0,
    0x0580, 0xFD80, 0x0220, 0xFE20,
    0x0300, 0x12E0, 0x02E0, 0x00F0,
    0x0AE0, 0x0B60, 0x02E0, 0x0210,
    0x08A0, 0xEDF0, 0x02A0, 0x0220,
    0x0300, 0xEDA0, 0x02E0, 0xFD70
};
// https://decomp.me/scratch/HNL4n `temp_a0 = D_800AE700[animStatus];`
u8 D_800AE700[16] = {
    0x64, 0x48, 0x48, 0x3C,
    0x3C, 0x6E, 0x6E, 0x6E,
    0x3A, 0x50, 0x3C, 0x3C,
    0x48, 0x5E, 0x64, 0x64
};

s_800C42E8     D_800C42E8[24];
s16            D_800C4408;
s16            __pad_bss_800C440A;
GsCOORDINATE2* D_800C440C;
GsCOORDINATE2* D_800C4410;
s8             D_800C4414; /** Flags. */
s8             __pad_bss_800C4415[3];
s_800C4418     D_800C4418;

// ========================================
// ENVIRONMENT EFFECTS HANDLING
// ========================================

#if VERSION_IS(EUR)
// @hack Statics of the included function below, remove once decompiled.
// Declared inside an uncalled function so the compiler emits them here instead of at the end of the file.
static inline void Map_EffectTexturesLoad_Statics(void)
{
    static u16 loadedEffectTextureFlags __asm__("loadedEffectTextureFlags");
    static s16 __pad_bss_800C38CA[7] __asm__("__pad_bss_800C38CA");

    (void)loadedEffectTextureFlags;
    (void)__pad_bss_800C38CA;
}
INCLUDE_ASM("bodyprog/nonmatchings/gfx/bodyprog_effects_8005E0DC", Map_EffectTexturesLoad);
#else
void Map_EffectTexturesLoad(s32 mapIdx) // 0x8005E0DC
{
    s32        i;
    s32        effectTexFlags;
    static u16 loadedEffectTextureFlags;
    static s16 __pad_bss_800C42DA[7];

    D_800A908C.v     = 0;
    D_800A908C.clutY = 0;
    D_800A9094.v     = 128;

    // Get effect texture flags.
    effectTexFlags = EffectTextureFlag_None;
    switch (mapIdx)
    {
        case NO_VALUE:
            Fs_QueueStartReadTim(FILE_TIM_BLD_TIM, (s32)FONT24_BUFFER - ALIGN(Fs_GetFileSize(FILE_TIM_BLD_TIM), 0x800), &D_800A9084);
            loadedEffectTextureFlags = EffectTextureFlag_None;
            break;

        case MapIdx_MAP0_S01:
            if (!Savegame_EventFlagGet(EventFlag_47))
            {
                effectTexFlags = EffectTextureFlag_Glass;
            }
            break;

        case MapIdx_MAP1_S02:
        case MapIdx_MAP1_S03:
            effectTexFlags = EffectTextureFlag_Water;
            break;

        case MapIdx_MAP1_S05:
            effectTexFlags = EffectTextureFlag_Fire | EffectTextureFlag_Ef;
            break;

        case MapIdx_MAP3_S05:
            if (!Savegame_EventFlagGet(EventFlag_284))
            {
                effectTexFlags = EffectTextureFlag_Fire;
            }
            break;

        case MapIdx_MAP4_S01:
            if (!Savegame_EventFlagGet(EventFlag_306))
            {
                effectTexFlags = EffectTextureFlag_Fire;
            }
            break;

        case MapIdx_MAP4_S05:
            effectTexFlags = EffectTextureFlag_Blood;
            break;

        case MapIdx_MAP5_S00:
        case MapIdx_MAP6_S03:
        case MapIdx_MAPX_S00: // @unused
            effectTexFlags = EffectTextureFlag_WaterRefract;
            break;
    }

    if (effectTexFlags == EffectTextureFlag_None)
    {
        return;
    }

    // Run through effect texture flags.
    loadedEffectTextureFlags = EffectTextureFlag_None;
    for (i = 0; i < 16; i++)
    {
        if (!((effectTexFlags >> i) & (1 << 0)))
        {
            continue;
        }

        // Set global flag.
        loadedEffectTextureFlags |= 1 << i;

        // TODO: Not sure if this is actually checking something gte related, but the macro/pointless branch is needed for match.
        if (gte_IsDisabled())
        {
            continue;
        }

        // Load effect textures.
        switch (1 << i)
        {
            case EffectTextureFlag_Glass:
                Fs_QueueStartReadTim(FILE_TIM_GLASS_TIM, (s32)FONT24_BUFFER - ALIGN(Fs_GetFileSize(FILE_TIM_GLASS_TIM), 0x800), &D_800A908C);
                break;

            case EffectTextureFlag_WaterRefract:
                Fs_QueueStartReadTim(FILE_TIM_DR_WAVE_TIM, (s32)FONT24_BUFFER - ALIGN(Fs_GetFileSize(FILE_TIM_DR_WAVE_TIM), 0x800), &D_800A908C);
                break;

            case EffectTextureFlag_Water:
                Fs_QueueStartReadTim(FILE_TIM_WATER_TIM, (s32)FONT24_BUFFER - ALIGN(Fs_GetFileSize(FILE_TIM_WATER_TIM), 0x800), &D_800A908C);
                break;

            case EffectTextureFlag_Fire:
                D_800A9094.v = 120;
                Fs_QueueStartReadTim(FILE_TIM_FIRE_TIM, (s32)FONT24_BUFFER - ALIGN(Fs_GetFileSize(FILE_TIM_FIRE_TIM), 0x800), &D_800A9094);
                break;

            case EffectTextureFlag_Ef:
                D_800A908C.v     = 64;
                D_800A908C.clutY = 4;
                Fs_QueueStartReadTim(FILE_TIM_EF_TIM, (s32)FONT24_BUFFER - ALIGN(Fs_GetFileSize(FILE_TIM_EF_TIM), 0x800), &D_800A908C);
                break;

            case EffectTextureFlag_Blood:
                Fs_QueueStartReadTim(FILE_TIM_BLOOD_TIM, (s32)FONT24_BUFFER - ALIGN(Fs_GetFileSize(FILE_TIM_BLOOD_TIM), 0x800), &D_800A908C);
                break;

            case EffectTextureFlag_WarmTest: // @unused See `e_EffectTextureFlags`.
                Fs_QueueStartReadTim(FILE_TEST_WARMTEST_TIM, (s32)FONT24_BUFFER - ALIGN(Fs_GetFileSize(FILE_TEST_WARMTEST_TIM), 0x800), &D_800A9094);
                break;
        }
    }
}
#endif

void func_8005E414(s32 orgIdx) // 0x8005E414
{
    RECT*      rect;
    PACKET*    packet;
    DR_AREA*   area;
    DR_OFFSET* offset;

    ClearImage(&D_800AE5A8[2], 0, 0, 0);

    area = (DR_AREA*)GsOUT_PACKET_P;

    SetDrawArea(area, &D_800AE5A8[2]);

    rect = &D_800AE5A8[0];

    addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[ORDERING_TABLE_SIZE - 1], area);
    area++;

    if (g_ActiveBufferIdx == 0)
    {
        rect = &D_800AE5A8[1];
    }

    SetDrawArea(area, rect);

    addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[orgIdx], area);
    area++;

    offset = area;

    SetDrawOffset(offset, &D_800AE5C8[0]);

    rect = D_800AE5C8 - 4;

    addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[ORDERING_TABLE_SIZE - 1], offset);
    offset++;

    if (g_ActiveBufferIdx == 0)
    {
        rect = D_800AE5C8 - 2;
    }

    SetDrawOffset(offset, rect);

    addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[orgIdx], offset);
    offset++;

    GsOUT_PACKET_P = offset;
}

void func_8005E650(s32 mapId) // 0x8005E650
{
    s32 i;
    s16 count;

    count = g_MapOverlayHdr.unkTable1Count_50;
    for (i = 0; i < count; i++)
    {
        g_MapOverlayHdr.unkTable1_4C[i].field_A = 0;
    }

    count = g_MapOverlayHdr.bloodSplatCount;
    for (i = 0; i < count; i++)
    {
        g_MapOverlayHdr.bloodSplats[i].field_0 = NO_VALUE;
    }

    D_800C4408 = 0;
    D_800C4414 = 0;

    if (g_MapOverlayHdr.initWorldObjects != NULL)
    {
        g_MapOverlayHdr.initWorldObjects();
    }
}

void func_8005E70C(void) // 0x8005E70C
{
    s32 i;
    s16 count;
    u8  temp_v0;

    count = g_MapOverlayHdr.unkTable1Count_50;
    for (i = 0; i < count; i++)
    {
        temp_v0 = g_MapOverlayHdr.unkTable1_4C[i].field_A;
        switch (temp_v0)
        {
            case 1:
            case 2:
            case 3:
            case 15:
            case 16:
            case 17:
            case 20:
            case 21:
            case 22:
                g_MapOverlayHdr.unkTable1_4C[i].field_A = 0;
                break;
        }
    }

    count = g_MapOverlayHdr.bloodSplatCount;
    for (i = 0; i < count; i++)
    {
        g_MapOverlayHdr.bloodSplats[i].field_0 = NO_VALUE;
    }

    D_800C4414 = 0;
}

s32 func_8005E7E0(s32 arg0) // 0x8005E7E0
{
    s32 idx;
    s32 i;

    idx = D_800C4408;

    for (i = 0; i < g_MapOverlayHdr.unkTable1Count_50; i++, idx++)
    {
        if (idx >= g_MapOverlayHdr.unkTable1Count_50)
        {
            idx = 0;
        }

        if (g_MapOverlayHdr.unkTable1_4C[idx].field_A == 0)
        {
            break;
        }
    }

    if (i == g_MapOverlayHdr.unkTable1Count_50)
    {
        return NO_VALUE;
    }

    g_MapOverlayHdr.unkTable1_4C[idx].field_A = arg0;

    D_800C4408 = idx + 1;
    return idx;
}

void func_8005E89C(void) // 0x8005E89C
{
    #define CLAMP_CUSTOM(a, b, min, max) \
        ((((a) >= (max)) ? (max) : (b)) < (min) ? (min) : (((b) >= (max)) ? (max) : (a)))

    SVECTOR          sp10;
    s32              posX;
    s32              posZ;
    s32              posY;
    s32              i;
    POLY_FT4*        poly;
    s_SubCharacter*  chara;
    s_func_8005E89C* ptr;
    s_800C42E8*      curPtr;

    ptr = PSX_SCRATCH;

    posX            = FP_FROM(g_SysWork.playerWork.player.position.vx, Q12_SHIFT);
    posY            = FP_FROM(g_SysWork.playerWork.player.position.vy, Q12_SHIFT);
    posZ            = FP_FROM(g_SysWork.playerWork.player.position.vz, Q12_SHIFT);
    ptr->field_0.vx = Q8(posX);
    ptr->field_0.vy = Q8(posY);
    ptr->field_0.vz = Q8(posZ);

    Vw_WorldScreenMatrixAtPositionGet(&ptr->worldToScreenMat, Q12(posX), Q12(posY), Q12(posZ));

    gte_SetRotMatrix(&ptr->worldToScreenMat);
    gte_SetTransMatrix(&ptr->worldToScreenMat);
    gte_ReadGeomScreen(&ptr->field_2C);

    vwGetViewAngle(&sp10);
    ptr->field_30.vx = sp10.vx;
    ptr->field_30.vy = sp10.vy;

    if (D_800C4414 & 0x1)
    {
        ptr->field_C4 = Q12_MULT_PRECISE(g_DeltaTime, g_MapOverlayHdr.field_5C->field_E);
        ptr->field_C8 = g_MapOverlayHdr.field_5C->field_4 * 16;
        ptr->field_CA = g_MapOverlayHdr.field_5C->field_5 * 16;
        ptr->field_CC = (0x100 - g_MapOverlayHdr.field_5C->field_5) * 16;
        ptr->field_CE = (g_DeltaTime * g_MapOverlayHdr.field_5C->field_E) / g_MapOverlayHdr.field_5C->field_16;
        ptr->field_C6 = (u16)g_MapOverlayHdr.field_5C->field_A >> 2;
        ptr->field_D0 = g_MapOverlayHdr.field_5C->field_20;
        ptr->field_D4 = g_MapOverlayHdr.field_5C->field_24;
        ptr->field_D8 = g_MapOverlayHdr.field_5C->field_6 * (2 * ptr->field_2C);

        switch (g_MapOverlayHdr.field_5C->field_1)
        {
            case 0:
                for (i = 0; i < g_MapOverlayHdr.unkTable1Count_50; i++)
                {
                    if (g_MapOverlayHdr.unkTable1_4C[i].field_A == 12)
                    {
                        g_MapOverlayHdr.func_60(i, 0);
                    }
                }
                break;

            case 1:
                g_MapOverlayHdr.field_5C->field_10 = MIN(g_MapOverlayHdr.field_5C->field_10 + ((g_DeltaTime * g_MapOverlayHdr.field_5C->field_2) >> 5),
                                                            Q12(1.0f));
                break;

            case 2:
                for (i = 0; i < g_MapOverlayHdr.unkTable1Count_50; i++)
                {
                    switch (g_MapOverlayHdr.unkTable1_4C[i].field_A)
                    {
                        case 8:
                        case 9:
                        case 10:
                        case 11:
                            g_MapOverlayHdr.unkTable1_4C[i].field_A = 12;
                            break;
                    }
                }
                break;

            case 3:
                g_MapOverlayHdr.field_5C->field_10 = CLAMP_LOW(g_MapOverlayHdr.field_5C->field_10 - ((g_DeltaTime * g_MapOverlayHdr.field_5C->field_2) >> 5),
                                                                  Q12(0.0f));

                for (i = 0; i < g_MapOverlayHdr.unkTable1Count_50; i++)
                {
                    if (g_MapOverlayHdr.unkTable1_4C[i].field_A == 12 &&
                        Rng_GenerateUInt(0, 4095) > g_MapOverlayHdr.field_5C->field_10 &&
                        Rng_GenerateInt(0, 15) != 0)
                    {
                        g_MapOverlayHdr.func_60(i, 1);
                    }
                }
                break;
        }
    }
    else if (D_800C4414 & 2)
    {
        if (g_MapOverlayHdr.field_7C->field_1C > 0)
        {
            g_MapOverlayHdr.field_7C->field_1C -= g_DeltaTime;
            g_MapOverlayHdr.field_7C->field_1C  = MAX(g_MapOverlayHdr.field_7C->field_1C, 0);
        }
        else
        {
            g_MapOverlayHdr.field_7C->field_10 = Q12_MULT_PRECISE(g_DeltaTime, g_MapOverlayHdr.field_7C->field_E) +
                                                    g_MapOverlayHdr.field_7C->field_10;
            g_MapOverlayHdr.field_7C->field_10 = MIN(g_MapOverlayHdr.field_7C->field_10, Q12(0.25f));
        }
    }
    else if (D_800C4414 & 8)
    {
        for (i = 0; i < g_MapOverlayHdr.field_94->field_78; i++)
        {
            if (g_MapOverlayHdr.field_94->field_30[i] == 0)
            {
                ptr->field_DC[i]         = Math_Sin(g_MapOverlayHdr.field_94->field_34[i]);
                ptr->field_E4[i]         = Math_Cos(g_MapOverlayHdr.field_94->field_34[i]);
                ptr->u_field_EC.raw_0[i] = (g_MapOverlayHdr.field_94->field_5C[i] >> 1) * Math_Sin(g_MapOverlayHdr.field_94->field_34[i]);
                ptr->u_field_FC.raw_0[i] = (g_MapOverlayHdr.field_94->field_5C[i] >> 1) * Math_Cos(g_MapOverlayHdr.field_94->field_34[i]);
                ptr->field_10C[i]        = g_MapOverlayHdr.field_94->field_64[i] * Math_Sin(g_MapOverlayHdr.field_94->field_34[i]);
                ptr->field_11C[i]        = g_MapOverlayHdr.field_94->field_64[i] * Math_Cos(g_MapOverlayHdr.field_94->field_34[i]);
            }
            else
            {
                ptr->u_field_EC.field_0[i].vx = Q12_MULT(g_MapOverlayHdr.field_94->field_5C[i] >> 1, Math_Sin(ptr->field_30.vy - Q12_ANGLE(90.0f)));
                ptr->u_field_EC.field_0[i].vy = Q12_MULT(g_MapOverlayHdr.field_94->field_5C[i] >> 1, Math_Sin(ptr->field_30.vy + Q12_ANGLE(90.0f)));
                ptr->u_field_FC.field_0[i].vx = Q12_MULT(g_MapOverlayHdr.field_94->field_5C[i] >> 1, Math_Cos(ptr->field_30.vy - Q12_ANGLE(90.0f)));
                ptr->u_field_FC.field_0[i].vy = Q12_MULT(g_MapOverlayHdr.field_94->field_5C[i] >> 1, Math_Cos(ptr->field_30.vy + Q12_ANGLE(90.0f)));
            }
        }

        g_MapOverlayHdr.func_9C();
    }

    for (i = 0; i < ARRAY_SIZE(D_800C42E8); i++)
    {
        curPtr = &D_800C42E8[i];

        if (D_800C42E8[i].field_0 != 0)
        {
            if (D_800C42E8[i].field_1 == 6)
            {
                chara = &g_SysWork.playerWork.player;
            }
            else
            {
                chara = &g_SysWork.npcs[D_800C42E8[i].field_1];
            }

            ptr->field_34[i] = CLAMP_CUSTOM((chara->position.vx + chara->collision.shapeOffsets.box.vx) - D_800C42E8[i].field_4,
                                            (chara->position.vx + chara->collision.shapeOffsets.box.vx) - curPtr->field_4,
                                            -Q12_MULT_PRECISE(g_DeltaTime, Q12(1.25f)),
                                             Q12_MULT_PRECISE(g_DeltaTime, Q12(1.25f)));

            ptr->field_64[i] = CLAMP_CUSTOM((chara->position.vy + chara->collision.box.offsetY) - D_800C42E8[i].field_2,
                                            (chara->position.vy + chara->collision.box.offsetY) - curPtr->field_2,
                                            -Q12_MULT_PRECISE(g_DeltaTime, Q12(1.25f)) >> 4,
                                            Q12_MULT_PRECISE(g_DeltaTime, Q12(1.25f)));

            ptr->field_94[i] = CLAMP_CUSTOM((chara->position.vz + chara->collision.shapeOffsets.box.vz) - D_800C42E8[i].field_8,
                                            (chara->position.vz + chara->collision.shapeOffsets.box.vz) - curPtr->field_8,
                                            -Q12_MULT_PRECISE(g_DeltaTime, Q12(1.25f)),
                                             Q12_MULT_PRECISE(g_DeltaTime, Q12(1.25f)));

            D_800C42E8[i].field_4 += ptr->field_34[i];
            D_800C42E8[i].field_2 += ptr->field_64[i];
            D_800C42E8[i].field_8 += ptr->field_94[i];
        }
        else
        {
            ptr->field_34[i] = Q12(0.0f);
            ptr->field_64[i] = Q12(0.0f);
            ptr->field_94[i] = Q12(0.0f);
        }
    }

    poly = (POLY_FT4*)GsOUT_PACKET_P;

    for (i = 0; i < g_MapOverlayHdr.unkTable1Count_50; i++)
    {
        switch (g_MapOverlayHdr.unkTable1_4C[i].field_A)
        {
            case 0:
                break;

            case 1:
                func_80060044(&poly, i);
                break;

            case 2:
                func_800611C0(&poly, i);
                break;

            case 3:
            case 4:
                func_80062708(&poly, i);
                break;

            case 15:
            case 16:
            case 17:
            case 18:
            case 19:
                func_80063A50(&poly, i);
                break;

            case 20:
            case 21:
            case 22:
                func_80064334(&poly, i);
                break;

            case 8:
            case 10:
                g_MapOverlayHdr.func_64(&poly, i);
                break;

            case 9:
            case 11:
                g_MapOverlayHdr.func_68(&poly, i);
                break;

            case 7:
                g_MapOverlayHdr.func_70(&poly, i);
                break;

            case 5:
                g_MapOverlayHdr.func_78(&poly, i);
                break;

            case 13:
                g_MapOverlayHdr.func_80(i);
                break;

            case 14:
                g_MapOverlayHdr.func_84(&poly, i);
                break;

            case 23:
            case 24:
            case 25:
            case 26:
                g_MapOverlayHdr.func_8C(&poly, i);
                break;

            case 27:
                g_MapOverlayHdr.func_98(&poly, i);
                break;

            case 28:
                g_MapOverlayHdr.func_A4(&poly, i);
                break;

            case 31:
                func_80064FC0(&poly, i);
                break;

            case 32:
                g_MapOverlayHdr.func_90(&poly, i);
                break;

            case 33:
                g_MapOverlayHdr.func_AC(&poly, i);
                break;

            case 34:
                g_MapOverlayHdr.func_B0(&poly, i);
                break;

            case 35:
                g_MapOverlayHdr.func_B4(&poly, i);
                break;
        }
    }

    GsOUT_PACKET_P = (PACKET*)poly;

    for (i = 0; i < g_MapOverlayHdr.bloodSplatCount; i++)
    {
        if (g_MapOverlayHdr.bloodSplats[i].field_0 != NO_VALUE &&
            g_MapOverlayHdr.unkTable1_4C[g_MapOverlayHdr.bloodSplats[i].field_0].field_A == 0)
        {
            g_MapOverlayHdr.bloodSplats[i].field_0 = NO_VALUE;
        }
    }

    if (D_800C4414 & (1 << 5))
    {
        g_MapOverlayHdr.func_A8();
    }

    if (g_SysWork.field_2388.isFlashlightUnavailable && g_SysWork.sysState == 0)
    {
        Game_TurnFlashlightOff();
    }
}

s32 func_8005F55C(s32 arg0) // 0x8005F55C
{
    if (arg0 == 15)
    {
        return 0;
    }

    // TODO: Use available enums.
    switch (g_GameWork.config.extraBloodColor)
    {
        case 3:
            return 6;

        case 6:
            return 7;

        case 9:
            return 5;

        case 12:
            return 4;

        case 13:
            return 0;
    }

    switch (arg0)
    {
        case 4:
            return 9;

        case 1:
        case 14:
        {
            // TODO: Use available enums. Some values appear unused.
            switch (g_GameWork.config.extraBloodColor)
            {
                case 5:
                    return 7;

                case 2:
                    return 6;

                case 8:
                    return 5;

                case 11:
                    return 4;

                default:
                    return 0;
            }
        }

        case 5:
        case 8:
        case 12:
            return 6;

        case 10:
            return 7;
    }

    // TODO: Use available enums. Some values appear unused.
    switch (g_GameWork.config.extraBloodColor)
    {
        case 1:
        case 2:
            return 6;

        case 4:
        case 5:
            return 7;

        case 7:
        case 8:
            return 5;

        case 10:
        case 11:
            return 4;
    }

    return 0;
}

bool func_8005F680(s_CollisionSurface* surface) // 0x8005F680
{
    bool isSomeFloorGroup;
    s8   floorType;

    floorType = surface->floorType;

    isSomeFloorGroup = false;
    if (floorType == FloorType_Default ||
        floorType == FloorType_None ||
        floorType == FloorType_7)
    {
        isSomeFloorGroup = true;
    }

    return isSomeFloorGroup;
}

void func_8005F6B0(s_SubCharacter* chara, VECTOR* pos, s32 arg2, s32 arg3) // 0x8005F6B0
{
    s_CollisionSurface surface;
    VECTOR3            camPos; // Q19.12
    q19_12             newPosX;
    q19_12             newPosZ;
    q20_12             dists[150];
    s32                idx;
    s32                unkCount;
    s32                var_s5;
    s32                i;
    s32                j;
    s32                k;
    s32                count;
    s32                curDist;
    GsCOORDINATE2*     camCoord;

    if (g_GameWork.config.extraBloodColor == 14) // TODO: Demagic 14.
    {
        arg3 = 5;
    }
    else
    {
        arg3 = func_8005F55C(arg3);
    }

    switch (arg2)
    {
        case 1:
            var_s5 = 1;
            count = 0;
            unkCount = 6;
            break;

        case 2:
            var_s5 = 2;
            count = 1;
            unkCount = 12;
            break;

        case 3:
            var_s5 = 3;
            count = Rng_GenerateUInt(1, 2);
            unkCount = 18;
            break;

        case 4:
            var_s5 = 3;
            count = Rng_GenerateUInt(1, 4);
            unkCount = 24;
            break;

        case 5:
            var_s5 = 4;
            count = Rng_GenerateUInt(2, 5);
            unkCount = 0x1E;
            break;

        case 6:
            var_s5 = 4;
            count = Rng_GenerateUInt(1, 8);
            unkCount = 36;
            break;

        case 7:
            var_s5 = 5;
            count = Rng_GenerateUInt(2, 9);
            unkCount = 42;
            break;

        case 0:
        default:
            var_s5 = 0;
            count = 0;
            unkCount = 0;
            break;

        case 8:
            var_s5 = 3;
            count = 0;
            unkCount = 16;
            break;

        case 9:
            var_s5 = 1;
            count = 0;
            unkCount = 16;
            break;
    }

    if (g_GameWork.config.extraBloodColor == 14)
    {
        count = 0;
    }

    if (arg2 != 8)
    {
        for (i = 0; i < ARRAY_SIZE(D_800C42E8); i++)
        {
            if (D_800C42E8[i].field_0 == 0)
            {
                break;
            }
        }

        if (chara->model.charaId == Chara_SplitHead)
        {
            i = ARRAY_SIZE(D_800C42E8);
        }

        if (i != ARRAY_SIZE(D_800C42E8))
        {
            D_800C42E8[i].field_0 = 1;
            D_800C42E8[i].field_1 = Chara_NpcIdxGet(chara);
            D_800C42E8[i].field_4 = chara->position.vx + chara->collision.shapeOffsets.box.vx;
            D_800C42E8[i].field_2 = chara->position.vy + chara->collision.box.offsetY;
            D_800C42E8[i].field_8 = chara->position.vz + chara->collision.shapeOffsets.box.vz;
        }
    }
    else
    {
        i = ARRAY_SIZE(D_800C42E8);
    }

    for (j = 0; j < var_s5; j++)
    {
        idx = func_8005E7E0(1);

        if (idx != NO_VALUE)
        {
            if (arg2 != 9)
            {
                g_MapOverlayHdr.unkTable1_4C[idx].field_0.vx_0 = Rng_AddGeneratedUInt(pos->vx, -255, 256);
                g_MapOverlayHdr.unkTable1_4C[idx].vy_8         = Rng_AddGeneratedUInt(pos->vy, -255, 256);
                g_MapOverlayHdr.unkTable1_4C[idx].field_4.vz_4 = Rng_AddGeneratedUInt(pos->vz, -255, 256);
            }
            else
            {
                g_MapOverlayHdr.unkTable1_4C[idx].field_0.vx_0 = pos->vx;
                g_MapOverlayHdr.unkTable1_4C[idx].vy_8         = pos->vy;
                g_MapOverlayHdr.unkTable1_4C[idx].field_4.vz_4 = pos->vz;
            }

            if (Rng_GenerateInt(0, 1) != 0) // 1 in 2 chance.
            {
                g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_0 = Rng_GenerateInt(0, 2) + (Rng_GenerateUInt(0, 7) * 8);
            }
            else
            {
                g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_0 = Rng_GenerateUInt(3, 4) + (Rng_GenerateUInt(0, 1) * 8);
            }

            g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_1  = unkCount;
            g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_2  = arg3;
            g_MapOverlayHdr.unkTable1_4C[idx].field_B              = i * 4;
            g_MapOverlayHdr.unkTable1_4C[idx].field_10.s_0.field_0 = Rng_GenerateUInt(0, 255);

            if (chara->model.charaId == Chara_Floatstinger)
            {
                g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_3 = 96;
            }
            else
            {
                g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_3 = 32;
            }

            if (arg2 == 9)
            {
                g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_3 = 33;
            }
        }
        else
        {
            return;
        }
    }

    if (count == 0)
    {
        return;
    }

    vwGetViewPosition(&camPos);
    camCoord   = vwGetViewCoord();
    camPos.vx += camCoord->coord.m[0][2] * 2;
    camPos.vz += camCoord->coord.m[2][2] * 2;

    for (j = 0; j < g_MapOverlayHdr.bloodSplatCount; j++)
    {
        if (g_MapOverlayHdr.bloodSplats[j].field_0 == NO_VALUE)
        {
            dists[j] = INT_MAX;
        }
        else
        {
            if (g_MapOverlayHdr.unkTable1_4C[g_MapOverlayHdr.bloodSplats[j].field_0].field_B != 2)
            {
                dists[j] = Q12(0.0f);
            }
            else
            {
                newPosX = ABS(camPos.vx - g_MapOverlayHdr.unkTable1_4C[g_MapOverlayHdr.bloodSplats[j].field_0].field_0.vx_0);
                newPosZ = ABS(camPos.vz - g_MapOverlayHdr.unkTable1_4C[g_MapOverlayHdr.bloodSplats[j].field_0].field_4.vz_4);

                dists[j] = MAX(newPosX, newPosZ) + (CLAMP_HIGH(newPosX, newPosZ) >> 1);

                if (dists[j] > Q12(80.0f))
                {
                    g_MapOverlayHdr.unkTable1_4C[g_MapOverlayHdr.bloodSplats[j].field_0].field_A = 0;
                    g_MapOverlayHdr.bloodSplats[j].field_0                                          = NO_VALUE;
                    dists[j]                                                                              = INT_MAX;
                }
                else
                {
                    dists[j] += Rng_GenerateUInt(Q12(0.0f), Q12(4.0f) - 1);
                }
            }
        }
    }

    for (j = 0; j < count; j++)
    {
        if (dists[j] != INT_MAX)
        {
            for (k = j + 1; k < g_MapOverlayHdr.bloodSplatCount; k++)
            {
                curDist = dists[j];
                if (dists[k] <= curDist)
                {
                    continue;
                }

                dists[j] = dists[k];
                dists[k] = curDist;

                unkCount                                       = g_MapOverlayHdr.bloodSplats[j].field_0;
                g_MapOverlayHdr.bloodSplats[j].field_0 = g_MapOverlayHdr.bloodSplats[k].field_0;
                g_MapOverlayHdr.bloodSplats[k].field_0 = unkCount;

                if (dists[j] == INT_MAX)
                {
                    break;
                }
            }

            if (dists[j] == Q12(0.0f))
            {
                count = j;
                break;
            }
        }
    }

    unkCount = 0;

    for (j = 0; j < count; j++)
    {
        if (g_MapOverlayHdr.bloodSplats[j].field_0 != NO_VALUE)
        {
            g_MapOverlayHdr.unkTable1_4C[g_MapOverlayHdr.bloodSplats[j].field_0].field_B          = 3;
            g_MapOverlayHdr.unkTable1_4C[g_MapOverlayHdr.bloodSplats[j].field_0].field_10.field_0 = 0;
            g_MapOverlayHdr.bloodSplats[j].field_0                                                   = NO_VALUE;
        }

        idx = func_8005E7E0(2);
        if (idx != NO_VALUE)
        {
            g_MapOverlayHdr.unkTable1_4C[idx].field_B      = i * 4;
            g_MapOverlayHdr.unkTable1_4C[idx].field_0.vx_0 = Rng_AddGeneratedUInt(pos->vx, -1023, 1024);
            g_MapOverlayHdr.unkTable1_4C[idx].vy_8         = Rng_AddGeneratedUInt(pos->vy, -2047, 2048);
            g_MapOverlayHdr.unkTable1_4C[idx].field_4.vz_4 = Rng_AddGeneratedUInt(pos->vz, -1023, 1024);

            Collision_SurfaceGet(&surface, g_MapOverlayHdr.unkTable1_4C[idx].field_0.vx_0, g_MapOverlayHdr.unkTable1_4C[idx].field_4.vz_4);

            if (func_8005F680(&surface) || (surface.floorHeight < (pos->vy - Q12(0.2f))))
            {
                g_MapOverlayHdr.unkTable1_4C[idx].field_A = 0;
            }
            else
            {
                if (surface.floorHeight < g_MapOverlayHdr.unkTable1_4C[idx].vy_8)
                {
                    g_MapOverlayHdr.unkTable1_4C[idx].vy_8 = surface.floorHeight;

                    // @hack
                    unkCount++;
                    unkCount--;
                }

                g_MapOverlayHdr.unkTable1_4C[idx].field_10.s_0.field_0 = -(Rng_Rand16() & 0x800);
                g_MapOverlayHdr.unkTable1_4C[idx].field_10.s_0.field_2 = surface.floorHeight;
                g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_0  = Rng_GenerateUInt(0, 15);
                g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_2  = arg3;
                g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_1  = Rng_GenerateUInt(6, 21);
                g_MapOverlayHdr.bloodSplats[j].field_0              = idx;
                unkCount++;
            }
        }
        else
        {
            return;
        }
    }

    if (unkCount != 0 && i != 24)
    {
        D_800C42E8[i].field_0 += 2;
    }
}

#if VERSION_IS(EUR)
INCLUDE_ASM("bodyprog/nonmatchings/gfx/bodyprog_effects_8005E0DC", func_80060044);
#else
bool func_80060044(POLY_FT4** poly, s32 idx) // 0x80060044
{
    s32              temp_v0_2;
    s32              temp_v1_5;
    s32              temp_v1_5_;
    s32              var_a2;
    s32              var_v0_6;
    s32              temp_a1;
    s32              temp_a2_2;
    u8               temp_s0;
    s_func_80060044* ptr;
    POLY_FT4*        next;

    ptr = PSX_SCRATCH;

    ptr->field_148 = g_MapOverlayHdr.unkTable1_4C[idx].field_10.s_0.field_0;

    if (g_GameWork.config.extraBloodColor == 14)
    {
        g_MapOverlayHdr.unkTable1_4C[idx].field_10.s_0.field_0 += (u16)g_DeltaTime * 4;
    }
    else if (g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_3 == 33)
    {
        g_MapOverlayHdr.unkTable1_4C[idx].field_10.s_0.field_0 += (u16)g_DeltaTime * 2;
    }
    else
    {
        g_MapOverlayHdr.unkTable1_4C[idx].field_10.s_0.field_0 += g_DeltaTime;
    }

    temp_a1 = g_MapOverlayHdr.unkTable1_4C[idx].field_B >> 2;
    if ((u16)g_MapOverlayHdr.unkTable1_4C[idx].field_10.s_0.field_0 >= 5233)
    {
        g_MapOverlayHdr.unkTable1_4C[idx].field_A = 0;

        if (temp_a1 != 24)
        {
            if (D_800C42E8[temp_a1].field_0 & 1)
            {
                D_800C42E8[temp_a1].field_0 -= 1;
            }
        }

        return false;
    }

    if (temp_a1 != 24)
    {
        g_MapOverlayHdr.unkTable1_4C[idx].field_0.vx_0 += ptr->field_0.field_34[temp_a1];
        g_MapOverlayHdr.unkTable1_4C[idx].vy_8 += ptr->field_0.field_64[temp_a1];
        g_MapOverlayHdr.unkTable1_4C[idx].field_4.vz_4 += ptr->field_0.field_94[temp_a1];
    }

    Math_SetSVectorFastSum(&ptr->field_138,
                           Q12_TO_Q8(g_MapOverlayHdr.unkTable1_4C[idx].field_0.vx_0) - (u16)ptr->field_0.field_0.vx,
                           Q12_TO_Q8(g_MapOverlayHdr.unkTable1_4C[idx].vy_8) - ptr->field_0.field_0.vy,
                           Q12_TO_Q8(g_MapOverlayHdr.unkTable1_4C[idx].field_4.vz_4) - ptr->field_0.field_0.vz);

    gte_ldv0(&ptr->field_138);
    gte_rtps();
    gte_stsxy(&ptr->field_144);
    gte_stsz(&ptr->field_140);

    temp_v0_2 = ptr->field_140 - g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_3;

    if (temp_v0_2 <= 0 || (temp_v0_2 >> 3) >= ORDERING_TABLE_SIZE ||
        ABS(ptr->field_144.vx) > 200 ||
        ABS(ptr->field_144.vy) > 160)
    {
        return false;
    }

    ptr->field_14C = ((g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_1 * 3) + ((ptr->field_148 * 2) / 327) +
                      (g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_0 < 3 ? g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_1 : 0)) *
                     ptr->field_0.field_2C / ptr->field_140;

    setPolyFT4(*poly);

    setXY0Fast(*poly, (u16)ptr->field_144.vx - (u16)ptr->field_14C, ptr->field_144.vy - ptr->field_14C);
    setXY1Fast(*poly, (u16)ptr->field_144.vx + (u16)ptr->field_14C, ptr->field_144.vy - ptr->field_14C);
    setXY2Fast(*poly, (u16)ptr->field_144.vx - (u16)ptr->field_14C, ptr->field_144.vy + ptr->field_14C);
    setXY3Fast(*poly, (u16)ptr->field_144.vx + (u16)ptr->field_14C, ptr->field_144.vy + ptr->field_14C);

    var_a2    = (g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_0) >> 3;
    temp_a2_2 = var_a2;

    temp_v1_5_ = temp_a2_2 ^ 0x1;
    temp_v1_5_ = temp_v1_5_ & 0x1;

    temp_v1_5 = (u32)temp_a2_2 >> 1;
    temp_v1_5 = temp_v1_5 ^ 0x1;
    temp_v1_5 = temp_v1_5 & 0x1;

    temp_v1_5 = temp_v1_5_ ^ temp_v1_5;

    if ((!(temp_a2_2 & 4) && (temp_v1_5 ^ 1) != 0) || ((temp_a2_2 & 4) && (temp_v1_5) != 0))
    {
        var_v0_6 = 0;
    }
    else
    {
        var_v0_6 = 0x1F;
    }
    ptr->field_154 = var_v0_6;

    if (temp_a2_2 >= 4)
    {
        ptr->field_168 = 0x1F;
    }
    else
    {
        ptr->field_168 = 0;
    }

    temp_v1_5 = temp_a2_2 ^ 0x1;
    temp_v1_5 = temp_v1_5 & 0x1;

    if ((!(temp_a2_2 & 4) && (temp_v1_5 ^ 1) != 0) || ((temp_a2_2 & 4) && (temp_v1_5) != 0))
    {
        var_v0_6 = 0;
    }
    else
    {
        var_v0_6 = 0x1F;
    }
    ptr->field_158 = var_v0_6;

    temp_v1_5 = (u32)temp_a2_2 >> 1;
    temp_v1_5 = temp_v1_5 ^ 1;
    temp_v1_5 = temp_v1_5 & 1;

    if ((!(temp_a2_2 & 0x4) && (temp_v1_5 ^ 0x1) != 0) || ((temp_a2_2 & 0x4) && temp_v1_5 != 0))
    {
        var_v0_6 = 0x1F;
    }
    else
    {
        var_v0_6 = 0;
    }
    ptr->field_16C = var_v0_6;

    temp_v1_5 = temp_a2_2 ^ 0x1;
    temp_v1_5 = temp_v1_5 & 0x1;

    if ((!(temp_a2_2 & 0x4) && (temp_v1_5 ^ 0x1) != 0) || ((temp_a2_2 & 0x4) && temp_v1_5 != 0))
    {
        var_v0_6 = 0x1F;
    }
    else
    {
        var_v0_6 = 0;
    }
    ptr->field_15C = var_v0_6;

    temp_v1_5 = (u32)temp_a2_2 >> 1;
    temp_v1_5 = temp_v1_5 ^ 1;
    temp_v1_5 = temp_v1_5 & 1;

    if ((!(temp_a2_2 & 0x4) && (temp_v1_5 ^ 0x1) != 0) || ((temp_a2_2 & 0x4) && temp_v1_5 != 0))
    {
        var_v0_6 = 0;
    }
    else
    {
        var_v0_6 = 0x1F;
    }
    ptr->field_170 = var_v0_6;

    temp_v1_5_ = temp_a2_2 ^ 0x1;
    temp_v1_5_ = temp_v1_5_ & 0x1;

    temp_v1_5 = (u32)temp_a2_2 >> 1;
    temp_v1_5 = temp_v1_5 ^ 0x1;
    temp_v1_5 = temp_v1_5 & 0x1;

    temp_v1_5 = temp_v1_5_ ^ temp_v1_5;

    if ((!(temp_a2_2 & 0x4) && (temp_v1_5 ^ 0x1) != 0) || ((temp_a2_2 & 0x4) && temp_v1_5 != 0))
    {
        var_v0_6 = 0x1F;
    }
    else
    {
        var_v0_6 = 0;
    }
    ptr->field_160 = var_v0_6;

    if (temp_a2_2 < 4)
    {
        ptr->field_174 = 0x1F;
    }
    else
    {
        ptr->field_174 = 0;
    }

    ptr->field_150 = ((g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_0 & 0x7) + 2);
    ptr->field_164 = CLAMP_LOW_THEN_MIN((ptr->field_148 / 327), 0, 7);

    setSemiTrans(*poly, true);

    if (g_GameWork.config.extraBloodColor == 14)
    {
        *(s32*)&(*poly)->u1 = (((ptr->field_164 << 5) + ptr->field_16C) << 8) + 0x2B0000 + ((ptr->field_150 << 5) + ptr->field_158);
        *(s32*)&(*poly)->u2 = (((ptr->field_164 << 5) + ptr->field_170) << 8) + ((ptr->field_150 << 5) + ptr->field_15C);
        *(s32*)&(*poly)->u3 = (((ptr->field_164 << 5) + ptr->field_174) << 8) + ((ptr->field_150 << 5) + ptr->field_160);
    }
    else
    {
        *(s32*)&(*poly)->u1 = (((ptr->field_164 << 5) + ptr->field_16C) << 8) + 0x4B0000 + ((ptr->field_150 << 5) + ptr->field_158);
        *(s32*)&(*poly)->u2 = (((ptr->field_164 << 5) + ptr->field_170) << 8) + ((ptr->field_150 << 5) + ptr->field_15C);
        *(s32*)&(*poly)->u3 = (((ptr->field_164 << 5) + ptr->field_174) << 8) + ((ptr->field_150 << 5) + ptr->field_160);
    }

    if (!(g_SysWork.field_2388.field_154.effectsInfo.flags.field_0 & (SpecialEnvEventFlags_DarkEnvironment | SpecialEnvEventFlags_FlashlightAllowed)))
    {
        if (g_GameWork.config.extraBloodColor == 14)
        {
            func_80055A90(&ptr->field_130, &ptr->field_134, 0x40, ptr->field_140 * 16);

            *(u16*)&(*poly)->r0 = ptr->field_130.r + (ptr->field_130.g << 8);
            (*poly)->b0         = ptr->field_130.b;

            *(s32*)&(*poly)->u0 = (((ptr->field_164 << 5) + ptr->field_154) << 8) + 0x930000 + ((ptr->field_150 << 5) + ptr->field_168);

            *(*poly + 1) = **poly;

            *(u16*)&(*poly + 1)->r0 = ptr->field_134.r + ((ptr->field_134.g >> 4) << 8);
            (*poly + 1)->b0         = ptr->field_134.b >> 4;
            (*poly + 1)->clut       = (g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_2 << 6) | 0x13;

            setSemiTrans(*poly + 1, false);

            addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[(ptr->field_140 - g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_3) >> 3], *poly);
            addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[(ptr->field_140 - g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_3) >> 3], *poly + 1);
            *poly += 2;
        }
        else
        {
            var_a2 = 0xFF - ((ptr->field_148 * 0x10) / 327);
            if (var_a2 < 0)
            {
                var_a2 = 0;
            }
            func_80055A90(&ptr->field_130, &ptr->field_134, var_a2, ptr->field_140 * 16);

            *(u16*)&(*poly)->r0 = ptr->field_130.r + (ptr->field_130.g << 8);
            (*poly)->b0         = ptr->field_130.b;

            *(s32*)&(*poly)->u0 = (((ptr->field_164 << 5) + ptr->field_154) << 8) + 0x930000 + ((ptr->field_150 << 5) + ptr->field_168);

            *(*poly + 3) = *(*poly + 2) = *(*poly + 1) = **poly;

            *(u16*)&(*poly + 1)->r0 = ptr->field_134.r + (ptr->field_134.g << 8);
            (*poly + 1)->b0         = ptr->field_134.b;
            (*poly)->tpage          = 43;
            (*poly + 1)->clut       = (g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_2 << 6) | 0x13;
            (*poly + 3)->tpage      = 43;

            ptr->field_12C = (PACKET*)*poly + 0xA0;
            SetPriority(ptr->field_12C, 0, 0);
            SetPriority(ptr->field_12C + 0xC, 1, 1);

            addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[(ptr->field_140 - g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_3) >> 3], *poly);
            addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[(ptr->field_140 - g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_3) >> 3], *poly + 1);
            addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[(ptr->field_140 - g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_3) >> 3], *poly + 2);
            addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[(ptr->field_140 - g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_3) >> 3], ptr->field_12C);
            addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[(ptr->field_140 - g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_3) >> 3], *poly + 3);
            addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[(ptr->field_140 - g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_3) >> 3], ptr->field_12C + 12);
            *poly = ptr->field_12C + 0x18;
        }
        return true;
    }

    if (g_GameWork.config.extraBloodColor == 14)
    {
        temp_s0             = func_80055D78(g_MapOverlayHdr.unkTable1_4C[idx].field_0.vx_0, g_MapOverlayHdr.unkTable1_4C[idx].vy_8, g_MapOverlayHdr.unkTable1_4C[idx].field_4.vz_4);
        *(u16*)&(*poly)->r0 = temp_s0 + (func_80055D78(g_MapOverlayHdr.unkTable1_4C[idx].field_0.vx_0, g_MapOverlayHdr.unkTable1_4C[idx].vy_8, g_MapOverlayHdr.unkTable1_4C[idx].field_4.vz_4) << 8);
        (*poly)->b0         = func_80055D78(g_MapOverlayHdr.unkTable1_4C[idx].field_0.vx_0, g_MapOverlayHdr.unkTable1_4C[idx].vy_8, g_MapOverlayHdr.unkTable1_4C[idx].field_4.vz_4);
    }
    else
    {
        var_a2 = 0xFF - (ptr->field_148 * 16) / 327;
        if (var_a2 < 0)
        {
            var_a2 = 0;
        }
        *(u16*)&(*poly)->r0 = var_a2 + (var_a2 << 8);
        (*poly)->b0         = var_a2;
    }

    *(s32*)&(*poly)->u0 = (((g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_2 << 6) | 0x13) << 16) + (((ptr->field_164 << 5) + ptr->field_168) << 8) + ((ptr->field_150 << 5) + ptr->field_154);

    addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[(ptr->field_140 - g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_3) >> 3], *poly);

    *poly += 1;

    return true;
}
#endif

#if VERSION_IS(EUR)
INCLUDE_ASM("bodyprog/nonmatchings/gfx/bodyprog_effects_8005E0DC", func_800611C0);
#else
bool func_800611C0(POLY_FT4** poly, s32 idx) // 0x800611C0
{
    s_func_800611C0    sp10;
    s_CollisionSurface surfaces[4];
    s16                temp_v1_6;
    s32                temp_v1_15;
    s32                temp_v1_15_;
    s32                var_t0;
    s32                var_v0_6;
    u32                idx0;
    u32                var_a0_3;
    s32                temp;
    s_func_800611C0*   ptr;

    ptr = PSX_SCRATCH;

    if (!(g_MapOverlayHdr.unkTable1_4C[idx].field_B & 0x3))
    {
        idx0 = g_MapOverlayHdr.unkTable1_4C[idx].field_B >> 2;

        if (idx0 != 24)
        {
            g_MapOverlayHdr.unkTable1_4C[idx].field_0.vx_0 += ptr->field_0.field_34[idx0];
            g_MapOverlayHdr.unkTable1_4C[idx].vy_8 += ptr->field_0.field_64[idx0];
            g_MapOverlayHdr.unkTable1_4C[idx].field_4.vz_4 += ptr->field_0.field_94[idx0];
        }

        g_MapOverlayHdr.unkTable1_4C[idx].field_10.s_0.field_0 = MIN(g_MapOverlayHdr.unkTable1_4C[idx].field_10.s_0.field_0 + (g_GravitySpeed >> 2), 0x2000);

        g_MapOverlayHdr.unkTable1_4C[idx].vy_8 += g_MapOverlayHdr.unkTable1_4C[idx].field_10.s_0.field_0;

        if (g_MapOverlayHdr.unkTable1_4C[idx].vy_8 > g_MapOverlayHdr.unkTable1_4C[idx].field_10.s_0.field_2)
        {
            if (idx0 != 24)
            {
                if (D_800C42E8[idx0].field_0 & (1 << 1))
                {
                    D_800C42E8[idx0].field_0 -= 2;
                }
            }

            g_MapOverlayHdr.unkTable1_4C[idx].vy_8 = g_MapOverlayHdr.unkTable1_4C[idx].field_10.s_0.field_2;

            sp10 = *ptr;

            ptr->field_178 = g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_1 << 5;

            Collision_SurfaceGet(&surfaces[0], g_MapOverlayHdr.unkTable1_4C[idx].field_0.vx_0 + ptr->field_178, g_MapOverlayHdr.unkTable1_4C[idx].field_4.vz_4);
            Collision_SurfaceGet(&surfaces[1], g_MapOverlayHdr.unkTable1_4C[idx].field_0.vx_0 - ptr->field_178, g_MapOverlayHdr.unkTable1_4C[idx].field_4.vz_4);
            Collision_SurfaceGet(&surfaces[2], g_MapOverlayHdr.unkTable1_4C[idx].field_0.vx_0, g_MapOverlayHdr.unkTable1_4C[idx].field_4.vz_4 + ptr->field_178);
            Collision_SurfaceGet(&surfaces[3], g_MapOverlayHdr.unkTable1_4C[idx].field_0.vx_0, g_MapOverlayHdr.unkTable1_4C[idx].field_4.vz_4 - ptr->field_178);

            if (func_8005F680(&surfaces[0]) || func_8005F680(&surfaces[1]) || func_8005F680(&surfaces[2]) || func_8005F680(&surfaces[3]) ||
                (g_MapOverlayHdr.unkTable1_4C[idx].vy_8 - surfaces[0].floorHeight) < Q12(-0.2f) ||
                (g_MapOverlayHdr.unkTable1_4C[idx].vy_8 - surfaces[1].floorHeight) < Q12(-0.2f) ||
                (g_MapOverlayHdr.unkTable1_4C[idx].vy_8 - surfaces[2].floorHeight) < Q12(-0.2f) ||
                (g_MapOverlayHdr.unkTable1_4C[idx].vy_8 - surfaces[3].floorHeight) < Q12(-0.2f))
            {
                g_MapOverlayHdr.unkTable1_4C[idx].field_A = 0;
            }
            else
            {
                g_MapOverlayHdr.unkTable1_4C[idx].field_B          = 1;
                g_MapOverlayHdr.unkTable1_4C[idx].field_10.field_0 = Rng_GenerateUInt(0, 255);
            }

            *ptr = sp10;

            gte_SetRotMatrix(&ptr->field_0.worldToScreenMat);
            gte_SetTransMatrix(&ptr->field_0.worldToScreenMat);
        }

        return true;
    }

    ptr->field_168 = g_MapOverlayHdr.unkTable1_4C[idx].field_10.field_0;

    if (g_MapOverlayHdr.unkTable1_4C[idx].field_B == 2)
    {
        if (*g_MapOverlayHdr.data_190 != 0)
        {
            g_MapOverlayHdr.unkTable1_4C[idx].field_10.field_0 += g_DeltaTime;
            if (g_MapOverlayHdr.unkTable1_4C[idx].field_10.field_0 > Q12(50.0f))
            {
                g_MapOverlayHdr.unkTable1_4C[idx].field_10.field_0 = Q12(50.0f);
            }

            ptr->field_168 = 0xCC0;
        }
    }
    else
    {
        g_MapOverlayHdr.unkTable1_4C[idx].field_10.field_0 += g_DeltaTime;

        if (g_MapOverlayHdr.unkTable1_4C[idx].field_10.field_0 > 0xCC0)
        {
            if (g_MapOverlayHdr.unkTable1_4C[idx].field_B == 1)
            {
                g_MapOverlayHdr.unkTable1_4C[idx].field_10.field_0    = 0xCC0;
                g_MapOverlayHdr.unkTable1_4C[idx].field_B             = 2;
                g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_3 = 0x80;
            }
            else
            {
                g_MapOverlayHdr.unkTable1_4C[idx].field_A = 0;
                return false;
            }
        }
    }

    vwGetViewPosition(&ptr->field_14C);

    if (ABS(ptr->field_14C.vx - g_MapOverlayHdr.unkTable1_4C[idx].field_0.vx_0) +
        ABS(ptr->field_14C.vz - g_MapOverlayHdr.unkTable1_4C[idx].field_4.vz_4) > Q12(20.0f))
    {
        if (g_MapOverlayHdr.unkTable1_4C[idx].field_B == 3)
        {
            g_MapOverlayHdr.unkTable1_4C[idx].field_A = 0;
        }

        return false;
    }

    if ((u32)(g_MapOverlayHdr.unkTable1_4C[idx].field_B & 3) < 3u)
    {
        temp           = g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_1 * 16;
        temp           = (ptr->field_168 * temp) / 816;
        ptr->field_178 = temp < (g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_1 << 5) ? temp : (g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_1 << 5);
    }
    else
    {
        ptr->field_178 = g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_1 << 5;
    }

    setPolyFT4(*poly);

    Math_SetSVectorFastSum(&ptr->field_134,
                           Q12_TO_Q8(g_MapOverlayHdr.unkTable1_4C[idx].field_0.vx_0 - ptr->field_178) - (u16)ptr->field_0.field_0.vx,
                           Q12_TO_Q8(g_MapOverlayHdr.unkTable1_4C[idx].vy_8) - ptr->field_0.field_0.vy,
                           Q12_TO_Q8(g_MapOverlayHdr.unkTable1_4C[idx].field_4.vz_4 + ptr->field_178) - ptr->field_0.field_0.vz);
    Math_SetSVectorFastSum(&ptr->field_13C,
                           Q12_TO_Q8(g_MapOverlayHdr.unkTable1_4C[idx].field_0.vx_0 + ptr->field_178) - (u16)ptr->field_0.field_0.vx,
                           Q12_TO_Q8(g_MapOverlayHdr.unkTable1_4C[idx].vy_8) - ptr->field_0.field_0.vy,
                           Q12_TO_Q8(g_MapOverlayHdr.unkTable1_4C[idx].field_4.vz_4 + ptr->field_178) - ptr->field_0.field_0.vz);
    Math_SetSVectorFastSum(&ptr->field_144,
                           Q12_TO_Q8(g_MapOverlayHdr.unkTable1_4C[idx].field_0.vx_0 - ptr->field_178) - (u16)ptr->field_0.field_0.vx,
                           Q12_TO_Q8(g_MapOverlayHdr.unkTable1_4C[idx].vy_8) - ptr->field_0.field_0.vy,
                           Q12_TO_Q8(g_MapOverlayHdr.unkTable1_4C[idx].field_4.vz_4 - ptr->field_178) - ptr->field_0.field_0.vz);

    gte_ldv3c(&ptr->field_134);
    gte_rtpt();
    gte_stsxy3_g3(*poly);
    gte_stsz3c(&ptr->field_158);

    Math_SetSVectorFastSum(&ptr->field_134,
                           Q12_TO_Q8(g_MapOverlayHdr.unkTable1_4C[idx].field_0.vx_0 + ptr->field_178) - (u16)ptr->field_0.field_0.vx,
                           Q12_TO_Q8(g_MapOverlayHdr.unkTable1_4C[idx].vy_8) - ptr->field_0.field_0.vy,
                           Q12_TO_Q8(g_MapOverlayHdr.unkTable1_4C[idx].field_4.vz_4 - ptr->field_178) - ptr->field_0.field_0.vz);

    gte_ldv0(&ptr->field_134);
    gte_rtps();
    gte_stsxy(&ptr->field_16C);
    gte_stsz(&ptr->field_164);

    ptr->field_158 = (ptr->field_158 + ptr->field_15C + ptr->field_160 + ptr->field_164) >> 2;

    if ((ptr->field_158 - 8) <= 0 || ((ptr->field_158 - 8) >> 3) >= ORDERING_TABLE_SIZE)
    {
        return false;
    }

    if (ABS(ptr->field_16C.vx) > 200)
    {
        return false;
    }

    if (ABS(ptr->field_16C.vy) > 160)
    {
        return false;
    }

    *(s32*)&(*poly)->x3 = *(s32*)&ptr->field_16C;

    ptr->field_17C = !(g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_0 & 8) << 5;

    if (g_MapOverlayHdr.unkTable1_4C[idx].field_B == 3)
    {
        ptr->field_190 = 224;
    }
    else
    {
        ptr->field_190 = MIN(ptr->field_168 / 408, 7) << 5;
    }

    var_t0 = g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_0 & 7;

    temp_v1_15_ = var_t0 ^ 0x1;
    temp_v1_15_ = temp_v1_15_ & 0x1;

    temp_v1_15 = (u32)var_t0 >> 1;
    temp_v1_15 = temp_v1_15 ^ 0x1;
    temp_v1_15 = temp_v1_15 & 0x1;

    temp_v1_15 = temp_v1_15_ ^ temp_v1_15;

    if ((!(var_t0 & 0x4) && (temp_v1_15 ^ 0x1) != 0) || ((var_t0 & 0x4) && temp_v1_15 != 0))
    {
        var_v0_6 = 0;
    }
    else
    {
        var_v0_6 = 0x1F;
    }
    ptr->field_180 = var_v0_6;

    if (var_t0 >= 4)
    {
        ptr->field_194 = 0x1F;
    }
    else
    {
        ptr->field_194 = 0;
    }

    temp_v1_15 = var_t0 ^ 0x1;
    temp_v1_15 = temp_v1_15 & 0x1;

    if ((!(var_t0 & 0x4) && (temp_v1_15 ^ 0x1) != 0) || ((var_t0 & 0x4) && temp_v1_15 != 0))
    {
        var_v0_6 = 0;
    }
    else
    {
        var_v0_6 = 0x1F;
    }
    ptr->field_184 = var_v0_6;

    temp_v1_15 = (u32)var_t0 >> 1;
    temp_v1_15 = temp_v1_15 ^ 1;
    temp_v1_15 = temp_v1_15 & 1;

    if ((!(var_t0 & 0x4) && (temp_v1_15 ^ 0x1) != 0) || ((var_t0 & 0x4) && temp_v1_15 != 0))
    {
        var_v0_6 = 0x1F;
    }
    else
    {
        var_v0_6 = 0;
    }
    ptr->field_198 = var_v0_6;

    temp_v1_15 = var_t0 ^ 1;
    temp_v1_15 = temp_v1_15 & 1;

    if ((!(var_t0 & 0x4) && (temp_v1_15 ^ 0x1) != 0) || ((var_t0 & 0x4) && temp_v1_15 != 0))
    {
        var_v0_6 = 0x1F;
    }
    else
    {
        var_v0_6 = 0;
    }
    ptr->field_188 = var_v0_6;

    temp_v1_15 = (u32)var_t0 >> 1;
    temp_v1_15 = temp_v1_15 ^ 1;
    temp_v1_15 = temp_v1_15 & 1;

    if ((!(var_t0 & 0x4) && (temp_v1_15 ^ 0x1) != 0) || ((var_t0 & 0x4) && temp_v1_15 != 0))
    {
        var_v0_6 = 0;
    }
    else
    {
        var_v0_6 = 0x1F;
    }
    ptr->field_19C = var_v0_6;

    temp_v1_15_ = var_t0 ^ 0x1;
    temp_v1_15_ = temp_v1_15_ & 0x1;

    temp_v1_15 = (u32)var_t0 >> 1;
    temp_v1_15 = temp_v1_15 ^ 0x1;
    temp_v1_15 = temp_v1_15 & 0x1;

    temp_v1_15 = temp_v1_15_ ^ temp_v1_15;

    if ((!(var_t0 & 0x4) && (temp_v1_15 ^ 0x1) != 0) || ((var_t0 & 0x4) && temp_v1_15 != 0))
    {
        var_v0_6 = 0x1F;
    }
    else
    {
        var_v0_6 = 0;
    }
    ptr->field_18C = var_v0_6;

    if (var_t0 < 4)
    {
        ptr->field_1A0 = 0x1F;
    }
    else
    {
        ptr->field_1A0 = 0;
    }

    if (g_MapOverlayHdr.unkTable1_4C[idx].field_B == 1)
    {
        var_t0 = 0xFF - ((ptr->field_168 * 0x10) / 204);
        var_t0 = MAX(var_t0, 0x80);
    }
    else if (g_MapOverlayHdr.unkTable1_4C[idx].field_B == 2)
    {
        if (*g_MapOverlayHdr.data_190 != 0)
        {
            temp_v1_6                                                 = (0x80 - (((g_MapOverlayHdr.unkTable1_4C[idx].field_10.field_0 - 0xCC0) << 7) / 204800)) < 0x20;
            var_a0_3                                                  = !(temp_v1_6) ? (0x80 - (((g_MapOverlayHdr.unkTable1_4C[idx].field_10.field_0 - 0xCC0) << 7) / 204800)) : 0x20;
            g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_3 = var_a0_3;
            var_t0                                                    = Q8_FRACT(var_a0_3);
        }
        else
        {
            var_t0 = g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_3;
        }
    }
    else
    {
        var_t0 = g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_3 - ((ptr->field_168 * 8) / 204);
        var_t0 = MAX(var_t0, 0);
    }

    *(s32*)&(*poly)->u1 = ((ptr->field_190 + ptr->field_198) << 8) + 0x4B0000 + (ptr->field_17C + ptr->field_184);
    *(s32*)&(*poly)->u2 = ((ptr->field_190 + ptr->field_19C) << 8) + (ptr->field_17C + ptr->field_188);
    *(s32*)&(*poly)->u3 = ((ptr->field_190 + ptr->field_1A0) << 8) + (ptr->field_17C + ptr->field_18C);

    setSemiTrans(*poly, true);

    if (!(g_SysWork.field_2388.field_154.effectsInfo.flags.field_0 & (SpecialEnvEventFlags_DarkEnvironment | SpecialEnvEventFlags_FlashlightAllowed)))
    {
        *(s32*)&(*poly)->u0 = ((ptr->field_190 + ptr->field_194) << 8) + 0x930000 + (ptr->field_17C + ptr->field_180);
        func_80055A90(&ptr->field_12C, &ptr->field_130, var_t0, ptr->field_158 * 16);
        *(u16*)&(*poly)->r0 = ptr->field_12C.r + (ptr->field_12C.g << 8);
        (*poly)->b0         = ptr->field_12C.b;

        *(*poly + 2) = *(*poly + 1) = **poly;

        (*poly)->tpage          = 0x2B;
        (*poly + 1)->clut       = (g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_2 << 6) | 0x13;
        *(u16*)&(*poly + 1)->r0 = ptr->field_130.r + (ptr->field_130.g << 8);
        (*poly + 1)->b0         = ptr->field_130.b;

        addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[ptr->field_158 >> 3], *poly);
        addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[ptr->field_158 >> 3], *poly + 1);
        addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[ptr->field_158 >> 3], *poly + 2);

        *poly = *poly + 3;
    }
    else
    {
        *(s32*)&(*poly)->u0 = (((g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_2 << 6) | 0x13) << 16) +
                              ((ptr->field_190 + ptr->field_194) << 8) + (ptr->field_17C + ptr->field_180);
        *(u16*)&(*poly)->r0 = var_t0 + (var_t0 << 8);
        (*poly)->b0         = var_t0;

        addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[ptr->field_158 >> 3], *poly);

        *poly = *poly + 1;
    }

    return true;
}
#endif

void func_800622B8(s32 unused, s_SubCharacter* chara, s32 animStatus, s32 arg3) // 0x800622B8
{
    s_CollisionSurface surface;
    s32                temp_s0;
    s32                temp_s2;
    s32                temp_s3;
    s32                idx;
    s32                i;

    if (g_GameWork.config.extraBloodColor == 14)
    {
        return;
    }

    // Related to blood.
    arg3 = func_8005F55C(arg3);

    i = D_800AE5CC[animStatus + 1] - D_800AE5CC[animStatus];
    while (i > 0)
    {
        idx = func_8005E7E0(3);
        if (idx == NO_VALUE)
        {
            break;
        }

        temp_s0 = (D_800AE5CC[animStatus] + i) - 1;

        temp_s3 = Rng_Rand16() % D_800AE5F0[temp_s0 * 4]       + D_800AE5F0[(temp_s0 * 4) + 1];
        temp_s2 = Rng_Rand16() % D_800AE5F0[(temp_s0 * 4) + 2] + D_800AE5F0[(temp_s0 * 4) + 3];

        g_MapOverlayHdr.unkTable1_4C[idx].field_0.vx_0 = chara->position.vx +
                                                            Q12_MULT(temp_s3, Math_Sin(chara->rotation.vy)) -
                                                            Q12_MULT(temp_s2, Math_Cos(chara->rotation.vy));
        g_MapOverlayHdr.unkTable1_4C[idx].vy_8 = chara->position.vy;
        g_MapOverlayHdr.unkTable1_4C[idx].field_4.vz_4 = chara->position.vz +
                                                            Q12_MULT(temp_s3, Math_Cos(chara->rotation.vy)) +
                                                            Q12_MULT(temp_s2, Math_Sin(chara->rotation.vy));

        Collision_SurfaceGet(&surface, g_MapOverlayHdr.unkTable1_4C[idx].field_0.vx_0, g_MapOverlayHdr.unkTable1_4C[idx].field_4.vz_4);

        if (ABS_DIFF(surface.floorHeight, chara->position.vy) > Q12(0.15f))
        {
            g_MapOverlayHdr.unkTable1_4C[(idx)].field_A = 0;
        }
        else
        {
            g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_0.field_2 = D_800AE700[animStatus] + (Rng_Rand16() % (D_800AE700[animStatus] >> 2));
            g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_1 = arg3;
            g_MapOverlayHdr.unkTable1_4C[idx].field_B             = Rng_GenerateUInt(4, 7);
            g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_0 = Chara_NpcIdxGet(chara);
            g_MapOverlayHdr.unkTable1_4C[idx].field_10.field_0    = Rng_GenerateUInt(0, 8191);
        }

        i--;
    }
}

void func_800625F4(VECTOR3* arg0, q3_12 angle, s32 arg2, s32 arg3) // 0x800625F4
{
    s32 idx;
    s8  var;

    var = func_8005F55C(arg2);

    idx = func_8005E7E0(4);
    if (idx == NO_VALUE)
    {
        return;
    }

    g_MapOverlayHdr.unkTable1_4C[idx].field_0.vx_0        = arg0->vx;
    g_MapOverlayHdr.unkTable1_4C[idx].vy_8                = arg0->vy;
    g_MapOverlayHdr.unkTable1_4C[idx].field_4.vz_4        = arg0->vz;
    g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_0.field_2 = angle;
    g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_1 = var;
    g_MapOverlayHdr.unkTable1_4C[idx].field_B             = Rng_GenerateUInt(0, 3);
    g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_0 = 6;
    g_MapOverlayHdr.unkTable1_4C[idx].field_10.field_0    = arg3 * Q12(5.0f);
}

#if VERSION_IS(EUR)
INCLUDE_ASM("bodyprog/nonmatchings/gfx/bodyprog_effects_8005E0DC", func_80062708);
#else
bool func_80062708(POLY_FT4** poly, s32 idx) // 0x80062708
{
    s_func_80062708    sp10;
    s_CollisionSurface surfaces[4];
    s32                temp_a1_3;
    s32                temp_s2;
    s32                j;
    s32                i;
    s32                var_s7;
    u32                temp_v1_6;
    s_func_80062708*   ptr;

    ptr = PSX_SCRATCH;

    if (g_MapOverlayHdr.unkTable1_4C[idx].field_A == 3)
    {
        var_s7 = 0xC;
        if (g_SysWork.npcs[g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_0].model.charaId == Chara_None)
        {
            g_MapOverlayHdr.unkTable1_4C[idx].field_A = 0;
            return false;
        }
    }
    else
    {
        var_s7 = 0x100;
    }

    if (g_MapOverlayHdr.unkTable1_4C[idx].field_B & (1 << 2))
    {
        g_MapOverlayHdr.unkTable1_4C[idx].field_10.field_0 += g_DeltaTime;
        if (g_MapOverlayHdr.unkTable1_4C[idx].field_10.field_0 > Q12(2.5f))
        {
            g_MapOverlayHdr.unkTable1_4C[idx].field_B         -= 4;
            g_MapOverlayHdr.unkTable1_4C[idx].field_10.field_0 = 0;
        }

        return false;
    }

    ptr->field_208 = g_MapOverlayHdr.unkTable1_4C[idx].field_10.field_0;

    if (*g_MapOverlayHdr.data_190 != NULL)
    {
        g_MapOverlayHdr.unkTable1_4C[idx].field_10.field_0 += g_DeltaTime;
        if (g_MapOverlayHdr.unkTable1_4C[idx].field_10.field_0 > Q12(45.0f))
        {
            g_MapOverlayHdr.unkTable1_4C[idx].field_10.field_0 = Q12(45.0f);
        }
    }
    else
    {
        if (g_MapOverlayHdr.unkTable1_4C[idx].field_10.field_0 < Q12(5.0f))
        {
            g_MapOverlayHdr.unkTable1_4C[idx].field_10.field_0 += g_DeltaTime;
        }
    }

    vwGetViewPosition(&ptr->field_1FC);

    if ((ABS(ptr->field_1FC.vx - g_MapOverlayHdr.unkTable1_4C[idx].field_0.vx_0) +
         ABS(ptr->field_1FC.vz - g_MapOverlayHdr.unkTable1_4C[idx].field_4.vz_4)) > Q12(20.0f))
    {
        return false;
    }

    if (ptr->field_208 >= Q12(5.0f))
    {
        ptr->field_2DC = (u16)g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_0.field_2;
    }
    else
    {
        ptr->field_2DC = (ptr->field_208 * (u16)g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_0.field_2) / Q12(5.0f);
        temp_s2        = (ptr->field_2DC >> 1) * 16; // To Q12.

        sp10 = *ptr;

        if (Rng_GenerateUInt(0, 1) != 0) // 1 in 2 chance.
        {
            Collision_SurfaceGet(&surfaces[0], g_MapOverlayHdr.unkTable1_4C[idx].field_0.vx_0 - temp_s2, g_MapOverlayHdr.unkTable1_4C[idx].field_4.vz_4);
            Collision_SurfaceGet(&surfaces[1], g_MapOverlayHdr.unkTable1_4C[idx].field_0.vx_0 + temp_s2, g_MapOverlayHdr.unkTable1_4C[idx].field_4.vz_4);
            Collision_SurfaceGet(&surfaces[2], g_MapOverlayHdr.unkTable1_4C[idx].field_0.vx_0, g_MapOverlayHdr.unkTable1_4C[idx].field_4.vz_4 - temp_s2);
            Collision_SurfaceGet(&surfaces[3], g_MapOverlayHdr.unkTable1_4C[idx].field_0.vx_0, g_MapOverlayHdr.unkTable1_4C[idx].field_4.vz_4 + temp_s2);
        }
        else
        {
            Collision_SurfaceGet(&surfaces[0], g_MapOverlayHdr.unkTable1_4C[idx].field_0.vx_0 - temp_s2, g_MapOverlayHdr.unkTable1_4C[idx].field_4.vz_4 - temp_s2);
            Collision_SurfaceGet(&surfaces[1], g_MapOverlayHdr.unkTable1_4C[idx].field_0.vx_0 + temp_s2, g_MapOverlayHdr.unkTable1_4C[idx].field_4.vz_4 - temp_s2);
            Collision_SurfaceGet(&surfaces[2], g_MapOverlayHdr.unkTable1_4C[idx].field_0.vx_0 - temp_s2, g_MapOverlayHdr.unkTable1_4C[idx].field_4.vz_4 + temp_s2);
            Collision_SurfaceGet(&surfaces[3], g_MapOverlayHdr.unkTable1_4C[idx].field_0.vx_0 + temp_s2, g_MapOverlayHdr.unkTable1_4C[idx].field_4.vz_4 + temp_s2);
        }

        if (func_8005F680(&surfaces[0]) ||
            func_8005F680(&surfaces[1]) ||
            func_8005F680(&surfaces[2]) ||
            func_8005F680(&surfaces[3]) ||
            ABS(surfaces[0].floorHeight - g_MapOverlayHdr.unkTable1_4C[idx].vy_8) > Q12(0.15f) ||
            ABS(surfaces[1].floorHeight - g_MapOverlayHdr.unkTable1_4C[idx].vy_8) > Q12(0.15f) ||
            ABS(surfaces[2].floorHeight - g_MapOverlayHdr.unkTable1_4C[idx].vy_8) > Q12(0.15f) ||
            ABS(surfaces[3].floorHeight - g_MapOverlayHdr.unkTable1_4C[idx].vy_8) > Q12(0.15f))
        {
            g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_0.field_2  = ptr->field_2DC;
            g_MapOverlayHdr.unkTable1_4C[idx].field_10.field_0 = Q12(5.0f);
        }

        *ptr = sp10;

        gte_SetRotMatrix(&ptr->field_0.worldToScreenMat);
        gte_SetTransMatrix(&ptr->field_0.worldToScreenMat);
    }

    ptr->field_210 = (g_MapOverlayHdr.unkTable1_4C[idx].field_B << 13) + 224;

    for (i = 0; i < 5; i++)
    {
        for (j = 0; j < 5; j++)
        {
            Math_SetSVectorFastSum(&ptr->field_134[(i * 5) + j],
                                   ((Q12_TO_Q8(g_MapOverlayHdr.unkTable1_4C[idx].field_0.vx_0) - (u16)ptr->field_0.field_0.vx) - (u16)ptr->field_2DC) + ((ptr->field_2DC >> 1) * j),
                                   Q12_TO_Q8(g_MapOverlayHdr.unkTable1_4C[idx].vy_8) - ptr->field_0.field_0.vy,
                                   ((Q12_TO_Q8(g_MapOverlayHdr.unkTable1_4C[idx].field_4.vz_4) - ptr->field_0.field_0.vz) - ptr->field_2DC) + ((ptr->field_2DC >> 1) * i));
        }
    }

    for (i = 0; i < 24; i += 3)
    {
        gte_ldv3c(&ptr->field_134[i]);
        gte_rtpt();
        gte_stsxy3c(&ptr->field_278[i]);
        gte_stsz3c(&ptr->field_214[i]);
    }

    gte_ldv0(&ptr->field_134[24]);
    gte_rtps();
    gte_stsxy(&ptr->field_278[24]);
    gte_stsz(&ptr->field_214[24]);

    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            temp_a1_3 = ((i * 4) + i) + j;

            if (ABS(ptr->field_278[temp_a1_3].vx) > 200)
            {
                continue;
            }

            if (ABS(ptr->field_278[temp_a1_3].vy) > 160)
            {
                continue;
            }

            ptr->field_20C = (ptr->field_214[temp_a1_3] + ptr->field_214[temp_a1_3 + 1] + ptr->field_214[temp_a1_3 + 5] + ptr->field_214[temp_a1_3 + 6]) >> 2;

            if (ptr->field_20C <= 0 || ptr->field_20C >> 3 >= ORDERING_TABLE_SIZE)
            {
                continue;
            }

            setPolyFT4(*poly);

            *(s32*)&(*poly)->x0 = *(s32*)&ptr->field_278[temp_a1_3];
            *(s32*)&(*poly)->x1 = *(s32*)&ptr->field_278[temp_a1_3 + 1];
            *(s32*)&(*poly)->x2 = *(s32*)&ptr->field_278[temp_a1_3 + 5];
            *(s32*)&(*poly)->x3 = *(s32*)&ptr->field_278[temp_a1_3 + 6];

            temp_s2             = (j * 8) + (i << 11) + ptr->field_210;
            *(s32*)&(*poly)->u0 = (((g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_1 << 6) | 0x13) << 16) + temp_s2;
            *(s32*)&(*poly)->u1 = temp_s2 + 0x4B0007;

            setSemiTrans(*poly, true);

            *(s32*)&(*poly)->u2 = temp_s2 + 0x700;
            *(s32*)&(*poly)->u3 = temp_s2 + 0x707;

            if (ptr->field_208 > Q12(5.0f))
            {
                temp_v1_6 = 160 - ((ptr->field_208 - Q12(5.0f)) >> 10);
                if (temp_v1_6 >= 32)
                {
                    temp_s2 = temp_v1_6;
                }
                else
                {
                    temp_s2 = 32;
                }
            }
            else
            {
                temp_s2 = 160;
            }

            if (!(g_SysWork.field_2388.field_154.effectsInfo.flags.field_0 & (SpecialEnvEventFlags_DarkEnvironment | SpecialEnvEventFlags_FlashlightAllowed)))
            {
                func_80055A90(&ptr->field_12C, &ptr->field_130, temp_s2, ptr->field_20C * 0x10);
                *(u16*)&(*poly)->r0 = ptr->field_12C.r + (ptr->field_12C.g << 8);
                (*poly)->b0         = ptr->field_12C.b;

                *(*poly + 2) = *(*poly + 1) = **poly;

                (*poly)->tpage = 43;
                (*poly)->clut = (*poly + 2)->clut = 147;
                *(u16*)&(*poly + 1)->r0           = ptr->field_130.r + (ptr->field_130.g << 8);
                (*poly + 1)->b0                   = ptr->field_130.b;

                addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[(ptr->field_20C + var_s7) >> 3], *poly);
                addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[(ptr->field_20C + var_s7) >> 3], *poly + 1);
                addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[(ptr->field_20C + var_s7) >> 3], *poly + 2);

                *poly = *poly + 3;
            }
            else
            {
                *(u16*)&(*poly)->r0 = temp_s2 + (temp_s2 << 8);
                (*poly)->b0         = temp_s2;

                addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[(ptr->field_20C + var_s7) >> 3], *poly);

                *poly = *poly + 1;
            }
        }
    }

    return true;
}
#endif

void func_8006342C(s32 weaponAttack, q3_12 rotY, q3_12 rotX, GsCOORDINATE2* coord) // 0x8006342C
{
    s32              i;
    s_func_8006342C* ptr;

    ptr = PSX_SCRATCH;

    switch (weaponAttack)
    {
        case WEAPON_ATTACK(EquippedWeaponId_Handgun, AttackInputType_Tap):
            D_800C440C = coord;
            Math_SetSVectorFast(&ptr->field_20, 2, -15, 71);
            break;

        case WEAPON_ATTACK(EquippedWeaponId_Shotgun, AttackInputType_Tap):
            D_800C440C = coord;
            Math_SetSVectorFast(&ptr->field_20, 3, -18, 109);
            break;

        case WEAPON_ATTACK(EquippedWeaponId_HuntingRifle, AttackInputType_Tap):
            D_800C440C = coord;
            Math_SetSVectorFast(&ptr->field_20, -2, -35, 221);
            break;

        case 70: // TODO: `WEAPON_ATTACK`
            D_800C4410 = coord;
            Math_SetSVectorFast(&ptr->field_20, 3, -24, 91);
            break;

        case 63: // TODO: `WEAPON_ATTACK`
            D_800C4410 = coord;
            Math_SetSVectorFast(&ptr->field_20, 2, -13, 67);
            break;

        case WEAPON_ATTACK(EquippedWeaponId_Chainsaw, AttackInputType_Tap):
            D_800C440C = coord;
            Math_SetSVectorFast(&ptr->field_20, 2, 20, 57);
            break;

        case WEAPON_ATTACK(EquippedWeaponId_RockDrill, AttackInputType_Tap):
            D_800C440C = coord;
            Math_SetSVectorFast(&ptr->field_20, 2, 24, 67);
            break;

        default:
            return;
    }

    Vw_CoordHierarchyMatrixCompute(&coord[10], &ptr->field_0);

    gte_SetRotMatrix(&ptr->field_0);
    gte_SetTransMatrix(&ptr->field_0);
    gte_ldv0(&ptr->field_20);
    gte_rt();
    gte_stlvnl(&ptr->field_28);

    switch (weaponAttack)
    {
        case WEAPON_ATTACK(EquippedWeaponId_Handgun, AttackInputType_Tap):
            ptr->field_38 = func_8005E7E0(15);
            ptr->field_50 = 3;

            for (i = 0; i < 3; i++)
            {
                ptr->field_3C[i] = func_8005E7E0(20);
            }
            break;

        case WEAPON_ATTACK(EquippedWeaponId_Shotgun, AttackInputType_Tap):
            ptr->field_38 = func_8005E7E0(16);
            ptr->field_50 = 5;

            for (i = 0; i < 5; i++)
            {
                ptr->field_3C[i] = func_8005E7E0(21);
            }
            break;

        case WEAPON_ATTACK(EquippedWeaponId_HuntingRifle, AttackInputType_Tap):
            ptr->field_38 = func_8005E7E0(17);
            ptr->field_50 = 3;

            for (i = 0; i < 3; i++)
            {
                ptr->field_3C[i] = func_8005E7E0(22);
            }
            break;

        // TODO: What weapon attack?
        case 70:
            ptr->field_38 = func_8005E7E0(18);
            ptr->field_50 = 3;

            for (i = 0; i < 3; i++)
            {
                ptr->field_3C[i] = func_8005E7E0(20);
            }
            break;

        // TODO: What weapon attack?
        case 63:
            ptr->field_38 = func_8005E7E0(19);
            ptr->field_50 = 3;

            for (i = 0; i < 3; i++)
            {
                ptr->field_3C[i] = func_8005E7E0(20);
            }
            break;

        case WEAPON_ATTACK(EquippedWeaponId_RockDrill, AttackInputType_Tap):
        case WEAPON_ATTACK(EquippedWeaponId_Chainsaw,  AttackInputType_Tap):
            ptr->field_50 = 3;

            for (i = 0; i < 3; i++)
            {
                ptr->field_3C[i] = func_8005E7E0(20);
            }
            break;
    }

    if (weaponAttack != WEAPON_ATTACK(EquippedWeaponId_Chainsaw,  AttackInputType_Tap) &&
        weaponAttack != WEAPON_ATTACK(EquippedWeaponId_RockDrill, AttackInputType_Tap))
    {
        if (ptr->field_38 != NO_VALUE)
        {
            // TODO: Demagic this.
            if (weaponAttack == 70)
            {
                g_MapOverlayHdr.unkTable1_4C[ptr->field_38].field_C.s_0.field_0 = Q12_ANGLE(-90.0f);
                g_MapOverlayHdr.unkTable1_4C[ptr->field_38].field_C.s_0.field_2 = Q12_ANGLE(90.0f);
            }
            else
            {
                g_MapOverlayHdr.unkTable1_4C[ptr->field_38].field_C.s_0.field_0 = rotY;
                g_MapOverlayHdr.unkTable1_4C[ptr->field_38].field_C.s_0.field_2 = rotX;
            }

            g_MapOverlayHdr.unkTable1_4C[ptr->field_38].field_B              = Rng_GenerateUInt(0, 1);
            g_MapOverlayHdr.unkTable1_4C[ptr->field_38].field_10.s_0.field_0 = Rng_GenerateUInt(0, 1023);
            g_MapOverlayHdr.unkTable1_4C[ptr->field_38].field_10.s_0.field_2 = Q12_ANGLE(0.0f);
        }
        else
        {
            return;
        }
    }

    for (i = 0; i < ptr->field_50; i++)
    {
        if (ptr->field_3C[i] == NO_VALUE)
        {
            break;
        }

        g_MapOverlayHdr.unkTable1_4C[ptr->field_3C[i]].field_0.vx_0 = Rng_AddGeneratedUInt(Q4_TO_Q8(ptr->field_28.vx), -128, 127);
        g_MapOverlayHdr.unkTable1_4C[ptr->field_3C[i]].vy_8         = Rng_AddGeneratedUInt(Q4_TO_Q8(ptr->field_28.vy), -128, 127);
        g_MapOverlayHdr.unkTable1_4C[ptr->field_3C[i]].field_4.vz_4 = Rng_AddGeneratedUInt(Q4_TO_Q8(ptr->field_28.vz), -128, 127);

        if (i < 3)
        {
            g_MapOverlayHdr.unkTable1_4C[ptr->field_3C[i]].field_B = i;
        }
        else
        {
            g_MapOverlayHdr.unkTable1_4C[ptr->field_3C[i]].field_B = Rng_GenerateInt(0, 2);
        }

        g_MapOverlayHdr.unkTable1_4C[ptr->field_3C[i]].field_C.s_0.field_0  = Rng_GenerateUInt(0, 4095);
        g_MapOverlayHdr.unkTable1_4C[ptr->field_3C[i]].field_C.s_0.field_2  = 0;
        g_MapOverlayHdr.unkTable1_4C[ptr->field_3C[i]].field_10.s_0.field_0 = 0;
    }
}

#if VERSION_IS(EUR)
INCLUDE_ASM("bodyprog/nonmatchings/gfx/bodyprog_effects_8005E0DC", func_80063A50);
#else
bool func_80063A50(POLY_FT4** poly, s32 idx) // 0x80063A50
{
    s_func_80063A50* ptr;

    ptr = PSX_SCRATCH;

    switch (g_MapOverlayHdr.unkTable1_4C[idx].field_A)
    {
        case 15:
            ptr->field_1DC         = 0xCC;
            ptr->field_1E0         = 0x333;
            ptr->field_1E4         = 0xA3;
            ptr->field_1E8         = Q12(1.0f / 16.0f);
            Math_SetSVectorFast(&ptr->field_164, 2, 0xFFF1, 0x47);

            Vw_CoordHierarchyMatrixCompute(&D_800C440C[10], &ptr->field_12C);
            break;

        default:
            return false;

        case 16:
            ptr->field_1DC         = 0xCC;
            ptr->field_1E0         = 0x28F;
            ptr->field_1E4         = 0xCC;
            ptr->field_1E8         = 0x180;
            Math_SetSVectorFast(&ptr->field_164, 3, 0xFFEE, 0x6D);

            Vw_CoordHierarchyMatrixCompute(&D_800C440C[10], &ptr->field_12C);
            break;

        case 17:
            ptr->field_1DC         = 0xA3;
            ptr->field_1E0         = 0x2E1;
            ptr->field_1E4         = 0x51;
            ptr->field_1E8         = 0xC0;
            Math_SetSVectorFast(&ptr->field_164, 0xFFFE, 0xFFDD, 0xDD);

            Vw_CoordHierarchyMatrixCompute(&D_800C440C[10], &ptr->field_12C);
            break;

        case 18:
            ptr->field_1DC         = 0x7A;
            ptr->field_1E0         = 0x333;
            ptr->field_1E4         = 0xA3;
            ptr->field_1E8         = 0x100;
            Math_SetSVectorFast(&ptr->field_164, 3, 0xFFE8, 0x5B);

            Vw_CoordHierarchyMatrixCompute(&D_800C4410[10], &ptr->field_12C);
            break;

        case 19:
            ptr->field_1DC         = 0xCC;
            ptr->field_1E0         = 0x333;
            ptr->field_1E4         = 0xA3;
            ptr->field_1E8         = 0x100;
            Math_SetSVectorFast(&ptr->field_164, 2, 0xFFF3, 0x43);

            Vw_CoordHierarchyMatrixCompute(&D_800C4410[10], &ptr->field_12C);
            break;
    }

    if (g_MapOverlayHdr.unkTable1_4C[idx].field_10.s_0.field_2 == 0)
    {
        func_8003EDA8();
    }
    else if (ptr->field_1DC < g_MapOverlayHdr.unkTable1_4C[idx].field_10.s_0.field_2)
    {
        g_MapOverlayHdr.unkTable1_4C[idx].field_A = 0;
        return false;
    }

    g_MapOverlayHdr.unkTable1_4C[idx].field_10.s_0.field_2 += g_DeltaTime;

    gte_SetRotMatrix(&ptr->field_12C);
    gte_SetTransMatrix(&ptr->field_12C);
    gte_ldv0(&ptr->field_164);
    gte_rt();
    gte_stlvnl(&ptr->field_1AC);

    g_MapOverlayHdr.unkTable1_4C[idx].field_0.vx_0 = Q4_TO_Q8(ptr->field_1AC.vx);
    g_MapOverlayHdr.unkTable1_4C[idx].vy_8 = Q4_TO_Q8((u16)ptr->field_1AC.vy);
    g_MapOverlayHdr.unkTable1_4C[idx].field_4.vz_4 = Q4_TO_Q8(ptr->field_1AC.vz);

    for (ptr->field_1D4 = 0; ptr->field_1D4 < 6; ptr->field_1D4++)
    {
        ptr->field_1D0 = g_MapOverlayHdr.unkTable1_4C[idx].field_10.field_0 + (Q12(ptr->field_1D4) / 6);

        Math_SetSVectorFastSum(&ptr->field_14C[0],
                               ((u16)g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_0.field_0 + Q12_MULT(ptr->field_1E8, Math_Cos(ptr->field_1D0))) - Q12_ANGLE(90.0f),
                               g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_0.field_2 + Q12_MULT(ptr->field_1E8, Math_Sin(ptr->field_1D0)),
                               ptr->field_1D0);
        Math_RotMatrixZxyNegGte(&ptr->field_14C, &ptr->field_12C);

        // `Q8(0.0f)`?
        ptr->field_16C[0].vx = 0;
        ptr->field_16C[0].vy = 0;
        ptr->field_16C[0].vz = 0;

        TransMatrix(&ptr->field_12C, &ptr->field_16C[0]);

        gte_SetRotMatrix(&ptr->field_12C);
        gte_SetTransMatrix(&ptr->field_12C);

        for (ptr->field_1D8 = 0; ptr->field_1D8 < 4; ptr->field_1D8++)
        {
            *(u32*)&ptr->field_14C[0].vx = (ptr->field_1D8 & 1) ? (u16)ptr->field_1E4 :
                                                                 -((u16)ptr->field_1E4) & 0xFFFF;

            ptr->field_14C[0].vz = (ptr->field_1D8 < 2) ? ptr->field_1E0 : 0;

            gte_ldv0(&ptr->field_14C);
            gte_rt();
            gte_stlvnl(&ptr->field_16C[ptr->field_1D8]);
        }

        gte_SetRotMatrix(&ptr->field_0.worldToScreenMat);
        gte_SetTransMatrix(&ptr->field_0.worldToScreenMat);

        for (ptr->field_1D8 = 0; ptr->field_1D8 < 3; ptr->field_1D8++)
        {
            Math_SetSVectorFastSum(&ptr->field_14C[ptr->field_1D8],
                                   Q8_TO_Q4(g_MapOverlayHdr.unkTable1_4C[idx].field_0.vx_0 + ptr->field_16C[ptr->field_1D8].vx) - (u16)ptr->field_0.field_0.vx,
                                   Q8_TO_Q4(g_MapOverlayHdr.unkTable1_4C[idx].vy_8 + ptr->field_16C[ptr->field_1D8].vy) - ptr->field_0.field_0.vy,
                                   Q8_TO_Q4(g_MapOverlayHdr.unkTable1_4C[idx].field_4.vz_4 + ptr->field_16C[ptr->field_1D8].vz) - ptr->field_0.field_0.vz);
        }

        gte_ldv3c(&ptr->field_14C);
        gte_rtpt();
        gte_stsxy3_g3(*poly);
        gte_stsz3c(&ptr->field_1BC);

        Math_SetSVectorFastSum(&ptr->field_14C[0],
                               (Q12_TO_Q8(g_MapOverlayHdr.unkTable1_4C[idx].field_0.vx_0 + ptr->field_19C.vx)) - (u16)ptr->field_0.field_0.vx,
                               (Q12_TO_Q8(g_MapOverlayHdr.unkTable1_4C[idx].vy_8 + ptr->field_19C.vy)) - ptr->field_0.field_0.vy,
                               (Q12_TO_Q8(g_MapOverlayHdr.unkTable1_4C[idx].field_4.vz_4 + ptr->field_19C.vz)) - ptr->field_0.field_0.vz);

        gte_ldv0(&ptr->field_14C);
        gte_rtps();
        gte_stsxy(&ptr->field_1CC);
        gte_stsz(&ptr->field_1C8);

        ptr->field_1BC = (ptr->field_1BC + ptr->field_1C0 + ptr->field_1C4 + ptr->field_1C8) >> 2;

        if (ptr->field_1BC <= 0 || (ptr->field_1BC >> 3) >= ORDERING_TABLE_SIZE ||
            ABS(ptr->field_1CC.vx) > 200 ||
            ABS(ptr->field_1CC.vy) > 160)
        {
            return false;
        }

        *(s32*)&(*poly)->x3 = *(s32*)&ptr->field_1CC;

        if (!(g_SysWork.field_2388.field_154.effectsInfo.flags.field_0 & (SpecialEnvEventFlags_DarkEnvironment | SpecialEnvEventFlags_FlashlightAllowed)))
        {
            *(s32*)&(*poly)->r0 = 0x2E103030;
        }
        else
        {
            *(s32*)&(*poly)->r0 = 0x2E406060;
        }

        *(s32*)&(*poly)->u0 = g_MapOverlayHdr.unkTable1_4C[idx].field_B ? 0xD380FF : 0xD380E0;
        *(s32*)&(*poly)->u1 = (g_MapOverlayHdr.unkTable1_4C[idx].field_B ? 255 : 224) +
                              (g_MapOverlayHdr.unkTable1_4C[idx].field_B ? 0x2B7FE1 : 0x2B801F);
        *(u16*)&(*poly)->u2 = g_MapOverlayHdr.unkTable1_4C[idx].field_B ? 0x9FFF : 0x9FE0;
        *(u16*)&(*poly)->u3 = (g_MapOverlayHdr.unkTable1_4C[idx].field_B ? 255 : 224) -
                              (g_MapOverlayHdr.unkTable1_4C[idx].field_B ? 0x611F : 0x60E1);

        addPrimFast(&g_OrderingTable0[g_ActiveBufferIdx].org[ptr->field_1BC >> 3], (*poly), 9);
        *poly += 1;
    }

    return true;
}
#endif

#if VERSION_IS(EUR)
INCLUDE_ASM("bodyprog/nonmatchings/gfx/bodyprog_effects_8005E0DC", func_80064334);
#else
bool func_80064334(POLY_FT4** poly, s32 idx) // 0x80064334
{
    s32              temp_s0;
    s32              temp_s4;
    q19_12           temp_v1_5;
    s_func_80064334* ptr;
    POLY_FT4*        next;

    ptr = PSX_SCRATCH;

    temp_s4 = Q12_MULT_PRECISE(g_DeltaTime, Rng_GenerateInt(Q12(0.8f), Q12(1.2f) - 2));

    if (g_MapOverlayHdr.unkTable1_4C[idx].field_A == 20)
    {
        ptr->field_164 = Q12(0.05f);
        ptr->field_168 = Q12(0.4f);
    }
    else if (g_MapOverlayHdr.unkTable1_4C[idx].field_A == 21)
    {
        ptr->field_164 = Q12(0.05f);
        ptr->field_168 = Q12(0.4f);
    }
    else
    {
        ptr->field_164 = Q12(0.04f);
        ptr->field_168 = Q12(0.35f);
    }

    ptr->field_15A = g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_0.field_2;

    g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_0.field_2 += temp_s4 + ((Math_Vector2MagCalcSafeQ6(*g_MapOverlayHdr.particleWindSpeedX, *g_MapOverlayHdr.particleWindSpeedZ) * temp_s4) >> 10);

    if (ptr->field_168 < (u16)g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_0.field_2)
    {
        g_MapOverlayHdr.unkTable1_4C[idx].field_A = 0;
    }

    ptr->field_158                                              = g_MapOverlayHdr.unkTable1_4C[idx].field_10.field_0;
    g_MapOverlayHdr.unkTable1_4C[idx].field_10.s_0.field_0 += temp_s4;

    Math_SetSVectorFastSum(&ptr->field_138,
                           Q12_TO_Q8(g_MapOverlayHdr.unkTable1_4C[idx].field_0.vx_0) - (u16)ptr->field_0.field_0.vx,
                           Q12_TO_Q8((g_MapOverlayHdr.unkTable1_4C[idx].vy_8)) - ptr->field_0.field_0.vy,
                           Q12_TO_Q8(g_MapOverlayHdr.unkTable1_4C[idx].field_4.vz_4) - ptr->field_0.field_0.vz);

    gte_ldv0(&ptr->field_138);
    gte_rtps();
    gte_stsxy(&ptr->field_154);
    gte_stsz(&ptr->field_150);

    if (ptr->field_150 <= 0 || (ptr->field_150 >> 3) >= ORDERING_TABLE_SIZE ||
        ABS(ptr->field_154.vx) > 200 ||
        ABS(ptr->field_154.vy) > 160)
    {
        return false;
    }

    ptr->field_160 = (((ptr->field_158 >> 6) + 16) * ptr->field_0.field_2C) / ptr->field_150;

    setPolyFT4(*poly);

    setXY0Fast(*poly, (u16)ptr->field_154.vx - (u16)ptr->field_160, ptr->field_154.vy - ptr->field_160);
    setXY1Fast(*poly, (u16)ptr->field_154.vx + (u16)ptr->field_160, ptr->field_154.vy - ptr->field_160);
    setXY2Fast(*poly, (u16)ptr->field_154.vx - (u16)ptr->field_160, ptr->field_154.vy + ptr->field_160);
    setXY3Fast(*poly, (u16)ptr->field_154.vx + (u16)ptr->field_160, ptr->field_154.vy + ptr->field_160);

    *(s32*)&(*poly)->u0 = (((g_MapOverlayHdr.unkTable1_4C[idx].field_B << 5) + 160) << 8) + 0x011300E0;
    *(s32*)&(*poly)->u1 = (((g_MapOverlayHdr.unkTable1_4C[idx].field_B << 5) + 160) << 8) + 0x2B00FF;
    *(u16*)&(*poly)->u2 = (((g_MapOverlayHdr.unkTable1_4C[idx].field_B << 5) + 191) << 8) + 0xE0;
    *(u16*)&(*poly)->u3 = (((g_MapOverlayHdr.unkTable1_4C[idx].field_B << 5) + 191) << 8) + 0xFF;

    setSemiTrans(*poly, true);

    if (ptr->field_15A < (ptr->field_168 >> 3))
    {
        ptr->field_15C = MIN(((ptr->field_15A << 9) / ptr->field_168) + 48, 128);
    }
    else
    {
        ptr->field_15C = CLAMP_LOW(120 - (ptr->field_15A * 120) / ptr->field_168, 0);
    }

    if (!(g_SysWork.field_2388.field_154.effectsInfo.flags.field_0 & (SpecialEnvEventFlags_DarkEnvironment | SpecialEnvEventFlags_FlashlightAllowed)))
    {
        func_80055A90(&ptr->field_134, &ptr->field_130, ptr->field_15C, ptr->field_150 * 16);

        next  = *poly + 1;
        *next = **poly;

        (*poly + 1)->clut = 0x93;

        *(u16*)&(*poly)->r0 = ptr->field_130.r + (ptr->field_130.g << 8);
        (*poly)->b0         = ptr->field_130.b;

        *(u16*)&(*poly + 1)->r0 = ptr->field_134.r + (ptr->field_134.g << 8);
        (*poly + 1)->b0         = ptr->field_134.b;

        ptr->field_12C = (PACKET*)(*poly) + 0x50;

        SetPriority(ptr->field_12C, 0, 0);
        SetPriority(ptr->field_12C + 12, 1, 1);

        addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[ptr->field_150 >> 3], *poly);
        addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[ptr->field_150 >> 3], ptr->field_12C);
        addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[ptr->field_150 >> 3], *poly + 1);
        addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[ptr->field_150 >> 3], ptr->field_12C + 12);

        *poly = ptr->field_12C + 24;
    }
    else
    {
        func_80055E90(&ptr->field_130, ptr->field_15C);
        *(u16*)&(*poly)->r0 = ptr->field_130.r + (ptr->field_130.g << 8);
        (*poly)->b0         = ptr->field_130.b;

        addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[ptr->field_150 >> 3], *poly);

        *poly = *poly + 1;
    }

    if (ptr->field_164 < ptr->field_158)
    {
        temp_v1_5 = Math_Sin(g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_0.field_0);
        temp_s0   = Q12_MULT_PRECISE(temp_s4, Q12(0.1f));

        g_MapOverlayHdr.unkTable1_4C[idx].field_0.vx_0 += Q12_MULT(temp_s0, temp_v1_5);
        g_MapOverlayHdr.unkTable1_4C[idx].field_4.vz_4 += Q12_MULT(temp_s0, Math_Cos(g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_0.field_0));

        if (*g_MapOverlayHdr.data_190 != 0)
        {
            g_MapOverlayHdr.unkTable1_4C[idx].field_0.vx_0 += TIMESTEP_SCALE_30_FPS(temp_s4, *g_MapOverlayHdr.particleWindSpeedX) >> 2;
            g_MapOverlayHdr.unkTable1_4C[idx].field_4.vz_4 += TIMESTEP_SCALE_30_FPS(temp_s4, *g_MapOverlayHdr.particleWindSpeedZ) >> 2;
        }
        else if (*g_MapOverlayHdr.data_18C != 0)
        {
            g_MapOverlayHdr.unkTable1_4C[idx].field_0.vx_0 += TIMESTEP_SCALE_30_FPS(temp_s4, *g_MapOverlayHdr.particleWindSpeedX) >> 1;
            g_MapOverlayHdr.unkTable1_4C[idx].field_4.vz_4 += TIMESTEP_SCALE_30_FPS(temp_s4, *g_MapOverlayHdr.particleWindSpeedZ) >> 1;
        }
        else
        {
            g_MapOverlayHdr.unkTable1_4C[idx].field_0.vx_0 += (TIMESTEP_SCALE_30_FPS(temp_s4, *g_MapOverlayHdr.particleWindSpeedX) * 2) / 3;
            g_MapOverlayHdr.unkTable1_4C[idx].field_4.vz_4 += (TIMESTEP_SCALE_30_FPS(temp_s4, *g_MapOverlayHdr.particleWindSpeedZ) * 2) / 3;
        }
    }

    return true;
}
#endif

void func_80064F04(VECTOR3* arg0, s8 arg1, q3_12 angle) // 0x80064F04
{
    s32 idx;

    idx = func_8005E7E0(31);
    if (idx != NO_VALUE)
    {
        g_MapOverlayHdr.unkTable1_4C[idx].field_0.vx_0        = Q12_TO_Q8(arg0->vx);
        g_MapOverlayHdr.unkTable1_4C[idx].vy_8                = Q12_TO_Q8(arg0->vy);
        g_MapOverlayHdr.unkTable1_4C[idx].field_4.vz_4        = Q12_TO_Q8(arg0->vz);
        g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_1.field_0 = arg1;
        g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_0.field_2 = angle;
    }
}

bool func_80064FC0(POLY_FT4** polys, s32 idx) // 0x80064FC0
{
    s_func_80064FC0* ptr;
    s16              temp;

    ptr = PSX_SCRATCH;

    g_MapOverlayHdr.unkTable1_4C[idx].field_A = 0;
    temp                                         = g_MapOverlayHdr.unkTable1_4C[idx].field_0.vx_0 - ptr->field_0.field_0.vx;
    Math_SetSVectorFastSum(&ptr->field_12C,
                           temp,
                           g_MapOverlayHdr.unkTable1_4C[idx].vy_8 - ptr->field_0.field_0.vy,
                           g_MapOverlayHdr.unkTable1_4C[idx].field_4.vz_4 - ptr->field_0.field_0.vz);

    gte_ldv0(&ptr->field_12C);
    gte_rtps();
    gte_stsxy(&ptr->field_13C);
    gte_stsz(&ptr->field_140);

    if (ABS(ptr->field_13C.vx) > 200 || ABS(ptr->field_13C.vy) > 160)
    {
        return false;
    }

    ptr->field_144 = ((g_MapOverlayHdr.unkTable1_4C[idx].field_C.s_0.field_2 * ptr->field_0.field_2C) / ptr->field_140) >> 4;
    setPolyFT4(*polys);
    setXY0Fast(*polys, (u16)ptr->field_13C.vx - (u16)ptr->field_144, ptr->field_13C.vy + ptr->field_144);
    setXY1Fast(*polys, (u16)ptr->field_13C.vx + (u16)ptr->field_144, ptr->field_13C.vy + ptr->field_144);
    setXY2Fast(*polys, (u16)ptr->field_13C.vx - (u16)ptr->field_144, ptr->field_13C.vy - ptr->field_144);
    setXY3Fast(*polys, (u16)ptr->field_13C.vx + (u16)ptr->field_144, ptr->field_13C.vy - ptr->field_144);
    *(u16*)&(*polys)->r0 = 0x1020;
    (*polys)->b0         = 16;

    setSemiTrans((*polys), true);

#if VERSION_REGION_IS(PAL)
    *(u32*)&(*polys)->u0 = 0x7DBB0000;
#else
    *(u32*)&(*polys)->u0 = 0x018C0000;
#endif
    *(u32*)&(*polys)->u1 = 0x2C003F;
    *(u16*)&(*polys)->u2 = 0x3F00;
    *(u16*)&(*polys)->u3 = 0x3F3F;

    addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[(ptr->field_140 - 32) >> 3], (*polys));

    *polys += 1;
    return true;
}
