#ifndef _MAIN_FSMEM_H
#define _MAIN_FSMEM_H

#include "common.h"

#define FS_MEM_BLOCK_COUNT 16       /** Number of FS memory blocks that can be allocated. */
#define FS_MEM_BASE        0x1C0000 /** FS heap base (0x801C0000). */
#define FS_MEM_SIZE        0x20000  /** FS heap size (128kb). */

/** @brief FS memory block.
 * Describes one allocation in the FS heap.
 */
typedef struct s_FsMemBlock
{
    u8*                  start; /** Start address. */
    u32                  size;  /** Size in bytes. */
    struct s_FsMemBlock* next;  /** Next block in list. */
} s_FsMemBlock;

/** @brief State of the FS memory heap.
 * This heap seems to only be used by the FS code.
 */
typedef struct _FsMemState
{
    u8*          start;                      /** Heap base. */
    u32          size;                       /** Total heap size in bytes. */
    s_FsMemBlock allocList;                  /** Head of list of allocated memory blocks. */
    s_FsMemBlock freeList;                   /** Head of list of free memory blocks. */
    s_FsMemBlock blocks[FS_MEM_BLOCK_COUNT]; /** Block pool. */
} s_FsMemState;

/** FS memory heap. */
extern s_FsMemState g_FsMemory;

void nullsub_800120b0(void);
void nullsub_80011cfc(void);

/** @brief Initialize FS memory heap.
 * @param start Heap base.
 * @param size Total heap size in bytes.
 */
void Fs_InitializeMem(u8* start, u32 size);

/** @brief Allocate FS memory block.
 * @param size Number of bytes to allocate.
 * @return Allocated memory block.
 */
void* Fs_AllocMem(s32 size);

/** @brief Clamp memory buffer size to heap bounds.
 * @param start Buffer start.
 * @param size Buffer end.
 * @return New block size, or 0 if it does not fit or is invalid.
 */
s32 Fs_ClampMemBlock(u8* start, u8* end);

/** @brief Free FS memory block.
 * @param ptr Address of memory block to free.
 * @return `true` if block was freed, `false` if it wasn't found.
 */
bool Fs_FreeMem(u8* ptr);

/** @brief Unlink an FS memblock from one list and link it to another.
 *
 * Unlinks `from->next` and links it to `to` (`to->next` becomes `from->next`).
 * Used to both allocate (`from = &free_list`) and free (`from` is in `alloc_list`).
 *
 * @param from List node to unlink from.
 * @param to List node to link to.
 * @param start New block base.
 * @param size New block size.
 */
void Fs_RelinkMemBlock(s_FsMemBlock* from, s_FsMemBlock* to, u8* start, u32 size);

#endif
