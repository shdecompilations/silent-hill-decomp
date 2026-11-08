#include "inline_no_dmpsx.h"

#include <psyq/gtemac.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/events/event_data.h"
#include "bodyprog/events/npc_main.h"
#include "bodyprog/gfx/map_effects.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player/player.h"
#include "main/rng.h"
#include "maps/map4/map4_s03.h"
#include "maps/common/particle.h"
#include "maps/common/characters/player.h"
#include "maps/common/characters/twinfeeler.h"

#include "../src/maps/common/particles/particle_acid.c"

#include "../src/maps/common/particles/particle.c"

#include "../src/maps/common/characters/player.c"

#include "../src/maps/common/unk_m3s03.c" // 0x800D04BC

// TODO: Move to separate `Chara_Twinfeeler` split.
#include "../src/maps/common/characters/twinfeeler.c"     // 0x800D078C

#include "../src/maps/common/map_util.c" // 0x800D606C

#include "maps/common/shared/Map_RoomBgmInit_4_s02.h" // 0x800D6278

void GameBoot_LoadScreen_StageString(void) {}

#include "maps/common/shared/MapEvent_DoorJammed.h" // 0x800D6554

#include "maps/common/shared/MapEvent_DoorLocked.h" // 0x800D65E8

const char* MAP_MESSAGES[] = {
    #include "maps/common/shared/map_msg_common.h"
#if VERSION_REGION_IS(NTSC)
    /* 15 */ "\tDaddy...where_are_you...? ",
    /* 16 */ "~J0(1.0)\tCheryl! ~E  ",
    /* 17 */ "\tThere_is_a_ ~C2 Hunting_rifle ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 18 */ "\tNothing_unusual. ~E ",
    /* 19 */ "\tA_picture_is_projected ~N\n\ton_the_screen. ",
    /* 20 */ "\tCheryl... ~N\n\twhere_could_she_be? ~E "
#elif VERSION_REGION_IS(NTSCJ)
    /* 15 */ "\tパパ……どこにいるの？ ~E ",
    /* 16 */ "~J0(1.0)\tシェリル！ ~E ",
    /* 17 */ "\t~C2 狩猟用ライフル ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 18 */ "\t特に気になることはない ~E ",
    /* 19 */ "\tスクリーンには映像が映し出されている ",
    /* 20 */ "\tシェリル……どこにいるんだ？ ~E "
#endif
};

void MapEvent_CommonItemTake(void) // 0x800D667C
{
    u32 pickupType;
    s32 eventFlagIdx;

    pickupType   = CommonPickupItemId_FirstAidKit;
    eventFlagIdx = 0;

    switch (g_MapEventData->mapPointIdx)
    {
        case 27:
            pickupType   = CommonPickupItemId_FirstAidKit;
            eventFlagIdx = EventFlag_M4S03_FirstAidKit;
            break;

        case 28:
            pickupType   = CommonPickupItemId_RifleShells;
            eventFlagIdx = EventFlag_M4S03_RifleShells0;
            break;

        case 29:
            pickupType   = CommonPickupItemId_RifleShells;
            eventFlagIdx = EventFlag_M4S03_RifleShells1;
            break;
    }

    Event_CommonItemTake(pickupType, eventFlagIdx);
}

void func_800D6704(void) // 0x800D6704
{
    s32 i;

    // Run through NPCs.
    for (i = 0; i < ARRAY_SIZE(g_SysWork.npcs); i++)
    {
        // Break on the first enemy character.
        if (g_SysWork.npcs[i].model.charaId >= Chara_Harry &&
            g_SysWork.npcs[i].model.charaId <= CHARA_LAST_ENEMY_ID)
        {
            break;
        }
    }

    if (i != ARRAY_SIZE(g_SysWork.npcs))
    {
        g_DeltaTime = Q12(0.0f);
    }

    Event_ItemTake(InvItemId_HuntingRifle, RIFLE_AMMO_PICKUP_ITEM_COUNT, EventFlag_M4S03_PickupHuntingRifle, 17);
}

void func_800D6774(void) // 0x800D6774
{
    s_CollisionSurface surface;
    VECTOR3            vec;
    s32                i;

    #define playerChara g_SysWork.playerWork.player

    // Skip.
    if (Pad_ButtonClickedBind(skip) &&
        g_SysWork.sysStateSteps[0] > 0 && g_SysWork.sysStateSteps[0] < 11)
    {
        SysWork_StateStepSet(0, 12);
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            // Warp camera.
            Event_CameraPositionSet(NULL, Q12(120.5f), Q12(-9.5f), Q12(137.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(NULL,
                                  playerChara.position.vx, playerChara.position.vy, playerChara.position.vz,
                                  Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);

            D_800DB9E0 = Q12(0.0f);
            break;

        case 1:
        case 2:
            Event_CameraPositionSet(NULL, Q12(120.5f), Q12(-5.5f), Q12(138.0f), Q12(0.0f), Q12(0.0f), cam_mv_prm_user.max_spd_xz >> 3, cam_mv_prm_user.max_spd_y >> 1, false);
            D_800DB9E0 += g_DeltaTime;

            if (D_800DB9E0 >= Q12(1.8f))
            {
                break;
            }

            i = Rng_Rand16() % (((Q12(2.5f) - D_800DB9E0) / Q12(0.5f)) + 1);
            for (; i > 0; i--)
            {
                vec.vx = Rng_GenerateInt(Q12(118.0f), Q12(123.0f) - 1);
                vec.vy = 0;
                vec.vz = Rng_GenerateInt(Q12(140.0f), Q12(143.0f) - 1);
                func_800D19AC(&vec);
            }
            break;

        case 3:
            // Warp camera.
            Event_CameraPositionSet(NULL, Q12(119.48f), Q12(-0.5f), Q12(143.35f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(NULL, Q12(118.16f), Q12(-0.86f), Q12(139.59f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            break;

        case 9:
        case 10:
            // Warp camera.
            Event_CameraPositionSet(NULL, Q12(119.87f), Q12(-3.4f), Q12(138.71f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(NULL, Q12(119.13f), Q12(-1.49f), Q12(142.15f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            break;
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            func_8008D448();
            Game_FlashlightAttribsSet();

            g_SysWork.lightIntensity = Q12(1.0f);
            Game_TurnFlashlightOn();

            WorldEnv_PresetsSet(6, 3);

            if (Savegame_EventFlagGet(EventFlag_322))
            {
                Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &playerChara, 129, false);
            }
            else
            {
                Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &playerChara, 128, false);
            }

            Collision_SurfaceGet(&surface, playerChara.position.vx, playerChara.position.vz);
            CutsceneBorder_ForceShow();
            Screen_FadeTimestepReset();

            playerChara.position.vy = surface.floorHeight;

            Sfx_WithFlagsPlay(Sfx_Unk1556, &Q12_VECTOR3(119.5f, 0.0f, 141.5f), Q8(0.5f), SfxFlag_None);
            func_80089470();
            SysWork_StateStepIncrement(0);
            break;

        case 1:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.0f), false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimLock, &playerChara, 0, false);
            break;

        case 2:
            Event_WaitTimer(Q12(4.5f), false);
            break;

        case 3:
            func_800D3FB0(&g_SysWork.npcs[0]);
            Savegame_EventFlagSet(EventFlag_329);
            SysWork_StateStepIncrement(0);

        case 4:
            Event_WaitTimer(Q12(0.7f), false);
            break;

        case 5:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimUnlock, &playerChara, 0, false);
            SysWork_StateStepIncrement(0);

        case 6:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AwaitAnimEnd, &playerChara, 0, false);
            break;

        case 7:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &playerChara, 123, false);
            SysWork_StateStepIncrement(0);

        case 8:
            Event_WaitTimer(Q12(2.4f), false);
            break;

        case 9:
            Player_ControlUnfreeze(true);
            Player_ControlFreeze();
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &playerChara, 52, false);

            playerChara.position.vx += Q12(0.5f);
            playerChara.rotation.vy += Q12_ANGLE(45.0f);

            SysWork_StateStepIncrement(0);

        case 10:
            Event_WaitTimer(Q12(3.5f), false);
            break;

        case 11:
            vcReturnPreAutoCamWork(false);
            SysWork_StateStepReset();
            break;

        case 12:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);

            if (g_SysWork.sysStateSteps[0] != 12)
            {
                // Warp camera.
                Event_CameraPositionSet(NULL, Q12(119.87f), Q12(-3.4f), Q12(138.71f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
                Event_CameraLookAtSet(NULL, Q12(119.13f), Q12(-1.49f), Q12(142.15f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);

                func_800D3E18(&g_SysWork.npcs[0]);
                func_800D4000(&g_SysWork.npcs[0]);
            }
            break;

        case 13:
            // Warp player.
            playerChara.position.vx = Q12(120.0f);
            playerChara.position.vz = Q12(141.5f);
            playerChara.rotation.vy = Q12_ANGLE(-45.0f);

            Savegame_EventFlagSet(EventFlag_329);

            // Return to gameplay.
            Player_ControlUnfreeze(true);
            SysWork_StateSetNext(SysState_Gameplay);
            vcReturnPreAutoCamWork(true);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            SysWork_StateStepIncrement(0);

        default:
            // Return to gameplay.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);

            func_800D4000(&g_SysWork.npcs[0]);
            Savegame_EventFlagSet(EventFlag_324);
            func_8003A16C();
            break;
    }

    #undef playerChara
}

void func_800D6F24(void) // 0x800D6F24
{
    s32   var_a0;
    s32   var_a0_2;
    s32   var_a2;
    s32   var_v0;
    void* var_s0;

    #define playerChara g_SysWork.playerWork.player

    // Skip.
    if (Pad_ButtonClickedBind(skip) &&
        g_SysWork.sysStateSteps[0] > 0 && g_SysWork.sysStateSteps[0] < 9)
    {
        SysWork_StateStepSet(0, 9);
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
            playerChara.position.vx = Q12(124.4f);
            playerChara.position.vz = Q12(140.9f);
            playerChara.rotation.vy = Q12_ANGLE(90.0f);

            Event_CameraPositionSet(NULL, Q12(122.1f), Q12(-4.34f), Q12(141.87f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(NULL, Q12(125.65f), Q12(-2.74f), Q12(140.95f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);

            Event_WaitTimer(Q12(2.5f), false);
            break;

        case 4:
            func_800D3FD8(&g_SysWork.npcs[0]);
            SysWork_StateStepIncrement(0);

        case 5:
            Event_WaitTimer(Q12(2.0f), false);
            break;

        case 6:
            Chara_ModelCharaIdClear(&g_SysWork.npcs[0], 0, 0);

            Savegame_EventFlagSet(EventFlag_327);

            Event_CameraPositionSet(NULL, Q12(128.82f), Q12(-0.26f), Q12(139.1f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(NULL, Q12(125.24f), Q12(-0.8f), Q12(140.79f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);

            Sfx_WithFlagsPlay(Sfx_Unk1554, &Q12_VECTOR3(132.0f, -0.75f, 140.0f), Q8(0.5f), SfxFlag_None);

            SysWork_StateStepIncrement(0);

        case 7:
            Event_WaitTimer(Q12(2.0f), false);
            break;

        case 8:
            playerChara.position.vx = Q12(126.86f);
            playerChara.position.vz = Q12(140.36f);

            Event_CameraPositionSet(NULL, Q12(122.55f), Q12(-2.1f), Q12(138.5f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(NULL, Q12(126.39f), Q12(-1.73f), Q12(139.56f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);

            Event_WaitTimer(Q12(3.5f), false);

            if (g_SysWork.sysStateSteps[0] != 8)
            {
                SysWork_StateStepReset();
            }
            break;

        case 9:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 10:
            playerChara.position.vx = Q12(126.86f);
            playerChara.position.vz = Q12(140.36f);
            playerChara.rotation.vy = Q12_ANGLE(90.0f);

            Chara_ModelCharaIdClear(&g_SysWork.npcs[0], 0, 0);

            Savegame_EventFlagSet(EventFlag_327);

            vcReturnPreAutoCamWork(true);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            SysWork_StateStepIncrement(0);
            break;

        default:
            // Return to gameplay.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            vcReturnPreAutoCamWork(false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);

            func_8003A16C();
            break;
    }

    #undef playerChara
}

s16 func_800D7394(void) // 0x800D7394
{
    if (g_DeltaTime != Q12(0.0f))
    {
        D_800DB914 = ((s32)(D_800DB918 * 0x7169AC35) >> 3) ^ 0xA547B39E;
        D_800DB918 = ((u32)(D_800DB914 * 0x892D719C) >> 3) ^ 0xC65A4B97;
    }

    return (u16)D_800DB914 & SHRT_MAX;
}

void func_800D7408(void) // 0x800D7408
{
    D_800E0698.field_0 = 0;
    D_800E0698.field_4 = 0;

    func_800D7450();
    func_800D7548();

    WorldObject_ModelNameSet(&D_800E0698.objRef_238, "REF_NEAR");
}

void func_800D7450(void) // 0x800D7450
{
    e_FsFile textureFileIdx;
    s32      i;

    textureFileIdx = NO_VALUE;

    for (i = 0; i < 3; i++)
    {
        switch (i)
        {
            case 0:
                textureFileIdx          = FILE_TIM_TV1_TIM;
                D_800DB91C.tPage[1] = 29;
                D_800DB91C.u        = 0;
                D_800DB91C.v        = 0;
                D_800DB91C.clutY    = 0;
                break;

            case 1:
                textureFileIdx          = FILE_TIM_TV2_TIM;
                D_800DB91C.tPage[1] = 12;
                D_800DB91C.u        = 32;
                D_800DB91C.v        = 0;
                D_800DB91C.clutY    = 5;
                break;

            case 2:
                textureFileIdx          = FILE_TIM_TV3_TIM;
                D_800DB91C.tPage[1] = 28;
                D_800DB91C.v        = 128;
                D_800DB91C.u        = 0;
                D_800DB91C.clutY    = 10;
                break;
        }

        Fs_QueueStartReadTim(textureFileIdx, FS_BUFFER_1, &D_800DB91C);
    }
}

void func_800D7548(void) // 0x800D7548
{
    s32 i;
    s32 j;

    func_800D761C(&D_800E0698.field_8[9], &D_800DB7D4, 1, 0);

    for (i = 0; i < ARRAY_SIZE(D_800DB7E4); i++)
    {
        for (j = 0; j < ARRAY_SIZE(D_800DB7E4[i]); j++)
        {
            func_800D761C(&D_800E0698.field_8[(i * ARRAY_SIZE(D_800DB7E4)) + j], &D_800DB7E4[i][j], 0, 0);
        }
    }
}

void func_800D761C(s_800E06A0* arg0, s_800DB7D4* arg1, s8 arg2, s32 arg3) // 0x800D761C
{
    arg0->field_20 = arg2;
    arg0->field_22 = 0;
    arg0->field_24 = 0;
    arg0->field_23 = 0;
    arg0->field_2C = 0;
    arg0->field_21 = 0;

    arg0->field_0[0] = arg1->field_0;
    arg0->field_0[3] = arg1->field_8;

    // Switches `vy` to the other `arg1` field?
    arg0->field_0[1].vx = arg1->field_8.vx;
    arg0->field_0[1].vy = arg1->field_0.vy;
    arg0->field_0[1].vz = arg1->field_8.vz;
    arg0->field_0[2].vx = arg1->field_0.vx;
    arg0->field_0[2].vy = arg1->field_8.vy;
    arg0->field_0[2].vz = arg1->field_0.vz;
}

void func_800D76BC(s32 arg0) // 0x800D76BC
{
    D_800E0698.field_0 = arg0;
    D_800E0698.field_4 = 0;

    func_800D7548();
}

bool func_800D76E8(void) // 0x800D76E8
{
    s32  i;
    bool result;

    result = true;

    for(i = 0; i < ARRAY_SIZE(D_800E0698.field_8); i++)
    {
        result &= D_800E0698.field_8[i].field_21;
    }

    return result;
}

void func_800D7718(void) // 0x800D7718
{
    MATRIX      mat;
    s16         temp_v0;
    s32         var_s2;
    s32         i;
    s_800E06A0* ptr;

    Vw_WorldScreenMatrixAtPositionGet(&mat, Q12(160.0f), Q12(0.0f), Q12(40.0f));
    SetRotMatrix(&mat);
    SetTransMatrix(&mat);

    var_s2 = 188;
    if (D_800E0698.field_0 != 0)
    {
        if (g_DeltaTime != Q12(0.0f))
        {
            D_800DB924.pad++;
            if (!(D_800DB924.pad & 0xB))
            {
                var_s2 = 164;
            }
        }

        WorldObjects_Add(&D_800E0698.objRef_238, &D_800DB7C8, (SVECTOR3*)&D_800DB924);
    }

    for (i = 0; i < ARRAY_SIZE(D_800E0698.field_8); i++)
    {
        func_800D7808(&D_800E0698.field_8[i], i);
        func_800D88C8(&D_800E0698.field_8[i], var_s2);
    }
}

void func_800D7808(s_800E06A0* arg0, s32 arg1) // 0x800D7808
{
    s32 temp;

    if (g_DeltaTime == 0)
    {
        return;
    }

    temp = D_800E0698.field_0;
    switch (D_800E0698.field_0)
    {
        case 0:
            func_800D78D4(arg0);
            break;

        case 1:
            if (func_800D78F4(arg0, arg1) != 0)
            {
                arg0->field_21 = temp;
            }
            break;

        case 2:
            if (func_800D7AE0(arg0) != 0)
            {
                func_800D76BC(3);
            }
            break;

        case 3:
            func_800D7F1C(arg0);
            break;
    }
}

bool func_800D78D4(s_800E06A0* arg0) // 0x800D78D4
{
    return func_800D81FC(arg0, 0);
}

const u32 D_800CACA8[10] = {
    0x00000B33,
    0x00001E66,
    0x00001999,
    0x00001B33,
    0x00002333,
    0x00000E66,
    0x00001E66,
    0x00001800,
    0x00001CCC,
    0x000004CC,
};

s32 func_800D78F4(s_800E06A0* arg0, s32 arg1) // 0x800D78F4
{
    u32 sp10[10];
    s32 var_s0;
    s32 var_s2;

    var_s0 = 0;
    var_s2 = 0;

    switch (arg0->field_22)
    {
        case 0:
        default:
            memcpy(&sp10, &D_800CACA8, sizeof(D_800CACA8));
            var_s0 = func_800D81FC(arg0, sp10[arg1] + 2048);
            break;

        case 1:
            var_s0 = func_800D826C(arg0, 0, 409);
            break;

        case 2:
            var_s0 = func_800D826C(arg0, 1, 409);
            break;

        case 3:
            var_s0 = func_800D826C(arg0, 3, 955);
            break;

        case 4:
            var_s0 = func_800D826C(arg0, 4, 546);
            break;

        case 5:
            var_s0 = func_800D8620(arg0, 0, 6144);
            break;

        case 6:
            var_s0 = func_800D826C(arg0, 2, 682);
            break;

        case 7:
            var_s0 = func_800D8354(arg0, 1, 8192);
            break;

        case 8:
            var_s0 = func_800D826C(arg0, 3, 1365);
            break;

        case 9:
            var_s0 = func_800D84C0(arg0, 0, 9011);
            break;

        case 10:
            var_s0 = func_800D826C(arg0, 4, 1365);
            break;

        case 11:
            var_s0 = func_800D85E4(arg0, 409);
            break;

        case 12:
            var_s0 = func_800D826C(arg0, 3, 1365);
            break;

        case 13:
            var_s2 = func_800D84C0(arg0, 1, 2048);
            break;
    }

    arg0->field_24 += g_DeltaTime;

    if (var_s0 != 0)
    {
        arg0->field_22++;
        arg0->field_24 = 0;
    }

    return var_s2;
}

s32 func_800D7AE0(s_800E06A0* arg0) // 0x800D7AE0
{
    s32 var_s0;
    s32 var_s2;

    var_s0 = false;
    var_s2 = false;

    switch (arg0->field_22)
    {
        case 0:
            var_s0 = func_800D84C0(arg0, 0, (func_800D7394() >> 5) + 0x1199);
            break;

        case 1:
            var_s0 = func_800D826C(arg0, 4, (func_800D7394() >> 5) + 0x555);
            break;

        case 2:
            var_s0 = func_800D8620(arg0, 1, (func_800D7394() >> 5) + 0x1800);
            break;

        case 3:
            var_s0 = func_800D826C(arg0, 3, (func_800D7394() >> 5) + 0x555);
            break;

        case 4:
            var_s0 = func_800D8230(arg0, 0, (func_800D7394() >> 5) + 0x2AA);
            break;

        case 5:
            var_s0 = func_800D826C(arg0, 3, (func_800D7394() >> 5) + 0x555);
            break;

        case 6:
            var_s0 = func_800D8230(arg0, 1, (func_800D7394() >> 5) + 0xAAA);
            break;

        case 7:
            var_s0 = func_800D826C(arg0, 4, (func_800D7394() >> 5) + 0x555);
            break;

        case 8:
            var_s0 = func_800D8230(arg0, 2, (func_800D7394() >> 5) + 0x1000);
            break;

        case 9:
            var_s0 = func_800D826C(arg0, 3, (func_800D7394() >> 5) + 0x555);
            break;

        case 10:
            var_s0 = func_800D8230(arg0, 3, (func_800D7394() >> 5) + 0xAAA);
            break;

        case 11:
            var_s0 = func_800D826C(arg0, 4, (func_800D7394() >> 5) + 0x555);
            break;

        case 12:
            var_s0 = func_800D8230(arg0, 4, (func_800D7394() >> 5) + 0x2AA);
            break;

        case 13:
            var_s0 = func_800D8230(arg0, 5, (func_800D7394() >> 5) + 0xAAA);
            break;

        case 14:
            var_s0 = func_800D826C(arg0, 3, (func_800D7394() >> 5) + 0x555);
            break;

        case 15:
            var_s0 = func_800D8230(arg0, 6, (func_800D7394() >> 5) + 0x2AA);
            break;

        case 16:
            var_s0 = func_800D826C(arg0, 4, (func_800D7394() >> 5) + 0x555);
            break;

        case 17:
            var_s0 = func_800D8230(arg0, 7, (func_800D7394() >> 5) + 0xD55);
            break;

        case 18:
            var_s0 = func_800D826C(arg0, 3, (func_800D7394() >> 5) + 0x555);
            break;

        case 19:
            var_s0 = func_800D8230(arg0, 6, (func_800D7394() >> 5) + 0xAAA);
            break;

        case 20:
            var_s0 = func_800D826C(arg0, 4, (func_800D7394() >> 5) + 0x555);
            break;

        case 21:
            var_s0 = func_800D8230(arg0, 8, (func_800D7394() >> 5) + 0x1555);
            break;

        case 22:
            var_s0 = func_800D826C(arg0, 4, (func_800D7394() >> 5) + 0x444);
            break;

        case 24:
            var_s0 = func_800D8620(arg0, 2, (func_800D7394() >> 5) + 0x1800);
            break;

        case 25:
            var_s0 = func_800D85E4(arg0, (func_800D7394() >> 5) + 0x199);
            break;

        case 26:
            var_s0 = func_800D826C(arg0, 4, (func_800D7394() >> 5) + 0x444);
            break;

        case 23:
        case 27:
            var_s0 = func_800D826C(arg0, 3, (func_800D7394() >> 5) + 0x2AA);
            break;

        case 28:
            var_s2 = func_800D8620(arg0, 0, (func_800D7394() >> 5) + 0x1800);
            break;
    }

    arg0->field_24 += g_DeltaTime;

    if (var_s0)
    {
        arg0->field_24  = 0;
        arg0->field_22 += 1;
    }

    return var_s2;
}

void func_800D7F1C(s_800E06A0* arg0) // 0x800D7F1C
{
    s32 var_s0;
    s32 var_s2;

    var_s0 = 0;
    var_s2 = 0;

    switch (arg0->field_22)
    {
        case 0:
        default:
            var_s0 = func_800D8620(arg0, -1, 0xAAA);
            break;

        case 0xA:
            var_s0 = func_800D8620(arg0, -1, 0xAAA);
            break;

        case 0x14:
            var_s0 = func_800D8620(arg0, -1, 0xD55);
            break;

        case 0x1E:
            var_s0 = func_800D8620(arg0, -1, 0x1000);
            break;

        case 0x28:
            var_s0 = func_800D8620(arg0, -1, 0x12AA);
            break;

        case 0x32:
            var_s0 = func_800D8620(arg0, -1, 0x1555);
            break;

        case 0x3C:
            var_s0 = func_800D8354(arg0, 0, 0x1AAA);
            break;

        case 0x46:
            var_s0 = func_800D8354(arg0, 0, 0x1AAA);
            break;

        case 0x50:
            var_s2 = func_800D826C(arg0, 4, 0x3BB);
            break;

        case 0x51:
            var_s0 = func_800D8230(arg0, 0, 0x2AA);
            break;

        case 0x5A:
            var_s2 = func_800D826C(arg0, 3, 0x444);
            break;

        case 0x5B:
            var_s0 = func_800D8230(arg0, 1, 0xAAA);
            break;

        case 0x64:
            var_s2 = func_800D826C(arg0, 4, 0x444);
            break;

        case 0x65:
            var_s0 = func_800D8230(arg0, 2, 0x1000);
            break;

        case 0x6E:
            var_s2 = func_800D826C(arg0, 3, 0x444);
            break;

        case 0x6F:
            var_s0 = func_800D8230(arg0, 3, 0xAAA);
            break;

        case 0x78:
            var_s2 = func_800D826C(arg0, 4, 0x444);
            break;

        case 0x79:
            var_s2 = func_800D8230(arg0, 4, 0x2AA);
            break;

        case 0x7A:
            var_s0 = func_800D8230(arg0, 5, 0xAAA);
            break;

        case 0x82:
            var_s2 = func_800D826C(arg0, 3, 0x555);
            break;

        case 0x83:
            var_s0 = func_800D8230(arg0, 6, 0x2AA);
            break;

        case 0x8D:
            var_s0 = func_800D8230(arg0, 7, 0xD55);
            break;

        case 0x96:
            var_s2 = func_800D826C(arg0, 3, 0x555);
            break;

        case 0x97:
            var_s0 = func_800D8230(arg0, 6, 0xAAA);
            break;

        case 0x8C:
        case 0xA0:
            var_s2 = func_800D826C(arg0, 4, 0x555);
            break;

        case 0xA1:
            var_s0 = func_800D8230(arg0, 8, 0x1000);
            break;
    }

    arg0->field_24 += g_DeltaTime;

    if (var_s2 != 0)
    {
        arg0->field_22++;
        arg0->field_24 = 0;
    }

    if (var_s0 != 0)
    {
        arg0->field_22 = (func_800D7394() % 17) * 10;
        arg0->field_24 = 0;
    }
}

bool func_800D81FC(s_800E06A0* arg0, s32 arg1) // 0x800D81FC
{
    if (arg0->field_24 == 0)
    {
        arg0->field_28 = arg1;
    }

    arg0->field_30 = 0;
    arg0->field_34 = 0;
    arg0->field_36 = 0;

    return arg0->field_24 >= arg0->field_28;
}

bool func_800D8230(s_800E06A0* arg0, s32 arg1, s32 arg2) // 0x800D8230
{
    if (arg0->field_24 == 0)
    {
        arg0->field_28 = arg2;
    }

    arg0->field_30 = arg1 + 6;
    arg0->field_36 = 0;
    arg0->field_34 = 0;

    return arg0->field_24 >= arg0->field_28;
}

bool func_800D826C(s_800E06A0* arg0, s32 arg1, s32 arg2) // 0x800D826C
{
    if (arg0->field_24 == 0)
    {
        arg0->field_28 = arg2;
    }

    arg0->field_30 = 4;
    arg0->field_34 = 0;

    switch (arg1)
    {
        default:
        case 0:
            arg0->field_36 = 0;
            break;

        case 1:
            arg0->field_36 = 64;
            break;

        case 2:
            arg0->field_36 = 32;
            break;

        case 4:
            arg0->field_36 = (u32)((MIN(arg2, arg0->field_24) << 12) / arg2) >> 6; // TODO: Weird shifts, not sure if these are FP related?
            break;

        case 3:
            arg0->field_36 = 64 - ((((MIN(arg2, arg0->field_24) << 12) / arg2) << 6) >> 12);
            break;
    }

    return arg0->field_24 >= arg0->field_28;
}

bool func_800D8354(s_800E06A0* arg0, s32 arg1, s32 arg2) // 0x800D8354
{
    s32 sp10[4];
    s32 temp_a0;
    s32 var_t0;
    s32 var_v1;

    if (arg1 != 0)
    {
        var_t0 = 1;
    }
    else
    {
        var_t0 = (func_800D7394() & 0xFFF) < 0x199;
    }

    if (arg0->field_24 == 0)
    {
        arg0->field_28 = arg2;
    }

    arg0->field_36 = 0;
    arg0->field_34 = 0;
    sp10[0]        = arg2 - 0x555;

    sp10[0] = CLAMP(sp10[0], 0, 0x111);
    var_v1  = Q12_MULT(arg2, 0x4CC);

    if (var_v1 > 0x5DD)
    {
        var_v1 = 0x5DD;
    }

    sp10[1] = var_v1 + sp10[0];
    sp10[2] = sp10[1] + 0x3BB;
    sp10[3] = sp10[1] + 0x45E;

    var_v1 = arg0->field_24;

    if (sp10[0] >= arg0->field_24)
    {
        arg0->field_30 = 2;
        temp_a0        = sp10[0];

        if (var_v1 <= temp_a0)
        {
            var_v1 = var_v1 << 0xC;
        }
        else
        {
            var_v1 = temp_a0 << 0xC;
        }
        arg0->field_36 = Q12_MULT(var_v1 / temp_a0, 0x19) - 0x20;
    }
    else
    {
        if (var_v1 < sp10[1])
        {
            arg0->field_30 = 2;
        }
        else if (var_v1 < sp10[2] || var_t0 == 0 || var_v1 >= sp10[3])
        {
            arg0->field_30 = 3;
        }
        else
        {
            arg0->field_30 = 2;
        }
    }
    return arg0->field_24 >= arg2;
}

bool func_800D84C0(s_800E06A0* arg0, s32 arg1, s32 arg2) // 0x800D84C0
{
    if (arg0->field_24 == 0)
    {
        arg0->field_28 = arg2;
    }

    arg0->field_34 = 0;

    switch (arg1)
    {
        case 0:
        default:
            arg0->field_30 = 5;
            arg0->field_36 = (FP_TO(arg0->field_24 % Q12((1.0f / 3.0f) * 2.0f), Q12_SHIFT) / Q12((1.0f / 3.0f) * 2.0f)) >> 6;

            if (arg0->field_36 > 24 && arg0->field_36 < 29)
            {
                arg0->field_36 = 32;
            }
            break;

        case 1:
            arg0->field_30 = 5;
            arg0->field_34 = func_800D7394() % 6;
            arg0->field_36 = 32;
            break;
    }

    return !(arg0->field_24 < arg0->field_28);
}

bool func_800D85E4(s_800E06A0* arg0, s32 arg1) // 0x800D85E4
{
    if (arg0->field_24 == 0)
    {
        arg0->field_28 = arg1;
    }

    arg0->field_30 = 15;
    arg0->field_36 = 0;
    arg0->field_34 = 0;

    return arg0->field_24 >= arg0->field_28;
}

bool func_800D8620(s_800E06A0* arg0, s32 arg1, s32 arg2) // 0x800D8620
{
    if (arg0->field_24 == 0)
    {
        arg0->field_28 = arg2;
    }

    if (arg1 == NO_VALUE)
    {
        arg1 = func_800D87AC(arg0);
    }

    switch (arg1 + 1)
    {
        default:
        case 0:
            arg0->field_30 = 1;
            arg0->field_34 = 0;
            arg0->field_36 = 0;
            break;

        case 2:
            arg0->field_30 = 1;
            arg0->field_34 = 50;
            arg0->field_36 = 24;
            break;

        case 3:
            arg0->field_30 = 1;
            arg0->field_34 = 128;
            arg0->field_36 = 0;
            break;

        case 4:
            arg0->field_34 = 0;
            arg0->field_30 = 4;
            arg0->field_36 = FP_FROM(-(Q12_DIV(arg0->field_24 % Q12(1.5f), Q12(1.5f)) * 80), Q12_SHIFT) - 108;
            break;

        case 5:
            arg0->field_30 = 1;
            arg0->field_34 = 0;
            arg0->field_36 = 0;
            arg0->field_36 = func_800D7394() % 320;
            break;
    }

    return !(arg0->field_24 < arg0->field_28);
}

s32 func_800D87AC(s_800E06A0* arg0) // 0x800D87AC
{
    s32  temp_v1_2;
    bool cond;

    cond = false;
    if (arg0->field_24 == 0)
    {
        cond           = true;
        arg0->field_2C = func_800D8874();
    }
    else if (arg0->field_24 >= arg0->field_2C)
    {
        cond            = true;
        arg0->field_2C += func_800D8874();
    }

    if (cond)
    {
        temp_v1_2 = Q12_FRACT(func_800D7394());
        if (temp_v1_2 < Q12(0.3f))
        {
            arg0->field_23 = 0;
        }
        else if (temp_v1_2 < Q12(0.5f))
        {
            arg0->field_23 = 1;
        }
        else if (temp_v1_2 < Q12(0.77f))
        {
            arg0->field_23 = 2;
        }
        else if (temp_v1_2 < Q12(0.83f))
        {
            arg0->field_23 = 3;
        }
        else
        {
            arg0->field_23 = 4;
        }
    }

    return arg0->field_23;
}

q19_12 func_800D8874(void) // 0x800D8874
{
    q3_12 temp;
    q3_12 temp2;

    temp2 = Q12_FRACT(func_800D7394());
    temp  = Q12(0.5f);

    return Q12_MULT_PRECISE(temp, temp2) + Q12(1.0f);
}

void func_800D88C8(s_800E06A0* arg0, u8 arg1) // 0x800D88C8
{
    typedef struct
    {
        SVECTOR field_0;
        u16     field_8;
        u16     field_A;
    } s_func_800D88C8;

    SVECTOR         sp10[4];
    s_func_800D88C8 sp30[3][3];
    s32             spA0;
    u16             spA8;
    s32             spB0;
    s32             j;
    s32             temp_a2;
    s32             i;
    s32             var_t0;
    s32             k;
    s32             temp_s4;
    s32             temp_s6;
    s32             temp;
    s_800DB874*     temp_fp;
    POLY_FT4*       poly;
    POLY_FT4*       poly2;

    temp_fp = &D_800DB874[arg0->field_30];

    spA8  = temp_fp->field_0 & 0x10;
    temp  = (temp_fp->field_0 & 0xF) | 0x20;
    spA8 |= temp;

    temp_s6 = arg0->field_34 + temp_fp->field_4 + func_800D7394() % (temp_fp->field_8 + 1);
    spB0    = arg0->field_34 + temp_fp->field_4 + func_800D7394() % (temp_fp->field_8 + 1) + temp_fp->field_6 - 1;
    temp_s4 = arg0->field_36 + temp_fp->field_5 + func_800D7394() % (temp_fp->field_9 + 1);
    var_t0  = temp_s4 + temp_fp->field_7 - 1;

    for (i = 0; i < ARRAY_SIZE(arg0->field_0); i++)
    {
        sp10[i].vz = RotTransPers(&arg0->field_0[i], &sp10[i], &spA0, &spA0) * 4;
    }

    if (arg0->field_20 == 0)
    {
        temp_a2 = MAX(sp10[0].vz, sp10[1].vz);
        temp_a2 = MAX(temp_a2, sp10[2].vz);
        temp_a2 = MAX(temp_a2, sp10[3].vz) >> 3;

        if (temp_a2 > 0 && temp_a2 < ORDERING_TABLE_SIZE - 1)
        {
            poly = GsOUT_PACKET_P;

            setPolyFT4(poly);
            setRGB0(poly, arg1, arg1, arg1);

            *(s32*)&poly->x0 = *(s32*)&sp10[0];
            *(s32*)&poly->x1 = *(s32*)&sp10[1];

            do {} while (false); // @hack

            *(s32*)&poly->x2 = *(s32*)&sp10[2];
            *(s32*)&poly->x3 = *(s32*)&sp10[3];

            setUV4(poly,
                   temp_s6, temp_s4,
                   spB0, temp_s4,
                   temp_s6, var_t0,
                   spB0, var_t0);

            poly->tpage = spA8;
            poly->clut  = temp_fp->field_2;
            AddPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[temp_a2], poly);
            GsOUT_PACKET_P = poly + 1;
        }
    }
    else
    {
        sp30[0][0].field_0 = sp10[0];
        sp30[0][0].field_8 = temp_s6;
        sp30[0][0].field_A = temp_s4;

        sp30[0][2].field_0 = sp10[1];
        sp30[0][2].field_8 = spB0;
        sp30[0][2].field_A = temp_s4;

        sp30[2][0].field_0 = sp10[2];
        sp30[2][0].field_8 = temp_s6;
        sp30[2][0].field_A = var_t0;

        sp30[2][2].field_0 = sp10[3];
        sp30[2][2].field_8 = spB0;
        sp30[2][2].field_A = var_t0;

        sp30[0][1].field_0.vx = (sp30[0][0].field_0.vx + sp30[0][2].field_0.vx) >> 1;
        sp30[0][1].field_0.vy = (sp30[0][0].field_0.vy + sp30[0][2].field_0.vy) >> 1;
        sp30[0][1].field_0.vz = (sp30[0][0].field_0.vz + sp30[0][2].field_0.vz) >> 1;
        sp30[0][1].field_8    = (sp30[0][0].field_8 + sp30[0][2].field_8) >> 1;
        sp30[0][1].field_A    = (sp30[0][0].field_A + sp30[0][2].field_A) >> 1;

        sp30[1][0].field_0.vx = (sp30[0][0].field_0.vx + sp30[2][0].field_0.vx) >> 1;
        sp30[1][0].field_0.vy = (sp30[0][0].field_0.vy + sp30[2][0].field_0.vy) >> 1;
        sp30[1][0].field_0.vz = (sp30[0][0].field_0.vz + sp30[2][0].field_0.vz) >> 1;
        sp30[1][0].field_8    = (sp30[0][0].field_8 + sp30[2][0].field_8) >> 1;
        sp30[1][0].field_A    = (sp30[0][0].field_A + sp30[2][0].field_A) >> 1;

        sp30[1][2].field_0.vx = (sp30[0][2].field_0.vx + sp30[2][2].field_0.vx) >> 1;
        sp30[1][2].field_0.vy = (sp30[0][2].field_0.vy + sp30[2][2].field_0.vy) >> 1;
        sp30[1][2].field_0.vz = (sp30[0][2].field_0.vz + sp30[2][2].field_0.vz) >> 1;
        sp30[1][2].field_8    = (sp30[0][2].field_8 + sp30[2][2].field_8) >> 1;
        sp30[1][2].field_A    = (sp30[0][2].field_A + sp30[2][2].field_A) >> 1;

        sp30[2][1].field_0.vx = (sp30[2][0].field_0.vx + sp30[2][2].field_0.vx) >> 1;
        sp30[2][1].field_0.vy = (sp30[2][0].field_0.vy + sp30[2][2].field_0.vy) >> 1;
        sp30[2][1].field_0.vz = (sp30[2][0].field_0.vz + sp30[2][2].field_0.vz) >> 1;
        sp30[2][1].field_8    = (sp30[2][0].field_8 + sp30[2][2].field_8) >> 1;
        sp30[2][1].field_A    = (sp30[2][0].field_A + sp30[2][2].field_A) >> 1;

        sp30[1][1].field_0.vx = (sp30[1][0].field_0.vx + sp30[1][2].field_0.vx) >> 1;
        sp30[1][1].field_0.vy = (sp30[1][0].field_0.vy + sp30[1][2].field_0.vy) >> 1;
        sp30[1][1].field_0.vz = (sp30[1][0].field_0.vz + sp30[1][2].field_0.vz) >> 1;
        sp30[1][1].field_8    = (sp30[1][0].field_8 + sp30[1][2].field_8) >> 1;
        sp30[1][1].field_A    = (sp30[1][0].field_A + sp30[1][2].field_A) >> 1;

        for (j = 0; j < 2; j++)
        {
            for (k = 0; k < 2; k++)
            {
                temp_a2 = MAX(sp30[j][k].field_0.vz, sp30[j][k + 1].field_0.vz);
                temp_a2 = MAX(temp_a2, sp30[j + 1][k].field_0.vz);
                temp_a2 = MAX(temp_a2, sp30[j + 1][k + 1].field_0.vz) >> 3;

                if (temp_a2 > 0 && temp_a2 < ORDERING_TABLE_SIZE - 1)
                {
                    poly2 = GsOUT_PACKET_P;

                    setPolyFT4(poly2);
                    setRGB0(poly2, arg1, arg1, arg1);

                    *(s32*)&poly2->x0 = *(s32*)&sp30[j][k];
                    *(s32*)&poly2->x1 = *(s32*)&sp30[j][k + 1];
                    *(s32*)&poly2->x2 = *(s32*)&sp30[j + 1][k];
                    *(s32*)&poly2->x3 = *(s32*)&sp30[j + 1][k + 1];

                    setUV4(poly2,
                           sp30[j][k].field_8, sp30[j][k].field_A,
                           sp30[j][k + 1].field_8, sp30[j][k + 1].field_A,
                           sp30[j + 1][k].field_8, sp30[j + 1][k].field_A,
                           sp30[j + 1][k + 1].field_8, sp30[j + 1][k + 1].field_A);

                    poly2->tpage = spA8;
                    poly2->clut  = temp_fp->field_2;
                    AddPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[temp_a2], poly2);
                    GsOUT_PACKET_P = poly2 + 1;
                }
            }
        }
    }
}

void func_800D8FC0(void) // 0x800D8FC0
{
    typedef struct
    {
        SPRT*     sprt_0;
        DR_TPAGE* tpage_4;
        DR_STP*   stp_8;
        s32       activeBufferIdx_C;
    } g_GteScratchData_func_800D8FC0;

    g_GteScratchData_func_800D8FC0* scratchData;
    s32                             i;

    scratchData = PSX_SCRATCH_ADDR(0);

    // Skip.
    if (Pad_ButtonClickedBind(skip) &&
        g_SysWork.sysStateSteps[0] > 0 && g_SysWork.sysStateSteps[0] < 5)
    {
        SysWork_StateStepSet(0, 5);
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_TweenReset(0);
            Event_TweenReset(1);
            Event_TweenReset(2);
            Event_TweenReset(3);
            Event_TweenReset(4);
            Event_TweenReset(5);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 2, Q12(0.0f), false);

            Savegame_EventFlagSet(EventFlag_333);

            func_800D76BC(1);
            Game_TurnFlashlightOn();
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            Event_WaitTimer(Q12(2.2f), false);

            scratchData->activeBufferIdx_C = g_ActiveBufferIdx;
            scratchData->sprt_0            = (SPRT*)GsOUT_PACKET_P;
            for (i = 0; i < 2; i++)
            {
                setCodeWord(scratchData->sprt_0, PRIM_RECT | RECT_BLEND | RECT_TEXTURE, COLOR_RGBC(128, 128, 128, 0));
                setXY0Fast(scratchData->sprt_0, ((i << 8) - 160), -112);
                scratchData->sprt_0->u0 = 0;
                scratchData->sprt_0->v0 = (scratchData->activeBufferIdx_C == 0) ? 32 : 0;
                setWH(scratchData->sprt_0, (i == 0) ? 256 : 64, FRAMEBUFFER_HEIGHT_PROGRESSIVE);
                addPrimFast(&g_OrderingTable2[g_ActiveBufferIdx].org[15], scratchData->sprt_0, 4);

                scratchData->sprt_0++;
                scratchData->tpage_4 = (DR_TPAGE*)scratchData->sprt_0;
                setDrawTPage(scratchData->tpage_4, 0, 0, getTPage(2, 0, ((scratchData->activeBufferIdx_C << 4) + (i << 2)) << 6, (((scratchData->activeBufferIdx_C << 4) >> 4) & 1) << 8));

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

            Event_CameraLookAtSet(NULL, Q12(162.5f), Q12(-1.15f), Q12(50.5f), Q12(0.375f), Q12(0.475f), Q12(0.0374f), Q12(0.0875f), false);
            break;

        case 3:
            Fs_QueueStartSeek(FILE_1ST_NO_BLOCK_TIM);

            // Warp player.
            g_SysWork.playerWork.player.position.vx = Q12(162.77f);
            g_SysWork.playerWork.player.position.vz = Q12(55.43f);
            g_SysWork.playerWork.player.rotation.vy = Q12_ANGLE(180.0f);

            Model_AnimFlagsClear(&g_SysWork.playerWork.player.model, 2);
            func_8008D438();

            // Warp camera.
            Event_CameraPositionSet(NULL, Q12(163.78f), Q12(-2.31f), Q12(58.91f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(NULL, Q12(162.5f), Q12(-1.15f), Q12(50.5f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);

            SysWork_StateStepIncrement(0);
            break;

        case 4:
            if (func_800D76E8())
            {
                SysWork_StateStepIncrement(0);
            }

            // Warp camera.
            Event_CameraPositionSet(NULL,
                                    Event_TweenLinear(Q12(-1.28f), Q12(6.5f), 0) + Q12(163.78f),
                                    Event_TweenLinear(Q12(1.16f), Q12(6.5f), 1) - Q12(2.31f),
                                    Event_TweenLinear(Q12(-4.41f), Q12(6.5f), 2) + Q12(58.91f),
                                    Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), false);

            if (g_SysWork.sysStateSteps[0] != 4)
            {
                SysWork_StateStepReset();
            }
            break;

        case 5:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        default:
            // Return to gameplay.
            SysWork_StateSetNext(SysState_Gameplay);

            Savegame_EventFlagSet(EventFlag_330);

            // Warp player.
            g_SysWork.playerWork.player.position.vx = Q12(162.77f);
            g_SysWork.playerWork.player.position.vz = Q12(55.43f);
            g_SysWork.playerWork.player.rotation.vy = Q12_ANGLE(180.0f);
            break;
    }
}

void func_800D960C(void) // 0x800D960C
{
    // Skip.
    if (Pad_ButtonClickedBind(skip) &&
        g_SysWork.sysStateSteps[0] >= 2 && g_SysWork.sysStateSteps[0] < 4)
    {
        SysWork_StateStepReset();
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            // Warp camera.
            Event_CameraPositionSet(NULL, Q12(159.59f), Q12(-1.25f), Q12(59.49f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(NULL, Q12(161.69f), Q12(-1.53f), Q12(56.1f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);

            func_800D76BC(2);
            D_800DB9E2 = 0;

            Model_AnimFlagsSet(&g_SysWork.playerWork.player.model, 2);

            func_8008D448();
            SysWork_StateStepIncrement(0);

        case 1:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.0f), false);
            break;

        case 2:
            Event_DisplayMapMsgWithAudio(16, &D_800DB9E2, &D_800DB92C);
            break;

        case 3:
            Event_WaitTimer(Q12(0.5f), false);
            break;

        default:
            Savegame_EventFlagSet(EventFlag_332);

            // Return to gameplay.
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            vcReturnPreAutoCamWork(false);
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;
    }
}

void func_800D9824(void) // 0x800D9824
{
    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Screen_FadeTimestepReset();

            SysWork_StateStepIncrement(0);
            break;

        case 1:
            func_800D4000(&g_SysWork.npcs[0]);

            SysWork_StateStepIncrement(0);
            break;

        default:
            Savegame_EventFlagClear(EventFlag_335);

            // Return to gameplay.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            break;
    }
}

void Map_WorldObjectsInit(void) // 0x800D991C
{
    D_800E05E2 = 0;
    D_800E05E0 = 0;
    D_800E05E1 = 0;
    D_800E05A8 = Q12(0.0f);
    D_800E05AC = 0;
    D_800E05AE = 0;

    if (!Savegame_EventFlagGet(EventFlag_324))
    {
        g_MapOverlayHdr.charaUpdateFuncs[Chara_Twinfeeler] = func_800D3694;
        func_800D7408();
        Fs_QueueWaitForEmpty();
    }

    WorldObject_Init(&g_WorldObject_Fence, "FENCE_HI", 130.5f, 0.0f, -93.0f, 0.0f, 0.0f, 0.0f);
    WorldObject_Init(&g_WorldObject_Mal5_21, "MAL5_21_", 117.2464f, -0.685f, 145.896f, -36.0f, 243.0f, 60.21f);

    WorldObject_ModelNameSet(&g_WorldObject_Mal6[0], "MAL6_27_");
    WorldObject_ModelNameSet(&g_WorldObject_Mal6[1], "MAL6_28_");

    Math_Vector3Set(&g_WorldObject_UnkPos, Q12(132.0f), Q12(-0.75f), Q12(140.0f));
    Math_SetSVectorFast(&g_WorldObject_UnkRot, Q12_ANGLE(90.0f), Q12_ANGLE(90.0f), Q12_ANGLE(0.0f));

    WorldObject_Init(&g_WorldObject_SavePad, g_CommonWorldObjectNames[1], 217.15f, -1.25f, -22.9f, 0.0f, -173.0f, 0.0f);

    if (g_SavegamePtr->gameDifficulty == GameDifficulty_Hard)
    {
        D_800C4418.field_0 = Q12(5.0f);
    }
    else
    {
        D_800C4418.field_0 = Q12(3.5f);
    }

    D_800C4418.field_2 = Q12(0.2f);
    D_800C4418.field_4 = Q12(0.1f);
    D_800C4418.field_6 = Q12(0.45f);
    D_800C4418.field_8 = Q12(6.0f);
    D_800C4418.field_A = Q12(0.55f);
    D_800C4418.field_C = 18;

    if (Savegame_EventFlagGet(EventFlag_324) && !Savegame_EventFlagGet(EventFlag_327))
    {
        Savegame_EventFlagSet(EventFlag_335);
        Savegame_EventFlagClear(EventFlag_325);
    }

    WorldObject_ModelNameSet(&g_CommonWorldObjects[0], g_CommonWorldObjectNames[2]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[1], g_CommonWorldObjectNames[3]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[2], g_CommonWorldObjectNames[4]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[3], g_CommonWorldObjectNames[5]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[4], g_CommonWorldObjectNames[6]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[5], g_CommonWorldObjectNames[7]);
}

void Map_WorldObjectsUpdate(void) // 0x800D9BB0
{
    q19_12 var_a0; // Angle?
    q19_12 var;    // Angle?
    MAP_CHUNK_CHECK_VARIABLE_DECL();

    if (PLAYER_IN_MAP_CHUNK(vx, 1, 6, -1, 6) && PLAYER_IN_MAP_CHUNK(vz, 1, -1, 0, 0))
    {
        WorldObjects_Add(&g_WorldObject_SavePad.object, &g_WorldObject_SavePad.position, &g_WorldObject_SavePad.rotation);
    }

    if ((PLAYER_IN_MAP_CHUNK(vx, 1, 4, -1, 4) || PLAYER_IN_MAP_CHUNK(vx, 1, 5, -1, 5)) && PLAYER_IN_MAP_CHUNK(vz, 1, -3, -1, -3))
    {
        if (Savegame_EventFlagGet(EventFlag_320))
        {
            if (D_800E05A8 == Q12(0.0f))
            {
                Sfx_WithFlagsPlay(Sfx_Unk1555, &Q12_VECTOR3(130.5f, 0.0f, -93.0f), Q8(0.5f), SfxFlag_None);

                g_SysWork.playerWork.player.fallSpeed += Q12(1.0f);
                D_800E05AE += Q12(1.0f);
            }

            Collision_TriggerFlagBitsClear(CollisionTriggerFlag_Objects);
            D_800E05A8 += g_DeltaTime;

            if (D_800E05AC) {} // @hack

            var = ratan2(Q12_ANGLE(360.0f), FP_FROM(g_GravitySpeed * Math_Cos(g_WorldObject_Fence.rotation.vx), Q12_SHIFT));
            var_a0 = MAX(Q12(0.0f), Q12(1.0f) - (D_800E05A8 * 2));
            D_800E05AC = Q12_MULT_PRECISE(var_a0, D_800E05AC + var);
            g_WorldObject_Fence.rotation.vx += Q12_MULT_PRECISE(g_DeltaTime, D_800E05AC);

            D_800E05AE = MIN(Q12(15.0f), (D_800E05AE + g_GravitySpeed) + (g_GravitySpeed >> 4));

            g_WorldObject_Fence.position.vy += Q12_MULT_PRECISE(g_DeltaTime, D_800E05AE);
            if (g_WorldObject_Fence.rotation.vx > Q12_ANGLE(90.0f))
            {
                g_WorldObject_Fence.rotation.vx = Q12_ANGLE(90.0f);
            }

            if (D_800E05A8 > Q12(1.0f))
            {
                // NOTE: Changed return type to `s32` in context.
                if (Player_PropertyField10DGet())
                {
                    Savegame_EventFlagSet(EventFlag_322);
                }

                Savegame_EventFlagSet(EventFlag_321);
            }
        }
        else
        {
            Collision_TriggerFlagBitsSet(2);
        }

        WorldObjects_Add(&g_WorldObject_Fence.object, &g_WorldObject_Fence.position, &g_WorldObject_Fence.rotation);
    }

    if ((PLAYER_IN_MAP_CHUNK(vx, 1, 3, -1, 3) || PLAYER_IN_MAP_CHUNK(vx, 1, 4, -1, 4)) && PLAYER_IN_MAP_CHUNK(vz, 1, 4, -1, 4))
    {
        if (!Savegame_EventFlagGet(EventFlag_M4S03_PickupHuntingRifle))
        {
            WorldObjects_Add(&g_WorldObject_Mal5_21.object, &g_WorldObject_Mal5_21.position, &g_WorldObject_Mal5_21.rotation);
        }

        if (!Savegame_EventFlagGet(EventFlag_327))
        {
            Collision_TriggerFlagBitsSet(2);
            WorldObjects_Add(&g_WorldObject_Mal6[0], &g_WorldObject_UnkPos, &g_WorldObject_UnkRot);
        }
        else
        {
            Collision_TriggerFlagBitsClear(CollisionTriggerFlag_Objects);
            WorldObjects_Add(&g_WorldObject_Mal6[1], &g_WorldObject_UnkPos, &g_WorldObject_UnkRot);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, 6, -1, 6) && PLAYER_IN_MAP_CHUNK(vz, 1, -1, 0, 0))
    {
        if (!Savegame_EventFlagGet(EventFlag_M4S03_FirstAidKit))
        {
            WorldObjects_Add(g_CommonWorldObjects, &g_CommonWorldObjectPoses[0].position, &g_CommonWorldObjectPoses[0].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, 6, -1, 6) && PLAYER_IN_MAP_CHUNK(vz, 1, -1, 0, 0))
    {
        if (!Savegame_EventFlagGet(EventFlag_M4S03_RifleShells0))
        {
            WorldObjects_Add(&g_CommonWorldObjects[4], &g_CommonWorldObjectPoses[1].position, &g_CommonWorldObjectPoses[1].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, 6, -1, 6) && PLAYER_IN_MAP_CHUNK(vz, 1, -1, 0, 0))
    {
        if (!Savegame_EventFlagGet(EventFlag_M4S03_RifleShells1))
        {
            WorldObjects_Add(&g_CommonWorldObjects[4], &g_CommonWorldObjectPoses[2].position, &g_CommonWorldObjectPoses[2].rotation);
        }
    }
}

void func_800DA3E0(void) // 0x800DA3E0
{
    MAP_CHUNK_CHECK_VARIABLE_DECL();

    if ((PLAYER_IN_MAP_CHUNK(vx, 1, 4, -1, 4) || PLAYER_IN_MAP_CHUNK(vx, 1, 5, -1, 5)) &&
        PLAYER_IN_MAP_CHUNK(vz, 1, 2, -1, 2))
    {
        func_800D7718();
        if (D_800E05E0 == 0)
        {
            D_800E05E0++;

            if (Savegame_EventFlagGet(EventFlag_332))
            {
                func_800D76BC(3);
            }
        }

        if (Savegame_EventFlagGet(EventFlag_333))
        {
            if (D_800E05E1 == 0)
            {
                SD_Call(Sfx_Unk1563);
                SD_Call(Sfx_Unk1564);
                D_800E05E1++;
            }

            Sfx_WithFalloffAndPitchPlay(Sfx_Unk1563, &Q12_VECTOR3(162.5f, -1.125f, 51.45f), Q8(0.5f), Q12(48.0f), 0);
            Sfx_WithFalloffAndPitchPlay(Sfx_Unk1564, &Q12_VECTOR3(166.421f, -2.0f, 52.578f), Q8(0.5f), Q12(48.0f), 0);
        }
    }
    else
    {
        D_800E05E0 = 0;

        if (D_800E05E1 != 0)
        {
            Sd_SfxStop(Sfx_Unk1563);
            Sd_SfxStop(Sfx_Unk1564);
        }

        D_800E05E1 = 0;
    }

    if ((PLAYER_IN_MAP_CHUNK(vx, 1, 4, -1, 4) || PLAYER_IN_MAP_CHUNK(vx, 1, 5, -1, 5)) &&
        PLAYER_IN_MAP_CHUNK(vz, 1, -3, -1, -3))
    {
        if (vcRetCamMvSmoothF() == 0 && (g_SysWork.playerWork.player.position.vx > Q12(125.8f) || D_800E05E2 == 1))
        {
            if (D_800E05E2 != 0)
            {
                WorldEnv_PresetsSet(6, 3);
            }
            else
            {
                WorldEnv_PresetsSet(6, 20);
            }

            D_800E05E2 ^= 1;
        }
    }
}

void func_800DA718(void) // 0x800DA718
{
    void** addr;

    if (Savegame_EventFlagGet(EventFlag_323) && !Savegame_EventFlagGet(EventFlag_325))
    {
        addr  = &g_MapOverlayHdr.charaUpdateFuncs[Chara_Twinfeeler];
        *addr = Twinfeeler_Update;

        Savegame_EventFlagSet(EventFlag_325);

        Fs_QueueStartReadTim(FILE_TIM_BLOOD_TIM, FS_BUFFER_1, &D_800A908C);
        Fs_QueueStartReadTim(FILE_TEST_WARMTEST_TIM, FS_BUFFER_1, &D_800A9094);
        Fs_QueueWaitForEmpty();
    }
}
