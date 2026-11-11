#include "bodyprog/bodyprog.h"
#include "bodyprog/chara/dms.h"
#include "bodyprog/events/event_data.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player/player.h"
#include "main/rng.h"
#include "maps/map3/map3_s00.h"
#include "maps/common/characters/kaufmann.h"
#include "maps/common/particle.h"
#include "maps/common/characters/player.h"

#include "../src/maps/common/particles/particle.c"

#include "../src/maps/common/characters/player.c"

#include "../src/maps/common/characters/kaufmann.c"

#include "../src/maps/common/chara_util.c" // 0x800CFE84

#include "../src/maps/common/map_util.c" // 0x800D08A8

#include "maps/common/shared/Map_RoomBgmInit_CheckCond.h" // 0x800D0960

#include "maps/common/shared/Map_RoomBgmInit_3_s00_CondTrue.h" // 0x800D0994

#include "maps/common/shared/Map_RoomBgmInit_3_s00_CondFalse.h" // 0x800D0994

void GameBoot_LoadScreen_StageString(void) {}

#include "maps/common/shared/MapEvent_DoorJammed.h" // 0x800D0B7C

#include "maps/common/shared/MapEvent_DoorLocked.h" // 0x800D0C10

void func_800D0CA4(void) {}

void MapEvent_FirstAidKitTake(void) // 0x800D0CAC
{
    Event_CommonItemTake(CommonPickupItemId_FirstAidKit, EventFlag_M3S00_FirstAidKit);
}

void MapEvent_PaperMapTake(void) // 0x800D0CD0
{
    Event_PaperMapTake(17, EventFlag_M3S00_PickupMap, 48);
}

const char* MAP_MESSAGES[] = {
    #include "maps/common/shared/map_msg_common.inc"
#if VERSION_REGION_IS(NTSC)
    /* 15 */ "~J0(1.0)\tHold_it... ~E ",
    /* 16 */ "~J0(0.6)\t~E ",
    /* 17 */ "~J0(1.7)\tStop! ~N\n\t\t\tDon't_shoot! ",
    /* 18 */ "~J0(2.4)\tWait..._I'm_not_here_to_fight. ",
    /* 19 */ "~J0(3.4)\tMy_name_is_Harry_Mason. ~N\n\t\t\tI'm_in_town_on_vacation. ~E ",
    /* 20 */ "~J1(2.8)\tThank_God. ~N\n\t\t\tAnother_human_being. ",
    /* 21 */ "~J1(1.5)\tDo_you_work_here? ",
    /* 22 */ "~J1(3.6)\tI'm_Dr._Michael_Kaufmann. ~N\n\t\t\tI_work_at_this_hospital. ~E ",
    /* 23 */ "~J1(2.5)\tSo_maybe_you_can_tell_me ~N\n\t\t\twhat's_going_on? ",
    /* 24 */ "~J1(1.5)\tI_really_can't_say. ",
    /* 25 */ "~J1(4.5)\tI_was_taking_a_nap ~N\n\t\t\tin_the_staff_room. ~N\n\t\t\tWhen_I_woke_up,_it_was_like_this. ",
    /* 26 */ "~J1(2.3)\tEveryone_seems_to ~N\n\t\t\thave_disappeared. ",
    /* 27 */ "~J1(3.0)\tAnd_it's_snowing_out. ~N\n\t\t\tThis_time_of_year. ",
    /* 28 */ "~J1(2.6)\tSomething's_gone ~N\n\t\t\tseriously_wrong. ",
    /* 29 */ "~J1(2.2)\tDid_you_see_those_monsters? ",
    /* 30 */ "~J1(2.4)\tHave_you_ever_seen ~N\n\t\t\tsuch_aberrations? ",
    /* 31 */ "~J1(2.0)\tEver_even_heard_of_such_things? ",
    /* 32 */ "~J1(3.5)\tYou_and_I_both_know, ~N\n\t\t\tcreatures_like_that_don't_exist! ",
    /* 33 */ "~J1(1.5)\tYeah... ~E ",
    /* 34 */ "~J1(2.4)\tHave_you_seen ~N\n\t\t\ta_little_girl_anywhere? ",
    /* 35 */ "~J1(1.6)\tI'm_looking_for_my_daughter. ",
    /* 36 */ "~J1(3.0)\tShe's_only_seven. ~N\n\t\t\tShort,_black_hair. ",
    /* 37 */ "~J1(2.8)\tShe's_missing? ~N\n\t\t\tI'm_sorry. ",
    /* 38 */ "~J1(4.0)\tBut_with_all_those ~N\n\t\t\tmonsters_around, ~N\n\t\t\tI_highly_doubt_that_she's... ",
    /* 39 */ "~J1(1.7)\t",
    /* 40 */ "~J1(2.8)\tSorry, ~N\n\t\t\tI_didn't_mean_to_alarm_you. ~E ",
    /* 41 */ "~J0(2.6)\tYour_wife,_she's_here_with_you? ",
    /* 42 */ "~J0(5.0)\tShe_died_four_years_ago. ~N\n\t\t\tNow,_it's_just_me_and_my_daughter. ",
    /* 43 */ "~J0(2.6)\tI_see... ~N\n\t\t\tI'm_sorry. ~E ",
    /* 44 */ "~J0(2.3)\tWell,_I'd_better_be_going. ",
    /* 45 */ "~J0(2.7)\tI_can't_just_sit ~N\n\t\t\taround_here_doing_nothing. ",
    /* 46 */ "~J0(2.8)\tSo_long. ~N\n\t\t\tGood_luck_out_there. ~E ",
    /* 47 */ "\tI_should_search_the_hospital. ~E ",
    /* 48 */ "~H\tThere_is_a_ ~C5 Hospital_map ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 49 */ "\tThese_are_phones_and_phonebooks. ~N\n\tNo_need_to_examine_this. ~E ",
    /* 50 */ "\tNothing_unusual. ~E ",
    /* 51 */ "\tNothing_helpful. ~E "
#elif VERSION_REGION_IS(NTSCJ)
    /* 15 */ "~J0(1.0)\t待ってくれ…… ~E ",
    /* 16 */ "~J0(0.6)\t~E ",
    /* 17 */ "~J0(1.7)\t待て！ ~N\n\t\t\t待て　撃つな！ ",
    /* 18 */ "~J0(2.4)\tその……別に怪しいものじゃない ",
    /* 19 */ "~J0(3.4)\tわたしはハリー・メイソン ~N\n\t\t\tこの町には観光にやってきたんだが…… ~E ",
    /* 20 */ "~J1(2.8)\t……やっと人がいたか ",
    /* 21 */ "~J1(1.5)\tあんたはここの人間か？ ",
    /* 22 */ "~J1(3.6)\tわたしはマイケル・カウフマン ~N\n\t\t\tこの病院の医者だ ~E ",
    /* 23 */ "~J1(2.5)\tこの町で何が起こったんだ？ ",
    /* 24 */ "~J1(1.5)\tわからんよ ",
    /* 25 */ "~J1(4.5)\t仕事明けに仮眠を取って ~N\n\t\t\t起きてみればこのありさまだ ",
    /* 26 */ "~J1(2.3)\t気がつけば人っ子一人いない ",
    /* 27 */ "~J1(3.0)\t季節外れの雪まで降っている ",
    /* 28 */ "~J1(2.6)\tそしてきわめつけはこの化け物だ ",
    /* 29 */ "~J1(2.2)\tきみはこんな生き物を知っているか？ ",
    /* 30 */ "~J1(2.4)\t見たことはあるか？ ",
    /* 31 */ "~J1(2.0)\t聞いたことはあるか？ ",
    /* 32 */ "~J1(3.5)\tこんな化け物　世界中の ~N\n\t\t\tどこを探したっていやしない ",
    /* 33 */ "~J1(1.5)\tそうだな…… ~E ",
    /* 34 */ "~J1(2.4)\tそうだ ~N\n\t\t\t女の子を見なかったか？ ",
    /* 35 */ "~J1(1.6)\tわたしの娘なんだが…… ",
    /* 36 */ "~J1(3.0)\tまだ小さい……７歳の ~N\n\t\t\t短い黒髪の子だ ",
    /* 37 */ "~J1(2.8)\tはぐれたのか？ ~N\n\t\t\t悪いがわたしは見ていない ",
    /* 38 */ "~J1(4.0)\tしかし……こんな化け物がうようよ ~N\n\t\t\tいる中で無事かどうか…… ",
    /* 39 */ "~J1(1.7)\t",
    /* 40 */ "~J1(2.8)\tすまんな ~N\n\t\t\t別に悲観させるつもりじゃないんだが ~E ",
    /* 41 */ "~J0(2.6)\t奥さんは？ ~N\n\t\t\t一緒に来ていないのか？ ",
    /* 42 */ "~J0(5.0)\t妻は４年前に死んだ ~N\n\t\t\t娘と二人だけだ ",
    /* 43 */ "~J0(2.6)\tそうか……失礼した ~E ",
    /* 44 */ "~J0(2.3)\tとにかく　わたしは町を出るよ ",
    /* 45 */ "~J0(2.7)\tこのままじっとしてもいられんからな ",
    /* 46 */ "~J0(2.8)\tそうか ~N\n\t\t\t気をつけろよ ~E ",
    /* 47 */ "\t病院の中を調べよう ~E ",
    /* 48 */ "~H ~C5 病院の地図 ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 49 */ "\t電話機とこの地域の電話帳がある ~N\n\t特に調べる必要はないだろう ~E ",
    /* 50 */ "\t特に気になることはない ~E ",
    /* 51 */ "\t役に立ちそうなものはない ~E "
#endif
};

void func_800D0CF8(void) // 0x800D0CF8
{
    // TODO: Fill this out.
    typedef enum _EventState
    {
        EventState_CutsceneStart = 4,
        EventState_CutsceneEnd   = 32,
        EventState_CutsceneSkip  = 36
    } e_EventState;

    if (Pad_ButtonClickedBind(skip) &&
        g_SysWork.sysStateSteps[0] >= EventState_CutsceneStart && g_SysWork.sysStateSteps[0] < EventState_CutsceneEnd)
    {
        SysWork_StateStepSet(0, EventState_CutsceneSkip);
    }

    if (g_SysWork.sysStateSteps[0] >= 24 && g_SysWork.sysStateSteps[0] < 28)
    {
        g_DeltaTime = Q12_MULT_PRECISE(g_DeltaTime, Q12(1.5f));
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Sfx_WithFlagsPlay(Sfx_DoorClose0, &Q12_VECTOR3(94.6f, -1.2f, 101.5f), Q8(0.5f), SfxFlag_None);
            Savegame_EventFlagSet(EventFlag_MapMark_FogHospital1F_StairsLocked);

            Fs_QueueStartRead(FILE_ANIM_HP1F03_DMS, FS_BUFFER_15);
            Fs_QueueWaitForEmpty();
            Dms_HeaderFixOffsets((s_DmsHeader*)FS_BUFFER_15);
            Chara_Spawn(Chara_Kaufmann, 0, Q12(100.0f), Q12(100.0f), 0, 3);
            Chara_MovementReset(&NPC_CHARA(0));

            CutsceneBorder_ForceShow();
            g_Cutscene_Timer = Q12(0.0f);
            D_800D256C = 0;
            g_SysWork.sysFlags |= SysFlag_CutsceneActive;

            func_800625F4(&Q12_VECTOR3(95.8f, 0.0f, 101.27f), 0x48, 2, 1);
            func_800625F4(&Q12_VECTOR3(96.05f, 0.0f, 101.07f), 0x48, 2, 1);
            func_800625F4(&Q12_VECTOR3(96.1f, 0.0f, 100.68f), 0x48, 2, 1);
            func_8003D03C();
            Player_EmptyWeaponHandSet();
            SysWork_StateStepIncrement(0);
            break;

        case 1:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, g_SysWork.npcs, 6, false);
            SysWork_StateStepIncrement(0);
            break;

        case 2:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimLock, g_SysWork.npcs, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.0f), false);
            break;

        case 3:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimUnlock, g_SysWork.npcs, 0, false);
            SysWork_StateStepIncrement(0);

        case EventState_CutsceneStart:
            g_Cutscene_Timer += Q12_MULT_PRECISE(g_DeltaTime, Q12(12.0f));
            if (g_Cutscene_Timer > Q12(10.0f))
            {
                g_Cutscene_Timer = Q12(10.0f);
                SysWork_StateStepIncrement(0);
            }
            break;

        case 5:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AwaitAnimEnd, g_SysWork.npcs, 0, false);

            if (g_Cutscene_Timer < Q12(11.0f))
            {
                g_Cutscene_Timer = Q12(11.0f);
            }
            else
            {
                g_Cutscene_Timer += Q12_MULT_PRECISE(g_DeltaTime, Q12(10.0f));
                if (g_Cutscene_Timer > Q12(25.0f))
                {
                    g_Cutscene_Timer = Q12(25.0f);
                }
            }

            Event_DisplayMapMsgWithAudio(15, &D_800D256C, &D_800D24F0);
            break;

        case 6:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &NPC_CHARA(0), 7, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &PLAYER_CHARA(), 0x73, false);
            SysWork_StateStepIncrement(0);

        case 7:
            Event_DisplayMapMsgWithAudio(15, &D_800D256C, &D_800D24F0);

            g_Cutscene_Timer += Q12_MULT_PRECISE(g_DeltaTime, Q12(10.0f));
            if (g_Cutscene_Timer > Q12(25.0f))
            {
                g_Cutscene_Timer = Q12(25.0f);
                SysWork_StateStepIncrement(0);
            }
            break;

        case 8:
            SysWork_StateStepIncrement(0);

        case 9:
            Event_DisplayMapMsgWithAudio(15, &D_800D256C, &D_800D24F0);

            g_Cutscene_Timer += Q12_MULT_PRECISE(g_DeltaTime, Q12(10.0f));
            if (g_Cutscene_Timer > Q12(109.0f))
            {
                g_Cutscene_Timer = Q12(109.0f);
            }
            break;

        case 10:
            Event_DisplayMapMsgWithAudio(16, &D_800D256C, &D_800D24F0);

            g_Cutscene_Timer += Q12_MULT_PRECISE(g_DeltaTime, Q12(10.0f));
            if (g_Cutscene_Timer > Q12(109.0f))
            {
                g_Cutscene_Timer = Q12(109.0f);
            }
            break;

        case 11:
            Event_DisplayMapMsgWithAudio(17, &D_800D256C, &D_800D24F0);
            Event_WaitTimer(0x7000, false);

            g_Cutscene_Timer += Q12_MULT_PRECISE(g_DeltaTime, Q12(10.0f));
            if (g_Cutscene_Timer > Q12(109.0f))
            {
                g_Cutscene_Timer = Q12(109.0f);
            }
            break;

        case 12:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &NPC_CHARA(0), 8, false);
            SysWork_StateStepIncrement(0);

        case 13:
            Event_DisplayMapMsgWithAudio(17, &D_800D256C, &D_800D24F0);

            g_Cutscene_Timer += Q12_MULT_PRECISE(g_DeltaTime, Q12(10.0f));
            if (g_Cutscene_Timer > Q12(109.0f))
            {
                g_Cutscene_Timer = Q12(109.0f);
            }
            break;

        case 14:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 15:
            g_Cutscene_Timer = Q12(110.0f);

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &PLAYER_CHARA(), 51, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &NPC_CHARA(0), 5, false);
            SysWork_StateStepIncrement(0);

        case 16:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.0f), false);
            break;

        case 17:
            Event_DisplayMapMsgWithAudio(20, &D_800D256C, &D_800D24F0);
            break;

        case 18:
            g_Cutscene_Timer = Q12(111.0f);
            Event_DisplayMapMsgWithAudio(23, &D_800D256C, &D_800D24F0);
            break;

        case 19:
            Event_WaitTimer(Q12(1.5f), false);
            break;

        case 20:
            Event_DisplayMapMsgWithAudio(34, &D_800D256C, &D_800D24F0);
            break;

        case 21:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, g_SysWork.npcs, 9, false);
            SysWork_StateStepIncrement(0);

        case 22:
            if (g_Cutscene_Timer < Q12(112.0f))
            {
                g_Cutscene_Timer = Q12(112.0f);
            }
            else
            {
                g_Cutscene_Timer += Q12_MULT_PRECISE(g_DeltaTime, Q12(10.0f));
                if (g_Cutscene_Timer > Q12(174.0f))
                {
                    g_Cutscene_Timer = Q12(174.0f);
                }
            }

            Event_WaitTimer(Q12(1.5f), false);
            break;

        case 23:
            Event_DisplayMapMsgWithAudio(41, &D_800D256C, &D_800D24F0);

            g_Cutscene_Timer += Q12_MULT_PRECISE(g_DeltaTime, Q12(10.0f));
            if (g_Cutscene_Timer > Q12(174.0f))
            {
                g_Cutscene_Timer = Q12(174.0f);
            }
            break;

        case 24:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &NPC_CHARA(0), 1, false);
            SysWork_StateStepIncrement(0);

        case 25:
            if (g_Cutscene_Timer >= Q12(175.0f))
            {
                g_Cutscene_Timer += Q12_MULT_PRECISE(g_DeltaTime, Q12(15.0f));
                if (g_Cutscene_Timer > Q12(182.0f))
                {
                    g_Cutscene_Timer = Q12(182.0f);
                    SysWork_StateStepIncrement(0);
                }
            }
            else
            {
                g_Cutscene_Timer = Q12(175.0f);
            }
            break;

        case 26:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &PLAYER_CHARA(), 0x48, false);
            SysWork_StateStepIncrement(0);

        case 27:
            if (g_Cutscene_Timer < Q12(175.0f))
            {
                g_Cutscene_Timer = Q12(175.0f);
            }
            else
            {
                g_Cutscene_Timer += Q12_MULT_PRECISE(g_DeltaTime, Q12(15.0f));
                if (g_Cutscene_Timer > Q12(220.0f))
                {
                    g_Cutscene_Timer = Q12(220.0f);
                    SysWork_StateStepIncrement(0);
                }
            }
            break;

        case 28:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &NPC_CHARA(0), 10, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &PLAYER_CHARA(), 51, false);
            SysWork_StateStepIncrement(0);

        case 29:
            if (g_Cutscene_Timer < Q12(221.0f))
            {
                g_Cutscene_Timer = Q12(221.0f);
            }
            else
            {
                g_Cutscene_Timer += Q12_MULT_PRECISE(g_DeltaTime, Q12(10.0f));
                if (g_Cutscene_Timer > Q12(242.0f))
                {
                    g_Cutscene_Timer = Q12(242.0f);
                }
            }

            Event_DisplayMapMsgWithAudio(0x2c, &D_800D256C, &D_800D24F0);
            break;

        case 30:
            Chara_ModelCharaIdClear(&NPC_CHARA(0), 0, 0);
            g_Cutscene_Timer = Q12(243.0f);
            SysWork_StateStepIncrement(0);

        case 31:
            Event_WaitTimer(Q12(0.2f), false);
            break;

        case EventState_CutsceneEnd:
            Sfx_WithFlagsPlay(Sfx_DoorOpen0, &D_800D2530, Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 33:
            Event_WaitTimer(Q12(1.2f), false);
            break;

        case 34:
            Sfx_WithFlagsPlay(Sfx_DoorClose0, &D_800D2530, Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 35:
            Event_WaitTimer(Q12(0.7f), false);
            break;

        case EventState_CutsceneSkip:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 37:
            g_Cutscene_Timer = Q12(243.0f);
            Chara_ModelCharaIdClear(&NPC_CHARA(0), 0, 0);
            SysWork_StateStepIncrement(0);
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            vcReturnPreAutoCamWork(true);
            SD_Call(0x13);

            g_Cutscene_Timer = NO_VALUE;

            Savegame_EventFlagSet(EventFlag_197);
            Savegame_EventFlagSet(EventFlag_198);
            func_8003D01C();
            Player_WeaponAttackRestore();
            break;
    }

    if (g_Cutscene_Timer >= Q12(0.0f))
    {
        Dms_CharacterTransformGet(&PLAYER_CHARA().position, &PLAYER_CHARA().rotation, "HERO", g_Cutscene_Timer, (s_DmsHeader* )FS_BUFFER_15);
        Dms_CharacterTransformGet(&NPC_CHARA(0).position, &NPC_CHARA(0).rotation, "KAU", g_Cutscene_Timer, (s_DmsHeader*)FS_BUFFER_15);
        vcChangeProjectionValue(Dms_CameraTargetsGet(&g_Cutscene_CameraPositionTarget, &g_Cutscene_CameraLookAtTarget, NULL, g_Cutscene_Timer, (s_DmsHeader*)FS_BUFFER_15));
        vcUserCamTarget(&g_Cutscene_CameraPositionTarget, NULL, true);
        vcUserWatchTarget(&g_Cutscene_CameraLookAtTarget, NULL, true);
    }
}

void Map_WorldObjectsInit(void) // 0x800D18DC
{
    WorldObject_PlacementInit(&g_WorldObject0, "TRUNK_HI", 97.2496f, 0.0f, 100.3428f);
    WorldObject_PlacementInit(&g_WorldObject1[0], "BIRD_HID", 98.4512f, 0.0f, 101.087f);
    WorldObject_PlacementInit(&g_WorldObject1[1], "BIRD2_HI", 98.4512f, 0.0f, 101.087f);
    WorldObject_Init(&g_WorldObject2, g_CommonWorldObjectNames[1], 26.0f, -1.0198f, 144.5498f, 0.0f, 163.8f, 0.0f);
    WorldObject_Init(&g_WorldObject3, "MAP_HIDE", 24.6f, -1.56f, 143.16f, 0.0f, 180.0f, 0.0f);
    WorldObject_Init(&g_WorldObject4, "MAP2_HID", 24.6f, -1.56f, 143.16f, 0.0f, 180.0f, 0.0f);
    WorldObject_Init(&g_WorldObject5, g_CommonWorldObjectNames[2], 27.6f, -0.45f, 143.6f, 0.0f, -90.0f, 0.0f);
}

void Map_WorldObjectsUpdate(void) // 0x800D1A98
{
    static const SVECTOR3 EMPTY_ROT = {};
    static const VECTOR3  SOUND_POS = VECTOR3(22.5f, -1.5f, 137.0f);

    s32 x;
    s32 tmp;
    s32 rand0;
    s32 diff;

    if (PLAYER_IN_MAP_CHUNK(2, 2))
    {
        if (!Savegame_EventFlagGet(EventFlag_198))
        {
            WorldObjects_Add(&g_WorldObject0.object, &g_WorldObject0.position, &EMPTY_ROT);
        }
        WorldObjects_Add(&g_WorldObject1[0].object, &g_WorldObject1[0].position, &EMPTY_ROT);
        WorldObjects_Add(&g_WorldObject1[1].object, &g_WorldObject1[1].position, &EMPTY_ROT);
    }

    if (PLAYER_IN_MAP_CHUNK(0, 3))
    {
        WorldObjects_Add(&g_WorldObject2.object, &g_WorldObject2.position, &g_WorldObject2.rotation);

        if (Savegame_EventFlagGet(EventFlag_M3S00_PickupMap))
        {
            WorldObjects_Add(&g_WorldObject4.object, &g_WorldObject4.position, &g_WorldObject4.rotation);
        }
        else
        {
            WorldObjects_Add(&g_WorldObject3.object, &g_WorldObject3.position, &g_WorldObject3.rotation);
        }

        if (!Savegame_EventFlagGet(EventFlag_M3S00_FirstAidKit))
        {
            WorldObjects_Add(&g_WorldObject5.object, &g_WorldObject5.position, &g_WorldObject5.rotation);
        }

        if (!Savegame_EventFlagGet(EventFlag_200) && (PLAYER_CHARA().position.vx < Q12(24.0f)))
        {
            tmp = Rng_Rand16() % Q12(3.0f);
            rand0 = tmp;
            diff = PLAYER_CHARA().position.vx - Q12(24.0f);

            if (diff >= 0)
            {
                x = diff / 3;
            }
            else
            {
                x = (Q12(24.0f) - PLAYER_CHARA().position.vx) / 3;
            }

            if (rand0 < Math_Sin(x))
            {
                Sfx_WithFlagsPlay(Sfx_Unk1492, &SOUND_POS, Q8(7.0f / 8.0f), SfxFlag_NoDistAtten);
                Savegame_EventFlagSet(EventFlag_200);
            }
        }
    }
}
