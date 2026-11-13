#ifndef _BODYPROG_BODYPROG80055028_H
#define _BODYPROG_BODYPROG80055028_H

void WorldEnvRender_Init(void);

/** @brief Draws 2D screen effects for the flashlight's lens flare and glow. */
void WorldGfx_2dEffectsDraw(void);

/** Sets visual world parameters. */
void WorldEnvRender_WorldLightingParamSet(u8 arg0, s32 arg1, u8 arg2, s32 tintR, s32 tintG, s32 tintB, q23_8 brightness);

void WorldEnvRender_FogParamsSet(u8 isFogEnabled, u8 fogColorR, u8 fogColorG, u8 fogColorB);

void WorldEnvRender_WorldLightTintSet(u32 overlapLighting, u8 lightPosR, u8 lightPosG, u8 lightPosB, u8 worldTintR, u8 worldTintG, u8 worldTintB);

/** @brief Gets the point light position.
 *
 * @param pos Output light position.
 */
void WorldEnvRender_LightPositionGet(VECTOR3* pos);

/** @brief Gets the point light rotation and intensity.
 *
 * @param rot Output light rotation.
 * @return Light intensity.
 */
s32 WorldEnvRender_LightRotationAndIntensityGet(SVECTOR* rot);

/** @brief Gets the point light direction and intensity.
 *
 * @param rot Output light direction.
 * @return Light intensity.
 */
s32 WorldEnvRender_LightDirectionAndIntensityGet(SVECTOR* dir);

/** Light function. */
void Gfx_FlashlightPositionUpdate(q19_12 lightIntensity, q3_12 lensFlareIntensity, GsCOORDINATE2* coord0, GsCOORDINATE2* coord1,
                                  SVECTOR* rot, q19_12 posX, q19_12 posY, q19_12 posZ, s_WaterZone* waterZones);

/** Light function. */
void func_80055648(s32 lightIntensity, const SVECTOR* dir);

s32 func_800557DC(void);

void func_80055814(s32 arg0);

void WorldEnvRender_FogDistanceSet(q19_12 nearDist, q19_12 farDist);

s32 func_800559A8(s32 arg0);

u8 func_80055A50(s32 arg0);

void func_80055A90(CVECTOR* arg0, CVECTOR* arg1, u8 arg2, s32 arg3);

/** @brief Applies uniform lighting and fog shading to `color`, outputting to `result`.
 * The fog factor is derived from `arg2` via `g_WorldEnvRenderWork.fogRamp` and blends the lit color toward the far/fog color in `g_WorldEnvRenderWork.field_1C/field_1D/field_1E`,
 * with lighting strength from `g_WorldEnvRenderWork.field_20` applied equally on all axes.
 */
void func_80055B74(CVECTOR* result, CVECTOR* color, s32 arg2);

void func_80055C3C(CVECTOR* result, CVECTOR* color, s32 arg2, s32 arg3, s32 arg4, s32 arg5);

u8 func_80055D78(q19_12 posX, q19_12 posY, q19_12 posZ);

void func_80055E90(CVECTOR* color, u8 fadeAmount);

void func_80055ECC(CVECTOR* color, SVECTOR3* arg1, SVECTOR3* arg2, MATRIX* worldMat);

u8 func_80055F08(SVECTOR3* arg0, SVECTOR3* arg1, MATRIX* worldMat);

void Lm_HeaderPtrsInit(s_LmHeader* lmHdr);

void ModelHeader_FixOffsets(s_ModelHeader* modelHdr, s_LmHeader* lmHdr);

#endif
