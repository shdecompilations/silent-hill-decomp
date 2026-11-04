#include "bodyprog/bodyprog.h"
#include "bodyprog/events/event_data.h"
#include "bodyprog/map/terrain.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player/player.h"
#include "main/rng.h"
#include "maps/map3/map3_s03.h"
#include "maps/particle.h"
#include "maps/characters/bloodsucker.h"
#include "maps/characters/player.h"
#include "maps/characters/puppet_nurse.h"

#include "../src/maps/particle.c"

#include "../src/maps/characters/player.c"

#include "../src/maps/unk_m3s03.c" // 0x800CD6B0

#include "../src/maps/characters/puppet_nurse.c"

// TODO: Move this line into separate Chara_Bloodsucker split.
#include "../src/maps/characters/bloodsucker.c" // 0x800D09C4

#include "../src/maps/map_util.c" // 0x800D1168

#include "maps/shared/Map_RoomBgmInit_CheckCond.h" // 0x800D1210

#include "maps/shared/Map_RoomBgmInit_3_s02_CondTrue.h" // 0x800D1244

#include "maps/shared/Map_RoomBgmInit_3_s02_CondFalse.h" // 0x800D1284

void GameBoot_LoadScreen_StageString(void) {}

const char* MAP_MESSAGES[] = {
    #include "maps/shared/map_msg_common.h"
#if VERSION_REGION_IS(NTSC)
    /* 15 */ "\tThere_is_a_ ~C2 Plate_of_[Hatter] ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 16 */ "\tThere_is_a_ ~C2 Plate_of_[Cat] ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 17 */ "\tThere_is_a_ ~C2 Plate_of_[Turtle] ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 18 */ "~D ~C3 Clouds_flowing_over_a_hill. ",
    /* 19 */ "~D ~C3 Sky_on_a_sunny_day. ",
    /* 20 */ "~D ~C3 Tangerines_that_are_bitter. ",
    /* 21 */ "~D ~C3 Lucky_four-leaf-clover. ",
    /* 22 */ "~D ~C3 Violets_in_the_garden. ",
    /* 23 */ "~D ~C3 Dandelions_along_a_path. ",
    /* 24 */ "~D ~C3 Unavoidable_sleeping_time. ",
    /* 25 */ "~D ~C3 Liquid_flowing ~N\n\t\tfrom_a_slashed_wrist. ~E ",
    /* 26 */ "\tThe_VCR_seems_broken. ~N\n\tIt_doesn't_play. ~E ",
    /* 27 */ "~J0(0.7)\t_ ",
    /* 28 */ "~J2(1.5)\t======= ",
    /* 29 */ "~J2(2.7)\tStill_=====_unusua== ~N\n\t\t\t====fever... ",
    /* 30 */ "~J2(2.6)\t===_don't_===== ~N\n\t\t\tget====_puls== ",
    /* 31 */ "~J2(2.8)\t===_just_bare========= ",
    /* 32 */ "~J2(2.3)\t===_skin_is_======== ",
    /* 33 */ "~J2(1.2)\tEven_when_I_ch=== ~N\n\t\t\t======== ",
    /* 34 */ "~J2(3.8)\t===================== ~N\n\t\t\t==zing_thro=== ",
    /* 35 */ "~J2(5.9)\tWhy= ~N\n\t\t\t================_chil===== ",
    /* 36 */ "~J2(2.8)\t============ ~N\n\t\t\t========= ",
    /* 37 */ "~J2(3.3)\t==won't_tell_==== ~N\n\t\t\t======= ",
    /* 38 */ "~J2(2.0)\t==please... ",
    /* 39 */ "~J2(0.5)\t_ ~E ",
    /* 40 */ "\tThere_is_a_ ~C2 Blood_pack ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 41 */ "\tThere_is_a_ ~C2 Disinfecting_alcohol ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 42 */ "\tThere_is_a_ ~C2 Lighter ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 43 */ "\tThere_is_a ~N\n\t ~C2 Basement_storeroom_key ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 44 */ "\t_ ~E ",
    /* 45 */ "\tSlate._Nothing_more_than_that. ~E ",
    /* 46 */ "\tAlcohol_bottle. ~N\n\tLooks_empty. ~E ",
    /* 47 */ "\tWhere_do_you_want_to ~N\n\tset_the_plate? ~E ",
    /* 48 */ "\tCan't_set_the_plate_there. ~E ",
    /* 49 */ "\tWhich_plate_do_you ~N\n\twant_to_remove? ~E ",
    /* 50 */ "\tCan't_remove_that_plate. ~E ",
    /* 51 */ "\tThere_are_4_square_indentations ~N\n\ton_the_door. ~E ",
    /* 52 */ "\t",
    /* 53 */ "\tBird_cage._It's_empty. ~E ",
    /* 54 */ "\tVCR._It's_old_but_usable. ~E ",
    /* 55 */ "\tA_steel_plate ~N\n\tis_screwed_to_the_wall. ~E ",
    /* 56 */ "\tNothing_unusual. ~E ",
    /* 57 */ "\tHere_are_some_dryers. ~N\n\tUseless_now. ~E ",
    /* 58 */ "\tNothing_helpful. ~E ",
    /* 59 */ "\tNothing_useful_in_the_boxes. ~E ",
    /* 60 */ "\tThere's_a_bottle_filled ~N\n\twith_some_kind_of_drug. ~N\n\tBetter_leave_it. ~E ",
    /* 61 */ "\tThere_are_some_empty_bottles. ~E ",
    /* 62 */ "\tNothing_special_in_the_box. ~N\n\tNothing_to_worry_about. ~E "
#elif VERSION_REGION_IS(NTSCJ)
    /* 15 */ "\t~C2 ［帽子屋］のプレート ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 16 */ "\t~C2 ［猫］のプレート ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 17 */ "\t~C2 ［海亀］のプレート ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 18 */ "~D ~C3\t丘を越え流れゆく雲 ",
    /* 19 */ "~D ~C3\t晴れた日の空 ",
    /* 20 */ "~D ~C3\t季節外れのミカン ",
    /* 21 */ "~D ~C3\t幸運の四つ葉 ",
    /* 22 */ "~D ~C3\t庭先のスミレ ",
    /* 23 */ "~D ~C3\t道端のタンポポ ",
    /* 24 */ "~D ~C3\t訪れた眠りの時間 ",
    /* 25 */ "~D ~C3\t手首から流れゆくもの ~E ",
    /* 26 */ "\tビデオデッキは壊れてしまったようだ ~N\n\t動かなくなっている ~E ",
    /* 27 */ "~J0(0.7)\t　 ",
    /* 28 */ "~J2(1.5)\t……………て？ ",
    /* 29 */ "~J2(2.7)\t………高熱………………続い……わ…… ",
    /* 30 */ "~J2(5.4)\t…………さない……………………… ~N\n\t\t\t……かに息を…………だけ ",
    /* 31 */ "~J2(2.3)\t皮………………が焼け…………………… ",
    /* 32 */ "~J2(5.0)\t…………度替え…… ~N\n\t\t\tすぐに血と…………ロにな………… ",
    /* 33 */ "~J2(5.9)\tどうして…… ~N\n\t\t\t…………あの子は……てるの？ ",
    /* 34 */ "~J2(2.8)\t……………………………………… ",
    /* 35 */ "~J2(3.3)\t……………話さな…… ~N\n\t\t\t……………… ",
    /* 36 */ "~J2(2.0)\t…………………許して ",
    /* 37 */ "~J2(0.5)\t　 ~E ",
    /* 38 */ "\t~C2 輸血用血液パック ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 39 */ "\t~C2 消毒用アルコール ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 40 */ "\t~C2 ライター ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 41 */ "\t~C2 地下倉庫の鍵 ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 42 */ "\t　 ~E ",
    /* 43 */ "\t石板がある ~N\n\tそれだけで他には特に何もないようだ ~E ",
    /* 44 */ "\tアルコールのボトルが置いてある ~N\n\t中身は入っていないようだ ~E ",
    /* 45 */ "\tどこにプレートをはめますか？ ~E ",
    /* 46 */ "\tそこにプレートをはめることはできません ~E ",
    /* 47 */ "\tどこのプレートを外しますか？ ~E ",
    /* 48 */ "\tそのプレートを外すことはできません ~E ",
    /* 49 */ "\t扉には４つの正方形のくぼみがある ~E ",
    /* 50 */ "\t~C4 ここで血液パックを使ってください ~C7 ~E ",
    /* 51 */ "\t鳥カゴがある ~N\n\t中は空っぽで何もいない ~E ",
    /* 52 */ "\tビデオデッキがある ~N\n\t古びてはいるが　まだ使えそうだ ~E ",
    /* 53 */ "\t鉄板が壁にネジ止めされている ~E ",
    /* 54 */ "\t特に気になるものはない ~E ",
    /* 55 */ "\t乾燥機が並んでいる ~N\n\t今は何の役にも立たない ~E ",
    /* 56 */ "\t特に役に立ちそうなものはない ~E ",
    /* 57 */ "\tダンボール箱の中に ~N\n\t役に立ちそうなものは入っていない ~E ",
    /* 58 */ "\t中身のわからない薬のビンが置かれている ~N\n\t無視していった方がいいだろう ~E ",
    /* 59 */ "\t空のビンが置かれている ~E ",
    /* 60 */ "\t箱の中にはろくなものが入っていない ~N\n\t気にしないでいいだろう ~E "
#endif
};

void func_800D151C(void) // 0x800D151C
{
    u32 pickupType;
    s32 eventFlagIdx;

    pickupType   = CommonPickupItemId_FirstAidKit;
    eventFlagIdx = 0;

    switch (g_MapEventData->mapPointIdx)
    {
        case 126:
            pickupType   = CommonPickupItemId_FirstAidKit;
            eventFlagIdx = EventFlag_M3S03_FirstAidKit0;
            break;

        case 127:
            pickupType   = CommonPickupItemId_HealthDrink;
            eventFlagIdx = EventFlag_M3S03_HealthDrink;
            break;

        case 128:
            pickupType   = CommonPickupItemId_HandgunBullets;
            eventFlagIdx = EventFlag_M3S03_HandgunBullets;
            break;

        case 129:
            pickupType   = CommonPickupItemId_FirstAidKit;
            eventFlagIdx = EventFlag_M3S03_FirstAidKit1;
            break;

        case 130:
            pickupType   = CommonPickupItemId_ShotgunShells;
            eventFlagIdx = EventFlag_M3S03_ShotgunShells;
            break;
    }

    Event_CommonItemTake(pickupType, eventFlagIdx);
}

#include "maps/shared/MapEvent_DoorJammed.h" // 0x800D15B8

#include "maps/shared/MapEvent_DoorLocked.h" // 0x800D164C

#include "maps/shared/MapEvent_DoorUnlocked.h" // 0x800D16E0

void func_800D1774(void) // 0x800D1774
{
    Event_ItemTake(InvItemId_PlateOfHatter, DEFAULT_PICKUP_ITEM_COUNT, EventFlag_M3S03_PickupPlateOfHatter, 15);
}

void func_800D17A0(void) // 0x800D17A0
{
    Event_ItemTake(InvItemId_PlateOfCat, DEFAULT_PICKUP_ITEM_COUNT, EventFlag_M3S03_PickupPlateOfCat, 16);
}

void func_800D17CC(void) // 0x800D17CC
{
    Event_ItemTake(InvItemId_PlateOfTurtle, DEFAULT_PICKUP_ITEM_COUNT, EventFlag_M3S03_PickupPlateOfTurtle, 17);
}

void func_800D17F8(s32 arg0, s32 arg1) // 0x800D17F8
{
    s32       idx;
    s32       i;
    SPRT*     sprt;
    DR_TPAGE* tpage;

    Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);

    sprt = GsOUT_PACKET_P;

    for (i = 0; (arg1 > 0) ? (i < 5) : (i < 4); i++)
    {
        if (D_800D8140[i] == 8)
        {
            continue;
        }

        if ((i == arg0 || i == 4) && arg1 < Q12(1.0f))
        {
            if (arg1 <= 0)
            {
                continue;
            }

            setRGBC0(sprt, arg1 >> 5, arg1 >> 5, arg1 >> 5, PRIM_RECT | RECT_BLEND | RECT_TEXTURE);
        }
        else
        {
            setRGBC0(sprt, 128, 128, 128, PRIM_RECT | RECT_TEXTURE);
        }

        idx = (i < 4) ? i : arg0;

        setXY0Fast(sprt, D_800D6B40[idx][0] - 160, D_800D6B40[idx][1] - 120);

        if (i < 4)
        {
            sprt->u0 = 64;
            sprt->v0 = D_800D6B50[D_800D8140[idx]] + 16;
        }
        else
        {
            sprt->u0 = D_800D6B40[idx][0];
            sprt->v0 = D_800D6B40[idx][1] + 16;
        }

        setWH(sprt, 28, 28);

        if (i != 4)
        {
            sprt->clut = 0x3CE;
        }
        else
        {
            sprt->clut = 0x38E;
        }

        addPrimFast(&g_OrderingTable0[g_ActiveBufferIdx].org[1], sprt, 4);
        sprt++;
        tpage = sprt;

        if (i < 4)
        {
            setDrawTPage(tpage, 0, 0, 167);
        }
        else
        {
            setDrawTPage(tpage, 0, 0, 197);
        }

        AddPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[1], tpage);
        tpage++;
        sprt = tpage;
    }

    GsOUT_PACKET_P = sprt;
}

void func_800D1A58(void) // 0x800D1A58
{
    VECTOR3 sfxPos;
    s32     temp_a2_2;
    s32     temp_v1_8;
    s32     j;
    s32     i;

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();

            for (i = 0; i < 16; i++)
            {
                if (Savegame_EventFlagGet(EventFlag_265 + i))
                {
                    break;
                }
            }

            if (g_MapEventLastUsedItem != InvItemId_Unequipped || i != 16)
            {
                SysWork_StateStepSet(0, 4);
            }
            else
            {
                SysWork_StateStepIncrement(0);
            }
            break;

        case 1:
            sfxPos.vx = MAP_POINTS[g_MapEventData->mapPointIdx].positionX;
            sfxPos.vy = Q12(-1.2f);
            sfxPos.vz = MAP_POINTS[g_MapEventData->mapPointIdx].positionZ;

            Sfx_WithFlagsPlay(Sfx_DoorLocked, &sfxPos, Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 2:
            Event_WaitTimer(Q12(0.2f), false);
            break;

        case 3:
            Event_DisplayMapMsg(false, 12, false, false, 0, false);
            break;

        case 4:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, false, false, false);
            break;

        case 5:
            g_SysWork.bgmStatusFlags |= BgmStatusFlag_Pause;

            Fs_QueueStartReadTim(FILE_TIM_COLORS_TIM, (void*)0x801D2600, &g_ItemInspectionImg);

            for (i = 0, Fs_QueueWaitForEmpty(), D_800D8145 = 0; i < 4; i++)
            {
                for (j = 0; j < 4; j++)
                {
                    if (Savegame_EventFlagGet((EventFlag_265 + i) + j * 4))
                    {
                        D_800D8140[i] = j;
                        D_800D8145++;
                        break;
                    }
                }

                if (j == 4)
                {
                    D_800D8140[i] = 8;
                }
            }

            D_800D6BD0 = 0;
            D_800D6BD4 = 0;
            D_800D8144 = 8;

            SysWork_StateStepIncrement(0);

        case 6:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, false, false, false);
            func_800D17F8(8, 0);

            if (g_SysWork.sysStateSteps[0] != 6)
            {
                if (g_MapEventLastUsedItem != InvItemId_Unequipped)
                {
                    SysWork_StateStepSet(0, 7);
                    D_800D8144 = g_MapEventLastUsedItem - InvItemId_PlateOfTurtle;
                }
                else if (D_800D8145 == 0)
                {
                    SysWork_StateStepSet(0, 15);
                }
                else
                {
                    SysWork_StateStepSet(0, 11);
                }
            }
            break;

        case 7:
            func_800D17F8(8, 0);
            Event_DisplayMapMsg(false, MAP_MSG(47), false, false, 0, false);
            break;

        case 8:
            func_800D17F8(8, 0);

            D_800D6BD0 += (g_Controller0->normalizedSticks.sticks_0.leftX << 14) / 75;
            D_800D6BD0  = CLAMP_RANGE(D_800D6BD0, Q12(-100.0f), Q12(100.0f));

            D_800D6BD4 += (g_Controller0->normalizedSticks.sticks_0.leftY << 14) / 75;
            D_800D6BD4  = CLAMP_RANGE(D_800D6BD4, Q12(-100.0f), Q12(100.0f));

            Game_TimerUpdate();

            Gfx_CursorDraw((s16)(FP_FROM(D_800D6BD0, Q12_SHIFT) + 8), FP_FROM(D_800D6BD4, Q12_SHIFT) + 8,
                          8, 8, 0, 0x40, 0x20, 0x20, 0x80, 0xC0, 0, 12);

            if (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.enter)
            {
                for (i = 0; i < 8; i++)
                {
                    if ((D_800D6B40[i][0] - 160) > FP_FROM(D_800D6BD0, Q12_SHIFT))
                    {
                        continue;
                    }

                    if ((D_800D6B40[i][0] - 132) <= FP_FROM(D_800D6BD0, Q12_SHIFT))
                    {
                        continue;
                    }

                    if ((D_800D6B40[i][1] - 120) > FP_FROM(D_800D6BD4, Q12_SHIFT))
                    {
                        continue;
                    }

                    if ((D_800D6B40[i][1] - 92) <= FP_FROM(D_800D6BD4, Q12_SHIFT))
                    {
                        continue;
                    }

                    if (i >= 4 || D_800D8140[i] != 8)
                    {
                        SysWork_StateStepSet(0, 10);
                        break;
                    }

                    D_800D8140[i] = D_800D8144;

                    temp_a2_2 = EventFlag_265;

                    Savegame_EventFlagSet((D_800D8144 * 4) + (i + temp_a2_2));

                    D_800D8144 = i;
                    D_800D6BD8 = 0;

                    SD_Call(Sfx_Unk1524);
                    SysWork_StateStepSet(0, 9);

                    for (j = 0; j < 4; j++)
                    {
                        if (D_800D8140[j] != j)
                        {
                            break;
                        }
                    }

                    if (j == 4)
                    {
                        Savegame_EventFlagSet(EventFlag_MapMark_713);
                    }

                    break;
                }
            }
            else if (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.cancel)
            {
                Event_InvItemCmd(InvItemCmd_AddItem, D_800D8144 + InvItemId_PlateOfTurtle, 1, false);
                SysWork_StateStepSet(0, 16);
            }
            break;

        case 9:
            func_800D17F8(D_800D8144, D_800D6BD8);

            D_800D6BD8 += 0x40;
            if (D_800D6BD8 >= Q12(1.2f))
            {
                if (Savegame_EventFlagGet(EventFlag_MapMark_713))
                {
                    SD_Call(Sfx_Unk1343);
                }

                SysWork_StateStepSet(0, 16);
            }
            break;

        case 10:
            func_800D17F8(8, 0);
            Event_DisplayMapMsg(false, MAP_MSG(48), false, false, 0, false);

            if (g_SysWork.sysStateSteps[0] != 10)
            {
                SysWork_StateStepSet(0, 8);
            }
            break;

        case 11:
            func_800D17F8(8, 0);
            Event_DisplayMapMsg(false, MAP_MSG(49), false, false, 0, false);
            break;

        case 12:
            func_800D17F8(8, 0);

            D_800D6BD0 += (g_Controller0->normalizedSticks.sticks_0.leftX << 14) / 75;
            D_800D6BD0  = CLAMP_RANGE(D_800D6BD0, Q12(-100.0f), Q12(100.0f));

            D_800D6BD4 += (g_Controller0->normalizedSticks.sticks_0.leftY << 14) / 75;
            D_800D6BD4  = CLAMP_RANGE(D_800D6BD4, Q12(-100.0f), Q12(100.0f));

            Game_TimerUpdate();

            Gfx_CursorDraw((s16)(FP_FROM(D_800D6BD0, Q12_SHIFT) + 8), FP_FROM(D_800D6BD4, Q12_SHIFT) + 8,
                          8, 8, 0, 0x40, 0x20, 0x20, 0x80, 0xC0, 0, 0xC);

            if (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.enter)
            {
                for (i = 0; i < 8; i++)
                {
                    if ((D_800D6B40[i][0] - 160) > FP_FROM(D_800D6BD0, Q12_SHIFT))
                    {
                        continue;
                    }

                    if ((D_800D6B40[i][0] - 132) <= FP_FROM(D_800D6BD0, Q12_SHIFT))
                    {
                        continue;
                    }
                    if ((D_800D6B40[i][1] - 120) > FP_FROM(D_800D6BD4, Q12_SHIFT))
                    {
                        continue;
                    }
                    if ((D_800D6B40[i][1] - 92) <= FP_FROM(D_800D6BD4, Q12_SHIFT))
                    {
                        continue;
                    }

                    if (i >= 4)
                    {
                        SysWork_StateStepSet(0, 14);
                    }
                    else
                    {
                        if (D_800D8140[i] == 8)
                        {
                            break;
                        }

                        if (D_800D8140[i] == 0)
                        {
                            Event_InvItemCmd(InvItemCmd_AddItem, InvItemId_PlateOfTurtle, 1, false);
                        }
                        else if (D_800D8140[i] == 1)
                        {
                            Event_InvItemCmd(InvItemCmd_AddItem, InvItemId_PlateOfHatter, 1, false);
                        }
                        else if (D_800D8140[i] == 2)
                        {
                            Event_InvItemCmd(InvItemCmd_AddItem, InvItemId_PlateOfCat, 1, false);
                        }
                        else if (D_800D8140[i] == 3)
                        {
                            Event_InvItemCmd(InvItemCmd_AddItem, InvItemId_PlateOfQueen, 1, false);
                        }

                        temp_v1_8 = EventFlag_265;

                        Savegame_EventFlagClear(D_800D8140[i] * 4 + (i + temp_v1_8));

                        D_800D8144 = i;
                        D_800D6BD8 = 0x1000;

                        SysWork_StateStepSet(0, 13);
                        SD_Call(Sfx_Unk1524);
                    }
                    break;
                }
            }
            else if (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.cancel)
            {
                SysWork_StateStepSet(0, 16);
            }
            break;

        case 13:
            func_800D17F8(D_800D8144, D_800D6BD8);

            D_800D6BD8 -= 0x40;
            if (D_800D6BD8 <= Q12(-0.2f))
            {
                D_800D8140[D_800D8144] = 8;
                SysWork_StateStepSet(0, 16);
            }
            break;

        case 14:
            func_800D17F8(8, 0);
            Event_DisplayMapMsg(false, MAP_MSG(50), false, false, 0, false);

            if (g_SysWork.sysStateSteps[0] != 14)
            {
                SysWork_StateStepSet(0, 12);
            }
            break;

        case 15:
            Event_DisplayMapMsg(false, MAP_MSG(51), false, false, 0, false);
            func_800D17F8(8, 0);
            break;

        case 16:
            func_800D17F8(8, 0);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, false, false, false);
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, false, false, false);
            break;
    }
}

void func_800D26D0(void) // 0x800D26D0
{
    g_Screen_BackgroundImgGamma = Q8(13.0f / 32.0f);
    Event_DisplayMapMsgWithBg(FILE_TIM_PLATECLR_TIM, Q12(0.0f), Q12(0.0f), 18);
}

void func_800D2708(void) // 0x800D2708
{
    g_Screen_BackgroundImgGamma = Q8(13.0f / 32.0f);
    Event_DisplayMapMsgWithBg(FILE_TIM_LITHGRPH_TIM, Q12(0.0f), Q12(0.0f), MAP_MSG(44));
}

void func_800D2740(void) // 0x800D2740
{
    g_Screen_BackgroundImgGamma = Q8(3.0f / 8.0f);
    Event_DisplayMapMsgWithBg(FILE_TIM_LITHGR_2_TIM, Q12(0.0f), Q12(0.0f), MAP_MSG(45));
}

void func_800D2778(void) // 0x800D2778
{
    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            SysWork_StateStepIncrement(0);

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;
    }
}

void func_800D27F4(void) // 0x800D27F4
{
    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_BgTextureCmd(BgTextureCmd_QueueRead, FILE_TIM_LISAVTR2_TIM, false);

            D_800D6BDA = 0;

            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 146, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 2, Q12(0.0f), false);

            // Warp player.
            g_SysWork.playerWork.player.position.vx = Q12(-62.38f);
            g_SysWork.playerWork.player.position.vz = Q12(-59.66f);
            g_SysWork.playerWork.player.rotation.vy = Q12_ANGLE(320.9f);

            // Warp camera.
            Event_CameraPositionSet(NULL, Q12(-61.03f), Q12(-2.14f), Q12(-62.3f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(NULL, Q12(-62.93f), Q12(-0.95f), Q12(-58.99f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);

            SysWork_StateStepIncrement(0);

        case 3:
            Event_WaitTimer(Q12(2.5f), false);
            break;

        case 4:
            Sfx_WithFlagsPlay(Sfx_Unk1526, &Q12_VECTOR3(-62.6f, -0.5f, -59.0f), Q8(0.5f), SfxFlag_None);
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
            SysWork_StateStepIncrement(0);

        case 9:
            Event_DisplayMapMsgWithAudio(27, &D_800D6BDA, &D_800D6B54);
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);

            if (g_SysWork.sysStateSteps[0] == 9 &&
                (g_Controller0->buttonFlags.clicked & (g_GameWorkPtr->config.controllerConfig.skip |
                                                       g_GameWorkPtr->config.controllerConfig.cancel)))
            {
                SD_Call(19);
                SysWork_StateStepIncrement(0);
            }
            break;

        case 10:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            break;

        case 11:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 147, false);
            SysWork_StateStepIncrement(0);
            break;

        case 12:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimLock, &g_SysWork.playerWork.player, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            SysWork_StateStepIncrement(0);

        case 13:
            Event_WaitTimer(Q12(0.5f), false);
            break;

        case 14:
            Sfx_WithFlagsPlay(Sfx_Unk1526, &Q12_VECTOR3(-62.6f, -0.5f, -59.0f), Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 15:
            Event_CharaAnimUnlockPlayToEnd(&g_SysWork.playerWork.player);
            break;

        default:
            Player_ControlUnfreeze(false);

            SysWork_StateSetNext(SysState_Gameplay);
            Savegame_EventFlagSet(EventFlag_254);

            vcReturnPreAutoCamWork(false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);

            Event_InvItemCmd(InvItemCmd_AddItem, InvItemId_VideoTape, 1, false);
            break;
    }
}

void func_800D2C2C(void) // 0x800D2C2C
{
    Event_ItemTake(InvItemId_BloodPack, DEFAULT_PICKUP_ITEM_COUNT, EventFlag_M3S03_PickupBloodPack, MAP_MSG(40));
}

void func_800D2C58(void) // 0x800D2C58
{
    Event_ItemTake(InvItemId_DisinfectingAlcohol, DEFAULT_PICKUP_ITEM_COUNT, EventFlag_M3S03_PickupDisinfectingAlcohol, MAP_MSG(41));
}

void func_800D2C84(void) // 0x800D2C84
{
    Event_ItemTake(InvItemId_Lighter, DEFAULT_PICKUP_ITEM_COUNT, EventFlag_M3S03_PickupLighter, MAP_MSG(42));
}

void func_800D2CB0(void) // 0x800D2CB0
{
    Event_ItemTake(InvItemId_BasementStoreroomKey, DEFAULT_PICKUP_ITEM_COUNT, EventFlag_M3S03_PickupBasementStoreroomKey, MAP_MSG(43));
}

void func_800D2CDC(void) // 0x800D2CDC
{
    s32 i;
    s32 state;

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 2, false, false);
            ScreenFade_ResetTimestep();

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            // Warp player.
            g_SysWork.playerWork.player.position.vx = Q12(-139.7f);
            g_SysWork.playerWork.player.position.vy = Q12(0.0f);
            g_SysWork.playerWork.player.position.vz = Q12(61.4f);
            g_SysWork.playerWork.player.rotation.vy = ratan2(Q12(-2.0f), Q12(-1.2f));

            WorldGfx_PlayerHeldItemSet(InvItemId_CutsceneBloodPack);
            Fs_QueueWaitForEmpty();

            func_8003D01C();
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 154, false);

            // Warp camera.
            Event_CameraPositionSet(NULL, Q12(-139.3f), Q12(-2.9f), Q12(65.22f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(NULL, Q12(-140.67f), Q12(-1.26f), Q12(61.83f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);

            SysWork_StateStepIncrement(0);
            break;

        case 3:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimLock, &g_SysWork.playerWork.player, 0, false);
            SysWork_StateStepIncrement(0);

        case 4:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.0f), false);
            break;

        case 5:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimUnlock, &g_SysWork.playerWork.player, 0, false);
            SysWork_StateStepIncrement(0);

        case 6:
            Event_WaitTimer(Q12(1.5f), false);
            break;

        case 7:
            state = 4; // @hack Needed to match reg order.
            for (i = 2; i >= 0; i--)
            {
                g_SysWork.npcs[i].model.controlState = state;
            }

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 51, false);

            func_800625F4(&Q12_VECTOR3(-141.7f, 0.0f, 60.2f), 110, 15, 0);
            func_800625F4(&Q12_VECTOR3(-141.7f, 0.0f, 60.2f), 100, 15, 0);
            func_8003D03C();
            WorldGfx_PlayerPrevHeldItem(&g_SysWork.playerCombat);

            // Warp camera.
            Event_CameraPositionSet(NULL, Q12(-140.45f), Q12(0.03f), Q12(59.45f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(NULL, Q12(-143.07f), Q12(-1.65f), Q12(61.97f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);

            SysWork_StateStepIncrement(0);

        case 8:
            Event_WaitTimer(Q12(1.5f), false);
            break;

        case 9:
            Savegame_EventFlagSet(EventFlag_250);

            // Warp camera.
            Event_CameraPositionSet(NULL, Q12(-140.05f), Q12(-3.09f), Q12(63.46f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(NULL, Q12(-142.08f), Q12(-0.92f), Q12(60.79f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);

            SysWork_StateStepIncrement(0);

        case 10:
            Event_WaitTimer(Q12(2.0f), false);
            break;

        default:
            Fs_QueueWaitForEmpty();
            Player_ControlUnfreeze(true);
            SysWork_StateSetNext(SysState_Gameplay);
            func_8003D01C();
            vcReturnPreAutoCamWork(true);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            break;
    }
}

void func_800D30FC(void) // 0x800D30FC
{
    Event_DisplayMapMsgWithBg(FILE_TIM_CAGEEMPT_TIM, Q12(0.0f), Q12(0.0f), MAP_MSG(53));
}

void func_800D3128(void) // 0x800D3128
{
    g_Screen_BackgroundImgGamma = Q8(7.0f / 16.0f);
    Event_DisplayMapMsgWithBg(FILE_TIM_ELCWIRE0_TIM, Q12(0.0f), Q12(0.0f), MAP_MSG(55));
}

const VECTOR3 sharedData_800CB088_3_s01 = { Q12(19.1f), Q12(-2.0f), Q12(59.1f) };

const VECTOR3 sharedData_800CB094_3_s01 = { Q12(17.2f), Q12(-1.0f), Q12(59.1f) };

#include "maps/shared/sharedFunc_800D15F0_3_s01.h" // 0x800D3160

void Map_WorldObjectsInit(void) // 0x800D3EAC
{
    D_800D8428 = 0;

    WorldObject_PlacementInit(&g_WorldObject_Dr[0], "DR1L_HID", 19.1f, 0.0f, 59.1f);
    WorldObject_PlacementInit(&g_WorldObject_Dr[1], "DR1R_HID", 19.1f, 0.0f, 59.1f);
    WorldObject_PlacementInit(&g_WorldObject_Dr[2], "DR3L_HID", 19.1f, 0.0f, 59.1f);
    WorldObject_PlacementInit(&g_WorldObject_Dr[3], "DR3R_HID", 19.1f, 0.0f, 59.1f);
    WorldObject_PlacementInit(&g_WorldObject_Dr[4], "DR2L_HID", 19.1f, 0.0f, 59.1f);
    WorldObject_PlacementInit(&g_WorldObject_Dr[5], "DR2R_HID", 19.1f, 0.0f, 59.1f);

    WorldObject_ModelNameSet(&g_WorldObject_Colors[0], "BLUE_HID");
    WorldObject_ModelNameSet(&g_WorldObject_Colors[1], "GREEN_HI");
    WorldObject_ModelNameSet(&g_WorldObject_Colors[2], "YELLOW_H");
    WorldObject_ModelNameSet(&g_WorldObject_Colors[3], "RED_HIDE");

    WorldObject_Init(&g_WorldObject_Plate0, "PLATE_HI", -140.15f, 0.0f, 59.75f, 0.0f, 180.0f, 0.0f);
    WorldObject_PlacementInit(&g_WorldObject_Plate1, "PLATE_HI", -20.0f, 0.0f, -19.2f);
    WorldObject_Init(&g_WorldObject_Panel, "PANEL_HI", -140.0f, 0.0f, -20.0f, 0.0f, 90.0f, 0.0f);
    WorldObject_Init(&g_WorldObject_Blood0, "BLOOD_NE", -57.45f, -0.9f, -21.05f, 0.0f, 90.0f, 0.0f);
    WorldObject_Init(&g_WorldObject_Lighter, "LIGHTER_", -100.7f, 0.0f, 57.95f, 0.0f, 180.0f, 0.0f);
    WorldObject_PlacementInit(&g_WorldObject_Alcohol, "ALCOHOL_", -60.8f, 0.0f, 21.6f);
    WorldObject_PlacementInit(&g_WorldObject_Key, "KEY_HIDE", -99.2f, 0.0f, 22.55f);
    WorldObject_Init(&g_WorldObject_SavePad, g_CommonWorldObjectNames[1], -58.6104f, -1.0f, -59.1241f, 0.0f, 9.0f, 0.0f);
    WorldObject_Init(&g_WorldObject_Blood1, "BLOOD_HI", -141.5f, 0.0f, 61.4f, 0.0f, -41.0f, 0.0f);
    WorldObject_PlacementInit(&g_WorldObject_Blood2, "BLOOD_HI", -141.7f, -0.05f, 60.2f);

    WorldObject_ModelNameSet(&g_CommonWorldObjects[0], g_CommonWorldObjectNames[2]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[1], g_CommonWorldObjectNames[3]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[2], g_CommonWorldObjectNames[4]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[3], g_CommonWorldObjectNames[5]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[4], g_CommonWorldObjectNames[6]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[5], g_CommonWorldObjectNames[7]);
}

void Map_WorldObjectsUpdate(void) // 0x800D429C
{
    VECTOR3 colorsPos;
    s32     cellZ0;
    s32     cellX0;
    s32     flags;
    s32     projCellZ0;
    s32     projCellX0;
    q19_12  drZ;
    s32     i;
    MAP_CHUNK_CHECK_VARIABLE_DECL();

    flags = 0;

    if (PLAYER_NOT_IN_MAP_CHUNK(vx, 1, -4, -1, -4) || PLAYER_NOT_IN_MAP_CHUNK(vz, 1, 2, -1, 2))
    {
        D_800D8428 = 0;
    }

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
        case CELL_XZ(17, 18):
            if (g_SysWork.sysState == SysState_Gameplay && (g_WorldObject_Dr->position.vz != Q12(59.1f)))
            {
                drZ = Q12(59.1f);

                for (i = ARRAY_SIZE(g_WorldObject_Dr) - 1; i >= 0; i--)
                {
                    g_WorldObject_Dr[i].position.vz = drZ;
                }
            }

            for (i = 0; i < ARRAY_SIZE(g_WorldObject_Dr); i++)
            {
                WorldObjects_Add(&g_WorldObject_Dr[i].object, &g_WorldObject_Dr[i].position, &SVECTOR3_Zero);
            }
            break;

        case CELL_XZ(15, 20):
            colorsPos.vz = Q12(140.0f);

            if (Savegame_EventFlagGet(EventFlag_265) ||
                Savegame_EventFlagGet(EventFlag_266) ||
                Savegame_EventFlagGet(EventFlag_267) ||
                Savegame_EventFlagGet(EventFlag_268))
            {
                if (Savegame_EventFlagGet(EventFlag_265) || Savegame_EventFlagGet(EventFlag_266))
                {
                    colorsPos.vx = Q12(-20.0f);
                }
                else
                {
                    colorsPos.vx = Q12(-19.6f);
                }

                if (Savegame_EventFlagGet(EventFlag_265) || Savegame_EventFlagGet(EventFlag_268))
                {
                    colorsPos.vy = Q12(0.0f);
                }
                else
                {
                    colorsPos.vy = Q12(0.4f);
                }

                WorldObjects_Add(&g_WorldObject_Colors[0], &colorsPos, &SVECTOR3_Zero);
            }

            if (Savegame_EventFlagGet(EventFlag_269) ||
                Savegame_EventFlagGet(EventFlag_270) ||
                Savegame_EventFlagGet(EventFlag_271) ||
                Savegame_EventFlagGet(EventFlag_272))
            {
                if (Savegame_EventFlagGet(EventFlag_269) || Savegame_EventFlagGet(EventFlag_270))
                {
                    colorsPos.vx = Q12(-20.0f);
                }
                else
                {
                    colorsPos.vx = Q12(-19.6f);
                }

                if (Savegame_EventFlagGet(EventFlag_270) || Savegame_EventFlagGet(EventFlag_271))
                {
                    colorsPos.vy = Q12(0.0f);
                }
                else
                {
                    colorsPos.vy = Q12(-0.4f);
                }

                WorldObjects_Add(&g_WorldObject_Colors[1], &colorsPos, &SVECTOR3_Zero);
            }

            if (Savegame_EventFlagGet(EventFlag_273) ||
                Savegame_EventFlagGet(EventFlag_274) ||
                Savegame_EventFlagGet(EventFlag_275) ||
                Savegame_EventFlagGet(EventFlag_276))
            {
                if (Savegame_EventFlagGet(EventFlag_275) || Savegame_EventFlagGet(EventFlag_276))
                {
                    colorsPos.vx = Q12(-20.0f);
                }
                else
                {
                    colorsPos.vx = Q12(-20.4f);
                }

                if (Savegame_EventFlagGet(EventFlag_274) || Savegame_EventFlagGet(EventFlag_275))
                {
                    colorsPos.vy = Q12(0.0f);
                }
                else
                {
                    colorsPos.vy = Q12(-0.4f);
                }

                WorldObjects_Add(&g_WorldObject_Colors[2], &colorsPos, &SVECTOR3_Zero);
            }

            if (Savegame_EventFlagGet(EventFlag_277) ||
                Savegame_EventFlagGet(EventFlag_278) ||
                Savegame_EventFlagGet(EventFlag_279) ||
                Savegame_EventFlagGet(EventFlag_280))
            {
                if (Savegame_EventFlagGet(EventFlag_279) || Savegame_EventFlagGet(EventFlag_280))
                {
                    colorsPos.vx = Q12(-20.0f);
                }
                else
                {
                    colorsPos.vx = Q12(-20.4f);
                }

                if (Savegame_EventFlagGet(EventFlag_277) || Savegame_EventFlagGet(EventFlag_280))
                {
                    colorsPos.vy = Q12(0.0f);
                }
                else
                {
                    colorsPos.vy = Q12(0.4f);
                }

                WorldObjects_Add(&g_WorldObject_Colors[3], &colorsPos, &SVECTOR3_Zero);
            }
            break;

        case CELL_XZ(12, 18):
            if (Savegame_EventFlagGet(EventFlag_250))
            {
                flags |= 1 << 2;
            }
            else
            {
                flags |= 1 << 1;
            }

            if (!Savegame_EventFlagGet(EventFlag_M3S03_PickupPlateOfHatter))
            {
                WorldObjects_Add(&g_WorldObject_Plate0.object, &g_WorldObject_Plate0.position, &g_WorldObject_Plate0.rotation);
            }

            if (Savegame_EventFlagGet(EventFlag_250))
            {
                WorldObjects_Add(&g_WorldObject_Blood2.object, &g_WorldObject_Blood2.position, &SVECTOR3_Zero);
            }

            if (!D_800D8428)
            {
                D_800D8428++;

                func_800625F4(&Q12_VECTOR3(-141.5f, 0.0f, 61.4f), 0x48, 15, 1);
                func_800625F4(&Q12_VECTOR3(-141.5f, 0.0f, 61.35f), 0x28, 15, 1);

                if (Savegame_EventFlagGet(EventFlag_250))
                {
                    func_800625F4(&Q12_VECTOR3(-141.7f, 0.0f, 60.2f), 0x6E, 15, 1);
                    func_800625F4(&Q12_VECTOR3(-141.7f, 0.0f, 60.2f), 0x64, 15, 1);
                }
            }
            break;

        case CELL_XZ(15, 15):
            if (!Savegame_EventFlagGet(EventFlag_M3S03_PickupPlateOfCat))
            {
                WorldObjects_Add(&g_WorldObject_Plate1.object, &g_WorldObject_Plate1.position, &SVECTOR3_Zero);
            }
            break;

        case CELL_XZ(12, 15):
            if (!Savegame_EventFlagGet(EventFlag_M3S03_PickupPlateOfTurtle))
            {
                WorldObjects_Add(&g_WorldObject_Panel.object, &g_WorldObject_Panel.position, &g_WorldObject_Panel.rotation);
            }

            if (!Savegame_EventFlagGet(EventFlag_239) &&
                Savegame_EventFlagGet(EventFlag_M3S03_PickupPlateOfTurtle) &&
                g_SysWork.playerWork.player.position.vz > Q12(-18.2f))
            {
                Sfx_WithFlagsPlay(Sfx_Unk1528, &Q12_VECTOR3(-140.0f, -1.2f, -23.5f), Q8_CLAMPED(1.0f), SfxFlag_NoDistAtten);
                Savegame_EventFlagSet(EventFlag_239);
            }
            break;

        case CELL_XZ(13, 18):
            if (!Savegame_EventFlagGet(EventFlag_M3S03_PickupLighter))
            {
                WorldObjects_Add(&g_WorldObject_Lighter.object, &g_WorldObject_Lighter.position, &g_WorldObject_Lighter.rotation);
            }
            break;

        case CELL_XZ(14, 17):
            if (!Savegame_EventFlagGet(EventFlag_M3S03_PickupDisinfectingAlcohol))
            {
                WorldObjects_Add(&g_WorldObject_Alcohol.object, &g_WorldObject_Alcohol.position, &SVECTOR3_Zero);
            }
            break;

        case CELL_XZ(13, 17):
            if (!Savegame_EventFlagGet(EventFlag_M3S03_PickupBasementStoreroomKey))
            {
                WorldObjects_Add(&g_WorldObject_Key.object, &g_WorldObject_Key.position, &SVECTOR3_Zero);
            }
            break;

        case CELL_XZ(14, 15):
            if (!Savegame_EventFlagGet(EventFlag_M3S03_PickupBloodPack))
            {
                WorldObjects_Add(&g_WorldObject_Blood0.object, &g_WorldObject_Blood0.position, &g_WorldObject_Blood0.rotation);
            }
            break;

        case CELL_XZ(14, 14):
            WorldObjects_Add(&g_WorldObject_SavePad.object, &g_WorldObject_SavePad.position, &g_WorldObject_SavePad.rotation);
            break;
    }

    Collision_TriggerFlagBitsClear(CollisionTriggerFlag_All);
    Collision_TriggerFlagBitsSet(flags);

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -4, -1, -4) && PLAYER_IN_MAP_CHUNK(vz, 1, 2, -1, 2))
    {
        D_800D8490 = 1;
    }
    else
    {
        if (D_800D8490 != 0)
        {
            Sd_SfxStop(Sfx_Unk1525);
            Sd_SfxStop(Sfx_Unk1527);
            D_800D8490 = 0;
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -1, 0, 0) && PLAYER_IN_MAP_CHUNK(vz, 0, 0, -1, 1))
    {
        if (!Savegame_EventFlagGet(EventFlag_M3S03_FirstAidKit0))
        {
            WorldObjects_Add(&g_CommonWorldObjects[0], &g_CommonWorldObjectPoses[0].position, &g_CommonWorldObjectPoses[0].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -3, -1, -3) && PLAYER_IN_MAP_CHUNK(vz, 1, -2, -1, -2))
    {
        if (!Savegame_EventFlagGet(EventFlag_M3S03_HealthDrink))
        {
            WorldObjects_Add(&g_CommonWorldObjects[1], &g_CommonWorldObjectPoses[1].position, &g_CommonWorldObjectPoses[1].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -2, -1, -2) && PLAYER_IN_MAP_CHUNK(vz, 1, -1, 0, 0))
    {
        if (!Savegame_EventFlagGet(EventFlag_M3S03_HandgunBullets))
        {
            WorldObjects_Add(&g_CommonWorldObjects[3], &g_CommonWorldObjectPoses[2].position, &g_CommonWorldObjectPoses[2].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -2, -1, -2) && PLAYER_IN_MAP_CHUNK(vz, 1, -1, 0, 0))
    {
        if (!Savegame_EventFlagGet(EventFlag_M3S03_FirstAidKit1))
        {
            WorldObjects_Add(&g_CommonWorldObjects[0], &g_CommonWorldObjectPoses[3].position, &g_CommonWorldObjectPoses[3].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -2, -1, -2) && PLAYER_IN_MAP_CHUNK(vz, 1, -2, -1, -2))
    {
        if (!Savegame_EventFlagGet(EventFlag_M3S03_ShotgunShells))
        {
            WorldObjects_Add(&g_CommonWorldObjects[5], &g_CommonWorldObjectPoses[4].position, &g_CommonWorldObjectPoses[4].rotation);
        }
    }
}

