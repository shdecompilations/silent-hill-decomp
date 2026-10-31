#include <psyq/libapi.h>

#include "game.h"

#include "bodyprog/bodyprog.h"
#include "bodyprog/screen/screen_data.h"
#include "bodyprog/screen/screen_draw.h"
#include "bodyprog/text/text_draw.h"
#include "bodyprog/math/math.h"

// TODO: Make FS_BUFFER define for 0x801E1E80 used in JPN funcs below.

#ifndef PAD_HACK_IGNORE
#if VERSION_REGION_IS(NTSCJ)
    const u32 __pad_rodata_80025D54 = 0;
#elif !VERSION_REGION_IS(PAL)
    const s32 __pad_rodata_80025D68 = 0;
#endif
#endif

// TODO: Find better name for this.
#if VERSION_REGION_IS(PAL)
    #define SPRITE_CX 816
#else
    #define SPRITE_CX 304
#endif

// ========================================
// STATIC VARIABLES
// ========================================

#if VERSION_IS(EUR)
INCLUDE_RODATA("bodyprog/nonmatchings/text/text_draw", D_80026898);
#endif

/** @brief Glyph widths of the 12x16 font. Used for kerning. */
static const u8 FONT_12X16_GLYPH_WIDTHS[] = {
#if VERSION_REGION_IS(PAL)
    3,  7,  7,  11, 11, 4,  10, 4,  6,  10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 4,  4,
    10, 11, 10, 8,  13, 12, 12, 12, 13, 11, 11, 13, 12, 9,  9,  12, 12, 13, 12, 13, 11,
    13, 12, 10, 11, 13, 12, 12, 12, 11, 12, 6,  4,  6,  8,  0,  3,  9,  10, 9,  9,  9,
    7,  11, 11, 6,  6,  10, 6,  13, 11, 10, 11, 10, 8,  8,  7,  10, 10, 12, 10, 10, 9,
    11, 9,  9,  9,  9,  9,  0,  0,  9,  9,  9,  9,  9,  6,  6,  6,  6,  0,  11, 10, 10,
    10, 10, 10, 11, 0,  10, 10, 10, 10, 0,  8,  7,  12, 12, 0,  0,  0,  0,  0,  0,  0,
    0,  0
#else
    3,  7,  7,  11, 11, 4,  10, 4,  6,  10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 4,  4,
    10, 11, 10, 8,  13, 12, 12, 12, 13, 11, 11, 13, 12, 9,  9,  12, 12, 13, 12, 13, 11,
    13, 12, 10, 11, 13, 12, 12, 12, 11, 12, 6,  4,  6,  8,  0,  3,  9,  10,  9, 9,  9,
    7,  11, 11, 6,  6,  10, 6,  13, 11, 10, 11, 10, 8,  8,  7,  10, 10, 12, 10, 10, 9
#endif
};

/** @brief See `e_StringColorId`. */
#if VERSION_REGION_IS(PAL)
#else
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
#endif

#if VERSION_REGION_IS(NTSCJ) && !defined(PAD_HACK_IGNORE)
const u32 __pad_rodata_80025DCC[2] = { 0, 0 };
#endif

static s16 g_StringColorId = StringColorId_White; /** `e_ColorId` */
// 2 bytes of padding.
static s32 g_StringLayerIdx = DEFAULT_TEXT_LAYER_IDX;
#if VERSION_REGION_IS(PAL)
u8 D_800AD098[4] = { 0, 0, 1, 2 };
#endif

#if VERSION_REGION_IS(NTSCJ)
// Additional color state for the JPN map message functions, set by `Gfx_StringColorSet_JP`.
static s16 g_StringColorId_JP = StringColorId_White;
// 2 bytes of padding.
static u32 STRING_COLORS_JP[StringColorId_Count] = {
    COLOR_RGBC(160, 128, 64,  PRIM_RECT | RECT_TEXTURE),
    COLOR_RGBC(0,   0,   128, PRIM_RECT | RECT_TEXTURE),
    COLOR_RGBC(24,  128, 40,  PRIM_RECT | RECT_TEXTURE),
    COLOR_RGBC(8,   184, 96,  PRIM_RECT | RECT_TEXTURE),
    COLOR_RGBC(128, 0,   0,   PRIM_RECT | RECT_TEXTURE),
    COLOR_RGBC(24,  128, 40,  PRIM_RECT | RECT_TEXTURE),
    COLOR_RGBC(100, 100, 100, PRIM_RECT | RECT_TEXTURE),
    COLOR_RGBC(128, 128, 128, PRIM_RECT | RECT_TEXTURE)
};
#endif

// ========================================
// GLOBAL VARIABLES
// ========================================

#if VERSION_REGION_IS(NTSCJ)
DVECTOR      g_StringPosition;
s32          g_StringPositionX1; // Copy of `g_StringPosition.vx` as `s32`. It's unclear what for.
#ifndef PAD_HACK_IGNORE
s32          __pad_bss_800C5DE0[2];
#endif
GsSPRITE     g_MapMsg_GlyphSprite;
s16          g_GlyphSpritePositionX;
s16          D_800C391E;
DVECTOR      g_StringPosition_JP; // Separate string position for the JPN map message functions.
s32          g_StringPositionX1_JP;
s_MapMsgLine g_MapMsg_ActiveLine;
#ifndef PAD_HACK_IGNORE
s8           __pad_bss_800C5E1A[2];
#endif
s32          g_MapMsg_WidthIdx;
s32          D_800C5E20;
#ifndef PAD_HACK_IGNORE
s32          __pad_bss_800C5E24[3];
#endif
s32          g_MapMsg_Widths[9];
s32          D_800C3920;
#elif VERSION_REGION_IS(PAL)
DVECTOR      g_StringPosition;
s32          D_800C2E9C;
s32          D_800C2EA0;
s32          g_StringPositionX1; // Copy of `g_StringPosition.vx` as `s32`. It's unclear what for.
s_MapMsgLine g_MapMsg_ActiveLine;
#ifndef PAD_HACK_IGNORE
s8           __pad_bss_800C2EA9[2];
#endif
s32          g_MapMsg_WidthIdx;
#ifndef PAD_HACK_IGNORE
s32          __pad_bss_800C2EB0[2];
#endif
s32          g_MapMsg_Widths[12];
GsSPRITE     g_MapMsg_GlyphSprite;
s16          g_GlyphSpritePositionX;
s16          D_800C391E;
#else
DVECTOR      g_StringPosition;
s32          g_StringPositionX1; // Copy of `g_StringPosition.vx` as `s32`. It's unclear what for.
s_MapMsgLine g_MapMsg_ActiveLine;
#ifndef PAD_HACK_IGNORE
s8           __pad_bss_800C38B2[2];
#endif
s32          g_MapMsg_WidthIdx;
#ifndef PAD_HACK_IGNORE
s32          __pad_bss_800C38B8[4];
#endif
s32          g_MapMsg_Widths[12];
GsSPRITE     g_MapMsg_GlyphSprite;
s16          g_GlyphSpritePositionX;
s16          D_800C391E;
s32          D_800C3920;
#ifndef PAD_HACK_IGNORE
s32          __pad_bss_800C3924;
#endif
#endif

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

#if VERSION_IS(EUR)
INCLUDE_RODATA("bodyprog/nonmatchings/text/text_draw", STRING_COLORS);
INCLUDE_ASM("bodyprog/nonmatchings/text/text_draw", Gfx_StringDraw);
#else
bool Gfx_StringDraw(char* str, s32 displayLength) // 0x8004A8E8
{
    #define WIDE_SPACE_SIZE 10
    #define ATLAS_BASE_Y    240

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
                       posX,                           posY * 2,
                       posX,                           (posY * 2) + 30,
                       posX + FONT_12X16_GLYPH_SIZE_X, posY * 2,
                       posX + FONT_12X16_GLYPH_SIZE_X, (posY * 2) + 30);

                posX += glyphWidth;

                u0 = (glyphIdx % FONT_12X16_ATLAS_COLUMN_COUNT) * FONT_12X16_GLYPH_SIZE_X;

                setUV0AndClutSum(glyphPoly, u0, 0xF0, 0x7FD3);
                setUV1AndTPageSum(glyphPoly, u0, 0xFF, ((glyphIdx / FONT_12X16_ATLAS_COLUMN_COUNT) & 0xF) | 16);
                setUV2Sum(glyphPoly, u0 + FONT_12X16_GLYPH_SIZE_X, 0xF0);
                setUV3Sum(glyphPoly, u0 + FONT_12X16_GLYPH_SIZE_X, 0xFF);

                addPrim(ot, glyphPoly);
                GsOUT_PACKET_P = (u8*)glyphPoly + sizeof(POLY_FT4);
            }
            else
            {
                glyphSprt = (SPRT*)packet;
                setWHFast(glyphSprt, FONT_12X16_GLYPH_SIZE_X, FONT_12X16_GLYPH_SIZE_Y);

                glyphIdx = charCode - GLYPH_TABLE_ASCII_OFFSET;

                addPrimFast(ot, glyphSprt, 4);
                setRGBC0Word(glyphSprt, glyphColor);
                setXY0Fast(glyphSprt, posX, posY);
                posX += FONT_12X16_GLYPH_WIDTHS[glyphIdx];

                setUV0AndClutSum(glyphSprt, (glyphIdx % FONT_12X16_ATLAS_COLUMN_COUNT) * FONT_12X16_GLYPH_SIZE_X, ATLAS_BASE_Y, 0x7FD3); // TODO: Demagic CLUT arg.

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
        // New color (`e_ColorId`).
        else if (charCode > 0 && charCode <= 7)
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
#endif

// ========================================
// USA MAPMSG FUNCTIONS
// (JPN versions relocated later in file)
// ========================================

#if !VERSION_REGION_IS(NTSCJ)

#if VERSION_IS(EUR)
INCLUDE_ASM("bodyprog/nonmatchings/text/text_draw", Gfx_MapMsg_WidthsCompute);
#else
s32 Gfx_MapMsg_WidthsCompute(s32 mapMsgIdx) // 0x8004ACF4
{
    s32 i;
    s32 j;
    s32 charCode;
    u8  msgCode;
    s32 posIdx;
    u8* mapMsg;

    g_MapMsg_WidthIdx  = 1;
    g_MapMsg_AudioType = MapMsgAudioType_None;

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
            // Ignore tabs, newlines, and spaces.
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
                            g_MapMsg_AudioType = MapMsgAudioType_VoiceStream;
                        }

                        // Ignore spaces and tabs.
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
}
#endif

#if VERSION_IS(EUR)
INCLUDE_ASM("bodyprog/nonmatchings/text/text_draw", Gfx_MapMsg_StringDraw);
#else
s32 Gfx_MapMsg_StringDraw(char* mapMsg, s32 displayLength) // 0x8004AF18
{
    #define LINE_SPACE_SIZE 32
    #define CHARCODE_OFFSET '\''

    s32       glyphPosX;
    s32       glyphPosY;
    u32       temp_a0;
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

                            mapMsg             = mapMsg + 2;
                            c                  = *mapMsg;
                            g_MapMsg_AudioType = arg + 1;

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
                            // Ignore spaces and tabs.
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
                setUV0AndClutSum(glyphPoly, temp_a0, 0xF0, 0x7FD3);
                setUV1AndTPageSum(glyphPoly, temp_a0, 0xFF, ((idx / FONT_12X16_ATLAS_COLUMN_COUNT) & 0xF) | 0x10);
                setUV2Sum(glyphPoly, temp_a0 + FONT_12X16_GLYPH_SIZE_X, 0xF0);
                setUV3Sum(glyphPoly, temp_a0 + FONT_12X16_GLYPH_SIZE_X, 0xFF);

                addPrim(ot, glyphPoly);
                GsOUT_PACKET_P = (PACKET*)glyphPoly + sizeof(POLY_FT4);
            }
            // Full-height.
            else
            {
                glyphSprt = (SPRT*)packet;
                setWHFast(glyphSprt, FONT_12X16_GLYPH_SIZE_X, FONT_12X16_GLYPH_SIZE_Y);

                idx = charCode - CHARCODE_OFFSET;

                addPrimFast(ot, glyphSprt, 4);
                setRGBC0Word(glyphSprt, color);
                setXY0Fast(glyphSprt, glyphPosX, glyphPosY);
                glyphPosX += FONT_12X16_GLYPH_WIDTHS[idx];
                setUV0AndClutSum(glyphSprt, (idx % FONT_12X16_ATLAS_COLUMN_COUNT) * FONT_12X16_GLYPH_SIZE_X, 0xF0, 0x7FD3);

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
#endif

#endif

void func_8004B658(void) // 0x8004B658
{
    g_MapMsg_GlyphSprite.attribute = 64;
    g_MapMsg_GlyphSprite.cx        = SPRITE_CX;
    g_MapMsg_GlyphSprite.v         = 240;
    g_MapMsg_GlyphSprite.h         = 16;
    
#if VERSION_REGION_IS(NTSCJ)
    func_8003652C();
#endif
}

#if !VERSION_REGION_IS(NTSCJ)
// USA-only Gfx_MapMsg_Reset (JPN version is relocated later in file.)
void Gfx_MapMsg_Reset(void) // 0x8004B684
{
    g_MapMsg_WidthIdx                = 1;
    g_MapMsg_ActiveLine.unused       = 0;
    g_MapMsg_ActiveLine.positionIdx  = 1;
    g_StringPositionX1               = -120;
    g_StringColorId                  = StringColorId_White;
    g_SysWork.enableHalfHeightGlyphs = false;
}
#endif

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
    }
    else
    {
        D_800C391E = arg0;
    }
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
                glyphSprt->cx    = SPRITE_CX;
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

void Gfx_StringDrawInt(s32 lengthMin, s32 val) // 0x8004B9F8
{
    #define GLYPH_SIZE_X       11
    #define ATLAS_COLUMN_COUNT 10

    s32   quotient;
    s32   isNegative;
    s32   i;
    char* str;

    if (lengthMin > 0)
    {
        for (i = 0; i < (lengthMin - 1); i++)
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

        if (lengthMin > 0)
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

#if !VERSION_REGION_IS(NTSCJ)
const s32 unused_Rodata_80025E88 = 0xC9457F00; /** @unused Garbage padding? */
#endif

// ========================================
// JPN MAPMSG FUNCTIONS
// ========================================

#if VERSION_REGION_IS(NTSCJ)

void Gfx_MapMsg_Reset(void) // 0x8004AEA8
{
    g_MapMsg_ActiveLine.unused = 0;
    g_MapMsg_ActiveLine.positionIdx = 1;
    g_MapMsg_WidthIdx = 1;
    g_StringPosition_JP.vx = -0x78;
    g_StringPosition_JP.vy = 0x4C;
    g_StringPositionX1_JP = -0x78;
    g_StringColorId_JP = StringColorId_White;
    g_SysWork.enableHalfHeightGlyphs = 0;
}

// Very similar to `Gfx_GlyphSprite_PositionSet` (that JPN already includes)
void func_8004AF14(s32 x, s32 y) // 0x8004AF14
{
    if (x != NO_VALUE)
    {
        g_StringPosition_JP.vx = x - (SCREEN_WIDTH / 2);
        g_StringPositionX1_JP = g_StringPosition_JP.vx;
    }

    if (y != NO_VALUE)
    {
        g_StringPosition_JP.vy = y - (FRAMEBUFFER_HEIGHT_PROGRESSIVE / 2);
    }
}

void Gfx_StringColorSet_JP(s16 colorId) // 0x8004A8DC
{
    g_StringColorId_JP = colorId;
}

s32 Gfx_MapMsg_WidthsCompute(s32 mapMsgIdx) // 0x8004AF5C
{
    RECT  rect;
    s32   i;
    s32   temp;
    s32   j;
    s32   ret;
    s32   posIdx;
    s32   msgCode;
    s32   msgCode2;
    s32   charCode;
    char* mapMsg;

    ret                = 0;
    g_MapMsg_WidthIdx         = 1;
    g_MapMsg_AudioType = MapMsgAudioType_None;

    for (i = 0; i < FONT_12X16_LINE_COUNT_MAX; i++)
    {
        g_MapMsg_Widths[i] = 0;
    }

    mapMsg = g_MapOverlayHdr.mapMessages[mapMsgIdx];

    for (j = 0; j < FONT_12X16_LINE_COUNT_MAX;)
    {
        for (i = 0; i < 21;)
        {
            charCode = *mapMsg;

            switch (charCode)
            {
                // Ignore tabs, newlines, and spaces. TODO: These serve a purpose in Japanese code?
                case '\t':
                case '\n':
                case ' ':
                    mapMsg++;
                    break;

                case MAP_MSG_CODE_MARKER:
                    msgCode = *++mapMsg;
                    posIdx  = *++mapMsg - '0';

                    if (msgCode == MAP_MSG_CODE_NEWLINE)
                    {
                        j++;
                        g_MapMsg_Widths[g_MapMsg_WidthIdx - 1] = i;
                        i                          = 21;
                        g_MapMsg_WidthIdx++;
                    }
                    else if (msgCode == MAP_MSG_CODE_JUMP)
                    {
                        if (posIdx == 2)
                        {
                            g_MapMsg_AudioType = MapMsgAudioType_VoiceStream;
                        }

                        while (posIdx != ' ' && posIdx != '\t')
                        {
                            posIdx = *++mapMsg;
                        }
                    }
                    else
                    {
                        mapMsg++;
                    }
                    break;

                case 0:
                    j                          = FONT_12X16_LINE_COUNT_MAX;
                    g_MapMsg_Widths[g_MapMsg_WidthIdx - 1] = i;
                    i                          = 21;
                    break;

                default:
                    mapMsg += 2;
                    i++;
                    break;
            }
        }
    }

    mapMsg = g_MapOverlayHdr.mapMessages[mapMsgIdx];

    for (j = 0; j < FONT_12X16_LINE_COUNT_MAX; j++)
    {
        setRECT(&rect, 0, 0, 0, 0);

        for (i = 0; i < 21;)
        {
            charCode = *mapMsg;

            switch (charCode)
            {
                // Ignore tabs, newlines, and spaces.
                case '\t':
                case '\n':
                case ' ':
                    mapMsg++;
                    break;

                case MAP_MSG_CODE_MARKER:
                    msgCode2 = *++mapMsg;
                    posIdx   = *++mapMsg - '0';

                    switch (msgCode2)
                    {
                        case MAP_MSG_CODE_COLOR:
                        case MAP_MSG_CODE_TAB:
                            break;

                        case MAP_MSG_CODE_NEWLINE:
                            switch (g_MapMsg_ActiveLine.positionIdx)
                            {
                                case 4:
                                    setRECT(&rect,
                                            (j % 5) << 6, (j / 5) ? (SCREEN_HEIGHT * 2) : FONT_12X16_GLYPH_SIZE_Y,
                                            i * 3, 16);
                                    break;

                                default:
                                    setRECT(&rect,
                                            j << 6, (g_MapMsg_ActiveLine.positionIdx & 0x1) ? (SCREEN_HEIGHT * 2) : FONT_12X16_GLYPH_SIZE_Y,
                                            i * 3, 16);
                                    break;
                            }

                            i = 21;
                            break;

                        case MAP_MSG_CODE_LINE_POSITION:
                            g_MapMsg_ActiveLine.positionIdx = posIdx;
                            break;

                        case MAP_MSG_CODE_JUMP:
                            // Ignore spaces and tabs.
                            while (posIdx != ' ' && posIdx != '\t')
                            {
                                posIdx = *++mapMsg;
                            }
                            break;

                        case MAP_MSG_CODE_HALF_HEIGHT:
                            g_SysWork.enableHalfHeightGlyphs = true;
                            break;

                        case MAP_MSG_CODE_SELECT:
                            ret = posIdx;
                            break;
                    }

                    mapMsg++;
                    break;

                case 0:
                    switch (g_MapMsg_ActiveLine.positionIdx)
                    {
                        case 4:
                            setRECT(&rect,
                                    (j % 5) << 6, (j / 5) ? (SCREEN_HEIGHT * 2) : FONT_12X16_GLYPH_SIZE_Y,
                                    i * 3, 16);
                            break;

                        default:
                            setRECT(&rect,
                                    j << 6, (g_MapMsg_ActiveLine.positionIdx & 0x1) ? (SCREEN_HEIGHT * 2) : FONT_12X16_GLYPH_SIZE_Y,
                                    i * 3, 16);
                            break;
                    }

                    i = 21;
                    j = FONT_12X16_LINE_COUNT_MAX;
                    break;

                default:
                    temp = func_8004C8AC(mapMsg);
                    if (temp == NO_VALUE)
                    {
                        mapMsg++;
                    }
                    else
                    {
                        D_800C3920 = g_MapMsg_Widths[j];
                        func_8004C8D8(temp, &i, 0);
                        mapMsg += 2;
                        i++;
                    }
                    break;
            }
        }

        if (rect.w != 0)
        {
            LoadImage(&rect, (u32*)0x801E1E80);
            DrawSync(0);
        }
    }

    return ret;
}

void func_8004B45C(s32 mapMsgBaseIdx, s32 arg1) // 0x8004B45C
{
    s32   sp10[3] = { 0 };
    RECT  rect;
    s32   j;
    s32   ret;
    s32   i;
    char* mapMsg;

    D_800C5E20 = arg1;

    for (i = 0; i < arg1; i++)
    {
        mapMsg = g_MapOverlayHdr.mapMessages[mapMsgBaseIdx + i];

        for (j = 0; j < 21;)
        {
            switch (*mapMsg)
            {
                // Ignore tabs and spaces.
                case '\t':
                case ' ':
                    mapMsg++;
                    break;

                case 0:
                    sp10[i] = j;
                    j       = 21;
                    break;

                default:
                    mapMsg += 2;
                    j++;
                    break;
            }
        }
    }

    for (i = 0; i < arg1; i++)
    {
        mapMsg = g_MapOverlayHdr.mapMessages[mapMsgBaseIdx + i];

        setRECT(&rect, 0, 0, 0, 0);

        for (j = 0; j < 21;)
        {
            switch (*mapMsg)
            {
                // Ignore tabs and spaces.
                case '\t':
                case ' ':
                    mapMsg++;
                    break;

                case 0:
                    setRECT(&rect,
                            ((i >> 1) << 6) + 192, ((i & 1) * 464) + FONT_12X16_GLYPH_SIZE_Y,
                            sp10[i] * 3, FONT_12X16_GLYPH_SIZE_Y);
                    j = 21;
                    break;

                default:
                    ret = func_8004C8AC(mapMsg);
                    if (ret == NO_VALUE)
                    {
                        mapMsg++;
                    }
                    else
                    {
                        D_800C3920 = sp10[i];
                        func_8004C8D8(ret, &j, 0);
                        mapMsg += 2;
                        j++;
                    }
                    break;
            }
        }

        if (rect.w != 0)
        {
            LoadImage(&rect, (u32*)0x801E1E80);
            if (i != 2)
            {
                DrawSync(0);
            }
        }
    }
}

s32 Gfx_MapMsg_StringDraw(char* mapMsg, s32 displayLength) // 0x8004B6F4
{
    GsOT*     ot2;
    s32       posY;
    s32       posX;
    s32       page;
    s32       decimalCount;
    s32       j;
    s32       value;
    s32       timer;
    s32       remaining;
    s32       hasDecimal;
    s32       glyphIdx;
    s32       lineIdx;
    s32       maxWidth;
    GsOT_TAG* ot;
    char*     str;
    u32       color;
    u8        code;
    u8        charCode;
    s32       digit;
    s32       arg;
    s32       result; // `e_MapMsgReturnCode`
    PACKET*   packet;
    DVECTOR*  pos;
    DVECTOR*  pos2;
    POLY_FT4* poly;

    str       = mapMsg;
    remaining = displayLength;
    packet    = NULL;
    result    = MapMsgReturnCode_None;
    ot        = &g_OtTags0[g_ActiveBufferIdx][6];
    color     = STRING_COLORS_JP[g_StringColorId_JP];

    if (!g_SysWork.enableHalfHeightGlyphs)
    {
        packet = GsOUT_PACKET_P;
    }

    switch (g_MapMsg_ActiveLine.positionIdx)
    {
        case 0:
            g_StringPosition_JP.vy = -92;
            break;

        case 1:
            g_StringPosition_JP.vy = 76 - ((g_MapMsg_WidthIdx - 1) * 16);
            break;

        case 2:
            g_StringPosition_JP.vy = -60;
            break;

        case 3:
            g_StringPosition_JP.vy = 44 - ((g_MapMsg_WidthIdx - 1) * 16);
            break;

        case 4:
            g_StringPosition_JP.vy = ((9 - g_MapMsg_WidthIdx) * 8) - 76;
            break;
    }

    maxWidth = g_MapMsg_Widths[0];
    for (glyphIdx = 1; glyphIdx < g_MapMsg_WidthIdx; glyphIdx++)
    {
        if (maxWidth < g_MapMsg_Widths[glyphIdx])
        {
            maxWidth = g_MapMsg_Widths[glyphIdx];
        }
    }

    posY = g_StringPosition_JP.vy;
    for (lineIdx = 0; lineIdx < g_MapMsg_WidthIdx; lineIdx++)
    {
        pos  = &g_StringPosition_JP;
        pos2 = &g_StringPosition_JP;

        if (result == MapMsgReturnCode_AlignCenter)
        {
            g_StringPosition_JP.vx = -g_MapMsg_Widths[lineIdx] * 6;
        }
        else
        {
            g_StringPosition_JP.vx = -maxWidth * 6;
        }

        // @hack `pos2` equals `pos` here - compiler will delete the store but keep the load, matching a dead load in the original.
        pos2->vx = pos->vx;

        switch (result)
        {
            case MapMsgReturnCode_AlignCenter:
                posX = g_StringPosition_JP.vx = -g_MapMsg_Widths[lineIdx] * 6;
                break;

            case MapMsgReturnCode_SetByT:
                posX = g_StringPositionX1_JP;
                break;

            default:
                posX = g_StringPosition_JP.vx = -maxWidth * 6;
                break;
        }

        for (glyphIdx = 0; glyphIdx < 21; )
        {
            charCode = *str;
            switch (charCode)
            {
                case '\t':
                case '\n':
                case ' ':
                    str++;
                    break;

                case MAP_MSG_CODE_MARKER:
                    code = *++str;
                    arg  = *++str - '0';

                    switch (code)
                    {
                        case MAP_MSG_CODE_NEWLINE:
                            posY     += 16;
                            glyphIdx  = 21;
                            break;

                        case MAP_MSG_CODE_JUMP:
                            decimalCount = 0;
                            hasDecimal   = 0;
                            value        = 0;
                            if (g_SysWork.mapMsgTimer == NO_VALUE)
                            {
                                str  += 2;
                                digit = (u8)*str;
                                g_MapMsg_AudioType = arg + 1;

                                while (digit != ')')
                                {
                                    if (digit == '.')
                                    {
                                        hasDecimal = 1;
                                    }
                                    else
                                    {
                                        if (hasDecimal != 0)
                                        {
                                            decimalCount++;
                                        }
                                        value *= 10;
                                        value -= '0' - digit;
                                    }
                                    str++;
                                    digit = (u8)*str;
                                }

                                timer = Q12(value);
                                for (j = 0; j < decimalCount; j++)
                                {
                                    timer /= 10;
                                }

                                g_SysWork.mapMsgTimer = timer;
                            }
                            else
                            {
                                while (arg != ' ' && arg != '\t')
                                {
                                    arg = *++str;
                                }
                            }
                            break;

                        case MAP_MSG_CODE_MIDDLE:
                            posX   = g_StringPosition_JP.vx = -g_MapMsg_Widths[lineIdx] * 6;
                            result = MapMsgReturnCode_AlignCenter;
                            break;

                        case MAP_MSG_CODE_TAB:
                            posX = g_StringPositionX1_JP;
                            result = MapMsgReturnCode_SetByT;
                            break;

                        case MAP_MSG_CODE_COLOR:
                            color      = STRING_COLORS_JP[arg];
                            g_StringColorId_JP = arg;
                            break;

                        case MAP_MSG_CODE_DISPLAY_ALL:
                            remaining = MAP_MESSAGE_DISPLAY_ALL_LENGTH;
                            break;

                        case MAP_MSG_CODE_END:
                            result   = NO_VALUE;
                            glyphIdx = 21;
                            lineIdx  = 9;
                            break;

                        case MAP_MSG_CODE_SELECT:
                            result   = arg;
                            glyphIdx = 21;
                            lineIdx  = 9;
                            break;

                        case 'W':
                            break;
                    }

                    str++;
                    break;

                case '\0':
                    result   = MapMsgReturnCode_Terminate;
                    glyphIdx = 21;
                    lineIdx  = 9;
                    break;

                default:
                    remaining--;

                    if (g_SysWork.enableHalfHeightGlyphs)
                    {
                        ot2  = &g_OrderingTable2[g_ActiveBufferIdx];
                        poly = (POLY_FT4*)GsOUT_PACKET_P;
                        setPolyFT4(poly);

                        setUV0AndClutSum(poly, glyphIdx * 12, (g_MapMsg_ActiveLine.positionIdx & 1) ? 0xE0 : 0x10, 0x7F93);
                        page = lineIdx & 0xF;
                        setUV1AndTPageSum(poly, glyphIdx * 12, (g_MapMsg_ActiveLine.positionIdx & 1) ? 0xF0 : 0x20,
                                          (g_MapMsg_ActiveLine.positionIdx & 1) ? (page | 0x10) : page);
                        setUV2Sum(poly, (glyphIdx + 1) * 12, (g_MapMsg_ActiveLine.positionIdx & 1) ? 0xE0 : 0x10);
                        setUV3Sum(poly, (glyphIdx + 1) * 12, (g_MapMsg_ActiveLine.positionIdx & 1) ? 0xF0 : 0x20);

                        setRGB0(poly, color, color >> 8, color >> 16);
                        setXY4(poly,
                               posX,      posY * 2,
                               posX,      (posY * 2) + 30,
                               posX + 12, posY * 2,
                               posX + 12, (posY * 2) + 30);
                        posX += 12;

                        addPrim(&ot2->org[10], poly);
                        GsOUT_PACKET_P = (PACKET*)poly + sizeof(POLY_FT4);
                    }
                    else
                    {
                        setWHFast((SPRT*)packet, FONT_12X16_GLYPH_SIZE_X, FONT_12X16_GLYPH_SIZE_Y);
                        addPrimFast(ot, (SPRT*)packet, 4);
                        setRGBC0Word((SPRT*)packet, color);
                        setXY0Fast((SPRT*)packet, posX, posY);

                        if (g_MapMsg_ActiveLine.positionIdx == 4)
                        {
                            setUV0AndClutSum((SPRT*)packet, glyphIdx * 12, (lineIdx / 5) ? 0xE0 : 0x10, 0x7F93);
                        }
                        else
                        {
                            setUV0AndClutSum((SPRT*)packet, glyphIdx * 12, (g_MapMsg_ActiveLine.positionIdx & 1) ? 0xE0 : 0x10, 0x7F93);
                        }
                        posX += 12;

                        packet += sizeof(SPRT);
                        if (g_MapMsg_ActiveLine.positionIdx == 4)
                        {
                            setDrawTPage((DR_TPAGE*)packet, 0, 1, ((lineIdx % 5) & 0xF) | ((lineIdx / 5) ? 0x10 : 0));
                        }
                        else
                        {
                            setDrawTPage((DR_TPAGE*)packet, 0, 1, (lineIdx & 0xF) | ((g_MapMsg_ActiveLine.positionIdx & 1) ? 0x10 : 0));
                        }
                        addPrim(ot, (DR_TPAGE*)packet);
                        packet += sizeof(DR_TPAGE);
                    }

                    str += 2;
                    glyphIdx++;

                    if (remaining <= 0)
                    {
                        if (!g_SysWork.enableHalfHeightGlyphs)
                        {
                            GsOUT_PACKET_P = packet;
                        }
                        return result;
                    }
            }
        }
    }

    if (!g_SysWork.enableHalfHeightGlyphs)
    {
        GsOUT_PACKET_P = packet;
    }

    return result;
}

void Gfx_StringDraw_JP(char* str, s32 lineIdx) // 0x8004BFC0
{
    GsOT_TAG* ot;
    GsOT*     ot2;
    PACKET*   packet;
    POLY_FT4* poly;
    SPRT*     sprt;
    u32       color;
    s32       i;
    s16       x;
    s16       y;
    s32       glyphY;
    s16       x1;
    s16       y0;
    s16       y1;
    DVECTOR*  pos;

    ot     = &g_OtTags0[g_ActiveBufferIdx][6];
    color  = STRING_COLORS_JP[g_StringColorId_JP];
    packet = NULL;

    if (!g_SysWork.enableHalfHeightGlyphs)
    {
        packet = GsOUT_PACKET_P;
    }

    pos = &g_StringPosition_JP;
    x   = pos->vx = -120;

    if (D_800C5E20 == 2)
    {
        y = (lineIdx * 16) - 12;
    }
    else
    {
        y = (lineIdx * 16) - 20;
    }

    pos->vy = y;
    glyphY  = y;

    for (i = 0; i < 21; )
    {
        switch (*str)
        {
            case '\t':
            case ' ':
                str++;
                break;

            case '\0':
                i = 21;
                break;

            default:
                if (g_SysWork.enableHalfHeightGlyphs)
                {
                    ot2  = &g_OrderingTable2[g_ActiveBufferIdx];
                    poly = (POLY_FT4*)GsOUT_PACKET_P;
                    setPolyFT4(poly);

                    setUV0AndClutSum(poly, i * 12, (lineIdx & 1) ? 0xE0 : 0x10, 0x7F93);
                    setUV1AndTPageSum(poly, i * 12, (lineIdx & 1) ? 0xF0 : 0x20,
                                      (lineIdx & 1) ? getTPage(0, 0, ((lineIdx >> 1) << 6) + 192, 256) :
                                                      getTPage(0, 0, ((lineIdx >> 1) << 6) + 192, 0));
                    setUV2Sum(poly, (i + 1) * 12, (lineIdx & 1) ? 0xE0 : 0x10);
                    setUV3Sum(poly, (i + 1) * 12, (lineIdx & 1) ? 0xF0 : 0x20);

                    setRGB0(poly, color, color >> 8, color >> 16);

                    y0 = glyphY * 2;
                    y1 = (glyphY + 15) * 2;
                    x1 = x + 12;
                    setXY4(poly, x, y0, x, y1, x1, y0, x1, y1);
                    x = x1;

                    addPrim(&ot2->org[10], poly);
                    GsOUT_PACKET_P = (u8*)poly + sizeof(POLY_FT4);
                }
                else
                {
                    sprt = (SPRT*)packet;
                    setWHFast(sprt, 12, 16);
                    addPrimFast(ot, sprt, 4);
                    setRGBC0Word(sprt, color);
                    setXY0Fast(sprt, x, glyphY);
                    setUV0AndClutSum(sprt, i * 12, (lineIdx & 1) ? 0xE0 : 0x10, 0x7F93);
                    x += 12;

                    packet += sizeof(SPRT);
                    setDrawTPage((DR_TPAGE*)packet, 0, 1,
                                 (lineIdx & 1) ? getTPage(0, 0, ((lineIdx >> 1) << 6) + 192, 256) :
                                                 getTPage(0, 0, ((lineIdx >> 1) << 6) + 192, 0));
                    addPrim(ot, (DR_TPAGE*)packet);
                    packet += sizeof(DR_TPAGE);
                }

                str += 2;
                i++;
                break;
        }
    }

    if (!g_SysWork.enableHalfHeightGlyphs)
    {
        GsOUT_PACKET_P = packet;
    }
}

void func_8004C394(char* str, s32 col, s32 row, s32 mode) // 0x8004C394
{
    PACKET*   packet;
    GsOT_TAG* ot;
    SPRT*     sprt;
    u32       color;
    s32       colorId;
    s32       code;
    s32       i;
    s32       x;
    s32       y;

    ot     = &g_OtTags0[g_ActiveBufferIdx][6];
    packet = GsOUT_PACKET_P;
    color  = STRING_COLORS_JP[g_StringColorId_JP];

    // @hack References to `mode` and `color` outside of the loop below are needed for match
    if (mode || color)
    {
        u8 x = -x;
    }
    
    if (col < 2)
    {
        if (col >= 0)
        {
            y = g_StringPosition_JP.vy = (row * 20) - 60;
            x = g_StringPosition_JP.vx = (col * 150) - 130;
        }
        else
        {
            y = g_StringPosition_JP.vy = 72 - ((col - 2) * 8);
            x = g_StringPosition_JP.vx = row * -6;
        }
    }
    else
    {
        y = g_StringPosition_JP.vy = 72 - ((col - 2) * 8);
        x = g_StringPosition_JP.vx = row * -6;
    }

    switch (mode)
    {
        case 9:
            y = g_StringPosition_JP.vy = (row * 20) - 60;
            x = g_StringPosition_JP.vx = (col * 150) - 112;
            break;

        case 0:
        case 2:
        case 3:
        case 4:
            y = g_StringPosition_JP.vy = -33;
            x = g_StringPosition_JP.vx = (col * 150) - 134;
            if (mode == 4)
            {
                x = g_StringPosition_JP.vx = (col * 150) - 128;
            }
            break;

        case 1:
            y = g_StringPosition_JP.vy = -33;
            x = g_StringPosition_JP.vx = (col * 150) - 134;
            break;

        case 5:
        case 6:
            y = g_StringPosition_JP.vy = -25;
            x = g_StringPosition_JP.vx = (col * 150) - 134;
            break;
    }
    
    i = 0;
    do
    {
        // Wrap to next line?
        if (i == 10 && mode >= 0 && mode < 7)
        {
            x = g_StringPosition_JP.vx;
            y = g_StringPosition_JP.vy += 18;
            if (mode == 3)
            {
                x = g_StringPosition_JP.vx -= 6;
            }
        }

        switch (*str)
        {
            case '~':
                str++;
                code    = *str++;
                colorId = *str - '0';
                if (code == 'C')
                {
                    color      = STRING_COLORS_JP[colorId];
                    g_StringColorId_JP = colorId;
                }
                str++;
                break;

            case '\0':
                i = 20;
                break;

            default:
                sprt = (SPRT*)packet;
                setWHFast(sprt, 12, 16);
                addPrimFast(ot, sprt, 4);
                setRGBC0Word(sprt, color);
                setXY0Fast(sprt, x, y);

                if (col < 2)
                {
                    setUV0AndClutSum(sprt, ((row & 1) * 120) + (i * 12), (col != 0) ? 0xE0 : 0x10, 0x7F93);
                }
                else
                {
                    setUV0AndClutSum(sprt, i * 12, 0x10, 0x7F93);
                }

                x += 12;
                packet += sizeof(SPRT);
                
                if (col < 2)
                {
                    setDrawTPage((DR_TPAGE*)packet, 0, 1, getTPage(0, 0, (row >> 1) << 6, (col != 0) ? 256 : 0));
                }
                else
                {
                    setDrawTPage((DR_TPAGE*)packet, 0, 1, getTPage(0, 0, 256, 0));
                }

                addPrim(ot, (DR_TPAGE*)packet);
                packet += sizeof(DR_TPAGE);

                str += 2;
                i++;
                break;
        }
    }
    while (i < 20);

    GsOUT_PACKET_P = packet;
}

void func_8004C7E4(void) // 0x8004C7E4
{
    const char* const msgs[3] = {
        "ここで使うことはできない　　　　　　　＿",
        "ここの地図は持っていない　　　　　　　　",
        "暗くて見ることができない　　　　　　　　"
    };

    D_800C3920 = 20;

    func_8004C918(msgs[0], 1, 1, 5);
    func_8004C918(msgs[1], 1, 1, 6);
    func_8004C918(msgs[2], 1, 1, 7);
}

void func_8004C870(void) // 0x8004C870
{
    D_800C3920 = 20;
    func_8004C918("セーブしますか？　はい　　　　　　いいえ", 1, 1, 5);
}

s32 func_8004C8AC(u8* arg0) // 0x8004C8AC
{
    return Krom2RawAdd2(arg0[1] | (arg0[0] << 8));
}

void func_8004C8D8(u16* arg0, s32* idx, s32 arg2) // 0x8004C8D8
{
    s16* ptr;

    // @hack Identical arms split basic blocks - first schedules `sw ra` first, second moves `arg0` to `a3`.
    if (arg2 == 0)
    {
        ptr = (s16*)0x801E1E80 + (*idx * 3);
    }
    else
    {
        ptr = (s16*)0x801E1E80 + (*idx * 3);
    }

    if (arg2 == 0)
    {
        func_80036E48(arg0, ptr);
    }
    else
    {
        func_80036E48(arg0, ptr);
    }
}

void func_8004C918(char* str, s32 idx, s32 arg2, s32 arg3) // 0x8004C918
{
    RECT  rect;
    s32   i;
    char* ptr;

    ptr = str;

    for (i = 0; i < D_800C3920; i++)
    {
        func_8004C8D8(func_8004C8AC(ptr), &i, arg2);
        ptr += 2;
    }

    switch (arg3)
    {
        case 13:
            setRECT(&rect, 158, 16, 30, 16);
            break;

        case 12:
            setRECT(&rect, 256, 480, 60, 16);
            break;

        case 7:
        case 8:
            setRECT(&rect, 192, 16, 60, 16);
            break;

        case 11:
            setRECT(&rect, 192, 480, 60, 16);
            break;

        case 0:
        case 6:
            setRECT(&rect, 256, 480, 60, 16);
            break;

        case 2:
        case 3:
        case 5:
            setRECT(&rect, 256, 16, 60, 16);
            break;

        case 9:
            setRECT(&rect, 158, 480, 30, 16);
            break;

        case 10:
            setRECT(&rect, ((idx % 5) << 5 & 0x3C0) + ((idx % 5) & 1) * 30, (idx / 5) != 0 ? 480 : 16, D_800C3920 * 3, 16);
            break;

        case 14:
            setRECT(&rect, (idx % 5) << 6, (idx / 5) != 0 ? 480 : 16, 60, 16);
            break;
    }

    // @hack Keeps `arg2` live to the end of the function.
    if (arg2 == 0)
    {
        u8 x = -x;
    }

    ClearImage(&rect, 0, 0, 0);
    LoadImage(&rect, (u32*)0x801E1E80);
    DrawSync(0);
}

#endif
