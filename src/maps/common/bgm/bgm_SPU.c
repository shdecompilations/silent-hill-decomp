#include "bodyprog/bodyprog.h"
#include "maps/common/bgm.h"
#include "bodyprog/events/bgm_update.h"
#include "maps/common/shared.h"

// BGM events for `MapType_SPU` maps.

void Map_RoomBgmInit(bool updateSong)
{
    s32               bgmFlags; // `e_BgmFlags`
    s32               fadeSpeed;
    s32               mapRoomIdx;
    s_BgmLayerLimits* var_a2;

    mapRoomIdx = g_SavegamePtr->mapRoomIdx;
    fadeSpeed  = Q12(0.1f);

    if (g_GameWork.bgmIdx == BgmCmd_Track13)
    {
        var_a2 = &g_BgmSPU_LayerLimits0;

        switch (mapRoomIdx)
        {
            case 15:
                bgmFlags = BgmFlag_Layer3;
                break;

            case 16:
                bgmFlags = BgmFlag_Layer2 | BgmFlag_Layer3;
                break;

            case 14:
                if (Savegame_EventFlagGet(EventFlag_352))
                {
                    fadeSpeed = Q12(10.0f);
                    bgmFlags  = BgmFlag_Layer1 | BgmFlag_KeepAlive;
                }
                else if (Savegame_EventFlagGet(EventFlag_350))
                {
                    fadeSpeed = Q12(1.0f / 8.0f);
                    bgmFlags  = BgmFlag_Layer1 | BgmFlag_KeepAlive;
                }
                else if (Savegame_EventFlagGet(EventFlag_348))
                {
                    if (NPC_CHARA(0).health < Q12(1000.0f))
                    {
                        bgmFlags = BgmFlag_Layer2 | BgmFlag_Layer3 | BgmFlag_Layer4 | BgmFlag_Layer5 |
                                   BgmFlag_Layer6 | BgmFlag_Layer7 | BgmFlag_Layer8 | BgmFlag_KeepAlive;
                    }
                    else
                    {
                        if (PLAYER_CHARA().position.vy > Q12(-0.5f))
                        {
                            sharedData_800ED420_4_s02 = BgmFlag_Layer2 | BgmFlag_Layer4 | BgmFlag_KeepAlive;
                        }
                        else if (PLAYER_CHARA().position.vy < Q12(-2.5f))
                        {
                            sharedData_800ED420_4_s02 = BgmFlag_Layer2 | BgmFlag_Layer5 | BgmFlag_KeepAlive;
                        }
                        bgmFlags = sharedData_800ED420_4_s02;

                        if (NPC_CHARA(0).health < Q12(2000.0f))
                        {
                            bgmFlags |= BgmFlag_Layer3 | BgmFlag_Layer6;
                        }
                        else if (NPC_CHARA(0).health < Q12(3000.0f))
                        {
                            bgmFlags |= BgmFlag_Layer3;
                        }
                    }
                }
                else
                {
                    fadeSpeed = Q12(1.0f);
                    bgmFlags  = BgmFlag_Layer1 | BgmFlag_KeepAlive;
                }
                break;

            default:
                bgmFlags = BgmFlag_Layer1;
                break;
        }
    }
    else if (g_GameWork.bgmIdx == BgmCmd_Track33)
    {
        var_a2 = &g_BgmSPU_LayerLimits1;

        switch (mapRoomIdx)
        {
            case 18:
                if  (Savegame_EventFlagGet(EventFlag_334) ||
                    !Savegame_EventFlagGet(EventFlag_333))
                {
                    bgmFlags = BgmFlag_Layer2;
                }
                else
                {
                    if (Savegame_EventFlagGet(EventFlag_330))
                    {
                        bgmFlags  = BgmFlag_Layer2 | BgmFlag_Layer3 | BgmFlag_Layer4 | BgmFlag_Layer5;
                        fadeSpeed = Q12(CHUNK_SIZE);
                    }
                    else
                    {
                        bgmFlags = BgmFlag_Layer2 | BgmFlag_Layer3 | BgmFlag_Layer4;
                    }
                }
                break;

            case 19:
                Savegame_EventFlagSet(EventFlag_334);

                if (Savegame_EventFlagGet(EventFlag_320))
                {
                    bgmFlags = BgmFlag_Layer1 | BgmFlag_KeepAlive;
                }
                else
                {
                    bgmFlags = BgmFlag_Layer3 | BgmFlag_KeepAlive;
                }
                break;

            case 21:
                bgmFlags = 8;
                break;

            case 20:
                if (Savegame_EventFlagGet(EventFlag_327))
                {
                    bgmFlags = BgmFlag_Layer5 | BgmFlag_KeepAlive;
                }
                else if (Savegame_EventFlagGet(EventFlag_324) || Savegame_EventFlagGet(EventFlag_329))
                {
                    fadeSpeed = Q12(0.5f);

                    if (NPC_CHARA(0).health > Q12(0.0f) &&
                        NPC_CHARA(0).position.vy < Q12(1.0f))
                    {
                        bgmFlags = BgmFlag_Layer6 | BgmFlag_Layer7 | BgmFlag_KeepAlive;
                    }
                    else
                    {
                        bgmFlags = BgmFlag_Layer6 | BgmFlag_KeepAlive;
                    }
                }
                else
                {
                    bgmFlags = BgmFlag_Layer5 | BgmFlag_KeepAlive;
                }
                break;

            default:
                bgmFlags = BgmFlag_Layer1 | BgmFlag_KeepAlive;
                break;
        }
    }
    else
    {
        var_a2 = &g_BgmSPU_LayerLimits2;

        if (!Savegame_EventFlagGet(EventFlag_314))
        {
            if (PLAYER_CHARA().position.vz < Q12(CHUNK_SIZE))
            {
                Savegame_EventFlagSet(EventFlag_314);
            }

            bgmFlags = BgmFlag_Layer2;
        }
        else
        {
            bgmFlags = sharedData_800ED42C_4_s02[mapRoomIdx];
        }
    }

    Bgm_LayersUpdate(bgmFlags, fadeSpeed, var_a2);
}
