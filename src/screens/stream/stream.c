#include "game.h"

#include <psyq/libetc.h>
#include <psyq/libds.h>
#include <psyq/libpress.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/libsd.h"
#include "bodyprog/screen/screen_draw.h"
#include "bodyprog/sys/game_main.h"
#include "bodyprog/sys/joy.h"
#include "bodyprog/text/text_draw.h"
#include "bodyprog/math/math.h"
#include "main/fileinfo.h"
#include "screens/stream/stream.h"

// Margin above and below the movie, PAL has 288 lines to center it in instead of 240.
#if VERSION_REGION_IS(PAL)
    #define SCREEN_MARGIN_Y 16
#else
    #define SCREEN_MARGIN_Y 8
#endif

#if VERSION_IS(EUR)
INCLUDE_ASM("screens/stream/nonmatchings/stream", GameState_MovieIntroFadeIn_Update);
#else
void GameState_MovieIntroFadeIn_Update(void) // 0x801E2654
{
    switch (g_GameWork.gameStateSteps[0])
    {
        case 0:
            VSync(SyncMode_Wait8);
            ScreenFade_Start(true, true, false);

#if VERSION_REGION_IS(NTSCJ)
            Fs_QueueStartRead(FILE_TIM_WATER_TIM, g_OvlDynamic);
#endif

            GameFs_TitleGfxLoad();

            g_GameWork.gameStateSteps[0]++;
            break;

        case 1:
            if (g_Controller0->buttonFlags.held != 0 || g_SysWork.gameStateCounter > SECONDS_60_FPS(5))
            {
                ScreenFade_Start(false, false, false);
                g_GameWork.gameStateSteps[0] = 2;
            }
            break;

        case 2:
            if (ScreenFade_IsFinished())
            {
                Fs_QueueWaitForEmpty();
                Game_StateSetNext(GameState_MovieIntro);
            }
            break;
    }

    Screen_BackgroundImgDraw(g_MemCardWarningImg);
}
#endif

void GameState_MovieIntro_Update(void) // 0x801E279C
{
#if VERSION_REGION_IS(NTSCJ)
    if (g_GameWork.gameStatePrev == GameState_KcetLogo ||
        g_GameWork.gameStatePrev == GameState_AutoLoadSavegame)
    {
        Fs_QueueStartRead(FILE_TIM_WATER_TIM, g_OvlDynamic);
        Fs_QueueWaitForEmpty();
    }
#endif

    open_main((g_GameWorkConst->config.extraOptionsEnabled & (1 << 0)) ? FILE_XA_C2_20670 : FILE_XA_C1_20670, 0);
    Game_StateSetNext(GameState_MainMenu);

    g_ScreenFadeTimestep = Q12(1.0f);
}

void GameState_MovieOpening_Update(void) // 0x801E2838
{
    open_main(FILE_XA_M1_03500, 0);
    Game_StateSetNext(GameState_MainLoadScreen);
}

void GameState_ExitMovie_Update(void) // 0x801E28B0
{
    Game_StateSetNext(GameState_InGame);
}

void GameState_DebugMoviePlayer_Update(void) // 0x801E2908
{
    static s32 g_Debug_MoviePlayerIdx = 0; // 0x801E3F3C

    if (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.cancel)
    {
        Game_StateSetNext(GameState_Unk16); // Changes to non-existent state 22 and crashes. Maybe removed debug menu.
    }

    if (g_Controller0->buttonFlags.pulsed & ControllerFlag_LStickHighLeft)
    {
        g_Debug_MoviePlayerIdx--;
    }

    if (g_Controller0->buttonFlags.pulsed & ControllerFlag_LStickHighRight)
    {
        g_Debug_MoviePlayerIdx++;
    }

    Text_Debug_PositionSet(SCREEN_WIDTH / 8, 40);

#if VERSION_EQUAL_OR_OLDER(PROTO_981216)
    // Code seen in 98-12-16 to display movie number selection (`Text_Debug_Draw` was nullsub in those builds).
    Text_Debug_Draw("MOVIE NO=");
    Text_Debug_Draw(Text_Debug_IntToString(2, g_Debug_MoviePlayerIdx));
#endif

    if (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.enter)
    {
        open_main(FILE_XA_ZC_14392 - g_Debug_MoviePlayerIdx, 0);
    }
}

void GameState_MovieIntroAlternate_Update(void) // 0x801E2A24
{
    // 2060 is a frame offset to make this play the second video, which is a duplicate intro.
    open_main(FILE_XA_C1_20670, 2060);

    Game_StateSetNext(GameState_MainMenu);
    g_ScreenFadeTimestep = Q12(1.0f);
}

s32 max_frame = 0; // 0x801E3F40
s32 frame_cnt = 0; // 0x801E3F44

DISPENV disp = {
    { 0, 256,             SCREEN_WIDTH, SCREEN_HEIGHT }, // `disp`
    { 0, SCREEN_MARGIN_Y, 256,          208           }, // `screen`
    false,                                   // `isinter`
    true,                                    // `isrgb24`
    0,                                       // `pad0`
    0                                        // `pad1`
}; // 0x801E3F48

MOVIE_STR* m;

void open_main(s32 file_idx, s16 num_frames) // 0x801E2AA4
{
    Fs_QueueWaitForEmpty();
    if (num_frames == 0)
    {
        num_frames = g_FileTable[file_idx].blockCount - 7;
    }

    Screen_RectInterlacedClear(0, 16, 480, 480, 0, 0, 0);
    movie_main(NULL, num_frames, g_FileTable[file_idx].startSector);
    Screen_RectInterlacedClear(0, 16, 480, 480, 0, 0, 0);
    VSync(SyncMode_Wait);
    GsSwapDispBuff();
}

#if VERSION_IS(EUR)
INCLUDE_ASM("screens/stream/nonmatchings/stream", movie_main);
#else
void movie_main(char* file_name, s32 f_size, s32 sector) // 0x801E2B9C
{
    #define SEARCH_COUNT_MAX 10000

    CdlFILE file;
    CdlLOC  loc;
    u8      param;
    s32     frame_no;
    s32     search_times;
    s32     prev_frame_no;

    frame_cnt = 0;
    max_frame = f_size;

    m         = (MOVIE_STR*)TEMP_MEMORY_ADDR;
    m->width  = SCREEN_WIDTH;
    m->height = SCREEN_HEIGHT;

    if (sector == 0)
    {
        search_times = 0;

        while (CdSearchFile(&file, file_name) == NULL)
        {
            if (search_times++ > SEARCH_COUNT_MAX)
            {
                return;
            }
        }

        m->loc.minute = file.pos.minute;
        m->loc.second = file.pos.second;
        m->loc.sector = file.pos.sector;
    }
    else
    {
        CdIntToPos(sector, &m->loc);
    }

    strSetDefDecEnv(&m->dec, 0, 16, 0, 256);
    VSync(SyncMode_Wait);
    strInit(&m->loc, strCallback);

    while (strNextVlc(&m->dec) == NO_VALUE)
    {
        loc = file.pos;
        strKickCD(&loc);
    }

    SsSetSerialAttr(0, 0, 1);
    SsSetSerialVol(0, 80, 80);
    VSync(SyncMode_Wait);

    prev_frame_no = 0;

    do
    {
        disp.disp.y   = 256 - (m->dec.rectid * SCREEN_HEIGHT);
        disp.screen.x = g_GameWorkConst->config.screenPositionX;
        disp.screen.y = (8 + ((224 - m->height) / 2)) + g_GameWorkConst->config.screenPositionY;
        disp.disp.y   = (disp.disp.y   <  16) ? 16 : ((disp.disp.y > 256)   ? 256 : disp.disp.y);
        disp.screen.h = (disp.screen.h <= 0)  ? 1  : ((disp.screen.h > 208) ? 208 : disp.screen.h);

        PutDispEnv(&disp);
        nullsub_800334C8();
        Joy_Update();
        DecDCTin(m->dec.vlcbuf[m->dec.vlcid], 3);
        DecDCTout((u_long*)m->dec.imgbuf, m->dec.slice.w * m->dec.slice.h / 2);

        while (strNextVlc(&m->dec) == NO_VALUE)
        {
            frame_no = StGetBackloc((DslLOC*)&loc);
            if (max_frame < frame_no || frame_no <= 0)
            {
                loc = file.pos;
            }

            strKickCD(&loc);
        }

        if (frame_cnt >= max_frame || prev_frame_no > frame_cnt)
        {
            break;
        }

        prev_frame_no = frame_cnt;

        strSync(&m->dec);
        VSync(SyncMode_Wait);
    }
    while (!(g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.skip) &&
           MainLoop_ShouldWarmReset() <= ResetType_None);

    SsSetSerialVol(0, 0, 0);

    param = CdlModeSpeed;
    CdControlB(CdlSetmode, &param, 0);
    CdControlB(CdlPause, NULL, NULL);

    StUnSetRing();
    DecDCToutCallback(NULL);

    #undef SEARCH_COUNT_MAX
}
#endif

void strSetDefDecEnv(DECENV* dec, s32 x0, s32 y0, s32 x1, s32 y1) // 0x801E2F8C
{
    dec->vlcbuf[0] = m->vlcbuf0;
    dec->vlcbuf[1] = m->vlcbuf1;
    dec->vlcid     = 0;
    dec->imgbuf    = m->imgbuf0;
    dec->rectid    = 0;
    dec->isdone    = 0;

    setRECT(&dec->rect[0], x0, y0, PPW(SCREEN_WIDTH), SCREEN_HEIGHT);
    setRECT(&dec->rect[1], x1, y1, PPW(SCREEN_WIDTH), SCREEN_HEIGHT);
    setRECT(&dec->slice, x0, y0, PPW(16), SCREEN_HEIGHT);
}

void strInit(CdlLOC* loc, void (*callback)()) // 0x801E300C
{
    DecDCTReset(0);
    DecDCToutCallback(callback);
    StSetRing(m->sect_buff, RING_SIZE);
    StSetStream(1, 1, 0xFFFFFFFF, 0, 0);
    strKickCD(loc);
}

void strCallback(void) // 0x801E307C
{
    RECT snap_rect;
    s32  id;
    u16* imgbuf;

    if (StCdIntrFlag)
    {
        StCdInterrupt();
        StCdIntrFlag = 0;
    }

    snap_rect = m->dec.slice;

    DrawSync(SyncMode_Wait);
    LoadImage(&snap_rect, (u_long*)m->dec.imgbuf);

    imgbuf = m->imgbuf0;
    m->dec.slice.x += m->dec.slice.w;

    if (m->dec.imgbuf == imgbuf)
    {
        imgbuf = m->imgbuf1;
    }

    m->dec.imgbuf = imgbuf;

    if (m->dec.slice.x < (m->dec.rect[m->dec.rectid].x + m->dec.rect[m->dec.rectid].w))
    {
        DecDCTout((u_long*)m->dec.imgbuf, m->dec.slice.w * m->dec.slice.h / 2);
    }
    else
    {
        DrawSync(SyncMode_Wait);
        m->dec.isdone = 1;

        m->dec.rectid  = m->dec.rectid ^ 1;
        m->dec.slice.x = m->dec.rect[m->dec.rectid].x;
        m->dec.slice.y = m->dec.rect[m->dec.rectid].y;
    }
}

void strKickCD(CdlLOC* loc) // 0x801E31CC
{
    s8 v2[8];
    u8 param;

    while (!CdControlB(CdlNop, 0, v2) || (v2[0] & (1 << 1)) == 0)
    {
        CdControlB(CdlStandby, 0, 0);
        VSync(SyncMode_Wait);
    }

    param = 0x80;
    while (!CdControl(CdlSetmode, &param, 0))
    {
        ;
    }

    while (!CdControl(CdlSeekL, loc, 0))
    {
        VSync(SyncMode_Wait);
    }

    while (!CdRead2(CdlModeStream | CdlModeSpeed | CdlModeRT | CdlModeSize1))
    {
        VSync(SyncMode_Wait);
    }
}

int strNextVlc(DECENV* dec) // 0x801E3298
{
    #define COUNT_MAX 2000

    u_long* next;

    u_long count = COUNT_MAX;
    while ((next = strNext(dec)) == 0)
    {
        count--;
        if (!count)
        {
            return NO_VALUE;
        }
    }

    dec->vlcid = dec->vlcid ^ 1;
    DecDCTvlc(next, dec->vlcbuf[dec->vlcid]);

    StFreeRing(next);
    return 0;

    #undef COUNT_MAX
}

u_long* strNext(DECENV* dec) // 0x801E331C
{
    u_long*   addr;
    CDSECTOR* sector;
    int       count = MOVIE_WAIT;

    while (StGetNext((u_long**)&addr, (u_long**)&sector))
    {
        if (--count == 0)
        {
            return 0;
        }
    }

    if (addr[0] != sector->headm || addr[1] != sector->headv)
    {
        StFreeRing(addr);
        return 0;
    }

    frame_cnt = sector->frameCount;

    if (m->width != sector->width || m->height != sector->height)
    {
        m->width  = sector->width;
        m->height = sector->height;
    }

    dec->rect[0].w = dec->rect[1].w = PPW(m->width);
    dec->rect[0].h = dec->rect[1].h = m->height;
    dec->slice.h                    = m->height;
    return addr;
}

void strSync(DECENV* dec) // 0x801E3438
{
    volatile u_long count = WAIT_TIME;

    while (dec->isdone == 0)
    {
        if (--count == 0)
        {
            dec->isdone  = 1;
            dec->rectid  = dec->rectid ^ 1;
            dec->slice.x = dec->rect[dec->rectid].x;
            dec->slice.y = dec->rect[dec->rectid].y;
        }
    }

    dec->isdone = 0;
}
