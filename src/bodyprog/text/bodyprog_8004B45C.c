#include "bodyprog/game.h"

#include <psyq/libetc.h>
#include <psyq/libpad.h>
#include <psyq/strings.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "bodyprog/screen/data.h"
#include "bodyprog/screen/draw.h"
#include "bodyprog/sound/sound_system.h"
#include "bodyprog/text/map_msg.h"
#include "bodyprog/text/text_draw.h"
#include "main/fsqueue.h"

/** @note This code is exclusively used in the Japanese version.
 * Something related to size and initalization of a pallete table
 * for japanese words.
 */

void func_80036E48(u16* arg0, s16* arg1) // 0x80036E48
{
    u16  sp10[4];
    u8   sp18[16];
    u8   sp28[12];
    s32  temp_a0;
    s32  temp_v0_2;
    s32  var_a2;
    s32  i;
    u8   var_t4;
    s32  var_v0;
    u16  temp_v0;
    u16  var_a3;
    s16* var_t2;
    u16* var_t7;

    var_t2 = arg1;
    var_t4 = 0;
    var_t7 = arg0;

    for (i = 0; i < 15;)
    {
        temp_v0 = *var_t7;

        for (var_a2 = 0; var_a2 < 15; var_a2++)
        {
            if ((((temp_v0 >> 8) | ((temp_v0 & 0xFF) << 8)) >> (0xF - var_a2)) & 1)
            {
                sp18[var_a2] = 1;
            }
            else
            {
                sp18[var_a2] = 0;
            }
        }

        sp18[15] = 0;
        sp28[0]  = sp18[0];
        sp28[1]  = sp18[1] | sp18[2];
        sp28[2]  = sp18[3];
        sp28[3]  = sp18[4];
        sp28[4]  = sp18[5] | sp18[6];
        sp28[5]  = sp18[7];
        sp28[6]  = sp18[8] | sp18[9];
        sp28[7]  = sp18[10];
        sp28[8]  = sp18[11];
        sp28[9]  = sp18[12] | sp18[13];
        var_v0   = 2;
        sp28[10] = sp18[14];
        sp28[11] = 0;

        for (var_a3 = 0, var_a2 = 0; var_a2 < 12; var_a2++)
        {
            var_v0  = 2;
            temp_a0 = (var_a2 & 0x3) * 4;

            if (sp28[var_a2] != 0)
            {
                var_a3 |= var_v0 << temp_a0;
            }
            else
            {
                if (var_a2 > 0 && sp28[var_a2 - 1] != 0)
                {
                    var_a3 |= 11 << temp_a0;
                }

                if (i > 0)
                {
                    if (var_a2 != 0 && ((sp10[var_a2 >> 2] >> temp_a0) & 0xF) == 11 && var_t4)
                    {
                        var_a3 |= 11 << temp_a0;
                    }

                    temp_v0_2 = (sp10[var_a2 >> 2] >> temp_a0) & 0xF;

                    if (temp_v0_2 > 0 && temp_v0_2 != 11)
                    {
                        var_a3 |= 11 << temp_a0;
                        var_t4 = 1;
                    }
                    else
                    {
                        var_t4 = 0;
                    }
                }
            }

            if ((var_a2 & 3) == 3)
            {
                sp10[var_a2 >> 2] = var_a3;
                *var_t2           = var_a3;
                var_t2++;
                var_a3 = 0;
            }
        }

        do { i++; } while (0); // @hack Required for match.

        var_t7++;
        var_t2 += (D_800C3920 - 1) * 3;
    }

    func_8003708C(var_t2, sp10);
}

void func_8003708C(s16* ptr0, u16* ptr1) // 0x8003708C
{
    s32 var0;
    s16 var1;
    s32 var3;
    s32 shift;
    s32 i;

    var0 = 0;
    var1 = 0;

    for (i = 0; i < 12; i++)
    {
        shift = (i & 0x3) * 4;
        var3  = (*ptr1 >> shift) & 0xF;
        if (i != 0 && var3 == 11 && var0 != 0)
        {
            var1 |= 11 << shift;
        }

        var0 = 0;
        if (var3 != 0 && var3 != 11)
        {
            var1 |= 11 << shift;
            var0  = 1;
        }

        if ((i & 0x3) == 3 || i == 12)
        {
            ptr1++;
            *ptr0++ = var1;
            var1    = 0;
        }
    }
}

void func_80037124(void) // 0x80037124
{
    g_MapMsg_Select.maxIdx = NO_VALUE;
    func_8003652C();
    DrawSync(SyncMode_Wait);
}
