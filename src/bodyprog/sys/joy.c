#include "game.h"

#include <psyq/libpad.h>

#include "bodyprog/math/math.h"
#include "bodyprog/screen/data.h"
#include "bodyprog/screen/draw.h"
#include "bodyprog/sys/joy.h"

void Joy_Init(void) // 0x8003441C
{
    PadInitDirect(&g_GameWork.rawController, g_Controller1);
    PadStartCom();
}

void Joy_ReadP1(void) // 0x80034450
{
    s_ControllerData* cont;

    cont = &g_GameWork.controllers[0];

    // NOTE: `memcpy` is close, reads `rawController` as two `s32`s, but doesn't give match.
    // memcpy(&cont->analogController, &g_GameWork.rawController, sizeof(s_AnalogController));

    *(s32*)&cont->analogController        = *(s32*)&g_GameWork.rawController;
    *(s32*)&cont->analogController.rightX = *(s32*)&g_GameWork.rawController.rightX;

    // Alternate
    // ((s32*)&cont->analogController)[0] = ((s32*)&g_GameWork.rawController)[0];
    // ((s32*)&cont->analogController)[1] = ((s32*)&g_GameWork.rawController)[1];
}

void Joy_Update(void) // 0x8003446C
{
    Joy_ReadP1();
    Joy_ControllerDataUpdate();
}

void Joy_ControllerDataUpdate(void) // 0x80034494
{
    #define CONTROLLER_COUNT             2
    #define PULSE_INITIAL_INTERVAL_TICKS (TICKS_PER_SECOND / 2)
    #define PULSE_INTERVAL_TICKS         (PULSE_INITIAL_INTERVAL_TICKS / 10)

    s_ControllerData* cont;
    s32               i;
    s32               prevHeldButFlags;
    s32               pulseTicks;
    s32               pulsedButFlags;

    // Update controller button flags.
    for (i = CONTROLLER_COUNT, cont = g_Controller0;
         i > 0;
         i--, cont++)
    {
        prevHeldButFlags = cont->buttonFlags.held;

        // Update held button flags.
        if (cont->analogController.status == 0xFF)
        {
            cont->buttonFlags.held = ControllerFlag_None;
        }
        else
        {
            cont->buttonFlags.held = ~cont->analogController.buttonFlags & ControllerFlag_FaceButtons;
        }

        // TODO: Demagic hex values.
        ControllerData_AnalogToDigital(cont, (*(u16*)&cont->analogController.status & 0x5300) == 0x5300);

        // Simulate high-threshold left stick inputs with D-pad and low-threshold left stick inputs.
        // `<< 20` handles D-pad buttons, `<< 8` handles low-threshold left stick.
        cont->buttonFlags.held |= ((cont->buttonFlags.held << 20) | (cont->buttonFlags.held << 8)) &
                                  (ControllerFlag_LStickHighUp    |
                                   ControllerFlag_LStickHighRight |
                                   ControllerFlag_LStickHighDown  |
                                   ControllerFlag_LStickHighLeft);

        // Clear up/down held flags if concurrent.
        if ((cont->buttonFlags.held & (ControllerFlag_LStickHighUp | ControllerFlag_LStickHighDown)) == (ControllerFlag_LStickHighUp | ControllerFlag_LStickHighDown))
        {
            cont->buttonFlags.held &= ~(ControllerFlag_LStickHighUp | ControllerFlag_LStickHighDown);
        }

        // Clear left/right held flags if concurrent.
        if ((cont->buttonFlags.held & (ControllerFlag_LStickHighRight | ControllerFlag_LStickHighLeft)) == (ControllerFlag_LStickHighRight | ControllerFlag_LStickHighLeft))
        {
            cont->buttonFlags.held = cont->buttonFlags.held & ~(ControllerFlag_LStickHighRight | ControllerFlag_LStickHighLeft);
        }

        // Update clicked and released button flags.
        cont->buttonFlags.clicked  = ~prevHeldButFlags & cont->buttonFlags.held;
        cont->buttonFlags.released =  prevHeldButFlags & ~cont->buttonFlags.held;

        // Update pulse ticks.
        pulseTicks = cont->pulseTicks;
        if (cont->buttonFlags.held != prevHeldButFlags)
        {
            pulseTicks = 0;
        }
        else
        {
            pulseTicks += g_VBlanks;
        }

        // Update pulsed button flags.
        if (pulseTicks >= PULSE_INITIAL_INTERVAL_TICKS)
        {
            cont->buttonFlags.pulsed = cont->buttonFlags.held;
            pulseTicks               = PULSE_INITIAL_INTERVAL_TICKS - PULSE_INTERVAL_TICKS;
        }
        else
        {
            cont->buttonFlags.pulsed = cont->buttonFlags.clicked;
        }

        pulsedButFlags              = cont->buttonFlags.pulsed;
        cont->pulseTicks            = pulseTicks;
        cont->buttonFlags.pulsedGui = pulsedButFlags;

        // Clear left/right pulse flags if concurrent.
        if ((pulsedButFlags & (ControllerFlag_LStickHighRight | ControllerFlag_LStickHighLeft)) == (ControllerFlag_LStickHighRight | ControllerFlag_LStickHighLeft))
        {
            cont->buttonFlags.pulsedGui &= ~(ControllerFlag_LStickHighRight | ControllerFlag_LStickHighLeft);
        }

        // Clear up/down pulse flags if concurrent.
        if ((cont->buttonFlags.pulsedGui & (ControllerFlag_LStickHighUp | ControllerFlag_LStickHighDown)) == (ControllerFlag_LStickHighUp | ControllerFlag_LStickHighDown))
        {
            cont->buttonFlags.pulsedGui &= ~(ControllerFlag_LStickHighUp | ControllerFlag_LStickHighDown);
        }

        // Clear left/right pulse flags if up/down is concurrent.
        if ((cont->buttonFlags.pulsedGui & (ControllerFlag_LStickHighUp | ControllerFlag_LStickHighDown)))
        {
            cont->buttonFlags.pulsedGui &= ~(ControllerFlag_LStickHighRight | ControllerFlag_LStickHighLeft);
        }
    }
}

void ControllerData_AnalogToDigital(s_ControllerData* cont, bool arg1) // 0x80034670
{
    s32 dpadButFlags; // Used as analog value at first.
    s32 axisIdx;
    s32 processedInputFlags;
    s32 normalizedAnalogData;
    s32 xorShiftedRawAnalog;
    s32 heldButFlags;
    s32 signedRawAnalog;
    s32 negDirBitIdx;
    s32 posDirBitIdx;

    heldButFlags = cont->buttonFlags.held;

    if (arg1)
    {
        // Convert unsigned range to signed range.
        signedRawAnalog = *(u32*)&cont->analogController.rightX ^ 0x80808080;

        xorShiftedRawAnalog = signedRawAnalog;

        for (normalizedAnalogData = 0, axisIdx = 3;
             axisIdx >= 0;
             axisIdx--)
        {
            normalizedAnalogData <<= 8;
            dpadButFlags           = xorShiftedRawAnalog >> 24;
            xorShiftedRawAnalog  <<= 8;

            if (dpadButFlags < -STICK_DEADZONE)
            {
                normalizedAnalogData |= (dpadButFlags + STICK_DEADZONE) & 0xFF;
                negDirBitIdx          = 23 - (axisIdx & (1 << 0));
                heldButFlags         |= 1 << (negDirBitIdx - (axisIdx * 2));
            }
            else if (dpadButFlags >= STICK_DEADZONE)
            {
                normalizedAnalogData |= (dpadButFlags - (STICK_DEADZONE - 1)) & 0xFF;
                posDirBitIdx          = (axisIdx & 0x1) + 21;
                heldButFlags         |= 1 << (posDirBitIdx - ((axisIdx >> 1) * 4));
            }
        }

        cont->buttonFlags.held = heldButFlags;
    }
    else
    {
        signedRawAnalog      = 0;
        normalizedAnalogData = 0;
    }

    processedInputFlags       = normalizedAnalogData;
    cont->rawSticks.rawData_0 = signedRawAnalog;

    // TODO: Demagic remaining hex values.
    if (cont == g_Controller0)
    {
        if (!(processedInputFlags & ControllerFlag_HighSticks))
        {
            dpadButFlags = heldButFlags & (ControllerFlag_DpadUp | ControllerFlag_DpadDown);
            if (dpadButFlags == ControllerFlag_DpadDown)
            {
                normalizedAnalogData = processedInputFlags | 0x2D000000;
            }
            else if (dpadButFlags == ControllerFlag_DpadUp)
            {
                normalizedAnalogData = processedInputFlags | 0xD3000000;
            }
        }

        if (!(normalizedAnalogData & ControllerFlag_LowSticks))
        {
            dpadButFlags = heldButFlags & (ControllerFlag_DpadRight | ControllerFlag_DpadLeft);
            if (dpadButFlags == ControllerFlag_DpadRight)
            {
                normalizedAnalogData |= 0x2D0000;
            }
            else if (dpadButFlags == ControllerFlag_DpadLeft)
            {
                normalizedAnalogData |= 0xD30000;
            }
        }

        if (!(processedInputFlags & ControllerFlag_HighSticks))
        {
            dpadButFlags = heldButFlags & (ControllerFlag_DpadUp | ControllerFlag_DpadDown);
            if (dpadButFlags == ControllerFlag_DpadDown)
            {
                processedInputFlags |= 0x20000000;
            }
            else if (dpadButFlags == ControllerFlag_DpadUp)
            {
                if (!(heldButFlags & g_GameWorkPtr->config.controllerConfig.run))
                {
                    processedInputFlags |= 0xE0000000;
                }
                else
                {
                    processedInputFlags |= 0xC0000000;
                }
            }
        }

        if (!(processedInputFlags & ControllerFlag_LowSticks))
        {
            dpadButFlags = heldButFlags & (ControllerFlag_DpadRight | ControllerFlag_DpadLeft);
            if (dpadButFlags == ControllerFlag_DpadRight)
            {
                processedInputFlags |= 0x200000;
            }
            else if (dpadButFlags == ControllerFlag_DpadLeft)
            {
                processedInputFlags |= 0xE00000;
            }
        }
    }

    cont->normalizedSticks.rawData_0 = normalizedAnalogData;
    cont->field_28                   = processedInputFlags;
}

bool func_8003483C(u16* arg0) // 0x8003483C
{
    if (g_Controller0->buttonFlags.clicked & *(*arg0 + arg0))
    {
        *arg0 = *arg0 + 1;
    }
    else if (g_Controller0->buttonFlags.clicked & (*(arg0 + 1)))
    {
        *arg0 = 2;
    }
    else if (g_Controller0->buttonFlags.clicked & ControllerFlag_FaceButtons)
    {
        *arg0 = 1;
    }

    if (*(*arg0 + arg0) == ControllerFlag_FaceButtons)
    {
        *arg0 = 1;
        return true;
    }
    else
    {
        return false;
    }
}
