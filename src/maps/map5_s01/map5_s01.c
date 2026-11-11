#include "bodyprog/bodyprog.h"
#include "bodyprog/chara/dms.h"
#include "bodyprog/events/event_data.h"
#include "bodyprog/events/radio.h"
#include "bodyprog/world/effects.h"
#include "bodyprog/items/item_screens.h"
#include "bodyprog/math/math.h"
#include "bodyprog/sound/sound_system.h"
#include "main/rng.h"
#include "maps/map5/map5_s01.h"
#include "maps/common/particle.h"
#include "maps/common/characters/air_screamer.h"
#include "maps/common/characters/player.h"
#include "maps/common/characters/romper.h"
#include "inline_no_dmpsx.h"
#include <psyq/gtemac.h>
#include "bodyprog/player/player.h"
#include "maps/common/characters/groaner.h"

#include "../src/maps/common/particles/ufo.c"

s16 func_800CD20C(void) // 0x800CD20C
{
    s32      i;
    GsDOBJ2* itemModel;

    if (D_800F159C == 1)
    {
        if (g_SysWork.sysStateSteps[0] == 5)
        {
            if (Pad_ButtonPulsed(ControllerFlag_LStickHighDown))
            {
                D_800ED5C8 += 3;
                Sd_SfxPlay(Sfx_MenuMove, Q7(0.0f), Q7(0.5f));
            }

            if (Pad_ButtonPulsed(ControllerFlag_LStickHighUp))
            {
                D_800ED5C8 += 1;
                Sd_SfxPlay(Sfx_MenuMove, Q7(0.0f), Q7(0.5f));
            }

            D_800ED5C8 &= 0x3;

            if (Pad_ButtonHeld(ControllerFlag_LStickHighLeft))
            {
                Sd_SfxPlay(Sfx_Unk1589, Q7(0.0f), Q7(0.5f));

                D_800F1598[D_800ED5C8]++;
                if (D_800F1598[D_800ED5C8] == 8)
                {
                    D_800F1598[D_800ED5C8] = 0;
                }
            }

            if (Pad_ButtonHeld(ControllerFlag_LStickHighRight))
            {
                Sd_SfxPlay(Sfx_Unk1589, Q7(0.0f), Q7(0.5f));

                D_800F1598[D_800ED5C8]--;
                if (D_800F1598[D_800ED5C8] == NO_VALUE)
                {
                    D_800F1598[D_800ED5C8] = 7;
                }
            }
        }
    }

    D_800F15A0 = D_800F1598[0] + (D_800F1598[1] * 10) + (D_800F1598[2] * 100) + (D_800F1598[3] * 1000);

    func_800CE180();

    func_800CD51C(D_800F15A0, D_800ED5C8);
    PushMatrix();
    func_8004BBF4(&D_800F1570, &D_800F1510, &D_800F1560);

    for (itemModel = g_Items_ItemsModelData, i = 0; i < 6; i++, itemModel++)
    {
        g_Items_Transforms[i].rotate.vx = Q12_ANGLE(90.0f);
        g_Items_Transforms[i].rotate.vz = Q12_ANGLE(0.0f);

        ItemScreen_ItemRotate(&g_Items_Coords[i].param->rotate, &g_Items_Coords[i]);

        if (D_800F159C != 0)
        {
            func_800CDB14(i, D_800ED5C8);
        }

        GsSetFlatLight(0, &D_800F1450[i][0]);
        func_8004BD74(i, itemModel, 0);
    }

    PopMatrix();
    func_800CDB98();

    if (D_800F159C == 1)
    {
        return D_800F15A0;
    }
    else
    {
        return 0;
    }
}

void func_800CD51C(s32 arg0, s32 arg1) // 0x800CD51C
{
    s32 temp_v0;
    s32 i;
    s32 var_t1;
    s32 var_v0_2;
    s32 var_v0_3;
    s8  temp_v1_2;

    var_t1 = 0;

    switch (D_800F159C)
    {
        case 0:
            for (i = 0; i < 6; i++)
            {
                g_Items_Transforms[i].scale.vx =
                g_Items_Transforms[i].scale.vy =
                g_Items_Transforms[i].scale.vz = Q12(2.0f);

                temp_v0 = (D_800F159D * 0xFF) / 32;

                D_800F1450[i][1].b = temp_v0;
                D_800F1450[i][1].g = temp_v0;
                D_800F1450[i][1].r = temp_v0;
                D_800F1450[i][0].b = temp_v0;
                D_800F1450[i][0].g = temp_v0;
                D_800F1450[i][0].r = temp_v0;
            }

            D_800F159D++;

            if (D_800F159D >= 32)
            {
                D_800F159C = 1;
                D_800F159D = 0;
            }
            break;

        case 1:
            if (D_800F1590 != arg0)
            {
                D_800F159C = 2;
            }

            for (i = 0; i < 6; i++)
            {
                switch (i)
                {
                    case 0:
                        g_Items_Coords[i].coord.t[0] = Q8(0.0f);
                        g_Items_Coords[i].coord.t[1] = Q8(-0.204f);
                        g_Items_Coords[i].coord.t[2] = Q8(-32.5f);
                        break;

                    case 5:
                        g_Items_Coords[i].coord.t[0] = Q8(0.0f);
                        g_Items_Coords[i].coord.t[1] = Q8(-0.08f);
                        g_Items_Coords[i].coord.t[2] = Q8(-32.5f);
                        break;

                    default:
                        g_Items_Coords[i].coord.t[0] = Q8(0.0f);
                        g_Items_Coords[i].coord.t[1] = Q8(1.555f) - (i * Q8(0.415f));
                        g_Items_Coords[i].coord.t[2] = Q8(-32.5f);
                        break;
                }
            }
            break;

        case 2:
            D_800F159D++;

            if ((D_800F1594[arg1] < D_800F1598[arg1] && (D_800F1594[arg1] != 0 || D_800F1598[arg1] != 7)) ||
                (D_800F1594[arg1] == 7 && D_800F1598[arg1] == 0))
            {
                var_t1 = 1;
            }
            else
            {
                var_t1 = -1;
            }

            if (D_800F159D >= 16)
            {
                D_800F159C = 1;
                D_800F159D = 0;
                D_800F1590 = arg0;

                for (i = 0; i < 4; i++)
                {
                    D_800F1594[i] = D_800F1598[i];
                }

                var_t1 = 0;
            }
            break;
    }

    var_v0_2 = var_t1 * (D_800F159D << 9);

    g_Items_Transforms[0].rotate.vy = Q12_ANGLE(292.5f);
    g_Items_Transforms[5].rotate.vy = Q12_ANGLE(292.5f);

    for (i = 1; i < 5; i++)
    {
        temp_v1_2 = D_800F1594[i - 1];
        if (temp_v1_2 == D_800F1598[i - 1])
        {
            var_v0_3 = temp_v1_2 << 9;
        }
        else
        {
            var_v0_3 = (temp_v1_2 << 9) + (var_v0_2 / 16);
        }

        g_Items_Transforms[i].rotate.vy = var_v0_3 + Q12_ANGLE(247.5f);
    }
}

void func_800CD818(void) // 0x800CD818
{
    Fs_QueueStartReadTim(FILE_ITEM_FOOK_TIM, FS_BUFFER_1, &D_800A907C);
    Fs_QueueStartRead(FILE_ITEM_FOOK_TMD, FS_BUFFER_21);
    Fs_QueueWaitForEmpty();
}

void func_800CD860(void) // 0x800CD860
{
    GsCOORD2PARAM* var_v1;
    GsCOORDINATE2* var_s1_2;
    s32            i;
    s16            temp_a0;
    s16            var_s1;

    var_s1                 = D_800F15A0;
    g_Items_DisplayedCount = 0;

    D_800F159C = 0;
    D_800F159D = 0;
    D_800ED5C8 = 3;

    ItemScreen_CameraSet(&D_800F1570, &D_800F1510, (SVECTOR3*)&D_800F1560, 0);

    for (i = 0; i < 6; i++)
    {
        g_Items_Transforms[i].rotate.vz = Q12_ANGLE(0.0f);;
        g_Items_Transforms[i].rotate.vx = Q12_ANGLE(0.0f);;
        g_Items_Transforms[i].trans.vz  = 0;
        g_Items_Transforms[i].trans.vy  = 0;
        g_Items_Transforms[i].trans.vx  = 0;

        switch (i)
        {
            case 0:
            case 5:
                g_Items_Transforms[i].rotate.vy = Q12_ANGLE(112.5f);
                break;

            default:
                D_800F1594[i] = temp_a0         = var_s1 % 10;
                var_s1                          = var_s1 / 10;
                g_Items_Transforms[i].rotate.vy = (temp_a0 << 9) + Q12_ANGLE(247.5f);
                break;
        }

        g_Items_Transforms[0].rotate.vy = Q12_ANGLE(292.5f);
        g_Items_Transforms[5].rotate.vy = Q12_ANGLE(292.5f);

        for (i = 1; i < 5; i++)
        {
            g_Items_Transforms[i].rotate.vy = (D_800F1598[i - 1] << 9) + Q12_ANGLE(247.5f);
        }
    }

    GameFs_TmdDataAlloc((s32*)FS_BUFFER_21);

    for (i = 0; i < 6; i++)
    {
        Gfx_Items_Display((s_TmdFile*)FS_BUFFER_21, i, i);
        func_8005487C(i);

        switch (i)
        {
            case 0:
                g_Items_Coords[i].coord.t[0] = Q8(0.0f);
                g_Items_Coords[i].coord.t[1] = Q8(-0.205f);
                g_Items_Coords[i].coord.t[2] = Q8(-32.5f);
                break;

            case 5:
                g_Items_Coords[i].coord.t[0] = Q8(0.0f);
                g_Items_Coords[i].coord.t[1] = Q8(-0.08f);
                g_Items_Coords[i].coord.t[2] = Q8(-32.5f);
                break;

            default:
                g_Items_Coords[i].coord.t[0] = Q8(0.0f);
                g_Items_Coords[i].coord.t[1] = Q8(1.555f) - (Q8(0.415f) * i);
                g_Items_Coords[i].coord.t[2] = Q8(-32.5f);
                break;
        }
    }

    func_800CDA8C();
}

void func_800CDA8C(void) // 0x800CDA8C
{
    s32 i;

    for (i = 0; i < 6; i++)
    {
        D_800F1450[i][0].vx = g_Items_Coords[i].coord.t[0];
        D_800F1450[i][0].vy = g_Items_Coords[i].coord.t[1];
        D_800F1450[i][0].vz = g_Items_Coords[i].coord.t[2] + 0x4E20;

        D_800F1450[i][1].vx = Q12(1.0f);
        D_800F1450[i][1].vy = 0;
        D_800F1450[i][1].vz = 0;
    }

    GsSetAmbient(256, 256, 256);
    GsSetLightMode(1);
}

void func_800CDB14(s32 arg0, s32 arg1) // 0x800CDB14
{
    s32 i;

    D_800F1450[arg0][0].vx = g_Items_Coords[arg0].coord.t[0];
    D_800F1450[arg0][0].vy = g_Items_Coords[arg0].coord.t[1];
    D_800F1450[arg0][0].vz = g_Items_Coords[arg0].coord.t[2] + 0x4E20;

    D_800F1450[arg0][1].vx = 0;
    D_800F1450[arg0][1].vy = 0;
    D_800F1450[arg0][1].vz = 0;

    for (i = 0; i < 2; i++)
    {
        D_800F1450[arg0][i].r = 255;
        D_800F1450[arg0][i].g = 255;
        D_800F1450[arg0][i].b = 255;
    }
}

void func_800CDB98(void) // 0x800CDB98
{
    GsOT*    ot;
    s32      i;
    POLY_G3* poly_g3;
    POLY_G4* poly_g4;
    s32      temp_s5;
    s32      temp_s6;
    s32      temp_s7;

    ot = &g_OrderingTable2[g_ActiveBufferIdx];

    for (i = 0; i < 16; i++)
    {
        poly_g3 = GsOUT_PACKET_P;

        setPolyG3(poly_g3);
        setSemiTrans(poly_g3, 1);

        if (D_800F159C == 0)
        {
            setRGB0(poly_g3, D_800F159D * 4, D_800F159D * 4, (D_800F159D * 112) / 32);
            setRGB1(poly_g3, (D_800F159D * 98) / 32, (D_800F159D * 98) / 32, (D_800F159D * 84) / 32);
            setRGB2(poly_g3, (D_800F159D * 98) / 32, (D_800F159D * 98) / 32, (D_800F159D * 84) / 32);
        }
        else
        {
            setRGB0(poly_g3, 128, 128, 112);
            setRGB1(poly_g3, 98, 98, 84);
            setRGB2(poly_g3, 98, 98, 84);
        }

        temp_s5 = 90;

        setXY0Fast(poly_g3, 0, 0);
        setXY1Fast(poly_g3, Q12_MULT(Math_Sin(i << 8), temp_s5), Q12_MULT(Math_Cos(i << 8), temp_s5));
        setXY2Fast(poly_g3, Q12_MULT(Math_Sin((i + 1) << 8), temp_s5), Q12_MULT(Math_Cos((i + 1) << 8), temp_s5));

        addPrim(&ot->org[10], poly_g3);
        GsOUT_PACKET_P = poly_g3 + 1;
    }

    for (i = 0; i < 16; i++)
    {
        poly_g4 = GsOUT_PACKET_P;

        setPolyG4(poly_g4);
        setSemiTrans(poly_g4, 1);

        if (D_800F159C == 0)
        {
            setRGB0(poly_g4, D_800F159D * 3, D_800F159D * 3, (D_800F159D * 84) / 32);
            setRGB1(poly_g4, D_800F159D * 3, D_800F159D * 3, (D_800F159D * 84) / 32);
        }
        else
        {
            setRGB0(poly_g4, 96, 96, 84);
            setRGB1(poly_g4, 96, 96, 84);
        }

        setRGB2(poly_g4, 0, 0, 0);
        setRGB3(poly_g4, 0, 0, 0);

        temp_s7 = 90;
        temp_s6 = 180;

        setXY0Fast(poly_g4, Q12_MULT(Math_Sin(i << 8), temp_s7), Q12_MULT(Math_Cos(i << 8), temp_s7));
        setXY1Fast(poly_g4, Q12_MULT(Math_Sin((i + 1) << 8), temp_s7), Q12_MULT(Math_Cos((i + 1) << 8), temp_s7));
        setXY2Fast(poly_g4, Q12_MULT(Math_Sin(i << 8), temp_s6), Q12_MULT(Math_Cos(i << 8), temp_s6));
        setXY3Fast(poly_g4, Q12_MULT(Math_Sin((i + 1) << 8), temp_s6), Q12_MULT(Math_Cos((i + 1) << 8), temp_s6));

        addPrim(&ot->org[10], poly_g4);
        GsOUT_PACKET_P = poly_g4 + 1;
    }

    Gfx_Primitive2dTextureSet(0, 0, 10, 1);
}

void func_800CE180(void) // 0x800CE180
{
    GsOT*    ot;
    s32      i;
    POLY_G3* poly_g3;

    ot = &g_OrderingTable2[g_ActiveBufferIdx];

    for (i = 0; i < 2; i++)
    {
        poly_g3 = GsOUT_PACKET_P;

        setPolyG3(poly_g3);
        setSemiTrans(poly_g3, 1);

        setRGB0(poly_g3, 192, 128, 0);
        setRGB1(poly_g3, 192, 128, 0);
        setRGB2(poly_g3, 255, 64, 0);

        setXY0Fast(poly_g3, -48 + (96  * i), 29 - (D_800ED5C8 * 20));
        setXY1Fast(poly_g3, -48 + (96  * i), 45 - (D_800ED5C8 * 20));
        setXY2Fast(poly_g3, -64 + (128 * i), 37 - (D_800ED5C8 * 20));

        addPrim(&ot->org[9], poly_g3);

        GsOUT_PACKET_P = poly_g3 + 1;
    }
}

#include "../src/maps/common/particles/particle.c"

#include "../src/maps/common/characters/player.c"

#include "../src/maps/common/characters/air_screamer.c" // 0x800D225C
#include "../src/maps/common/characters/groaner.c" // 0x800E3294

// TODO: Move this line into separate `Chara_Romper` split.
#include "../src/maps/common/characters/romper.c" // 0x800E786C

#include "../src/maps/common/map_util.c" // 0x800EB460

void func_800EB6B0(void) // 0x800EB6B0
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

void GameBoot_LoadScreen_StageString(void) {}

#include "maps/common/shared/MapEvent_DoorJammed.h" // 0x800EB87C

#include "maps/common/shared/MapEvent_DoorLocked.h" // 0x800EB910

const char* MAP_MESSAGES[] = {
    #include "maps/common/shared/map_msg_common.inc"
#if VERSION_REGION_IS(NTSC)
    /* 15 */ "~H\tThere_is_a_ ~C5 Resort_area_map ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 16 */ "\tNothing_unusual. ~E ",
    /* 17 */ "\tUsed_the_ ~C2 Kaufmann_key ~C7 . ~E ",
    /* 18 */ "~J0(1.0)\tWhat? ~E ",
    /* 19 */ "~J0(2.5)\tThat_light!? ~E "
#elif VERSION_REGION_IS(NTSCJ)
    /* 15 */ "~H\t~C5 リゾート街の地図 ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 16 */ "\t特に気になることはない ~E ",
    /* 17 */ "\t~C2 カウフマンの鍵 ~C7 を使った ~E ",
    /* 18 */ "~J0(1.0)\t何だ？ ~E ",
    /* 19 */ "~J0(2.5)\tあの光は！？ ~E"
#endif
};

void MapEvent_CommonItemTake(void) // 0x800EB9A4
{
    u32 pickupType;
    s32 eventFlagIdx;

    pickupType   = CommonPickupItemId_FirstAidKit;
    eventFlagIdx = 0;

    switch (g_MapEventData->mapPointIdx)
    {
        case 40:
            pickupType   = CommonPickupItemId_RifleShells;
            eventFlagIdx = EventFlag_M5S01_RifleShells0;
            break;

        case 41:
            pickupType   = CommonPickupItemId_HandgunBullets;
            eventFlagIdx = EventFlag_M5S01_HandgunBullets;
            break;

        case 42:
            pickupType   = CommonPickupItemId_FirstAidKit;
            eventFlagIdx = EventFlag_M5S01_FirstAidKit;
            break;

        case 43:
            pickupType   = CommonPickupItemId_HealthDrink;
            eventFlagIdx = EventFlag_M5S01_HealthDrink;
            break;

        case 44:
            pickupType   = CommonPickupItemId_RifleShells;
            eventFlagIdx = EventFlag_M5S01_RifleShells1;
            break;
    }

    Event_CommonItemTake(pickupType, eventFlagIdx);
}

void func_800EBA40(void) // 0x800EBA40
{
    s32 i;
    s32 j;

    Game_RadioSoundStop();

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 0, Q12(0.0f), false);
            D_800F0358 = 0;
            D_800F0354 = 0;

            for (i = 0; i < ARRAY_SIZE(D_800F0350); i++)
            {
                D_800F0350[i] = 10;
            }

            SysWork_StateStepIncrement(0);

        case 1:
            Event_BgTextureCmd(BgTextureCmd_Auto, FILE_TIM_DORPANEL_TIM, false);
            break;

        case 2:
            Event_ScreenFadeCmd(ScreenFadeCmd_Wait, true, 0, Q12(0.0f), false);
            break;

        case 3:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.0f), false);
            Screen_BackgroundImgDraw(&g_ItemInspectionImg);
            break;

        case 4:
            Screen_BackgroundImgDraw(&g_ItemInspectionImg);

            D_800F0354 += (g_Controller0->normalizedSticks.sticks.leftX * 16384) / 75;
            D_800F0354  = CLAMP_RANGE(D_800F0354, Q12(-160.0f), Q12(160.0f));

            D_800F0358 += (g_Controller0->normalizedSticks.sticks.leftY * 16384) / 75;
            D_800F0358  = CLAMP_RANGE(D_800F0358, Q12(-120.0f), Q12(120.0f));

            Game_TimerUpdate();
            Gfx_CursorDraw((s16)(FP_FROM(D_800F0354, Q12_SHIFT) + 8), FP_FROM(D_800F0358, Q12_SHIFT) + 8, 8, 8, 0, 64, 32, 32, 128, 192, 0, 12);

            if (Pad_ButtonClickedBind(cancel))
            {
                SysWork_StateStepSet(0, 7);
                break;
            }

            if (Pad_ButtonClickedBind(enter))
            {
                for (i = 0; i < ARRAY_SIZE(D_800F0158); i++)
                {
                    if ((D_800F0158[i].field_0 - 160) > FP_FROM(D_800F0354, Q12_SHIFT) ||
                        (D_800F0158[i].field_0 - 114) < FP_FROM(D_800F0354, Q12_SHIFT))
                    {
                        continue;
                    }

                    if ((D_800F0158[i].field_1 - 120) > FP_FROM(D_800F0358, Q12_SHIFT) ||
                        (D_800F0158[i].field_1 - 74) < FP_FROM(D_800F0358, Q12_SHIFT))
                    {
                        continue;
                    }

                    Sd_SfxPlay(Sfx_Unk1586, Q7(0.0f), Q7(1.0f));

                    if (i < 11)
                    {
                        for (j = 0; j < (ARRAY_SIZE(D_800F0350) - 1); j++)
                        {
                            D_800F0350[j] = D_800F0350[j + 1];
                        }

                        D_800F0350[3] = i;
                        break;
                    }

                    for (j = 0; j < ARRAY_SIZE(D_800F0350); j++)
                    {
                        if (D_800F0170[j] != D_800F0350[j])
                        {
                            break;
                        }
                    }

                    if (j == 4)
                    {
                        Savegame_EventFlagSet(EventFlag_387);
                        SysWork_StateStepIncrement(0);
                    }
                    else
                    {
                        for (j = 0; j < ARRAY_SIZE(D_800F0350); j++)
                        {
                            D_800F0350[j] = 10;
                        }
                    }
                    break;
                }
            }
            break;

        case 5:
            Event_WaitTimer(Q12(0.6f), false);
            Screen_BackgroundImgDraw(&g_ItemInspectionImg);
            break;

        case 6:
            Sd_SfxPlay(Sfx_Unk1587, Q7(0.0f), Q7(1.0f));
            SysWork_StateStepIncrement(0);

        case 7:
            Screen_BackgroundImgDraw(&g_ItemInspectionImg);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        default:
            // Return to gameplay.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            Game_RadioNoiseReset();
            break;
    }
}

void MapEvent_PaperMapTake(void) // 0x800EBF48
{
    Event_PaperMapTake(4, EventFlag_M5S01_PickupMap, 15);
}

void func_800EBF70(void) // 0x800EBF70
{
    VECTOR3 camPos;    // Q19.12
    VECTOR3 canLookAt; // Q19.12
    s16     temp_v0;
    s16     temp;

    temp = 0x1D9;

    if (g_SysWork.sysStateSteps[0] > 2)
    {
        Game_RadioSoundStop();
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            func_800CD818();

            CutsceneBorder_ForceShow();

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 2, 0, false);

            PLAYER_CHARA().position.vx = Q12(52.0f);
            PLAYER_CHARA().position.vz = Q12(-57.8f);
            PLAYER_CHARA().rotation.vy = Q12_ANGLE(107.6f);

            camPos.vx = Q12(49.8f);
            camPos.vy = Q12(-1.5f);
            camPos.vz = Q12(-60.0f);

            canLookAt.vx = Q12(52.5f);
            canLookAt.vy = Q12(-1.3f);
            canLookAt.vz = Q12(-57.9f);

            Event_CameraPositionSet(&camPos, 0, 0, 0, 0, 0, 0, 0, true);
            Event_CameraLookAtSet(&canLookAt, 0, 0, 0, 0, 0, 0, 0, true);
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitTimer(Q12(1.0f), false);
            break;

        case 2:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, 0, false);
            break;

        case 3:
            if (Fs_QueueChunksLoad())
            {
                g_SysWork.bgmStatusFlags |= BgmStatusFlag_Pause;

                func_800CD860();
                SysWork_StateStepIncrement(0);
            }

        case 4:
            g_SysWork.bgmStatusFlags |= BgmStatusFlag_Pause;
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, 0, false);
            break;

        case 5:
            Game_TimerUpdate();
            g_SysWork.bgmStatusFlags |= BgmStatusFlag_Pause;

            temp_v0 = func_800CD20C();
            if (!Pad_ButtonClickedBind(cancel))
            {
                if (temp != temp_v0)
                {
                    break;
                }

                Savegame_EventFlagSet(EventFlag_371);
                Sd_SfxPlay(Sfx_Unk1588, Q7(0.0f), Q7(1.0f));
            }

            SysWork_StateStepIncrement(0);
            break;

        case 6:
            g_SysWork.bgmStatusFlags |= BgmStatusFlag_Pause;

            func_800CD20C();
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, 0, false);
            break;

        case 7:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, 0, false);

        default:
            // Return to gameplay.
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, 0, false);
            vcReturnPreAutoCamWork(true);
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            Game_RadioNoiseReset();
            break;
    }
}

void func_800EC2D8(void) // 0x800EC2D8
{
    VECTOR3 sfxPos = { MAP_POINTS[g_MapEventData->mapPointIdx].positionX, Q12(-1.2f), MAP_POINTS[g_MapEventData->mapPointIdx].positionZ };

    Player_ItemRemove(InvItemId_KaufmannKey, 1);
    Event_DisplayMapMsgWithSfx(17, Sfx_UseKey, &sfxPos); // "Used the Kaufmann key."
    Savegame_EventFlagSet(EventFlag_M5S01_KaufmannKeyUsed);
}

#include "maps/common/shared/Event_CutsceneTimerAdvance.h" // 0x800EC394

void func_800EC42C(void) // 0x800EC42C
{
    s32    mulZ;
    s32    mulX;
    s32    pitch;
    s32    tmp0;
    s32    tmp1;
    q19_12 sin0;
    q19_12 sin1;

    tmp0 = D_800F0360;
    sin0 = Math_Sin(tmp0 >> 2);
    sin1 = Math_Sin(tmp0 >> 3);
    tmp1 = ((sin0 * 2) + sin1) + Math_Sin(Math_Cos(tmp0 >> 4));
    pitch = tmp1 >> 8;
    D_800F0360 += g_DeltaTime;

    if (g_SysWork.sysStateSteps[0] >= 10)
    {
        D_800F035E -= Q12_MULT_PRECISE(g_DeltaTime, Q12(0.5f));
        if (D_800F035E < 0)
        {
            D_800F035E = 0;
        }

        Sd_SfxAttributesUpdate(Sfx_Unk1467, 0, ~(D_800F035E >> 4), pitch);
    }
    else if (g_SysWork.sysStateSteps[0] > 0)
    {
        D_800F035E += Q12_MULT_PRECISE(g_DeltaTime, Q12(0.25f));
        if (D_800F035E > Q12(0.9961f))
        {
            D_800F035E = Q12(0.9961f);
        }

        Sd_SfxAttributesUpdate(Sfx_Unk1467, 0, ~(D_800F035E >> 4), pitch);
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Fs_QueueStartReadTim(FILE_TIM_ENBAN_TIM, FS_BUFFER_1, &D_800F0178);
            Fs_QueueStartRead(FILE_ANIM_UFO3_DMS, FS_BUFFER_13);

            g_Cutscene_Timer = NO_VALUE;

            Screen_FadeTimestepReset();
            SD_Call(Sfx_Unk1467);
            Sd_SfxAttributesUpdate(Sfx_Unk1467, 0, Q8_CLAMPED(1.0f), 0);

            D_800F035E = 0;
            D_800F0360 = 0;
            D_800F035D = 0;

            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            if (Fs_QueueChunksLoad() != false)
            {
                SysWork_StateStepIncrement(0);
            }
            break;

        case 3:
            g_Cutscene_Timer = 0;

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 3, 0, false);
            Dms_HeaderFixOffsets((s_DmsHeader* )FS_BUFFER_13);
            func_8003D03C();
            Player_EmptyWeaponHandSet();
            Game_TurnFlashlightOn();

            D_800F035C = 0;

            SysWork_StateStepIncrement(0);

        case 4:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, 0, false);
            break;

        case 5:
            Event_DisplayMapMsgWithAudio(18, &D_800F035C, &D_800F0174);
            break;

        case 6:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(40.0f), Q12(0.0f), Q12(30.0f), true, true);
    block_18:
            sharedFunc_800CE5D4_1_s03(&D_800F0180, Q12(0.5f), Q12(0.05f), 0);
            break;

        case 7:
            Event_DisplayMapMsg(false, 19, 0, 0, 0, false);
            sharedFunc_800CE5D4_1_s03(&D_800F0180, Q12(0.5f), Q12(0.05f), 0);
            break;

        case 8:
            Vc_SetLookAtMatFromBoneCoord(&g_SysWork.playerBoneCoords[HarryBone_Head], &SVECTOR3_Zero, &Q12_VECTOR3(-196.53f, -14.25f, -9.73f));
            sharedFunc_800CE5D4_1_s03(&D_800F0180, Q12(1.0f), Q12(0.1f), 1);
            Event_WaitTimer(Q12(2.5f), false);
            break;

        case 9:
            g_SysWork.sysStateStepData[0] += g_DeltaTime;

            switch (g_SysWork.sysStateStepData[0] / Q12(0.1f))
            {
                case 0:
                case 1:
                case 2:
                case 3:
                    mulZ = Q12(0.0f);
                    mulX = Q12(0.0f);
                    break;

                case 4:
                case 5:
                case 6:
                case 11:
                case 12:
                case 13:
                case 18:
                case 19:
                case 20:
                    mulX = Q12(-0.9f);
                    mulZ = Q12(1.0f);
                    break;

                case 7:
                case 8:
                case 9:
                case 10:
                case 14:
                case 15:
                case 16:
                case 17:
                case 21:
                case 22:
                case 23:
                case 24:
                    mulX = Q12(1.2f);
                    mulZ = Q12(1.0f);
                    break;

                case 25:
                    if (D_800F035D == 0)
                    {
                        D_800F0180.vy = Q12(-13.0f);
                        D_800F0180.vz = Q12(2.5f);
                        D_800F035D++;
                    }

                case 26:
                case 27:
                case 31:
                case 32:
                case 33:
                case 37:
                case 38:
                case 39:
                case 43:
                case 44:
                case 45:
                    mulX = Q12(-1.2f);
                    mulZ = Q12(-1.2f);
                    break;

                case 28:
                case 29:
                case 30:
                case 34:
                case 35:
                case 36:
                case 40:
                case 41:
                case 42:
                    mulX = Q12(1.2f);
                    mulZ = Q12(-1.2f);
                    break;

                case 46:
                    if (D_800F035D == 1)
                    {
                        D_800F0180.vy = Q12(-15.75f);
                        D_800F0180.vz = Q12(-12.25f);
                        D_800F035D++;
                    }

                case 59:
                    mulX = Q12(0.0f);
                    mulZ = Q12(-1.6f);
                    break;

                case 47:
                case 48:
                case 49:
                case 56:
                case 57:
                case 58:
                    mulX = Q12(0.6f);
                    mulZ = Q12(-1.6f);
                    break;

                case 50:
                case 51:
                case 52:
                case 53:
                case 54:
                case 55:
                    mulX = Q12(-0.6f);
                    mulZ = Q12(-1.6f);
                    break;

                default:
                    mulZ = Q12(0.0f);
                    mulX = Q12(0.0f);
                    SysWork_StateStepIncrement(0);
                    break;
            }

            D_800F0180.vy += Q12_MULT_PRECISE(g_DeltaTime, mulX * 4);
            D_800F0180.vz += Q12_MULT_PRECISE(g_DeltaTime, mulZ * 4);

            sharedFunc_800CE5D4_1_s03(&D_800F0180, Q12(0.5f), Q12(0.05f), 0);
            break;

        case 10:
            Event_WaitTimer(Q12(2.5f), false);
            break;

        default:
            // Return to gameplay.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, 0, false);

            vcReturnPreAutoCamWork(true);
            Savegame_EventFlagSet(EventFlag_458);
            Event_InvItemCmd(InvItemCmd_AddItem, InvItemId_ChannelingStone, 1, false);
            func_8003D01C();
            Player_WeaponAttackRestore();

            g_Cutscene_Timer = NO_VALUE;

            Sd_SfxStop(Sfx_Unk1467);
            break;
    }

    if (g_Cutscene_Timer >= Q12(0.0f))
    {
        Dms_CharacterTransformGet(&PLAYER_CHARA().position, &PLAYER_CHARA().rotation, "HERO", g_Cutscene_Timer, (s_DmsHeader*)FS_BUFFER_13);

        PLAYER_CHARA().position.vx -= Q12(20.0f);

        vcChangeProjectionValue(Dms_CameraTargetsGet(&g_Cutscene_CameraPositionTarget, &g_Cutscene_CameraLookAtTarget, NULL, g_Cutscene_Timer, (s_DmsHeader*)FS_BUFFER_13));

        g_Cutscene_CameraPositionTarget.vx -= Q12(20.0f);
        g_Cutscene_CameraLookAtTarget.vx -= Q12(20.0f);

        vcUserCamTarget(&g_Cutscene_CameraPositionTarget, NULL, true);
        vcUserWatchTarget(&g_Cutscene_CameraLookAtTarget, NULL, true);
    }
}

void Map_WorldObjectsInit(void) // 0x800ECB58
{
    WorldObject_PlacementInit(&g_WorldObject0, "RSRMAP_H", -46.5f, 0.0f, 2.5f);
    WorldObject_Init(&g_WorldObject1, "FOOK_HID", 52.15f, -1.359f, -57.925f, 0.0f, 0.0f, 5.8f);

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

    WorldObject_ModelNameSet(&g_CommonWorldObjects[0], g_CommonWorldObjectNames[2]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[1], g_CommonWorldObjectNames[3]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[2], g_CommonWorldObjectNames[4]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[3], g_CommonWorldObjectNames[5]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[4], g_CommonWorldObjectNames[6]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[5], g_CommonWorldObjectNames[7]);
}

void Map_WorldObjectsUpdate(void) // 0x800ECC8C
{
    if ((PLAYER_IN_MAP_CHUNK(-2, 0)) ||
        (PLAYER_IN_MAP_CHUNK(-2, -1)))
    {
            if (!Savegame_EventFlagGet(EventFlag_M5S01_PickupMap))
            {
                WorldObjects_Add(&g_WorldObject0.object, &g_WorldObject0.position, &SVECTOR3_Zero);
            }
    }

    if (PLAYER_IN_MAP_CHUNK(1, -2))
    {
            if (!Savegame_EventFlagGet(EventFlag_371))
            {
                WorldObjects_Add(&g_WorldObject1.object, &g_WorldObject1.position, &g_WorldObject1.rotation);
            }
    }

    if (PLAYER_IN_MAP_CHUNK(-2, 0))
    {
        if (!Savegame_EventFlagGet(EventFlag_M5S01_RifleShells0))
        {
            WorldObjects_Add(&g_CommonWorldObjects[4], &g_CommonWorldObjectPoses[0].position, &g_CommonWorldObjectPoses[0].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(-2, 0))
    {
        if (!Savegame_EventFlagGet(EventFlag_M5S01_HandgunBullets))
        {
            WorldObjects_Add(&g_CommonWorldObjects[3], &g_CommonWorldObjectPoses[1].position, &g_CommonWorldObjectPoses[1].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(-2, 0))
    {
        if (!Savegame_EventFlagGet(EventFlag_M5S01_FirstAidKit))
        {
            WorldObjects_Add(g_CommonWorldObjects, &g_CommonWorldObjectPoses[2].position, &g_CommonWorldObjectPoses[2].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(-2, -5))
    {
        if (!Savegame_EventFlagGet(EventFlag_M5S01_HealthDrink))
        {
            WorldObjects_Add(&g_CommonWorldObjects[1], &g_CommonWorldObjectPoses[3].position, &g_CommonWorldObjectPoses[3].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(-2, -5))
    {
        if (!Savegame_EventFlagGet(EventFlag_M5S01_RifleShells1))
        {
            WorldObjects_Add(&g_CommonWorldObjects[4], &g_CommonWorldObjectPoses[4].position, &g_CommonWorldObjectPoses[4].rotation);
        }
    }
}
