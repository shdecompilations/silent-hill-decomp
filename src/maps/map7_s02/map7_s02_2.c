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
#include "maps/map7/map7_s02.h"
#include "maps/common/characters/bloody_lisa.h"
#include "maps/common/characters/dahlia.h"
#include "maps/common/characters/ghost_child_alessa.h"
#include "maps/common/characters/ghost_doctor.h"
#include "maps/common/characters/kaufmann.h"
#include "maps/common/characters/lisa.h"
#include "inline_no_dmpsx.h"
#include "maps/common/particle.h"
#include <psyq/gtemac.h>
#include "maps/common/characters/player.h"
#include "maps/common/characters/stalker.h"

#include "../src/maps/common/particles/particle.c"

#include "../src/maps/common/characters/player.c"

#include "../src/maps/common/characters/stalker.c" // 0x800D06F8
#include "../src/maps/common/characters/lisa.c"
#include "../src/maps/common/characters/bloody_lisa.c"
#include "../src/maps/common/characters/bloodsucker.c" // 0x800D6874
#include "../src/maps/common/characters/dahlia.c"
#include "../src/maps/common/characters/ghost_child_alessa.c"
#include "../src/maps/common/characters/kaufmann.c"
#include "../src/maps/common/characters/ghost_doctor.c"

#include "../src/maps/common/chara_util.c" // 0x800D8C74

#include "../src/maps/common/map_util.c" // 0x800D9754

static inline D_800E9ED8_Set(void)
{
    if (D_800E9ED8 == Q12(0.0f) && !Savegame_EventFlagGet(EventFlag_555))
    {
        Sd_SfxPlay(Sfx_Unk1664, Q8(0.0f), Q8(7.0f / 8.0f));
        D_800E9ED8 = Rng_GenerateInt(Q12(1.2f), Q12(2.8f) - 1);
    }
    else
    {
        D_800E9ED8 = ((D_800E9ED8 - g_DeltaTime) >= Q12(0.0f)) ? (D_800E9ED8 - (q3_12)g_DeltaTime) : Q12(0.0f);
    }
}

void func_800D97FC(s32 arg0) // 0x800D97FC
{
    (arg0 != 0) ? func_800D9864() : sharedFunc_800D0110_7_s00();
}

void func_800D9830(s32 arg0) // 0x800D9830
{
    (arg0 != 0) ? func_800D98E4() : sharedFunc_800D0110_7_s00();
}

void func_800D9864(void) // 0x800D9864
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

void func_800D98E4(void) // 0x800D98E4
{
    Bgm_SongChange(Savegame_EventFlagGet(EventFlag_M5S03_KaufmannHasMotorcycleVial) ? 40 : 35);
}

#include "maps/common/shared/sharedFunc_800D0110_7_s00.h" // 0x800D9924

void GameBoot_LoadScreen_StageString(void) {}

#include "maps/common/shared/Event_CutsceneTimerAdvance.h" // 0x800DA1B0

const char* MAP_MESSAGES[] = {
#include "maps/common/shared/map_msg_common.h"
#if VERSION_REGION_IS(NTSC)
    /* 15 */ "\t\tSame_thing... ~E ",
    /* 16 */ "~L4 ~C3 Investigation_stalled. ~N\n\t[PTV]_dealers_still_at_large. ~N\n\t~N\n\tSuspicious_deaths_continue. ~N\n\tLike_anti-drug_Mayor, ~N\n\tnarcotics_officer_dies_of ~N\n\tsudden_heart_failure_of ~N\n\tunknown_origin. ~E ",
    /* 17 */ "\t\tAre_there_others? ~N\n\t\tIt's_pretty_old... ",
    /* 18 */ "~L4 ~C3 Fire_broke_out_in_town. ~N\n\t6_homes_destroyed. ~N\n\t~N\n\tCharred_body ~N\n\tof_Alessa_Gillespie(7) ~N\n\tfound_in_aftermath. ~N\n\t~N\n\tCause_of_fire_currently_under ~N\n\tinvestigation. ",
    /* 19 */ "~L4 ~C3 Investigations_show_source ~N\n\tas_basement_of_Gillespie_home. ~N\n\tBlaze_now_believed_caused_by ~N\n\tmalfunction_of_antiquated ~N\n\tboiler. ~E ",
    /* 20 */ "\tThe_date_of_the_blaze... ",
    /* 21 */ "\tit's_the_same_day ~N\n\twe_found_Cheryl! ~E ",
    /* 22 */ "\tThere's_a_place_mark ~N\n\tin_the_book.. ~E ",
    /* 23 */ "~L4 ~C3 WHITE_CLAUDIA. ~N\n\t~N\n\tPerennial_herb_found_near_water. ~N\n\tReaches_height_of_10_to_15_in. ~N\n\tOblong_leaves,_white_blossoms. ",
    /* 24 */ "~L4 ~C3 Seeds_contain_hallucinogen. ~N\n\tAncient_records_show_it_was ~N\n\tused_for_religious_ceremonies. ~N\n\tThe_hallucinogenic_effect ~N\n\twas_key. ~E ",
    /* 25 */ "~L4 ~C3 Ask_doctor_to_let_me_quit_being ~N\n\tin_charge_of_that_patient. ~N\n\t~N\n\tIt's_too_weird. ~E ",
    /* 26 */ "~L4 ~C3 The_room_is_filled_with_insects. ~N\n\t~N\n\tEven_with_doors ~N\n\tand_windows_shut ~N\n\tthey_get_in_to_spite_me. ~N\n\t~N\n\tTo_the_hospital... ~E ",
    /* 27 */ "~L4 ~C3 Still_alive, ~N\n\tbut_with_wounds_that_won't_heal. ~E ",
    /* 28 */ "~L4 ~C3 Blood_and_pus_flow_from ~N\n\tthe_bathroom_faucet. ~N\n\t~N\n\tI_try_to_stop_it, ~N\n\tbut_it_won't_turn_off. ~E ",
    /* 29 */ "~L4 ~C3 Feeling_bad. ~N\n\tNeed_to_throw_up. ~N\n\tBut_nothing_comes_out. ~N\n\tVomiting_only_bile. ~E ",
    /* 30 */ "~L4 ~C3\tNeed_drug. ~E ",
    /* 31 */ "~L4 ~C3 Told_the_doctor_I_quit. ~N\n\tWon't_work_at_that ~N\n\thospital_anymore. ~E ",
    /* 32 */ "~J0(0.7)\t_ ",
    /* 33 */ "~J2(1.5)\tWhat_is_it? ",
    /* 34 */ "~J2(2.7)\tStill_has_an_unusually ~N\n\t\t\thigh_fever... ",
    /* 35 */ "~J2(2.6)\tEyes_don't_open... ~N\n\t\t\tgetting_a_pulse... ",
    /* 36 */ "~J2(2.8)\tBut_just_barely_breathing. ",
    /* 37 */ "~J2(2.3)\tHer_skin_is_all_charred! ",
    /* 38 */ "~J2(1.2)\tEven_when_I_change ~N\n\t\t\tthe_bandages, ",
    /* 39 */ "~J2(3.8)\tthe_blood_and_pus_just_start ~N\n\t\t\toozing_through! ",
    /* 40 */ "~J2(5.9)\tWhy... ~N\n\t\t\tWhat_is_keeping_that_child_alive? ",
    /* 41 */ "~J2(2.8)\tI..._can't_stand_it ~N\n\t\t\tany_longer... ",
    /* 42 */ "~J2(3.3)\tI_won't_tell_a_soul ~N\n\t\t\t...promise. ",
    /* 43 */ "~J2(2.0)\tSo_please... ",
    /* 44 */ "~J2(1.0)\t_ ~E ",
    /* 45 */ "~J0(1.3)\t",
    /* 46 */ "~J0(3.3)\t~E ",
    /* 47 */ "~J1(6.4)\tEverything_is_going ~N\n\t\t\taccording_to_plan. ~N\n\t\t\tSheltered_in_the_womb. ",
    /* 48 */ "~J1(1.8)\tBut_it's_not_done_yet. ",
    /* 49 */ "~J1(4.5)\tHalf_the_soul_is_lost. ~N\n\t\t\tThat_is_why_the_seed ~N\n\t\t\tlies_dormant. ",
    /* 50 */ "~J1(3.6)\tAnd_what_soul_remains ~N\n\t\t\tcaptured_in_that_husk, ",
    /* 51 */ "~J1(3.7)\tis_buried_deep_down ~N\n\t\t\tin_the_subconscious. ",
    /* 52 */ "~J1(2.6)\tAre_you_trying_to_say ~N\n\t\t\tit_won't_work!? ",
    /* 53 */ "~J1(2.0)\tThat_wasn't_our_agreement! ",
    /* 54 */ "~J1(4.0)\tNo,_no. ~N\n\t\t\tThese_are_just_stalling_tactics. ",
    /* 55 */ "~J1(4.4)\tIf_we_lend_a_hand, ~N\n\t\t\twe_will_be_able_to_get_power. ",
    /* 56 */ "~J1(5.4)\tNever_fear. ~N\n\t\t\tThe_promise_shall_not_be_broken. ",
    /* 57 */ "~J1(5.0)\tBut_the_power_we_could ~N\n\t\t\tdraw_now_would_be ~N\n\t\t\tvery_weak;_almost_nothing. ",
    /* 58 */ "~J1(3.2)\tUnless_we_get_the_other ~N\n\t\t\thalf_of_the_soul... ",
    /* 59 */ "~J1(3.1)\tWe'll_use_a_magical_spell. ",
    /* 60 */ "~J1(6.0)\tFeeling_this_child's_pain, ~N\n\t\t\tit's_sure_to_come. ",
    /* 61 */ "~J1(2.8)\tBut_that_will_take_time... ~E ",
    /* 62 */ "~J0(2.9)\t",
    /* 63 */ "~J0(3.0)\tNo! ~N\n\t\t\tI_don't_want_to! ",
    /* 64 */ "~J0(2.7)\tDo_what_mommy_tells_you,_now. ",
    /* 65 */ "~J0(6.3)\tI_just_want_you_to_lend_me ~N\n\t\t\ta_teeny_bit_of_your_power. ~N\n\t\t\tThat's_all. ",
    /* 66 */ "~J2(1.2)\tNo! ~E ",
    /* 67 */ "~J0(2.0)\tI_don't_want_to_do_it! ~E ",
    /* 68 */ "~J1(5.8)\tIt_will_make_everyone_happy... ~N\n\t\t\tand_it's_for_your_own_good,_too. ",
    /* 69 */ "~J1(4.3)\tOh,_but_mommy. ~N\n\t\t\tI_just_want_to_be_with_you. ",
    /* 70 */ "~J1(4.3)\tJust_two_of_us. ~N\n\t\t\tPlease_understand. ~E ",
    /* 71 */ "~J1(7.1)\tOh,_yes,_I_see... ~N\n\t\t\tMaybe_mommy_has_been_wrong. ",
    /* 72 */ "~J1(1.3)\tMommy... ",
    /* 73 */ "~J1(5.3)\tWhy_didn't_I_see_this_before? ~N\n\t\t\tThere's_no_reason_to_wait... ",
    /* 74 */ "~J1(7.2)\tHerein_lies_the_mother's_womb, ~N\n\t\t\tcontaining_the_power ~N\n\t\t\tto_create_life. ",
    /* 75 */ "~J1(4.2)\tI_could_have_done_it_all_myself. ",
    /* 76 */ "~J1(1.6)\tMommy? ~E ",
    /* 77 */ "\tUsed_the_ ~C2 Key_of_[Bethor] ~C7 . ~E ",
    /* 78 */ "\tUsed_the_ ~C2 Key_of_[Aratron] ~C7 . ~E ",
    /* 79 */ "\tThere_is_a_ ~C2 Key_of_[Bethor] ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 80 */ "\tThere_is_a_ ~C2 Key_of_[Aratron] ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 81 */ "\tThere_is_an_ ~C2 Ankh ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 82 */ "\tThere_is_a_ ~C2 Dagger_of_Melchior ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 83 */ "\tThere_is_a_ ~C2 Disk_of_Ouroboros ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 84 */ "\tThe_generator_is_working. ~N\n\tDo_you_want_to_press_the_switch? ~S4 ",
    /* 85 */ "\tThe_generator_is_stopped. ~N\n\tDo_you_want_to_press_the_switch? ~S4 ",
    /* 86 */ "\tIt's_locked. ",
    /* 87 */ "\tThe_word,_[Bethor] ~N\n\tis_carved_on_the_door. ~E ",
    /* 88 */ "\tIt's_locked. ",
    /* 89 */ "\tThe_word,_[Aratron] ~N\n\tis_carved_on_the_door. ~E ",
    /* 90 */ "\tJelly_beans. ~N\n\tDo_you_want_to_open_the_bag? ~S4 ",
    /* 91 */ "\tThe_chain_is_missing_a_link. ~E ",
    /* 92 */ "\tThe_chain_is_fixed ~N\n\twith_the_ring. ~E ",
    /* 93 */ "\tThere_is_a_ ~C2 Ring_of_contract ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 94 */ "\tThere_is_a_ ~C2 Screwdriver ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 95 */ "\tThere_is_a_ ~C2 Camera ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 96 */ "\tThere_is_an_ ~C2 Amulet_of_Solomon ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 97 */ "\tThere_is_a_ ~C2 Crest_of_Mercury ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 98 */ "~C3\tGo_home. ",
    /* 99 */ "~C3\tThief. ",
    /* 100 */ "~C3\tDrop_dead. ~E ",
    /* 101 */ "\t_ ~E ",
    /* 102 */ "~C3 Names_engraved ~N\n\tOn_a_lithograph. ~N\n\tThe_Grim_Reaper's_list. ",
    /* 103 */ "~C3 Yes,_the_headcount_is_set ~N\n\tYoung_and_old_lined_up ~N\n\tIn_order_of_age. ",
    /* 104 */ "~C3 Then,_the_pathway_opens ~N\n\tAwaiting_them,_the_frenzied ~N\n\tUproar,_the_feast_of_death! ~E ",
    /* 105 */ "\tThere_is_a_key. ",
    /* 106 */ "\tElectricity_is_flowing_to_the_key. ~N\n\tI'll_get_shocked_if_I_grab_it_now. ~E ",
    /* 107 */ "~C3\tThe_light_to_the_future. ~E ",
    /* 108 */ "~C3\tThe_light_illuminating ~N\n\t\t_the_darkness. ~E ",
    /* 109 */ "\tBird_cage._It's_empty. ~E ",
    /* 110 */ "\tA_steel_plate ~N\n\tis_screwed_to_the_wall. ~E ",
    /* 111 */ "~J0(1.2)\tHarry? ~E ",
    /* 112 */ "~J1(2.6)\tLisa... ~N\n\t\t\tWhat's_the_matter_with_you? ",
    /* 113 */ "~J1(1.8)\tI_get_it_now... ",
    /* 114 */ "~J1(4.0)\tWhy_I'm_still_alive_even ~N\n\t\t\tthough_everyone_else's_dead. ",
    /* 115 */ "~J1(4.0)\tI'm_not_the_only_one ~N\n\t\t\twho's_still_walking_around. ",
    /* 116 */ "~J1(1.7)\tI'm_the_same_as_them. ",
    /* 117 */ "~J1(2.8)\tI_just_hadn't_noticed_it_before. ~E ",
    /* 118 */ "~J1(1.3)\tLisa? ",
    /* 119 */ "~J1(3.6)\tStay_by_me,_Harry. ~N\n\t\t\tPlease. ",
    /* 120 */ "~J1(3.3)\tI'm_so_scared. ~N\n\t\t\tHelp_me... ~E ",
    /* 121 */ "~J0(2.4)\tSave_me_from_them. ",
    /* 122 */ "~J0(4.0)\tPlease...__Harry... ~E ",
    /* 123 */ "~J0(8.9)\t~E ",
    /* 124 */ "\t[Alessa] ~E ",
    /* 125 */ "\tUsed_the_ ~C2 Amulet_of_Solomon ~C7 . ~E ",
    /* 126 */ "\tUsed_the_ ~C2 Crest_of_Mercury ~C7 . ~E ",
    /* 127 */ "\tUsed_the_ ~C2 Ankh ~C7 . ~E ",
    /* 128 */ "\tUsed_the_ ~C2 Dagger_of_Melchior ~C7 . ~E ",
    /* 129 */ "\tUsed_the_ ~C2 Disk_of_Ouroboros ~C7 . ~E ",
    /* 130 */ "~C3 thou_possess_them ~N\n\tto_guard_thy_spirit ~N\n\tevoke_five_rites ~N\n\tunveil_thy_fate ~E ",
    /* 131 */ "\tThere's_a_severed_power_line. ~N\n\tNothing_special_here_either. ~E ",
    /* 132 */ "\tThe_elevator_doesn't ~N\n\tappear_to_be_working. ~E ",
    /* 133 */ "\tThere's_a_clock ~N\n\twith_a_broken face. ~E ",
    /* 134 */ "\tUsed_the_ ~C2 Ring_of_contract ~C7 . ~E ",
    /* 135 */ "\tNone_of_this_stuff_is_of_any_use. ~E ",
    /* 136 */ "\tJust_a_wall. ~E ",
    /* 137 */ "\tThere_are_specimen_of ~N\n\tbutterflies_and_moths ~N\n\thanging_on_the_wall. ~E ",
    /* 138 */ "\tA_girl's_clothing_hangs ~N\n\ton_the_wall. ",
    /* 139 */ "\tSix,_maybe_seven... ~N\n\tMust_be_a_kid_about_that_age. ~E ",
    /* 140 */ "\tCards_are_scattered ~N\n\ton_the_floor. ~E ",
    /* 141 */ "\tThere_are_dolls_on_the_floor. ~E ",
    /* 142 */ "\tA_child's_drawings_are_scattered ~N\n\ton_the_floor. ~E ",
    /* 143 */ "\tNothing_unusual. ~E ",
    /* 144 */ "\tThere_are_old_fairy_tales, ~N\n\tpicture_books_of_animals, ~N\n\tand_plants_on_the_shelf. ~E ",
    /* 145 */ "\tNone_of_this_stuff_is_of_any_use. ~E ",
    /* 146 */ "~L4 ~C3 Help_me... ~E ",
    /* 147 */ "\tThere_is_a_diary_on_the_floor. ~E ",
    /* 148 */ "\tNothing_helpful. ~E ",
    /* 149 */ "\tLooks_like_an_altar. ~N\n\tWonder_what_they_worship? ~E ",
    /* 150 */ "\tThe_floors_and_walls_are_covered ~N\n\twith_graffiti. ~E ",
    /* 151 */ "\tVCR._It's_old_but_usable. ~E ",
    /* 152 */ "\tA_body_is_laid_out_on_the_bed. ~N\n\tDon't_feel_like_checking_this_out. ~E ",
    /* 153 */ "\tThere's_a_bottle_filled ~N\n\twith_some_kind_of_drug. ~N\n\tBetter_leave_it. ~E ",
    /* 154 */ "\tThere_are_some_empty_bottles. ~E ",
    /* 155 */ "\tNothing_special_in_the_box. ~N\n\tNothing_to_worry_about. ~E ",
    /* 156 */ "\tNothing_inside. ~E ",
    /* 157 */ "\tNothing_useful_in_the_boxes. ~E ",
    /* 158 */ "\tThere's_a_botanical ~N\n\treference_book. ~N\n\tDon't_need_to_read_it_now. ~E "
#elif VERSION_REGION_IS(NTSCJ)
    /* 15 */ "\tまた　これか…… ~E ",
    /* 16 */ "~L4 ~C3\n\t捜査難航。 ~N\n\t麻薬“ＰＴＶ”の売人特定できず。~N\n\t~N\n\t相次ぐ怪死事件。 ~N\n\t麻薬売買の取締り強化を掲げた市長に続き、 ~N\n\t麻薬捜査官が原因不明の心臓発作で死亡。 ~E ",
    /* 17 */ "\t\t他にもあるのか？ ~N\n\t\tかなり古いな…… ",
    /* 18 */ "~L4 ~C3\n\t繁華街で不審火。 ~N\n\t６棟が全焼。 ~N\n\t~N\n\t逃げ遅れたアレッサ・ギレスピー（７歳）の ~N\n\t焼死体が発見された。 ~N\n\t~N\n\t出火の原因を調査中。 ",
    /* 19 */ "~L4 ~C3\n\t調査の結果、出火場所は ~N\n\tギレスピー宅の地下室と限定された。 ~N\n\t出火原因は老朽化したボイラーの ~N\n\t故障ではないかと推測される。 ~E ",
    /* 20 */ "\tこの火事の日付は…… ",
    /* 21 */ "\tシェリルを見つけたあの日だ…… ~E ",
    /* 22 */ "\t\tしおりの挟んである本がある ~E ",
    /* 23 */ "~L4 ~C3\n\t［ホワイトクロジェア］ ~N\n\t~N\n\t河岸／湖岸に生える多年草。 ~N\n\t高さは３０〜４０ｃｍ。 ~N\n\t長楕円状の葉を持ち、白い花を咲かせる。 ~N\n\t種に幻覚作用のある成分が含まれている。 ~N\n\t古来、その幻覚作用を利用して、宗教儀式 ~N\n\tに使われていたという記録も残っている。 ~E ",
    /* 24 */ "~L4 ~C3\n\t先生にあの患者の担当から ~N\n\t外してもらえるよう頼んだ。 ~N\n\t~N\n\tどう考えてもおかしい。 ~E ",
    /* 25 */ "~L4 ~C3\n\t部屋に数え切れないほどの虫がいる。 ~N\n\t~N\n\t窓や扉を閉めても入ってくる。 ~N\n\t~N\n\t震えるあたしをあざ笑っている。 ~N\n\t~N\n\t病院へ…… ~E ",
    /* 26 */ "~L4 ~C3\n\tまだ生きている。 ~N\n\tなのに傷は治らない。 ~E ",
    /* 27 */ "~L4 ~C3\n\tバスルームの蛇口から ~N\n\t血と膿が流れ出してくる。 ~N\n\t止めようとしても止まらない。 ~E ",
    /* 28 */ "~L4 ~C3\n\t気持ちが悪い。 ~N\n\t吐き気がする。 ~N\n\tだけど、もう何も出てこない。 ~N\n\t胃液すら吐きつくした。 ~E ",
    /* 29 */ "~L4 ~C3\t薬が、欲しい。 ~E ",
    /* 30 */ "~L4 ~C3\t先生に辞める意志を伝えた。 ~N\n\t\t\tもう、あの病院で働くつもりはない。 ~E ",
    /* 31 */ "~J0(0.7)\t　 ",
    /* 32 */ "~J2(1.5)\t……どうして？ ",
    /* 33 */ "~J2(2.7)\t異常な高熱状態がずっと続いてるわ…… ",
    /* 34 */ "~J2(5.4)\t目も覚まさない……点滴を受け…… ~N\n\t\t\tかすかに息をしているだけ ",
    /* 35 */ "~J2(2.3)\t皮膚のほとんどが焼けただれてるのよ！ ",
    /* 36 */ "~J2(5.0)\t包帯を何度替えても ~N\n\t\t\tすぐに血と膿でドロドロになるのよ！ ",
    /* 37 */ "~J2(5.9)\tどうして…… ~N\n\t\t\tどうしてあの子は生きてるの？ ",
    /* 38 */ "~J2(2.8)\tあたし……もう耐えられない…… ",
    /* 39 */ "~J2(3.3)\t……誰にも話さないわ ~N\n\t\t\t……約束する ",
    /* 40 */ "~J2(2.0)\tだから……もう許して ",
    /* 41 */ "~J2(1.0)\t　 ~E ",
    /* 42 */ "~J0(1.3)\t",
    /* 43 */ "~J0(3.3)\t~E ",
    /* 44 */ "~J1(6.4)\t計画は成功していたわ ~N\n\t\t\t胎内に宿っていた ",
    /* 45 */ "~J1(1.8)\tしかし完全ではない ",
    /* 46 */ "~J1(4.5)\t魂の半分が失せている ~N\n\t\t\tそのせいで種子は目覚めない ",
    /* 47 */ "~J1(3.6)\t残った魂も　種子を抱えたまま ",
    /* 48 */ "~J1(3.7)\t意識の底に閉じこもってしまった ",
    /* 49 */ "~J1(2.6)\tそれでは役に立たないというのか？ ",
    /* 50 */ "~J1(2.0)\t話が違うぞ！ ",
    /* 51 */ "~J1(4.0)\tいいえ　必死に押さえつけているだけよ ",
    /* 52 */ "~J1(4.4)\tわたしたちが手を貸せば ~N\n\t\t\t力を引き出すことはできるわ ",
    /* 53 */ "~J1(5.4)\t大丈夫　約束は守るわ ",
    /* 54 */ "~J1(5.0)\tしかし　こんな状態では引き出せる力は ~N\n\t\t\tごくわずかだ……無力に等しい ",
    /* 55 */ "~J1(3.2)\t消えた魂のかけらを探さねば…… ",
    /* 56 */ "~J1(3.1)\t呪法を使うわ ",
    /* 57 */ "~J1(6.0)\tこの子の苦しみを感じて ~N\n\t\t\tきっと現れるはずよ ",
    /* 58 */ "~J1(2.8)\t時間がかかるな…… ~E ",
    /* 59 */ "~J0(2.9)\t",
    /* 60 */ "~J0(3.0)\tいやよ！ ~N\n\t\t\tもう　いやなの！ ",
    /* 61 */ "~J0(2.7)\tどうしてママの言うことを ~N\n\t\t\t聞いてくれないの ",
    /* 62 */ "~J0(6.3)\tほんの少し　少しだけあなたの力を ~N\n\t\t\t貸してくれればそれでいいのよ ",
    /* 63 */ "~J2(1.2)\tいや！ ~E ",
    /* 64 */ "~J0(2.0)\tこんなことしたくない！ ~E ",
    /* 65 */ "~J1(5.8)\tみんなが幸せになるためなのよ…… ~N\n\t\t\tあなたのためにもなるのよ ",
    /* 66 */ "~J1(4.3)\tこんなことするより　あたしはママと ~N\n\t\t\t一緒にいられる方が幸せなの…… ",
    /* 67 */ "~J1(4.3)\tそれ以上いらない ~N\n\t\t\tだからわかってよ ~E ",
    /* 68 */ "~J1(7.1)\tそう……　そうね ~N\n\t\t\tママがまちがっていたのかもしれないわね ",
    /* 69 */ "~J1(1.3)\tママ…… ",
    /* 70 */ "~J1(5.3)\tどうして気がつかなかったのかしら？ ~N\n\t\t\t待っている必要なんかないんだわ…… ",
    /* 71 */ "~J1(7.2)\t復活のための力を秘めた ~N\n\t\t\t母胎はここにあるわ ",
    /* 72 */ "~J1(4.2)\tすべてをあたしの手で行えば良かったのよ ",
    /* 73 */ "~J1(1.6)\tママ……？ ~E ",
    /* 74 */ "\t~C2\t［ベトール］の鍵 ~C7 を使った ~E ",
    /* 75 */ "\t~C2\t［アラトロン］の鍵 ~C7 を使った ~E ",
    /* 76 */ "\t~C2\t［ベトール］の鍵 ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 77 */ "\t~C2\t［アラトロン］の鍵 ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 78 */ "\t~C2 アンク ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 79 */ "\t~C2 メルキオールの小剣 ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 80 */ "\t~C2\tウロボロスの円環 ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 81 */ "\t非常用発電機は動いている ~N\n\tスイッチを押しますか？ ~S4 ",
    /* 82 */ "\t非常用発電機は止まっている ~N\n\tスイッチを押しますか？ ~S4 ",
    /* 83 */ "\t鍵がかかっている ",
    /* 84 */ "\t扉には“ベトール”と刻まれている ~E ",
    /* 85 */ "\t鍵がかかっている ",
    /* 86 */ "\t扉には“アラトロン”と刻まれている ~E ",
    /* 87 */ "\tゼリービーンズがある ~N\n\t開けますか？ ~S4 ",
    /* 88 */ "\t鎖は途中で切れている ~E ",
    /* 89 */ "\t指輪で補われて鎖はつながっている ~E ",
    /* 90 */ "\t~C2 契約の指輪 ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 91 */ "\t~C2\tドライバー ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 92 */ "\t~C2\tカメラ ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 93 */ "\t~C2 ソロモンの護符 ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 94 */ "\t~C2\tメルクリウスの紋章 ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 95 */ "~C3\t帰れ ",
    /* 96 */ "~C3\t泥棒 ",
    /* 97 */ "~C3\t死ね ~E ",
    /* 98 */ "\t　 ~E ",
    /* 99 */ "~C3\t石板に刻まれた名前 ~N\n\tそれは冥府に帰りし者の目録 ",
    /* 100 */ "~C3\tさあ、その頭を並べろ ~N\n\t若さを尊び老いをあざけり列を作れ ",
    /* 101 */ "~C3\tその時、道が開かれる ~N\n\t行く手に待つは狂騒、死の宴！ ~E ",
    /* 102 */ "\t鍵がある ",
    /* 103 */ "\t鍵には電気が流れているようだ ~N\n\tこのまま取ると感電してしまうだろう ~E ",
    /* 104 */ "~C3\t光導く未来 ~E ",
    /* 105 */ "~C3\t闇を開く光 ~E ",
    /* 106 */ "\t鳥カゴがある ~N\n\t中は空っぽで何もいない ~E ",
    /* 107 */ "\t鉄板が壁にネジ止めされている ~E ",
    /* 108 */ "~J0(1.2)\tハリー ~E ",
    /* 109 */ "~J1(2.6)\tリサ…… ~N\n\t\t\tいったいどうしたんだ？ ",
    /* 110 */ "~J1(1.8)\tわかったのよ ",
    /* 111 */ "~J1(4.0)\tどうしてみんな死んでしまったのに ~N\n\t\t\tあたしは生きてるのか ",
    /* 112 */ "~J1(4.0)\tあたしだけ生きてるんじゃなかったのよ ",
    /* 113 */ "~J1(1.7)\tみんなと同じ ",
    /* 114 */ "~J1(2.8)\tあたしは気がつかなかっただけ ~E ",
    /* 115 */ "~J1(1.3)\tリサ…… ",
    /* 116 */ "~J1(3.6)\tハリー ~N\n\t\t\tお願い……そばにいて…… ",
    /* 117 */ "~J1(3.3)\tあたし怖いわ ~N\n\t\t\t助けて…… ~E ",
    /* 118 */ "~J0(2.4)\tあいつらから守って…… ",
    /* 119 */ "~J0(4.0)\tね？　ハリー…… ~E ",
    /* 120 */ "~J0(8.9)\t~E ",
    /* 121 */ "~C3\tアレッサ ~E ",
    /* 122 */ "\t~C2 ソロモンの護符 ~C7 を使った ~E ",
    /* 123 */ "\t~C2 メルクリウスの紋章 ~C7 を使った ~E ",
    /* 124 */ "\t~C2 アンク ~C7 を使った ~E ",
    /* 125 */ "\t~C2 メルキオールの小剣 ~C7 を使った ~E ",
    /* 126 */ "\t~C2 ウロボロスの円環 ~C7 を使った ~E ",
    /* 127 */ "~M ~C3\n\t汝が手中にあり ~N\n\tその身を護りし ~N\n\t五つのまじない ~N\n\tここに捧げよ ~E ",
    /* 128 */ "\t切れた電線がある ~N\n\t特に気にすることもないだろう ~E ",
    /* 129 */ "\tエレベーターは動いていないようだ ~E ",
    /* 130 */ "\t文字盤の壊れた時計がある ~E ",
    /* 131 */ "\t~C2 契約の指輪 ~C7 を使った ~E ",
    /* 132 */ "\tいろいろなものが置かれているが ~N\n\t特に役に立ちそうなものはない ~E ",
    /* 133 */ "\tただの壁だ ~E ",
    /* 134 */ "\t壁に蝶や蛾の標本が飾ってある ~E ",
    /* 135 */ "\t女の子の洋服が壁にかけてある ",
    /* 136 */ "\t６歳か７歳…… ~N\n\tたぶんそれぐらいの年齢の子だろう ~E ",
    /* 137 */ "\t床にカードが散らばっている ~E ",
    /* 138 */ "\t床に人形が置かれている ~E ",
    /* 139 */ "\t床に子どもの描いた絵が散らばっている ~N\n\tこれは何の絵なのだろう？ ~E ",
    /* 140 */ "\tいろいろなものが置かれている ~N\n\tだが役に立ちそうなものはない ~E ",
    /* 141 */ "\t棚の中には古びてボロボロになった ~N\n\t童話や動植物の図鑑などが置かれている ~E ",
    /* 142 */ "\t棚の中には何もない ~E ",
    /* 143 */ "~L4 ~C3\tだれか、たすけて…… ~E ",
    /* 144 */ "\t床に日記が置かれている ~E ",
    /* 145 */ "\t役に立ちそうなものはない ~E ",
    /* 146 */ "\t祭壇らしきものがある ~N\n\tこれは何を祭っているのだろうか？ ~E ",
    /* 147 */ "\t壁や床は落書きで埋め尽くされている ~E ",
    /* 148 */ "\tビデオデッキがある ~N\n\t古びてはいるが　まだ使えそうだ ~E ",
    /* 149 */ "\t死体がベッドに寝かされている ~N\n\tあれこれと調べる気にはなれない ~E ",
    /* 150 */ "\t中身のわからない薬のビンが置かれている ~N\n\t無視していった方がいいだろう ~E ",
    /* 151 */ "\t空のビンが置かれている ~E ",
    /* 152 */ "\t箱の中にはろくなものが入っていない ~N\n\t特に気にする必要はないだろう ~E ",
    /* 153 */ "\t棚の中には何も入っていない ~E ",
    /* 154 */ "\tダンボール箱の中に ~N\n\t役に立ちそうなものは入っていない ~E ",
    /* 155 */ "\t植物事典が置かれている ~N\n\t今は読む必要はないだろう ~E "
#endif
};

void func_800DA248(void) // 0x800DA248
{
    s_FsImageDesc charaTex;

    if (Pad_ButtonClickedBind(skip) &&
        g_SysWork.sysStateSteps[0] >= 4 && g_SysWork.sysStateSteps[0] <= 20)
    {
        SysWork_StateStepSet(0, 22);
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            D_800EB6B0 = 0;

            Fs_QueueStartRead(FILE_ANIM_HBM7_SLS_ANM, FS_BUFFER_4);
            Fs_QueueStartRead(FILE_ANIM_NURSE1_DMS, FS_BUFFER_11);
            Fs_QueueWaitForEmpty();

            Dms_HeaderFixOffsets((s_DmsHeader*)FS_BUFFER_11);
            g_Cutscene_Timer = Q12(0.0f);

            Anim_CharaTypeAnimInfoClear();
            Chara_Load(0, Chara_Lisa, g_SysWork.npcBoneCoordBuffer, CHARA_FORCE_FREE_ALL, NULL, NULL);

            Screen_FadeTimestepReset();
            CutsceneBorder_ForceShow();
            g_SysWork.lightIntensity = Q12(0.8f);
            g_SysWork.sysFlags      |= SysFlag_CutsceneActive;

            Game_TurnFlashlightOn();
            D_800E9ECC = 0;

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
            Event_DisplayMapMsgWithAudio(MAP_MSG(111), &D_800EB6B0, &D_800E9CE4);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(0.0f), Q12(20.0f), true, false);
            Event_WaitTimer(Q12(0.8f), false);
            break;

        case 5:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 72, false);
            SysWork_StateStepIncrement(0);

        case 6:
            Event_DisplayMapMsgWithAudio(MAP_MSG(111), &D_800EB6B0, &D_800E9CE4);
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
            Event_DisplayMapMsgWithAudio(MAP_MSG(112), &D_800EB6B0, &D_800E9CE4);
            break;

        case 10:
            Event_DisplayMapMsgWithAudio(MAP_MSG(118), &D_800EB6B0, &D_800E9CE4);
            break;

        case 11:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 17, false);
            SysWork_StateStepIncrement(0);

        case 12:
            Event_DisplayMapMsgWithAudio(MAP_MSG(121), &D_800EB6B0, &D_800E9CE4);
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
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, g_SysWork.npcs, 18, false);
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

void func_800DAA4C(void) // 0x800DAA4C
{
    VECTOR3  pos;
    SVECTOR3 rot;

    // Skip.
    if (Pad_ButtonClickedBind(skip) &&
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
            D_800EB6B0 = 0;
            D_800E9ECD = 0;

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
            g_SysWork.lightRotation.vz = Q12_ANGLE(0.0f);
            break;

        case 6:
            D_800E9ECD = 1;

            func_8008D448();
            Game_FlashlightAttribsSet();

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
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
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
            Game_FlashlightAttribsSet();

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

        if (D_800E9ECD != 0)
        {
            Dms_CharacterTransformGet(&g_WorldObject_Door.position, &g_WorldObject_Door.rotation, "ADOOR", g_Cutscene_Timer, (s_DmsHeader*)FS_BUFFER_11);
        }

        vcChangeProjectionValue(Dms_CameraTargetsGet(&g_Cutscene_CameraPositionTarget, &g_Cutscene_CameraLookAtTarget, NULL, g_Cutscene_Timer, (s_DmsHeader*)FS_BUFFER_11));
        vcUserCamTarget(&g_Cutscene_CameraPositionTarget, NULL, true);
        vcUserWatchTarget(&g_Cutscene_CameraLookAtTarget, NULL, true);
    }
}

#include "maps/common/shared/MapEvent_DoorJammed.h" // 0x800DB21C

#include "maps/common/shared/MapEvent_DoorLocked.h" // 0x800DB2B0

#include "maps/common/shared/MapEvent_DoorUnlocked.h" // 0x800DB344

void MapEvent_CommonItemTake(void) // 0x800DB3D8
{
    u32 pickupType;
    s32 eventFlagIdx;

    pickupType   = CommonPickupItemId_FirstAidKit;
    eventFlagIdx = 0;

    switch (g_MapEventData->mapPointIdx)
    {
        case 212:
            pickupType   = CommonPickupItemId_RifleShells;
            eventFlagIdx = EventFlag_M7S01_RifleShells;
            break;

        case 213:
            pickupType   = CommonPickupItemId_HandgunBullets;
            eventFlagIdx = EventFlag_M7S01_HandgunBullets0;
            break;

        case 214:
            pickupType   = CommonPickupItemId_HealthDrink;
            eventFlagIdx = EventFlag_M7S01_HealthDrink0;
            break;

        case 215:
            pickupType   = CommonPickupItemId_HandgunBullets;
            eventFlagIdx = EventFlag_M7S01_HandgunBullets1;
            break;

        case 216:
            pickupType   = CommonPickupItemId_ShotgunShells;
            eventFlagIdx = EventFlag_M7S01_ShotgunShells;
            break;

        case 217:
            pickupType   = CommonPickupItemId_FirstAidKit;
            eventFlagIdx = EventFlag_M7S01_FirstAidKit;
            break;

        case 218:
            pickupType   = CommonPickupItemId_HandgunBullets;
            eventFlagIdx = EventFlag_M7S01_HandgunBullets2;
            break;

        case 219:
            pickupType   = CommonPickupItemId_HealthDrink;
            eventFlagIdx = EventFlag_M7S01_HealthDrink1;
            break;
    }

    Event_CommonItemTake(pickupType, eventFlagIdx);
}

void func_800DB498(void) // 0x800DB498
{
    VECTOR3 sfxPos = { MAP_POINTS[g_MapEventData->mapPointIdx].positionX, Q12(-1.2f), MAP_POINTS[g_MapEventData->mapPointIdx].positionZ };

    Event_DisplayMapMsgWithSfx(MAP_MSG(86), Sfx_DoorLocked, &sfxPos);
}

void func_800DB52C(void) // 0x800DB52C
{
    VECTOR3 sfxPos = { MAP_POINTS[g_MapEventData->mapPointIdx].positionX, Q12(-1.2f), MAP_POINTS[g_MapEventData->mapPointIdx].positionZ };

    Event_DisplayMapMsgWithSfx(MAP_MSG(88), Sfx_DoorLocked, &sfxPos);
}

void MapEvent_KeyOfBethorUse(void) // 0x800DB5C0
{
    VECTOR3 sfxPos = { MAP_POINTS[g_MapEventData->mapPointIdx].positionX, Q12(-1.2f), MAP_POINTS[g_MapEventData->mapPointIdx].positionZ };

    Player_ItemRemove(InvItemId_KeyOfBethor, 1);
    Event_DisplayMapMsgWithSfx(MAP_MSG(77), Sfx_UseKey, &sfxPos);
    Savegame_EventFlagSet(EventFlag_533);
}

void MapEvent_KeyOfAratronUse(void) // 0x800DB67C
{
    VECTOR3 sfxPos = { MAP_POINTS[g_MapEventData->mapPointIdx].positionX, Q12(-1.2f), MAP_POINTS[g_MapEventData->mapPointIdx].positionZ };

    Player_ItemRemove(InvItemId_KeyOfAratron, 1);
    Event_DisplayMapMsgWithSfx(MAP_MSG(78), Sfx_UseKey, &sfxPos);
    Savegame_EventFlagSet(EventFlag_534);
}

void func_800DB738(void) // 0x800DB738
{
    const static VECTOR3 SFX_POS = { Q12(97.4f), Q12(-0.5f), Q12(-59.0f) };

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_BgTextureCmd(BgTextureCmd_QueueRead, FILE_TIM_LISAVTR_TIM, false);

            D_800EB6B0 = 0;

            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 146, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 2, Q12(0.0f), false);

            g_SysWork.playerWork.player.position.vx = Q12(97.61f);
            g_SysWork.playerWork.player.position.vz = Q12(-59.59f);
            g_SysWork.playerWork.player.rotation.vy = Q12(0.8915f);

            Event_CameraPositionSet(NULL, Q12(100.56f), Q12(-1.06f), Q12(-61.53f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(NULL, Q12(97.61f), Q12(-0.73f), Q12(-59.2f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            SysWork_StateStepIncrement(0);

        case 3:
            Event_WaitTimer(Q12(2.5f), false);
            break;

        case 4:
            Sfx_WithFlagsPlay(Sfx_Unk1526, &SFX_POS, Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 5:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 6:
            Event_BgTextureCmd(BgTextureCmd_AwaitLoad, 0, false);
            break;

        case 7:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.0f), false);
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            break;

        case 8:
            Event_DisplayMapMsgWithAudio(MAP_MSG(32), &D_800EB6B0, &D_800E9CFC);
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);

            if (g_SysWork.sysStateSteps[0] == 8 &&
                Pad_ButtonClicked(Pad_BindingGet(skip) | Pad_BindingGet(cancel)))
            {
                SD_Call(19);
                SysWork_StateStepIncrement(0);
            }
            break;

        case 9:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            break;

        case 10:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 147, false);
            SysWork_StateStepIncrement(0);
            break;

        case 11:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimLock, &g_SysWork.playerWork.player, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            SysWork_StateStepIncrement(0);

        case 12:
            Event_WaitTimer(Q12(0.5f), false);
            break;

        case 13:
            Sfx_WithFlagsPlay(Sfx_Unk1526, &SFX_POS, Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 14:
            Event_CharaAnimUnlockPlayToEnd(&g_SysWork.playerWork.player);
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            vcReturnPreAutoCamWork(false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            Event_InvItemCmd(InvItemCmd_AddItem, InvItemId_VideoTape, 1, false);
            break;
    }
}

void func_800DBB34(void) // 0x800DBB34
{
    if (Pad_ButtonClickedBind(skip) &&
        g_SysWork.sysStateSteps[0] >= 1 && g_SysWork.sysStateSteps[0] <= 13)
    {
        SysWork_StateStepSet(0, 14);
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Anim_CharaTypeAnimInfoClear();
            Chara_Load(0, Chara_GhostChildAlessa, &g_SysWork.npcBoneCoordBuffer[0], CHARA_FORCE_FREE_ALL, NULL, NULL);

            CutsceneBorder_ForceShow();

            Screen_FadeTimestepReset();
            Model_AnimFlagsClear(&g_SysWork.playerWork.player.model, AnimFlag_Visible);
            func_8008D438();
            Event_CameraPositionSet(NULL,
                                    Q12(20.09f), Q12(-0.9f),
                                    Q12(-97.78f), Q12(0.0f),
                                    Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(NULL, Q12(18.5f), Q12(0.2f), Q12(-101.28f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            SysWork_StateStepIncrement(0);

        case 1:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.0f), false);
            break;

        case 2:
            Event_TweenReset(3);
            Event_TweenReset(4);
            Event_TweenReset(5);
            SysWork_StateStepIncrement(0);

        case 3:
            Event_WaitTimer(Q12(0.8f), false);
            break;

        case 4:
            Event_CameraLookAtSet(NULL,
                                  Event_TweenLinear(Q12(-0.7901f), Q12(3.0f), 3) + Q12(18.5f),
                                  Event_TweenLinear(Q12(-2.1299f), Q12(3.0f), 4) + Q12(0.2f),
                                  Event_TweenLinear(Q12(0.45f), Q12(3.0f), 5) + Q12(-101.28f), Q12(0.0f), Q12(0.0f),
                                  Q12(0.0f), Q12(0.0f),
                                  false);
            Event_WaitTimer(Q12(3.0f), false);
            break;

        case 5:
            Event_TweenReset(3);
            Event_TweenReset(4);
            Event_TweenReset(5);
            Chara_ProcessLoads();
            Chara_Spawn(Chara_GhostChildAlessa, 0, Q12(21.2f), Q12(-100.5f), Q12_ANGLE(180.0f), 3);
            SysWork_StateStepIncrement(0);
            break;

        case 6:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 11, false);
            SD_Call(Sfx_XaAudio603);
            SysWork_StateStepIncrement(0);

        case 7:
            Event_WaitTimer(Q12(0.5f), false);
            break;

        case 8:
            Event_CameraLookAtSet(NULL,
                                  Event_TweenLinear(Q12(3.74f), Q12(3.4f), 3) + Q12(17.71f),
                                  Event_TweenLinear(Q12(0.3902f), Q12(3.4f), 4) + Q12(-1.93f),
                                  Event_TweenLinear(Q12(-0.6602f), Q12(3.4f), 5) + Q12(-100.83f),
                                  Q12(0.0f), Q12(0.0f),
                                  Q12(0.0f), Q12(0.0f),
                                  false);
            Event_WaitTimer(Q12(3.4f), false);
            break;

        case 9:
            Event_TweenReset(3);
            Event_TweenReset(4);
            Event_TweenReset(5);
            SysWork_StateStepIncrement(0);
            break;

        case 10:
            Event_WaitTimer(Q12(0.5f), false);
            break;

        case 11:
            Event_CameraLookAtSet(NULL,
                                  Event_TweenLinear(Q12(-0.1302f), Q12(0.6f), 3) + Q12(21.45f),
                                  Event_TweenLinear(Q12(1.21f), Q12(0.6f), 4) + Q12(-1.54f),
                                  Event_TweenLinear(Q12(-0.05f), Q12(0.6f), 5) + Q12(-101.49f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_WaitTimer(Q12(0.6f), false);
            break;

        case 12:
            g_SysWork.npcs[0].timer_C6 += Q12_MULT_PRECISE(Q12(0.25f), g_DeltaTime);
            if (g_SysWork.npcs[0].timer_C6 > Q12(1.0f))
            {
                Chara_ModelCharaIdClear(&g_SysWork.npcs[0], 0, 0);
                SysWork_StateStepIncrement(0);
            }
            break;

        case 13:
            Event_WaitTimer(Q12(1.0f), false);

            if (g_SysWork.sysStateSteps[0] != 13)
            {
                SysWork_StateStepReset();
            }
            break;

        case 14:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 15:
            Chara_ModelCharaIdClear(&g_SysWork.npcs[0], 0, 0);
            SD_Call(19);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            SysWork_StateStepIncrement(0);

        default:
            // Restore player control.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);

            vcReturnPreAutoCamWork(true);
            Savegame_EventFlagSet(EventFlag_548);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            Model_AnimFlagsSet(&g_SysWork.playerWork.player.model, AnimFlag_Visible);
            func_8008D448();
            Anim_CharaTypeAnimInfoClear();
            break;
    }
}

void MapEvent_AnkhTake(void) // 0x800DC120
{
    Event_ItemTake(InvItemId_Ankh, DEFAULT_PICKUP_ITEM_COUNT, EventFlag_M7S02_PickupAnkh, MAP_MSG(81));
}

void func_800DC14C(void) // 0x800DC14C
{
    s32 i;
    s32 floorHitCount;

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_BgTextureCmd(BgTextureCmd_QueueRead, FILE_TIM_JELYBEAN_TIM, false);
            SysWork_StateStepIncrement(0);

        case 1:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 2:
            Event_BgTextureCmd(BgTextureCmd_AwaitLoad, 0, false);
            break;

        case 3:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.0f), false);
            break;

        case 4:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_DisplayMapMsg(true, MAP_MSG(90), 7, 5, 0, false);
            break;

        case 5:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 6:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;

        case 7:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 8:
            Savegame_EventFlagSet(EventFlag_556);
            CutsceneBorder_ForceShow();
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 143, false);

            // Warp player.
            g_SysWork.playerWork.player.position.vx = Q12(61.9627f);
            g_SysWork.playerWork.player.position.vz = Q12(-60.9957f);
            g_SysWork.playerWork.player.rotation.vy = Q12_ANGLE(90.0f);
            SysWork_StateStepIncrement(0);
            break;

        case 9:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimLock, &g_SysWork.playerWork.player, 0, false);

            // Warp camera.
            Event_CameraPositionSet(NULL, Q12(59.74f), Q12(-1.22f), Q12(-60.36f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(NULL, Q12(63.67f), Q12(-1.02f), Q12(-61.05f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);

            SysWork_StateStepIncrement(0);

        case 10:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.0f), false);
            break;

        case 11:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimUnlock, &g_SysWork.playerWork.player, 0, false);
            SysWork_StateStepIncrement(0);

        case 12:
            Event_WaitTimer(Q12(4.5f), false);
            break;

        case 13:
            D_800E9ED4 = Q12(-1.294f);
            D_800E9ED0 = 0;
            Sfx_WithFlagsPlay(Sfx_Unk1662, &Q12_VECTOR3(62.3f, -1.45f, -61.07f), Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 14:
            D_800E9ED0 += g_GravitySpeed;
            if (D_800E9ED0 > Q12(5.0f))
            {
                D_800E9ED0 = Q12(5.0f);
            }

            floorHitCount = 0;
            for (i = 0; i < 16; i++)
            {
                g_WorldObject_Beans[i].position.vy += Q12_MULT_PRECISE(g_DeltaTime, D_800E9ED0 + FP_FROM(D_800EBA14[i] * Math_Sin(D_800EB9F4[i]), Q12_SHIFT));
                g_WorldObject_Beans[i].position.vz += Q12_MULT_PRECISE(g_DeltaTime, FP_FROM(D_800EBA14[i] * Math_Cos(D_800EB9F4[i]), Q12_SHIFT));

                if (g_WorldObject_Beans[i].position.vy > 0)
                {
                    g_WorldObject_Beans[i].position.vy = 0;
                    floorHitCount++;
                }

                WorldObjects_Add(&g_WorldObject_Beans[i].object, &g_WorldObject_Beans[i].position, &g_WorldObject_Beans[i].rotation);
            }

            if (D_800E9ED4 < 0)
            {
                D_800E9ED4 += Q12_MULT_PRECISE(g_DeltaTime, D_800E9ED0);
                if (D_800E9ED4 > 0)
                {
                    Sfx_WithFlagsPlay(Sfx_Unk1651, &Q12_VECTOR3(62.3f, 0.0f, -61.07f), Q8(0.5f), SfxFlag_None);
                }
            }

            if (floorHitCount == 16)
            {
                SysWork_StateStepIncrement(0);
            }
            break;

        case 15:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            vcReturnPreAutoCamWork(true);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            break;
    }
}

void func_800DC778(void) // 0x800DC778
{
    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_InvItemCmd(InvItemCmd_QueueLoad, InvItemId_KeyOfBethor, 0, false);
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            Event_CharaAnimPlayToEnd(&g_SysWork.playerWork.player, 59);
            break;

        case 3:
            Event_InvItemCmd(InvItemCmd_AwaitLoad, InvItemId_KeyOfBethor, 0, false);
            break;

        case 4:
            Savegame_EventFlagSet(EventFlag_M7S02_PickupKeyOfBethor);

            if (Gfx_PickupItemAnimate(InvItemId_KeyOfBethor))
            {
                Event_DisplayMapMsg(true, MAP_MSG(79), 5, 6, 0, false);
            }
            break;

        case 5:
            Event_InvItemCmd(InvItemCmd_AddItem, InvItemId_KeyOfBethor, 1, false);
            SysWork_StateStepSet(0, 7);
            break;

        case 6:
            Savegame_EventFlagClear(EventFlag_M7S02_PickupKeyOfBethor);
            SysWork_StateStepIncrement(0);

        case 7:
            Event_CharaAnimPlayToEnd(&g_SysWork.playerWork.player, 60);
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;
    }
}

void func_800DC94C(void) {}

void func_800DC954(void) // 0x800DC954
{
    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_BgTextureCmd(BgTextureCmd_QueueRead, FILE_TIM_CHAINRF1_TIM, false);
            D_800E9ED6 = 0;
            SysWork_StateStepIncrement(0);

        case 1:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 2:
            Event_BgTextureCmd(BgTextureCmd_AwaitLoad, 0, false);
            break;

        case 3:
            Event_BgTextureCmd(BgTextureCmd_StoreVram, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            SysWork_StateStepIncrement(0);

        case 4:
            Event_BgTextureCmd(BgTextureCmd_8, FILE_TIM_CHAINRF2_TIM, false);
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
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
            Sd_SfxPlay(Sfx_Unk1669, Q8(0.0f), Q8(0.5f));
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
            Event_DisplayMapMsg(false, MAP_MSG(134), 0, 0, 0, false);
            Event_BgTextureCmd(BgTextureCmd_DrawSecondary, 0, false);
            break;

        case 10:
            Event_BgTextureCmd(BgTextureCmd_DrawSecondary, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        default:
            Event_BgTextureCmd(BgTextureCmd_RestoreVram, 0, false);
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            Savegame_EventFlagSet(EventFlag_552);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            break;
    }

    if (g_SysWork.sysStateSteps[0] >= 4 && g_SysWork.sysStateSteps[0] < 11)
    {
        if (D_800E9ED6 == 0 && !Savegame_EventFlagGet(EventFlag_555))
        {
            Sd_SfxPlay(Sfx_Unk1664, Q8(0.0f), Q8(7.0f / 8.0f));
            D_800E9ED6 = Rng_GenerateInt(Q12(1.2f), Q12(2.8f) - 1);
        }
        else
        {
            D_800E9ED6 = MAX(0, D_800E9ED6 - g_DeltaTime);
        }
        return;
    }

    Sd_SfxStop(Sfx_Unk1664);
}

void func_800DCD00(void) // 0x800DCD00
{
    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();

            if (Savegame_EventFlagGet(EventFlag_552))
            {
                Event_BgTextureCmd(BgTextureCmd_QueueRead, FILE_TIM_CHAINRF2_TIM, false);
            }
            else
            {
                Event_BgTextureCmd(BgTextureCmd_QueueRead, FILE_TIM_CHAINRF1_TIM, false);
            }

            D_800E9ED8 = Q12(0.0f);
            SysWork_StateStepIncrement(0);

        case 1:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 2:
            Event_BgTextureCmd(BgTextureCmd_AwaitLoad, 0, false);
            break;

        case 3:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.0f), false);
            D_800E9ED8_Set();
            break;

        case 4:
            Sd_SfxPlay(Sfx_Unk1665, Q8(0.0f), Q8(0.5f));
            SysWork_StateStepIncrement(0);

        case 5:
            if (Savegame_EventFlagGet(EventFlag_552))
            {
                Event_DisplayMapMsg(false, MAP_MSG(92), 0, 0, 0, false);
            }
            else
            {
                Event_DisplayMapMsg(false, MAP_MSG(91), 0, 0, 0, false);
            }

            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            D_800E9ED8_Set();
            break;

        case 6:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_SysStateBranchOnFlag(EventFlag_M7S02_PickupDaggerOfMelchior, 14, 7, false);
            break;

        case 7:
            Event_InvItemCmd(InvItemCmd_QueueLoad, InvItemId_DaggerOfMelchior, 0, false);
            SysWork_StateStepIncrement(0);

        case 8:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            D_800E9ED8_Set();

            if (g_SysWork.sysStateSteps[0] != 8)
            {
                Sd_SfxStop(Sfx_Unk1664);
            }
            break;

        case 9:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.0f), false);
            break;

        case 10:
            Event_InvItemCmd(InvItemCmd_AwaitLoad, InvItemId_DaggerOfMelchior, 0, false);
            break;

        case 11:
            if (Gfx_PickupItemAnimate(InvItemId_DaggerOfMelchior) != false)
            {
                Event_DisplayMapMsg(true, MAP_MSG(82), 12, 13, 0, false);
            }

            Savegame_EventFlagSet(EventFlag_M7S02_PickupDaggerOfMelchior);
            break;

        case 12:
            Event_InvItemCmd(InvItemCmd_AddItem, InvItemId_DaggerOfMelchior, 1, false);
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;

        case 13:
            Savegame_EventFlagClear(EventFlag_M7S02_PickupDaggerOfMelchior);
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;

        case 14:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            D_800E9ED8_Set();
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            Sd_SfxStop(Sfx_Unk1664);
            break;
    }
}

void func_800DD2D4(void) // 0x800DD2D4
{
    typedef struct
    {
        SPRT*     sprt_0;
        DR_TPAGE* tpage_4;
        DR_STP*   stp_8;
        u8        unk_C[4];
        u8        unk_10[4];
        s32       activeBufferIdx_14;
    } g_GteScratchData_func_800DD2D4;

    g_GteScratchData_func_800DD2D4* scratchData;
    s32                             i;

    scratchData = PSX_SCRATCH_ADDR(0);

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Sfx_WithFlagsPlay(Sfx_Unk1667, &Q12_VECTOR3(21.1f, -1.5f, -60.75f), Q8(0.5f), SfxFlag_None);
            Fs_QueueStartRead(FILE_ANIM_KITCHEN1_DMS, FS_BUFFER_11);
            Savegame_EventFlagSet(EventFlag_557);

            g_Cutscene_Timer = NO_VALUE;

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
            Dms_HeaderFixOffsets(FS_BUFFER_11);
            CutsceneBorder_ForceShow();
            g_SysWork.sysFlags |= SysFlag_CutsceneActive;

            Anim_CharaTypeAnimInfoClear();
            Chara_Load(0, Chara_Bloodsucker, &g_SysWork.npcBoneCoordBuffer[0], CHARA_FORCE_FREE_ALL, NULL, NULL);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 144, false);

            g_Cutscene_Timer = Q12(0.0f);

            SysWork_StateStepIncrement(0);

        case 4:
            if (g_SysWork.playerWork.player.model.anim.status & 1)
            {
                g_Cutscene_Timer = g_SysWork.playerWork.player.model.anim.time - Q12(Player_AnimGetSomething());
                if (g_Cutscene_Timer > Q12(29.0f))
                {
                    g_Cutscene_Timer = Q12(29.0f);
                    SysWork_StateStepIncrement(0);
                }
            }
            break;

        case 5:
            Chara_ProcessLoads();
            Chara_Spawn(Chara_Bloodsucker, 64, Q12(21.5f), Q12(-60.8f), Q12_ANGLE(-90.0f), 17);
            SD_Call(Sfx_Unk1668);
            Savegame_EventFlagSet(EventFlag_573);

            g_WorldObject_Dor[0].rotation.vy = Q12_ANGLE(25.0f);
            g_WorldObject_Dor[1].rotation.vy = Q12_ANGLE(58.0f);
            g_WorldObject_Dor[2].rotation.vy = Q12_ANGLE(-50.0f);
            g_WorldObject_Dor[3].rotation.vy = Q12_ANGLE(-15.0f);

            SysWork_StateStepIncrement(0);

        case 6:
            g_Cutscene_Timer = g_SysWork.playerWork.player.model.anim.time - Q12(Player_AnimGetSomething());

            scratchData->activeBufferIdx_14 = g_ActiveBufferIdx;
            scratchData->sprt_0             = (SPRT*)GsOUT_PACKET_P;
            for (i = 0; i < 2; i++)
            {
                setCodeWord(scratchData->sprt_0, PRIM_RECT | RECT_BLEND | RECT_TEXTURE, COLOR_RGBC(128, 128, 128, 0));
                setXY0Fast(scratchData->sprt_0, ((i << 8) - 160), -112);
                scratchData->sprt_0->u0 = 0;
                scratchData->sprt_0->v0 = (scratchData->activeBufferIdx_14 == 0) ? 32 : 0;
                setWH(scratchData->sprt_0, (i == 0) ? 256 : 64, FRAMEBUFFER_HEIGHT_PROGRESSIVE);
                addPrimFast(&g_OrderingTable2[g_ActiveBufferIdx].org[15], scratchData->sprt_0, 4);

                scratchData->sprt_0++;
                scratchData->tpage_4 = (DR_TPAGE*)scratchData->sprt_0;
                setDrawTPage(scratchData->tpage_4, 0, 0, getTPage(2, 0, ((scratchData->activeBufferIdx_14 << 4) + (i << 2)) << 6, (((scratchData->activeBufferIdx_14 << 4) >> 4) & 1) << 8));

                AddPrim(&g_OrderingTable2[g_ActiveBufferIdx].org[15], scratchData->tpage_4);
                scratchData->tpage_4++;
                scratchData->sprt_0 = (SPRT*)scratchData->tpage_4;
            }

            scratchData->stp_8 = (DR_STP*)scratchData->sprt_0;
            SetDrawStp(scratchData->stp_8, 1);
            addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[ORDERING_TABLE_SIZE - 1], scratchData->stp_8);
            scratchData->stp_8++;
            SetDrawStp(scratchData->stp_8, 0);
            addPrim(&g_OrderingTable2[g_ActiveBufferIdx].org[0], scratchData->stp_8);
            scratchData->stp_8++;
            GsOUT_PACKET_P = (PACKET*)scratchData->stp_8;

            if (g_Cutscene_Timer > Q12(39.0f))
            {
                g_Cutscene_Timer = Q12(39.0f);
                SysWork_StateStepIncrement(0);
            }
            break;

        case 7:
            g_Cutscene_Timer = g_SysWork.playerWork.player.model.anim.time - Q12(Player_AnimGetSomething());
            if (g_Cutscene_Timer > Q12(96.0f))
            {
                SD_Call(Sfx_XaAudio635);
                SysWork_StateStepIncrement(0);
            }
            break;

        case 8:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(1.0f), false);
            g_Cutscene_Timer = g_SysWork.playerWork.player.model.anim.time - Q12(Player_AnimGetSomething());
            break;

        case 9:
            Event_WaitTimer(Q12(2.0f), false);
            break;

        default:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);

            SysWork_StateSetNext(SysState_GameOver);
            break;
    }

    if (g_SysWork.sysStateSteps[0] >= 6)
    {
        // TODO: Unsure what character `npcs[0]` is meant to be, only incubus has a timer field at 0xE8?
        g_SysWork.npcs[0].properties.player.afkTimer = g_SysWork.playerWork.player.model.anim.time - Q12(Player_AnimGetSomething());
    }

    if (g_Cutscene_Timer >= Q12(0.0f))
    {
        Dms_CharacterTransformGet(&g_SysWork.playerWork.player.position, &g_SysWork.playerWork.player.rotation, "HERO", g_Cutscene_Timer, FS_BUFFER_11);
        vcChangeProjectionValue(Dms_CameraTargetsGet(&g_Cutscene_CameraPositionTarget, &g_Cutscene_CameraLookAtTarget, NULL, g_Cutscene_Timer, FS_BUFFER_11));
        vcUserCamTarget(&g_Cutscene_CameraPositionTarget, NULL, true);
        vcUserWatchTarget(&g_Cutscene_CameraLookAtTarget, NULL, true);
    }
}

void func_800DD9E8(void) // 0x800DD9E8
{
    if (g_SysWork.sysStateSteps[0] >= 0 && g_SysWork.sysStateSteps[0] <= 6)
    {
        if (D_800E9EDA == 0)
        {
            Sfx_WithFlagsPlay(Sfx_Unk1666, &Q12_VECTOR3(21.1f, -1.5f, -60.75f), Q8(0.5f), SfxFlag_None);

            if (g_SysWork.sysStateSteps[0] == 6)
            {
                D_800E9EDA = Rng_GenerateInt(Q12(0.1f), Q12(0.2f) - 2);
            }
            else
            {
                D_800E9EDA = Rng_GenerateInt(Q12(0.4f), Q12(1.2f) - 2);
            }
        }
        else
        {
            D_800E9EDA = (D_800E9EDA - g_DeltaTime) >= 0 ? D_800E9EDA - (s16)g_DeltaTime : 0;
        }
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            D_800E9EDA = 0;
            Savegame_EventFlagSet(EventFlag_557);
            CutsceneBorder_ForceShow();
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 144, false);

            Event_CameraPositionSet(NULL, Q12(20.98f), Q12(-4.08f), Q12(-59.9f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(NULL, Q12(18.56f), Q12(-1.07f), Q12(-60.94f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);

            g_SysWork.playerWork.player.position.vx = Q12(19.2f);
            g_SysWork.playerWork.player.position.vz = Q12(-61.0f);
            g_SysWork.playerWork.player.rotation.vy = Q12_ANGLE(-90.0f);

            SysWork_StateStepIncrement(0);

        case 3:
            Event_WaitTimer(Q12(2.5f), false);
            break;

        case 4:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 51, false);
            g_SysWork.playerWork.player.rotation.vy = Q12_ANGLE(90.0f);

            Event_CameraPositionSet(NULL, Q12(16.37f), Q12(-2.56f), Q12(-60.41f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(NULL, Q12(20.18f), Q12(-1.4f), Q12(-60.76f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);

            Event_TweenReset(0);
            Event_TweenReset(2);
            Event_TweenReset(3);
            Event_TweenReset(5);

            Savegame_EventFlagSet(EventFlag_573);

            SysWork_StateStepIncrement(0);
            break;

        case 5:
            // Warp camera.
            Event_CameraPositionSet(NULL,
                                    Event_TweenLinear(Q12(1.7701f), Q12(2.0f), 0) + Q12(16.37f),
                                    Q12(-2.56f),
                                    Event_TweenLinear(Q12(-0.16f), Q12(2.0f), 2) - Q12(60.41f),
                                    Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), false);
            Event_CameraLookAtSet(NULL,
                                  Event_TweenLinear(Q12(1.7801f), Q12(2.0f), 0) + Q12(20.18f),
                                  Q12(-1.4f),
                                  Event_TweenLinear(Q12(-0.0301f), Q12(2.0f), 2) - Q12(60.76f),
                                  Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), false);

            Event_WaitTimer(Q12(2.0f), false);
            break;

        case 6:
            Event_WaitTimer(Q12(2.0f), false);
            break;

        case 7:
            Event_WaitTimer(Q12(2.4f), false);
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);

            Savegame_EventFlagSet(EventFlag_555);

            vcReturnPreAutoCamWork(false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            func_8008D448();

            Game_FlashlightAttribsSet();
            g_SysWork.lightIntensity = Q12(1.0f);
            break;
    }
}

void func_800DDEC8(void) // 0x800DDEC8
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
            Event_SysStateBranchOnFlag(EventFlag_549, 3, 7, false);
            break;

        case 3:
            g_SysWork.silentYesSelection = true;
            Event_DisplayMapMsg(true, MAP_MSG(85), 4, -1, 0, false);
            break;

        case 4:
            Sfx_WithFlagsPlay(Sfx_Unk1494, &Q12_VECTOR3(180.5f, -0.5f, -60.6f), Q8(0.5f), SfxFlag_None);
            SD_Call(Sfx_Unk1495);
            Savegame_EventFlagClear(EventFlag_549);

            D_800EBAF4 = 1;

            SysWork_StateStepIncrement(0);

        case 5:
            Event_WaitTimer(Q12(2.5f), false);
            break;

        case 7:
            g_SysWork.silentYesSelection = true;
            Event_DisplayMapMsg(true, MAP_MSG(84), 8, -1, 0, false);
            break;

        case 8:
            Sfx_WithFlagsPlay(Sfx_Unk1494, &Q12_VECTOR3(180.5f, -0.5f, -60.6f), Q8(0.5f), SfxFlag_None);
            Savegame_EventFlagSet(EventFlag_549);

            D_800EBAF4 = 0;

            SysWork_StateStepIncrement(0);

        case 9:
            Sfx_WithFalloffAndPitchPlay(Sfx_Unk1495, &D_800E9D00, (Q12(4.0f) - g_SysWork.sysStateStepData[0]) >> 7, Q12(16.0f), 0);
            Sfx_WithFalloffAndPitchPlay(Sfx_Unk1503, &D_800E9D00, (Q12(4.0f) - g_SysWork.sysStateStepData[0]) >> 7, Q12(16.0f), 0);

            g_SysWork.sysStateStepData[0] += g_DeltaTime;
            if (g_SysWork.sysStateStepData[0] > Q12(3.0f))
            {
                SysWork_StateStepIncrement(0);
            }
            break;

        case 10:
            Sd_SfxStop(Sfx_Unk1495);
            Sd_SfxStop(Sfx_Unk1503);
            Sfx_WithFlagsPlay(Sfx_Unk1497, &D_800E9D00, Q8(0.5f), SfxFlag_None);

        case 6:
            SysWork_StateStepReset();
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;
    }

    if (!Savegame_EventFlagGet(EventFlag_549))
    {
        func_800894B8((g_SysWork.sysStateSteps[0] >= 4 && g_SysWork.sysStateSteps[0] <= 5) ? 0x90 : 0x60); // Hex?
    }
}

void func_800DE1FC(void) // 0x800DE1FC
{
    typedef struct
    {
        SPRT*      sprt_0;
        DR_TPAGE*  tpage_4;
        DR_AREA*   drawArea_8;
        DR_OFFSET* drawOffset_C;
        DR_STP*    stp_10;
    } s_ScratchData;

    VECTOR3        sp18;
    SVECTOR3       sp28;
    s_FsImageDesc  charaTex;
    s32            i;
    s32            temp;
    s32            colorBase;
    s32            tpageBase;
    s32            vOffset;
    s32            barPhase;
    s_ScratchData* scratch;

    // Skip.
    if (Pad_ButtonClickedBind(skip) &&
        g_SysWork.sysStateSteps[0] >= 2 && g_SysWork.sysStateSteps[0] < 8)
    {
        SD_Call(0x13);
        SysWork_StateStepSet(0, 9);
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();

            D_800EA484 = g_SysWork.playerWork.player.position;
            D_800EB6B0 = 0;
            D_800E9EDC = Q12(1.0f);

            Fs_QueueStartRead(FILE_ANIM_HIDDEN1_DMS, FS_BUFFER_11);
            Fs_QueueWaitForEmpty();
            Dms_HeaderFixOffsets(FS_BUFFER_11);

            g_Cutscene_Timer = Q12(0.0f);

            Anim_CharaTypeAnimInfoClear();
            Chara_Load(0, Chara_GhostDoctor, &D_800E9EE4, -1, NULL, NULL);
            Chara_Load(1, Chara_Dahlia, g_SysWork.npcBoneCoordBuffer, 0, NULL, NULL);
            Chara_FsImageCalc(&charaTex, Chara_Kaufmann, 2);
            Chara_Load(2, Chara_Kaufmann, &g_SysWork.npcBoneCoordBuffer[30], 0, LM_BUFFER_2, &charaTex);
            Chara_ProcessLoads();

            Chara_Spawn(Chara_Kaufmann, 0, Q12(140.0f), Q12(-60.0f), 0, 3);
            Chara_Spawn(Chara_Dahlia, 0, Q12(140.0f), Q12(-60.0f), 0, 3);
            Chara_Spawn(Chara_GhostDoctor, 0, Q12(140.0f), Q12(-60.0f), 0, 3);
            Chara_Spawn(Chara_GhostDoctor, 1, Q12(140.0f), Q12(-60.0f), 0, 3);
            Screen_FadeTimestepReset();

            CutsceneBorder_ForceShow();
            g_SysWork.sysFlags           |= 1 << 3;

            func_8008D438();

            g_SysWork.lightBoneCoord     = NULL;
            g_SysWork.lensFlareBoneCoord = NULL;
            g_SysWork.lightIntensity     = Q12(0.6f);

            Game_TurnFlashlightOn();
            SysWork_StateStepIncrement(0);
            break;

        case 1:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[1], 5, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 5, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[2], 0, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[3], 0, false);

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 51, false);
            Model_AnimFlagsClear(&g_SysWork.playerWork.player.model, 2);
            ClearImage(&D_800E9D1C, 0, 0, 0);
            SysWork_StateStepIncrement(0);

        case 2:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, 0, false);
            WorldObjects_Add(&g_WorldObject_Cover.object, &g_WorldObject_Cover.position, &SVECTOR3_Zero);
            break;

        case 3:
            Event_DisplayMapMsgWithAudio(MAP_MSG(47), &D_800EB6B0, &D_800E9D30);
            WorldObjects_Add(&g_WorldObject_Cover.object, &g_WorldObject_Cover.position, &SVECTOR3_Zero);
            break;

        case 4:
            WorldObjects_Add(&g_WorldObject_Cover.object, &g_WorldObject_Cover.position, &SVECTOR3_Zero);

            scratch             = PSX_SCRATCH;
            scratch->drawArea_8 = (DR_AREA*)GsOUT_PACKET_P;

            SetDrawArea(scratch->drawArea_8, (g_ActiveBufferIdx == 0) ? &D_800E9D0C[1] : &D_800E9D0C[0]);
            addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[2], scratch->drawArea_8);
            scratch->drawArea_8++;

            scratch->drawOffset_C = scratch->drawArea_8;
            SetDrawOffset(scratch->drawOffset_C, (g_ActiveBufferIdx == 0) ? &D_800E9D24[2] : &D_800E9D24[0]);
            addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[2], scratch->drawOffset_C);
            scratch->drawOffset_C++;

            scratch->stp_10 = scratch->drawOffset_C;
            SetDrawStp(scratch->stp_10, 0);
            addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[2], scratch->stp_10);

            scratch->stp_10++;
            scratch->sprt_0 = scratch->stp_10;

            for (i = 0; i < 2; i++)
            {
                setCodeWord(scratch->sprt_0, PRIM_RECT | RECT_TEXTURE, COLOR_RGBC(128, 128, 128, 0));
                setXY0Fast(scratch->sprt_0, ((i << 8) - 160), -112);

                scratch->sprt_0->u0 = 0;
                scratch->sprt_0->v0 = (g_ActiveBufferIdx != 0) << 5;

                setWH(scratch->sprt_0, i != 0 ? 64 : 256, FRAMEBUFFER_HEIGHT_PROGRESSIVE);

                addPrimFast(&g_OrderingTable0[g_ActiveBufferIdx].org[2], scratch->sprt_0, 4);

                scratch->sprt_0++;
                scratch->tpage_4 = (DR_TPAGE*)scratch->sprt_0;

                temp = 4;
                setDrawTPage(scratch->tpage_4, 0, 0,
                             getTPage(2, 0, ((i * temp + (g_ActiveBufferIdx == 0) * 16) & 0xF) << 6, ((g_ActiveBufferIdx == 0)) << 8));

                AddPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[2], scratch->tpage_4);
                scratch->tpage_4++;
                scratch->sprt_0 = (SPRT*)scratch->tpage_4;
            }

            scratch->drawArea_8 = (DR_AREA*)scratch->sprt_0;
            SetDrawArea(scratch->drawArea_8, &D_800E9D1C);
            addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[2], scratch->drawArea_8);
            scratch->drawArea_8++;

            scratch->drawOffset_C = scratch->drawArea_8;
            SetDrawOffset(scratch->drawOffset_C, &D_800E9D2C);
            addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[2], scratch->drawOffset_C);
            scratch->drawOffset_C++;

            scratch->stp_10 = scratch->drawOffset_C;
            SetDrawStp(scratch->stp_10, 1);
            addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[2], scratch->stp_10);
            scratch->stp_10++;

            GsOUT_PACKET_P = scratch->stp_10;
            SysWork_StateStepIncrement(0);
            break;

        case 5:
            Chara_ModelCharaIdClear(&g_SysWork.npcs[1], 0, 0);
            Chara_ModelCharaIdClear(&g_SysWork.npcs[0], 0, 0);
            Chara_ModelCharaIdClear(&g_SysWork.npcs[2], 0, 0);
            Chara_ModelCharaIdClear(&g_SysWork.npcs[3], 0, 0);
            SysWork_StateStepIncrement(0);

        case 6:
            WorldObjects_Add(&g_WorldObject_Bed3.object, &g_WorldObject_Bed3.position, &g_WorldObject_Bed3.rotation);

            colorBase       = D_800E9EDC >> 5;
            scratch         = PSX_SCRATCH;
            scratch->sprt_0 = GsOUT_PACKET_P;
            vOffset         = 0;
            barPhase        = 1;
            tpageBase       = 21;

            for (i = 0; i < 6; i++)
            {
                if (i == 2)
                {
                    barPhase  = 2;
                    vOffset   = (g_ActiveBufferIdx != 0) << 5;
                    tpageBase = !(g_ActiveBufferIdx) * 16;
                }
                else if (i == 4)
                {
                    colorBase = 0x80;
                    barPhase  = 0;
                    vOffset   = (g_ActiveBufferIdx == 0) << 5;
                    tpageBase = g_ActiveBufferIdx * 16;
                }

                setRGBC0(scratch->sprt_0, colorBase, colorBase, colorBase, PRIM_RECT | RECT_BLEND | RECT_TEXTURE);
                setXY0Fast(scratch->sprt_0, ((i & 0x1) << 8) - (SCREEN_WIDTH / 2), -(FRAMEBUFFER_HEIGHT_PROGRESSIVE / 2));

                scratch->sprt_0->u0 = 0;
                scratch->sprt_0->v0 = vOffset;

                setWH(scratch->sprt_0, (i & 0x1) ? 64 : 256, FRAMEBUFFER_HEIGHT_PROGRESSIVE);

                if (i < 4)
                {
                    addPrimFast(&g_OrderingTable0[g_ActiveBufferIdx].org[2], scratch->sprt_0, 4);
                }
                else
                {
                    addPrimFast(&g_OrderingTable2[g_ActiveBufferIdx].org[15], scratch->sprt_0, 4);
                }

                scratch->sprt_0++;
                scratch->tpage_4 = (DR_TPAGE*)scratch->sprt_0;

                setDrawTPage(scratch->tpage_4, 0, 0,
                             getTPage(2, barPhase, (tpageBase + ((i & 1) * 4)) << 6, ((tpageBase >> 4) & (1 << 0)) << 8));

                if (i < 4)
                {
                    AddPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[2], scratch->tpage_4);
                }
                else
                {
                    AddPrim(&g_OrderingTable2[g_ActiveBufferIdx].org[15], scratch->tpage_4);
                }

                scratch->tpage_4++;
                scratch->sprt_0 = scratch->tpage_4;
            }

            scratch->stp_10 = (DR_STP*)scratch->sprt_0;
            SetDrawStp(scratch->stp_10, 1);
            addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[0x7FF], scratch->stp_10);
            scratch->stp_10++;

            SetDrawStp(scratch->stp_10, 0);
            addPrim(&g_OrderingTable2[g_ActiveBufferIdx].org[0], scratch->stp_10);
            scratch->stp_10++;

            GsOUT_PACKET_P = scratch->stp_10;

            D_800E9EDC -= Q12_MULT_PRECISE(g_DeltaTime, Q12(0.5f));
            if (D_800E9EDC < 0)
            {
                SysWork_StateStepIncrement(0);
                Savegame_EventFlagSet(EventFlag_551);
                Event_CharaAnimCmdExecute(CharaAnimCmd_AnimLock, &g_SysWork.playerWork.player, 0, false);
            }
            break;

        case 7:
            Event_WaitTimer(Q12(1.5f), false);
            break;

        case 8:
            SysWork_StateStepSet(0, NO_VALUE);
            break;

        case 9:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, 0, false);
            break;

        case 10:
            Chara_ModelCharaIdClear(&g_SysWork.npcs[1], 0, 0);
            Chara_ModelCharaIdClear(&g_SysWork.npcs[0], 0, 0);
            Chara_ModelCharaIdClear(&g_SysWork.npcs[2], 0, 0);
            Chara_ModelCharaIdClear(&g_SysWork.npcs[3], 0, 0);

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, 0, false);
            Savegame_EventFlagSet(EventFlag_551);
            SysWork_StateStepIncrement(0);

        default:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimUnlock, &g_SysWork.playerWork.player, 0, false);
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            Model_AnimFlagsSet(&g_SysWork.playerWork.player.model, 2);

            vcReturnPreAutoCamWork(false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, 0, false);
            func_8008D448();
            Game_FlashlightAttribsSet();

            g_SysWork.lightIntensity = Q12(1.0f);
            g_Cutscene_Timer         = NO_VALUE;

            Anim_CharaTypeAnimInfoClear();
            break;
    }

    if (g_Cutscene_Timer >= 0)
    {
        g_DeltaTime >>= 1;

        Dms_CharacterTransformGet(&g_SysWork.playerWork.player.position, &g_SysWork.playerWork.player.rotation, "HERO", g_Cutscene_Timer, FS_BUFFER_11);
        Dms_CharacterTransformGet(&g_SysWork.npcs[1].position, &g_SysWork.npcs[1].rotation, "DARIA", g_Cutscene_Timer, FS_BUFFER_11);
        Dms_CharacterTransformGet(&g_SysWork.npcs[0].position, &g_SysWork.npcs[0].rotation, "KAU", g_Cutscene_Timer, FS_BUFFER_11);
        Dms_CharacterTransformGet(&g_SysWork.npcs[2].position, &g_SysWork.npcs[2].rotation, "DOC1", g_Cutscene_Timer, FS_BUFFER_11);
        Dms_CharacterTransformGet(&g_SysWork.npcs[3].position, &g_SysWork.npcs[3].rotation, "DOC2", g_Cutscene_Timer, FS_BUFFER_11);
        Dms_CharacterTransformGet(&g_SysWork.lightPosition, &sp28, "LIGHT", g_Cutscene_Timer, FS_BUFFER_11);
        Dms_CharacterTransformGet(&sp18, &sp28, "L_INT", g_Cutscene_Timer, FS_BUFFER_11);

        g_SysWork.lightRotation.vx = -ratan2(sp18.vy - g_SysWork.lightPosition.vy,
                                                  Math_Vector2MagCalcSafeQ6(sp18.vx - g_SysWork.lightPosition.vx,
                                                                      sp18.vz - g_SysWork.lightPosition.vz));
        g_SysWork.lightRotation.vy = ratan2(sp18.vx - g_SysWork.lightPosition.vx, sp18.vz - g_SysWork.lightPosition.vz);
        g_SysWork.lightRotation.vz = 0;

        Vc_UpdateLookAtPointSetAlt();
    }
}

void func_800DF1E8(void) {}

void MapEvent_DiskOfOuroborosTake(void) // 0x800DF1F0
{
    Event_ItemTake(InvItemId_DiskOfOuroboros, DEFAULT_PICKUP_ITEM_COUNT, EventFlag_M7S02_PickupDiskOfOuroboros, MAP_MSG(83));
}

void func_800DF21C(void) // 0x800DF21C
{
    s_FsImageDesc charaTex;
    VECTOR3       lightIntPos;
    SVECTOR3      unused;

    if (Pad_ButtonClickedBind(skip) &&
        g_SysWork.sysStateSteps[0] >= 3 && g_SysWork.sysStateSteps[0] < 11)
    {
        SysWork_StateStepSet(0, 11);
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            D_800EB6B0 = 0;
            Game_TurnFlashlightOn();
            CutsceneBorder_ForceShow();
            Screen_FadeTimestepReset();
            g_SysWork.sysFlags |= SysFlag_CutsceneActive;
            func_8003D03C();
            Player_EmptyWeaponHandSet();

            Fs_QueueStartRead(FILE_ANIM_HALLWAY_DMS, FS_BUFFER_11);
            Fs_QueueWaitForEmpty();
            Dms_HeaderFixOffsets(FS_BUFFER_11);

            g_Cutscene_Timer = Q12(0.0f);

            Anim_CharaTypeAnimInfoClear();
            Chara_Load(0, Chara_GhostChildAlessa, &g_SysWork.npcBoneCoordBuffer[0], CHARA_FORCE_FREE_ALL, NULL, NULL);
            Chara_FsImageCalc(&charaTex, Chara_Dahlia, 2);
            Chara_Load(1, Chara_Dahlia, &g_SysWork.npcBoneCoordBuffer[30], 0, LM_BUFFER_2, &charaTex);
            Chara_ProcessLoads();
            Chara_Spawn(Chara_GhostChildAlessa, 0, Q12(100.0f), Q12(-100.0f), 0, 3);
            Chara_Spawn(Chara_Dahlia, 0, Q12(100.0f), Q12(-100.0f), 0, 3);

            g_SysWork.lightBoneCoord     = NULL;
            g_SysWork.lensFlareBoneCoord = NULL;
            g_SysWork.lightIntensity     = Q12(0.8f);

            Savegame_EventFlagSet(EventFlag_560);
            Event_DisplayMapMsgWithAudio(MAP_MSG(62), &D_800EB6B0, &D_800E9D50);
            SysWork_StateStepIncrement(0);
            break;

        case 1:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 6, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[1], 25, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 51, false);

            Model_AnimFlagsClear(&g_SysWork.playerWork.player.model, AnimFlag_Visible);

            Event_DisplayMapMsgWithAudio(MAP_MSG(62), &D_800EB6B0, &D_800E9D50);
            SysWork_StateStepIncrement(0);
            break;

        case 2:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.6f), false);
            SysWork_StateStepIncrement(0);

        case 3:
            Event_DisplayMapMsgWithAudio(MAP_MSG(62), &D_800EB6B0, &D_800E9D50);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(5.0f), Q12(0.0f), Q12(32.0f), true, false);
            break;

        case 4:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 8, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[1], 27, false);
            Model_AnimFlagsSet(&g_SysWork.playerWork.player.model, AnimFlag_Visible);
            SysWork_StateStepIncrement(0);

        case 5:
            Event_DisplayMapMsgWithAudio(MAP_MSG(67), &D_800EB6B0, &D_800E9D50);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(5.0f), Q12(33.0f), Q12(59.0f), true, false);
            break;

        case 6:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(5.0f), Q12(33.0f), Q12(59.0f), true, true);
            break;

        case 7:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 10, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[1], 12, false);

            g_Cutscene_Timer = Q12(60.0f);

            Savegame_EventFlagClear(EventFlag_560);
            SysWork_StateStepIncrement(0);

        case 8:
            Event_DisplayMapMsgWithAudio(MAP_MSG(68), &D_800EB6B0, &D_800E9D50);
            break;

        case 9:
            g_Cutscene_Timer = Q12(61.0f);
            Savegame_EventFlagSet(EventFlag_560);
            Event_DisplayMapMsgWithAudio(MAP_MSG(71), &D_800EB6B0, &D_800E9D50);
            break;

        case 10:
            g_SysWork.npcs[1].timer_C6 += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.25f);
            g_SysWork.npcs[0].timer_C6 += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.25f);
            if (g_SysWork.npcs[0].timer_C6 > Q12(1.0f))
            {
                g_SysWork.npcs[0].timer_C6 = Q12(1.0f);
                g_SysWork.npcs[1].timer_C6 = Q12(1.0f);

                SysWork_StateStepReset();
            }
            break;

        case 11:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 12:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            SysWork_StateStepIncrement(0);

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            vcReturnPreAutoCamWork(true);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            Chara_ModelCharaIdClear(&g_SysWork.npcs[0], 0, 0);
            Chara_ModelCharaIdClear(&g_SysWork.npcs[1], 0, 0);
            Anim_CharaTypeAnimInfoClear();
            func_8008D448();

            Game_FlashlightAttribsSet();
            g_SysWork.lightIntensity = Q12(1.0f);

            g_Cutscene_Timer = NO_VALUE;

            SD_Call(19);
            Savegame_EventFlagSet(EventFlag_560);
            func_8003D01C();
            Player_WeaponAttackRestore();
            Model_AnimFlagsSet(&g_SysWork.playerWork.player.model, AnimFlag_Visible);
            break;
    }

    if (g_Cutscene_Timer >= Q12(0.0f))
    {
        Dms_CharacterTransformGet(&g_SysWork.playerWork.player.position, &g_SysWork.playerWork.player.rotation, "HERO", g_Cutscene_Timer, FS_BUFFER_11);

        Dms_CharacterTransformGet(&g_SysWork.npcs[0].position, &g_SysWork.npcs[0].rotation, "ALESSA", g_Cutscene_Timer, FS_BUFFER_11);
        Dms_CharacterTransformGet(&g_SysWork.npcs[1].position, &g_SysWork.npcs[1].rotation, "DAHLIA", g_Cutscene_Timer, FS_BUFFER_11);

        Dms_CharacterTransformGet(&g_SysWork.lightPosition, &unused, "LIGHT", g_Cutscene_Timer, FS_BUFFER_11);
        Dms_CharacterTransformGet(&lightIntPos, &unused, "L_INT", g_Cutscene_Timer, FS_BUFFER_11);

        g_SysWork.lightRotation.vx = -ratan2(lightIntPos.vy - g_SysWork.lightPosition.vy, Math_Vector2MagCalcSafeQ6(lightIntPos.vx - g_SysWork.lightPosition.vx, lightIntPos.vz - g_SysWork.lightPosition.vz));
        g_SysWork.lightRotation.vy = ratan2(lightIntPos.vx - g_SysWork.lightPosition.vx, lightIntPos.vz - g_SysWork.lightPosition.vz);
        g_SysWork.lightRotation.vz = 0;

        vcChangeProjectionValue(Dms_CameraTargetsGet(&g_Cutscene_CameraPositionTarget, &g_Cutscene_CameraLookAtTarget, NULL, g_Cutscene_Timer, FS_BUFFER_11));
        vcUserCamTarget(&g_Cutscene_CameraPositionTarget, NULL, true);
        vcUserWatchTarget(&g_Cutscene_CameraLookAtTarget, NULL, true);

        g_SysWork.playerWork.player.rotation.vy += Q12_ANGLE(16.88f);
    }
}

void func_800DFA54(void) // 0x800DFA54
{
    Event_DisplayMapMsgWithBg(FILE_TIM_ABUSEDSK_TIM, Q12(0.0f), Q12(0.0f), MAP_MSG(98));
}

void MapEvent_ScrewdriverTake(void) // 0x800DFA80
{
    Event_ItemTake(InvItemId_Screwdriver, DEFAULT_PICKUP_ITEM_COUNT, EventFlag_M7S01_PickupScrewdriver, MAP_MSG(94));
}

void func_800DFAAC(void) // 0x800DFAAC
{
    g_Screen_BackgroundImgGamma = Q8(13.0f / 32.0f);
    Event_DisplayMapMsgWithBg(FILE_TIM_LITHGRPH_TIM, Q12(0.0f), Q12(0.0f), MAP_MSG(101));
}

void func_800DFAE4(void) // 0x800DFAE4
{
    g_Screen_BackgroundImgGamma = Q8(13.0f / 32.0f);
    Event_DisplayMapMsgWithBg(FILE_TIM_LITHGR_3_TIM, Q12(0.0f), Q12(0.0f), MAP_MSG(102));
}

void MapEvent_AmuletOfSolomonTake(void) // 0x800DFB1C
{
    Event_ItemTake(InvItemId_AmuletOfSolomon, DEFAULT_PICKUP_ITEM_COUNT, EventFlag_M7S01_PickupAmuletOfSolomon, MAP_MSG(96));
}

void func_800DFB48(void) // 0x800DFB48
{
    g_Screen_BackgroundImgGamma = Q8(9.0f / 32.0f);

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_BgTextureCmd(BgTextureCmd_QueueRead, FILE_TIM_LISDIARY_TIM, false);

            D_800EA490 = 0;

            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            Event_CharaAnimPlayToEnd(&g_SysWork.playerWork.player, 59);
            break;

        case 3:
            Event_DisplayMapMsg(false, MAP_MSG(147), 0, 0, 0, false);
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
            Event_DisplayMapMsg(false, D_800E9D6C[D_800EA490], 0, 0, 0, false);
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            break;

        case 10:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);

            D_800EA490++;
            if (D_800EA490 >= 8)
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

void func_800DFDDC(void) // 0x800DFDDC
{
    // @note Mirror any changes to `map7_s01::func_800D9C9C` (likely copy-pasted func with some small differences).
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

    if (D_800E9D8E != 0)
    {
        if ((g_SysWork.sysStateSteps[0] >= 8 && g_SysWork.sysStateSteps[0] <= 10) || g_SysWork.sysStateSteps[0] == 15)
        {
            sprt = (SPRT*)GsOUT_PACKET_P;
            setCodeWord(sprt, PRIM_RECT | RECT_TEXTURE, COLOR_RGBC(128, 128, 128, 0));
            setXY0Fast(sprt, (((D_800E9D8E - 1) % 3) * 35) - 42, (((D_800E9D8E - 1) / 3) * 35) - 32);
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
            D_800E9D8E         = 0;
            D_800EA494.field_0 = 0;

            while (true)
            {
                if (Savegame_EventFlagGet(EventFlag_500 + D_800EA494.field_0))
                {
                    break;
                }
                D_800EA494.field_0++;
            }

            Event_BgTextureCmd(BgTextureCmd_QueueRead, D_800E9D80[D_800EA494.field_0], false);
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
            if (Pad_ButtonClickedBind(enter) ||
                Pad_ButtonClickedBind(cancel))
            {
                if (!Savegame_EventFlagGet(EventFlag_487) &&
                    (D_800EA494.field_0 == 1 || D_800EA494.field_0 == 5 || D_800EA494.field_0 == 6))
                {
                    SysWork_StateStepSet(0, 5);
                }
                else
                {
                    SysWork_StateStepSet(0, 15);
                }
            }
            else if (Pad_ButtonClickedBind(cancel))
            {
                SysWork_StateStepSet(0, 15);
            }

            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            break;

        case 6:
            Event_BgTextureCmd(BgTextureCmd_Auto, FILE_TIM_HS_PANEL_TIM, false);
            break;

        case 7:
            if (D_800EA494.field_0 == 1)
            {
                var_v0 = EventFlag_507;
            }
            else if (D_800EA494.field_0 == 5)
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
                    D_800E9D8E += 1 << i;
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
            Gfx_CursorDraw((s16)(FP_FROM(sharedData_800E2CA8_7_s01, 12) + 8),
                           (s16)FP_FROM(sharedData_800E2CAC_7_s01, 12) + 8,
                           8, 8, 0, 0x40, 0x20, 0x20, 0x80, 0xC0, 0, 0xC);

            if (Pad_ButtonClickedBind(cancel))
            {
                SysWork_StateStepSet(0, 15);
                break;
            }

            if (Pad_ButtonClickedBind(enter))
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
                            D_800E9D8E = j + (i * 3) + 1;
                            SD_Call(Sfx_Base);

                            // @hack Maybe not meant to reuse `temp_a0_2`, but that helps it to match.
                            if (D_800EA494.field_0 == 1)
                            {
                                temp_a0 = EventFlag_507;
                            }
                            else if (D_800EA494.field_0 == 5)
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
                                if ((D_800E9D8E >> i) & 1)
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

                            if (D_800E9D7C == j)
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
            D_800EA492         = 0;

            Model_AnimFlagsClear(&g_SysWork.playerWork.player.model, AnimFlag_Visible);
            func_8008D438();

            // Warp camera.
            Event_CameraPositionSet(NULL,
                                    Q12(-138.15f), Q12(-1.71f), Q12(-138.77f),
                                    Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                    true);
            Event_CameraLookAtSet(NULL,
                                  Q12(-140.56f), Q12(-1.03f), Q12(-141.88f),
                                  Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                  true);

            WorldObject_ModelNameSet(&D_800EBB64.object, "STONE2_H");
            SysWork_StateStepIncrement(0);

        case 12:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.0f), false);
            break;

        case 13:
            switch (g_SysWork.sysStateSteps[1])
            {
                case 0:
                    SD_Call(Sfx_Base);

                case 1:
                    g_SysWork.sysStateStepData[0] += g_DeltaTime;
                    if (g_SysWork.sysStateStepData[0] > Q12(2.0f))
                    {
                        g_SysWork.sysStateStepData[0] = Q12(2.0f);

                        WorldObject_ModelNameSet(&D_800EBB64.object, "STONE3_H");
                        Sd_SfxStop(Sfx_Base);

                        SysWork_StateStepIncrement(1);
                    }

                    D_800EBB64.position.vz = ((g_SysWork.sysStateStepData[0] * Q12(0.1f)) / Q12(2.0f)) - Q12(140.6f);
                    break;

                case 2:
                    D_800EA492 += g_GravitySpeed >> 1;

                    if (D_800EBB64.position.vy < Q12(-1.05f))
                    {
                        D_800EBB64.position.vy += Q12_MULT_PRECISE(g_DeltaTime, D_800EA492);
                        if (D_800EBB64.position.vy > Q12(-1.05f))
                        {
                            D_800EBB64.position.vy = Q12(-1.05f);

                            SysWork_StateStepIncrement(1);
                            g_SysWork.sysStateStepData[0] = Q12(0.0f);
                            D_800EA492         = 0;

                            SD_Call(Sfx_Base);
                        }
                    }
                    break;

                case 3:
                    D_800EBB64.rotation.vz += Q12_MULT_PRECISE(g_DeltaTime, Q12(-0.0694f));
                    D_800EBB64.position.vy  = Q12_MULT(Math_Cos(D_800EBB64.rotation.vz), Q12(0.15f)) - (Q12(1.2f) - 1); // TODO: Why `- 1`?
                    D_800EBB64.position.vz  = Q12_MULT(Math_Sin(D_800EBB64.rotation.vz), Q12(-0.15f)) - Q12(140.5f);

                    g_SysWork.sysStateStepData[0] += g_DeltaTime;
                    if (g_SysWork.sysStateStepData[0] > Q12(0.5f))
                    {
                        SysWork_StateStepIncrement(1);
                        g_SysWork.sysStateStepData[0] = Q12(0.0f);
                    }
                    break;

                case 4:
                    D_800EA492                += g_GravitySpeed;
                    D_800EBB64.rotation.vz += Q12_MULT_PRECISE(g_DeltaTime, Q12(-0.3333f));
                    D_800EBB64.position.vy += Q12_MULT_PRECISE(D_800EA492, g_DeltaTime);
                    D_800EBB64.position.vz += Q12_MULT_PRECISE(g_DeltaTime, Q12(0.3f));

                    if (D_800EBB64.position.vy > 0)
                    {
                        SysWork_StateStepIncrement(1);
                    }
                    break;

                case 5:
                    SD_Call(Sfx_Base);

                    D_800EBB64.position.vx = Q12(-139.5f);
                    D_800EBB64.position.vy = Q12(-0.05f);
                    D_800EBB64.position.vz = Q12(-140.0f);
                    D_800EBB64.rotation.vz = Q12(-0.25f);

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
            Savegame_EventFlagClear(EventFlag_500 + D_800EA494.field_0);
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
            Savegame_EventFlagClear(EventFlag_500 + D_800EA494.field_0);
            break;
    }
}

void MapEvent_CrestOfMercuryTake(void) // 0x800E0C30
{
    Event_ItemTake(InvItemId_CrestOfMercury, DEFAULT_PICKUP_ITEM_COUNT, EventFlag_M7S01_PickupCrestOfMercury, MAP_MSG(97));
}

void MapEvent_RingOfContractTake(void) // 0x800E0C5C
{
    Event_ItemTake(InvItemId_RingOfContract, DEFAULT_PICKUP_ITEM_COUNT, EventFlag_M7S01_PickupRingOfContract, MAP_MSG(93));
}

void MapEvent_CameraTake(void) // 0x800E0C88
{
    Event_ItemTake(InvItemId_Camera, DEFAULT_PICKUP_ITEM_COUNT, EventFlag_M7S01_PickupCamera, MAP_MSG(95));
}

void func_800E0CB4(void) // 0x800E0CB4
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

            D_800EA494.field_2 = Q12(0.3f);
            g_Cutscene_Timer    = Q12(0.0f);

            SysWork_StateStepIncrement(0);

        case 1:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(0.0f), Q12(31.0f), false, true);

            // TODO: Possible inline?
            var_s0 = &D_800EA494.field_2;
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
            var_s0 = &D_800EA494.field_2;
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

void func_800E0FF0(void) // 0x800E0FF0
{
    if (Savegame_EventFlagGet(EventFlag_486) && Savegame_EventFlagGet(EventFlag_549))
    {
        g_Screen_BackgroundImgGamma = Q8(15.0f / 32.0f);

        switch (g_SysWork.sysStateSteps[0])
        {
            case 0:
                Player_ControlFreeze();
                SysWork_StateStepIncrement(0);

            case 1:
                Event_BgTextureLoadFadeIn(FILE_TIM_ELCWIRE2_TIM, 0);
                break;

            case 2:
                Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
                SysWork_StateStepIncrement(0);

            case 3:
                SysWork_StateStepIncrement(0);

            case 4:
                Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
                Event_ScreenFadeCmd(ScreenFadeCmd_Wait, false, 0, Q12(0.0f), false);
                break;

            case 5:
                Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
                Event_DisplayMapMsg(true, MAP_MSG(80), 6, 7, 0, false);
                break;

            case 6:
                Event_InvItemCmd(InvItemCmd_AddItem, InvItemId_KeyOfAratron, 1, false);
                Savegame_EventFlagSet(EventFlag_M7S02_PickupKeyOfAratron);
                SysWork_StateStepIncrement(0);

            case 7:
                Event_BgTextureFadeOut(0, 0);
                break;

            default:
                Player_ControlUnfreeze(false);
                SysWork_StateSetNext(SysState_Gameplay);
                break;
        }
    }
    else
    {
        if (Savegame_EventFlagGet(EventFlag_486))
        {
            Event_DisplayMapMsgWithBg(FILE_TIM_ELCWIRE1_TIM, 0, 0, MAP_MSG(105));
            if (g_SysWork.sysStateSteps[1] >= 3 && g_SysWork.sysStateSteps[1] <= 5)
            {
                // Plays SFX after random time between 0.2f - 2.2f?
                if (D_800EBB94 != Q12(0.0f))
                {
                    D_800EBB94 -= g_DeltaTime;
                    if (D_800EBB94 < Q12(0.0f))
                    {
                        D_800EBB94 = Q12(0.0f);
                    }
                }
                else
                {
                    SD_Call(Sfx_Unk1653);
                    D_800EBB94 = Rng_GenerateInt(Q12(0.2f), Q12(2.2f) - 1);
                }
            }
        }
        else
        {
            g_Screen_BackgroundImgGamma = Q8(7.0f / 16.0f);
            Event_DisplayMapMsgWithBg(FILE_TIM_ELCWIRE0_TIM, 0, 0, MAP_MSG(110));
        }
    }
}

void func_800E12E4(void) // 0x800E12E4
{
    if (Savegame_EventFlagGet(EventFlag_496))
    {
        if (Savegame_EventFlagGet(EventFlag_498))
        {
            Event_DisplayMapMsgWithBg(FILE_TIM_3X3HINT2_TIM, 0, 0, MAP_MSG(108));
        }
        else
        {
            Event_DisplayMapMsgWithBg(FILE_TIM_3X3PICT2_TIM, 0, 0, MAP_MSG(108));
        }
    }
    else
    {
        if (Savegame_EventFlagGet(EventFlag_499))
        {
            Event_DisplayMapMsgWithBg(FILE_TIM_3X3HINT1_TIM, 0, 0, MAP_MSG(107));
        }
        else
        {
            Event_DisplayMapMsgWithBg(FILE_TIM_3X3PICT1_TIM, 0, 0, MAP_MSG(107));
        }
    }

    Savegame_EventFlagClear(EventFlag_496);
    Savegame_EventFlagClear(EventFlag_497);
}

void func_800E1398(void) // 0x800E1398
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

            Screen_FadeReset();
            g_SysWork.bgmStatusFlags |= BgmStatusFlag_Pause;

            SD_Call(0x679U);
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
                Event_DisplayMapMsg(false, MAP_MSG(108), 0, 0, 0, false);
            }
            else
            {
                Event_DisplayMapMsg(false, MAP_MSG(107), 0, 0, 0, false);
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

#include "maps/common/shared/sharedFunc_800DB60C_7_s01.h" // 0x800E15D4

void func_800E1DAC(void) // 0x800E1DAC
{
    const static VECTOR3 SFX_POS = { Q12(100.8f), Q12(-1.0f), Q12(-20.0f) };

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();

            D_800EB6B0 = 0;

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
            Event_DisplayMapMsg(false, MAP_MSG(15), 0, 0, 0, false);
            break;

        case 5:
            g_Screen_BackgroundImgGamma = Q8(6.0f / 32.0f);

            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_DisplayMapMsg(false, MAP_MSG(16), 0, 0, 0, false);
            break;

        case 6:
            SD_Call(Sfx_Unk1654);
            SysWork_StateStepIncrement(0);

        case 7:
            g_Screen_BackgroundImgGamma = Q8(6.0f / 32.0f);

            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_DisplayMapMsg(false, MAP_MSG(17), 0, 0, 0, false);
            break;

        case 8:
            g_Screen_BackgroundImgGamma = Q8(6.0f / 32.0f);

            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_DisplayMapMsg(false, MAP_MSG(20), 0, 0, 0, false);
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

void func_800E1FF4(void) // 0x800E1FF4
{
    const static VECTOR3 SFX_POS = { Q12(97.8f), Q12(-1.0f), Q12(-19.2f) };

    if (g_SysWork.sysStateSteps[1] == 0)
    {
        Sfx_WithFlagsPlay(Sfx_Unk1464, &SFX_POS, Q8(0.5f), SfxFlag_None);
    }

    Event_DisplayMapMsgWithDimmedBg(FILE_TIM_PLANTBK_TIM, Q12(0.0f), Q12(0.0f), MAP_MSG(22), MAP_MSG(23));
}

const VECTOR3 sharedData_800CB094_3_s01 = { Q12(-100.9f), Q12(-2.0f), Q12(-60.9f) };

#include "maps/common/shared/sharedFunc_800D15F0_3_s01.h" // 0x800E204C

void func_800E2DC0(void) // 0x800E2DC0
{
    Event_DisplayMapMsgWithBg(FILE_TIM_ALESSA_TIM, Q12(0.0f), Q12(0.0f), MAP_MSG(124));
}

void func_800E2DEC(void) // 0x800E2DEC
{
    VECTOR3 playerPosBackup;
    s32     i;

    g_Screen_BackgroundImgGamma = Q8(13.0f / 32.0f);

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            for (i = 0; i < 5; i++)
            {
                if (Savegame_EventFlagGet(EventFlag_564 + i))
                {
                    break;
                }
            }

            if (i != 5)
            {
                D_800EA4A9 = i;
                Savegame_EventFlagSet(EventFlag_540 + D_800EA4A9);

                for (i = 0; i < 5; i++)
                {
                    if (!Savegame_EventFlagGet(EventFlag_540 + i))
                    {
                        break;
                    }
                }

                if (i == 5)
                {
                    Savegame_EventFlagSet(EventFlag_545);
                }

                playerPosBackup                            = g_SysWork.playerWork.player.position;
                g_SysWork.playerWork.player.position.vx = Q12(101.0f);
                g_SysWork.playerWork.player.position.vz = Q12(-98.8f);
                WorldGfx_CloseRangeChunksInit();
                g_SysWork.playerWork.player.position = playerPosBackup;

                SysWork_StateStepSet(0, 3);
            }
            else
            {
                SysWork_StateStepSet(0, 7);
            }
            break;

        case 3:
            Event_DisplayMapMsg(false, MAP_MSG(125) + D_800EA4A9, 0, 0, 0, false);
            break;

        case 4:
            Sfx_WithFlagsPlay(Sfx_Unk1663, &Q12_VECTOR3(60.0f, -1.2f, -96.0f), Q8(0.5f), SfxFlag_None);

            if (!Savegame_EventFlagGet(EventFlag_545))
            {
                Savegame_EventFlagClear(EventFlag_564 + D_800EA4A9);
                Player_ControlUnfreeze(false);
                SysWork_StateSetNext(SysState_Gameplay);
            }
            else
            {
                SysWork_StateStepIncrement(0);
            }
            break;

        case 5:
            Event_WaitTimer(Q12(0.7f), false);
            break;

        case 6:
            Sfx_WithFlagsPlay(Sfx_Unk1343, &Q12_VECTOR3(60.0f, -1.2f, -96.0f), Q8(0.5f), SfxFlag_None);
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;

        case 7:
            Sfx_WithFlagsPlay(Sfx_DoorLocked, &Q12_VECTOR3(60.0f, -1.2f, -96.0f), Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 8:
            Event_DisplayMapMsg(false, 12, 0, 0, 0, false);
            break;

        case 9:
            for (i = 0; i < 5; i++)
            {
                if (Savegame_EventFlagGet(EventFlag_540 + i))
                {
                    break;
                }
            }

            if (i != 5)
            {
                Player_ControlUnfreeze(false);
                SysWork_StateSetNext(SysState_Gameplay);
            }
            else
            {
                SysWork_StateStepIncrement(0);
            }
            break;

        case 10:
            Event_BgTextureCmd(BgTextureCmd_QueueRead, FILE_TIM_LASTDOOR_TIM, false);
            SysWork_StateStepIncrement(0);

        case 11:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 12:
            Event_BgTextureCmd(BgTextureCmd_AwaitLoad, 0, false);
            break;

        case 13:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.0f), false);
            break;

        case 14:
            Event_DisplayMapMsg(false, MAP_MSG(130), 0, 0, 0, false);
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            break;

        case 15:
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

void func_800E32E0(void) // 0x800E32E0
{
    s32 i;
    s32 j;
    s32 temp;
    s16 cursorX;

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 0, Q12(0.0f), false);

            sharedData_800E2CAC_7_s01 = 0;
            sharedData_800E2CA8_7_s01 = 0;

            for (i = 0; i < ARRAY_SIZE(D_800EA4AC); i++)
            {
                D_800EA4AC[i] = 26;
            }

            SysWork_StateStepIncrement(0);

        case 1:
            Event_BgTextureCmd(BgTextureCmd_Auto, FILE_TIM_ALERTDOR_TIM, false);
            break;

        case 2:
            Event_ScreenFadeCmd(ScreenFadeCmd_Wait, true, 0, Q12(0.0f), false);
            break;

        case 3:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, false, false, false);
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            break;

        case 4:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);

            sharedData_800E2CA8_7_s01 += (g_Controller0->normalizedSticks.sticks_0.leftX * 16384) / 75;
            sharedData_800E2CA8_7_s01  = CLAMP_RANGE(sharedData_800E2CA8_7_s01, Q12(-120.0f), Q12(120.0f));

            sharedData_800E2CAC_7_s01 += (g_Controller0->normalizedSticks.sticks_0.leftY * 16384) / 75;
            sharedData_800E2CAC_7_s01  = CLAMP_RANGE(sharedData_800E2CAC_7_s01, Q12(-120.0f), Q12(120.0f));

            Game_TimerUpdate();

            cursorX = FP_FROM(sharedData_800E2CA8_7_s01, Q12_SHIFT) + 8;
            Gfx_CursorDraw(cursorX, FP_FROM(sharedData_800E2CAC_7_s01, Q12_SHIFT) + 8, 8, 8, 0, 64, 32, 32, 128, 192, 0, 12);

            if (Pad_ButtonClickedBind(cancel))
            {
                SysWork_StateStepSet(0, 8);
                break;
            }

            if (Pad_ButtonClickedBind(enter))
            {
                for (i = 0; i < ARRAY_SIZE(D_800E9DE8); i++)
                {
                    if ((D_800E9DE8[i][0] - 174) > FP_FROM(sharedData_800E2CA8_7_s01, Q12_SHIFT) ||
                        (D_800E9DE8[i][0] - 146) < FP_FROM(sharedData_800E2CA8_7_s01, Q12_SHIFT) ||
                        (D_800E9DE8[i][1] - 134) > FP_FROM(sharedData_800E2CAC_7_s01, Q12_SHIFT) ||
                        (D_800E9DE8[i][1] - 106) < FP_FROM(sharedData_800E2CAC_7_s01, Q12_SHIFT))
                    {
                        continue;
                    }

                    temp = D_800E9DE8[i][0] - 160 - FP_FROM(sharedData_800E2CA8_7_s01, Q12_SHIFT);
                    j    = SQUARE(temp);
                    temp = SQUARE(D_800E9DE8[i][1] - 120 - FP_FROM(sharedData_800E2CAC_7_s01, Q12_SHIFT));

                    if (j + temp >= 197)
                    {
                        continue;
                    }

                    SD_Call(Sfx_Unk1650);

                    for (j = 0; j < (ARRAY_SIZE(D_800EA4AC) - 1); j++)
                    {
                        D_800EA4AC[j] = D_800EA4AC[j + 1];
                    }

                    D_800EA4AC[4] = i;

                    for (j = 0; j < ARRAY_SIZE(D_800EA4AC); j++)
                    {
                        if (D_800EA4AC[j] != D_800E9E1C[j])
                        {
                            break;
                        }
                    }

                    if (j == ARRAY_SIZE(D_800EA4AC))
                    {
                        Savegame_EventFlagSet(EventFlag_488);

                        SysWork_StateStepSet(0, 5);
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
            SysWork_StateStepIncrement(0);

        case 7:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_WaitTimer(Q12(0.6f), false);
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

void Map_WorldObjectsInit(void) // 0x800E3804
{
    char beanObjName[] = "BEAN0_HI\0";
    s32  i;

    D_800EBB94 = 0;

    WorldObject_Init(&g_WorldObject_Diary, "DIARY_HI", -180.0f, 0.0f, -140.755f, 0.0f, 180.0f, 0.0f);
    WorldObject_ModelNameSet(&g_WorldObject_SavePad, g_CommonWorldObjectNames[1]);
    WorldObject_PoseInit(&g_WorldObject_UnkPose0, -16.6226f, -1.2396f, -59.0691f, 0.0f, 105.21f, 0.0f);
    WorldObject_PoseInit(&g_WorldObject_UnkPose1, 58.9993f, -0.82f, -99.0521f, 0.0f, -86.1f, 0.0f);
    WorldObject_ModelNameSet(&g_WorldObject_KaigaR[0], "KAIGAR_H");
    WorldObject_ModelNameSet(&g_WorldObject_KaigaR[1], "KAIGAR2_");
    Math_Vector3Set(&g_WorldObject_UnkPos0, Q12(56.0f), Q12(-1.9f), Q12(-19.25f));
    WorldObject_ModelNameSet(&g_WorldObject_KaigaL[0], "KAIGAL_H");
    WorldObject_ModelNameSet(&g_WorldObject_KaigaL[1], "KAIGAL2_");

    Math_Vector3Set(&g_WorldObject_UnkPos1, Q12(56.0f), Q12(-1.9f), Q12(-21.45f));
    Math_SetSVectorFast(&g_WorldObject_UnkRot0, Q12_ANGLE(0.0f), Q12_ANGLE(180.0f), Q12_ANGLE(0.0f));

    WorldObject_Init(&g_WorldObject_Bed3, "BED3_HID", 139.6f, 0.0f, -59.6f, 0.0f, 90.0f, 0.0f);
    WorldObject_PlacementInit(&g_WorldObject_Cover, "COVER_HI", 139.71f, 0.3f, -59.55f);
    WorldObject_PlacementInit(&g_WorldObject_Ank, "ANK_HIDE", 21.5767f, -1.3956f, -100.8592f);
    WorldObject_PlacementInit(&g_WorldObject_Ref[0], "REF1_HID", 20.3f, 0.0f, -60.75f);
    WorldObject_PlacementInit(&g_WorldObject_Ref[1], "REF2_HID", 20.3f, 0.0f, -60.75f);
    WorldObject_Init(&g_WorldObject_Dor[0], "DOR1_HID", 21.15f, -0.8f, -60.0f, 0.0f, 0.0f, 0.0f);
    WorldObject_Init(&g_WorldObject_Dor[1], "DOR2_HID", 21.15f, -0.1f, -60.0f, 0.0f, 0.0f, 0.0f);
    WorldObject_Init(&g_WorldObject_Dor[2], "DOR3_HID", 21.15f, -0.1f, -61.5f, 0.0f, 0.0f, 0.0f);
    WorldObject_Init(&g_WorldObject_Dor[3], "DOR4_HID", 21.15f, -0.8f, -61.5f, 0.0f, 0.0f, 0.0f);
    WorldObject_PlacementInit(&g_WorldObject_Chain1[0], "CHAIN1_N", 20.3f, 0.31f, -60.75f);
    WorldObject_PlacementInit(&g_WorldObject_Chain1[1], "CHAIN1_H", 20.3f, 0.0206f, -60.75f);
    WorldObject_PlacementInit(&g_WorldObject_Sword, "SWORD_HI", 20.3f, 0.0f, -60.75f);
    WorldObject_Init(&g_WorldObject_Jelly, "JELLY_NE", 62.55f, -0.9f, -61.05f, 0.0f, 90.0f, 0.0f);

    for (i = 0; i < ARRAY_SIZE(g_WorldObject_Beans); i++)
    {
        beanObjName[4] = (i >> 1) + '1';
        WorldObject_ModelNameSet(&g_WorldObject_Beans[i], beanObjName);
        g_WorldObject_Beans[i].position.vx = Q12(62.3f);
        g_WorldObject_Beans[i].position.vy = Rng_GenerateInt(Q12(-1.45f), Q12(-1.3501f) - 1);
        g_WorldObject_Beans[i].position.vz = Rng_GenerateInt(Q12(-61.07f), Q12(-60.93f) - 1);
        g_WorldObject_Beans[i].rotation.vx = Rng_GenerateUInt(0, 4095);
        g_WorldObject_Beans[i].rotation.vy = Rng_GenerateUInt(0, 4095);
        g_WorldObject_Beans[i].rotation.vz = Rng_GenerateUInt(0, 4095);
        D_800EB9F4[i]                         = Rng_GenerateUInt(0, 4095);
        D_800EBA14[i]                         = FP_FROM(Math_Sin(Rng_GenerateUInt(0, 511) | 0x200) * Q12(2.2f), Q12_SHIFT);
    }

    WorldObject_Init(&g_WorldObject_Key0, "KEY_HIDE", 62.2f, 0.0f, -61.1f, 0.0f, 0.0f, 90.0f);
    WorldObject_PlacementInit(&g_WorldObject_Wall9, "WALL9_HI", 103.9f, 0.0f, -99.5f);
    WorldObject_PlacementInit(&g_WorldObject_Door9, "DOOR9_HI", 104.0f, -1.6f, -100.2f);
    WorldObject_PlacementInit(&g_WorldObject_Gofu, "GOFU_HID", 17.072f, -1.6f, -19.2f);
    WorldObject_PlacementInit(&g_WorldObject_Driver, "DRIVER_H", -19.2f, 0.0f, -97.45f);
    WorldObject_PlacementInit(&g_WorldObject_Zukan, "ZUKAN_HI", 99.8f, 0.0f, -20.0f);
    WorldObject_PlacementInit(&g_WorldObject_Scrap, "SCRAP_HI", 100.0f, 0.0f, -20.0f);

    WorldObject_ModelNameSet(&g_WorldObject_Key1, "KEY_HIDE");
    WorldObject_ModelNameSet(&g_WorldObject_Futa[0], "FUTA1_HI");
    WorldObject_ModelNameSet(&g_WorldObject_Futa[1], "FUTA2_HI");

    WorldObject_PoseInit(&g_WorldObject_UnkPose2, -60.0f, 0.0f, -140.8f, 0.0f, 180.0f, 0.0f);
    WorldObject_Init(&g_WorldObject_Camera, "CAMERA_H", -19.6f, -0.8f, -142.2f, 0.0f, 126.7f, 0.0f);
    WorldObject_Init(&g_WorldObject_Door, "DOOR_HID", 182.5247f, 0.0f, -106.064f, 0.0f, 0.0f, 0.0f);
    WorldObject_Init(&g_WorldObject_Ring, "RING_NEA", -178.381f, -0.5716f, -98.846f, 0.0f, 0.0f, -32.5f);
    WorldObject_Init(&g_WorldObject_Gofu_B, "GOFU_B_H", -181.9f, -0.57f, -101.2f, 0.0f, 59.0f, 0.0f);
    WorldObject_PlacementInit(&g_WorldObject_Dr[0], "DR1L_HID", -100.9f, 0.0f, -60.9f);
    WorldObject_PlacementInit(&g_WorldObject_Dr[1], "DR1R_HID", -100.9f, 0.0f, -60.9f);
    WorldObject_PlacementInit(&g_WorldObject_Dr[2], "DR3L_HID", -100.9f, 0.0f, -60.9f);
    WorldObject_PlacementInit(&g_WorldObject_Dr[3], "DR3R_HID", -100.9f, 0.0f, -60.9f);
    WorldObject_PlacementInit(&g_WorldObject_Dr[4], "DR2L_HID", -100.9f, 0.0f, -60.9f);
    WorldObject_PlacementInit(&g_WorldObject_Dr[5], "DR2R_HID", -100.9f, 0.0f, -60.9f);
    WorldObject_PlacementInit(&g_WorldObject_Mov1, "MOV1_HID", -21.7317f, -0.387f, -57.2073f);
    WorldObject_PlacementInit(&g_WorldObject_Key2, "KEY_HIDE", 140.6558f, -0.81f, -58.937f);
    WorldObject_Init(&g_WorldObject_GofuA[0], "GOFUA_HI", 59.7776f, -1.8433f, -96.1695f, 0.0f, 90.0f, 0.0f);
    WorldObject_Init(&g_WorldObject_GofuA[1], "GOFUB_HI", 59.77f, -1.2757f, -95.924f, 90.0f, 180.0f, 0.0f);
    WorldObject_Init(&g_WorldObject_GofuA[2], "GOFUC_HI", 60.2471f, -1.8104f, -95.917f, 0.0f, 90.0f, 0.0f);
    WorldObject_Init(&g_WorldObject_GofuA[3], "GOFUD_HI", 59.9993f, -1.5943f, -95.9087f, 90.0f, 0.0f, 0.0f);
    WorldObject_Init(&g_WorldObject_GofuA[4], "GOFUE_HI", 60.2496f, -1.2691f, -95.9105f, 90.0f, 0.0f, 0.0f);
    WorldObject_Init(&g_WorldObject_KeyX2, "KEYX2_HI", -16.6692f, -1.23f, -61.6f, 90.9f, 90.0f, 0.0f);
    WorldObject_Init(&g_WorldObject_Stone, "STONE_HI", -16.6692f, -0.71f, -61.533f, 0.0f, 90.0f, 0.0f);
    WorldObject_Init(&g_WorldObject_Window, "WINDOW_H", -16.666f, -1.235f, -61.503f, 0.0f, 90.0f, 0.0f);
    WorldObject_PlacementInit(&g_WorldObject_Kubomi, "KUBOMI_H", -139.5f, 0.0f, -138.1f);

    if (g_SavegamePtr->gameDifficulty == GameDifficulty_Easy)
    {
        Chara_SpawnFlagsSet(Chara_Stalker, 5, CharaSpawnFlag_None);
        Chara_SpawnFlagsSet(Chara_Stalker, 7, CharaSpawnFlag_None);
        Chara_SpawnFlagsSet(Chara_Stalker, 8, CharaSpawnFlag_None);
    }
    if (g_SavegamePtr->gameDifficulty == GameDifficulty_Hard)
    {
        Chara_SpawnFlagsSet(Chara_Stalker, 9, CharaSpawnFlag_0 | CharaSpawnFlag_1 | CharaSpawnFlag_3);
        Chara_SpawnFlagsSet(Chara_Stalker, 12, CharaSpawnFlag_0 | CharaSpawnFlag_1);
        Chara_SpawnFlagsSet(Chara_Stalker, 13, CharaSpawnFlag_0 | CharaSpawnFlag_1);
        Chara_SpawnFlagsSet(Chara_Stalker, 14, CharaSpawnFlag_0 | CharaSpawnFlag_1);
    }

    WorldObject_ModelNameSet(&g_CommonWorldObjects[0], g_CommonWorldObjectNames[2]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[1], g_CommonWorldObjectNames[3]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[2], g_CommonWorldObjectNames[4]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[3], g_CommonWorldObjectNames[5]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[4], g_CommonWorldObjectNames[6]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[5], g_CommonWorldObjectNames[7]);
}

static const VECTOR3 D_800CDC4C = { Q12(-63.3f), Q12(-1.6f), Q12(-139.2f) };
static const VECTOR3 D_800CDC58 = { Q12(140.0f), Q12(-1.2f), Q12(-23.5f) };

void Map_WorldObjectsUpdate(void) // 0x800E4528
{
    s32 cellZ0;
    s32 cellX0;
    s32 i;
    s32 projCellZ0;
    s32 projCellX0;

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

        case CELL_XZ(18, 15):
            WorldObjects_Add(&g_WorldObject_KaigaR[Savegame_EventFlagGet(EventFlag_498) ? 1 : 0], &g_WorldObject_UnkPos0, &g_WorldObject_UnkRot0);
            WorldObjects_Add(&g_WorldObject_KaigaL[Savegame_EventFlagGet(EventFlag_499) ? 1 : 0], &g_WorldObject_UnkPos1, &g_WorldObject_UnkRot0);
            break;

        case CELL_XZ(15, 14):
            WorldObjects_Add(&g_WorldObject_Stone.object, &g_WorldObject_Stone.position, &g_WorldObject_Stone.rotation);
            WorldObjects_Add(&g_WorldObject_Window.object, &g_WorldObject_Window.position, &g_WorldObject_Window.rotation);
            WorldObjects_Add(&g_WorldObject_SavePad, &g_WorldObject_UnkPose0.position, &g_WorldObject_UnkPose0.rotation);
            WorldObjects_Add(&g_WorldObject_KeyX2.object, &g_WorldObject_KeyX2.position, &g_WorldObject_KeyX2.rotation);
            break;

        case CELL_XZ(20, 14):
            if (Savegame_EventFlagGet(EventFlag_551))
            {
                WorldObjects_Add(&g_WorldObject_Bed3.object, &g_WorldObject_Bed3.position, &g_WorldObject_Bed3.rotation);
                if (!Savegame_EventFlagGet(EventFlag_M7S02_PickupDiskOfOuroboros))
                {
                    WorldObjects_Add(&g_WorldObject_Key2.object, &g_WorldObject_Key2.position, &SVECTOR3_Zero);
                }
            }
            break;

        case CELL_XZ(17, 13):
            if (!Savegame_EventFlagGet(EventFlag_M7S02_PickupAnkh))
            {
                WorldObjects_Add(&g_WorldObject_Ank.object, &g_WorldObject_Ank.position, &SVECTOR3_Zero);
            }
            break;

        case CELL_XZ(17, 14):
            if (Savegame_EventFlagGet(EventFlag_557))
            {
                WorldObjects_Add(&g_WorldObject_Ref[1].object, &g_WorldObject_Ref[1].position, &SVECTOR3_Zero);
                for (i = 0; i < 4; i++)
                {
                    WorldObjects_Add(&g_WorldObject_Dor[i].object, &g_WorldObject_Dor[i].position, &g_WorldObject_Dor[i].rotation);
                }
            }
            else
            {
                WorldObjects_Add(&g_WorldObject_Ref->object, &g_WorldObject_Ref->position, &SVECTOR3_Zero);
                if (!Savegame_EventFlagGet(EventFlag_M7S02_PickupDaggerOfMelchior))
                {
                    WorldObjects_Add(&g_WorldObject_Sword.object, &g_WorldObject_Sword.position, &SVECTOR3_Zero);
                }
            }

            if (Savegame_EventFlagGet(EventFlag_552))
            {
                WorldObjects_Add(&g_WorldObject_Chain1[1].object, &g_WorldObject_Chain1[1].position, &SVECTOR3_Zero);
            }
            else
            {
                WorldObjects_Add(&g_WorldObject_Chain1[0].object, &g_WorldObject_Chain1[0].position, &SVECTOR3_Zero);
            }

            if (Savegame_EventFlagGet(EventFlag_M7S02_PickupDaggerOfMelchior))
            {
                if (Savegame_EventFlagGet(EventFlag_552))
                {
                    Savegame_EventFlagSet(EventFlag_554);
                    Savegame_EventFlagClear(EventFlag_553);
                }
                else
                {
                    Savegame_EventFlagSet(EventFlag_553);
                    Savegame_EventFlagClear(EventFlag_554);
                }
            }
            else
            {
                Savegame_EventFlagClear(EventFlag_553);
                Savegame_EventFlagClear(EventFlag_554);
            }
            break;

        case CELL_XZ(18, 14):
            if (!Savegame_EventFlagGet(EventFlag_556))
            {
                WorldObjects_Add(&g_WorldObject_Jelly.object, &g_WorldObject_Jelly.position, &g_WorldObject_Jelly.rotation);
            }
            else if (!Savegame_EventFlagGet(EventFlag_M7S02_PickupKeyOfBethor))
            {
                WorldObjects_Add(&g_WorldObject_Key0.object, &g_WorldObject_Key0.position, &g_WorldObject_Key0.rotation);
            }
            break;

        case CELL_XZ(19, 13):
            if (Savegame_EventFlagGet(EventFlag_560))
            {
                WorldObjects_Add(&g_WorldObject_Wall9.object, &g_WorldObject_Wall9.position, &SVECTOR3_Zero);
                WorldObjects_Add(&g_WorldObject_Door9.object, &g_WorldObject_Door9.position, &SVECTOR3_Zero);
            }
            break;

        case CELL_XZ(17, 15):
            if (!Savegame_EventFlagGet(EventFlag_M7S01_PickupAmuletOfSolomon))
            {
                WorldObjects_Add(&g_WorldObject_Gofu.object, &g_WorldObject_Gofu.position, &SVECTOR3_Zero);
            }
            break;

        case CELL_XZ(15, 13):
            if (!Savegame_EventFlagGet(EventFlag_M7S01_PickupScrewdriver))
            {
                WorldObjects_Add(&g_WorldObject_Driver.object, &g_WorldObject_Driver.position, &SVECTOR3_Zero);
            }
            break;

        case CELL_XZ(19, 15):
            WorldObjects_Add(&g_WorldObject_Zukan.object, &g_WorldObject_Zukan.position, &SVECTOR3_Zero);
            if (Savegame_EventFlagGet(EventFlag_M5S03_SeenSecondNewspaper))
            {
                WorldObjects_Add(&g_WorldObject_Scrap.object, &g_WorldObject_Scrap.position, &SVECTOR3_Zero);
            }
            break;

        case CELL_XZ(14, 12):
            if (Savegame_EventFlagGet(EventFlag_486))
            {
                if (!Savegame_EventFlagGet(EventFlag_M7S02_PickupKeyOfAratron))
                {
                    WorldObjects_Add(&g_WorldObject_Key1, &g_WorldObject_UnkPose2.position, &g_WorldObject_UnkPose2.rotation);
                }
                WorldObjects_Add(&g_WorldObject_Futa[1], &g_WorldObject_UnkPose2.position, &g_WorldObject_UnkPose2.rotation);
            }
            else
            {
                WorldObjects_Add(&g_WorldObject_Futa[0], &g_WorldObject_UnkPose2.position, &g_WorldObject_UnkPose2.rotation);
            }

            if (Savegame_EventFlagGet(EventFlag_486) && !Savegame_EventFlagGet(EventFlag_549))
            {
                if (D_800EBB94 != Q12(0.0f))
                {
                    D_800EBB94 -= g_DeltaTime;
                    if (D_800EBB94 < Q12(0.0f))
                    {
                        D_800EBB94 = Q12(0.0f);
                    }
                }
                else
                {
                    Sfx_WithFlagsPlay(Sfx_Unk1653, &D_800CDC4C, Q8(0.125f), SfxFlag_None);
                    D_800EBB94 = (Rng_Rand16() % Q12(2.0f)) + Q12(0.2f);
                }
            }
            break;

        case CELL_XZ(12, 12):
            WorldObjects_Add(&g_WorldObject_Kubomi.object, &g_WorldObject_Kubomi.position, &SVECTOR3_Zero);
            break;

        case CELL_XZ(15, 12):
            if (!Savegame_EventFlagGet(EventFlag_M7S01_PickupCamera))
            {
                WorldObjects_Add(&g_WorldObject_Camera.object, &g_WorldObject_Camera.position, &g_WorldObject_Camera.rotation);
            }
            break;

        case CELL_XZ(21, 13):
            WorldObjects_Add(&g_WorldObject_Door.object, &g_WorldObject_Door.position, &g_WorldObject_Door.rotation);
            break;

        case CELL_XZ(15, 15):
        case CELL_XZ(13, 14):
            for (i = 0; i < 6; i++)
            {
                WorldObjects_Add(&g_WorldObject_Dr[i].object, &g_WorldObject_Dr[i].position, &SVECTOR3_Zero);
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

        case CELL_XZ(18, 13):
            if (Savegame_EventFlagGet(EventFlag_540))
            {
                WorldObjects_Add(&g_WorldObject_GofuA[0].object, &g_WorldObject_GofuA[0].position, &g_WorldObject_GofuA->rotation);
            }

            if (Savegame_EventFlagGet(EventFlag_541))
            {
                WorldObjects_Add(&g_WorldObject_GofuA[1].object, &g_WorldObject_GofuA[1].position, &g_WorldObject_GofuA[1].rotation);
            }

            if (Savegame_EventFlagGet(EventFlag_542))
            {
                WorldObjects_Add(&g_WorldObject_GofuA[2].object, &g_WorldObject_GofuA[2].position, &g_WorldObject_GofuA[2].rotation);
            }

            if (Savegame_EventFlagGet(EventFlag_543))
            {
                WorldObjects_Add(&g_WorldObject_GofuA[3].object, &g_WorldObject_GofuA[3].position, &g_WorldObject_GofuA[3].rotation);
            }

            if (Savegame_EventFlagGet(EventFlag_544))
            {
                WorldObjects_Add(&g_WorldObject_GofuA[4].object, &g_WorldObject_GofuA[4].position, &g_WorldObject_GofuA[4].rotation);
            }

            WorldObjects_Add(&g_WorldObject_SavePad, &g_WorldObject_UnkPose1.position, &g_WorldObject_UnkPose1.rotation);
            break;

        case CELL_XZ(12, 15):
            if (!Savegame_EventFlagGet(EventFlag_478) && g_SysWork.playerWork.player.position.vz < Q12(-19.0f))
            {
                Savegame_EventFlagSet(EventFlag_478);
                Sfx_WithFlagsPlay(Sfx_Unk1528, &D_800CDC58, Q8_CLAMPED(1.0f), SfxFlag_NoDistAtten);
            }
            break;
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, 5, -1, 5) && PLAYER_IN_MAP_CHUNK(vz, 1, -2, -1, -2))
    {
        if (!Savegame_EventFlagGet(EventFlag_549))
        {
            if (!D_800EBAF4)
            {
                SD_Call(Sfx_Unk1503);
                D_800EBAF4++;
            }
            else if (D_800EBAF4 == 1)
            {
                Sfx_WithFalloffAndPitchPlay(Sfx_Unk1495, &D_800E9D00, Q8(0.5f), Q12(16.0f), 0);
                Sfx_WithFalloffAndPitchPlay(Sfx_Unk1503, &D_800E9D00, Q8(0.5f), Q12(16.0f), 0);
            }
        }
    }
    else
    {
        if (D_800EBAF4 == 1)
        {
            Sd_SfxStop(Sfx_Unk1495);
            Sd_SfxStop(Sfx_Unk1503);

            D_800EBAF4 = 0;
        }
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
            WorldObjects_Add(g_CommonWorldObjects, &g_CommonWorldObjectPoses[5].position, &g_CommonWorldObjectPoses[5].rotation);
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

void func_800E5628(void) // 0x800E5628
{
    if (D_800A9945 != 7 || !Chara_ModelLoadedCheck(Chara_Stalker))
    {
        Anim_CharaTypeAnimInfoClear();
        Chara_Load(0, 7, g_SysWork.npcBoneCoordBuffer, NO_VALUE, NULL, NULL);
        Chara_ProcessLoads();
    }

    if (Savegame_EventFlagGet(EventFlag_M7S02_PickupKeyOfAratron))
    {
        Chara_SpawnFlagsSet(Chara_Stalker, 3, CharaSpawnFlag_0 | CharaSpawnFlag_1);

        if (g_SavegamePtr->gameDifficulty == GameDifficulty_Hard)
        {
            Chara_SpawnFlagsSet(Chara_Stalker, 10, CharaSpawnFlag_0 | CharaSpawnFlag_1 | CharaSpawnFlag_3);
            Chara_SpawnFlagsSet(Chara_Stalker, 10, CharaSpawnFlag_0 | CharaSpawnFlag_1);
        }
    }

    if (Savegame_EventFlagGet(EventFlag_549))
    {
        Savegame_EventFlagClear(EventFlag_420);
    }
    else
    {
        Savegame_EventFlagSet(EventFlag_420);
    }
}
