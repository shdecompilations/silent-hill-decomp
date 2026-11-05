#include "game.h"
#include "inline_no_dmpsx.h"

#include <psyq/gtemac.h>
#include <psyq/libapi.h>
#include <psyq/strings.h>

#include "bodyprog/bodyprog.h"
#include "main/fsqueue.h"

const RECT D_80028A20 = { SCREEN_WIDTH, 256, 192, SCREEN_HEIGHT };

void func_80066D90(void) // 0x80066D90
{
    s32       i;
    DR_TPAGE* tPage; // TODO: Guessed type.
    TILE*     tile;  // TODO: Guessed type.

    for (i = 0; i < 63; i++)
    {
        tPage = PSX_SCRATCH;
        setDrawTPage(tPage, 0, 1, getTPageN(0, 2, 0, 0));
        DrawPrim(tPage);

        tile = PSX_SCRATCH;
        setlen(tile, 3);

        setRGBC0(tile, 8, 8, 8, PRIM_RECT | RECT_BLEND); // `setTile(); setSemiTrans();`
        setXY0Fast(tile, -(SCREEN_WIDTH / 2), -FRAMEBUFFER_HEIGHT_PROGRESSIVE);
        setWHFast(tile, SCREEN_WIDTH, FRAMEBUFFER_HEIGHT_PROGRESSIVE * 2);
        DrawPrim(tile);

        Fs_QueueUpdate();
        VSync(SyncMode_Wait);
    }

    Fs_QueueWaitForEmpty();
}

void func_80066E40(void) // 0x80066E40
{
    DrawSync(SyncMode_Wait);
    StoreImage(&D_80028A20, FS_BUFFER_3);
    DrawSync(SyncMode_Wait);
}

void func_80066E7C(void) // 0x80066E7C
{
    LoadImage(&D_80028A20, FS_BUFFER_3);
    DrawSync(SyncMode_Wait);
}
