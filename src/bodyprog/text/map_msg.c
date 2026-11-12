#include "bodyprog/game.h"

#include <psyq/libetc.h>
#include <psyq/libpad.h>
#include <psyq/strings.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "bodyprog/screen/data.h"
#include "bodyprog/screen/draw.h"
#include "bodyprog/sound/sound_system.h"
#include "bodyprog/text/map_msg.h"
#include "bodyprog/text/text_draw.h"
#include "main/fsqueue.h"

// ========================================
// STATIC VARIABLES
// ========================================

static s32   g_MapMsg_CurrentIdx       = 0;
static q3_12 g_MapMsg_SelectFlashTimer = Q12(0.0f);

// ========================================
// GLOBAL VARIABLES
// ========================================

s_MapMsgSelect g_MapMsg_Select;
u8             g_MapMsg_AudioType;
s8             g_MapMsg_SelectCancelIdx;

// @hack JP calls different `Gfx_StringColorSet` / `Gfx_StringDraw` funcs here.
// The normal funcs available are also used in JP, so can't be renamed.
// For now override `Gfx_StringColorSet` calls in this file until those JP funcs get figured out.
#if VERSION_REGION_IS(NTSCJ)
    #define Gfx_StringColorSet Gfx_StringColorSet_JP
#endif

s32 Gfx_MapMsg_Draw(s32 mapMsgIdx) // 0x800365B8
{
    #define FINISH_MAP_MSG  0xFF
    
#if VERSION_REGION_IS(PAL)
    #define ROLLOUT_ADVANCE 0
#else
    #define ROLLOUT_ADVANCE 2
#endif

    s32         curRolloutState;
    bool        skip;
    s32         curMenuSelection;
#if VERSION_REGION_IS(NTSCJ)
    s32         unkJapVal;
#endif
    static s32  rolloutState;
    static s32  menuSelection;
    static s32  displayLength;
    static s32  activeMapMsgIdx;
    static s32  displayLengthInc;
    static bool loadAudio;

    // Check for user input to skip.
    skip = false;
    if ((g_Controller0->buttonFlags.clicked & (g_GameWorkPtr->config.controllerConfig.enter |
                                               g_GameWorkPtr->config.controllerConfig.cancel)) ||
        (g_Controller0->buttonFlags.held & g_GameWorkPtr->config.controllerConfig.skip))
    {
        skip = true;
    }

    PLAYER_CHARA().properties.player.gasWeaponPowerTimer = Q12(0.0f);
    func_8004C564(PLAYER_COMBAT().weaponAttack, WEAPON_ATTACK(AttackType_RockDrill, AttackInputType_Tap));

    if (activeMapMsgIdx != mapMsgIdx)
    {
        g_SysWork.isMsgStringSet = false;
    }

    switch (g_SysWork.isMsgStringSet)
    {
        case false:
            g_SysWork.mapMsgTimer            = NO_VALUE;
            g_MapMsg_Select.maxIdx           = NO_VALUE;
            g_MapMsg_Select.selectedEntryIdx = 0;
            g_MapMsg_AudioType               = MapMsgAudioType_None;
            g_MapMsg_CurrentIdx              = mapMsgIdx;
            rolloutState                     = 0;
            menuSelection                    = 0;
            activeMapMsgIdx                  = mapMsgIdx;
            displayLength                    = 0;
            displayLengthInc                 = ROLLOUT_ADVANCE;

            Gfx_MapMsg_Reset();

#if VERSION_REGION_IS(NTSCJ)
            unkJapVal = Gfx_MapMsg_WidthsCompute(g_MapMsg_CurrentIdx);
#else
            Gfx_MapMsg_WidthsCompute(g_MapMsg_CurrentIdx);
#endif

#if VERSION_REGION_IS(NTSCJ)
            if (unkJapVal != 0)
            {
                switch (unkJapVal)
                {
                    case 2:
                    case 3:
                        func_8004B45C(g_MapMsg_CurrentIdx + 1, unkJapVal);
                        break;

                    case 4:
                        func_8004B45C(0, 2);
                        break;
                }
            }
#endif

            loadAudio = true;
            g_SysWork.isMsgStringSet++;
            return MapMsgState_Finish;

        case true:
            if (g_SysWork.bgmStatusFlags & BgmStatusFlag_VoiceDialog)
            {
                if (Sd_AudioStreamingCheck() == AudioStreamingState_XaLoadPending)
                {
                    loadAudio = false;
                    break;
                }

                if (loadAudio)
                {
                    break;
                }
            }
            else
            {
                loadAudio = false;
            }

            Gfx_StringColorSet(StringColorId_White);

#if VERSION_REGION_IS(NTSC) || VERSION_REGION_IS(PAL)
            Gfx_StringPositionSet(SCREEN_WIDTH / 8, (SCREEN_HEIGHT / 3) * 2);
#endif

#if VERSION_REGION_IS(PAL)
            displayLengthInc += g_DeltaTimeRaw;
            if (displayLength != 400)
            {
                displayLength = Q12_MULT(displayLengthInc, 60);
            }
#else
            displayLength += displayLengthInc;
#endif

            displayLength  = CLAMP(displayLength, 0, MAP_MESSAGE_DISPLAY_ALL_LENGTH);

            if (g_MapMsg_AudioType != MapMsgAudioType_None && g_SysWork.mapMsgTimer > Q12(0.0f))
            {
                g_SysWork.mapMsgTimer -= g_DeltaTimeRaw;
                g_SysWork.mapMsgTimer  = CLAMP(g_SysWork.mapMsgTimer, Q12(0.0f), INT_MAX);
            }

            curRolloutState = rolloutState;
            if (curRolloutState == NO_VALUE)
            {
                if (g_MapMsg_AudioType == MapMsgAudioType_None)
                {
                    Game_TimerUpdate();
                }

                curMenuSelection = menuSelection;
                if (curMenuSelection == curRolloutState)
                {
                    if (g_MapMsg_Select.maxIdx == curMenuSelection)
                    {
                        if (!((g_MapMsg_AudioType & MAP_MSG_UNSKIPPABLE_AUDIO_TYPE_FLAG) || !skip) ||
                            (g_MapMsg_AudioType != MapMsgAudioType_None && g_SysWork.mapMsgTimer == Q12(0.0f)))
                        {
                            menuSelection = FINISH_MAP_MSG;

                            if (g_SysWork.bgmStatusFlags & BgmStatusFlag_VoiceDialog)
                            {
                                SD_Call(19);
                            }
                            break;
                        }
                    }
                    // Cancel.
                    else if (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.cancel)
                    {
                        g_MapMsg_Select.maxIdx           = curMenuSelection;
                        g_MapMsg_Select.selectedEntryIdx = g_MapMsg_SelectCancelIdx;

                        Sd_SfxPlay(Sfx_MenuCancel, Q7(0.0f), Q7(0.5f));

                        if (g_SysWork.silentYesSelection)
                        {
                            g_SysWork.silentYesSelection = false;
                        }

                        menuSelection = FINISH_MAP_MSG;
                        break;
                    }
                    // Enter.
                    else if (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.enter)
                    {
                        g_MapMsg_Select.maxIdx = curMenuSelection;

                        if (g_MapMsg_Select.selectedEntryIdx == (s8)g_MapMsg_SelectCancelIdx)
                        {
                            Sd_SfxPlay(Sfx_MenuCancel, Q7(0.0f), Q7(0.5f));
                        }
                        else if (!g_SysWork.silentYesSelection)
                        {
                            Sd_SfxPlay(Sfx_MenuConfirm, Q7(0.0f), Q7(0.5f));
                        }

                        if (g_SysWork.silentYesSelection)
                        {
                            g_SysWork.silentYesSelection = false;
                        }

                        menuSelection = FINISH_MAP_MSG;
                        break;
                    }
                }
                else if ((!(g_MapMsg_AudioType & MAP_MSG_UNSKIPPABLE_AUDIO_TYPE_FLAG) && skip && g_MapMsg_Select.maxIdx != 0) ||
                         (g_MapMsg_AudioType != MapMsgAudioType_None && g_SysWork.mapMsgTimer == Q12(0.0f)))
                {
                    if (g_MapMsg_Select.maxIdx != NO_VALUE)
                    {
                        g_MapMsg_Select.maxIdx = NO_VALUE;
                        menuSelection          = FINISH_MAP_MSG;
                        break;
                    }

#if VERSION_REGION_IS(PAL)
                    displayLengthInc = 0;
#endif
                    g_MapMsg_CurrentIdx++;
                    g_SysWork.mapMsgTimer = g_MapMsg_Select.maxIdx;

#if VERSION_REGION_IS(NTSCJ)
            unkJapVal = Gfx_MapMsg_WidthsCompute(g_MapMsg_CurrentIdx);
#else
            Gfx_MapMsg_WidthsCompute(g_MapMsg_CurrentIdx);
#endif

#if VERSION_REGION_IS(NTSCJ)
                    if (unkJapVal != 0)
                    {
                        switch (unkJapVal)
                        {
                            case 2:
                            case 3:
                                func_8004B45C(g_MapMsg_CurrentIdx + 1, unkJapVal);
                                break;

                            case 4:
                                func_8004B45C(0, 2);
                                break;
                        }
                    }
#endif

                    displayLength = 0;
                    rolloutState  = 0;

                    if (g_MapMsg_AudioType == MapMsgAudioType_VoiceStream)
                    {
                        loadAudio = false;
                        return MapMsgState_Idle;
                    }

                    if (g_SysWork.bgmStatusFlags & BgmStatusFlag_VoiceDialog)
                    {
                        SD_Call(19);
                    }

                    loadAudio = true;
                    return MapMsgState_Finish;
                }
            }
            else
            {
                if (skip)
                {
                    displayLength = MAP_MESSAGE_DISPLAY_ALL_LENGTH;
                }
            }

            rolloutState  = 0;
            menuSelection = Gfx_MapMsg_SelectionUpdate(g_MapMsg_CurrentIdx, &displayLength);

            if (menuSelection != MapMsgReturnCode_None && menuSelection < MapMsgReturnCode_YesOrNo)
            {
                rolloutState = NO_VALUE;
            }
    }

    if (menuSelection != FINISH_MAP_MSG)
    {
        return MapMsgState_Idle;
    }

    g_SysWork.isMsgStringSet         = false;
    g_SysWork.enableHalfHeightGlyphs = false;
    displayLength                    = 0;

    if (g_SysWork.bgmStatusFlags & BgmStatusFlag_VoiceDialog)
    {
        loadAudio = true;
    }

    return g_MapMsg_Select.selectedEntryIdx + 1;

    #undef FINISH_MAP_MSG
}

s32 Gfx_MapMsg_SelectionUpdate(u8 mapMsgIdx, s32* displayLength) // 0x80036B5C
{
    #define FLASH_TIMER_MAX    Q12(0.5f)
    #define STRING_LINE_OFFSET 16

    s32 i;
    s32 returnCode;

    returnCode = Gfx_MapMsg_StringDraw(g_MapOverlayHdr.mapMessages[mapMsgIdx], *displayLength);

    g_MapMsg_SelectFlashTimer += g_DeltaTimeRaw;
    if (g_MapMsg_SelectFlashTimer >= FLASH_TIMER_MAX)
    {
        g_MapMsg_SelectFlashTimer -= FLASH_TIMER_MAX;
    }

    switch (returnCode)
    {
        case NO_VALUE:
        case MapMsgReturnCode_None:
            g_MapMsg_SelectFlashTimer = Q12(0.0f);
            break;

        case MapMsgReturnCode_Select2:
        case MapMsgReturnCode_Select3:
        case MapMsgReturnCode_YesOrNo:
            g_MapMsg_Select.maxIdx   = 1;
            g_MapMsg_SelectCancelIdx = (returnCode == MapMsgReturnCode_Select3) ? 2 : 1;

            // Yes/no selection prompt.
            if (returnCode == MapMsgReturnCode_YesOrNo)
            {
                // Shows selection prompt with map messages at indices 0 and 1.
                // @note All maps have "Yes" and "No" as messages 0 and 1, respectively.
                for (i = 0; i < 2; i++)
                {
                    if (g_MapMsg_Select.selectedEntryIdx == i)
                    {
                        Gfx_StringColorSet(((g_MapMsg_SelectFlashTimer >> 10) * 3) + 4);
                    }
                    else
                    {
                        Gfx_StringColorSet(StringColorId_White);
                    }

#if VERSION_REGION_IS(NTSC) || VERSION_REGION_IS(PAL)
                    Gfx_StringPositionSet(SCREEN_WIDTH / 10, (STRING_LINE_OFFSET * i) + 98);
                    Gfx_StringDraw(g_MapOverlayHdr.mapMessages[i], MAP_MESSAGE_DISPLAY_ALL_LENGTH);
#else
                    Gfx_StringDraw_JP(g_MapOverlayHdr.mapMessages[i], i);
#endif
                }

                returnCode = MapMsgReturnCode_Select2;
            }
            // 2-3 option selection prompt.
            else
            {
                // Shows selection prompt with 2 or 3 map messages from current index + 1/2/3.
                // Requires prompt options to be arranged sequentially in the map message array with the last option
                // being to cancel, e.g.
                // `[idx]`:     "Select one of 3 options. ~S3"
                // `[idx + 1]`: "Option 1"
                // `[idx + 2]`: "Option 2"
                // `[idx + 3]`: "Cancel"
                for (i = 0; i < returnCode; i++)
                {
                    if (g_MapMsg_Select.selectedEntryIdx == i)
                    {
                        Gfx_StringColorSet(((g_MapMsg_SelectFlashTimer >> 10) * 3) + 4);
                    }
                    else
                    {
                        Gfx_StringColorSet(StringColorId_White);
                    }

#if VERSION_REGION_IS(NTSC) || VERSION_REGION_IS(PAL)
                    Gfx_StringPositionSet(32, (STRING_LINE_OFFSET * i) + 96);
                    Gfx_StringDraw(g_MapOverlayHdr.mapMessages[(mapMsgIdx + i) + 1], MAP_MESSAGE_DISPLAY_ALL_LENGTH);
#else
                    Gfx_StringDraw_JP(g_MapOverlayHdr.mapMessages[(mapMsgIdx + i) + 1], i);
#endif
                }
            }

            if (g_Controller0->buttonFlags.clicked & ControllerFlag_LStickHighUp &&
                g_MapMsg_Select.selectedEntryIdx != 0)
            {
                g_MapMsg_SelectFlashTimer = Q12(0.0f);
                g_MapMsg_Select.selectedEntryIdx--;

                Sd_SfxPlay(Sfx_MenuMove, Q7(0.0f), Q7(0.5f));
            }

            if (g_Controller0->buttonFlags.clicked & ControllerFlag_LStickHighDown &&
                g_MapMsg_Select.selectedEntryIdx != (returnCode - 1))
            {
                g_MapMsg_SelectFlashTimer = Q12(0.0f);
                g_MapMsg_Select.selectedEntryIdx++;

                Sd_SfxPlay(Sfx_MenuMove, Q7(0.0f), Q7(0.5f));
            }

            returnCode = NO_VALUE;
            break;

        // @unused This code is never returned and instant text rollout is handled elsewhere.
        case MapMsgReturnCode_DisplayAll:
            *displayLength = MAP_MESSAGE_DISPLAY_ALL_LENGTH;
            break;
    }

    return returnCode;

    #undef FLASH_TIMER_MAX
    #undef STRING_LINE_OFFSET
}
