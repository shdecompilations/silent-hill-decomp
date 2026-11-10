#include "game.h"

#include "bodyprog/bodyprog.h"
#include "bodyprog/text/text_draw.h"
#include "bodyprog/items/item_screens.h"
#include "bodyprog/math/math.h"
#include "bodyprog/items/ranking.h"
#include "main/fsqueue.h"
#include "screens/stf_roll/credits.h"

s32 D_800C48F0;

s_CreditTextState g_CreditTextState = {
    .lineHeight = 24,
    .color      = COLOR_RGBC(255, 255, 255, PRIM_RECT | RECT_TEXTURE),
    .widthTable = D_801E5C24,
    .colorTable = g_CreditsColorTable
};

s_CreditText3dState g_CreditText3dState = {
    .sub_0 = {
        .lineHeight = 24,
        .color      = COLOR_RGBC(255, 255, 255, RECT_TEXTURE | RECT_SIZE_1 | PRIM_POLY),
        .widthTable = D_801E5C24,
        .colorTable = g_CreditsColorTable3d
    },
    .field_1C = Q12(1.0f),
    .field_24 = 240,
    .field_30 = 0xF0000,
    .field_34 = Q12(1.0f),
    .field_3C = Q12(-1.0f),
    .field_48 = Q12(1.0f),
    .field_4C = Q12(1.1111f)
};

void GameFs_StfRollBinLoad() // 0x80091778
{
    // @hack Required for match.
    if (g_Font24AtlasImg.tPage != 0) {}

    Fs_QueueStartRead(FILE_VIN_STF_ROLL_BIN, FS_BUFFER_1);
    func_800917C0(FONT24_BUFFER, 1, &g_Font24AtlasImg);
}

void func_800917C0(void* arg0, s32 arg1, s_FsImageDesc* img) // 0x800917C0
{
    u32 tPage;

    if (arg1 != 0)
    {
        arg0 -= ALIGN(Fs_GetFileSize(FILE_TIM_FONT24_TIM), 0x800);
    }

    Fs_QueueStartReadTim(FILE_TIM_FONT24_TIM, arg0, img);

    g_CreditTextState.uv = img->tPage[1]; // `Fs_QueuePostLoadTim` does something similar.
    tPage                = img->tPage[1];

    g_CreditTextState.tpage  = tPage & 0x10;
    g_CreditTextState.tpage |= tPage & 0xF;
    g_CreditTextState.clut   = (img->clutY << 6) | ((img->clutX >> 4) & 0x3F);
}

void func_8009185C(s16 arg0, s16 arg1, s32 arg2, q19_12 arg3, q19_12 arg4, q19_12 arg5, q19_12 arg6, q19_12 arg7, s32 arg8, s32 arg9) // 0x8009185C
{
    s32    temp_lo;
    s32    temp_lo_2;
    q19_12 temp_lo_4;
    q19_12 temp_lo_7;
    s32    temp_t1;
    q19_12 temp_t2;
    q19_12 temp_v0;
    u16    temp_t0 = 24;

    temp_t1   = arg2 << 22;
    temp_lo   = temp_t1 / (arg6 >> 2);
    temp_lo_2 = arg9 * temp_t0;
    temp_lo_4 = (arg6 + (s32)Q12_MULT_PRECISE(arg3, temp_lo_2)) >> 2;
    temp_lo_4 = temp_t1 / temp_lo_4;

    g_CreditText3dState.sub_0.textX      = 0;
    g_CreditText3dState.sub_0.textY      = 0;
    g_CreditText3dState.sub_0.leftMargin = 0;
    g_CreditText3dState.sub_0.lineHeight = 24;
    g_CreditText3dState.sub_0.semiTrans  = 0;
    g_CreditText3dState.sub_0.color      = COLOR_RGBC(255, 255, 255, RECT_TEXTURE | RECT_SIZE_1 | PRIM_POLY);
    g_CreditText3dState.sub_0.uv         = g_CreditTextState.uv;
    g_CreditText3dState.sub_0.clut       = g_CreditTextState.clut;
    g_CreditText3dState.sub_0.tpage      = g_CreditTextState.tpage;
    g_CreditText3dState.field_1C         = arg3;
    g_CreditText3dState.field_20         = arg0;
    g_CreditText3dState.field_22         = arg1;
    g_CreditText3dState.field_24         = arg2;
    g_CreditText3dState.field_28         = arg4;
    g_CreditText3dState.field_2C         = arg5;
    g_CreditText3dState.field_30         = arg6;
    g_CreditText3dState.field_34         = arg7;
    g_CreditText3dState.field_38         = arg8;
    g_CreditText3dState.field_3C         = arg9;
    g_CreditText3dState.field_40         = Q12_MULT_PRECISE(temp_lo, arg4);
    g_CreditText3dState.field_44         = Q12_MULT_PRECISE(temp_lo_4, arg4);

    temp_v0                      = arg4 + Q12_MULT_PRECISE(arg3, arg7);
    g_CreditText3dState.field_48 = Q12_MULT_PRECISE(temp_lo, temp_v0) - g_CreditText3dState.field_40;
    g_CreditText3dState.field_4C = Q12_MULT_PRECISE(temp_lo_4, temp_v0) - g_CreditText3dState.field_44;
    g_CreditText3dState.field_50 = Q12_MULT_PRECISE(temp_lo, arg5);

    temp_lo_7                    = arg8 * temp_t0;
    temp_t2                      = arg5 + Q12_MULT_PRECISE(arg3, temp_lo_7);
    g_CreditText3dState.field_54 = Q12_MULT_PRECISE(temp_lo_4, temp_t2);
}
