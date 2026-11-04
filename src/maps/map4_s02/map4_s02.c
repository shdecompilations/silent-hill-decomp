#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "bodyprog/sound/sound_system.h"
#include "main/rng.h"
#include "maps/map4/map4_s02.h"
#include "maps/particle.h"
#include "maps/characters/player.h"
#include "inline_no_dmpsx.h"
#include <psyq/gtemac.h>
#include "bodyprog/player/player.h"
#include "maps/characters/air_screamer.h"
#include "maps/characters/groaner.h"
#include "maps/characters/romper.h"

#include "../src/maps/particle.c"

#include "../src/maps/characters/player.c"

#include "../src/maps/characters/air_screamer.c" // 0x800D0B24

#include "../src/maps/characters/groaner.c" // 0x800E1B5C

#include "../src/maps/characters/romper.c" // 0x800E6134

#include "../src/maps/map_util.c" // 0x800E9D28

#include "maps/shared/Map_RoomBgmInit_4_s02.h" // 0x800E9F34

void GameBoot_LoadScreen_StageString(void) {}

#include "maps/shared/MapEvent_DoorJammed.h" // 0x800EA210

#include "maps/shared/MapEvent_DoorLocked.h" // 0x800EA2A4

void func_800EA338(void) {}

const char* MAP_MESSAGES[] = {
    #include "maps/shared/map_msg_common.h"
#if VERSION_REGION_IS(NTSC)
    /* 15 */ "~C3\tgreen_lion ~E ",
    /* 16 */ "~C3\tAlchemilla_Hospital ~E "
#elif VERSION_REGION_IS(NTSCJ)
    /* 15 */ "~C3\tｇｒｅｅｎ　ｌｉｏｎ ~E ",
    /* 16 */ "~C3\tアルケミラ病院 ~E "
#endif
};

void Map_WorldObjectsInit(void) // 0x800EA340
{
    D_800F13AC = 0;

    WorldObject_PlacementInit(&g_WorldObject_Kidn04, "KIDN04_H", -119.8019f, 5.0f, 107.861f);
    WorldObject_ModelNameSet(&g_WorldObject_Fan0, "FAN0_HID");

    if (g_SavegamePtr->gameDifficulty == GameDifficulty_Easy)
    {
        g_SysWork.npcFlagId = 3;
    }
    else if (g_SavegamePtr->gameDifficulty == GameDifficulty_Normal)
    {
        g_SysWork.npcFlagId = 4;
    }
    else
    {
        g_SysWork.npcFlagId = 5;
    }
}

void Map_WorldObjectsUpdate(void) // 0x800EA3F0
{
    VECTOR3  objPos;
    SVECTOR3 objRot;
    q19_12   dist0;
    q19_12   dist1;
    s32      tmp0;
    s32      i;
    s32      vol;
    s32      balance;
    s32      var_v0;
    s32      tmp1;
    MAP_CHUNK_CHECK_VARIABLE_DECL();

    Collision_TriggerFlagBitsClear(CollisionTriggerFlag_All);

    if ((PLAYER_IN_MAP_CHUNK(vx, 1, -3, -1, -3) && PLAYER_IN_MAP_CHUNK(vz, 1, 3, -1, 3)) ||
        (PLAYER_IN_MAP_CHUNK(vx, 1, -4, -1, -4) && PLAYER_IN_MAP_CHUNK(vz, 1, 3, -1, 3)))
    {
        WorldObjects_Add(&g_WorldObject_Kidn04.object, &g_WorldObject_Kidn04.position, &SVECTOR3_Zero);
        Collision_TriggerFlagBitsSet(2);
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -3, -1, -3) && PLAYER_IN_MAP_CHUNK(vz, 1, -2, -1, -2))
    {
        if (Savegame_EventFlagGet(EventFlag_346))
        {
            Collision_TriggerFlagBitsSet(4);

            if (g_SysWork.playerWork.player.position.vz < Q12(-68.0f))
            {
                Savegame_EventFlagClear(EventFlag_346);
            }
        }
        else
        {
            Collision_TriggerFlagBitsSet(2);
        }
    }

    balance = 0;
    tmp0    = Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(90.0f));

    for (i = 0; i < ARRAY_SIZE(D_800ED8B0); i++)
    {
        switch (i)
        {
            case 0:
                objPos.vx = Q12(53.211f);
                objPos.vy = Q12(-2.375f);
                objPos.vz = Q12(32.5f);
                Math_SetSVectorFast(&objRot, 0, 0, 0);
                break;

            case 4:
                objPos.vx = Q12(56.575f);
                objPos.vy = Q12(-2.875f);
                objPos.vz = Q12(27.5f);
                break;

            case 7:
                objPos.vx = Q12(65.45f);
                objPos.vy = Q12(-2.875f);
                objPos.vz = Q12(27.5f);
                objRot.vy = Q12_ANGLE(180.0f);
                break;

            case 11:
                objPos.vx = Q12(68.8f);
                objPos.vy = Q12(-2.375f);
                objPos.vz = Q12(32.5f);
                break;
        }

        if (i < 7)
        {
            D_800ED8B0[i] -= tmp0;
        }
        else
        {
            D_800ED8B0[i] += tmp0;
        }

        // @hack
        tmp1 = (g_SysWork.playerWork.player.position.vx / ((s32)(40.0f * Q12(1.0f))));

        objRot.vx = D_800ED8B0[i];

        if (PLAYER_IN_MAP_CHUNK(vx, 1, 2, -1, 2) && PLAYER_IN_MAP_CHUNK(vz, 0, 0, -1, 1))
        {
            WorldObjects_Add(&g_WorldObject_Fan0, &objPos, &objRot);
        }

        balance   += Vc_StereoBalanceGet(&objPos);
        objPos.vz -= Q12(5.0f);
    }

    if (PLAYER_NEAR_POS(vx, 61.0f, 4.0f) && PLAYER_NEAR_POS(vz, 22.5f, 5.0f))
    {
        vol = Q8(1.0f);
    }
    else
    {
        dist0 = Math_Vector2MagCalcSafeQ6(g_SysWork.playerWork.player.position.vx - Q12(61.0f), g_SysWork.playerWork.player.position.vz - Q12(27.5f));
        dist1 = Math_Vector2MagCalcSafeQ6(g_SysWork.playerWork.player.position.vx - Q12(61.0f), g_SysWork.playerWork.player.position.vz - Q12(17.5f));
        if (dist1 >= dist0)
        {
            tmp0 = dist0;
        }
        else
        {
            tmp0 = dist1;
        }

        if (tmp0 <= Q12(32.0f))
        {
            tmp1 = tmp0 >> 9;
            vol = Q8(1.0f) - tmp1;
        }
        else
        {
            vol = Q8(0.0f);
        }
    }

    if (vol != Q8(0.0f))
    {
        if (!D_800F13AC)
        {
            SD_Call(Sfx_Unk1542);
            D_800F13AC++;
        }

        Sd_SfxAttributesUpdate(Sfx_Unk1542, balance >> 4, -vol, 0);
    }
    else if (D_800F13AC)
    {
        Sd_SfxStop(Sfx_Unk1542);
        D_800F13AC = 0;
    }
}
