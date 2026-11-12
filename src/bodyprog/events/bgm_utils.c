#include "bodyprog/game.h"
#include "inline_no_dmpsx.h"

#include <psyq/libpad.h>
#include <psyq/strings.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/events/bgm_update.h"
#include "bodyprog/events/bgm_utils.h"
#include "bodyprog/items/item_screens.h"
#include "bodyprog/math/math.h"
#include "bodyprog/screen/draw.h"
#include "bodyprog/sound/sound_system.h"
#include "main/fsqueue.h"

// ========================================
// BGM RELATED
// ========================================

void Bgm_PlayNewSong(s32 bgmIdx) // 0x80087EA8
{
    if (Sd_BgmActiveSongCheck(bgmIdx) == false)
    {
        return;
    }

    Sd_BgmSongSet(bgmIdx);
}

void Bgm_CrossfadeToTrack(s32 bgmIdx) // 0x80087EDC
{
    if (Sd_AudioStreamingCheck() != AudioStreamingState_None || !Fs_QueueChunksLoad())
    {
        return;
    }

    switch (g_SysWork.sysStateSteps[1])
    {
        case 0:
            if (Sd_BgmActiveSongCheck(bgmIdx) == false)
            {
                SysWork_StateStepSet(1, 3);
                break;
            }

            g_SysWork.bgmStatusFlags |= BgmStatusFlag_RequestMute;
            SysWork_StateStepIncrement(1);
            break;

        case 1:
            g_SysWork.bgmStatusFlags |= BgmStatusFlag_RequestMute;
            SD_Call(23);

            SysWork_StateStepIncrement(1);
            break;

        case 2:
            g_SysWork.bgmStatusFlags |= BgmStatusFlag_RequestMute;

            if (Sd_MidiChannelTaskGet() == 0)
            {
                Sd_BgmSongSet(bgmIdx);
                SysWork_StateStepIncrement(1);
            }
            break;

        case 3:
            SysWork_StateStepIncrement(0); // Resets `field_10` to 0.
            break;
    }
}

void Bgm_CrossfadeToSilence(void) // 0x80088028
{
    Bgm_CrossfadeToTrack(BgmCmd_UpdateLayers);
}

void Bgm_SongStopImmediate(void) // 0x80088048
{
    if (Sd_AudioStreamingCheck() != AudioStreamingState_None)
    {
        return;
    }

    switch (g_SysWork.sysStateSteps[1])
    {
        case 0:
            Bgm_LayerGlobalVariablesMute();
            SD_Call(18);
            SysWork_StateStepIncrement(1);
            break;

        case 1:
            if (Sd_MidiChannelTaskGet() == 0)
            {
                SysWork_StateStepIncrement(0); // Resets `field_10` to 0.
            }
            break;

        default:
            break;
    }
}

void Bgm_SongStopFadeOut(bool useSlowFade) // 0x800880F0
{
    if (Sd_AudioStreamingCheck() != AudioStreamingState_None)
    {
        return;
    }

    switch (g_SysWork.sysStateSteps[1])
    {
        case 0:
            Bgm_LayerGlobalVariablesMute();

            if (!useSlowFade)
            {
                SD_Call(22);
            }
            else
            {
                SD_Call(23);
            }

            SysWork_StateStepIncrement(1);
            break;

        case 1:
            if (Sd_MidiChannelTaskGet() == 0)
            {
                SysWork_StateStepIncrement(0); // Resets `field_10` to 0.
            }
            break;
    }
}
