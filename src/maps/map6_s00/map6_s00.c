#include "inline_no_dmpsx.h"

#include <psyq/gtemac.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/chara/dms.h"
#include "bodyprog/events/event_data.h"
#include "bodyprog/world/effects.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player/player.h"
#include "bodyprog/sound/sound_system.h"
#include "main/rng.h"
#include "maps/map6/map6_s00.h"
#include "maps/common/particle.h"
#include "maps/common/characters/air_screamer.h"
#include "maps/common/characters/groaner.h"
#include "maps/common/characters/player.h"
#include "maps/common/characters/romper.h"

#include "../src/maps/common/particles/particle.c"

#include "../src/maps/common/characters/player.c"

#include "../src/maps/common/characters/air_screamer.c" // 0x800D18FC

#include "../src/maps/common/characters/groaner.c" // 0x800E2934


// TODO: Move this line into separate `Chara_Romper` split.
#include "../src/maps/common/characters/romper.c" // 0x800E6F0C

#include "../src/maps/common/map_util.c" // 0x800EAB00

#include "maps/common/shared/Map_RoomBgmInit_6_s00.h" // 0x800EAD0C

void GameBoot_LoadScreen_StageString(void) {}

#include "maps/common/shared/Event_CutsceneTimerAdvance.h" // 0x800EAFF8

const char* MAP_MESSAGES[] = {
    #include "maps/common/shared/map_msg_common.inc"
#if VERSION_REGION_IS(NTSC)
    /* 15 */ "~J0(1.5)\tWhat's_this! ~E ",
    /* 16 */ "~J0(0.1)\t~E ",
    /* 17 */ "~J0(0.1)\t~E ",
    /* 18 */ "~J0(0.1)\t~E ",
    /* 19 */ "~J1(4.0)\tNot_again...!? ~N\n\t\t\tNo,_this_time_it_feels_different. ",
    /* 20 */ "~J1(4.5)\tRather_than_shifting ~N\n\t\t\tfrom_reality_to_a_nightmare, ",
    /* 21 */ "~J1(4.5)\tthis_is_more_like_reality ~N\n\t\t\tbecoming_a_nightmare. ",
    /* 22 */ "~J1(6.0)\tI_don't_like_this_feeling. ~N\n\t\t\tLike_something_bad ~N\n\t\t\twill_happen... ",
    /* 23 */ "~J1(4.0)\tNo_doubt,_something_terrible ~N\n\t\t\tis_going_on... ~E ",
    /* 24 */ "\tThe_cover_has_been_removed... ",
    /* 25 */ "\tCould_Cybil_have_gotten ~N\n\tto_the_Amusement_Park_from_here? ~E ",
    /* 26 */ "~C3\tLAKE_SIDE_AMUSEMENT_PARK ~E "
#elif VERSION_REGION_IS(NTSCJ)
    /* 15 */ "~J0(1.5)\t何だ？ ~E ",
    /* 16 */ "~J0(0.1)\t~E ",
    /* 17 */ "~J0(0.1)\t~E ",
    /* 18 */ "~J0(0.1)\t~E ",
    /* 19 */ "~J0(4.0)\tまたか……？ ~N\n\t\t\tいや　今までとは違うような気がする ",
    /* 20 */ "~J0(4.5)\t現実から悪夢に移ったというよりは…… ",
    /* 21 */ "~J0(4.5)\t現実が悪夢に変わったような ",
    /* 22 */ "~J0(4.5)\t嫌な気分だ ~N\n\t\t\t悪い予感がする…… ",
    /* 23 */ "~J0(6.0)\tまちがいない　とんでもなく ~N\n\t\t\tまずい事態になってきている ~E ",
    /* 24 */ "\t下水道の入口が開いている…… ",
    /* 25 */ "\tシビルはここから遊園地に向かったのか？ ~E ",
    /* 26 */ "~C3\tレイクサイド遊園地 ~E "
#endif
};

void MapEvent_CommonItemTake(void) // 0x800EB090
{
    u32 pickupType;
    s32 eventFlagIdx;

    pickupType   = CommonPickupItemId_FirstAidKit;
    eventFlagIdx = 0;

    switch (g_MapEventData->mapPointIdx)
    {
        case 22:
            pickupType   = CommonPickupItemId_HealthDrink;
            eventFlagIdx = EventFlag_M6S00_HealthDrink;
            break;

        case 23:
            pickupType   = CommonPickupItemId_ShotgunShells;
            eventFlagIdx = EventFlag_M6S00_ShotgunShells;
            break;

        case 24:
            pickupType   = CommonPickupItemId_FirstAidKit;
            eventFlagIdx = EventFlag_M6S00_FirstAidKit;
            break;
    }

    Event_CommonItemTake(pickupType, eventFlagIdx);
}

void func_800EB11C(void) // 0x800EB11C
{
    typedef enum _EventState
    {
        EventState_Skip = 19
    } e_EventState;

    typedef struct
    {
        SPRT*     sprt_0;
        DR_TPAGE* tpage_4;
        DR_STP*   stp_8;
        u8        unk_C[4];
        u8        unk_10[4];
        s32       activeBufferIdx_14;
    } g_GteScratchData_func_800DD2D4;
    g_GteScratchData_func_800DD2D4* scratchData;

    s32 i;
    s32 vol;

    // Skip.
    if (Pad_ButtonClickedBind(skip) &&
        g_SysWork.sysStateSteps[0] >= 2 && g_SysWork.sysStateSteps[0] < EventState_Skip)
    {
        SysWork_StateStepSet(0, EventState_Skip);
    }

    if (g_SysWork.sysStateSteps[0] >= 5 && g_SysWork.sysStateSteps[0] < 13)
    {
        scratchData                     = PSX_SCRATCH_ADDR(0);
        scratchData->activeBufferIdx_14 = g_ActiveBufferIdx;
        scratchData->sprt_0             = (SPRT*)GsOUT_PACKET_P;
        for (i = 0; i < 2; i++)
        {
            setCodeWord(scratchData->sprt_0, PRIM_RECT | RECT_BLEND | RECT_TEXTURE, COLOR_RGBC(128, 128, 128, 0));
            setXY0Fast(scratchData->sprt_0, ((i << 8) - 160), -112);
            scratchData->sprt_0->u0 = 0;
            scratchData->sprt_0->v0 = (scratchData->activeBufferIdx_14 == 0) ? 32 : 0;
            setWH(scratchData->sprt_0, (i == 0) ? 256 : 64, FRAMEBUFFER_HEIGHT_PROGRESSIVE);
            addPrimFast(&g_OrderingTable2[g_ActiveBufferIdx].org[15], scratchData->sprt_0, 4);

            scratchData->sprt_0++;
            scratchData->tpage_4 = (DR_TPAGE*)scratchData->sprt_0;
            setDrawTPage(scratchData->tpage_4, 0, 0, getTPage(2, 0, ((scratchData->activeBufferIdx_14 << 4) + (i << 2)) << 6, (((scratchData->activeBufferIdx_14 << 4) >> 4) & 1) << 8));

            AddPrim(&g_OrderingTable2[g_ActiveBufferIdx].org[15], scratchData->tpage_4);
            scratchData->tpage_4++;
            scratchData->sprt_0 = (SPRT*)scratchData->tpage_4;
        }

        scratchData->stp_8 = (DR_STP*)scratchData->sprt_0;
        SetDrawStp(scratchData->stp_8, 1);
        addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[ORDERING_TABLE_SIZE - 1], scratchData->stp_8);
        scratchData->stp_8++;
        SetDrawStp(scratchData->stp_8, 0);
        addPrim(&g_OrderingTable2[g_ActiveBufferIdx].org[0], scratchData->stp_8);
        scratchData->stp_8++;
        GsOUT_PACKET_P = (PACKET*)scratchData->stp_8;
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Screen_FadeTimestepReset();
            CutsceneBorder_ForceShow();
            g_SysWork.sysFlags |= SysFlag_CutsceneActive;

            Fs_QueueStartRead(FILE_ANIM_RSU_DMS, FS_BUFFER_15);
            Fs_QueueWaitForEmpty();
            Dms_HeaderFixOffsets(FS_BUFFER_15);

            func_8003D03C();
            Player_EmptyWeaponHandSet();

            D_800F0684 = 0;
            D_800F0044 = 0;
            g_Cutscene_Timer = Q12(0.0f);

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 53, false);
            Game_TurnFlashlightOn();

            Particle_EnvironmentSet(2, 127);
            Particle_SystemUpdate(0, g_SavegamePtr->mapIdx, 0);
            sharedFunc_800D0B18_0_s00(6);

            SD_Call(Sfx_Unk1522);
            SD_Call(Sfx_Unk1599);
            D_800F0686 = 0;
            SysWork_StateStepIncrement(0);

        case 1:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.0f), false);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(12.5f), Q12(0.0f), Q12(22.0f), true, false);
            break;

        case 2:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(12.5f), Q12(0.0f), Q12(22.0f), true, true);
            break;

        case 3:
            g_Cutscene_Timer = Q12(23.0f);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 52, false);
            SysWork_StateStepIncrement(0);

        case 4:
            Event_DisplayMapMsgWithAudio(15, &D_800F0684, &D_800F0038);
            break;

        case 5:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 126, false);
            Savegame_EventFlagSet(EventFlag_413);
            SysWork_StateStepIncrement(0);

        case 6:
            Event_DisplayMapMsgWithAudio(16, &D_800F0684, &D_800F0038);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(18.0f), Q12(23.0f), Q12(43.0f), true, false);
            break;

        case 7:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(18.0f), Q12(23.0f), Q12(43.0f), true, true);
            break;

        case 8:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 133, false);
            SysWork_StateStepIncrement(0);

        case 9:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(4.2f), Q12(44.0f), Q12(47.0f), true, true);
            break;

        case 10:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 114, false);
            Savegame_EventFlagSet(EventFlag_402);
            Particle_EnvironmentSet(6, 127);
            Particle_SystemUpdate(0, g_SavegamePtr->mapIdx, 0);
            Savegame_EventFlagSet(EventFlag_414);
            D_800F0044 = Q12(3.4f);
            SysWork_StateStepIncrement(0);

        case 11:
            Event_DisplayMapMsgWithAudio(17, &D_800F0684, &D_800F0038);

        case 12:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(4.0f), Q12(48.0f), Q12(57.0f), true, true);
            D_800F0044 += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 1.0f);
            break;

        case 13:
            D_800F0044 = Q12(5.2f);
            SysWork_StateStepIncrement(0);

        case 14:
            Event_DisplayMapMsgWithAudio(18, &D_800F0684, &D_800F0038);

        case 15:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(4.0f), Q12(58.0f), Q12(67.0f), true, true);
            D_800F0044 += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.7f);
            break;

        case 16:
            Event_WaitTimer(Q12(2.2f), false);
            D_800F0044 += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.7f);
            break;

        case 17:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 127, false);
            Savegame_EventFlagSet(EventFlag_415);
            SysWork_StateStepIncrement(0);

        case 18:
            Event_DisplayMapMsg(false, 19, 0, 0, 0, false);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(2.5f), Q12(68.0f), Q12(143.0f), true, false);

            D_800F0044 += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.4f);
            if (D_800F0044 > Q12(16.0f))
            {
                D_800F0044 = Q12(16.0f);
            }
            break;

        case EventState_Skip:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            vcReturnPreAutoCamWork(true);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);

            g_Cutscene_Timer = NO_VALUE;

            Savegame_EventFlagSet(EventFlag_402);
            Savegame_EventFlagClear(EventFlag_414);

            SD_Call(19);
            func_8003D01C();
            Player_WeaponAttackRestore();
            Sd_SfxStop(Sfx_Unk1522);
            Sd_SfxStop(Sfx_Unk1599);
            Particle_EnvironmentSet(6, 127);
            Particle_SystemUpdate(0, g_SavegamePtr->mapIdx, 0);
            break;
    }

    func_800EC4B4(D_800F0044);

    if (g_SysWork.sysStateSteps[0] >= 1 && g_SysWork.sysStateSteps[0] < 17)
    {
        if (g_SysWork.sysStateSteps[0] >= 2)
        {
            D_800F0686 += g_DeltaTime >> 3;
        }

        if (D_800F0686 > Q12(1.0f))
        {
            D_800F0686 = Q12(1.0f);
        }

        vol = (D_800F0686 + Math_Sin(D_800F0686 >> 2)) >> 5;
        if (vol > 255)
        {
            vol = 255;
        }
        Sd_SfxAttributesUpdate(Sfx_Unk1522, 0, ~vol, 0);

        vol = (Q12(1.0f) - Math_Cos((D_800F0686) >> 2)) >> 4;
        if (vol > 255)
        {
            vol = 255;
        }
        Sd_SfxAttributesUpdate(Sfx_Unk1599, 0, ~vol, 0);
    }
    else if (g_SysWork.sysStateSteps[0] >= 17 && g_SysWork.sysStateSteps[0] < 20)
    {
        D_800F0686 -= g_DeltaTime >> 3;
        if (D_800F0686 < 0)
        {
            D_800F0686 = 0;
        }

        vol = D_800F0686 >> 4;
        if (vol > 255)
        {
            vol = 255;
        }
        Sd_SfxAttributesUpdate(Sfx_Unk1522, 0, ~vol, 0);

        vol = D_800F0686 >> 4;
        if (vol > 255)
        {
            vol = 255;
        }
        Sd_SfxAttributesUpdate(Sfx_Unk1599, 0, ~vol, 0);
    }

    if (g_SysWork.sysStateSteps[0] >= 5 && g_SysWork.sysStateSteps[0] < 10)
    {
        func_800894DC();
    }

    if (g_Cutscene_Timer >= Q12(0.0f))
    {
        Dms_CharacterTransformGet(&g_SysWork.playerWork.player.position, &g_SysWork.playerWork.player.rotation, "HERO", g_Cutscene_Timer, FS_BUFFER_15);
        vcChangeProjectionValue(Dms_CameraTargetsGet(&g_Cutscene_CameraPositionTarget, &g_Cutscene_CameraLookAtTarget, NULL, g_Cutscene_Timer, FS_BUFFER_15));
        vcUserCamTarget(&g_Cutscene_CameraPositionTarget, NULL, true);
        vcUserWatchTarget(&g_Cutscene_CameraLookAtTarget, NULL, true);
    }
}

void Map_WorldObjectsInit(void) // 0x800EBCE8
{
    if (!Savegame_EventFlagGet(EventFlag_402))
    {
        g_LoadingScreenImg.v = 0;
        Fs_QueueStartReadTim(FILE_TIM_GROUND_TIM, IMAGE_BUFFER_4, &g_LoadingScreenImg);
    }

    WorldObject_PlacementInit(&g_WorldObject_Window, "WINDOW_H", -155.0f, 3.0f, 20.0f);
    WorldObject_PlacementInit(&g_WorldObject_DrOpen1, "DROPEN1_", -190.5f, 0.0f, 132.5f);
    WorldObject_PlacementInit(&g_WorldObject_DrOpen2, "DROPEN2_", -190.5f, -0.05f, 132.214f);
    WorldObject_PlacementInit(&g_WorldObject_DrClose, "DRCLOSE_", -189.5f, 0.0f, 132.5f);

    func_8008D448();
    Game_FlashlightAttribsSet();

    g_SysWork.lightIntensity = Q12(1.0f);

    switch (g_SavegamePtr->gameDifficulty)
    {
        case GameDifficulty_Normal:
            g_SysWork.npcFlagId = 4;
            break;

        case GameDifficulty_Easy:
            g_SysWork.npcFlagId = 3;
            break;

        default:
            g_SysWork.npcFlagId = 5;
            break;
    }

    if (Savegame_EventFlagGet(EventFlag_428))
    {
        Savegame_EventFlagClear(EventFlag_428);
        Game_TurnFlashlightOn();
    }

    WorldObject_ModelNameSet(&g_CommonWorldObjects[0], g_CommonWorldObjectNames[2]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[1], g_CommonWorldObjectNames[3]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[2], g_CommonWorldObjectNames[4]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[3], g_CommonWorldObjectNames[5]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[4], g_CommonWorldObjectNames[6]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[5], g_CommonWorldObjectNames[7]);
}

void Map_WorldObjectsUpdate(void) // 0x800EBEE0
{
    s32 triggerFlags;

    triggerFlags = CollisionTriggerFlag_None;

    if (!Savegame_EventFlagGet(EventFlag_401) && Savegame_EventFlagGet(EventFlag_403))
    {
        Savegame_EventFlagSet(EventFlag_401);
        func_8003A16C();
    }

    if (PLAYER_IN_MAP_CHUNK(-1, 6))
    {
        if (!Savegame_EventFlagGet(EventFlag_402))
        {
            triggerFlags |= CollisionTriggerFlag_Objects;
        }

        Text_Debug_PositionSet(30, 30);
    }

    if (PLAYER_IN_MAP_CHUNK_Z(0))
    {
        if (PLAYER_IN_MAP_CHUNK_X(-4) || PLAYER_IN_MAP_CHUNK_X(-5))
        {
            WorldObjects_Add(&g_WorldObject_Window.object, &g_WorldObject_Window.position, &SVECTOR3_Zero);
        }
    }

    if (PLAYER_IN_MAP_CHUNK_X(-5))
    {
        if (PLAYER_IN_MAP_CHUNK_Z(3) || PLAYER_IN_MAP_CHUNK_Z(2))
        {
            if (Savegame_EventFlagGet(EventFlag_424))
            {
                WorldObjects_Add(&g_WorldObject_DrOpen1.object, &g_WorldObject_DrOpen1.position, &SVECTOR3_Zero);
                WorldObjects_Add(&g_WorldObject_DrOpen2.object, &g_WorldObject_DrOpen2.position, &SVECTOR3_Zero);

                triggerFlags |= CollisionTriggerFlag_Objects;
            }
            else
            {
                WorldObjects_Add(&g_WorldObject_DrClose.object, &g_WorldObject_DrClose.position, &SVECTOR3_Zero);
            }
        }
    }

    Collision_TriggerFlagBitsClear(CollisionTriggerFlag_All);
    Collision_TriggerFlagBitsSet(triggerFlags);

    if (PLAYER_IN_MAP_CHUNK(-2, 3))
    {
        if (!Savegame_EventFlagGet(EventFlag_M6S00_HealthDrink))
        {
            WorldObjects_Add(&g_CommonWorldObjects[1], &g_CommonWorldObjectPoses[0].position, &g_CommonWorldObjectPoses[0].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(-2, 3))
    {
        if (!Savegame_EventFlagGet(EventFlag_M6S00_ShotgunShells))
        {
            WorldObjects_Add(&g_CommonWorldObjects[5], &g_CommonWorldObjectPoses[1].position, &g_CommonWorldObjectPoses[1].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(-5, -2))
    {
        if (!Savegame_EventFlagGet(EventFlag_M6S00_FirstAidKit))
        {
            WorldObjects_Add(g_CommonWorldObjects, &g_CommonWorldObjectPoses[2].position, &g_CommonWorldObjectPoses[2].rotation);
        }
    }
}

static inline s32 inline_sub(s32 arg0, s32 arg1)
{
    return arg0 - arg1;
}

void func_800EC4B4(s32 arg0) // 0x800EC4B4
{
    s32                posX;
    s32                posZ;
    s32                posY;
    s32                temp_v1_14;
    s32                var_a2;
    s32                var_a3;
    s32                var_a3_2;
    s32                j;
    s32                i;
    s32                var_t0_2;
    s32                var_t1_2;
    s32                var_t2_2;
    s32                var_t3;
    s32                temp;
    s_func_800EC4B4*   ptr;
    s_func_800EC4B4_2* ptr2;

    ptr2 = FS_BUFFER_1;
    ptr  = PSX_SCRATCH;

    posX = FP_FROM(g_SysWork.playerWork.player.position.vx, Q12_SHIFT);
    posY = FP_FROM(g_SysWork.playerWork.player.position.vy, Q12_SHIFT);
    posZ = FP_FROM(g_SysWork.playerWork.player.position.vz, Q12_SHIFT);

    ptr->field_5C.vx = posX << 8;
    ptr->field_5C.vy = posY << 8;
    ptr->field_5C.vz = posZ << 8;

    Vw_WorldScreenMatrixAtPositionGet(&ptr->field_4, FP_TO(posX, Q12_SHIFT), FP_TO(posY, Q12_SHIFT), FP_TO(posZ, Q12_SHIFT));

    gte_SetRotMatrix(&ptr->field_4);
    gte_SetTransMatrix(&ptr->field_4);
    gte_ReadGeomScreen(&ptr->field_84);

    ptr->field_24      = GsIDMATRIX;
    ptr->field_24.t[0] = Q12(-0.75f); // TODO: Or Q8, since it's a `MATRIX`?
    ptr->field_24.t[2] = Q12(15.0f);
    ptr->field_24.t[1] = Q12(0.0f);
    ptr->field_68.vx   = Q12(0.0f);
    ptr->field_68.vy   = Q12(-1.0f);
    ptr->field_68.vz   = Q12(0.0f);

    for (i = 0; i < 287; i += 3)
    {
        for (j = 0; j < 3; j++)
        {
            Math_SetSVectorFastSum(&ptr->field_44[j], inline_sub((((i + j) % 17) << 8) - 0x1400, (u16)ptr->field_5C.vx),
                                   -ptr->field_5C.vy, inline_sub(-0x800 - (u16)ptr->field_5C.vz, ((i + j) / 17) << 8));
        }

        gte_ldv3c(&ptr->field_44[0]);
        gte_rtpt();
        gte_stsxy3c(&ptr2->field_0[i / 17][i % 17]);
        gte_stsz3c(&ptr2->field_484[i / 17][i % 17]);
    }

    for (i = 288; i < 289; i++)
    {
        Math_SetSVectorFastSum(&ptr->field_44[0], inline_sub(((i % 17) << 8) - 0x1400, (u16)ptr->field_5C.vx),
                               -ptr->field_5C.vy, inline_sub(-0x800 - (u16)ptr->field_5C.vz, (i / 17) << 8));

        gte_ldv0(&ptr->field_44[0]);
        gte_rtps();
        gte_stsxy(&ptr2->field_0[i % 17][i / 17]);
        gte_stsz(&ptr2->field_484[i % 17][i / 17]);
    }

    for (i = 0; i < 17; i++)
    {
        for (j = 0; j < 17; j++)
        {
            *(s32*)&ptr->field_44[0] = ((j << 8) - 0x800) & 0xFFFF;
            ptr->field_44[0].vz      = 0x800 - (i << 8);

            if (arg0 < D_800F0084[j][i].vx * 4)
            {
                ptr->field_90[i][j] = 0;
                ptr->field_1B4      = 0x80;
            }
            else if (arg0 >= D_800F0084[j][i].vy * 4)
            {
                ptr->field_90[i][j] = 3;
                ptr->field_1B4      = 0;
            }
            else
            {
                ptr->field_1B8 = arg0 - D_800F0084[j][i].vx * 4;
                ptr->field_1BC = (D_800F0084[j][i].vy - D_800F0084[j][i].vx) * 4;
                ptr->field_1C0 = ptr->field_1BC >> 2;

                if (ptr->field_1B8 < ptr->field_1C0)
                {
                    ptr->field_90[i][j] = 0;
                    ptr->field_1B4      = ((ptr->field_1C0 - ptr->field_1B8) << 7) / ptr->field_1C0;
                }
                else
                {
                    ptr->field_1C4 = ptr->field_1BC >> 1;

                    if (ptr->field_1B8 < ptr->field_1C4)
                    {
                        ptr->field_90[i][j] = 1;
                        ptr->field_1B4      = ((ptr->field_1C4 - ptr->field_1B8) << 7) / (ptr->field_1C4 - ptr->field_1C0);
                    }
                    else
                    {
                        ptr->field_1C0 = (ptr->field_1BC * 3) >> 2;

                        if (ptr->field_1B8 < ptr->field_1C0)
                        {
                            ptr->field_90[i][j] = 2;
                            ptr->field_1B4      = ((ptr->field_1C0 - ptr->field_1B8) << 7) / (ptr->field_1C0 - ptr->field_1C4);
                        }
                        else
                        {
                            ptr->field_90[i][j] = 3;
                            ptr->field_1BC     -= (ptr->field_1BC - ptr->field_1C0) >> 1;

                            if (ptr->field_1BC < ptr->field_1B8)
                            {
                                ptr->field_1B4 = 0;
                            }
                            else
                            {
                                ptr->field_1B4 = ((ptr->field_1BC - ptr->field_1B8) << 7) / (ptr->field_1BC - ptr->field_1C0);
                            }
                        }
                    }
                }
            }

            func_80055ECC(&ptr->field_70, &ptr->field_44, &ptr->field_68, &ptr->field_24);

            ptr2->field_908[i][j][0].r = MIN((ptr->field_70.r * ptr->field_1B4) >> 7, 0xFF);
            ptr2->field_908[i][j][0].g = MIN((ptr->field_70.g * ptr->field_1B4) >> 7, 0xFF);
            ptr2->field_908[i][j][0].b = MIN((ptr->field_70.b * ptr->field_1B4) >> 7, 0xFF);

            ptr2->field_908[i][j][1].r = MIN((ptr->field_70.r * (0x80 - ptr->field_1B4)) >> 7, 0xFF);
            ptr2->field_908[i][j][1].g = MIN((ptr->field_70.g * (0x80 - ptr->field_1B4)) >> 7, 0xFF);
            ptr2->field_908[i][j][1].b = MIN((ptr->field_70.b * (0x80 - ptr->field_1B4)) >> 7, 0xFF);
        }
    }

    ptr->field_0 = (POLY_GT4*)GsOUT_PACKET_P;

    for (i = 0; i < 16; i++)
    {
        for (j = 0; j < 16; j++)
        {
            ptr->field_88 = (ptr2->field_484[i][j] + ptr2->field_484[i + 1][j] +
                             ptr2->field_484[i][j + 1] + ptr2->field_484[i + 1][j + 1]) >>
                            2;

            if (ptr->field_88 + 0x80 < ptr->field_84)
            {
                continue;
            }

            if ((ptr->field_88 + 0x80 >> 3) >= 0x800)
            {
                continue;
            }

            temp = 0x2F;

            for (var_t2_2 = 0; var_t2_2 < 5; var_t2_2++)
            {
                for (var_a3 = 0; var_a3 < 2; var_a3++)
                {
                    for (var_a2 = 0; var_a2 < 2; var_a2++)
                    {
                        if (ptr->field_90[i + var_a3][j + var_a2] == var_t2_2)
                        {
                            ptr->field_74[(var_a3 * 2) + var_a2].r = ptr2->field_908[i + var_a3][j + var_a2][0].r;
                            ptr->field_74[(var_a3 * 2) + var_a2].g = ptr2->field_908[i + var_a3][j + var_a2][0].g;
                            ptr->field_74[(var_a3 * 2) + var_a2].b = ptr2->field_908[i + var_a3][j + var_a2][0].b;
                        }
                        else if ((ptr->field_90[i + var_a3][j + var_a2] + 1) == var_t2_2)
                        {
                            ptr->field_74[(var_a3 * 2) + var_a2].r = ptr2->field_908[i + var_a3][j + var_a2][1].r;
                            ptr->field_74[(var_a3 * 2) + var_a2].g = ptr2->field_908[i + var_a3][j + var_a2][1].g;
                            ptr->field_74[(var_a3 * 2) + var_a2].b = ptr2->field_908[i + var_a3][j + var_a2][1].b;
                        }
                        else
                        {
                            ptr->field_74[(var_a3 * 2) + var_a2].r = 0;
                            ptr->field_74[(var_a3 * 2) + var_a2].g = 0;
                            ptr->field_74[(var_a3 * 2) + var_a2].b = 0;
                        }
                    }
                }

                if (!(ptr->field_74[0].r == 0 && ptr->field_74[0].g == 0 && ptr->field_74[0].b == 0) ||
                    !(ptr->field_74[1].r == 0 && ptr->field_74[1].g == 0 && ptr->field_74[1].b == 0) ||
                    !(ptr->field_74[2].r == 0 && ptr->field_74[2].g == 0 && ptr->field_74[2].b == 0) ||
                    !(ptr->field_74[3].r == 0 && ptr->field_74[3].g == 0 && ptr->field_74[3].b == 0))
                {
                    ptr->field_1B8 = !(var_t2_2 < 2) + !(var_t2_2 < 3);
                    temp_v1_14     = ptr->field_1B8 + 5;

                    ptr->field_8E = temp_v1_14 | 0x20;
                    ptr->field_8C = getClut(g_LoadingScreenImg.clutX, g_LoadingScreenImg.clutY + var_t2_2);

                    if (var_t2_2 == 0)
                    {
                        var_t0_2 = 0;
                        var_a3_2 = 0;
                        var_t3   = 0x2F;
                        var_t1_2 = temp;
                    }
                    else if (var_t2_2 == 4)
                    {
                        var_t0_2       = 0;
                        ptr->field_1B8 = (((u32)i & 7) > 0) + ((i & 7) == 7);
                        ptr->field_1BC = (((u32)j & 7) > 0) + ((j & 7) == 7);

                        if (ptr->field_1B8 == 1)
                        {
                            if (ptr->field_1BC == 1)
                            {
                                var_a3_2 = 0;
                            }
                            else
                            {
                                var_a3_2 = 0x90;
                            }
                        }
                        else
                        {
                            if (ptr->field_1BC == 1)
                            {
                                var_a3_2 = 0x30;
                            }
                            else
                            {
                                var_a3_2 = 0x60;
                            }
                        }

                        if (ptr->field_1B8 >= 2)
                        {
                            var_t3    = -0x2F;
                            var_t0_2 += 0x2F;
                        }
                        else
                        {
                            var_t3 = 0x2F;
                        }

                        if (ptr->field_1BC <= 0)
                        {
                            var_t1_2  = -0x2F;
                            var_a3_2 += 0x2F;
                        }
                        else
                        {
                            var_t1_2 = 0x2F;
                        }
                    }
                    else
                    {
                        if (j & 4)
                        {
                            var_a3_2 = 0xBF - ((j & 3) * 0x30);
                            var_t1_2 = -0x2F;
                        }
                        else
                        {
                            var_a3_2 = ((j & 3) * 3) * 0x10;
                            var_t1_2 = 0x2F;
                        }

                        if (i & 4)
                        {
                            var_t0_2 = ((i & 3) * 0x30) + 0x40;
                            var_t3   = 0x2F;
                        }
                        else
                        {
                            var_t0_2 = 0xFF - ((i & 3) * 0x30);
                            var_t3   = -0x2F;
                        }
                    }

                    setPolyGT4(ptr->field_0);
                    setSemiTrans(ptr->field_0, 1);

                    if (var_t2_2 == 4 && ptr->field_1B8 != ptr->field_1BC && ptr->field_1B8 != 1 && ptr->field_1BC != 1)
                    {
                        setXY0Fast(ptr->field_0, ptr2->field_0[i][j + 1].vx, ptr2->field_0[i][j + 1].vy);
                        setXY1Fast(ptr->field_0, ptr2->field_0[i][j].vx, ptr2->field_0[i][j].vy);
                        setXY2Fast(ptr->field_0, ptr2->field_0[i + 1][j + 1].vx, ptr2->field_0[i + 1][j + 1].vy);
                        setXY3Fast(ptr->field_0, ptr2->field_0[i + 1][j].vx, ptr2->field_0[i + 1][j].vy);

                        setRGB0Fast(ptr->field_0, ptr->field_74[1].r, ptr->field_74[1].g, ptr->field_74[1].b);
                        setRGB1Fast(ptr->field_0, ptr->field_74[0].r, ptr->field_74[0].g, ptr->field_74[0].b);
                        setRGB2Fast(ptr->field_0, ptr->field_74[3].r, ptr->field_74[3].g, ptr->field_74[3].b);
                        setRGB3Fast(ptr->field_0, ptr->field_74[2].r, ptr->field_74[2].g, ptr->field_74[2].b);

                        *(s32*)&ptr->field_0->u0 = (var_a3_2 + var_t1_2) + (var_t0_2 << 8) + (ptr->field_8C << 16);
                        *(s32*)&ptr->field_0->u1 = ((var_a3_2 + var_t1_2) - var_t1_2) + (var_t0_2 << 8) + (ptr->field_8E << 16);
                        *(u16*)&ptr->field_0->u2 = (var_a3_2 + var_t1_2) + ((var_t0_2 + var_t3) << 8);
                        *(u16*)&ptr->field_0->u3 = ((var_a3_2 + var_t1_2) - var_t1_2) + ((var_t0_2 + var_t3) << 8);
                    }
                    else
                    {
                        setXY0Fast(ptr->field_0, ptr2->field_0[i][j].vx, ptr2->field_0[i][j].vy);
                        setXY1Fast(ptr->field_0, ptr2->field_0[i][j + 1].vx, ptr2->field_0[i][j + 1].vy);
                        setXY2Fast(ptr->field_0, ptr2->field_0[i + 1][j].vx, ptr2->field_0[i + 1][j].vy);
                        setXY3Fast(ptr->field_0, ptr2->field_0[i + 1][j + 1].vx, ptr2->field_0[i + 1][j + 1].vy);

                        setRGB0Fast(ptr->field_0, ptr->field_74[0].r, ptr->field_74[0].g, ptr->field_74[0].b);
                        setRGB1Fast(ptr->field_0, ptr->field_74[1].r, ptr->field_74[1].g, ptr->field_74[1].b);
                        setRGB2Fast(ptr->field_0, ptr->field_74[2].r, ptr->field_74[2].g, ptr->field_74[2].b);
                        setRGB3Fast(ptr->field_0, ptr->field_74[3].r, ptr->field_74[3].g, ptr->field_74[3].b);

                        *(s32*)&ptr->field_0->u0 = var_a3_2 + (var_t0_2 << 8) + (ptr->field_8C << 16);
                        *(s32*)&ptr->field_0->u1 = (var_a3_2 + var_t1_2) + (var_t0_2 << 8) + (ptr->field_8E << 16);
                        *(u16*)&ptr->field_0->u2 = var_a3_2 + ((var_t0_2 + var_t3) << 8);
                        *(u16*)&ptr->field_0->u3 = (var_a3_2 + var_t1_2) + ((var_t0_2 + var_t3) << 8);
                    }

                    addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[(ptr->field_88 + 0x80) >> 3], ptr->field_0);
                    ptr->field_0++;
                }
            }
        }
    }

    GsOUT_PACKET_P = (PACKET*)ptr->field_0;
}
