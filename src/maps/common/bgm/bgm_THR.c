#include "bodyprog/bodyprog.h"
#include "maps/common/bgm.h"
#include "bodyprog/events/bgm_update.h"
#include "maps/common/shared.h"

// BGM events for `MapType_THR` maps.
// `MAP0_S00` `MAP0_S01` has map-specific BGM.

void Map_RoomBgmInit(bool updateSong)
{
#if defined(MAP0_S01)
    (updateSong != 0) ? Map_BgmSongChange() : Map_BgmLayersUpdate();
#elif defined(MAP0_S00)
    s32    i;
    q19_12 fadeSpeed;
    s32    bgmFlags;
    u32    saveByte;

    // @hack Not used directly, but gets merged with  `Savegame_EventFlagGet` macros below.
    saveByte = g_SavegamePtr->eventFlags[0];
    fadeSpeed     = Q12(0.1f);
    bgmFlags    = 1 << 8;

    if (PLAYER_CHARA().health > 0 && (!(Savegame_EventFlagGet(EventFlag_23) && !Savegame_EventFlagGet(EventFlag_20))))
    {
        for (i = 1; i < 7; i++)
        {
            if (Savegame_EventFlagGet(D_800DF300[i]))
            {
                bgmFlags |= 1 << i;
            }
        }
    }
    else
    {
        Savegame_EventFlagClear(EventFlag_20);
        fadeSpeed = Q12(0.3f);
    }

    Bgm_LayersUpdate(bgmFlags, fadeSpeed, &g_BgmTHR_LayerLimits0);
#else
    extern s_BgmLayerLimits g_BgmTHR_LayerLimits1;
    s32       bgmFlags;
    s32       mapRoomIdx;

    mapRoomIdx = g_SavegamePtr->mapRoomIdx;
    bgmFlags   = WorldEnv_IsFlashlightAllowed() ? BgmFlag_Layer3 : BgmFlag_Layer2;

    switch (mapRoomIdx)
    {
        case 33:
            if (!Savegame_EventFlagGet(EventFlag_195))
            {
                if (Savegame_EventFlagGet(EventFlag_165))
                {
                    bgmFlags |= BgmFlag_Layer5;
                }
            }
            break;

        case 37:
        case 38:
            bgmFlags = WorldEnv_IsFlashlightAllowed() ? BgmFlag_Layer2 : BgmFlag_Layer1;
            break;

        case 27:
        case 30:
            bgmFlags |= BgmFlag_Layer2;
            break;

        case 35:
            bgmFlags = BgmFlag_Layer2 | BgmFlag_Layer3;
            break;
    }

    if (bgmFlags & BgmFlag_Layer4)
    {
        s8 tmp                       = func_800364BC();
        g_BgmTHR_LayerLimits1.limits[3] = tmp;
    }

    Bgm_LayersUpdate(bgmFlags, Q12(0.1f), &g_BgmTHR_LayerLimits1);
#endif
}

#if defined(MAP0_S01)
void Map_BgmSongChange(void)
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

void Map_BgmLayersUpdate(void)
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
#endif
