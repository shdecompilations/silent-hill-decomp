#include "bodyprog/bodyprog.h"
#include "maps/common/bgm.h"
#include "bodyprog/events/bgm_update.h"
#include "maps/common/shared.h"

// BGM events for `MapType_RSU` maps.

void Map_RoomBgmInit(bool updateSong)
{
    s32               fadeSpeed;
    q24_7             var_s2; // TODO: Some limit value.
    s32               roomIdx;
    s_BgmLayerLimits* layerLimits;
    u16               bgmFlags; // `e_BgmFlags`

    layerLimits = &g_BgmRSU_LayerLimits0;
    roomIdx     = g_SavegamePtr->mapRoomIdx;
    fadeSpeed   = Q12(0.2f);

    switch (g_GameWork.bgmIdx)
    {
        case BgmCmd_Track5:
            g_SysWork.bgmStatusFlags &= ~BgmStatusFlag_Duck;

            switch (g_GameWork.gameState)
            {
                case GameState_InventoryScreen:
                case GameState_LoadStatusScreen:
                    if (g_GameWork.gameState != GameState_InventoryScreen || g_GameWork.gameStateSteps[1] != 25)
                    {
                        bgmFlags = BgmFlag_Layer2 | BgmFlag_Layer3 | BgmFlag_Layer4 | BgmFlag_Layer5 |
                                   BgmFlag_Layer6 | BgmFlag_Layer7 | BgmFlag_Layer8;

                        if (g_GameWork.gameStatePrev != GameState_SaveScreen)
                        {
                            fadeSpeed = Q12(240.0f);
                            break;
                        }
                    }

                    bgmFlags  = BgmFlag_Layer1 | BgmFlag_MuteAll;
                    fadeSpeed = Q12(3.0f);
                    break;

                case GameState_SaveScreen:
                    if (g_Screen_FadeStatus >= SCREEN_FADE_STATUS(ScreenFadeState_FadeInStart, false))
                    {
                        bgmFlags  = BgmFlag_Layer2 | BgmFlag_Layer3 | BgmFlag_Layer4 | BgmFlag_Layer5 |
                                    BgmFlag_Layer6 | BgmFlag_Layer7 | BgmFlag_Layer8;
                        fadeSpeed = Q12(240.0f);
                    }
                    else if (g_Screen_FadeStatus < SCREEN_FADE_STATUS(ScreenFadeState_FadeOutStart, false))
                    {
                        bgmFlags  = BgmFlag_Layer2 | BgmFlag_Layer3 | BgmFlag_Layer4 | BgmFlag_Layer5 |
                                    BgmFlag_Layer6 | BgmFlag_Layer7 | BgmFlag_Layer8;
                        fadeSpeed = Q12(240.0f);
                    }
                    else
                    {
                        bgmFlags  = BgmFlag_Layer1 | BgmFlag_MuteAll;
                        fadeSpeed = Q12(3.0f);
                    }
                    break;

                default:
                    bgmFlags  = BgmFlag_Layer1 | BgmFlag_MuteAll;
                    fadeSpeed = Q12(3.0f);
                    break;
            }
            break;

        case BgmCmd_Track41:
            layerLimits = NULL;
            if (Savegame_EventFlagGet(EventFlag_419))
            {
                fadeSpeed = Q12(2.0f);
                bgmFlags  = BgmFlag_Layer3 | BgmFlag_KeepAlive;
            }
            else
            {
                bgmFlags = BgmFlag_Layer1 | BgmFlag_KeepAlive | BgmFlag_MuteAll;
                if (!Savegame_EventFlagGet(EventFlag_418))
                {
                    fadeSpeed = Q12(2.0f);
                    bgmFlags  = BgmFlag_Layer2 | BgmFlag_KeepAlive;
                }
            }

            break;

        case BgmCmd_Track21:
            bgmFlags = BgmFlag_Layer1 | BgmFlag_KeepAlive | BgmFlag_MuteAll;

            if (!Savegame_EventFlagGet(EventFlag_469))
            {
                bgmFlags = BgmFlag_Layer2 | BgmFlag_Layer3 | BgmFlag_Layer4 | BgmFlag_Layer5 |
                           BgmFlag_Layer6 | BgmFlag_Layer7 | BgmFlag_Layer8 | BgmFlag_KeepAlive;

                if (Savegame_EventFlagGet(EventFlag_417) && !Savegame_EventFlagGet(EventFlag_460))
                {
                    bgmFlags = BgmFlag_Layer1 | BgmFlag_KeepAlive;
                }
            }
            break;

        case BgmCmd_Track20:
        default:
            if (Savegame_EventFlagGet(EventFlag_410))
            {
                bgmFlags = sharedData_800EFD08_6_s00[roomIdx];
                if (Savegame_EventFlagGet(EventFlag_426))
                {
                    bgmFlags |= BgmFlag_Layer8;
                }
            }
            else if (Savegame_EventFlagGet(EventFlag_402))
            {
                bgmFlags = sharedData_800EFD34_6_s00[roomIdx];
            }
            else
            {
                bgmFlags = BgmFlag_Layer2 | BgmFlag_Layer3;
            }
            break;
    }

    var_s2 = Q7(0.75f);
    if (roomIdx <= 19)
    {
        if (roomIdx >= 15)
        {
            var_s2 = Q7_CLAMPED(1.0f);
        }
    }

    sharedData_800EFD04_6_s00   += g_DeltaTime;
    g_BgmRSU_LayerLimits0.limits[2] = Q12_MULT_PRECISE(var_s2, (Math_Sin(sharedData_800EFD04_6_s00 / 12) + Q12(3.0f)) / 4);

    Bgm_LayersUpdate(bgmFlags, fadeSpeed, layerLimits);
}
