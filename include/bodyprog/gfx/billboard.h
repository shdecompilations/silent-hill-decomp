#ifndef _BODYPROG_GFX_BILLBOARD_H
#define _BODYPROG_GFX_BILLBOARD_H

/** @brief 2D billboard sprite info. */
typedef struct _BillboardInfo
{
    /* 0x0  */ q3_12   positionY;
    /* 0x2  */ q3_12   offsetDistance;
    /* 0x4  */ q3_12   offsetAngle; // Or maybe the facing angle, i.e. the direction toward which it's displayed?
    /* 0x6  */ s16     field_6;     // } Related to depth scale, maybe for XZ axes.
    /* 0x8  */ s16     field_8;     // }
    /* 0xA  */ s8      __pad_A[2];
    /* 0xC  */ SVECTOR offsetDirection; /** Q3.12 | Unused, dead code. Set in `Gfx_BillboardInfosInit` and never referenced again. */
    /* 0x14 */ SVECTOR position;        /** Q3.12 */
} s_BillboardInfo;

/** @brief Workspace for 2D billboard sprites to draw. */
typedef struct _BillboardWork
{
    /* 0x0 */ s_BillboardInfo* billboardInfos;     /** Billboards to draw on the current frame. */
    /* 0x4 */ u8               billboardInfoCount; /** `billboardInfos` array size. */
    /* 0x5 */ s8               __pad_5;
    /* 0x6 */ q7_8             offsetY;
    /* 0x8 */ u8               field_8; // u0
    /* 0x9 */ u8               field_9; // 
    /* 0xA */ u8               field_A; // u3
    /* 0xB */ u8               field_B; //
} s_BillboardWork;

/** @brief Initializes positions in the `g_BillboardInfos` array.
 *
 * @param viewCoord Camera coordinate.
 */
void Gfx_BillboardInfosInit(GsCOORDINATE2* viewCoord);

/** @brief Draws a 2D billboard sprite.
 *
 * @param idx Billboard index. TODO: Find out how everything is stored.
 * @param posX X position.
 * @param posY Y position.
 * @param posZ Z position.
 * @param ot Ordering table.
 * @param otShift Ordering table shift.
 */
void Gfx_BillboardDraw(s32 idx, q19_12 posX, q19_12 posY, q19_12 posZ, GsOT* ot, s32 otShift);

#endif
