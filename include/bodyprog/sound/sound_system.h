#ifndef _BODYPROG_SOUND_SOUNDSYSTEM_H
#define _BODYPROG_SOUND_SOUNDSYSTEM_H

/** @brief Game-specific sound system and not part of the sound library (LIBSD) itself.
 * Specifically, handles audio file stream processing and some general sound effects.
 *
 * @note Name deobfuscation:
 * Other KCET PS1 and early PS2 games (including SH2 PS2 version) inherited or shared
 * this same code among them. Other decompiled KCET as it is Vandal Hearts and Castlevania: SOTN
 * also featured this same code, but as expected with servere differences due the game target
 * and timestamps.
 *
 * `Tokimeki Memorial ~Forever With You~` and `Konami International Rally Championship` symbols
 * indicate what the decomp calls `commands` are actually `tasks`.
 * TM suggests that at some point, they were called `events`.
 */

/** @note MIDI Channels/BGM clarification:
 * One of the most obfuscated or hardest to points to understand of the entire system is the purpose
 * and usage of `g_Sd_AudioWork.midiChannelsVolTask`.
 *
 * The initialization of BGM consist of 3 steps:
 * * Load KDT file containing music sheet.
 * * Load VAB file containing music key sounds.
 * * Setting up volumes of SPU's MIDI channels for handling music layers.
 *
 * The first two steps are very straight forward. In the circumstance of a new song being play
 * the game sends a new task to `SD_Call` which directly loads the KDT file and then sets a task
 * to load the pair VAB file.
 *
 * This process completely changes when it comes to assign volumes for MIDI channels in order
 * to make the BGM layering system.
 * The way the game handle that is through a 2D array that specify what channels to use to represent
 * layers on specific songs. The array is `g_Sd_SongsChannelsForLayers` and more information is available
 * on it's description.
 *
 * The main confusing part comes here. The way the game determines to what row of `g_Sd_SongsChannelsForLayers`
 * access is by calling `Sd_BgmChannelSet` which uses the current playing song index to access to a list task commands
 * set in `g_BgmChannelSetTask` and assign the task to `SD_Call` that range from `0x301` to `0x328`.
 * The fundamental parts of that task handling process are: First it gives `g_Sd_AudioWork.midiChannelsVolTaskToSet`
 * the task ID, then it passed to `g_Sd_AudioWork.midiChannelsVolTask` and lastly `Sd_MidiChannelsVolumeSet` (mainly
 * used by `Bgm_LayersUpdate`) locally strips the task ID base (0x300) and the remaining value is used to access to
 * a row of `g_Sd_SongsChannelsForLayers`.
 *
 * This is an extremely simplified explanation of how it works, but process has two noticeable odd code decisions
 * that makes this part of the system hard to comprehend at first glance. Here are those odd decisions:
 * * The base from the task ID that set the update MIDI channels config is never strip down leaving values that are
 * * bigger than the array they are meant to be used, what the game does is stripping the base (0x300) by casting
 * * the value as a `s8` instead of using `& 0xFF` which would look more comprehensible. This can be noted at
 * * `Sd_MidiChannelVolumeGet` and `Sd_MidiChannelVolumeSet`.
 * * `g_Sd_AudioWork.midiChannelsVolTaskToSet` may be useless. This is a failsafe to avoid running a same task twice,
 * * however, this could either be an unnecessary failsafe as this could only ever happen if the console suddenly start
 * * misbehaving or this is a particular fix or failsafe for the Nowhere section of the game as for some reason there
 * * is another function (`sharedFunc_800D0110_7_s00`) that calls `Sd_BgmChannelSet`.
 *
 * Additional note: The OPM 16 build (earliest build available) shows that the code was completely different and
 * actually strip the base and apparently there wasn't another check.
 */

// ==============
// HELPER MACROS
// ==============

/** @brief Packs an audio type (VAB ID) and program index into a single 16-bit value.
 *
 * This macro replicates the encoding used in the third field of `s_VabInfo`. 
 * The resulting value is passed to `SdVoKeyOn` via the `vab_pro` argument, where the function extracts:
 * - The Program Index: derived via `(vab_pro & 0x7F)`.
 * - The VAB ID: derived via `vab_pro >> 8`.
 *
 * For example, a value of 516 (0x0204) results in a program index of 4 and a VAB ID of 2. Similarly, 256 (0x0100) and
 * 514 (0x0202) follow this bit-packed structure.
 *
 * @param audioType ID used to index `vab_h`, which manages VAG data allocation in SPU memory.
 * @param progIdx Specific program index within the VAB attribute table.
 * @return Packed audio type and program index.
 */
#define TYPE_AND_PROG_SFX(audioType, progIdx) \
    (((audioType) << 8) + (progIdx))

#define SD_TASK_CHANNEL_SET(idx) \
    (0x300 + (idx))

// ======
// ENUMS
// ======

/** @brief Audio modes. */
typedef enum _AudioMode
{
    AudioMode_Mono   = 1,
    AudioMode_Stereo = 2
} e_AudioMode;

/** @brief Audio types. */
typedef enum _AudioType
{
    AudioType_MusicSheet    = 0, // KTD file.
    AudioType_BaseAudio     = 0,
    AudioType_Weapon        = 1,
    AudioType_Ambient       = 2,
    AudioType_MusicKeys     = 3  // VAB file containing audio keys for a loaded KTD file.
} e_AudioType;

/** @brief VAB audio load states. */
typedef enum _AudioLoadState
{
    AudioLoadState_Reset     = 0,
    AudioLoadState_Stop      = 1,
    AudioLoadState_SetOff    = 2,
    AudioLoadState_LoadFile  = 3,
    AudioLoadState_CheckLoad = 4,
    AudioLoadState_Move      = 5,
    AudioLoadState_SetNext   = 6,
    AudioLoadState_MoveNext  = 7,
    AudioLoadState_MoveLast  = 8,
    AudioLoadState_Finalize  = 9
} e_AudioLoadState;

/** @brief Audio streaming states. Returned by `Sd_AudioStreamingCheck`. */
typedef enum _AudioStreamingState
{
    AudioStreamingState_None             = 0,
    AudioStreamingState_XaPlaying        = 1,
    AudioStreamingState_VabPlaying       = 2,
    AudioStreamingState_XaLoading        = 3,
    AudioStreamingState_XaLoadPending    = 4,
    AudioStreamingState_AudioTaskPending = 5
} e_AudioStreamingState;

/** @brief XA load states. */
typedef enum _XaLoadState
{
    XaLoadState_Initialize    = 0,
    XaLoadState_SetMode       = 1,
    XaLoadState_PrepareFilter = 2,
    XaLoadState_SetFilter     = 3,
    XaLoadState_CalculateLba  = 4,
    XaLoadState_Seek          = 5,
    XaLoadState_StartRead     = 6,
    XaLoadState_EnableAudio   = 7
} e_XaLoadState;

/** @brief XA stop states. */
typedef enum _XaStopState
{
    XaStopState_FadeOut   = 0,
    XaStopState_Mute      = 1,
    XaStopState_PauseDisc = 2,
    XaStopState_Cleanup   = 3
} e_XaStopState;

// ========
// STRUCTS
// ========

#ifdef BSS_HACK_SD_CALL_C
// @hack This union is required to build.
// This union is used by a variable that figure as one of the many .BSS static
// variables that were inserted through the `common` segment
// which at this point hasn't been capable of replicate it's behaviour.
// As this union uses `CdlFILTER` which is declared at `libcd.h` and we haven't
// decided if we are going to nest header files inside header files or not, this
// union is wrapped inside `BSS_HACK_SD_CALL_C` to only be declared at `sound/sd_call.c`.

// Used for loading XA files.
typedef union _Sd_XaCdlInfo
{
    u8        cdlMode;
    CdlFILTER cdlFilter;
} u_Sd_XaCdlInfo;
#endif

/** @brief Sound struct for tracking information of the game specific audio system. */
typedef struct _Sd_AudioWork
{
    /* 0x0 */   u16 cdErrorCount;             /** Counter for failed attempts when processing a primitive command. */
    /* 0x2 */   u16 xaAudioIdxCheck;          /** XA Audio index. Used to check if the file exists. */
    /* 0x4 */   u16 xaAudioIdx;               /** XA Audio index. Used to play the audio. */
    /* 0x6 */   u16 bgmLoadedSongIdx;         /** Index of the currently loaded music. */
    /* 0x8 */   u16 activeVabAudioIdx[3];     /** Stores the index of currently loaded VAB audio in `g_Sd_VabBuffers`, except of music notes. */
    /* 0xE */   u16 midiChannelsVolTask;      /** MIDI channel assignment for BGM layers.
                                                  Index of `g_Sd_SongsChannelsForLayers`. Requires to be wrapped by doing `& 0xFF` as the command base to assign the value is `0x300`. */
    /* 0x10 */  u16 midiChannelsVolTaskToSet; /** Temporarily stores the value intended for `midiChannelsVolTask`. Used to avoid running the same process twice for a same target MIDI channel configuration. */
    /* 0x11 */  u8  isStereoEnabled;          /** `bool` */
    /* 0x12 */  s8  isXaStopping;             /** `bool` | Set to `true` to stop an XA file in memory from playing, otherwise `false`. */
    /* 0x13 */  u8  bgmFadeSpeed;             /** Music fade speed. Range: `[0, 2]`, default: 0. */
    /* 0x14 */  u8  isAudioLoading;           /** `bool` | If a KDT or VAB file is being loaded. | Loading: `true`, Nothing loading: `false`, default: Nothing loading. */
    /* 0x15 */  u8  isXaNotPlaying;           /** `bool` | Playing: `false`, Nothing playing: `true`, default: Nothing playing. */
    /* 0x16 */  u8  muteGame;                 /** `bool` | Mutes the game. If the value is `true`, the whole game audio will progressively get lower
                                               * in volume until mute (the sounds will keep playing, but muted).
                                               */
} s_Sd_AudioWork;

/** @brief Sound struct for tracking VAB and XA audios and KDT file streaming. */
typedef struct _AudioStreamingStates
{
    /* 0x0 */ u8 audioLoadState; /** Load VAB audio and KDT music key notes state. */
    /* 0x1 */ u8 xaLoadState;    /** Load XA audio state. */
    /* 0x2 */ u8 xaStopState;    /** Stop XA audio streaming state. */
    /* 0x3 */ u8 xaPreLoadState; /** Prepare Load XA audio state. Positions the current read point to the one where the XA audio to load resides. */
} s_AudioStreamingStates;

/** @brief Game audio channels volume configuration struct. */
// @note Could the values of the fields be some sort of fractional value?
typedef struct _ChannelsVolumeController
{
    /* 0x0 */ s16 volumeXa;
    /* 0x2 */ s16 volumeXaToSet; // Sets value for `volumeXa`. See `Sd_XaAudioPlay`.
    /* 0x4 */ u16 volumeSe;
    /* 0x6 */ s16 volumeBgm;
    /* 0x8 */ s16 volumeBgmToSet;
    
    /** As main difference with previous volume controlers, this influence the overall audio behavior of the game.
     *
     * These are also the values modified in the configuration menu.
     */
    /* 0xA */ s16 globalVolumeGame; /** Volume of the entire game (not configurable). */
    /* 0xC */ u8  globalVolumeSe;   /** Global SE volume channel. */
    /* 0xD */ u8  globalVolumeBgm;  /** Global BGM volume channel. */
    /* 0xE */ u8  globalVolumeXa;   /** Global Voice volume channel (not configurable). */
    /* 0xF */ u8  reverbDepth;
} s_ChannelsVolumeController;

/** @brief Sound struct for tracking currently playing XA audio play time. */
typedef struct
{
    /* 0x0 */ q20_12 xaAudioLength;
    /* 0x4 */ q20_12 xaAudioPlayCurrentTime; /** Current audio timestamp. */
    /* 0x8 */ q20_12 vSyncTimeSinceBoot;     /** Time when the XA audio started playing. */
} s_XaAudioPlayTracking;

/** @brief Sound struct for currently used SFX.
 * @note It is recomended to read VAB file format documentation to fully understand this struct.
 */
typedef struct _VabPlayingInfo
{
    /* 0x0 */ u8  voiceIdx; /** Index of audio at PSX's "Voice" channels. */
    /* 0x1 */ s8  __pad_1;
    /* 0x2 */ s16 typeIdx; /** `e_AudioType` */
    /* 0x4 */ s16 progIdx;
    /* 0x6 */ s16 toneIdx;
    /* 0x8 */ s16 noteIdx;
    /* 0xA */ s16 pitch;
    /* 0xC */ s16 volumeLeft;
    /* 0xE */ s16 volumeRight;
} s_VabPlayingInfo;

/** @brief Sound struct VAB audio information.
 * @note It is recomended to read VAB file format documentation to fully understand this struct.
 */
typedef struct _VabInfo
{
    /* 0x0 */ u8  audioVabIdx; /** Index of audio inside VAB files. */
    /* 0x1 */ s8  __pad_1;
    /* 0x2 */ u16 vabProgIdx;  /** See `TYPE_AND_PROG_SFX`. */
    /* 0x4 */ u8  noteIdx;
    /* 0x5 */ s8  volumeMin;   /** Minimun volume required to play the audio. */
} s_VabInfo;

// TODO: Field with `_24` seems to be part of a thing related to how XA files work.
typedef struct _XaItemData
{
    /* 0x0    */ u8  xaFileIdx;
    /* 0x1    */ s8  __pad[3];
    /* 0x4+0  */ u32 sector      : 24;
    /* 0x4+24 */ u8  field_4_24  : 8; // Index.
    /* 0x8+0  */ u32 audioLength : 24;
    /* 0x8+24 */ u8  field_8_24  : 8; // Index.
} s_XaItemData;
STATIC_ASSERT_SIZEOF(s_XaItemData, 12);

// Used to store KDT and VAB data access.
typedef struct _AudioItemData
{
    /* 0x0 */ s8  typeIdx;       /** See `e_AudioType`. */
    /* 0x1 */ s8  __pad_1;
    /* 0x2 */ u16 vagDataOffset; /** Offset of VAG data in VAB files. */
    /* 0x4 */ u32 fileSize;      /** VAB/KDT file size. */
    /* 0x8 */ s32 fileOffset;    /** VAB/KDT audio offset in the file container. */
} s_AudioItemData;
STATIC_ASSERT_SIZEOF(s_AudioItemData, 12);

// ========
// GLOBALS
// ========

/** @brief Addresses where loaded VAB files are stored in memory.
 * 0 = Generic game sound file (BASE.VAB).
 * 1 = Weapon VAB.
 * 2 = Ambient VAB.
 * 3 = Music keys VAB.
 */
extern u8* g_Sd_VabBuffers[];

/** @brief Stores the currently loaded KDT file.
 * Declared as an array because of the way the code handles VAB file loading, as it expect to have a position.
 */
extern u8* g_Sd_KdtBuffer[];

extern s32 D_800A9FDC[];

/** @brief Stores information to access to all VAB/KDT files available. */
extern s_AudioItemData g_AudioData[];

/** @brief Stores all ambients reverb depth presets. */
extern u8 g_Sd_ReverbDepths[];

/** @brief Stores the index for MIDI channels intended to be use for playing layers of a song.
 *
 * The columns are the amount of song in the game, each row field defines what MIDI
 * channels should be use when using certain music layers.
 *
 * The index of every field from the rows represent each of the 16 MIDI channels
 * that can be use to reproduce music, the value that each field contains represent
 * the song's layer. This way using `Sd_MidiChannelsVolumeSet` set the volume of
 * all MIDI channels that matches the music layer specified in the first argument.
 *
 * @note Possibly a name that breaks our common naming conventions will be required for this?
 */
extern u8 g_Sd_SongsChannelsForLayers[41][16];

/** @brief Stores information to access to all XA audios available.
 *
 * @note Name from retrieved debug symbols.
 * In IRC .MAP file there is a variable named `gSDXATable` which serves the same
 * purpose as this variable. While the struct is missing the code hints that the
 * game counts the struct used by the variable have a size of 16/0x10 bytes, however,
 * from a quick code analysis the struct similarly to SH1 shares XA audio files sector
 * information also part of this information being stored in a bitfield of 24.
 */
extern s_XaItemData gSDXATable[];

/** @brief Stores information to access to all audios inside VAB files availables. */
extern s_VabInfo g_Vab_InfoTable[];

#ifndef PAD_HACK_IGNORE
#ifdef BSS_HACK_SD_CALL_C
// ====================
// GLOBALS (BSS; Hack; sd_call.c)
// ====================
// To match the order of the BSS segment, extern declarations
// are required in a predetermined order. All these variables
// are static.
// This is done until a way to replicate `common`
// segment behavior is found.

extern CdlLOC                     g_Sd_XaCdLocation;
extern s32                        __pad_bss_800C15EC;
extern u_Sd_XaCdlInfo             g_Sd_XaCdlInfo;
extern s16                        __pad_bss_800C15F2[2];
extern u16                        g_AudioPlayingIdxList[24];
extern s16                        g_AudioPlayingPitchList[24];
extern s_Sd_AudioWork             g_Sd_AudioWork;
extern s_AudioStreamingStates     g_Sd_AudioStreamingStates;
extern s32                        __pad_bss_800C1674;
extern s_ChannelsVolumeController gSDVolConfig; /** Original name. */
extern s_XaAudioPlayTracking      g_Sd_XaAudioPlayTracking;
extern s32                        __pad_bss_800C1694;
extern s_VabPlayingInfo           g_Sd_VabPlayingInfo;
extern u8                         g_Sd_TaskPool[];
extern s8                         D_800C16C8[];
extern u8                         g_Sd_AudioType;
extern char                       __pad_bss_800C37C9[3];
extern u32                        g_Sd_FileDataTransferred;
extern u8                         g_Sd_DataLoadAttempts;
extern char                       __pad_bss_800C37D1[3];
extern s_AudioItemData*           g_Sd_VabTargetLoad;
extern s_AudioItemData*           g_Sd_KdtTargetLoad;
extern u8                         g_Sd_XaTaskPending;
extern u8                         g_Sd_CurrentTask;
#endif
#endif

// ==========
// FUNCTIONS
// ==========

/** @brief Passes a task to the sound driver, playing SFX among other things.
 * Scratch: https://decomp.me/scratch/IniqJ
 *
 * @note Name from retrieved debug symbols.
 * Some PS1 and early PS2 KCET games and SH2 feature a function
 * with this name differing in case between each game.
 * * `Tokimeki Memorial ~Forever With You~` and `International Rally Championship` call it `SD_Call`.
 * * `Winning Eleven 6` calls it `SD_call`.
 * * `Silent Scope 3` and `Silent Hill 2` call it `sd_call`.
 *
 * Each game's implementation varies slightly depending on the requirements,
 * but in all instances, it's used to pass a command/task to the
 * audio streaming system. The most similar are IRC and SH2, while the most different
 * being SS3 and WE6.
 */
void SD_Call(u32 task);

/** @brief Checks if an audio file is loading, is going to be loaded, or an XA file is playing.
 * Depending of the audio file, it marks different numbers.
 * Scratch: https://decomp.me/scratch/41vuh
 *
 * @return `e_AudioStreamingState` with the current state.
 */
u8 Sd_AudioStreamingCheck(void);

/** @brief Gets the task index for current MIDI preset.
 * Scratch: https://decomp.me/scratch/ZiViR
 *
 * @return Task index for current MIDI preset.
 */
u16 Sd_MidiChannelTaskGet(void);

/** @brief Sound effect management and VAB + KDT file load.
 * Scratch: https://decomp.me/scratch/AA6ui
 *
 * @note Name from retrieved debug symbols.
 * `Tokimeki Memorial ~Forever With You~` and `International Rally Championship`
 * have a function with a similar purpose as this for handling
 * the loading of VAB files and some other audio system features.
 * Both cases are different. Notably in TM, it's used
 * to also handle XA files and more low-level features related the audio system
 * that neither SH1 nor IRC have.
 */
void SD_BranchCTRL(u16 task);

/** @brief Sets the audio system to stereo or mono.
 * Scratch: https://decomp.me/scratch/jtrpu
 */
void Sd_AudioSystemSet(u8 isStereo);

/** @brief Initalizes the entire audio system.
 *
 * @note Name from retrieved debug symbols.
 * In `International Rally Championship` can be found name as name
 * as `SD_Init`, however, there the it serves a three step functionallity
 * as it boot first part of the LIBSD system, then the game audio handler
 * code and other part of the LIBSD system, being more similar with the
 * second step call `SD_InitMyself`, on other case, `Tokimeki Memorial ~FWY~`
 * merges all this steps in their `SD_Init` counterpart.
 */
void SD_Init(void);

/** @brief Initalizes game audio system structs.
 *
 * @note Name from retrieved debug symbols.
 * Between IRC and TM ~FWY~ this function shares more similarity with IRC's
 * counterpart as there the function call as last one of the few things it
 * does is update the console audio channel volume, however, it still
 * being considerably different as there no other function is call.
 */
void SD_InitStruct(void);

/** @brief @unused Stops the main audio system. */
void Sd_AudioStop(void);

/** @brief Plays audio.
 *
 * @param sfxId `e_SfxId`.
 * @param balance Stereo balance.
 * @param vol Audio volume.
 * @return Audio index in PSX's "Voice" channels.
 */
u8 Sd_SfxPlay(u16 sfxId, q0_7 balance, q0_8 vol);

/** @brief Updates attributes from currently playing specified audio.
 *
 * @param sfxId `e_SfxId`.
 * @param balance Stereo balance.
 * @param vol Audio volume.
 * @param pitch Target pitch volume.
 */
void Sd_SfxAttributesUpdate(u16 sfxId, q0_7 balance, q0_8 vol, s8 pitch);

/** @brief Plays audio.
 *
 * @note The differences with `Sd_SfxPlay` is that this function has an option to
 * specify the audio pitch and doesn't register the playing sound in `g_AudioPlayingIdxList`.
 *
 * @param sfxId `e_SfxId`.
 * @param balance Stereo balance.
 * @param vol Audio volume.
 * @param pitch Target pitch volume.
 * @return Audio index in PSX's "Voice" channels.
 */
void Sd_SfxWithPitchPlay(u16 sfxId, q0_7 balance, q0_8 vol, s8 pitch);

/** @brief Stops the last VAB audio data playback. */
void Sd_LastSfxStop(void);

/** @brief Stops a specified VAB audio data playback. */
void Sd_SfxStop(u16 sfxId);

/** @brief Stops a specified VAB audio data playback according to an SFX ID. */
void Sd_SfxStopStep(u16 sfxId);

/** @brief Stops all VAB audio data playback. */
void Sd_AllSfxStop(void);

/** @brief Stops all VAB audio data playback with `Release Rate` mode enabled. */
void Sd_AllSfxWithRRStop(void);

/** Sound command func. Unknown category. */
void Sd_SetupBgmMidiChannels(u16 task);

/** Returns the current BGM audio channel volume based on the PSX's MIDI channel. Returns Q7 value?? */
u8 Sd_MidiChannelVolumeGet(u8 channelIdx);

/** Manipulates the BGM audio channel volume. */
void Sd_MidiChannelsVolumeSet(u8 channelIdx, u8 vol);

/** @brief Sets the volume for the global channels of the music, sound effects, and voices. */
void Sd_GlobalVolumeSet(u8 xaVol, s16 bgmVol, u8 seVol);

/** @brief Sets the volume for the channels of music. */
void Sd_BgmVolumeSet(s16 volumeLeft, s16 volumeRight);

/** @brief Sets the volume for the channels of voices. */
void Sd_XaVolumeSet(s16 volumeLeft, s16 volumeRight);

/** @brief Sets the volume for the channels of sound effects. */
s16 Sd_SeVolumeGet(s16 arg0);

void Sd_XaAudioPlayTaskAdd(u16 sfx);

s32 Sd_XaAudioLengthGet(s32 idx);

/** @brief Initializes the process to play XA audios in `gSDXATable`. */
void Sd_XaPreLoadAudioPreTaskAdd(u16 xaIdx);

void Sd_XaPreLoadAudioTaskAdd(s32 xaIdx);

void Sd_TaskPoolAdd(u8 task);

void Sd_SetReverbDepth(u8 depth);

void Sd_SetReverbEnable(s32 mode);

void Sd_VabLoad_TaskAdd(s32 task);

void Sd_KdtLoad_TaskAdd(u16 task);

/** Nullsub */
void func_800485B0(s16 arg0, u8 arg1, u8 arg2, s16 arg3, s16 arg4);

/** Nullsub */
void func_800485B8(s32 arg0, u8 arg1, u32 arg2);

void func_800485C0(s32 idx);

void Sd_TaskPoolExecute(void);

/** @brief Executes a new primitive command and checks the status against the previous.
 * If the previous primitive commands haven't completed, it starts
 * adding to `g_Sd_AudioWork.cdErrorCount` each time the process fails. When it
 * reaches 600 failed attemps, it restarts the CD-ROM system.
 */
u8 Sd_CdPrimitiveCmdTry(s32 com, u8* param, u8* res);

#endif
