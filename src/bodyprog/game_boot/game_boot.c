#include "game.h"

#include <psyq/libetc.h>
#include <psyq/libpad.h>
#include <psyq/strings.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/demo.h"
#include "bodyprog/events/bgm_update.h"
#include "bodyprog/events/collision_trigger.h"
#include "bodyprog/events/radio.h"
#include "bodyprog/game_boot/fs_chara_anim.h"
#include "bodyprog/screen/screen_data.h"
#include "bodyprog/screen/screen_draw.h"
#include "bodyprog/sound/sound_system.h"

// ========================================
// INGAME INITIALIZATION PROCESS
// ========================================

void GameState_LoadScreen_Update(void)
{
    #define LOAD_COUNT_MAX 20

    GameBoot_LoadingScreen();
    GameBoot_InGameStartup();

    if (g_SysWork.sysFlags & SysFlag_LoadActive)
    {
        g_MapAreaLoadCounter++;
        if (g_MapAreaLoadCounter > LOAD_COUNT_MAX)
        {
            g_SysWork.sysFlags &= ~SysFlag_LoadActive;

            SD_Call(Sfx_Unk1502);
            SD_Call(Sfx_Unk1501);
        }
    }

    #undef LOAD_COUNT_MAX
}

void GameBoot_InGameStartup(void)
{
    // Makes up to 5 attemps. If the load fails, the timer used to check if a demo should be triggered is restarted.
    static s32 demoLoadAttempCount;

    #define playerChara g_SysWork.playerWork.player

    // Handle boot startup step.
    switch (g_GameWork.gameStateSteps[0])
    {
        case 0:
            g_IntervalVBlanks              = 1;
            g_GameWork.background2dColor.r = 0;
            g_GameWork.background2dColor.g = 0;
            g_GameWork.background2dColor.b = 0;

            if (g_SysWork.processFlags == ProcessFlag_RoomTransition)
            {
                AreaLoad_UpdatePlayerPosition();
                g_GameWork.gameStateSteps[0] = 7;
            }
            else if (g_SysWork.processFlags == ProcessFlag_BootDemo)
            {
                demoLoadAttempCount            = 0;
                g_GameWork.gameStateSteps[0]   = 1;
                g_SysWork.gameStateStepCounter = 1;
            }
            else
            {
                g_GameWork.gameStateSteps[0] = 3;
            }

            SD_Call(19);
            break;

        case 1:
            if (g_SysWork.gameStateStepCounter > SECONDS_TO_FRAMES(20) && 
                Fs_QueueGetLength() == 0 && Sd_AudioStreamingCheck() == AudioStreamingState_None)
            {
                Demo_DemoFileSavegameUpdate();
                GameBoot_WorldInit();

                if (Demo_PlayFileBufferSetup())
                {
                    GameBoot_MapLoad(g_SavegamePtr->mapIdx);
                    Game_StateStepSet(0, 2);
                    break;
                }

                Demo_SequenceAdvance(1);
                Demo_DemoDataRead();

                demoLoadAttempCount++;
                if (demoLoadAttempCount >= 5)
                {
                    demoLoadAttempCount            = 0;
                    g_SysWork.gameStateStepCounter = 0;
                    break;
                }
            }
            break;

        case 2:
            if (Fs_QueueGetLength() == 0 && Sd_AudioStreamingCheck() == AudioStreamingState_None)
            {
                Demo_PlayDataRead();
                Game_StateStepSet(0, 3);
            }
            break;

        case 3:
            if (Fs_QueueGetLength() == 0)
            {
                g_GameWork.gameStateSteps[0] = 4;
            }
            break;

        case 4:
            if (g_SysWork.processFlags == ProcessFlag_OverlayTransition)
            {
                AreaLoad_UpdatePlayerPosition();
            }
            else if (g_SysWork.processFlags == ProcessFlag_LoadSave ||
                     g_SysWork.processFlags == ProcessFlag_Continue)
            {
                g_SysWork.loadingScreenIdx = LoadingScreenId_PlayerRun;
            }

            g_GameWork.gameStateSteps[0]++;
            break;

        case 5:
            Fs_CharaAnimDataAlloc(1, g_MapOverlayHdr.charaGroupIds[0], NULL, 0);
            Fs_CharaAnimDataAlloc(2, g_MapOverlayHdr.charaGroupIds[1], NULL, 0);
            Fs_CharaAnimDataAlloc(3, g_MapOverlayHdr.charaGroupIds[2], NULL, 0);
            WorldGfx_MapInitCharaLoad(&g_MapOverlayHdr);

            g_GameWork.gameStateSteps[0]++;

        case 6:
            if (Fs_QueueGetLength() == 0)
            {
                g_GameWork.gameStateSteps[0]++;
            }
            break;

        case 7:
            if (AreaLoad_TransitionFlags() & AreaTransitionFlag_UnfreezeWorld)
            {
                WorldGfx_MapReset();
            }

            WorldGfx_MapInit(&g_MapOverlayHdr, playerChara.position.vx, playerChara.position.vz);
            if (g_SysWork.processFlags == ProcessFlag_OverlayTransition)
            {
                Game_RadioSoundStop();
            }

            g_GameWork.gameStateSteps[0]++;

        case 8:
            if (WorldGfx_ChunksInitCheck())
            {
                Game_StateStepIncrement(0);
            }
            break;

        case 9:
            if (!Sd_BgmInit())
            {
                g_GameWork.gameState = GameState_MainLoadScreen;
                Game_StateStepIncrement(0);
            }
            break;

        case 10:
            if (g_SysWork.processFlags == ProcessFlag_BootDemo && !(g_SysWork.sysFlags & SysFlag_DemoActive))
            {
                Demo_Start();
                g_SysWork.sysFlags |= SysFlag_DemoActive;
            }

            if (AreaLoad_TransitionFlags() & AreaTransitionFlag_SkipAmbientSfxInit || Sd_AmbientSfxInit() == 0)
            {
                Game_StateStepIncrement(0);
            }
            break;

        case 11:
            if (g_SysWork.gameStateCounter >= SECONDS_TO_FRAMES(1))
            {
                if (g_SysWork.processFlags == ProcessFlag_RoomTransition)
                {
                    GameBoot_NpcInit();
                }
                else
                {
                    GameBoot_InGameInit();
                }

                if (g_SysWork.processFlags == ProcessFlag_None || 
                    g_SysWork.processFlags == ProcessFlag_RoomTransition ||
                    g_SysWork.processFlags == ProcessFlag_OverlayTransition)
                {
                    AreaLoad_TransitionSound();
                }

                MemCard_SysDisable();
                g_GameWork.gameStateSteps[0]++;
            }
            break;

        case 12:
            if (Sd_AudioStreamingCheck() == AudioStreamingState_None)
            {
                Game_StateSetNext(GameState_InGame);

                if (AreaLoad_TransitionFlags() & AreaTransitionFlag_SkipFadeIn)
                {
                    g_GameWork.gameStateSteps[0] = 1;
                    g_ScreenFade_Status          = SCREEN_FADE_STATUS(ScreenFadeState_ResetTimestep,
                                                                      IS_SCREEN_FADE_WHITE(g_ScreenFade_Status));
                }
            }
            break;

        default:
            break;
    }

    #undef playerChara
}

/** @brief Initalizes drawing of the loading screen. */
static void GameBoot_LoadingScreen(void) // 0x80034E58
{
    if (g_SysWork.loadingScreenIdx != LoadingScreenId_None && g_GameWork.gameStateSteps[0] < 10)
    {
        ScreenFade_Start(false, true, false);
        g_ScreenFadeTimestep = Q12(0.8f);
        g_MapOverlayHdr.loadingScreenFuncs[g_SysWork.loadingScreenIdx]();
    }

    Screen_BackgroundMotionBlur(SyncMode_Wait2);
}

static void GameBoot_NpcClear(void) // 0x80034EC8
{
    s32 i;

    g_SysWork.field_228C[0] = 0;
    g_SysWork.npcFlags      = 0;

    bzero(g_SysWork.npcs, ARRAY_SIZE(g_SysWork.npcs) * sizeof(s_SubCharacter));

    for (i = 0; i < CHARA_GROUP_COUNT; i++)
    {
        g_SysWork.charaGroupFlags[i] = 0;
    }
}

static void GameBoot_NpcInit(void) // 0x80034F18
{
    #define playerChara g_SysWork.playerWork.player

    vcSetCameraUseWarp(&playerChara.position, g_SysWork.cameraAngleY);
    func_8005E70C();

    if (g_SysWork.field_234A)
    {
        g_MapOverlayHdr.particleEnviromentSet(g_SysWork.field_2349, 127);
        g_MapOverlayHdr.particleSystemUpdate(0, g_SavegamePtr->mapIdx, 0);
    }

    GameBoot_NpcClear();
    Game_NpcRoomInitSpawn(false);
    Game_PlayerHeightUpdate();

    #undef playerChara
}

void GameBoot_InGameInit(void) // 0x80034FB8
{
    s32 mapOvlId;

    #define playerChara g_SysWork.playerWork.player

    mapOvlId = g_SavegamePtr->mapIdx;

    vcInitCamera(&g_MapOverlayHdr, &playerChara.position);

    vcSetCameraUseWarp(&playerChara.position, g_SysWork.cameraAngleY);
    World_CollisionTriggersSet(&g_MapOverlayHdr);
    GameBoot_WolrdEnvInit(0);
    WorldGfx_CharaModelProcessAllLoads();
    Game_FlashlightAttributesFix();

    g_MapOverlayHdr.particleSystemUpdate(0, mapOvlId, NO_VALUE);

    GameBoot_NpcClear();

    g_SysWork.npcFlagId = 5;

    func_8005E650(mapOvlId);
    func_80037124();
    func_8007E8C0();
    Game_NpcRoomInitSpawn(false);
    Game_PlayerHeightUpdate();
    Fs_CharaAnimBoneInfoSet();
    GameFs_WeaponInfoUpdate();
    GameFs_Tim00TIMLoad();
    Fs_QueueWaitForEmpty();
    GameFs_MapItemsModelLoad(mapOvlId);

    #undef playerChara
}

void GameBoot_SavegameInitialize(s8 overlayId, s32 difficulty) // 0x800350BC
{
    s32  i;
    s32* mapEnemyStatesPtr;

    bzero(g_SavegamePtr, sizeof(s_Savegame));

    g_SavegamePtr->mapIdx = overlayId;

    difficulty = CLAMP(difficulty, GameDifficulty_Easy, GameDifficulty_Hard);

    mapEnemyStatesPtr = g_SavegamePtr->mapEnemyStates;

    g_SavegamePtr->gameDifficulty = difficulty;
    g_SavegamePtr->paperMapIdx    = PaperMapIdx_OldTown;

    // Define all enemies from an overlay as alive.
    // Odd code. Possibly a hack.
    for (i = 0; i < Chara_Count; i++)
    {
        mapEnemyStatesPtr[44] = NO_VALUE;
        mapEnemyStatesPtr--;
    }

    Game_SavegameResetPlayer();
}

void GameBoot_WorldInit(void) // 0x80035178
{
    #define ANIMS_DATA_BUFFER_SIZE 0x2E630

    World_Init();
    WorldGfx_CharaModelsFree();
    WorldGfx_HeldItemModelFree();
    Anim_BonesInit((s_AnmHeader*)FS_BUFFER_0, g_SysWork.playerBoneCoords);
    WorldGfx_PlayerModelProcessLoad();

    g_SysWork.unused_229C = NO_VALUE;

    if ((g_SavegamePtr->itemToggleFlags >> 1) & (1 << 0)) // `& ItemToggleFlag_FlashlightOff`
    {
        Game_TurnFlashlightOff();
    }
    else
    {
        Game_TurnFlashlightOn();
    }

    g_CharaModelAnimsData[0].activeSize = ANIMS_DATA_BUFFER_SIZE;
    g_CharaModelAnimsData[0].allocSize  = ANIMS_DATA_BUFFER_SIZE;

    Game_PlayerInfoInit();

    #undef ANIMS_DATA_BUFFER_SIZE
}

#if VERSION_IS(EUR)
e_FsFile GameBoot_MapFileIdGet(s32 mapIdx) // 0x80034A98
{
    return g_MapLanguageBaseIds[g_GameWorkConst->config.palLanguageId] + mapIdx;
}
#endif

void GameBoot_MapLoad(s32 mapIdx) // 0x8003521C
{
    e_FsFile fileIdx = FILE_VIN_MAP0_S00_BIN + mapIdx;
    
#if VERSION_REGION_IS(PAL)
    fileIdx = GameBoot_MapFileIdGet(mapIdx);
    GameBoot_ItemLangLoad();
#endif
    
    Fs_QueueStartRead(fileIdx, g_OvlDynamic);
    Map_EffectTexturesLoad(mapIdx);
    GameFs_PlayerMapAnimLoad(mapIdx);

    // If the player spawns in with a weapon equipped (either because it's a demo or because the user saved the game
    // with a weapon equipped), this and the next function make it appear and allocate its data.
    if (g_SysWork.processFlags & (ProcessFlag_NewGame  |
                                  ProcessFlag_LoadSave |
                                  ProcessFlag_Continue |
                                  ProcessFlag_BootDemo))
    {
        WorldGfx_PlayerPrevHeldItem(&g_SysWork.playerCombat);
    }
    Gfx_PlayerHeldItemAttach(g_SysWork.playerCombat.weaponAttack);
}

#if VERSION_IS(EUR)
void GameBoot_ItemLangLoad(void) // 0x80034B44
{
    if (g_GameWork.loadedPalItemLang != g_GameWork.config.palLanguageId + 1)
    {
        g_GameWork.loadedPalItemLang = g_GameWork.config.palLanguageId + 1;
        Fs_QueueStartRead(g_ItemLanguageFileIds[g_GameWork.config.palLanguageId], g_OvlItemLang);
    }
}
#endif
