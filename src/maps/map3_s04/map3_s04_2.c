#include "bodyprog/bodyprog.h"
#include "bodyprog/chara/dms.h"
#include "bodyprog/events/event_data.h"
#include "bodyprog/world/world_effects.h"
#include "bodyprog/map/terrain.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player/player.h"
#include "main/rng.h"
#include "maps/map3/map3_s04.h"
#include "maps/common/characters/lisa.h"
#include "bodyprog/sound/sound_system.h"
#include "maps/common/particle.h"
#include "maps/common/characters/player.h"
#include "maps/common/characters/puppet_nurse.h"

#include "../src/maps/common/particles/particle.c"

#include "../src/maps/common/characters/player.c"

#include "../src/maps/common/unk_m3s03.c" // 0x800CD574

#include "../src/maps/common/characters/puppet_nurse.c"
#include "../src/maps/common/characters/lisa.c"

#include "../src/maps/common/chara_util.c" // 0x800D1374

#include "../src/maps/common/map_util.c" // 0x800D1D7C

#include "maps/common/shared/Map_RoomBgmInit_CheckCond.h" // 0x800D1E24

#include "maps/common/shared/Map_RoomBgmInit_3_s02_CondTrue.h" // 0x800D1E58

#include "maps/common/shared/Map_RoomBgmInit_3_s02_CondFalse.h" // 0x800D1E98

void GameBoot_LoadScreen_StageString(void) {}

#include "maps/common/shared/MapEvent_DoorJammed.h" // 0x800D2130

const char* MAP_MESSAGES[] = {
    #include "maps/common/shared/map_msg_common.inc"
#if VERSION_REGION_IS(NTSC)
    /* 15 */ "~J1(3.6)\tFinally. ~N\n\t\t\tSomeone_else_who's_OK. ",
    /* 16 */ "~J1(1.0)\tWho_are_you? ",
    /* 17 */ "~J1(3.1)\tMy_name's_Lisa_Garland. ~N\n\t\t\tWhat's_yours? ",
    /* 18 */ "~J1(1.2)\tHarry_Mason. ~E ",
    /* 19 */ "~J0(4.7)\tHarry,_tell_me ~N\n\t\t\twhat's_happening_here? ~N\n\t\t\tWhere_is_everybody? ",
    /* 20 */ "~J0(2.7)\tI_must_have_gotten_knocked_out. ",
    /* 21 */ "~J0(3.2)\tWhen_I_came_to, ~N\n\t\t\teveryone_was_gone. ",
    /* 22 */ "~J0(1.3)\tIt's_awful... ~E ",
    /* 23 */ "~J1(3.8)\tSo_you_don't_know ~N\n\t\t\tanything_either.__Great. ",
    /* 24 */ "~J1(2.5)\tI_just_don't_get_it. ~E ",
    /* 25 */ "~J1(4.0)\tIt's_like_this_is ~N\n\t\t\tall_some_kind_of_bad_dream. ",
    /* 26 */ "~J1(2.8)\tYeah. ~N\n\t\t\tA_living_nightmare. ~E ",
    /* 27 */ "~J1(3.6)\tLet_me_ask_you,_have_you_seen ~N\n\t\t\ta_little_girl_around_here? ",
    /* 28 */ "~J1(2.8)\tShort,_black_hair. ~N\n\t\t\tSeven_years_old. ",
    /* 29 */ "~J1(3.8)\tA_seven-year-old_girl? ~N\n\t\t\tWhat?_She's_your_daughter? ",
    /* 30 */ "~J1(1.2)\tYes. ",
    /* 31 */ "~J1(3.0)\tA_seven-year-old_girl... ",
    /* 32 */ "~J1(4.8)\tI_can't_say_that_I_have. ~N\n\t\t\tI_was_unconscious_all_this_time. ",
    /* 33 */ "~J1(1.3)\tI'm_sorry. ~E ",
    /* 34 */ "~J1(2.2)\tThat's_alright. ",
    /* 35 */ "~J1(3.8)\tDo_you_know_anything_about_all ~N\n\t\t\tthat_weird_stuff_in_the_basement? ",
    /* 36 */ "~J1(3.8)\tNo.__Why? ~N\n\t\t\tIs_there_something_down_there? ",
    /* 37 */ "~J1(2.7)\tYou_don't_know? ~N\n\t\t\tDon't_you_work_here? ",
    /* 38 */ "~J1(4.0)\tWe're_under_strict_orders_never_to ~N\n\t\t\tenter_the_basement_storeroom. ",
    /* 39 */ "~J1(2.0)\tSo_I_really_don't_know. ~E ",
    /* 40 */ "~J1(2.0)\tWhat_did_you_say_was_down_there? ",
    /* 41 */ "~J1(1.7)\tWell,_it's... ~E ",
    /* 42 */ "~J0(0.8)\t~E ",
    /* 43 */ "~J0(2.5)\tDamn! ~N\n\t\t\tMy_head...  ",
    /* 44 */ "~J0(2.5)\tWhat's_wrong? ~N\n\t\t\tHarry...? ~E ",
    /* 45 */ "~J0(2.6)\tHarry, ~N\n\t\t\tlet_me_help_you. ~E ",
    /* 46 */ "~J0(1.0)\t~E ",
    /* 47 */ "\tThere_is_ a_ ~C2 Plate_of_[Queen] ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 48 */ "\tUsed_the_ ~C2 Examination_room_key ~C7 . ~E ",
    /* 49 */ "\tThere_is_a_broken ~N\n\tvending_machine. ~E ",
    /* 50 */ "\tNothing_unusual. ~E ",
    /* 51 */ "\tThe_refrigerator_is_empty. ~E ",
    /* 52 */ "\tNothing_helpful. ~E ",
    /* 53 */ "\tNothing_useful ~N\n\tin_the_desk_drawer. ~E ",
    /* 54 */ "\tA_sound_is_heard ~N\n\tfrom_beyond_the_door. ~E "
#elif VERSION_REGION_IS(NTSCJ)
    /* 15 */ "~J1(3.6)\t良かった…… ~N\n\t\t\tまだ無事な人がいたのね…… ",
    /* 16 */ "~J1(1.0)\tきみは？ ",
    /* 17 */ "~J1(3.1)\tあたしはリサ・ガーランド ~N\n\t\t\tあなたは？ ",
    /* 18 */ "~J1(1.2)\tハリー・メイソンだ~E ",
    /* 19 */ "~J0(4.7)\tねえ　いったい何が起こったの？ ~N\n\t\t\tみんなどうなってしまったの？ ",
    /* 20 */ "~J0(2.7)\tあたしが気を失っている間に…… ",
    /* 21 */ "~J0(3.2)\t誰もいなくなってしまって…… ",
    /* 22 */ "~J0(1.3)\t本当に怖かったわ…… ~E ",
    /* 23 */ "~J1(3.8)\tきみも知らないのか…… ",
    /* 24 */ "~J1(3.0)\tわたしもどうしてこんなことに ~N\n\t\t\tなったのか　わからないんだ ~E ",
    /* 25 */ "~J1(4.0)\tどうなっているんだ…… ~N\n\t\t\tすべてが悪い夢のようにも思えてくる…… ",
    /* 26 */ "~J1(2.8)\t……あたしもよ ~N\n\t\t\tすべてが悪夢のようだわ…… ~E ",
    /* 27 */ "~J1(3.6)\tところで……女の子を見なかったか？ ",
    /* 28 */ "~J1(2.8)\t短い黒髪の７歳の女の子だ ",
    /* 29 */ "~J1(3.8)\t７歳の女の子……？ ~N\n\t\t\tあなたの娘さん？ ",
    /* 30 */ "~J1(1.2)\tああ ",
    /* 31 */ "~J1(3.0)\t７歳の女の子…… ",
    /* 32 */ "~J1(4.8)\tわからないわ ~N\n\t\t\tずっと気を失っていたから…… ",
    /* 33 */ "~J1(1.3)\tごめんなさい ~E ",
    /* 34 */ "~J1(2.2)\tいや　いいんだ ",
    /* 35 */ "~J1(3.8)\tそれなら　この地下にある ~N\n\t\t\tあの妙な一帯は何なのか知ってるか？ ",
    /* 36 */ "~J1(3.8)\t何のこと？ ~N\n\t\t\tここの地下に何？ ",
    /* 37 */ "~J1(2.7)\t知らないのか？ ~N\n\t\t\tこの病院に勤めていたのに ",
    /* 38 */ "~J1(4.0)\t地下の倉庫には絶対に ~N\n\t\t\t入らないように言われているの ",
    /* 39 */ "~J1(2.0)\tだから……わからないわ ~E ",
    /* 40 */ "~J1(2.0)\tあそこに何があるというの……？ ",
    /* 41 */ "~J1(1.7)\tあそこは…… ~E ",
    /* 42 */ "~J0(0.8)\t~E ",
    /* 43 */ "~J0(2.5)\t……くそ！ ~N\n\t\t\t頭が…… ",
    /* 44 */ "~J0(2.5)\tどうしたの？ ~N\n\t\t\tハリー……？ ~E ",
    /* 45 */ "~J0(2.6)\tハリー ~N\n\t\t\tしっかりして…… ~E ",
    /* 46 */ "~J0(1.0)\t~E ",
    /* 47 */ "\t~C2 ［女王］のプレート ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 48 */ "\t~C2 診察室の鍵 ~C7 を使った ~E ",
    /* 49 */ "\t壊れた自動販売機だ ~E ",
    /* 50 */ "\t特に気になるものはない ~E ",
    /* 51 */ "\t冷蔵庫の中は空っぽだ ~E ",
    /* 52 */ "\t役に立ちそうなものはない ~E ",
    /* 53 */ "\t机の中には何もない ~E ",
    /* 54 */ "\t扉の向こうから物音が聞こえる ~E "
#endif
};

const VECTOR3 D_800CB1E0 = { Q12(14.5f), Q12(-1.0f), Q12(139.9f) };

void func_800D21C4(void) // 0x800D21C4
{
    s32     i;
    VECTOR3 sfxPos = { MAP_POINTS[g_MapEventData->mapPointIdx].positionX, Q12(-1.2f), MAP_POINTS[g_MapEventData->mapPointIdx].positionZ };

    if (!Savegame_EventFlagGet(EventFlag_232) && g_MapEventData->mapPointIdx == 5)
    {
        for (i = 0; i < ARRAY_SIZE(g_SysWork.npcs); i++)
        {
            if (g_SysWork.npcs[i].model.charaId >= Chara_Harry         &&
                g_SysWork.npcs[i].model.charaId <= CHARA_LAST_ENEMY_ID &&
                g_SysWork.npcs[i].health > Q12(0.0f))
            {
                break;
            }
        }

        if (i != ARRAY_SIZE(g_SysWork.npcs))
        {
            g_DeltaTime = Q12(0.0f);
        }

        switch (g_SysWork.sysStateSteps[0])
        {
            case 0:
                Player_ControlFreeze();
                Sfx_WithFlagsPlay(Sfx_DoorLocked, &sfxPos, Q8(0.5f), SfxFlag_None);
                SysWork_StateStepIncrement(0);

            case 1:
                Event_WaitTimer(Q12(0.2f), false);
                break;

            case 2:
                Event_DisplayMapMsg(false, 12, false, false, 0, false);
                break;

            case 3:
                SysWork_StateStepIncrement(0);

            case 4:
            case 6:
                Sfx_WithFlagsPlay(Sfx_Unk1529, &D_800CB1E0, Q8(0.5f), SfxFlag_None);
                SysWork_StateStepIncrement(0);

            case 5:
                Event_WaitTimer(Q12(0.2f), false);
                break;

            case 7:
                Event_WaitTimer(Q12(0.8f), false);
                break;

            case 8:
                Event_DisplayMapMsg(false, 54, false, false, 0, false);
                break;

            case 9:
                SysWork_StateStepIncrement(0);

            default:
                Player_ControlUnfreeze(false);
                SysWork_StateSetNext(SysState_Gameplay);
                Savegame_EventFlagSet(EventFlag_232);
                break;
        }
    }
    else
    {
        Event_DisplayMapMsgWithSfx(0xC, Sfx_DoorLocked, &sfxPos);
    }
}

void func_800D2470(void) // 0x800D2470
{
    u32 pickupType;
    s32 eventFlagIdx;

    pickupType   = CommonPickupItemId_FirstAidKit;
    eventFlagIdx = 0;

    switch (g_MapEventData->mapPointIdx)
    {
        case 52:
            pickupType   = CommonPickupItemId_HealthDrink;
            eventFlagIdx = EventFlag_M3S04_HealthDrink0;
            break;

        case 53:
            pickupType   = CommonPickupItemId_HealthDrink;
            eventFlagIdx = EventFlag_M3S04_HealthDrink1;
            break;

        case 54:
            pickupType   = CommonPickupItemId_HealthDrink;
            eventFlagIdx = EventFlag_M3S04_HealthDrink2;
            break;

        case 55:
            pickupType   = CommonPickupItemId_HealthDrink;
            eventFlagIdx = EventFlag_M3S04_HealthDrink3;
            break;

        case 56:
            pickupType   = CommonPickupItemId_HandgunBullets;
            eventFlagIdx = EventFlag_M3S04_HandgunBullets;
            break;
    }

    Event_CommonItemTake(pickupType, eventFlagIdx);
}

void func_800D250C(void) // 0x800D250C
{
    VECTOR3 sfxPos = { MAP_POINTS[g_MapEventData->mapPointIdx].positionX, Q12(-1.2f), MAP_POINTS[g_MapEventData->mapPointIdx].positionZ };

    Player_ItemRemove(InvItemId_ExaminationRoomKey, 1);
    Event_DisplayMapMsgWithSfx(48, Sfx_UseKey, &sfxPos);

    Savegame_EventFlagSet(EventFlag_258);
    Savegame_EventFlagSet(EventFlag_MapMark_AltHospital1F_WomensBathroomBroken);
}

#include "maps/common/shared/Event_CutsceneTimerAdvance.h" // 0x800D25D0

void func_800D2668(void) // 0x800D2668
{
    typedef enum _EventState
    {
        EventState_Skip = 25
    } e_EventState;

    s32 i;

    // Skip.
    if (Pad_ButtonClickedBind(skip) &&
        g_SysWork.sysStateSteps[0] >= 2 && g_SysWork.sysStateSteps[0] < 23)
    {
        SysWork_StateStepSet(0, EventState_Skip);
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();

            D_800D5A48[0] = Q12(2.2f);
            D_800D5A48[1] = Q12(2.1f);
            D_800D5A48[2] = Q12(2.6f);
            D_800D5A3C = 0;

            Fs_QueueStartRead(FILE_ANIM_HSPTL3_DMS, FS_BUFFER_11);
            Chara_Load(0, Chara_Lisa, &g_SysWork.npcBoneCoordBuffer[0], -1, NULL, NULL);
            Chara_ProcessLoads();
            Chara_Spawn(Chara_Lisa, 0, Q12(4.4f), Q12(269.9f), 0, 3);

            Chara_MovementReset(&g_SysWork.npcs[0]);
            Dms_HeaderFixOffsets(FS_BUFFER_11);

            CutsceneBorder_ForceShow();
            Screen_FadeTimestepReset();
            g_SysWork.sysFlags |= SysFlag_CutsceneActive;

#if VERSION_EQUAL_OR_NEWER(USA) // @bugfix?
            Game_TurnFlashlightOn();
#endif

            g_SysWork.lightBoneCoord     = NULL;
            g_SysWork.lensFlareBoneCoord = NULL;
            g_SysWork.lightIntensity     = Q12(0.7f);
            Math_Vector3Set(&g_SysWork.lightPosition, Q12(58.34f), Q12(-1.93f), Q12(141.8f));

            Math_SetSVectorFastSum(&g_SysWork.lightRotation, Q12_ANGLE(-15.0f), Q12_ANGLE(-177.0f), Q12_ANGLE(0.0f));

            func_8008D438();
            func_8003D03C();
            Player_EmptyWeaponHandSet();

            g_Cutscene_Timer = 0;
            D_800D5A4E = 0;

            SysWork_StateStepIncrement(0);
            break;

        case 1:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 5, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 51, false);
            SysWork_StateStepIncrement(0);

        case 2:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, 0, false);
            break;

        case 3:
            Event_DisplayMapMsgWithAudio(15, &D_800D5A3C, &D_800D599C);
            break;

        case 4:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, g_SysWork.npcs, 6, false);
            SysWork_StateStepIncrement(0);

        case 5:
            Event_DisplayMapMsgWithAudio(19, &D_800D5A3C, &D_800D599C);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(1.0f), Q12(21.0f), true, false);
            break;

        case 6:
            g_Cutscene_Timer = Q12(22.0f);

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 7, false);
            SysWork_StateStepIncrement(0);
            break;

        case 7:
            Event_WaitTimer(Q12(0.5f), false);
            break;

        case 8:
            SysWork_StateStepIncrement(0);

        case 9:
            Event_DisplayMapMsgWithAudio(23, &D_800D5A3C, &D_800D599C);
            break;

        case 10:
            Event_WaitTimer(Q12(0.7f), false);
            break;

        case 11:
            Event_DisplayMapMsgWithAudio(25, &D_800D5A3C, &D_800D599C);
            break;

        case 12:
            Event_WaitTimer(Q12(0.8f), false);
            break;

        case 13:
            Event_DisplayMapMsgWithAudio(27, &D_800D5A3C, &D_800D599C);
            break;

        case 14:
            Event_DisplayMapMsgWithAudio(34, &D_800D5A3C, &D_800D599C);
            Savegame_EventFlagSet(EventFlag_298);
            break;

        case 15:
            Event_DisplayMapMsgWithAudio(40, &D_800D5A3C, &D_800D599C);
            break;

        case 16:
            SysWork_StateStepIncrement(0);

        case 17:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 0x7D, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 8, false);
            SD_Call(Sfx_Unk1522);

            D_800D5A4E++;

            SysWork_StateStepIncrement(0);

        case 18:
            Event_DisplayMapMsgWithAudio(42, &D_800D5A3C, &D_800D599C);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(9.0f), Q12(23.0f), Q12(139.0f), true, false);
            break;

        case 19:
            Event_WaitTimer(Q12(0.5f), false);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(9.0f), Q12(23.0f), Q12(139.0f), true, false);
            break;

        case 20:
            Event_DisplayMapMsgWithAudio(43, &D_800D5A3C, &D_800D599C);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(9.0f), Q12(23.0f), Q12(139.0f), true, false);
            break;

        case 21:
            Event_DisplayMapMsgWithAudio(45, &D_800D5A3C, &D_800D599C);
            Event_WaitTimer(Q12(2.2f), false);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(9.0f), Q12(23.0f), Q12(139.0f), true, false);

            if (g_SysWork.sysStateSteps[0] != 21)
            {
                Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 0, Q12(1.0f), false);
            }
            break;

        case 22:
            Event_DisplayMapMsgWithAudio(45, &D_800D5A3C, &D_800D599C);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(9.0f), Q12(23.0f), Q12(139.0f), true, false);
            break;

        case 23:
            Event_ScreenFadeCmd(ScreenFadeCmd_Wait, true, 0, 0, false);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(9.0f), Q12(23.0f), Q12(139.0f), true, false);
            break;

        case 24:
            Event_DisplayMapMsgWithAudio(46, &D_800D5A3C, &D_800D599C);
            Savegame_EventFlagSet(EventFlag_299);
            break;

        case EventState_Skip:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, 0, false);
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            Player_WeaponAttackRestore();
            Chara_ModelCharaIdClear(g_SysWork.npcs, 0, 0);

            g_Cutscene_Timer = NO_VALUE;

            Savegame_EventFlagSet(EventFlag_294);
            Savegame_EventFlagSet(EventFlag_MapMark_AltHospital1F_WomensBathroomBroken);
            func_8008D448();
            break;
    }

    if (D_800D5A4E != Q12(0.0f))
    {
        D_800D5A4E += g_DeltaTime >> 1;
        if (D_800D5A4E > Q12(1.0f))
        {
            D_800D5A4E = Q12(1.0f);
        }

        Sd_SfxAttributesUpdate(Sfx_Unk1522, 0, -(D_800D5A4E >> 4), 0);

        i = 0;
        while (i < 3)
        {
            if (D_800D5A48[i] != 0)
            {
                D_800D5A48[i] -= g_DeltaTime;

                if ((D_800D5A48[i] << 16) <= 0)
                {
                    D_800D5A48[i] = 0;
                    SD_Call(Sfx_Unk1522);
                    D_800D5A4E = 1;
                }
                break;
            }

            i++;
        }
    }

    if (g_Cutscene_Timer >= Q12(0.0f))
    {
        Dms_CharacterTransformGet(&g_SysWork.playerWork.player.position, &g_SysWork.playerWork.player.rotation, "HERO", g_Cutscene_Timer, FS_BUFFER_11);
        Dms_CharacterTransformGet(&g_SysWork.npcs[0].position, &g_SysWork.npcs[0].rotation, "LISA", g_Cutscene_Timer, FS_BUFFER_11);
        vcChangeProjectionValue(Dms_CameraTargetsGet(&g_Cutscene_CameraPositionTarget, &g_Cutscene_CameraLookAtTarget, NULL, g_Cutscene_Timer, FS_BUFFER_11));
        vcUserCamTarget(&g_Cutscene_CameraPositionTarget, NULL, true);
        vcUserWatchTarget(&g_Cutscene_CameraLookAtTarget, NULL, true);
    }
}

void func_800D2E58(void) // 0x800D2E58
{
    Event_ItemTake(InvItemId_PlateOfQueen, DEFAULT_PICKUP_ITEM_COUNT, EventFlag_M3S04_PickupPlateOfQueen, 47);
}

const VECTOR3 sharedData_800CB088_3_s01 = { Q12(19.1f), Q12(-2.0f), Q12(59.1f) };

const VECTOR3 sharedData_800CB094_3_s01 = { Q12(17.2f), Q12(-1.0f), Q12(59.1f) };

#include "maps/common/shared/sharedFunc_800D15F0_3_s01.h" // 0x800D2E84

void Map_WorldObjectsInit(void) // 0x800D3BD0
{
    WorldObject_PlacementInit(&g_WorldObject_Plate, "PLATE_NE", 100.0f, 0.0f, 60.0f);
    WorldObject_Init(&g_WorldObject_SavePad, g_CommonWorldObjectNames[1], 102.5972f, -0.8191f, 58.7896f, 0.0f, 86.0f, 0.0f);
    WorldObject_PlacementInit(&g_WorldObject_Dr[0], "DR1L_HID", 19.1f, 0.0f, 59.1f);
    WorldObject_PlacementInit(&g_WorldObject_Dr[1], "DR1R_HID", 19.1f, 0.0f, 59.1f);
    WorldObject_PlacementInit(&g_WorldObject_Dr[2], "DR3L_HID", 19.1f, 0.0f, 59.1f);
    WorldObject_PlacementInit(&g_WorldObject_Dr[3], "DR3R_HID", 19.1f, 0.0f, 59.1f);
    WorldObject_PlacementInit(&g_WorldObject_Dr[4], "DR2L_HID", 19.1f, 0.0f, 59.1f);
    WorldObject_PlacementInit(&g_WorldObject_Dr[5], "DR2R_HID", 19.1f, 0.0f, 59.1f);

    WorldObject_ModelNameSet(&g_CommonWorldObjects[0], g_CommonWorldObjectNames[2]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[1], g_CommonWorldObjectNames[3]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[2], g_CommonWorldObjectNames[4]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[3], g_CommonWorldObjectNames[5]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[4], g_CommonWorldObjectNames[6]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[5], g_CommonWorldObjectNames[7]);
}

// Rodata at the end of this file. TODO: Inline once all users are decompiled.
extern SVECTOR3 D_800CB35C;
extern VECTOR3 D_800CB364;
extern VECTOR3 D_800CB370;

void Map_WorldObjectsUpdate(void) // 0x800D3DA4
{
    s32    i;
    q19_12 drZ;

    switch (PLAYER_MAP_CHUNK_CELL())
    {
        case MAP_CHUNK_CELL(2, 1):
            if (!Savegame_EventFlagGet(EventFlag_M3S04_PickupPlateOfQueen))
            {
                WorldObjects_Add(&g_WorldObject_Plate.object, &g_WorldObject_Plate.position, &D_800CB35C);
            }

            WorldObjects_Add(&g_WorldObject_SavePad.object, &g_WorldObject_SavePad.position, &g_WorldObject_SavePad.rotation);
            break;

        case MAP_CHUNK_CELL(0, 1):
            if (g_SysWork.sysState == SysState_Gameplay && g_WorldObject_Dr->position.vz != Q12(59.1f))
            {
                drZ = Q12(59.1f);

                for (i = ARRAY_SIZE(g_WorldObject_Dr) - 1; i >= 0; i--)
                {
                    g_WorldObject_Dr[i].position.vz = drZ;
                }
            }

            for (i = 0; i < ARRAY_SIZE(g_WorldObject_Dr); i++)
            {
                WorldObjects_Add(&g_WorldObject_Dr[i].object, &g_WorldObject_Dr[i].position, &D_800CB35C);
            }
            break;
    }

    if (PLAYER_IN_MAP_CHUNK(2, 1))
    {
        if (!Savegame_EventFlagGet(EventFlag_300))
        {
            if (g_SysWork.playerWork.player.position.vx > Q12(101.5f) &&
                g_SysWork.playerWork.player.position.vz > Q12(59.0f))
            {
                Sfx_WithFlagsPlay(Sfx_Unk1530, &D_800CB364, Q8_CLAMPED(1.0f), SfxFlag_NoDistAtten);
                Savegame_EventFlagSet(EventFlag_300);

                if (g_SavegamePtr->mapEnemyStates[g_SavegamePtr->mapIdx] & (1 << 2))
                {
                    g_SavegamePtr->mapEnemyStates[g_SavegamePtr->mapIdx] &= ~(1 << 3);
                }
            }
        }
    }

    if (PLAYER_IN_MAP_CHUNK(3, 2))
    {
        if (!Savegame_EventFlagGet(EventFlag_316))
        {
            if (Savegame_EventFlagGet(EventFlag_301) && g_SysWork.playerWork.player.position.vz > Q12(104.0f))
            {
                if (!Vw_AabbVisibleInScreenCheck(Q12(139.0f), Q12(142.0f), Q12(-3.0f), Q12(0.0f), Q12(98.8f), Q12(99.0f)))
                {
                    Sfx_WithFlagsPlay(Sfx_Unk1528, &D_800CB370, 0xFF, SfxFlag_NoDistAtten);
                    Savegame_EventFlagSet(EventFlag_316);
                }
            }

            if (g_SysWork.playerWork.player.position.vz < Q12(100.0f))
            {
                Savegame_EventFlagSet(EventFlag_301);
            }
        }
    }

    if (Pad_ButtonClicked(ControllerFlag_R3))
    {
        SD_Call(Sfx_Unk1529);
    }

    if (PLAYER_IN_MAP_CHUNK(3, 1))
    {
        if (!Savegame_EventFlagGet(EventFlag_M3S04_HealthDrink0))
        {
            WorldObjects_Add(&g_CommonWorldObjects[1], &g_CommonWorldObjectPoses[0].position, &g_CommonWorldObjectPoses[0].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(2, 2))
    {
        if (!Savegame_EventFlagGet(EventFlag_M3S04_HandgunBullets))
        {
            WorldObjects_Add(&g_CommonWorldObjects[3], &g_CommonWorldObjectPoses[1].position, &g_CommonWorldObjectPoses[1].rotation);
        }
    }
}

void func_800D43B8(void) // 0x800D43B8
{
    if (Savegame_EventFlagGet(EventFlag_300))
    {
        Chara_SpawnFlagsSet(Chara_PuppetDoctor, 2, CharaSpawnFlag_0);
        Chara_SpawnFlagsSet(Chara_PuppetDoctor, 3, CharaSpawnFlag_0);
    }
}

const SVECTOR3 D_800CB35C = { 0, 0, 0 };

const VECTOR3 D_800CB364 = { Q12(96.0f), Q12(-1.2f), Q12(59.8f) };

const VECTOR3 D_800CB370 = { Q12(140.5f), Q12(-1.8f), Q12(98.0f) };
