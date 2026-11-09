#include "bodyprog/bodyprog.h"
#include "bodyprog/events/event_data.h"
#include "bodyprog/gfx/map_effects.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player/player.h"
#include "main/rng.h"
#include "maps/map3/map3_s01.h"
#include "maps/common/particle.h"
#include "maps/common/characters/creeper.h"
#include "maps/common/characters/player.h"
#include "inline_no_dmpsx.h"
#include <psyq/gtemac.h>

#include "../src/maps/common/particles/particle.c"

#include "../src/maps/common/characters/player.c"

#include "../src/maps/common/characters/creeper.c" // 0x800CEE4C

#include "../src/maps/common/map_util.c" // 0x800D10C0

#include "maps/common/shared/Map_RoomBgmInit_CheckCond.h" // 0x800D1178

#include "maps/common/shared/Map_RoomBgmInit_3_s00_CondTrue.h" // 0x800D11AC

#include "maps/common/shared/Map_RoomBgmInit_3_s00_CondFalse.h" // 0x800D11EC

void GameBoot_LoadScreen_StageString(void) {}

#include "maps/common/shared/MapEvent_DoorJammed.h" // 0x800D1394

#include "maps/common/shared/MapEvent_DoorLocked.h" // 0x800D1428

const char* MAP_MESSAGES[] = {
    #include "maps/common/shared/map_msg_common.h"
#if VERSION_REGION_IS(NTSC)
    /* 15 */ "\tThe_elevator_doesn't ~N\n\tappear_to_be_working. ~E ",
    /* 16 */ "\tA_glass_vial_lies_shattered. ",
    /* 17 */ "\tIt's_not_just_broken. ~N\n\tIt_looks_smashed_on_purpose. ~E ",
    /* 18 */ "\tThe_remaining_liquid_is_emptied ~N\n\tinto_the_ ~C2 Plastic_bottle ~C7 . ",
    /* 19 */ "\tNow_what_have_we_here? ~E ",
    /* 20 */ "\tA_glass_vial_lies_shattered. ~E ",
    /* 21 */ "\tThere_is_a_ ~C2 Plastic_bottle ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 22 */ "~H\tThere_is_a_ ~C5 Hospital_map ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 23 */ "~H\tThere_is_a ~N\n\t~C5 Hospital_basement_map ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 24 */ "\tThere_is_a_ ~C2 Basement_key ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 25 */ "~C3\tThe_in-house_generator ~N\n\t\tpowers_elevators,_ICUs ~N\n\t\tand_operating_rooms_only. ",
    /* 26 */ "\tDo_you_want_to_press_the_switch? ~S4 ",
    /* 27 */ "\tThe_generator_is_on. ~E ",
    /* 28 */ "\tUsed_the_ ~C2 Basement_key ~C7 _. ~E ",
    /* 29 */ "\tI_should_search_the_hospital. ~E ",
    /* 30 */ "\tThese_are_phones_and_phonebooks. ~N\n\tNo_need_to_examine_this. ~E ",
    /* 31 */ "\tThere's_a_vending_machine. ~N\n\tIt_doesn't_work. ~E ",
    /* 32 */ "\tThe_shelves_are_all_messed_up. ~N\n\tWho_did_this? ~E ",
    /* 33 */ "\tBooks_are_scattered ~N\n\taround_the_desk. ~N\n\tNothing_else_special_here. ~E ",
    /* 34 */ "\tThe_phone's_out. ~N\n\tCan't_get_a_dial_tone. ~E ",
    /* 35 */ "\tThere's_a_white_board. ~N\n\tNothing_special_written_here. ~E ",
    /* 36 */ "\tThere_is_a_newspaper. ",
    /* 37 */ "\tThis_article's_been ~N\n\tclipped_out... ~E ",
    /* 38 */ "\tThe_refrigerator_is_empty. ~E ",
    /* 39 */ "\tNothing_unusual. ~E ",
    /* 40 */ "\tNothing_helpful. ~E "
#elif VERSION_REGION_IS(NTSCJ)
    /* 15 */ "\tエレベーターは動いていないようだ ~E ",
    /* 16 */ "\tガラスのビンが床で割れている ",
    /* 17 */ "\t落として割ったというより ~N\n\t意図的に壊したという感じだ ~E ",
    /* 18 */ "\tビンの底に少しだけ残っていた液体を ~N\n\t~C2 ペットボトル ~C7 へあけた  ",
    /* 19 */ "\tこれはいったい何なのだろう……？ ~E ",
    /* 20 */ "\tガラスのビンが床で割れている ~E ",
    /* 21 */ "\t~C2 ペットボトル ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 22 */ "~H ~C5 病院の地図 ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 23 */ "~H ~C5 病院地下の地図 ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 24 */ "\t~C2 病院地下の鍵 ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 25 */ "~C3\t非常用自家発電機 ~N\n\t\tエレベーター、ＩＣＵ、手術室のみ通電 ",
    /* 26 */ "\tスイッチがある ~N\n\t押しますか？ ~S4 ",
    /* 27 */ "\t発電機は動いている ~E ",
    /* 28 */ "\t~C2 病院地下の鍵 ~C7 を使った ~E ",
    /* 29 */ "\t病院の中を調べよう ~E ",
    /* 30 */ "\t電話機とこの地域の電話帳がある ~N\n\t特に調べる必要はないだろう ~E ",
    /* 31 */ "\t自動販売機がある ~N\n\t故障か停電かわからないが動いていない ~E ",
    /* 32 */ "\t棚の中は荒らされている ~N\n\t誰がやったのだろうか？ ~E ",
    /* 33 */ "\t机の周辺には本が散らかっている ~N\n\t他に特に気になるものはない ~E ",
    /* 34 */ "\t電話機は使えないようだ ~N\n\t何をしても反応がない ~E ",
    /* 35 */ "\tホワイトボードがある ~N\n\t特に気になるようなことは書かれていない ~E ",
    /* 36 */ "\t新聞が置いてある ",
    /* 37 */ "\t切り抜いた跡があるな…… ~E ",
    /* 38 */ "\t冷蔵庫がある ~N\n\t中に気になるものはない ~E ",
    /* 39 */ "\t特に気になることはない ~E ",
    /* 40 */ "\t役に立ちそうなものはない ~E "
#endif
};

void func_800D14BC(void) // 0x800D14BC
{
    u32 pickupType;
    s32 eventFlagIdx;

    pickupType   = CommonPickupItemId_FirstAidKit;
    eventFlagIdx = 0;

    switch (g_MapEventData->mapPointIdx)
    {
        case 101:
            pickupType   = CommonPickupItemId_HealthDrink;
            eventFlagIdx = EventFlag_M3S01_HealthDrink;
            break;
    }

    Event_CommonItemTake(pickupType, eventFlagIdx);
}

void func_800D1500(void) // 0x800D1500
{
    Event_CommonItemTake(CommonPickupItemId_FirstAidKit, EventFlag_M3S00_FirstAidKit);
}

void func_800D1524(void) // 0x800D1524
{
    VECTOR3 sfxPos = { MAP_POINTS[g_MapEventData->mapPointIdx].positionX, Q12(-1.2f), MAP_POINTS[g_MapEventData->mapPointIdx].positionZ };

    Player_ItemRemove(InvItemId_BasementKey, 1);
    Event_DisplayMapMsgWithSfx(28, Sfx_UseKey, &sfxPos); // "Used basement key."

    Savegame_EventFlagSet(EventFlag_MapMark_585);
    Savegame_EventFlagSet(EventFlag_M3S01_BasementDoorOpen);
}

const VECTOR3 sharedData_800CB088_3_s01 = { Q12(59.1f), Q12(-2.0f), Q12(19.1f) };

const VECTOR3 sharedData_800CB094_3_s01 = { Q12(57.2f), Q12(-1.0f), Q12(19.1f) };

const VECTOR3 sharedData_800CB0A0_3_s01 = { Q12(57.2f), Q12(-2.0f), Q12(19.1f) };

#include "maps/common/shared/sharedFunc_800D15F0_3_s01.h" // 0x800D15F0

void MapEvent_UnknownLiquidInteract(void) // 0x800D23AC
{
    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_BgTextureCmd(BgTextureCmd_QueueRead, FILE_TIM_SHATVIAL_TIM, false);
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            Event_CharaAnimPlayToEnd(&g_SysWork.playerWork.player, 59);
            break;

        case 3:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 0, Q12(0.0f), false);
            SysWork_StateStepIncrement(0);

        case 4:
            Event_BgTextureCmd(BgTextureCmd_AwaitLoad, 0, false);
            break;

        case 5:
            Event_ScreenFadeCmd(ScreenFadeCmd_Wait, true, 0, Q12(0.0f), false);
            break;

        case 6:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.0f), false);
            break;

        case 7:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_DisplayMapMsg(false, 16, 0, 0, 0, false); // "A glass vial lies shattered."
            break;

        case 8:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 9:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.0f), false);
            break;

        case 10:
            Event_CharaAnimPlayToEnd(&g_SysWork.playerWork.player, 60);
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            Savegame_EventFlagSet(EventFlag_204);
            break;
    }
}

void MapEvent_UseBottleOnLiquid(void) // 0x800D25A8
{
    typedef enum _EventState
    {
        EventState_0     = 0,
        EventState_1     = 1,
        EventState_2     = 2,
        EventState_SfxId = 3,
        EventState_Msg   = 4,
        EventState_5     = 5
    } e_EventState;

    switch (g_SysWork.sysStateSteps[0])
    {
        case EventState_0:
            Player_ControlFreeze();
            SysWork_StateStepIncrement(0);

        case EventState_1:
            Event_WaitPlayerStop();
            break;

        case EventState_2:
            Event_CharaAnimPlayToEnd(&g_SysWork.playerWork.player, 59);
            break;

        case EventState_SfxId:
            Sfx_WithFlagsPlay(Sfx_Unk1493, &Q12_VECTOR3(141.6f, -0.0999f, 20.75f), Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case EventState_Msg:
            Event_DisplayMapMsg(false, 18, 0, 0, 0, false); // "The remaining liquid is emptied into the Plastic bottle."
            break;

        case EventState_5:
            Event_CharaAnimPlayToEnd(&g_SysWork.playerWork.player, 60);
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            Savegame_EventFlagSet(EventFlag_M3S01_PickupUnknownLiquid);

            // Replace Plastic Bottle with Unknown Liquid in inventory.
            Player_ItemRemove(InvItemId_PlasticBottle, 1);
            Event_InvItemCmd(InvItemCmd_AddItem, InvItemId_UnknownLiquid, DEFAULT_PICKUP_ITEM_COUNT, false);
            break;
    }
}

void MapEvent_PlasticBottleTake(void) // 0x800D2720
{
    Event_ItemTake(InvItemId_PlasticBottle, DEFAULT_PICKUP_ITEM_COUNT, EventFlag_M3S01_PickupPlasticBottle, 21);
}

void MapEvent_PaperMapTake0(void) // 0x800D274C
{
    Event_PaperMapTake(17, EventFlag_M3S00_PickupMap, 22);
}

void MapEvent_PaperMapTake1(void) // 0x800D2774
{
    Event_PaperMapTake(16, EventFlag_M3S01_PickupMap, 23);
}

void func_800D279C(void) // 0x800D279C
{
    Event_ItemTake(InvItemId_BasementKey, DEFAULT_PICKUP_ITEM_COUNT, EventFlag_M3S01_PickupBasementKey, 24);
}

void MapEvent_Generator0(void) // 0x800D27C8
{
    typedef enum _EventState
    {
        EventState_PressSwitch     = 3,
        EventState_DontPressSwitch = NO_VALUE
    } e_EventState;

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            g_SysWork.silentYesSelection = true;
            Event_DisplayMapMsg(true, 25, EventState_PressSwitch, EventState_DontPressSwitch, 0, false); // "Do you want to press the switch?"
            break;

        case EventState_PressSwitch:
            Sfx_WithFlagsPlay(Sfx_Unk1494, &Q12_VECTOR3(140.5f, -0.5f, -20.6f), Q8(0.5f), SfxFlag_None);
            SD_Call(Sfx_Unk1495);
            Savegame_EventFlagSet(EventFlag_M3S01_GeneratorOn);

            g_GeneratorMakeNoise = true;

            SysWork_StateStepIncrement(0);

        case 4:
            Event_WaitTimer(Q12(2.5f), false);
            break;

        case 5:
            Savegame_EventFlagSet(EventFlag_M3S01_GeneratorOn);
            g_GeneratorMakeNoise = true;
            SysWork_StateStepIncrement(0);

        default: // `EventState_DontPressSwitch`
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;
    }
    if (Savegame_EventFlagGet(EventFlag_M3S01_GeneratorOn))
    {
        func_800894B8(144);
    }
}

void func_800D29A4(void) // 0x800D29A4
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
            Event_DisplayMapMsg(false, 27, 0, 0, 0, false);
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;
    }

    func_800894B8(0x60); // Hex?
}

void func_800D2A88(void) // 0x800D2A88
{
    if (g_SysWork.sysStateSteps[1] == 0)
    {
        Sfx_WithFlagsPlay(Sfx_MenuMap, &Q12_VECTOR3(61.72f, -0.8f, 100.5098f), Q8(0.5f), SfxFlag_None);
    }

    Event_DisplayMapMsgWithBg(Sfx_Unk1916, Q12(0.0f), Q12(0.0f), 36);
    Savegame_EventFlagSet(EventFlag_203);
}

void Map_WorldObjectsInit(void) // 0x800D2AF4
{
    D_800D8734 = 0;
    g_GeneratorMakeNoise = false;

    WorldObject_PlacementInit(&g_WorldObject1, "BOT_HIDE", 20.3f, 0.0f, 59.25f);
    WorldObject_Init(&g_WorldObject2, "MAP_HIDE", 60.0f, 0.0f, 60.0f, 0.0f, 9.7f, 0.0f);
    WorldObject_Init(&g_WorldObject5, "KEY_HIDE", 100.3f, 0.0f, 58.9f, 0.0f, 72.8f, 0.0f);
    WorldObject_PlacementInit(&g_WorldObject3[0], "BT3_HIDE", 59.1f, 0.0f, 19.1f);
    WorldObject_PlacementInit(&g_WorldObject3[1], "BT4_HIDE", 59.1f, 0.0f, 19.1f);
    WorldObject_PlacementInit(&g_WorldObject_Dr[0], "DR1L_HID", 59.1f, 0.0f, 19.1f);
    WorldObject_PlacementInit(&g_WorldObject_Dr[1], "DR1R_HID", 59.1f, 0.0f, 19.1f);
    WorldObject_PlacementInit(&g_WorldObject_Dr[2], "DR3L_HID", 59.1f, 0.0f, 19.1f);
    WorldObject_PlacementInit(&g_WorldObject_Dr[3], "DR3R_HID", 59.1f, 0.0f, 19.1f);
    WorldObject_PlacementInit(&g_WorldObject_Dr[4], "DR2L_HID", 59.1f, 0.0f, 19.1f);
    WorldObject_PlacementInit(&g_WorldObject_Dr[5], "DR2R_HID", 59.1f, 0.0f, 19.1f);
    WorldObject_Init(&g_WorldObject6, g_CommonWorldObjectNames[1], 26.0f, -1.0198f, 144.5498f, 0.0f, 163.8f, 0.0f);
    WorldObject_Init(&g_WorldObject7, "MAP_HIDE", 24.6f, -1.56f, 143.16f, 0.0f, 180.0f, 0.0f);
    WorldObject_Init(&g_WorldObject8, "MAP2_HID", 24.6f, -1.56f, 143.16f, 0.0f, 180.0f, 0.0f);
    WorldObject_Init(&g_WorldObject9, g_CommonWorldObjectNames[2], 27.6f, -0.45f, 143.6f, 0.0f, -90.0f, 0.0f);

    WorldObject_ModelNameSet(&g_CommonWorldObjects[0], g_CommonWorldObjectNames[2]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[1], g_CommonWorldObjectNames[3]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[2], g_CommonWorldObjectNames[4]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[3], g_CommonWorldObjectNames[5]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[4], g_CommonWorldObjectNames[6]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[5], g_CommonWorldObjectNames[7]);
}

void Map_WorldObjectsUpdate(void) // 0x800D2E54
{
    s32    i;
    q19_12 posZ;

    if (PLAYER_IN_MAP_CHUNK(0, 1))
    {
        if (!Savegame_EventFlagGet(EventFlag_M3S01_PickupPlasticBottle))
        {
            WorldObjects_Add(&g_WorldObject1.object, &g_WorldObject1.position, &SVECTOR3_Zero);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(1, 1))
    {
        if (!Savegame_EventFlagGet(EventFlag_M3S01_PickupMap))
        {
            WorldObjects_Add(&g_WorldObject2.object, &g_WorldObject2.position, &g_WorldObject2.rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(2, 1))
    {
        if (!Savegame_EventFlagGet(EventFlag_M3S01_PickupBasementKey))
        {
            WorldObjects_Add(&g_WorldObject5.object, &g_WorldObject5.position, &g_WorldObject5.rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(1, -1))
    {
        if (D_800D8734 != 1)
        {
            WorldEnv_PresetsSet(7, 4);
            Game_TurnFlashlightOff();
            D_800D8734 = 1;
        }
    }

    if ((PLAYER_IN_MAP_CHUNK(0, -1)) ||
        (PLAYER_IN_MAP_CHUNK(2, 0)))
    {
        if (D_800D8734 != 2)
        {
            WorldEnv_PresetsSet(13, 13);
            Game_TurnFlashlightOff();
            D_800D8734 = 2;
        }
    }

    if (PLAYER_IN_MAP_CHUNK(0, 0))
    {
        if (D_800D8734 != 3)
        {
            WorldEnv_PresetsSet(2, 2);
            D_800D8734 = 3;
        }
    }

    if (PLAYER_IN_MAP_CHUNK(1, 0))
    {
        if (D_800D8734 != 4)
        {
            D_800D8734 = 4;

            WorldEnv_PresetsSet(4, 4);

            g_SysWork.lightBoneCoord     = NULL;
            g_SysWork.lightPosition.vx   = Q12(59.1f);
            g_SysWork.lightPosition.vy   = Q12(-3.0f);
            g_SysWork.lightPosition.vz   = Q12(19.1f);
            g_SysWork.lensFlareBoneCoord = NULL;
            g_SysWork.lightRotation.vx   = Q12_ANGLE(-90.0f);
            g_SysWork.lightRotation.vy   = Q12_ANGLE(0.0f);
            g_SysWork.lightRotation.vz   = Q12_ANGLE(0.0f);
            g_SysWork.lightIntensity     = Q12(0.7f);

            func_8008D438();

            posZ = Q12(19.1f);
            for (i = 5; i >= 0; i--)
            {
                g_WorldObject_Dr[i].position.vz = posZ;
            }
        }

        for (i = 0; i < ARRAY_SIZE(g_WorldObject_Dr); i++)
        {
            WorldObjects_Add(&g_WorldObject_Dr[i].object, &g_WorldObject_Dr[i].position, &SVECTOR3_Zero);
        }

        if (Savegame_EventFlagGet(EventFlag_MapMark_AltHospital2F_OperatingPrepRoomArrow) && Savegame_EventFlagGet(EventFlag_MapMark_AltHospital2F_CorridorMidArrows))
        {
            WorldObjects_Add(&g_WorldObject3[1].object, &g_WorldObject3[1].position, &SVECTOR3_Zero);
        }
        else
        {
            WorldObjects_Add(&g_WorldObject3[0].object, &g_WorldObject3[0].position, &SVECTOR3_Zero);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(-1, 0))
    {
        if (D_800D8734 != 5)
        {
            WorldEnv_PresetsSet(2, 2);
            D_800D8734 = 5;
        }
    }

    if (PLAYER_IN_MAP_CHUNK(-1, -1))
    {
        if (D_800D8734 != 6)
        {
            WorldEnv_PresetsSet(2, 2);
            D_800D8734 = 6;
        }
    }

    if (PLAYER_IN_MAP_CHUNK(3, -1))
    {
        if (Savegame_EventFlagGet(EventFlag_M3S01_GeneratorOn))
        {
            if (!g_GeneratorMakeNoise)
            {
                SD_Call(Sfx_Unk1503);
                g_GeneratorMakeNoise++;
            }
            else if (g_GeneratorMakeNoise == true)
            {
                Sfx_WithFalloffAndPitchPlay(Sfx_Unk1495, &D_800D4CE4, 0x80, Q12(16.0f), 0);
                Sfx_WithFalloffAndPitchPlay(Sfx_Unk1503, &D_800D4CE4, 0x80, Q12(16.0f), 0);
            }
        }
    }
    else
    {
        if (g_GeneratorMakeNoise == true)
        {
            Sd_SfxStop(Sfx_Unk1495);
            Sd_SfxStop(Sfx_Unk1503);
            g_GeneratorMakeNoise = false;
        }
    }

    if (PLAYER_IN_MAP_CHUNK(0, 3))
    {
        WorldObjects_Add(&g_WorldObject6.object, &g_WorldObject6.position, &g_WorldObject6.rotation);

        if (!Savegame_EventFlagGet(EventFlag_M3S00_PickupMap))
        {
            WorldObjects_Add(&g_WorldObject7.object, &g_WorldObject7.position, &g_WorldObject7.rotation);
        }
        else
        {
            WorldObjects_Add(&g_WorldObject8.object, &g_WorldObject8.position, &g_WorldObject8.rotation);
        }

        if (!Savegame_EventFlagGet(EventFlag_M3S00_FirstAidKit))
        {
            WorldObjects_Add(&g_WorldObject9.object, &g_WorldObject9.position, &g_WorldObject9.rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(0, 1))
    {
        if (!Savegame_EventFlagGet(EventFlag_M3S01_HealthDrink))
        {
            WorldObjects_Add(&g_WorldObjectA, &g_WorldObjectAPos.position, &g_WorldObjectAPos.rotation);
        }
    }
}
