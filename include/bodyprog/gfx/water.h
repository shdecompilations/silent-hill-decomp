#ifndef _BODYPROG_GFX_WATER_H
#define _BODYPROG_GFX_WATER_H

void func_8008D41C(void);

void func_8008D438(void);

void func_8008D448(void);

void func_8008D454(void);

void func_8008D464(void);

/** Light function. */
void func_8008D470(q3_12 lensFlareIntensity, SVECTOR* rot, VECTOR3* pos, s_WaterZone* waterZones);

void func_8008D5A0(VECTOR3* arg0, s16 arg1);

/** Light function. */
q19_12 func_8008D8C0(q3_12 lensFlareIntensity, q19_12 x1, q19_12 x2);

void func_8008D990(s32 arg0, q19_12 arg1, VECTOR3* arg2, s32 arg3, s32 arg4);

/** @brief Gets the water zone at a given position.
 *
 * @param posX X position.
 * @param posZ Z position.
 * @param waterZones Water zones to query.
 * @return Water zone at the given position.
 */
s_WaterZone* Map_WaterZoneGet(q27_4 posX, q27_4 posZ, s_WaterZone* waterZones);

void func_8008E5B4(void);

void func_8008E794(VECTOR3* posXz, q3_12 angle, q19_12 posY);

void func_8008EA68(SVECTOR* arg0, VECTOR3* posXz, q19_12 posY);

#endif
