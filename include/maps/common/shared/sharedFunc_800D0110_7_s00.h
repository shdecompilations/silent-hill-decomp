void sharedFunc_800D0110_7_s00(void)
{
    s32               bgmFlagsAndLayer2;
    s32               bgmFlagsAndLayer3;
    s32               i;
    q19_12            fadeSpeed;
    s32               mapRoomIdx;
    s32               bgmFlags;
    s_BgmLayerLimits* bgmLayerLimit;

    bgmFlags      = 1 << 0;
    fadeSpeed     = Q12(0.1f);
    mapRoomIdx    = g_SavegamePtr->mapRoomIdx;
    bgmLayerLimit = NULL;

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
                        }
                        else
                        {
                            bgmFlags  = BgmFlag_Layer1 | BgmFlag_MuteAll;
                            fadeSpeed = Q12(3.0f);
                        }
                    }
                    else
                    {
                        bgmFlags  = BgmFlag_Layer1 | BgmFlag_MuteAll;
                        fadeSpeed = Q12(3.0f);
                    }
                    break;

                case GameState_SaveScreen:
                    switch (g_ScreenFade_Status)
                    {
                        default:
                            bgmFlags  = BgmFlag_Layer2 | BgmFlag_Layer3 | BgmFlag_Layer4 | BgmFlag_Layer5 |
                                        BgmFlag_Layer6 | BgmFlag_Layer7 | BgmFlag_Layer8;
                            fadeSpeed = Q12(240.0f);
                            break;

                        case 2:
                        case 3:
                        case 4:
                        case 5:
                            bgmFlags  = BgmFlag_Layer1 | BgmFlag_MuteAll;
                            fadeSpeed = Q12(3.0f);
                            break;
                    }
                    break;

                default:
                    bgmFlags  = BgmFlag_Layer1 | BgmFlag_MuteAll;
                    fadeSpeed = Q12(3.0f);
                    break;
            }
            break;

        case BgmCmd_Track4:
            bgmLayerLimit = &sharedData_800D2F18_7_s00;

            switch (mapRoomIdx)
            {
                case 20:
                case 21:
                case 23:
                case 24:
                    if (Savegame_EventFlagGet(EventFlag_482) || Savegame_EventFlagGet(EventFlag_483))
                    {
                        bgmFlags  = BgmFlag_Layer3;
                        fadeSpeed = Q12(240.0f);
                    }
                    else if (Savegame_EventFlagGet(EventFlag_571))
                    {
                        if (Savegame_EventFlagGet(EventFlag_572))
                        {
                            fadeSpeed = Q12(5.0f);
                            bgmFlags  = BgmFlag_Layer3;
                        }
                        else
                        {
                            fadeSpeed = Q12(240.0f);
                            bgmFlags  = BgmFlag_Layer1 | BgmFlag_MuteAll;

                            if (Sd_MidiChannelTaskGet() == 0)
                            {
                                Savegame_EventFlagSet(EventFlag_572);
                            }
                        }
                    }
                    else if (Savegame_EventFlagGet(EventFlag_572))
                    {
                        fadeSpeed = Q12(1.0f);
                        bgmFlags  = BgmFlag_Layer1 | BgmFlag_MuteAll;
                    }
                    else if (Savegame_EventFlagGet(EventFlag_570))
                    {
                        fadeSpeed = Q12(1.0f);
                        bgmFlags  = BgmFlag_Layer1 | BgmFlag_MuteAll;

                        if (Sd_MidiChannelTaskGet() == 0)
                        {
                            Savegame_EventFlagSet(EventFlag_572);
                        }
                    }
                    else
                    {
                        bgmFlags = BgmFlag_Layer2;
                    }
                    break;

                default:
                    bgmFlags = BgmFlag_Layer1 | BgmFlag_MuteAll;
                    break;
            }
            break;

        case BgmCmd_Track10:
        case BgmCmd_Track15:
            bgmLayerLimit = &sharedData_800D2F20_7_s00;

            switch (mapRoomIdx)
            {
                default:
                    bgmFlags = BgmFlag_Layer1;
                    break;

                case 1:
                    if (!Savegame_EventFlagGet(EventFlag_476))
                    {
                        bgmFlags = BgmFlag_Layer1;
                    }
                    else if (Savegame_EventFlagGet(EventFlag_471))
                    {
                        fadeSpeed = Q12(240.0f);
                        bgmFlags  = BgmFlag_Layer1 | BgmFlag_MuteAll;
                    }
                    else if (Savegame_EventFlagGet(EventFlag_474))
                    {
                        fadeSpeed = Q12(0.1f);
                        bgmFlags  = 1 << 0;
                    }
                    else
                    {
                        fadeSpeed = Q12(240.0f);
                        bgmFlags  = BgmFlag_Layer2 | BgmFlag_Layer3 | BgmFlag_Layer4 | BgmFlag_Layer5 |
                                    BgmFlag_Layer6 | BgmFlag_Layer7 | BgmFlag_Layer8;
                    }
                    break;

                case 2:
                    if (Savegame_EventFlagGet(EventFlag_475))
                    {
                        fadeSpeed = Q12(1.0f);
                        bgmFlags  = BgmFlag_Layer2 | BgmFlag_Layer3 | BgmFlag_Layer4 | BgmFlag_Layer5 |
                                    BgmFlag_Layer6 | BgmFlag_Layer7 | BgmFlag_Layer8;
                    }
                    else
                    {
                        fadeSpeed = Q12(0.25f);
                        bgmFlags  = BgmFlag_Layer1 | BgmFlag_MuteAll;
                    }
                    break;

                case 3:
                    fadeSpeed = Q12(0.15f);
                    bgmFlags  = BgmFlag_Layer1 | BgmFlag_MuteAll;
                    break;
            }
            break;

        case BgmCmd_Track14:
        case BgmCmd_Track22:
            bgmLayerLimit = &sharedData_800D2F74_7_s00;
            bgmFlags      = sharedData_800D2F28_7_s00[mapRoomIdx];

            switch (mapRoomIdx)
            {
                case 36:
                    fadeSpeed = Q12(0.5f);
                    break;

                case 21:
                    fadeSpeed = Q12(240.0f);
                    break;

                case 4:
                    if (!Savegame_EventFlagGet(EventFlag_485))
                    {
                        bgmFlags = BgmFlag_Layer1;
                    }
                    break;

                case 7:
                    if (!Savegame_EventFlagGet(EventFlag_M7S01_PickupKeyOfPhaleg))
                    {
                        bgmFlags = BgmFlag_Layer1;
                    }
                    break;

                case 22:
                    if (Savegame_EventFlagGet(EventFlag_487) && !Savegame_EventFlagGet(EventFlag_489))
                    {
                        bgmFlags = BgmFlag_Layer3 | BgmFlag_Layer4;
                    }
                    break;

                case 17:
                    if (Savegame_EventFlagGet(EventFlag_486))
                    {
                        if (!Savegame_EventFlagGet(EventFlag_549))
                        {
                            bgmFlags = BgmFlag_Layer5;
                        }
                        else
                        {
                            bgmFlags = BgmFlag_Layer3;
                        }
                    }
                    break;

                case 25:
                    if (!Savegame_EventFlagGet(EventFlag_492) && Savegame_EventFlagGet(EventFlag_498))
                    {
                        bgmFlags |= BgmFlag_Layer3;
                    }

                    if (!Savegame_EventFlagGet(EventFlag_494) && Savegame_EventFlagGet(EventFlag_499))
                    {
                        bgmFlags |= 1 << 1;
                    }
                    break;

                case 29:
                    if (Savegame_EventFlagGet(EventFlag_555))
                    {
                        if (g_GameWork.bgmIdx == BgmCmd_Track22)
                        {
                            bgmFlags = BgmFlag_Layer3;
                        }
                        else
                        {
                            bgmFlags = BgmFlag_Layer4;
                        }
                    }
                    else if (Savegame_EventFlagGet(EventFlag_573))
                    {
                        if (Savegame_EventFlagGet(EventFlag_552))
                        {
                            if (g_GameWork.bgmIdx == BgmCmd_Track22)
                            {
                                bgmFlags = BgmFlag_Layer4;
                            }
                            else
                            {
                                bgmFlags = BgmFlag_Layer2;
                            }
                        }
                        else
                        {
                            if (g_GameWork.bgmIdx == BgmCmd_Track22)
                            {
                                bgmFlags = BgmFlag_Layer4 | BgmFlag_Layer5;
                            }
                            else
                            {
                                bgmFlags = BgmFlag_Layer2 | BgmFlag_Layer5;
                            }
                        }
                    }
                    else
                    {
                        if (!Savegame_EventFlagGet(EventFlag_557))
                        {

                            if (Savegame_EventFlagGet(EventFlag_M7S02_PickupDaggerOfMelchior))
                            {
                                bgmFlags = BgmFlag_Layer3 | BgmFlag_Layer4;
                            }
                            else
                            {
                                bgmFlags = BgmFlag_Layer3;
                            }
                        }
                        else
                        {
                            bgmFlags = BgmFlag_Layer5;
                        }
                    }
                    break;

                case 31:
                    if (!Savegame_EventFlagGet(EventFlag_548))
                    {
                        bgmFlags = BgmFlag_Layer1;
                    }
                    break;

                case 32:
                    if (!Savegame_EventFlagGet(EventFlag_551))
                    {
                        fadeSpeed = Q12(240.0f);
                        bgmFlags  = 1;
                    }
                    break;

                case 33:
                    if (!Savegame_EventFlagGet(EventFlag_560))
                    {
                        for (bgmFlags = 0, i = 540; i < 545; i++)
                        {
                            if (Savegame_EventFlagGet(i))
                            {
                                bgmFlags = (bgmFlags * 2) | BgmFlag_Layer2;
                            }
                        }
                    }
                    break;
            }

            if (g_GameWork.bgmIdx == BgmCmd_Track22)
            {
                bgmFlagsAndLayer2 = bgmFlags & BgmFlag_Layer2;
                bgmFlagsAndLayer3 = bgmFlags & BgmFlag_Layer3;
                bgmFlags         ^= (bgmFlagsAndLayer2 | bgmFlagsAndLayer3);
                bgmFlags         |= (bgmFlagsAndLayer2 * 2) | (bgmFlagsAndLayer3 >> 1);
            }
            break;

        case BgmCmd_Track35:
            if (Savegame_EventFlagGet(EventFlag_582))
            {
                bgmFlags = BgmFlag_Layer1 | BgmFlag_KeepAlive | BgmFlag_MuteAll;
                break;
            }

            if (Savegame_EventFlagGet(EventFlag_580))
            {
                if (!Savegame_EventFlagGet(EventFlag_575))
                {
                    fadeSpeed = Q12(240.0f);
                    bgmFlags  = BgmFlag_Layer6 | BgmFlag_KeepAlive;
                }
                else
                {
                    fadeSpeed = Q12(0.5f);
                    bgmFlags  = BgmFlag_Layer1 | BgmFlag_KeepAlive | BgmFlag_MuteAll;
                }
                break;
            }

            if (Savegame_EventFlagGet(EventFlag_577))
            {
                if (g_SysWork.npcs[0].health < Q12(500.0f))
                {
                    bgmFlags = BgmFlag_Layer2 | BgmFlag_Layer3 | BgmFlag_Layer4 | BgmFlag_Layer5 |
                               BgmFlag_Layer6 | BgmFlag_KeepAlive;
                }
                else
                {
                    bgmFlags = BgmFlag_Layer4 | BgmFlag_Layer6 | BgmFlag_KeepAlive;
                }
                break;
            }

            if (Savegame_EventFlagGet(EventFlag_591))
            {
                bgmFlags = BgmFlag_Layer2 | BgmFlag_KeepAlive;
            }
            else
            {
                bgmFlags = BgmFlag_KeepAlive;
            }

            if (Savegame_EventFlagGet(EventFlag_592))
            {
                bgmFlags |= BgmFlag_Layer3;
            }

            if (Savegame_EventFlagGet(EventFlag_593))
            {
                bgmFlags |= BgmFlag_Layer4;
            }

            if (Savegame_EventFlagGet(EventFlag_575))
            {
                bgmFlags |= BgmFlag_Layer5;
            }

            if (Savegame_EventFlagGet(EventFlag_576))
            {
                fadeSpeed = Q12(240.0f);
            }

            if (bgmFlags == BgmFlag_KeepAlive)
            {
                bgmFlags = BgmFlag_Layer1 | BgmFlag_KeepAlive | BgmFlag_MuteAll;
            }
            break;

        case BgmCmd_Track29:
            bgmLayerLimit = &sharedData_800D2F7C_7_s00;
            fadeSpeed     = Q12(240.0f);

            if (!Savegame_EventFlagGet(EventFlag_576))
            {
                bgmFlags = BgmFlag_Layer1 | BgmFlag_KeepAlive;
            }
            else
            {
                if (Savegame_EventFlagGet(EventFlag_591))
                {
                    g_SysWork.bgmStatusFlags |= BgmStatusFlag_RadioActive;
                }

                bgmFlags = BgmFlag_KeepAlive;
            }
            break;

        case BgmCmd_Track40:
            bgmLayerLimit = &sharedData_800D2F84_7_s00;
            if (Savegame_EventFlagGet(EventFlag_575))
            {
                fadeSpeed = Q12(0.25f);
                bgmFlags  = BgmFlag_Layer1 | BgmFlag_KeepAlive;
                break;
            }

            if (Savegame_EventFlagGet(EventFlag_580))
            {
                fadeSpeed = Q12(2.0f);
                bgmFlags  = BgmFlag_Layer1 | BgmFlag_KeepAlive | BgmFlag_MuteAll;
                break;
            }

            if (Savegame_EventFlagGet(EventFlag_578))
            {
                if (!Savegame_EventFlagGet(EventFlag_593))
                {
                    fadeSpeed = Q12(240.0f);
                    bgmFlags  = BgmFlag_Layer1 | BgmFlag_KeepAlive;
                }
                else
                {
                    bgmFlags = BgmFlag_Layer6 | BgmFlag_KeepAlive;
                }

                if (Savegame_EventFlagGet(EventFlag_592))
                {
                    if (Savegame_EventFlagGet(EventFlag_593))
                    {
                        bgmFlags |= BgmFlag_Layer7 | BgmFlag_Layer8;
                    }
                    else
                    {
                        bgmFlags |= BgmFlag_Layer7;
                    }
                }
                break;
            }

            if (Savegame_EventFlagGet(EventFlag_577) || Savegame_EventFlagGet(EventFlag_591))
            {
                if (g_SysWork.bgmLayerVolumes[1] == Q12(1.0f))
                {
                    if (Savegame_EventFlagGet(EventFlag_577))
                    {
                        fadeSpeed = Q12(1.0f);

                        if (!(g_SysWork.npcs[2].flags & CharaFlag_Hit))
                        {
                            bgmFlags = BgmFlag_Layer2 | BgmFlag_KeepAlive;
                        }
                        else
                        {
                            bgmFlags = BgmFlag_Layer2 | BgmFlag_Layer3 | BgmFlag_Layer4 | BgmFlag_Layer5 |
                                       BgmFlag_KeepAlive;
                        }
                    }
                    else
                    {
                        fadeSpeed = Q12(0.12f);

                        if (g_SysWork.bgmLayerVolumes[2] == Q12(1.0f))
                        {
                            if (g_SysWork.bgmLayerVolumes[3] == Q12(1.0f))
                            {
                                bgmFlags = BgmFlag_Layer2 | BgmFlag_Layer3 | BgmFlag_Layer4 | BgmFlag_Layer5 |
                                           BgmFlag_KeepAlive;
                            }
                            else
                            {
                                bgmFlags = BgmFlag_Layer2 | BgmFlag_Layer3 | BgmFlag_Layer4 | BgmFlag_KeepAlive;
                            }
                        }
                        else
                        {
                            if (Savegame_EventFlagGet(EventFlag_576))
                            {
                                bgmFlags = BgmFlag_Layer2 | BgmFlag_Layer3 | BgmFlag_KeepAlive;
                            }
                            else
                            {
                                bgmFlags = BgmFlag_Layer2 | BgmFlag_KeepAlive;
                            }
                        }
                    }
                    break;
                }

                if (Savegame_EventFlagGet(EventFlag_577))
                {
                    fadeSpeed = Q12(240.0f);
                }

                bgmFlags = BgmFlag_Layer2 | BgmFlag_KeepAlive;

                if (g_SysWork.bgmLayerVolumes[0] != Q12(1.0f))
                {
                    if (Sd_MidiChannelTaskGet() == 0)
                    {
                        Sd_BgmChannelSet();
                    }
                    else
                    {
                        Sd_MidiChannelsVolumeSet(0, 0x7F);
                        Sd_MidiChannelsVolumeSet(1, 1);

                        g_SysWork.bgmLayerVolumes[0] = Q12(1.0f);
                        g_SysWork.bgmLayerVolumes[1] = 32;
                    }
                }
            }
            else
            {
                bgmFlags = BgmFlag_Layer1 | BgmFlag_KeepAlive | BgmFlag_MuteAll;
            }
            break;
    }

    Bgm_LayersUpdate(bgmFlags, fadeSpeed, bgmLayerLimit);
}
