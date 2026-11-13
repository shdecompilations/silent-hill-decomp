#ifndef _GPU_H
#define _GPU_H

#include "common.h"

#include <psyq/libgte.h>
#include <psyq/libgpu.h>
#include <psyq/libgs.h>

#define LINE_VERT_COUNT 2
#define RECT_VERT_COUNT 4
#define BOX_VERT_COUNT  8

/** @brief Model primitive material flags. */
typedef enum _MaterialFlags
{
    MaterialFlag_None = 0,
    MaterialFlag_0    = 1 << 0,
    MaterialFlag_1    = 1 << 1,
    MaterialFlag_2    = 1 << 2
} e_MaterialFlags;

/** @brief Semi-transparency blend modes. */
typedef enum _BlendMode
{
    BlendMode_Average     = 0,
    BlendMode_Additive    = 1,
    BlendMode_Subtractive = 2
} e_BlendMode;

/** @brief Primitive flags.
 * TODO: Could split these into 3 enums and create a `PRIM_CODE` macro for convenient packing.
 */
typedef enum _PrimitiveFlags
{
    RECT_MODULATE = 1 << 0, /** Use primitive color to modulate texture. */
    RECT_BLEND    = 1 << 1, /** Semi-transparency flag. */
    RECT_TEXTURE  = 1 << 2, /** Rectangle is textured (`SPRT`). */

    RECT_SIZE_1   = 1 << 3, /** Rectangle is 1x1 (`TILE_1`). */
    RECT_SIZE_8   = 2 << 3, /** Rectangle is 8x8 (`TILE_8` or `SPRT_8`). */
    RECT_SIZE_16  = 3 << 3, /** Rectangle is 16x16 (`TILE_16` or `SPRT_16`). */

    PRIM_POLY     = 1 << 5, /** Polygon (`POLY`). */
    PRIM_LINE     = 2 << 5, /** Line (`LINE`). */
    PRIM_RECT     = 3 << 5  /** Rectangle (`TILE` or `SPRT`). */
} e_PrimitiveFlags;

/** @brief Psy-Q priority primitive, set by `SetPriority`. Removed from later SDK headers. */
typedef struct
{
    u_long tag;
    u_long code[2];
} DR_PRIO;

/** @brief 2D screen-space line. */
typedef struct _Line2d
{
    /* 0x0 */ DVECTOR vertex0;
    /* 0x4 */ DVECTOR vertex1;
} s_Line2d;
STATIC_ASSERT_SIZEOF(s_Line2d, 8);

/** @brief 2D screen-space triangle. */
typedef struct _Triangle2d
{
    /* 0x0 */ DVECTOR vertex0;
    /* 0x4 */ DVECTOR vertex1;
    /* 0x8 */ DVECTOR vertex2;
} s_Triangle2d;
STATIC_ASSERT_SIZEOF(s_Triangle2d, 12);

/** @brief 2D screen-space quad. */
typedef struct _Quad2d
{
    /* 0x0 */ DVECTOR vertex0;
    /* 0x4 */ DVECTOR vertex1;
    /* 0x8 */ DVECTOR vertex2;
    /* 0xC */ DVECTOR vertex3;
} s_Quad2d;
STATIC_ASSERT_SIZEOF(s_Quad2d, 16);

/** @brief Colored 2D screen-space line. */
typedef struct _ColoredLine2d
{
    /* 0x0 */ s_Line2d line;
    /* 0x8 */ u16      r;
    /* 0xA */ u16      g;
    /* 0xC */ u16      b;
    /* 0xE */ u16      __pad_E; // Maybe 4th component of an RGB+code struct?
} s_ColoredLine2d;
STATIC_ASSERT_SIZEOF(s_ColoredLine2d, 16);

/** @brief 2D screen-space line border. */
typedef struct _LineBorder
{
    /* 0x0 */ s_Line2d lines[RECT_VERT_COUNT];
} s_LineBorder;
STATIC_ASSERT_SIZEOF(s_LineBorder, 32);

/** @brief 2D screen-space quad border. */
typedef struct _QuadBorder
{
    /* 0x0 */ s_Quad2d quads[RECT_VERT_COUNT];
} s_QuadBorder;
STATIC_ASSERT_SIZEOF(s_QuadBorder, 64);

/** @brief Primitive color. */
typedef struct _PrimColor
{
    u8 r;
    u8 g;
    u8 b;
    u8 p;
} s_PrimColor;

/** @brief Same as `getTPage`, but `xn` and `yn` are indices instead of VRAM coordinates. */
#define getTPageN(tp, abr, xn, yn) \
    ((((tp) & 0x3) << 7) | (((abr) & 0x3) << 5) | (((yn) & 0x1) << 4) | ((xn) & 0xF))

/** @brief Same as `setRECT`, but uses 2x 32-bit stores instead of 4x 16-bit stores. */
#define setRECTFast(r, x, y, w, h)        \
    ((u32*)(r))[0] = ((x) | ((y) << 16)), \
    ((u32*)(r))[1] = ((w) | ((h) << 16))

/** @brief Same as `setXY0`, but uses 1x 32-bit store instead of 2x 16-bit stores. */
#define setXY0Fast(p, x, y) \
    *(u32*)(&(p)->x0) = (((x) & 0xFFFF) + ((y) << 16))

#define setXY1Fast(p, x, y) \
    *(u32*)(&(p)->x1) = (((x) & 0xFFFF) + ((y) << 16))

#define setXY2Fast(p, x, y) \
    *(u32*)(&(p)->x2) = (((x) & 0xFFFF) + ((y) << 16))

#define setXY3Fast(p, x, y) \
    *(u32*)(&(p)->x3) = (((x) & 0xFFFF) + ((y) << 16))

/** @brief Same as `setWH`, but uses 1x 32-bit store instead of 2x 16-bit stores. */
#define setWHFast(p, _w, _h) \
    *(u32*)(&(p)->w) = ((_w) + ((_h) << 16))

/** @brief Combines `setUV0` and `setClut` into a single 32-bit stores. Also doesn't call `GetClut`. */
#define setUV0AndClut(p, u, v, cx, cy) \
    *(u32*)(&(p)->u0) = (((((cy) << 6) | (((cx) >> 4) & 0x3F)) << 16) | ((v) << 8) | (u))

/** @brief Combines `setUV0` and `setClut` into a single 32-bit store, using ADD to combine `u` and `v`. */
#define setUV0AndClutSum(p, u, v, clut) \
    *(u32*)(&(p)->u0) = ((u) + ((v) << 8) + ((clut) << 16))

/** @brief Combines `setUV1` and `setClut` into a single 32-bit store, using ADD to combine `u` and `v`. */
#define setUV1AndTPageSum(p, u, v, tpage) \
    *(u32*)(&(p)->u1) = ((u) + ((v) << 8) + ((tpage) << 16))

/** @brief Same as `setUV2`, using ADD to combine `u` and `v`. */
#define setUV2Sum(p, u, v) \
    *(u16*)(&(p)->u2) = ((u) + ((v) << 8))

/** @brief Same as `setUV3`, using ADD to combine `u` and `v`. */
#define setUV3Sum(p, u, v) \
    *(u16*)(&(p)->u3) = ((u) + ((v) << 8))

/** @brief Combines `setcode` and `setRGB0`. */
#define setCodeWord(p, code, rgb24) \
    *(u32*)(((u8*)(p)) + 4) = (((code) << 24) | ((rgb24) & 0xFFFFFF))

/** @brief Sets `r0`, `g0`, `b0` and `code` from one packed 32-bit word. */
#define setRGBC0Word(p, rgbc) \
    *(u32*)(&(p)->r0) = (rgbc)

/** @brief Sets `r1`, `g1`, `b1` and the padding byte after them from one packed 32-bit word. */
#define setRGBC1Word(p, rgbc) \
    *(u32*)(&(p)->r1) = (rgbc)

/** @brief Sets `r2`, `g2`, `b2` and the padding byte after them from one packed 32-bit word. */
#define setRGBC2Word(p, rgbc) \
    *(u32*)(&(p)->r2) = (rgbc)

/** @brief Sets `r3`, `g3`, `b3` and the padding byte after them from one packed 32-bit word. */
#define setRGBC3Word(p, rgbc) \
    *(u32*)(&(p)->r3) = (rgbc)

// TODO: Perhaps `setRGBC0`, `setRGBC1`, `setRGBC2`, and `setRGBC3` were one macro. Incidental value set to padding fields suggests it?
/** @brief Combines `setRGB0` and `setcode`. */
#define setRGBC0(prim, r, g, b, code) \
    *(u32*)(&(prim)->r0) = ((((r) + ((g) << 8)) + ((b) << 16)) + ((code) << 24))

/** @brief Combines `setRGB1` and incidentally applies code to the padding component. */
#define setRGBC1(prim, r, g, b, code) \
    *(u32*)(&(prim)->r1) = ((((r) + ((g) << 8)) + ((b) << 16)) + ((code) << 24))

/** @brief Combines `setRGB2` and incidentally applies code to the padding component. */
#define setRGBC2(prim, r, g, b, code) \
    *(u32*)(&(prim)->r2) = ((((r) + ((g) << 8)) + ((b) << 16)) + ((code) << 24))

/** @brief Combines `setRGB3` and incidentally applies code to the padding component. */
#define setRGBC3(prim, r, g, b, code) \
    *(u32*)(&(prim)->r3) = ((((r) + ((g) << 8)) + ((b) << 16)) + ((code) << 24))

/** @brief Slightly faster `setRGB0`. */
#define setRGB0Fast(p, r, g, b) \
    (*(u16*)&(p)->r0 = (r) + ((g) << 8), (p)->b0 = (b))

#define setRGB1Fast(p, r, g, b) \
    (*(u16*)&(p)->r1 = (r) + ((g) << 8), (p)->b1 = (b))

#define setRGB2Fast(p, r, g, b) \
    (*(u16*)&(p)->r2 = (r) + ((g) << 8), (p)->b2 = (b))

#define setRGB3Fast(p, r, g, b) \
    (*(u16*)&(p)->r3 = (r) + ((g) << 8), (p)->b3 = (b))

/** @brief Combines `addPrim` and `setlen`. */
#define addPrimFast(ot, p, _len) \
    (((p)->tag = getaddr(ot) | ((_len) << 24)), setaddr(ot, p))

/** @brief Combines `setPolyFT4` with `tpage` setter.
  * @hack Needed to allow `tpage` and POLY_FT4 code `0x2C` to be merged in some cases.
*/
#define setPolyFT4TPage(poly, tp) \
({ \
    s32 tpage = (tp); \
    setPolyFT4((poly)); \
    (poly)->tpage = tpage; \
})

extern _GsFCALL GsFCALL4;

void GsTMDfastG3LFG(void* op, VERT* vp, VERT* np, PACKET* pk, int n, int shift, GsOT* ot, u_long* scratch);
void GsTMDfastTG3LFG(void* op, VERT* vp, VERT* np, PACKET* pk, int n, int shift, GsOT* ot, u_long* scratch);
void GsTMDfastG4LFG(void* op, VERT* vp, VERT* np, PACKET* pk, int n, int shift, GsOT* ot, u_long* scratch);
void GsTMDfastTG4LFG(void* op, VERT* vp, VERT* np, PACKET* pk, int n, int shift, GsOT* ot, u_long* scratch);

void SetPriority(DR_PRIO* p, s32 pbc, s32 pbw);

/** @brief Sets the `DQA` register in the GTE. Not part of Psy-Q for some reason. */
#define gte_lddqa(r0) __asm__ volatile( \
    "ctc2  %0, $27;"                    \
    :                                   \
    : "r"(r0))

/** @brief Zeroes the `DQB` register in the GTE. */
#define gte_lddqb_0() __asm__ volatile( \
    "ctc2  $zero, $28;")

/** @brief Zeroes transfer vector in the GTE. */
#define gte_ldtr_0() __asm__ volatile( \
    "ctc2  $zero, $5;"                 \
    "ctc2  $zero, $6;"                 \
    "ctc2  $zero, $7;")

/** @brief Loads `SVECTOR` into light matrix, similar to `gte_SetLightMatrix`. */
#define gte_SetLightSVector(p) __asm__ volatile( \
    "lw    $12, 0(%0);"                          \
    "lhu   $13, 4(%0);"                          \
    "ctc2  $12, $8;"                             \
    "ctc2  $13, $9;"                             \
    "ctc2  $zero, $10;"                          \
    "ctc2  $zero, $11;"                          \
    "ctc2  $zero, $12;"                          \
    :                                            \
    : "r"(p)                                     \
    : "$12", "$13", "memory");

/** @brief Broadcasts a single value into the GTE universal vector, similar to `gte_ldsv`. */
#define gte_ldsv_(val) __asm__ volatile( \
    "mtc2  %0, $9;"                      \
    "mtc2  %0, $10;"                     \
    "mtc2  %0, $11;"                     \
    :                                    \
    : "r"(val)                           \
    : "memory");

/** @brief Returns the value of the GTE `IR1` register. */
#define gte_stIR1()                                        \
    ({                                                     \
        u32 __r;                                           \
        __asm__ volatile("mfc2 %0, $9; nop;" : "=r"(__r)); \
        __r;                                               \
    })

/** @brief Loads `SVECTOR` into GTE Vector 0. */
#define gte_SetVector0(p) __asm__ volatile( \
    "lw    $12, 0(%0);"                     \
    "lhu   $13, 4(%0);"                     \
    "mtc2  $12, $0;"                        \
    "mtc2  $13, $1;"                        \
    :                                       \
    : "r"(p)                                \
    : "$12", "$13", "memory")

/** @brief Loads GTE light source vector `x` and `y`. */
#define gte_SetLightSourceXY(x, y) __asm__ volatile( \
    "sll  %0, %0, 16;"                               \
    "srl  %0, %0, 16;"                               \
    "sll  %1, %1, 16;"                               \
    "or   %0, %0, %1;"                               \
    "ctc2 %0, $8;"                                   \
    :                                                \
    : "r"(x), "r"(y))

/** @brief Loads GTE light source vector `z`. */
#define gte_SetLightSourceZ(z) __asm__ volatile( \
    "ctc2  %0, $9;"                              \
    "ctc2  $zero, $10;"                          \
    "ctc2  $zero, $11;"                          \
    "ctc2  $zero, $12;"                          \
    :                                            \
    : "r"((z) & 0xFFFF))

/** @brief Loads row 0 and row 1 from `MATRIX` into GTE rotation matrix. */
#define gte_SetRotMatrix_Row0_1(r0) __asm__ volatile( \
    "lw   $12, 0( %0 );"                              \
    "lw   $13, 4( %0 );"                              \
    "ctc2 $12, $0;"                                   \
    "lw   $12, 8( %0 );"                              \
    "ctc2 $13, $1;"                                   \
    "ctc2 $12, $2"                                    \
    :                                                 \
    : "r"(r0)                                         \
    : "$12", "$13")

/** @brief Loads row 2 from `MATRIX` into GTE rotation matrix. */
#define gte_SetRotMatrix_Row2(r0) __asm__ volatile( \
    "lw   $12, 12( %0 );"                           \
    "lw   $13, 16( %0 );"                           \
    "ctc2 $12, $3;"                                 \
    "ctc2 $13, $4;"                                 \
    :                                               \
    : "r"(r0)                                       \
    : "$12", "$13")

#define gte_LoadVector0_1_2_XYZ(xy, z) __asm__ volatile( \
    /* Load vector 0. */ \
    "lhu   $12, 0(%1);" /* CPU $12   = Z0 (unsigned 16-bit from `z[0]`) */ \
    "lwc2  $0, 0(%0);"  /* COP2 VXY0 = X0,Y0 (packed 2x16-bit from `xy[0]`) */ \
    "mtc2  $12, $1;"    /* COP2 VZ0  = Z0 (low 16 bits used) */ \
    /* Load vector 1. */ \
    "lhu   $12, 2(%1);" /* CPU $12   = Z1 */ \
    "lwc2  $2, 4(%0);"  /* COP2 VXY1 = X1,Y1 */ \
    "mtc2  $12, $3;"    /* COP2 VZ1  = Z1 */ \
    /* Load vector 2. */ \
    "lhu   $12, 4(%1);" /* CPU $12   = Z2 */ \
    "lwc2  $4, 8(%0);"  /* COP2 VXY2 = X2,Y2 */ \
    "mtc2  $12, $5;"    /* COP2 VZ2  = Z2 */ \
    : \
    : "r"(xy), "r"(z) \
    : "$12", "memory")

#define gte_FetchScreen0_1_2_XYZ(xy, z) __asm__ volatile( \
    /* Vertex 0. */ \
    "mfc2  $12, $17;"   /* CPU $12 = SZ1 (depth for vertex 0) */ \
    "swc2  $12, 0(%0);" /* COP2 SXY0 (reg 12) -> xy[0] (packed X0, Y0) */ \
    "sh    $12, 0(%1);" /* CPU $12 (SZ1) -> `z[0]` */ \
    /* Vertex 1. */ \
    "mfc2  $12, $18;"   /* CPU $12 = SZ2 */ \
    "swc2  $13, 4(%0);" /* COP2 SXY1 (reg 13) -> `xy[1]` */ \
    "sh    $12, 2(%1);" /* CPU $12 (SZ2) -> `z[1]` */ \
    /* Vertex 2. */ \
    "mfc2  $12, $19;"   /* CPU $12 = SZ3 */ \
    "swc2  $14, 8(%0);" /* COP2 SXY2 (reg 14) -> `xy[2]` */ \
    "sh    $12, 4(%1);" /* CPU $12 (SZ3) -> `z[2]` */ \
    : \
    : "r"(xy), "r"(z) \
    : "$12", "memory")

/** @brief Less efficient version of `gte_SetRotMatrix` from PsyQ?
* PsyQ `gte_SetRotMatrix` loads 32-bit words from the `MATRIX` straight into GTE,
* while this macro reads 16-bit words and combines them into a 32-bit word before loading.
* Not sure of reason why, wonder if it's from some older PsyQ SDK.
*/
#define gte_SetRotMatrix_custom(mat) __asm__ volatile( \
    "lhu $12, 16(%0);"                                 \
    "nop;"                                             \
    "ctc2 $12, $4;"                                    \
    "lhu $12, 10(%0);"                                 \
    "lhu $13, 4(%0);"                                  \
    "sll $12, $12, 16;"                                \
    "addu $12, $12, $13;"                              \
    "ctc2 $12, $3;"                                    \
    "lhu $12, 14(%0);"                                 \
    "lhu $13, 8(%0);"                                  \
    "sll $12, $12, 16;"                                \
    "addu $12, $12, $13;"                              \
    "ctc2 $12, $2;"                                    \
    "lhu $12, 2(%0);"                                  \
    "lhu $13, 12(%0);"                                 \
    "sll $12, $12, 16;"                                \
    "addu $12, $12, $13;"                              \
    "ctc2 $12, $1;"                                    \
    "lhu $12, 6(%0);"                                  \
    "lhu $13, 0(%0);"                                  \
    "sll $12, $12, 16;"                                \
    "addu $12, $12, $13;"                              \
    "ctc2 $12, $0;"                                    \
    :                                                  \
    : "r"(mat)                                         \
    : "$12", "$13", "memory")

/** @brief Loads `x`/`y`/`z` into GTE Vector 0 / `VZY0`/`VZ0`. */
#define gte_LoadVector0_XYZ(x, y, z) __asm__ volatile( \
    "sll %0, %0, 16;"                                  \
    "srl %0, %0, 16;"                                  \
    "sll %1, %1, 16;"                                  \
    "or  %0, %0, %1;"                                  \
    "mtc2 %0, $0;"                                     \
    "mtc2 %2, $1;"                                     \
    :                                                  \
    : "r"(x), "r"(y), "r"(z))

/** @brief Sets the GTE's `vxy0` register to 0. */
#define gte_ldvxy0_Zero() __asm__ volatile( \
    "mtc2  $zero, $0;")

#define gte_ldvxy0(val) __asm__ volatile( \
    "mtc2  %0, $0;"                       \
    :                                     \
    : "r"(val)                            \
    : "memory")

/** @brief Sets the GTE's `vz0` register to 0. */
#define gte_ldvz0() __asm__ volatile( \
    "mtc2  $zero, $1;")

/** @brief Retrieves the value from the GTE's `SZ3` register. */
#define gte_stSZ3()                                   \
    ({                                                \
        u32 __r;                                      \
        __asm__ volatile("mfc2 %0, $19" : "=r"(__r)); \
        __r;                                          \
    })

#define gte_stMAC0()                                       \
    ({                                                     \
        s32 __r;                                           \
        __asm__ volatile("mfc2 %0, $24; nop" : "=r"(__r)); \
        __r;                                               \
    })

#define gte_stMAC1()                                        \
    ({                                                      \
        u32 __r;                                            \
        __asm__ volatile("mfc2 %0, $25; nop;" : "=r"(__r)); \
        __r;                                                \
    })

#define gte_stMAC2()                                        \
    ({                                                      \
        u32 __r;                                            \
        __asm__ volatile("mfc2 %0, $26; nop;" : "=r"(__r)); \
        __r;                                                \
    })

#define gte_stMAC12(r0) __asm__ volatile( \
    "mfc2    $12, $25;"                   \
    "nop;"                                \
    "sh    $12, 0( %0 );"                 \
    "mfc2    $13, $26;"                   \
    "nop;"                                \
    "sh    $13, 2( %0 );"                 \
    :                                     \
    : "r"(r0)                             \
    : "$12", "$13", "memory")

#define gte_ldsv3_(x, y, z) __asm__ volatile( \
    "mtc2  %0, $9;"                           \
    "mtc2  %1, $10;"                          \
    "mtc2  %2, $11;"                          \
    :                                         \
    : "r"(x), "r"(y), "r"(z)                  \
    : "memory")

#define gte_ldR13R21(val) __asm__ volatile( \
    "or    $12, %0, $0;"                    \
    "negu  $12, $12;"                       \
    "ctc2  $12, $1;"                        \
    :                                       \
    : "r"(val)                              \
    : "$12", "memory")

#define gte_ldR11R12(r0) __asm__ volatile( \
    "ctc2 %0, $0;"                         \
    "ctc2 %0, $2;"                         \
    :                                      \
    : "r"(r0)                              \
    : "memory")

#define gte_stlzcr(dst) __asm__ volatile( \
    "mfc2  %0, $31"                       \
    : "=r"(dst))

#define gte_ldir_stbk() __asm__ volatile ( \
    "cfc2    $12, $13;"                    \
    "nop;"                                 \
    "mtc2    $12, $9;"                     \
    "cfc2    $12, $14;"                    \
    "nop;"                                 \
    "mtc2    $12, $10;"                    \
    "cfc2    $12, $15;"                    \
    "nop;"                                 \
    "mtc2    $12, $11;"                    \
    : : : "$12")

#define gte_ldmac_stir() __asm__ volatile ( \
    "mfc2    $12, $25;"                     \
    "nop;"                                  \
    "ctc2    $12, $13;"                     \
    "mfc2    $12, $26;"                     \
    "nop;"                                  \
    "ctc2    $12, $14;"                     \
    "mfc2    $12, $27;"                     \
    "nop;"                                  \
    "ctc2    $12, $15;"                     \
    : : : "$12")

#endif
