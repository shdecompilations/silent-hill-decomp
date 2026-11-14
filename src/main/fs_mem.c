#include "main/fs_mem.h"

s_FsMemState g_FsMemory;

void Fs_InitializeMem(u8* start, u32 size)
{
    s32 i;

    // This really wants the start and size to be aligned to 8,
    // but since they're already hardcoded elsewhere, it doesn't really matter.
    if ((u32)start & 0x3)
    {
        size = 0;
    }
    if (size & 0x3)
    {
        size = 0;
    }

    g_FsMemory.start           = start;
    g_FsMemory.size            = size;
    g_FsMemory.allocList.next  = NULL;
    g_FsMemory.allocList.start = start;
    g_FsMemory.allocList.size  = 0;
    g_FsMemory.freeList.next   = g_FsMemory.blocks;
    g_FsMemory.freeList.start  = NULL;
    g_FsMemory.freeList.size   = 0;

    for (i = 0; i < FS_MEM_BLOCK_COUNT; i++)
    {
        g_FsMemory.blocks[i].start = NULL;
        g_FsMemory.blocks[i].size  = 0;

        if (i == FS_MEM_BLOCK_COUNT - 1)
        {
            g_FsMemory.blocks[i].next = NULL;
        }
        else
        {
            g_FsMemory.blocks[i].next = &g_FsMemory.blocks[i + 1];
        }
    }
}

void nullsub_80011cfc(void) {}

void* Fs_AllocMem(s32 size)
{
    s_FsMemBlock  bufMin; // Seems to use this struct for some reason.
    s_FsMemBlock* blockMin;
    s_FsMemBlock* iter;
    s32           clampedSize;
    u8*           start;
    u8*           end;

    if (size == 0)
    {
        return NULL;
    }

    // Align to 4.
    size = (size + 3) & ~0x3;

    // No free blocks left.
    if (g_FsMemory.freeList.next == NULL)
    {
        return NULL;
    }

    iter         = &g_FsMemory.allocList;
    bufMin.start = NULL;
    bufMin.size  = 0x0FFFFFFF;
    blockMin     = NULL;

    for (; iter != NULL; iter = iter->next)
    {
        // Last allocation; check if there's space left after it.
        if (iter->next == NULL)
        {
            start       = iter->start + iter->size;
            end         = g_FsMemory.start + g_FsMemory.size;
            clampedSize = Fs_ClampMemBlock(start, end);
        }
        // Check if there's space left between current and nex allocation.
        else
        {
            end         = iter->next->start;
            start       = iter->start + iter->size;
            clampedSize = Fs_ClampMemBlock(start, end);
        }

        // Track smallest possible hole new block can fit into.
        if (clampedSize >= size && clampedSize < (s32)bufMin.size)
        {
            blockMin     = iter;
            bufMin.size  = clampedSize;
            bufMin.start = blockMin->start + blockMin->size;
        }
    }

    if (bufMin.start != NULL)
    {
        Fs_RelinkMemBlock(&g_FsMemory.freeList, blockMin, bufMin.start, size);
    }

    return bufMin.start;
}

static inline u8* Fs_ClampToHeapBounds(u8* ptr)
{
    u8* min;
    u8* max;

    min = g_FsMemory.start;
    if (ptr < min)
    {
        return min;
    }

    max = min + g_FsMemory.size;
    if (ptr > max)
    {
        return max;
    }

    return ptr;
}

s32 Fs_ClampMemBlock(u8* start, u8* end)
{
    u8* clampedStart;
    u8* clampedEnd;

    clampedStart = Fs_ClampToHeapBounds(start);
    clampedEnd   = Fs_ClampToHeapBounds(end);

    if (clampedEnd < clampedStart)
    {
        return 0;
    }

    return clampedEnd - clampedStart;
}

bool Fs_FreeMem(u8* ptr)
{
    s_FsMemBlock* iter;
    s_FsMemBlock* prev;
    bool          result;

    prev   = &g_FsMemory.allocList;
    result = false;

    while (prev->next != NULL)
    {
        iter = prev->next;
        if (iter->start == ptr)
        {
            Fs_RelinkMemBlock(prev, &g_FsMemory.freeList, NULL, 0u);
            result = true;
            break;
        }

        prev = iter;
    }

    return result;
}

void Fs_RelinkMemBlock(s_FsMemBlock* from, s_FsMemBlock* to, u8* start, u32 size)
{
    s_FsMemBlock* temp;

    temp       = from->next;
    from->next = temp->next;

    temp->next = to->next;
    to->next   = temp;

    temp->start = start;
    temp->size  = size;
}
