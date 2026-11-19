#include "bodyprog/bodyprog.h"
#include "maps/common/bgm.h"
#include "bodyprog/events/bgm_update.h"
#include "maps/common/shared.h"

// BGM events for `MapType_SC` maps.
// `MAP1_S06` has map-specific BGM.

extern s_BgmLayerLimits g_BgmSC_LayerLimits1;
extern u16 sharedData_800DCC54_1_s00[42]; // Flags.

void Map_RoomBgmInit(bool updateSong)
{
#if defined(MAP1_S06)
    Bgm_LayersUpdate(Savegame_EventFlagGet(EventFlag_133) ? 0xFE : 1, Q12(0.1f), &g_BgmSC_LayerLimits0);
#else
    u16 bgmFlags;

    if (g_SavegamePtr->mapRoomIdx == 23)
    {
        bgmFlags = BgmFlag_Layer6;
        if (Savegame_EventFlagGet(EventFlag_71) && Savegame_EventFlagGet(EventFlag_72))
        {
            bgmFlags = BgmFlag_Layer2 | BgmFlag_Layer3 | BgmFlag_Layer4 | BgmFlag_Layer5 |
                       BgmFlag_Layer6 | BgmFlag_Layer7 | BgmFlag_Layer8 |
                       BgmFlag_KeepAlive;
        }
    }
    else
    {
        bgmFlags = sharedData_800DCC54_1_s00[g_SavegamePtr->mapRoomIdx];
    }

    Bgm_LayersUpdate(bgmFlags, Q12(0.1f), &g_BgmSC_LayerLimits1);
#endif
}
