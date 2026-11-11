#include "inline_no_dmpsx.h"

#include <psyq/gtemac.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/events/event_data.h"
#include "bodyprog/world/effects.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player/player.h"
#include "main/rng.h"
#include "maps/map1/map1_s06.h"
#include "maps/common/particle.h"
#include "maps/common/characters/player.h"
#include "maps/common/characters/split_head.h"

#include "../src/maps/common/particles/drool.c"

#include "../src/maps/common/particles/particle.c"

#include "../src/maps/common/characters/player.c"


// TODO: Move this line into separate `Chara_SplitHead` split.
#include "../src/maps/common/characters/split_head.c" // 0x800D02CC

#include "../src/maps/common/map_util.c" // 0x800D5350

void Map_RoomBgmInit(bool arg0) // 0x800D5400
{
    Bgm_LayersUpdate(Savegame_EventFlagGet(EventFlag_133) ? 0xFE : 1, Q12(0.1f), D_800D71E8);
}

void GameBoot_LoadScreen_StageString(void) {}

#include "maps/common/shared/MapEvent_DoorJammed.h" // 0x800D5450

#include "maps/common/shared/MapEvent_DoorLocked.h" // 0x800D54E4

#include "maps/common/shared/MapEvent_DoorUnlocked.h" // 0x800D5578

void func_800D560C(void) {}

const char* MAP_MESSAGES[] = {
    #include "maps/common/shared/map_msg_common.inc"
#if VERSION_REGION_IS(NTSC)
    /* 15 */ "~J0(1.3)\t",
    /* 16 */ "~J2(1.5)\tWhat_was_that...? ~E ",
    /* 17 */ "~J0(2.4)\tWho_in_the_hell_was_that? ~E ",
    /* 18 */ "\tThere_is_a_ ~C2 K._Gordon_key ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 19 */ "\tThe_boiler_is_off. ~E ",
    /* 20 */ "\tIt_appears_to_be_broken. ~E ",
    /* 21 */ "\tThe_valve_is_tightly_shut. ~N\n\tCan't_move_it. ~E ",
    /* 22 */ "~J0(2.0)\tI_hear_a_... ",
    /* 23 */ "~J0(3.0)\tA_church_bell,_maybe? ~N\n\tWho_is_ringing_the_bell? ~E ",
    /* 24 */ "\tMoore,_Ranaldo,_Gordon... ~N\n\tThis_must_be_the_list_of_teachers. ~E ",
    /* 25 */ "\tList_of_teachers. ",
    /* 26 */ "\tK._Gordon..._that's_it. ~N\n\tI'll_note_the_address_down ~N\n\ton_the_map. ~E ",
    /* 27 */ "~H\tThere_is_a_ ~C5 School_map ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 28 */ "~J1(3.5)\tWhere_am_I? ",
    /* 29 */ "~J1(3.0)\tThis_is..._a_boiler_room!? ",
    /* 30 */ "~J1(3.5)\tWhat_is_going_on_here? ~E "
#elif VERSION_REGION_IS(NTSCJ)
    /* 15 */ "~J0(1.3)\t",
    /* 16 */ "~J2(1.5)\t今のは……？ ~E ",
    /* 17 */ "~J1(2.4)\t今のはいったい……誰なんだ？ ~E ",
    /* 18 */ "\t~C2 Ｋ・ゴードンの鍵 ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 19 */ "\tボイラーは止まっている ~E ",
    /* 20 */ "\t壊れていて動かないようだ ~E ",
    /* 21 */ "\tバルブは固く閉められていて ~N\n\t動かせそうにない ~E ",
    /* 22 */ "~J0(2.0)\t鐘の音…… ",
    /* 23 */ "~J0(2.0)\t教会の鐘か？ ~N\n\t\t\tいったい誰が鳴らしてるんだ……？ ~E ",
    /* 24 */ "\tムーア　ラナルド　ゴードン…… ~N\n\tどうやら　この学校の教員名簿のようだ ~E ",
    /* 25 */ "\t教員名簿がある ",
    /* 26 */ "\tＫ・ゴードン……名前がのっているな ~N\n\t家の場所を地図に書き写しておこう ~E ",
    /* 27 */ "~H\t~C5 学校の地図 ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 28 */ "~J1(3.5)\tそれにここは……ボイラー室？ ",
    /* 29 */ "~J1(3.0)\tいったいどうなってるんだ？ ",
    /* 30 */ "~J1(3.5)\tわからないことばかりだ…… ~E "
#endif
};

void func_800D5614(void) // 0x800D5614
{
    if (Pad_ButtonClickedBind(skip) &&
        g_SysWork.sysStateSteps[0] >= 3 && g_SysWork.sysStateSteps[0] < 13)
    {
        SysWork_StateStepSet(0, 14);
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            WorldEnv_PresetsSet(19, 4);

            CutsceneBorder_ForceShow();
            D_800D7760         = 0;

            // Warp player.
            PLAYER_CHARA().position.vx = Q12(21.55f);
            PLAYER_CHARA().position.vz = Q12(-14.6f);
            PLAYER_CHARA().rotation.vy = Q12_ANGLE(180.0f);
            Model_AnimFlagsClear(&PLAYER_CHARA().model, 2);

            // Warp camera.
            Event_CameraPositionSet(NULL,
                                    Q12(22.47f), Q12(-3.43f), Q12(-12.94f),
                                    Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                    true);
            Event_CameraLookAtSet(NULL,
                                  Q12(21.59f), Q12(-1.52f), Q12(-16.34f),
                                  Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                  true);

            Game_TurnFlashlightOff();

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(1.0f), false);
            SysWork_StateStepIncrement(0);

        case 1:
            if (Sd_AmbientSfxInit() == 0)
            {
                SysWork_StateStepIncrement(0);
            }
            else
            {
                break;
            }

        case 2:
            Event_ScreenFadeCmd(ScreenFadeCmd_Wait, false, 0, Q12(0.0f), false);
            break;

        case 3:
            Event_DisplayMapMsgWithAudio(15, &D_800D7760, &D_800D775C);
            break;

        case 4:
            Savegame_EventFlagSet(EventFlag_143);
            SysWork_StateStepIncrement(0);

        case 5:
            Event_WaitTimer(Q12(1.5f), false);
            D_800DAF78 += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.1f);
            break;

        case 6:
            Event_DisplayMapMsgWithAudio(17, &D_800D7760, &D_800D775C);
            break;

        case 7:
            // Warp player.
            PLAYER_CHARA().position.vx = Q12(21.45f);
            PLAYER_CHARA().position.vz = Q12(-15.26f);
            PLAYER_CHARA().rotation.vy = Q12_ANGLE(180.0f);
            Model_AnimFlagsSet(&PLAYER_CHARA().model, 2);

            // Warp camera.
            Event_CameraPositionSet(NULL,
                                    Q12(22.11f), Q12(-5.18f), Q12(-20.45f),
                                    Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                    true);
            Event_CameraLookAtSet(NULL,
                                  Q12(21.24f), Q12(-2.4f), Q12(-17.71f),
                                  Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                  true);

            SysWork_StateStepIncrement(0);

        case 8:
            Event_WaitTimer(Q12(1.0f), false);
            break;

        case 9:
            Game_TurnFlashlightOn();
            SysWork_StateStepIncrement(0);

        case 10:
            Event_WaitTimer(Q12(1.0f), false);
            break;

        case 11:
            Event_DisplayMapMsg(false, 28, 0, 0, 0, false);
            break;

        case 12:
            Event_WaitTimer(Q12(2.0f), false);
            break;

        case 13:
            SysWork_StateStepReset();
            break;

        case 14:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 15:
            SD_Call(19);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);

            // Warp player.
            PLAYER_CHARA().position.vx = Q12(21.45f);
            PLAYER_CHARA().position.vz = Q12(-15.26f);
            PLAYER_CHARA().rotation.vy = Q12_ANGLE(180.0f);

            // Warp camera.
            Event_CameraPositionSet(NULL,
                                    Q12(22.11f), Q12(-5.18f), Q12(-20.45f),
                                    Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                    true);
            Event_CameraLookAtSet(NULL,
                                  Q12(21.24f), Q12(-2.4f), Q12(-17.71f),
                                  Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                  true);

            Model_AnimFlagsSet(&PLAYER_CHARA().model, 2);
            Game_TurnFlashlightOn();

            Savegame_EventFlagSet(EventFlag_143);

            D_800DAF78 = Q12(0.15f);
            SysWork_StateStepIncrement(0);
            break;

        default:
            Savegame_EventFlagSet(EventFlag_134);

            WorldEnv_PresetsSet(7, 4);

            vcReturnPreAutoCamWork(false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);

            Player_ControlUnfreeze(false);

            SysWork_StateSetNext(SysState_Gameplay);

            func_8003A16C();
            break;
    }
}

void func_800D5B98(void) // 0x800D5B98
{
    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_InvItemCmd(InvItemCmd_QueueLoad, InvItemId_KGordonKey, 0, false);
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            Event_CharaAnimPlayToEnd(&PLAYER_CHARA(), 59);
            break;

        case 3:
            Event_InvItemCmd(InvItemCmd_AwaitLoad, InvItemId_KGordonKey, 0, false);
            break;

        case 4:
            Savegame_EventFlagSet(EventFlag_M1S06_PickupKGordonKey);

            if (Gfx_PickupItemAnimate(InvItemId_KGordonKey))
            {
                Event_DisplayMapMsg(true, 18, 5, 6, 0, false);
            }
            break;

        case 5:
            Event_InvItemCmd(InvItemCmd_AddItem, InvItemId_KGordonKey, 1, false);
            SysWork_StateStepSet(0, 7);
            break;

        case 6:
            Savegame_EventFlagClear(EventFlag_M1S06_PickupKGordonKey);
            SysWork_StateStepIncrement(0);

        case 7:
            Event_CharaAnimPlayToEnd(&PLAYER_CHARA(), 60);
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;
    }
}

void func_800D5D6C(u16 scrollX) // 0x800D5D6C
{
    PaperMap_DrawScaled(scrollX, 120, Q12(0.5f));
    func_80067914(1, scrollX, 120, Q12(0.5f));
    func_80068E0C(1, 1, 0, 0, scrollX, 120, Q12(0.5f));
}

void func_800D5DD8(void) // 0x800D5DD8
{
    typedef enum _EventState
    {
        EventState_Skip = 14
    } e_EventState;

    // Skip.
    if (Pad_ButtonClickedBind(skip))
    {
        if (g_SysWork.sysStateSteps[0] > 0 && g_SysWork.sysStateSteps[0] < 6)
        {
            SysWork_StateStepSet(0, EventState_Skip);
        }

        if (D_800D7790 == 0 && g_SysWork.sysStateSteps[0] > 6 && g_SysWork.sysStateSteps[0] < 12)
        {
            D_800D7790 = 1;
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 0, Q12(0.0f), false);
        }
    }

    if (D_800D7790)
    {
        if (Screen_IsFadeFinished())
        {
            D_800D7790 = 0;
            SysWork_StateStepSet(0, 13);
        }
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Screen_FadeTimestepReset();

            CutsceneBorder_ForceShow();
            PLAYER_CHARA().position.vx = Q12(109.2f);
            PLAYER_CHARA().position.vz = Q12(43.2f);
            PLAYER_CHARA().rotation.vy = Q12(0.4553f);

            D_800D778E = g_SavegamePtr->paperMapIdx;
            g_SavegamePtr->paperMapIdx = 1;
            D_800D7790 = 0;

            // Warp camera.
            Event_CameraPositionSet(NULL,
                                    Q12(106.5799f), Q12(-3.6199f), Q12(45.23f),
                                    Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                    true);
            Event_CameraLookAtSet(NULL,
                                  Q12(109.19f), Q12(-1.3699f), Q12(43.2f),
                                  Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                  true);

            SysWork_StateStepIncrement(0);

        case 1:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(1.0f), false);
            break;

        case 2:
            Event_WaitTimer(Q12(1.0f), false);
            break;

        case 3:
            Event_DisplayMapMsg(false, 22, 0, 0, 0, false); // "I hear a church bell."
            break;

        case 4:
            Fs_QueueStartSeek(FILE_TIM_MP_0TOWN_TIM + D_800A99B5);
            SysWork_StateStepIncrement(0);

        case 5:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 6:
            Event_PaperMapCmd(PaperMapCmd_Load, 1);

            D_800D778D = 0;
            D_800D778F = 0;

            SysWork_StateStepIncrement(0);

        case 7:
            func_800D5D6C(0);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.0f), false);
            break;

        case 8:
            func_800D5D6C(0);
            Event_WaitTimer(Q12(0.6f), false);
            break;

        case 9:
            D_800D778D++;

            func_800D5D6C(D_800D778D);

            if (D_800D778D == 122)
            {
                SysWork_StateStepIncrement(0);
            }
            break;

        case 10:
            func_800D5D6C(122);
            Event_WaitTimer(Q12(0.6f), false);
            break;

        case 11:
            func_800D5D6C(122);
            func_80068E0C(2, 1, 926, D_800D778F, 122, 120, Q12(0.5f));

            D_800D778F++;
            if (D_800D778F >= 0x80)
            {
                D_800D778F = 0x80;

                if (Pad_ButtonClicked(Pad_BindingGet(enter) | Pad_BindingGet(cancel)))
                {
                    SysWork_StateStepIncrement(0);
                }

                if (g_SysWork.sysStateSteps[0] == 11)
                {
                    Event_WaitTimer(Q12(1.5f), false);
                }
            }
            break;

        case 12:
            func_800D5D6C(122);
            func_80068E0C(2, 1, 926, 128, 122, 120, Q12(0.5f));
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 13:
            Event_PaperMapCmd(PaperMapCmd_Unload, 0);
            SysWork_StateStepIncrement(0);

        case EventState_Skip:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        default:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            vcReturnPreAutoCamWork(true);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);

            g_SavegamePtr->paperMapIdx = D_800D778E;

            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);

            Savegame_EventFlagSet(EventFlag_M1S06_ChurchMarkedOnMap);
            Savegame_EventFlagSet(EventFlag_MapMark_OldTown_BalkanChurchCircle);
            break;
    }
}

void func_800D6338(void) // 0x800D6338
{
    g_Screen_BackgroundImgGamma = Q8(7.0f / 16.0f);

    if (Savegame_EventFlagGet(EventFlag_M1S06_PickupKGordonKey))
    {
        Event_DisplayMapMsgWithBg(FILE_TIM_RSCBOOK_TIM, Q12(0.0f), Q12(0.0f), 25);
        Savegame_EventFlagSet(EventFlag_MapMark_OldTown_KGordonDotAndSignOnly);
    }
    else
    {
        Event_DisplayMapMsgWithBg(FILE_TIM_RSCBOOK_TIM, Q12(0.0f), Q12(0.0f), 24);
    }
}

void MapEvent_PaperMapTake(void) // 0x800D63B0
{
    Event_PaperMapTake(6, EventFlag_M1S00_PickupMap, 27);
}

void Map_WorldObjectsInit(void) // 0x800D63D8
{
    WorldObject_PosePositionInit(&D_800DAF84, "REDX_HID", 20.4568f, -0.8345f, -17.97f);
    WorldObject_Init(&D_800DAFB4, "KEY_HIDE", 20.9021f, -0.002f, -17.26f, 0.0f, -37.45f, 0.0f);

    D_800D7761 = 0;
    D_800DAFE4 = 0;
    D_800DAF78 = 0;

    WorldObject_Init(&D_800DAFF4, g_CommonWorldObjectNames[1], 61.8531f, -0.805f, 140.61f, 0, 92.9f, 0.0f);
    WorldObject_Init(&D_800DB024, "PICT00_H", 133.294f, -0.8614f, 19.04f, 0.0f, 162.6f, 0.0f);
    WorldObject_Init(&D_800DB064, "MAP_HIDE", 96.9405f, -0.3365f, 21.092f, 0.0f, 66.8f, 0.0f);

    if (!Savegame_EventFlagGet(EventFlag_143))
    {
        Sd_SfxStop(Sfx_Unk1359);
    }
}

void Map_WorldObjectsUpdate(void) // 0x800D6578
{
    VECTOR3 soundPos;
    s32     vol;

    if (PLAYER_IN_MAP_CHUNK(2, 0))
    {
        if (!Savegame_EventFlagGet(EventFlag_M1S00_PickupMap))
        {
            WorldObjects_Add(&D_800DB064.object, &D_800DB064.position, &D_800DB064.rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(1, 3))
    {
       WorldObjects_Add(&D_800DAFF4.object, &D_800DAFF4.position, &D_800DAFF4.rotation);
    }

    if (PLAYER_IN_MAP_CHUNK(0, -1))
    {
        WorldObjects_Add(&D_800DAF84.object, &D_800DAF84.position, &SVECTOR3_Zero);

        if (!Savegame_EventFlagGet(EventFlag_M1S06_PickupKGordonKey))
        {
            WorldObjects_Add(&D_800DAFB4.object, &D_800DAFB4.position, &D_800DAFB4.rotation);
        }

        if (D_800D7761 != 1)
        {
            WorldEnv_PresetsSet(7, 4);
            D_800D7761 = 1;
        }

        if (Savegame_EventFlagGet(EventFlag_134))
        {
            D_800DAF78 = Q12(0.15f);
        }
    }
    else if (PLAYER_IN_MAP_CHUNK(1, -1))
    {
        if (D_800D7761 != 0)
        {
            WorldEnv_PresetsSet(2, 2);
            Game_TurnFlashlightOff();
            D_800D7761 = 0;
        }

        if (!Savegame_EventFlagGet(EventFlag_142))
        {
            D_800DAF78 = Q12_MULT_FLOAT_PRECISE(PLAYER_CHARA().position.vy, -0.2f) + Q12(0.25f);
        }
    }
    else
    {
        if (PLAYER_IN_MAP_CHUNK(2, 0))
        {
            if (Savegame_EventFlagGet(EventFlag_MapMark_OldTown_SchoolCircle))
            {
                Savegame_EventFlagSet(EventFlag_MapMark_OldTown_SchoolCheck);
            }

            Savegame_EventFlagSet(EventFlag_142);
        }

        if (Savegame_EventFlagGet(EventFlag_142))
        {
            D_800DAF78 -= Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.2f);
            if (D_800DAF78 < Q12(0.0f))
            {
                D_800DAF78 = Q12(0.0f);
                D_800DAFE4 = 0;
            }
        }
        else
        {
            D_800DAF78 = Q12(0.45f);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(3, 0))
    {
        WorldObjects_Add(&D_800DB024.object, &D_800DB024.position, &D_800DB024.rotation);
    }

    if (!D_800DAFE4)
    {
        if (!Savegame_EventFlagGet(EventFlag_142) && Savegame_EventFlagGet(EventFlag_143))
        {
            SD_Call(Sfx_Unk1480);
            D_800DAFE4 = 1;
        }

        if (!D_800DAFE4)
        {
            return;
        }
    }

    // Set sound position.
    soundPos.vx = PLAYER_CHARA().position.vx;
    soundPos.vy = PLAYER_CHARA().position.vy;
    soundPos.vz = PLAYER_CHARA().position.vz - Q12(16.0f);

    D_800DB055 = Vc_StereoBalanceGet(&soundPos);
    if (D_800DB054 < D_800DB055)
    {
        D_800DB054 = MIN((D_800DB054 + (g_DeltaTime >> 6)), 127);
    }

    if (D_800DB055 < D_800DB054)
    {
        D_800DB054 = CLAMP_LOW(D_800DB054 - (g_DeltaTime >> 6), -127);
    }

    vol = MAX(255 - (D_800DAF78 >> 4), 0);
    Sd_SfxAttributesUpdate(Sfx_Unk1480, D_800DB054, vol & 255, 0);
}

void func_800D6C88(void) // 0x800D6C88
{
    if (PLAYER_CHARA().position.vx == Q12(21.45f) &&
        PLAYER_CHARA().position.vz == Q12(-15.26f))
    {
        // Warp camera.
        Event_CameraPositionSet(NULL,
                                Q12(21.69f), Q12(-2.38f), Q12(-19.16f),
                                Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                true);
        Event_CameraLookAtSet(NULL,
                              Q12(21.08f), Q12(-0.45f), Q12(-15.71f),
                              Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                              true);

        vcExecCamera();
        vcReturnPreAutoCamWork(false);
    }
}
