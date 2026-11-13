#ifndef _BODYPROG_TEXT_TEXTDEBUGDRAW_H
#define _BODYPROG_TEXT_TEXTDEBUGDRAW_H

/** @brief Sets the position at which text will be drawn.
 *
 * @param x X text screen position.
 * @param y Y text screen position.
 */
void Text_Debug_PositionSet(s16 x, s16 y);

/** @brief Draws debug strings using 8x8 glyphs from `FONT8.TIM`.
 *
 * @note Valid `char` range: `[42, 95]` (`*` to `_`).
 *
 * @param str Text string to drawn.
 */
void Text_Debug_Draw(char* str);

/** @brief Converts an integer to a text string, written backwards into scratchpad memory.
 *
 * Handles negative values. Pads with `'\v'` (blank character in debug font) on the left to meet the minimum length.
 *
 * @param lengthMin Minimum length.
 * @param val Integer value to convert.
 * @return Formatted text string in scratchpad memory.
 */
char* Text_Debug_IntToString(s32 lengthMin, s32 val);

#if VERSION_EQUAL_OR_OLDER(PROTO_981216)
/** @brief Draws a Q19.12 fixed-point value to the screen with a specified number of decimal places.
 *
 * @note Converts the fixed-point value to an integer with extra precision, then passes it to `Text_Debug_DrawDecimal`
 * with rounding enabled.
 *
 * @param offsetX Character offset for the integer portion. Adjusted internally for the decimal point and fractional digits.
 * @param fracDigits Number of digits to display after the decimal point.
 * @param val Q19.12 fixed-point value to draw.
 */
void Text_Debug_DrawQ12(s32 offsetX, s32 fracDigits, q19_12 val);
#endif

/** @brief Formats an integer as a debug text string with optional decimal point and rounding.
 *
 * @note Writes the formatted string backwards into scratchpad memory and adjusts `g_Text_Debug_PositionSet1` cursor
 * position. Handles negative values.
 *
 * @param fieldWidth Total character width for the formatted number.
 * @param fracDigits Number of digits after the decimal point. 0 for no decimal point.
 * @param val Integer value to format. Must be converted from Q19.12 before calling.
 * @param round If `true`, rounds the value by `(value + 5) / 10` before formatting.
 */
void Text_Debug_DrawDecimal(s32 fieldWidth, s32 fracDigits, s32 val, bool round);

#endif
