#include "game.h"

#include <psyq/libcd.h>
#include <psyq/libetc.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/demo.h"
#include "bodyprog/math/math.h"
#include "bodyprog/memcard.h"
#include "bodyprog/screen/screen_data.h"
#include "bodyprog/screen/screen_draw.h"
#include "bodyprog/sys/joy.h"
#include "main/fsqueue.h"
#include "screens/b_konami/b_konami.h"
#include "screens/b_konami/lzss.h"
#include "screens/stream/stream.h"

#if VERSION_IS(EUR)
INCLUDE_RODATA("screens/b_konami/nonmatchings/b_konami", D_800CB378);
INCLUDE_ASM("screens/b_konami/nonmatchings/b_konami", GameState_KonamiLogo_Update);
#else
void GameState_KonamiLogo_Update(void) // 0x800C95AC
{
    while (g_GameWork.gameState == GameState_KonamiLogo)
    {
        Joy_Update();

        switch (g_GameWork.gameStateSteps[0])
        {
            case KonamiLogoStateStep_Init:
                Screen_Init(SCREEN_WIDTH * 2, true);

                ScreenFade_Start(true, true, false);
                g_ScreenFadeTimestep = Q12(0.2f);

                // Load `1ST/KONAMI2.TIM` (Konami logo).
                Fs_QueueStartReadTim(FILE_1ST_KONAMI2_TIM, FS_BUFFER_1, &g_KcetLogoImg);

                WorldGfx_HarryCharaLoad();
                GameFs_BgItemLoad();
                Map_EffectTexturesLoad(NO_VALUE);

                // Start loading `ANIM/HB_BASE.ANM` (base Harry animations).
                Fs_QueueStartRead(FILE_ANIM_HB_BASE_ANM, FS_BUFFER_0);

                g_GameWork.gameStateSteps[0]++;
                break;

            case KonamiLogoStateStep_WaitForFade:
                if (ScreenFade_IsNone())
                {
                    g_GameWork.gameStateSteps[0] = KonamiLogoStateStep_LogoDelay;
                }
                break;

            case KonamiLogoStateStep_LogoDelay:
                if (g_Controller0->buttonFlags.held != 0 || g_SysWork.gameStateCounter > SECONDS_60_FPS(3))
                {
                    ScreenFade_Start(false, false, false);
                    g_ScreenFadeTimestep         = Q12(0.2f);
                    g_GameWork.gameStateSteps[0] = KonamiLogoStateStep_FinishAfterFade;
                }
                break;

            case KonamiLogoStateStep_FinishAfterFade:
                if (ScreenFade_IsFinished())
                {
                    Fs_QueueWaitForEmpty();
                    Game_StateSetNext(GameState_KcetLogo);
                }
                break;
        }

        BootScreen_KonamiScreenDraw();
        Screen_FadeUpdate();
        Fs_QueueUpdate();
        MemCard_Update();
        MemCard_ElementsUpdate();
        nullsub_800334C8();
        VSync(SyncMode_Wait);
        GsSwapDispBuff();
        GsDrawOt(&g_OrderingTable2[g_ActiveBufferIdx]);

        g_ActiveBufferIdx = GsGetActiveBuff();
        GsOUT_PACKET_P   = (PACKET*)(TEMP_MEMORY_ADDR + (g_ActiveBufferIdx << 15));

        GsClearOt(0, 0, &g_OrderingTable0[g_ActiveBufferIdx]);
        GsClearOt(0, 0, &g_OrderingTable2[g_ActiveBufferIdx]);
    }
}
#endif

#if VERSION_IS(EUR)
INCLUDE_ASM("screens/b_konami/nonmatchings/b_konami", GameState_KcetLogo_MemCardCheck);
#else
s32 GameState_KcetLogo_MemCardCheck(void) // 0x800C9874
{
    s32 saveEntryTypeSlot1;
    s32 saveEntryTypeSlot2;

    // Memory cards not ready yet, rerun this on next frame.
    if (MemCard_ElementsUpdate() == false)
    {
        return KcetLogoStateStep_CheckMemCards;
    }

    g_MemCard_ActiveMemCardSlotSaves = MemCard_ActiveMemCardSlotGet(0);
    saveEntryTypeSlot1               = g_MemCard_ActiveMemCardSlotSaves->type;

    g_MemCard_ActiveMemCardSlotSaves = MemCard_ActiveMemCardSlotGet(1);
    saveEntryTypeSlot2               = g_MemCard_ActiveMemCardSlotSaves->type;

    // No memory cards.
    if (saveEntryTypeSlot1 == SavegameEntryType_NoMemCard && saveEntryTypeSlot2 == SavegameEntryType_NoMemCard)
    {
        return KcetLogoStateStep_NoMemCard;
    }

    // No free space on any card.
    if ((saveEntryTypeSlot1 == SavegameEntryType_OutOfBlocks && (saveEntryTypeSlot2 == SavegameEntryType_OutOfBlocks || saveEntryTypeSlot2 == SavegameEntryType_NoMemCard)) ||
        (saveEntryTypeSlot1 == SavegameEntryType_NoMemCard && saveEntryTypeSlot2 == SavegameEntryType_OutOfBlocks))
    {
        return KcetLogoStateStep_NoMemCardFreeSpace;
    }

    if (saveEntryTypeSlot1 == SavegameEntryType_Save || saveEntryTypeSlot2 == SavegameEntryType_Save)
    {
        g_MemCard_ActiveMemCardSlotSaves = MemCard_ActiveMemCardSlotGet(g_SelectedSaveSlotIdx);
        g_MemCard_ActiveMemCardSlotSaves = &g_MemCard_ActiveMemCardSlotSaves[g_SlotElementSelectedIdx[g_SelectedSaveSlotIdx]];

        g_SelectedDeviceId            = g_MemCard_ActiveMemCardSlotSaves->deviceId;
        g_SelectedFileIdx             = g_MemCard_ActiveMemCardSlotSaves->fileIdx;
        g_Savegame_SelectedElementIdx = g_MemCard_ActiveMemCardSlotSaves->elementIdx;

        return KcetLogoStateStep_HasSavegame;
    }

    return KcetLogoStateStep_NoSavegame;
}
#endif

#if VERSION_IS(EUR)
// @hack Static of the included function below, remove once decompiled.
static u8 nextGameState = GameState_Init;
INCLUDE_ASM("screens/b_konami/nonmatchings/b_konami", GameState_KcetLogo_Update);
#else
void GameState_KcetLogo_Update(void) // 0x800C99A4
{
    static u8 nextGameState = GameState_Init; // 0x800CA4F0

    while (g_GameWork.gameState == GameState_KcetLogo)
    {
        Joy_Update();

        switch (g_GameWork.gameStateSteps[0])
        {
            case KcetLogoStateStep_Init:
                Settings_RestoreDefaults();

                ScreenFade_Start(true, true, false);
                g_ScreenFadeTimestep = Q12(0.2f);

                GameFs_BgEtcGfxLoad();
                Fs_QueueStartRead(FILE_BG_HP_SAFE1_BIN, FS_BUFFER_5);
                Fs_QueueStartRead(FILE_BG_S__SAFE2_BIN, FS_BUFFER_6);
                g_GameWork.gameStateSteps[0]++;
                break;

            case KcetLogoStateStep_CheckMemCards:
                if (ScreenFade_IsNone())
                {
                    s32 curTime;

                    Fs_QueueWaitForEmpty();

#if VERSION_REGION_IS(NTSCJ)
                    // Anti-modchip code from NTSC-J releases, using Sony's `safechk.obj` code.
                    // Decompresses/decrypts the `S__SAFE2` / `HP_SAFE1` overlays:
                    // - `S__SAFE2`: runs `safechk.obj` to detect non-stealth modchips and halt game if found,
                    //   then calls init code relocated from the start of `MainLoop`.
                    // - `HP_SAFE1`: includes same `safechk.obj` and near-identical `AntiModchip_Check` code,
                    //   only difference is branches jump to other `MainLoop` init code instead of actually invoking `safechk`.
                    //
                    // Init calls were likely moved here so skipping these overlays would break things.
                    // `HP_SAFE1` being a near-copy of `S__SAFE2` may be just to slightly confuse pirates.
                    //
                    // TODO:
                    // - `CdDiskReady` and `CdGetDiskType` are part of `libcd/type.o`, not included in US release, need conversion from SDK libs.
                    // - Add `FS_BUFFER_` constants for the addresses used here.

                    while (CdDiskReady(false) != CdlComplete || CdGetDiskType() == CdlStatShellOpen)
                    {
                        VSync(0);
                    }

                    // Decompress the `HP_SAFE1/S__SAFE2` overlays.
                    Lzss_Init(FS_BUFFER_5, FS_BUFFER_21, 3000); // Larger than actual `HP_SAFE1` size?
                    Lzss_Decode(NO_VALUE);
                    Lzss_Init(FS_BUFFER_6, (void*)0x801E6600, 3000);
                    Lzss_Decode(NO_VALUE);

                    // Decrypt `S__SAFE2` and run `AntiModchip_Check`
                    Fs_DecryptOverlay((void*)0x801E7600, (void*)0x801E6600, 4096);
                    curTime = g_SysWork.gameStateCounter;
                    AntiModchip_Check();

                    // Decrypt `HP_SAFE1` and run `AntiModchip_Check`
                    Fs_DecryptOverlay((void*)0x801E7600, FS_BUFFER_21, 4096);
                    
                    // Only run `HP_SAFE1` if `S__SAFE2` took enough time to execute, cheap way of checking if the call above was skipped?
                    if ((g_SysWork.gameStateCounter - curTime) >= 100)
                    {
                        AntiModchip_Check();
                    }

                    // Reset drive & sound driver
                    CdReset(1);
                    SD_InitStruct();
#endif

                    while (g_GameWork.gameStateSteps[0] < KcetLogoStateStep_NoMemCard)
                    {
                        g_GameWork.gameStateSteps[0] = GameState_KcetLogo_MemCardCheck();
                        MemCard_Update();
                        VSync(SyncMode_Wait);
                    }
                }
                break;

            case KcetLogoStateStep_NoMemCard:
#if VERSION_REGION_IS(NTSCJ)
                Fs_QueueStartReadTim(FILE_1ST_NO_MEMCD_TIM, FS_BUFFER_1, &g_MemCardWarningImg);
#else
                Fs_QueueStartReadTim(FILE_1ST_NO_MCD_E_TIM, FS_BUFFER_1, &g_MemCardWarningImg);
#endif
                GameFs_StreamBinLoad();
                nextGameState = GameState_MovieIntroFadeIn;

                Game_StateStepSet(0, KcetLogoStateStep_LogoDelay);
                break;

            case KcetLogoStateStep_NoMemCardFreeSpace:
#if VERSION_REGION_IS(NTSCJ)
                Fs_QueueStartReadTim(FILE_1ST_NO_BLOCK_TIM, FS_BUFFER_1, &g_MemCardWarningImg);
#else
                Fs_QueueStartReadTim(FILE_1ST_NO_BLK_E_TIM, FS_BUFFER_1, &g_MemCardWarningImg);
#endif
                GameFs_StreamBinLoad();
                nextGameState = GameState_MovieIntroFadeIn;

                Game_StateStepSet(0, KcetLogoStateStep_LogoDelay);
                break;

            case KcetLogoStateStep_NoSavegame:
                GameFs_StreamBinLoad();
                GameFs_TitleGfxSeek();
                nextGameState = GameState_MovieIntro;

                Game_StateStepSet(0, KcetLogoStateStep_LogoDelay);
                break;

            case KcetLogoStateStep_HasSavegame:
                while (g_GameWork.gameStateSteps[1] < 3)
                {
                    switch (g_GameWork.gameStateSteps[1])
                    {
                        case 0:
                            MemCard_ProcessSet(MemCardGameProcessId_Load_Game, g_SelectedDeviceId, 0, 0);
                            Game_StateStepIncrement(1);

                        case 1:
                            if (MemCard_LastMemCardResultGet() != MemCardWorkResult_Success)
                            {
                                Game_StateStepIncrement(1);
                            }
                            break;

                        case 2:
                            if (g_GameWorkConst->config.autoLoad)
                            {
                                Fs_QueueStartRead(FILE_VIN_SAVELOAD_BIN, FS_BUFFER_1);
                                Fs_QueueStartSeek(FILE_TIM_SAVELOAD_TIM);
                                nextGameState = GameState_AutoLoadSavegame;
                            }
                            else
                            {
                                GameFs_StreamBinLoad();
                                GameFs_TitleGfxSeek();
                                nextGameState = GameState_MovieIntro;
                            }

                            Game_StateStepIncrement(1);
                            break;
                    }

                    MemCard_ElementsUpdate();
                    MemCard_Update();
                    VSync(SyncMode_Wait);
                }

                Game_StateStepSet(0, KcetLogoStateStep_LogoDelay);
                break;

            case KcetLogoStateStep_LogoDelay:
                if (g_Controller0->buttonFlags.held != 0 || g_SysWork.gameStateCounter > SECONDS_60_FPS(3))
                {
                    ScreenFade_Start(false, false, false);
                    g_ScreenFadeTimestep = Q12(0.2f);
                    g_GameWork.gameStateSteps[0]++;
                }
                break;

            case KcetLogoStateStep_FinishAfterFade:
                if (ScreenFade_IsFinished())
                {
                    Settings_ScreenAndVolUpdate();
                    Screen_Init(SCREEN_WIDTH, false);

                    switch (nextGameState)
                    {
                        case GameState_AutoLoadSavegame:
                            Fs_QueueStartReadTim(FILE_TIM_SAVELOAD_TIM, FS_BUFFER_7, &g_ItemInspectionImg);
                            break;

                        case GameState_MovieIntroFadeIn:
                            break;

                        case GameState_MovieIntroAlternate:
                        default:
                            // USA and later versions move these `Demo_*` calls to after the switch.
#if VERSION_EQUAL_OR_OLDER(JAP0) 
                            Demo_SequenceAdvance(0);
                            Demo_DemoDataRead();
#endif
                            GameFs_TitleGfxLoad();
                            break;
                    }
                    
#if VERSION_EQUAL_OR_NEWER(USA) // @bugfix Moved from the switch above.
                    Demo_SequenceAdvance(0);
                    Demo_DemoDataRead();
#endif
                    Fs_QueueWaitForEmpty();

                    Game_StateSetNext(nextGameState);
                }
                break;
        }

        BootScreen_KcetScreenDraw();
        Screen_FadeUpdate();
        Fs_QueueUpdate();
        MemCard_Update();
        MemCard_ElementsUpdate();
        nullsub_800334C8();
        VSync(SyncMode_Wait);
        GsSwapDispBuff();
        GsDrawOt(&g_OrderingTable2[g_ActiveBufferIdx]);

        g_ActiveBufferIdx = GsGetActiveBuff();
        GsOUT_PACKET_P   = (g_ActiveBufferIdx << 0xF) + (u32)TEMP_MEMORY_ADDR;

        GsClearOt(0, 0, &g_OrderingTable0[g_ActiveBufferIdx]);
        GsClearOt(0, 0, &g_OrderingTable2[g_ActiveBufferIdx]);
    }
}
#endif

// PAL versions relocate this to bodyprog, as a bodyprog func (`MainMenu_BackgroundDraw`) was updated to also require it.
// Note: changes here should be copied over to bodyprog version too.
#if !VERSION_REGION_IS(PAL)
void BootScreen_ImageSegmentDraw(s_FsImageDesc* image, s32 otz, s32 vramX, s32 vramY, s32 w, s32 h, s32 x, s32 y) // 0x800C9E6C
{
    DR_TPAGE* tPage;
    SPRT*     prim     = (SPRT*)GsOUT_PACKET_P;
    u32       vramBase = image->tPage[1] + (u32)(vramX >> 8) + (((u32)(vramY >> 8)) << 4);
    u32*      addr     = &g_OtTags0[g_ActiveBufferIdx][otz];

    addPrimFast(addr, prim, 4);
    setCodeWord(prim, PRIM_RECT | RECT_TEXTURE, 0x808080);
    setWH(prim, w, h);

    vramX = vramX & 0xFF;
    vramY = vramY & 0xFF;

    //setUV0AndClut(prim, vramX, vramY, image->clutX, image->clutY);
    *(u32*)(&prim->u0) = vramX + (vramY << 8) + (((image->clutY << 6) | ((image->clutX >> 4) & 0x3F)) << 16);

    setXY0Fast(prim, (u16)x, y);

    tPage = (DR_TPAGE*)((u8*)prim + sizeof(SPRT));
    setDrawTPage(tPage, 0, 1, getTPage(image->tPage[0], 0, (vramBase << 6), (((vramBase >> 4) & (1 << 0)) << 8)));
    AddPrim(addr, tPage);

    GsOUT_PACKET_P = (u8*)tPage + sizeof(DR_TPAGE);
}
#endif

void BootScreen_KonamiScreenDraw(void) // 0x800C9FB8
{
    s32*  ptr;
    TILE* tile;

    // Draw Konami logo.
    BootScreen_ImageSegmentDraw(&g_KonamiLogoImg, 0xF, 0, 0, 256, 256, -192, -192);
    BootScreen_ImageSegmentDraw(&g_KonamiLogoImg, 0xF, 256, 0, 128, 256, 64, -192);
    BootScreen_ImageSegmentDraw(&g_KonamiLogoImg, 0xF, 0, 256, 256, 128, -192, 64);
    BootScreen_ImageSegmentDraw(&g_KonamiLogoImg, 0xF, 256, 256, 128, 128, 64, 64);

    ptr = &g_OtTags0[g_ActiveBufferIdx][15];
    tile = (TILE*)GsOUT_PACKET_P;
    
    // Draw fading overlay tile.
    addPrimFast(ptr, tile, 3);
    setCodeWord(tile, PRIM_RECT, 0xFFFFFF);

#if VERSION_REGION_IS(PAL)
    setXY0Fast(tile, -SCREEN_WIDTH, -FRAMEBUFFER_HEIGHT_PROGRESSIVE);
    setWH(tile, SCREEN_WIDTH * 2, FRAMEBUFFER_HEIGHT_PROGRESSIVE * 2);
#else
    setXY0Fast(tile, -SCREEN_WIDTH, -SCREEN_HEIGHT);
    setWH(tile, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2);
#endif

#if VERSION_REGION_IS(NTSCJ)
    // Draw unknown JPN0 tile.
    tile++;
    ptr--;

    addPrimFast(ptr, tile, 3);
    setCodeWord(tile, PRIM_RECT, 0xFFFFFF);
    setXY0Fast(tile, 136, 140);
    setWH(tile, 13, 13);
#endif

    GsOUT_PACKET_P = (PACKET*)&tile[1];
}

#if VERSION_IS(EUR)
INCLUDE_ASM("screens/b_konami/nonmatchings/b_konami", BootScreen_KcetScreenDraw);
#else
void BootScreen_KcetScreenDraw(void) // 0x800CA120
{
    u32* ptr;

    // Draw KCET logo.
    BootScreen_ImageSegmentDraw(&g_KcetLogoImg, 0xF, 0, 0, 256, 160, -208, -80);
    BootScreen_ImageSegmentDraw(&g_KcetLogoImg, 0xF, 256, 0, 160, 160, 48, -80);

    // Draw fading overlay tile.
    ptr = &g_OtTags0[g_ActiveBufferIdx][15];
    addPrimFast(ptr, (TILE*)GsOUT_PACKET_P, 3);
    setCodeWord((TILE*)GsOUT_PACKET_P, PRIM_RECT, 0xFFFFFF);
    setXY0Fast((TILE*)GsOUT_PACKET_P, -SCREEN_WIDTH, -SCREEN_HEIGHT);
    setWH((TILE*)GsOUT_PACKET_P, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2);
    GsOUT_PACKET_P = (PACKET*)((u8*)GsOUT_PACKET_P + sizeof(TILE));
}
#endif
