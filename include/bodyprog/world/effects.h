#ifndef _BODYPROG_WORLD_EFFECTS_H
#define _BODYPROG_WORLD_EFFECTS_H

/** @brief Used by `func_8003F654` to cast a specific field to the desired type. */
typedef enum _PrimitiveType
{
    PrimitiveType_None = 0,
    PrimitiveType_S8   = 1,
    PrimitiveType_U8   = 2,
    PrimitiveType_S16  = 3,
    PrimitiveType_U16  = 4,
    PrimitiveType_S32  = 5
} e_PrimitiveType;

/** @brief World environment flags. Used by `s_WorldEnvPreset::flags`. */
typedef enum _WorldEnvFlags
{
    WorldEnvFlag_None              = 0,
    WorldEnvFlag_Dark              = 1 << 0, /** Set on the "flashlight off" preset in dark areas. Lots of map overlay code check this to tell if area is currently in darkness. (TODO: Macro for checking this?) */
    WorldEnvFlag_FlashlightAllowed = 1 << 1,
    WorldEnvFlag_EnableBrightness  = 1 << 2, /** Applies the user's brightness setting. */
    WorldEnvFlag_UseLighter        = 1 << 3,
    WorldEnvFlag_EnableLensFlare   = 1 << 4
} e_WorldEnvFlags;

/** @brief World environment light modes. Determines how models and billboards are lit by the dynamic light.
 * Used by `s_WorldEnvPreset::lightMode`.
 */
typedef enum _WorldEnvLightMode
{
    WorldEnvLightMode_None        = 0, /** No dynamic light. Flat vertex colors nad fog drawn as overlay polygons. */
    WorldEnvLightMode_Spot        = 1, /** Positional light (normally the flashlight). Enables the lens flare. */
    WorldEnvLightMode_Directional = 2  /** Lights everywhere from one direction. No source point or distance falloff. */
} e_WorldEnvLightMode;

/** @brief World environment fog modes. Used by`s_WorldEnvPreset::fogMode`. */
typedef enum _WorldEnvFogMode
{
    WorldEnvFogMode_Off         = 0,
    WorldEnvFogMode_On          = 1,
    WorldEnvFogMode_Zero        = 2, /** @unused Fog on with a distance of 0. No preset sets it. */
    WorldEnvFogMode_FollowLight = 3  /** @unused Fog distance follows the light position. Only set by preset 5, which nothing selects? */
} e_WorldEnvFogMode;

/** @brief Checks if the world environment is in darkness, without the flashlight turned on.
 *
 * @return `WorldEnvFlag_Dark` if the work environment is in darkness, `false` otherwise.
 */
#define WorldEnv_IsInDarkness() \
    (g_SysWork.worldEnv.current.params.flags & WorldEnvFlag_Dark)

/** @brief Checks if the world environment allows the flashlight to be used.
 *
 * @return `WorldEnvFlag_FlashlightAllowed` if the flashlight can be used, `false` otherwise.
 */
#define WorldEnv_IsFlashlightAllowed() \
    (g_SysWork.worldEnv.current.params.flags & WorldEnvFlag_FlashlightAllowed)

/** @brief Checks if the flashlight is turned on.
 *
 * @return `true` if the flashlight is turned on, `false` otherwise.
 */
#define WorldEnv_IsFlashlightOn() \
    (g_SysWork.worldEnv.isFlashlightOn)

/** @brief Checks if any active world environment presets can count as in darkness.
 *
 * @param `true`, otherwise `false`.
 */
#define WorldEnv_CanBeInDarkness()                                          \
    ((g_SysWork.worldEnv.transition[0].params.flags & WorldEnvFlag_Dark) || \
     (g_SysWork.worldEnv.transition[1].params.flags & WorldEnvFlag_Dark))

/** @brief Checks if the world is lit by any of the following conditions:
 * - The flashlight beign disallowed in a lit area. 
 * - The flashlight being turned on.
 * - The world being unable to turn dark.
 *
 * @return `true` if the world is lit, `false` otherwise.
 */
#define WorldEnv_IsWorldLit() \
    (!WorldEnv_IsFlashlightAllowed() || WorldEnv_IsFlashlightOn() || !WorldEnv_CanBeInDarkness())

/** Loads a flame graphic. */
void GameFs_FlameGfxLoad(void);

void Game_SpotlightLoadScreenAttribsSet(void);

/** @brief Adjust light point attributes to make it simile a flashlight. */
void Game_FlashlightAttribsSet(void);

/** @brief Determines what enviroment effects data from `WORLD_ENV_PRESETS` will use based on
 * `s_MapOverlayHdr::field_16`.
 */
void WorldEnv_MapPresetSet(s_MapOverlayHdr* mapHdr);

/** @brief Toggles the player's flashlight on. */
void Game_TurnFlashlightOn(void);

/** @brief Toggles the player's flashlight off. */
void Game_TurnFlashlightOff(void);

/** @brief Toggles the player's flashlight. */
void Game_FlashlightToggle(void);

/** @brief Checks if the player's flashlight is on.
 *
 * @return `true` if the flashlight is on, `false` otherwise.
 */
bool Game_FlashlightIsOn(void);

void Gfx_MapInitMapEffectsUpdate(s32 idx0, s32 idx1);

void func_8003EDA8(void);

void WorldEnv_SnowRainColorGet(CVECTOR* snowColor, CVECTOR* rainColor);

/** @brief @unused */
void func_8003EE30(s32 arg0, s8* arg1, s32 arg2, s32 arg3);

void WorldEnv_LoadScreenPresetsSet(s32 arg0, s32 arg1);

void WorldEnv_TransitionStart(s32 idx0, s32 idx1, e_PrimitiveType primType, void* primData, s32 arg4, s32 arg5);

void WorldEnv_TransitionStartFromPresets(const s_WorldEnvPreset* preset0, const s_WorldEnvPreset* preset1,
                                         e_PrimitiveType primType, void* primData, s32 arg4, s32 arg5);

void WorldEnv_StateFromPreset(s_WorldEnvState* arg0, const s_WorldEnvPreset* preset);

void func_8003FE04(const s_WorldEnvPreset* preset0, const s_WorldEnvPreset* preset1, const s_WorldEnvPreset* preset2, q19_12 alphaTo);

s32 func_8003FEC0(const s_WorldEnvPreset* preset);

void WorldEnv_Apply(s_WorldEnvState* state);

#endif
