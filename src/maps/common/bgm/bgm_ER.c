#include "bodyprog/bodyprog.h"
#include "maps/common/bgm.h"
#include "bodyprog/events/bgm_update.h"
#include "maps/common/shared.h"

// BGM events for `MapType_ER` maps.
// `MAP6_S01` has map-specific BGM.

void Map_RoomBgmInit(bool updateSong)
{
    (updateSong != 0) ? Map_BgmSongChange() : Map_BgmLayersUpdate();
}

void Map_BgmSongChange(void)
{
#if defined(MAP6_S01)
    if (!Savegame_EventFlagGet(EventFlag_424) ||
        (Savegame_EventFlagGet(EventFlag_425) && !Savegame_EventFlagGet(EventFlag_426)))
    {
        Bgm_SongChange(BgmCmd_Track23);
    }
    else
    {
        Bgm_SongChange(BgmCmd_Track20);
    }
#else
    switch (g_SavegamePtr->mapRoomIdx)
    {
        case 5:
            Bgm_SongChange(BgmCmd_Track6);
            break;

        case 18:
            Bgm_SongChange(!Savegame_EventFlagGet(EventFlag_379) ? BgmCmd_Track29 : BgmCmd_Track7);
            break;

        case 17:
            Bgm_SongChange(BgmCmd_Track7);
            break;
    }
#endif
}

void Map_BgmLayersUpdate(void)
{
    s32               bgmFlags;
    q19_12            fadeSpeed;
    s_BgmLayerLimits* layerLimits;

    fadeSpeed   = Q12(0.1f);
    layerLimits = &g_BgmER_LayerLimits0;

    switch (g_SavegamePtr->mapRoomIdx)
    {
        case 1:
            fadeSpeed = Q12(240.0f);
            bgmFlags  = Savegame_EventFlagGet(EventFlag_182) ? (BgmFlag_Layer2 | BgmFlag_Layer3 | BgmFlag_Layer4 | BgmFlag_Layer5 |
                                                                BgmFlag_Layer6 | BgmFlag_Layer7 | BgmFlag_Layer8) :
                                                               (BgmFlag_Layer1 | BgmFlag_MuteAll);
            break;

        case 2:
        case 3:
        case 4:
            bgmFlags    = WorldEnv_IsFlashlightAllowed() ? BgmFlag_Layer2 : BgmFlag_Layer1;
            layerLimits = &g_BgmER_LayerLimits1;
            break;

        case 10:
        case 12:
        case 13:
            fadeSpeed   = Q12(0.25f);
            layerLimits = &g_BgmER_LayerLimits2;

            if (Savegame_EventFlagGet(EventFlag_312))
            {
                bgmFlags = BgmFlag_Layer3 | BgmFlag_Layer4;
            }
            else if (!Savegame_EventFlagGet(EventFlag_303))
            {
                if (Savegame_EventFlagGet(EventFlag_311))
                {
                    fadeSpeed = Q12(0.125f);
                    bgmFlags  = BgmFlag_Layer2;
                }
                else
                {
                    fadeSpeed = Q12(240.0f);
                    bgmFlags  = BgmFlag_Layer1 | BgmFlag_MuteAll;
                }
            }
            else
            {
                bgmFlags = BgmFlag_Layer4;
            }
            break;

        case 11:
            bgmFlags = BgmFlag_Layer1 | BgmFlag_MuteAll;
            break;

        case 17:
            layerLimits = &g_BgmER_LayerLimits3;
            bgmFlags    = BgmFlag_Layer1 | BgmFlag_MuteAll;
            break;

        case 18:
            layerLimits = (g_GameWork.bgmIdx == BgmCmd_Track7) ? &g_BgmER_LayerLimits4 : &g_BgmER_LayerLimits5;

            if (Savegame_EventFlagGet(EventFlag_379))
            {
                if (Savegame_EventFlagGet(EventFlag_381))
                {
                    fadeSpeed = Q12(240.0f);
                    Savegame_EventFlagClear(EventFlag_381);
                }
                else
                {
                    fadeSpeed = Q12(0.125f);
                }

                bgmFlags = BgmFlag_Layer1 | BgmFlag_KeepAlive | BgmFlag_MuteAll;
            }
            else if (Savegame_EventFlagGet(EventFlag_381))
            {
                fadeSpeed = Q12(240.0f);
                bgmFlags  = BgmFlag_Layer2 | BgmFlag_Layer3 | BgmFlag_Layer4 | BgmFlag_Layer5 |
                            BgmFlag_Layer6 | BgmFlag_Layer7 | BgmFlag_Layer8 |
                            BgmFlag_KeepAlive;
            }
            else
            {
                fadeSpeed = Savegame_EventFlagGet(EventFlag_378) ? Q12(0.25f) : Q12(0.125f);
                bgmFlags  = BgmFlag_Layer1 | BgmFlag_KeepAlive | BgmFlag_MuteAll;
            }
            break;

        case 6:
        case 7:
        case 8:
        case 9:
            if (g_GameWork.bgmIdx == BgmCmd_Track34)
            {
                if (Savegame_EventFlagGet(EventFlag_M5S03_KaufmannHasMotorcycleVial))
                {
                    fadeSpeed = Q12(240.0f);
                    bgmFlags  = BgmFlag_Layer1 | BgmFlag_MuteAll;
                }
                else if (Savegame_EventFlagGet(EventFlag_395))
                {
                    fadeSpeed = Q12(240.0f);
                    bgmFlags  = BgmFlag_Layer2 | BgmFlag_Layer3 | BgmFlag_Layer4 | BgmFlag_Layer5 |
                                BgmFlag_Layer6 | BgmFlag_Layer7 | BgmFlag_Layer8;
                }
                else
                {
                    bgmFlags = BgmFlag_Layer1 | BgmFlag_MuteAll;
                }
            }
            else
            {
                layerLimits = &g_BgmER_LayerLimits6;
                bgmFlags    = BgmFlag_Layer1;
            }
            break;

        case 5:
            bgmFlags = BgmFlag_Layer1;
            break;

        case 19:
        case 20:
            if (g_GameWork.bgmIdx == BgmCmd_Track23)
            {
                layerLimits = &g_BgmER_LayerLimits7;
                fadeSpeed   = Q12(0.25f);

                if (Savegame_EventFlagGet(EventFlag_423) && !Savegame_EventFlagGet(EventFlag_424))
                {
                    bgmFlags = Savegame_EventFlagGet(EventFlag_427) ? (BgmFlag_Layer2 | BgmFlag_Layer4 | BgmFlag_Layer5) :
                                                                      (BgmFlag_Layer2 | BgmFlag_Layer3 | BgmFlag_Layer4);
                }
                else if (Savegame_EventFlagGet(EventFlag_425) && !Savegame_EventFlagGet(EventFlag_426))
                {
                    bgmFlags = BgmFlag_Layer2 | BgmFlag_Layer3;
                }
                else
                {
                    bgmFlags = BgmFlag_Layer2;
                }
            }
            else
            {
                layerLimits = &g_BgmER_LayerLimits8;
                bgmFlags    = BgmFlag_Layer2 | BgmFlag_Layer3 | BgmFlag_Layer5;
            }
            break;

        default:
            bgmFlags = BgmFlag_Layer2 | BgmFlag_Layer3 | BgmFlag_Layer4 | BgmFlag_Layer5 |
                       BgmFlag_Layer6 | BgmFlag_Layer7 | BgmFlag_Layer8;
            break;
    }

    Bgm_LayersUpdate(bgmFlags, fadeSpeed, layerLimits);
}
