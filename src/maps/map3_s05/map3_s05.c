#include "bodyprog/bodyprog.h"
#include "bodyprog/events/data.h"
#include "bodyprog/world/effects.h"
#include "bodyprog/items/item_screens.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player/player.h"
#include "main/rng.h"
#include "maps/map3/map3_s05.h"
#include "maps/common/particle.h"
#include "maps/common/characters/creeper.h"
#include "maps/common/characters/player.h"
#include "maps/common/characters/puppet_nurse.h"

#include "../src/maps/common/particles/fire.c"

#include "../src/maps/common/particles/particle.c"

#include "../src/maps/common/characters/player.c"

// TODO: Move this line into separate `Chara_Creeper` split.
#include "../src/maps/common/characters/creeper.c" // 0x800CFEAC

#include "../src/maps/common/unk_m3s03.c" // 0x800D2120

#include "../src/maps/common/characters/puppet_nurse.c"

#include "../src/maps/common/map_util.c" // 0x800D5434

#include "maps/common/shared/Map_RoomBgmInit_CheckCond.h" // 0x800D54DC

#include "maps/common/shared/Map_RoomBgmInit_3_s02_CondTrue.h" // 0x800D5510

#include "maps/common/shared/Map_RoomBgmInit_3_s02_CondFalse.h" // 0x800D5550

void GameBoot_LoadScreen_StageString(void) {}

void func_800D57E8(void) // 0x800D57E8
{
    VECTOR3 sfxPos = { MAP_POINTS[g_MapEventData->mapPointIdx].positionX, Q12(-1.2f), MAP_POINTS[g_MapEventData->mapPointIdx].positionZ };

    if (g_MapEventData->mapPointIdx == 25)
    {
        Event_DisplayMapMsgWithSfx(11, Sfx_DoorLocked2, &sfxPos);
    }
    else
    {
        Event_DisplayMapMsgWithSfx(11, Sfx_DoorJammed, &sfxPos);
    }
}

#include "maps/common/shared/MapEvent_DoorLocked.h" // 0x800D58A0

const char* MAP_MESSAGES[] = {
    #include "maps/common/shared/map_msg_common.inc"
#if VERSION_REGION_IS(NTSC)
    /* 15 */ "\tThere_is_a_ ~C2 Video_tape ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 16 */ "~C3\tAlessa ~E ",
    /* 17 */ "\tAlessa...? ~E ",
    /* 18 */ "~J0(1.2)\tWhat's_this? ~E ",
    /* 19 */ "~J0(2.6)\tLooks_like_someone's_been_here... ~E ",
    /* 20 */ "\t~J0(0.4) ~E ",
    /* 21 */ "\tThere_is_an_ ~C2 Examination_room_key ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 22 */ "\tLooks_like_a_hallway ~N\n\tunder_the_grate. ",
    /* 23 */ "\tCan't_move_it_since_the_vines ~N\n\thave_grown_around_it. ~E ",
    /* 24 */ "\tNo. ~N\n\tThe_fire_goes_out_right_away ~N\n\twhen_I_light_the_vines. ~E ",
    /* 25 */ "\tPoured_ ~C2 Disinfecting_alcohol ~C7  ~N\n\ton_the_vines. ~E ",
    /* 26 */ "\tThe_vines_absorbed_the_alcohol. ~E ",
    /* 27 */ "\tUsed_the_ ~C2 Basement_storeroom_key ~C7 . ~E ",
    /* 28 */ "\tDo_you_want_to_push ~N\n\tthe_cabinet? ~S4 ",
    /* 29 */ "\tThe_generator_is_on. ~E ",
    /* 30 */ "\tThere_are_skid_marks_from ~N\n\tthe_cabinet_being_moved. ~E ",
    /* 31 */ "\tThere_is_a_ ~C2 Hammer ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 32 */ "\tA_bed?_What's_it_doing_here? ~N\n\tWho_would... ~E ",
    /* 33 */ "\tA_body_is_laid_out_on_the_bed. ~N\n\tDon't_feel_like_checking_this_out. ~E ",
    /* 34 */ "\tNothing_helpful. ~E ",
    /* 35 */ "\tNothing_useful_in_the_boxes. ~E ",
    /* 36 */ "\tThere's_a_bottle_filled ~N\n\twith_some_kind_of_drug. ~N\n\tBetter_leave_it. ~E ",
    /* 37 */ "\tThere_are_some_empty_bottles. ~E ",
    /* 38 */ "\tNothing_special_in_the_box. ~N\n\tNothing_to_worry_about. ~E ",
    /* 39 */ "\tNothing_inside. ~E "
#elif VERSION_REGION_IS(NTSCJ)
    /* 15 */ "\t~C2 ビデオテープ ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 16 */ "~C3\tアレッサ ~E ",
    /* 17 */ "\tアレッサ……？ ~E ",
    /* 18 */ "~J0(1.2)\tここは……？ ~E ",
    /* 19 */ "~J0(2.6)\tここには　誰かいたような形跡があるな…… ~E ",
    /* 20 */ "\t~J0(0.4) ~E ",
    /* 21 */ "\t~C2 診察室の鍵 ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 22 */ "\t床の格子の下には ~N\n\t通路のようなものが見える ",
    /* 23 */ " \tしかし針金のように堅いツタが絡まっていて ~N\n\t格子を動かすことはできない ~E ",
    /* 24 */ "\t駄目だ…… ~N\n\tツタに火をつけても　すぐに消えてしまう ~E ",
    /* 25 */ "\tツタに ~C2 消毒用アルコール ~C7 を振りかけた ~E ",
    /* 26 */ "\tツタはアルコールで湿っている ~E ",
    /* 27 */ "\t~C2 地下倉庫の鍵 ~C7 を使った ~E ",
    /* 28 */ "\t棚を押しますか？ ~S4 ",
    /* 29 */ "\t発電機は動いている ~E  ",
    /* 30 */ "\t床に何かを引きずったような跡がある ~E ",
    /* 31 */ "\t~C2 ハンマー ~C7 が置いてある ~N\n\t取りますか？ ~S4 ",
    /* 32 */ "\tこんなところにベッドが……？ ~N\n\t……誰が？ ~E ",
    /* 33 */ "\t死体がベッドに寝かされている ~N\n\tあれこれと調べる気にはなれない ~E ",
    /* 34 */ "\t特に役に立ちそうなものはない ~E ",
    /* 35 */ "\tダンボール箱の中に ~N\n\t役に立ちそうなものは入っていない ~E ",
    /* 36 */ "\t中身のわからない薬のビンが置かれている ~N\n\t無視していった方がいいだろう ~E ",
    /* 37 */ "\t空のビンが置かれている ~E ",
    /* 38 */ "\t箱の中にはろくなものが入っていない ~N\n\t特に気にする必要はないだろう ~E ",
    /* 39 */ "\t棚の中には何も入っていない ~E "
#endif
};

void func_800D5934(void) // 0x800D5934
{
    u32 pickupType;
    s32 eventFlagIdx;

    pickupType   = CommonPickupItemId_FirstAidKit;
    eventFlagIdx = 0;

    switch (g_MapEventData->mapPointIdx)
    {
        case 96:
            pickupType   = CommonPickupItemId_Ampoule;
            eventFlagIdx = EventFlag_M3S05_Ampoule;
            break;

        case 97:
            pickupType   = CommonPickupItemId_HealthDrink;
            eventFlagIdx = EventFlag_M3S05_HealthDrink;
            break;

        case 98:
            pickupType   = CommonPickupItemId_HandgunBullets;
            eventFlagIdx = EventFlag_M3S05_HandgunBullets;
            break;

        case 99:
            pickupType   = CommonPickupItemId_ShotgunShells;
            eventFlagIdx = EventFlag_M3S05_ShotgunShells;
            break;
    }

    Event_CommonItemTake(pickupType, eventFlagIdx);
}

void func_800D59D4(void) // 0x800D59D4
{
    VECTOR3 sfxPos = { MAP_POINTS[g_MapEventData->mapPointIdx].positionX, Q12(-1.2f), MAP_POINTS[g_MapEventData->mapPointIdx].positionZ };

    Player_ItemRemove(InvItemId_BasementStoreroomKey, 1);
    Event_DisplayMapMsgWithSfx(27, Sfx_UseKey, &sfxPos);

    Savegame_EventFlagSet(EventFlag_MapMark_FogHospital3F_NearElevatorBroken);
    Savegame_EventFlagSet(EventFlag_256);
}

void func_800D5A98(void) // 0x800D5A98
{
    g_Screen_BackgroundImgGamma = Q8(10.0f / 32.0f);

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_BgTextureCmd(BgTextureCmd_QueueRead, FILE_TIM_IVIES_TIM, false);
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            Event_CharaAnimPlayToEnd(&PLAYER_CHARA(), 59);
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
            SD_Call(Sfx_Base);
            SysWork_StateStepIncrement(0);

        case 7:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_DisplayMapMsg(false, 22, 0, 0, 0, false);
            break;

        case 8:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            break;

        case 9:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            SysWork_StateStepIncrement(0);

        case 10:
            Event_CharaAnimPlayToEnd(&PLAYER_CHARA(), 60);
            break;

        default:
            // Return to gameplay.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;
    }
}

const VECTOR3 D_800CB35C = { Q12(21.1f), Q12(0.0f), Q12(-20.0f) };

void func_800D5C98(void) // 0x800D5C98
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
            // Warp player.
            PLAYER_CHARA().position.vx     = Q12(20.4f);
            PLAYER_CHARA().position.vz     = Q12(-20.0f);
            PLAYER_CHARA().rotation.vy     = Q12_ANGLE(90.0f);
            PLAYER_CHARA().collision.state = CharaCollisionState_Default;

            // Warp camera.
            Event_CameraPositionSet(NULL, Q12(18.29f), Q12(-0.92f), Q12(-19.71f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(NULL, Q12(22.24f), Q12(-0.76f), Q12(-20.32f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);

            SysWork_StateStepIncrement(0);

        case 3:
            Event_CharaAnimPlayToEnd(&PLAYER_CHARA(), 59);
            break;

        case 4:
            Event_WaitTimer(Q12(0.6f), false);
            break;

        case 5:
            Sfx_WithFlagsPlay(Sfx_Unk1532, &D_800CB35C, Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 6:
            Event_WaitTimer(Q12(0.8f), false);
            break;

        case 7:
            Sfx_WithFlagsPlay(Sfx_Unk1532, &D_800CB35C, Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 8:
            Event_WaitTimer(Q12(0.5f), false);
            break;

        case 9:
            Sfx_WithFlagsPlay(Sfx_Unk1532, &D_800CB35C, Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 10:
            Event_WaitTimer(Q12(1.0f), false);
            break;

        case 11:
            Event_DisplayMapMsg(false, 24, 0, 0, 0, false);
            break;

        case 12:
            Event_CharaAnimPlayToEnd(&PLAYER_CHARA(), 60);
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);

            // Warp player.
            PLAYER_CHARA().position.vx = Q12(20.6f);
            PLAYER_CHARA().position.vz = Q12(-20.0f);
            PLAYER_CHARA().rotation.vy = Q12_ANGLE(90.0f);

            Event_InvItemCmd(InvItemCmd_AddItem, InvItemId_Lighter, 1, false);
            PLAYER_CHARA().collision.state = CharaCollisionState_Npc;

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            vcReturnPreAutoCamWork(true);
            break;
    }
}

void func_800D5FC4(void) // 0x800D5FC4
{
    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 2, Q12(0.0f), false);

            D_800DACE8 = Q12(-0.3f);

            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            // Warp player.
            PLAYER_CHARA().position.vx = Q12(20.3f);
            PLAYER_CHARA().position.vz = Q12(-20.0f);
            PLAYER_CHARA().rotation.vy = Q12_ANGLE(90.0f);

            // Warp camera.
            Event_CameraPositionSet(NULL, Q12(19.5f), Q12(-1.09f), Q12(-20.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(NULL, Q12(23.49f), Q12(-0.85f), Q12(-19.74f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &PLAYER_CHARA(), 141, false);
            SysWork_StateStepIncrement(0);

        case 3:
            Event_WaitTimer(Q12(1.05f), false);
            break;

        case 4:
            g_SysWork.sysStateStepData[0] += g_DeltaTime;
            if (g_SysWork.sysStateStepData[0] > Q12(3.8f))
            {
                SysWork_StateStepIncrement(0);
                break;
            }

            if (g_SysWork.sysStateStepData[0] < Q12(3.2f))
            {
                if (D_800DACE8 > 0)
                {
                    D_800DACE8 -= g_DeltaTime;
                    if (D_800DACE8 < 0)
                    {
                        D_800DACE8 = ((u16)D_800DACE8 - Q12(0.3f)) - (Rng_Rand16() % Q12(0.1f));
                        Sfx_WithFlagsPlay(Sfx_Unk1536, &Q12_VECTOR3(21.6f, 0.0f, -20.0f), Q8(0.5f), SfxFlag_None);
                        break;
                    }
                }
                else
                {
                    D_800DACE8 += g_DeltaTime;
                    if (D_800DACE8 >= 0)
                    {
                        D_800DACE8 += Q12(0.05f) + (Rng_Rand16() % Q12(0.05f));
                        Sfx_WithFlagsPlay(Sfx_Unk1531, &Q12_VECTOR3(21.6f, 0.0f, -20.0f), Q8(0.5f), SfxFlag_None);
                        break;
                    }
                }
            }
            else
            {
                break;
            }
            break;

        case 5:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimLock, &PLAYER_CHARA(), 0, false);
            Event_DisplayMapMsg(false, 25, 0, 0, 0, false); // "Poured Disinfecting alcohol on the vines."
            break;

        case 6:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimUnlock, &PLAYER_CHARA(), 0, false);
            SysWork_StateStepIncrement(0);

        case 7:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AwaitAnimEnd, &PLAYER_CHARA(), 0, false);
            break;

        default:
            // Return to gameplay.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);

            Savegame_EventFlagSet(EventFlag_M3S05_AlcoholPouredOnVines);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            vcReturnPreAutoCamWork(true);
            break;
    }
}

void func_800D63C4(void) // 0x800D63C4
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
            Event_CharaAnimPlayToEnd(&PLAYER_CHARA(), 59);
            break;

        case 3:
            Event_DisplayMapMsg(false, 26, 0, 0, 0, false);
            break;

        case 4:
            Event_WaitTimer(Q12(0.3f), false);
            break;

        case 5:
            Event_CharaAnimPlayToEnd(&PLAYER_CHARA(), 60);
            break;

        default:
            // Return to gameplay.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;
    }
}

const VECTOR3 D_800CB3E0 = { Q12(21.6f), Q12(-0.3f), Q12(-20.0f) };

const VECTOR3 D_800CB3EC = { Q12(21.6f), Q12(-0.5f), Q12(-20.0f) };

void func_800D64E0(void) // 0x800D64E0
{
    s32 i;

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            D_800DACEC = 0;
            WorldEnv_TransitionStart(7, 9, PrimitiveType_S32, &D_800DACEC, 0, 0x64000);
            break;

        case 5:
        case 7:
        case 9:
            D_800DACEC += Q12_MULT_PRECISE(g_DeltaTime, Q12(5.0f));
            break;

        case 11:
        case 12:
        case 13:
            D_800DACEC += Q12_MULT_PRECISE(g_DeltaTime, Q12(10.0f));
            break;

        case 14:
        case 15:
            D_800DACEC -= Q12_MULT_PRECISE(g_DeltaTime, Q12(20.0f));
            break;
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 2, 0, false);
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            // Warp player.
            PLAYER_CHARA().position.vx     = Q12(20.65f);
            PLAYER_CHARA().position.vz     = Q12(-20.0f);
            PLAYER_CHARA().rotation.vy     = Q12(0.25f);
            PLAYER_CHARA().collision.state = CharaCollisionState_Ignore;
            PLAYER_CHARA().collision.state = CharaCollisionState_Default;

            // Warp camera.
            Event_CameraPositionSet(NULL, Q12(17.15f), Q12(-1.32f), Q12(-18.71f), 0, 0, 0, 0, true);
            Event_CameraLookAtSet(NULL, Q12(20.96f), Q12(-1.13f), -Q12(19.93f), 0, 0, 0, 0, true);

            func_8003D03C();
            Player_EmptyWeaponHandSet();
            SysWork_StateStepIncrement(0);

        case 3:
            Event_CharaAnimPlayToEnd(&PLAYER_CHARA(), 59);
            break;

        case 4:
            Sfx_WithFlagsPlay(Sfx_Unk1532, &D_800CB35C, Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 5:
            Event_WaitTimer(Q12(0.8f), false);
            break;

        case 6:
            Sfx_WithFlagsPlay(Sfx_Unk1532, &D_800CB35C, Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 7:
            Event_WaitTimer(Q12(0.5f), false);
            break;

        case 8:
            Sfx_WithFlagsPlay(Sfx_Unk1532, &D_800CB35C, Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 9:
            Event_WaitTimer(Q12(0.65f), false);
            break;

        case 10:
            sharedData_800D8568_1_s05.field_1  = 3;
            sharedData_800D8568_1_s05.flags  = 0x18;
            sharedData_800D8568_1_s05.field_C  = 0x12C;
            sharedData_800D8568_1_s05.field_12 = Q12(-0.2f);
            sharedData_800D8568_1_s05.field_14 = Q12(0.05f);
            sharedData_800D8568_1_s05.field_18 = Q12(0.6f);
            sharedData_800D8568_1_s05.field_1A = Q12(0.6f);
            sharedData_800D8568_1_s05.field_E  = Q12(-1.0f);
            sharedData_800D8568_1_s05.field_20 = Q12(21.6f);
            sharedData_800D8568_1_s05.field_24 = Q12(-20.0f);
            sharedData_800D8568_1_s05.field_3  = 0x20;
            sharedData_800D8568_1_s05.field_4  = 0x40;
            sharedData_800D8568_1_s05.field_6  = 0x2D;
            sharedData_800D8568_1_s05.field_5  = 0x20;
            sharedData_800D8568_1_s05.field_8  = 0x100;
            sharedData_800D8568_1_s05.field_A  = Q12(1.0f);
            sharedData_800D8568_1_s05.field_2  = 8;

            sharedFunc_800CAAD0_1_s05();
            SD_Call(Sfx_Unk1533);
            LensFlare_LensFlareDisable();
            SysWork_StateStepIncrement(0);

        case 11:
            Event_CharaAnimPlayToEnd(&PLAYER_CHARA(), 60);
            break;

        case 12:
            Event_CharaAnimPlayToEnd(&PLAYER_CHARA(), 81);

            // Warp camera.
            Event_CameraPositionSet(NULL, Q12(23.61f), Q12(-2.78f), Q12(-18.69f), 0, 0, 0, 0, true);
            Event_CameraLookAtSet(NULL, Q12(20.64f), Q12(-0.67f), Q12(-20.33f), 0, 0, 0, 0, true);
            break;

        case 13:
            Event_WaitTimer(Q12(3.0f), false);
            break;

        case 14:
            sharedData_800D8568_1_s05.field_1 = 1;
            sharedData_800D8568_1_s05.field_2 = 4;

            Event_WaitTimer(Q12(0.5f), false);
            break;

        case 15:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, 0, false);
            break;

        case 16:
            Sd_SfxStop(Sfx_Unk1533);
            SysWork_StateStepIncrement(0);

        case 17:
            Event_WaitTimer(Q12(1.0f), false);
            break;

        case 18:
            Sfx_WithFlagsPlay(Sfx_Unk1534, &D_800CB3E0, Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 19:
            Event_WaitTimer(Q12(0.5f), false);
            break;

        default:
            // Return to gameplay.
            Player_ControlUnfreeze(true);
            SysWork_StateSetNext(SysState_Gameplay);

            for (i = 0; i < 450; i++)
            {
                if (sharedData_800DFB7C_0_s00[i].field_A == 8 || sharedData_800DFB7C_0_s00[i].field_A == 11)
                {
                    sharedData_800DFB7C_0_s00[i].field_A = 0;
                }
            }

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, 0, false);
            Savegame_EventFlagSet(EventFlag_284);

            WorldEnv_PresetsSet(7, 4);
            LensFlare_LensFlareEnable();
            vcReturnPreAutoCamWork(true);

            PLAYER_CHARA().position.vx -= Q12(1.2f);

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, 0, false);

            PLAYER_CHARA().collision.state = CharaCollisionState_Ignore;
            PLAYER_CHARA().collision.state = CharaCollisionState_Npc;

            func_8003D01C();
            Player_WeaponAttackRestore();
            break;
    }

    if (g_SysWork.sysStateSteps[0] > 10 && g_SysWork.sysStateSteps[0] < 16)
    {
        Sfx_WithFalloffAndPitchPlay(Sfx_Unk1533, &D_800CB3EC, 0x80 - Q12_MULT_PRECISE(D_800DD190, 0x80), Q12(16.0f), 0);
    }
}

void func_800D6BB4(void) // 0x800D6BB4
{
    Event_ItemTake(InvItemId_VideoTape, DEFAULT_PICKUP_ITEM_COUNT, EventFlag_M3S05_PickupVideoTape, 15);
}

void func_800D6BE0(void) // 0x800D6BE0
{
    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            SysWork_StateStepIncrement(0);

        case 1:
            Event_BgTextureFadeIn(FILE_TIM_ALESSA_TIM, 0, 0);
            break;

        case 2:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_DisplayMapMsg(false, 16, 0, 0, 0, false);
            break;

        case 3:
            Event_BgTextureFadeOut(0, 0);
            break;

        default:
            // Return to gameplay.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;
    }
}

void func_800D6CF0(void) // 0x800D6CF0
{
    Event_ItemTake(InvItemId_ExaminationRoomKey, DEFAULT_PICKUP_ITEM_COUNT, EventFlag_M3S05_PickupExaminationRoomKey, 21);
}

void func_800D6D1C(void) // 0x800D6D1C
{
    if (Pad_ButtonClickedBind(skip) &&
        g_SysWork.sysStateSteps[0] > 0 && g_SysWork.sysStateSteps[0] < 11)
    {
        SysWork_StateStepSet(0, 15);
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            CutsceneBorder_ForceShow();

            // Warp camera.
            Event_CameraPositionSet(NULL, Q12(18.1f), Q12(-1.71f), Q12(-97.34f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(NULL, Q12(20.04f), Q12(-0.74f), Q12(-100.7f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);

            D_800DAC74 = 0;
            func_8003D03C();
            Player_EmptyWeaponHandSet();
            Game_TurnFlashlightOn();

            SysWork_StateStepIncrement(0);

        case 1:
            Event_DisplayMapMsgWithAudio(18, &D_800DAC74, &D_800DAC70);
            break;

        case 2:
            Event_CharaAnimPlayToEnd(&PLAYER_CHARA(), 116);
            break;

        case 3:
            Event_DisplayMapMsgWithAudio(19, &D_800DAC74, &D_800DAC70);
            break;

        case 4:
            Event_PathWaypointSet(true, 0, 0, Q12_ANGLE(0.0f), Q12(21.7f), Q12(-100.9f));
            Event_PathWaypointSet(true, 0, 1, Q12_ANGLE(0.0f), Q12(21.9f), Q12(-100.3f));
            Event_PathWaypointSet(true, 0, 2, Q12_ANGLE(0.0f), Q12(21.0f), Q12(-99.4f));
            SysWork_StateStepIncrement(0);

        case 5:
            Event_PathWaypointExecutePlayer(53, 3, false);
            Event_WaitTimer(Q12(2.0f), false);
            break;

        case 6:
            // Warp camera.
            Event_CameraPositionSet(NULL, Q12(18.44f), Q12(-4.54f), Q12(-100.98f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(NULL, Q12(20.54f), Q12(-1.58f), Q12(-99.3f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            SysWork_StateStepIncrement(0);

        case 7:
            Event_PathWaypointExecutePlayer(53, 3, false);
            break;

        case 8:
            Event_DisplayMapMsg(false, 20, 0, 0, 0, false);
            break;

        case 9:
            Event_BgTextureFadeIn(FILE_TIM_ALESSA_TIM, 0, 0);
            break;

        case 10:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_DisplayMapMsg(false, 16, 0, 0, 0, false);
            break;

        case 11:
            vcReturnPreAutoCamWork(true);
            SysWork_StateStepIncrement(0);

        case 12:
            Event_BgTextureFadeOut(0, 0);
            break;

        case 13:
            Event_DisplayMapMsg(false, 17, 0, 0, 0, false);
            break;

        case 15:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 16:
            vcReturnPreAutoCamWork(true);

            // Warp player.
            PLAYER_CHARA().position.vx = Q12(21.0f);
            PLAYER_CHARA().position.vz = Q12(-99.4f);
            PLAYER_CHARA().rotation.vy = Q12_ANGLE(0.0f);

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);

        default:
            // Return to gameplay.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);

            func_8003D01C();
            Player_WeaponAttackRestore();

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);

            Savegame_EventFlagSet(EventFlag_260);
            break;
    }
}

void func_800D719C(void) // 0x800D719C
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
            Event_DisplayMapMsg(false, 29, 0, 0, 0, false);
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;
    }

    func_800894B8(0x60); // Hex?
}

void func_800D7280(void) // 0x800D7280
{
    Event_ItemTake(InvItemId_Hammer, DEFAULT_PICKUP_ITEM_COUNT, EventFlag_M3S05_PickupHammer, 31);
}

void func_800D72AC(void) // 0x800D72AC
{
    typedef enum _EventState
    {
        EventState_DontPushCabinet = 3,
        EventState_PushCabinet     = 4
    } e_EventState;

    s32    tmp0;
    q19_12 distAdd;

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            D_800DACF0 = 0;
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            Event_DisplayMapMsg(true, 28, EventState_PushCabinet, EventState_DontPushCabinet, 0, false); // "Do you want to push the cabinet?"
            break;

        case EventState_DontPushCabinet:
            // Return to gameplay.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;

        case EventState_PushCabinet:
            SysWork_StateStepIncrement(0);

        case 5:
            // Warp camera.
            Event_CameraPositionSet(NULL, Q12(57.86f), Q12(-3.62f), Q12(-21.1f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(NULL, Q12(58.37f), Q12(-0.4f), Q12(-23.42f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 2, Q12(0.0f), false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &PLAYER_CHARA(), 105, false);

            PLAYER_CHARA().position.vx = Q12(57.15f);
            PLAYER_CHARA().position.vz = Q12(-22.8f);
            PLAYER_CHARA().rotation.vy = Q12_ANGLE(90.0f);

            SD_Call(Sfx_Unk1535);
            SysWork_StateStepIncrement(0);

        case 6:
            tmp0 = Q12_MULT_PRECISE(g_DeltaTime, DEFAULT_PLAYER_CYLINDER_INNER_RADIUS);
            distAdd = tmp0;
            PLAYER_CHARA().position.vx += distAdd;
            g_WorldObject0.position.vx += distAdd;

            if (g_WorldObject0.position.vx > Q12(61.0f))
            {
                SysWork_StateStepIncrement(0);

            }

            D_800DACF0 += ((u16) g_DeltaTime * 2);
            if (D_800DACF0 > Q12(1.0f))
            {
                D_800DACF0 = Q12(1.0f);
            }

            Sfx_WithFalloffAndPitchPlay(Sfx_Unk1535, &g_WorldObject0.position, D_800DACF0 >> 5, Q12(16.0f), 0);
            break;

        case 7:
            tmp0 = Q12_MULT_PRECISE(g_DeltaTime, DEFAULT_PLAYER_CYLINDER_INNER_RADIUS);
            distAdd = tmp0;
            PLAYER_CHARA().position.vx += distAdd;
            g_WorldObject0.position.vx += distAdd;

            if (g_WorldObject0.position.vx > Q12(61.25f))
            {
                tmp0 = g_WorldObject0.position.vx - Q12(61.25f);
                PLAYER_CHARA().position.vx -= tmp0;
                g_WorldObject0.position.vx -= tmp0;
            }

            D_800DACF0 -= ((u16) g_DeltaTime * 2);
            if (D_800DACF0 < 0)
            {
                D_800DACF0 = 0;
            }

            Sfx_WithFalloffAndPitchPlay(Sfx_Unk1535, &g_WorldObject0.position, D_800DACF0 >> 5, Q12(16.0f), 0);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);

            if (g_SysWork.sysStateSteps[0] != 7)
            {
                Savegame_EventFlagSet(EventFlag_M3S05_CabinetPushed);
            }
            break;

        default:
            // Return to gameplay.
            Player_ControlUnfreeze(true);
            SysWork_StateSetNext(SysState_Gameplay);

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            vcReturnPreAutoCamWork(true);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            Sd_SfxStop(Sfx_Unk1535);
            break;
    }
}

const VECTOR3 sharedData_800CB088_3_s01 = { Q12(19.1f), Q12(-2.0f), Q12(59.1f) };

const VECTOR3 sharedData_800CB094_3_s01 = { Q12(17.2f), Q12(-1.0f), Q12(59.1f) };

#include "maps/common/shared/sharedFunc_800D15F0_3_s01.h" // 0x800D76D8

void Map_WorldObjectsInit(void) // 0x800D8424
{
    D_800DD528 = 0;
    D_800DD52A = 0;

    WorldObject_Init(&g_WorldObject7, "HAMMER_H", 140.4485f, -0.5f, -18.9f, -87.05f, -73.2f, 0.0f);
    WorldObject_Init(&g_WorldObject2, "TAPE_HID", 59.64f, -0.85f, -100.27f, 0.0f, 7.5f, 0.0f);
    WorldObject_Init(&g_WorldObject3, "BED3_HID", 19.6f, 0.0f, -99.6f, 0.0f, 90.0f, 0.0f);
    WorldObject_Init(&g_WorldObject4, "KEY_HIDE", 20.6587f, -0.81055f, -98.93555f, 0.0f, -31.7f, 0.0f);
    WorldObject_Init(&g_WorldObject0, "TANA_HID", 60.0f, 0.0f, -21.6f, 0.0f, 180.0f, 0.0f);

    if (Savegame_EventFlagGet(EventFlag_M3S05_CabinetPushed))
    {
        g_WorldObject0.position.vx = Q12(61.25f);
    }

    WorldObject_ModelNameSet(&g_WorldObject5, "DOR1_HID");
    WorldObject_ModelNameSet(&g_WorldObject6, "DOR2_HID");

    Math_Vector3Set(&D_800DD420, Q12(20.0f), Q12(0.0f), Q12(-20.0f));
    Math_Vector3Set(&D_800DD430, Q12(19.9998f), Q12(-0.682f), Q12(-21.05005f));
    Math_SetSVectorFast(&D_800DD440, Q12_ANGLE(0.0f), Q12_ANGLE(180.0f), Q12_ANGLE(0.0f));
    Math_SetSVectorFast(&D_800DD448, Q12_ANGLE(58.05f), Q12_ANGLE(180.0f), Q12_ANGLE(0.0f));

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

void Map_WorldObjectsUpdate(void) // 0x800D87AC
{
    VECTOR3 soundPos;
    s32     chunkX;
    s32     chunkZ;
    s32     triggerFlags;
    s32     i;
    s32     newZ;

    chunkX = MAP_CHUNK_IDX(PLAYER_CHARA().position.vx);
    chunkZ = MAP_CHUNK_IDX(PLAYER_CHARA().position.vz);

    triggerFlags = CollisionTriggerFlag_None;
    Collision_TriggerFlagBitsClear(CollisionTriggerFlag_All);

    if (chunkX == MAP_CHUNK_IDX_FROM_GRID(1) && chunkZ == MAP_CHUNK_IDX_FROM_GRID(-3) && !Savegame_EventFlagGet(EventFlag_M3S05_PickupVideoTape))
    {
        WorldObjects_Add(&g_WorldObject2.object, &g_WorldObject2.position, &g_WorldObject2.rotation);
    }

    if (chunkX == MAP_CHUNK_IDX_FROM_GRID(0) && chunkZ == MAP_CHUNK_IDX_FROM_GRID(-3))
    {
        WorldObjects_Add(&g_WorldObject3.object, &g_WorldObject3.position, &g_WorldObject3.rotation);

        if (!Savegame_EventFlagGet(EventFlag_M3S05_PickupExaminationRoomKey))
        {
            WorldObjects_Add(&g_WorldObject4.object, &g_WorldObject4.position, &g_WorldObject4.rotation);
        }
    }

    if (chunkX == MAP_CHUNK_IDX_FROM_GRID(1) && chunkZ == MAP_CHUNK_IDX_FROM_GRID(-1))
    {
        if (!Savegame_EventFlagGet(EventFlag_M3S05_CabinetPushed))
        {
            triggerFlags |= CollisionTriggerFlag_Objects;
            Savegame_EventFlagSet(EventFlag_288);
        }
        else
        {
            triggerFlags |= CollisionTriggerFlag_Block;
            Savegame_EventFlagClear(EventFlag_288);
        }

        WorldObjects_Add(&g_WorldObject0.object, &g_WorldObject0.position, &g_WorldObject0.rotation);
    }

    if (chunkX == MAP_CHUNK_IDX_FROM_GRID(0) && chunkZ == MAP_CHUNK_IDX_FROM_GRID(-1))
    {
        if (!Savegame_EventFlagGet(EventFlag_284))
        {
            WorldObjects_Add(&g_WorldObject5, &D_800DD420, &D_800DD440);
            WorldObjects_Add(&g_WorldObject6, &D_800DD420, &D_800DD440);
        }
        else
        {
            WorldObjects_Add(&g_WorldObject6, &D_800DD430, &D_800DD448);
        }
    }

    if (chunkX == MAP_CHUNK_IDX_FROM_GRID(3) && chunkZ == MAP_CHUNK_IDX_FROM_GRID(-1))
    {
        if (!Savegame_EventFlagGet(EventFlag_M3S05_PickupHammer))
        {
            WorldObjects_Add(&g_WorldObject7.object, &g_WorldObject7.position, &g_WorldObject7.rotation);
        }

        if (D_800DD528 == 0)
        {
            SD_Call(Sfx_Unk1503);
            D_800DD528++;
        }

        Sfx_WithFalloffAndPitchPlay(Sfx_Unk1503, &D_800DACD4, Q8(0.5f), Q12(16.0f), 0);
    }
    else if (D_800DD528)
    {
        Sd_SfxStop(Sfx_Unk1503);
        D_800DD528 = 0;
    }

    Collision_TriggerFlagBitsSet(triggerFlags);

    if (chunkX == MAP_CHUNK_IDX_FROM_GRID(3) && chunkZ == MAP_CHUNK_IDX_FROM_GRID(-2))
    {
        if (D_800DD52A > Q12(0.0f))
        {
            D_800DD52A = CLAMP_LOW(D_800DD52A - g_DeltaTime, Q12(0.0f));
        }

        if (D_800DD52A == Q12(0.0f) && Rng_GenerateUInt(0, 63) == 0) // 1 in 64 chance.
        {
            D_800DD52A = Q12(0.8f);
            soundPos.vx = Rng_GenerateInt(Q12(137.5f), Q12(142.5f) - 1);
            soundPos.vy = -Rng_GenerateInt(0, 8191);
            soundPos.vx = Q12(-56.8f) - Rng_GenerateInt(0, Q12(6.4f) - 1);
            Sfx_WithFlagsPlay(Sfx_Unk1537, &soundPos, Rng_GenerateUInt(192, 255), SfxFlag_NoDistAtten);
        }
    }

    if (chunkX == MAP_CHUNK_IDX_FROM_GRID(0) && chunkZ == MAP_CHUNK_IDX_FROM_GRID(1))
    {
        if (g_SysWork.sysState == SysState_Gameplay && g_WorldObject_Dr->position.vz != Q12(59.1f))
        {
            newZ = Q12(59.1f);
            for (i = 5; i >= 0; i--)
            {
                g_WorldObject_Dr[i].position.vz = newZ;
            }
        }

        for (i = 0; i < ARRAY_SIZE(g_WorldObject_Dr); i++)
        {
            WorldObjects_Add(&g_WorldObject_Dr[i].object, &g_WorldObject_Dr[i].position, &SVECTOR3_Zero);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(0, -2))
    {
        if (!Savegame_EventFlagGet(EventFlag_M3S05_Ampoule))
        {
            WorldObjects_Add(&g_CommonWorldObjects[2], &g_CommonWorldObjectPoses[0].position, &g_CommonWorldObjectPoses[0].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(0, -2))
    {
        if (!Savegame_EventFlagGet(EventFlag_M3S05_HealthDrink))
        {
            WorldObjects_Add(&g_CommonWorldObjects[1], &g_CommonWorldObjectPoses[1].position, &g_CommonWorldObjectPoses[1].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(1, -1))
    {
        if (!Savegame_EventFlagGet(EventFlag_M3S05_HandgunBullets))
        {
            WorldObjects_Add(&g_CommonWorldObjects[3], &g_CommonWorldObjectPoses[2].position, &g_CommonWorldObjectPoses[2].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(1, -1))
    {
        if (!Savegame_EventFlagGet(EventFlag_M3S05_ShotgunShells))
        {
            WorldObjects_Add(&g_CommonWorldObjects[5], &g_CommonWorldObjectPoses[3].position, &g_CommonWorldObjectPoses[3].rotation);
        }
    }
}
