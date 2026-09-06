#ifndef _BODYPROG_TEXT_TEXTDRAW_H
#define _BODYPROG_TEXT_TEXTDRAW_H

/** @note Likely the original name for this subsystem is `font` as indicated by SH2 and SH4 symbols. */

#define MAP_MSG_CODE_MARKER        '~' /** Message code start. */
#define MAP_MSG_CODE_COLOR         'C' /** Set color. */
#define MAP_MSG_CODE_DISPLAY_ALL   'D' /** Display message instantly with no rollout. */
#define MAP_MSG_CODE_END           'E' /** End message. */
#define MAP_MSG_CODE_HALF_HEIGHT   'H' /** Half-height glyphs. */
#define MAP_MSG_CODE_JUMP          'J' /** Jump timer. */
#define MAP_MSG_CODE_LINE_POSITION 'L' /** Set next line position. */
#define MAP_MSG_CODE_MIDDLE        'M' /** Align center. */
#define MAP_MSG_CODE_NEWLINE       'N' /** Newline. */
#define MAP_MSG_CODE_SELECT        'S' /** Display dialog prompt with selectable entries. */
#define MAP_MSG_CODE_TAB           'T' /** Inset line. */

#define FONT_12X16_GLYPH_COUNT        84
#define FONT_12X16_GLYPH_SIZE_X       12
#define FONT_12X16_GLYPH_SIZE_Y       16
#define FONT_12X16_SPACE_SIZE         6
#define FONT_12X16_LINE_COUNT_MAX     9
#define FONT_12X16_ATLAS_COLUMN_COUNT (FONT_12X16_GLYPH_COUNT / 4)

#define GLYPH_TABLE_ASCII_OFFSET '\'' /** Subtracted from ASCII bytes to get index to some string-related table. */
#define DEFAULT_TEXT_LAYER_IDX   6    /** Values < 6 would make text unaffected by the screen fade effect and are thus @unused. */

/** @brief String color IDs for strings displayed in screen space.
 * Used as indices into `STRING_COLORS`.
 */
typedef enum _StringColorId
{
    StringColorId_Gold        = 0,
    StringColorId_DarkGrey    = 1,
    StringColorId_Green       = 2,
    StringColorId_Nuclear     = 3,
    StringColorId_Red         = 4,
    StringColorId_GreenUnused = 5, // @unused Same as `StringColorId_Green`.
    StringColorId_LightGrey   = 6,
    StringColorId_White       = 7,

    StringColorId_Count       = 8
} e_StringColorId;

/** @brief Map message line data. */
typedef struct _MapMsgLine
{
    /* 0x0 */ s8 unused;
    /* 0x1 */ u8 positionIdx;
} s_MapMsgLine;

// ====================
// GLOBALS (BSS; Hack; text_draw.c)
// ====================
// To match the order of the BSS segment, extern declarations
// are required in a predetermined order.
// This is done until a way to replicate `common`
// segment behavior is found.

extern DVECTOR      g_StringPosition;
extern s32          g_StringPositionX1; // Copy of `g_StringPosition.vx` as `s32`. It's unclear what for.
extern s_MapMsgLine g_MapMsg_ActiveLine;
extern s8           __pad_bss_800C38B2[2];
extern s32          g_MapMsg_WidthIdx;
extern s32          __pad_bss_800C38B8[4];
extern s32          g_MapMsg_Widths[12];
extern GsSPRITE     g_MapMsg_GlyphSprite;
extern s16          g_GlyphSpritePositionX;
extern s16          __pad_bss_800C391E;
extern s32          D_800C3920; // Something for Japanese glyphs.
extern s32          __pad_bss_800C3924;

// ==========
// FUNCTIONS
// ==========

/** @brief Sets the global position of the next string to be drawn by `Gfx_StringDraw`.
 *
 * @param x X screen position.
 * @param y Y screen position.
 */
void Gfx_StringPositionSet(s32 x, s32 y);

/** @brief Set the global `g_StringLayerIdx`.
 *
 * @param layerIdx New layer index.
 */
void Gfx_StringLayerIdxSet(s32 layerIdx);

/** @brief Resets the global `g_StringLayerIdx` to `DEFAULT_TEXT_LAYER_IDX`. */
void Gfx_StringLayerIdxReset(void);

/** @brief Sets the global color state of the next string drawn by `Gfx_StringDraw`.
 *
 * @param colorId ID of the new color to set (`e_ColorId`).
 */
void Gfx_StringColorSet(s16 colorId);

/** @brief Draws a string in screen space using 12x16 glyphs. The position and color must be set by
 * `Gfx_StringPositionSet` and `Gfx_StringColorSet` before calling this function.
 *
 * @note References glyphs in `FONT16.TIM`. The texture is loaded into VRAM across multiple texture pages,
 * hence why the texture is a single row with 4-pixel padding every 21st glyph instead of a stacked arrangement.
 *
 * @param str String to draw.
 * @param displayLength Number of consecutive glyphs to draw from the string.
 */
bool Gfx_StringDraw(char* str, s32 displayLength);

/** @brief Computes the screen space widths of lines in a map message using 12x16 glyphs and populates
 * `g_MapMsg_Widths`.
 *
 * @param mapMsgIdx Index of the map message to evaluate.
 */
s32 Gfx_MapMsg_WidthsCompute(s32 mapMsgIdx);

/** @brief Draws a string in screen space using 12x16 glyphs and returns a map message code.
 *
 * @param mapMsg Map message to draw.
 * @param displayLength Number of consecutive glyphs to draw from the map message.
 * @return Map message return code (`e_MapMsgReturnCode`).
 */
s32 Gfx_MapMsg_StringDraw(char* mapMsg, s32 displayLength);

/** @brief @unused? Might be from JAP builds. */
void func_8004B658(void);

/** @brief Resets global map message parameters to defaults. */
void Gfx_MapMsg_Reset(void);

/** @brief @unused Sets the global glyph sprite position relative to the center of the screen.
 *
 * @param x Center-relative X screen position.
 * @param y Center-relative Y screen position.
 */
void Gfx_GlyphSprite_PositionSet(s16 x, s16 y);

/** @unused */
void func_8004B74C(s16 arg0);

/** @unused Draws string. */
void func_8004B76C(char* str, bool useFixedWidth);

/** @brief Draws an integer string in screen space using 12x16 glyphs.
 *
 * @param widthMin Minimum width of the integer string.
 * @param displayLength Number of consecutive glyphs to draw from the integer string.
 */
void Gfx_StringDrawInt(s32 widthMin, s32 displayLength);

#if VERSION_REGION_IS(NTSCJ)
    void func_8004B45C(s32 mapMsgBaseIdx, s32 arg1);
    void func_8004C8D8(u16*, s32*, s32);
    s32 func_8004C8AC(u8*);
#endif

// TODO: Move following funcs to item_screens_cam header.
#if VERSION_REGION_IS(NTSCJ)
    bool ItemScreen_TmdGsFCallInitTG3(void);
    void ItemScreen_TmdGsFCallInitG3G4(void);
    void ItemScreen_TmdGsFCallInitTG4(void);
#else
    void ItemScreen_TmdGsFCallInit(void);
#endif

#endif
