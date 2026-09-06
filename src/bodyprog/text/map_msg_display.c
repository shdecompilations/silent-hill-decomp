#include "game.h"

#include <psyq/libetc.h>
#include <psyq/libpad.h>
#include <psyq/strings.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/events/map_msg.h"
#include "bodyprog/math/math.h"
#include "bodyprog/screen/screen_data.h"
#include "bodyprog/screen/screen_draw.h"
#include "bodyprog/sound/sound_system.h"
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
u8             g_MapMsg_AudioLoadBlock;
s8             g_MapMsg_SelectCancelIdx;

// @hack JP calls different `Gfx_StringColorSet` / `Gfx_StringDraw` funcs here.
// The normal funcs available are also used in JP, so can't be renamed.
// For now override `Gfx_StringColorSet` calls in this file until those JP funcs get figured out.
#if VERSION_REGION_IS(NTSCJ)
    #define Gfx_StringColorSet Gfx_StringColorSet_JP
#endif

s32 Gfx_MapMsg_Draw(s32 mapMsgIdx) // 0x800365B8
{
    #define FINISH_CUTSCENE 0xFF
    #define FINISH_MAP_MSG  0xFF

    s32         temp_s1;
    bool        hasInput;
    s32         temp;
    s32         var_a1;
    static s32  stateMachineIdx0;
    static s32  stateMachineIdx1;
    static s32  displayLength;
    static s32  activeMapMsgIdx;
    static s32  displayLengthInc;
    static bool loadAudio;

    // Check for user input.
    hasInput = false;
    if ((g_Controller0->buttonFlags.clicked & (g_GameWorkPtr->config.controllerConfig.enter |
                                               g_GameWorkPtr->config.controllerConfig.cancel)) ||
        (g_Controller0->buttonFlags.held & g_GameWorkPtr->config.controllerConfig.skip))
    {
        hasInput = true;
    }

    g_SysWork.playerWork.player.properties.player.gasWeaponPowerTimer = Q12(0.0f);
    func_8004C564(g_SysWork.playerCombat.weaponAttack, WEAPON_ATTACK(EquippedWeaponId_RockDrill, AttackInputType_Tap));

    if (activeMapMsgIdx != mapMsgIdx)
    {
        g_SysWork.isMgsStringSet = false;
    }

    switch (g_SysWork.isMgsStringSet)
    {
        case false:
            g_SysWork.mapMsgTimer            = NO_VALUE;
            g_MapMsg_Select.maxIdx           = NO_VALUE;
            g_MapMsg_Select.selectedEntryIdx = 0;
            g_MapMsg_AudioLoadBlock          = 0;
            g_MapMsg_CurrentIdx              = mapMsgIdx;
            stateMachineIdx0                 = 0;
            stateMachineIdx1                 = 0;
            activeMapMsgIdx                  = mapMsgIdx;
            displayLength                    = 0;
            displayLengthInc                 = 2; // Advance 2 glyphs at a time.

            Gfx_MapMsg_Reset();
            var_a1 = Gfx_MapMsg_WidthsCompute(g_MapMsg_CurrentIdx);

#if VERSION_REGION_IS(NTSCJ)
            if (var_a1 != 0)
            {
                switch (var_a1)
                {
                    case 2:
                    case 3:
                        func_8004B45C(g_MapMsg_CurrentIdx + 1, var_a1);
                        break;

                    case 4:
                        func_8004B45C(0, 2);
                        break;
                }
            }
#endif

            loadAudio = true;
            g_SysWork.isMgsStringSet++;
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
#if VERSION_REGION_IS(NTSC)
            Gfx_StringPositionSet(40, 160);
#endif

            displayLength += displayLengthInc;
            displayLength  = CLAMP(displayLength, 0, MAP_MESSAGE_DISPLAY_ALL_LENGTH);

            if (g_MapMsg_AudioLoadBlock != 0 && g_SysWork.mapMsgTimer > Q12(0.0f))
            {
                g_SysWork.mapMsgTimer -= g_DeltaTimeRaw;
                g_SysWork.mapMsgTimer  = CLAMP(g_SysWork.mapMsgTimer, Q12(0.0f), INT_MAX);
            }

            temp_s1 = stateMachineIdx0;
            if (temp_s1 == NO_VALUE)
            {
                if (g_MapMsg_AudioLoadBlock == 0)
                {
                    Game_TimerUpdate();
                }

                temp = stateMachineIdx1;
                if (temp == temp_s1)
                {
                    if (g_MapMsg_Select.maxIdx == temp)
                    {
                        if (!((g_MapMsg_AudioLoadBlock & (1 << 0)) || !hasInput) ||
                            (g_MapMsg_AudioLoadBlock != 0 && g_SysWork.mapMsgTimer == Q12(0.0f)))
                        {
                            stateMachineIdx1 = FINISH_MAP_MSG;

                            if (g_SysWork.bgmStatusFlags & BgmStatusFlag_VoiceDialog)
                            {
                                SD_Call(19);
                            }
                            break;
                        }
                    }
                    else if (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.cancel)
                    {
                        g_MapMsg_Select.maxIdx           = temp;
                        g_MapMsg_Select.selectedEntryIdx = g_MapMsg_SelectCancelIdx;

                        Sd_SfxPlay(Sfx_MenuCancel, Q8(0.0f), Q8(0.25f));

                        if (g_SysWork.silentYesSelection)
                        {
                            g_SysWork.silentYesSelection = false;
                        }

                        stateMachineIdx1 = FINISH_MAP_MSG;
                        break;
                    }
                    else if (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.enter)
                    {
                        g_MapMsg_Select.maxIdx = temp;

                        if (g_MapMsg_Select.selectedEntryIdx == (s8)g_MapMsg_SelectCancelIdx)
                        {
                            Sd_SfxPlay(Sfx_MenuCancel, Q8(0.0f), Q8(0.25f));
                        }
                        else if (!g_SysWork.silentYesSelection)
                        {
                            Sd_SfxPlay(Sfx_MenuConfirm, Q8(0.0f), Q8(0.25f));
                        }

                        if (g_SysWork.silentYesSelection)
                        {
                            g_SysWork.silentYesSelection = false;
                        }

                        stateMachineIdx1 = FINISH_MAP_MSG;
                        break;
                    }
                }
                else if ((!(g_MapMsg_AudioLoadBlock & (1 << 0)) && hasInput && g_MapMsg_Select.maxIdx != 0) ||
                         (g_MapMsg_AudioLoadBlock != 0 && g_SysWork.mapMsgTimer == Q12(0.0f)))
                {
                    if (g_MapMsg_Select.maxIdx != NO_VALUE)
                    {
                        g_MapMsg_Select.maxIdx = NO_VALUE;
                        stateMachineIdx1       = FINISH_MAP_MSG;
                        break;
                    }

                    g_MapMsg_CurrentIdx++;
                    g_SysWork.mapMsgTimer = g_MapMsg_Select.maxIdx;

                    var_a1 = Gfx_MapMsg_WidthsCompute(g_MapMsg_CurrentIdx);

#if VERSION_REGION_IS(NTSCJ)
                    if (var_a1 != 0)
                    {
                        switch (var_a1)
                        {
                            case 2:
                            case 3:
                                func_8004B45C(g_MapMsg_CurrentIdx + 1, var_a1);
                                break;

                            case 4:
                                func_8004B45C(0, 2);
                                break;
                        }
                    }
#endif

                    displayLength    = 0;
                    stateMachineIdx0 = 0;

                    if (g_MapMsg_AudioLoadBlock == MapMsgAudioLoadBlock_J2)
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
                if (hasInput)
                {
                    displayLength = MAP_MESSAGE_DISPLAY_ALL_LENGTH;
                }
            }

            stateMachineIdx0 = 0;
            stateMachineIdx1 = Gfx_MapMsg_SelectionUpdate(g_MapMsg_CurrentIdx, &displayLength);

            if (stateMachineIdx1 != 0 && stateMachineIdx1 < MapMsgReturnCode_Select4)
            {
                stateMachineIdx0 = NO_VALUE;
            }
    }

    if (stateMachineIdx1 != FINISH_MAP_MSG)
    {
        return MapMsgState_Idle;
    }

    g_SysWork.isMgsStringSet         = false;
    g_SysWork.enableHalfHeightGlyphs = false;
    displayLength                    = 0;

    if (g_SysWork.bgmStatusFlags & BgmStatusFlag_VoiceDialog)
    {
        loadAudio = true;
    }

    return g_MapMsg_Select.selectedEntryIdx + 1;

    #undef FINISH_CUTSCENE
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
        case MapMsgReturnCode_Select4:
            g_MapMsg_Select.maxIdx   = 1;
            g_MapMsg_SelectCancelIdx = (returnCode == 3) ? 2 : 1;

            if (returnCode == MapMsgReturnCode_Select4)
            {
                // Shows selection prompt with map messages at indices 0 and 1.
                // All maps have "Yes" and "No" as messages 0 and 1, respectively.
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

#if VERSION_REGION_IS(NTSC)
                    Gfx_StringPositionSet(32, (STRING_LINE_OFFSET * i) + 98);
                    Gfx_StringDraw(g_MapOverlayHdr.mapMessages[i], MAP_MESSAGE_DISPLAY_ALL_LENGTH);
#else
                    Gfx_StringDraw_JP(g_MapOverlayHdr.mapMessages[i], i);
#endif
                }

                returnCode = 2;
            }
            else
            {
                // Shows selection prompt with 2 or 3 map messages from current index + 1/2/3.
                // Requires prompt options to be arranged sequentially in the map message array, e.g.
                // `[idx]`:     "Select one of 3 options. ~S3"
                // `[idx + 1]`: "Option 1"
                // `[idx + 2]`: "Option 2"
                // `[idx + 3]`: "Option 3"
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

#if VERSION_REGION_IS(NTSC)
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

                Sd_SfxPlay(Sfx_MenuMove, Q8(0.0f), Q8(0.25f));
            }

            if (g_Controller0->buttonFlags.clicked & ControllerFlag_LStickHighDown &&
                g_MapMsg_Select.selectedEntryIdx != (returnCode - 1))
            {
                g_MapMsg_SelectFlashTimer = Q12(0.0f);
                g_MapMsg_Select.selectedEntryIdx++;

                Sd_SfxPlay(Sfx_MenuMove, Q8(0.0f), Q8(0.25f));
            }

            returnCode = NO_VALUE;
            break;

        case MapMsgReturnCode_DisplayAll:
            *displayLength = MAP_MESSAGE_DISPLAY_ALL_LENGTH;
            break;
    }

    return returnCode;

    #undef FLASH_TIMER_MAX
    #undef STRING_LINE_OFFSET
}
