#include "inline_no_dmpsx.h"

#include <psyq/gtemac.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/chara/dms.h"
#include "bodyprog/events/event_data.h"
#include "bodyprog/gfx/map_effects.h"
#include "bodyprog/math/math.h"
#include "bodyprog/sys/memcard.h"
#include "bodyprog/player/player.h"
#include "bodyprog/ranking.h"
#include "bodyprog/sound/sound_system.h"
#include "main/rng.h"
#include "maps/map6/map6_s02.h"
#include "maps/common/particle.h"
#include "maps/common/characters/player.h"

#include "../src/maps/common/unk_draw_m1s03.c"

#include "../src/maps/common/particles/particle.c"

#include "../src/maps/common/characters/player.c"

#include "../src/maps/common/characters/alessa.c"

#include "../src/maps/common/chara_util.c" // 0x800CDEB0

#include "../src/maps/common/map_util.c" // 0x800CE87C

#include "maps/common/shared/Map_RoomBgmInit_6_s00.h" // 0x800CEA88

void GameBoot_LoadScreen_StageString(void) {}

void func_800CED74(s_SubCharacter* chara, bool arg1) // 0x800CED74
{
    VECTOR3 sp10;
    VECTOR3 sp20;
    bool    cond;
    q19_12  height;
    bool    vcPrsFViewFlag;
    bool    vcPrsFViewFlag2;
    bool    vcOldPrsFViewFlag;
    bool    warp;

    if (arg1)
    {
        D_800D3C84 = NO_VALUE;
    }

    if (chara->position.vy > Q12(3.2f))
    {
        cond = false;

        Math_Vector3Set(&sp10, Q12(-19.05f), Q12(1.41f), Q12(-18.46f));
        Math_Vector3Set(&sp20, Q12(-19.85f), Q12(3.46f), Q12(-21.8f));
    }
    else
    {
        cond   = true;
        height = MAX(chara->position.vy - Q12(3.56f), Q12(-9.65f));

        Math_Vector3Set(&sp10, Q12(-20.0f), height, Q12(-20.01f));
        Math_Vector3Set(&sp20, Q12(-20.0f), height + Q12(2.0f), Q12(-19.99f));
    }

    // TODO: These `VC_PRS_F_VIEW_F`/`VC_OLD_PRS_F_VIEW_F` checks have showed up a few times now.
    // With the same odd XOR pattern, and `VC_PRS_F_VIEW_F` check followed by `VC_OLD_PRS_F_VIEW_F`.
    // Inlines have been seen to change condition checks into XOR before, but no luck making an inline for this check yet though.
    vcPrsFViewFlag = (vcWork.flags & VC_PRS_F_VIEW_F) == VC_PRS_F_VIEW_F;
    if (((g_GameWorkConst->config.extraViewCtrl && (vcPrsFViewFlag ^ 1) != 0) ||
         (!g_GameWorkConst->config.extraViewCtrl && vcPrsFViewFlag)) &&
        (g_GameWorkConst->config.extraViewMode != 0))
    {
        vcPrsFViewFlag2 = (vcWork.flags & VC_PRS_F_VIEW_F) == VC_PRS_F_VIEW_F;

        if ((g_GameWorkConst->config.extraViewCtrl && (vcPrsFViewFlag2 ^ 1) != 0) ||
            (!g_GameWorkConst->config.extraViewCtrl && vcPrsFViewFlag2))
        {
            vcOldPrsFViewFlag = (vcWork.flags & VC_OLD_PRS_F_VIEW_F) == VC_OLD_PRS_F_VIEW_F;

            if ((g_GameWorkConst->config.extraViewCtrl && (vcOldPrsFViewFlag ^ 1) == 0) ||
                (!g_GameWorkConst->config.extraViewCtrl && !vcOldPrsFViewFlag))
            {
                vcReturnPreAutoCamWork(true);
            }
            else
            {
                vcReturnPreAutoCamWork(false);
            }
        }
        else
        {
            vcReturnPreAutoCamWork(false);
        }
    }
    else
    {
        warp = (cond != D_800D3C84);
        vcUserCamTarget(&sp10, NULL, warp);
        vcUserWatchTarget(&sp20, NULL, warp);

        if (warp)
        {
            vcExecCamera();
        }
    }

    D_800D3C84 = cond;
}

#include "maps/common/shared/MapEvent_DoorJammed.h" // 0x800CEF88

#include "maps/common/shared/MapEvent_DoorLocked.h" // 0x800CF01C

void func_800CF0B0(void) {}

const char* MAP_MESSAGES[] = {
    #include "maps/common/shared/map_msg_common.h"
#if VERSION_REGION_IS(NTSC)
    /* 15 */ "~J0(1.0)\t...! ~E ",
    /* 16 */ "~J0(1.0)\tWait! ~E ",
    /* 17 */ "~J0(1.7)\tDamn! ~N\n\t\t\tI_was_too_late? ~E ",
    /* 18 */ "~J0(1.5)\tSorry... ",
    /* 19 */ "~J0(0.6)\tAh... ",
    /* 20 */ "~J0(1.5)\t_ ",
    /* 21 */ "~J0(3.6)\tLet_me_ask_you,_have_you_seen ~N\n\t\t\ta_little_girl_around_here? ~E ",
    /* 22 */ "~J0(2.8)\tShort,_black_hair... ~E ",
    /* 23 */ "~J0(1.0)\tWhat? ~E ",
    /* 24 */ "~J0(2.5)\tThat_light!? ~E "
#elif VERSION_REGION_IS(NTSCJ)
    /* 15 */ "~J0(1.0)\tこれは…… ~E ",
    /* 16 */ "~J0(1.0)\t待て！ ~E ",
    /* 17 */ "~J0(1.7)\tくそっ…… ~N\n\t\t\t間に合わなかったか ~E ",
    /* 18 */ "~J0(1.5)\tちょっと失礼 ",
    /* 19 */ "~J0(0.6)\tえーと…… ",
    /* 20 */ "~J0(1.5)\t_ ",
    /* 21 */ "~J0(3.6)\tここらへんで女の子を見なかったかな？ ~E ",
    /* 22 */ "~J0(2.8)\t短い黒髪で…… ~E ",
    /* 23 */ "~J0(1.0)\t何だ？ ~E ",
    /* 24 */ "~J0(2.5)\tあの光は！？ ~E"
#endif
};

void func_800CF0B8(void) // 0x800CF0B8
{
    bool hasSkippedEarly;

    Savegame_EventFlagClear(EventFlag_412);

    hasSkippedEarly = false;
    if ((g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.skip) &&
        g_SysWork.sysStateSteps[0] >= 2 && g_SysWork.sysStateSteps[0] <= 19)
    {
        // Sets flag to `true` if player skipped before step 19.
        hasSkippedEarly = g_SysWork.sysStateSteps[0] < 19;
        SysWork_StateStepSet(0, 20);
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();

            CutsceneBorder_ForceShow();
            Screen_FadeTimestepReset();

            func_8003D03C();
            Player_EmptyWeaponHandSet();

            Savegame_EventFlagClear(EventFlag_405);

            SysWork_StateStepIncrement(0);

            // Warp camera.
            Event_CameraPositionSet(NULL, Q12(57.09f), Q12(-3.23f), Q12(-20.41f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(NULL, Q12(60.07f), Q12(-0.94f), Q12(-19.06f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);

            Savegame_EventFlagSet(EventFlag_412);

            // Warp player.
            g_SysWork.playerWork.player.position.vx = Q12_MULT_FLOAT(Math_Sin(Q12_ANGLE(135.0f)), 0.9f) + Q12(60.0f);
            g_SysWork.playerWork.player.position.vy = Q12(1.64f);
            g_SysWork.playerWork.player.position.vz = Q12_MULT_FLOAT(Math_Cos(Q12_ANGLE(135.0f)), 0.9f) - Q12(20.0f);
            g_SysWork.playerWork.player.rotation.vy = Q12_ANGLE(45.0f);

            D_800D4E09 = 1;
            break;

        case 1:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 53, false);
            Chara_Load(0, Chara_Alessa, &g_SysWork.npcBoneCoordBuffer[0], CHARA_FORCE_FREE_ALL, NULL, NULL);
            SysWork_StateStepIncrement(0);

        case 2:
            g_SysWork.sysStateStepData[0] += g_DeltaTime;

            // Warp player.
            g_SysWork.playerWork.player.position.vx = Q12_MULT_FLOAT(Math_Sin(Q12_ANGLE(135.0f) - (g_SysWork.sysStateStepData[0] >> 2)), 0.9f) + Q12(60.0f);
            g_SysWork.playerWork.player.position.vz = Q12_MULT_FLOAT(Math_Cos(Q12_ANGLE(135.0f) - (g_SysWork.sysStateStepData[0] >> 2)), 0.9f) - Q12(20.0f);
            g_SysWork.playerWork.player.rotation.vy = Q12_ANGLE(45.0f) - (g_SysWork.sysStateStepData[0] >> 2);

            if ((g_SysWork.sysStateStepData[0] >> 2) > Q12_ANGLE(140.7f))
            {
                SysWork_StateStepIncrement(0);
            }
            break;

        case 3:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 52, false);
            SysWork_StateStepIncrement(0);

        case 4:
            Event_DisplayMapMsg(false, 15, 0, 0, 0, false);
            break;

        case 5:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 71, false);
            SysWork_StateStepIncrement(0);

        case 6:
            Event_WaitTimer(Q12(1.5f), false);
            break;

        case 7:
            Chara_ProcessLoads();
            Chara_Spawn(Chara_Alessa, 0, Q12(60.0f), Q12(-23.0f), 0, 3);
            SysWork_StateStepIncrement(0);
            break;

        case 8:
            // Warp player.
            Event_CameraPositionSet(NULL, Q12(56.79f), Q12(-7.9f), Q12(-27.3f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(NULL, Q12(57.96f), Q12(-5.03f), Q12(-24.77f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);

            // Warp player.
            g_SysWork.playerWork.player.position.vx = Q12(59.6f);
            g_SysWork.playerWork.player.position.vz = Q12(-19.0f);
            g_SysWork.playerWork.player.rotation.vy = Q12_ANGLE(180.0f);

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 51, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 1, false);

            Savegame_EventFlagSet(EventFlag_412);
            D_800D4E09 = 0;
            SysWork_StateStepIncrement(0);

        case 9:
            Event_WaitTimer(Q12(2.0f), false);
            break;

        case 10:
            // Warp camera.
            Event_CameraPositionSet(NULL, Q12(57.42f), Q12(-2.48f), Q12(-16.58f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(NULL, Q12(59.23f), Q12(-1.24f), Q12(-19.93f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);

            // Warp player.
            g_SysWork.playerWork.player.position.vx = Q12(59.0f);
            g_SysWork.playerWork.player.position.vz = Q12(-19.5f);
            g_SysWork.playerWork.player.rotation.vy = ratan2(g_SysWork.npcs[0].position.vx - g_SysWork.playerWork.player.position.vx,
                                                                g_SysWork.npcs[0].position.vz - g_SysWork.playerWork.player.position.vz);

            WorldGfx_CharaModelTransparentSet(Chara_Alessa, true);
            Savegame_EventFlagSet(EventFlag_412);
            SysWork_StateStepIncrement(0);

        case 11:
            Event_WaitTimer(Q12(0.5f), false);
            break;

        case 12:
            g_SysWork.npcs[0].timer_C6 += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.25f);
            Event_WaitTimer(Q12(0.3f), false);
            Event_DisplayMapMsgWithAudio(16, &D_800D4E08, &D_800D3B40);
            break;

        case 13:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 111, false);
            SysWork_StateStepIncrement(0);

        case 14:
            Event_DisplayMapMsgWithAudio(16, &D_800D4E08, &D_800D3B40);

            g_SysWork.npcs[0].timer_C6 += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.25f);
            if (g_SysWork.npcs[0].timer_C6 > Q12(1.0f))
            {
                g_SysWork.npcs[0].timer_C6 = Q12(1.0f);
            }
            break;

        case 15:
            g_SysWork.npcs[0].timer_C6 += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.25f);
            if (g_SysWork.npcs[0].timer_C6 > Q12(1.0f))
            {
                g_SysWork.npcs[0].timer_C6 = Q12(1.0f);
                SysWork_StateStepIncrement(0);
            }
            break;

        case 16:
            Chara_ModelCharaIdClear(&g_SysWork.npcs[0], 0, 0);
            SysWork_StateStepIncrement(0);

        case 17:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AwaitAnimEnd, &g_SysWork.playerWork.player, 0, false);
            break;

        case 18:
            // Warp camera.
            Event_CameraPositionSet(NULL, Q12(64.87f), Q12(-5.4f), Q12(-14.82f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(NULL, Q12(62.29f), Q12(-3.36f), Q12(-17.09f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 51, false);
            func_8003D01C();
            Player_WeaponAttackRestore();

            Savegame_EventFlagSet(EventFlag_412);
            SysWork_StateStepIncrement(0);

        case 19:
            Event_DisplayMapMsgWithAudio(17, &D_800D4E08, &D_800D3B40);

            if (g_SysWork.sysStateSteps[0] != 19)
            {
                SysWork_StateStepReset();
            }
            break;

        case 20:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 21:
            Fs_QueueWaitForEmpty();
            vcReturnPreAutoCamWork(true);

            if (!hasSkippedEarly)
            {
                func_8003D01C();
                Player_WeaponAttackRestore();
            }

            // Warp player.
            g_SysWork.playerWork.player.position.vx = Q12(59.0f);
            g_SysWork.playerWork.player.position.vz = Q12(-19.5f);
            g_SysWork.playerWork.player.rotation.vy = Q12_ANGLE(164.1f);

            Chara_ModelCharaIdClear(&g_SysWork.npcs[0], 0, 0);
            Player_ControlUnfreeze(true);

            SysWork_StateSetNext(SysState_Gameplay);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            SysWork_StateStepIncrement(0);

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);

            Savegame_EventFlagSet(EventFlag_410);
            Savegame_EventFlagClear(EventFlag_407);
            Savegame_EventFlagSet(EventFlag_MapMark_ResortTown_LighthouseCheck);

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            vcReturnPreAutoCamWork(false);
            D_800D4E09 = 0;
            break;
    }
}

#include "maps/common/shared/Event_CutsceneTimerAdvance.h" // 0x800CFB9C

void func_800CFC34(void) // 0x800CFC34
{
    s32 i;
    s32 curTimerVal;
    s32 pitch;
    s32 pitchDiv512;

    curTimerVal = D_800D3C90;
    pitch       = (Math_Sin(curTimerVal >> 3) * 2) + Math_Cos(curTimerVal >> 4) + Math_Sin(Math_Cos(curTimerVal >> 4));

    D_800D3C90 += g_DeltaTime;
    D_800D3C90 += g_DeltaTime;

    pitchDiv512 = pitch >> 9; // `/ 512`

    if (g_SysWork.sysStateSteps[0] == 11 || g_SysWork.sysStateSteps[0] == 13)
    {
        D_800D3C94 -= Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.5f);
        if (D_800D3C94 < 0)
        {
            D_800D3C94 = 0;
        }
        Sd_SfxAttributesUpdate(Sfx_Unk1467, 0, ~(D_800D3C94 >> 4), pitchDiv512);
    }
    else if (g_SysWork.sysStateSteps[0] > 0)
    {
        D_800D3C94 += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.25f);
        if (D_800D3C94 > 4080)
        {
            D_800D3C94 = 4080;
        }
        Sd_SfxAttributesUpdate(Sfx_Unk1467, 0, ~(D_800D3C94 >> 4), pitchDiv512);
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Sd_SfxStop(Sfx_Unk1611);

            if (Savegame_EventFlagGet(EventFlag_456) && Savegame_EventFlagGet(EventFlag_457) &&
                Savegame_EventFlagGet(EventFlag_458) && Savegame_EventFlagGet(EventFlag_459))
            {
                Savegame_EventFlagSet(EventFlag_469);
            }

            D_800D3C88 = 0;
            Fs_QueueStartReadTim(FILE_TIM_ENBAN_TIM, FS_BUFFER_1, &D_800D3B70);
            Fs_QueueStartRead(FILE_ANIM_UFO5_DMS, FS_BUFFER_13);

            g_Cutscene_Timer = NO_VALUE;

            Screen_FadeTimestepReset();

            SD_Call(Sfx_Unk1467);
            Sd_SfxAttributesUpdate(Sfx_Unk1467, 0, 255, 0);

            D_800D3C94 = 0;
            D_800D3C90 = 0;
            Savegame_EventFlagSet(EventFlag_417);

            D_800D3C8C = 0;
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

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 3, Q12(0.0f), false);
            Dms_HeaderFixOffsets(FS_BUFFER_13);
            func_8003D03C();
            Player_EmptyWeaponHandSet();
            Game_TurnFlashlightOn();
            SysWork_StateStepIncrement(0);

        case 4:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.0f), false);

        case 5:
            Event_DisplayMapMsgWithAudio(23, &D_800D3C88, &D_800D3B6C);
            break;

        case 6:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(40.0f), Q12(0.0f), Q12(30.0f), true, true);

            for (i = 0; i < 15; i++)
            {
                sharedFunc_800CE5D4_1_s03(&D_800D3B78[i], Q12(0.5f), Q12(0.05f), 0);
            }
            break;

        case 7:
            if (Savegame_EventFlagGet(EventFlag_469))
            {
                Fs_QueueStartReadTim(FILE_TIM_UFO1_TIM, FS_BUFFER_1, &D_800D3B44[0]);
                Fs_QueueStartReadTim(FILE_TIM_UFO6_TIM, FS_BUFFER_1, &D_800D3B44[1]);
            }
            SysWork_StateStepIncrement(0);

        case 8:
            Event_DisplayMapMsg(false, 24, 0, 0, 0, false);
            for (i = 0; i < 15; i++)
            {
                sharedFunc_800CE5D4_1_s03(&D_800D3B78[i], Q12(0.5f), Q12(0.05f), 0);
            }
            break;

        case 9:
            Vc_SetLookAtMatFromBoneCoord(&g_SysWork.playerBoneCoords[HarryBone_Head], &SVECTOR3_Zero, &Q12_VECTOR3(5.39f, -19.0f, -16.34f));

            for (i = 0; i < 15; i++)
            {
                sharedFunc_800CE5D4_1_s03(&D_800D3B78[i], Q12(0.5f), Q12(0.15f), 0);
            }

            Event_WaitTimer(Q12(2.5f), false);

            if (g_SysWork.sysStateSteps[0] != 9)
            {
                Savegame_EventFlagSet(EventFlag_412);
            }
            break;

        case 10:
            if (D_800D3C8C > Q12(8.0f))
            {
                if (Savegame_EventFlagGet(EventFlag_469))
                {
                    SysWork_StateStepSet(0, 12);
                }
                else
                {
                    SysWork_StateStepSet(0, 11);
                }
            }
            break;

        case 11:
            Event_WaitTimer(Q12(2.5f), false);

            if (g_SysWork.sysStateSteps[0] != 11)
            {
                SysWork_StateStepReset();
            }
            break;

        case 12:
            Event_WaitTimer(Q12(4.0f), false);

            if (g_SysWork.sysStateSteps[0] != 12)
            {
                Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            }
            break;

        case 13:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 1, Q12(1.0f), false);
            break;

        default:
            vcReturnPreAutoCamWork(true);
            Savegame_EventFlagSet(EventFlag_460);
            Event_InvItemCmd(InvItemCmd_AddItem, InvItemId_ChannelingStone, 1, false);
            func_8003D01C();
            Player_WeaponAttackRestore();

            g_Cutscene_Timer = NO_VALUE;

            if (Savegame_EventFlagGet(EventFlag_469))
            {
                SysWork_StateSetNext(SysState_Gameplay);
                D_800C4414 &= ~(1 << 5);
            }
            else
            {
                Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
                Player_ControlUnfreeze(false);

                SysWork_StateSetNext(SysState_Gameplay);

                SD_Call(Sfx_Unk1611);
                Sd_SfxStop(Sfx_Unk1467);
            }
            break;
    }

    if (g_SysWork.sysStateSteps[0] == 10 ||
        g_SysWork.sysStateSteps[0] == 12 ||
        g_SysWork.sysStateSteps[0] == 13)
    {
        D_800D3C8C += g_DeltaTime;

        for (i = 0; i < 15; i++)
        {
            if (Savegame_EventFlagGet(EventFlag_469) || D_800D3C8C < D_800D3C2C[i])
            {
                sharedFunc_800CE5D4_1_s03(&D_800D3B78[i], Q12(0.5f), Q12(0.05f), 0);

                if (D_800D3C8C > Q12(1.5f))
                {
                    D_800D3B78[i].vx += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 3.5f);
                    D_800D3B78[i].vy += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 1.2f);
                    D_800D3B78[i].vz += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, -0.2f);
                }
            }
        }
    }

    if (g_Cutscene_Timer >= Q12(0.0f))
    {
        // TODO: Remove null chars from "HERO" once later rodata is added.
        Dms_CharacterTransformGet(&g_SysWork.playerWork.player.position, &g_SysWork.playerWork.player.rotation, "HERO", g_Cutscene_Timer, FS_BUFFER_13);
        vcChangeProjectionValue(Dms_CameraTargetsGet(&g_Cutscene_CameraPositionTarget, &g_Cutscene_CameraLookAtTarget, NULL, g_Cutscene_Timer, FS_BUFFER_13));
        vcUserCamTarget(&g_Cutscene_CameraPositionTarget, NULL, true);
        vcUserWatchTarget(&g_Cutscene_CameraLookAtTarget, NULL, true);
    }
}

void func_800D0500(void) // 0x800D0500
{
    s32 temp_v0;
    s32 var_s2;
    s32 var_v0;
    s32 var_v0_2;
    s32 var_v1;

    g_SysWork.bgmStatusFlags |= BgmStatusFlag_Pause;

    var_s2 = 0;

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            D_800D3CA0 = 0;
            Bgm_CrossfadeToTrack(BgmCmd_Track41);
            break;

        case 1:
            Sd_SfxStop(Sfx_Unk1467);
            Fs_QueueWaitForEmpty();
            Anim_CharaTypeAnimInfoClear();
            Fs_QueueStartReadTim(FILE_TIM_UFO2_TIM, FS_BUFFER_1, &D_800D3B44[2]);
            Fs_QueueStartReadTim(FILE_TIM_UFO3_TIM, FS_BUFFER_1, &D_800D3B44[3]);
            Fs_QueueStartReadTim(FILE_TIM_UFO7_TIM, FS_BUFFER_1, &D_800D3B44[4]);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 1, Q12(0.5f), false);

            D_800D3C9C = NO_VALUE;
            D_800D4E2C = 0;
            D_800D4E2D = 0;
            D_800D3C9E = 0;

            SD_Call(Sfx_Unk1613);

            D_800D3C98 = 0;

            SysWork_StateStepIncrement(0);

        case 2:
            Event_WaitTimer(Q12(5.0f), false);
            D_800D3C98 += g_DeltaTime;

            // TODO: Some kind of `Q12_ANGLE_ABS`?
            if ((D_800D3C98 - Q12_TRUNC(D_800D3C98)) > Q12_ANGLE(180.0f))
            {
                Screen_BackgroundImgDrawBlackFadeIn(&D_800D3B44[1]);
            }
            else
            {
                Screen_BackgroundImgDrawBlackFadeIn(&D_800D3B44[0]);
            }

            D_800D3C9C = MAX(0, D_800D3C9C - 3);
            Sd_SfxAttributesUpdate(Sfx_Unk1613, 0, D_800D3C9C, 0);
            break;

        case 3:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.5f), false);

            D_800D3C98 += g_DeltaTime;
            if ((D_800D3C98 - Q12_TRUNC(D_800D3C98)) > Q12_ANGLE(180.0f))
            {
                Screen_BackgroundImgDrawBlackFadeIn(&D_800D3B44[1]);
            }
            else
            {
                Screen_BackgroundImgDrawBlackFadeIn(&D_800D3B44[0]);
            }

            D_800D3C9C = MIN(D_800D3C9C + 3, 255);
            Sd_SfxAttributesUpdate(Sfx_Unk1613, 0, D_800D3C9C, 0);
            break;

        case 4:
            Fs_QueueStartReadTim(FILE_TIM_UFO4_TIM, FS_BUFFER_1, D_800D3B44);
            Sd_SfxStop(Sfx_Unk1613);
            SD_Call(Sfx_Unk1612);
            Fs_QueueWaitForEmpty();
            SysWork_StateStepIncrement(0);
            Sd_XaPreLoadAudioPreTaskAdd(Sfx_XaAudio565);

        case 5:
            Event_WaitTimer(Q12(2.5f), false);
            break;

        case 6:
            SD_Call(Sfx_XaAudio565);
            SysWork_StateStepIncrement(0);

        case 7:
            if (Sd_AudioStreamingCheck() != AudioStreamingState_XaPlaying)
            {
                break;
            }
            SysWork_StateStepIncrement(0);

        case 8:
            Event_WaitTimer(Q12(1.0f), false);
            break;

        case 9:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(2.0f), false);
            Savegame_EventFlagSet(EventFlag_418);
            SysWork_StateStepIncrement(0);

        case 10:
        case 11:
            if (g_SysWork.sysStateSteps[0] == 10)
            {
                Event_WaitTimer(Q12(6.5f), false);
            }
            else
            {
                Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(2.0f), false);
            }

            Gfx_MapMsg_Draw(18);
            Screen_BackgroundImgDrawBlackFadeIn(&D_800D3B44[2]);
            break;

        case 12:
            Event_WaitTimer(Q12(1.8f), false);
            break;

        case 13:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(2.0f), false);
            D_800D3C98 = 0;
            SysWork_StateStepIncrement(0);

        case 14:
        case 15:
            if (g_SysWork.sysStateSteps[0] == 14)
            {
                Event_WaitTimer(Q12(4.2f), false);
            }
            else
            {
                Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(2.0f), false);
            }

            Screen_BackgroundImgDrawBlackFadeIn(&D_800D3B44[3]);

            D_800D3C98 += g_DeltaTime;
            if (D_800D3C98 % Q12(0.2f) > Q12(0.1f))
            {
                func_800D1040(0);
            }
            else
            {
                func_800D1040(1);
            }

            func_800892A4(10);
            break;

        case 16:
            if (Sd_AudioStreamingCheck() != AudioStreamingState_None)
            {
                break;
            }

            SysWork_StateStepIncrement(0);

        case 17:
            Fs_QueueStartReadTim(FILE_TIM_UFO5_TIM, FS_BUFFER_1, &D_800D3B44[1]);
            Fs_QueueWaitForEmpty();
            GameFs_StfRollBinLoad();
            Fs_QueueWaitForEmpty();
            SD_Call(Sfx_XaAudio562);
            SysWork_StateStepIncrement(0);

        case 18:
            if (Sd_AudioStreamingCheck() != AudioStreamingState_XaPlaying)
            {
                break;
            }

            SysWork_StateStepIncrement(0);

        case 19:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(2.0f), false);
            SysWork_StateStepIncrement(0);

        case 20:
        case 21:
            if (g_SysWork.sysStateSteps[0] == 20)
            {
                Event_WaitTimer(Q12(4.0f), false);
            }
            else
            {
                Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(2.0f), false);
            }
            Screen_BackgroundImgDrawBlackFadeIn(D_800D3B44);
            break;

        case 22:
            Event_WaitTimer(Q12(3.8f), false);
            break;

        case 23:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(2.0f), false);
            D_800D3C98 = 0;
            SysWork_StateStepIncrement(0);

        case 24:
            Screen_BackgroundImgDrawBlackFadeIn(&D_800D3B44[1]);
            switch (FP_FROM(D_800D3C98, Q12_SHIFT))
            {
                case 0:
                case 1:
                case 2:
                case 3:
                    func_800D1330(D_800D3C98 >> 2);
                    break;

                case 6:
                    Savegame_EventFlagSet(EventFlag_419);

                case 4:
                case 5:
                    func_800D1040(2);
                    break;

                default:
                    func_801E386C();
                    D_800D3CA0 = 1;
                    D_800D3C98 = 0;
                    SysWork_StateStepIncrement(0);

                case 7:
                case 8:
                case 9:
                case 10:
                case 11:
                case 12:
                case 13:
                    func_800D1040(3);
                    break;
            }

            D_800D3C98 += g_DeltaTime;
            if (D_800D4E2C == 0 && D_800D4E2D == 0)
            {
                if (Rng_GenerateInt(0, 255) == 0) // 1 in 256 chance.
                {
                    D_800D4E2C = Rng_GenerateUInt(-8, 7);
                    D_800D4E2D = Rng_GenerateUInt(-8, 7);
                }
            }
            else
            {
                if (Rng_GenerateInt(0, 31) == 0) // 1 in 32 chance.
                {
                    D_800D4E2C = 0;
                    D_800D4E2D = 0;
                }
            }
            break;

        default:
            Screen_BackgroundImgDrawBlackFadeIn(&D_800D3B44[1]);

            if (D_800D3C9E >= 0)
            {
                D_800D3C9E -= g_DeltaTime;
                if (D_800D3C9E < 0)
                {
                    D_800D3C9E = -Rng_Rand16() % Q12(0.4f);
                }
                else
                {
                    func_800D1040(3);
                }
            }
            else
            {
                D_800D3C9E += g_DeltaTime;
                if (D_800D3C9E >= 0)
                {
                    D_800D3C9E = Rng_GenerateInt(Q12(1.0f), Q12(3.4f) - 1);
                }
                else
                {
                    func_800D1040(2);
                }
            }

            if (D_800D4E2C == 0 && D_800D4E2D == 0)
            {
                if (!Rng_GenerateInt(0, 255)) // 1 in 256 chance.
                {
                    D_800D4E2C = Rng_GenerateUInt(-8, 7);
                    D_800D4E2D = Rng_GenerateUInt(-8, 7);
                }
            }
            else if (!Rng_GenerateInt(0, 31)) // 1 in 32 chance.
            {
                D_800D4E2C = 0;
                D_800D4E2D = 0;
            }
            break;
    }

    D_800C48F0 += g_VBlanks;

    if (D_800D3CA0 == 1)
    {
        var_s2 = func_801E3970(); // Movie player? Credits?
        if (var_s2)
        {
            func_800D1040(4);

            if (D_800D4E2C == 0 && D_800D4E2D == 0)
            {
                if (!Rng_GenerateInt(0, 255)) // 1 in 256 chance.
                {
                    D_800D4E2C = Rng_GenerateUInt(-8, 7);
                    D_800D4E2D = Rng_GenerateUInt(-8, 7);
                }
            }
            else if (!Rng_GenerateInt(0, 31)) // 1 in 32 chance.
            {
                D_800D4E2C = 0;
                D_800D4E2D = 0;
            }
        }
    }

    if (g_SysWork.sysStateSteps[0] >= 25)
    {
        if (Sd_AudioStreamingCheck() != AudioStreamingState_None || var_s2 == 0)
        {
            D_800D3C98 = 0;
            return;
        }

        switch (g_Screen_FadeStatus)
        {
            case ScreenFadeState_Reset:
            case ScreenFadeState_None:
                D_800D3C98 += g_DeltaTime;
                if (D_800D3C98 >= Q12(4.0f))
                {
                    Screen_FadeStart(true, false, false);
                }
                break;

            case ScreenFadeState_FadeOutComplete:
                Savegame_EventFlagSet(EventFlag_588);
                SysWork_StateSetNext(SysState_Gameplay);
                break;
        }
    }
}

void func_800D1040(s32 arg0) // 0x800D1040
{
    s32       var_t4;
    POLY_FT4* poly;

    var_t4 = D_800D3B44->tPage[33];

    if (arg0 == 4)
    {
        var_t4++;
    }

    poly = GsOUT_PACKET_P;

    setPolyFT4(poly);

    if (arg0 == 4)
    {
        // @hack `s16` cast needed to match, can't add to macro as it causes mismatches elsewhere.
        setXY0Fast(poly, (s16)(D_800D3C4C[arg0].x + D_800D4E2C), D_800D3C4C[arg0].y + D_800D4E2D);
        setXY1Fast(poly, (s16)(D_800D3C4C[arg0].x + D_800D4E2C + D_800D3C4C[arg0].w[0]), D_800D3C4C[arg0].y + D_800D4E2D);
        setXY2Fast(poly, (s16)(D_800D3C4C[arg0].x + D_800D4E2C), (D_800D3C4C[arg0].y + D_800D4E2D) + D_800D3C4C[arg0].w[1]);
        setXY3Fast(poly, (s16)(D_800D3C4C[arg0].x + D_800D4E2C + D_800D3C4C[arg0].w[0]), (D_800D3C4C[arg0].y + D_800D4E2D) + D_800D3C4C[arg0].w[1]);
    }
    else
    {
        setXY0Fast(poly, (s16)(D_800D3C4C[arg0].x), D_800D3C4C[arg0].y);
        setXY1Fast(poly, (s16)(D_800D3C4C[arg0].x + D_800D3C4C[arg0].w[0]), D_800D3C4C[arg0].y);
        setXY2Fast(poly, (s16)(D_800D3C4C[arg0].x), (D_800D3C4C[arg0].y + D_800D3C4C[arg0].w[1]));
        setXY3Fast(poly, (s16)(D_800D3C4C[arg0].x + D_800D3C4C[arg0].w[0]), (D_800D3C4C[arg0].y) + D_800D3C4C[arg0].w[1]);
    }

    setUV0AndClutSum(poly, D_800D3C4C[arg0].h[0], D_800D3C4C[arg0].h[1], getClut(D_800D3B44[4].clutX, D_800D3B44[4].clutY));
    setUV1AndTPageSum(poly, D_800D3C4C[arg0].h[0] + D_800D3C4C[arg0].w[0], D_800D3C4C[arg0].h[1], getTPage(1, 0, var_t4 << 6, ((var_t4 >> 4) & 1) << 8));
    setUV2Sum(poly, D_800D3C4C[arg0].h[0], D_800D3C4C[arg0].h[1] + D_800D3C4C[arg0].w[1]);
    setUV3Sum(poly, D_800D3C4C[arg0].h[0] + D_800D3C4C[arg0].w[0], D_800D3C4C[arg0].h[1] + D_800D3C4C[arg0].w[1]);

    setSemiTrans(poly, false);
    setRGB0Fast(poly, 0x80, 0x80, 0x80);

    addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[0], poly);

    poly++;
    GsOUT_PACKET_P = poly;
}

void func_800D1330(s16 arg0) // 0x800D1330
{
    POLY_FT4* poly;
    s32       temp_t8;
    s32       temp_v0_5;
    s32       var_t1;
    s32       var_t2;
    s32       var_t4;
    s32       i;
    s32       var_v1;
    s32       temp_v0;
    s32       temp_v0_2;
    s32       temp_v0_4;
    s32       var_a3;
    s32       temp;

    poly = GsOUT_PACKET_P;

    temp_v0   = Q12_MULT_PRECISE(arg0, 76) - 88;
    temp_t8   = D_800D4E2D + temp_v0;
    temp_v0_5 = D_800D4E2D - 8;

    for (i = 0; i < 2; i++)
    {
        if (i == 0)
        {
            temp_v0_2 = D_800D4E2C + 18;
            temp      = Q12_MULT_PRECISE(arg0, 167) - 160;
            var_t1    = D_800D4E2C + temp;
            var_v1    = temp_v0_2;
            var_a3    = D_800D3B44[4].tPage[1];
            var_t2    = 0;
            var_t4    = 0xB2;
        }
        else
        {
            temp_v0_4 = Q12_MULT_PRECISE(arg0, 135) - 160;
            var_t1    = D_800D4E2C + 18;
            var_v1    = D_800D4E2C - temp_v0_4;
            var_a3    = D_800D3B44[4].tPage[1] + 1;
            var_t2    = 50;
            var_t4    = 142;
        }

        setPolyFT4(poly);

        setXY0Fast(poly, var_t1, temp_t8);
        setXY1Fast(poly, var_v1, temp_t8);
        setXY2Fast(poly, var_t1, temp_v0_5);
        setXY3Fast(poly, var_v1, temp_v0_5);

        setUV0AndClutSum(poly, var_t2, 0, getClut(D_800D3B44[4].clutX, D_800D3B44[4].clutY));
        setUV1AndTPageSum(poly, var_t2 + var_t4, 0, getTPage(1, 0, var_a3 << 6, ((var_a3 >> 4) & 1) << 8));
        setUV2Sum(poly, var_t2, 80);
        setUV3Sum(poly, var_t2 + var_t4, 80);

        setSemiTrans(poly, 0);
        setRGB0Fast(poly, 0x80, 0x80, 0x80);

        addPrim(g_OrderingTable0[g_ActiveBufferIdx].org, poly);
        poly++;
    }

    GsOUT_PACKET_P = poly;
}

void Map_WorldObjectsInit(void) // 0x800D1658
{
    WorldObject_Init(&g_WorldObject_SavePad, g_CommonWorldObjectNames[1], -18.85f, 3.06f, -21.16f, 0.0f, 111.6f, 0.0f);

    D_800D4E09 = 0;
    D_800D4E6C = 0;
    D_800D4E6D = 0;
    D_800D4E6E = 0;
    D_800D4E70 = 0;

    g_SysWork.lightBoneCoord = NULL;
    g_SysWork.lensFlareBoneCoord = NULL;

    func_8008D438();
    Gfx_MapEnvSet(4, 4);

    Savegame_EventFlagSet(EventFlag_428);
}

void Map_WorldObjectsUpdate(void) // 0x800D1718
{
    s32    flags;
    q19_12 mag;
    MAP_CHUNK_CHECK_VARIABLE_DECL();

    flags = 0;

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -1, 0, 0) && PLAYER_IN_MAP_CHUNK(vz, 1, -1, 0, 0))
    {
        flags = func_800D1D40();
        WorldObjects_Add(&g_WorldObject_SavePad.object, &g_WorldObject_SavePad.position, &g_WorldObject_SavePad.rotation);
    }
    else
    {
        Savegame_EventFlagClear(EventFlag_405);
        Savegame_EventFlagClear(EventFlag_406);
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, 2, -1, 2) && PLAYER_IN_MAP_CHUNK(vz, 1, -1, 0, 0))
    {
        mag = Math_Vector2MagCalcSafeQ6(g_SysWork.playerWork.player.position.vx - Q12(60.0f),
                                  g_SysWork.playerWork.player.position.vz + Q12(20.0f));
        if (mag < Q12(1.4f))
        {
            if (ratan2(g_SysWork.playerWork.player.position.vx - Q12(60.0f),
                       g_SysWork.playerWork.player.position.vz + Q12(20.0f)) > Q12_ANGLE(67.5f))
            {
                Savegame_EventFlagSet(EventFlag_407);
                D_800D4E6C = 6;
            }
        }

        if (!D_800D4E6D)
        {
            g_SysWork.lightIntensity = Q12(2.8f);
            Math_Vector3Set(&g_SysWork.lightPosition, Q12(60.0f), Q12(-2.5f), Q12(-20.0f));
            Math_SetSVectorFast(&g_SysWork.lightRotation, Q12_ANGLE(-90.0f), Q12_ANGLE(0.0f), Q12_ANGLE(0.0f));

            func_800D2170(false);
            SD_Call(Sfx_Unk1611);

            D_800D4E6D = 1;
        }
        else if (g_SysWork.bgmStatusFlags & BgmStatusFlag_6)
        {
            func_800D2170(true);
            D_800D4E6D = 1;
        }
#if VERSION_EQUAL_OR_NEWER(JAP2) // @bugfix?
        else if (D_800D4E6D == 1)
        {
            D_800D4E6D++;
        }
        else if (D_800D4E6D == 2)
        {
            D_800D4E6D++;
            D_800C4414 |= 1 << 5;
        }
#else
        else if (D_800D4E6D == 1)
        {
            D_800D4E6D++;
            D_800C4414 |= 1 << 5;
        }
#endif

        D_800D4E70 += Q12_MULT_PRECISE(Rng_GenerateUInt(0, 4095), g_DeltaTime);

        Sd_SfxAttributesUpdate(Sfx_Unk1611, 0, ((Math_Sin(D_800D4E70) >> 10) - 32), -64);
    }
    else
    {
        if (D_800D4E6D)
        {
            Sd_SfxStop(Sfx_Unk1611);
            D_800D4E6D = 0;
            D_800C4414 &= ~(1 << 5);
        }

        Savegame_EventFlagClear(EventFlag_407);
        Savegame_EventFlagClear(EventFlag_408);
    }

    Collision_TriggerFlagBitsClear(CollisionTriggerFlag_All);
    Collision_TriggerFlagBitsSet(flags);
}

void func_800D1AE4(void) // 0x800D1AE4
{
    s32      temp_s2;
    q19_12   angle;
    VECTOR3* vec;
    SVECTOR* svec;
    MAP_CHUNK_CHECK_VARIABLE_DECL();

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -1, -1, -1) && PLAYER_IN_MAP_CHUNK(vz, 1, -1, -1, -1))
    {
        if (D_800D4E6E == 0)
        {
            Game_TurnFlashlightOn();
            func_800CED74(&g_SysWork.playerWork.player, true);

            D_800D4E6E = 1;
        }

        g_SysWork.lightIntensity = Q12(0.7f);

        if (g_SysWork.playerWork.player.position.vy > Q12(2.84f))
        {
            angle                             = Q12_ANGLE(0.0f);
            g_SysWork.lightIntensity = (((g_SysWork.playerWork.player.position.vy - Q12(2.84f)) * Q12(0.4f)) / (Q12(1.16f) + 1)) + Q12(0.7f);
        }
        else
        {
            angle = ratan2(g_SysWork.playerWork.player.position.vx + Q12(20.0f),
                           g_SysWork.playerWork.player.position.vz + Q12(20.0f));
        }

        func_800CED74(&g_SysWork.playerWork.player, false);

        temp_s2 = angle + Q12_ANGLE(70.0f);

        vec = &g_SysWork.lightPosition;

        vec->vx = Q12_MULT(Math_Sin(temp_s2), Q12(0.55f)) - Q12(20.0f);
        vec->vy = MIN(g_SysWork.playerWork.player.position.vy - Q12(1.6f), Q12(0.5f));
        vec->vz = Q12_MULT(Math_Cos(temp_s2), Q12(0.55f)) - Q12(20.0f);

        svec = &g_SysWork.lightRotation;

        Math_SetSVectorFast(svec, Q12_ANGLE(-40.0f), angle - Q12_ANGLE(150.0f), 0);
    }
    else
    {
        D_800D4E6E = 0;
    }
}

s32 func_800D1D40(void) // 0x800D1D40
{
    q19_12 angle;

    angle = ratan2(g_SysWork.playerWork.player.position.vx + Q12(20.0f),
                   g_SysWork.playerWork.player.position.vz + Q12(20.0f));

    if (D_800D4E6C == 0)
    {
        Savegame_EventFlagSet(EventFlag_416);

        if (angle > Q12_ANGLE(0.0f) && angle < Q12_ANGLE(90.0f))
        {
            D_800D4E6C = 1;
        }

        return 258;
    }

    if (D_800D4E6C & (1 << 0))
    {
        if (angle < Q12_ANGLE(-78.8f))
        {
            D_800D4E6C++;
        }
        else
        {
            if ((D_800D4E6C == 1 && angle > Q12_ANGLE(90.0f)) || angle > Q12_ANGLE(78.8f))
            {
                D_800D4E6C--;
            }
        }
    }
    else
    {
        if (angle < Q12_ANGLE(0.0f) && angle > Q12_ANGLE(-78.8f))
        {
            D_800D4E6C--;
        }
        else if (angle > Q12_ANGLE(0.0f) && angle < Q12_ANGLE(78.8f))
        {
            D_800D4E6C++;
        }

        if (D_800D4E6C == 6 && angle < Q12_ANGLE(-157.5f))
        {
            Savegame_EventFlagSet(EventFlag_405);
        }
    }

    Savegame_EventFlagClear(EventFlag_416);
    return (1 << D_800D4E6C) + 0x80;
}

void func_800D1EB8(s32 arg0, s32* arg1, s32* arg2) // 0x800D1EB8
{
    typedef struct
    {
        RECT field_0;
        u8   field_8[128];
        u8   field_88[128];
        u8   field_108[128];
    } s_func_800D1EB8;

    s32              var_a0;
    s32              j;
    s32              i;
    s32              var_t2;
    s32              var_t3;
    s32              var_t6;
    s32*             ptr0;
    s32*             ptr1;
    s_func_800D1EB8* basePtr;

    basePtr = PSX_SCRATCH;

    ptr0 = basePtr->field_8;

    for (i = 31; i >= 0; i--, ptr0++)
    {
        *ptr0 = 0;
    }

    ptr1 = arg1;

    for (i = 0; i < 64; i++, ptr0++, ptr1++)
    {
        *ptr0 = *ptr1;
    }

    var_t6 = 0;

    for (i = 0; i < 256; i++)
    {
        ptr0 = arg2 + (i << 5);

        for (j = 0, var_t3 = 1, var_t2 = -1; j < 256; var_t3++, j++, var_t2++)
        {
            var_a0 = 0;

            if (j > 0)
            {
                var_a0 = (basePtr->field_88[var_t2 >> 1] >> (var_t2 & 0x1) * 4) & 0xF;
            }

            if (j < 255)
            {
                var_a0 += (basePtr->field_88[var_t3 >> 1] >> (var_t3 & 0x1) * 4) & 0xF;
            }

            if (i > 0)
            {
                var_a0 += (basePtr->field_8[j >> 1] >> (j & 0x1) * 4) & 0xF;

                if (j > 0)
                {
                    var_a0 += (basePtr->field_8[var_t2 >> 1] >> (var_t2 & 0x1) * 4) & 0xF;
                }
                if (j < 255)
                {
                    var_a0 += (basePtr->field_8[var_t3 >> 1] >> (var_t3 & 0x1) * 4) & 0xF;
                }
            }

            if (i < 255)
            {
                var_a0 += (basePtr->field_108[j >> 1] >> (j & 0x1) * 4) & 0xF;

                if (j > 0)
                {
                    var_a0 += (basePtr->field_108[var_t2 >> 1] >> (var_t2 & 0x1) * 4) & 0xF;
                }

                if (j < 255)
                {
                    var_a0 += (basePtr->field_108[var_t3 >> 1] >> (var_t3 & 0x1) * 4) & 0xF;
                }
            }

            var_t6 |= ((var_a0 + 7) >> 3) << (j & 0x7) * 4;

            if ((j & 7) == 7)
            {
                *ptr0  = var_t6;
                var_t6 = 0;
                ptr0++;
            }
        }

        ptr0 = basePtr->field_8;

        if (i != 255)
        {
            ptr1 = basePtr->field_88;

            for (j = 0; j < 64; j++, ptr0++, ptr1++)
            {
                *ptr0 = *ptr1;
            }

            if (i == 254)
            {
                for (j = 31; j >= 0; j--, ptr0++)
                {
                    *ptr0 = 0;
                }
            }
            else
            {
                ptr1 = arg1 + ((i + 2) << 5);

                for (j = 0; j < 32; j++, ptr0++, ptr1++)
                {
                    *ptr0 = *ptr1;
                }
            }
        }
    }

    basePtr->field_0.x = (arg0 & 0xF) << 6;
    basePtr->field_0.y = (arg0 * 16) & 0x100;
    basePtr->field_0.w = 64;
    basePtr->field_0.h = 256;

    LoadImage(&basePtr->field_0, arg2);
    DrawSync(0);
}

void func_800D2170(s32 arg0) // 0x800D2170
{
    TIM_IMAGE       img;
    s32*            buffers[2];
    s32             temp_a0;
    s32             rng15;
    s32             rng0;
    s32             i;
    s32             j;
    u32*            ptr2;
    s_8019F8F8_908* ptr;
    s_8019F8F8*     base;

    base = FS_BUFFER_22;

    ptr  = &base->field_908;
    ptr2 = ptr->field_2000;

    if (!arg0)
    {
        i = 0;

        func_8008D438();
        Fs_QueueStartReadTim(FILE_TIM_LHEFFECT_TIM, FS_BUFFER_1, &D_800CAB90);
        Fs_QueueWaitForEmpty();
        OpenTIM(FS_BUFFER_1);
        ReadTIM(&img);

        buffers[0] = (s32*)img.paddr;
        buffers[1] = buffers[0] + 0x2000;
    }
    else
    {
        memset(ptr, 0, sizeof(s_8019F8F8_908));
        StoreImage(&D_800CAB98, FS_BUFFER_1);
        DrawSync(0);

        buffers[0] = FS_BUFFER_1;
        buffers[1] = FS_BUFFER_10;
        i          = 2;
    }

    for (; i < (ARRAY_SIZE(D_800D3C74) - 1); i++)
    {
        func_800D1EB8(D_800D3C74[i + 1], buffers[i & 1], buffers[(i & 0x1) == 0]);
    }

    for (i = 0; i < 16; i++)
    {
        for (j = 0; j < 16; j++)
        {
            rng0 = Rng_Rand16();
            rng15 = rng0 & 0xF;

            if (rng15 < 8)
            {
                temp_a0 = (rng0 >> 4) & 0x7FFFF;

                GET_PTR(ptr, i, j, rng15) = temp_a0;

                if (rng15 > 0)
                {
                    GET_PTR(ptr, i, j, rng15 - 1) = temp_a0 + 0x80000;
                }
            }

            GET_PTR(ptr2, i, j, 0) = 0x100000;
        }
    }
}

void func_800D2364(void) // 0x800D2364
{
    typedef struct
    {
        SPRT*     field_0;
        TILE*     field_4;
        DR_TPAGE* field_8;
        DR_STP*   field_C;
        s8        unk_10[8];
        VECTOR3   field_18;
        MATRIX    field_24;
        SVECTOR   field_44[4];
        s32       field_64;
        s32       field_68;
        u8        field_6C[17][17];
        u8        field_18D[17][17];
    } s_func_800D2364;

    s_8019F8F8_908*  sp10;
    u32*             sp14;
    PACKET*          packet;
    s32              k;
    s32              temp_a2;
    s32              temp_s0;
    s32              temp_v0_4;
    s32              j;
    s32              i;
    s_8019F8F8_0*    var_t2_2;
    u8               temp_v1_17;
    POLY_GT4*        poly;
    s32              temp;
    s_func_800D2364* ptr;
    s32              otIdx;
    s_8019F8F8*      base;

    base = FS_BUFFER_22;

    sp10     = &base->field_908;
    sp14     = sp10->field_2000;
    var_t2_2 = &base->field_0;

    ptr = PSX_SCRATCH;

    ptr->field_18.vx = Q12(3.75f);
    ptr->field_18.vy = Q12(0.0f);
    ptr->field_18.vz = Q12(-1.25f);

    Vw_WorldScreenMatrixAtPositionGet(&ptr->field_24, 0x3C000, 0, -0x14000);

    gte_SetRotMatrix(&ptr->field_24);
    gte_SetTransMatrix(&ptr->field_24);
    gte_ReadGeomScreen(&ptr->field_64);

    ptr->field_68 = g_ActiveBufferIdx;
    temp          = Q12_MULT_PRECISE((g_GameWork.config.brightness * 8) + 4, Screen_FadeProgressGet());

    ptr->field_4 = GsOUT_PACKET_P;

    otIdx = 3;

    for (j = 0; j < 4; j++)
    {
        i = j & 1;

        setRGBC0(ptr->field_4, temp, temp, temp, PRIM_RECT | RECT_BLEND);
        setXY0Fast(ptr->field_4, (i << 8) - 0xA0, 0xFF90);
        setWH(ptr->field_4, i ? 64 : 256, FRAMEBUFFER_HEIGHT_PROGRESSIVE);

        addPrimFast(&g_OrderingTable2[g_ActiveBufferIdx].org[otIdx], ptr->field_4, 3);
        ptr->field_4++;

        ptr->field_8 = ptr->field_4;

        setDrawTPage(ptr->field_8, 0, 0, getTPage(2, (j >= 2) ? 2 : 1, ((ptr->field_68 << 4) + (i << 2)) << 6, (((ptr->field_68 << 4) >> 4) & 1) << 8));
        AddPrim(&g_OrderingTable2[g_ActiveBufferIdx].org[otIdx], ptr->field_8);

        ptr->field_8++;
        ptr->field_4 = ptr->field_8;
    }

    ptr->field_0 = ptr->field_4;

    if (Savegame_EventFlagGet(EventFlag_412))
    {
        ptr->field_68 ^= 1;
    }

    for (i = 0; i < 2; i++)
    {
        setRGBC0(ptr->field_0, 112, 112, 112, PRIM_RECT | RECT_BLEND | RECT_TEXTURE);
        setXY0Fast(ptr->field_0, (i << 8) - 160, -112);
        setUV0(ptr->field_0, 0, (ptr->field_68 == 0) << 5);
        setWH(ptr->field_0, i ? 64 : 256, FRAMEBUFFER_HEIGHT_PROGRESSIVE);

        addPrimFast(&g_OrderingTable2[g_ActiveBufferIdx].org[otIdx], ptr->field_0, 4);
        ptr->field_0++;

        ptr->field_8 = ptr->field_0;

        setDrawTPage(ptr->field_8, 0, 0, getTPage(2, 0, ((ptr->field_68 << 4) + (i << 2)) << 6, (((ptr->field_68 << 4) >> 4) & 1) << 8));

        AddPrim(&g_OrderingTable2[g_ActiveBufferIdx].org[otIdx], ptr->field_8);

        ptr->field_8++;
        ptr->field_0 = ptr->field_8;
    }

    ptr->field_C = ptr->field_0;

    SetDrawStp(ptr->field_C, 1);
    addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[ORDERING_TABLE_SIZE - 1], ptr->field_C);
    ptr->field_C++;

    SetDrawStp(ptr->field_C, 0);
    addPrim(&g_OrderingTable2[g_ActiveBufferIdx].org[0], ptr->field_C);

    ptr->field_C++;
    packet = ptr->field_C;

    for (i = 0; i < 17; i++)
    {
        for (j = 0; j < 17; j++)
        {
            ptr->field_18D[j][i] = 0;
        }
    }

    for (k = 0; k < 8; k++)
    {
        for (i = 1; i < 16; i++)
        {
            for (j = 1; j < 16; j++)
            {
                temp_s0 = GET_PTR(sp10, j, i, k);

                if (temp_s0 > 0)
                {
                    temp_v0_4 = Q12_MULT_PRECISE(g_DeltaTime, GET_PTR(sp14, i, j, 0));
                    ptr->field_18D[j][i]++;

                    if (temp_s0 > 0x80000)
                    {
                        temp_s0 += temp_v0_4;
                        if (temp_s0 > 0x100000)
                        {
                            GET_PTR(sp10, j, i, k) = 0;
                            ptr->field_6C[j][i]    = 0;
                        }
                        else
                        {
                            GET_PTR(sp10, j, i, k) = temp_s0;
                            ptr->field_6C[j][i]    = -(temp_s0 >> Q12_SHIFT);
                        }
                    }
                    else
                    {
                        temp_s0 += temp_v0_4;
                        if (k < 7 && temp_s0 > 0x80000)
                        {
                            if (Rng_Rand16() & 0xF)
                            {
                                GET_PTR(sp10, j, i, k + 1)
                                ++;
                            }
                            ptr->field_6C[j][i] = -(temp_s0 >> Q12_SHIFT);
                        }
                        else
                        {
                            ptr->field_6C[j][i] = temp_s0 >> Q12_SHIFT;
                        }

                        GET_PTR(sp10, j, i, k) = temp_s0;
                    }
                }
                else
                {
                    ptr->field_6C[j][i] = 0;
                }
            }

            ptr->field_6C[16][i] = 0;
            ptr->field_6C[0][i]  = 0;
        }

        for (j = 0; j < 17; j++)
        {
            ptr->field_6C[j][16] = 0;
            ptr->field_6C[j][0]  = 0;
        }

        for (i = 0; i < 287; i += 3)
        {
            for (j = 0; j < 3; j++)
            {
                Math_SetSVectorFastSum(&ptr->field_44[j],
                                       (((i + j) / 17) << 7) - 0x400,
                                       -8 * k,
                                       0x400 - (((i + j) % 17) << 7));
            }

            gte_ldv3c(&ptr->field_44[0]);
            gte_rtpt();
            gte_stsxy3c(&var_t2_2->field_0[i / 17][i % 17]);
            gte_stsz3c(&var_t2_2->field_484[i / 17][i % 17]);
        }

        for (i = 287; i < 289; i++)
        {
            Math_SetSVectorFastSum(&ptr->field_44[0],
                                   ((i / 17) << 7) - 0x400,
                                   -8 * k,
                                   0x400 - ((i % 17) << 7));

            gte_ldv0(&ptr->field_44[0]);
            gte_rtps();
            gte_stsxy(&var_t2_2->field_0[i / 17][i % 17]);
            gte_stsz(&var_t2_2->field_484[i / 17][i % 17]);
        }

        for (i = 0; i < 16; i++)
        {
            for (j = 0; j < 16; j++)
            {
                temp_a2 = (var_t2_2->field_484[i][j] + var_t2_2->field_484[i + 1][j] +
                           var_t2_2->field_484[i][j + 1] + var_t2_2->field_484[i + 1][j + 1]) >> 2;
                if (temp_a2 < 65 || ((temp_a2 - 64) >> 3) >= ORDERING_TABLE_SIZE)
                {
                    continue;
                }

                if (ABS(var_t2_2->field_0[i][j].vx) > 200 && ABS(var_t2_2->field_0[i + 1][j].vx) > 200 &&
                    ABS(var_t2_2->field_0[i][j + 1].vx) > 200 && ABS(var_t2_2->field_0[i + 1][j + 1].vx) > 200)
                {
                    continue;
                }

                if (ABS(var_t2_2->field_0[i][j].vy) > 160 && ABS(var_t2_2->field_0[i + 1][j].vy) > 160 &&
                    ABS(var_t2_2->field_0[i][j + 1].vy) > 160 && ABS(var_t2_2->field_0[i + 1][j + 1].vy) > 160)
                {
                    continue;
                }

                if (ptr->field_6C[i][j] < 4 && ptr->field_6C[i + 1][j] < 4 &&
                    ptr->field_6C[i][j + 1] < 4 && ptr->field_6C[i + 1][j + 1] < 4)
                {
                    continue;
                }

                poly = packet;

                setPolyGT4(poly);
                setSemiTrans(poly, 1);

                setXY0Fast(poly, var_t2_2->field_0[i][j].vx, var_t2_2->field_0[i][j].vy);
                setXY1Fast(poly, var_t2_2->field_0[i + 1][j].vx, var_t2_2->field_0[i + 1][j].vy);
                setXY2Fast(poly, var_t2_2->field_0[i][j + 1].vx, var_t2_2->field_0[i][j + 1].vy);
                setXY3Fast(poly, var_t2_2->field_0[i + 1][j + 1].vx, var_t2_2->field_0[i + 1][j + 1].vy);

                setRGB0(poly, ptr->field_6C[i][j], ptr->field_6C[i][j], ptr->field_6C[i][j]);
                setRGB1(poly, ptr->field_6C[i + 1][j], ptr->field_6C[i + 1][j], ptr->field_6C[i + 1][j]);
                setRGB2(poly, ptr->field_6C[i][j + 1], ptr->field_6C[i][j + 1], ptr->field_6C[i][j + 1]);
                setRGB3(poly, ptr->field_6C[i + 1][j + 1], ptr->field_6C[i + 1][j + 1], ptr->field_6C[i + 1][j + 1]);

                setUV0AndClutSum(poly, i << 4, j << 4, 0x030E);
                temp_v1_17 = D_800D3C74[k];
                setUV1AndTPageSum(poly, 0xF + (i << 4), (j << 4), getTPage(0, 1, temp_v1_17 << 6, (((temp_v1_17 >> 4) & (1 << 0)) << 8)));
                setUV2Sum(poly, i << 4, 0xF + 0x10 * j);
                setUV3Sum(poly, 0xF + i * 16, 0xF + 0x10 * j);

                addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[(temp_a2 - 0x40) >> 3], poly);
                poly++;
                packet = poly;
            }
        }
    }

    GsOUT_PACKET_P = packet;

    for (i = 1; i < 16; i++)
    {
        for (j = 1; j < 16; j++)
        {
            if (ptr->field_18D[j][i] == 0)
            {
                if ((Rng_Rand16() & 0x7FFF) <= g_DeltaTime)
                {
                    GET_PTR(sp10, j, i, 0)
                    ++;
                    GET_PTR(sp14, j, i, 0) = (Rng_Rand16() & 0x7FFFF) + 0x100000;
                }
            }
        }
    }
}

void func_800D32D0(void) // 0x800D32D0
{
    g_Screen_FadeStatus = ScreenFadeState_FadeOutComplete;

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimLock, &g_SysWork.playerWork.player, 0, false);

            g_SysWork.sysStateSteps[0]++;
            break;

        case 1:
            Bgm_CrossfadeToTrack(BgmCmd_Track5);
            break;

        case 2:
            g_SavegamePtr->clearGameCount++;
            g_SavegamePtr->clearGameCount                = CLAMP(g_SavegamePtr->clearGameCount, 1, 99);
            g_SavegamePtr->currentEndingFlags            = GameEndingFlag_Ufo;
            g_SavegamePtr->clearGameEndings             |= GameEndingFlag_Ufo;
            g_GameWorkConst->config.extraOptionsEnabled |= 1 << 4;
            g_SavegamePtr->locationId                    = SaveLocationId_NextFear;

            SysWork_StateSetNext(SysState_StatusMenu);
            break;
    }
}

const s_FsImageDesc D_800CAB90 = { { 0, 25 }, 0, 0, 224, 12 };

const RECT D_800CAB98 = { 832, 256, 64, 256 };
