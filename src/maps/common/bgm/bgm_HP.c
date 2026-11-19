#include "bodyprog/bodyprog.h"
#include "maps/common/bgm.h"
#include "bodyprog/events/bgm_update.h"
#include "maps/common/shared.h"

// BGM events for `MapType_HP` maps.

void Map_RoomBgmInit(bool updateSong)
{
    updateSong ? Map_BgmSongChange() : Map_BgmLayersUpdate();
}

void Map_BgmSongChange(void)
{
    Bgm_SongChange(!Savegame_EventFlagGet(EventFlag_295) ? BgmCmd_Track25 : BgmCmd_Track16);
}

void Map_BgmLayersUpdate(void)
{
    s32    bgmFlags; // `e_BgmFlags`
    q19_12 fadeSpeed;
    s32    mapRoomIdx;

    mapRoomIdx = g_SavegamePtr->mapRoomIdx;
    fadeSpeed  = Q12(0.15f);

    switch (g_GameWork.bgmIdx)
    {
        case BgmCmd_Track16:
            bgmFlags = sharedData_800D21E8_3_s00[mapRoomIdx];

            // Check a few specific room indices.
            switch (mapRoomIdx)
            {
                case 1:
                    if (!Savegame_EventFlagGet(EventFlag_197))
                    {
                        // Test player Z position against specific value.
                        if (PLAYER_CHARA().position.vz > Q12(143.2f))
                        {
                            bgmFlags = BgmFlag_Layer2 | BgmFlag_Layer3;
                        }
                        else
                        {
                            bgmFlags = BgmFlag_Layer2 | BgmFlag_Layer3 | BgmFlag_Layer4;
                        }
                    }
                    break;

                case 17:
                case 20:
                    if (Savegame_EventFlagGet(EventFlag_M3S01_GeneratorOn))
                    {
                        bgmFlags |= BgmFlag_Layer7;
                    }
                    break;
            }

            if (Savegame_EventFlagGet(EventFlag_MapMark_AltHospital2F_OperatingPrepRoomArrow) &&
                Savegame_EventFlagGet(EventFlag_MapMark_AltHospital2F_CorridorMidArrows))
            {
                bgmFlags |= BgmFlag_Layer2 | BgmFlag_Layer3 | BgmFlag_Layer4 | BgmFlag_Layer5 |
                            BgmFlag_KeepAlive;
            }
            break;

        case BgmCmd_Track34:
            if (!Savegame_EventFlagGet(EventFlag_MapMark_FogHospital1F_StairsLocked))
            {
                fadeSpeed = Q12(240.0f);
                bgmFlags  = BgmFlag_Layer1 | BgmFlag_MuteAll;
            }
            else if (Savegame_EventFlagGet(EventFlag_197))
            {
                fadeSpeed = Q12(0.2f);
                bgmFlags  = BgmFlag_Layer1 | BgmFlag_MuteAll;
            }
            else
            {
                fadeSpeed = Q12(240.0f);
                bgmFlags  = BgmFlag_Layer2 | BgmFlag_Layer3 | BgmFlag_Layer4 | BgmFlag_Layer5 |
                            BgmFlag_Layer6 | BgmFlag_Layer7 | BgmFlag_Layer8 | BgmFlag_KeepAlive;
            }
            break;

        case BgmCmd_Track25:
            if (!Savegame_EventFlagGet(EventFlag_295))
            {
                fadeSpeed = Q12(240.0f);
                bgmFlags  = BgmFlag_Layer2 | BgmFlag_Layer3 | BgmFlag_Layer4 | BgmFlag_Layer5 |
                            BgmFlag_Layer6 | BgmFlag_Layer7 | BgmFlag_Layer8 | BgmFlag_KeepAlive;
            }
            else
            {
                fadeSpeed = Q12(0.2f);
                bgmFlags  = BgmFlag_Layer1 | BgmFlag_KeepAlive | BgmFlag_MuteAll;
            }
            break;

        default:
            bgmFlags = BgmFlag_Layer1;
            break;
    }

    Bgm_LayersUpdate(bgmFlags, fadeSpeed, NULL);
}
