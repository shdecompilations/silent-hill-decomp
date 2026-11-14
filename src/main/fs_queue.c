#include "gpu.h"
#include "bodyprog/bodyprog.h"
#include "bodyprog/sys/demo.h"
#include "main/fs_queue.h"
#include "main/fs_mem.h"

#include <memory.h>
#include <psyq/libapi.h>
#include <psyq/libcd.h>
#include <psyq/libgte.h>
#include <psyq/libgpu.h>
#include <psyq/string.h>
#include <psyq/sys/file.h>

/** @brief Wraps a filesystem queue entry index to `FS_QUEUE_LENGTH`.
 *
 * @param idx Index to wrap.
 * @return Wrapped `idx`.
 */
#define FSQUEUE_IDX_WRAP(idx) \
    ((u32)(idx) % FS_QUEUE_LENGTH)

s_FsQueue g_FsQueue;

const char* const a_PcDrvPath = "sim:.\\DATA";

bool Fs_QueueIsEntryLoaded(s32 queueIdx)
{
    return queueIdx < g_FsQueue.postLoad.idx;
}

s32 Fs_QueueGetLength(void)
{
    return (g_FsQueue.last.idx + 1) - g_FsQueue.postLoad.idx;
}

bool Fs_QueueChunksLoad(void)
{
    bool isFinished;

    g_Demo_IsLoadingChunks = true;

    isFinished = false;
    if (Fs_QueueGetLength() == 0)
    {
        isFinished = WorldGfx_ChunksInitCheck() != false;
    }

    return isFinished;
}

void Fs_QueueWaitForEmpty(void)
{
    func_800892A4(0);
    func_80089128();

    while (true)
    {
        VSync(SyncMode_Wait);
        if (Fs_QueueGetLength() <= 0)
        {
            break;
        }

        Fs_QueueUpdate();
    }

    func_800892A4(1);
    DrawSync(SyncMode_Wait);
    VSync(SyncMode_Wait);
}

s32 Fs_QueueStartSeek(e_FsFile fileIdx)
{
    return Fs_QueueEnqueue(fileIdx, FsQueueOp_Seek, FsQueuePostLoadType_None, false, NULL, 0, NULL);
}

s32 Fs_QueueStartRead(e_FsFile fileIdx, void* dest)
{
    return Fs_QueueEnqueue(fileIdx, FsQueueOp_Read, FsQueuePostLoadType_None, false, dest, 0, NULL);
}

s32 Fs_QueueStartReadTim(e_FsFile fileIdx, void* dest, const s_FsImageDesc* image)
{
    s_FsQueueExtra extra;

    if (image != NULL)
    {
        extra.image = *image;
    }
    else
    {
        extra.image.u     = NO_VALUE;
        extra.image.clutX = NO_VALUE;
        extra.image.v     = NO_VALUE;
        extra.image.clutY = NO_VALUE;
    }

    return Fs_QueueEnqueue(fileIdx, FsQueueOp_Read, FsQueuePostLoadType_Tim, false, dest, 0, &extra);
}

s32 Fs_QueueStartReadAnm(s32 animDataIdx, s32 charaId, void* dest, GsCOORDINATE2* boneCoords)
{
    s32            fileIdx;
    s_FsQueueExtra extra;

    fileIdx               = CHARA_FILE_INFOS[charaId].animFileIdx;
    extra.anm.charaId     = charaId;
    extra.anm.animDataIdx = animDataIdx;
    extra.anm.boneCoords  = boneCoords;
    return Fs_QueueEnqueue(fileIdx, FsQueueOp_Read, FsQueuePostLoadType_Anm, false, dest, 0, &extra);
}

s32 Fs_QueueEnqueue(e_FsFile fileIdx, u8 op, u8 postLoad, u8 alloc, void* data, u32 unused0, s_FsQueueExtra* extra)
{
    s_FsQueueEntry* newEntry;
    s_FsQueuePtr*   lastPtr;

    // Wait for space in queue.
    while (Fs_QueueGetLength() >= FS_QUEUE_LENGTH)
    {
        Fs_QueueUpdate();
    }

    // This is the reason these pointers and indices are wrapped into structs.
    // If left as they are in the queue struct, this doesn't match unless manually addressed.
    lastPtr = &g_FsQueue.last;
    lastPtr->idx++;
    lastPtr->ptr = &g_FsQueue.entries[FSQUEUE_IDX_WRAP(lastPtr->idx)];

    newEntry               = g_FsQueue.last.ptr;
    newEntry->info         = &g_FileTable[fileIdx];
    newEntry->operation    = op;
    newEntry->postLoad     = postLoad;
    newEntry->allocate     = alloc;
    newEntry->externalData = data;
    newEntry->unused1      = unused0;

    if (extra != NULL)
    {
        newEntry->extra = *extra;
    }

    return g_FsQueue.last.idx;
}

void Fs_QueueInitialize(void)
{
    bzero(&g_FsQueue, sizeof(g_FsQueue));

    g_FsQueue.last.idx      = NO_VALUE;
    g_FsQueue.last.ptr      = &g_FsQueue.entries[FS_QUEUE_LENGTH - 1];
    g_FsQueue.read.idx      = 0;
    g_FsQueue.read.ptr      = g_FsQueue.entries;
    g_FsQueue.postLoad.idx  = 0;
    g_FsQueue.postLoad.ptr  = g_FsQueue.entries;
    g_FsQueue.state         = 0;
    g_FsQueue.postLoadState = 0;
    g_FsQueue.resetTimer0   = 0;
    g_FsQueue.resetTimer1   = 0;

    Fs_InitializeMem(FS_MEM_BASE, FS_MEM_SIZE);
}

void Fs_QueueReset(void)
{
    if (Fs_QueueGetLength() <= 0)
    {
        return;
    }

    if (g_FsQueue.read.idx <= g_FsQueue.last.idx)
    {
        g_FsQueue.read.idx = g_FsQueue.read.idx + FS_QUEUE_LENGTH;
        g_FsQueue.read.ptr = &g_FsQueue.entries[FSQUEUE_IDX_WRAP(g_FsQueue.read.idx)];
        g_FsQueue.last     = g_FsQueue.read;
    }

    g_FsQueue.postLoad           = g_FsQueue.read;
    g_FsQueue.postLoadState      = FsQueuePostLoadState_Init;
    g_FsQueue.read.ptr->postLoad = FsQueuePostLoadType_None;
}

void Fs_QueueUpdate(void)
{
    s_FsQueuePtr*   read;
    s_FsQueueEntry* tick;
    bool            status;

    status = false;

    // Pending read/seek operations; tick them.
    tick = g_FsQueue.read.ptr;
    if (g_FsQueue.read.idx <= g_FsQueue.last.idx)
    {
        switch (tick->operation)
        {
            case FsQueueOp_Seek:
                status = Fs_QueueUpdateSeek(tick);
                break;

            case FsQueueOp_Read:
                status = Fs_QueueUpdateRead(tick);
                break;
        }

        // Seek or read done, proceed to next one.
        if (status == true)
        {
            s32 idx;
            read                  = &g_FsQueue.read;
            g_FsQueue.state       = 0; // `FsQueueReadState_Allocate` or `FSQS_SEEK_SETLOC`.
            g_FsQueue.resetTimer0 = 0;
            g_FsQueue.resetTimer1 = 0;
            idx                   = ++read->idx;
            read->ptr             = &g_FsQueue.entries[FSQUEUE_IDX_WRAP(idx)];
        }
    }
    // Nothing to read.
    else
    {
        g_FsQueue.state = 0; // `FsQueueReadState_Allocate` or `FSQS_SEEK_SETLOC`.
    }

    // Preparations to post-load in queue; tick them.
    tick = g_FsQueue.postLoad.ptr;
    if (g_FsQueue.postLoad.idx < g_FsQueue.read.idx)
    {
        status = Fs_QueueUpdatePostLoad(tick);
        if (status == true)
        {
            g_FsQueue.postLoadState = FsQueuePostLoadState_Init;
            status                  = ++g_FsQueue.postLoad.idx;
            g_FsQueue.postLoad.ptr  = &g_FsQueue.entries[FSQUEUE_IDX_WRAP(status)];
        }
    }
    // Nothing to post-load.
    else
    {
        g_FsQueue.postLoadState = FsQueuePostLoadState_Init;
    }
}

bool Fs_QueueUpdateSeek(s_FsQueueEntry* entry)
{
    bool result;
    s32  state;

    result = false;
    state  = g_FsQueue.state;

    switch (state)
    {
        case FsQueueSeekState_SetLoc:
            switch (Fs_QueueTickSetLoc(entry))
            {
                // CdlSetloc failed, reset and retry.
                case false:
                    g_FsQueue.state = FsQueueSeekState_Reset;
                    break;

                case true:
                    g_FsQueue.state = FsQueueSeekState_SeekL;
                    break;
            }
            break;

        case FsQueueSeekState_SeekL:
            switch (CdControl(CdlSeekL, NULL, NULL))
            {
                // `CdlSeekL` failed, reset and retry.
                case 0:
                    g_FsQueue.state = FsQueueSeekState_Reset;
                    break;

                case 1:
                    g_FsQueue.state = FsQueueSeekState_Sync;
                    break;
            }
            break;

        case FsQueueSeekState_Sync:
            switch (CdSync(1, NULL))
            {
                // Keep waiting, operation in progress.
                case CdlNoIntr:
                    break;

                // Done seeking.
                case CdlComplete:
                    result = true;
                    break;

                // Disk error; reset and retry.
                case CdlDiskError:
                    g_FsQueue.state = FsQueueSeekState_Reset;
                    break;

                // Inknown error, reset and retry.
                default:
                    g_FsQueue.state = FsQueueSeekState_Reset;
                    break;
            }
            break;

        case FsQueueSeekState_Reset:
            switch (Fs_QueueResetTick(entry))
            {
                // Still resetting.
                case 0:
                    break;

                // Reset done, retry from beginning.
                case 1:
                    g_FsQueue.state = FsQueueSeekState_SetLoc;
                    break;
            }
            break;
    }

    return result;
}

bool Fs_QueueUpdateRead(s_FsQueueEntry* entry)
{
    bool result;

    result = false;
    switch (g_FsQueue.state)
    {
        case FsQueueReadState_Allocate:
            switch (Fs_QueueAllocEntryData(entry))
            {
                // Retry until memory is allocated?
                case 0:
                    break;

                case 1:
                    g_FsQueue.state = FsQueueReadState_Check;
                    break;
            }

            break;

        case FsQueueReadState_Check:
            switch (Fs_QueueCanRead(entry))
            {
                // Can't read yet, memory in use by another operation. Wait until next tick.
                case 0:
                    break;

                case 1:
                    g_FsQueue.state = FsQueueReadState_SetLoc;
                    break;
            }

            break;

        case FsQueueReadState_SetLoc:
            switch (Fs_QueueTickSetLoc(entry))
            {
                // `CdlSetloc` failed, reset CD.
                case false:
                    g_FsQueue.state = FsQueueReadState_Reset;
                    break;

                case true:
                    g_FsQueue.state = FsQueueReadState_Read;
                    break;
            }

            break;

        case FsQueueReadState_Read:
            switch (Fs_QueueTickRead(entry))
            {
                // `CdRead` failed, reset CD and retry.
                case 0:
                    g_FsQueue.state = FsQueueReadState_Reset;
                    break;

                case 1:
                    g_FsQueue.state = FsQueueReadState_Sync;
                    break;
            }

            break;

        // Check how read is going.
        case FsQueueReadState_Sync:
            switch (CdReadSync(1, NULL))
            {
                // `CdReadSync` failed, reset CD.
                case NO_VALUE:
                    g_FsQueue.state = FsQueueReadState_Reset;
                    break;

                // Done reading and no state transition, let caller know that it's done.
                case 0:
                    result = true;
                    break;
            }

            break;

        case FsQueueReadState_Reset:
            switch (Fs_QueueResetTick(entry))
            {
                // Still resetting.
                case 0:
                    break;

                // Reset done, retry from `setloc`.
                case 1:
                    g_FsQueue.state = FsQueueReadState_SetLoc;
                    break;
            }

            break;
    }

    return result;
}

bool Fs_QueueAllocEntryData(s_FsQueueEntry* entry)
{
    bool result;
    
    result = false;

    if (entry->allocate)
    {
        entry->data = Fs_AllocMem(ALIGN(entry->info->blockCount * FS_BLOCK_SIZE, FS_SECTOR_SIZE));
    }
    else
    {
        entry->data = entry->externalData;
    }

    if (entry->data != 0)
    {
        result = true;
    }
    return result;
}

bool Fs_QueueCanRead(s_FsQueueEntry* entry)
{
    s_FsQueueEntry* other;
    s32             queueLength;
    bool            hasOverlap;
    s32             i;

    queueLength = g_FsQueue.read.idx - g_FsQueue.postLoad.idx;

    for (i = 0; i < queueLength; i++)
    {
        other      = &g_FsQueue.entries[FSQUEUE_IDX_WRAP(g_FsQueue.postLoad.idx + i)];
        hasOverlap = false;
        if (other->postLoad || other->allocate)
        {
            hasOverlap = Fs_QueueDoBuffersOverlap(entry->data,
                                                  ALIGN(entry->info->blockCount * FS_BLOCK_SIZE, FS_SECTOR_SIZE),
                                                  other->data,
                                                  other->info->blockCount * FS_BLOCK_SIZE);
        }

        if (hasOverlap == true)
        {
            return false;
        }
    }

    return true;
}

bool Fs_QueueDoBuffersOverlap(u8* data0, u32 size0, u8* data1, u32 size1)
{
    u32 data0Low;
    u32 data1Low;

    data0Low = (u32)data0 & 0xFFFFFF;
    data1Low = (u32)data1 & 0xFFFFFF;

    if (data1Low >= (data0Low + size0) || data0Low >= (data1Low + size1))
    {
        return false;
    }

    return true;
}

bool Fs_QueueTickSetLoc(s_FsQueueEntry* entry)
{
    CdlLOC cdloc;

    CdIntToPos(entry->info->startSector, &cdloc);
    return CdControl(CdlSetloc, (u_char*)&cdloc, NULL);
}

bool Fs_QueueTickRead(s_FsQueueEntry* entry)
{
    s32 sectorCount;

    // Round up to sector boundary. Masking not needed because of `>> 11` below.
    sectorCount = ((entry->info->blockCount * FS_BLOCK_SIZE) + FS_SECTOR_SIZE) - 1;

    // Overflow check?
    if (sectorCount < 0)
    {
        sectorCount += FS_SECTOR_SIZE - 1;
    }

    return CdRead(sectorCount >> FS_SECTOR_SHIFT, (u32*)entry->data, CdlModeSpeed);
}

bool Fs_QueueResetTick(s_FsQueueEntry* entry)
{
    bool result;

    result = false;

    g_FsQueue.resetTimer0++;

    if (g_FsQueue.resetTimer0 >= 8)
    {
        result                = true;
        g_FsQueue.resetTimer0 = 0;
        g_FsQueue.resetTimer1++;

        if (g_FsQueue.resetTimer1 >= 9)
        {
            if (CdReset(0) == 1)
            {
                g_FsQueue.resetTimer1 = 0;
            }
            else
            {
                result = false;
            }
        }
    }

    return result;
}

bool Fs_QueueTickReadPcDrv(s_FsQueueEntry* entry)
{
    s32         handle;
    s32         retry;
    bool        result;
    s_FileInfo* file;
    char        pathBuf[64];
    char        nameBuf[32];

    file = entry->info;
    result = false;

    strcpy(pathBuf, a_PcDrvPath);
    strcat(pathBuf, g_FilePaths[file->pathIdx]);
    Fs_GetFileInfoName(nameBuf, file);
    strcat(pathBuf, nameBuf);

    for (retry = 0; retry <= 2; retry++)
    {
        handle = open(pathBuf, O_NOBUF | O_RDONLY);
        if (handle == NO_VALUE)
        {
            continue;
        }

        if (read(handle,entry->data, ALIGN(file->blockCount * FS_BLOCK_SIZE, FS_SECTOR_SIZE)) == NO_VALUE)
        {
            continue;
        }

        while(close(handle) == NO_VALUE)
        {
        }

        result = true;
        break;
    }

    return result;
}

bool Fs_QueueUpdatePostLoad(s_FsQueueEntry* entry)
{
    bool result;
    s32  state;
    u8   postLoad;

    result = false;
    state  = g_FsQueue.postLoadState;

    switch (state)
    {
        case FsQueuePostLoadState_Init:
            if (entry->allocate)
            {
                g_FsQueue.postLoadState = FsQueuePostLoadState_Skip;
            }
            else
            {
                g_FsQueue.postLoadState = FsQueuePostLoadState_Exec;
            }
            break;

        // Do nothing.
        case FsQueuePostLoadState_Skip:
            break;

        case FsQueuePostLoadState_Exec:
            postLoad = entry->postLoad;

            switch (postLoad)
            {
                case FsQueuePostLoadType_None:
                    result = true;
                    break;

                case FsQueuePostLoadType_Tim:
                    result = Fs_QueuePostLoadTim(entry);
                    break;

                case FsQueuePostLoadType_Anm:
                    result = Fs_QueuePostLoadAnm(entry);
                    break;

                default:
                    break;
            }
            break;

        default:
            break;
    }

    return result;
}

bool Fs_QueuePostLoadTim(s_FsQueueEntry* entry)
{
    TIM_IMAGE tim;
    RECT      tempRect;

    OpenTIM((u64*)entry->externalData);
    ReadTIM(&tim);

    tempRect = *tim.prect;
    if (entry->extra.image.u != UCHAR_MAX)
    {
        // This contraption simply extracts XY from tPage value.
        // For some reason it seems to be byte swapped, or maybe tPage is stored as u8[2]?
        // Same as `tempRect.x = (entry->extra.image.tPage & 0x0F) * 64` for normal tPage.
        tempRect.x = entry->extra.image.u + ((entry->extra.image.tPage[1] & 0xF) << 6);

        // Same as `tempRect.y = (entry->extra.image.tPage & 0x10) * 16` for normal tPage.
        tempRect.y = entry->extra.image.v + ((entry->extra.image.tPage[1] << 4) & 0x100);
    }

    LoadImage(&tempRect, tim.paddr);

    if (tim.caddr != NULL)
    {
        tempRect = *tim.crect;
        if (entry->extra.image.clutX != NO_VALUE)
        {
            tempRect.x = entry->extra.image.clutX;
            tempRect.y = entry->extra.image.clutY;
        }

        LoadImage(&tempRect, tim.caddr);
    }

    return true;
}

bool Fs_QueuePostLoadAnm(s_FsQueueEntry* entry)
{
    Fs_CharaAnimDataUpdate(entry->extra.anm.animDataIdx, entry->extra.anm.charaId, entry->externalData,
                           entry->extra.anm.boneCoords);
    return true;
}
