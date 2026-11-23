#ifndef _BODYPROG_ITEMS_CAMERA_H
#define _BODYPROG_ITEMS_CAMERA_H

/** @brief Sets the camera for the item screen. `camRot` is always cleared to the identity rotation.
 *
 * @param camView Camera view.
 * @param camCoord Camera coord.
 * @param camRot Camera rotation.
 * @param unused Unused.
 */
void ItemScreen_CameraSet(VbRVIEW* camView, GsCOORDINATE2* camCoord, SVECTOR3* camRot, s32 unused); // 0x8004BB4C

/** @brief Sets the camera for the item screen. `camRot` is always cleared to the identity rotation.
 *
 * @param camView Camera view.
 * @param camCoord Camera coord.
 * @param camRot Camera rotation.
 */
void func_8004BBF4(VbRVIEW* camView, GsCOORDINATE2* camCoord, SVECTOR* camRot);

void GameFs_TmdDataAlloc(s32* buf); // 0x8004BCBC

/** @brief Used for item rotation and scrolling in the inventory.
 *
 * Name might be inaccurate as it also appears to be used for the camera.
 */
void ItemScreen_ItemRotate(const SVECTOR* itemRot, GsCOORDINATE2* itemCoord); // 0x8004BCDC

void ItemScreen_ItemDraw(s32 itemIdx, GsDOBJ2* obj, s32 arg2); // 0x8004BD74

/** Removing this causes the models of items to appear farther from the camera.
 * `Gfx_Items_ViewPointAdjustment`?
 */
void ItemScreen_ViewpointAdjust(void); // 0x8004BFE8

/** Possible failsafe?
 * Used when exiting the inventory screen or going into options and map menus through the inventory.
 *
 * Removing it doesn't affect the game.
 *
 * @note From a member of the PS Decomp Discord:
 * "[The function] essentially just tries to restore projection/offset settings
 * and the transform matrix to what they were in the main game, but I'm guessing
 * the game sets them at the start of every frame anyway, so it doesn't really
 * achieve anything."
 */
void ItemScreen_CameraReset(void); // 0x8004C040

#endif
