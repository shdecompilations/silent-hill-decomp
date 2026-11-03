#include "game.h"

#include <memory.h>
#include <psyq/libetc.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/events/radio.h"
#include "bodyprog/gfx/map_effects.h"
#include "bodyprog/item_screens.h"
#include "bodyprog/math/math.h"
#include "bodyprog/screen/screen_data.h"
#include "bodyprog/screen/screen_draw.h"
#include "bodyprog/sound/sound_system.h"
#include "bodyprog/text/text_debug_draw.h"
#include "bodyprog/text/text_draw.h"
#include "screens/options.h"
#include "screens/stream/stream.h"

#if VERSION_REGION_IS(PAL)
extern s32 D_800A93B8;
#endif

#if VERSION_REGION_IS(NTSCJ)
INCLUDE_RODATA("screens/options/nonmatchings/options", D_801E2600);
#endif

// Japanese releases draw the main and extra option menus 20 pixels higher, PAL only moves the main menu up by 8.
#if VERSION_REGION_IS(NTSCJ)
    #define MENU_SHIFT_Y       -20
    #define EXTRA_MENU_SHIFT_Y -20
#elif VERSION_REGION_IS(PAL)
    #define MENU_SHIFT_Y       -8
    #define EXTRA_MENU_SHIFT_Y 0
#else
    #define MENU_SHIFT_Y       0
    #define EXTRA_MENU_SHIFT_Y 0
#endif

#define LINE_CURSOR_TIMER_MAX          8
#define LAYER_24                       24
#define LAYER_32                       32
#define LAYER_36                       36
#define LAYER_40                       40
#define LAYER_44                       44
#define LAYER_48                       48
#define LAYER_8148                     8148
#define OPTIONS_ENTRY_STRING_LAYER_IDX 8 // TODO: Leftover from JAP builds or required for all?

s32  g_MainOptionsMenu_SelectedEntry      = 0;
s32  g_ExtraOptionsMenu_SelectedEntry     = 0;
s32  g_MainOptionsMenu_PrevSelectedEntry  = 0;
s32  g_ExtraOptionsMenu_PrevSelectedEntry = 0;
bool g_ScreenPosMenu_InvertBackgroundFade = false;
bool g_ControllerMenu_IsOnActionsPane     = false;

/** @brief Tracks movement time of the cursor highlight. */
static s32 g_Options_SelectionHighlightTimer;

#if VERSION_REGION_IS(PAL)
/** @brief Whether the language entry in the main options menu can be used, only when the menu was opened from the main menu. */
static bool g_Options_IsLanguageSelectable;
#endif

/** @brief Number of options to show in the extra options screen. Shows extra unlockable settings if they are unlocked. */
static s32 g_ExtraOptionsMenu_EntryCount;

static s32 g_ExtraOptionsMenu_SelectedBloodColorEntry;

static s32 g_ExtraOptionsMenu_BulletMultMax;

#if VERSION_REGION_IS(PAL)
static s32 __pad_bss_801E76A8[2];
#endif

// ========================================
// MAIN AND EXTRA OPTION SCREEN
// ========================================

void GameState_Options_Update(void) // 0x801E2D44
{
    s32 unlockedOptFlags;
    s32 i;
#if VERSION_REGION_IS(NTSCJ)
    char* mainOptionsDescs[10] =
    {
        "オプションモードを終了します　　　　　　",
        "画面の明るさを調節します　　　　　　　　",
        "操作方法を変更します　　　　　　　　　　",
        "画面の表示位置を調節します　　　　　　　",
        "　アナログコントローラの振動設定：　　　",
        "　　　　オートロードの設定：　　　　　　",
        "　　サウンドの設定：　　　　　　　　　　",
        "ＢＧＭのボリュームを調節します　　　　　",
        "ＳＥのボリュームを調節します　　　　　　",
        "オンオフステレオモノラル　　　　　　　　"
    };
    char* extraOptions[10] =
    {
        "武器の構えタイプ：　　　　　　　　　　　",
        "血しぶきの色：　　　　　　　　　　　　　",
        "サーチビュー操作：　　　　　　　　　　　",
        "後退時の旋回：　　　　　　　　　　　　　",
        "歩行／走行の操作：　　　　　　　　　　　",
        "武器の自動照準：　　　　　　　　　　　　",
        "サーチビュー効果：　　　　　　　　　　　",
        "弾薬量設定：　　×１×２×３×４×５×６",
        "切り替え押し続け×１×２×３×５オフオン",
        "リバースノーマル緑紫黒白前方視点自己視点"
    };
#endif

    if (g_GameWork.gameStatePrev == GameState_InGame)
    {
        Bgm_MenuUpdate();
    }

    if (g_GameWork.gameStatePrev != GameState_MainMenu)
    {
        Game_TimerUpdate();
    }

    // Handle options menu state.
    switch (g_GameWork.gameStateSteps[0])
    {
        case OptionsMenuState_EnterMainOptions:
            DrawSync(SyncMode_Wait);

            if (g_GameWork.gameStatePrev != GameState_InGame)
            {
#if VERSION_REGION_IS(NTSC)
                VSync(SyncMode_Wait8);
#else
                VSync(SyncMode_Wait3);
#endif
            }

            g_GameWork.background2dColor.r = 0;
            g_GameWork.background2dColor.g = 0;
            g_GameWork.background2dColor.b = 0;

            ScreenFade_Start(false, true, false);
            g_IntervalVBlanks   = 1;

            if (g_GameWork.gameStatePrev == GameState_InGame)
            {
                Game_RadioSoundStop();
            }

            g_MainOptionsMenu_SelectedEntry      = MainOptionsMenuEntry_Exit;
            g_MainOptionsMenu_PrevSelectedEntry  = 0;
            g_ExtraOptionsMenu_SelectedEntry     = 0;
            g_ExtraOptionsMenu_PrevSelectedEntry = 0;
            g_Options_SelectionHighlightTimer    = 0;
#if VERSION_REGION_IS(PAL)
            g_Options_IsLanguageSelectable       = g_GameWork.gameStatePrev == GameState_MainMenu;
#endif
            g_ExtraOptionsMenu_BulletMultMax     = 1;
            unlockedOptFlags                     = g_GameWork.config.extraOptionsEnabled;

            // Set available bullet multiplier.
            for (i = 0; i < 5; i++)
            {
                if (unlockedOptFlags & (1 << i))
                {
                    g_ExtraOptionsMenu_BulletMultMax++;
                }
            }

            // Set selected blood color.
            switch (g_GameWork.config.extraBloodColor)
            {
                case BloodColor_Normal:
                    g_ExtraOptionsMenu_SelectedBloodColorEntry = BloodColorMenuEntry_Normal;
                    break;

                case BloodColor_Green:
                    g_ExtraOptionsMenu_SelectedBloodColorEntry = BloodColorMenuEntry_Green;
                    break;

                case BloodColor_Violet:
                    g_ExtraOptionsMenu_SelectedBloodColorEntry = BloodColorMenuEntry_Violet;
                    break;

                case BloodColor_Black:
                    g_ExtraOptionsMenu_SelectedBloodColorEntry = BloodColorMenuEntry_Black;
                    break;
            }

            g_ExtraOptionsMenu_EntryCount = (g_GameWork.config.extraOptionsEnabled) ? 8 : 6;

            Game_StateStepSet(0, OptionsMenuState_MainOptions);

#if VERSION_REGION_IS(NTSCJ)
            func_80037124();

            D_800C3920 = 20;
            for (i = 0; i < 10; i++)
            {
                func_8004C918(mainOptionsDescs[i], i, 1, 14);
            }
#endif
            break;

        case OptionsMenuState_LeaveScreenPos:
        case OptionsMenuState_LeaveBrightness:
        case OptionsMenuState_LeaveController:
            Game_StateStepSet(0, OptionsMenuState_MainOptions);
            break;

        case OptionsMenuState_EnterScreenPos:
            if (ScreenFade_IsFinished())
            {
                Game_StateStepSet(0, OptionsMenuState_ScreenPos);
            }
            break;

        case OptionsMenuState_ScreenPos:
            Options_ScreenPosMenu_Control();
            break;

        case OptionsMenuState_EnterBrightness:
            if (ScreenFade_IsFinished())
            {
                Fs_QueueWaitForEmpty();
                Game_StateStepSet(0, OptionsMenuState_Brightness);
            }
            break;

        case OptionsMenuState_Brightness:
            Options_BrightnessMenu_Control();
            break;

        case OptionsMenuState_EnterController:
            // Switch to controller binding menu.
            if (ScreenFade_IsFinished())
            {
                Game_StateStepSet(0, OptionsMenuState_Controller);
            }
            break;

        case OptionsMenuState_Controller:
            Options_ControllerMenu_Control();
            break;

        case OptionsMenuState_Leave:
            ScreenFade_Start(true, false, false);
            Game_StateStepSet(0, OptionsMenuState_LeaveMainOptions);
            break;

        case OptionsMenuState_LeaveMainOptions:
            if (ScreenFade_IsFinished())
            {
                // TODO: `Game_StateSetPrevious` won't work here? (also tried macro version of it)
                e_GameState prevGameState = g_GameWork.gameStatePrev;
                Game_StateSetNext(prevGameState);
                g_GameWork.gameStateSteps[0] = OptionsMenuState_EnterMainOptions;
            }

            break;

        case OptionsMenuState_EnterExtraOptions:
            if (ScreenFade_IsFinished())
            {
                Game_StateStepSet(0, OptionsMenuState_ExtraOptions);
                ScreenFade_Start(false, true, false);
                g_Options_SelectionHighlightTimer = 0;

#if VERSION_REGION_IS(NTSCJ)
                D_800C3920 = 20;
                for (i = 0; i < 10; i++)
                {
                    func_8004C918(extraOptions[i], i, 1, 14);
                }
#endif
            }
            break;

        case OptionsMenuState_LeaveExtraOptions:
            if (ScreenFade_IsFinished())
            {
                Game_StateStepSet(0, OptionsMenuState_EnterMainOptions);
                ScreenFade_Start(false, true, false);
            }
            break;
    }

    // Handle menu state.
    switch (g_GameWork.gameStateSteps[0])
    {
        case OptionsMenuState_MainOptions:
        case OptionsMenuState_Leave:
        case OptionsMenuState_LeaveMainOptions:
        case OptionsMenuState_EnterScreenPos:
        case OptionsMenuState_EnterBrightness:
        case OptionsMenuState_EnterController:
        case OptionsMenuState_EnterExtraOptions:
            Options_MainOptionsMenu_Control();
            break;

        case OptionsMenuState_ExtraOptions:
        case OptionsMenuState_LeaveExtraOptions:
            Options_ExtraOptionsMenu_Control();
            break;
    }
}

void Options_ExtraOptionsMenu_Control(void) // 0x801E318C
{
    Options_ExtraOptionsMenu_EntryStringsDraw();
    Options_ExtraOptionsMenu_ConfigDraw();

#if VERSION_REGION_IS(NTSCJ)
    func_801E749C();
#endif

    Options_ExtraOptionsMenu_SelectionHighlightDraw();
    Options_Menu_BackgroundDraw();
    Screen_BackgroundImgDraw(&g_ItemInspectionImg);

    if (g_GameWork.gameStateSteps[0] != OptionsMenuState_ExtraOptions)
    {
        return;
    }

    // Increment line move timer.
    if ((LINE_CURSOR_TIMER_MAX - 1) < g_Options_SelectionHighlightTimer)
    {
        g_Options_SelectionHighlightTimer = LINE_CURSOR_TIMER_MAX;
    }
    else
    {
        g_Options_SelectionHighlightTimer++;
    }

    if (g_Options_SelectionHighlightTimer == LINE_CURSOR_TIMER_MAX)
    {
        g_ExtraOptionsMenu_PrevSelectedEntry = g_ExtraOptionsMenu_SelectedEntry;

        // Leave to gameplay (if options menu was accessed with `Option` input action).
        if (g_GameWork.gameStatePrev == GameState_InGame &&
            !(g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.enter) &&
            (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.option))
        {
            Sd_SfxPlay(Sfx_MenuCancel, Q8(0.0f), Q8(0.25f));
            Game_StateStepSet(0, OptionsMenuState_Leave);
            return;
        }

        // Move selection cursor up/down.
        if (g_Controller0->buttonFlags.pulsed & ControllerFlag_LStickHighUp)
        {
            s32 var = 1;
            Sd_SfxPlay(Sfx_MenuMove, Q8(0.0f), Q8(0.25f));
            g_ExtraOptionsMenu_SelectedEntry  = ((g_ExtraOptionsMenu_SelectedEntry - var) + g_ExtraOptionsMenu_EntryCount) % g_ExtraOptionsMenu_EntryCount;
            g_Options_SelectionHighlightTimer = 0;
        }
        if (g_Controller0->buttonFlags.pulsed & ControllerFlag_LStickHighDown)
        {
            Sd_SfxPlay(Sfx_MenuMove, Q8(0.0f), Q8(0.25f));
            g_ExtraOptionsMenu_SelectedEntry++;
            g_ExtraOptionsMenu_SelectedEntry  = g_ExtraOptionsMenu_SelectedEntry % g_ExtraOptionsMenu_EntryCount;
            g_Options_SelectionHighlightTimer = 0;
        }

        // Handle config change.
        switch (g_ExtraOptionsMenu_SelectedEntry)
        {
            case ExtraOptionsMenuEntry_WeaponCtrl:
                // Scroll left/right.
                if (g_Controller0->buttonFlags.clicked & (ControllerFlag_LStickHighRight | ControllerFlag_LStickHighLeft))
                {
                    Sd_SfxPlay(Sfx_MenuMove, Q8(0.0f), Q8(0.25f));
                    g_GameWork.config.extraWeaponCtrl = !g_GameWork.config.extraWeaponCtrl;
                }
                break;

            case ExtraOptionsMenuEntry_Blood:
                // Scroll left/right.
                if (g_Controller0->buttonFlags.clicked & ControllerFlag_LStickHighRight)
                {
                    Sd_SfxPlay(Sfx_MenuMove, Q8(0.0f), Q8(0.25f));
                    g_ExtraOptionsMenu_SelectedBloodColorEntry++;
                }
                if (g_Controller0->buttonFlags.clicked & ControllerFlag_LStickHighLeft)
                {
                    Sd_SfxPlay(Sfx_MenuMove, Q8(0.0f), Q8(0.25f));
                    g_ExtraOptionsMenu_SelectedBloodColorEntry += 3;
                }

                // Set config.
                g_ExtraOptionsMenu_SelectedBloodColorEntry = g_ExtraOptionsMenu_SelectedBloodColorEntry % BloodColorMenuEntry_Count;
                switch (g_ExtraOptionsMenu_SelectedBloodColorEntry)
                {
                    case BloodColorMenuEntry_Normal:
                        g_GameWork.config.extraBloodColor = BloodColor_Normal;
                        break;

                    case BloodColorMenuEntry_Green:
                        g_GameWork.config.extraBloodColor = BloodColor_Green;
                        break;

                    case BloodColorMenuEntry_Violet:
                        g_GameWork.config.extraBloodColor = BloodColor_Violet;
                        break;

                    case BloodColorMenuEntry_Black:
                        g_GameWork.config.extraBloodColor = BloodColor_Black;
                        break;
                }
                break;

            case ExtraOptionsMenuEntry_ViewCtrl:
                // Scroll left/right.
                if (g_Controller0->buttonFlags.clicked & (ControllerFlag_LStickHighRight | ControllerFlag_LStickHighLeft))
                {
                    Sd_SfxPlay(Sfx_MenuMove, Q8(0.0f), Q8(0.25f));

                    // Set config.
                    g_GameWork.config.extraViewCtrl = !g_GameWork.config.extraViewCtrl;
                }
                break;

            case ExtraOptionsMenuEntry_RetreatTurn:
                // Scroll left/right.
                if (g_Controller0->buttonFlags.clicked & (ControllerFlag_LStickHighRight | ControllerFlag_LStickHighLeft))
                {
                    Sd_SfxPlay(Sfx_MenuMove, Q8(0.0f), Q8(0.25f));

                    // Set config.
                    g_GameWork.config.extraRetreatTurn = (s8)g_GameWork.config.extraRetreatTurn == 0;
                }
                break;

            case ExtraOptionsMenuEntry_MovementCtrl:
                // Scroll left/right.
                if (g_Controller0->buttonFlags.clicked & (ControllerFlag_LStickHighRight | ControllerFlag_LStickHighLeft))
                {
                    Sd_SfxPlay(Sfx_MenuMove, Q8(0.0f), Q8(0.25f));

                    // Set config.
                    g_GameWork.config.extraWalkRunCtrl = (s8)g_GameWork.config.extraWalkRunCtrl == 0;
                }
                break;

            case ExtraOptionsMenuEntry_AutoAiming:
                // Scroll left/right.
                if (g_Controller0->buttonFlags.clicked & (ControllerFlag_LStickHighRight | ControllerFlag_LStickHighLeft))
                {
                    Sd_SfxPlay(Sfx_MenuMove, Q8(0.0f), Q8(0.25f));

                    // Set config.
                    g_GameWork.config.extraAutoAiming = (s8)g_GameWork.config.extraAutoAiming == 0;
                }
                break;

            case ExtraOptionsMenuEntry_ViewMode:
                // Scroll left/right.
                if (g_Controller0->buttonFlags.clicked & (ControllerFlag_LStickHighRight | ControllerFlag_LStickHighLeft))
                {
                    Sd_SfxPlay(Sfx_MenuMove, Q8(0.0f), Q8(0.25f));

                    // Set config.
                    g_GameWork.config.extraViewMode = !g_GameWork.config.extraViewMode;
                }
                break;

            case ExtraOptionsMenuEntry_BulletMult:
                // Scroll left/right.
                if (g_Controller0->buttonFlags.clicked & ControllerFlag_LStickHighRight)
                {
                    Sd_SfxPlay(Sfx_MenuMove, Q8(0.0f), Q8(0.25f));

                    // Set config.
                    g_GameWork.config.extraBulletAdjust++;
                }
                if (g_Controller0->buttonFlags.clicked & ControllerFlag_LStickHighLeft)
                {
                    Sd_SfxPlay(Sfx_MenuMove, Q8(0.0f), Q8(0.25f));

                    // Set config.
                    g_GameWork.config.extraBulletAdjust = g_GameWork.config.extraBulletAdjust + (g_ExtraOptionsMenu_BulletMultMax - 1);
                }
                g_GameWork.config.extraBulletAdjust = g_GameWork.config.extraBulletAdjust % g_ExtraOptionsMenu_BulletMultMax;
                break;
        }
    }

    // Leave menu.
    if ((g_Controller0->buttonFlags.clicked & (g_GameWorkPtr->config.controllerConfig.cancel |
                                               (ControllerFlag_L2 | ControllerFlag_R2 |
                                                ControllerFlag_L1 | ControllerFlag_R1))) &&
        g_GameWork.gameStateSteps[0] != OptionsMenuState_LeaveExtraOptions)
    {
        if (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.cancel)
        {
            Sd_SfxPlay(Sfx_MenuCancel, Q8(0.0f), Q8(0.25f));
        }
        else
        {
            Sd_SfxPlay(Sfx_MenuConfirm, Q8(0.0f), Q8(0.25f));
        }

        ScreenFade_Start(true, false, false);
        Game_StateStepSet(0, OptionsMenuState_LeaveExtraOptions);
    }
}

void Options_MainOptionsMenu_Control(void) // 0x801E3770
{
    #define SOUND_VOL_STEP 8

    s32 audioType;
    s32 vol;

    // Draw graphics.
    Options_MainOptionsMenu_EntryStringsDraw();
    Options_MainOptionsMenu_ConfigDraw();

#if VERSION_REGION_IS(NTSCJ)
    #define BRIGHTNESS_IMG_FILE FILE_TIM_OP_BRT_TIM
    func_801E794C();
#else
    #define BRIGHTNESS_IMG_FILE FILE_TIM_OP_BRT_E_TIM
#endif
    
    Options_MainOptionsMenu_SelectionHighlightDraw();
    Options_Menu_BackgroundDraw();
    Screen_BackgroundImgDraw(&g_ItemInspectionImg);
    Options_MainOptionsMenu_BgmVolumeBarDraw();
    Options_MainOptionsMenu_SfxVolumeBarDraw();

    if (g_GameWork.gameStateSteps[0] != OptionsMenuState_MainOptions)
    {
        return;
    }

    // Increment line move timer.
    if ((LINE_CURSOR_TIMER_MAX - 1) < g_Options_SelectionHighlightTimer)
    {
        g_Options_SelectionHighlightTimer = LINE_CURSOR_TIMER_MAX;
    }
    else
    {
        g_Options_SelectionHighlightTimer++;
    }

    if (g_Options_SelectionHighlightTimer != LINE_CURSOR_TIMER_MAX)
    {
        return;
    }

    g_MainOptionsMenu_PrevSelectedEntry = g_MainOptionsMenu_SelectedEntry;

    // Leave to gameplay (if options menu was accessed with `Option` input action).
    if (g_GameWork.gameStatePrev == GameState_InGame &&
        !(g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.enter) &&
        (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.option))
    {
        Sd_SfxPlay(Sfx_MenuCancel, Q8(0.0f), Q8(0.25f));
        Game_StateStepSet(0, OptionsMenuState_Leave);
        return;
    }

    // Move selection cursor up/down.
    if (g_Controller0->buttonFlags.pulsed & ControllerFlag_LStickHighUp)
    {
        Sd_SfxPlay(Sfx_MenuMove, Q8(0.0f), Q8(0.25f));

        g_Options_SelectionHighlightTimer = 0;

#if VERSION_REGION_IS(PAL)
        // Skip over the language entry when it can't be used.
        if (!g_Options_IsLanguageSelectable && g_MainOptionsMenu_SelectedEntry == MainOptionsMenuEntry_Exit)
        {
            g_MainOptionsMenu_SelectedEntry = -1;
        }
#endif

        g_MainOptionsMenu_SelectedEntry   = (g_MainOptionsMenu_SelectedEntry + (MainOptionsMenuEntry_Count - 1)) % MainOptionsMenuEntry_Count;
    }
    if (g_Controller0->buttonFlags.pulsed & ControllerFlag_LStickHighDown)
    {
        Sd_SfxPlay(Sfx_MenuMove, Q8(0.0f), Q8(0.25f));

        g_Options_SelectionHighlightTimer = 0;

#if VERSION_REGION_IS(PAL)
        if (!g_Options_IsLanguageSelectable && g_MainOptionsMenu_SelectedEntry == MainOptionsMenuEntry_SfxVolume)
        {
            g_MainOptionsMenu_SelectedEntry = MainOptionsMenuEntry_Language;
        }
#endif

        g_MainOptionsMenu_SelectedEntry   = (g_MainOptionsMenu_SelectedEntry + 1) % MainOptionsMenuEntry_Count;
    }

    // Handle config change.
    switch (g_MainOptionsMenu_SelectedEntry)
    {
        case MainOptionsMenuEntry_Exit:
            // Exit menu to gameplay.
            if (g_Controller0->buttonFlags.clicked & (g_GameWorkPtr->config.controllerConfig.enter |
                                                      g_GameWorkPtr->config.controllerConfig.cancel))
            {
                Sd_SfxPlay(Sfx_MenuCancel, Q8(0.0f), Q8(0.25f));
                Game_StateStepSet(0, OptionsMenuState_Leave);
            }
            break;

        case MainOptionsMenuEntry_Controller:
            // Enter controller screen.
            if (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.enter)
            {
                Sd_SfxPlay(Sfx_MenuConfirm, Q8(0.0f), Q8(0.25f));
                Fs_QueueStartReadTim(FILE_TIM_OPTION2_TIM, IMAGE_BUFFER_3, &g_ControllerButtonAtlasImg);
                ScreenFade_Start(true, false, false);
                Game_StateStepSet(0, OptionsMenuState_EnterController);
            }
            break;

        case MainOptionsMenuEntry_ScreenPosition:
            // Enter screen position screen.
            if (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.enter)
            {
                Sd_SfxPlay(Sfx_MenuConfirm, Q8(0.0f), Q8(0.25f));
                ScreenFade_Start(true, false, false);
                Game_StateStepSet(0, OptionsMenuState_EnterScreenPos);
            }
            break;

        case MainOptionsMenuEntry_Brightness:
            if (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.enter)
            {
                Sd_SfxPlay(Sfx_MenuConfirm, Q8(0.0f), Q8(0.25f));
#if !VERSION_REGION_IS(PAL)
                if (g_GameWork.gameStatePrev == GameState_MainMenu)
                {
                    Fs_QueueStartReadTim(BRIGHTNESS_IMG_FILE, IMAGE_BUFFER_3, &g_BrightnessScreenImg0);
                }
                else
                {
                    Fs_QueueStartReadTim(BRIGHTNESS_IMG_FILE, IMAGE_BUFFER_3, &g_BrightnessScreenImg1);
                }
#endif

                ScreenFade_Start(true, false, false);
                Game_StateStepSet(0, OptionsMenuState_EnterBrightness);
            }
            break;

        case MainOptionsMenuEntry_Vibration:
            if (g_Controller0->buttonFlags.clicked & (ControllerFlag_LStickHighRight | ControllerFlag_LStickHighLeft))
            {
                Sd_SfxPlay(Sfx_MenuMove, Q8(0.0f), Q8(0.25f));
                g_GameWork.config.vibrationEnabled = !g_GameWork.config.vibrationEnabled << 7;
            }
            break;

        case MainOptionsMenuEntry_AutoLoad:
            if (g_Controller0->buttonFlags.clicked & (ControllerFlag_LStickHighRight | ControllerFlag_LStickHighLeft))
            {
                Sd_SfxPlay(Sfx_MenuMove, Q8(0.0f), Q8(0.25f));
                g_GameWork.config.autoLoad = (s8)g_GameWork.config.autoLoad == 0;
            }
            break;

        case MainOptionsMenuEntry_Sound:
            if (g_Controller0->buttonFlags.clicked & (ControllerFlag_LStickHighRight | ControllerFlag_LStickHighLeft))
            {
                Sd_SfxPlay(Sfx_MenuMove, Q8(0.0f), Q8(0.25f));

                // Set config.
                audioType                           = AudioMode_Stereo;
                g_GameWork.config.soundType = !g_GameWork.config.soundType;
                if (g_GameWork.config.soundType)
                {
                    audioType = AudioMode_Mono;
                }
                SD_Call(audioType);
            }
            break;

        case MainOptionsMenuEntry_BgmVolume:
            vol = g_GameWork.config.volumeBgm;

            if ((vol < OPT_SOUND_VOLUME_MAX && (g_Controller0->buttonFlags.pulsed & ControllerFlag_LStickHighRight)) ||
                (vol > 0                    && (g_Controller0->buttonFlags.pulsed & ControllerFlag_LStickHighLeft)))
            {
                SD_Call(Sfx_MenuMove);
            }
            if ((vol == OPT_SOUND_VOLUME_MAX && (g_Controller0->buttonFlags.clicked & ControllerFlag_LStickHighRight)) ||
                (vol == 0                    && (g_Controller0->buttonFlags.clicked & ControllerFlag_LStickHighLeft)))
            {
                SD_Call(Sfx_MenuError);
            }

            // Scroll left/right.
            if (g_Controller0->buttonFlags.pulsed & ControllerFlag_LStickHighRight)
            {
                vol += SOUND_VOL_STEP;
            }
            if (g_Controller0->buttonFlags.pulsed & ControllerFlag_LStickHighLeft)
            {
                vol -= SOUND_VOL_STEP;
            }

            // Set config.
            vol = CLAMP(vol, 0, OPT_SOUND_VOLUME_MAX);
            Sd_GlobalVolumeSet(OPT_SOUND_VOLUME_MAX, vol, g_GameWork.config.volumeSe);
            g_GameWork.config.volumeBgm = vol;
            break;

        case MainOptionsMenuEntry_SfxVolume:
            vol = g_GameWork.config.volumeSe;

            if ((vol < OPT_SOUND_VOLUME_MAX && (g_Controller0->buttonFlags.pulsed & ControllerFlag_LStickHighRight)) ||
                (vol > 0                    && (g_Controller0->buttonFlags.pulsed & ControllerFlag_LStickHighLeft)))
            {
                SD_Call(Sfx_MenuMove);
            }
            if ((vol == OPT_SOUND_VOLUME_MAX && (g_Controller0->buttonFlags.clicked & ControllerFlag_LStickHighRight)) ||
                (vol == 0                    && (g_Controller0->buttonFlags.clicked & ControllerFlag_LStickHighLeft)))
            {
                SD_Call(Sfx_MenuError);
            }

            if (g_Controller0->buttonFlags.pulsed & ControllerFlag_LStickHighRight)
            {
                vol = vol + SOUND_VOL_STEP;
            }
            if (g_Controller0->buttonFlags.pulsed & ControllerFlag_LStickHighLeft)
            {
                vol = vol - SOUND_VOL_STEP;
            }

            vol = CLAMP(vol, 0, OPT_SOUND_VOLUME_MAX);

            Sd_GlobalVolumeSet(OPT_SOUND_VOLUME_MAX, vol, g_GameWork.config.volumeSe);
            g_GameWork.config.volumeSe = vol;
            break;

#if VERSION_REGION_IS(PAL)
        case MainOptionsMenuEntry_Language:
            if (g_Controller0->buttonFlags.clicked & (ControllerFlag_LStickHighRight | ControllerFlag_LStickHighLeft))
            {
                Sd_SfxPlay(Sfx_MenuMove, Q8(0.0f), Q8(0.25f));

                // Cycle through the 5 languages.
                if (g_Controller0->buttonFlags.clicked & ControllerFlag_LStickHighRight)
                {
                    g_GameWork.config.palLanguageId++;
                }
                else
                {
                    g_GameWork.config.palLanguageId += 4;
                }

                g_GameWork.config.palLanguageId %= 5;
                D_800A93B8                  = g_GameWork.config.palLanguageId;
            }
            break;
#endif

        default:
            break;
    }

#if VERSION_REGION_IS(PAL)
    if (!(g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.enter) &&
        (g_Controller0->buttonFlags.clicked & (ControllerFlag_L2 | ControllerFlag_R2 |
                                               ControllerFlag_L1 | ControllerFlag_R1)))
#else
    vol = 0;

    if (g_Controller0->buttonFlags.clicked & (ControllerFlag_L2 | ControllerFlag_R2 |
                                              ControllerFlag_L1 | ControllerFlag_R1))
#endif
    {
        if (g_GameWork.gameStateSteps[0] == OptionsMenuState_EnterExtraOptions)
        {
            return;
        }

        Sd_SfxPlay(Sfx_MenuConfirm, Q8(0.0f), Q8(0.25f));

        ScreenFade_Start(true, false, false);
        Game_StateStepSet(0, OptionsMenuState_EnterExtraOptions);
    }

    // Reset selection cursor.
    if (((g_GameWork.gameStateSteps[0] != OptionsMenuState_EnterExtraOptions && g_MainOptionsMenu_SelectedEntry != MainOptionsMenuEntry_Exit) &&
         !(g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.enter)) &&
        (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.cancel))
    {
        Sd_SfxPlay(Sfx_MenuCancel, Q8(0.0f), Q8(0.25f));

        g_Options_SelectionHighlightTimer = 0;
        g_MainOptionsMenu_SelectedEntry   = MainOptionsMenuEntry_Exit;
    }

    #undef BRIGHTNESS_IMG_FILE
    #undef SOUND_VOL_STEP
}

void Options_MainOptionsMenu_BgmVolumeBarDraw(void) // 0x801E3F68
{
    Options_MainOptionsMenu_VolumeBarDraw(false, g_GameWork.config.volumeBgm);
}

void Options_MainOptionsMenu_SfxVolumeBarDraw(void) // 0x801E3F90
{
    Options_MainOptionsMenu_VolumeBarDraw(true, g_GameWork.config.volumeSe);
}

#if VERSION_IS(EUR)
INCLUDE_ASM("screens/options/nonmatchings/options", Options_MainOptionsMenu_VolumeBarDraw);
#else
void Options_MainOptionsMenu_VolumeBarDraw(bool isSfx, u8 vol) // 0x801E3FB8
{
    #define STR_OFFSET_Y 16
    #define NOTCH_SIZE_X 5
    #define NOTCH_COUNT  16

    s32      x0Offset;
    s32      x0;
    s32      offset;
    s32      yOffset;
    s32      localVol;
    s32      i;
    s32      j;
    u32      colorComp;
    s32      xOffset;
    s32      color0;
    s32      color1;
    s32      color2;
    GsOT*    ot;
    POLY_F4* poly;

    ot       = &g_OrderingTable2[g_ActiveBufferIdx];
    localVol = vol;

    // Draw bar notches.
    for (i = 0; i < NOTCH_COUNT; i++)
    {
        colorComp = localVol & 0x7;
        colorComp = (colorComp * 12) + 64;

        for (j = 1; j >= 0; j--)
        {
            poly = (POLY_F4*)GsOUT_PACKET_P;
            setPolyF4(poly);

            if (i < (vol / 8))
            {
                color0 = 0xA0 + (0x40 * j);
                setRGBC0(poly, color0, color0, color0, PRIM_POLY | RECT_SIZE_1);
            }
            else if (i > (vol / 8))
            {
                color1 = 0x40 + (0x40 * j);
                setRGBC0(poly, color1, color1, color1, PRIM_POLY | RECT_SIZE_1);
            }
            else
            {
                color2 = colorComp + (0x40 * j);
                setRGBC0(poly, color2, color2, color2, PRIM_POLY | RECT_SIZE_1);
            }

            xOffset = 24 + (i * 6);
            offset  = -69 - MENU_SHIFT_Y;

            x0Offset = j + 24;
            x0       = (x0Offset + (i * 6)) & 0xFFFF;
            yOffset  = j + (56 + MENU_SHIFT_Y);

            setXY0Fast(poly, x0,                           (isSfx * STR_OFFSET_Y) + yOffset);
            setXY1Fast(poly, x0,                           (isSfx * STR_OFFSET_Y) - (j + offset));
            setXY2Fast(poly, (xOffset - j) + NOTCH_SIZE_X, (isSfx * STR_OFFSET_Y) + yOffset);
            setXY3Fast(poly, (xOffset - j) + NOTCH_SIZE_X, (isSfx * STR_OFFSET_Y) - (j + offset));
            addPrim((u8*)ot->org + LAYER_24, poly);
            GsOUT_PACKET_P = (u8*)poly + sizeof(POLY_F4);
        }
    }

    #undef STR_OFFSET_Y
    #undef NOTCH_SIZE_X
    #undef NOTCH_COUNT
}
#endif

void Options_ExtraOptionsMenu_EntryStringsDraw(void) // 0x801E416C
{
    #define LINE_BASE_X   (SCREEN_WIDTH / 5)
    #define LINE_BASE_Y   (64 + EXTRA_MENU_SHIFT_Y)
    #define LINE_OFFSET_X 16
    #define LINE_OFFSET_Y 16

#if VERSION_REGION_IS(PAL)
    #define UNUSED_LINE_BASE_Y 56
#else
    #define UNUSED_LINE_BASE_Y LINE_BASE_Y
#endif

    s32            i;
    static DVECTOR selectionHighlightFromUnused;
    static DVECTOR selectionHighlightToUnused;

#if VERSION_REGION_IS(NTSCJ)
    const DVECTOR STR_POS = { 86, 12 };
#else
    const DVECTOR STR_POS = { 86, 20 };
#endif

    const char* EXTRA_OPTIONS_STR = "EXTRA_OPTION_\x01\x01\x01\x01\x01S";

    const char* ENTRY_STRS[] = {
        "Weapon_Control",
        "Blood_Color",
        "View_Control",
        "Retreat_Turn",
        "\x01W"
        "\x01"
        "a\x01l\x01k/R\x01\x01u\x01n_\x01\x01\x01\x01"
        "Co\x01n\x01t\x01ro\x01l",
        "Auto_Aiming",
        "View_Mode",
        "Bullet_Adjust"
    };

    // @unused Likely an older implementation for active highlight selection position reference setup found in `Options_ExtraOptionsMenu_SelectionHighlightDraw`.
    if (g_Options_SelectionHighlightTimer == 0)
    {
        selectionHighlightFromUnused.vx = LINE_BASE_X - LINE_OFFSET_X;
        selectionHighlightToUnused.vx   = LINE_BASE_X;
        selectionHighlightFromUnused.vy = ((u16)g_MainOptionsMenu_PrevSelectedEntry * LINE_OFFSET_Y) + UNUSED_LINE_BASE_Y;
        selectionHighlightToUnused.vy   = ((u16)g_MainOptionsMenu_SelectedEntry     * LINE_OFFSET_Y) + UNUSED_LINE_BASE_Y;
    }
    Math_Sin(g_Options_SelectionHighlightTimer << 7);

    // Draw heading string.
    Gfx_StringColorSet(StringColorId_White);
    Gfx_StringPositionSet(STR_POS.vx, STR_POS.vy);
    Gfx_StringLayerIdxSet(OPTIONS_ENTRY_STRING_LAYER_IDX);
    Gfx_StringDraw(EXTRA_OPTIONS_STR, DEFAULT_MAP_MESSAGE_LENGTH);

    // Draw entry strings.
    for (i = 0; i < g_ExtraOptionsMenu_EntryCount; i++)
    {
        Gfx_StringPositionSet(LINE_BASE_X, LINE_BASE_Y + (i * LINE_OFFSET_Y));
        Gfx_StringLayerIdxSet(OPTIONS_ENTRY_STRING_LAYER_IDX);
        Gfx_StringDraw(ENTRY_STRS[i], DEFAULT_MAP_MESSAGE_LENGTH);
    }

    #undef LINE_BASE_X
    #undef LINE_BASE_Y
    #undef LINE_OFFSET_X
    #undef LINE_OFFSET_Y
    #undef UNUSED_LINE_BASE_Y
}

void Options_MainOptionsMenu_EntryStringsDraw(void) // 0x801E42EC
{
    #define LINE_BASE_X   64
    #define LINE_BASE_Y   (56 + MENU_SHIFT_Y)
    #define LINE_OFFSET_X 16
    #define LINE_OFFSET_Y 16

    s32            i;
    static DVECTOR selectionHighlightFromUnused;
    static DVECTOR selectionHighlightToUnused;

#if VERSION_REGION_IS(NTSCJ)
    DVECTOR strPos = { 121, 12 };
#else
    DVECTOR strPos = { 121, 20 };
#endif

    const char* OPTIONS_STR = "OPTION_\x01\x01\x01\x01\x01S";

    const char* ENTRY_STRS[] = {
        "Exit",
        "Brightness_Level",
#if VERSION_REGION_IS(NTSCJ)
        "Key_Config",
#else
        "Controller_Config",
#endif
        "Screen_Position",
        "Vibration",
        "Auto_Load",
        "Sound",
        "BGM_Volume",
        "SE_Volume",
#if VERSION_REGION_IS(PAL)
        "Language"
#endif
    };

    // @unused Likely an older implementation for active highlight selection position reference setup found in `Options_MainOptionsMenu_SelectionHighlightDraw`.
    if (g_Options_SelectionHighlightTimer == 0)
    {
        selectionHighlightFromUnused.vx = LINE_BASE_X - LINE_OFFSET_X;
        selectionHighlightToUnused.vx   = LINE_BASE_X;
        selectionHighlightFromUnused.vy = ((u16)g_MainOptionsMenu_PrevSelectedEntry * LINE_OFFSET_Y) + LINE_BASE_Y;
        selectionHighlightToUnused.vy   = ((u16)g_MainOptionsMenu_SelectedEntry     * LINE_OFFSET_Y) + LINE_BASE_Y;
    }
    Math_Sin(g_Options_SelectionHighlightTimer << 7);

    // Draw heading string.
    Gfx_StringColorSet(StringColorId_White);
    Gfx_StringPositionSet(strPos.vx, strPos.vy);
    Gfx_StringLayerIdxSet(OPTIONS_ENTRY_STRING_LAYER_IDX);
    Gfx_StringDraw(OPTIONS_STR, DEFAULT_MAP_MESSAGE_LENGTH);

    // Draw entry strings.
    for (i = 0; i < MainOptionsMenuEntry_Count; i++)
    {
        Gfx_StringPositionSet(LINE_BASE_X, LINE_BASE_Y + (i * LINE_OFFSET_Y));
        Gfx_StringLayerIdxSet(OPTIONS_ENTRY_STRING_LAYER_IDX);

#if VERSION_REGION_IS(PAL)
        if (i == MainOptionsMenuEntry_Language && !g_Options_IsLanguageSelectable)
        {
            Gfx_StringColorSet(StringColorId_LightGrey);
        }
#endif

        Gfx_StringDraw(ENTRY_STRS[i], DEFAULT_MAP_MESSAGE_LENGTH);
    }

#if VERSION_REGION_IS(PAL)
    Gfx_StringColorSet(StringColorId_White);
#endif

    Gfx_StringLayerIdxReset();

    #undef LINE_BASE_X
    #undef LINE_BASE_Y
    #undef LINE_OFFSET_X
    #undef LINE_OFFSET_Y
}

void Options_ExtraOptionsMenu_SelectionHighlightDraw(void) // 0x801E4450
{
    #define BULLET_QUAD_COUNT  2
    #define LINE_BASE_X        64
    #define LINE_BASE_Y        56
    #define LINE_OFFSET_X      16
    #define LINE_OFFSET_Y      16
    #define HIGHLIGHT_OFFSET_X -121
    #define HIGHLIGHT_OFFSET_Y (50 - EXTRA_MENU_SHIFT_Y)

    s32            i;
    s32            j;
    q3_12          interpAlpha;
    s_Line2d       highlightLine;
    s_Quad2d       bulletQuads[BULLET_QUAD_COUNT];
    DVECTOR*       quadVerts;
    static DVECTOR selectionHighlightFrom;
    static DVECTOR selectionHighlightTo;

    const u8 SELECTION_HIGHLIGHT_WIDTHS[] = {
        157, 126, 135, 135, 157, 130, 112, 134
    };

    // 12x12 quad.
#if VERSION_REGION_IS(NTSCJ)
    const DVECTOR FRONT_BULLET_QUAD[] = {
        { -120, -67 },
        { -120, -55 },
        { -108, -67 },
        { -108, -55 }
    };
#else
    const DVECTOR FRONT_BULLET_QUAD[] = {
        { -120, -47 },
        { -120, -35 },
        { -108, -47 },
        { -108, -35 }
    };
#endif

    // 14x14 quad.
#if VERSION_REGION_IS(NTSCJ)
    const DVECTOR BACK_BULLET_QUAD[] = {
        { -121, -68 },
        { -121, -54 },
        { -107, -68 },
        { -107, -54 }
    };
#else
    const DVECTOR BACK_BULLET_QUAD[] = {
        { -121, -48 },
        { -121, -34 },
        { -107, -48 },
        { -107, -34 }
    };
#endif

    // Set active selection highlight position references.
    if (g_Options_SelectionHighlightTimer == 0)
    {
        selectionHighlightFrom.vx = SELECTION_HIGHLIGHT_WIDTHS[g_ExtraOptionsMenu_PrevSelectedEntry] + (65536 + HIGHLIGHT_OFFSET_X); // TODO
        selectionHighlightFrom.vy = ((u16)g_ExtraOptionsMenu_PrevSelectedEntry * LINE_OFFSET_Y)      - HIGHLIGHT_OFFSET_Y;
        selectionHighlightTo.vx   = SELECTION_HIGHLIGHT_WIDTHS[g_ExtraOptionsMenu_SelectedEntry]     + (65536 + HIGHLIGHT_OFFSET_X); // TODO
        selectionHighlightTo.vy   = ((u16)g_ExtraOptionsMenu_SelectedEntry * LINE_OFFSET_Y)          - HIGHLIGHT_OFFSET_Y;
    }

    // Compute sine-based interpolation alpha.
    interpAlpha = Math_Sin(g_Options_SelectionHighlightTimer << 7);

    // Draw active selection highlight.
    highlightLine.vertex0.vx = HIGHLIGHT_OFFSET_X;
    highlightLine.vertex1.vx = selectionHighlightFrom.vx +
                               Q12_MULT(selectionHighlightTo.vx - selectionHighlightFrom.vx, interpAlpha);
    highlightLine.vertex1.vy = selectionHighlightFrom.vy +
                               Q12_MULT(selectionHighlightTo.vy - selectionHighlightFrom.vy, interpAlpha) +
                               LINE_OFFSET_Y;
    highlightLine.vertex0.vy = highlightLine.vertex1.vy;
    Options_Selection_HighlightDraw(&highlightLine, true, false);

    // Draw selection bullet points.
    for (i = 0; i < g_ExtraOptionsMenu_EntryCount; i++)
    {
        // Set bullet quads.
        quadVerts = (DVECTOR*)&bulletQuads;
        for (j = 0; j < RECT_VERT_COUNT; j++)
        {
            quadVerts[j].vx                   = FRONT_BULLET_QUAD[j].vx;
            quadVerts[j].vy                   = FRONT_BULLET_QUAD[j].vy + (i * LINE_OFFSET_Y);
            quadVerts[j + sizeof(DVECTOR)].vx = BACK_BULLET_QUAD[j].vx;
            quadVerts[j + sizeof(DVECTOR)].vy = BACK_BULLET_QUAD[j].vy + (i * LINE_OFFSET_Y);
        }

        // Active selection bullet point.
        if (i == g_ExtraOptionsMenu_SelectedEntry)
        {
            Options_Selection_BulletPointDraw(&bulletQuads[0], false, false);
            Options_Selection_BulletPointDraw(&bulletQuads[1], true,  false);
        }
        // Inactive selection bullet point.
        else
        {
            Options_Selection_BulletPointDraw(&bulletQuads[0], false, true);
            Options_Selection_BulletPointDraw(&bulletQuads[1], true,  true);
        }
    }

    #undef BULLET_QUAD_COUNT
    #undef LINE_BASE_X
    #undef LINE_BASE_Y
    #undef LINE_OFFSET_X
    #undef LINE_OFFSET_Y
    #undef HIGHLIGHT_OFFSET_X
    #undef HIGHLIGHT_OFFSET_Y
}

#if VERSION_IS(EUR)
// @hack Statics of the included function below, remove once decompiled.
// Declared inside an uncalled function so the compiler emits them here instead of at the end of the file.
static inline void Options_MainOptionsMenu_SelectionHighlightDraw_Statics(void)
{
    static DVECTOR mainOptions_SelectionHighlightFrom __asm__("mainOptions_SelectionHighlightFrom");
    static DVECTOR mainOptions_SelectionHighlightTo __asm__("mainOptions_SelectionHighlightTo");

    (void)mainOptions_SelectionHighlightFrom;
    (void)mainOptions_SelectionHighlightTo;
}
INCLUDE_RODATA("screens/options/nonmatchings/options", D_801E286C);
INCLUDE_RODATA("screens/options/nonmatchings/options", D_801E2878);
INCLUDE_RODATA("screens/options/nonmatchings/options", D_801E2888);
INCLUDE_ASM("screens/options/nonmatchings/options", Options_MainOptionsMenu_SelectionHighlightDraw);
#else
void Options_MainOptionsMenu_SelectionHighlightDraw(void) // 0x801E472C
{
    #define LINE_OFFSET_Y      16
    #define HIGHLIGHT_OFFSET_X -121
    #define HIGHLIGHT_OFFSET_Y (58 - MENU_SHIFT_Y)

    s32            i;
    s32            j;
    s16            interpAlpha;
    s_Line2d       highlightLine;
    s_Quad2d       bulletQuads[2];
    DVECTOR*       quadVerts;
    static DVECTOR selectionHighlightFrom;
    static DVECTOR selectionHighlightTo;

#if VERSION_REGION_IS(NTSCJ)
    const u8 SELECTION_HIGHLIGHT_WIDTHS[] = {
        59, 169, 119, 156, 104, 112, 75, 129, 112
    };
#else
    const u8 SELECTION_HIGHLIGHT_WIDTHS[] = {
        59, 169, 174, 156, 104, 112, 75, 129, 112
    };
#endif

    // 12x12 quad.
#if VERSION_REGION_IS(NTSCJ)
    const DVECTOR BULLET_QUAD_VERTS_FRONT[] = {
        { -120, -75 },
        { -120, -63 },
        { -108, -75 },
        { -108, -63 }
    };
#else
    const DVECTOR BULLET_QUAD_VERTS_FRONT[] = {
        { -120, -55 },
        { -120, -43 },
        { -108, -55 },
        { -108, -43 }
    };
#endif

    // 14x14 quad.
#if VERSION_REGION_IS(NTSCJ)
    const DVECTOR BULLET_QUAD_VERTS_BACK[] = {
        { -121, -76 },
        { -121, -62 },
        { -107, -76 },
        { -107, -62 }
    };
#else
    const DVECTOR BULLET_QUAD_VERTS_BACK[] = {
        { -121, -56 },
        { -121, -42 },
        { -107, -56 },
        { -107, -42 }
    };
#endif

    // Set active selection highlight position references.
    if (g_Options_SelectionHighlightTimer == 0)
    {
        selectionHighlightFrom.vx = SELECTION_HIGHLIGHT_WIDTHS[g_MainOptionsMenu_PrevSelectedEntry] + (65536 + HIGHLIGHT_OFFSET_X); // TODO
        selectionHighlightFrom.vy = ((u16)g_MainOptionsMenu_PrevSelectedEntry * LINE_OFFSET_Y)      - HIGHLIGHT_OFFSET_Y;
        selectionHighlightTo.vx   = SELECTION_HIGHLIGHT_WIDTHS[g_MainOptionsMenu_SelectedEntry]     + (65536 + HIGHLIGHT_OFFSET_X); // TODO
        selectionHighlightTo.vy   = ((u16)g_MainOptionsMenu_SelectedEntry * LINE_OFFSET_Y)          - HIGHLIGHT_OFFSET_Y;
    }

    // Compute sine-based interpolation alpha.
    interpAlpha = Math_Sin(g_Options_SelectionHighlightTimer << 7);

    // Draw active selection highlight.
    highlightLine.vertex0.vx = HIGHLIGHT_OFFSET_X;
    highlightLine.vertex1.vx = selectionHighlightFrom.vx +
                               FP_FROM((selectionHighlightTo.vx - selectionHighlightFrom.vx) * interpAlpha, Q12_SHIFT);
    highlightLine.vertex1.vy = selectionHighlightFrom.vy +
                               FP_FROM((selectionHighlightTo.vy - selectionHighlightFrom.vy) * interpAlpha, Q12_SHIFT) +
                               LINE_OFFSET_Y;
    highlightLine.vertex0.vy = highlightLine.vertex1.vy;
    Options_Selection_HighlightDraw(&highlightLine, true, false);

    // Draw selection bullet points.
    for (i = 0; i < MainOptionsMenuEntry_Count; i++)
    {
        // Set bullet quads.
        quadVerts = (DVECTOR*)&bulletQuads;
        for (j = 0; j < RECT_VERT_COUNT; j++)
        {
            quadVerts[j].vx                   = BULLET_QUAD_VERTS_FRONT[j].vx;
            quadVerts[j].vy                   = BULLET_QUAD_VERTS_FRONT[j].vy + (i * LINE_OFFSET_Y);
            quadVerts[j + sizeof(DVECTOR)].vx = BULLET_QUAD_VERTS_BACK[j].vx;
            quadVerts[j + sizeof(DVECTOR)].vy = BULLET_QUAD_VERTS_BACK[j].vy + (i * LINE_OFFSET_Y);
        }

        // Active selection bullet point.
        if (i == g_MainOptionsMenu_SelectedEntry)
        {
            Options_Selection_BulletPointDraw(&bulletQuads[0], false, false);
            Options_Selection_BulletPointDraw(&bulletQuads[1], true,  false);
        }
        // Inactive selection bullet point.
        else
        {
            Options_Selection_BulletPointDraw(&bulletQuads[0], false, true);
            Options_Selection_BulletPointDraw(&bulletQuads[1], true,  true);
        }
    }

    #undef LINE_OFFSET_Y
    #undef HIGHLIGHT_OFFSET_X
    #undef HIGHLIGHT_OFFSET_Y
}
#endif

#if VERSION_IS(EUR)
INCLUDE_ASM("screens/options/nonmatchings/options", Options_Menu_BackgroundDraw);
#else
void Options_Menu_BackgroundDraw(void) // 0x801E49F0
{
    s32      y0;
    s32      y1;
    s32      xy2;
    s32      xy3;
    s32      i;
    GsOT*    ot;
    POLY_G4* poly;

    ot = &g_OrderingTable0[g_ActiveBufferIdx];

    xy3 = 160 + (0xFFA0 << 16); // TODO: -96
    xy2 = 160 + (0xFF90 << 16); // TODO: -112
    y1  = 0xFFA0 << 16;         // TODO: -96
    y0  = 0xFF90 << 16;         // TODO: -112

    for (i = 0; i < 2; i++)
    {
        poly = (POLY_G4*)GsOUT_PACKET_P;

        setPolyG4(poly);
        setSemiTrans(poly, true);

        setRGB0(poly, 0x60, 0x60, 0x60);
        setRGB1(poly, 0, 0, 0);
        setRGB2(poly, 0x60, 0x60, 0x60);
        setRGB3(poly, 0, 0, 0);

        *(u32*)(&poly->x0) = 0xFF60 + (y0 + ((0xE0 * i) << 16)); // TODO: -160
        *(u32*)(&poly->x1) = 0xFF60 + (y1 + ((0xA8 * i) << 16)); // TODO: -160
        *(u32*)(&poly->x2) = xy2    + ((0xE0 * i) << 16);
        *(u32*)(&poly->x3) = xy3    + ((0xA8 * i) << 16);

        addPrim((u8*)ot->org + LAYER_8148, poly);
        GsOUT_PACKET_P = (u8*)poly + sizeof(POLY_G4);
    }

    Gfx_Primitive2dTextureSet(0, 0, 2037, 6);
}
#endif

#if VERSION_IS(EUR)
INCLUDE_RODATA("screens/options/nonmatchings/options", D_801E2898);
INCLUDE_RODATA("screens/options/nonmatchings/options", D_801E2958);
INCLUDE_RODATA("screens/options/nonmatchings/options", D_801E2A18);
INCLUDE_RODATA("screens/options/nonmatchings/options", D_801E2A20);
INCLUDE_RODATA("screens/options/nonmatchings/options", D_801E2A28);
INCLUDE_RODATA("screens/options/nonmatchings/options", D_801E2A30);
INCLUDE_RODATA("screens/options/nonmatchings/options", D_801E2A38);
INCLUDE_RODATA("screens/options/nonmatchings/options", D_801E2A40);
INCLUDE_RODATA("screens/options/nonmatchings/options", D_801E2A48);
INCLUDE_RODATA("screens/options/nonmatchings/options", D_801E2A4C);
INCLUDE_RODATA("screens/options/nonmatchings/options", D_801E2A54);
INCLUDE_RODATA("screens/options/nonmatchings/options", D_801E2A58);
INCLUDE_RODATA("screens/options/nonmatchings/options", D_801E2A5C);
INCLUDE_RODATA("screens/options/nonmatchings/options", D_801E2A68);
INCLUDE_RODATA("screens/options/nonmatchings/options", D_801E2A6C);
INCLUDE_RODATA("screens/options/nonmatchings/options", D_801E2A70);
INCLUDE_RODATA("screens/options/nonmatchings/options", D_801E2A74);
INCLUDE_RODATA("screens/options/nonmatchings/options", D_801E2A78);
INCLUDE_RODATA("screens/options/nonmatchings/options", D_801E2A7C);
INCLUDE_RODATA("screens/options/nonmatchings/options", D_801E2A80);
INCLUDE_ASM("screens/options/nonmatchings/options", Options_ExtraOptionsMenu_ConfigDraw);
#else
void Options_ExtraOptionsMenu_ConfigDraw(void) // 0x801E4B2C
{
    #define STR_BASE_Y   (64 + EXTRA_MENU_SHIFT_Y)
    #define STR_OFFSET_Y 16

#if VERSION_REGION_IS(NTSCJ)
    const s_Triangle2d FRONT_ARROWS[] = {
        { { 38,  -62 }, { 46,  -70 }, { 46,  -54 } },
        { { 120, -62 }, { 112, -70 }, { 112, -54 } },
        { { 38,  -46 }, { 46,  -54 }, { 46,  -38 } },
        { { 120, -46 }, { 112, -54 }, { 112, -38 } },
        { { 35,  -30 }, { 43,  -38 }, { 43,  -22  } },
        { { 123, -30 }, { 115, -38 }, { 115, -22  } },
        { { 35,   -14  }, { 43,  -22  }, { 43,   -6 } },
        { { 123,  -14  }, { 115, -22  }, { 115,  -6 } },
        { { 35,   2 }, { 43,   -6 }, { 43,   10 } },
        { { 123,  2 }, { 115,  -6 }, { 115,  10 } },
        { { 51,   18 }, { 59,   10 }, { 59,   26 } },
        { { 107,  18 }, { 99,   10 }, { 99,   26 } },
        { { 24,   34 }, { 32,   26 }, { 32,   42 } },
        { { 131,  34 }, { 123,  26 }, { 123,  42 } },
        { { 57,   50 }, { 65,   42 }, { 65,   58 } },
        { { 104,  50 }, { 96,   42 }, { 96,   58 } }
    };
#else
    const s_Triangle2d FRONT_ARROWS[] = {
        { { 38,  -42 }, { 46,  -50 }, { 46,  -34 } },
        { { 120, -42 }, { 112, -50 }, { 112, -34 } },
        { { 38,  -26 }, { 46,  -34 }, { 46,  -18 } },
        { { 120, -26 }, { 112, -34 }, { 112, -18 } },
        { { 35,  -10 }, { 43,  -18 }, { 43,  -2  } },
        { { 123, -10 }, { 115, -18 }, { 115, -2  } },
        { { 35,   6  }, { 43,  -2  }, { 43,   14 } },
        { { 123,  6  }, { 115, -2  }, { 115,  14 } },
        { { 35,   22 }, { 43,   14 }, { 43,   30 } },
        { { 123,  22 }, { 115,  14 }, { 115,  30 } },
        { { 51,   38 }, { 59,   30 }, { 59,   46 } },
        { { 107,  38 }, { 99,   30 }, { 99,   46 } },
        { { 24,   54 }, { 32,   46 }, { 32,   62 } },
        { { 131,  54 }, { 123,  46 }, { 123,  62 } },
        { { 57,   70 }, { 65,   62 }, { 65,   78 } },
        { { 104,  70 }, { 96,   62 }, { 96,   78 } }
    };
#endif

#if VERSION_REGION_IS(NTSCJ)
    const s_Triangle2d BACK_ARROWS[] = {
        { { 37,  -62 }, { 47,  -72 }, { 47,  -52 } },
        { { 121, -62 }, { 111, -72 }, { 111, -52 } },
        { { 37,  -46 }, { 47,  -56 }, { 47,  -36 } },
        { { 121, -46 }, { 111, -56 }, { 111, -36 } },
        { { 34,  -30 }, { 44,  -40 }, { 44,   -20  } },
        { { 124, -30 }, { 114, -40 }, { 114,  -20  } },
        { { 34,   -14  }, { 44,  -24  }, { 44,   -4 } },
        { { 124,  -14  }, { 114, -24  }, { 114,  -4 } },
        { { 34,   2 }, { 44,   -8 }, { 44,   12 } },
        { { 124,  2 }, { 114,  -8 }, { 114,  12 } },
        { { 50,   18 }, { 60,   8 }, { 60,   28 } },
        { { 108,  18 }, { 98,   8 }, { 98,   28 } },
        { { 23,   34 }, { 33,   24 }, { 33,   44 } },
        { { 132,  34 }, { 122,  24 }, { 122,  44 } },
        { { 56,   50 }, { 66,   40 }, { 66,   60 } },
        { { 105,  50 }, { 95,   40 }, { 95,   60 } }
    };
#else
    const s_Triangle2d BACK_ARROWS[] = {
        { { 37,  -42 }, { 47,  -52 }, { 47,  -32 } },
        { { 121, -42 }, { 111, -52 }, { 111, -32 } },
        { { 37,  -26 }, { 47,  -36 }, { 47,  -16 } },
        { { 121, -26 }, { 111, -36 }, { 111, -16 } },
        { { 34,  -10 }, { 44,  -20 }, { 44,   0  } },
        { { 124, -10 }, { 114, -20 }, { 114,  0  } },
        { { 34,   6  }, { 44,  -4  }, { 44,   16 } },
        { { 124,  6  }, { 114, -4  }, { 114,  16 } },
        { { 34,   22 }, { 44,   12 }, { 44,   32 } },
        { { 124,  22 }, { 114,  12 }, { 114,  32 } },
        { { 50,   38 }, { 60,   28 }, { 60,   48 } },
        { { 108,  38 }, { 98,   28 }, { 98,   48 } },
        { { 23,   54 }, { 33,   44 }, { 33,   64 } },
        { { 132,  54 }, { 122,  44 }, { 122,  64 } },
        { { 56,   70 }, { 66,   60 }, { 66,   80 } },
        { { 105,  70 }, { 95,   60 }, { 95,   80 } }
    };
#endif

    // TODO: Can this be split?
    const char* CONFIG_STRS[] = {
        "Press",
        "Switch",

        "Normal",
        "Green",
        "Violet",
        "Black",

        "_",

        "Normal",
        "Reverse",

        "On",
        "Off",

        "Normal",
        "Self_View",

        "x1",
        "x2",
        "x3",
        "x4",
        "x5",
        "x6"
    };

    s32 strPosX;
    s32 i;
    s32 j;

    Gfx_StringColorSet(StringColorId_White);

    // Draw left/right arrows for subset of options.
    if (g_ExtraOptionsMenu_SelectedEntry < (u32)ExtraOptionsMenuEntry_Count)
    {
        // Draw flashing left/right arrows.
        for (i = 0; i < 2; i++)
        {
            Options_Selection_ArrowDraw(&FRONT_ARROWS[(g_ExtraOptionsMenu_SelectedEntry * 2) + i], true, false);
        }

        // Draw border to highlight flashing left/right arrow corresponding to direction of UI navigation.
        if (g_Controller0->buttonFlags.held & ControllerFlag_LStickHighLeft)
        {
            Options_Selection_ArrowDraw(&BACK_ARROWS[g_ExtraOptionsMenu_SelectedEntry << 1], false, false);
        }
        if (g_Controller0->buttonFlags.held & ControllerFlag_LStickHighRight)
        {
            Options_Selection_ArrowDraw(&BACK_ARROWS[(g_ExtraOptionsMenu_SelectedEntry << 1) + 1], false, false);
        }
    }

    // Draw entry strings.
    for (j = 0; j < g_ExtraOptionsMenu_EntryCount; j++)
    {
        switch (j)
        {
            case ExtraOptionsMenuEntry_WeaponCtrl:
                strPosX = (g_GameWork.config.extraWeaponCtrl != 0) ? 217 : 212;
                Gfx_StringPositionSet(strPosX, STR_BASE_Y + (STR_OFFSET_Y * ExtraOptionsMenuEntry_WeaponCtrl));
                Gfx_StringDraw(CONFIG_STRS[!g_GameWork.config.extraWeaponCtrl], 10);
                break;

            case ExtraOptionsMenuEntry_Blood:
                switch (g_ExtraOptionsMenu_SelectedBloodColorEntry)
                {
                    case BloodColorMenuEntry_Normal:
                        Gfx_StringPositionSet(210, STR_BASE_Y + (STR_OFFSET_Y * ExtraOptionsMenuEntry_Blood));
                        break;

                    case BloodColorMenuEntry_Green:
                        Gfx_StringPositionSet(214, STR_BASE_Y + (STR_OFFSET_Y * ExtraOptionsMenuEntry_Blood));
                        break;

                    case BloodColorMenuEntry_Violet:
                        Gfx_StringPositionSet(214, STR_BASE_Y + (STR_OFFSET_Y * ExtraOptionsMenuEntry_Blood));
                        break;

                    case BloodColorMenuEntry_Black:
                        Gfx_StringPositionSet(217, STR_BASE_Y + (STR_OFFSET_Y * ExtraOptionsMenuEntry_Blood));
                        break;
                }

                Gfx_StringDraw(CONFIG_STRS[g_ExtraOptionsMenu_SelectedBloodColorEntry + 2], 10);
                break;

            case ExtraOptionsMenuEntry_ViewCtrl:
                strPosX = !g_GameWork.config.extraViewCtrl ? 210 : 206;
                Gfx_StringPositionSet(strPosX, STR_BASE_Y + (STR_OFFSET_Y * ExtraOptionsMenuEntry_ViewCtrl));
                Gfx_StringDraw(CONFIG_STRS[((g_GameWork.config.extraViewCtrl != 0) ? 32 : 28) >> 2], 10);
                break;

            case ExtraOptionsMenuEntry_RetreatTurn:
                strPosX = !g_GameWork.config.extraRetreatTurn ? 210 : 206;
                Gfx_StringPositionSet(strPosX, STR_BASE_Y + (STR_OFFSET_Y * ExtraOptionsMenuEntry_RetreatTurn));
                Gfx_StringDraw(CONFIG_STRS[((g_GameWork.config.extraRetreatTurn != 0) ? 32 : 28) >> 2], 10);
                break;

            case ExtraOptionsMenuEntry_MovementCtrl:
                strPosX = !g_GameWork.config.extraWalkRunCtrl ? 210 : 206;
                Gfx_StringPositionSet(strPosX, STR_BASE_Y + (STR_OFFSET_Y * ExtraOptionsMenuEntry_MovementCtrl));
                Gfx_StringDraw(CONFIG_STRS[((g_GameWork.config.extraWalkRunCtrl != 0) ? 32 : 28) >> 2], 10);
                break;

            case ExtraOptionsMenuEntry_AutoAiming:
                strPosX = !g_GameWork.config.extraAutoAiming ? 228 : 226;
                Gfx_StringPositionSet(strPosX, STR_BASE_Y + (STR_OFFSET_Y * ExtraOptionsMenuEntry_AutoAiming));
                Gfx_StringDraw(CONFIG_STRS[((g_GameWork.config.extraAutoAiming != 0) ? 40 : 36) >> 2], 10);
                break;

            case ExtraOptionsMenuEntry_ViewMode:
                strPosX = !g_GameWork.config.extraViewMode ? 210 : 200;
                Gfx_StringPositionSet(strPosX, STR_BASE_Y + (STR_OFFSET_Y * ExtraOptionsMenuEntry_ViewMode));
                Gfx_StringDraw(CONFIG_STRS[(g_GameWork.config.extraViewMode ? 48 : 44) >> 2], 10);
                break;

            case ExtraOptionsMenuEntry_BulletMult:
                Gfx_StringPositionSet(230, STR_BASE_Y + (STR_OFFSET_Y * ExtraOptionsMenuEntry_BulletMult));
                Gfx_StringDraw(CONFIG_STRS[g_GameWork.config.extraBulletAdjust + 13], 10);
                break;
        }
    }

    #undef STR_BASE_Y
    #undef STR_OFFSET_Y
}
#endif

#if VERSION_IS(EUR)
INCLUDE_RODATA("screens/options/nonmatchings/options", D_801E2AF0);
INCLUDE_RODATA("screens/options/nonmatchings/options", D_801E2B80);
INCLUDE_RODATA("screens/options/nonmatchings/options", D_801E2C10);
INCLUDE_RODATA("screens/options/nonmatchings/options", D_801E2C18);
INCLUDE_RODATA("screens/options/nonmatchings/options", D_801E2C24);
INCLUDE_RODATA("screens/options/nonmatchings/options", D_801E2C2C);
INCLUDE_RODATA("screens/options/nonmatchings/options", D_801E2C34);
INCLUDE_RODATA("screens/options/nonmatchings/options", D_801E2C3C);
INCLUDE_RODATA("screens/options/nonmatchings/options", D_801E2C44);
INCLUDE_RODATA("screens/options/nonmatchings/options", D_801E2C4C);
INCLUDE_ASM("screens/options/nonmatchings/options", Options_MainOptionsMenu_ConfigDraw);
#else
void Options_MainOptionsMenu_ConfigDraw(void) // 0x801E4FFC
{
#if VERSION_REGION_IS(NTSCJ)
    const s_Triangle2d FRONT_ARROWS[] = {
        { { 40,  -6 }, { 48,  -14  }, { 48,  2 } },
        { { 96,  -6 }, { 88,  -14  }, { 88,  2 } },
        { { 40,  10 }, { 48,  2 }, { 48,  18 } },
        { { 96,  10 }, { 88,  2 }, { 88,  18 } },
        { { 19,  26 }, { 27,  18 }, { 27,  34 } },
        { { 124, 26 }, { 116, 18 }, { 116, 34 } },
        { { 12,  42 }, { 20,  34 }, { 20,  50 } },
        { { 131, 42 }, { 123, 34 }, { 123, 50 } },
        { { 12,  58 }, { 20,  50 }, { 20,  66 } },
        { { 131, 58 }, { 123, 50 }, { 123, 66 } }
    };
#else
    const s_Triangle2d FRONT_ARROWS[] = {
        { { 40,  14 }, { 48,  6  }, { 48,  22 } },
        { { 96,  14 }, { 88,  6  }, { 88,  22 } },
        { { 40,  30 }, { 48,  22 }, { 48,  38 } },
        { { 96,  30 }, { 88,  22 }, { 88,  38 } },
        { { 19,  46 }, { 27,  38 }, { 27,  54 } },
        { { 124, 46 }, { 116, 38 }, { 116, 54 } },
        { { 12,  62 }, { 20,  54 }, { 20,  70 } },
        { { 131, 62 }, { 123, 54 }, { 123, 70 } },
        { { 12,  78 }, { 20,  70 }, { 20,  86 } },
        { { 131, 78 }, { 123, 70 }, { 123, 86 } }
    };
#endif

#if VERSION_REGION_IS(NTSCJ)
    const s_Triangle2d BACK_ARROWS[] = {
        { { 39,  -6 }, { 49,  -16  }, { 49,  4 } },
        { { 97,  -6 }, { 87,  -16  }, { 87,  4 } },
        { { 39,  10 }, { 49,  0 }, { 49,  20 } },
        { { 97,  10 }, { 87,  0 }, { 87,  20 } },
        { { 18,  26 }, { 28,  16 }, { 28,  36 } },
        { { 125, 26 }, { 115, 16 }, { 115, 36 } },
        { { 11,  42 }, { 21,  32 }, { 21,  52 } },
        { { 132, 42 }, { 122, 32 }, { 122, 52 } },
        { { 11,  58 }, { 21,  48 }, { 21,  68 } },
        { { 132, 58 }, { 122, 48 }, { 122, 68 } }
    };
#else
    const s_Triangle2d BACK_ARROWS[] = {
        { { 39,  14 }, { 49,  4  }, { 49,  24 } },
        { { 97,  14 }, { 87,  4  }, { 87,  24 } },
        { { 39,  30 }, { 49,  20 }, { 49,  40 } },
        { { 97,  30 }, { 87,  20 }, { 87,  40 } },
        { { 18,  46 }, { 28,  36 }, { 28,  56 } },
        { { 125, 46 }, { 115, 36 }, { 115, 56 } },
        { { 11,  62 }, { 21,  52 }, { 21,  72 } },
        { { 132, 62 }, { 122, 52 }, { 122, 72 } },
        { { 11,  78 }, { 21,  68 }, { 21,  88 } },
        { { 132, 78 }, { 122, 68 }, { 122, 88 } }
    };
#endif

    const char* CONFIG_STRS[] =
    {
        "On",
        "Off",
        "Stereo",
        "Monaural"
    };

    s32 strPosX;
    s32 strIdx;
    s32 i;

    Gfx_StringColorSet(StringColorId_White);

    // Draw left/right arrows for subset of options.
    if (g_MainOptionsMenu_SelectedEntry >= 4 && g_MainOptionsMenu_SelectedEntry < 9)
    {
        // Draw flashing left/right arrows.
        for (i = 0; i < 2; i++)
        {
            Options_Selection_ArrowDraw(&FRONT_ARROWS[(((g_MainOptionsMenu_SelectedEntry - 4) * 2) + i)], true, false);
        }

        // Draw border to highlight flashing left/right arrow corresponding to direction of UI navigation.
        if (g_Controller0->buttonFlags.held & ControllerFlag_LStickHighLeft)
        {
            Options_Selection_ArrowDraw(&BACK_ARROWS[(g_MainOptionsMenu_SelectedEntry - 4) << 1], false, false);
        }
        if (g_Controller0->buttonFlags.held & ControllerFlag_LStickHighRight)
        {
            Options_Selection_ArrowDraw(&BACK_ARROWS[((g_MainOptionsMenu_SelectedEntry - 4) << 1) + 1], false, false);
        }
    }

    for (i = 0; i < 3; i++)
    {
        switch (i)
        {
            case 0:
                strPosX = !g_GameWork.config.vibrationEnabled ? 214 : 216;
                Gfx_StringPositionSet(strPosX, 120 + MENU_SHIFT_Y);

                strIdx = !g_GameWork.config.vibrationEnabled;
                Gfx_StringDraw(CONFIG_STRS[strIdx], 10);
                break;

            case 1:
                strPosX = !g_GameWork.config.autoLoad ? 214 : 216;
                Gfx_StringPositionSet(strPosX, 136 + MENU_SHIFT_Y);

                strIdx = !g_GameWork.config.autoLoad;
                Gfx_StringDraw(CONFIG_STRS[strIdx], 10);
                break;

            case 2:
                strPosX = g_GameWork.config.soundType ? 194 : 206;
                Gfx_StringPositionSet(strPosX, 152 + MENU_SHIFT_Y);

                strIdx = g_GameWork.config.soundType + 2;
                Gfx_StringDraw(CONFIG_STRS[strIdx], 10);
                break;
        }
    }
}
#endif

// ========================================
// SCREEN POSITION OPTION SCREEN
// ========================================

#if VERSION_IS(EUR)
// @hack Statics of the included function below, remove once decompiled.
// Declared inside an uncalled function so the compiler emits them here instead of at the end of the file.
static inline void Options_ScreenPosMenu_Control_Statics(void)
{
    static s32 screenPosMenu_BackgroundFade __asm__("screenPosMenu_BackgroundFade");
    static s16 screenPosMenu_PositionX __asm__("screenPosMenu_PositionX");
    static s16 screenPosMenu_PositionY __asm__("screenPosMenu_PositionY");

    (void)screenPosMenu_BackgroundFade;
    (void)screenPosMenu_PositionX;
    (void)screenPosMenu_PositionY;
}
INCLUDE_ASM("screens/options/nonmatchings/options", Options_ScreenPosMenu_Control);
#else
void Options_ScreenPosMenu_Control(void) // 0x801E53A0
{
    #define OPT_SCREEN_POS_X_RANGE 11
    #define OPT_SCREEN_POS_Y_RANGE 8
    #define BG_FADE_STEP           16

    s32        i;
    s8         posX;
    TILE*      tile;
    PACKET*    packet;
    static s32 screenPosMenu_BackgroundFade;
    static s16 screenPosMenu_PositionX;
    static s16 screenPosMenu_PositionY;

    posX = g_GameWorkConst->config.screenPositionX;
    if (posX != screenPosMenu_PositionX || g_GameWorkConst->config.screenPositionY != screenPosMenu_PositionY)
    {
        Screen_XyPositionSet(posX, g_GameWorkConst->config.screenPositionY);
    }

    screenPosMenu_PositionX = g_GameWorkConst->config.screenPositionX;
    screenPosMenu_PositionY = g_GameWorkConst->config.screenPositionY;

    switch (g_GameWork.gameStateSteps[1])
    {
        case ScreenPosMenuState_0:
            ScreenFade_Start(true, true, false);
            screenPosMenu_BackgroundFade  = 255;
            Game_StateStepSet(1, ScreenPosMenuState_1);

        case ScreenPosMenuState_1:
            Game_StateStepIncrement(1);
            break;

        case ScreenPosMenuState_2:
            // Set config.
            if (g_Controller0->buttonFlags.pulsed & ControllerFlag_LStickHighUp)
            {
                g_GameWorkConst->config.screenPositionY--;
            }
            if (g_Controller0->buttonFlags.pulsed & ControllerFlag_LStickHighDown)
            {
                g_GameWorkConst->config.screenPositionY++;
            }
            if (g_Controller0->buttonFlags.pulsed & ControllerFlag_LStickHighLeft)
            {
                g_GameWorkConst->config.screenPositionX--;
            }
            if (g_Controller0->buttonFlags.pulsed & ControllerFlag_LStickHighRight)
            {
                g_GameWorkConst->config.screenPositionX++;
            }
            g_GameWorkConst->config.screenPositionX = CLAMP(g_GameWorkConst->config.screenPositionX, -OPT_SCREEN_POS_X_RANGE, OPT_SCREEN_POS_X_RANGE);
            g_GameWorkConst->config.screenPositionY = CLAMP(g_GameWorkConst->config.screenPositionY, -OPT_SCREEN_POS_Y_RANGE, OPT_SCREEN_POS_Y_RANGE);

            // Play sound.
            if (g_GameWorkConst->config.screenPositionX != screenPosMenu_PositionX ||
                g_GameWorkConst->config.screenPositionY != screenPosMenu_PositionY)
            {
                Sd_SfxPlay(Sfx_MenuMove, Q8(0.0f), Q8(0.25f));
            }

            // Start background color fade.
            if (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.enter)
            {
                if (screenPosMenu_BackgroundFade == 255)
                {
                    screenPosMenu_BackgroundFade       = 0;
                    g_ScreenPosMenu_InvertBackgroundFade = (g_ScreenPosMenu_InvertBackgroundFade + 1) & (1 << 0);
                }
            }

            // Leave menu.
            if (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.cancel)
            {
                Sd_SfxPlay(Sfx_MenuCancel, Q8(0.0f), Q8(0.25f));

                ScreenFade_Start(true, false, false);
                Game_StateStepIncrement(1);
            }
            break;

        case ScreenPosMenuState_Leave:
            // Switch to previous menu.
            if (ScreenFade_IsFinished())
            {
                ScreenFade_Start(true, true, false);

                Game_StateStepSet(0, OptionsMenuState_LeaveScreenPos);

                g_GameWork.background2dColor.r = 0;
                g_GameWork.background2dColor.g = 0;
                g_GameWork.background2dColor.b = 0;
                return;
            }
            break;
    }

    // Update background fade.
    screenPosMenu_BackgroundFade += BG_FADE_STEP;
    screenPosMenu_BackgroundFade  = CLAMP(screenPosMenu_BackgroundFade, 0, 255);
    switch (g_ScreenPosMenu_InvertBackgroundFade)
    {
        case false:
            g_GameWork.background2dColor.r = ~screenPosMenu_BackgroundFade;
            g_GameWork.background2dColor.g = ~screenPosMenu_BackgroundFade;
            g_GameWork.background2dColor.b = ~screenPosMenu_BackgroundFade;
            break;

        case true:
            g_GameWork.background2dColor.r = screenPosMenu_BackgroundFade;
            g_GameWork.background2dColor.g = screenPosMenu_BackgroundFade;
            g_GameWork.background2dColor.b = screenPosMenu_BackgroundFade;
            break;
    }

    packet = GsOUT_PACKET_P;

    for (i = 0; i < 6; i++)
    {
        tile = (TILE*)packet;

        setTile(tile);

        if (g_ScreenPosMenu_InvertBackgroundFade == 0)
        {
            setRGB0(tile, 0xFF, 0, 0);
        }
        else if (g_ScreenPosMenu_InvertBackgroundFade == 1)
        {
            setRGB0(tile, 0xFF, 0, 0);
        }

        if (i < 3)
        {
            setXY0(tile, -160, -97 + (96 * i));
            setWH(tile, SCREEN_WIDTH, 2);
        }
        else
        {
            setXY0(tile, -577 + (144 * i), -120);
            setWH(tile, 2, SCREEN_HEIGHT);
        }

        addPrim(g_OrderingTable0[g_ActiveBufferIdx].org, (TILE*)packet);
        packet += sizeof(TILE);
    }

    for (i = 0; i < 28; i++)
    {
        tile = (TILE*)packet;

        setTile(tile);

        switch (g_ScreenPosMenu_InvertBackgroundFade)
        {
            case 0:
                setRGB0(tile, screenPosMenu_BackgroundFade, screenPosMenu_BackgroundFade, screenPosMenu_BackgroundFade);
                break;

            case 1:
                setRGB0(tile, ~screenPosMenu_BackgroundFade, ~screenPosMenu_BackgroundFade, ~screenPosMenu_BackgroundFade);
                break;
        }

        if (i < 11)
        {
            setXY0(tile, -160, -81 + (16 * i));
            setWH(tile, SCREEN_WIDTH, 2);
        }
        else
        {
            setXY0(tile, -305 + (16 * i), -120);
            setWH(tile, 2, SCREEN_HEIGHT);
        }

        addPrim(g_OrderingTable0[g_ActiveBufferIdx].org, (TILE*)packet);
        packet += sizeof(TILE);
    }

    GsOUT_PACKET_P = packet;

    Options_ScreenPosMenu_ArrowsDraw();
    Options_ScreenPosMenu_ConfigDraw();

    #undef OPT_SCREEN_POS_X_RANGE
    #undef OPT_SCREEN_POS_Y_RANGE
    #undef BG_FADE_STEP
}
#endif

void Options_ScreenPosMenu_ArrowsDraw(void) // 0x801E5A08
{
    #define DIR_COUNT 4

    const s_Triangle2d FRONT_ARROWS[] = {
        { {  0,  -100 }, { -8,   -92 }, {  8,  -92 } },
        { {  0,   100 }, { -8,    92 }, {  8,   92 } },
        { { -148, 0   }, { -140, -8  }, { -140, 8  } },
        { {  148, 0   }, {  140, -8  }, {  140, 8  } }
    };

    const s_Triangle2d BACK_ARROWS[] = {
        { {  0,  -101 }, { -10,  -91 }, {  9,  -91 } },
        { {  0,   101 }, { -10,   91 }, {  9,   91 } },
        { { -149, 0   }, { -139, -10 }, { -139, 10 } },
        { {  149, 0   }, {  139, -10 }, {  139, 10 } }
    };

    u8  dirs[DIR_COUNT]; // Booleans.
    s32 i;

    // Clear directions array for no reason.
    memset(dirs, 0, DIR_COUNT);

    // Draw flashing up/down/left/right arrows.
    for (i = 0; i < DIR_COUNT; i++)
    {
        Options_Selection_ArrowDraw(&FRONT_ARROWS[i], true, false);
    }

    if ((g_Controller0->buttonFlags.clicked & ControllerFlag_LStickHighUp) ||
        (g_Controller0->buttonFlags.held     & ControllerFlag_LStickHighUp))
    {
        dirs[0] = true;
    }
    if ((g_Controller0->buttonFlags.clicked & ControllerFlag_LStickHighDown) ||
        (g_Controller0->buttonFlags.held     & ControllerFlag_LStickHighDown))
    {
        dirs[1] = true;
    }
    if ((g_Controller0->buttonFlags.clicked & ControllerFlag_LStickHighLeft) ||
        (g_Controller0->buttonFlags.held     & ControllerFlag_LStickHighLeft))
    {
        dirs[2] = true;
    }
    if ((g_Controller0->buttonFlags.clicked & ControllerFlag_LStickHighRight) ||
        (g_Controller0->buttonFlags.held     & ControllerFlag_LStickHighRight))
    {
        dirs[3] = true;
    }

    // Draw border to highlight flashing up/down/left/right arrow corresponding to direction of UI navigation.
    for (i = 0; i < DIR_COUNT; i++)
    {
        if (dirs[i])
        {
            Options_Selection_ArrowDraw(&BACK_ARROWS[i], false, false);
        }
    }

    #undef DIR_COUNT
}

void Options_ScreenPosMenu_ConfigDraw(void) // 0x801E5CBC
{
    s32      i;
    LINE_F2* line;
    POLY_F4* poly;
    GsOT*    ot = &g_OrderingTable2[g_ActiveBufferIdx];

    const DVECTOR LINE_BASES[] = {
        { -60, 40 },
        { -60, 70 },
        { 60, 70 },
        { 60, 40 }
    };

    const char* AXIS_OFFSET_STRS[] = {
        "X:_",
        "Y:_"
    };

    for (i = 0; i < 4; i++)
    {
        line = (LINE_F2*)GsOUT_PACKET_P;

        setLineF2(line);
        setCodeWord(line, 0x40, (i < 2) ? COLOR_RGBC(240, 240, 240, 0) : COLOR_RGBC(128, 128, 128, 0));

        setXY0Fast(line, (u16)(LINE_BASES[i].vx             - g_GameWorkConst->config.screenPositionX), LINE_BASES[i].vy             - g_GameWorkConst->config.screenPositionY);
        setXY1Fast(line, (u16)(LINE_BASES[(i + 1) & 0x3].vx - g_GameWorkConst->config.screenPositionX), LINE_BASES[(i + 1) & 0x3].vy - g_GameWorkConst->config.screenPositionY);

        addPrim((u8*)ot->org + LAYER_40, line);
        GsOUT_PACKET_P = (u8*)line + sizeof(LINE_F2);
    }

    poly = (POLY_F4*)GsOUT_PACKET_P;
    setPolyF4(poly);

    setCodeWord(poly, 40, 0);
    setXY0Fast(poly, (u16)(LINE_BASES[0].vx - g_GameWorkConst->config.screenPositionX), LINE_BASES[0].vy - g_GameWorkConst->config.screenPositionY);
    setXY1Fast(poly, (u16)(LINE_BASES[1].vx - g_GameWorkConst->config.screenPositionX), LINE_BASES[1].vy - g_GameWorkConst->config.screenPositionY);
    setXY2Fast(poly, (u16)(LINE_BASES[3].vx - g_GameWorkConst->config.screenPositionX), LINE_BASES[3].vy - g_GameWorkConst->config.screenPositionY);
    setXY3Fast(poly, (u16)(LINE_BASES[2].vx - g_GameWorkConst->config.screenPositionX), LINE_BASES[2].vy - g_GameWorkConst->config.screenPositionY);

    addPrim((u8*)ot->org + LAYER_40, poly);
    GsOUT_PACKET_P = (u8*)poly + sizeof(POLY_F4);

    Gfx_StringPositionSet(108 - g_GameWorkConst->config.screenPositionX, 162 - g_GameWorkConst->config.screenPositionY);
    Gfx_StringDraw(AXIS_OFFSET_STRS[0], 10);
    Gfx_StringDrawInt(3, g_GameWorkConst->config.screenPositionX);

    Gfx_StringPositionSet(168 - g_GameWorkConst->config.screenPositionX, 162 - g_GameWorkConst->config.screenPositionY);
    Gfx_StringDraw(AXIS_OFFSET_STRS[1], 10);
    Gfx_StringDrawInt(3, g_GameWorkConst->config.screenPositionY);
}

// ========================================
// BRIGHTNESS OPTION SCREEN
// ========================================

#if VERSION_IS(EUR)
INCLUDE_ASM("screens/options/nonmatchings/options", Options_BrightnessMenu_Control);
#else
void Options_BrightnessMenu_Control(void) // 0x801E6018
{
    // Handle menu state.
    switch (g_GameWork.gameStateSteps[1])
    {
        case BrightnessMenuState_Enter:
            Game_StateStepSet(1, BrightnessMenuState_StartFade);
            break;

        case BrightnessMenuState_StartFade:
            ScreenFade_Start(true, true, false);
            Game_StateStepSet(1, BrightnessMenuState_SetConfig);
            break;

        case BrightnessMenuState_SetConfig:
            if (g_Controller0->buttonFlags.pulsed & ControllerFlag_LStickHighLeft)
            {
                if (g_GameWork.config.brightness != 0)
                {
                    g_GameWork.config.brightness--;
                    Sd_SfxPlay(Sfx_MenuMove, Q8(0.0f), Q8(0.25f));
                }
            }
            if (g_Controller0->buttonFlags.pulsed & ControllerFlag_LStickHighRight)
            {
                if (g_GameWork.config.brightness < 7)
                {
                    g_GameWork.config.brightness++;
                    Sd_SfxPlay(Sfx_MenuMove, Q8(0.0f), Q8(0.25f));
                }
            }

            // Fade screen and leave menu.
            if (g_Controller0->buttonFlags.clicked & (g_GameWorkPtr->config.controllerConfig.enter |
                                                      g_GameWorkPtr->config.controllerConfig.cancel))
            {
                if (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.enter)
                {
                    Sd_SfxPlay(Sfx_MenuConfirm, Q8(0.0f), Q8(0.25f));
                }
                else
                {
                    Sd_SfxPlay(Sfx_MenuCancel, Q8(0.0f), Q8(0.25f));
                }

                ScreenFade_Start(true, false, false);
                Game_StateStepIncrement(1);
            }
            break;

        case BrightnessMenuState_Leave:
            // Switch to previous menu.
            // TODO: Odd check for `ScreenFade_IsFinished()`.
            if ( (g_ScreenFade_Status & (1 << 2)) &&
                !(g_ScreenFade_Status & (1 << 1)) &&
                 (g_ScreenFade_Status & (1 << 0)))
            {
                ScreenFade_Start(true, true, false);
                Game_StateStepSet(0, OptionsMenuState_LeaveBrightness);

                g_GameWork.background2dColor.r = 0;
                g_GameWork.background2dColor.g = 0;
                g_GameWork.background2dColor.b = 0;
            }
            break;
    }

    // Draw graphics.
    if (g_GameWork.gameStatePrev == GameState_MainMenu)
    {
        Screen_BackgroundImgDraw(&g_BrightnessScreenImg0);
    }
    else
    {
        Screen_BackgroundImgDraw(&g_BrightnessScreenImg1);
    }

    Options_BrightnessMenu_LinesDraw(g_GameWork.config.brightness);
    Options_BrightnessMenu_ArrowsDraw();
    Options_BrightnessMenu_ConfigDraw();
}
#endif

#if VERSION_IS(EUR)
INCLUDE_ASM("screens/options/nonmatchings/options", Options_BrightnessMenu_ConfigDraw);
#else
void Options_BrightnessMenu_ConfigDraw(void) // 0x801E6238
{
    const char* LEVEL_STR = "LEVEL_________";

    Gfx_StringColorSet(StringColorId_White);
    Gfx_StringPositionSet(SCREEN_WIDTH / 4, 190);
    Gfx_StringDraw(LEVEL_STR, 20);
    Gfx_StringDrawInt(1, g_GameWork.config.brightness);
}
#endif

#if VERSION_IS(EUR)
INCLUDE_RODATA("screens/options/nonmatchings/options", D_801E2D4C);
INCLUDE_RODATA("screens/options/nonmatchings/options", D_801E2D64);
INCLUDE_ASM("screens/options/nonmatchings/options", Options_BrightnessMenu_ArrowsDraw);
#else
void Options_BrightnessMenu_ArrowsDraw(void) // 0x801E628C
{
    const s_Triangle2d FRONT_ARROWS[] = {
        { { 8, 84  }, { 16, 76 }, { 16, 92 } },
        { { 64, 84 }, { 56, 76 }, { 56, 92 } }
    };

    const s_Triangle2d BORDER_ARROWS[] = {
        { { 7, 84  }, { 17, 74 }, { 17, 94 } },
        { { 65, 84 }, { 55, 74 }, { 55, 94 } }
    };

    s32 heldButFlags;
    s32 i;
    s32 dir;

    // Determine UI movement direction.
    heldButFlags = g_Controller0->buttonFlags.held;
    if (heldButFlags & ControllerFlag_LStickHighLeft)
    {
        dir = 1;
    }
    else if (heldButFlags & ControllerFlag_LStickHighRight)
    {
        dir = 2;
    }
    else
    {
        dir = 0;
    }

    // Draw flashing left/right arrows.
    for (i = 0; i < 2; i++)
    {
        Options_Selection_ArrowDraw(&FRONT_ARROWS[i], true, false);
    }

    // Draw border to highlight flashing left/right arrow corresponding to direction of UI navigation.
    for (i = dir - 1; i < dir; i++)
    {
        Options_Selection_ArrowDraw(&BORDER_ARROWS[i], false, false);
    }
}
#endif

// ========================================
// DRAW OPTIONS FEATURES SCREEN
// ========================================

void Options_Selection_HighlightDraw(const s_Line2d* line, bool hasShadow, bool invertGradient) // 0x801E641C
{
    #define STR_OFFSET_Y 16

    s_Line2d* localLine;
    LINE_G2*  linePrim;
    POLY_G4*  poly;
    GsOT*     ot;

    ot        = &g_OrderingTable2[g_ActiveBufferIdx];
    linePrim  = (LINE_G2*)GsOUT_PACKET_P;
    localLine = line;

    // Draw underline.
    setLineG2(linePrim);

    // @unused Non-functioning line color gradient inversion? Gradient is unchanged regardless of `invertGradient`'s value,
    // which itself is always passed as `false`. Purpose is guessed based on a similar parameter in `Options_Selection_BulletPointDraw`.
    if (invertGradient)
    {
        setRGBC0(linePrim, 176, 176, 176, PRIM_LINE | RECT_SIZE_8);
        setRGBC1(linePrim, 160, 128, 64,  PRIM_LINE | RECT_SIZE_8);
    }
    else
    {
        setRGBC0(linePrim, 176, 176, 176, PRIM_LINE | RECT_SIZE_8);
        setRGBC1(linePrim, 160, 128, 64,  PRIM_LINE | RECT_SIZE_8);
    }

    setXY0Fast(linePrim, localLine->vertex0.vx, localLine->vertex0.vy);
    setXY1Fast(linePrim, localLine->vertex1.vx, localLine->vertex1.vy);
    addPrim((u8*)ot->org + (hasShadow ? LAYER_36 : LAYER_24), linePrim);
    GsOUT_PACKET_P = (u8*)linePrim + sizeof(LINE_G2);

    // Draw shadow gradient.
    if (hasShadow)
    {
        poly = (POLY_G4*)GsOUT_PACKET_P;
        setPolyG4(poly);
        setSemiTrans(poly, true);
        setRGB0(poly, 0, 0, 0);
        setRGB1(poly, Q8_COLOR(0.375f), Q8_COLOR(0.375f), Q8_COLOR(0.375f));
        setRGB2(poly, 0, 0, 0);
        setRGB3(poly, Q8_COLOR(0.375f), Q8_COLOR(0.375f), Q8_COLOR(0.375f));
        setXY4(poly,
               localLine->vertex0.vx, localLine->vertex0.vy - STR_OFFSET_Y, localLine->vertex0.vx, localLine->vertex0.vy,
               localLine->vertex1.vx, localLine->vertex1.vy - STR_OFFSET_Y, localLine->vertex1.vx, localLine->vertex1.vy);
        addPrim((u8*)ot->org + LAYER_36, poly);
        GsOUT_PACKET_P = (u8*)poly + sizeof(POLY_G4);

        Gfx_Primitive2dTextureSet(0, 0, 9, 2);
    }

    #undef STR_OFFSET_Y
}

void Options_Selection_ArrowDraw(const s_Triangle2d* arrow, bool isFlashing, bool resetColor) // 0x801E662C
{
    s32      colorFade;
    s32      colorStart;
    s32      colorEnd;
    POLY_G3* arrowPoly;
    GsOT*    ot;

    ot = &g_OrderingTable2[g_ActiveBufferIdx];

    // @unused `resetColor` doesn't serve any meaningful purpose.
    if (resetColor)
    {
        colorEnd   = 0;
        colorStart = 0;
    }

    colorFade = (u32)g_SysWork.gameStateCounter % 128;

    // Fade start color.
    if (colorFade >= 32)
    {
        colorStart = 32;
        if (colorFade < 64)
        {
            colorStart = 32;
        }
        else if (colorFade < 96)
        {
            colorStart = 96 - colorFade;
        }
        else
        {
            colorStart = 0;
        }
    }
    else
    {
        colorStart = colorFade;
    }

    // Fade end color.
    if (colorFade >= 32)
    {
        if (colorFade < 64)
        {
            colorEnd = colorFade - 32;
        }
        else if (colorFade >= 96)
        {
            colorEnd = 128 - colorFade;
        }
        else
        {
            colorEnd = 32;
        }
    }
    else
    {
        colorEnd = 0;
    }

    // Draw blue arrow.
    arrowPoly = (POLY_G3*)GsOUT_PACKET_P;
    setPolyG3(arrowPoly);

    // Flash color from blue to cyan.
    if (isFlashing)
    {
        // Base color is blue. `* 0x700` Shifts green component into place.
        setRGBC0Word(arrowPoly, (colorEnd   * 0x700) + COLOR_RGBC(0, 0, 255, PRIM_POLY | RECT_SIZE_8));
        setRGBC1Word(arrowPoly, (colorStart * 0x700) + COLOR_RGBC(0, 0, 255, PRIM_POLY | RECT_SIZE_8));
        setRGBC2Word(arrowPoly, (colorStart * 0x700) + COLOR_RGBC(0, 0, 255, PRIM_POLY | RECT_SIZE_8));
    }
    // Set solid cyan color.
    else
    {
        setRGBC0(arrowPoly, 0, 240, 240, PRIM_POLY | RECT_SIZE_8);
        setRGBC1(arrowPoly, 0, 240, 240, PRIM_POLY | RECT_SIZE_8);
        setRGBC2(arrowPoly, 0, 240, 240, PRIM_POLY | RECT_SIZE_8);
    }

    setXY0Fast(arrowPoly, arrow->vertex0.vx, arrow->vertex0.vy);
    setXY1Fast(arrowPoly, arrow->vertex1.vx, arrow->vertex1.vy);
    setXY2Fast(arrowPoly, arrow->vertex2.vx, arrow->vertex2.vy);
    addPrim((u8*)ot->org + LAYER_40, arrowPoly);
    GsOUT_PACKET_P = (u8*)arrowPoly + sizeof(POLY_G3);
}

void Options_Selection_BulletPointDraw(const s_Quad2d* quad, bool isBorder, bool isInactive) // 0x801E67B0
{
    #define TRI_COUNT 2

    GsOT*    ot = &g_OrderingTable2[g_ActiveBufferIdx];
    s32      i;
    POLY_G3* poly;

    // Draw quad as triangles to achieve diagonal gradient.
    for (i = 0; i < TRI_COUNT; i++)
    {
        poly = (POLY_G3*)GsOUT_PACKET_P;
        setPolyG3(poly);

        if (isBorder)
        {
            // Set color.
            switch (isInactive)
            {
                case false:
                    setRGBC0(poly, 255, 255, 255, PRIM_POLY | RECT_SIZE_8);
                    setRGBC1(poly, 160, 128, 64,  PRIM_POLY | RECT_SIZE_8);
                    setRGBC2(poly, 64,  64,  64,  PRIM_POLY | RECT_SIZE_8);
                    break;

                case true:
                    setRGBC0(poly, 128, 128, 128, PRIM_POLY | RECT_SIZE_8);
                    setRGBC1(poly, 40,  32,  16,  PRIM_POLY | RECT_SIZE_8);
                    setRGBC2(poly, 16,  16,  16,  PRIM_POLY | RECT_SIZE_8);
                    break;
            }

            // Draw triangle.
            if (i != 0)
            {
                setXY0Fast(poly, quad->vertex0.vx, quad->vertex0.vy);
                setXY1Fast(poly, quad->vertex1.vx, quad->vertex1.vy);
                setXY2Fast(poly, quad->vertex3.vx, quad->vertex3.vy);
            }
            else
            {
                setXY0Fast(poly, quad->vertex0.vx, quad->vertex0.vy);
                setXY1Fast(poly, quad->vertex2.vx, quad->vertex2.vy);
                setXY2Fast(poly, quad->vertex3.vx, quad->vertex3.vy);
            }
        }
        else
        {
            // Set color.
            switch (isInactive)
            {
                case false:
                    setRGBC0(poly, 160, 128, 64,  PRIM_POLY | RECT_SIZE_8);
                    setRGBC1(poly, 255, 255, 255, PRIM_POLY | RECT_SIZE_8);
                    setRGBC2(poly, 255, 255, 255, PRIM_POLY | RECT_SIZE_8);
                    break;

                case true:
                    setRGBC0(poly, 80,  64,  32,  PRIM_POLY | RECT_SIZE_8);
                    setRGBC1(poly, 160, 160, 160, PRIM_POLY | RECT_SIZE_8);
                    setRGBC2(poly, 160, 160, 160, PRIM_POLY | RECT_SIZE_8);
                    break;
            }

            // Draw triangle.
            if (i != 0)
            {
                setXY0Fast(poly, quad->vertex3.vx, quad->vertex3.vy);
                setXY1Fast(poly, quad->vertex1.vx, quad->vertex1.vy);
                setXY2Fast(poly, quad->vertex2.vx, quad->vertex2.vy);
            }
            else
            {
                setXY0Fast(poly, quad->vertex0.vx, quad->vertex0.vy);
                setXY1Fast(poly, quad->vertex1.vx, quad->vertex1.vy);
                setXY2Fast(poly, quad->vertex2.vx, quad->vertex2.vy);
            }
        }

        addPrim((u8*)ot->org + LAYER_24, poly);
        GsOUT_PACKET_P = (u8*)poly + sizeof(POLY_G3);
    }

    #undef TRI_COUNT
}

// ========================================
// JAPANESE SCREEN DRAW FUNCTIONS
// ========================================

#if VERSION_REGION_IS(NTSCJ)
void func_801E6F1C(RECT* rect)
{
    DVECTOR outline[6] =
    {
        { rect->x,               rect->y                },
        { rect->x + rect->w - 8, rect->y                },
        { rect->x + rect->w,     rect->y + 8            },
        { rect->x + rect->w,     rect->y + rect->h      },
        { rect->x + 8,           rect->y + rect->h      },
        { rect->x,               rect->y + rect->h - 8  }
    };
    s_Triangle2d corners[2] =
    {
        {
            { rect->x + rect->w,     rect->y     },
            { rect->x + rect->w - 8, rect->y     },
            { rect->x + rect->w,     rect->y + 8 }
        },
        {
            { rect->x,     rect->y + rect->h     },
            { rect->x + 8, rect->y + rect->h     },
            { rect->x,     rect->y + rect->h - 8 }
        }
    };
    GsOT*    ot;
    POLY_F3* tri;
    POLY_F4* quad;
    LINE_G2* line;
    s32      i;

    ot = &g_OrderingTable2[g_ActiveBufferIdx];

    // Draw corner triangles.
    for (i = 0; i < 2; i++)
    {
        tri = (POLY_F3*)GsOUT_PACKET_P;
        setPolyF3(tri);
        setSemiTrans(tri, true);
        setRGB0(tri, 0x30, 0x30, 0x30);
        setXY3(tri, corners[i].vertex0.vx, corners[i].vertex0.vy,
                    corners[i].vertex1.vx, corners[i].vertex1.vy,
                    corners[i].vertex2.vx, corners[i].vertex2.vy);
        addPrim((u8*)ot->org + LAYER_44, tri);
        GsOUT_PACKET_P = (u8*)tri + sizeof(POLY_F3);
    }

    Gfx_Primitive2dTextureSet(0, 0, 11, 2);

    // Draw background.
    quad = (POLY_F4*)GsOUT_PACKET_P;
    setPolyF4(quad);
    setSemiTrans(quad, true);
    setRGB0(quad, 0x30, 0x30, 0x30);
    setXY4(quad, rect->x,           rect->y,
                 rect->x,           rect->y + rect->h,
                 rect->x + rect->w, rect->y,
                 rect->x + rect->w, rect->y + rect->h);
    addPrim((u8*)ot->org + LAYER_48, quad);
    GsOUT_PACKET_P = (u8*)quad + sizeof(POLY_F4);

    Gfx_Primitive2dTextureSet(0, 0, 12, 1);

    // Draw outline.
    for (i = 0; i < 6; i++)
    {
        line = (LINE_G2*)GsOUT_PACKET_P;
        setLineG2(line);

        if (i & 1)
        {
            setRGB0(line, 160, 128, 64);
            setRGB1(line, 176, 176, 176);
        }
        else
        {
            setRGB0(line, 176, 176, 176);
            setRGB1(line, 160, 128, 64);
        }

        setXY2(line, outline[i].vx, outline[i].vy, outline[(i + 1) % 6].vx, outline[(i + 1) % 6].vy);
        addPrim((u8*)ot->org + LAYER_32, line);
        GsOUT_PACKET_P = (u8*)line + sizeof(LINE_G2);
    }
}

void func_801E749C(void)
{
    s16   entryPosX[8] = { -78, -60, -78, -66, -78, -60, -78, -48 };
    GsOT* ot;
    SPRT* prim;
    RECT  rect;

    prim = (SPRT*)GsOUT_PACKET_P;
    ot   = &g_OrderingTable2[g_ActiveBufferIdx];

    setSprt(prim);
    setRGBC0(prim, 128, 128, 128, 0x64);
    setWHFast(prim, g_ExtraOptionsMenu_SelectedEntry == 7 ? 72 : 120, 16);
    setXY0Fast(prim, entryPosX[g_ExtraOptionsMenu_SelectedEntry], 78);
    setUV0AndClut(prim, 0x00, (g_ExtraOptionsMenu_SelectedEntry / 5) != 0 ? 0xE0 : 0x10, 0x130, 0x1FE);

    addPrim((u8*)ot->org + LAYER_32, prim);
    GsOUT_PACKET_P = (u8*)prim + sizeof(SPRT);

    Gfx_Primitive2dTextureSet((g_ExtraOptionsMenu_SelectedEntry % 5) << 6, ((g_ExtraOptionsMenu_SelectedEntry / 5) != 0) << 8, 8, 0);

    prim = (SPRT*)GsOUT_PACKET_P;
    setSprt(prim);
    setRGBC0(prim, 128, 128, 128, 0x64);

    switch (g_ExtraOptionsMenu_SelectedEntry)
    {
        case 0:
            setWHFast(prim, 48, 16);
            setXY0Fast(prim, 30, 78);
            setUV0AndClut(prim, g_GameWork.config.extraWeaponCtrl != 0 ? 0x30 : 0, 0xE0, 0x130, 0x1FE);
            break;

        case 1:
            setWHFast(prim, g_ExtraOptionsMenu_SelectedBloodColorEntry == 0 ? 48 : 12, 16);
            setXY0Fast(prim, 24, 78);
            setUV0AndClutSum(prim, g_ExtraOptionsMenu_SelectedBloodColorEntry == 0 ? 0x30 : 0x60 + ((g_ExtraOptionsMenu_SelectedBloodColorEntry - 1) * 12), 0xE0, 0x7F93);
            break;

        case 2:
            setWHFast(prim, 48, 16);
            setXY0Fast(prim, 30, 78);
            setUV0AndClut(prim, g_GameWork.config.extraViewCtrl != 0 ? 0 : 0x30, 0xE0, 0x130, 0x1FE);
            break;

        case 3:
            setWHFast(prim, 48, 16);
            setXY0Fast(prim, 18, 78);
            setUV0AndClut(prim, g_GameWork.config.extraRetreatTurn != 0 ? 0 : 0x30, 0xE0, 0x130, 0x1FE);
            break;

        case 4:
            setWHFast(prim, 48, 16);
            setXY0Fast(prim, 30, 78);
            setUV0AndClut(prim, g_GameWork.config.extraWalkRunCtrl != 0 ? 0 : 0x30, 0xE0, 0x130, 0x1FE);
            break;

        case 5:
            setWHFast(prim, 24, 16);
            setXY0Fast(prim, 36, 78);
            setUV0AndClut(prim, g_GameWork.config.extraAutoAiming != 0 ? 0xC0 : 0xD8, 0xE0, 0x130, 0x1FE);
            break;

        case 6:
            setWHFast(prim, 48, 16);
            setXY0Fast(prim, 30, 78);
            setUV0AndClut(prim, g_GameWork.config.extraViewMode != 0 ? 0xC0 : 0x90, 0xE0, 0x130, 0x1FE);
            break;

        case 7:
            setWHFast(prim, 24, 16);
            setXY0Fast(prim, 24, 78);
            setUV0AndClutSum(prim, 0x60 + (g_GameWork.config.extraBulletAdjust * 24), 0xE0, 0x7F93);
            break;
    }

    addPrim((u8*)ot->org + LAYER_32, prim);
    GsOUT_PACKET_P = (u8*)prim + sizeof(SPRT);

    switch (g_ExtraOptionsMenu_SelectedEntry)
    {
        case 0:
        case 5:
            Gfx_Primitive2dTextureSet(192, 256, 8, 0);
            break;

        case 1:
        case 2:
        case 3:
        case 4:
        case 6:
            Gfx_Primitive2dTextureSet(256, 256, 8, 0);
            break;

        case 7:
            Gfx_Primitive2dTextureSet(128, 256, 8, 0);
            break;
    }

    setRECT(&rect, -136, 72, 272, 28);
    func_801E6F1C(&rect);

    setRECT(&rect, -136, -80, 272, 148);
    func_801E6F1C(&rect);
}

void func_801E794C(void)
{
    s16   entryPosX[9] = { -84, -72, -60, -78, -120, -120, -96, -90, -84 };
    GsOT* ot;
    SPRT* prim;
    RECT  rect;

    prim = (SPRT*)GsOUT_PACKET_P;
    ot   = &g_OrderingTable2[g_ActiveBufferIdx];

    setSprt(prim);
    setRGBC0(prim, 128, 128, 128, 0x64);
    setWHFast(prim, 240, 16);
    setXY0Fast(prim, entryPosX[g_MainOptionsMenu_SelectedEntry], 78);
    setUV0AndClut(prim, 0x00, (g_MainOptionsMenu_SelectedEntry / 5) != 0 ? 0xE0 : 0x10, 0x130, 0x1FE);

    addPrim((u8*)ot->org + LAYER_32, prim);
    GsOUT_PACKET_P = (u8*)prim + sizeof(SPRT);

    Gfx_Primitive2dTextureSet((g_MainOptionsMenu_SelectedEntry % 5) << 6, ((g_MainOptionsMenu_SelectedEntry / 5) != 0) << 8, 8, 0);

    if (g_MainOptionsMenu_SelectedEntry < 7)
    {
        if (g_MainOptionsMenu_SelectedEntry >= 4)
        {
            prim = (SPRT*)GsOUT_PACKET_P;
            setSprt(prim);
            setRGBC0(prim, 128, 128, 128, 0x64);

            switch (g_MainOptionsMenu_SelectedEntry)
            {
                case 6:
                    setWHFast(prim, 48, 16);
                    setXY0Fast(prim, 24, 78);
                    setUV0AndClut(prim, g_GameWork.config.soundType != 0 ? 0x60 : 0x30, 0xE0, 0x130, 0x1FE);
                    break;

                case 5:
                    setWHFast(prim, 24, 16);
                    setXY0Fast(prim, 48, 78);
                    setUV0AndClut(prim, g_GameWork.config.autoLoad == 0 ? 0x18 : 0, 0xE0, 0x130, 0x1FE);
                    break;

                case 4:
                    setWHFast(prim, 24, 16);
                    setXY0Fast(prim, 84, 78);
                    setUV0AndClut(prim, g_GameWork.config.vibrationEnabled == 0 ? 0x18 : 0, 0xE0, 0x130, 0x1FE);
                    break;
            }

            addPrim((u8*)ot->org + LAYER_32, prim);
            GsOUT_PACKET_P = (u8*)prim + sizeof(SPRT);

            Gfx_Primitive2dTextureSet(256, 256, 8, 0);
        }
    }

    setRECT(&rect, -136, 72, 272, 28);
    func_801E6F1C(&rect);

    setRECT(&rect, -136, -80, 272, 148);
    func_801E6F1C(&rect);
}
#endif

// ========================================
// CONTROLS OPTION SCREEN
// ========================================

void Options_ControllerMenu_Control(void) // 0x801E69BC
{
    s32                                     boundActionIdx = NO_VALUE;
    e_InputAction                           actionIdx;
    static s_ControllerMenu_SelectedEntries selectedEntries;

    // Handle controller config menu state.
    switch (g_GameWork.gameStateSteps[1])
    {
        case ControllerMenuState_Exit:
            ScreenFade_Start(false, true, false);
            selectedEntries.preset = ControllerMenuState_Exit;

            // Leave menu.
            if (g_Controller0->buttonFlags.clicked & (g_GameWorkPtr->config.controllerConfig.enter |
                                                      g_GameWorkPtr->config.controllerConfig.cancel))
            {
                SD_Call(Sfx_MenuCancel);

                ScreenFade_Start(false, false, false);
                Game_StateStepSet(1, ControllerMenuState_Leave);
                break;
            }

            // Move selection cursor up/down.
            if (g_Controller0->buttonFlags.pulsedGui & ControllerFlag_LStickHighUp)
            {
                Game_StateStepSet(1, ControllerMenuState_Type3);
            }
            else if (g_Controller0->buttonFlags.pulsedGui & ControllerFlag_LStickHighDown)
            {
                Game_StateStepSet(1, ControllerMenuState_Type1);
            }
            // Move selection cursor left/right.
            else if (g_Controller0->buttonFlags.pulsedGui & (ControllerFlag_LStickHighLeft |
                                                             ControllerFlag_LStickHighRight))
            {
                Game_StateStepSet(1, ControllerMenuState_Actions);
            }
            break;

        case ControllerMenuState_Type1:
        case ControllerMenuState_Type2:
        case ControllerMenuState_Type3:
            selectedEntries.preset = g_GameWork.gameStateSteps[1];

            // Set binding preset.
            if (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.enter)
            {
                SD_Call(Sfx_MenuConfirm);
                Settings_RestoreControlDefaults(g_GameWork.gameStateSteps[1] - 1);
            }
            // Reset selection cursor.
            else if (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.cancel)
            {
                SD_Call(Sfx_MenuCancel);
                Game_StateStepSet(1, ControllerMenuState_Exit);
            }
            // Move selection cursor.
            else
            {
                // Move selection cursor up/down.
                if (g_Controller0->buttonFlags.pulsedGui & ControllerFlag_LStickHighUp)
                {
                    Game_StateStepSet(1, (g_GameWork.gameStateSteps[1] - 1) & 3); // % 4
                }
                else if (g_Controller0->buttonFlags.pulsedGui & ControllerFlag_LStickHighDown)
                {
                    Game_StateStepSet(1, (g_GameWork.gameStateSteps[1] + 1) & 3); // % 4
                }
                // Move selection cursor left/right.
                else if (g_Controller0->buttonFlags.pulsedGui & (ControllerFlag_LStickHighLeft |
                                                                 ControllerFlag_LStickHighRight))
                {
                    Game_StateStepSet(1, ControllerMenuState_Actions);
                }
            }
            break;

        case ControllerMenuState_Actions:
            actionIdx = selectedEntries.action;

            // Move selection cursor up/down.
            if (g_Controller0->buttonFlags.pulsedGui & ControllerFlag_LStickHighUp)
            {
                if (actionIdx != InputAction_Enter)
                {
                    selectedEntries.action = actionIdx - 1;
                }
                else
                {
                    selectedEntries.action = InputAction_Option;
                }
            }
            else if (g_Controller0->buttonFlags.pulsedGui & ControllerFlag_LStickHighDown)
            {
                if (actionIdx != InputAction_Option)
                {
                    selectedEntries.action = actionIdx + 1;
                }
                else
                {
                    selectedEntries.action = InputAction_Enter;
                }
            }
            // Move selection cursor left/right.
            else if (g_Controller0->buttonFlags.pulsedGui & (ControllerFlag_LStickHighLeft |
                                                             ControllerFlag_LStickHighRight))
            {
                Game_StateStepSet(1, selectedEntries.preset);
            }
            // Bind button to input action.
            else
            {
                boundActionIdx = Options_ControllerMenu_ConfigUpdate(actionIdx);
            }
            break;

        case ControllerMenuState_Leave:
            // Switch to previous menu.
            if (ScreenFade_IsFinished())
            {
                ScreenFade_Start(true, true, false);
                Game_StateStepSet(0, OptionsMenuState_LeaveController);
            }
            break;
    }

    if (g_GameWork.gameStateSteps[1] == ControllerMenuState_Actions)
    {
        g_ControllerMenu_IsOnActionsPane = true;
    }
    else
    {
        g_ControllerMenu_IsOnActionsPane = false;
    }

    // Play cursor navigation SFX.
    if (g_Controller0->buttonFlags.pulsedGui & (ControllerFlag_LStickHighUp    |
                                                ControllerFlag_LStickHighRight |
                                                ControllerFlag_LStickHighDown  |
                                                ControllerFlag_LStickHighLeft))
    {
        SD_Call(Sfx_MenuMove);
    }

    // Draw menu graphics.
    Options_ControllerMenu_EntriesDraw(g_ControllerMenu_IsOnActionsPane, selectedEntries.preset, selectedEntries.action, boundActionIdx);
}

s32 Options_ControllerMenu_ConfigUpdate(s32 actionIdx) // 0x801E6CF4
{
    u16* bindings;
    u16  boundButFlag;
    u16  butFlag;
    s32  curActionIdx;
    s32  boundActionIdx;
    s32  i;
    u32  j;

    boundActionIdx = NO_VALUE;
    bindings       = (u16*)&g_GameWorkPtr->config.controllerConfig;

    // Run through all controller flags, excluding stick axes.
    for (i = 0; i < 16; i++)
    {
        butFlag = 1 << i;
        if ((butFlag & (ControllerFlag_DpadUp    |
                        ControllerFlag_DpadRight |
                        ControllerFlag_DpadDown  |
                        ControllerFlag_DpadLeft)) ||
            !(butFlag & g_Controller0->buttonFlags.clicked))
        {
            continue;
        }

        boundButFlag = bindings[actionIdx];

        // Remove binding.
        if (boundButFlag & butFlag)
        {
            if ((actionIdx <  InputAction_Skip   ||
                 actionIdx == InputAction_Action ||
                 actionIdx == InputAction_Aim    ||
                 actionIdx == InputAction_Item) &&
                !(bindings[actionIdx] & ~butFlag))
            {
                boundActionIdx = actionIdx;
                SD_Call(Sfx_MenuError);
            }
            else
            {
                bindings[actionIdx] &= ~butFlag;
                SD_Call(Sfx_MenuConfirm);
            }
        }
        else
        {
            curActionIdx = NO_VALUE;
            switch (actionIdx)
            {
                case 0:
                case 1:
                    curActionIdx = actionIdx == 0;
                    if (bindings[curActionIdx] & butFlag)
                    {
                        if (!(bindings[curActionIdx] & ~butFlag))
                        {
                            boundActionIdx = curActionIdx;
                            SD_Call(Sfx_MenuError);
                        }
                        else
                        {
                            bindings[curActionIdx] &= ~butFlag;
                            bindings[actionIdx]    |= butFlag;
                            SD_Call(Sfx_MenuConfirm);
                        }
                    }
                    else
                    {
                        bindings[actionIdx] = boundButFlag | butFlag;
                        SD_Call(Sfx_MenuConfirm);
                    }
                    break;

                case 2:
                    bindings[InputAction_Skip] |= butFlag;
                    SD_Call(Sfx_MenuConfirm);
                    break;

                default:
                    curActionIdx = NO_VALUE;
                    for (j = InputAction_Action; j < InputAction_Count; j++)
                    {
                        if (bindings[j] & butFlag)
                        {
                            curActionIdx = j;
                            break;
                        }
                    }

                    if (curActionIdx != NO_VALUE)
                    {
                        if ((curActionIdx <  InputAction_Skip   ||
                             curActionIdx == InputAction_Action ||
                             curActionIdx == InputAction_Aim    ||
                             curActionIdx == InputAction_Item) &&
                            !(bindings[curActionIdx] & ~butFlag))
                        {
                            SD_Call(Sfx_MenuError);
                            boundActionIdx = curActionIdx;
                        }
                        else
                        {
                            bindings[curActionIdx] &= ~butFlag;
                            bindings[actionIdx]    |= butFlag;
                            SD_Call(Sfx_MenuConfirm);
                        }
                    }
                    else
                    {
                        bindings[actionIdx] |= butFlag;
                        SD_Call(Sfx_MenuConfirm);
                    }
                    break;
            }
        }
    }

    return boundActionIdx;
}

void Options_ControllerMenu_EntriesDraw(bool isOnRightPane, s32 presetsEntryIdx, s32 actionsEntryIdx, s32 boundActionIdx) // 0x801E6F60
{
    #define STR_BASE_Y    22
    #define STR_OFFSET_Y  20
    #define ICON_SIZE_Y   12
    #define ICON_OFFSET_X -12

    s16      highlightY0;
    s16      highlightY1;
    s32      strYPos;
    s32      i;
    u16*     contConfig;
    DR_MODE* drMode;
    POLY_G4* poly;
    GsOT*    ot;

    /** @brief Draw modes for textured entry selection highlights in the controller config menu.
     * 0: Left presets pane.
     * 1: Right actions pane.
     */
    static DR_MODE SELECTION_HIGHLIGHT_DRAW_MODES[2] = {
        {
            .tag  = 0x03000000,
            .code = { 0xE1000200, 0 }
        },
        {
            .tag  = 0x03000000,
            .code = { 0xE1000200, 0 }
        }
    };

    /** @brief Quads for textured entry selection highlights in the controller config menu.
     * 0: Left presets pane.
     * 1: Right actions pane.
     */
    static POLY_G4 SELECTION_HIGHLIGHT_QUADS[2] = {
        {
            .tag  = 0x08000000,
            .r0   = 255,
            .g0   = 255,
            .b0   = 255,
            .code = 0x3A,
            .r3   = 255,
            .g3   = 255,
            .b3   = 255
        },
        {
            .tag  = 0x08000000,
            .code = 0x3A,
            .r1   = 255,
            .g1   = 255,
            .b1   = 255,
            .r2   = 255,
            .g2   = 255,
            .b2   = 255
        },
    };

    /** @brief Controller menu entry strings for the presets pane on the left. */
    static const char* PRESETS_PANE_ENTRY_STRS[] = {
        "EXIT",
        "TYPE_1",
        "TYPE_2",
        "TYPE_3"
    };

    /** @brief Controller menu entry strings for the actions pane on the right. */
    static const char* ACTIONS_PANE_ENTRY_STRS[] = {
        "ENTER",
        "CANCEL",
        "SKIP",
        "ACTION",
        "AIM",
        "LIGHT",
        "RUN",
        "VIEW",
        "STEP L",
        "STEP R",
        "PAUSE",
        "ITEM",
        "MAP",
        "OPTION"
    };


    ot     = &g_OtTags0[g_ActiveBufferIdx][15];
    poly   = &SELECTION_HIGHLIGHT_QUADS[g_ActiveBufferIdx];
    drMode = &SELECTION_HIGHLIGHT_DRAW_MODES[g_ActiveBufferIdx];

    // Draw entry strings.
    for (i = 0; i < ControllerMenuState_Count; i++)
    {
        Gfx_StringPositionSet(24, STR_BASE_Y + (i * STR_OFFSET_Y));
        Gfx_StringDraw(PRESETS_PANE_ENTRY_STRS[i], 20);
    }

    if (!isOnRightPane)
    {
        highlightY1 = presetsEntryIdx * STR_OFFSET_Y;
        highlightY0 = highlightY1 - 91;
        setXY4(poly,
               -137, highlightY0,
               -76,  highlightY0,
               -137, highlightY1 - 76,
               -76,  highlightY1 - 76);
    }

    strYPos     = STR_BASE_Y;
    highlightY0 = -300;

    // Draw controller config.
    for (i = 0, contConfig = (u16*)&g_GameWorkPtr->config.controllerConfig; i < (u32)InputAction_Count; i++, contConfig++)
    {
        // Draw action string.
        Text_Debug_PositionSet(96, strYPos);
        Text_Debug_Draw(ACTIONS_PANE_ENTRY_STRS[i]);

        // Draw button icon.
        if (i != boundActionIdx)
        {
            Options_ControllerMenu_ButtonIconsDraw(ICON_OFFSET_X, strYPos - 114, *contConfig);
        }

        if (i == actionsEntryIdx)
        {
            highlightY0 = strYPos - 113;
        }

        strYPos = (strYPos + ICON_SIZE_Y) + ((i == 2) ? ICON_SIZE_Y : 0);
    }

    if (isOnRightPane == true)
    {
        setXY4(poly,
               -65, highlightY0,
               -15, highlightY0,
               -65, highlightY0 + 10,
               -15, highlightY0 + 10);
    }

    AddPrim(ot, poly);
    AddPrim(ot, drMode);

    #undef STR_BASE_Y
    #undef STR_OFFSET_Y
    #undef ICON_SIZE_Y
    #undef ICON_OFFSET_X
}

void Options_ControllerMenu_ButtonIconsDraw(s32 baseX, s32 baseY, u16 config) // 0x801E716C
{
    #define ICON_SIZE_X   12
    #define ICON_SIZE_Y   12
    #define ICON_OFFSET_X 14

    s32            i;
    s32            posX;
    u16            clutX;
    u32            clutY;
    s32            temp;
    s32            v0;
    GsOT*          ot;
    SPRT*          prim;
    DR_TPAGE*      tpage;
    PACKET*        packet;
    s_FsImageDesc* image;

    image = &g_ControllerButtonAtlasImg;

    ot     = &g_OtTags1[g_ActiveBufferIdx][16];
    packet = GsOUT_PACKET_P;

    // Draw button sprites.
    for (posX = baseX, i = ICON_SIZE_X; i < 28; i++)
    {
        temp = i & 0xF;
        v0   = ((temp + 8) & 0xF) << 4;

        if (!((config >> temp) & (1 << 0)))
        {
            continue;
        }

        prim = (SPRT*)packet;
        addPrimFast(ot, prim, 4);
        setCodeWord(prim, PRIM_RECT | RECT_TEXTURE, COLOR_RGBC(128, 128, 128, 0));
        setWH(prim, ICON_SIZE_X, ICON_SIZE_Y);

        clutY = image->clutY;
        clutX = image->clutX;

        setXY0Fast(prim, posX, baseY);
        posX += ICON_OFFSET_X;

        // setUV0AndClut(prim, 0xF4, v0, clutY, clutX);
        *(u32*)(&prim->u0) = 244 + (v0 << 8) + (((clutY << 6) | ((clutX >> 4) & 0x3F)) << 16);

        packet = (u8*)prim + sizeof(SPRT);
        tpage  = (DR_TPAGE*)packet;

        setDrawTPage(tpage, 0, 1, getTPageN(0, 0, 7, 0));
        AddPrim(ot, tpage);
        packet = (u8*)tpage + sizeof(DR_TPAGE);
    }

    GsOUT_PACKET_P = packet;

    #undef ICON_SIZE_X
    #undef ICON_SIZE_Y
    #undef ICON_OFFSET_X
}


/** @brief Unknown .rodata value.
 * The type is assumed. It is unknown where this is used and
 * could be something defined by a macro.
 */
static const u16 D_801E2D42 = 4160;
