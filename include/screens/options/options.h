#ifndef _SCREENS_OPTIONS_H
#define _SCREENS_OPTIONS_H

// ======
// ENUMS
// ======

/** @brief Options menu states.
 *
 * When the user navigates an options menu, this facilitates menu switching
 * via `s_GameWork::gameStateSteps[0]`.
 */
typedef enum _OptionsMenuState
{
    OptionsMenuState_EnterMainOptions  = 0,  /** Entering main options menu. */
    OptionsMenuState_MainOptions       = 1,  /** In main options menu. */
    OptionsMenuState_ScreenPos         = 2,  /** In screen position menu. */
    OptionsMenuState_Brightness        = 3,  /** In brightness menu. */
    OptionsMenuState_Controller        = 4,  /** In controller config menu. */
    OptionsMenuState_Leave             = 5,  /** Leaving options menu back to gameplay. */
    OptionsMenuState_LeaveMainOptions  = 6,  /** Leaving main options menu. */
    OptionsMenuState_EnterScreenPos    = 7,  /** Entering screen position menu. */
    OptionsMenuState_EnterBrightness   = 8,  /** Entering brightness menu. */
    OptionsMenuState_EnterController   = 9,  /** Entering controller config menu. */
    OptionsMenuState_LeaveScreenPos    = 10, /** Leaving screen position menu. */
    OptionsMenuState_LeaveBrightness   = 11, /** Leaving brightness menu. */
    OptionsMenuState_LeaveController   = 12, /** Leaving controller config menu. */
    OptionsMenuState_EnterExtraOptions = 13, /** Entering extra options menu. */
    OptionsMenuState_ExtraOptions      = 14, /** In extra options menu. */
    OptionsMenuState_LeaveExtraOptions = 15  /** Leaving extra options menu. */
} e_OptionsMenuState;

/** @brief Main options menu entries. */
typedef enum _MainOptionsMenuEntry
{
    MainOptionsMenuEntry_Exit           = 0,
    MainOptionsMenuEntry_Brightness     = 1,
    MainOptionsMenuEntry_Controller     = 2,
    MainOptionsMenuEntry_ScreenPosition = 3,
    MainOptionsMenuEntry_Vibration      = 4,
    MainOptionsMenuEntry_AutoLoad       = 5,
    MainOptionsMenuEntry_Sound          = 6,
    MainOptionsMenuEntry_BgmVolume      = 7,
    MainOptionsMenuEntry_SfxVolume      = 8,

#if VERSION_REGION_IS(PAL)
    MainOptionsMenuEntry_Language       = 9,
#endif

    MainOptionsMenuEntry_Count
} e_MainOptionsMenuEntry;

/** @brief Extra options menu entries. */
typedef enum _ExtraOptionsMenuEntry
{
    ExtraOptionsMenuEntry_WeaponCtrl   = 0,
    ExtraOptionsMenuEntry_Blood        = 1,
    ExtraOptionsMenuEntry_ViewCtrl     = 2,
    ExtraOptionsMenuEntry_RetreatTurn  = 3,
    ExtraOptionsMenuEntry_MovementCtrl = 4,
    ExtraOptionsMenuEntry_AutoAiming   = 5,
    ExtraOptionsMenuEntry_ViewMode     = 6,
    ExtraOptionsMenuEntry_BulletMult   = 7,

    ExtraOptionsMenuEntry_Count        = 8
} e_ExtraOptionsMenuEntry;

/** @brief Blood color menu entries. */
typedef enum _BloodColorMenuEntry
{
    BloodColorMenuEntry_Normal = 0,
    BloodColorMenuEntry_Green  = 1,
    BloodColorMenuEntry_Violet = 2,
    BloodColorMenuEntry_Black  = 3,

    BloodColorMenuEntry_Count  = 4
} e_BloodColorMenuEntry;

typedef enum _BloodColor
{
    BloodColor_Normal = 0,
    BloodColor_Green  = 2,
    BloodColor_Violet = 5,
    BloodColor_Black  = 11
} e_BloodColor;

/** @brief Screen position menu states.
 *
 * When the user navigates the screen position menu, this facilitates menu switching
 * via `s_GameWork::gameStateSteps[1]`.
 */
typedef enum _ScreenPosMenuState
{
    ScreenPosMenuState_0     = 0,
    ScreenPosMenuState_1     = 1,
    ScreenPosMenuState_2     = 2,
    ScreenPosMenuState_Leave = 3
} e_ScreenPosMenuState;

/** @brief Controller config menu states. Also used as entries for the left presets pane.
 *
 * When the user navigates the controller config menu, this facilitates menu switching
 * via `s_GameWork::gameStateSteps[1]`.
 */
typedef enum _ControllerMenuState
{
    ControllerMenuState_Leave   = -2,
    ControllerMenuState_Actions = -1,
    ControllerMenuState_Exit    = 0,
    ControllerMenuState_Type1   = 1,
    ControllerMenuState_Type2   = 2,
    ControllerMenuState_Type3   = 3,

    ControllerMenuState_Count   = 4
} e_ControllerMenuState;

/** @brief Input actions. Also used as controller config menu entries for the right pane. */
typedef enum _InputAction
{
    InputAction_Enter  = 0,
    InputAction_Cancel = 1,
    InputAction_Skip   = 2,
    InputAction_Action = 3,
    InputAction_Aim    = 4,
    InputAction_Light  = 5,
    InputAction_Run    = 6,
    InputAction_View   = 7,
    InputAction_StepL  = 8,
    InputAction_StepR  = 9,
    InputAction_Pause  = 10,
    InputAction_Item   = 11,
    InputAction_Map    = 12,
    InputAction_Option = 13,

    InputAction_Count  = 14
} e_InputAction;

/** @brief Brightness menu states.
 *
 * When the user navigates the brightness menu, this facilitates menu switching
 * via `s_GameWork::gameStateSteps[1]`.
 */
typedef enum _BrightnessMenuState
{
    BrightnessMenuState_Enter     = 0,
    BrightnessMenuState_StartFade = 1,
    BrightnessMenuState_SetConfig = 2,
    BrightnessMenuState_Leave     = 3
} e_BrightnessMenuState;

// ========
// STRUCTS
// ========

/** @brief Selected controller options menu column entries. */
typedef struct _ControllerMenu_SelectedEntries
{
    /* 0x0 */ e_ControllerMenuState preset;
    /* 0x4 */ e_InputAction         action;
} s_ControllerMenu_SelectedEntries;

// ==========
// FUNCTIONS
// ==========

/** @brief Options menu game state handler. */
void GameState_Options_Update(void);

/** @brief Controller for the extra options menu. */
void Options_ExtraOptionsMenu_Control(void);

/** @brief Controller for the main options menu. Handles menu states, user input, and graphics draw calls. */
void Options_MainOptionsMenu_Control(void);

/** @brief Draws a BGM volume bar in the main options menu. */
void Options_MainOptionsMenu_BgmVolumeBarDraw(void);

/** @brief Draws an SFX volume bar in the main options menu. */
void Options_MainOptionsMenu_SfxVolumeBarDraw(void);

/** @brief Draws a volume bar.
 *
 * Called by `Options_MainOptionsMenu_BgmVolumeBarDraw` and `Options_MainOptionsMenu_SfxVolumeBarDraw`.
 */
void Options_MainOptionsMenu_VolumeBarDraw(bool isSfx, u8 vol);

/** @brief Draws the heading and all listed entry strings in the extra options menu. */
void Options_ExtraOptionsMenu_EntryStringsDraw(void);

/** @brief Draws the heading and all listed entry strings in the main options menu. */
void Options_MainOptionsMenu_EntryStringsDraw(void);

/** @brief Draws gold bullet points next to the listed entries and a highlight indicating the
 * selected entry in the extra options menu.
 */
void Options_ExtraOptionsMenu_SelectionHighlightDraw(void);

/** @brief Draws gold bullet points next to the listed entries and a highlight indicating the
 * selected entry in the main options menu.
 */
void Options_MainOptionsMenu_SelectionHighlightDraw(void);

/** @brief Draws the background of Harry in the main and extra options menus. */
void Options_Menu_BackgroundDraw(void);

/** @brief Draws configuration strings and blue arrows to the right of the listed entries in the extra options menu. */
void Options_ExtraOptionsMenu_ConfigDraw(void);

/** @brief Draws configuration strings and blue arrows to the right of the listed entries in the main options menu. */
void Options_MainOptionsMenu_ConfigDraw(void);

/** @brief Controller for the screen position options menu. Handles menu states, user input, SFX, and graphics drawing. */
void Options_ScreenPosMenu_Control(void);

/** @brief Draws the arrows at the edges of the screen indicating input directions in the screen position options menu.
 *
 * Called by `Options_ScreenPosMenu_Control`.
 */
void Options_ScreenPosMenu_ArrowsDraw(void);

/** @brief Draws the screen position configuration string and its background box. */
void Options_ScreenPosMenu_ConfigDraw(void);

/** @brief Controller for the brightness options menu.
 *
 * Handles menu states, user input, SFX, and graphics drawing.
 */
void Options_BrightnessMenu_Control(void);

/** @brief Draws the string indicating the light level in the brightness options menu. */
void Options_BrightnessMenu_ConfigDraw(void);

/** @brief Draws arrows on the left and right edges of the screen indicating input directions in the brightness options menu.
 *
 * Called by `Options_BrightnessMenu_Control`.
 */
void Options_BrightnessMenu_ArrowsDraw(void);

/** @brief Draws a scaling entry selection highlight in the main and extra options menus.
 *
 * Called by `Options_MainOptionsMenu_SelectionHighlightDraw` and `Options_ExtraOptionsMenu_SelectionHighlightDraw`.
 *
 * @param line Reference line for the highlight position and width.
 * @param hasShadow @unused `true` for a highlight with a shadow and a line, `false` for a line only. Always passed as `true`.
 * @param invertGradient @unused `true` to invert the line gradient, `false` to not invert. Non-functional.
 */
void Options_Selection_HighlightDraw(const s_Line2d* line, bool hasShadow, bool invertGradient);

/** @brief Draws a blue arrow element used for certain listed entries in the main and extra options menus.
 *
 * @note Called twice if the arrow requires a border, with `isFlashing` passed as `true` and `false` on consecutive calls.
 *
 * @param arrow 2D polygon of the arrow element.
 * @param isFlashing `true` for a flashing element with a gradient, `false` for a border.
 * @param resetColor @unused No meaningful purpose.
*/
void Options_Selection_ArrowDraw(const s_Triangle2d* arrow, bool isFlashing, bool resetColor);

/** @brief Draws a gold bullet point element used next to listed entries in the main and extra options menus.
 *
 * @note Called twice to build a whole bullet point, with `isBorder` passed as `true` and `false` on consecutive calls.
 *
 * @param quad 2D polygon of the bullet point element.
 * @param isBorder `true` for a border quad, `false` for a quad.
 * @param isInactive `true` if the entry is unselected, `false` if selected.
 */
void Options_Selection_BulletPointDraw(const s_Quad2d* quad, bool isBorder, bool isInactive);

#if VERSION_REGION_IS(NTSCJ)
void func_801E6F1C(RECT* rect);
void func_801E749C(void);
void func_801E794C(void);
#endif

/** @brief Controller for the controller configuration options menu.
 *
 * Handles the menu state, user input, SFX, and graphics drawing.
 */
void Options_ControllerMenu_Control(void);

/** @brief Changes the button mapping based on the input and updates the configuration.
 *
 * @param actionIdx Index of the input action for which to update the controller configuration.
 */
s32 Options_ControllerMenu_ConfigUpdate(s32 actionIdx);

/** @brief Draws entry strings and icons in the controller configuration options menu.
 *
 * @param isOnRightPane `true` if on the right pane, `false` if on the left pane.
 * @param presetsEntryIdx Selected presets pane entry index.
 * @param actionsEntryIdx Selected actions pane entry index.
 * @param boundActionIdx Index of the newly bound input action.
 */
void Options_ControllerMenu_EntriesDraw(bool isOnRightPane, s32 presetsEntryIdx, s32 actionsEntryIdx, s32 boundActionIdx);

/** @brief Draws button mapping icons in the controller configuration options menu.
 *
 * Called by `Options_ControllerMenu_EntriesDraw`.
 *
 * @param baseX Base X position at which to begin drawing.
 * @param baseY Base Y position at which to begin drawing.
 * @param config Controller configuration flags.
 */
void Options_ControllerMenu_ButtonIconsDraw(s32 baseX, s32 baseY, u16 config);

#endif
