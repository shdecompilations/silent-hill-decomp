#ifndef _BODYPROG_WORLD_LIGHTING_H
#define _BODYPROG_WORLD_LIGHTING_H

bool Chara_ModelLoadedCheck(e_CharaId charaId);

/** Related to the screen. Called by `WorldEnvRender_Init`. */
void func_80040BAC(void);

void WorldEnvRender_WorldLightTintParamSet(u8 lightPosR, u8 lightPosG, u8 lightPosB,
                                           u8 worldTintR, u8 worldTintG, u8 worldTintB);

void func_80041074(GsOT* ot, s32 arg1, SVECTOR* rot, const VECTOR3* pos);

void func_800410D8(VECTOR3* pos0, q19_12* azimuthAngle, q19_12* altitudeAngle, SVECTOR* rot, const VECTOR3* pos1);

/** @brief Computes the relative spherical rotation on the Y-axis from one direction to another,
 * derived from normalized world-space offsets.
 *
 * @param azimuthAngle Output azimuth angle (Y rotation).
 * @param altitudeAngle Output altitude angle (X rotation).
 * @param offsetFrom First world-space offset (Q19.12).
 * @param offsetTo Second world-space offset (Q19.12).
 */
void Math_RelativeRotationGet(q19_12* azimuthAngle, q19_12* altitudeAngle, const VECTOR* offsetFrom, const VECTOR* offsetTo);

void func_8004137C(VECTOR3* result, const VECTOR* offset0, const VECTOR* offset1, s32 screenDist);

void func_800414E0(GsOT* ot, VECTOR3* arg1, s32 arg2, q19_12 angle0, q19_12 angle1);

#endif
