#ifndef _BODYPROG_MATH_MATH_H
#define _BODYPROG_MATH_MATH_H

#include <limits.h>

#include "bodyprog/math/arithmetic.h"
#include "bodyprog/math/constants.h"
#include "bodyprog/math/fixed_point.h"

#define SVECTOR3_Zero (SVECTOR3){ 0, 0, 0 }
#define SVECTOR_Zero  (SVECTOR){ 0, 0, 0, 0}
#define VECTOR_Zero   (VECTOR){ 0, 0, 0, 0 }

/** @brief Constructs a `VECTOR3` in a fixed-point Q19.12 format.
 *
 * @param x X component (`float`).
 * @param y Y component (`float`).
 * @param z Z component (`float`).
 * @return `VECTOR3` in a fixed-point Q19.12 format.
 */
#define VECTOR3(x, y, z) \
    { Q12(x), Q12(y), Q12(z) }

/** @brief Constructs a `VECTOR3` in a fixed-point Q19.12 format.
 *
 * @param x X component (`float`).
 * @param y Y component (`float`).
 * @param z Z component (`float`).
 * @return `VECTOR3` in a fixed-point Q19.12 format.
 */
#define Q12_VECTOR3(x, y, z) \
    (VECTOR3)VECTOR3(x, y, z)

/** @brief Constructs a `SVECTOR` in a fixed-point Q3.12 format.
 *
 * @param x X component (`float`).
 * @param y Y component (`float`).
 * @param z Z component (`float`).
 * @return `SVECTOR` in a fixed-point Q3.12 format.
 */
#define SVECTOR(x, y, z) \
    { Q12_ANGLE(x), Q12_ANGLE(y), Q12_ANGLE(z) }

/** @brief Packs a 16-bit XZ vector.
 *
 * @param x X component.
 * @param z Z component.
 * @return Packed XZ vector.
 */
#define PACKED_XZ16(x, z) \
    (((x) & 0xFFFF) + ((z) << 16))

/** @brief Packs an RGB color.
 *
 * @param r Red component.
 * @param g Green component.
 * @param b Blue component.
 * @return Packed RGB color.
 */
#define COLOR_RGB(r, g, b) \
    (((r) & 0xFF) | ((g) & 0xFF) << 8 | ((b) & 0xFF) << 16)

/** @brief Packs an RGB + code color.
 *
 * @param r Red component.
 * @param g Green component.
 * @param b Blue component.
 * @param code Code component.
 * @return Packed RGB + code color.
 */
#define COLOR_RGBC(r, g, b, code) \
    (((r) & 0xFF) | (((g) & 0xFF) << 8) | (((b) & 0xFF) << 16) | ((code) << 24))

/** @brief Checks if a flag in a flags bitfield is set to a required status.
 *
 * @param flags Flags bitfield to check.
 * @param flag Flag to check.
 * @param isSet Required status to check for.
 * @return `true` if the flag is set to the required status, `false` otherwise.
 */
#define CHECK_FLAG(bitfield, flag, isSet) \
    (!((bitfield) & (flag)) == !(isSet))

/** @brief Checks if a flag in a flags bitfield is set to a required status.
 *
 * @param flags Flags bitfield to check.
 * @param flagIdx Index of the flag to check.
 * @param isSet Required status to check for.
 * @return `true` if the flag is set to the required status, `false` otherwise.
 */
#define CHECK_FLAG_IDX(flags, flagIdx, isSet) \
    (!(((u16)(flags) >> (flagIdx)) & (1 << 0)) == !(isSet))

/** @brief Scales a Q19.12 fixed-point value by a delta time relative to a 30 FPS timestep.
 *
 * @param deltaTime Time delta.
 * @param x Q19.12 fixed-point value to scale.
 * @return Scaled Q19.12 fixed-point value.
 */
#define TIMESTEP_SCALE_30_FPS(deltaTime, x) \
    (((x) * (deltaTime)) / TIMESTEP_30_FPS)

/** @brief Scales a Q19.12 fixed-point value by a delta time relative to a 60 FPS timestep.
 *
 * @param deltaTime Time delta.
 * @param x Q19.12 fixed-point value to scale.
 * @return Scaled Q19.12 fixed-point value.
 */
#define TIMESTEP_SCALE_60_FPS(deltaTime, x) \
    (((x) * (deltaTime)) / TIMESTEP_60_FPS)

/** @brief Used as the maximum rotation allowed on the current tick with a minimum of 1.
 *
 * TODO: Could this have been a more abstract macro which was garbled by optimizations taking advantage of 1 fixed-point
 * full rotation being the same as 1 fixed-point second (4096)?
 */
#define TIMESTEP_ANGLE(denom, shift) \
    (((g_DeltaTime / (denom)) >> (shift)) + 1)

/** @brief Multiplies an integer in fixed-point Q format by a float converted to fixed-point Q format,
 * using a 64-bit intermediate via `Math_MulFixed` for higher precision.
 *
 * @param a First fixed-point factor.
 * @param b Second floating-point factor.
 * @param shift Fixed-point shift.
 * @return Fixed-point product of `a` and `b`.
 */
#define Math_MultiplyFloatPrecise(a, b, shift) \
    Math_MulFixed(a, TO_FIXED(b, shift), shift)

/** @brief Computes the dot product(?) of the first column of a matrix with a vector in Q17.15(?).
 *
 * @param mat Input matrix.
 * @param vec Input vector.
 * @return Dot product(?).
 */
#define Math_MultiplyMatrix(mat, vec)   \
    (s32)((((mat).m[0][0] * (vec).vx) + \
           ((mat).m[1][0] * (vec).vy) + \
           ((mat).m[2][0] * (vec).vz)) >> 17)

/** @brief Computes the Y axis angle from one 3D position in fixed-point Q*.12 to another.
 *
 * @param from Start position (Q19.12).
 * @param to End position (Q19.12).
 * @return Y axis angle between positions (Q19.12).
 */
#define Math_AngleBetweenPositionsGet(from, to) \
    ratan2(to.vx - from.vx,                     \
           to.vz - from.vz)                     \

/** @brief Computes the square magnitude of a value in Q19.12, converting to Q25.6 to avoid overflow.
 *
 * @param x Magnitude (Q19.12).
 * @return Square magnitude (Q25.6).
 */
#define Math_SqrMagCalcToQ6(x) \
    SQUARE(Q12_TO_Q6(x))

/** @brief Computes the square magnitude of a value in Q23.8, converting to Q23.8 to avoid overflow.
 *
 * @param x Magnitude (Q19.12).
 * @return Square magnitude (Q23.8).
 */
#define Math_SqrMagCalcToQ8(x) \
    SQUARE(Q12_TO_Q8(x))

/** @brief Computes the magnitude of a 2D vector in Q19.12.
 *
 * @param x X vector component (Q19.12).
 * @param z Z vector component (Q19.12).
 * @return 2D vector magnitude (Q19.12).
 */
#define Math_Vector2MagCalc(x, z) \
    SquareRoot0(SQUARE(x) + SQUARE(z))

/** @brief Computes the magnitude of a 2D vector in Q19.12, using intermediate components in Q25.6 to safely
 * avoid overflow.
 *
 * @param x X vector component (Q19.12).
 * @param z Z vector component (Q19.12).
 * @return 2D vector magnitude (Q19.12).
 */
#define Math_Vector2MagCalcSafeQ6(x, z) \
    Q6_TO_Q12(SquareRoot0(Math_SqrMagCalcToQ6(x) + Math_SqrMagCalcToQ6(z)))

/** @brief Computes the magnitude of a 2D vector in Q19.12, using intermediate components in Q23.8 to safely
 * avoid overflow.
 *
 * @param x X vector component (Q19.12).
 * @param z Z vector component (Q19.12).
 * @return 2D vector magnitude (Q19.12).
 */
#define Math_Vector2MagCalcSafeQ8(x, z) \
    Q8_TO_Q12(SquareRoot0(Math_SqrMagCalcToQ8(x) + Math_SqrMagCalcToQ8(z)))

/** @brief Computes the magnitude of a 3D vector in Q19.12.
 *
 * @param x X vector component (Q19.12).
 * @param y Y vector component (Q19.12).
 * @param z Z vector component (Q19.12).
 * @return 3D vector magnitude (Q19.12).
 */
#define Math_Vector3MagCalc(x, y, z) \
    SquareRoot0(SQUARE(x) + SQUARE(y) + SQUARE(z))

/** @brief Computes the magnitude of a 3D vector in Q19.12, using intermediate components in Q25.6 to safely
 * avoid overflow.
 *
 * @param x X vector component (Q19.12).
 * @param y Y vector component (Q19.12).
 * @param z Z vector component (Q19.12).
 * @return 3D vector magnitude (Q19.12).
 */
#define Math_Vector3MagCalcSafe(x, y, z) \
    Q6_TO_Q12(SquareRoot0(Math_SqrMagCalcToQ6(x) + Math_SqrMagCalcToQ6(y) + Math_SqrMagCalcToQ6(z)))

/** @brief Computes the magnitude of a 3D vector in Q19.12, using intermediate components in Q23.8 to safely
 * avoid overflow.
 *
 * @param x X vector component (Q19.12).
 * @param y Y vector component (Q19.12).
 * @param z Z vector component (Q19.12).
 * @return 3D vector magnitude (Q19.12).
 */
#define Math_Vector3MagCalcSafeToQ8(x, y, z) \
    Q8_TO_Q12(SquareRoot0(Math_SqrMagCalcToQ8(x) + Math_SqrMagCalcToQ8(y) + Math_SqrMagCalcToQ8(z)))

/** @brief Sets a `DVECTOR` using a fast bitwise method.
 *
 * @param vec Vector to set.
 * @param x X component.
 * @param y Y component.
 */
#define Math_SetDVectorFast(vec, x, y) \
    *((s32*)&(vec)->vx) = ((x) & 0xFFFF) + ((y) << 16)

/** @brief Sets an `SVECTOR` using a fast bitwise method.
 *
 * @param vec Vector to set.
 * @param x X component.
 * @param y Y component.
 * @param z Z component.
 */
#define Math_SetSVectorFast(vec, x, y, z)                         \
    *(s32*)&((vec)->vx) = (s32)((x) & 0xFFFF) | (s32)((y) << 16); \
    *(s16*)&((vec)->vz) = (z)

/** @brief Sets an `SVECTOR` using a fast bitwise method. Variant of `Math_SetSVectorFast`.
 *
 * @param vec Vector to set.
 * @param x X component.
 * @param y Y component.
 * @param z Z component.
 */
#define Math_SetSVectorFastSum(vec, x, y, z)                      \
    *(s32*)&((vec)->vx) = (s32)((x) & 0xFFFF) + (s32)((y) << 16); \
    ((vec)->vz) = (z)

/** @brief Clears an `SVECTOR`'s components.
 *
 * @param vec Output vector.
 */
#define Math_SVectorZero(vec) \
{                             \
    (vec)->vx = 0;            \
    (vec)->vy = 0;            \
    (vec)->vz = 0;            \
}

/** @brief Sets an `SVECTOR`'s components.
 *
 * @param vec Output vector.
 * @param x X component.
 * @param y Y component.
 * @param z Z component.
 */
#define Math_SVectorSet(vec, x, y, z) \
{                                     \
    (vec)->vx = (x);                  \
    (vec)->vy = (y);                  \
    (vec)->vz = (z);                  \
}

/** @brief Clears a `VECTOR3`'s components.
 *
 * @param vec Output vector.
 */
#define Math_Vector3Zero(vec) \
{                             \
    (vec)->vx = 0;            \
    (vec)->vy = 0;            \
    (vec)->vz = 0;            \
}

/** @brief Sets a `VECTOR3`'s components.
 *
 * @param vec Output vector.
 * @param x X component.
 * @param y Y component.
 * @param z Z component.
 */
#define Math_Vector3Set(vec, x, y, z) \
{                                     \
    (vec)->vx = (x);                  \
    (vec)->vy = (y);                  \
    (vec)->vz = (z);                  \
}

/** @brief Normalizes Q19.12 fixed-point degrees, unsigned integer range `[0, 4096]` to the signed integer range `[-2048, 2047]`.
 * Thin wrapper for `Q12_ANGLE_NORM_S`.
 *
 * @param angle Unsigned fixed-point degrees, integer range `[0, 4096]`.
 * @return Signed fixed-point degrees wrapped to the integer range `[-2048, 2047]` (`s16`).
 */
static inline q3_12 Math_AngleNormalize(q19_12 angle)
{
    return Q12_ANGLE_NORM_S(angle);
}

// @hack Checks if `val >= -range && val < range`.
// Needed to allow `li XX, (range * 2), sltu` to be emitted instead of just `sltiu`.
inline static bool Math_CheckSignedRange(s32 val, s32 range)
{
    return (u32)(val + range) > (range * 2);
}

void Math_RotMatrixZxyNeg(const SVECTOR* rot, MATRIX* mat);

void Math_RotMatrixZxyNegGte(const SVECTOR* rot, MATRIX* mat);

/** @brief Computes the sine in Q19.12 of degrees in Q19.12, integer range `[0, 4096]`.
 *
 * TODO: Matched on decomp.me.
 * Possible original name: `shRsin`.
 *
 * @param angle Fixed-point degrees in Q19.12, integer range `[0, 4096]`.
 * @return Sine in Q19.12, integer range `[0, 4096]`.
 */
q19_12 Math_Sin(q19_12 angle);

/** @brief Computes the cosine in Q19.12 of degrees in Q19.12, integer range `[0, 4096]`.
 *
 * TODO: Matched on decomp.me.
 * Possible original name: `shRcos`.
 *
 * @param angle Fixed-point degrees in Q19.12, integer range `[0, 4096]`.
 * @return Cosine in Q19.12, integer range `[0, 4096]`.
 */
q19_12 Math_Cos(q19_12 angle);

MATRIX* Math_RotMatrixZ(s32, MATRIX*); // Previous name: `shRotMatrixZ`

void Math_RotMatrixXyz(SVECTOR* rot, MATRIX* mat); // 0x80096A5C

#endif
