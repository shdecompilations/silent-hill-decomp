#include "bodyprog/bodyprog.h"
#include "bodyprog/chara/dms.h"
#include "bodyprog/events/event_data.h"
#include "bodyprog/gfx/map_effects.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player/player.h"
#include "main/rng.h"
#include "maps/map4/map4_s01.h"
#include "maps/common/characters/cybil.h"
#include "maps/common/particle.h"
#include "maps/common/characters/player.h"

#include "../src/maps/common/unk_draw.c"

#include "../src/maps/common/particles/particle.c"

#include "../src/maps/common/characters/player.c"

#include "../src/maps/common/characters/cybil.c"

#include "../src/maps/common/chara_util.c" // 0x800D10C0

#include "../src/maps/common/map_util.c" // 0x800D1AD0

#include "maps/common/shared/Map_RoomBgmInit_CheckCond.h" // 0x800D1B78

#include "maps/common/shared/Map_RoomBgmInit_0_s02_CondTrue.h" // 0x800D1BAC

#include "maps/common/shared/Map_RoomBgmInit_0_s02_CondFalse.h" // 0x800D1C28

void GameBoot_LoadScreen_StageString(void) {}

#include "maps/common/shared/Event_CutsceneTimerAdvance.h" // 0x800D1EBC

void MapEvent_DoorJammed(void) // 0x800D1F54
{
    VECTOR3 sfxPos = { MAP_POINTS[g_MapEventData->mapPointIdx].positionX, Q12(-1.2f), MAP_POINTS[g_MapEventData->mapPointIdx].positionZ };

    Event_DisplayMapMsgWithSfx(MapMsgIdx_DoorJammed, Sfx_Unk1541, &sfxPos);
}

void func_800D1FE8(void) {}

const char* MAP_MESSAGES[] = {
    #include "maps/common/shared/map_msg_common.h"
#if VERSION_REGION_IS(NTSC)
    /* 15  */ "~J0(1.6)\tWhat's_this? ~E ",
    /* 16  */ "~J0(1.0)\tHarry! ",
    /* 17  */ "~J0(1.0)\tCybil? ~E ",
    /* 18  */ "~J0(2.8)\tI'm_glad_you're_OK. ",
    /* 19  */ "~J1(4.0)\tI_shouldn't_have_left_you. ~N\n\t\t\tThings_are_worse_than_I_thought. ",
    /* 20  */ "~J1(1.4)\tIt's_nuts! ~E ",
    /* 21  */ "~J1(3.0)\tWhat_are_you_doing_here? ~N\n\t\t\tI_thought_you_left_town. ",
    /* 22  */ "~J1(2.8)\tI_saw_you_go_in_here, ~N\n\t\t\tso_I_followed_you. ",
    /* 23  */ "~J1(1.8)\tI_couldn't_get_out. ",
    /* 24  */ "~J1(2.0)\tAll_the_roads_out_of_town ~N\n\t\t\tare_blocked. ",
    /* 25  */ "~J1(2.2)\tCars_have_completely ~N\n\t\t\tstopped_running. ",
    /* 26  */ "~J1(2.4)\tThe_phones_and_radios ~N\n\t\t\tare_still_out,_too. ~E ",
    /* 27  */ "~J1(2.7)\tWhat_about_my_daughter? ~N\n\t\t\tDid_you_see_her? ",
    /* 28  */ "~J1(1.9)\tI_did_see_a_girl. ",
    /* 29  */ "~J1(1.4)\tWas_it_Cheryl? ",
    /* 30  */ "~J1(2.6)\tI_only_caught_a_glimpse_of_her ~N\n\t\t\tthrough_the_fog. ",
    /* 31  */ "~J1(2.4)\tI_went_after_her, ~N\n\t\t\tbut_she_vanished. ",
    /* 32  */ "~J1(2.7)\tI_don't_know_about ~N\n\t\t\tyour_daughter,_but... ~E ",
    /* 33  */ "~J1(3.5)\tAnd_you_just_let_her_go!? ~N\n\t\t\tWhere_was_it? ",
    /* 34  */ "~J1(3.4)\tOn_Bachman_Road. ~N\n\t\t\tShe_was_heading_towards_the_lake. ",
    /* 35  */ "~J1(2.0)\tNow_don't_get_excited. ",
    /* 36  */ "~J1(2.5)\tIt_wasn't_like_she_ran_off ~N\n\t\t\texactly. ",
    /* 37  */ "~J1(4.8)\tThere_was_no_place_for_her_to_go. ~N\n\t\t\tThe_road_has_been_obliterated. ~E ",
    /* 38  */ "~J1(3.0)\tWhat!? ~N\n \t\t\tSo_then_Cheryl... ",
    /* 39  */ "~J1(3.6)\tIt_was_like_she_was_walking ~N\n\t\t\ton_thin_air. ",
    /* 40  */ "~J1(2.0)\t~E ",
    /* 41  */ "~J1(3.0)\tWhat_about_you?__Anything? ",
    /* 42  */ "~J1(3.0)\tYeah... ~N\n\t\t\tI_met_this_bizarre_woman. ",
    /* 43  */ "~J1(4.0)\tHer_name's_Dahlia_Gillespie. ~N\n\t\t\tDo_you_know_her? ",
    /* 44  */ "~J1(1.8)\tDahlia_Gillespie? ",
    /* 45  */ "~J1(1.0)\tNo. ",
    /* 46  */ "~J1(1.2)\tAnd? ~E ",
    /* 47  */ "~J1(4.0)\tShe_said_something_about ~N\n\t\t\tthe_town_being_devoured ~N\n\t\t\tby_darkness.",
    /* 48  */ "~J1(1.8)\tGibberish_like_that. ",
    /* 49  */ "~J1(2.0)\tAny_idea_what_it_means? ~E ",
    /* 50  */ "~J1(2.9)\tDarkness_devouring_the_town? ",
    /* 51  */ "~J1(1.6)\tMust_be_on_drugs. ",
    /* 52  */ "~J1(2.0)\tThey_sell_'em_to_the_tourists. ",
    /* 53  */ "~J1(2.8)\tThe_force_still_can't_figure_out ~N\n\t\t\twho's_behind_it. ",
    /* 54  */ "~J1(2.2)\tNone_of_our_leads ~N\n\t\t\thave_panned_out, ",
    /* 55  */ "~J1(2.5)\tand_the_investigation_is_stalled. ",
    /* 56  */ "~J1(3.5)\tWhat_could_drug_trafficking ~N\n\t\t\thave_to_do_with_all_this? ",
    /* 57  */ "~J1(3.2)\tI_really_don't_know. ",
    /* 58  */ "~J1(3.6)\tBut,_maybe_that's_the_darkness ~N\n\t\t\tshe_was_talking_about. ",
    /* 59  */ "~J1(1.8)\tThat's_all_I_can_think_of. ",
    /* 60  */ "~J1(2.0)\t~E ",
    /* 61  */ "~J1(1.2)\tWhat's_this? ",
    /* 62  */ "~J1(1.5)\tJust_discovered_it. ",
    /* 63  */ "~J1(1.8)\tMaybe_there's_something ~N\n\t\t\tback_there. ",
    /* 64  */ "~J1(1.4)\tLet's_have_a_look. ",
    /* 65  */ "~J1(1.0)\tWait. ",
    /* 66  */ "~J1(4.0)\tWe_don't_know_what's_back_there. ~N\n\t\t\tI'd_better_check_it_out_first. ",
    /* 67  */ "~J1(2.2)\tI'm_a_cop.__I_should_go. ",
    /* 68  */ "~J1(3.0)\tNo.__I'm_going. ",
    /* 69  */ "~J1(0.9)\tAlright. ",
    /* 70  */ "~J1(1.5)\tI'll_cover_you_from_here. ",
    /* 71  */ "~J1(1.7)\tBe_careful. ",
    /* 72  */ "~J1(3.6)\tIf_anything_looks_fishy, ~N\n\t\t\tget_back_here_on_the_double. ~E ",
    /* 73  */ "~J0(0.8)  \tOkay. ~E ",
    /* 74  */ "~J0(1.0)\tCybil. ",
    /* 75  */ "~J0(0.9)\tYes? ~E ",
    /* 76  */ "~J1(2.2)\tDo_you_know_anything_about... ",
    /* 77  */ "~J1(6.2)\tWell...like_some_other_world... ~N\n\t\t\tIt's_like_some_kind_of_bad_dream...? ",
    /* 78  */ "~J1(1.9)\tWhat_are_you_talking_about? ",
    /* 79  */ "~J1(1.7)\tI'm_not_quite_sure. ",
    /* 80  */ "~J1(3.7)\tI_try_to_make_sense_of_it, ~N\n\t\t\tbut_then_my_mind_goes_blank. ",
    /* 81  */ "~J1(4.0)\tEverything's_dark_there, ~N\n\t\t\tand_I_hear_sirens_in_the_distance. ",
    /* 82  */ "~J1(2.7)\tI_met_this_nurse..._Lisa. ",
    /* 83  */ "~J1(3.7)\tIt's_like_I_was_there, ~N\n\t\t\tbut_not_really. ",
    /* 84  */ "~J1(4.2)\tIt's_all_a_blur. ~N\n\t\t\tLike_some_kind_of_hallucination, ~N\n\t\t\tyou_know? ",
    /* 85  */ "~J1(3.0)\tI_have_no_idea_what ~N\n\t\t\tyou're_talking_about,_Harry. ~E ",
    /* 86  */ "~J1(1.8)\tOh... ",
    /* 87  */ "~J1(4.5)\tI_was_just_wondering. ~N\n\t\t\tNever_mind. ",
    /* 88  */ "~J1(1.0)\tHarry. ",
    /* 89  */ "~J1(2.5)\tYou're_tired. ~E ",
    /* 90  */ "~J0(2.4)\tYeah,_maybe... ~E ",
    /* 91  */ "~J1(4.1)\tWhat's_this...? ~N\n\t\t\tSome_kind_of_altar...? ",
    /* 92  */ "~J1(2.6)\tNever_seen_anything ~N\n\t\t\tlike_this_before. ",
    /* 93  */ "~J1(2.5)\tMaybe_this_is ~N\n\t\t\t[the_other_church.] ~E ",
    /* 94  */ "\t\t\tSome_kind_of_powder ~N\n\t\t\tis_left_in_the_chalice. ~E ",
    /* 95  */ "~J0(1.5)\t",
    /* 96  */ "~J2(1.5)\tWhat_the...!? ~E ",
    /* 97  */ "~J0(4.2)\tHarry? ~N\n\t\t\tAre_you_OK? ~E ",
    /* 98  */ "~J0(1.0)\tHarry? ~E ",
    /* 99  */ "~J0(2.3)\tHarry? ~E ",
    /* 100 */ "\tThere_is_a_trace_on_the_floor ~N\n\tthat_somebody_moved_the_cabinet. ",
    /* 101 */ "\tDo_you_want_to_push ~N\n\tthe_cabinet? ~S4 ",
    /* 102 */ "~J0(2.0)\tWas_that_another_dream? ~E ",
    /* 103 */ "~J0(2.0)\tDid_I_pass_out_again? ~E ",
    /* 104 */ "~J0(2.0)\tI_don't_want_to_think_so, ",
    /* 105 */ "~J0(3.0)\tbut_maybe_this_is_all_just ~N\n\t\t\tgoing_on_in_my_head. ",
    /* 106 */ "~J0(4.0)\tI_could_have_had_a_car_accident, ~N\n\t\t\tand_now_I'm_lying_unconscious ~N\n\t\t\tin_a_hospital_bed...",
    /* 107 */ "~J0(2.0)\tI_don't_know ~N\n\t\t\twhat's_real_anymore... ~E ",
    /* 108 */ "~J0(1.0)\t_ ",
    /* 109 */ "~J0(3.0)\tCybil_said_Cheryl_was_heading ~N\n\t\t\ttowards_the_lake. ",
    /* 110 */ "~J0(2.2)\tBut,_the_road_to_the_lake ~N\n\t\t\tis_blocked... ",
    /* 111 */ "~J0(2.0)\tAren't_there_any_other_roads? ",
    /* 112 */ "~J0(2.0)\tLisa_would_probably_know... ~E ",
    /* 113 */ "\tThere_is_an_ ~C2 Axe ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 114 */ "\tThere_is_a_clock. ~N\n\tNothing_special. ~E ",
    /* 115 */ "\tNo_need_to_go_back. ~E ",
    /* 116 */ "\tNone_of_this_stuff_is_of_any_use. ~E ",
    /* 117 */ "\tLooks_like_an_altar. ~N\n\tWonder_what_they_worship? ~E "
#elif VERSION_REGION_IS(NTSCJ)
    /* 15 */ "~J0(1.6)\tこれは……？ ~E ",
    /* 16 */ "~J0(1.0)\tハリー！ ",
    /* 17 */ "~J0(1.0)\tシビル！？ ~E ",
    /* 18 */ "~J0(2.8)\t無事で良かった ",
    /* 19 */ "~J1(4.0)\t一人にしたのを後悔したわ ~N\n\t\t\t思ったよりもずっと悪い状況ね ",
    /* 20 */ "~J1(1.8)\t頭がおかしくなりそうよ ~E ",
    /* 21 */ "~J1(3.0)\tどうしてここに？ ~N\n\t\t\t町は出なかったのか？ ",
    /* 22 */ "~J1(2.8)\tあなたがこの店に入るのが ~N\n\t\t\t見えたから追いかけてきたのよ ",
    /* 23 */ "~J1(1.8)\tこの町からは出られなかった…… ",
    /* 24 */ "~J1(2.0)\t外へつながる道はすべてふさがれているわ ",
    /* 25 */ "~J1(2.2)\t車も全然動かないのよ ",
    /* 26 */ "~J1(2.4)\tそれにあいかわらず電話も無線も駄目 ~E ",
    /* 27 */ "~J1(2.7)\tシェリルは？ ~N\n\t\t\t見なかったか？ ",
    /* 28 */ "~J1(1.9)\t見たわ　女の子を ",
    /* 29 */ "~J1(1.4)\tシェリルなのか！？ ",
    /* 30 */ "~J1(2.6)\t霧のかなたにかすかに見えただけなのよ ",
    /* 31 */ "~J1(2.4)\t追いかけたけれど ~N\n\t\t\tいなくなってしまったわ ",
    /* 32 */ "~J1(2.7)\tあなたの娘かどうかはわからないけど…… ~E ",
    /* 33 */ "~J1(3.5)\tそれで放っておいたのか！？ ~N\n\t\t\tどこで見たんだ？ ",
    /* 34 */ "~J1(3.4)\tバックマン通り ~N\n\t\t\t湖の方へ続く道を歩いていったわ ",
    /* 35 */ "~J1(2.0)\tでもね　落ち着いて ",
    /* 36 */ "~J1(2.5)\t正確にはいなくなったわけじゃないの ",
    /* 37 */ "~J1(4.8)\tその子が行った先には道がなかったのよ ~N\n\t\t\t陥没で根こそぎ消えていたわ ~E ",
    /* 38 */ "~J1(3.0)\t何だって……？ ~N\n \t\t\tだったら　シェリルは…… ",
    /* 39 */ "~J1(3.6)\t……何もないところを歩いていったのよ ",
    /* 40 */ "~J1(2.0)\t~E ",
    /* 41 */ "~J1(3.0)\t……あなたの方は何かわかった？ ",
    /* 42 */ "~J1(3.0)\tああ…… ~N\n\t\t\t変な女に会ったんだ ",
    /* 43 */ "~J1(4.0)\tダリア・ギレスピーと名乗っていた ~N\n\t\t\t知ってるか？ ",
    /* 44 */ "~J1(1.8)\tダリア・ギレスピー？ ",
    /* 45 */ "~J1(1.0)\t知らないわ ",
    /* 46 */ "~J1(1.2)\tどうかしたの？ ~E ",
    /* 47 */ "~J1(4.0)\t彼女が言っていたんだ ~N\n\t\t\t町を闇が蝕んでいる…… ",
    /* 48 */ "~J1(1.8)\tそんなようなことを ",
    /* 49 */ "~J1(2.0)\t何のことだかわかるか？ ~E ",
    /* 50 */ "~J1(2.9)\t町を蝕む闇？ ",
    /* 51 */ "~J1(1.6)\tドラッグに決まってるわ ",
    /* 52 */ "~J1(2.0)\t観光客を中心に出回ってるのよ ",
    /* 53 */ "~J1(2.8)\tどういうルートで流れてくるのか ~N\n\t\t\tまだはっきりわからないの ",
    /* 54 */ "~J1(2.2)\tいろいろと問題があって…… ",
    /* 55 */ "~J1(2.5)\t捜査も思うように進まなくてね ",
    /* 56 */ "~J1(3.5)\tドラッグが今の状況と関係あるのか？ ",
    /* 57 */ "~J1(3.2)\tさあ　わからないわ ",
    /* 58 */ "~J1(3.6)\tただ　この町を蝕む闇なんて ~N\n\t\t\t言われたって ",
    /* 59 */ "~J1(1.8)\tあたしには他に考えられないわ ",
    /* 60 */ "~J1(2.0)\t~E ",
    /* 61 */ "~J1(1.2)\tこれは？ ",
    /* 62 */ "~J1(1.5)\t……さっき見つけたんだ ",
    /* 63 */ "~J1(1.8)\t奥に何かあるのかもしれない ",
    /* 64 */ "~J1(1.4)\t入ってみましょう ",
    /* 65 */ "~J1(1.0)\t待ってくれ ",
    /* 66 */ "~J1(4.0)\t何が起こるかわからない ~N\n\t\t\tまずはわたしが様子を見てくる ",
    /* 67 */ "~J1(2.2)\tそれならあたしが ",
    /* 68 */ "~J1(3.0)\tいや　わたしが行く ",
    /* 69 */ "~J1(0.9)\t……わかった ",
    /* 70 */ "~J1(1.5)\tあたしはここで待ってるわ ",
    /* 71 */ "~J1(1.7)\t気をつけて ",
    /* 72 */ "~J1(3.6)\t危険を感じたらすぐに戻るのよ ~E ",
    /* 73 */ "~J0(0.8)  \tああ ~E ",
    /* 74 */ "~J0(1.0)\tシビル ",
    /* 75 */ "~J0(0.9)\t何？ ~E ",
    /* 76 */ "~J1(2.2)\tきみは……わからないかな？ ",
    /* 77 */ "~J1(6.2)\tもう一つの世界というか…… ~N\n\t\t\tまるで悪夢の中のような…… ",
    /* 78 */ "~J1(1.9)\t何のこと？ ",
    /* 79 */ "~J1(1.7)\tはっきりとは言えないんだ ",
    /* 80 */ "~J1(3.7)\tしっかり覚えているつもりでいても ~N\n\t\t\tすぐに記憶から色あせてしまう ",
    /* 81 */ "~J1(4.0)\t真っ暗で　遠くでサイレンが鳴っていて…… ",
    /* 82 */ "~J1(2.7)\tリサという看護婦に出会って…… ",
    /* 83 */ "~J1(3.7)\t確かに見たはずなのに実感がまるでない…… ",
    /* 84 */ "~J1(4.2)\t今思い出そうとしてもぼんやりして ~N\n\t\t\t幻覚だったのかとも思えるんだ…… ",
    /* 85 */ "~J1(3.0)\t……わからないわ ~E ",
    /* 86 */ "~J1(1.8)\t……そうか ",
    /* 87 */ "~J1(4.5)\tいや　いいんだ ~N\n\t\t\t気にしないでくれ ",
    /* 88 */ "~J1(1.0)\tハリー ",
    /* 89 */ "~J1(2.5)\tあなた　少し疲れているんじゃない？ ~E ",
    /* 90 */ "~J0(2.4)\t……そうかもしれないな ~E ",
    /* 91 */ "~J1(4.1)\tこれは…… ~N\n\t\t\t祭壇なのか……？ ",
    /* 92 */ "~J1(2.6)\t見たことのない様式のものだな ",
    /* 93 */ "~J1(2.5)\tもう一つの教会とはこれのことか…… ~E ",
    /* 94 */ "\t\t\tカップの中に粉のようなものが残っている ~E ",
    /* 95 */ "~J0(1.5)\t",
    /* 96 */ "~J2(1.5)\t何だ！？ ~E ",
    /* 97 */ "~J0(4.2)\tハリー？　大丈夫？ ~E ",
    /* 98 */ "~J0(1.0)\tハリー？ ~E ",
    /* 99 */ "~J0(2.3)\tハリー……？ ~E ",
    /* 100 */ "\t床に棚を動かしたような跡があるな…… ",
    /* 101 */ "\t棚を動かしますか？ ~S4 ",
    /* 102 */ "~J0(2.0)\tこれも夢か？ ~E ",
    /* 103 */ "~J0(2.0)\tまた　気を失っていたのか？ ~E ",
    /* 104 */ "~J0(2.0)\t考えたくないが…… ",
    /* 105 */ "~J0(3.0)\tすべてはわたしの頭の中だけで ~N\n\t\t\t起こっているに過ぎないんじゃないか？ ",
    /* 106 */ "~J0(4.0)\t車で事故にあって　病院のベッドで ~N\n\t\t\t意識のないまま今も寝ているとか…… ",
    /* 107 */ "~J0(2.5)\t何が本当のことなのか ~N\n\t\t\tさっぱりわからない…… ~E ",
    /* 108 */ "~J0(1.0)\t　 ",
    /* 109 */ "~J0(2.8)\tシェリルは湖の方へ歩いていったと ~N\n\t\t\tシビルが言っていたな…… ",
    /* 110 */ "~J0(2.0)\tだが　湖に向かう道は通れない……か…… ",
    /* 111 */ "~J0(1.8)\t……他に道はないのか？ ",
    /* 112 */ "~J0(2.0)\tリサに聞けばわかるだろうか…… ~E ",
    /* 113 */ "\t~C2 斧 ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 114 */ "\t置き時計がある ~N\n\t特に気になることはない ~E ",
    /* 115 */ "\t戻る必要はない　奥を調べに行こう ~E ",
    /* 116 */ "\tいろいろなものが置かれているが ~N\n\t特に役に立ちそうなものはない ~E ",
    /* 117 */ "\t祭壇らしきものがある ~N\n\tこれは何を祭っているのだろうか？ ~E "
#endif
};

void func_800D1FF0(void) // 0x800D1FF0
{
    s32 tmp;

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            Event_DisplayMapMsg(true, 100, 3, -1, 0, false); // "Do you want to push the cabinet?"
            break;

        case 3:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 2, Q12(0.0f), false);
            SysWork_StateStepIncrement(0);

        case 4:
            Event_WaitTimer(Q12(1.5f), false);
            break;

        case 5:
            Event_CameraPositionSet(NULL, Q12(179.35f), Q12(-1.5f), Q12(61.0098f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(NULL, Q12(176.98f), Q12(-0.49f), Q12(64.07f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            SD_Call(Sfx_Unk1538);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 105, false);

            g_SysWork.playerWork.player.position.vx = Q12(178.3f);
            g_SysWork.playerWork.player.position.vz = Q12(61.9f);
            g_SysWork.playerWork.player.rotation.vy = Q12_ANGLE(0.0f);

            func_8003D03C();
            Player_EmptyWeaponHandSet();
            WorldGfx_PlayerHeldItemSet(InvItemId_Handgun);
            Chara_Load(0, Chara_Cybil, &g_SysWork.npcBoneCoordBuffer[0], CHARA_FORCE_FREE_ALL, NULL, NULL);
            Savegame_EventFlagSet(EventFlag_302);
            SysWork_StateStepIncrement(0);

        case 6:
            tmp = Q12_MULT_PRECISE(g_DeltaTime, Q12(0.22f));

            g_SysWork.playerWork.player.position.vz += tmp;
            WorldObject_D_800D7FF0.position.vz += tmp;

            D_800D5AE8 += Q12_MULT_PRECISE(g_DeltaTime, Q12(2.0f));
            if (D_800D5AE8 > Q12(1.0f))
            {
                D_800D5AE8 = Q12(1.0f);
            }

            if (WorldObject_D_800D7FF0.position.vz > Q12(63.6814f))
            {
                tmp = WorldObject_D_800D7FF0.position.vz - Q12(63.6814f);
                g_SysWork.playerWork.player.position.vz -= tmp;
                WorldObject_D_800D7FF0.position.vz -= tmp;

                SysWork_StateStepIncrement(0);
            }

            Sfx_WithFalloffAndPitchPlay(Sfx_Unk1538, &WorldObject_D_800D7FF0.position, D_800D5AE8 >> 5, Q12(12.0f), 0);
            break;

        case 7:
            D_800D5AE8 -= Q12_MULT_PRECISE(g_DeltaTime, Q12(2.0f));
            if (D_800D5AE8 < Q12(0.0f))
            {
                D_800D5AE8 = Q12(0.0f);
            }

            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(3.0f), false);
            Sfx_WithFalloffAndPitchPlay(Sfx_Unk1538, &WorldObject_D_800D7FF0.position, D_800D5AE8 >> 5, Q12(12.0f), 0);
            break;

        default:
            // Return to gameplay.
            if (Savegame_EventFlagGet(EventFlag_302))
            {
                Sd_SfxStop(Sfx_Unk1538);

                SysWork_StateSetNext(SysState_Gameplay);
                Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
                break;
            }

            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;
    }
}

void func_800D2408(void) // 0x800D2408
{
    #define soundPos Q12_VECTOR3(182.0f, -1.2f, 58.9f)

    typedef enum _EventState
    {
        EventState_Skip = 43
    } e_EventState;

    VECTOR3  lightIntPos;
    SVECTOR3 unused;

    #define playerChara &g_SysWork.playerWork.player
    #define cybilChara  &g_SysWork.npcs[0]

    // Skip.
    if (Pad_ButtonClickedBind(skip) &&
        g_SysWork.sysStateSteps[0] >= 3 && g_SysWork.sysStateSteps[0] < 41)
    {
        SysWork_StateStepSet(0, EventState_Skip);
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            g_Cutscene_MapMsgAudioIdx      = 0;
            g_Cutscene_UpdateSibyl = false;

            Fs_QueueStartRead(FILE_ANIM_AQSH1_DMS, FS_BUFFER_11);
            Fs_QueueWaitForEmpty();
            Dms_HeaderFixOffsets(FS_BUFFER_11);

            g_Cutscene_Timer0 = Q12(0.0f);

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, playerChara, 51, false);

            CutsceneBorder_ForceShow();
            g_SysWork.sysFlags           |= SysFlag_CutsceneActive;

            func_8008D438();

            g_SysWork.lightBoneCoord     = NULL;
            g_SysWork.lensFlareBoneCoord = NULL;
            g_SysWork.lightIntensity     = Q12(1.0f);

            SysWork_StateStepIncrement(0);

        case 1:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.0f), false);
            break;

        case 2:
            Chara_ProcessLoads();
            SysWork_StateStepIncrement(0);

        case 3:
            Event_DisplayMapMsgWithAudio(15, &g_Cutscene_MapMsgAudioIdx, g_Cutscene_MapMsgAudioCmds);
            break;

        case 4:
            Sfx_WithFlagsPlay(Sfx_DoorOpen0, &soundPos, Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 5:
            Event_WaitTimer(Q12(0.5f), false);
            break;

        case 6:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, playerChara, 118, false);
            Sfx_WithFlagsPlay(Sfx_DoorClose0, &soundPos, Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 7:
            Event_WaitTimer(Q12(1.0f), false);
            break;

        case 8:
            Chara_Spawn(Chara_Cybil, 0, Q12(183.0f), Q12(61.0f), 0, 3);
            g_Cutscene_UpdateSibyl = true;

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, cybilChara, 5, false);
            func_8003D01C();
            SysWork_StateStepIncrement(0);

        case 9:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AwaitAnimEnd, playerChara, 0, false);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer0, Q12(10.0f), Q12(0.0f), Q12(28.0f), true, false);
            break;

        case 10:
            Event_DisplayMapMsgWithAudio(16, &g_Cutscene_MapMsgAudioIdx, g_Cutscene_MapMsgAudioCmds);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer0, Q12(10.0f), Q12(0.0f), Q12(28.0f), true, false);
            break;

        case 11:
            Event_CharaAnimPlayToEnd(playerChara, 119);
            break;

        case 12:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, playerChara, 51, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, cybilChara, 1, false);
            func_8003D03C();
            Savegame_EventFlagSet(EventFlag_311);
            SysWork_StateStepIncrement(0);

        case 13:
            Event_DisplayMapMsgWithAudio(18, &g_Cutscene_MapMsgAudioIdx, g_Cutscene_MapMsgAudioCmds);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer0, Q12(16.0f), Q12(29.0f), Q12(56.0f), true, true);
            break;

        case 14:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, cybilChara, 5, false);

            g_Cutscene_Timer0        = Q12(57.0f);
            g_SysWork.lightIntensity = Q12(0.7f);

            SysWork_StateStepIncrement(0);

        case 15:
            Event_DisplayMapMsgWithAudio(18, &g_Cutscene_MapMsgAudioIdx, g_Cutscene_MapMsgAudioCmds);
            break;

        case 16:
            Event_DisplayMapMsgWithAudio(21, &g_Cutscene_MapMsgAudioIdx, g_Cutscene_MapMsgAudioCmds);
            break;

        case 17:
            Event_DisplayMapMsgWithAudio(27, &g_Cutscene_MapMsgAudioIdx, g_Cutscene_MapMsgAudioCmds);
            break;

        case 18:
            g_Cutscene_Timer0        = Q12(58.0f);
            g_SysWork.lightIntensity = Q12(1.0f);

            Event_DisplayMapMsgWithAudio(33, &g_Cutscene_MapMsgAudioIdx, g_Cutscene_MapMsgAudioCmds);
            break;

        case 19:
            Event_DisplayMapMsgWithAudio(38, &g_Cutscene_MapMsgAudioIdx, g_Cutscene_MapMsgAudioCmds);
            break;

        case 20:
            Event_WaitTimer(Q12(1.5f), false);
            break;

        case 21:
            Event_DisplayMapMsgWithAudio(41, &g_Cutscene_MapMsgAudioIdx, g_Cutscene_MapMsgAudioCmds);
            break;

        case 22:
            g_Cutscene_Timer0        = Q12(59.0f);
            g_SysWork.lightIntensity = Q12(0.7f);

            Event_DisplayMapMsgWithAudio(47, &g_Cutscene_MapMsgAudioIdx, g_Cutscene_MapMsgAudioCmds);
            break;

        case 23:
            Event_DisplayMapMsgWithAudio(50, &g_Cutscene_MapMsgAudioIdx, g_Cutscene_MapMsgAudioCmds);
            break;

        case 24:
            Event_WaitTimer(Q12(1.5f), false);
            break;

        case 25:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, playerChara, 72, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, cybilChara, 1, false);
            SysWork_StateStepIncrement(0);

        case 26:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer0, Q12(10.0f), Q12(59.0f), Q12(64.0f), true, true);
            break;

        case 27:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer0, Q12(16.0f), Q12(65.0f), Q12(83.0f), true, true);
            g_SysWork.lightIntensity = Q12(1.0f);
            break;

        case 28:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimUnlock, playerChara, 0, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, playerChara, 51, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, cybilChara, 5, false);

            g_Cutscene_Timer0        = Q12(87.0f);
            g_SysWork.lightIntensity = Q12(0.5f);

            SysWork_StateStepIncrement(0);

        case 29:
            Event_DisplayMapMsgWithAudio(0x3D, &g_Cutscene_MapMsgAudioIdx, g_Cutscene_MapMsgAudioCmds);
            break;

        case 30:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, playerChara, 140, false);
            g_SysWork.lightIntensity = Q12(1.0f);
            SysWork_StateStepIncrement(0);

        case 31:
            g_Cutscene_Timer0 = Q12(88.0f);
            Event_DisplayMapMsgWithAudio(73, &g_Cutscene_MapMsgAudioIdx, g_Cutscene_MapMsgAudioCmds);
            break;

        case 32:
            Event_WaitTimer(Q12(0.8f), false);
            break;

        case 33:
            Event_DisplayMapMsgWithAudio(74, &g_Cutscene_MapMsgAudioIdx, g_Cutscene_MapMsgAudioCmds);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer0, Q12(10.0f), Q12(88.0f), Q12(121.0f), true, false);
            break;

        case 34:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer0, Q12(10.0f), Q12(88.0f), Q12(121.0f), true, true);
            break;

        case 35:
            g_Cutscene_Timer0 = Q12(122.0f);
            D_800D8018 = 1;
            g_SysWork.lightIntensity = Q12(1.2f);

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, playerChara, 51, false);
            SysWork_StateStepIncrement(0);

        case 36:
            Event_DisplayMapMsgWithAudio(76, &g_Cutscene_MapMsgAudioIdx, g_Cutscene_MapMsgAudioCmds);
            break;

        case 37:
            Event_DisplayMapMsgWithAudio(86, &g_Cutscene_MapMsgAudioIdx, g_Cutscene_MapMsgAudioCmds);
            break;

        case 38:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, playerChara, 130, false);

            D_800D8018 = 0;
            g_SysWork.lightIntensity = Q12(1.0f);

            SysWork_StateStepIncrement(0);

        case 39:
            Event_DisplayMapMsgWithAudio(90, &g_Cutscene_MapMsgAudioIdx, g_Cutscene_MapMsgAudioCmds);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer0, Q12(10.0f), Q12(123.0f), Q12(139.0f), true, false);
            break;

        case 40:
            Event_CharaAnimPlayToEnd(playerChara, 0x78);

            g_SysWork.lightIntensity = Q12(1.4f);

            Savegame_EventFlagSet(EventFlag_313);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer0, Q12(10.0f), Q12(140.0f), Q12(165.0f), true, false);
            break;

        case 41:
            Player_WeaponAttackRestore();
            WorldGfx_PlayerPrevHeldItem(&g_SysWork.playerCombat);
            Player_EmptyWeaponHandSet();
            SysWork_StateStepIncrement(0);

        case 42:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);

            if (g_SysWork.sysStateSteps[0] != 42)
            {
                Player_WeaponAttackRestore();
                SysWork_StateStepReset();
            }
            break;

        case EventState_Skip:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 44:
            Player_WeaponAttackRestore();
            WorldGfx_PlayerPrevHeldItem(&g_SysWork.playerCombat);
            SysWork_StateStepIncrement(0);

        default:
            Player_ControlUnfreeze(false);

            g_Cutscene_Timer0 = NO_VALUE;

            SysWork_StateSetNext(SysState_Gameplay);

            D_800D8018 = 0;

            Chara_ModelCharaIdClear(cybilChara, 0, 0);
            Fs_QueueWaitForEmpty();
            func_8003D01C();
            SD_Call(19);
            Savegame_EventFlagSet(EventFlag_303);
            func_8008D448();
            Game_FlashlightAttribsSet();

            g_SysWork.lightIntensity = Q12(1.0f);
            Savegame_EventFlagClear(EventFlag_313);
            CutsceneBorder_Reset();
            break;
    }

    if (g_Cutscene_Timer0 >= Q12(0.0f))
    {
        Dms_CharacterTransformGet(playerChara.position, playerChara.rotation, "HERO", g_Cutscene_Timer0, (s_DmsHeader*)FS_BUFFER_11);

        if (g_Cutscene_UpdateSibyl)
        {
            Dms_CharacterTransformGet(cybilChara.position, cybilChara.rotation, "SIBYL", g_Cutscene_Timer0, (s_DmsHeader*)FS_BUFFER_11);
        }

        vcChangeProjectionValue(Dms_CameraTargetsGet(&g_Cutscene_CameraPosition, &g_Cutscene_CameraLookAt, NULL, g_Cutscene_Timer0, (s_DmsHeader*)FS_BUFFER_11));
        vcUserCamTarget(&g_Cutscene_CameraPosition, NULL, true);
        vcUserWatchTarget(&g_Cutscene_CameraLookAt, NULL, true);

        Dms_CharacterTransformGet(&g_SysWork.lightPosition, &unused, "LIGHT", g_Cutscene_Timer0, FS_BUFFER_11);
        Dms_CharacterTransformGet(&lightIntPos, &unused, "L_INT", g_Cutscene_Timer0, FS_BUFFER_11);
        g_SysWork.lightRotation.vx = -ratan2(lightIntPos.vy - g_SysWork.lightPosition.vy, Math_Vector2MagCalcSafeQ6(lightIntPos.vx - g_SysWork.lightPosition.vx, lightIntPos.vz - g_SysWork.lightPosition.vz));
        g_SysWork.lightRotation.vy = ratan2(lightIntPos.vx - g_SysWork.lightPosition.vx, lightIntPos.vz - g_SysWork.lightPosition.vz);
        g_SysWork.lightRotation.vz = 0;
    }

    #undef soundPos
}

void func_800D2F74(void) // 0x800D2F74
{
    typedef enum _EventState
    {
        EventState_Skip = 10
    } e_EventState;

    #define playerChara &g_SysWork.playerWork.player

    // Skip.
    if (Pad_ButtonClickedBind(skip) &&
        g_SysWork.sysStateSteps[0] >= 3 && g_SysWork.sysStateSteps[0] < 10)
    {
        SysWork_StateStepSet(0, EventState_Skip);
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Fs_QueueStartRead(FILE_ANIM_AQRM_DMS, FS_BUFFER_11);
            Fs_QueueWaitForEmpty();
            Dms_HeaderFixOffsets(FS_BUFFER_11);

            CutsceneBorder_ForceShow();

            Screen_FadeTimestepReset();

            g_Cutscene_MapMsgAudioIdx0 = 0;
            g_Cutscene_Timer1 = Q12(0.0f);
            g_SysWork.sysFlags |= SysFlag_CutsceneActive;

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, playerChara, 121, false);
            Game_TurnFlashlightOn();
            Player_EmptyWeaponHandSet();
            func_8003D03C();
            SysWork_StateStepIncrement(0);

            CutsceneBorder_ForceShow();
            g_SysWork.sysFlags |= SysFlag_CutsceneActive;
            break;

        case 1:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            SysWork_StateStepIncrement(0);

        case 2:
            SysWork_StateStepIncrement(0);

        case 3:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer1, Q12(10.0f), Q12(0.0f), Q12(34.0f), true, true);
            break;

        case 4:
            Event_CharaAnimPlayToEnd(playerChara, 116);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer1, Q12(10.0f), Q12(35.0f), Q12(71.0f), true, false);
            break;

        case 5:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, playerChara, 53, false);
            SysWork_StateStepIncrement(0);

        case 6:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer1, Q12(15.0f), Q12(72.0f), Q12(93.0f), true, true);
            break;

        case 7:
            SysWork_StateStepIncrement(0);
            break;

        case 8:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, playerChara, 51, false);

            g_Cutscene_Timer1 = Q12(95.0f);

            SysWork_StateStepIncrement(0);

        case 9:
            Event_DisplayMapMsgWithAudio(0x5B, &g_Cutscene_MapMsgAudioIdx0, g_Cutscene_MapMsgAudioCmds0);
            break;

        case EventState_Skip:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);

            if (g_SysWork.sysStateSteps[0] != 10)
            {
                g_Cutscene_Timer1 = Q12(95.0f);
            }
            break;

        default:
            g_Cutscene_Timer1 = NO_VALUE;

            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);

            Savegame_EventFlagSet(EventFlag_305);
            vcReturnPreAutoCamWork(true);
            Fs_QueueWaitForEmpty();
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            SD_Call(19);
            Player_WeaponAttackRestore();
            func_8003D01C();
            break;
    }

    if (g_Cutscene_Timer1 >= Q12(0.0f))
    {
        Dms_CharacterTransformGet(playerChara.position, playerChara.rotation, "HERO", g_Cutscene_Timer1, (s_DmsHeader*)FS_BUFFER_11);
        vcChangeProjectionValue(Dms_CameraTargetsGet(&g_Cutscene_CameraPosition, &g_Cutscene_CameraLookAt, NULL, g_Cutscene_Timer1, (s_DmsHeader*)FS_BUFFER_11));
        vcUserCamTarget(&g_Cutscene_CameraPosition, NULL, true);
        vcUserWatchTarget(&g_Cutscene_CameraLookAt, NULL, true);
    }
}

void func_800D3420(void) // 0x800D3420
{
    typedef enum _EventState
    {
        EventState_Skip = 30
    } e_EventState;

    typedef struct
    {
        SPRT*     sprt_0;
        DR_TPAGE* tpage_4;
        DR_STP*   stp_8;
        u8        unk_C[4];
        u8        unk_10[4];
        s32       activeBufferIdx_14;
    } g_GteScratchData_func_800DD2D4;

    VECTOR3                         lightIntPos;
    SVECTOR3                        unused;
    s32                             i;
    g_GteScratchData_func_800DD2D4* scratchData;

    #define playerChara &g_SysWork.playerWork.player
    #define cybilChara  &g_SysWork.npcs[0]

    scratchData = PSX_SCRATCH_ADDR(0);

    // Skip.
    if (Pad_ButtonClickedBind(skip) &&
        g_SysWork.sysStateSteps[0] >= 2 && g_SysWork.sysStateSteps[0] < 29)
    {
        SysWork_StateStepSet(0, EventState_Skip);
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            D_800D5B06         = false;
            CutsceneBorder_ForceShow();
            g_Cutscene_Timer3         = Q12(96.0f);

            sharedData_800D8568_1_s05.field_1  = 3;
            sharedData_800D8568_1_s05.flags  = (1 << 4) | (1 << 0);
            sharedData_800D8568_1_s05.field_C  = 0xFA;
            sharedData_800D8568_1_s05.field_12 = -0x2333;
            sharedData_800D8568_1_s05.field_14 = -0x1333;
            sharedData_800D8568_1_s05.field_1C = 0x400;
            sharedData_800D8568_1_s05.field_E  = -0x800;
            sharedData_800D8568_1_s05.field_3  = 0x40;
            sharedData_800D8568_1_s05.field_4  = 0x80;
            sharedData_800D8568_1_s05.field_5  = 0xFF;
            sharedData_800D8568_1_s05.field_8  = 0x80;
            sharedData_800D8568_1_s05.field_20 = 0xB04B0;
            sharedData_800D8568_1_s05.field_24 = 0x132CE;
            sharedData_800D8568_1_s05.field_6  = 0x14;
            sharedData_800D8568_1_s05.field_A  = 0x1000;
            sharedData_800D8568_1_s05.field_2  = 8;

            g_SysWork.sysFlags |= SysFlag_CutsceneActive;

            sharedFunc_800CAAD0_1_s05();

            g_SysWork.lightBoneCoord = NULL;
            Math_Vector3Set(&g_SysWork.lightPosition, sharedData_800D8568_1_s05.field_20, Q12(-2.0f), sharedData_800D8568_1_s05.field_24 + Q12(1.5f));
            g_SysWork.lensFlareBoneCoord = NULL;
            Math_SetSVectorFast(&g_SysWork.lightRotation, Q12_ANGLE(-90.0f), Q12_ANGLE(0.0f), Q12_ANGLE(0.0f));

            WorldEnv_TransitionStart(11, 11, PrimitiveType_S32, &D_800D5B00, 0, Q12(100.0f));
            D_800D5B00 = Q12(0.0f);
            func_8008D438();

            g_SysWork.lightIntensity                = Q12(2.5f);
            g_SysWork.playerWork.player.position.vx = Q12(180.0f);
            g_SysWork.playerWork.player.position.vz = Q12(60.0f);

            WorldGfx_CloseRangeChunksInit();
            SD_Call(Sfx_Unk1540);

            D_800D5B04 = Q12(0.2f);
            D_800D5AF9 = true;

            Savegame_EventFlagSet(EventFlag_312);

            func_8003D03C();
            Player_EmptyWeaponHandSet();
            SysWork_StateStepIncrement(0);

        case 1:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, playerChara, 142, false);
            D_800D5B06 = true;
            SysWork_StateStepIncrement(0);

        case 2:
            D_800D5B00 += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 12.0f);

            Event_DisplayMapMsgWithAudio(95, &g_Cutscene_MapMsgAudioIdx2, &g_Cutscene_MapMsgAudioCmds2);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer3, Q12(5.0f), Q12(96.0f), Q12(115.0f), true, false);

            D_800D5B04 += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 1.5f);
            if (D_800D5B04 > Q12(1.0f))
            {
                D_800D5B04 = Q12(1.0f);
            }
            break;

        case 3:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer3, Q12(5.0f), Q12(96.0f), Q12(115.0f), true, true);
            D_800D5B00 += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 12.0f);

            D_800D5B04 += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 1.5f);
            if (D_800D5B04 > Q12(1.0f))
            {
                D_800D5B04 = Q12(1.0f);
            }
            break;

        case 4:
            SysWork_StateStepIncrement(0);

        case 5:
            SysWork_StateStepIncrement(0);

        case 6:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, playerChara, 51, false);
            SysWork_StateStepIncrement(0);

        case 7:
            g_Cutscene_Timer3  = Q12(139.0f);

            D_800D5B00 += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 12.0f);
            if (D_800D5B00 >= Q12(100.0f))
            {
                SysWork_StateStepIncrement(0);
            }
            break;

        case 8:
            D_800D5B04 -= Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.8f);
            if (D_800D5B04 < Q12(0.0f))
            {
                D_800D5B04 = Q12(0.0f);
            }

            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.5f), false);
            break;

        case 9:
            D_800D5B06 = false;

            for (i = ARRAY_SIZE(sharedData_800DFB7C_0_s00) - 1; i >= 0; i--)
            {
                sharedData_800DFB7C_0_s00[i].field_A = 0;
            }

            D_800D5AF9 = false;
            Sd_SfxStop(Sfx_Unk1540);
            g_Cutscene_Timer3 = Q12(140.0f);
            WorldEnv_PresetsSet(7, 4);

            g_SysWork.lightBoneCoord     = NULL;
            g_SysWork.lensFlareBoneCoord = NULL;
            g_SysWork.lightIntensity     = Q12(1.0f);
            Math_Vector3Set(&g_SysWork.lightPosition, Q12(178.8f), Q12(0.0f), Q12(62.8f));
            Math_SetSVectorFast(&g_SysWork.lightRotation, Q12_ANGLE(30.0f), Q12_ANGLE(-90.0f), Q12_ANGLE(0.0f));

            SysWork_StateStepIncrement(0);
            break;

        case 10:
            Chara_Spawn(Chara_Cybil, 0, Q12(183.0f), Q12(61.0f), Q12_ANGLE(0.0f), 3);
            Model_AnimFlagsClear(&(playerChara)->model, 2);
            SysWork_StateStepIncrement(0);
            break;

        case 11:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, cybilChara, 18, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, 0, false);
            Fs_QueueWaitForEmpty();
            SysWork_StateStepIncrement(0);
            break;

        case 12:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimLock, cybilChara, 0, false);
            Event_WaitTimer(Q12(1.0f), false);
            break;

        case 13:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimUnlock, cybilChara, 0, false);
            SysWork_StateStepIncrement(0);

        case 14:
            Event_DisplayMapMsgWithAudio(97, &g_Cutscene_MapMsgAudioIdx2, &g_Cutscene_MapMsgAudioCmds2);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer3, Q12(10.0f), Q12(140.0f), Q12(200.0f), true, false);
            break;

        case 15:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer3, Q12(10.0f), Q12(140.0f), Q12(200.0f), true, true);
            break;

        case 16:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, cybilChara, 19, false);
            SysWork_StateStepIncrement(0);

        case 17:
            Event_DisplayMapMsgWithAudio(98, &g_Cutscene_MapMsgAudioIdx2, &g_Cutscene_MapMsgAudioCmds2);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer3, Q12(2.0f), Q12(201.0f), Q12(216.0f), true, false);
            break;

        case 18:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer3, Q12(2.0f), Q12(201.0f), Q12(216.0f), true, true);
            break;

        case 19:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AwaitAnimEnd, cybilChara, 0, false);
            break;

        case 20:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 21:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, cybilChara, 20, false);
            Math_Vector3Set(&g_SysWork.lightPosition, Q12(175.9f), Q12(-1.0f), Q12(22.8f));
            Math_SetSVectorFast(&g_SysWork.lightRotation, Q12_ANGLE(0.0f), Q12_ANGLE(60.0f), Q12_ANGLE(0.0f));

            g_SysWork.lightIntensity = Q12(1.0f);
            g_Cutscene_Timer3        = Q12(217.0f);

            SysWork_StateStepIncrement(0);
            break;

        case 22:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimLock, cybilChara, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            Fs_QueueWaitForEmpty();
            SysWork_StateStepIncrement(0);

        case 23:
            Event_WaitTimer(Q12(0.3f), false);
            break;

        case 24:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimUnlock, cybilChara, 0, false);
            SysWork_StateStepIncrement(0);

        case 25:
            Event_DisplayMapMsgWithAudio(99, &g_Cutscene_MapMsgAudioIdx2, &g_Cutscene_MapMsgAudioCmds2);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer3, Q12(8.0f), Q12(217.0f), Q12(259.0f), true, false);
            break;

        case 26:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer3, Q12(8.0f), Q12(217.0f), Q12(259.0f), true, true);
            break;

        case 27:
            Event_WaitTimer(Q12(1.4f), false);
            break;

        case 28:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.5f), false);
            break;

        case 29:
            SysWork_StateStepSet(0, -1);
            break;

        case 30:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);

            D_800D5B06 = false;
            for (i = ARRAY_SIZE(sharedData_800DFB7C_0_s00) - 1; i >= 0; i--)
            {
                sharedData_800DFB7C_0_s00[i].field_A = 0;
            }

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            g_Cutscene_Timer3 = NO_VALUE;
            SD_Call(19);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            Savegame_EventFlagSet(EventFlag_306);

            Model_AnimFlagsSet(&(playerChara)->model, 2);
            g_SysWork.lightIntensity = Q12(1.0f);
            func_8003D01C();
            Player_WeaponAttackRestore();
            break;
    }

    if (D_800D5AF9)
    {
        Sfx_WithFalloffAndPitchPlay(Sfx_Unk1540, &Q12_VECTOR3(176.3f, -1.5f, 19.18f), Q8(0.5f) - Q12_MULT_PRECISE(D_800D7F30, 128), Q12(16.0f), 0);
    }

    if (g_Cutscene_Timer3 >= Q12(0.0f))
    {
        Dms_CharacterTransformGet(playerChara.position, playerChara.rotation, "HERO", g_Cutscene_Timer3, FS_BUFFER_11);
        if (g_SysWork.sysStateSteps[0] >= 10)
        {
            Dms_CharacterTransformGet(cybilChara.position, cybilChara.rotation, "SIBYL", g_Cutscene_Timer3, FS_BUFFER_11);
        }

        vcChangeProjectionValue(Dms_CameraTargetsGet(&g_Cutscene_CameraPosition, &g_Cutscene_CameraLookAt, NULL, g_Cutscene_Timer3, FS_BUFFER_11));
        vcUserCamTarget(&g_Cutscene_CameraPosition, NULL, true);
        vcUserWatchTarget(&g_Cutscene_CameraLookAt, NULL, true);

        if (g_SysWork.sysStateSteps[0] >= 21)
        {
            Dms_CharacterTransformGet(&g_SysWork.lightPosition, &unused, "LIGHT", g_Cutscene_Timer3, FS_BUFFER_11);
            Dms_CharacterTransformGet(&lightIntPos, &unused, "L_INT", g_Cutscene_Timer3, FS_BUFFER_11);
            g_SysWork.lightRotation.vx = -ratan2(lightIntPos.vy - g_SysWork.lightPosition.vy, Math_Vector2MagCalcSafeQ6(lightIntPos.vx - g_SysWork.lightPosition.vx, lightIntPos.vz - g_SysWork.lightPosition.vz));
            g_SysWork.lightRotation.vy = ratan2(lightIntPos.vx - g_SysWork.lightPosition.vx, lightIntPos.vz - g_SysWork.lightPosition.vz);
            g_SysWork.lightRotation.vz = 0;
        }
    }

    if (D_800D5B06)
    {
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
    }
}

void func_800D4410(void) // 0x800D4410
{
    typedef enum _EventState
    {
        EventState_Skip = 14
    } e_EventState;

    #define playerChara &g_SysWork.playerWork.player

    // Skip.
    if (Pad_ButtonClickedBind(skip) &&
        g_SysWork.sysStateSteps[0] >= 2 && g_SysWork.sysStateSteps[0] < 13)
    {
        SD_Call(19);
        g_SysWork.sysStateSteps[0] = EventState_Skip;
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();

            CutsceneBorder_ForceShow();
            Screen_FadeTimestepReset();
            g_SysWork.sysFlags |= SysFlag_CutsceneActive;

            Fs_QueueStartRead(FILE_ANIM_SQSU_DMS, FS_BUFFER_11);
            Fs_QueueWaitForEmpty();
            Dms_HeaderFixOffsets((s_DmsHeader*)FS_BUFFER_11);

            g_Cutscene_Timer2 = Q12(0.0f);
            g_Cutscene_MapMsgAudioIdx1 = 0;

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, playerChara, 124, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimLock, playerChara, 0, false);
            func_8008D438();

            g_SysWork.lightBoneCoord     = NULL;
            g_SysWork.lightPosition.vx   = Q12(19.6f);
            g_SysWork.lightPosition.vy   = Q12(2.0f);
            g_SysWork.lightPosition.vz   = Q12(62.0f);
            g_SysWork.lensFlareBoneCoord = NULL;
            g_SysWork.lightRotation.vx   = Q12_ANGLE(90.0f);
            g_SysWork.lightRotation.vy   = Q12_ANGLE(0.0f);
            g_SysWork.lightRotation.vz   = Q12_ANGLE(0.0f);
            g_SysWork.lightIntensity     = Q12(1.0f);

            WorldEnv_PresetsSet(14, 14);
            func_8003D03C();
            Player_EmptyWeaponHandSet();
            SysWork_StateStepIncrement(0);

        case 1:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.5f), false);
            break;

        case 2:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimUnlock, playerChara, 0, false);
            Event_DisplayMapMsgWithAudio(102, &g_Cutscene_MapMsgAudioIdx1, g_Cutscene_MapMsgAudioCmds1);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer2, Q12(10.0f), Q12(0.0f), Q12(106.0f), true, false);
            break;

        case 3:
            Event_WaitTimer(Q12(1.5f), false);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer2, Q12(10.0f), Q12(0.0f), Q12(106.0f), true, false);
            break;

        case 4:
            Event_DisplayMapMsgWithAudio(103, &g_Cutscene_MapMsgAudioIdx1, g_Cutscene_MapMsgAudioCmds1);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer2, Q12(10.0f), Q12(0.0f), Q12(106.0f), true, false);
            break;

        case 5:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer2, Q12(10.0f), Q12(0.0f), Q12(106.0f), true, true);
            break;

        case 6:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, playerChara, 153, false);
            SysWork_StateStepIncrement(0);

        case 7:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer2, Q12(10.0f), Q12(107.0f), Q12(128.0f), true, true);
            break;

        case 8:
            Event_DisplayMapMsg(false, 104, 0, 0, 0, false);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer2, Q12(3.8f), Q12(129.0f), Q12(228.0f), true, false);
            break;

        case 9:
            Event_WaitTimer(Q12(1.5f), false);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer2, Q12(3.8f), Q12(129.0f), Q12(228.0f), true, false);
            break;

        case 10:
            Event_DisplayMapMsg(false, 108, 0, 0, 0, false);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer2, Q12(3.8f), Q12(129.0f), Q12(228.0f), true, false);
            break;

        case 11:
            Event_WaitTimer(Q12(0.6f), false);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer2, Q12(3.8f), Q12(129.0f), Q12(228.0f), true, false);
            break;

        case 12:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(1.0f), false);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer2, Q12(3.8f), Q12(129.0f), Q12(228.0f), true, false);
            break;

        case 13:
            SysWork_StateStepReset();
            break;

        case EventState_Skip:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        default:
            Player_ControlUnfreeze(true);
            SysWork_StateSetNext(SysState_Gameplay);
            Savegame_EventFlagSet(EventFlag_309);
            vcReturnPreAutoCamWork(true);

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            WorldEnv_PresetsSet(7, 4);
            func_8008D448();
            Game_FlashlightAttribsSet();

            g_SysWork.lightIntensity = Q12(1.0f);
            Game_TurnFlashlightOn();

            g_Cutscene_Timer2 = NO_VALUE;

            func_8003D01C();
            Player_WeaponAttackRestore();
            break;
    }

    if (g_Cutscene_Timer2 >= Q12(0.0f))
    {
        Dms_CharacterTransformGet(playerChara.position, playerChara.rotation, "HERO", g_Cutscene_Timer2, (s_DmsHeader*)FS_BUFFER_11);
        vcChangeProjectionValue(Dms_CameraTargetsGet(&g_Cutscene_CameraPosition, &g_Cutscene_CameraLookAt, NULL, g_Cutscene_Timer2, (s_DmsHeader*)FS_BUFFER_11));
        vcUserCamTarget(&g_Cutscene_CameraPosition, NULL, true);
        vcUserWatchTarget(&g_Cutscene_CameraLookAt, NULL, true);
    }
}

void func_800D496C(void) // 0x800D496C
{
    Event_ItemTake(InvItemId_Axe, DEFAULT_PICKUP_ITEM_COUNT, EventFlag_M4S01_PickupAxe, 113);
}

void func_800D4998(void) // 0x800D4998
{
    Event_DisplayMapMsgWithBg(FILE_TIM_CLOCKATQ_TIM, Q12(0.0f), Q12(0.0f), 114);
}

void Map_WorldObjectsInit(void) // 0x800D49C4
{
    WorldObject_PlacementInit(&WorldObject_D_800D7FF0, "MOV1_HID", 178.2681f, -0.387f, 63.6814f);
    WorldObject_PlacementInit(&WorldObject_D_800D8020, "MOV1_HID", 174.81f, -1.4197f, 21.3344f);

    if (!Savegame_EventFlagGet(EventFlag_302))
    {
        WorldObject_D_800D7FF0.position.vz = Q12(62.7466f);
    }

    WorldObject_ModelNameSet(&WorldObject_D_800D8050.object, g_CommonWorldObjectNames[1]);

    WorldObject_PoseInit(&WorldObject_D_800D8070, 183.3833f, -1.2673f, 60.951f, 0.0f, 66.01, 0.0f);
    WorldObject_PoseInit(&WorldObject_D_800D8090, 23.4f, -0.91f, 60.8f, 0.0f, 90.4f, 0.0f);
    WorldObject_PlacementInit(&WorldObject_D_800D80B0, "WALLX_HI", 181.22f, 0.0f, 61.2015f);
    WorldObject_PlacementInit(&WorldObject_D_800D80E0[0], "CUBE90_H", 180.5647f, -0.502f, 63.8853f);
    WorldObject_PlacementInit(&WorldObject_D_800D80E0[1], "CUBE9_HI", 180.5647f, -0.502f, 63.8853f);

    D_800D8018 = 0;
}

void Map_WorldObjectsUpdate(void) // 0x800D4BA4
{
    MAP_CHUNK_CHECK_VARIABLE_DECL();

    if (PLAYER_IN_MAP_CHUNK(vx, 1, 5, -1, 5) && PLAYER_IN_MAP_CHUNK(vz, 1, 2, -1, 2))
    {
        if (Savegame_EventFlagGet(EventFlag_302))
        {
            Collision_TriggerFlagBitsClear(CollisionTriggerFlag_All);
        }
        else
        {
            Collision_TriggerFlagBitsSet(CollisionTriggerFlag_Objects);
        }

        WorldObjects_Add(&WorldObject_D_800D7FF0.object, &WorldObject_D_800D7FF0.position, &SVECTOR3_Zero);
        WorldObjects_Add(&WorldObject_D_800D8050.object, &WorldObject_D_800D8070.position, &WorldObject_D_800D8070.rotation);

        if (Savegame_EventFlagGet(EventFlag_313))
        {
            WorldObjects_Add(&WorldObject_D_800D80B0.object, &WorldObject_D_800D80B0.position, &SVECTOR3_Zero);
        }

        WorldObjects_Add(&WorldObject_D_800D80E0[D_800D8018].object, &WorldObject_D_800D80E0[D_800D8018].position, &SVECTOR3_Zero);
    }
    else
    {
        Collision_TriggerFlagBitsClear(CollisionTriggerFlag_All);
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, 5, -1, 5) && PLAYER_IN_MAP_CHUNK(vz, 0, 0, -1, 1))
    {
        if (!Savegame_EventFlagGet(EventFlag_M4S01_PickupAxe))
        {
            WorldObjects_Add(&WorldObject_D_800D8020.object, &WorldObject_D_800D8020.position, &SVECTOR3_Zero);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 0, 0, -1, 1) && PLAYER_IN_MAP_CHUNK(vz, 1, 2, -1, 2))
    {
        WorldObjects_Add(&WorldObject_D_800D8050.object, &WorldObject_D_800D8090.position, &WorldObject_D_800D8090.rotation);
    }
}
