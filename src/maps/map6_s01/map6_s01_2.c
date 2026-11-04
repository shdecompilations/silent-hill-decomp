#include "bodyprog/bodyprog.h"
#include "bodyprog/chara/dms.h"
#include "bodyprog/events/event_data.h"
#include "bodyprog/gfx/map_effects.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player/player.h"
#include "bodyprog/sound/sound_system.h"
#include "main/rng.h"
#include "maps/map6/map6_s01.h"
#include "maps/common/characters/cybil.h"
#include "maps/common/characters/dahlia.h"
#include "maps/common/particle.h"
#include "maps/common/characters/player.h"

#include "../src/maps/common/unk_draw_m1s03.c"

#include "../src/maps/common/particles/particle.c"

#include "../src/maps/common/characters/player.c"

#include "../src/maps/common/characters/cybil.c"
#include "../src/maps/common/characters/dahlia.c"

#include "../src/maps/common/chara_util.c" // 0x800D0358

#include "../src/maps/common/map_util.c" // 0x800D0DD0

#include "maps/common/shared/Map_RoomBgmInit_CheckCond.h" // 0x800D0E78

void Map_RoomBgmInit_CondTrue(void) // 0x800D0EAC
{
    if (!Savegame_EventFlagGet(EventFlag_424) || (Savegame_EventFlagGet(EventFlag_425) && !Savegame_EventFlagGet(EventFlag_426)))
    {
        Bgm_SongChange(BgmCmd_Track23);
    }
    else
    {
        Bgm_SongChange(BgmCmd_Track20);
    }
}

#include "maps/common/shared/Map_RoomBgmInit_0_s02_CondFalse.h" // 0x800D0EF8

void GameBoot_LoadScreen_StageString(void) {}

#include "maps/common/shared/MapEvent_DoorJammed.h" // 0x800D118C

#include "maps/common/shared/MapEvent_DoorLocked.h" // 0x800D1220

const char* MAP_MESSAGES[] = {
    #include "maps/common/shared/map_msg_common.h"
#if VERSION_REGION_IS(NTSC)
    /* 15 */ "~J0(1.0)\tCybil! ~E ",
    /* 16 */ "~J1(1.1)\tHarry! ",
    /* 17 */ "~J1(1.6)\tHow_did_you_get_back_here?  ",
    /* 18 */ "~J1(1.6)\tI_followed_the_sewer. ",
    /* 19 */ "~J1(1.9)\tWere_you_the_one ~N\n\t\t\twho_cut_the_fence? ",
    /* 20 */ "~J1(1.0)\tYeah. ",
    /* 21 */ "~J1(3.0)\tI'm_glad_you_made_it. ~N\n\t\t\tI_was_worried_about_you. ~E ",
    /* 22 */ "~J1(3.6)\tYou_were_worried!? ~N\n\t\t\tWhere_did_you_disappear_to? ",
    /* 23 */ "~J1(1.0)\tNever_mind. ",
    /* 24 */ "~J1(2.1)\tI_want_to_know ~N\n\t\t\twhat's_going_on_here. ",
    /* 25 */ "~J1(2.4)\tWhat_is_with_this_town?  ~E ",
    /* 26 */ "~J1(3.5)\tThis_may_sound_really_off ~N\n\t\t\tthe_wall,_but_listen_to_me. ",
    /* 27 */ "~J1(1.8)\tYou've_got_to_believe_me. ",
    /* 28 */ "~J1(3.2)\tI_haven't_gone_crazy, ~N\n\t\t\tand_I'm_not_fooling_around. ~E ",
    /* 29 */ "~J1(3.7)\tAt_first,_I_thought_I_was ~N\n\t\t\tlosing_my_mind. ",
    /* 30 */ "~J1(2.3)\tBut_now_I_know_I'm_not. ",
    /* 31 */ "~J1(3.0)\tIt's_not_me. ~N\n\t\t\tThis_whole_town... ",
    /* 32 */ "~J1(2.5)\tit's_being_invaded ~N\n\t\t\tby_the_Otherworld. ",
    /* 33 */ "~J1(4.0)\tBy_a_world_of_someone's ~N\n\t\t\tnightmarish_delusions ~N\n\t\t\tcome_to_life... ",
    /* 34 */ "~J1(2.9)\tLittle_by_little ~N\n\t\t\tthe_invasion_is_spreading... ",
    /* 35 */ "~J1(2.8)\tTrying_to_swallow_up ~N\n\t\t\teverything_in_darkness. ",
    /* 36 */ "~J1(4.2)\tI_think_I'm_finally_beginning ~N\n\t\t\tto_understand_what_that_lady ~N\n\t\t\twas_talking_about. ~E ",
    /* 37 */ "~J1(4.6)\tHarry. ~N\n\t\t\tHold_on_a_minute. ~N\n\t\t\tI_don't_get_it? ",
    /* 38 */ "~J1(3.1)\tLook,_I_don't_understand ~N\n\t\t\tit_all_myself. ",
    /* 39 */ "~J1(1.8)\tI_guess_I_can't_explain_it. ",
    /* 40 */ "~J1(2.2)\tWell,_what's_making ~N\n\t\t\tthis_happen? ",
    /* 41 */ "~J1(1.6)\tI_don't_know_that_either. ",
    /* 42 */ "~J1(2.8)\tBut,_I_do_know ~N\n\t\t\tCheryl_is_[there]. ",
    /* 43 */ "~J1(1.0)\t[There]...? ",
    /* 44 */ "~J1(3.1)\tUnder_whoever_created ~N\n\t\t\tthis_darkness. ",
    /* 45 */ "~J1(3.2)\tCheryl_is_somewhere. ~N\n\t\t\tAnd_she_needs_my_help. ~E ",
    /* 46 */ "~J1(1.0)\tHarry. ",
    /* 47 */ "~J1(2.8)\tThis_whole_thing's_been ~N\n\t\t\ta_major_blow_to_you. ",
    /* 48 */ "~J1(1.8)\tYou_need_to_rest. ",
    /* 49 */ "~J1(1.5)\tCybil,_I... ~E ",
    /* 50 */ "~J1(5.2)\tThe_demon_is_awakening! ~N\n\t\t\tSpreading_those_wings. ",
    /* 51 */ "~J1(1.7)\tDahlia_Gillespie... ~E ",
    /* 52 */ "~J1(2.9)\tWas_it_not_as_I_said? ",
    /* 53 */ "~J1(4.4)\tI_see_it_all,_now. ~N\n\t\t\tYes,_everything. ",
    /* 54 */ "~J1(4.9)\tHungry_for_sacrifice, ~N\n\t\t\tthe_demon_will_swallow_up ~N\n\t\t\tthe_land. ",
    /* 55 */ "~J1(3.1)\tI_knew_this_day_would_come. ",
    /* 56 */ "~J1(3.9)\tAnd_what's_more, ~N\n\t\t\tthe_task_is_almost_finished. ",
    /* 57 */ "~J1(2.5)\tThere's_only_two_left. ",
    /* 58 */ "~J1(5.3)\tTo_seal_this_town_to_the_abyss, ~N\n\t\t\tthe_mark_of_Samael. ",
    /* 59 */ "~J1(3.6)\tWhen_it_is_completed,_all_is_lost. ",
    /* 60 */ "~J1(4.6)\tEven_in_daytime, ~N\n\t\t\tdarkness_will_cover_the_sun. ",
    /* 61 */ "~J1(5.8)\tThe_dead_will_walk, ~N\n\t\t\tand_martyrs_will_burn ~N\n\t\t\tin_the_fires_of_hell. ",
    /* 62 */ "~J1(3.1)\tEveryone_will_die. ~E ",
    /* 63 */ "~J0(3.5)\tSo_what_am_I_supposed_to_do? ~N\n\t\t\tI've_got_to_save_Cheryl. ",
    /* 64 */ "~J1(3.6)\tIt_is_simple. ~N\n\t\t\tStop_the_demon. ",
    /* 65 */ "~J1(4.6)\tThe_demon, ~N\n\t\t\tthe_demon_taking_that ~N\n\t\t\tchild's_form. ",
    /* 66 */ "~J1(3.7)\tStop_it_before_your_daughter ~N\n\t\t\tbecomes_a_sacrifice. ",
    /* 67 */ "~J1(2.6)\tBefore_it_is_too_late. ",
    /* 68 */ "~J1(3.0)\tStop_it._Stop_it. ",
    /* 69 */ "~J1(1.6)\tWhat_do_I_do? ",
    /* 70 */ "~J1(3.0)\tGo_to_the_lighthouse ~N\n\t\t\ton_the_lake, ",
    /* 71 */ "~J1(3.5)\tand_to_the_center_of ~N\n\t\t\tthe_amusement_park. ",
    /* 72 */ "~J1(3.5)\tMake_haste. ~N\n\t\t\tYou_are_the_only_hope. ~E ",
    /* 73 */ "~J0(1.1)\tLook_Harry. ",
    /* 74 */ "~J0(2.1)\tI_really_don't_get ~N\n\t\t\twhat's_going_on. ",
    /* 75 */ "~J0(4.4)\tBut_if_there's_a_chance ~N\n\t\t\twe_can_save_your_daughter, ~N\n\t\t\tI'm_in. ",
    /* 76 */ "~J0(1.9)\tI'll_check_out ~N\n\t\t\tthe_amusement_park. ",
    /* 77 */ "~J0(1.7)\tYou_go_to_the_lighthouse. ~E ",
    /* 78 */ "~J0(2.5)\tCybil..._Thanks. ~E ",
    /* 79 */ "~J0(2.1)\tYou_will_need_to_use_it. ~E ",
    /* 80 */ "~J0(1.2)\tUse_what? ",
    /* 81 */ "~J0(1.7)\tThe_Flauros. ",
    /* 82 */ "~J0(3.2)\tOnly_with_that_can_you ~N\n\t\t\tstop_it. ~E ",
    /* 83 */ "~J0(2.0)\tWhat_about_Cybil!? ~E ",
    /* 84 */ "~J0(2.0)\tCybil_hasn't_come_back. ~E ",
    /* 85 */ "~J0(3.5)\tThat_creep's_sure_to_show_up ~N\n\t\t\tat_the_Amusement_Park ~N\n\t\t\tpretty_soon. ",
    /* 86 */ "~J0(2.0)\tLet_me_be_on_time. ~E ",
    /* 87 */ "\tThere's_a_ship's_wheel. ~N\n\tDon't_worry_about_that_now, ~N\n\tthough. ~E ",
    /* 88 */ "\tThere's_a_body_stuck ~N\n\tto_the_bulkhead. ~E ",
    /* 89 */ "~J0(1.0)\tWhat? ~E ",
    /* 90 */ "~J0(2.5)\tThat_light!? ~E "
#elif VERSION_REGION_IS(NTSCJ)
    /* 15 */ "~J0(1.0)\tシビル！ ~E ",
    /* 16 */ "~J1(1.1)\tハリー ",
    /* 17 */ "~J1(1.6)\tどうやってここに？ ",
    /* 18 */ "~J1(1.6)\t下水道を通ってきたの ",
    /* 19 */ "~J1(1.9)\tあそこを開けたのはあなた？ ",
    /* 20 */ "~J1(1.0)\tああ ",
    /* 21 */ "~J1(3.0)\tとにかく良かった ~N\n\t\t\t心配していたんだ ~E ",
    /* 22 */ "~J1(3.6)\tこっちの台詞よ ~N\n\t\t\tどこに消えていたの？ ",
    /* 23 */ "~J1(1.4)\tいえ　今はそんなことより…… ",
    /* 24 */ "~J1(2.1)\tこのありさまはいったい何なの？ ",
    /* 25 */ "~J1(2.4)\tこの町はどうなってしまったというの？ ~E ",
    /* 26 */ "~J1(3.5)\tわかりにくい話かもしれないが聞いてくれ ",
    /* 27 */ "~J1(1.8)\tただ……信じてほしいんだ ",
    /* 28 */ "~J1(3.2)\tわたしは狂っているわけでも ~N\n\t\t\tきみをからかうつもりでもない ~E ",
    /* 29 */ "~J1(3.7)\t一時はわたしの頭が ~N\n\t\t\tおかしくなったのかとも思ったよ ",
    /* 30 */ "~J1(2.3)\tだけど　違うんだ ",
    /* 31 */ "~J1(3.0)\tわたしだけじゃない ~N\n\t\t\tこの町すべてが…… ",
    /* 32 */ "~J1(2.5)\t侵蝕されているんだ ~N\n\t\t\tもう一つの世界に ",
    /* 33 */ "~J1(4.0)\t誰かが妄想した悪夢が ~N\n\t\t\t形を成したような世界に…… ",
    /* 34 */ "~J1(2.9)\tそしてその侵蝕は少しずつ拡がっている…… ",
    /* 35 */ "~J1(2.8)\tすべてがその闇にのまれようとしている ",
    /* 36 */ "~J1(4.2)\tあの女の言っていたことが ~N\n\t\t\tわかりかけてきたような気がする ~E ",
    /* 37 */ "~J1(4.6)\tハリー？ ~N\n\t\t\t何の話だか　全然わからないわ…… ",
    /* 38 */ "~J1(3.1)\tわたしもすべてわかってるわけじゃない ",
    /* 39 */ "~J1(1.8)\tはっきりとは説明はできないんだ ",
    /* 40 */ "~J1(2.2)\t結局　町がこうなった原因は何なの？ ",
    /* 41 */ "~J1(1.6)\tそれもわからない ",
    /* 42 */ "~J1(3.5)\tわたしにわかってるのは　あとは ~N\n\t\t\tシェリルがそこにいるということだけだ ",
    /* 43 */ "~J1(1.0)\tそこ？ ",
    /* 44 */ "~J1(3.1)\tこの闇を生み出した誰かの下だ ",
    /* 45 */ "~J1(3.2)\tシェリルはそこにいる ~N\n\t\t\tそしてわたしに助けを求めている ~E ",
    /* 46 */ "~J1(1.0)\tハリー ",
    /* 47 */ "~J1(2.8)\tあなたはずっと無理しすぎよ ",
    /* 48 */ "~J1(1.8)\t少し休むといいわ ",
    /* 49 */ "~J1(1.5)\tシビル　わたしは…… ~E ",
    /* 50 */ "~J1(5.2)\t悪魔が目覚めかけているわ！ ~N\n\t\t\tその翼を拡げ始めたのよ ",
    /* 51 */ "~J1(1.7)\tダリア・ギレスピー…… ~E ",
    /* 52 */ "~J1(2.9)\tあたしの言った通りだったでしょう？ ",
    /* 53 */ "~J1(4.4)\tあたしはずっと見ていたわ ~N\n\t\t\tそう　すべてを ",
    /* 54 */ "~J1(4.9)\tあの悪魔は地に潜み ~N\n\t\t\t生けにえをむさぼり ",
    /* 55 */ "~J1(3.1)\tこの日が来るのを待っていた ",
    /* 56 */ "~J1(3.9)\tそして　ついにそれが ~N\n\t\t\t成就してしまおうとしているわ ",
    /* 57 */ "~J1(2.5)\tあと　残りは２つ ",
    /* 58 */ "~J1(5.3)\tこの町と　かの深淵とを ~N\n\t\t\t結びつけるサマエルの印章 ",
    /* 59 */ "~J1(3.6)\tすべてが完成したときが終わりの時よ ",
    /* 60 */ "~J1(4.6)\t昼間でも闇が太陽に取って代わり ",
    /* 61 */ "~J1(5.8)\t死人が歩き　殉教者は業火に焼かれるのよ ",
    /* 62 */ "~J1(3.1)\tみんな死ぬのよ ~E  ",
    /* 63 */ "~J0(3.5)\tどうすればいいんだ？ ~N\n\t\t\tどうすればシェリルを助けられる？ ",
    /* 64 */ "~J1(3.6)\t単純なことよ ~N\n\t\t\tあの悪魔を止めればいいのよ ",
    /* 65 */ "~J1(4.6)\tあの悪魔を…… ~N\n\t\t\tあの少女の姿をした悪魔をね ",
    /* 66 */ "~J1(3.7)\tあなたの娘が生けにえにされる前に ",
    /* 67 */ "~J1(2.6)\t間に合わなくなる前にね ",
    /* 68 */ "~J1(3.0)\tとめるのよ……　やめさせるのよ…… ",
    /* 69 */ "~J1(1.6)\tどこに行けばいいんだ？ ",
    /* 70 */ "~J1(3.3)\t湖に張り出した灯台 ",
    /* 71 */ "~J1(3.5)\tそして遊園地の中心よ ",
    /* 72 */ "~J1(3.5)\t急ぎなさい　あなたしかいないわ ~E ",
    /* 73 */ "~J0(1.1)\tハリー ",
    /* 74 */ "~J0(2.1)\tあたしには何が ~N\n\t\t\tどうなってるかわからないけど ",
    /* 75 */ "~J0(4.4)\tとにかく　あなたの娘を助けられるのね？ ~N\n\t\t\tやってみるしかなさそうね ",
    /* 76 */ "~J0(1.9)\tあたしは遊園地に向かうわ ",
    /* 77 */ "~J0(1.7)\tあなたは灯台へ行って ~E ",
    /* 78 */ "~J0(2.5)\tシビル……ありがとう ~E ",
    /* 79 */ "~J0(2.1)\tあれを使うのよ ~E ",
    /* 80 */ "~J0(1.2)\t何？ ",
    /* 81 */ "~J0(1.7)\tフラウロス…… ",
    /* 82 */ "~J0(3.2)\tそうすれば止められるわ ~E ",
    /* 83 */ "~J0(2.0)\tシビルは……！ ~E ",
    /* 84 */ "~J0(2.0)\tやはりシビルは戻っていないか…… ~E ",
    /* 85 */ "~J0(3.5)\tおそらく　あいつは ~N\n\t\t\tこれから遊園地に現れるだろう ",
    /* 86 */ "~J0(2.0)\t間に合ってくれ…… ~E ",
    /* 87 */ "\t船の舵輪がある　だが今はそんな ~N\n\tものを気にしている場合ではない ~E ",
    /* 88 */ "\t船の壁に死体がはりつけられている ~E ",
    /* 89 */ "~J0(1.0)\t何だ？ ~E ",
    /* 90 */ "~J0(2.5)\tあの光は！？ ~E"
#endif
};

void MapEvent_CommonItemTake(void) // 0x800D12B4
{
    u32 pickupType;
    s32 eventFlagIdx;

    pickupType   = CommonPickupItemId_FirstAidKit;
    eventFlagIdx = 0;

    switch (g_MapEventData->mapPointIdx)
    {
        case 13:
            pickupType   = CommonPickupItemId_RifleShells;
            eventFlagIdx = EventFlag_M6S01_RifleShells;
            break;

        case 14:
            pickupType   = CommonPickupItemId_HandgunBullets;
            eventFlagIdx = EventFlag_M6S01_HandgunBullets;
            break;

        case 15:
            pickupType   = CommonPickupItemId_HealthDrink;
            eventFlagIdx = EventFlag_M6S01_HealthDrink;
            break;
    }

    Event_CommonItemTake(pickupType, eventFlagIdx);
}

#include "maps/common/shared/Event_CutsceneTimerAdvance.h" // 0x800D1340

void func_800D13D8(void) // 0x800D13D8
{
    s_FsImageDesc charaTex;
    VECTOR3       pos;
    SVECTOR3      rot;

    if ((g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.skip) &&
        g_SysWork.sysStateSteps[0] >= 3 && g_SysWork.sysStateSteps[0] < 52)
    {
        SysWork_StateStepSet(0, 53);
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();

            CutsceneBorder_ForceShow();
            Screen_FadeTimestepReset();
            g_SysWork.sysFlags |= SysFlag_CutsceneActive;

            Chara_Load(0, Chara_Cybil, &g_SysWork.npcBoneCoordBuffer[0], CHARA_FORCE_FREE_ALL, NULL, NULL);
            Fs_QueueStartRead(FILE_ANIM_SIP_DMS, FS_BUFFER_15);
            Chara_ProcessLoads();
            Chara_Spawn(Chara_Cybil, 0, Q12(-19.0f), Q12(58.0f), Q12_ANGLE(0.0f), 3);

            Dms_HeaderFixOffsets(FS_BUFFER_15);

            g_Cutscene_Timer           = Q12(0.0f);
            g_Cutscene_MapMsgAudioIdx0 = 0;
            g_Cutscene_UpdateSibyl     = true;
            g_Cutscene_UpdateDaria     = false;

            func_8003D03C();
            Player_EmptyWeaponHandSet();

            g_SysWork.lightBoneCoord     = NULL;
            g_SysWork.lensFlareBoneCoord = NULL;
            g_SysWork.lightIntensity     = Q12(0.6f);

            Gfx_MapEnvSet(16, 16);
            func_8008D438();
            SysWork_StateStepIncrement(0);

        case 1:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.0f), false);
            break;

        case 2:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 113, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 14, false);
            SysWork_StateStepIncrement(0);

        case 3:
            Event_DisplayMapMsgWithAudio(15, &g_Cutscene_MapMsgAudioIdx0, &g_Cutscene_MapMsgAudioCmds0);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(9.5f), Q12(0.0f), Q12(21.0f), true, false);
            break;

        case 4:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(9.5f), Q12(0.0f), Q12(21.0f), true, true);
            break;

        case 5:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 51, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 5, false);
            g_Cutscene_Timer = Q12(22.0f);

            SysWork_StateStepIncrement(0);

        case 6:
            Event_DisplayMapMsgWithAudio(16, &g_Cutscene_MapMsgAudioIdx0, &g_Cutscene_MapMsgAudioCmds0);
            break;

        case 7:
            Event_DisplayMapMsgWithAudio(22, &g_Cutscene_MapMsgAudioIdx0, &g_Cutscene_MapMsgAudioCmds0);
            break;

        case 8:
            g_Cutscene_Timer = Q12(23.0f);
            Event_DisplayMapMsgWithAudio(26, &g_Cutscene_MapMsgAudioIdx0, &g_Cutscene_MapMsgAudioCmds0);
            break;

        case 9:
            Event_DisplayMapMsgWithAudio(29, &g_Cutscene_MapMsgAudioIdx0, &g_Cutscene_MapMsgAudioCmds0);
            break;

        case 10:
            g_Cutscene_Timer = Q12(24.0f);
            Event_DisplayMapMsgWithAudio(37, &g_Cutscene_MapMsgAudioIdx0, &g_Cutscene_MapMsgAudioCmds0);
            break;

        case 11:
            Chara_FsImageCalc(&charaTex, Chara_Dahlia, 2);
            Chara_Load(1, Chara_Dahlia, &g_SysWork.npcBoneCoordBuffer[30], 0, LM_BUFFER_2, &charaTex);
            SysWork_StateStepIncrement(0);

        case 12:
            if (Fs_QueueChunksLoad() != false)
            {
                SysWork_StateStepIncrement(0);
            }
            break;

        case 13:
            Chara_ProcessLoads();
            SysWork_StateStepIncrement(0);

        case 14:
            Event_DisplayMapMsgWithAudio(46, &g_Cutscene_MapMsgAudioIdx0, &g_Cutscene_MapMsgAudioCmds0);
            break;

        case 15:
            Sfx_WithFlagsPlay(Sfx_Unk1604, &Q12_VECTOR3(-21.3f, -1.2f, 58.7f), Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 16:
            Event_WaitTimer(Q12(0.5f), false);
            break;

        case 17:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 70, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 17, false);
            Sfx_WithFlagsPlay(Sfx_Unk1605, &Q12_VECTOR3(-21.3f, -1.2f, 58.7f), Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 18:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(25.0f), Q12(34.0f), true, true);
            break;

        case 19:
            Chara_Spawn(Chara_Dahlia, 0, Q12(-19.0f), Q12(58.0f), Q12_ANGLE(0.0f), 3);

            g_Cutscene_UpdateDaria = true;

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 51, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 5, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[1], 5, false);
            Savegame_EventFlagSet(EventFlag_427);

            g_Cutscene_Timer = Q12(35.0f);

            SysWork_StateStepIncrement(0);

        case 20:
            Event_DisplayMapMsgWithAudio(50, &g_Cutscene_MapMsgAudioIdx0, &g_Cutscene_MapMsgAudioCmds0);
            break;

        case 21:
            g_Cutscene_Timer = Q12(36.0f);
            Event_DisplayMapMsgWithAudio(52, &g_Cutscene_MapMsgAudioIdx0, &g_Cutscene_MapMsgAudioCmds0);
            break;

        case 22:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 113, false);
            SysWork_StateStepIncrement(0);

        case 23:
            Event_DisplayMapMsgWithAudio(63, &g_Cutscene_MapMsgAudioIdx0, &g_Cutscene_MapMsgAudioCmds0);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(37.0f), Q12(49.0f), true, false);
            Event_WaitTimer(Q12(1.0f), false);
            break;

        case 24:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 131, false);
            SysWork_StateStepIncrement(0);

        case 25:
            Event_DisplayMapMsgWithAudio(63, &g_Cutscene_MapMsgAudioIdx0, &g_Cutscene_MapMsgAudioCmds0);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(37.0f), Q12(49.0f), true, false);
            break;

        case 26:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 16, false);

            g_Cutscene_Timer = Q12(50.0f);
            D_800D5345 = true;

            Model_AnimFlagsClear(&g_SysWork.npcs[1].model, AnimFlag_Visible);
            SysWork_StateStepIncrement(0);

        case 27:
            Event_DisplayMapMsgWithAudio(73, &g_Cutscene_MapMsgAudioIdx0, &g_Cutscene_MapMsgAudioCmds0);
            Event_WaitTimer(Q12(1.5f), false);
            break;

        case 28:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 71, false);
            SysWork_StateStepIncrement(0);

        case 29:
            Event_DisplayMapMsgWithAudio(73, &g_Cutscene_MapMsgAudioIdx0, &g_Cutscene_MapMsgAudioCmds0);
            break;

        case 30:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 13, false);

            D_800D5345 = false;

            Model_AnimFlagsSet(&g_SysWork.npcs[1].model, AnimFlag_Visible);
            SysWork_StateStepIncrement(0);

        case 31:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(6.0f), Q12(51.0f), Q12(60.0f), true, true);
            break;

        case 32:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 51, false);
            SysWork_StateStepIncrement(0);

        case 33:
            Event_DisplayMapMsgWithAudio(78, &g_Cutscene_MapMsgAudioIdx0, &g_Cutscene_MapMsgAudioCmds0);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(6.0f), Q12(61.0f), Q12(97.0f), true, false);
            break;

        case 34:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(6.0f), Q12(61.0f), Q12(97.0f), true, true);
            break;

        case 35:
            Chara_ModelCharaIdClear(&g_SysWork.npcs[0], 0, 0);
            Sfx_WithFlagsPlay(Sfx_Unk1609, &Q12_VECTOR3(-19.95f, -1.2f, 60.7f), Q8(0.5f), SfxFlag_None);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 113, false);
            SysWork_StateStepIncrement(0);

        case 36:
            Event_WaitTimer(Q12(0.3f), false);

            if (g_SysWork.sysStateSteps[0] != 36)
            {
                Sfx_WithFlagsPlay(Sfx_Unk1610, &Q12_VECTOR3(-19.95f, -1.2f, 60.7f), Q8(0.5f), SfxFlag_None);
            }

        case 37:
            if (g_SysWork.sysStateSteps[0] == 37)
            {
                Event_CharaAnimCmdExecute(CharaAnimCmd_AwaitAnimEnd, &g_SysWork.playerWork.player, 0, false);
            }

            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(98.0f), Q12(121.0f), true, false);
            break;

        case 38:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 131, false);
            SysWork_StateStepIncrement(0);

        case 39:
            Event_DisplayMapMsgWithAudio(79, &g_Cutscene_MapMsgAudioIdx0, &g_Cutscene_MapMsgAudioCmds0);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(98.0f), Q12(121.0f), true, false);
            Event_WaitTimer(Q12(0.6f), false);
            break;

        case 40:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 132, false);
            SysWork_StateStepIncrement(0);

        case 41:
            Event_DisplayMapMsgWithAudio(79, &g_Cutscene_MapMsgAudioIdx0, &g_Cutscene_MapMsgAudioCmds0);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(98.0f), Q12(121.0f), true, false);
            break;

        case 42:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[1], 8, false);
            Model_AnimFlagsClear(&g_SysWork.playerWork.player.model, AnimFlag_Visible);
            SysWork_StateStepIncrement(0);

        case 43:
            Event_DisplayMapMsgWithAudio(80, &g_Cutscene_MapMsgAudioIdx0, &g_Cutscene_MapMsgAudioCmds0);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(122.0f), Q12(142.0f), true, false);
            break;

        case 44:
            SysWork_StateStepIncrement(0);

        case 45:
            Event_DisplayMapMsgWithAudio(83, &g_Cutscene_MapMsgAudioIdx0, &g_Cutscene_MapMsgAudioCmds0);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(122.0f), Q12(157.0f), true, false);
            break;

        case 46:
            Chara_ModelCharaIdClear(&g_SysWork.npcs[1], 0, 0);
            Sfx_WithFlagsPlay(Sfx_Unk1604, &Q12_VECTOR3(-21.3f, -1.2f, 58.7f), Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 47:
            Event_WaitTimer(Q12(0.3f), false);
            break;

        case 48:
            Sfx_WithFlagsPlay(Sfx_Unk1605, &Q12_VECTOR3(-21.3f, -1.2f, 58.7f), Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 49:
            Event_WaitTimer(Q12(0.6f), false);
            break;

        case 50:
            Event_CharaAnimPlayToEnd(&g_SysWork.playerWork.player, 71);
            Model_AnimFlagsSet(&g_SysWork.playerWork.player.model, AnimFlag_Visible);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(158.0f), Q12(168.0f), true, false);
            break;

        case 51:
            g_Cutscene_Timer = Q12(169.0f);
            Event_WaitTimer(Q12(2.0f), false);
            break;

        case 52:
            SysWork_StateStepIncrement(0);
            break;

        case 53:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 54:
            Chara_ModelCharaIdClear(&g_SysWork.npcs[0], 0, 0);
            Chara_ModelCharaIdClear(&g_SysWork.npcs[1], 0, 0);

            g_Cutscene_Timer = Q12(169.0f);

            SysWork_StateStepIncrement(0);

            D_800D5345 = false;
            break;

        default:
            Player_ControlUnfreeze(true);
            SysWork_StateSetNext(SysState_Gameplay);
            Game_TurnFlashlightOn();
            Gfx_MapEnvSet(7, 4);
            func_8008D448();
            Game_FlashlightAttribsSet();

            g_SysWork.lightIntensity = Q12(1.0f);

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            vcReturnPreAutoCamWork(true);

            g_Cutscene_Timer = NO_VALUE;

            SD_Call(19);
            Savegame_EventFlagSet(EventFlag_424);
            Savegame_EventFlagSet(EventFlag_MapMark_ResortTown_LighthouseCircle);

            func_8003D01C();
            Player_WeaponAttackRestore();
            Model_AnimFlagsSet(&g_SysWork.playerWork.player.model, AnimFlag_Visible);
            break;
    }

    // Update objects.
    if (g_Cutscene_Timer >= Q12(0.0f))
    {
        Dms_CharacterTransformGet(&g_SysWork.playerWork.player.position, &g_SysWork.playerWork.player.rotation, "HERO", g_Cutscene_Timer, FS_BUFFER_15);

        if (g_Cutscene_UpdateSibyl)
        {
            Dms_CharacterTransformGet(&g_SysWork.npcs[0].position, &g_SysWork.npcs[0].rotation, "SIBYL", g_Cutscene_Timer, FS_BUFFER_15);
        }
        if (g_Cutscene_UpdateDaria)
        {
            Dms_CharacterTransformGet(&g_SysWork.npcs[1].position, &g_SysWork.npcs[1].rotation, "DARIA", g_Cutscene_Timer, FS_BUFFER_15);
        }

        vcChangeProjectionValue(Dms_CameraTargetsGet(&g_Cutscene_CameraPositionTarget, &g_Cutscene_CameraLookAtTarget, NULL, g_Cutscene_Timer, FS_BUFFER_15));
        vcUserCamTarget(&g_Cutscene_CameraPositionTarget, NULL, true);
        vcUserWatchTarget(&g_Cutscene_CameraLookAtTarget, NULL, true);

        Dms_CharacterTransformGet(&g_SysWork.lightPosition, &rot, "LIGHT", g_Cutscene_Timer, FS_BUFFER_15);
        Dms_CharacterTransformGet(&pos, &rot, "L_INT", g_Cutscene_Timer, FS_BUFFER_15);

        g_SysWork.lightRotation.vx = -ratan2(pos.vy - g_SysWork.lightPosition.vy, Math_Vector2MagCalcSafeQ6(pos.vx - g_SysWork.lightPosition.vx, pos.vz - g_SysWork.lightPosition.vz));
        g_SysWork.lightRotation.vy = ratan2(pos.vx - g_SysWork.lightPosition.vx, pos.vz - g_SysWork.lightPosition.vz);
        g_SysWork.lightRotation.vz = 0;
    }
}

void func_800D236C(void) // 0x800D236C
{
    if ((g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.skip) &&
        g_SysWork.sysStateSteps[0] >= 3 && g_SysWork.sysStateSteps[0] < 8)
    {
        SysWork_StateStepSet(0, 8);
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            g_Cutscene_MapMsgAudioIdx1 = 0;
            Screen_FadeTimestepReset();
            CutsceneBorder_ForceShow();

            g_SysWork.playerWork.player.position.vx = Q12(-20.98);
            g_SysWork.playerWork.player.position.vz = Q12(58.67f);
            g_SysWork.playerWork.player.rotation.vy = Q12_ANGLE(90.0f);

            Event_CameraPositionSet(NULL, Q12(-18.7f), Q12(-5.59f), Q12(57.22f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(NULL, Q12(-19.46f), Q12(-1.92f), Q12(58.62f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);

            Sfx_WithFlagsPlay(Sfx_Unk1604, &Q12_VECTOR3(-21.3f, -1.2f, 58.7f), Q8(0.5f), SfxFlag_None);

            Game_TurnFlashlightOn();
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitTimer(Q12(0.8f), false);
            break;

        case 2:
            Sfx_WithFlagsPlay(Sfx_Unk1605, &Q12_VECTOR3(-21.3f, -1.2f, 58.7f), Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 3:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.5f), false);
            break;

        case 4:
            Event_WaitTimer(Q12(1.5f), false);
            break;

        case 5:
            Event_DisplayMapMsgWithAudio(84, &g_Cutscene_MapMsgAudioIdx1, &g_Cutscene_MapMsgAudioCmds1);
            break;

        case 6:
            Event_WaitTimer(Q12(3.5f), false);
            break;

        case 7:
            Event_DisplayMapMsgWithAudio(85, &g_Cutscene_MapMsgAudioIdx1, &g_Cutscene_MapMsgAudioCmds1);
            break;

        case 8:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            vcReturnPreAutoCamWork(true);

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);

            Savegame_EventFlagSet(EventFlag_426);
            Savegame_EventFlagSet(EventFlag_MapMark_ResortTown_ThemeParkArrow);
            break;
    }
}

void func_800D2658(void) // 0x800D2658
{
    const static SVECTOR EMPTY_ROT = {};
    const static VECTOR3 UNK_POS   = { Q12(-23.37f), Q12(-1.25f), Q12(19.28f) };

    s32 temp_a0_4;
    s32 temp_s0;
    s32 temp_s1;
    s32 var_s4;
    s32 i;

    // @unused Maybe some removed debug print.
    temp_s0 = D_800D41B4;
    temp_s1 = temp_s0 >> 1;
    Math_Cos(temp_s1);
    Math_Sin(temp_s0 >> 4);
    Math_Sin(Math_Sin(temp_s1 * 2));

    D_800D41B4 += g_DeltaTime;

    if (g_SysWork.sysStateSteps[0] >= 10)
    {
        D_800D41B0 -= Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.5f);
        if (D_800D41B0 < Q12(0.0f))
        {
            D_800D41B0 = Q12(0.0f);
        }
        Sd_SfxAttributesUpdate(Sfx_Unk1467, 0, ~Q12_TO_Q8(D_800D41B0), 0);
    }
    else if (g_SysWork.sysStateSteps[0] > 0)
    {
        D_800D41B0 += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.25f);
        if (D_800D41B0 > Q12(0.9961f))
        {
            D_800D41B0 = Q12(0.9961f);
        }
        Sd_SfxAttributesUpdate(Sfx_Unk1467, 0, ~Q12_TO_Q8(D_800D41B0), 0);
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            g_Cutscene_MapMsgAudioIdx0 = 0;

            Fs_QueueStartReadTim(FILE_TIM_ENBAN_TIM, FS_BUFFER_1, &D_800D410C);
            Fs_QueueStartRead(FILE_ANIM_UFO4_DMS, FS_BUFFER_13);
            g_Cutscene_Timer = NO_VALUE;

            Screen_FadeTimestepReset();
            SD_Call(Sfx_Unk1467);
            Sd_SfxAttributesUpdate(Sfx_Unk1467, 0, Q8_CLAMPED(1.0f), 0);

            D_800D41B0 = Q12(0.0f);
            D_800D41B4 = Q12(0.0f);

            for (i = 0; i < ARRAY_SIZE(D_800D4174); i++)
            {
                D_800D4174[i].vx = FP_FROM(D_800D4114[i].vx * Math_Sin(FP_TO(i, Q12_SHIFT) / 5), Q12_SHIFT) - Q12(23.0f);
                D_800D4174[i].vy = FP_FROM(D_800D4114[i].vx * Math_Cos(FP_TO(i, Q12_SHIFT) / 5), Q12_SHIFT) - Q12(1.0f);
                D_800D4174[i].vz = Q12(18.0f);
            }

            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            if (Fs_QueueChunksLoad())
            {
                SysWork_StateStepIncrement(0);
            }
            break;

        case 3:
            g_Cutscene_Timer = Q12(0.0f);

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 3, Q12(0.0f), false);
            Dms_HeaderFixOffsets(FS_BUFFER_13);
            func_8003D03C();
            Player_EmptyWeaponHandSet();
            Game_TurnFlashlightOn();
            SysWork_StateStepIncrement(0);

        case 4:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.0f), false);

        case 5:
            Event_DisplayMapMsgWithAudio(89, &g_Cutscene_MapMsgAudioIdx0, &D_800D4108);
            break;

        case 6:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(40.0f), Q12(0.0f), Q12(30.0f), true, true);

            for (i = 0; i < ARRAY_SIZE(D_800D4174); i++)
            {
                sharedFunc_800CE5D4_1_s03(&D_800D4174[i], Q12(0.5f), Q12(0.05f), 0);
            }
            break;

        case 7:
            Event_DisplayMapMsg(false, 90, 0, 0, 0, false);

            for (i = 0; i < ARRAY_SIZE(D_800D4174); i++)
            {
                sharedFunc_800CE5D4_1_s03(&D_800D4174[i], Q12(0.5f), Q12(0.05f), 0);
            }
            break;

        case 8:
            Vc_SetLookAtMatFromBoneCoord(&g_SysWork.playerBoneCoords[HarryBone_Head], &EMPTY_ROT, &UNK_POS);

            for (i = 0; i < 5; i++)
            {
                sharedFunc_800CE5D4_1_s03(&D_800D4174[i], Q12(0.5f), Q12(0.05f), i == 0);
            }

            Event_WaitTimer(Q12(2.5f), false);
            break;

        case 9:
            if (g_SysWork.sysStateStepData[0] > Q12(0.8f))
            {
                var_s4 = (g_SysWork.sysStateStepData[0] - Q12(0.8f)) >> 2;
                if (var_s4 > Q12(0.25f))
                {
                    var_s4 = Q12(0.25f);
                }

                var_s4 = Q12(1.0f) - Math_Cos(var_s4);

                for (i = 0; i < ARRAY_SIZE(D_800D4174); i++)
                {
                    temp_a0_4         = Q12_MULT_PRECISE(var_s4, D_800D4114[i].vy) + D_800D4114[i].vy;
                    temp_a0_4         = Q12_MULT_FLOAT_PRECISE(temp_a0_4, 1.2f);
                    D_800D4174[i].vx += Q12_MULT_PRECISE(g_DeltaTime, Q12_MULT(temp_a0_4, Math_Sin(FP_TO(i, Q12_SHIFT) / 5)));
                    D_800D4174[i].vy += Q12_MULT_PRECISE(g_DeltaTime, Q12_MULT(temp_a0_4, Math_Cos(FP_TO(i, Q12_SHIFT) / 5)));
                }
            }

            for (i = 0; i < ARRAY_SIZE(D_800D4174); i++)
            {
                sharedFunc_800CE5D4_1_s03(&D_800D4174[i], Q12(0.5f), Q12(0.05f), 0);
            }

            g_SysWork.sysStateStepData[0] += g_DeltaTime;
            if (g_SysWork.sysStateStepData[0] > Q12(1.8f))
            {
                SysWork_StateStepIncrement(0);
            }
            break;

        case 10:
            Event_WaitTimer(Q12(2.5f), false);
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            vcReturnPreAutoCamWork(true);

            Savegame_EventFlagSet(EventFlag_459);
            Event_InvItemCmd(InvItemCmd_AddItem, InvItemId_ChannelingStone, 1, false);

            func_8003D01C();
            Player_WeaponAttackRestore();
            g_Cutscene_Timer = NO_VALUE;
            Sd_SfxStop(Sfx_Unk1467);
            break;
    }

    if (g_Cutscene_Timer >= Q12(0.0f))
    {
        Dms_CharacterTransformGet(&g_SysWork.playerWork.player.position, &g_SysWork.playerWork.player.rotation, "HERO", g_Cutscene_Timer, FS_BUFFER_13);
        vcChangeProjectionValue(Dms_CameraTargetsGet(&g_Cutscene_CameraPositionTarget, &g_Cutscene_CameraLookAtTarget, NULL, g_Cutscene_Timer, FS_BUFFER_13));
        vcUserCamTarget(&g_Cutscene_CameraPositionTarget, NULL, true);
        vcUserWatchTarget(&g_Cutscene_CameraLookAtTarget, NULL, true);
    }
}

void Map_WorldObjectsInit(void) // 0x800D2E6C
{
    D_800D5345 = false;

    WorldObject_Init(&D_800D5374, g_CommonWorldObjectNames[1], -19.237060546875f, -0.90576171875f, 60.533f, 0.0f, 78.93f, 0.0f);
    WorldObject_Init(&D_800D53A4[0], "DOOR2_HI", -21.38f, -0.95f, 58.7f, 0.0, 180.0f, 0.0f);
    WorldObject_Init(&D_800D53A4[1], "BRI3_HID", -21.29f, -1.434814453125f, 60.31f, 0.0, 180.0f, 0.0f);
    WorldObject_Init(&D_800D53A4[2], "BAR1_HID", -21.31f, -1.71f, 59.87f, 0.0, 180.0f, 0.0f);
    WorldObject_Init(&D_800D53A4[3], "WID1_HID", -21.38f, -0.95f, 57.8f, 0.0, 180.0f, 0.0f);
    WorldObject_Init(&D_800D53A4[4], "WID2_HID", -21.38f, -0.95f, 60.05f, 0.0, 180.0f, 0.0f);
    WorldObject_Init(&D_800D53A4[5], "GLAS2_HI", -21.4f, -1.4208984375f, 58.77f, 0.0, 180.0f, 0.0f);
    WorldObject_Init(&D_800D53A4[6], "RIGHT_HI", -19.6f, -0.825f, 59.4f, 0.0, 180.0f, 0.0f);

    WorldObject_ModelNameSet(&g_CommonWorldObjects[0], g_CommonWorldObjectNames[2]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[1], g_CommonWorldObjectNames[3]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[2], g_CommonWorldObjectNames[4]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[3], g_CommonWorldObjectNames[5]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[4], g_CommonWorldObjectNames[6]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[5], g_CommonWorldObjectNames[7]);
}

const s16 D_800CBB5C = 0x1440;

void Map_WorldObjectsUpdate(void) // 0x800D30DC
{
    s32 i;
    MAP_CHUNK_CHECK_VARIABLE_DECL();

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -1, 0, 0) && PLAYER_IN_MAP_CHUNK(vz, 1, 2, -1, 2))
    {
        WorldObjects_Add(&D_800D5374.object, &D_800D5374.position, &D_800D5374.rotation);

        if (!D_800D5345)
        {
            for (i = 0; i < ARRAY_SIZE(D_800D53A4); i++)
            {
                WorldObjects_Add(&D_800D53A4[i].object, &D_800D53A4[i].position, &D_800D53A4[i].rotation);
            }
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -1, 0, 0) && PLAYER_IN_MAP_CHUNK(vz, 1, 2, -1, 2))
    {
        if (!Savegame_EventFlagGet(EventFlag_M6S01_RifleShells))
        {
            WorldObjects_Add(&g_CommonWorldObjects[4], &g_CommonWorldObjectPoses[0].position, &g_CommonWorldObjectPoses[0].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -1, 0, 0) && PLAYER_IN_MAP_CHUNK(vz, 1, 2, -1, 2))
    {
        if (!Savegame_EventFlagGet(EventFlag_M6S01_HandgunBullets))
        {
            WorldObjects_Add(&g_CommonWorldObjects[3], &g_CommonWorldObjectPoses[1].position, &g_CommonWorldObjectPoses[1].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -1, 0, 0) && PLAYER_IN_MAP_CHUNK(vz, 1, 2, -1, 2))
    {
        if (!Savegame_EventFlagGet(EventFlag_M6S01_HealthDrink))
        {
            WorldObjects_Add(&g_CommonWorldObjects[1], &g_CommonWorldObjectPoses[2].position, &g_CommonWorldObjectPoses[2].rotation);
        }
    }
}
