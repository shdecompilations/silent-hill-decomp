#include "bodyprog/game.h"

#include "bodyprog/sys/demo.h"
#include "bodyprog/screen/draw.h"
#include "bodyprog/sys/game_main.h"

void Screen_VSyncCallback(void) // 0x80032B80
{
    g_Demo_FrameCount++;
    g_WarmBootTimer++;

    g_SysWork.gameStateCounter++;
    g_SysWork.gameStateStepCounter++;
    g_SysWork.sysStateCounter++;
}
