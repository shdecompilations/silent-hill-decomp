#include "common.h"

// S__SAFE2: First anti-modchip overlay, ran before HP_SAFE1.
// Calls `SafetyCheck` from Sony `safechk.obj` in a loop to check for non-stealth modchips and display an error message
// if found.
// Then calls some init functions that are normally called by bodyprog.

void AntiModchip_Check(void) // JAP0 0x801E7EB4
{
    while (SafetyCheck(0, 2, 0) != 0)
    {
        VSync(0);
    }
    
    // These are normally called at start of `MainLoop`, likely moved here to break things if this code is skipped.
    InitGeom();
    Vibration_Init();
}
