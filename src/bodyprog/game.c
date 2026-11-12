#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "game.h"

s_SysWork  g_SysWork;
s_GameWork g_GameWork;

#if VERSION_IS(EUR)
    s32 D_800BC2F0;
#endif

s_GameWork* const       g_GameWorkConst = &g_GameWork;
s_Savegame* const       g_SavegamePtr   = &g_GameWork.savegame;
s_ControllerData* const g_Controller0   = &g_GameWork.controllers[0];
s_ControllerData* const g_Controller1   = &g_GameWork.controllers[1];
s_GameWork* const       g_GameWorkPtr   = &g_GameWork;
