#include "bodyprog/game.h"
#include "inline_no_dmpsx.h"

#include <psyq/libpad.h>
#include <psyq/strings.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/events/data.h"
#include "bodyprog/events/events_util.h"
#include "bodyprog/screen/data.h"
#include "bodyprog/screen/draw.h"
#include "bodyprog/screen/background.h"
#include "bodyprog/items/item_screens.h"
#include "bodyprog/math/math.h"
#include "bodyprog/sound/sd_call.h"
#include "main/fs_queue.h"

/** @brief EVENT AND INTERACTION HELPERS
 *
 * The following are helper functions for map events.
 * Most are designed to be called once per tick and track their own progress across ticks.
 *
 * Map events use a state machine driven by `sysStateSteps[0]` (or `sysStateSteps[1]` for sub-steps).
 * Each tick, the game re-enters the map event function and runs the case matching the current step.
 * The helper called within that case does its work and only advances `sysStateSteps` when
 * complete, allowing the next tick to move on to the next case.
 *
 * Example map event code:
 *
 * switch (g_SysWork.sysStateSteps[0])
 * {
 *   case 0:
 *       Event_WaitTimer(Q12(5.0f), false); // Called every tick until 5 seconds have elapsed.
 *       break;                             // Once elapsed, `Event_WaitTimer` handles incrementing `sysStateSteps[0]` to 1.
 *
 *   case 1:
 *       Event_WaitPlayerStop();            // Called every tick until player has stopped.
 *       break;                             // Once stopped, `Event_WaitPlayerStop` handles incrementing `sysStateSteps[0]` to 2.
 *
 *   case 2:
 *       DoSomething();                     // Only reached after both previous steps have completed.
 *       break;
 * }
 *
 * This pattern allows sequential, blocking-style code to be written without actually blocking the game's main loop.
 * The helpers above "wait" by simply returning without advancing the step, and "complete" by incrementing the step counter.
 * The animation/inventory item/map message functions work similarly, being called every tick and "waiting" by simply not advancing the step.
 * TODO: Describe the "Auto" commands that use `sysStateSteps[2]`. Need to check how map events use that.
 */

VECTOR3 g_Event_PathWaypoints[2][8];
q3_12   g_Event_PathWaypointHeadingAngles[8]; // TODO: should only have 2 entries, 1 for each character slot?
q19_12  g_Event_TweenTimers[6];

void Event_SysStateStepIncrement(bool incSubStep) // 0x80085D78
{
    if (incSubStep)
    {
        SysWork_StateStepIncrement(1);
    }
    else
    {
        SysWork_StateStepIncrement(0);
    }
}

void Event_SysStateStepSet(bool setSubStep, s32 sysStateStep) // 0x80085DC0
{
    if (setSubStep)
    {
        SysWork_StateStepSet(1, sysStateStep);
    }
    else
    {
        SysWork_StateStepSet(0, sysStateStep);
    }
}

void Event_WaitPlayerStop(void) // 0x80085DF0
{
    g_SysWork.sysStateStepData[1] += g_DeltaTimeRaw;
    if (g_MapOverlayHdr.playerMoveSpeedIsZero() || g_SysWork.sysStateStepData[1] > Q12(1.0f))
    {
        SysWork_StateStepIncrement(0);
    }
}

void Event_WaitTimer(q19_12 delay, bool incSubStep) // 0x80085E6C
{
    g_SysWork.sysStateStepData[1] += g_DeltaTimeRaw;
    if (delay < g_SysWork.sysStateStepData[1])
    {
        Event_SysStateStepIncrement(incSubStep);
    }
}

void Event_CharaAnimCmdExecute(e_CharaAnimCmd cmd, s_SubCharacter* chara, s32 animState, bool incSubStep) // 0x80085EB8
{
    s32  playbackState;

    #define isPlayer (chara == &PLAYER_CHARA())

    switch (cmd)
    {
        case CharaAnimCmd_SetState:
            if (isPlayer)
            {
                g_MapOverlayHdr.playerAnimStateSet(animState);
            }
            else
            {
                g_MapOverlayHdr.charaAnimStateSet(chara, animState);
            }
            break;

        case CharaAnimCmd_AwaitAnimEnd:
            if (isPlayer)
            {
                playbackState = g_MapOverlayHdr.playerAnimPlaybackStateGet();
                if (playbackState == AnimPlaybackState_End)
                {
                    Event_SysStateStepIncrement(incSubStep);
                }
            }
            else
            {
                playbackState = g_MapOverlayHdr.charaAnimPlaybackStateGet(chara);
                if (playbackState == AnimPlaybackState_End)
                {
                    Event_SysStateStepIncrement(incSubStep);
                }
            }
            break;

        case CharaAnimCmd_AnimLock:
            if (isPlayer)
            {
                g_MapOverlayHdr.playerAnimLock();
            }
            else
            {
                g_MapOverlayHdr.charaAnimLock(chara);
            }
            break;

        case CharaAnimCmd_AnimUnlock:
            if (isPlayer)
            {
                g_MapOverlayHdr.playerAnimUnlock();
            }
            else
            {
                g_MapOverlayHdr.charaAnimUnlock(chara);
            }
            break;

        case CharaAnimCmd_AnimReset:
            if (isPlayer)
            {
                g_MapOverlayHdr.playerAnimUnlock();
                g_MapOverlayHdr.playerAnimReset();
            }
            else
            {
                g_MapOverlayHdr.charaControlStateReset(chara);
            }
            break;
    }

    #undef isPlayer
}

void Event_SysStateBranchOnFlag(e_EventFlag eventFlagIdx, s32 stepTrue, s32 stepFalse, bool setSubStep) // 0x8008605C
{
    if (!Savegame_EventFlagGet(eventFlagIdx))
    {
        Event_SysStateStepSet(setSubStep, stepFalse);
    }
    else
    {
        Event_SysStateStepSet(setSubStep, stepTrue);
    }
}

void Event_DisplayMapMsg(bool hasSelection, s32 mapMsgIdx, s32 step0, s32 step1, s32 step2, bool incSubStep) // 0x800860B0
{
    s32 mapMsgState;

    mapMsgState = Gfx_MapMsg_Draw(mapMsgIdx);
    if (mapMsgState <= MapMsgState_Idle)
    {
        return;
    }

    if (!hasSelection)
    {
        Event_SysStateStepIncrement(incSubStep);
        return;
    }

    if (mapMsgState == MapMsgState_SelectEntry0)
    {
        Event_SysStateStepSet(incSubStep, step0);
    }
    if (mapMsgState == MapMsgState_SelectEntry1)
    {
        Event_SysStateStepSet(incSubStep, step1);
    }
    if (mapMsgState == MapMsgState_SelectEntry2)
    {
        Event_SysStateStepSet(incSubStep, step2);
    }
}

void Event_ScreenFadeCmd(e_ScreenFadeCmd cmd, bool fadeOut, e_ScreenFadeType fadeType, q19_12 fadeTimestep, bool incSubStep) // 0x8008616C
{
    e_ScreenFadeCmd activeCmd;

    // If `cmd == ScreenFadeCmd_Auto`, `sysStateSteps[2]` dictates the command. This field is manipulated often in map event functions.
    if (cmd != ScreenFadeCmd_Auto)
    {
        activeCmd = cmd;
    }
    else
    {
        activeCmd = g_SysWork.sysStateSteps[2];
    }

    switch (activeCmd)
    {
        case ScreenFadeCmd_Start:
            if (fadeType != ScreenFadeType_ScreenBorders)
            {
                g_ScreenFadeTimestep = fadeTimestep;
            }

            if (fadeOut)
            {
                if (fadeType == ScreenFadeType_Black)
                {
                    Screen_FadeStart(false, false, false);
                }
                else if (fadeType == ScreenFadeType_White)
                {
                    Screen_FadeStart(false, false, true);
                }
                else
                {
                    CutsceneBorder_FadeInStart();

                    if (fadeType == ScreenFadeType_CutsceneBorders)
                    {
                        g_SysWork.sysFlags |= SysFlag_CutsceneActive;
                    }
                }
            }
            else
            {
                if (fadeType == ScreenFadeType_Black)
                {
                    Screen_FadeStart(false, true, false);
                }
                else if (fadeType == ScreenFadeType_White)
                {
                    Screen_FadeStart(false, true, true);
                }
                else
                {
                    CutsceneBorder_FadeOutStart();
                }
            }

            // `cmd` will only be different if `ScreenFadeCmd_Auto` was passed.
            if (cmd != ScreenFadeCmd_Start)
            {
                SysWork_StateStepIncrement(2);
            }
            break;

        case ScreenFadeCmd_Wait:
            // `fadeType == ScreenFadeType_Black || fadeType == ScreenFadeType_White`.
            if (fadeType < ScreenFadeType_ScreenBorders)
            {
                if ((fadeOut == false && Screen_IsFadeNone()) ||
                    (fadeOut == true  && Screen_IsFadeFinished()))
                {
                    Event_SysStateStepIncrement(incSubStep);
                }
            }
            else if ((fadeOut == false && g_SysWork.cutsceneBorderState == CutsceneBorderState_None) ||
                     (fadeOut == true  && g_SysWork.cutsceneBorderState == CutsceneBorderState_Shown))
            {
                Event_SysStateStepIncrement(incSubStep);
            }

            break;
    }
}

const RECT D_8002AB10 = // 0x8002AB10 .rodata
{
    SCREEN_WIDTH, 256,
    (SCREEN_WIDTH / 5) * 3, SCREEN_HEIGHT
};

void Event_BgTextureCmd(e_BgTextureCmd cmd, e_FsFile texFileIdx, bool incSubStep) // 0x800862F8
{
    e_BgTextureCmd activeCmd;

    if (cmd == BgTextureCmd_Auto)
    {
        activeCmd = g_SysWork.sysStateSteps[2];
    }
    else
    {
        activeCmd = cmd;
        if (activeCmd == BgTextureCmd_8)
        {
            activeCmd = BgTextureCmd_AwaitLoad;
            if (g_SysWork.sysStateSteps[2] == 0)
            {
                activeCmd = BgTextureCmd_QueueReadSecondary;
            }
        }
    }

    switch (activeCmd)
    {
        case BgTextureCmd_QueueRead:
            Fs_QueueStartReadTim(texFileIdx, FS_BUFFER_1, &g_ItemInspectionImg);

            if (cmd != BgTextureCmd_QueueRead)
            {
                SysWork_StateStepIncrement(2);

                if (Fs_QueueChunksLoad())
                {
                    Event_SysStateStepIncrement(incSubStep);
                }
            }
            break;

        case BgTextureCmd_AwaitLoad:
            if (Fs_QueueChunksLoad())
            {
                Event_SysStateStepIncrement(incSubStep);
            }
            break;

        case BgTextureCmd_Draw:
            Screen_BackgroundImgDrawBlackFadeIn(&g_ItemInspectionImg);
            break;

        case BgTextureCmd_StoreVram:
            DrawSync(SyncMode_Wait);
            StoreImage(&D_8002AB10, IMAGE_BUFFER_2);
            DrawSync(SyncMode_Wait);
            break;

        case BgTextureCmd_QueueReadSecondary:
            Fs_QueueStartReadTim(texFileIdx, FS_BUFFER_1, &D_800A9A04);

            if (cmd == BgTextureCmd_8)
            {
                SysWork_StateStepSet(2, BgTextureCmd_AwaitLoad);
            }
            break;

        case BgTextureCmd_DrawSecondary:
            Screen_BackgroundImgDrawBlackFadeIn(&D_800A9A04);
            break;

        case BgTextureCmd_RestoreVram:
            LoadImage(&D_8002AB10, IMAGE_BUFFER_2);
            DrawSync(SyncMode_Wait);
            break;
    }
}

void Event_InvItemCmd(e_InvItemCmd cmd, e_InvItemId itemId, s32 itemCount, bool incSubStep) // 0x80086470
{
    // This func does weird remapping of the input `cmd` to `activeCmd`.
    // `cmd` is also `u32` while `activeCmd` is `s32`, added internal enum here to help func make more sense.
    typedef enum _InvItemCmdInternal
    {
        InvItemCmdInternal_QueueLoad = 0,
        InvItemCmdInternal_AwaitLoad = 1,
        InvItemCmdInternal_AddItem   = 2, // Remapped from `InvItemCmd_AddItem` (3)
        InvItemCmdInternal_Nop       = 3, // Remapped from `InvItemCmd_Nop` (2)
        InvItemCmdInternal_Hack      = -1
    } e_InvItemCmdInternal;

    e_InvItemCmdInternal activeCmd;

    if (cmd == InvItemCmd_6 && g_SysWork.sysStateSteps[2] == 0)
    {
        SysWork_StateStepSet(2, InvItemCmdInternal_AddItem);
    }

    activeCmd = cmd;
    if (cmd >= InvItemCmd_Nop)
    {
        if (cmd == InvItemCmd_Nop)
        {
            activeCmd = InvItemCmdInternal_Nop;
        }
        else if (cmd == InvItemCmd_AddItem)
        {
            activeCmd = InvItemCmdInternal_AddItem;
        }
        else
        {
            activeCmd = g_SysWork.sysStateSteps[2];
        }
    }

    switch (activeCmd)
    {
        case InvItemCmdInternal_QueueLoad:
            GameFs_UniqueItemModelLoad(itemId);

            if (cmd == InvItemCmd_QueueLoad)
            {
                SysWork_StateStepIncrement(1);
                g_SysWork.sysStateSteps[1]--;
            }

            SysWork_StateStepIncrement(2);

        case InvItemCmdInternal_AwaitLoad:
            if (!Fs_QueueChunksLoad())
            {
                break;
            }

            func_80054A04(itemId);

            if (cmd == InvItemCmd_AwaitLoad || cmd == InvItemCmd_4)
            {
                Event_SysStateStepIncrement(incSubStep);
                break;
            }

            SysWork_StateStepIncrement(2);

        case InvItemCmdInternal_AddItem:
            SysWork_StateStepSet(2, 0);

            if (cmd == InvItemCmd_AddItem || cmd == InvItemCmd_6)
            {
                Inventory_AddSpecialItem(itemId, itemCount);
            }
            break;
    }
}

void Event_PathWaypointSet(bool isAbs, s32 charaSlotIdx, s32 waypointIdx, q3_12 headingAngle, q19_12 posX, q19_12 posZ) // 0x800865FC
{
    if (isAbs == false)
    {
        g_Event_PathWaypoints[charaSlotIdx][waypointIdx].vx = PLAYER_CHARA().position.vx + posX;
        g_Event_PathWaypoints[charaSlotIdx][waypointIdx].vy = PLAYER_CHARA().position.vy;
        g_Event_PathWaypoints[charaSlotIdx][waypointIdx].vz = PLAYER_CHARA().position.vz + posZ;

        // @bug `headingAngle` is stored per `charaSlotIdx`, not per waypoint.
        // Event code often calls `Event_PathWaypointSet` multiple times in a single tick
        // with different `headingAngle` values, but the same `charaSlot`.
        // Only the last written `headingAngle` is retained and other values are silently discarded.
        // Unlikely this was intentional, but doesn't seem to break things visually at least.
        // Maybe pathfinding code doesn't rely on this angle value that much?
        g_Event_PathWaypointHeadingAngles[charaSlotIdx] = headingAngle;
    }
    else if (isAbs == true)
    {
        g_Event_PathWaypoints[charaSlotIdx][waypointIdx].vx = posX;
        g_Event_PathWaypoints[charaSlotIdx][waypointIdx].vy = PLAYER_CHARA().position.vy;
        g_Event_PathWaypoints[charaSlotIdx][waypointIdx].vz = posZ;

        g_Event_PathWaypointHeadingAngles[charaSlotIdx] = headingAngle;
    }
}

void Event_PathWaypointExecutePlayer(s32 animId, s32 waypointCount, bool incSubStep) // 0x800866D4
{
    if (g_MapOverlayHdr.playerPathWaypointExecute(animId, &g_Event_PathWaypoints[0][0], g_Event_PathWaypointHeadingAngles[0], waypointCount) == true)
    {
        Event_SysStateStepIncrement(incSubStep);
    }
}

void Event_PathWaypointExecuteChara(s_SubCharacter* chara, s32 animId, s32 waypointCount, bool incSubStep) // 0x80086728
{
    if (g_MapOverlayHdr.charaPathWaypointExecute(chara, animId, &g_Event_PathWaypoints[1][0], g_Event_PathWaypointHeadingAngles[1], waypointCount) == true)
    {
        Event_SysStateStepIncrement(incSubStep);
    }
}

void Event_PathWaypointExecuteCharaNoWait(s_SubCharacter* chara, s32 animId, s32 waypointCount) // 0x8008677C
{
    g_MapOverlayHdr.charaPathWaypointExecute(chara, animId, &g_Event_PathWaypoints[1][0], g_Event_PathWaypointHeadingAngles[1], waypointCount);
}

void Event_PaperMapCmd(e_PaperMapCmd cmd, s32 paperMapIdx) // 0x800867B4
{
    switch (cmd)
    {
        case PaperMapCmd_Load:
            DrawSync(SyncMode_Wait);
            StoreImage(&D_8002AB10, IMAGE_BUFFER_2);
            DrawSync(SyncMode_Wait);

            Fs_QueueStartReadTim(FILE_TIM_MP_0TOWN_TIM + g_PaperMapFileIdxs[paperMapIdx], FS_BUFFER_2, &g_PaperMapImg);
            Fs_QueueStartReadTim(FILE_TIM_MR_0TOWN_TIM + g_PaperMapMarkingFileIdxs[paperMapIdx], FS_BUFFER_1, &g_PaperMapMarkingAtlasImg);

            Screen_Init(SCREEN_WIDTH, true);
            GsSwapDispBuff();
            Fs_QueueWaitForEmpty();
            break;

        case PaperMapCmd_Draw:
            Screen_BackgroundImgDraw(&g_PaperMapImg);
            break;

        case PaperMapCmd_Unload:
            LoadImage(&D_8002AB10, IMAGE_BUFFER_2);
            DrawSync(SyncMode_Wait);
            Screen_Init(SCREEN_WIDTH, false);
            break;
    }
}

void Event_TweenReset(s32 timerIdx) // 0x800868DC
{
    #define tweenTimer g_Event_TweenTimers[timerIdx]

    tweenTimer = Q12(0.0f);

    #undef tweenTimer
}

q19_12 Event_TweenLinear(q19_12 target, q19_12 duration, s32 timerIdx) // 0x800868F4
{
    #define tweenTimer g_Event_TweenTimers[timerIdx]

    tweenTimer += g_DeltaTime;
    tweenTimer  = MIN(duration, tweenTimer);
    return (target * tweenTimer) / duration;

    #undef tweenTimer
}

q19_12 Event_TweenSine(q19_12 amp, q3_12 startAngle, q3_12 sweepAngle, q19_12 duration, s32 timerIdx) // 0x8008694C
{
    #define tweenTimer g_Event_TweenTimers[timerIdx]

    tweenTimer += g_DeltaTime;
    tweenTimer  = MIN(duration, tweenTimer);
    return Q12_MULT(amp, Math_Sin(startAngle + ((sweepAngle * tweenTimer) / duration)));

    #undef tweenTimer
}

void Event_DisplayMapMsgWithAudio(s32 mapMsgIdx, u8* audioIdx, const u16* audioCmds) // 0x800869E4
{
    s32 mapMsgState;

    g_SysWork.bgmStatusFlags |= BgmStatusFlag_VoiceDialog;

    mapMsgState = Gfx_MapMsg_Draw(mapMsgIdx);
    if (mapMsgState == MapMsgState_SelectEntry0)
    {
        SysWork_StateStepIncrement(0);
    }
    else if (mapMsgState == MapMsgState_Finish)
    {
        SD_Call(audioCmds[*audioIdx]);
        *audioIdx += 1;
    }
}

void Event_CameraPositionSet(VECTOR3* pos,
                             q19_12 offsetOrPosX, q19_12 offsetOrPosY, q19_12 offsetOrPosZ,
                             q19_12 accelXz, q19_12 accelY, q19_12 speedXzMax, q19_12 speedYMax,
                             bool warp) // 0x80086A94
{
    VECTOR3         posTarget;
    VC_CAM_MV_PARAM camMoveParams;

    // Set position target.
    if (pos != NULL)
    {
        posTarget.vx = pos->vx + offsetOrPosX;
        posTarget.vy = pos->vy + offsetOrPosY;
        posTarget.vz = pos->vz + offsetOrPosZ;
    }
    else
    {
        posTarget.vx = offsetOrPosX;
        posTarget.vy = offsetOrPosY;
        posTarget.vz = offsetOrPosZ;
    }

    // Set acceleration on XZ plane.
    if (accelXz == Q12(0.0f))
    {
        camMoveParams.accel_xz = cam_mv_prm_user.accel_xz;
    }
    else
    {
        camMoveParams.accel_xz = accelXz;
    }

    // Set acceleration on Y axis.
    if (accelY == Q12(0.0f))
    {
        camMoveParams.accel_y = cam_mv_prm_user.accel_y;
    }
    else
    {
        camMoveParams.accel_y = accelY;
    }

    // Set max speed on XZ plane.
    if (speedXzMax == Q12(0.0f))
    {
        camMoveParams.max_spd_xz = cam_mv_prm_user.max_spd_xz;
    }
    else
    {
        camMoveParams.max_spd_xz = speedXzMax;
    }

    // Set max speed on Y axis.
    if (speedYMax == Q12(0.0f))
    {
        camMoveParams.max_spd_y = cam_mv_prm_user.max_spd_y;
    }
    else
    {
        camMoveParams.max_spd_y = speedYMax;
    }

    // Set camera position target.
    vcUserCamTarget(&posTarget, &camMoveParams, warp);
}

void Event_CameraLookAtSet(VECTOR3* lookAt,
                           q19_12 lookAtOffsetOrPosX, q19_12 lookAtOffsetOrPosY, q19_12 lookAtOffsetOrPosZ,
                           q19_12 angularAccelX, q19_12 angularAccelY, q19_12 angularSpeedXMax, q19_12 angularSpeedYMax,
                           bool warp) // 0x80086B70
{
    VECTOR3           lookAtTarget;
    VC_WATCH_MV_PARAM camLookAtMoveParams;

    // Set look-at target.
    if (lookAt != NULL)
    {
        lookAtTarget.vx = lookAt->vx + lookAtOffsetOrPosX;
        lookAtTarget.vy = lookAt->vy + lookAtOffsetOrPosY;
        lookAtTarget.vz = lookAt->vz + lookAtOffsetOrPosZ;
    }
    else
    {
        lookAtTarget.vx = lookAtOffsetOrPosX;
        lookAtTarget.vy = lookAtOffsetOrPosY;
        lookAtTarget.vz = lookAtOffsetOrPosZ;
    }

    // Set angular acceleration on X axis.
    if (angularAccelX == Q12_ANGLE(0.0f))
    {
        camLookAtMoveParams.ang_accel_x = deflt_watch_mv_prm.ang_accel_x;
    }
    else
    {
        camLookAtMoveParams.ang_accel_x = angularAccelX;
    }

    // Set angular acceleration on Y axis.
    if (angularAccelY == Q12_ANGLE(0.0f))
    {
        camLookAtMoveParams.ang_accel_y = deflt_watch_mv_prm.ang_accel_y;
    }
    else
    {
        camLookAtMoveParams.ang_accel_y = angularAccelY;
    }

    // Set max angular speed on X axis.
    if (angularSpeedXMax == Q12_ANGLE(0.0f))
    {
        camLookAtMoveParams.max_ang_spd_x = deflt_watch_mv_prm.max_ang_spd_x;
    }
    else
    {
        camLookAtMoveParams.max_ang_spd_x = angularSpeedXMax;
    }

    // Set max angular speed on Y axis.
    if (angularSpeedYMax == Q12_ANGLE(0.0f))
    {
        camLookAtMoveParams.max_ang_spd_y = deflt_watch_mv_prm.max_ang_spd_y;
    }
    else
    {
        camLookAtMoveParams.max_ang_spd_y = angularSpeedYMax;
    }

    // Set camera flags and rotation target.
    vcWorkSetFlags(VC_NOFLAG, VC_VISIBLE_CHARA_F);
    vcUserWatchTarget(&lookAtTarget, &camLookAtMoveParams, warp);
}

void Event_CharaAnimPlayToEnd(s_SubCharacter* chara, s32 animState) // 0x80086C58
{
    switch (g_SysWork.sysStateSteps[1])
    {
        case 0:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, chara, animState, false);
            SysWork_StateStepIncrement(1);
            break;

        case 1:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AwaitAnimEnd, chara, 0, true);
            break;

        default:
            SysWork_StateStepIncrement(0);
            break;
    }
}

void Event_CharaAnimUnlockPlayToEnd(s_SubCharacter* chara) // 0x80086D04
{
    switch (g_SysWork.sysStateSteps[1])
    {
        case 0:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimUnlock, chara, 0, false);
            SysWork_StateStepIncrement(1);
            break;

        case 1:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AwaitAnimEnd, chara, 0, true);
            break;

        default:
            SysWork_StateStepIncrement(0);
            break;
    }
}

void Event_BgTextureLoadFadeIn(e_FsFile texFileIdx, q19_12 fadeTimestep) // 0x80086DA8
{
    switch (g_SysWork.sysStateSteps[1])
    {
        case 0:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 0, fadeTimestep, false);
            SysWork_StateStepIncrement(1);

        case 1:
            Event_BgTextureCmd(BgTextureCmd_Auto, texFileIdx, true);
            break;

        default:
            Event_ScreenFadeCmd(ScreenFadeCmd_Wait, true, 0, Q12(0.0f), false);
            break;
    }
}

void Event_BgTextureFadeIn(e_FsFile texFileIdx, q19_12 fadeTimestep0, q19_12 fadeTimestep1) // 0x80086E50
{
    switch (g_SysWork.sysStateSteps[1])
    {
        case 0:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 0, fadeTimestep0, false);
            SysWork_StateStepIncrement(1);

        case 1:
            Event_BgTextureCmd(BgTextureCmd_Auto, texFileIdx, true);
            break;

        case 2:
            Event_ScreenFadeCmd(ScreenFadeCmd_Wait, true, 0, Q12(0.0f), true);
            break;

        default:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, fadeTimestep1, false);
    }
}

void Event_BgTextureFadeOut(q19_12 fadeTimestepFromBlack, q19_12 fadeTimestepToBlack) // 0x80086F44
{
    if (g_SysWork.sysStateSteps[1] == 0)
    {
        Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
        Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, fadeTimestepToBlack, true);
        return;
    }

    Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, fadeTimestepFromBlack, false);
    SysWork_StateStepIncrement(0);
}

void Event_DisplayMapMsgWithSfx(s32 mapMsgIdx, e_SfxId sfxId, VECTOR3* sfxPos) // 0x80086FE8
{
    s32 i;

    if (!(g_SysWork.sysFlags & SysFlag_5))
    {
        // Run through NPCs.
        for (i = 0; i < ARRAY_SIZE(g_SysWork.npcs); i++)
        {
            if (NPC_CHARA(i).model.charaId >= Chara_Harry         &&
                NPC_CHARA(i).model.charaId <= CHARA_LAST_ENEMY_ID &&
                NPC_CHARA(i).health > Q12(0.0f))
            {
                break;
            }
        }

        if (i != ARRAY_SIZE(g_SysWork.npcs))
        {
            g_DeltaTime = Q12(0.0f);
        }
    }

    switch (g_SysWork.sysStateSteps[1])
    {
        case 0:
            g_MapOverlayHdr.playerControlFreeze();
            Sfx_WithFlagsPlay(sfxId, sfxPos, Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(1);

        case 1:
            Event_WaitTimer(Q12(0.2f), true);
            break;

        case 2:
            Event_DisplayMapMsg(false, mapMsgIdx, 0, 0, 0, true);
            break;

        default:
            g_MapOverlayHdr.playerControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;
    }
}

void Event_DisplayBgTexture(e_FsFile texFileIdx, q19_12 fadeTimestep0, q19_12 fadeTimestep1) // 0x8008716C
{
    switch (g_SysWork.sysStateSteps[1])
    {
        case 0:
            g_MapOverlayHdr.playerControlFreeze();
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 0, fadeTimestep0, false);
            SysWork_StateStepIncrement(1);

        case 1:
            Event_BgTextureCmd(BgTextureCmd_Auto, texFileIdx, true);
            break;

        case 2:
            Event_ScreenFadeCmd(ScreenFadeCmd_Wait, true, 0, Q12(0.0f), true);
            break;

        case 3:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, fadeTimestep1, true);
            break;

        case 4:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);

            if (Pad_ButtonClicked(Pad_BindingGet(enter) | Pad_BindingGet(cancel)))
            {
                SysWork_StateStepIncrement(1);
            }
            break;

        case 5:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, fadeTimestep1, true);
            break;

        default:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, fadeTimestep0, false);
            g_MapOverlayHdr.playerControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;
    }
}

void Event_DisplayMapMsgWithBg(e_FsFile texFileIdx, q19_12 fadeTimestep0, q19_12 fadeTimestep1, s32 mapMsgIdx) // 0x80087360
{
    // TODO: Name these.
    typedef enum _EventStates
    {
        EventStates_Initialize = 0,
        EventStates_1          = 1,
        EventStates_2          = 2,
        EventStates_3          = 3,
        EventStates_4          = 4,
        EventStates_5          = 5,
        EventStates_6          = 6
    } e_EventStates;

    switch (g_SysWork.sysStateSteps[1])
    {
        case EventStates_Initialize:
            // Freeze player control.
            g_MapOverlayHdr.playerControlFreeze();

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 0, fadeTimestep0, false);
            SysWork_StateStepIncrement(1);

        case EventStates_1:
            Event_BgTextureCmd(BgTextureCmd_Auto, texFileIdx, true);
            break;

        case EventStates_2:
            Event_ScreenFadeCmd(ScreenFadeCmd_Wait, true, 0, Q12(0.0f), true);
            break;

        case EventStates_3:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, fadeTimestep1, true);
            break;

        case EventStates_4:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_DisplayMapMsg(false, mapMsgIdx, 0, 0, 0, true);
            break;

        case EventStates_5:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, fadeTimestep1, true);
            break;

        default: // `EventStates_6`
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, fadeTimestep0, false);

            // Restore player control.
            g_MapOverlayHdr.playerControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;
    }
}

void Event_DisplayMapMsgWithDimmedBg(e_FsFile texFileIdx, q19_12 fadeTimestep0, q19_12 fadeTimestep1, s32 mapMsgIdx0, s32 mapMsgIdx1) // 0x80087540
{
    // TODO: Name these.
    typedef enum _EventStates
    {
        EventStates_Initialize = 0,
        EventStates_1          = 1,
        EventStates_2          = 2,
        EventStates_3          = 3,
        EventStates_4          = 4,
        EventStates_5          = 5,
        EventStates_6          = 6,
        EventStates_7          = 7
    } e_EventStates;

    switch (g_SysWork.sysStateSteps[1])
    {
        case EventStates_Initialize:
            // Freeze player control.
            g_MapOverlayHdr.playerControlFreeze();

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 0, fadeTimestep0, false);
            SysWork_StateStepIncrement(1);

        case EventStates_1:
            Event_BgTextureCmd(BgTextureCmd_Auto, texFileIdx, true);
            break;

        case EventStates_2:
            Event_ScreenFadeCmd(ScreenFadeCmd_Wait, true, 0, Q12(0.0f), true);
            break;

        case EventStates_3:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, fadeTimestep1, true);
            break;

        case EventStates_4:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);

            if (mapMsgIdx0 != MapMsgReturnCode_None)
            {
                Event_DisplayMapMsg(false, mapMsgIdx0, 0, 0, 0, true);
                break;
            }

            // Check for "continue" input.
            if (Pad_ButtonClicked(Pad_BindingGet(enter) | Pad_BindingGet(cancel)))
            {
                SysWork_StateStepIncrement(1);
            }
            break;

        case EventStates_5:
            g_Screen_BackgroundImgGamma = Q8(6.0f / 32.0f);

            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_DisplayMapMsg(false, mapMsgIdx1, 0, 0, 0, true);
            break;

        case EventStates_6:
            g_Screen_BackgroundImgGamma = Q8(6.0f / 32.0f);

            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, fadeTimestep1, true);
            break;

        default: // `EventStates_7`
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, fadeTimestep0, false);
            g_MapOverlayHdr.playerControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;
    }
}

void Event_ItemTake(e_InvItemId itemId, s32 itemCount, e_EventFlag eventFlagIdx, s32 mapMsgIdx) // 0x800877B8
{
    typedef enum _EventState
    {
        EventState_Initialize      = 0,
        EventState_LoadItemModel   = 1,
        EventState_SelectionDialog = 2,
        EventState_TakeItem        = 3,
        EventState_DontTakeItem    = 4
    } e_EventState;

    s32 i            = itemId;
    s32 mapMsgIdxCpy = mapMsgIdx;

    if (!(g_SysWork.sysFlags & SysFlag_5))
    {
        // Run through NPCs.
        for (i = 0; i < ARRAY_SIZE(g_SysWork.npcs); i++)
        {
            if (NPC_CHARA(i).model.charaId >= Chara_Harry         &&
                NPC_CHARA(i).model.charaId <= CHARA_LAST_ENEMY_ID &&
                NPC_CHARA(i).health > Q12(0.0f))
            {
                break;
            }
        }

        if (i != ARRAY_SIZE(g_SysWork.npcs))
        {
            g_DeltaTime = Q12(0.0f);
        }
    }

    switch (g_SysWork.sysStateSteps[1])
    {
        case EventState_Initialize:
            // Freeze player and start loading item model.
            g_MapOverlayHdr.playerControlFreeze();
            Event_InvItemCmd(InvItemCmd_QueueLoad, itemId, 0, false);

            SysWork_StateStepIncrement(1);

        case EventState_LoadItemModel:
            Event_InvItemCmd(InvItemCmd_AwaitLoad, itemId, 0, true);
            break;

        case EventState_SelectionDialog:
            // `Gfx_PickupItemAnimate` increases model scale and returns `false`, then starts rotating it and returns `true`.
            if (Gfx_PickupItemAnimate(itemId))
            {
                Event_DisplayMapMsg(true, mapMsgIdxCpy, 3, NO_VALUE, 0, true); // 3 is "Yes", `NO_VALUE` is "No".
            }

            // Flag pickup item as collected.
            Savegame_EventFlagSet(eventFlagIdx);
            break;

        case EventState_TakeItem:
            // "Yes" selected.
            Event_InvItemCmd(InvItemCmd_AddItem, itemId, itemCount, false);
            SysWork_StateStepIncrement(1);

        default: // `EventState_DontTakeItem`
            // Flag pickup item as uncollected. Selecting 'No' sets `field_10` to `NO_VALUE`.
            if (g_SysWork.sysStateSteps[1] == NO_VALUE)
            {
                Savegame_EventFlagClear(eventFlagIdx);
            }

            // Restore player control.
            g_MapOverlayHdr.playerControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;
    }
}

void Event_CommonItemTake(u32 pickupType, e_EventFlag eventFlagIdx) // 0x800879FC
{
    #define EASY_DIFFICULTY_AMMO_COUNT_MULT_MIN 2

    s32 ammoCountMult;

    // Compute ammo count multiplier.
    ammoCountMult = g_GameWork.config.extraBulletAdjust + 1;
    if (g_SavegamePtr->gameDifficulty == GameDifficulty_Easy)
    {
        ammoCountMult = CLAMP(ammoCountMult, EASY_DIFFICULTY_AMMO_COUNT_MULT_MIN, ammoCountMult);
    }

    // Handle item pickup.
    switch (pickupType)
    {
        case CommonPickupItemId_FirstAidKit:
            Event_ItemTake(InvItemId_FirstAidKit, DEFAULT_PICKUP_ITEM_COUNT, eventFlagIdx, MapMsgIdx_FirstAidSelect);
            break;

        case CommonPickupItemId_HealthDrink:
            Event_ItemTake(InvItemId_HealthDrink, DEFAULT_PICKUP_ITEM_COUNT, eventFlagIdx, MapMsgIdx_HealthDrinkSelect);
            break;

        case CommonPickupItemId_Ampoule:
            Event_ItemTake(InvItemId_Ampoule, DEFAULT_PICKUP_ITEM_COUNT, eventFlagIdx, MapMsgIdx_AmpouleSelect);
            break;

        case CommonPickupItemId_HandgunBullets:
            Event_ItemTake(InvItemId_HandgunBullets, ammoCountMult * HANDGUN_AMMO_PICKUP_ITEM_COUNT, eventFlagIdx, MapMsgIdx_HandgunAmmoSelect);
            break;

        case CommonPickupItemId_ShotgunShells:
            Event_ItemTake(InvItemId_ShotgunShells, ammoCountMult * SHOTGUN_AMMO_PICKUP_ITEM_COUNT, eventFlagIdx, MapMsgIdx_ShotgunAmmoSelect);
            break;

        case CommonPickupItemId_RifleShells:
            Event_ItemTake(InvItemId_RifleShells, ammoCountMult * RIFLE_AMMO_PICKUP_ITEM_COUNT, eventFlagIdx, MapMsgIdx_RifleAmmoSelect);
            break;
    }

    #undef EASY_DIFFICULTY_AMMO_COUNT_MULT_MIN
}

void Event_PaperMapTake(s32 paperMapFlagIdx, e_EventFlag eventFlagIdx, s32 mapMsgIdx) // 0x80087AF4
{
    static const RECT RECT = {
        SCREEN_WIDTH, 256,
        SCREEN_WIDTH / 2, SCREEN_HEIGHT
    };

    g_DeltaTime = Q12(0.0f);

    switch (g_SysWork.sysStateSteps[1])
    {
        case 0:
            // Freeze player control.
            g_MapOverlayHdr.playerControlFreeze();

            Fs_QueueStartSeek(FILE_TIM_MP_0TOWN_TIM + g_PaperMapFileIdxs[paperMapFlagIdx]);
            SysWork_StateStepIncrement(1);

        case 1:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), true);
            break;

        case 2:
            DrawSync(SyncMode_Wait);
            StoreImage(&RECT, IMAGE_BUFFER);
            DrawSync(SyncMode_Wait);
            Fs_QueueStartReadTim(FILE_TIM_MP_0TOWN_TIM + g_PaperMapFileIdxs[paperMapFlagIdx], FS_BUFFER_2, &g_PaperMapImg);
            Screen_Init(SCREEN_WIDTH, true);

            g_IntervalVBlanks = 1;

            GsSwapDispBuff();
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            Fs_QueueWaitForEmpty();

            SysWork_StateStepIncrement(1);

        case 3:
            g_Screen_BackgroundImgGamma = Q8(11.0f / 32.0f);

            Screen_BackgroundImgDraw(&g_PaperMapImg);
            Event_DisplayMapMsg(true, mapMsgIdx, 4, 5, 0, true); // 4 is "No", 5 is "Yes".
            break;

        case 4:
            SET_FLAG(g_SavegamePtr->paperMapFlags, paperMapFlagIdx);

            switch (paperMapFlagIdx)
            {
                case PaperMapIdx_FogSchool1F:
                    SET_FLAG(g_SavegamePtr->paperMapFlags, PaperMapIdx_FogSchoolBF);
                    SET_FLAG(g_SavegamePtr->paperMapFlags, PaperMapIdx_FogSchool2F);
                    SET_FLAG(g_SavegamePtr->paperMapFlags, PaperMapIdx_FogSchoolRF);
                    SET_FLAG(g_SavegamePtr->paperMapFlags, PaperMapIdx_AltSchoolBF);
                    SET_FLAG(g_SavegamePtr->paperMapFlags, PaperMapIdx_AltSchool1F);
                    SET_FLAG(g_SavegamePtr->paperMapFlags, PaperMapIdx_AltSchool2F);
                    SET_FLAG(g_SavegamePtr->paperMapFlags, PaperMapIdx_AltSchoolRF);
                    break;

                case PaperMapIdx_FogHospital1F:
                    SET_FLAG(g_SavegamePtr->paperMapFlags, PaperMapIdx_FogHospital2F);
                    SET_FLAG(g_SavegamePtr->paperMapFlags, PaperMapIdx_FogHospital3F);
                    SET_FLAG(g_SavegamePtr->paperMapFlags, PaperMapIdx_AltHospital1F);
                    SET_FLAG(g_SavegamePtr->paperMapFlags, PaperMapIdx_AltHospital2F);
                    SET_FLAG(g_SavegamePtr->paperMapFlags, PaperMapIdx_AltHospital3F);
                    break;

                case PaperMapIdx_FogHospitalBF:
                    SET_FLAG(g_SavegamePtr->paperMapFlags, PaperMapIdx_AltHospitalBF);
                    break;

                case PaperMapIdx_FogSewer1F:
                    SET_FLAG(g_SavegamePtr->paperMapFlags, PaperMapIdx_FogSewer2F);
                    break;

                case PaperMapIdx_FogCentralTown:
                    SET_FLAG(g_SavegamePtr->paperMapFlags, PaperMapIdx_AltCentralTown);
                    break;
            }

            Savegame_EventFlagSet(eventFlagIdx);
            SysWork_StateStepIncrement(1);

        case 5:
            g_Screen_BackgroundImgGamma = Q8(11.0f / 32.0f);

            Screen_BackgroundImgDraw(&g_PaperMapImg);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), true);
            break;

        default:
            LoadImage(&RECT, IMAGE_BUFFER);
            DrawSync(SyncMode_Wait);
            Screen_Init(SCREEN_WIDTH, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);

            // Restore player control.
            g_MapOverlayHdr.playerControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;
    }
}
