#ifndef _TYPES_H
#define _TYPES_H

#include "psyq/sys/types.h"

typedef signed char        byte;
typedef signed char        s8;
typedef signed short       s16;
typedef signed int         s32;
typedef signed long long   s64;
typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;

typedef signed char        q0_7;   // Q0.7 fixed-point.
typedef signed short       q11_4;  // Q11.4 fixed-point.
typedef signed short       q7_8;   // Q7.8 fixed-point.
typedef signed short       q3_12;  // Q3.12 fixed-point.
typedef signed int         q27_4;  // Q27.4 fixed-point.
typedef signed int         q25_6;  // Q25.6 fixed-point.
typedef signed int         q24_7;  // Q24.7 fixed-point.
typedef signed int         q23_8;  // Q23.8 fixed-point.
typedef signed int         q21_10; // Q21.10 fixed-point.
typedef signed int         q19_12; // Q19.12 fixed-point.
typedef signed long long   q51_12; // Q51.12 fixed-point.
typedef unsigned char      q0_8;   // Q0.8 fixed-point.
typedef unsigned short     q8_8;   // Q8.8 fixed-point.
typedef unsigned short     q4_12;  // Q4.12 fixed-point.
typedef unsigned int       q24_8;  // Q24.8 fixed-point.
typedef unsigned int       q20_12; // Q20.12 fixed-point.
typedef unsigned long long q52_12; // Q52.12 fixed-point.

#ifndef __cplusplus
    typedef enum { false, true } bool;
#endif

#ifndef NULL
    #define NULL 0
#endif

#define NO_VALUE -1

/** @brief Smaller `VECTOR` with padding removed. Used for fixed-point positions. */
typedef struct _VECTOR3
{
    /* 0x0 */ long vx;
    /* 0x4 */ long vy;
    /* 0x8 */ long vz;
} VECTOR3;

/** @brief Smaller `SVECTOR` with padding removed. Used for fixed-point rotations. */
typedef struct _SVECTOR3
{
    /* 0x0 */ s16 vx;
    /* 0x2 */ s16 vy;
    /* 0x4 */ s16 vz;
} SVECTOR3;

/** @brief `DVECTOR` variant with a `vz` component instead of `vy`. */
typedef struct _DVECTOR_XZ
{
    /* 0x0 */ s16 vx;
    /* 0x2 */ s16 vz;
} DVECTOR_XZ;

/** @brief World position and rotation. */
typedef struct _Pose
{
    /* 0x0 */ VECTOR3  position; /** Q19.12 */
    /* 0xC */ SVECTOR3 rotation; /** Q19.12 */
} s_Pose;

/** @brief Polygon normal. */
typedef struct _Normal
{
    /* 0x0 */ s8 nx;
    /* 0x1 */ s8 ny;
    /* 0x2 */ s8 nz;
    /* 0x3 */ u8 count;
} s_Normal;

/** @brief 8-character string usually used for filenames. Can be compared via the `u32` field. */
typedef union _Filename
{
    /* 0x0 */ char str[8];
    /* 0x8 */ u32  u32[2];
} u_Filename;

/** @brief Compares 8-character filenames using `u32`. Similar to `strcmp`.
 *
 * @param a First filename.
 * @param b Second filename.
 * @return `true` if the filenames aren't equal, `false` otherwise.
 */
#define COMPARE_FILENAMES(a, b)                                  \
    (((u_Filename*)(a))->u32[0] != ((u_Filename*)(b))->u32[0] || \
     ((u_Filename*)(a))->u32[1] != ((u_Filename*)(b))->u32[1])

#endif
