#ifndef _MAIN_FSQUEUE_H
#define _MAIN_FSQUEUE_H

#include "gpu.h"
#include "main/file_info.h"

 /** FS queue size. */
#define FS_QUEUE_LENGTH 32

/** TODO: Rename in address order when all become known. Follows a pattern of 4096-byte buffers.
 * Could also make something like this later to address magic:
 *
 * typedef enum _BufferIdx
 * {
 *     BufferIdx_WeaponAnim = 0,
 *     BufferIdx_PlayerAnim = 1
 * } e_BufferIdx;
 *
 * #define FS_BUFFER(idx) \
 *     (void*)(0x80100600 + ((idx) * 0x1000))
 */
#define FS_BUFFER_0      (void*)0x8010A600
#define FS_BUFFER_12     (void*)0x801201B4 // Used for weapon anim.     } Sub-buffers within the 4096-byte buffers?
#define FS_BUFFER_4      (void*)0x80124384 // Used for player map anim. }
#define FS_BUFFER_11     (void*)0x80169600 // } Used for DMS cutscene data.
#define FS_BUFFER_17     (void*)0x80169E00 // }
#define FS_BUFFER_13     (void*)0x8016AE00 // }
#define FS_BUFFER_14     (void*)0x800F9600 // }
#define FS_BUFFER_15     (void*)0x801F3600 // }
#define FS_BUFFER_16     (void*)0x801EBE00 // }
#define FS_BUFFER_18     (void*)0x80180600 // }
#define FS_BUFFER_19     (void*)0x801A1E00 // }
#define FS_BUFFER_20     (void*)0x80185600 // }
#define GLOBAL_LM_BUFFER (s_LmHeader*)0x8016B600
#define IPD_BUFFER       (s_IpdHeader*)0x80175600 // Used by map.
#define LM_BUFFER_2      (s_LmHeader*)0x8019E600
#define FS_BUFFER_3      (void*)0x801B2600
#define FS_BUFFER_8      (void*)0x801B5E80        // Used for loading inventory item models.
#define FS_BUFFER_7      (void*)0x801C2600        // Used in `b_konami.c`.
#define FS_BUFFER_2      (void*)0x801CF600
#define FS_BUFFER_1      (void*)0x801E2600
#define FS_BUFFER_5      (void*)0x801E3600 // Used in `b_konami.c`.
#define FS_BUFFER_6      (void*)0x801E4600 // Used in `b_konami.c`.
#define FS_BUFFER_21     (void*)0x801E5600
#define FS_BUFFER_10     (void*)0x801EA600
#define FS_BUFFER_9      (void*)0x801EC600
#define FS_BUFFER_22     (void*)0x8019F8F8

#define FS_BUFFER_24     (void*)0x801A0600
#define FS_BUFFER_25     (void*)0x80167600
#define FS_BUFFER_26     (void*)0x8018C600
#define FS_BUFFER_27     (void*)0x80196E00
#define FS_BUFFER_28     (void*)0x801D6E00
#define FS_BUFFER_29     (void*)0x80142A00
#define FS_BUFFER_30     (void*)0x80156A00
#define FS_BUFFER_31     (void*)0x80102E00
#define FS_BUFFER_32     (void*)0x80106E00
#define FS_BUFFER_33     (void*)0x80153A00
#define FS_BUFFER_34     (void*)0x80163200
#define FS_BUFFER_35     (void*)0x801950F0

#define IMAGE_BUFFER   (u_long*)0x801AFA00
#define IMAGE_BUFFER_0 (u_long*)0x801CFA00
#define IMAGE_BUFFER_1 (u_long*)0x801C8200
#define IMAGE_BUFFER_2 (u_long*)0x801ABE00
#define IMAGE_BUFFER_3 (u_long*)0x801EB600
#define IMAGE_BUFFER_4 (u_long*)0x801DCE00
#define IMAGE_BUFFER_5 (u_long*)0x801DE600

#define TEMP_MEMORY_ADDR (s8*)0x801A2600
#define CD_ADDR_0        (u_long*)0x801E2600 // Used to temporarily allocate VAB files.

#define FONT24_BUFFER (u_long*)0x801F5600 // Loaded by `GameFs_StfRollBinLoad`.

#define HARRY_LM_BUFFER (void*)0x800FE600 /** Harry character model. */
#define MAP_CHARA_BASE  (void*)0x800FEE00

/** Model of an item held in the player's hand. */
#define HELD_ITEM_LM_BUFFER \
    (s_LmHeader*)((HARRY_LM_BUFFER) + Fs_GetFileSize(FILE_CHARA_HERO_ILM))

/** Up to 4 chara models used in a map. */
#define MAP_CHARA_LM_BUFFER \
    ((void*)((u8*)(MAP_CHARA_BASE) + Fs_GetFileSize(FILE_CHARA_HERO_ILM)))

/** @brief `FsQueue::state`s when processing a read operation (`Fs_QueueUpdateRead`).
 *
 * When `Fs_QueueUpdate` is called and the current op is a read, it will perform the corresponding action below.
 * Unless otherwise specified, success of that action will advance to the next state.
 *
 * `FsQueueReadState_Reset` and `FsQueueReadState_Check` perform one check/tick iteration every time `Fs_QueueUpdate` is called
 * and only advance to the next state when they're done.
 */
typedef enum _FsQueueReadState
{
    FsQueueReadState_Allocate = 0, /** Allocate memory for the current read operation (`fsQueueAllocData`), if needed. */
    FsQueueReadState_Check    = 1, /** Check if the current read operation can proceed (`Fs_QueueCanRead`). Goto next state if it can. */
    FsQueueReadState_SetLoc   = 2, /** Set start sector from `info` (`Fs_QueueTickSetLoc`). If failure, goto `FsQueueReadState_Reset`. */
    FsQueueReadState_Read     = 3, /** Read from CD (`Fs_QueueTickRead`). If failure, goto `FsQueueReadState_Reset`. */
    FsQueueReadState_Sync     = 4, /** Wait for read to complete. If failure, goto `FsQueueReadState_Reset`. */
    FsQueueReadState_Reset    = 5  /** Tick the reset timers (`Fs_QueueResetTick`). When done, reset CD driver and goto `FsQueueReadState_SetLoc`. */
} e_FsQueueReadState;

/** @brief `FsQueue::state`s when processing a seek operation (`Fs_QueueUpdateSeek`).
 *
 * When `Fs_QueueUpdate` is called and the current operation is a seek, it will perform the corresponding action below.
 * Unless otherwise specified, success of that action will advance to the next state.
 */
typedef enum _FsQueueSeekState
{
    FsQueueSeekState_SetLoc = 0, /** Set seek sector from `info` (`Fs_QueueTickSetLoc`). */
    FsQueueSeekState_SeekL  = 1, /** Start seeking to above location (via `CdControl(CdlSeekL, ...)`). */
    FsQueueSeekState_Sync   = 2, /** Wait for seek to complete (`CdSync()`). If `CdlDiskError`, goto `FsQueueSeekState_Reset`. */
    FsQueueSeekState_Reset  = 3  /** See `FsQueueReadState_Reset`. When done, reset CD driver and go to `FsQueueSeekState_SetLoc`. */
} e_FsQueueSeekState;

/** @brief Post-load states.
 *
 * When `Fs_QueueUpdate` is called it will perform an action on the current post-load entry, if any,
 * according to `g_FsQueue.postLoadState`, which can have one of these values.
 *
 * See `FsQueue::postLoadState`.
 */
typedef enum _FsQueuePostLoadState
{
    FsQueuePostLoadState_Init = 0, /** Check for allocated memory and proceed to `SKIP` or `EXEC`. */
    FsQueuePostLoadState_Skip = 1, /** Skip post-loading because this entry owns allocated memory. */
    FsQueuePostLoadState_Exec = 2  /** Execute post-load operation. */
 } e_FsQueuePostLoadState;

/** @brief Post-load types.
 *
 * What to do with a queue entry after its `operation` is done. Might be better described as "file format", but
 * it only applies to two filetypes.
 *
 * See `FsQueueEntry::postLoad`.
 */
typedef enum _FsQueuePostLoadType
{
    FsQueuePostLoadType_None = 0, /** Do nothing. */
    FsQueuePostLoadType_Tim  = 1, /** Parse TIM file (`Fs_QueuePostLoadTim`). Can use `extra.image`. */
    FsQueuePostLoadType_Anm  = 2  /** Parse ANM file maybe (`Fs_QueuePostLoadAnm`). Always uses `extra.anm`. */
} e_FsQueuePostLoadType;

/** @brief FS queue operation types.
 *
 * What to do for a queue entry.
 * See `s_FsQueueEntry::operation`.
 */
typedef enum _FsQueueOperation
{
    FsQueueOp_None = 0, /** Uninitialized. */
    FsQueueOp_Seek = 1, /** Seek to file location on CD (`Fs_QueueUpdateSeek`). */
    FsQueueOp_Read = 2  /** Read from CD (`Fs_QueueUpdateRead`). */
} e_FsQueueOperation;

/** @brief Extra queue entry data describing where to upload a TIM after reading.
 * See `FsQueueExtra`.
 *
 * @note `tPage` seems to be byte-swapped.
 */
typedef struct _FsImageDesc
{
    u8  tPage[2];
    u8  u;
    u8  v;
    s16 clutX;
    s16 clutY;
} s_FsImageDesc;
STATIC_ASSERT_SIZEOF(s_FsImageDesc, 8);

/** @brief Extra queue entry data describing something related to loading some ANM files.
 * See `FsQueueExtra`.
 */
typedef struct _FsAnmDesc
{
    u32            field_0; // Unknown index.
    u32            charaId;
    GsCOORDINATE2* coords_8;
} s_FsAnmDesc;
STATIC_ASSERT_SIZEOF(s_FsAnmDesc, 12);

/** @brief Extra data passed with the queue entry.
 *
 * Seems to be either a TIM descriptor (`image`), or something for ANM files (`anm`).
 *
 * For `image` usage, see `Fs_QueuePostLoadTim`.
 *
 * Unknown what exactly `anm` is, but it is used with preprocess type 2, which is only used
 * for some ANM files, but not others. See `Fs_QueuePostLoadAnm`.
 */
typedef union _FsQueueExtra
{
    s_FsImageDesc image; /** Location in VRAM where to upload a TIM during post-load. */
    s_FsAnmDesc   anm;   /** Unknown. Used when loading some ANM files. */
} s_FsQueueExtra;

/** @brief FS queue entry.
 *
 * Entry in the FS queue.
 * Holds the state of one read/seek operation and a pointer to the `FileInfo` of the file it's for.
 */
typedef struct _FsQueueEntry
{
    const s_FileInfo* info;         /** Pointer to the file table entry of the file this entry is for. */
    u8                operation;    /** What to do. See `e_FsQueueOperation`. */
    u8                postLoad;     /** What to do after `operation` is done. See `e_FsQueuePostLoadType`. */
    u8                allocate;     /** Boolean. If `true`, allocate a buffer for `data` from `g_FsMemory`, otherwise use `externalData` */
    u8                unused0;      /** Unused or padding. */
    void*             externalData; /** Pointer to an external buffer. */
    u32               unused1;      /** Unused but set by `Fs_QueueEnqueue`. */
    s_FsQueueExtra    extra;        /** Extra data, used during post-load. */
    void*             data;         /** Output buffer. Either allocated or same as `externalData`. */
} s_FsQueueEntry;

/** @brief Queue pointer.
 *
 * These had to be wrapped into a struct for some code to match.
 * Used for last added element, current read/seek op and current post process op.
 */
typedef struct _FsQueuePtr
{
    s32             idx; /** Index in `entries` this is pointing to. */
    s_FsQueueEntry* ptr; /** Entry in `entries` this is pointing to. */
} s_FsQueuePtr;

/** @brief FS queue.
 *
 * State of the file system operations queue.
 *
 * The queue is used for async reads from the CD and from PCDRV and async seeking on the CD.
 * Each operation can optionally include a post-process (post-load) step that happens after the main operation is complete.
 * For example, parsing a TIM file after reading it.
 *
 * New entries are added by `Fs_QueueEnqueue`. The queue is processed in `Fs_QueueUpdate`.
 *
 * @note Assuming this is a single struct because there's a `bzero` that zeroes out this entire block.
 */
typedef struct _FsQueue
{
    s_FsQueueEntry entries[FS_QUEUE_LENGTH]; /** Circular buffer for the queue itself. */
    s_FsQueuePtr   last;                     /** Index and address of the last added entry. */
    s_FsQueuePtr   read;                     /** Index and address the current operation entry to process. */
    s_FsQueuePtr   postLoad;                 /** Index and address of the current operation entry to post-process. */
    u32            state;                    /** Current processing stage. `e_FsQueueReadState` for reads, `e_FsQueueSeekState` for seeks. */
    u32            postLoadState;            /** Current postprocessing stage. See `e_FsQueuePostLoadState`. */
    s32            resetTimer0;              /** Reset timer (lo). Increments up to 8, then incrementss `reset_timer_1`. See `Fs_QueueResetTick`. */
    s32            resetTimer1;              /** Reset timer (hi). When it reaches 9, `CdReset` is called. See `Fs_QueueResetTick`. */
} s_FsQueue;

/** The FS queue. See `s_FsQueue`. */
extern s_FsQueue g_FsQueue;

/** @brief Checks if a queue entry index has been loaded and post-loaded.
 *
 * @param queueIdx Index of the queue entry to check.
 * @return `true` if the entry has been fully processed, `false` otherwise.
 */
bool Fs_QueueIsEntryLoaded(s32 queueIdx);

/** @brief Gets the number of operations currently in the queue.
 *
 * @return Number of operations in the queue. Includes both pending reads and pending post-loads.
 */
s32 Fs_QueueGetLength(void);

/** @brief Loads map chunks currently in view.
 *
 * @return `true` if the chunks in view are loaded, `false` otherwise.
 */
bool Fs_QueueChunksLoad(void);

/** @brief Spin-waits for the queue to become empty while calling `Fs_QueueUpdate`.
 * Calls some bodyprog functions before and after the wait, `VSync` during the waits and `DrawSync`
 * after the wait.
 */
void Fs_QueueWaitForEmpty(void);

/** @brief Add a new seek operation to the queue.
 *
 * @param fileIdx File table index of the file to seek to.
 * @return Index of the new queue entry.
 */
s32 Fs_QueueStartSeek(e_FsFile fileIdx);

/** @brief Add a new read operation to the queue.
 *
 * @param fileIdx File table index of the file to read.
 * @param dest Destination buffer. Seems there are no size checks.
 * @return Index of the new queue entry.
 */
s32 Fs_QueueStartRead(e_FsFile fileIdx, void* dest);

/** @brief Add a new TIM read operation to the queue.
 * Adds a read operation with `post-load = FsQueuePostLoadType_Tim`.
 *
 * @param fileIdx File table index of the file to read.
 * @param dest Destination buffer. There are no size checks.
 * @param image Where to upload the TIM in VRAM.
 * @return Index of the new queue entry.
 */
s32 Fs_QueueStartReadTim(e_FsFile fileIdx, void* dest, const s_FsImageDesc* image);

/** @brief Add a new ANM read operation to the queue.
 * Adds a read operation with `postLoad = FsQueuePostLoadType_Anm`.
 *
 * @note Does not actually take a file number, but instead takes one from an array of structs at 0x800A90FC in bodyprog,
 * using `arg1` as an index. Does not seem to take a `s_FsAnmDesc` pointer either. Maybe by value?
 *
 * @param idx Unknown index.
 * @param charaId Character ID.
 * @param dest Destination buffer.
 * @param coords Character coords.
 * @return Index of the new queue entry.
 */
s32 Fs_QueueStartReadAnm(s32 idx, s32 charaId, void* dest, GsCOORDINATE2* coords);

/** @brief Add new operation to the queue.
 *
 * If the queue is full, it will spin while calling `Fs_QueueUpdate` and wait until space frees up.
 * Called by all of the `Fs_QueueStart...` functions.
 *
 * @param fileIdx File table index of the file to load/seek.
 * @param op Operation type (`e_FsQueueOperation`).
 * @param postLoad Post-load type (`e_FsQueuePostLoadType`).
 * @param alloc Whether to allocate owned memory for this operation (`s_FsQueueEntry::allocate`).
 * @param unused0 Value for `s_FsQueueEntry::unused1`. Seems to be unused.
 * @param extra Extra data for operation (`s_FsQueueEntry::extra`).
 * @return Index of the new queue entry.
 */
s32 Fs_QueueEnqueue(e_FsFile fileIdx, u8 op, u8 postLoad, u8 alloc, void* data, u32 unused0, s_FsQueueExtra* extra);

/** @brief Initialize FS queue and FS memory.
 * Initializes `g_FsQueue` and calls `Fs_InitializeMem`.
 */
void Fs_QueueInitialize(void);

/** Seems to clear the queue. */
void Fs_QueueReset(void);

/** @brief Ticks the FS queue once.
 *
 * Depending on current read entry's operation type, either ticks reading it (`Fs_QueueUpdateRead`) or seeking it (`Fs_QueueUpdateSeek`).
 * If they return `true`, advances `g_FsQueue.read.idx` and `g_FsQueue.read.ptr`.
 *
 * Regardless of the outcome of the above, also ticks post-loading (`Fs_QueueUpdatePostLoad`) if there is an entry to post-load.
 * If that reports that the current post-load entry is done post-loading, advances `g_FsQueue.postLoad.idx` and `g_FsQueue.postLoad.ptr`.
 */
void Fs_QueueUpdate(void);

/** @brief Ticks a seek operation once.
 *
 * Performs one step in the seeking process according to `g_FsQueue.state`. When the whole process is done, returns `true`.
 *
 * @param entry Entry to tick.
 * @return `true` when `entry` is done seeking, `false` otherwise.
 */
bool Fs_QueueUpdateSeek(s_FsQueueEntry* entry);

/** @brief Ticks a read operation once.
 *
 * Performs one step in the reading process according to `g_FsQueue.state`. When the whole process is done, it returns `true`.
 *
 * @param entry Entry to tick.
 * @return `true` when `entry` is done loading, `false` otherwise.
 */
bool Fs_QueueUpdateRead(s_FsQueueEntry* entry);

/** If `entry->allocate` is set, allocate memory for `entry->data`, otherwise use `entry->externalData`.
 *
 * @param entry Entry to allocate memory for.
 * @return `true` if allocation was successful or was not needed, `false` otherwise.
 */
bool Fs_QueueAllocEntryData(s_FsQueueEntry* entry);

/** @brief Check if the specified read operation entry can be processed.
 *
 * Checks if loading `entry` will clobber any memory that was allocated for pending entries in the queue
 * or memory that's used for post-loading.
 *
 * @param entry Entry to check against.
 * @return `true` if the entry can be loaded without clobbering anything, `false` otherwise.
 */
bool Fs_QueueCanRead(s_FsQueueEntry* entry);

/** @brief Check if two buffers overlap in memory. Used by `Fs_QueueCanRead`.
 *
 * @param data0 Start of the first buffer.
 * @param size0 Size of the first buffer in bytes.
 * @param data1 Start of the second buffer.
 * @param size1 Size of the second buffer in bytes.
 * @return `true` if buffers overlap, `false` otherwise.
 */
bool Fs_QueueDoBuffersOverlap(u8* data0, u32 size0, u8* data1, u32 size1);

/** @brief Process `FsQueueReadState_SetLoc` or `FSQS_SEEK_SETLOC` state: set target sector.
 *
 * Calls `CdControl(CdlSetloc, ...)`.
 *
 * @param entry Entry to process.
 * @return `true` if succeded, `false` if `CdControl` failed.
 */
bool Fs_QueueTickSetLoc(s_FsQueueEntry* entry);

/** @brief Process `FsQueueReadState_Read` state: read from CD.
 *
 * Calls `CdRead()`.
 *
 * @param entry Entry to process.
 * @return `true` if succeded, `false` if `CdControl` failed.
 */
bool Fs_QueueTickRead(s_FsQueueEntry* entry);

/** @brief Process `FsQueueReadState_Reset` or `FsQueueSeekState_Reset` state: wait for a bit and reset CD driver.
 *
 * Increments `g_FsQueue.resetTimer0` once. If it has reached 8, clears it and increments `g_FsQueue.resetTimer1`.
 * If `g_FsQueue.resetTimer1` has reached 9, clears it and calls `CdReset()`.
 *
 * @param entry Entry to process.
 * @return `true` if succeded, `false if `CdControl` failed.
 */
bool Fs_QueueResetTick(s_FsQueueEntry* entry);

/** Process a read from PCDRV. Seems to be unused in release.
 * @param entry PCDRV read operation entry to process.
 * @return `true` if succeeded, `false` otherwise.
 */
bool Fs_QueueTickReadPcDrv(s_FsQueueEntry* entry);

/** @brief Ticks post-loading once.
 *
 * Performs one step in the post-loading process according to `entry->postLoadState`. When the whole process is done, returns 1.
 *
 * @param entry Entry to tick.
 * @return `true` when `entry` is done post-loading, `false` otherwise.
 */
bool Fs_QueueUpdatePostLoad(s_FsQueueEntry* entry);

/** @brief Parse a TIM file after loading it.
 *
 * Called during post-loading for TIM files (`entry->postLoad = 1`).
 * Will use `OpenTIM`/`ReadTIM` to parse the loaded data and then upload it via `LoadImage`.
 * If `entry->extra.image.u` is not 0xFF, will override the XY components of the pixel and CLUT rects with values from `image`.
 *
 * @param entry Entry to parse.
 * @return Always `true`.
 */
bool Fs_QueuePostLoadTim(s_FsQueueEntry* entry);

/** @brief Parse an ANM file after loading it?
 *
 * Called during post-loading for some ANM files (when `entry->postLoad = 2`).
 * For example, it is called for CLD2.ANM, SRL.ANM, SBL.ANM (maybe ones used for NPCs?), but not for others
 * (e.g. HB_M0S01.ANM).
 *
 * Calls into BODYPROG, where a function gets a `GsCOORDINATE2` from somewhere in the loaded data.
 * Always uses data from `entry->extra.anm`.
 *
 * @param entry Entry to parse.
 * @return Always `true`.
 */
bool Fs_QueuePostLoadAnm(s_FsQueueEntry* entry);

#endif
