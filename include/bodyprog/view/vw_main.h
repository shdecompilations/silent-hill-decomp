#ifndef _BODYPROG_VIEW_VWMAIN_H
#define _BODYPROG_VIEW_VWMAIN_H

void vwInitViewInfo(void);

/** @brief Gets the camera coordinate.
 *
 * @return Camera coordinate.
 */
GsCOORDINATE2* vwGetViewCoord(void);

/** @brief Gets the camera position.
 *
 * @param pos Output camera position (Q19.12).
 */
void vwGetViewPosition(VECTOR3* pos);

/** @brief Gets the camera rotation.
 *
 * @param ang Output camera rotation (Q19.12).
 */
void vwGetViewAngle(SVECTOR* ang);

/** @brief Sets the camera's look-at direction matrix.
 *
 * @param pos Camera position.
 * @param lookAt Camera look-at position.
 */
void Vw_SetLookAtMatrix(const VECTOR3* pos, const VECTOR3* lookAt);

// TODO: Doc.
void vwSetCoordRefAndEntou(GsCOORDINATE2* parent_p,
                           q19_12 ref_x, q19_12 ref_y, q19_12 ref_z,
                           q3_12 cam_ang_y, q3_12 cam_ang_z, q19_12 cam_y, q19_12 cam_xz_r);

/** @brief Sets an camera matrix.
 *
 * @param pcoord Matrix coordinate.
 * @param cammat Camera matrix.
 */
void vwSetViewInfoDirectMatrix(GsCOORDINATE2* pcoord, const MATRIX* cammat);

/** @brief Sets camera info for view matrices. */
void vwSetViewInfo(void);

/** @brief Clamps an angle range to be within a given set of constraints.
 *
 * @param angleMin Output minimum angle to clamp.
 * @param angleMax Output maximum angle to clamp.
 * @param angleConstrainMin Minimum angle constraint.
 * @param angleConstraintMax Maximum angle constraint.
 */
void Vw_ClampAngleRange(q3_12* angleMin, q3_12* angleMax, q3_12 angleConstraintMin, q3_12 angleConstraintMax);

/** @brief Computes the parametric intersection ratio of a line segment against a bounded interval.
 * Used in collision to compute how far a moving edge must be clipped.
 *
 * @param segmentLength Length/extent of the segment.
 * @param segmentDir Direction component of the line segment.
 * @param distToBound Signed distance to the boundary.
 * @param boundsMin Minimum bound of valid interval.
 * @param boundsMax Maximum bound of valid interval.
 * @return `Q12(1.0f)` if no intersection, `Q12(0.0f)` if at boundary.
 */
q3_12 Vw_LineSegmentIntersectionCheck(s16 segmentLength, s16 segmentDir,
                                      s16 distToBound, q3_12 boundsMin, q3_12 boundsMax);

#endif
