#ifndef _BODYPROG_SCREEN_FADE_H
#define _BODYPROG_SCREEN_FADE_H

/** @brief Screen fade states used by `g_Screen_FadeStatus`. The flow is not linear. */
typedef enum _ScreenFadeState
{
    ScreenFadeState_Reset           = 0,
    ScreenFadeState_None            = 1,
    ScreenFadeState_FadeOutStart    = 2,
    ScreenFadeState_FadeOutSteps    = 3,
    ScreenFadeState_ResetTimestep   = 4,
    ScreenFadeState_FadeOutComplete = 5,
    ScreenFadeState_FadeInStart     = 6,
    ScreenFadeState_FadeInSteps     = 7
} e_ScreenFadeState;

/** @brief Active screen fade effect data. Uses `e_ScreenFadeState`.
 *
 * Data is packed as follows:
 * State as `e_ScreenFadeState`: bits 0-2.
 * Is white:                     bit 3.
 */
extern s32 g_Screen_FadeStatus;

/** @brief Packs a screen fade status containing a fade state and white flag.
 * See `g_Screen_FadeStatus` for bit layout.
 *
 * @param state Screen fade state.
 * @param isWhite White status (`bool`).
 * @return Packed screen fade status containing a fade state and white flag.
 */
#define SCREEN_FADE_STATUS(state, isWhite) \
    ((state) | ((isWhite) ? (1 << 3) : 0))

/** @brief Checks if the screen fade is white.
 * See `g_Screen_FadeStatus` for bit layout.
 *
 * @param fadeStatus Packed screen fade status containing a fade state and white flag.
 * @return `true` if white, `false` if black.
 */
#define IS_SCREEN_FADE_WHITE(fadeStatus) \
    ((fadeStatus) & (1 << 3))

/** @brief Checks if the screen fade is not in progress (finished step) by masking away the color bit.
 *
 * @return `true` if finished, `false` if still in progress.
 */
#define Screen_IsFadeFinished() \
    ((g_Screen_FadeStatus & 0x7) == ScreenFadeState_FadeOutComplete)

/** @brief Checks if the screen fade is not in progress (idle step) without masking away the color bit.
 *
 * @return `true` if idle, `false` otherwise.
 */
#define Screen_IsFadeNone() \
    (g_Screen_FadeStatus == ScreenFadeState_None)

/** @brief Starts a screen fade in/out.
 *
 * @param reset `true` to reset fade progress to 0, `false` to keep it.
 *              Speculation: Skipping the screen fade progress step is a mistake. It still works because once a fade
 *              is finished, the progress variable will be reset to 0 anyway.
 * @param fadeIn `true` for fade in, `false` for fade out.
 * @param isWhite `true` for white fade, `false` for black fade.
 */
#define Screen_FadeStart(reset, fadeIn, isWhite)                                                                \
    g_Screen_FadeStatus = (((((reset) == true) ? ScreenFadeState_FadeOutStart : ScreenFadeState_FadeOutSteps) + \
                            (((fadeIn) == true) ? (1 << 2) : 0)) |                                              \
                           (((isWhite) == true) ? (1 << 3) : 0))

/** @brief Resets the screen fade. */
#define Screen_FadeReset() \
    g_Screen_FadeStatus = ScreenFadeState_Reset

/** @brief Resets the custom screen fade timestep back to zero, disregarding the color bit. */
#define Screen_FadeTimestepReset() \
    g_Screen_FadeStatus = ScreenFadeState_ResetTimestep

/** @brief Gets the screen fade effect's progress.
 *
 * @return Fade progress.
 */
q19_12 Screen_FadeProgressGet(void);

/** @brief Updates the screen fade effect. */
void Screen_FadeUpdate(void);

#endif
