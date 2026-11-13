#include "bodyprog/game.h"

#include <psyq/libapi.h>
#include <psyq/limits.h>
#include <psyq/strings.h>
#include <psyq/sys/file.h>

#include "main/fsqueue.h"
#include "bodyprog/sys/memcard.h"

#ifndef PAD_HACK_IGNORE
    s32 __pad_bss_800B5484;
#endif

/** @unused Formatting feature.
 * Commonly PS1 features an option to format the PS1. This game also count with a fully working
 * format feature, but there is no way to to naturally trigger it.
 */

// ========================================
// STATIC VARIABLES
// ========================================

static s_MemCard_SaveHeader g_MemCard_SaveHeaderInfo_Slot1[MEMCARD_FILE_COUNT_MAX];
static s_MemCard_SaveHeader g_MemCard_SaveHeaderInfo_Slot2[MEMCARD_FILE_COUNT_MAX];
static s_MemCard_SaveHeader g_MemCard_SaveHeaderInfo_Null[MEMCARD_FILE_COUNT_MAX];

/** @brief Handles processes related to the save game, access and management of the memory card. */
static s_MemCardSaveWork g_MemCardSaveWork;

/** @brief Define if the memory card system is currently active or not. */
static bool               g_MemCard_SysAvailibityStatus;

/** @brief Handles memory card processes from the console. */
static s_MemCardWork     g_MemCardWork;

u32 g_MemCard_SaveIconTim[48] = {
    0x00000010, 0x00000008, 0x0000002C, 0x00000000,
    0x00010010, 0x84438000, 0x88658C62, 0x8CA798A4,
    0xA10794C8, 0xA92794EA, 0xA16C992C, 0xA9F3A58F,
    0xAE37AE16, 0x0000008C, 0x00000000, 0x00100004,
    0x65553300, 0x00001136, 0x01113310, 0x00033111,
    0x58ACC610, 0x00132111, 0xFFEEFC00, 0x0022111A,
    0xFEDEED00, 0x0022103D, 0xEEEFFD10, 0x0022116D,
    0x633AA510, 0x0022158A, 0x001CA500, 0x00745CA5,
    0xCBDEEC00, 0x00BB7CDE, 0xDEDDEC00, 0x004B79BC,
    0xACC8A800, 0x00049978, 0x8BCA8500, 0x00049976,
    0x65558100, 0x00097777, 0x58AAA000, 0x000B9444,
    0x246A5000, 0x006CB422, 0x01300000, 0x06DCCA31,
};

// ========================================
// INLINE FUNCTIONS
// ========================================

static inline void MemCard_DirectoryFileClear(s32 idx)
{
    strcpy(g_MemCardWork.directories->filenames[idx], "");
    g_MemCardWork.directories->blockCounts[idx] = 0;
}

static inline void MemCard_SaveWork_SetParams(s_MemCard_GameProcess* ptr, s32 processId, s32 deviceId, s32 fileIdx, s32 saveIdx, s32 state, s32 lastMemCardResult)
{
    ptr->processId         = processId;
    ptr->deviceId          = deviceId;
    ptr->fileIdx           = fileIdx;
    ptr->saveIdx           = saveIdx;
    ptr->processState      = state;
    ptr->lastMemCardResult = lastMemCardResult;
}

// ========================================
// MEMORY CARD - INITIALIZATION
// ========================================

void MemCard_SysInit(void)
{
    s32                   i;
    s_MemCard_SaveHeader* saveHeaderInfoPtr;

    MemCard_WorkInit();

    g_MemCard_SysAvailibityStatus = false;

    // Clear specific game memory card system arrays.
    bzero(&g_MemCardSaveWork, sizeof(s_MemCardSaveWork));
    bzero(g_MemCard_SaveHeaderInfo_Slot1, sizeof(s_MemCard_SaveHeader) * 3);

    // Clears any information from the device information
    // and assign the correspondent `g_MemCard_SaveHeaderInfo`
    // pointer to the memory card.
    for (i = 0; i < MEMCARD_DEVICE_COUNT_MAX; i++)
    {
        g_MemCardSaveWork.devices[i].status = MemCardState_Null;

        MemCard_FileStatusClear(i); // This is redundant as `MemCard_DeviceInfoClear` will also call this function.

        switch (i)
        {
            case 0: // Slot 1
                saveHeaderInfoPtr = g_MemCard_SaveHeaderInfo_Slot1;
                break;

            case 4: // Slot 2
                saveHeaderInfoPtr = g_MemCard_SaveHeaderInfo_Slot2;
                break;

            default:
                saveHeaderInfoPtr = g_MemCard_SaveHeaderInfo_Null;
                break;
        }

        g_MemCardSaveWork.devices[i].saveHeader = saveHeaderInfoPtr;

        MemCard_DeviceInfoClear(i);
    }
}

/** @brief Clear PSX device information and data.
 *
 * Scratch 1: https://decomp.me/scratch/b2iIE
 * Scratch 2: https://decomp.me/scratch/phao2
 *
 * @param deviceId Memory card index.
 */
static void MemCard_DeviceInfoClear(s32 deviceId)
{
    g_MemCardSaveWork.devices[deviceId].status = MemCardState_Null;

    MemCard_FileStatusClear(deviceId);
    bzero(g_MemCardSaveWork.devices[deviceId].saveHeader, sizeof(s_MemCard_SaveHeader) * MEMCARD_FILE_COUNT_MAX);

    g_MemCardSaveWork.devices[deviceId].fileLimit = 0;
}

/** @brief Clear all files from a specified memory card.
 *
 * Scratch 1: https://decomp.me/scratch/EIyBh
 * Scratch 2: https://decomp.me/scratch/T3buH
 *
 * @param deviceId Memory card index.
 */
static void MemCard_FileStatusClear(s32 deviceId)
{
    s32 i;

    for (i = 0; i < MEMCARD_FILE_COUNT_MAX; i++)
    {
        g_MemCardSaveWork.devices[deviceId].fileState[i] = FileState_Unused;
    }
}

/** @brief Checks if all files from a specified memory card are unused.
 *
 * Scratch: https://decomp.me/scratch/jQRDn
 *
 * @param deviceId Memory card index.
 * @return True if all files are unused, false if any file is used (either by being broken or being used).
 */
static bool MemCard_AreAllFilesUsed(s32 deviceId)
{
    bool result;
    s32  i;

    result = true;
    for (i = 0; i < MEMCARD_FILE_COUNT_MAX; i++)
    {
        if (g_MemCardSaveWork.devices[deviceId].fileState[i] != FileState_Unused)
        {
            result = false;
            break;
        }
    }

    return result;
}

void MemCard_SysEnable(void)
{
    if (g_MemCard_SysAvailibityStatus == true)
    {
        return;
    }

    g_MemCard_SysAvailibityStatus = true;
    MemCard_InitStatusSuccess();
    MemCard_EventsInit();

    MemCard_SaveWork_SetParams(&g_MemCardSaveWork.saveWork[0], 0, 0, 0, 0, 0, MemCardWorkResult_NotConnected);
    MemCard_SaveWork_SetParams(&g_MemCardSaveWork.saveWork[1], 0, 0, 0, 0, 0, MemCardWorkResult_NotConnected);
}

void MemCard_SysDisable(void)
{
    if (g_MemCard_SysAvailibityStatus != false)
    {
        g_MemCard_SysAvailibityStatus = false;
        MemCard_EventsClose();
    }
}

void MemCard_InitStatus(void)
{
    g_MemCardSaveWork.memCardInitalized = true;
}

void MemCard_InitStatusNotConnected(void)
{
    g_MemCardSaveWork.memCardInitalized = false;

    MemCard_SaveWork_SetParams(&g_MemCardSaveWork.saveWork[1], 0, 0, 0, 0, 0, MemCardWorkResult_NotConnected);
}

s32 MemCard_AllMemCardsStatusGet(void)
{
    s32 ret;
    s32 i;

    ret = 0;
    for (i = 0; i < MEMCARD_DEVICE_COUNT_MAX; i++)
    {
        ret |= MemCard_StatusStore(g_MemCardSaveWork.devices[i].status, i);
    }

    return ret;
}

void func_8002E8D4(void)
{
    g_MemCardSaveWork.memCardInitalized = true;
}

void MemCard_InitStatusSuccess(void)
{
    g_MemCardSaveWork.memCardInitalized = false;

    MemCard_SaveWork_SetParams(&g_MemCardSaveWork.saveWork[1], 0, 0, 0, 0, 0, MemCardWorkResult_Success);
}

s32 func_8002E914(void)
{
    s32 ret;
    s32 i;

    ret = 0;
    for (i = 0; i < MEMCARD_DEVICE_COUNT_MAX; i++)
    {
        ret |= MemCard_FileStatusStore(g_MemCardSaveWork.devices[i].status, i);
    }

    return ret;
}

bool MemCard_ProcessSet(s32 processId, s32 deviceId, s32 fileIdx, s32 saveIdx)
{
    if (g_MemCardSaveWork.saveWork[0].processId != MemCardGameProcessId_None)
    {
        return false;
    }

    MemCard_SaveWork_SetParams(&g_MemCardSaveWork.saveWork[0], processId, deviceId, fileIdx, saveIdx, 0, MemCardWorkResult_Success);
    return true;
}

s32 MemCard_LastMemCardResultGet(void)
{
    return g_MemCardSaveWork.saveWork[0].lastMemCardResult;
}

s32 MemCard_FileStatusesGet(s32 deviceId)
{
    s32 ret;
    s32 i;

    ret = 0;

    for (i = 0; i < MEMCARD_FILE_COUNT_MAX; i++)
    {
        ret |= MemCard_FileStatusStore(g_MemCardSaveWork.devices[deviceId].fileState[i], i);
    }

    return ret;
}

s_MemCard_SaveMetadata* MemCard_SaveMetadataGet(s32 deviceId, s32 fileIdx, s32 saveIdx)
{
    return &g_MemCardSaveWork.devices[deviceId].saveHeader[fileIdx].saveMetadata[saveIdx];
}

s32 MemCard_UsedFileCount(s32 deviceId)
{
    s32 ret;
    s32 i;

    ret = 0;

    for (i = 0; i < MEMCARD_FILE_COUNT_MAX; i++)
    {
        if (g_MemCardSaveWork.devices[deviceId].fileState[i] != FileState_Unused)
        {
            ret++;
        }
    }

    return ret;
}

s32 MemCard_FreeFilesCount(s32 deviceId)
{
    return g_MemCardSaveWork.devices[deviceId].fileLimit - MemCard_UsedFileCount(deviceId);
}

bool MemCard_NoSavesDoneCheck(s32* outDeviceId, s32* outFileIdx, s32* outSaveIdx)
{
    s_MemCard_TotalSavesInfo saveInfo;
    s32                      i;
    s32                      totalSavegameCount;

    totalSavegameCount = 0;

    *outDeviceId = 0;
    *outFileIdx  = 0;
    *outSaveIdx  = 0;

    for (i = 0; i < MEMCARD_DEVICE_COUNT_MAX; i++)
    {
        if (g_MemCardSaveWork.devices[i].status == MemCardState_Available)
        {
            MemCard_SaveWithBiggestTotalSavegameCountGet(i, &saveInfo);

            if (totalSavegameCount < saveInfo.totalSavegameCount)
            {
                *outDeviceId = i;
                *outFileIdx  = saveInfo.fileIdx;
                *outSaveIdx  = saveInfo.saveIdx;

                totalSavegameCount = saveInfo.totalSavegameCount;
            }
        }
    }

    return totalSavegameCount != 0;
}

// ========================================
// MEMORY CARD - PROCESSES
// ========================================

void MemCard_Update(void)
{
    s_MemCard_GameProcess* statusPtr;

    if (g_MemCard_SysAvailibityStatus == false)
    {
        return;
    }

    MemCard_StateUpdate();

    // Sets `statusPtr` to store the pointer that has the memory card process data.
    // If both memory cards have a process assigned, memory card 2 is prioritized.
    if (g_MemCardSaveWork.saveWork[0].processId != MemCardGameProcessId_None)
    {
        if (g_MemCardSaveWork.saveWork[1].processId == MemCardGameProcessId_None)
        {
            statusPtr = &g_MemCardSaveWork.saveWork[0];
        }
        else
        {
            statusPtr = &g_MemCardSaveWork.saveWork[1];
        }
    }
    else
    {
        if (g_MemCardSaveWork.memCardInitalized == true &&
            g_MemCardSaveWork.saveWork[1].processId == MemCardGameProcessId_None)
        {
            MemCard_SaveWork_SetParams(&g_MemCardSaveWork.saveWork[1], g_MemCardSaveWork.memCardInitalized,
            g_MemCardSaveWork.saveWork[1].deviceId, 0, 0, 0, g_MemCardSaveWork.memCardInitalized);
        }

        statusPtr = &g_MemCardSaveWork.saveWork[1];
    }

    switch (statusPtr->processId)
    {
        case MemCardGameProcessId_Init: // Works on updating memory card too.
            MemCard_Process_Init(statusPtr);
            break;

        case MemCardGameProcessId_Load_Game:
        case MemCardGameProcessId_Load_Settings:
            MemCard_Process_Load(statusPtr);
            break;

        case MemCardGameProcessId_Save_Settings:
        case MemCardGameProcessId_Save_Game:
            MemCard_Process_Save(statusPtr);
            break;

        case MemCardGameProcessId_Format:
            MemCard_Process_Format(statusPtr);
            break;

        case MemCardGameProcessId_None:
        default:
            break;
    }

    if (statusPtr->processId != MemCardGameProcessId_None && statusPtr->lastMemCardResult != MemCardWorkResult_Success)
    {
        statusPtr->processId = MemCardGameProcessId_None;
        if (statusPtr == &g_MemCardSaveWork.saveWork[1])
        {
            g_MemCardSaveWork.saveWork[1].deviceId = (g_MemCardSaveWork.saveWork[1].deviceId + 1) & 0x7;
        }
    }
}

/** @brief Process to format memory card.
 *
 * Scratch: https://decomp.me/scratch/hChGg
 *
 * @param statusPtr Memory card process work information.
 */
static void MemCard_Process_Format(s_MemCard_GameProcess* statusPtr)
{
    if (MemCard_DeviceFormat(statusPtr->deviceId) != 0)
    {
        statusPtr->lastMemCardResult = MemCardWorkResult_FileIoComplete;

        g_MemCardSaveWork.devices[statusPtr->deviceId].status = MemCardState_Available;

        MemCard_FileStatusClear(statusPtr->deviceId);

        g_MemCardSaveWork.devices[statusPtr->deviceId].fileLimit = MEMCARD_FILE_COUNT_MAX;
    }
    else
    {
        statusPtr->lastMemCardResult = MemCardWorkResult_FileIoError;
    }
}

/** @brief Process to initalize memory card.
 * Also used to update memory card information.
 *
 * Scratch 1: https://decomp.me/scratch/n0yu4
 * Scratch 2: https://decomp.me/scratch/nUPKd
 *
 * @param statusPtr Memory card process work information.
 */
static void MemCard_Process_Init(s_MemCard_GameProcess* statusPtr)
{
    char                      filePath[24];
    s32                       memCardResult;
    s32                       i;
    s_MemCard_SaveHeader*     saveHeaderPtr;
    s_MemCard_DeviceInfo*     deviceInfoPtr;
    static s32                fileIdx;
    static s32                readDataErrorAttempt;
    static s32                checkSumValidationAttempts;
    static s32                D_800B2624; // Unused.
    static s_MemCardDirectory directoryInfoCpy;

    statusPtr->lastMemCardResult = MemCardWorkResult_Success;

    deviceInfoPtr = &g_MemCardSaveWork.devices[statusPtr->deviceId];

    switch (statusPtr->processState)
    {
        case 0: // Start memcard process initialization.
            readDataErrorAttempt = 0;

            if (MemCard_WorkSet(MemCardWorkIoMode_Init, statusPtr->deviceId, NULL, NULL, 0, 0, NULL, 0))
            {
                statusPtr->processState = 1;
            }
            break;

        case 1: // Checks if previous step was successful
            memCardResult = MemCard_StateResult();
            switch (memCardResult)
            {
                case MemCardWorkResult_NotConnected:
                    MemCard_DeviceInfoClear(statusPtr->deviceId);
                    deviceInfoPtr->status        = MemCardState_Unavailable;
                    statusPtr->lastMemCardResult = memCardResult;
                    break;

                case MemCardWorkResult_InitError:
                    statusPtr->processState = 2;
                    break;

                case MemCardWorkResult_InitComplete:
                    switch(deviceInfoPtr->status)
                    {
                        case MemCardState_Available:
                            statusPtr->lastMemCardResult = MemCardWorkResult_FileIoComplete;
                            break;

                        case MemCardState_Format:
                            statusPtr->lastMemCardResult = MemCardWorkResult_LoadError;
                            break;

                        case MemCardState_Broken:
                            statusPtr->lastMemCardResult = MemCardWorkResult_FileIoError;
                            break;

                        default:
                            statusPtr->processState = 2;
                            break;
                    }
                    break;
            }
            break;

        case 2: // Copies memory card directory information.
            deviceInfoPtr->status = MemCardState_Loading;
            if (MemCard_WorkSet(MemCardWorkIoMode_DirRead, statusPtr->deviceId, &directoryInfoCpy, NULL, 0, 0, NULL, 0))
            {
                statusPtr->processState = 3;
            }
            break;

        case 3: // Checks if previous step was successful
            memCardResult = MemCard_StateResult();
            switch (memCardResult)
            {
                case MemCardWorkResult_NotConnected:
                    MemCard_DeviceInfoClear(statusPtr->deviceId);
                    statusPtr->lastMemCardResult = memCardResult;
                    deviceInfoPtr->status        = MemCardState_Unavailable;
                    break;

                case MemCardWorkResult_LoadError:
                    MemCard_DeviceInfoClear(statusPtr->deviceId);
                    statusPtr->lastMemCardResult = memCardResult;
                    deviceInfoPtr->status        = MemCardState_Format;
                    break;

                case MemCardWorkResult_NewDevice:
                case MemCardWorkResult_NoNewDevice:
                    statusPtr->processState = 4;
                    return;
            }
            break;

        case 4: // Clear memory card files status.
            fileIdx = NO_VALUE;

            MemCard_FileStatusClear(statusPtr->deviceId);
            bzero(g_MemCardSaveWork.devices[statusPtr->deviceId].saveHeader, sizeof(s_MemCard_SaveHeader) * MEMCARD_FILE_COUNT_MAX);

            statusPtr->processState = 5;

        case 5: // Checks if memory card contains game directory.
            fileIdx++;
            checkSumValidationAttempts = 0;

            for (fileIdx; fileIdx < MEMCARD_FILE_COUNT_MAX; fileIdx++)
            {
                MemCard_FilenameGenerate(filePath, fileIdx);

                for (i = 0; i < MEMCARD_FILE_COUNT_MAX; i++)
                {
                    if (strcmp(directoryInfoCpy.filenames[i], filePath) == 0)
                    {
                        statusPtr->processState = 6;
                        return;
                    }
                }
            }

            if (fileIdx == MEMCARD_FILE_COUNT_MAX)
            {
                statusPtr->processState = 9;
            }
            break;

        case 6: // Copies memory card header data and ties game directory to file.
            MemCard_FilenameGenerate(filePath, fileIdx);

            if (MemCard_WorkSet(MemCardWorkIoMode_Read, statusPtr->deviceId, NULL, filePath, 0, sizeof(s_MemCard_SaveHeader) * 2, &g_MemCardSaveWork.devices[statusPtr->deviceId].saveHeader[fileIdx], sizeof(s_MemCard_SaveHeader)))
            {
                statusPtr->processState = 7;
            }
            break;

        case 7: // Checks if previous step was successful
            memCardResult = MemCard_StateResult();
            switch (memCardResult)
            {
                case MemCardWorkResult_NotConnected:
                    MemCard_DeviceInfoClear(statusPtr->deviceId);

                    statusPtr->lastMemCardResult = memCardResult;
                    deviceInfoPtr->status        = MemCardState_Unavailable;
                    break;

                case MemCardWorkResult_FileOpenError:
                case MemCardWorkResult_FileSeekError:
                case MemCardWorkResult_FileIoError:
                    statusPtr->processState = 0;

                    // If read data check fails start a process where the memory card will be read three times,
                    // if the file fails to read after three attempts the file will be qualified to be damage.
                    if (readDataErrorAttempt >= 3)
                    {
                        MemCard_DeviceInfoClear(statusPtr->deviceId);

                        statusPtr->lastMemCardResult = MemCardWorkResult_FileIoError;
                        deviceInfoPtr->status        = MemCardState_Broken;
                        break;
                    }

                    readDataErrorAttempt++;
                    statusPtr->processState = 2;
                    break;

                case MemCardWorkResult_FileIoComplete:
                    statusPtr->processState = 8;
                    break;
            }
            break;

        case 8: // Checks if save header checksum matches with current save header data.
            saveHeaderPtr = &g_MemCardSaveWork.devices[statusPtr->deviceId].saveHeader[fileIdx];

            // Checksum check.
            if (MemCard_ChecksumValidate(&saveHeaderPtr->footer, (s8*)saveHeaderPtr, sizeof(s_MemCard_SaveHeader)))
            {
                deviceInfoPtr->fileState[fileIdx] = FileState_Used;
                statusPtr->processState           = 5;
                return;
            }

            // If checksum check fails start a process where the memory card will be read three times,
            // if the file fails to load after three attempts the file will be qualified to be damage.
            checkSumValidationAttempts++;

            if (checkSumValidationAttempts >= 3)
            {
                statusPtr->processState           = 5;
                deviceInfoPtr->fileState[fileIdx] = FileState_Damaged;
                return;
            }

            statusPtr->processState = 6;
            break;

        case 9: // Finalize and marks as succesful memory card initalization process.
            deviceInfoPtr->fileLimit     = MemCard_FileLimitUpdate(statusPtr->deviceId, &directoryInfoCpy);
            statusPtr->lastMemCardResult = MemCardWorkResult_FileIoComplete;
            deviceInfoPtr->status        = MemCardState_Available;
            break;
    }
}

/** @brief Gets the available file count. Used to update thhe file count limit in `MemCard_Process_Init`.
 *
 * Scratch: https://decomp.me/scratch/VUvee
 *
 * @param deviceId Memory card index.
 * @param dir Memory card directory.
 * @return Total number of files on the memory card.
 */
static s32 MemCard_FileLimitUpdate(s32 deviceId, s_MemCardDirectory* dir)
{
    s32 ret;
    s32 i;

    ret = MEMCARD_FILE_COUNT_MAX;

    for (i = 0; i < MEMCARD_FILE_COUNT_MAX; i++)
    {
        ret -= dir->blockCounts[i];
    }

    return ret + MemCard_UsedFileCount(deviceId);
}

/** @brief Process to load memory card information.
 *
 * Scratch: https://decomp.me/scratch/FFVlw
 *
 * @param statusPtr Memory card process work information.
 */
static void MemCard_Process_Load(s_MemCard_GameProcess* statusPtr)
{
    char                  filePath[24];
    s32                   memCardResult;
    s32                   saveData0Offset;
    s8*                   saveData0Buf;
    s32                   saveData0Size;
    s_MemCard_DeviceInfo* deviceInfoPtr;
    s8*                   saveData1Buf;
    s32                   saveData1Size;
    s_Savegame_Footer*    saveData1Footer;
    static s32            fileIdx;

    deviceInfoPtr = &g_MemCardSaveWork.devices[statusPtr->deviceId];

    statusPtr->lastMemCardResult = MemCardWorkResult_Success;

    switch (statusPtr->processState)
    {
        case 0: // Checks if any file from the memory card is used.
            if (statusPtr->processId == MemCardGameProcessId_Load_Game)
            {
                if (MemCard_AreAllFilesUsed(statusPtr->deviceId) != true)
                {
                    fileIdx = MemCard_FileWithBiggestTotalSavegameCountGet(statusPtr->deviceId);
                    if (fileIdx == NO_VALUE)
                    {
                        statusPtr->lastMemCardResult = MemCardWorkResult_FileIoError;
                    }
                    else
                    {
                        statusPtr->processState = 1;
                    }
                }
                else
                {
                    statusPtr->lastMemCardResult = MemCardWorkResult_Full;
                }
            }
            else
            {
                fileIdx = statusPtr->fileIdx;
                switch (deviceInfoPtr->fileState[fileIdx])
                {
                    case FileState_Used:
                        if (MemCard_SaveMetadataGet(statusPtr->deviceId, fileIdx, statusPtr->saveIdx)->totalSavegameCount != 0)
                        {
                            statusPtr->processState = 1;
                            break;
                        }

                    case FileState_Unused:
                        statusPtr->lastMemCardResult = MemCardWorkResult_Full;
                        break;

                    case FileState_Damaged:
                        statusPtr->lastMemCardResult = MemCardWorkResult_DamagedData;
                        break;
                }
            }
            break;

        case 1: // Reads savegame data or reads game configurations.
            if (statusPtr->processId == MemCardGameProcessId_Load_Game) // Load only game configurations.
            {
                saveData0Offset = sizeof(s_PsxSaveBlock) + sizeof(s_MemCard_SaveHeader);
                saveData0Buf    = (s8*)&g_MemCardSaveWork.optionsConfig;
                saveData0Size   = sizeof(s_Savegame_OptionsConfig);
            }
            else
            {
                saveData0Offset = sizeof(s_PsxSaveBlock) + sizeof(s_MemCard_SaveHeader) + sizeof(s_Savegame_OptionsConfig) + (statusPtr->saveIdx * sizeof(s_Savegame_Container));
                saveData0Buf    = (s8*)&g_MemCardSaveWork.savegame;
                saveData0Size   = sizeof(s_Savegame_Container);
            }

            MemCard_FilenameGenerate(filePath, fileIdx);

            if (MemCard_WorkSet(MemCardWorkIoMode_Read, statusPtr->deviceId, NULL, filePath, 0, saveData0Offset, saveData0Buf, saveData0Size) == true)
            {
                statusPtr->processState = 2;
            }
            break;

        case 2: // Checks if previous step was successful
            memCardResult = MemCard_StateResult();
            switch (memCardResult)
            {
                case MemCardWorkResult_NotConnected:
                    MemCard_DeviceInfoClear(statusPtr->deviceId);
                    statusPtr->lastMemCardResult = memCardResult;
                    deviceInfoPtr->status        = MemCardState_Unavailable;
                    break;

                case MemCardWorkResult_FileOpenError:
                case MemCardWorkResult_FileSeekError:
                case MemCardWorkResult_FileIoError:
                    MemCard_DeviceInfoClear(statusPtr->deviceId);
                    statusPtr->lastMemCardResult = MemCardWorkResult_FileIoError;
                    deviceInfoPtr->status        = MemCardState_Null;
                    break;

                case MemCardWorkResult_FileIoComplete:
                    statusPtr->processState = 3;
                    break;
            }
            break;

        case 3: // Checks if data checksum matches and moves data to game's global variables.
            if (statusPtr->processId == MemCardGameProcessId_Load_Game)
            {
                saveData1Size   = sizeof(s_Savegame_OptionsConfig);
                saveData1Buf    = (s8*)&g_MemCardSaveWork.optionsConfig;
                saveData1Footer = &g_MemCardSaveWork.optionsConfig.footer;
            }
            else
            {
                saveData1Buf    = (s8*)&g_MemCardSaveWork.savegame;
                saveData1Size   = sizeof(s_Savegame_Container);
                saveData1Footer = &g_MemCardSaveWork.savegame.footer;
            }

            if (MemCard_ChecksumValidate(saveData1Footer, saveData1Buf, saveData1Size) == false)
            {
                statusPtr->lastMemCardResult = MemCardWorkResult_DamagedData;
                return;
            }

            statusPtr->lastMemCardResult = MemCardWorkResult_FileIoComplete;

            if (statusPtr->processId == MemCardGameProcessId_Load_Game)
            {
                memcpy(&g_GameWorkConst->config, &g_MemCardSaveWork.optionsConfig.config, sizeof(s_OptionsConfig));
            }
            else
            {
                memcpy(g_SavegamePtr, &g_MemCardSaveWork.savegame.savegame, sizeof(s_Savegame));
            }
            break;
    }
}

/** @brief Process to save game in the memory card.
 *
 * Scratch: https://decomp.me/scratch/uAT7K
 *
 * @param statusPtr Memory card process work information.
 */
static void MemCard_Process_Save(s_MemCard_GameProcess* statusPtr)
{
    char                  filePath[24];
    s32                   fileIdx;
    s32                   memCardResult;
    s_MemCard_DeviceInfo* deviceInfoPtr;
    static s32            fileIdxCpy;
    static s32            D_800B277C;

    statusPtr->lastMemCardResult = MemCardWorkResult_Success;

    deviceInfoPtr = &g_MemCardSaveWork.devices[statusPtr->deviceId];

    switch (statusPtr->processState)
    {
        case 0: // Checks currently saving file status.
            if (statusPtr->processId == MemCardGameProcessId_Save_Settings)
            {
                fileIdx = statusPtr->fileIdx;
                if (fileIdx != NO_VALUE)
                {
                    switch (deviceInfoPtr->fileState[fileIdx])
                    {
                        case FileState_Unused:
                            fileIdxCpy              = fileIdx;
                            statusPtr->processState = 1;
                            break;

                        case FileState_Used:
                            fileIdxCpy              = fileIdx;
                            statusPtr->processState = 3;
                            break;

                        case FileState_Damaged:
                            statusPtr->lastMemCardResult = MemCardWorkResult_FileIoError;
                            break;

                        default:
                            break;
                    }
                }
                else
                {
                    if (MemCard_AreAllFilesUsed(statusPtr->deviceId) == true)
                    {
                        fileIdxCpy              = 0;
                        statusPtr->processState = 1;
                    }
                    else
                    {
                        fileIdxCpy = MemCard_FileWithBiggestTotalSavegameCountGet(statusPtr->deviceId);
                        if (fileIdxCpy != fileIdx)
                        {
                            statusPtr->processState = 3;
                        }
                        else
                        {
                            statusPtr->lastMemCardResult = MemCardWorkResult_FileIoError;
                        }
                    }
                }
            }
            else
            {
                fileIdxCpy = statusPtr->fileIdx;
                switch (deviceInfoPtr->fileState[fileIdxCpy])
                {
                    case FileState_Unused:
                        statusPtr->processState = 1;
                        return;

                    case FileState_Used:
                        statusPtr->processState = 5;
                        return;

                    case FileState_Damaged:
                        statusPtr->lastMemCardResult = MemCardWorkResult_FileIoError;
                        break;

                    default:
                        break;
                }
            }
            break;

        case 1: // Creates a new file in the memory card.
            MemCard_SaveBlockGenerate(&g_MemCardSaveWork.saveBlock, 1, fileIdxCpy, 0, 0, 0x70, 0x60, 0, 0);
            MemCard_SaveInfoClear(&g_MemCardSaveWork.saveInfo);
            MemCard_FilenameGenerate(filePath, fileIdxCpy);

            if (MemCard_WorkSet(MemCardWorkIoMode_Create, statusPtr->deviceId, NULL, filePath, 1, 0, &g_MemCardSaveWork.saveBlock, 0x300))
            {
                statusPtr->processState = 2;
            }
            break;

        case 2: // Checks if previous step was successful.
            memCardResult = MemCard_StateResult();
            switch (memCardResult)
            {
                case MemCardWorkResult_NotConnected:
                    MemCard_DeviceInfoClear(statusPtr->deviceId);
                    statusPtr->lastMemCardResult = memCardResult;
                    deviceInfoPtr->status        = MemCardState_Unavailable;
                    break;

                case MemCardWorkResult_FileCreateError:
                    MemCard_DeviceInfoClear(statusPtr->deviceId);
                    statusPtr->lastMemCardResult = memCardResult;
                    deviceInfoPtr->status        = MemCardState_Null;
                    break;

                case MemCardWorkResult_FileOpenError:
                case MemCardWorkResult_FileSeekError:
                case MemCardWorkResult_FileIoError:
                    MemCard_DeviceInfoClear(statusPtr->deviceId);

                    statusPtr->lastMemCardResult = MemCardWorkResult_FileIoError;
                    deviceInfoPtr->status        = MemCardState_Null;

                    MemCard_FilenameGenerate(filePath, fileIdxCpy);
                    MemCard_FileClear(statusPtr->deviceId, filePath);
                    break;

                case MemCardWorkResult_FileIoComplete:
                    deviceInfoPtr->fileState[fileIdxCpy] = FileState_Used;

                    if (statusPtr->processId == MemCardGameProcessId_Save_Settings)
                    {
                        statusPtr->processState = 3;
                    }
                    else
                    {
                        statusPtr->processState = 5;
                    }
                    break;

                default:
                    break;
            }
            break;

        case 3: // Copies and saves user configs.
            MemCard_UserConfigCopy(&g_MemCardSaveWork.optionsConfig, &g_GameWorkConst->config);
            MemCard_FilenameGenerate(filePath, fileIdxCpy);

            if (MemCard_WorkSet(MemCardWorkIoMode_Write, statusPtr->deviceId, NULL, filePath, 0, 0x300, &g_MemCardSaveWork.optionsConfig, 0x80))
            {
                statusPtr->processState = 4;
            }
            break;

        case 4: // Checks if previous step was successful.
            memCardResult = MemCard_StateResult();
            switch (memCardResult)
            {
                case MemCardWorkResult_NotConnected:
                    MemCard_DeviceInfoClear(statusPtr->deviceId);
                    statusPtr->lastMemCardResult = memCardResult;
                    deviceInfoPtr->status        = MemCardState_Unavailable;
                    break;

                case MemCardWorkResult_FileOpenError:
                case MemCardWorkResult_FileSeekError:
                case MemCardWorkResult_FileIoError:
                    MemCard_DeviceInfoClear(statusPtr->deviceId);
                    statusPtr->lastMemCardResult = MemCardWorkResult_FileIoError;
                    deviceInfoPtr->status        = MemCardState_Null;
                    break;

                case MemCardWorkResult_FileIoComplete:
                    statusPtr->lastMemCardResult = memCardResult;
                    break;

                default:
                    break;
            }
            break;

        case 5: // Copies and saves user progress.
            MemCard_FilenameGenerate(filePath, fileIdxCpy);
            MemCard_GameDataCopy(&g_MemCardSaveWork.savegame, g_SavegamePtr);

            if (MemCard_WorkSet(MemCardWorkIoMode_Write, statusPtr->deviceId, NULL, filePath, 0, (statusPtr->saveIdx * 0x280) + 0x380, &g_MemCardSaveWork.savegame, 0x280))
            {
                statusPtr->processState = 6;
            }
            break;

        case 6: // Checks if previous step was successful
            memCardResult = MemCard_StateResult();
            switch (memCardResult)
            {
                case MemCardWorkResult_NotConnected:
                    MemCard_DeviceInfoClear(statusPtr->deviceId);
                    statusPtr->lastMemCardResult = memCardResult;
                    deviceInfoPtr->status        = MemCardState_Unavailable;
                    break;

                case MemCardWorkResult_FileOpenError:
                case MemCardWorkResult_FileSeekError:
                case MemCardWorkResult_FileIoError:
                    MemCard_DeviceInfoClear(statusPtr->deviceId);
                    statusPtr->lastMemCardResult = MemCardWorkResult_FileIoError;
                    deviceInfoPtr->status        = MemCardState_Null;
                    break;

                case MemCardWorkResult_FileIoComplete:
                    statusPtr->processState = 7;
                    break;
            }
            break;

        case 7: // Updates total save games count.
            MemCard_TotalSavegameCountUpdate(statusPtr->deviceId, fileIdxCpy, statusPtr->saveIdx, g_SavegamePtr);
            statusPtr->processState = 8;

        case 8: // Saves header information progress.
            MemCard_FilenameGenerate(filePath, fileIdxCpy);

            if (MemCard_WorkSet(MemCardWorkIoMode_Write, statusPtr->deviceId, NULL, filePath, 0, 512, (u8*)g_MemCardSaveWork.devices[statusPtr->deviceId].saveHeader + (fileIdxCpy * sizeof(s_MemCard_SaveHeader)), sizeof(s_MemCard_SaveHeader)))
            {
                statusPtr->processState = 9;
            }
            break;

        case 9: // Checks if previous step was successful
            memCardResult = MemCard_StateResult();
            switch (memCardResult)
            {
                case MemCardWorkResult_NotConnected:
                    MemCard_DeviceInfoClear(statusPtr->deviceId);
                    statusPtr->lastMemCardResult = memCardResult;
                    deviceInfoPtr->status        = MemCardState_Unavailable;
                    break;

                case MemCardWorkResult_FileOpenError:
                case MemCardWorkResult_FileSeekError:
                case MemCardWorkResult_FileIoError:
                    MemCard_DeviceInfoClear(statusPtr->deviceId);
                    statusPtr->lastMemCardResult = MemCardWorkResult_FileIoError;
                    deviceInfoPtr->status        = MemCardState_Null;
                    break;

                case MemCardWorkResult_FileIoComplete:
                    statusPtr->lastMemCardResult = memCardResult;
                    break;
            }
            break;
    }
}

/** @brief Clears all save information from a file.
 *
 * Scratch 1: https://decomp.me/scratch/5bnmv
 * Scratch 2: https://decomp.me/scratch/C7gtF
 * Scratch 3: https://decomp.me/scratch/8lE86
 *
 * @param saveInfo Game save information.
 */
static void MemCard_SaveInfoClear(s_MemCard_SaveHeader* saveInfo)
{
    s32 i;

    bzero(saveInfo, sizeof(s_MemCard_SaveHeader));

    for (i = 0; i < MEMCARD_SAVES_COUNT_MAX; i++)
    {
        saveInfo->saveMetadata[i].totalSavegameCount = 0;
    }

    MemCard_ChecksumUpdate(&saveInfo->footer, (s8*)saveInfo, sizeof(s_MemCard_SaveHeader));
}

/** @brief Copies user config into an `s_Savegame_OptionsConfig` and calculates footer checksum.
 *
 * Scratch 1: https://decomp.me/scratch/2Pkiv
 * Scratch 2: https://decomp.me/scratch/Zx6xm
 * Scratch 3: https://decomp.me/scratch/zHEAg
 */
static void MemCard_UserConfigCopy(s_Savegame_OptionsConfig* dest, s_OptionsConfig* src)
{
    bzero(dest, sizeof(s_Savegame_OptionsConfig));
    dest->config = *src;
    MemCard_ChecksumUpdate(&dest->footer, (s8*)&dest->config, sizeof(s_Savegame_OptionsConfig));
}

/** @brief Retrieves the index of the file with the save with the
 * biggest total savegame count within the specified memory card.
 *
 * Scratch: https://decomp.me/scratch/v5gVl
 *
 * @param deviceId Memory card index.
 * @return Biggest total savegame count within the specified memory card.
 */
static s32 MemCard_FileWithBiggestTotalSavegameCountGet(s32 deviceId)
{
    s32 totalSavegameCount;
    s32 saveIdx;
    s32 fileIdx;
    s32 biggesttotalSavegameCount;
    s32 fileIdxWithBiggestTotalSavegameCount;

    fileIdxWithBiggestTotalSavegameCount = NO_VALUE;
    biggesttotalSavegameCount            = NO_VALUE;

    for (fileIdx = 0; fileIdx < MEMCARD_FILE_COUNT_MAX; fileIdx++)
    {
        if (g_MemCardSaveWork.devices[deviceId].fileState[fileIdx] != FileState_Used)
        {
            continue;
        }

        for (saveIdx = 0; saveIdx < MEMCARD_SAVES_COUNT_MAX; saveIdx++)
        {
            totalSavegameCount = g_MemCardSaveWork.devices[deviceId].saveHeader[fileIdx].saveMetadata[saveIdx].totalSavegameCount;
            if (biggesttotalSavegameCount < totalSavegameCount)
            {
                fileIdxWithBiggestTotalSavegameCount = fileIdx;
                biggesttotalSavegameCount            = totalSavegameCount;
            }
        }
    }

    return fileIdxWithBiggestTotalSavegameCount;
}

/** @brief Copies asavegame into a `s_Savegame_Container` and computes the footer checksum.
 *
 * Scratch: No scratch.
 *
 * @param dest Destination where data will be moved.
 * @param src Source savegame data.
 */
static void MemCard_GameDataCopy(s_Savegame_Container* dest, s_Savegame* src)
{
    bzero(dest, sizeof(s_Savegame_Container));
    memcpy(&dest->savegame, src, sizeof(s_Savegame));
    MemCard_ChecksumUpdate(&dest->footer, (s8*)&dest->savegame, sizeof(s_Savegame_Container));
}

/** @brief Updates game save's total save count.
 *
 * Scratch 1: https://decomp.me/scratch/CXEis
 * Scratch 2: https://decomp.me/scratch/3M4wO
 *
 * @param deviceId Memory card index.
 * @param fileIdx File index on the memory card.
 * @param saveIdx Index of the selected save from the selected file on the memory card.
 * @param unused Unknown `s_Savegame` pointer. Possibly this function was originally intended to update `s_Savegame::savegameCount`,
 */
static void MemCard_TotalSavegameCountUpdate(s32 deviceId, s32 fileIdx, s32 saveIdx, s_Savegame* unused)
{
    s_MemCard_SaveHeader* saveHdrPtr;

    saveHdrPtr = &g_MemCardSaveWork.devices[deviceId].saveHeader[fileIdx];

    MemCard_TotalSavegameCountStepUpdate(deviceId, fileIdx, saveIdx);
    MemCard_ChecksumUpdate(&saveHdrPtr->footer, (s8*)saveHdrPtr, sizeof(s_MemCard_SaveHeader));
}

/** @brief Finds the biggest total save game count in the memory card and
 * updates the target save total game save count.
 *
 * Scratch: https://decomp.me/scratch/TaVpJ
 *
 * @param deviceId Memory card index.
 * @param fileIdx File index on the memory card.
 * @param saveIdx Index of the selected save from the selected file on the memory card.
 */
void MemCard_TotalSavegameCountStepUpdate(s32 deviceId, s32 fileIdx, s32 saveIdx)
{
    s32                      i;
    s32                      totalSavegameCount;
    s_MemCard_TotalSavesInfo saveInfo;

    totalSavegameCount = 0;
    for (i = 0; i < MEMCARD_DEVICE_COUNT_MAX; i++)
    {
        MemCard_SaveWithBiggestTotalSavegameCountGet(i, &saveInfo);

        if (totalSavegameCount < saveInfo.totalSavegameCount)
        {
            totalSavegameCount = saveInfo.totalSavegameCount;
        }
    }

    g_MemCardSaveWork.devices[deviceId].saveHeader[fileIdx].saveMetadata[saveIdx].totalSavegameCount = totalSavegameCount + 1;
}

/** @brief Retrieves the savegames with the highest total save count on the indicated memory card.
 *
 * Scratch: https://decomp.me/scratch/cam86
 *
 * @param deviceId Memory card index.
 * @param result Info containing the savegame index with the highest total save count on the memory card.
 */
static void MemCard_SaveWithBiggestTotalSavegameCountGet(s32 deviceId, s_MemCard_TotalSavesInfo* result)
{
    s32 totalSavegameCount;
    s32 saveIdx;
    s32 fileIdx;

    result->fileIdx            = 0;
    result->saveIdx            = 0;
    result->totalSavegameCount = 0;

    if (g_MemCardSaveWork.devices[deviceId].status != MemCardState_Available)
    {
        return;
    }

    for (fileIdx = 0; fileIdx < MEMCARD_FILE_COUNT_MAX; fileIdx++)
    {
        if (g_MemCardSaveWork.devices[deviceId].fileState[fileIdx] != FileState_Used)
        {
            continue;
        }

        for (saveIdx = 0; saveIdx < MEMCARD_SAVES_COUNT_MAX; saveIdx++)
        {
            totalSavegameCount = g_MemCardSaveWork.devices[deviceId].saveHeader[fileIdx].saveMetadata[saveIdx].totalSavegameCount;

            if (result->totalSavegameCount < totalSavegameCount)
            {
                result->fileIdx            = fileIdx;
                result->saveIdx            = saveIdx;
                result->totalSavegameCount = totalSavegameCount;
            }
        }
    }
}

// ========================================
// MEMORY CARD - CHECKSUM
// ========================================

void MemCard_ChecksumUpdate(s_Savegame_Footer* saveFooter, s8* saveData, s32 saveDataLength)
{
    u8 checksum;

    saveFooter->checksum[0] = saveFooter->checksum[1] = 0;
    saveFooter->magic                                 = SAVEGAME_FOOTER_MAGIC;
    checksum                                          = MemCard_ChecksumGenerate(saveData, saveDataLength);
    saveFooter->checksum[0] = saveFooter->checksum[1] = checksum;
}

bool MemCard_ChecksumValidate(s_Savegame_Footer* saveFooter, s8* saveData, s32 saveDataLength)
{
    bool isValid = false;

    if (saveFooter->checksum[0] == MemCard_ChecksumGenerate(saveData, saveDataLength))
    {
        isValid = saveFooter->magic == SAVEGAME_FOOTER_MAGIC;
    }

    return isValid;
}

u8 MemCard_ChecksumGenerate(s8* saveData, s32 saveDataLength)
{
    u8  checksum = 0;
    s32 i        = 0;

    for (i = 0; i < saveDataLength;)
    {
        i++;
        checksum ^= *saveData++;
    }

    return checksum;
}

// ========================================
// MEMORY CARD - BLOCK
// ========================================

/** @brief Generates a filename for a given savegame index.
 *
 * Scratch 1: https://decomp.me/scratch/y2zUU
 * Scratch 2: https://decomp.me/scratch/Mt2pD
 */
static void MemCard_FilenameGenerate(char* dest, s32 fileIdx)
{
    char buf[3];

#if VERSION_REGION_IS(NTSCJ)
    strcpy(dest, "BI");
#elif VERSION_REGION_IS(PAL)
    strcpy(dest, "BE");
#else
    strcpy(dest, "BA");
#endif
    strcat(dest, VERSION_SERIAL);
    strcat(dest, "SILENT");

    buf[0] = '0' + (fileIdx / 10);
    buf[1] = '0' + (fileIdx % 10);
    buf[2] = 0;

    strcat(dest, buf);
}

/** @brief Generates a PS1 save block.
 *
 * Scratch: https://decomp.me/scratch/62slY
 *
 * @param saveBlock Savegame block information.
 * @param blockCount Block count.
 * @param fileIdx File index on the memory card where the block is being generated.
 * @param arg3 Unknown, dead code.
 * @param arg5 Unknown, dead code.
 * @param arg6 Unknown, dead code.
 * @param arg7 Unknown, dead code.
 * @param arg8 Unknown, dead code. 
 */
static void MemCard_SaveBlockGenerate(s_PsxSaveBlock* saveBlock, s8 blockCount, s32 fileIdx, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8)
{
    char      fileIdxStr[8];
    TIM_IMAGE iconTexture;

#if VERSION_EQUAL_OR_NEWER(USA) // `bzero` call missing in JPN0
    bzero(saveBlock, sizeof(s_PsxSaveBlock));
#endif

    saveBlock->magic[0]        = 'S';
    saveBlock->magic[1]        = 'C';
    saveBlock->iconDisplayFlag = 0x11; // ICON_HAS_1_STATIC_FRAME
    saveBlock->blockCount      = blockCount;
    bzero(saveBlock->titleNameShiftJis, 0x40);

    strcpy(fileIdxStr, "００");
    fileIdxStr[1] += (fileIdx + 1) / 10;
    fileIdxStr[3] += (fileIdx + 1) % 10;

#if VERSION_REGION_IS(NTSC) || VERSION_REGION_IS(PAL)
    strcpy(saveBlock->titleNameShiftJis, "ＳＩＬＥＮＴ　ＨＩＬＬ");
    strcat(saveBlock->titleNameShiftJis, "　　ＦＩＬＥ");
#elif VERSION_REGION_IS(NTSCJ)
    strcpy(saveBlock->titleNameShiftJis, "サイレントヒル");
    strcat(saveBlock->titleNameShiftJis, "　ファイル");
#endif

    strcat(saveBlock->titleNameShiftJis, fileIdxStr);

    bzero(&saveBlock->pocketStationHeader, sizeof(s_PocketStationHeader));

    OpenTIM(g_MemCard_SaveIconTim);
    ReadTIM(&iconTexture);

    memcpy(saveBlock->iconPalette, iconTexture.caddr, iconTexture.crect->w * iconTexture.crect->h * 2);
    memcpy(saveBlock->frameIconData, iconTexture.paddr, iconTexture.prect->w * iconTexture.prect->h * 2);
}

/** @brief @unused Writes `0xFF` to the first 128 bytes of a memory card and checks if an event is triggered.
 *
 * Scratch 1: https://decomp.me/scratch/fi1GL
 * Scratch 2: https://decomp.me/scratch/fKUIm
 *
 * @param deviceId Memory card index.
 */
static bool MemCard_DeviceTest(s32 deviceId)
{
    u8 cardBuf[128];

    memset(cardBuf, 0xFF, 128);

    MemCard_HwEventsReset();
    _new_card();
    _card_write(MemCard_Chan(deviceId), 0, cardBuf);

    g_MemCardWork.devicesPending |= 1 << g_MemCardWork.deviceId;

    return MemCard_HwLastEvent() != 0;
}

/** @brief Format game filename in memory card.
 *
 * Scratch 1: https://decomp.me/scratch/Ir3Z4
 * Scratch 2: https://decomp.me/scratch/RuxhB
 * Scratch 3: https://decomp.me/scratch/VSZEp
 *
 * @param deviceId Memory card index.
 */
static s32 MemCard_DeviceFormat(s32 deviceId)
{
    #define BUF_SIZE 16

    char buf[BUF_SIZE];

    MemCard_DevicePathGenerate(deviceId, buf);

    return format(buf);

    #undef BUF_SIZE
}

/** @brief Clear game filename in memory card.
 *
 * Scratch 1: https://decomp.me/scratch/BL03h
 * Scratch 2: https://decomp.me/scratch/pkwrp
 *
 * @param deviceId Memory card index.
 * @param fileName Save data filename.
 */
static s32 MemCard_FileClear(s32 deviceId, char* fileName)
{
    #define BUF_SIZE 32

    char buf[BUF_SIZE];

    MemCard_DevicePathGenerate(deviceId, buf);

    strcat(buf, fileName);

    return erase(buf);

    #undef BUF_SIZE
}

/** @unused @brief Rename game filename.
 *
 * Scratch: https://decomp.me/scratch/HEQn8
 *
 * @param deviceId Memory card index.
 * @param prevName Previous save data filename.
 * @param newName New save data filename.
 */
static s32 MemCard_FileRename(s32 deviceId, char* prevName, char* newName)
{
    #define BUF_SIZE 32

    char prevBuf[BUF_SIZE];
    char newBuf[BUF_SIZE];

    MemCard_DevicePathGenerate(deviceId, prevBuf);
    MemCard_DevicePathGenerate(deviceId, newBuf);

    strcat(prevBuf, prevName);
    strcat(newBuf, newName);

    return rename(prevBuf, newBuf);

    #undef BUF_SIZE
}

// ========================================
// MEMORY CARD - EVENTS
// ========================================

void MemCard_WorkInit(void)
{
    InitCARD(0);
    StartCARD();
    g_MemCardWork.devicesPending = UINT_MAX; // All bits set.
}

void MemCard_EventsInit(void)
{
    MemCard_StateInit();
    MemCard_SwEventsInit();
    MemCard_HwEventsInit();
}

/** @brief Start memory card work processes states.
 *
 * Scratch: https://decomp.me/scratch/GfCpR
 */
static void MemCard_StateInit(void)
{
    g_MemCardWork.state       = MemCardWorkState_Idle;
    g_MemCardWork.stateStep   = 0;
    g_MemCardWork.stateResult = 0;
}

/** @brief Enables tracking memory card events (Software level).
 *
 * Scratch 1: https://decomp.me/scratch/yKRru
 * Scratch 2: https://decomp.me/scratch/lxBDY
 */
static void MemCard_SwEventsInit(void)
{
    EnterCriticalSection();
    g_MemCardWork.eventSwSpIOE    = OpenEvent(SwCARD, EvSpIOE,    EvMdNOINTR, NULL);
    g_MemCardWork.eventSwSpERROR  = OpenEvent(SwCARD, EvSpERROR,  EvMdNOINTR, NULL);
    g_MemCardWork.eventSwSpTIMOUT = OpenEvent(SwCARD, EvSpTIMOUT, EvMdNOINTR, NULL);
    g_MemCardWork.eventSwSpNEW    = OpenEvent(SwCARD, EvSpNEW,    EvMdNOINTR, NULL);
    ExitCriticalSection();

    EnableEvent(g_MemCardWork.eventSwSpIOE);
    EnableEvent(g_MemCardWork.eventSwSpERROR);
    EnableEvent(g_MemCardWork.eventSwSpTIMOUT);
    EnableEvent(g_MemCardWork.eventSwSpNEW);

    MemCard_SwEventsReset();
}

/** @brief Enables tracking memory card events (Hardware level).
 *
 * Scratch 1: https://decomp.me/scratch/hCYR6
 * Scratch 2: https://decomp.me/scratch/2epbu
 */
static void MemCard_HwEventsInit(void)
{
    EnterCriticalSection();
    g_MemCardWork.eventHwSpIOE     = OpenEvent(HwCARD, EvSpIOE,     EvMdINTR, MemCard_HwEventSpIOE);
    g_MemCardWork.eventHwSpERROR   = OpenEvent(HwCARD, EvSpERROR,   EvMdINTR, MemCard_HwEventSpERROR);
    g_MemCardWork.eventHwSpTIMOUT  = OpenEvent(HwCARD, EvSpTIMOUT,  EvMdINTR, MemCard_HwEventSpTIMOUT);
    g_MemCardWork.eventHwSpNEW     = OpenEvent(HwCARD, EvSpNEW,     EvMdINTR, MemCard_HwEventSpNEW);
    g_MemCardWork.eventHwSpUNKNOWN = OpenEvent(HwCARD, EvSpUNKNOWN, EvMdINTR, MemCard_HwEventSpUNKNOWN);
    ExitCriticalSection();

    EnableEvent(g_MemCardWork.eventHwSpIOE);
    EnableEvent(g_MemCardWork.eventHwSpERROR);
    EnableEvent(g_MemCardWork.eventHwSpTIMOUT);
    EnableEvent(g_MemCardWork.eventHwSpNEW);
    EnableEvent(g_MemCardWork.eventHwSpUNKNOWN);

    MemCard_HwEventsReset();
}

void MemCard_EventsClose(void)
{
    MemCard_SwEventsClose();
    MemCard_HwEventsClose();
}

/** @brief Disable tracking memory card events (Software level).
 *
 * Scratch: https://decomp.me/scratch/IFjjm
 */
static void MemCard_SwEventsClose(void)
{
    EnterCriticalSection();
    CloseEvent(g_MemCardWork.eventSwSpIOE);
    CloseEvent(g_MemCardWork.eventSwSpERROR);
    CloseEvent(g_MemCardWork.eventSwSpTIMOUT);
    CloseEvent(g_MemCardWork.eventSwSpNEW);
    ExitCriticalSection();
}

/** @brief Disable tracking memory card events (Hardware level).
 *
 * Scratch 1: https://decomp.me/scratch/eu76G
 * Scratch 2: https://decomp.me/scratch/GRwPV
 */
static void MemCard_HwEventsClose(void)
{
    EnterCriticalSection();
    CloseEvent(g_MemCardWork.eventHwSpIOE);
    CloseEvent(g_MemCardWork.eventHwSpERROR);
    CloseEvent(g_MemCardWork.eventHwSpTIMOUT);
    CloseEvent(g_MemCardWork.eventHwSpNEW);
    CloseEvent(g_MemCardWork.eventHwSpUNKNOWN);
    ExitCriticalSection();
}

s32 MemCard_SwEventsTest(void)
{
    if (TestEvent(g_MemCardWork.eventSwSpERROR) == true)
    {
        return EvSpERROR;
    }

    if (TestEvent(g_MemCardWork.eventSwSpTIMOUT) == true)
    {
        return EvSpTIMOUT;
    }

    if (TestEvent(g_MemCardWork.eventSwSpNEW) == true)
    {
        return EvSpNEW;
    }

    if (TestEvent(g_MemCardWork.eventSwSpIOE) == true)
    {
        return EvSpIOE;
    }

    return NULL;
}

void MemCard_SwEventsReset(void)
{
    TestEvent(g_MemCardWork.eventSwSpERROR);
    TestEvent(g_MemCardWork.eventSwSpTIMOUT);
    TestEvent(g_MemCardWork.eventSwSpNEW);
    TestEvent(g_MemCardWork.eventSwSpIOE);
}

s32 MemCard_HwLastEvent(void)
{
    return g_MemCardWork.lastEventHw;
}

void MemCard_HwEventsReset(void)
{
    TestEvent(g_MemCardWork.eventHwSpERROR);
    TestEvent(g_MemCardWork.eventHwSpTIMOUT);
    TestEvent(g_MemCardWork.eventHwSpNEW);
    TestEvent(g_MemCardWork.eventHwSpIOE);
    TestEvent(g_MemCardWork.eventHwSpUNKNOWN);

    g_MemCardWork.lastEventHw = 0;
}

void MemCard_HwEventSpIOE(void)
{
    g_MemCardWork.lastEventHw = EvSpIOE;
}

void MemCard_HwEventSpERROR(void)
{
    g_MemCardWork.lastEventHw = EvSpERROR;
}

void MemCard_HwEventSpNEW(void)
{
    g_MemCardWork.lastEventHw = EvSpNEW;
}

void MemCard_HwEventSpTIMOUT(void)
{
    g_MemCardWork.lastEventHw = EvSpTIMOUT;
}

void MemCard_HwEventSpUNKNOWN(void)
{
    g_MemCardWork.lastEventHw = EvSpUNKNOWN;
}

// ========================================
// MEMORY CARD - STATES WORK
// ========================================

s32 MemCard_StateResult(void)
{
    return g_MemCardWork.stateResult;
}

bool MemCard_WorkSet(e_MemCardWorkIoMode mode, s32 deviceId, s_MemCardDirectory* outDir, char* filename, s32 createBlockCount, s32 fileOffset, void* outBuf, s32 bufSize)
{
    if (MemCard_MemCardIsIdle() == false)
    {
        return false;
    }

    g_MemCardWork.MemCardWorkIoMode = mode;

    switch (mode)
    {
        case MemCardWorkIoMode_Init:
        case MemCardWorkIoMode_DirRead:
            g_MemCardWork.state     = MemCardWorkState_Init;
            g_MemCardWork.stateStep = 0;
            break;

        case MemCardWorkIoMode_Read:
        case MemCardWorkIoMode_Write:
            g_MemCardWork.state     = MemCardWorkState_FileOpen;
            g_MemCardWork.stateStep = 0;
            break;

        case MemCardWorkIoMode_Create:
            g_MemCardWork.state     = MemCardWorkState_FileCreate;
            g_MemCardWork.stateStep = 0;
            break;

        default:
            break;
    }

    g_MemCardWork.deviceId    = deviceId;
    g_MemCardWork.directories = outDir;

    MemCard_DevicePathGenerate(deviceId, g_MemCardWork.filePath);
    strcat(g_MemCardWork.filePath, filename);

    g_MemCardWork.createBlockCount = createBlockCount;
    g_MemCardWork.seekOffset       = fileOffset;
    g_MemCardWork.dataBuffer       = outBuf;
    g_MemCardWork.dataSize         = bufSize;
    g_MemCardWork.hasNewDevice     = false;
    return true;
}

/** @brief Checks if the memory card, where a process is being carried, is in idle state or not.
 *
 * Scratch: https://decomp.me/scratch/2ZFUl
 *
 * @return `true` if memory card is in idle state.
 */
static bool MemCard_MemCardIsIdle(void)
{
    return g_MemCardWork.state == MemCardWorkState_Idle;
}

/** @brief Execute memory card process.
 *
 * Scratch: https://decomp.me/scratch/eM9zU
 */
static void MemCard_StateUpdate(void)
{
    switch (g_MemCardWork.state)
    {
        case MemCardWorkState_Idle:
            // @hack Probably some optimized out code here.
            g_MemCardWork.stateResult += 0;
            break;

        case MemCardWorkState_Init:
            g_MemCardWork.stateResult = MemCard_State_Init();
            break;

        case MemCardWorkState_Check:
            g_MemCardWork.stateResult = MemCard_State_Check();
            break;

        case MemCardWorkState_Load:
            g_MemCardWork.stateResult = MemCard_State_Load();
            break;

        case MemCardWorkState_DirRead:
            g_MemCardWork.stateResult = MemCard_State_DirRead();
            break;

        case MemCardWorkState_FileCreate:
            g_MemCardWork.stateResult = MemCard_State_FileCreate();
            break;

        case MemCardWorkState_FileOpen:
            g_MemCardWork.stateResult = MemCard_State_FileOpen();
            break;

        case MemCardWorkState_FileReadWrite:
            g_MemCardWork.stateResult = MemCard_State_FileReadWrite();
            break;

        default:
            break;
    }
}

/** @brief Checks if memory card has been initalized or tries to do it.
 *
 * Scratch: https://decomp.me/scratch/ZqZwd
 *
 * @return `e_MemCardWorkResult`.
 */
static s32 MemCard_State_Init(void)
{
    s32 channel;
    s32 result;

    result  = MemCardWorkResult_Success;
    channel = MemCard_Chan(g_MemCardWork.deviceId);

    switch (g_MemCardWork.stateStep)
    {
        case 0:
            g_MemCardWork.retryCount = 0;
            g_MemCardWork.field_7C   = 0;
            g_MemCardWork.stateStep  = 1;

        case 1:
            MemCard_SwEventsReset();

            if (_card_info(channel) == 1)
            {
                g_MemCardWork.stateStep++;
            }
            else
            {
                g_MemCardWork.retryCount++;
            }
            break;

        case 2:
            switch (MemCard_SwEventsTest())
            {
                case EvSpIOE: // Connected.
                    if (g_MemCardWork.MemCardWorkIoMode == MemCardWorkIoMode_Init)
                    {
                        result                  = MemCardWorkResult_InitComplete;
                        g_MemCardWork.state     = MemCardWorkState_Idle;
                        g_MemCardWork.stateStep = 0;
                    }
                    else if (!((g_MemCardWork.devicesPending >> g_MemCardWork.deviceId) & 1))
                    {
                        g_MemCardWork.state     = MemCardWorkState_DirRead;
                        g_MemCardWork.stateStep = 0;
                    }
                    else
                    {
                        g_MemCardWork.state     = MemCardWorkState_Check;
                        g_MemCardWork.stateStep = 0;
                    }
                    break;

                case EvSpNEW: // "No writing after connection"
                    g_MemCardWork.hasNewDevice = true;

                    if (g_MemCardWork.MemCardWorkIoMode == MemCardWorkIoMode_Init)
                    {
                        result                  = MemCardWorkResult_InitError;
                        g_MemCardWork.state     = MemCardWorkState_Idle;
                        g_MemCardWork.stateStep = 0;
                    }
                    else
                    {
                        g_MemCardWork.state     = MemCardWorkState_Check;
                        g_MemCardWork.stateStep = 0;
                    }
                    break;

                case EvSpTIMOUT: // Not connected.
                    result                  = MemCardWorkResult_NotConnected;
                    g_MemCardWork.state     = MemCardWorkState_Idle;
                    g_MemCardWork.stateStep = 0;
                    break;

                case EvSpERROR: // Error.
                    g_MemCardWork.stateStep = 1;
                    break;
            }
            break;
    }

    return result;
}

/** @brief Checks if data can be written in the memory card.
 *
 * Scratch: https://decomp.me/scratch/6XlCR
 *
 * @return `e_MemCardWorkResult`.
 */
static s32 MemCard_State_Check(void)
{
    s32 channel;
    s32 result;

    result  = MemCardWorkResult_Success;
    channel = MemCard_Chan(g_MemCardWork.deviceId);

    switch (g_MemCardWork.stateStep)
    {
        case 0:
            g_MemCardWork.retryCount = 0;
            g_MemCardWork.field_7C   = 0;
            g_MemCardWork.stateStep  = 1;

        case 1:
            MemCard_HwEventsReset();

            if (_card_clear(channel) == 1)
            {
                g_MemCardWork.stateStep++;
            }
            break;

        case 2:
            switch (MemCard_HwLastEvent())
            {
                case EvSpIOE: // Completed.
                    g_MemCardWork.state     = MemCardWorkState_Load;
                    g_MemCardWork.stateStep = 0;
                    break;

                case EvSpTIMOUT: // Card not connected.
                    result                  = MemCardWorkResult_NotConnected;
                    g_MemCardWork.state     = MemCardWorkState_Idle;
                    g_MemCardWork.stateStep = 0;
                    break;

                case EvSpNEW:   // New card detected.
                case EvSpERROR: // Error.
                    g_MemCardWork.stateStep = 1;
                    break;
            }
            break;
    }

    return result;
}

/** @brief Checks if data can be retrieved from the memory card.
 *
 * Scratch: https://decomp.me/scratch/SnDHT
 *
 * @return `e_MemCardWorkResult`.
 */
static s32 MemCard_State_Load(void)
{
    s32 channel;
    s32 result;

    result  = MemCardWorkResult_Success;
    channel = MemCard_Chan(g_MemCardWork.deviceId);

    switch (g_MemCardWork.stateStep)
    {
        case 0:
            g_MemCardWork.retryCount = 0;
            g_MemCardWork.field_7C   = 0;
            g_MemCardWork.stateStep  = 1;

        case 1:
            MemCard_SwEventsReset();

            if (_card_load(channel) == 1)
            {
                g_MemCardWork.stateStep++;
                if ((g_MemCardWork.deviceId & 4) == 0) // Memory Card 1
                {
                    g_MemCardWork.devicesPending |= 0xF;
                }
                else // Memory Card 2
                {
                    g_MemCardWork.devicesPending |= 0xF0;
                }
            }
            break;

        case 2:
            switch (MemCard_SwEventsTest())
            {
                case EvSpIOE: // Read completed.
                    g_MemCardWork.state           = MemCardWorkState_DirRead;
                    g_MemCardWork.stateStep       = 0;
                    g_MemCardWork.devicesPending &= ~(1 << g_MemCardWork.deviceId);
                    break;

                case EvSpNEW: // Uninitialized card.
                    g_MemCardWork.devicesPending |= 1 << g_MemCardWork.deviceId;
                    if (g_MemCardWork.retryCount < 3)
                    {
                        g_MemCardWork.retryCount++;
                        g_MemCardWork.stateStep = 1;
                    }
                    else
                    {
                        result                  = MemCardWorkResult_LoadError;
                        g_MemCardWork.state     = MemCardWorkState_Idle;
                        g_MemCardWork.stateStep = 0;
                    }
                    break;

                case EvSpTIMOUT: // Not connected.
                    result                  = MemCardWorkResult_NotConnected;
                    g_MemCardWork.state     = MemCardWorkState_Idle;
                    g_MemCardWork.stateStep = 0;
                    break;

                case EvSpERROR: // Error.
                    g_MemCardWork.stateStep = 1;
                    break;
            }
            break;
    }

    return result;
}

/** @brief Checks if memory card is empty or has been previously used to save games from Silent Hill.
 *
 * Scratch: https://decomp.me/scratch/FanVw
 *
 * @return `e_MemCardWorkResult`.
 */
static s32 MemCard_State_DirRead(void)
{
    struct DIRENTRY  fileInfo;
    struct DIRENTRY* curFile;
    char             filePath[16];
    s32              result;
    s32              i;

    for (i = 0; i < MEMCARD_FILE_COUNT_MAX; i++)
    {
        MemCard_DirectoryFileClear(i);
    }

    for (i = 0; i < MEMCARD_FILE_COUNT_MAX; i++)
    {
        if (i == 0)
        {
            MemCard_DevicePathGenerate(g_MemCardWork.deviceId, filePath);
            strcat(filePath, "*");
            curFile = firstfile(filePath, &fileInfo);
        }
        else
        {
            curFile = nextfile(&fileInfo);
        }

        if (curFile == NULL)
        {
            break;
        }

        strcpy(g_MemCardWork.directories->filenames[i], fileInfo.name);
        g_MemCardWork.directories->blockCounts[i] = (fileInfo.size + (8192 - 1)) / 8192;
    }

    result = (g_MemCardWork.hasNewDevice == true) ? MemCardWorkResult_NewDevice : MemCardWorkResult_NoNewDevice;

    g_MemCardWork.state     = MemCardWorkState_Idle;
    g_MemCardWork.stateStep = 0;

    return result;
}

/** @brief Tries to generates memory card file.
 *
 * Scratch: https://decomp.me/scratch/yksQ6
 *
 * @return `e_MemCardWorkResult`.
 */
static s32 MemCard_State_FileCreate(void)
{
    s32 result;

    result = MemCardWorkResult_Success;

    switch (g_MemCardWork.stateStep)
    {
        case 0:
            g_MemCardWork.retryCount = 0;
            g_MemCardWork.field_7C   = 0;
            g_MemCardWork.stateStep  = 1;

        case 1:
            g_MemCardWork.fileHandle = open(g_MemCardWork.filePath, (g_MemCardWork.createBlockCount << 16) | O_CREAT);
            if (g_MemCardWork.fileHandle == NO_VALUE)
            {
                if (g_MemCardWork.retryCount++ >= 15)
                {
                    result                  = MemCardWorkResult_FileCreateError;
                    g_MemCardWork.state     = MemCardWorkState_Idle;
                    g_MemCardWork.stateStep = 0;
                    break;
                }
            }
            else
            {
                close(g_MemCardWork.fileHandle);
                g_MemCardWork.state     = MemCardWorkState_FileOpen;
                g_MemCardWork.stateStep = 0;
            }
            break;
    }

    return result;
}

/** @brief Tries to opens memory card file.
 *
 * Scratch: https://decomp.me/scratch/8NOEJ
 *
 * @return `e_MemCardWorkResult`.
 */
static s32 MemCard_State_FileOpen(void)
{
    s32 mode;
    s32 result;

    result = MemCardWorkResult_Success;

    switch (g_MemCardWork.stateStep)
    {
        case 0:
            g_MemCardWork.retryCount = 0;
            g_MemCardWork.field_7C   = 0;
            g_MemCardWork.stateStep  = 1;

        case 1:
            switch (g_MemCardWork.MemCardWorkIoMode)
            {
                case MemCardWorkIoMode_Read:
                    mode = O_RDONLY;
                    break;

                case MemCardWorkIoMode_Write:
                case MemCardWorkIoMode_Create:
                    mode = O_WRONLY;
                    break;

                default:
                    mode = 0;
                    break;
            }

            g_MemCardWork.fileHandle = open(g_MemCardWork.filePath, mode | O_NOWAIT);
            if (g_MemCardWork.fileHandle == NO_VALUE)
            {
                if (g_MemCardWork.retryCount++ >= 15)
                {
                    result                  = MemCardWorkResult_FileOpenError;
                    g_MemCardWork.state     = MemCardWorkState_Idle;
                    g_MemCardWork.stateStep = 0;
                    break;
                }
            }
            else
            {
                g_MemCardWork.state     = MemCardWorkState_FileReadWrite;
                g_MemCardWork.stateStep = 0;
            }
            break;
    }

    return result;
}

/** @brief Tries to manage (read or write) data from a memory card file.
 *
 * Scratch: https://decomp.me/scratch/DgVXq
 *
 * @return `e_MemCardWorkResult`.
 */
static s32 MemCard_State_FileReadWrite(void)
{
    s32 result;
    s32 ioResult;

    result = MemCardWorkResult_Success;

    switch (g_MemCardWork.stateStep)
    {
        case 0:
            g_MemCardWork.retryCount = 0;
            g_MemCardWork.field_7C   = 0;
            g_MemCardWork.stateStep  = 1;

        case 1:
            if (lseek(g_MemCardWork.fileHandle, g_MemCardWork.seekOffset, SEEK_SET) == NO_VALUE)
            {
                if (g_MemCardWork.retryCount++ >= 15)
                {
                    result                  = MemCardWorkResult_FileSeekError;
                    g_MemCardWork.state     = MemCardWorkState_Idle;
                    g_MemCardWork.stateStep = 0;
                }
            }
            else
            {
                g_MemCardWork.retryCount = 0;
                g_MemCardWork.stateStep++;
            }
            break;

        case 2:
            MemCard_SwEventsReset();

            switch (g_MemCardWork.MemCardWorkIoMode)
            {
                case MemCardWorkIoMode_Read:
                    ioResult = read(g_MemCardWork.fileHandle, g_MemCardWork.dataBuffer, g_MemCardWork.dataSize);
                    break;

                case MemCardWorkIoMode_Write:
                case MemCardWorkIoMode_Create:
                    ioResult = write(g_MemCardWork.fileHandle, g_MemCardWork.dataBuffer, g_MemCardWork.dataSize);
                    break;

                default:
                    ioResult = NO_VALUE;
                    break;
            }

            if (ioResult == NO_VALUE)
            {
                if (g_MemCardWork.retryCount++ >= 15)
                {
                    result                  = MemCardWorkResult_FileIoError;
                    g_MemCardWork.state     = MemCardWorkState_Idle;
                    g_MemCardWork.stateStep = 0;
                    close(g_MemCardWork.fileHandle);
                }
            }
            else
            {
                g_MemCardWork.stateStep++;
            }
            break;

        case 3:
            switch (MemCard_SwEventsTest())
            {
                case EvSpIOE: // Completed.
                    result                  = MemCardWorkResult_FileIoComplete;
                    g_MemCardWork.state     = MemCardWorkState_Idle;
                    g_MemCardWork.stateStep = 0;
                    close(g_MemCardWork.fileHandle);
                    break;

                case EvSpTIMOUT: // Card not connected.
                    result                  = MemCardWorkResult_NotConnected;
                    g_MemCardWork.state     = MemCardWorkState_Idle;
                    g_MemCardWork.stateStep = 0;
                    close(g_MemCardWork.fileHandle);
                    break;

                case EvSpNEW: // New card detected.
                    result                  = MemCardWorkResult_FileIoError;
                    g_MemCardWork.state     = MemCardWorkState_Idle;
                    g_MemCardWork.stateStep = 0;
                    close(g_MemCardWork.fileHandle);

                case EvSpERROR: // Error.
                    g_MemCardWork.stateStep = 1;
                    break;
            }
    }

    return result;
}

/** @brief
 *
 * Scratch: https://decomp.me/scratch/JLcsG
 *
 * @param deviceId Memory card index.
 * @param result Generated device path string.
 */
static void MemCard_DevicePathGenerate(s32 deviceId, char* result)
{
    strcpy(result, "buXX:");

    // Convert sequential device ID to PSX channel number.
    result[2] = '0' + ((deviceId & 4) >> 2);
    result[3] = '0' + (deviceId & 3);
}

