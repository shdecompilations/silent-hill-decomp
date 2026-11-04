void sharedFunc_800DB60C_7_s01(void)
{
    s32       temp_v1_4;
    s32       i;
    s32       k;
    s32       j;
    s32       x;
    s32       y;
    s16       cursorX;
    SPRT*     sprt;
    DR_TPAGE* tPage;

    switch (g_SysWork.sysStateSteps[0])
    {
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
            sprt = (SPRT*)GsOUT_PACKET_P;

            for (i = 0; i < 3; i++)
            {
                for (j = 0; j < 3; j++)
                {
                    for (k = 0; k < 3; k++)
                    {
                        if (sharedData_800E1694_7_s01 & (1 << (((i * 9) + (k * 3)) + j)))
                        {
                            setRGBC0(sprt, 128, 128, 128, PRIM_RECT | RECT_TEXTURE);

                            x = sharedData_800E1578_7_s01[i][j][k][0];
                            y = sharedData_800E1578_7_s01[i][j][k][1];

                            setUV0(sprt, 64, 16);
                            setWH(sprt, 9, 9);
                            setXY0Fast(sprt, x - 164, y - 124);
                            sprt->clut = 910;
                            addPrimFast(&g_OrderingTable0[g_ActiveBufferIdx].org[1], sprt, 4);
                            sprt++;
                        }
                    }
                }
            }

            tPage = (DR_TPAGE*)sprt;

            setDrawTPage(tPage, 0, 0, getTPageN(1, 0, 7, 0));
            AddPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[1], tPage);
            tPage++;

            GsOUT_PACKET_P = (PACKET*)tPage;
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            SysWork_StateStepIncrement(0);

        case 1:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, 0, false);
            break;

        case 2:
            g_SysWork.bgmStatusFlags |= BgmStatusFlag_Pause;

            Fs_QueueStartReadTim(FILE_TIM_3X3DOR_TIM, IMAGE_BUFFER_5, &g_ItemInspectionImg);
            Fs_QueueWaitForEmpty();

            sharedData_800E2CA8_7_s01 = 0;
            sharedData_800E2CAC_7_s01 = 0;
            sharedData_800E1694_7_s01 = 0;

            SysWork_StateStepIncrement(0);

        case 3:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, 0, false);
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            break;

        case 4:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);

            sharedData_800E2CA8_7_s01 += (g_Controller0->normalizedSticks.sticks_0.leftX << 13) / 75;
            sharedData_800E2CA8_7_s01  = CLAMP_RANGE(sharedData_800E2CA8_7_s01, Q12(-115.0f), Q12(115.0f));

            sharedData_800E2CAC_7_s01 += (g_Controller0->normalizedSticks.sticks_0.leftY << 13) / 75;
            sharedData_800E2CAC_7_s01  = CLAMP_RANGE(sharedData_800E2CAC_7_s01, Q12(-105.0f), Q12(105.0f));

            Game_TimerUpdate();

            cursorX = FP_FROM(sharedData_800E2CA8_7_s01, Q12_SHIFT) + 8;
            Gfx_CursorDraw(cursorX, FP_FROM(sharedData_800E2CAC_7_s01, Q12_SHIFT) + 8, 8, 8, 0, 0x40, 0x20, 0x20, 0x80, 0xC0u, 0, 0xC);

            if (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.cancel)
            {
#ifndef MAP7_S02
                SD_Call(Sfx_Unk1652);
#endif
                if (sharedData_800E1694_7_s01 != 0)
                {

                    sharedData_800E1694_7_s01 = 0;
#ifdef MAP7_S02
                    SD_Call(Sfx_Unk1652);
#endif
                }
                else
                {
                    SysWork_StateStepSet(0, 8);
                }

                break;
            }

            if (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.enter)
            {
                for (i = 0; i < 3; i++)
                {
                    for (j = 0; j < 3; j++)
                    {
                        for (k = 0; k < 3; k++)
                        {
                            if ((sharedData_800E1578_7_s01[i][j][k][0] - 169) > FP_FROM(sharedData_800E2CA8_7_s01, Q12_SHIFT) ||
                                (sharedData_800E1578_7_s01[i][j][k][0] - 151) < FP_FROM(sharedData_800E2CA8_7_s01, Q12_SHIFT))
                            {
                                continue;
                            }

                            if ((sharedData_800E1578_7_s01[i][j][k][1] - 129) > FP_FROM(sharedData_800E2CAC_7_s01, Q12_SHIFT) ||
                                (sharedData_800E1578_7_s01[i][j][k][1] - 111) < FP_FROM(sharedData_800E2CAC_7_s01, Q12_SHIFT))
                            {
                                continue;
                            }

                            if ((SQUARE(sharedData_800E1578_7_s01[i][j][k][0] - 160 - FP_FROM(sharedData_800E2CA8_7_s01, Q12_SHIFT)) +
                                 SQUARE(sharedData_800E1578_7_s01[i][j][k][1] - 120 - FP_FROM(sharedData_800E2CAC_7_s01, Q12_SHIFT))) > 81)
                            {
                                continue;
                            }

                            temp_v1_4 = 1 << (((i * 9) + (k * 3)) + j);
                            if (!(sharedData_800E1694_7_s01 & temp_v1_4))
                            {
                                sharedData_800E1694_7_s01 += temp_v1_4;
                            }
                            else
                            {
                                sharedData_800E1694_7_s01 -= temp_v1_4;
                            }

                            SD_Call(Sfx_Unk1652);

                            if ((sharedData_800E1694_7_s01 == sharedData_800E1574_7_s01 && Savegame_EventFlagGet(EventFlag_493)) ||
                                (sharedData_800E1694_7_s01 == sharedData_800E1570_7_s01 && Savegame_EventFlagGet(EventFlag_495)))
                            {
                                SysWork_StateStepSet(0, 5);
                            }
                        }
                    }
                }
            }
            break;

        case 5:
            Event_WaitTimer(Q12(0.6f), false);
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            break;

        case 6:
            SD_Call(Sfx_Unk1343);

            if (!Savegame_EventFlagGet(EventFlag_493))
            {
                Savegame_EventFlagSet(EventFlag_494);
            }
            else
            {
                Savegame_EventFlagSet(EventFlag_492);
            }

            SysWork_StateStepIncrement(0);

        case 7:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_WaitTimer(0x800, false);
            break;

        case 8:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, 0, false);
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, 0, false);

            Savegame_EventFlagClear(EventFlag_493);
            Savegame_EventFlagClear(EventFlag_495);
            break;
    }
}
