#include "bodyprog/bodyprog.h"
#include "maps/common/bgm.h"
#include "bodyprog/events/bgm_update.h"
#include "maps/common/shared.h"

// BGM events for `MapType_APU` maps.

void Map_RoomBgmInit(bool updateSong)
{
    (updateSong == true) ? Map_BgmSongChange() : Map_BgmLayersUpdate();
}

void Map_BgmSongChange(void)
{
    Bgm_SongChange(Savegame_EventFlagGet(EventFlag_440) ? BgmCmd_Track39 : BgmCmd_Track37);
}

void Map_BgmLayersUpdate(void)
{
    q19_12            dist0;
    s32               bgmFlags; // `e_BgmFlags`
    s32               fadeSpeed;
    s32               bgmCmd;
    q19_12            dist;
    s_BgmLayerLimits* layerLimits;

    fadeSpeed = Q12(0.2f);

    bgmCmd = g_GameWork.bgmIdx;
    if (bgmCmd == BgmCmd_Track36)
    {
        layerLimits = &g_BgmAPU_LayerLimits0;
        fadeSpeed   = Q12(0.1f);

        if (Savegame_EventFlagGet(EventFlag_465))
        {
            bgmFlags = BgmFlag_Layer6;
        }
        else if (!Savegame_EventFlagGet(EventFlag_464))
        {
            fadeSpeed = Q12(240.0f);
            bgmFlags  = BgmFlag_Layer2;
        }
        else
        {
            bgmFlags = BgmFlag_Layer5;
        }
    }
    else if (bgmCmd == BgmCmd_Track39)
    {
        layerLimits = &g_BgmAPU_LayerLimits1;

        if (Savegame_EventFlagGet(EventFlag_445))
        {
            if (Savegame_EventFlagGet(EventFlag_448))
            {
                if (Savegame_EventFlagGet(EventFlag_462))
                {
                    bgmFlags = BgmFlag_Layer5 | BgmFlag_KeepAlive;
                }
                else
                {
                    bgmFlags = BgmFlag_KeepAlive;
                }
            }
            else if (Savegame_EventFlagGet(EventFlag_470))
            {
                g_BgmAPU_LayerLimits1.limits[1] = 0x80;
                fadeSpeed                       = Q12(0.5f);

                if (!Savegame_EventFlagGet(EventFlag_447))
                {
                    if (Savegame_EventFlagGet(EventFlag_463))
                    {
                        bgmFlags = BgmFlag_Layer1 | BgmFlag_KeepAlive;
                    }
                    else
                    {
                        bgmFlags = BgmFlag_Layer2 | BgmFlag_KeepAlive;
                    }
                }
                else
                {
                    fadeSpeed = Q12(240.0f);
                    bgmFlags  = BgmFlag_Layer1 | BgmFlag_KeepAlive;
                }
            }
            else if (Savegame_EventFlagGet(EventFlag_446))
            {
                g_BgmAPU_LayerLimits1.limits[1] = 0;
                fadeSpeed                       = Q12(240.0f);
                bgmFlags                        = BgmFlag_Layer1 | BgmFlag_KeepAlive;
            }
            else
            {
                g_BgmAPU_LayerLimits1.limits[1] = 0x60;
                bgmFlags                        = BgmFlag_Layer2 | BgmFlag_Layer4 | BgmFlag_KeepAlive;
            }
        }
        else
        {
            g_BgmAPU_LayerLimits1.limits[1] = 0x60;

            if (Savegame_EventFlagGet(EventFlag_443))
            {
                // @unused Always overwritten, might be worth investigating.
                bgmFlags = BgmFlag_Layer4 | BgmFlag_KeepAlive;

                if (!Savegame_EventFlagGet(EventFlag_444))
                {
                    if (Savegame_EventFlagGet(EventFlag_455))
                    {
                        bgmFlags = BgmFlag_Layer2 | BgmFlag_Layer4 | BgmFlag_KeepAlive;
                    }
                }
                else
                {
                    bgmFlags = BgmFlag_Layer2 | BgmFlag_Layer4 | BgmFlag_KeepAlive;
                }
            }
            else
            {
                fadeSpeed = Q12(240.0f);

                if (Savegame_EventFlagGet(EventFlag_461))
                {
                    bgmFlags = BgmFlag_Layer3 | BgmFlag_KeepAlive;
                }
                else
                {
                    bgmFlags = BgmFlag_Layer1 | BgmFlag_KeepAlive | BgmFlag_MuteAll;
                }
            }
        }
    }
    else
    {
        dist        = Q12(0.0f);
        layerLimits = &g_BgmAPU_LayerLimits2;

        switch (g_SavegamePtr->mapRoomIdx)
        {
            case 3:
                bgmFlags = BgmFlag_Layer3;

                dist = Math_Distance2dGet(&PLAYER_CHARA().position, &g_BgmAPU_LayerLimits3);
                if (dist < Q12(10.0f))
                {
                    dist = Q12(1.0f / 32.0f);
                }
                else
                {
                    dist = Q12(1.0f / 32.0f) - Q12_MULT_PRECISE(dist - Q12(10.0f), Q12(1.0f / 256.0f));
                    if (dist < Q12(0.0f))
                    {
                        dist = Q12(0.0f);
                    }
                }
                break;

            case 1:
            case 2:
            case 4:
            case 5:
                bgmFlags = BgmFlag_Layer2;
                break;

            default:
                bgmFlags = BgmFlag_Layer1;
                break;
        }

        g_BgmAPU_LayerLimits2.limits[2] = dist;
    }

    Bgm_LayersUpdate(bgmFlags, fadeSpeed, layerLimits);
}
