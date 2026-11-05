#include "game.h"

#include <psyq/libetc.h>
#include <psyq/libpad.h>
#include <psyq/strings.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/events/radio.h"
#include "bodyprog/sound/sound_system.h"

void Game_RadioNoiseReset(void) // 0x80037154
{
    s32 i;

    for (i = 0; i < ARRAY_SIZE(g_RadioNoise); i++)
    {
        g_RadioNoise[i].closeNpcInfoIdx = NO_VALUE;
        g_RadioNoise[i].idx             = NO_VALUE;
        g_RadioNoise[i].unused_3        = 0;
    }
}

void Game_RadioSoundStop(void) // 0x80037188
{
    s32 i;

    for (i = 0; i < ARRAY_SIZE(g_RadioNoise); i++)
    {
        g_RadioNoise[i].prevIdx = NO_VALUE;
    }

    // Stop `Sfx_RadioInterferenceLoop` and `Sfx_RadioStaticLoop`.
    for (i = 0; i < ARRAY_SIZE(g_RadioNoise); i++)
    {
        Sd_SfxStop(Sfx_RadioInterferenceLoop + i);
    }
}
