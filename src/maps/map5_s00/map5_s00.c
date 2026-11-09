
#include "inline_no_dmpsx.h"

#include <psyq/gtemac.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/events/event_data.h"
#include "bodyprog/gfx/map_effects.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player/player.h"
#include "main/rng.h"
#include "maps/map5/map5_s00.h"
#include "maps/common/particle.h"
#include "maps/common/characters/player.h"
#include "maps/common/characters/hanged_scratcher.h"
#include "maps/common/characters/creeper.h"
#include "maps/common/shared.h"

void func_800CB0D8(void) // 0x800CB0D8
{
    s32 idx;
    s32 i;

    idx = func_8005E7E0(32);
    if (idx == NO_VALUE)
    {
        return;
    }

    sharedData_800DFB7C_0_s00[idx].field_B = 0;
    sharedData_800DFB7C_0_s00[idx].field_C.s_1.field_2 = Rng_GenerateInt(0, 2);

    for (i = 0; i < 12; i++)
    {
        idx = func_8005E7E0(32);
        if (idx == NO_VALUE)
        {
            break;
        }

        sharedData_800DFB7C_0_s00[idx].field_B = 1;
        sharedData_800DFB7C_0_s00[idx].field_0.vx_0 = Rng_GenerateInt(Q12(-92.75f), Q12(-91.25f) - 1);
        sharedData_800DFB7C_0_s00[idx].vy_8 = Q12(0.5f);
        sharedData_800DFB7C_0_s00[idx].field_4.vz_4 = Rng_GenerateInt(Q12(7.35f), Q12(8.85f) - 1);
        sharedData_800DFB7C_0_s00[idx].field_C.s_2.field_0 = Rng_GenerateUInt(0, Q12_CLAMPED(1.0f));
        sharedData_800DFB7C_0_s00[idx].field_C.s_1.field_2 = Rng_GenerateInt(0, 2);
    }
}

bool func_800CB25C(POLY_FT4** poly, s32 idx) // 0x800CB25C
{
    typedef struct
    {
        s_func_8005E89C field_0;
        SVECTOR         field_12C[3];
        DVECTOR         field_144;
        VECTOR3         field_148;
        s32             field_154;
        s32             field_158;
        s32             field_15C;
        s32             field_160;
    } s_func_800CB25C;

    DVECTOR          sp10[5][5];
    s32              sp78[5][5];
    s32              j;
    s32              i;
    s32              var_v0_4;
    s_func_800CB25C* ptr;

    ptr = PSX_SCRATCH;

    if (g_SysWork.playerWork.player.position.vx > Q12(-80.0f))
    {
        return false;
    }

    if (g_SysWork.playerWork.player.position.vz < Q12(0.0f))
    {
        return false;
    }

    if (WorldEnv_IsFlashlightAllowed() && WorldEnv_IsInDarkness())
    {
        return false;
    }

    if (sharedData_800DFB7C_0_s00[idx].field_B != 0)
    {
        sharedData_800DFB7C_0_s00[idx].field_C.s_2.field_0 += Rng_GenerateUInt(-32, 32 - 1);
        sharedData_800DFB7C_0_s00[idx].field_C.s_2.field_0  = CLAMP_MIN_THEN_LOW(sharedData_800DFB7C_0_s00[idx].field_C.s_2.field_0, 0, 0xFFF);

        ptr->field_15C = Q12_MULT_PRECISE(sharedData_800DFB7C_0_s00[idx].field_C.s_2.field_0, Q12(1.5f));

        setPolyFT4(*poly);

        Math_SetSVectorFastSum(&ptr->field_12C[0],
                               Q12_TO_Q8(sharedData_800DFB7C_0_s00[idx].field_0.vx_0 - ptr->field_15C) - (u16)ptr->field_0.field_0.vx,
                               Q12_TO_Q8(sharedData_800DFB7C_0_s00[idx].vy_8) - ptr->field_0.field_0.vy,
                               Q12_TO_Q8(sharedData_800DFB7C_0_s00[idx].field_4.vz_4 + ptr->field_15C) - ptr->field_0.field_0.vz);

        Math_SetSVectorFastSum(&ptr->field_12C[1],
                               Q12_TO_Q8(sharedData_800DFB7C_0_s00[idx].field_0.vx_0 + ptr->field_15C) - (u16)ptr->field_0.field_0.vx,
                               Q12_TO_Q8(sharedData_800DFB7C_0_s00[idx].vy_8) - ptr->field_0.field_0.vy,
                               Q12_TO_Q8(sharedData_800DFB7C_0_s00[idx].field_4.vz_4 + ptr->field_15C) - ptr->field_0.field_0.vz);

        Math_SetSVectorFastSum(&ptr->field_12C[2],
                               Q12_TO_Q8(sharedData_800DFB7C_0_s00[idx].field_0.vx_0 - ptr->field_15C) - (u16)ptr->field_0.field_0.vx,
                               Q12_TO_Q8(sharedData_800DFB7C_0_s00[idx].vy_8) - ptr->field_0.field_0.vy,
                               Q12_TO_Q8(sharedData_800DFB7C_0_s00[idx].field_4.vz_4 - ptr->field_15C) - ptr->field_0.field_0.vz);

        gte_ldv3c(&ptr->field_12C);
        gte_rtpt();
        gte_stsxy3_g3(*poly);
        gte_stsz3c(&ptr->field_148);

        Math_SetSVectorFastSum(&ptr->field_12C[0],
                               Q12_TO_Q8(sharedData_800DFB7C_0_s00[idx].field_0.vx_0 + ptr->field_15C) - (u16)ptr->field_0.field_0.vx,
                               Q12_TO_Q8(sharedData_800DFB7C_0_s00[idx].vy_8) - ptr->field_0.field_0.vy,
                               Q12_TO_Q8(sharedData_800DFB7C_0_s00[idx].field_4.vz_4 - ptr->field_15C) - ptr->field_0.field_0.vz);

        gte_ldv0(&ptr->field_12C);
        gte_rtps();
        gte_stsxy(&ptr->field_144);
        gte_stsz(&ptr->field_154);

        ptr->field_148.vx = (ptr->field_148.vx + ptr->field_148.vy + ptr->field_148.vz + ptr->field_154) >> 2;

        if (ptr->field_148.vx <= 0 || (ptr->field_148.vx >> 3) >= ORDERING_TABLE_SIZE)
        {
            return false;
        }

        if (ABS(ptr->field_144.vx) > 200 || ABS(ptr->field_144.vy) > 160)
        {
            return false;
        }

        *(s32*)&(*poly)->x3 = *(s32*)&ptr->field_144;
        setSemiTrans(*poly, 1);

        if (CLAMP_LOW(Q12_MULT_PRECISE(Q12(1.0f) - sharedData_800DFB7C_0_s00[idx].field_C.s_2.field_0, 32), 0) < 16)
        {
            var_v0_4 = MAX(Q12_MULT_PRECISE(Q12(1.0f) - sharedData_800DFB7C_0_s00[idx].field_C.s_2.field_0, 32), 0);
        }
        else
        {
            var_v0_4 = 16;
        }

        ptr->field_160 = var_v0_4;

        setUV0AndClutSum(*poly, 224, 160 + (sharedData_800DFB7C_0_s00[idx].field_C.s_1.field_2 << 5), 19);
        setUV1AndTPageSum(*poly, 255, 160 + (sharedData_800DFB7C_0_s00[idx].field_C.s_1.field_2 << 5), 75);
        setUV2Sum(*poly, 224, 191 + (sharedData_800DFB7C_0_s00[idx].field_C.s_1.field_2 << 5));
        setUV3Sum(*poly, 255, 191 + (sharedData_800DFB7C_0_s00[idx].field_C.s_1.field_2 << 5));

        setRGB0Fast(*poly, ptr->field_160, ptr->field_160, ptr->field_160);

        addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[(ptr->field_148.vx + 128) >> 3], *poly);
        *poly += 1;
    }
    else
    {
        for (i = 0; i < 25; i += 3)
        {
            for (j = 0; j < 3; j++)
            {
                Math_SetSVectorFastSum(&ptr->field_12C[j],
                                       Q12_TO_Q8(((((i + j) % 5) * Q12(1.5f)) >> 1) - Q12(93.5f)) - (u16)ptr->field_0.field_0.vx,
                                       Q8(0.5f) - ptr->field_0.field_0.vy,
                                       Q12_TO_Q8(Q8(153.6f) - ((i + j) / 5 * Q12(1.5f) >> 1)) - ptr->field_0.field_0.vz);
            }

            gte_ldv3c(&ptr->field_12C[0]);
            gte_rtpt();
            gte_stsxy3c(&sp10[i / 5][i % 5]);
            gte_stsz3c(&sp78[i / 5][i % 5]);
        }

        Math_SetSVectorFastSum(&ptr->field_12C[0],
                               Q8(-90.5f) - (u16)ptr->field_0.field_0.vx,
                               Q8(0.5f) - ptr->field_0.field_0.vy,
                               Q8(6.6f) - ptr->field_0.field_0.vz);

        gte_ldv0(&ptr->field_12C);
        gte_rtps();
        gte_stsxy(&sp10[4][4]);
        gte_stsz(&sp78[4][4]);

        ptr->field_160 = !(func_80055D78(Q12(-92.0f), Q12(0.5f), Q12(8.1f)) < 129) * 8;

        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 4; j++)
            {
                if (sp78[i][j] <= 0 || (sp78[i][j] >> 3) >= ORDERING_TABLE_SIZE)
                {
                    continue;
                }

                if (ABS(sp10[i][j].vx) > 200 || ABS(sp10[i][j].vy) > 160)
                {
                    continue;
                }

                setPolyFT4(*poly);

                setXY4(*poly,
                       sp10[i][j].vx, sp10[i][j].vy,
                       sp10[i][j + 1].vx, sp10[i][j + 1].vy,
                       sp10[i + 1][j].vx, sp10[i + 1][j].vy,
                       sp10[i + 1][j + 1].vx, sp10[i + 1][j + 1].vy);

                setSemiTrans(*poly, 1);

                *(u16*)&(*poly)->r0 = ptr->field_160;
                (*poly)->b0         = 0;

                setUV0AndClutSum(*poly, 224 + (j * 8), (i * 8) + 160 + (sharedData_800DFB7C_0_s00[idx].field_C.s_1.field_2 << 5), 0x0293);
                setUV1AndTPageSum(*poly, 231 + (j * 8), (i * 8) + 160 + (sharedData_800DFB7C_0_s00[idx].field_C.s_1.field_2 << 5), 0x2B);
                setUV2Sum(*poly, 224 + (j * 8), (i * 8) + 160 + (sharedData_800DFB7C_0_s00[idx].field_C.s_1.field_2 << 5) + 7);
                setUV3Sum(*poly, 231 + (j * 8), (i * 8) + 160 + (sharedData_800DFB7C_0_s00[idx].field_C.s_1.field_2 << 5) + 7);

                ptr->field_158 = (sp78[i][j] + sp78[i + 1][j] + sp78[i][j + 1] + sp78[i + 1][j + 1]) >> 2;

                addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[(ptr->field_158 + 128) >> 3], *poly);
                *poly += 1;
            }
        }
    }

    return true;
}

#include "../src/maps/common/particles/particle.c"

#include "../src/maps/common/characters/player.c"

#include "../src/maps/common/characters/hanged_scratcher.c" // 0x800CFC24
#include "../src/maps/common/characters/creeper.c" // 0x800D387C

void func_800D5B00(void) // 0x800D5B00
{
    q19_12               angle;
    q19_12               cosAngle;
    q19_12               randAngle;
    q19_12               sinAngle;
    q19_12               temp_lo;
    s32                  i;
    s_func_800D5B00_D94* ptr;
    s_func_800D5B00*     base;

    base = (s_func_800D5B00*)FS_BUFFER_1;
    ptr  = base->field_D94;

    for (i = 0; i < 200; i++, ptr++)
    {
        ptr->field_34   = Rng_GenerateInt(0, 4914);
        ptr->field_0.vx = 0;
        ptr->field_0.vy = 0;
        ptr->field_0.vz = 0;
        ptr->field_30   = 255;

        angle = Rng_Rand16();

        if (Math_Sin(angle) < Q12(0.0f))
        {
            sinAngle = Math_Sin(angle);
        }
        else
        {
            sinAngle = -Math_Sin(angle);
        }

        cosAngle   = Math_Cos(angle);
        randAngle = Rng_Rand16();

        angle = Q12_MULT(Math_Sin(randAngle), cosAngle);
        temp_lo = Q12_MULT(Math_Cos(randAngle), cosAngle);

        ptr->field_20.vx = Q12_MULT_PRECISE(angle, Q12(1.8f));
        ptr->field_20.vy = Q12_MULT_PRECISE(sinAngle, Q12(4.4f));
        ptr->field_20.vz = Q12_MULT_PRECISE(temp_lo, Q12(1.8f));
    }
}

void func_800D5CC4(s32 x, s32 y, s32 val) // 0x800D5CC4
{
    s32              offsetX;
    s32              offsetY;
    s16              row;
    s16              col;
    s_func_800D5B00* ptr;
    u8*              buf;

    ptr = FS_BUFFER_1;

    offsetX = ptr->field_D84 & 0xFFFF;
    offsetY = ptr->field_D84 >> 16;

    col = (x + 41) - offsetX;
    row = (y + 81) - offsetY;

    col += (col > 0) ? 1 : -1;
    col /= 2;

    row += (row > 0) ? 1 : -1;
    row /= 2;

    if (col > 0 && col < 41 && row > 0 && row < 81)
    {
        buf  = ptr->field_5D + (col + (41 * row));
        *buf = val;
    }
}

void func_800D5D90(void) // 0x800D5D90
{
    SVECTOR              sp10;
    DVECTOR              sp18[2];
    s32                  sp20;
    s32                  i;
    s_func_800D5B00_D94* ptr;
    s_func_800D5B00*     base;

    base = (s_func_800D5B00*)FS_BUFFER_1;
    ptr  = base->field_D94;

    for (i = 0; i < 200; i++, ptr++)
    {
        if (ptr->field_34 < 0)
        {
            sp10.vx = Q12_TO_Q4(ptr->field_0.vx);
            sp10.vy = Q12_TO_Q4(ptr->field_0.vy);
            sp10.vz = Q12_TO_Q4(ptr->field_0.vz);

            RotTransPers(&sp10, &sp18[0], &sp18[1], &sp20);
            func_800D5CC4(sp18[0].vx, sp18[0].vy, ptr->field_30);

            ptr->field_0.vx += ptr->field_20.vx;
            ptr->field_0.vy  = (ptr->field_0.vy + ptr->field_20.vy) + Q12(1.2f);
            ptr->field_0.vz += ptr->field_20.vz;
            ptr->field_30    = ptr->field_30 - 20;
            ptr->field_30    = MAX(ptr->field_30, 0);
        }
        else
        {
            ptr->field_34 -= g_DeltaTime;
        }
    }
}

s32 func_800D5EA8(s32 arg0, s32 arg1) // 0x800D5EA8
{
    s_func_800D5B00* buf;

    buf = (s_func_800D5B00*)FS_BUFFER_1;

    return D_800DA154[buf->field_5D[(arg1 * 41) + arg0]];
}

void func_800D5EE8(void) // 0x800D5EE8
{
    // TODO: Messy decl order matters.
    u32              sp18;
    s_func_800D5B00* ptr;
    GsOT_TAG*        ot;
    s32              x;
    s32              y;
    s32              col0;
    s32              col2;
    s32              col3;
    s32              i;
    s32              j;
    s32              col1;
    POLY_G4*         poly;
    DR_MODE*         mode;
    PACKET*          packet;
    s32              col;
    s32              idx;

    ptr = (s_func_800D5B00*)FS_BUFFER_1;

    packet = GsOUT_PACKET_P;
    poly   = packet;

    x = (ptr->field_D84 & 0xFFFF) - 41;
    y = ((u32)ptr->field_D84 >> 16) - 81;

    idx = g_ActiveBufferIdx;
    ot    = g_OrderingTable0[idx].org;
    ot    = &ot[ptr->field_D88 >> 1];

    col = 0x3A000000;

    for (i = 1; i < 81; i++)
    {
        col1 = func_800D5EA8(0, i - 1);
        col3 = func_800D5EA8(0, i);

        for (j = 1; j < 41; j++)
        {
            sp18 = ptr->field_5D[(i * 41) + j];

            col0 = col1;
            col2 = col3;
            col1 = func_800D5EA8(j, i - 1);

            col3 = func_800D5EA8(j, i);

            if (col  == col0 &&
                col0 == col1 &&
                col0 == col2 &&
                col2 == col3)
            {
                continue;
            }

            if ((j + i) & 0x1)
            {
                poly->x0 = x + (j * 2);
                poly->y0 = y + (i * 2);
                poly->x1 = (x + (j * 2)) + 2;
                poly->y1 = y + (i * 2);
                poly->x2 = x + (j * 2);
                poly->y2 = (y + (i * 2)) + 2;
                poly->x3 = (x + (j * 2)) + 2;
                poly->y3 = (y + (i * 2)) + 2;

                *(s32*)&poly->r0 = col0;
                *(s32*)&poly->r1 = col1;
                *(s32*)&poly->r2 = col2;
                *(s32*)&poly->r3 = col3;
            }
            else
            {
                poly->x1 = x + (j * 2);
                poly->y1 = y + (i * 2);
                poly->x0 = (x + (j * 2)) + 2;
                poly->y0 = y + (i * 2);
                poly->x3 = x + (j * 2);
                poly->y3 = (y + (i * 2)) + 2;
                poly->x2 = (x + (j * 2)) + 2;
                poly->y2 = (y + (i * 2)) + 2;

                *(s32*)&poly->r0 = col1;
                *(s32*)&poly->r1 = col0;
                *(s32*)&poly->r2 = col3;
                *(s32*)&poly->r3 = col2;
            }

            setPolyG4(poly);
            
            // @hack Identical arms needed for match.
            if (sp18 != 0)
            {
                setSemiTrans(poly, 1);
            }
            else
            {
                setSemiTrans(poly, 1);
            }

            addPrim(ot, poly);
            poly++;
        }
    }

    packet = poly;
    mode   = packet;

    SetDrawMode(mode, 0, 1, 0x2A, NULL);
    addPrim(ot, mode);
    packet         = mode + 1;
    GsOUT_PACKET_P = packet;
}

void func_800D61D4(void) // 0x800D61D4
{
    s32              i;
    s32              j;
    s32              val;
    s_func_800D5B00* buf;
    u8*              tab;

    buf = FS_BUFFER_1;

    for (i = 0; i < 81; i++)
    {
        tab = &buf->field_5D[i * 41];

        for (j = 0; j < 41; j++)
        {
            val = tab[j - 41];

            if (j == 0)
            {
                val = 0;
            }
            else
            {
                val += tab[j - 1];
            }

            if (j == 40)
            {
                val = 0;
            }
            else
            {
                val = val + tab[j + 1];
            }

            val  += tab[j + 41];
            val >>= 2;
            val  -= 3;

            if (val <= 0)
            {
                tab[j] = 0;
            }
            else
            {
                tab[j] = val;
            }

            if (!Rng_GenerateUInt(0, 4095)) // 1 in 4096 chance.
            {
                tab[j] = 0;
            }
        }
    }
}

void func_800D62C8(void)
{
    s_func_800D5B00* buf = FS_BUFFER_1;

    memset(buf->field_34, 0, sizeof(buf->field_34));
    memset(buf->field_5D, 0, sizeof(buf->field_5D));
    memset(buf->field_D56, 0, sizeof(buf->field_D56));
}

void func_800D631C(VECTOR* arg0, q19_12 rotY) // 0x800D631C
{
    MATRIX  mat;
    VECTOR  sp30;
    SVECTOR rot; // Q3.12

    SetRotMatrix(&GsWSMATRIX);
    SetTransMatrix(&GsWSMATRIX);
    ApplyRotMatrixLV(arg0, &sp30);

    sp30.vx += GsWSMATRIX.t[0];
    sp30.vy += GsWSMATRIX.t[1];
    sp30.vz += GsWSMATRIX.t[2];

    TransMatrix(&mat, &sp30);
    SetTransMatrix(&mat);

    rot.vx = Q12_ANGLE(0.0f);
    rot.vy = rotY;
    rot.vz = Q12_ANGLE(0.0f);

    Math_RotMatrixZxyNeg(&rot, &mat);
    SetMulRotMatrix(&mat);
}

s32 func_800D63DC(s32* screenXy) // 0x800D63DC
{
    SVECTOR vec;
    s32     p;

    vec.vx = 0;
    vec.vy = 0;
    vec.vz = 0;
    return RotTransPers(&vec, screenXy, &p, &p);
}

void func_800D6414(void) // 0x800D6414
{
    s_func_800D5B00* buf = FS_BUFFER_1;

    if (buf->field_D90 > 0)
    {
        buf->field_D90 -= g_DeltaTime;
        return;
    }

    func_800D631C(&buf->field_4, buf->field_D8C);
    buf->field_D88 = func_800D63DC(&buf->field_D84);

    func_800D5D90();
    func_800D61D4();
    func_800D5EE8();
}

void func_800D6490(VECTOR3* arg0) // 0x800D6490
{
    s_func_800D5B00* buf = FS_BUFFER_1;

    buf->field_D8C  = 0;
    buf->field_D90  = 9830;
    buf->field_4.vx = Q12_TO_Q8(arg0->vx);
    buf->field_4.vy = Q12_TO_Q8(arg0->vy);
    buf->field_4.vz = Q12_TO_Q8(arg0->vz);

    func_800D62C8();
    func_800D5B00();
}

#include "../src/maps/common/map_util.c" // 0x800D64F8

void func_800D6790(void) // 0x800D6790
{
    u32 bgmFlags = D_800DA578[g_SavegamePtr->mapRoomIdx];

    if (Savegame_EventFlagGet(EventFlag_354))
    {
        bgmFlags |= BgmFlag_Layer6 | BgmFlag_Layer4 | BgmFlag_Layer2;
    }

    Bgm_LayersUpdate(bgmFlags, Q12(0.25f), &D_800DA570);
}

void GameBoot_LoadScreen_StageString(void) {}

void func_800D67F4(void) // 0x800D67F4
{
    VECTOR3 pos = { MAP_POINTS[g_MapEventData->mapPointIdx].positionX, Q12(-1.2f), MAP_POINTS[g_MapEventData->mapPointIdx].positionZ };

    Event_DisplayMapMsgWithSfx(MapMsgIdx_DoorJammed, Sfx_Unk1576, &pos);
}

void func_800D6888(void) // 0x800D6888
{
    VECTOR3 pos = { MAP_POINTS[g_MapEventData->mapPointIdx].positionX, Q12(-1.2f), MAP_POINTS[g_MapEventData->mapPointIdx].positionZ };

    Event_DisplayMapMsgWithSfx(MapMsgIdx_DoorLocked, Sfx_Unk1576, &pos);
}

const char* MAP_MESSAGES[] = {
    #include "maps/common/shared/map_msg_common.h"
#if VERSION_REGION_IS(NTSC)
    /* 15 */ "\tUsed_the_ ~C2 Sewer_key ~C7 . ~E ",
    /* 16 */ "\tUsed_the_ ~C2 Sewer_exit_key ~C7 . ~E ",
    /* 17 */ "\tThere_is_a_ ~C2 Sewer_key ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 18 */ "\tThere_is_a_ ~C2 Sewer_exit_key ~C7 ~N\n\tin_the_water._Take_it? ~S4 ",
    /* 19 */ "\tGot_to_find_Cheryl. ~N\n\tNo_time_to_go_back. ~E ",
    /* 20 */ "~H\tThere_is_a_ ~C5 Sewer_map ~N\n\t(connecting_to_resort_area) ~C7 . ~N\n\tTake_it? ~S4 "
#elif VERSION_REGION_IS(NTSCJ)
    /* 15 */ "\t~C2 下水道通路の鍵 ~C7 を使った ~E ",
    /* 16 */ "\t~C2 下水道出口の鍵 ~C7 を使った ~E ",
    /* 17 */ "\t~C2 下水道通路の鍵 ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 18 */ "\t水の中に ~C2 下水道出口の鍵 ~C7 が落ちている ~N\n\t取りますか？ ~S4 ",
    /* 19 */ "\tシェリルを探しに行こう ~N\n\t戻っている暇はない ~E ",
    /* 20 */ "~H ~C5 下水道の地図（リゾート方面） ~C7 がある ~N\n\t取りますか？ ~S4 "
#endif
};

void MapEvent_CommonItemTake(void) // 0x800D691C
{
    u32 pickupType;
    s32 eventFlagIdx;

    pickupType   = CommonPickupItemId_FirstAidKit;
    eventFlagIdx = 0;

    switch (g_MapEventData->mapPointIdx)
    {
        case 38:
            pickupType   = CommonPickupItemId_ShotgunShells;
            eventFlagIdx = EventFlag_M5S00_ShotgunShells;
            break;

        case 39:
            pickupType   = CommonPickupItemId_HealthDrink;
            eventFlagIdx = EventFlag_M5S00_HealthDrink0;
            break;

        case 40:
            pickupType   = CommonPickupItemId_HandgunBullets;
            eventFlagIdx = EventFlag_M5S00_HandgunBullets0;
            break;

        case 41:
            pickupType   = CommonPickupItemId_RifleShells;
            eventFlagIdx = EventFlag_M5S00_RifleShells;
            break;

        case 42:
            pickupType   = CommonPickupItemId_FirstAidKit;
            eventFlagIdx = EventFlag_M5S00_FirstAidKit0;
            break;

        case 43:
            pickupType   = CommonPickupItemId_FirstAidKit;
            eventFlagIdx = EventFlag_M5S00_FirstAidKit1;
            break;

        case 44:
            pickupType   = CommonPickupItemId_HealthDrink;
            eventFlagIdx = EventFlag_M5S00_HealthDrink1;
            break;

        case 45:
            pickupType   = CommonPickupItemId_HandgunBullets;
            eventFlagIdx = EventFlag_M5S00_HandgunBullets1;
            break;
    }

    Event_CommonItemTake(pickupType, eventFlagIdx);
}

void func_800D69DC(void) // 0x800D69DC
{
    VECTOR3 pos = { MAP_POINTS[g_MapEventData->mapPointIdx].positionX, Q12(-1.2f), MAP_POINTS[g_MapEventData->mapPointIdx].positionZ };

    switch (g_MapEventData->mapPointIdx)
    {
        case 10:
            Player_ItemRemove(InvItemId_SewerKey, 1);
            Event_DisplayMapMsgWithSfx(15, Sfx_UseKey, &pos);
            break;

        case 14:
            Player_ItemRemove(InvItemId_SewerExitKey, 1);
            Event_DisplayMapMsgWithSfx(16, Sfx_UseKey, &pos);
            break;

        default:
            Event_DisplayMapMsgWithSfx(MapMsgIdx_DoorUnlocked, Sfx_DoorUnlocked, &pos);
            break;
    }
}

void MapEvent_SewerKeyTake(void) // 0x800D6AD4
{
    Event_ItemTake(InvItemId_SewerKey, DEFAULT_PICKUP_ITEM_COUNT, EventFlag_M5S00_PickupSewerKey, 17);
}

void func_800D6B00(void) // 0x800D6B00
{
    VECTOR3 vec;
    s32     i;

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            D_800DAB78 = Q12(-0.5f);
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            Event_CharaAnimPlayToEnd(&g_SysWork.playerWork.player, 59);
            break;

        case 3:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 0, Q12(0.0f), false);
            SysWork_StateStepIncrement(0);

        case 4:
            Event_ScreenFadeCmd(ScreenFadeCmd_Wait, true, 0, Q12(0.0f), false);
            break;

        case 5:
            g_SysWork.bgmStatusFlags |= BgmStatusFlag_Pause;
            Event_BgTextureCmd(BgTextureCmd_Auto, FILE_TIM_DRAINKEY_TIM, false);
            break;

        case 6:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            SysWork_StateStepIncrement(0);

        case 7:
            SysWork_StateStepIncrement(0);

        case 8:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Wait, false, 0, Q12(0.0f), false);
            break;

        case 9:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            g_SysWork.silentYesSelection = true;
            Event_DisplayMapMsg(true, 18, 10, 11, 0, false);
            break;

        case 10:
            Savegame_EventFlagSet(EventFlag_354);

            Event_InvItemCmd(InvItemCmd_AddItem, InvItemId_SewerExitKey, 1, false);

            // Warp player.
            g_SysWork.playerWork.player.position.vx = Q12(-92.0f);
            g_SysWork.playerWork.player.position.vz = Q12(7.7f);
            g_SysWork.playerWork.player.rotation.vy = Q12_ANGLE(0.0f);

            for (i = 0; i < ARRAY_SIZE(D_800DAB7C); i++)
            {
                D_800DAB7C[i] = Chara_Spawn(Chara_HangedScratcher, i, Q12(-93.3f), Q12(7.1f), Q12_ANGLE(180.0f), 18 + i);
            }

            if (g_SavegamePtr->gameDifficulty != GameDifficulty_Easy)
            {
                Chara_Spawn(Chara_HangedScratcher, 14, Q12(-94.3f), Q12(-5.5f), Q12_ANGLE(0.0f), 7u);
            }

            CutsceneBorder_ForceShow();

            Sd_SfxPlay(Sfx_Unk1575, Q7(0.0f), Q7(1.0f));
            SysWork_StateStepIncrement(0);

        case 11:
            Event_BgTextureFadeOut(0, 0);
            break;

        case 12:
            Event_SysStateBranchOnFlag(EventFlag_354, 13, 16, false);
            break;

        case 13:
            // Warp camera.
            Event_CameraPositionSet(NULL, Q12(-88.04f), Q12(-1.29f), Q12(6.26f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(NULL, Q12(-91.77f), Q12(-0.89f), Q12(7.63f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);

            g_SysWork.lightIntensity = Q12(2.0f);

            SysWork_StateStepIncrement(0);

            vec = Q12_VECTOR3(-91.3f, 0.3f, 8.5f);
            func_800D6490(&vec);

        case 14:
            if (D_800DAB78 < Q12(1.5f) && (D_800DAB78 + g_DeltaTime) > Q12(1.5f))
            {
                Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 114, false);
            }

            D_800DAB78 += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.6f);

            if (D_800DAB78 > Q12(3.55f))
            {
                SysWork_StateStepIncrement(0);
            }
            else if (D_800DAB78 > Q12(2.5f))
            {
                g_SysWork.npcs[D_800DAB7C[2]].properties.dummy.properties_E8[0].val8[0] |= (1 << 7);
            }
            else if (D_800DAB78 > Q12(1.25f))
            {
                g_SysWork.npcs[D_800DAB7C[1]].properties.dummy.properties_E8[0].val8[0] |= (1 << 7);
            }
            else if (D_800DAB78 > Q12(-0.1f))
            {
                g_SysWork.npcs[D_800DAB7C[0]].properties.dummy.properties_E8[0].val8[0] |= (1 << 7);
            }

            func_800D6414();
            break;

        case 15:
            vcReturnPreAutoCamWork(true);
            g_SysWork.lightIntensity = Q12(1.0f);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);

            g_SysWork.npcs[D_800DAB7C[0]].model.controlState     = 0;
            g_SysWork.npcs[D_800DAB7C[0]].model.stateStep = 17;
            g_SysWork.npcs[D_800DAB7C[0]].position.vx     += Q12(-0.1878f);
            g_SysWork.npcs[D_800DAB7C[0]].position.vz     += Q12(0.245f);
            g_SysWork.npcs[D_800DAB7C[0]].rotation.vy      = Q12_ANGLE(112.5f);

            g_SysWork.npcs[D_800DAB7C[1]].model.controlState     = 0;
            g_SysWork.npcs[D_800DAB7C[1]].model.stateStep = 17;
            g_SysWork.npcs[D_800DAB7C[1]].position.vx     += Q12(1.8128f);
            g_SysWork.npcs[D_800DAB7C[1]].position.vz     += Q12(0.799f);
            g_SysWork.npcs[D_800DAB7C[1]].rotation.vy      = Q12_ANGLE(180.0f);

            g_SysWork.npcs[D_800DAB7C[2]].model.controlState     = 0;
            g_SysWork.npcs[D_800DAB7C[2]].model.stateStep = 17;
            g_SysWork.npcs[D_800DAB7C[2]].position.vx     += Q12(0.6531f);
            g_SysWork.npcs[D_800DAB7C[2]].position.vz     += Q12(-1.2493f);
            g_SysWork.npcs[D_800DAB7C[2]].rotation.vy      = Q12_ANGLE(0.0f);

            g_SysWork.playerWork.player.position.vz += Q12(-1.2f);

            // Return to gameplay.
            Player_ControlUnfreeze(true);
            SysWork_StateSetNext(SysState_Gameplay);
            break;

        case 16:
            Event_CharaAnimPlayToEnd(&g_SysWork.playerWork.player, 60);
            break;

        default:
            // Return to gameplay.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;
    }
}

void func_800D732C(void) // 0x800D732C
{
    // Skip.
    if (Pad_ButtonClickedBind(skip) &&
        g_SysWork.sysStateSteps[0] > 0 && g_SysWork.sysStateSteps[0] < 3)
    {
        Screen_FadeTimestepReset();
        SysWork_StateStepReset();
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            // Warp player.
            Player_ControlFreeze();
            g_SysWork.playerWork.player.position.vx = Q12(56.789f);
            g_SysWork.playerWork.player.position.vy = Q12(-2.02f);
            g_SysWork.playerWork.player.position.vz = Q12(60.02f);
            g_SysWork.playerWork.player.rotation.vy = Q12_ANGLE(-90.0f);
            Game_TurnFlashlightOn();

            // Warp camera.
            Event_CameraPositionSet(NULL, Q12(58.49f), Q12(1.18f), Q12(59.07f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(NULL, Q12(56.67f), Q12(-2.3f), Q12(59.86f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 88, false);

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(1.5f), false);
            SysWork_StateStepIncrement(0);

        case 1:
            // Move player.
            g_SysWork.playerWork.player.position.vy += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.4f);

            Event_WaitTimer(Q12(3.8f), false);
            break;

        case 2:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(1.5f), false);

            // Move player.
            g_SysWork.playerWork.player.position.vy += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.4f);
            break;

        default:
            // Return to gameplay.
            Player_ControlUnfreeze(true);
            SysWork_StateSetNext(SysState_Gameplay);

            // Warp player.
            g_SysWork.playerWork.player.position.vx = Q12(16.7f);
            g_SysWork.playerWork.player.position.vy = Q12(0.0f);
            g_SysWork.playerWork.player.position.vz = Q12(52.0f);
            g_SysWork.playerWork.player.rotation.vy = Q12_ANGLE(90.0f);

            vcReturnPreAutoCamWork(true);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            Savegame_EventFlagSet(EventFlag_355);
            func_8003A16C();
            break;
    }
}

void func_800D75FC(void) // 0x800D75FC
{
    // Skip.
    if (Pad_ButtonClickedBind(skip) &&
        g_SysWork.sysStateSteps[0] > 0 && g_SysWork.sysStateSteps[0] < 6)
    {
        Screen_FadeTimestepReset();
        SysWork_StateStepReset();
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 0, Q12(0.0f), false);
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            Event_ScreenFadeCmd(ScreenFadeCmd_Wait, true, 0, Q12(0.0f), false);
            if (g_SysWork.sysStateSteps[0] == 2)
            {
                g_SysWork.sysStateStepData[0] += g_DeltaTimeRaw;
                if (g_SysWork.sysStateStepData[0] > Q12(1.0f))
                {
                    SysWork_StateStepSet(0, 3);
                }
            }
            break;

        case 3:
            // Warp player.
            g_SysWork.playerWork.player.position.vx = Q12(56.773f);
            g_SysWork.playerWork.player.position.vy = Q12(-1.444f);
            g_SysWork.playerWork.player.position.vz = Q12(60.036f);
            g_SysWork.playerWork.player.rotation.vy = Q12_ANGLE(-90.0f);
            Game_TurnFlashlightOn();

            // Warp camera.
            Event_CameraPositionSet(NULL, Q12(57.29f), Q12(-0.86f), Q12(59.36f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(NULL, Q12(55.07f), Q12(-4.0f), Q12(60.29f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 87, false);

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(1.5f), false);
            SysWork_StateStepIncrement(0);

        case 4:
            g_SysWork.playerWork.player.position.vy += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, -0.4f);
            Event_WaitTimer(Q12(3.8f), false);
            break;

        case 5:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(1.5f), false);
            g_SysWork.playerWork.player.position.vy += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, -0.4f);
            break;

        default:
            // Return to gameplay.
            Player_ControlUnfreeze(true);
            SysWork_StateSetNext(SysState_Gameplay);

            Savegame_EventFlagSet(EventFlag_356);
            Savegame_EventFlagClear(EventFlag_357);
            break;
    }
}

void func_800D7940(void) // 0x800D7940
{
    // Skip.
    if (Pad_ButtonClickedBind(skip) &&
        g_SysWork.sysStateSteps[0] > 0 && g_SysWork.sysStateSteps[0] < 6)
    {
        Screen_FadeTimestepReset();
        SysWork_StateStepReset();
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 0, Q12(0.0f), false);
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            Event_ScreenFadeCmd(ScreenFadeCmd_Wait, true, 0, Q12(0.0f), false);

            if (g_SysWork.sysStateSteps[0] == 2)
            {
                g_SysWork.sysStateStepData[0] += g_DeltaTimeRaw;
                if (g_SysWork.sysStateStepData[0] > Q12(1.0f))
                {
                    SysWork_StateStepSet(0, 3);
                }
            }
            break;

        case 3:
            // Warp player.
            g_SysWork.playerWork.player.position.vx = Q12(56.769f);
            g_SysWork.playerWork.player.position.vy = Q12(-2.07f);
            g_SysWork.playerWork.player.position.vz = Q12(60.012f);
            g_SysWork.playerWork.player.rotation.vy = Q12_ANGLE(-90.0f);
            Game_TurnFlashlightOn();

            // Warp camera.
            Event_CameraPositionSet(NULL, Q12(57.09f), Q12(-5.76f), Q12(60.59f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(NULL, Q12(55.96f), Q12(-2.01f), Q12(59.78f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 88, false);

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(1.5f), false);
            SysWork_StateStepIncrement(0);

        case 4:
            g_SysWork.playerWork.player.position.vy += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.4f);
            Event_WaitTimer(Q12(3.8f), false);
            break;

        case 5:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(1.5f), false);
            g_SysWork.playerWork.player.position.vy += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.4f);
            break;

        default:
            Player_ControlUnfreeze(true);
            SysWork_StateSetNext(SysState_Gameplay);
            Savegame_EventFlagSet(EventFlag_357);
            Savegame_EventFlagClear(EventFlag_356);
            break;
    }
}

void func_800D7C84(void) // 0x800D7C84
{
    // Skip.
    if (Pad_ButtonClickedBind(skip) &&
        g_SysWork.sysStateSteps[0] > 0 && g_SysWork.sysStateSteps[0] < 6)
    {
        Screen_FadeTimestepReset();
        SysWork_StateStepReset();
    }

    g_SysWork.bgmStatusFlags |= BgmStatusFlag_ApplyMute;

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 0, Q12(0.0f), false);
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            Event_ScreenFadeCmd(ScreenFadeCmd_Wait, true, 0, Q12(0.0f), false);
            break;

        case 3:
            // Warp player.
            g_SysWork.playerWork.player.position.vx = Q12(-22.234f);
            g_SysWork.playerWork.player.position.vy = Q12(-1.734f);
            g_SysWork.playerWork.player.position.vz = Q12(60.508f);
            g_SysWork.playerWork.player.rotation.vy = Q12_ANGLE(-90.0f);
            Game_TurnFlashlightOn();

            // Warp camera.
            Event_CameraPositionSet(NULL, Q12(-20.39f), Q12(-3.84f), Q12(63.99f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(NULL, Q12(-22.56f), Q12(-2.83f), Q12(60.78f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 87, false);

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(1.5f), false);
            SysWork_StateStepIncrement(0);

        case 4:
            g_SysWork.playerWork.player.position.vy += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, -0.4f);
            Event_WaitTimer(Q12(3.8f), false);
            break;

        case 5:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(1.5f), false);
            g_SysWork.playerWork.player.position.vy += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, -0.4f);
            break;

        default:
            // Return to gameplay.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);

            Savegame_EventFlagSet(EventFlag_358);
            Savegame_EventFlagClear(EventFlag_359);
            break;
    }
}

void func_800D7F88(void) // 0x800D7F88
{
    // Skip.
    if (Pad_ButtonClickedBind(skip) &&
        g_SysWork.sysStateSteps[0] > 0 && g_SysWork.sysStateSteps[0] < 6)
    {
        Screen_FadeTimestepReset();
        SysWork_StateStepReset();
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
            Event_ScreenFadeCmd(ScreenFadeCmd_Wait, true, 0, Q12(0.0f), false);
            break;

        case 3:
            // Warp player.
            g_SysWork.playerWork.player.position.vx = Q12(-22.245f);
            g_SysWork.playerWork.player.position.vy = Q12(-2.439f);
            g_SysWork.playerWork.player.position.vz = Q12(60.488f);
            g_SysWork.playerWork.player.rotation.vy = Q12_ANGLE(-90.0f);
            Game_TurnFlashlightOn();

            // Warp camera.
            Event_CameraPositionSet(NULL, Q12(-21.29f), Q12(-0.04f), Q12(62.13f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
            Event_CameraLookAtSet(NULL, Q12(-22.91f), Q12(-3.23f), Q12(60.34f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 88, false);

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(1.5f), false);
            SysWork_StateStepIncrement(0);

        case 4:
            g_SysWork.playerWork.player.position.vy += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.37f);
            Event_WaitTimer(Q12(3.8f), false);
            break;

        case 5:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(1.5f), false);
            g_SysWork.playerWork.player.position.vy += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.4f);
            break;

        default:
            // Return to gameplay.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);

            Savegame_EventFlagClear(EventFlag_360);
            Savegame_EventFlagClear(EventFlag_358);
            Savegame_EventFlagSet(EventFlag_359);
            break;
    }
}

void MapEvent_PaperMapTake(void) // 0x800D8280
{
    Event_PaperMapTake(13, EventFlag_M5S00_PickupMap, 20);
}

void Map_WorldObjectsInit(void) // 0x800D82A8
{
    WorldObject_Init(&g_WorldObject_Map, "MAP_NEAR", 41.2f, -0.7f, -48.7f, 0.0f, -100.1f, 0.0f);
    func_800CB0D8();

    WorldObject_Init(&g_WorldObject_SavePad0, g_CommonWorldObjectNames[1], 40.503f, -0.709f, -48.7925f, 0.0f, 5.7f, 0.0f);
    WorldObject_Init(&g_WorldObject_SavePad1, g_CommonWorldObjectNames[1], -86.469f, -1.041f, -103.4905f, 0.0f, 206.3f, 0.0f);

    if (g_SavegamePtr->gameDifficulty == GameDifficulty_Easy)
    {
        Chara_SpawnFlagsSet(Chara_Creeper, 0, CharaSpawnFlag_None);
        Chara_SpawnFlagsSet(Chara_Creeper, 1, CharaSpawnFlag_None);
        Chara_SpawnFlagsSet(Chara_Creeper, 4, CharaSpawnFlag_None);
        Chara_SpawnFlagsSet(Chara_Creeper, 7, CharaSpawnFlag_None);
        Chara_SpawnFlagsSet(Chara_Creeper, 12, CharaSpawnFlag_None);

        g_SysWork.npcFlagId = 3;
    }
    else if (g_SavegamePtr->gameDifficulty == GameDifficulty_Hard)
    {
        Chara_SpawnFlagsSet(Chara_Creeper, 5, CharaSpawnFlag_2 | CharaSpawnFlag_3);
        Chara_SpawnFlagsSet(Chara_Creeper, 6, CharaSpawnFlag_2 | CharaSpawnFlag_3);
        Chara_SpawnFlagsSet(Chara_Creeper, 7, CharaSpawnFlag_None);
        Chara_SpawnFlagsSet(Chara_Creeper, 8, CharaSpawnFlag_0 | CharaSpawnFlag_2 | CharaSpawnFlag_3);
        Chara_SpawnFlagsSet(Chara_Creeper, 9, CharaSpawnFlag_0 | CharaSpawnFlag_2 | CharaSpawnFlag_3);

        g_SysWork.npcFlagId = 4;
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

void Map_WorldObjectsUpdate(void) // 0x800D84D8
{
    if (PLAYER_IN_MAP_CHUNK(1, -2))
    {
        if (!Savegame_EventFlagGet(EventFlag_M5S00_PickupMap))
        {
            WorldObjects_Add(&g_WorldObject_Map.object,
                               &g_WorldObject_Map.position,
                               &g_WorldObject_Map.rotation);
        }

        WorldObjects_Add(&g_WorldObject_SavePad0.object,
                           &g_WorldObject_SavePad0.position,
                           &g_WorldObject_SavePad0.rotation);
    }

    if (PLAYER_IN_MAP_CHUNK(-3, -3))
    {
        WorldObjects_Add(&g_WorldObject_SavePad1.object,
                           &g_WorldObject_SavePad1.position,
                           &g_WorldObject_SavePad1.rotation);
    }

    if (PLAYER_IN_MAP_CHUNK(-3, -2))
    {
        if (!Savegame_EventFlagGet(EventFlag_368))
        {
            if (Savegame_EventFlagGet(EventFlag_354) && (g_SysWork.playerWork.player.position.vz < Q12(-66.0f)))
            {
                Sfx_WithFlagsPlay(Sfx_Unk1585, &D_800CB0CC, Q8_CLAMPED(1.0f), SfxFlag_NoDistAtten);
                Savegame_EventFlagSet(EventFlag_368);
            }
        }
    }

    if (PLAYER_IN_MAP_CHUNK(1, 0))
    {
        if (!Savegame_EventFlagGet(EventFlag_M5S00_ShotgunShells))
        {
            WorldObjects_Add(&g_CommonWorldObjects[5], &D_800DAAD0.position, &D_800DAAD0.rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(1, 0))
    {
        if (!Savegame_EventFlagGet(EventFlag_M5S00_HealthDrink0))
        {
            WorldObjects_Add(&g_CommonWorldObjects[1], &D_800DAAE4.position, &D_800DAAE4.rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(1, 0))
    {
        if (!Savegame_EventFlagGet(EventFlag_M5S00_HandgunBullets0))
        {
            WorldObjects_Add(&g_CommonWorldObjects[3], &D_800DAAF8.position, &D_800DAAF8.rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(1, 0))
    {
        if (!Savegame_EventFlagGet(EventFlag_M5S00_RifleShells))
        {
            WorldObjects_Add(&g_CommonWorldObjects[4], &D_800DAB0C.position, &D_800DAB0C.rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(-1, -1))
    {
        if (!Savegame_EventFlagGet(EventFlag_M5S00_FirstAidKit0))
        {
            WorldObjects_Add(g_CommonWorldObjects, &D_800DAB20.position, &D_800DAB20.rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(-2, -1))
    {
        if (!Savegame_EventFlagGet(EventFlag_M5S00_FirstAidKit1))
        {
            WorldObjects_Add(&g_CommonWorldObjects[0], &D_800DAB34.position, &D_800DAB34.rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(-2, -3))
    {
        if (!Savegame_EventFlagGet(EventFlag_M5S00_HealthDrink1))
        {
            WorldObjects_Add(&g_CommonWorldObjects[1], &D_800DAB48.position, &D_800DAB48.rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(-2, -3))
    {
        if (!Savegame_EventFlagGet(EventFlag_M5S00_HandgunBullets1))
        {
            WorldObjects_Add(&g_CommonWorldObjects[3], &D_800DAB5C.position, &D_800DAB5C.rotation);
        }
    }
}

void func_800D8DFC(void) // 0x800D8DFC
{
    if (Savegame_EventFlagGet(EventFlag_354))
    {
        Chara_SpawnFlagsSet(Chara_HangedScratcher, 0, CharaSpawnFlag_1 | CharaSpawnFlag_2);
        Chara_SpawnFlagsSet(Chara_HangedScratcher, 1, CharaSpawnFlag_1 | CharaSpawnFlag_2);
        Chara_SpawnFlagsSet(Chara_HangedScratcher, 2, CharaSpawnFlag_1 | CharaSpawnFlag_2);

        if (g_SavegamePtr->gameDifficulty != GameDifficulty_Easy)
        {
            Chara_SpawnFlagsSet(Chara_HangedScratcher, 11, CharaSpawnFlag_1 | CharaSpawnFlag_2);
        }

        if (Savegame_EventFlagGet(EventFlag_MapMark_527))
        {
            Chara_SpawnPositionSet(Chara_HangedScratcher, 12, Q12(-90.3f), Q12(-92.9f));
        }

        Chara_SpawnFlagsSet(Chara_HangedScratcher, 12, CharaSpawnFlag_1 | CharaSpawnFlag_2);
        Chara_SpawnFlagsSet(Chara_HangedScratcher, 13, CharaSpawnFlag_1 | CharaSpawnFlag_2);
    }

    if (Savegame_EventFlagGet(EventFlag_M5S00_PickupSewerKey))
    {
        Chara_SpawnFlagsSet(Chara_HangedScratcher, 6, CharaSpawnFlag_0 | CharaSpawnFlag_3);
    }
}

const VECTOR3 D_800CB0CC = { Q12(-91.95f), Q12(0.0f), Q12(-82.2f) };
