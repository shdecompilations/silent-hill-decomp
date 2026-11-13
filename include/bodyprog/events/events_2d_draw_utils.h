#ifndef _BODYPROG_EVENTS_EVENTS2DDRAWUTILS_H
#define _BODYPROG_EVENTS_EVENTS2DDRAWUTILS_H

void Gfx_CursorDraw(s32 x0, s16 y0, s32 x1, s16 y1, s16 u, s16 v, s16 width, s32 height, s32 tint,
                    u32 clutX, s16 clutY, s32 tPage);

/** @brief Draws a set of expanding boxes used to highlight a point on a paper map.
 *
 * @param progressAlpha Progress weight.
 * @param startX X start corner (top-left).
 * @param startY Y start corner (top-left).
 * @param startWidth Start width.
 * @param startHeight Start height.
 * @param endX X end corner (bottom-right).
 * @param endY Y end corner (bottom-right).
 * @param endWidth End width.
 * @param endHeight End height.
 */
void PaperMap_ExpandingBoxesDraw(q3_12 progressAlpha,
                                 q3_12 startX, q3_12 startY, q3_12 startWidth, q3_12 startHeight,
                                 q3_12 endX, q3_12 endY, q3_12 endWidth, q3_12 endHeight);

#endif
