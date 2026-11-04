#include "bodyprog/bodyprog.h"
#include "bodyprog/chara/dms.h"
#include "bodyprog/events/event_data.h"
#include "bodyprog/gfx/map_effects.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player/player.h"
#include "main/rng.h"
#include "maps/map0/map0_s01.h"

const char* MAP_MESSAGES[] = {
    #include "maps/shared/map_msg_common.h"
#if VERSION_REGION_IS(NTSC)
    /* 15 */ "~J0(2.0)\tWas_I_dreaming?  ~E ",
    /* 16 */ "~J0(2.5)\tHow_do_you_feel? ~E ",
    /* 17 */ "~J0(6.0)\tLike_I've_been ~N\n\t\t\trun_over_by_a_truck, ~N\n\t\t\tbut_I'm_all_right,_I_guess. ~E ",
    /* 18 */ "~J0(1.5)\tGlad_to_hear_it.",
    /* 19 */ "~J0(5.0)\tYou_from_around_here? ~N\n\t\t\tWhy_don't_you ~N\n\t\t\ttell_me_what_happened? ",
    /* 20 */ "~J0(3.0)\tWait_a_second. ~N\n\t\t\tI'm_just_a_tourist. ",
    /* 21 */ "~J0(5.2)\tI_came_here_for_a_vacation. ~N\n\t\t\tI_just_got_here... ",
    /* 22 */ "~J0(4.2)\tI_don't_know_what_happened. ",
    /* 23 */ "~J0(4.0)\tI'd_like_to_find_out_myself. ",
    /* 24 */ "~J0(2.0)\tUh-huh? ~E ",
    /* 25 */ "~J0(3.0)\tHave_you_seen_a_little_girl? ",
    /* 26 */ "~J0(4.0)\tJust_turned_seven_last_month... ~N\n\t\t\tShort,_black_hair... ",
    /* 27 */ "~J0(1.5)\tMy_daughter. ",
    /* 28 */ "~J0(1.0)\tSorry. ",
    /* 29 */ "~J0(3.0)\tThe_only_person_I've_seen ~N\n\t\t\tin_this_town_is_you. ",
    /* 30 */ "~J0(2.5)\tWhere_is_everybody? ",
    /* 31 */ "~J0(3.5)\tI'd_tell_you_if_I_knew,~N\n\t\t\tbelieve_me. ",
    /* 32 */ "~J0(5.5)\tBut,_from_what_I_can_tell, ~N\n\t\t\tsomething_bizarre_is_going_on. ",
    /* 33 */ "~J0(2.5)\tThat's_all_I_know. ",
    /* 34 */ "~J0(2.0)\tHmmm... ~E ",
    /* 35 */ "~J0(2.0)\tWhat's_your_name? ",
    /* 36 */ "~J0(1.0)\tHarry... ",
    /* 37 */ "~J0(2.0)\tHarry_Mason. ",
    /* 38 */ "~J0(2.0)\tCybil_Bennett. ",
    /* 39 */ "~J0(4.0)\tI'm_a_police_officer ~N\n\t\t\tfrom_Brahms, ~N\n\t\t\tthe_next_town_over. ",
    /* 40 */ "~J0(3.0)\tThe_phones_are_all_dead, ~N\n\t\t\tand_the_radio,_too. ",
    /* 41 */ "~J0(4.0)\tI'm_going_back_to_call ~N\n\t\t\tin_some_reinforcements. ",
    /* 42 */ "~J0(1.5)\tHmph. ~E ",
    /* 43 */ "~D\t~J0(4.0)\tHold_it! ~N\n\t\t\tWhere_do_you_think ~N\n\t\t\tyou're_going? ~E ",
    /* 44 */ "~J0(4.0)\tMy_daughter... ~N\n\t\t\tI've_gotta_find_her! ~E ",
    /* 45 */ "~D\t~J0(1.5)\tNo_way! ~E ",
    /* 46 */ "~J0(3.0)\tIt's_dangerous_out_there. ",
    /* 47 */ "~J0(5.0)\tIn_that_case, ~N\n\t\t\tI_need_to_find_her_now! ~N\n\t\t\tCheryl's_my_little_girl. ",
    /* 48 */ "~J0(3.0)\tI_can't_just_leave_her_out ~N\n\t\t\tthere_by_herself. ~E ",
    /* 49 */ "~J0(2.5)\tHave_you_got_a_gun? ",
    /* 50 */ "~J0(3.0)\tUmmm..._no. ~E ",
    /* 51 */ "~J0(5.0)\tTake_this. ~N\n\t\t\tAnd_hope_you_don't ~N\n\t\t\thave_to_use_it. ",
    /* 52 */ "~J0(1.5)\tNow_listen_to_me, ",
    /* 53 */ "~J0(4.5)\tbefore_you_pull_the_trigger, ~N\n\t\t\tknow_who_you're_shooting. ",
    /* 54 */ "~J0(3.0)\tAnd_don't_do_it ~N\n\t\t\tunless_you_have_to. ~E ",
    /* 55 */ "~J0(3.0)\tAnd_don't_go_blasting_me ~N\n\t\t\tby_mistake. ",
    /* 56 */ "~J0(2.5)\tGot_it? ~E ",
    /* 57 */ "~J0(2.5)\tYeah._Thanks. ~E ",
    /* 58 */ "~J0(5.0)\tYou'd_do_best_to_stay_nearby. ~N\n\t\t\tI'll_be_back_with ~N\n\t\t\thelp_as_quick_as_I_can. ~E ",
    /* 59 */ "~H There_is_a_ ~C5 Residential_area_map ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 60 */ "\tThere_is_a_ ~C2 Kitchen_knife ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 61 */ "\tThere_is_a_ ~C2 Flashlight ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 62 */ "~J0(1.1)\tWhat's_that? ~E ",
    /* 63 */ "~J0(4.5)\tHuh?_Radio? ~N\n\t\t\tWhat's_going_on_with_that_radio? ~E ",
    /* 64 */ "\tThere_is_a_ ~C2 Pocket_radio ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 65 */ "\tA_radio... is_it_broken? ~N\n\tCan't_hear_anything. ~E ",
    /* 66 */ " ",
    /* 67 */ "\tMaybe_I_should_check_for ~N\n\tsomething_useful_before_leaving. ~E ",
    /* 68 */ "~J0(4.3)\tThis_is_not_a_dream! ~N\n\tWhat's_happening_to_this_place!? ~E ",
    /* 69 */ "\tI'll_worry_about_the_radio_later, ~N\n\tI_have_more_important_stuff ~N\n\tto_do. ~E ",
    /* 70 */ "\tI've_got_to_beat_that_monster! ~E ",
    /* 71 */ "\tThe_radio_may_be ~N\n\tuseful_for_something. ",
    /* 72 */ "\tI_better_take_it_with_me. ~E ",
    /* 73 */ "\tPinball_machine. ~N\n\tNothing_unusual. ~E ",
    /* 74 */ "\tCan't_turn_on_the_TV. ~N\n\tNo_power? ~E ",
    /* 75 */ "\tI_have_more_important_stuff_to_do. ~E "
#elif VERSION_REGION_IS(NTSCJ)
    /* 15 */ "~J0(2.0)\t……夢？ ~E ",
    /* 16 */ "~J0(2.5)\t気がついた？ ~N\n\t\t\t気分はどう？ ~E ",
    /* 17 */ "~J0(6.0)\tあまり良くはないが……何とか大丈夫だ ~E ",
    /* 18 */ "~J1(1.5)\tそう良かった ",
    /* 19 */ "~J1(5.0)\tあなたはこの町の住人？ ~N\n\t\t\t何が起こったの？ ",
    /* 20 */ "~J1(3.0)\tちょっと待ってくれ ~N\n\t\t\tわたしはただの観光客だ ",
    /* 21 */ "~J1(5.2)\tこの町には休暇を取りにやってきたんだ ~N\n\t\t\t着いたのはついさっきで……それだけだ ",
    /* 22 */ "~J1(4.2)\t何がどうなっているのか ~N\n\t\t\tわたしにはまったくわからない ",
    /* 23 */ "~J1(4.0)\t何が起こったのか ~N\n\t\t\tわたしが聞きたいぐらいだ ",
    /* 24 */ "~J1(2.0)\tそう ~E ",
    /* 25 */ "~J0(3.0)\tそれより女の子を見なかったか？ ",
    /* 26 */ "~J0(4.0)\t先月７歳の誕生日を迎えた ~N\n\t\t\tばかりで　短い黒髪の…… ",
    /* 27 */ "~J0(1.5)\t娘なんだ ",
    /* 28 */ "~J0(1.0)\tいいえ ",
    /* 29 */ "~J0(3.0)\tこの町で会ったのはあなた一人よ ",
    /* 30 */ "~J0(2.5)\t……この町はいったい ~N\n\t\t\tどうなってるんだ？ ",
    /* 31 */ "~J0(3.5)\t知っていれば教えて ~N\n\t\t\tあげたいところだけど…… ",
    /* 32 */ "~J0(5.5)\t何とかわかってるのは ~N\n\t\t\t信じられないようなことに ~N\n\t\t\tなっているということ…… ",
    /* 33 */ "~J0(2.5)\tたったそれだけなの ",
    /* 34 */ "~J0(2.0)\t~E ",
    /* 35 */ "~J1(2.0)\tあなた　名前は？ ",
    /* 36 */ "~J1(1.0)\tハリー ",
    /* 37 */ "~J1(2.0)\tハリー・メイソンだ ",
    /* 38 */ "~J1(2.0)\tあたしはシビル・ベネット",
    /* 39 */ "~J1(4.0)\t隣のブラマ市の警官よ ",
    /* 40 */ "~J1(3.0)\t電話も無線も通じないの ",
    /* 41 */ "~J1(4.0)\tとりあえず　あたしは一旦戻って ~N\n\t\t\t応援を呼んでくるわ ",
    /* 42 */ "~J1(1.5)\tそうか…… ~E ",
    /* 43 */ "~D\t~J0(4.0) 待って！ ~N\n\t\t\tどこへ行くの？ ~E ",
    /* 44 */ "~J0(4.0)\t決まっている ~N\n\t\t\t娘を探すんだ ~E ",
    /* 45 */ "~D\t~J0(1.5)\t駄目よ！ ~E ",
    /* 46 */ "~D\t~J0(3.0)\t危険だわ ",
    /* 47 */ "~J0(5.0)\t……そうだとしても ~N\n\t\t\tじっとしているわけにはいかない ",
    /* 48 */ "~J0(3.0)\t町が危険ならば　なおさらだ ~E ",
    /* 49 */ "~J0(2.5)\t銃は持ってる？ ",
    /* 50 */ "~J0(3.0)\t……いや ~E ",
    /* 51 */ "~J0(5.0)\tこれを預けておくわ ~N\n\t\t\t使わずに済むといいけど ",
    /* 52 */ "~J0(1.5) ",
    /* 53 */ "~J0(4.5)\t引き金を引く時には ~N\n\t\t\t相手をちゃんと見きわめるのよ ",
    /* 54 */ "~J0(3.0)\t撃つべきか否か ~E ",
    /* 55 */ "~J0(3.0)\tまちがってもあたしを撃ったりしないでよ ",
    /* 56 */ "~J0(2.5)\tいいわね？ ~E ",
    /* 57 */ "~J0(2.5)\tああ ~E ",
    /* 58 */ "~J0(5.0)\tできるだけここを動かないで ~N\n\t\t\tすぐに人を呼んで戻ってくるわ ~E ",
    /* 59 */ "~H\t~C5 住宅地の地図 ~C7 がある ~N\n\t\t取りますか？ ~S4 ",
    /* 60 */ "\t~C2 調理用ナイフ ~C7 がある ~N\n\t\t取りますか？ ~S4 ",
    /* 61 */ "\t~C2 ライト ~C7 がある ~N\n\t\t取りますか？ ~S4 ",
    /* 62 */ "~J0(1.1)\t何だ？ ~E ",
    /* 63 */ "~J0(4.5)\tラジオか…… ~N\n\t\t\tどうしたんだ突然？ ~E ",
    /* 64 */ "~C2 ポケットラジオ ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 65 */ "\tラジオか……壊れてるのか？ ~N\n\t何も聞こえないな…… ~E ",
    /* 66 */ "\tラジオが気になるな ~N\n\t調べておいた方がいいだろう~E ",
    /* 67 */ "\t外に出る前に何か役に立ちそうなものが ~N\n\tないか探してみた方がいいだろう…… ~E ",
    /* 68 */ "~J0(4.3) ……夢じゃなかったっていうのか！ ~N\n\tこの町はいったいどうなってるんだ！？ ~E ",
    /* 69 */ "\tラジオを取るのは後回しだ ~N\n\t今はこの怪物を倒すのが先だ ~E ",
    /* 70 */ "\tこの怪物を倒さなくては！ ~E ",
    /* 71 */ "\t何かの役に立つかもしれない ~N\n\tラジオは持っていった方がいいだろう ~E ",
    /* 72 */ "\tピンボールの台だ ~N\n\t特に気になることはない ~E ",
    /* 73 */ "\tテレビが点かないな…… ~N\n\t停電しているのか？ ~E ",
    /* 74 */ "\t今はそんなことをしている場合ではない ~E "
#endif
};

void MapEvent_CafeCutscene(void) // 0x800DA980
{
    typedef enum _EventState
    {
        EventState_Skip = 52
    } e_EventState;

    u8              temp_s0_5;
    s_SubCharacter* cybil0;
    s_SubCharacter* cybil1;

    #define cybilChara g_SysWork.npcs[0]

    // Skip.
    if ((g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.skip) &&
        g_SysWork.sysStateSteps[0] > 5 && g_SysWork.sysStateSteps[0] < 47)
    {
        SysWork_StateStepSet(0, EventState_Skip);
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();

            CutsceneBorder_ForceShow();
            ScreenFade_ResetTimestep();
            g_SysWork.sysFlags              |= SysFlag_CutsceneActive;
            g_SavegamePtr->itemToggleFlags &= ~ItemToggleFlag_RadioOn;
            g_SysWork.sysFlags              |= SysFlag_5 | SysFlag_9;
            g_Cutscene_MapMsgAudioIdx                   = 0;
            D_800DE250                         = 0;
            D_800DE251                         = 0;

            Fs_QueueStartRead(FILE_ANIM_CAFE_DMS, FS_BUFFER_11);
            Fs_QueueWaitForEmpty();
            Dms_HeaderFixOffsets(FS_BUFFER_11);
            Chara_Load(0, Chara_Cybil, &g_SysWork.npcBoneCoordBuffer[0], CHARA_FORCE_FREE_ALL, NULL, NULL);
            Chara_ProcessLoads();
            Chara_Spawn(Chara_Cybil, 0, Q12(4.4f), Q12(269.9f), Q12_ANGLE(0.0f), 2);

            g_Cutscene_Timer = Q12(0.0f);
            SysWork_StateStepIncrement(0);

        case 1:
            Bgm_CrossfadeToTrack(BgmCmd_Track9);
            break;

        case 2:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 77, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            sharedFunc_800D2E6C_0_s00();
            SysWork_StateStepIncrement(0);
            break;

        case 3:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimLock, &g_SysWork.playerWork.player, NULL, false);
            SysWork_StateStepIncrement(0);

        case 4:
            Event_ScreenFadeCmd(ScreenFadeCmd_Wait, false, 0, Q12(0.0f), false);
            break;

        case 5:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimUnlock, &g_SysWork.playerWork.player, NULL, false);
            SysWork_StateStepIncrement(0);

        case 6:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(7.8f), Q12(0.0f), Q12(5.0f), false, true);
            break;

        case 7:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(7.8f), Q12(0.0f), Q12(26.0f), true, false);
            Event_DisplayMapMsgWithAudio(15, &g_Cutscene_MapMsgAudioIdx, g_Cutscene_MapMsgAudioCmds); // "Was I dreaming?"
            break;

        case 8:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(7.8f), Q12(0.0f), Q12(26.0f), true, true);
            break;

        case 9:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 78, false);
            D_800DE250 = 1;
            SysWork_StateStepIncrement(0);

        case 10:
        case 11:
        case 12:
        case 13:
            switch (g_SysWork.sysStateSteps[0])
            {
                case 10:
                    Event_DisplayMapMsgWithAudio(16, &g_Cutscene_MapMsgAudioIdx, g_Cutscene_MapMsgAudioCmds); // "How do you feel?"
                    break;

                case 11:
                    Event_DisplayMapMsgWithAudio(17, &g_Cutscene_MapMsgAudioIdx, g_Cutscene_MapMsgAudioCmds); // "Like I've been hit..."
                    break;

                case 12:
                    if (g_Cutscene_Timer == Q12(48.0f) && D_800DE251 == 2)
                    {
                        SysWork_StateStepSet(0, 13);
                    }
                    break;

                case 13:
                    Event_DisplayMapMsgWithAudio(18, &g_Cutscene_MapMsgAudioIdx, g_Cutscene_MapMsgAudioCmds); // "Glad to hear it."
                    break;
            }

            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(4.0f), Q12(27.0f), Q12(48.0f), true, false);

            if (D_800DE251 == 0 && g_Cutscene_Timer > Q12(38.0f))
            {
                Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &cybilChara, 10, false);
                Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 110, false);

                D_800DE251++;
            }

            temp_s0_5 = D_800DE251;
            if (D_800DE251 == 1 && Chara_AnimPlaybackStateGet(&cybilChara) == temp_s0_5)
            {
                Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &cybilChara, 11, false);
                D_800DE251++;
            }
            break;

        case 14:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(3.0f), Q12(49.0f), Q12(139.0f), true, false);
            Event_DisplayMapMsgWithAudio(25, &g_Cutscene_MapMsgAudioIdx, g_Cutscene_MapMsgAudioCmds); // "Have you seen a little girl?"
            break;

        case 15:
            g_Cutscene_Timer = Q12(140.0f);
            SysWork_StateStepIncrement(0);

        case 16:
            Event_DisplayMapMsgWithAudio(35, &g_Cutscene_MapMsgAudioIdx, g_Cutscene_MapMsgAudioCmds); // "What's your name?"
            break;

        case 17:
            Event_WaitTimer(Q12(1.2f), false);
            break;

        case 18:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 93, false);
            SysWork_StateStepIncrement(0);

        case 19:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(141.0f), Q12(152.0f), false, true);
            break;

        case 20:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 53, false);
            SysWork_StateStepIncrement(0);

        case 21:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(141.0f), Q12(159.0f), true, true);
            Sd_XaPreLoadAudioPreTaskAdd(Sfx_XaAudio12);
            break;

        case 22:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &cybilChara, 7, false);
            SD_Call(Sfx_XaAudio12);
            SysWork_StateStepIncrement(0);

        case 23:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(160.0f), Q12(173.0f), true, false);
            Event_DisplayMapMsg(false, 43, 0, 0, 0, false);
            Text_Debug_PositionSet(30, 30);
            break;

        case 24:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 71, false);
            SysWork_StateStepIncrement(0);

        case 25:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(174.0f), Q12(184.0f), true, false);
            Event_DisplayMapMsgWithAudio(44, &g_Cutscene_MapMsgAudioIdx, g_Cutscene_MapMsgAudioCmds);
            break;

        case 26:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 51, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &cybilChara, 8, 0);
            SysWork_StateStepIncrement(0);

        case 27:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(185.0f), Q12(194.0f), true, false);
            Event_DisplayMapMsgWithAudio(45, &g_Cutscene_MapMsgAudioIdx, g_Cutscene_MapMsgAudioCmds);
            break;

        case 28:
            g_Cutscene_Timer = Q12(195.0f);

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &cybilChara, 5, false);
            SysWork_StateStepIncrement(0);

        case 29:
            Event_DisplayMapMsgWithAudio(46, &g_Cutscene_MapMsgAudioIdx, g_Cutscene_MapMsgAudioCmds); // "It's dangerous out there."
            break;

        case 30:
            Event_WaitTimer(Q12(1.5f), false);
            break;

        case 31:
            Event_DisplayMapMsgWithAudio(49, &g_Cutscene_MapMsgAudioIdx, g_Cutscene_MapMsgAudioCmds);
            break;

        case 32:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &cybilChara, 1, false);
            SysWork_StateStepIncrement(0);

        case 33:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(20.0f), Q12(207.0f), Q12(222.0f), true, true);
            break;

        case 34:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &cybilChara, 6, false);
            SysWork_StateStepIncrement(0);

        case 35:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(5.0f), Q12(223.0f), Q12(251.0f), true, false);
            Event_DisplayMapMsgWithAudio(51, &g_Cutscene_MapMsgAudioIdx, g_Cutscene_MapMsgAudioCmds);
            break;

        case 36:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(5.0f), Q12(223.0f), Q12(251.0f), true, false);
            Event_WaitTimer(Q12(0.6f), false);
            break;

        case 37:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(5.0f), Q12(223.0f), Q12(251.0f), true, false);
            Event_DisplayMapMsgWithAudio(55, &g_Cutscene_MapMsgAudioIdx, g_Cutscene_MapMsgAudioCmds);
            break;

        case 38:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &cybilChara, 9, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 0x4C, false);
            SysWork_StateStepIncrement(0);

        case 39:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(252.0f), Q12(280.0f), true, false);
            Event_DisplayMapMsgWithAudio(57, &g_Cutscene_MapMsgAudioIdx, g_Cutscene_MapMsgAudioCmds);
            break;

        case 40:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(252.0f), Q12(280.0f), true, true);
            break;

        case 41:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, g_SysWork.npcs, 5, false);
            SysWork_StateStepIncrement(0);

        case 42:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(281.0f), Q12(296.0f), true, true);
            break;

        case 43:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 51, false);
            g_Cutscene_Timer = Q12(297.0f);
            SysWork_StateStepIncrement(0);

        case 44:
            Event_DisplayMapMsgWithAudio(58, &g_Cutscene_MapMsgAudioIdx, g_Cutscene_MapMsgAudioCmds);
            break;

        case 45:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 90, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &cybilChara, 1, false);
            SysWork_StateStepIncrement(0);

        case 46:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(298.0f), Q12(308.0f), true, true);
            break;

        case 47:
            D_800DE250 = 0;

            Sfx_WithFlagsPlay(Sfx_Unk1357, &Q12_VECTOR3(0.8f, -1.2f, 269.5f), Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 48:
            Chara_ModelCharaIdClear(&cybilChara, 0, 0);
            SD_Call(19);
            Chara_Load(0, Chara_AirScreamer, &g_SysWork.npcBoneCoordBuffer[0], CHARA_FORCE_FREE_ALL, 0, 0);
            Event_InvItemCmd(InvItemCmd_AddItem, InvItemId_Handgun, HANDGUN_AMMO_PICKUP_ITEM_COUNT, false);
            SysWork_StateStepIncrement(0);

        case 49:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 50:
            g_Cutscene_Timer = Q12(308.0f);
            Event_WaitTimer(Q12(0.5f), false);
            break;

        case 51:
            SysWork_StateStepReset();
            break;

        case EventState_Skip:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 53:
            SysWork_StateStepSet(0, 48);
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);

            vcReturnPreAutoCamWork(true);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            SD_Call(23);

            Savegame_EventFlagSet(EventFlag_49);
            g_Cutscene_Timer = NO_VALUE;
            break;
    }

    if (g_Cutscene_Timer >= Q12(0.0f))
    {
        vcChangeProjectionValue(Dms_CameraTargetsGet(&g_Cutscene_CameraPositionTarget, &g_Cutscene_CameraLookAtTarget, NULL, g_Cutscene_Timer, FS_BUFFER_11));
        vcUserCamTarget(&g_Cutscene_CameraPositionTarget, NULL, true);
        vcUserWatchTarget(&g_Cutscene_CameraLookAtTarget, NULL, true);
        Dms_CharacterTransformGet(&g_SysWork.playerWork.player.position, &g_SysWork.playerWork.player.rotation, "HERO", g_Cutscene_Timer, FS_BUFFER_11);

        if (D_800DE250 != 0)
        {
            cybil0                    = &cybilChara;
            cybil0->model.anim.flags |= AnimFlag_Visible;
            Dms_CharacterTransformGet(&cybil0->position, &cybil0->rotation, "SIBYL", g_Cutscene_Timer, FS_BUFFER_11);
            return;
        }

        cybil1 = &cybilChara;
        cybil1->model.anim.flags &= ~AnimFlag_Visible;
    }

    #undef cybilChara
}

void MapEvent_ToBeContinued(void) // 0x800DB790
{
    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            SysWork_StateStepIncrement(0);

        case 1:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 0, Q12(0.0f), false);
            Event_BgTextureCmd(BgTextureCmd_QueueRead, FILE_TIM_CONTINUE_TIM, false);
            SysWork_StateStepIncrement(0);

        case 2:
            Event_ScreenFadeCmd(ScreenFadeCmd_Wait, true, 0, Q12(0.0f), false);
            return;

        case 3:
            Event_BgTextureCmd(BgTextureCmd_AwaitLoad, 0, false);
            return;

        case 4:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(2.0f), false);
            SysWork_StateStepIncrement(0);

        case 5:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            if (g_Controller0->buttonFlags.clicked & (g_GameWorkPtr->config.controllerConfig.enter |
                                                      g_GameWorkPtr->config.controllerConfig.cancel))
            {
                SysWork_StateStepIncrement(0);
            }

            Event_WaitTimer(Q12(4.0f), false);

            if (g_SysWork.sysStateSteps[0] == 7)
            {
                g_SysWork.sysStateSteps[0] = 6;
            }
            break;

        case 7:
            Event_BgTextureCmd(BgTextureCmd_Auto, FILE_TIM_COMESOON_TIM, false);
            break;

        case 8:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(1.5f), false);
            Sd_SfxStop(Sfx_RadioInterferenceLoop);
            Sd_SfxStop(Sfx_RadioStaticLoop);
            SysWork_StateStepIncrement(0);

        case 9:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            if (g_Controller0->buttonFlags.clicked & (g_GameWorkPtr->config.controllerConfig.enter |
                                                 g_GameWorkPtr->config.controllerConfig.cancel))
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

        case 6:
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

const VECTOR3 D_800CCA1C = { Q12(0.8f), Q12(-1.4f), Q12(267.7f) };
const VECTOR3 D_800CCA28 = { Q12(0.3f), Q12(-1.4f), Q12(267.7f) };
const VECTOR3 D_800CCA34 = { Q12(0.8f), Q12(-1.8f), Q12(267.7f) };
const VECTOR3 D_800CCA40 = { Q12(0.8f), Q12(-1.4f), Q12(273.9f) };
const VECTOR3 D_800CCA4C = { Q12(0.3f), Q12(-1.4f), Q12(273.9f) };
const VECTOR3 D_800CCA58 = { Q12(0.8f), Q12(-1.8f), Q12(264.1f) };

void MapEvent_AirScreamerIntroCutscene(void) // 0x800DBAA0
{
    #define playerChara      g_SysWork.playerWork.player
    #define airScreamerChara g_SysWork.npcs[0]

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            func_8003A16C();

            Savegame_EventFlagSet(EventFlag_50);

            SD_Call(Sfx_RadioInterferenceLoop);
            SD_Call(Sfx_RadioStaticLoop);

            Savegame_EventFlagSet(EventFlag_52);

            // Warp player.
            playerChara.position.vx = Q12(1.3f);
            playerChara.position.vz = Q12(269.7f);
            playerChara.rotation.vy = Q12_ANGLE(5.0f);

            g_Cutscene_Timer = Q12(26.0f);

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 3, Q12(0.0f), false);

            g_Cutscene_MapMsgAudioIdx = 0;
            SysWork_StateStepIncrement(0);

        case 1:
            Bgm_CrossfadeToTrack(BgmCmd_Track30);
            break;

        case 2:
            Event_WaitPlayerStop();
            break;

        case 3:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &playerChara, 91, 0);
            SysWork_StateStepIncrement(0);

        case 4:
            g_Cutscene_Timer = MIN(g_Cutscene_Timer + Q12_MULT_PRECISE(g_DeltaTime, Q12(15.0f)), Q12(37.0f));
            if (g_Cutscene_Timer >= Q12(37.0f))
            {
                SysWork_StateStepIncrement(0);
            }
            break;

        case 5:
            Event_DisplayMapMsgWithAudio(62, &g_Cutscene_MapMsgAudioIdx, &D_800DE124); // "What's that?"
            break;

        case 6:
            Event_WaitTimer(Q12(0.5f), false);
            break;

        case 7:
            Event_DisplayMapMsgWithAudio(63, &g_Cutscene_MapMsgAudioIdx, &D_800DE124); // "Huh? Radio?"
            break;

        case 8:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &playerChara, 92, false);
            SysWork_StateStepIncrement(0);

        case 9:
            g_Cutscene_Timer = MIN(g_Cutscene_Timer + Q12_MULT_PRECISE(g_DeltaTime, Q12(15.0f)), Q12(52.0f));
            if (g_Cutscene_Timer >= Q12(52.0f))
            {
                SysWork_StateStepIncrement(0);
            }
            break;

        case 10:
            g_Cutscene_Timer = MAX(g_Cutscene_Timer, Q12(53.0f));
            D_800E23A1 = 0x80;
            Savegame_EventFlagSet(EventFlag_M0S01_AirScreamerIntroCutsceneStarted);

            Event_WaitTimer(Q12(1.5f), false);
            break;

        case 11:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &playerChara, 53, false);
            SysWork_StateStepIncrement(0);

        case 12:
            g_Cutscene_Timer = MIN(g_Cutscene_Timer + Q12_MULT_PRECISE(g_DeltaTime, Q12(15.0f)), Q12(75.0f));
            if (g_Cutscene_Timer >= Q12(75.0f))
            {
                SysWork_StateStepIncrement(0);
            }
            break;

        case 13:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &playerChara, 89, false);

            D_800E23A1 = 0xE0;
            Savegame_EventFlagSet(EventFlag_46);
            g_Cutscene_Timer = Q12(76.0f);

            SysWork_StateStepIncrement(0);

        case 14:
            g_Cutscene_Timer = MIN(g_Cutscene_Timer + Q12_MULT_PRECISE(g_DeltaTime, Q12(18.0f)), Q12(115.0f));
            if (g_Cutscene_Timer >= Q12(115.0f))
            {
                SysWork_StateStepIncrement(0);
            }
            break;

        case 15:
            sharedFunc_800CCB8C_0_s01(&D_800CCA1C, &D_800CCA28, Q12(2.5f), Q12(0.05f), Q12(0.6f), Q12(0.4f), Q12(0.1f), 0);
            Sfx_WithFlagsPlay(Sfx_Unk1373, &D_800CCA34, Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 16:
            g_Cutscene_Timer = MIN(g_Cutscene_Timer + Q12_MULT_PRECISE(g_DeltaTime, Q12(18.0f)), Q12(146.0f));
            if (g_Cutscene_Timer >= Q12(146.0f))
            {
                SysWork_StateStepIncrement(0);
            }
            break;

        case 17:
            sharedFunc_800CCB8C_0_s01(&D_800CCA40, &D_800CCA4C, Q12(3.0f), Q12(0.08f), Q12(0.4f), Q12(0.4f), Q12(0.058f), 0);
            Sfx_WithFlagsPlay(Sfx_Unk1374, &D_800CCA58, Q8(0.5f), SfxFlag_None);
            Sfx_WithFlagsPlay(Sfx_Unk1375, &D_800CCA58, Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 18:
            g_Cutscene_Timer = MIN(g_Cutscene_Timer + Q12_MULT_PRECISE(g_DeltaTime, Q12(18.0f)), Q12(147.0f));
            if (g_Cutscene_Timer >= Q12(147.0f))
            {
                SysWork_StateStepIncrement(0);
            }
            break;

        case 19:
            func_800D3AC0(&airScreamerChara);
            SysWork_StateStepIncrement(0);

        case 20:
            g_Cutscene_Timer = MIN(g_Cutscene_Timer + Q12_MULT_PRECISE(g_DeltaTime, Q12(18.0f)), Q12(154.0f));
            if (g_Cutscene_Timer >= Q12(154.0f))
            {
                SysWork_StateStepIncrement(0);
            }
            break;

        case 21:
            Savegame_EventFlagSet(EventFlag_47);
            func_800D39F4(&airScreamerChara);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);

            g_Cutscene_Timer = NO_VALUE;

            vcReturnPreAutoCamWork(true);

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            Player_FallBackward();

            g_SysWork.sysFlags &= ~SysFlag_5;
            g_SysWork.sysFlags &= ~SysFlag_9;
            break;
    }

    if (g_Cutscene_Timer >= Q12(0.0f))
    {
        Dms_CharacterTransformGet(&playerChara.position, &playerChara.rotation, "HERO", g_Cutscene_Timer, FS_BUFFER_11);

        if (g_SysWork.sysStateSteps[0] >= 20)
        {
            Dms_CharacterTransformGet(&airScreamerChara.position, &airScreamerChara.rotation, "BIRD", g_Cutscene_Timer, FS_BUFFER_11);
            airScreamerChara.position.vx += Q12_MULT(Math_Sin(airScreamerChara.rotation.vy), Q12(0.2f));
            airScreamerChara.position.vz += Q12_MULT(Math_Cos(airScreamerChara.rotation.vy), Q12(0.2f));
        }

        vcChangeProjectionValue(Dms_CameraTargetsGet(&g_Cutscene_CameraPositionTarget, &g_Cutscene_CameraLookAtTarget, NULL, g_Cutscene_Timer, FS_BUFFER_11));
        vcUserCamTarget(&g_Cutscene_CameraPositionTarget, NULL, true);
        vcUserWatchTarget(&g_Cutscene_CameraLookAtTarget, NULL, true);
    }

    #undef playerChara
    #undef airScreamerChara
}

void MapEvent_PocketRadioItemTake(void) // 0x800DC34C
{
    Event_ItemTake(InvItemId_PocketRadio, DEFAULT_PICKUP_ITEM_COUNT, EventFlag_M0S01_PickupPocketRadio, 64);
    g_SavegamePtr->itemToggleFlags |= ItemToggleFlag_RadioOn;
}

void MapEvent_FlashlightItemTake(void) // 0x800DC394
{
    Event_ItemTake(InvItemId_Flashlight, DEFAULT_PICKUP_ITEM_COUNT, EventFlag_M0S01_PickupFlashlight, 61);
    Game_TurnFlashlightOff();
}

void MapEvent_MapItemTake(void) // 0x800DC3C8
{
    static const RECT IMG_BUFFER_RECT = { 320, 256, 160, 240 };

    #define playerChara      g_SysWork.playerWork.player
    #define airScreamerChara g_SysWork.npcs[0]

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Fs_QueueStartSeek(FILE_TIM_MP_0TOWN_TIM + D_800A99B5);
            SysWork_StateStepIncrement(0);

        case 1:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 2:
            DrawSync(SyncMode_Wait);
            StoreImage(&IMG_BUFFER_RECT, IMAGE_BUFFER);
            DrawSync(SyncMode_Wait);
            Fs_QueueStartReadTim(FILE_TIM_MP_0TOWN_TIM + D_800A99B5, FS_BUFFER_2, &g_PaperMapImg);
            Screen_Init(SCREEN_WIDTH, true);

            g_IntervalVBlanks = 1;

            GsSwapDispBuff();
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            Fs_QueueWaitForEmpty();
            SysWork_StateStepIncrement(0);

        case 3:
            g_Screen_BackgroundImgGamma = Q8(11.0f / 32.0f);

            Screen_BackgroundImgDraw(&g_PaperMapImg);
            Event_DisplayMapMsg(true, 59, 6, 4, 0, false);
            break;

        case 5:
            LoadImage(&IMG_BUFFER_RECT, IMAGE_BUFFER);
            DrawSync(SyncMode_Wait);
            Screen_Init(SCREEN_WIDTH, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            SysWork_StateStepReset();
            break;

        case 6:
            g_SavegamePtr->paperMapFlags |= 1 << 1;

            Fs_QueueStartRead(FILE_ANIM_CAFE2_DMS, FS_BUFFER_11);
            SysWork_StateStepIncrement(0);

        case 4:
        case 7:
            g_Screen_BackgroundImgGamma = Q8(11.0f / 32.0f);

            Screen_BackgroundImgDraw(&g_PaperMapImg);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 8:
            LoadImage(&IMG_BUFFER_RECT, IMAGE_BUFFER);
            DrawSync(SyncMode_Wait);
            Screen_Init(SCREEN_WIDTH, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            SysWork_StateStepIncrement(0);

        case 9:
            Chara_ProcessLoads();
            SysWork_StateStepIncrement(0);

        case 10:
            // Set cutscene character.
            Chara_Spawn(Chara_AirScreamer, 0, Q12(0.0f), Q12(0.0f), Q12_ANGLE(0.0f), 12);
            Dms_HeaderFixOffsets(FS_BUFFER_11);
            Dms_CharacterTransformGet(&airScreamerChara.position, &airScreamerChara.rotation, "BIRD", 0, FS_BUFFER_11);

            // Set camera.
            vcChangeProjectionValue(Dms_CameraTargetsGet(&g_Cutscene_CameraPositionTarget, &g_Cutscene_CameraLookAtTarget, NULL, 0, FS_BUFFER_11));
            vcUserCamTarget(&g_Cutscene_CameraPositionTarget, NULL, true);
            vcUserWatchTarget(&g_Cutscene_CameraLookAtTarget, NULL, true);

            // Warp player.
            playerChara.position.vx = Q12(4.585938f);
            playerChara.position.vz = Q12(267.285156f);
            playerChara.rotation.vy = Q12_ANGLE(90.0f);

            // Set flags.
            SysWork_NpcFlagSet(0);
            g_SysWork.field_228C[0] |= 1 << 0;
            Savegame_EventFlagSet(EventFlag_M0S01_PickupMap);

            SysWork_StateStepIncrement(0);

        case 11:
            Event_ScreenFadeCmd(ScreenFadeCmd_Wait, false, 0, Q12(0.0f), false);
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;
    }

    #undef playerChara
    #undef airScreamerChara
}

void MapEvent_KitchenKnifeItemTake(void) // 0x800DC830
{
    Event_ItemTake(InvItemId_KitchenKnife, DEFAULT_PICKUP_ITEM_COUNT, EventFlag_M0S01_PickupKitchenKnife, 60);
}

/** Presumably a pickup function. Maybe debug or something unused and commented out. */
void MapEvent_EmptyFunction(void) {} // 0x800DC85C

void MapEvent_CommonItemTake(void) // 0x800DC864
{
    s32 eventFlagIdx;
    u32 pickupType;

    pickupType   = CommonPickupItemId_FirstAidKit;
    eventFlagIdx = 0;

    switch (g_MapEventData->mapPointIdx)
    {
        case 13:
            pickupType   = CommonPickupItemId_HealthDrink;
            eventFlagIdx = EventFlag_M0S01_HealthDrink0;
            break;

        case 14:
            pickupType   = CommonPickupItemId_HealthDrink;
            eventFlagIdx = EventFlag_M0S01_HealthDrink1;
            break;
    }

    Event_CommonItemTake(pickupType, eventFlagIdx);
}

void MapEvent_AirScreamerDeath(void) // 0x800DC8C8
{
    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Sd_SfxStop(Sfx_RadioInterferenceLoop);
            Sd_SfxStop(Sfx_RadioStaticLoop);

            Savegame_EventFlagClear(EventFlag_52);
            g_Cutscene_MapMsgAudioIdx = 0;
            SysWork_StateStepIncrement(0);

        case 1:
            Event_DisplayMapMsgWithAudio(68, &g_Cutscene_MapMsgAudioIdx, &D_800DE128); // "This is not a dream!"
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);

            Savegame_EventFlagSet(EventFlag_M0S01_AirScreamerDeathPlayed);
            break;
    }
}

void Map_WorldObjectsInit(void) // 0x800DC9C8
{
    D_800E23A1 = 224;
    Math_Vector3Set(&g_CutscenePosition, Q12(0.0f), Q12(0.0f), Q12(280.0f));

    WorldObject_ModelNameSet(&D_800E23B0[0], "IN_BLD1_");
    WorldObject_ModelNameSet(&D_800E23D0[0], "IN1_HIDE");
    WorldObject_ModelNameSet(&D_800E23F0[0], "BRK_BLD1");
    WorldObject_ModelNameSet(&D_800E23F0[1], "BRK_GLS1");
    WorldObject_ModelNameSet(&D_800E23F0[2], "BRK_GLS2");
    WorldObject_ModelNameSet(&D_800E2450[0], "OUT1_HID");
    WorldObject_ModelNameSet(&D_800E2450[1], "OUT_BLD_");

    WorldObject_Init(&g_WorldObject_KitchenKnife, "KNIFE_HI", 5.09f, -1.01f, 274.12f,  0.0f, 32.0f,  0.0f);
    WorldObject_Init(&g_WorldObject_Flashlight,   "FLASH_HI", 5.18f, -1.0f,  267.28f, -3.0f, 111.0f, 0.0f);
    WorldObject_Init(&g_WorldObject_Map,          "MAP_HIDE", 5.24f, -1.0f,  267.21f,  0.0f, 57.0f,  0.0f);
    WorldObject_Init(&g_WorldObject_PocketRadio,  "RADIO_HI", 2.31f, -0.6f,  273.95f,  0.0f, 147.0f, 0.0f);

    if (Savegame_EventFlagGet(EventFlag_M0S01_CafeCutsceneStarted) && !Savegame_EventFlagGet(EventFlag_47))
    {
        g_SysWork.sysFlags |= SysFlag_5 | SysFlag_9;

        Chara_Load(0, Chara_AirScreamer, &g_SysWork.npcBoneCoordBuffer[0], CHARA_FORCE_FREE_ALL, NULL, NULL);

        if (Savegame_EventFlagGet(EventFlag_M0S01_PickupMap))
        {
            Chara_ProcessLoads();
            Chara_Spawn(Chara_AirScreamer, 0, Q12(1048566.0f), Q12(280.0f), Q12_ANGLE(0.0f), 12);
            func_800D3A3C(&g_SysWork.npcs[0]);

            Fs_QueueStartRead(FILE_ANIM_CAFE2_DMS, FS_BUFFER_11);
            Fs_QueueWaitForEmpty();
            Dms_HeaderFixOffsets((s_DmsHeader*)FS_BUFFER_11);
        }
    }

    if (Savegame_EventFlagGet(EventFlag_159) && !Savegame_EventFlagGet(EventFlag_134))
    {
        Gfx_MapEnvSet(6, 3);
    }

    g_Cutscene_Timer = Q12(0.0f);
    D_800E2560 = Q12(0.0f);

    WorldObject_ModelNameSet(&g_CommonWorldObjects[0], g_CommonWorldObjectNames[CommonPickupItemId_Ampoule]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[1], g_CommonWorldObjectNames[3]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[2], g_CommonWorldObjectNames[4]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[3], g_CommonWorldObjectNames[5]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[4], g_CommonWorldObjectNames[6]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[5], g_CommonWorldObjectNames[7]);
}

void Map_WorldObjectsUpdate(void) // 0x800DCCF4
{
    MAP_CHUNK_CHECK_VARIABLE_DECL();
    s32  temp_a1;
    s32  var_a2;
    s32* ptr;

    #define playerChara      g_SysWork.playerWork.player
    #define airScreamerChara g_SysWork.npcs[0]

    if (!Savegame_EventFlagGet(EventFlag_M0S01_PickupMap))
    {
        WorldObjects_Add(&g_WorldObject_Map.object, &g_WorldObject_Map.position, &g_WorldObject_Map.rotation);
    }

    if (!Savegame_EventFlagGet(EventFlag_M0S01_PickupFlashlight))
    {
        WorldObjects_Add(&g_WorldObject_Flashlight.object, &g_WorldObject_Flashlight.position, &g_WorldObject_Flashlight.rotation);
    }

    if (!Savegame_EventFlagGet(EventFlag_M0S01_PickupPocketRadio))
    {
        WorldObjects_Add(&g_WorldObject_PocketRadio.object, &g_WorldObject_PocketRadio.position, &g_WorldObject_PocketRadio.rotation);
    }

    if (!Savegame_EventFlagGet(EventFlag_M0S01_PickupKitchenKnife))
    {
        WorldObjects_Add(&g_WorldObject_KitchenKnife.object, &g_WorldObject_KitchenKnife.position, &g_WorldObject_KitchenKnife.rotation);
    }

    if (Savegame_EventFlagGet(EventFlag_M0S01_PickupKitchenKnife) && Savegame_EventFlagGet(EventFlag_M0S01_PickupMap))
    {
        Savegame_EventFlagSet(EventFlag_M0S01_PickedUpKnifeAndMap);
    }
    else
    {
        Savegame_EventFlagClear(EventFlag_M0S01_PickedUpKnifeAndMap);
    }

    if (Savegame_EventFlagGet(EventFlag_47))
    {
        CutsceneObjects_Add(2);
    }
    else if (Savegame_EventFlagGet(EventFlag_M0S01_AirScreamerIntroCutsceneStarted) && !Savegame_EventFlagGet(EventFlag_46))
    {
        CutsceneObjects_Add(1);
    }
    else
    {
        CutsceneObjects_Add(0);
    }

    if (Savegame_EventFlagGet(EventFlag_M0S01_PickupMap) && !Savegame_EventFlagGet(EventFlag_42))
    {
        if (Savegame_EventFlagGet(EventFlag_41))
        {
            temp_a1 = g_Cutscene_Timer + Q12_MULT_PRECISE(g_DeltaTime, Model_AnimDurationGet(&airScreamerChara.model));

            ptr = &g_Cutscene_Timer;
            if (temp_a1 < Q12(25.0f))
            {
                var_a2 = g_Cutscene_Timer + Q12_MULT_PRECISE(g_DeltaTime, Model_AnimDurationGet(&airScreamerChara.model));
            }
            else
            {
                var_a2 = Q12(25.0f);
            }
            *ptr = var_a2;

            Dms_CharacterTransformGet(&airScreamerChara.position, &airScreamerChara.rotation, "BIRD",
                                      g_Cutscene_Timer, (s_DmsHeader*)FS_BUFFER_11);

            if (g_Cutscene_Timer >= Q12(25.0f) ||
                ABS(playerChara.position.vx - Q12(4.586f)) > Q12(0.7f) ||
                ABS(playerChara.position.vz - Q12(267.2852f)) > Q12(0.7f))
            {
                Savegame_EventFlagSet(EventFlag_42);
                vcReturnPreAutoCamWork(true);

                airScreamerChara.position.vy = Q12(-10.0f);
                airScreamerChara.position.vz = Q12(280.0f);
            }
        }
        else if (playerChara.position.vx != Q12(4.586f) ||
                 playerChara.position.vz != Q12(267.2852f) ||
                 playerChara.rotation.vy != Q12_ANGLE(90.0f) ||
                 D_800E2560 > Q12(7.5f))
        {
            Savegame_EventFlagSet(EventFlag_41);
            func_800D3AC0(&airScreamerChara);
        }
        else
        {
            D_800E2560 += g_DeltaTime;
        }
    }

    if (Savegame_EventFlagGet(EventFlag_47))
    {
        if (airScreamerChara.model.charaId == Chara_AirScreamer && airScreamerChara.health == NO_VALUE)
        {
            Savegame_EventFlagSet(EventFlag_M0S01_AirScreamerDied);
            Savegame_EventFlagClear(EventFlag_49);
            Savegame_EventFlagClear(EventFlag_50);
        }
    }

    if (Savegame_EventFlagGet(EventFlag_52))
    {
        if (g_SysWork.bgmStatusFlags & BgmStatusFlag_6)
        {
            SD_Call(Sfx_RadioInterferenceLoop);
            SD_Call(Sfx_RadioStaticLoop);
        }

        Sfx_WithFalloffAndPitchPlay(Sfx_RadioInterferenceLoop, &D_800DE154, D_800E23A1, Q12(16.0f), 0);
        Sfx_WithFalloffAndPitchPlay(Sfx_RadioStaticLoop, &D_800DE154, D_800E23A1, Q12(16.0f), 0);

        Savegame_EventFlagSet(EventFlag_52);
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 0, 0, -1, 1) && PLAYER_IN_MAP_CHUNK(vz, 1, 7, -1, 7))
    {
        if (!Savegame_EventFlagGet(EventFlag_M0S01_HealthDrink0))
        {
            WorldObjects_Add(&g_CommonWorldObjects[1], &D_800DE12C.position, &D_800DE12C.rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 0, 0, -1, 1) && PLAYER_IN_MAP_CHUNK(vz, 1, 7, -1, 7))
    {
        if (!Savegame_EventFlagGet(EventFlag_M0S01_HealthDrink1))
        {
            WorldObjects_Add(&g_CommonWorldObjects[1], &D_800DE140.position, &D_800DE140.rotation);
        }
    }

    #undef playerChara
    #undef airScreamerChara
}

void CutsceneObjects_Add(s32 arg0) // 0x800DD2EC
{
    static const SVECTOR3 DEFAULT_ROT = {};

    s32 i;

    if (arg0 == 0)
    {
        for (i = 0; i < ARRAY_SIZE(D_800E23D0); i++)
        {
            WorldObjects_Add(&D_800E23D0[i], &g_CutscenePosition, &DEFAULT_ROT);
        }

        for (i = 0; i < ARRAY_SIZE(D_800E23B0); i++)
        {
            WorldObjects_Add(&D_800E23B0[i], &g_CutscenePosition, &DEFAULT_ROT);
        }

        return;
    }

    if (arg0 == 1)
    {
        for (i = 0; i < ARRAY_SIZE(D_800E2450); i++)
        {
            WorldObjects_Add(&D_800E2450[i], &g_CutscenePosition, &DEFAULT_ROT);
        }

        return;
    }

    for (i = 0; i < ARRAY_SIZE(D_800E23D0); i++)
    {
        WorldObjects_Add(&D_800E23D0[i], &g_CutscenePosition, &DEFAULT_ROT);
    }

    for (i = 0; i < ARRAY_SIZE(D_800E23F0); i++)
    {
        WorldObjects_Add(&D_800E23F0[i], &g_CutscenePosition, &DEFAULT_ROT);
    }
}
