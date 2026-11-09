#include "inline_no_dmpsx.h"

#include <psyq/gtemac.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/events/event_data.h"
#include "bodyprog/item_screens.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player/player.h"
#include "main/rng.h"
#include "maps/map1/map1_s00.h"
#include "maps/common/particle.h"
#include "maps/common/characters/larval_stalker.h"
#include "maps/common/characters/player.h"
#include "maps/common/characters/stalker.h"

#include "../src/maps/common/particles/particle.c"

#include "../src/maps/common/characters/player.c"

#include "../src/maps/common/characters/larval_stalker.c" // 0x800CF050
#include "../src/maps/common/characters/stalker.c" // 0x800D24F8

#include "../src/maps/common/map_util.c" // 0x800D7748

#define playerChara g_SysWork.playerWork.player

void Map_RoomBgmInit(bool arg0) // 0x800D77F8
{
    u16 bgmFlags;

    if (g_SavegamePtr->mapRoomIdx == 23)
    {
        bgmFlags = BgmFlag_Layer6;
        if (Savegame_EventFlagGet(EventFlag_71) && Savegame_EventFlagGet(EventFlag_72))
        {
            bgmFlags = BgmFlag_Layer2 |
                    BgmFlag_Layer3 |
                    BgmFlag_Layer4 |
                    BgmFlag_Layer5 |
                    BgmFlag_Layer6 |
                    BgmFlag_Layer7 |
                    BgmFlag_Layer8 |
                    BgmFlag_KeepAlive;
        }
    }
    else
    {
        bgmFlags = D_800DCC54[g_SavegamePtr->mapRoomIdx];
    }

    Bgm_LayersUpdate(bgmFlags, Q12(0.1f), &D_800DCC4C);
}

void GameBoot_LoadScreen_StageString(void) {}

#include "maps/common/shared/MapEvent_DoorJammed.h" // 0x800D786C

#include "maps/common/shared/MapEvent_DoorLocked.h" // 0x800D7900

#include "maps/common/shared/MapEvent_DoorUnlocked.h" // 0x800D7994

const char* MAP_MESSAGES[] = {
    #include "maps/common/shared/map_msg_common.h"
#if VERSION_REGION_IS(NTSC)
    /* 15 */ "\tThe_tower_door_is_locked. ~E ",
    /* 16 */ "\tThe_hands_are_stopped_at_10:00. ~E ",
    /* 17 */ "\tThe_hands_are_stopped_at_12:00. ~E ",
    /* 18 */ "\tThe_hands_are_stopped_at_5:00. ~E ",
    /* 19 */ "\t~C3 A_Golden_Sun ~E ",
    /* 20 */ "\t~C3 A_Silver_Moon ~E ",
    /* 21 */ "\tThe_door_won't_open. ~E ",
    /* 22 */ "~L4 ~M ~C3 10:00 ~N\n\t[Alchemy_laboratory] ~N\n\t~N\n\tGold_in_an_old_man's_palm. ~N\n\tThe_future_hidden_in_his_fist. ~N\n\tExchange_for_sage's_water. ~E ",
    /* 23 */ "~L4 ~M ~C3 12:00 ~N\n\t[A_place_with_songs_and_sound] ~N\n\t~N\n\tA_silver_guidepost_is ~N\n\tuntapped_in_lost_tongues. ~N\n\tAwakening_at_the_ordained_order. ~E ",
    /* 24 */ "~L4 ~M ~C3 5:00 ~N\n\t[Darkness_that_brings ~N\n\tthe_choking_heat] ~N\n\t~N\n\tFlames_render_the_silence, ~N\n\tawakening_the_hungry_beast. ~N\n\tOpen_time's_door_to_beckon_prey. ~E ",
    /* 25 */ "\tIt's_written_in_blood. ~E ",
    /* 26 */ "\tCheryl_might_be_here. ~N\n\tI_should_search_for_her. ~E ",
    /* 27 */ "\tA_picture_of_a_door. ",
    /* 28 */ "\tI_don't_know_who_drew_it, ~N\n\tbut_it_is_certainly_in_bad_taste. ~E ",
    /* 29 */ "\tThere_is_a_switch. ~N\n\tDo_you_want_to_press_the_switch? ~S4 ",
    /* 30 */ "\tNothing_appears_to_happen. ~E ",
    /* 31 */ "\tThe_boiler_is_working. ~E ",
    /* 32 */ "\tIt_appears_to_be_broken. ~E ",
    /* 33 */ "\tThe_valve_is_tightly_shut. ~N\n\tCan't_move_it. ",
    /* 34 */ "\tDon't_know_what_it_is_for, ~N\n\tbetter_leave_it_alone. ~E ",
    /* 35 */ "\tNothing_special_is_written. ~E ",
    /* 36 */ "\tNothing_useful ~N\n\tin_the_desk_drawer. ~E ",
    /* 37 */ "~H\tThere_is_a_ ~C5 School_map ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 38 */ "\tMoore,_Ranaldo,_Gordon... ~N\n\tThis_must_be_the_list_of_teachers. ~E ",
    /* 39 */ "\tNothing_inside. ~E "
#elif VERSION_REGION_IS(NTSCJ)
    /* 15 */ "\t時計塔の扉には鍵がかかっている ~E ",
    /* 16 */ "\t時計の針は１０時を ~N\n\t指したまま止まっている ~E ",
    /* 17 */ "\t時計の針は１２時を ~N\n\t指したまま止まっている ~E ",
    /* 18 */ "\t時計の針は５時を ~N\n\t指したまま止まっている ~E ",
    /* 19 */ "\t~C3 黄金の時 ~E ",
    /* 20 */ "\t~C3 白銀の月 ~E ",
    /* 21 */ "\t扉が開かなくなっている！ ~E ",
    /* 22 */ "~L4 ~M ~C3\n\t十時 ~N\n\t~N\n\t錬金術師の実験場 ~N\n\t~N\n\t老人の掌に黄金 ~N\n\t握り隠された未来 ~N\n\t賢者の水と引き替えに ~E ",
    /* 23 */ "~L4 ~M ~C3\n\t十二時 ~N\n\t~N\n\t歌と音の集う場所 ~N\n\t~N\n\t失われた言葉の中 ~N\n\t眠りにつく銀の道標 ~N\n\t定められた流れに目覚める ~E ",
    /* 24 */ "~L4 ~M ~C3\n\t五時 ~N\n\t~N\n\t暑い風を生む光届かぬ地 ~N\n\t~N\n\t静寂を破り灯される炎 ~N\n\t目覚める飢えたけもの ~N\n\t時の扉を開き餌を招く ~E ",
    /* 25 */ "\t血で文字が書かれている ~E ",
    /* 26 */ "\tシェリルがここにいるかもしれない ~N\n\t探してみた方がいいだろう ~E ",
    /* 27 */ "\t扉が描かれている ~N\n\t誰が飾ったかは知らないが悪趣味な絵だ ~E ",
    /* 28 */ "\tスイッチがある ~N\n\t押しますか？ ~S4 ",
    /* 29 */ "\t何も起こらないようだ ~E ",
    /* 30 */ "\tボイラーが稼動している ~E ",
    /* 31 */ "\t壊れていて動かないようだ ~E ",
    /* 32 */ "\tバルブは固く閉められていて ~N\n\t動かせそうにない ",
    /* 33 */ "\t何の配管かわからないし ~N\n\t下手にいじらない方がいいだろう ~E ",
    /* 34 */ "\t特に気になることは書かれていない ~E ",
    /* 35 */ "\t机の中に役に立ちそうな物は入ってない ~E ",
    /* 36 */ "~H\t~C5 学校の地図 ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 37 */ "\tムーア　ラナルド　ゴードン…… ~N\n\tどうやら　この学校の教員名簿のようだ ~E ",
    /* 38 */ "\t中には何もない ~E "
#endif
};

void MapEvent_CommonItemTake(void) // 0x800D7A28
{
    u32 pickupType;
    s32 eventFlagIdx;

    pickupType   = CommonPickupItemId_FirstAidKit;
    eventFlagIdx = 0;

    switch (g_MapEventData->mapPointIdx)
    {
        case 94:
            pickupType   = CommonPickupItemId_HandgunBullets;
            eventFlagIdx = EventFlag_M1S00_HandgunBullets0;
            break;

        case 95:
            pickupType   = CommonPickupItemId_HandgunBullets;
            eventFlagIdx = EventFlag_M1S00_HandgunBullets1;
            break;

        case 96:
            pickupType   = CommonPickupItemId_HandgunBullets;
            eventFlagIdx = EventFlag_M1S00_HandgunBullets2;
            break;

        case 97:
            pickupType   = CommonPickupItemId_HealthDrink;
            eventFlagIdx = EventFlag_M1S00_HealthDrink;
            break;

        case 98:
            pickupType   = CommonPickupItemId_FirstAidKit;
            eventFlagIdx = EventFlag_M1S00_FirstAidKit;
            break;
    }

    Event_CommonItemTake(pickupType, eventFlagIdx);
}

void MapEvent_GoldenSunEmptyReceptableInspect(void) // 0x800D7AC4
{
    g_DeltaTime = Q12(0.0f);
    Event_DisplayMapMsgWithBg(FILE_TIM_GOLD1_TIM, Q12(2.5f), Q12(2.0f), 19); // "A Golden Sun"
}

void MapEvent_GoldenSunFilledReceptacleInspect(void) // 0x800D7AF8
{
    g_DeltaTime = Q12(0.0f);
    Event_DisplayMapMsgWithBg(FILE_TIM_GOLD2_TIM, Q12(2.5f), Q12(2.0f), 19); // "A Golden Sun"
}

void MapEvent_GoldenSunItemUse(void)
{
    g_DeltaTime = Q12(0.0f);

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 0, Q12(2.5f), false);
            SysWork_StateStepIncrement(0);

        case 1:
            Event_BgTextureCmd(BgTextureCmd_Auto, FILE_TIM_GOLD1_TIM, false);
            break;

        case 2:
            Event_ScreenFadeCmd(ScreenFadeCmd_Wait, true, 0, Q12(0.0f), false);
            break;

        case 3:
            Event_BgTextureCmd(BgTextureCmd_StoreVram, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(2.0f), false);
            SysWork_StateStepIncrement(0);

        case 4:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_BgTextureCmd(BgTextureCmd_8, FILE_TIM_GOLD2_TIM, false);
            break;

        case 5:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Wait, false, 0, Q12(0.0f), false);
            break;

        case 6:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_WaitTimer(Q12(0.5f), false);
            break;

        case 7:
            SD_Call(Sfx_Unk1416);
            SysWork_StateStepIncrement(0);

        case 8:
            Screen_BackgroundImgTransition(&g_ItemInspectionImg, &D_800A9A04, g_SysWork.sysStateStepData[0]);

            g_SysWork.sysStateStepData[0] += Q12(1.0f / 16.0f);
            if (g_SysWork.sysStateStepData[0] > Q12(1.0f))
            {
                SysWork_StateStepIncrement(0);
            }
            break;

        case 9:
            SD_Call(Sfx_Unk1419);
            SysWork_StateStepIncrement(0);

        case 10:
            Event_BgTextureCmd(BgTextureCmd_DrawSecondary, 0, false);
            Event_WaitTimer(Q12(1.0f), false);
            break;

        case 11:
            Event_BgTextureCmd(BgTextureCmd_DrawSecondary, 0, false);
            Event_DisplayMapMsg(false, 19, 0, 0, 0, false); // "A Golden Sun"
            break;

        case 12:
            Event_BgTextureCmd(BgTextureCmd_DrawSecondary, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(2.0f), false);
            break;

        default:
            Event_BgTextureCmd(BgTextureCmd_RestoreVram, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(2.5f), false);

            Savegame_EventFlagSet(EventFlag_71);
            Savegame_EventFlagSet(EventFlag_MapMark_FogSchool1F_ClockTowerCircle);

            // Restore player control.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;
    }
}

void MapEvent_SilverMoonEmptyReceptableInspect(void) // 0x800D7E48
{
    g_DeltaTime = Q12(0.0f);
    Event_DisplayMapMsgWithBg(FILE_TIM_SILVER1_TIM, Q12(2.5f), Q12(2.0f), 20); // "A Silver Moon"
}

void MapEvent_SilverMoonFilledReceptacleInspect(void) // 0x800D7E7C
{
    g_DeltaTime = Q12(0.0f);
    Event_DisplayMapMsgWithBg(FILE_TIM_SILVER2_TIM, Q12(2.5f), Q12(2.0f), 20); // "A Silver Moon"
}

void MapEvent_SilverMoonItemUse(void)
{
    g_DeltaTime = Q12(0.0f);

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 0, Q12(2.5f), false);
            SysWork_StateStepIncrement(0);

        case 1:
            Event_BgTextureCmd(BgTextureCmd_Auto, FILE_TIM_SILVER1_TIM, false);
            break;

        case 2:
            Event_ScreenFadeCmd(ScreenFadeCmd_Wait, true, 0, Q12(0.0f), false);
            break;

        case 3:
            Event_BgTextureCmd(BgTextureCmd_StoreVram, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(2.0f), false);
            SysWork_StateStepIncrement(0);

        case 4:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_BgTextureCmd(BgTextureCmd_8, FILE_TIM_SILVER2_TIM, false);
            break;

        case 5:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Wait, false, 0, Q12(0.0f), false);
            break;

        case 6:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_WaitTimer(Q12(0.5f), false);
            break;

        case 7:
            SD_Call(Sfx_Unk1416);
            SysWork_StateStepIncrement(0);

        case 8:
            Screen_BackgroundImgTransition(&g_ItemInspectionImg, &D_800A9A04, g_SysWork.sysStateStepData[0]);

            g_SysWork.sysStateStepData[0] += Q12(1.0f / 16.0f);
            if (g_SysWork.sysStateStepData[0] > Q12(1.0f))
            {
                SysWork_StateStepIncrement(0);
            }
            break;

        case 9:
            SD_Call(Sfx_Unk1419);
            SysWork_StateStepIncrement(0);

        case 10:
            Event_BgTextureCmd(BgTextureCmd_DrawSecondary, 0, false);
            Event_WaitTimer(Q12(1.0f), false);
            break;

        case 11:
            Event_BgTextureCmd(BgTextureCmd_DrawSecondary, 0, false);
            Event_DisplayMapMsg(false, 20, 0, 0, 0, false); // "A silver moon."
            break;

        case 12:
            Event_BgTextureCmd(BgTextureCmd_DrawSecondary, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(2.0f), false);
            break;

        default:
            Event_BgTextureCmd(BgTextureCmd_RestoreVram, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(2.5f), false);

            Savegame_EventFlagSet(EventFlag_72);
            Savegame_EventFlagSet(EventFlag_MapMark_FogSchool1F_ClockTowerCircle);

            // Restore player control.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;
    }
}

void MapEvent_ClockTowerInspect(void) // 0x800D81CC
{
    g_DeltaTime = Q12(0.0f);

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitTimer(Q12(0.3f), false);
            break;

        case 2:
            Event_DisplayMapMsg(false, 15, false, false, 0, false); // "The tower door is locked."
            break;

        case 3:
            if (Savegame_EventFlagGet(EventFlag_72))
            {
                Event_BgTextureFadeIn(FILE_TIM_CLOCK5_TIM, Q12(3.0f), Q12(2.0f));
            }
            else
            {
                if (Savegame_EventFlagGet(EventFlag_71))
                {
                    Event_BgTextureFadeIn(FILE_TIM_CLOCK12_TIM, Q12(3.0f), Q12(2.0f));
                }
                else
                {
                    Event_BgTextureFadeIn(FILE_TIM_CLOCK_TIM, Q12(3.0f), Q12(2.0f));
                }
            }
            break;

        case 4:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);

            if (Savegame_EventFlagGet(EventFlag_72))
            {
                Event_DisplayMapMsg(false, 18, false, false, 0, false); // "Hands are stopped at 5:00."
            }
            else
            {
                if (Savegame_EventFlagGet(EventFlag_71))
                {
                    Event_DisplayMapMsg(false, 17, false, false, 0, false); // "Hands are stopped at 12:00."
                }
                else
                {
                    Event_DisplayMapMsg(false, 16, false, false, 0, false); // "Hands are stopped at 10:00."
                }
            }
            break;

        case 5:
            Event_BgTextureFadeOut(Q12(3.0f), Q12(2.0f));
            break;

        default:
            // Restore player control.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;
    }
}

void func_800D8354(void) // 0x800D8354
{
    VECTOR3 sfxPos = {
        MAP_POINTS[g_MapEventData->mapPointIdx].positionX,
        Q12(-1.2f),
        MAP_POINTS[g_MapEventData->mapPointIdx].positionZ
    };

    g_DeltaTime = Q12(0.0f);

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();

            Sfx_WithFlagsPlay(Sfx_DoorLocked, &sfxPos, Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            Event_DisplayMapMsg(false, 12, 0, 0, 0, false); // "It's locked."
            break;

        case 3:
            Event_BgTextureFadeIn(FILE_TIM_PAPER1_TIM, Q12(3.0f), Q12(2.0f));
            break;

        case 4:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);

            if (Pad_ButtonClicked(Pad_BindingGet(enter) | Pad_BindingGet(cancel)))
            {
                SysWork_StateStepIncrement(0);
                break;
            }
            break;

        case 5:
            Event_BgTextureFadeOut(Q12(3.0f), Q12(2.0f));
            break;

        default:
            // Restore player control.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;
    }
}

void func_800D853C(void) // 0x800D853C
{
    Event_DisplayMapMsgWithDimmedBg(FILE_TIM_PAPER1_TIM, Q12(2.5f), Q12(1.5f), 25, 22);
}

void func_800D8570(void) // 0x800D8570
{
    Event_DisplayMapMsgWithDimmedBg(FILE_TIM_PAPER2_TIM, Q12(2.5f), Q12(1.5f), 25, 23);
}

void func_800D85A4(void) // 0x800D85A4
{
    Event_DisplayMapMsgWithDimmedBg(FILE_TIM_PAPER3_TIM, Q12(2.5f), Q12(1.5f), 25, 24);
}

void func_800D85D8(void) // 0x800D85D8
{
    // Skip.
    if (Pad_ButtonClickedBind(skip) &&
        g_SysWork.sysStateSteps[0] >= 3 && g_SysWork.sysStateSteps[0] < 5)
    {
        Screen_FadeTimestepReset();
        SysWork_StateStepReset();
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            // Warp camera.
            Event_CameraPositionSet(NULL,
                                    Q12(-56.41f), Q12(-5.67f), Q12(-55.39f),
                                    Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                    true);
            Event_CameraLookAtSet(NULL,
                                  Q12(-56.4f), Q12(-1.8699f), Q12(-54.16f),
                                  Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                  true);

            // Warp player.
            playerChara.position.vy = Q12(-2.3f);
            playerChara.rotation.vy = Q12_ANGLE(0.0f);

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(1.5f), false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &playerChara, 88, false);
            SysWork_StateStepIncrement(0);

        case 3:
            Event_WaitTimer(Q12(3.5f), false);
            playerChara.position.vy += Q12_MULT_PRECISE(g_DeltaTime, Q12(0.3f));
            break;

        case 4:
            playerChara.position.vy += Q12_MULT_PRECISE(g_DeltaTime, Q12(0.3f));
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(1.5f), false);
            break;

        default:
            if (Savegame_EventFlagGet(EventFlag_78))
            {
                // Warp player.
                playerChara.position.vx = Q12(-60.0f);
                playerChara.position.vy = Q12(0.0f);
                playerChara.position.vz = Q12(-63.7f);
                playerChara.rotation.vy = Q12_ANGLE(0.0f);

                Savegame_EventFlagClear(EventFlag_184);
                Savegame_EventFlagClear(EventFlag_78);
            }
            else if (Savegame_EventFlagGet(EventFlag_81))
            {
                // Warp player.
                playerChara.position.vx = Q12(-60.0f);
                playerChara.position.vy = Q12(0.6f);
                playerChara.position.vz = Q12(-56.3f);
                playerChara.rotation.vy = Q12_ANGLE(180.0f);

                Savegame_EventFlagClear(EventFlag_185);
                Savegame_EventFlagClear(EventFlag_81);
            }

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(1.5f), false);

            // Restore player control.
            Player_ControlUnfreeze(true);
            SysWork_StateSetNext(SysState_Gameplay);

            vcReturnPreAutoCamWork(true);
            break;
    }
}

void func_800D8948(void) // 0x800D8948
{
    // Skip.
    if (Pad_ButtonClickedBind(skip) &&
        g_SysWork.sysStateSteps[0] >= 4 && g_SysWork.sysStateSteps[0] < 6)
    {
        Screen_FadeTimestepReset();
        SysWork_StateStepReset();
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();

            Savegame_EventFlagClear(EventFlag_82);
            Savegame_EventFlagClear(EventFlag_79);
            Savegame_EventFlagClear(EventFlag_80);

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 0, Q12(1.5f), false);
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            Event_ScreenFadeCmd(ScreenFadeCmd_Wait, true, 0, Q12(0.0f), false);
            break;

        case 3:
            // Warp camera.
            Event_CameraPositionSet(NULL,
                                    Q12(-56.74f), Q12(-1.7698f), Q12(-55.13f),
                                    Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                    true);
            Event_CameraLookAtSet(NULL,
                                  Q12(-55.43f), Q12(-5.5f), Q12(-54.56f),
                                  Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                  true);

            // Warp player.
            playerChara.position.vx = Q12(-56.34f);
            playerChara.position.vy = Q12(-2.276f);
            playerChara.position.vz = Q12(-55.1f);
            playerChara.rotation.vy = Q12_ANGLE(0.0f);

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(1.5f), false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &playerChara, 87, false);

            if (g_MapEventData->mapPointIdx == 16)
            {
                Savegame_EventFlagSet(EventFlag_79);
            }
            else if (g_MapEventData->mapPointIdx == 17)
            {
                Savegame_EventFlagSet(EventFlag_80);
            }

            SysWork_StateStepIncrement(0);

        case 4:
            Event_WaitTimer(Q12(3.5f), false);

            // Warp player.
            playerChara.rotation.vy = Q12_ANGLE(0.0f);
            playerChara.position.vy += Q12_MULT_PRECISE(g_DeltaTime, Q12(-0.3f));
            break;

        case 5:
            // Warp player.
            playerChara.position.vy += Q12_MULT_PRECISE(g_DeltaTime, Q12(-0.3f));

            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(1.5f), false);
            break;

        default:
            // Restore player control.
            Player_ControlUnfreeze(true);
            SysWork_StateSetNext(SysState_Gameplay);

            // Warp player.
            playerChara.position.vy = Q12(0.0f);
            break;
        }
}

void func_800D8CC4(void) // 0x800D8CC4
{
    Event_DisplayMapMsgWithBg(FILE_TIM_RECEPPIC_TIM, Q12(0.0f), Q12(0.0f), 27);
}

void MapEvent_Boiler0(void) // 0x800D8CF0
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
            g_SysWork.silentYesSelection = true;
            Event_DisplayMapMsg(true, MAP_MSG(29), 3, -1, 0, false); // "Do you want to press the switch?"
            break;

        case 3:
            Sfx_WithFlagsPlay(Sfx_Unk1420, &Q12_VECTOR3(20.5f, -1.2f, -17.5f), Q8(0.5f), SfxFlag_None);
            Savegame_EventFlagSet(EventFlag_83);
            SysWork_StateStepIncrement(0);

        case 4:
            Event_WaitTimer(Q12(1.0f), false);
            break;

        case 5:
            D_800DD718 += g_DeltaTime;
            if ((D_800DD718 % Q12(0.8499f)) < Q12(0.45f))
            {
                WorldObjects_Add(&g_WorldObject0.object, &g_WorldObject0.position, &SVECTOR3_Zero);
            }

            if (D_800DD718 > Q12(3.5f))
            {
                SysWork_StateStepIncrement(0);
            }
            break;

        case 6:
            Sfx_WithFlagsPlay(Sfx_Unk1422, &Q12_VECTOR3(18.8f, 4.0f, -17.5f), Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 7:
            Savegame_EventFlagSet(EventFlag_73);
            Event_WaitTimer(Q12(1.5f), false);
            break;

        case 8:
            SysWork_StateStepReset();
            Savegame_EventFlagSet(EventFlag_84);
            break;

        default:
            // Restore player control.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;
    }

    if (Savegame_EventFlagGet(EventFlag_73))
    {
        func_800894B8(112);
    }
    else if (Savegame_EventFlagGet(EventFlag_83))
    {
        func_800894B8(144);
    }
}

void MapEvent_Boiler1(void)
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
            g_SysWork.silentYesSelection = true;
            Event_DisplayMapMsg(true, MAP_MSG(29), 3, -1, 0, false); // "Do you want to press the switch?"
            break;

        case 3:
            Sfx_WithFlagsPlay(Sfx_Unk1420, &Q12_VECTOR3(20.5f, -1.2f, -17.5f), Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 4:
            Event_WaitTimer(Q12(0.5f), false);
            break;

        case 5:
            Event_DisplayMapMsg(false, MAP_MSG(30), 0, 0, 0, false); // "Nothing appears to happen."
            break;

        default:
            // Restore player control.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;
    }
}

void MapEvent_Boiler2(void) // 0x800D9148
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
            Event_DisplayMapMsg(false, MAP_MSG(31), 0, 0, 0, false); // "The boiler is working."
            break;

        default:
            // Restore player control.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;
    }

    func_800894B8(96);
}

void MapEvent_TownMapTake(void) // 0x800D922C
{
    Event_PaperMapTake(6, EventFlag_M1S00_PickupMap, MAP_MSG(37));
}

void func_800D9254(void) // 0x800D9254
{
    g_Screen_BackgroundImgGamma = Q8(13.0f / 32.0f);
    Event_DisplayMapMsgWithBg(FILE_TIM_USCBOOK_TIM, Q12(0.0f), Q12(0.0f), MAP_MSG(38));
}

void func_800D928C(void) // 0x800D928C
{
    Player_ControlFreeze();
    Player_ControlUnfreeze(false);
    SysWork_StateSetNext(SysState_Gameplay);
}

void Map_WorldObjectsInit(void)
{
    D_800E1180 = 0;
    D_800E1182 = 0;

    WorldObject_Init(&g_WorldObject9, "PICT00_H", 133.294f, -0.8614f, 19.04f, 0.0f, 162.6f, 0.0f);
    WorldObject_PlacementInit(&g_WorldObject1[0], "PAPER1_H", 133.63f, -0.816f, 17.089f);
    WorldObject_PlacementInit(&g_WorldObject1[1], "NOTE_HID", 133.991f, -0.8f, 16.307f);
    WorldObject_PlacementInit(&g_WorldObject1[2], "TINORI1_", 133.6f, -1.7f, 16.0f);
    WorldObject_PlacementInit(&g_WorldObject1[3], "TINORI2_", 133.6f, -1.19f, 18.2f);
    WorldObject_PlacementInit(&g_WorldObject1[4], "TINORI3_", 134.486f, -1.19f, 18.2f);
    WorldObject_PlacementInit(&g_WorldObject1[5], "GRID2_HI", 134.4f, -0.91f, 17.9568f);
    WorldObject_PlacementInit(&g_WorldObject2, "GOLD_HID", -12.2f, -1.0999f, -67.88f);
    WorldObject_PlacementInit(&g_WorldObject3, "SILVER_H", -13.5198f, -1.0999f, -69.0999f);
    WorldObject_PlacementInit(&g_WorldObject4, "ANA3_HID", -12.72f, -1.0f, -68.5999f);
    WorldObject_PlacementInit(&g_WorldObject5, "ANA2_HID", -12.7898f, -1.0f, -69.6199f);
    WorldObject_PlacementInit(&g_WorldObject6[0], "REDX_HID", 20.4568f, -0.8345f, -17.97f);
    WorldObject_PlacementInit(&g_WorldObject6[1], "R_HIDE_I", 19.5799f, 0.0f, -18.3f);
    WorldObject_PlacementInit(&g_WorldObject0, "G_HIDE_I", 19.5799f, 0.0f, -18.3f);
    WorldObject_Init(&g_WorldObject7, g_CommonWorldObjectNames[1], 61.8531f, -0.805f, 140.6099f, 0.0f, 92.9f, 0.0f);
    WorldObject_Init(&g_WorldObject8, "MAP_HIDE", 96.9405f, -0.3365f, 21.0918f, 0.0f, 66.8f, 0.0f);

    Player_ItemRemove(InvItemId_NoteToSchool, 1);
    Savegame_EventFlagSet(EventFlag_147);

    if (g_SavegamePtr->gameDifficulty == GameDifficulty_Easy)
    {
        Chara_SpawnFlagsSet(Chara_GreyChildOrMumbler, 3, CharaSpawnFlag_None);
        Chara_SpawnFlagsSet(Chara_GreyChildOrMumbler, 9, CharaSpawnFlag_None);
    }

    if (g_SavegamePtr->gameDifficulty == GameDifficulty_Hard)
    {
        Chara_SpawnFlagsSet(Chara_GreyChildOrMumbler, 10, CharaSpawnFlag_0 | CharaSpawnFlag_1 | CharaSpawnFlag_3);
        Chara_SpawnFlagsSet(Chara_GreyChildOrMumbler, 11, CharaSpawnFlag_0 | CharaSpawnFlag_1);
        Chara_SpawnFlagsSet(Chara_GreyChildOrMumbler, 12, CharaSpawnFlag_0 | CharaSpawnFlag_1);
        Chara_SpawnFlagsSet(Chara_GreyChildOrMumbler, 13, CharaSpawnFlag_0 | CharaSpawnFlag_1);
    }

    WorldObject_ModelNameSet(&g_CommonWorldObjects[0], g_CommonWorldObjectNames[2]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[1], g_CommonWorldObjectNames[3]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[2], g_CommonWorldObjectNames[4]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[3], g_CommonWorldObjectNames[5]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[4], g_CommonWorldObjectNames[6]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[5], g_CommonWorldObjectNames[7]);
}

void Map_WorldObjectsUpdate(void)
{
    s32 i;

    if (!Savegame_EventFlagGet(EventFlag_226))
    {
        Savegame_EventFlagSet(EventFlag_226);
        func_8003A16C();
    }

    if (PLAYER_IN_MAP_CHUNK(-1, -2))
    {
        if (Savegame_EventFlagGet(EventFlag_71))
        {
            WorldObjects_Add(&g_WorldObject2.object, &g_WorldObject2.position, &SVECTOR3_Zero);
        }
        else
        {
            WorldObjects_Add(&g_WorldObject4.object, &g_WorldObject4.position, &SVECTOR3_Zero);
        }

        if (Savegame_EventFlagGet(EventFlag_72))
        {
            WorldObjects_Add(&g_WorldObject3.object, &g_WorldObject3.position, &SVECTOR3_Zero);
        }
        else
        {
            WorldObjects_Add(&g_WorldObject5.object, &g_WorldObject5.position, &SVECTOR3_Zero);
        }

    }

    if (PLAYER_IN_MAP_CHUNK(-3, -2))
    {
        if (D_800E1180 == 0)
        {
            D_800E1180 = 1;
            Sd_SfxPlay(Sfx_Unk1359, Q7(0.0f), Q8(29.0f / 32.0f));
        }
    }

    if (PLAYER_IN_MAP_CHUNK(-2, -2))
    {
        if (D_800E1180 == 1)
        {
            D_800E1180 = 2;
            Sd_SfxPlay(Sfx_Unk1359, Q7(0.0f), Q8(27.0f / 32.0f));
        }
    }

    if (PLAYER_IN_MAP_CHUNK(1, 3))
    {
        WorldObjects_Add(&g_WorldObject7.object, &g_WorldObject7.position, &g_WorldObject7.rotation);
    }

    if (PLAYER_IN_MAP_CHUNK(0, -1))
    {
        if (Savegame_EventFlagGet(EventFlag_72))
        {
            WorldObjects_Add(&g_WorldObject6[1].object, &g_WorldObject6[1].position, &SVECTOR3_Zero);
            func_80064F04(&Q12_VECTOR3(20.58f, -1.14f, -17.41f), 0, 0x385); // TODO: Demagic angle.
        }
        else
        {
            WorldObjects_Add(&g_WorldObject6[0].object, &g_WorldObject6[0].position, &SVECTOR3_Zero);
        }

        if (Savegame_EventFlagGet(EventFlag_73))
        {
            WorldObjects_Add(&g_WorldObject0.object, &g_WorldObject0.position, &SVECTOR3_Zero);
        }

        if (Savegame_EventFlagGet(EventFlag_72))
        {
            if (D_800E1182 == 0)
            {
                SD_Call(Sfx_Unk1430);
            }

            if (!Savegame_EventFlagGet(EventFlag_84))
            {
                if (Savegame_EventFlagGet(EventFlag_83))
                {
                    D_800E1182 += Q12_MULT_PRECISE(g_DeltaTime, Q12(0.15f));
                    if (D_800E1182 > Q12(1.0f))
                    {
                        D_800E1182 = Q12(1.0f);
                    }
                }
                else
                {
                    D_800E1182 = Q12(0.0999f);
                }
            }
            else
            {
                D_800E1182 = Q12(1.0f);
            }

            Sfx_WithFalloffAndPitchPlay(Sfx_Unk1430, &Q12_VECTOR3(18.8f, -1.0f, -17.5f), D_800E1182 >> 5, Q12(24.0f), 0);
        }
    }

    else if (PLAYER_IN_MAP_CHUNK(1, -1))
    {
        if (Savegame_EventFlagGet(EventFlag_84))
        {
            if (D_800E1182 == 0)
            {
                SD_Call(Sfx_Unk1430);
            }

            D_800E1182 = Q12(0.2f);
            Sfx_WithFalloffAndPitchPlay(Sfx_Unk1430, &Q12_VECTOR3(52.0f, -1.0f, -16.0f), 25, Q12(16.0f), 0);
        }
    }
    else
    {
        if (D_800E1182 != 0)
        {
            Sd_SfxStop(Sfx_Unk1430);
        }

        D_800E1182 = 0;
    }

    if (PLAYER_IN_MAP_CHUNK(3, 0))
    {
        WorldObjects_Add(&g_WorldObject9.object, &g_WorldObject9.position, &g_WorldObject9.rotation);

        for (i = 0; i < 6; i++)
        {
            WorldObjects_Add(&g_WorldObject1[i].object, &g_WorldObject1[i].position, &SVECTOR3_Zero);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(2, 0))
    {
        if (!Savegame_EventFlagGet(EventFlag_M1S00_PickupMap))
        {
            WorldObjects_Add(&g_WorldObject8.object, &g_WorldObject8.position, &g_WorldObject8.rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(1, 0))
    {
        if (!Savegame_EventFlagGet(EventFlag_M1S00_HandgunBullets0))
        {
            WorldObjects_Add(&g_CommonWorldObjects[3], &g_CommonWorldObjectPoses[0].position, &g_CommonWorldObjectPoses[0].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(2, 3))
    {
        if (!Savegame_EventFlagGet(EventFlag_M1S00_HandgunBullets1))
        {
            WorldObjects_Add(&g_CommonWorldObjects[3], &g_CommonWorldObjectPoses[1], &g_CommonWorldObjectPoses[1].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(3, 2))
    {
        if (!Savegame_EventFlagGet(EventFlag_M1S00_HandgunBullets2))
        {
            WorldObjects_Add(&g_CommonWorldObjects[3], &g_CommonWorldObjectPoses[2], &g_CommonWorldObjectPoses[2].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(1, 3))
    {
        if (!Savegame_EventFlagGet(EventFlag_M1S00_HealthDrink))
        {
            WorldObjects_Add(&g_CommonWorldObjects[1], &g_CommonWorldObjectPoses[3], &g_CommonWorldObjectPoses[3].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(1, 3))
    {
        if (!Savegame_EventFlagGet(EventFlag_M1S00_FirstAidKit))
        {
            WorldObjects_Add(&g_CommonWorldObjects[0], &g_CommonWorldObjectPoses[4], &g_CommonWorldObjectPoses[4].rotation);
        }
    }
}

void func_800DA3FC(void) // 0x800DA3FC
{
    if (Savegame_EventFlagGet(EventFlag_M1S01_PickupSilverMedallion))
    {
        Chara_SpawnFlagsSet(Chara_GreyChildOrMumbler, 0, CharaSpawnFlag_0 | CharaSpawnFlag_1);
        Chara_SpawnFlagsSet(Chara_GreyChildOrMumbler, 1, CharaSpawnFlag_0 | CharaSpawnFlag_1);
    }
}
