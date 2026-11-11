#include "bodyprog/bodyprog.h"
#include "bodyprog/chara/dms.h"
#include "bodyprog/events/event_data.h"
#include "bodyprog/world/effects.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player/player.h"
#include "main/rng.h"
#include "maps/map3/map3_s06.h"
#include "maps/common/characters/dahlia.h"
#include "maps/common/particle.h"
#include "maps/common/characters/player.h"

#include "../src/maps/common/particles/particle.c"

#include "../src/maps/common/characters/player.c"

#include "../src/maps/common/characters/dahlia.c"

#include "../src/maps/common/chara_util.c" // 0x800CFCD8

#include "../src/maps/common/map_util.c" // 0x800D06E8

#include "maps/common/shared/Map_RoomBgmInit_CheckCond.h" // 0x800D07A0

#include "maps/common/shared/Map_RoomBgmInit_3_s00_CondTrue.h" // 0x800D07D4

#include "maps/common/shared/Map_RoomBgmInit_3_s00_CondFalse.h" // 0x800D0814

void GameBoot_LoadScreen_StageString(void) {}

#include "maps/common/shared/MapEvent_DoorJammed.h" // 0x800D09BC

#include "maps/common/shared/MapEvent_DoorLocked.h" // 0x800D0A50

void func_800D0AE4(void) {}

const char* MAP_MESSAGES[] = {
    #include "maps/common/shared/map_msg_common.inc"
#if VERSION_REGION_IS(NTSC)
    /* 15 */ "~J0(1.5)\tWas_I_dreaming? ~E ",
    /* 16 */ "~J0(2.6)\tYou_were_too_late. ~E ",
    /* 17 */ "~J0(1.2)\tIt's_you... ~E ",
    /* 18 */ "~J0(3.4)\tYes.__Dahlia_Gillespie. ~E ",
    /* 19 */ "~J1(3.0)\tTell_me_everything_you_know. ~N\n\t\t\tWhat's_going_on? ",
    /* 20 */ "~J1(1.5)\tDarkness. ",
    /* 21 */ "~J1(3.9)\tThe_town_is_being_devoured ~N\n\t\t\tby_darkness. ",
    /* 22 */ "~J1(5.8)\tStrength_must_overcome ~N\n\t\t\tpetty_desire. ~N\n\t\t\tChildish_sleep_talk. ",
    /* 23 */ "~J1(3.4)\tI_knew_this_day_would_come. ",
    /* 24 */ "~J1(4.0)\tWhat_are_you_talking_about? ~N\n\t\t\tI_don't_understand_a_word_of_this. ~E ",
    /* 25 */ "~J0(3.8)\tBelieve_the_evidence ~N\n\t\t\tof_your_eyes. ",
    /* 26 */ "~J0(5.3)\tThe_other_church_in_this_town. ~N\n\t\t\tThat_is_your_destination. ",
    /* 27 */ "~J0(6.5)\tThis_is_beyond_my_abilities. ~N\n\t\t\tOnly_you_can_stop_it_now. ~E ",
    /* 28 */ "~J0(5.0)\tHave_you_not_seen ~N\n\t\t\tthe_crest_marked ~N\n\t\t\ton_the_ground_all_over_town? ",
    /* 29 */ "~J0(3.2)\tSo_that's_what_I_saw ~N\n\t\t\tin_the_schoolyard. ",
    /* 30 */ "~J0(1.2)\tWhat_does_it_mean? ",
    /* 31 */ "~J0(3.1)\tIt_is_the_mark_of_Samael... ",
    /* 32 */ "~J0(2.8)\tDon't_let_it_be_completed. ~E ",
    /* 33 */ "~J0(1.8)\tHey!__Wait! ~E ",
    /* 34 */ "\tThe_elevator_doesn't ~N\n\tappear_to_be_working. ~E ",
    /* 35 */ "~H\tThere_is_a_ ~C5 Hospital_map ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 36 */ "\tThere_is_an_ ~C2 Antique_shop_key ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 37 */ "\tThese_are_phones_and_phonebooks. ~N\n\tNo_need_to_examine_this. ~E ",
    /* 38 */ "\tThese_are_phones_and_phonebooks. ",
    /* 39 */ "\tI'll_copy_the_location_of ~N\n\tthe_antique_shop_to_the_map. ~E ",
    /* 40 */ "\tThere's_a_vending_machine. ~N\n\tIt_doesn't_work. ~E ",
    /* 41 */ "\tThere_is_a_newspaper. ",
    /* 42 */ "\tThis_article's_been ~N\n\tclipped_out... ~E ",
    /* 43 */ "\tNothing_unusual. ~E ",
    /* 44 */ "\tThere's_a_white_board. ~N\n\tNothing_special_written_here. ~E ",
    /* 45 */ "\tNothing_helpful. ~E ",
    /* 46 */ "\tThe_phone's_out. ~N\n\tCan't_get_a_dial_tone. ~E "
#elif VERSION_REGION_IS(NTSCJ)
    /* 15 */ "~J0(1.5)\t……夢？ ~E ",
    /* 16 */ "~J0(2.6)\t間に合わなかったようね ~E ",
    /* 17 */ "~J0(1.2)\tあんたは…… ~E ",
    /* 18 */ "~J0(3.4)\tあたしはダリア・ギレスピー ~E ",
    /* 19 */ "~J1(3.0)\t何を知っている？ ~N\n\t\t\tこの町はいったいどうなっているんだ？ ",
    /* 20 */ "~J1(1.5)\t闇よ ",
    /* 21 */ "~J1(3.9)\t町を闇が蝕んでいるのよ ",
    /* 22 */ "~J1(5.8)\t卑小なる者の欲望の忌むべき力…… ~N\n\t\t\t稚拙な夢語り ",
    /* 23 */ "~J1(3.4)\tわかっていたわ　この日が来るのは…… ",
    /* 24 */ "~J1(4.0)\t何を言ってるんだ？ ~N\n\t\t\tさっぱりわからないんだが…… ~E ",
    /* 25 */ "~J0(3.8)\t自分の目で見たものを信じればいいだけよ ",
    /* 26 */ "~J0(5.3)\tこの町のもう一つの教会 ~N\n\t\t\t行く先はそこよ ",
    /* 27 */ "~J0(6.5)\tあたしでは届かない ~N\n\t\t\tあなただけが止められるのよ ~E ",
    /* 28 */ "~J0(5.0)\t町中で地面に描かれた ~N\n\t\t\t紋章のようなものを見なかった？ ",
    /* 29 */ "~J0(3.2)\t小学校の中庭にあったあれか？ ",
    /* 30 */ "~J0(1.2)\tあれが何だというんだ？ ",
    /* 31 */ "~J0(3.1)\tサマエルの印章…… ",
    /* 32 */ "~J0(2.8)\tすべてが完成する前に止めるのね~E ",
    /* 33 */ "~J0(1.8)\t待て！　おい！ ~E ",
    /* 34 */ "\tエレベーターは動いていないようだ ~E ",
    /* 35 */ "~H ~C5 病院の地図 ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 36 */ "~C2 骨董屋の鍵 ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 37 */ "\t電話機とこの地域の電話帳がある ~N\n\t特に調べる必要はないだろう ~E ",
    /* 38 */ "\t電話機とこの地域の電話帳がある ",
    /* 39 */ "\t骨董屋の場所がのっている ~N\n\t地図に書き写しておこう ~E ",
    /* 40 */ "\t自動販売機がある ~N\n\t故障か停電かわからないが動いていない ~E ",
    /* 41 */ "\t新聞が置いてある ",
    /* 42 */ "\t切り抜いた跡があるな…… ~E ",
    /* 43 */ "\t特に気になることはない ~E ",
    /* 44 */ "\tホワイトボードがある ~N\n\t特に気になるようなことは書かれていない ~E ",
    /* 45 */ "\t役に立ちそうなものはない ~E ",
    /* 46 */ "\t電話機は使えないようだ ~N\n\t何をしても反応がない ~E "
#endif
};

void MapEvent_CommonItemTake(void) // 0x800D0AEC
{
    Event_CommonItemTake(CommonPickupItemId_FirstAidKit, EventFlag_M3S00_FirstAidKit);
}

void func_800D0B10(void) // 0x800D0B10
{
    const static VECTOR3 D_800CB21C = { Q12(61.72f), Q12(-0.8f), Q12(100.51f) };

    if (g_SysWork.sysStateSteps[1] == 0)
    {
        Sfx_WithFlagsPlay(Sfx_MenuMap, &D_800CB21C, Q8(0.5f), SfxFlag_None);
    }

    Event_DisplayMapMsgWithBg(FILE_TIM_NEWSP1_TIM, Q12(0.0f), Q12(0.0f), 41);
    Savegame_EventFlagSet(EventFlag_203);
}

#include "maps/common/shared/Event_CutsceneTimerAdvance.h" // 0x800D0B7C

void func_800D0C14(void) // 0x800D0C14
{
    // Skip.
    if (Pad_ButtonClickedBind(skip) &&
        g_SysWork.sysStateSteps[0] >= 4 && g_SysWork.sysStateSteps[0] < 30)
    {
        SysWork_StateStepSet(0, 30);
        Savegame_EventFlagSet(EventFlag_297);
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            CutsceneBorder_ForceShow();
            Screen_FadeTimestepReset();
            g_SysWork.sysFlags |= SysFlag_CutsceneActive;

            Sd_SfxStop(Sfx_Unk1522);

            Fs_QueueStartRead(FILE_ANIM_HSPTL2_DMS, FS_BUFFER_15);
            Fs_QueueWaitForEmpty();
            Dms_HeaderFixOffsets(FS_BUFFER_15);

            g_Cutscene_Timer = Q12(0.0f);
            D_800D2736 = 0;
            D_800D2737 = false;

            func_8003D03C();
            Player_EmptyWeaponHandSet();

            D_800D2738 = 0;
            D_800D2734 = Q12(0.0f);

            SysWork_StateStepIncrement(0);
            break;

        case 1:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);

            Chara_Load(0, Chara_Dahlia, &g_SysWork.npcBoneCoordBuffer[0], CHARA_FORCE_FREE_ALL, NULL, NULL);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &PLAYER_CHARA(), 117, false);

            SysWork_StateStepIncrement(0);

        case 2:
            Event_ScreenFadeCmd(ScreenFadeCmd_Wait, false, 0, Q12(0.0f), false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimLock, &PLAYER_CHARA(), 0, false);
            break;

        case 3:
            Chara_ProcessLoads();
            Chara_Spawn(Chara_Dahlia, 0, Q12(98.0f), Q12(100.0f), Q12_ANGLE(0.0f), 3);
            Chara_MovementReset(&NPC_CHARA(0));

            D_800D2737 = true;
            SysWork_StateStepIncrement(0);

        case 4:
            Event_DisplayMapMsgWithAudio(15, &D_800D2736, &D_800D26D0);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(6.0f), Q12(0.0f), Q12(39.0f), true, false);
            break;

        case 5:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimUnlock, &PLAYER_CHARA(), 0, false);
            D_800D2738 = 0;
            SysWork_StateStepIncrement(0);

        case 6:
            Event_DisplayMapMsgWithAudio(16, &D_800D2736, &D_800D26D0);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(6.0f), Q12(0.0f), Q12(39.0f), true, false);

            if (D_800D2738 != 2)
            {
                g_SysWork.sysStateStepData[0] += g_DeltaTime;

                if (D_800D2738 == 0 && !(g_SysWork.sysStateStepData[0] <= Q12(0.3f)))
                {
                    Sfx_WithFlagsPlay(Sfx_DoorOpen0, &D_800D26F8, Q8(0.5f), SfxFlag_None);
                    D_800D2738++;
                }
                else if (D_800D2738 == 1 && !(g_SysWork.sysStateStepData[0] <= Q12(1.6f)))
                {
                    Sfx_WithFlagsPlay(Sfx_DoorClose0, &D_800D26F8, Q8(0.5f), SfxFlag_None);
                    D_800D2738++;
                }
            }
            break;

        case 7:
            if (Sd_AmbientSfxInit() == 0)
            {
                SysWork_StateStepIncrement(0);
            }

        case 8:
            D_800D2734 += g_DeltaTime;
            if (D_800D2734 > Q12(2.0f) && g_SysWork.sysStateSteps[0] == 8)
            {
                SysWork_StateStepSet(0, 9);
            }

            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(6.0f), Q12(0.0f), Q12(39.0f), true, false);
            break;

        case 9:
            Event_DisplayMapMsgWithAudio(17, &D_800D2736, &D_800D26D0);
            break;

        case 10:
            Event_WaitTimer(Q12(1.5f), false);
            break;

        case 11:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &NPC_CHARA(0), 11, false);
            SysWork_StateStepIncrement(0);

        case 12:
            Event_DisplayMapMsgWithAudio(18, &D_800D2736, &D_800D26D0);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(6.5f), Q12(40.0f), Q12(60.0f), true, false);
            break;

        case 13:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(6.5f), Q12(40.0f), Q12(60.0f), true, true);
            break;

        case 14:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 15:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &PLAYER_CHARA(), 51, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &NPC_CHARA(0), 0, false);
            SysWork_StateStepIncrement(0);

        case 16:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.0f), false);
            g_Cutscene_Timer = Q12(61.0f);
            break;

        case 17:
            Event_WaitTimer(Q12(2.0f), false);
            break;

        case 18:
            Event_DisplayMapMsgWithAudio(19, &D_800D2736, &D_800D26D0);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(2.0f), Q12(61.0f), Q12(100.0f), true, false);
            break;

        case 19:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &NPC_CHARA(0), 9, false);
            SysWork_StateStepIncrement(0);

        case 20:
            Event_DisplayMapMsgWithAudio(25, &D_800D2736, &D_800D26D0);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(2.0f), Q12(61.0f), Q12(126.0f), true, false);

            if (g_Cutscene_Timer >= Q12(120.0f))
            {
                Model_AnimFlagsClear(&PLAYER_CHARA().model, 2);
            }
            break;

        case 21:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &NPC_CHARA(0), 10, false);
            Model_AnimFlagsSet(&PLAYER_CHARA().model, 2);
            SysWork_StateStepIncrement(0);

        case 22:
            Event_DisplayMapMsgWithAudio(28, &D_800D2736, &D_800D26D0);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(127.0f), Q12(177.0f), true, false);
            Event_WaitTimer(Q12(9.8f), false);
            break;

        case 23:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &NPC_CHARA(0), 0, false);
            SysWork_StateStepIncrement(0);

        case 24:
            Event_DisplayMapMsgWithAudio(28, &D_800D2736, &D_800D26D0);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(127.0f), Q12(177.0f), true, false);
            break;

        case 25:
            Savegame_EventFlagSet(EventFlag_297);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &NPC_CHARA(0), 11, false);
            SysWork_StateStepIncrement(0);

        case 26:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(13.5f), Q12(178.0f), Q12(194.0f), true, true);
            break;

        case 27:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &PLAYER_CHARA(), 111, false);
            Chara_ModelCharaIdClear(&NPC_CHARA(0), 0, 0);
            D_800D2738 = 0;
            SysWork_StateStepIncrement(0);

        case 28:
            Event_DisplayMapMsgWithAudio(33, &D_800D2736, &D_800D26D0);

            if (D_800D2738 != 1)
            {
                g_SysWork.sysStateStepData[0] += g_DeltaTime;
                if (D_800D2738 == 0 && g_SysWork.sysStateStepData[0] > Q12(0.6f))
                {
                    Sfx_WithFlagsPlay(Sfx_DoorClose0, &D_800D26F8, Q8(0.5f), SfxFlag_None);
                    D_800D2738++;
                }
            }

            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(199.0f), Q12(244.0f), true, false);
            break;

        case 29:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(199.0f), Q12(244.0f), true, true);
            break;

        case 30:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 31:
            g_Cutscene_Timer = Q12(244.0f);
            D_800D2737 = false;
            SysWork_StateStepIncrement(0);
            break;

        case 32:
            if (Sd_AmbientSfxInit() == 0)
            {
                SysWork_StateStepIncrement(0);
            }
            else
            {
                break;
            }

        default:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimUnlock, &PLAYER_CHARA(), 0, false);

            // Return to gameplay.
            Player_ControlUnfreeze(true);
            SysWork_StateSetNext(SysState_Gameplay);
            vcReturnPreAutoCamWork(true);

            g_Cutscene_Timer = NO_VALUE;

            func_8003D01C();
            Player_WeaponAttackRestore();
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            Savegame_EventFlagSet(EventFlag_295);
            SD_Call(19);

            Chara_ModelCharaIdClear(&NPC_CHARA(0), 0, 0);
            Model_AnimFlagsSet(&PLAYER_CHARA().model, 2);
            Game_TurnFlashlightOff();
            break;
    }

    if (g_Cutscene_Timer >= Q12(0.0f))
    {
        Dms_CharacterTransformGet(&PLAYER_CHARA().position, &PLAYER_CHARA().rotation, "HERO", g_Cutscene_Timer, FS_BUFFER_15);
        if (D_800D2737)
        {
            Dms_CharacterTransformGet(&NPC_CHARA(0).position, &NPC_CHARA(0).rotation, "DARIA", g_Cutscene_Timer, FS_BUFFER_15);
        }

        vcChangeProjectionValue(Dms_CameraTargetsGet(&g_Cutscene_CameraPositionTarget, &g_Cutscene_CameraLookAtTarget, NULL, g_Cutscene_Timer, FS_BUFFER_15));
        vcUserCamTarget(&g_Cutscene_CameraPositionTarget, NULL, true);
        vcUserWatchTarget(&g_Cutscene_CameraLookAtTarget, NULL, true);
    }
}

void MapEvent_PaperMapTake(void) // 0x800D17B0
{
    Event_PaperMapTake(17, EventFlag_M3S00_PickupMap, 35);
}

void func_800D17D8(void) // 0x800D17D8
{
    Event_ItemTake(InvItemId_AntiqueShopKey, DEFAULT_PICKUP_ITEM_COUNT, EventFlag_M3S06_PickupAntiqueShopKey, 36);
}

void Map_WorldObjectsInit(void) // 0x800D1804
{
    WorldObject_Init(&g_WorldObject_SavePad, g_CommonWorldObjectNames[1], 26.0f, -1.02f, 144.55f, 0.0f, 163.8f, 0.0f);
    WorldObject_Init(&g_WorldObject_Map, "MAP_HIDE", 24.6f, -1.56f, 143.16f, 0.0f, 180.0f, 0.0f);
    WorldObject_Init(&g_WorldObject_Map2, "MAP2_HID", 24.6f, -1.56f, 143.16f, 0.0f, 180.0f, 0.0f);
    WorldObject_Init(&g_WorldObject_Key, "KEY_HIDE", 95.734f, -0.8062f, 99.8885f, 0.0f, -35.9f, 0.0f);
    WorldObject_Init(&g_WorldObject_FirstAidKit, g_CommonWorldObjectNames[2], 27.6f, -0.45f, 143.6f, 0.0f, -90.0f, 0.0f);
}

void Map_WorldObjectsUpdate(void) // 0x800D197C
{
    if (PLAYER_IN_MAP_CHUNK_X(0))
    {
        if (PLAYER_IN_MAP_CHUNK_Z(3))
        {
            WorldObjects_Add(&g_WorldObject_SavePad.object, &g_WorldObject_SavePad.position, &g_WorldObject_SavePad.rotation);

            if (!(Savegame_EventFlagGet(EventFlag_M3S00_PickupMap)))
            {
                WorldObjects_Add(&g_WorldObject_Map.object, &g_WorldObject_Map.position, &g_WorldObject_Map.rotation);
            }
            else
            {
                WorldObjects_Add(&g_WorldObject_Map2.object, &g_WorldObject_Map2.position, &g_WorldObject_Map2.rotation);
            }

            if (!(Savegame_EventFlagGet(EventFlag_M3S00_FirstAidKit)))
            {
                WorldObjects_Add(&g_WorldObject_FirstAidKit.object, &g_WorldObject_FirstAidKit.position, &g_WorldObject_FirstAidKit.rotation);
            }
        }
    }

    if (PLAYER_IN_MAP_CHUNK_X(2))
    {
        if (PLAYER_IN_MAP_CHUNK_Z(2))
        {
            if (!Savegame_EventFlagGet(EventFlag_M3S06_PickupAntiqueShopKey) && Savegame_EventFlagGet(EventFlag_297))
            {
                WorldObjects_Add(&g_WorldObject_Key.object, &g_WorldObject_Key.position, &g_WorldObject_Key.rotation);
            }
        }
    }

    if (Savegame_EventFlagGet(EventFlag_M3S06_PickupAntiqueShopKey) && Savegame_EventFlagGet(EventFlag_M2S00_PickupMap))
    {
        Savegame_EventFlagSet(EventFlag_317);
    }
}
