void Map_RoomBgmInit(bool arg0)
{
    s32 var_s1;
    s32 var_s2;
    s32 temp_a2;
    u8* var_s3;
    u16 bgmFlags;

    var_s3  = sharedData_800EFCFC_6_s00;
    temp_a2 = g_SavegamePtr->mapRoomIdx;
    var_s1  = Q12(0.2f);

    switch (g_GameWork.bgmIdx)
    {
        case 5:
            g_SysWork.bgmStatusFlags &= ~BgmStatusFlag_Duck;

            switch (g_GameWork.gameState)
            {
                case GameState_InventoryScreen:
                case GameState_LoadStatusScreen:
                    if ((g_GameWork.gameState != GameState_InventoryScreen) || (g_GameWork.gameStateSteps[1] != 25))
                    {
                        bgmFlags = 0xFE;

                        if (g_GameWork.gameStatePrev != GameState_SaveScreen)
                        {
                            var_s1 = Q12(240.0f);
                            break;
                        }
                    }

                    bgmFlags  = 0x201;
                    var_s1 = Q12(3.0f);
                    break;

                case GameState_SaveScreen:
                    if (g_Screen_FadeStatus >= SCREEN_FADE_STATUS(ScreenFadeState_FadeInStart, false))
                    {
                        bgmFlags  = 0xFE;
                        var_s1 = Q12(240.0f);
                    }
                    else if (g_Screen_FadeStatus < SCREEN_FADE_STATUS(ScreenFadeState_FadeOutStart, false))
                    {
                        bgmFlags  = 0xFE;
                        var_s1 = Q12(240.0f);
                    }
                    else
                    {
                        bgmFlags  = 0x201;
                        var_s1 = Q12(3.0f);
                    }
                    break;

                default:
                    bgmFlags  = 0x201;
                    var_s1 = Q12(3.0f);
                    break;
            }
            break;

        case 41:
            var_s3 = NULL;
            if (Savegame_EventFlagGet(EventFlag_419))
            {
                var_s1 = Q12(2.0f);
                bgmFlags  = 0x104;
            }
            else
            {
                bgmFlags = 0x301;
                if (!Savegame_EventFlagGet(EventFlag_418))
                {
                    var_s1 = Q12(2.0f);
                    bgmFlags  = 0x102;
                }
            }

            break;

        case 21:
            bgmFlags = 0x301;

            if (!Savegame_EventFlagGet(EventFlag_469))
            {
                bgmFlags = 0x1FE;

                if (Savegame_EventFlagGet(EventFlag_417) && !Savegame_EventFlagGet(EventFlag_460))
                {
                    bgmFlags = 0x101;
                }
            }
            break;

        case 20:
        default:
            if (Savegame_EventFlagGet(EventFlag_410))
            {
                bgmFlags = sharedData_800EFD08_6_s00[temp_a2];
                if (Savegame_EventFlagGet(EventFlag_426))
                {
                    bgmFlags |= 0x80;
                }
            }
            else if (Savegame_EventFlagGet(EventFlag_402))
            {
                bgmFlags = sharedData_800EFD34_6_s00[temp_a2];
            }
            else
            {
                bgmFlags = 6;
            }
            break;
    }

    var_s2 = 96;
    if (temp_a2 < 20)
    {
        if (temp_a2 >= 15)
        {
            var_s2 = 127;
        }
    }

    sharedData_800EFD04_6_s00   += g_DeltaTime;
    sharedData_800EFCFC_6_s00[2] = Q12_MULT_PRECISE(var_s2, (Math_Sin(sharedData_800EFD04_6_s00 / 12) + Q12(3.0f)) / 4);

    Bgm_LayersUpdate(bgmFlags, var_s1, var_s3);
}
