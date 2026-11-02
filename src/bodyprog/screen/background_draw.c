#include "game.h"

#include "main/fsqueue.h"
#include "bodyprog/screen/screen_data.h"
#include "bodyprog/screen/screen_draw.h"
#include "bodyprog/screen/background_draw.h"
#include "bodyprog/math/math.h"

// ========================================
// GLOBAL VARIABLES
// ========================================

q0_8 g_Screen_BackgroundImgGamma = Q8(0.5f);
s16  D_800A8E5A                  = 3; // Unused.

// ========================================
// 2D BACKGROUND IMAGE DRAWING
// ========================================

// PAL versions relocate this to bodyprog, as a bodyprog func (`MainMenu_BackgroundDraw`) was updated to also require it.
// Note: changes here should be copied over to b_konami version too.
#if VERSION_REGION_IS(PAL)
void BootScreen_ImageSegmentDraw(s_FsImageDesc* image, s32 otz, s32 vramX, s32 vramY, s32 w, s32 h, s32 x, s32 y) // 0x800C9E6C
{
    DR_TPAGE* tPage;
    SPRT*     prim     = (SPRT*)GsOUT_PACKET_P;
    u32       vramBase = image->tPage[1] + (u32)(vramX >> 8) + (((u32)(vramY >> 8)) << 4);
    u32*      addr     = &g_OtTags0[g_ActiveBufferIdx][otz];

    addPrimFast(addr, prim, 4);
    setCodeWord(prim, PRIM_RECT | RECT_TEXTURE, 0x808080);
    setWH(prim, w, h);

    vramX = vramX & 0xFF;
    vramY = vramY & 0xFF;

    //setUV0AndClut(prim, vramX, vramY, image->clutX, image->clutY);
    *(u32*)(&prim->u0) = vramX + (vramY << 8) + (((image->clutY << 6) | ((image->clutX >> 4) & 0x3F)) << 16);

    setXY0Fast(prim, (u16)x, y);

    tPage = (DR_TPAGE*)((u8*)prim + sizeof(SPRT));
    setDrawTPage(tPage, 0, 1, getTPage(image->tPage[0], 0, (vramBase << 6), (((vramBase >> 4) & (1 << 0)) << 8)));
    AddPrim(addr, tPage);

    GsOUT_PACKET_P = (u8*)tPage + sizeof(DR_TPAGE);
}
#endif

#if VERSION_IS(EUR)
INCLUDE_ASM("bodyprog/nonmatchings/screen/background_draw", Screen_BackgroundImgDraw);
#else
void Screen_BackgroundImgDraw(s_FsImageDesc* image)
{
    s32       baseYOffset;
    s32       tileX;
    s32       tileIdxX;
    s32       x;
    s32       y;
    u32       texShift;
    s32       texPageX;
    u32       texOffsetY;
    GsOT*     ot;
    DR_TPAGE* tPage;
    SPRT*     sprt;
    PACKET*   packet;

    ot          = (GsOT*)&g_OtTags1[g_ActiveBufferIdx + 1][0];
    packet      = GsOUT_PACKET_P;
    baseYOffset = -120 << (g_GameWork.gsScreenHeight >> 8);
    texShift    = image->tPage[0];
    texPageX    = image->tPage[1];
    texOffsetY  = image->v;

    for (y = 0; (g_GameWork.gsScreenHeight >> 8) >= y; y++)
    {
        for (x = 0; ((g_GameWork.gsScreenWidth - 1) >> (8 - texShift)) >= x; x++)
        {
            sprt = (SPRT*)packet;

            addPrimFast(ot, sprt, 4);
            setRGBC0(sprt,
                     g_Screen_BackgroundImgGamma, g_Screen_BackgroundImgGamma, g_Screen_BackgroundImgGamma,
                     PRIM_RECT | RECT_TEXTURE);

            if (y == 0)
            {
                setWH(sprt, 256, 256 - texOffsetY);
                setUV0AndClutSum(sprt, 0, texOffsetY, getClut(image->clutX, image->clutY));
            }
            else
            {
                setWH(sprt, 256, 256);
                setUV0AndClutSum(sprt, 0, 0, getClut(image->clutX, image->clutY));
            }

            tileX = x * 256;

            setXY0Fast(sprt,
                       (tileX - (g_GameWork.gsScreenWidth / 2)) - (image->u << (2 - texShift)),
                       baseYOffset + ((256 - texOffsetY) * y));

            packet += sizeof(SPRT);
            tPage   = (DR_TPAGE*)packet;

            tileIdxX = x << texShift;
            setDrawTPage(tPage, 0, 1, getTPage(texShift, 0, (texPageX + tileIdxX) * 64, ((texPageX * 16) & 0x100) + y * 256));

            AddPrim(ot, tPage);
            packet = (PACKET*)tPage + sizeof(DR_TPAGE);
        }
    }

    GsOUT_PACKET_P              = packet;
    g_SysWork.bgmStatusFlags   |= BgmStatusFlag_Pause;
    g_Screen_BackgroundImgGamma = Q8(0.5f);
}
#endif

void Screen_BackgroundImgTransition(s_FsImageDesc* imageIn, s_FsImageDesc* imageOut, q3_12 alpha)
{
    volatile s32   pad; // Hack
    s32            i;
    s32            j;
    u32            color;
    u8             tPageY;
    s32            offsetX;
    s_FsImageDesc* image;
    POLY_FT4*      poly;

    poly = (POLY_FT4*)GsOUT_PACKET_P;

    for (i = 0; i < 3; i++)
    {
        image = (i > 0) ? imageIn : imageOut;
        color = (i < 2) ? Q12_MULT_PRECISE(alpha, Q12(0.0313f)) : Q12(0.0313f);

        for (j = 0; j < 3; j++)
        {
            setPolyFT4(poly);

            setXY0Fast(poly, -160 + (128 * j), -120);
            setXY1Fast(poly, (128 * j) + ((j != 2) ? -32 : -96), -120);

            offsetX = 128 * j;

            setXY2Fast(poly, -160 + offsetX, 120);
            setXY3Fast(poly, offsetX + ((j != 2) ? -32 : -96), 120);
            setUV0AndClutSum(poly, 0, image->v, getClut(image->clutX, image->clutY));

            tPageY = image->tPage[1];

            setUV1AndTPageSum(poly, ((j != 2) ? 128 : 64), image->v,
                              getTPage(image->tPage[0], i + 1, (image->tPage[1] + j) * 64, (tPageY * 16) & 0x100));

            setUV2Sum(poly, 0,                     image->v + 239);
            setUV3Sum(poly, ((j != 2) ? 128 : 64), image->v + 239);

            setSemiTrans(poly, i < 2);

            setRGB0Fast(poly, color, color, color);

            addPrim(g_OrderingTable0[g_ActiveBufferIdx].org, poly);
            poly++;
        }
    }

    g_SysWork.bgmStatusFlags |= BgmStatusFlag_Pause;
    GsOUT_PACKET_P            = (PACKET*)poly;
}

void Screen_BackgroundImgDrawBlackFadeIn(s_FsImageDesc* image)
{
    volatile s32 __pad; // Hack.
    s32          i;
    s32          xOffset;
    u8           tPageY;
    POLY_FT4*    poly;

    poly = (POLY_FT4*)GsOUT_PACKET_P;

    for (i = 0; i < 3; i++)
    {
        setPolyFT4(poly);
        setXY0Fast(poly, -160 + (128 * i), -120);
        setXY1Fast(poly, (128 * i) + ((i == 2) ? -96 : -32), -120);

        xOffset = 128 * i;

        setXY2Fast(poly, -160 + xOffset, 120);
        setXY3Fast(poly, xOffset + (i == 2 ? -96 : -32), 120);
        setUV0AndClutSum(poly, 0, image->v, getClut(image->clutX, image->clutY));

        tPageY = image->tPage[1];
        
        setUV1AndTPageSum(poly, (i == 2) ? 64 : 128, image->v,
                          getTPage(image->tPage[0], 0, (image->tPage[1] + i) * 64, (tPageY * 16) & 0x100));
        
        setUV2Sum(poly, 0,                     image->v + 239);
        setUV3Sum(poly, ((i == 2) ? 64 : 128), image->v + 239);

        setSemiTrans(poly, false);

        setRGB0Fast(poly, g_Screen_BackgroundImgGamma, g_Screen_BackgroundImgGamma, g_Screen_BackgroundImgGamma);

        addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[2], poly);
        poly++;
    }

    GsOUT_PACKET_P              = (PACKET*)poly;
    g_SysWork.bgmStatusFlags   |= BgmStatusFlag_Pause;
    g_Screen_BackgroundImgGamma = Q8(0.5f);
}

bool Screen_BackgroundMotionBlur(s32 vBlanks)
{
    s32       interlacingEnabled;
    s32       i;
    s32       offsetY;
    s32       texOffsetY;
    s32       tPageOffsetY;
    s32       j;
    GsOT*     ot;
    SPRT*     sprt;
    DR_TPAGE* tPage;

    ot                 = (GsOT*)&g_OtTags1[g_ActiveBufferIdx + 1][0];
    sprt               = (SPRT*)GsOUT_PACKET_P;
    interlacingEnabled = GsDISPENV.isinter;

    for (i = 0; (i == 0 || (interlacingEnabled && i == 1)); i++)
    {
        for (j = 0; j < 3; j++)
        {
            if (interlacingEnabled)
            {
#if VERSION_REGION_IS(PAL)
                texOffsetY   = (i == 0) ? 1 : 0;
#else
                texOffsetY   = (i == 0) ? 32 : 0;
#endif
                offsetY      = (i == 0) ? -FRAMEBUFFER_HEIGHT_PROGRESSIVE : 0;
                tPageOffsetY = i * 256;
            }
            else
            {
#if VERSION_REGION_IS(PAL)
                texOffsetY   = 0;
                offsetY      = -127;
#else
                texOffsetY   = (g_ActiveBufferIdx == 0) * 32;
                offsetY      = -112;
#endif
                tPageOffsetY = g_ActiveBufferIdx * 256;
            }

            addPrimFast(ot, sprt, 4);

            if ((VSync(SyncMode_Count) % vBlanks) == 0)
            {
                setRGBC0(sprt, 127, 127, 127, PRIM_RECT | RECT_TEXTURE);
            }
            else
            {
                setRGBC0(sprt, 128, 128, 128, PRIM_RECT | RECT_TEXTURE);
            }

            setWH(sprt, 256, FRAMEBUFFER_HEIGHT_PROGRESSIVE);
            setUV0AndClutSum(sprt, 0, texOffsetY, 0);

            setXY0Fast(sprt, (-g_GameWork.gsScreenWidth / 2) + (j * 256), offsetY);

            sprt++;
            tPage = (DR_TPAGE*)sprt;

            setDrawTPage(tPage, 0, 1, getTPage(2, 0, (j * 256), tPageOffsetY));
            AddPrim(ot, tPage);

            sprt = (PACKET*)sprt + sizeof(DR_TPAGE);
        }
    }

    GsOUT_PACKET_P = sprt;
    return false;
}
