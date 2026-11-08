#include <memory.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/chara/dms.h"
#include "bodyprog/events/bgm_utils.h"
#include "bodyprog/gfx/map_effects.h"
#include "bodyprog/item_screens.h"
#include "bodyprog/math/math.h"
#include "bodyprog/sys/memcard.h"
#include "bodyprog/player/player.h"
#include "bodyprog/ranking.h"
#include "bodyprog/sound/sound_system.h"
#include "main/rng.h"
#include "maps/map7/map7_s03.h"
#include "maps/common/characters/alessa.h"
#include "maps/common/characters/bloody_incubator.h"
#include "maps/common/characters/bloody_lisa.h"
#include "maps/common/characters/cybil.h"
#include "maps/common/characters/dahlia.h"
#include "maps/common/characters/incubator.h"
#include "maps/common/characters/incubus.h"
#include "maps/common/characters/kaufmann.h"
#include "maps/common/characters/unknown23.h"
#include "screens/stf_roll/credits.h"
#include "maps/common/particle.h"
#include "maps/common/characters/player.h"

#include "../src/maps/common/particles/particle.c"

#include "../src/maps/common/characters/player.c"

#include "../src/maps/common/characters/cybil.c"
#include "../src/maps/common/characters/dahlia.c"
#include "../src/maps/common/characters/alessa.c"
#include "../src/maps/common/characters/bloody_incubator.c"
#include "../src/maps/common/characters/incubator.c"
#include "../src/maps/common/characters/kaufmann.c"
#include "../src/maps/common/characters/bloody_lisa.c"

#include "../src/maps/common/chara_util.c" // 0x800D5040

#include "../src/maps/common/characters/little_incubus.c" // 0x800D5BC8

void func_800D5D24(void) // 0x800D5D24
{
    s32  i;
    s32* ptr;

    ptr = D_800F2418;

    for (i = 5; i >= 0; i--)
    {
        *ptr++ = 0;
    }
}

bool func_800D5D48(void) // 0x800D5D48
{
    s32  var_s0;
    s32  i;
    s32* ptr0;
    s32* ptr1;

    ptr1   = NULL;
    var_s0 = 0;
    ptr0   = D_800F2418;

    // TODO: 6 refers to NPCs array?
    for (i = 0; i < 6; i++, ptr0++)
    {
        if (*ptr0 <= 0)
        {
            ptr1 = ptr0;
        }
        else if (D_800EB008 < *ptr0)
        {
            var_s0    += 2;
            D_800EB008 = (Rng_Rand16() / 21) + Q12(0.8f);
        }
        else if (D_800EB00C < *ptr0)
        {
            var_s0++;
            D_800EB00C = (Rng_Rand16() / 41) + Q12(0.4f);
        }
    }

    if (var_s0 < 2 && ptr1 != NULL)
    {
        *ptr1 = Q12(2.0f);
        return true;
    }
    else
    {
        return false;
    }
}

void func_800D5E78(void) // 0x800D5E78
{
    s32  i;
    s32* ptr;

    ptr = D_800F2418;

    // TODO: 6 refers to NPCs array?
    for (i = 0; i < 6; i++, ptr++)
    {
        if (*ptr > 0)
        {
            *ptr -= g_DeltaTime;
        }
    }
}

void func_800D5EC0(void) // 0x800D5EC0
{
    q19_12               cosAngle;
    q19_12               sinAngle;
    q19_12               randAngle;
    q19_12               dist;
    s32                  i;
    s_func_800D5EC0_A60* ptr1;
    s_func_800D5EC0*     ptr;
    s64                  temp;

    ptr = FS_BUFFER_1;

    ptr1 = ptr->field_A60;

    for (i = 0; i < 20; i++, ptr1++)
    {
        ptr1->field_34 = 0;

        ptr1->field_0.vx = Q12(0.0f);
        ptr1->field_0.vy = Q12(0.0f);
        ptr1->field_0.vz = Q12(0.0f);

        ptr1->field_30 = 0xFF;

        randAngle = Rng_Rand16();
        sinAngle = Math_Sin(randAngle);
        cosAngle = Math_Cos(randAngle);

        dist = Q12_MULT_PRECISE((Rng_Rand16() % Q12(1.0f)) + Q12(0.5f), Q12(6.0f));

        ptr1->field_20.vx = Q12_MULT_PRECISE(cosAngle, dist);
        ptr1->field_20.vy = Q12_MULT_PRECISE(sinAngle, dist);

        temp                = Q12(-1.6f) + 1;
        ptr1->field_20.vz = temp;
    }
}

void func_800D600C(s32 x, s32 y, s32 val) // 0x800D600C
{
    s32              offsetX;
    s32              offsetY;
    s16              row;
    s16              col;
    s_func_800D5EC0* ptr;
    u8*              buf;

    ptr = FS_BUFFER_1;

    offsetX = ptr->field_A50 & 0xFFFF;
    offsetY = ptr->field_A50 >> 16;

    col = (x + 82) - offsetX;
    row = (y + 122) - offsetY;

    col += (col > 0) ? 2 : -2;
    col /= 4;

    row += (row > 0) ? 2 : -2;
    row /= 4;

    if (col > 0 && col < 41 && row > 0 && row < 61)
    {
        buf  = ptr->field_5D + (col + (41 * row));
        *buf = val;
    }
}

void func_800D60E4(void) // 0x800D60E4
{
    SVECTOR              sp10;
    DVECTOR              sp18[2];
    s32                  sp20;
    s32                  i;
    s_func_800D5EC0_A60* ptr;
    s_func_800D5EC0*     base;

    base = (s_func_800D5EC0*)FS_BUFFER_1;
    ptr  = base->field_A60;

    for (i = 0; i < 20; i++, ptr++)
    {
        if (ptr->field_34 < 0)
        {
            sp10.vx = Q12_TO_Q4(ptr->field_0.vx);
            sp10.vy = Q12_TO_Q4(ptr->field_0.vy);
            sp10.vz = Q12_TO_Q4(ptr->field_0.vz);

            RotTransPers(&sp10, &sp18[0], &sp18[1], &sp20);
            func_800D600C(sp18[0].vx, sp18[0].vy, ptr->field_30);

            ptr->field_0.vx  += ptr->field_20.vx;
            ptr->field_0.vy  += ptr->field_20.vy;
            ptr->field_0.vz  += ptr->field_20.vz;
            ptr->field_20.vz -= 0x199;

            ptr->field_20.vz = MAX(ptr->field_20.vz, -0x1000);
            ptr->field_30   -= 20;
            ptr->field_30    = MAX(ptr->field_30, 0);
        }
        else
        {
            ptr->field_34 -= g_DeltaTime;
        }
    }
}

s32 func_800D621C(s32 arg0, s32 arg1) // 0x800D621C
{
    s_func_800D5EC0* buf;

    buf = (s_func_800D5EC0*)FS_BUFFER_1;

    return D_800EB010[buf->field_5D[(arg1 * 41) + arg0]];
}

void func_800D625C(void) // 0x800D625C
{
    u32              sp18;
    s_func_800D5EC0* ptr;
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
    int              code;

    ptr = (s_func_800D5EC0*)FS_BUFFER_1;

    packet = GsOUT_PACKET_P;
    poly   = packet;

    x = (ptr->field_A50 & 0xFFFF) - 82;
    y = ((u32)ptr->field_A50 >> 16) - 122;

    idx = g_ActiveBufferIdx;
    ot  = g_OrderingTable0[idx].org;
    ot  = &ot[ptr->field_A54 >> 1];

    col  = 0x3A000000;
    code = 0x3A;

    for (i = 1; i < 61; i++)
    {
        col1 = func_800D621C(0, i - 1);
        col3 = func_800D621C(0, i);

        for (j = 1; j < 41; j++)
        {
            sp18 = ptr->field_5D[(i * 41) + j];

            col0 = col1;
            col2 = col3;
            col1 = func_800D621C(j, i - 1);

            col3 = func_800D621C(j, i);

            if (col == col0 &&
                col0 == col1 &&
                col0 == col2 &&
                col2 == col3)
            {
                continue;
            }

            if ((j + i) & 0x1)
            {
                poly->x0 = x + (j * 4);
                poly->y0 = y + (i * 4);
                poly->x1 = (x + (j * 4)) + 4;
                poly->y1 = y + (i * 4);
                poly->x2 = x + (j * 4);
                poly->y2 = (y + (i * 4)) + 4;
                poly->x3 = (x + (j * 4)) + 4;
                poly->y3 = (y + (i * 4)) + 4;

                *(s32*)&poly->r0 = col0;
                *(s32*)&poly->r1 = col1;
                *(s32*)&poly->r2 = col2;
                *(s32*)&poly->r3 = col3;
            }
            else
            {
                poly->x1 = x + (j * 4);
                poly->y1 = y + (i * 4);
                poly->x0 = (x + (j * 4)) + 4;
                poly->y0 = y + (i * 4);
                poly->x3 = x + (j * 4);
                poly->y3 = (y + (i * 4)) + 4;
                poly->x2 = (x + (j * 4)) + 4;
                poly->y2 = (y + (i * 4)) + 4;

                *(s32*)&poly->r0 = col1;
                *(s32*)&poly->r1 = col0;
                *(s32*)&poly->r2 = col3;
                *(s32*)&poly->r3 = col2;
            }

            setPolyG4(poly);
            poly->code = (float)sp18; // @hack
            poly->code = code;

            addPrim(ot, poly);
            poly++;
        }
    }

    packet = poly;
    mode   = packet;

    SetDrawMode(mode, 0, 1, 42, NULL);
    addPrim(ot, mode);
    packet         = mode + 1;
    GsOUT_PACKET_P = packet;
}

void func_800D6548(void) // 0x800D6548
{
    s32              i;
    s32              j;
    s32              val;
    s_func_800D5EC0* buf;
    u8*              tab;

    buf = FS_BUFFER_1;

    for (i = 0; i < 61; i++)
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
            val  -= 1;

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

void func_800D663C(void) // 0x800D663C
{
    s_func_800D5EC0* buf = FS_BUFFER_1;
    memset(buf->field_34, 0, sizeof(buf->field_34));
    memset(buf->field_5D, 0, sizeof(buf->field_5D));
    memset(buf->field_A22, 0, sizeof(buf->field_A22));
}

void func_800D6690(VECTOR* arg0, q19_12 rotY) // 0x800D6690
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

s32 func_800D6750(s32* sxy) // 0x800D6750
{
    SVECTOR sp10;
    s32     sp18;

    sp10.vx = Q8(0.0f);
    sp10.vy = Q8(0.0f);
    sp10.vz = Q8(0.0f);
    return RotTransPers(&sp10, sxy, &sp18, &sp18);
}

void func_800D6788(void) // 0x800D6788
{
    s_func_800D5EC0* buf = FS_BUFFER_1;

    if (buf->field_A5C > 0)
    {
        buf->field_A5C -= g_DeltaTime;
        return;
    }

    func_800D6690(&buf->field_4, buf->field_A58);
    buf->field_A54 = func_800D6750(&buf->field_A50);

    func_800D60E4();
    func_800D6548();
    func_800D625C();
}

void func_800D6804(const VECTOR3* from, const VECTOR3* to) // 0x800D6804
{
    q19_12           x0;
    q19_12           z0;
    q19_12           x1;
    q19_12           z1;
    s_func_800D5EC0* buf;

    buf = FS_BUFFER_1;

    x0 = to->vx - from->vx;
    z0 = to->vz - from->vz;

    buf->field_A58 = ratan2(x0, z0);

    if (x0 < Q12(0.0f))
    {
        x1 = x0 + 3;
    }
    else
    {
        x1 = x0;
    }

    x0 = from->vx + (x1 >> 2);

    if (z0 < Q12(0.0f))
    {
        z1 = z0 + 3;
    }
    else
    {
        z1 = z0;
    }

    z0 = (from->vz + (z1 >> 2));

    buf->field_4.vx = Q12_TO_Q8(x0);
    buf->field_4.vy = Q8(-1.0f);
    buf->field_4.vz = Q12_TO_Q8(z0);

    buf->field_A5C = 0;

    func_800D663C();
    func_800D5EC0();
}

void func_800D68C4(void) // 0x800D68C4
{
    q19_12               posX;
    q19_12               posY;
    q19_12               posZ;
    q19_12               dist;
    q19_12               randAngle;
    s32                  i;
    s_func_800D68C4*     base;
    s_func_800D68C4_A08* curPtr;

    base = FS_BUFFER_1;

    curPtr = base->field_A08;
    for (i = 0; i < 100; i++, curPtr++)
    {
        curPtr->field_0.vx = Q12(0.0f);
        curPtr->field_0.vy = Q12(0.0f);
        curPtr->field_0.vz = Q12(0.0f);

        curPtr->field_30 = 0xFF;

        randAngle = Rng_Rand16();
        posY = Q12_MULT_PRECISE(Math_Sin(randAngle), Q12(0.6f));
        dist = Q12_MULT_PRECISE(Math_Cos(randAngle), Q12(0.6f));

        randAngle = Rng_Rand16();
        posX = Q12_MULT_PRECISE(Math_Sin(randAngle), dist);
        posZ = Q12_MULT_PRECISE(Math_Cos(randAngle), dist);

        curPtr->field_20.vx = posX;
        curPtr->field_20.vy = posY;
        curPtr->field_20.vz = posZ;
    }
}

void func_800D6A10(s32 x, s32 y, s32 val) // 0x800D6A10
{
    s32              offsetX;
    s32              offsetY;
    s16              row;
    s16              col;
    s_func_800D68C4* ptr;
    u8*              buf;

    ptr = FS_BUFFER_1;

    offsetX = ptr->field_9FC & 0xFFFF;
    offsetY = ptr->field_9FC >> 16;

    col = (x + 49) - offsetX;
    row = (y + 49) - offsetY;

    col += (col > 0) ? 1 : -1;
    col /= 2;

    row += (row > 0) ? 1 : -1;
    row /= 2;

    if (col > 0 && col < 49 && row > 0 && row < 49)
    {
        buf  = ptr->field_65 + (col + (49 * row));
        *buf = val;
    }
}

void func_800D6ADC(void) // 0x800D6ADC
{
    SVECTOR              sp10;
    DVECTOR              sp18[2];
    s32                  sp20;
    s32                  i;
    s_func_800D68C4_A08* curPtr;
    s_func_800D68C4*     base;

    base = (s_func_800D68C4*)FS_BUFFER_1;

    curPtr = base->field_A08;
    for (i = 0; i < 100; i++, curPtr++)
    {
        sp10.vx = Q12_TO_Q4(curPtr->field_0.vx);
        sp10.vy = Q12_TO_Q4(curPtr->field_0.vy);
        sp10.vz = Q12_TO_Q4(curPtr->field_0.vz);

        RotTransPers(&sp10, &sp18[0], &sp18[1], &sp20);
        func_800D6A10(sp18[0].vx, sp18[0].vy, curPtr->field_30);

        curPtr->field_0.vx += curPtr->field_20.vx;
        curPtr->field_0.vy += curPtr->field_20.vy;
        curPtr->field_0.vz += curPtr->field_20.vz;
        curPtr->field_30    = curPtr->field_30 - 8;
        curPtr->field_30    = MAX(curPtr->field_30, 0);
    }
}

s32 func_800D6BCC(s32 arg0, s32 arg1) // 0x800D6BCC
{
    s_func_800D68C4* buf;

    buf = (s_func_800D68C4*)FS_BUFFER_1;

    return D_800EB410[buf->field_65[(arg1 * 49) + arg0]];
}

void func_800D6C0C(void) // 0x800D6C0C
{
    u32              sp18;
    s_func_800D68C4* ptr;
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
    int              code;

    ptr = FS_BUFFER_1;

    packet = GsOUT_PACKET_P;
    poly   = packet;

    x = (ptr->field_9FC & 0xFFFF) - 49;
    y = ((u32)ptr->field_9FC >> 16) - 49;

    idx = g_ActiveBufferIdx;
    ot  = g_OrderingTable0[idx].org;
    ot  = &ot[ptr->field_A00 >> 1];

    col  = 0x3A000000;
    code = 0x3A;

    for (i = 1; i < 49; i++)
    {
        col1 = func_800D6BCC(0, i - 1);
        col3 = func_800D6BCC(0, i);

        for (j = 1; j < 49; j++)
        {
            sp18 = ptr->field_65[(i * 49) + j];

            col0 = col1;
            col2 = col3;
            col1 = func_800D6BCC(j, i - 1);

            col3 = func_800D6BCC(j, i);

            if (col == col0 &&
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
            poly->code = (float)sp18; // @hack
            poly->code = code;

            addPrim(ot, poly);
            poly++;
        }
    }

    packet = poly;
    mode   = packet;

    SetDrawMode(mode, 0, 1, 42, NULL);
    addPrim(ot, mode);
    packet         = mode + 1;
    GsOUT_PACKET_P = packet;
}

void func_800D6EF8(void) // 0x800D6EF8
{
    s32              i;
    s32              j;
    s32              val;
    s_func_800D68C4* buf;
    u8*              tab;

    buf = FS_BUFFER_1;

    for (i = 0; i < 49; i++)
    {
        tab = &buf->field_65[i * 49];

        for (j = 0; j < 49; j++)
        {
            val = tab[j - 49];

            if (j == 0)
            {
                val = 0;
            }
            else
            {
                val += tab[j - 1];
            }

            if (j == 48)
            {
                val = 0;
            }
            else
            {
                val = val + tab[j + 1];
            }

            val  += tab[j + 49];
            val >>= 2;
            val  -= 1;

            if (val <= 0)
            {
                tab[j] = 0;
            }
            else
            {
                tab[j] = val;
            }
        }
    }
}

void func_800D6FA0(void) // 0x800D6FA0
{
    s_func_800D68C4* buf = FS_BUFFER_1;

    memset(buf->field_34, 0, sizeof(buf->field_34));
    memset(buf->field_65, 0, sizeof(buf->field_65));
    memset(buf->field_9C6, 0, sizeof(buf->field_9C6));
}

void func_800D6FF4(VECTOR* arg0, q19_12 rotY) // 0x800D6FF4
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

s32 func_800D70B4(s32* sxy) // 0x800D70B4
{
    SVECTOR sp10;
    s32     sp18;

    sp10.vx = Q8(0.0f);
    sp10.vy = Q8(0.0f);
    sp10.vz = Q8(0.0f);
    return RotTransPers(&sp10, sxy, &sp18, &sp18);
}

void func_800D70EC(void) // 0x800D70EC
{
    s_func_800D68C4* buf = FS_BUFFER_1;

    func_800D6FF4(&buf->field_4, buf->field_A04);
    buf->field_A00 = func_800D70B4(&buf->field_9FC);

    func_800D6ADC();
    func_800D6EF8();
    func_800D6C0C();
}

void func_800D7144(VECTOR3* arg0) // 0x800D7144
{
    s_func_800D68C4* buf = FS_BUFFER_1;

    buf->field_A04 = 0;

    buf->field_4.vx = Q12_TO_Q8(arg0->vx);
    buf->field_4.vy = Q12_TO_Q8(arg0->vy);
    buf->field_4.vz = Q12_TO_Q8(arg0->vz);

    func_800D6FA0();
    func_800D68C4();
}

void func_800D71A4(s32 arg0) // 0x800D71A4
{
    D_800F2430 = arg0;
}

void func_800D71B0(VECTOR3* pos, s32 arg1) // 0x800D71B0
{
    s32                  temp_hi_4;
    s32                  temp_hi_5;
    s32                  temp_hi_6;
    q19_12               sinAngle;
    q19_12               cosAngle;
    q19_12               randAngle0;
    s32                  randAngle1;
    s32                  temp_v0_4;
    s32                  temp_v0_6;
    s32                  i;
    s32                  j;
    s_func_800D71B0_498* ptr;
    s_func_800D71B0*     base;

    base = FS_BUFFER_25;
    ptr  = base->field_498;

    for (j = 0, i = 0; i < 200; i++, ptr++)
    {
        if (ptr->field_20 != 0)
        {
            continue;
        }

        if (j >= arg1)
        {
            break;
        }

        j++;

        randAngle0 = Rng_Rand16();
        sinAngle = Math_Sin(randAngle0);
        cosAngle = Math_Cos(randAngle0);

        randAngle1 = Rng_Rand16();
        temp_v0_4 = Q12_MULT_PRECISE(cosAngle, Math_Sin(randAngle1));
        temp_v0_6 = Q12_MULT_PRECISE(cosAngle, Math_Cos(randAngle1));

        ptr->field_0.vx = pos->vx + Q12_MULT_PRECISE(temp_v0_4, D_800EB810);
        ptr->field_0.vy = pos->vy + Q12_MULT_PRECISE(sinAngle, D_800EB810);
        ptr->field_0.vz = pos->vz + Q12_MULT_PRECISE(temp_v0_6, D_800EB810);
        ptr->field_28   = 0xFFF;

        randAngle0 = Rng_Rand16();
        sinAngle = Math_Sin(randAngle0);
        cosAngle = Math_Cos(randAngle0);

        randAngle0   = Rng_Rand16();
        temp_v0_4 = Q12_MULT_PRECISE(Math_Sin(randAngle0), cosAngle);
        temp_v0_6 = Q12_MULT_PRECISE(Math_Cos(randAngle0), cosAngle);

        temp_hi_4 = Q12_MULT_PRECISE(temp_v0_4, Q12(0.6f));
        temp_hi_5 = Q12_MULT_PRECISE(temp_v0_6, Q12(0.6f));
        temp_hi_6 = Q12_MULT_PRECISE(sinAngle, Q12(0.6f));

        ptr->field_10.vx = ((ptr->field_0.vx - pos->vx) * 4) + Q12_MULT_PRECISE(temp_v0_4, Q12(0.6f));
        ptr->field_10.vy = ((ptr->field_0.vy - pos->vy) * 4) + Q12_MULT_PRECISE(sinAngle, Q12(0.6f));
        ptr->field_10.vz = ((ptr->field_0.vz - pos->vz) * 4) + Q12_MULT_PRECISE(temp_v0_6, Q12(0.6f));

        randAngle0       = Rng_Rand16();
        ptr->field_34 = 0;
        randAngle0       = Rng_Rand16();
        ptr->field_30 = Q12(1.0f);
        ptr->field_20 = 1;
    }
}

void func_800D74F4(s32 x, s32 y, s32 val) // 0x800D74F4
{
    s16              row;
    s16              col;
    s_func_800D71B0* ptr;
    u8*              buf;

    ptr = FS_BUFFER_25;

    col = (x + 164);
    row = (y + 100);

    col += (col > 0) ? 4 : -4;
    col /= 8;

    row += (row > 0) ? 4 : -4;
    row /= 8;

    if (col > 0 && col < 41 && row > 0 && row < 25)
    {
        buf = ptr->field_5D + (col + (41 * row));
        if (val >> 4 < 0x100)
        {
            *buf = val >> 4;
        }
        else
        {
            *buf = 0xFF;
        }
    }
}

void func_800D75D0(void) // 0x800D75D0
{
    SVECTOR              sp10;
    DVECTOR              sp18[2];
    s32                  sp20;
    s32                  i;
    s_func_800D71B0_498* ptr;
    s_func_800D71B0*     base;

    base = FS_BUFFER_25;
    ptr  = base->field_498;

    for (i = 0; i < 200; i++, ptr++)
    {
        if (ptr->field_20 != 0)
        {
            if (ptr->field_34 < 0)
            {
                sp10.vx = Q12_TO_Q8(ptr->field_0.vx);
                sp10.vy = Q12_TO_Q8(ptr->field_0.vy);
                sp10.vz = Q12_TO_Q8(ptr->field_0.vz);

                RotTransPers(&sp10, &sp18[0], &sp18[1], &sp20);
                func_800D74F4(sp18[0].vx, sp18[0].vy, ptr->field_28);

                ptr->field_0.vx += Q12_MULT_PRECISE(ptr->field_10.vx, g_DeltaTime);
                ptr->field_0.vy += Q12_MULT_PRECISE(ptr->field_10.vy, g_DeltaTime);
                ptr->field_0.vz += Q12_MULT_PRECISE(ptr->field_10.vz, g_DeltaTime);

                ptr->field_28 -= Q12_MULT_PRECISE(g_DeltaTime, 0x1000);
                ptr->field_28  = MAX(ptr->field_28, 0);

                ptr->field_30 -= g_DeltaTime;

                if (ptr->field_30 < 0)
                {
                    ptr->field_20 = 0;
                }
            }
            else
            {
                ptr->field_34 -= g_DeltaTime;
            }
        }
    }
}

s32 func_800D77A4(s32 arg0, s32 arg1) // 0x800D77A4
{
    s_func_800D71B0* buf;

    buf = (s_func_800D71B0*)FS_BUFFER_25;

    return D_800EB814[buf->field_5D[(arg1 * 41) + arg0]];
}

void func_800D77E4(void) // 0x800D77E4
{
    u32              sp18;
    s_func_800D71B0* ptr;
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
    int              code;

    ptr = FS_BUFFER_25;

    x = -0xA4;
    y = -0x64;

    idx = g_ActiveBufferIdx;
    ot  = g_OrderingTable0[idx].org;

    if ((ptr->field_490 >> 1) < 0x706)
    {
        ot = &ot[(ptr->field_490 >> 1) + 250];
    }
    else
    {
        ot = &ot[2036];
    }

    packet = GsOUT_PACKET_P;
    poly   = packet;

    i    = 1;
    col  = 0x3A000000;
    code = 0x3A;

    for (; i < 25; i++)
    {
        col1 = func_800D77A4(0, i - 1);
        col3 = func_800D77A4(0, i);

        for (j = 1; j < 41; j++)
        {
            sp18 = ptr->field_5D[(i * 41) + j];

            col0 = col1;
            col2 = col3;
            col1 = func_800D77A4(j, i - 1);

            col3 = func_800D77A4(j, i);

            if (col == col0 &&
                col0 == col1 &&
                col0 == col2 &&
                col2 == col3)
            {
                continue;
            }

            if ((j + i) & 0x1)
            {
                poly->x0 = x + (j * 8);
                poly->y0 = y + (i * 8);
                poly->x1 = (x + (j * 8)) + 8;
                poly->y1 = y + (i * 8);
                poly->x2 = x + (j * 8);
                poly->y2 = (y + (i * 8)) + 8;
                poly->x3 = (x + (j * 8)) + 8;
                poly->y3 = (y + (i * 8)) + 8;

                *(s32*)&poly->r0 = col0;
                *(s32*)&poly->r1 = col1;
                *(s32*)&poly->r2 = col2;
                *(s32*)&poly->r3 = col3;
            }
            else
            {
                poly->x1 = x + (j * 8);
                poly->y1 = y + (i * 8);
                poly->x0 = (x + (j * 8)) + 8;
                poly->y0 = y + (i * 8);
                poly->x3 = x + (j * 8);
                poly->y3 = (y + (i * 8)) + 8;
                poly->x2 = (x + (j * 8)) + 8;
                poly->y2 = (y + (i * 8)) + 8;

                *(s32*)&poly->r0 = col1;
                *(s32*)&poly->r1 = col0;
                *(s32*)&poly->r2 = col3;
                *(s32*)&poly->r3 = col2;
            }

            setPolyG4(poly);
            poly->code = (float)sp18; // @hack
            poly->code = code;

            addPrim(ot, poly);
            poly++;
        }
        ot -= 0x28 / 4;
    }

    packet = poly;
    mode   = packet;

    SetDrawMode(mode, 0, 1, 42, NULL);
    addPrim(ot, mode);
    packet         = mode + 1;
    GsOUT_PACKET_P = packet;
}

void func_800D7AD0(void) // 0x800D7AD0
{
    s32              i;
    s32              j;
    s32              val;
    s_func_800D71B0* buf;
    u8*              tab;

    buf = FS_BUFFER_25;

    for (i = 0; i < 25; i++)
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
            val  -= 1;

            if (val <= 0)
            {
                tab[j] = 0;
            }
            else
            {
                tab[j] = val;
            }
        }
    }
}

void func_800D7B78(void) // 0x800D7B78
{
    s32                  i;
    s_func_800D71B0_498* ptr;
    s_func_800D71B0*     buf;

    buf = FS_BUFFER_25;

    ptr = buf->field_498;

    memset(buf->field_34, 0, sizeof(buf->field_34));
    memset(buf->field_5D, 0, sizeof(buf->field_5D));
    memset(buf->field_45E, 0, sizeof(buf->field_45E));

    for (i = 199; i >= 0; i--, ptr++)
    {
        ptr->field_20 = 0;
    }

    func_800D71A4(0xFF);
}

void func_800D7BF4(VECTOR* arg0, q19_12 rotY) // 0x800D7BF4
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

s32 func_800D7CB4(s32* sxy) // 0x800D7CB4
{
    SVECTOR sp10;
    s32     sp18;

    sp10.vx = Q8(0.0f);
    sp10.vy = Q8(0.0f);
    sp10.vz = Q8(0.0f);
    return RotTransPers(&sp10, sxy, &sp18, &sp18);
}

void func_800D7CEC(void) // 0x800D7CEC
{
    D_800F2434 = 0;
}

void func_800D7CF8(void) // 0x800D7CF8
{
    D_800F2434 = 1;
}

void func_800D7D08(void) // 0x800D7D08
{
    s_func_800D71B0* buf = FS_BUFFER_25;

    func_800D7BF4(&buf->field_4, buf->field_494);
    buf->field_490 = func_800D7CB4(&buf->field_48C) + 0x140;

    if (D_800F2434 == 0)
    {
        func_800D75D0();
    }
    func_800D7AD0();
    func_800D77E4();
}

void func_800D7D74(s32 arg0) // 0x800D7D74
{
    VECTOR3          sp10;
    VECTOR3          sp20;
    s_func_800D71B0* ptr;

    if (D_800F2434 == 0)
    {
        sp10.vx = Rng_Rand16() - 0x3FFF;
        sp10.vy = (Rng_Rand16() >> 2) + 0x4801;
        sp10.vz = (Rng_Rand16() * 2) - 0x7FFF;

        if (func_800D5D48())
        {
            sp20 = sp10;

            ptr      = FS_BUFFER_25;
            sp20.vx += ptr->field_4.vx * 0x10;
            sp20.vy += ptr->field_4.vy * 0x10;
            sp20.vz += ptr->field_4.vz * 0x10;

            Sfx_WithFlagsPlay(Sfx_Unk1694, &sp20, D_800F2430, SfxFlag_None);
        }

        func_800D71B0(&sp10, 0x1E);
    }
}

void func_800D7E50(VECTOR3* arg0) // 0x800D7E50
{
    s_func_800D71B0* ptr;

    ptr = FS_BUFFER_25;

    memset(ptr, 0xA7, sizeof(s_func_800D71B0));

    func_800D7B78();

    ptr->field_494  = 0;
    ptr->field_4.vx = Q12_TO_Q8(arg0->vx);
    ptr->field_4.vy = Q12_TO_Q8(arg0->vy);
    ptr->field_4.vz = Q12_TO_Q8(arg0->vz);
}

void func_800D7ED0(void) // 0x800D7ED0
{
    func_800D7B78();
}

void func_800D7EF0(void) // 0x800D7D08
{
    func_800D7D08();
}

PACKET* func_800D7F10(void) // 0x800D7F10
{
    return GsOUT_PACKET_P;
}

void func_800D7F20(u8* arg0) // 0x800D7F20
{
    GsOUT_PACKET_P = arg0;
}

void func_800D7F2C(GsOT_TAG* ot, s32 arg1, q19_12 angle, q19_12 dist0, q19_12 dist1, s16 x1, s16 y1, s32 arg7) // 0x800D7F2C
{
    q19_12   sp14;
    s32      i;
    q19_12   angle0;
    q19_12   y2;
    q19_12   x2;
    q19_12   y0;
    q19_12   x0;
    q19_12   x3;
    q19_12   y3;
    POLY_G4* poly;

    sp14 = Q12(1.0f) / arg1;
    sp14 = sp14 / 2;

    poly   = func_800D7F10();
    angle0 = angle;

    for (i = 0; i < arg1; i++)
    {
        x0 = Q12_MULT_PRECISE(Math_Sin(angle0), dist1);
        y0 = Q12_MULT_PRECISE(Math_Cos(angle0), dist1);

        angle0 += sp14;

        x2 = Q12_MULT_PRECISE(Math_Sin(angle0), dist0);
        y2 = Q12_MULT_PRECISE(Math_Cos(angle0), dist0);

        angle0 += sp14;

        if (i == (arg1 - 1))
        {
            angle0 = angle;
        }

        x0 += x1;
        x2 += x1;
        y0 += y1;
        y2 += y1;

        x3 = Q12_MULT_PRECISE(Math_Sin(angle0), dist1);
        y3 = Q12_MULT_PRECISE(Math_Cos(angle0), dist1);

        x3 += x1;
        y3 += y1;

        *(s32*)&poly->r0 = 0;

        setPolyG4(poly);
        setSemiTrans(poly, 1);

        *(s32*)&poly->r1 = arg7;
        *(s32*)&poly->r2 = 0;
        *(s32*)&poly->r3 = 0;

        setXY4(poly, x0, y0, x1, y1, x2, y2, x3, y3);

        addPrim(ot, poly);
        poly++;
    }

    func_800D7F20(poly);
}

s32 func_800D822C(SVECTOR* worldPos, s16* outScreenX, s16* outScreenY) // 0x800D822C
{
    DVECTOR screenCoords[2];
    s32     depth;
    s32     scale;
    s16     screenX;
    s16     screenY;

    depth = RotTransPers(worldPos, &screenCoords[0], &screenCoords[1], &screenCoords[1]);
    scale = (ReadGeomScreen() * 200) / (depth + 1);

    screenX = screenCoords[0].vx;
    screenY = screenCoords[0].vy;

    *outScreenX = screenX;
    *outScreenY = screenY;
    return scale;
}

void func_800D82AC(GsOT_TAG* ot, s32 arg1, s32 arg2, s32 arg3) // 0x800D82AC
{
    s32 val1;
    s32 val2;

    val1 = Q12_MULT_PRECISE(arg3, 3000);
    val2 = Q12_MULT_PRECISE(arg3, 5000);

    func_800D7F2C(ot, 16, D_800EBC14, val2, val1, arg1, arg2, 0x141414);
    func_800D7F2C(ot, 12, 0, val2, val1, arg1, arg2, 0x141414);

    D_800EBC14 += 4;

    func_800D7F2C(ot, 4, -512,
                  Q12_MULT_PRECISE(arg3, 15000), Q12_MULT_PRECISE(arg3, 1500),
                  arg1, arg2, 0x102020);
}

s32 func_800D8438(s32 idx) // 0x800D8438
{
    return D_800EBC18[idx];
}

void func_800D8454(s32* arg0, s32 x, s32 y, s32 s) // 0x800D8454
{
    s32      i;
    s32      j;
    s32      col0;
    s32      col1;
    q19_12   angle;
    q19_12   x0;
    q19_12   y0;
    q19_12   w0;
    q19_12   h0;
    q19_12   x1;
    q19_12   y1;
    q19_12   w1;
    q19_12   h1;
    q19_12   sp34;
    q20_12   var_s1;
    s32      temp;
    POLY_G4* poly;

    sp34 = Q12(25.0f);
    angle = Q12_ANGLE(0.0f);

    x = Q12(x);
    y = Q12(y);
    s = Q12_MULT_PRECISE(s, Q12(1.5f));

    poly = (POLY_G4*)func_800D7F10();
    temp = 8;

    for (i = 0; i < 32; i++)
    {
        w0  = Math_Sin(angle);
        h0  = Math_Cos(angle);
        angle = angle + Q12_ANGLE(11.25f);
        w1  = Math_Sin(angle);
        h1  = Math_Cos(angle);

        w0 = (w0 * s) / temp;
        h0 = (h0 * s) / temp;
        w1 = (w1 * s) / temp;
        h1 = (h1 * s) / temp;

        var_s1 = Q12(200.0f);

        x0 = x;
        y0 = y;
        x1 = x;
        y1 = y;

        for (j = 0; j < temp; j++)
        {
            poly->x0 = FP_FROM(x0, Q12_SHIFT);
            poly->x1 = FP_FROM(x0 + w0, Q12_SHIFT);
            poly->x2 = FP_FROM(x1, Q12_SHIFT);
            poly->x3 = FP_FROM(x1 + w1, Q12_SHIFT);

            poly->y0 = FP_FROM(y0, Q12_SHIFT);
            poly->y1 = FP_FROM(y0 + h0, Q12_SHIFT);
            poly->y2 = FP_FROM(y1, Q12_SHIFT);
            poly->y3 = FP_FROM(y1 + h1, Q12_SHIFT);

            col0 = func_800D8438(FP_FROM(var_s1, Q12_SHIFT));
            col1 = func_800D8438(FP_FROM(var_s1 - sp34, Q12_SHIFT));

            *(s32*)&poly->r0 = col0;

            SetPolyG4(poly);
            *(s32*)&poly->r1 = col1;
            *(s32*)&poly->r2 = col0;
            *(s32*)&poly->r3 = col1;

            setSemiTrans(poly, 1);

            addPrim(arg0, poly);

            x0     += w0;
            x1     += w1;
            y0     += h0;
            y1     += h1;
            var_s1 -= sp34;
            poly++;
        }
    }

    func_800D7F20((u8*)poly);
}

void func_800D8738(void* ot, s32 abr) // 0x800D8738
{
    DR_MODE* drMode;

    drMode = func_800D7F10();
    SetDrawMode(drMode, 0, 1, getTPage(0, abr, 640, 0), NULL);
    addPrim(ot, drMode);

    func_800D7F20(&drMode[1]);
}

void func_800D87D4(void* ot) // 0x800D87D4
{
    SVECTOR worldPos;
    s16     screenX;
    s16     screenY;
    s32     screenDepth;

    Math_SVectorSet(&worldPos, 0, 0, 0);

    screenDepth = func_800D822C(&worldPos, &screenX, &screenY);
    if (screenDepth <= 100)
    {
        func_800D82AC(ot, screenX, screenY, screenDepth);
        func_800D8454(ot, screenX, screenY, screenDepth);
        func_800D8738(ot, 1);
    }
}

void func_800D8858(s32 arg0) // 0x800D8858
{
    GsOT_TAG* ot;
    s32       bufferIdx;

    if (arg0 < 3)
    {
        bufferIdx = g_ActiveBufferIdx;
        ot        = g_OrderingTable0[bufferIdx].org;
        ot        = &ot[300];

        func_800D7BF4(&D_800EC018[arg0], 0);
        func_800D87D4(ot);
    }
}

void func_800D88C4(void) // 0x800D88C4
{
}

void func_800D88CC(void) // 0x800D88CC
{
    D_800F3D8C = 0;
}

void func_800D88D8(void) // 0x800D88D8
{
    D_800F3D8C = 1;
}

s_800F3D48_0* func_800D88E8(s_800F3D48* arg0) // 0x800D88E8
{
    s_800F3D48_0* next;
    s_800F3D48_0* curPtr;
    s32           field8Value;

    curPtr      = arg0->ptr_0;
    field8Value = curPtr->field_8;

    if (field8Value == 1)
    {
        curPtr = curPtr->next_4;
        next   = curPtr->next_4;
    }
    else
    {
        next = curPtr->next_4;
    }

    if (arg0->field_4.field_0 <= 0)
    {
        if (field8Value != 2 && g_DeltaTime != Q12(0.0f))
        {
            arg0->ptr_0           = &curPtr[1];
            arg0->field_4.field_0 = curPtr[1].field_0;
        }
    }
    else
    {
        arg0->field_4.field_0--;
    }

    return next;
}

void func_800D8954(s_800F3D48* arg0, s_800F3D48_0_0* arg1) // 0x800D8954
{
    SVECTOR   sp10; // Q23.8
    DVECTOR   sp18[2];
    s32       sp20;
    s16       x0;
    s16       y0;
    s32       offsetX;
    s32       offsetY;
    s32       temp_lo_4;
    s32       temp_lo_5;
    s32       temp_s1;
    s32       temp_s3;
    s32       temp_t0;
    s32       temp_v0;
    s32       temp;
    s32       temp2;
    s32       bufferIdx;
    s32       otIdx;
    s32       temp5;
    s32       temp6;
    GsOT_TAG* ot;
    POLY_FT4* poly;

    temp5   = 0x10101;
    temp_s3 = arg0->field_4.field_3C * temp5;

    sp10.vx = Q12_TO_Q8(arg0->field_4.field_18.vx);
    sp10.vy = Q12_TO_Q8(arg0->field_4.field_18.vy);
    sp10.vz = Q12_TO_Q8(arg0->field_4.field_18.vz);

    temp_s1 = RotTransPers(&sp10, &sp18[0], &sp18[1], &sp20);

    if (sp20 & 0x20000)
    {
        return;
    }

    temp_v0 = ReadGeomScreen();
    temp_t0 = arg0->field_4.field_14;

    offsetX = arg1->field_4;
    offsetY = arg1->field_6;

    offsetX = ((offsetX * temp_v0) / 4) / temp_s1;
    offsetY = ((offsetY * temp_v0) / 4) / temp_s1;

    temp_lo_4 = ((arg1->field_C / 4) * temp_v0) / temp_s1;
    temp_lo_5 = ((arg1->field_E / 4) * temp_v0) / temp_s1;

    offsetX = Q12_MULT_PRECISE(offsetX, temp_t0);
    offsetY = Q12_MULT_PRECISE(offsetY, temp_t0);

    temp  = Q12_MULT_PRECISE(temp_lo_4, temp_t0);
    temp2 = Q12_MULT_PRECISE(temp_lo_5, temp_t0);

    temp6   = sp18[0].vy;
    x0 = sp18[0].vx - temp;
    y0 = temp6 - temp2;

    if (x0 < (-offsetX - 160) || x0 > 160 ||
        y0 < (-offsetY - 160) || y0 > 112)
    {
        return;
    }

    poly = GsOUT_PACKET_P;

    setXY4(poly,
           x0, y0,
           x0 + offsetX, y0,
           x0, y0 + offsetY,
           x0 + offsetX, y0 + offsetY);

    setUV4(poly, arg1->field_0, arg1->field_2, arg1->field_0 + arg1->field_4, arg1->field_2,
           arg1->field_0, arg1->field_2 + arg1->field_6, arg1->field_0 + arg1->field_4, arg1->field_2 + arg1->field_6);

    *(s32*)&poly->r0 = temp_s3;
    poly->tpage      = arg1->field_8;
    poly->clut       = arg1->field_A + arg0->field_4.field_E;

    setPolyFT4(poly);
    setSemiTrans(poly, 1);

    bufferIdx = g_ActiveBufferIdx;
    ot        = g_OrderingTable0[bufferIdx].org;
    otIdx     = temp_s1 >> 1;
    ot        = &ot[otIdx];

    addPrim(ot, poly);
    poly++;

    GsOUT_PACKET_P = poly;
}

void func_800D8CD4(q19_12 rotZ, SVECTOR* rot) // 0x800D8CD4
{
    MATRIX      transformMat;
    VECTOR      offset; // Rotation offset?
    SVECTOR     rot1;
    SVECTOR     rot0;
    q19_12      rotY;
    s_800F3D58* ptr;

    ptr = &D_800F3D58;

    ApplyRotMatrix(rot, &offset);

    offset.vx += ptr->mat_10.t[0];
    offset.vy += ptr->mat_10.t[1];
    offset.vz += ptr->mat_10.t[2];

    TransMatrix(&transformMat, &offset);
    SetTransMatrix(&transformMat);
    vwGetViewAngle(&rot0);

    rot1.vx = Q12_ANGLE(0.0f);
    rot1.vz = -rotZ;
    rotY    = rot0.vy;
    rot1.vy = rotY;

    Math_RotMatrixZxy(&rot1, &transformMat);
    SetMulRotMatrix(&transformMat);
}

void func_800D8D90(s_800F3D48* arg0, s_800F3D48_0_0* arg1) // 0x800D8D90
{
    SVECTOR   sp28;
    SVECTOR   sp30;
    SVECTOR   sp38;
    SVECTOR   sp40;
    SVECTOR   sp48;
    DVECTOR   sp50[4];
    s32       sp60;
    s32       sp64;
    s32       temp_a2;
    s32       temp_t1;
    s32       temp_s2;
    s32       temp_v0;
    s32       bufferIdx;
    s32       otIdx;
    s32       temp;
    GsOT_TAG* ot;
    POLY_FT4* poly;

    PushMatrix();

    sp28.vx = Q12_TO_Q8(arg0->field_4.field_18.vx);
    sp28.vy = Q12_TO_Q8(arg0->field_4.field_18.vy);
    sp28.vz = Q12_TO_Q8(arg0->field_4.field_18.vz);
    func_800D8CD4(arg0->field_4.field_38, &sp28);

    temp_t1 = arg1->field_4;
    temp_v0 = arg0->field_4.field_14;
    temp_a2 = arg1->field_6;

    temp_t1 = Q12_MULT_PRECISE(temp_t1, temp_v0);
    temp_a2 = Q12_MULT_PRECISE(temp_a2, temp_v0);

    temp = 0x10101;

    sp30.vx = -temp_t1;
    sp30.vy = -temp_a2;
    sp30.vz = 0;

    sp38.vx = temp_t1;
    sp38.vy = -temp_a2;
    sp38.vz = 0;

    sp40.vx = -temp_t1;
    sp40.vy = temp_a2;
    sp40.vz = 0;

    sp48.vx = temp_t1;
    sp48.vy = temp_a2;
    sp48.vz = 0;

    temp_s2 = RotTransPers4(&sp30, &sp38, &sp40, &sp48, &sp50[0], &sp50[1], &sp50[2], &sp50[3], &sp60, &sp64);

    PopMatrix();

    if (sp64 & 0x60000)
    {
        return;
    }

    poly = GsOUT_PACKET_P;

    *(s32*)&poly->x0 = *(s32*)&sp50[0];
    *(s32*)&poly->x1 = *(s32*)&sp50[1];
    *(s32*)&poly->x2 = *(s32*)&sp50[2];
    *(s32*)&poly->x3 = *(s32*)&sp50[3];

    setUV4(poly, arg1->field_0, arg1->field_2, arg1->field_0 + arg1->field_4, arg1->field_2,
           arg1->field_0, arg1->field_2 + arg1->field_6, arg1->field_0 + arg1->field_4, arg1->field_2 + arg1->field_6);

    *(s32*)&poly->r0 = arg0->field_4.field_3C * temp;
    poly->tpage      = arg1->field_8;
    poly->clut       = arg1->field_A;

    setPolyFT4(poly);
    setSemiTrans(poly, 1);

    bufferIdx = g_ActiveBufferIdx;
    ot        = g_OrderingTable0[bufferIdx].org;
    otIdx     = temp_s2 >> 1;
    ot        = &ot[otIdx];

    addPrim(ot, poly);
    poly++;

    GsOUT_PACKET_P = poly;
}

s_800F3D48* func_800D905C(void) // 0x800D905C
{
    s32         i;
    s_800F3D48* ptr;

    ptr = D_800F3D48;

    for (i = 0; i < D_800F2438; i++, ptr++)
    {
        s_800F3D48_4* ptr2 = &ptr->field_4;

        if (ptr2->field_4 != 0 || ptr2->field_6 != 0)
        {
            continue;
        }

        ptr2->field_6  = 1;
        ptr2->field_C  = 0;
        ptr2->field_8  = 0;
        ptr2->field_40 = 0;
        ptr2->field_E  = 0;
        ptr2->field_0  = 0;

        return ptr;
    }

    return NULL;
}

void func_800D90C8(void) // 0x800D90C8
{
    s_800F3D58* ptr;

    ptr = &D_800F3D58;

    Vw_WorldScreenMatrixAtPositionGet(&ptr->mat_10, ptr->field_0.vx, 0, ptr->field_0.vz);
    SetRotMatrix(&ptr->mat_10);
    SetTransMatrix(&ptr->mat_10);
}

void func_800D9114(s_800F3D48* arg0) // 0x800D9114
{
    arg0->field_4.field_18.vz += Q12_MULT_PRECISE(arg0->field_4.field_8, g_DeltaTime);

    if (g_DeltaTime != Q12(0.0f))
    {
        arg0->field_4.field_3C -= arg0->field_4.field_40;
        arg0->field_4.field_3C  = MAX(0, arg0->field_4.field_3C);
    }
}

void func_800D917C(void) // 0x800D917C
{
    s32             i;
    s_800F3D48*     curPtr;
    s_800F3D48_0*   ptr1;
    s_800F3D48_0_0* ptr2;

    curPtr = D_800F3D48;

    func_800D90C8();

    for (i = 0; i < D_800F2438; i++, curPtr++)
    {
        if (curPtr->field_4.field_4 != 0)
        {
            if (curPtr->field_4.field_44 != NULL)
            {
                if (D_800F3D8C == 0)
                {
                    ptr1 = curPtr->field_4.field_44(curPtr);
                    if (ptr1 != NULL && curPtr->field_4.field_48 != NULL && D_800F3D90 == 0)
                    {
                        curPtr->field_4.field_48(curPtr, ptr1);
                    }
                }
                else if (curPtr->field_4.field_48 != NULL && D_800F3D90 == 0)
                {
                    // TODO: `func_800D88E8` returns different struct.
                    ptr2 = (s_800F3D48_0_0*)func_800D88E8(curPtr);
                    curPtr->field_4.field_48(curPtr, ptr2);
                }

                func_800D9114(curPtr);
            }
        }
    }

    curPtr = D_800F3D48;
    for (i = 0; i < D_800F2438; i++, curPtr++)
    {
        if (curPtr->field_4.field_6 != 0)
        {
            curPtr->field_4.field_4 = 1;
            curPtr->field_4.field_6 = 0;

            if (curPtr->field_4.field_44 != NULL)
            {
                if (D_800F3D8C == 0)
                {
                    ptr1 = curPtr->field_4.field_44(curPtr);

                    if (ptr1 != NULL && curPtr->field_4.field_48 != NULL && D_800F3D90 == 0)
                    {
                        curPtr->field_4.field_48(curPtr, ptr1);
                    }
                }
                else if (curPtr->field_4.field_48 != NULL && D_800F3D90 == 0)
                {
                    // TODO: `func_800D88E8` returns different struct.
                    ptr2 = (s_800F3D48_0_0*)func_800D88E8(curPtr);
                    curPtr->field_4.field_48(curPtr, ptr2);
                }
            }
        }
    }
}

void func_800D9394(void) // 0x800D9394
{
    s_FsImageDesc image;

    image.tPage[0] = 0;
    image.tPage[1] = 55;
    image.u        = 0;
    image.v        = 0;
    image.clutX    = 80;
    image.clutY    = 0;
    Fs_QueueStartReadTim(FILE_TIM_ELAST_TIM, FS_BUFFER_1, &image);

    image.tPage[0] = 0;
    image.tPage[1] = 56;
    image.u        = 0;
    image.v        = 0;
    image.clutX    = 96;
    image.clutY    = 0;
    Fs_QueueStartReadTim(FILE_TIM_ELAST2_TIM, FS_BUFFER_1, &image);

    image.tPage[0] = 0;
    image.tPage[1] = 54;
    image.u        = 0;
    image.v        = 0;
    image.clutX    = 64;
    image.clutY    = 0;
    Fs_QueueStartReadTim(FILE_TIM_EF1_TIM, FS_BUFFER_1, &image);

    image.tPage[0] = 0;
    image.tPage[1] = 57;
    image.u        = 0;
    image.v        = 0;
    image.clutX    = 112;
    image.clutY    = 0;
    Fs_QueueStartReadTim(FILE_TIM_ELAST3_TIM, FS_BUFFER_1, &image);
}

void func_800D947C(void) // 0x800D947C
{
    s_800F3D48* ptr1;
    s32         i;
    s_800F3D58* ptr0;

    D_800F3D48 = (s_800F3D48*)FS_BUFFER_27;
    D_800F2438 = 80;

    ptr0 = &D_800F3D58;

    memset(D_800F3D48, 0xA7, 0x1900);

    ptr1 = D_800F3D48;

    for (i = 0; i < D_800F2438; i++, ptr1++)
    {
        ptr1->field_4.field_4 = 0;
        ptr1->field_4.field_6 = 0;
    }

    D_800F3D8C = 0;

    ptr0->field_0.vx = Q12(64.0f);
    ptr0->field_0.vy = Q12(0.0f);
    ptr0->field_0.vz = Q12(-64.0f);
}

void func_800D952C(void) // 0x800D952C
{
    s32         i;
    s_800F3D58* ptr0;
    s_800F3D48* ptr1;

    D_800F3D48 = &D_800F2448;
    D_800F2438 = 80;

    ptr0 = &D_800F3D58;

    memset(&D_800F2448, 0xA5, 0x1900);

    ptr1 = D_800F3D48;

    for (i = 0; i < D_800F2438; i++, ptr1++)
    {
        ptr1->field_4.field_4 = 0;
        ptr1->field_4.field_6 = 0;
    }

    D_800F3D8C = 0;

    ptr0->field_0.vx = Q12(64.0f);
    ptr0->field_0.vy = Q12(0.0f);
    ptr0->field_0.vz = Q12(-64.0f);
}

s_800F3D48_0* func_800D95D4(s_800F3D48* arg0) // 0x800D95D4
{
    s_800F3D48_0* result;

    arg0->field_4.field_18.vx += Q12_MULT_PRECISE(arg0->field_4.field_28.vx, g_DeltaTime);
    arg0->field_4.field_18.vy += Q12_MULT_PRECISE(arg0->field_4.field_28.vy, g_DeltaTime);
    arg0->field_4.field_18.vz += Q12_MULT_PRECISE(arg0->field_4.field_28.vz, g_DeltaTime);

    result = func_800D88E8(arg0);

    if (arg0->ptr_0->field_8 == 2)
    {
        arg0->field_4.field_4 = 0;
        arg0->field_4.field_6 = 0;
    }

    arg0->field_4.field_28.vx = Q12_MULT_FLOAT_PRECISE(arg0->field_4.field_28.vx, 0.8f);
    arg0->field_4.field_28.vy = Q12_MULT_FLOAT_PRECISE(arg0->field_4.field_28.vy, 0.8f);
    arg0->field_4.field_28.vz = Q12_MULT_FLOAT_PRECISE(arg0->field_4.field_28.vz, 0.8f);

    return result;
}

s_800F3D48_0* func_800D9740(s_800F3D48* arg0) // 0x800D9740
{
    s_800F3D48_0* ptr;

    arg0->field_4.field_18.vx += Q12_MULT_PRECISE(arg0->field_4.field_28.vx, g_DeltaTime);
    arg0->field_4.field_18.vy += Q12_MULT_PRECISE(arg0->field_4.field_28.vy, g_DeltaTime);
    arg0->field_4.field_18.vz += Q12_MULT_PRECISE(arg0->field_4.field_28.vz, g_DeltaTime);

    ptr = func_800D88E8(arg0);

    if (arg0->ptr_0->field_8 == 2)
    {
        arg0->field_4.field_4 = 0;
        arg0->field_4.field_6 = 0;
    }

    return ptr;
}

void func_800D982C(s_800F3D48* arg0) // 0x800D982C
{
    s32         rand;
    s32         temp;
    s_800F3D48* ptr;

    ptr = func_800D905C();

    if (ptr != NULL)
    {
        ptr->field_4.field_18 = arg0->field_4.field_18;
        rand                  = (Rng_Rand16() / 41) + 0x4CC;

        ptr->field_4.field_28.vx = Q12_MULT_PRECISE(arg0->field_4.field_28.vx, rand);
        ptr->field_4.field_28.vy = Q12_MULT_PRECISE(arg0->field_4.field_28.vy, rand);
        ptr->field_4.field_28.vz = Q12_MULT_PRECISE(arg0->field_4.field_28.vz, rand);

        temp = arg0->field_4.field_14;

        ptr->ptr_0 = &D_800EC5A8;

        ptr->field_4.field_44 = func_800D9740;
        ptr->field_4.field_48 = func_800D8D90;

        ptr->field_4.field_14 = temp;
        ptr->field_4.field_3C = arg0->field_4.field_3C;
        ptr->field_4.field_38 = arg0->field_4.field_38;
    }
}

void func_800D997C(s_800F3D48* arg0) // 0x800D997C
{
    q19_12      randAngle;
    s_800F3D48* ptr;

    ptr = func_800D905C();

    if (ptr != NULL)
    {
        ptr->field_4.field_3C    = arg0->field_4.field_3C;
        ptr->field_4.field_18    = arg0->field_4.field_18;
        ptr->field_4.field_18.vy = 0;

        randAngle = Rng_Rand16();

        ptr->field_4.field_28.vx = Q12_MULT_PRECISE(Math_Sin(randAngle), Q12(1.0f));
        ptr->field_4.field_28.vy = Q12(0.0f);
        ptr->field_4.field_28.vz = Q12_MULT_PRECISE(Math_Cos(randAngle), Q12(1.0f));

        ptr->field_4.field_14 = Q12_MULT_PRECISE(arg0->field_4.field_14, Q12(2.0f));

        ptr->ptr_0            = &D_800EC53C;
        ptr->field_4.field_44 = func_800D95D4;
        ptr->field_4.field_48 = func_800D8954;
    }
}

void func_800D9AA0(s_800F3D48* arg0) // 0x800D9AA0
{
    s32         temp;
    s_800F3D48* ptr;

    ptr = func_800D905C();

    if (ptr != NULL)
    {
        ptr->field_4.field_18 = arg0->field_4.field_18;
        ptr->field_4.field_3C = arg0->field_4.field_3C;

        temp = arg0->field_4.field_14;

        ptr->ptr_0 = &D_800EC614;

        ptr->field_4.field_28.vx = Q12(0.0f);
        ptr->field_4.field_28.vz = Q12(0.0f);

        ptr->field_4.field_44 = func_800D9740;
        ptr->field_4.field_48 = func_800D8954;
        ptr->field_4.field_14 = temp;

        ptr->field_4.field_18.vy += Q12_MULT_PRECISE(temp, Q12(-0.25f));
        ptr->field_4.field_28.vy  = Q12_MULT_PRECISE(temp, Q12(-1.0f));
    }
}

void func_800D9B90(s_800F3D48* arg0) // 0x800D9B90
{
    s32         temp;
    s32         temp2;
    s_800F3D48* ptr;

    ptr = func_800D905C();

    if (ptr != NULL)
    {
        ptr->field_4.field_18 = arg0->field_4.field_18;

        temp2 = arg0->field_4.field_14;
        temp  = arg0->field_4.field_3C;

        ptr->field_4.field_28.vx = Q12(0.0f);
        ptr->field_4.field_28.vz = Q12(0.0f);
        ptr->field_4.field_28.vy = Q12(1.0f);

        ptr->ptr_0            = &D_800EC53C;
        ptr->field_4.field_44 = func_800D9740;
        ptr->field_4.field_48 = func_800D8954;
        ptr->field_4.field_3C = temp;
        ptr->field_4.field_14 = temp2;
    }
}

s_800F3D48_0* func_800D9C20(s_800F3D48* arg0) // 0x800D9C20
{
    VECTOR3       sfxPos; // Q19.12
    s_800F3D48_0* ret;
    s_800F3D58*   ptr;

    ret = NULL;

    ptr = &D_800F3D58;

    switch (arg0->field_4.field_C)
    {
        case 0:
            arg0->ptr_0            = &D_800EC680[Rng_Rand16() / Q12(1.0f)];
            arg0->field_4.field_48 = func_800D8954;

            if (func_800D5D48() != false)
            {
                sfxPos.vx = arg0->field_4.field_18.vx + ptr->field_0.vx;
                sfxPos.vy = arg0->field_4.field_18.vy + ptr->field_0.vy;
                sfxPos.vz = arg0->field_4.field_18.vz + ptr->field_0.vz;
                Sfx_WithFlagsPlay(Sfx_Unk1692, &sfxPos, Q8_CLAMPED(1.0f), SfxFlag_None);
            }

            arg0->field_4.field_C++;

        case 1:
            ret = func_800D88E8(arg0);
            if (ret == &D_800EC34C)
            {
                func_800D9AA0(arg0);
            }

            if (Rng_Rand16() == 0)
            {
                func_800D9B90(arg0);
            }

            if (arg0->field_4.field_10 < Q12(0.0f))
            {
                arg0->field_4.field_10 = Q12(0.7f);
                arg0->field_4.field_C++;
            }
            break;

        case 2:
            ret                    = func_800D88E8(arg0);
            arg0->field_4.field_3C = Q12_MULT_PRECISE(arg0->field_4.field_3C, Q12(0.9f));
            if (arg0->field_4.field_10 < Q12(0.0f))
            {
                arg0->field_4.field_4 = 0;
                arg0->field_4.field_6 = 0;
            }
            break;
    }

    arg0->field_4.field_10 -= g_DeltaTime;
    return ret;
}

s_800F3D48_0* func_800D9DF8(s_800F3D48* arg0) // 0x800D9DF8
{
    s_CollisionSurface surface;
    SVECTOR            sp20;
    q19_12             floorHeight;
    s32                i;
    s_800F3D48_0*      ret;
    s_800F3D58*        ptr;

    ret = NULL;

    ptr = &D_800F3D58;

    switch (arg0->field_4.field_C)
    {
        case 0:
            sp20.vx = Q12_TO_Q8(arg0->field_4.field_18.vx);
            sp20.vy = Q12_TO_Q8(arg0->field_4.field_18.vy);
            sp20.vz = Q12_TO_Q8(arg0->field_4.field_18.vz);

            arg0->field_4.field_18.vx += Q12_MULT_PRECISE(arg0->field_4.field_28.vx, g_DeltaTime);
            arg0->field_4.field_18.vy += Q12_MULT_PRECISE(arg0->field_4.field_28.vy, g_DeltaTime);
            arg0->field_4.field_18.vz += Q12_MULT_PRECISE(arg0->field_4.field_28.vz, g_DeltaTime);

            if (arg0->field_4.field_18.vy > Q12(5.0f))
            {
                arg0->field_4.field_4 = 0;
                arg0->field_4.field_6 = 0;
            }
            else
            {
                PushMatrix();

                floorHeight = arg0->field_4.field_18.vy + ptr->field_0.vy;

                Collision_SurfaceGet(&surface, arg0->field_4.field_18.vx + ptr->field_0.vx,
                                     arg0->field_4.field_18.vz + ptr->field_0.vz);
                PopMatrix();

                if (surface.floorHeight < floorHeight)
                {
                    arg0->field_4.field_18.vy = surface.floorHeight;
                    arg0->field_4.field_48    = NULL;
                    arg0->field_4.field_10    = Q12(5.0f);
                    arg0->field_4.field_C++;

                    for (i = 0; i < 5; i++)
                    {
                        func_800D997C(arg0);
                    }
                }
            }

            ret = func_800D88E8(arg0);

            if (ret == &D_800EC3EC)
            {
                func_800D982C(arg0);
            }
            break;

        case 1:
            arg0->field_4.field_C  = 0;
            arg0->field_4.field_10 = Q12(5.0f);
            arg0->field_4.field_44 = func_800D9C20;
            arg0->field_4.field_14 = Q12_MULT_PRECISE(arg0->field_4.field_14, Q12(2.0f));
            break;
    }

    arg0->field_4.field_10 -= g_DeltaTime;
    return ret;
}

void func_800DA04C(void) // 0x800DA04C
{
    s_800F3D48* curPtr;
    s32         i;

    curPtr = D_800F3D48;
    for (i = 0; i < D_800F2438; i++, curPtr++)
    {
        curPtr->field_4.field_4 = 0;
        curPtr->field_4.field_6 = 0;
    }
}

q19_12 func_800DA08C(q19_12 arg0, q19_12 arg1, q19_12 arg2) // 0x800DA08C
{
    q19_12 scaledArg0;
    q19_12 scaledArg1;

    switch (arg2)
    {
        case 0:
            if (arg1 < arg0)
            {
                q19_12 temp = arg0;
                arg0        = arg1;
                arg1        = temp;
            }

            // TODO: Unsure what 0x6486 is meant to be.
            // 0x6486 = `Q12(6.2828f)`, while `sqrt(6.2828)` is 2.50655141579
            // `Q12(SQUARE(2.5f))` is very close, but gives 0x6400 instead of 0x6486...
            // Maybe 6.2828 is approximation of `PI * 2`? Doesn't work with our `PI` macro though.
            scaledArg0 = Q12_MULT_PRECISE(Q12_MULT_PRECISE(arg0, arg0), 0x6486);
            scaledArg1 = Q12_MULT_PRECISE(Q12_MULT_PRECISE(arg1, arg1), 0x6486);
            return scaledArg1 - scaledArg0;

        case 1:
            return Q12_MULT_PRECISE(arg0, arg1);

        default:
            return Q12(0.0f);
    }
}

q19_12 func_800DA178(q19_12 arg0, q19_12 arg1, q19_12 arg2, q19_12 arg3, q19_12 arg4) // 0x800DA178
{
    q19_12 temp_t0;

    if (arg2 < arg1)
    {
        temp_t0 = arg1;
        arg1    = arg2;
        arg2    = temp_t0;
    }

    temp_t0 = arg2 - arg1;

    arg0 = Q12_MULT_PRECISE(arg0, arg3);
    if (arg4 != Q12(0.0f) && temp_t0 != Q12(0.0f))
    {
        arg0 = Q12_MULT_PRECISE(arg0, temp_t0);
    }
    else
    {
        arg0 = Q12_MULT_PRECISE(arg0, g_DeltaTime);
    }
    return arg0;
}

s32 func_800DA1F4(VECTOR3* arg0, VECTOR3* arg1, q19_12 arg2, s32 arg3, s32 arg4) // 0x800DA1F4
{
    q19_12   temp_a0;
    s32      dist;
    s32      sinAngle;
    q19_12   randAngle;
    s32      var_fp;
    s32      temp_s0;
    VECTOR3* ptr;

    var_fp = 0x80;

    ptr = &D_800F3D58.field_0;

    switch (arg4)
    {
        case 0:
            temp_s0 = (arg3 - arg2) >> 3;
            temp_a0 = Rng_Rand16() / ((0x7FFF / temp_s0) + 1);
            temp_a0 = temp_a0 * 8;
            dist = arg2 + temp_a0;
            var_fp  = (Q12(10.0f) - temp_a0) / 320;

            randAngle = Rng_Rand16();

            sinAngle = Q12_MULT_PRECISE(Math_Sin(randAngle), dist);
            randAngle = Q12_MULT_PRECISE(Math_Cos(randAngle), dist);

            arg1->vx = (sinAngle + arg0->vx) - ptr->vx;
            arg1->vy = arg0->vy - ptr->vy;
            arg1->vz = (randAngle + arg0->vz) - ptr->vz;
            break;

        case 1:
            temp_a0 = Rng_Rand16() / (0x7FFF / arg2 + 1);
            sinAngle = temp_a0 - (arg2 / 2);
            temp_a0 = Rng_Rand16() / (0x7FFF / arg3 + 1);

            arg1->vx = (sinAngle + arg0->vx) - ptr->vx;
            arg1->vy = arg0->vy - ptr->vy;
            temp_a0  = temp_a0 - arg3 / 2;
            arg1->vz = (temp_a0 + arg0->vz) - ptr->vz;
            break;
    }

    return var_fp;
}

q19_12 func_800DA420(VECTOR3* result) // 0x800DA420
{
    q19_12 posX;
    q19_12 posY;

    posX = Q12_MULT_PRECISE(Math_Sin(Q12_ANGLE(15.0f)), Q12(5.0f));
    posY = Q12_MULT_PRECISE(Math_Cos(Q12_ANGLE(15.0f)), Q12(5.0f));

    result->vx = posX;
    result->vy = posY;
    result->vz = Q12(0.0f);
    return Q12_ANGLE(15.0f);
}

void func_800DA4B4(s32* arg0, s32* arg1) // 0x800DA4B4
{
    arg0[0] += (arg1[0] * arg0[1]) / arg1[1];
}

s32 func_800DA4EC(s32 min, s32 max) // 0x800DA4EC
{
    s32 range;

    if (max < min)
    {
        range = min;
        min   = max;
        max   = range;
    }

    range = max - min;
    min  += Rng_Rand16() / ((0x7FFF / range) + 1);
    return min;
}

void func_800DA550(s_800ED7E0_ptr* arg0) // 0x800DA550
{
    s32         temp_s4;
    s_800F3D48* ptr;

    ptr = func_800D905C();

    if (ptr == NULL)
    {
        return;
    }

    ptr->field_4.field_3C = func_800DA1F4(arg0, &ptr->field_4.field_18, arg0->field_C, arg0->field_10, arg0->field_24);
    ptr->field_4.field_14 = func_800DA4EC(arg0->field_1C, arg0->field_20);
    temp_s4               = func_800DA4EC(arg0->field_14, arg0->field_18);

    switch (arg0->field_28)
    {
        case 0:
            ptr->field_4.field_8  = arg0->field_2C;
            ptr->field_4.field_40 = arg0->field_30;
            ptr->field_4.field_38 = func_800DA420(&ptr->field_4.field_28);

            func_800DA4B4(&ptr->field_4.field_18, &ptr->field_4.field_28);

            ptr->ptr_0                = &D_800EC6EC;
            ptr->field_4.field_44     = func_800D9DF8;
            ptr->field_4.field_48     = func_800D8D90;
            ptr->field_4.field_18.vx += Q12_MULT_PRECISE(ptr->field_4.field_28.vx, temp_s4);
            ptr->field_4.field_18.vy += Q12_MULT_PRECISE(ptr->field_4.field_28.vy, temp_s4);
            break;

        case 1:
            ptr->field_4.field_14 *= 2;
            ptr->field_4.field_8   = arg0->field_2C;
            ptr->field_4.field_40  = arg0->field_30;
            ptr->ptr_0             = &D_800EC680[Rng_Rand16() / Q12(1.0f)];
            ptr->field_4.field_44  = func_800D9C20;
            ptr->field_4.field_10  = temp_s4;
            ptr->field_4.field_48  = func_800D8954;
            break;

        case 2:
            ptr->field_4.field_38 = func_800DA420(&ptr->field_4.field_28);
            func_800DA4B4(&ptr->field_4.field_18, &ptr->field_4.field_28);
            break;
    }
}

void func_800DA774(s_800ED7E0_ptr* arg0) // 0x800DA774
{
    s32 temp_v0;
    s32 i;
    s32 var_s0_2;
    s32 count;

    if (arg0->field_28 == 99)
    {
        Rng_SetSeed(0);
        func_800DA04C();
        return;
    }

    var_s0_2 = func_800DA178(func_800DA08C(arg0->field_C, arg0->field_10, arg0->field_24),
                             arg0->field_14, arg0->field_18, arg0->field_34, arg0->flags_38 & (1 << 1));

    count     = FP_FROM(var_s0_2, Q12_SHIFT);
    var_s0_2 %= Q12(1.0f);

    temp_v0  = Rng_Rand16();
    temp_v0 %= Q12(1.0f);

    if (var_s0_2 > temp_v0)
    {
        count++;
    }

    if (count > 50)
    {
        Rng_SetSeed(0);
    }

    for (i = 0; i < count; i++)
    {
        func_800DA550(arg0);
    }
}

s_800F3D48_0* func_800DA884(s_800F3D48* arg0) // 0x800DA884
{
    s_800F3D48_0* result;

    switch (arg0->field_4.field_C)
    {
        case 0:
            if (arg0->field_4.field_10 <= 0)
            {
                arg0->field_4.field_10 = 8;
                arg0->field_4.field_C++;
            }
            else
            {
                arg0->field_4.field_10 -= g_DeltaTime;
            }
            break;

        case 1:
            if (arg0->field_4.field_10 < 0)
            {
                arg0->field_4.field_40 = 10;
            }

            arg0->field_4.field_10--;

            arg0->field_4.field_18.vx += Q12_MULT_PRECISE(arg0->field_4.field_28.vx, g_DeltaTime);
            arg0->field_4.field_18.vy += Q12_MULT_PRECISE(arg0->field_4.field_28.vy, g_DeltaTime);
            arg0->field_4.field_18.vz += Q12_MULT_PRECISE(arg0->field_4.field_28.vz, g_DeltaTime);

            result = func_800D88E8(arg0);
            if (arg0->ptr_0->field_8 == 2)
            {
                arg0->field_4.field_4 = 0;
                arg0->field_4.field_6 = 0;
            }

            return result;
    }

    return NULL;
}

void func_800DA9F8(VECTOR3* arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) // 0x800DA9F8
{
    s_800F3D48_0* sp10[3];
    s_800F3D48*   ptr;
    VECTOR3*      vec;

    memcpy(sp10, D_800CAE30, sizeof(D_800CAE30));

    vec = &D_800F3D58.field_0;

    ptr = func_800D905C();

    if (ptr != NULL)
    {
        ptr->field_4.field_18.vx = arg0->vx - vec->vx;
        ptr->field_4.field_18.vy = arg0->vy - vec->vy;
        ptr->field_4.field_18.vz = arg0->vz - vec->vz;

        ptr->field_4.field_28.vx = 0;
        ptr->field_4.field_28.vy = 0;
        ptr->field_4.field_28.vz = 0;

        ptr->field_4.field_3C = 0x80;
        ptr->field_4.field_14 = arg4;

        ptr->ptr_0 = sp10[arg1];

        ptr->field_4.field_E  = arg2 << 6;
        ptr->field_4.field_44 = func_800DA884;
        ptr->field_4.field_10 = arg3;
        ptr->field_4.field_48 = func_800D8954;
    }
}

void func_800DAB18(VECTOR3* arg0, s32 arg1, s32 arg2, s32 arg3) // 0x800DAB18
{
    s_800F3D48* ptr;
    VECTOR3*    vec;

    vec = &D_800F3D58.field_0;

    ptr = func_800D905C();

    if (ptr != NULL)
    {
        ptr->field_4.field_18.vx = arg0->vx - vec->vx;
        ptr->field_4.field_18.vy = arg0->vy - vec->vy;
        ptr->field_4.field_18.vz = arg0->vz - vec->vz;
        ptr->field_4.field_3C    = 30;
        ptr->field_4.field_28.vx = 0;
        ptr->field_4.field_28.vy = 0;
        ptr->field_4.field_28.vz = 0;
        ptr->ptr_0               = &D_800EC1B8;
        ptr->field_4.field_14    = arg3;
        ptr->field_4.field_E     = arg1 << 6;
        ptr->field_4.field_10    = arg2;
        ptr->field_4.field_44    = func_800DA884;
        ptr->field_4.field_48    = func_800D8954;
    }
}

void func_800DAC04(VECTOR3* arg0, void* arg1, s32 arg2) // 0x800DAC04
{
    s32 i;
    s32 var_s4;

    if (Rng_Rand16() & 0x50)
    {
        var_s4 = 2;
    }
    else
    {
        var_s4 = 3;
    }

    for (i = 0; i < var_s4; i++)
    {
        func_800DA9F8(arg0, Rng_Rand16() / (Q12(2.667f) - 1), arg2, D_800EC758[i], D_800EC764[Rng_Rand16() / (Q12(2.667f) - 1)]);
    }
}

void func_800DACFC(VECTOR3* vec, s32 arg1, s32 arg2) // 0x800DACFC
{
    func_800DAB18(vec, arg2, 0, Q12(2.5f));
    func_800DAC04(vec, arg1, arg2);
}

void func_800DAD54(void) // 0x800DAD54
{
    s32              i;
    s_func_800DAD54* curPtr;
    VECTOR3*         vec;

    vec = &D_800F3D58.field_0;

    curPtr = (s_func_800DAD54*)FS_BUFFER_28;
    for (i = 0; i < 400; i++, curPtr++)
    {
        curPtr->field_28 = 0;
    }

    vec->vx = Q12(64.0f);
    vec->vy = Q12(0.0f);
    vec->vz = Q12(-64.0f);
}

s_func_800DAD54* func_800DAD90(void) // 0x800DAD90
{
    s32              i;
    s_func_800DAD54* curPtr;

    curPtr = (s_func_800DAD54*)FS_BUFFER_28;
    for (i = 0; i < 400; i++, curPtr++)
    {
        if (curPtr->field_28 == 0)
        {
            curPtr->field_4  = 0;
            curPtr->field_28 = 1;
            return curPtr;
        }
    }

    return NULL;
}

void func_800DADE0(s_func_800DAD54* arg0, s_800F3D48_0_0* arg1) // 0x800DADE0
{
    SVECTOR   sp10; // Q23.8
    DVECTOR   sp18[2];
    s32       flags;
    s16       x0;
    s16       y0;
    s32       offsetX;
    s32       offsetY;
    s32       temp_lo_4;
    s32       temp_lo_5;
    s32       temp_s1;
    s32       temp_s3;
    s32       temp_t0;
    s32       temp_v0;
    s32       temp;
    s32       temp2;
    s32       bufferIdx;
    s32       otIdx;
    s32       temp5;
    s32       temp6;
    GsOT_TAG* ot;
    POLY_FT4* poly;

    temp5   = 0x10101;
    temp_s3 = arg0->field_1C * temp5;

    sp10.vx = Q12_TO_Q8(arg0->field_C.vx);
    sp10.vy = Q12_TO_Q8(arg0->field_C.vy);
    sp10.vz = Q12_TO_Q8(arg0->field_C.vz);

    temp_s1 = RotTransPers(&sp10, &sp18[0], &sp18[1], &flags);

    if (flags & (1 << 17))
    {
        return;
    }

    temp_v0 = ReadGeomScreen();
    temp_t0 = arg0->field_8;

    offsetX = arg1->field_4;
    offsetY = arg1->field_6;

    offsetX = ((offsetX * temp_v0) / 4) / temp_s1;
    offsetY = ((offsetY * temp_v0) / 4) / temp_s1;

    temp_lo_4 = ((arg1->field_C / 4) * temp_v0) / temp_s1;
    temp_lo_5 = ((arg1->field_E / 4) * temp_v0) / temp_s1;

    offsetX = Q12_MULT_PRECISE(offsetX, temp_t0);
    offsetY = Q12_MULT_PRECISE(offsetY, temp_t0);

    temp  = Q12_MULT_PRECISE(temp_lo_4, temp_t0);
    temp2 = Q12_MULT_PRECISE(temp_lo_5, temp_t0);

    temp6 = sp18[0].vy;
    x0    = sp18[0].vx - temp;
    y0    = temp6 - temp2;

    if (x0 < (-offsetX - 160) || x0 > 160 ||
        y0 < (-offsetY - 160) || y0 > 112)
    {
        return;
    }

    poly = GsOUT_PACKET_P;

    setXY4(poly,
           x0, y0,
           x0 + offsetX, y0,
           x0, y0 + offsetY,
           x0 + offsetX, y0 + offsetY);

    setUV4(poly, arg1->field_0, arg1->field_2, arg1->field_0 + arg1->field_4, arg1->field_2,
           arg1->field_0, arg1->field_2 + arg1->field_6, arg1->field_0 + arg1->field_4, arg1->field_2 + arg1->field_6);

    *(s32*)&poly->r0 = temp_s3;
    poly->tpage      = arg1->field_8;
    poly->clut       = arg1->field_A;

    setPolyFT4(poly);
    setSemiTrans(poly, 1);

    bufferIdx = g_ActiveBufferIdx;
    ot        = g_OrderingTable0[bufferIdx].org;
    otIdx     = temp_s1 >> 1;
    ot        = &ot[otIdx];

    addPrim(ot, poly);
    poly++;

    GsOUT_PACKET_P = poly;
}

void func_800DB154(GsCOORDINATE2* boneCoords) // 0x800DB154
{
    MATRIX           boneMat;
    VECTOR           boneOffsets[HarryBone_Count];
    s32              i;
    s32              coordCount;
    GsCOORDINATE2*   curBoneCoord;
    s_func_800DAD54* curPtr;
    VECTOR*          boneOffset;
    VECTOR3*         pos;

    curPtr = (s_func_800DAD54*)FS_BUFFER_28;
    coordCount = HarryBone_Count - 1;

    pos = &D_800F3D58.field_0;

    curBoneCoord = boneCoords;
    curBoneCoord++;

    // Get bone offsets.
    for (i = 1; i <= coordCount; i++, curBoneCoord++)
    {
        Vw_CoordHierarchyMatrixCompute(curBoneCoord, &boneMat);

        boneOffsets[i].vx = Q8_TO_Q12(boneMat.t[0]) - pos->vx;
        boneOffsets[i].vy = Q8_TO_Q12(boneMat.t[1]) - pos->vy;
        boneOffsets[i].vz = Q8_TO_Q12(boneMat.t[2]) - pos->vz;
    }

    func_800D90C8();

    for (i = 0; i < 400; i++, curPtr++)
    {
        if (curPtr->field_28 != 0)
        {
            boneOffset         = &boneOffsets[curPtr->field_24];
            curPtr->field_C.vx = boneOffset->vx;
            curPtr->field_C.vy = boneOffset->vy;
            curPtr->field_C.vz = boneOffset->vz;

            // TODO: `func_800D88E8` returns different struct.
            func_800DADE0(curPtr, (s_800F3D48_0_0*)func_800D88E8(curPtr));
        }
    }
}

void func_800DB288(void) // 0x800DB288
{
    s32              i;
    s32              rand;
    s_func_800DAD54* ptr;

    for (i = 1; i < 18; i++)
    {
        ptr = func_800DAD90();

        if (ptr != NULL)
        {
            rand = Rng_Rand16() / Q12(1.0f);

            ptr->field_0 = &D_800EC680[rand];

            rand = Rng_Rand16() / 16;

            ptr->field_8  = rand + Q12(0.5f);
            ptr->field_1C = 0x60;
            ptr->field_24 = i;
        }
    }
}

#include "../src/maps/common/unk_m3s03.c" // 0x800DB338

void func_800DB608(void) // 0x800DB608
{
    s_D_800F48A8* ptr;
    q19_12        speed;

    ptr = &D_800F48A8;

    if (g_DeltaTime != Q12(0.0f))
    {
        speed = (g_SavegamePtr->gameDifficulty == GameDifficulty_Easy) ? Q12(0.2f) : Q12(0.7f);

        // Velocity value based on previously saved player position?
        ptr->velocityX_3C      = ((g_SysWork.playerWork.player.position.vx - ptr->playerPosition_30.vx) * speed) / g_DeltaTime;
        ptr->velocityZ_40      = ((g_SysWork.playerWork.player.position.vz - ptr->playerPosition_30.vz) * speed) / g_DeltaTime;
        ptr->playerPosition_30 = g_SysWork.playerWork.player.position;
    }
    else
    {
        ptr->velocityX_3C = Q12(0.0f);
        ptr->velocityZ_40 = Q12(0.0f);
    }
}

void func_800DB6D0(MATRIX* arg0, const VECTOR* rot0, const VECTOR* rot1, const MATRIX* mat, s32 arg4) // 0x800DB6D0
{
    MATRIX  sp10;
    MATRIX  sp30;
    SVECTOR sp50;
    s32     temp_a3;
    s32     sp5C;
    s32     temp_a0;
    s32     sp64;
    s32     sp68;
    s32     sp6C;
    s32     var_s4;
    s32     var_s5;
    q19_12  angle0;
    q19_12  angle1;
    s32     temp_s6;
    s32     temp_s2;
    q19_12  dist;
    q19_12  angle2;
    s32     var_fp;
    s32     var_s1;
    q19_12  absAngle2;

    angle1 = arg4;

    if (mat != NULL)
    {
        var_s4 = -mat->m[2][0];
        var_s5 = mat->m[0][0];
        sp68   = -mat->m[1][2];
        sp6C   = mat->m[1][1];
    }
    else
    {
        var_s4 = 0;
        var_s5 = Q12(1.0f);
        sp68   = 0;
        sp6C   = Q12(1.0f);
    }

    angle2 = rot1->vx;
    var_s1  = angle2 - rot0->vx;
    temp_s6 = rot1->vy - rot0->vy;
    angle0  = rot1->vz - rot0->vz;

    temp_a3 = Q12_MULT(var_s1, var_s1);
    temp_a0 = Q12_MULT(angle0, angle0);
    sp5C    = Q12_MULT(temp_s6, temp_s6);

    temp_s2 = SquareRoot12(temp_a3 + temp_a0);

    if (temp_s2 != 0)
    {
        sp64   = Q12_DIV(var_s1, temp_s2);
        var_fp = Q12_DIV(angle0, temp_s2);
    }
    else
    {
        sp64   = var_s4;
        var_fp = var_s5;
    }

    if (mat != NULL)
    {
        angle0 = ratan2(var_s4, var_s5);
        angle2 = ratan2(sp64, var_fp) - angle0;
        angle2 = Q12_ANGLE_NORM_S(angle2);
        absAngle2  = ABS(angle2);

        if (angle1 < absAngle2)
        {
            if (angle2 <= Q12_ANGLE(0.0f))
            {
                angle0 = angle0 - angle1;
            }
            else
            {
                angle0 = angle0 + angle1;
            }

            sp64   = Math_Sin(angle0);
            var_fp = Math_Cos(angle0);
        }
    }

    dist = SquareRoot12(sp5C + temp_a3 + temp_a0);
    if (dist != Q12(0.0f))
    {
        var_s4 = -FP_TO(temp_s6, Q12_SHIFT) / dist; // TODO: `Q12_DIV` doesn't match.
        var_s1 = Q12_DIV(temp_s2, dist);
    }
    else
    {
        var_s4 = sp68;
        var_s1 = sp6C;
    }

    angle1 = angle1 / 2;

    if (mat != NULL)
    {
        angle0  = ratan2(sp68, sp6C);
        angle2 = ratan2(var_s4, var_s1) - angle0;
        angle2 = Q12_ANGLE_NORM_S(angle2);
        absAngle2   = ABS(angle2);

        if (angle1 < absAngle2)
        {
            if (angle2 <= Q12_ANGLE(0.0f))
            {
                angle0 = angle0 - angle1;
            }
            else
            {
                angle0 = angle0 + angle1;
            }

            var_s4 = Math_Sin(angle0);
            var_s1 = Math_Cos(angle0);
        }
    }

    vwGetViewAngle(&sp50);

    sp10.m[0][0] = Q12(1.0f);
    sp10.m[0][1] = 0;
    sp10.m[0][2] = 0;

    sp10.m[1][0] = 0;
    sp10.m[1][1] = Q12(1.0f);
    sp10.m[1][2] = 0;

    sp10.m[2][0] = 0;
    sp10.m[2][1] = 0;
    sp10.m[2][2] = Q12(1.0f);

    Math_RotMatrixZ(0, &sp10);
    MulMatrix(arg0, &sp10);

    sp30.m[0][0] = var_fp;
    sp30.m[0][1] = 0;
    sp30.m[0][2] = sp64;

    sp30.m[1][0] = 0;
    sp30.m[1][1] = Q12(1.0f);
    sp30.m[1][2] = 0;

    sp30.m[2][0] = -sp64;
    sp30.m[2][1] = 0;
    sp30.m[2][2] = var_fp;

    sp10.m[0][0] = Q12(1.0f);
    sp10.m[0][1] = 0;
    sp10.m[0][2] = 0;

    sp10.m[1][0] = 0;
    sp10.m[1][1] = var_s1;
    sp10.m[1][2] = -var_s4;

    sp10.m[2][0] = 0;
    sp10.m[2][1] = var_s4;
    sp10.m[2][2] = var_s1;

    MulMatrix0(&sp30, &sp10, arg0);
}

s32 func_800DBA08(SVECTOR* arg0, s32* arg1, s32* arg2) // 0x800DBA08
{
    s32 sp10;
    s32 sp14;
    s32 temp_s4;
    s32 temp_s0;

    temp_s4 = RotTransPers(arg0, arg1, &sp10, &sp14);

    if (sp14 & (1 << 17))
    {
        return 0;
    }

    arg0++;
    temp_s0 = RotTransPers(arg0, arg2, &sp10, &sp14);

    if (sp14 & (1 << 17))
    {
        return 0;
    }

    return (temp_s4 + temp_s0) / 2;
}

void func_800DBABC(void) // 0x800DBABC
{
    q19_12 val;
    s32    i;

    val = Q12(1.0f);
    for (i = 4; i >= 0; i--)
    {
        D_800F3D98[i] = val;
    }
}

void func_800DBAE8(const VECTOR3* pos, s32 idx) // 0x800DBAE8
{
    s32 temp_s0;

    temp_s0 = D_800EC770[idx].field_6;
    if (temp_s0 < D_800F3D98[idx])
    {
        temp_s0 = D_800EC770[idx].field_4;

        D_800EC770[idx].field_6 = temp_s0 + (Rng_Rand16() / ((SHRT_MAX / (temp_s0 >> 3)) + 1));
        D_800F3D98[idx]         = 0;

        Sfx_WithFlagsPlay(D_800EC770[idx].sfxId_0, pos, D_800EC770[idx].sfxVol_2, SfxFlag_None);
    }
}

void func_800DBBA0(void) // 0x800DBBA0
{
    q19_12 deltaTime;
    s32    i;

    deltaTime = g_DeltaTime;
    for (i = 0; i < ARRAY_SIZE(D_800F3D98); i++)
    {
        D_800F3D98[i] += deltaTime;
    }
}

void func_800DBBD8(MATRIX* mat) // 0x800DBBD8
{
    MATRIX        transformMat;
    s_D_800F48A8* base;

    base = &D_800F48A8;

    CompMatrix(&base->mat_8, mat, &transformMat);
    SetRotMatrix(&transformMat);
    SetTransMatrix(&transformMat);
}

void func_800DBC18(q19_12 damageAmount) // 0x800DBC18
{
    s32 i;

    g_SysWork.playerWork.player.attackReceived = 67; // TODO: What weapon attack?

    if (D_800F3DB4 > Q12(0.8f))
    {
        D_800F3DB0 = 1;
        D_800F3DB4 = 0;
    }
    else
    {
        D_800F3DB4 = 0;
        D_800F3DB0++;
    }

    for (i = 1; i < D_800F3DB0; i++)
    {
        damageAmount >>= 1;
    }

    g_SysWork.playerWork.player.damage.amount += damageAmount;
}

bool func_800DBCA4(MATRIX* mat, VECTOR3* outVec) // 0x800DBCA4
{
    s_D_800F48A8* ptr;
    MATRIX        torsoMat;
    q23_8         deltaX;
    q23_8         deltaY;
    q23_8         deltaZ;
    q23_8         sqrX;
    q23_8         sqrY;
    q23_8         sqrZ;

    ptr = &D_800F48A8;

    Vw_CoordHierarchyMatrixCompute(&g_SysWork.playerBoneCoords[HarryBone_Torso], &torsoMat);
    deltaX = torsoMat.t[0] - Q12_TO_Q8(ptr->positionX);
    deltaY = torsoMat.t[1];
    deltaZ = torsoMat.t[2] - Q12_TO_Q8(ptr->positionZ);

    sqrX = Q12_TO_Q8(SQUARE(deltaX - mat->t[0]));
    sqrY = Q12_TO_Q8(SQUARE(deltaY - mat->t[1]));
    sqrZ = Q12_TO_Q8(SQUARE(deltaZ - mat->t[2]));

    if ((sqrX + sqrZ + sqrY) <= Q8(SQUARE(2.398f))) // TODO: Odd number, might be different Q format?
    {
        outVec->vx = Q8_TO_Q12(deltaX) + D_800F48A8.positionX;
        outVec->vy = Q8_TO_Q12(deltaY);
        outVec->vz = Q8_TO_Q12(deltaZ) + D_800F48A8.positionZ;
        return true;
    }

    return false;
}

void func_800DBD94(s_800F3DAC* arg0, GsOT_TAG* ot) // 0x800DBD94
{
    SVECTOR       sp10;
    SVECTOR       sp18;
    VECTOR3       sp20;
    s32           sp30;
    s32           sp34;
    s32           sp38;
    s32           sp3C;
    s32           sp40;
    s32*          sp44;
    q19_12        dist;
    u16           sp4C;
    GsOT_TAG*     temp_a0_3;
    MATRIX*       var_s7;
    q3_12         angle1;
    q19_12        angle0;
    s32           temp_v1;
    s32           var_a2;
    s32           i;
    q19_12        angle2;
    s32           var_s5;
    s32           var_s6;
    s32*          var_s3;
    s32           var_a0;
    POLY_FT4*     poly;
    PACKET*       packet;
    s_D_800F48A8* ptr;

    sp4C = 67;
    dist = Q12_MULT_PRECISE(arg0->rotationZ_4D8, Q12(0.9f));

    if (g_SysWork.npcs[2].model.charaId == Chara_Incubus)
    {
        sp4C = 3;
    }

    sp44   = arg0->field_A0;
    var_s7 = arg0->mat_118;
    var_s3 = arg0->field_28;

    sp38 = 0x80808;
    sp40 = arg0->field_20;

    for (i = 0; i < arg0->field_1C; i++, var_s7++, var_s3++, sp44++);

    angle2 = *sp44;

    sp10.vx = Q12_TO_Q8(Q12_MULT_PRECISE(Math_Sin(angle2), dist));
    sp10.vy = Q12_TO_Q8(Q12_MULT_PRECISE(Math_Cos(angle2), dist));
    angle0 = angle2 + Q12_ANGLE(90.0f);
    sp18.vx = Q12_TO_Q8(Q12_MULT_PRECISE(Math_Sin(angle0), dist));

    var_s3++;
    i++;
    sp44++;

    sp18.vy = Q12_TO_Q8(Q12_MULT_PRECISE(Math_Cos(angle0), dist));
    sp18.vz = Q8(0.0f);
    sp10.vz = Q8(0.0f);

    func_800DBBD8(var_s7++);

    var_a2 = func_800DBA08(&sp10, &sp30, &sp34);

    packet = GsOUT_PACKET_P;
    poly   = packet;
    ptr    = &D_800F48A8;

    for (; i < sp40;
         i++, var_s7++, var_s3++, sp44++)
    {
        var_a0 = angle2;
        angle2 = *sp44;
        sp3C   = var_a2;

        do
        {
            angle1 = Math_AngleNormalizeSigned(angle2 - var_a0);
        }
        while (false); // @hack

        angle0 = angle1;

        if (ABS(angle1) < Q12_ANGLE(90.0f))
        {
            var_s6 = sp30;
            var_s5 = sp34;
        }
        else
        {
            var_s6 = sp34;
            var_s5 = sp30;
        }

        sp10.vx = Q12_TO_Q8(Q12_MULT_PRECISE(Math_Sin(angle2), dist));
        sp10.vy = Q12_TO_Q8(Q12_MULT_PRECISE(Math_Cos(angle2), dist));
        angle0 = angle2 + Q12_ANGLE(90.0f);
        sp18.vx = Q12_TO_Q8(Q12_MULT_PRECISE(Math_Sin(angle0), dist));
        sp18.vy = Q12_TO_Q8(Q12_MULT_PRECISE(Math_Cos(angle0), dist));
        sp18.vz = Q8(0.0f);
        sp10.vz = Q8(0.0f);

        if (arg0->field_4E8 == 0 && func_800DBCA4(var_s7, &sp20) &&
            g_DeltaTime != Q12(0.0f) && ptr->field_44 == 0)
        {
            if (ptr->field_48 != 2)
            {
                if (ptr->field_48 == 0)
                {
                    func_800DBC18(Q12(30.0f));
                    func_800DACFC(&sp20, 0, 0);
                    func_800DBAE8(&g_SysWork.playerWork.player.position, 1);
                }
                else
                {
                    func_800DBC18(Q12(20.0f));
                    func_800DACFC(&sp20, 0, 1);
                }

                func_800DBAE8(&g_SysWork.playerWork.player.position, 3);
            }

            if (i < arg0->field_20)
            {
                sp40 = i;
                if (i < arg0->field_18)
                {
                    sp40 = i + 1;
                }

                arg0->field_18 = sp40;
                arg0->field_20 = sp40;
            }

            arg0->field_4E8 = 1;
        }

        func_800DBBD8(var_s7);

        var_a2 = func_800DBA08(&sp10, &sp30, &sp34);

        if (var_a2 == 0 || sp3C == 0)
        {
            continue;
        }

        if ((u16)var_s6 > 320 && (u32)(var_s6 >> 16) > 224 &&
            (u16)var_s5 > 320 && (u32)(var_s5 >> 16) > 224)
        {
            // @hack Some optimized out code?
            temp_v1 = sp30;
            temp_v1++;
            temp_v1--;
            sp30 = temp_v1;
        }

        if (*var_s3 == 0)
        {
            continue;
        }

        *(s32*)&poly->x0 = var_s6;
        *(s32*)&poly->x1 = var_s5;
        *(s32*)&poly->x2 = sp30;
        *(s32*)&poly->x3 = sp34;

        var_a0  = (i & 1) ? 0 : 0x7F;
        temp_v1 = 0x40;

        setUV4(poly, 0, var_a0, temp_v1, var_a0, 0, var_a0 + 127, temp_v1, var_a0 + 127);

        poly->tpage = 53;
        poly->clut  = sp4C;

        if (g_DeltaTime != Q12(0.0f))
        {
            *var_s3 -= sp38;
        }

        *(s32*)&poly->r0 = *var_s3;

        setPolyFT4(poly);
        setSemiTrans(poly, 1);

        temp_a0_3 = &ot[((sp3C + var_a2) / 2) >> 1];
        addPrim(temp_a0_3, poly);
        poly++;
    }

    packet         = poly;
    GsOUT_PACKET_P = packet;
}

void func_800DC3EC(s_800F3DAC* arg0) // 0x800DC3EC
{
    s32 valMax;
    s32 field20Val;
    s32 field1CVal;

    // TODO: Might be using some kind of `CLAMP` in this func, couldn't get match with our macros though.
    valMax = arg0->field_18;

    field20Val = ((arg0->timer_C - arg0->timer) * 30) / (arg0->timer_C / 2);
    // TODO: `field20Val = MIN(maxVal, field20Val)`?
    if (field20Val >= valMax)
    {
        field20Val = valMax;
    }
    arg0->field_20 = field20Val;

    if (arg0->timer < (arg0->timer_C / 2))
    {
        field1CVal = (((arg0->timer_C / 2) - arg0->timer) * 30) / (arg0->timer_C / 2);
        // TODO: `field1CVal = MIN(maxVal, field1CVal)`?
        if (field1CVal >= valMax)
        {
            field1CVal = valMax;
        }

        field1CVal     = MAX(0, field1CVal);
        arg0->field_1C = field1CVal;
    }
}

void func_800DC49C(s_800F3DAC* arg0) // 0x800DC49C
{
    VECTOR3 pos;
    bool    cond;
    MATRIX* mat;

    cond = D_800F48A8.field_48 != 0;

    if (arg0->field_4F4 == 0)
    {
        if (arg0->field_18 == arg0->field_20)
        {
            mat = &arg0->mat_118[arg0->field_18 - 1];
            Math_Vector3Set(&pos,
                            Q8_TO_Q12(mat->t[0]) + D_800F48A8.positionX,
                            Q8_TO_Q12(mat->t[1]),
                            Q8_TO_Q12(mat->t[2]) + D_800F48A8.positionZ);
            func_800DAC04(&pos, NULL, cond);
            arg0->field_4F4++;
        }
    }
}

void func_800DC544(GsOT_TAG* ot) // 0x800DC544
{
    s32         i;
    s_800F3DAC* ptr;

    ptr = D_800F3DAC;

    for (i = 0; i < 30; i++, ptr++)
    {
        if (ptr->field_0 == 0)
        {
            continue;
        }

        if (ptr->timer > Q12(0.0f) || ptr->field_1C != ptr->field_18)
        {
            func_800DC3EC(ptr);

            if (ptr->field_4F0)
            {
                ptr->field_4F0(ptr);
            }

            func_800DBD94(ptr, ot);
            ptr->timer -= g_DeltaTime;
        }
        else
        {
            ptr->field_0 = 0;
            func_800DD4CC(ptr);
        }
    }

    D_800F3DB4 += g_DeltaTime;

    func_800DBBA0();
}

bool Math_DistanceCheck(const VECTOR3* from, const VECTOR3* to, q19_12 distMax) // 0x800DC650
{
    q19_12 offsetX;
    q19_12 offsetY;
    q19_12 offsetZ;

    // Compute offset.
    offsetX = to->vx - from->vx;
    offsetY = to->vy - from->vy;
    offsetZ = to->vz - from->vz;

    // Compute square offset.
    offsetX = Q12_SQUARE(offsetX);
    offsetY = Q12_SQUARE(offsetY);
    offsetZ = Q12_SQUARE(offsetZ);

    // Check offset distance.
    return distMax < abs(SquareRoot12(offsetX + offsetZ + offsetY));
}

extern s_800F3D48_0 D_800EC224;
extern s_800F3D48_0 D_800EC26C;
extern s_800F3D48_0 D_800EC2B4;

s_800F3D48_0* const D_800CAE30[3] = { &D_800EC224, &D_800EC26C, &D_800EC2B4 };

static const SVECTOR D_800CAE3C = { 0, 0, 0x4000 };

bool func_800DC6DC(VECTOR* arg0, VECTOR* arg1, VECTOR3* arg2, s_800F3DAC* arg3) // 0x800DC6DC
{
    VECTOR  sp10;
    SVECTOR sp20;
    SVECTOR sp28;
    MATRIX  sp30;
    q19_12  dx;
    q19_12  dy;
    q19_12  dz;
    s32     dy_2;
    q19_12  randAngle;
    s32     temp_v1;
    q19_12  dist;

    if (arg3->field_10 == 0)
    {
        arg3->field_4E0 = 0;

        switch (arg3->field_4E4)
        {
            case 3:
                arg3->field_10 = 18;
                break;

            case 4:
                arg3->field_10 = 19;
                break;

            case 6:
                arg3->field_10 = 8;
                break;

            case 2:
                arg3->field_10 = 2;
                break;

            case 7:
                arg3->field_10 = 18;
                break;

            default:
                arg3->field_14 = 170;
                arg3->field_10++;
                break;
        }
    }

loop:
    switch (arg3->field_10)
    {
        case 1:
            if (!(Rng_Rand16() & 0xF0))
            {
                arg3->rotationZ_4D8  = Q12(0.2f);
                arg3->field_10  = 2;
                arg3->field_4E0 = 0;
            }
            else if (!(Rng_Rand16() & 0xF0))
            {
                arg3->field_10  = 14;
                arg3->field_4E0 = 0;
            }
            else if (arg0->vy <= Q12(0.5f))
            {
                arg3->field_10  = 8;
                arg3->field_4E0 = 0;
            }
            else if (!(Rng_Rand16() & 0x80))
            {
                arg3->field_10  = 6;
                arg3->field_4E0 = 0;
            }
            break;

        case 2:
            *arg1          = D_800EC798[(Rng_Rand16() >> 4) % 6];
            arg3->field_10 = 3;

        case 3:
            if (Math_DistanceCheck(arg0, arg1, Q12(0.3f)))
            {
                arg3->field_4E0++;
            }
            else
            {
                arg3->field_4E0 = 0;
                arg3->field_10  = 4;
            }
            break;

        case 4:
            arg1->vy = 0;
            arg3->field_10++;

        case 5:
            if (arg0->vy < 0)
            {
                arg3->field_4E0++;
                break;
            }

            arg3->field_4E0 = 0;
            arg3->field_4E4 = 5;
            return false;

        case 6:
            randAngle = Rng_Rand16();
            arg1->vx  = Math_Sin(randAngle);
            arg1->vy  = Q12(0.0f);
            arg1->vz  = Math_Cos(randAngle);
            arg3->field_10++;

        case 7:
            if (arg3->field_4E0 >= 4)
            {
                arg3->field_4E0 = 0;
                arg3->field_4E4 = 0;
                return false;
            }

            arg3->field_4E0++;
            break;

        case 8:
            dx = arg2->vx - arg0->vx;
            dx = FP_SQUARE(dx, Q12_SHIFT);

            dy = arg2->vy - arg0->vy;
            dy = FP_SQUARE(dy, Q12_SHIFT);

            dz = arg2->vz - arg0->vz;
            dz = FP_SQUARE(dz, Q12_SHIFT);

            dist = SquareRoot12(dx + dz + dy);

            dx = arg2->vx - arg0->vx;
            dy = Q12(-3.8f) - (dist / 4);
            dz = arg2->vz - arg0->vz;

            randAngle   = Rng_Rand16();
            arg1->vx  = arg0->vx + (dx / 2) - 0xFFF;
            arg1->vx += randAngle >> 2;

            randAngle   = Rng_Rand16();
            arg1->vy  = dy - 0xFFF;
            arg1->vy += randAngle >> 2;

            randAngle   = Rng_Rand16();
            arg1->vz  = arg0->vz + (dz / 2) - 0xFFF;
            arg1->vz += randAngle >> 2;

            arg3->field_10++;

        case 9:
            if (Math_DistanceCheck(arg0, arg1, arg3->rotationZ_4D8))
            {
                arg3->field_4E0++;
                break;
            }

            arg3->field_10  = 10;
            arg3->field_4E0 = 0;
            break;

        case 10:
            dx   = arg2->vx - arg0->vx;
            dy_2 = arg2->vz - arg0->vz;

            randAngle  = Rng_Rand16();
            temp_v1  = arg2->vx - 0x7FF;
            temp_v1 += randAngle >> 3;
            arg1->vx = temp_v1 - (dx / 8);

            randAngle        = Rng_Rand16();
            arg1->vy       = (randAngle >> 3) - (Q12(4.3f) - 1);
            randAngle        = Rng_Rand16();
            temp_v1        = arg2->vz - 0x7FF;
            temp_v1       += randAngle >> 3;
            arg1->vz       = temp_v1 - (dy_2 / 8);
            arg3->field_10 = 11;

        case 11:
            if (Math_DistanceCheck(arg0, arg1, Q12_MULT_PRECISE(arg3->rotationZ_4D8, Q12(1.5f))))
            {
                arg3->field_4E0++;
                break;
            }

            arg3->field_10  = 12;
            arg3->field_4E0 = 0;
            break;

        case 12:
            randAngle      = Rng_Rand16();
            temp_v1        = arg2->vx;
            temp_v1       += -0x7FF;
            temp_v1       += randAngle >> 3;
            arg1->vx       = temp_v1;
            arg1->vy       = 0;

            randAngle      = Rng_Rand16();
            temp_v1        = arg2->vz;
            temp_v1       += -0x7FF;
            temp_v1       += randAngle >> 3;
            arg1->vz       = temp_v1;
            arg3->field_10 = 11;

        case 13:
            if (!Math_DistanceCheck(arg0, arg1, arg3->rotationZ_4D8))
            {
                arg3->field_4E0 = 0;
                return false;
            }

            arg3->field_4E0++;
            break;

        case 14:
            sp28    = D_800CAE3C;
            sp20.vx = Rng_Rand16();
            sp20.vy = Rng_Rand16();
            sp20.vz = Q12(0.0f);

            Math_RotMatrixXyz(&sp20, &sp30);
            SetRotMatrix(&sp30);
            ApplyRotMatrix(&sp28, &sp10);

            arg1->vx = arg0->vx + sp10.vx;
            arg1->vy = arg0->vy + sp10.vy;
            arg1->vz = arg0->vz + sp10.vz;

        case 15:
            if (arg3->field_4E0 == 0)
            {
                arg3->field_4E0 = 1;
                break;
            }

            if (Rng_Rand16() & 0xF0)
            {
                break;
            }

            arg3->field_4E0 = 0;
            arg3->field_10  = 16;
            break;

        case 16:
            *arg1 = D_800EC798[6];
            arg3->field_10++;

        case 17:
            if (Math_DistanceCheck(arg0, arg1, arg3->rotationZ_4D8))
            {
                arg3->field_4E0++;
                break;
            }

            arg3->field_4E0 = 0;
            arg3->field_4E4 = 0;
            return 0;

        default:
            arg3->field_4E0++;
            break;

        case 18:
            if (!Math_DistanceCheck(arg0, arg1, arg3->rotationZ_4D8 / 2))
            {
                arg3->field_4E0 = 0;
                return false;
            }

            arg3->field_4E0++;
            break;

        case 19:
            if (arg3->field_4E0 >= 25)
            {
                arg3->field_4E0 = 0;
                return false;
            }

            arg3->field_4E0++;
            break;
    }

    if (arg3->field_4E0 == 0)
    {
        goto loop;
    }

    return true;
}

void func_800DCD94(MATRIX* mat, VECTOR3* pos) // 0x800DCD94
{
    mat->t[0] = Q12_TO_Q8(pos->vx - D_800F48A8.positionX);
    mat->t[1] = Q12_TO_Q8(pos->vy);
    mat->t[2] = Q12_TO_Q8(pos->vz - D_800F48A8.positionZ);
}

void func_800DCDDC(s_800F3DAC* arg0, const VECTOR3* arg1, const VECTOR3* arg2) // 0x800DCDDC
{
    VECTOR  sp18;
    VECTOR  sp28;
    SVECTOR sp38;
    VECTOR  sp40;
    s32     cond;
    s32     rand;
    s32     i;
    s32     randSum;
    MATRIX* mat;

    randSum = 0;
    mat     = &arg0->mat_118[0];

    sp18.vx = arg1->vx;
    sp18.vy = arg1->vy;
    sp18.vz = arg1->vz;

    if (arg2 != NULL)
    {
        sp28.vx = arg2->vx;
        sp28.vy = arg2->vy;
        sp28.vz = arg2->vz;
    }

    for (i = 0; i < ARRAY_SIZE(arg0->mat_118); i++, mat++)
    {
        cond = func_800DC6DC(&sp18, &sp28, arg2, arg0);
        if (i == 0)
        {
            func_800DB6D0(mat, &sp18, &sp28, NULL, 0);
        }
        else
        {
            arg0->field_14++; // @hack Needed for reg match.
            arg0->field_14--;
            func_800DB6D0(mat, &sp18, &sp28, mat - 1, arg0->field_14);
        }

        SetRotMatrix(mat);
        func_800DCD94(mat, &sp18);

        sp38.vx = 0;
        sp38.vy = 0;
        sp38.vz = arg0->rotationZ_4D8;
        ApplyRotMatrix(&sp38, &sp40);

        sp18.vx += sp40.vx;
        sp18.vy += sp40.vy;
        sp18.vz += sp40.vz;

        arg0->field_28[i] = 0xC0C0C0;

        rand              = Rng_GenerateInt(-0x400, 0x400 - 1);
        randSum          += rand;
        arg0->field_A0[i] = randSum;

        if (cond == 0)
        {
            break;
        }
    }

    arg0->field_18 = i;
}

void func_800DCF94(void) // 0x800DCF94
{
    s32         i;
    s_800F3DAC* curPtr;

    D_800F3DAC = (s_800F3DAC*)FS_BUFFER_35;
    curPtr     = D_800F3DAC;
    memset(curPtr, 0xA7, sizeof(s_800F3DAC) * 30);

    for (i = 0; i < 30; i++, curPtr++)
    {
        curPtr->field_0 = 0;
    }

    func_800DBABC();
}

void func_800DCFF8(void) // 0x800DCFF8
{
    s32         i;
    s_800F3DAC* curPtr;

    D_800F3DAC = (s_800F3DAC*)FS_BUFFER_30;

    curPtr = D_800F3DAC;
    for (i = 0; i < 30; i++, curPtr++)
    {
        curPtr->field_0 = 0;
    }

    func_800DBABC();
}

void func_800DD044(void) // 0x800DD044
{
    s32         i;
    s_800F3DAC* ptr;

    D_800F3DAC = (s_800F3DAC*)FS_BUFFER_1;
    ptr        = D_800F3DAC;

    for (i = 0; i < 30; i++, ptr++)
    {
        ptr->field_0 = 0;
    }

    func_800DBABC();
}

s_800F3DAC* func_800DD090(void) // 0x800DD090
{
    s32         i;
    s_800F3DAC* ptr;

    ptr = D_800F3DAC;

    for (i = 0; i < 30; i++, ptr++)
    {
        if (ptr->field_0 != 0)
        {
            continue;
        }

        ptr->field_0   = 1;
        ptr->field_4E8 = 0;
        ptr->field_1C  = 0;
        ptr->field_10  = 0;
        ptr->field_4F0 = 0;
        ptr->field_4F4 = 0;
        return ptr;
    }

    return NULL;
}
void func_800DD0EC(const VECTOR3* pos, s32 coordIdx) // 0x800DD0EC
{
    VECTOR        newPos;     // Q19.12
    VECTOR        offset;     // Q19.12
    SVECTOR       rot;        // Q3.12
    SVECTOR       offsetBase; // Q3.12
    MATRIX        rotMat;
    s_800F3DAC*   ptr0;
    s_D_800F48A8* ptr;

    offsetBase = (SVECTOR){ 0, 0, Q12(1.5f) }; // .rodata 0x800CAEAC

    ptr = &D_800F48A8;

    ptr0 = func_800DD090();
    if (ptr0 != NULL)
    {
        // Compute rotation matrix.
        rot.vx = Rng_Rand16();
        rot.vy = Rng_Rand16();
        rot.vz = Q12_ANGLE(0.0f);
        Math_RotMatrixXyz(&rot, &rotMat);

        // Rotate offset.
        SetRotMatrix(&rotMat);
        ApplyRotMatrix(&offsetBase, &offset);

        // Compute new position.
        newPos.vx = pos->vx + offset.vx;
        newPos.vy = pos->vy + offset.vy;
        newPos.vz = pos->vz + offset.vz;

        ptr0->field_4E4    = 3;
        ptr0->rotationZ_4D8     = Q12_ANGLE(108.0f);
        ptr0->timer_C      = Q12(1.0f);
        ptr0->timer      = Q12(1.0f);
        ptr0->coordIdx_4EC = coordIdx;
        ptr0->field_14     = Q12_ANGLE(15.0f);
        func_800DCDDC(ptr0, &newPos, pos);

        if (ptr->field_48 == 0)
        {
            func_800DBAE8(pos, 2);
        }
        else
        {
            func_800DBAE8(pos, 4);
        }
    }
}

void func_800DD240(const VECTOR3* pos) // 0x800DD240
{
    func_800DD0EC(pos, 2);
}

void func_800DD260(const VECTOR3* arg0, const VECTOR3* pos) // 0x800DD260
{
    s_800F3DAC* ptr;

    ptr = func_800DD090();
    if (ptr != NULL)
    {
        ptr->rotationZ_4D8  = Q12_ANGLE(43.2f);
        ptr->field_4E4 = 4;
        ptr->timer_C   = Q12(0.5f);
        ptr->timer   = Q12(0.5f);
        ptr->field_14  = Q12_ANGLE(45.0f);

        func_800DCDDC(ptr, arg0, pos);
    }
}

void func_800DD2C8(const VECTOR3* arg0, const VECTOR3* arg1) // 0x800DD2C8
{
    s_800F3DAC* ptr;

    ptr = func_800DD090();
    if (ptr != NULL)
    {
        ptr->rotationZ_4D8  = Q12_ANGLE(180.0f);
        ptr->field_4E4 = 4;
        ptr->timer_C   = Q12(0.5f);
        ptr->timer   = Q12(0.5f);
        ptr->field_14  = Q12_ANGLE(45.0f);

        func_800DCDDC(ptr, arg0, arg1);
    }
}

void func_800DD32C(const VECTOR3* arg0, const VECTOR3* arg1) // 0x800DD32C
{
    s_800F3DAC*   retPtr;
    s_D_800F48A8* ptr;

    ptr    = &D_800F48A8;
    retPtr = func_800DD090();
    if (retPtr != NULL)
    {
        retPtr->field_4F0 = func_800DC49C;
        retPtr->rotationZ_4D8  = Q12_ANGLE(216.0f);
        retPtr->field_4E4 = 6;
        retPtr->timer_C   = Q12(2.0f);
        retPtr->timer   = Q12(2.0f);
        retPtr->field_14  = Q12_ANGLE(45.0f);

        func_800DCDDC(retPtr, arg0, arg1);

        if (ptr->field_48 == 0)
        {
            func_800DBAE8(arg0, 0);
        }

        func_800DBAE8(arg0, 2);
    }
}

void func_800DD3D4(void* arg0, q19_12 scaleX, q19_12 scaleY, q19_12 scaleZ) // 0x800DD3D4
{
    VECTOR3 pos;
    MATRIX  mat;

    Vw_CoordHierarchyMatrixCompute(&g_SysWork.playerBoneCoords[HarryBone_Torso], &mat);

    pos.vx = Q8_TO_Q12(mat.t[0]) + scaleX;
    pos.vy = Q8_TO_Q12(mat.t[1]) + scaleY;
    pos.vz = Q8_TO_Q12(mat.t[2]) + scaleZ;

    func_800DD32C(arg0, &pos);
}

void func_800DD464(const VECTOR3* arg0) // 0x800DD464
{
    s_800F3DAC* ptr;

    ptr = func_800DD090();
    if (ptr != NULL)
    {
        ptr->field_4F0 = func_800DC49C;
        ptr->rotationZ_4D8  = Q12_ANGLE(90.0f);
        ptr->field_4E4 = 2;
        ptr->timer_C   = Q12(1.5f);
        ptr->timer   = Q12(1.5f);
        ptr->field_14  = Q12_ANGLE(20.0f);

        func_800DCDDC(ptr, arg0, arg0);
    }
}

void func_800DD4CC(s_800F3DAC* arg0) // 0x800DD4CC
{
    VECTOR3 newPos;  // Q19.12
    VECTOR3 prevPos; // Q19.12
    MATRIX  mat;
    s32     temp_v1;

    if (arg0->field_4E4 == 3)
    {
        temp_v1 = arg0->field_18 - 1;
        newPos.vx = Q8_TO_Q12(arg0->mat_118[temp_v1].t[0]) + D_800F48A8.positionX;
        newPos.vy = Q8_TO_Q12(arg0->mat_118[temp_v1].t[1]);
        newPos.vz = Q8_TO_Q12(arg0->mat_118[temp_v1].t[2]) + D_800F48A8.positionZ;

        Vw_CoordHierarchyMatrixCompute(&D_800F48A8.coords_2C[arg0->coordIdx_4EC], &mat);
        prevPos.vx = Q8_TO_Q12(mat.t[0]);
        prevPos.vy = Q8_TO_Q12(mat.t[1]);
        prevPos.vz = Q8_TO_Q12(mat.t[2]);

        func_800DD260(&newPos, &prevPos);
    }
}

void func_800DD594(const VECTOR3* pos, s_SubCharacter* chara, GsCOORDINATE2* boneCoords, s32 arg3) // 0x800DD594
{
    q19_12        newPosX;
    q19_12        newPosZ;
    s_D_800F48A8* ptr;

    ptr = &D_800F48A8;

    newPosX = pos->vx;
    if (newPosX < Q12(0.0f))
    {
        newPosX += Q12(64.0f) - 1;
    }
    newPosX = (newPosX >> 18) << 18;

    newPosZ = pos->vz;
    if (newPosZ < Q12(0.0f))
    {
        newPosZ += Q12(64.0f) - 1;
    }

    ptr->positionX = newPosX;
    ptr->positionZ = (newPosZ >> 18) << 18;
    ptr->field_28 = chara;
    ptr->coords_2C = boneCoords;
    ptr->playerPosition_30 = g_SysWork.playerWork.player.position;
    ptr->field_44 = 0;
    ptr->field_48 = arg3;

    func_800DCF94();
}

void func_800DD62C(const VECTOR3* pos, s_SubCharacter* chara, GsCOORDINATE2* boneCoords) // 0x800DD62C
{
    func_800D952C();
    func_800DD594(pos, chara, boneCoords, 0);
}

void func_800DD67C(const VECTOR3* pos, s_SubCharacter* chara, GsCOORDINATE2* boneCoords) // 0x800DD67C
{
    func_800D952C();
    func_800DD594(pos, chara, boneCoords, 1);
}

void func_800DD6CC(void) // 0x800DD6CC
{
    q19_12    posX;
    q19_12    posZ;
    s32       activeBufferIdx;
    GsOT_TAG* ot;

    posX            = D_800F48A8.positionX;
    posZ            = D_800F48A8.positionZ;
    activeBufferIdx = g_ActiveBufferIdx;
    ot              = g_OrderingTable0[activeBufferIdx].org;

    Vw_WorldScreenMatrixAtPositionGet(&D_800F48A8.mat_8, posX, Q12(0.0f), posZ);
    func_800DC544(ot);
    func_800D917C();
}

void func_800DD738(const VECTOR3* pos0, const VECTOR3* pos1, q19_12 rotZ, q19_12 timer) // 0x800DD738
{
    s_800F3DAC* ptr;

    ptr = func_800DD090();
    if (ptr != NULL)
    {
        ptr->field_4E4 = 7;
        ptr->rotationZ_4D8  = rotZ;
        ptr->timer_C   = timer;
        ptr->timer   = timer;
        ptr->field_14  = Q12_ANGLE(45.0f);

        func_800DCDDC(ptr, pos0, pos1);

        if (rotZ > Q12_ANGLE(108.0f))
        {
            func_800DBAE8(pos0, 0);
        }
        else
        {
            func_800DBAE8(pos0, 4);
        }
    }
}

void func_800DD7D0(VECTOR3* charaPos) // 0x800DD7D0
{
    s32           gridX;
    s32           gridZ;
    s_D_800F48A8* ptr;

    ptr = &D_800F48A8;

    // TODO: Possibly an inline/macro, based on needing separate `s_D_800F48A8*`
    gridX            = (charaPos->vx / Q12(64.0f)) * Q12(64.0f);
    gridZ            = (charaPos->vz / Q12(64.0f)) * Q12(64.0f);
    ptr->positionX = gridX;
    ptr->positionZ = gridZ;

    ptr->playerPosition_30 = g_SysWork.playerWork.player.position;
    ptr->field_44          = 1;
    ptr->field_48          = 2;

    func_800DCFF8();
}

void func_800DD868(void) // 0x800DD868
{
    q19_12    posX;
    q19_12    posZ;
    s32       activeBufferIdx;
    GsOT_TAG* ot;

    posX            = D_800F48A8.positionX;
    posZ            = D_800F48A8.positionZ;
    activeBufferIdx = g_ActiveBufferIdx;
    ot              = g_OrderingTable0[activeBufferIdx].org;

    Vw_WorldScreenMatrixAtPositionGet(&D_800F48A8.mat_8, posX, Q12(0.0f), posZ);
    func_800DC544(ot);
}

void func_800DD8CC(VECTOR3* charaPos) // 0x800DD8CC
{
    s32           gridX;
    s32           gridZ;
    s_D_800F48A8* ptr;

    ptr = &D_800F48A8;

    // TODO: Possibly an inline/macro, based on needing separate `s_D_800F48A8*`
    gridX            = (charaPos->vx / Q12(64.0f)) * Q12(64.0f);
    gridZ            = (charaPos->vz / Q12(64.0f)) * Q12(64.0f);
    ptr->positionX = gridX;
    ptr->positionZ = gridZ;

    ptr->playerPosition_30 = g_SysWork.playerWork.player.position;
    ptr->field_44          = 1;
    ptr->field_48          = 2;

    func_800DD044();
}

#include "../src/maps/common/characters/incubus.c" // 0x800DD964
#include "../src/maps/common/characters/unknown23.c" // 0x800DF1D4

extern u8 g_EndingIdx; /** `e_GameEnding` */

void func_800E0A34(void) // 0x800E0A34
{
    s_FsImageDesc tex;

    tex.tPage[0] = 0;
    tex.tPage[1] = 21;
    tex.u        = 0;
    tex.v        = 0;
    tex.clutX    = 48;
    tex.clutY    = 0;
    Fs_QueueStartReadTim(FILE_TIM_BOSEF00_TIM, FS_BUFFER_1, &tex);
}

#include "../src/maps/common/map_util.c" // 0x800E0A80

void func_800E0B28(s32 arg0) // 0x800E0B28
{
    (arg0 != 0) ? func_800E0B90() : sharedFunc_800D0110_7_s00();
}

void func_800E0B5C(s32 arg0) // 0x800E0B5C
{
    (arg0 != 0) ? func_800E0C10() : sharedFunc_800D0110_7_s00();
}

void func_800E0B90(void) // 0x800E0B90
{
    s32 bgmIdx; // `e_BgmCmd`

    if (g_SavegamePtr->mapRoomIdx == 3)
    {
        bgmIdx = BgmCmd_Track15;
    }
    else if (Savegame_EventFlagGet(EventFlag_481))
    {
        if (!Savegame_EventFlagGet(EventFlag_574))
        {
            Savegame_EventFlagSet(EventFlag_574);
            bgmIdx = BgmCmd_UpdateTrack;
        }
        else
        {
            bgmIdx = BgmCmd_Track22;
        }
    }
    else
    {
        bgmIdx = BgmCmd_Track14;
    }

    Bgm_SongChange(bgmIdx);
}

void func_800E0C10(void) // 0x800E0C10
{
    Bgm_SongChange(Savegame_EventFlagGet(EventFlag_M5S03_KaufmannHasMotorcycleVial) ? 40 : 35);
}

#include "maps/common/shared/sharedFunc_800D0110_7_s00.h" // 0x800E0C50

void GameBoot_LoadScreen_StageString(void) {}

void func_800E14DC(s_SubCharacter* player, s_SubCharacter* otherChara, bool warpCamera) // 0x800E14DC
{
    VECTOR3 camTargetPos;
    VECTOR3 camLookAtTargetPos;
    bool    isCamUpdated;
    q19_12  angleToOtherChara;
    s32     vcPrsFViewFlag;

    isCamUpdated        = false;
    angleToOtherChara = ratan2(player->position.vx - otherChara->position.vx,
                               player->position.vz - otherChara->position.vz);

    vcPrsFViewFlag = (vcWork.flags & VC_PRS_F_VIEW_F) == VC_PRS_F_VIEW_F;
    if (((g_GameWorkConst->config.extraViewCtrl && (vcPrsFViewFlag ^ 1) != 0) ||
         (!g_GameWorkConst->config.extraViewCtrl && vcPrsFViewFlag)) &&
        (g_GameWorkConst->config.extraViewMode == 0))
    {
        isCamUpdated = true;

        camTargetPos.vx = player->position.vx +
                          Q12_MULT_FLOAT(Math_Sin(player->rotation.vy), 1.5f) +
                          Q12_MULT_FLOAT(Math_Sin(angleToOtherChara), 2.5f);
        camTargetPos.vz = player->position.vz +
                          Q12_MULT_FLOAT(Math_Cos(player->rotation.vy), 1.5f) +
                          Q12_MULT_FLOAT(Math_Cos(angleToOtherChara), 2.5f);

        if (otherChara->model.charaId == Chara_Incubus)
        {
            camTargetPos.vy = Q12(-1.4f);
        }
        else
        {
            camTargetPos.vy = Q12(-2.3f);
        }

        camLookAtTargetPos.vx = otherChara->position.vx + FP_FROM(Q12(Math_Sin(angleToOtherChara)), Q12_SHIFT);
        camLookAtTargetPos.vz = otherChara->position.vz + FP_FROM(Q12(Math_Cos(angleToOtherChara)), Q12_SHIFT);

        if (warpCamera)
        {
            camLookAtTargetPos.vy = Q12(-0.002f);
        }
        else if (otherChara->model.charaId == Chara_Incubus)
        {
            camLookAtTargetPos.vy = Vc_LookAtPositionYGet();
        }
        else
        {
            camLookAtTargetPos.vy = Q12(-0.8f);
        }
    }

    if (isCamUpdated)
    {
        vcUserCamTarget(&camTargetPos, NULL, warpCamera);
        vcUserWatchTarget(&camLookAtTargetPos, NULL, warpCamera);

        if (warpCamera)
        {
            vcExecCamera();
        }
    }
    else
    {
        vcReturnPreAutoCamWork(false);
    }
}

void func_800E16FC(void) // 0x800E16FC
{
    D_800F4B40.field_0  = NO_VALUE;
    D_800F4B40.timer  = Q12(0.0f);
    D_800F4B40.field_18[0] = 250;
    D_800F4B40.field_18[1] = 200;
    D_800F4B40.field_18[2] = 150;

    D_800F4B40.field_1C[0].vec_0 = &D_800F4B40.field_B8[0];
    D_800F4B40.field_1C[0].vec_8 = &D_800F4B40.field_B8[1];
    D_800F4B40.field_1C[1].vec_0 = &D_800F4B40.field_B8[2];
    D_800F4B40.field_1C[1].vec_8 = &D_800F4B40.field_B8[3];
    D_800F4B40.field_64[0].vec_0 = &D_800F4B40.field_B8[4];
    D_800F4B40.field_64[0].vec_8 = &D_800F4B40.field_B8[5];
    D_800F4B40.field_64[1].vec_0 = &D_800F4B40.field_B8[6];
    D_800F4B40.field_64[1].vec_8 = &D_800F4B40.field_B8[7];

    func_800E1788(0);
}

void func_800E1788(s32 arg0) // 0x800E1788
{
    if (arg0 != D_800F4B40.field_0)
    {
        D_800F4B40.field_0  = arg0;
        D_800F4B40.field_4  = 0;
        D_800F4B40.timer  = Q12(0.0f);
        D_800F4B40.field_C  = 0;
        D_800F4B40.timer_10 = Q12(0.0f);
    }
}

void func_800E17B8(s_800F4B40_1C* dest, s_800F4B40_1C* src) // 0x800E17B8
{
    VECTOR3* origVec0Ptr;
    VECTOR3* origVec1Ptr;

    // Backup original `VECTOR3` pointers.
    origVec0Ptr = dest->vec_0;
    origVec1Ptr = dest->vec_8;

    // Copy struct data.
    *dest = *src;

    // Restore `VECTOR3` pointers.
    dest->vec_0 = origVec0Ptr;
    dest->vec_8 = origVec1Ptr;

    // Copy `VECTOR3` data.
    *dest->vec_0 = *src->vec_0;
    *dest->vec_8 = *src->vec_8;
}

const char* MAP_MESSAGES[] = {
    #include "maps/common/shared/map_msg_common.h"
#if VERSION_REGION_IS(NTSC)
    /* 15 */ "~J1(4.9)\tI_was_shocked_to_realize ~N\n\t\t\tthe_talisman_of_Metraton ~N\n\t\t\twas_being_used. ",
    /* 16 */ "~J1(4.0)\tIn_spite_of_the_lost_soul ~N\n\t\t\treturning_at_last, ",
    /* 17 */ "~J1(4.3)\tJust_a_little_longer_and_all ~N\n\t\t\twould_have_been_for_naught. ",
    /* 18 */ "~J1(7.0)\tIt's_all_because_of_that_man. ~N\n\t\t\tWe_must_be_thankful_to_him. ",
    /* 19 */ "~J1(3.2)\tEven_though_Alessa ~N\n\t\t\thas_been_stopped, ",
    /* 20 */ "~J1(8.5)\this_little_girl_still_has_to_go. ~N\n\t\t\tWhat_a_pity... ~E ",
    /* 21 */ "~J0(1.2)\tFreeze! ~E ",
    /* 22 */ "~J0(2.6)\tWhat_in_the_devil's_name...! ~E ",
    /* 23 */ "~J0(1.1)\t~E ",
    /* 24 */ "~J1(1.1)\tDahlia! ",
    /* 25 */ "~J1(5.6)\tWell,_well,_well. ~N\n\t\t\tTo_think_you'd_make_it_this_far. ",
    /* 26 */ "~J1(2.5)\tWhere's_Cheryl!? ~N\n\t\t\tWhat_have_you_done_to_her!? ",
    /* 27 */ "~J1(2.0)\tWhat_are_you_talking_about? ",
    /* 28 */ "~J1(4.8)\tYou've_seen_her_many_times. ~N\n\t\t\tRestored_to_her_former_self. ",
    /* 29 */ "~J1(2.4)\tI'm_in_no_mood_for_jokes. ~E ",
    /* 30 */ "~J0(3.4)\tDon't_you_see? ~N\n\t\t\tShe's_right_there. ~E ",
    /* 31 */ "~J1(1.8)\tThat's_absurd. ",
    /* 32 */ "~J1(4.5)\tYou_are_the_only_one ~N\n\t\t\twho_thinks_so. ",
    /* 33 */ "~J1(1.4)\tWhy? ",
    /* 34 */ "~J1(2.2)\tWhy_are_you_doing_this? ~E ",
    /* 35 */ "~J1(3.8)\tIt's_been_a_long_seven_years... ",
    /* 36 */ "~J1(4.4)\tFor_the_seven_years ~N\n\t\t\tsince_that_terrible_day, ",
    /* 37 */ "~J1(3.2)\tAlessa_has_been_kept_alive, ",
    /* 38 */ "~J1(3.9)\tsuffering_a_fate_worse ~N\n\t\t\tthan_death. ",
    /* 39 */ "~J1(6.5)\tAlessa_has_been_trapped ~N\n\t\t\tin_an_endless_nightmare ~N\n\t\t\tfrom_which_she_never_wakens. ",
    /* 40 */ "~J1(4.0)\tHe_has_been_nurtured ~N\n\t\t\tby_that_nightmare. ",
    /* 41 */ "~J1(3.6)\tWaiting_for ~N\n\t\t\tthe_day_to_be_born. ",
    /* 42 */ "~J1(3.6)\tThat_day_has_finally_come. ",
    /* 43 */ "~J1(2.9)\tThe_time_is_neigh. ~E ",
    /* 44 */ "~J1(5.1)\tEveryone_will_be_released, ~N\n\t\t\tfrom_pain_and_suffering. ",
    /* 45 */ "~J1(3.1)\tOur_salvation_is_at_hand. ",
    /* 46 */ "~J1(2.8)\tThis_is_the_day_of_reckoning. ",
    /* 47 */ "~J1(4.7)\tWhen_all_our_sorrows ~N\n\t\t\twill_be_washed_away. ",
    /* 48 */ "~J1(4.8)\tWhen_we_return_to ~N\n\t\t\tthe_true_paradise!  ~E ",
    /* 49 */ "~J0(6.2)\tMy_daughter_will_be ~N\n\t\t\tthe_mother_of_God! ~E ",
    /* 50 */ "~J0(4.0)\t~E ",
    /* 51 */ "~J1(1.7)\tQuit_screwing_around! ",
    /* 52 */ "~J1(2.4)\tReturn_things_to_how ~N\n\t\t\tthey_were_before! ",
    /* 53 */ "~J1(1.0)\tKaufmann! ",
    /* 54 */ "~J1(1.9)\tDid_I_ask_for_this! ",
    /* 55 */ "~J1(1.9)\tNobody_uses_me! ",
    /* 56 */ "~J1(3.0)\tYou_won't_get_away ~N\n\t\t\twith_this! ~E ",
    /* 57 */ "~J1(4.7)\tYour_role_is_over. ~N\n\t\t\tWe_don't_need_you_anymore. ",
    /* 58 */ "~J1(3.6)\tWhat_do_you_think_you_can ~N\n\t\t\taccomplish_by_coming_here? ",
    /* 59 */ "~J1(3.5)\tMy,_aren't_we_getting_cocky? ",
    /* 60 */ "~J1(3.2)\tBet_you_can't_see_this ~N\n\t\t\tand_keep_your_cool.  ~E ",
    /* 61 */ "~J0(3.5)\tAglaophotis! ~N\n\t\t\tI_thought_I_got_rid_of_that! ",
    /* 62 */ "~J0(3.2)\tAll_I_had_to_do_was_plant_it ~N\n\t\t\tsomewhere_for_you_to_find, ",
    /* 63 */ "~J0(3.9)\tYou_all,_well, ~N\n\t\t\tit_kept_you_busy. ",
    /* 64 */ "~J0(1.7)\tYou're_easy! ",
    /* 65 */ "~J0(2.4)\tAnd_there's_more ~N\n\t\t\twhere_this_came_from! ~E ",
    /* 66 */ "~J0(1.7)\tStop_it!!  ~E ",
    /* 67 */ "~J0(2.2)\t~E ",
    /* 68 */ "~J0(1.4)\t",
    /* 69 */ "~J2(1.2)\tWhat_the!? ~E ",
    /* 70 */ "~J0(10.4)\t~E ",
    /* 71 */ "~J0(3.0)\tWhat_on_earth? ~N\n\t\t\tThat's_not_supposed_to... ~E ",
    /* 72 */ "~J0(1.8)\t~E ",
    /* 73 */ "~J0(3.5)\t~E ",
    /* 74 */ "~J0(3.4)\tIt's_awakening! ~N\n\t\t\tIt's_awake! ",
    /* 75 */ "~J0(3.0)\tNow_no_one_can_interfere! ~E ",
    /* 76 */ "~J0(1.2)\tCheryl!! ",
    /* 77 */ "~J0(3.5)\tGive_up! ~N\n\t\t\tThat_person_no_longer_exists. ~E ",
    /* 78 */ "~J0(2.0)\tThis_is_a_God! ",
    /* 79 */ "~J2(4.9)\tDescended_among_us_to_reach_out ~N\n\t\t\tand_save_us! ~E ",
    /* 80 */ "~J0(13.2)\t_ ~E ",
    /* 81 */ "~J0(2.4)\t_ ~E ",
    /* 82 */ "~J0(1.3)\t_",
    /* 83 */ "~J0(1.1)\t_ ~E ",
    /* 84 */ "~J0(1.6)\tThank_you_daddy. ",
    /* 85 */ "~J0(1.4)\tGoodbye. ~E ",
    /* 86 */ "~J0(3.0)\tCheryl...? ~E ",
    /* 87 */ "~J0(2.3)\tCan't_be? ",
    /* 88 */ "~J0(2.2)\tYou_can't_leave_like_this... ",
    /* 89 */ "~J0(3.2)\tThis_can't_be_happening! ~N\n\t\t\tCheryl!? ~E ",
    /* 90 */ "~J0(1.4)\t...Cheryl!! ~E ",
    /* 91 */ "~J0(1.1)\t~E ",
    /* 92 */ "~J0(3.9)\t~E ",
    /* 93 */ "~J0(1.3)\tWait... ~E ",
    /* 94 */ "~J0(4.8)\t~E ",
    /* 95 */ "~J0(1.0)\t~E ",
    /* 96 */ "~J0(1.0)\t~E ",
    /* 97 */ "~J0(4.5)\t~E ",
    /* 98 */ "~J0(1.4)\tHarry. ",
    /* 99 */ "~J0(1.0)\tGo. ~E "
#elif VERSION_REGION_IS(NTSCJ)
    /* 15 */ "~J1(4.9)\tメトラトンの印章を作っていることに ~N\n\t\t\t気がついたときは本当に驚いたわ ",
    /* 16 */ "~J1(4.0)\t待ち望んでいた消えた魂が ~N\n\t\t\tやっと帰ってきたというのに ",
    /* 17 */ "~J1(4.3)\tもう少しですべてが ~N\n\t\t\t水の泡になるところだったのよ ",
    /* 18 */ "~J1(7.0)\tあの男のおかげよ ~N\n\t\t\t本当に感謝しているわ ",
    /* 19 */ "~J1(3.2)\tアレッサを止めても止めなくても ",
    /* 20 */ "~J1(8.5)\t結局　あの男の娘なんて ~N\n\t\t\t消えてしまうのにね ~E ",
    /* 21 */ "~J0(1.2)\tふざけるな！ ~E ",
    /* 22 */ "~J0(2.6)\tそんな……！ ~E ",
    /* 23 */ "~J0(1.1)\t~E ",
    /* 24 */ "~J1(1.1)\tダリア！ ",
    /* 25 */ "~J1(5.6)\t……まあ ~N\n\t\t\tこんなところまでたどり着くなんて ",
    /* 26 */ "~J1(2.5)\tシェリルは！？ ~N\n\t\t\tいったいどこにいるんだ！ ",
    /* 27 */ "~J1(2.0)\t何を言っているの？ ",
    /* 28 */ "~J1(4.8)\t何度も会っているじゃない ~N\n\t\t\t本来の自分に還元されて ",
    /* 29 */ "~J1(2.4)\t……何だって？ ~E ",
    /* 30 */ "~J0(3.4)\tまだわからない？ ~N\n\t\t\tそこにいるじゃない ~E ",
    /* 31 */ "~J1(1.8)\tそんな馬鹿な ",
    /* 32 */ "~J1(4.5)\tそう思わないのはあなたの勝手よ ",
    /* 33 */ "~J1(1.4)\t何のためだ？ ",
    /* 34 */ "~J1(2.2)\t何のためにこんなことをする？ ~E ",
    /* 35 */ "~J1(3.8)\t……この７年間　長かったわ ",
    /* 36 */ "~J1(4.4)\tあの日から７年の間 ",
    /* 37 */ "~J1(3.2)\tアレッサは生かされ続けたわ ",
    /* 38 */ "~J1(3.9)\t死よりつらい苦しみが７年も続いたのよ ",
    /* 39 */ "~J1(6.5)\tアレッサは目覚めることのない ~N\n\t\t\t悪夢を見続け…… ",
    /* 40 */ "~J1(4.0)\t彼はその悪夢と共に育っていった…… ",
    /* 41 */ "~J1(3.8)\t彼はじっと生まれる日を待ち続けたわ ",
    /* 42 */ "~J1(3.6)\tそして　ついにこの日が来たのよ ",
    /* 43 */ "~J1(2.9)\tすべてはこれからよ ~E ",
    /* 44 */ "~J1(5.1)\tみんな解放されるわ ~N\n\t\t\tすべての苦しみから ",
    /* 45 */ "~J1(3.1)\tあたしたちの悲願…… ",
    /* 46 */ "~J1(2.8)\t約束の日 ",
    /* 47 */ "~J1(4.7)\tすべての罪が洗い流される日 ",
    /* 48 */ "~J1(4.8)\tあたしたちは真の楽園を取り戻すのよ！ ~E ",
    /* 49 */ "~J0(6.2)\tあたしの娘が！ ~N\n\t\t\t救い手の母となるのよ！ ~E ",
    /* 50 */ "~J0(4.0)\t~E ",
    /* 51 */ "~J1(1.7)\tふざけるな！ ",
    /* 52 */ "~J1(2.4)\tとっとと元の世界に戻せ！ ",
    /* 53 */ "~J1(1.0)\tカウフマン！ ",
    /* 54 */ "~J1(1.9)\t誰がこんなことを望んだ？ ",
    /* 55 */ "~J1(1.9)\tこの俺を利用しやがって！ ",
    /* 56 */ "~J1(4.0)\tおまえらのような化け物に ~N\n\t\t\t好きにさせてたまるか！ ~E ",
    /* 57 */ "~J1(4.7)\tあなたの役目は終わったの ~N\n\t\t\t必要ないのよ ",
    /* 58 */ "~J1(3.6)\tこんなところに現れて ~N\n\t\t\tどうしようというの？ ",
    /* 59 */ "~J1(3.0)\t何だと？ ~N\n\t\t\tずいぶんと偉そうじゃないか？ ",
    /* 60 */ "~J1(3.2)\tだが　こいつを見てもそんなふうに ~N\n\t\t\t涼しい顔でいられるのか？ ~E ",
    /* 61 */ "~J0(3.5)\tアグラオフォティス！ ~N\n\t\t\t処分したはずだわ！ ",
    /* 62 */ "~J0(3.2)\tわざと見つかるところに１つ置いておけば ",
    /* 63 */ "~J0(3.9)\tおまえらじゃ　それ以上 ~N\n\t\t\t気がまわらないからな ",
    /* 64 */ "~J0(1.7)\t甘いんだよ！ ",
    /* 65 */ "~J0(2.4)\t切り札が１枚しかないと思ったか ~E ",
    /* 66 */ "~J0(1.7)\tやめて！！ ~E ",
    /* 67 */ "~J0(2.2)\t~E ",
    /* 68 */ "~J0(1.4)\t",
    /* 69 */ "~J2(1.2)\t……あれは！？ ~E ",
    /* 70 */ "~J0(10.4)\t~E ",
    /* 71 */ "~J0(3.0)\tいったいどうしたというんだ……？ ~N\n\t\t\tこんなはずは…… ~E ",
    /* 72 */ "~J0(1.8)\t~E ",
    /* 73 */ "~J0(3.5)\t~E ",
    /* 74 */ "~J0(3.4)\t目覚めたわ！ ~N\n\t\t\t目覚めたのよ！ ",
    /* 75 */ "~J0(3.0)\tもう誰にも邪魔されないわ！ ~E ",
    /* 76 */ "~J0(1.2)\tシェリル！！ ",
    /* 77 */ "~J0(3.5)\t無駄よ！ ~N\n\t\t\tもうそんな人間は存在しないわ ~E ",
    /* 78 */ "~J0(2.0)\tここにいるのは神よ！ ",
    /* 79 */ "~J2(4.9)\tあたしたちに救いの手を ~N\n\t\t\t差し伸べるために降臨したのよ！ ~E ",
    /* 80 */ "~J0(13.2)\t~E ",
    /* 81 */ "~J0(2.4)\t~E ",
    /* 82 */ "~J0(1.3)\t",
    /* 83 */ "~J0(1.1)\t~E ",
    /* 84 */ "~J0(1.6)\tありがとう　パパ ",
    /* 85 */ "~J0(1.4)\tさようなら ~E ",
    /* 86 */ "~J0(3.0)\tシェリル……？ ~E ",
    /* 87 */ "~J0(2.3)\t嘘だろう……？ ",
    /* 88 */ "~J0(2.2)\tこんなこと許されるはずないじゃないか…… ",
    /* 89 */ "~J0(3.2)\t嘘だろう？　シェリル！？ ~E ",
    /* 90 */ "~J0(1.4)\t……シェリル！！ ~E ",
    /* 91 */ "~J0(1.1)\t~E ",
    /* 92 */ "~J0(3.9)\t~E ",
    /* 93 */ "~J0(1.3)\t待ってくれ…… ~E ",
    /* 94 */ "~J0(4.8)\t~E ",
    /* 95 */ "~J0(1.0)\t~E ",
    /* 96 */ "~J0(1.0)\t~E ",
    /* 97 */ "~J0(4.5)\t~E ",
    /* 98 */ "~J0(1.4)\tハリー ",
    /* 99 */ "~J0(1.0)\t行くのよ ~E "
#endif
};

void func_800E1854(void) // 0x800E1854
{
    VECTOR3 sp10;
    VECTOR3 sp20;
    q19_12  var_v0;

    switch (D_800F4B40.field_0)
    {
        case NO_VALUE:
            return;

        case 0:
            func_800E1FE0(&D_800ED274[0]);
            break;

        case 1:
            if (D_800F4B40.field_4 == 0)
            {
                D_800F4B40.field_4 = 1;
                func_800E1FE0(&D_800ED274[1]);
            }
            break;

        case 2:
            if (D_800F4B40.field_4 == 0)
            {
                D_800F4B40.field_4 = 1;
                func_800E1FE0(&D_800ED274[2]);
            }
            break;

        case 3:
            switch (D_800F4B40.field_4)
            {
                case 0:
                    func_800E1FE0(&D_800ED274[3]);
                    D_800F4B40.field_4++;
                    break;

                case 1:
                    if (D_800F4B40.timer > Q12(0.6f))
                    {
                        D_800F4B40.field_4 = 2;
                        func_800E1FE0(&D_800ED274[4]);
                        D_800F4B40.timer = Q12(0.0f);
                    }
                    break;
            }
            break;

        case 5:
            if (D_800F4B40.field_4 == 0)
            {
                D_800F4B40.field_4 = 1;
                func_800E1FE0(&D_800ED274[5]);
            }
            break;

        case 6:
            if (D_800F4B40.field_4 == 0)
            {
                D_800F4B40.field_4 = 1;
                func_800E1FE0(&D_800ED274[6]);
            }
            break;

        case 7:
            D_800F4B30.vx = Q8_TO_Q12(g_SysWork.npcBoneCoordBuffer[1].workm.t[0]);
            D_800F4B30.vy = Q8_TO_Q12(g_SysWork.npcBoneCoordBuffer[1].workm.t[1]);
            D_800F4B30.vz = Q8_TO_Q12(g_SysWork.npcBoneCoordBuffer[1].workm.t[2]);

            if (D_800F4B40.field_4 == 0)
            {
                D_800F4B40.field_4 = 1;
                func_800E1FE0(&D_800ED274[7]);
            }
            break;

        case 8:
            D_800F4B30.vx = Q8_TO_Q12(g_SysWork.npcBoneCoordBuffer[1].workm.t[0]);
            D_800F4B30.vy = Q8_TO_Q12(g_SysWork.npcBoneCoordBuffer[1].workm.t[1]);
            D_800F4B30.vz = Q8_TO_Q12(g_SysWork.npcBoneCoordBuffer[1].workm.t[2]);

            if (D_800F4B40.field_4 == 0 && D_800F4B40.timer > Q12(0.5f))
            {
                D_800F4B40.field_4 = 1;
                func_800E1FE0(&D_800ED274[8]);
            }
            break;

        case 9:
            if (D_800F4B40.field_4 == 0)
            {
                D_800F4B40.field_4 = 1;
                func_800E1FE0(&D_800ED274[9]);
            }
            break;

        case 11:
            if (D_800F4B40.field_4 == 0)
            {
                D_800F4B40.field_4 = 1;
                func_800E1FE0(&D_800ED274[10]);
            }
            break;

        case 12:
            switch (D_800F4B40.field_4)
            {
                case 0:
                    D_800F4B40.field_4 = 1;
                    func_800E1FE0(&D_800ED274[11]);
                    break;

                case 1:
                    if (D_800F4B40.timer > Q12(3.5f))
                    {
                        D_800F4B40.field_4 = 2;
                        func_800E1FE0(&D_800ED274[12]);
                        D_800F4B40.timer = Q12(0.0f);
                    }
                    break;
            }
            break;

        case 13:
            switch (D_800F4B40.field_4)
            {
                case 0:
                    func_800E1FE0(&D_800ED274[13]);
                    D_800F4B40.field_4++;
                    break;

                case 1:
                    if (D_800F4B40.timer > Q12(2.0f))
                    {
                        func_800E1FE0(&D_800ED274[14]);
                        D_800F4B40.timer = Q12(0.0f);
                        D_800F4B40.field_4++;
                    }
                    break;

                case 2:
                    if (D_800F4B40.timer > Q12(3.0f))
                    {
                        D_800F4B40.timer = Q12(0.0f);
                        D_800F4B40.field_4++;
                    }
                    // @bug Missed `break` here?

                case 3:
                    if (D_800F4B40.timer > Q12(0.1f))
                    {
                        func_800E1FE0(&D_800ED274[15]);
                        D_800F4B40.timer = Q12(0.0f);
                        D_800F4B40.field_4++;
                    }
                    break;

                case 4:
                    if (D_800F4B40.timer > Q12(0.25f))
                    {
                        func_800E1FE0(&D_800ED274[14]);
                        D_800F4B40.timer = Q12(0.0f);
                        D_800F4B40.field_4--;
                    }
                    break;
            }
            break;

        case 14:
            if (D_800F4B40.field_4 == 0 && D_800F4B40.timer > Q12(4.0f))
            {
                func_800E1FE0(&D_800ED274[16]);
                D_800F4B40.field_4++;
            }
            break;

        case 15:
            if (D_800F4B40.field_4 == 0)
            {
                func_800E1FE0(&D_800ED274[17]);
                D_800F4B40.field_4++;
            }
            break;
    }

    switch (D_800F4B40.field_0)
    {
        case 4:
            if (D_800F4B40.field_C == 0)
            {
                D_800F4B40.field_C = 1;
                func_800DD7D0(&D_800ED244);
            }

            if (D_800F4B40.timer_10 < Q12(11.0f))
            {
                var_v0 = Q12(0.0f);
            }
            else
            {
                var_v0 = MIN(D_800F4B40.timer_10 - Q12(11.0f), Q12(6.0f)) / 6;
            }

            func_800E2664(Q12(1.8f), Q12_MULT_FLOAT(var_v0, 0.3f) + 245);
            func_800DD868();
            break;

        case 5:
            if (D_800F4B40.timer_10 < Q12(2.0f))
            {
                VECTOR3* srcPos = &g_SysWork.npcs[4].position;
                sp10            = *srcPos;
                sp10.vy        -= Q12(1.1f);

                D_800F4B40.field_C++;
                if (D_800F4B40.field_C % 2)
                {
                    func_800E27D0(Q12(4.0f), Q12(0.4f), 1, &sp10);
                }

                func_800E27D0(Q12(3.5f), Q12(1.0f), 0, &sp10);
                func_800E27D0(Q12(3.5f), Q12(0.5f), 0, &sp10);
                func_800DD868();
            }
            break;

        case 10:
            if (D_800F4B40.field_C == 0)
            {
                func_800DD8CC(&D_800ED244);
                D_800F4B40.field_C++;
            }

            if (D_800F4B40.timer_10 < Q12(2.0f))
            {
                sp20     = D_800ED250;
                sp20.vy -= Q12(3.81f);

                D_800F4B40.field_C++;
                if (D_800F4B40.field_C % 2)
                {
                    func_800E27D0(Q12(3.5f), Q12(0.5f), 1, &sp20);
                    func_800E27D0(Q12(3.5f), Q12(0.5f), 1, &sp20);
                }

                func_800E27D0(Q12(3.0f), Q12(1.0f), 0, &sp20);
                func_800E27D0(Q12(3.0f), Q12(0.5f), 0, &sp20);
                func_800DD868();
            }
            break;
    }

    func_800E20A4(&D_800F4B40.field_1C[0], &D_800F4B40.field_A4[0], D_800F4B40.field_5C, &D_800F4B40.field_64[0]);
    func_800E20A4(&D_800F4B40.field_1C[1], &D_800F4B40.field_A4[1], D_800F4B40.field_60, &D_800F4B40.field_64[1]);
    func_800E24A0(&D_800F4B40.field_1C[0]);
    func_800E24A0(&D_800F4B40.field_1C[1]);

    D_800F4B40.timer  += g_DeltaTime;
    D_800F4B40.timer_10 += g_DeltaTime;
}

void func_800E1FE0(s_func_800E1FE0* arg0) // 0x800E1FE0
{
    bzero(&D_800F4B40.field_A4[0], sizeof(D_800F4B40.field_A4[0]));
    bzero(&D_800F4B40.field_A4[1], sizeof(D_800F4B40.field_A4[1]));

    D_800F4B40.field_5C = &arg0->field_4[0];
    D_800F4B40.field_60 = &arg0->field_4[1];

    if (arg0->field_0 != 0)
    {
        func_800E17B8(&D_800F4B40.field_1C[0], &arg0->field_4[0]);
        func_800E17B8(&D_800F4B40.field_1C[1], &arg0->field_4[1]);
        func_800E17B8(&D_800F4B40.field_64[0], &arg0->field_4[0]);
        func_800E17B8(&D_800F4B40.field_64[1], &arg0->field_4[1]);
    }
    else
    {
        func_800E17B8(&D_800F4B40.field_64[0], &D_800F4B40.field_1C[0]);
        func_800E17B8(&D_800F4B40.field_64[1], &D_800F4B40.field_1C[1]);
    }
}

void func_800E20A4(s_800F4B40_1C* arg0, s_800F4B40_A4* arg1, s_800F4B40_1C* arg2, s_800F4B40_1C* arg3) // 0x800E20A4
{
    s32 var_v1;

    arg0->field_1E = arg2->field_1E;
    arg0->field_12 = func_800E2444(arg2->field_16, arg0->field_12, arg2->field_12);

    var_v1 = ABS((arg2->field_14 - arg3->field_14) / 15) + 1;
    if (arg0->field_14 > (arg2->field_14 + var_v1))
    {
        arg0->field_14 -= var_v1;
    }
    else if (arg0->field_14 < (arg2->field_14 - var_v1))
    {
        arg0->field_14 += var_v1;
    }
    else
    {
        arg0->field_14 = arg2->field_14;
    }

    arg1->now_spd_0 = vwRetNewVelocityToTargetVal(arg1->now_spd_0, arg0->pos_10, arg2->pos_10, Q12(0.586f), arg2->total_max_spd_18, Q12(1.9532f));
    arg0->pos_10   += Q12_MULT_PRECISE(arg1->now_spd_0, g_DeltaTime);

    // TODO: Rename the struct fields `field_8` etc passed here to match `func_800E22AC` param names.
    func_800E22AC(arg0->vec_0, &arg0->field_4, &arg1->field_8, &arg1->field_6, arg2->vec_0, arg2->field_4, arg3->vec_0, arg3->field_4, arg2->field_1C);
    func_800E22AC(arg0->vec_8, &arg0->field_C, &arg1->field_4, &arg1->field_2, arg2->vec_8, arg2->field_C, arg3->vec_8, arg3->field_C, arg2->field_1A);
}

void func_800E22AC(VECTOR3* arg0, s32* arg1, s16* mv_pos, s16* now_spd, VECTOR3* arg4, s32 arg5, VECTOR3* arg6, s32 arg7, s16 total_max_spd) // 0x800E22AC
{
    *now_spd = vwRetNewVelocityToTargetVal(*now_spd, *mv_pos, Q12(1.0f), Q12(2.0f), total_max_spd, Q12(10.0f));
    *mv_pos += Q12_MULT_PRECISE(*now_spd, g_DeltaTime);
    *arg1    = arg7 + Q12_MULT(*mv_pos, (arg5 - arg7));

    arg0->vx = arg6->vx + Q12_MULT(*mv_pos, (arg4->vx - arg6->vx));
    arg0->vy = arg6->vy + Q12_MULT(*mv_pos, (arg4->vy - arg6->vy));
    arg0->vz = arg6->vz + Q12_MULT(*mv_pos, (arg4->vz - arg6->vz));
}

q19_12 func_800E2444(q19_12 dampingRate, q19_12 current, q19_12 target) // 0x800E2444
{
    q19_12 delta;
    q19_12 step;
    q19_12 result;

    delta = current - target;
    step  = Q12_MULT_PRECISE(dampingRate, g_DeltaTime);
    if (step < delta)
    {
        result = current - step;
    }
    else
    {
        result = current + step;
        if (delta >= -step)
        {
            result = target;
        }
    }

    return result;
}

void func_800E24A0(s_800F4B40_1C* arg0) // 0x800E24A0
{
    VECTOR3 pos0; // Q19.12
    VECTOR3 pos1; // Q19.12
    DVECTOR screenPos0;
    DVECTOR screenPos1;
    s32     projDepth;
    q23_8   var_a1;

    if (arg0->pos_10 != Q12(0.0f) && arg0->field_12 != 0 && arg0->field_14 != 0)
    {
        SetRotMatrix(&GsWSMATRIX);
        SetTransMatrix(&GsWSMATRIX);

        pos0.vx = Q12_TO_Q8(arg0->vec_0->vx);
        pos0.vy = Q12_TO_Q8(arg0->vec_0->vy + arg0->field_4);
        pos0.vz = Q12_TO_Q8(arg0->vec_0->vz);

        pos1.vx = Q12_TO_Q8(arg0->vec_8->vx);
        pos1.vy = Q12_TO_Q8(arg0->vec_8->vy + arg0->field_C);
        pos1.vz = Q12_TO_Q8(arg0->vec_8->vz);

        projDepth = Vw_TransformAndProjectPoint(&pos0, &screenPos0) * 4;
        Vw_TransformAndProjectPoint(&pos1, &screenPos1);

        var_a1 = Q8(16.0f);
        if (arg0->field_1E != 0)
        {
            var_a1 = Q8(16384.0f) / MAX(projDepth, Q8(1.5f));
        }

        func_800E2968(D_800F4B40.field_118, 16, 6, &screenPos0, &screenPos1, 0,
                      Math_MulFixed(Q12_TO_Q8(arg0->pos_10), var_a1, Q12_SHIFT), Q12(0.0f), Q12(1.0f),
                      D_800F4B40.field_18, arg0->field_12, arg0->field_14);
        func_800E2C28(D_800F4B40.field_118, 16, 6, 1, 1);
    }
}

void func_800E2664(s32 arg0, s16 arg1) // 0x800E2664
{
    VECTOR3 pos1;
    VECTOR3 pos0;
    q19_12  angle;

    if (arg1 > Q12_ANGLE_NORM_U(func_800E28F4()))
    {
        VECTOR3* srcPos;
        srcPos = &g_SysWork.npcs[4].position;

        angle   = Q12_ANGLE_NORM_U(func_800E28F4());
        pos0.vx = srcPos->vx + Q12_MULT(Math_Sin(angle), arg0);
        pos0.vz = srcPos->vz + Q12_MULT(Math_Cos(angle), arg0);
        pos0.vy = ((func_800E28F4() % (arg0 * 2)) - arg0) - Q12(1.0f);

        pos1     = g_SysWork.npcs[4].position;
        pos1.vy -= Q12(1.1f);
        pos1.vx += Q12_MULT_FLOAT(Math_Sin(angle), 0.4f);
        pos1.vz += Q12_MULT_FLOAT(Math_Cos(angle), 0.4f);

        func_800DD738(&pos0, &pos1, Q12_ANGLE(54.0f), Q12(1.0f));
    }
}

void func_800E27D0(s32 arg0, s16 arg1, s32 arg2, VECTOR3* pos) // 0x800E27D0
{
    VECTOR3* srcPos;
    VECTOR3  pos0;
    q19_12   angle;

    if (arg2 != 0 || arg1 > Q12_ANGLE_NORM_U(func_800E28F4()))
    {
        angle = Q12_ANGLE_NORM_U(func_800E28F4());
        if (arg2 != 0)
        {
            srcPos  = &g_SysWork.npcs[1].position;
            pos0.vx = srcPos->vx - Q12(0.5f);
            pos0.vy = srcPos->vy - Q12(1.2f);
            pos0.vz = srcPos->vz;
        }
        else
        {
            // @hack Pointer to `position` needed for match, but doesn't work as a `VECTOR3*` variable?
            pos0.vx = (&g_SysWork.npcs[4].position)->vx + Q12_MULT(Math_Sin(angle), arg0);
            pos0.vz = (&g_SysWork.npcs[4].position)->vz + Q12_MULT(Math_Cos(angle), arg0);
            pos0.vy = (func_800E28F4() % (arg0 >> 1)) - (arg0 >> 2);
        }

        func_800DD738(pos, &pos0, Q12_ANGLE(180.0f), Q12(0.5f));
    }
}

q3_12 func_800E28F4(void) // 0x800E28F4
{
    if (g_DeltaTime != Q12(0.0f))
    {
        D_800ED73C = ((s32)(D_800ED740 * 0x7169AC35) >> 3) ^ 0xA547B39E;
        D_800ED740 = ((u32)(D_800ED73C * 0x892D719C) >> 3) ^ 0xC65A4B97;
    }

    return D_800ED73C & SHRT_MAX;
}

void func_800E2968(s_800F4B40_118* arg0, s32 colCount, s32 rowCount, DVECTOR* arg3, DVECTOR* arg4,
                   s32 arg5, s32 arg6, q19_12 angle0, q19_12 angle1, s_PrimColor* color, s32 arg10, u8 arg11) // 0x800E2968
{
    s_PrimColor color0;
    s32         i;
    s32         j;
    s32         temp_lo;
    q19_12      angle2;
    s32         temp_s3;
    s32         temp_s4;
    s32         temp_s5;
    s32         temp_v0;
    s32         colorComp;

    for (i = 0; i < rowCount; i++)
    {
        temp_lo = Q12(i) / (rowCount - 1);
        temp_v0 = vwOresenHokan(D_800ED744, ARRAY_SIZE(D_800ED744), temp_lo, 0, Q12(1.0f));

        colorComp = Q12_MULT(arg10, Q12_MULT(color->r, temp_v0));
        if (colorComp < arg11)
        {
            color0.r = colorComp;
        }
        else
        {
            color0.r = arg11;
        }

        colorComp = Q12_MULT(arg10, Q12_MULT(color->g, temp_v0));
        if (colorComp < arg11)
        {
            color0.g = colorComp;
        }
        else
        {
            color0.g = arg11;
        }

        colorComp = Q12_MULT(arg10, Q12_MULT(color->b, temp_v0));
        if (colorComp < arg11)
        {
            color0.b = colorComp;
        }
        else
        {
            color0.b = arg11;
        }

        temp_s5 = arg3->vx + Q12_MULT(temp_lo, arg4->vx - arg3->vx);
        temp_s4 = arg3->vy + Q12_MULT(temp_lo, arg4->vy - arg3->vy);
        temp_s3 = arg5 + Q12_MULT(temp_lo, arg6 - arg5);

        for (j = 0; j < colCount; j++)
        {
            s_800F4B40_118* temp_s1;
            temp_s1 = &arg0[(colCount * i) + j];

            *(s32*)&temp_s1->color_4 = *(s32*)&color0;

            angle2  = angle0;
            angle2 += FP_FROM((Q12(j) / (colCount - 1)) * (angle1 - angle2), Q12_SHIFT);

            temp_s1->x_0 = temp_s5 + Q12_MULT(temp_s3, Math_Cos(angle2));
            temp_s1->y_2 = temp_s4 + Q12_MULT(temp_s3, Math_Sin(angle2));
        }
    }
}

void func_800E2C28(s_800F4B40_118* arg0, s32 colCount, s32 rowCount, s32 zDepth, s32 arg4) // 0x800E2C28
{
    s32             row;
    s32             col;
    POLY_G4*        poly;
    GsOT_TAG*       ot;
    s_800F4B40_118* colData;
    DR_MODE*        drMode;

    poly = GsOUT_PACKET_P;
    ot   = &g_OrderingTable0[g_ActiveBufferIdx].org[zDepth];

    for (row = 0; row < rowCount - 1; row++)
    {
        for (col = 0; col < colCount - 1; col++, poly++)
        {
            colData = &arg0[(row * colCount) + col];

            *(s32*)&poly->x0 = *(s32*)&colData[0].x_0;
            *(s32*)&poly->r0 = *(s32*)&colData[0].color_4;
            *(s32*)&poly->x1 = *(s32*)&colData[1].x_0;
            *(s32*)&poly->r1 = *(s32*)&colData[1].color_4;

            // Fetch x2/x3 from same column in next row
            *(s32*)&poly->x2 = *(s32*)&colData[colCount + 0].x_0;
            *(s32*)&poly->r2 = *(s32*)&colData[colCount + 0].color_4;
            *(s32*)&poly->x3 = *(s32*)&colData[colCount + 1].x_0;
            *(s32*)&poly->r3 = *(s32*)&colData[colCount + 1].color_4;

            setPolyG4(poly);
            setSemiTrans(poly, 1);

            addPrim(ot, poly);
        }
    }

    GsOUT_PACKET_P = poly;

    drMode = poly;
    SetDrawMode(drMode, 0, 1, (arg4 & 3) << 5, NULL);
    GsOUT_PACKET_P = &drMode[1];
    addPrim(ot, drMode);
}

#include "maps/common/shared/Event_CutsceneTimerAdvance.h" // 0x800E2DF8

void Map_CutsceneObjectsUpdate(void) // 0x800E2E90
{
    if (g_Cutscene_Timer >= Q12(0.0f))
    {
        if (g_Cutscene_UpdateHero)
        {
            Dms_CharacterTransformGet(&g_SysWork.playerWork.player.position, &g_SysWork.playerWork.player.rotation, "HERO", g_Cutscene_Timer, D_800ED230[D_800F4806]);
        }
        if (g_Cutscene_UpdateSibyl)
        {
            Dms_CharacterTransformGet(&g_SysWork.npcs[0].position, &g_SysWork.npcs[0].rotation, "SIBYL", g_Cutscene_Timer, D_800ED230[D_800F4806]);
        }
        if (g_Cutscene_UpdateDaria)
        {
            Dms_CharacterTransformGet(&g_SysWork.npcs[1].position, &g_SysWork.npcs[1].rotation, "DARIA", g_Cutscene_Timer, D_800ED230[D_800F4806]);
        }
        if (g_Cutscene_UpdateArsia)
        {
            Dms_CharacterTransformGet(&g_SysWork.npcs[2].position, &g_SysWork.npcs[2].rotation, "ARSIA", g_Cutscene_Timer, D_800ED230[D_800F4806]);
        }
        if (g_Cutscene_UpdateKau)
        {
            Dms_CharacterTransformGet(&g_SysWork.npcs[3].position, &g_SysWork.npcs[3].rotation, "KAU", g_Cutscene_Timer, D_800ED230[D_800F4806]);
        }
        if (g_Cutscene_UpdateMar)
        {
            Dms_CharacterTransformGet(&g_SysWork.npcs[4].position, &g_SysWork.npcs[4].rotation, "MAR", g_Cutscene_Timer, D_800ED230[D_800F4806]);
        }
        if (g_Cutscene_UpdateBar)
        {
            Dms_CharacterTransformGet(&g_SysWork.npcs[5].position, &g_SysWork.npcs[5].rotation, "BAR", g_Cutscene_Timer, D_800ED230[D_800F4806]);
        }
        if (g_Cutscene_UpdateLitl)
        {
            Dms_CharacterTransformGet(&g_SysWork.npcs[0].position, &g_SysWork.npcs[0].rotation, "LITL", g_Cutscene_Timer, D_800ED230[D_800F4806]);
        }
        if (g_Cutscene_UpdateBos)
        {
            Dms_CharacterTransformGet(&g_SysWork.npcs[2].position, &g_SysWork.npcs[2].rotation, "BOS", g_Cutscene_Timer, D_800ED230[D_800F4806]);
        }
        if (g_Cutscene_UpdateBin)
        {
            Dms_CharacterTransformGet(&g_WorldObject_Bin.position, &g_WorldObject_Bin.rotation, "BIN", g_Cutscene_Timer, D_800ED230[D_800F4806]);
        }
        if (g_Cutscene_UpdateBaby)
        {
            Dms_CharacterTransformGet(&g_WorldObject_Baby.position, &g_WorldObject_Baby.rotation, "BABY", g_Cutscene_Timer, D_800ED230[D_800F4806]);
        }

        vcChangeProjectionValue(Dms_CameraTargetsGet(&g_Cutscene_CameraPositionTarget, &g_Cutscene_CameraLookAtTarget, NULL, g_Cutscene_Timer, D_800ED230[D_800F4806]));
        vcUserCamTarget(&g_Cutscene_CameraPositionTarget, NULL, true);
        vcUserWatchTarget(&g_Cutscene_CameraLookAtTarget, NULL, true);

        // "LIGHT", cutscene light position?
        Dms_CharacterTransformGet(&g_SysWork.lightPosition, &D_800F47E8, "LIGHT", g_Cutscene_Timer, D_800ED230[D_800F4806]);

        // "L_INT", interior light or intersection point?
        Dms_CharacterTransformGet(&D_800F47D8, &D_800F47E8, "L_INT", g_Cutscene_Timer, D_800ED230[D_800F4806]);

        // Set light rotation.
        g_SysWork.lightRotation.vx = -ratan2(D_800F47D8.vy - g_SysWork.lightPosition.vy,
                                             Math_Vector2MagCalcSafeQ6(D_800F47D8.vx - g_SysWork.lightPosition.vx,
                                                                       D_800F47D8.vz - g_SysWork.lightPosition.vz));
        g_SysWork.lightRotation.vy = ratan2(D_800F47D8.vx - g_SysWork.lightPosition.vx,
                                            D_800F47D8.vz - g_SysWork.lightPosition.vz);
        g_SysWork.lightRotation.vz = Q12_ANGLE(0.0f);
    }
}

void func_800E3390(void) // 0x800E3390
{
    s32 i;

    if (Pad_ButtonClickedBind(skip) &&
        D_800F4805 > 0 && D_800F4805 < 4)
    {
        D_800F4805 = 5;
        Savegame_EventFlagClear(EventFlag_576);
        SD_Call(19);
    }

    switch (D_800F4805)
    {
        case 0:
            // Freeze player.
            Player_ControlFreeze();
            func_8003A16C();
            WorldEnv_PresetsSet(18, 18);
            func_8008D438();

            g_Cutscene_UpdateSibyl = false;
            g_Cutscene_UpdateDaria = false;
            g_Cutscene_UpdateArsia = false;
            g_Cutscene_UpdateKau   = false;
            g_Cutscene_UpdateMar   = false;
            g_Cutscene_UpdateBar   = false;
            g_Cutscene_UpdateLitl  = false;
            g_Cutscene_UpdateBos   = false;
            g_Cutscene_UpdateBin   = false;
            g_Cutscene_UpdateNu    = true;
            g_Cutscene_UpdateBaby  = false;
            D_800F4813 = 0;
            D_800F4814 = 0;
            D_800F4815 = 0;
            D_800F4816 = 0;
            D_800F4817 = 0;
            D_800F4818 = false;
            D_800F4819 = 0;
            D_800F481A = false;
            D_800F481B = 0;

            Anim_CharaTypeAnimInfoClear();
            WorldGfx_CharaLmBufferAssign(CHARA_FORCE_FREE_ALL);

            Screen_FadeTimestepReset();
            CutsceneBorder_ForceShow();
            g_SysWork.sysFlags |= SysFlag_CutsceneActive;

            func_800E0A34();

            Fs_QueueStartRead(FILE_ANIM_LAST12_DMS, FS_BUFFER_20);
            Fs_QueueWaitForEmpty();
            Dms_HeaderFixOffsets(FS_BUFFER_20);

            D_800F4806 = 0;
            g_Cutscene_Timer = Q12(0.0f);

            func_8003D03C();
            Player_EmptyWeaponHandSet();
            func_800E94AC();
            func_800E94F4();
            Anim_CharaTypeAnimInfoClear();

            Savegame_EventFlagSet(EventFlag_576);
            Savegame_EventFlagSet(EventFlag_591);
            D_800F4805++;

        case 1:
            if (Savegame_EventFlagGet(EventFlag_449))
            {
                func_800E3F30();
            }
            else
            {
                D_800F4805++;
                func_800E4714();
            }
            break;

        case 2:
            func_800E4714();
            break;

        case 3:
            if (Savegame_EventFlagGet(EventFlag_M5S03_KaufmannHasMotorcycleVial))
            {
                func_800E514C();
            }
            else
            {
                func_800E70F0();
            }
            break;

        case 5:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 0, Q12(0.0f), false);

            if (Savegame_EventFlagGet(EventFlag_M5S03_KaufmannHasMotorcycleVial))
            {
                D_800F4805++;
                SysWork_StateStepSet(0, 0);
            }
            else
            {
                D_800F4805 += 2;
            }

        case 6:
            Bgm_CrossfadeToTrack(BgmCmd_Track40);

            if (g_SysWork.sysStateSteps[0] != 0)
            {
                D_800F4805++;
            }
            break;

        case 7:
            if (Screen_IsFadeFinished())
            {
                D_800F4805++;
            }
            break;

        case 8:
            for (i = 0; i < ARRAY_SIZE(g_SysWork.npcs); i++)
            {
                func_800E9490(&g_SysWork.npcs[i]);
            }

            if (Savegame_EventFlagGet(EventFlag_M5S03_KaufmannHasMotorcycleVial))
            {
                if (!D_800F4815)
                {
                    Fs_QueueStartRead(FILE_ANIM_LAST3_DMS, FS_BUFFER_18);
                }

                Fs_QueueWaitForEmpty();
                Dms_HeaderFixOffsets(FS_BUFFER_18);

                if (!D_800F4816)
                {
                    Fs_QueueStartRead(FILE_ANIM_LAST4_DMS, FS_BUFFER_20);
                    Fs_QueueWaitForEmpty();
                }

                D_800F4806 = 1;

                if (D_800F4813 != 2)
                {
                    if (!D_800F4813)
                    {
                        WorldGfx_CharaLmBufferAssign(CHARA_FORCE_FREE_ALL);
                        func_800E9260(Chara_Incubus, 1);
                    }

                    func_800E941C();
                }

                func_800E9444(Chara_Incubus, &g_SysWork.npcs[2]);
                func_800DD9B0(&g_SysWork.npcs[2]);

                g_Cutscene_UpdateBaby  = false;
                g_Cutscene_UpdateNu    = false;
                g_Cutscene_UpdateBin   = false;
                g_Cutscene_UpdateLitl  = false;
                g_Cutscene_UpdateBar   = false;
                g_Cutscene_UpdateMar   = false;
                g_Cutscene_UpdateKau   = false;
                g_Cutscene_UpdateArsia = false;
                g_Cutscene_UpdateDaria = false;
                g_Cutscene_UpdateSibyl = false;
                g_Cutscene_UpdateBos   = true;
                g_Cutscene_Timer       = Q12(670.0f);
            }
            else
            {
                if (!D_800F4817)
                {
                    Fs_QueueStartRead(FILE_ANIM_LAST5_DMS, FS_BUFFER_18);
                }

                Fs_QueueWaitForEmpty();
                Dms_HeaderFixOffsets(FS_BUFFER_18);

                D_800F4806 = 1;
                if (D_800F4813 != 2)
                {
                    if (!D_800F4813)
                    {
                        WorldGfx_CharaLmBufferAssign(CHARA_FORCE_FREE_ALL);
                        func_800E9260(Chara_Incubator, 1);
                    }

                    func_800E941C();
                }

                func_800E9444(Chara_Incubator, &g_SysWork.npcs[4]);
                func_800E9498();

                g_Cutscene_UpdateBaby  = false;
                g_Cutscene_UpdateNu    = false;
                g_Cutscene_UpdateBin   = false;
                g_Cutscene_UpdateLitl  = false;
                g_Cutscene_UpdateBar   = false;
                g_Cutscene_UpdateBos   = false;
                g_Cutscene_UpdateKau   = false;
                g_Cutscene_UpdateArsia = false;
                g_Cutscene_UpdateDaria = false;
                g_Cutscene_UpdateSibyl = false;
                g_Cutscene_UpdateMar   = true;
                g_Cutscene_Timer       = Q12(68.0f);

                func_800DFB50(&g_SysWork.npcs[4]);
            }

            func_800D9394();
            Fs_QueueWaitForEmpty();

            g_Cutscene_UpdateHero = true;

            Model_AnimFlagsSet(&g_SysWork.playerWork.player.model, 2);
            Map_CutsceneObjectsUpdate();

            D_800F4805 = NO_VALUE;

            if (Savegame_EventFlagGet(EventFlag_M5S03_KaufmannHasMotorcycleVial))
            {
                func_800E1788(-1);
            }
            else
            {
                func_800E1788(6);
            }

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);

        default:
            Player_ControlUnfreeze(true);
            SysWork_StateSetNext(SysState_Gameplay);
            vcReturnPreAutoCamWork(true);

            if (Savegame_EventFlagGet(EventFlag_M5S03_KaufmannHasMotorcycleVial))
            {
                func_800DD9D4(&g_SysWork.npcs[2]);
            }
            else
            {
                func_800E9498();
                func_800DFB74(&g_SysWork.npcs[4]);
            }

            Savegame_EventFlagSet(EventFlag_577);

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);

            g_Cutscene_Timer = NO_VALUE;

            func_8003D01C();
            Player_WeaponAttackRestore();
            SD_Call(19);
            func_800E14DC(&g_SysWork.playerWork.player, &g_SysWork.npcs[2], 1);

            D_800F4805               = 0;
            g_SysWork.lightIntensity = Q12(1.0f);
            break;
    }

    Map_CutsceneObjectsUpdate();

    if (g_Cutscene_UpdateMar && D_800F4805 == 3 && Savegame_EventFlagGet(EventFlag_M5S03_KaufmannHasMotorcycleVial))
    {
        g_SysWork.npcs[4].rotation.vy += Q12_ANGLE(180.0f);
    }
}

void func_800E3B6C(void) // 0x800E3B6C
{
    switch (D_800F4805)
    {
        case 0:
            Player_ControlFreeze();

            CutsceneBorder_ForceShow();
            g_SysWork.sysStateSteps[0] = 0;
            g_SysWork.sysFlags       |= SysFlag_CutsceneActive;

            func_8003D03C();
            Player_EmptyWeaponHandSet();
            func_8005E70C();

            D_800F4805++;

        case 1:
            func_800E62CC();
            break;

        default:
            SysWork_StateSetNext(SysState_Gameplay);
            D_800F4805 = 0;
            Savegame_EventFlagSet(EventFlag_579);
            break;
    }

    Map_CutsceneObjectsUpdate();
}

void func_800E3C48(void) // 0x800E3C48
{
    switch (D_800F4805)
    {
        case 0:
            if (Savegame_EventFlagGet(EventFlag_449))
            {
                func_800E787C();
                Savegame_EventFlagSet(EventFlag_583);
                Savegame_EventFlagSet(EventFlag_587);
            }
            else
            {
                func_800E86BC();
                Savegame_EventFlagSet(EventFlag_585);
                Savegame_EventFlagSet(EventFlag_587);
            }
            g_SysWork.playerWork.player.health = Q12(100.0f);
            break;

        default:
            SysWork_StateSetNext(SysState_Gameplay);
            D_800F4805 = 0;
            break;
    }

    D_800C48F0 += g_VBlanks;
    Map_CutsceneObjectsUpdate();
}

void func_800E3D18(void) // 0x800E3D18
{
    switch (D_800F4805)
    {
        case 0:
            Player_ControlFreeze();
            g_SysWork.playerWork.player.health = Q12(100.0f);

            CutsceneBorder_ForceShow();
            g_SysWork.sysStateSteps[0] = 0;
            g_SysWork.sysFlags        |= SysFlag_CutsceneActive;

            func_8003D03C();
            Player_EmptyWeaponHandSet();
            func_8005E70C();
            D_800F4805++;

        case 1:
            func_800E7380();
            break;

        case 2:
            if (Savegame_EventFlagGet(EventFlag_449))
            {
                D_800F4805 = 3;
                Savegame_EventFlagSet(EventFlag_584);
                break;
            }

            D_800F4805 = 3;

            Savegame_EventFlagSet(EventFlag_586);
            Sd_SfxStop(Sfx_Unk1688);
            Sd_SfxStop(Sfx_Unk1689);
            Sd_SfxStop(Sfx_Unk1690);

            D_800F4820 = 0;

        default:
            SysWork_StateSetNext(SysState_Gameplay);
            D_800F4805 = 0;

            Savegame_EventFlagSet(EventFlag_587);
            break;
    }

    Map_CutsceneObjectsUpdate();
}

void func_800E3E84(void) // 0x800E3E84
{
    switch (D_800F4805)
    {
        case 0:
            CutsceneBorder_ForceShow();
            g_SysWork.sysStateSteps[0] = 0;
            D_800F4805                 = 1;
            g_SysWork.sysFlags        |= SysFlag_CutsceneActive;

        case 1:
            func_800E8D20();
            break;

        default:
            SysWork_StateSetNext(SysState_Gameplay);
            Savegame_EventFlagSet(EventFlag_590);
            break;
    }

    Map_CutsceneObjectsUpdate();
}

void func_800E3F30(void) // 0x800E3F30
{
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
    s32                             i;

    if (g_SysWork.sysStateSteps[0] == 0)
    {
        D_800EDA04 = 0;
    }

    if (D_800EDA04 != 0)
    {
        scratchData                     = PSX_SCRATCH_ADDR(0);
        scratchData->activeBufferIdx_14 = g_ActiveBufferIdx;
        scratchData->sprt_0             = (SPRT*)GsOUT_PACKET_P;

        for (i = 0; i < 2; i++)
        {
            setCodeWord(scratchData->sprt_0, PRIM_RECT | RECT_BLEND | RECT_TEXTURE, COLOR_RGBC(128, 128, 128, 0));
            setXY0Fast(scratchData->sprt_0, ((i << 8) - (SCREEN_WIDTH / 2)), -(FRAMEBUFFER_HEIGHT_PROGRESSIVE / 2));
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
            g_Cutscene_MapMsgAudioIdx = 0;

            func_800E9260(Chara_EndingCybil, 1);
            func_800E9260(Chara_Alessa, 2);
            func_800E9260(Chara_EndingDahlia, 3);
            func_800E941C();
            func_800E9444(Chara_EndingCybil, &g_SysWork.npcs[0]);
            func_800E9444(Chara_EndingDahlia, &g_SysWork.npcs[1]);
            func_800E9444(Chara_Alessa, &g_SysWork.npcs[2]);

            g_Cutscene_UpdateArsia = true;
            g_Cutscene_UpdateDaria = true;
            g_Cutscene_UpdateSibyl = true;

            g_SysWork.lightIntensity = Q12(0.7f);

            Model_AnimFlagsClear(&g_SysWork.playerWork.player.model, 2);

            SysWork_StateStepIncrement(0);
            D_800EDA04 = 0;
            break;

        case 1:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 21, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[1], 5, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[2], 10, false);
            SysWork_StateStepIncrement(0);
            break;

        case 2:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimLock, &g_SysWork.npcs[0], 0, false);
            SysWork_StateStepIncrement(0);

        case 3:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, 0, false);
            break;

        case 4:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimUnlock, &g_SysWork.npcs[0], 0, false);
            SysWork_StateStepIncrement(0);

        case 5:
            Event_DisplayMapMsgWithAudio(15, &g_Cutscene_MapMsgAudioIdx, &D_800ED768);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(0.0f), Q12(16.0f), true, false);
            break;

        case 6:
            SysWork_StateStepIncrement(0);

        case 7:
            Event_DisplayMapMsgWithAudio(21, &g_Cutscene_MapMsgAudioIdx, &D_800ED768);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(17.0f), Q12(31.0f), true, false);
            break;

        case 8:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(17.0f), Q12(31.0f), true, true);
            break;

        case 9:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 22, false);
            SysWork_StateStepIncrement(0);

        case 10:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(32.0f), Q12(37.0f), false, true);
            break;

        case 11:
            Savegame_EventFlagClear(EventFlag_576);
            SysWork_StateStepIncrement(0);
            func_800D6804(&g_SysWork.npcs[1].position, &g_SysWork.npcs[0].position);

        case 12:
            Event_DisplayMapMsgWithAudio(22, &g_Cutscene_MapMsgAudioIdx, &D_800ED768);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(32.0f), Q12(59.0f), true, false);
            func_800D6788();
            break;

        case 13:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 23, false);
            SysWork_StateStepIncrement(0);

        case 14:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(32.0f), Q12(59.0f), true, false);
            func_800D6788();
            Event_WaitTimer(Q12(1.0f), false);
            break;

        case 15:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 24, false);
            Event_DisplayMapMsgWithAudio(23, &g_Cutscene_MapMsgAudioIdx, &D_800ED768);

            g_SysWork.lightIntensity = Q12(0.6f);
            D_800EDA04               = 1;

            SysWork_StateStepIncrement(0);

        case 16:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(61.0f), Q12(91.0f), true, true);
            break;

        case 17:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, 0, false);
            break;

        default:
            g_SysWork.sysStateSteps[0] = 0;
            D_800F4805++;

            Model_AnimFlagsSet(&g_SysWork.playerWork.player.model, 2);
            func_800E9490(&g_SysWork.npcs[0]);

            g_Cutscene_UpdateSibyl = false;
            break;
    }
}

void func_800E4714(void) // 0x800E4714
{
    s32 playerPlaybackState;

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            g_Cutscene_UpdateHero = true;
            D_800F4828 = 0;

            if (!Savegame_EventFlagGet(EventFlag_449))
            {
                func_800E9260(Chara_Alessa, 2);
                func_800E9260(Chara_EndingDahlia, 3);
                func_800E941C();
                func_800E9444(Chara_EndingDahlia, &g_SysWork.npcs[1]);
                func_800E9444(Chara_Alessa, &g_SysWork.npcs[2]);

                g_Cutscene_UpdateArsia = true;
                g_Cutscene_UpdateDaria = true;
            }

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 153, false);

            g_Cutscene_MapMsgAudioIdx = 0;
            g_Cutscene_PrevPlayerPlaybackState = AnimPlaybackState_Active;

            Savegame_EventFlagSet(EventFlag_592);
            g_SysWork.lightIntensity = Q12(0.8f);
            SysWork_StateStepIncrement(0);
            break;

        case 1:
            if (!Savegame_EventFlagGet(EventFlag_449))
            {
                Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[1], 5, false);
                Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[2], 10, false);
            }

            SysWork_StateStepIncrement(0);

        case 2:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.0f), false);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(92.0f), Q12(112.0f), true, false);
            break;

        case 3:
            Event_DisplayMapMsgWithAudio(24, &g_Cutscene_MapMsgAudioIdx, &g_Cutscene_MapMsgAudioCmds);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(92.0f), Q12(112.0f), true, false);

            playerPlaybackState = Player_AnimPlaybackStateGet();
            if (playerPlaybackState                == AnimPlaybackState_End &&
                g_Cutscene_PrevPlayerPlaybackState == AnimPlaybackState_Active)
            {
                Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 131, false);
                g_Cutscene_PrevPlayerPlaybackState = playerPlaybackState;
            }
            break;

        case 4:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[1], 15, false);
            SysWork_StateStepIncrement(0);

        case 5:
            Event_DisplayMapMsgWithAudio(30, &g_Cutscene_MapMsgAudioIdx, &g_Cutscene_MapMsgAudioCmds);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(113.0f), Q12(129.0f), true, false);
            break;

        case 6:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(113.0f), Q12(129.0f), true, true);
            break;

        case 7:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 70, false);
            func_800E9260(Chara_Incubator, 1);
            D_800F4814 = 1;

            SysWork_StateStepIncrement(0);

        case 8:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(113.0f), Q12(134.0f), false, true);
            break;

        case 9:
            if (Fs_QueueChunksLoad())
            {
                SysWork_StateStepIncrement(0);
            }
            break;

        case 10:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 51, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[1], 16, false);
            SysWork_StateStepIncrement(0);

        case 11:
            Event_DisplayMapMsgWithAudio(31, &g_Cutscene_MapMsgAudioIdx, &g_Cutscene_MapMsgAudioCmds);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(135.0f), Q12(142.0f), true, false);
            break;

        case 12:
            Event_DisplayMapMsgWithAudio(35, &g_Cutscene_MapMsgAudioIdx, &g_Cutscene_MapMsgAudioCmds);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(135.0f), Q12(142.0f), true, false);
            break;

        case 13:
            Event_DisplayMapMsgWithAudio(44, &g_Cutscene_MapMsgAudioIdx, &g_Cutscene_MapMsgAudioCmds);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(135.0f), Q12(142.0f), true, false);
            break;

        case 14:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[1], 6, false);
            SysWork_StateStepIncrement(0);

        case 15:
            Event_DisplayMapMsgWithAudio(49, &g_Cutscene_MapMsgAudioIdx, &g_Cutscene_MapMsgAudioCmds);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(8.5f), Q12(143.0f), Q12(207.0f), true, false);

            Savegame_EventFlagClear(EventFlag_576);
            Savegame_EventFlagSet(EventFlag_593);
            Savegame_EventFlagSet(EventFlag_575);

            if (g_SysWork.sysStateSteps[0] != 15)
            {
                SD_Call(19);
            }
            break;

        case 16:
            SD_Call(Sfx_XaAudio600);
            SysWork_StateStepIncrement(0);

        case 17:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(8.5f), Q12(143.0f), Q12(207.0f), true, false);

            if (Sd_AudioStreamingCheck() == AudioStreamingState_XaPlaying)
            {
                SysWork_StateStepIncrement(0);
            }
            break;

        case 18:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(8.5f), Q12(143.0f), Q12(207.0f), true, false);

            if (Sd_AudioStreamingCheck() == AudioStreamingState_XaPlaying)
            {
                SysWork_StateStepIncrement(0);
            }
            else
            {
                break;
            }

        case 19:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 171, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[1], 18, false);
            func_800E1788(1);
            SysWork_StateStepIncrement(0);

        case 20:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(208.0f), Q12(231.0f), true, true);
            break;

        case 21:
            g_Cutscene_UpdateDaria = false;

            Model_AnimFlagsClear(&g_SysWork.npcs[1].model, 2);
            func_800E941C();
            func_800E9444(Chara_Incubator, &g_SysWork.npcs[4]);

            D_800F4814 = 2;

            Model_AnimFlagsClear(&g_SysWork.npcs[4].model, 2);
            SysWork_StateStepIncrement(0);

        case 22:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(232.0f), Q12(287.0f), false, true);

            if (g_Cutscene_Timer > Q12(230.0f))
            {
                if (D_800F481B == 0)
                {
                    Model_AnimFlagsClear(&g_SysWork.npcs[2].model, 2);
                    func_800E1788(2);
                    D_800F481B++;
                }
            }

            if (g_Cutscene_Timer > Q12(260.0f) && D_800F481B == 1)
            {
                D_800F481B++;
            }

            if (g_Cutscene_Timer > Q12(285.0f))
            {
                if (D_800F481B == 2)
                {
                    func_800E1788(3);
                    D_800F481B++;
                }
            }
            break;

        case 23:
            func_800E9490(&g_SysWork.npcs[2]);

            g_Cutscene_UpdateArsia = false;
            g_Cutscene_UpdateNu    = false;

            SysWork_StateStepIncrement(0);

        case 24:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(232.0f), Q12(292.0f), false, true);
            break;

        case 25:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[4], 1, false);

            g_Cutscene_UpdateMar = true;

            Model_AnimFlagsSet(&g_SysWork.npcs[4].model, 2);
            func_800DD7D0(&g_SysWork.npcs[1].position);

            D_800F4828 = 1;

            SysWork_StateStepIncrement(0);

        case 26:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(232.0f), Q12(332.0f), false, true);
            break;

        case 27:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[4], 2, false);
            SysWork_StateStepIncrement(0);

        case 28:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(232.0f), Q12(341.0f), true, true);
            break;

        case 29:
            g_Cutscene_UpdateArsia = false;
            func_800D9394();

            if (Savegame_EventFlagGet(EventFlag_M5S03_KaufmannHasMotorcycleVial))
            {
                Fs_QueueStartRead(FILE_ANIM_LAST3_DMS, FS_BUFFER_18);
                D_800F4815 = 1;
            }
            else
            {
                Fs_QueueStartRead(FILE_ANIM_LAST5_DMS, FS_BUFFER_18);
                D_800F4817 = 1;
            }

            SysWork_StateStepIncrement(0);

        case 30:
            Event_WaitTimer(Q12(4.0f), false);
            break;

        default:
            g_SysWork.sysStateSteps[0] = 0;
            D_800F4805++;
            break;
    }

    if (D_800F4828 != Q12(0.0f))
    {
        func_800DD868();
    }
}

void func_800E514C(void) // 0x800E514C
{
    VECTOR3 pos; // Q19.12
    s32     localRand;

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Fs_QueueWaitForEmpty();

            g_Cutscene_MapMsgAudioIdx = 0;

            Dms_HeaderFixOffsets(FS_BUFFER_18);

            D_800F4806 = 1;
            g_Cutscene_Timer = Q12(0.0f);

            func_800E9260(Chara_EndingKaufmann, 2);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 51, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[1], 19, false);

            Sfx_WithFlagsPlay(Sfx_Unk1670, &g_SysWork.npcs[1].position, Q8_CLAMPED(0.785f), SfxFlag_None);

            g_SysWork.lightIntensity = Q12(0.7f);

            Model_AnimFlagsSet(&g_SysWork.npcs[1].model, 2);

            Savegame_EventFlagClear(EventFlag_591);
            Savegame_EventFlagClear(EventFlag_592);
            Savegame_EventFlagClear(EventFlag_593);
            Savegame_EventFlagClear(EventFlag_575);
            Savegame_EventFlagSet(EventFlag_576);

            g_Cutscene_UpdateDaria = true;
            SysWork_StateStepIncrement(0);

        case 1:
            Bgm_CrossfadeToTrack(BgmCmd_Track29);
            break;

        case 2:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(0.0f), Q12(12.0f), true, true);
            break;

        case 3:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(13.0f), Q12(37.0f), true, true);
            break;

        case 4:
            func_800E941C();
            func_800E9444(Chara_EndingKaufmann, &g_SysWork.npcs[3]);
            g_Cutscene_UpdateKau = true;

            SysWork_StateStepIncrement(0);
            break;

        case 5:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[3], 25, false);
            SysWork_StateStepIncrement(0);

        case 6:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(38.0f), Q12(51.0f), true, true);
            break;

        case 7:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(52.0f), Q12(58.0f), true, true);
            break;

        case 8:
            g_Cutscene_Timer = Q12(59.0f);

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[3], 24, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[1], 20, false);
            Savegame_EventFlagSet(EventFlag_591);
            SysWork_StateStepIncrement(0);

        case 9:
            Event_DisplayMapMsgWithAudio(51, &g_Cutscene_MapMsgAudioIdx, &D_800ED7B4);
            break;

        case 10:
            Event_CharaAnimPlayToEnd(&g_SysWork.npcs[1], 21);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(60.0f), Q12(78.0f), true, false);
            break;

        case 11:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[1], 22, false);
            SysWork_StateStepIncrement(0);

        case 12:
            Event_DisplayMapMsgWithAudio(57, &g_Cutscene_MapMsgAudioIdx, &D_800ED7B4);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(60.0f), Q12(78.0f), true, false);
            break;

        case 13:
            Event_CharaAnimPlayToEnd(&g_SysWork.npcs[3], 18);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(79.0f), Q12(100.0f), true, true);
            break;

        case 14:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[3], 19, false);
            g_Cutscene_Timer = Q12(101.0f);
            SysWork_StateStepIncrement(0);

        case 15:
            Event_DisplayMapMsgWithAudio(61, &g_Cutscene_MapMsgAudioIdx, &D_800ED7B4);
            break;

        case 16:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[1], 23, false);
            g_SysWork.lightIntensity = Q12(0.6f);
            Savegame_EventFlagClear(EventFlag_591);
            SysWork_StateStepIncrement(0);

        case 17:
            Event_DisplayMapMsgWithAudio(66, &g_Cutscene_MapMsgAudioIdx, &D_800ED7B4);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(102.0f), Q12(116.0f), true, false);
            break;

        case 18:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(102.0f), Q12(116.0f), true, true);
            break;

        case 19:
            func_800E9490(&g_SysWork.npcs[1]);
            func_800E9490(&g_SysWork.npcs[4]);

            g_Cutscene_UpdateMar   = false;
            g_Cutscene_UpdateDaria = false;

            WorldGfx_CharaLmBufferAssign(1);
            func_800E9260(Chara_Incubator, 3);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[3], 20, false);

            g_SysWork.lightIntensity = Q12(0.7f);

            SysWork_StateStepIncrement(0);

        case 20:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(117.0f), Q12(156.0f), true, true);
            break;

        case 21:
            g_Cutscene_UpdateBin = true;
            D_800F482C = Q12_ANGLE(-167.6f);

            if (g_SysWork.sysStateSteps[0] != 18)
            {
                Savegame_EventFlagClear(EventFlag_576);
            }

            Bgm_PlayNewSong(BgmCmd_Track40);
            SysWork_StateStepIncrement(0);

        case 22:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(157.0f), Q12(193.0f), true, true);
            break;

        case 23:
            func_800E941C();
            func_800E9444(Chara_Incubator, &g_SysWork.npcs[4]);
            func_800E9260(Chara_BloodyIncubator, 1);
            SysWork_StateStepIncrement(0);

        case 24:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(7.5f), Q12(194.0f), Q12(280.0f), true, true);
            break;

        case 25:
            if (Fs_QueueChunksLoad())
            {
                SysWork_StateStepIncrement(0);
            }
            else
            {
                break;
            }

        case 26:
            g_Cutscene_UpdateMar = true;

            func_800E941C();
            func_800E9444(Chara_BloodyIncubator, &g_SysWork.npcs[5]);
            Model_AnimFlagsClear(&g_SysWork.npcs[5].model, 2);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[4], 2, false);
            WorldGfx_CharaModelTransparentSet(Chara_BloodyIncubator, false);

            g_SysWork.lightIntensity = Q12(0.6f);

            Sd_XaPreLoadAudioPreTaskAdd(Sfx_XaAudio606);
            SysWork_StateStepIncrement(0);

        case 27:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(281.0f), Q12(313.0f), false, true);
            break;

        case 28:
            SysWork_StateStepIncrement(0);

        case 29:
            SysWork_StateStepIncrement(0);

        case 30:
            SD_Call(Sfx_XaAudio606);
            SysWork_StateStepIncrement(0);

        case 31:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(281.0f), Q12(320.0f), true, false);
            if (Sd_AudioStreamingCheck() != AudioStreamingState_XaPlaying)
            {
                SysWork_StateStepIncrement(0);
            }
            break;

        case 32:
            if (Sd_AudioStreamingCheck() != AudioStreamingState_XaPlaying)
            {
                Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(281.0f), Q12(320.0f), true, false);
                break;
            }

            SysWork_StateStepIncrement(0);

        case 33:
            func_800D7144(&g_WorldObject_Bin.position);
            func_800E1788(9);
            SysWork_StateStepIncrement(0);

        case 34:
            g_Cutscene_Timer = Q12(321.0f);
            Event_WaitTimer(Q12(1.0f), false);

            g_Cutscene_UpdateBin = false;
            func_800D70EC();
            break;

        case 35:
            g_Cutscene_UpdateMar = false;

            func_800E9490(&g_SysWork.npcs[4]);

            g_Cutscene_UpdateBar = true;

            Model_AnimFlagsSet(&g_SysWork.npcs[5].model, 2);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[5], 1, false);
            SysWork_StateStepIncrement(0);

        case 36:
            if (Sd_AudioStreamingCheck() == AudioStreamingState_None)
            {
                func_800E9260(Chara_EndingDahlia, 3);
                SysWork_StateStepIncrement(0);
            }

            func_800D70EC();
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(322.0f), Q12(391.0f), true, false);
            break;

        case 37:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(322.0f), Q12(391.0f), true, true);

            if (g_SysWork.sysStateSteps[0] != 37)
            {
                Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 51, false);
            }
            break;

        case 38:
            if (Fs_QueueChunksLoad())
            {
                SysWork_StateStepIncrement(0);
            }

            g_SysWork.lightIntensity = Q12(0.7f);
            g_Cutscene_Timer         = Q12(392.0f);
            break;

        case 39:
            Event_DisplayMapMsgWithAudio(68, &g_Cutscene_MapMsgAudioIdx, &D_800ED7B4);
            Savegame_EventFlagSet(EventFlag_591);
            break;

        case 40:
            func_800E941C();
            func_800E9444(Chara_EndingDahlia, &g_SysWork.npcs[1]);
            SysWork_StateStepIncrement(0);
            break;

        case 41:
            g_Cutscene_UpdateDaria = true;
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[1], 22, false);

            g_SysWork.lightIntensity = Q12(0.6f);
            g_Cutscene_Timer         = Q12(393.0f);

            SysWork_StateStepIncrement(0);

        case 42:
            Event_DisplayMapMsgWithAudio(70, &g_Cutscene_MapMsgAudioIdx, &D_800ED7B4);
            break;

        case 43:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[3], 22, false);

            g_SysWork.lightIntensity = Q12(0.7f);
            g_Cutscene_UpdateDaria   = false;

            func_800E9490(&g_SysWork.npcs[1]);
            SysWork_StateStepIncrement(0);

        case 44:
            Event_DisplayMapMsgWithAudio(71, &g_Cutscene_MapMsgAudioIdx, &D_800ED7B4);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(394.0f), Q12(417.0f), true, false);
            break;

        case 45:
            func_800E9260(Chara_LittleIncubus, 3);
            SysWork_StateStepIncrement(0);

        case 46:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(394.0f), Q12(417.0f), true, true);
            break;

        case 47:
            if (Fs_QueueChunksLoad())
            {
                SysWork_StateStepIncrement(0);
            }
            break;

        case 48:
            func_800E941C();
            func_800E9444(Chara_LittleIncubus, g_SysWork.npcs);

            g_Cutscene_UpdateLitl = true;

            WorldGfx_CharaModelTransparentSet(Chara_BloodyIncubator, true);
            Savegame_EventFlagSet(EventFlag_576);

            g_SysWork.lightIntensity = Q12(0.5f);

            SysWork_StateStepIncrement(0);

        case 49:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(418.0f), Q12(498.0f), true, true);

            g_SysWork.sysStateStepData[0] += g_DeltaTime;
            g_SysWork.npcs[5].timer_C6    += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 1.0f / 16.0f);

            if (g_SysWork.sysStateStepData[0] > Q12(0.3f))
            {
                localRand           = Rng_Rand16();
                pos.vx              = g_SysWork.npcs[0].position.vx + Q12(0.2f);
                pos.vy              = g_SysWork.npcs[0].position.vy + Q12(-0.9f);
                pos.vz              = g_SysWork.npcs[0].position.vz + Q12(0.2f);

                g_SysWork.sysStateStepData[0] -= Q12(0.2f);
                g_SysWork.sysStateStepData[0] -= Rng_GenerateIntFromInput(localRand, 0, Q12(0.1f) - 1);

                func_8005F6B0(g_SysWork.npcs, &pos, 8, 1);
            }
            break;

        case 50:
            func_800E9490(&g_SysWork.npcs[5]);
            func_800E9490(&g_SysWork.npcs[0]);

            g_Cutscene_UpdateLitl = false;
            g_Cutscene_UpdateBar  = false;

            func_800E9260(Chara_Incubus, 1);

            D_800F4813 = 1;

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 171, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[3], 24, false);
            func_8005E70C();

            g_SysWork.lightIntensity = Q12(0.7f);
            SysWork_StateStepIncrement(0);

        case 51:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(499.0f), Q12(509.0f), true, true);
            break;

        case 52:
            func_800E941C();
            func_800E9444(Chara_Incubus, &g_SysWork.npcs[2]);

            D_800F4813 = 2;
            g_Cutscene_UpdateBos = true;

            SysWork_StateStepIncrement(0);
            break;

        case 53:
            func_800DD9B0(&g_SysWork.npcs[2]);
            func_800E9260(Chara_EndingDahlia, 3);

            g_SysWork.lightIntensity = Q12(0.8f);

            SysWork_StateStepIncrement(0);

        case 54:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(510.0f), Q12(539.0f), true, true);
            break;

        case 55:
            func_800E941C();
            func_800E9444(Chara_EndingDahlia, &g_SysWork.npcs[1]);

            g_Cutscene_UpdateDaria = true;

            SysWork_StateStepIncrement(0);
            break;

        case 56:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 51, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[1], 28, false);

            g_SysWork.lightIntensity = Q12(0.9f);

            Fs_QueueStartRead(FILE_ANIM_LAST4_DMS, FS_BUFFER_20);

            D_800F4816 = 0;

            SysWork_StateStepIncrement(0);

        case 57:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(540.0f), Q12(586.0f), true, true);
            break;

        case 58:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(587.0f), Q12(615.0f), false, true);
            g_SysWork.lightIntensity = Q12(0.8f);
            break;

        case 59:
            func_800E1788(10);
            func_800DAD54();
            func_800DB288();
            SysWork_StateStepIncrement(0);

        case 60:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(587.0f), Q12(629.0f), false, true);
            break;

        case 61:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[1], 24, false);
            SysWork_StateStepIncrement(0);

        case 62:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(587.0f), Q12(730.0f), true, true);
            break;

        case 63:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        default:
            g_SysWork.sysStateSteps[0] = 0;
            D_800F4805++;

            func_800E9490(&g_SysWork.npcs[3]);
            func_800E9490(&g_SysWork.npcs[1]);

            g_Cutscene_UpdateKau   = false;
            g_Cutscene_UpdateDaria = false;

            WorldGfx_CharaLmBufferAssign(6);
            break;
    }

    if (g_SysWork.sysStateSteps[0] >= 62 && g_SysWork.sysStateSteps[0] < 64)
    {
        func_800DB154(&g_NpcBoneCoords[HarryBone_Root]);
    }
}

const s_800ED7E0_ptr D_800CC320[8] = {
    { { Q12(0.0f),   Q12(0.0f),   Q12(0.0f)    }, Q12(0.0f), Q12(0.0f),  Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), 0, 99, Q12(0.0f), 0, Q12(0.0f),  2 },
    { { Q12(0.0f),   Q12(0.0f),   Q12(0.0f)    }, Q12(0.0f), Q12(0.0f),  Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), 0, 99, Q12(0.0f), 0, Q12(0.0f),  3 },
    { { Q12(134.7f), Q12(0.0f),   Q12(-104.6f) }, Q12(2.0f), Q12(6.0f),  Q12(0.0f), Q12(4.0f), Q12(0.5f), Q12(1.0f), 0, 1,  Q12(0.0f), 0, Q12(0.01f), 2 },
    { { Q12(144.7f), Q12(-10.0f), Q12(-104.6f) }, Q12(2.0f), Q12(10.0f), Q12(0.0f), Q12(0.0f), Q12(0.5f), Q12(1.0f), 0, 0,  Q12(0.0f), 0, Q12(0.01f), 0 },
    { { Q12(0.0f),   Q12(0.0f),   Q12(0.0f)    }, Q12(0.0f), Q12(0.0f),  Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), 0, 99, Q12(0.0f), 0, Q12(0.0f),  3 },
    { { Q12(144.7f), Q12(-10.0f), Q12(-104.6f) }, Q12(2.0f), Q12(10.0f), Q12(0.0f), Q12(2.0f), Q12(0.5f), Q12(1.0f), 0, 0,  Q12(0.0f), 0, Q12(0.01f), 3 },
    { { Q12(144.7f), Q12(0.0f),   Q12(-104.6f) }, Q12(2.0f), Q12(6.0f),  Q12(0.0f), Q12(4.0f), Q12(0.5f), Q12(1.0f), 0, 1,  Q12(0.0f), 0, Q12(0.01f), 2 },
    { { Q12(139.7f), Q12(-10.0f), Q12(-99.6f)  }, Q12(2.0f), Q12(10.0f), Q12(0.0f), Q12(0.0f), Q12(0.5f), Q12(1.0f), 0, 0,  Q12(0.0f), 0, Q12(0.01f), 0 },
};

void func_800E62CC(void) // 0x800E62CC
{
    VECTOR3         unused;
    s32             flags;
    s_800ED7E0_ptr* ptr;

    if (g_SysWork.sysStateSteps[0] >= 12)
    {
        func_80089500();
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Dms_HeaderFixOffsets(FS_BUFFER_20);

            D_800F4806 = 0;
            g_Cutscene_Timer = Q12(0.0f);
            g_Cutscene_MapMsgAudioIdx = 0;

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 182, false);
            func_800E9260(Chara_BloodyIncubator, 5);

            D_800F4820 = 0;

            Rng_SetSeed(0);

            g_SysWork.lightIntensity = Q12(1.0f);

            SysWork_StateStepIncrement(0);

        case 1:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(0.0f), Q12(24.0f), true, true);

            if (g_SysWork.sysStateSteps[0] != 1)
            {
                func_800E941C();
            }
            break;

        case 2:
            Sd_XaPreLoadAudioPreTaskAdd(Sfx_XaAudio596);
            Event_WaitTimer(Q12(0.3f), false);
            break;

        case 3:
            SD_Call(Sfx_XaAudio596);
            func_800DD9F8(&g_SysWork.npcs[2]);
            SysWork_StateStepIncrement(0);

        case 4:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(25.0f), Q12(175.0f), true, true);
            break;

        case 5:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 183, false);
            WorldGfx_CharaModelTransparentSet(Chara_Incubus, true);
            func_800E1788(11);
            SysWork_StateStepIncrement(0);

        case 6:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(176.0f), Q12(200.0f), false, true);
            break;

        case 7:
            func_800E9444(Chara_BloodyIncubator, &g_SysWork.npcs[5]);
            Model_AnimFlagsClear(&g_SysWork.npcs[5].model, 2);
            func_800E1788(12);
            SysWork_StateStepIncrement(0);

        case 8:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(176.0f), Q12(253.0f), true, true);

            g_SysWork.npcs[2].timer_C6 += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.15f);
            if (g_SysWork.npcs[2].timer_C6 > Q12(1.0f))
            {
                g_SysWork.npcs[2].timer_C6 = Q12(1.0f);
            }
            break;

        case 9:
            g_Cutscene_UpdateBar = true;
            Model_AnimFlagsSet(&g_SysWork.npcs[5].model, 2);

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[5], 6, false);
            WorldGfx_CharaModelTransparentSet(Chara_BloodyIncubator, true);

            g_SysWork.npcs[5].timer_C6 = Q12(0.751f); // TODO: Odd value, possibly bad Q format.

            func_800E9490(&g_SysWork.npcs[2]);
            func_800E1788(13);

            g_Cutscene_UpdateBos = false;

            func_800D7E50(&g_SysWork.npcs[2].position);
            func_800D947C();

            Savegame_EventFlagSet(EventFlag_592);
            SysWork_StateStepIncrement(0);

        case 10:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(254.0f), Q12(314.0f), true, true);

            g_SysWork.npcs[5].timer_C6 -= Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.25f);
            if (g_SysWork.npcs[5].timer_C6 < 0)
            {
                g_SysWork.npcs[5].timer_C6 = 0;
            }
            break;

        case 11:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 174, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[5], 7, false);
            WorldGfx_CharaModelTransparentSet(Chara_BloodyIncubator, false);

            SysWork_StateStepIncrement(0);

            D_800F4820 = 1;
            D_800F4824 = 0;
            Savegame_EventFlagSet(EventFlag_593);

        case 12:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(315.0f), Q12(420.0f), true, true);
            break;

        case 13:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[5], 2, false);
            WorldGfx_PlayerHeldItemSet(InvItemId_CutsceneBaby);
            func_800E1788(14);
            SysWork_StateStepIncrement(0);

        case 14:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(421.0f), Q12(475.0f), true, true);
            break;

        case 15:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(476.0f), Q12(536.0f), true, false);
            SysWork_StateStepIncrement(0);
            break;

        case 16:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(476.0f), Q12(536.0f), true, true);
            break;

        case 17:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[5], 3, false);

            g_Cutscene_UpdateBaby = true;

            Sfx_WithFlagsPlay(Sfx_Unk1689, NULL, CHARA_FORCE_FREE_ALL, SfxFlag_Mono | SfxFlag_NoDistAtten);
            func_800E1788(15);
            SysWork_StateStepIncrement(0);

        case 18:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(537.0f), Q12(600.0f), true, true);
            break;

        case 19:
            Fs_QueueWaitForEmpty();
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 179, false);
            WorldGfx_CharaModelTransparentSet(Chara_BloodyIncubator, true);
            WorldGfx_CharaModelMaterialSet(Chara_BloodyIncubator, 0);

            g_Cutscene_UpdateBaby = false;

            func_8003D01C();
            SysWork_StateStepIncrement(0);

        case 20:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(601.0f), Q12(619.0f), false, true);
            break;

        case 21:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[5], 4, false);
            SysWork_StateStepIncrement(0);

        case 22:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(601.0f), Q12(635.0f), true, true);
            break;

        case 23:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(636.0f), Q12(653.0f), true, false);
            SysWork_StateStepIncrement(0);
            break;

        case 24:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(636.0f), Q12(653.0f), true, true);
            break;

        case 25:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 180, false);
            SysWork_StateStepIncrement(0);

        case 26:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(654.0f), Q12(721.0f), true, true);
            break;

        case 27:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[5], 8, false);

            if (Savegame_EventFlagGet(EventFlag_449))
            {
                func_800E9260(Chara_EndingCybil, 4);
            }
            else
            {
                func_800E9260(Chara_EndingKaufmann, 4);
            }

            Sd_SfxStop(Sfx_Unk1689);
            Sfx_WithFlagsPlay(Sfx_Unk1688, NULL, Q8_CLAMPED(1.0f), SfxFlag_Mono | SfxFlag_NoDistAtten);
            func_800D88C4();
            SysWork_StateStepIncrement(0);

        case 28:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(722.0f), Q12(752.0f), true, true);
            break;

        case 29:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 180, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[5], 9, false);
            Player_FlexRotationYReset();
            Savegame_EventFlagSet(EventFlag_593);
            SysWork_StateStepIncrement(0);

        case 30:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(753.0f), Q12(843.0f), true, true);
            break;

        case 31:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 181, false);
            SysWork_StateStepIncrement(0);

        case 32:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(844.0f), Q12(935.0f), true, true);
            break;

        case 33:
            if (Savegame_EventFlagGet(EventFlag_449))
            {
                func_800E941C();
                func_800E9444(Chara_EndingCybil, g_SysWork.npcs);

                g_Cutscene_UpdateSibyl = true;
                Model_AnimFlagsClear(&g_SysWork.npcs[0].model, 2);

                SysWork_StateStepIncrement(0);
            }
            else
            {
#if VERSION_EQUAL_OR_NEWER(JAP1) // @bugfix?
                func_800E941C();
#endif
                SysWork_StateStepSet(0, 38);
            }
            break;

        case 34:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, g_SysWork.npcs, 26, false);
            Model_AnimFlagsSet(&g_SysWork.npcs[0].model, 2);
            func_800E9490(&g_SysWork.npcs[5]);
            func_800E9260(Chara_EndingKaufmann, 5);
            Sd_XaPreLoadAudioPreTaskAdd(Sfx_XaAudio657);

            g_Cutscene_UpdateBar = 0;

            SysWork_StateStepIncrement(0);

        case 35:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(936.0f), Q12(975.0f), true, true);
            break;

        case 36:
#if VERSION_EQUAL_OR_NEWER(JAP1) // @bugfix?
            func_800E941C();
#endif
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, g_SysWork.npcs, 25, false);
            SD_Call(Sfx_XaAudio657);
            func_800D7ED0();
            SysWork_StateStepIncrement(0);

        case 37:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(976.0f), Q12(1058.0f), true, true);

            if (g_SysWork.sysStateSteps[0] != 37)
            {
                SD_Call(19);
            }
            break;

        case 38:
#if !VERSION_EQUAL_OR_NEWER(JAP1) // @bugfix?
            func_800E941C();
#endif
            func_800E9444(Chara_EndingKaufmann, &g_SysWork.npcs[3]);

            if (Savegame_EventFlagGet(EventFlag_449))
            {
                Fs_QueueStartRead(FILE_ANIM_ENDAA_DMS, FS_BUFFER_18);
            }
            else
            {
                Fs_QueueStartRead(FILE_ANIM_ENDBA_DMS, FS_BUFFER_18);
            }

            SysWork_StateStepIncrement(0);
            break;

        case 39:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[3], 21, false);
            g_Cutscene_UpdateKau = true;

            SysWork_StateStepIncrement(0);

        case 40:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(1059.0f), Q12(1122.0f), true, true);
            break;

        case 41:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, 0, false);
            break;

        default:
            SysWork_StateSetNext(SysState_Gameplay);
            func_800E9490(&g_SysWork.npcs[3]);

            g_Cutscene_UpdateKau = false;

            Sd_SfxStop(Sfx_Unk1688);

            D_800F4805++;
            break;
    }

    if (D_800F4820 != Q12(0.0f))
    {
        switch (D_800F4824)
        {
            case 0:
                if (g_Cutscene_Timer > Q12(700.0f))
                {
                    D_800F4824 = 1;
                }
                break;

            case 1:
                if (!(Rng_Rand16() & 0x70))
                {
                    func_800D7D74(0);
                }

                if (g_SysWork.sysStateSteps[0] >= 34)
                {
                    func_800D71A4(32);
                    D_800F4824++;
                }
                break;

            case 2:
                if (!(Rng_Rand16() & 0xF8))
                {
                    func_800D7D74(0);
                }
                break;
        }

        if (g_SysWork.sysStateSteps[0] >= 30 && g_SysWork.sysStateSteps[0] < 40)
        {
            if (g_SysWork.sysStateSteps[0] < 33)
            {
                D_800F4830 = 1;
            }
            else
            {
                D_800F4830 = 2;
                func_800D8858(1);
            }
        }
        else
        {
            D_800F4830 = 0;
        }

        ptr = D_800ED7E0[g_SysWork.sysStateSteps[0]];
        if (ptr)
        {
            do
            {
                // @unused
                unused.vx = ptr->field_0.vx >> 4;
                unused.vy = ptr->field_0.vy >> 4;
                unused.vz = ptr->field_0.vz >> 4;

                if (!(ptr->flags_38 & 2) || g_SysWork.sysStateSteps[0] != D_800EDA0C)
                {
                    func_800DA774(ptr);
                }

                flags = ptr->flags_38;
                ptr++;
            }
            while (flags & (1 << 0));
        }

        D_800EDA0C = g_SysWork.sysStateSteps[0];
    }
}

void func_800E70F0(void) // 0x800E70F0
{
    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Fs_QueueWaitForEmpty();
            Dms_HeaderFixOffsets((s_DmsHeader*)FS_BUFFER_18);

            D_800F4806 = 1;
            g_Cutscene_Timer = Q12(0.0f);
            g_Cutscene_MapMsgAudioIdx = 0;

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 51, false);
            func_800E1788(4);

            Model_AnimFlagsSet(&g_SysWork.npcs[1].model, 2);
            g_SysWork.lightIntensity = Q12(0.8f);
            SysWork_StateStepIncrement(0);

        case 1:
            Event_DisplayMapMsgWithAudio(74, &g_Cutscene_MapMsgAudioIdx, &D_800ED88C);
            break;

        case 2:
            Event_DisplayMapMsgWithAudio(76, &g_Cutscene_MapMsgAudioIdx, &D_800ED88C);
            g_Cutscene_Timer = Q12(1.0f);
            break;

        case 3:
            Event_DisplayMapMsgWithAudio(78, &g_Cutscene_MapMsgAudioIdx, &D_800ED88C);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(2.0f), Q12(33.0f), true, false);
            break;

        case 4:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[1], 17, false);
            Event_DisplayMapMsgWithAudio(80, &g_Cutscene_MapMsgAudioIdx, &D_800ED88C);
            func_800E1788(5);
            func_800DAD54();
            func_800DB288();
            SysWork_StateStepIncrement(0);

        case 5:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(2.0f), Q12(41.0f), true, true);
            break;

        case 6:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(42.0f), Q12(68.0f), true, true);
            break;

        case 7:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        default:
            g_SysWork.sysStateSteps[0] = 0;
            D_800F4805++;

            func_800E9490(&g_SysWork.npcs[1]);

            g_Cutscene_UpdateDaria = false;
            break;
    }

    if (g_SysWork.sysStateSteps[0] >= 5 && g_SysWork.sysStateSteps[0] < 8)
    {
        func_800DB154(&g_NpcBoneCoords[HarryBone_Root]);
    }
}

const s_800ED7E0_ptr D_800CC5C8[1] = {
    { { Q12(139.7f), Q12(-8.0f), Q12(-104.6f) }, Q12(2.0f), Q12(10.0f), Q12(0.0f), Q12(0.0f), Q12(0.5f), Q12(1.0f), 0, 0, Q12(0.0f), 0, Q12(0.01f), 0 },
};

void func_800E7380(void) // 0x800E7380
{
    s_800ED7E0_ptr* ptr;
    s32             flags;

    if (g_SysWork.sysStateSteps[0] >= 6 && g_SysWork.sysStateSteps[0] < 10)
    {
        func_80089500();
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            D_800F4806 = 1;
            func_800E94AC();

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 51, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[4], 3, false);

            g_Cutscene_MapMsgAudioIdx = 0;
            g_Cutscene_Timer = Q12(69.0f);

            SD_Call(Sfx_XaAudio602);

            g_SysWork.lightIntensity = Q12(0.8f);
            func_800E1788(7);
            SysWork_StateStepIncrement(0);

        case 1:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(6.8f), Q12(69.0f), Q12(143.0f), true, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_AwaitAnimEnd, &g_SysWork.npcs[4], 0, false);
            break;

        case 2:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(6.8f), Q12(69.0f), Q12(143.0f), true, true);

            if (g_SysWork.sysStateSteps[0] != 2)
            {
                SD_Call(19);
            }
            break;

        case 3:
            g_Cutscene_Timer = Q12(144.0f);
            Event_DisplayMapMsgWithAudio(82, &g_Cutscene_MapMsgAudioIdx, &D_800ED898);
            break;

        case 4:
            Event_DisplayMapMsgWithAudio(84, &g_Cutscene_MapMsgAudioIdx, &D_800ED898);
            break;

        case 5:
            func_800E9260(Chara_EndingCybil, 5);
            func_800D7E50(&g_SysWork.playerWork.player.position);
            func_800D947C();

            D_800F4824 = 0;
            D_800F4820 = 1;

            func_800E1788(8);
            SysWork_StateStepIncrement(0);

        case 6:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(12.0f), Q12(145.0f), Q12(201.0f), true, true);

            g_SysWork.npcs[4].timer_C6 += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.25f);
            if (g_SysWork.npcs[4].timer_C6 > Q12(1.0f))
            {
                g_SysWork.npcs[4].timer_C6 = Q12(1.0f);
            }
            break;

        case 7:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 162, false);
            g_SysWork.lightIntensity = Q12(0.7f);
            func_800E9490(&g_SysWork.npcs[4]);
            g_Cutscene_UpdateMar = false;
            SysWork_StateStepIncrement(0);

        case 8:
            Event_DisplayMapMsgWithAudio(86, &g_Cutscene_MapMsgAudioIdx, &D_800ED898);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(6.5f), Q12(202.0f), Q12(301.0f), true, false);
            break;

        case 9:
            Event_DisplayMapMsgWithAudio(87, &g_Cutscene_MapMsgAudioIdx, &D_800ED898);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(6.5f), Q12(202.0f), Q12(301.0f), true, false);
            break;

        case 10:
            Event_DisplayMapMsgWithAudio(90, &g_Cutscene_MapMsgAudioIdx, &D_800ED898);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(6.5f), Q12(202.0f), Q12(301.0f), true, false);
            break;

        case 11:
            if (Savegame_EventFlagGet(EventFlag_449))
            {
                Fs_QueueStartRead(FILE_ANIM_ENDC_DMS, FS_BUFFER_20);
            }

            SysWork_StateStepIncrement(0);

        case 12:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(6.5f), Q12(202.0f), Q12(301.0f), true, false);
            break;

        default:
            g_SysWork.sysStateSteps[0] = 0;
            D_800F4805++;
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            break;
    }

    if (D_800F4820 != 0)
    {
        func_800D7EF0();

        if (D_800F4820)
        {
            ptr = D_800ED8B0[g_SysWork.sysStateSteps[0]];
            if (ptr != NULL)
            {
                do
                {
                    if (!(ptr->flags_38 & 2) || g_SysWork.sysStateSteps[0] != D_800ED8AC)
                    {
                        func_800DA774(ptr);
                    }

                    flags = ptr->flags_38;
                    ptr++;
                }
                while (flags & (1 << 0));
            }

            D_800ED8AC = g_SysWork.sysStateSteps[0];
        }
    }
}

const s_800ED7E0_ptr D_800CC63C[27] = {
    { { Q12(139.7f), Q12(-10.0f), Q12(-99.6f)  }, Q12(0.0f), Q12(8.0f),  Q12(0.0f), Q12(0.0f), Q12(0.5f), Q12(1.5f), 0, 0,  Q12(0.0f),  0, Q12(0.1f),  1 },
    { { Q12(139.7f), Q12(-10.0f), Q12(-99.6f)  }, Q12(0.0f), Q12(3.0f),  Q12(1.0f), Q12(1.5f), Q12(0.5f), Q12(1.5f), 0, 0,  Q12(0.0f),  0, Q12(0.3f),  2 },
    { { Q12(0.0f),   Q12(0.0f),   Q12(0.0f)    }, Q12(0.0f), Q12(0.0f),  Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), 0, 99, Q12(0.0f),  0, Q12(0.0f),  3 },
    { { Q12(139.7f), Q12(0.0f),   Q12(-99.6f)  }, Q12(1.0f), Q12(8.0f),  Q12(0.0f), Q12(4.0f), Q12(0.5f), Q12(1.0f), 0, 1,  Q12(0.0f),  0, Q12(0.03f), 3 },
    { { Q12(139.7f), Q12(-10.0f), Q12(-99.6f)  }, Q12(1.0f), Q12(8.0f),  Q12(0.0f), Q12(2.0f), Q12(0.5f), Q12(1.0f), 0, 0,  Q12(0.0f),  0, Q12(0.03f), 3 },
    { { Q12(139.7f), Q12(-10.0f), Q12(-99.6f)  }, Q12(0.0f), Q12(8.0f),  Q12(0.0f), Q12(0.0f), Q12(0.5f), Q12(1.0f), 0, 0,  Q12(0.0f),  0, Q12(0.03f), 1 },
    { { Q12(139.7f), Q12(-10.0f), Q12(-99.6f)  }, Q12(0.0f), Q12(3.0f),  Q12(1.6f), Q12(2.0f), Q12(0.5f), Q12(1.5f), 0, 0,  Q12(0.0f),  0, Q12(0.3f),  2 },
    { { Q12(0.0f),   Q12(0.0f),   Q12(0.0f)    }, Q12(0.0f), Q12(0.0f),  Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), 0, 99, Q12(0.0f),  0, Q12(0.0f),  3 },
    { { Q12(100.3f), Q12(-10.0f), Q12(-139.6f) }, Q12(2.0f), Q12(10.0f), Q12(0.0f), Q12(2.0f), Q12(0.5f), Q12(1.0f), 0, 0,  Q12(-2.0f), 1, Q12(0.03f), 3 },
    { { Q12(100.3f), Q12(0.0f),   Q12(-139.6f) }, Q12(2.0f), Q12(10.0f), Q12(0.0f), Q12(4.0f), Q12(0.5f), Q12(1.0f), 0, 1,  Q12(-2.0f), 1, Q12(0.03f), 3 },
    { { Q12(100.3f), Q12(-10.0f), Q12(-137.6f) }, Q12(2.0f), Q12(10.0f), Q12(0.0f), Q12(2.0f), Q12(0.5f), Q12(1.0f), 0, 0,  Q12(-2.0f), 1, Q12(0.03f), 1 },
    { { Q12(100.3f), Q12(0.0f),   Q12(-137.6f) }, Q12(2.0f), Q12(10.0f), Q12(0.0f), Q12(4.0f), Q12(0.5f), Q12(1.0f), 0, 1,  Q12(-2.0f), 1, Q12(0.03f), 0 },
    { { Q12(0.0f),   Q12(0.0f),   Q12(0.0f)    }, Q12(0.0f), Q12(0.0f),  Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), 0, 99, Q12(0.0f),  0, Q12(0.0f),  3 },
    { { Q12(139.7f), Q12(0.0f),   Q12(-98.6f)  }, Q12(0.8f), Q12(8.0f),  Q12(0.0f), Q12(4.0f), Q12(0.5f), Q12(1.0f), 0, 1,  Q12(0.0f),  0, Q12(0.03f), 3 },
    { { Q12(139.7f), Q12(-10.0f), Q12(-99.6f)  }, Q12(0.5f), Q12(8.0f),  Q12(0.0f), Q12(2.0f), Q12(0.5f), Q12(1.0f), 0, 0,  Q12(0.0f),  0, Q12(0.03f), 3 },
    { { Q12(139.7f), Q12(-10.0f), Q12(-99.6f)  }, Q12(0.0f), Q12(3.0f),  Q12(1.6f), Q12(1.9f), Q12(0.5f), Q12(1.5f), 0, 0,  Q12(0.0f),  0, Q12(0.3f),  2 },
    { { Q12(0.0f),   Q12(0.0f),   Q12(0.0f)    }, Q12(0.0f), Q12(0.0f),  Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), 0, 99, Q12(0.0f),  0, Q12(0.0f),  3 },
    { { Q12(100.3f), Q12(0.0f),   Q12(-139.6f) }, Q12(2.0f), Q12(10.0f), Q12(0.0f), Q12(4.0f), Q12(0.5f), Q12(1.0f), 0, 1,  Q12(0.0f),  0, Q12(0.01f), 3 },
    { { Q12(100.3f), Q12(-10.0f), Q12(-139.6f) }, Q12(0.5f), Q12(8.0f),  Q12(0.0f), Q12(1.0f), Q12(0.5f), Q12(2.0f), 0, 0,  Q12(0.0f),  0, Q12(2.0f),  3 },
    { { Q12(100.3f), Q12(-10.0f), Q12(-139.6f) }, Q12(0.0f), Q12(3.0f),  Q12(0.0f), Q12(1.6f), Q12(0.5f), Q12(2.0f), 0, 0,  Q12(0.0f),  0, Q12(0.3f),  2 },
    { { Q12(100.3f), Q12(-10.0f), Q12(-139.6f) }, Q12(2.0f), Q12(10.0f), Q12(0.0f), Q12(0.0f), Q12(0.5f), Q12(1.0f), 0, 0,  Q12(0.0f),  0, Q12(0.1f),  0 },
    { { Q12(0.0f),   Q12(0.0f),   Q12(0.0f)    }, Q12(0.0f), Q12(0.0f),  Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), 0, 99, Q12(0.0f),  0, Q12(0.0f),  3 },
    { { Q12(100.3f), Q12(0.0f),   Q12(-139.6f) }, Q12(2.0f), Q12(10.0f), Q12(0.0f), Q12(4.0f), Q12(0.5f), Q12(1.0f), 0, 1,  Q12(0.0f),  0, Q12(0.03f), 3 },
    { { Q12(100.3f), Q12(-10.0f), Q12(-139.6f) }, Q12(2.0f), Q12(10.0f), Q12(0.0f), Q12(2.0f), Q12(0.5f), Q12(1.0f), 0, 0,  Q12(0.0f),  0, Q12(0.03f), 3 },
    { { Q12(100.3f), Q12(-10.0f), Q12(-139.6f) }, Q12(2.0f), Q12(10.0f), Q12(0.0f), Q12(0.0f), Q12(0.5f), Q12(1.0f), 0, 0,  Q12(0.0f),  0, Q12(0.1f),  0 },
    { { Q12(100.3f), Q12(-10.0f), Q12(-139.6f) }, Q12(2.0f), Q12(10.0f), Q12(0.0f), Q12(0.0f), Q12(0.5f), Q12(1.0f), 0, 0,  Q12(-2.0f), 0, Q12(0.03f), 0 },
    { { Q12(100.3f), Q12(0.0f),   Q12(-139.6f) }, Q12(2.0f), Q12(10.0f), Q12(2.0f), Q12(4.0f), Q12(0.5f), Q12(1.0f), 0, 1,  Q12(-2.0f), 0, Q12(0.03f), 2 },
};

void func_800E787C(void) // 0x800E787C
{
    s_800ED7E0_ptr* ptr;
    s32             flags;

    if (g_SysWork.sysStateSteps[0] == 31)
    {
        Particle_EnvironmentSet(2, 127);
    }

    if (g_SysWork.sysStateSteps[0] >= 31)
    {
        Particle_SystemUpdate(0, g_SavegamePtr->mapIdx, g_SysWork.sysStateSteps[0] != 31);
    }

    if (g_SysWork.sysStateSteps[0] >= 1 && g_SysWork.sysStateSteps[0] < 14)
    {
        func_80089500();
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Bgm_CrossfadeToTrack(BgmCmd_Track35);
            break;

        case 1:
            Fs_QueueWaitForEmpty();
            Dms_HeaderFixOffsets(FS_BUFFER_18);

            D_800F4806 = 1;
            g_Cutscene_Timer = Q12(0.0f);

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 176, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 2, false);

            Fs_QueueStartRead(FILE_ANIM_ENDAB_DMS, FS_BUFFER_20);
            func_800E9260(Chara_BloodyIncubator, 5);

            D_800F4819 = 1;
            D_800F481A = true;

            Model_AnimFlagsClear(&g_SysWork.npcs[5].model, 2);
            func_800D7E50(&g_SysWork.playerWork.player.position);

            func_800D947C();
            D_800F4824 = 0;
            D_800F4820 = 1;
            D_800F4834 = 0;
            D_800F4838 = 1;

            Sfx_WithFlagsPlay(Sfx_Unk1688, NULL, Q8_CLAMPED(1.0f), SfxFlag_Mono | SfxFlag_NoDistAtten);

            SysWork_StateStepIncrement(0);

        case 2:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.0f), false);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(0.0f), Q12(64.0f), true, false);
            break;

        case 3:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(0.0f), Q12(64.0f), false, true);
            break;

        case 4:
            Sfx_WithFlagsPlay(Sfx_Unk1694, NULL, Q8_CLAMPED(1.0f), SfxFlag_Mono | SfxFlag_NoDistAtten);
            Sfx_WithFlagsPlay(Sfx_Unk1695, NULL, Q8_CLAMPED(1.0f), SfxFlag_Mono | SfxFlag_NoDistAtten);
            SysWork_StateStepIncrement(0);

        case 5:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(0.0f), Q12(68.0f), true, true);
            break;

        case 6:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 177, false);
            Sfx_WithFlagsPlay(Sfx_Unk1694, NULL, Q8_CLAMPED(1.0f), SfxFlag_Mono | SfxFlag_NoDistAtten);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 27, false);
            SD_Call(Sfx_XaAudio612);
            D_800F4819 = 2;
            SysWork_StateStepIncrement(0);

        case 7:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(69.0f), Q12(89.0f), true, true);
            break;

        case 8:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 184, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 29, false);
            SysWork_StateStepIncrement(0);

        case 9:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(90.0f), Q12(98.0f), true, true);
            break;

        case 10:
            GameFs_StfRollBinLoad();
            SysWork_StateStepIncrement(0);

        case 11:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 1, Q12(0.5f), false);
            break;

        case 12:
            func_800D88D8();
            func_800D7CF8();
            D_800F4834 = 1;
            Sd_SfxStop(Sfx_Unk1688);
            Sfx_WithFlagsPlay(Sfx_Unk1689, NULL, Q8_CLAMPED(1.0f), SfxFlag_Mono | SfxFlag_NoDistAtten);
            SysWork_StateStepIncrement(0);

        case 13:
            Savegame_EventFlagSet(EventFlag_575);

            if (Sd_AudioStreamingCheck() == AudioStreamingState_None)
            {
                func_800E941C();
                func_800E9444(Chara_BloodyIncubator, &g_SysWork.npcs[5]);
                Model_AnimFlagsClear(&g_SysWork.npcs[5].model, 2);
                func_801E2E28(g_EndingIdx);
                SysWork_StateStepIncrement(0);
            }
            else
            {
                break;
            }

        case 14:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 178, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 1, Q12(0.5f), false);
            SysWork_StateStepIncrement(0);

        case 15:
            if (func_801E2ED8())
            {
                SysWork_StateStepIncrement(0);
            }
            break;

        case 16:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(99.0f), Q12(154.0f), true, true);
            break;

        case 17:
            D_800F4806 = 0;
            g_Cutscene_Timer = Q12(0.0f);

            Dms_HeaderFixOffsets(FS_BUFFER_20);

            g_Cutscene_UpdateBar   = true;
            g_Cutscene_UpdateSibyl = false;
            g_Cutscene_UpdateHero  = false;

            Model_AnimFlagsSet(&g_SysWork.npcs[5].model, 2);

#if VERSION_EQUAL_OR_NEWER(USA) // @bugfix?
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 51, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 5, false);
#endif

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[5], 9, false);
            WorldGfx_CharaModelTransparentSet(Chara_BloodyIncubator, true);

            g_SysWork.playerWork.player.position.vx = Q12(140.0f);
            g_SysWork.playerWork.player.position.vz = Q12(-100.0f);

            D_800F4819 = 0;
            D_800F481A = false;
            D_800F4838 = 0;
            D_800F4824 = 0;

            Model_AnimFlagsClear(&g_SysWork.playerWork.player.model, 2);
            func_800D7E50(&g_SysWork.playerWork.player.position);

            SysWork_StateStepIncrement(0);
            Rng_SetSeed(0);

        case 18:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(0.0f), Q12(60.0f), true, true);
            break;

        case 19:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 20:
            D_800F4806 = 1;
            g_Cutscene_UpdateBar   = false;
            g_Cutscene_UpdateSibyl = true;
            g_Cutscene_UpdateHero  = true;
            g_Cutscene_Timer        = Q12(155.0f);

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 176, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 2, false);

            D_800F4819 = 1;

            Model_AnimFlagsSet(&g_SysWork.playerWork.player.model, 2);

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            WorldEnv_TransitionStart(17, 17, PrimitiveType_S32, &D_800F483C, 0, Q12(100.0f));

            D_800F483C = Q12(40.0f);
            D_800F4838 = 1;
            D_800F4824 = 0;

            func_800D7E50(&g_SysWork.playerWork.player.position);
            SysWork_StateStepIncrement(0);

        case 21:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(155.0f), Q12(224.0f), true, true);
            D_800F483C += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 3.0f);
            break;

        case 22:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            D_800F483C += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 3.0f);
            break;

        case 23:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[5], 9, false);

#if VERSION_EQUAL_OR_NEWER(USA) // @bugfix?
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 51, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 5, false);
#endif

            D_800F483C = Q12(100.0f);

            WorldEnv_PresetsSet(9, 9);

            D_800F4806           = false;
            g_Cutscene_Timer      = Q12(61.0f);
            g_Cutscene_UpdateBar = true;

            g_SysWork.playerWork.player.position.vx = Q12(140.0f);
            g_SysWork.playerWork.player.position.vz = Q12(-100.0f);

            g_Cutscene_UpdateSibyl = false;
            g_Cutscene_UpdateHero  = false;
            D_800F4819 = 0;

            Model_AnimFlagsClear(&g_SysWork.playerWork.player.model, 2);

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            func_800D7E50(&g_SysWork.playerWork.player.position);
            SysWork_StateStepIncrement(0);

        case 24:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(61.0f), Q12(77.0f), false, true);
            break;

        case 25:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[5], 10, false);
            SysWork_StateStepIncrement(0);

        case 26:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(61.0f), Q12(84.0f), false, true);
            break;

        case 27:
            D_800F4824 = 0;

            func_800D88CC();
            func_800D7CEC();

            D_800F4838 = 0;
            D_800F4834 = 0;

            Sd_SfxStop(Sfx_Unk1689);
            Sfx_WithFlagsPlay(Sfx_Unk1690, NULL, Q8_CLAMPED(1.0f), SfxFlag_Mono | SfxFlag_NoDistAtten);
            SysWork_StateStepIncrement(0);

        case 28:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(61.0f), Q12(95.0f), true, true);
            break;

        case 29:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(96.0f), Q12(121.0f), true, true);
            break;

        case 30:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 31:
            D_800F4806 = 1;
            D_800F4820 = 0;

            func_800D7E50(&g_SysWork.playerWork.player.position);
            Sd_SfxStop(Sfx_Unk1690);

            g_Cutscene_UpdateBar   = false;
            g_Cutscene_UpdateSibyl = true;
            g_Cutscene_UpdateHero  = true;
            g_Cutscene_Timer        = Q12(225.0f);

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 176, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 2, false);

            D_800F4819 = 0;
            D_800F4818 = true;

            Model_AnimFlagsSet(&g_SysWork.playerWork.player.model, 2);
            WorldEnv_PresetsSet(9, 9);
            WorldEnv_TransitionStart(17, 17, PrimitiveType_S32, &D_800F483C, 0, Q12(100.0f));
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            D_800F483C = Q12(80.0f);
            SysWork_StateStepIncrement(0);

        case 32:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(225.0f), Q12(259.0f), true, true);
            D_800F483C += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 10.0f);
            if (D_800F483C > Q12(100.0f))
            {
                D_800F483C = Q12(100.0f);
            }
            break;

        case 33:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(260.0f), Q12(325.0f), false, true);

            D_800F483C += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 10.0f);
            if (D_800F483C > Q12(100.0f))
            {
                D_800F483C = Q12(100.0f);
            }
            break;

        case 34:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            SysWork_StateStepIncrement(0);

        case 35:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 1, Q12(0.4f), false);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(260.0f), Q12(340.0f), true, false);
            break;

        default:
            D_800F4818 = false;
            D_800F4820 = 0;

            g_SysWork.sysStateSteps[0] = 0;

            D_800F483C = Q12(100.0f);
            D_800F4805++;

            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            func_800E9490(&g_SysWork.npcs[0]);
            break;
    }

    if (D_800F4820 != 0)
    {
        switch (D_800F4824)
        {
            case 0:
                D_800F4824 = 1;

            case 1:
                if (!D_800F4834 && !(Rng_Rand16() & 0x70))
                {
                    func_800D7D74(0);
                }
                break;
        }

        if (D_800F4820)
        {
            ptr = D_800ED8EC[g_SysWork.sysStateSteps[0]];
            if (ptr)
            {
                do
                {
                    if (!(ptr->flags_38 & 2) || g_SysWork.sysStateSteps[0] != D_800ED8E8)
                    {
                        func_800DA774(ptr);
                    }

                    flags = ptr->flags_38;
                    ptr++;
                }
                while (flags & (1 << 0));
            }

            D_800ED8E8 = g_SysWork.sysStateSteps[0];
        }
    }

    if (g_SysWork.sysStateSteps[0] >= 1 && g_SysWork.sysStateSteps[0] < 7)
    {
        D_800F4830 = 3;
    }
    else
    {
        D_800F4830 = 0;
    }
}

const s_800ED7E0_ptr D_800CCD20[5] = {
    { { Q12(139.7f), Q12(-10.0f), Q12(-99.6f) }, Q12(0.0f), Q12(8.0f), Q12(0.0f), Q12(0.0f), Q12(0.5f), Q12(1.5f), 0, 0,  Q12(0.0f), 0, Q12(0.1f),  0 },
    { { Q12(0.0f),   Q12(0.0f),   Q12(0.0f)   }, Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), 0, 99, Q12(0.0f), 0, Q12(0.0f),  3 },
    { { Q12(139.7f), Q12(0.0f),   Q12(-99.6f) }, Q12(1.0f), Q12(8.0f), Q12(0.0f), Q12(4.0f), Q12(0.5f), Q12(1.0f), 0, 1,  Q12(0.0f), 0, Q12(0.03f), 3 },
    { { Q12(139.7f), Q12(-10.0f), Q12(-99.6f) }, Q12(1.0f), Q12(8.0f), Q12(0.0f), Q12(2.0f), Q12(0.5f), Q12(1.0f), 0, 0,  Q12(0.0f), 0, Q12(0.03f), 3 },
    { { Q12(139.7f), Q12(-10.0f), Q12(-99.6f) }, Q12(0.0f), Q12(8.0f), Q12(0.0f), Q12(0.0f), Q12(0.5f), Q12(1.0f), 0, 0,  Q12(0.0f), 0, Q12(0.03f), 0 },
};

void func_800E86BC(void) // 0x800E86BC
{
    s_800ED7E0_ptr* ptr;
    s32             flags;

    if (g_SysWork.sysStateSteps[0] == 8)
    {
        Particle_EnvironmentSet(2, 127);
    }

    if (g_SysWork.sysStateSteps[0] >= 8)
    {
        Particle_SystemUpdate(0, g_SavegamePtr->mapIdx, g_SysWork.sysStateSteps[0] != 8);
    }

    if (g_SysWork.sysStateSteps[0] >= 1 && g_SysWork.sysStateSteps[0] < 6)
    {
        func_80089500();
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Bgm_CrossfadeToTrack(BgmCmd_Track35);
            break;

        case 1:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 176, false);

            Dms_HeaderFixOffsets(FS_BUFFER_18);
            Fs_QueueStartRead(FILE_ANIM_ENDBB_DMS, FS_BUFFER_20);

            D_800F4806 = 1;
            g_Cutscene_UpdateHero = true;
            g_Cutscene_UpdateBar  = false;
            D_800F4819 = 1;
            D_800F481A = true;

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            func_800D7E50(&g_SysWork.playerWork.player.position);
            func_800D947C();
            D_800F4824 = 0;
            D_800F4820 = 1;
            D_800F4838 = 1;

            GameFs_StfRollBinLoad();
            WorldGfx_CharaModelTransparentSet(Chara_BloodyIncubator, true);
            SysWork_StateStepIncrement(0);

        case 2:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(0.0f), Q12(89.0f), true, true);
            break;

        case 3:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(90.0f), Q12(160.0f), true, true);
            break;

        case 4:
            Fs_QueueWaitForEmpty();
            Dms_HeaderFixOffsets(FS_BUFFER_20);

            D_800F4806 = 0;
            g_Cutscene_Timer       = Q12(0.0f);
            g_Cutscene_UpdateHero = false;
            g_Cutscene_UpdateBar  = true;
            D_800F4819 = 0;
            D_800F481A = false;

#if VERSION_EQUAL_OR_NEWER(USA) // @bugfix?
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 51, false);
#endif

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[5], 11, false);

            g_SysWork.playerWork.player.position.vx = Q12(140.0f);
            g_SysWork.playerWork.player.position.vz = Q12(-100.0f);
            Model_AnimFlagsClear(&g_SysWork.playerWork.player.model, 2);

            Savegame_EventFlagSet(EventFlag_575);

            func_801E2E28(g_EndingIdx);
            D_800F4838 = 0;
            SysWork_StateStepIncrement(0);

        case 5:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(0.0f), Q12(60.0f), true, true);
            break;

        case 6:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 7:
            if (func_801E2ED8())
            {
                SysWork_StateStepIncrement(0);
            }
            break;

        case 8:
            D_800F4806 = 1;
            func_800E9490(&g_SysWork.npcs[5]);
            g_Cutscene_Timer = Q12(161.0f);

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 176, false);

            g_Cutscene_UpdateHero = true;
            g_Cutscene_UpdateBar  = false;
            
#if VERSION_EQUAL_OR_NEWER(USA) // @bugfix?
            D_800F4820 = 0;
            D_800F4818 = true;

            Model_AnimFlagsSet(&g_SysWork.playerWork.player.model, 2);

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            D_800F483C = Q12(70.0f);
#else
            Model_AnimFlagsSet(&g_SysWork.playerWork.player.model, 2);
            WorldEnv_PresetsSet(17, 17);

            D_800F4820 = 0;
            D_800F4818 = true;

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
#endif

            WorldEnv_PresetsSet(9, 9);
            WorldEnv_TransitionStart(17, 17, PrimitiveType_S32, &D_800F483C, 0, Q12(100.0f));
            SysWork_StateStepIncrement(0);

        case 9:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(161.0f), Q12(291.0f), true, true);
            D_800F483C += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 10.0f);
            if (D_800F483C > Q12(100.0f))
            {
                D_800F483C = Q12(100.0f);
            }
            break;

        case 10:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            SysWork_StateStepIncrement(0);

        case 11:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 1, Q12(1.0f), false);
            break;

        default:
            D_800F4805++;

            Sd_SfxStop(Sfx_Unk1688);
            Sd_SfxStop(Sfx_Unk1689);
            Sd_SfxStop(Sfx_Unk1690);

            D_800F4818 = false;

            g_SysWork.sysStateSteps[0] = 0;

            D_800F4820 = 0;
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;
    }

    if (D_800F4820 != 0)
    {
        switch (D_800F4824)
        {
            case 0:
                if (!(Rng_Rand16() & 0x70))
                {
                    func_800D7D74(0);
                }
                break;

            case 1:
                if (!(Rng_Rand16() & 0x70))
                {
                    func_800D7D74(0);
                }

            case 2:
                break;
        }

        if (D_800F4820 != 0)
        {
            ptr = D_800ED984[g_SysWork.sysStateSteps[0]];
            if (ptr)
            {
                do
                {
                    if (!(ptr->flags_38 & 2) || g_SysWork.sysStateSteps[0] != D_800ED980)
                    {
                        func_800DA774(ptr);
                    }

                    flags = ptr->flags_38;
                    ptr++;
                }
                while (flags & (1 << 0));
            }

            D_800ED980 = g_SysWork.sysStateSteps[0];
        }
    } 
}

const s_800ED7E0_ptr D_800CCE80[10] = {
    { { Q12(139.7f), Q12(-10.0f), Q12(-104.6f) }, Q12(2.0f), Q12(10.0f), Q12(0.0f), Q12(0.0f), Q12(0.5f), Q12(1.0f), 0, 0,  Q12(0.0f), 0, Q12(0.03f), 0 },
    { { Q12(0.0f),   Q12(0.0f),   Q12(0.0f)    }, Q12(0.0f), Q12(0.0f),  Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), 0, 99, Q12(0.0f), 0, Q12(0.0f),  3 },
    { { Q12(139.7f), Q12(0.0f),   Q12(-104.6f) }, Q12(2.0f), Q12(10.0f), Q12(0.0f), Q12(4.0f), Q12(0.5f), Q12(1.0f), 0, 1,  Q12(0.0f), 0, Q12(0.01f), 3 },
    { { Q12(139.7f), Q12(-10.0f), Q12(-104.6f) }, Q12(2.0f), Q12(10.0f), Q12(0.0f), Q12(0.0f), Q12(0.5f), Q12(1.0f), 0, 0,  Q12(0.0f), 0, Q12(0.03f), 3 },
    { { Q12(139.7f), Q12(-10.0f), Q12(-104.6f) }, Q12(2.0f), Q12(10.0f), Q12(0.0f), Q12(0.0f), Q12(0.5f), Q12(1.0f), 0, 0,  Q12(0.0f), 0, Q12(0.03f), 0 },
    { { Q12(144.7f), Q12(-10.0f), Q12(-104.6f) }, Q12(2.0f), Q12(10.0f), Q12(0.0f), Q12(0.0f), Q12(0.5f), Q12(1.0f), 0, 0,  Q12(0.0f), 0, Q12(0.03f), 3 },
    { { Q12(144.7f), Q12(-10.0f), Q12(-104.6f) }, Q12(2.0f), Q12(10.0f), Q12(0.0f), Q12(0.0f), Q12(0.5f), Q12(1.0f), 0, 0,  Q12(0.0f), 0, Q12(0.03f), 0 },
    { { Q12(0.0f),   Q12(0.0f),   Q12(0.0f)    }, Q12(0.0f), Q12(0.0f),  Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), 0, 99, Q12(0.0f), 0, Q12(0.0f),  3 },
    { { Q12(144.7f), Q12(-10.0f), Q12(-104.6f) }, Q12(2.0f), Q12(10.0f), Q12(0.0f), Q12(0.0f), Q12(0.5f), Q12(1.0f), 0, 0,  Q12(0.0f), 0, Q12(0.03f), 0 },
    { { Q12(139.7f), Q12(0.0f),   Q12(-104.6f) }, Q12(2.0f), Q12(10.0f), Q12(0.0f), Q12(4.0f), Q12(0.5f), Q12(1.0f), 0, 1,  Q12(0.0f), 0, Q12(0.01f), 2 },
};

void func_800E8D20(void) // 0x800E8D20
{
    s32             flags;
    s_800ED7E0_ptr* ptr;

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Fs_QueueWaitForEmpty();
            Dms_HeaderFixOffsets(FS_BUFFER_20);

            D_800F4806 = 0;

            func_800E941C();
            func_800E9444(Chara_EndingCybil, &g_SysWork.npcs[0]);

            g_Cutscene_UpdateSibyl = true;
            g_Cutscene_MapMsgAudioIdx = 0;

            Model_AnimFlagsClear(&g_SysWork.playerWork.player.model, 2);
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimUnlock, &g_SysWork.playerWork.player, 0, false);
            g_Cutscene_Timer = Q12(0.0f);
            func_800D7E50(&g_SysWork.playerWork.player.position);
            func_800D947C();

            D_800F4824 = 0;
            D_800F4820 = 1;

            g_SysWork.lightIntensity = Q12(0.7f);
            SysWork_StateStepIncrement(0);
            break;

        case 1:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 26, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            SysWork_StateStepIncrement(0);

        case 2:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(0.0f), Q12(23.0f), true, true);
            break;

        case 3:
            func_800D7ED0();
            SysWork_StateStepIncrement(0);

        case 4:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(24.0f), Q12(39.0f), true, true);
            break;

        case 5:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 25, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 173, false);
            SysWork_StateStepIncrement(0);

        case 6:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(40.0f), Q12(122.0f), true, true);
            break;

        case 7:
            g_Cutscene_Timer = Q12(123.0f);
            Model_AnimFlagsSet(&g_SysWork.playerWork.player.model, 2);
            Event_WaitTimer(Q12(1.0f), false);
            break;

        case 8:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 172, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.npcs[0], 28, false);
            SysWork_StateStepIncrement(0);

        case 9:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(124.0f), Q12(162.0f), false, true);
            break;

        case 10:
            Event_DisplayMapMsgWithAudio(98, &g_Cutscene_MapMsgAudioIdx, &D_800ED9B4);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(124.0f), Q12(400.0f), true, false);
            break;

        case 11:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(124.0f), Q12(258.0f), false, true);
            break;

        case 12:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(10.0f), Q12(124.0f), Q12(400.0f), true, false);
            if (g_Cutscene_Timer > Q12(375.0f))
            {
                Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.3f), false);
            }
            break;

        default:
            D_800F4805++;

            Sd_SfxStop(Sfx_Unk1688);
            Sd_SfxStop(Sfx_Unk1689);
            Sd_SfxStop(Sfx_Unk1690);

            D_800F4820 = 0;

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);

            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            func_800E9490(&g_SysWork.npcs[0]);
            break;
    }

    if (D_800F4820 != 0)
    {
        switch (D_800F4824)
        {
            case 0:
                if (!(Rng_Rand16() & 0x70))
                {
                    func_800D7D74(0);
                }
                break;

            case 1:
                if (!(Rng_Rand16() & 0x70))
                {
                    func_800D7D74(0);
                }

            case 2:
                break;
        }

        if (D_800F4820 != 0)
        {
            ptr = D_800ED9BC[g_SysWork.sysStateSteps[0]];
            if (ptr != NULL)
            {
                do
                {
                    if (!(ptr->flags_38 & 2) || g_SysWork.sysStateSteps[0] != D_800ED9B8)
                    {
                        func_800DA774(ptr);
                    }

                    flags = ptr->flags_38;
                    ptr++;
                }
                while (flags & (1 << 0));
            }

            D_800ED9B8 = g_SysWork.sysStateSteps[0];
        }
    }
}

void func_800E9260(e_CharaId charaId, s32 arg1) // 0x800E9260
{
    // TODO: Setup buffer defines for these (or maybe they're included in existing buffer?)

    WorldGfx_CharaLmBufferAssign(1 << (arg1 - 1));

    switch (arg1)
    {
        case 1:
            Fs_CharaAnimDataAlloc(arg1, charaId, (s_AnmHeader*)FS_BUFFER_29, &g_SysWork.npcBoneCoordBuffer[0]);
            WorldGfx_CharaLoad(charaId, 0, (s_LmHeader*)FS_BUFFER_31, &D_800ED218);
            break;

        case 2:
            Fs_CharaAnimDataAlloc(arg1, charaId, (s_AnmHeader*)FS_BUFFER_30, &g_SysWork.npcBoneCoordBuffer[30]);
            WorldGfx_CharaLoad(charaId, 1, LM_BUFFER_2, &D_800ED220);
            break;

        case 3:
            Fs_CharaAnimDataAlloc(arg1, charaId, (s_AnmHeader*)FS_BUFFER_26, &g_NpcBoneCoords[HarryBone_Root]);
            WorldGfx_CharaLoad(charaId, 2, (s_LmHeader*)FS_BUFFER_32, &D_800ED228);
            break;

        case 4:
            func_800E94C0();
            Fs_CharaAnimDataAlloc(arg1 - 3, charaId, (s_AnmHeader*)FS_BUFFER_29, &g_SysWork.npcBoneCoordBuffer[0]);
            WorldGfx_CharaLoad(charaId, 0, (s_LmHeader*)FS_BUFFER_31, &D_800ED218);
            func_800E94F4();
            break;

        case 5:
            func_800E94C0();
            Fs_CharaAnimDataAlloc(arg1 - 3, charaId, (s_AnmHeader*)FS_BUFFER_33, &g_SysWork.npcBoneCoordBuffer[30]);
            WorldGfx_CharaLoad(charaId, 1, (s_LmHeader*)FS_BUFFER_34, &D_800ED220);
            func_800E94F4();
            break;
    }
}

void func_800E941C(void) // 0x800E941C
{
    Fs_QueueWaitForEmpty();
    WorldGfx_CharaModelProcessAllLoads();
}

void func_800E9444(e_CharaId charaId, s_SubCharacter* chara) // 0x800E9444
{
    bzero(chara, sizeof(s_SubCharacter));

    chara->model.charaId      = charaId;
    chara->model.controlState = 0;
    chara->model.anim.flags  |= AnimFlag_Visible;
}

void func_800E9490(s_SubCharacter* chara) // 0x800E9490
{
    chara->model.charaId = Chara_None;
}

void func_800E9498(void) // 0x800E9498
{
    g_MapOverlayHdr.charaUpdateFuncs[Chara_Incubator] = Unknown23_Update;
}

void func_800E94AC(void) // 0x800E94AC
{
    g_MapOverlayHdr.charaUpdateFuncs[Chara_Incubator] = Incubator_Update;
}

void func_800E94C0(void) // 0x800E94C0
{
    // @hack Weird pointer arithmetic in this func.
    // Tracking that separately lets it match, but there's probably still a cleaner way.
    #define SET_ANIM_FILE_IDX(ptr, val) \
        ((ptr)->animFileIdx = (val))

    s_CharaFileInfo* ptr = &CHARA_FILE_INFOS[Chara_EndingCybil];

    SET_ANIM_FILE_IDX(&CHARA_FILE_INFOS[Chara_EndingCybil], FILE_ANIM_SBL_LAST_ANM);
    ptr += 10;
    SET_ANIM_FILE_IDX(&CHARA_FILE_INFOS[Chara_BloodyIncubator], FILE_ANIM_BAR_LAST_ANM);
    ptr += 2;
    SET_ANIM_FILE_IDX(&CHARA_FILE_INFOS[Chara_EndingKaufmann], FILE_ANIM_KAU_LAST_ANM);
    ptr -= 3;
    SET_ANIM_FILE_IDX(&CHARA_FILE_INFOS[Chara_Incubator], FILE_ANIM_MAR_LAST_ANM);

    #undef SET_ANIM_FILE_IDX
}

void func_800E94F4(void) // 0x800E94F4
{
    #define SET_ANIM_FILE_IDX(ptr, val) \
        ((ptr)->animFileIdx = (val))

    // @hack Weird pointer arithmetic in this func.
    // Tracking that separately lets it match, but there's probably still a cleaner way.
    s_CharaFileInfo* ptr = &CHARA_FILE_INFOS[Chara_EndingCybil];

    SET_ANIM_FILE_IDX(&CHARA_FILE_INFOS[Chara_EndingCybil], FILE_ANIM_SBL2_ANM);
    ptr += 10;
    SET_ANIM_FILE_IDX(&CHARA_FILE_INFOS[Chara_BloodyIncubator], FILE_ANIM_BAR_ANM);
    ptr += 2;
    SET_ANIM_FILE_IDX(&CHARA_FILE_INFOS[Chara_EndingKaufmann], FILE_ANIM_KAU2_ANM);
    ptr -= 3;
    SET_ANIM_FILE_IDX(&CHARA_FILE_INFOS[Chara_Incubator], FILE_ANIM_MAR_ANM);

    #undef SET_ANIM_FILE_IDX
}

void Map_WorldObjectsInit(void) // 0x800E9528
{
    D_800F4805 = 0;
    D_800F48A4 = 0;

    WorldObject_ModelNameSet(&g_WorldObject_Real, "REAL_HID");
    WorldObject_ModelNameSet(&g_WorldObject_Ura, "URA_HIDE");
    WorldObject_ModelNameSet(&g_WorldObject_Under, "UNDER_HI");

    Math_Vector3Set(&g_WorldObject_UnkPos, Q12(100.0f), Q12(0.0f), Q12(-140.0f));
    WorldObject_Init(&g_WorldObject_Bin, "BIN_HIDE", 140.0f, 0.0f, -100.0f, 0.0f, 0.0f, 0.0f);
    WorldObject_Init(&g_WorldObject_Nu, "NU_HIDE", 139.7f, 0.0f, -99.1f, 0.0f, -90.0f, 0.0f);
    WorldObject_Init(&g_WorldObject_Baby, "BABY_HID", 140.0f, 0.0f, -100.0f, 0.0f, 0.0f, 0.0f);

    g_SysWork.lightBoneCoord = NULL;

    // Set light position.
    g_SysWork.lightPosition.vx = Q12(139.7f);
    g_SysWork.lightPosition.vy = Q12(-4.5f);
    g_SysWork.lightPosition.vz = Q12(-98.1f);

    g_SysWork.lensFlareBoneCoord = NULL;

    // Set light rotation.
    g_SysWork.lightRotation.vx = Q12_ANGLE(-90.0f);
    g_SysWork.lightRotation.vy = Q12_ANGLE(0.0f);
    g_SysWork.lightRotation.vz = Q12_ANGLE(0.0f);

    g_SysWork.lightIntensity = Q12(2.0f);

    D_800F4820 = 0;

    func_800D5D24();
    func_800E16FC();

    D_800F4838 = 0;
    D_800F4830 = 0;

    if (Savegame_EventFlagGet(EventFlag_449))
    {
        g_EndingIdx = GameEnding_GoodPlus;
    }
    else
    {
        g_EndingIdx = GameEnding_Good;
    }

    // Switch to `GameEnding_BadPlus` or `GameEnding_Bad` if vial wasn't found.
    if (!Savegame_EventFlagGet(EventFlag_M5S03_KaufmannHasMotorcycleVial))
    {
        g_EndingIdx += 2;
    }
}

void Map_WorldObjectsUpdate(void) // 0x800E972C
{
    if (g_Cutscene_UpdateBin)
    {
        Math_SetSVectorFast(&g_WorldObject_Bin.rotation, D_800F482C, Q12_ANGLE(0.0f), Q12_ANGLE(0.0f));
        WorldObjects_Add(&g_WorldObject_Bin.object, &g_WorldObject_Bin.position, &g_WorldObject_Bin.rotation);
        D_800F482C -= Q12_DIV(g_DeltaTime, Q12(3.6f));
    }

    if (g_Cutscene_UpdateNu)
    {
        WorldObjects_Add(&g_WorldObject_Nu.object, &g_WorldObject_Nu.position, &g_WorldObject_Nu.rotation);
    }

    if (g_Cutscene_UpdateBaby)
    {
        WorldObjects_Add(&g_WorldObject_Baby.object, &g_WorldObject_Baby.position, &g_WorldObject_Baby.rotation);
    }

    if (Savegame_EventFlagGet(EventFlag_577) && !Savegame_EventFlagGet(EventFlag_578) && !Savegame_EventFlagGet(EventFlag_582))
    {
        if (Savegame_EventFlagGet(EventFlag_M5S03_KaufmannHasMotorcycleVial))
        {
            func_800E14DC(&g_SysWork.playerWork.player, &g_SysWork.npcs[2], false);
        }
        else
        {
            func_800E14DC(&g_SysWork.playerWork.player, &g_SysWork.npcs[4], false);
        }
    }

    if (D_800F4818 || D_800F4819)
    {
        func_800E98EC();
    }
}

void func_800E9874(void) // 0x800E9874
{
    func_800E1854();

    if (D_800F4820 != 0)
    {
        if (D_800F4838 == 0)
        {
            func_800D5E78();
            func_800D7D08();
        }

        func_800D917C();
    }

    if (D_800F4830 > 0)
    {
        func_800D8858(D_800F4830 - 1);
    }
}

void func_800E98EC(void) // 0x800E98EC
{
    static const SVECTOR3 DEFAULT_ROT = {}; // .rodata 0x800CD168

    s32     i;
    VECTOR3 objPos;

    if (D_800F4819 != 2)
    {
        D_800F48A4 += Q12_MULT_PRECISE(g_DeltaTime, Q12(2.67f));
    }

    if (D_800F48A4 > Q12(20.0f))
    {
        D_800F48A4 -= Q12(20.0f);
    }

    for (i = 0; i < 3; i++)
    {
        if (i == 0)
        {
            objPos.vx = g_WorldObject_UnkPos.vx;
            objPos.vy = g_WorldObject_UnkPos.vy;
            objPos.vz = g_WorldObject_UnkPos.vz - D_800F48A4;
        }
        else if (i == 1)
        {
            objPos.vz = (g_WorldObject_UnkPos.vz - D_800F48A4) - Q12(20.0f);
        }
        else
        {
            objPos.vz = (g_WorldObject_UnkPos.vz - D_800F48A4) + Q12(20.0f);
        }

        if (D_800F4818)
        {
            WorldObjects_Add(&g_WorldObject_Real, &objPos, &DEFAULT_ROT);
        }
        if (D_800F4819)
        {
            WorldObjects_Add(&g_WorldObject_Ura, &objPos, &DEFAULT_ROT);
        }
        if (D_800F481A)
        {
            WorldObjects_Add(&g_WorldObject_Under, &objPos, &DEFAULT_ROT);
        }
    }

    Collision_TriggerFlagBitsClear(CollisionTriggerFlag_All);

    if (D_800F4818)
    {
        Collision_TriggerFlagBitsSet(2);
    }

    if (D_800F4819)
    {
        Collision_TriggerFlagBitsSet(4);
    }
}

void func_800E9AC8(void) // 0x800E9AC8
{
    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimLock, &g_SysWork.playerWork.player, 0, false);

            if (g_EndingIdx == GameEnding_GoodPlus || g_EndingIdx == GameEnding_Good)
            {
                g_SysWork.sysStateSteps[0] = 3;
                break;
            }

            GameFs_StfRollBinLoad();
            g_SysWork.sysStateSteps[0]++;

        case 1:
            if (Fs_QueueGetLength() == 0)
            {
                func_801E2E28(g_EndingIdx);
                g_SysWork.sysStateSteps[0]++;
            }
            break;

        case 2:
            if (func_801E2ED8())
            {
                g_SysWork.sysStateSteps[0]++;
            }
            break;

        case 3:
            if (func_801E2FC0())
            {
                Savegame_EventFlagSet(EventFlag_588);
                SysWork_StateSetNext(SysState_Gameplay);
            }
            break;
    }
}

void func_800E9C28(void) // 0x800E9C28
{
    g_Screen_FadeStatus = ScreenFadeState_FadeOutComplete;

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimLock, &g_SysWork.playerWork.player, 0, false);
            g_SysWork.sysStateSteps[0]++;
            break;

        case 1:
            Bgm_CrossfadeToTrack(BgmCmd_Track5);
            break;

        case 2:
            g_SavegamePtr->isNextFearMode = true;

            g_SavegamePtr->clearGameCount++;
            g_SavegamePtr->clearGameCount = CLAMP(g_SavegamePtr->clearGameCount, 1, 99);

            g_SavegamePtr->currentEndingFlags            = 1 << (g_EndingIdx - 1);
            g_SavegamePtr->clearGameEndings             |= 1 << (g_EndingIdx - 1);
            g_GameWorkConst->config.extraOptionsEnabled |= 1 << (g_EndingIdx - 1);

            g_SavegamePtr->locationId = SaveLocationId_NextFear;

            SysWork_StateSetNext(SysState_StatusMenu);
            break;
    }
}

