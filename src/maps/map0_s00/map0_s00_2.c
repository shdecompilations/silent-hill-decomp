#include "bodyprog/bodyprog.h"
#include "bodyprog/chara/dms.h"
#include "bodyprog/gfx/map_effects.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player/player.h"
#include "main/rng.h"
#include "maps/map0/map0_s00.h"
#include "maps/common/particle.h"
#include "maps/common/characters/cheryl.h"
#include "inline_no_dmpsx.h"
#include <psyq/gtemac.h>
#include "maps/common/characters/player.h"
#include "maps/common/characters/stalker.h"
#include "maps/common/shared.h"

#include "../src/maps/common/particles/particle.c"

#include "../src/maps/common/characters/player.c"

#include "../src/maps/common/characters/stalker.c" // 0x800D2F08
#include "../src/maps/common/characters/cheryl.c"

#include "../src/maps/common/chara_util.c" // 0x800D88AC

#include "../src/maps/common/map_util.c" // 0x800D929C

void Map_RoomBgmInit(bool arg0) // 0x800D94F8
{
    s32    i;
    q19_12 fadeSpeed;
    s32    bgmFlags;
    u32    saveByte;

    // @hack Not used directly, but gets merged with  `Savegame_EventFlagGet` macros below.
    saveByte = g_SavegamePtr->eventFlags[0];
    fadeSpeed     = Q12(0.1f);
    bgmFlags    = 1 << 8;

    if (g_SysWork.playerWork.player.health > 0 && (!(Savegame_EventFlagGet(EventFlag_23) && !Savegame_EventFlagGet(EventFlag_20))))
    {
        for (i = 1; i < 7; i++)
        {
            if (Savegame_EventFlagGet(D_800DF300[i]))
            {
                bgmFlags |= 1 << i;
            }
        }
    }
    else
    {
        Savegame_EventFlagClear(EventFlag_20);
        fadeSpeed = Q12(0.3f);
    }

    Bgm_LayersUpdate(bgmFlags, fadeSpeed, &D_800DF2F8);
}

void GameBoot_LoadScreen_StageString(void) // 0x800D95D4
{
    Screen_FadeStart(false, true, false);

    Text_Debug_PositionSet(100, 100);
    Text_Debug_Draw("STAGE 0-0");
}

const char* MAP_MESSAGES[] = {
    #include "map_msgs.h"
};

void func_800D9610(void) // 0x800D9610
{
    VECTOR3 sfxPos = {
        MAP_POINTS[g_MapEventData->mapPointIdx].positionX,
        Q12(-1.2f),
        MAP_POINTS[g_MapEventData->mapPointIdx].positionZ
    };

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Sfx_WithFlagsPlay(Sfx_Unk1355, &sfxPos, Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitTimer(Q12(0.1f), false);
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
    }
}

extern q19_12  g_Cutscene_Timer;
extern VECTOR3 g_CameraPositionTarget;
extern VECTOR3 g_CameraLookAtTarget;

void MapEvent_CutsceneOpening(void) // 0x0x800D9748
{
    typedef enum _EventState
    {
        EventState_Initialize = 0,
        EventState_LoadChunks = 1,
        EventState_2          = 2,
        EventState_3          = 3,
        EventState_4          = 4,
        EventState_5          = 5,
        EventState_6          = 6,
        EventState_7          = 7,
        EventState_8          = 8,
        EventState_9          = 9,
        EventState_10         = 10,
        EventState_11         = 11,
        EventState_12         = 12,
        EventState_13         = 13
    } e_EventState;

    bool skip;

    #define playerChara g_SysWork.playerWork.player

    // Skip.
    skip = false;
    if (Pad_ButtonClickedBind(skip) &&
        g_SysWork.sysStateSteps[0] >= EventState_3 &&
        g_SysWork.sysStateSteps[0] <  EventState_13)
    {
        skip = true;
        SysWork_StateStepReset();
    }

    // Control cutscene.
    switch (g_SysWork.sysStateSteps[0])
    {
        case EventState_Initialize:
            Player_ControlFreeze();
            Fs_QueueStartRead(FILE_ANIM_OPEN_DMS, FS_BUFFER_16);

            CutsceneBorder_ForceShow();
            Screen_FadeTimestepReset();

            g_SysWork.sysFlags |= SysFlag_CutsceneActive;

            Sd_SfxPlay(Sfx_Unk1361, Q7(0.0f), Q8(9.0f / 16.0f));
            SysWork_StateStepIncrement(0);

        case EventState_LoadChunks:
            if (Fs_QueueChunksLoad())
            {
                SysWork_StateStepIncrement(0);
            }
            break;

        case EventState_2:
            // Load Cheryl character.
            Dms_HeaderFixOffsets((s_DmsHeader*)FS_BUFFER_16);
            Chara_Load(0, Chara_Cheryl, g_SysWork.npcBoneCoordBuffer, 0, NULL, NULL);

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(3.0f), false);
            g_Cutscene_Timer = Q12(0.0f);

            SysWork_StateStepIncrement(0);
            break;

        case EventState_3:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &playerChara, 53, false);
            SysWork_StateStepIncrement(0);

        case EventState_4:
            g_Cutscene_Timer = MIN(g_Cutscene_Timer + Q12_MULT_PRECISE(g_DeltaTime, Q12(10.0f)), Q12(22.0f));
            if (g_Cutscene_Timer >= Q12(22.0f))
            {
                SysWork_StateStepIncrement(0);
            }
            break;

        case EventState_5:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &playerChara, 74, false);
            SysWork_StateStepIncrement(0);

        case EventState_6:
            g_Cutscene_Timer = MIN(g_Cutscene_Timer + Q12_MULT_PRECISE(g_DeltaTime, Q12(8.0f)), Q12(26.0f));
            if (g_Cutscene_Timer >= Q12(26.0f))
            {
                SysWork_StateStepIncrement(0);
            }
            break;

        case EventState_7:
            g_Cutscene_Timer = MIN((g_Cutscene_Timer + Q12_MULT_PRECISE(g_DeltaTime, Q12(4.0f))), Q12(72.0f));
            Event_DisplayMapMsg(false, MAP_MSG(15), false, false, 0, false); // "Cheryl. Where could you be?"
            break;

        case EventState_8:
            g_Cutscene_Timer = MIN((g_Cutscene_Timer + Q12_MULT_PRECISE(g_DeltaTime, Q12(4.0f))), Q12(72.0f));
            Event_WaitTimer(Q12(0.8f), false);
            break;

        case EventState_9:
            g_Cutscene_Timer = MIN((g_Cutscene_Timer + Q12_MULT_PRECISE(g_DeltaTime, Q12(4.0f))), Q12(72.0f));
            Event_DisplayMapMsg(false, MAP_MSG(16), false, false, 0, false); // "It's strange..."
            break;

        case EventState_10:
            g_Cutscene_Timer = MIN(g_Cutscene_Timer + Q12_MULT_PRECISE(g_DeltaTime, Q12(4.0f)), Q12(72.0f));
            if (g_Cutscene_Timer >= Q12(72.0f))
            {
                SysWork_StateStepIncrement(0);
            }
            break;

        case EventState_11:
            Event_WaitTimer(Q12(1.5f), false);
            break;

        case EventState_12:
            Event_DisplayMapMsg(false, MAP_MSG(19), false, false, 0, false); // "Cheryl."
            break;

        case EventState_13:
            Event_WaitTimer(Q12(1.0f), false);
            break;

        default:
            g_Cutscene_Timer = NO_VALUE;

            if (skip)
            {
                // Restore player control.
                Player_ControlUnfreeze(true);
                SysWork_StateSetNext(SysState_Gameplay);

                if (Screen_IsFadeFinished())
                {
                    Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
                }
            }
            else
            {
                // Restore player control.
                Player_ControlUnfreeze(false);
                SysWork_StateSetNext(SysState_Gameplay);
            }

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            vcReturnPreAutoCamWork(true);

            // Load Cheryl character.
            Chara_ProcessLoads();
            Chara_Spawn(Chara_Cheryl, 0, Q12(-29.5f), Q12(128.7f), Q12(-0.3125f), 1);
            break;
    }

    // Control player and camera.
    if (g_Cutscene_Timer >= Q12(0.0f))
    {
        Dms_CharacterTransformGet(&playerChara.position, &playerChara.rotation, "HERO", g_Cutscene_Timer, (s_DmsHeader*)FS_BUFFER_16);
        vcChangeProjectionValue(Dms_CameraTargetsGet(&g_CameraPositionTarget, &g_CameraLookAtTarget, NULL, g_Cutscene_Timer, (s_DmsHeader*)FS_BUFFER_16));
        vcUserCamTarget(&g_CameraPositionTarget, NULL, true);
        vcUserWatchTarget(&g_CameraLookAtTarget, NULL, true);
    }

    #undef playerChara
}

void MapEvent_CutsceneCherylFootsteps0(void) // 0x800D9D98
{
    Cheryl_DistantFootstepSfxPlay();

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 2, Q12(0.0f), false);
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitTimer(Q12(3.0f), false);
            break;

        case 2:
            Event_WaitPlayerStop();
            break;

        case 3:
            Event_PathWaypointSet(false, 0, 0, Q12_ANGLE(180.0f), Q12(0.0f), Q12(0.0f));
            SysWork_StateStepIncrement(0);

        case 4:
            Event_PathWaypointExecutePlayer(53, 1, false);
            break;

        case 5:
            Event_DisplayMapMsg(false, MAP_MSG(20), 0, 0, 0, false); // "Footsteps?"
            break;

        case 6:
            Event_PathWaypointSet(true, 0, 0, Q12_ANGLE(180.0f),  Q12(-11.0f), Q12(141.0f));
            Event_PathWaypointSet(true, 0, 1, Q12_ANGLE(180.0f),  Q12(-11.5f), Q12(137.0f));
            Event_PathWaypointSet(true, 0, 2, Q12_ANGLE(180.0f),  Q12(-9.0f),  Q12(133.0f));
            Event_PathWaypointSet(true, 0, 3, Q12_ANGLE(-135.0f), Q12(-10.5f), Q12(130.0f));
            Event_PathWaypointSet(true, 0, 4, Q12_ANGLE(-90.0f),  Q12(-14.0f), Q12(129.0f));
            Event_PathWaypointSet(true, 0, 5, Q12_ANGLE(-90.0f),  Q12(-18.0f), Q12(129.5f));
            SysWork_StateStepIncrement(0);

        case 7:
            Event_PathWaypointExecutePlayer(54, 6, false);
            break;

        default:
            SysWork_StateSetNext(SysState_Gameplay);
            break;
    }
}

void MapEvent_CutsceneCherylFootsteps1(void) // 0x800DA028
{
    Cheryl_DistantFootstepSfxPlay();

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 2, Q12(0.0f), false);
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitTimer(Q12(3.0f), false);
            break;

        case 2:
            Event_WaitPlayerStop();
            break;

        case 3:
            Event_PathWaypointSet(false, 0, 0, Q12_ANGLE(-135.0f), Q12(0.0f), Q12(0.0f));
            SysWork_StateStepIncrement(0);

        case 4:
            Event_PathWaypointExecutePlayer(53, 1, false);
            break;

        case 5:
            Event_DisplayMapMsg(false, MAP_MSG(20), 0, 0, 0, false); // "Footsteps?"
            break;

        case 6:
            Event_PathWaypointSet(true, 0, 0, Q12_ANGLE(180.0f), Q12(-6.0f),  Q12(135.0f));
            Event_PathWaypointSet(true, 0, 1, Q12_ANGLE(-90.0f), Q12(-7.0f),  Q12(131.0f));
            Event_PathWaypointSet(true, 0, 2, Q12_ANGLE(-90.0f), Q12(-11.0f), Q12(128.0f));
            Event_PathWaypointSet(true, 0, 3, Q12_ANGLE(-90.0f), Q12(-17.5f), Q12(129.5f));
            SysWork_StateStepIncrement(0);

        case 7:
            Event_PathWaypointExecutePlayer(54, 4, false);
            break;

        default:
            SysWork_StateSetNext(SysState_Gameplay);
    }
}

void MapEvent_CutsceneCherylFootsteps2(void) // 0x800DA254
{
    Cheryl_DistantFootstepSfxPlay();

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 2, Q12(0.0f), false);
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitTimer(Q12(3.0f), false);
            break;

        case 2:
            Event_WaitPlayerStop();
            break;

        case 3:
            Event_PathWaypointSet(false, 0, 0, Q12_ANGLE(-112.5f), Q12(0.0f), Q12(0.0f));
            SysWork_StateStepIncrement(0);

        case 4:
            Event_PathWaypointExecutePlayer(53, 1, false);
            break;

        case 5:
            Event_DisplayMapMsg(false, MAP_MSG(20), 0, 0, 0, false); // "Footsteps?"
            break;

        case 6:
            Event_PathWaypointSet(true, 0, 0, Q12_ANGLE(-135.0f), Q12(5.0f),   Q12(129.0f));
            Event_PathWaypointSet(true, 0, 1, Q12_ANGLE(-90.0f),  Q12(-6.0f),  Q12(128.0f));
            Event_PathWaypointSet(true, 0, 2, Q12_ANGLE(-90.0f),  Q12(-17.5f), Q12(129.5f));
            SysWork_StateStepIncrement(0);

        case 7:
            Event_PathWaypointExecutePlayer(54, 3, false);
            break;

        default:
            SysWork_StateSetNext(SysState_Gameplay);
    }
}

void Cheryl_DistantFootstepSfxPlay(void) // 0x800DA454
{
    VECTOR3 sfxPos;
    q3_12   angleToTarget;
    s32     temp_s0;
    q19_12  sinAngleToTarget;
    s32     temp_v0_5;
    s32     temp_v1;
    s32     var_a3;

    #define playerChara g_SysWork.playerWork.player

    if (Math_Vector2MagCalcSafeQ6(Q12(-29.5f) - playerChara.position.vx,
                            Q12(128.7f) - playerChara.position.vz) >= Q12(16.0f))
    {
        D_800DFAB8 += g_DeltaTime;
        if (D_800DFAB8 > Q12(0.8f))
        {
            angleToTarget = ratan2(Q12(-29.5f) - playerChara.position.vx,
                                   Q12(128.7f) - playerChara.position.vz);
            sinAngleToTarget = Math_Sin(angleToTarget);

            sfxPos.vy = Q12(0.0f);
            sfxPos.vx = playerChara.position.vx + (sinAngleToTarget * 16);
            sfxPos.vz = playerChara.position.vz + (Math_Cos((q19_12)angleToTarget) * 16);
            Sfx_WithPitchPlay(Sfx_CherylFootstep, &sfxPos, Rng_GenerateUInt(75, 106), Rng_GenerateInt(-16, 15));

            D_800DFAB8 = Q12(0.0f);
        }
    }

    #undef playerChara
}

void MapEvent_CutsceneCherylSpotted(void) // 0x800DA5A0
{
    q19_12 prevPlayerPosX;
    q19_12 prevPlayerPosZ;

    #define playerChara g_SysWork.playerWork.player
    #define cherylChara g_SysWork.npcs[0]
    #define cherylProps cherylChara.properties.npc

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            // Warp camera.
            Event_CameraPositionSet(NULL,
                                    Q12(-22.0f), Q12(-2.4f), Q12(130.1f),
                                    Q12(0.0f), Q12(0.0f),
                                    Q12(0.0f), Q12(0.0f),
                                    true);
            Event_CameraLookAtSet(&cherylChara.position,
                                  Q12(0.0f), Q12(-1.0f), Q12(0.0f),
                                  Q12(0.0f), Q12(0.0f),
                                  Q12(0.0f), Q12(0.0f),
                                  true);
            break;

        case 1:
        case 2:
            Event_TweenReset(0);
            Event_TweenReset(1);
            Event_TweenReset(2);
            break;

        case 4:
        case 5:
        case 6:
            // Move camera.
            Event_CameraPositionSet(NULL,
                               Q12(-22.0f) - Event_TweenLinear(Q12(4.0f),  Q12(5.0f), 0),
                               Q12(-2.4f)  - Event_TweenLinear(Q12(-1.5f), Q12(5.0f), 1),
                               Q12(130.1f) - Event_TweenLinear(Q12(4.0f),  Q12(5.0f), 2),
                               Q12(0.0f), Q12(0.0f),
                               Q12(0.0f), Q12(0.0f),
                               false);
            Event_CameraLookAtSet(&cherylChara.position,
                             Q12(0.0f), Q12(-1.0f), Q12(0.0f),
                             Q12(0.0f), Q12(0.0f),
                             Q12(0.0f), Q12(0.0f),
                             false);
            break;

        case 3:
            // Warp camera.
            Event_CameraLookAtSet(&cherylChara.position,
                             Q12(0.0f), Q12(-1.0f), Q12(0.0f),
                             Q12(0.0f), Q12(0.0f),
                             Q12(0.0f), Q12(0.0f),
                             false);
            break;

        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        default:
            vcReturnPreAutoCamWork(D_800DFAC2);
            D_800DFAC2 = 0;
            break;
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Chara_MovementReset(&cherylChara);
            Event_PathWaypointSet(true, 1, 0, Q12_ANGLE(-135.0f), Q12(-35.0f), Q12(120.0f));

            prevPlayerPosX = playerChara.position.vx;
            prevPlayerPosZ = playerChara.position.vz;

            g_Cutscene_MapMsgAudioIdx = 0;

            // Temporarily warp player to initialize required chunks.
            playerChara.position.vx = Q12(-40.0f);
            playerChara.position.vz = Q12(127.0f);
            WorldGfx_CloseRangeChunksInit();
            playerChara.position.vx = prevPlayerPosX;
            playerChara.position.vz = prevPlayerPosZ;

            SysWork_StateStepIncrement(0);
            break;

        case 1:
            Particle_SystemUpdate(0, g_SavegamePtr->mapIdx, 0);
            SysWork_StateStepIncrement(0);

        case 2:
            Event_DisplayMapMsgWithAudio(MAP_MSG(21), &g_Cutscene_MapMsgAudioIdx, g_Cutscene_MapMsgAudioCmds); // "Cheryl? Is that Cheryl!?"
            break;

        case 3:
            cherylProps.moveDistance_124 = Q12(1.8f);

            Event_PathWaypointExecuteChara(&cherylChara, 2, 1, 0);
            Event_WaitTimer(Q12(1.5f), false);
            break;

        case 4:
            cherylProps.moveDistance_124 = Q12(1.8f);
            Event_PathWaypointExecuteChara(&cherylChara, 2, 1, 0);
            break;

        case 5:
            playerChara.position.vx = Q12(-26.0f);
            playerChara.position.vz = Q12(127.0f);
            playerChara.rotation.vy = Q12_ANGLE(-135.0f);

            Event_PathWaypointSet(true, 0, 0, Q12_ANGLE(-135.0f), Q12(-29.0f), Q12(125.0f));
            SysWork_StateStepIncrement(0);

        case 6:
            Event_PathWaypointExecutePlayer(54, 1, false);
            break;

        case 7:
            if (WorldGfx_ChunksInitCheck())
            {
                SysWork_StateStepIncrement(0);
            }
            break;

        case 8:
            Event_DisplayMapMsgWithAudio(MAP_MSG(22), &g_Cutscene_MapMsgAudioIdx, g_Cutscene_MapMsgAudioCmds); // "Where are you going?"
            break;

        case 9:
            Event_PathWaypointSet(true, 1, 0, Q12_ANGLE(-112.5f), Q12(-36.0f), Q12(119.5f));
            SysWork_StateStepIncrement(0);

        case 10:
            cherylProps.moveDistance_124 = Q12(0.55f);
            Event_PathWaypointExecuteChara(&cherylChara, 1, 1, 0);
            break;

        case 11:
            Event_DisplayMapMsgWithAudio(MAP_MSG(23), &g_Cutscene_MapMsgAudioIdx, g_Cutscene_MapMsgAudioCmds); // "Hey wait...stop!"
            break;

        default:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            Savegame_EventFlagSet(EventFlag_4);

            // Restore player control.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);

            Chara_AnimReset(&cherylChara, false);
            break;
    }

    #undef playerChara
    #undef cherylChara
    #undef cherylProps
}

void MapEvent_CutsceneCherylRedirect0(void) // 0x800DAA68
{
    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_PathWaypointSet(false, 0, 0, Q12_ANGLE(180.0f), Q12(0.0f), Q12(-1.0f));
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitTimer(Q12(0.3f), false);
            break;

        case 2:
            Event_WaitPlayerStop();
            break;

        case 3:
            Event_DisplayMapMsg(false, MAP_MSG(24), 0, 0, 0, false); // "I'd better follow Cheryl..."
            break;

        case 4:
            Event_PathWaypointExecutePlayer(54, 1, false);
            break;

        default:
            // Restore player control.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
    }
}

void MapEvent_CutsceneCherylRedirect1(void) // 0x800DAB8C
{
    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_PathWaypointSet(false, 0, 0, Q12_ANGLE(90.0f), Q12(1.0f), Q12(0.0f));
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitTimer(Q12(0.3f), false);
            break;

        case 2:
            Event_WaitPlayerStop();
            break;

        case 3:
            Event_DisplayMapMsg(false, MAP_MSG(24), 0, 0, 0, false); // "I'd better follow Cheryl..."
            break;

        case 4:
            Event_PathWaypointExecutePlayer(54, 1, false);
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
    }
}

void MapEvent_CutsceneCherylRedirect2(void) // 0x800DACB0
{
    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_PathWaypointSet(false, 0, 0, Q12_ANGLE(-90.0f), Q12(-1.0f), Q12(0.0f));
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitTimer(Q12(0.3f), false);
            break;

        case 2:
            Event_WaitPlayerStop();
            break;

        case 3:
            Event_DisplayMapMsg(false, MAP_MSG(24), 0, 0, 0, false); // "I'd better follow Cheryl..."
            break;

        case 4:
            Event_PathWaypointExecutePlayer(54, 1, false);
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
    }
}

void MapEvent_CutsceneCherylRedirect3(void) // 0x800DADD4
{
    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_PathWaypointSet(false, 0, 0, Q12(0.375f), Q12(0.7f), Q12(-0.7f));
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitTimer(Q12(0.3f), false);
            break;

        case 2:
            Event_WaitPlayerStop();
            break;

        case 3:
            Event_DisplayMapMsg(false, MAP_MSG(24), 0, 0, 0, false); // "I'd better follow Cheryl..."
            break;

        case 4:
            Event_PathWaypointExecutePlayer(54, 1, false);
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;
    }
}

void MapEvent_CutsceneCherylIntoTheAlley(void) // 0x800DAEFC
{
    q19_12 camPosY;
    q19_12 camPosZ;
    q19_12 playerOffsetZ0;
    q19_12 playerOffsetZ1;

    #define playerChara g_SysWork.playerWork.player
    #define cherylChara g_SysWork.npcs[0]

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            // Freeze player.
            Player_ControlFreeze();

            // Warp Cheryl to starting position in the alley.
            cherylChara.position.vx = Q12(-62.0f);
            cherylChara.rotation.vy = Q12_ANGLE(180.0f);
            cherylChara.position.vz = playerChara.position.vz - Q12(19.0f);

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 2, Q12(0.0f), false);
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            Event_PathWaypointSet(false, 0, 0, Q12_ANGLE(180.0f), Q12(0.0f), Q12(0.0f));

            // Warp camera.
            Event_CameraPositionSet(&playerChara.position,
                                    Q12(0.24f), Q12(-4.06f), Q12(-5.0f),
                                    Q12(0.0f), Q12(0.0f),
                                    Q12(0.0f), Q12(0.0f),
                                    true);
            Event_CameraLookAtSet(&playerChara.position,
                                  Q12(-0.72f), Q12(-2.11f), Q12(-1.63f),
                                  Q12(0.0f), Q12(0.0f),
                                  Q12(0.0f), Q12(0.0f),
                                  true);

            SysWork_StateStepIncrement(0);

        case 3:
            Event_PathWaypointExecutePlayer(53, 1, false);
            break;

        case 4:
            Event_PathWaypointSet(true, 0, 0,
                         Q12_ANGLE(180.0f),
                         Q12(-62.0f), playerChara.position.vz - Q12(10.0f));
            Event_WaitTimer(Q12(1.0f), false);
            break;

        case 5:
            Event_PathWaypointExecutePlayer(54, 1, false);
            Event_WaitTimer(Q12(1.0f), false);
            break;

        case 6:
            Event_PathWaypointExecutePlayer(54, 1, false);

            camPosY        = Q12_MULT(Math_Cos(playerChara.rotation.vy), Q12(0.6f)) - Q12(1.84f);
            playerOffsetZ0 = Q12_MULT(Math_Cos(playerChara.rotation.vy), Q12(2.0f)) + Q12(4.0f);

            if ((playerChara.position.vz + playerOffsetZ0) < Q12(130.0f))
            {
                playerOffsetZ1 = Q12_MULT(Math_Cos(playerChara.rotation.vy), Q12(2.0f)) + Q12(4.0f);
                camPosZ        = playerChara.position.vz + playerOffsetZ1;
            }
            else
            {
                camPosZ = Q12(130.0f);
            }

            // Warp camera.
            Event_CameraPositionSet(NULL,
                                    Q12(-62.0f), camPosY, camPosZ,
                                    Q12(0.0f), Q12(0.0f),
                                    Q12(0.0f), Q12(0.0f),
                                    g_WarpCamera0);
            Event_CameraLookAtSet(NULL,
                                  playerChara.position.vx, Q12(-0.7f), playerChara.position.vz - Q12(5.0f),
                                  Q12(0.0f), Q12(0.0f),
                                  Q12(0.0f), Q12(0.0f),
                                  g_WarpCamera0);

            g_WarpCamera0 = false;
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);

            Savegame_EventFlagSet(EventFlag_7);
            Savegame_EventFlagSet(EventFlag_8);

            Event_PathWaypointSet(true, 1, 0, Q12_ANGLE(180.0f), Q12(-62.0f), Q12(49.0f));
    }

    #undef playerChara
    #undef cherylChara
}

void func_800DB26C(void) // 0x800DB26C
{
    #define playerChara g_SysWork.playerWork.player
    #define cherylChara g_SysWork.npcs[0]

    // Warp camera.
    if (g_SysWork.sysStateSteps[0] == 0)
    {
        Event_CameraPositionSet(NULL,
                                Q12(-62.0f), Q12(-2.24f), Q12(117.0f),
                                Q12(0.0f), Q12(0.0f),
                                Q12(0.0f), Q12(0.0f),
                                true);
        Event_CameraLookAtSet(NULL,
                              Q12(-62.0f), Q12(-0.7f), Q12(104.0f),
                              Q12(0.0f), Q12(0.0f),
                              Q12(0.0f), Q12(0.0f),
                              true);

        Event_TweenReset(2);
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 2, Q12(0.0f), false);
            Event_PathWaypointSet(true, 0, 0, Q12_ANGLE(-90.0f), Q12(-60.5f), playerChara.position.vz);

            // Warp Cheryl.
            cherylChara.position.vx = Q12(-62.0f);
            cherylChara.rotation.vy = Q12_ANGLE(180.0f);
            cherylChara.position.vz = playerChara.position.vz - Q12(9.0f);

            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            Event_PathWaypointExecutePlayer(54, 1, false);
            break;

        case 3:
            g_DeltaTime = DIV_FAST(g_DeltaTime, 2);

            Event_PathWaypointSet(false, 0, 0, Q12_ANGLE(-135.0f), Q12(0.0f), Q12(0.0f));
            Event_WaitTimer(Q12(0.8f), false);
            break;

        case 4:
            g_DeltaTime = DIV_FAST(g_DeltaTime, 2);

            Savegame_EventFlagSet(EventFlag_8);
            Event_PathWaypointExecutePlayer(54, 1, false);
            break;

        case 5:
            g_DeltaTime = DIV_FAST(g_DeltaTime, 2);
            Event_WaitTimer(Q12(1.0f), false);
            break;

        default:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            Savegame_EventFlagSet(EventFlag_7);

            Event_PathWaypointSet(true, 1, 0, Q12_ANGLE(180.0f), Q12(-62.0f), Q12(49.0f));

            // Return to gameplay.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;
    }

    #undef playerChara
    #undef cherylChara
}

void MapEven_CutsceneAlleyGetsDarker(void) // 0x800DB514
{
    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();

            D_800DFB5C = Q12(0.0f);
            D_800DFB60 = 0;

            GameFs_FlameGfxLoad();
            SysWork_StateStepIncrement(0);

        case 1:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 2, Q12(0.0f), false);
            break;

        case 2:
            Event_WaitPlayerStop();
            break;

        case 3:
            SysWork_StateStepIncrement(0);

        case 4:
            Event_DisplayMapMsg(false, MAP_MSG(25), 0, 0, 0, false); // "That's strange, it's getting darker."
            break;

        case 5:
            Event_SysStateBranchOnFlag(EventFlag_16, 6, 5, false);
            break;

        case 6:
            if (D_800DFB60)
            {
                SysWork_StateStepIncrement(0);
            }
            break;

            if (Fs_QueueChunksLoad())
            {
                D_800DFB60++;
                Chara_Load(1, Chara_GreyChildOrMumbler, &g_SysWork.npcBoneCoordBuffer[0], 0, NULL, NULL);
            }

        case 7:
            Player_CutsceneWeaponUnequip();
            SysWork_StateStepIncrement(0);

        case 8:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AwaitAnimEnd, &g_SysWork.playerWork.player, 0, false);
            break;

        case 9:
            WorldEnv_TransitionStart(10, 10, PrimitiveType_S32, &D_800DFB5C, 0, Q12(0.5f));
            SysWork_StateStepIncrement(0);

        case 10:
            D_800DFB5C += g_DeltaTime;

            if (D_800DFB5C > Q12(0.5f))
            {
                SysWork_StateStepIncrement(0);
            }
            break;

        case 11:
            Savegame_EventFlagSet(EventFlag_17);
            Event_DisplayMapMsg(false, MAP_MSG(30), 0, 0, 0, false); // "......better than nothing, I guess."
            break;

        default:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            Player_ControlUnfreeze(true);
            SysWork_StateSetNext(SysState_Gameplay);

            Chara_ProcessLoads();
            Chara_Spawn(Chara_GreyChildOrMumbler, 0, Q12(-252.0f), Q12(223.0f), Q12_ANGLE(180.0f), 5);
            Chara_Spawn(Chara_GreyChildOrMumbler, 1, Q12(-254.0f), Q12(221.0f), Q12_ANGLE(135.0f), 5);
            Chara_Spawn(Chara_GreyChildOrMumbler, 2, Q12(-259.0f), Q12(232.0f), Q12_ANGLE(0.0f),   5);
            break;
    }

    if (D_800DFB60 == 0)
    {
        if (Fs_QueueChunksLoad())
        {
            D_800DFB60++;
            Chara_Load(1, Chara_GreyChildOrMumbler, &g_SysWork.npcBoneCoordBuffer[0], 0, NULL, NULL);
        }
    }
}

void func_800DB870(void) // 0x800DB870
{
    typedef enum _EventState
    {
        EventState_Freeze     = 0,
        EventState_1          = 1,
        EventState_MsgDeadEnd = 2,
        EventState_Unfreeze   = 3
    } e_EventState;

    switch (g_SysWork.sysStateSteps[0])
    {
        case EventState_Freeze:
            // Freeze player.
            Player_ControlFreeze();
            SysWork_StateStepIncrement(0);

        case EventState_1:
            Event_WaitPlayerStop();
            break;

        case EventState_MsgDeadEnd:
            Event_DisplayMapMsg(false, MAP_MSG(26), 0, 0, 0, false); // "A dead end? What the hell!?"
            break;

        default: // `EventState_Unfreeze`
            // Restore player control.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;
    }
}

void MapEvent_CutsceneAlleyNightmare(void) // 0x800DB94C
{
    typedef enum _EventState
    {
        EventState_Initialize     = 0,
        EventState_1              = 1,
        EventState_2              = 2,
        EventState_3              = 3,
        EventState_LookAtDeadBody = 4
    } e_EventState;

    MATRIX  neck_lwm;
    SVECTOR offset;
    VECTOR  camPos; // Q19.12
    s32     i;

    #define playerChara g_SysWork.playerWork.player

    // Control player and camera movement.
    switch (g_SysWork.sysStateSteps[0])
    {
        case EventState_3:
            // Warp camera.
            if (D_800DFACC == 0)
            {
                Event_CameraPositionSet(NULL,
                                        Q12(-254.63f), Q12(-0.77f), Q12(219.56f),
                                        Q12(0.0f), Q12(0.0f),
                                        Q12(0.0f), Q12(0.0f),
                                        true);
                Event_CameraLookAtSet(&playerChara.position,
                                      Q12(0.0f), Q12(-0.6f), Q12(0.0f),
                                      Q12(0.0f), Q12(0.0f),
                                      Q12(0.0f), Q12(0.0f),
                                      true);

                D_800DFACC++;
            }

            // Warp camera.
            Event_CameraLookAtSet(&playerChara.position,
                             Q12(0.0f), Q12(-0.6f), Q12(0.0f),
                             Q12(0.0f), Q12(0.0f),
                             Q12(0.0f), Q12(0.039f),
                             false);
            break;

        case EventState_LookAtDeadBody:
            if (D_800DFACC == 1)
            {
                bool warpCam = D_800DFACC;

                // Make player invisible.
                Model_AnimFlagsClear(&playerChara.model, AnimFlag_Visible);

                // Warp camera.
                Event_CameraPositionSet(NULL,
                                        Q12(-250.81f), Q12(-0.32f), Q12(218.59f),
                                        Q12(0.0f), Q12(0.0f),
                                        Q12(0.0f), Q12(0.0f),
                                        warpCam);
                Event_CameraLookAtSet(NULL,
                                      Q12(-247.13f), Q12(-0.56f), Q12(217.04f),
                                      Q12(0.0f), Q12(0.0f),
                                      Q12(0.0f), Q12(0.0f),
                                      warpCam);

                // Warp player.
                Math_Vector3Set(&playerChara.position, Q12(-251.12f), Q12(0.0f), Q12(218.56f));

                D_800DFACC++;
            }
            else
            {
                Math_SetSVectorFast(&offset, 0, -38, 0);

                Vw_CoordHierarchyMatrixCompute(&g_SysWork.playerBoneCoords[HarryBone_Head], &neck_lwm);

                // Load transformation matrix.
                gte_SetRotMatrix(&neck_lwm);
                gte_SetTransMatrix(&neck_lwm);

                // Transform offset into world-space.
                gte_ldv0(&offset);
                gte_rtv0tr();
                gte_stlvnl(&camPos);

                // Extract `camPos` from result.
                camPos.vx = Q8_TO_Q12(camPos.vx);
                camPos.vy = Q8_TO_Q12(camPos.vy);
                camPos.vz = Q8_TO_Q12(camPos.vz);

                // Set camera to look toward handing dead body.
                Event_CameraPositionSet(NULL,
                                        camPos.vx, camPos.vy, camPos.vz,
                                        Q12(0.0f), Q12(3.0f),
                                        Q12(0.0f), Q12(0.0f),
                                        false);
                Event_CameraLookAtSet(NULL,
                                      Q12(-247.43f), Q12(-1.0298f), Q12(217.34f),
                                      Q12(0.0f), Q12(0.0f),
                                      Q12(0.0f), Q12(0.0f),
                                      false);
            }
            break;

        case EventState_Initialize:
        case EventState_1:
        case EventState_2:
            break;

        default:
            // Restore player control.
            Model_AnimFlagsSet(&playerChara.model, AnimFlag_Visible);
            vcReturnPreAutoCamWork(true);
            break;
    }

    // Control audio, messages, and game states.
    switch (g_SysWork.sysStateSteps[0])
    {
        case EventState_Initialize:
            Player_ControlFreeze();

            WorldMap_ChunkSet(FILE_BG_THRF908_IPD, -7, 6);
            Sd_SfxStop(Sfx_Unk1358);
            Sd_SfxPlay(Sfx_Unk1359, Q7(0.0f), Q8(13.0f / 16.0f));
            Event_PathWaypointSet(true, 0, 0, Q12_ANGLE(180.0f), Q12(-252.0f), Q12(219.0f));
            Event_PathWaypointSet(true, 0, 1, Q12_ANGLE(112.5f), Q12(-251.0f), Q12(218.5f));

            D_800DFB61 = 0;

            SysWork_StateStepIncrement(0);

        case EventState_1:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 2, Q12(0.0f), false);
            break;

        case EventState_2:
            Event_WaitPlayerStop();
            break;

        case EventState_3:
            Event_PathWaypointExecutePlayer(53, 2, 0);
            break;

        case EventState_LookAtDeadBody:
            Event_DisplayMapMsgWithAudio(MAP_MSG(27), &D_800DFB61, &D_800DFAC8); // "What is this?"
            break;

        default:
            // Restore player control.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);

            // Make all Grey Children aggressive.
            for (i = 0; i < ARRAY_SIZE(g_SysWork.npcs); i++)
            {
                if (g_SysWork.npcs[i].model.charaId == Chara_GreyChildOrMumbler)
                {
                    g_SysWork.npcs[i].model.controlState = 0;
                    g_SysWork.npcs[i].model.stateStep    = 6;
                }
            }
            break;
    }

    #undef playerChara
}

void func_800DBE00(void) // 0x800DBE00
{
    #define playerChara g_SysWork.playerWork.player

    Sd_SfxStop(Sfx_Unk1359);
    Sd_SfxStop(Sfx_Unk1361);
    Sd_SfxStop(Sfx_Unk1360);

    SD_Call(22);
    func_800892A4(4);

    SysWork_StateSetNext(SysState_Gameplay);
    playerChara.position.vy = Q12(0.0f);

    #undef playerChara
}

void Map_WorldObjectsInit(void) // 0x800DBE68
{
    WorldObject_ModelNameSet(&D_800E3A5C[0], "WHEEL1_H");
    WorldObject_ModelNameSet(&D_800E3A5C[1], "WHEEL2_H");

    Math_Vector3Set(&D_800E3A9C, Q12(-268.32f), Q12(-0.44f), Q12(245.72f));

    Math_SetSVectorFast(&D_800E3AAC, Q12_ANGLE(-43.0f), Q12_ANGLE(-115.0f), Q12_ANGLE(0.0f));

    Savegame_EventFlagClear(EventFlag_188);
}

void Map_WorldObjectsUpdate(void) // 0x800DBF08
{
    VECTOR3 viewPos;
    MAP_CHUNK_CHECK_VARIABLE_DECL();

    vwGetViewPosition(&viewPos);

    if (Savegame_EventFlagGet(EventFlag_4) && !Savegame_EventFlagGet(EventFlag_13))
    {
        if (!Savegame_EventFlagGet(EventFlag_6))
        {
            func_800DC33C();
        }
        else if (!Savegame_EventFlagGet(EventFlag_9))
        {
            func_800DC694();
        }
        else if (!Savegame_EventFlagGet(EventFlag_11))
        {
            func_800DC8D8();
        }

        func_800DCA30();
    }

    if (Savegame_EventFlagGet(EventFlag_13) && !Savegame_EventFlagGet(EventFlag_16))
    {
        func_800DCC54();
    }

    if (Savegame_EventFlagGet(EventFlag_17))
    {
        func_800DD0CC();
    }

    if (!Savegame_EventFlagGet(EventFlag_24))
    {
        if (PLAYER_IN_MAP_CHUNK(vx, 1, -7, -1, -7) &&
            (PLAYER_IN_MAP_CHUNK(vz, 1, 6, -1, 6) || PLAYER_IN_MAP_CHUNK(vz, 1, 7, -1, 7)))
        {
            // Spin wheelchair wheel?
            D_800E3AAC.vz += Q12_MULT_PRECISE(g_DeltaTime, (Q12_ANGLE(-90.0f) - (Rng_Rand16() & 0x1FF)));
            WorldObjects_Add(&D_800E3A5C[0], &D_800E3A9C, &D_800E3AAC);
            WorldObjects_Add(&D_800E3A5C[1], &D_800E3A9C, &D_800E3AAC);

            if (g_SysWork.bgmStatusFlags & BgmStatusFlag_6)
            {
                Savegame_EventFlagClear(EventFlag_188);
            }

            if (!Savegame_EventFlagGet(EventFlag_188))
            {
                Sd_SfxStop(Sfx_Unk1361);
                SD_Call(Sfx_Unk1358);
                Savegame_EventFlagSet(EventFlag_188);
            }

            if (!Savegame_EventFlagGet(EventFlag_24))
            {
                Sfx_WithFalloffAndPitchPlay(Sfx_Unk1358, &D_800E3A9C, Q8(0.25f), Q12(8.0f), 0);
            }
        }
    }
}

void MapEvent_GreyChildrenSpawn(void) // 0x800DC1E8
{
    MAP_CHUNK_CHECK_VARIABLE_DECL();

    if (Savegame_EventFlagGet(EventFlag_17) && g_SysWork.npcs[0].model.charaId == Chara_None)
    {
        if (PLAYER_IN_MAP_CHUNK(vx, 1, -7, -1, -7) && PLAYER_IN_MAP_CHUNK(vz, 1, 7, -1, 7))
        {
            Chara_Spawn(Chara_GreyChildOrMumbler, 0, Q12(-252.0f), Q12(223.0f), Q12_ANGLE(180.0f), 5);
            Chara_Spawn(Chara_GreyChildOrMumbler, 1, Q12(-254.0f), Q12(221.0f), Q12_ANGLE(135.0f), 5);
            Chara_Spawn(Chara_GreyChildOrMumbler, 2, Q12(-259.0f), Q12(232.0f), Q12_ANGLE(0.0f),   5);
        }
    }
}

void func_800DC33C(void) // 0x800DC33C
{
    VECTOR3 camPos;
    s16     temp_a0;
    s16     var_s1;
    q19_12  temp_a0_2;
    s32     temp_v1;
    s32     var_a0;
    s32     var_a2;
    s32     var_v1_2;

    #define playerChara g_SysWork.playerWork.player

    if (!Savegame_EventFlagGet(EventFlag_5))
    {
        Event_PathWaypointSet(true, 1, 0, Q12_ANGLE(180.0f), Q12(-62.0f), Q12(108.0f));
        Chara_MovementReset(&g_SysWork.npcs[0]);

        Savegame_EventFlagSet(EventFlag_5);
    }

    vwGetViewPosition(&camPos);
    if (Math_Vector2MagCalcSafeQ6(g_SysWork.npcs[0].position.vx - camPos.vx,
                            g_SysWork.npcs[0].position.vz - camPos.vz) <= Q12(16.5f) &&
        g_SysWork.npcs[0].position.vx >= Q12(-58.0f))
    {
        if (!(g_SysWork.sysFlags & SysFlag_MenuActive))
        {
            if (!Vw_AabbVisibleInScreenCheck(g_SysWork.npcs[0].position.vx - Q12(1.0f),
                                             g_SysWork.npcs[0].position.vx + Q12(1.0f),
                                             g_SysWork.npcs[0].position.vy - Q12(1.5f),
                                             g_SysWork.npcs[0].position.vy,
                                             g_SysWork.npcs[0].position.vz - Q12(1.0f),
                                             g_SysWork.npcs[0].position.vz + Q12(1.0f)))
            {
block7:
                g_SysWork.npcs[0].position.vz = 1;
                g_SysWork.npcs[0].position.vx = 1;
                g_SysWork.npcs[0].properties.npc.moveDistance_124 = Q12(0.0f);

                Chara_AnimReset(&g_SysWork.npcs[0], false);

                Savegame_EventFlagSet(EventFlag_6);
                return;
            }
        }

        var_s1 = Math_AngleNormalizeSigned(ratan2(g_SysWork.npcs[0].position.vx - playerChara.position.vx,
                                      g_SysWork.npcs[0].position.vz - playerChara.position.vz) -
                               playerChara.headingAngle);

        temp_a0 = Math_AngleNormalizeSigned(ratan2(Q12(-62.0f) - playerChara.position.vx,
                                       Q12(106.0f) - playerChara.position.vz) -
                                playerChara.headingAngle);

        if (ABS(var_s1) > ABS(temp_a0))
        {
            var_s1 = temp_a0;
        }
        var_a0 = var_s1 << 16;

        // TODO: Why shift 16?
        temp_a0_2 = (playerChara.moveSpeed * Math_Cos(var_a0 >> 16) * 16) >> 16;
        if (temp_a0_2 > Q12(1.8f))
        {
            D_800DFAD0 = temp_a0_2 + Q12(0.9f);
        }
        else if (D_800DFAD0 > Q12(1.8f))
        {
            var_a2 = Q12(1.8f);
            temp_v1 = D_800DFAD0 - Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.5f);
            if (temp_v1 >= Q12(1.8f))
            {
                var_a2 = temp_v1;
            }
            D_800DFAD0 = var_a2;
        }
        else
        {
            var_v1_2 = D_800DFAD0 + Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.5f);
            if (var_v1_2 > Q12(1.8f))
            {
                var_v1_2 = Q12(1.8f);
            }
            D_800DFAD0 = var_v1_2;
        }

        g_SysWork.npcs[0].properties.npc.moveDistance_124 = D_800DFAD0;
        Event_PathWaypointExecuteCharaNoWait(&g_SysWork.npcs[0], 2, 1);
        return;
    }

    // TODO: Remove `goto`.
    goto block7;

    #undef playerChara
}

void func_800DC694(void) // 0x800DC694
{
    q19_12 mag;
    s32    temp_a0_2;
    s32    temp_v1;
    s32    var_a2;
    s32    var_v1;

    #define playerChara g_SysWork.playerWork.player

    mag = Math_Vector2MagCalcSafeQ6(g_SysWork.npcs[0].position.vx - playerChara.position.vx,
                              (g_SysWork.npcs[0].position.vz - playerChara.position.vz) - Q12(2.0f));

    if (!Savegame_EventFlagGet(EventFlag_7))
    {
        Chara_MovementReset(&g_SysWork.npcs[0]);
        return;
    }

    if (mag <= Q12(14.5f) && g_SysWork.npcs[0].position.vz >= Q12(49.0f))
    {
        if (!(g_SysWork.sysFlags & SysFlag_MenuActive))
        {
            if (!Vw_AabbVisibleInScreenCheck(g_SysWork.npcs[0].position.vx - Q12(1.0f),
                                             g_SysWork.npcs[0].position.vx + Q12(1.0f),
                                             g_SysWork.npcs[0].position.vy - Q12(1.5f),
                                             g_SysWork.npcs[0].position.vy,
                                             g_SysWork.npcs[0].position.vz - Q12(1.0f),
                                             g_SysWork.npcs[0].position.vz + Q12(1.0f)))
            {
block7:
                g_SysWork.npcs[0].position.vz = 1;
                g_SysWork.npcs[0].position.vx = 1;

                Chara_AnimReset(&g_SysWork.npcs[0], false);

                Savegame_EventFlagSet(EventFlag_9);
                return;
            }
        }

        temp_a0_2 = Q12_MULT(playerChara.moveSpeed, Math_Cos(playerChara.headingAngle + Q12_ANGLE(180.0f)));
        if (temp_a0_2 > Q12(1.8f))
        {
            D_800DFAD4 = temp_a0_2 + Q12(0.6f);
        }
        else if (D_800DFAD4 > Q12(1.8f))
        {
            var_a2 = Q12(1.8f);
            temp_v1 = D_800DFAD4 - Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.5f);
            if (temp_v1 >= Q12(1.8f))
            {
                var_a2 = temp_v1;
            }
            D_800DFAD4 = var_a2;
        }
        else
        {
            var_v1 = D_800DFAD4 + Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.5f);
            if (var_v1 > Q12(1.8f))
            {
                var_v1 = Q12(1.8f);
            }
            D_800DFAD4 = var_v1;
        }

        g_SysWork.npcs[0].properties.npc.moveDistance_124 = D_800DFAD4;

        Event_PathWaypointExecuteCharaNoWait(&g_SysWork.npcs[0], 2, 1);
        return;
    }

// TODO: Remvoe `goto`.
block_7:
    goto block7;

    #undef playerChara
}

void func_800DC8D8(void) // 0x800DC8D8
{
    q19_12 mag;

    #define playerChara g_SysWork.playerWork.player

    mag = Math_Vector2MagCalcSafeQ6(g_SysWork.npcs[0].position.vx - playerChara.position.vx,
                              (g_SysWork.npcs[0].position.vz - playerChara.position.vz) - Q12(2.0f));

    if (!Savegame_EventFlagGet(EventFlag_10))
    {
        g_SysWork.npcs[0].position.vx = Q12(-61.0f);
        g_SysWork.npcs[0].position.vz = Q12(48.0f);
        g_SysWork.npcs[0].rotation.vy = Q12_ANGLE(90.0f);

        if (mag < Q12(14.8f))
        {
            Chara_MovementReset(&g_SysWork.npcs[0]);
            Savegame_EventFlagSet(EventFlag_10);
            Event_PathWaypointSet(true, 1, 0, Q12_ANGLE(90.0f), Q12(-57.0f), Q12(47.0f));
        }
    }
    else
    {
        g_SysWork.npcs[0].properties.npc.moveDistance_124 = Q12(1.8f);

        Event_PathWaypointExecuteCharaNoWait(&g_SysWork.npcs[0], 2, 1);

        if (g_SysWork.npcs[0].position.vx > Q12(-58.0f))
        {
            Savegame_EventFlagSet(EventFlag_11);
            g_SysWork.npcs[0].model.charaId = Chara_None;

            Sfx_WithFlagsPlay(Sfx_Unk1354, &Q12_VECTOR3(-57.0f, -1.0f, 47.0f), Q8(0.5f), SfxFlag_None);
        }
    }

    #undef playerChara
}

void func_800DCA30(void) // 0x800DCA30
{
    q19_12 camPosX;
    q19_12 camPosY;
    q19_12 camPosZ;
    q19_12 playerPosX;
    q19_12 playerPosZ;
    q3_12  playerRotY;
    float  four; // @hack Permuter find.

    #define playerChara g_SysWork.playerWork.player

    playerPosX = playerChara.position.vx;
    playerPosZ = playerChara.position.vz;
    playerRotY = playerChara.rotation.vy;

    if (Savegame_EventFlagGet(EventFlag_8))
    {
        // Warp camera.
        if (playerPosZ < Q12(48.0f) && playerPosX > Q12(-59.0f))
        {
            Event_CameraPositionSet(NULL,
                                    Q12(-60.55f), Q12(-0.51f), Q12(47.63f),
                                    Q12(0.0f), Q12(0.0f),
                                    Q12(0.0f), Q12(0.0f),
                                    g_WarpCamera);
            Event_CameraLookAtSet(NULL,
                                  Q12(-56.91f), Q12(-1.26f), Q12(46.14f),
                                  Q12(0.0f), Q12(0.0f),
                                  Q12(0.0f), Q12(0.0f),
                                  g_WarpCamera);

            g_WarpCamera = false;
            return;
        }

        // Warp camera.
        if (playerPosZ < Q12(100.0f) || (playerPosZ < Q12(121.0f) && playerPosX < Q12(-58.0f)))
        {
            four = 4.0f;

            camPosY  = Q12_MULT(Math_Cos(playerRotY), Q12(0.2f)) - Q12(1.8f);
            camPosZ  = playerPosZ;
            camPosZ += Q12_MULT(Math_Cos(playerRotY), Q12(2.0f)) + Q12(four);
            camPosX  = MAX(MIN(Q12(-61.5f), playerPosX), Q12(-63.5f));

            Event_CameraPositionSet(NULL,
                                    camPosX, camPosY, camPosZ,
                                    Q12(0.0f), Q12(0.0f),
                                    Q12(0.0f), Q12(0.0f),
                                    !g_WarpCamera);
            Event_CameraLookAtSet(NULL,
                                  playerPosX, Q12(-0.7f), playerPosZ - Q12(5.0f),
                                  Q12(0.0f), Q12(0.0f),
                                  Q12(0.0f), Q12(0.0f),
                                  !g_WarpCamera);

            g_WarpCamera = true;
            return;
        }

        vcReturnPreAutoCamWork(g_WarpCamera);
        g_WarpCamera = false;
    }

    #undef playerChara
}

void func_800DCC54(void) // 0x800DCC54
{
    if (D_800DFADC == NO_VALUE)
    {
        WorldEnv_TransitionStart(6, 6, PrimitiveType_S32, &D_800DFADC, Q12(0.0f), Q12(100.0f));
        D_800DFADC = Q12(0.0f);

        Sd_SfxPlay(Sfx_Unk1359, Q7(0.0f), Q8(7.0f / 8.0f));
    }

    if (Savegame_EventFlagGet(EventFlag_14))
    {
        if (!Savegame_EventFlagGet(EventFlag_18))
        {
            sharedFunc_800D0B18_0_s00(6);
            Savegame_EventFlagSet(EventFlag_18);
            Sd_SfxPlay(Sfx_Unk1359, Q7(0.0f), Q8(0.75f));

            D_800DFADC = Q12(60.0f);
        }

        D_800DFADC = func_800DCF38(D_800DFADC);
    }
    else
    {
        D_800DFADC = func_800DCDA8();
    }

    if (D_800DFADC >= Q12(80.0f))
    {
        Savegame_EventFlagSet(EventFlag_15);
    }

    if (D_800DFADC >= Q12(100.0f))
    {
        Savegame_EventFlagSet(EventFlag_16);
    }
}

s32 func_800DCDA8(void) // 0x800DCDA8
{
    s16 sp20;
    s32 i;
    s32 var_s2;
    s16 var_s5;
    s32 var_s4;
    s32 temp_v1_3;

    var_s4 = 0;
    var_s5 = 0;
    var_s2 = 0;

    #define playerChara g_SysWork.playerWork.player

    for (i = 0; i < 5; i++)
    {
        temp_v1_3 = func_8005C478(&sp20,
                                  playerChara.position.vx, playerChara.position.vz,
                                  D_800DFAE0[i].vx, D_800DFAE0[i].vy,
                                  D_800DFAE0[i + 1].vx, D_800DFAE0[i + 1].vy);

        if (i == 0)
        {
            var_s4 = temp_v1_3;
            var_s5 = sp20;
            var_s2 = 0;
        }
        else if (temp_v1_3 < var_s4)
        {
            var_s4 = temp_v1_3;
            var_s5 = sp20;
            var_s2 = i;
        }
    }

    return Q12_MULT_PRECISE(D_800DFAE0[var_s2 + 1].vz - D_800DFAE0[var_s2].vz, var_s5) + D_800DFAE0[var_s2].vz;

    #undef playerChara
}

s32 func_800DCF38(s32 arg0) // 0x800DCF38
{
    s16 sp20;
    s32 var_t0;
    s32 var_v0;

    #define playerChara g_SysWork.playerWork.player

    var_t0 = 0;

    if (g_SysWork.sysState == SysState_EventCallback)
    {
        if (!Savegame_EventFlagGet(EventFlag_15))
        {
            return arg0;
        }
    }
    else
    {
        if (!Savegame_EventFlagGet(EventFlag_15))
        {
            if (playerChara.position.vx < Q12(-200.0f))
            {
                func_8005C478(&sp20,
                              playerChara.position.vx, playerChara.position.vz,
                              D_800DFB28[0].vx, D_800DFB28[0].vy, D_800DFB28[1].vx, D_800DFB28[1].vy);

                var_v0 = Q12_MULT_PRECISE(D_800DFB28[1].vz - D_800DFB28[0].vz, sp20) + Q12(60.0f);
                var_t0 = D_800DFB28[0].vz + var_v0;
            }

            return MAX(var_t0, arg0);
        }
    }

    return Q12_MULT_PRECISE(g_DeltaTime, Q12(9.0f)) + arg0;

    #undef playerChara
}

void func_800DD0CC(void) // 0x800DD0CC
{
    s32 vecs[2];

    if (D_800DFB40 == NO_VALUE)
    {
        WorldEnv_TransitionStart(6, 6, PrimitiveType_S32, &D_800DFB40, 0, Q12(100.0f));
        D_800DFB40 = 0;
    }

    D_800DFB44.field_0 += g_DeltaTime;

    vecs[0] = FP_MULTIPLY(D_800DFB44.field_0, D_800DFB48.field_0, 6);
    if (vecs[0] >= Q12(1.0f))
    {
        D_800DFB44.field_0 = 0;
        D_800DFB48.field_0 = Rng_GenerateUInt(32, 95);
        D_800DFB48.field_1 = Rng_GenerateUInt(32, 63);
        vecs[0]           &= Q12_FRACT(vecs[0]);
    }

    vecs[0] = Q12_MULT(D_800DFB48.field_1, Math_Sin(D_800DFB44.field_2));
    D_800DFB44.field_2 += g_DeltaTime;

    vecs[1] = FP_MULTIPLY(D_800DFB44.field_2, D_800DFB48.field_2, 6);
    if (vecs[1] >= Q12(1.0f))
    {
        D_800DFB44.field_2 = 0;
        D_800DFB48.field_2 = Rng_GenerateUInt(32, 95);
        vecs[1]           &= Q12_FRACT(vecs[1]);
    }

    D_800DFB40 = MAX(Q12(0.0f), Q12_MULT((FP_MULTIPLY(vecs[0], Q12(4.5f), 6) + Q12(2.5f)), Math_Sin(vecs[1])) + Q12(4.0f));
}
