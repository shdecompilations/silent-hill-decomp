#include "bodyprog/game.h"

#include "bodyprog/bodyprog.h"
#include "bodyprog/gfx/brightness_menu_lines.h"
#include "bodyprog/screen/data.h"
#include "bodyprog/math/math.h"

void Options_BrightnessMenu_LinesDraw(s32 brightness) // 0x8003E5E8
{
    #define LINE_COUNT 20

    s32       i;
    u8        color;
    GsOT_TAG* ot;
    PACKET*   packet;
    LINE_G2*  line;

    packet = GsOUT_PACKET_P;
    ot     = &g_OrderingTable0[g_ActiveBufferIdx].org[1];

    // Draw vertical lines.
    for (i = -(LINE_COUNT / 2); i <= (LINE_COUNT / 2); i++)
    {
        // Get line primitive.
        line = (LINE_G2*)packet;
        setLineG2(line);

        // Compute start and end points.
        line->x1 = ((g_GameWork.gsScreenWidth - 64) / 20) * i;
        line->x0 = line->x1;
        line->y0 = -16;
        line->y1 = (g_GameWork.gsScreenHeight / 2) - 45;

        // Compute color.
        color    = (brightness * 8) + 4;
        line->b1 = color;
        line->g1 = color;
        line->r1 = color;
        line->b0 = color;
        line->g0 = color;
        line->r0 = color;

        // Add line primitive.
        AddPrim(ot, line);
        packet += sizeof(LINE_G2);
    }

    GsOUT_PACKET_P = packet;

    #undef LINE_COUNT
}
