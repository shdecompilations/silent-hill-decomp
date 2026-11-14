#include "bodyprog/game.h"

#include <psyq/libetc.h>
#include <psyq/libpad.h>
#include <psyq/strings.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/sys/fs_screens.h"
#include "bodyprog/text/text_draw.h"
#include "main/fs_queue.h"

void GameFs_TitleGfxSeek(void) // 0x80032BD0
{
#if VERSION_REGION_IS(NTSCJ)
    Fs_QueueStartSeek(FILE_TIM_TITLE_TIM);
#else
    Fs_QueueStartSeek(FILE_TIM_TITLE_E_TIM);
#endif
}

void GameFs_TitleGfxLoad(void) // 0x80032BF0
{
#if VERSION_REGION_IS(NTSCJ)
    Fs_QueueStartReadTim(FILE_TIM_TITLE_TIM, FS_BUFFER_3, &g_TitleImg);
#else
    Fs_QueueStartReadTim(FILE_TIM_TITLE_E_TIM, FS_BUFFER_3, &g_TitleImg);
#endif
}

void GameFs_StreamBinSeek(void) // 0x80032C20
{
    Fs_QueueStartSeek(FILE_VIN_STREAM_BIN);
}

void GameFs_StreamBinLoad(void) // 0x80032C40
{
    Fs_QueueStartRead(FILE_VIN_STREAM_BIN, FS_BUFFER_1);
}

void GameFs_OptionBinLoad(void) // 0x80032C68
{
    Fs_QueueStartReadTim(FILE_TIM_OPTION_TIM, FS_BUFFER_1, &g_ItemInspectionImg);
    Fs_QueueStartRead(FILE_VIN_OPTION_BIN, FS_BUFFER_1);
}

void GameFs_SaveLoadBinLoad(void) // 0x80032CA8
{
    Fs_QueueStartReadTim(FILE_TIM_SAVELOAD_TIM, FS_BUFFER_1, &g_ItemInspectionImg);
    Fs_QueueStartRead(FILE_VIN_SAVELOAD_BIN, FS_BUFFER_1);
}

void GameFs_LoadingTextDraw(void) // 0x80032CE8
{
    Gfx_StringPositionSet(108, 104);
    Gfx_StringDraw("\7Now_loading.", 100);
}
