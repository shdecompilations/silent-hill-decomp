#include "game.h"

#include <memory.h>
#include <psyq/libetc.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/demo.h"
#include "bodyprog/sys/joy.h"
#include "bodyprog/screen/screen_draw.h"
#include "bodyprog/sys/game_main.h"
#include "bodyprog/math/math.h"
#include "bodyprog/sound/sound_system.h"

void SysWork_Clear(void) // 0x800340E0
{
    bzero(&g_SysWork, sizeof(s_SysWork));
}

s32 MainLoop_ShouldWarmReset(void) // 0x80034108
{
    #define WARM_BOOT_COMBO_HOLD      (ControllerFlag_Select | ControllerFlag_Start)
    #define WARM_BOOT_COMBO_PRESS     (ControllerFlag_Select | ControllerFlag_Start | \
                                       ControllerFlag_L2 | ControllerFlag_R2 | ControllerFlag_L1 | ControllerFlag_R1)
    #define WARM_BOOT_COMBO_PRESS_ALT (ControllerFlag_Start | ControllerFlag_Triangle | ControllerFlag_Square)

    if (g_GameWork.gameState < GameState_MovieIntroAlternate)
    {
        return ResetType_None;
    }

    if (g_GameWork.gameState == GameState_LoadSavegameScreen && g_GameWork.gameStateSteps[0] == 4)
    {
        return ResetType_None;
    }

    if (g_GameWork.gameState == GameState_SaveScreen &&
        (g_GameWork.gameStateSteps[0] == 2 || g_GameWork.gameStateSteps[0] == 3))
    {
        return ResetType_None;
    }

    if (g_SysWork.sysFlags & SysFlag_DemoActive)
    {
        if (g_Demo_FrameCount > SECONDS_TO_FRAMES(30))
        {
            return ResetType_WarmBoot;
        }
    }
    else
    {
        g_Demo_FrameCount = ResetType_None;
    }

    if (g_GameWork.gameState == GameState_MainMenu)
    {
        return ResetType_None;
    }

    // Reset frame counter if reset buttons not held.
    if ((g_Controller0->buttonFlags.held & WARM_BOOT_COMBO_HOLD) != WARM_BOOT_COMBO_HOLD)
    {
        g_WarmBootTimer = 0;
    }

    if (g_WarmBootTimer > SECONDS_TO_FRAMES(2))
    {
        return ResetType_WarmBoot;
    }
    else if (g_Controller0->buttonFlags.held == WARM_BOOT_COMBO_PRESS &&
             (g_Controller0->buttonFlags.clicked & WARM_BOOT_COMBO_PRESS))
    {
        return ResetType_WarmBoot;
    }
    else if (g_Controller0->buttonFlags.held == WARM_BOOT_COMBO_PRESS_ALT &&
             (g_Controller0->buttonFlags.clicked & ControllerFlag_Start))
    {
        return ResetType_WarmBoot;
    }

    return (g_SysWork.sysFlags & SysFlag_DoWarmReset) ? ResetType_WarmBoot : ResetType_None;

    #undef WARM_BOOT_COMBO_HOLD
    #undef WARM_BOOT_COMBO_PRESS
    #undef WARM_BOOT_COMBO_PRESS_ALT
}

void Game_WarmBoot(void) // 0x80034264
{
    e_GameState prevState;

    DrawSync(SyncMode_Wait);

#if VERSION_REGION_IS(PAL)
    Screen_RectInterlacedClear(0, 0, 512, 511, 0, 0, 0);
#else
    Screen_RectInterlacedClear(0, 32, 512, 448, 0, 0, 0);
#endif

    func_800892A4(4);
    func_80089128();
    SD_Call(19);

    while (Sd_AudioStreamingCheck() != AudioStreamingState_None)
    {
        Sd_TaskPoolExecute();
        VSync(SyncMode_Wait);
    }

    SD_Call(20);

    while (Sd_AudioStreamingCheck() != AudioStreamingState_None)
    {
        Sd_TaskPoolExecute();
        VSync(SyncMode_Wait);
    }

    Fs_QueueReset();
    Fs_QueueWaitForEmpty();
    SD_InitStruct();
    Sd_AmbientSfxSet(1);

    while (Sd_AudioStreamingCheck() != AudioStreamingState_None)
    {
        Sd_TaskPoolExecute();
        VSync(SyncMode_Wait);
    }

    if (g_SysWork.sysFlags & SysFlag_DemoActive)
    {
        Demo_Stop();
    }

    SysWork_Clear();
    Demo_SequenceAdvance(1);
    Demo_DemoDataRead();
    GameFs_TitleGfxLoad();
    Fs_QueueWaitForEmpty();
    Joy_Update();

    Game_StateSetNext(GameState_MainMenu);

    ScreenFade_Start(true, true, false);
    g_ScreenFadeTimestep = Q12(0.0f);
}
