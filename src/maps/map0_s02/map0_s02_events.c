#include "bodyprog/bodyprog.h"
#include "bodyprog/events/event_data.h"
#include "bodyprog/gfx/map_effects.h"
#include "bodyprog/math/math.h"
#include "main/rng.h"
#include "maps/map0/map0_s02.h"

const char* MAP_MESSAGES[] = {
    #include "maps/shared/map_msg_common.h"
#if VERSION_REGION_IS(NTSC)
    /* 15 */ "\tThere_is_a_ ~C2 Chain_saw ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 16 */ "\tSome_oil_cans_are_on_the_shelf. ~E ",
    /* 17 */ "\tA_car_engine. ~N\n\tNo_problem_there. ~E ",
    /* 18 */ "\tThere's_a_TV_set. ~N\n\tIt_doesn't_turn_on. ~E ",
    /* 19 */ "\tThere_is_a_ ~C2 Gasoline_tank ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 20 */ "\tThere_is_a_ ~C2 Katana ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 21 */ "\tThere_is_a_ ~C2 Channeling_stone ~C7 . ~N\n\tTake_it? ~S4 "
#elif VERSION_REGION_IS(NTSCJ)
    /* 15 */ "\t~C2 チェーンソー ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 16 */ "\t棚にオイルの缶が置かれている ~E ",
    /* 17 */ "\t車のエンジンだ ~N\n\t特に気にする必要はないだろう ~E ",
    /* 18 */ "\tテレビが置かれている ~N\n\t電源を押しても何の反応もないな ~E ",
    /* 19 */ "\t~C2 ガソリン ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 20 */ "\t~C2 日本刀 ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 21 */ "\t~C2 チャネリングストーン ~C7 がある ~N\n\t取りますか？ ~S4 "
#endif
};

void MapEvent_CommonItemTake(void) // 0x800CEBC8
{
    u32 pickupType;
    s32 eventFlagIdx;

    pickupType   = CommonPickupItemId_FirstAidKit;
    eventFlagIdx = 0;
    switch (g_MapEventData->mapPointIdx)
    {
        case 29:
            pickupType   = CommonPickupItemId_FirstAidKit;
            eventFlagIdx = EventFlag_M0S02_FirstAidKit;
            break;

        case 30:
            pickupType   = CommonPickupItemId_HealthDrink;
            eventFlagIdx = EventFlag_M0S02_HealthDrink0;
            break;

        case 31:
            pickupType   = CommonPickupItemId_HealthDrink;
            eventFlagIdx = EventFlag_M0S02_HealthDrink1;
            break;

        case 32:
            pickupType   = CommonPickupItemId_HealthDrink;
            eventFlagIdx = EventFlag_M0S02_HealthDrink2;
            break;

        case 33:
            pickupType   = CommonPickupItemId_HandgunBullets;
            eventFlagIdx = EventFlag_M0S02_HandgunBullets;
            break;

        case 34:
            pickupType   = CommonPickupItemId_HealthDrink;
            eventFlagIdx = EventFlag_M0S02_HealthDrink3;
            break;

        case 35:
            pickupType   = CommonPickupItemId_HealthDrink;
            eventFlagIdx = EventFlag_M0S02_HealthDrink4;
            break;
    }

    Event_CommonItemTake(pickupType, eventFlagIdx);
}

void MapEvent_EmptyFunction(void) {}

void MapEvent_GasolineTankItemTake(void) // 0x800CEC84
{
    Event_ItemTake(InvItemId_GasolineTank, DEFAULT_PICKUP_ITEM_COUNT, EventFlag_M0S02_PickupGasolineTank, 19);
}

void MapEvent_KatanaItemTake(void) // 0x800CECB0
{
    Event_ItemTake(InvItemId_Katana, DEFAULT_PICKUP_ITEM_COUNT, EventFlag_M0S02_PickupKatana, 20);
}

void MapEvent_ChannelingStoneItemTake(void) // 0x800CECDC
{
    Event_ItemTake(InvItemId_ChannelingStone, DEFAULT_PICKUP_ITEM_COUNT, EventFlag_M0S02_PickupChannelingStone, 21);
}

extern s_WorldObjectModel g_WorldObject_Winr[4][2];
extern VECTOR3 g_WorldObjectPose_Winr;
extern s_WorldObjectModel g_WorldObject_SavePad;
extern s_Pose g_WorldObjectPose_SavePad0;
extern s_Pose g_WorldObjectPose_SavePad1;
extern s_Pose g_WorldObjectPose_SavePad2;
extern s_WorldObjectPose g_WorldObject_Gas;
extern s_WorldObjectPose g_WorldObject_Katana;
extern u8 D_800D3474;
extern s_WorldObjectPose g_WorldObject_Cosmo;
extern s_WorldObjectModel g_CommonWorldObjects[6];
extern s_Pose g_CommonWorldObjectPoses[7];

void Map_WorldObjectsInit(void)
{
    D_800D3474 = 0;
    if (!Savegame_EventFlagGet(EventFlag_131) && Savegame_EventFlagGet(EventFlag_159))
    {
        Gfx_MapEnvSet(7, 4);
    }

    WorldObject_ModelNameSet(&g_WorldObject_Winr[0][0], "WINR_0_N");
    WorldObject_ModelNameSet(&g_WorldObject_Winr[1][0], "WINR_1_H");
    WorldObject_ModelNameSet(&g_WorldObject_Winr[2][0], "WINR_2_H");
    WorldObject_ModelNameSet(&g_WorldObject_Winr[3][0], "WINR_4_N");
    WorldObject_ModelNameSet(&g_WorldObject_Winr[0][1], "WINU_0_N");
    WorldObject_ModelNameSet(&g_WorldObject_Winr[1][1], "WINU_1_H");
    WorldObject_ModelNameSet(&g_WorldObject_Winr[2][1], "WINU_2_H");
    WorldObject_ModelNameSet(&g_WorldObject_Winr[3][1], "WINU_4_N");

    Math_Vector3Set(&g_WorldObjectPose_Winr, Q12(-100.0f), Q12(0.0f), Q12(20.0f));
    WorldObject_ModelNameSet(&g_WorldObject_SavePad, g_CommonWorldObjectNames[1]);

    WorldObject_PoseInit(&g_WorldObjectPose_SavePad0, -100.47f, -1.575f, 22.22f, 0.0f, -75.8f, 0.0f);
    WorldObject_PoseInit(&g_WorldObjectPose_SavePad1, -178.08f, -0.705f, 22.0298f, 0.0f, 12.1f, 0.0f);
    WorldObject_PoseInit(&g_WorldObjectPose_SavePad2, -146.4248f, -0.911f, 21.325f, 0.0f, 8.0f, 0.0f);
    WorldObject_Init(&g_WorldObject_Gas, "GAS_HIDE", -177.2998f, -1.2522f, 21.125f, 0.0f, -145.55f, 0.0f);
    WorldObject_PosePositionInit(&g_WorldObject_Katana, "KATANA1_", -140.8628f, -0.5511f, 62.3404f);
    WorldObject_Init(&g_WorldObject_Cosmo, "COSMO_HI", -145.75f, -0.92f, 18.3274f, 0.0f, -55.1f, 0.0f);

    WorldObject_ModelNameSet(&g_CommonWorldObjects[0], g_CommonWorldObjectNames[2]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[1], g_CommonWorldObjectNames[3]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[2], g_CommonWorldObjectNames[4]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[3], g_CommonWorldObjectNames[5]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[4], g_CommonWorldObjectNames[6]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[5], g_CommonWorldObjectNames[7]);
}

void Map_WorldObjectsUpdate(void)
{
    u32 idx;
    s32 i;
    MAP_CHUNK_CHECK_VARIABLE_DECL();
    if (PLAYER_IN_MAP_CHUNK(vx, 1, -3, -1, -3) && PLAYER_IN_MAP_CHUNK(vz, 0, 0, -1, 1))
    {
        WorldObjects_Add(&g_WorldObject_SavePad, &g_WorldObjectPose_SavePad0.position, &g_WorldObjectPose_SavePad0.rotation);

        idx = Savegame_EventFlagGet(EventFlag_131);
        idx = !idx;
        for (i = 0; i < 4; i++)
        {
            WorldObjects_Add(&g_WorldObject_Winr[i][idx], &g_WorldObjectPose_Winr, &SVECTOR3_Zero);
        }
    }

     if (PLAYER_IN_MAP_CHUNK(vx, 1, -5, -1, -5) && PLAYER_IN_MAP_CHUNK(vz, 0, 0, -1, 1))
     {
        WorldObjects_Add(&g_WorldObject_SavePad, &g_WorldObjectPose_SavePad1.position, &g_WorldObjectPose_SavePad1.rotation);
        if (Savegame_EventFlagGet(EventFlag_M0S02_UnlockedGasolineTank) && !Savegame_EventFlagGet(EventFlag_M0S02_PickupGasolineTank))
        {
            WorldObjects_Add(&g_WorldObject_Gas.object, &g_WorldObject_Gas.position, &g_WorldObject_Gas.rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -4, -1, -4) && PLAYER_IN_MAP_CHUNK(vz, 0, 0, -1, 1))
    {
        WorldObjects_Add(&g_WorldObject_SavePad, &g_WorldObjectPose_SavePad2.position, &g_WorldObjectPose_SavePad2.rotation);
        if (Savegame_EventFlagGet(EventFlag_M0S02_UnlockedChannelingStone) && !Savegame_EventFlagGet(EventFlag_M0S02_PickupChannelingStone))
        {
            WorldObjects_Add(&g_WorldObject_Cosmo.object, &g_WorldObject_Cosmo.position, &g_WorldObject_Cosmo.rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -4, -1, -4) && PLAYER_IN_MAP_CHUNK(vz, 1, 2, -1, 2))
    {
        if (D_800D3474 == 0)
        {
            if (Savegame_EventFlagGet(EventFlag_134) || !Savegame_EventFlagGet(EventFlag_159))
            {
                Gfx_MapEnvSet(13, 13);
                D_800D3474 = 1;
            }
        }
        if (!Savegame_EventFlagGet(EventFlag_M0S02_PickupKatana))
        {
            WorldObjects_Add(&g_WorldObject_Katana.object, &g_WorldObject_Katana.position, &SVECTOR3_Zero);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -4, -1, -4) && PLAYER_IN_MAP_CHUNK(vz, 0, 0, -1, 1))
    {
        if (!Savegame_EventFlagGet(EventFlag_M0S02_FirstAidKit))
        {
            WorldObjects_Add(&g_CommonWorldObjects[0], &g_CommonWorldObjectPoses[0].position, &g_CommonWorldObjectPoses[0].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -4, -1, -4) && PLAYER_IN_MAP_CHUNK(vz, 0, 0, -1, 1))
    {
        if (!Savegame_EventFlagGet(EventFlag_M0S02_HealthDrink0))
        {
             WorldObjects_Add(&g_CommonWorldObjects[1], &g_CommonWorldObjectPoses[1].position, &g_CommonWorldObjectPoses[1].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -4, -1, -4) && PLAYER_IN_MAP_CHUNK(vz, 0, 0, -1, 1))
    {

        if (!Savegame_EventFlagGet(EventFlag_M0S02_HealthDrink1))
        {
            WorldObjects_Add(&g_CommonWorldObjects[1], &g_CommonWorldObjectPoses[2].position, &g_CommonWorldObjectPoses[2].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -4, -1, -4) && PLAYER_IN_MAP_CHUNK(vz, 0, 0, -1, 1))
    {
        if (!Savegame_EventFlagGet(EventFlag_M0S02_HealthDrink2))
        {
            WorldObjects_Add(&g_CommonWorldObjects[1], &g_CommonWorldObjectPoses[3].position, &g_CommonWorldObjectPoses[3].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -5, -1, -5) && PLAYER_IN_MAP_CHUNK(vz, 0, 0, -1, 1))
    {
        if (!Savegame_EventFlagGet(EventFlag_M0S02_HandgunBullets))
        {
             WorldObjects_Add(&g_CommonWorldObjects[3], &g_CommonWorldObjectPoses[4].position, &g_CommonWorldObjectPoses[4].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -3, -1, -3) && PLAYER_IN_MAP_CHUNK(vz, 0, 0, -1, 1))
    {
        if (!Savegame_EventFlagGet(EventFlag_M0S02_HealthDrink3))
        {
             WorldObjects_Add(&g_CommonWorldObjects[1], &g_CommonWorldObjectPoses[5].position, &g_CommonWorldObjectPoses[5].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -3, -1, -3) && PLAYER_IN_MAP_CHUNK(vz, 0, 0, -1, 1))
    {
        if (!Savegame_EventFlagGet(EventFlag_M0S02_HealthDrink4))
        {
            WorldObjects_Add(&g_CommonWorldObjects[1], &g_CommonWorldObjectPoses[6].position, &g_CommonWorldObjectPoses[6].rotation);
        }
    }
}

