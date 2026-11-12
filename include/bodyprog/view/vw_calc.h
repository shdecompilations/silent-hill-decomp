#ifndef _BODYPROG_VIEW_VW_CALC_H
#define _BODYPROG_VIEW_VW_CALC_H

#include "gpu.h"

void vwRenewalXZVelocityToTargetPos(q19_12* velo_x, q19_12* velo_z, const VECTOR3* now_pos, const VECTOR3* tgt_pos, q19_12 tgt_r,
                                    q19_12 accel, q19_12 total_max_spd, q19_12 dec_forwd_lim_spd, q19_12 dec_accel_side);

void vwLimitOverLimVector(q19_12* vec_x, q19_12* vec_z, q19_12 lim_vec_len, q3_12 lim_vec_ang_y);

void vwDecreaseSideOfVector(q19_12* vec_x, q19_12* vec_z, q19_12 dec_val, q19_12 max_side_vec_len, q3_12 dir_ang_y);

q19_12 vwRetNewVelocityToTargetVal(q19_12 now_spd, q19_12 mv_pos, q19_12 tgt_pos, q19_12 accel, q19_12 total_max_spd, q19_12 dec_val_lim_spd);

/** @brief TODO
 *
 * @param now_ang_spd Current angular speed.
 * @param now_ang Current angle.
 * @param tgt_ang Target angle.
 * @param accel_spd Acceleration speed.
 * @param total_max_ang_spd Max angular speed.
 * @param dec_val_lim_spd Deceleration speed.
 * @return New angular speed.
 */
q19_12 vwRetNewAngSpdToTargetAng(q19_12 now_ang_spd, q3_12 now_ang, q3_12 tgt_ang,
                                 q19_12 accel_spd, q19_12 total_max_ang_spd, q19_12 dec_val_lim_spd);

/** @brief @unused Computes a FPS-independent clamped interpolation speed.
 *
 * @param speedFrom Speed from.
 * @param speedTo Speed to.
 * @param rateMax Maximum rate of change per unit time.
 * @return Clamped speed.
 */
q19_12 Vw_ClampedSpeedToTargetGet(q19_12 speedFrom, q19_12 speedTo, q19_12 rateMax);

/** @brief Projects a 3D world position to 2D screen coordinates with translation accumulation.
 *
 * @param worldPos 3D position to transform and project.
 * @param screenPos Output 2D screen coordinates.
 * @return Projected depth / 4.
 */
s32 Vw_TransformAndProjectPoint(VECTOR* worldPos, DVECTOR* screenPos);

void vwMatrixToAngleYXZ(SVECTOR* ang, const MATRIX* mat);

void Vw_MultiplyAndTransformMatrix(MATRIX* transformMat, MATRIX* inMat, MATRIX* outMat);

void vbSetWorldScreenMatrix(GsCOORDINATE2* coord);

/** @brief Sets the camera's reference view.
 *
 * @param rview Reference view to set.
 */
void vbSetRefView(VbRVIEW* rview);

/** @brief Computes the transformation matrix of a given coord.
 *
 * @param boneCoords Character model bone coords.
 * @param transformMat Output transformation matrix.
 */
void Vw_CoordHierarchyMatrixCompute(GsCOORDINATE2* boneCoords, MATRIX* transformMat);

/** @brief Computes a view-space matrix for a coordinate hierarchy node.
 *
 * Gets the cumulative world matrix, subtracts the camera position, and multiplies by `VwCrdWsRotMat`.
 *
 * @param boneCoords Character model bone coords.
 * @param vewMat Output view-space matrix.
 */
void Vw_CoordToViewSpaceMatrix(GsCOORDINATE2* boneCoords, MATRIX* viewMat);

/** @brief Computes world-space and view-space matrices for a coordinate node hierachy.
 *
 * @param boneCoords Character model bone coords.
 * @param worldMat Output world-space matrix.
 * @param viewMat Output view-space matrix.
 */
void Vw_CoordToWorldAndViewMatrices(GsCOORDINATE2* boneCoords, MATRIX* worldMat, MATRIX* viewMat);

/** @brief Constructs a world-to-screen matrix at a given world position.
 *
 * @param worldToScreenMat Output world-screen matrix.
 * @param posX World X position).
 * @param posY World Y position).
 * @param posZ World Z position).
 */
void Vw_WorldScreenMatrixAtPositionGet(MATRIX* worldToScreenMat, q19_12 posX, q19_12 posY, q19_12 posZ);

/** @brief Checks if an AABB is visible in the screen.
 *
 * @param minX AABB X minimum point.
 * @param maxX AABB X maxiumum point.
 * @param minY AABB Y minimum point.
 * @param maxY AABB Y maxiumum point.
 * @param minZ AABB Z minimum point.
 * @param maxZ AABB Z maxiumum point.
 * @return `true` if the AABB is visible, `false` otherwise.
 */
bool Vw_AabbVisibleInScreenCheck(s32 minX, s32 maxX, s32 minY, s32 maxY, s32 minZ, s32 maxZ);

/** @brief Checks if an AABB is visible in a frustum.
 *
 * @param modelMat Model matrix. TODO: What for?
 * @param minX AABB X minimum point.
 * @param minY AABB Y minimum point.
 * @param minZ AABB Z minimum point.
 * @param maxX AABB X maxiumum point.
 * @param maxY AABB Y maxiumum point.
 * @param maxZ AABB Z maxiumum point.
 * @param nearPlane Frustum near plane.
 * @param farPlane Frustum far plane.
 * @return `true` if the AABB is visible, `false` otherwise.
 */
bool Vw_AabbVisibleInFrustumCheck(MATRIX* modelMat,
                                  q7_8 minX, q7_8 minY, q7_8 minZ,
                                  q23_8 maxX, q23_8 maxY, q23_8 maxZ,
                                  u16 nearPlane, u16 farPlane);

/** @brief Checks if screen-space region flags span across the screen center.
 *
 * @param regionFlags 3x3 screen region occupancy flags.
 * @return `true` if geometry spans the visible region, `false` otherwise.
 */
bool Vw_ScreenRegionSpanCheck(s_CameraScreenRegionFlags* regionFlags);

/** @brief Converts a rotation to a direction vector with a given length.
 *
 * @param vec Output direction vector.
 * @param ang Rotation to convert.
 * @param r Radius representing the direction vector's length.
 */
void vwAngleToVector(SVECTOR* vec, const SVECTOR* ang, s32 r);

/** @brief Converts a direction vector to a rotation.
 *
 * @param ang Output rotation.
 * @param vec Direction vector to convert.
 * @return Direction vector length.
 */
q19_12 vwVectorToAngle(SVECTOR* ang, const SVECTOR* vec);

/** @brief Performs linear interpolation between Y values based on an input X within a given range.
 *
 * @param y_ary Array of Y values.
 * @param y_suu `y_ary` size.
 * @param input_x Input value.
 * @param min_x Minimum range.
 * @param max_x Maximum range.
 */
s32 vwOresenHokan(const s32* y_ary, s32 y_suu, s32 input_x, s32 min_x, s32 max_x);

#endif
