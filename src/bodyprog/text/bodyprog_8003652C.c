#include "game.h"

#include <psyq/libetc.h>
#include <psyq/libpad.h>
#include <psyq/strings.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/screen/screen_data.h"
#include "bodyprog/screen/screen_draw.h"
#include "bodyprog/text/map_msg.h"
#include "bodyprog/text/text_draw.h"

void func_8003652C(void) // 0x8003652C
{
#if VERSION_REGION_IS(PAL) // TODO: text_draw.c also makes use of same define here, merge somehow?
    #define SPRITE_CX 816
#else
    #define SPRITE_CX 304
#endif

    RECT rect;

    const u32 VALS[] = {
        0xFFFF0000, 0xBBEEE318, 0xFFEC9304, 0x83FFE30C,
        0x001F8318, 0x90840018, 0x90808080, 0x80048084
    };

    rect.x = SPRITE_CX;
    rect.y = 510;
    rect.w = 16;
    rect.h = 1;

    LoadImage(&rect, VALS);

    #undef SPRITE_CX
}

static const s32 __pad = 0;
