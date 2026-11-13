#ifndef _BODYPROG_WORLD_LIGHTING_H
#define _BODYPROG_WORLD_LIGHTING_H

bool Chara_ModelLoadedCheck(e_CharaId charaId);

/** Related to the screen. Called by `WorldEnvRender_Init`. */
void func_80040BAC(void);

void WorldEnvRender_WorldLightTintParamSet(u8 lightPosR, u8 lightPosG, u8 lightPosB,
                                           u8 worldTintR, u8 worldTintG, u8 worldTintB);

/** @brief Draws the tint light overlap effect for a light.
 *
 * Projects the light to the screen, then draws the overlay polygons from its screen position and angles.
 *
 * @param ot Ordering table to add the polygons to.
 * @param intensity Light intensity.
 * @param world_light_vec World-space light direction.
 * @param world_light_pos World-space light position (Q19.12).
 */
void WorldEnvRender_LightOverlapDraw(GsOT* ot, q19_12 intensity, SVECTOR* world_light_vec, const VECTOR3* world_light_pos);

/** @brief Gets a light's screen position and screen angles.
 *
 * Transforms the light's position and direction into view space, then computes its screen angles with
 * `WorldEnvRender_LightScreenAngleGet` and screen position with `WorldEnvRender_LightScreenPosGet`.
 * Also used by the lens flare.
 *
 * @note SH2 name: `shLensFlareMakeScreenInfo`. SH2 moved this into its lens flare code.
 *
 * @param scr_l_pos Output screen position. Z is the view-space depth.
 * @param scr_l_ang_xy Output screen-space direction of the plane containing the eye-to-light ray and the light direction.
 * @param scr_l_ang_z Output angle between the eye-to-light ray and the light direction.
 * @param world_light_vec World-space light direction.
 * @param world_light_pos World-space light position (Q19.12).
 */
void WorldEnvRender_LightScreenInfoGet(VECTOR3* scr_l_pos, q19_12* scr_l_ang_xy, q19_12* scr_l_ang_z, SVECTOR* world_light_vec, const VECTOR3* world_light_pos);

/** @brief Computes a light's screen angles from its view-space position and direction.
 *
 * @note SH2 name: `shLensFlareMakeScreenAngle`.
 *
 * @param scr_l_ang_xy Output screen-space direction of the plane containing the eye-to-light ray and the light direction.
 * @param scr_l_ang_z Output angle between the eye-to-light ray and the light direction.
 * @param ws_l_sxyz_p View-space light position (Q19.12).
 * @param ws_l_vec_p View-space light direction (Q19.12).
 */
void WorldEnvRender_LightScreenAngleGet(q19_12* scr_l_ang_xy, q19_12* scr_l_ang_z, const VECTOR* ws_l_sxyz_p, const VECTOR* ws_l_vec_p);

/** @brief Projects a light's view-space position to the screen.
 *
 * @note SH2 name: `shLensFlareMakeScreenPos`.
 *
 * @param scr_l_pos Output screen position. Z is the view-space depth after moving the light.
 * @param ws_l_sxyz_p View-space light position (Q19.12).
 * @param ws_l_vec_p View-space light direction (Q19.12).
 * @param scr_z Screen distance, as from `ReadGeomScreen`.
 */
void WorldEnvRender_LightScreenPosGet(VECTOR3* scr_l_pos, const VECTOR* ws_l_sxyz_p, const VECTOR* ws_l_vec_p, s32 scr_z);

/** @brief Builds and adds the tint light overlap polygons for a projected light.
 *
 * @param ot Ordering table to add the polygons to.
 * @param scr_l_pos Light screen position, from `WorldEnvRender_LightScreenInfoGet`.
 * @param intensity Light intensity.
 * @param scr_l_ang_xy Light screen direction angle, from `WorldEnvRender_LightScreenInfoGet`.
 * @param scr_l_ang_z Angle between the eye-to-light ray and the light direction, from `WorldEnvRender_LightScreenInfoGet`.
 */
void WorldEnvRender_LightOverlapPolysDraw(GsOT* ot, const VECTOR3* scr_l_pos, q19_12 intensity, q19_12 scr_l_ang_xy, q19_12 scr_l_ang_z);

#endif
