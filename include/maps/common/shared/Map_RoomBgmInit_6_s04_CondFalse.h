void Map_RoomBgmInit_CondFalse(void)
{
    q19_12 dist0;
    s32    bgmFlags;
    s32    fadeSpeed;
    s32    soundCmd;
    q19_12 dist;
    u8*    dataPtr;

    fadeSpeed    = Q12(0.2f);
    soundCmd = g_GameWork.bgmIdx;
    if (soundCmd == 36)
    {
        dataPtr = &sharedData_800EB738_6_s04;
        fadeSpeed   = Q12(0.1f);
        if (Savegame_EventFlagGet(EventFlag_465))
        {
            bgmFlags = 0x20;
        }
        else if (!Savegame_EventFlagGet(EventFlag_464))
        {
            fadeSpeed = Q12(240.0f);
            bgmFlags = 2;
        }
        else
        {
            bgmFlags = 0x10;
        }
    }
    else if (soundCmd == 39)
    {
        dataPtr = sharedData_800EB740_6_s04;

        if (Savegame_EventFlagGet(EventFlag_445))
        {
            if (Savegame_EventFlagGet(EventFlag_448))
            {
                bgmFlags = 0x100;
                if (Savegame_EventFlagGet(EventFlag_462))
                {
                    bgmFlags = 0x110;
                }
            }
            else if (Savegame_EventFlagGet(EventFlag_470))
            {
                sharedData_800EB740_6_s04[1] = 0x80;
                fadeSpeed                        = Q12(0.5f);

                if (!Savegame_EventFlagGet(EventFlag_447))
                {
                    bgmFlags = 0x102;
                    if (Savegame_EventFlagGet(EventFlag_463))
                    {
                        bgmFlags = 0x101;
                    }
                }
                else
                {
                    fadeSpeed = Q12(240.0f);
                    bgmFlags = 0x101;
                }
            }
            else if (Savegame_EventFlagGet(EventFlag_446))
            {
                sharedData_800EB740_6_s04[1] = 0;
                fadeSpeed                        = Q12(240.0f);
                bgmFlags                        = 0x101;
            }
            else
            {
                sharedData_800EB740_6_s04[1] = 0x60;
                bgmFlags                        = 0x10A;
            }
        }
        else
        {
            sharedData_800EB740_6_s04[1] = 0x60;

            if (Savegame_EventFlagGet(EventFlag_443))
            {
                bgmFlags = 0x108;
                if (!Savegame_EventFlagGet(EventFlag_444))
                {
                    if (Savegame_EventFlagGet(EventFlag_455))
                    {
                        bgmFlags = 0x10A;
                    }
                }
                else
                {
                    bgmFlags = 0x10A;
                }
            }
            else
            {
                fadeSpeed = Q12(240.0f);

                bgmFlags = 0x301;
                if (Savegame_EventFlagGet(EventFlag_461))
                {
                    bgmFlags = 0x104;
                }
            }
        }
    }
    else
    {
        dist    = Q12(0.0f);
        dataPtr = &sharedData_800EB748_6_s04;

        switch (g_SavegamePtr->mapRoomIdx)
        {
            case 3:
                bgmFlags = 4;

                dist = Math_Distance2dGet(&g_SysWork.playerWork.player.position, &sharedData_800EB750_6_s04);
                if (dist < Q12(10.0f))
                {
                    dist = Q12(1.0f / 32.0f);
                }
                else
                {
                    dist = Q12(1.0f / 32.0f) - Q12_MULT_PRECISE(dist - Q12(10.0f), Q12(1.0f / 256.0f));
                    if (dist < Q12(0.0f))
                    {
                        dist = Q12(0.0f);
                    }
                }
                break;

            case 1:
            case 2:
            case 4:
            case 5:
                bgmFlags = 2;
                break;

            default:
                bgmFlags = 1;
                break;
        }

        sharedData_800EB74A_6_s04 = dist;
    }

    Bgm_LayersUpdate(bgmFlags, fadeSpeed, dataPtr);
}
