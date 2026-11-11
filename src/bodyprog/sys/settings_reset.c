#include "game.h"

#include "bodyprog/bodyprog.h"
#include "bodyprog/sound/sound_system.h"
#include "bodyprog/screen/draw.h"
#include "screens/options/options.h"

void Settings_ScreenAndVolUpdate(void) // 0x800333CC
{
    Screen_XyPositionSet(g_GameWork.config.screenPositionX, g_GameWork.config.screenPositionY);
    SD_Call((g_GameWork.config.soundType != 0) ? AudioMode_Mono : AudioMode_Stereo);
    Sd_GlobalVolumeSet(Q7(1.0f), g_GameWork.config.volumeBgm, g_GameWork.config.volumeSe);
}

void Settings_RestoreDefaults(void) // 0x8003342C
{
    g_GameWork.config.extraWeaponCtrl = 1;
    g_GameWork.config.brightness      = 3;

    Settings_RestoreControlDefaults(0);

    g_GameWork.config.vibrationEnabled = Q7(1.0f);
    g_GameWork.config.volumeBgm        = Q7(1.0f);
    g_GameWork.config.volumeSe         = Q7(1.0f);

#if VERSION_REGION_IS(PAL)
    g_GameWork.config.extraAutoAiming = true;
#endif

    Settings_ScreenAndVolUpdate();

    g_GameWork.config.extraBloodColor = BloodColor_Normal;
}

void Settings_RestoreControlDefaults(s32 configIdx) // 0x80033480
{
    u32  i;
    u16* ptr;

#if VERSION_REGION_IS(NTSC) || VERSION_REGION_IS(PAL)
    static const s_ControllerConfig DEFAULT_CONTROLLER_CONFIGS[3] = {
        {
            .enter      = ControllerFlag_Start | ControllerFlag_Cross,
            .cancel     = ControllerFlag_Triangle | ControllerFlag_Circle | ControllerFlag_Square,
            .skip       = ControllerFlag_Start,
            .action     = ControllerFlag_Cross,
            .aim        = ControllerFlag_R2,
            .light      = ControllerFlag_Circle,
            .run        = ControllerFlag_Square,
            .view       = ControllerFlag_L2,
            .stepLeft  = ControllerFlag_L1,
            .stepRight = ControllerFlag_R1,
            .pause     = ControllerFlag_Start,
            .item      = ControllerFlag_Select,
            .map       = ControllerFlag_Triangle,
            .option    = ControllerFlag_None
        },
        {
            .enter      = ControllerFlag_Start | ControllerFlag_Cross,
            .cancel     = ControllerFlag_Triangle | ControllerFlag_Circle | ControllerFlag_Square,
            .skip       = ControllerFlag_Start,
            .action     = ControllerFlag_Cross,
            .aim        = ControllerFlag_R1,
            .light      = ControllerFlag_Circle,
            .run        = ControllerFlag_Square,
            .view       = ControllerFlag_L1,
            .stepLeft  = ControllerFlag_L2,
            .stepRight = ControllerFlag_R2,
            .pause     = ControllerFlag_Start,
            .item      = ControllerFlag_Select,
            .map       = ControllerFlag_Triangle,
            .option    = ControllerFlag_None
        },
        {
            .enter      = ControllerFlag_Start | ControllerFlag_Cross,
            .cancel     = ControllerFlag_Triangle | ControllerFlag_Circle | ControllerFlag_Square,
            .skip       = ControllerFlag_Start,
            .action     = ControllerFlag_Cross,
            .aim        = ControllerFlag_R2,
            .light      = ControllerFlag_Circle,
            .run        = ControllerFlag_Square,
            .view       = ControllerFlag_L2,
            .stepLeft  = ControllerFlag_L1,
            .stepRight = ControllerFlag_R1,
            .pause     = ControllerFlag_Start,
            .item      = ControllerFlag_Triangle,
            .map       = ControllerFlag_Select,
            .option    = ControllerFlag_None
        }
    };
#elif VERSION_REGION_IS(NTSCJ)
    static const s_ControllerConfig DEFAULT_CONTROLLER_CONFIGS[3] = {
        {
            .enter      = ControllerFlag_Start | ControllerFlag_Circle,
            .cancel     = ControllerFlag_Cross,
            .skip       = ControllerFlag_Start,
            .action     = ControllerFlag_Circle,
            .aim        = ControllerFlag_R2,
            .light      = ControllerFlag_Square,
            .run        = ControllerFlag_Cross,
            .view       = ControllerFlag_L2,
            .stepLeft  = ControllerFlag_L1,
            .stepRight = ControllerFlag_R1,
            .pause     = ControllerFlag_Start,
            .item      = ControllerFlag_Select,
            .map       = ControllerFlag_Triangle,
            .option    = ControllerFlag_None
        },
        {
            .enter      = ControllerFlag_Square | ControllerFlag_Circle | ControllerFlag_Start,
            .cancel     = ControllerFlag_Cross,
            .skip       = ControllerFlag_Start,
            .action     = ControllerFlag_Square | ControllerFlag_Circle,
            .aim        = ControllerFlag_R1,
            .light      = ControllerFlag_Triangle,
            .run        = ControllerFlag_Cross,
            .view       = ControllerFlag_L1,
            .stepLeft  = ControllerFlag_L2,
            .stepRight = ControllerFlag_R2,
            .pause     = ControllerFlag_None,
            .item      = ControllerFlag_Start,
            .map       = ControllerFlag_Select,
            .option    = ControllerFlag_None
        },
        {
            .enter      = ControllerFlag_Start | ControllerFlag_Circle,
            .cancel     = ControllerFlag_Cross,
            .skip       = ControllerFlag_Start,
            .action     = ControllerFlag_Circle,
            .aim        = ControllerFlag_R2,
            .light      = ControllerFlag_Square,
            .run        = ControllerFlag_Cross,
            .view       = ControllerFlag_L2,
            .stepLeft  = ControllerFlag_L1,
            .stepRight = ControllerFlag_R1,
            .pause     = ControllerFlag_Start,
            .item      = ControllerFlag_Triangle,
            .map       = ControllerFlag_Select,
            .option    = ControllerFlag_None
        }
    };
#endif

    for (i = 0, ptr = &g_GameWorkPtr->config.controllerConfig; i < INPUT_ACTION_COUNT; i++, ptr++)
    {
        *ptr = (&DEFAULT_CONTROLLER_CONFIGS[configIdx].enter)[i];
    }
}

void nullsub_800334C8(void) {} // 0x800334C8

void func_800334D0(void) {} // 0x800334D0
