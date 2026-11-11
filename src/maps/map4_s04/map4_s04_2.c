#include "bodyprog/bodyprog.h"
#include "bodyprog/chara/dms.h"
#include "bodyprog/events/event_data.h"
#include "bodyprog/world/effects.h"
#include "bodyprog/math/math.h"
#include "bodyprog/text/text_draw.h"
#include "bodyprog/sound/sound_system.h"
#include "bodyprog/player/player.h"
#include "main/rng.h"
#include "maps/map4/map4_s04.h"
#include "maps/common/characters/lisa.h"
#include "maps/common/particle.h"
#include "maps/common/characters/player.h"

#include "../src/maps/common/particles/particle.c"

#include "../src/maps/common/characters/player.c"

#include "../src/maps/common/characters/lisa.c"

#include "../src/maps/common/chara_util.c" // 0x800D098C

#include "../src/maps/common/map_util.c" // 0x800D1394

#include "maps/common/shared/Map_RoomBgmInit_CheckCond.h" // 0x800D143C

#include "maps/common/shared/Map_RoomBgmInit_3_s02_CondTrue.h" // 0x800D1470

#include "maps/common/shared/Map_RoomBgmInit_3_s02_CondFalse.h" // 0x800D14B0

void GameBoot_LoadScreen_StageString(void) {}

#include "maps/common/shared/MapEvent_DoorJammed.h" // 0x800D1748

#include "maps/common/shared/MapEvent_DoorLocked.h" // 0x800D17DC

void func_800D1870(void) {}

#include "maps/common/shared/Event_CutsceneTimerAdvance.h" // 0x800D1878

const char* MAP_MESSAGES[] = {
    #include "maps/common/shared/map_msg_common.inc"
#if VERSION_REGION_IS(NTSC)
    /* 15 */ "~J0(1.8)\tWhere_am_I? ~E ",
    /* 16 */ "~J1(1.0)\tHarry. ",
    /* 17 */ "~J1(1.2)\tLisa... ",
    /* 18 */ "~J1(1.5)\tThen_I'm_in_the_hospital. ",
    /* 19 */ "~J1(2.4)\tYou_were_having_a_bad_dream. ",
    /* 20 */ "~J1(0.8)\tWas_I? ",
    /* 21 */ "~J1(4.2)\tHey,_you_don't_look_too_good. ~N\n\t\t\tAre_you_OK? ",
    /* 22 */ "~J1(1.3)\tI'm_fine. ",
    /* 23 */ "~J1(2.2)\tNothing_you_need_to ~N\n\t\t\tworry_about. ",
    /* 24 */ "~J1(2.2)\tWell,_if_you're_sure... ~E ",
    /* 25 */ "~J1(4.3)\tLisa... ~N\n\t\t\tDo_you_know_a_woman ~N\n\t\t\tnamed_Dahlia_Gillespie? ",
    /* 26 */ "~J1(3.2)\tOh_yeah, ~N\n\t\t\tthat_crazy_Gillespie_lady. ",
    /* 27 */ "~J1(2.3)\tShe's_kinda'_famous_around_here. ",
    /* 28 */ "~J1(4.0)\tShe_never_sees_anybody, ~N\n\t\t\tso_I_don't_know ~N\n\t\t\tthat_much_about_her. ",
    /* 29 */ "~J1(5.2)\tI_heard_her_kid_died_in_a_fire, ~N\n\t\t\tand_supposedly_she's_been_crazy ~N\n\t\t\tever_since. ~E ",
    /* 30 */ "~J1(5.1)\tWell,_she_says_the_town_is_being ~N\n\t\t\tdevoured_by_the_darkness. ",
    /* 31 */ "~J1(2.7)\tDo_you_have_any_idea ~N\n\t\t\twhat_she's_talking_about? ",
    /* 32 */ "~J1(3.5)\tThe_town..._devoured ~N\n\t\t\tby_the_darkness. ",
    /* 33 */ "~J1(2.1)\tYes,_I_think_I_do. ",
    /* 34 */ "~J1(2.9)\tBefore_this_place_was_turned ~N\n\t\t\tinto_a_resort, ",
    /* 35 */ "~J1(3.4)\tthe_townspeople_here_were ~N\n\t\t\ton_the_quiet_side. ",
    /* 36 */ "~J1(4.1)\tEverybody_followed ~N\n\t\t\tsome_kind_of_queer_religion. ~E ",
    /* 37 */ "~J1(4.8)\tWeird_occult_stuff... ~N\n\t\t\tBlack_magic,_that_kind_of_thing. ",
    /* 38 */ "~J1(5.0)\tAs_young_people_moved_away, ~N\n\t\t\tthe_people_figured_they'd_been ~N\n\t\t\tsummoned_by_the_gods. ",
    /* 39 */ "~J1(3.7)\tEvidently,_things_like_that ~N\n\t\t\tused_to_happen_around_here ~N\n\t\t\tall_the_time. ",
    /* 40 */ "~J1(3.5)\tBefore_the_resort, ~N\n\t\t\tthere_really_wasn't ~N\n\t\t\tanything_else_out_here. ",
    /* 41 */ "~J1(4.5)\tEveryone_was_so_flipped_out. ~N\n\t\t\tGotta_blame_it_on_something. ",
    /* 42 */ "~J1(3.8)\tThen_a_lot_of_new_people ~N\n\t\t\tcame_in_and_everybody ~N\n\t\t\tclammed_up_about_it. ",
    /* 43 */ "~J0(1.8)\tA_cult... ~E ",
    /* 44 */ "~J0(4.8)\tLast_time_I_heard_anything ~N\n\t\t\tabout_it_was,_gosh,_years_ago... ",
    /* 45 */ "~J1(4.4)\tWhen_several_people_connected ~N\n\t\t\twith_developing_the_town ~N\n\t\t\tdied_in_accidents. ",
    /* 46 */ "~J1(2.0)\tPeople_said_it_was_a_curse. ",
    /* 47 */ "~J1(4.2)\tI'm_sorry,_I'm_rambling... ~N\n\t\t\tI'll_shut_up. ~E ",
    /* 48 */ "~J0(1.1)\tHarry! ~E ",
    /* 49 */ "~J0(1.6)\tGlad_you're_OK. ~E ",
    /* 50 */ "~J0(4.6)\tThank_God_you_came_back. ~N\n\t\t\tI_was_scared_to_be_here_all_alone. ",
    /* 51 */ "~J0(2.5)\tI'm_here_now. ~N\n\t\t\tI_was_worried,_too. ",
    /* 52 */ "~J0(1.6)\tI'm_real_happy_to_see_you. ~E ",
    /* 53 */ "~J1(3.3)\tLisa,_can_you_tell_me ~N\n\t\t\thow_to_get_to_the_lake? ",
    /* 54 */ "~J1(3.0)\tThe_lake? ~N\n\t\t\tYou_take_Bachman_Road. ",
    /* 55 */ "~J1(1.5)\tThe_road's_blocked. ",
    /* 56 */ "~J1(2.3)\tWell,_that's ~N\n\t\t\tthe_only_way_out_there. ",
    /* 57 */ "~J1(1.3)\tAre_you_sure? ",
    /* 58 */ "~J1(2.7)\tThere's_gotta_be_another_way. ~E ",
    /* 59 */ "~J1(3.0)\tWait, ~N\n\t\t\tI_just_remembered_something. ",
    /* 60 */ "~J1(0.6)\tWhat? ",
    /* 61 */ "~J1(3.3)\tThere's_a_water_works_over_by ~N\n\t\t\tmy_old_elementary_school. ",
    /* 62 */ "~J1(2.3)\tIt's_been_abandoned_for_years. ",
    /* 63 */ "~J1(4.2)\tThere's_an_underground_tunnel ~N\n\t\t\tout_there_used_for_inspections, ~N\n\t\t\tor_something! ",
    /* 64 */ "~J1(3.2)\tI_remember_hearing_it_runs ~N\n\t\t\tall_the_way_to_the_lake! ~E ",
    /* 65 */ "~J1(0.9)\tReally?! ",
    /* 66 */ "~J1(2.3)\tYou_think_I_can_get_to_the_lake ~N\n\t\t\tfrom_there? ",
    /* 67 */ "~J1(3.1)\tI've_never_been_down ~N\n\t\t\tin_there_myself, ~N\n\t\t\tso_I'm_not_positive. ",
    /* 68 */ "~J1(2.8)\tBesides,_it's_all_fenced_off ~N\n\t\t\tto_keep_people_out. ",
    /* 69 */ "~J1(2.3)\tIf_there's_a_chance, ~N\n\t\t\tI've_got_to_try. ~E  ",
    /* 70 */ "~J1(2.0)\tHarry,_don't_go! ",
    /* 71 */ "~J1(4.2)\tI_don't_want_to_be_alone. ~N\n\t\t\tIt's_so_scary._I_can't_stand_it. ",
    /* 72 */ "~J1(1.6)\tHow_about_coming_with_me? ",
    /* 73 */ "~J1(2.6)\tThis_may_not_be_the_safest_place ~N\n\t\t\tin_the_world_either. ",
    /* 74 */ "~J1(3.6)\tI_can't_promise_you_anything, ~N\n\t\t\tbut_I'll_do_my_best ~N\n\t\t\tto_protect_you. ",
    /* 75 */ "~J1(4.2)\tNo... ~N\n\t\t\tsomehow_I_feel_I'm_not_supposed ~N\n\t\t\tto_leave_this_place. ",
    /* 76 */ "~J1(4.0)\tOh,_Harry,_I'm_so_scared... ~N\n\t\t\tI'm_cold. ",
    /* 77 */ "~J1(3.8)\tLook,_just_wait_here ~N\n\t\t\ta_little_longer. ",
    /* 78 */ "~J1(3.0)\tI'll_be_back_as_soon_as ~N\n\t\t\tI_find_my_daughter. ~E ",
    /* 79 */ "~J0(1.5)\tHarry... ~E ",
    /* 80 */ "\tNothing_unusual. ~E ",
    /* 81 */ "\tNothing_helpful. ~E "
#elif VERSION_REGION_IS(NTSCJ)
    /* 15 */ "~J0(1.8)\t……ここは？ ~E ",
    /* 16 */ "~J1(1.0)\tハリー ",
    /* 17 */ "~J1(1.2)\tリサ…… ",
    /* 18 */ "~J1(1.5)\tここは病院か…… ",
    /* 19 */ "~J1(2.4)\tひどくうなされていたわ ",
    /* 20 */ "~J1(0.8)\tそうか…… ",
    /* 21 */ "~J1(4.2)\tきみこそ顔色が悪いんじゃないか？ ~N\n\t\t\t大丈夫か？ ",
    /* 22 */ "~J1(1.3)\t大丈夫よ ",
    /* 23 */ "~J1(2.2)\t心配しないで ",
    /* 24 */ "~J1(2.2)\tそれならいいが…… ~E ",
    /* 25 */ "~J1(4.3)\tリサ…… ~N\n\t\t\tダリア・ギレスピーという女を ~N\n\t\t\t知ってるか？ ",
    /* 26 */ "~J1(3.2)\tええ　あのおかしなギレスピーおばさんね ",
    /* 27 */ "~J1(2.3)\tこの町ではちょっとした有名人よ ",
    /* 28 */ "~J1(4.0)\tめったに人前には姿を見せないから ~N\n\t\t\t詳しいことは知らないけど ",
    /* 29 */ "~J1(5.2)\t火事で子供を亡くしてから ~N\n\t\t\tおかしくなったって聞いたことがあるわね ~E ",
    /* 30 */ "~J1(5.1)\t彼女が言ってたんだ ~N\n\t\t\t町を闇が蝕んでいると ",
    /* 31 */ "~J1(2.7)\t何のことだ？ ~N\n\t\t\t心当たりはないか？ ",
    /* 32 */ "~J1(3.5)\t町を闇が……？ ",
    /* 33 */ "~J1(2.1)\tそうね…… ",
    /* 34 */ "~J1(2.9)\tこの町が観光地になる前…… ",
    /* 35 */ "~J1(3.4)\t町は古くから住む静かな生活を ~N\n\t\t\t求める人たちばかりだったのよ ",
    /* 36 */ "~J1(4.1)\tその人たちの間ではね　独特の ~N\n\t\t\t奇妙な信仰みたいなものがあったの ~E ",
    /* 37 */ "~J1(4.8)\tオカルトじみた…… ~N\n\t\t\t魔術だとかいったような……そんな感じの ",
    /* 38 */ "~J1(5.0)\t町の若い人が突然いなくなると ~N\n\t\t\t土地の神様に召されたとか言いあって ",
    /* 39 */ "~J1(3.7)\t昔はそういったことが多かったらしいのよ ",
    /* 40 */ "~J1(3.5)\t観光地になる前は本当に ~N\n\t\t\t何もないところだったから…… ",
    /* 41 */ "~J1(4.5)\t何かのせいにしないと ~N\n\t\t\t不安でしようがなかったのね ",
    /* 42 */ "~J1(3.8)\t新しく入って来る人が増えてからは ~N\n\t\t\tすっかり息を潜めてしまったらしいけれど ",
    /* 43 */ "~J1(1.8)\t土地の信仰…… ~E ",
    /* 44 */ "~J1(4.8)\t最後にそのことが話題に ~N\n\t\t\tあがったのは何年前かしら…… ",
    /* 45 */ "~J1(4.4)\tこの町を開発しようとする団体の ~N\n\t\t\t役員たちに事故死が相次いだとき ",
    /* 46 */ "~J1(2.0)\tたたりだとか噂になって…… ",
    /* 47 */ "~J1(4.2)\tあんまり関係なかったわね ~N\n\t\t\tごめんなさい ~E ",
    /* 48 */ "~J0(1.1)\tハリー！ ~E ",
    /* 49 */ "~J0(1.6)\t無事だったか　良かった ~E ",
    /* 50 */ "~J0(4.6)\t良かった　帰ってきてくれて！ ~N\n\t\t\t一人で怖かったわ ",
    /* 51 */ "~J0(2.5)\tわたしもだ ~N\n\t\t\t会えてほっとしたよ ",
    /* 52 */ "~J0(2.0)\t不安でしかたなかったんだ ~E ",
    /* 53 */ "~J1(3.3)\tリサ　湖の方へ行くにはどうすればいい？ ",
    /* 54 */ "~J1(3.0)\t湖？ ~N\n\t\t\tバックマン通りを行けばいいわ ",
    /* 55 */ "~J1(1.5)\tその道は通れないんだ ",
    /* 56 */ "~J1(2.3)\t他に道はないわ ",
    /* 57 */ "~J1(1.3)\t何だって？ ",
    /* 58 */ "~J1(2.7)\tどうにかならないのか？ ~N\n\t\t\t他に方法は…… ~E ",
    /* 59 */ "~J1(3.0)\t待って！　道はあるかもしれないわ！ ",
    /* 60 */ "~J1(0.6)\tどこだ！ ",
    /* 61 */ "~J1(3.3)\tあたしが子供のころ通っていた ~N\n\t\t\t学校のそばに給水施設があるのよ ",
    /* 62 */ "~J1(2.3)\t今はもう使われていなかったと思うけど…… ",
    /* 63 */ "~J1(4.2)\tそこに点検用の地下への入口があって ",
    /* 64 */ "~J1(3.2)\tそのころ聞いた話では　確か湖の方まで ~N\n\t\t\tつながってるって ~E ",
    /* 65 */ "~J1(0.9)\t本当か？ ",
    /* 66 */ "~J1(2.3)\tそこから湖まで行けるんだな？ ",
    /* 67 */ "~J1(3.1)\t絶対とは言い切れないわ ~N\n\t\t\t入ったことがないんだもの ",
    /* 68 */ "~J1(2.8)\tそれに金網で囲まれていて ~N\n\t\t\t入れなかったはずよ ",
    /* 69 */ "~J1(2.3)\t少しでも望みがあるのなら ~N\n\t\t\t行ってみる価値はある ~E  ",
    /* 70 */ "~J1(2.0)\tハリー　行かないで！ ",
    /* 71 */ "~J1(4.2)\t一人になるのが怖いのよ ~N\n\t\t\tどうしようもなく怖いの ",
    /* 72 */ "~J1(1.6)\tきみも一緒に行くか？ ",
    /* 73 */ "~J1(2.6)\tここもいつまでも安全とは限らないし ",
    /* 74 */ "~J1(3.6)\t保証はできないが ~N\n\t\t\t一人ぐらいなら何とか守れるだろう ",
    /* 75 */ "~J1(4.2)\t駄目……ここから離れては ~N\n\t\t\tいけないような気がするの ",
    /* 76 */ "~J1(4.0)\tねえ　ハリー　怖いのよ ~N\n\t\t\tそれにすごく寒いの ",
    /* 77 */ "~J1(3.8)\t……もうしばらくここにいてくれ ",
    /* 78 */ "~J1(3.0)\t娘を見つけ出したら　必ず戻ってくる ~E ",
    /* 79 */ "~J0(1.5)\tハリー…… ~E ",
    /* 80 */ "\t特に気になるものはない ~E ",
    /* 81 */ "\t役に立ちそうなものはない ~E "
#endif
};

void func_800D1910(void) // 0x800D1910
{
    Text_Debug_PositionSet(30, 30);
    ClearImage(&D_800D3720, 0, 0, 0);

    // GPU packet setup.
    {
        typedef struct
        {
            SPRT*      sprt_0;
            DR_TPAGE*  tpage_4;
            DR_STP*    stp_8;
            DR_AREA*   area_C;
            DR_OFFSET* offset_10;
        } s_ScratchData;

        s32            layer;
        s32            i;
        s_ScratchData* scratch;

        scratch = PSX_SCRATCH_ADDR(0);

        scratch->sprt_0 = (SPRT*)GsOUT_PACKET_P;
        for (layer = 0; layer < 4; layer++)
        {
            for (i = 0; i < 2; i++)
            {
                s32 colorVal;
                u8  tpageAbr;

                // @hack Might have been a switch instead?
                setXY0Fast(scratch->sprt_0,
                           (i * 256) - ((layer < 2) ? ((layer == 1) ? 159 : 161) : 160),
                           ((layer < 2) ? -112 : ((layer == 2) ? -111 : -113)));

                setUV0(scratch->sprt_0, 0, 0);

                switch (layer)
                {
                    case 0:
                        colorVal = 32;
                        tpageAbr = 1;
                        break;

                    case 1:
                        colorVal = 64;
                        tpageAbr = 0;
                        break;

                    default:
                        colorVal = 128;
                        tpageAbr = 0;
                }

                if (layer == 3)
                {
                    setRGBC0(scratch->sprt_0, 128, 128, 128, PRIM_RECT | RECT_TEXTURE);
                }
                else
                {
                    setRGBC0(scratch->sprt_0, colorVal, colorVal, colorVal, PRIM_RECT | RECT_TEXTURE | RECT_BLEND);
                }

                setWH(scratch->sprt_0, (i == 0) ? 256 : 64, FRAMEBUFFER_HEIGHT_PROGRESSIVE);
                addPrimFast(&g_OrderingTable0[g_ActiveBufferIdx].org[2], scratch->sprt_0, 4);

                scratch->sprt_0++;
                scratch->tpage_4 = (DR_TPAGE*)scratch->sprt_0;

                setDrawTPage(scratch->tpage_4, 0, 0, getTPageN(2, tpageAbr, (i * 4) + 21, 1));

                addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[2], scratch->tpage_4);
                scratch->tpage_4++;
                scratch->sprt_0 = (SPRT*)scratch->tpage_4;
            }
        }

        // First `DR_AREA`.
        scratch->area_C = (DR_AREA*)scratch->sprt_0;
        SetDrawArea(scratch->area_C, &D_800D3710[g_ActiveBufferIdx == 0 ? 1 : 0]);
        addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[2], scratch->area_C);
        scratch->area_C++;

        // Second `DR_AREA`.
        SetDrawArea(scratch->area_C, &D_800D3720);
        addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[ORDERING_TABLE_SIZE - 1], scratch->area_C);
        scratch->area_C++;

        // First `DR_OFFSET`.
        scratch->offset_10 = (DR_OFFSET*)scratch->area_C;
        SetDrawOffset(scratch->offset_10, &D_800D3728[g_ActiveBufferIdx == 0 ? 1 : 0]);
        addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[2], scratch->offset_10);
        scratch->offset_10++;

        // Second `DR_OFFSET`.
        SetDrawOffset(scratch->offset_10, &D_800D3730);
        addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[ORDERING_TABLE_SIZE - 1], scratch->offset_10);
        scratch->offset_10++;

        // First `DR_STP` (disabled).
        scratch->stp_8 = (DR_STP*)scratch->offset_10;
        SetDrawStp(scratch->stp_8, 0);
        addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[2], scratch->stp_8);
        scratch->stp_8++;

        // Second `DR_STP` (enabled).
        SetDrawStp(scratch->stp_8, 1);
        addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[ORDERING_TABLE_SIZE - 1], scratch->stp_8);
        scratch->stp_8++;

        GsOUT_PACKET_P = (PACKET*)scratch->stp_8;
    }

    if (Pad_ButtonClickedBind(skip) &&
        g_SysWork.sysStateSteps[0] > 0 && g_SysWork.sysStateSteps[0] < 14)
    {
        SysWork_StateStepSet(0, 14);
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            D_800D6EF8 = 0;

            Fs_QueueStartRead(FILE_ANIM_HSPTL4_DMS, FS_BUFFER_15);
            Fs_QueueWaitForEmpty();
            Dms_HeaderFixOffsets((s_DmsHeader*)FS_BUFFER_15);

            g_Cutscene_Timer = Q12(0.0f);

            Chara_Load(0, Chara_Lisa, &g_SysWork.npcBoneCoordBuffer[0], CHARA_FORCE_FREE_ALL, NULL, NULL);
            Chara_ProcessLoads();
            Chara_Spawn(Chara_Lisa, 0, Q12(70.0f), Q12(150.0f), 0, 3);
            func_8003D03C();
            Player_EmptyWeaponHandSet();

            D_800D6EF8 = 0;

            // TODO: Find correct order. Looking at other functions, seems to be random.
            g_SysWork.lightBoneCoord     = NULL;
            g_SysWork.lensFlareBoneCoord = NULL;
            g_SysWork.lightIntensity     = Q12(1.0f);
            Math_Vector3Set(&g_SysWork.lightPosition, Q12(57.0f), Q12(-3.0f), Q12(141.8f));

            Math_SetSVectorFastSum(&g_SysWork.lightRotation, Q12_ANGLE(-90.0f), Q12_ANGLE(0.0f), Q12_ANGLE(0.0f));

            func_8008D438();

            CutsceneBorder_ForceShow();
            Screen_FadeTimestepReset();
            g_SysWork.sysFlags |= SysFlag_CutsceneActive;

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &PLAYER_CHARA(), 117, false);
            D_800D37C0 = 0;

            WorldEnv_PresetsSet(15, 15);
            SysWork_StateStepIncrement(0);
            g_WorldObject0.position.vz = Q12(141.0f);
            break;

        case 1:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimLock, &PLAYER_CHARA(), 0, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 9, false);

            D_800D37C0 = 1;

            SysWork_StateStepIncrement(0);

        case 2:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(1.0f), false);
            break;

        case 3:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimUnlock, &PLAYER_CHARA(), 0, false);
            SysWork_StateStepIncrement(0);
            break;

        case 4:
            Event_WaitTimer(Q12(1.2f), false);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(8.0f), Q12(0.0f), Q12(23.0f), true, false);
            break;

        case 5:
            Event_DisplayMapMsgWithAudio(15, &D_800D6EF8, &D_800D3734); // "Where am I?"
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(8.0f), Q12(0.0f), Q12(23.0f), true, false);
            break;

        case 6:
            g_Cutscene_Timer               = Q12(24.0f);
            g_WorldObject0.position.vz = Q12(140.8f);

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &PLAYER_CHARA(), 135, false);
            SysWork_StateStepIncrement(0);

        case 7:
            Event_DisplayMapMsgWithAudio(16, &D_800D6EF8, &D_800D3734); // "Harry. Lisa... Then I'm in the hospital."
            break;

        case 8:
            Event_DisplayMapMsgWithAudio(25, &D_800D6EF8, &D_800D3734);
            break;

        case 9:
            Event_DisplayMapMsgWithAudio(30, &D_800D6EF8, &D_800D3734);
            break;

        case 10:
            Event_DisplayMapMsgWithAudio(37, &D_800D6EF8, &D_800D3734);
            break;

        case 11:
            Event_DisplayMapMsgWithAudio(44, &D_800D6EF8, &D_800D3734);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(25.0f), Q12(46.0f), true, false);
            g_WorldObject0.position.vz = Q12(140.95f);
            break;

        case 12:
            Event_CharaAnimPlayToEnd(&g_SysWork.npcs[0], 10);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(25.0f), Q12(46.0f), true, false);
            break;

        case 13:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.8f), false);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(25.0f), Q12(46.0f), true, false);
            break;

        case 14:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        default:
            // Return to gameplay.
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimUnlock, &PLAYER_CHARA(), 0, false);
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            Chara_ModelCharaIdClear(&g_SysWork.npcs[0], 0, 0);

            Savegame_EventFlagSet(EventFlag_338);

            SD_Call(19);
            func_8003D01C();
            Player_WeaponAttackRestore();
            break;
    }

    g_SysWork.npcs[0].position.vy = Q12(0.0f);

    if (g_Cutscene_Timer >= Q12(0.0f))
    {
        Dms_CharacterTransformGet(&PLAYER_CHARA().position, &PLAYER_CHARA().rotation, "HERO", g_Cutscene_Timer, (s_DmsHeader*)FS_BUFFER_15);

        if (D_800D37C0 != 0)
        {
            Dms_CharacterTransformGet(&g_SysWork.npcs[0].position, &g_SysWork.npcs[0].rotation, "LISA", g_Cutscene_Timer, (s_DmsHeader*)FS_BUFFER_15);
        }

        vcChangeProjectionValue(Dms_CameraTargetsGet(&g_Cutscene_CameraPositionTarget, &g_Cutscene_CameraLookAtTarget, NULL, g_Cutscene_Timer, (s_DmsHeader*)FS_BUFFER_15));
        vcUserCamTarget(&g_Cutscene_CameraPositionTarget, NULL, true);
        vcUserWatchTarget(&g_Cutscene_CameraLookAtTarget, NULL, true);
    }
}

void func_800D23E4(void) // 0x800D23E4
{
    VECTOR3  lightIntPos;
    SVECTOR3 unused;

    if (Pad_ButtonClickedBind(skip) &&
        g_SysWork.sysStateSteps[0] > 0 && g_SysWork.sysStateSteps[0] < 22)
    {
        SysWork_StateStepSet(0, 26);
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Game_TurnFlashlightOn();
            D_800D6EF8 = 0;

            Fs_QueueStartRead(FILE_ANIM_HSPTL5_DMS, FS_BUFFER_15);
            Fs_QueueWaitForEmpty();
            Dms_HeaderFixOffsets(FS_BUFFER_15);

            g_Cutscene_Timer = Q12(0.0f);

            // Load Lisa character.
            Chara_Load(0, Chara_Lisa, &g_SysWork.npcBoneCoordBuffer[0], CHARA_FORCE_FREE_ALL, NULL, NULL);
            Chara_ProcessLoads();
            Chara_Spawn(Chara_Lisa, 0, Q12(60.0f), Q12(140.0f), Q12_ANGLE(0.0f), 3);

            func_8003D03C();
            Player_EmptyWeaponHandSet();

            D_800D6EF8 = 0;

            g_SysWork.lightBoneCoord     = NULL;
            g_SysWork.lensFlareBoneCoord = NULL;
            g_SysWork.lightIntensity     = Q12(1.2f);

            func_8008D438();

            CutsceneBorder_ForceShow();
            Screen_FadeTimestepReset();

            D_800D37C1            = 0;
            g_SysWork.sysFlags |= SysFlag_CutsceneActive;
            Sfx_WithFlagsPlay(Sfx_DoorClose0, &Q12_VECTOR3(54.6f, -1.2f, 141.5f), Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);
            break;

        case 1:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 11, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &PLAYER_CHARA(), 51, false);
            SysWork_StateStepIncrement(0);
            break;

        case 2:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimLock, &g_SysWork.npcs[0], 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.0f), false);
            break;

        case 3:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimUnlock, &g_SysWork.npcs[0], 0, false);
            SysWork_StateStepIncrement(0);

        case 4:
            Event_DisplayMapMsgWithAudio(48, &D_800D6EF8, &D_800D3778);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(6.5f), Q12(0.0f), Q12(20.0f), true, false);
            break;

        case 5:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(6.5f), Q12(0.0f), Q12(20.0f), true, true);
            break;

        case 6:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 12, false);
            g_SysWork.lightIntensity = Q12(1.0f);
            SysWork_StateStepIncrement(0);

        case 7:
            Event_DisplayMapMsgWithAudio(49, &D_800D6EF8, &D_800D3778);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(8.0f), Q12(21.0f), Q12(54.0f), true, false);

            if (g_Cutscene_Timer >= Q12(32.0f) && D_800D37C1 == 0)
            {
                Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &PLAYER_CHARA(), 134, false);
                D_800D37C1++;
            }
            break;

        case 8:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(8.0f), Q12(21.0f), Q12(54.0f), true, true);

            if (g_Cutscene_Timer >= Q12(32.0f) && D_800D37C1 == 0)
            {
                Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &PLAYER_CHARA(), 134, false);
                D_800D37C1++;
            }
            break;

        case 9:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &PLAYER_CHARA(), 51, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 5, false);

            g_SysWork.lightIntensity = Q12(1.2f);
            g_Cutscene_Timer         = Q12(55.0f);

            SysWork_StateStepIncrement(0);

        case 10:
            Event_DisplayMapMsgWithAudio(50, &D_800D6EF8, &D_800D3778);
            break;

        case 11:
            g_Cutscene_Timer = Q12(56.0f);
            SysWork_StateStepIncrement(0);

        case 12:
            Event_DisplayMapMsgWithAudio(53, &D_800D6EF8, &D_800D3778);
            break;

        case 13:
            Event_WaitTimer(Q12(2.0f), false);
            break;

        case 14:
            Event_DisplayMapMsgWithAudio(59, &D_800D6EF8, &D_800D3778);
            break;

        case 15:
            Event_DisplayMapMsgWithAudio(65, &D_800D6EF8, &D_800D3778);
            break;

        case 16:
            g_Cutscene_Timer         = Q12(57.0f);
            g_SysWork.lightIntensity = Q12(1.0f);
            Event_DisplayMapMsgWithAudio(70, &D_800D6EF8, &D_800D3778);
            break;

        case 17:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 13, false);
            g_SysWork.lightIntensity = Q12(1.2f);
            SysWork_StateStepIncrement(0);

        case 18:
            Event_DisplayMapMsgWithAudio(79, &D_800D6EF8, &D_800D3778);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(58.0f), Q12(126.0f), true, false);
            break;

        case 19:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(58.0f), Q12(126.0f), true, true);
            break;

        case 20:
            Model_AnimFlagsClear(&PLAYER_CHARA().model, 2);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &PLAYER_CHARA(), 53, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 21, false);
            SysWork_StateStepIncrement(0);

        case 21:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(127.0f), Q12(151.0f), false, true);
            break;

        case 22:
            Sfx_WithFlagsPlay(Sfx_DoorOpen0, &Q12_VECTOR3(54.6f, -1.2f, 141.5f), Q8(0.5f), SfxFlag_None);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &PLAYER_CHARA(), 51, false);
            SysWork_StateStepIncrement(0);

        case 23:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(127.0f), Q12(160.0f), false, true);
            break;

        case 24:
            Sfx_WithFlagsPlay(Sfx_DoorClose0, &Q12_VECTOR3(54.6f, -1.2f, 141.5f), Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 25:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(127.0f), Q12(200.0f), true, true);
            break;

        case 26:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);

            SD_Call(19);

            Model_AnimFlagsSet(&PLAYER_CHARA().model, 2);

            func_8003D01C();
            Chara_ModelCharaIdClear(&g_SysWork.npcs[0], 0, 0);

            Savegame_EventFlagSet(EventFlag_MapMark_AltHospital1F_RightEntranceBroken);
            Savegame_EventFlagSet(EventFlag_340);

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);

            func_8008D448();
            Game_FlashlightAttribsSet();

            g_SysWork.lightIntensity = Q12(1.0f);
            g_Cutscene_Timer         = NO_VALUE;

            vcReturnPreAutoCamWork(false);
            func_8003D01C();
            Player_WeaponAttackRestore();
            break;
    }

    if (g_Cutscene_Timer >= Q12(0.0f))
    {
        Dms_CharacterTransformGet(&PLAYER_CHARA().position, &PLAYER_CHARA().rotation, "HERO", g_Cutscene_Timer, FS_BUFFER_15);
        Dms_CharacterTransformGet(&g_SysWork.npcs[0].position, &g_SysWork.npcs[0].rotation, "LISA", g_Cutscene_Timer, FS_BUFFER_15);
        vcChangeProjectionValue(Dms_CameraTargetsGet(&g_Cutscene_CameraPositionTarget, &g_Cutscene_CameraLookAtTarget, NULL, g_Cutscene_Timer, FS_BUFFER_15));
        vcUserCamTarget(&g_Cutscene_CameraPositionTarget, NULL, true);
        vcUserWatchTarget(&g_Cutscene_CameraLookAtTarget, NULL, true);

        // "LIGHT", cutscene light position?
        Dms_CharacterTransformGet(&g_SysWork.lightPosition, &unused, "LIGHT", g_Cutscene_Timer, FS_BUFFER_15);

        // "L_INT", interior light or intersection point?
        Dms_CharacterTransformGet(&lightIntPos, &unused, "L_INT", g_Cutscene_Timer, FS_BUFFER_15);

        // Set light rotation.
        g_SysWork.lightRotation.vx = -ratan2(lightIntPos.vy - g_SysWork.lightPosition.vy, Math_Vector2MagCalcSafeQ6(lightIntPos.vx - g_SysWork.lightPosition.vx, lightIntPos.vz - g_SysWork.lightPosition.vz));
        g_SysWork.lightRotation.vy =  ratan2(lightIntPos.vx - g_SysWork.lightPosition.vx, lightIntPos.vz - g_SysWork.lightPosition.vz);
        g_SysWork.lightRotation.vz = Q12_ANGLE(0.0f);
    }
}

void Map_WorldObjectsInit(void) // 0x800D2D6C
{
    WorldObject_Init(&g_WorldObjectSavepad, g_CommonWorldObjectNames[1], 106.687f, -1.02f, 144.601f, 0.0f, 163.0f, 0.0f);
    WorldObject_PlacementInit(&g_WorldObject0, "ISU_HIDE", 57.8f, 0.0f, 0.0f);
}

void Map_WorldObjectsUpdate(void) // 0x800D2DF4
{
    if (PLAYER_IN_MAP_CHUNK(2, 3))
    {
        WorldObjects_Add(&g_WorldObjectSavepad.object, &g_WorldObjectSavepad.position, &g_WorldObjectSavepad.rotation);
    }

    if (PLAYER_IN_MAP_CHUNK(1, 3))
    {
        if (Savegame_EventFlagGet(EventFlag_337) && !Savegame_EventFlagGet(EventFlag_338))
        {
            WorldObjects_Add(&g_WorldObject0.object, &g_WorldObject0.position, &SVECTOR3_Zero);
        }
    }
}
