#include "bodyprog/bodyprog.h"
#include "maps/common/bgm.h"
#include "bodyprog/events/bgm_update.h"
#include "maps/common/shared.h"

// BGM events for `MapType_DR` maps.

void Map_RoomBgmInit(bool updateSong)
{
    u32 bgmFlags = D_800DA578[g_SavegamePtr->mapRoomIdx];

    if (Savegame_EventFlagGet(EventFlag_354))
    {
        bgmFlags |= BgmFlag_Layer6 | BgmFlag_Layer4 | BgmFlag_Layer2;
    }

    Bgm_LayersUpdate(bgmFlags, Q12(0.25f), &D_800DA570);
}
