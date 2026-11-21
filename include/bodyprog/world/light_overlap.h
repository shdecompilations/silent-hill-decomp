#ifndef _BODYPROG_WORLD_LIGHTOVERLAP_H
#define _BODYPROG_WORLD_LIGHTOVERLAP_H

enum _CharaId;

/** @brief Light overlap primitives.
 *
 * Drawn as two layers. The additive layer is a glow fan centered on the light's screen position. The subtractive
 * layer darkens by the world tint: three gradient rings around the glow, then quads out to the screen edge.
 */
typedef struct _LightOverlapPrims
{
    /* 0x0   */ DR_TPAGE tpages[2]; /** 0: subtractive layer, 1: additive layer. */
    /* 0x10  */ POLY_G3  fan[16];   /** Additive glow. Light color at the center to black at ring 0. */
    /* 0x1D0 */ POLY_G4  rings[48]; /** Subtractive gradient. 3 rings of 16: ring N to ring N + 1. */
    /* 0x890 */ POLY_F4  edge[16];  /** Subtractive full tint from ring 3 to the screen edge. */
} s_LightOverlapPrims;

/** @brief Checks if a character's model is set in a `g_WorldGfxWork` model slot.
 *
 * @note May still return `true` while the model is being loaded.
 *
 * @param charaId ID of the character to check.
 * @return `true` if a slot holds the character's model, `false` otherwise.
 */
bool WorldGfx_CharaModelAssignedCheck(enum _CharaId charaId);

/** @brief Sets up the light overlap primitives in both frame buffers.
 *
 * Sets the primitive codes, transparency, layer tpages, and the edge's screen border corners.
 * Called by `WorldEnvRender_Init`.
 */
void WorldEnvRender_LightOverlapInit(void);

/** @brief Sets the light overlap colors in both frame buffers.
 *
 * The glow fades from the light color to black, then the subtractive rings step up to the full world tint in thirds.
 *
 * @param lightColorR Red glow color at the light's screen position.
 * @param lightColorG Green glow color at the light's screen position.
 * @param lightColorB Blue glow color at the light's screen position.
 * @param worldTintR Red world tint subtracted outside the glow.
 * @param worldTintG Green world tint subtracted outside the glow.
 * @param worldTintB Blue world tint subtracted outside the glow.
 */
void WorldEnvRender_WorldLightTintParamSet(u8 lightColorR, u8 lightColorG, u8 lightColorB,
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
 * `WorldEnvRender_LightScreenAngleGet` and screen position with `WorldEnvRender_LightScreenPositionGet`.
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
void WorldEnvRender_LightScreenInfoGet(VECTOR3* scr_l_pos, q19_12* scr_l_ang_xy, q19_12* scr_l_ang_z,
                                       SVECTOR* world_light_vec, const VECTOR3* world_light_pos);

/** @brief Computes a light's screen angles from its view-space position and direction.
 *
 * @note SH2 name: `shLensFlareMakeScreenAngle`.
 *
 * @param scr_l_ang_xy Output screen-space direction of the plane containing the eye-to-light ray and the light direction.
 * @param scr_l_ang_z Output angle between the eye-to-light ray and the light direction.
 * @param ws_l_sxyz_p View-space light position (Q19.12).
 * @param ws_l_vec_p View-space light direction (Q19.12).
 */
void WorldEnvRender_LightScreenAngleGet(q19_12* scr_l_ang_xy, q19_12* scr_l_ang_z,
                                        const VECTOR* ws_l_sxyz_p, const VECTOR* ws_l_vec_p);

/** @brief Projects a light's view-space position to the screen.
 *
 * @note SH2 name: `shLensFlareMakeScreenPos`.
 *
 * @param scr_l_pos Output screen position. Z is the view-space depth after moving the light.
 * @param ws_l_sxyz_p View-space light position (Q19.12).
 * @param ws_l_vec_p View-space light direction (Q19.12).
 * @param scr_z Screen distance, as from `ReadGeomScreen`.
 */
void WorldEnvRender_LightScreenPositionGet(VECTOR3* scr_l_pos,
                                           const VECTOR* ws_l_sxyz_p, const VECTOR* ws_l_vec_p, s32 scr_z);

/** @brief Builds and adds the tint light overlap polygons for a projected light.
 *
 * @param ot Ordering table to add the polygons to.
 * @param scr_l_pos Light screen position (from `WorldEnvRender_LightScreenInfoGet`).
 * @param intensity Light intensity.
 * @param scr_l_ang_xy Light screen direction angle (from `WorldEnvRender_LightScreenInfoGet`).
 * @param scr_l_ang_z Angle between the eye-to-light ray and the light direction (from `WorldEnvRender_LightScreenInfoGet`).
 */
void WorldEnvRender_LightOverlapPolysDraw(GsOT* ot, const VECTOR3* scr_l_pos, q19_12 intensity,
                                          q19_12 scr_l_ang_xy, q19_12 scr_l_ang_z);

#endif
