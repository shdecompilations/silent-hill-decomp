#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "bodyprog/sound/sd_call.h"
#include "main/rng.h"

// TODO: .bss is emitted in order of first declaration, right now our headers
// declare externs in semi-random order though.
// For now we'll declare them before headers in the order they appear in the
// map .bss, later can hopefully improve order in headers instead.
extern e_MapIdx                     g_ParticleMapIdx0;
extern e_MapIdx                     g_ParticleMapIdx1;
extern s32                          __pad_bss_800D5CF0[2];
extern u8                           sharedData_800D5CF8_1_s05[9];
extern s8                           __pad_bss_800D5D01[7];
extern u8                           sharedData_800D5D08_1_s05[9];
extern u8                           D_800D5D11;
extern s8                           __pad_bss_800D5D12[2];
extern s32                          g_Particle_SpeedX;
extern s32                          g_Particle_SpeedZ;
extern s32                          sharedData_800DFB6C_0_s00;
extern s32                          sharedData_800DFB70_0_s00;
extern s32                          __pad_bss_800D5D24;
extern s_MapHdr_field_4C            sharedData_800DFB7C_0_s00[];
extern s_BloodSplat                 g_Effect_BloodSplats[];
extern s8                           __pad_bss_800D8564[4];
extern s_MapOverlayHdr_5C           sharedData_800D8568_1_s05;
extern s32                          __pad_bss_800D8590[2];
extern struct _ParticleVectors      g_ParticleVectors0;
extern s32                          __pad_bss_800D85C4;
extern struct _ParticleVectors      g_ParticleVectors1;
extern s32                          sharedData_800E0CA8_0_s00;
extern s32                          sharedData_800E0CAC_0_s00;
extern u32                          sharedData_800E0CB0_0_s00;
extern u16                          sharedData_800E0CB4_0_s00;
extern u16                          sharedData_800E0CB6_0_s00;
extern u16                          sharedData_800E0CB8_0_s00;
extern s8                           sharedData_800E0CBA_0_s00;
extern s8                           __pad_bss_800D8607;
extern s32                          sharedData_800E39D8_0_s00;
extern q19_12                       g_Player_MoveSpeed;
extern GsCOORDINATE2*               sharedData_800D8610_1_s05;
extern q3_12                        g_SplitHead_MovementOffsetX;
extern q3_12                        g_SplitHead_MovementOffsetZ;
extern VECTOR3                      sharedData_800D8618_1_s05[];
extern q19_12                       sharedData_800D8684_1_s05;
extern q19_12                       sharedData_800D8688_1_s05;
extern s32                          __pad_bss_800D868C[3];
extern struct _WorldObjectPose      g_WorldObject0;
extern struct _WorldObjectPose      g_WorldObject1;
extern s16                          D_800D86F8[2];
extern u8                           D_800D86FC[2];
extern u8                           D_800D86FE;

#include "maps/map1/map1_s05.h"
#include "maps/common/particle.h"
#include "maps/common/characters/player.h"
#include "inline_no_dmpsx.h"

#include <psyq/gtemac.h>

#include "maps/common/characters/split_head.h"

u8                D_800D5D11;
s_WorldObjectPose g_WorldObject0;
s_WorldObjectPose g_WorldObject1;
s16               D_800D86F8[2];
u8                D_800D86FC[2];
u8                D_800D86FE;

s32 __pad_bss_800D5CF0[2];
s8  __pad_bss_800D5D01[7];
s8  __pad_bss_800D5D12[2];
s32 __pad_bss_800D5D24;
s8  __pad_bss_800D8564[4];
s32 __pad_bss_800D8590[2];
s32 __pad_bss_800D85C4;
s8  __pad_bss_800D8607;
s32 __pad_bss_800D868C[3];

#include "../src/maps/common/particles/fire.c"

#include "../src/maps/common/particles/drool.c"

#include "../src/maps/common/particles/particle.c"

#include "../src/maps/common/characters/player.c"


// TODO: Move this line into separate `Chara_SplitHead` split.
#include "../src/maps/common/characters/split_head.c" // 0x800CF818

#include "../src/maps/common/map_util.c" // 0x800D489C

void Map_RoomBgmInit(bool arg0) // 0x800D494C
{
    s32 bgmFlags;

    bgmFlags = 1;
    if (NPC_CHARA(0).health > Q12(0.0f))
    {
        bgmFlags = 2;
        if (NPC_CHARA(0).properties.splitHead.flags & SplitHeadFlag_4)
        {
            bgmFlags = 6;
        }
    }

    Bgm_LayersUpdate(bgmFlags, Q12(0.1f), &D_800D5C3C);
}

void GameBoot_LoadScreen_StageString(void) {}

const char* MAP_MESSAGES[] = {
    #include "maps/common/shared/map_msg_common.inc"
};

void func_800D49AC(void) // 0x800D49AC
{
    s32 i;

    if (Pad_ButtonClickedBind(skip))
    {
        if (g_SysWork.sysStateSteps[0] >= 3)
        {
            sharedData_800D8568_1_s05.field_1  = 0;
            sharedData_800D8568_1_s05.field_10 = 0;

            for (i = 0; i < ARRAY_SIZE(sharedData_800DFB7C_0_s00); i++)
            {
                s32 field_A = sharedData_800DFB7C_0_s00[i].field_A;
                if (field_A < 12)
                {
                    if (field_A >= 8)
                    {
                        sharedData_800DFB7C_0_s00[i].field_A = 12;
                    }
                }
            }

            vcReturnPreAutoCamWork(true);
            SysWork_StateStepReset();
        }
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);

            CutsceneBorder_ForceShow();
            g_SysWork.lightIntensity      = Q12(0.0f);

            // Warp camera.
            Event_CameraPositionSet(NULL,
                                    Q12(19.72f), Q12(-5.2f), Q12(-27.6f),
                                    Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                    true);
            Event_CameraLookAtSet(NULL,
                                  Q12(19.72f), Q12(-3.3f), Q12(-24.0f),
                                  Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                  true);

            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitTimer(Q12(1.5f), false);
            break;

        case 2:
            sharedData_800D8568_1_s05.field_1 = 3;
            SysWork_StateStepIncrement(0);

        case 3:
            Event_WaitTimer(Q12(0.2f), false);
            break;

        case 4:
            g_SysWork.lightIntensity += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.225f);
            if (g_SysWork.lightIntensity > Q12(2.25f))
            {
                g_SysWork.lightIntensity = Q12(2.25f);
                SysWork_StateStepIncrement(0);
            }
            break;

        case 5:
            g_SysWork.lightIntensity -= Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.45f);
            if (g_SysWork.lightIntensity < Q12(1.3843f))
            {
                Savegame_EventFlagSet(EventFlag_129);
                SysWork_StateStepReset();
            }
            break;

        default:
            Player_ControlUnfreeze(false);

            SysWork_StateSetNext(SysState_Gameplay);

            g_SysWork.lightIntensity = Q12(1.3843f);

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            vcReturnPreAutoCamWork(false);

            Savegame_EventFlagSet(EventFlag_130);
            func_8003A16C();
            break;
    }
}

void func_800D4D1C(void) // 0x800D4D1C
{
    typedef struct
    {
        SPRT*     sprt_0;
        DR_TPAGE* tpage_4;
        DR_STP*   stp_8;
        s32       activeBufferIdx_C;
    } g_GteScratchData_func_800D4D1C;

    g_GteScratchData_func_800D4D1C* scratchData;
    s32                             i;

    scratchData = PSX_SCRATCH_ADDR(0);

    if (g_SysWork.sysStateSteps[0] < 5)
    {
        if (g_SysWork.sysStateSteps[0] > 0)
        {
            scratchData->activeBufferIdx_C = g_ActiveBufferIdx;
            scratchData->sprt_0            = (SPRT*)GsOUT_PACKET_P;
            for (i = 0; i < 2; i++)
            {
                setCodeWord(scratchData->sprt_0, PRIM_RECT | RECT_BLEND | RECT_TEXTURE, COLOR_RGBC(128, 128, 128, 0));
                setXY0Fast(scratchData->sprt_0, ((i << 8) - 160), -112);
                scratchData->sprt_0->u0 = 0;
                scratchData->sprt_0->v0 = (scratchData->activeBufferIdx_C == 0) ? 32 : 0;
                setWH(scratchData->sprt_0, (i == 0) ? 256 : 64, FRAMEBUFFER_HEIGHT_PROGRESSIVE);
                addPrimFast(&g_OrderingTable2[g_ActiveBufferIdx].org[15], scratchData->sprt_0, 4);

                scratchData->sprt_0++;
                scratchData->tpage_4 = (DR_TPAGE*)scratchData->sprt_0;
                setDrawTPage(scratchData->tpage_4, 0, 0, getTPage(2, 0, ((scratchData->activeBufferIdx_C << 4) + (i << 2)) << 6, (((scratchData->activeBufferIdx_C << 4) >> 4) & 1) << 8));

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
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Savegame_EventFlagClear(EventFlag_129);
            D_800D5D11 = 1;
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitTimer(Q12(1.5f), false);

            g_SysWork.lightIntensity += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.2f);
            if (g_SysWork.lightIntensity > Q12(3.0f))
            {
                g_SysWork.lightIntensity = Q12(3.0f);
            }
            break;

        case 2:
            sharedData_800D8568_1_s05.field_1 = 1;
            sharedData_800D8568_1_s05.field_2 = 3;
            SysWork_StateStepIncrement(0);

        case 3:
            if (D_800D5D11 & 1)
            {
                SD_Call(Sfx_Unk1359);
                D_800D5D11++;
            }

            Sd_SfxAttributesUpdate(Sfx_Unk1359, 0, MAX(0, (Q12_FRACT(g_SysWork.lightIntensity) >> 4) - (D_800D5D11 * 8)), 0);

            g_SysWork.lightIntensity -= Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.15f);
            if (g_SysWork.lightIntensity < (6 - D_800D5D11) * Q12(0.5f))
            {
                D_800D5D11++;
            }

            if (g_SysWork.lightIntensity < Q12(0.0f))
            {
                g_SysWork.lightIntensity = Q12(0.0f);
                SysWork_StateStepIncrement(0);
            }
            break;

        case 4:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(2.5f), false);
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            Savegame_EventFlagSet(EventFlag_132);
            break;
    }
}

void Map_WorldObjectsInit(void) // 0x800D525C
{
    D_800D86F8[0] = 0;
    D_800D86F8[1] = 0;
    D_800D86FC[0] = 0x40;
    D_800D86FC[1] = 0x40;
    sharedData_800D8568_1_s05.flags = (1 << 3) | (1 << 0);
    sharedData_800D8568_1_s05.field_C = 0x12C;
    sharedData_800D8568_1_s05.field_12 = -0x2CCCu;
    sharedData_800D8568_1_s05.field_14 = -0x800;
    sharedData_800D8568_1_s05.field_1C = 0xA00;
    sharedData_800D8568_1_s05.field_E = -0x1000;
    sharedData_800D8568_1_s05.field_4 = 0x80;
    sharedData_800D8568_1_s05.field_5 = 0xC4;
    sharedData_800D8568_1_s05.field_6 = 0x3C;
    sharedData_800D8568_1_s05.field_8 = 0x200;
    sharedData_800D8568_1_s05.field_A = 0x400;
    sharedData_800D8568_1_s05.field_1 = 2;
    sharedData_800D8568_1_s05.field_2 = 8;
    sharedData_800D8568_1_s05.field_3 = 0x40;
    sharedData_800D8568_1_s05.field_20 = Q12(19.7f);
    sharedData_800D8568_1_s05.field_24 = Q12(-19.1f);
    D_800D86FE = 48;

    if (Savegame_EventFlagGet(EventFlag_130))
    {
        g_SysWork.lightIntensity = Q12(1.3843f);
        sharedData_800D8568_1_s05.field_1 = 0;
    }

    sharedFunc_800CAAD0_1_s05();
    WorldObject_Init(&g_WorldObject0, "SPHERE_H", 19.7f, 1.58f, -19.1f, 0.0f, 0.0f, 0.0f);

    WorldObject_Init(&g_WorldObject1, "WHEEL_HI", 19.7f, -0.06f, -19.1f, 0.0f, 0.0f, 0.0f);

    g_SysWork.lightBoneCoord = NULL;
    g_SysWork.lightPosition.vx = Q12(19.7f);
    g_SysWork.lightPosition.vy = Q12(-3.0f);
    g_SysWork.lightPosition.vz = Q12(-19.1f);

    g_SysWork.lensFlareBoneCoord = NULL;
    g_SysWork.lightRotation.vx = Q12_ANGLE(-90.0f);
    g_SysWork.lightRotation.vy = Q12_ANGLE(0.0f);
    g_SysWork.lightRotation.vz = Q12_ANGLE(0.0f);

    SD_Call(Sfx_Unk1478);
}

void Map_WorldObjectsUpdate(void) // 0x800D5400
{
    s32 sp18[2];
    s32 i;

    if (Savegame_EventFlagGet(EventFlag_129))
    {
        D_800D86F8[0] += g_DeltaTime;
        if ((D_800D86F8[0] * D_800D86FC[0]) >= Q12(SQUARE(8.0f)))
        {
            D_800D86F8[0] = 0;
            D_800D86FC[0] = Rng_GenerateUInt(32, 95);
            D_800D86FE    = Rng_GenerateUInt(32, 63);
        }

        sp18[0] = Q12_MULT(D_800D86FE, Math_Sin(D_800D86F8[1]));

        D_800D86F8[1] += g_DeltaTime;

        sp18[1] = (D_800D86F8[1] * D_800D86FC[1]) >> 6;

        if (sp18[1] >= Q12(1.0f))
        {
            D_800D86F8[1] = 0;
            D_800D86FC[1] = Rng_GenerateUInt(32, 95);
            sp18[1]       = Q12_FRACT(sp18[1]);
        }

        // TODO: Why doesn't `Q12_MULT_FLOAT_PRECISE(sp18[0], 0.6f)` work here?
        g_SysWork.lightIntensity = Q12(1.35f) + (((Q12(0.9f) + Q12_MULT_PRECISE(sp18[0], Q12(0.6f))) * Math_Sin(sp18[1])) >> 15);
    }

    g_WorldObject0.rotation.vy += Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(-90.0f));
    WorldObjects_Add(&g_WorldObject0.object, &g_WorldObject0.position, &g_WorldObject0.rotation);

    g_WorldObject1.rotation.vy += Q12_MULT_PRECISE(g_DeltaTime, Q12_ANGLE(15.0f));
    WorldObjects_Add(&g_WorldObject1.object, &g_WorldObject1.position, &g_WorldObject1.rotation);

    i = 0;
    while (1)
    {
        if (++i >= ARRAY_SIZE(sharedData_800DFB7C_0_s00))
        {
            break;
        }
    }

    Sfx_WithFalloffAndPitchPlay(Sfx_Unk1478, &Q12_VECTOR3(19.7f, -1.5f, -19.1f), Q8(0.5f) - Q12_MULT_PRECISE(sharedData_800D8568_1_s05.field_10, Q8(0.5f)), Q12(16.0f), 0);
}
