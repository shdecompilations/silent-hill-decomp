#ifndef _BODYPROG_ITEMS_CAMERA_H
#define _BODYPROG_ITEMS_CAMERA_H

/** Appears to set the camera for items. `itemRot` is always cleared to the identity rotation. */
void ItemScreen_CameraSet(VbRVIEW* camView, GsCOORDINATE2* camCoord, SVECTOR3* itemRot, s32 unused1); // 0x8004BB4C

void func_8004BBF4(VbRVIEW* camView, GsCOORDINATE2* camCoord, SVECTOR* camRot);

void GameFs_TmdDataAlloc(s32* buf); // 0x8004BCBC

/** @brief Used for item rotation and scrolling in the inventory.
 *
 * Name might be inaccurate as it also appears to be used for the camera.
 */
void ItemScreen_ItemRotate(const SVECTOR* itemRot, GsCOORDINATE2* itemCoord); // 0x8004BCDC

void func_8004BD74(s32 displayItemIdx, GsDOBJ2* obj, s32 arg2); // 0x8004BD74

/** Removing this causes the models of items to appear farther from the camera.
 * `Gfx_Items_ViewPointAdjustment`?
 */
void func_8004BFE8(void); // 0x8004BFE8

void func_8004C040(void); // 0x8004C040

#endif
