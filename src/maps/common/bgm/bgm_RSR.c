#include "bodyprog/bodyprog.h"
#include "bodyprog/events/bgm_update.h"
#include "maps/common/shared.h"

// BGM events for `MapType_RSR` maps.

void Map_RoomBgmInit(bool updateSong)
{
    s32    temp_v0;
    q19_12 posX;
    q19_12 posZ;
    q19_12 newPosComp;
    s32    var_v1;
    s32    bgmFlags;

    posX  = PLAYER_CHARA().position.vx;
    posZ  = PLAYER_CHARA().position.vz;
    bgmFlags = D_800EFC80[g_SavegamePtr->mapRoomIdx];

    if (g_SavegamePtr->mapRoomIdx == 10)
    {
        posX += Q12(150.0f);
        posZ  = Q12(-25.0f) - posZ;
        if (posX < posZ)
        {
            newPosComp = posX;
        }
        else
        {
            newPosComp = posZ;
        }

        newPosComp /= 2;
    }
    else
    {
        if (posZ > Q12(-120.0f))
        {
            posX = posX - Q12(16.0f);
            posZ = Q12(-48.0f) - posZ;
            if (posX < posZ)
            {
                newPosComp = posX;
            }
            else
            {
                newPosComp = posZ;
            }
        }
        else
        {
            if (posX > Q12(0.0f))
            {
                posX = posX - Q12(16.0f);
            }
            else
            {
                posX = Q12(-52.0f) - posX;
            }

            posZ = Q12(-144.0f) - posZ;

            if (posZ < posX)
            {
                newPosComp = posX;
            }
            else
            {
                newPosComp = posZ;
            }
        }
    }

    D_800EFC7C += g_DeltaTime;
    if (newPosComp < Q12(0.0f))
    {
        newPosComp = Q12(0.0f);
    }
    else if (newPosComp >= Q12(32.0f))
    {
        newPosComp = Q12(32.0f) - 1;
    }

    temp_v0 = (Math_Sin(D_800EFC7C / 12) + Q12(2.0f)) / 2;
    var_v1  = Q12_MULT_PRECISE(newPosComp, temp_v0);

    D_800EFC74.limits[4] = var_v1 / Q12(0.5f);
    Bgm_LayersUpdate(bgmFlags, Q12(0.2f), &D_800EFC74);
}
