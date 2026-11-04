void Map_RoomBgmInit_CondFalse(void)
{
    s32    bgmFlags;
    q19_12 fadeSpeed;
    s32    mapRoomIdx;

    mapRoomIdx = g_SavegamePtr->mapRoomIdx;
    fadeSpeed        = Q12(0.15f);

    switch (g_GameWork.bgmIdx)
    {
        case 16:
            bgmFlags = sharedData_800D21E8_3_s00[mapRoomIdx];

            // Check a few specific room indices.
            switch (mapRoomIdx)
            {
                case 1:
                    if (!Savegame_EventFlagGet(EventFlag_197))
                    {
                        // Test player Z position against specific value.
                        if (g_SysWork.playerWork.player.position.vz > Q12(143.2f))
                        {
                            bgmFlags = 0x6;
                        }
                        else
                        {
                            bgmFlags = 0xE;
                        }
                    }
                    break;

                case 17:
                case 20:
                    if (Savegame_EventFlagGet(EventFlag_M3S01_GeneratorOn))
                    {
                        bgmFlags |= 0x40;
                    }
                    break;
            }

            if (Savegame_EventFlagGet(EventFlag_MapMark_AltHospital2F_OperatingPrepRoomArrow) &&
                Savegame_EventFlagGet(EventFlag_MapMark_AltHospital2F_CorridorMidArrows))
            {
                bgmFlags |= 0x11E;
            }
            break;

        case 34:
            if (!Savegame_EventFlagGet(EventFlag_MapMark_FogHospital1F_StairsLocked))
            {
                fadeSpeed   = Q12(240.0f);
                bgmFlags = 0x201;
            }
            else if (Savegame_EventFlagGet(EventFlag_197))
            {
                fadeSpeed   = Q12(0.2f);
                bgmFlags = 0x201;
            }
            else
            {
                fadeSpeed   = Q12(240.0f);
                bgmFlags = 0x1FE;
            }
            break;

        case 25:
            if (!Savegame_EventFlagGet(EventFlag_295))
            {
                fadeSpeed   = Q12(240.0f);
                bgmFlags = 0x1FE;
            }
            else
            {
                fadeSpeed   = Q12(0.2f);
                bgmFlags = 0x301;
            }
            break;

        default:
            bgmFlags = BgmFlag_Layer1;
            break;
    }

    Bgm_LayersUpdate(bgmFlags, fadeSpeed, NULL);
}
