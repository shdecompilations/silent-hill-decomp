#include "bodyprog/bodyprog.h"
#include "maps/common/bgm.h"
#include "bodyprog/events/bgm_update.h"
#include "maps/common/shared.h"

// BGM events for `MapType_DRU` maps.

void Map_RoomBgmInit(bool updateSong)
{
    q19_12 dist0;
    q19_12 dist1;
    q19_12 dist2;
    s32    fadeSpeed;
    s32    roomIdx;
    s32    bgmFlags;

    roomIdx = g_SavegamePtr->mapRoomIdx;
    fadeSpeed = 0x400;

    if (Savegame_EventFlagGet(EventFlag_435))
    {
        bgmFlags = BgmFlag_Layer1;
        if (Savegame_EventFlagGet(EventFlag_437))
        {
            fadeSpeed = 0x547;
        }
        else
        {
            bgmFlags = D_800DBCE4[roomIdx];

            switch (roomIdx)
            {
                case 15:
                case 16:
                    D_800DBCDC.limits[5] = 0;
                    D_800DBCDC.limits[6] = 0;
                    D_800DBCDC.limits[7] = 0;
                    break;

                case 17:
                    // VECTOR3 D_800DBD10 = VECTOR3(23.0f, 0.0f, 20.0f); // in .data
                    dist0 = Math_Distance2dGet(&PLAYER_CHARA().position, &D_800DBD10);
                    if (dist0 < Q12(5.0f))
                    {
                        dist0 = Q8(0.313f);
                    }
                    else
                    {
                        dist0 = Q8(0.313f) - Q12_MULT_PRECISE((dist0 - Q12(5.0f)), 3);
                        if (dist0 < Q12(0.0f))
                        {
                            dist0 = Q12(0.0f);
                        }
                    }

                    D_800DBCDC.limits[5] = dist0;
                    D_800DBCDC.limits[6] = Q12(0.0f);
                    D_800DBCDC.limits[7] = Q12(0.0f);
                    break;

                case 18:
                case 19:
                case 20:
                    D_800DBCDC.limits[5] = Q8(0.313f);

                    dist1 = Q12(64.0f) - PLAYER_CHARA().position.vx;
                    if (dist1 < Q12(0.0f))
                    {
                        dist1 = Q8(0.313f);
                    }
                    else
                    {
                        dist1 = Q8(0.313f) - Q12_MULT_PRECISE(dist1, 3);
                        if (dist1 < Q12(0.0f))
                        {
                            dist1 = Q12(0.0f);
                        }
                    }

                    D_800DBCDC.limits[6] = dist1;

                    dist2 = Q12(80.0f) - PLAYER_CHARA().position.vx;
                    if (dist2 < Q12(0.0f))
                    {
                        dist2 = Q8(0.313f);
                    }
                    else
                    {
                        dist2 = Q8(0.313f) - Q12_MULT_PRECISE(dist2, 5);
                        if (dist2 < Q12(0.0f))
                        {
                            dist2 = Q12(0.0f);
                        }
                    }

                    D_800DBCDC.limits[7] = dist2;
                    break;
            }
        }
    }
    else
    {
        bgmFlags = (1 << 1) | (1 << 2);
        if (!Savegame_EventFlagGet(EventFlag_431))
        {
            fadeSpeed = 0x547;
            bgmFlags = BgmFlag_Layer2;
        }
    }

    Bgm_LayersUpdate(bgmFlags, fadeSpeed, &D_800DBCDC);
}
