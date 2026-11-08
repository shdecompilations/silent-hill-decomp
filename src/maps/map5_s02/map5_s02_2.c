#include "bodyprog/bodyprog.h"
#include "bodyprog/chara/dms.h"
#include "bodyprog/events/event_data.h"
#include "bodyprog/gfx/map_effects.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player/player.h"
#include "main/rng.h"
#include "maps/map5/map5_s02.h"
#include "maps/common/characters/kaufmann.h"
#include "inline_no_dmpsx.h"
#include <psyq/gtemac.h>
#include "maps/common/particle.h"
#include "maps/common/characters/player.h"
#include "maps/common/characters/stalker.h"

#include "../src/maps/common/particles/particle.c"


// player.c and stalker.c must stay in the same TU because they share an
// identical VECTOR3 compound literal that the compiler deduplicates into
// a single rodata entry. Splitting them into separate TUs produces two 
// copies, inflating rodata by 0x10 bytes.
//
// Could indicate that characters weren't built as separate .c files,
// but a single .c that would include the code for them as needed.
// (maybe a characters.c that had every character and conditionally included them based on what map needed?)
// 
// map5_s02 is currently the only map where this deduplication has been observed.
// Other maps use separate per-character splits for now, as proper splits make handling
// version differences easier. 
#include "../src/maps/common/characters/player.c"

#include "../src/maps/common/characters/stalker.c" // 0x800CDC08

#include "../src/maps/common/characters/kaufmann.c"

#include "../src/maps/common/chara_util.c" // 0x800D3AFC

#include "../src/maps/common/map_util.c" // 0x800D450C

#include "maps/common/shared/Map_RoomBgmInit_CheckCond.h" // 0x800D45B4

#include "maps/common/shared/Map_RoomBgmInit_0_s02_CondTrue.h" // 0x800D45E8

#include "maps/common/shared/Map_RoomBgmInit_0_s02_CondFalse.h" // 0x800D4664

void GameBoot_LoadScreen_StageString(void) {}

#include "maps/common/shared/Event_CutsceneTimerAdvance.h" // 0x800D48F8

#include "maps/common/shared/MapEvent_DoorJammed.h" // 0x800D4990

#include "maps/common/shared/MapEvent_DoorLocked.h" // 0x800D4A24

const char* MAP_MESSAGES[] = {
    #include "maps/common/shared/map_msg_common.h"
#if VERSION_REGION_IS(NTSC)
    /* 15 */ "~J0(8.7)\t~E ",
    /* 16 */ "~J0(1.2)\tAre_you_OK? ~E ",
    /* 17 */ "~J0(1.4)\tYeah,_I_guess_so. ",
    /* 18 */ "~J0(3.0)\tBut_I'm_beat. ~N\n\t\t\tI_thought_I_was_a_goner_there. ~E ",
    /* 19 */ "~J1(3.0)\tSo,_how'd_it_go? ~N\n\t\t\tDid_you_find_a_way_out? ",
    /* 20 */ "~J1(2.4)\tNo._Not_yet. ",
    /* 21 */ "~J1(1.1)\tHow_about_you? ",
    /* 22 */ "~J1(1.0)\tZip. ",
    /* 23 */ "~J1(1.8)\tBut_it's_too_soon_to_give_up. ",
    /* 24 */ "~J1(2.8)\tThis_craziness_can't ~N\n\t\t\tgo_on_forever. ",
    /* 25 */ "~J1(3.4)\tA_military_rescue_squad ~N\n\t\t\tshould_be_here_any_time_now. ",
    /* 26 */ "~J1(2.8)\tIf_they_come_through_the_town, ~N\n\t\t\twe're_home_free. ",
    /* 27 */ "~J1(1.7)\tI_hope_so. ",
    /* 28 */ "~J1(1.4)\tI_better_get_going. ",
    /* 29 */ "~J1(3.4)\tThis_isn't_the_time_to_stand ~N\n\t\t\taround_flapping_our_gums. ~E ",
    /* 30 */ "~J0(2.0)\tDo_you_know ~N\n\t\t\ta_girl_called_Alessa? ~E ",
    /* 31 */ "~J0(1.0)\tNo. ~E ",
    /* 32 */ "\tKaufmann_must_have_dropped ~N\n\tthis. ",
    /* 33 */ "\tLooks_like_it's_full_of_stuff... ~E ",
    /* 34 */ "\tWhat_is_this? ~N\n\tDrugs? ~E ",
    /* 35 */ "~C3\tAugust_20 ",
    /* 36 */ "~C3\tHe_came_by._I_handed_over_the ~N\n\t\tpackage_that_the_woman_left_here. ",
    /* 37 */ "~C3\tSeptember_12 ",
    /* 38 */ "~C3\tHe_showed_up_at_Norman's,_too. ",
    /* 39 */ "~C3\tDon't_want_to_be_involved_with ~N\n\t\tthe_likes_of_them_anymore,_but... ",
    /* 40 */ "~C3\tI'm_getting_creeped_out ~N\n\t\teven_more_than_before. ",
    /* 41 */ "~C3\tThought_of_leaving_town,_but ~N\n\t\tI'm_afraid_of_what_will_happen ~N\n\t\tif_I_do. ~E ",
    /* 42 */ "~C3\t3_loaves_of_bread. ~N\n\t\t3_cartons_of_milk. ~N\n\t\t2_dozen_eggs. ",
    /* 43 */ "~C3\tDeliver_to_back_door_daily_8_a.m. ~N\n\t\tRear_entrance_code_0886 ~N\n\t\t________________________Norman_Young ~E ",
    /* 44 */ "~C3\tNorman's_Grand_Opening. ~E ",
    /* 45 */ "\tThere_is_a_drawer. ~N\n\tDo_you_want_to_open_it? ~S4 ",
    /* 46 */ "\tUsed_the_ ~C2 Safe_key ~C7 . ~E ",
    /* 47 */ "\tThere_is_a_ ~C2 Safe_key ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 48 */ "\tIt's_locked. ~E ",
    /* 49 */ "\tI've_got_a_ ~C2 Receipt ~C7 . ~E ",
    /* 50 */ "\tI've_got_a_ ~C2 Kaufmann_key ~C7 . ~E "
#elif VERSION_REGION_IS(NTSCJ)
    /* 15 */ "~J0(8.7)\t~E ",
    /* 16 */ "~J0(1.2)\t大丈夫か？ ~E ",
    /* 17 */ "~J0(1.4)\tああ　何とかな ",
    /* 18 */ "~J0(3.0)\t不意をつかれたんだ ~N\n\t\t\t死ぬかと思ったよ ~E ",
    /* 19 */ "~J1(3.0)\tところでどうだ？ ~N\n\t\t\t町から出られそうか？ ",
    /* 20 */ "~J1(2.4)\tいや　今のところは…… ",
    /* 21 */ "~J1(1.1)\tあんたの方は？ ",
    /* 22 */ "~J1(1.0)\t駄目だ ",
    /* 23 */ "~J1(1.8)\tだが　あきらめるのは早い ",
    /* 24 */ "~J1(2.8)\tこんな馬鹿げた事態が ~N\n\t\t\tいつまでも続くはずがない ",
    /* 25 */ "~J1(3.4)\t今ごろは軍の救助隊が ~N\n\t\t\t近くまで来ているだろう ",
    /* 26 */ "~J1(2.8)\t町を抜ければきっと助かる ",
    /* 27 */ "~J1(1.7)\t……そうだといいな ",
    /* 28 */ "~J1(1.4)\tわたしはそろそろ行く ",
    /* 29 */ "~J1(3.4)\t悠長に話をしている場合ではないからな ~E ",
    /* 30 */ "~J0(2.0)\tアレッサという子を知っているか？ ~E ",
    /* 31 */ "~J0(1.0)\t知らないな ~E ",
    /* 32 */ "\tカウフマンが落としたのか？ ~N\n\tいろいろと入っているようだが…… ~E ",
    /* 33 */ "\tこれは…… ~N\n\tドラッグなのか？ ~E ",
    /* 34 */ "~C3\t８月２０日 ",
    /* 35 */ "~C3\tあいつがやってきた。 ~N\n\t\tあの女から預かっているものを渡す。 ",
    /* 36 */ "~C3\t９月１２日 ",
    /* 37 */ "~C3\tあいつがノーマンのところへも ~N\n\t\t現れたらしい。 ",
    /* 38 */ "~C3\tあんな奴らとはこれ以上 ~N\n\t\t関わり合いになりたくないのだが…… ",
    /* 39 */ "~C3\tあいつには以前にはなかった ~N\n\t\t得体の知れない恐ろしさを感じる。 ",
    /* 40 */ "~C3\tいっそのことこの町を出ようかとも ~N\n\t\t考えたが、それこそどんな目にあうか ~N\n\t\t考えただけでも恐ろしい。 ~E ",
    /* 41 */ "~C3\tパン　　３斤 ~N\n\t\tミルク　３パック ~N\n\t\t卵　　　２ケース ",
    /* 42 */ "~C3\t毎朝８時までに裏口へ届けること ~N\n\t\t裏口のナンバーは０８８６ ~N\n\t\t　　　　　　　　　　ノーマン・ヤング ~E ",
    /* 43 */ "~C3\tノーマン開業祝い ~E ",
    /* 44 */ "\t引き出しがある ~N\n\t開けますか？ ~S4 ",
    /* 45 */ "\t~C2 金庫の鍵 ~C7 を使った ~E ",
    /* 46 */ "\t~C2 金庫の鍵 ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 47 */ "\t鍵がかかっている ~E ",
    /* 48 */ "\t~C2 領収証 ~C7 を手に入れた ~E ",
    /* 49 */ "\t~C2 カウフマンの鍵 ~C7 を手に入れた ~E "
#elif VERSION_REGION_IS(PAL)
    #include "map_msg_pal.h"
#endif
};

void MapEvent_CommonItemTake(void) // 0x800D4AB8
{
    u32 pickupType;
    s32 eventFlagIdx;

    pickupType   = CommonPickupItemId_FirstAidKit;
    eventFlagIdx = 0;

    switch (g_MapEventData->mapPointIdx)
    {
        case 15:
            pickupType   = CommonPickupItemId_HealthDrink;
            eventFlagIdx = EventFlag_M5S02_HealthDrink0;
            break;

        case 16:
            pickupType   = CommonPickupItemId_HealthDrink;
            eventFlagIdx = EventFlag_M5S02_HealthDrink1;
            break;

        case 17:
            pickupType   = CommonPickupItemId_RifleShells;
            eventFlagIdx = EventFlag_M5S02_RifleShells;
            break;

        case 18:
            pickupType   = CommonPickupItemId_HealthDrink;
            eventFlagIdx = EventFlag_M5S02_HealthDrink2;
            break;
    }

    Event_CommonItemTake(pickupType, eventFlagIdx);
}

void func_800D4B58(void) // 0x800D4B58
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
            Event_DisplayMapMsg(true, MAP_MSG(45), 3, NO_VALUE, 0, false); // "There is a drawer. Do you want to open it?"
            break;

        case 3:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 66, false);
            CutsceneBorder_ForceShow();

            // Warp camera.
            Event_CameraPositionSet(NULL, Q12(135.17f), Q12(-3.3f), Q12(61.09f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(NULL, Q12(134.29f), Q12(-0.21f), Q12(63.18f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);

            // Warp player.
            g_SysWork.playerWork.player.position.vx = Q12(134.9f);
            g_SysWork.playerWork.player.position.vz = Q12(62.45f);
            g_SysWork.playerWork.player.rotation.vy = Q12_ANGLE(-90.0f);

            SysWork_StateStepIncrement(0);

        case 4:
            Event_WaitTimer(Q12(1.5f), false);
            break;

        case 5:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 6:
            Sfx_WithFlagsPlay(Sfx_Unk1593, &Q12_VECTOR3(134.6f, -1.5f, 62.45f), Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 7:
            Event_WaitTimer(Q12(1.0f), false);
            break;

        case 8:
            vcReturnPreAutoCamWork(true);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            Savegame_EventFlagSet(EventFlag_375);

            // Warp player.
            g_SysWork.playerWork.player.position.vx = Q12(135.15f);

            SysWork_StateStepIncrement(0);

        default:
            // Return to gameplay.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;
    }
}

void func_800D4DF8(void) // 0x800D4DF8
{
    Event_ItemTake(InvItemId_SafeKey, DEFAULT_PICKUP_ITEM_COUNT, EventFlag_M5S02_PickupSafeKey, MAP_MSG(47));
}

void func_800D4E24(void) // 0x800D4E24
{
    Savegame_EventFlagSet(EventFlag_397);
    Event_DisplayMapMsgWithBg(FILE_TIM_DIARYINR_TIM, Q12(0.0f), Q12(0.0f), MAP_MSG(35));
}

void func_800D4E64(void) // 0x800D4E64
{
    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Savegame_EventFlagSet(EventFlag_396);

            // Warp player.
            g_SysWork.playerWork.player.position.vx = Q12(135.05f);
            g_SysWork.playerWork.player.position.vz = Q12(59.75f);
            g_SysWork.playerWork.player.rotation.vy = Q12_ANGLE(180.0f);

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 2, Q12(0.0f), false);
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            Event_CharaAnimPlayToEnd(&g_SysWork.playerWork.player, 59);
            break;

        case 3:
            Sfx_WithFlagsPlay(Sfx_Unk1592, &Q12_VECTOR3(134.9f, -0.5f, 59.5f), Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 4:
            Event_DisplayMapMsg(false, MAP_MSG(46), 0, 0, 0, false); // "Used the Safe Key."
            break;

        case 5:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 6:
            Savegame_EventFlagSet(EventFlag_M5S02_UsedSafeKey);
            Sfx_WithFlagsPlay(Sfx_Unk1591, &Q12_VECTOR3(134.7f, -0.5f, 59.5f), Q8(0.5f), SfxFlag_None);

            // Warp camera.
            Event_CameraPositionSet(NULL, Q12(135.26f), Q12(-1.61f), Q12(60.22f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(NULL, Q12(133.36f), Q12(1.12f), Q12(58.01f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);

            g_WorldObject_BDoor1.rotation.vy = Q12_ANGLE(-142.21f);
            SysWork_StateStepIncrement(0);

        case 7:
            Event_WaitTimer(Q12(0.5f), false);
            break;

        case 8:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.0f), false);
            break;

        case 9:
            Event_DisplayMapMsg(false, MAP_MSG(34), 0, 0, 0, false); // "What is this? Drugs?"
            break;

        case 10:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 11:
            vcReturnPreAutoCamWork(true);
            g_WorldObject_BDoor1.rotation.vy = Q12_ANGLE(-7.5f);
            SysWork_StateStepIncrement(0);

        case 12:
            Event_WaitTimer(Q12(0.5f), false);
            break;

        case 13:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            SysWork_StateStepIncrement(0);

        case 14:
            Event_CharaAnimPlayToEnd(&g_SysWork.playerWork.player, 60);
            break;

        default:
            // Return to gameplay.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            vcReturnPreAutoCamWork(true);
            break;
    }
}

void func_800D519C(void) // 0x800D519C
{
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
            Event_CharaAnimPlayToEnd(&g_SysWork.playerWork.player, 59);
            break;

        case 3:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 4:
            Savegame_EventFlagSet(EventFlag_M5S02_UsedSafeKey);
            Sfx_WithFlagsPlay(Sfx_Unk1591, &Q12_VECTOR3(134.7f, -0.5f, 59.5f), Q8(0.5f), SfxFlag_None);
            Event_CameraPositionSet(NULL, Q12(135.26f), Q12(-1.61f), Q12(60.22f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(NULL, Q12(133.36f), Q12(1.12f), Q12(58.01f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);

            g_WorldObject_BDoor1.rotation.vy = Q12_ANGLE(-142.21f);

            g_SysWork.playerWork.player.position.vx = Q12(135.05f);
            g_SysWork.playerWork.player.position.vz = Q12(59.75f);
            g_SysWork.playerWork.player.rotation.vy = Q12_ANGLE(180.f);
            SysWork_StateStepIncrement(0);

        case 5:
            Event_WaitTimer(Q12(0.5f), false);
            break;

        case 6:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.0f), false);
            break;

        case 7:
            Event_DisplayMapMsg(false, MAP_MSG(34), 0, 0, 0, false); // "What is this? Drugs?"
            break;

        case 8:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 9:
            vcReturnPreAutoCamWork(true);
            g_WorldObject_BDoor1.rotation.vy = Q12_ANGLE(-7.5f);
            SysWork_StateStepIncrement(0);

        case 10:
            Event_WaitTimer(Q12(0.5f), false);
            break;

        case 11:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            SysWork_StateStepIncrement(0);

        case 12:
            Event_CharaAnimPlayToEnd(&g_SysWork.playerWork.player, 60);
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            vcReturnPreAutoCamWork(true);
            break;
    }
}

void func_800D5478(void) // 0x800D5478
{
    Event_DisplayMapMsgWithBg(FILE_TIM_PICT_INR_TIM, Q12(0.0f), Q12(0.0f), MAP_MSG(44));
}

void func_800D54A4(void) // 0x800D54A4
{
    Event_DisplayMapMsgWithBg(FILE_TIM_MEMO_INR_TIM, Q12(0.0f), Q12(0.0f), MAP_MSG(42));
}

void func_800D54D0(void) // 0x800D54D0
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
            Event_CharaAnimPlayToEnd(&g_SysWork.playerWork.player, 59);
            break;

        case 3:
            Event_InvItemCmd(InvItemCmd_QueueLoad, InvItemId_KaufmannKey, 0, false);
            SysWork_StateStepIncrement(0);

        case 4:
            Event_DisplayMapMsg(false, MAP_MSG(32), 0, 0, 0, false);
            break;

        case 5:
            Event_InvItemCmd(InvItemCmd_AwaitLoad, InvItemId_KaufmannKey, 0, false);
            break;

        case 6:
            Savegame_EventFlagSet(EventFlag_380);

            if (Gfx_PickupItemAnimate(InvItemId_KaufmannKey))
            {
                Event_DisplayMapMsg(false, MAP_MSG(50), 0, 0, 0, false);

                if (g_SysWork.sysStateSteps[0] != 6)
                {
                    SD_Call(Sfx_MenuConfirm);
                }
            }
            break;

        case 7:
            Event_InvItemCmd(InvItemCmd_QueueLoad, InvItemId_Receipt, 0, false);
            SysWork_StateStepIncrement(0);

        case 8:
            Event_InvItemCmd(InvItemCmd_AwaitLoad, InvItemId_Receipt, 0, false);
            break;

        case 9:
            if (Gfx_PickupItemAnimate(InvItemId_Receipt))
            {
                Event_DisplayMapMsg(false, MAP_MSG(49), 0, 0, 0, false);

                if (g_SysWork.sysStateSteps[0] != 9)
                {
                    SD_Call(Sfx_MenuConfirm);
                }
            }
            break;

        case 10:
            Event_CharaAnimPlayToEnd(&g_SysWork.playerWork.player, 60);
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            Savegame_EventFlagSet(EventFlag_380);
            Event_InvItemCmd(InvItemCmd_AddItem, InvItemId_KaufmannKey, 1, false);
            Event_InvItemCmd(InvItemCmd_AddItem, InvItemId_Receipt, 1, false);
            break;
    }
}

void MapEvent_KaufmannBarFightCutscene(void) // 0x800D5744
{
    VECTOR3  lightIntPos;
    SVECTOR3 unused;
    s32      i;

    if (Pad_ButtonClickedBind(skip) &&
        g_SysWork.sysStateSteps[0] >= 3 && g_SysWork.sysStateSteps[0] < 41)
    {
        SysWork_StateStepSet(0, 43);
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            D_800DA6E8 = 0;
            Screen_FadeTimestepReset();
            CutsceneBorder_ForceShow();
            g_SysWork.sysFlags |= SysFlag_CutsceneActive;

            func_8008D438();

#if VERSION_EQUAL_OR_NEWER(JAP2) // @bugfix?
            Game_TurnFlashlightOn();
#endif

            g_SysWork.lightBoneCoord     = NULL;
            g_SysWork.lensFlareBoneCoord = NULL;
            g_SysWork.lightIntensity     = Q12(1.0f);

            Fs_QueueStartRead(FILE_ANIM_POOL1_DMS, FS_BUFFER_17);
            Fs_QueueWaitForEmpty();
            Dms_HeaderFixOffsets(FS_BUFFER_17);

            g_Cutscene_Timer                                 = Q12(0.0f);
            g_SysWork.playerWork.player.position.vy = Q12(0.0f);

            Chara_Load(0, Chara_Kaufmann, &g_SysWork.npcBoneCoordBuffer[0], CHARA_FORCE_FREE_ALL, NULL, NULL);
            Chara_Load(1, Chara_Mumbler, &g_SysWork.npcBoneCoordBuffer[30], 0, NULL, NULL);

            func_8007F14C(32);
            func_8003D03C();
            Player_EmptyWeaponHandSet();
            WorldGfx_PlayerHeldItemSet(InvItemId_Handgun);

            g_SysWork.playerWork.player.collision.state = CharaCollisionState_Default;

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 153, false);
            D_800D94F4 = 0;

            SysWork_StateStepIncrement(0);
            break;

        case 1:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimLock, &g_SysWork.playerWork.player, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.0f), false);
            break;

        case 2:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimUnlock, &g_SysWork.playerWork.player, 0, false);
            SysWork_StateStepIncrement(0);

        case 3:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(8.0f), Q12(0.0f), Q12(20.0f), true, true);
            break;

        case 4:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 70, false);
            Savegame_EventFlagSet(EventFlag_381);
            SysWork_StateStepIncrement(0);

        case 5:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(8.0f), Q12(21.0f), Q12(31.0f), true, true);
            break;

        case 6:
            Chara_ProcessLoads();
            Chara_Spawn(Chara_Kaufmann, 0, Q12(140.5f), Q12(23.0f), Q12_ANGLE(0.0f), 3);
            SysWork_StateStepIncrement(0);
            break;

        case 7:
            Chara_Spawn(Chara_Mumbler, 0, Q12(140.5f), Q12(23.0f), Q12_ANGLE(0.0f), 17);
            D_800D94F4 = 1;

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 12, false);
            SysWork_StateStepIncrement(0);

        case 8:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(32.0f), Q12(43.0f), true, true);
            break;

        case 9:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 186, false);
            SysWork_StateStepIncrement(0);

        case 10:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(44.0f), Q12(56.0f), true, true);
            break;

        case 11:
            func_8003D01C();
            SysWork_StateStepIncrement(0);

        case 12:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 164, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 11, false);

            ModelAnim_StatusDecrement(&g_SysWork.npcs[1].model.anim);
            SysWork_StateStepIncrement(0);

        case 13:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(63.0f), Q12(93.0f), true, true);
            break;

        case 14:
            Event_CharaAnimPlayToEnd(&g_SysWork.playerWork.player, 165);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(94.0f), Q12(150.0f), false, false);
            break;

        case 15:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 166, false);
            SysWork_StateStepIncrement(0);

        case 16:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(94.0f), Q12(150.0f), true, true);
            break;

        case 17:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 14, false);
            SysWork_StateStepIncrement(0);

        case 18:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(151.0f), Q12(175.0f), true, true);
            break;

        case 19:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 13, false);
            func_8003D03C();

            Savegame_EventFlagClear(EventFlag_381);
            g_SysWork.npcs[1].model.stateStep++;

            SysWork_StateStepIncrement(0);

        case 20:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(176.0f), Q12(206.0f), false, true);
            break;

        case 21:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 51, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 5, false);
            func_800625F4(&Q12_VECTOR3(139.3f, 0.0f, 23.6f), 120, 3, 1);

            D_800D94F4 = 2;
            g_Cutscene_Timer = Q12(207.0f);

            SysWork_StateStepIncrement(0);

        case 22:
            Event_DisplayMapMsgWithAudio(MAP_MSG(16), &D_800DA6E8, &D_800D947C);
            break;

        case 23:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 10, false);
            SysWork_StateStepIncrement(0);

        case 24:
            Event_DisplayMapMsgWithAudio(MAP_MSG(17), &D_800DA6E8, &D_800D947C);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(207.0f), Q12(227.0f), true, false);
            break;

        case 25:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 5, false);
            g_Cutscene_Timer = Q12(228.0f);
            SysWork_StateStepIncrement(0);

        case 26:
            Event_DisplayMapMsgWithAudio(MAP_MSG(19), &D_800DA6E8, &D_800D947C);
            break;

        case 27:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 9, false);
            Model_AnimFlagsClear(&g_SysWork.playerWork.player.model, 2);
            SysWork_StateStepIncrement(0);

        case 28:
            Event_WaitTimer(Q12(1.8f), false);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(229.0f), Q12(294.0f), true, false);
            break;

        case 29:
            Event_DisplayMapMsgWithAudio(MAP_MSG(30), &D_800DA6E8, &D_800D947C);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(229.0f), Q12(294.0f), true, false);
            break;

        case 30:
            SysWork_StateStepIncrement(0);

        case 31:
            Event_WaitTimer(Q12(0.4f), false);
            break;

        case 32:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 1, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 73, false);
            Model_AnimFlagsSet(&g_SysWork.playerWork.player.model, 2);
            SysWork_StateStepIncrement(0);

        case 33:
            Event_DisplayMapMsgWithAudio(MAP_MSG(31), &D_800DA6E8, &D_800D947C);

        case 34:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(8.0f), Q12(295.0f), Q12(315.0f), true, true);
            break;

        case 35:
            Chara_ModelCharaIdClear(&g_SysWork.npcs[0], 0, 0);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 51, false);

            g_Cutscene_Timer = Q12(316.0f);

            Player_WeaponAttackRestore();
            WorldGfx_PlayerPrevHeldItem(&g_SysWork.playerCombat);
            Player_EmptyWeaponHandSet();
            SysWork_StateStepIncrement(0);

        case 36:
            Event_WaitTimer(Q12(0.5f), false);
            break;

        case 37:
            Sfx_WithFlagsPlay(Sfx_DoorOpen0, &Q12_VECTOR3(134.5f, -1.2f, 17.9f), Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 38:
            Event_WaitTimer(Q12(1.0f), false);
            break;

        case 39:
            Sfx_WithFlagsPlay(Sfx_DoorClose0, &Q12_VECTOR3(134.5f, -1.2f, 17.9f), Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 40:
            Event_WaitTimer(Q12(0.6f), false);
            break;

        case 41:
            if (Fs_QueueChunksLoad())
            {
                SysWork_StateStepIncrement(0);
            }
            break;

        case 42:
            SysWork_StateStepReset();
            break;

        case 43:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 44:
            if (!D_800D94F4)
            {
                Chara_ProcessLoads();
                g_SysWork.npcs[0].model.charaId = Chara_Kaufmann;
                Chara_Spawn(Chara_Mumbler, 0, Q12(140.5f), Q12(23.0f), Q12_ANGLE(0.0f), 17);
                D_800D94F4 = 1;
            }

            Chara_ModelCharaIdClear(&g_SysWork.npcs[0], 0, 0);
            Player_WeaponAttackRestore();
            WorldGfx_PlayerPrevHeldItem(&g_SysWork.playerCombat);

            g_Cutscene_Timer = Q12(316.0f);
            SysWork_StateStepIncrement(0);
            break;

        case 45:
            if (D_800D94F4 == 1)
            {
                g_SysWork.npcs[1].model.stateStep = 3;
                func_800625F4(&Q12_VECTOR3(139.3f, 0.0f, 23.6f), 120, 3, 1);
            }

            Fs_QueueWaitForEmpty();
            SysWork_StateStepIncrement(0);
            break;

        case 46:
            for (i = 0; i < ARRAY_SIZE(sharedData_800DFB7C_0_s00); i++)
            {
                if (sharedData_800DFB7C_0_s00[i].field_A == 1)
                {
                    sharedData_800DFB7C_0_s00[i].field_A = 0;
                }
            }

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            SysWork_StateStepIncrement(0);

        default:
            Player_ControlUnfreeze(true);
            SysWork_StateSetNext(SysState_Gameplay);
            g_SysWork.playerWork.player.position.vy = Q12(0.0f);

            func_8003D01C();
            Player_WeaponAttackRestore();
            func_8007F14C((u8)g_SysWork.playerCombat.weaponAttack);

            Model_AnimFlagsSet(&g_SysWork.playerWork.player.model, 2);
            vcReturnPreAutoCamWork(true);

            g_Cutscene_Timer = NO_VALUE;

            Savegame_EventFlagSet(EventFlag_379);
            func_8008D448();

            Game_FlashlightAttribsSet();
            g_SysWork.lightIntensity = Q12(1.0f);

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            g_SysWork.playerWork.player.collision.state = CharaCollisionState_Npc;

            SD_Call(19);
            break;
    }

    if (g_SysWork.sysStateSteps[0] < 27)
    {
        WorldObjects_Add(&g_WorldObject_06LBag.object, &g_WorldObject_06LBag.position, &g_WorldObject_06LBag.rotation);
    }

    if (g_Cutscene_Timer >= Q12(0.0f))
    {
        Dms_CharacterTransformGet(&g_SysWork.playerWork.player.position, &g_SysWork.playerWork.player.rotation, "HERO", g_Cutscene_Timer, FS_BUFFER_17);
        Dms_CharacterTransformGet(&g_SysWork.npcs[0].position, &g_SysWork.npcs[0].rotation, "KAU", g_Cutscene_Timer, FS_BUFFER_17);
        Dms_CharacterTransformGet(&g_SysWork.npcs[1].position, &g_SysWork.npcs[1].rotation, "CLD4", g_Cutscene_Timer, FS_BUFFER_17);

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

void Map_WorldObjectsInit(void) // 0x800D6598
{
    WorldObject_ModelNameSet(&g_WorldObject_SavePad, g_CommonWorldObjectNames[1]);

    WorldObject_PoseInit(&g_WorldObject_UnkPose0, 135.5022f, -1.3f, 21.71f, 0.0f, 11.4f, 0.0f);
    WorldObject_Init(&g_WorldObject_BDoor1, "BDOOR1_H", 134.5318f, 0.0f, 59.471f, 0.0f, 0.0f, 0.0f);

    WorldObject_ModelNameSet(&g_WorldObject_Box[0], "BOX091_H");
    WorldObject_ModelNameSet(&g_WorldObject_Box[1], "BOX191_H");

    Math_Vector3Set(&g_WorldObject_UnkPos[0], Q12(134.3162f), Q12(-0.504f), Q12(62.472f));

    WorldObject_ModelNameSet(&g_WorldObject_Box[2], "BOX092_H");
    WorldObject_ModelNameSet(&g_WorldObject_Box[3], "BOX192_H");

    Math_Vector3Set(&g_WorldObject_UnkPos[1], Q12(134.5752f), Q12(0.0f), Q12(62.4656f));

    WorldObject_Init(&g_WorldObject_Key, "KEY_HIDE", 134.5752f, -0.1067f, 62.4656f, 0.0f, -27.51f, 0.0f);
    WorldObject_Init(&g_WorldObject_Daly, "DALY_HID", 135.9241f, -1.1517f, 60.315f, 0.0f, 9.8f, 0.0f);

    WorldObject_ModelNameSet(&g_WorldObject_Drug, "DRUG_HID");

    WorldObject_PoseInit(&g_WorldObject_UnkPose1[0], 134.6792f, -0.1561f, 59.1995f, 0.0f, -6.4f, 0.0f);
    WorldObject_PoseInit(&g_WorldObject_UnkPose1[1], 134.8038f, -0.1561f, 59.29f, 0.0f, 170.7f, 0.0f);
    WorldObject_PoseInit(&g_WorldObject_UnkPose1[2], 134.8704f, -0.2054f, 59.25f, 0.0f, -3.0f, 20.4f);
    WorldObject_Init(&g_WorldObject_06LBag, "06LBAG_H", 138.8426f, 0.0f, 20.2635f, 0.0f, 180.0f, 0.0f);
    WorldObject_Init(&g_WorldObject_Obj00, "OBJ00_HI", 139.2938f, 0.0f, 25.4083f, 0.0f, 33.5f, 0.0f);

    if (Savegame_EventFlagGet(EventFlag_M5S02_UsedSafeKey))
    {
        g_WorldObject_BDoor1.rotation.vy = Q12_ANGLE(-7.5f);
    }

    WorldObject_ModelNameSet(&g_CommonWorldObjects[0], g_CommonWorldObjectNames[2]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[1], g_CommonWorldObjectNames[3]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[2], g_CommonWorldObjectNames[4]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[3], g_CommonWorldObjectNames[5]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[4], g_CommonWorldObjectNames[6]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[5], g_CommonWorldObjectNames[7]);
}

void Map_WorldObjectsUpdate(void) // 0x800D68FC
{
    MAP_CHUNK_CHECK_VARIABLE_DECL();

    Collision_TriggerFlagBitsClear(CollisionTriggerFlag_All);

    if (PLAYER_IN_MAP_CHUNK(vx, 1, 4, -1, 4) && PLAYER_IN_MAP_CHUNK(vz, 1, 2, -1, 2))
    {
        if (!Savegame_EventFlagGet(EventFlag_375))
        {
            WorldObjects_Add(g_WorldObject_Box, g_WorldObject_UnkPos, &SVECTOR3_Zero);
            WorldObjects_Add(&g_WorldObject_Box[1], g_WorldObject_UnkPos, &SVECTOR3_Zero);
        }
        else
        {
            WorldObjects_Add(&g_WorldObject_Box[2], &g_WorldObject_UnkPos[1], &SVECTOR3_Zero);
            WorldObjects_Add(&(&g_WorldObject_Box[2])[1], &g_WorldObject_UnkPos[1], &SVECTOR3_Zero);

            if (!Savegame_EventFlagGet(EventFlag_M5S02_PickupSafeKey))
            {
                WorldObjects_Add(&g_WorldObject_Key.object, &g_WorldObject_Key.position, &g_WorldObject_Key.rotation);
            }

            Collision_TriggerFlagBitsSet(CollisionTriggerFlag_Objects);
        }

        if (Savegame_EventFlagGet(EventFlag_M5S02_UsedSafeKey))
        {
            WorldObjects_Add(&g_WorldObject_Drug, &g_WorldObject_UnkPose1->position, &g_WorldObject_UnkPose1->rotation);
            WorldObjects_Add(&g_WorldObject_Drug, &g_WorldObject_UnkPose1[1].position, &g_WorldObject_UnkPose1[1].rotation);
            WorldObjects_Add(&g_WorldObject_Drug, &g_WorldObject_UnkPose1[2].position, &g_WorldObject_UnkPose1[2].rotation);
        }

        WorldObjects_Add(&g_WorldObject_BDoor1.object, &g_WorldObject_BDoor1.position, &g_WorldObject_BDoor1.rotation);
        WorldObjects_Add(&g_WorldObject_Daly.object, &g_WorldObject_Daly.position, &g_WorldObject_Daly.rotation);
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, 4, -1, 4) && PLAYER_IN_MAP_CHUNK(vz, 0, 0, -1, 1))
    {
        WorldObjects_Add(&g_WorldObject_SavePad, &g_WorldObject_UnkPose0.position, &g_WorldObject_UnkPose0.rotation);

        if (!Savegame_EventFlagGet(EventFlag_380))
        {
            WorldObjects_Add(&g_WorldObject_Obj00.object, &g_WorldObject_Obj00.position, &g_WorldObject_Obj00.rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, 4, -1, 4) && PLAYER_IN_MAP_CHUNK(vz, 0, 0, -1, 1))
    {
        if (!Savegame_EventFlagGet(EventFlag_M5S02_HealthDrink0))
        {
            WorldObjects_Add(&g_CommonWorldObjects[1], &g_CommonWorldObjectPoses[0].position, &g_CommonWorldObjectPoses[0].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, 4, -1, 4) && PLAYER_IN_MAP_CHUNK(vz, 0, 0, -1, 1))
    {
        if (!Savegame_EventFlagGet(EventFlag_M5S02_HealthDrink1))
        {
            WorldObjects_Add(&g_CommonWorldObjects[1], &g_CommonWorldObjectPoses[1].position, &g_CommonWorldObjectPoses[1].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, 4, -1, 4) && PLAYER_IN_MAP_CHUNK(vz, 1, 2, -1, 2))
    {
        if (!Savegame_EventFlagGet(EventFlag_M5S02_RifleShells))
        {
            WorldObjects_Add(&g_CommonWorldObjects[4], &g_CommonWorldObjectPoses[2].position, &g_CommonWorldObjectPoses[2].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, 4, -1, 4) && PLAYER_IN_MAP_CHUNK(vz, 1, 2, -1, 2))
    {
        if (!Savegame_EventFlagGet(EventFlag_M5S02_HealthDrink2))
        {
            WorldObjects_Add(&g_CommonWorldObjects[1], &g_CommonWorldObjectPoses[3].position, &g_CommonWorldObjectPoses[3].rotation);
        }
    }
}
