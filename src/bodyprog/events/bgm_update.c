#include "game.h"

#include <psyq/libetc.h>
#include <psyq/libpad.h>
#include <psyq/strings.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/sys/demo.h"
#include "bodyprog/events/event_data.h"
#include "bodyprog/events/bgm_update.h"
#include "bodyprog/math/math.h"
#include "bodyprog/screen/screen_data.h"
#include "bodyprog/screen/screen_draw.h"
#include "bodyprog/sound/sound_system.h"
#include "bodyprog/text/text_draw.h"
#include "main/fsqueue.h"

/** Unused, dead code. Pointed to by `D_800A999C`. */
extern const s8 D_80025234[];

// ========================================
// GLOBAL VARIABLES
// ========================================

/** Unused, dead code. Returned by `sharedFunc_800D929C_0_s00`. */
u32 D_800A999C = &D_80025234;

// ========================================
// STATIC VARIABLES
// ========================================

static s32 g_Bgm_LayersUpdated;
static s32 g_Bgm_ChannelSetProcessState = 0;
static u8  g_Bgm_ChannelLimits[8]       = { 128, 128, 128, 128, 128, 128, 128, 128 };

// ========================================
// MUSIC UPDATE
// ========================================

void Bgm_Update(bool updateSong) // 0x80035DB4
{
    g_Bgm_LayersUpdated = false;

    if (g_MapOverlayHdr.bgmEvent != NULL) // Checks if function exists.
    {
        g_MapOverlayHdr.bgmEvent(updateSong);
        if (updateSong == false && g_Bgm_LayersUpdated == false)
        {
            Bgm_LayersUpdate(BgmFlag_Layer1, Q12(240.0f), NULL);
        }
    }
}

void Bgm_LayerGlobalVariablesMute(void) // 0x80035E1C
{
    s32 i;

    // Mute all BGM layers.
    for (i = 0; i < ARRAY_SIZE(g_SysWork.bgmLayerVolumes); i++)
    {
        g_SysWork.bgmLayerVolumes[i] = Q12(0.0f);
    }
}

bool Bgm_MuteCheck(void)
{
    s32 i;
    u16 enabledChannelTask;

    for (i = 0; i < (ARRAY_SIZE(g_SysWork.bgmLayerVolumes) - 1); i++)
    {
        if (g_SysWork.bgmLayerVolumes[i] != Q12(0.0f))
        {
            return false;
        }
    }

    enabledChannelTask = Sd_MidiChannelTaskGet();

    // Idle.
    if (enabledChannelTask == 0)
    {
        return true;
    }
    // Stop/Disable.
    else if (enabledChannelTask == 0xFFFF)
    {
        return false;
    }

    for (i = 1; i < (ARRAY_SIZE(g_SysWork.bgmLayerVolumes) - 1); i++)
    {
        if (Sd_MidiChannelVolumeGet(i) != 0)
        {
            return false;
        }
    }

    return true;
}

/** @brief Updates global variables for music layer volumes. */
static void Bgm_LayerGlobalVariablesUpdate(void)
{
    s32 i;

    for (i = 1; i < (ARRAY_SIZE(g_SysWork.bgmLayerVolumes) - 1); i++)
    {
        g_SysWork.bgmLayerVolumes[i] = Sd_MidiChannelVolumeGet(i) << 5; // Conversion to Q12.
    }

    if (Sd_MidiChannelTaskGet() == 0)
    {
        g_SysWork.bgmLayerVolumes[0] = Q12(1.0f);
    }

    g_SysWork.bgmLayerVolumes[ARRAY_SIZE(g_SysWork.bgmLayerVolumes) - 1] = Q12(0.0f);
}

void Bgm_LayersUpdate(e_BgmStatusFlags bgmFlags, q19_12 fadeSpeed, s_BgmLayerLimits* layerLimits) // 0x80035F4C
{
    q3_12     firstSongLayerVol; // Value in range `[0.0f, 1.0f]` used to determine the first BGM layer's volume,
                                 // prompted by radio noise or opening a menu while in-game.
    q19_12    ducking;
    q19_12    targetVol;
    q19_12    curLayerVol;
    q19_12    curChannelVol;
    s32       activeSetChannelTask;
    s32       i;
    s32       flagsCpy;
    bool      isBgmChannelActive;
    bool      isMusicPlaying;
    q19_12    adjustLayerValue;
    bool      areChannelsActive;
    s32       lastLayerIdx;
    q3_12*    layerVols;
    u8*       channelLimitsCpy;
    static s8 bgmChannelVols[8];

    // Setup.
    flagsCpy         = bgmFlags;
    channelLimitsCpy = layerLimits;
    layerVols        = g_SysWork.bgmLayerVolumes;

    // Ensure layer limits are valid.
    if (channelLimitsCpy == NULL)
    {
        channelLimitsCpy = g_Bgm_ChannelLimits;
    }

    // Continue music at reduced volume if player is dead.
    if (g_SysWork.playerWork.player.health <= Q12(0.0f) || g_SysWork.sysState == SysState_GameOver)
    {
        flagsCpy &= BgmFlag_KeepAlive;
        flagsCpy |= BgmFlag_Layer1;
        fadeSpeed = Q12(0.2f);
    }

    // If player is not dead and the radio is active this set the
    // BGM status flag for radio active.
    if (!(flagsCpy & BgmFlag_KeepAlive) && g_RadioPitchState > 0 &&
        (g_SavegamePtr->itemToggleFlags & ItemToggleFlag_RadioOn))
    {
        g_SysWork.bgmStatusFlags |= BgmStatusFlag_RadioActive;
    }

    // Mute layers.
    if (g_SysWork.bgmStatusFlags & BgmStatusFlag_RequestMute)
    {
        flagsCpy                  = BgmFlag_Layer1 | BgmFlag_MuteAll;
        g_SysWork.bgmStatusFlags |= BgmStatusFlag_ApplyMute;
    }

    if (flagsCpy & BgmFlag_Layer1)
    {
        flagsCpy &= BgmFlag_KeepAlive | BgmFlag_MuteAll;
    }
    else
    {
        flagsCpy ^= BgmFlag_Layer1;
    }

    // Updates music layers volume.
    for (i = 0, lastLayerIdx = (ARRAY_SIZE(g_SysWork.bgmLayerVolumes) - 1);
         i < ARRAY_SIZE(g_SysWork.bgmLayerVolumes);
         i++)
    {
        curLayerVol = layerVols[i];

        if (i == lastLayerIdx)
        {
            adjustLayerValue = Q12_MULT_FLOAT_PRECISE(g_DeltaTimeRaw, 0.25f);
            if (g_SysWork.bgmStatusFlags & BgmStatusFlag_ApplyMute)
            {
                ducking = Q12(1.0f);
            }
            else if (g_SysWork.bgmStatusFlags & BgmStatusFlag_RadioActive)
            {
                ducking = Q12(0.75f);
            }
            else
            {
                ducking = (g_SysWork.bgmStatusFlags & BgmStatusFlag_Duck) ? Q12(0.5f) : Q12(0.0f);
            }
        }
        else
        {
            // Turn on music layer.
            if ((flagsCpy >> i) & 1)
            {
                adjustLayerValue = FP_MULTIPLY(g_DeltaTimeRaw, fadeSpeed, Q12_SHIFT - 1); // @hack Should be multiplied by 2 but doesn't match.
                ducking          = Q12(1.0f);
            }
            // Turn off music layer.
            else
            {
                adjustLayerValue = Q12_MULT(g_DeltaTimeRaw, fadeSpeed);
                ducking          = Q12(0.0f);
            }
        }

        targetVol = ducking - curLayerVol;
        if (curLayerVol != ducking)
        {
            if (adjustLayerValue < targetVol)
            {
                curLayerVol += adjustLayerValue;
            }
            else if (targetVol >= -adjustLayerValue)
            {
                curLayerVol = ducking;
            }
            else
            {
                curLayerVol -= adjustLayerValue;
            }
        }

        layerVols[i] = curLayerVol;
    }

    isBgmChannelActive = false;
    firstSongLayerVol  = Q12(1.0f) - layerVols[8];
    
    // TODO: Figure out this floating-point math.
    // @note These extremely small values likely relate to delta time, as `layerVols[8]` is derived from it.
    // The resulting value sets a parameter used to adjust channel 0 volume in specific contexts (e.g. in the inventory
    // menu).
    // Updates console's MIDI channel volume.
    for (i = 0; i < (ARRAY_SIZE(g_SysWork.bgmLayerVolumes) - 1); i++)
    {
        curChannelVol       = layerVols[i];
        isBgmChannelActive |= curChannelVol != Q12(0.0f);

        if (i == 0)
        {
            curChannelVol = Q12_MULT_PRECISE(curChannelVol, firstSongLayerVol);
        }

        curChannelVol = Q12_MULT_PRECISE(curChannelVol, Q12(0.0312f));
        if (curChannelVol > Q12(0.0312f))
        {
            curChannelVol = Q12(0.0312f);
        }

        curChannelVol = (curChannelVol * channelLimitsCpy[i]) >> 7; // TODO: Equivalent of `/ Q12(0.0312f)` but causes missmatch.
        if (curChannelVol > Q12(0.0312f))
        {
            curChannelVol = Q12(0.0312f);
        }

        bgmChannelVols[i] = curChannelVol;
    }

    isMusicPlaying    = false;
    areChannelsActive = activeSetChannelTask = Sd_MidiChannelTaskGet();

    areChannelsActive = activeSetChannelTask != 0 && areChannelsActive != 0xFFFF;

    // Update music channels.
    if (isBgmChannelActive)
    {
        switch (g_Bgm_ChannelSetProcessState)
        {
            case 3:
                Bgm_LayerGlobalVariablesMute();

                if (areChannelsActive)
                {
                    g_Bgm_ChannelSetProcessState = 0;
                }
                else
                {
                    Sd_BgmChannelSet();
                    g_Bgm_ChannelSetProcessState = 2;
                }
                break;

            case 2:
                Bgm_LayerGlobalVariablesMute();
                g_Bgm_ChannelSetProcessState = 1;
                break;

            case 1:
                if (areChannelsActive)
                {
                    Bgm_LayerGlobalVariablesUpdate();
                }
                else
                {
                    Bgm_LayerGlobalVariablesMute();
                }

                g_Bgm_ChannelSetProcessState = 0;
                break;

            case 0:
                isMusicPlaying = true;
                break;
        }
    }
    else if (flagsCpy & BgmFlag_MuteAll)
    {
        if (g_Bgm_ChannelSetProcessState != 3)
        {
            g_Bgm_ChannelSetProcessState = 3;
            SD_Call(18);
        }
    }
    else if (g_Bgm_ChannelSetProcessState == 0)
    {
        isMusicPlaying = true;
    }

    if (isMusicPlaying)
    {
        if (areChannelsActive)
        {
            for (i = 0; i < (ARRAY_SIZE(g_SysWork.bgmLayerVolumes) - 1); i++)
            {
                Sd_MidiChannelsVolumeSet(i, bgmChannelVols[i]);
            }
        }
        else
        {
            Bgm_LayerGlobalVariablesMute();
            g_Bgm_ChannelSetProcessState = 3;
        }
    }

    g_Bgm_LayersUpdated = true;
}

void Bgm_MenuUpdate(void) // 0x800363D0
{
    g_RadioPitchState         = 0;
    g_SysWork.bgmStatusFlags |= BgmStatusFlag_Duck;
    Bgm_Update(false);
}

void Bgm_SongChange(s32 bgmIdx) // 0x8003640C
{
    if (bgmIdx != BgmCmd_UpdateLayers)
    {
        g_MapOverlayHdr.bgmCmd = bgmIdx;
    }
}

// ========================================
// PLAYER ROOM INFORMATION
// ========================================

void Game_MapRoomIdxUpdate(void) // 0x80036420
{
    q19_12 posX;
    q19_12 posZ;
    s8     newMapRoomIdx;

    #define playerChara g_SysWork.playerWork.player

    posX = playerChara.position.vx;
    posZ = playerChara.position.vz;

    // Set map room index based on current player position.
    if (g_MapOverlayHdr.mapRoomIdxGet == NULL)
    {
        newMapRoomIdx = 0;
    }
    else
    {
        newMapRoomIdx = g_MapOverlayHdr.mapRoomIdxGet(posX, posZ);
    }
    g_SavegamePtr->mapRoomIdx = newMapRoomIdx;

    #undef playerChara
}

s32 func_8003647C(void) // 0x8003647C
{
    return g_SavegamePtr->mapRoomIdx > g_MapOverlayHdr.unused_8;
}

s32 func_80036498(void) // 80036498
{
    return !(g_SavegamePtr->mapRoomIdx > g_MapOverlayHdr.unused_8);
}

// ========================================
// UNKNOWN UNUSED MATH
// ========================================

u32 func_800364BC(void) // 0x800364BC
{
    u32        var0;
    u32        var1;
    static u32 D_800BCD58;

    D_800BCD58 += g_DeltaTimeRaw * (Q12(64.0f) + 1);

    var0  = Q12(64.0f);
    var0 += Math_Sin(D_800BCD58 >> 18) * 8;
    var1  = Math_Sin((D_800BCD58 & 0xFFFF) / 16) * 32;
    return FP_FROM(var0 + var1, Q12_SHIFT);
}
