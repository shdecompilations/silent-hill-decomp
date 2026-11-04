void Map_RoomBgmInit(bool arg0)
{
    s32 bgmFlags;
    s32 fadeSpeed;
    s32 mapRoomIdx;
    u8* var_a2;

    mapRoomIdx = g_SavegamePtr->mapRoomIdx;
    fadeSpeed  = Q12(0.1f);

    if (g_GameWork.bgmIdx == BgmCmd_Track13)
    {
        var_a2 = sharedData_800ED418_4_s02;

        switch (mapRoomIdx)
        {
            case 15:
                bgmFlags = BgmFlag_Layer3;
                break;

            case 16:
                bgmFlags = (1 << 1) | (1 << 2);
                break;

            case 14:
                if (Savegame_EventFlagGet(EventFlag_352))
                {
                    fadeSpeed = Q12(10.0f);
                    bgmFlags = (1 << 0) | (1 << 8);
                }
                else if (Savegame_EventFlagGet(EventFlag_350))
                {
                    fadeSpeed = Q12(0.25f / 2.0f);
                    bgmFlags = (1 << 0) | (1 << 8);
                }
                else if (Savegame_EventFlagGet(EventFlag_348))
                {
                    if (g_SysWork.npcs[0].health < Q12(1000.0f))
                    {
                        bgmFlags = 0x1FE; // TODO: Demagic.
                    }
                    else
                    {
                        if (g_SysWork.playerWork.player.position.vy > Q12(-0.5f))
                        {
                            sharedData_800ED420_4_s02 = (1 << 1) | (1 << 3) | (1 << 8);
                        }
                        else if (g_SysWork.playerWork.player.position.vy < Q12(-2.5f))
                        {
                            sharedData_800ED420_4_s02 = (1 << 1) | (1 << 4) | (1 << 8);
                        }
                        bgmFlags = sharedData_800ED420_4_s02;

                        if (g_SysWork.npcs[0].health < Q12(2000.0f))
                        {
                            bgmFlags |= (1 << 2) | (1 << 5);
                        }
                        else if (g_SysWork.npcs[0].health < Q12(3000.0f))
                        {
                            bgmFlags |= 1 << 2;
                        }
                    }
                }
                else
                {
                    fadeSpeed = Q12(1.0f);
                    bgmFlags = (1 << 0) | (1 << 8);
                }
                break;

            default:
                bgmFlags = BgmFlag_Layer1;
                break;
        }
    }
    else if (g_GameWork.bgmIdx == BgmCmd_Track33)
    {
        var_a2 = sharedData_800ED424_4_s02;

        switch (mapRoomIdx)
        {
            case 18:
                if (Savegame_EventFlagGet(EventFlag_334) || !Savegame_EventFlagGet(EventFlag_333))
                {
                    bgmFlags = BgmFlag_Layer2;
                }
                else
                {
                    if (Savegame_EventFlagGet(EventFlag_330))
                    {
                        bgmFlags = (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4);
                        fadeSpeed = Q12(CHUNK_SIZE);
                    }
                    else
                    {
                        bgmFlags = (1 << 1) | (1 << 2) | (1 << 3);
                    }
                }
                break;

            case 19:
                Savegame_EventFlagSet(EventFlag_334);

                if (Savegame_EventFlagGet(EventFlag_320))
                {
                    bgmFlags = (1 << 0) | (1 << 8);
                }
                else
                {
                    bgmFlags = (1 << 2) | (1 << 8);
                }
                break;

            case 21:
                bgmFlags = 8;
                break;

            case 20:
                if (Savegame_EventFlagGet(EventFlag_327))
                {
                    bgmFlags = (1 << 4) | (1 << 8);
                }
                else if (Savegame_EventFlagGet(EventFlag_324) || Savegame_EventFlagGet(EventFlag_329))
                {
                    fadeSpeed = Q12(0.5f);

                    if (g_SysWork.npcs[0].health > Q12(0.0f) &&
                        g_SysWork.npcs[0].position.vy < Q12(1.0f))
                    {
                        bgmFlags = (1 << 5) | (1 << 6) | (1 << 8);
                    }
                    else
                    {
                        bgmFlags = (1 << 5) | (1 << 8);
                    }
                }
                else
                {
                    bgmFlags = (1 << 4) | (1 << 8);
                }
                break;

            default:
                bgmFlags = (1 << 0) | (1 << 8);
                break;
        }
    }
    else
    {
        var_a2 = sharedData_800ED458_4_s02;

        if (!Savegame_EventFlagGet(EventFlag_314))
        {
            if (g_SysWork.playerWork.player.position.vz < Q12(CHUNK_SIZE))
            {
                Savegame_EventFlagSet(EventFlag_314);
            }

            bgmFlags = BgmFlag_Layer2;
        }
        else
        {
            bgmFlags = sharedData_800ED42C_4_s02[mapRoomIdx];
        }
    }

    Bgm_LayersUpdate(bgmFlags, fadeSpeed, var_a2);
}
