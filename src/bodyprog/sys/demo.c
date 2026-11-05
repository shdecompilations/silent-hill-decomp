#include "game.h"

#include "bodyprog/bodyprog.h"
#include "bodyprog/sys/demo.h"
#include "bodyprog/text/text_draw.h"
#include "bodyprog/screen/draw.h"
#include "main/fsqueue.h"
#include "main/rng.h"

s32                  g_Demo_DemoFileIdx;
s32                  g_Demo_PlayFileIdx;
s32                  __pad_bss_800C4848[2];
s_OptionsConfig      g_Demo_OptionsConfigBackup;
u32                  g_Demo_PrevRandSeed;
u32                  g_Demo_RandSeedBackup;
s_DemoPlaybackFrame* g_Demo_ActivePlaybackFrame;
s32                  g_Demo_DemoStep;
s32                  g_Demo_VideoPresentInterval;
bool                 g_Demo_IsLoadingChunks;
s32                  g_Demo_DemoId   = 0;
u16                  g_Demo_RandSeed = 0;
// 2 bytes of padding.
s_DemoPlaybackFrame* g_Demo_PlaybackFrames = (s_DemoPlaybackFrame*)0x800F5E00;

bool Demo_SequenceAdvance(s32 incAmount) // 0x8008EF20
{
#if VERSION_REGION_IS(PAL)
    #define DEMO_FILE_COUNT_MAX 4
#else
    #define DEMO_FILE_COUNT_MAX 5
#endif

    static s_DemoFileInfo DEMO_FILE_INFOS[DEMO_FILE_COUNT_MAX] =
    {
#if VERSION_REGION_IS(NTSC)
        { FILE_MISC_DEMO0009_DAT, FILE_MISC_PLAY0009_DAT, 0 },
        { FILE_MISC_DEMO000A_DAT, FILE_MISC_PLAY000A_DAT, 0 },
        { FILE_MISC_DEMO0003_DAT, FILE_MISC_PLAY0003_DAT, 0 },
        { FILE_MISC_DEMO000B_DAT, FILE_MISC_PLAY000B_DAT, 0 },
        { FILE_MISC_DEMO0005_DAT, FILE_MISC_PLAY0005_DAT, 0 }
#elif VERSION_REGION_IS(NTSCJ)
        { FILE_MISC_DEMO0006_DAT, FILE_MISC_PLAY0006_DAT, 0 },
        { FILE_MISC_DEMO0007_DAT, FILE_MISC_PLAY0007_DAT, 0 },
        { FILE_MISC_DEMO0008_DAT, FILE_MISC_PLAY0008_DAT, 0 },
        { FILE_MISC_DEMO0004_DAT, FILE_MISC_PLAY0004_DAT, 0 },
        { FILE_MISC_DEMO0005_DAT, FILE_MISC_PLAY0005_DAT, 0 }
#elif VERSION_REGION_IS(PAL)
        { FILE_MISC_DEMO000C_DAT, FILE_MISC_PLAY000C_DAT, 0 },
        { FILE_MISC_DEMO000D_DAT, FILE_MISC_PLAY000D_DAT, 0 },
        { FILE_MISC_DEMO0003_DAT, FILE_MISC_PLAY0003_DAT, 0 },
        { FILE_MISC_DEMO0005_DAT, FILE_MISC_PLAY0005_DAT, 0 }
#endif
    };

    g_Demo_DemoId += incAmount;

    while (true)
    {
        // Cycle demo ID.
        while (g_Demo_DemoId < 0)
        {
            g_Demo_DemoId += DEMO_FILE_COUNT_MAX;
        }
        while ((u32)g_Demo_DemoId >= DEMO_FILE_COUNT_MAX)
        {
            g_Demo_DemoId -= DEMO_FILE_COUNT_MAX;
        }

        // Call optional funcptr associated with this demo.
        // If funcptr is set, return whether demo is eligible to play, possibly based on game progress or other conditions.
        // In retail demos this pointer is always `NULL`.
        if (DEMO_FILE_INFOS[g_Demo_DemoId].canPlayDemo   == NULL ||
            DEMO_FILE_INFOS[g_Demo_DemoId].canPlayDemo() == true)
        {
            break;
        }

        // If funcptr is set and returned false, skip to next demo.
        // Direction to skip depends on sign of `incrementAmount` (forward or backward).
        if (incAmount >= 0)
        {
            g_Demo_DemoId++;
        }
        else
        {
            g_Demo_DemoId--;
        }
    }

    g_Demo_DemoFileIdx = DEMO_FILE_INFOS[g_Demo_DemoId].demoFileId;
    g_Demo_PlayFileIdx = DEMO_FILE_INFOS[g_Demo_DemoId].playFileId;
    return true;

    #undef DEMO_FILE_COUNT_MAX
}

void Demo_DemoDataRead(void) // 0x8008F048
{
    if (g_Demo_DemoFileIdx != NO_VALUE)
    {
        Fs_QueueStartRead(g_Demo_DemoFileIdx, g_Demo_ActiveState);
    }
}

void Demo_PlayDataRead(void) // 0x8008F07C
{
    Demo_SequenceAdvance(0);

    if (g_Demo_PlayFileIdx != NO_VALUE)
    {
        Fs_QueueStartRead(g_Demo_PlayFileIdx, g_Demo_PlaybackFrames);
    }
}

bool Demo_PlayFileBufferSetup(void) // 0x8008F0BC
{
    s32 mapOverlaySize;
    s32 playFileSize;

    // Get map overlay size used in demo.
    mapOverlaySize = Fs_GetFileSize(FILE_VIN_MAP0_S00_BIN + g_Demo_ActiveState->savegame.mapIdx);

    // Get play file size rounded up to next 0x800-byte boundary.
    playFileSize = ALIGN(Fs_GetFileSize(g_Demo_PlayFileIdx), 0x800);

    // Try placing play file buffer just before `g_Demo_ActiveState` in memory.
    g_Demo_PlaybackFrames = (void*)((s32)g_Demo_ActiveState - playFileSize);

    // If play file or map overlay is too large, buffer pointer may overlap with map.
    // Return `true` if buffer fits (no overlap with map overlay), otherwise, return `false`.
    return (u32)g_Demo_PlaybackFrames >= (u32)(g_OvlDynamic + mapOverlaySize);
}

void Demo_DemoFileSavegameUpdate(void) // 0x8008F13C
{
    g_GameWork.savegame = g_Demo_ActiveState->savegame;
}

void Demo_GameGlobalsUpdate(void) // 0x8008F1A0
{
    // Backup current options config.
    g_Demo_OptionsConfigBackup = g_GameWork.config;

    // Update `Demo_RandSeed`.
    g_Demo_RandSeed = g_Demo_ActiveState->randSeed;

    // Replace options config with config from demo file.
    g_GameWork.config = g_Demo_ActiveState->config;

    // Restore user options config over demo values.
    g_GameWork.config.screenPositionX  = g_Demo_OptionsConfigBackup.screenPositionX;
    g_GameWork.config.screenPositionY  = g_Demo_OptionsConfigBackup.screenPositionY;
    g_GameWork.config.soundType        = g_Demo_OptionsConfigBackup.soundType;
    g_GameWork.config.volumeBgm        = OPT_SOUND_VOLUME_MIN;
    g_GameWork.config.volumeSe         = g_Demo_OptionsConfigBackup.volumeSe;
    g_GameWork.config.vibrationEnabled = OPT_VIBRATION_DISABLED;
    g_GameWork.config.brightness       = g_Demo_OptionsConfigBackup.brightness;

    Sd_GlobalVolumeSet(OPT_SOUND_VOLUME_MIN, OPT_SOUND_VOLUME_MIN, g_GameWork.config.volumeSe);
}

void Demo_GameGlobalsRestore(void) // 0x8008F2BC
{
    g_GameWork.config = g_Demo_OptionsConfigBackup;
    Sd_GlobalVolumeSet(OPT_SOUND_VOLUME_MAX, g_GameWork.config.volumeBgm, g_GameWork.config.volumeSe);
}

void Demo_GameRandSeedUpdate(void) // 0x8008F33C
{
    g_Demo_PrevRandSeed = Rng_GetSeed();
    Rng_SetSeed(g_Demo_RandSeed);
}

void Demo_GameRandSeedRestore(void) // 0x8008F370
{
    Rng_SetSeed(g_Demo_PrevRandSeed);
}

bool g_Demo_Play = false;

void Demo_Start(void) // 0x8008F398
{
    g_Demo_Play         = true;
    g_SysWork.sysFlags |= SysFlag_DemoActive;

    Demo_GameGlobalsUpdate();
    Demo_GameRandSeedUpdate();

    g_GameWork.field_5A8 = 1;
    g_GameWork.field_5AC = 1;
}

void Demo_Stop(void) // 0x8008f3f0
{
    g_Demo_Play         = false;
    g_SysWork.sysFlags &= ~SysFlag_DemoActive;

    Demo_GameGlobalsRestore();
    Demo_GameRandSeedRestore();
}

bool Gfx_ScreenFadeIn_IsInProgress(s32 arg0)
{
    s32 screenFadeStatus;

    screenFadeStatus = arg0 & ~(1 << 0);

    switch (screenFadeStatus)
    {
        case SCREEN_FADE_STATUS(ScreenFadeState_FadeOutStart,    false):
        case SCREEN_FADE_STATUS(ScreenFadeState_FadeOutSteps,    false):
        case SCREEN_FADE_STATUS(ScreenFadeState_ResetTimestep,   false):
        case SCREEN_FADE_STATUS(ScreenFadeState_FadeOutComplete, false):
        case SCREEN_FADE_STATUS(ScreenFadeState_FadeOutStart,    true):
        case SCREEN_FADE_STATUS(ScreenFadeState_FadeOutSteps,    true):
        case SCREEN_FADE_STATUS(ScreenFadeState_ResetTimestep,   true):
        case SCREEN_FADE_STATUS(ScreenFadeState_FadeOutComplete, true):
            return false;
    }

    return true;
}

s32 Demo_StateGet(s32 gameState)
{
    switch (gameState)
    {
        case GameState_InGame:
            if (g_SysWork.sysState == SysState_GameOver)
            {
                return DemoState_Exit;
            }
            else if (g_GameWork.gameStatePrev == GameState_SaveScreen)
            {
                return DemoState_Exit;
            }

        case GameState_MapEvent:
        case GameState_ExitMovie:
        case GameState_InventoryScreen:
        case GameState_PaperMapScreen:
            return DemoState_Step;

        case GameState_OptionScreen:
            return DemoState_Step;
    }

    return DemoState_None;
}

void Demo_ExitDemo(void) // 0x8008F4E4
{
    g_Demo_FrameCount          = SECONDS_TO_FRAMES(999);
    g_Demo_ActivePlaybackFrame = NULL;
    g_Demo_DemoStep            = 0;
    g_SysWork.sysFlags        |= SysFlag_DoWarmReset;
}

void nullsub_8008F518(void) {} // 0x8008F518

bool func_8008F520(void) // 0x8008F520
{
    return false;
}

void Demo_DemoRandSeedBackup(void) // 0x8008F528
{
    if (g_SysWork.sysFlags & SysFlag_DemoActive)
    {
        g_Demo_RandSeedBackup = Rng_GetSeed();
    }
}

void Demo_DemoRandSeedRestore(void) // 0x8008F560
{
    if (g_SysWork.sysFlags & SysFlag_DemoActive)
    {
        Rng_SetSeed(g_Demo_RandSeedBackup);
    }
}

void Demo_DemoRandSeedAdvance(void) // 0x8008F598
{
#if VERSION_EQUAL_OR_NEWER(USA)
    #define SEED_OFFSET 0x3C6EF35F

    if (g_SysWork.sysFlags & SysFlag_DemoActive)
    {
        Rng_SetSeed(g_Demo_RandSeedBackup + SEED_OFFSET);
    }

    #undef SEED_OFFSET
#else
    // JAP0 doesn't have code here, just 16 bytes of stack for some reason?
    u8 unused[16];
#endif
}

bool Demo_Update(void) // 0x8008F5D8
{
    s32         prevScreenFadeCpy;
    bool        isLoadingChunks;
    u32         demoStep;
    s_GameWork* gameWork;

    static s32 prevScreenFade = SCREEN_FADE_STATUS(ScreenFadeState_Reset, false);

    prevScreenFadeCpy      = prevScreenFade;
    isLoadingChunks        = g_Demo_IsLoadingChunks;
    g_Demo_IsLoadingChunks = false;
    prevScreenFade         = g_Screen_FadeStatus;

    if (!(g_SysWork.sysFlags & SysFlag_DemoActive))
    {
        g_Demo_ActivePlaybackFrame = NULL;
        g_Demo_DemoStep            = 0;
        return true;
    }

    if (g_Demo_PlaybackFrames == NULL)
    {
        g_Demo_ActivePlaybackFrame = NULL;
        return false;
    }

    demoStep = g_Demo_DemoStep;
    if (g_Demo_ActiveState->frameCount <= demoStep)
    {
        nullsub_8008F518();
        Demo_ExitDemo();
        return false;
    }

    if (!Gfx_ScreenFadeIn_IsInProgress(prevScreenFadeCpy)   ||
        !Gfx_ScreenFadeIn_IsInProgress(g_Screen_FadeStatus) ||
        isLoadingChunks)
    {
        g_Demo_ActivePlaybackFrame = NULL;
        return true;
    }

    gameWork = &g_GameWork;

    // Handle demo state.
    switch (Demo_StateGet(gameWork->gameState))
    {
        case DemoState_Step:
            g_Demo_ActivePlaybackFrame = &g_Demo_PlaybackFrames[g_Demo_DemoStep];

            if (g_Demo_ActivePlaybackFrame->expectedGameState != gameWork->gameState)
            {
                Text_Debug_PositionSet(8, 80);
                Text_Debug_Draw("STEP ERROR:[H:");
                Text_Debug_Draw(Text_Debug_IntToString(2, g_Demo_ActivePlaybackFrame->expectedGameState));
                Text_Debug_Draw("]/[M:");
                Text_Debug_Draw(Text_Debug_IntToString(2, gameWork->gameState));
                Text_Debug_Draw("]");

                g_Demo_ActivePlaybackFrame = NULL;
            }

            g_Demo_DemoStep++;
            nullsub_8008F518();
            return true;

        case DemoState_Exit:
            nullsub_8008F518();
            Demo_ExitDemo();
            return false;

        case DemoState_None:
            break;
    }

    g_Demo_ActivePlaybackFrame = NULL;
    return true;
}

bool Demo_ControllerDataUpdate(void) // 0x8008F7CC
{
    u32 butFlags;

    if (!(g_SysWork.sysFlags & SysFlag_DemoActive))
    {
        return false;
    }

    butFlags = g_Controller0->analogController.buttonFlags;
    if (butFlags != ControllerFlag_FaceButtons)
    {
        Demo_ExitDemo();
        return true;
    }

    g_Demo_FrameCount = 0;

    if (g_Demo_ActivePlaybackFrame != NULL)
    {
        g_Controller0->analogController = g_Demo_ActivePlaybackFrame->analogController;
        return true;
    }

    *(u16*)&g_Controller0->analogController.status = 0x7300;
    g_Controller0->analogController.buttonFlags    = butFlags;

    // Convert unsigned range to signed range.
    *(u32*)&g_Controller0->analogController.rightX = 0x80808080;
    return true;
}

bool Demo_PresentIntervalUpdate(void) // 0x8008F87C
{
    g_Demo_VideoPresentInterval = 1;

    if (g_Demo_ActivePlaybackFrame == NULL)
    {
        return false;
    }

    g_Demo_VideoPresentInterval = g_Demo_ActivePlaybackFrame->videoPresentInterval;
    return true;
}

bool Demo_GameRandSeedSet(void) // 0x8008F8A8
{
    if (!(g_SysWork.sysFlags & SysFlag_DemoActive))
    {
        return true;
    }
    else if (g_Demo_ActivePlaybackFrame == NULL)
    {
        Rng_SetSeed(g_Demo_RandSeed);
        return false;
    }
    else
    {
        Rng_SetSeed(g_Demo_ActivePlaybackFrame->randSeed);
        return true;
    }
}

bool func_8008F914(s32 posX, s32 posZ)
{
    if (g_SysWork.sysFlags & SysFlag_DemoActive)
    {
        return WorldMap_CloseChunkEdgeCheck(posX, posZ);
    }

    return true;
}
