#ifndef _MAPS_MAPUTIL_H
#define _MAPS_MAPUTIL_H

/** @brief @unused Map header `func_C` points to this, but nothing to calls into it. */
s32 sharedFunc_800D929C_0_s00(void);

/** @brief Gets the room index corresponding to the current 2D position.
 *
 * @param posX X position.
 * @param posZ Z position.
 * @return Room index.
 */
u8 Map_RoomIdxGet(q19_12 posX, q19_12 posZ);

#endif
