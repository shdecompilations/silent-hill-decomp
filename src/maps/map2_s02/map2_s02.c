#include "bodyprog/bodyprog.h"
#include "bodyprog/events/event_data.h"
#include "bodyprog/gfx/map_effects.h"
#include "bodyprog/math/math.h"
#include "main/rng.h"
#include "maps/map2/map2_s02.h"
#include "maps/particle.h"
#include "maps/characters/player.h"
#include "maps/characters/romper.h"
#include "inline_no_dmpsx.h"
#include <psyq/gtemac.h>
#include "bodyprog/player/player.h"
#include "maps/characters/air_screamer.h"
#include "maps/characters/groaner.h"

#include "../src/maps/particles/particle.c"

#include "../src/maps/characters/player.c"

#include "../src/maps/characters/air_screamer.c" // 0x800D0728
#include "../src/maps/characters/groaner.c" // 0x800E1760

// TODO: Move this line into separate `Chara_Romper` split.
#include "../src/maps/characters/romper.c" // 0x800E5D38

#include "../src/maps/map_util.c" // 0x800E992C

#include "maps/shared/Map_RoomBgmInit_2_s02.h" // 0x800E9B38

void GameBoot_LoadScreen_StageString(void) {}

#include "maps/shared/MapEvent_DoorJammed.h" // 0x800E9C2C

#include "maps/shared/MapEvent_DoorLocked.h" // 0x800E9CC0

void func_800E9D54(void) // 0x800E9D54
{
    VECTOR3 sfxPos = { MAP_POINTS[g_MapEventData->mapPointIdx].positionX, Q12(-1.2f), MAP_POINTS[g_MapEventData->mapPointIdx].positionZ };

    Player_ItemRemove(InvItemId_AntiqueShopKey, 1);
    Event_DisplayMapMsgWithSfx(15, Sfx_UseKey, &sfxPos);
    Savegame_EventFlagSet(EventFlag_M2S02_AntiqueShopOpen);
}

const char* MAP_MESSAGES[] = {
    #include "maps/shared/map_msg_common.h"
#if VERSION_REGION_IS(NTSC)
    /* 15 */ "\tUsed_the_ ~C2 Antique_shop_key ~C7 . ~E ",
    /* 16 */ "~C3\tAlchemilla_Hospital ~E ",
    /* 17 */ "~C3\tgreen_lion ~E "
#elif VERSION_REGION_IS(NTSCJ)
    /* 15 */ "\t~C2 骨董屋の鍵 ~C7 を使った ~E ",
    /* 16 */ "~C3\tアルケミラ病院 ~E ",
    /* 17 */ "~C3\tｇｒｅｅｎ　ｌｉｏｎ ~E "
#endif
};

void func_800E9E10(void) // 0x800E9E10
{
    u32 pickupType;
    s32 eventFlagIdx;

    pickupType   = CommonPickupItemId_FirstAidKit;
    eventFlagIdx = 0;

    switch (g_MapEventData->mapPointIdx)
    {
        case 41:
            pickupType   = CommonPickupItemId_HealthDrink;
            eventFlagIdx = EventFlag_M2S02_HealthDrink0;
            break;

        case 42:
            pickupType   = CommonPickupItemId_HealthDrink;
            eventFlagIdx = EventFlag_M2S02_HealthDrink1;
            break;

        case 43:
            pickupType   = CommonPickupItemId_HandgunBullets;
            eventFlagIdx = EventFlag_M2S02_HandgunBullets0;
            break;

        case 44:
            pickupType   = CommonPickupItemId_HandgunBullets;
            eventFlagIdx = EventFlag_M2S02_HandgunBullets1;
            break;

        case 45:
            pickupType   = CommonPickupItemId_HealthDrink;
            eventFlagIdx = EventFlag_M2S02_HealthDrink2;
            break;
    }

    Event_CommonItemTake(pickupType, eventFlagIdx);
}

void func_800E9EAC(void) // 0x800E9EAC
{
    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();

            g_ScreenFade_Status = 12; // TODO: Can't be created with `ScreenFade_Start` macro?

            Sd_SfxStop(Sfx_Unk1522);
            SysWork_StateStepIncrement(0);
            break;

        case 1:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 1, Q12(0.25f), false);
            SysWork_StateStepIncrement(0);

        case 2:
            Event_WaitTimer(Q12(1.2f), false);
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            Savegame_EventFlagSet(EventFlag_191);
            Game_TurnFlashlightOff();
            func_8003A16C();
            break;
    }
}

void func_800E9FDC(void) // 0x800E9FDC
{
    g_DeltaTime = Q12(0.0f);

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            ScreenFade_ResetTimestep();
            SysWork_StateStepIncrement(0);

        case 1:
            if (Sd_AmbientSfxInit() != 0)
            {
                break;
            }

            SysWork_StateStepIncrement(0);

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            Savegame_EventFlagSet(EventFlag_189);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            break;
    }
}

void Map_WorldObjectsInit(void) // 0x800EA0E0
{
    if (Savegame_EventFlagGet(EventFlag_MapMark_FogCentralTown_AntiqueWholeSign))
    {
        Savegame_EventFlagSet(EventFlag_MapMark_AltCentralTown_AntiqueWholeSign);
    }

    D_800F0B2C = Q12(0.0f);

    if (g_SavegamePtr->gameDifficulty == GameDifficulty_Easy)
    {
        g_SysWork.npcFlagId = 2;
    }
    else if (g_SavegamePtr->gameDifficulty == GameDifficulty_Normal)
    {
        g_SysWork.npcFlagId = 3;
    }
    else
    {
        g_SysWork.npcFlagId = 4;
    }

    WorldObject_ModelNameSet(&g_CommonWorldObjects[0], g_CommonWorldObjectNames[2]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[1], g_CommonWorldObjectNames[3]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[2], g_CommonWorldObjectNames[4]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[3], g_CommonWorldObjectNames[5]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[4], g_CommonWorldObjectNames[6]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[5], g_CommonWorldObjectNames[7]);
}

void Map_WorldObjectsUpdate(void) // 0x800EA1C4
{
    MAP_CHUNK_CHECK_VARIABLE_DECL();

    Collision_TriggerFlagBitsClear(CollisionTriggerFlag_All);

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -3, -1, -3) && PLAYER_IN_MAP_CHUNK(vz, 1, -2, -1, -2))
    {
        if (Savegame_EventFlagGet(EventFlag_346))
        {
            Collision_TriggerFlagBitsSet(CollisionTriggerFlag_Block);
            Collision_TriggerFlagBitsClear(CollisionTriggerFlag_Objects);

            if (g_SysWork.playerWork.player.position.vz < Q12(-68.0f))
            {
                Savegame_EventFlagClear(EventFlag_346);
            }
        }
        else
        {
            Collision_TriggerFlagBitsClear(CollisionTriggerFlag_Block);
            Collision_TriggerFlagBitsSet(CollisionTriggerFlag_Objects);
        }
    }

    if (Savegame_EventFlagGet(EventFlag_191) && !Savegame_EventFlagGet(EventFlag_194))
    {
        if (PLAYER_IN_MAP_CHUNK(vx, 1, -3, -1, -3) && PLAYER_IN_MAP_CHUNK(vz, 1, 3, -1, 3))
        {
                if (g_SysWork.playerWork.player.position.vx > Q12(-116.0f))
                {
                    if (g_SysWork.playerWork.player.position.vy > Q12(0.0f))
                    {
                        if (g_ScreenFadeTimestep > Q12(0.0f))
                        {
                            g_ScreenFadeTimestep = MIN(g_ScreenFadeTimestep + g_DeltaTime, Q12(3.0f));
                        }
                    }
                }
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -6, -1, -6) && PLAYER_IN_MAP_CHUNK(vz, 1, 3, -1, 3))
    {
            Game_TurnFlashlightOff();
    }

    if (!Savegame_EventFlagGet(EventFlag_192))
    {
        if (PLAYER_IN_MAP_CHUNK(vx, 1, -3, -1, -3) && PLAYER_IN_MAP_CHUNK(vz, 1, -3, -1, -3))
        {
            if (D_800F0B2C == Q12(0.0f))
            {
                if (Math_Vector2MagCalcSafeQ6(g_SysWork.playerWork.player.position.vx + Q12(96.0f),
                                        g_SysWork.playerWork.player.position.vz + Q12(89.0f)) < Q12(4.0f))
                {
                    Sfx_WithFlagsPlay(Sfx_Unk1492, &D_800ED938, Q8_CLAMPED(0.766f), SfxFlag_NoDistAtten);
                    D_800F0B2C = Q12(0.3f);
                }
            }
        }

        if (D_800F0B2C > Q12(0.0f))
        {
            D_800F0B2C -= g_DeltaTime;
            if (D_800F0B2C < Q12(0.0f))
            {
                Savegame_EventFlagSet(EventFlag_192);
                Sfx_WithFlagsPlay(Sfx_Unk1492, &D_800ED938, Q8_CLAMPED(0.766f), SfxFlag_NoDistAtten);
            }
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -1, 0, 0) && PLAYER_IN_MAP_CHUNK(vz, 1, -2, -1, -2))
    {
        if (!Savegame_EventFlagGet(EventFlag_M2S02_HealthDrink0))
        {
            WorldObjects_Add(&g_CommonWorldObjects[1], &g_CommonWorldObjectPoses[0].position, &g_CommonWorldObjectPoses[0].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -1, 0, 0) && PLAYER_IN_MAP_CHUNK(vz, 1, -3, -1, -3))
    {
        if (!Savegame_EventFlagGet(EventFlag_M2S02_HealthDrink1))
        {
            WorldObjects_Add(&g_CommonWorldObjects[1], &g_CommonWorldObjectPoses[1].position, &g_CommonWorldObjectPoses[1].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -1, 0, 0) && PLAYER_IN_MAP_CHUNK(vz, 1, -3, -1, -3))
    {
        if (!Savegame_EventFlagGet(EventFlag_M2S02_HandgunBullets0))
        {
            WorldObjects_Add(&g_CommonWorldObjects[3], &g_CommonWorldObjectPoses[2].position, &g_CommonWorldObjectPoses[2].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -4, -1, -4) && PLAYER_IN_MAP_CHUNK(vz, 1, -1, 0, 0))
    {
        if (!Savegame_EventFlagGet(EventFlag_M2S02_HandgunBullets1))
        {
            WorldObjects_Add(&g_CommonWorldObjects[3], &g_CommonWorldObjectPoses[3].position, &g_CommonWorldObjectPoses[3].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -5, -1, -5) && PLAYER_IN_MAP_CHUNK(vz, 0, 0, -1, 1))
    {
        if (!Savegame_EventFlagGet(EventFlag_M2S02_HealthDrink2))
        {
            WorldObjects_Add(&g_CommonWorldObjects[1], &g_CommonWorldObjectPoses[4].position, &g_CommonWorldObjectPoses[4].rotation);
        }
    }
}

void func_800EAA50(void) // 0x800EAA50
{
    if (Savegame_EventFlagGet(EventFlag_191))
    {
        if (!Savegame_EventFlagGet(EventFlag_194))
        {
            Savegame_EventFlagSet(EventFlag_194);
            Sd_AmbientSfxInit();
        }
    }
}
