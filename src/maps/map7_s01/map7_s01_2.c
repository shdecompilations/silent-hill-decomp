#include "bodyprog/bodyprog.h"
#include "bodyprog/chara/dms.h"
#include "bodyprog/events/event_data.h"
#include "bodyprog/gfx/map_effects.h"
#include "bodyprog/item_screens.h"
#include "bodyprog/map/terrain.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player/player.h"
#include "bodyprog/sound/sound_system.h"
#include "main/rng.h"
#include "maps/map7/map7_s01.h"
#include "maps/characters/bloody_lisa.h"
#include "maps/characters/ghost_child_alessa.h"
#include "maps/characters/lisa.h"
#include "maps/characters/player.h"
#include "maps/particle.h"
#include "maps/characters/puppet_nurse.h"

#include "../src/maps/particle_glass.c"

#include "../src/maps/particle.c"


// player.c/puppet_nurse.c have some identical rodata which has to be deduplicated to match.
// More info at `src/maps/map5_s02/characters.c` as that had the same issue.

#include "../src/maps/characters/player.c"

#include "../src/maps/unk_m3s03.c" // 0x800D16A4

#include "../src/maps/characters/puppet_nurse.c"

#include "../src/maps/characters/ghost_child_alessa.c"
#include "../src/maps/characters/lisa.c"
#include "../src/maps/characters/bloody_lisa.c"

#include "../src/maps/chara_util.c" // 0x800D5CD8

#include "../src/maps/map_util.c" // 0x800D6768

void func_800D6810(s32 arg0) // 0x800D6810
{
    (arg0 != 0) ? func_800D6878() : sharedFunc_800D0110_7_s00();
}

void func_800D6844(s32 arg0) // 0x800D6844
{
    (arg0 != 0) ? func_800D68F8() : sharedFunc_800D0110_7_s00();
}

void func_800D6878(void) // 0x800D6878
{
    s32 bgmIdx; // `e_BgmCmd`

    if (g_SavegamePtr->mapRoomIdx == 3)
    {
        bgmIdx = BgmCmd_Track15;
    }
    else if (Savegame_EventFlagGet(EventFlag_481))
    {
        if (!Savegame_EventFlagGet(EventFlag_574))
        {
            Savegame_EventFlagSet(EventFlag_574);
            bgmIdx = BgmCmd_UpdateTrack;
        }
        else
        {
            bgmIdx = BgmCmd_Track22;
        }
    }
    else
    {
        bgmIdx = BgmCmd_Track14;
    }

    Bgm_SongChange(bgmIdx);
}

void func_800D68F8(void) // 0x800D68F8
{
    Bgm_SongChange(Savegame_EventFlagGet(EventFlag_M5S03_KaufmannHasMotorcycleVial) ? 40 : 35);
}

#include "maps/shared/sharedFunc_800D0110_7_s00.h" // 0x800D6938

void GameBoot_LoadScreen_StageString(void) {}

#include "maps/shared/Event_CutsceneTimerAdvance.h" // 0x800D71C4

const char* MAP_MESSAGES[] = {
#include "maps/shared/map_msg_common.h"
#if VERSION_REGION_IS(NTSC)
    /* 15 */ "~J0(5.5)\t",
    /* 16 */ "~J0(2.8)\tDamn! ~N\n\t\t\tWhat's_that? ~E ",
    /* 17 */ "~J0(1.2)\tHarry? ~E ",
    /* 18 */ "~J1(2.6)\tLisa... ~N\n\t\t\tWhat's_the_matter_with_you? ",
    /* 19 */ "~J1(1.8)\tI_get_it_now... ",
    /* 20 */ "~J1(4.0)\tWhy_I'm_still_alive_even ~N\n\t\t\tthough_everyone_else's_dead. ",
    /* 21 */ "~J1(4.0)\tI'm_not_the_only_one ~N\n\t\t\twho's_still_walking_around. ",
    /* 22 */ "~J1(1.7)\tI'm_the_same_as_them. ",
    /* 23 */ "~J1(2.8)\tI_just_hadn't_noticed_it_before. ~E ",
    /* 24 */ "~J1(1.3)\tLisa? ",
    /* 25 */ "~J1(3.6)\tStay_by_me,_Harry. ~N\n\t\t\tPlease. ",
    /* 26 */ "~J1(3.3)\tI'm_so_scared. ~N\n\t\t\tHelp_me... ~E ",
    /* 27 */ "~J0(2.4)\tSave_me_from_them. ",
    /* 28 */ "~J0(4.0)\tPlease...__Harry... ~E ",
    /* 29 */ "~J0(8.9)\t~E ",
    /* 30 */ "\t\tSame_thing... ~E ",
    /* 31 */ "~L4 ~C3 Investigation_stalled. ~N\n\t[PTV]_dealers_still_at_large. ~N\n\t~N\n\tSuspicious_deaths_continue. ~N\n\tLike_the_anti-drug_mayor, ~N\n\ta_narcotics_officer_dies_of ~N\n\ta_sudden_heart_failure_of ~N\n\tunknown_origin. ~E ",
    /* 32 */ "\t\tAre_there_others? ~N\n\t\tIt's_pretty_old... ",
    /* 33 */ "~L4 ~C3 Fire_broke_out_in_town. ~N\n\t6_homes_destroyed. ~N\n\t~N\n\tCharred_body ~N\n\tof_Alessa_Gillespie(7) ~N\n\tfound_in_aftermath. ~N\n\t~N\n\tCause_of_fire_currently_under ~N\n\tinvestigation. ",
    /* 34 */ "~L4 ~C3 Investigations_show_source ~N\n\tas_basement_of_Gillespie_home. ~N\n\tBlaze_now_believed_caused_by ~N\n\tmalfunction_of_antiquated ~N\n\tboiler. ~E ",
    /* 35 */ "\tThe_date_of_the_blaze... ",
    /* 36 */ "\tit's_the_same_day ~N\n\twe_found_Cheryl! ~E ",
    /* 37 */ "\tThere's_a_place_mark ~N\n\tin_the_book... ~E ",
    /* 38 */ "~L4 ~C3 WHITE_CLAUDIA. ~N\n\t~N\n\tPerennial_herb_found_near_water. ~N\n\tReaches_height_of_10_to_15_in. ~N\n\tOblong_leaves,_white_blossoms. ",
    /* 39 */ "~L4 ~C3 Seeds_contain_hallucinogen. ~N\n\tAncient_records_show_it_was ~N\n\tused_for_religious_ceremonies. ~N\n\tThe_hallucinogenic_effect ~N\n\twas_key. ~E ",
    /* 40 */ "~L4 ~C3 Ask_doctor_to_let_me_quit_being ~N\n\tin_charge_of_that_patient. ~N\n\t~N\n\tIt's_too_weird. ~E ",
    /* 41 */ "~L4 ~C3 The_room_is_filled_with_insects. ~N\n\t~N\n\tEven_with_doors ~N\n\tand_windows_shut ~N\n\tthey_get_in_to_spite_me. ~N\n\t~N\n\tTo_the_hospital... ~E ",
    /* 42 */ "~L4 ~C3 Still_alive, ~N\n\tbut_with_wounds_that_won't_heal. ~E ",
    /* 43 */ "~L4 ~C3 Blood_and_pus_flow_from ~N\n\tthe_bathroom_faucet. ~N\n\t~N\n\tI_try_to_stop_it, ~N\n\tbut_it_won't_turn_off. ~E ",
    /* 44 */ "~L4 ~C3 Feeling_bad. ~N\n\tNeed_to_throw_up. ~N\n\tBut_nothing_comes_out. ~N\n\tVomiting_only_bile. ~E ",
    /* 45 */ "~L4 ~C3\tNeed_drug. ~E ",
    /* 46 */ "~L4 ~C3 Told_the_doctor_I_quit. ~N\n\tWon't_work_at_that ~N\n\thospital_anymore. ~E ",
    /* 47 */ "~L4 ~C3\tHelp_me... ~E ",
    /* 48 */ "\tUsed_the_ ~C2 Key_of_[Ophiel] ~C7 . ~E ",
    /* 49 */ "\tUsed_the_ ~C2 Key_of_[Hagith] ~C7 . ~E ",
    /* 50 */ "\tUsed_the_ ~C2 Key_of_[Phaleg] ~C7 . ~E ",
    /* 51 */ "\tThere_is_a_ ~C2 Ring_of_contract ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 52 */ "\tThere_are_ ~C2 Pliers ~C7 . ~N\n\tTake_them? ~S4 ",
    /* 53 */ "\tThere_is_a_ ~C2 Screwdriver ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 54 */ "\tThere_is_a_ ~C2 Bird_cage_key ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 55 */ "\tThere_is_a_ ~C2 Camera ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 56 */ "\tThere_is_a_ ~C2 Stone_of_time ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 57 */ "\tThere_is_a_ ~C2 Key_of_[Ophiel] ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 58 */ "\tThere_is_a_ ~C2 Key_of_[Hagith] ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 59 */ "\tThere_is_a_ ~C2 Key_of_[Phaleg] ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 60 */ "\tThere_is_an_ ~C2 Amulet_of_Solomon ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 61 */ "\tThere_is_a_ ~C2 Crest_of_Mercury ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 62 */ "~C3\tGo_home. ",
    /* 63 */ "~C3\tThief. ",
    /* 64 */ "~C3\tDrop_dead. ~E ",
    /* 65 */ "\tOld_clock. ~N\n\tThere_is_a_square_hole ~N\n\tin_the_center_of_the_clock. ",
    /* 66 */ "\tThe_hands_of_the_clock ~N\n\t...it's_a_key. ",
    /* 67 */ "\tI_can't_get_to_it_because ~N\n\tof_the_glass_cover. ~E ",
    /* 68 */ "\tThere_is_a_key_in_the_faucet, ",
    /* 69 */ "\tbut_I_can't_get_it_out_no_matter ~N\n\thow_much_I_pull. ~E ",
    /* 70 */ "\tThere_is_a_slate. ~E ",
    /* 71 */ "~C3\tNames_engraved ~N\n\t\tOn_a_lithograph. ~N\n\t\tThe_Grim_Reaper's_list. ",
    /* 72 */ "~C3\tYes,_the_headcount_is_set ~N\n\t\tYoung_and_old_lined_up ~N\n\t\tIn_order_of_age. ",
    /* 73 */ "~C3\tThen,_the_pathway_opens ~N\n\t\tAwaiting_them,_the_frenzied ~N\n\t\tUproar,_the_feast_of_death! ~E ",
    /* 74 */ "\tThere_is_a_key. ",
    /* 75 */ "\tElectricity_is_flowing_to_the_key. ~N\n\tI'll_get_shocked_if_I_grab_it_now. ~E ",
    /* 76 */ "~C3\tThe_light_to_the_future. ~E ",
    /* 77 */ "~C3\tThe_light_illuminating ~N\n\tthe_darkness. ~E ",
    /* 78 */ "\tThe_bird_cage_is_locked._~E ",
    /* 79 */ "\tUsed_the_ ~C2 Bird_cage_key ~C7 . ~E ",
    /* 80 */ "\tBird_cage._It's_empty. ~E ",
    /* 81 */ "\tIt's_locked. ",
    /* 82 */ "\tThe_word,_[Ophiel] ~N\n\tis_carved_on_the_door. ~E ",
    /* 83 */ "\tIt's_locked. ",
    /* 84 */ "\tThe_word,_[Hagith] ~N\n\tis_carved_on_the_door. ~E ",
    /* 85 */ "\tIt's_locked. ",
    /* 86 */ "\tThe_word,_[Phaleg] ~N\n\tis_carved_on_the_door. ~E ",
    /* 87 */ "\tA_steel_plate ~N\n\tis_screwed_to_the_wall. ~E ",
    /* 88 */ "\tThe_elevator_doesn't ~N\n\tappear_to_be_working. ~E ",
    /* 89 */ "\tThere's_a_clock ~N\n\twith_a_broken_face. ~E ",
    /* 90 */ "\tNone_of_this_stuff_is_of_any_use. ~E ",
    /* 91 */ "\tJust_a_wall. ~E ",
    /* 92 */ "\tThere_is_a_diary_on_the_floor. ~E ",
    /* 93 */ "\tNothing_helpful. ~E ",
    /* 94 */ "\tLooks_like_an_altar. ~N\n\tWonder_what_they_worship? ~E ",
    /* 95 */ "\tA_body_is_laid_out_on_the_bed. ~N\n\tDon't_feel_like_checking_this_out. ~E ",
    /* 96 */ "\tThere's_a_botanical ~N\n\treference_book. ~N\n\tDon't_need_to_read_it_now. ~E "
#elif VERSION_REGION_IS(NTSCJ)
    /* 15 */ "~J0(5.5)\t",
    /* 16 */ "~J0(2.8)\t何だ今のは！？ ~E ",
    /* 17 */ "~J0(1.2)\tハリー ~E ",
    /* 18 */ "~J1(2.6)\tリサ…… ~N\n\t\t\tいったいどうしたんだ？ ",
    /* 19 */ "~J1(1.8)\tわかったのよ ",
    /* 20 */ "~J1(4.0)\tどうしてみんな死んでしまったのに ~N\n\t\t\tあたしは生きてるのか ",
    /* 21 */ "~J1(4.0)\tあたしだけ生きてるんじゃなかったのよ ",
    /* 22 */ "~J1(1.7)\tみんなと同じ ",
    /* 23 */ "~J1(2.8)\tあたしは気がつかなかっただけ ~E ",
    /* 24 */ "~J1(1.3)\tリサ…… ",
    /* 25 */ "~J1(3.6)\tハリー ~N\n\t\t\tお願い……そばにいて…… ",
    /* 26 */ "~J1(3.3)\tあたし怖いわ ~N\n\t\t\t助けて…… ~E ",
    /* 27 */ "~J0(2.4)\tあいつらから守って…… ",
    /* 28 */ "~J0(4.0)\tね？　ハリー…… ~E ",
    /* 29 */ "~J0(8.9)\t~E ",
    /* 30 */ "\t\tまた　これか…… ~E ",
    /* 31 */ "~L4 ~C3\n\t捜査難航。 ~N\n\t麻薬“ＰＴＶ”の売人特定できず。~N\n\t~N\n\t相次ぐ怪死事件。 ~N\n\t麻薬売買の取締り強化を掲げた市長に続き、 ~N\n\t麻薬捜査官が原因不明の心臓発作で死亡。 ~E ",
    /* 32 */ "\t\t他にもあるのか？ ~N\n\t\tかなり古いな…… ",
    /* 33 */ "~L4 ~C3\n\t繁華街で不審火。 ~N\n\t６棟が全焼。 ~N\n\t~N\n\t逃げ遅れたアレッサ・ギレスピー（７歳）の ~N\n\t焼死体が発見された。 ~N\n\t~N\n\t出火の原因を調査中。 ",
    /* 34 */ "~L4 ~C3\n\t調査の結果、出火場所は ~N\n\tギレスピー宅の地下室と限定された。 ~N\n\t出火原因は老朽化したボイラーの ~N\n\t故障ではないかと推測される。 ~E ",
    /* 35 */ "\tこの火事の日付は…… ",
    /* 36 */ "\tシェリルを見つけたあの日だ…… ~E ",
    /* 37 */ "\t\tしおりの挟んである本がある ~E ",
    /* 38 */ "~L4 ~C3\n\t［ホワイトクロジェア］ ~N\n\t~N\n\t河岸／湖岸に生える多年草。 ~N\n\t高さは３０〜４０ｃｍ。 ~N\n\t長楕円状の葉を持ち、白い花を咲かせる。 ~N\n\t種に幻覚作用のある成分が含まれている。 ~N\n\t古来、その幻覚作用を利用して、宗教儀式 ~N\n\tに使われていたという記録も残っている。 ~E ",
    /* 39 */ "~L4 ~C3\n\t先生にあの患者の担当から ~N\n\t外してもらえるよう頼んだ。 ~N\n\t~N\n\tどう考えてもおかしい。 ~E ",
    /* 40 */ "~L4 ~C3\n\t部屋に数え切れないほどの虫がいる。 ~N\n\t~N\n\t窓や扉を閉めても入ってくる。 ~N\n\t~N\n\t震えるあたしをあざ笑っている。 ~N\n\t~N\n\t病院へ…… ~E ",
    /* 41 */ "~L4 ~C3\n\tまだ生きている。 ~N\n\tなのに傷は治らない。 ~E ",
    /* 42 */ "~L4 ~C3\n\tバスルームの蛇口から ~N\n\t血と膿が流れ出してくる。 ~N\n\t止めようとしても止まらない。 ~E ",
    /* 43 */ "~L4 ~C3\n\t気持ちが悪い。 ~N\n\t吐き気がする。 ~N\n\tだけど、もう何も出てこない。 ~N\n\t胃液すら吐きつくした。 ~E ",
    /* 44 */ "~L4 ~C3\t薬が、欲しい。 ~E ",
    /* 45 */ "~L4 ~C3\n\t先生に辞める意志を伝えた。 ~N\n\tもう、あの病院で働くつもりはない。 ~E ",
    /* 46 */ "~L4 ~C3\tだれか、たすけて…… ~E ",
    /* 47 */ "\t~C2\t［オフィエル］の鍵 ~C7 を使った ~E ",
    /* 48 */ "\t~C2\t［ハギト］の鍵 ~C7 を使った ~E ",
    /* 49 */ "\t~C2\t［ファレグ］の鍵 ~C7 を使った ~E ",
    /* 50 */ "\t~C2 契約の指輪 ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 51 */ "\t~C2\tペンチ ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 52 */ "\t~C2\tドライバー ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 53 */ "\t~C2\t鳥カゴの鍵 ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 54 */ "\t~C2\tカメラ ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 55 */ "\t~C2\t時の石 ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 56 */ "\t~C2\t［オフィエル］の鍵 ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 57 */ "\t~C2\t［ハギト］の鍵 ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 58 */ "\t~C2\t［ファレグ］の鍵 ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 59 */ "\t~C2 ソロモンの護符 ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 60 */ "\t~C2\tメルクリウスの紋章 ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 61 */ "~C3\t帰れ ",
    /* 62 */ "~C3\t泥棒 ",
    /* 63 */ "~C3\t死ね ~E ",
    /* 64 */ "\t古びた置き時計がある ~N\n\t時計の中央には四角い穴が空いている ",
    /* 65 */ "\tこの時計の針は……鍵だ ~N\n\tガラスがあるせいで取ることはできない ~E ",
    /* 66 */ "\t蛇口から鍵がのぞいている ~N\n\tしかし　どう引っ張っても取れそうにない ~E ",
    /* 67 */ "\t石板がある ~E ",
    /* 68 */ "~C3\t石板に刻まれた名前 ~N\n\t\tそれは冥府に帰りし者の目録 ",
    /* 69 */ "~C3\tさあ、その頭を並べろ ~N\n\t\t若さを尊び老いをあざけり列を作れ ",
    /* 70 */ "~C3\tその時、道が開かれる ~N\n\t\t行く手に待つは狂騒、死の宴！ ~E ",
    /* 71 */ "\t鍵がある ",
    /* 72 */ "\t鍵には電気が流れているようだ ~N\n\tこのまま取ると感電してしまうだろう ~E ",
    /* 73 */ "~C3\t光導く未来 ~E ",
    /* 74 */ "~C3\t闇を開く光 ~E ",
    /* 75 */ "\t鳥カゴには鍵がかかっている ~E ",
    /* 76 */ "\t~C2\t鳥カゴの鍵 ~C7 を使った ~E ",
    /* 77 */ "\t鳥カゴがある ~N\n\t中は空っぽで何もいない ~E ",
    /* 78 */ "\t鍵がかかっている ",
    /* 79 */ "\t扉には“オフィエル”と刻まれている ~E ",
    /* 80 */ "\t鍵がかかっている ",
    /* 81 */ "\t扉には“ハギト”と刻まれている ~E ",
    /* 82 */ "\t鍵がかかっている ",
    /* 83 */ "\t扉には“ファレグ”と刻まれている ~E ",
    /* 84 */ "\t鉄板が壁にネジ止めされている ~E ",
    /* 85 */ "\tエレベーターは動いていないようだ ~E ",
    /* 86 */ "\t文字盤の壊れた時計がある ~E ",
    /* 87 */ "\tいろいろなものが置かれているが ~N\n\t特に役に立ちそうなものはない ~E ",
    /* 88 */ "\tただの壁だ ~E ",
    /* 89 */ "\t床に日記が置かれている ~E ",
    /* 90 */ "\t役に立ちそうなものはない ~E ",
    /* 91 */ "\t祭壇らしきものがある ~N\n\tこれは何を祭っているのだろうか？ ~E ",
    /* 92 */ "\t死体がベッドに寝かされている ~N\n\tあれこれと調べる気にはなれない ~E ",
    /* 93 */ "\t植物事典が置かれている ~N\n\t今は読む必要はないだろう ~E "
#endif
};

void func_800D725C(void) // 0x800D725C
{
    s_FsImageDesc charaTex;

    if ((g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.skip) &&
        g_SysWork.sysStateSteps[0] >= 4 && g_SysWork.sysStateSteps[0] <= 20)
    {
        SysWork_StateStepSet(0, 22);
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            D_800E2C64 = 0;

            Fs_QueueStartRead(FILE_ANIM_HBM7_SLS_ANM, FS_BUFFER_4);
            Fs_QueueStartRead(FILE_ANIM_NURSE1_DMS, FS_BUFFER_11);
            Fs_QueueWaitForEmpty();

            Dms_HeaderFixOffsets((s_DmsHeader*)FS_BUFFER_11);
            g_Cutscene_Timer = Q12(0.0f);
            Anim_CharaTypeAnimInfoClear();

            Chara_Load(0, Chara_Lisa, g_SysWork.npcBoneCoordBuffer, CHARA_FORCE_FREE_ALL, NULL, NULL);

            ScreenFade_ResetTimestep();

            CutsceneBorder_ForceShow();
            g_SysWork.lightIntensity      = Q12(0.8f);
            g_SysWork.sysFlags           |= SysFlag_CutsceneActive;

            Game_TurnFlashlightOn();
            D_800E1670 = 0;

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 51, false);
            Player_EmptyWeaponHandSet();
            func_8003D03C();
            Bgm_SongChange(BgmCmd_Track4);
            SysWork_StateStepIncrement(0);

        case 1:
            Bgm_CrossfadeToTrack(BgmCmd_Track4);
            break;

        case 2:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(1.0f), false);
            break;

        case 3:
            Chara_ProcessLoads();
            Chara_Spawn(Chara_Lisa, 0, Q12(130.0f), Q12(-70.0f), Q12_ANGLE(0.0f), 3);
            SysWork_StateStepIncrement(0);

        case 4:
            Event_DisplayMapMsgWithAudio(17, &D_800E2C64, &D_800E14E8);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(0.0f), Q12(20.0f), true, false);
            Event_WaitTimer(Q12(0.8f), false);
            break;

        case 5:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 72, false);
            SysWork_StateStepIncrement(0);

        case 6:
            Event_DisplayMapMsgWithAudio(17, &D_800E2C64, &D_800E14E8);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(0.0f), Q12(20.0f), true, false);
            break;

        case 7:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(0.0f), Q12(20.0f), true, true);
            break;

        case 8:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 5, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 51, false);

            g_Cutscene_Timer = Q12(21.0f);

            SysWork_StateStepIncrement(0);

        case 9:
            Event_DisplayMapMsgWithAudio(18, &D_800E2C64, &D_800E14E8);
            break;

        case 10:
            Event_DisplayMapMsgWithAudio(24, &D_800E2C64, &D_800E14E8);
            break;

        case 11:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 17, false);
            SysWork_StateStepIncrement(0);

        case 12:
            Event_DisplayMapMsgWithAudio(27, &D_800E2C64, &D_800E14E8);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(22.0f), Q12(97.0f), false, false);
            break;

        case 13:
            SysWork_StateStepIncrement(0);

        case 14:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(22.0f), Q12(97.0f), false, true);
            break;

        case 15:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 155, false);
            SD_Call(Sfx_XaAudio568);
            SysWork_StateStepIncrement(0);

        case 16:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(22.0f), Q12(132.0f), true, true);
            break;

        case 17:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 18, false);
            Savegame_EventFlagSet(EventFlag_570);
            SysWork_StateStepIncrement(0);

        case 18:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(133.0f), Q12(157.0f), true, true);
            break;

        case 19:
            Chara_FsImageCalc(&charaTex, Chara_BloodyLisa, 2);
            Chara_Load(1, Chara_BloodyLisa, &g_SysWork.npcBoneCoordBuffer[30], 0, LM_BUFFER_2, &charaTex);

            g_SysWork.playerWork.player.position.vx = Q12(180.0f);
            g_SysWork.playerWork.player.position.vz = Q12(-100.0f);

            WorldGfx_CloseRangeChunksInit();
            SysWork_StateStepIncrement(0);

        case 20:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.5f), false);
            break;

        case 21:
            SysWork_StateStepReset();
            break;

        case 22:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 23:
            Savegame_EventFlagSet(EventFlag_571);
            SysWork_StateStepIncrement(0);

        case 24:
            g_SysWork.playerWork.player.position.vx = Q12(180.0f);
            g_SysWork.playerWork.player.position.vz = Q12(-100.0f);

            WorldGfx_CloseRangeChunksInit();
            Chara_FsImageCalc(&charaTex, Chara_BloodyLisa, 2);
            Chara_Load(1, Chara_BloodyLisa, &g_SysWork.npcBoneCoordBuffer[30], 0, LM_BUFFER_2, &charaTex);
            Chara_ProcessLoads();
            SysWork_StateStepIncrement(0);

        default:
            SysWork_StateSetNext(SysState_Gameplay);
            Savegame_EventFlagSet(EventFlag_482);
            Chara_ModelCharaIdClear(&g_SysWork.npcs[0], 0, 0);
            Chara_ProcessLoads();
            SD_Call(19);
            break;
    }

    if (g_Cutscene_Timer >= Q12(0.0f))
    {
        Dms_CharacterTransformGet(&g_SysWork.playerWork.player.position, &g_SysWork.playerWork.player.rotation, "HERO", g_Cutscene_Timer, (s_DmsHeader*)FS_BUFFER_11);
        Dms_CharacterTransformGet(&g_SysWork.npcs[0].position, &g_SysWork.npcs[0].rotation, "LISA", g_Cutscene_Timer, (s_DmsHeader*)FS_BUFFER_11);
        vcChangeProjectionValue(Dms_CameraTargetsGet(&g_Cutscene_CameraPositionTarget, &g_Cutscene_CameraLookAtTarget, NULL, g_Cutscene_Timer, (s_DmsHeader*)FS_BUFFER_11));
        vcUserCamTarget(&g_Cutscene_CameraPositionTarget, NULL, true);
        vcUserWatchTarget(&g_Cutscene_CameraLookAtTarget, NULL, true);
    }
}

void func_800D7A60(void) // 0x800D7A60
{
    VECTOR3  pos;
    SVECTOR3 rot;

    // Skip.
    if ((g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.skip) &&
        g_SysWork.sysStateSteps[0] >= 2 && g_SysWork.sysStateSteps[0] <= 15)
    {
        SysWork_StateStepSet(0, 17);
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Fs_QueueStartRead(FILE_ANIM_NURSE2_DMS, FS_BUFFER_11);
            Fs_QueueWaitForEmpty();
            Dms_HeaderFixOffsets((s_DmsHeader*)FS_BUFFER_11);
            Chara_Spawn(Chara_BloodyLisa, 0, g_SysWork.playerWork.player.position.vx, g_SysWork.playerWork.player.position.vz + Q12(1.0f), 0, 3);

            g_Cutscene_Timer = Q12(0.0f);
            D_800E2C64 = 0;
            D_800E1671 = 0;

            SysWork_StateStepIncrement(0);
            break;

        case 1:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 156, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 6, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            SysWork_StateStepIncrement(0);

        case 2:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(0.0f), Q12(43.0f), true, true);
            break;

        case 3:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(44.0f), Q12(59.0f), true, true);
            break;

        case 4:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 157, false);

            g_SysWork.lightBoneCoord     = NULL;
            g_SysWork.lensFlareBoneCoord = NULL;
            g_SysWork.lightIntensity     = Q12(1.0f);

            func_8008D438();
            SysWork_StateStepIncrement(0);

        case 5:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(60.0f), Q12(76.0f), true, true);
            Dms_CharacterTransformGet(&g_SysWork.lightPosition, &rot, "LIGHT", g_Cutscene_Timer, (s_DmsHeader*)FS_BUFFER_11);
            Dms_CharacterTransformGet(&pos, &rot, "L_INT", g_Cutscene_Timer, (s_DmsHeader*)FS_BUFFER_11);
            g_SysWork.lightRotation.vx = -ratan2(pos.vy - g_SysWork.lightPosition.vy, Math_Vector2MagCalcSafeQ6(pos.vx - g_SysWork.lightPosition.vx, pos.vz - g_SysWork.lightPosition.vz));
            g_SysWork.lightRotation.vy = ratan2(pos.vx - g_SysWork.lightPosition.vx, pos.vz - g_SysWork.lightPosition.vz);
            g_SysWork.lightRotation.vz = 0;
            break;

        case 6:
            D_800E1671 = 1;

            func_8008D448();
            Game_FlashlightAttributesFix();

            g_SysWork.lightIntensity = Q12(0.8f);

            Sfx_WithFlagsPlay(Sfx_Unk1336, &g_WorldObject_Door.position, Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 7:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(77.0f), Q12(100.0f), true, true);
            break;

        case 8:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 158, false);
            Chara_ModelCharaIdClear(&g_SysWork.npcs[0], 0, 0);
            Sfx_WithFlagsPlay(Sfx_Unk1337, &g_WorldObject_Door.position, Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 9:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(101.0f), Q12(152.0f), true, false);
            Event_WaitTimer(Q12(0.3f), false);
            break;

        case 10:
            SD_Call(4662);
            SysWork_StateStepIncrement(0);

        case 11:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(101.0f), Q12(142.0f), false, true);
            break;

        case 12:
            SysWork_StateStepIncrement(0);

        case 13:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(101.0f), Q12(152.0f), true, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, false, Q12(0.25f), false);
            break;

        case 14:
            if (Sd_AudioStreamingCheck() != AudioStreamingState_None)
            {
                break;
            }

            SysWork_StateStepIncrement(0);

        case 15:
            Event_WaitTimer(Q12(2.0f), false);
            break;

        case 16:
            SysWork_StateStepSet(0, 19);
            break;

        case 17:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, false, Q12(0.0f), false);
            break;

        case 18:
            SD_Call(19);
            Chara_ModelCharaIdClear(&g_SysWork.npcs[0], 0, 0);
            SysWork_StateStepIncrement(0);
            break;

        case 19:
            GameFs_PlayerMapAnimLoad(g_SavegamePtr->mapIdx);
            Fs_QueueWaitForEmpty();
            SysWork_StateStepIncrement(0);

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);

            g_Cutscene_Timer = NO_VALUE;

            vcReturnPreAutoCamWork(true);
            Savegame_EventFlagSet(EventFlag_484);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            func_8008D448();
            Game_FlashlightAttributesFix();

            g_SysWork.lightIntensity = Q12(1.0f);

            Game_TurnFlashlightOn();
            Player_WeaponAttackRestore();
            func_8003D01C();
            Anim_CharaTypeAnimInfoClear();
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, false, Q12(1.0f), false);
            break;
    }

    if (g_Cutscene_Timer >= Q12(0.0f))
    {
        Dms_CharacterTransformGet(&g_SysWork.playerWork.player.position, &g_SysWork.playerWork.player.rotation, "HERO", g_Cutscene_Timer, (s_DmsHeader*)FS_BUFFER_11);
        Dms_CharacterTransformGet(&g_SysWork.npcs[0].position, &g_SysWork.npcs[0].rotation, "BLS", g_Cutscene_Timer, (s_DmsHeader*)FS_BUFFER_11);

        if (D_800E1671 != 0)
        {
            Dms_CharacterTransformGet(&g_WorldObject_Door.position, &g_WorldObject_Door.rotation, "ADOOR", g_Cutscene_Timer, (s_DmsHeader*)FS_BUFFER_11);
        }

        vcChangeProjectionValue(Dms_CameraTargetsGet(&g_Cutscene_CameraPositionTarget, &g_Cutscene_CameraLookAtTarget, NULL, g_Cutscene_Timer, (s_DmsHeader*)FS_BUFFER_11));
        vcUserCamTarget(&g_Cutscene_CameraPositionTarget, NULL, true);
        vcUserWatchTarget(&g_Cutscene_CameraLookAtTarget, NULL, true);
    }
}

void func_800D8230(void) // 0x800D8230
{
    VECTOR3 sfxPos = { MAP_POINTS[g_MapEventData->mapPointIdx].positionX, Q12(-1.2f), MAP_POINTS[g_MapEventData->mapPointIdx].positionZ };

    Event_DisplayMapMsgWithSfx(MapMsgIdx_DoorJammed, Sfx_DoorJammed, &sfxPos);
}

void func_800D82C4(void) // 0x800D82C4
{
    VECTOR3 sfxPos = { MAP_POINTS[g_MapEventData->mapPointIdx].positionX, Q12(-1.2f), MAP_POINTS[g_MapEventData->mapPointIdx].positionZ };

    Event_DisplayMapMsgWithSfx(MapMsgIdx_DoorLocked, Sfx_DoorLocked, &sfxPos);
}

void func_800D8358(void) // 0x800D8358
{
    VECTOR3 sfxPos = { MAP_POINTS[g_MapEventData->mapPointIdx].positionX, Q12(-1.2f), MAP_POINTS[g_MapEventData->mapPointIdx].positionZ };

    Event_DisplayMapMsgWithSfx(MapMsgIdx_DoorUnlocked, Sfx_DoorUnlocked, &sfxPos);
}

void MapEvent_CommonItemTake(void) // 0x800D83EC
{
    u32 pickupType;
    s32 eventFlagIdx;

    pickupType   = CommonPickupItemId_FirstAidKit;
    eventFlagIdx = 0;

    switch (g_MapEventData->mapPointIdx)
    {
        case 170:
            pickupType   = CommonPickupItemId_RifleShells;
            eventFlagIdx = EventFlag_M7S01_RifleShells;
            break;

        case 171:
            pickupType   = CommonPickupItemId_HandgunBullets;
            eventFlagIdx = EventFlag_M7S01_HandgunBullets0;
            break;

        case 172:
            pickupType   = CommonPickupItemId_HealthDrink;
            eventFlagIdx = EventFlag_M7S01_HealthDrink0;
            break;

        case 173:
            pickupType   = CommonPickupItemId_HandgunBullets;
            eventFlagIdx = EventFlag_M7S01_HandgunBullets1;
            break;

        case 174:
            pickupType   = CommonPickupItemId_ShotgunShells;
            eventFlagIdx = EventFlag_M7S01_ShotgunShells;
            break;

        case 175:
            pickupType   = CommonPickupItemId_FirstAidKit;
            eventFlagIdx = EventFlag_M7S01_FirstAidKit;
            break;

        case 176:
            pickupType   = CommonPickupItemId_HandgunBullets;
            eventFlagIdx = EventFlag_M7S01_HandgunBullets2;
            break;

        case 177:
            pickupType   = CommonPickupItemId_HealthDrink;
            eventFlagIdx = EventFlag_M7S01_HealthDrink1;
            break;
    }

    Event_CommonItemTake(pickupType, eventFlagIdx);
}

void func_800D84AC(void) // 0x800D84AC
{
    VECTOR3 sfxPos = { MAP_POINTS[g_MapEventData->mapPointIdx].positionX, Q12(-1.2f), MAP_POINTS[g_MapEventData->mapPointIdx].positionZ };

    Event_DisplayMapMsgWithSfx(MAP_MSG(81), Sfx_DoorLocked, &sfxPos);
}

void func_800D8540(void) // 0x800D8540
{
    VECTOR3 sfxPos = { MAP_POINTS[g_MapEventData->mapPointIdx].positionX, Q12(-1.2f), MAP_POINTS[g_MapEventData->mapPointIdx].positionZ };

    Event_DisplayMapMsgWithSfx(MAP_MSG(83), Sfx_DoorLocked, &sfxPos);
}

void func_800D85D4(void) // 0x800D85D4
{
    VECTOR3 sfxPos = { MAP_POINTS[g_MapEventData->mapPointIdx].positionX, Q12(-1.2f), MAP_POINTS[g_MapEventData->mapPointIdx].positionZ };

    Event_DisplayMapMsgWithSfx(MAP_MSG(85), Sfx_DoorLocked, &sfxPos);
}

void MapEvent_KeyOfOphielUse(void) // 0x800D8668
{
    VECTOR3 sfxPos = { MAP_POINTS[g_MapEventData->mapPointIdx].positionX, Q12(-1.2f), MAP_POINTS[g_MapEventData->mapPointIdx].positionZ };

    Player_ItemRemove(InvItemId_KeyOfOphiel, 1);
    Event_DisplayMapMsgWithSfx(MAP_MSG(48), Sfx_UseKey, &sfxPos);
    Savegame_EventFlagSet(EventFlag_530);
}

void MapEvent_KeyOfHagithUse(void) // 0x800D8724
{
    VECTOR3 sfxPos = { MAP_POINTS[g_MapEventData->mapPointIdx].positionX, Q12(-1.2f), MAP_POINTS[g_MapEventData->mapPointIdx].positionZ };

    Player_ItemRemove(InvItemId_KeyOfHagith, 1);
    Event_DisplayMapMsgWithSfx(MAP_MSG(49), Sfx_UseKey, &sfxPos);
    Savegame_EventFlagSet(EventFlag_531);
}

void MapEvent_KeyOfPhalegUse(void) // 0x800D87E0
{
    VECTOR3 sfxPos = { MAP_POINTS[g_MapEventData->mapPointIdx].positionX, Q12(-1.2f), MAP_POINTS[g_MapEventData->mapPointIdx].positionZ };

    Player_ItemRemove(InvItemId_KeyOfPhaleg, 1);
    Event_DisplayMapMsgWithSfx(MAP_MSG(50), Sfx_UseKey, &sfxPos);
    Savegame_EventFlagSet(EventFlag_532);
}

void func_800D889C(void) // 0x800D889C
{
    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            D_800E2C64 = 0;
            SysWork_StateStepIncrement(0);

        case 1:
            Event_DisplayMapMsgWithAudio(15, &D_800E2C64, &D_800E1500);
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;
    }
}

void func_800D8958(void) // 0x800D8958
{
    Event_DisplayMapMsgWithBg(FILE_TIM_BIRDCAGE_TIM, Q12(0.0f), Q12(0.0f), MAP_MSG(78));
}

void MapEvent_BirdCageKeyUse(void) // 0x800D8984
{
    const static VECTOR3 sfxPos = { Q12(-179.64f), Q12(-1.0f), Q12(-20.05f) };

    Event_DisplayMapMsgWithSfx(MAP_MSG(79), Sfx_Unk1645, &sfxPos);
    Player_ItemRemove(InvItemId_BirdCageKey, 1);
    Savegame_EventFlagSet(EventFlag_491);
}

void func_800D89D8(void) // 0x800D89D8
{
    Event_ItemTake(InvItemId_KeyOfPhaleg, DEFAULT_PICKUP_ITEM_COUNT, EventFlag_M7S01_PickupKeyOfPhaleg, MAP_MSG(59));
}

void func_800D8A04(void) // 0x800D8A04
{
    Event_DisplayMapMsgWithBg(FILE_TIM_CAGEEMPT_TIM, Q12(0.0f), Q12(0.0f), MAP_MSG(80));
}

void func_800D8A30(void) // 0x800D8A30
{
    Event_DisplayMapMsgWithBg(FILE_TIM_WATERTAP_TIM, Q12(0.0f), Q12(0.0f), MAP_MSG(68));
}

void func_800D8A5C(void) // 0x800D8A5C
{
    const static VECTOR3 D_800CC534 = { Q12(-140.05f), Q12(-1.4f), Q12(-22.15f) };
    const static VECTOR3 D_800CC540 = { Q12(-140.05f), Q12(0.0f), Q12(-22.15f) };

    s32 i;

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();

            D_800E1678[0] = Q12(1.9f);
            D_800E1678[1] = Q12(1.5f);
            D_800E1678[2] = Q12(1.5f);
            D_800E1678[3] = Q12(0.4f);

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 2, Q12(0.0f), false);
            Player_EmptyWeaponHandSet();
            func_8003D03C();
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            Event_ScreenFadeCmd(ScreenFadeCmd_Wait, true, 2, Q12(0.0f), false);
            break;

        case 3:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 145, false);
            Event_CameraPositionSet(NULL, Q12(-139.38f), Q12(-0.92f), Q12(-19.99f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(NULL, Q12(-141.0f), Q12(-1.64f), Q12(-23.58f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);

            g_SysWork.playerWork.player.position.vx = Q12(-140.0f);
            g_SysWork.playerWork.player.position.vz = Q12(-21.7f);
            g_SysWork.playerWork.player.rotation.vy = Q12(0.5f);

            SysWork_StateStepIncrement(0);

        case 4:
            for (i = 0; i < ARRAY_SIZE(D_800E1678); i++)
            {
                if (D_800E1678[i] > 0)
                {
                    D_800E1678[i] -= g_DeltaTime;

                    if (D_800E1678[i] > 0)
                    {
                        break;
                    }

                    D_800E1678[i] = 0;

                    if (i == 3)
                    {
                        SysWork_StateStepIncrement(0);
                        break;
                    }

                    Sfx_WithFlagsPlay(Sfx_Unk1642, &D_800CC534, Q8(0.5f), SfxFlag_None);
                    break;
                }
            }
            break;

        case 5:
            Sfx_WithFlagsPlay(Sfx_Unk1643, &D_800CC540, Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 6:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AwaitAnimEnd, &g_SysWork.playerWork.player, 0, false);
            break;

        case 7:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            Player_WeaponAttackRestore();
            func_8003D01C();
            vcReturnPreAutoCamWork(true);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            Savegame_EventFlagSet(EventFlag_490);
            break;
    }
}

void func_800D8DB4(void) // 0x800D8DB4
{
    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_InvItemCmd(InvItemCmd_QueueLoad, InvItemId_KeyOfOphiel, 0, false);
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 59, false);
            SysWork_StateStepIncrement(0);

        case 3:
            Event_InvItemCmd(InvItemCmd_AwaitLoad, InvItemId_KeyOfOphiel, 0, false);
            break;

        case 4:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AwaitAnimEnd, &g_SysWork.playerWork.player, 0, false);
            break;

        case 5:
            if (Gfx_PickupItemAnimate(InvItemId_KeyOfOphiel))
            {
                Event_DisplayMapMsg(true, MAP_MSG(57), 6, 7, 0, false);
            }

            Savegame_EventFlagSet(EventFlag_525);
            break;

        case 6:
            Event_InvItemCmd(InvItemCmd_AddItem, InvItemId_KeyOfOphiel, 1, false);
            SysWork_StateStepSet(0, 8);
            break;

        case 7:
            Savegame_EventFlagClear(EventFlag_525);
            SysWork_StateStepIncrement(0);

        case 8:
            Event_CharaAnimPlayToEnd(&g_SysWork.playerWork.player, 60);
            break;

        default: // `EventState_Finish`
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;
    }
}

void func_800D8FCC(void) // 0x800D8FCC
{
    Event_DisplayMapMsgWithBg(FILE_TIM_CLOCKKEY_TIM, Q12(0.0f), Q12(0.0f), MAP_MSG(65));
}

void func_800D8FF8(void) // 0x800D8FF8
{
    const static VECTOR3 D_800CC594 = { Q12(-16.6661f), Q12(-1.234f), Q12(-61.49756f) };
    const static VECTOR3 D_800CC5A0 = { Q12(-16.6661f), Q12(-1.234f), Q12(-61.69751f) };
    const static VECTOR3 D_800CC5AC = { Q12(-15.8f), Q12(-1.23f), Q12(-60.22f) };
    const static VECTOR3 D_800CC5B8 = { Q12(-16.7f), Q12(-1.5f), Q12(-60.8f) };

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            ScreenFade_ResetTimestep();
            CutsceneBorder_ForceShow();

            Fs_QueueStartReadTim(FILE_TIM_GLASS_TIM, FS_BUFFER_1, &D_800A908C);
            Savegame_EventFlagSet(EventFlag_563);
            Sfx_WithFlagsPlay(Sfx_Unk1660, &D_800E1504, Q8(0.5f), SfxFlag_None);

            D_800E1680 = 0;

            // Warp camera.
            Event_CameraPositionSet(NULL, Q12(-15.39f), Q12(-3.6f), Q12(-61.47f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(NULL, Q12(-17.38f), Q12(-0.18f), Q12(-60.85f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);

            // Warp player.
            g_SysWork.playerWork.player.position.vx = Q12(-16.7f);
            g_SysWork.playerWork.player.position.vz = Q12(-61.15f);
            g_SysWork.playerWork.player.rotation.vy = Q12(0.5f);

            SysWork_StateStepIncrement(0);

        case 1:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.0f), false);
            break;

        case 3:
            if (Fs_QueueChunksLoad() != false)
            {
                SysWork_StateStepIncrement(0);
            }
            break;

        case 4:
            if (g_SysWork.sysStateStepData[0] == Q12(0.0f))
            {
                Sfx_WithFlagsPlay(Sfx_Unk1661, &D_800E1504, Q8(0.5f), SfxFlag_None);

                D_800E1680++;
                if (D_800E1680 == 5)
                {
                    SysWork_StateStepIncrement(0);
                }
                else
                {
                    g_SysWork.sysStateStepData[0]++;
                }
                break;
            }

            g_SysWork.sysStateStepData[0] += g_DeltaTime;
            if (g_SysWork.sysStateStepData[0] > Q12(1.4f))
            {
                g_SysWork.sysStateStepData[0] = Q12(0.0f);
            }
            break;

        case 5:
            Event_WaitTimer(Q12(2.5f), false);
            break;

        case 6:
            sharedFunc_800CCB8C_0_s01(&D_800CC594, &D_800CC5A0, Q12(2.4f), Q12(0.022f), Q12(0.18f), Q12(0.2f), Q12(0.05f), 0);
            Sfx_WithFlagsPlay(Sfx_Unk1658, &D_800CC5AC, Q8(0.5f), SfxFlag_None);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 167, false);
            SysWork_StateStepIncrement(0);

        case 7:
            Event_WaitTimer(Q12(0.2f), false);
            break;

        case 8:
            Sfx_WithFlagsPlay(Sfx_Unk1327, &D_800CC5B8, Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 2:
        case 9:
            Event_WaitTimer(Q12(1.0f), false);
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            vcReturnPreAutoCamWork(true);

            // Warp player.
            g_SysWork.playerWork.player.position.vz = Q12(-60.7f);

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            Savegame_EventFlagSet(EventFlag_489);
            break;
    }
}

void func_800D93BC(void) // 0x800D93BC
{
    Event_ItemTake(InvItemId_KeyOfHagith, DEFAULT_PICKUP_ITEM_COUNT, EventFlag_M7S01_PickupKeyOfHagith, MAP_MSG(58));
}

void func_800D93E8(void) // 0x800D93E8
{
    Event_DisplayMapMsgWithBg(FILE_TIM_ABUSEDSK_TIM, Q12(0.0f), Q12(0.0f), MAP_MSG(62));
}

void func_800D9414(void) // 0x800D9414
{
    Event_ItemTake(InvItemId_Pliers, DEFAULT_PICKUP_ITEM_COUNT, EventFlag_M7S01_PickupPliers, MAP_MSG(52));
}

void func_800D9440(void) // 0x800D9440
{
    Event_ItemTake(InvItemId_Screwdriver, DEFAULT_PICKUP_ITEM_COUNT, EventFlag_M7S01_PickupScrewdriver, MAP_MSG(53));
}

void func_800D946C(void) // 0x800D946C
{
    g_Screen_BackgroundImgGamma = Q8(13.0f / 32.0f);
    Event_DisplayMapMsgWithBg(FILE_TIM_LITHGRPH_TIM, Q12(0.0f), Q12(0.0f), MAP_MSG(70));
}

void func_800D94A4(void) // 0x800D94A4
{
    g_Screen_BackgroundImgGamma = Q8(13.0f / 32.0f);
    Event_DisplayMapMsgWithBg(FILE_TIM_LITHGR_3_TIM, Q12(0.0f), Q12(0.0f), MAP_MSG(71));
}

void func_800D94DC(void) // 0x800D94DC
{
    s32         i;
    s32         j;

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 0, Q12(0.0f), false);
            sharedData_800E2CAC_7_s01 = 0;
            sharedData_800E2CA8_7_s01 = 0;

            for (i = 0; i < ARRAY_SIZE(D_800E1688); i++)
            {
                D_800E1688[i] = 26; // Sets all 5 values to length of `D_800E1510` array?
            }

            SysWork_StateStepIncrement(0);

        case 1:
            Event_BgTextureCmd(BgTextureCmd_Auto, FILE_TIM_ALERTDOR_TIM, false);
            break;

        case 2:
            Event_ScreenFadeCmd(ScreenFadeCmd_Wait, true, 0, Q12(0.0f), false);
            break;

        case 3:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.0f), false);
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            break;

        case 4:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);

            sharedData_800E2CA8_7_s01 += (g_Controller0->normalizedSticks.sticks_0.leftX * 16384) / 75;
            sharedData_800E2CA8_7_s01  = CLAMP_RANGE(sharedData_800E2CA8_7_s01, Q12(-120.0f), Q12(120.0f));

            sharedData_800E2CAC_7_s01 += (g_Controller0->normalizedSticks.sticks_0.leftY * 16384) / 75;
            sharedData_800E2CAC_7_s01  = CLAMP_RANGE(sharedData_800E2CAC_7_s01, Q12(-120.0f), Q12(120.0f));

            Game_TimerUpdate();
            Gfx_CursorDraw((s16)(FP_FROM(sharedData_800E2CA8_7_s01, Q12_SHIFT) + 8), FP_FROM(sharedData_800E2CAC_7_s01, Q12_SHIFT) + 8, 8, 8, 0, 64, 32, 32, 128, 192, 0, 12);

            if (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.cancel)
            {
                SysWork_StateStepSet(0, 8);
                break;
            }

            if (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.enter)
            {
                for (i = 0; i < ARRAY_SIZE(D_800E1510); i++)
                {
                    if (D_800E1510[i].field_0 - 174 > FP_FROM(sharedData_800E2CA8_7_s01, Q12_SHIFT) ||
                        D_800E1510[i].field_0 - 146 < FP_FROM(sharedData_800E2CA8_7_s01, Q12_SHIFT))
                    {
                        continue;
                    }

                    if (D_800E1510[i].field_1 - 134 > FP_FROM(sharedData_800E2CAC_7_s01, Q12_SHIFT) ||
                        D_800E1510[i].field_1 - 106 < FP_FROM(sharedData_800E2CAC_7_s01, Q12_SHIFT))
                    {
                        continue;
                    }

                    if (SQUARE(D_800E1510[i].field_0 - 160 - FP_FROM(sharedData_800E2CA8_7_s01, Q12_SHIFT)) +
                            SQUARE(D_800E1510[i].field_1 - 120 - FP_FROM(sharedData_800E2CAC_7_s01, Q12_SHIFT)) >=
                        197)
                    {
                        continue;
                    }

                    SD_Call(Sfx_Unk1650);

                    for (j = 0; j < (ARRAY_SIZE(D_800E1688) - 1); j++)
                    {
                        D_800E1688[j] = D_800E1688[j + 1];
                    }

                    D_800E1688[4] = i;

                    for (j = 0; j < ARRAY_SIZE(D_800E1688); j++)
                    {
                        if (D_800E1688[j] != D_800E1544[j])
                        {
                            break;
                        }
                    }

                    if (j == ARRAY_SIZE(D_800E1688))
                    {
                        Savegame_EventFlagSet(EventFlag_488);

                        SysWork_StateStepSet(0, 5);
                    }
                }
            }
            break;

        case 6:
            SD_Call(Sfx_Unk1343);
            SysWork_StateStepIncrement(0);

        case 5:
        case 7:
            Event_WaitTimer(Q12(0.6f), false);
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            break;

        case 8:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            break;
    }
}

void func_800D99DC(void) // 0x800D99DC
{
    Event_ItemTake(InvItemId_AmuletOfSolomon, DEFAULT_PICKUP_ITEM_COUNT, EventFlag_M7S01_PickupAmuletOfSolomon, MAP_MSG(60));
}

void func_800D9A08(void) // 0x800D9A08
{
    g_Screen_BackgroundImgGamma = Q8(9.0f / 32.0f);

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_BgTextureCmd(BgTextureCmd_QueueRead, FILE_TIM_LISDIARY_TIM, false);

            D_800E168D = 0;

            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            Event_CharaAnimPlayToEnd(&g_SysWork.playerWork.player, 59);
            break;

        case 3:
            Event_DisplayMapMsg(false, MAP_MSG(92), 0, 0, 0, false);
            break;

        case 4:
            Event_BgTextureCmd(BgTextureCmd_AwaitLoad, 0, false);
            break;

        case 5:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 6:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.0f), false);
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            break;

        case 7:
            SD_Call(Sfx_MenuMap);
            SysWork_StateStepIncrement(0);

        case 8:
            Event_WaitTimer(Q12(0.3f), false);
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            break;

        case 9:
            Event_DisplayMapMsg(false, D_800E154C[D_800E168D], 0, 0, 0, false);
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            break;

        case 10:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);

            D_800E168D++;
            if (D_800E168D >= 8)
            {
                SysWork_StateStepIncrement(0);
                break;
            }

            SysWork_StateStepSet(0, 7);
            break;

        case 11:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            break;

        case 12:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.0f), false);
            break;

        case 13:
            Event_CharaAnimPlayToEnd(&g_SysWork.playerWork.player, 60);
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;
    }
}

void func_800D9C9C(void) // 0x800D9C9C
{
    // @note Mirror any changes to `map7_s02::func_800DFDDC` (likely copy-pasted func with some small differences).
    s32       i;
    s32       j;
    s32       temp_a0;
    s32       temp_a3;
    s32       temp_t0;
    s32       temp_v0_2;
    s32       temp_v0_3;
    s32       temp_v1;
    s32       var_v0_3;
    s32       var_v0_4;
    s32       var_v0;
    SPRT*     sprt;
    DR_TPAGE* tPage;

    if (D_800E156E != 0)
    {
        if ((g_SysWork.sysStateSteps[0] >= 8 && g_SysWork.sysStateSteps[0] <= 10) || g_SysWork.sysStateSteps[0] == 15)
        {
            sprt = (SPRT*)GsOUT_PACKET_P;
            setCodeWord(sprt, PRIM_RECT | RECT_TEXTURE, COLOR_RGBC(128, 128, 128, 0));
            setXY0Fast(sprt, (((D_800E156E - 1) % 3) * 35) - 42, (((D_800E156E - 1) / 3) * 35) - 32);
            setUV0(sprt, 64, 16);
            setWH(sprt, 14, 14);
            sprt->clut = 0x38E; // TODO: Is this calculated from something?

            addPrimFast(&g_OrderingTable0[g_ActiveBufferIdx].org[1], sprt, 4);

            sprt++;

            tPage = (DR_TPAGE*)sprt;
            setDrawTPage(tPage, 0, 0, 135);
            AddPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[1], tPage);

            tPage++;

            GsOUT_PACKET_P = (PACKET*)tPage;
        }
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            D_800E156E         = 0;
            D_800E1690.field_0 = 0;

            while (true)
            {
                if (Savegame_EventFlagGet(EventFlag_500 + D_800E1690.field_0))
                {
                    break;
                }

                D_800E1690.field_0++;
            }

            Event_BgTextureCmd(BgTextureCmd_QueueRead, D_800E1560[D_800E1690.field_0], false);
            SysWork_StateStepIncrement(0);

        case 1:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);

        case 2:
            Event_BgTextureCmd(BgTextureCmd_AwaitLoad, 0, false);
            break;

        case 3:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.0f), false);
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            break;

        case 4:
            if (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.enter)
            {
                if (!Savegame_EventFlagGet(EventFlag_487) && (D_800E1690.field_0 == 1 || D_800E1690.field_0 == 5 || D_800E1690.field_0 == 6))
                {
                    SysWork_StateStepSet(0, 5);
                }
                else
                {
                    SysWork_StateStepSet(0, 15);
                }
            }
            else if (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.cancel)
            {
                SysWork_StateStepSet(0, 15);
            }

            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            break;

        case 6:
            Fs_QueueStartReadTim(FILE_TIM_HS_PANEL_TIM, IMAGE_BUFFER_5, &g_ItemInspectionImg);
            Fs_QueueWaitForEmpty();
            SysWork_StateStepIncrement(0);

        case 7:
            if (D_800E1690.field_0 == 1)
            {
                var_v0 = EventFlag_507;
            }
            else if (D_800E1690.field_0 == 5)
            {
                var_v0 = EventFlag_511;
            }
            else
            {
                var_v0 = EventFlag_515;
            }

            for (i = 0; i < 4; i++)
            {
                if (Savegame_EventFlagGet(var_v0 + i))
                {
                    D_800E156E += 1 << i;
                }
            }

            sharedData_800E2CA8_7_s01 = 0;
            sharedData_800E2CAC_7_s01 = 0;
            SysWork_StateStepIncrement(0);

        case 8:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.0f), false);
            break;

        case 9:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);

            sharedData_800E2CA8_7_s01 += (g_Controller0->normalizedSticks.sticks_0.leftX * 16384) / 75;
            sharedData_800E2CA8_7_s01  = CLAMP_RANGE(sharedData_800E2CA8_7_s01, Q12(-80.0f), Q12(80.0f));

            sharedData_800E2CAC_7_s01 += (g_Controller0->normalizedSticks.sticks_0.leftY * 16384) / 75;
            sharedData_800E2CAC_7_s01  = CLAMP_RANGE(sharedData_800E2CAC_7_s01, Q12(-80.0f), Q12(80.0f));

            Game_TimerUpdate();

            // TODO: Should `Gfx_CursorDraw` first args be `s16`?
            Gfx_CursorDraw((s16)(FP_FROM(sharedData_800E2CA8_7_s01, 12) + 8), (s16)FP_FROM(sharedData_800E2CAC_7_s01, 12) + 8, 8, 8, 0, 0x40, 0x20, 0x20, 0x80, 0xC0, 0, 0xC);

            if (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.cancel)
            {
                SysWork_StateStepSet(0, 15);
                break;
            }

            if (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.enter)
            {
                for (i = 0; i < 3; i++)
                {
                    for (j = 0; j < 3; j++)
                    {
                        // TODO: Not sure what's with these multiplies/subtractions, something to do with menu selections?
                        temp_t0  = j * 35;
                        var_v0_3 = temp_t0 - 46; // Only matches as separate var, TODO: add another for `- 24`?
                        temp_a3  = FP_FROM(sharedData_800E2CA8_7_s01, 12);
                        if (temp_a3 < var_v0_3 || temp_a3 > (temp_t0 - 24))
                        {
                            continue;
                        }

                        temp_a0 = i * 35;
                        var_v0_4  = temp_a0 - 37;
                        temp_v1   = FP_FROM(sharedData_800E2CAC_7_s01, 12);
                        if (temp_v1 < var_v0_4 || temp_v1 > (temp_a0 - 15))
                        {
                            continue;
                        }

                        temp_v0_2 = temp_a3 + 35;
                        temp_v0_2 = temp_t0 - temp_v0_2;

                        temp_v0_3 = temp_v1 + 26;
                        temp_v0_3 = temp_a0 - temp_v0_3;

                        if (SQUARE(temp_v0_2) + SQUARE(temp_v0_3) < 122)
                        {
                            D_800E156E = j + (i * 3) + 1;
                            SD_Call(Sfx_Unk1644);

                            // @hack Maybe not meant to reuse `temp_a0_2`, but that helps it to match.
                            if (D_800E1690.field_0 == 1)
                            {
                                temp_a0 = EventFlag_507;
                            }
                            else if (D_800E1690.field_0 == 5)
                            {
                                temp_a0 = EventFlag_511;
                            }
                            else
                            {
                                temp_a0 = EventFlag_515;
                            }

                            // @bug This is overwriting `i` from the outer loop?
                            for (i = 0; i < 4; i++)
                            {
                                if ((D_800E156E >> i) & 1)
                                {
                                    Savegame_EventFlagSet(temp_a0 + i);
                                }
                                else
                                {
                                    Savegame_EventFlagClear(temp_a0 + i);
                                }
                            }

                            for (i = 0, j = 0; i < 12; i++)
                            {
                                if (Savegame_EventFlagGet(EventFlag_507 + i))
                                {
                                    j += 1 << i;
                                }
                            }

                            if (D_800E155C == j)
                            {
                                SysWork_StateStepSet(0, 10);
                            }
                        }
                    }
                }
            }
            break;

        case 11:
            CutsceneBorder_ForceShow();
            D_800E168E         = Q12(0.0f);

            Model_AnimFlagsClear(&g_SysWork.playerWork.player.model, AnimFlag_Visible);
            func_8008D438();
            Event_CameraPositionSet(NULL, Q12(-138.15f), Q12(-1.71f), Q12(-138.77f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(NULL, Q12(-140.56f), Q12(-1.03f), Q12(-141.88f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            WorldObject_ModelNameSet(&g_WorldObject_Stone0.object, "STONE2_H");
            SysWork_StateStepIncrement(0);

        case 12:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.0f), false);
            break;

        case 13:
            switch (g_SysWork.sysStateSteps[1])
            {
                case 0:
                    Sfx_WithFlagsPlay(Sfx_Unk1647, &g_WorldObject_Stone0.position, Q8(0.5f), SfxFlag_None);
                    SysWork_StateStepIncrement(1);

                case 1:
                    g_SysWork.sysStateStepData[0] += g_DeltaTime;
                    if (g_SysWork.sysStateStepData[0] > Q12(2.0f))
                    {
                        g_SysWork.sysStateStepData[0] = Q12(2.0f);

                        WorldObject_ModelNameSet(&g_WorldObject_Stone0.object, "STONE3_H");
                        SysWork_StateStepIncrement(1);
                        Sd_SfxStop(Sfx_Unk1647);
                    }

                    g_WorldObject_Stone0.position.vz = ((g_SysWork.sysStateStepData[0] * Q12(0.1f)) / Q12(2.0f)) - Q12(140.6f);
                    break;

                case 2:
                    D_800E168E += g_GravitySpeed >> 1;

                    if (g_WorldObject_Stone0.position.vy < Q12(-1.05f))
                    {
                        g_WorldObject_Stone0.position.vy += Q12_MULT_PRECISE(g_DeltaTime, D_800E168E);
                        if (g_WorldObject_Stone0.position.vy > Q12(-1.05f))
                        {
                            g_WorldObject_Stone0.position.vy = Q12(-1.05f);

                            SysWork_StateStepIncrement(1);
                            g_SysWork.sysStateStepData[0] = Q12(0.0f);
                            D_800E168E         = Q12(0.0f);

                            Sfx_WithFlagsPlay(Sfx_Unk1648, &g_WorldObject_Stone0.position, Q8(0.5f), SfxFlag_None);
                        }
                    }
                    break;

                case 3:
                    g_WorldObject_Stone0.rotation.vz += Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(-25.0f));
                    g_WorldObject_Stone0.position.vy  = Q12_MULT(Math_Cos(g_WorldObject_Stone0.rotation.vz), Q12(0.15f)) - (Q12(1.2f) - 1);
                    g_WorldObject_Stone0.position.vz  = Q12_MULT(Math_Sin(g_WorldObject_Stone0.rotation.vz), Q12(-0.15f)) - Q12(140.5f);

                    g_SysWork.sysStateStepData[0] += g_DeltaTime;
                    if (g_SysWork.sysStateStepData[0] > Q12(0.5f))
                    {
                        SysWork_StateStepIncrement(1);
                        g_SysWork.sysStateStepData[0] = Q12(0.0f);
                    }
                    break;

                case 4:
                    D_800E168E                          += g_GravitySpeed;
                    g_WorldObject_Stone0.rotation.vz += Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(-120.0f));
                    g_WorldObject_Stone0.position.vy += Q12_MULT_PRECISE(D_800E168E, g_DeltaTime);
                    g_WorldObject_Stone0.position.vz += Q12_MULT_PRECISE(g_DeltaTime, Q12(0.3f));

                    if (g_WorldObject_Stone0.position.vy > Q12(0.0f))
                    {
                        SysWork_StateStepIncrement(1);
                    }
                    break;

                case 5:
                    g_WorldObject_Stone0.position.vx = Q12(-139.5f);
                    g_WorldObject_Stone0.position.vy = Q12(-0.05f);
                    g_WorldObject_Stone0.position.vz = Q12(-140.0f);
                    g_WorldObject_Stone0.rotation.vz = Q12(-0.25f);

                    Sfx_WithFlagsPlay(Sfx_Unk1649, &g_WorldObject_Stone0.position, Q8(0.5f), SfxFlag_None);
                    SysWork_StateStepIncrement(0);
                    break;
            }
            break;

        case 14:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            vcReturnPreAutoCamWork(true);
            func_8008D448();

            Model_AnimFlagsSet(&g_SysWork.playerWork.player.model, AnimFlag_Visible);
            Savegame_EventFlagClear(EventFlag_500 + D_800E1690.field_0);
            Savegame_EventFlagSet(EventFlag_487);

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            break;

        case 5:
        case 10:
        case 15:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);

            Savegame_EventFlagClear(EventFlag_500 + D_800E1690.field_0);
            break;
    }
}

void func_800DAB64(void) // 0x800DAB64
{
    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_InvItemCmd(InvItemCmd_QueueLoad, InvItemId_StoneOfTime, 0, false);
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 59, false);
            SysWork_StateStepIncrement(0);

        case 3:
            Event_InvItemCmd(InvItemCmd_AwaitLoad, InvItemId_StoneOfTime, 0, false);
            break;

        case 4:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AwaitAnimEnd, &g_SysWork.playerWork.player, 0, false);
            break;

        case 5:
            if (Gfx_PickupItemAnimate(InvItemId_StoneOfTime) != false)
            {
                Event_DisplayMapMsg(true, MAP_MSG(56), 6, 7, 0, false);
            }

            Savegame_EventFlagSet(EventFlag_M7S01_PickupStoneOfTime);
            break;

        case 6:
            Event_InvItemCmd(InvItemCmd_AddItem, InvItemId_StoneOfTime, 1, false);
            SysWork_StateStepSet(0, 8);
            break;

        case 7:
            Savegame_EventFlagClear(EventFlag_M7S01_PickupStoneOfTime);
            SysWork_StateStepIncrement(0);

        case 8:
            Event_CharaAnimPlayToEnd(&g_SysWork.playerWork.player, 60);
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;
    }
}

void func_800DAD7C(void) // 0x800DAD7C
{
    Event_ItemTake(InvItemId_CrestOfMercury, DEFAULT_PICKUP_ITEM_COUNT, EventFlag_M7S01_PickupCrestOfMercury, MAP_MSG(61));
}

void func_800DADA8(void) // 0x800DADA8
{
    Event_ItemTake(InvItemId_RingOfContract, DEFAULT_PICKUP_ITEM_COUNT, EventFlag_M7S01_PickupRingOfContract, MAP_MSG(51));
}

void MapEvent_CutsceneCherylRedirect3(void) // 0x800DADD4
{
    Event_ItemTake(InvItemId_Camera, DEFAULT_PICKUP_ITEM_COUNT, EventFlag_M7S01_PickupCamera, MAP_MSG(55));
}

void func_800DAE00(void) // 0x800DAE00
{
    const static VECTOR3 SFX_POS_0 = { Q12(-63.2f), Q12(-1.45f), Q12(-139.05f) };
    const static VECTOR3 SFX_POS_1 = { Q12(-62.9f), Q12(0.0f), Q12(-139.1f) };

    s16* var_s0;

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();

            CutsceneBorder_ForceShow();
            g_SysWork.sysFlags |= SysFlag_CutsceneActive;

            Fs_QueueStartRead(FILE_ANIM_DRIVR_DMS, (void*)FS_BUFFER_11);
            Fs_QueueWaitForEmpty();
            Dms_HeaderFixOffsets((s_DmsHeader*)FS_BUFFER_11);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 148, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);

            D_800E1690.field_2 = Q12(0.3f);
            g_Cutscene_Timer         = Q12(0.0f);

            SysWork_StateStepIncrement(0);

        case 1:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(0.0f), Q12(31.0f), false, true);

            // TODO: Possible inline?
            var_s0 = &D_800E1690.field_2;
            if (*var_s0 == 0)
            {
                Sfx_WithFlagsPlay(Sfx_Unk1656, &SFX_POS_0, Q8(0.5f), SfxFlag_None);
                *var_s0 = Q12(1.2f);
            }
            else
            {
                *var_s0 -= g_DeltaTime;
                if (*var_s0 < 0)
                {
                    *var_s0 = 0;
                }
            }
            break;

        case 2:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(0.0f), Q12(32.0f), true, false);

            // TODO: Possible inline?
            var_s0 = &D_800E1690.field_2;
            if (*var_s0 == 0)
            {
                Sfx_WithFlagsPlay(Sfx_Unk1656, &SFX_POS_0, Q8(0.5f), SfxFlag_None);
                *var_s0 = Q12(1.2f);
            }
            else
            {
                *var_s0 -= g_DeltaTime;
                if (*var_s0 < Q12(0.0f))
                {
                    *var_s0 = Q12(0.0f);
                }
            }
            break;

        case 3:
            Sfx_WithFlagsPlay(Sfx_Unk1655, &SFX_POS_1, Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 4:
            Event_WaitTimer(Q12(0.5f), false);
            break;

        default:
            Player_ControlUnfreeze(true);
            SysWork_StateSetNext(SysState_Gameplay);
            vcReturnPreAutoCamWork(true);

            g_Cutscene_Timer = NO_VALUE;

            Savegame_EventFlagSet(EventFlag_486);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            break;
    }

    if (g_Cutscene_Timer >= Q12(0.0f))
    {
        Dms_CharacterTransformGet(&g_SysWork.playerWork.player.position, &g_SysWork.playerWork.player.rotation, "HERO", g_Cutscene_Timer, (s_DmsHeader*)FS_BUFFER_11);
        vcChangeProjectionValue(Dms_CameraTargetsGet(&g_Cutscene_CameraPositionTarget, &g_Cutscene_CameraLookAtTarget, NULL, g_Cutscene_Timer, (s_DmsHeader*)FS_BUFFER_11));
        vcUserCamTarget(&g_Cutscene_CameraPositionTarget, NULL, true);
        vcUserWatchTarget(&g_Cutscene_CameraLookAtTarget, NULL, true);
    }
}

void func_800DB13C(void) // 0x800DB13C
{
#if VERSION_EQUAL_OR_NEWER(JAP1) // @bugfix?
    if (Savegame_EventFlagGet(EventFlag_486))
    {
#endif
        Event_DisplayMapMsgWithBg(FILE_TIM_ELCWIRE1_TIM, Q12(0.0f), Q12(0.0f), MAP_MSG(74));

        if (g_SysWork.sysStateSteps[1] >= 3 && g_SysWork.sysStateSteps[1] <= 5)
        {
            if (D_800E2CE8 != Q12(0.0f))
            {
                D_800E2CE8 -= g_DeltaTime;
                if (D_800E2CE8 < Q12(0.0f))
                {
                    D_800E2CE8 = Q12(0.0f);
                }
            }
            else
            {
                SD_Call(Sfx_Unk1653);
                D_800E2CE8 = Rng_GenerateInt(Q12(0.2f), Q12(2.2f) - 1);
            }
        }
#if VERSION_EQUAL_OR_NEWER(JAP1)
    }
    else
    {
        g_Screen_BackgroundImgGamma = Q8(14.0f / 32.0f);
        Event_DisplayMapMsgWithBg(FILE_TIM_ELCWIRE0_TIM, Q12(0.0f), Q12(0.0f), MAP_MSG(87));
    }
#endif
}

void func_800DB1F0(void) // 0x800DB1F0
{
    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            ScreenFade_ResetTimestep();
            Chara_Load(0, Chara_GhostChildAlessa, g_SysWork.npcBoneCoordBuffer, CHARA_FORCE_FREE_ALL, NULL, NULL);
            Chara_ProcessLoads();
            Chara_Spawn(Chara_GhostChildAlessa, 0, Q12(57.2f), Q12(-20.4f), Q12(-0.25f), 3);
            SysWork_StateStepIncrement(0);
            break;

        default:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 9, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);

            Savegame_EventFlagSet(EventFlag_562);
            Savegame_EventFlagSet(EventFlag_223);
            break;
    }
}

void func_800DB31C(void) // 0x800DB31C
{
    if (Savegame_EventFlagGet(EventFlag_496))
    {
        if (Savegame_EventFlagGet(EventFlag_498))
        {
            Event_DisplayMapMsgWithBg(FILE_TIM_3X3HINT2_TIM, 0, 0, MAP_MSG(77));
        }
        else
        {
            Event_DisplayMapMsgWithBg(FILE_TIM_3X3PICT2_TIM, 0, 0, MAP_MSG(77));
        }
    }
    else
    {
        if (Savegame_EventFlagGet(EventFlag_499))
        {
            Event_DisplayMapMsgWithBg(FILE_TIM_3X3HINT1_TIM, 0, 0, MAP_MSG(76));
        }
        else
        {
            Event_DisplayMapMsgWithBg(FILE_TIM_3X3PICT1_TIM, 0, 0, MAP_MSG(76));
        }
    }

    Savegame_EventFlagClear(EventFlag_496);
    Savegame_EventFlagClear(EventFlag_497);
}

void func_800DB3D0(void) // 0x800DB3D0
{
    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();

            if (Savegame_EventFlagGet(EventFlag_496))
            {
                Event_BgTextureCmd(BgTextureCmd_QueueRead, FILE_TIM_3X3HINT2_TIM, false);
                Savegame_EventFlagSet(EventFlag_498);
            }
            else
            {
                Event_BgTextureCmd(BgTextureCmd_QueueRead, FILE_TIM_3X3HINT1_TIM, false);
                Savegame_EventFlagSet(EventFlag_499);
            }

            ScreenFade_Reset();
            g_SysWork.bgmStatusFlags |= BgmStatusFlag_Pause;

            SD_Call(Sfx_Unk1657);
            SysWork_StateStepIncrement(0);
            break;

        case 1:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 1, Q12(2.5f), false);
            g_SysWork.bgmStatusFlags |= BgmStatusFlag_Pause;
            break;

        case 2:
            Event_BgTextureCmd(BgTextureCmd_AwaitLoad, 0, false);
            break;

        case 3:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 1, Q12(0.4f), false);
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            break;

        case 4:
            if (Savegame_EventFlagGet(EventFlag_496))
            {
                Event_DisplayMapMsg(false, MAP_MSG(77), 0, 0, 0, false);
            }
            else
            {
                Event_DisplayMapMsg(false, MAP_MSG(76), 0, 0, 0, false);
            }

            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            break;

        case 5:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);

            Savegame_EventFlagClear(EventFlag_496);
            Savegame_EventFlagClear(EventFlag_497);

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            Event_InvItemCmd(InvItemCmd_AddItem, InvItemId_Camera, 1, false);
            break;
    }
}

#include "maps/shared/sharedFunc_800DB60C_7_s01.h" // 0x800DB60C

void func_800DBDE0(void) // 0x800DBDE0
{
    const static VECTOR3 SFX_POS = { Q12(100.8f), Q12(-1.0f), Q12(-20.0f) };

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();

            D_800E2C64 = 0;

            Event_BgTextureCmd(BgTextureCmd_QueueRead, FILE_TIM_NEWSP3_TIM, false);
            Sfx_WithFlagsPlay(Sfx_Unk1654, &SFX_POS, Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 1:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 2:
            Event_BgTextureCmd(BgTextureCmd_AwaitLoad, 0, false);
            break;

        case 3:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.0f), false);
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            break;

        case 4:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_DisplayMapMsg(false, 30, 0, 0, 0, false);
            break;

        case 5:
            g_Screen_BackgroundImgGamma = Q8(6.0f / 32.0f);

            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_DisplayMapMsg(false, 31, 0, 0, 0, false);
            break;

        case 6:
            SD_Call(Sfx_Unk1654);
            SysWork_StateStepIncrement(0);

        case 7:
            g_Screen_BackgroundImgGamma = Q8(6.0f / 32.0f);

            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_DisplayMapMsg(false, 32, 0, 0, 0, false);
            break;

        case 8:
            g_Screen_BackgroundImgGamma = Q8(6.0f / 32.0f);

            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_DisplayMapMsg(false, 35, 0, 0, 0, false);
            break;

        case 9:
            g_Screen_BackgroundImgGamma = Q8(6.0f / 32.0f);

            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            break;
    }
}

void func_800DC028(void) // 0x800DC028
{
    const static VECTOR3 SFX_POS = { Q12(97.8f), Q12(-1.0f), Q12(-19.2f) };

    if (g_SysWork.sysStateSteps[1] == 0)
    {
        Sfx_WithFlagsPlay(Sfx_Unk1464, &SFX_POS, Q8(0.5f), SfxFlag_None);
    }

    Event_DisplayMapMsgWithDimmedBg(FILE_TIM_PLANTBK_TIM, Q12(0.0f), Q12(0.0f), 37, MAP_MSG(38));
}

void func_800DC080(void) // 0x800DC080
{
    Event_ItemTake(InvItemId_BirdCageKey, DEFAULT_PICKUP_ITEM_COUNT, EventFlag_M7S01_PickupBirdCageKey, MAP_MSG(54));
}

const VECTOR3 sharedData_800CB094_3_s01 = { Q12(-100.9f), Q12(-2.0f), Q12(-60.9f) };

#include "maps/shared/sharedFunc_800D15F0_3_s01.h" // 0x800DC0AC

void func_800DCE20(void) // 0x800DCE20
{
    s32 i;

    // Skip.
    if ((g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.skip) &&
        g_SysWork.sysStateSteps[0] > 0 && g_SysWork.sysStateSteps[0] < 6)
    {
        SysWork_StateStepSet(0, 6);
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            ScreenFade_ResetTimestep();
            CutsceneBorder_ForceShow();
            SysWork_StateStepIncrement(0);
            break;

        case 1:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.6f), false);
            Event_CameraPositionSet(NULL, Q12(-19.47f), Q12(-2.18f), Q12(-23.16f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(NULL, Q12(-22.32f), Q12(-0.95f), Q12(-20.64f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_PathWaypointSet(true, 0, 0, Q12_ANGLE(-90.0f), Q12(-26.0f), Q12(-20.9f));
            SysWork_StateStepIncrement(0);

        case 2:
            D_800E16A8[0] = MIN(D_800E16A8[0] + Q12_MULT_PRECISE(g_DeltaTime, Q12(0.25f)), Q12(0.1f));

            for (i = 0; i < 4; i++)
            {
                g_WorldObject_Dr[i].position.vz += Q12_MULT_PRECISE(g_DeltaTime, (i & 1) ? D_800E16A8[0] : -D_800E16A8[0]);
            }

            Event_WaitTimer(Q12(0.8f), false);
            break;

        case 5:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(1.5f), false);

        case 4:
            Event_PathWaypointExecutePlayer(53, 1, false);

        case 3:
            D_800E16A8[0] = MIN(D_800E16A8[0] + Q12_MULT_PRECISE(g_DeltaTime, Q12(0.25f)), Q12(0.1f));
            D_800E16A8[1] = MIN(D_800E16A8[1] + Q12_MULT_PRECISE(g_DeltaTime, Q12(0.25f)), Q12(0.1f));

            for (i = 0; i < ARRAY_SIZE(g_WorldObject_Dr); i++)
            {
                g_WorldObject_Dr[i].position.vz += Q12_MULT_PRECISE(g_DeltaTime, (i & 1) ? D_800E16A8[i >= 4 ? 1 : 0] : -D_800E16A8[i >= 4 ? 1 : 0]);
            }

            if (g_SysWork.sysStateSteps[0] == 3)
            {
                Event_WaitTimer(Q12(4.0f), false);
            }

            if (g_SysWork.sysStateSteps[0] == 4)
            {
                Event_WaitTimer(Q12(2.0f), false);
            }

            g_DeltaTime = g_DeltaTime >> 1;
            break;

        case 6:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            g_DeltaTime = g_DeltaTime >> 1;
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);

            Savegame_EventFlagSet(EventFlag_479);

            for (i = 0; i < ARRAY_SIZE(g_WorldObject_Dr); i++)
            {
                Math_Vector3Set(&g_WorldObject_Dr[i].position, Q12(-100.9f), Q12(0.0f), Q12(-60.9f));
            }
            break;
    }
}

void func_800DD348(void* unused, s32 idx, u8 spawnFlags) // 0x800DD348
{
    u8* curSpawnFlags;

    curSpawnFlags  = &g_MapOverlayHdr.charaSpawnInfos[0][idx].flags;
    *curSpawnFlags = spawnFlags;
}

void Map_WorldObjectsInit(void) // 0x800DD368
{
    D_800E2CE8 = 0;
    D_800E33A0 = Rng_GenerateInt(0, Q12(4.0f) - 1);
    D_800E33A4 = 0;

    WorldObject_Init(&g_WorldObject_Diary, "DIARY_HI", -180.0f, 0.0f, -140.755f, 0.0f, 180.0f, 0.0f);
    WorldObject_ModelNameSet(&g_WorldObject_SavePad, g_CommonWorldObjectNames[1]);
    WorldObject_PoseInit(&g_WorldObject_UnkPose0, -16.6226f, -1.2396f, -59.0691f, 0.0f, 105.21f, 0.0f);
    WorldObject_ModelNameSet(&g_WorldObject_KaigaR[0], "KAIGAR_H");
    WorldObject_ModelNameSet(&g_WorldObject_KaigaR[1], "KAIGAR2_");

    Math_Vector3Set(&g_WorldObject_UnkPos0, Q12(56.0f), Q12(-1.9f), Q12(-19.25f));

    WorldObject_ModelNameSet(&g_WorldObject_KaigaL[0], "KAIGAL_H");
    WorldObject_ModelNameSet(&g_WorldObject_KaigaL[1], "KAIGAL2_");

    Math_Vector3Set(&g_WorldObject_UnkPos1, Q12(56.0f), Q12(-1.9f), Q12(-21.45f));
    Math_SetSVectorFast(&g_WorldObject_UnkRot0, Q12_ANGLE(0.0f), Q12_ANGLE(180.0f), Q12_ANGLE(0.0f));

    WorldObject_Init(&g_WorldObject_Key0, "KEY_HIDE", -181.05f, 0.0f, -20.0f, 0.0f, 180.0f, 0.0f);
    WorldObject_PlacementInit(&g_WorldObject_Gofu, "GOFU_HID", 17.072f, -1.6f, -19.2f);
    WorldObject_PlacementInit(&g_WorldObject_Key1, "KEY_HIDE", 141.811f, -0.4f, -21.4512f);
    WorldObject_PlacementInit(&g_WorldObject_Penchi, "PENCHI_H", -19.2f, 0.0f, -97.45f);
    WorldObject_PlacementInit(&g_WorldObject_Driver, "DRIVER_H", -19.2f, 0.0f, -97.45f);
    WorldObject_PlacementInit(&g_WorldObject_Zukan, "ZUKAN_HI", 99.8f, 0.0f, -20.0f);
    WorldObject_PlacementInit(&g_WorldObject_Scrap, "SCRAP_HI", 100.0f, 0.0f, -20.0f);

    WorldObject_ModelNameSet(&g_WorldObject_Key2, "KEY_HIDE");
    WorldObject_ModelNameSet(&g_WorldObject_Futa[0], "FUTA1_HI");
    WorldObject_ModelNameSet(&g_WorldObject_Futa[1], "FUTA2_HI");

    WorldObject_PoseInit(&g_WorldObject_UnkPose1, -60.0f, 0.0f, -140.8f, 0.0f, 180.0f, 0.0f);

    WorldObject_ModelNameSet(&g_WorldObject_Key1_2[0], "KEY1_HID");
    WorldObject_ModelNameSet(&g_WorldObject_Key1_2[1], "KEY2_HID");

    WorldObject_PoseInit(&g_WorldObject_UnkPose2, -140.0f, 0.0f, -20.0f, 0.0f, 90.0f, 0.0f);
    WorldObject_Init(&g_WorldObject_Camera, "CAMERA_H", -19.6f, -0.8f, -142.2f, 0.0f, 126.7f, 0.0f);

    if (Savegame_EventFlagGet(EventFlag_487))
    {
        WorldObject_Init(&g_WorldObject_Stone0, "STONE3_H", -139.5f, -0.05f, -140.0f, 0.0f, 90.0f, -90.0f);
    }
    else
    {
        WorldObject_Init(&g_WorldObject_Stone0, "STONE1_H", -139.5f, -1.35f, -140.6f, 0.0f, 90.0f, 0.0f);
    }

    WorldObject_Init(&g_WorldObject_Door, "DOOR_HID", 182.5247f, 0.0f, -106.064f, 0.0f, 0.0f, 0.0f);
    WorldObject_Init(&g_WorldObject_Ring, "RING_NEA", -178.381f, -0.5716f, -98.846f, 0.0f, 0.0f, -32.5f);
    WorldObject_Init(&g_WorldObject_Gofu_B, "GOFU_B_H", -181.9f, -0.57f, -101.2f, 0.0f, 59.0f, 0.0f);

    Math_Vector3Set(&g_WorldObject_UnkPos2, Q12(-16.6692f), Q12(-1.23f), Q12(-61.6f));

    WorldObject_ModelNameSet(&g_WorldObject_KeyX1, "KEYX1_HI");

    Math_SetSVectorFast(&g_WorldObject_UnkRot1, Q12_ANGLE(25.6f), Q12_ANGLE(90.0f), Q12_ANGLE(0.0f));
    Math_SetSVectorFast(&g_WorldObject_UnkRot2, Q12_ANGLE(-113.0f), Q12_ANGLE(90.0f), Q12_ANGLE(0.0f));

    WorldObject_ModelNameSet(&g_WorldObject_KeyX2, "KEYX2_HI");

    Math_SetSVectorFast(&g_WorldObject_UnkRot3, Q12_ANGLE(126.8f), Q12_ANGLE(90.0f), Q12_ANGLE(0.0f));
    Math_SetSVectorFast(&g_WorldObject_UnkRot4, Q12_ANGLE(90.9f), Q12_ANGLE(90.0f), Q12_ANGLE(0.0f));

    WorldObject_Init(&g_WorldObject_Ana, "ANA_HIDE", -16.6661f, -0.7271f, -61.6368f, 0.0f, 90.0f, 0.0f);
    WorldObject_Init(&g_WorldObject_Stone1, "STONE_HI", -16.6692f, -0.71f, -61.533f, 0.0f, 90.0f, 0.0f);
    WorldObject_Init(&g_WorldObject_Window, "WINDOW_H", -16.666f, -1.235f, -61.503f, 0.0f, 90.0f, 0.0f);
    WorldObject_PlacementInit(&g_WorldObject_Kubomi, "KUBOMI_H", -139.5f, 0.0f, -138.1f);

    if (Savegame_EventFlagGet(EventFlag_479))
    {
        WorldObject_PlacementInit(&g_WorldObject_Dr[0], "DR1L_HID", -100.9f, 0.0f, -60.9f);
        WorldObject_PlacementInit(&g_WorldObject_Dr[1], "DR1R_HID", -100.9f, 0.0f, -60.9f);
        WorldObject_PlacementInit(&g_WorldObject_Dr[2], "DR3L_HID", -100.9f, 0.0f, -60.9f);
        WorldObject_PlacementInit(&g_WorldObject_Dr[3], "DR3R_HID", -100.9f, 0.0f, -60.9f);
        WorldObject_PlacementInit(&g_WorldObject_Dr[4], "DR2L_HID", -100.9f, 0.0f, -60.9f);
        WorldObject_PlacementInit(&g_WorldObject_Dr[5], "DR2R_HID", -100.9f, 0.0f, -60.9f);
    }
    else
    {
        WorldObject_PlacementInit(&g_WorldObject_Dr[0], "DR1L_HID", -20.9f, 0.0f, -20.9f);
        WorldObject_PlacementInit(&g_WorldObject_Dr[1], "DR1R_HID", -20.9f, 0.0f, -20.9f);
        WorldObject_PlacementInit(&g_WorldObject_Dr[2], "DR3L_HID", -20.9f, 0.0f, -20.9f);
        WorldObject_PlacementInit(&g_WorldObject_Dr[3], "DR3R_HID", -20.9f, 0.0f, -20.9f);
        WorldObject_PlacementInit(&g_WorldObject_Dr[4], "DR2L_HID", -20.9f, 0.0f, -20.9f);
        WorldObject_PlacementInit(&g_WorldObject_Dr[5], "DR2R_HID", -20.9f, 0.0f, -20.9f);
    }

    if (g_SavegamePtr->gameDifficulty == GameDifficulty_Easy)
    {
        func_800DD348(16, 8, 0);
    }

    if (g_SavegamePtr->gameDifficulty == GameDifficulty_Hard)
    {
        func_800DD348(16, 9, 27);
        func_800DD348(16, 11, 23);
    }

    WorldObject_ModelNameSet(&g_CommonWorldObjects[0], g_CommonWorldObjectNames[2]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[1], g_CommonWorldObjectNames[3]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[2], g_CommonWorldObjectNames[4]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[3], g_CommonWorldObjectNames[5]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[4], g_CommonWorldObjectNames[6]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[5], g_CommonWorldObjectNames[7]);
}

// Rodata at the bottom of the file. Inline once all users are decomped.
extern VECTOR3 D_800CC984;
extern SVECTOR3 D_800CC990;
extern VECTOR3 D_800CC998;

void Map_WorldObjectsUpdate(void) // 0x800DDCD4
{
    s32              npcTimer;
    s32              cellZ0;
    s32              cellX0;
    s32              projCellZ0;
    s32              projCellX0;
    s32              tmp1;
    s32              tmp2;
    s32              tmp3;
    s32              temp_v0_5;
    s32              v1;
    s32              i;
    s_WorldObjectModel* objPtr;
    MAP_CHUNK_CHECK_VARIABLE_DECL();

    cellZ0 = g_SysWork.playerWork.player.position.vz / Q12(CHUNK_SIZE);
    cellX0 = g_SysWork.playerWork.player.position.vx / Q12(CHUNK_SIZE);

    if (g_SysWork.playerWork.player.position.vx > Q12(0.0f))
    {
        projCellX0 = cellX0 + 17;
    }
    else
    {
        projCellX0 = cellX0 + 15;
    }

    if (g_SysWork.playerWork.player.position.vz > Q12(0.0f))
    {
        projCellZ0 = cellZ0 + 17;
    }
    else
    {
        projCellZ0 = cellZ0 + 15;
    }

    switch (CELL_XZ(projCellX0, projCellZ0))
    {
        case CELL_XZ(11, 12):
            if (Savegame_EventFlagGet(EventFlag_569))
            {
                WorldObjects_Add(&g_WorldObject_Diary.object, &g_WorldObject_Diary.position, &g_WorldObject_Diary.rotation);
            }
            break;

        case CELL_XZ(14, 15):
            if (!Savegame_EventFlagGet(EventFlag_485))
            {
                if (g_SysWork.npcs[0].model.charaId != Chara_GhostChildAlessa)
                {
                    Chara_Spawn(Chara_GhostChildAlessa, 0, Q12(-60.0f), Q12(-20.0f), Q12_ANGLE(180.0f), 3);
                    Sd_XaPreLoadAudioPreTaskAdd(Sfx_XaAudio569);
                }
                else if (g_SysWork.playerWork.player.position.vx > Q12(-62.0f))
                {
                    Savegame_EventFlagSet(EventFlag_485);
                    Chara_MovementReset(&g_SysWork.npcs[0]);
                    Event_PathWaypointSet(true, 1, 0, Q12_ANGLE(90.0f), Q12(-60.0f), Q12(-35.0f));
                    SD_Call(Sfx_XaAudio569);
                }
            }
            else if (g_SysWork.npcs[0].model.charaId == Chara_GhostChildAlessa)
            {
                g_SysWork.npcs[0].properties.npc.moveDistance_124 = Q12(1.25f);
                Event_PathWaypointExecuteCharaNoWait(&g_SysWork.npcs[0], 2, 1);
                npcTimer = 0;

                if (Sd_AudioStreamingCheck() == AudioStreamingState_XaPlaying)
                {
                    if (g_SysWork.npcs[0].position.vz < Q12(-24.0f))
                    {
                        npcTimer = (Q12(-24.0f) - g_SysWork.npcs[0].position.vz) / 5;
                        v1 = CLAMP_LOW(npcTimer, D_800E33A4);
                        D_800E33A4 = v1;
                    }
                    else
                    {
                        D_800E33A4 = 1;
                    }
                }
                else if (D_800E33A4 > 0)
                {
                    D_800E33A4 += Q12_MULT_PRECISE(g_DeltaTime, Q12(0.6f));
                    npcTimer = D_800E33A4;
                }

                tmp2 = npcTimer;
                if (tmp2 > Q12(1.0f))
                {
                    tmp2 = Q12(1.0f);
                }

                tmp1 = npcTimer;
                if (tmp1 > Q12(1.0f))
                {
                    tmp1 = Q12(1.0f);
                }

                if (tmp1 < Q12(0.0f))
                {
                    tmp2 = Q12(0.0f);
                }
                npcTimer = tmp2;

                g_SysWork.npcs[0].timer_C6 = npcTimer;
                if (g_SysWork.npcs[0].position.vz < Q12(-30.0f) || g_SysWork.npcs[0].timer_C6 == Q12(1.0f))
                {
                    Chara_ModelCharaIdClear(g_SysWork.npcs, 0, 0);
                }
            }
            break;

        case CELL_XZ(11, 15):
            if (!Savegame_EventFlagGet(EventFlag_M7S01_PickupKeyOfPhaleg))
            {
                WorldObjects_Add(&g_WorldObject_Key0.object, &g_WorldObject_Key0.position, &g_WorldObject_Key0.rotation);

                if (D_800E33A0)
                {
                    tmp3 = D_800E33A0 - g_DeltaTime;
                    D_800E33A0 = tmp3;
                    if (tmp3 < 0)
                    {
                        D_800E33A0 = 0;
                    }
                }
                else
                {
                    Sfx_WithFlagsPlay(Sfx_Unk1646, &D_800CC984, Q8(0.5f), SfxFlag_None);
                    D_800E33A0 = Rng_GenerateInt(Q12(2.0f), Q12(4.5f) - 1);
                }
            }
            break;

        case CELL_XZ(18, 15):
            if (g_SysWork.npcs[0].model.charaId == Chara_GhostChildAlessa)
            {
                g_SysWork.npcs[0].timer_C6 += Q12_MULT_PRECISE(g_DeltaTime, Q12(0.2f));
                if (g_SysWork.npcs[0].timer_C6 > Q12(1.0f))
                {
                    Chara_ModelCharaIdClear(&g_SysWork.npcs[0], 0, 0);
                }
            }

            objPtr = &g_WorldObject_KaigaR[0];
            if (Savegame_EventFlagGet(EventFlag_498))
            {
                objPtr = &g_WorldObject_KaigaR[1];
            }
            WorldObjects_Add(objPtr, &g_WorldObject_UnkPos0, &g_WorldObject_UnkRot0);

            objPtr = &g_WorldObject_KaigaL[0];
            if (Savegame_EventFlagGet(EventFlag_499))
            {
                objPtr = &g_WorldObject_KaigaL[1];
            }
            WorldObjects_Add(objPtr, &g_WorldObject_UnkPos1, &g_WorldObject_UnkRot0);
            break;

        case CELL_XZ(12, 15):
            if (!Savegame_EventFlagGet(EventFlag_490))
            {
                WorldObjects_Add(&g_WorldObject_Key1_2[0], &g_WorldObject_UnkPose2.position, &g_WorldObject_UnkPose2.rotation);
            }
            else if (!Savegame_EventFlagGet(EventFlag_525))
            {
                WorldObjects_Add(&g_WorldObject_Key1_2[1], &g_WorldObject_UnkPose2.position, &g_WorldObject_UnkPose2.rotation);
            }
            break;

        case CELL_XZ(17, 15):
            if (!Savegame_EventFlagGet(EventFlag_M7S01_PickupAmuletOfSolomon))
            {
                WorldObjects_Add(&g_WorldObject_Gofu.object, &g_WorldObject_Gofu.position, &D_800CC990);
            }
            break;

        case CELL_XZ(20, 15):
            if (!Savegame_EventFlagGet(EventFlag_M7S01_PickupBirdCageKey))
            {
                WorldObjects_Add(&g_WorldObject_Key1.object, &g_WorldObject_Key1.position, &D_800CC990);
            }
            break;

        case CELL_XZ(15, 13):
            if (!Savegame_EventFlagGet(EventFlag_M7S01_PickupPliers))
            {
                WorldObjects_Add(&g_WorldObject_Penchi.object, &g_WorldObject_Penchi.position, &D_800CC990);
            }
            if (!Savegame_EventFlagGet(EventFlag_M7S01_PickupScrewdriver))
            {
                WorldObjects_Add(&g_WorldObject_Driver.object, &g_WorldObject_Driver.position, &D_800CC990);
                break;
            }
            break;

        case CELL_XZ(19, 15):
            WorldObjects_Add(&g_WorldObject_Zukan.object, &g_WorldObject_Zukan.position, &D_800CC990);

            if (Savegame_EventFlagGet(EventFlag_M5S03_SeenSecondNewspaper))
            {
                WorldObjects_Add(&g_WorldObject_Scrap.object, &g_WorldObject_Scrap.position, &D_800CC990);
            }
            break;

        case CELL_XZ(14, 12):
            if (Savegame_EventFlagGet(EventFlag_486))
            {
                if (!Savegame_EventFlagGet(EventFlag_M7S02_PickupKeyOfAratron))
                {
                    WorldObjects_Add(&g_WorldObject_Key2, &g_WorldObject_UnkPose1.position, &g_WorldObject_UnkPose1.rotation);
                }

                WorldObjects_Add(&g_WorldObject_Futa[1], &g_WorldObject_UnkPose1.position, &g_WorldObject_UnkPose1.rotation);
            }
            else
            {
                WorldObjects_Add(&g_WorldObject_Futa[0], &g_WorldObject_UnkPose1.position, &g_WorldObject_UnkPose1.rotation);
            }

            if (Savegame_EventFlagGet(EventFlag_486) && !Savegame_EventFlagGet(EventFlag_549))
            {
                if (D_800E2CE8 != Q12(0.0f))
                {
                    D_800E2CE8 -= g_DeltaTime;
                    if (D_800E2CE8 < Q12(0.0f))
                    {
                        D_800E2CE8 = Q12(0.0f);
                    }
                    break;
                }

                Sfx_WithFlagsPlay(Sfx_Unk1653, &D_800CC998, Q8(0.125f), SfxFlag_None);

                D_800E2CE8 = Rng_GenerateInt(819, 9010);
            }
            break;

        case CELL_XZ(15, 12):
            if (!Savegame_EventFlagGet(EventFlag_M7S01_PickupCamera))
            {
                WorldObjects_Add(&g_WorldObject_Camera.object, &g_WorldObject_Camera.position, &g_WorldObject_Camera.rotation);
                break;
            }
            break;

        case CELL_XZ(12, 12):
            if (!Savegame_EventFlagGet(EventFlag_M7S01_PickupStoneOfTime))
            {
                WorldObjects_Add(&g_WorldObject_Stone0.object, &g_WorldObject_Stone0.position, &g_WorldObject_Stone0.rotation);
            }

            WorldObjects_Add(&g_WorldObject_Kubomi.object, &g_WorldObject_Kubomi.position, &D_800CC990);
            break;

        case CELL_XZ(21, 13):
            WorldObjects_Add(&g_WorldObject_Door.object, &g_WorldObject_Door.position, &g_WorldObject_Door.rotation);
            break;

        case CELL_XZ(15, 15):
        case CELL_XZ(13, 14):
            for (i = 0; i < ARRAY_SIZE(g_WorldObject_Dr); i++)
            {
                WorldObjects_Add(&g_WorldObject_Dr[i].object, &g_WorldObject_Dr[i].position, &D_800CC990);
            }
            break;

        case CELL_XZ(11, 13):
            if (!Savegame_EventFlagGet(EventFlag_M7S01_PickupRingOfContract))
            {
                WorldObjects_Add(&g_WorldObject_Ring.object, &g_WorldObject_Ring.position, &g_WorldObject_Ring.rotation);
            }
            if (!Savegame_EventFlagGet(EventFlag_M7S01_PickupCrestOfMercury))
            {
                WorldObjects_Add(&g_WorldObject_Gofu_B.object, &g_WorldObject_Gofu_B.position, &g_WorldObject_Gofu_B.rotation);

            }
            break;

        case CELL_XZ(15, 14):
            if (Savegame_EventFlagGet(EventFlag_563))
            {
                WorldObjects_Add(&g_WorldObject_Stone1.object, &g_WorldObject_Stone1.position, &g_WorldObject_Stone1.rotation);
            }
            else
            {
                WorldObjects_Add(&g_WorldObject_Ana.object, &g_WorldObject_Ana.position, &g_WorldObject_Ana.rotation);
            }

            if (!Savegame_EventFlagGet(EventFlag_489))
            {
                WorldObjects_Add(&g_WorldObject_KeyX1, &g_WorldObject_UnkPos2, &g_WorldObject_UnkRot1);
                WorldObjects_Add(&g_WorldObject_KeyX2, &g_WorldObject_UnkPos2, &g_WorldObject_UnkRot3);
            }
            else
            {
                WorldObjects_Add(&g_WorldObject_Window.object, &g_WorldObject_Window.position, &g_WorldObject_Window.rotation);

                if (!Savegame_EventFlagGet(EventFlag_M7S01_PickupKeyOfHagith))
                {
                    WorldObjects_Add(&g_WorldObject_KeyX1, &g_WorldObject_UnkPos2, &g_WorldObject_UnkRot2);
                }

                WorldObjects_Add(&g_WorldObject_KeyX2, &g_WorldObject_UnkPos2, &g_WorldObject_UnkRot4);
            }

            WorldObjects_Add(&g_WorldObject_SavePad, &g_WorldObject_UnkPose0.position, &g_WorldObject_UnkPose0.rotation);
            break;
    }

    Collision_TriggerFlagBitsClear(CollisionTriggerFlag_All);
    Collision_TriggerFlagBitsSet(CollisionTriggerFlag_None);

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -5, -1, -5) && PLAYER_IN_MAP_CHUNK(vz, 1, -3, -1, -3))
    {
        if (!Savegame_EventFlagGet(EventFlag_M7S01_RifleShells))
        {
            WorldObjects_Add(&g_CommonWorldObjects[4], &g_CommonWorldObjectPoses[0].position, &g_CommonWorldObjectPoses[0].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -1, 0, 0) && PLAYER_IN_MAP_CHUNK(vz, 1, -4, -1, -4))
    {
        if (!Savegame_EventFlagGet(EventFlag_M7S01_HandgunBullets0))
        {
            WorldObjects_Add(&g_CommonWorldObjects[3], &g_CommonWorldObjectPoses[1].position, &g_CommonWorldObjectPoses[1].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, 3, -1, 3) && PLAYER_IN_MAP_CHUNK(vz, 1, -1, 0, 0))
    {
        if (!Savegame_EventFlagGet(EventFlag_M7S01_HealthDrink0))
        {
            WorldObjects_Add(&g_CommonWorldObjects[1], &g_CommonWorldObjectPoses[2].position, &g_CommonWorldObjectPoses[2].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -5, -1, -5) && PLAYER_IN_MAP_CHUNK(vz, 1, -1, 0, 0))
    {
        if (!Savegame_EventFlagGet(EventFlag_M7S01_HandgunBullets1))
        {
            WorldObjects_Add(&g_CommonWorldObjects[3], &g_CommonWorldObjectPoses[3].position, &g_CommonWorldObjectPoses[3].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -1, 0, 0) && PLAYER_IN_MAP_CHUNK(vz, 1, -2, -1, -2))
    {
        if (!Savegame_EventFlagGet(EventFlag_M7S01_ShotgunShells))
        {
            WorldObjects_Add(&g_CommonWorldObjects[5], &g_CommonWorldObjectPoses[4].position, &g_CommonWorldObjectPoses[4].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -1, 0, 0) && PLAYER_IN_MAP_CHUNK(vz, 1, -2, -1, -2))
    {
        if (!Savegame_EventFlagGet(EventFlag_M7S01_FirstAidKit))
        {
            WorldObjects_Add(&g_CommonWorldObjects[0], &g_CommonWorldObjectPoses[5].position, &g_CommonWorldObjectPoses[5].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, 2, -1, 2) && PLAYER_IN_MAP_CHUNK(vz, 1, -2, -1, -2))
    {
        if (!Savegame_EventFlagGet(EventFlag_M7S01_HandgunBullets2))
        {
            WorldObjects_Add(&g_CommonWorldObjects[3], &g_CommonWorldObjectPoses[6].position, &g_CommonWorldObjectPoses[6].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, 2, -1, 2) && PLAYER_IN_MAP_CHUNK(vz, 1, -2, -1, -2))
    {
        if (!Savegame_EventFlagGet(EventFlag_M7S01_HealthDrink1))
        {
            WorldObjects_Add(&g_CommonWorldObjects[1], &g_CommonWorldObjectPoses[7].position, &g_CommonWorldObjectPoses[7].rotation);
        }
    }
}

void func_800DEDA4(void) // 0x800DEDA4
{
    if (!Savegame_EventFlagGet(EventFlag_485))
    {
        if (D_800A9945 != Chara_GhostChildAlessa || !Chara_ModelLoadedCheck(Chara_GhostChildAlessa))
        {
            Anim_CharaTypeAnimInfoClear();
            Chara_Load(0, Chara_GhostChildAlessa, g_SysWork.npcBoneCoordBuffer, CHARA_FORCE_FREE_ALL, NULL, NULL);
            Chara_ProcessLoads();
        }
    }
    else if (D_800A9945 == Chara_PuppetNurse)
    {
        if (!Chara_ModelLoadedCheck(Chara_PuppetNurse))
        {
            g_MapOverlayHdr.charaGroupIds[0] = Chara_PuppetNurse;

            Anim_CharaTypeAnimInfoClear();
            Chara_Load(0, Chara_PuppetNurse, g_SysWork.npcBoneCoordBuffer, CHARA_FORCE_FREE_ALL, NULL, NULL);
            Chara_ProcessLoads();
        }
    }
    else
    {
        g_MapOverlayHdr.charaGroupIds[0] = Chara_PuppetNurse;

        Anim_CharaTypeAnimInfoClear();
        Chara_Load(0, Chara_PuppetNurse, g_SysWork.npcBoneCoordBuffer, CHARA_FORCE_FREE_ALL, NULL, NULL);
        Chara_ProcessLoads();
    }

    if (Savegame_EventFlagGet(EventFlag_M7S01_PickupStoneOfTime))
    {
        if (g_SavegamePtr->gameDifficulty != GameDifficulty_Easy)
        {
            Chara_SpawnFlagsSet(16, 2, CharaSpawnFlag_0 | CharaSpawnFlag_1 | CharaSpawnFlag_3 | CharaSpawnFlag_4);
        }

        Chara_SpawnFlagsSet(16, 3, CharaSpawnFlag_0 | CharaSpawnFlag_1 | CharaSpawnFlag_3 | CharaSpawnFlag_4);
    }
    else if (Savegame_EventFlagGet(EventFlag_485))
    {
        Chara_SpawnFlagsSet(16, 0, CharaSpawnFlag_0 | CharaSpawnFlag_1 | CharaSpawnFlag_4);
        Chara_SpawnFlagsSet(16, 1, CharaSpawnFlag_0 | CharaSpawnFlag_1 | CharaSpawnFlag_4);
    }

    if (Savegame_EventFlagGet(EventFlag_525) && g_SavegamePtr->gameDifficulty != GameDifficulty_Easy)
    {
        Chara_SpawnFlagsSet(16, 12, CharaSpawnFlag_0 | CharaSpawnFlag_1 | CharaSpawnFlag_4);
        Chara_SpawnFlagsSet(16, 13, CharaSpawnFlag_0 | CharaSpawnFlag_1 | CharaSpawnFlag_2 | CharaSpawnFlag_4);
        Chara_SpawnFlagsSet(16, 14, CharaSpawnFlag_0 | CharaSpawnFlag_1 | CharaSpawnFlag_3 | CharaSpawnFlag_4);
    }
}

const VECTOR3 D_800CC984 = { Q12(-180.8f), Q12(-1.0f), Q12(-20.0f) };

const SVECTOR3 D_800CC990 = { 0, 0, 0 };

const VECTOR3 D_800CC998 = { Q12(-63.3f), Q12(-1.6f), Q12(-139.2f) };
