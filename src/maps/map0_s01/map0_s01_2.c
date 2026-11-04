#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player/player.h"
#include "main/rng.h"
#include "maps/map0/map0_s01.h"
#include "maps/common/characters/cybil.h"
#include "maps/common/particle.h"
#include "maps/common/characters/air_screamer.h"
#include "maps/common/characters/player.h"
#include "inline_no_dmpsx.h"
#include <psyq/gtemac.h>

#include "../src/maps/common/particles/particle_glass.c"

#include "../src/maps/common/particles/particle.c"

#include "../src/maps/common/characters/player.c"

#include "../src/maps/common/characters/air_screamer.c" // 0x800D2170
#include "../src/maps/common/characters/cybil.c"

#include "../src/maps/common/chara_util.c" // 0x800D9B0C

#include "../src/maps/common/map_util.c" // 0x800DA51C

#include "maps/common/shared/Map_RoomBgmInit_CheckCond.h" // 0x800DA778

void Map_RoomBgmInit_CondTrue(void) // 0x800DA7AC
{
    s32 bgmIdx; // `e_BgmCmd`

    if (!Savegame_EventFlagGet(EventFlag_M0S01_AirScreamerDied))
    {
        if (!Savegame_EventFlagGet(EventFlag_49)) // If Cybil and Harry cutscene hasn't finished.
        {
            bgmIdx = BgmCmd_Track9;
        }
        else
        {
            bgmIdx = BgmCmd_Track30;
        }
    }
    else
    {
        bgmIdx = BgmCmd_Track6;
    }

    Bgm_SongChange(bgmIdx);
}

void Map_RoomBgmInit_CondFalse(void) // 0x800DA7FC
{
    s32    bgmFlags;
    q19_12 fadeSpeed;

    fadeSpeed = Q12(0.333f);
    if (!Savegame_EventFlagGet(EventFlag_146))
    {
        // Air Screamer isn't dead or the Cybil and Harry cutscene has started.
        if (!(Savegame_EventFlagGet(EventFlag_M0S01_AirScreamerDied) || Savegame_EventFlagGet(EventFlag_49)))
        {
            fadeSpeed = Q12(240.0f);

            // FMV has just played and unknown.
            if (Savegame_EventFlagGet(EventFlag_M0S01_FmvStarted) &&
                g_GameWork.bgmIdx == BgmCmd_Track9)
            {
                bgmFlags = BgmFlag_Layer2;
            }
            else
            {
                bgmFlags = BgmFlag_Layer1 | BgmFlag_MuteAll;
            }
        }
        // Air Screamer cutscene finished.
        else if (Savegame_EventFlagGet(EventFlag_47))
        {
            if (!Savegame_EventFlagGet(EventFlag_M0S01_AirScreamerDied))
            {
                fadeSpeed = Q12(240.0f);
                bgmFlags = BgmFlag_KeepAlive;
            }
            else
            {
                fadeSpeed = Q12(0.125f);
                bgmFlags  = BgmFlag_Layer1 | BgmFlag_MuteAll;
            }
        }
        else
        {
            bgmFlags = BgmFlag_Layer1;
        }
    }
    else
    {
        bgmFlags = BgmFlag_Layer1;
    }

    Bgm_LayersUpdate(bgmFlags, fadeSpeed, NULL);
}

void GameBoot_LoadScreen_StageString(void) // 0x800DA8A4
{
    ScreenFade_Start(false, true, false);

    Text_Debug_PositionSet(100, 100);
    Text_Debug_Draw("STAGE 0-1");
    GameBoot_LoadScreen_BackgroundImg();
}

#include "maps/common/shared/Event_CutsceneTimerAdvance.h" // 0x800DA8E8
