#include "inline_no_dmpsx.h"

#include <psyq/gtemac.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/chara/dms.h"
#include "bodyprog/events/event_data.h"
#include "bodyprog/item_screens.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player/player.h"
#include "main/rng.h"
#include "maps/map1/map1_s01.h"
#include "maps/common/particle.h"
#include "maps/common/characters/larval_stalker.h"
#include "maps/common/characters/player.h"
#include "maps/common/characters/stalker.h"

#include "../src/maps/common/particles/particle_smoke.c"

#include "../src/maps/common/particles/particle.c"

#include "../src/maps/common/characters/player.c"

#include "../src/maps/common/characters/larval_stalker.c" // 0x800CE648
#include "../src/maps/common/characters/stalker.c" // 0x800D1AF0
#include "../src/maps/common/characters/cat.c" // 0x800D6D40

#include "../src/maps/common/map_util.c" // 0x800D6F34

#define playerChara g_SysWork.playerWork.player

void Map_RoomBgmInit(bool arg0) // 0x800D6FE4
{
    u16 bgmFlags;

    if (g_SavegamePtr->mapRoomIdx == 23)
    {
        bgmFlags = BgmFlag_Layer6;
        if (Savegame_EventFlagGet(EventFlag_71) && Savegame_EventFlagGet(EventFlag_72))
        {
            bgmFlags = BgmFlag_Layer2 | BgmFlag_Layer3 | BgmFlag_Layer4 | BgmFlag_Layer5 |
                       BgmFlag_Layer6 | BgmFlag_Layer7 | BgmFlag_Layer8 |
                       BgmFlag_KeepAlive;
        }
    }
    else
    {
        bgmFlags = D_800DCA04[g_SavegamePtr->mapRoomIdx];
    }

    Bgm_LayersUpdate(bgmFlags, Q12(0.1f), &D_800DC9FC);
}

void GameBoot_LoadScreen_StageString(void) {}

void MapEvent_DoorJammed(void) // 0x800D7058
{
    VECTOR3 sfxPos = { MAP_POINTS[g_MapEventData->mapPointIdx].positionX, Q12(-1.2f), MAP_POINTS[g_MapEventData->mapPointIdx].positionZ };

    Event_DisplayMapMsgWithSfx(MapMsgIdx_DoorJammed, Sfx_DoorJammed, &sfxPos);
}

void MapEvent_DoorLocked(void) // 0x800D70EC
{
    VECTOR3 sfxPos = { MAP_POINTS[g_MapEventData->mapPointIdx].positionX, Q12(-1.2f), MAP_POINTS[g_MapEventData->mapPointIdx].positionZ };

    Event_DisplayMapMsgWithSfx(MapMsgIdx_DoorLocked, Sfx_DoorLocked, &sfxPos);
}

void MapEvent_DoorUnlocked(void) // 0x800D7180
{
    VECTOR3 sfxPos = { MAP_POINTS[g_MapEventData->mapPointIdx].positionX, Q12(-1.2f), MAP_POINTS[g_MapEventData->mapPointIdx].positionZ };

    Event_DisplayMapMsgWithSfx(MapMsgIdx_DoorUnlocked, Sfx_DoorUnlocked, &sfxPos);
}

const char* MAP_MESSAGES[] = {
    #include "maps/common/shared/map_msg_common.h"
#if VERSION_REGION_IS(NTSC)
    /* 15 */ "\tStatue_of_an_old_man's_hand.",
    /* 16 */ "\tThe_fist_shut_tight, ~N\n\tas_if_never_to_let_go. ~E ",
    /* 17 */ "\tThere_is_a_ ~C2 Gold_medallion ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 18 */ "\tThere_is_a_ ~C2 Chemical ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 19 */ "\tNothing_unusual.~E ",
    /* 20 */ "\tJust_the_melted_remains_of ~N\n\ta_statue_of_a_hand. ",
    /* 21 */ "\tNothing_to_worry_about. ~E ",
    /* 22 */ "\t",
    /* 23 */ "~L4 ~M ~C3 [A_Tale_of_Birds_Without_a_Voice] ~N\n\t~N\n\tFirst_flew_the_greedy_Pelican, ~N\n\tEager_for_the_reward, ~N\n\tWhite_wings_flailing. ~N\n\t~N\n\tThen_came_a_silent_Dove, ~N\n\tFlying_beyond_the_Pelican, ~N\n\tAs_far_as_he_could. ",
    /* 24 */ "~L4 ~M ~C3 A_Raven_flies_in, ~N\n\tFlying_higher_than_the_Dove, ~N\n\tJust_to_show_that_he_can. ~N\n\t~N\n\tA_Swan_glides_in, ~N\n\tTo_find_a_peaceful_spot, ~N\n\tNext_to_another_bird. ",
    /* 25 */ "~L4 ~M ~C3 Finally_out_comes_a_Crow, ~N\n\tComing_quickly_to_a_stop, ~N\n\tYawning_and_then_napping. ~N\n\t~N\n\tWho_will_show_the_way, ~N\n\tWho_will_be_the_key, ~N\n\tWho_will_lead_to ~N\n\tThe_silver_reward. ~E ",
    /* 26 */ "\tIt_won't_open._Odd, ~N\n\tdoesn't_look_locked,_though. ~E ",
    /* 27 */ "\tThere's_some_blood ~N\n\ton_some_of_the_keys..... ",
    /* 28 */ "\tDoes_this_mean_anything? ~E ",
    /* 29 */ "\tCheck_the_piano? ~S4 ",
    /* 30 */ "\tThere_is_a_ ~C2 Silver_medallion ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 31 */ "\tIt's_written_in_blood. ~E ",
    /* 32 */ "\tNothing_inside. ~E ",
    /* 33 */ "~J0(2.0)\tJust_a_cat... ~E ",
    /* 34 */ "\tDistilled_water. ~N\n\tNo_reason_to_take_it. ~E ",
    /* 35 */ "\tNothing_useful_on_the_shelf. ~E ",
    /* 36 */ "\tNothing_useful ~N\n\tin_the_desk_drawer. ~E ",
    /* 37 */ "\tNothing_helpful. ~E ",
    /* 38 */ "\tGlucose. ~N\n\tNo_reason_to_take_it. ~E ",
    /* 39 */ "\t~C3 Warning._Don't_forget_to ~N\n\ttake_all_of_your_possessions ~N\n\t_after_using_the_lockers. ",
    /* 40 */ "\tNormal_elementary_school ~N\n\tbulletin_board. ~N\n\tNothing_in_particular_here. ~E ",
    /* 41 */ "\t~C3 ~M NO ~N\n\tRUNNING_OR_PLAYING ~N\n\tIN_THE_LOCKER_ROOM ~E ",
    /* 42 */ "\tNothing_useful_in_the_locker. ~E ",
    /* 43 */ "\tNothing_useful ~N\n\tin_the_desk_drawer. ~E ",
    /* 44 */ "\tNo_useful_books. ~E ",
    /* 45 */ "\tNo_special_book_here. ~E ",
    /* 46 */ "\tNo_need_to_read ~N\n\tany_of_these_books. ~E ",
    /* 47 */ "\tThere's_some_blood ~N\n\ton_some_of_the_keys..... ~E "
#elif VERSION_REGION_IS(NTSCJ)
    /* 15 */ "\t\t老人の手の彫像がある ",
    /* 16 */ "\t\t何かを放すまいとするかのように ~N\n\t\t堅く握り締められている ~E ",
    /* 17 */ "\t\t~C2 金のメダリオン ~C7 がある ~N\n\t\t取りますか？ ~S4 ",
    /* 18 */ "\t\t~C2 化学薬品 ~C7 がある ~N\n\t\t取りますか？ ~S4 ",
    /* 19 */ "\t\t特に何もない ~E ",
    /* 20 */ "\t\t溶けた手の彫像があるだけだ ~N\n\t\t特に気になることはない ~E ",
    /* 21 */ "\t\t鍵がかかっている ~N\n\t\t中に何かいるような気配がするのだが…… ~E ",
    /* 22 */ "~L4 ~M ~C3\n\t−声無き鳥の飛び比べ− ~N\n\t~N\n\t最初に飛び出す欲張りアヒル ~N\n\tご褒美欲しくてたまらない ~N\n\tバタバタ動かす白い羽 ~N\n\t~N\n\t続けて飛び出す気取り屋ペリカン ~N\n\tひょいとアヒルを追い抜いて ~N\n\t見くだし見おろし得意顔 ",
    /* 23 */ "~L4 ~M ~C3 ~N\n\tその次飛び出すツバメの高さ ~N\n\tペリカンよりも上だった ~N\n\tにやりと笑うと宙返り ~N\n\t~N\n\t遅れて来たのはカモメの水兵 ~N\n\t平凡なりには努力して ~N\n\tアヒルに勝ったがその程度 ",
    /* 24 */ "~L4 ~M ~C3\n\tぼんやりカラスは最後の最後 ~N\n\t飛ばず鳴かずに昼寝する ~N\n\tのんびりぐうたら大あくび ~N\n\t~N\n\t鳴く声無くした鳥たちが ~N\n\t勲章めぐって大騒ぎ ~N\n\t~N\n\t銀色ご褒美誰のもの ~N\n\t銀ピカメダルは誰のもの ~E ",
    /* 25 */ "\t開かないな　鍵がかかって ~N\n\tいるわけでもなさそうだが…… ~E ",
    /* 26 */ "\t鍵盤の一部に血がついている ~N\n\t何か意味があるのだろうか……？ ~E ",
    /* 27 */ "\tピアノを調べますか？ ~S4 ",
    /* 28 */ "\t~C2 銀のメダリオン ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 29 */ "\t血で文字が書かれている ~E ",
    /* 30 */ "\tロッカーの中には何もない ~E ",
    /* 31 */ "~J0(2.0)\t何だ　猫か…… ~E ",
    /* 32 */ "\t蒸留水が置いてある ~N\n\t持っていく必要はないだろう ~E ",
    /* 33 */ "\t棚の中に役に立ちそうなものはない ~E ",
    /* 34 */ "\t机の中に役に立ちそうな物は入ってない ~E ",
    /* 35 */ "\t使えそうなものは特にない ~E ",
    /* 36 */ "\tブドウ糖が置いてある ~N\n\t持っていく必要はないだろう ~E ",
    /* 37 */ "\t~C3 忘れ物注意 ~N\n\tロッカー室使用上の注意 ~N\n\tサマーキャンプの案内 ",
    /* 38 */ "\t普通の小学校の掲示板だな ~N\n\t特に気になることはないか…… ~E ",
    /* 39 */ "\t~C3 ロッカー室で騒がないこと ~E ",
    /* 40 */ "\tロッカーの中に役に ~N\n\t立ちそうな物は入ってない ~E ",
    /* 41 */ "\t机の中に役に立ちそうな物は入ってない ~E ",
    /* 42 */ "\t役に立ちそうな本はない ~E ",
    /* 43 */ "\t特に気になる本はない ~E ",
    /* 44 */ "\tここの本は特に読む必要もないだろう ~E ",
    /* 45 */ "\t鍵盤の一部に血がついている ~E "
#endif
};

void MapEvent_CommonItemTake(void) // 0x800D7214
{
    u32 pickupType;
    s32 eventFlagIdx;

    pickupType   = CommonPickupItemId_FirstAidKit;
    eventFlagIdx = 0;

    switch (g_MapEventData->mapPointIdx)
    {
        case 96:
            pickupType   = CommonPickupItemId_HandgunBullets;
            eventFlagIdx = EventFlag_M1S01_HandgunBullets0;
            break;

        case 97:
            pickupType   = CommonPickupItemId_FirstAidKit;
            eventFlagIdx = EventFlag_M1S01_FirstAidKit;
            break;

        case 98:
            pickupType   = CommonPickupItemId_HealthDrink;
            eventFlagIdx = EventFlag_M1S01_HealthDrink;
            break;

        case 99:
            pickupType   = CommonPickupItemId_HandgunBullets;
            eventFlagIdx = EventFlag_M1S01_HandgunBullets1;
            break;

        case 100:
            pickupType   = CommonPickupItemId_HandgunBullets;
            eventFlagIdx = EventFlag_M1S01_HandgunBullets2;
            break;
    }

    Event_CommonItemTake(pickupType, eventFlagIdx);
}

void MapEvent_HandStatueInspect(void) // 0x800D72B0
{
    Event_DisplayMapMsgWithBg(FILE_TIM_HAND_TIM, Q12(2.0f), Q12(1.5f), 15); // "Statue of an old man's hand."
}

void MapEvent_ChemicalItemTake(void) // 0x800D72DC
{
    Event_ItemTake(InvItemId_Chemical, DEFAULT_PICKUP_ITEM_COUNT, EventFlag_M1S01_PickupChemical, 18); // "There is a Chemical. Take it?"
}

void func_800D7308(void)
{
    typedef enum _EventState
    {
        EventState_Skip = 8
    } e_EventState;

    s32 i;

    g_SysWork.bgmStatusFlags |= BgmStatusFlag_ApplyMute;

    // Skip.
    if (Pad_ButtonClickedBind(skip) &&
        g_SysWork.sysStateSteps[0] > 0 && g_SysWork.sysStateSteps[0] < EventState_Skip)
    {
        SysWork_StateStepSet(0, EventState_Skip);
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 2, Q12(0.0f), false);
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            Event_ScreenFadeCmd(ScreenFadeCmd_Wait, true, 2, Q12(0.0f), false);
            break;

        case 3:
            // Warp player.
            playerChara.position.vx = Q12(-59.2f);
            playerChara.position.vz = Q12(98.9f);
            playerChara.rotation.vy = Q12_ANGLE(0.0f);

            // Warp camera.
            Event_CameraPositionSet(NULL, 
                                    Q12(-59.61f), Q12(-0.41f), Q12(97.48f),
                                    Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                    true);
            Event_CameraLookAtSet(NULL,
                                  Q12(-58.6f), Q12(-2.29f), Q12(100.87f),
                                  Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                  true);

            SysWork_StateStepIncrement(0);

        case 4:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &playerChara, 83, false);
            SysWork_StateStepIncrement(0);

        case 5:
            Event_WaitTimer(Q12(2.5f), false);
            break;

        case 6:
            Sfx_WithFlagsPlay(Sfx_Unk1433, &Q12_VECTOR3(-59.2f, -1.0f, 99.5f), Q8(0.5f), SfxFlag_None);

            sharedData_800DEE50_1_s01.field_0  = 0;
            sharedData_800DEE50_1_s01.field_4  = 0x96;
            sharedData_800DEE50_1_s01.field_A  = 0x2E1;
            sharedData_800DEE50_1_s01.field_6  = Q12(-1.5f);
            sharedData_800DEE50_1_s01.field_8  = Q12(-1.0f);
            sharedData_800DEE50_1_s01.field_C  = Q12(-0.4f);
            sharedData_800DEE50_1_s01.field_E  = 0x51;
            sharedData_800DEE50_1_s01.field_1  = 0x20;
            sharedData_800DEE50_1_s01.field_2  = 0;
            sharedData_800DEE50_1_s01.field_14 = Q12(-59.2f);
            sharedData_800DEE50_1_s01.field_18 = Q12(99.5f);
            sharedData_800DEE50_1_s01.field_1C = Q12(3.0f);
            sharedData_800DEE50_1_s01.field_12 = 0;

            sharedFunc_800CB7F4_1_s01();
            SysWork_StateStepIncrement(0);

        case 7:
            Event_WaitTimer(Q12(6.0f), false);
            break;

        case 8:
            Sd_SfxStop(Sfx_Unk1433);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 9:
            for (i = 0; i < ARRAY_SIZE(sharedData_800DFB7C_0_s00); i++)
            {
                if (sharedData_800DFB7C_0_s00[i].field_A == 13 ||
                    sharedData_800DFB7C_0_s00[i].field_A == 14)
                {
                    sharedData_800DFB7C_0_s00[i].field_A = 0;
                }
            }

            SysWork_StateStepIncrement(0);

        default:
            // Restore player control.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);

            vcReturnPreAutoCamWork(true);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);

            if (Screen_IsFadeFinished())
            {
                Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            }

            Savegame_EventFlagSet(EventFlag_74);
            break;
    }
}

void MapEvent_GoldMedallionItemPickup(void) // 0x800D76F4
{
    typedef enum _EventState
    {
        EventState_PickUpMedallion     = 4,
        EventState_DontPickUpMedallion = 5
    } e_EventState;

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_InvItemCmd(InvItemCmd_QueueLoad, InvItemId_GoldMedallion, 0, false);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            Event_InvItemCmd(InvItemCmd_AwaitLoad, InvItemId_GoldMedallion, 0, false);
            break;

        case 3:
            if (Gfx_PickupItemAnimate(InvItemId_GoldMedallion))
            {
                Event_DisplayMapMsg(true, 17, EventState_PickUpMedallion, EventState_DontPickUpMedallion, 0, false); // "There is a Gold medalion. Take it?"
            }

            Savegame_EventFlagSet(EventFlag_M1S01_PickupGoldMedallion);
            break;

        case EventState_PickUpMedallion:
            Event_InvItemCmd(InvItemCmd_AddItem, InvItemId_GoldMedallion, 1, false);

        default:
            // Restore player control.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;

        case EventState_DontPickUpMedallion:
            Savegame_EventFlagClear(EventFlag_M1S01_PickupGoldMedallion);

            // Restore player control.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
    }
}

void func_800D7830(void) // 0x800D7830
{
    Event_DisplayMapMsgWithDimmedBg(FILE_TIM_SCORE_TIM, Q12(2.0f), Q12(1.5f), MAP_MSG(31), MAP_MSG(23));
}

void MapEvent_PianoPuzzleInteract(void) // 0x800D7864
{
    typedef enum _EventState
    {
        EventState_CheckPiano             = 5,
        EventState_MedallionCutsceneStart = 10,
        EventState_Skip                   = 17,
        EventState_MedallionCutsceneEnd   = 19,
        EventState_DontCheckPiano         = 21
    } e_EventState;

    g_SysWork.bgmStatusFlags |= BgmStatusFlag_ApplyMute;

    if (Pad_ButtonClickedBind(skip) &&
        g_SysWork.sysStateSteps[0] >= EventState_MedallionCutsceneStart &&
        g_SysWork.sysStateSteps[0] <  EventState_MedallionCutsceneEnd)
    {
        Screen_FadeTimestepReset();
        SysWork_StateStepSet(0, EventState_Skip);

        // Warp Silver Medallion pickup.
        g_WorldObject_SilverH.rotation.vx = Q12_ANGLE(-90.0f);
        g_WorldObject_SilverH.position.vx = Q12(-98.8f);
        g_WorldObject_SilverH.position.vy = Q12(0.0f);
        g_WorldObject_SilverH.position.vz = Q12(22.8f);
        g_WorldObject_SilverH.rotation.vy = Q12_ANGLE(28.5f);
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            SysWork_StateStepIncrement(0);

        case 1:
            Event_BgTextureLoadFadeIn(FILE_TIM_PIANO1_TIM, Q12(2.5f));
            break;

        case 2:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(2.0f), false);
            SysWork_StateStepIncrement(0);

        case 3:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_DisplayMapMsg(false, MAP_MSG(27), false, false, 0, false); // "There's some blood on some of the keys."
            break;

        case 4:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_DisplayMapMsg(true, MAP_MSG(29), EventState_CheckPiano, EventState_DontCheckPiano, 0, false); // "Check the piano?"
            break;

        case EventState_CheckPiano:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(1.5f), false);
            break;

        case 6:
            Event_BgTextureCmd(BgTextureCmd_Auto, FILE_TIM_PIANO2_TIM, false);
            break;

        case 7:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(1.5f), false);
            PianoPuzzle_Control(false);
            SysWork_StateStepIncrement(0);

        case 8:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            PianoPuzzle_Control(true);
            break;

        case 9:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_SysStateBranchOnFlag(EventFlag_M1S01_PianoPuzzleSolved, EventState_MedallionCutsceneStart, EventState_MedallionCutsceneEnd, false);
            break;

        case EventState_MedallionCutsceneStart:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 2, Q12(0.0f), false);
            SysWork_StateStepIncrement(0);

        case 11:
            // Warp player.
            playerChara.position.vx = Q12(-100.0f);
            playerChara.position.vz = Q12(20.1f);
            playerChara.rotation.vy = Q12_ANGLE(-22.5f);

            Event_WaitTimer(Q12(1.5f), false);

            // Warp camera.
            Event_CameraPositionSet(NULL,
                                    Q12(-97.3f), Q12(-2.3699f), Q12(21.97f),
                                    Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                    true);
            Event_CameraLookAtSet(NULL,
                                  Q12(-100.37f), Q12(-2.2f), Q12(24.53f),
                                  Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                  true);
            break;

        case 12:
            Event_WaitTimer(Q12(0.5f), false);

            g_WorldObject_SilverH.rotation.vx += Q12_MULT_PRECISE(g_DeltaTime, Q12(0.0555f));
            g_WorldObject_SilverH.position.vy  = FP_FROM((Math_Cos(g_WorldObject_SilverH.rotation.vx) * Q12_ANGLE(-43.2f)), Q12_SHIFT) - Q12(2.5601f);
            g_WorldObject_SilverH.position.vz  = FP_FROM((Math_Sin(g_WorldObject_SilverH.rotation.vx) * Q12_ANGLE(-43.2f)), Q12_SHIFT) + Q12(23.73f);
            break;

        case 13:
            D_800DD4FC                 += g_GravitySpeed;
            g_WorldObject_SilverH.rotation.vx += Q12_MULT_PRECISE(g_DeltaTime, Q12(0.75f));
            g_WorldObject_SilverH.position.vy += Q12_MULT_PRECISE(g_DeltaTime, D_800DD4FC);
            g_WorldObject_SilverH.position.vz += Q12_MULT_PRECISE(g_DeltaTime, Q12(-0.5f));

            if (g_WorldObject_SilverH.position.vy >= Q12(0.0f))
            {
                SD_Call(Sfx_Unk1417);
                SysWork_StateStepIncrement(0);
            }
            break;

        case 14:
            Event_WaitTimer(Q12(1.0f), false);
            break;

        case 15:
            Event_WaitTimer(Q12(2.0f), false);

            // Warp Silver Medallion pickup.
            g_WorldObject_SilverH.position.vx = Q12(-98.8f);
            g_WorldObject_SilverH.rotation.vx = Q12_ANGLE(-90.0f);
            g_WorldObject_SilverH.position.vy = Q12(0.0f);
            g_WorldObject_SilverH.position.vz = Q12(22.8f);
            g_WorldObject_SilverH.rotation.vy = Q12_ANGLE(28.5f);

            // Warp camera.
            Event_CameraPositionSet(NULL,
                                    Q12(-101.39f), Q12(-2.6099f), Q12(22.13f),
                                    Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                    true);
            Event_CameraLookAtSet(NULL,
                                  Q12(-98.4f), Q12(-0.14f), Q12(23.1f),
                                  Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                  true);
            break;

        case EventState_Skip:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 2, Q12(0.0f), false);
            break;

        case 18:
        case 20:
            SysWork_StateStepReset();
            break;

        case EventState_MedallionCutsceneEnd:
        case EventState_DontCheckPiano:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);

        case 16:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(2.0f), false);
            break;

        default:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(2.5f), false);

            // Restore player control.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);

            vcReturnPreAutoCamWork(true);
            break;
    }
}

void func_800D7EEC(void) // 0x800D7EEC
{
    Event_DisplayMapMsgWithBg(FILE_TIM_PIANO1_TIM, Q12(0.0f), Q12(0.0f), MAP_MSG(47));
}

void PianoPuzzle_Control(bool playNote) // 0x800D7F18
{
    s16    cursorX;
    s16    cursorY;
    q19_12 curCursorX;
    s32    keyIdx;
    s32    i;

    if (!playNote)
    {
        g_PianoKeyCounter = 0;
        g_PianoCursorY    = 0;
        g_PianoCursorX    = 0;
        D_800DD594        = false;
        return;
    }

    if (g_SysWork.sysStateSteps[1] & 1)
    {
        if (g_SysWork.sysStateSteps[1] == 1)
        {
            Event_WaitTimer(Q12(0.3f), true);
        }
        if (g_SysWork.sysStateSteps[1] == 3)
        {
            Event_WaitTimer(Q12(0.2f), true);
        }
        if (g_SysWork.sysStateSteps[1] == 5)
        {
            Event_WaitTimer(Q12(0.3f), true);
        }
        return;
    }

    if (g_SysWork.sysStateSteps[1] == 4)
    {
        SD_Call(Sfx_Unk1421);
        SysWork_StateStepSet(1, 5);
        return;
    }

    g_SysWork.sysStateSteps[1] = 0;

    if (g_PianoKeyCounter == 5)
    {
        Savegame_EventFlagSet(EventFlag_M1S01_PianoPuzzleSolved);
        SysWork_StateStepIncrement(0);
        return;
    }

    if (Pad_ButtonClickedBind(cancel))
    {
        SysWork_StateStepIncrement(0);
        return;
    }

    g_PianoCursorX += (g_Controller0->normalizedSticks.sticks_0.leftX * Q12(4.0f)) / 75;
    g_PianoCursorX  = CLAMP_RANGE(g_PianoCursorX, Q12(-89.0f), Q12(85.0f));

    g_PianoCursorY += (g_Controller0->normalizedSticks.sticks_0.leftY * Q12(4.0f)) / 75;
    g_PianoCursorY  = CLAMP_RANGE(g_PianoCursorY, Q12(-71.0f), Q12(84.0f));

    Game_TimerUpdate();

    cursorX = FP_FROM(g_PianoCursorX, Q12_SHIFT) + 8;
    cursorY = FP_FROM(g_PianoCursorY, Q12_SHIFT) + 8;
    Gfx_CursorDraw(cursorX, cursorY, 8, 8, 0, 64, 32, 32, 128, 192, 0, 12);

    if (Pad_ButtonClickedBind(enter))
    {
        if (FP_FROM(g_PianoCursorY, Q12_SHIFT) >= 34)
        {
            curCursorX = FP_FROM(g_PianoCursorX, Q12_SHIFT);
            if (curCursorX < -64)
            {
                keyIdx = 0;
            }
            else if (curCursorX < -39)
            {
                keyIdx = 2;
            }
            else if (curCursorX < -14)
            {
                keyIdx = 4;
            }
            else if (curCursorX < 11)
            {
                keyIdx = 5;
            }
            else if (curCursorX < 36)
            {
                keyIdx = 7;
            }
            else if (curCursorX < 61)
            {
                keyIdx = 9;
            }
            else
            {
                keyIdx = ARRAY_SIZE(g_PianoKeys);
            }
        }
        else
        {
            for (i = 0; i < ARRAY_SIZE(g_PianoKeys); i++)
            {
                if (FP_FROM(g_PianoCursorX, Q12_SHIFT) < g_PianoKeys[i])
                {
                    break;
                }
            }

            keyIdx = i;
        }

        switch (keyIdx)
        {
            case 0:
                SysWork_StateStepSet(1, 1);
                SD_Call(Sfx_Unk1438);
                g_PianoCursorX = Q12(-77.0f);
                break;

            case 1:
                SysWork_StateStepSet(1, 3);
                SD_Call(Sfx_Unk1445);
                g_PianoCursorX = Q12(-64.0f);
                break;

            case 2:
                SysWork_StateStepSet(1, 3);
                SD_Call(Sfx_Unk1445);
                g_PianoCursorX = Q12(-52.0f);
                break;

            case 3:
                SysWork_StateStepSet(1, 1);
                SD_Call(Sfx_Unk1439);
                g_PianoCursorX = Q12(-39.0f);
                break;

            case 4:
                SysWork_StateStepSet(1, 1);
                SD_Call(Sfx_Unk1440);
                g_PianoCursorX = Q12(-27.0f);
                break;

            case 5:
                SysWork_StateStepSet(1, 1);
                SD_Call(Sfx_Unk1441);
                g_PianoCursorX = Q12(-2.0f);
                break;

            case 6:
                SysWork_StateStepSet(1, 1);
                SD_Call(Sfx_Unk1442);
                g_PianoCursorX = Q12(8.0f);
                break;

            case 7:
                SysWork_StateStepSet(1, 3);
                SD_Call(Sfx_Unk1445);
                g_PianoCursorX = Q12(23.0f);
                break;

            case 8:
                SysWork_StateStepSet(1, 1);
                SD_Call(Sfx_Unk1443);
                g_PianoCursorX = Q12(36.0f);
                break;

            case 9:
                SysWork_StateStepSet(1, 3);
                SD_Call(Sfx_Unk1445);
                g_PianoCursorX = Q12(48.0f);
                break;

            case 10:
                SysWork_StateStepSet(1, 3);
                SD_Call(Sfx_Unk1445);
                g_PianoCursorX = Q12(64.0f);
                break;

            case 11:
                SysWork_StateStepSet(1, 1);
                SD_Call(Sfx_Unk1444);
                g_PianoCursorX = Q12(73.0f);
                break;
        }

        switch (keyIdx)
        {
            case 0:
            case 3:
            case 4:
            case 5:
            case 6:
            case 8:
            case 11:
                g_PianoKeyCounter = 0;
                return;

            default:
                if (keyIdx == g_PianoKeySequence[0])
                {
                    g_PianoKeyCounter = 1;
                    return;
                }

                if (g_PianoKeySequence[g_PianoKeyCounter] == keyIdx)
                {
                    g_PianoKeyCounter++;
                    return;
                }

                g_PianoKeyCounter = 0;
                break;
        }
    }
}

void func_800D857C(void) // 0x800D857C
{
    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_InvItemCmd(InvItemCmd_QueueLoad, InvItemId_SilverMedallion, 0, false);
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &playerChara, 59, false);
            SysWork_StateStepIncrement(0);

        case 3:
            Event_InvItemCmd(InvItemCmd_AwaitLoad, InvItemId_SilverMedallion, 0, false);
            break;

        case 4:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AwaitAnimEnd, &playerChara, 0, false);
            break;

        case 5:
            if (Gfx_PickupItemAnimate(InvItemId_SilverMedallion))
            {
                Event_DisplayMapMsg(true, MAP_MSG(30), 6, 7, 0, false);
            }

            Savegame_EventFlagSet(EventFlag_M1S01_PickupSilverMedallion);
            break;

        case 6:
            Event_InvItemCmd(InvItemCmd_AddItem, InvItemId_SilverMedallion, 1, false);
            SysWork_StateStepSet(0, 8);
            break;

        case 7:
            Savegame_EventFlagClear(EventFlag_M1S01_PickupSilverMedallion);
            SysWork_StateStepIncrement(0);

        case 8:
            Event_CharaAnimPlayToEnd(&playerChara, 60);
            break;

        default:
            // Restore player control.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;
    }
}

void func_800D8794(void) // 0x800D8794
{
    Event_DisplayMapMsgWithBg(FILE_TIM_LOCKER0_TIM, Q12(0.0f), Q12(0.0f), MAP_MSG(32));
}

extern q19_12 g_Cutscene_Timer;
extern u8 D_800DD598;
extern VECTOR3 g_CameraPositionTarget;
extern VECTOR3 g_CameraLookAtTarget;

void func_800D87C0(void) // 0x800D87C0
{
    typedef enum _EventState
    {
        EventState_Skip = 20
    } e_EventState;

    s32                tmp0;
    s_WorldObjectPose* objTobira00;

    // Skip.
    if (Pad_ButtonClickedBind(skip) &&
        g_SysWork.sysStateSteps[0] > 0 && g_SysWork.sysStateSteps[0] < EventState_Skip)
    {
        if ( g_SysWork.sysStateSteps[0]  < 4)
        {
            D_800DD598 = 1;
        }

        SysWork_StateStepSet(0, EventState_Skip);
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 3, Q12(0.0f), false);
            Fs_QueueStartRead(FILE_ANIM_LOCKER_DMS, FS_BUFFER_16);

            D_800DD598 = 0;

            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            if (Fs_QueueChunksLoad())
            {
                SysWork_StateStepIncrement(0);
            }

            if (g_SysWork.sysStateSteps[0] != 2)
            {
                Chara_Load(2, Chara_Cat, g_SysWork.npcBoneCoordBuffer, 4, NULL, NULL);
            }
            break;

        case 3:
            Event_ScreenFadeCmd(ScreenFadeCmd_Wait, true, 2, Q12(0.0f), false);
            break;

        case 4:
            Dms_HeaderFixOffsets((s_DmsHeader* )FS_BUFFER_16);

            g_Cutscene_Timer = Q12(0.0f);

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &playerChara, 85, false);
            func_8003D03C();
            Player_EmptyWeaponHandSet();
            SysWork_StateStepIncrement(0);

        case 5:
            g_Cutscene_Timer = MIN((g_Cutscene_Timer + Q12_MULT_PRECISE(g_DeltaTime, Q12(12.0f))), Q12(8.0f));
            SysWork_StateStepIncrement(0);

        case 6:
            Chara_ProcessLoads();
            Chara_Spawn(Chara_Cat, 0, Q12(-60.0f), Q12(18.0f), Q12(0.0f), 3);
            SysWork_StateStepIncrement(0);

        case 7:
            g_Cutscene_Timer = MIN((g_Cutscene_Timer + Q12_MULT_PRECISE(g_DeltaTime, Q12(12.0f))), Q12(8.0f));
            if (g_Cutscene_Timer >= Q12(8.0f))
            {
                SysWork_StateStepIncrement(0);
            }
            break;

        case 8:
            g_SysWork.npcs[0].model.stateStep = 0;
            g_SysWork.npcs[0].model.controlState++;

            Savegame_EventFlagSet(EventFlag_76);

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &playerChara, 122, false);
            func_800892DC(18, 96);
            Sfx_WithFlagsPlay(Sfx_Unk1437, &Q12_VECTOR3(-59.0f, -0.5f, 18.6f), Q8(0.5f), SfxFlag_None);
            Sfx_WithFlagsPlay(Sfx_Unk1436, &Q12_VECTOR3(-58.8f, -1.2f, 18.8f), Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 9:
            g_Cutscene_Timer = MIN((g_Cutscene_Timer + Q12_MULT_PRECISE(g_DeltaTime, Q12(18.0f))), Q12(33.0f));
            if (g_Cutscene_Timer >= Q12(33.0f))
            {
                SysWork_StateStepIncrement(0);
            }

            g_Cutscene_Timer = MAX(g_Cutscene_Timer, Q12(16.0f));
            break;

        case 10:
            g_SysWork.npcs[0].model.stateStep = 0;
            g_SysWork.npcs[0].model.controlState++;

            SysWork_StateStepIncrement(0);

        case 11:
            g_Cutscene_Timer = MIN((Q12_MULT_PRECISE(g_DeltaTime, Q12(21.0f)) + g_Cutscene_Timer), Q12(48.0f));
            if (g_Cutscene_Timer >= Q12(48.0f))
            {
                SysWork_StateStepIncrement(0);
            }

            g_Cutscene_Timer = MAX(g_Cutscene_Timer, Q12(34.0f));
            break;

        case 12:
            Event_DisplayMapMsg(false, MAP_MSG(33), 0, 0, 0, false); // "Just a cat..."

            g_Cutscene_Timer = MIN((Q12_MULT_PRECISE(g_DeltaTime, Q12(20.0f)) + g_Cutscene_Timer), Q12(121.0f));
            g_Cutscene_Timer = MAX(g_Cutscene_Timer, Q12(49.0f));
            break;

        case 13:
            g_Cutscene_Timer = MIN((Q12_MULT_PRECISE(g_DeltaTime, Q12(20.0f)) + g_Cutscene_Timer), Q12(121.0f));
            if (g_Cutscene_Timer >= Q12(121.0f))
            {
                SysWork_StateStepIncrement(0);
            }

            g_Cutscene_Timer = MAX(g_Cutscene_Timer, Q12(49.0f));
            break;

        case 14:
            Sfx_WithFlagsPlay(Sfx_Unk1448, &Q12_VECTOR3(-56.4f, -0.8f, 16.8f), Q8(0.5f), SfxFlag_None);
            Chara_ModelCharaIdClear(&g_SysWork.npcs[0], 0, 0);
            SysWork_StateStepIncrement(0);

        case 15:
            Event_WaitTimer(Q12(1.0f), false);
            break;

        case 16:
            Sfx_WithFlagsPlay(Sfx_Unk1449, &Q12_VECTOR3(-56.4f, -0.8f, 16.8f), Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 17:
            Event_WaitTimer(Q12(2.0f), false);
            break;

        case 18:
            Sfx_WithFlagsPlay(Sfx_Unk1446, &Q12_VECTOR3(-57.6f, 0.0f, 16.0f), Q8(0.5f), SfxFlag_None);
            Sfx_WithFlagsPlay(Sfx_Unk1364, &Q12_VECTOR3(-57.6f, 0.0f, 16.0f), Q8(0.25f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 19:
            Event_WaitTimer(Q12(3.0f), false);
            break;

        case 20:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        default:
            g_Cutscene_Timer                         = NO_VALUE;
            playerChara.position.vx += FP_FROM((Math_Sin(playerChara.rotation.vy + Q12_ANGLE(180.0f)) * Q12(1.1f)), Q12_SHIFT);
            playerChara.position.vz += FP_FROM((Math_Cos(playerChara.rotation.vy + Q12_ANGLE(180.0f)) * Q12(1.1f)), Q12_SHIFT);

            vcReturnPreAutoCamWork(true);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);

            Savegame_EventFlagSet(EventFlag_77);

            // Restore player control.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);

            Chara_BonesInit(0);

            Savegame_EventFlagSet(EventFlag_76);
            Chara_ModelCharaIdClear(&g_SysWork.npcs[0], 0, 0);
            func_8003D01C();

            if (D_800DD598 == 0)
            {
                Player_WeaponAttackRestore();
            }
            break;
    }

    if (g_Cutscene_Timer >= Q12(0.0f))
    {
        Dms_CharacterTransformGet(&playerChara.position, &playerChara.rotation, "HERO", g_Cutscene_Timer, (s_DmsHeader*)FS_BUFFER_16);

        if (g_Cutscene_Timer >= Q12(16.0f))
        {
            Dms_CharacterTransformGet(&g_SysWork.npcs[0].position, &g_SysWork.npcs[0].rotation, "CAT", g_Cutscene_Timer, (s_DmsHeader*)FS_BUFFER_16);
        }

        vcChangeProjectionValue(Dms_CameraTargetsGet(&g_CameraPositionTarget, &g_CameraLookAtTarget, NULL, g_Cutscene_Timer, (s_DmsHeader*)FS_BUFFER_16));
        vcUserCamTarget(&g_CameraPositionTarget, NULL, true);
        vcUserWatchTarget(&g_CameraLookAtTarget, NULL, true);
    }

    if (Savegame_EventFlagGet(EventFlag_76) && !Savegame_EventFlagGet(EventFlag_77))
    {
        objTobira00 = &g_WorldObject_Tobira00;
        tmp0 = MIN((g_Cutscene_Timer - Q12(16.0f)), Q12(2.0f));

        objTobira00->rotation.vy = Q12_MULT_PRECISE(tmp0, Q12_ANGLE(-4.1f)) - Q12_ANGLE(42.05f);
    }
}

void Map_WorldObjectsInit(void) // 0x800D91EC
{
    WorldObject_Init(&g_WorldObject_Tobira00, "TOBIRA00", -58.81f, -1.3f, 18.0198f, 0.0f, 0.0f, 0.0f);
    WorldObject_Init(&g_WorldObject_Tobira01, "TOBIRA01", -58.81f, -1.3f, 18.0198f, 0.0f, 0.0f, 0.0f);
    WorldObject_PosePositionInit(&g_WorldObject_HakoHid, "HAKO_HID", -58.81f, -1.4f, 18.0198f);

    WorldObject_ModelNameSet(&g_WorldObject_HandHid, "HAND_HID");
    WorldObject_ModelNameSet(&g_WorldObject_Hand2Hi, "HAND2_HI");
    WorldObject_ModelNameSet(&g_WorldObject_PanelgH, "PANELG_H");

    Math_Vector3Set(&D_800DF080, Q12(-59.2f), Q12(-1.0f), Q12(99.5f));

    WorldObject_PosePositionInit(&g_WorldObject_AcidHid, "ACID_HID", -101.2f, -1.3f, 100.3f);
    WorldObject_PosePositionInit(&g_WorldObject_Futa1Hi, "FUTA1_HI", -98.2798f, -0.58f, 19.93f);
    WorldObject_PosePositionInit(&g_WorldObject_Futa2Hi, "FUTA2_HI", -99.44f, -0.85f, 19.93f);

    if (!Savegame_EventFlagGet(EventFlag_M1S01_PianoPuzzleSolved))
    {
        WorldObject_Init(&g_WorldObject_SilverH, "SILVER_H", -99.0f, -2.68f, 23.73f, 0.0f, 0.0f, 0.0f);
    }
    else
    {
        WorldObject_Init(&g_WorldObject_SilverH, "SILVER_H", -98.8f, 0.0f, 22.8f, -90.0f, 28.5f, 0);
    }

    if (g_SavegamePtr->gameDifficulty == GameDifficulty_Easy)
    {
        Chara_SpawnFlagsSet(Chara_GreyChildOrMumbler, 10, CharaSpawnFlag_None);
    }
    else if (g_SavegamePtr->gameDifficulty == GameDifficulty_Hard)
    {
        Chara_SpawnFlagsSet(Chara_GreyChildOrMumbler, 0, CharaSpawnFlag_0 | CharaSpawnFlag_1);
        Chara_SpawnFlagsSet(Chara_GreyChildOrMumbler, 1, CharaSpawnFlag_0 | CharaSpawnFlag_1);
        Chara_SpawnFlagsSet(Chara_GreyChildOrMumbler, 2, CharaSpawnFlag_0 | CharaSpawnFlag_1);
    }

    WorldObject_ModelNameSet(&g_CommonWorldObjects[0], g_CommonWorldObjectNames[2]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[1], g_CommonWorldObjectNames[3]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[2], g_CommonWorldObjectNames[4]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[3], g_CommonWorldObjectNames[5]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[4], g_CommonWorldObjectNames[6]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[5], g_CommonWorldObjectNames[7]);
}

void Map_WorldObjectsUpdate(void) // 0x800D9514
{
    MAP_CHUNK_CHECK_VARIABLE_DECL();
    if (PLAYER_IN_MAP_CHUNK(vx, 1, -2, -1, -2) && PLAYER_IN_MAP_CHUNK(vz, 1, 3, -1, 3))
    {
        if (Savegame_EventFlagGet(EventFlag_74))
        {
            if (!Savegame_EventFlagGet(EventFlag_M1S01_PickupGoldMedallion))
            {
                WorldObjects_Add(&g_WorldObject_PanelgH, &D_800DF080, &SVECTOR3_Zero);
            }
            WorldObjects_Add(&g_WorldObject_Hand2Hi, &D_800DF080, &SVECTOR3_Zero);
        }
        else
        {
            WorldObjects_Add(&g_WorldObject_HandHid, &D_800DF080, &SVECTOR3_Zero);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -3, -1, -3) && PLAYER_IN_MAP_CHUNK(vz, 1, 3, -1, 3))
    {
        if (!Savegame_EventFlagGet(EventFlag_M1S01_PickupChemical))
        {
            WorldObjects_Add(&g_WorldObject_AcidHid.object, &g_WorldObject_AcidHid.position, &SVECTOR3_Zero);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -2, -1, -2) && PLAYER_IN_MAP_CHUNK(vz, 0, 0, -1, 1))
    {
        func_800D9EC4();
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -3, -1, -3) && PLAYER_IN_MAP_CHUNK(vz, 0, 0, -1, 1))
    {
        if (Savegame_EventFlagGet(EventFlag_71))
        {
            WorldObjects_Add(&g_WorldObject_Futa1Hi.object, &g_WorldObject_Futa1Hi.position, &SVECTOR3_Zero);
        }
        else
        {
            WorldObjects_Add(&g_WorldObject_Futa2Hi.object, &g_WorldObject_Futa2Hi.position, &SVECTOR3_Zero);
        }

        if (!Savegame_EventFlagGet(EventFlag_M1S01_PickupSilverMedallion))
        {
            WorldObjects_Add(&g_WorldObject_SilverH.object, &g_WorldObject_SilverH.position, &g_WorldObject_SilverH.rotation);
        }
    }

    if (!Savegame_EventFlagGet(EventFlag_240))
    {
        if (PLAYER_IN_MAP_CHUNK(vx, 1, -4, -1, -4) && PLAYER_IN_MAP_CHUNK(vz, 1, 2, -1, 2))
        {
            if (playerChara.position.vx < Q12(-138.5999f))
            {
                if (vcRetCamMvSmoothF() == VC_MV_CHASE)
                {
                    Sfx_WithFlagsPlay(Sfx_Unk1450, &Q12_VECTOR3(142.6f, -0.4f, 58.8f), Q8_CLAMPED(1.0f), SfxFlag_NoDistAtten);
                    Savegame_EventFlagSet(EventFlag_240);
                }
            }
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -2, -1, -2) && PLAYER_IN_MAP_CHUNK(vz, 1, 3, -1, 3))
    {
        if (!Savegame_EventFlagGet(EventFlag_M1S01_HandgunBullets0))
        {
            WorldObjects_Add(&g_CommonWorldObjects[3], &g_CommonWorldObjectPoses[0].position, &g_CommonWorldObjectPoses[0].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -2, -1, -2) && PLAYER_IN_MAP_CHUNK(vz, 1, 4, -1, 4))
    {

        if (!Savegame_EventFlagGet(EventFlag_M1S01_FirstAidKit))
        {
            WorldObjects_Add(&g_CommonWorldObjects[0], &g_CommonWorldObjectPoses[1].position, &g_CommonWorldObjectPoses[1].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -4, -1, -4) && PLAYER_IN_MAP_CHUNK(vz, 1, 4, -1, 4))
    {
        if (!Savegame_EventFlagGet(EventFlag_M1S01_HealthDrink))
        {
            WorldObjects_Add(&g_CommonWorldObjects[1], &g_CommonWorldObjectPoses[2].position, &g_CommonWorldObjectPoses[2].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -4, -1, -4) && PLAYER_IN_MAP_CHUNK(vz, 0, 0, -1, 1))
    {
        if (!Savegame_EventFlagGet(EventFlag_M1S01_HandgunBullets1))
        {
            WorldObjects_Add(&g_CommonWorldObjects[3], &g_CommonWorldObjectPoses[3].position, &g_CommonWorldObjectPoses[3].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -1, 0, 0) && PLAYER_IN_MAP_CHUNK(vz, 1, 2, -1, 2))
    {
        if (!Savegame_EventFlagGet(EventFlag_M1S01_HandgunBullets2))
        {
            WorldObjects_Add(&g_CommonWorldObjects[3], &g_CommonWorldObjectPoses[4].position, &g_CommonWorldObjectPoses[4].rotation);
        }
    }
}

void func_800D9DDC(void) // 0x800D9DDC
{
    if (g_SavegamePtr->gameDifficulty != GameDifficulty_Hard)
    {
        if (Savegame_EventFlagGet(EventFlag_M1S01_PickupChemical))
        {
            Chara_SpawnFlagsSet(Chara_GreyChildOrMumbler, 0, CharaSpawnFlag_0 | CharaSpawnFlag_1);
        }

        if (Savegame_EventFlagGet(EventFlag_M1S01_PickupGoldMedallion))
        {
            Chara_SpawnFlagsSet(Chara_GreyChildOrMumbler, 1, CharaSpawnFlag_0 | CharaSpawnFlag_1);

            if (g_SavegamePtr->gameDifficulty == GameDifficulty_Normal)
            {
                Chara_SpawnFlagsSet(Chara_GreyChildOrMumbler, 2, CharaSpawnFlag_0 | CharaSpawnFlag_1 | CharaSpawnFlag_2);
            }
        }
    }

    if (Savegame_EventFlagGet(EventFlag_77))
    {
        Chara_SpawnFlagsSet(Chara_GreyChildOrMumbler, 3, CharaSpawnFlag_0 | CharaSpawnFlag_1);

        if (g_SavegamePtr->gameDifficulty == GameDifficulty_Easy)
        {
            Chara_SpawnFlagsSet(Chara_GreyChildOrMumbler, 7, CharaSpawnFlag_None);
        }
    }
}

void func_800D9EC4(void) // 0x800D9EC4
{
    static const VECTOR3 soundPos = VECTOR3(-58.81f, -1.4f, 18.0198f);

    if (!Savegame_EventFlagGet(EventFlag_76))
    {
        if (D_800DD57C == 0)
        {
            if (Rng_GenerateUInt(0, 31) == 0) // 1 in 32 chance.
            {
                D_800DD57C = -Rng_GenerateUInt(2, 9);
            }
        }

        if (D_800DD57C != 0)
        {
            if (ABS(D_800DD57C) == 1)
            {
                g_WorldObject_Tobira00.rotation.vy += D_800DD57C;
            }
            g_WorldObject_Tobira00.rotation.vy += D_800DD57C;

            if (D_800DD57C > 0)
            {
                if (g_WorldObject_Tobira00.rotation.vy > 0)
                {
                    Sfx_WithFlagsPlay(Sfx_Unk1435, &soundPos, Q8(0.5f), SfxFlag_None);
                    g_WorldObject_Tobira00.rotation.vy = 0;

                    if (D_800DD57C >= 8 && Rng_GenerateUInt(0, 3) == 0) // 1 in 4 chance.
                    {
                        D_800DD57C = 0;
                    }
                    else
                    {
                        D_800DD57C = (Rng_GenerateUInt(0, 3) - D_800DD57C) + 1;
                    }
                    D_800DD57C = MIN(D_800DD57C, 0);
                }
            }
            else if (g_WorldObject_Tobira00.rotation.vy < Q12_ANGLE(-1.8f))
            {
                Sfx_WithFlagsPlay(Sfx_Unk1435, &soundPos, Q8(0.5f), SfxFlag_None);

                D_800DD57C = ~Rng_GenerateUInt(0, 3) - D_800DD57C;
                D_800DD57C = MAX(D_800DD57C, 1);
            }
        }
    }
    else if (!Savegame_EventFlagGet(EventFlag_77))
    {
        WorldObjects_Add(&g_WorldObject_HakoHid.object, &g_WorldObject_HakoHid.position, &SVECTOR3_Zero);
    }
    else
    {
        g_WorldObject_Tobira00.rotation.vy = Q12_ANGLE(-2.9f);
    }

    WorldObjects_Add(&g_WorldObject_Tobira00.object, &g_WorldObject_Tobira00.position, &g_WorldObject_Tobira00.rotation);
    g_WorldObject_Tobira01.rotation.vy = g_WorldObject_Tobira00.rotation.vy;
    WorldObjects_Add(&g_WorldObject_Tobira01.object, &g_WorldObject_Tobira01.position, &g_WorldObject_Tobira01.rotation);
}
