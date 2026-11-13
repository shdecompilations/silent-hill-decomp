#include "inline_no_dmpsx.h"

#include <psyq/gtemac.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/events/event_data.h"
#include "bodyprog/world/effects.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player/player.h"
#include "bodyprog/sys/warm_boot.h"
#include "main/rng.h"
#include "maps/map1/map1_s02.h"
#include "maps/common/particle.h"
#include "maps/common/characters/creeper.h"
#include "maps/common/characters/player.h"
#include "maps/common/characters/stalker.h"

#include "../src/maps/common/particles/water.c"

#include "../src/maps/common/particles/particle.c"

#include "../src/maps/common/characters/player.c"

#include "../src/maps/common/characters/stalker.c" // 0x800D2948
#include "../src/maps/common/characters/creeper.c" // 0x800D7B98

#include "../src/maps/common/map_util.c" // 0x800D9E0C

#include "maps/common/shared/Map_RoomBgmInit_1_s02.h" // 0x800D9EBC

void GameBoot_LoadScreen_StageString(void) {}

#include "maps/common/shared/MapEvent_DoorJammed.h" // 0x800DA020

#include "maps/common/shared/MapEvent_DoorLocked.h" // 0x800DA0B4

#include "maps/common/shared/MapEvent_DoorUnlocked.h" // 0x800DA16C

const char* MAP_MESSAGES[] = {
    #include "maps/common/shared/map_msg_common.inc"
#if VERSION_REGION_IS(NTSC)
    /* 15 */ "~J0(1.2)\tWhere_am_I? ~E ",
    /* 16 */ "~J0(2.0)\tHave_I_been_here_before? ~E ",
    /* 17 */ "~J0(3.8)\tI_don't_remember ~N\n\t\t\tthis_being_here_before... ~E ",
    /* 18 */ "\tThe_tower_door_is_locked. ~E ",
    /* 19 */ "\t",
    /* 20 */ "\t",
    /* 21 */ "\t~C5 A_Golden_Sun ~C7 ~E ",
    /* 22 */ "\t~C5 A_Silver_Moon ~C7 ~E ",
    /* 23 */ "\tWhat? ~N\n\tIs_this_a_door? ",
    /* 24 */ "\tIf_it_is,_there's_no_keyhole, ~N\n\tor_even_a_knob... ",
    /* 25 */ "\tIf_I_push_it,_it_moves_a_bit, ~N\n\tbut_it_certainly_won't_open. ~E ",
    /* 26 */ "\tWhat's_this? ~E ",
    /* 27 */ "\tIn_the_center_of_the_door_is ~N\n\ta_horizontal_slot. ",
    /* 28 */ "\tWhat_is_it_for? ~E ",
    /* 29 */ "\tThere_is_a_ ~C2 Picture_card ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 30 */ "\tSlid_the_ ~C2 Picture_card ~C7 ~N\n\tinto_the_slot. ~E ",
    /* 31 */ "~J0(0.9)\t_ ",
    /* 32 */ "~J0(3.0)\tDaddy==_help_me=== ",
    /* 33 */ "~J0(3.0)\t==Daddy==where_are_you==? ",
    /* 34 */ "~D ~J0(2.0)\n\tCheryl! ~E ",
    /* 35 */ "~J1(2.4)\n\tThat_was_Cheryl's_voice. ~N\n\tI_know_it. ",
    /* 36 */ "~J1(2.0)\n\tWhere_is_she? ~N\n\tCheryl... ~E ",
    /* 37 */ "\tThere_is_a_ ~C2 Rubber_ball ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 38 */ "\t~C5 Leonard_Rhine ~N\n\tThe_Monster_Lurks ~C7 ",
    /* 39 */ "\tThe_monster_lurks... ~N\n\tWhat's_that_supposed_to_mean? ~E ",
    /* 40 */ "\tThere_is_a_ ~C2 Shotgun ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 41 */ "\t_ ~S3 ",
    /* 42 */ "\tTurn_the_valve_left. ",
    /* 43 */ "\tTurn_the_valve_right. ",
    /* 44 */ "\tStop_turning_the_valve. ",
    /* 45 */ "\tCards_are_scattered... ~E ",
    /* 46 */ "\tA_wheelchair? ~N\n\tAt_an_elementary_school? ~E ",
    /* 47 */ "\tLocker_is_locked. ~E ",
    /* 48 */ "\tA_huge_fan_is_rotating. ",
    /* 49 */ "\tIf_approached_carelessly, ~N\n\tI_may_be_sucked_in. ~E ",
    /* 50 */ "\tCan't_use_the_phone. ~E ",
    /* 51 */ "\tThere_is_a_ ~C2 Classroom_key ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 52 */ "\tA_hanging_body... ~N\n\tWhy_would_somebody... ~E ",
    /* 53 */ "\tA_doll_is_on_the_floor. ~E ",
    /* 54 */ "\tA_headless_doll_is_stuck ~N\n\tto_the_wall. ~E "
#elif VERSION_REGION_IS(NTSCJ)
    /* 15 */ "~J0(1.2)\t……ここは？ ~E ",
    /* 16 */ "~J0(2.0)\t元の場所？ ~E ",
    /* 17 */ "~J0(3.8)\tこれは何だ？ ~N\n\t\t\tさっきまではなかったはずだが…… ~E ",
    /* 18 */ "\t扉は開かない ~E ",
    /* 19 */ "\t~C2 ロビーの鍵 ~C7 を使った ~E ",
    /* 20 */ "\tつづく ~E ",
    /* 21 */ "\t~C5 黄金の時 ~C7 ~E ",
    /* 22 */ "\t~C5 白銀の月 ~C7 ~E ",
    /* 23 */ "\t何だ……？ ~N\n\tこれは扉なのか？ ",
    /* 24 */ "\t扉にしては鍵穴どころか ~N\n\tノブもないが…… ",
    /* 25 */ "\t押してみても　かすかには動くが ~N\n\tそれ以上は開きそうにない ~E ",
    /* 26 */ "\t……これは？ ~E ",
    /* 27 */ "\t扉の中央に横長のスリットがある ",
    /* 28 */ "\t何の意味があるのだろう？ ~E ",
    /* 29 */ "\t~C2 絵札 ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 30 */ "\t~C2 絵札 ~C7 をスリットに差し込んだ ~E ",
    /* 31 */ "~J0(0.9)\t　 ",
    /* 32 */ "~J0(3.0)\tパパ……　たすけて…… ",
    /* 33 */ "~J0(3.0)\tパパ……どこにいるの……？ ",
    /* 34 */ "~D ~J0(2.0)\n\tシェリル！ ~E ",
    /* 35 */ "~J1(2.4)\tシェリルの声だ　まちがいない ",
    /* 36 */ "~J1(2.0)\tどこにいるんだ？ ~N\n\t\t\tシェリル…… ~E ",
    /* 37 */ "\t~C2 ゴムボール ~C7 が置かれている ~N\n\t取りますか？ ~S4 ",
    /* 38 */ "\t~C5 レナード・ライン　潜在する怪物 ~C7 ",
    /* 39 */ "\t潜在する怪物…… ~N\n\t何のことだ？ ~E ",
    /* 40 */ "\t~C2 ショットガン ~C7 が落ちている ~N\n\t取りますか？ ~S4 ",
    /* 41 */ "\tバルブがある ~S3 ",
    /* 42 */ "\t左に回す ",
    /* 43 */ "\t右に回す ",
    /* 44 */ "\t回すのを止める ",
    /* 45 */ "\tカードが散らばっている ~E ",
    /* 46 */ "\t車椅子が置かれている ~N\n\t小学校に……？ ~E ",
    /* 47 */ "\tロッカーには鍵がかかっている ~E ",
    /* 48 */ "\t巨大なファンが回転している ~N\n\t無闇に近づくと引き込まれてしまいそうだ ~E ",
    /* 49 */ "\t電話機は使えない ~E ",
    /* 50 */ "\t~C2 教室の鍵 ~C7 が落ちている ~N\n\t取りますか？ ~S4 ",
    /* 51 */ "\t吊るされた死体だ…… ~N\n\tいったい誰が……！？ ~E ",
    /* 52 */ "\t人形が床に置かれている ~E ",
    /* 53 */ "\t首のない人形が壁に張りつけられている ~E "
#endif
};

void MapEvent_CommonItemTake(void) // 0x800DA200
{
    u32 pickupType;
    s32 eventFlagIdx;

    pickupType   = CommonPickupItemId_FirstAidKit;
    eventFlagIdx = 0;

    switch (g_MapEventData->mapPointIdx)
    {
        case 107:
            pickupType   = CommonPickupItemId_Ampoule;
            eventFlagIdx = EventFlag_M1S02_Ampoule0;
            break;

        case 108:
            pickupType   = CommonPickupItemId_ShotgunShells;
            eventFlagIdx = EventFlag_M1S02_ShotgunShells0;
            break;

        case 109:
            pickupType   = CommonPickupItemId_ShotgunShells;
            eventFlagIdx = EventFlag_M1S02_ShotgunShells1;
            break;

        case 110:
            pickupType   = CommonPickupItemId_FirstAidKit;
            eventFlagIdx = EventFlag_M1S02_FirstAidKit0;
            break;

        case 111:
            pickupType   = CommonPickupItemId_HandgunBullets;
            eventFlagIdx = EventFlag_M1S02_HandgunBullets0;
            break;

        case 112:
            pickupType   = CommonPickupItemId_HandgunBullets;
            eventFlagIdx = EventFlag_M1S02_HandgunBullets1;
            break;

        case 113:
            pickupType   = CommonPickupItemId_HandgunBullets;
            eventFlagIdx = EventFlag_M1S02_HandgunBullets2;
            break;

        case 114:
            pickupType   = CommonPickupItemId_Ampoule;
            eventFlagIdx = EventFlag_M1S02_Ampoule1;
            break;

        case 115:
            pickupType   = CommonPickupItemId_HealthDrink;
            eventFlagIdx = EventFlag_M1S02_HealthDrink0;
            break;

        case 116:
            pickupType   = CommonPickupItemId_FirstAidKit;
            eventFlagIdx = EventFlag_M1S02_FirstAidKit1;
            break;

        case 117:
            pickupType   = CommonPickupItemId_HealthDrink;
            eventFlagIdx = EventFlag_M1S02_HealthDrink1;
            break;
    }

    Event_CommonItemTake(pickupType, eventFlagIdx);
}

void MapEvent_LobbyKeyUse(void) // 0x800DA2E4
{
    VECTOR3 sfxPos = { MAP_POINTS[g_MapEventData->mapPointIdx].positionX, Q12(-1.2f), MAP_POINTS[g_MapEventData->mapPointIdx].positionZ };

    Player_ItemRemove(InvItemId_LobbyKey, 1);
    Event_DisplayMapMsgWithSfx(19, Sfx_DoorUnlocked, &sfxPos); // Empty message?
}

void func_800DA384(void) // 0x800DA384
{
    VECTOR3 viewPos;
    bool    isSkipped;

    isSkipped = false;

    if (Pad_ButtonClickedBind(skip) &&
        g_SysWork.sysStateSteps[0] > 0 && g_SysWork.sysStateSteps[0] < 10)
    {
        isSkipped = true;
        SysWork_StateStepReset();
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            // Warp camera.
            Event_CameraPositionSet(NULL,
                                    Q12(-19.96f), Q12(-1.4f), Q12(-61.45f),
                                    Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                    true);
            Event_CameraLookAtSet(NULL,
                                  Q12(-17.14f), Q12(-1.49f), Q12(-64.27f),
                                  Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                  true);
            break;

        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            break;

        case 7:
        case 8:
        case 9:
        case 10:
            if (D_800E1EE4 == 0)
            {
                // Warp camera.
                Event_CameraPositionSet(NULL,
                                        Q12(-21.2f), Q12(-2.28f), Q12(-59.23f),
                                        Q12(0.0f), Q12(0.0f),
                                        DIV_FAST(cam_mv_prm_user.max_spd_xz, 8), DIV_FAST(cam_mv_prm_user.max_spd_y, 8),
                                        false);
                Event_CameraLookAtSet(NULL,
                                      Q12(-13.1f), Q12(-1.2f), Q12(-68.3f),
                                      Q12(0.0f), Q12(0.0f),
                                      DIV_FAST(deflt_watch_mv_prm.max_ang_spd_x, 8), DIV_FAST(deflt_watch_mv_prm.max_ang_spd_y, 8),
                                      false);

                vwGetViewPosition(&viewPos);

                if (ABS(viewPos.vx - Q12(-21.2f)) < Q12(0.3f) && ABS(viewPos.vz - Q12(-59.23f)) < Q12(0.3f))
                {
                    D_800E1EE4 = 1;
                }
            }
            else
            {
                // Warp camera.
                Event_CameraPositionSet(NULL,
                                        Q12(-17.79f), Q12(-5.03f), Q12(-54.09f),
                                        Q12(0.0f), Q12(0.0f), DIV_FAST(cam_mv_prm_user.max_spd_xz, 8), DIV_FAST(cam_mv_prm_user.max_spd_y, 8),
                                        false);
                Event_CameraLookAtSet(NULL,
                                      Q12(-19.57f), Q12(0.0f), Q12(-61.21f),
                                      Q12(0.0f), Q12(0.0f), DIV_FAST(deflt_watch_mv_prm.max_ang_spd_x, 8), DIV_FAST(deflt_watch_mv_prm.max_ang_spd_y, 8),
                                      false);
            }
            break;

        default:
            Savegame_EventFlagSet(EventFlag_MapMark_AltSchool1F_CourtyardQuestion);
            break;
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Game_TurnFlashlightOn();

            Particle_EnvironmentSet(6, 127);
            Particle_SystemUpdate(0, g_SavegamePtr->mapIdx, 0);

            PLAYER_CHARA().position.vx = Q12(-13.3f);
            PLAYER_CHARA().position.vz = Q12(-68.1f);

            Event_PathWaypointSet(true, 0, 0, Q12_ANGLE(-45.0f), Q12(-18.18f), Q12(-62.24f));
            Savegame_EventFlagClear(EventFlag_92);

            D_800E1EE2 = 0;

            SysWork_StateStepIncrement(0);

        case 1:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 2, Q12(0.0f), false);
            break;

        case 2:
            Event_WaitTimer(Q12(1.5f), false);
            break;

        case 3:
            Event_WaitPlayerStop();
            break;

        case 4:
            Event_DisplayMapMsgWithAudio(15, &D_800E1EE2, &D_800E1EDC); // "Where am I?"
            break;

        case 5:
            Event_CharaAnimPlayToEnd(&PLAYER_CHARA(), 74);
            break;

        case 6:
            Event_DisplayMapMsgWithAudio(16, &D_800E1EE2, &D_800E1EDC); // "Have I been here before?"
            break;

        case 7:
            Savegame_EventFlagSet(EventFlag_92);
            Event_PathWaypointExecutePlayer(53, 1, false);
            break;

        case 8:
            Event_WaitTimer(Q12(2.2f), false);
            break;

        case 9:
            Event_DisplayMapMsgWithAudio(17, &D_800E1EE2, &D_800E1EDC); // "I don't remember this being here before..."
            break;

        case 10:
            Event_WaitTimer(Q12(1.0f), false);
            break;

        default:
            if (isSkipped)
            {
                // Warp player.
                PLAYER_CHARA().position.vx = Q12(-18.18f);
                PLAYER_CHARA().position.vy = Q12(0.0f);
                PLAYER_CHARA().position.vz = Q12(-62.24f);
                PLAYER_CHARA().rotation.vy = Q12_ANGLE(-45.0f);

                // Restore player control.
                Player_ControlUnfreeze(true);
                SysWork_StateSetNext(SysState_Gameplay);

                // Restore camera control.
                vcReturnPreAutoCamWork(true);
            }
            else
            {
                // Restore player control.
                Player_ControlUnfreeze(false);
                SysWork_StateSetNext(SysState_Gameplay);

                // Restore camera control.
                vcReturnPreAutoCamWork(false);
            }

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);

            // Sets `EventFlag_225` temporarily for function call and clears afterward?
            Savegame_EventFlagSet(EventFlag_225);
            func_8003A16C();
            Savegame_EventFlagClear(EventFlag_225);
            break;
    }
}

void func_800DA8F8(void) // 0x800DA8F8
{
    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            Event_DisplayMapMsg(false, 18, 0, 0, 0, false); // "The tower door is locked."
            break;

        default:
            // Restore player control.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;
    }
}

void func_800DA9D4(void) // 0x800DA9D4
{
    Event_DisplayMapMsgWithBg(FILE_TIM_GOLD2_TIM, Q12(3.0f), Q12(2.5f), 21);
}

void func_800DAA00(void) // 0x800DAA00
{
    Event_DisplayMapMsgWithBg(FILE_TIM_SILVER2_TIM, Q12(3.0f), Q12(2.5f), 22);
}

void MapEvent_GameTrialOver(void) // 0x800DAA2C
{
    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            SysWork_StateStepIncrement(0);

        case 1:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 1, Q12(1.0f), false);
            Event_BgTextureCmd(BgTextureCmd_QueueRead, FILE_TIM_CONTINUE_TIM, false);
            SysWork_StateStepIncrement(0);

        case 2:
            Event_ScreenFadeCmd(ScreenFadeCmd_Wait, true, 0, Q12(0.0f), false);
            break;

        case 3:
            Event_BgTextureCmd(BgTextureCmd_AwaitLoad, 0, false);
            break;

        case 4:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 1, Q12(1.0f), false);
            SysWork_StateStepIncrement(0);

        case 5:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);

            if (Pad_ButtonClicked(Pad_BindingGet(enter) | Pad_BindingGet(cancel)))
            {
                SysWork_StateStepIncrement(0);
            }

            Event_WaitTimer(Q12(4.0f), false);

            if (g_SysWork.sysStateSteps[0] == 7)
            {
                g_SysWork.sysStateSteps[0] = 6;
            }
            break;

        case 6:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(1.5f), false);
            break;

        case 7:
            Event_BgTextureCmd(BgTextureCmd_Auto, FILE_TIM_COMING2_TIM, false);
            break;

        case 8:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(1.5f), false);
            SysWork_StateStepIncrement(0);

        case 9:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);

            if (Pad_ButtonClicked(Pad_BindingGet(enter) | Pad_BindingGet(cancel)))
            {
                SysWork_StateStepIncrement(0);
            }

            Event_WaitTimer(Q12(4.0f), false);

            if (g_SysWork.sysStateSteps[0] == 11)
            {
                g_SysWork.sysStateSteps[0] = 10;
                break;
            }
            break;

        case 10:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(1.5f), false);
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            Game_WarmBoot();

            g_SysWork.bgmStatusFlags |= BgmStatusFlag_Pause;
            break;
    }
}

extern s16 D_800E1FD0;

void MapEvent_DoorWithHorizontalSlotInteract(void) // 0x800DAD2C
{
    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 0, Q12(0.0f), false);
            Event_BgTextureCmd(BgTextureCmd_QueueRead, FILE_TIM_RECEPDR1_TIM, false);

            D_800E1FD0 = 0;

            SysWork_StateStepIncrement(0);

        case 1:
            Event_BgTextureCmd(BgTextureCmd_AwaitLoad, 0, false);
            break;

        case 2:
            Event_ScreenFadeCmd(ScreenFadeCmd_Wait, true, 0, Q12(0.0f), false);
            break;

        case 3:
            Event_BgTextureCmd(BgTextureCmd_StoreVram, 0, false);
            Event_BgTextureCmd(BgTextureCmd_QueueReadSecondary, FILE_TIM_RECEPDR2_TIM, false);
            SysWork_StateStepIncrement(0);

        case 4:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.0f), false);
            break;

        case 5:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);

            if (Savegame_EventFlagGet(EventFlag_M1S02_SeenDoorWithHorizontalSlot))
            {
                Event_DisplayMapMsg(false, 25, false, false, 0, false); // "If I push it, it moves a bit."
            }
            else
            {
                Event_DisplayMapMsg(false, 23, false, false, 0, false); // "What? Is this a door?"
            }
            break;

        case 6:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_BgTextureCmd(BgTextureCmd_AwaitLoad, 0, false);
            break;

        case 7:
            if (Savegame_EventFlagGet(EventFlag_M1S02_SeenDoorWithHorizontalSlot))
            {
                SysWork_StateStepIncrement(0);
            }
            else
            {
                Event_DisplayMapMsg(false, 26, false, false, 0, false); // "What's this?"
            }

        case 8:
            Screen_BackgroundImgTransition(&g_ItemInspectionImg, &D_800A9A04, D_800E1FD0);

            D_800E1FD0 += Q12(0.0313f);
            if (D_800E1FD0 > Q12(1.0f))
            {
                if (g_SysWork.sysStateSteps[0] == 8)
                {
                    SysWork_StateStepSet(0, 9);
                    break;
                }

                D_800E1FD0 = Q12(1.0f);
                break;
            }
            break;

        case 9:
            Event_BgTextureCmd(BgTextureCmd_DrawSecondary, 0, false);
            Event_DisplayMapMsg(false, 27, false, false, 0, false); // "In the center of the door is a horizontal slot."
            break;

        case 10:
            Event_BgTextureCmd(BgTextureCmd_DrawSecondary, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, 1, 0, Q12(0.0f), false);
            break;

        default:
            Event_BgTextureCmd(BgTextureCmd_RestoreVram, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);

            Savegame_EventFlagSet(EventFlag_M1S02_SeenDoorWithHorizontalSlot);

            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;
        }
}

void func_800DB058(void) // 0x800DB058
{
    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            // Freeze player.
            Player_ControlFreeze();
            Player_MoveSpeedClear();

            CutsceneBorder_ForceShow();

            // Warp player.
            PLAYER_CHARA().position.vx = Q12(56.5f);
            PLAYER_CHARA().position.vz = Q12(19.3f);
            PLAYER_CHARA().rotation.vy = Q12(-0.25f);

            // Warp camera.
            Event_CameraPositionSet(NULL,
                                    Q12(60.59f), Q12(-0.83f), Q12(18.34f),
                                    Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                    true);
            Event_CameraLookAtSet(NULL,
                                  Q12(56.7698f), Q12(-1.45f), Q12(19.34f),
                                  Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                  true);

            func_8003D03C();
            Player_EmptyWeaponHandSet();
            SysWork_StateStepIncrement(0);

        case 1:
            Event_ScreenFadeCmd(ScreenFadeCmd_Wait, false, 0, Q12(0.0f), false);
            break;

        case 2:
            Event_WaitTimer(Q12(0.2f), false);
            break;

        case 3:
            Event_CharaAnimPlayToEnd(&PLAYER_CHARA(), 94);
            break;

        case 4:
            Sfx_WithFlagsPlay(Sfx_Unk1454, &Q12_VECTOR3(55.85f, -1.1f, 19.3f), Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 5:
            Event_DisplayMapMsg(false, 30, 0, 0, 0, false); // "Slid the Picture card into the slot."
            break;

        case 6:
            Sfx_WithFlagsPlay(Sfx_DoorPuzzleUnlocked, &Q12_VECTOR3(56.0f, -1.2f, 19.3f), Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 7:
            Event_CharaAnimPlayToEnd(&PLAYER_CHARA(), 95);
            break;

        case 8:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        default:
            // Restore player control.
            Player_ControlUnfreeze(true);
            SysWork_StateSetNext(SysState_Gameplay);

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            vcReturnPreAutoCamWork(true);

            Savegame_EventFlagSet(EventFlag_MapMark_AltSchool1F_SecretDoorArrows);

            func_8003D01C();
            Player_WeaponAttackRestore();
            break;
    }
}

void func_800DB310(void) // 0x800DB310
{
    Event_ItemTake(InvItemId_PictureCard, DEFAULT_PICKUP_ITEM_COUNT, EventFlag_M1S02_PickupPictureCard, 29);
}

void func_800DB33C(void) // 0x800DB33C
{
    Event_ItemTake(InvItemId_RubberBall, DEFAULT_PICKUP_ITEM_COUNT, EventFlag_M1S02_PickupRubberBall, 37);
}

void MapEvent_CutscenePhoneCall(void) // 0x800DB368
{
    if (Pad_ButtonClickedBind(skip) &&
        g_SysWork.sysStateSteps[0] >= 2 && g_SysWork.sysStateSteps[0] < 26)
    {
        if (g_SysWork.sysStateSteps[0] < 5)
        {
            D_800E1FD8 = true;
        }

        SysWork_StateStepSet(0, 27);
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();

            Savegame_EventFlagSet(EventFlag_122);
            SD_Call(Sfx_XaAudio53);
            Event_PathWaypointSet(true, 0, 0, Q12_ANGLE(-90.0f), Q12(20.3f), Q12(143.5f));

            Event_TweenReset(0);
            Event_TweenReset(3);

            D_800E1FD8 = false;
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            Event_WaitTimer(Q12(1.5f), false);
            break;

        case 3:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 2, Q12(0.0f), false);
            break;

        case 4:
            Event_PathWaypointExecutePlayer(53, 1, false);
            Event_WaitTimer(Q12(2.0f), false);
            break;

        case 5:
            func_8003D03C();
            Player_EmptyWeaponHandSet();
            Game_TurnFlashlightOn();

            // Warp camera.
            Event_CameraPositionSet(NULL,
                                    Q12(18.06f), Q12(-1.99f), Q12(142.77f),
                                    Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                    true);
            Event_CameraLookAtSet(NULL,
                                  Q12(20.95f), Q12(0.27f), Q12(144.37f),
                                  Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                  true);

            SysWork_StateStepIncrement(0);

        case 6:
            Event_PathWaypointExecutePlayer(53, 1, false);
            break;

        case 7:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &PLAYER_CHARA(), 97, false);
            SysWork_StateStepIncrement(0);

        case 8:
            Event_WaitTimer(Q12(0.5f), false);
            break;

        case 9:
            // Warp camera.
            Event_CameraPositionSet(NULL,
                                    PLAYER_CHARA().position.vx + Q12(4.0f), Q12(-1.5f), PLAYER_CHARA().position.vz,
                                    Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                    true);
            Event_CameraLookAtSet(NULL,
                                  PLAYER_CHARA().position.vx, Q12(-1.0f), PLAYER_CHARA().position.vz,
                                  Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                  true);

            SysWork_StateStepIncrement(0);

        case 10:
            Event_WaitTimer(Q12(3.5f), false);
            break;

        case 11:
            SD_Call(Sfx_Unk1456);
            SD_Call(19);

            D_800E1FD4 = WorldGfx_PlayerHeldItemSet(InvItemId_CutscenePhone);
            SysWork_StateStepIncrement(0);

        case 12:
            if (Fs_QueueIsEntryLoaded(D_800E1FD4))
            {
                func_8003D01C();
                Sd_XaPreLoadAudioPreTaskAdd(Sfx_XaAudio54);
                SysWork_StateStepIncrement(0);
            }
            break;

        case 13:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AwaitAnimEnd, &PLAYER_CHARA(), 0, false);
            Sd_XaPreLoadAudioPreTaskAdd(Sfx_XaAudio54);
            break;

        case 14:
            SD_Call(Sfx_XaAudio54);
            SysWork_StateStepIncrement(0);

        case 15:
            Event_DisplayMapMsg(false, 31, 0, 0, 0, false); // Pause.

            // Warp camera.
            Event_CameraPositionSet(NULL,
                                    PLAYER_CHARA().position.vx + Q12(4.0f) + Event_TweenLinear(Q12(-2.8f), Q12(6.5f), 0), Q12(-1.5f), PLAYER_CHARA().position.vz,
                                    Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                    false);
            Event_CameraLookAtSet(NULL,
                                  PLAYER_CHARA().position.vx + Event_TweenLinear(Q12(-2.8f), Q12(6.5f), 3), Q12(-1.0f), PLAYER_CHARA().position.vz,
                                  Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                  false);
            break;

        case 16:
            // Warp camera.
            Event_CameraPositionSet(NULL,
                                    Q12(17.51f), Q12(-3.18f), Q12(144.05f),
                                    Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                    true);
            Event_CameraLookAtSet(NULL,
                                  Q12(20.28f), Q12(-0.42f), Q12(143.22f),
                                  Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                  true);

            SysWork_StateStepIncrement(0);

        case 17:
            Event_WaitTimer(Q12(1.8f), false);
            break;

        case 18:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &PLAYER_CHARA(), 98, false);
            SysWork_StateStepIncrement(0);

        case 19:
            Event_WaitTimer(Q12(1.0f), false);
            break;

        case 20:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimLock, &PLAYER_CHARA(), 0, false);
            Event_DisplayMapMsg(false, 35, 0, 0, 0, false); // "That was Cheryl's voice. I know it."
            break;

        case 21:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimUnlock, &PLAYER_CHARA(), 0, false);
            SysWork_StateStepIncrement(0);

        case 22:
            Event_WaitTimer(Q12(1.5f), false);
            break;

        case 23:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 24:
            Player_WeaponAttackRestore();
            SD_Call(19);

            D_800E1FD4 = WorldGfx_PlayerPrevHeldItem(&PLAYER_COMBAT());
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);

            SD_Call(Sfx_Unk1457);
            SysWork_StateStepIncrement(0);

        case 25:
            Event_WaitTimer(Q12(1.0f), false);
            break;

        case 26:
            if (Fs_QueueIsEntryLoaded(D_800E1FD4))
            {
                SysWork_StateStepReset();
            }
            break;

        case 27:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 28:
            SD_Call(19);
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimUnlock, &PLAYER_CHARA(), 0, false);

            if (!D_800E1FD8)
            {
                Player_WeaponAttackRestore();
            }

            WorldGfx_PlayerPrevHeldItem(&PLAYER_COMBAT());
            func_8003D01C();

            // Warp camera.
            Event_CameraPositionSet(NULL,
                                    Q12(17.51f), Q12(-3.18f), Q12(144.05f),
                                    Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                    true);
            Event_CameraLookAtSet(NULL,
                                  Q12(20.28f), Q12(-0.42f), Q12(143.22f),
                                  Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                  true);

            // Warp player.
            PLAYER_CHARA().position.vx = Q12(20.3f);
            PLAYER_CHARA().position.vz = Q12(143.5f);
            PLAYER_CHARA().rotation.vy = Q12_ANGLE(-90.0f);

            Fs_QueueWaitForEmpty();
            CutsceneBorder_Reset();
            SysWork_StateStepIncrement(0);
            break;

        default:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);

            Savegame_EventFlagSet(EventFlag_98);
            Savegame_EventFlagClear(EventFlag_99);

            vcReturnPreAutoCamWork(true);
            break;
    }
}

void func_800DBB7C(void) // 0x800DBB7C
{
    q3_12 angleAdd;

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Savegame_EventFlagSet(EventFlag_121);
            SD_Call(Sfx_XaAudio56);
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            Model_AnimFlagsClear(&PLAYER_CHARA().model, 2);

            LensFlare_LensFlareDisable();

            Event_TweenReset(0);
            Event_TweenReset(1);

            // Warp camera.
            Event_CameraPositionSet(NULL,
                                    Q12(21.0f), Q12(-0.5f), Q12(60.0f),
                                    Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                    true);
            Event_CameraLookAtSet(NULL,
                                  Q12(17.0f), Q12(-0.5f), Q12(60.0f),
                                  Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                  true);

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 2, Q12(0.0f), false);
            SysWork_StateStepIncrement(0);
            break;

        case 3:
            // Warp camera.
            if (D_800E1EE8 > Q12(1.0f))
            {
                Event_CameraPositionSet(NULL,
                                        Event_TweenLinear(Q12(-1.5f), Q12(5.0f), 0) + Q12(21.0f), Q12(-0.5f), Q12(60.0f),
                                        Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                        false);
                Event_CameraLookAtSet(NULL,
                                      Event_TweenLinear(Q12(-1.5f), Q12(5.0f), 0) + Q12(17.0f),
                                      Event_TweenLinear(Q12(-1.5f), Q12(5.0f), 1) - Q12(0.5f),
                                      Q12(60.0f),
                                      Q12(3.0f), Q12(0.0f), Q12(0.3f), Q12(0.0f),
                                      false);
            }

            D_800E1EE8 += g_DeltaTime;
            if (D_800E1EE8 < Q12(0.3f))
            {
                angleAdd = Q12_ANGLE(0.0f);
            }
            else if (D_800E1EE8 < Q12(0.5f))
            {
                angleAdd = Q12_ANGLE(-45.0f);
            }
            else if (D_800E1EE8 < Q12(1.0f))
            {
                angleAdd = Q12_ANGLE(5.0f);
            }
            else if (D_800E1EE8 < Q12(8.5f))
            {
                if (D_800E1EE8 - Q12(1.0f) <= Q12(6.0f))
                {
                    angleAdd = Q12_MULT_PRECISE(D_800E1EE8 - Q12(1.0f), Q12_ANGLE(5.0f)) - Q12_ANGLE(35.0f);
                }
                else
                {
                    angleAdd = Q12_ANGLE(-5.5f);
                }
            }
            else
            {
                angleAdd = Q12_ANGLE(0.0f);
                SysWork_StateStepIncrement(0);
            }

            g_WorldObject_DoorHid.rotation.vy += Q12_MULT_PRECISE(g_DeltaTime, angleAdd);
            if (g_WorldObject_DoorHid.rotation.vy < Q12_ANGLE(-112.5f))
            {
                g_WorldObject_DoorHid.rotation.vy = Q12_ANGLE(-112.5f);
            }

            break;

        default:
            // Restore player control.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);

            Model_AnimFlagsSet(&PLAYER_CHARA().model, 2);
            LensFlare_LensFlareEnable();

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            vcReturnPreAutoCamWork(true);

            Savegame_EventFlagSet(EventFlag_96);
            break;
    }
}

void func_800DBF88(void) // 0x800DBF88
{
    Savegame_EventFlagSet(EventFlag_97);
    Event_DisplayMapMsgWithBg(FILE_TIM_LVTRYMSG_TIM, Q12(0.0f), Q12(0.0f), 38);
}

void func_800DBFC8(void) // 0x800DBFC8
{
    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_InvItemCmd(InvItemCmd_QueueLoad, InvItemId_Shotgun, 0, false);
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &PLAYER_CHARA(), 59, false);
            SysWork_StateStepIncrement(0);

        case 3:
            Event_InvItemCmd(InvItemCmd_AwaitLoad, InvItemId_Shotgun, 0, false);
            break;

        case 4:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AwaitAnimEnd, &PLAYER_CHARA(), 0, false);
            break;

        case 5:
            if (Gfx_PickupItemAnimate(InvItemId_Shotgun))
            {
                Event_DisplayMapMsg(true, 40, 6, 7, 0, false);
            }

            Savegame_EventFlagSet(EventFlag_M1S02_PickupShotgun);
            break;

        case 6:
            Event_InvItemCmd(InvItemCmd_AddItem, InvItemId_Shotgun, SHOTGUN_AMMO_PICKUP_ITEM_COUNT, false);
            SysWork_StateStepSet(0, 8);
            break;

        case 7:
            Savegame_EventFlagClear(EventFlag_M1S02_PickupShotgun);
            SysWork_StateStepIncrement(0);

        case 8:
            Event_CharaAnimPlayToEnd(&PLAYER_CHARA(), 60);
            break;

        default:
            // Restore player control.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;
    }
}

const VECTOR3 D_800CB934 = { Q12(60.5f), Q12(-0.7f), Q12(-61.0f) };

const VECTOR3 D_800CB940 = { Q12(60.5f), Q12(-0.7f), Q12(-59.0f) };

const VECTOR3 D_800CB94C = { Q12(58.4f), Q12(-1.0f), Q12(-60.0f) };

void func_800DC1E0(void) // 0x800DC1E0
{
    q19_12 rotY0;
    q19_12 rotY1;

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            D_800E1FE2 = 0;
            SysWork_StateStepIncrement(0);

        case 1:
            Event_DisplayMapMsg(2, 41, 2, 4, NO_VALUE, false);

            D_800E1FE3 = 0;
            D_800E1FE0 = 0;
            D_800E1FDC.vy = 0;
            D_800E1FDC.vx = 0;
            break;

        case 2:
            D_800E1FE3 = 1;

        case 4:
            if (D_800E1FE2 == 0)
            {
                // Warp camera.
                Event_CameraPositionSet(NULL,
                                        Q12(61.0f), Q12(-1.2f), Q12(-60.0f),
                                        Q12(2.5f), Q12(0.75f), Q12(1.5f), Q12(0.5f),
                                        false);

                Event_WaitTimer(Q12(2.0f), false);

                if (g_SysWork.sysStateSteps[0] != 2 && g_SysWork.sysStateSteps[0] != 4)
                {
                    SysWork_StateStepSet(0, 5);
                    D_800E1FE2++;
                }
            }
            else
            {
                SysWork_StateStepSet(0, 5);
            }
            break;

        case 5:
            if (Savegame_EventFlagGet(EventFlag_118))
            {
                Sfx_WithFlagsPlay(Sfx_Unk1452, &D_800CB934, Q8(0.5f), SfxFlag_None);
            }
            else
            {
                Sfx_WithFlagsPlay(Sfx_Unk1452, &D_800CB940, Q8(0.5f), SfxFlag_None);
            }

            SysWork_StateStepIncrement(0);

        case 6:
            Event_WaitTimer(Q12(1.0f), false);
            break;

        case 7:
            if (D_800E1FDC.vx == 0)
            {
                SD_Call(Sfx_Unk1453);
            }

            if (D_800E1FDC.vx < Q12(0.25f))
            {
                D_800E1FE0 += Q12_MULT_PRECISE(g_DeltaTime, Q12(0.1666f));
            }
            else
            {
                D_800E1FE0 -= Q12_MULT_PRECISE(g_DeltaTime, Q12(0.1666f));
                D_800E1FE0 = MAX(D_800E1FE0, 170); // TODO: Float.
            }

            if (D_800E1FE3 != 0)
            {
                if (Savegame_EventFlagGet(EventFlag_118))
                {
                    rotY0 = -Q12_MULT_PRECISE(g_DeltaTime, MIN(D_800E1FE0, Q12_ANGLE(60.0f)));
                    if ((D_800E1FDC.vx - rotY0) > Q12_ANGLE(180.0f))
                    {
                        rotY0 = D_800E1FDC.vx - Q12_ANGLE(180.0f);
                        D_800E1FDC.vx = Q12_ANGLE(180.0f);
                    }
                    else
                    {
                        D_800E1FDC.vx -= rotY0;
                    }

                    rotY1 = -Q12_MULT_PRECISE(g_DeltaTime, MIN(D_800E1FE0, Q12_ANGLE(60.0f))) >> 1;
                    if ((D_800E1FDC.vy - rotY1) > Q12_ANGLE(90.0f))
                    {
                        rotY1 = D_800E1FDC.vy - Q12_ANGLE(90.0f);
                        D_800E1FDC.vy = Q12_ANGLE(90.0f);
                    }
                    else
                    {
                        D_800E1FDC.vy -= rotY1;
                    }
                }
                else
                {
                    rotY1 = -Q12_MULT_PRECISE(g_DeltaTime, MIN(D_800E1FE0, Q12_ANGLE(60.0f)));
                    if ((D_800E1FDC.vx - rotY1) > Q12_ANGLE(180.0f))
                    {
                        rotY1 = D_800E1FDC.vx - Q12_ANGLE(180.0f);
                        D_800E1FDC.vx = Q12_ANGLE(180.0f);
                    }
                    else
                    {
                        D_800E1FDC.vx -= rotY1;
                    }

                    rotY0 = -Q12_MULT_PRECISE(g_DeltaTime, MIN(D_800E1FE0, Q12_ANGLE(60.0f))) >> 1;
                    if ((D_800E1FDC.vy - rotY0) > Q12_ANGLE(90.0f))
                    {
                        rotY0 = D_800E1FDC.vy - Q12_ANGLE(90.0f);
                        D_800E1FDC.vy = Q12_ANGLE(90.0f);
                    }
                    else
                    {
                        D_800E1FDC.vy -= rotY0;
                    }
                }
            }
            else if (Savegame_EventFlagGet(EventFlag_118))
            {
                rotY0 = Q12_MULT_PRECISE(g_DeltaTime, MIN(D_800E1FE0, Q12_ANGLE(60.0f)));
                if ((D_800E1FDC.vx + rotY0) > Q12_ANGLE(180.0f))
                {
                    rotY0 = Q12_ANGLE(180.0f) - D_800E1FDC.vx;
                    D_800E1FDC.vx = Q12_ANGLE(180.0f);
                }
                else
                {
                    D_800E1FDC.vx += rotY0;
                }

                rotY1 = Q12_MULT_PRECISE(g_DeltaTime, MIN(D_800E1FE0, Q12_ANGLE(60.0f))) >> 1;
                if ((D_800E1FDC.vy + rotY1) > Q12_ANGLE(90.0f))
                {
                    rotY1 = Q12_ANGLE(90.0f) - D_800E1FDC.vy;
                    D_800E1FDC.vy = Q12_ANGLE(90.0f);
                }
                else
                {
                    D_800E1FDC.vy += rotY1;
                }
            }
            else
            {
                rotY1 = Q12_MULT_PRECISE(g_DeltaTime, MIN(D_800E1FE0, Q12_ANGLE(60.0f)));
                if ((D_800E1FDC.vx + rotY1) > Q12_ANGLE(180.0f))
                {
                    rotY1 = Q12_ANGLE(180.0f) - D_800E1FDC.vx;
                    D_800E1FDC.vx = Q12_ANGLE(180.0f);
                }
                else
                {
                    D_800E1FDC.vx += rotY1;
                }

                rotY0 = Q12_MULT_PRECISE(g_DeltaTime, MIN(D_800E1FE0, Q12_ANGLE(60.0f))) >> 1;
                if ((D_800E1FDC.vy + rotY0) > Q12_ANGLE(90.0f))
                {
                    rotY0 = Q12_ANGLE(90.0f) - D_800E1FDC.vy;
                    D_800E1FDC.vy = Q12_ANGLE(90.0f);
                }
                else
                {
                    D_800E1FDC.vy += rotY0;
                }
            }

            g_WorldObject_Door2Hi.rotation.vy += rotY0;
            g_WorldObject_Door1Hi.rotation.vy += rotY1;

            Sfx_WithFalloffAndPitchPlay(Sfx_Unk1453, &D_800CB94C, D_800E1FE0 >> 3, Q12(18.0f), 0);

            if (D_800E1FDC.vx == Q12_ANGLE(180.0f) && D_800E1FDC.vy == Q12_ANGLE(90.0f))
            {
                // TODO: What are these angles?
                g_WorldObject_Door2Hi.rotation.vy = (g_WorldObject_Door2Hi.rotation.vy + 0x1040) & 0xC00;
                g_WorldObject_Door1Hi.rotation.vy = (g_WorldObject_Door1Hi.rotation.vy + 0x1040) & 0xC00;

                Sd_SfxStop(Sfx_Unk1453);

                SysWork_StateStepSet(0, 1);
            }
            break;

        default:
            // Restore player control.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);

            Savegame_EventFlagClear(EventFlag_119);
            Savegame_EventFlagClear(EventFlag_118);

            if (D_800E1FE2 != 0)
            {
                vcReturnPreAutoCamWork(false);
            }
            break;
    }
}

void func_800DCF00(void) // 0x800DCF00
{
    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 2, Q12(0.0f), false);

            D_800E1FE4 = 0;

            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            Event_WaitTimer(Q12(0.3f), false);
            break;

        case 3:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &PLAYER_CHARA(), 106, false);
            SysWork_StateStepIncrement(0);

        case 4:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 0, Q12(0.03f), false);
            SysWork_StateStepIncrement(0);

        case 5:
            Event_ScreenFadeCmd(ScreenFadeCmd_Wait, true, 0, Q12(0.0f), false);

            g_ScreenFadeTimestep += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.1f);
            g_ScreenFadeTimestep  = MIN(g_ScreenFadeTimestep, Q12(3.0f));
            break;

        default:
            // Restore player control.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);

            Savegame_EventFlagSet(EventFlag_120);
            Sd_SfxStop(Sfx_Unk1451);
            break;
    }

    if (g_SysWork.sysStateSteps[0] >= 2 && g_SysWork.sysStateSteps[0] < 6)
    {
        s32 var_t0;

        if (D_800E1FE4 == 0)
        {
            SD_Call(Sfx_Unk1451);
        }

        Sfx_WithFalloffAndPitchPlay(Sfx_Unk1451, &g_WorldObject_BoxHide.position, (D_800E1FE4 + Q12(0.1f)) >> 4, Q12(32.0f), 0);

        var_t0     = D_800E1FE4 + Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 1.0f);
        var_t0     = MIN(var_t0, Q12(0.4f));
        D_800E1FE4 = var_t0;

        PLAYER_CHARA().position.vy += Q12_MULT_PRECISE(g_DeltaTime, D_800E1FE4);
        g_WorldObject_BoxHide.position.vy              += Q12_MULT_PRECISE(g_DeltaTime, D_800E1FE4);
    }
}

void MapEvent_ClassroomKeyItemTake(void) // 0x800DD208
{
    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_InvItemCmd(InvItemCmd_QueueLoad, InvItemId_ClassroomKey, 0, false);
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &PLAYER_CHARA(), 59, false);
            SysWork_StateStepIncrement(0);

        case 3:
            Event_InvItemCmd(InvItemCmd_AwaitLoad, InvItemId_ClassroomKey, 0, false);
            break;

        case 4:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AwaitAnimEnd, &PLAYER_CHARA(), 0, false);
            break;

        case 5:
            if (Gfx_PickupItemAnimate(InvItemId_ClassroomKey))
            {
                Event_DisplayMapMsg(true, MAP_MSG(51), 6, 7, 0, false); // "There is a CLassroom key. Take it?"
            }

            Savegame_EventFlagSet(EventFlag_M1S02_PickupClassroomKey);
            break;

        case 6:
            Event_InvItemCmd(InvItemCmd_AddItem, InvItemId_ClassroomKey, 1, false);
            SysWork_StateStepSet(0, 8);
            break;

        case 7:
            Savegame_EventFlagClear(EventFlag_M1S02_PickupClassroomKey);
            SysWork_StateStepIncrement(0);

        case 8:
            Event_CharaAnimPlayToEnd(&PLAYER_CHARA(), 60);
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;
    }
}
void func_800DD420(void) // 0x800DD420
{
    Particle_EnvironmentSet(6, 127);
    Particle_SystemUpdate(0, g_SavegamePtr->mapIdx, 0);
    Savegame_EventFlagClear(EventFlag_225);
    SysWork_StateSetNext(SysState_Gameplay);
}

void Map_WorldObjectsInit(void) // 0x800DD494
{
    s32 idx;

    WorldObject_ModelNameSet(&g_WorldObjectC, "FAN_HIDE");

    Math_Vector3Set(&g_ObjPosC[0], Q12(100.0f), Q12(-1.6f), Q12(142.9f));
    Math_SetSVectorFast(&g_ObjRotC[0], 0, 0, 0);
    Math_Vector3Set(&g_ObjPosC[1], Q12(100.0f), Q12(-1.6f), Q12(145.25f));
    Math_SetSVectorFast(&g_ObjRotC[1], 0, 0, 0);
    Math_Vector3Set(&g_ObjPosC[1], Q12(100.0f), Q12(-1.6f), Q12(145.25f)); // Set again?

    WorldObject_Init(&g_WorldObject_DoorHid, "DOOR_HID", 18.37f, -0.8f, 59.51f, 0.0f, 0.0f, 0.0f);
    WorldObject_Init(&g_WorldObject_GunHide, "GUN_HIDE", 17.8643f, -0.04f, 59.5135f, 0.0f, -215.3f, 0.0f);
    WorldObject_Init(&g_WorldObject_Ring1Hi, "RING1_HI", 60.5f, -1.04f, -61.0f, 0.0f, 0.0f, 0.0f);
    WorldObject_Init(&g_WorldObject_Ring2Hi, "RING2_HI", 60.5f, -1.04f, -59.0f, 0.0f, 0.0f, 0.0f);
    WorldObject_Init(&g_WorldObject_Door2Hi, "DOOR2_HI", 58.4f, 0.0f, -60.4f, 0.0f, 0.0f, 0.0f);
    WorldObject_Init(&g_WorldObject_Door1Hi, "DOOR1_HI", 58.4f, 0.0f, -59.6f, 0.0f, 0.0f, 0.0f);
    WorldObject_PlacementInit(&g_WorldObject_BoxHide, "BOX_HIDE", 54.47f, 0.0f, -60.0f);
    WorldObject_Init(&g_WorldObject_KeyHide, "KEY_HIDE", 139.199f, -0.8753f, 99.3733f, 0.0f, 46.2f, 0.0f);
    WorldObject_Init(&g_WorldObject_Phone3H, "PHONE3_H", 18.974f, -0.9f, 143.515f, 0.0f, -78.3f, 0.0f);
    WorldObject_Init(&g_WorldObject_Phone2H, "PHONE2_H", 18.974f, -0.9f, 143.515f, 0.0f, -78.3f, 0.0f);
    WorldObject_Init(&g_WorldObject_KeyHide1, "KEY_HIDE", -16.51f, -0.11f, -51.54f, 0.0f, -19.3f, 0.0f);
    WorldObject_PlacementInit(&g_WorldObject_BollHid, "BOLL_HID", 100.124f, -1.08f, 101.166f);
    WorldObject_Init(&g_WorldObjectB, g_CommonWorldObjectNames[1], 60.14f, -0.902f, 141.763f, 0.0f, 92.4f, 0.0f);

    if (g_SavegamePtr->gameDifficulty == GameDifficulty_Easy)
    {
        Chara_SpawnFlagsSet(Chara_GreyChildOrMumbler, 1, CharaSpawnFlag_None);
        Chara_SpawnFlagsSet(Chara_GreyChildOrMumbler, 3, CharaSpawnFlag_None);
        Chara_SpawnFlagsSet(Chara_GreyChildOrMumbler, 8, CharaSpawnFlag_None);
        Chara_SpawnFlagsSet(Chara_Creeper, 1, CharaSpawnFlag_None);
    }
    else if (g_SavegamePtr->gameDifficulty == GameDifficulty_Hard)
    {
        Chara_SpawnFlagsSet(Chara_GreyChildOrMumbler, 11, CharaSpawnFlag_0 | CharaSpawnFlag_1 | CharaSpawnFlag_3);
        Chara_SpawnFlagsSet(Chara_GreyChildOrMumbler, 13, CharaSpawnFlag_0 | CharaSpawnFlag_1 | CharaSpawnFlag_3);
        Chara_SpawnFlagsSet(Chara_GreyChildOrMumbler, 14, CharaSpawnFlag_0 | CharaSpawnFlag_1);
        Chara_SpawnFlagsSet(Chara_GreyChildOrMumbler, 15, CharaSpawnFlag_0 | CharaSpawnFlag_1);
        Chara_SpawnFlagsSet(Chara_Creeper, 0, CharaSpawnFlag_0 | CharaSpawnFlag_2 | CharaSpawnFlag_3);
        Chara_SpawnFlagsSet(Chara_Creeper, 2, CharaSpawnFlag_0 | CharaSpawnFlag_2 | CharaSpawnFlag_3);
        Chara_SpawnFlagsSet(Chara_Creeper, 4, CharaSpawnFlag_0 | CharaSpawnFlag_2 | CharaSpawnFlag_3);
        Chara_SpawnFlagsSet(Chara_Creeper, 6, CharaSpawnFlag_0 | CharaSpawnFlag_2 | CharaSpawnFlag_3);
        Chara_SpawnFlagsSet(Chara_Creeper, 9, CharaSpawnFlag_0 | CharaSpawnFlag_1);
    }

    sharedData_800E30C8_1_s02.field_0[0]  = Q12(-16.4f);
    sharedData_800E30C8_1_s02.field_10[0] = Q12(0.1f);
    sharedData_800E30C8_1_s02.field_18[0] = Q12(-51.2f);
    sharedData_800E30C8_1_s02.field_28[0] = Q12(1.6f);
    sharedData_800E30C8_1_s02.field_34[0] = Q12_ANGLE(90.0f);
    sharedData_800E30C8_1_s02.field_30[0] = 0;
    sharedData_800E30C8_1_s02.field_3C[0] = Q12(0.4f);
    sharedData_800E30C8_1_s02.field_44[0] = Q12(0.5f);
    sharedData_800E30C8_1_s02.field_4C[0] = Q12(0.05f);
    sharedData_800E30C8_1_s02.field_5C[0] = Q12(0.4f);
    sharedData_800E30C8_1_s02.field_64[0] = Q12(0.5f);
    sharedData_800E30C8_1_s02.field_6c[0] = Q12(0.05f);

    sharedData_800E30C8_1_s02.field_0[1]  = Q12(-14.8f);
    sharedData_800E30C8_1_s02.field_10[1] = Q12(0.1f);
    sharedData_800E30C8_1_s02.field_18[1] = Q12(-51.2f);
    sharedData_800E30C8_1_s02.field_34[1] = Q12_ANGLE(270.0f);
    sharedData_800E30C8_1_s02.field_28[1] = Q12(1.6f);
    sharedData_800E30C8_1_s02.field_30[1] = 0;
    sharedData_800E30C8_1_s02.field_3C[1] = Q12(0.4f);
    sharedData_800E30C8_1_s02.field_44[1] = Q12(0.5f);
    sharedData_800E30C8_1_s02.field_4C[1] = Q12(0.05f);
    sharedData_800E30C8_1_s02.field_5C[1] = Q12(0.4f);
    sharedData_800E30C8_1_s02.field_74 = 10;
    sharedData_800E30C8_1_s02.field_64[1] = Q12(0.5f);
    sharedData_800E30C8_1_s02.field_6c[1] = Q12(0.05f);
    sharedData_800E30C8_1_s02.field_78 = 2;

    idx = 2;
    sharedData_800E30C8_1_s02.field_28[idx] = Q12(0.0f);
    sharedData_800E30C8_1_s02.field_10[idx] = Q12(0.1f);
    sharedData_800E30C8_1_s02.field_3C[idx] = Q12(0.4f);
    sharedData_800E30C8_1_s02.field_5C[idx] = Q12(0.4f);
    sharedData_800E30C8_1_s02.field_64[idx] = Q12(0.5f);

    D_800E5A98 = 0;
    D_800E5A99 = 0;

    WorldObject_ModelNameSet(&g_CommonWorldObjects[0], g_CommonWorldObjectNames[2]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[1], g_CommonWorldObjectNames[3]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[2], g_CommonWorldObjectNames[4]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[3], g_CommonWorldObjectNames[5]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[4], g_CommonWorldObjectNames[6]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[5], g_CommonWorldObjectNames[7]);
}

void Map_WorldObjectsUpdate(void) // 0x800DDA84
{
    s32 rng0;
    s32 rng1;
    s32 triggerFlags;

    triggerFlags = CollisionTriggerFlag_None;

    if (PLAYER_IN_MAP_CHUNK(-1, -2))
    {
        if (!D_800E5A99 && Savegame_EventFlagGet(EventFlag_M1S03_KeyDownTheDrain))
        {
            sharedFunc_800CBC98_1_s02();
            D_800E5A99 = 1;
        }

        if (Savegame_EventFlagGet(EventFlag_M1S03_KeyDownTheDrain) && !Savegame_EventFlagGet(EventFlag_M1S02_PickupClassroomKey))
        {
            WorldObjects_Add(&g_WorldObject_KeyHide1.object, &g_WorldObject_KeyHide1.position, &g_WorldObject_KeyHide1.rotation);
        }
    }
    else
    {
        if (D_800E5A99)
        {
            sharedFunc_800CBD58_1_s02();
            D_800E5A99 = 0;
        }
    }

    if (PLAYER_IN_MAP_CHUNK(2, 3))
    {
        rng0 = Q12_ANGLE(-180.0f) - (Rng_Rand16() & 0x1FF);
        g_ObjRotC[0].vz += Q12_MULT_PRECISE(g_DeltaTime, rng0);

        WorldObjects_Add(&g_WorldObjectC, g_ObjPosC, (SVECTOR3*)g_ObjRotC);

        rng1 = Q12_ANGLE(-205.0f) - (Rng_Rand16() & 0x1FF);
        g_ObjRotC[1].vz += Q12_MULT_PRECISE(g_DeltaTime, rng1);

        WorldObjects_Add(&g_WorldObjectC, &g_ObjPosC[1], (SVECTOR3*)&g_ObjRotC[1]);

        if (!D_800E5A98)
        {
            SD_Call(Sfx_Unk1455);
            D_800E5A98 = 1;
        }

        Sfx_WithFalloffAndPitchPlay(Sfx_Unk1455, &g_ObjPosC[0], Q8(0.766f), Q12(16.0f), Q8(-0.25f));
    }
    else
    {
        if (D_800E5A98)
        {
            D_800E5A98 = 0;
            Sd_SfxStop(Sfx_Unk1455);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(2, 2))
    {
        if (!Savegame_EventFlagGet(EventFlag_M1S02_PickupRubberBall))
        {
            WorldObjects_Add(&g_WorldObject_BollHid.object, &g_WorldObject_BollHid.position, &SVECTOR3_Zero);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(0, 1))
    {
        if (!Savegame_EventFlagGet(EventFlag_M1S02_PickupShotgun))
        {
            WorldObjects_Add(&g_WorldObject_GunHide.object, &g_WorldObject_GunHide.position, &g_WorldObject_GunHide.rotation);
        }

        if (Savegame_EventFlagGet(EventFlag_96))
        {
            g_WorldObject_DoorHid.rotation.vy = Q12_ANGLE(-112.5f);
        }
        // Activate collisions for door blocking corpse in bathroom with shotgun.
        else
        {
            triggerFlags = CollisionTriggerFlag_Objects;
        }

        WorldObjects_Add(&g_WorldObject_DoorHid.object, &g_WorldObject_DoorHid.position, &g_WorldObject_DoorHid.rotation);
    }

    if (PLAYER_IN_MAP_CHUNK(1, -2))
    {
        WorldObjects_Add(&g_WorldObject_Ring1Hi.object, &g_WorldObject_Ring1Hi.position, &g_WorldObject_Ring1Hi.rotation);
        WorldObjects_Add(&g_WorldObject_Ring2Hi.object, &g_WorldObject_Ring2Hi.position, &g_WorldObject_Ring2Hi.rotation);
        WorldObjects_Add(&g_WorldObject_Door2Hi.object, &g_WorldObject_Door2Hi.position, &g_WorldObject_Door2Hi.rotation);
        WorldObjects_Add(&g_WorldObject_Door1Hi.object, &g_WorldObject_Door1Hi.position, &g_WorldObject_Door1Hi.rotation);
        WorldObjects_Add(&g_WorldObject_BoxHide.object, &g_WorldObject_BoxHide.position, &SVECTOR3_Zero);

        // Activate collisions for door locking boss entrance.
        if (g_WorldObject_Door2Hi.rotation.vy != Q12_ANGLE(180.0f) ||
            g_WorldObject_Door1Hi.rotation.vy != g_WorldObject_Door2Hi.rotation.vy)
        {
            triggerFlags |= CollisionTriggerFlag_Block;
            Savegame_EventFlagClear(EventFlag_362);
        }
        else
        {
            Savegame_EventFlagSet(EventFlag_362);
        }

        triggerFlags |= CollisionTriggerFlag_Objects;
    }
    else
    {
        if (Savegame_EventFlagGet(EventFlag_362))
        {
            g_WorldObject_Door1Hi.rotation.vy = Q12_ANGLE(180.0f);
        }
        else
        {
            g_WorldObject_Door1Hi.rotation.vy = Q12_ANGLE(90.0f);
        }

        g_WorldObject_Door2Hi.rotation.vy = Q12_ANGLE(180.0f);
    }

    if (PLAYER_IN_MAP_CHUNK(3, 2))
    {
        if (!Savegame_EventFlagGet(EventFlag_M1S02_PickupPictureCard))
        {
            WorldObjects_Add(&g_WorldObject_KeyHide.object, &g_WorldObject_KeyHide.position, &g_WorldObject_KeyHide.rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(0, 3))
    {
        if (!Savegame_EventFlagGet(EventFlag_99))
        {
            WorldObjects_Add(&g_WorldObject_Phone3H.object, &g_WorldObject_Phone3H.position, &g_WorldObject_Phone3H.rotation);
            WorldObjects_Add(&g_WorldObject_Phone2H.object, &g_WorldObject_Phone2H.position, &g_WorldObject_Phone2H.rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(1, 3))
    {
        WorldObjects_Add(&g_WorldObjectB.object, &g_WorldObjectB.position, &g_WorldObjectB.rotation);
    }

    Collision_TriggerFlagBitsClear(CollisionTriggerFlag_All);

    if (triggerFlags != CollisionTriggerFlag_None)
    {
        Collision_TriggerFlagBitsSet(triggerFlags);
    }

    if (PLAYER_IN_MAP_CHUNK(0, -2))
    {
        if (!Savegame_EventFlagGet(EventFlag_M1S02_Ampoule0))
        {
            WorldObjects_Add(&g_CommonWorldObjects[2], &g_CommonWorldObjectPoses->position, &g_CommonWorldObjectPoses->rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(0, -2))
    {
        if (!Savegame_EventFlagGet(EventFlag_M1S02_ShotgunShells0))
        {
            WorldObjects_Add(&g_CommonWorldObjects[5], &g_CommonWorldObjectPoses[1].position, &g_CommonWorldObjectPoses[1].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(0, -2))
    {
        if (!Savegame_EventFlagGet(EventFlag_M1S02_ShotgunShells1))
        {
            WorldObjects_Add(&g_CommonWorldObjects[5], &g_CommonWorldObjectPoses[2].position, &g_CommonWorldObjectPoses[2].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(2, 3))
    {
        if (!Savegame_EventFlagGet(EventFlag_M1S02_FirstAidKit0))
        {
            WorldObjects_Add(g_CommonWorldObjects, &g_CommonWorldObjectPoses[3].position, &g_CommonWorldObjectPoses[3].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(2, 3))
    {
        if (!Savegame_EventFlagGet(EventFlag_M1S02_HandgunBullets0))
        {
            WorldObjects_Add(&g_CommonWorldObjects[3], &g_CommonWorldObjectPoses[4].position, &g_CommonWorldObjectPoses[4].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(0, 3))
    {
        if (!Savegame_EventFlagGet(EventFlag_M1S02_HandgunBullets1))
        {
            WorldObjects_Add(&g_CommonWorldObjects[3], &g_CommonWorldObjectPoses[5].position, &g_CommonWorldObjectPoses[5].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(1, 0))
    {
        if (!Savegame_EventFlagGet(EventFlag_M1S02_HandgunBullets2))
        {
            WorldObjects_Add(&g_CommonWorldObjects[3], &g_CommonWorldObjectPoses[6].position, &g_CommonWorldObjectPoses[6].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(2, 0))
    {
        if (!Savegame_EventFlagGet(EventFlag_M1S02_Ampoule1))
        {
            WorldObjects_Add(&g_CommonWorldObjects[2], &g_CommonWorldObjectPoses[7].position, &g_CommonWorldObjectPoses[7].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(1, 3))
    {
        if (!Savegame_EventFlagGet(EventFlag_M1S02_HealthDrink0))
        {
            WorldObjects_Add(&g_CommonWorldObjects[1], &g_CommonWorldObjectPoses[8].position, &g_CommonWorldObjectPoses[8].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(1, 3))
    {
        if (!Savegame_EventFlagGet(EventFlag_M1S02_FirstAidKit1))
        {
            WorldObjects_Add(g_CommonWorldObjects, &g_CommonWorldObjectPoses[9].position, &g_CommonWorldObjectPoses[9].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(2, 1))
    {
        if (!Savegame_EventFlagGet(EventFlag_M1S02_HealthDrink1))
        {
            WorldObjects_Add(&g_CommonWorldObjects[1], &g_CommonWorldObjectPoses[10].position, &g_CommonWorldObjectPoses[10].rotation);
        }
    }
}

void func_800DEC88(void) // 0x800DEC88
{
    if (Savegame_EventFlagGet(EventFlag_MapMark_AltSchool1F_ReceptionArrows))
    {
        if (g_SavegamePtr->gameDifficulty == 1)
        {
            Chara_SpawnPositionSet(Chara_GreyChildOrMumbler, 12, Q12(134.1f), Q12(21.3f));
        }
        else
        {
            Chara_SpawnFlagsSet(Chara_GreyChildOrMumbler, 12, CharaSpawnFlag_None);
        }
    }

    if (Savegame_EventFlagGet(EventFlag_98))
    {
        Chara_SpawnFlagsSet(Chara_Creeper, 7, CharaSpawnFlag_2 | CharaSpawnFlag_3);
        Chara_SpawnFlagsSet(Chara_Creeper, 8, CharaSpawnFlag_2 | CharaSpawnFlag_3);
        if (g_SavegamePtr->gameDifficulty == 1)
        {
            Chara_SpawnFlagsSet(Chara_Creeper, 9, CharaSpawnFlag_2 | CharaSpawnFlag_3);
        }
    }
}
