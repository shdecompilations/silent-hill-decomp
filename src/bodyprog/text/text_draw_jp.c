#include <psyq/libapi.h>

#include "game.h"

#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "bodyprog/screen/screen_data.h"
#include "bodyprog/screen/screen_draw.h"
#include "bodyprog/text/text_draw.h"

// TODO: JPN releases seem to use much different text draw functions.
// Some functions from USA are still included, but in a different order to USA.
// Unsure if these can be merged into same .c, might need to be kept seperate.

extern s_MapMsgLine g_MapMsg_ActiveLine; // 0x800C5E18 in JPN.

DVECTOR    g_StringPosition;
s32        g_StringPositionX1;
static s16 g_StringColorId = StringColorId_White; /** `e_ColorId` */
// 2 bytes of padding.
static s32 g_StringLayerIdx = DEFAULT_TEXT_LAYER_IDX;

// TODO: Unsure if these correspond to variables in USA.
extern s16 D_800AF83C; // Set by `Gfx_StringColorSet_JP`
extern s16 D_800C5DEC;
extern s16 D_800C5DEE;
extern s16 g_GlyphSpritePositionX; // 0x800C5E0C;
extern s16 D_800C391E; // 0x800C5E0E;
extern DVECTOR D_800C5E10;
extern s32 D_800C5E14;
extern s32 D_800C5E1C;

extern s32 D_800C5E30[];
extern s32 D_800C5E20;

const u32 __pad_rodata_80025D54 = 0;

/** @brief Glyph widths for the 12x16 font. Used for kerning. */
static const u8 FONT_12X16_GLYPH_WIDTHS[FONT_12X16_GLYPH_COUNT] = {
    3,  7,  7,  11, 11, 4,  10, 4,  6,  10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 4,  4,
    10, 11, 10, 8,  13, 12, 12, 12, 13, 11, 11, 13, 12, 9,  9,  12, 12, 13, 12, 13, 11,
    13, 12, 10, 11, 13, 12, 12, 12, 11, 12, 6,  4,  6,  8,  0,  3,  9,  10,  9, 9,  9,
    7,  11, 11, 6,  6,  10, 6,  13, 11, 10, 11, 10, 8,  8,  7,  10, 10, 12, 10, 10, 9
};

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

const u32 __pad_rodata_80025DCC[2] = { 0, 0 };

void Gfx_StringPositionSet(s32 x, s32 y) // 0x8004A5B0
{
    #define OFFSET_X (SCREEN_WIDTH / 2)
    #define OFFSET_Y 112

    if (x != NO_VALUE)
    {
        g_StringPosition.vx = x - OFFSET_X;
        g_StringPositionX1  = (s16)(x - OFFSET_X);
    }

    if (y != NO_VALUE)
    {
        g_StringPosition.vy = y - OFFSET_Y;
    }

    g_StringLayerIdx = DEFAULT_TEXT_LAYER_IDX;

    #undef OFFSET_X
    #undef OFFSET_Y
}

void Gfx_StringLayerIdxSet(s32 idx) // 0x8004A5F4
{
    g_StringLayerIdx = idx;
}

void Gfx_StringLayerIdxReset(void) // 0x8004A600
{
    g_StringLayerIdx = DEFAULT_TEXT_LAYER_IDX;
}

void Gfx_StringColorSet(s16 colorId) // 0x8004A610
{
    g_StringColorId = colorId;
}

bool Gfx_StringDraw(char* str, s32 strLength) // 0x8004A61C
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
    sizeCpy = strLength;

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

// TODO: Might match USA `func_8004B658` with added func call?
// USA func doesn't come after Gfx_StringDraw though.
void func_8004AA28(void) // 0x8004AA28
{
    g_MapMsg_GlyphSprite.attribute = 64;
    g_MapMsg_GlyphSprite.cx        = 304;
    g_MapMsg_GlyphSprite.v         = 240;
    g_MapMsg_GlyphSprite.h         = 16;

    func_8003652C();
}

// TODO: Matches USA `Gfx_GlyphSprite_PositionSet`, rename symbols to match.
void func_8004AA6C(s16 x, s16 y) // 0x8004AA6C
{
    if (x != NO_VALUE)
    {
        D_800C5DEC = x + (-g_GameWork.gsScreenWidth / 2);
        g_GlyphSpritePositionX = D_800C5DEC;
    }

    if (y != NO_VALUE)
    {
        D_800C5DEE = y + (-g_GameWork.gsScreenHeight / 2);
    }
}

// TODO: Matches USA `func_8004B74C`, rename symbols to match.
void func_8004AAE4(s16 arg0) // 0x8004AAE4
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

void func_8004B76C(char* str, bool useFixedWidth) // 0x8004AB04
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

void Gfx_StringDrawInt(s32 widthMin, s32 val) // 0x8004AD90
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
        *str          = '-';
        g_MapMsg_GlyphSprite.x -= GLYPH_SIZE_X;
    }

    // Draw numeric string.
    Gfx_StringDraw(str, 5);
    return;

    #undef GLYPH_SIZE_X
    #undef ATLAS_COLUMN_COUNT
}

void Gfx_MapMsg_Reset(void) // 0x8004AEA8
{
    g_MapMsg_ActiveLine.unused = 0;
    g_MapMsg_ActiveLine.positionIdx = 1;
    D_800C5E1C = 1;
    D_800C5E10.vx = -0x78;
    D_800C5E10.vy = 0x4C;
    D_800C5E14 = -0x78;
    D_800AF83C = StringColorId_White;
    g_SysWork.enableHalfHeightGlyphs = 0;
}

void func_8004AF14(s32 x, s32 y) // 0x8004AF14
{
    if (x != -1)
    {
        D_800C5E10.vx = x - (SCREEN_WIDTH / 2);
        D_800C5E14 = D_800C5E10.vx;
    }
    if (y != -1)
    {
        D_800C5E10.vy = y - (FRAMEBUFFER_HEIGHT_PROGRESSIVE / 2);
    }
}

void Gfx_StringColorSet_JP(s16 colorId) // 0x8004A8DC
{
    D_800AF83C = colorId;
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

    ret                     = 0;
    D_800C5E1C              = 1;
    g_MapMsg_AudioLoadBlock = 0;

    for (i = 0; i < FONT_12X16_LINE_COUNT_MAX; i++)
    {
        D_800C5E30[i] = 0;
    }

    mapMsg = g_MapOverlayHdr.mapMessages[mapMsgIdx];

    for (j = 0; j < FONT_12X16_LINE_COUNT_MAX;)
    {
        for (i = 0; i < 21;)
        {
            charCode = *mapMsg;

            switch (charCode)
            {
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
                        D_800C5E30[D_800C5E1C - 1] = i;
                        i                          = 21;
                        D_800C5E1C++;
                    }
                    else if (msgCode == MAP_MSG_CODE_JUMP)
                    {
                        if (posIdx == 2)
                        {
                            g_MapMsg_AudioLoadBlock = 3;
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
                    D_800C5E30[D_800C5E1C - 1] = i;
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
                        D_800C3920 = D_800C5E30[j];
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

INCLUDE_ASM("bodyprog/nonmatchings/text/text_draw_jp", Gfx_MapMsg_StringDraw);

INCLUDE_ASM("bodyprog/nonmatchings/text/text_draw_jp", Gfx_StringDraw_JP);

INCLUDE_ASM("bodyprog/nonmatchings/text/text_draw_jp", func_8004C394);

void func_8004C7E4(void) // 0x8004C7E4
{
    // TODO: .rodata? `u8` used as placeholder, likely some kind of struct.
    extern u8      D_80025EB4;
    extern u8      D_80025EE0;
    extern u8      D_80025F0C;
    extern VECTOR3 D_80025F38;

    VECTOR3 unused = D_80025F38;

    D_800C3920 = 20;

    func_8004C918(&D_80025EB4, 1, 1, 5);
    func_8004C918(&D_80025EE0, 1, 1, 6);
    func_8004C918(&D_80025F0C, 1, 1, 7);
}

void func_8004C870(void)  // 0x8004C870
{
    extern u8 D_80025F44; // TODO: .rodata? `u8` used as placeholder, likely some kind of struct.

    D_800C3920 = 20;
    func_8004C918(&D_80025F44, 1, 1, 5);
}

s32 func_8004C8AC(u8* arg0) // 0x8004C8AC
{
    return Krom2RawAdd2(arg0[1] | (arg0[0] << 8));
}

INCLUDE_ASM("bodyprog/nonmatchings/text/text_draw_jp", func_8004C8D8);

INCLUDE_RODATA("bodyprog/nonmatchings/text/text_draw_jp", D_80025EB4);

INCLUDE_RODATA("bodyprog/nonmatchings/text/text_draw_jp", D_80025EE0);

INCLUDE_RODATA("bodyprog/nonmatchings/text/text_draw_jp", D_80025F0C);

INCLUDE_RODATA("bodyprog/nonmatchings/text/text_draw_jp", D_80025F38);

INCLUDE_RODATA("bodyprog/nonmatchings/text/text_draw_jp", D_80025F44);

INCLUDE_ASM("bodyprog/nonmatchings/text/text_draw_jp", func_8004C918);
