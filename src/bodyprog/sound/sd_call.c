#include "game.h"

#include <psyq/libcd.h>
#include <psyq/libetc.h>
#include <psyq/libspu.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/libsd.h"
#include "bodyprog/math/math.h"
#include "main/fsqueue.h"

#define BSS_HACK_SD_CALL_C
#include "bodyprog/sound/sound_system.h"

#ifndef PAD_HACK_IGNORE
    const  s32  __pad_rodata_80025BF4 = 0;
    static s32  __pad_bss_800C15EC;
    static s16  __pad_bss_800C15F2[2];
    static s32  __pad_bss_800C1674;
    static s32  __pad_bss_800C1694;
    static char __pad_bss_800C37C9[3];
    static char __pad_bss_800C37D1[3];
#endif

// ========================================
// DEFINES
// ========================================

#define CD_ERROR_LIMIT           600 // Matches value used in beatmania `FSCD.C`.
#define TASK_POOL_SIZE           32
#define AUDIO_DATA_LOAD_ATTEMPTS 16
#define VAB_BUFFER_LIMIT         0xC800u
#define PREGAP                   150

// ========================================
// STATIC VARIABLES
// ========================================

/** @brief Stores read position of processing XA file in CD. */
static CdlLOC g_Sd_XaCdLocation;

/** @brief Stores information for processing access to XA file data. */
static u_Sd_XaCdlInfo g_Sd_XaCdlInfo;

/** @brief Stores SFX IDs of currently playing SFXs.
 * SFX IDs are stored in the same index where the audio is being play
 * at PSX "Voice" channels.
 */
static u16 g_AudioPlayingIdxList[SD_VOICE_COUNT];

/** @brief Stores the pitch of currently playing SFX.
 * Shares the same index of where the SFX is stored at `g_AudioPlayingIdxList`.
 */
static s16 g_AudioPlayingPitchList[SD_VOICE_COUNT];

/** @brief Stores information of some audio processes. */
static s_Sd_AudioWork g_Sd_AudioWork;

/** @brief Holds states for different audio types streaming. */
static s_AudioStreamingStates g_Sd_AudioStreamingStates;

/** @brief Stores main game audio channels volumes.
 * 
 * @note Name from retrieved debug symbols in Konami International Rally Championship.
 * Symbols lacks the original struct. The size may contradict this definition as it seems the variable is intended
 * to be 48/0x30 bytes, while this is 16/0x10 bytes.
 */
static s_ChannelsVolumeController gSDVolConfig;

/** @brief Stores timestamp from the currently playing XA audio. */
static s_XaAudioPlayTracking g_Sd_XaAudioPlayTracking;

/** @brief Stores information from the currently playing VAB audio. */
static s_VabPlayingInfo g_Sd_VabPlayingInfo;

/** @brief Task pool related to audio and audio data streaming.
 * `Sd_TaskPoolExecute` is the main function responsible for executing tasks.
 *
 * @note Possible name retrieved from debug symbols.
 * `Tokimeki Memorial ~Forever With You~` symbols have a global variable named
 * `gSDEvt`. This function can't be restored, but the name would fit for
 * this purpose. This game also features a similar command pool system
 * to the one in SH1.
 */
static u8 g_Sd_TaskPool[TASK_POOL_SIZE];

/** @unused Dead code. Called by `SdSetTableSize`, but the function is a nullsub. */
static s8 D_800C16C8[0x2100];

/** @brief The type of audio file being loaded. See `e_AudioType`. */
static u8 g_Sd_AudioType;

/** @brief Amount of data transferred when loading KDT/VAB files. */
static u32 g_Sd_FileDataTransferred;

/** @brief Amount of attempts when loading VAB/KDT files.
 * 
 * @note Limit is defined in `AUDIO_DATA_LOAD_ATTEMPTS` and by default is 16.
 */
static u8 g_Sd_DataLoadAttempts;

/** @brief Pointer to the data of the VAB file being loaded. */
static s_AudioItemData* g_Sd_VabTargetLoad;

/** @brief Pointer to the data of the KTD file being loaded. */
static s_AudioItemData* g_Sd_KdtTargetLoad;

/** @brief Boolean | Check if a process of loading a XA audio is pending. */
static u8 g_Sd_XaTaskPending;

/** @brief First element from `g_Sd_TaskPool[TASK_POOL_SIZE]`. */
static u8 g_Sd_CurrentTask;

// ========================================
// GENERAL AUDIO SYSTEM CORE
// ========================================

void SD_Call(u32 task) // 0x80045A7C
{
    // Execute sound command based on category.
    switch ((task >> 8) & 0xFF)
    {
        // Sound effect management and VAB + KDT file loading. Range [0, 255].
        case 0:
            SD_BranchCTRL(task);
            return;

        // Setup MIDI channels for target song. Range [300, 1279].
        case 3:
        case 4:
            Sd_SetupBgmMidiChannels(task);
            return;

        // Play SFX. Range [1280, 1791].
        case 5:
        case 6:
            Sd_SfxPlay(task, Q8(0.0f), Q8(0.0f));
            return;

        // Stop SFX. Range [1792, 2303].
        case 7:
        case 8:
            Sd_SfxStopStep(task - 0x200);
            return;

        // Stop Last Sfx Playing. Range [2816, 3327].
        case 11:
        case 12:
            Sd_LastSfxStop();
            return;

        // Play XA audio files (voice lines). Range [4096, 5887].
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
            Sd_XaAudioPlayTaskAdd(task);
    }
}

u8 Sd_AudioStreamingCheck(void) // 0x80045B28
{
    u8 state;

    state = AudioStreamingState_XaPlaying;
    if (g_Sd_AudioWork.xaAudioIdx != 0)
    {
        return state;
    }

    state = AudioStreamingState_VabPlaying;
    if (!g_Sd_AudioWork.isAudioLoading)
    {
        if (g_Sd_AudioStreamingStates.xaPreLoadState != 0)
        {
            g_Sd_XaAudioPlayTracking.vSyncTimeSinceBoot     = VSync(SyncMode_Count);
            g_Sd_XaAudioPlayTracking.xaAudioPlayCurrentTime = 0;
            return AudioStreamingState_XaLoading;
        }

        if (!g_Sd_XaTaskPending)
        {
            if (g_Sd_CurrentTask == 0)
            {
                return AudioStreamingState_None;
            }

            state = AudioStreamingState_AudioTaskPending;
            return state;
        }

        state = AudioStreamingState_XaLoadPending;
        return state;
    }

    return state;
}

u16 Sd_MidiChannelTaskGet(void) // 0x80045BC8
{
    return g_Sd_AudioWork.midiChannelsVolTask;
}

void SD_BranchCTRL(u16 task) // 0x80045BD8
{
    switch (task)
    {
        case 1: // Set audio to mono.
            Sd_AudioSystemSet(false);
            break;

        case 2: // Set audio to stereo.
            Sd_AudioSystemSet(true);
            break;

        case 16:
            Sd_AllSfxStop();
            Sd_LastSfxStop();
            break;

        case 17:
            Sd_AllSfxWithRRStop();
            Sd_LastSfxStop();
            break;

        case 18:
            Sd_BgmStopTaskAdd();
            break;

        case 21:
            Sd_AllSfxWithRRStop();

        case 20:
            Sd_AllSfxStop();
            Sd_LastSfxStop();
            Sd_BgmStopTaskAdd();

        case 19:
            Sd_XaAudioStopTaskAdd();
            break;

        case 22:
            g_Sd_AudioWork.bgmFadeSpeed = 1;

        default:
            break;

        case 23:
            g_Sd_AudioWork.bgmFadeSpeed = 2;
            break;

        case 3:
            g_Sd_AudioWork.muteGame = true;
            break;

        case 4:
            g_Sd_AudioWork.muteGame = false;
            break;
    }

    // Load VAB audio.
    if (task >= 160 && task < 245)
    {
        Sd_VabLoad_TaskAdd(task);
    }

    // Load KDT song file and VAB music samples.
    // Passes command to previous conditional to load VAB audio.
    if (task >= 32 && task < 72)
    {
        Sd_KdtLoad_TaskAdd(task);
    }
}

void Sd_AudioSystemSet(u8 isStereo) // 0x80045D28
{
    CdlATV vol;

    switch (isStereo)
    {
        case false:
            SdSetMono();

            // SPU (L).
            vol.val0 = vol.val2 = 79;

            // SPU (R).
            vol.val1 = vol.val3 = 79;

            CdMix(&vol);

            gSDVolConfig.volumeSe          = 127;
            g_Sd_AudioWork.isStereoEnabled = false;
            return;

        case true:
            SdSetStereo();

            // SPU (L).
            vol.val0 = vol.val2 = 127;

            // SPU (R).
            vol.val1 = vol.val3 = 0;
            CdMix(&vol);

            gSDVolConfig.volumeSe          = 127;
            g_Sd_AudioWork.isStereoEnabled = true;
            return;
    }
}

void SD_Init(void) // 0x80045DD4
{
    SdInit();
    SdSetTickMode(1);
    Sd_AudioSystemSet(true);
    SdSetReservedVoice(SD_VOICE_COUNT);
    SdStart();
    SdSetTableSize(&D_800C16C8, 16, 3);

    gSDVolConfig.globalVolumeSe  = OPT_SOUND_VOLUME_MAX - 1;
    gSDVolConfig.globalVolumeBgm = OPT_SOUND_VOLUME_MAX - 1;
    gSDVolConfig.globalVolumeXa  = OPT_SOUND_VOLUME_MAX - 1;

    SD_InitStruct();
}

void SD_InitStruct(void) // 0x80045E44
{
    static s32 i;

    SdSetAutoKeyOffMode(0);
    SdUtSetReverbType(1);
    SpuClearReverbWorkArea(1);
    SdUtReverbOn();
    SpuSetTransferMode(0);

    gSDVolConfig.reverbDepth = 20;

    SdUtSetReverbDepth(20, 20);
    Sd_SetReverbEnable(0);
    SdSetSerialAttr(0, 0, 0);
    Sd_XaVolumeSet(0, 0);

    g_Sd_XaCdlInfo.cdlMode = CdlModeSpeed;
    Sd_CdPrimitiveCmdTry(CdlSetmode, &g_Sd_XaCdlInfo.cdlMode, NULL);

    for (i = 0; i < (TASK_POOL_SIZE - 1); i++)
    {
        g_Sd_TaskPool[i] = 0;
    }

    for (i = 0; i < SD_VOICE_COUNT; i++)
    {
        g_AudioPlayingIdxList[i] = 0;
    }

    g_Sd_AudioWork.bgmLoadedSongIdx     = 0;
    g_Sd_AudioWork.activeVabAudioIdx[0] = 0;
    g_Sd_AudioWork.activeVabAudioIdx[1] = 0;
    g_Sd_AudioWork.activeVabAudioIdx[2] = 0;
    g_Sd_AudioWork.xaAudioIdx           = 0;
    g_Sd_AudioWork.isXaStopping         = false;
    g_Sd_AudioWork.cdErrorCount         = 0;
    g_Sd_AudioWork.bgmFadeSpeed         = 0;
    g_Sd_AudioWork.isAudioLoading       = false;
    g_Sd_AudioWork.isXaNotPlaying       = false;
    g_Sd_AudioWork.muteGame             = false;
    gSDVolConfig.globalVolumeGame       = 127;

    SdSetMVol(127, 127);

    g_Sd_XaTaskPending                       = false;
    g_Sd_AudioWork.midiChannelsVolTask       = 0;
    g_Sd_AudioWork.midiChannelsVolTaskToSet  = 0;
    g_Sd_AudioStreamingStates.audioLoadState = AudioLoadState_Reset;
    g_Sd_AudioStreamingStates.xaLoadState    = XaLoadState_Initialize;
    g_Sd_AudioStreamingStates.xaStopState    = 0;
    g_Sd_AudioStreamingStates.xaPreLoadState = 0;
    gSDVolConfig.volumeXa                    = 84;
    gSDVolConfig.volumeBgmToSet              = 40;
    gSDVolConfig.volumeBgm                   = 40;

    Sd_BgmVolumeSet(gSDVolConfig.volumeBgm, gSDVolConfig.volumeBgm);
}

// ========================================
// AUDIO VAB
// ========================================

static inline void Sd_SharedVolSet(s16* left, s16* right, s16 vol)
{
    *left  = vol;
    *right = vol;
}

void Sd_AudioStop(void) // 0x80045FF8
{
    s32 i;

    SdSeqClose(0);

    for (i = 4; i >= 0; i--)
    {
        SdVabClose(i);
    }

    SdEnd();
    SdQuit();
}

u8 Sd_SfxPlay(u16 sfxId, q0_7 balance, q0_8 vol) // 0x80046048
{
    static s16   audioIdx;
    SpuVoiceAttr attr;
    s16          targetVol;
    s16          volCpy;
    s32          i;

    if (sfxId == Sfx_Base)
    {
        return NO_VALUE;
    }

    audioIdx = sfxId - Sfx_Base;
    volCpy   = vol;

    // Copy key VAB information.
    g_Sd_VabPlayingInfo.typeIdx = g_Vab_InfoTable[audioIdx].vabProgIdx >> 8;
    g_Sd_VabPlayingInfo.progIdx = g_Vab_InfoTable[audioIdx].vabProgIdx & 0xFF;
    g_Sd_VabPlayingInfo.noteIdx = g_Vab_InfoTable[audioIdx].noteIdx;

    targetVol  = gSDVolConfig.volumeSe + g_Vab_InfoTable[audioIdx].volumeMin;
    targetVol -= (targetVol * volCpy) / 255;

    Sd_SharedVolSet(&g_Sd_VabPlayingInfo.volumeLeft, &g_Sd_VabPlayingInfo.volumeRight, targetVol);

    // Apply stereo balance.
    if (g_Sd_AudioWork.isStereoEnabled == true)
    {
        if (balance < Q8(0.0f))
        {
            g_Sd_VabPlayingInfo.volumeRight -= (g_Sd_VabPlayingInfo.volumeLeft * ABS(balance)) >> 7;
        }
        else
        {
            g_Sd_VabPlayingInfo.volumeLeft -= (g_Sd_VabPlayingInfo.volumeLeft * balance) >> 7;
        }
    }

    // Clamp volume to positive range.
    if (g_Sd_VabPlayingInfo.volumeLeft < 0)
    {
        g_Sd_VabPlayingInfo.volumeLeft = 0;
    }
    if (g_Sd_VabPlayingInfo.volumeRight < 0)
    {
        g_Sd_VabPlayingInfo.volumeRight = 0;
    }

    if (sfxId == Sfx_RadioInterferenceLoop)
    {
        g_Sd_VabPlayingInfo.toneIdx = g_Vab_InfoTable[audioIdx].audioVabIdx;
        SdUtKeyOnV(22, g_Sd_VabPlayingInfo.typeIdx, g_Sd_VabPlayingInfo.progIdx,
                   g_Sd_VabPlayingInfo.toneIdx, g_Sd_VabPlayingInfo.noteIdx, 0,
                   Sd_SeVolumeGet(g_Sd_VabPlayingInfo.volumeLeft), Sd_SeVolumeGet(g_Sd_VabPlayingInfo.volumeRight));
        g_Sd_VabPlayingInfo.voiceIdx = 22;
    }
    else if (sfxId == Sfx_RadioStaticLoop)
    {
        g_Sd_VabPlayingInfo.toneIdx = g_Vab_InfoTable[audioIdx].audioVabIdx;
        SdUtKeyOnV(23, g_Sd_VabPlayingInfo.typeIdx, g_Sd_VabPlayingInfo.progIdx,
                   g_Sd_VabPlayingInfo.toneIdx, g_Sd_VabPlayingInfo.noteIdx, 120,
                   Sd_SeVolumeGet(g_Sd_VabPlayingInfo.volumeLeft), Sd_SeVolumeGet(g_Sd_VabPlayingInfo.volumeRight));
        g_Sd_VabPlayingInfo.voiceIdx = 23;
    }
    else
    {
        g_Sd_VabPlayingInfo.voiceIdx = SdVoKeyOn(g_Vab_InfoTable[audioIdx].vabProgIdx, g_Sd_VabPlayingInfo.noteIdx * 0x100,
                                                 Sd_SeVolumeGet(g_Sd_VabPlayingInfo.volumeLeft), Sd_SeVolumeGet(g_Sd_VabPlayingInfo.volumeRight));
    }

    // Clears any instance where the audio was being previously play at some "Voice" channel.
    for (i = 0; i < SD_VOICE_COUNT; i++)
    {
        if (g_AudioPlayingIdxList[i] == sfxId)
        {
            g_AudioPlayingIdxList[i] = 0;
        }
    }

    // Updates `g_AudioPlayingIdxList` to set which "Voice" channel will be playing the audio
    // and retrieves the pitch intended to be used, updating `g_AudioPlayingPitchList`.
    if (g_Sd_VabPlayingInfo.voiceIdx < SD_VOICE_COUNT)
    {
        g_AudioPlayingIdxList[g_Sd_VabPlayingInfo.voiceIdx] = sfxId;
        attr.voice                                          = 1 << g_Sd_VabPlayingInfo.voiceIdx;

        SpuGetVoiceAttr(&attr);

        g_AudioPlayingPitchList[g_Sd_VabPlayingInfo.voiceIdx] = attr.pitch;
        return g_Sd_VabPlayingInfo.voiceIdx;
    }

    return NO_VALUE;
}

void Sd_SfxAttributesUpdate(u16 sfxId, q0_7 balance, q0_8 vol, s8 pitch) // 0x800463C0
{
    static s16   audioIdx;
    static u16   audioPitch;
    SpuVoiceAttr attr;
    s16          convertedVol;
    s32          voiceIdx;
    s32          i;

    if (sfxId == Sfx_Base)
    {
        return;
    }

    audioIdx                    = sfxId - Sfx_Base;
    g_Sd_VabPlayingInfo.volumeLeft = gSDVolConfig.volumeSe + g_Vab_InfoTable[audioIdx].volumeMin;

    if (sfxId == Sfx_RadioInterferenceLoop)
    {
        voiceIdx   = 22;
        attr.voice = 1 << 22;
    }
    else if (sfxId == Sfx_RadioStaticLoop)
    {
        voiceIdx   = 23;
        attr.voice = 1 << 23;
    }
    else
    {
        voiceIdx = NO_VALUE;
        for (i = 0; i < SD_VOICE_COUNT; i++)
        {
            if (g_AudioPlayingIdxList[i] == sfxId)
            {
                voiceIdx = i;
            }
        }

        if (voiceIdx < 0)
        {
            return;
        }

        attr.voice = 1 << voiceIdx;
    }

    g_Sd_VabPlayingInfo.pitch   = 0;
    g_Sd_VabPlayingInfo.noteIdx = g_Vab_InfoTable[audioIdx].noteIdx;
    audioPitch                  = g_AudioPlayingPitchList[voiceIdx] + (pitch * 2);
    convertedVol                = vol;
    convertedVol                = g_Sd_VabPlayingInfo.volumeLeft - ((g_Sd_VabPlayingInfo.volumeLeft * (convertedVol)) / 255);

    Sd_SharedVolSet(&g_Sd_VabPlayingInfo.volumeLeft, &g_Sd_VabPlayingInfo.volumeRight, convertedVol);

    // Apply stereo balance.
    if (g_Sd_AudioWork.isStereoEnabled == true)
    {
        if (balance < Q8(0.0f))
        {
            g_Sd_VabPlayingInfo.volumeRight -= (convertedVol * ABS(balance)) >> 7;
        }
        else
        {
            g_Sd_VabPlayingInfo.volumeLeft -= (convertedVol * balance) >> 7;
        }
    }

    SpuGetVoiceAttr(&attr);

    attr.mask          = 0x1F;
    attr.volmode.left  = 0;
    attr.volmode.right = 0;
    attr.volmode.left  = 0;
    attr.volmode.right = 0;

    // Clamp volume to positive range.
    if (g_Sd_VabPlayingInfo.volumeLeft < 0)
    {
        g_Sd_VabPlayingInfo.volumeLeft = 0;
    }
    if (g_Sd_VabPlayingInfo.volumeRight < 0)
    {
        g_Sd_VabPlayingInfo.volumeRight = 0;
    }

    attr.volume.right = Sd_SeVolumeGet(g_Sd_VabPlayingInfo.volumeRight << 7);
    attr.volume.left  = Sd_SeVolumeGet(g_Sd_VabPlayingInfo.volumeLeft << 7);
    attr.pitch        = audioPitch;

    SpuSetVoiceAttr(&attr);
}

void Sd_SfxWithPitchPlay(u16 sfxId, q0_7 balance, q0_8 vol, s8 pitch) // 0x80046620
{
    static s16 audioIdx;
    s16        volMin;
    s16        convertedVol;

    if (sfxId == Sfx_Base)
    {
        return;
    }

    audioIdx                    = sfxId - Sfx_Base;
    g_Sd_VabPlayingInfo.typeIdx = g_Vab_InfoTable[audioIdx].vabProgIdx >> 8;
    g_Sd_VabPlayingInfo.progIdx = g_Vab_InfoTable[audioIdx].vabProgIdx & 0xFF;
    g_Sd_VabPlayingInfo.toneIdx = g_Vab_InfoTable[audioIdx].audioVabIdx;
    g_Sd_VabPlayingInfo.noteIdx = g_Vab_InfoTable[audioIdx].noteIdx + (s8)((pitch * 5) / 127);

    if (pitch > 0)
    {
        g_Sd_VabPlayingInfo.pitch = ABS(pitch * 5) % 127;
    }
    else
    {
        g_Sd_VabPlayingInfo.pitch = 127 - (ABS(pitch * 5) % 127);
    }

    volMin                         = gSDVolConfig.volumeSe + g_Vab_InfoTable[audioIdx].volumeMin;
    convertedVol                   = vol;
    g_Sd_VabPlayingInfo.volumeLeft = volMin - ((volMin * convertedVol) / 255);

    Sd_SharedVolSet(&g_Sd_VabPlayingInfo.volumeLeft, &g_Sd_VabPlayingInfo.volumeRight, g_Sd_VabPlayingInfo.volumeLeft);

    // Apply stereo balance.
    if (g_Sd_AudioWork.isStereoEnabled == true)
    {
        if (balance < Q8(0.0f))
        {
            g_Sd_VabPlayingInfo.volumeRight -= (g_Sd_VabPlayingInfo.volumeRight * ABS(balance)) >> 7;
        }
        else
        {
            g_Sd_VabPlayingInfo.volumeLeft -= (g_Sd_VabPlayingInfo.volumeLeft * balance) >> 7;
        }
    }

    // Clamp volume to positive range.
    if (g_Sd_VabPlayingInfo.volumeLeft < 0)
    {
        g_Sd_VabPlayingInfo.volumeLeft = 0;
    }
    if (g_Sd_VabPlayingInfo.volumeRight < 0)
    {
        g_Sd_VabPlayingInfo.volumeRight = 0;
    }

    g_Sd_VabPlayingInfo.voiceIdx = SdUtKeyOn(g_Sd_VabPlayingInfo.typeIdx, g_Sd_VabPlayingInfo.progIdx, g_Sd_VabPlayingInfo.toneIdx, g_Sd_VabPlayingInfo.noteIdx, g_Sd_VabPlayingInfo.pitch,
                                             Sd_SeVolumeGet(g_Sd_VabPlayingInfo.volumeLeft), Sd_SeVolumeGet(g_Sd_VabPlayingInfo.volumeRight));
}

void Sd_LastSfxStop(void) // 0x800468EC
{
    SdUtKeyOffV(23);
}

void Sd_SfxStop(u16 sfxId) // 0x8004690C
{
    Sd_SfxStopStep(sfxId);
}

void Sd_SfxStopStep(u16 sfxId) // 0x8004692C
{
    static s16 vabInfoIdx;
    static s16 vabProgIdxs;
    static s16 pitch;

    if (sfxId == Sfx_Base)
    {
        return;
    }

    vabInfoIdx  = sfxId - Sfx_Base;
    vabProgIdxs = g_Vab_InfoTable[vabInfoIdx].vabProgIdx;
    pitch       = g_Vab_InfoTable[vabInfoIdx].noteIdx << 8;
    SdVoKeyOff(vabProgIdxs, pitch);
}

void Sd_AllSfxStop(void) // 0x800469AC
{
    s32 i;

    for (i = 0; i < SD_VOICE_COUNT; i++)
    {
        SdUtKeyOffV(i);
    }
}

void Sd_AllSfxWithRRStop(void) // 0x800469E8
{
    s32 i;

    for (i = 0; i < SD_VOICE_COUNT; i++)
    {
        SdUtKeyOffVWithRROff(i);
    }
}

// ========================================
// BGM HANDLING
// ========================================

static inline void Sd_MidiChannelTaskUpdate(u16 task)
{
    g_Sd_AudioWork.bgmFadeSpeed             = 0;
    g_Sd_AudioWork.midiChannelsVolTaskToSet = 0;
    g_Sd_AudioWork.midiChannelsVolTask      = task;
}

void Sd_SetupBgmMidiChannels(u16 task) // 0x80046A24
{
    if (g_Sd_AudioWork.midiChannelsVolTaskToSet != task && g_Sd_AudioWork.midiChannelsVolTask != task)
    {
        g_Sd_AudioWork.midiChannelsVolTaskToSet = task;
        Sd_TaskPoolAdd(7);
    }
}

static void func_80046A70(void) // 0x80046A70
{
    Sd_MidiChannelTaskUpdate(g_Sd_AudioWork.midiChannelsVolTaskToSet);

    SdSeqPlay(0, 1, 0);

    gSDVolConfig.volumeBgm      = 40;
    gSDVolConfig.volumeBgmToSet = 40;

    Sd_BgmVolumeSet(gSDVolConfig.volumeBgm, gSDVolConfig.volumeBgm);
    Sd_TaskPoolUpdate();
}

static void Sd_BgmStopTaskAdd(void) // 0x80046AD8
{
    g_Sd_AudioWork.midiChannelsVolTask = NO_VALUE;
    Sd_TaskPoolAdd(8);
}

static void Sd_BgmStop(void) // 0x80046B04
{
    if (gSDVolConfig.volumeBgmToSet > 0)
    {
        gSDVolConfig.volumeBgmToSet -= 4;
    }

    if (gSDVolConfig.volumeBgmToSet <= 0)
    {
        gSDVolConfig.volumeBgmToSet = 0;
        Sd_BgmStopStep();
        Sd_TaskPoolUpdate();
    }

    gSDVolConfig.volumeBgm = gSDVolConfig.volumeBgmToSet;
    Sd_BgmVolumeSet(gSDVolConfig.volumeBgm, gSDVolConfig.volumeBgm);
}

static void Sd_BgmStopStep(void) // 0x80046B78
{
    Sd_BgmVolumeSet(0, 0);
    SdSeqStop(0);

    g_Sd_AudioWork.bgmFadeSpeed        = 0;
    g_Sd_AudioWork.midiChannelsVolTask = 0;
}

u8 Sd_MidiChannelVolumeGet(u8 channelIdx) // 0x80046BB4
{
    u32 i;
    u8  vol;

    if (channelIdx == 0)
    {
        return 0;
    }

    if (g_Sd_AudioWork.midiChannelsVolTask > SD_TASK_CHANNEL_SET(40))
    {
        return 0;
    }

    vol = 0;

    for (i = 0; i < 15; i++)
    {
        if (g_Sd_SongsChannelsForLayers[(u8)g_Sd_AudioWork.midiChannelsVolTask][i] == channelIdx)
        {
            vol = SdGetMidiVol(0, i);
            break;
        }
    }

    return vol;
}

void Sd_MidiChannelsVolumeSet(u8 channelIdx, u8 vol) // 0x80046C54
{
    u32 i;
    s16 volCpy;
    u8  targetChannelIdx;
    u8  idx;

    if (channelIdx == 0)
    {
        gSDVolConfig.volumeBgm = (vol * 40) / 127;
    }
    else if (g_Sd_AudioWork.midiChannelsVolTask <= SD_TASK_CHANNEL_SET(40))
    {

        idx = (u8)g_Sd_AudioWork.midiChannelsVolTask;

        for (i = 0; i < 15; i++)
        {
            targetChannelIdx = g_Sd_SongsChannelsForLayers[idx][i];
            volCpy           = vol;

            if (targetChannelIdx == channelIdx)
            {
                SdSetMidiVol(0, i, volCpy);
            }
        }
    }
}

// ========================================
// XA FILES RELATED
// ========================================

void Sd_XaAudioPlayTaskAdd(u16 sfx) // 0x80046D3C
{
    g_Sd_AudioWork.xaAudioIdxCheck = sfx & 0xFFF;

    if (gSDXATable[g_Sd_AudioWork.xaAudioIdxCheck].xaFileIdx != 0)
    {
        g_Sd_XaTaskPending                              = true;
        g_Sd_XaAudioPlayTracking.vSyncTimeSinceBoot     = VSync(SyncMode_Count);
        g_Sd_XaAudioPlayTracking.xaAudioPlayCurrentTime = 0;

        Sd_TaskPoolAdd(2);

        g_Sd_AudioWork.xaAudioIdx = g_Sd_AudioWork.xaAudioIdxCheck;

        Sd_TaskPoolAdd(1);
    }
}

/** @unused Gets the length of XA audios in `gSDXATable`. */
s32 Sd_XaAudioLengthGet(s32 idx) // 0x80046DCC
{
    return (gSDXATable[idx & 0xFFF].audioLength & 0xFFFFFF) + 32;
}

/** @brief Loads and plays XA audio in `gSDXATable`. */
static void Sd_XaAudioPlay(void) // 0x80046E00
{

    static u16 xaAudioIdx;
    static u32 xaFileOffset;
    u32*       xaFileOffsetsPtr;
    u32*       xaFileOffsetTargetPtr;

    g_Sd_AudioWork.cdErrorCount++;

    switch (g_Sd_AudioStreamingStates.xaLoadState)
    {
        case XaLoadState_Initialize:
            if (g_Sd_AudioWork.bgmFadeSpeed == 0)
            {
                gSDVolConfig.volumeBgm = 24;
            }

            xaAudioIdx = g_Sd_AudioWork.xaAudioIdxCheck;
            switch (xaAudioIdx)
            {
                case 53:
                case 56:
                case 596:
                case 597:
                case 598:
                case 600:
                case 602:
                case 612:
                case 614:
                case 620:
                case 657:
                case 606:
                    gSDVolConfig.volumeXaToSet = Sd_SeVolumeGet(84);
                    break;

                case 723:
                case 725:
                    gSDVolConfig.volumeXaToSet = 50;
                    break;

                case 724:
                    gSDVolConfig.volumeXaToSet = 40;
                    break;

                default:
                    gSDVolConfig.volumeXaToSet = 84;
                    break;
            }

            gSDVolConfig.volumeXa             = gSDVolConfig.volumeXaToSet;
            Sd_XaVolumeSet(gSDVolConfig.volumeXaToSet, gSDVolConfig.volumeXaToSet);
            g_Sd_XaCdlInfo.cdlMode                = CdlModeSpeed | CdlModeRT | CdlModeSF;
            g_Sd_AudioStreamingStates.xaLoadState = XaLoadState_SetMode;
            break;

        case XaLoadState_SetMode:
            if (!Sd_CdPrimitiveCmdTry(CdlSetmode, &g_Sd_XaCdlInfo.cdlMode, NULL))
            {
                g_Sd_AudioWork.cdErrorCount           = 0;
                g_Sd_AudioStreamingStates.xaLoadState = XaLoadState_PrepareFilter;
            }
            break;

        default:
            break;

        case XaLoadState_PrepareFilter:
            g_Sd_XaCdlInfo.cdlFilter.file         = gSDXATable[xaAudioIdx].field_8_24;
            g_Sd_XaCdlInfo.cdlFilter.chan         = gSDXATable[xaAudioIdx].field_4_24;
            g_Sd_AudioStreamingStates.xaLoadState = XaLoadState_SetFilter;
            break;

        case XaLoadState_SetFilter:
            if (!Sd_CdPrimitiveCmdTry(CdlSetfilter, &g_Sd_XaCdlInfo.cdlFilter, NULL))
            {
                g_Sd_AudioWork.cdErrorCount           = 0;
                g_Sd_AudioStreamingStates.xaLoadState = XaLoadState_CalculateLba;
            }
            break;

        case XaLoadState_CalculateLba:
            // @hack Needed for match, weird code.
            xaFileOffsetsPtr      = g_FileXaLoc;
            xaFileOffsetTargetPtr = &xaFileOffsetsPtr[gSDXATable[xaAudioIdx].xaFileIdx];
            xaFileOffset          = *xaFileOffsetTargetPtr;
            xaFileOffset         += PREGAP + gSDXATable[xaAudioIdx].sector;

            g_Sd_XaAudioPlayTracking.xaAudioLength = gSDXATable[xaAudioIdx].audioLength + 32;

            g_Sd_AudioStreamingStates.xaLoadState = XaLoadState_Seek;
            g_Sd_XaCdLocation.sector              = itob(xaFileOffset % 75);
            xaFileOffset                         /= 75;
            g_Sd_XaCdLocation.second              = itob(xaFileOffset % 60);
            xaFileOffset                         /= 60;
            g_Sd_XaCdLocation.minute              = itob(xaFileOffset);
            break;

        case XaLoadState_Seek:
            if (!Sd_CdPrimitiveCmdTry(CdlSeekL, &g_Sd_XaCdLocation, NULL))
            {
                g_Sd_AudioWork.cdErrorCount           = 0;
                g_Sd_AudioStreamingStates.xaLoadState = XaLoadState_StartRead;
            }
            break;

        case XaLoadState_StartRead:
            if (!Sd_CdPrimitiveCmdTry(CdlReadN, NULL, NULL))
            {
                g_Sd_AudioWork.cdErrorCount           = 0;
                g_Sd_XaTaskPending                    = false;
                g_Sd_AudioStreamingStates.xaLoadState = XaLoadState_EnableAudio;
            }
            break;

        case XaLoadState_EnableAudio:
            g_Sd_AudioWork.xaAudioIdx = xaAudioIdx;

            SdSetSerialAttr(0, 0, 1);
            g_Sd_XaAudioPlayTracking.vSyncTimeSinceBoot     = VSync(SyncMode_Count);
            g_Sd_XaAudioPlayTracking.xaAudioPlayCurrentTime = 0;
            g_Sd_AudioStreamingStates.xaLoadState           = XaLoadState_Initialize;

            Sd_TaskPoolUpdate();
            g_Sd_AudioWork.cdErrorCount   = 0;
            g_Sd_AudioWork.isXaNotPlaying = false;
            break;
    }
}

void Sd_XaPreLoadAudioPreTaskAdd(u16 xaIdx) // 0x8004729C
{
    Sd_XaPreLoadAudioTaskAdd(xaIdx);
}

void Sd_XaPreLoadAudioTaskAdd(s32 xaIdx) // 0x800472BC
{
    g_Sd_AudioWork.xaAudioIdxCheck = xaIdx & 0xFFF;
    g_Sd_XaTaskPending             = true;

    if (g_Sd_AudioWork.xaAudioIdx != 0)
    {
        Sd_TaskPoolAdd(2);
    }

    Sd_TaskPoolAdd(6);
}

static void Sd_XaPreLoadAudio(void) // 0x80047308
{
    static u16 xaAudioIdx;
    static u16 __pad_800C15D2;
    static u32 xaFileOffset;
    u32*       xaFileOffsetsPtr;
    u32*       xaFileOffsetTargetPtr;

    g_Sd_AudioWork.cdErrorCount++;

    switch (g_Sd_AudioStreamingStates.xaPreLoadState)
    {
        case 0:
            xaAudioIdx                               = g_Sd_AudioWork.xaAudioIdxCheck;
            Sd_XaVolumeSet(0, 0);
            g_Sd_XaCdlInfo.cdlMode                   = CdlModeSpeed | CdlModeRT | CdlModeSF;
            g_Sd_AudioStreamingStates.xaPreLoadState = 1;
            break;

        case 1:
            if (!Sd_CdPrimitiveCmdTry(CdlSetmode, &g_Sd_XaCdlInfo.cdlMode, NULL))
            {
                g_Sd_AudioWork.cdErrorCount              = 0;
                g_Sd_AudioStreamingStates.xaPreLoadState = 2;
            }
            break;

        case 2:
            g_Sd_XaCdlInfo.cdlFilter.file            = gSDXATable[xaAudioIdx].field_8_24;
            g_Sd_XaCdlInfo.cdlFilter.chan            = gSDXATable[xaAudioIdx].field_4_24;
            g_Sd_AudioStreamingStates.xaPreLoadState = 3;
            return;

        case 3:
            if (!Sd_CdPrimitiveCmdTry(CdlSetfilter, &g_Sd_XaCdlInfo.cdlFilter, NULL))
            {
                g_Sd_AudioWork.cdErrorCount              = 0;
                g_Sd_AudioStreamingStates.xaPreLoadState = 4;
            }
            break;

        case 4:
            // @hack Needed for match, weird code.
            xaFileOffsetsPtr      = g_FileXaLoc;
            xaFileOffsetTargetPtr = &xaFileOffsetsPtr[gSDXATable[xaAudioIdx].xaFileIdx];
            xaFileOffset          = *xaFileOffsetTargetPtr;
            xaFileOffset         += PREGAP + gSDXATable[xaAudioIdx].sector;

            g_Sd_XaAudioPlayTracking.xaAudioLength = gSDXATable[xaAudioIdx].audioLength + 32;

            g_Sd_AudioStreamingStates.xaPreLoadState = 5;
            g_Sd_XaCdLocation.sector                 = itob(xaFileOffset % 75);
            xaFileOffset                            /= 75;
            g_Sd_XaCdLocation.second                 = itob(xaFileOffset % 60);
            xaFileOffset                            /= 60;
            g_Sd_XaCdLocation.minute                 = itob(xaFileOffset);
            break;

        case 5:
            if (!Sd_CdPrimitiveCmdTry(CdlSeekL, &g_Sd_XaCdLocation, NULL))
            {
                g_Sd_AudioWork.cdErrorCount              = 0;
                g_Sd_AudioStreamingStates.xaPreLoadState = 6;
            }
            break;

        case 6:
            if (!Sd_CdPrimitiveCmdTry(CdlPause, NULL, NULL))
            {
                g_Sd_AudioStreamingStates.xaPreLoadState = 0;
                g_Sd_XaTaskPending                       = false;
                Sd_TaskPoolUpdate();
                g_Sd_AudioWork.cdErrorCount              = 0;
            }
            break;

        default:
            break;
    }
}

/** @brief Prepares the audio load of the XA set in `gSDXATable`. */
static void Sd_XaAudioStopTaskAdd(void) // 0x8004760C
{
    Sd_TaskPoolAdd(2);
    g_Sd_AudioWork.isXaNotPlaying = true;
}

/** @brief Stops the streaming of the currently loaded XA audio in memory. */
static void Sd_XaAudioStop(void) // 0x80047634
{
    g_Sd_AudioWork.isXaStopping = true;

    switch (g_Sd_AudioStreamingStates.xaStopState)
    {
        case XaStopState_FadeOut:
            Sd_XaVolumeSet(gSDVolConfig.volumeXa, gSDVolConfig.volumeXa);
            gSDVolConfig.volumeXa     -= 24;
            gSDVolConfig.volumeXaToSet = gSDVolConfig.volumeXa;

            if (gSDVolConfig.volumeXa < 2)
            {
                g_Sd_AudioStreamingStates.xaStopState = XaStopState_Mute;
            }
            break;

        case XaStopState_Mute:
            gSDVolConfig.volumeXa      = 0;
            gSDVolConfig.volumeXaToSet = 0;

            Sd_XaVolumeSet(0, 0);
            SdSetSerialAttr(0, 0, 0);

            g_Sd_AudioStreamingStates.xaStopState = XaStopState_PauseDisc;
            break;

        case XaStopState_PauseDisc:
            if (!Sd_CdPrimitiveCmdTry(CdlPause, NULL, NULL))
            {
                g_Sd_AudioWork.cdErrorCount           = 0;
                g_Sd_AudioStreamingStates.xaStopState = XaStopState_Cleanup;
            }

            g_Sd_AudioWork.cdErrorCount++;
            break;

        case XaStopState_Cleanup:
            g_Sd_AudioWork.isXaStopping           = false;
            g_Sd_AudioWork.xaAudioIdx             = 0;
            g_Sd_AudioStreamingStates.xaStopState = XaStopState_FadeOut;

            if (g_Sd_AudioWork.bgmFadeSpeed == 0)
            {
                gSDVolConfig.volumeBgm = 40;
            }

            Sd_TaskPoolUpdate();
            g_Sd_AudioWork.cdErrorCount = 0;
            break;

        default:
            break;
    }
}

// ========================================
// VOLUME SET
// ========================================

void Sd_GlobalVolumeSet(u8 xaVol, s16 bgmVol, u8 seVol) // 0x80047798
{
    gSDVolConfig.globalVolumeXa  = xaVol;
    gSDVolConfig.globalVolumeBgm = bgmVol;
    gSDVolConfig.globalVolumeSe  = seVol;

    if (g_Sd_AudioWork.midiChannelsVolTask != 0)
    {
        Sd_BgmVolumeSet(gSDVolConfig.volumeBgmToSet, gSDVolConfig.volumeBgmToSet);
    }

    if (g_Sd_AudioWork.xaAudioIdx != 0)
    {
        Sd_XaVolumeSet(gSDVolConfig.volumeXa, gSDVolConfig.volumeXa);
    }
}

void Sd_BgmVolumeSet(s16 volumeLeft, s16 volumeRight) // 0x80047808
{
    SdSeqSetVol(0, (volumeLeft * gSDVolConfig.globalVolumeBgm) >> 7, (volumeRight * gSDVolConfig.globalVolumeBgm) >> 7);
}

void Sd_XaVolumeSet(s16 volumeLeft, s16 volumeRight) // 0x80047860
{
    SdSetSerialVol(0, (volumeLeft * gSDVolConfig.globalVolumeXa) >> 7, (volumeRight * gSDVolConfig.globalVolumeXa) >> 7);
}

s16 Sd_SeVolumeGet(s16 arg0) // 0x800478B8
{
    return (arg0 * gSDVolConfig.globalVolumeSe) >> 7;
}

// ========================================
// TASK POOL HANDLING
// ========================================

/** Updates and add tasks to a task pool. */
void Sd_TaskPoolAdd(u8 task) // 0x800478DC
{
    static s32 i;
    static s32 y;

    // If `task` is 2, shift field next to element containing value that matches 1 in `g_Sd_TaskPool`.
    if (task == 2)
    {
        for (i = 1; i < (TASK_POOL_SIZE - 2); i++)
        {
            if (g_Sd_TaskPool[i] == 1)
            {
                for (y = i; y < (TASK_POOL_SIZE - 2); y++)
                {
                    g_Sd_TaskPool[y] = g_Sd_TaskPool[y + 1];
                }

                g_Sd_TaskPool[31]  = 0;
                g_Sd_XaTaskPending = false;
            }
        }
    }

    // Shift field next to element containing value that matches `task` in `g_Sd_TaskPool`.
    for (i = 1; i < (TASK_POOL_SIZE - 2); i++)
    {
        if (g_Sd_TaskPool[i] == task)
        {
            for (y = i; y < (TASK_POOL_SIZE - 2); y++)
            {
                g_Sd_TaskPool[y] = g_Sd_TaskPool[y + 1];
            }

            g_Sd_TaskPool[31] = 0;
        }
    }

    // If `g_Sd_TaskPool` field is empty, assign value of `task`.
    for (i = 0; i < (TASK_POOL_SIZE - 1); i++)
    {
        if (g_Sd_TaskPool[i] == 0)
        {
            g_Sd_TaskPool[i] = task;
            break;
        }
    }

    g_Sd_CurrentTask = g_Sd_TaskPool[0];
}

/** @brief Updates a task pool by shifting a field. */
static void Sd_TaskPoolUpdate(void) // 0x80047A70
{
    static s32 i;
    static s32 __pad_800C15E4;

    if (g_Sd_TaskPool[0] != 0)
    {
        for (i = 0; i < (TASK_POOL_SIZE - 1); i++)
        {
            g_Sd_TaskPool[i] = g_Sd_TaskPool[i + 1];
        }

        g_Sd_TaskPool[31] = 0;
    }
}

// ========================================
// REVERB HANDLING
// ========================================

void Sd_SetReverbDepth(u8 depth) // 0x80047AD0
{
    s32 depthCpy;

    gSDVolConfig.reverbDepth = depth;

    depthCpy = depth;
    SdUtSetReverbDepth(depthCpy, depthCpy);
}

void Sd_SetReverbEnable(s32 mode) // 0x80047AFC
{
    SdSetSerialAttr(0, 1, mode);
}

// ========================================
// LOAD VAB FILE
// ========================================

void Sd_VabLoad_TaskAdd(s32 task) // 0x80047B24
{
    if (g_Sd_AudioWork.xaAudioIdx != 0)
    {
        Sd_TaskPoolAdd(2);
    }

    g_Sd_DataLoadAttempts = 0;
    Sd_TaskPoolAdd(task);
    g_Sd_AudioWork.isAudioLoading = true;
}

static void Sd_VabLoad(void) // 0x80047B80
{
    u8 depth;
    u8 task;

    switch (g_Sd_AudioStreamingStates.audioLoadState)
    {
        case AudioLoadState_Reset:
            task               = g_Sd_TaskPool[0];
            g_Sd_VabTargetLoad = &g_AudioData[task - 160];
            g_Sd_AudioType     = g_Sd_VabTargetLoad->typeIdx;

            // If audio being loaded isn't BASE.VAB or KDT file.
            if (g_Sd_AudioType != 0)
            {
                if (g_Sd_AudioWork.activeVabAudioIdx[g_Sd_AudioType - 1] == task)
                {
                    g_Sd_AudioStreamingStates.audioLoadState = AudioLoadState_Reset;
                    g_Sd_AudioWork.isAudioLoading            = false;
                    Sd_TaskPoolUpdate();
                    break;
                }

                g_Sd_AudioWork.activeVabAudioIdx[g_Sd_AudioType - 1] = task;
            }

            // Ambient sounds.
            if (task >= 170 && task <= 204)
            {
                depth = g_Sd_ReverbDepths[task - 170];
                if (depth != gSDVolConfig.reverbDepth)
                {
                    Sd_SetReverbDepth(depth);
                }
            }

            g_Sd_AudioStreamingStates.audioLoadState = AudioLoadState_Stop;
            break;

        case AudioLoadState_Stop:
            Sd_VabLoad_TypeClear();
            break;

        case AudioLoadState_SetOff:
            Sd_VabLoad_OffSet();
            break;

        case AudioLoadState_LoadFile:
            Sd_VabLoad_FileLoad();
            break;

        case AudioLoadState_CheckLoad:
            Sd_VabLoad_OffVagDataSet();
            break;

        case AudioLoadState_Move:
            Sd_VabLoad_VagDataMove();
            break;

        case AudioLoadState_SetNext:
            Sd_VabLoad_OffVagNextDataSet();
            break;

        case AudioLoadState_MoveNext:
            Sd_VabLoad_NextVagDataMove();
            break;

        case AudioLoadState_MoveLast:
            Sd_VabLoad_LastVagDataMove();
            break;

        case AudioLoadState_Finalize:
            Sd_VabLoad_Finalization();
            break;

        default:
            break;
    }
}

static void Sd_VabLoad_TypeClear(void) // 0x80047D1C
{
    g_Sd_FileDataTransferred = 0;
    SdVabClose(g_Sd_AudioType);
    g_Sd_AudioStreamingStates.audioLoadState = AudioLoadState_SetOff;
}

/** @brief Sets the reader offset to the target VAB position. */
static void Sd_VabLoad_OffSet(void) // 0x80047D50
{
    CdlLOC sp10;

    if (!Sd_CdPrimitiveCmdTry(CdlSetloc, (u8*)CdIntToPos(g_Sd_VabTargetLoad->fileOffset + (g_Sd_FileDataTransferred  / 2048), &sp10), 0))
    {
        g_Sd_AudioStreamingStates.audioLoadState = AudioLoadState_LoadFile;
    }
}

static void Sd_VabLoad_FileLoad(void) // 0x80047DB0
{
    if (CdSync(1, 0) == CdlComplete)
    {
        if (g_Sd_VabTargetLoad->fileSize < VAB_BUFFER_LIMIT)
        {
            CdRead((g_Sd_VabTargetLoad->fileSize + 2047) / 2048, CD_ADDR_0, 128);
        }
        else
        {
            CdRead(25, CD_ADDR_0, 128);
        }

        // @hack
        if (g_Sd_AudioStreamingStates.audioLoadState != AudioLoadState_Reset)
        {
            char unk = -unk;
        }

        g_Sd_AudioStreamingStates.audioLoadState = AudioLoadState_CheckLoad;
        g_Sd_AudioWork.cdErrorCount              = 0;
    }

    g_Sd_AudioWork.cdErrorCount++;
}

/** @brief Sets the reader offset to the VAG data position. */
static void Sd_VabLoad_OffVagDataSet(void) // 0x80047E3C
{
    s32 i;
    u8* ptr0;
    u8* ptr1;

    if (CdReadSync(1, NULL) == 0)
    {
        ptr1 = (u8*)CD_ADDR_0;
        ptr0 = g_Sd_VabBuffers[g_Sd_AudioType];

        for (i = 0; i < g_Sd_VabTargetLoad->vagDataOffset; i++)
        {
            *ptr0++ = *ptr1++;
        }

        SdVabOpenHeadSticky(g_Sd_VabBuffers[g_Sd_AudioType], g_Sd_AudioType, D_800A9FDC[g_Sd_AudioType]);
        g_Sd_AudioStreamingStates.audioLoadState = AudioLoadState_Move;
    }

    g_Sd_AudioWork.cdErrorCount++;
}

/** @brief Moves VAG data from the temporary file location to the indicated `g_Sd_VabBuffers` buffer.
 * If the file is larger than `VAB_BUFFER_LIMIT`, it loops to move remaining bytes beyond this size.
 */
static void Sd_VabLoad_VagDataMove(void) // 0x80047F18
{
    s32  dataMoveCheck;
    s32* ptr;

    if (g_Sd_VabTargetLoad->fileSize < VAB_BUFFER_LIMIT)
    {
        dataMoveCheck = SdVabTransBody((u8*)CD_ADDR_0 + g_Sd_VabTargetLoad->vagDataOffset, g_Sd_AudioType);
        ptr           = &g_Sd_VabTargetLoad->fileSize;

        g_Sd_FileDataTransferred                 = *ptr;
        g_Sd_AudioStreamingStates.audioLoadState = AudioLoadState_Finalize;
    }
    else
    {
        dataMoveCheck = SdVabTransBodyPartly((u8*)CD_ADDR_0 + g_Sd_VabTargetLoad->vagDataOffset, VAB_BUFFER_LIMIT - g_Sd_VabTargetLoad->vagDataOffset, g_Sd_AudioType);

        g_Sd_FileDataTransferred                 = VAB_BUFFER_LIMIT;
        g_Sd_AudioStreamingStates.audioLoadState = AudioLoadState_SetNext;
    }

    if (dataMoveCheck == NO_VALUE && g_Sd_DataLoadAttempts < AUDIO_DATA_LOAD_ATTEMPTS)
    {
        g_Sd_DataLoadAttempts++;
        g_Sd_AudioStreamingStates.audioLoadState = AudioLoadState_Stop;
    }
}

static void Sd_VabLoad_OffVagNextDataSet(void) // 0x80048000
{
    s32     i;
    CdlLOC  cdLocArg;
    CdlLOC* cdLocRes;

    if (SdVabTransCompleted(0) == 1)
    {
        i        = g_Sd_VabTargetLoad->fileOffset + ((g_Sd_FileDataTransferred + 2047) / 2048);
        cdLocRes = CdIntToPos(i, &cdLocArg);

        if (!Sd_CdPrimitiveCmdTry(CdlSetloc, (u8*)cdLocRes, 0))
        {
            g_Sd_AudioStreamingStates.audioLoadState = AudioLoadState_MoveNext;
        }
    }
}

static void Sd_VabLoad_NextVagDataMove(void) // 0x8004807C
{
    u32 remainingData;

    if (CdSync(1, 0) != CdlComplete)
    {
        return;
    }

    remainingData = g_Sd_VabTargetLoad->fileSize - g_Sd_FileDataTransferred;
    if (remainingData < VAB_BUFFER_LIMIT)
    {
        CdRead(((remainingData + 2047) / 2048), CD_ADDR_0, 128);
    }
    else
    {
        CdRead(25, CD_ADDR_0, 128);
    }

    g_Sd_AudioStreamingStates.audioLoadState = AudioLoadState_MoveLast;
}

static void Sd_VabLoad_LastVagDataMove(void) // 0x800480FC
{
    s32 dataMoveCheck;
    u32 remainingData;

    if (CdReadSync(1, 0) != 0)
    {
        return;
    }

    remainingData = g_Sd_VabTargetLoad->fileSize - g_Sd_FileDataTransferred;
    if (remainingData < VAB_BUFFER_LIMIT)
    {
        dataMoveCheck                            = SdVabTransBodyPartly((u8*)CD_ADDR_0, remainingData, g_Sd_AudioType);
        g_Sd_FileDataTransferred                 = g_Sd_VabTargetLoad->fileSize;
        g_Sd_AudioStreamingStates.audioLoadState = AudioLoadState_Finalize;
    }
    else
    {
        dataMoveCheck                            = SdVabTransBodyPartly((u8*)CD_ADDR_0, VAB_BUFFER_LIMIT, g_Sd_AudioType);
        g_Sd_FileDataTransferred                += VAB_BUFFER_LIMIT;
        g_Sd_AudioStreamingStates.audioLoadState = AudioLoadState_SetNext;
    }

    if (dataMoveCheck == NO_VALUE && g_Sd_DataLoadAttempts < AUDIO_DATA_LOAD_ATTEMPTS)
    {
        g_Sd_DataLoadAttempts++;
        g_Sd_AudioStreamingStates.audioLoadState = AudioLoadState_Stop;
    }
}

static void Sd_VabLoad_Finalization(void) // 0x800481F8
{
    if (SdVabTransCompleted(0) != 1)
    {
        return;
    }

    g_Sd_AudioStreamingStates.audioLoadState = AudioLoadState_Reset;
    g_Sd_AudioWork.cdErrorCount              = 0;
    g_Sd_AudioWork.isAudioLoading            = false;
    Sd_TaskPoolUpdate();
}

// ========================================
// LOAD KDT FILE FOR MUSIC
// ========================================

void Sd_KdtLoad_TaskAdd(u16 task) // 0x80048244
{
    if (g_Sd_AudioWork.bgmLoadedSongIdx == task)
    {
        return;
    }

    if (g_Sd_AudioWork.xaAudioIdx != 0)
    {
        Sd_TaskPoolAdd(2);
    }

    Sd_BgmStopTaskAdd();
    SD_Call((u16)(task + 173));
    Sd_TaskPoolAdd(task);

    g_Sd_DataLoadAttempts           = 0;
    g_Sd_AudioWork.bgmLoadedSongIdx = task;
    g_Sd_AudioWork.isAudioLoading   = true;
}

static void Sd_KdtLoad(void) // 0x800482D8
{
    switch (g_Sd_AudioStreamingStates.audioLoadState)
    {
        case AudioLoadState_Reset:
            g_Sd_KdtTargetLoad                       = &g_AudioData[54 + g_Sd_TaskPool[0]];
            g_Sd_AudioType                           = g_Sd_KdtTargetLoad->typeIdx;
            g_Sd_AudioStreamingStates.audioLoadState = AudioLoadState_Stop;
            break;

        case AudioLoadState_Stop:
            Sd_KdtLoad_StopSeq();
            break;

        case AudioLoadState_SetOff:
            Sd_KdtLoad_OffSet();
            break;

        case AudioLoadState_LoadFile:
            Sd_KdtLoad_FileLoad();
            break;

        case AudioLoadState_CheckLoad:
            Sd_KdtLoad_LoadCheck();
            break;

        default:
            break;
    }
}

static void Sd_KdtLoad_StopSeq(void) // 0x8004839C
{
    Sd_BgmStopStep();
    SdSeqClose(g_Sd_AudioType);

    g_Sd_AudioStreamingStates.audioLoadState = AudioLoadState_SetOff;
}

static void Sd_KdtLoad_OffSet(void) // 0x800483D4
{
    CdlLOC cdLoc;

    if (!Sd_CdPrimitiveCmdTry(CdlSetloc, (u8*)CdIntToPos(g_Sd_KdtTargetLoad->fileOffset, &cdLoc), 0))
    {
        g_Sd_AudioStreamingStates.audioLoadState = AudioLoadState_LoadFile;
    }
}

static void Sd_KdtLoad_FileLoad(void) // 0x80048424
{
    if (CdSync(1, 0) == 2)
    {
        CdRead((g_Sd_KdtTargetLoad->fileSize + 2047) / 2048, FS_BUFFER_1, 128);

        g_Sd_AudioStreamingStates.audioLoadState = AudioLoadState_CheckLoad;
        g_Sd_AudioWork.cdErrorCount              = 0;
    }

    g_Sd_AudioWork.cdErrorCount++;
}

static void Sd_KdtLoad_LoadCheck(void) // 0x80048498
{
    s32 i;
    u8* ptr0;
    u8* ptr1;

    if (CdReadSync(1, NULL) == 0)
    {
        ptr1 = (u8*)FS_BUFFER_1;
        ptr0 = g_Sd_KdtBuffer[g_Sd_AudioType];

        for (i = 0; i < g_Sd_KdtTargetLoad->fileSize; i++)
        {
            *ptr0++ = *ptr1++;
        }

        i = SdSeqOpen(g_Sd_KdtBuffer[g_Sd_AudioType], 3);
        if (i == NO_VALUE && g_Sd_DataLoadAttempts < AUDIO_DATA_LOAD_ATTEMPTS)
        {
            g_Sd_DataLoadAttempts++;
            g_Sd_AudioStreamingStates.audioLoadState = AudioLoadState_Stop;
        }
        else
        {
            g_Sd_AudioStreamingStates.audioLoadState = AudioLoadState_Reset;
            g_Sd_AudioWork.isAudioLoading            = false;

            Sd_TaskPoolUpdate();
        }

        g_Sd_AudioWork.cdErrorCount = 0;
    }

    g_Sd_AudioWork.cdErrorCount++;
}

/** @note All these functions are also nullsub in the Jan 16 Demo, Nov 24 Demo, and 98 Preview,.
 * Additionally, `func_800485C0` doesn't exist in the Jan 16 Demo.
 */

void func_800485B0(s16 arg0, u8 arg1, u8 arg2, s16 arg3, s16 arg4) {} // 0x800485B0

void func_800485B8(s32 arg0, u8 arg1, u32 arg2) {} // 0x800485B8

void func_800485C0(s32 idx) // 0x800485C0
{
    g_AudioPlayingIdxList[idx] = 0;
}

void Sd_TaskPoolExecute(void) // 0x800485D8
{
    g_Sd_CurrentTask = g_Sd_TaskPool[0];
    switch (g_Sd_CurrentTask)
    {
        case 0:
            break;

        case 1:
            Sd_XaAudioPlay();
            break;

        case 2:
            Sd_XaAudioStop();
            break;

        case 6:
            Sd_XaPreLoadAudio();
            break;

        case 7:
            func_80046A70();
            break;

        case 8:
            Sd_BgmStop();
            break;

        default:
            if (g_Sd_CurrentTask >= 160)
            {
                Sd_VabLoad();
            }
            else if (g_Sd_CurrentTask >= 32)
            {
                Sd_KdtLoad();
            }
            else
            {
                Sd_TaskPoolUpdate();
            }
            break;
    }

    if (g_Sd_AudioWork.xaAudioIdx != 0)
    {
        g_Sd_XaAudioPlayTracking.xaAudioPlayCurrentTime = VSync(SyncMode_Count) -
                                                          g_Sd_XaAudioPlayTracking.vSyncTimeSinceBoot;
    }

    // Fade out background music.
    if (g_Sd_AudioWork.bgmFadeSpeed != 0)
    {
        g_Sd_AudioWork.midiChannelsVolTask = NO_VALUE;

        if (gSDVolConfig.volumeBgmToSet <= 0)
        {
            gSDVolConfig.volumeBgmToSet = 0;
            Sd_BgmStopStep();
        }
        else
        {
            gSDVolConfig.volumeBgmToSet -= g_Sd_AudioWork.bgmFadeSpeed;

            if ((gSDVolConfig.volumeBgmToSet << 16) <= 0)
            {
                gSDVolConfig.volumeBgmToSet = 0;
                Sd_BgmStopStep();
            }
        }

        gSDVolConfig.volumeBgm = gSDVolConfig.volumeBgmToSet;

        Sd_BgmVolumeSet(gSDVolConfig.volumeBgmToSet, gSDVolConfig.volumeBgmToSet);
    }
    else if (gSDVolConfig.volumeBgm != gSDVolConfig.volumeBgmToSet)
    {
        if (gSDVolConfig.volumeBgmToSet < gSDVolConfig.volumeBgm)
        {
            gSDVolConfig.volumeBgmToSet++;
            if (ABS(gSDVolConfig.volumeBgmToSet - gSDVolConfig.volumeBgm) < 2)
            {
                gSDVolConfig.volumeBgmToSet = gSDVolConfig.volumeBgm;
            }
        }
        else
        {
            gSDVolConfig.volumeBgmToSet--;
            if (ABS(gSDVolConfig.volumeBgmToSet - gSDVolConfig.volumeBgm) < 2)
            {
                gSDVolConfig.volumeBgmToSet = gSDVolConfig.volumeBgm;
            }
        }

        Sd_BgmVolumeSet(gSDVolConfig.volumeBgmToSet, gSDVolConfig.volumeBgmToSet);
    }

    if (g_Sd_XaAudioPlayTracking.xaAudioPlayCurrentTime > g_Sd_XaAudioPlayTracking.xaAudioLength)
    {
        if (g_Sd_CurrentTask == 0)
        {
            if (g_Sd_AudioWork.isXaNotPlaying == false)
            {
                Sd_TaskPoolAdd(2);
            }

            g_Sd_XaAudioPlayTracking.vSyncTimeSinceBoot     = VSync(SyncMode_Count);
            g_Sd_XaAudioPlayTracking.xaAudioPlayCurrentTime = 0;
        }
    }

    // Slowly fade in/out game audio based if `g_Sd_AudioWork.muteGame` is enabled.
    if (g_Sd_AudioWork.muteGame == true)
    {
        if (gSDVolConfig.globalVolumeGame > 0)
        {
            gSDVolConfig.globalVolumeGame -= 8;
            if ((gSDVolConfig.globalVolumeGame << 16) <= 0)
            {
                gSDVolConfig.globalVolumeGame = 0;
            }

            SdSetMVol(gSDVolConfig.globalVolumeGame, gSDVolConfig.globalVolumeGame);
        }
    }
    else
    {
        if (gSDVolConfig.globalVolumeGame < (OPT_SOUND_VOLUME_MAX - 1))
        {
            gSDVolConfig.globalVolumeGame += 4;
            if (gSDVolConfig.globalVolumeGame >= (OPT_SOUND_VOLUME_MAX - 1))
            {
                gSDVolConfig.globalVolumeGame = OPT_SOUND_VOLUME_MAX - 1;
            }

            SdSetMVol(gSDVolConfig.globalVolumeGame, gSDVolConfig.globalVolumeGame);
        }
    }

    // Reset audio streaming system if failed.
    if (g_Sd_AudioWork.cdErrorCount > CD_ERROR_LIMIT)
    {
        CdReset(0);
        CdControlB(CdlNop, NULL, NULL);

        if (g_Sd_AudioStreamingStates.audioLoadState != AudioLoadState_Reset)
        {
            g_Sd_AudioStreamingStates.audioLoadState = AudioLoadState_Stop;
        }

        g_Sd_AudioStreamingStates.xaLoadState    = XaLoadState_Initialize;
        g_Sd_AudioStreamingStates.xaStopState    = XaStopState_FadeOut;
        g_Sd_AudioStreamingStates.xaPreLoadState = 0;
        g_Sd_AudioWork.cdErrorCount              = 0;
    }
}

u8 Sd_CdPrimitiveCmdTry(s32 com, u8* param, u8* res) // 0x80048954
{
    u8 syncRes;
    u8 comCpy;

    comCpy = com;

    if (CdSync(1, &syncRes) == CdlComplete && CdControl(comCpy, param, res))
    {
        g_Sd_AudioWork.cdErrorCount = 0;
        return false;
    }

    g_Sd_AudioWork.cdErrorCount++;
    if (g_Sd_AudioWork.cdErrorCount > CD_ERROR_LIMIT)
    {
        CdReset(0);
        CdControlB(CdlNop, NULL, NULL);
        VSync(SyncMode_Wait3);

        if (g_Sd_AudioStreamingStates.audioLoadState != AudioLoadState_Reset)
        {
            g_Sd_AudioStreamingStates.audioLoadState = AudioLoadState_Stop;
        }

        g_Sd_AudioStreamingStates.xaLoadState    = XaLoadState_Initialize;
        g_Sd_AudioStreamingStates.xaStopState    = XaStopState_FadeOut;
        g_Sd_AudioStreamingStates.xaPreLoadState = 0;
        g_Sd_AudioWork.cdErrorCount              = 0;
    }

    return true;
}

#undef CD_ERROR_LIMIT
#undef TASK_POOL_SIZE
#undef AUDIO_DATA_LOAD_ATTEMPTS
#undef VAB_BUFFER_LIMIT
#undef PREGAP
