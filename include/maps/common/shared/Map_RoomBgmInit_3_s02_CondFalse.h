void Map_RoomBgmInit_CondFalse(void)
{
    const static s_BgmLayerLimits D_800CA70C = { { 0x80, 0x70, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00 } };

    u16           bgmFlags;
    q19_12        fadeSpeed;
    s_BgmLayerLimits* dataPtr;
    s_BgmLayerLimits  data;
    s32           roomIdx;

    dataPtr = sharedData_800D1D14_3_s02;
    roomIdx = g_SavegamePtr->mapRoomIdx;

    if (g_GameWork.bgmIdx == BgmCmd_Track8)
    {
        fadeSpeed = Q12(240.0f);
        bgmFlags = 0xFE;
    }
    else if (g_GameWork.bgmIdx == BgmCmd_Track32)
    {
        fadeSpeed = Q12(0.15f);
        if (Savegame_EventFlagGet(EventFlag_338))
        {
            bgmFlags = 4;
            if (!Savegame_EventFlagGet(EventFlag_340))
            {
                bgmFlags = 2;
                if (Savegame_EventFlagGet(EventFlag_339))
                {
                    bgmFlags = 0xE;
                }
            }
        }
        else
        {
            bgmFlags = 1;
        }
    }
    else
    {
        bgmFlags = sharedData_800D1D1C_3_s02[roomIdx];
        fadeSpeed = Q12(0.15f);

        switch (roomIdx)
        {
            case 23:
            case 26:
            case 38:
            case 45:
                if (!(Savegame_EventFlagGet(EventFlag_285) || Savegame_EventFlagGet(EventFlag_286)))
                {
                    bgmFlags = 0x1E;
                }
                else if (Savegame_EventFlagGet(EventFlag_285))
                {
                    Savegame_EventFlagSet(EventFlag_286);
                }
                break;

            case 5:
            case 25:
            case 37:
                if (!Savegame_EventFlagGet(EventFlag_285) && Savegame_EventFlagGet(EventFlag_286))
                {
                    Savegame_EventFlagSet(EventFlag_285);
                    Savegame_EventFlagClear(EventFlag_286);
                }

                if (!Savegame_EventFlagGet(EventFlag_285))
                {
                    bgmFlags = 0x1FE;
                }
                else if (!Savegame_EventFlagGet(EventFlag_286))
                {
                    bgmFlags = 0x13E;
                }

                break;

            case 4:
                if (!Player_ItemRemove(InvItemId_ExaminationRoomKey, 0))
                {
                    bgmFlags = BgmFlag_Layer1;
                }

            case 6:
            case 7:
            case 10:
            case 11:
            case 12:
            case 13:
            case 14:
            case 15:
            case 21:
            case 22:
            case 24:
            case 27:
            case 28:
            case 29:
            case 30:
            case 31:
            case 32:
            case 34:
            case 35:
            case 36:
            case 39:
            case 40:
            case 41:
            case 42:
            case 43:
            case 44:
            case 46:
            case 47:
                Savegame_EventFlagSet(EventFlag_286);
                break;

            case 3:
                if (g_GameWork.bgmIdx == BgmCmd_Track31)
                {
                    data    = D_800CA70C;
                    dataPtr = &data;

                    if (!Savegame_EventFlagGet(EventFlag_293))
                    {
                        fadeSpeed = Q12(240.0f);
                        bgmFlags = BgmFlag_Layer3;
                    }
                    else if (!Savegame_EventFlagGet(EventFlag_298))
                    {
                        fadeSpeed = Q12(0.25f);
                        bgmFlags = BgmFlag_Layer2;
                    }
                    else if (!Savegame_EventFlagGet(EventFlag_299))
                    {
                        fadeSpeed = Q12(0.125f);
                        bgmFlags = BgmFlag_Layer4;
                    }
                    else
                    {
                        fadeSpeed = Q12(240.0f);
                        if (!Savegame_EventFlagGet(EventFlag_294))
                        {
                            fadeSpeed = Q12(0.5f);
                            bgmFlags = BgmFlag_Layer1 | BgmFlag_MuteAll;
                        }
                        else
                        {
                            bgmFlags = BgmFlag_Layer1;
                        }
                    }
                }
                else
                {
                    bgmFlags = BgmFlag_Layer1 | BgmFlag_MuteAll;
                }
                break;

            case 58:
                if (!Savegame_EventFlagGet(EventFlag_260))
                {
                    bgmFlags = BgmFlag_Layer1;
                }
                break;
        }
    }

    Bgm_LayersUpdate(bgmFlags, fadeSpeed, dataPtr);
}
