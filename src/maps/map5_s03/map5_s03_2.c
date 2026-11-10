#include "bodyprog/bodyprog.h"
#include "bodyprog/chara/dms.h"
#include "bodyprog/events/event_data.h"
#include "bodyprog/gfx/map_effects.h"
#include "bodyprog/items/item_screens.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player/player.h"
#include "main/rng.h"
#include "maps/map5/map5_s03.h"
#include "maps/common/characters/kaufmann.h"
#include "maps/common/particle.h"
#include "maps/common/characters/player.h"

#include "../src/maps/common/particles/particle.c"

#include "../src/maps/common/characters/player.c"

#include "../src/maps/common/characters/kaufmann.c"

#include "../src/maps/common/chara_util.c" // 0x800D054C

#include "../src/maps/common/map_util.c" // 0x800D0F5C

#include "maps/common/shared/Map_RoomBgmInit_CheckCond.h" // 0x800D1004

#include "maps/common/shared/Map_RoomBgmInit_0_s02_CondTrue.h" // 0x800D1038

#include "maps/common/shared/Map_RoomBgmInit_0_s02_CondFalse.h" // 0x800D10B4

void GameBoot_LoadScreen_StageString(void) {} // 0x800D1340

#include "maps/common/shared/Event_CutsceneTimerAdvance.h" // 0x800D1348

#include "maps/common/shared/MapEvent_DoorJammed.h" // 0x800D13E0

#include "maps/common/shared/MapEvent_DoorLocked.h" // 0x800D1474

#include "maps/common/shared/MapEvent_DoorUnlocked.h" // 0x800D1508

const char* MAP_MESSAGES[] = {
    #include "maps/common/shared/map_msg_common.h"
#if VERSION_REGION_IS(NTSC)
    /* 15 */ "\tI've_got_a_ ~C2 Motorcycle_key ~C7 . ~E ",
    /* 16 */ "\tThere's_a_crack. ~E ",
    /* 17 */ "\tThere's_a_crack. ~N\n\tSomething_glints_inside. ",
    /* 18 */ "\tThe_crack_is_too_small ~N\n\tto_reach_in. ~E ",
    /* 19 */ "\tAn_old_motorcycle. ",
    /* 20 */ "\tLooks_like_it's_been_sitting ~N\n\there_for_years. ",
    /* 21 */ "\t~J1(1.5) ",
    /* 22 */ "\tThat's_weird. ",
    /* 23 */ "\tThe_dust_is_wiped_away ~N\n\tjust_around_the_gas_tank_cap. ~E ",
    /* 24 */ "\tA_small_glass_vial, ~N\n\twrapped_in_a_plastic_bag. ",
    /* 25 */ "\tWhat's_this? ~E ",
    /* 26 */ "\tIt's_like_the_busted_vial ~N\n\tI_found_in_the_director's_office ~N\n\tat_the_hospital. ~E ",
    /* 27 */ "~J0(1.3)\tGive_me_that! ",
    /* 28 */ "~J0(1.2)\tWhat_is_this? ~E ",
    /* 29 */ "~J1(2.0)\tThat's_none_of_your_business! ",
    /* 30 */ "~J1(3.3)\tInstead_of_messing_with_that, ~N\n\t\t\thow_about_coming_up_with_a_way ~N\n\t\t\tto_get_out_of_here? ",
    /* 31 */ "~J1(0.9)\tHumph. ~E ",
    /* 32 */ "~J1(2.6)\tYou_shouldn't_be_hanging ~N\n\t\t\taround_here_goofing_off! ",
    /* 33 */ "~J1(1.3)\tWhat_do_you_think ~N\n\t\t\tyou_are_doing? ",
    /* 34 */ "~J1(3.0)\tYou_want_to_get_yourself_killed!? ~N\n\t\t\tGet_outta_here! ",
    /* 35 */ "~J1(1.9)\tOkay,_take_it_easy. ~E ",
    /* 36 */ "~J1(3.2)\tUnless_you_want_to_die, ~N\n\t\t\tkeep_your_mind_on_business. ",
    /* 37 */ "~J1(0.8)\tGot_it? ~E ",
    /* 38 */ "~J1(3.3)\tThat_guy's_gotta_be_involved ~N\n\t\t\tin_the_local_drug_racket. ",
    /* 39 */ "~J1(3.1)\tMan,_was_he_pissed. ~N\n\t\t\tAnd_in_such_a_rush,_too. ",
    /* 40 */ "~J1(1.8)\tThat_was_probably_dope ~N\n\t\t\tin_the_bottle. ",
    /* 41 */ "~J1(3.9)\tAnyway,_better_let_him_do_as_he ~N\n\t\t\tpleases._There's_more_than ~N\n\t\t\tmy_life_at_stake. ~E ",
    /* 42 */ "~J1(2.0)\tI_guess_I_wasted_my_time. ",
    /* 43 */ "~J1(2.0)\tBetter_hurry.... ~N\n\t\t\tI'm_worried_about_Cheryl. ~E ",
    /* 44 */ "~J1(3.1)\tMan,_was_he_pissed. ~N\n\t\t\tAnd_in_such_a_rush,_too. ~E ",
    /* 45 */ "\tThere_is_a_newspaper. ~E ",
    /* 46 */ "\tNewspaper. ",
    /* 47 */ "\tIt's_the_same_date_as ~N\n\tthe_one_in_the_hospital... ~E ",
    /* 48 */ "~L4 ~C3 Investigation_stalled. ~N\n\t[PTV]_dealers_still_at_large. ~N\n\t~N\n\tSuspicious_deaths_continue. ~N\n\tLike_the_anti-drug_mayor, ~N\n\ta_narcotics_officer_dies_of ~N\n\ta_sudden_heart_failure_of ~N\n\tunknown_origin. ~E ",
    /* 49 */ "~C3\tSeptember_10 ",
    /* 50 */ "~C3\tTook_package. ~N\n\t\tTold_to_sit_on_it_awhile. ",
    /* 51 */ "~C3\tDon't_want_to_get_involved, ~N\n\t\tbut_can't_disobey... ",
    /* 52 */ "~C3\tHe's_probably_linked_to_the_death ~N\n\t\tof_the_mayor_and_others. ~E ",
    /* 53 */ "\tThere_is_a_ ~C2 Magnet ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 54 */ "\tUsed_the_ ~C2 Motorcycle_key ~C7 . ~E ",
    /* 55 */ "\tBatteries,_oil_cans, ~N\n\tand_other_items_line_the_shelves. ",
    /* 56 */ "\tA_fine_layer_of_~N\n\tdust_covers_everything, ~N\n\tas_if_long_abandoned. ~E ",
    /* 57 */ "\tThere's_a_radio ~N\n\ton_top_of_the_cabinet. ",
    /* 58 */ "\tIt's_completely_dead. ~N\n\tMaybe_it's_broken. ~E ",
    /* 59 */ "\tA_poster_is_on_the_wall. ~N\n\tDon't_look_at_that_now,_though. ~E ",
    /* 60 */ "\tThere's_a_TV_set. ",
    /* 61 */ "\tThe_power_must_be_out. ~N\n\tIt_doesn't_work. ~E ",
    /* 62 */ "\tThere's_a_guest_register. ~N\n\tNothing_special_written_here. ~E ",
    /* 63 */ "\tNothing_special. ~E ",
    /* 64 */ "\tThere's_a_vending_machine. ~N\n\tIt_doesn't_work. ~E ",
    /* 65 */ "\tNobody_inside. ~E ",
    /* 66 */ "\tDo_you_want_to_push ~N\n\tthe_shelf? ~S4 ",
    /* 67 */ "\tAn_old_motorcycle. ~E "
#elif VERSION_REGION_IS(NTSCJ)
    /* 15 */ "\t~C2 バイクの鍵 ~C7 を手に入れた ~E ",
    /* 16 */ "\t床に裂け目がある ~E ",
    /* 17 */ "\t床に裂け目がある ",
    /* 18 */ "\t奥で何か光っているが ~N\n\t裂け目が小さすぎて手は入らない ~E ",
    /* 19 */ "\t古びたバイクだ ~N\n\tもう何年も動かしていないだろう ",
    /* 20 */ "\t~J1(1.0) ",
    /* 21 */ "\t……変だな ",
    /* 22 */ "\tタンクのキャップ周辺だけ ~N\n\tほこりが少ない ~E ",
    /* 23 */ "\t\tビニールに包まれたガラスの小ビンがある ",
    /* 24 */ "\t\tこれは……？ ~E ",
    /* 25 */ "\t\t病院の院長室で割れていたビンと ~N\n\t\t同じビンのような気がする…… ~E ",
    /* 26 */ "~J0(1.3)\tそいつをこっちに渡せ！ ",
    /* 27 */ "~J0(1.2)\tこれはいったい何なんだ？ ~E ",
    /* 28 */ "~J1(2.0)\tきみには関係ないことだ！  ",
    /* 29 */ "~J1(3.9)\tそれより　この馬鹿げた状況から ~N\n\t\t\t逃げ出す算段はついたのか！？ ",
    /* 30 */ "~J1(0.9)\t……いや ~E ",
    /* 31 */ "~J1(2.6)\tなら　こんなところでのんびりしている ~N\n\t\t\t場合じゃないだろう！ ",
    /* 32 */ "~J1(1.3)\tいったい何をやっているんだ？ ",
    /* 33 */ "~J1(3.0)\tきみだって死にたくないだろ！？ ~N\n\t\t\tそれなら逃げ道を探すんだよ！ ",
    /* 34 */ "~J1(1.9)\tわかったよ ~E ",
    /* 35 */ "~J1(3.2)\t死にたくないなら余計なことは考えるな！ ",
    /* 36 */ "~J1(0.8)\tいいな！ ~E ",
    /* 37 */ "~J1(3.3)\tあの男が薬の売買に関わっていたのは ~N\n\t\t\tまちがいないだろう ",
    /* 38 */ "~J1(3.1)\tえらい剣幕だったな ~N\n\t\t\tずいぶんと　あせっていたようだが…… ",
    /* 39 */ "~J1(1.8)\tあのビンの中身も麻薬なのか？ ",
    /* 40 */ "~J1(3.9)\t何にしろ　勝手にするがいいさ ~N\n\t\t\t命より大事なようだからな ~E ",
    /* 41 */ "~J1(2.0)\t無駄な時間を過ごしてしまった ",
    /* 42 */ "~J1(2.0)\t急ごう　シェリルが心配だ ~E ",
    /* 43 */ "~J1(3.1)\tえらい剣幕だったな ~N\n\t\t\tずいぶんと　あせっていたようだが…… ~E ",
    /* 44 */ "\tテーブルの上に新聞が置いてある ~E ",
    /* 45 */ "\tテーブルの上に新聞が置いてある ",
    /* 46 */ "\t病院で切り抜かれていたやつと ~N\n\t同じ日付だな…… ~E ",
    /* 47 */ "~L4 ~C3\n\t捜査難航。 ~N\n\t麻薬“ＰＴＶ”の売人特定できず。~N\n\t~N\n\t相次ぐ怪死事件。 ~N\n\t麻薬売買の取締り強化を掲げた市長に続き、 ~N\n\t麻薬捜査官が原因不明の心臓発作で死亡。 ~E ",
    /* 48 */ "~C3\t９月１０日 ",
    /* 49 */ "~C3\t荷物を預けられて、 ~N\n\t\tしばらく隠しておくように言われた。 ",
    /* 50 */ "~C3\tやばいことに巻き込まれるのはごめんだが ~N\n\t\t……あいつには逆らえない。 ",
    /* 51 */ "~C3\t市長たちの事件もきっとあいつが ~N\n\t\t絡んでいるのに違いない。 ~E ",
    /* 52 */ "\t~C2 磁石 ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 53 */ "\t~C2 バイクの鍵 ~C7 を使った ~E ",
    /* 54 */ "\t棚にはバッテリーだのオイルの缶だの ~N\n\tいろいろなものが置かれている ",
    /* 55 */ "\tどれもが長い間　手をつけられなかったかの ~N\n\tようにほこりがうっすらと積もっている ~E ",
    /* 56 */ "\tラジオが棚の上に置かれている ~N\n\t壊れているのか何の反応も示さない ~E ",
    /* 57 */ "\t壁にポスターが貼られている ~N\n\t今はこんなものを見ている場合ではない ~E ",
    /* 58 */ "\tテレビが置いてある ",
    /* 59 */ "\t停電しているらしく ~N\n\t電源スイッチを押しても反応がない ~E ",
    /* 60 */ "\t宿泊者名簿が置かれている ~N\n\t特に気になることは書かれていない ~E ",
    /* 61 */ "\t特に役立ちそうなものはない ~E ",
    /* 62 */ "\t自動販売機がある ~N\n\t動いていないようだ ~E ",
    /* 63 */ "\t何もいない ~E ",
    /* 64 */ "\t棚を押しますか？ ~S4 ",
    /* 65 */ "\t古びたバイクがある ~E "
#endif
};

void MapEvent_CommonItemTake(void) // 0x800D159C
{
    u32 pickupType;
    s32 eventFlagIdx;

    pickupType   = CommonPickupItemId_FirstAidKit;
    eventFlagIdx = 0;

    switch (g_MapEventData->mapPointIdx)
    {
        case 37:
            pickupType   = CommonPickupItemId_HealthDrink;
            eventFlagIdx = EventFlag_M5S03_HealthDrink0;
            break;

        case 38:
            pickupType   = CommonPickupItemId_ShotgunShells;
            eventFlagIdx = EventFlag_M5S03_ShotgunShells;
            break;

        case 39:
            pickupType   = CommonPickupItemId_HealthDrink;
            eventFlagIdx = EventFlag_M5S03_HealthDrink1;
            break;
    }

    Event_CommonItemTake(pickupType, eventFlagIdx);
}

void func_800D1628(void) // 0x800D1628
{
    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_InvItemCmd(InvItemCmd_QueueLoad, InvItemId_MotorcycleKey, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 2, Q12(0.0f), false);

            // Warp player.
            g_SysWork.playerWork.player.position.vx = Q12(103.64f);
            g_SysWork.playerWork.player.position.vz = Q12(59.49f);
            g_SysWork.playerWork.player.rotation.vy = Q12_ANGLE(51.0f);

            // Warp camera.
            Event_CameraPositionSet(NULL, Q12(103.54f), Q12(-2.91f), Q12(59.2f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(NULL, Q12(104.65f), Q12(0.72f), Q12(60.44f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);

            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 187, false);
            SysWork_StateStepIncrement(0);

        case 3:
            Event_WaitTimer(Q12(3.8f), false);
            break;

        case 4:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimLock, &g_SysWork.playerWork.player, 0, false);
            Sfx_WithFlagsPlay(Sfx_Unk1598, &Q12_VECTOR3(104.25f, 0.1f, 60.0f), Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 5:
            Event_InvItemCmd(InvItemCmd_AwaitLoad, InvItemId_MotorcycleKey, 0, false);
            break;

        case 6:
            if (Gfx_PickupItemAnimate(InvItemId_MotorcycleKey))
            {
                Event_DisplayMapMsg(false, 15, 0, 0, 0, false);

                if (g_SysWork.sysStateSteps[0] != 6)
                {
                    SD_Call(Sfx_MenuConfirm);
                }
            }
            break;

        case 7:
            Event_CharaAnimUnlockPlayToEnd(&g_SysWork.playerWork.player);
            break;

        default:
            Player_ControlUnfreeze(false);

            SysWork_StateSetNext(SysState_Gameplay);
            vcReturnPreAutoCamWork(true);

            Player_ItemRemove(InvItemId_Magnet, 1);
            Event_InvItemCmd(InvItemCmd_AddItem, InvItemId_MotorcycleKey, 1, false);
            Savegame_EventFlagSet(EventFlag_390);

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            break;
    }
}

void func_800D1904(void) // 0x800D1904
{
    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_BgTextureCmd(BgTextureCmd_QueueRead, FILE_TIM_MOTELKEY_TIM, false);
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            Event_CharaAnimPlayToEnd(&g_SysWork.playerWork.player, 59);
            break;

        case 3:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 4:
            Event_BgTextureCmd(BgTextureCmd_AwaitLoad, 0, false);
            break;

        case 5:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.0f), false);
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            break;

        case 6:
            Event_DisplayMapMsg(false, 17, 0, 0, 0, false);
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            break;

        case 7:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.0f), false);
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            break;

        case 8:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.0f), false);
            break;

        case 9:
            Event_CharaAnimPlayToEnd(&g_SysWork.playerWork.player, 60);
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;
    }
}

void func_800D1A84(void) // 0x800D1A84
{
    Event_DisplayMapMsgWithBg(FILE_TIM_DIARYMTL_TIM, Q12(2.5f), Q12(2.0f), MAP_MSG(49));
    Savegame_EventFlagSet(EventFlag_398);
}

void func_800D1ACC(void) // 0x800D1ACC
{
    Event_ItemTake(InvItemId_Magnet, DEFAULT_PICKUP_ITEM_COUNT, EventFlag_M5S03_PickupMagnet, MAP_MSG(53));
}

void func_800D1AF8(void) // 0x800D1AF8
{
    SVECTOR3 unused;
    VECTOR3  lightIntPos;

    if (Pad_ButtonClickedBind(skip) &&
        g_SysWork.sysStateSteps[0] >= 14 && g_SysWork.sysStateSteps[0] < 29)
    {
        SysWork_StateStepSet(0, 32);
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();

            D_800D6F54 = 0;
            g_Cutscene_Timer = NO_VALUE;

            func_8003D03C();
            Player_EmptyWeaponHandSet();

            WorldGfx_PlayerHeldItemSet(InvItemId_CutsceneAglaophotis);

            D_800D3C40 = Fs_QueueStartReadTim(FILE_TIM_BICITEM_TIM, FS_BUFFER_1, &g_ItemInspectionImg);
            Fs_QueueStartRead(FILE_ANIM_GARAGE1_DMS, FS_BUFFER_17);

            Chara_Load(0, Chara_Kaufmann, g_SysWork.npcBoneCoordBuffer, CHARA_FORCE_FREE_ALL, NULL, NULL);
            Sfx_WithFlagsPlay(Sfx_Unk1597, &Q12_VECTOR3(101.706f, -0.889f, 19.7684f), Q8(0.5f), SfxFlag_None);

            SysWork_StateStepIncrement(0);

        case 1:
            Event_DisplayMapMsg(false, MAP_MSG(54), 0, 0, 0, false); // "Used the Motorcycle Key."
            break;

        case 2:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 3:
            Sfx_WithFlagsPlay(Sfx_Unk1596, &Q12_VECTOR3(101.706f, -0.889f, 19.7684f), Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 4:
            if (Fs_QueueIsEntryLoaded(D_800D3C40))
            {
                SysWork_StateStepIncrement(0);
                func_8003D01C();
            }
            break;

        case 5:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.0f), false);
            break;

        case 6:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_DisplayMapMsg(false, MAP_MSG(24), 0, 0, 0, false); // "A small glass vial, wrapped in a plastic bag."

            if (g_SysWork.sysStateSteps[0] != 6 && !Savegame_EventFlagGet(EventFlag_204))
            {
                // Skip next step if event flag not set.
                SysWork_StateStepSet(0, 8);
            }
            break;

        case 7:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_DisplayMapMsg(false, MAP_MSG(26), 0, 0, 0, false); // "It's like the busted vial I found in the director's office at the hospital."
            break;

        case 8:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            break;

        case 9:
            Chara_ProcessLoads();
            Dms_HeaderFixOffsets(FS_BUFFER_17);
            SysWork_StateStepIncrement(0);

        case 10:
            Bgm_CrossfadeToTrack(BgmCmd_Track34);
            break;

        case 11:
            CutsceneBorder_ForceShow();
            g_SysWork.sysFlags |= SysFlag_CutsceneActive;

            func_8008D438();

            g_SysWork.lightIntensity = Q12(1.0f);
            g_SysWork.lightBoneCoord = NULL;
            g_SysWork.lensFlareBoneCoord = NULL;

            g_Cutscene_Timer = Q12(0.0f);

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 159, false);

            SysWork_StateStepIncrement(0);
            break;

        case 12:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.0f), false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimLock, &g_SysWork.playerWork.player, 0, false);
            break;

        case 13:
            Chara_Spawn(Chara_Kaufmann, 0, Q12(140.5f), Q12(23.0f), Q12_ANGLE(0.0f), 3);

            Sfx_WithFlagsPlay(Sfx_Unk1595, &Q12_VECTOR3(103.0f, -1.2f, 17.6f), Q8(0.5f), SfxFlag_None);

            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimUnlock, &g_SysWork.playerWork.player, 0, false);

            Savegame_EventFlagSet(EventFlag_395);
            SysWork_StateStepIncrement(0);

        case 14:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(0.0f), Q12(17.0f), true, true);
            Model_AnimFlagsClear(&g_SysWork.npcs[0].model, 2);
            break;

        case 15:
            Event_CharaAnimPlayToEnd(&g_SysWork.npcs[0], 16);
            Model_AnimFlagsSet(&g_SysWork.npcs[0].model, 2);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(18.0f), Q12(38.0f), true, false);
            break;

        case 16:
            Event_DisplayMapMsgWithAudio(MAP_MSG(27), &D_800D6F54, &D_800D3BDC);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(18.0f), Q12(38.0f), true, false);
            break;

        case 17:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 169, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 15, false);
            SysWork_StateStepIncrement(0);

        case 18:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(39.0f), Q12(75.0f), true, true);
            break;

        case 19:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 51, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 5, false);
            func_8003D03C();

            g_Cutscene_Timer = Q12(76.0f);

            SysWork_StateStepIncrement(0);

        case 20:
            Event_DisplayMapMsgWithAudio(MAP_MSG(29), &D_800D6F54, &D_800D3BDC);
            break;

        case 21:
            Event_DisplayMapMsgWithAudio(MAP_MSG(32), &D_800D6F54, &D_800D3BDC);
            g_Cutscene_Timer = Q12(77.0f);
            break;

        case 22:
            g_Cutscene_Timer = Q12(78.0f);
            Event_DisplayMapMsgWithAudio(MAP_MSG(36), &D_800D6F54, &D_800D3BDC);
            break;

        case 23:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 72, false);
            SysWork_StateStepIncrement(0);

        case 24:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(78.0f), Q12(98.0f), true, true);
            break;

        case 25:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 1, false);
            Player_WeaponAttackRestore();
            WorldGfx_PlayerPrevHeldItem(&g_SysWork.playerCombat);
            Player_EmptyWeaponHandSet();
            SysWork_StateStepIncrement(0);

        case 26:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(99.0f), Q12(121.0f), true, true);
            break;

        case 27:
            Chara_ModelCharaIdClear(&g_SysWork.npcs[0], 0, 0);
            Sfx_WithFlagsPlay(Sfx_Unk1595, &Q12_VECTOR3(103.0f, -1.2f, 17.6f), Q8(0.5f), SfxFlag_None);

            Savegame_EventFlagClear(EventFlag_395);

            g_Cutscene_Timer = Q12(122.0f);

            SysWork_StateStepIncrement(0);

        case 28:
            if (Savegame_EventFlagGet(EventFlag_396) &&
                Savegame_EventFlagGet(EventFlag_397) &&
                Savegame_EventFlagGet(EventFlag_398))
            {
                Event_DisplayMapMsg(false, MAP_MSG(38), 0, 0, 0, false); // "That guy's gotta be involved in the local drug racket."
            }
            else
            {
                Event_DisplayMapMsg(false, MAP_MSG(44), 0, 0, 0, false); // "Man, was he pissed. And in such a rush, too."
            }
            break;

        case 29:
            vcReturnPreAutoCamWork(true);

            g_Cutscene_Timer = NO_VALUE;

            Player_WeaponAttackRestore();
            func_8003D01C();
            func_8008D448();
            Game_FlashlightAttribsSet();

            g_SysWork.lightIntensity = Q12(1.0f);

            SysWork_StateStepIncrement(0);

        case 30:
            Event_DisplayMapMsg(false, MAP_MSG(42), 0, 0, 0, false); // "I guess I wasted my time."
            break;

        case 31:
            SysWork_StateStepReset();
            break;

        case 32:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 33:
            Player_WeaponAttackRestore();

            WorldGfx_PlayerPrevHeldItem(&g_SysWork.playerCombat);
            Chara_ModelCharaIdClear(&g_SysWork.npcs[0], 0, 0);

            Fs_QueueWaitForEmpty();

            vcReturnPreAutoCamWork(true);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);

            g_Cutscene_Timer = Q12(122.0f);

            SysWork_StateStepIncrement(0);
            break;

        default:
            Player_ControlUnfreeze(false);

            SysWork_StateSetNext(SysState_Gameplay);

            vcReturnPreAutoCamWork(true);

            Savegame_EventFlagSet(EventFlag_M5S03_KaufmannHasMotorcycleVial);

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);

            g_Cutscene_Timer = NO_VALUE;

            func_8008D448();
            Game_FlashlightAttribsSet();

            g_SysWork.lightIntensity = Q12(1.0f);

            SD_Call(19);
            Player_WeaponAttackRestore();
            func_8003D01C();
            break;
    }

    if (g_Cutscene_Timer >= Q12(0.0f))
    {
        Dms_CharacterTransformGet(&g_SysWork.playerWork.player.position, &g_SysWork.playerWork.player.rotation, "HERO", g_Cutscene_Timer, FS_BUFFER_17);
        Dms_CharacterTransformGet(&g_SysWork.npcs[0].position, &g_SysWork.npcs[0].rotation, "KAU", g_Cutscene_Timer, FS_BUFFER_17);
        vcChangeProjectionValue(Dms_CameraTargetsGet(&g_Cutscene_CameraPositionTarget, &g_Cutscene_CameraLookAtTarget, NULL, g_Cutscene_Timer, FS_BUFFER_17));
        vcUserCamTarget(&g_Cutscene_CameraPositionTarget, NULL, true);
        vcUserWatchTarget(&g_Cutscene_CameraLookAtTarget, NULL, true);

        // "LIGHT", cutscene light position?
        Dms_CharacterTransformGet(&g_SysWork.lightPosition, &unused, "LIGHT", g_Cutscene_Timer, FS_BUFFER_17);

        // "L_INT", interior light or intersection point?
        Dms_CharacterTransformGet(&lightIntPos, &unused, "L_INT", g_Cutscene_Timer, FS_BUFFER_17);

        // Set light rotation.
        g_SysWork.lightRotation.vx = -ratan2(lightIntPos.vy - g_SysWork.lightPosition.vy, Math_Vector2MagCalcSafeQ6(lightIntPos.vx - g_SysWork.lightPosition.vx, lightIntPos.vz - g_SysWork.lightPosition.vz));
        g_SysWork.lightRotation.vy =  ratan2(lightIntPos.vx - g_SysWork.lightPosition.vx, lightIntPos.vz - g_SysWork.lightPosition.vz);
        g_SysWork.lightRotation.vz = Q12_ANGLE(0.0f);
    }
}

void func_800D2640(void) // 0x800D2640
{
    Event_DisplayMapMsgWithDimmedBg(FILE_TIM_NEWSP2_TIM, Q12(0.0f), Q12(0.0f), MAP_MSG(46), MAP_MSG(48));

    // @bugfix JAP0 & USA releases are missing code to set `EventFlag_M5S03_SeenSecondNewspaper` here,
    // causing later newspaper in `M7S01` & `M7S02` not to appear. Fixed in JAP1 and later releases.
#if VERSION_EQUAL_OR_NEWER(JAP1)
    Savegame_EventFlagSet(EventFlag_M5S03_SeenSecondNewspaper);
#endif
}

void func_800D2674(void) // 0x800D2674
{
    q19_12 moveDist;

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            Event_DisplayMapMsg(true, MAP_MSG(66), 4, 3, 0, false); // "Do you want to push the shelf?"
            break;

        case 3:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;

        case 4:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 2, Q12(0.0f), false);

            g_SysWork.playerWork.player.position.vx = Q12(104.17f);
            g_SysWork.playerWork.player.position.vz = Q12(59.5f);
            g_SysWork.playerWork.player.rotation.vy = Q12_ANGLE(0.0f);

            Event_CameraPositionSet(NULL, Q12(102.07f), Q12(-1.42f), Q12(58.77f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(NULL, Q12(105.14f), Q12(0.37f), Q12(60.61f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 105, false);
            SD_Call(Sfx_Unk1520);
            SysWork_StateStepIncrement(0);

        case 5:
        case 6:
            moveDist                                          = Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.14f);
            g_SysWork.playerWork.player.position.vz += moveDist;
            g_WorldObject_Movaches.position.vz      += moveDist;

            if (g_SysWork.sysStateSteps[0] == 5)
            {
                if (g_WorldObject_Movaches.position.vz > Q12(60.35f))
                {
                    SysWork_StateStepSet(0, 6);
                }

                D_800D3C44 = MIN(D_800D3C44 + (g_DeltaTime * 2), Q12(1.0f));
            }
            else
            {
                if (g_WorldObject_Movaches.position.vz > Q12(60.44f))
                {
                    moveDist = g_WorldObject_Movaches.position.vz - Q12(60.44f);

                    g_SysWork.playerWork.player.position.vz -= moveDist;
                    g_WorldObject_Movaches.position.vz      -= moveDist;
                }

                // `D_800D3C44 = MAX(D_800D3C44 - (g_DeltaTime * 2), Q12(0.0f));`?
                D_800D3C44 = ((D_800D3C44 - (g_DeltaTime * 2)) >= Q12(0.0f)) ? (D_800D3C44 - ((u16)g_DeltaTime * 2)) : Q12(0.0f);

                Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            }

            Sfx_WithFalloffAndPitchPlay(Sfx_Unk1538, &g_WorldObject_Movaches.position, D_800D3C44 >> 5, Q12(12.0f), 0);
            break;

        default:
            Savegame_EventFlagSet(EventFlag_389);

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            Player_ControlUnfreeze(true);
            SysWork_StateSetNext(SysState_Gameplay);

            vcReturnPreAutoCamWork(true);
            Sd_SfxStop(Sfx_Unk1520);
            break;
    }
}

void Map_WorldObjectsInit(void) // 0x800D2A04
{
    WorldObject_PlacementInit(&g_WorldObject_Movaches, "MOVACHES", 104.2225f, -0.1683f, 60.0667f);
    WorldObject_ModelNameSet(&g_WorldObject_SavePad, g_CommonWorldObjectNames[1]);
    WorldObject_PoseInit(&g_WorldObjectPose_SavePad[0], 58.1048f, -0.9f, 62.5574f, 0.0f, 17.41f, 0.0f);
    WorldObject_PoseInit(&g_WorldObjectPose_SavePad[1], 57.905f, -0.9f, 22.5574f, 0.0f, 17.41f, 0.0f);
    WorldObject_Init(&g_WorldObject_Mag, "MAG_HIDE", 61.1727f, 0.43f, 18.6243f, 0.0f, 60.3f, 0.0f);

    WorldObject_ModelNameSet(&g_CommonWorldObjects[0], g_CommonWorldObjectNames[2]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[1], g_CommonWorldObjectNames[3]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[2], g_CommonWorldObjectNames[4]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[3], g_CommonWorldObjectNames[5]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[4], g_CommonWorldObjectNames[6]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[5], g_CommonWorldObjectNames[7]);
}

void Map_WorldObjectsUpdate(void) // 0x800D2B68
{
    s32 triggerFlags;

    triggerFlags = CollisionTriggerFlag_None;

    if (PLAYER_IN_MAP_CHUNK(2, 1))
    {
        if (Savegame_EventFlagGet(EventFlag_389))
        {
            D_800D6F8C = 0x3C720; // TODO: `Q12(60.4454f)`? Weird number.
        }
        else
        {
            triggerFlags = CollisionTriggerFlag_Objects;
        }

        WorldObjects_Add(&g_WorldObject_Movaches.object, &g_WorldObject_Movaches.position, &SVECTOR3_Zero);
    }
    else
    {
        Collision_TriggerFlagBitsClear(CollisionTriggerFlag_All);
    }

    if (PLAYER_IN_MAP_CHUNK(1, 1))
    {
        WorldObjects_Add(&g_WorldObject_SavePad, &g_WorldObjectPose_SavePad[0].position, &g_WorldObjectPose_SavePad[0].rotation);
    }

    if (PLAYER_IN_MAP_CHUNK(1, 0))
    {
        WorldObjects_Add(&g_WorldObject_SavePad, &g_WorldObjectPose_SavePad[1].position, &g_WorldObjectPose_SavePad[1].rotation);

        if (!Savegame_EventFlagGet(EventFlag_M5S03_PickupMagnet))
        {
            WorldObjects_Add(&g_WorldObject_Mag.object, &g_WorldObject_Mag.position, &g_WorldObject_Mag.rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(2, 1))
    {
        if (!Savegame_EventFlagGet(EventFlag_M5S03_HealthDrink0))
        {
            WorldObjects_Add(&g_WorldObject_HealthDrink, &g_WorldObjectPose_HealthDrink0.position, &g_WorldObjectPose_HealthDrink0.rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(2, 0))
    {
        if (!Savegame_EventFlagGet(EventFlag_M5S03_ShotgunShells))
        {
            WorldObjects_Add(&g_WorldObject_ShotgunShells, &g_WorldObjectPose_ShotgunShells.position, &g_WorldObjectPose_ShotgunShells.rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(2, 0))
    {
        if (!Savegame_EventFlagGet(EventFlag_M5S03_HealthDrink1))
        {
            WorldObjects_Add(&g_WorldObject_HealthDrink, &g_WorldObjectPose_HealthDrink1.position, &g_WorldObjectPose_HealthDrink1.rotation);
        }
    }

    Collision_TriggerFlagBitsClear(CollisionTriggerFlag_All);
    Collision_TriggerFlagBitsSet(triggerFlags);
}
