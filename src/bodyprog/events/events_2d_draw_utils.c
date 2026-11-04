#include "game.h"
#include "inline_no_dmpsx.h"

#include <psyq/libpad.h>
#include <psyq/strings.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/item_screens.h"
#include "bodyprog/math/math.h"
#include "bodyprog/screen/data.h"
#include "bodyprog/screen/draw.h"
#include "bodyprog/sound/sound_system.h"
#include "main/fsqueue.h"

// ========================================
// ADDITIONAL 2D GFX
// ========================================

void Gfx_CursorDraw(s32 x0, s16 y0, s32 x1, s16 y1, s16 u, s16 v, s16 width, s32 height, s32 tint,
                    u32 clutX, s16 clutY, s32 tPage) // 0x800881B8
{
    POLY_FT4* poly;

    // Get polygon.
    poly = (POLY_FT4*)GsOUT_PACKET_P;
    setPolyFT4(poly);

    // Set vertices.
    setXY0Fast(poly, x0 - x1, y0 - y1);
    setXY1Fast(poly, x0 + x1, y0 - y1);
    setXY2Fast(poly, x0 - x1, y0 + y1);
    setXY3Fast(poly, x0 + x1, y0 + y1);

    // Set UVs. TODO: Use macros here?
    *(u32*)(&poly->u0) = u + (v << 8) + (getClut(clutX, clutY) << 16);
    *(u32*)(&poly->u1) = (u + width) + (v << 8) + (getTPage(0, 0, tPage << 6, (((tPage >> 4) & 0x1) << 8)) << 16);
    *(u16*)(&poly->u2) = u + ((v + height) << 8);
    *(u16*)(&poly->u3) = (u + width) + ((v + height) << 8);

    // Set color.
    setRGB0Fast(poly, tint, tint, tint);
    setSemiTrans(poly, false);

    // Submit polygon.
    addPrim(g_OrderingTable0[g_ActiveBufferIdx].org, poly);
    poly++;
    GsOUT_PACKET_P = (PACKET*)poly;
}

void PaperMap_ExpandingBoxesDraw(q3_12 progressAlpha,
                                 q3_12 startX, q3_12 startY, q3_12 startWidth, q3_12 startHeight,
                                 q3_12 endX, q3_12 endY, q3_12 endWidth, q3_12 endHeight) // 0x80088370
{
    #define BOX_COUNT 5

    q19_12   lerpWeight;
    q3_12    interpStep;
    q4_12    cornersX[2];
    q3_12    cornersY[2];
    s32      i;
    LINE_F3* line;
    s32      blueIntensity;
    q3_12    tempX;
    q19_12   invLerpWeight;

    // Get line.
    line = (LINE_F3*)GsOUT_PACKET_P;

    // Draw boxes.
    for (i = 0; i < BOX_COUNT; i++)
    {
        if (progressAlpha > Q12(0.0f))
        {
            interpStep = CLAMP_LOW_THEN_MIN(Q12_DIV(progressAlpha - ((i * Q12(0.5f)) / 5), Q12(0.5f)),
                                            Q12(0.0f),
                                            Q12(1.0f));
        }
        else
        {
            interpStep = -progressAlpha;
        }

        lerpWeight    = interpStep;
        invLerpWeight = Q12(1.0f) - lerpWeight;

        cornersX[0] = Q12_MULT_PRECISE(startX, invLerpWeight) + Q12_MULT_PRECISE(endX, lerpWeight);
        cornersY[0] = Q12_MULT_PRECISE(startY, invLerpWeight) + Q12_MULT_PRECISE(endY, lerpWeight);

        tempX       = cornersX[0] + Q12_MULT_PRECISE(startWidth, invLerpWeight);
        cornersX[1] = Q12_MULT_PRECISE(endWidth, lerpWeight) + tempX;

        tempX       = cornersY[0] + Q12_MULT_PRECISE(startHeight, invLerpWeight);
        cornersY[1] = Q12_MULT_PRECISE(endHeight, lerpWeight) + tempX;

        // Set vertices for first line.
        setLineF3(line);
        setXY0Fast(line, cornersX[0], cornersY[0]);
        setXY1Fast(line, cornersX[1], cornersY[0]);
        setXY2Fast(line, cornersX[1], cornersY[1]);
        setSemiTrans(line, 0);

        // Set color.
        blueIntensity      = 128 - ((i << 6) / 5);
        setRGB0Fast(line, 0, 0, blueIntensity);

        // Draw first line.
        addPrim(g_OrderingTable0[g_ActiveBufferIdx].org, line);

        line[1] = line[0];
        line[2] = line[1];
        line[3] = line[2];
        line++;

        // Draw second line.
        setXY0Fast(line, cornersX[0], cornersY[0] - 1);
        setXY1Fast(line, cornersX[1], cornersY[0] - 1);
        setXY2Fast(line, cornersX[1], cornersY[1] + 1);
        addPrim(g_OrderingTable0[g_ActiveBufferIdx].org, line);
        line++;

        // Draw third line.
        setXY0Fast(line, cornersX[1], cornersY[1]);
        setXY1Fast(line, cornersX[0], cornersY[1]);
        setXY2Fast(line, cornersX[0], cornersY[0]);
        addPrim(g_OrderingTable0[g_ActiveBufferIdx].org, line);
        line++;

        // Draw fourth line.
        setXY0Fast(line, cornersX[1], cornersY[1] + 1);
        setXY1Fast(line, cornersX[0], cornersY[1] + 1);
        setXY2Fast(line, cornersX[0], cornersY[0] - 1);
        addPrim(g_OrderingTable0[g_ActiveBufferIdx].org, line);
        line++;
    }

    GsOUT_PACKET_P = line;

    #undef BOX_COUNT
}
