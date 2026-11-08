#ifndef _BODYPROG_GFX_MAPEFFECTS_H
#define _BODYPROG_GFX_MAPEFFECTS_H

void Options_BrightnessMenu_LinesDraw(s32 brightness);

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

// https://decomp.me/scratch/XWo1G
void WorldEnv_Update(void);

/** @brief Computes the weighted average of `a` and `b`.
 *
 * @param a First value.
 * @param b Second value.
 * @param weight Weight as a fixed-point alpha in Q3.12, range `[0, 4096]`.
 * @return Weighted average of `a` and `b`.
 */
q19_12 Math_WeightedAverageGet(s32 a, s32 b, q19_12 weight);

void func_8003FE04(const s_WorldEnvPreset* arg0, const s_WorldEnvPreset* arg1, const s_WorldEnvPreset* arg2, q19_12 alphaTo);

s32 func_8003FEC0(const s_WorldEnvPreset* arg0);

void func_8003FF2C(s_WorldEnvState* arg0);

#endif
