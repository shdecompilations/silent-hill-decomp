#include "bodyprog/bodyprog.h"
#include "maps/common/bgm.h"
#include "bodyprog/events/bgm_update.h"
#include "maps/common/shared.h"

// BGM events for `MapType_SPR` maps.

void Map_RoomBgmInit(bool updateSong)
{
    s32 bgmFlags;

    bgmFlags = BgmFlag_Layer2;

    switch (g_SavegamePtr->mapRoomIdx)
    {
        case 17:
            if (Savegame_EventFlagGet(EventFlag_295) && !Savegame_EventFlagGet(EventFlag_352))
            {
                bgmFlags = BgmFlag_Layer2 | BgmFlag_Layer3;
            }
            break;

        case 12:
            if (!Savegame_EventFlagGet(EventFlag_295))
            {
                bgmFlags = BgmFlag_Layer2 | BgmFlag_Layer5;
            }
            break;
    }

    if (Savegame_EventFlagGet(EventFlag_M3S06_PickupAntiqueShopKey))
    {
        if (!Savegame_EventFlagGet(EventFlag_M2S02_AntiqueShopOpen))
        {
            bgmFlags |= BgmFlag_Layer4;
        }
    }

    if (bgmFlags & BgmFlag_Layer4)
    {
        s8 temp_v0 = func_800364BC();
        g_BgmSPR_LayerLimits0.limits[3] = temp_v0;
    }

    Bgm_LayersUpdate(bgmFlags, Q12(0.1f), &g_BgmSPR_LayerLimits0);
}
