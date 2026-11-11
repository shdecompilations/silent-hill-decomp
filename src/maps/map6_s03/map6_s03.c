#include "inline_no_dmpsx.h"

#include <psyq/gtemac.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/events/event_data.h"
#include "bodyprog/events/npc_main.h"
#include "bodyprog/world/world_effects.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player/player.h"
#include "main/rng.h"
#include "maps/map6/map6_s03.h"
#include "maps/common/particle.h"
#include "maps/common/characters/hanged_scratcher.h"
#include "maps/common/characters/player.h"
#include "maps/common/characters/stalker.h"

#include "../src/maps/common/particles/particle.c"

#include "../src/maps/common/characters/player.c"

#include "../src/maps/common/characters/stalker.c" // 0x800CEFCC
#include "../src/maps/common/characters/hanged_scratcher.c" // 0x800D4264

#include "../src/maps/common/map_util.c" // 0x800D7F14

void func_800D7F98(void) // 0x800D7F98
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
                    D_800DBCDC[5] = 0;
                    D_800DBCDC[6] = 0;
                    D_800DBCDC[7] = 0;
                    break;

                case 17:
                    // VECTOR3 D_800DBD10 = VECTOR3(23.0f, 0.0f, 20.0f); // in .data
                    dist0 = Math_Distance2dGet(&g_SysWork.playerWork.player.position, &D_800DBD10);
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

                    D_800DBCDC[5] = dist0;
                    D_800DBCDC[6] = Q12(0.0f);
                    D_800DBCDC[7] = Q12(0.0f);
                    break;

                case 18:
                case 19:
                case 20:
                    D_800DBCDC[5] = Q8(0.313f);

                    dist1 = Q12(64.0f) - g_SysWork.playerWork.player.position.vx;
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

                    D_800DBCDC[6] = dist1;

                    dist2 = Q12(80.0f) - g_SysWork.playerWork.player.position.vx;
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

                    D_800DBCDC[7] = dist2;
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

void GameBoot_LoadScreen_StageString(void) {}

const char* MAP_MESSAGES[] = {
    #include "maps/common/shared/map_msg_common.inc"
#if VERSION_REGION_IS(NTSC)
    /* 15 */ "~H\tThere_is_a_ ~C5 Sewer_map ~N\n\t(connecting_to_amusement_park) ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 16 */ "\tGotta'_go_find_Cybil! ~N\n\tNo_time_to_go_back. ~E "
#elif VERSION_REGION_IS(NTSCJ)
    /* 15 */ "~H\t~C5 下水道の地図（遊園地方面） ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 16 */ "\tシビルを探しに行かなくては！ ~E "
#endif
};

void MapEvent_CommonItemTake(void) // 0x800D81AC
{
    u32 pickupType;
    s32 eventFlagIdx;

    pickupType   = CommonPickupItemId_FirstAidKit;
    eventFlagIdx = 0;

    switch (g_MapEventData->mapPointIdx)
    {
        case 10:
            pickupType   = CommonPickupItemId_HealthDrink;
            eventFlagIdx = EventFlag_M6S03_HealthDrink0;
            break;

        case 11:
            pickupType   = CommonPickupItemId_HandgunBullets;
            eventFlagIdx = EventFlag_M6S03_HandgunBullets;
            break;

        case 12:
            pickupType   = CommonPickupItemId_HealthDrink;
            eventFlagIdx = EventFlag_M6S03_HealthDrink1;
            break;
    }

    Event_CommonItemTake(pickupType, eventFlagIdx);
}

void func_800D822C(void) // 0x800D822C
{
    if (Pad_ButtonClickedBind(skip))
    {
        if (g_SysWork.sysStateSteps[0] == 3)
        {
            SysWork_StateStepSet(0, 4);
        }
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Screen_FadeTimestepReset();
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            // Warp player.
            g_SysWork.playerWork.player.position.vx = Q12(-20.0486f);
            g_SysWork.playerWork.player.position.vy = Q12(-4.07f);
            g_SysWork.playerWork.player.position.vz = Q12(24.7134f);
            g_SysWork.playerWork.player.rotation.vy = Q12_ANGLE(180.0f);

            // Turn on flashlight.
            Game_TurnFlashlightOn();

            // Warp camera.
            Event_CameraPositionSet(NULL, Q12(-19.66f), Q12(0.17f), Q12(26.3f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(NULL, Q12(-20.49f), Q12(-3.23f), Q12(24.37f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 88, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(1.5f), false);
            SysWork_StateStepIncrement(0);

        case 3:
            g_SysWork.playerWork.player.position.vy += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.4f);
            Event_WaitTimer(Q12(3.8f), false);
            break;

        case 4:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(1.5f), false);
            g_SysWork.playerWork.player.position.vy += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.4f);
            break;

        default:
            Player_ControlUnfreeze(true);
            SysWork_StateSetNext(SysState_Gameplay);
            Savegame_EventFlagSet(EventFlag_431);
            func_8003A16C();
            break;
    }
}

void func_800D84EC(void) // 0x800D84EC
{
    if (Pad_ButtonClickedBind(skip))
    {
        if (g_SysWork.sysStateSteps[0] == 4)
        {
            SysWork_StateStepSet(0, 5);
        }
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 0, Q12(0.0f), false);
            Savegame_EventFlagSet(EventFlag_437);
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            Event_ScreenFadeCmd(ScreenFadeCmd_Wait, true, 0, Q12(0.0f), false);
            break;

        case 3:
            g_SysWork.playerWork.player.position.vx = Q12(89.9092f);
            g_SysWork.playerWork.player.position.vy = Q12(-0.0733f);
            g_SysWork.playerWork.player.position.vz = Q12(18.8875f);
            g_SysWork.playerWork.player.rotation.vy = Q12_ANGLE(90.0f);
            Game_TurnFlashlightOn();
            Event_CameraPositionSet(NULL, Q12(88.92f), Q12(-1.64f), Q12(20.16f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(NULL, Q12(90.98f), Q12(-4.45f), Q12(18.2f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 87, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(1.5f), false);
            SysWork_StateStepIncrement(0);

        case 4:
            g_SysWork.playerWork.player.position.vy -= Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.415f);
            Event_WaitTimer(Q12(5.0f), false);
            break;

        case 5:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(1.5f), false);
            g_SysWork.playerWork.player.position.vy -= Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.41f);
            break;

        default:
            Player_ControlUnfreeze(true);
            SysWork_StateSetNext(SysState_Gameplay);
            Savegame_EventFlagSet(EventFlag_433);
            break;
    }
}

void MapEvent_PaperMapTake(void) // 0x800D87F0
{
    Event_PaperMapTake(15, EventFlag_M6S03_PickupMap, 15);
}

void Map_WorldObjectsInit(void) // 0x800D8818
{
    D_800DFEF5 = 0;
    D_800DFEF4 = 0;
    D_800DFEF2 = 0;
    D_800DFEF0 = 0;

    WorldObject_ModelNameSet(&g_WorldObject0[0], "YUKABFO1");
    WorldObject_ModelNameSet(&g_WorldObject0[1], "YUKAAFT1");
    Math_Vector3Set(&g_WorldObjectPos0, Q12(-20.0f), Q12(0.0f), Q12(34.0f));

    WorldObject_Init(&g_WorldObject1, "SITAI_NE", 75.0f, 0.5f, 19.7f, 0.0f, 180.0f, 0.0f);
    WorldObject_PlacementInit(&g_WorldObject2, "MP_DRNBK", -22.0f, 0.0f, 26.4f);

    if (g_SavegamePtr->gameDifficulty == GameDifficulty_Easy)
    {
        g_SysWork.npcFlagId = 3;
    }

    WorldObject_ModelNameSet(&g_CommonWorldObjects[0], g_CommonWorldObjectNames[2]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[1], g_CommonWorldObjectNames[3]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[2], g_CommonWorldObjectNames[4]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[3], g_CommonWorldObjectNames[5]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[4], g_CommonWorldObjectNames[6]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[5], g_CommonWorldObjectNames[7]);
}

static const SVECTOR3 D_800CB258 = { 0, 0, 0 };
static const VECTOR3  D_800CB260 = { Q12(-41.6f), Q12(0.0f), Q12(20.0f) };

void Map_WorldObjectsUpdate(void) // 0x800D89A0
{
    s32  temp_s0_2;
    s32  var_v0_2;
    s32  temp_s0;
    s32  triggerFlags;
    s32  temp_a0;
    s32  i;
    s16* ptr;

    if (!Savegame_EventFlagGet(EventFlag_429) && Savegame_EventFlagGet(EventFlag_432))
    {
        Savegame_EventFlagSet(EventFlag_429);
        func_8003A16C();
    }

    Collision_TriggerFlagBitsClear(CollisionTriggerFlag_All);

    if (MAP_CHUNK_IDX(g_SysWork.playerWork.player.position.vx) < 0)
    {
        if (!Savegame_EventFlagGet(EventFlag_430) && Savegame_EventFlagGet(EventFlag_435))
        {
            Sfx_WithFlagsPlay(Sfx_Unk1614, &g_WorldObjectPos0, Q8(0.5f), SfxFlag_None);
            Savegame_EventFlagSet(EventFlag_430);

            for (i = 0; i < ARRAY_SIZE(g_SysWork.npcs); i++)
            {
                if (g_SysWork.npcs[i].model.charaId != Chara_None &&
                    g_SysWork.npcs[i].position.vz < Q12(36.5f))
                {
                    g_SysWork.npcs[i].model.charaId = Chara_None;
                    Savegame_EnemyStateUpdate(&g_SysWork.npcs[i]);
                }
            }
        }

        temp_s0 = (g_SavegamePtr->eventFlags[13] >> 19) & 0x1;
        WorldObjects_Add(&g_WorldObject0[temp_s0], &g_WorldObjectPos0, &D_800CB258);
        if (temp_s0 != 0)
        {
            // @bug? `CollisionTriggerFlag_Unused3` is set but never checked. TODO: Find out what the intent was.
            triggerFlags = CollisionTriggerFlag_Block | CollisionTriggerFlag_Unused3;
        }
        else
        {
            triggerFlags = CollisionTriggerFlag_Objects;
        }

        Collision_TriggerFlagBitsSet(triggerFlags);
    }

    if (PLAYER_IN_MAP_CHUNK(1, 0))
    {
        temp_a0 = (Rng_Rand16() & 0x1F) + 0x20;
        if (D_800DFEF4 > 0 && D_800DFEF0 > 0 && temp_a0 < D_800DFEF0)
        {
            D_800DFEF4 = -(Rng_Rand16() & 0x1) - 1;
        }
        else if (D_800DFEF4 < 0 && D_800DFEF0 < 0 && temp_a0 < -D_800DFEF0)
        {
            D_800DFEF4 = (Rng_Rand16() & 0x1) + 1;
        }
        else if (!D_800DFEF4 || !Rng_GenerateInt(0, 0x1F))
        {
            D_800DFEF4 = Rng_GenerateInt(-2, 2);
        }

        ptr       = &D_800DFEF0;
        temp_s0_2 = D_800DFEF0;
        if (Q8_FRACT(Rng_Rand16()) < ((ABS(D_800DFEF4) << 6) + 0x40))
        {
            if (D_800DFEF4 <= 0)
            {
                var_v0_2 = temp_s0_2 - 1;
            }
            else
            {
                var_v0_2 = temp_s0_2 + 1;
            }
        }
        else
        {
            var_v0_2 = temp_s0_2;
        }
        *ptr = var_v0_2;

        g_WorldObject1.rotation.vy = D_800DFEF0 + 0x800;
        temp_a0                       = (Rng_Rand16() & 0x1F) + 0x10;
        if (D_800DFEF5 > 0 && temp_a0 < (D_800DFEF2 + ABS(D_800DFEF0 >> 1)))
        {
            D_800DFEF5 = -(Rng_Rand16() & 0x1) - 1;
        }
        else if (D_800DFEF5 == 0 || !(Rng_Rand16() & 0x3F))
        {
            D_800DFEF5 = (Rng_Rand16() % 5) - 2;
        }

        ptr       = &D_800DFEF2;
        temp_s0_2 = D_800DFEF2;
        temp_a0   = Rng_Rand16() & 0xFF;

        if (temp_a0 < ((ABS(D_800DFEF5) << 6) + 0x60))
        {
            if (D_800DFEF5 <= 0)
            {
                var_v0_2 = temp_s0_2 - 1;
            }
            else
            {
                var_v0_2 = temp_s0_2 + 1;
            }
        }
        else
        {
            var_v0_2 = temp_s0_2;
        }
        *ptr = var_v0_2;

        D_800DFEF2                    = MAX(0, D_800DFEF2);
        g_WorldObject1.position.vz = D_800DFEF2 + Q12(19.7f);
        WorldObjects_Add(&g_WorldObject1.object, &g_WorldObject1.position, &g_WorldObject1.rotation);
    }

    if (PLAYER_IN_MAP_CHUNK(-1, 0))
    {
        if (!Savegame_EventFlagGet(EventFlag_M6S03_PickupMap))
        {
            WorldObjects_Add(&g_WorldObject2.object, &g_WorldObject2.position, &D_800CB258);
        }
    }

    if (!Savegame_EventFlagGet(EventFlag_439) && g_SysWork.playerWork.player.position.vx > Q12(48.0f))
    {
        if (!Vw_AabbVisibleInScreenCheck(40, 44, -2, 0, 18, 22))
        {
            Savegame_EventFlagSet(EventFlag_439);

            for (i = 0; i < ARRAY_SIZE(g_SysWork.npcs); i++)
            {
                if (g_SysWork.npcs[i].position.vx < Q12(32.0f) &&
                    g_SysWork.npcs[i].model.charaId != Chara_None)
                {
                    Chara_ModelCharaIdClear(&g_SysWork.npcs[i], 0, 0);
                }
            }

            Chara_Spawn(Chara_HangedScratcher, 9, Q12(42.2f), Q12(20.9), Q12_ANGLE(90.0f), 6u);
            Chara_SpawnFlagsSet(Chara_HangedScratcher, 9, CharaSpawnFlag_1 | CharaSpawnFlag_2);
            Chara_Spawn(Chara_HangedScratcher, 10, Q12(41.1f), Q12(19.0f), Q12_ANGLE(90.0f), 6u);
            Chara_SpawnFlagsSet(Chara_HangedScratcher, 10, CharaSpawnFlag_1 | CharaSpawnFlag_2);
            Sfx_WithFlagsPlay(Sfx_Unk1585, &D_800CB260, Q8_CLAMPED(1.0f), SfxFlag_NoDistAtten);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(0, 1))
    {
        if (!Savegame_EventFlagGet(EventFlag_M6S03_HealthDrink0))
        {
            WorldObjects_Add(&g_CommonWorldObjects[1], &g_CommonWorldObjectPoses[0].position, &g_CommonWorldObjectPoses[0].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(0, 1))
    {
        if (!Savegame_EventFlagGet(EventFlag_M6S03_HandgunBullets))
        {
            WorldObjects_Add(&g_CommonWorldObjects[3], &g_CommonWorldObjectPoses[1].position, &g_CommonWorldObjectPoses[1].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(0, 1))
    {
        if (!Savegame_EventFlagGet(EventFlag_M6S03_HealthDrink1))
        {
            WorldObjects_Add(&g_CommonWorldObjects[1], &g_CommonWorldObjectPoses[2].position, &g_CommonWorldObjectPoses[2].rotation);
        }
    }
}

void func_800D936C(void) {}
