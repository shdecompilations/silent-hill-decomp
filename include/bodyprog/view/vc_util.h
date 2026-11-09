#ifndef _BODYPROG_VIEW_VCUTIL_H
#define _BODYPROG_VIEW_VCUTIL_H

#include "gpu.h"

struct _MapOverlayHdr;

#define MIN_OFFSET(x, neg, pos) \
    ((((x) + (-neg)) <= ((x) + (pos))) ? ((x) - (neg)) : ((x) + (pos)))

#define MAX_OFFSET(x, neg, pos) \
    ((((x) - (neg)) > ((x) + (pos))) ? ((x) - (neg)) : ((x) + (pos)))

/** @brief Initializes the camera subsystem.
 *
 * @param map_overlay_ptr Active map overlay header.
 * @param chr_pos Character position.
 */
void vcInitCamera(struct _MapOverlayHdr* map_overlay_ptr, const VECTOR3* chr_pos);

/** @brief Warps the camera in relation to a character position.
 *
 * @param chr_pos Reference character position (Q19.12).
 * @param chr_ang_y Y rotation.
 */
void vcSetCameraUseWarp(const VECTOR3* chr_pos, q3_12 chr_ang_y);

/** @brief Gets the camera's active smooth move type?
 * TODO: Figure out what a "smooth move" is.
 *
 * @return Smooth move type (`VC_CAM_MV_TYPE`).
 */
s32 vcRetCamMvSmoothF(void);

/** @brief Sets the camera's elevation rate to normal or locked. So far @unused.
 *
 * @param isUnlocked `true` to set the elevation rate to normal, `false` to lock.
 */
void Vc_CameraElevationRateLockSet(bool isUnlocked);

/** @brief Sets the camera's active elevation rate.
 *
 * @param ev_cam_rate New elevation rate.
 */
void vcSetEvCamRate(q3_12 ev_cam_rate);

/** @brief Sets `vcWork.updateLookAtPoint` to `true`.
 * Alternative to `Vc_UpdateLookAtPointSet`.
 */
void Vc_UpdateLookAtPointSetAlt(void);

/** @brief Moves the camera in relation to the player.
 *
 * @param in_connect_f `true` to lock on player position, `false` to lock on player head position.
 * @param change_debug_mode Increment camera state step?
 * @param for_f Contract the XZ radius.
 * @param back_f Expand the XZ radius.
 * @param right_f Rotate counter-clockwise on the Y axis.
 * @param left_f Rotate clockwise on the Y axis.
 * @param up_f Move up.
 * @param down_f Move down.
 */
void vcMoveAndSetCamera(bool in_connect_f, bool change_debug_mode, bool for_f, bool back_f, bool right_f, bool left_f, bool up_f, bool down_f);

/** @brief Gets the player's head position.
 *
 * @param head_pos Output player head position (Q19.12).
 */
void vcMakeHeroHeadPos(VECTOR3* head_pos);

/** @brief Translates a camera position by a horizontal and vertical offset in the direction of a given angle.
 *
 * @param out_pos Output translated position (Q19.12).
 * @param in_pos Position to translate (Q19.12).
 * @param ofs_xz_r Horizontal offset.
 * @param ang_y Offset angle defining the direction of translation.
 * @param ofs_y Y offset.
 */
void vcAddOfsToPos(VECTOR3* out_pos, const VECTOR3* in_pos, q3_12 ofs_xz_r, q3_12 ang_y, q19_12 ofs_y);

/** @brief Translates a reference camera position.
 *
 * @param ref_pos Reference position.
 * @param sys_p System work.
 * @param for_f Contract the XZ radius.
 * @param back_f Expand the XZ radius.
 * @param right_f Rotate counter-clockwise on the Y axis.
 * @param left_f Rotate clockwise on the Y axis.
 * @param up_f Move up.
 * @param down_f Move down.
 */
void vcSetRefPosAndSysRef2CamParam(VECTOR3* ref_pos, s_SysWork* sys_p,
                                   bool for_f, bool back_f, bool right_f, bool left_f, bool up_f, bool down_f);

/** @brief Sets a reference camera position according to analog stick input.
 *
 * @param ref_pos Reference position.
 * @param sys_p System work.
 */
void vcSetRefPosAndCamPosAngByPad(VECTOR3* ref_pos, s_SysWork* sys_p);

/** @brief Computes the stereo sound balance based on the relative position to the camera.
 *
 * @param soundPos Sound source position.
 * @return Stereo sound balance, where values affect the left-right channel balance.
 */
q0_7 Vc_StereoBalanceGet(const VECTOR3* soundPos);

#endif
