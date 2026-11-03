#pragma once
/**
 * PC file system — replaces Fs_Queue + CD on PC.
 *
 * On PSX, files live in a flat sector table inside SILENT./HILL. and are
 * read via CdRead + Fs_Queue. On PC we extract them once (tools/silentassets)
 * and load them as regular files from assets/USA/.
 *
 * This shim keeps the e_FsFile enum and Fs_Queue API so game code compiles
 * unchanged, but the backend is POSIX fopen/fread + async thread pool.
 */

#include "decomp/types.h"
#include "main/fileinfo.h"

#ifdef PC_PORT

#include <stddef.h>
#include <stdbool.h>

#define PC_FS_MAX_QUEUE  16
#define PC_FS_MAX_PATH   512

typedef enum _PcFsState {
    PcFsState_Idle = 0,
    PcFsState_Loading = 1,
    PcFsState_Ready = 2,
    PcFsState_Error = 3,
} e_PcFsState;

typedef struct _PcFsEntry {
    e_FsFile  fileId;
    void*     buffer;     // destination (Fs_Queue's FS_BUFFER_n concept)
    size_t    size;
    e_PcFsState state;
    int       priority;
} s_PcFsEntry;

// Global queue — mirrors PSX's Fs_Queue
extern s_PcFsEntry g_PcFsQueue[PC_FS_MAX_QUEUE];
extern int g_PcFsQueueLen;

// Init with the extracted assets directory, e.g. "assets/USA"
void PcFs_Init(const char* assetsRoot);
void PcFs_Shutdown(void);

// Queue a file for async load — mirrors Fs_QueueStartRead(file, buffer)
void PcFs_QueueStartRead(e_FsFile file, int bufferId);
void PcFs_QueueStartReadTim(e_FsFile file, int bufferId, void* imageDesc);

// Tick — pump async loads, call from main loop
void PcFs_Update(void);

// Blocking helpers for boot
void* PcFs_LoadFile(e_FsFile file, size_t* outSize);
void  PcFs_FreeFile(void* data);

// Sync status
int  PcFs_GetQueueLength(void);
bool PcFs_ChunksLoad(void); // mirrors Fs_QueueChunksLoad: true when all ready

// Path resolution: e_FsFile → host path
const char* PcFs_GetPath(e_FsFile file);
size_t      PcFs_GetSize(e_FsFile file);

// Compatibility shims so existing src/main/fsqueue.c compiles under PC_PORT
// We provide Fs_Queue* aliases that delegate to PcFs_*
void Fs_QueueInitialize(void);
void Fs_QueueStartRead(e_FsFile file, s32 bufferId);
void Fs_QueueStartReadTim(e_FsFile file, s32 bufferId, void* img);
void Fs_QueueUpdate(void);
int  Fs_QueueGetLength(void);
bool Fs_QueueChunksLoad(void);
s32  Fs_QueueGetFileSize(e_FsFile file);
void Fs_QueueWaitForEmpty(void);

#define FS_BUFFER_0   0
#define FS_BUFFER_1   1
#define FS_BUFFER_16  16

extern void* FS_BUFFER_0_PTR;
extern void* FS_BUFFER_16_PTR;
extern void* g_FsExtraBuffers[32];

#else
// PSX: original fsqueue.h
#endif
