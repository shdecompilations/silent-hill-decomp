#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "main/rng.h"
#include "maps/shared.h"
#include "maps/characters/twinfeeler.h"
#include "maps/unk_m3s03.h"

#define twinfeelerProps twinfeeler->properties.twinfeeler

void func_800D078C(void) // 0x800D078C
{
    // @hack string needs to be in separate TU for the alignment in this TU to match properly.
    // Might need to move this func + related ones to new split?
    extern const char D_800CA77C[]; // "SPUM602F".
    u32               tpage1;
    u16               x;
    u16               y;
    s_Texture*        tex;

    tex    = WorldMap_ActiveTextureInfoGet(D_800CA77C);
    tpage1 = tex->imageDesc.tPage[1];
    x      = tex->imageDesc.clutX;
    y      = tex->imageDesc.clutY;

    D_800DF554 = tpage1;
    D_800DF558 = tpage1 | 0x20;
    D_800DF55C = tpage1 | 0x40;

    D_800DF560 = getClut(x, y + 1);
    D_800DF564 = getClut(x, y + 14);
    D_800DF568 = getClut(x, y + 15);
    D_800DF56C = getClut(x, y + 13);
    D_800DF570 = x >> 4;
}

void func_800D0840(void) // 0x800D0840
{
    RECT sp10;
    s32  temp_s0;
    s32  temp_s2;
    s32  temp_s3;
    s32  temp_t0;
    s32  var_a0;
    s32  var_a1_5;
    s32  var_a2;
    s32  i;
    s32  var_v1;
    s32  var_s6;
    u16* ptr0;
    u16* ptr1;
    u16* ptr2;

    var_s6 = 7;

    sp10.x = D_800DF570 * 16;
    sp10.y = 1;
    sp10.w = 16;
    sp10.h = 1;

    ptr0 = &D_800DB980;

    StoreImage(&sp10, ptr0);
    DrawSync(0);

    temp_t0 = D_800DB980[7];

    temp_s3 = (temp_t0 & 0x1F);
    temp_s2 = ((temp_t0 >> 5) & 0x1F);
    temp_s0 = ((temp_t0 >> 10) & 0x1F);

    temp_s3 += D_800DAA4C;
    temp_s2 += D_800DAA50;
    temp_s0 += D_800DAA54;

    ptr2 = &ptr0[16];
    ptr1 = &ptr0[32];

    Text_Debug_PositionSet(160, 92);

    for (i = 0; i < var_s6; i++)
    {
        ptr2[i] = 0;

        temp_t0 = ptr0[i];

        var_a2 = temp_t0 & 0x1F;
        var_a0 = (temp_t0 >> 5) & 0x1F;
        var_v1 = (temp_t0 >> 10) & 0x1F;

        var_a2 = temp_s3 - var_a2;
        var_a2 = MAX((var_a2 * D_800DAA44) >> 7, 0);
        var_a2 = CLAMP_HIGH(var_a2, 0x1F);

        var_a0 = temp_s2 - var_a0;
        var_a0 = MAX((var_a0 * D_800DAA44) >> 7, 0);
        var_a0 = CLAMP_HIGH(var_a0, 0x1F);

        var_v1 = temp_s0 - var_v1;
        var_v1 = MAX((var_v1 * D_800DAA44) >> 7, 0);
        var_v1 = CLAMP_HIGH(var_v1, 0x1F);

        if (temp_t0 != 0)
        {
            ptr1[i] = var_a2 | (var_a0 << 5) | (var_v1 << 10) | 0x8000;
        }
        else
        {
            ptr1[i] = 0;
        }
    }

    for (; i < 16; i++)
    {
        temp_t0 = ptr0[i];

        var_a2 = temp_t0 & 0x1F;
        var_a0 = (temp_t0 >> 5) & 0x1F;
        var_v1 = (temp_t0 >> 10) & 0x1F;

        var_a2 = var_a2 - temp_s3;
        var_a2 = MAX((var_a2 * D_800DAA48) >> 7, 0);
        var_a2 = CLAMP_HIGH(var_a2, 0x1F);

        var_a0 = var_a0 - temp_s2;
        var_a0 = MAX((var_a0 * D_800DAA48) >> 7, 0);
        var_a0 = CLAMP_HIGH(var_a0, 0x1F);

        var_v1 = var_v1 - temp_s0;
        var_v1 = MAX((var_v1 * D_800DAA48) >> 7, 0);
        var_v1 = CLAMP_HIGH(var_v1, 0x1F);

        if (temp_t0 != 0)
        {
            ptr2[i] = var_a2 | (var_a0 << 5) | (var_v1 << 10) | 0x8000;
        }
        else
        {
            ptr2[i] = 0;
        }

        ptr1[i] = 0;
    }

    sp10.y = 14;
    sp10.h = 2;

    LoadImage(&sp10, &D_800DB980[16]);
    DrawSync(0);

    D_800DB980[0] = 0;

    ptr0 = &D_800DB980[1];

    for (i = 1; i < 16; i++, ptr0++)
    {
        var_a2 = i * 2;
        var_a2 = Q12_MULT_PRECISE(var_a2, Q12(0.4f));

        var_a2 = MAX(var_a2, 0);
        var_a2 = CLAMP_HIGH(var_a2, 0x1F);

        temp_t0 = Q12_MULT_PRECISE(i * 2, Q12(0.35f));

        var_a1_5 = MAX(temp_t0, 0);
        var_a1_5 = CLAMP_HIGH(var_a1_5, 0x1F);

        var_a0 = MAX(temp_t0, 0);
        var_a0 = CLAMP_HIGH(var_a0, 0x1F);

        *ptr0 = (var_a1_5 << 5) | (var_a0 << 10) | var_a2 | ~0x7FFF;
    }

    sp10.y = 13;
    sp10.h = 1;

    LoadImage(&sp10, &D_800DB980);
    DrawSync(0);
}

void func_800D0C50(SVECTOR* rot, MATRIX* mat) // 0x800D0C50
{
    MATRIX outMat;

    SetRotMatrix(mat);
    SetTransMatrix(mat);
    Math_RotMatrixXyz(rot, &outMat);
    SetMulRotMatrix(&outMat);
}

void func_800D0CA0(q19_12 rotY, SVECTOR* pos) // 0x800D0CA0
{
    MATRIX      mat;
    VECTOR      trans;
    SVECTOR     rot; // Q3.12
    s_800E0988* ptr;

    ptr = &D_800E0988;

    SetRotMatrix(&ptr->world_8);
    SetTransMatrix(&ptr->world_8);

    ApplyRotMatrix(pos, &trans);

    trans.vx += ptr->world_8.t[0];
    trans.vy += ptr->world_8.t[1];
    trans.vz += ptr->world_8.t[2];

    TransMatrix(&mat, &trans);
    SetTransMatrix(&mat);

    rot.vx = Q12_ANGLE(0.0f);
    rot.vy = rotY;
    rot.vz = Q12_ANGLE(0.0f);
    Math_RotMatrixZxyNeg(&rot, &mat);
    SetMulRotMatrix(&mat);
}

void func_800D0D6C(MATRIX* worldMat, SVECTOR* pos, q19_12 rotY) // 0x800D0D6C
{
    SVECTOR     rot; // Q3.12
    s_800E0988* ptr;

    ptr = &D_800E0988;

    rot.vx = Q12_ANGLE(0.0f);
    rot.vy = rotY;
    rot.vz = Q12_ANGLE(0.0f);
    Math_RotMatrixXyz(&rot, worldMat);

    worldMat->t[0] = Q12_TO_Q8(ptr->x_0) + pos->vx;
    worldMat->t[1] = Q12_TO_Q8(Q12(0.0f));
    worldMat->t[2] = Q12_TO_Q8(ptr->z_4) + pos->vz;
}

void func_800D0DE4(SVECTOR* out, VECTOR* in, q19_12 headingAngle, q19_12 dist) // 0x800D0DE4
{
    q19_12 offsetX;
    q19_12 offsetZ;
    s32    x;
    s32    z;

    offsetX = Q12_MULT_PRECISE(Math_Sin(headingAngle), dist);
    offsetZ = Q12_MULT_PRECISE(Math_Cos(headingAngle), dist);

    x = in->vx - D_800E0988[0].x_0;
    z = in->vz - D_800E0988[0].z_4;

    out->vx = Q12_TO_Q8(x + offsetX);
    out->vy = Q12_TO_Q8(in->vy);
    out->vz = Q12_TO_Q8(z + offsetZ);
}

void func_800D0EC0(u8* buf, s32 w, s32 h) // 0x800D0EC0
{
    s32 i, j;
    s32 sum;

    for (i = 0; i < h - 1; i++)
    {
        for (j = 0; j < w; j++)
        {
            sum   = buf[w + j];
            sum  += buf[j - 1];
            sum  += buf[j];
            sum  += buf[j + 1];
            sum >>= 2;

            if (sum <= 0)
            {
                buf[j] = 0;
            }
            else
            {
                buf[j] = sum;
            }
        }

        buf += w;
    }
}

s32 func_800D0F40(s32 arg0, s32 arg1, s32 arg2) // 0x800D0F40
{
    s32 index;

    index = (arg0 * arg1) >> 8;
    if (index != 0 && g_DeltaTime != 0)
    {
        index += Rng_GenerateUInt(-2, 1);
        // TODO: `CLAMP` doesn't match?
        index = MIN(255, index);
        index = MAX(index, 0);
    }

    // @hack Needed to match weird a2 usage.
    if (arg2)
    {
        arg2++;
        arg2--;
    }

    return D_800DAA58[index];
}

void func_800D0FD4(s32* ord, void* arg1, u8* arg2, MATRIX* arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 abr) // 0x800D0FD4
{
    SVECTOR   vtx = { 0 };
    s32       sxy;
    s32       w;
    s32       h;
    s32       x;
    s32       y;
    s32       xbase;
    s32       ybase;
    s32       p;
    s32       flag;
    s32       col;
    s32*      ot;
    s32       depth;
    s16       sxy_l;
    s16       sxy_h;
    s32       sz;
    s32       i;
    s32       j;
    s32       col0;
    s32       col1;
    s32       col2;
    s32       col3;
    PACKET*   packet;
    DR_TPAGE* tpage;
    POLY_G4*  poly;
    u8*       ptr0;
    u8*       ptr1;

    col = D_800DAA58[0];
    sz  = RotTransPers(&vtx, &sxy, &p, &flag);

    // Divide overflow.
    if (flag & 0x20000)
    {
        return;
    }

    packet = GsOUT_PACKET_P;
    ot     = &ord[sz >> 1];

    sxy_l = sxy;
    sxy_h = sxy >> 16;
    depth = ReadGeomScreen();

    w = (depth * 8) / sz + 1;
    h = (depth * 12) / sz + 1;

    xbase = sxy_l - (arg5 / 2) * w;
    ybase = sxy_h - arg6 * h;

    for (i = 1; i < arg6 + 1; i++)
    {
        col3 = col;
        col1 = col3;

        ptr0 = arg2 + ((i - 1) * arg7);
        ptr0 = ptr0 + 1;
        ptr1 = arg2 + (i * arg7);
        ptr1 = ptr1 + 1;

        y = ((i - 1) * h) + ybase;

        for (j = 1; j < arg5 + 1; j++)
        {
            poly = packet;

            col0 = col1;
            col2 = col3;

            if (j == arg5)
            {
                col3 = col;
                col1 = col3;
            }
            else
            {
                col1 = func_800D0F40(*ptr0, arg4, abr);

                if (i == arg6)
                {
                    col3 = col;
                }
                else
                {
                    col3 = func_800D0F40(*ptr1, arg4, abr);
                }
            }

            if (col0 != col || col1 != col0 || col2 != col1 || col3 != col2)
            {
                x = (j - 1) * w + xbase;
                setXYWH(poly, x, y, w, h);

                *(s32*)&poly->r0 = col0;
                *(s32*)&poly->r1 = col1;
                *(s32*)&poly->r2 = col2;
                *(s32*)&poly->r3 = col3;

                setPolyG4(poly);
                setSemiTrans(poly, 1);
                addPrim(ot, packet);
            }

            packet += sizeof(POLY_G4);
            ptr0++;
            ptr1++;
        }
    }

    tpage = packet;

    setDrawTPage(tpage, 0, 1, getTPage(0, abr, 320, 0));
    addPrim(ot, tpage);
    packet        += sizeof(DR_TPAGE);
    GsOUT_PACKET_P = packet;
}

void func_800D13B4(u8* arg0, s32 arg1, s32 arg2, s32 arg3) // 0x800D13B4
{
    u8* buf;
    s32 i;
    s32 rnd;
    s32 index;

    buf = arg0 + (arg1 * (arg2 - 1));
    memset(buf, 0, arg1);

    if (arg3 > Q12(0.75f))
    {
        for (i = 0; i < 3; i++)
        {
            if (Rng_Rand16() & 0x70)
            {
                rnd            = Rng_Rand16() >> 2;
                index          = (rnd % (arg1 / 4)) + (arg1 / 4);
                buf[index + 1] = 0xff;
            }
        }
    }
}

void func_800D1478(SVECTOR* arg0, s32 arg1, q19_12 headingAgle, s32 mode, SVECTOR* arg4) // 0x800D1478
{
    s32    x;
    s32    y;
    s32    z;
    q19_12 dist;
    q19_12 angleStep;

    x = 0;
    y = 0;
    z = 0;

    switch (mode)
    {
        case 0:
        case 1:
            dist         = (arg1 > Q12(0.75f)) ? 128 : 76;
            angleStep    = mode ? Q12_ANGLE(-90.0f) : Q12_ANGLE(90.0f);
            headingAgle += angleStep;

            x = Q12_MULT_PRECISE(dist, Math_Sin(headingAgle));
            z = Q12_MULT_PRECISE(dist, Math_Cos(headingAgle));
            y = (arg1 > Q12(0.75f)) ? 0x59 : 0x80;
            break;

        case 2:
            x = arg4->vx;
            y = arg4->vy;
            z = arg4->vz;
            break;

        case 3:
            break;
    }

    arg0->vx += Q12_MULT_PRECISE(x, g_DeltaTime);
    arg0->vy += Q12_MULT_PRECISE(y, g_DeltaTime);
    arg0->vz += Q12_MULT_PRECISE(z, g_DeltaTime);
}

void func_800D1604(GsOT_TAG* ot, int arg1) // 0x800D1604
{
    SVECTOR     sp28;
    SVECTOR     sp30;
    s_800DF580* iter;
    int         i;
    int         tick;
    int         temp_s4;

    sp28 = D_800CA788;

    memset(&sp30, 0, sizeof(sp30));

    iter = D_800DF580;

    for (i = 0; i < 16; iter++, i++)
    {
        if (iter->field_8 <= 0)
        {
            continue;
        }

        tick = (iter->field_C8 != NULL) ? iter->field_C8(iter) : 1;

        if (tick)
        {
            func_800D1478(&iter->field_14, iter->field_8, arg1, iter->field_4, &iter->field_C);
            func_800D0D6C(&iter->worldMat, &iter->field_14, Q12_ANGLE(0.0f));

            temp_s4 = func_80055F08(&sp30, &sp28, &iter->worldMat);

            func_800D0CA0(0, &iter->field_14);

            if (g_DeltaTime != Q12(0.0f))
            {
                func_800D13B4(&iter->field_40[1], 12, 11, iter->field_8);
                func_800D0EC0(&iter->field_40[1], 12, 11);
            }

            func_800D0FD4(ot, &iter->field_14, &iter->field_40[1], &iter->worldMat,
                          Q12_MULT_PRECISE(temp_s4, iter->field_CC), 11, 9, 12, iter->field_3C);
            iter->field_8 -= g_DeltaTime;
        }
    }
}

void func_800D17FC(void) // 0x800D17FC
{
    s32         i;
    s_800DF580* ptr;

    ptr = D_800DF580;
    for (i = 0; i < 16; i++, ptr++)
    {
        ptr->field_8  = 0;
        ptr->field_D0 = 0;
        ptr->field_C8 = NULL;

        memset(&ptr->field_40, 0, sizeof(ptr->field_40));
    }
}

void func_800D185C(s32 arg0, s32 idx) // 0x800D185C
{
    s_800DF580* ptr;

    ptr = &D_800DF580[idx];
    if (ptr->field_8 <= 0)
    {
        ptr->field_4  = idx & 0x1;
        ptr->field_0  = idx;
        ptr->field_8  = Q12(1.0f);
        ptr->field_3C = 1;
        ptr->field_C8 = NULL;
        ptr->field_D0 = 0;
        ptr->field_CC = Q12(1.0f);

        memset(&ptr->field_40, 0, sizeof(ptr->field_40));
        func_800D0DE4(&ptr->field_14, arg0, Q12_ANGLE(0.0f), Q12(0.0f));
    }
}

s_800DF580* func_800D1900(VECTOR* arg0, s32 arg1) // 0x800D1900
{
    s32         i;
    s_800DF580* ptr;

    ptr = D_800DF580;
    for (i = 0; i < 16; i++, ptr++)
    {
        if (ptr->field_8 <= Q12(0.0f))
        {
            ptr->field_C8 = NULL;
            ptr->field_D0 = 0;
            ptr->field_0  = i;
            ptr->field_8  = Q12(1.0f);
            ptr->field_4  = arg1;
            ptr->field_3C = 1;

            func_800D0DE4(&ptr->field_14, arg0, Q12_ANGLE(0.0f), Q12(0.0f));
            memset(&ptr->field_40, 0, sizeof(ptr->field_40));
            return ptr;
        }
    }

    return NULL;
}

void func_800D19AC(VECTOR3* pos) // 0x800D19AC
{
    func_800D1900(pos, 3);
}

void func_800D19CC(const VECTOR3* pos) // 0x800D19CC
{
    D_800E08F0.vx = pos->vx;
    D_800E08F0.vy = pos->vy;
    D_800E08F0.vz = pos->vz;
}

bool func_800D19F0(s_800DF580* arg0) // 0x800D19F0
{
    switch (arg0->field_D0)
    {
        case 0:
            if (arg0->field_D4 < 0)
            {
                arg0->field_D0 = 1;
            }

            arg0->field_D4 -= g_DeltaTime;
            return false;

        case 1:
            if (arg0->field_8 > Q12(0.6f))
            {
                if (arg0->field_CC < Q12(0.65f))
                {
                    arg0->field_CC += Q12_MULT_PRECISE(g_DeltaTime, Q12(0.34f));
                }
                else
                {
                    arg0->field_CC = Q12(0.65f);
                }
            }
            else
            {
                if (arg0->field_CC > Q12(0.0f))
                {
                    arg0->field_CC -= Q12_MULT_PRECISE(g_DeltaTime, Q12(0.25f));
                }
                else
                {
                    arg0->field_CC = Q12(0.0f);
                }
            }
            break;
    }

    return true;
}

void func_800D1AFC(void) // 0x800D1AFC
{
    VECTOR      sp10;
    q19_12      var_s1;
    s32         i;
    s_800DF580* ptr;
    VECTOR3*    vec;

    var_s1 = Rng_Rand16();

    for (i = 0; i < 5; i++)
    {
        vec = &D_800E0698.field_258;

        sp10.vx = vec->vx;
        sp10.vy = vec->vy;
        sp10.vz = vec->vz;

        ptr = func_800D1900(&sp10, 2);

        if (ptr != NULL)
        {
            var_s1       += Rng_GenerateUInt(-0x80, 0x80 - 1);
            ptr->field_C  = Q12_MULT_PRECISE(Math_Sin(var_s1), 0x99);
            ptr->field_E  = Rng_GenerateUInt(0x70, 0x90 - 1);
            ptr->field_10 = Q12_MULT_PRECISE(Math_Cos(var_s1), 0x99);
            ptr->field_8  = Q12(1.9f);
            ptr->field_D4 = Rng_GenerateUInt(0, 255);
            ptr->field_CC = Q12(0.3f);
            ptr->field_D0 = 0;
            ptr->field_C8 = func_800D19F0;
            ptr->field_3C = 1;
        }

        var_s1 += Q12(0.2f);
    }
}

void func_800D1C48(void) // 0x800D1C48
{
    VECTOR      sp10;    // Q19.12
    s_800DF580* ptr;

    if (!(Rng_Rand16() & 0x30))
    {
        sp10.vx = D_800E08F0.vx;
        sp10.vy = D_800E08F0.vy + Q12(0.2f);
        sp10.vz = D_800E08F0.vz;

        ptr = func_800D1900(&sp10, 2);

        if (ptr != NULL)
        {
            ptr->field_C  = Rng_Rand16() % 0x99 - 0x4C;
            ptr->field_E  = -0x4C;
            ptr->field_10 = Rng_Rand16() % 0x99 - 0x4C;
            ptr->field_3C = 1;
        }
    }
}

void func_800D1D3C(GsOT_TAG* tag, SVECTOR3* arg1, MATRIX* worldMat, s32 arg3) // 0x800D1D3C
{
    SVECTOR   sp28;
    SVECTOR   sp30;
    SVECTOR   sp38;
    SVECTOR   sp40;
    long      sp48;
    long      sp4C;
    long      sp50;
    long      sp54;
    long      sp58;
    long      sp5C;
    long      sp60;
    s32       temp_a3;
    s32       temp_t0;
    s32       temp_t1;
    s32       temp_v0_3;
    s32       temp_v1_2;
    u32       temp_a1;
    s32       temp_a2;
    s32       temp_v0;
    s32       var_a1;
    GsOT_TAG* temp_a1_2;
    POLY_FT4* poly;
    POLY_FT4* poly2;

    PushMatrix();
    temp_v0 = func_80055F08(arg1, &D_800DAE58, worldMat) & 0xFF;
    temp_a2 = Q12_MULT_PRECISE(temp_v0, arg3);
    if (temp_a2 < 0x100)
    {
        var_a1 = temp_a2;
    }
    else
    {
        var_a1 = 0xFF;
    }

    func_80055E90((CVECTOR*)&sp48, var_a1 & 0xFF);
    PopMatrix();

    temp_t1   = arg1->vx - 0x80;
    temp_a3   = arg1->vy;
    temp_t0   = arg1->vz - 0x80;
    temp_v0_3 = arg1->vx + 0x80;
    temp_v1_2 = arg1->vz + 0x80;

    sp28.vx = temp_t1;
    sp28.vy = temp_a3;
    sp28.vz = temp_t0;

    sp30.vx = temp_v0_3;
    sp30.vy = temp_a3;
    sp30.vz = temp_t0;

    sp38.vx = temp_t1;
    sp38.vy = temp_a3;
    sp38.vz = temp_v1_2;

    sp40.vx = temp_v0_3;
    sp40.vy = temp_a3;
    sp40.vz = temp_v1_2;

    temp_a1 = RotTransPers4(&sp28, &sp30, &sp38, &sp40, &sp4C, &sp50, &sp54, &sp58, &sp5C, &sp60);
    poly = poly2 = (POLY_FT4*)GsOUT_PACKET_P;

    if (sp60 & 0x20000)
    {
        return;
    }

    poly2->tpage = D_800DF558;
    poly2->clut  = D_800DF564;

    setUV4(poly2,
           0xE0, 0xC0,
           0xFF, 0xC0,
           0xE0, 0xE0,
           0xFF, 0xE0);

    *(s32*)&poly2->x0 = *(s32*)&sp4C;
    *(s32*)&poly2->x1 = *(s32*)&sp50;
    *(s32*)&poly2->x2 = *(s32*)&sp54;
    *(s32*)&poly2->x3 = *(s32*)&sp58;
    *(s32*)&poly2->r0 = *(s32*)&sp48;

    setPolyFT4(poly2);
    setSemiTrans(poly2, true);

    temp_a1_2 = &tag[temp_a1 >> 1];
    addPrim(&temp_a1_2[25], poly2);

    poly        = poly2 + 1;
    poly->tpage = D_800DF55C;
    poly->clut  = D_800DF568;

    setUV4(poly,
           0xE0, 0xC0,
           0xFF, 0xC0,
           0xE0, 0xE0,
           0xFF, 0xE0);

    *(s32*)&poly->x0 = *(s32*)&sp4C;
    *(s32*)&poly->x1 = *(s32*)&sp50;
    *(s32*)&poly->x2 = *(s32*)&sp54;
    *(s32*)&poly->x3 = *(s32*)&sp58;
    *(s32*)&poly->r0 = *(s32*)&sp48;

    setPolyFT4(poly);
    setSemiTrans(poly, true);

    temp_a1_2 = &tag[temp_a1 >> 1];
    addPrim(&temp_a1_2[25], poly);

    poly++;
    GsOUT_PACKET_P = (PACKET*)poly;
}

void func_800D1FF4(GsOT_TAG* arg0) // 0x800D1FF4
{
    SVECTOR     sp10 = { 0 };
    MATRIX      worldMat;
    s32         i;
    s_800E0300* ptr1;
    s_800E0988* ptr0;

    ptr1 = D_800E0300;

    func_800D0D6C(&worldMat, &sp10, 0);

    ptr0 = &D_800E0988;

    SetRotMatrix(&ptr0->world_8);
    SetTransMatrix(&ptr0->world_8);

    for (i = 0; i < ARRAY_SIZE(D_800E0300); i++, ptr1++)
    {
        if (ptr1->timer_0 > 0)
        {
            if (ptr1->timer_0 > Q12(1.2f))
            {
                ptr1->field_4 = Q12(1.0f);
            }
            else
            {
                ptr1->field_4 -= Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.3f);
                ptr1->field_4  = MAX(ptr1->field_4, Q12(0.0f));
            }

            func_800D1D3C(arg0, &ptr1->field_8, &worldMat, ptr1->field_4);
            ptr1->timer_0 -= g_DeltaTime;
        }
    }
}

void func_800D2120(void) // 0x800D2120
{
    s32         i;
    s_800E0300* ptr;

    ptr = D_800E0300;

    for (i = 0; i < 16; i++, ptr++)
    {
        ptr->timer_0 = 0;
        ptr->field_4 = 0x800;
    }
}

void func_800D2150(VECTOR* arg0, s32 arg1) // 0x800D2150
{
    s_800E0300* ptr;

    ptr = &D_800E0300[arg1];
    if (ptr->timer_0 <= Q12(0.0f))
    {
        ptr->timer_0 = Q12(3.0f);
        func_800D0DE4(&ptr->field_8, arg0, Q12_ANGLE(0.0f), Q12(0.0f));
    }
}

void func_800D21AC(GsOT_TAG* ot, MATRIX* worldMat, s32 arg2, s32 arg3) // 0x800D21AC
{
    SVECTOR   sp20[9];
    DVECTOR   sp68[9];
    CVECTOR   sp90[9];
    u16       spB8[9];
    s32       spD0;
    SVECTOR*  var_s0;
    s32       temp_v0;
    s32       temp_v0_2;
    s32       temp_v1;
    s32       k;
    s32       i;
    s32       j;
    s32       var_s4;
    s32       var_v0;
    s32       var_v0_2;
    DVECTOR*  var_s1;
    CVECTOR*  var_s1_2;
    u16*      var_a0_3;
    s32       idx;
    u8*       var_t4;
    GsOT_TAG* temp_v0_3;
    GsOT_TAG* ot1;
    POLY_GT4* poly;
    POLY_GT4* poly2;

    var_s4 = 0;
    var_s0 = sp20;

    temp_v0 = Q12_MULT_PRECISE(arg3, 0x100);
    temp_v1 = temp_v0;

    for (i = -1; i < 2; i++)
    {
        for (k = -1; k < 2; k++)
        {
            var_s0->vx = temp_v0 * k;
            var_s0->vy = 0;
            var_s0->vz = temp_v1 * i;
            var_s0++;
        }
    }

    PushMatrix();

    var_s0   = sp20;
    var_s1_2 = sp90;

    for (j = 0; j < 9; j++)
    {
        temp_v0_2 = func_80055F08(var_s0, &D_800DAE60, worldMat);

        func_80055E90(var_s1_2, Q12_MULT_PRECISE(temp_v0_2, arg2));
        var_s1_2++;
        var_s0++;
    }

    PopMatrix();

    var_a0_3 = &spB8[0];

    for (i = 0; i < 3; i++)
    {
        var_v0 = (-0x80 + i * 0x20) & 0xFF;

        if (i == 2)
        {
            var_v0 -= 1;
        }

        for (k = 0; k < 3; k++)
        {
            var_v0_2 = (0x40 + k * 0x20) & 0xFF;
            if (k == 2)
            {
                var_v0_2 -= 1;
            }
            *var_a0_3 = (var_v0_2 & 0xFF) | ((var_v0 & 0xFF) << 8);
            var_a0_3++;
        }
    }

    var_s0 = sp20;
    var_s1 = sp68;

    for (j = 0; j < 3; j++)
    {
        temp_v1 = RotTransPers3(&var_s0[0], &var_s0[1], &var_s0[2],
                                &var_s1[0], &var_s1[1], &var_s1[2], &spD0, &spD0);
        if (j == 2)
        {
            var_s4 = temp_v1;
        }
        var_s0 += 3;
        var_s1 += 3;
    }

    temp_v0_3 = &ot[var_s4 >> 1];
    ot1       = &temp_v0_3[25];

    poly = (POLY_GT4*)GsOUT_PACKET_P;

    var_t4 = D_800DAE68;

    for (j = 0; j < 4; j++, var_t4 += 4)
    {
        idx = var_t4[0];
        COPY_GT4_DATA(poly, idx, (u16*)&spB8, (s32*)&sp68, (s32*)&sp90, 0);
        idx = var_t4[1];
        COPY_GT4_DATA(poly, idx, (u16*)&spB8, (s32*)&sp68, (s32*)&sp90, 1);
        idx = var_t4[2];
        COPY_GT4_DATA(poly, idx, (u16*)&spB8, (s32*)&sp68, (s32*)&sp90, 2);
        idx = var_t4[3];
        COPY_GT4_DATA(poly, idx, (u16*)&spB8, (s32*)&sp68, (s32*)&sp90, 3);

        poly->tpage = D_800DF558;
        poly->clut  = D_800DF564;

        setPolyGT4(poly);
        setSemiTrans(poly, 1);

        addPrim(ot1, poly);
        poly++;

        idx = var_t4[0];
        COPY_GT4_DATA(poly, idx, (u16*)&spB8, (s32*)&sp68, (s32*)&sp90, 0);
        idx = var_t4[1];
        COPY_GT4_DATA(poly, idx, (u16*)&spB8, (s32*)&sp68, (s32*)&sp90, 1);
        idx = var_t4[2];
        COPY_GT4_DATA(poly, idx, (u16*)&spB8, (s32*)&sp68, (s32*)&sp90, 2);
        idx = var_t4[3];
        COPY_GT4_DATA(poly, idx, (u16*)&spB8, (s32*)&sp68, (s32*)&sp90, 3);

        poly->tpage = D_800DF55C;
        poly->clut  = D_800DF568;

        setPolyGT4(poly);
        setSemiTrans(poly, 1);

        addPrim(ot1, poly);
        poly++;
    }

    GsOUT_PACKET_P = (PACKET*)poly;
}

void func_800D2684(VECTOR* pos, q19_12 headingAngle) // 0x800D2684
{
    s32         i;
    s_800E0900* curPtr;

    curPtr = D_800E0900;

    for (i = 0; i < ARRAY_SIZE(D_800E0900); i++, curPtr++)
    {
        if (curPtr->field_0 <= Q12(0.0f))
        {
            curPtr->field_0  = Q12(4.5f);
            curPtr->field_C  = Q12(0.5f);
            curPtr->field_10 = Q12(0.4f);

            func_800D0DE4(&curPtr->field_4, pos, Q12_ANGLE(0.0f), Q12(0.0f));
            return;
        }
    }
}

void func_800D26FC(VECTOR* pos, q19_12 headingAngle) // 0x800D26FC
{
    s32         i;
    s_800E0440* curPtr;

    curPtr = D_800E0440;

    for (i = 0; i < ARRAY_SIZE(D_800E0440); i++, curPtr++)
    {
        if (curPtr->field_0 <= Q12(0.0f))
        {
            curPtr->field_0  = Q12(8.0f);
            curPtr->field_C  = Q12(0.0f);
            curPtr->field_10 = Q12(0.0f);
            curPtr->field_14 = headingAngle;

            func_800D0DE4(&curPtr->field_4, pos, Q12_ANGLE(0.0f), Q12(0.0f));
            break;
        }
    }

    func_800D2684(pos, headingAngle);
}

void func_800D2790(GsOT_TAG* ot, MATRIX* worldMat, s32 arg2, s32 arg3) // 0x800D2790
{
    SVECTOR   sp20[9];
    DVECTOR   sp68[9];
    CVECTOR   sp90[9];
    u16       spB8[9];
    u16       spD0[9];
    s32       spE8;
    SVECTOR*  var_s0;
    s32       temp_v0;
    s32       temp_v0_2;
    s32       temp_v1;
    s32       k;
    s32       i;
    s32       j;
    s32       l;
    s32       var_s4;
    s32       var_v0;
    s32       var_v0_2;
    DVECTOR*  var_s1;
    CVECTOR*  var_s1_2;
    u16*      var_a0_3;
    s32       idx;
    u8*       var_t4;
    GsOT_TAG* temp_v0_3;
    GsOT_TAG* ot0;
    POLY_GT4* poly;
    POLY_GT4* poly2;
    u16*      var_v1_2;
    s32       var_a1_2;

    var_s4 = 0;
    var_s0 = sp20;

    temp_v0 = Q12_MULT_PRECISE(arg3, Q12(0.05f));
    temp_v1 = temp_v0;

    for (i = -1; i < 2; i++)
    {
        for (k = -1; k < 2; k++)
        {
            var_s0->vx = temp_v0 * k;
            var_s0->vy = 0;
            var_s0->vz = temp_v1 * i;
            var_s0++;
        }
    }

    PushMatrix();

    var_s0   = sp20;
    var_s1_2 = sp90;

    for (l = 0; l < 9; l++)
    {
        temp_v0_2 = func_80055F08(var_s0, &D_800DAE78, worldMat);
        var_a1_2  = CLAMP_HIGH(Q12_MULT_PRECISE(temp_v0_2, arg2), 0xFF);

        func_80055E90(var_s1_2, var_a1_2);
        var_s1_2++;
        var_s0++;
    }

    PopMatrix();

    var_a0_3 = spB8;

    for (i = 0; i < 3; i++)
    {
        var_v0 = (-0x80 + i * 0x20) & 0xFF;

        if (i == 2)
        {
            var_v0 -= 1;
        }

        for (k = 0; k < 3; k++)
        {
            var_v0_2 = (k << 5) & 0xFF;
            if (k == 2)
            {
                var_v0_2 -= 1;
            }
            *var_a0_3 = (var_v0_2 & 0xFF) | ((var_v0 & 0xFF) << 8);
            var_a0_3++;
        }
    }

    var_v1_2 = spD0;

    for (i = 0; i < 3; i++)
    {
        var_v0 = (-0x40 + i * 0x20) & 0xFF;
        if (i == 2)
        {
            var_v0 -= 1;
        }

        for (k = 0; k < 3; k++)
        {
            var_v0_2 = (k << 5) & 0xFF;
            if (k == 2)
            {
                var_v0_2 -= 1;
            }
            *var_v1_2 = (var_v0_2 & 0xFF) | ((var_v0 & 0xFF) << 8);
            var_v1_2++;
        }
    }

    var_s0 = sp20;
    var_s1 = sp68;

    for (l = 0; l < 3; l++)
    {
        temp_v1 = RotTransPers3(&var_s0[0], &var_s0[1], &var_s0[2],
                                &var_s1[0], &var_s1[1], &var_s1[2], &spE8, &spE8);
        if (l == 2)
        {
            var_s4 = temp_v1;
        }
        var_s0 += 3;
        var_s1 += 3;
    }

    temp_v0_3 = &ot[var_s4 >> 1];
    ot0       = &temp_v0_3[25];

    poly = (POLY_GT4*)GsOUT_PACKET_P;

    var_t4 = D_800DAE80;

    for (l = 0; l < 4; l++, var_t4 += 4)
    {
        idx = var_t4[0];
        COPY_GT4_DATA(poly, idx, (u16*)&spB8, (s32*)&sp68, (s32*)&sp90, 0);
        idx = var_t4[1];
        COPY_GT4_DATA(poly, idx, (u16*)&spB8, (s32*)&sp68, (s32*)&sp90, 1);
        idx = var_t4[2];
        COPY_GT4_DATA(poly, idx, (u16*)&spB8, (s32*)&sp68, (s32*)&sp90, 2);
        idx = var_t4[3];
        COPY_GT4_DATA(poly, idx, (u16*)&spB8, (s32*)&sp68, (s32*)&sp90, 3);

        poly->tpage = 0x4D;
        poly->clut  = D_800DF56C;

        setPolyGT4(poly);
        setSemiTrans(poly, 1);

        addPrim(ot0, poly);
        poly++;

        idx = var_t4[0];
        COPY_GT4_DATA(poly, idx, (u16*)&spD0, (s32*)&sp68, (s32*)&sp90, 0);
        idx = var_t4[1];
        COPY_GT4_DATA(poly, idx, (u16*)&spD0, (s32*)&sp68, (s32*)&sp90, 1);
        idx = var_t4[2];
        COPY_GT4_DATA(poly, idx, (u16*)&spD0, (s32*)&sp68, (s32*)&sp90, 2);
        idx = var_t4[3];
        COPY_GT4_DATA(poly, idx, (u16*)&spD0, (s32*)&sp68, (s32*)&sp90, 3);

        poly->tpage = 0x4D;
        poly->clut  = D_800DF56C;

        setPolyGT4(poly);
        setSemiTrans(poly, 1);

        addPrim(ot0 - 25, poly);
        poly++;
    }

    GsOUT_PACKET_P = (PACKET*)poly;
}

void func_800D2CC8(void) // 0x800D2CC8
{
    s32         i;
    s_800E0900* ptr;

    ptr = D_800E0900;

    for (i = 1; i >= 0; i--)
    {
        ptr->field_0 = 0;
        ptr++;
    }
}

void func_800D2CEC(void) // 0x800D2CEC
{
    s32         i;
    s_800E0440* ptr;

    ptr = D_800E0440;

    for (i = 2; i >= 0; i--)
    {
        ptr->field_0 = 0;
        ptr++;
    }

    func_800D2CC8();
}

void func_800D2D28(GsOT_TAG* arg0)
{
    MATRIX      worldMat;
    SVECTOR     sp30;
    s32         temp_s4;
    s32         var_a0;
    s32         i;
    s_800E0900* ptr;

    ptr = D_800E0900;
    vwGetViewAngle(&sp30);
    temp_s4 = sp30.vy + Q12(0.5f);

    for (i = 0; i < 2; i++, ptr++)
    {
        if (ptr->field_0 > 0)
        {
            ptr->field_10 += Q12_MULT_PRECISE(g_DeltaTime, Q12(0.7f));
            ptr->field_10  = MIN(Q12(1.0f), ptr->field_10);

            if (ptr->field_0 > Q12(2.7f))
            {
                var_a0 = ptr->field_C + Q12_MULT_PRECISE(g_DeltaTime, Q12(0.5f));
                if (var_a0 > Q12(1.0f))
                {
                    ptr->field_C = Q12(1.0f);
                }
                else
                {
                    ptr->field_C = var_a0;
                }
            }
            else
            {
                ptr->field_C -= Q12_MULT_PRECISE(g_DeltaTime, Q12(0.4f));
                ptr->field_C  = MAX(ptr->field_C, 0);
            }

            func_800D0D6C(&worldMat, &ptr->field_4, temp_s4);
            func_800D0CA0(temp_s4, &ptr->field_4);
            func_800D2790(arg0, &worldMat, ptr->field_C, ptr->field_10);
            ptr->field_0 -= g_DeltaTime;
        }
    }
}

void func_800D2ED0(GsOT_TAG* arg0) // 0x800D2ED0
{
    MATRIX      worldMat;
    s32         i;
    s_800E0440* ptr;

    ptr = D_800E0440;

    for (i = 0; i < 3; i++, ptr++)
    {
        if (ptr->field_0 > 0)
        {
            ptr->field_10 += Q12_MULT_PRECISE(g_DeltaTime, Q12(0.5f));
            ptr->field_10  = MIN(Q12(1.0f), ptr->field_10);

            if (ptr->field_0 > Q12(2.7f))
            {
                ptr->field_C = Q12(1.0f);
            }
            else
            {
                ptr->field_C -= Q12_MULT_PRECISE(g_DeltaTime, Q12(0.4f));
                ptr->field_C  = MAX(ptr->field_C, 0);
            }

            func_800D0D6C(&worldMat, &ptr->field_4, 0);
            func_800D0CA0(0, &ptr->field_4);
            func_800D21AC(arg0, &worldMat, ptr->field_C, ptr->field_10);

            ptr->field_0 -= g_DeltaTime;
        }
    }

    func_800D2D28(arg0);
}

void Twinfeeler_TextureLoad(void) // 0x800D3038
{
    Fs_QueueStartReadTim(FILE_TEST_WARMTEST_TIM, FS_BUFFER_1, &D_800A9094);
}

void Twinfeeler_Translate(VECTOR3* newPos, const VECTOR3* pos, q19_12 headingAngle, q19_12 dist) // 0x800D3068
{
    newPos->vx = pos->vx + Q12_MULT_PRECISE(Math_Sin(headingAngle), dist);
    newPos->vy = pos->vy;
    newPos->vz = pos->vz + Q12_MULT_PRECISE(Math_Cos(headingAngle), dist);
}

void func_800D3114(void) // 0x800D3114
{
    VECTOR3     newPos;  // Q19.12
    s_800E0930* ptr;

    ptr = D_800E0930;

    switch (ptr->stateStep)
    {
        case 0:
            Twinfeeler_Translate(&newPos, &ptr->position, ptr->chara->rotation.vy, Q12(1.25f));
            func_800D19CC(&newPos);
            func_800D1AFC();
            func_800D26FC(&newPos, ptr->chara->rotation.vy);

            ptr->stateStep++;
            break;

        case 1:
            if (ptr->timer >= Q12(0.8f))
            {
                func_800D1AFC();

                ptr->stateStep++;
            }
            break;

        case 2:
            if (ptr->timer >= Q12(1.0f))
            {
                func_800D1C48();

                ptr->stateStep++;
            }
            break;

        case 3:
            if (ptr->timer >= Q12(1.5f))
            {
                func_800D1AFC();
                ptr->funcptr_18 = NULL;
            }

            if (Rng_Rand16() & 0x80)
            {
                func_800D1C48();
            }
            break;
    }

    ptr->timer += g_DeltaTime;
}

INCLUDE_RODATA("maps/map4_s03/nonmatchings/map4_s03", D_800CA788);

void func_800D326C(void) // 0x800D326C
{
    VECTOR3     newPos;  // Q19.12
    s_800E0930* ptr;

    ptr = D_800E0930;

    switch (ptr->stateStep)
    {
        case 0:
            Twinfeeler_Translate(&newPos, &ptr->position, ptr->chara->rotation.vy, Q12(1.3f));
            func_800D19CC(&newPos);

            ptr->stateStep++;
            break;

        case 1:
            if (ptr->timer >= Q12(0.3f))
            {
                ptr->stateStep++;
            }

            func_800D1C48();
            break;

        case 2:
            if (ptr->timer >= Q12(0.35f))
            {
                Twinfeeler_Translate(&newPos, &ptr->position, ptr->chara->rotation.vy, Q12(1.15f));
                func_800D26FC(&newPos, ptr->chara->rotation.vy);
                func_800D1AFC();

                ptr->stateStep++;
            }
            break;

        case 3:
            if (ptr->timer >= Q12(0.9f))
            {
                func_800D1C48();

                ptr->stateStep++;
            }
            break;

        case 4:
            if (ptr->timer >= Q12(1.3f))
            {
                func_800D1AFC();
                ptr->funcptr_18 = NULL;
            }
            break;
    }

    ptr->timer += g_DeltaTime;
}

void func_800D33D0(void) // 0x800D33D0
{
    s32         i;
    s_800E0930* ptr;

    ptr = D_800E0930;

    for (i = 0; i < 3; i++, ptr++)
    {
        if (ptr->funcptr_18 != NULL)
        {
            ptr->funcptr_18(ptr);
        }
    }
}

void func_800D3428(void) // 0x800D3428
{
    s32         i;
    s_800E0930* ptr;

    ptr = D_800E0930;

    for (i = 2; i >= 0; i--)
    {
        ptr->funcptr_18 = NULL;
        ptr++;
    }
}

s_800E0930* func_800D344C(s_SubCharacter* twinfeeler, void (*funcptr)(s_800E0930*)) // 0x800D344C
{
    s_CollisionSurface surface;
    s32                i;
    s_800E0930*        curPtr;

    curPtr = D_800E0930;
    for (i = 0; i < ARRAY_SIZE(D_800E0930); i++, curPtr++)
    {
        if (curPtr->funcptr_18 == NULL)
        {
            Collision_SurfaceGet(&surface, twinfeeler->position.vx, twinfeeler->position.vz);

            curPtr->chara       = twinfeeler;
            curPtr->position.vx = twinfeeler->position.vx;
            curPtr->position.vy = surface.groundHeight;
            curPtr->position.vz = twinfeeler->position.vz;
            curPtr->funcptr_18  = funcptr;
            curPtr->timer       = Q12(0.0f);
            curPtr->stateStep   = 0;
            return curPtr;
        }
    }

    return NULL;
}

void func_800D3504(s_SubCharacter* twinfeeler) // 0x800D3504
{
    func_800D344C(twinfeeler, &func_800D326C);
}

void func_800D3528(s_SubCharacter* twinfeeler) // 0x800D3528
{
    func_800D344C(twinfeeler, &func_800D3114);
}

void func_800D354C(VECTOR3* arg0) // 0x800D354C
{
    s32         var_a0;
    s32         var_v1;
    s_800E0988* ptr;

    ptr = D_800E0988;

    var_v1 = arg0->vx;
    if (var_v1 < 0)
    {
        var_v1 += 0x3FFFF;
    }
    var_v1 = (var_v1 >> 18) << 18;

    var_a0 = arg0->vz;
    if (var_a0 < 0)
    {
        var_a0 += 0x3FFFF;
    }

    ptr->x_0 = var_v1;
    ptr->z_4 = (var_a0 >> 18) << 18;

    func_800D078C();
    func_800D0840();
    func_800D17FC();
    func_800D2120();
    func_800D2CEC();
    func_800D3428();
}

void func_800D35DC(SVECTOR* rot) // 0x800D35DC
{
    GsOT_TAG*   ot;
    s_800E0988* ptr;
    s32         idx;

    ptr = D_800E0988;

    if (g_DeltaTime != Q12(0.0f))
    {
        func_800D33D0();
    }

    idx = g_ActiveBufferIdx;
    ot  = g_OrderingTable0[idx].org;

    Vw_WorldScreenMatrixAtPositionGet(&ptr->world_8, ptr->x_0, Q12(0.0f), ptr->z_4);
    func_800D0C50(rot, &ptr->world_8);
    func_800D1604(ot, rot->vy);
    func_800D1FF4(ot);
    func_800D2ED0(ot);
}

void func_800D3694(s_SubCharacter* twinfeeler, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords) // 0x800D3694
{
    typedef struct
    {
        MATRIX  field_0;
        SVECTOR field_20;
        VECTOR  field_28; // Q23.8
    } s_func_800D3694;

    enum _SfxId      sfxId;
    q19_12           scale;
    s_AnimInfo*      anim;
    s_func_800D3694* ptr;

    twinfeeler->position.vy = D_800DB1E0[twinfeeler->model.stateStep];

    if (twinfeeler->model.controlState == TwinfeelerControl_None)
    {
        twinfeeler->model.controlState = TwinfeelerControl_WalkForward;
        Chara_AnimSet(twinfeeler, ANIM_STATUS(TwinfeelerAnim_WalkForward, true), 163);
        twinfeeler->health           = 1;
        twinfeeler->model.anim.alpha = Q12(0.0f);
        twinfeeler->collision.state  = CharaCollisionState_Npc;
        twinfeeler->model.stateStep -= 18;
        twinfeeler->rotation.vx      = D_800DB1E8[twinfeeler->model.stateStep];
        twinfeeler->rotation.vy      = D_800DB1F0[twinfeeler->model.stateStep];
    }

    if (twinfeeler->model.stateStep == 1)
    {
        twinfeeler->timer_C6 = Q12(0.75f) - func_80055D78(twinfeeler->position.vx, twinfeeler->position.vy, twinfeeler->position.vz) * 16;
    }
    else
    {
        twinfeeler->timer_C6 = Q12(1.0f) - func_80055D78(twinfeeler->position.vx, twinfeeler->position.vy, twinfeeler->position.vz) * 16;
    }

    if (twinfeeler->timer_C6 < Q12(0.0f))
    {
        twinfeeler->timer_C6 = Q12(0.0f);
    }
    else if (twinfeeler->timer_C6 > Q12(1.0f))
    {
        twinfeeler->timer_C6 = Q12(1.0f);
    }

    if (twinfeeler->health > Q12(0.0f) && twinfeeler->damage.amount > Q12(0.0f))
    {
        twinfeeler->health            = Q12(0.0f);
        twinfeeler->model.anim.status = ANIM_STATUS(TwinfeelerAnim_DeathStart, false);
    }

    if (twinfeeler->health > Q12(0.0f) && twinfeelerProps.field_E8.val32 == Q12(0.0f))
    {
        if (Rng_GenerateUInt(0, 3)) // 1 in 4 chance.
        {
            sfxId = Sfx_Unk1553;
        }
        else
        {
            sfxId = Sfx_Unk1552;
        }

        Sfx_WithFlagsPlay(sfxId, &twinfeeler->position, Q8(0.5f), SfxFlag_None);
        twinfeelerProps.field_E8.val32 = Rng_GenerateInt(Q12(1.0f), Q12(1.8f) - 1);
    }
    else
    {
        twinfeelerProps.field_E8.val32 -= g_DeltaTime;
        if (twinfeelerProps.field_E8.val32 < Q12(0.0f))
        {
            twinfeelerProps.field_E8.val32 = Q12(0.0f);
        }
    }

    if (twinfeeler->model.anim.status == ANIM_STATUS(TwinfeelerAnim_StunIdleEnd, true))
    {
        if (twinfeeler->health == Q12(0.0f))
        {
            if (Chara_NpcIdxGet(twinfeeler) != g_SysWork.targetNpcIdx)
            {
                twinfeeler->health          = NO_VALUE;
                twinfeeler->collision.state = CharaCollisionState_Ignore;
                Savegame_EnemyStateUpdate(twinfeeler);
            }
        }
    }

    Math_MatrixTransform(&twinfeeler->position, (SVECTOR*)&twinfeeler->rotation, boneCoords);

    anim = &TWINFEELER_ANIM_INFOS[twinfeeler->model.anim.status];
    anim->playbackFunc(&twinfeeler->model, anmHdr, boneCoords, anim);

    scale = D_800DB1D8[twinfeeler->model.stateStep];
    Chara_ModelBoneScaleSet(boneCoords, 0, scale, scale, scale);

    twinfeeler->collision.box.height               = Q12(0.0f);
    twinfeeler->collision.box.bottom               = Q12(0.0f);
    twinfeeler->collision.shapeOffsets.cylinder.vz = Q12(0.0f);
    twinfeeler->collision.shapeOffsets.cylinder.vx = Q12(0.0f);
    twinfeeler->collision.cylinder.radius          = Q12(0.0f);

    ptr = PSX_SCRATCH;

    Vw_CoordHierarchyMatrixCompute(&boneCoords[11], &ptr->field_0);

    gte_SetRotMatrix(&ptr->field_0);
    gte_SetTransMatrix(&ptr->field_0);
    Math_SetSVectorFast(&ptr->field_20, 64, 89, 0);

    gte_ldv0(&ptr->field_20);
    gte_rt();
    gte_stlvnl(&ptr->field_28);

    twinfeeler->collision.box.top             = Q12_MULT_PRECISE(Q8_TO_Q12(ptr->field_28.vy), scale);
    twinfeeler->collision.box.offsetY         = twinfeeler->collision.box.top + Q12(0.03f);
    twinfeeler->collision.shapeOffsets.box.vx = Q12_MULT_PRECISE(twinfeeler->position.vx - Q8_TO_Q12(ptr->field_28.vx), scale);
    twinfeeler->collision.shapeOffsets.box.vz = Q12_MULT_PRECISE(twinfeeler->position.vz - Q8_TO_Q12(ptr->field_28.vz), scale);
    twinfeeler->collision.cylinder.field_2    = Q12(0.05f);
}

void func_800D3AE0(s_SubCharacter* chara, s32 soundIdx)
{
    Sfx_WithFlagsPlay(D_800DB1F8[soundIdx].id, &chara->position, D_800DB1F8[soundIdx].volume_2.val16, SfxFlag_None);
}

u32 func_800D3B1C(void) // 0x800D3B1C
{
    u8 sp10;

    Player_DisableDamage(&sp10, false);
    return sp10;
}

void func_800D3B44(bool disableDamage)
{
    u8 sp10;

    Player_DisableDamage(&sp10, disableDamage);
}

void func_800D3B68(s_SubCharacter* twinfeeler) // 0x800D3B68
{
    twinfeeler->health = twinfeeler->properties.twinfeeler.prevHealth;
}

void func_800D3B74(s_SubCharacter* twinfeeler) // 0x800D3B74
{
    if (twinfeeler->health >= Q12(0.0f))
    {
        twinfeeler->properties.twinfeeler.prevHealth = twinfeeler->health;
        twinfeeler->health                           = NO_VALUE;
    }
}

void Twinfeeler_GroundSet(s_SubCharacter* twinfeeler) // 0x800D3B98
{
    s_CollisionSurface surface;
    q3_12              headingAngle;
    q19_12             dist;
    q19_12             posX;
    q19_12             posZ;

    headingAngle = twinfeeler->rotation.vy;
    dist         = Q12_MULT_PRECISE(Math_Sin(headingAngle), Q12(1.0f));

    // Snap to ground.
    Collision_SurfaceGet(&surface, twinfeeler->position.vx + dist,
                         Q12_MULT_PRECISE(Math_Cos(headingAngle), Q12(1.0f)) + twinfeeler->position.vz);
    twinfeeler->position.vy = surface.groundHeight;

    // Fallback snap.
    if (twinfeeler->position.vy < Q12(-1.0f))
    {
        posX = twinfeeler->position.vx;
        posZ = twinfeeler->position.vz;
        Collision_SurfaceGet(&surface, posX, posZ);

        twinfeeler->position.vy = surface.groundHeight;
        if (twinfeeler->position.vy < Q12(-1.0f))
        {
            twinfeeler->position.vx = Q12(118.5f);
            twinfeeler->position.vz = Q12(137.9f);

            Collision_SurfaceGet(&surface, posX, posZ);
            twinfeeler->position.vy = surface.groundHeight;
        }
    }

    twinfeeler->properties.twinfeeler.flags &= ~TwinfeelerFlag_Underground;
}

void Twinfeeler_UndergroundSet(s_SubCharacter* twinfeeler) // 0x800D3CBC
{
    twinfeeler->position.vy = Q12(10.0f);
    twinfeelerProps.flags  |= TwinfeelerFlag_Underground;
}

bool Twinfeeler_Init(s_SubCharacter* twinfeeler) // 0x800D3CD4
{
    q19_12          posX;
    q19_12          posZ;
    s_SubCharacter* localTwinfeeler; // TODO: Not sure why this is needed here, could be an inline in this func.

    localTwinfeeler = twinfeeler;

    if (!Fs_QueueChunksLoad())
    {
        return false;
    }

    posX = twinfeeler->position.vx;
    posZ = twinfeeler->position.vz;

    twinfeeler->health = Q12(3000.0f);

    localTwinfeeler->properties.twinfeeler.prevHealth = NO_VALUE;

    twinfeeler->collision.cylinder.radius = Q12(0.3f);
    twinfeeler->model.anim.alpha          = Q12(0.0f);
    twinfeeler->moveSpeed                 = Q12(0.0f);
    twinfeeler->headingAngle              = twinfeeler->rotation.vy;
    twinfeeler->collision.state           = CharaCollisionState_4;

    localTwinfeeler->properties.twinfeeler.damage.position.vx = Q12(0.0f);
    localTwinfeeler->properties.twinfeeler.damage.position.vy = Q12(0.0f);
    localTwinfeeler->properties.twinfeeler.damage.position.vz = Q12(0.0f);
    localTwinfeeler->properties.twinfeeler.accumulatedDamage  = Q12(0.0f);
    localTwinfeeler->properties.twinfeeler.field_110          = 0;
    localTwinfeeler->properties.twinfeeler.flags              = TwinfeelerFlag_None;
    localTwinfeeler->properties.twinfeeler.spawnPositionX     = posX;
    localTwinfeeler->properties.twinfeeler.spawnPositionZ     = posZ;

    Twinfeeler_UndergroundSet(twinfeeler);

    twinfeeler->collision.shapeOffsets.cylinder.vx = Q12(0.0f);
    twinfeeler->collision.shapeOffsets.cylinder.vz = Q12(0.0f);
    twinfeeler->flags                             |= CharaFlag_Hit;

    if (twinfeeler->model.stateStep != 0)
    {
        twinfeeler->model.controlState = twinfeeler->model.stateStep;
        twinfeeler->model.stateStep    = 0;
    }
    else
    {
        twinfeeler->model.controlState = TwinfeelerControl_Idle;
        twinfeeler->model.stateStep    = 0;
    }

    *(u16*)&localTwinfeeler->properties.twinfeeler.field_E8.val16[1] = NO_VALUE;

    func_800D3B74(twinfeeler);

    Chara_AnimSet(twinfeeler, ANIM_STATUS(TwinfeelerAnim_InchForward, false), 258);
    ModelAnim_AnimInfoSet(&twinfeeler->model.anim, TWINFEELER_ANIM_INFOS);

    Chara_DamageClear(twinfeeler);

    func_800D354C(&twinfeeler->position);
    twinfeeler->flags |= CharaFlag_NoRadioStatic;
    return true;
}

void func_800D3E18(s_SubCharacter* twinfeeler) // 0x800D3E18
{
    *(u16*)&twinfeeler->properties.twinfeeler.field_E8.val16[1] = NO_VALUE;

    Sd_SfxStop(Sfx_Unk1567);
    Sd_SfxStop(Sfx_Unk1559);
    func_800D354C(&twinfeeler->position);
}

void func_800D3E58(s_SubCharacter* twinfeeler) // 0x800D3E58
{
    s32 temp_v1;
    s32 var_a1;
    u8  temp_s1;

    if (twinfeeler->damage.amount > Q12(0.0f) && twinfeeler->health >= Q12(0.0f))
    {
        twinfeeler->properties.twinfeeler.damage             = twinfeeler->damage;
        twinfeeler->properties.twinfeeler.accumulatedDamage += twinfeeler->damage.amount;
        twinfeeler->health                                   = MAX(twinfeeler->health - twinfeeler->damage.amount, Q12(0.0f));

        temp_s1 = D_800DB190[ANIM_STATUS_IDX_GET(twinfeeler->model.anim.status)];

        if (twinfeeler->health > Q12(20.0f))
        {
            if (twinfeeler->properties.twinfeeler.accumulatedDamage > Q12(0.0f) &&
                twinfeeler->model.controlState != TwinfeelerControl_2 && !(temp_s1 & 0x1))
            {
                twinfeeler->properties.twinfeeler.accumulatedDamage = Q12(0.0f);
                twinfeeler->model.controlState                      = TwinfeelerControl_2;
                twinfeeler->model.stateStep                         = 0;
            }
        }
        else if (func_800D3B1C() == 0 && twinfeeler->model.controlState != TwinfeelerControl_9)
        {
            if (!(temp_s1 & (1 << 1)))
            {
                twinfeeler->properties.twinfeeler.field_118 = true;
            }
            else
            {
                twinfeeler->properties.twinfeeler.field_118 = false;
            }

            twinfeeler->health             = NO_VALUE;
            twinfeeler->model.controlState = TwinfeelerControl_9;
            twinfeeler->model.stateStep    = 0;
            twinfeeler->flags             |= CharaFlag_Unk2;
        }
    }

    Chara_DamageClear(twinfeeler);
}

void func_800D3FB0(s_SubCharacter* twinfeeler) // 0x800D3FB0
{
    if (twinfeeler->model.controlState >= TwinfeelerControl_2)
    {
        twinfeeler->model.controlState = TwinfeelerControl_12;
        twinfeeler->model.stateStep    = 0;
    }
    else
    {
        twinfeeler->model.stateStep = 12;
    }
}

void func_800D3FD8(s_SubCharacter* twinfeeler) // 0x800D3FD8
{
    if (twinfeeler->model.controlState >= TwinfeelerControl_2)
    {
        twinfeeler->model.controlState = TwinfeelerControl_13;
        twinfeeler->model.stateStep    = 0;
    }
    else
    {
        twinfeeler->model.stateStep = 13;
    }
}

void func_800D4000(s_SubCharacter* twinfeeler) // 0x800D4000
{
    if (twinfeeler->model.controlState >= TwinfeelerControl_2)
    {
        twinfeeler->model.controlState = TwinfeelerControl_3;
        twinfeeler->model.stateStep    = 0;
    }
    else
    {
        twinfeeler->model.stateStep = 3;
    }
}

void Twinfeeler_ControlIdle(s_SubCharacter* twinfeeler) // 0x800D4028
{
    if (twinfeeler->model.stateStep == 0)
    {
        func_800D3B74(twinfeeler);
        twinfeeler->moveSpeed = Q12(0.0f);
        twinfeeler->model.stateStep++;
    }

    Twinfeeler_UndergroundSet(twinfeeler);
}

void Twinfeeler_Control_12(s_SubCharacter* twinfeeler) // 0x800D4078
{
    s_SubCharacter* localTwinfeeler;

    localTwinfeeler = twinfeeler;

    if (twinfeeler->model.stateStep == 0)
    {
        twinfeeler->rotation.vy  = Q12_ANGLE(0.0f);
        twinfeeler->headingAngle = Q12_ANGLE(0.0f);
        twinfeeler->position.vx  = Q12(118.5f);
        twinfeeler->position.vz  = Q12(137.9f);

        func_800D3504(twinfeeler);

        twinfeeler->properties.twinfeeler.digTimer = Q12(0.7f);
        twinfeeler->model.stateStep++;
    }

    switch (twinfeeler->model.stateStep)
    {
        case 1:
            if (twinfeeler->properties.twinfeeler.digTimer < Q12(0.0f))
            {
                Twinfeeler_GroundSet(twinfeeler);

                twinfeeler->moveSpeed         = Q12(1.8f);
                twinfeeler->model.anim.status = ANIM_STATUS(TwinfeelerAnim_Resurface, false);

                func_800D3AE0(twinfeeler, 5);
                twinfeeler->model.stateStep++;
            }

        case 2:
            if (localTwinfeeler->model.anim.status == ANIM_STATUS(TwinfeelerAnim_Idle, false))
            {
                twinfeeler->properties.twinfeeler.digTimer = Q12(1.0f);
                localTwinfeeler->moveSpeed                 = Q12(1.2f);
                localTwinfeeler->model.anim.status         = ANIM_STATUS(TwinfeelerAnim_InchForward, false);

                func_800D3AE0(localTwinfeeler, 4);
                localTwinfeeler->model.stateStep++;
            }
            break;

        case 3:
            if (twinfeeler->properties.twinfeeler.digTimer < Q12(0.0f))
            {
                twinfeeler->moveSpeed = Q12(0.8877f);

                Sd_SfxStop(Sfx_Unk1567);
                func_800D3AE0(twinfeeler, 1);

                twinfeeler->model.anim.status = ANIM_STATUS(TwinfeelerAnim_Burrow, false);
                func_800D3528(twinfeeler);

                twinfeeler->model.stateStep++;
            }
            break;

        case 4:
            if (twinfeeler->model.anim.status == ANIM_STATUS(TwinfeelerAnim_Idle, false))
            {
                twinfeeler->model.controlState = TwinfeelerControl_Idle;
                twinfeeler->model.stateStep    = 0;
                Sd_SfxStop(Sfx_Unk1559);
            }
            break;
    }

    twinfeeler->properties.twinfeeler.digTimer -= g_DeltaTime;
}

void Twinfeeler_Control_13(s_SubCharacter* twinfeeler) // 0x800D4248
{
    if (twinfeeler->model.stateStep == 0)
    {
        twinfeeler->position.vx  = Q12(131.0f);
        twinfeeler->position.vz  = Q12(140.0f);
        twinfeeler->rotation.vy  = Q12_ANGLE(90.0f);
        twinfeeler->headingAngle = Q12_ANGLE(90.0f);

        func_800D3504(twinfeeler);
        twinfeeler->properties.twinfeeler.digTimer = Q12(0.7f);

        Savegame_EnemyStateUpdate(twinfeeler);
        twinfeeler->model.stateStep++;
    }

    switch (twinfeeler->model.stateStep)
    {
        case 1:
            if (twinfeeler->properties.twinfeeler.digTimer < Q12(0.0f))
            {
                Twinfeeler_GroundSet(twinfeeler);

                twinfeeler->model.anim.status = ANIM_STATUS(TwinfeelerAnim_23, false);
                twinfeeler->moveSpeed         = Q12(1.35f);

                func_800D3AE0(twinfeeler, 5);
                twinfeeler->model.stateStep++;
            }
            break;

        case 2:
            if (twinfeeler->model.anim.status == ANIM_STATUS(TwinfeelerAnim_Idle, false))
            {
                twinfeeler->moveSpeed = Q12(0.8877f);
                func_800D3AE0(twinfeeler, 1);

                twinfeeler->model.anim.status = 4;
                twinfeeler->model.stateStep++;
            }
            break;

        case 3:
            if (twinfeeler->model.anim.status == ANIM_STATUS(TwinfeelerAnim_Idle, false))
            {
                Sd_SfxStop(Sfx_Unk1559);
                twinfeeler->model.controlState = TwinfeelerControl_Idle;
                twinfeeler->model.stateStep    = 1;
            }
            break;
    }

    twinfeeler->properties.twinfeeler.digTimer -= g_DeltaTime;
}

void func_800D43AC(s_SubCharacter* twinfeeler, s32 arg1) // 0x800D43AC
{
    q19_12 angleDeltaToPlayer;
    s32    temp_v0_2;
    s32    var_s1;

    if (arg1 < 15)
    {
        var_s1 = 512;
    }
    else
    {
        var_s1 = 170;
    }

    angleDeltaToPlayer = Math_AngleNormalizeSigned(ratan2(g_SysWork.playerWork.player.position.vx - twinfeeler->position.vx,
                                                          g_SysWork.playerWork.player.position.vz - twinfeeler->position.vz) -
                                                   twinfeeler->rotation.vy);

    if ((u32)ABS(angleDeltaToPlayer) > 56) // TODO: Needs casting to `u32`.
    {
        temp_v0_2 = Q12_MULT_PRECISE(g_DeltaTime, var_s1);
        if (angleDeltaToPlayer > Q12_ANGLE(0.0f))
        {
            twinfeeler->rotation.vy += temp_v0_2;
        }
        else
        {
            twinfeeler->rotation.vy -= temp_v0_2;
        }
    }
}

q19_12 func_800D4488(VECTOR3* arg0, s32 arg1) // 0x800D4488
{
    q19_12 sinAngle;
    q19_12 angle;
    q19_12 temp_v1;
    s32    temp_s3;
    s32    temp_s4;
    s32    temp;
    s32    temp2;
    s32    temp3;
    s32    newVar;

    temp_s4 = (arg1 >> 8) & 0xFF;
    temp_s3 = arg1 & 0xFF;

    temp    = ((Rng_Rand16() << 8) >> 15) + Q12(7.125f);
    temp_v1 = temp_s4 << 9;

    newVar  = 16;
    temp_s4 = temp_v1 + temp;
    temp_s4 = temp_s4 * newVar;

    temp3   = temp_s3 << 9;
    temp2   = ((Rng_Rand16() << 8) >> 15) + Q12(8.5f);
    temp_s3 = temp3 + temp2;
    temp_s3 = temp_s3 * newVar;

    angle = ratan2(g_SysWork.playerWork.player.position.vx - temp_s4,
                   g_SysWork.playerWork.player.position.vz - temp_s3);

    sinAngle = Math_Sin(angle);
    temp_v1  = temp_s3 - Math_Cos(angle);

    arg0->vx = temp_s4 - sinAngle;
    arg0->vz = temp_v1;

    return angle;
}

q19_12 func_800D4558(q19_12 headingAngle, q19_12 fromX, q19_12 fromZ) // 0x800D4558
{
    s_RayTrace trace;
    VECTOR3    offset; // Q19.12
    VECTOR3    from;   // Q19.12

    from.vx = fromX;
    from.vy = Q12(-0.3f);
    from.vz = fromZ;

    offset.vx = Math_Sin(headingAngle);
    offset.vy = Q12(0.0f);
    offset.vz = Math_Cos(headingAngle);

    if (func_8006DC18(&trace, &from, &offset))
    {
        return trace.hitDistance;
    }
    else
    {
        return INT_MAX;
    }
}

void Twinfeeler_Control_5(s_SubCharacter* twinfeeler)
{
    s_SubCharacter* localTwinfeeler;

    localTwinfeeler = twinfeeler;

    if (twinfeeler->model.stateStep == 0)
    {
        twinfeeler->properties.twinfeeler.digTimer = Q12(0.7f);
        func_800D3504(twinfeeler);

        twinfeeler->model.stateStep++;
    }

    switch (twinfeeler->model.stateStep)
    {
        case 1:
            if (twinfeeler->properties.twinfeeler.digTimer < Q12(0.0f))
            {
                func_800D3B68(twinfeeler);
                Twinfeeler_GroundSet(twinfeeler);

                twinfeeler->moveSpeed         = Q12(1.8f);
                twinfeeler->model.anim.status = ANIM_STATUS(TwinfeelerAnim_Resurface, false);

                func_800D3AE0(twinfeeler, 5);
                twinfeeler->model.stateStep++;
            }
            break;

        case 2:
            func_800D43AC(twinfeeler, FP_FROM(twinfeeler->model.anim.time, Q12_SHIFT));
            break;
    }

    localTwinfeeler->properties.twinfeeler.digTimer -= g_DeltaTime;

    if (twinfeeler->model.anim.status == ANIM_STATUS(TwinfeelerAnim_Idle, false))
    {
        func_800D4558(twinfeeler->rotation.vy, twinfeeler->position.vx, twinfeeler->position.vz);

        twinfeeler->model.controlState = TwinfeelerControl_10;
        twinfeeler->model.stateStep    = 0;
    }
}

void Twinfeeler_AcidThrow(VECTOR3* from, VECTOR3* to) // 0x800D46E0
{
    sharedFunc_800CB0A4_4_s03(from, to);
}

void Twinfeeler_Control_6(s_SubCharacter* twinfeeler, GsCOORDINATE2* boneCoords) // 0x800D4700
{
    MATRIX          transformMat;
    VECTOR3         from;
    VECTOR3         to; // Q19.12
    s32             keyframeIdx;
    s_SubCharacter* localChara;

    localChara = twinfeeler;

    keyframeIdx = twinfeeler->model.stateStep == 0;
    if (keyframeIdx != Q12(0.0f))
    {
        twinfeeler->properties.twinfeeler.digTimer = Q12(0.7f);
        func_800D3504(twinfeeler);

        twinfeeler->model.stateStep++;
    }

    keyframeIdx = FP_FROM(twinfeeler->model.anim.time, Q12_SHIFT);

    switch (twinfeeler->model.stateStep)
    {
        case 1:
            if (twinfeeler->properties.twinfeeler.digTimer < Q12(0.0f))
            {
                func_800D3B68(twinfeeler);
                Twinfeeler_GroundSet(twinfeeler);

                twinfeeler->moveSpeed         = Q12(1.8f);
                twinfeeler->model.anim.status = ANIM_STATUS(TwinfeelerAnim_Resurface, false);

                func_800D3AE0(twinfeeler, 5);
                twinfeeler->model.stateStep++;
            }
            break;

        case 2:
            if (keyframeIdx > 5)
            {
                func_800D3AE0(twinfeeler, 3);

                // Throw acid projectile.
                Vw_CoordHierarchyMatrixCompute(&boneCoords[TwinfeelerBone_Head], &transformMat);
                from.vx = Q8_TO_Q12(transformMat.t[0]);
                from.vy = Q8_TO_Q12(transformMat.t[1]);
                from.vz = Q8_TO_Q12(transformMat.t[2]);
                to.vx   = g_SysWork.playerWork.player.position.vx;
                to.vy   = g_SysWork.playerWork.player.position.vy - Q12(1.0f);
                to.vz   = g_SysWork.playerWork.player.position.vz;
                Twinfeeler_AcidThrow(&from, &to);

                twinfeeler->model.stateStep++;
            }

        case 3:
            func_800D43AC(localChara, keyframeIdx);
            break;
    }

    if (twinfeeler->model.anim.status == ANIM_STATUS(TwinfeelerAnim_Idle, false))
    {
        twinfeeler->model.controlState = TwinfeelerControl_10;
        twinfeeler->model.stateStep    = 0;
    }

    localChara->properties.twinfeeler.digTimer -= g_DeltaTime;
}

bool func_800D48CC(s32 arg0, s32 arg1) // 0x800D48CC
{
    s32 temp_a0;
    s32 temp_a1;

    temp_a0 = arg0 - D_800DB210.field_0;
    temp_a1 = arg1 - D_800DB210.field_4;

    if (temp_a0 > 0 && temp_a0 < D_800DB210.field_8 &&
        temp_a1 > 0 && temp_a1 < D_800DB210.field_C)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool func_800D4924(VECTOR3* vec, q19_12 angle) // 0x800D4924
{
    q19_12 posX;
    q19_12 posZ;

    posX = vec->vx + Q12_MULT_FLOAT_PRECISE(Math_Sin(angle), 1.0f);
    posZ = vec->vz + Q12_MULT_FLOAT_PRECISE(Math_Cos(angle), 1.0f);
    return func_800D48CC(Q12_TO_Q8(posX), Q12_TO_Q8(posZ));
}

void Twinfeeler_Control_10(s_SubCharacter* twinfeeler) // 0x800D49C0
{
    if (twinfeeler->model.stateStep == 0)
    {
        twinfeeler->moveSpeed         = Q12(1.2f);
        twinfeeler->model.anim.status = ANIM_STATUS(TwinfeelerAnim_InchForward, false);

        func_800D3AE0(twinfeeler, 4);
        twinfeeler->model.stateStep++;
    }
    else if (func_800D4924(&twinfeeler->position, twinfeeler->rotation.vy))
    {
        twinfeeler->model.controlState = TwinfeelerControl_4;
        twinfeeler->model.stateStep    = 0;
    }
}

void Twinfeeler_ControlStand(s_SubCharacter* twinfeeler) // 0x800D4A34
{
    // Stand up.
    if (twinfeeler->model.stateStep == 0)
    {
        twinfeeler->model.anim.status = ANIM_STATUS(TwinfeelerAnim_IdleToStand, false);
        twinfeeler->moveSpeed         = Q12(0.0f);
        twinfeeler->model.stateStep++;
    }

    // Attack.
    if (twinfeeler->model.anim.status == ANIM_STATUS(TwinfeelerAnim_Idle, false))
    {
        twinfeeler->model.controlState = TwinfeelerControl_Attack;
        twinfeeler->model.stateStep    = 0;
    }
}

void Twinfeeler_ControlAttack(s_SubCharacter* chara, GsCOORDINATE2* boneCoords)
{
    MATRIX  transformMat;
    VECTOR3 headPos; // Q19.12

    if (chara->model.stateStep == 0)
    {
        func_800D3AE0(chara, 3);
        chara->model.anim.status = ANIM_STATUS(TwinfeelerAnim_StandAcidAttack, false);

        Vw_CoordHierarchyMatrixCompute(&boneCoords[TwinfeelerBone_Head], &transformMat);
        headPos.vx = Q8_TO_Q12(transformMat.t[0]);
        headPos.vy = Q8_TO_Q12(transformMat.t[1]);
        headPos.vz = Q8_TO_Q12(transformMat.t[2]);

        Twinfeeler_AcidThrow(&headPos, &g_SysWork.playerWork.player.position);

        chara->moveSpeed = Q12(0.0f);
        chara->model.stateStep++;
    }

    if (chara->model.anim.status == ANIM_STATUS(TwinfeelerAnim_Idle, false))
    {
        chara->model.controlState = TwinfeelerControl_10;
        chara->model.stateStep    = 0;
    }
}

void Twinfeeler_Control_4(s_SubCharacter* twinfeeler)
{
    s_SubCharacter* localTwinfeeler;

    localTwinfeeler = twinfeeler;

    if (twinfeeler->model.stateStep == 0)
    {
        func_800D3528(twinfeeler);
        twinfeeler->properties.twinfeeler.digTimer = Q12(0.0f);
        func_800D3AE0(twinfeeler, 1);

        twinfeeler->model.anim.status = ANIM_STATUS(TwinfeelerAnim_Burrow, false);
        twinfeeler->moveSpeed         = Q12(0.8877f);
        twinfeeler->model.stateStep++;
    }

    if (twinfeeler->model.stateStep == 1 && twinfeeler->properties.twinfeeler.digTimer >= Q12(1.0f))
    {
        func_800D3B74(twinfeeler);
        twinfeeler->model.stateStep++;
    }

    if (twinfeeler->model.anim.status == ANIM_STATUS(TwinfeelerAnim_Idle, false))
    {
        Sd_SfxStop(Sfx_Unk1559);

        twinfeeler->model.controlState = TwinfeelerControl_3;
        twinfeeler->model.stateStep    = 0;
    }

    localTwinfeeler->properties.twinfeeler.digTimer += g_DeltaTime;
}

bool func_800D4C0C(u32 row, s32 col) // 0x800D4C0C
{
    if (row < 9 && col >= 0 && col < 5)
    {
        if ((D_800DB1A8[(row * 5) + col] & 0x1) == 0)
        {
            return true;
        }
    }

    return false;
}

u16 func_800D4C5C(u8* arg0, s32 arg1, s32 arg2, s32 arg3) // 0x800D4C5C
{
    s32 temp_s1;
    s32 temp_s2;
    s32 i;
    u8  temp_a1;
    s32 temp;
    s32 temp2;
    s32 temp3;
    s32 ret;

    for (i = arg1 - 1; i > 0; i--)
    {
        temp       = (u32)((i + 1) * Rng_Rand16()) >> 16;
        temp_a1    = arg0[i];
        arg0[i]    = arg0[temp];
        arg0[temp] = temp_a1;
    }

    for (i = 0; i < arg1; i++)
    {
        temp = arg0[i];

        temp2 = (temp % 3) - 1;
        temp3 = (temp / 3) - 1;

        temp_s2 = arg2 + temp2;
        temp_s1 = arg3 + temp3;

        if (func_800D4C0C(temp_s2, temp_s1) != 0)
        {
            ret = ((temp_s1 & 0xFFFF) + (temp_s2 << 8));
            return ret;
        }
    }

    return 0xFFFF;
}

s32 func_800D4D80(s32 arg0, s32 arg1) // 0x800D4D80
{
    u8  sp10[16];
    s32 i;
    u8* ptr;

    ptr = &sp10;

    for (i = 0; i < 9; i++)
    {
        if (i != 4)
        {
            *ptr = i;
            ptr += 1;
        }
    }

    return func_800D4C5C(&sp10, 8, arg0, arg1);
}

u32 func_800D4DD8(void) // 0x800D4DD8
{
    s32 var_v0;

    var_v0 = (g_SysWork.playerWork.player.rotation.vy - 256) & 0xFFF;
    if (var_v0 < 0)
    {
        var_v0 += 0x1FF;
    }

    return var_v0 >> 9;
}

u16 func_800D4E00(s32 arg0, s32 arg1, s32 arg2) // 0x800D4E00
{
    u16 var_a0;

    var_a0 = func_800D4C5C(&D_800DB220[arg2], 3, arg0, arg1);
    if (var_a0 == 0xFFFF)
    {
        var_a0 = func_800D4D80(arg0, arg1);
    }

    return var_a0;
}

s32 func_800D4E78(s32 arg0, s32 arg1) // 0x800D4E78
{
    s32 temp_a3;
    s32 temp_s0;
    s32 temp_s2;
    s32 var_a2;
    s32 var_s0;
    s32 var_s2;
    s32 var_v0_2;
    s32 var_v1;

    temp_s0  = Q12_MULT_PRECISE(arg0, 0x2333);
    temp_s2  = Q12_MULT_PRECISE(Math_Sin(arg1), temp_s0);
    var_v0_2 = Q12_MULT_PRECISE(Math_Cos(arg1), temp_s0);

    var_s2  = var_v0_2 + g_SysWork.playerWork.player.position.vz;
    var_s0  = g_SysWork.playerWork.player.position.vx + temp_s2;
    temp_a3 = var_s0 >> 4;

    var_a2 = temp_a3 - 0x7100;
    if (var_a2 < 0)
    {
        var_a2 = temp_a3 - 0x6F01;
    }

    var_s0 = var_a2 >> 9;
    if (var_s0 < 0)
    {
        var_s0 = 0;
    }

    if (var_s0 < 9)
    {
        var_v1 = var_s0;
    }
    else
    {
        var_v1 = 8;
    }

    var_s0 = var_v1;

    var_s2 = ((var_s2 >> 4) - 0x8700) / 0x200;
    if (var_s2 < 0)
    {
        var_s2 = 0;
    }

    if (var_s2 < 5)
    {
        var_v1 = var_s2;
    }
    else
    {
        var_v1 = 4;
    }

    var_s2 = var_v1;
    return func_800D4E00(var_s0, var_s2, 7 - func_800D4DD8());
}

void Twinfeeler_Control_3(s_SubCharacter* twinfeeler) // 0x800D4FC0
{
    s16   temp_v0;
    q3_12 angleDeltaToPlayer;
    s16   var_v0_2;

    if (twinfeeler->model.stateStep == 0)
    {
        twinfeelerProps.digTimer = Q12(2.0f);
        twinfeeler->model.anim.status = ANIM_STATUS(TwinfeelerAnim_InchForward, false);
        twinfeeler->model.stateStep++;
    }

    Twinfeeler_UndergroundSet(twinfeeler);

    if (twinfeelerProps.digTimer < Q12(0.0f))
    {
        temp_v0 = func_800D4488(&twinfeeler->position,
                                func_800D4E78(g_SysWork.playerWork.player.moveSpeed, g_SysWork.playerWork.player.headingAngle) & 0xFFFF);

        twinfeeler->headingAngle = temp_v0;
        twinfeeler->rotation.vy  = temp_v0;

        angleDeltaToPlayer = Math_AngleNormalizeSigned((ratan2(g_SysWork.playerWork.player.position.vx - twinfeeler->position.vx,
                                                               g_SysWork.playerWork.player.position.vz - twinfeeler->position.vz) -
                                                        temp_v0));

        angleDeltaToPlayer = ABS(angleDeltaToPlayer);

        if (angleDeltaToPlayer >= Q12_ANGLE(45.0f))
        {
            twinfeeler->model.controlState = TwinfeelerControl_5;
        }
        else
        {
            twinfeeler->model.controlState = TwinfeelerControl_6;
        }

        twinfeeler->model.stateStep = 0;
    }

    twinfeelerProps.digTimer -= g_DeltaTime;
}

void Twinfeeler_Control_2(s_SubCharacter* twinfeeler) // 0x800D50D8
{
    if (twinfeeler->model.stateStep == 0)
    {
        twinfeeler->model.anim.status = ANIM_STATUS(TwinfeelerAnim_StandRecoil, false);
        twinfeeler->model.stateStep++;
    }

    if (twinfeeler->model.anim.status == ANIM_STATUS(TwinfeelerAnim_Idle, false))
    {
        twinfeeler->model.controlState = TwinfeelerControl_4;
        twinfeeler->model.stateStep    = 0;
        twinfeeler->moveSpeed          = Q12(0.0f);
    }
}

void Twinfeeler_Control_9(s_SubCharacter* twinfeeler) // 0x800D511C
{
    s32             temp_s1;
    s_SubCharacter* localTwinfeeler;

    localTwinfeeler = twinfeeler;

    if (twinfeeler->model.stateStep == 0)
    {
        Sd_SfxStop(Sfx_Unk1559);

        if (!twinfeeler->properties.twinfeeler.field_118)
        {
            twinfeeler->model.anim.status = ANIM_STATUS(TwinfeelerAnim_IdleToStand, false);
            twinfeeler->model.stateStep   = 1;
        }
        else
        {
            func_800D3AE0(twinfeeler, 0);
            twinfeeler->model.anim.status = ANIM_STATUS(TwinfeelerAnim_StunStart, false);
            twinfeeler->model.stateStep   = 2;
        }
    }

    switch (twinfeeler->model.stateStep)
    {
        case 1:
            if (twinfeeler->model.anim.status == ANIM_STATUS(TwinfeelerAnim_Idle, false))
            {
                func_800D3AE0(twinfeeler, 0);
                twinfeeler->model.anim.status = ANIM_STATUS(TwinfeelerAnim_StunStart, false);
                twinfeeler->model.stateStep++;
            }
            break;

        case 2:
            if (twinfeeler->model.anim.status == ANIM_STATUS(TwinfeelerAnim_StunEnd, false))
            {
                localTwinfeeler->properties.twinfeeler.digTimer = Q12(5.0f);
                twinfeeler->model.stateStep++;
            }
            break;

        case 3:
            if (localTwinfeeler->properties.twinfeeler.digTimer < Q12(0.0f))
            {
                twinfeeler->model.anim.status = ANIM_STATUS(TwinfeelerAnim_StunToIdle, false);
                twinfeeler->model.stateStep++;
            }
            break;

        case 4:
            if (twinfeeler->model.anim.status == ANIM_STATUS(TwinfeelerAnim_Idle, false))
            {
                twinfeeler->moveSpeed         = Q12(1.2f);
                twinfeeler->model.anim.status = ANIM_STATUS(TwinfeelerAnim_InchForward, false);
                twinfeeler->model.stateStep++;
            }
            break;

        case 5:
            temp_s1 = func_800D4558(twinfeeler->rotation.vy, twinfeeler->position.vx, twinfeeler->position.vz);
            if (func_800D4924(&twinfeeler->position, twinfeeler->rotation.vy) ||
                temp_s1 < Q12(2.0f))
            {
                func_800D3528(twinfeeler);
                func_800D3AE0(twinfeeler, 1);

                twinfeeler->model.anim.status = ANIM_STATUS(TwinfeelerAnim_Burrow, false);
                twinfeeler->moveSpeed         = Q12(0.8877f);
                twinfeeler->model.stateStep++;
            }
            break;

        // TODO: Never reached?
        case 6:
            if (twinfeeler->model.anim.status == ANIM_STATUS(TwinfeelerAnim_Idle, false))
            {
                Sd_SfxStop(Sfx_Unk1559);
                Savegame_EventFlagSet(EventFlag_326);

                twinfeeler->model.controlState = ANIM_STATUS(TwinfeelerAnimDeathThroes, true);
                twinfeeler->model.stateStep    = 0;
            }
            break;
    }

    if (twinfeeler->model.stateStep < 5)
    {
        Chara_MoveSpeedUpdate(twinfeeler, Q12(1.0f));
    }

    localTwinfeeler->properties.twinfeeler.digTimer -= g_DeltaTime;
}

void Twinfeeler_ControlUpdate(s_SubCharacter* twinfeeler, GsCOORDINATE2* boneCoords) // 0x800D53B0
{
    // Handle control state.
    switch (twinfeeler->model.controlState)
    {
        case TwinfeelerControl_Idle:
            Twinfeeler_ControlIdle(twinfeeler);
            break;

        case TwinfeelerControl_12:
            Twinfeeler_Control_12(twinfeeler);
            break;

        case TwinfeelerControl_13:
            Twinfeeler_Control_13(twinfeeler);
            break;

        case TwinfeelerControl_5:
            Twinfeeler_Control_5(twinfeeler);
            break;

        case TwinfeelerControl_6:
            Twinfeeler_Control_6(twinfeeler, boneCoords);
            break;

        case TwinfeelerControl_4:
            Twinfeeler_Control_4(twinfeeler);
            break;

        case TwinfeelerControl_3:
            Twinfeeler_Control_3(twinfeeler);
            break;

        case TwinfeelerControl_Stand:
            Twinfeeler_ControlStand(twinfeeler);
            break;

        case TwinfeelerControl_Attack:
            Twinfeeler_ControlAttack(twinfeeler, boneCoords);
            break;

        case TwinfeelerControl_2:
            Twinfeeler_Control_2(twinfeeler);
            break;

        case TwinfeelerControl_9:
            Twinfeeler_Control_9(twinfeeler);
            break;

        case TwinfeelerControl_10:
            Twinfeeler_Control_10(twinfeeler);
            break;
    }
}

void func_800D54B4(s_SubCharacter* twinfeeler, GsCOORDINATE2* boneCoords) // 0x800D54B4
{
    VECTOR3         pos;                                                  // Q19.12
    MATRIX          sp30;
    s32             var_s0;                                               // `e_EquippedWeaponId`
    s32             var_v0;
    q19_12          posX;
    q19_12          posY;
    q19_12          posZ;
    s_SubCharacter* localChara;

    localChara = twinfeeler;

    // Handle control state.
    switch (twinfeeler->model.controlState)
    {
        case 5:
        case 6:
        case 8:
            var_s0 = EquippedWeaponId_Unk61;
            var_v0 = 14;
            break;

        default:
        case 2:
        case 3:
        case 4:
        case 7:
        case 9:
        case 10:
            var_s0 = 0; // Skips code below.
            var_v0 = 0;
            break;
    }

    if (var_s0)
    {
        Vw_CoordHierarchyMatrixCompute(&boneCoords[var_v0], &sp30);

        posX = Q8_TO_Q12(sp30.t[1]);
        posY = Q8_TO_Q12(sp30.t[0]);
        posZ = Q8_TO_Q12(sp30.t[2]);

        twinfeeler->collision.box.field_8 = twinfeeler->position.vy - posX;

        pos.vx = posY;
        pos.vy = posX;
        pos.vz = posZ;

        if (func_8008A0E4(1, var_s0, twinfeeler, &pos, &g_SysWork.playerWork.player, twinfeeler->rotation.vy, Q12_ANGLE(90.0f)) != NO_VALUE)
        {
            localChara->properties.twinfeeler.flags |= TwinfeelerFlag_0;
        }
    }
}

void func_800D55C8(s_SubCharacter* twinfeeler) // 0x800D55C8
{
    s_CollisionResult collResult;
    VECTOR            damagePos; // Q19.12
    VECTOR            moveDir;   // Q19.12
    VECTOR            pos;       // @hack Unused but required for match.
    q19_12            moveSpeed;
    s32               newPosZ;
    q19_12            posComp;
    s32               newPosX;
    s32               newPosY;

    damagePos.vx = twinfeelerProps.damage.position.vx;
    damagePos.vz = twinfeelerProps.damage.position.vz;
    damagePos.vy = twinfeelerProps.damage.position.vy;

    moveSpeed                     = twinfeeler->moveSpeed;
    twinfeelerProps.prevMoveSpeed = moveSpeed;

    moveDir.vx = Math_Sin(twinfeeler->rotation.vy);
    moveDir.vz = Math_Cos(twinfeeler->rotation.vy);
    moveDir.vy = Q12(0.0f);

    newPosX = Q12_MULT_PRECISE(damagePos.vx, Q12(3.5f)) + Q12_MULT_PRECISE(moveSpeed, moveDir.vx);
    newPosY = Q12_MULT_PRECISE(damagePos.vy, Q12(3.5f));
    newPosZ = Q12_MULT_PRECISE(damagePos.vz, Q12(3.5f)) + Q12_MULT_PRECISE(moveSpeed, moveDir.vz);

    pos.vx = newPosX;
    pos.vy = newPosY;
    pos.vz = newPosZ;

    twinfeeler->moveSpeed    = SquareRoot12(Q12_MULT_PRECISE(newPosX, newPosX) + Q12_MULT_PRECISE(newPosZ, newPosZ));
    twinfeeler->headingAngle = ratan2(newPosX, newPosZ);
    twinfeeler->fallSpeed   += g_GravitySpeed;

    if (!(twinfeelerProps.flags & TwinfeelerFlag_Underground))
    {
        Chara_MovementUpdate0(twinfeeler, &collResult);
    }

    posComp                            = twinfeelerProps.damage.position.vx;
    twinfeeler->moveSpeed              = twinfeelerProps.prevMoveSpeed;
    twinfeelerProps.damage.position.vx = SquareRoot12(Q12_MULT_PRECISE(posComp, posComp) >> g_VBlanks);

    if (posComp <= Q12(0.0f))
    {
        twinfeelerProps.damage.position.vx = -twinfeelerProps.damage.position.vx;
    }

    posComp                            = twinfeelerProps.damage.position.vy;
    twinfeelerProps.damage.position.vy = SquareRoot12(Q12_MULT_PRECISE(posComp, posComp) >> g_VBlanks);

    if (posComp <= Q12(0.0f))
    {
        twinfeelerProps.damage.position.vy = -twinfeelerProps.damage.position.vy;
    }

    posComp                            = twinfeelerProps.damage.position.vz;
    twinfeelerProps.damage.position.vz = SquareRoot12(Q12_MULT_PRECISE(posComp, posComp) >> g_VBlanks);

    if (posComp <= Q12(0.0f))
    {
        twinfeelerProps.damage.position.vz = -twinfeelerProps.damage.position.vz;
    }

    twinfeeler->rotation.vy = Math_AngleNormalizeSigned(twinfeeler->rotation.vy);
}

void func_800D5888(s_SubCharacter* twinfeeler, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords) // 0x800D5888
{
    s_AnimInfo* animInfo;

    Math_MatrixTransform(&twinfeeler->position, &twinfeeler->rotation, boneCoords);

    if (twinfeeler->model.anim.status != ANIM_STATUS(TwinfeelerAnim_Still, false))
    {
        animInfo = &TWINFEELER_ANIM_INFOS[twinfeeler->model.anim.status];
        animInfo->playbackFunc(&twinfeeler->model, anmHdr, boneCoords, animInfo);
    }
}

void func_800D5904(s_SubCharacter* twinfeeler, GsCOORDINATE2* boneCoords) // 0x800D5904
{
    MATRIX transformMat;
    q19_12 unkPosY;
    q19_12 unkBasePosY;
    q19_12 posX;
    q19_12 posY;
    q19_12 posZ;

    if (twinfeeler->model.anim.status == ANIM_STATUS(TwinfeelerAnim_Burrow, true))
    {
        Vw_CoordHierarchyMatrixCompute(&boneCoords[TwinfeelerBone_6], &transformMat);
    }
    else
    {
        Vw_CoordHierarchyMatrixCompute(&boneCoords[TwinfeelerBone_Head], &transformMat);
    }

    posY = twinfeeler->position.vy;
    posX = Q8_TO_Q12(transformMat.t[0]) - twinfeeler->position.vx;
    posZ = Q8_TO_Q12(transformMat.t[2]) - twinfeeler->position.vz;

    unkBasePosY                   = Q8_TO_Q12(transformMat.t[1]) - posY;
    unkPosY                       = unkBasePosY - Q12(0.25f);
    twinfeeler->collision.box.top = unkPosY;
    if (unkPosY >= posY)
    {
        twinfeeler->collision.box.bottom = posY;
    }
    else
    {
        twinfeeler->collision.box.bottom = unkPosY;
    }

    twinfeeler->collision.box.height       = unkBasePosY + Q12(0.25f);
    twinfeeler->collision.cylinder.radius  = Q12(0.5f);
    twinfeeler->collision.box.offsetY      = unkBasePosY;
    twinfeeler->collision.cylinder.field_2 = Q12(0.4f);
    sharedFunc_800CD920_3_s03(twinfeeler, posX, posZ);

    twinfeeler->collision.shapeOffsets.box.vx = twinfeeler->collision.shapeOffsets.cylinder.vx;
    twinfeeler->collision.shapeOffsets.box.vz = twinfeeler->collision.shapeOffsets.cylinder.vz;
}

void func_800D59C0(s_BoneNode* boneNodes) // 0x800D59C0
{
    s32 i;

    for (i = 0; i < BONE_NODE_COUNT_MAX; i++)
    {
        boneNodes[i].bone.modelInfo.field_0 &= ~(1 << 0);
    }
}

void func_800D59EC(s_SubCharacter* twinfeeler, GsCOORDINATE2* boneCoords) // 0x800D59EC
{
    u32         temp_v1;
    s32         temp_v1_3;
    s32         j;
    s32         i;
    s32         idx;
    u32         temp_a0_3;
    s8*         ptr;
    s_BoneNode* boneNodes;
    s_Bone*     bone;

    boneNodes = WorldGfx_CharaModelBoneNodesGet(Chara_Twinfeeler);
    func_800D59C0(boneNodes);

    temp_v1 = FP_FROM(twinfeeler->model.anim.time, Q12_SHIFT);

    if (temp_v1 < 32)
    {
        temp_v1_3 = D_800DB27C[temp_v1];

        for (i = temp_v1_3; i < 11; i++)
        {
            ptr = D_800DB238[i];

            for (j = 0; j < 6; j++, ptr++)
            {
                idx = *ptr;

                if (idx == NO_VALUE)
                {
                    break;
                }

                bone                     = &boneNodes[idx - 1].bone;
                bone->modelInfo.field_0 |= 1 << 0;
            }
        }
    }
    else
    {
        temp_a0_3 = temp_v1 - 32;
        if (temp_a0_3 < 41)
        {
            temp_v1_3 = D_800DB2FC[temp_a0_3];

            for (i = 0; i < temp_v1_3; i++)
            {
                ptr = D_800DB238[i];

                for (j = 0; j < 6; j++, ptr++)
                {
                    idx = *ptr;

                    if (idx == NO_VALUE)
                    {
                        break;
                    }

                    bone                     = &boneNodes[idx - 1].bone;
                    bone->modelInfo.field_0 |= 1 << 0;
                }
            }
        }
    }
}

void func_800D5B6C(s_SubCharacter* twinfeeler, GsCOORDINATE2* boneCoords) // 0x800D5B6C
{
    q19_12 posY;

    if (twinfeeler->model.anim.flags & AnimFlag_Visible)
    {
        func_800D5904(twinfeeler, boneCoords);
        return;
    }

    posY                              = twinfeeler->position.vy;
    twinfeeler->collision.box.bottom  = posY;
    twinfeeler->collision.box.height  = posY;
    twinfeeler->collision.box.top     = posY - Q12(1.0f);
    twinfeeler->collision.box.offsetY = posY - Q12(0.5f);
}

void func_800D5BC8(s_SubCharacter* twinfeeler, GsCOORDINATE2* boneCoords) // 0x800D5BC8
{
    if (twinfeeler->model.stateStep == 0)
    {
        Twinfeeler_ControlUpdate(twinfeeler, boneCoords);
    }
}

bool func_800D5BF8(s32 bitIdx, s32 arg1, s32 arg2, u16* arg3) // 0x800D5BF8
{
    bool prevBit;
    bool newBit;
    bool bitChanged;

    prevBit    = (*arg3 >> bitIdx) & 0x1;
    newBit     = (arg2 < arg1);
    bitChanged = prevBit ^ newBit;

    if (newBit)
    {
        *arg3 |= 1 << bitIdx;
    }
    else
    {
        *arg3 &= ~(1 << bitIdx);
    }

    return bitChanged;
}

void func_800D5C3C(s_SubCharacter* twinfeeler, GsCOORDINATE2* boneCoords) // 0x800D5C3C
{
    s32             temp_s1;
    s32             undergroundFlag;
    s32             var_a1;
    s32             i;
    s_SubCharacter* localChara;

    u8 D_800CA814[16] = {
        0x1E, 0x20, 0x1C, 0x1A,
        0x18, 0x16, 0x24, 0x22,
        0x28, 0x26, 0x2C, 0x2A,
        0x30, 0x2E, 0x34, 0x32
    };

    s32 D_800CA824[16] = {
        0xFFFFFFE7, 0xFFFFFFE7, 0xFFFFFFE2, 0xFFFFFFE2,
        0xFFFFFFD8, 0xFFFFFFD8, 0xFFFFFFBE, 0xFFFFFFBE,
        0xFFFFFFE2, 0xFFFFFFE2, 0xFFFFFFEC, 0xFFFFFFEC,
        0xFFFFFFE9, 0xFFFFFFE9, 0xFFFFFFE4, 0xFFFFFFE4
    };

    MATRIX transformMat;
    VECTOR pos;

    localChara = twinfeeler;

    // Weird `TwinfeelerFlag_Underground` check.
    undergroundFlag  = localChara->properties.twinfeeler.flags >> 2;
    undergroundFlag ^= 1;
    undergroundFlag &= 1;

    Text_Debug_PositionSet(170, 20);

    for (i = 0; i < 16; i++)
    {
        Vw_CoordHierarchyMatrixCompute(&boneCoords[D_800CA814[i]], &transformMat);

        if (func_800D5BF8(i, transformMat.t[1], D_800CA824[i], &localChara->properties.twinfeeler.field_E8.val16[1]))
        {
            pos.vx = Q8_TO_Q12(transformMat.t[0]);
            pos.vy = Q12(0.0f);
            pos.vz = Q8_TO_Q12(transformMat.t[2]);

            if (i < 0)
            {
                var_a1 = i + 3;
            }
            else
            {
                var_a1 = i;
            }

            temp_s1 = (var_a1 >> 2) + (i & 0x1);

            func_800D185C(&pos, temp_s1);

            if (func_800D48CC(transformMat.t[0], transformMat.t[2]) && undergroundFlag != 0)
            {
                func_800D2150(&pos, temp_s1);
            }
        }
    }
}

void func_800D5DF4(s_SubCharacter* twinfeeler, GsCOORDINATE2* boneCoords) // 0x800D5DF4
{
    if (g_DeltaTime != Q12(0.0f))
    {
        func_800D5C3C(twinfeeler, boneCoords);
    }

    func_800D35DC(&twinfeeler->rotation);
}

void func_800D5E30(s_SubCharacter* twinfeeler, GsCOORDINATE2* boneCoords) // 0x800D5E30
{
    MATRIX           transformMat;
    s32              i;
    q19_12           posY;
    q19_12           newPosX;
    q19_12           newPosY;
    q19_12           newPosZ;
    s_func_8006CF18* ptrs;

    ptrs = &D_800E04A0;

    twinfeeler->collision.field_E4   = &D_800E04A0;
    twinfeeler->collision.field_E1_4 = 4;

    for (i = 0; i < 4; i++, ptrs++)
    {
        Vw_CoordHierarchyMatrixCompute(&boneCoords[D_800DB3A0[i]], &transformMat);

        posY    = twinfeeler->position.vy;
        newPosX = Q8_TO_Q12(transformMat.t[0]);
        newPosY = Q8_TO_Q12(transformMat.t[1]);
        newPosZ = Q8_TO_Q12(transformMat.t[2]);

        ptrs->field_10       = Q12(0.45f);
        ptrs->field_E        = newPosY - Q12(0.3f);
        ptrs->field_C        = posY;
        ptrs->position.vx    = newPosX;
        ptrs->position.vy    = newPosY;
        ptrs->position.vz    = newPosZ;
        ptrs->collisionState = CharaCollisionState_Npc;
    }
}

void Twinfeeler_Update(s_SubCharacter* twinfeeler, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords) // 0x800D5F28
{
    if (twinfeeler->model.controlState == 0)
    {
        Twinfeeler_TextureLoad();
        twinfeeler->model.controlState = TwinfeelerControl_WalkForward;
        twinfeeler->model.stateStep    = 0;
    }

    if (twinfeeler->model.controlState != TwinfeelerControl_WalkForward || Twinfeeler_Init(twinfeeler))
    {
        if (g_DeltaTime != Q12(0.0f))
        {
            func_800D3E58(twinfeeler);
            Twinfeeler_ControlUpdate(twinfeeler, boneCoords);
            func_800D55C8(twinfeeler);
            func_800D5BC8(twinfeeler, boneCoords);
            func_800D5888(twinfeeler, anmHdr, boneCoords);
            func_800D5B6C(twinfeeler, boneCoords);
            func_800D54B4(twinfeeler, boneCoords);
            func_800D5DF4(twinfeeler, boneCoords);
            func_800D59EC(twinfeeler, boneCoords);
            func_800D5E30(twinfeeler, boneCoords);
            func_800D3B44(twinfeeler->flags & CharaFlag_Unk2);

            if (twinfeeler->model.controlState != TwinfeelerControl_10)
            {
                Sd_SfxStop(Sfx_Unk1567);
            }
        }
        else
        {
            func_800D5888(twinfeeler, anmHdr, boneCoords);
            func_800D5DF4(twinfeeler, boneCoords);
        }
    }
}
