#include "bodyprog/game.h"

#include <psyq/libetc.h>
#include <psyq/libpad.h>
#include <psyq/strings.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/sound/track_control.h"
#include "bodyprog/events/bgm_update.h"
#include "bodyprog/game_boot/game_boot.h"
#include "bodyprog/sound/sd_call.h"

/** @brief Task commands for `SD_Call` to load BGM KDT and VAB files. */
static u16 g_BgmTaskLoad[42] = {
    0,  0, 
    32, 33, 34, 35, 36, 37, 38, 39, 40, 41,
    42, 43, 44, 46, 47, 48, 49, 50, 51, 52,
    53, 54, 55, 56, 57, 58, 59, 60, 61, 62,
    64, 65, 66, 67, 68, 69, 45, 70, 71, 63
};

/** @brief Task commands for `SD_Call` to set current BGM channels to be used. */
static u16 g_BgmChannelSetTask[42] = {
    0, 0,
    SD_TASK_CHANNEL_SET(1),  SD_TASK_CHANNEL_SET(2),  SD_TASK_CHANNEL_SET(3),  SD_TASK_CHANNEL_SET(4),
    SD_TASK_CHANNEL_SET(5),  SD_TASK_CHANNEL_SET(6),  SD_TASK_CHANNEL_SET(7),  SD_TASK_CHANNEL_SET(8),
    SD_TASK_CHANNEL_SET(9),  SD_TASK_CHANNEL_SET(10), SD_TASK_CHANNEL_SET(11), SD_TASK_CHANNEL_SET(12),
    SD_TASK_CHANNEL_SET(13), SD_TASK_CHANNEL_SET(15), SD_TASK_CHANNEL_SET(16), SD_TASK_CHANNEL_SET(17),
    SD_TASK_CHANNEL_SET(18), SD_TASK_CHANNEL_SET(19), SD_TASK_CHANNEL_SET(20), SD_TASK_CHANNEL_SET(21),
    SD_TASK_CHANNEL_SET(22), SD_TASK_CHANNEL_SET(23), SD_TASK_CHANNEL_SET(24), SD_TASK_CHANNEL_SET(25),
    SD_TASK_CHANNEL_SET(26), SD_TASK_CHANNEL_SET(27), SD_TASK_CHANNEL_SET(28), SD_TASK_CHANNEL_SET(29),
    SD_TASK_CHANNEL_SET(30), SD_TASK_CHANNEL_SET(31), SD_TASK_CHANNEL_SET(33), SD_TASK_CHANNEL_SET(34),
    SD_TASK_CHANNEL_SET(35), SD_TASK_CHANNEL_SET(36), SD_TASK_CHANNEL_SET(37), SD_TASK_CHANNEL_SET(38),
    SD_TASK_CHANNEL_SET(14), SD_TASK_CHANNEL_SET(39), SD_TASK_CHANNEL_SET(40), SD_TASK_CHANNEL_SET(32)
};

/** @brief Task commands for `SD_Call` to load ambient VAB files. */
static u16 g_AmbienceTaskLoad[40] = {
    0,   162, 170, 171, 204, 172, 173, 174,
    175, 176, 177, 178, 179, 179, 179, 180,
    181, 182, 183, 184, 185, 186, 187, 188,
    189, 184, 190, 191, 192, 193, 194, 195,
    196, 197, 198, 199, 200, 201, 202, 203
};

s32 Sd_BgmControl(void) // 0x80035780
{
    if (Sd_AudioStreamingCheck() != AudioStreamingState_None ||
        Fs_QueueGetLength() > 0)
    {
        return NO_VALUE;
    }

    // Handle BGM state step.
    switch (g_GameWork.gameStateSteps[1])
    {
        case 0:
            Sd_BgmUpdate();
            g_GameWork.gameStateSteps[1]++;

        case 1:
            if (!Sd_ActiveBgmCheck(g_MapOverlayHdr.bgmCmd))
            {
                g_GameWork.gameStateSteps[1] += 2;
            }
            else
            {
                SD_Call(18);
                Bgm_LayerGlobalVariablesMute();

                g_GameWork.gameStateSteps[1]++;
            }
            break;

        case 2:
            // Checks if no BGM channel is being used.
            if (Sd_MidiChannelTaskGet() == 0)
            {
                Sd_BgmSet(g_MapOverlayHdr.bgmCmd);
                g_GameWork.gameStateSteps[1]++;
            }
            break;

        default:
            return 0;
    }

    return 1;
}

bool Sd_ActiveBgmCheck(s32 bgmIdx) // 0x800358A8
{
    if (bgmIdx == BgmCmd_UpdateLayers)
    {
        return false;
    }

    if (bgmIdx == BgmCmd_UpdateTrack)
    {
        return false;
    }

    return g_GameWork.bgmIdx != bgmIdx;
}

void Sd_BgmSet(s32 bgmIdx) // 0x800358DC
{
    if (bgmIdx == BgmCmd_UpdateLayers)
    {
        return;
    }

    if (bgmIdx == BgmCmd_UpdateTrack)
    {
        return;
    }

    g_GameWork.bgmIdx = bgmIdx;
    SD_Call(g_BgmTaskLoad[bgmIdx]);
}

void Sd_BgmChannelSet(void) // 0x80035924
{
    if (g_GameWork.bgmIdx == BgmCmd_UpdateLayers)
    {
        return;
    }

    if (g_GameWork.bgmIdx == BgmCmd_UpdateTrack)
    {
        return;
    }

    SD_Call(g_BgmChannelSetTask[g_GameWork.bgmIdx]);
}

void Sd_BgmUpdate(void)
{
    if (g_MapOverlayHdr.bgmCmd == BgmCmd_UpdateTrack)
    {
        Bgm_Update(true);
    }
}

s32 Sd_AmbienceControl(void) // 0x8003599C
{
    if (Sd_AudioStreamingCheck() != AudioStreamingState_None ||
        Fs_QueueGetLength() > 0)
    {
        return NO_VALUE;
    }

    // Handle ambience state step.
    switch (g_GameWork.gameStateSteps[1])
    {
        case 0:
            if (g_SavegamePtr->mapIdx == MapIdx_MAP2_S00)
            {
                if (Savegame_EventFlagGet(EventFlag_133) ||
                    Savegame_EventFlagGet(EventFlag_181))
                {
                    g_MapOverlayHdr.ambienceIdx = 11;
                }
                else
                {
                    g_MapOverlayHdr.ambienceIdx = 4;
                }
            }

            if (Sd_ActiveAmbienceCheck((s8)g_MapOverlayHdr.ambienceIdx))
            {
                SD_Call(17);
                g_GameWork.gameStateSteps[1]++;
                return 1;
            }
            break;

        case 1:
            Sd_AmbienceSet((s8)g_MapOverlayHdr.ambienceIdx);
            g_GameWork.gameStateSteps[1]++;
            return 1;

        default:
           break;
    }

    return 0;
}

bool Sd_ActiveAmbienceCheck(s32 ambienceIdx) // 0x80035AB0
{
    return g_GameWork.ambienceIdx != ambienceIdx;
}

void Sd_AmbienceSet(s32 ambienceIdx) // 0x80035AC8
{
    g_GameWork.ambienceIdx = ambienceIdx;
    SD_Call(g_AmbienceTaskLoad[ambienceIdx]);
}
