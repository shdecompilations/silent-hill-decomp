#ifndef _BODYPROG_BODYPROGMATH8005BF38_H
#define _BODYPROG_BODYPROGMATH8005BF38_H

/** @brief Normalizes a signed Q3.12 fixed-point angle to the clamped signed integer range `[-2048, 2047]`.
 *
 * TODO: Appears to have the same effect as `Q12_ANGLE_NORM_S` macro. Maybe this was written by someone unaware of that?
 * Once the build is shiftable, check if calls to this can simply be switched over to the macro.
 *
 * @param angle Angle to normalize.
 * @return Normalized angle.
 */
q3_12 Math_AngleNormalizeSigned(q3_12 angle);

/** @unused */
bool func_8005BF58(s32* arg0, s32* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9);

/** @unused */
s32 func_8005C1CC(s32* arg0, s32* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);

u32 func_8005C478(s16* arg0, q19_12 x0, q19_12 y0, q19_12 x1, q19_12 y1, q19_12 x2, q19_12 y2);

q19_12 func_8005C7B0(q19_12 angle);

#endif
