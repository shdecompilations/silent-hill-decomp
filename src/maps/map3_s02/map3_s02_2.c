#include "bodyprog/bodyprog.h"
#include "bodyprog/chara/dms.h"
#include "bodyprog/gfx/map_effects.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player/player.h"
#include "main/rng.h"
#include "maps/map3/map3_s02.h"
#include "maps/common/particle.h"
#include "maps/common/characters/creeper.h"
#include "maps/common/characters/player.h"

#include "../src/maps/common/particles/particle.c"

#include "../src/maps/common/characters/player.c"

#include "../src/maps/common/characters/creeper.c" // 0x800CCA14
#include "../src/maps/common/characters/alessa.c"

#include "../src/maps/common/chara_util.c" // 0x800CF404

#include "../src/maps/common/map_util.c" // 0x800CFDD0

#include "maps/common/shared/Map_RoomBgmInit_CheckCond.h" // 0x800CFE78

#include "maps/common/shared/Map_RoomBgmInit_3_s02_CondTrue.h" // 0x800CFEAC

#include "maps/common/shared/Map_RoomBgmInit_3_s02_CondFalse.h" // 0x800CFEEC

void GameBoot_LoadScreen_StageString(void) {}

#include "maps/common/shared/MapEvent_DoorJammed.h" // 0x800D0184

#include "maps/common/shared/MapEvent_DoorLocked.h" // 0x800D0218

void func_800D02AC(void) {}

const char* MAP_MESSAGES[] = {
    #include "maps/common/shared/map_msg_common.h"
#if VERSION_REGION_IS(NTSC)
    /* 15 */ "\tNothing_happens_when_the ~N\n\televator_button_is_pressed. ~E ",
    /* 16 */ "\tIt's_locked! ~E "
#elif VERSION_REGION_IS(NTSCJ)
    /* 15 */ "\tエレベーターのボタンを押しても ~N\n\t何の反応もない…… ~E ",
    /* 16 */ "\t鍵がかかっている！ ~E "
#endif
};

const VECTOR3 D_800CAAF8 = { Q12(100.0f), Q12(-1.2f), Q12(-127.2f) };

void func_800D02B4(void) // 0x800D02B4
{
    typedef enum _EventState
    {
        EventState_0   = 0,
        EventState_1   = 1,
        EventState_2   = 2,
        EventState_3   = 3,
        EventState_4   = 4,
        EventState_Finish = 5
    } e_EventState;

    switch (g_SysWork.sysStateSteps[0])
    {
        case EventState_0:
            Player_ControlFreeze();
            Screen_FadeTimestepReset();
            SysWork_StateStepIncrement(0);
            break;

        case EventState_1:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            SysWork_StateStepIncrement(0);

        case EventState_2:
            Event_WaitTimer(Q12(1.0f), false);
            break;

        case EventState_3:
            Sfx_WithFlagsPlay(Sfx_Unk1523, &D_800CAAF8, Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case EventState_4:
            Event_WaitTimer(Q12(0.3f), false);

        default: // `EventState_Finish`
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            return;
    }
}

void func_800D03FC(void) // 0x800D03FC
{
    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();

            D_800D1FBC = Q12(100.0f);

            Screen_FadeTimestepReset();
            SysWork_StateStepIncrement(0);
            break;

        case 1:
            Gfx_MapEnvSet(7, 4);

            D_800D3154 = 0;

            Game_TurnFlashlightOff();
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            SysWork_StateStepIncrement(0);
            break;

        case 2:
            Event_WaitTimer(Q12(0.6f), false);
            break;

        case 3:
            Game_TurnFlashlightOn();
            SysWork_StateStepIncrement(0);

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            Savegame_EventFlagSet(EventFlag_236);
            break;
    }
}

#include "maps/common/shared/Event_CutsceneTimerAdvance.h" // 0x800D0570

void func_800D0608(void) // 0x800D0608
{
    VECTOR3  camPosTarget;
    VECTOR3  camLookAtTarget;
    VECTOR3  lightIntPos;
    SVECTOR3 unused;

    // Weird code that changes g_DeltaTime, some kind of 50Hz/60Hz conversion trick?
    // Maybe cutscene was timed at wrong rate?
    g_DeltaTime = (g_DeltaTime * 4) / 5;
    ClearImage(&D_800D1FD0, 0, 0, 0);

    // GPU packet setup.
    {
        typedef struct
        {
            SPRT*      sprt_0;
            DR_TPAGE*  tpage_4;
            DR_STP*    stp_8;
            DR_AREA*   area_C;
            DR_OFFSET* offset_10;
        } s_ScratchData;

        s32            layer;
        s32            i;
        s_ScratchData* scratch;

        scratch = PSX_SCRATCH_ADDR(0);

        scratch->sprt_0 = (SPRT*)GsOUT_PACKET_P;
        for (layer = 0; layer < 4; layer++)
        {
            for (i = 0; i < 2; i++)
            {
                s32 colorVal;
                u8  tpageAbr;

                // @hack Might have been a switch instead?
                setXY0Fast(scratch->sprt_0,
                           (i * 256) - ((layer < 2) ? ((layer == 1) ? 159 : 161) : 160),
                           ((layer < 2) ? -112 : ((layer == 2) ? -111 : -113)));

                setUV0(scratch->sprt_0, 0, 0);

                switch (layer)
                {
                    case 0:
                        colorVal = 0x20;
                        tpageAbr = 1;
                        break;

                    case 1:
                        colorVal = 0x40;
                        tpageAbr = 0;
                        break;

                    default:
                        colorVal = 0x80;
                        tpageAbr = 0;
                }

                if (layer == 3)
                {
                    setRGBC0(scratch->sprt_0, 0x80, 0x80, 0x80, PRIM_RECT | RECT_TEXTURE);
                }
                else
                {
                    setRGBC0(scratch->sprt_0, colorVal, colorVal, colorVal, PRIM_RECT | RECT_TEXTURE | RECT_BLEND);
                }

                setWH(scratch->sprt_0, (i == 0) ? 256 : 64, FRAMEBUFFER_HEIGHT_PROGRESSIVE);
                addPrimFast(&g_OrderingTable0[g_ActiveBufferIdx].org[2], scratch->sprt_0, 4);

                scratch->sprt_0++;
                scratch->tpage_4 = (DR_TPAGE*)scratch->sprt_0;

                setDrawTPage(scratch->tpage_4, 0, 0, getTPageN(2, tpageAbr, (i * 4) + 21, 1));

                addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[2], scratch->tpage_4);
                scratch->tpage_4++;
                scratch->sprt_0 = (SPRT*)scratch->tpage_4;
            }
        }

        // First `DR_AREA`.
        scratch->area_C = (DR_AREA*)scratch->sprt_0;
        SetDrawArea(scratch->area_C, &D_800D1FC0[g_ActiveBufferIdx == 0 ? 1 : 0]);
        addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[2], scratch->area_C);
        scratch->area_C++;

        // Second `DR_AREA`.
        SetDrawArea(scratch->area_C, &D_800D1FD0);
        addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[ORDERING_TABLE_SIZE - 1], scratch->area_C);
        scratch->area_C++;

        // First `DR_OFFSET`.
        scratch->offset_10 = (DR_OFFSET*)scratch->area_C;
        SetDrawOffset(scratch->offset_10, &D_800D1FD8[g_ActiveBufferIdx == 0 ? 1 : 0]);
        addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[2], scratch->offset_10);
        scratch->offset_10++;

        // Second `DR_OFFSET`.
        SetDrawOffset(scratch->offset_10, &D_800D1FE0);
        addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[ORDERING_TABLE_SIZE - 1], scratch->offset_10);
        scratch->offset_10++;

        // First `DR_STP` (disabled).
        scratch->stp_8 = (DR_STP*)scratch->offset_10;
        SetDrawStp(scratch->stp_8, 0);
        addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[2], scratch->stp_8);
        scratch->stp_8++;

        // Second `DR_STP` (enabled).
        SetDrawStp(scratch->stp_8, 1);
        addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[ORDERING_TABLE_SIZE - 1], scratch->stp_8);
        scratch->stp_8++;

        GsOUT_PACKET_P = (PACKET*)scratch->stp_8;
    }

    if (Pad_ButtonClickedBind(skip) &&
        g_SysWork.sysStateSteps[0] > 0 && g_SysWork.sysStateSteps[0] < 4)
    {
        SysWork_StateStepSet(0, 4);
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            func_8003A16C();
            Screen_FadeTimestepReset();

            CutsceneBorder_ForceShow();
            g_SysWork.sysFlags |= SysFlag_CutsceneActive;

            Fs_QueueStartRead(FILE_ANIM_DWSTWY_DMS, FS_BUFFER_24);
            Fs_QueueWaitForEmpty();
            Dms_HeaderFixOffsets(FS_BUFFER_24);

            Chara_Spawn(Chara_Alessa, 0, Q12(80.0f), Q12(100.0f), Q12_ANGLE(0.0f), 3);

            g_SysWork.lightBoneCoord     = NULL;
            g_SysWork.lensFlareBoneCoord = NULL;
            g_SysWork.lightIntensity     = Q12(0.7f);

            Model_AnimFlagsClear(&g_SysWork.playerWork.player.model, 2);

            func_8008D438();
            Gfx_MapEnvSet(16, 16);
            D_800D1FEC = 0;

            SysWork_StateStepIncrement(0);
            break;

        case 1:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 2, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(1.0f), false);
            SysWork_StateStepIncrement(0);

        case 2:
            Event_CutsceneTimerAdvance(&D_800D1FEC, Q12(12.0f), Q12(0.0f), Q12(60.0f), false, true);
            break;

        case 3:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(1.0f), false);
            Event_CutsceneTimerAdvance(&D_800D1FEC, Q12(12.0f), Q12(0.0f), Q12(75.0f), true, false);
            break;

        case 4:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        default:
            Player_ControlUnfreeze(false);

            SysWork_StateSetNext(SysState_Gameplay);

            Model_AnimFlagsSet(&g_SysWork.playerWork.player.model, 2);

            func_8008D448();
            Game_FlashlightAttribsSet();

            g_SysWork.lightIntensity      = Q12(1.0f);
            CutsceneBorder_Reset();

            Savegame_EventFlagSet(EventFlag_237);

            vcReturnPreAutoCamWork(true);
            D_800D1FEC = NO_VALUE;
            Gfx_MapEnvSet(2, 2);
            Gfx_MapEnvUpdate(7, 7, PrimitiveType_S32, &D_800D1FBC, 0, Q12(100.0f));
            break;
    }

    if (D_800D1FEC >= 0)
    {
        Dms_CharacterTransformGet(&g_SysWork.npcs[0].position, &g_SysWork.npcs[0].rotation, "ARISA", D_800D1FEC, FS_BUFFER_24);
        vcChangeProjectionValue(Dms_CameraTargetsGet(&camPosTarget, &camLookAtTarget, NULL, D_800D1FEC, FS_BUFFER_24));
        vcUserCamTarget(&camPosTarget, NULL, true);
        vcUserWatchTarget(&camLookAtTarget, NULL, true);
        Dms_CharacterTransformGet(&g_SysWork.lightPosition, &unused, "LIGHT", D_800D1FEC, FS_BUFFER_24);
        Dms_CharacterTransformGet(&lightIntPos, &unused, "L_INT", D_800D1FEC, FS_BUFFER_24);

        g_SysWork.lightRotation.vx = -ratan2(lightIntPos.vy - g_SysWork.lightPosition.vy, Math_Vector2MagCalcSafeQ6(lightIntPos.vx - g_SysWork.lightPosition.vx, lightIntPos.vz - g_SysWork.lightPosition.vz));
        g_SysWork.lightRotation.vy = ratan2(lightIntPos.vx - g_SysWork.lightPosition.vx, lightIntPos.vz - g_SysWork.lightPosition.vz);
        g_SysWork.lightRotation.vz = Q12_ANGLE(0.0f);
    }
}

void Map_WorldObjectsInit(void) // 0x800D0F8C
{
    D_800D3154 = 1;
}

void Map_WorldObjectsUpdate(void) // 0x800D0F9C
{
    MAP_CHUNK_CHECK_VARIABLE_DECL();

    if (D_800D3154)
    {
        if (PLAYER_IN_MAP_CHUNK(vx, 1, 4, -1, 4) && PLAYER_IN_MAP_CHUNK(vz, 1, -4, -1, -4))
        {
            D_800D1FBC = MAX(D_800D1FBC, Q12(10.0f));
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, 3, -1, 3) && PLAYER_IN_MAP_CHUNK(vz, 1, -4, -1, -4))
    {
        if (D_800D3154)
        {
            D_800D1FBC = MAX(D_800D1FBC, Q12_MULT_PRECISE(func_800D1354(), Q12(24.0f)) + Q12(25.0f));
        }

        if (!Savegame_EventFlagGet(EventFlag_233) && g_SysWork.playerWork.player.position.vz < Q12(-129.0f))
        {
            Savegame_EventFlagSet(EventFlag_233);
            Sfx_WithFlagsPlay(Sfx_Unk1523, &D_800CAAF8, Q8(0.5f), SfxFlag_None);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, 2, -1, 2) && PLAYER_IN_MAP_CHUNK(vz, 1, -4, -1, -4))
    {

        if (!Savegame_EventFlagGet(EventFlag_234) && g_SysWork.playerWork.player.position.vx < Q12(67.0f))
        {
            Savegame_EventFlagSet(EventFlag_234);
            Sfx_WithFlagsPlay(Sfx_Unk1523, &Q12_VECTOR3(69.6f, -1.2f, -138.4f), Q8(0.5f), SfxFlag_None);
        }
    }
}

s16 func_800D1354(void) // 0x800D1354
{
    s32 cos0;
    s32 tmp0;
    s32 x;
    s32 z;
    s32 var1;
    s32 var;

    x = g_SysWork.playerWork.player.position.vx - Q12(95.2f);
    z = g_SysWork.playerWork.player.position.vz + Q12(140.0f);
    cos0 = Math_Cos(Math_AngleNormalizeSigned((ratan2(x, z) - ratan2(Q12(4.8f) + 1, Q12(12.8f) + 1))));

    tmp0 = FP_FROM(cos0 * Math_Vector2MagCalcSafeQ6(g_SysWork.playerWork.player.position.vx - Q12(95.2f),
                                                    g_SysWork.playerWork.player.position.vz + Q12(140.0f)),
                   Q12_SHIFT);
    var1 = Q12_DIV(tmp0, Q6_TO_Q12(SquareRoot0(0xBAC52)));
    var = MIN(var1, Q8(16.0f));

    if (var >= 0)
    {
        if (var1 > Q8(16.0f))
        {
            var1 = Q8(16.0f);
        }

        return Q8(16.0f) - var1;
    }

    return Q8(16.0f);
}
