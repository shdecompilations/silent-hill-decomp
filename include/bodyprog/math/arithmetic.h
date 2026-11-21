#ifndef _BODYPROG_MATH_ARITHMETIC_H
#define _BODYPROG_MATH_ARITHMETIC_H

/** @brief Squares a value.
 *
 * @param x Value to be squared.
 * @return `x` squared.
 */
#define SQUARE(x) \
    ((x) * (x))

/** @brief Determines the smaller of two values.
 *
 * @param a First value.
 * @param b Second value.
 * @return `true` if `a` is smaller than `b`, `false` otherwise.
 */
#define MIN(a, b) \
    (((a) < (b)) ? (a) : (b))

/** @brief Computes the minimum of two values and subtracts a base offset.
 *
 * @param a First value.
 * @param b Second value.
 * @param base Base value to subtract.
 * @return Minimum of `a` and `b`, minus `base`.
 */
#define MIN_EXTENT_OFFSET(a, b, base) \
    (((a) < (b)) ? ((a) - (base)) : ((b) - (base)))

/** @brief Determines the larger of two values.
 *
 * @param a First value.
 * @param b Second value.
 * @return `true` if `a` is larger than `b`, `false` otherwise.
 */
#define MAX(a, b) \
    (((a) > (b)) ? (a) : (b))

/** @brief Clamps a value to the range `[min, max]`.
 *
 * @param x Value to clamp.
 * @param min Lower range.
 * @param max Upper range.
 * @return `x` clamped to the range `[min, max]`.
 */
#define CLAMP(x, min, max) \
    (((x) < (min)) ? (min) : (((x) > (max)) ? (max) : (x)))

/** @brief Clamps a value to be not less than a given lower bound.
 *
 * @param x Value to clamp.
 * @param min Lower bound.
 * @return `x` if `x >= min`, otherwise `min`.
 */
#define CLAMP_LOW(x, min) \
    (((x) < (min)) ? (min) : (x))

/** @brief Clamps a value to be not greater than a given upper bound.
 *
 * @param x Value to clamp.
 * @param max Upper bound.
 * @return `x` if `x <= max`, otherwise `max`.
 */
#define CLAMP_HIGH(x, max) \
    (((x) > (max)) ? (max) : (x))

/** @brief Clamps a value to the inclusive range `[min, max]`.
 *
 * Uses `CLAMP_HIGH`, then `CLAMP_LOW` to ensure the value
 * is between the specified bounds.
 *
 * @param x Value to clamp.
 * @param min Lower bound.
 * @param max Upper bound.
 * @return `x` clamped to the range `[min, max]`.
 */
#define CLAMP_RANGE(x, min, max) \
    CLAMP_LOW(CLAMP_HIGH(x, max), min)

/** @brief Clamps a value to the range `[min, max]` using `MIN` first.
 *
 * Takes the smaller value between `x` and `max`, then ensures the result
 * is not less than `min`.
 *
 * @param x Value to clamp.
 * @param min Lower bound.
 * @param max Upper bound.
 * @return `x` clamped to the range `[min, max]`.
 */
#define CLAMP_MIN_THEN_LOW(x, min, max) \
    CLAMP_LOW(MIN(x, max), min)

/** @brief Clamps a value to the range `[min, max]` using `CLAMP_LOW` first.
 *
 * Ensures `x` is not less than `min`, then uses `MIN` to restrict
 * the result to not exceed `max`.
 *
 * @param x Value to clamp.
 * @param min Lower bound.
 * @param max Upper bound.
 * @return `x` clamped to the range `[min, max]`.
 */
#define CLAMP_LOW_THEN_MIN(x, min, max) \
    MIN(CLAMP_LOW(x, min), max)

/** @brief Floors a value to the closest step.
 *
 * @param x Value to be floored.
 * @param step Flooring step.
 * @return `x` floored to the closest `step`.
 */
#define FLOOR_TO_STEP(x, step) \
    (((x) >= 0) ? ((x) / (step)) : (((x) - ((step) - 1)) / (step)))

/** @brief Computes the absolute value.
 *
 * @param x Value.
 * @return Absolute value of `x`.
 */
#define ABS(x) \
    (((x) < 0) ? -(x) : (x))

/** @brief Computes the absolute value of an `s32` by shifting.
 *
 * @param x Value.
 * @return Absolute value of `x`.
 */
#define ABS_32(x) \
    (((x) ^ ((x) >> 31)) - ((x) >> 31))

/** @brief Computes the absolute sum of two values.
 *
 * @param a First value.
 * @param b Second value.
 * @return Absolute sum of `a` and `b`.
 */
#define ABS_ADD(a, b) \
    (((a) >= 0) ? ((b) + (a)) : ((b) - (a)))

/** @brief Computes the absolute difference between two values.
 *
 * @param a First value.
 * @param b Second value.
 * @return Absolute difference between `a` and `b`.
 */
#define ABS_DIFF(a, b) \
    ABS((a) - (b))

/** @brief Checks if two values have different signs.
 *
 * @param a First value.
 * @param b Second value.
 * @return `true` if `a` and `b` have different signs, `false` otherwise.
 */
#define DIFF_SIGN(a, b) \
    (((a) >= 0 && (b) < 0) || ((a) < 0 && (b) >= 0))

/** @brief Divides an integer by another using a shift.
 *
 * @note `b` must be a power of 2.
 * Supported divisors: 2, 4, 8, 16, 32, 64, 128, 256.
 *
 * @param a Numerator.
 * @param b Denominator.
 * @return Result of `a` divided by `b`.
 */
#define DIV_FAST(a, b)   \
    ((a) >>              \
     (((b) == 2)   ? 1 : \
      ((b) == 4)   ? 2 : \
      ((b) == 8)   ? 3 : \
      ((b) == 16)  ? 4 : \
      ((b) == 32)  ? 5 : \
      ((b) == 64)  ? 6 : \
      ((b) == 128) ? 7 : \
      ((b) == 256) ? 8 : \
      0))

/** @brief Scales a large `x` before trigonometric multiplication.
 *
 * @note "Range-based scaling mechanism common in fixed-point DSP or min-level game engine math." - ChatGPT
 *
 * @param x Value to use for overflow computation.
 * @return 4 if `x` overflows, otherwise 0. */
#define OVERFLOW_GUARD(x) \
    (((u32)((x) + SHRT_MAX) >= USHRT_MAX) ? 4 : 0)

/** @brief Wraps an index to the range `[0, count - 1]`.
 *
 * @param idx Index to wrap.
 * @param count Number of entries.
 * @return `idx` wrapped to the range `[0, count - 1]`.
 */
#define IDX_WRAP(idx, count) \
    ((u32)(idx) % (count))

#endif
