#include "bodyprog/bodyprog.h"
#include "bodyprog/events/event_data.h"
#include "bodyprog/gfx/map_effects.h"
#include "bodyprog/math/math.h"
#include "main/rng.h"
#include "maps/map2/map2_s04.h"
#include "maps/particle.h"
#include "maps/characters/player.h"

#include "../src/maps/particles/particle.c"

#include "../src/maps/characters/player.c"

#include "../src/maps/map_util.c" // 0x800CCA88

#include "maps/shared/Map_RoomBgmInit_CheckCond.h" // 0x800CCB30

#include "maps/shared/Map_RoomBgmInit_0_s02_CondTrue.h" // 0x800CCB64

#include "maps/shared/Map_RoomBgmInit_0_s02_CondFalse.h" // 0x800CCBE0

void GameBoot_LoadScreen_StageString(void) {} // 0x800CCE6C

#include "maps/shared/MapEvent_DoorJammed.h" // 0x800CCE74

#include "maps/shared/MapEvent_DoorLocked.h" // 0x800CCF08

const char* MAP_MESSAGES[] = {
    #include "maps/shared/map_msg_common.h"
#if VERSION_REGION_IS(NTSC)
    /* 15 */ "\tThere's_a_memo_on_the_desk. ",
    /* 16 */ "~C3\tCoroner_Seals_called. ",
    /* 17 */ "~C3\tOfficer_Gucci_is_unlikely_to ~N\n\t\tbe_murdered._He_apparently ~N\n\t\tdied_naturally. ",
    /* 18 */ "~C3\tBut,_medical_records_show ~N\n\t\tOfficer_Gucci_had_no_prior ~N\n\t\tsymptoms_of_heart_disease. ~E ",
    /* 19 */ "\tThere's_something_on_it. ",
    /* 20 */ "~C3\tProduct_only_available_in ~N\n\t\tselected_areas_of_Silent_Hill. ",
    /* 21 */ "~C3\tRaw_material_is_White_Claudia, ~N\n\t\ta_plant_peculiar_to_the_region. ",
    /* 22 */ "~C3\tManufactured_here? ~N\n\t\tDealer_*_manufacturer? ~E ",
    /* 23 */ "~H\tThere_is_a_ ~C5 Shopping_district_map ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 24 */ "\tNothing_helpful. ~E ",
    /* 25 */ "\tThe_desk_is_messy. ~E ",
    /* 26 */ "\tThere_is_a_ ~C2 Shotgun ~C7 . ~N\n\tTake_it? ~S4 "
#elif VERSION_REGION_IS(NTSCJ)
    /* 15 */ "\tメモが置いてある ",
    /* 16 */ "~C3\tシールズ医師より電話。 ~N\n\t\tグッジ捜査官が殺された可能性はきわめて ~N\n\t\t低いとのこと。自然死らしいです。 ",
    /* 17 */ "~C3\tただし、グッジ捜査官は過去の検査で、 ~N\n\t\t心臓の疾患が発見されたことは ~N\n\t\tなかったとも言っていました。 ~E ",
    /* 18 */ "\t何か書いてある ",
    /* 19 */ "~C3\tサイレントヒルを中心にした ~N\n\t\t特定地域のみで出回っている。 ",
    /* 20 */ "~C3\t原料となるホワイトクロジェアは ~N\n\t\tこの地方特有の植物。 ",
    /* 21 */ "~C3\t生産地もこの近辺？ ~N\n\t\t販売者＝生産者？ ~E ",
    /* 22 */ "~H\t~C5 繁華街の地図 ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 23 */ "\t特に気になるものはない ~E ",
    /* 24 */ "\t机の上は散らかっている ~E ",
    /* 25 */ "\t~C2 ショットガン ~C7 が落ちている ~N\n\t取りますか？ ~S4 "
#endif
};

void func_800CCF9C(void) // 0x800CCF9C
{
    u32 pickupType;
    s32 eventFlagIdx;

    pickupType   = CommonPickupItemId_FirstAidKit;
    eventFlagIdx = 0;

    switch (g_MapEventData->mapPointIdx)
    {
        case 24:
            pickupType   = CommonPickupItemId_ShotgunShells;
            eventFlagIdx = EventFlag_M2S04_ShotgunShells0;
            break;

        case 25:
            pickupType   = CommonPickupItemId_HandgunBullets;
            eventFlagIdx = EventFlag_M2S04_HandgunBullets0;
            break;

        case 26:
            pickupType   = CommonPickupItemId_HandgunBullets;
            eventFlagIdx = EventFlag_M2S04_HandgunBullets1;
            break;

        case 27:
            pickupType   = CommonPickupItemId_ShotgunShells;
            eventFlagIdx = EventFlag_M2S04_ShotgunShells1;
            break;

        case 28:
            pickupType   = CommonPickupItemId_HandgunBullets;
            eventFlagIdx = EventFlag_M2S04_HandgunBullets2;
            break;

        case 29:
            pickupType   = CommonPickupItemId_HandgunBullets;
            eventFlagIdx = EventFlag_M2S04_HandgunBullets3;
            break;

        case 30:
            pickupType   = CommonPickupItemId_RifleShells;
            eventFlagIdx = EventFlag_M2S04_RifleShells;
            break;
    }

    Event_CommonItemTake(pickupType, eventFlagIdx);
}

void func_800CD050(void) // 0x800CD050
{
    g_Screen_BackgroundImgGamma = Q8(10.0f / 32.0f);
    Event_DisplayMapMsgWithBg(FILE_TIM_PLCSMEMO_TIM, Q12(0.0f), Q12(0.0f), 15);
}

void MapEvent_PoliceChalkboardInspect(void) // 0x800CD088
{
    Event_DisplayMapMsgWithBg(FILE_TIM_PLCSBORD_TIM, Q12(0.0f), Q12(0.0f), MAP_MSG(19));
    Savegame_EventFlagSet(EventFlag_M2S04_SeenPoliceChalkboard);
}

void MapEvent_PaperMapTake(void) // 0x800CD0D0
{
    Event_PaperMapTake(2, EventFlag_M2S00_PickupMap, MAP_MSG(23));
}

void MapEvent_ShotgunTake(void) // 0x800CD0F8
{
    Event_ItemTake(InvItemId_Shotgun, SHOTGUN_AMMO_PICKUP_ITEM_COUNT, EventFlag_M1S02_PickupShotgun, MAP_MSG(26));
}

extern u8 D_800CF280;
extern s_WorldObjectModel g_CommonWorldObjects[8];
extern s_WorldObjectPose g_WorldObject_Map;
extern s_WorldObjectPose g_WorldObject_SavePad0;
extern s_WorldObjectPose g_WorldObject_SavePad1;
extern s_WorldObjectPose g_WorldObject_Shotgun;
extern s_Pose g_CommonWorldObjectPoses[7];

void Map_WorldObjectsInit(void)
{
    D_800CF280 = 0;

    WorldObject_Init(&g_WorldObject_SavePad0, g_CommonWorldObjectNames[1], -60.9976f, -0.8289f, 20.1067f, 0.0f, -100.7f, 0.0f);
    WorldObject_Init(&g_WorldObject_Map, "MAP_HIDE", -20.0f, 0.0f, 20.45f, 0, 180.0f, 0);
    WorldObject_Init(&g_WorldObject_SavePad1, g_CommonWorldObjectNames[1], -102.8699f, -1.25f, 57.15f, 0, -117.55f, 0);
    WorldObject_Init(&g_WorldObject_Shotgun, "SHOTGUN_", -59.484f, -0.8443f, 18.1517f, 0, -23.3f, 0);

    WorldObject_ModelNameSet(&g_CommonWorldObjects[0], g_CommonWorldObjectNames[2]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[1], g_CommonWorldObjectNames[3]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[2], g_CommonWorldObjectNames[4]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[3], g_CommonWorldObjectNames[5]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[4], g_CommonWorldObjectNames[6]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[5], g_CommonWorldObjectNames[7]);
}

void Map_WorldObjectsUpdate(void)
{
    MAP_CHUNK_CHECK_VARIABLE_DECL();

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -2, -1, -2) && PLAYER_IN_MAP_CHUNK(vz, 0, 0, -1, 1))
    {
        WorldObjects_Add(&g_WorldObject_SavePad0.object, &g_WorldObject_SavePad0.position, &g_WorldObject_SavePad0.rotation);

        if (!Savegame_EventFlagGet(EventFlag_M1S02_PickupShotgun))
        {
            WorldObjects_Add(&g_WorldObject_Shotgun.object, &g_WorldObject_Shotgun.position, &g_WorldObject_Shotgun.rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -1, 0, 0) && PLAYER_IN_MAP_CHUNK(vz, 0, 0, -1, 1))
    {
        if (!Savegame_EventFlagGet(EventFlag_M2S00_PickupMap))
        {
            WorldObjects_Add(&g_WorldObject_Map.object, &g_WorldObject_Map.position, &g_WorldObject_Map.rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -3, -1, -3) && PLAYER_IN_MAP_CHUNK(vz, 1, 2, -1, 2))
    {
        if (!D_800CF280)
        {
            Gfx_MapEnvSet(7, 4);
            D_800CF280++;
        }

        WorldObjects_Add(&g_WorldObject_SavePad1.object, &g_WorldObject_SavePad1.position, &g_WorldObject_SavePad1.rotation);
    }
    else
    {
        D_800CF280 = 0;
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -1, 0, 0) && PLAYER_IN_MAP_CHUNK(vz, 0, 0, -1, 1))
    {
        if (!Savegame_EventFlagGet(EventFlag_M2S04_ShotgunShells0))
        {
            WorldObjects_Add(&g_CommonWorldObjects[5], &g_CommonWorldObjectPoses[0].position, &g_CommonWorldObjectPoses[0].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -1, 0, 0) && PLAYER_IN_MAP_CHUNK(vz, 0, 0, -1, 1))
    {
        if (!Savegame_EventFlagGet(EventFlag_M2S04_HandgunBullets0))
        {
            WorldObjects_Add(&g_CommonWorldObjects[3], &g_CommonWorldObjectPoses[1].position, &g_CommonWorldObjectPoses[1].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -1, 0, 0) && PLAYER_IN_MAP_CHUNK(vz, 0, 0, -1, 1))
    {
        if (!Savegame_EventFlagGet(EventFlag_M2S04_HandgunBullets1))
        {
            WorldObjects_Add(&g_CommonWorldObjects[3], &g_CommonWorldObjectPoses[2].position, &g_CommonWorldObjectPoses[2].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -2, -1, -2) && PLAYER_IN_MAP_CHUNK(vz, 0, 0, -1, 1))
    {
        if (!Savegame_EventFlagGet(EventFlag_M2S04_ShotgunShells1))
        {
            WorldObjects_Add(&g_CommonWorldObjects[5], &g_CommonWorldObjectPoses[3].position, &g_CommonWorldObjectPoses[3].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -2, -1, -2) && PLAYER_IN_MAP_CHUNK(vz, 0, 0, -1, 1))
    {
        if (!Savegame_EventFlagGet(EventFlag_M2S04_HandgunBullets2))
        {
            WorldObjects_Add(&g_CommonWorldObjects[3], &g_CommonWorldObjectPoses[4].position, &g_CommonWorldObjectPoses[4].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -3, -1, -3) && PLAYER_IN_MAP_CHUNK(vz, 1, 2, -1, 2))
    {
        if (!Savegame_EventFlagGet(EventFlag_M2S04_HandgunBullets3))
        {
            WorldObjects_Add(&g_CommonWorldObjects[3], &g_CommonWorldObjectPoses[5].position, &g_CommonWorldObjectPoses[5].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -3, -1, -3) && PLAYER_IN_MAP_CHUNK(vz, 1, 2, -1, 2))
    {
        if (!Savegame_EventFlagGet(EventFlag_M2S04_RifleShells))
        {
            WorldObjects_Add(&g_CommonWorldObjects[4], &g_CommonWorldObjectPoses[6].position, &g_CommonWorldObjectPoses[6].rotation);
        }
    }
}
