#include "game.h"

#include "bodyprog/bodyprog.h"
#include "bodyprog/screen/screen_data.h"
#include "bodyprog/screen/screen_draw.h"
#include "bodyprog/text/text_draw.h"
#include "bodyprog/math/math.h"

#ifndef PAD_HACK_IGNORE
    const s32 __pad_rodata_80025D68 = 0;
    s8 __pad_bss_800C38B2[2];
    s32 __pad_bss_800C38B8[4];
    s16 __pad_bss_800C391E;
    s32 __pad_bss_800C3924;
#endif

// ========================================
// STATIC VARIABLES
// ========================================

/** @brief Glyph widths of the 12x16 font. Used for kerning. */
static const u8 FONT_12X16_GLYPH_WIDTHS[FONT_12X16_GLYPH_COUNT] = {
    3,  7,  7,  11, 11, 4,  10, 4,  6,  10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 4,  4,
    10, 11, 10, 8,  13, 12, 12, 12, 13, 11, 11, 13, 12, 9,  9,  12, 12, 13, 12, 13, 11,
    13, 12, 10, 11, 13, 12, 12, 12, 11, 12, 6,  4,  6,  8,  0,  3,  9,  10,  9, 9,  9,
    7,  11, 11, 6,  6,  10, 6,  13, 11, 10, 11, 10, 8,  8,  7,  10, 10, 12, 10, 10, 9
};

/** @brief See `e_StringColorId`. */
static const u32 STRING_COLORS[StringColorId_Count] = {
    COLOR_RGBC(160, 128, 64,  PRIM_RECT | RECT_TEXTURE),
    COLOR_RGBC(32,  32,  32,  PRIM_RECT | RECT_TEXTURE),
    COLOR_RGBC(24,  128, 40,  PRIM_RECT | RECT_TEXTURE),
    COLOR_RGBC(8,   184, 96,  PRIM_RECT | RECT_TEXTURE),
    COLOR_RGBC(128, 0,   0,   PRIM_RECT | RECT_TEXTURE),
    COLOR_RGBC(24,  128, 40,  PRIM_RECT | RECT_TEXTURE),
    COLOR_RGBC(100, 100, 100, PRIM_RECT | RECT_TEXTURE),
    COLOR_RGBC(128, 128, 128, PRIM_RECT | RECT_TEXTURE)
};

static s16 g_StringColorId = StringColorId_White; /** `e_ColorId` */
// 2 bytes of padding.
static s32 g_StringLayerIdx = DEFAULT_TEXT_LAYER_IDX;

// ========================================
// GLOBAL VARIABLES
// ========================================

DVECTOR      g_StringPosition;
s32          g_StringPositionX1;
s_MapMsgLine g_MapMsg_ActiveLine;
s32          g_MapMsg_WidthIdx;
s32          g_MapMsg_Widths[12];
GsSPRITE     g_MapMsg_GlyphSprite;
s16          g_GlyphSpritePositionX;
s32          D_800C3920;

void Gfx_StringPositionSet(s32 x, s32 y) // 0x8004A87C
{
    #define OFFSET_FROM_CENTER_X 160
    #define OFFSET_FROM_CENTER_Y 112

    if (x != NO_VALUE)
    {
        g_StringPosition.vx = x - OFFSET_FROM_CENTER_X;
        g_StringPositionX1  = (s16)(x - OFFSET_FROM_CENTER_X);
    }

    if (y != NO_VALUE)
    {
        g_StringPosition.vy = y - OFFSET_FROM_CENTER_Y;
    }

    g_StringLayerIdx = DEFAULT_TEXT_LAYER_IDX;

    #undef OFFSET_FROM_CENTER_X
    #undef OFFSET_FROM_CENTER_Y
}

void Gfx_StringLayerIdxSet(s32 layerIdx) // 0x8004A8C0
{
    g_StringLayerIdx = layerIdx;
}

void Gfx_StringLayerIdxReset(void) // 0x8004A8CC
{
    g_StringLayerIdx = DEFAULT_TEXT_LAYER_IDX;
}

void Gfx_StringColorSet(s16 colorId) // 0x8004A8DC
{
    g_StringColorId = colorId;
}

bool Gfx_StringDraw(char* str, s32 displayLength) // 0x8004A8E8
{
    #define WIDE_SPACE_SIZE 10
    #define ATLAS_BASE_Y    240

    // TODO: This only works for one case. There may originally have been some other generic macro.
    #define setSprtUvClut(glyphSprt, idx, clut)                                                                                                     \
    *((u32*)&(glyphSprt)->u0) = (((idx) % FONT_12X16_ATLAS_COLUMN_COUNT) * FONT_12X16_GLYPH_SIZE_X) + /* `u0`:   Column in atlas. */            \
                                (ATLAS_BASE_Y << 8)                                                 + /* `v0`:   Row 0 in atlas with offset. */ \
                                ((clut) << 16)                                                        /* `clut`: Packed magic value. */

    s32       posX;
    s32       posY;
    s32       u0;
    s32       sizeCpy;
    s32       glyphIdx;
    u32       glyphColor;
    u32       charCode;
    u8*       strCpy;
    bool      result;
    s32       glyphWidth;
    s32       posXCpy;
    GsOT*     ot;
    PACKET*   packet;
    DR_TPAGE* tPage;
    POLY_FT4* glyphPoly;
    SPRT*     glyphSprt;

    // Create local argument copies.
    strCpy  = str;
    sizeCpy = displayLength;

    packet = NULL;
    result = false;

    // Set base screen position.
    posX = g_StringPosition.vx;
    posY = g_StringPosition.vy;

    glyphColor = STRING_COLORS[g_StringColorId];
    ot         = &g_OtTags0[g_ActiveBufferIdx][g_StringLayerIdx];

    if (!g_SysWork.enableHalfHeightGlyphs)
    {
        packet = GsOUT_PACKET_P;
    }

    // Parse string.
    while (sizeCpy > 0)
    {
        charCode = *strCpy;

        // TODO: Try refactoring into switch.

        // Convert literal `!` and `&` into `char`s mappable to representative atlas glyphs.
        if (charCode == '!')
        {
            charCode = '\\';
        }
        else if (charCode == '&')
        {
            charCode = '^';
        }

        // Space.
        if (charCode == '_')
        {
            posX += FONT_12X16_SPACE_SIZE;
        }
        // Wide space.
        else if (charCode == '\v')
        {
            posX += WIDE_SPACE_SIZE;
        }
        // Start of header.
        else if (charCode == '\x01')
        {
            posX--;
        }
        // Regular character.
        else if (charCode >= GLYPH_TABLE_ASCII_OFFSET && charCode <= 'z')
        {
            sizeCpy--;

            // Draw glyph sprite.
            if (g_SysWork.enableHalfHeightGlyphs)
            {
                glyphPoly = (POLY_FT4*)GsOUT_PACKET_P;

                glyphIdx   = charCode - GLYPH_TABLE_ASCII_OFFSET;
                glyphWidth = FONT_12X16_GLYPH_WIDTHS[glyphIdx];

                setPolyFT4(glyphPoly);
                setRGB0(glyphPoly, glyphColor, glyphColor >> 8, glyphColor >> 16);
                setXY4(glyphPoly,
                       posX,                             posY * 2,
                       posX,                             (posY * 2) + 30,
                       posX + FONT_12X16_GLYPH_SIZE_X, posY * 2,
                       posX + FONT_12X16_GLYPH_SIZE_X, (posY * 2) + 30);

                posX += glyphWidth;

                u0 = (glyphIdx % FONT_12X16_ATLAS_COLUMN_COUNT) * FONT_12X16_GLYPH_SIZE_X;

                *((u32*)&glyphPoly->u0) = u0 + (0xF000 + (0x7FD3 << 16));                                                    // `u0`, `v0`, `clut`.
                *((u32*)&glyphPoly->u1) = u0 + (((((glyphIdx / FONT_12X16_ATLAS_COLUMN_COUNT) & 0xF) | 16) << 16) | 0xFF00); // `u1`, `v1`, `page`.
                *((u16*)&glyphPoly->u2) = u0 - 0xFF4;                                                                        // `u2`, `v2`.
                *((u16*)&glyphPoly->u3) = u0 - 0xF4;                                                                         // `u3`, `v3`.

                addPrim(ot, glyphPoly);
                GsOUT_PACKET_P = (u8*)glyphPoly + sizeof(POLY_FT4);
            }
            else
            {
                posXCpy = (u16)posX;

                glyphSprt              = (SPRT*)packet;
                *((u32*)&glyphSprt->w) = 0x10000C;

                glyphIdx = charCode - GLYPH_TABLE_ASCII_OFFSET;
                posX    += FONT_12X16_GLYPH_WIDTHS[glyphIdx];

                addPrimFast(ot, glyphSprt, 4);
                *((u32*)&glyphSprt->r0)   = glyphColor;
                *((u32*)(&glyphSprt->x0)) = posXCpy + (posY << 16);

                setSprtUvClut(glyphSprt, glyphIdx, 0x7FD3); // TODO: Demagic CLUT arg.
                //*((u32*)&glyphSprt->u0) = ((glyphIdx % FONT_12X16_ATLAS_COLUMN_COUNT) * FONT_12X16_GLYPH_SIZE_X) + 0xF000 + (0x7FD3 << 16); // `u0`, `v0`, `clut`.

                packet += sizeof(SPRT);

                tPage = (DR_TPAGE*)packet;
                setDrawTPage(tPage, 0, 1, ((glyphIdx / FONT_12X16_ATLAS_COLUMN_COUNT) & 0xF) | 16);
                addPrim(ot, tPage);

                packet += sizeof(DR_TPAGE);
            }
        }
        // Newline.
        else if (charCode == '\n')
        {
            posX  = g_StringPositionX1;
            posY += FONT_12X16_GLYPH_SIZE_Y;
        }
        // New color.
        else if (charCode >= '\x01' && charCode < '\b')
        {
            glyphColor      = STRING_COLORS[charCode];
            g_StringColorId = charCode;
        }
        // Terminator.
        else if (charCode == '\0')
        {
            result = true;
            break;
        }

        strCpy++;
    }

    if (!g_SysWork.enableHalfHeightGlyphs)
    {
        GsOUT_PACKET_P = packet;
    }

    // Reset base string position?
    Math_SetDVectorFast(&g_StringPosition, posX, posY);

    return result;

    #undef WIDE_SPACE_SIZE
    #undef ATLAS_BASE_Y
}

s32 Gfx_MapMsg_WidthsCompute(s32 mapMsgIdx) // 0x8004ACF4
{
    s32 i;
    s32 j;
    s32 charCode;
    u8  msgCode;
    s32 posIdx;
    u8* mapMsg;

    g_MapMsg_WidthIdx       = 1;
    g_MapMsg_AudioLoadBlock = 0;

    for (i = (FONT_12X16_LINE_COUNT_MAX - 1); i >= 0; i--)
    {
        g_MapMsg_Widths[i] = 0;
    }

    mapMsg = g_MapOverlayHdr.mapMessages[mapMsgIdx];

    for (j = 0; j < FONT_12X16_LINE_COUNT_MAX; )
    {
        charCode = *mapMsg;

        switch (charCode)
        {
            case '\t':
            case '\n':
            case ' ':
                mapMsg++;
                break;

            case '_':
                ++mapMsg;
                g_MapMsg_Widths[g_MapMsg_WidthIdx - 1] += FONT_12X16_SPACE_SIZE;
                break;

            case MAP_MSG_CODE_MARKER:
                msgCode = *++mapMsg;
                posIdx  = *++mapMsg - '0';

                switch (msgCode)
                {
                    case MAP_MSG_CODE_COLOR:
                    case MAP_MSG_CODE_SELECT:
                    case MAP_MSG_CODE_TAB:
                        break;

                    case MAP_MSG_CODE_NEWLINE:
                        j++;
                        g_MapMsg_WidthIdx++;
                        break;

                    case MAP_MSG_CODE_END:
                        j = FONT_12X16_LINE_COUNT_MAX;
                        break;

                    case MAP_MSG_CODE_LINE_POSITION:
                        g_MapMsg_ActiveLine.positionIdx = posIdx;
                        break;

                    case MAP_MSG_CODE_JUMP:
                        if (posIdx == 2)
                        {
                            g_MapMsg_AudioLoadBlock = 3;
                        }

                        while (posIdx != ' ' && posIdx != '\t')
                        {
                            posIdx = *++mapMsg;
                        }

                        break;

                    case MAP_MSG_CODE_HALF_HEIGHT:
                        g_SysWork.enableHalfHeightGlyphs = true;
                        break;
                }

                mapMsg++;
                break;

            case 0:
                j = FONT_12X16_LINE_COUNT_MAX;
                break;

            default:
                // Convert literal `!` and `&` into `char`s mappable to representative atlas glyphs.
                if (charCode == '!')
                {
                    charCode = '\\';
                }
                else if (charCode == '&')
                {
                    charCode = '^';
                }

                g_MapMsg_Widths[g_MapMsg_WidthIdx - 1] += FONT_12X16_GLYPH_WIDTHS[charCode - GLYPH_TABLE_ASCII_OFFSET];
                mapMsg++;
                break;
        }
    }

    // TODO: JAP0 includes extra code and returns a value here.
}

s32 Gfx_MapMsg_StringDraw(char* mapMsg, s32 displayLength) // 0x8004AF18
{
    #define LINE_SPACE_SIZE 32
    #define CHARCODE_OFFSET '\''

    s32       glyphPosX;
    s32       glyphPosY;
    u32       temp_a0;
    s32       temp_a0_2;
    s32       digit;
    s32       i;
    s32       fractionDigits;
    s32       longestLineWidth;
    bool      isFraction;
    s32       lineIdx;
    u32       color;
    u8        code;
    s32       arg;
    s32       returnCode; // `e_MagMsgReturnCode`
    s32       charCode;
    s32       idx;
    s32       charWidth;
    GsOT*     ot;
    PACKET*   packet;
    DR_TPAGE* tPage;
    POLY_FT4* glyphPoly;
    SPRT*     glyphSprt;

    packet     = NULL;
    returnCode = MapMsgReturnCode_None;

    ot                  = (GsOT*)&g_OtTags0[g_ActiveBufferIdx][6];
    color               = STRING_COLORS[g_StringColorId];
    g_StringPosition.vx = -(g_MapMsg_Widths[0] >> 1);

    if (!g_SysWork.enableHalfHeightGlyphs)
    {
        packet = GsOUT_PACKET_P;
    }

    switch (g_MapMsg_ActiveLine.positionIdx)
    {
        case 0: // @unused
            g_StringPosition.vy = -92;
            break;

        case 1: // @unused
            g_StringPosition.vy = 76 - ((g_MapMsg_WidthIdx - 1) * FONT_12X16_GLYPH_SIZE_Y);
            break;

        case 2: // @unused
            g_StringPosition.vy = -60;
            break;

        case 3: // @unused
            g_StringPosition.vy = 44 - ((g_MapMsg_WidthIdx - 1) * FONT_12X16_GLYPH_SIZE_Y);
            break;

        case 4:
            g_StringPosition.vy = ((FONT_12X16_LINE_COUNT_MAX - g_MapMsg_WidthIdx) * 8) - 76;
            break;
    }

    longestLineWidth = g_MapMsg_Widths[0];
    for (i = 0; i < g_MapMsg_WidthIdx; i++)
    {
        if (longestLineWidth < g_MapMsg_Widths[i])
        {
            longestLineWidth = g_MapMsg_Widths[i];
        }
    }

    g_StringPosition.vx = -(longestLineWidth >> 1);
    g_StringPositionX1  = g_StringPosition.vx;
    glyphPosX           = g_StringPositionX1;
    glyphPosY           = g_StringPosition.vy;

    // Parse string.
    for (lineIdx = 0; lineIdx < FONT_12X16_LINE_COUNT_MAX;)
    {
        // Convert literal `!` and `&` into `char`s mappable to representative atlas glyphs.
        charCode = *mapMsg;
        if (charCode == '!')
        {
            charCode = '\\';
        }
        else if (charCode == '&')
        {
            charCode = '^';
        }

        // Process `char`.
        switch (charCode)
        {
            // Space.
            case '_':
                glyphPosX += FONT_12X16_SPACE_SIZE;
                mapMsg++;
                break;

            // Ignore spaces and tabs.
            case ' ':
            case '\t':
                mapMsg++;
                break;

            case MAP_MSG_CODE_MARKER:
                code = *++mapMsg;       // E.g. `L` in `~L4`.
                arg  = *++mapMsg - '0'; // E.g. `4` in `~L4`.

                switch (code)
                {
                    default:
                        break;

                    case MAP_MSG_CODE_NEWLINE:
                        lineIdx++;

                        switch (returnCode)
                        {
                            // Set new center-aligned position for next line.
                            case MapMsgReturnCode_AlignCenter:
                                glyphPosX = -(g_MapMsg_Widths[lineIdx] >> 1);
                                break;

                            // Tabulate next line.
                            case MapMsgReturnCode_SetByT:
                                glyphPosX = g_StringPositionX1;
                                break;

                            // Set new line.
                            default:
                                glyphPosX = -(longestLineWidth >> 1);
                                break;
                        }

                        glyphPosY += FONT_12X16_GLYPH_SIZE_Y;
                        break;

                    case MAP_MSG_CODE_JUMP:
                        fractionDigits = 0;
                        isFraction     = 0;
                        digit          = 0;

                        // Parse time value.
                        if (g_SysWork.mapMsgTimer == NO_VALUE)
                        {
                            s32 c;

                            mapMsg                  = mapMsg + 2;
                            c                       = *mapMsg;
                            g_MapMsg_AudioLoadBlock = arg + 1;

                            while (c != ')')
                            {
                                if (c == '.')
                                {
                                    isFraction = 1;
                                }
                                else
                                {
                                    if (isFraction != 0)
                                    {
                                        fractionDigits++;
                                    }

                                    digit *= 10;
                                    digit -= '0' - c;
                                }

                                mapMsg++;
                                c = *mapMsg;
                            }

                            digit = Q12(digit);
                            for (i = 0; i < fractionDigits; i++)
                            {
                                digit /= 10;
                            }

                            g_SysWork.mapMsgTimer = digit;
                            mapMsg                = mapMsg + 1;
                        }
                        else
                        {
                            while (arg != ' ' && arg != '\t')
                            {
                                arg = *++mapMsg;
                            }
                        }
                        break;

                    case MAP_MSG_CODE_MIDDLE:
                        returnCode = MapMsgReturnCode_AlignCenter;
                        glyphPosX  = -(g_MapMsg_Widths[lineIdx] >> 1);
                        break;

                    case MAP_MSG_CODE_TAB:
                        returnCode         = MapMsgReturnCode_SetByT;
                        g_StringPositionX1 = -120;
                        glyphPosX          = -120;
                        break;

                    case MAP_MSG_CODE_COLOR:
                        color           = STRING_COLORS[arg];
                        g_StringColorId = arg;
                        break;

                    case MAP_MSG_CODE_DISPLAY_ALL:
                        displayLength = MAP_MESSAGE_DISPLAY_ALL_LENGTH;
                        break;

                    case MAP_MSG_CODE_END:
                        returnCode = NO_VALUE;
                        lineIdx    = FONT_12X16_LINE_COUNT_MAX;
                        break;

                    case MAP_MSG_CODE_SELECT:
                        returnCode = arg;
                        lineIdx    = FONT_12X16_LINE_COUNT_MAX;
                        break;

                    case MAP_MSG_CODE_HALF_HEIGHT:
                        g_SysWork.enableHalfHeightGlyphs = true;
                        break;
            }

            mapMsg++;
            break;

        // Terminator.
        case '\0':
            returnCode = MapMsgReturnCode_Terminate;
            lineIdx    = FONT_12X16_LINE_COUNT_MAX;
            break;

        // Draw glyph sprite.
        default:
            displayLength--;

            // Half-height.
            if (g_SysWork.enableHalfHeightGlyphs)
            {
                glyphPoly = (POLY_FT4*)GsOUT_PACKET_P;

                idx       = charCode - CHARCODE_OFFSET;
                charWidth = FONT_12X16_GLYPH_WIDTHS[charCode - CHARCODE_OFFSET];

                setPolyFT4(glyphPoly);
                setRGB0(glyphPoly, (s8)color, (s8)(color >> 8), (s8)(color >> 16));
                setXY4(glyphPoly,
                       glyphPosX,                           glyphPosY * 2,
                       glyphPosX,                           (glyphPosY * 2) + 30,
                       glyphPosX + FONT_12X16_GLYPH_SIZE_X, glyphPosY * 2,
                       glyphPosX + FONT_12X16_GLYPH_SIZE_X, (glyphPosY * 2) + 30);

                glyphPosX += charWidth;

                temp_a0 = (idx % FONT_12X16_ATLAS_COLUMN_COUNT) * FONT_12X16_GLYPH_SIZE_X;
                *((u32*)&glyphPoly->u0) = temp_a0 + 0xF000 + (0x7FD3 << 16);                                                   // `u0`, `v0`, `clut`.
                *((u32*)&glyphPoly->u1) = temp_a0 + (((((idx / FONT_12X16_ATLAS_COLUMN_COUNT) & 0xF) | 0x10) << 16) | 0xFF00); // `u1`, `v1`, `page`.
                *((u16*)&glyphPoly->u2) = temp_a0 - 0xFF4;
                *((u16*)&glyphPoly->u3) = temp_a0 - 244;

                addPrim(ot, glyphPoly);
                GsOUT_PACKET_P = (PACKET*)glyphPoly + sizeof(POLY_FT4);
            }
            // Full-height.
            else
            {
                temp_a0_2 = (u16)glyphPosX;

                glyphSprt              = (SPRT*)packet;
                *((u32*)&glyphSprt->w) = 0x10000C;

                idx        = charCode - CHARCODE_OFFSET;
                glyphPosX += FONT_12X16_GLYPH_WIDTHS[idx];

                addPrimFast(ot, glyphSprt, 4);
                *((u32*)&glyphSprt->r0)   = color;
                *((u32*)(&glyphSprt->x0)) = temp_a0_2 + ((glyphPosY) << 16);
                *((u32*)&glyphSprt->u0)   = (s32)(((idx % FONT_12X16_ATLAS_COLUMN_COUNT) * FONT_12X16_GLYPH_SIZE_X) + 0xF000 + (0x7FD3 << 16)); // `u0`, `v0`, `clut`.

                packet += sizeof(SPRT);

                tPage = (DR_TPAGE*)packet;
                setDrawTPage(tPage, 0, 1, ((idx / FONT_12X16_ATLAS_COLUMN_COUNT) & 0xF) | 0x10);
                addPrim(ot, tPage);

                packet += sizeof(DR_TPAGE);
            }

            mapMsg++;

            // Stop drawing if length exceeded.
            if (displayLength <= 0)
            {
                if (!g_SysWork.enableHalfHeightGlyphs)
                {
                    GsOUT_PACKET_P = packet;
                }

                return returnCode;
            }
        }
    }

    if (!g_SysWork.enableHalfHeightGlyphs)
    {
        GsOUT_PACKET_P = packet;
    }

    Math_SetDVectorFast(&g_StringPosition, glyphPosX, glyphPosY);
    return returnCode;

    #undef LINE_SPACE_SIZE
    #undef CHARCODE_OFFSET
}

void func_8004B658(void) // 0x8004B658
{
    g_MapMsg_GlyphSprite.attribute = 64;
    g_MapMsg_GlyphSprite.cx        = 304;
    g_MapMsg_GlyphSprite.v         = 240;
    g_MapMsg_GlyphSprite.h         = 16;
}

void Gfx_MapMsg_Reset(void) // 0x8004B684
{
    g_MapMsg_WidthIdx                = 1;
    g_MapMsg_ActiveLine.unused       = 0;
    g_MapMsg_ActiveLine.positionIdx  = 1;
    g_StringPositionX1               = -120;
    g_StringColorId                  = StringColorId_White;
    g_SysWork.enableHalfHeightGlyphs = false;
}

void Gfx_GlyphSprite_PositionSet(s16 x, s16 y) // 0x8004B6D4
{
    if (x != NO_VALUE)
    {
        g_MapMsg_GlyphSprite.x = x + (-g_GameWork.gsScreenWidth / 2);
        g_GlyphSpritePositionX = g_MapMsg_GlyphSprite.x;
    }

    if (y != NO_VALUE)
    {
        g_MapMsg_GlyphSprite.y = y + (-g_GameWork.gsScreenHeight / 2);
    }
}

void func_8004B74C(s16 arg0) // 0x8004B74C
{
    if (arg0 < 0 || arg0 >= 5)
    {
        D_800C391E = 0;
        return;
    }

    D_800C391E = arg0;
}

void func_8004B76C(char* str, bool useFixedWidth) // 0x8004B76C
{
    #define GLYPH_SIZE_X       11
    #define GLYPH_SIZE_Y       12
    #define SPACE_SIZE         12
    #define LINE_SPACE_SIZE    16
    #define ATLAS_COLUMN_COUNT 21

    s32       tileRow;
    s32       glyphIdx;
    GsOT*     ot;
    GsSPRITE* glyphSprt;

    glyphSprt  = (GsSPRITE*)PSX_SCRATCH_ADDR(0x30);
    *glyphSprt = g_MapMsg_GlyphSprite;
    ot         = &g_OrderingTable2[g_ActiveBufferIdx];

    // Parse string.
    while (*str != '\0')
    {
        switch (*str)
        {
            // Draw glyph sprite.
            default:
                glyphIdx     = *str - GLYPH_TABLE_ASCII_OFFSET;
                tileRow      = glyphIdx / ATLAS_COLUMN_COUNT;
                glyphSprt->u = (glyphIdx % ATLAS_COLUMN_COUNT) * GLYPH_SIZE_Y;

                if (useFixedWidth)
                {
                    glyphSprt->w = GLYPH_SIZE_X;
                }
                else
                {
                    glyphSprt->w = FONT_12X16_GLYPH_WIDTHS[glyphIdx];
                }

                glyphSprt->tpage = (tileRow & 0xF) | 0x10;
                glyphSprt->cx    = 304;
                glyphSprt->cy    = D_800C391E + 506;

                GsSortFastSprite(glyphSprt, ot, 4);

                glyphSprt->x += glyphSprt->w;
                break;

            // Space.
            case ' ':
            case '\t':
                glyphSprt->x += SPACE_SIZE;
                break;

            // Backspace.
            case '~':
            case '\b':
                glyphSprt->x -= SPACE_SIZE;
                break;

            // Newline.
            case '\n':
                glyphSprt->x  = g_GlyphSpritePositionX;
                glyphSprt->y += LINE_SPACE_SIZE;
                break;

            // Carriage return.
            case '\r':
                glyphSprt->x  = g_GlyphSpritePositionX;
                glyphSprt->y -= LINE_SPACE_SIZE;
                break;
        }

        str++;
    }

    g_MapMsg_GlyphSprite = *glyphSprt;

    #undef GLYPH_SIZE_X
    #undef GLYPH_SIZE_Y
    #undef SPACE_SIZE
    #undef LINE_SPACE_SIZE
    #undef ATLAS_COLUMN_COUNT
}

void Gfx_StringDrawInt(s32 widthMin, s32 val) // 0x8004B9F8
{
    #define GLYPH_SIZE_X       11
    #define ATLAS_COLUMN_COUNT 10

    s32   quotient;
    s32   isNegative;
    s32   i;
    char* str;

    if (widthMin > 0)
    {
        for (i = 0; i < (widthMin - 1); i++)
        {
            g_MapMsg_GlyphSprite.x += GLYPH_SIZE_X;
        }
    }

    str  = (char*)PSX_SCRATCH_ADDR(0x2F);
    *str = 0;

    if (val < 0)
    {
        isNegative = true;
        val        = -val;
    }
    else
    {
        isNegative = false;
    }

    // Wrap atlas row?
    while (val >= ATLAS_COLUMN_COUNT)
    {
        str--;
        quotient = (val / ATLAS_COLUMN_COUNT) >> 32;
        *str     = (val - (quotient * ATLAS_COLUMN_COUNT)) + '0';

        if (widthMin > 0)
        {
            g_MapMsg_GlyphSprite.x -= GLYPH_SIZE_X;
        }

        val = quotient;
    }

    str--;
    *str = val + '0';

    if (isNegative)
    {
        str--;
        *str                    = '-';
        g_MapMsg_GlyphSprite.x -= GLYPH_SIZE_X;
    }

    // Draw numeric string.
    Gfx_StringDraw(str, 5);
    return;

    #undef GLYPH_SIZE_X
    #undef ATLAS_COLUMN_COUNT
}

const s32 unused_Rodata_80025E88 = 0xC9457F00; /** @unused */
