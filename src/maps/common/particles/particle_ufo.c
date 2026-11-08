#include "inline_no_dmpsx.h"

#include <psyq/gtemac.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "main/rng.h"
#include "maps/common/shared.h"

// Unknown drawing code included in M1S03, M4S05, M5S01, M6S01 and M6S02.
// Called by cutscene event code? Might be for a full-screen light flash effect.
// TODO: Make this separate split in each map instead of `#include`.

void sharedFunc_800CE5D4_1_s03(VECTOR3* arg0, u16 arg1, u16 arg2, u32 arg3)
{
    s32 idx;

    idx = func_8005E7E0(35);
    if (idx != NO_VALUE)
    {
        sharedData_800DFB7C_0_s00[idx].field_0.vx_0        = Q12_TO_Q8(arg0->vx);
        sharedData_800DFB7C_0_s00[idx].vy_8            = Q12_TO_Q8(arg0->vy);
        sharedData_800DFB7C_0_s00[idx].field_4.vz_4        = Q12_TO_Q8(arg0->vz);
        sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_0 = arg1 / 2;
        sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_2 = Q12_TO_Q8(arg2);
        sharedData_800DFB7C_0_s00[idx].field_B         = arg3;
    }
}

bool sharedFunc_800CE688_1_s03(POLY_FT4** poly, s32 idx)
{
    // TODO: Is there a way to make `Rng_GenerateInt` fit instead?
    #define Rng_SymmetricOffset(rand, range) \
        ((s32)((rand) % (range)) - ((range) >> 1))

    // TODO: `s_func_80060044` is a close match to this struct (based on `func_80060044` `gte_ldv0/gte_stsxy/gte_stsz` usage)
    // but that func flipped `field_140/field_144`? (maybe coder mistaking fields for `gte_stsxy/gte_stsz`?)
    typedef struct
    {
        VECTOR3   field_0;
        u8        unk_C[32];
        s32       field_2C;
        u8        unk_30[252];
        SPRT*     sprt_12C;
        DR_TPAGE* tpage_130;
        DR_STP*   stp_134;
        SVECTOR   field_138;
        s16       x_140;
        s16       y_142;
        s32       z_144;
        union
        {
            u16 u16;
            u8  u8[2]; // `activeBufferIdx` at [1].
        } field_148;
    } s_ScratchData_func_800CE688;

    s_ScratchData_func_800CE688* scratchData;
    s32                          polyV;
    s32                          i;

    scratchData = PSX_SCRATCH_ADDR(0);

    sharedData_800DFB7C_0_s00[idx].field_A = 0;
    if (sharedData_800DFB7C_0_s00[idx].field_B & 1)
    {
        scratchData->field_148.u8[1] = g_ActiveBufferIdx;
        scratchData->sprt_12C        = (SPRT*)*poly;

        for (i = 0; i < 2; i++)
        {
            setCodeWord(scratchData->sprt_12C, PRIM_RECT | RECT_BLEND | RECT_TEXTURE, COLOR_RGBC(128, 128, 128, 0));
            setXY0Fast(scratchData->sprt_12C, ((i << 8) - 160), -112);
            scratchData->sprt_12C->u0 = 0;
            scratchData->sprt_12C->v0 = (scratchData->field_148.u8[1] == 0) ? 32 : 0;
            setWH(scratchData->sprt_12C, (i == 0) ? 256 : 64, FRAMEBUFFER_HEIGHT_PROGRESSIVE);
            addPrimFast(&g_OrderingTable2[g_ActiveBufferIdx].org[15], scratchData->sprt_12C, 4);

            scratchData->sprt_12C++;
            scratchData->tpage_130 = (DR_TPAGE*)scratchData->sprt_12C;
            setDrawTPage(scratchData->tpage_130, 0, 0, getTPage(2, 0, ((scratchData->field_148.u8[1] << 4) + (i << 2)) << 6, (((scratchData->field_148.u8[1] << 4) >> 4) & 1) << 8));

            AddPrim(&g_OrderingTable2[g_ActiveBufferIdx].org[15], scratchData->tpage_130);
            scratchData->tpage_130++;
            scratchData->sprt_12C = (SPRT*)scratchData->tpage_130;
        }

        scratchData->stp_134 = (DR_STP*)scratchData->sprt_12C;
        SetDrawStp(scratchData->stp_134, 1);
        addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[ORDERING_TABLE_SIZE - 1], scratchData->stp_134);
        scratchData->stp_134++;
        SetDrawStp(scratchData->stp_134, 0);
        addPrim(&g_OrderingTable2[g_ActiveBufferIdx].org[0], scratchData->stp_134);
        scratchData->stp_134++;
        *poly = (PACKET*)scratchData->stp_134;
    }

    for (i = 0; i < 4; i++)
    {
        Math_SetSVectorFastSum(&scratchData->field_138,
                               (u16)sharedData_800DFB7C_0_s00[idx].field_0.vx_0 - (u16)scratchData->field_0.vx,
                               sharedData_800DFB7C_0_s00[idx].vy_8 - scratchData->field_0.vy,
                               sharedData_800DFB7C_0_s00[idx].field_4.vz_4 - scratchData->field_0.vz);

        if (i != 0)
        {

            scratchData->field_138.vx += Rng_SymmetricOffset(Rng_Rand16(), (u16)sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_2);
            scratchData->field_138.vy += Rng_SymmetricOffset(Rng_Rand16(), (u16)sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_2);
            scratchData->field_138.vz += Rng_SymmetricOffset(Rng_Rand16(), (u16)sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_2);
            polyV                      = (Rng_GenerateUInt(0, 1) == 0) << 6;
        }
        else
        {
            polyV = 0;
        }

        gte_ldv0(&scratchData->field_138);
        gte_rtps();
        gte_stsxy(&scratchData->x_140);
        gte_stsz(&scratchData->z_144);

        if (scratchData->z_144 <= 0 || (scratchData->z_144 >> 3) >= 0x800 ||
            ABS(scratchData->x_140) > 200 ||
            ABS(scratchData->y_142) > 160)
        {
            return false;
        }
        else
        {
            scratchData->field_148.u16 = (((u16)sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_0 * scratchData->field_2C) / scratchData->z_144) >> 4;

            setPolyFT4(*poly);

            setXY0Fast(*poly, (u16)scratchData->x_140 - scratchData->field_148.u16, scratchData->y_142 - (s16)scratchData->field_148.u16);
            setXY1Fast(*poly, (u16)scratchData->x_140 + scratchData->field_148.u16, scratchData->y_142 - (s16)scratchData->field_148.u16);
            setXY2Fast(*poly, (u16)scratchData->x_140 - scratchData->field_148.u16, scratchData->y_142 + (s16)scratchData->field_148.u16);
            setXY3Fast(*poly, (u16)scratchData->x_140 + scratchData->field_148.u16, scratchData->y_142 + (s16)scratchData->field_148.u16);

            setRGB0Fast(*poly, 0x40, 0x40, 0x40);

            setSemiTrans(*poly, true);

            setUV0AndClutSum(*poly, 128, polyV, getClut(768, 128));
            setUV1AndTPageSum(*poly, 255, polyV, 44);
            setUV2Sum(*poly, 128, polyV + 63);
            setUV3Sum(*poly, 255, polyV + 63);

            addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[scratchData->z_144 >> 3], *poly);
            (*poly)++;
        }
    }

    return true;
}
