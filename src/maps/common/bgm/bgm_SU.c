#include "bodyprog/bodyprog.h"
#include "maps/common/bgm.h"
#include "bodyprog/events/bgm_update.h"
#include "maps/common/shared.h"

// BGM events for `MapType_SU` maps.
// `MAP1_S05` has map-specific BGM.

extern s_BgmLayerLimits sharedData_800DCC4C_1_s00;
extern u16 sharedData_800DCC54_1_s00[42]; // Flags.

void Map_RoomBgmInit(bool updateSong)
{
#if defined(MAP1_S05)
    s32 bgmFlags;

    bgmFlags = 1;
    if (NPC_CHARA(0).health > Q12(0.0f))
    {
        bgmFlags = 2;
        if (NPC_CHARA(0).properties.splitHead.flags & SplitHeadFlag_4)
        {
            bgmFlags = 6;
        }
    }

    Bgm_LayersUpdate(bgmFlags, Q12(0.1f), &D_800D5C3C);
#else
    s32 fadeSpeed;
    s32 roomIdx;
    s32 bgmFlags;

    roomIdx   = g_SavegamePtr->mapRoomIdx;
    bgmFlags  = sharedData_800DCC54_1_s00[roomIdx];
    fadeSpeed = Q12(0.1f);

    switch (roomIdx)
    {
        case 23:
            bgmFlags = BgmFlag_Layer1;
            if (Savegame_EventFlagGet(EventFlag_MapMark_AltSchool1F_CourtyardQuestion))
            {
                bgmFlags = BgmFlag_Layer2 | BgmFlag_Layer3 | BgmFlag_Layer4 | BgmFlag_Layer5 |
                           BgmFlag_Layer6 | BgmFlag_Layer7 | BgmFlag_Layer8 |
                           BgmFlag_KeepAlive;
                if (Savegame_EventFlagGet(EventFlag_MapMark_308) || Savegame_EventFlagGet(EventFlag_MapMark_AltSchool1F_CourtyardTopArrows))
                {
                    bgmFlags = BgmFlag_Layer5;
                }
            }
            break;

        case 38:
            if (Savegame_EventFlagGet(EventFlag_100) && !Savegame_EventFlagGet(EventFlag_107))
            {
                bgmFlags  = BgmFlag_Layer1;
                fadeSpeed = Q12(0.3f);
            }
            break;

        case 5:
            if (Savegame_EventFlagGet(EventFlag_121))
            {
                fadeSpeed = Q12(240.0f);

                if (Savegame_EventFlagGet(EventFlag_96))
                {
                    bgmFlags |= BgmFlag_Layer4;
                }
                else
                {
                    bgmFlags = BgmFlag_Layer1;
                }
            }
            break;

        case 11:
            Savegame_EventFlagClear(EventFlag_121);
            break;

        case 7:
            if (!Savegame_EventFlagGet(EventFlag_98) && Savegame_EventFlagGet(EventFlag_122))
            {
                fadeSpeed = Q12(240.0f);
                bgmFlags  = BgmFlag_Layer1;
            }
            break;
    }

    Bgm_LayersUpdate(bgmFlags, fadeSpeed, &sharedData_800DCC4C_1_s00);
#endif
}
