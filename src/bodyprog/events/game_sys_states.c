#include "game.h"

#include <psyq/libetc.h>
#include <psyq/libpad.h>
#include <psyq/strings.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/events/bodyprog_data_800A99B4.h"
#include "bodyprog/events/collision_trigger.h"
#include "bodyprog/events/events_main.h"
#include "bodyprog/events/npc_main.h"
#include "bodyprog/events/radio.h"
#include "bodyprog/demo.h"
#include "bodyprog/gfx/map_effects.h"
#include "bodyprog/item_screens.h"
#include "bodyprog/math/math.h"
#include "bodyprog/memcard.h"
#include "bodyprog/screen/screen_data.h"
#include "bodyprog/screen/screen_draw.h"
#include "bodyprog/text/text_draw.h"
#include "bodyprog/player.h"
#include "bodyprog/ranking.h"
#include "bodyprog/sound/sound_system.h"
#include "main/fsqueue.h"
#include "main/rng.h"

#ifndef PAD_HACK_IGNORE
    s8  __pad_bss_800BCD81[3];
    s32 __pad_bss_800BCD88[2];
    s32 __pad_bss_800BCD94[5];
    s32 __pad_bss_800BCDD0;
    s8  __pad_bss_800BCDD5[3];
#endif

// ========================================
// STATIC VARIABLES
// ========================================

/** @brief Game system state functions. Used with `e_SysState`. */
static void (*g_SysStateFuncs[])(void) = {
    SysState_Gameplay_Update,
    SysState_OptionsMenu_Update,
    SysState_StatusMenu_Update,
    SysState_MapScreen_Update,
    SysState_Fmv_Update,
    SysState_LoadArea_Update,
    SysState_LoadArea_Update,
    SysState_ReadMessage_Update,
    SysState_SaveMenu_Update,
    SysState_SaveMenu_Update,
    SysState_EventCallback_Update,
    SysState_EventSetFlag_Update,
    SysState_EventPlaySound_Update,
    SysState_GameOver_Update,
    SysState_GamePaused_Update
};

/** Used to store the previous delta time state of the delta timer. There are some instances where 2D backgrounds
 * are drawn using `g_DeltaTimeRaw` while `g_DeltaTime` is stopped.
 */
static q19_12 g_DeltaTimeCpy;

// ========================================
// GLOBAL VARIABLES
// ========================================

s_EventData* g_ItemEvents[];
s_RadioNoise g_RadioNoise[2];
s_MapPoint2d g_MapPoint;
s32          g_ItemTriggerItemIds[5];
u8           g_MapAreaLoadCounter;
s_EventData*  g_MapEventData;

void GameState_InGame_Update(void) // 0x80038BD4
{
    s_SubCharacter* player;

    Demo_DemoRandSeedBackup();

    switch (g_GameWork.gameStateSteps[0])
    {
        case 0:
            ScreenFade_Start(true, true, false);
            g_ScreenFadeTimestep         = Q12(3.0f);
            g_GameWork.gameStateSteps[0] = 1;

        case 1:
            DrawSync(SyncMode_Wait);
            Game_RadioNoiseReset();
            Game_MapRoomIdxUpdate();
            func_800892A4(1);

            g_IntervalVBlanks = 2;
            g_GameWork.gameStateSteps[0]++;
            g_SysWork.bgmStatusFlags |= BgmStatusFlag_6;
            break;
    }

    if (g_SysWork.sysState != SysState_Gameplay && g_SysWork.playerWork.player.health <= Q12(0.0f))
    {
        SysWork_StateSetNext(SysState_Gameplay);
    }

    if (g_DeltaTime != Q12(0.0f))
    {
        g_DeltaTimeCpy = g_DeltaTime;
    }
    else
    {
        g_DeltaTimeCpy = g_DeltaTimeRaw;
    }

    // Run system state.
    if (g_SysWork.sysState == SysState_Gameplay)
    {
        g_SysWork.isMgsStringSet = false;
        g_SysStateFuncs[SysState_Gameplay]();
    }
    else
    {
        g_DeltaTime = Q12(0.0f);
        g_SysStateFuncs[g_SysWork.sysState]();

        if (g_SysWork.sysState == SysState_Gameplay)
        {
            Event_Update(true);

            if (g_MapEventSysState != SysState_Invalid)
            {
                SysWork_StateSetNext(g_MapEventSysState);
            }
        }
    }
    Demo_DemoRandSeedRestore();

    g_IsLoadingFinished = ScreenFade_IsFinished() && Fs_QueueChunksLoad();

    // Update world objects if not paused.
    if (!(g_SysWork.bgmStatusFlags & BgmStatusFlag_Pause) &&
        g_MapOverlayHdr.updateWorldObjects != NULL)
    {
        g_MapOverlayHdr.updateWorldObjects();
    }

    Screen_CutsceneCameraStateUpdate();
    Bgm_Update(false);
    Demo_DemoRandSeedRestore();
    Demo_DemoRandSeedRestore();

    // Update world scene if not paused.
    if (!(g_SysWork.bgmStatusFlags & BgmStatusFlag_Pause))
    {
        World_NearbyPlayerCollisionTriggersGet();
        vcMoveAndSetCamera(false, false, false, false, false, false, false, false);

        if (g_MapOverlayHdr.func_44 != NULL)
        {
            g_MapOverlayHdr.func_44();
        }

        Demo_DemoRandSeedRestore();

        player = &g_SysWork.playerWork.player;
        Player_Update(player, FS_BUFFER_0, g_SysWork.playerBoneCoords);

        Demo_DemoRandSeedRestore();
        Gfx_EffectsUpdate();

        if (g_SavegamePtr->mapIdx != MapIdx_MAP7_S03)
        {
            g_MapOverlayHdr.particleSystemUpdate(0, g_SavegamePtr->mapIdx, 1);
        }

        Demo_DemoRandSeedRestore();

        if (player->model.anim.flags & AnimFlag_Visible)
        {
            WorldGfx_CharaDraw(Chara_Harry, g_SysWork.playerBoneCoords, 1, g_SysWork.playerWork.player.timer_C6, 0);
            Chara_Flag8Clear(&g_SysWork.playerWork.player);
            Player_CombatUpdate(&g_SysWork.playerWork, g_SysWork.playerBoneCoords);
            func_8008A3AC(&g_SysWork.playerWork.player);
        }

        Demo_DemoRandSeedRestore();
        Game_NpcRoomInitSpawn(true);
        Game_NpcUpdate();
        func_8005E89C();
        WorldGfx_CloseRangeChunksInit();
        WorldGfx_Draw(1);
        Demo_DemoRandSeedAdvance();
    }
}

void SysState_Gameplay_Update(void) // 0x80038BD4
{
    s_SubCharacter* player;

    player = &g_SysWork.playerWork.player;

    Event_Update(player->attackReceived != NO_VALUE);
    Game_MapRoomIdxUpdate();

    switch (FP_ROUND_SCALED(player->health, 10, Q12_SHIFT))
    {
        case 0:
            func_800892A4(17);
            break;

        case 1:
        case 2:
            func_800892A4(16);
            break;

        case 3:
            func_800892A4(15);
            break;

        case 4:
            func_800892A4(14);
            break;

        case 5:
            func_800892A4(13);
            break;

        case 6:
            func_800892A4(12);
            break;
    }

    if (g_SysWork.playerWork.player.health <= Q12(0.0f))
    {
        return;
    }

    if (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.light &&
        g_SysWork.field_2388.field_154.effectsInfo.flags.field_00[0] & SpecialEnvEventFlags_FlashlightAllowed)
    {
        Game_FlashlightToggle();
    }

    if (g_MapEventSysState != SysState_Invalid)
    {
        SysWork_StateSetNext(g_MapEventSysState);
    }
    else if (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.pause)
    {
        SysWork_StateSetNext(SysState_GamePaused);
    }
    else if (Player_IsAttacking() == true)
    {
        return;
    }
    else if (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.item)
    {
        SysWork_StateSetNext(SysState_StatusMenu);
    }
    else if (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.map)
    {
        SysWork_StateSetNext(SysState_MapScreen);
        g_SysWork.isMgsStringSet = false;
    }
    else if (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.option)
    {
        SysWork_StateSetNext(SysState_OptionsMenu);
    }

    if (g_SysWork.sysState == SysState_OptionsMenu ||
        g_SysWork.sysState == SysState_StatusMenu ||
        g_SysWork.sysState == SysState_MapScreen)
    {
        g_SysWork.sysFlags |= SysFlag_MenuActive;
    }
    else if (ScreenFade_IsNone())
    {
        g_SysWork.sysFlags &= ~SysFlag_MenuActive;
    }
}

void SysState_GamePaused_Update(void) // 0x800391E8
{
    static s32 D_800A9A68 = 0;

    D_800A9A68 += g_DeltaTimeRaw;
    if (!((D_800A9A68 >> 11) & (1 << 0)))
    {
#if VERSION_REGION_IS(NTSCJ)
        Gfx_StringPositionSet(131, 104);
        Gfx_StringDraw("\7PAUSE", DEFAULT_MAP_MESSAGE_LENGTH);
#else
        Gfx_StringPositionSet(125, 104);
        Gfx_StringDraw("\7PAUSED", DEFAULT_MAP_MESSAGE_LENGTH);
#endif
    }

    func_80091380();
    Game_TimerUpdate();

    if (g_SysWork.sysStateSteps[0] == 0)
    {
        SD_Call(3);
        g_SysWork.sysStateSteps[0]++;
    }

    // Debug button combo to bring up save screen from pause screen.
    // DPad-Left + L2 + L1 + LS-Left + RS-Left + L3
    if ((g_Controller0->buttonFlags.held == (ControllerFlag_L3               |
                                             ControllerFlag_DpadLeft         |
                                             ControllerFlag_L2               |
                                             ControllerFlag_L1               |
                                             ControllerFlag_LStickLowLeft    |
                                             ControllerFlag_RStickLowLeft    |
                                             ControllerFlag_LStickHighLeft)) &&
        (g_Controller0->buttonFlags.clicked & ControllerFlag_L3))
    {
        D_800A9A68 = 0;
        SD_Call(4);
        g_MapEventParam = 0;
        SysWork_StateSetNext(SysState_SaveMenu1);
        return;
    }

    if (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.pause)
    {
        D_800A9A68 = 0;

        SD_Call(4);
        SysWork_StateSetNext(SysState_Gameplay);
    }
}

void SysState_OptionsMenu_Update(void) // 0x80039344
{
    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            ScreenFade_Start(true, false, false);
            g_ScreenFadeTimestep       = Q12(0.0f);
            g_SysWork.sysStateSteps[0] = 1;

        case 1:
            if (WorldGfx_ChunksInitCheck())
            {
                SD_Call(19);
                GameFs_OptionBinLoad();

                g_SysWork.sysStateSteps[0]++;
            }
            break;
    }

    if (g_IsLoadingFinished)
    {
        Game_StateSetNext(GameState_OptionScreen);
    }
}

void func_8003943C(void) // 0x8003943C
{
    s32 roundedVal0;
    s32 roundedVal1;
    s32 val0;
    s32 val1;

    #define isRockDrillAttack (g_SysWork.playerCombat.weaponAttack == WEAPON_ATTACK(EquippedWeaponId_RockDrill, AttackInputType_Tap))

    func_8008B3E4(0);

    if (g_SysWork.field_275C > Q12(256.0f))
    {
        val0        = g_SysWork.field_275C - Q12(256.0f);
        roundedVal0 = FP_ROUND_TO_ZERO(val0, Q12_SHIFT);
        func_8008B438(!isRockDrillAttack, roundedVal0, 0);

        if (isRockDrillAttack)
        {
            val1        = g_SysWork.field_2764 - Q12(256.0f);
            roundedVal1 = FP_ROUND_TO_ZERO(val1, Q12_SHIFT);
            func_8008B40C(roundedVal1, 0);
        }
    }
    else
    {
        func_8008B438(!isRockDrillAttack, 0, 0);

        if (isRockDrillAttack)
        {
            func_8008B40C(0, 0);
        }
    }

    switch (g_SavegamePtr->mapIdx)
    {
        case MapIdx_MAP0_S01:
        case MapIdx_MAP0_S02:
        case MapIdx_MAP1_S00:
        case MapIdx_MAP1_S01:
        case MapIdx_MAP1_S02:
        case MapIdx_MAP1_S03:
        case MapIdx_MAP1_S04:
        case MapIdx_MAP1_S05:
        case MapIdx_MAP1_S06:
        case MapIdx_MAP2_S00:
        case MapIdx_MAP2_S01:
        case MapIdx_MAP2_S02:
        case MapIdx_MAP2_S03:
        case MapIdx_MAP2_S04:
        case MapIdx_MAP3_S00:
        case MapIdx_MAP3_S01:
        case MapIdx_MAP3_S02:
        case MapIdx_MAP3_S04:
        case MapIdx_MAP3_S05:
        case MapIdx_MAP3_S06:
        case MapIdx_MAP4_S00:
        case MapIdx_MAP4_S01:
        case MapIdx_MAP4_S02:
        case MapIdx_MAP4_S03:
        case MapIdx_MAP4_S04:
        case MapIdx_MAP4_S05:
        case MapIdx_MAP4_S06:
        case MapIdx_MAP5_S00:
        case MapIdx_MAP5_S01:
        case MapIdx_MAP5_S02:
        case MapIdx_MAP5_S03:
        case MapIdx_MAP6_S00:
        case MapIdx_MAP6_S01:
        case MapIdx_MAP6_S02:
        case MapIdx_MAP6_S03:
        case MapIdx_MAP6_S04:
        case MapIdx_MAP6_S05:
        case MapIdx_MAP7_S00:
        case MapIdx_MAP7_S01:
        case MapIdx_MAP7_S02:
            break;

        case MapIdx_MAP3_S03:
            Sd_SfxStop(Sfx_Unk1525);
            Sd_SfxStop(Sfx_Unk1527);
            break;

        case MapIdx_MAP0_S00:
            Sd_SfxStop(Sfx_Unk1358);
            break;
    }

    #undef isRockDrillAttack
}

void SysState_StatusMenu_Update(void) // 0x80039568
{
    Game_StateSetNext(GameState_LoadStatusScreen);
}

void GameState_LoadStatusScreen_Update(void) // 0x800395C0
{
    s_Savegame* save;

    if (g_GameWork.gameStateSteps[0] == 0)
    {
        DrawSync(SyncMode_Wait);
        g_IntervalVBlanks = 1;
        ScreenFade_Reset();

        func_8003943C();

        if (Sd_AudioStreamingCheck() != AudioStreamingState_None)
        {
            SD_Call(19);
        }

        save = g_SavegamePtr;
        func_800540A4(save->mapIdx);
        GameFs_MapItemsTextureLoad(save->mapIdx);

        g_GameWork.gameStateSteps[0]++;
    }

    Screen_BackgroundMotionBlur(SyncMode_Wait2);

    if (Fs_QueueChunksLoad())
    {
        Game_StateSetNext(GameState_InventoryScreen);
    }
}

void SysState_MapScreen_Update(void) // 0x800396D4
{
    if (!HAS_PAPER_MAP(g_SavegamePtr->paperMapIdx))
    {
        if (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.map ||
            Gfx_MapMsg_Draw(MapMsgIdx_NoMap) > MapMsgState_Idle)
        {
            SysWork_StateSetNext(SysState_Gameplay);
        }
    }
    else if ((g_SysWork.field_2388.field_154.effectsInfo.flags.field_00[0] & SpecialEnvEventFlags_FlashlightAllowed) && !g_SysWork.field_2388.isFlashlightOn &&
             ((g_SysWork.field_2388.field_1C[0].effectsInfo.flags.field_00[0] & SpecialEnvEventFlags_DarkEnvironment) ||
              (g_SysWork.field_2388.field_1C[1].effectsInfo.flags.field_00[0] & SpecialEnvEventFlags_DarkEnvironment)))
    {
        if (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.map ||
            Gfx_MapMsg_Draw(MapMsgIdx_TooDarkForMap) > MapMsgState_Idle)
        {
            SysWork_StateSetNext(SysState_Gameplay);
        }
    }
    else
    {
        if (g_SysWork.sysStateSteps[0] == 0)
        {
            if (g_PaperMapMarkingFileIdxs[g_SavegamePtr->paperMapIdx] != NO_VALUE)
            {
                Fs_QueueStartReadTim(FILE_TIM_MR_0TOWN_TIM + g_PaperMapMarkingFileIdxs[g_SavegamePtr->paperMapIdx], FS_BUFFER_1, &g_PaperMapMarkingAtlasImg);
            }

            Fs_QueueStartSeek(FILE_TIM_MP_0TOWN_TIM + g_PaperMapFileIdxs[g_SavegamePtr->paperMapIdx]);

            ScreenFade_Start(true, false, false);
            g_ScreenFadeTimestep = Q12(0.0f);
            g_SysWork.sysStateSteps[0]++;
        }

        if (g_IsLoadingFinished)
        {
            Game_StateSetNext(GameState_PaperMapScreen);
        }
    }
}

void GameState_LoadMapScreen_Update(void) // 0x8003991C
{
    if (g_GameWork.gameStateSteps[0] == 0)
    {
        DrawSync(SyncMode_Wait);
        g_IntervalVBlanks = 1;

        func_8003943C();
        func_80066E40();

        if (g_PaperMapMarkingFileIdxs[g_SavegamePtr->paperMapIdx] != NO_VALUE)
        {
            Fs_QueueStartReadTim(FILE_TIM_MR_0TOWN_TIM + g_PaperMapMarkingFileIdxs[g_SavegamePtr->paperMapIdx], FS_BUFFER_1, &g_PaperMapMarkingAtlasImg);
        }

        Fs_QueueStartReadTim(FILE_TIM_MP_0TOWN_TIM + g_PaperMapFileIdxs[g_SavegamePtr->paperMapIdx], FS_BUFFER_2, &g_PaperMapImg);
        g_GameWork.gameStateSteps[0]++;
    }

    Screen_BackgroundMotionBlur(SyncMode_Wait2);

    if (Fs_QueueChunksLoad())
    {
        Game_StateSetNext(GameState_PaperMapScreen);
    }
}

void SysState_Fmv_Update(void) // 0x80039A58
{
    #define BASE_AUDIO_FILE_IDX FILE_XA_ZC_14392

    static RECT D_800A9A6C = { 320, 256, 160, 240 };

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            ScreenFade_Start(false, false, false);
            g_IsLoadingFinished                 = false;
            g_SysWork.sysStateSteps[0] = 1;

        case 1:
            if (WorldGfx_ChunksInitCheck())
            {
                GameFs_StreamBinLoad();
                g_SysWork.sysStateSteps[0]++;
            }
            break;
    }

    if (!g_IsLoadingFinished)
    {
        return;
    }

    // Copy framebuffer into `IMAGE_BUFFER_0` before movie playback.
    DrawSync(SyncMode_Wait);
    StoreImage(&D_800A9A6C, (u32*)IMAGE_BUFFER_0);
    DrawSync(SyncMode_Wait);

    func_800892A4(0);
    func_80089128();

    // Start playing movie. File to play is based on file ID `BASE_AUDIO_FILE_IDX - g_MapEventParam`.
    // Blocks until movie has finished playback or user has skipped it.
    open_main(BASE_AUDIO_FILE_IDX - g_MapEventParam, g_FileTable[BASE_AUDIO_FILE_IDX - g_MapEventParam].blockCount);

    func_800892A4(1);

    // Restore copied framebuffer from `IMAGE_BUFFER_0`.
    GsSwapDispBuff();
    LoadImage(&D_800A9A6C, (u32*)IMAGE_BUFFER_0);
    DrawSync(SyncMode_Wait);

    // Set savegame flag based on `g_MapEventData->completeFlag` flag ID.
    Savegame_EventFlagSetAlt(g_MapEventData->completeFlag);

    // Return to gameplay.
    Game_StateSetNext(GameState_InGame);

    // If flag is set, returns to `GameState_InGame` with `gameStateSteps[0]` = 1.
    if (g_MapEventData->transitionFlags & AreaTransitionFlag_SkipFadeIn)
    {
        g_GameWork.gameStateSteps[0] = 1;
    }
}

void SysState_LoadArea_Update(void) // 0x80039C40
{
    q20_12        offsetZ;
    s_MapPoint2d* mapPoint;

    g_SysWork.unused_229C         = 0;
    g_SysWork.loadingScreenIdx    = g_MapPoint.loadingScreenId;
    g_SysWork.sfxPairIdx          = g_MapEventData->sfxPairIdx;
    g_SysWork.areaTransitionFlags = g_MapEventData->transitionFlags;

    SD_Call(SFX_PAIRS[g_SysWork.sfxPairIdx].sfx_0);

    if (g_SysWork.sfxPairIdx == SfxPairIdx_7)
    {
        g_MapAreaLoadCounter = 0;
        g_SysWork.sysFlags  |= SysFlag_LoadActive;
    }

    g_MapPoint = g_MapOverlayHdr.mapPoints[g_MapEventData->eventParam];
    if (g_MapPoint.triggerParam1 == 1)
    {
        mapPoint              = &g_MapOverlayHdr.mapPoints[g_MapEventData->mapPointIdx];
        offsetZ               = g_SysWork.playerWork.player.position.vz - mapPoint->positionZ;
        g_MapPoint.positionX += g_SysWork.playerWork.player.position.vx - mapPoint->positionX;
        g_MapPoint.positionZ += offsetZ;
    }

    if (g_SysWork.sysState == SysState_LoadOverlay)
    {
        g_SysWork.processFlags = ProcessFlag_OverlayTransition;
        g_SavegamePtr->mapIdx  = g_MapEventData->mapIdx;
        GameBoot_MapLoad(g_SavegamePtr->mapIdx);
    }
    else
    {
        g_SysWork.processFlags = ProcessFlag_RoomTransition;
        Bgm_SongChange(g_MapEventData->mapIdx);

        if (g_MapOverlayHdr.mapPoints[g_MapEventData->eventParam].field_4_5 != 0)
        {
            g_SysWork.field_2349 = g_MapOverlayHdr.mapPoints[g_MapEventData->eventParam].field_4_5 - 1;
        }
    }

    Savegame_EventFlagSetAlt(g_MapEventData->completeFlag);

    if (g_MapEventData->setCameraRail)
    {
        g_SysWork.sysFlags |= SysFlag_SetCameraRail;
    }
    else
    {
        g_SysWork.sysFlags &= ~SysFlag_SetCameraRail;
    }

    g_SysWork.bgmStatusFlags |= BgmStatusFlag_Pause;
    Game_StateSetNext(GameState_MainLoadScreen);
    Screen_BackgroundMotionBlur(SyncMode_Immediate);
}

void AreaLoad_UpdatePlayerPosition(void) // 0x80039F30
{
    Chara_PositionSet(&g_MapPoint);
}

void AreaLoad_TransitionSound(void) // 0x80039F54
{
    SD_Call(SFX_PAIRS[g_SysWork.sfxPairIdx].sfx_2);
}

s8 AreaLoad_TransitionFlags(void) // 0x80039F90
{
    if (g_SysWork.processFlags & (ProcessFlag_RoomTransition | ProcessFlag_OverlayTransition))
    {
        return g_SysWork.areaTransitionFlags;
    }

    return AreaTransitionFlag_None;
}

void SysState_ReadMessage_Update(void) // 0x80039FB8
{
    s32 i;
    void (**unfreezePlayerFunc)(bool);

    // When `SysState_ReadMessage_Update` is called, the game world freezes.
    // The following conditions unfreeze:
    // - A specific event related flag is disabled.
    // - A specific camera related flag is disabled.
    // - All enemies are dead.
    if (!(g_MapEventData->transitionFlags & AreaTransitionFlag_UnfreezeWorld) &&
        !(g_SysWork.sysFlags & SysFlag_5))
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

        if (i == ARRAY_SIZE(g_SysWork.npcs))
        {
            g_DeltaTime = g_DeltaTimeCpy;
        }
    }
    else
    {
        g_DeltaTime = g_DeltaTimeCpy;
    }

    if (g_SysWork.isMgsStringSet == false)
    {
        g_MapOverlayHdr.playerControlFreeze();
    }

    switch (Gfx_MapMsg_Draw(g_MapEventParam))
    {
        case MapMsgState_Finish:
            break;

        case MapMsgState_Idle:
            break;

        case MapMsgState_SelectEntry0:
            Savegame_EventFlagSetAlt(g_MapEventData->completeFlag);

            unfreezePlayerFunc = &g_MapOverlayHdr.playerControlUnfreeze;
            SysWork_StateSetNext(SysState_Gameplay);
            (*unfreezePlayerFunc)(false);
            break;
    }
}

void SysWork_SavegameUpdatePlayer(void) // 0x8003A120
{
    s_Savegame* save;

    save = g_SavegamePtr;

    save->locationId      = g_MapEventParam;
    save->playerPositionX = g_SysWork.playerWork.player.position.vx;
    save->playerPositionZ = g_SysWork.playerWork.player.position.vz;
    save->playerRotationY = g_SysWork.playerWork.player.rotation.vy;
    save->playerHealth    = g_SysWork.playerWork.player.health;
}

void func_8003A16C(void) // 0x8003A16C
{
    if (!(g_SysWork.sysFlags & SysFlag_DemoActive))
    {
        // Update `savegame` with player info.
        SysWork_SavegameUpdatePlayer();

        g_GameWork.autosave = g_GameWork.savegame;
    }
}

void SysWork_SavegameReadPlayer(void) // 0x8003A1F4
{
    g_SysWork.playerWork.player.position.vx = g_SavegamePtr->playerPositionX;
    g_SysWork.playerWork.player.position.vz = g_SavegamePtr->playerPositionZ;
    g_SysWork.playerWork.player.rotation.vy = g_SavegamePtr->playerRotationY;
    g_SysWork.playerWork.player.health      = g_SavegamePtr->playerHealth;
}

void SysState_SaveMenu_Update(void) // 0x8003A230
{
    MemCard_ElementsUpdate();

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            SysWork_SavegameUpdatePlayer();

            if (Savegame_EventFlagGet(EventFlag_SeenSaveScreen) ||
                g_SavegamePtr->locationId == SaveLocationId_NextFear || g_MapEventParam == 0)
            {
                GameFs_SaveLoadBinLoad();

                ScreenFade_Start(true, false, false);
                SysWork_StateStepIncrement(0);
            }
            else if (Gfx_MapMsg_Draw(MapMsgIdx_SaveGame) == MapMsgState_SelectEntry0)
            {
                Savegame_EventFlagSet(EventFlag_SeenSaveScreen);
                GameFs_SaveLoadBinLoad();
                ScreenFade_Start(true, false, false);
                SysWork_StateStepIncrement(0);
            }
            break;

        case 1:
            if (g_IsLoadingFinished)
            {
                ScreenFade_Start(true, true, false);
                func_8003943C();
                Game_StateSetNext(GameState_SaveScreen);
            }
            break;
    }
}

void SysState_EventCallback_Update(void) // 0x8003A3C8
{
    if (g_MapEventData->transitionFlags != AreaTransitionFlag_None)
    {
        Savegame_EventFlagSetAlt(g_MapEventData->completeFlag);
    }

    g_DeltaTime = g_DeltaTimeCpy;
    g_MapOverlayHdr.mapEventFuncs[g_MapEventParam]();
}

void SysState_EventSetFlag_Update(void) // 0x8003A460
{
    g_DeltaTime = g_DeltaTimeCpy;
    Savegame_EventFlagSetAlt(g_MapEventData->completeFlag);
    g_SysWork.sysState = SysState_Gameplay;
}

void SysState_EventPlaySound_Update(void) // 0x8003A4B4
{
    g_DeltaTime = g_DeltaTimeCpy;

    SD_Call(((u16)g_MapEventParam + Sfx_Base) & 0xFFFF);

    Savegame_EventFlagSetAlt(g_MapEventData->completeFlag);
    g_SysWork.sysState = SysState_Gameplay;
}

/** @brief Checks a flag state is `true` in the array of 16-bit flags.
 *
 * @param flags Flag array.
 * @param flagIdx Flag index.
 */
static inline s32 Flags16b_IsSet(const u16* flags, s32 flagIdx)
{
    // @bug `>> 5` divides `flagId` by 32 to get array index, but array contains 16-bit values.
    // Maybe copy-paste from `u32` version of func.
    return (flags[flagIdx >> 5] >> (flagIdx & 0x1F)) & (1 << 0);
}

void SysState_GameOver_Update(void) // 0x8003A52C
{
    #define TIP_COUNT 15

// PAL loads the tip into the loading screen image instead of a dedicated one.
#if VERSION_REGION_IS(PAL)
    #define TIP_IMG g_LoadingScreenImg
#else
    #define TIP_IMG g_DeathTipImg
#endif

    static u8 prevTipIdx;
    u16       seenTipIdxs[1];
    s32       tipIdx;
    s32       randTipVal;
    u16*      temp_a0;

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            g_MapOverlayHdr.playerControlFreeze();
            g_SysWork.sysStateStepData[0] = 0;

            if (g_GameWork.autosave.continueCount < 99)
            {
                g_GameWork.autosave.continueCount++;
            }

            MainMenu_SelectedOptionIdxReset();

            // If every game over tip has been seen, reset flag bits.
            if (g_GameWork.config.seenGameOverTips[0] == SHRT_MAX)
            {
                g_GameWork.config.seenGameOverTips[0] = 0;
            }

            randTipVal = 0;

            seenTipIdxs[0] = g_GameWork.config.seenGameOverTips[0];
            for (tipIdx = 0; tipIdx < TIP_COUNT; tipIdx++)
            {
                if (!Flags16b_IsSet(seenTipIdxs, tipIdx))
                {
                    if ((!(g_SysWork.field_2388.field_154.effectsInfo.flags.field_0 & (SpecialEnvEventFlags_DarkEnvironment | SpecialEnvEventFlags_FlashlightAllowed)) && (tipIdx - 13) >= 2u) ||
                        ( (g_SysWork.field_2388.field_154.effectsInfo.flags.field_0 & (SpecialEnvEventFlags_DarkEnvironment | SpecialEnvEventFlags_FlashlightAllowed)) && (tipIdx - 13) <  2u))
                    {
                        randTipVal += 3;
                    }
                    else
                    {
                        randTipVal++;
                    }
                }
            }

            randTipVal = Rng_GenerateInt(0, randTipVal - 1);

            // `randTipVal` seems to go unused after loop, gets checked during loop and can cause early exit,
            // thereby affecting what `tipIdx` will contain.
            for (tipIdx = 0; tipIdx < TIP_COUNT; tipIdx++)
            {
                if (!Flags16b_IsSet(seenTipIdxs, tipIdx))
                {
                    if ((!(g_SysWork.field_2388.field_154.effectsInfo.flags.field_0 & (SpecialEnvEventFlags_DarkEnvironment | SpecialEnvEventFlags_FlashlightAllowed)) && (tipIdx - 13) >= 2u) ||
                        ( (g_SysWork.field_2388.field_154.effectsInfo.flags.field_0 & (SpecialEnvEventFlags_DarkEnvironment | SpecialEnvEventFlags_FlashlightAllowed)) && (tipIdx - 13) <  2u))
                    {
                        if (randTipVal < 3)
                        {
                            break;
                        }

                        randTipVal -= 3;
                    }
                    else
                    {
                        if (randTipVal <= 0)
                        {
                            break;
                        }

                        randTipVal--;
                    }
                }
            }

            // Store current shown `tipIdx`, later `sysStateSteps == 7` will set it inside `seenGameOverTips`.
            prevTipIdx = tipIdx;

#if VERSION_REGION_IS(NTSC)
            Fs_QueueStartReadTim(FILE_TIM_TIPS_E01_TIM + tipIdx, FS_BUFFER_1, &TIP_IMG);
#elif VERSION_REGION_IS(NTSCJ)
            Fs_QueueStartReadTim(FILE_TIM_TIPS_J01_TIM + tipIdx, FS_BUFFER_1, &TIP_IMG);
#elif VERSION_REGION_IS(PAL)
            Fs_QueueStartReadTim((g_GameWorkConst->config.palLanguageId * TIP_COUNT) + FILE_TIM_TIPS_E01_TIM + tipIdx, FS_BUFFER_1, &TIP_IMG);
#endif
            SysWork_StateStepIncrement(0);

        case 1:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.5f), false);
            break;

        case 2:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.5f), false);
            SysWork_StateStepIncrement(0);

        case 3:
            Gfx_StringPositionSet(104, 104);
            Gfx_StringDraw("\7GAME_OVER", DEFAULT_MAP_MESSAGE_LENGTH);
            g_SysWork.sysStateStepData[0]++;

            if ((g_Controller0->buttonFlags.clicked & (g_GameWorkPtr->config.controllerConfig.enter |
                                                       g_GameWorkPtr->config.controllerConfig.cancel)) ||
                g_SysWork.sysStateStepData[0] > SECONDS_TO_FRAMES(4))
            {
                SysWork_StateStepIncrement(0);
            }
            break;

        case 4:
            Gfx_StringPositionSet(104, 104);
            Gfx_StringDraw("\7GAME_OVER", DEFAULT_MAP_MESSAGE_LENGTH);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(2.0f), false);
            break;

        case 5:
            if (g_SavegamePtr->gameDifficulty == GameDifficulty_Hard)
            {
                SysWork_StateStepReset();
                break;
            }
            else
            {
                Fs_QueueWaitForEmpty();
                Game_RadioSoundStop();
                SysWork_StateStepIncrement(0);
            }

        case 6:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(2.0f), false);
            g_SysWork.sysStateStepData[0] = 0;
            Screen_BackgroundImgDraw(&TIP_IMG);
            break;

        case 7:
            g_SysWork.sysStateStepData[0]++;
            Screen_BackgroundImgDraw(&TIP_IMG);

            if (!(g_Controller0->buttonFlags.clicked & (g_GameWorkPtr->config.controllerConfig.enter |
                                                        g_GameWorkPtr->config.controllerConfig.cancel)))
            {
                if (g_SysWork.sysStateStepData[0] <= SECONDS_TO_FRAMES(8))
                {
                    break;
                }
            }

            // TODO: Some inline FlagSet func?
            // Couldn't get matching version, but pretty sure `temp_a0` can be removed somehow.
            temp_a0 = &g_GameWork.config.seenGameOverTips[(prevTipIdx >> 5)];
            *temp_a0 |= (1 << 0) << (prevTipIdx & 0x1F);

            SysWork_StateStepIncrement(0);
            break;

        case 8:
            Screen_BackgroundImgDraw(&TIP_IMG);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(2.0f), false);
            break;

        default:
            g_MapOverlayHdr.playerControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            Game_WarmBoot();
            break;
    }

    if (g_SysWork.sysStateSteps[0] >= 2 || g_GameWork.gameState != GameState_InGame)
    {
        g_SysWork.bgmStatusFlags |= BgmStatusFlag_Pause;
    }

    #undef TIP_COUNT
    #undef TIP_IMG
}

void GameState_MapEvent_Update(void) // 0x8003AA4C
{
    if (g_GameWork.gameStateSteps[0] == 0)
    {
        g_IntervalVBlanks = 1;
        ScreenFade_Start(true, true, false);
        g_GameWork.gameStateSteps[0] = 1;
    }

    g_IsLoadingFinished = ScreenFade_IsFinished() && Fs_QueueChunksLoad();

    Savegame_EventFlagSetAlt(g_MapEventData->completeFlag);
    g_MapOverlayHdr.mapEventFuncs[g_MapEventParam]();
    Screen_BackgroundImgDraw(&g_ItemInspectionImg);
}
