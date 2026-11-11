#include "inline_no_dmpsx.h"

#include <psyq/gtemac.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/chara/dms.h"
#include "bodyprog/world/effects.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player/player.h"
#include "bodyprog/sound/sound_system.h"
#include "main/rng.h"
#include "maps/map4/map4_s05.h"
#include "maps/common/particle.h"
#include "maps/common/characters/floatstinger.h"
#include "maps/common/characters/player.h"

#include "../src/maps/common/particles/acid.c"

#include "../src/maps/common/particles/ufo.c"

#include "../src/maps/common/particles/particle.c"

#include "../src/maps/common/characters/player.c"


// TODO: Move floatstinger to separate split.
#include "../src/maps/common/characters/floatstinger.c" // 0x800D16FC

#include "../src/maps/common/map_util.c" // 0x800D5AFC

#include "maps/common/shared/Map_RoomBgmInit_4_s02.h" // 0x800D5D08

void GameBoot_LoadScreen_StageString(void) {}

#include "maps/common/shared/Event_CutsceneTimerAdvance.h" // 0x800D5FE4

#include "maps/common/shared/MapEvent_DoorJammed.h" // 0x800D607C

#include "maps/common/shared/MapEvent_DoorLocked.h" // 0x800D6110

void func_800D61A4(void) {}

const char* MAP_MESSAGES[] = {
    #include "maps/common/shared/map_msg_common.inc"
#if VERSION_REGION_IS(NTSC)
    /* 15 */ "~J0(1.0)\tWhat? ~E ",
    /* 16 */ "~J0(2.5)\tThat_light!? ~E ",
    /* 17 */ "\tThe_stairs_are_wrecked! ~E ",
    /* 18 */ "~C3\tAlchemilla_Hospital ~E "
#elif VERSION_REGION_IS(NTSCJ)
    /* 15 */ "~J0(1.0)\t何だ？ ~E ",
    /* 16 */ "~J0(2.5)\tあの光は！？ ~E",
    /* 17 */ "\t階段は壊れていて ~N\n\t逃げることはできない！ ~E ",
    /* 18 */ "~C3\tアルケミラ病院 ~E "
#endif
};

const VECTOR3 D_800CABE4 = { Q12(-115.5f), Q12(2.0f), Q12(110.5f) };

void func_800D61AC(void) // 0x800D61AC
{
    if (Pad_ButtonClickedBind(skip) &&
        g_SysWork.sysStateSteps[0] > 0 && g_SysWork.sysStateSteps[0] < 12)
    {
        // Warp player.
        g_SysWork.playerWork.player.position.vx = Q12(-120.0f);
        g_SysWork.playerWork.player.position.vz = Q12(109.0f);
        g_SysWork.playerWork.player.rotation.vy = Q12(0.25f);

        // Warp NPC.
        g_SysWork.npcs[0].position.vx = Q12(-114.5f);
        g_SysWork.npcs[0].position.vy = Q12(2.0f);
        g_SysWork.npcs[0].position.vz = Q12(108.0f);

        vcReturnPreAutoCamWork(true);
        SysWork_StateStepReset();
        Savegame_EventFlagSet(EventFlag_348);
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Game_TurnFlashlightOn();

            // Warp player.
            g_SysWork.playerWork.player.position.vy = Q12(4.5f);
            g_SysWork.playerWork.player.position.vx = Q12(-115.5f);
            g_SysWork.playerWork.player.position.vz = Q12(106.65f);
            g_SysWork.playerWork.player.rotation.vy = Q12_ANGLE(0.0f);
            CutsceneBorder_ForceShow();

            Event_PathWaypointSet(true, 0, 0, 0, Q12(-115.5f), Q12(110.0f));
            Event_PathWaypointSet(true, 0, 1, 0, Q12(-115.6f), Q12(110.1f));
            Event_PathWaypointSet(true, 0, 2, Q12_ANGLE(-90.0f), Q12(-115.8f), Q12(110.5f));
            Event_PathWaypointSet(true, 0, 3, Q12_ANGLE(-90.0f), Q12(-115.9f), Q12(110.6f));
            Event_PathWaypointSet(true, 0, 4, Q12_ANGLE(-90.0f), Q12(-116.08f), Q12(110.7f));

            Event_CameraPositionSet(NULL, Q12(-103.27f), Q12(2.02f), Q12(104.6f), 0, 0, 0, 0, true);
            Event_CameraLookAtSet(NULL, Q12(-107.06f), Q12(2.18f), Q12(105.87f), 0, 0, 0, 0, true);

            SysWork_StateStepIncrement(0);

        case 1:
            Event_PathWaypointExecutePlayer(53, 5, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, 0, false);
            break;

        case 2:
            Event_PathWaypointExecutePlayer(53, 5, false);
            Event_WaitTimer(Q12(1.8f), false);
            break;

        case 3:
            // Warp camera.
            Event_CameraPositionSet(NULL,
                                    g_SysWork.playerWork.player.position.vx + Q12(4.0f), Q12(-1.12f), Q12(113.08f),
                                    Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                    true);
            Event_CameraLookAtSet(&g_SysWork.playerWork.player.position,
                                  Q12(0.0f), Q12(-1.5f), Q12(0.0f),
                                  Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                  true);
            
            SysWork_StateStepIncrement(0);

        case 4:
            // Warp camera.
            Event_CameraPositionSet(NULL,
                                    g_SysWork.playerWork.player.position.vx + Q12(4.0f), Q12(-1.12f), Q12(113.08f),
                                    Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                    true);
            Event_CameraLookAtSet(&g_SysWork.playerWork.player.position,
                                  Q12(0.0f), Q12(-1.5f), Q12(0.0f),
                                  Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                  false);

            Event_PathWaypointExecutePlayer(53, 5, false);
            break;

        case 5:
            Event_WaitTimer(Q12(0.8f), false);
            Event_PathWaypointSet(true, 0, 0, Q12_ANGLE(-112.5f), Q12(-120.0f), Q12(109.0f));
            break;

        case 6:
            // Warp camera.
            Event_CameraPositionSet(NULL,
                                    g_SysWork.playerWork.player.position.vx + Q12(4.0f), Q12(-1.12f), Q12(113.08f),
                                    Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                    true);
            Event_CameraLookAtSet(&g_SysWork.playerWork.player.position,
                                  Q12(0.0f), Q12(-1.5f), Q12(0.0f),
                                  Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                  false);

            Event_PathWaypointExecutePlayer(53, 1, false);
            break;

        case 7:
            Event_WaitTimer(Q12(0.2f), false);
            break;

        case 8:
            Sfx_WithFlagsPlay(Sfx_Unk1574, &D_800CABE4, Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 9:
            Event_CharaAnimPlayToEnd(&g_SysWork.playerWork.player, 109);
            break;

        case 10:
            // Warp player.
            g_SysWork.playerWork.player.rotation.vy = Q12_ANGLE(90.0f);

            // Warp NPC.
            g_SysWork.npcs[0].position.vx     = Q12(-114.5f);
            g_SysWork.npcs[0].position.vy     = Q12(3.0f);
            g_SysWork.npcs[0].position.vz     = Q12(108.0f);
            g_SysWork.npcs[0].model.stateStep = 1;

            Savegame_EventFlagSet(EventFlag_348);

            // Warp camera.
            Event_CameraPositionSet(NULL,
                                    Q12(-122.73f), Q12(-0.2f), Q12(112.85f),
                                    Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                    true);
            Event_CameraLookAtSet(NULL,
                                  Q12(-119.67f), Q12(0.23f), Q12(110.31f),
                                  Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                  true);

            Player_ControlUnfreeze(true);
            Player_ControlFreeze();
            SysWork_StateStepIncrement(0);

        case 11:
            Event_WaitTimer(Q12(1.5f), false);
            break;

        default:
            // Return to gameplay.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            vcReturnPreAutoCamWork(false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, 0, false);

            Savegame_EventFlagSet(EventFlag_349);
            g_SysWork.npcs[0].model.stateStep = 2;

            func_8003A16C();
            break;
    }
}

void func_800D6800(void) // 0x800D6800
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
            CutsceneBorder_ForceShow();

            MAP_POINTS[14].positionX = g_SysWork.playerWork.player.position.vx;
            MAP_POINTS[14].positionZ = g_SysWork.playerWork.player.position.vz;
            // TODO: Likely wrong union field.
            MAP_POINTS[14].triggerParam0 = ((u32)Math_AngleNormalizeSigned(g_SysWork.playerWork.player.rotation.vy) + Q12(1.0f)) >> 4;

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 52, false);

            // Warp camera.
            Event_CameraPositionSet(NULL,
                                    Q12(-118.22f), Q12(1.46f), Q12(105.67f),
                                    Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                    true);
            Event_CameraLookAtSet(NULL,
                                  Q12(-119.97f), Q12(-1.4f), Q12(107.85f),
                                  Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                  true);

            func_8005E70C();

            D_800D7D80 = Q12(0.0f);
            D_800D7D82 = 0;

            SysWork_StateStepIncrement(0);

        case 3:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimLock, &g_SysWork.playerWork.player, 0, false);
            Event_WaitTimer(Q12(3.5f), false);

            // Warp player.
            g_SysWork.playerWork.player.position.vx = Q12(-120.0f);
            g_SysWork.playerWork.player.position.vy = Q12(2.0f);
            g_SysWork.playerWork.player.position.vz = Q12(104.0f);
            g_SysWork.playerWork.player.rotation.vy = 0;

            // Warp NPC.
            g_SysWork.npcs[0].position.vx = Q12(-120.0f);
            g_SysWork.npcs[0].position.vy = Q12(2.0f);
            g_SysWork.npcs[0].position.vz = Q12(108.0f);
            g_SysWork.npcs[0].rotation.vy = Q12_ANGLE(180.0f);
            break;

        case 4:
            Savegame_EventFlagSet(EventFlag_351);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 1, Q12(0.2f), false);
            SysWork_StateStepIncrement(0);

        case 5:
            Event_WaitTimer(Q12(2.0f), false);
            break;

        case 6:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            SysWork_StateStepIncrement(0);

        case 7:
            Event_ScreenFadeCmd(ScreenFadeCmd_Wait, true, 0, Q12(0.0f), false);
            break;

        default:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimUnlock, &g_SysWork.playerWork.player, 0, false);

            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);

            Savegame_EventFlagSet(EventFlag_352);
            break;
    }

    if (D_800D7D80 == 0)
    {
        SD_Call(Sfx_Unk1522);
        D_800D7D82++;
    }

    D_800D7D80 = MIN(D_800D7D80 + ((g_DeltaTime * 2) / 3), Q12(1.0f));
    Sd_SfxAttributesUpdate(Sfx_Unk1522, 0, -(D_800D7D80 >> 4) & 0xFF, 0);

    if (D_800D7D80 == Q12(1.0f) && D_800D7D82 >= 0 && D_800D7D82 < 3)
    {
        D_800D7D80 = 0;
    }
}

void func_800D6BC0(void) // 0x800D6BC0
{
    s32    pitch;
    s32    tmp0;
    s32    tmp1;
    q19_12 cos0;
    q19_12 sin0;

    tmp0  = D_800D7D6C;
    tmp1  = tmp0 >> 1;
    sin0  = Math_Sin(tmp1);
    cos0  = Math_Cos(tmp0 >> 4);
    pitch = ((sin0 * 2) + cos0 + Math_Cos(Math_Cos(tmp1))) >> 9;

    D_800D7D6C += g_DeltaTime;

    if (g_SysWork.sysStateSteps[0] >= 10)
    {
        D_800D7D70 -= Q12_MULT_PRECISE(g_DeltaTime, Q12(0.5f));
        if (D_800D7D70 < 0)
        {
            D_800D7D70 = 0;
        }

        Sd_SfxAttributesUpdate(Sfx_Unk1467, 0, ~(D_800D7D70 >> 4), pitch);
    }
    else if (g_SysWork.sysStateSteps[0] > 0)
    {
        D_800D7D70 += Q12_MULT_PRECISE(g_DeltaTime, Q12(0.25f));
        if (D_800D7D70 > Q12(0.9961f))
        {
            D_800D7D70 = Q12(0.9961f);
        }

        Sd_SfxAttributesUpdate(Sfx_Unk1467, 0, ~(D_800D7D70 >> 4), pitch);
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();

            D_800D7D94 = 0;

            Fs_QueueStartReadTim(FILE_TIM_ENBAN_TIM, FS_BUFFER_1, &D_800D7D64);
            Fs_QueueStartRead(FILE_ANIM_UFO2_DMS, FS_BUFFER_19);

            g_Cutscene_Timer = NO_VALUE;
            Screen_FadeTimestepReset();

            SD_Call(Sfx_Unk1467);
            Sd_SfxAttributesUpdate(Sfx_Unk1467, 0, Q8_CLAMPED(1.0f), 0);

            D_800D7D70 = 0;
            D_800D7D6C = 0;

            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            if (Fs_QueueChunksLoad())
            {
                SysWork_StateStepIncrement(0);
            }
            break;

        case 3:
            g_Cutscene_Timer = Q12(0.0f);

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 3, 0, false);
            Dms_HeaderFixOffsets(FS_BUFFER_19);
            func_8003D03C();
            Player_EmptyWeaponHandSet();
            Game_TurnFlashlightOn();

            D_800D7D88.vx = Q12(155.0f);
            D_800D7D88.vy = Q12(-16.0f);
            D_800D7D88.vz = Q12(28.0f);
            D_800D7D95    = 0;

            SysWork_StateStepIncrement(0);

        case 4:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, 0, false);
            break;

        case 5:
            Event_DisplayMapMsgWithAudio(15, &D_800D7D94, &D_800D7D74);
            break;

        case 6:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(40.0f), Q12(0.0f), Q12(30.0f), true, true);
            sharedFunc_800CE5D4_1_s03(&D_800D7D88, Q12(0.6f), Q12(0.15f), 0);
            break;

        case 7:
            Event_DisplayMapMsg(false, 16, 0, 0, 0, false);
            sharedFunc_800CE5D4_1_s03(&D_800D7D88, Q12(0.6f), Q12(0.15f), 0);
            break;

        case 8:
            Vc_SetLookAtMatFromBoneCoord(&g_SysWork.playerBoneCoords[HarryBone_Head], &SVECTOR3_Zero, &Q12_VECTOR3(154.31f, -15.5f, 24.07f));
            sharedFunc_800CE5D4_1_s03(&D_800D7D88, Q12(1.0f), Q12(0.2f), 1);
            Event_WaitTimer(Q12(2.5f), false);
            break;

        case 9:
            sharedFunc_800CE5D4_1_s03(&D_800D7D88, Q12(0.6f), Q12(0.15f), 0);

            g_SysWork.sysStateStepData[0] += g_DeltaTime;
            if (g_SysWork.sysStateStepData[0] > Q12(1.5f))
            {
                if (D_800D7D95 != 0)
                {
                    D_800D7D88.vy -= Q12_MULT_PRECISE(g_DeltaTime, Q12(3.5f));
                    if (D_800D7D88.vy < Q12(-17.5f))
                    {
                        D_800D7D95 = 0;
                    }
                }
                else
                {
                    D_800D7D88.vy += Q12_MULT_PRECISE(g_DeltaTime, Q12(3.5f));
                    if (D_800D7D88.vy > Q12(-14.5f))
                    {
                        D_800D7D95 = 1;
                    }
                }
            }

            if (g_SysWork.sysStateStepData[0] > Q12(0.5f))
            {
                D_800D7D88.vx -= Q12_MULT_PRECISE(g_DeltaTime, Q12(2.5f));
            }

            if (g_SysWork.sysStateStepData[0] > Q12(6.0f))
            {
                SysWork_StateStepIncrement(0);
            }
            break;

        case 10:
            Event_WaitTimer(Q12(2.5f), false);
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, 0, false);
            vcReturnPreAutoCamWork(true);

            Savegame_EventFlagSet(EventFlag_457);
            Event_InvItemCmd(InvItemCmd_AddItem, InvItemId_ChannelingStone, 1, false);
            func_8003D01C();
            Player_WeaponAttackRestore();

            g_Cutscene_Timer = NO_VALUE;

            break;
    }

    if (g_Cutscene_Timer >= Q12(0.0f))
    {
        Dms_CharacterTransformGet(&g_SysWork.playerWork.player.position, &g_SysWork.playerWork.player.rotation, "HERO", g_Cutscene_Timer, FS_BUFFER_19);
        vcChangeProjectionValue(Dms_CameraTargetsGet(&g_Cutscene_CameraPositionTarget, &g_Cutscene_CameraLookAtTarget, NULL, g_Cutscene_Timer, FS_BUFFER_19));
        vcUserCamTarget(&g_Cutscene_CameraPositionTarget, NULL, true);
        vcUserWatchTarget(&g_Cutscene_CameraLookAtTarget, NULL, true);
    }
}

void Map_WorldObjectsInit(void) // 0x800D723C
{
    D_800DB9B0 = 0;

    WorldObject_PlacementInit(&g_WorldObject_Kidn04, "KIDN04_H", -119.8019f, 5.0f, 107.861f);
    WorldObject_PlacementInit(&g_WorldObject_Kidn05, "KIDN05_H", -114.89f, 5.0f, 108.36f);

    if (g_SavegamePtr->gameDifficulty == GameDifficulty_Hard)
    {
        D_800C4418.field_0 = Q12(5.5f);
    }
    else
    {
        D_800C4418.field_0 = Q12(3.75f);
    }

    D_800C4418.field_2 = Q12(0.2f);
    D_800C4418.field_4 = Q12(0.1f);
    D_800C4418.field_6 = Q12(0.45f);
    D_800C4418.field_8 = Q12(6.0f);
    D_800C4418.field_A = Q12(0.55f);
    D_800C4418.field_C = 18;

    if (Savegame_EventFlagGet(EventFlag_349) && !Savegame_EventFlagGet(EventFlag_350))
    {
        Chara_SpawnFlagsSet(Chara_Floatstinger, 0, CharaSpawnFlag_2);
    }
}

void Map_WorldObjectsUpdate(void) // 0x800D7360
{
    Collision_TriggerFlagBitsClear(CollisionTriggerFlag_All);

    if (PLAYER_IN_MAP_CHUNK_Z(2))
    {
        if (D_800DB9B0 == 0)
        {
            WorldEnv_PresetsSet(6, 12);
            D_800DB9B0++;
        }

        if (Savegame_EventFlagGet(EventFlag_348))
        {
            WorldObjects_Add(&g_WorldObject_Kidn05.object, &g_WorldObject_Kidn05.position, &(SVECTOR3){ Q12_ANGLE(0.0f), Q12_ANGLE(0.0f), Q12_ANGLE(0.0f) });
            Collision_TriggerFlagBitsSet(4);
        }
        else
        {
            WorldObjects_Add(&g_WorldObject_Kidn04.object, &g_WorldObject_Kidn04.position, &(SVECTOR3){ Q12_ANGLE(0.0f), Q12_ANGLE(0.0f), Q12_ANGLE(0.0f) });
            Collision_TriggerFlagBitsSet(2);
        }
    }

    else if (PLAYER_IN_MAP_CHUNK_Z(5))
    {
        if (Savegame_EventFlagGet(EventFlag_346))
        {
            Collision_TriggerFlagBitsSet(CollisionTriggerFlag_Block);

            if (g_SysWork.playerWork.player.position.vz < Q12(212.0f))
            {
                Savegame_EventFlagClear(EventFlag_346);
            }
        }
        else
        {
            Collision_TriggerFlagBitsSet(CollisionTriggerFlag_Objects);
        }
    }
    else
    {
        if (PLAYER_IN_MAP_CHUNK(-3, 4))
        {
            Savegame_EventFlagSet(EventFlag_MapMark_AltCentralTown_KoontzStBigCross);
            Savegame_EventFlagSet(EventFlag_MapMark_AltCentralTown_CrichtonKoontzBigCross);
        }

        Collision_TriggerFlagBitsClear(CollisionTriggerFlag_All);
    }
}

#undef floatstingerProps
