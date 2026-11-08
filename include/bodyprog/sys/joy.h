#ifndef _BODYPROG_SYS_JOY_H
#define _BODYPROG_SYS_JOY_H

#include "bodyprog/math/math.h"

/** @brief Handles the general controller functionality, controller IO stream, and state.
 * Game controller logic is split between different parts of the game depending on what the user is using.
 * For example, controller logic in the inventory screen is handled separately from in-game controller logic.
 */

#define INPUT_ACTION_COUNT   14
#define CONTROLLER_COUNT_MAX 2
#define STICK_DEADZONE       FP_STICK(0.5f)

/** @brief Checks controller 1 button flags against a mask of `e_ControllerFlags`. */
#define Pad_ButtonClicked(buttons)   (g_Controller0->buttonFlags.clicked & (buttons))
#define Pad_ButtonHeld(buttons)      (g_Controller0->buttonFlags.held & (buttons))
#define Pad_ButtonPulsed(buttons)    (g_Controller0->buttonFlags.pulsed & (buttons))
#define Pad_ButtonPulsedGui(buttons) (g_Controller0->buttonFlags.pulsedGui & (buttons))

/** @brief Checks controller 2 held button flags against a mask of `e_ControllerFlags`. */
#define Pad2_ButtonHeld(buttons)     (g_Controller1->buttonFlags.held & (buttons))

/** @brief Gets the buttons bound to an input action, e.g. `Pad_BindingGet(cancel)`. */
#define Pad_BindingGet(action) (g_GameWorkPtr->config.controllerConfig.action)

/** @brief Checks controller 1 button flags against the buttons bound to an input action. */
#define Pad_ButtonClickedBind(action) Pad_ButtonClicked(Pad_BindingGet(action))
#define Pad_ButtonHeldBind(action)    Pad_ButtonHeld(Pad_BindingGet(action))

/** @brief PSX controller input flags. */
typedef enum _ControllerFlags
{
    ControllerFlag_None            = 0,
    ControllerFlag_Select          = 1 << 0,
    ControllerFlag_L3              = 1 << 1,
    ControllerFlag_R3              = 1 << 2,
    ControllerFlag_Start           = 1 << 3,
    ControllerFlag_DpadUp          = 1 << 4,
    ControllerFlag_DpadRight       = 1 << 5,
    ControllerFlag_DpadDown        = 1 << 6,
    ControllerFlag_DpadLeft        = 1 << 7,
    ControllerFlag_L2              = 1 << 8,
    ControllerFlag_R2              = 1 << 9,
    ControllerFlag_L1              = 1 << 10,
    ControllerFlag_R1              = 1 << 11,
    ControllerFlag_Triangle        = 1 << 12,
    ControllerFlag_Circle          = 1 << 13,
    ControllerFlag_Cross           = 1 << 14,
    ControllerFlag_Square          = 1 << 15,
    ControllerFlag_LStickLowUp     = 1 << 16,
    ControllerFlag_LStickLowRight  = 1 << 17,
    ControllerFlag_LStickLowDown   = 1 << 18,
    ControllerFlag_LStickLowLeft   = 1 << 19,
    ControllerFlag_RStickLowUp     = 1 << 20,
    ControllerFlag_RStickLowRight  = 1 << 21,
    ControllerFlag_RStickLowDown   = 1 << 22,
    ControllerFlag_RStickLowLeft   = 1 << 23,
    ControllerFlag_LStickHighUp    = 1 << 24,
    ControllerFlag_LStickHighRight = 1 << 25,
    ControllerFlag_LStickHighDown  = 1 << 26,
    ControllerFlag_LStickHighLeft  = 1 << 27,
    ControllerFlag_RStickHighUp    = 1 << 28,
    ControllerFlag_RStickHighRight = 1 << 29,
    ControllerFlag_RStickHighDown  = 1 << 30,
    ControllerFlag_RStickHighLeft  = 1 << 31,

    ControllerFlag_FaceButtons = ControllerFlag_Select | ControllerFlag_L3 | ControllerFlag_R3 | ControllerFlag_Start |
                                 ControllerFlag_DpadUp | ControllerFlag_DpadRight | ControllerFlag_DpadDown | ControllerFlag_DpadLeft |
                                 ControllerFlag_L2 | ControllerFlag_R2 | ControllerFlag_L1 | ControllerFlag_R1 |
                                 ControllerFlag_Triangle | ControllerFlag_Circle | ControllerFlag_Cross | ControllerFlag_Square,
    ControllerFlag_LowSticks   = ControllerFlag_LStickLowUp | ControllerFlag_LStickLowRight | ControllerFlag_LStickLowDown | ControllerFlag_LStickLowLeft |
                                 ControllerFlag_RStickLowUp | ControllerFlag_RStickLowRight | ControllerFlag_RStickLowDown | ControllerFlag_RStickLowLeft,
    ControllerFlag_HighSticks  = ControllerFlag_LStickHighUp | ControllerFlag_LStickHighRight | ControllerFlag_LStickHighDown | ControllerFlag_LStickHighLeft |
                                 ControllerFlag_RStickHighUp | ControllerFlag_RStickHighRight | ControllerFlag_RStickHighDown | ControllerFlag_RStickHighLeft,
} e_ControllerFlags;

/** @brief PSX controller analog stick states. */
typedef union
{
    /* 0x0 */ u32 rawData_0;
              struct
              {
                  /* 0x0 */ s8 rightX;
                  /* 0x1 */ s8 rightY;
                  /* 0x2 */ s8 leftX;
                  /* 0x3 */ s8 leftY;
    /* 0x4 */ } sticks_0; // Normalized range: `[-128, 127]`.
} s_AnalogSticks;

/** @brief Analog PSX controller state. */
typedef struct _AnalogController
{
    /* 0x0 */ u8  status;
    /* 0x1 */ u8  receivedBytes : 4; /** Number of bytes received / 2. */
    /* 0x2 */ u8  terminalType  : 4; /** `e_PadTerminalType` */
    /* 0x3 */ u16 buttonFlags;       /** `e_ControllerFlags` */
    /* 0x5 */ u8  rightX;
    /* 0x6 */ u8  rightY;
    /* 0x7 */ u8  leftX;
    /* 0x8 */ u8  leftY;
} s_AnalogController;
STATIC_ASSERT_SIZEOF(s_AnalogController, 8);

/** @brief PSX controller button state flags. */
typedef struct _ButtonFlags
{
    /* 0x0  */ e_ControllerFlags held;
    /* 0x4  */ e_ControllerFlags clicked;
    /* 0x8  */ e_ControllerFlags released;
    /* 0xC  */ e_ControllerFlags pulsed;
    /* 0x10 */ e_ControllerFlags pulsedGui;
} s_ButtonFlags;

/** @brief PSX controller input data. */
typedef struct _ControllerData
{
    /* 0x0  */ s_AnalogController analogController;
    /* 0x8  */ s32                pulseTicks;
    /* 0xC  */ s_ButtonFlags      buttonFlags;
    /* 0x20 */ s_AnalogSticks     rawSticks;        /** Raw analog stick values, signed range `[-128, 127]`. */
    /* 0x24 */ s_AnalogSticks     normalizedSticks; /** Normalized analog stick values with deadzone, signed range `[-112, 112]`. */
    /* 0x28 */ s32                field_28;         // Processed input flags.
} s_ControllerData;
STATIC_ASSERT_SIZEOF(s_ControllerData, 44);

/** @brief Controller key bindings for input actions. Contains bitfield of button presses assigned to each action.
 *
 * Bitfields only contain buttons. Analog directions and D-Pad aren't included.
 */
typedef struct _ControllerConfig
{
    /* 0x0  */ u16 enter;
    /* 0x2  */ u16 cancel;
    /* 0x4  */ u16 skip;
    /* 0x6  */ u16 action;
    /* 0x8  */ u16 aim;
    /* 0xA  */ u16 light;
    /* 0xC  */ u16 run;
    /* 0xE  */ u16 view;
    /* 0x10 */ u16 stepLeft;
    /* 0x12 */ u16 stepRight;
    /* 0x14 */ u16 pause;
    /* 0x16 */ u16 item;
    /* 0x18 */ u16 map;
    /* 0x1A */ u16 option;
} s_ControllerConfig;
STATIC_ASSERT_SIZEOF(s_ControllerConfig, 28);

extern s_ControllerData* const g_Controller0;
extern s_ControllerData* const g_Controller1;

// ==========
// FUNCTIONS
// ==========

/** @brief Initializes controller 1. */
void Joy_Init(void);

/** @brief Reads analog data from controller 1. */
void Joy_ReadP1(void);

/** @brief Updates input data for all controllers. */
void Joy_Update(void);

/** @brief Updates digital data for all controllers, additionally handling special directional cases. */
void Joy_ControllerDataUpdate(void);

// TODO: Finish demagicking hex values. Does special handling for player movement.
void ControllerData_AnalogToDigital(s_ControllerData* cont, bool arg1);

/** @brief @unused */
bool func_8003483C(u16* arg0);

#endif
