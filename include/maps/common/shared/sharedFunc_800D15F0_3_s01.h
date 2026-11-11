#if defined(MAP7_S01) || defined(MAP7_S02)
    #define EventFlagBase0 EventFlag_213
    #define EventFlagBase1 EventFlag_212
    #define EventFlagBase2 EventFlag_216
#else
    #define EventFlagBase0 EventFlag_212
    #define EventFlagBase1 EventFlag_M3S01_GeneratorOn
    #define EventFlagBase2 EventFlag_215
#endif

#ifdef MAP3_S01
    #define MAX_IDX 7
#elif defined(MAP7_S01) || defined(MAP7_S02)
    #define MAX_IDX 5
#else
    #define MAX_IDX 6
#endif

#if defined(MAP7_S01) || defined(MAP7_S02)
    #define CHECK_IDX 4
#else
    #define CHECK_IDX 5
#endif

void sharedFunc_800D15F0_3_s01(void)
{
    s32 j;
    s32 i;
    s16 cursorX;
    s32 temp;
    s32 temp2;

    // Skip.
    if (Pad_ButtonClickedBind(skip) &&
        g_SysWork.sysStateSteps[0] >= 9 && g_SysWork.sysStateSteps[0] < 17)
    {
        SysWork_StateStepSet(0, 17);
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:

            Player_ControlFreeze();
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, false, false, false);

            sharedData_800D4D14_3_s01 = 0;
            sharedData_800D4D10_3_s01 = 0;

            for (i = 0; i < 8; i++)
            {
                Savegame_EventFlagClear(EventFlag_216 + i);
            }

            for (i = 0; i < 4; i++)
            {
                if (Savegame_EventFlagGet(EventFlagBase0 + i))
                {
                    sharedData_800D4D14_3_s01 = (sharedData_800D4CD4_3_s01[i + 1][1] - 110) << 12;
                    sharedData_800D4D18_3_s01 = i;
                }
            }

            SysWork_StateStepIncrement(0);

        case 1:
#ifdef MAP3_S01
            if (Savegame_EventFlagGet(EventFlag_MapMark_AltHospital2F_OperatingPrepRoomArrow) &&
                Savegame_EventFlagGet(EventFlag_MapMark_AltHospital2F_CorridorMidArrows))
            {
                Event_BgTextureCmd(BgTextureCmd_Auto, FILE_TIM_ELEVAT_2_TIM, false);
            }
            else
            {
                Event_BgTextureCmd(BgTextureCmd_Auto, FILE_TIM_ELEVATOR_TIM, false);
            }
#elif defined(MAP7_S01) || defined(MAP7_S02)
            Event_BgTextureCmd(BgTextureCmd_Auto, FILE_TIM_ELEVAT_3_TIM, false);
#else
            Event_BgTextureCmd(BgTextureCmd_Auto, FILE_TIM_ELEVAT_4_TIM, false);
#endif
            break;

        case 2:
            Event_ScreenFadeCmd(ScreenFadeCmd_Wait, true, false, false, false);
            break;

        case 3:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, false, false, false);
            break;

        case 4:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            sharedData_800D4D10_3_s01 += (g_Controller0->normalizedSticks.sticks.leftX * 16384) / 75;
            sharedData_800D4D10_3_s01  = CLAMP_RANGE(sharedData_800D4D10_3_s01, Q12(-70.0f), Q12(68.0f));

            sharedData_800D4D14_3_s01 += (g_Controller0->normalizedSticks.sticks.leftY * 16384) / 75;
            sharedData_800D4D14_3_s01  = CLAMP_RANGE(sharedData_800D4D14_3_s01, Q12(-110.0f), Q12(110.0f));

            Game_TimerUpdate();

            cursorX = FP_FROM(sharedData_800D4D10_3_s01, Q12_SHIFT) + 8;
            Gfx_CursorDraw(cursorX, FP_FROM(sharedData_800D4D14_3_s01, Q12_SHIFT) + 8, 8, 8, 0, 64, 32, 32, 128, 192, 0, 12);

            if (Pad_ButtonClickedBind(cancel))
            {
                SysWork_StateStepSet(0, 6);
                break;
            }

            if (!Pad_ButtonClickedBind(enter))
            {
                break;
            }

            for (i = 0; i < MAX_IDX; i++)
            {
                temp = sharedData_800D4CD4_3_s01[i][0];
                if ((temp - 160) > FP_FROM(sharedData_800D4D10_3_s01, Q12_SHIFT))
                {
                    continue;
                }

                switch (i)
                {
                    case 0:
                        if ((temp - 140) >= FP_FROM(sharedData_800D4D10_3_s01, Q12_SHIFT))
                        {
                            break;
                        }
                        continue;

                    default:
                        if ((temp - 141) < FP_FROM(sharedData_800D4D10_3_s01, Q12_SHIFT))
                        {
                            continue;
                        }
                        break;
                }

                if ((sharedData_800D4CD4_3_s01[i][1] - 120) > FP_FROM(sharedData_800D4D14_3_s01, Q12_SHIFT) ||
                    (sharedData_800D4CD4_3_s01[i][1] - 100) < FP_FROM(sharedData_800D4D14_3_s01, Q12_SHIFT))
                {
                    continue;
                }

#if defined(MAP3_S03) || defined(MAP3_S04) || defined(MAP3_S05) || defined(MAP7_S01) || defined(MAP7_S02)
                SD_Call(Sfx_Unk1500);
#endif
                if (i == CHECK_IDX)
                {
#ifdef MAP3_S01
                    if (Savegame_EventFlagGet(EventFlag_MapMark_AltHospital2F_OperatingPrepRoomArrow) &&
                        Savegame_EventFlagGet(EventFlag_MapMark_AltHospital2F_CorridorMidArrows))
                    {
                        Savegame_EventFlagSet(EventFlag_224);
                        SysWork_StateStepSet(0, 5);
                        SD_Call(Sfx_Unk1500);

                        sharedData_800D4D10_3_s01 = 1;
                    }
#endif
                }
                else
                {
#ifdef MAP3_S01
                    if (i != 6 && (i <= 0 || !Savegame_EventFlagGet(EventFlag_M3S01_GeneratorOn + i)))
#else
                    if (i <= 0 || !Savegame_EventFlagGet(EventFlagBase1 + i))
#endif
                    {
                        if (i != 0)
                        {
                            Savegame_EventFlagSet(EventFlagBase2 + i);
                            sharedData_800D4D10_3_s01 = 1;

                            temp2                     = 1;
                            sharedData_800D4D18_3_s01 = ABS((sharedData_800D4D18_3_s01 + temp2) - i);
                            SysWork_StateStepSet(0, 5);
                        }
                        else
                        {
                            for (j = 0; j < 4; j++)
                            {
                                if (Savegame_EventFlagGet(EventFlag_212 + j))
                                {
                                    Savegame_EventFlagSet(EventFlag_216 + j);
                                    break;
                                }
                            }

                            sharedData_800D4D10_3_s01 = 0;
                            SysWork_StateStepSet(0, 5);
                        }
                    }
#ifdef MAP3_S01
                    SD_Call(Sfx_Unk1500);
#endif
                }
            }
            break;

        case 5:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);

            for (i = 0; i < 4; i++)
            {
                Savegame_EventFlagClear(EventFlag_212 + i);
            }

            sharedData_800D4D0C_3_s01.vy = 0;
            sharedData_800D4D0C_3_s01.vx = 0;

#ifdef MAP3_S01
            PLAYER_CHARA().rotation.vy = Q12_ANGLE(-112.5f);
            PLAYER_CHARA().position.vx = Q12(58.6f);
            PLAYER_CHARA().position.vz = Q12(19.4f);
#elif defined(MAP7_S01) || defined(MAP7_S02)
            PLAYER_CHARA().rotation.vy = Q12_ANGLE(-90.0f);
            PLAYER_CHARA().position.vx = Q12(-101.9f);
            PLAYER_CHARA().position.vz = Q12(-60.3f);
#else
            PLAYER_CHARA().rotation.vy = Q12_ANGLE(-135.0f);
            PLAYER_CHARA().position.vx = Q12(18.2f);
            PLAYER_CHARA().position.vz = Q12(59.7f);
#endif
            SysWork_StateStepSet(0, 8);
            break;

        case 6:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, false, false, false);
            break;

        case 7:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, false, false, false);
            break;

        case 8:
            if (sharedData_800D4D10_3_s01 != 0)
            {
                SD_Call(Sfx_Unk1499);
            }

            SysWork_StateStepIncrement(0);

        case 9:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);

            if (sharedData_800D4D10_3_s01 != 0)
            {
                g_SysWork.sysStateStepData[0] += g_DeltaTime;
                if (g_SysWork.sysStateStepData[0] > Q12(1.0f))
                {
                    g_SysWork.sysStateStepData[0] = Q12(1.0f);
                }

                Sfx_WithFalloffAndPitchPlay(Sfx_Unk1499, &sharedData_800CB088_3_s01, g_SysWork.sysStateStepData[0] >> 5, Q12(16.0f), 0);
                Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, false, Q12(1.0f), false);
            }
            else
            {
                Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, false, Q12(2.0f), false);
            }
            break;

        case 10:
            if (sharedData_800D4D10_3_s01 != 0)
            {
                Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, false, Q12(1.0f), false);
                Sfx_WithFalloffAndPitchPlay(Sfx_Unk1499, &sharedData_800CB088_3_s01, Q8(0.5f), Q12(16.0f), 0u);
            }
            else
            {
                Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, false, Q12(2.0f), false);

                if (g_SysWork.sysStateSteps[0] != 10)
                {
                    Sfx_WithFlagsPlay(Sfx_Unk1501, &sharedData_800CB094_3_s01, Q8(0.5f), SfxFlag_None);
                    SysWork_StateStepSet(0, 14);
                }
            }
            break;

        case 11:
            if (g_SysWork.sysStateStepData[0] > ((sharedData_800D4D18_3_s01 - 1) * Q12(0.75f)))
            {
                SysWork_StateStepIncrement(0);
            }
            else
            {
                g_SysWork.sysStateStepData[0] += g_DeltaTime;
            }
            break;

        case 12:
            Sfx_WithFalloffAndPitchPlay(Sfx_Unk1499, &sharedData_800CB088_3_s01, Q8(0.5f) - (g_SysWork.sysStateStepData[0] >> 5), Q12(16.0f), 0);

            g_SysWork.sysStateStepData[0] += g_DeltaTime;
            if (g_SysWork.sysStateStepData[0] > Q12(0.5f))
            {
#if defined(MAP7_S01) || defined(MAP7_S02)
                Sfx_WithFlagsPlay(Sfx_Unk1498, &sharedData_800CB094_3_s01, Q8(0.5f), SfxFlag_None);
#else
                Sfx_WithFlagsPlay(Sfx_Unk1498, &sharedData_800CB088_3_s01, Q8(0.5f), SfxFlag_None);
#endif
                SysWork_StateStepIncrement(0);
            }
            break;

        case 13:
            Sfx_WithFalloffAndPitchPlay(Sfx_Unk1499, &sharedData_800CB088_3_s01, Q8(0.25f) - (g_SysWork.sysStateStepData[0] >> 5), Q12(16.0f), 0);

            g_SysWork.sysStateStepData[0] += g_DeltaTime;
            if (g_SysWork.sysStateStepData[0] > Q12(0.5f))
            {
                SD_Call(Sfx_Unk1502);
#if defined(MAP3_S03) || defined(MAP3_S04) || defined(MAP3_S05) || defined(MAP7_S01) || defined(MAP7_S02)
                Sfx_WithFlagsPlay(Sfx_Unk1501, &sharedData_800CB094_3_s01, Q8(0.5f), SfxFlag_None);
#else
                Sfx_WithFlagsPlay(Sfx_Unk1501, &sharedData_800CB0A0_3_s01, Q8(0.5f), SfxFlag_None);
#endif
                Sd_SfxStop(Sfx_Unk1499);
                SysWork_StateStepIncrement(0);
            }
            break;

        case 14:
            sharedData_800D4D0C_3_s01.vx = MIN(Q12_MULT_PRECISE(g_DeltaTime, Q12(0.8f)) + sharedData_800D4D0C_3_s01.vx, Q12(0.35f));

            for (i = 0; i < 4; i++)
            {
                g_WorldObject_Dr[i].position.vz += Q12_MULT_PRECISE(g_DeltaTime, (i & 1) ? sharedData_800D4D0C_3_s01.vx : -sharedData_800D4D0C_3_s01.vx);
            }

            Event_WaitTimer(Q12(0.3f), false);
            break;

        case 15:
        case 16:
            sharedData_800D4D0C_3_s01.vx = MIN(Q12_MULT_PRECISE(g_DeltaTime, Q12(0.8f)) + sharedData_800D4D0C_3_s01.vx, Q12(0.35f));
            sharedData_800D4D0C_3_s01.vy = MIN(Q12_MULT_PRECISE(g_DeltaTime, Q12(0.8f)) + sharedData_800D4D0C_3_s01.vy, Q12(0.35f));

            for (i = 0; i < 4; i++)
            {
                g_WorldObject_Dr[i].position.vz += Q12_MULT_PRECISE(g_DeltaTime, (i & 1) ? sharedData_800D4D0C_3_s01.vx : -sharedData_800D4D0C_3_s01.vx);
            }

            for (i = 4; i < ARRAY_SIZE(g_WorldObject_Dr); i++)
            {
                g_WorldObject_Dr[i].position.vz += Q12_MULT_PRECISE(g_DeltaTime, (i & 1) ? sharedData_800D4D0C_3_s01.vy : -sharedData_800D4D0C_3_s01.vy);
            }

            if (g_SysWork.sysStateSteps[0] == 15)
            {
                Event_WaitTimer(Q12(0.3f), false);
            }
            else
            {
                Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, false, Q12(1.5f), false);
            }
            break;

        case 17:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, false, false, false);
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);

#ifdef MAP3_S01
            func_8008D448();
            Game_FlashlightAttribsSet();
            g_SysWork.lightIntensity = Q12(1.0f);
#endif
            Sd_SfxStop(Sfx_Unk1499);
            Sd_SfxStop(Sfx_Unk1501);
            Sd_SfxStop(Sfx_Unk1498);

#if defined(MAP7_S01) || defined(MAP7_S02)
            temp2 = Q12(-60.9f);

            for (i = 5; i >= 0; i--)
            {
                g_WorldObject_Dr[i].position.vz = temp2;
            }
#endif
            break;
    }
}
