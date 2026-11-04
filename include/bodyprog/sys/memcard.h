#ifndef _BODYPROG_MEMCARD_H
#define _BODYPROG_MEMCARD_H

// ==========
// CONSTANTS
// ==========

#define SAVEGAME_ENTRY_BUFFER_0 ((u8*)0x801E09E0) // Slot 1 savegame entry.
#define SAVEGAME_ENTRY_BUFFER_1 ((u8*)0x801E1430) // Slot 2 savegame entry.

#define MEMCARD_DEVICE_COUNT_MAX 8
#define MEMCARD_SAVES_COUNT_MAX  11
#define MEMCARD_FILE_COUNT_MAX   15
#define MEMCARD_SLOT_COUNT_MAX   2

#define SAVEGAME_FOOTER_MAGIC 0xDCDC
#define SAVEGAME_COUNT_MAX    MEMCARD_SAVES_COUNT_MAX * MEMCARD_FILE_COUNT_MAX

// ==============
// HELPER MACROS
// ==============

#define MemCard_ActiveMemCardSlotGet(slotIdx) \
    ((s_SaveScreenElement*)&SAVEGAME_ENTRY_BUFFER_0[2640 * (slotIdx)])

#define MemCard_StatusGet(status, deviceIdx) \
    status >> (deviceIdx * 3) & 0x7

#define MemCard_StatusStore(status, deviceIdx) \
    status << (deviceIdx * 3)

#define MemCard_FileStatusGet(status, deviceIdx) \
    status >> (deviceIdx * 2) & 0x3

#define MemCard_FileStatusStore(status, deviceIdx) \
    status << (deviceIdx * 2)

/** @brief Memory card access macros. */

#define MemCard_Port(x) \
    ((x) & 4) * 4

#define MemCard_CardNumber(x) \
    (x & 3)

#define MemCard_Chan(x) \
    MemCard_Port(x) + MemCard_CardNumber(x)

// ============
// ENUMERATORS
// ============

// Game-specific.

/** @brief Used by `s_SaveScreenElement`. */
typedef enum _SavegameEntryType
{
    SavegameEntryType_NoMemCard          = 0,
    SavegameEntryType_UnformattedMemCard = 1,
    SavegameEntryType_CorruptedMemCard   = 2,
    SavegameEntryType_LoadMemCard        = 3,
    SavegameEntryType_OutOfBlocks        = 4,
    SavegameEntryType_NoDataInMemCard    = 5,
    SavegameEntryType_Unk6               = 6,
    SavegameEntryType_CorruptedSave      = 7,
    SavegameEntryType_Save               = 8,
    SavegameEntryType_NewSave            = 9,
    SavegameEntryType_NewFile            = 10
} e_SavegameEntryType;

/** @brief Memory card game process.
 * Determines the process from the specific game memory card system being executed.
 */
typedef enum _MemCardGameProcessId
{
    MemCardGameProcessId_None          = 0,
    MemCardGameProcessId_Init          = 1,
    MemCardGameProcessId_Load_Game     = 2,
    MemCardGameProcessId_Save_Settings = 3,
    MemCardGameProcessId_Load_Settings = 4,
    MemCardGameProcessId_Save_Game     = 5,
    MemCardGameProcessId_Format        = 6
} e_MemCardGameProcessId;

/** @brief Memory card states. */
typedef enum _MemCardState
{
    MemCardState_Null        = 0, /** Null state. */
    MemCardState_Unavailable = 1, /** Not connected. */
    MemCardState_Loading     = 2, /** Loading the memory card. */
    MemCardState_Available   = 3,
    MemCardState_Format      = 4, /** Format required. */
    MemCardState_Broken      = 5
} e_MemCardState;

/** @brief Memory card file states. */
typedef enum _FileState
{
    FileState_Unused  = 0,
    FileState_Used    = 1,
    FileState_Damaged = 3
} e_FileState;

/** @brief Memory card process states. */
typedef enum _MemCardWorkState
{
    MemCardWorkState_Idle          = 0,
    MemCardWorkState_Init          = 1,
    MemCardWorkState_Check         = 2,
    MemCardWorkState_Load          = 3,
    MemCardWorkState_DirRead       = 4,
    MemCardWorkState_FileCreate    = 5,
    MemCardWorkState_FileOpen      = 6,
    MemCardWorkState_FileReadWrite = 7
} e_MemCardWorkState;

// Low level process.

/** @brief Memory card I/O streaming process. */
typedef enum _MemCardWorkIoMode
{
    MemCardWorkIoMode_Init    = 0,
    MemCardWorkIoMode_DirRead = 1, // TODO: Not sure if this is actual purpose yet.
    MemCardWorkIoMode_Read    = 2,
    MemCardWorkIoMode_Write   = 3,
    MemCardWorkIoMode_Create  = 4
} e_MemCardWorkIoMode;

typedef enum _MemCardWorkResult
{
    MemCardWorkResult_NotConnected    = 0,   /** "Card not connected". */
    MemCardWorkResult_Success         = 1,   /** Default code returned when no errors occur. */
    MemCardWorkResult_InitError       = 2,   /** `MemCard_State_Init` `EvSpNEW` "No writing after connection". */
    MemCardWorkResult_InitComplete    = 3,   /** `MemCard_State_Init` `EvSpIOE` "Connected". */
    MemCardWorkResult_LoadError       = 4,   /** `MemCard_State_Load` `EvSpNEW` "Uninitialized card". */
    MemCardWorkResult_NewDevice       = 5,   /** `MemCard_State_DirRead` when `g_MemCardWork.hasNewDevice`. */
    MemCardWorkResult_NoNewDevice     = 6,   /** `MemCard_State_DirRead` when `!g_MemCardWork.hasNewDevice`. */
    MemCardWorkResult_FileCreateError = 7,   /** `MemCard_State_FileCreate` after 15 retries. */
    MemCardWorkResult_FileOpenError   = 8,   /** `MemCard_State_FileOpen` after 15 retries. */
    MemCardWorkResult_FileSeekError   = 9,   /** `MemCard_State_FileReadWrite` after 15 retries. */
    MemCardWorkResult_FileIoError     = 10,  /** `MemCard_State_FileReadWrite` after 15 retries. */
    MemCardWorkResult_FileIoComplete  = 11,  /** `MemCard_State_FileReadWrite` `EvSpIOE` "Completed". */
    MemCardWorkResult_Full            = 100, /** Used outside main memcard code. */
    MemCardWorkResult_DamagedData     = 101  /** Used outside main memcard code. */
} e_MemCardWorkResult;

// ========
// STRUCTS
// ========

/** @note Naming. Oddly fortunate event in the OPM16 build there
 * are some strings related to this split which indicate
 * the name of 6 structs. Those split names being:
 *  MCM_FUNC_WORK
 *  MC_FILE
 *  SCE_HEADER
 *  MC_HEADER
 *  MC_CONFIG
 *  MC_PROGRESS
 */

/** @note Reference information for `s_PsxSaveBlock` and `s_PocketStationHeader`:
 * https://github.com/Sparagas/Silent-Hill/blob/1f24eb097a4b99129bc7c9793d23c82244848a27/010%20Editor%20-%20Binary%20Templates/ps1_memory_card.bt#L122C8-L122C17
 * https://problemkaputt.de/psxspx-memory-card-data-format.htm
 * https://problemkaputt.de/psxspx-pocketstation-file-header-icons.htm
 * https://medievil.wiki/w/Saved_game_data_in_MediEvil_(1998)
 * https://www.psdevwiki.com/ps3/PS1_Savedata
 */

/** @unused @brief Pocket Station save header data.
 * Serves no purpose, it could have even be completely ignored. However,
 * `MemCard_SaveBlockGenerate` has a piece of code dedicated to clean this
 * space and there the usage of `sizeof` mixed with this struct fits well
 * enough. This may have been never intended to be used and just a leftover
 * as it is possible this memory card code may have been inherited from
 * other KCET (just like the game's audio system).
 */
typedef struct _PocketStationHeader
{
    /* 0x0 */   s8   __pad_0[12];
    /* 0xC */   u16  pocketStationMcIconFrameCount;
    /* 0xE */   s8   pocketStationId[4];
    /* 0x12 */  u16  pocketStationApiIconFrameCount;
    /* 0x14 */  s8   __pad_14[8];
} s_PocketStationHeader;

/** @brief PS1 Save block data. */
typedef struct _PsxSaveBlock
{
    /* 0x0  */  char                  magic[2];
    /* 0x2  */  u8                    iconDisplayFlag; /** Sets the amount of frames the icon counts (This game only uses 1). */
    /* 0x3  */  u8                    blockCount;
    /* 0x4  */  u16                   titleNameShiftJis[32];
    /* 0x44 */  s_PocketStationHeader pocketStationHeader;
    /* 0x60 */  s8                    iconPalette[32];       // CLUT data copied from `TIM_IMAGE.caddr`.
    /* 0x80 */  s8                    frameIconData[3][128]; // Copied from `TIM_IMAGE.paddr`.
} s_PsxSaveBlock;

/** @brief PS1 Save block information. */
typedef struct _MemCardDirectory
{
    /* 0x0   */ char filenames[MEMCARD_FILE_COUNT_MAX][21];
    /* 0x13B */ u8   blockCounts[MEMCARD_FILE_COUNT_MAX]; // Size of each file in 8192 byte blocks.
    /* 0x14C */ s8   __pad[2];
} s_MemCardDirectory;

/** @brief PS1 Save block information. */
typedef struct _MemCardWork
{
    /* 0x0  */ s32 devicesPending; /** Bitfield of device IDs, each set bit index is an ID that must be read/initialized first. */
    /* 0x4  */ s32 state;          /** `e_CardState` */
    /* 0x8  */ s32 stateStep;
    /* 0xC  */ s32 stateResult;    /** `e_MemCardWorkResult` */
    /* 0x10 */ s32 eventSwSpIOE;
    /* 0x14 */ s32 eventSwSpERROR;
    /* 0x18 */ s32 eventSwSpTIMOUT;
    /* 0x1C */ s32 eventSwSpNEW;
    /* 0x20 */ s32 eventHwSpIOE;
    /* 0x24 */ s32 eventHwSpERROR;
    /* 0x28 */ s32 eventHwSpTIMOUT;
    /* 0x2C */ s32 eventHwSpNEW;
    /* 0x30 */ s32 eventHwSpUNKNOWN;
    /* 0x34 */ s32 lastEventHw;
    /* 0x38 */ s32 MemCardWorkIoMode; /** `e_MemCardWorkIoMode` */
    /* 0x3C */ s32 deviceId;
    
    /* 0x40 */ s_MemCardDirectory* directories; /** Array of files on the card, pointer supplied by caller to `MemCard_WorkSet`. */
    /* 0x44 */ char                filePath[28];
    /* 0x60 */ s32                 createBlockCount; /** Block count passed to `open` when creating new file. */
    /* 0x64 */ s32                 seekOffset;
    /* 0x68 */ void*               dataBuffer;
    /* 0x6C */ s32                 dataSize;
    /* 0x70 */ bool                hasNewDevice;
    /* 0x74 */ s32                 fileHandle;
    /* 0x78 */ s32                 retryCount;
    /* 0x7C */ s32                 field_7C; /** Dead code. Only ever set to 0. */
} s_MemCardWork;

/** @brief Savegame metadata for displaying in the save screen. */
typedef struct _MemCard_SaveMetadata
{
    /* 0x0   */ s32 totalSavegameCount;
    /* 0x4   */ u32 gameplayTimer;
    /* 0x8   */ u16 savegameCount;
    /* 0xA   */ u8  locationId;
    /* 0xB+0 */ u8  isNextFearMode           : 1;
    /* 0xB+1 */ u8  add290Hours              : 2;
    /* 0xB+3 */ u8  pickedUpSpecialItemCount : 5; /** See `pickedUpSpecialItemCount` comment in `s_Savegame`. */
} s_MemCard_SaveMetadata;

/** @brief Information of elements in the save screen.
 * This is used to determine both saves and other
 * elements in save screen as it is use to display
 * the "New Save" or "The file is damage" elements.
 * It is also used to display status messages in rectangles,
 * for example when no memory card is inserted or no save is
 * available.
 */
typedef struct _SaveScreenElement
{
    /* 0x0 */ s16                     totalSavegameCount; /** Counter for all savegame instances created throughout the game.
                                                           * The value is derived by running through all savegames on a memory card
                                                           * and picking the one with the largest value.
                                                           *
                                                           * @bug This counter is used to determine if the "New Save" element has
                                                           * been selected, which causes overwrites to not show the "Yes/No" message.
                                                           */
    /* 0x2 */ s16                     savegameCount;
    /* 0x4 */ s8                      type; /** `e_SavegameEntryType` */
    /* 0x5 */ s8                      deviceId;
    /* 0x6 */ s8                      fileIdx;
    /* 0x7 */ s8                      elementIdx;
    /* 0x8 */ s8                      locationId;
              // 3 bytes of padding.
    /* 0xC */ s_MemCard_SaveMetadata* saveMetadata;
} s_SaveScreenElement;

/** @brief Appended to `s_Savegame` and `s_OptionsConfig` during game save. Contains 8-bit XOR checksum + magic.
 *
 * @note Checksum generated via `MemCard_ChecksumGenerate`.
 */
typedef struct _Savegame_Footer
{
    /* 0x0 */ u8  checksum[2];
    /* 0x2 */ u16 magic;
} s_Savegame_Footer;

/** @brief Contains `s_Savegame` data with the footer appended to the end containing the checksum + magic. */
typedef struct _Savegame_Container
{
    /* 0x0   */ s_Savegame        savegame;
    /* 0x27C */ s_Savegame_Footer footer;
} s_Savegame_Container;

/** @brief Contains `s_OptionsConfig` and a footer at the end containing checksum + magic.
 *
 * @note For some reason the struct needs to be 128/0x80 bytes long.
 */
typedef struct _Savegame_OptionsConfig
{
    /* 0x0  */ s_OptionsConfig   config;
#if !defined(M2CTX)
    /* 0x38 */ u8                __pad[128 - sizeof(s_OptionsConfig) - sizeof(s_Savegame_Footer)];
#else
    /* 0x38 */ u8                __pad[0x44];
#endif
    /* 0x7C */ s_Savegame_Footer footer;
} s_Savegame_OptionsConfig;

/** @brief Contains `s_MemCard_SaveMetadata` and a footer at the end containing checksum + magic.
 *
 * @note For some reason the struct needs to be 256/0x100 bytes long.
 */
typedef struct _MemCard_SaveHeader
{
    /* 0x0  */ s32                    field_0;
    /* 0x4  */ s_MemCard_SaveMetadata saveMetadata[MEMCARD_SAVES_COUNT_MAX];
#if !defined(M2CTX)
    /* 0x88 */ u8                     __pad[256 - sizeof(s32) - (sizeof(s_MemCard_SaveMetadata) * MEMCARD_SAVES_COUNT_MAX) - sizeof(s_Savegame_Footer)];
#else
    /* 0x88 */ u8                     __pad[0x74];
#endif
    /* 0xFC */ s_Savegame_Footer      footer;
} s_MemCard_SaveHeader;

typedef struct _MemCard_DeviceInfo
{
    /* 0x0  */ s32                   status;                            /** `e_MemCardState`. */
    /* 0x4  */ s8                    fileState[MEMCARD_FILE_COUNT_MAX]; /** `e_FileState`. */
    /* 0x14 */ s_MemCard_SaveHeader* saveHeader;                        /** Slots saves information. */
    /* 0x18 */ s32                   fileLimit;                         /** Max count of files allowed in the memory card. */
} s_MemCard_DeviceInfo;

/** @note Information about game specific memory card process.
 *
 * Stores information about processes done by game-specific memory card system.
 */
typedef struct _MemCard_GameProcess
{
    /* 0x0  */ s32 processId;         /** `e_MemCardGameProcess` */
    /* 0x4  */ s32 deviceId;
    /* 0x8  */ s32 fileIdx;
    /* 0xC  */ s32 saveIdx;
    /* 0x10 */ s32 processState;      /** States related to specific memory card events. */
    /* 0x14 */ s32 lastMemCardResult; /** `e_MemCardWorkResult` */
} s_MemCard_GameProcess;

/** @brief See `g_MemCardSaveWork`.
 *
 * @note OPM16 has `MCM_FUNC_WORK` struct with size 0x6D8, close to this 0x718.
 */
typedef struct _MemCardSaveWork
{
    /* 0x0   */ s_MemCard_DeviceInfo     devices[MEMCARD_DEVICE_COUNT_MAX];
    /* 0xE0  */ s_MemCard_GameProcess     saveWork[2];
    /* 0x110 */ bool                     memCardInitalized;
    /* 0x114 */ s32                      unk_114;
    /* 0x118 */ s_PsxSaveBlock           saveBlock;
    /* 0x318 */ s_MemCard_SaveHeader     saveInfo;
    /* 0x418 */ s_Savegame_OptionsConfig optionsConfig;
    /* 0x498 */ s_Savegame_Container     savegame;
} s_MemCardSaveWork;

/** @brief Save count information.
 * Used specifically fby `MemCard_SaveWithBiggestTotalSavegameCountGet` to temporarily store access information about
 * the savegame with the highest save countsaves.
 */
typedef struct _MemCard_TotalSavesInfo
{
    /* 0x0 */ s32 totalSavegameCount;
    /* 0x4 */ s32 fileIdx;
    /* 0x8 */ s32 saveIdx;
} s_MemCard_TotalSavesInfo;

// ========
// GLOBALS
// ========

/** @brief Basic information required to draw info elements in save slots.
 * Address access is based on the slot: slot 1 = 0x801E09E0, slot 2 = 0x801E1440.
 *
 * @note Macros for its references are:
 * `SAVEGAME_ENTRY_BUFFER_0`
 * `SAVEGAME_ENTRY_BUFFER_1`
 */

extern u8 g_SlotElementSelectedIdx[2]; // 0 - Slot 1, 1 - Slot 2.

extern s8 g_SelectedSaveSlotIdx; // 0 - Slot 1, 1 - Slot 2.

/** @brief Dead code. Defined as 0 and whenever `SaveScreen_Continue` is triggered it turns 1. */
extern u8 D_800A97D7;

/** @brief Defines if the player is or not in the save screen under the save game mode or in the load game mode. */
extern s8 g_SaveScreen_IsInSaveScreen;

/** @brief Dead code. Only used for a check which asks if this is 0. */
extern s8 D_800A97D9;

// ====================
// GLOBALS (BSS; Hack; memcard.c)
// ====================
// To match the order of the BSS segment, extern declarations
// are required in a predetermined order.
// This is done until a way to replicate `common`
// segment behavior is found.

extern s_MemCard_SaveHeader g_MemCard_SaveHeaderInfo_Slot1[MEMCARD_FILE_COUNT_MAX];
extern s_MemCard_SaveHeader g_MemCard_SaveHeaderInfo_Slot2[MEMCARD_FILE_COUNT_MAX];
extern s_MemCard_SaveHeader g_MemCard_SaveHeaderInfo_Null[MEMCARD_FILE_COUNT_MAX];
extern bool g_MemCard_SysAvailibityStatus;
extern s32 __pad_bss_800B5484;
extern s_MemCardWork g_MemCardWork;
extern s_MemCardSaveWork g_MemCardSaveWork;
extern s32 g_MemCard_PrevSavegameCount;

// ====================
// GLOBALS (BSS; Hack; sys/memcard_2.c)
// ====================

extern s16 g_MemCard_SavegameCount;
extern s16 __pad_bss_800BCD2A;
extern s_SaveScreenElement* g_MemCard_ActiveMemCardSlotSaves;
extern u8  g_Savegame_ElementCount0[MEMCARD_SLOT_COUNT_MAX];
extern s16 __pad_bss_800BCD32;
extern u32 g_MemCard_AllMemCardsStatus;
extern s8 g_SaveScreen_SaveScreenState;
extern s8 g_SaveScreen_IsLoadError;
extern s16 g_MemCard_TotalElementsCount;
extern u8 g_Savegame_ElementCount1[MEMCARD_SLOT_COUNT_MAX];
extern u8 g_Savegame_SelectedElementIdx;
extern s8 g_SelectedFileIdx;
extern s8 g_SelectedDeviceId;
extern s8 __pad_bss_800BCD41[3];

// ==========
// FUNCTIONS
// ==========

/** @brief Initializes memory card system.
 * This Initializes the entire memory card handling system while
 * `MemCard_WorkInit` exclusively initalizes the memory cards.
 *
 * Scratch 1: https://decomp.me/scratch/TYf23
 * Scratch 2: https://decomp.me/scratch/Jde6W
 */
void MemCard_SysInit(void);

/** @brief Enables game memory card handling system.
 * Additionally, it initializes card events.
 * 
 * Scratch 1: https://decomp.me/scratch/Tdfjl
 * Scratch 2: https://decomp.me/scratch/XPlxF
 */
void MemCard_SysEnable(void);

/** @brief Enables game memory card handling system.
 * Additionally, it stop card events.
 * 
 * Scratch: https://decomp.me/scratch/NSB17
 */
void MemCard_SysDisable(void);

/** @brief Sets `g_MemCardSaveWork.memCardInitalized` to true.
 * 
 * Scratch: https://decomp.me/scratch/V506y
 */
void MemCard_InitStatus(void);

/** @unused @brief Sets `g_MemCardSaveWork.memCardInitalized`
 * to false and initalizes a null "Save Work" process with a
 * last saved memory card state as `MemCardWorkResult_NotConnected`.
 * 
 * Scratch 1: https://decomp.me/scratch/DTZBs
 * Scratch 2: https://decomp.me/scratch/L8XKw
 */
void MemCard_InitStatusNotConnected(void);

/** @brief Get all the currently connected memory card status.
 * 
 * Scratch: https://decomp.me/scratch/x4Wrk
 *
 * @return Status bitfield. `MemCard_StatusGet` is used to retrieve data.
 */
s32 MemCard_AllMemCardsStatusGet(void);

/** @unused @brief Identical to `MemCard_InitStatus`.
 * 
 * Scratch: https://decomp.me/scratch/Ws9R9
 */
void func_8002E8D4(void);

/** @unused @brief Sets `g_MemCardSaveWork.memCardInitalized` to `false` and initalizes a null "Save Work" process with
 * a last saved memory card state as `MemCardWorkResult_Success`.
 * 
 * Scratch 1: https://decomp.me/scratch/uHWZ1
 * Scratch 2: https://decomp.me/scratch/bBBHt
 */
void MemCard_InitStatusSuccess(void);

/** @unused @brief Almost identical to `MemCard_AllMemCardsStatusGet`.
 * 
 * Scratch: https://decomp.me/scratch/67dYs
 *
 * @return Status bitfield. `MemCard_StatusGet` is used to retrieve data.
 */
s32 func_8002E914(void);

/** @brief Sets `g_MemCardSaveWork.saveWork[0]` process if none has been set.
 * 
 * Scratch: https://decomp.me/scratch/lmp3g
 *
 * @param processId `e_MemCardGameProcess`.
 * @param deviceId Memory card index.
 * @param fileIdx File index on the memory card.
 * @param saveIdx Index of the selected save from the selected file on the memory card.
 * @return `true` if the process was succesfully set, `false` otherwise.
 */
bool MemCard_ProcessSet(s32 processId, s32 deviceId, s32 fileIdx, s32 saveIdx);

/** @brief Returns the memory card last process result state.
 *
 * Scratch: https://decomp.me/scratch/0ZNLb
 *
 * @return `e_MemCardGameProcess`.
 */
s32 MemCard_LastMemCardResultGet(void);

/** @brief Returns the status of all files on a memory card.
 *
 * Scratch: https://decomp.me/scratch/IZ2Xs
 *
 * @param deviceId Memory card index.
 * @return Status bitfield. `MemCard_StatusGet` is used to retrieve data.
 */
s32 MemCard_FileStatusesGet(s32 deviceId);

/** @brief Retrieves a savegame's metadata (s_MemCard_SaveMetadata).
 * 
 * Scratch: https://decomp.me/scratch/hs5LP
 *
 * @param deviceId Memory card index.
 * @param fileIdx File index on the memory card.
 * @param saveIdx Index of the selected save from the selected file on the memory card.
 * @return Data stored at `g_MemCard_SaveHeaderInfo_Slot`.
 */
s_MemCard_SaveMetadata* MemCard_SaveMetadataGet(s32 deviceId, s32 fileIdx, s32 saveIdx);

/** @brief Returns the count of non-empty files on the specified memory card.
 * 
 * Scratch: https://decomp.me/scratch/dJ8Oq
 *
 * @param deviceId Memory card index.
 * @return Count of non-empty files in memory card.
 */
s32 MemCard_UsedFileCount(s32 deviceId);

/** @brief Returns the count of available files on the specified memory card.
 * 
 * Scratch: https://decomp.me/scratch/3agLG
 *
 * @param deviceId Memory card index.
 * @return Count of available files in memory card.
 */
s32 MemCard_FreeFilesCount(s32 deviceId);

/** @brief @unused Checks if savegames have been created on any inserted memory card.
 * If savegames are found, output arguments are set to the index
 * of the memory card, file index, and savegame index with the highest total savegame count from any memory card.
 *
 * Scratch: https://decomp.me/scratch/9SoBG
 *
 * @param outDeviceId Output device ID (memory card).
 * @param outFileIdx Output file index on the memory card.
 * @param outSaveIdx Output savegame index in the file from the memory card.
 * @return `true` if any save have been detected, `false` otherwise.
 */
bool MemCard_NoSavesDoneCheck(s32* outDeviceId, s32* outFileIdx, s32* outSaveIdx);

/** @brief Game's memory card update function.
 * Updates the internal memory card processes state (`g_MemCardWork.state`) and processes any process set in
 * `g_MemCardSaveWork.saveWork[X].processId`.
 *
 * Scratch 1: https://decomp.me/scratch/QgEOs
 * Scratch 2: https://decomp.me/scratch/cONhw
 *
 * @note Can be disabled by setting `g_MemCard_SysAvailibityStatus` to `false`.
 */
void MemCard_Update(void);

/** @brief Updates the footer with the checksum of the given data.
 *
 * Scratch: https://decomp.me/scratch/Gj9Ox
 *
 * @param saveFooter Savegame data footer with the checksum.
 * @param saveData Savegame data.
 * @param saveDataLength Size of save data.
 */
void MemCard_ChecksumUpdate(s_Savegame_Footer* saveFooter, s8* saveData, s32 saveDataLength);

/** @brief Generates a checksum of the given `saveData` and compares it against the checksum value in the footer.
 *
 * Scratch: https://decomp.me/scratch/oAvuF
 *
 * @param saveFooter Savegame data footer with the checksum.
 * @param saveData Savegame data data.
 * @param saveDataLength Savegame data size.
 * @return `true` if the checksums match, `false` otherwise.
 */
bool MemCard_ChecksumValidate(s_Savegame_Footer* saveFooter, s8* saveData, s32 saveDataLength);

/** @brief Generates an 8-bit XOR checksum over the given data. Only used with `s_Savegame` data.
 *
 * Scratch 1: https://decomp.me/scratch/TyNmD
 * Scratch 2: https://decomp.me/scratch/AvFBU
 *
 * @param saveData Pointer to save data.
 * @param saveDataLength Size of save data.
 * @return `true` if the checksums match, `false` otherwise.
 */
u8 MemCard_ChecksumGenerate(s8* saveData, s32 saveDataLength);

/** @brief Initializes the memory card BIOS system.
 *
 * Scratch: https://decomp.me/scratch/4Bd9c
 */
void MemCard_WorkInit(void);

/** @brief Initializes the memory card processes system.
 *
 * Scratch: https://decomp.me/scratch/dQ9Mh
 */
void MemCard_EventsInit(void);

/** @brief Finalizes the memory card processes system.
 *
 * Scratch: https://decomp.me/scratch/9iYwp
 */
void MemCard_EventsClose(void);

/** @brief Returns a state from any active memory card event.
 *
 * Scratch: https://decomp.me/scratch/r0KzP
 * @return EvSpERROR, EvSpTIMOUT, EvSpNEW, EvSpIOE or NULL.
 */
s32 MemCard_SwEventsTest(void);

/** @brief Resets the memory card event system (software level).
 *
 * Scratch 1: https://decomp.me/scratch/bAN2R
 * Scratch 2: https://decomp.me/scratch/D9ZX8
 */
void MemCard_SwEventsReset(void);

/** @brief Returns the last memory card event registered (hardware level).
 *
 * Scratch: https://decomp.me/scratch/xxLr7
 *
 * @return Event registered in `g_MemCardWork.lastEventHw`.
 */
s32 MemCard_HwLastEvent(void);

/** @brief Resets the memory card event system (hardware level).
 *
 * Scratch 1: https://decomp.me/scratch/BzLTs
 * Scratch 2: https://decomp.me/scratch/ZvY0z
 */
void MemCard_HwEventsReset(void);

/** @brief
 *
 * Scratch: https://decomp.me/scratch/T0fbk
 */
void MemCard_HwEventSpIOE(void);

/** @brief
 *
 * Scratch: https://decomp.me/scratch/Wszu4
 */
void MemCard_HwEventSpERROR(void);

/** @brief
 *
 * Scratch: https://decomp.me/scratch/uuikJ
 */
void MemCard_HwEventSpNEW(void);

/** @brief
 *
 * Scratch: https://decomp.me/scratch/xomR5
 */
void MemCard_HwEventSpTIMOUT(void);

/** @brief
 *
 * Scratch: https://decomp.me/scratch/JLaeo
 */
void MemCard_HwEventSpUNKNOWN(void);

/** @brief
 *
 * Scratch: https://decomp.me/scratch/ta76s
 *
 * @return `e_MemCardWorkResult`
 */
s32 MemCard_StateResult(void);

/** @brief Sets basic processes related to data transfer.
 * See `e_MemCardWorkIoMode` for what kind of processes this function can handle.
 *
 * Scratch: https://decomp.me/scratch/ta76s
 *
 * @param mode `e_MemCardWorkIoMode`.
 * @param deviceId Memory card index.
 * @param outDir Memory card directories information.
 * @param filename PS1 filename directory identifier.
 * @param createBlockCount Memory card file count.
 * @param fileOffset Memory card data pointer.
 * @param outBuf Extracted data buffer.
 * @param bufSize Data size to extract.
 * @return `true` if the work has been set, otherwise `false` if the memory card is in an idle state.
 */
bool MemCard_WorkSet(e_MemCardWorkIoMode mode, s32 deviceId, s_MemCardDirectory* outDir, char* filename, s32 createBlockCount, s32 fileOffset, void* outBuf, s32 bufSize);

/** @brief Updates memory card elements in memory.
 *
 * Scratch (USA): https://decomp.me/scratch/xgVSr 
 * Scratch (JAP): https://decomp.me/scratch/j3Uvz
 *
 * @return Memory cards status.
 */
bool MemCard_ElementsUpdate(void);

#endif
