#ifndef _BODYPROG_MATH_FIXEDPOINT_H
#define _BODYPROG_MATH_FIXEDPOINT_H

#include "types.h"

#include "bodyprog/math/arithmetic.h"
#include "bodyprog/math/constants.h"

// =====================
// ARITHMETIC AND UTILS
// =====================

/** @brief Converts an integer to a fixed-point Q format.
 *
 * @note Deprecated.
 *
 * @param x `int` to convert.
 * @param shift Fixed-point shift.
 * @return `x` converted to fixed-point.
 */
#define FP_TO(x, shift) \
    ((x) << (shift))

/** @brief Converts a float to a fixed-point Q format.
 *
 * @param x `float` to convert.
 * @param shift Fixed-point shift.
 * @return `x` converted to fixed-point (`s32`).
 */
#define TO_FIXED(x, shift) \
    (s32)((x) * (1 << (shift)))

/** @brief Converts an integer from a fixed-point Q format.
 *
 * @param x `int` to convert.
 * @param shift Fixed-point shift.
 * @return `x` converted from fixed-point.
 */
#define FP_FROM(x, shift) \
    ((x) >> (shift))

/** @brief Converts an integer from a scaled fixed-point Q format rounded to the nearest value.
 *
 * @param x Fixed-point value to convert.
 * @param scale Fixed-point scale.
 * @param shift Fixed-point shift.
 * @return `x` rounded and converted from fixed-point.
 */
#define FP_ROUND_SCALED(x, scale, shift) \
    (((x) + ((TO_FIXED(1.0f, shift) * (scale)) - 1)) / (TO_FIXED(1.0f, shift) * (scale)))

/** @brief Converts an integer from a fixed-point Q format rounded toward 0.
 *
 * @param x Fixed-point value to convert.
 * @param shift Fixed-point shift.
 * @return `x` rounded toward 0 and converted from fixed-point.
 */
#define FP_ROUND_TO_ZERO(x, shift) \
    ((s32)(FP_FROM(x, shift) + ((u32)(x) >> 31)) >> 1)

/** @brief Floors a fixed-point value.
 *
 * @param x Fixed-point value to floor.
 * @param shift Fixed-point shift.
 * @return Fixed-point result of `x` floored to the closest integer.
 */
#define FP_FLOOR(x, shift) \
    TO_FIXED(FP_FROM(x, shift), shift)

/** @brief Truncates a value in a fixed-point Q format toward zero.
 *
 * @param x Fixed-point value to truncate.
 * @return Fixed-point result of `x` truncated toward zero.
 */
#define FP_TRUNCATE(x, shift) \
    TO_FIXED((x) / TO_FIXED(1.0f, shift), shift)

/** @brief Multiplies two integers in a fixed-point Q format.
 *
 * @param a First fixed-point factor.
 * @param b Second fixed-point factor.
 * @param shift Fixed-point shift.
 * @return Fixed-point product of `a` and `b`.
 */
#define FP_MULTIPLY(a, b, shift) \
    (((a) * (b)) >> (shift))

/** @brief Multiplies two integers in a fixed-point Q format, using 64-bit intermediates for higher precision.
 *
 * @param a First fixed-point factor.
 * @param b Second fixed-point factor.
 * @param shift Fixed-point shift.
 * @return Precise fixed-point product of `a` and `b`.
 */
#define FP_MULTIPLY_PRECISE(a, b, shift) \
    ((s32)(((s64)(a) * (s64)(b)) >> (shift)))

/** @brief Multiplies an integer in a fixed-point Q format by a float converted to a fixed-point Q format.
 *
 * @param a First fixed-point factor.
 * @param b Second floating-point factor.
 * @param shift Fixed-point shift.
 * @return Fixed-point product of `a` and `b`.
 */
#define FP_MULTIPLY_FLOAT(a, b, shift) \
    FP_MULTIPLY(a, TO_FIXED(b, shift), shift)

/** @brief Multiplies an integer in a fixed-point Q format by a float converted to fixed-point Q format,
 * using a 64-bit intermediates for higher precision.
 *
 * @param a First fixed-point factor.
 * @param b Second floating-point factor.
 * @param shift Fixed-point shift.
 * @return Precise product of `a` and `b` converted from fixed-point.
 */
#define FP_MULTIPLY_FLOAT_PRECISE(a, b, shift) \
    FP_MULTIPLY((s64)(a), (s64)TO_FIXED(b, shift), shift)

/** @brief Divides an integer in a fixed-point Q format by another.
 *
 * @param a Fixed-point numerator.
 * @param b Fixed-point denominator.
 * @return Fixed-point result of `a` divided by `b`.
 */
#define FP_DIVIDE(a, b, shift) \
    (((a) << (shift)) / (b))

/** @brief Squares a fixed-point value.
 *
 * @param x Fixed-point value to be squared.
 * @param shift Fixed-point shift.
 * @return Fixed-point square of `x`.
 */
#define FP_SQUARE(x, shift) \
    FP_MULTIPLY(x, x, shift)

/** @brief Squares a fixed-point value, using 64-bit intermediates for higher precision.
 *
 * @param x Fixed-point value to be squared.
 * @param shift Fixed-point shift.
 * @return Fixed-point square of `x`.
 */
#define FP_SQUARE_PRECISE(x, shift) \
    FP_MULTIPLY_PRECISE(x, x, shift)

/** @brief Floors a Q*.12 fixed-point value.
 *
 * @param x Q*.12 fixed-point value to floor.
 * @return Q*.12 result of `x` floored to the closest integer.
 */
#define Q12_FLOOR(x) \
    FP_FLOOR(x, Q12_SHIFT)

/** @brief Truncates a Q*.12 fixed-point value toward zero.
 *
 * @param x Q*.12 fixed-point value to truncate.
 * @return Q*.12 result of `x` truncated toward zero.
 */
#define Q12_TRUNC(x) \
    FP_TRUNCATE(x, Q12_SHIFT)

/** @brief Multiplies two integers in Q*.7 fixed-point.
 *
 * @param a First Q*.7 fixed-point factor.
 * @param b Second Q*.7 fixed-point factor.
 * @return Q*.7 product of `a` and `b`.
 */
#define Q7_MULT(a, b) \
    FP_MULTIPLY(a, b, Q7_SHIFT)

/** @brief Multiplies two integers in Q*.12 fixed-point.
 *
 * @param a First Q*.12 fixed-point factor.
 * @param b Second Q*.12 fixed-point factor.
 * @return Q*.12 product of `a` and `b`.
 */
#define Q12_MULT(a, b) \
    FP_MULTIPLY(a, b, Q12_SHIFT)

/** @brief Multiplies two integers in Q*.12 fixed-point.
 * Alternative to `Q12_MULT` using division instead of a bitwise shift.
 *
 * @param a First Q*.12 fixed-point factor.
 * @param b Second Q*.12 fixed-point factor.
 * @return Q*.12 product of `a` and `b`.
 */
#define Q12_MULT_ALT(a, b) \
    (((a) * (b)) / Q12(1.0f))

/** @brief Multiplies two integers in Q*.12 fixed-point, using 64-bit intermediates for higher precision.
 *
 * @param a First Q*.12 fixed-point factor.
 * @param b Second Q*.12 fixed-point factor.
 * @return Precise Q*.12 product of `a` and `b`.
 */
#define Q12_MULT_PRECISE(a, b) \
    FP_MULTIPLY_PRECISE(a, b, Q12_SHIFT)

/** @brief Multiplies an integer in Q*.12 fixed-point by a float converted to Q*.12 fixed-point.
 *
 * @param a First Q*.12 fixed-point factor.
 * @param b Second floating-point factor.
 * @return Q*.12 product of `a` and `b`.
 */
#define Q12_MULT_FLOAT(a, b) \
    FP_MULTIPLY_FLOAT(a, b, Q12_SHIFT)

/** @brief Multiplies an integer in Q*.12 fixed-point by a float converted to Q*.12 fixed-point,
 * using 64-bit intermediates for higher precision.
 *
 * @param a First Q*.12 fixed-point factor.
 * @param b Second floating-point factor.
 * @return Precise Q*.12 product of `a` and `b`.
 */
#define Q12_MULT_FLOAT_PRECISE(a, b) \
    FP_MULTIPLY_FLOAT_PRECISE(a, b, Q12_SHIFT)

/** @brief Divides an integer in Q*.12 fixed-point by another.
 *
 * @param a Q*.12 fixed-point numerator.
 * @param b Q*.12 fixed-point denominator.
 * @return Q*.12 result of `a` divided by `b`.
 */
#define Q12_DIV(a, b) \
    FP_DIVIDE(a, b, Q12_SHIFT)

/** @brief Squares a Q*.12 value.
 *
 * @param x Q*.12 value to be squared.
 * @return Q*.12 square of `x`.
 */
#define Q12_SQUARE(x) \
    FP_SQUARE(x, Q12_SHIFT)

/** @brief Squares a fixed-point value, using 64-bit intermediates for higher precision.
 *
 * @param x Q*.12 value to be squared.
 * @return Q*.12 square of `x`.
 */
#define Q12_SQUARE_PRECISE(x) \
    FP_SQUARE_PRECISE(x, Q12_SHIFT)

/** @brief Computes the square 2D distance between two positions in Q19.12 fixed-point,
 * using Q21.8 fixed-point intermediates to avoid overflow.
 *
 * @param from First Q19.12 position.
 * @param to Second Q19.12 position.
 * @param return 2D Q19.12 distance between two positions.
 */
#define Q12_2D_DISTANCE_SQR(from, to)         \
    (SQUARE(Q12_TO_Q8((to).vx - (from).vx)) + \
     SQUARE(Q12_TO_Q8((to).vz - (from).vz)))

// ==================================
// RAW Q FORMAT CONVERSION AND UTILS
// ==================================

/** @brief Converts a floating-point value to Q*.4 fixed-point.
 *
 * @param x Value to convert (`float`).
 * @return `x` converted to Q*.4 fixed-point.
 */
#define Q4(x) \
    TO_FIXED(x, Q4_SHIFT)

/** @brief Converts a floating-point value to Q*.6 fixed-point.
 *
 * @param x Value to convert (`float`).
 * @return `x` converted to Q*.6 fixed-point.
 */
#define Q6(x) \
    TO_FIXED(x, Q6_SHIFT)

/** @brief Converts a floating-point value to Q*.7 fixed-point.
 *
 * @param x Value to convert (`float`).
 * @return `x` converted to Q*.7 fixed-point.
 */
#define Q7(x) \
    TO_FIXED(x, Q7_SHIFT)

/** @brief Converts a floating-point value to clamped Q*.7 fixed-point.
 *
 * @param x Value to convert (`float`).
 * @return `x` converted to clamped Q*.7 fixed-point.
 */
#define Q7_CLAMPED(x) \
    CLAMP(Q7(x), Q7(0.0f), Q7(1.0f) - 1)

/** @brief Converts a floating-point value to Q*.8 fixed-point.
 *
 * @param x Value to convert (`float`).
 * @return `x` converted to Q*.8 fixed-point.
 */
#define Q8(x) \
    TO_FIXED(x, Q8_SHIFT)

/** @brief Converts a floating-point value to clamped Q*.8 fixed-point.
 *
 * @param x Value to convert (`float`).
 * @return `x` converted to clamped Q*.8 fixed-point.
 */
#define Q8_CLAMPED(x) \
    CLAMP(Q8(x), Q8(0.0f), Q8(1.0f) - 1)

/** @brief Converts a floating-point value to Q*.12 fixed-point.
 *
 * @param x Value to convert (`float`).
 * @return `x` converted to Q*.12 fixed-point.
 */
#define Q12(x) \
    TO_FIXED(x, Q12_SHIFT)

/** @brief Converts a floating-point value to clamped Q*.12 fixed-point.
 *
 * @param x Value to convert (`float`).
 * @return `x` converted to clamped Q*.12 fixed-point.
 */
#define Q12_CLAMPED(x) \
    CLAMP(Q12(x), Q12(0.0f), Q12(1.0f) - 1)

/** @brief Converts a fixed-point value from Q*.4 to Q*.8.
 *
 * @param x Q*.4 fixed-point value to convert.
 * @return `x` converted to Q*.8 fixed-point.
 */
#define Q4_TO_Q8(x) \
    ((x) << (Q8_SHIFT - Q4_SHIFT))

/** @brief Converts a fixed-point value from Q*.4 to Q*.12.
 *
 * @param x Q*.4 fixed-point value to convert.
 * @return `x` converted to Q*.12 fixed-point.
 */
#define Q4_TO_Q12(x) \
    ((x) << (Q12_SHIFT - Q4_SHIFT))

/** @brief Converts a fixed-point value from Q*.6 to Q*.12.
 *
 * @param x Q*.6 fixed-point value to convert.
 * @return `x` converted to Q*.12 fixed-point.
 */
#define Q6_TO_Q12(x) \
    ((x) << (Q12_SHIFT - Q6_SHIFT))

/** @brief Converts a fixed-point value from Q*.8 to Q*.12.
 *
 * @param x Q*.8 fixed-point value to convert.
 * @return `x` converted to Q*.12 fixed-point.
 */
#define Q8_TO_Q12(x) \
    ((x) << (Q12_SHIFT - Q8_SHIFT))

/** @brief Converts a fixed-point value from Q*.8 to Q*.4.
 *
 * @param x Q*.8 fixed-point value to convert.
 * @return `x` converted to Q*.4 fixed-point.
 */
#define Q8_TO_Q4(x) \
    ((x) >> (Q8_SHIFT - Q4_SHIFT))

/** @brief Converts a fixed-point value from Q*.10 to Q*.12.
 *
 * @param x Q*.10 fixed-point value to convert.
 * @return `x` converted to Q*.12 fixed-point.
 */
#define Q10_TO_Q12(x) \
    ((x) << (Q12_SHIFT - Q10_SHIFT))

/** @brief Converts a fixed-point value from Q*.12 to Q*.4.
 *
 * @param x Q*.12 fixed-point value to convert.
 * @return `x` converted to Q*.4 fixed-point.
 */
#define Q12_TO_Q4(x) \
    ((x) >> (Q12_SHIFT - Q4_SHIFT))

/** @brief Converts a fixed-point value from Q*.12 to Q*.6.
 *
 * @param x Q*.12 fixed-point value to convert.
 * @return `x` converted to Q*.6 fixed-point.
 */
#define Q12_TO_Q6(x) \
    ((x) >> (Q12_SHIFT - Q6_SHIFT))

/** @brief Converts a fixed-point value from Q*.12 to Q*.8.
 *
 * @param x Q*.12 fixed-point value to convert.
 * @return `x` converted to Q*.8 fixed-point.
 */
#define Q12_TO_Q8(x) \
    ((x) >> (Q12_SHIFT - Q8_SHIFT))

/** @brief Converts a fixed-point value from Q*.12 to Q*.10.
 *
 * @param x Q*.12 fixed-point value to convert.
 * @return `x` converted to Q*.10 fixed-point.
 */
#define Q12_TO_Q10(x) \
    ((x) >> (Q12_SHIFT - Q10_SHIFT))

/** @brief Extracts the fractional part of a value in Q*.12 fixed-point.
 *
 * @param x Q*.12 fixed-point value.
 * @return Fractional part of `x` in Q*.12 fixed-point.
 */
#define Q12_FRACT(x) \
    ((x) & 0xFFF)

/** @brief Extracts the fractional part of a value in Q*.8 fixed-point.
 *
 * @param x Q*.8 fixed-point value.
 * @return Fractional part of `x` in Q*.8 fixed-point.
 */
#define Q8_FRACT(x) \
    ((x) & 0xFF)

// =======================================
// ABSTRACT Q FORMAT CONVERSION AND UTILS
// =======================================

/** @brief Converts a normalized floating-point color component in the range `[0.0f, 1.0f]` to Q0.8 fixed-point,
 * integer range `[0, 255]`.
 *
 * TODO: Deprecated, don't use. Doesn't make sense to have `float` color components in this project.
 *
 * @param comp Color component (`float`).
 * @return Q0.8 fixed-point color component, clamped integer range `[0, 255]` (`q0_8`).
 */
#define Q8_COLOR(comp) \
    (q0_8)Q8_CLAMPED(comp)

/** @brief Converts floating-point degrees to signed Q3.12 fixed-point, full rotation integer range `[0, 4096]`.
 *
 * This angle format is used in world-space.
 *
 * @note 1 degree = 11.377778 units.
 *
 * @param deg Degrees (`float`).
 * @return Unsigned Q3.12 fixed-point angle, full rotation integer range `[0, 4096]` (`q3_12`).
 */
#define Q12_ANGLE(deg) \
    (q3_12)Q12((deg) / 360.0f)

/** @brief Converts floating-point degrees to unsigned Q0.8 fixed-point, clamped full rotation integer range `[0, 255]`.
 *
 * This angle format is used in map data.
 *
 * @note 1 degree = 0.711111 units.
 *
 * @param deg Angle in degrees (`float`).
 * @return Unsigned Q0.8 fixed-point angle, clamped full rotation integer range `[0, 255]` (`q0_8`).
 */
#define Q8_ANGLE(deg) \
    (q0_8)Q8_CLAMPED((deg) / 360.0f)

/** @brief Converts an unsigned Q0.8 fixed-point angle, full rotation integer range `[0, 255]` to
 * unsigned Q3.12 fixed-point, full rotation integer range `[0, 4096]`.
 *
 * @param angle Unsigned Q0.8 fixed-point angle, full rotation integer range `[0, 255]`.
 * @return Unsigned Q3.12 fixed-point angle, full rotation integer range `[0, 4096]` (`q3_12`).
 */
#define Q12_ANGLE_FROM_Q8(angle) \
    (q3_12)Q8_TO_Q12(angle)

/** @brief Normalizes a signed Q3.12 fixed-point angle to the clamped unsigned integer range `[0, 4095]`.
 *
 * @note Has the same effect as `Q12_ANGLE_NORM_U`. Could they somehow be combined?
 *
 * @param angle Signed Q3.12 fixed-point angle, full rotation integer range `[-2048, 2047]`.
 * @return Unsigned Q3.12 fixed-point angle, wrapped to the clamped integer range `[0, 4095]` (`q3_12`).
 */
#define Q12_ANGLE_ABS(angle) \
    Q12_FRACT((angle) + Q12_ANGLE(360.0f))

/** @brief Normalizes an unsigned Q3.12 fixed-point angle to the clamped signed integer range `[-2048, 2047]`.
 *
 * @param angle Unsigned Q3.12 fixed-point angle, full rotation integer range `[0, 4095]`.
 * @return Signed Q3.12 fixed-point angle wrapped to the clamped integer range `[-2048, 2047]` (`q3_12`).
 */
#define Q12_ANGLE_NORM_S(angle) \
    (((angle) << 20) >> 20)

/** @brief Normalizes a signed Q3.12 fixed-point angle to the clamped unsigned range `[0, 4095]`.
 *
 * @param angle Signed Q3.12 fixed-point angle, full rotation integer range `[-2048, 2047]`.
 * @return Unsigned Q3.12 fixed-point angle, wrapped to the clamped integer range `[0, 4095]` (`q3_12`).
 */
#define Q12_ANGLE_NORM_U(angle) \
    ((angle) & (Q12_ANGLE(360.0f) - 1))

/** @brief Converts floating-point radians in the range `[-PI, PI]` to the fixed-point full rotation,
 * integer range `[0, 20480]`.
 *
 * This angle format is only used in `vcSetDataToVwSystem`.
 *
 * @note π = 10240 units.
 *
 * @param rad Angle in radians (`float`).
 * @return Fixed-point radian representation, full rotation integer range `[0, 20480]` (`s32`).
 */
#define FP_RADIAN(rad)                                                                \
    (s32)(((((rad) < 0.0f) ? (PI + (PI - ABS(rad))) : (rad)) * ((float)FP_PI / PI)) * \
          (((rad) < 0.0f || (rad) >= PI) ? 1.0f : 2.0f))

#endif
