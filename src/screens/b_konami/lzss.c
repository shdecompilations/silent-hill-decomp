#include "bodyprog/game.h"

#include <psyq/libetc.h>

#include "main/fs_queue.h"
#include "screens/b_konami/lzss.h"

// Constants from `LZSS.C`
// https://github.com/mattseabrook/LZSS/blob/f7a5b206bdb1964bbbeb660b464fc3e65e754eb5/1989/LZSS.C

#define N         4096 /* size of ring buffer */
#define F         18   /* upper limit for match_length */
#define THRESHOLD 2    /* encode string into position and length if match_length is greater than this */

static u8*  g_Lzss_Window = FS_BUFFER_1;
static bool g_Lzss_IsActive;
static u8*  g_Lzss_CurPosition;
static u8*  g_Lzss_OutPosition;
static s32  g_Lzss_WindowOffset;
static u8*  g_Lzss_EndPosition;
static s32  g_Lzss_CurFlag;

void Lzss_Reset(void) // 0x800CA234
{
    g_Lzss_IsActive = false;
}

s32 Lzss_DecompressedSizeGet(u8* inData) // 0x800CA240
{
    return *(s32*)inData;
}

void Lzss_Init(u8* inData, u8* outData, s32 inDataLen) // 0x800CA24C
{
    s32 i;

    g_Lzss_IsActive     = true;
    g_Lzss_CurPosition  = inData;
    g_Lzss_OutPosition  = outData;
    g_Lzss_EndPosition  = inData + inDataLen;
    g_Lzss_CurPosition += sizeof(s32); // Skip 4 byte uncompressed-size header.

    for (i = 0; i < N; i += sizeof(s32))
    {
        *(s32*)&g_Lzss_Window[i] = 0;
    }

    g_Lzss_WindowOffset = N - F;
    g_Lzss_CurFlag = 0;
}

bool Lzss_IsActive(void) // 0x800CA2B8
{
    return g_Lzss_IsActive;
}

void Lzss_Decode(s32 timeLimit) // 0x800CA2C8
{
    static const s32 timeVar0 = 25;
    static const s32 timeVar1 = 180;

    s32 temp_v0;
    s32 currentTime;
    s32 temp_v1_2;

    s32 var_a1;

    s32 windowOffset; // `g_Lzss_WindowOffset`
    u32 flag;         // `g_Lzss_CurFlag`
    s32 expectedTime;

    u32 temp_v0_2;
    u32 temp_v0_3;
    u32 temp_v1;

    u8* curPos;    // `g_Lzss_CurPosition`
    u8* endPos;    // `g_Lzss_EndPosition`
    u8* windowPos;
    u8* outPos;    // `g_Lzss_OutPosition`

    if (g_Lzss_IsActive != true)
    {
        return;
    }

    expectedTime = (timeLimit * 263) - timeVar0; // 263.5 scanlines per frame.

    curPos       = g_Lzss_CurPosition;
    outPos       = g_Lzss_OutPosition;
    windowOffset = g_Lzss_WindowOffset;
    flag         = g_Lzss_CurFlag;
    endPos       = g_Lzss_EndPosition;

    currentTime = VSync(SyncMode_Immediate);

    if (expectedTime - currentTime < timeVar1)
    {
        expectedTime  = currentTime + 468;
        expectedTime  = (expectedTime / 263) * 263;
        expectedTime -= timeVar0;
    }

    while (true)
    {
        if (curPos == endPos)
        {
            g_Lzss_IsActive = false;
            break;
        }

        // Pause decompression if time limit is passed.
        // State vars will be written to globals after breaking, allowing the func to run again on the next tick.
        // Pausing is only allowed when bit 9 is clear (after 8 shifts), meaning a new flag is about to be read after
        // the shift right below, ensuring it pauses at boundaries between flag bytes rather than during mid-process.
        // NOTE: SH only seems to pass `NO_VALUE` to this, so this pause code goes unused.
        if (!(flag & (1 << 9)) && VSync(SyncMode_Immediate) > expectedTime && timeLimit != NO_VALUE)
        {
            break;
        }

        // Shift register. If 'sentinel' bit at bit 8 is gone,
        // 8 bits have been processed and need to read new flag byte.
        flag = flag >> 1;

        if (!(flag & (1 << 8)))
        {
            // Load new flag and set sentinel bits above 8th bit.
            flag = *curPos++;
            flag |= 0xFF00;
        }

        // LSB now contains current flag.
        if (flag & 1)
        {
            // Flag 1 = Copy literal byte.
            windowPos  = &g_Lzss_Window[windowOffset];
            temp_v1    = *curPos++;
            *windowPos = temp_v1;
            *outPos++  = temp_v1;
            windowOffset++;
            windowOffset &= (N - 1);
        }
        else
        {
            // Flag 0 = Copy reference (2 bytes).
            if (curPos == (endPos - 1))
            {
                // Not enough room for LZSS reference, end decoding.
                g_Lzss_IsActive = false;
                break;
            }

            temp_v0 = *curPos;
            curPos++;

            temp_v0_2 = *curPos & 0xF0;
            temp_v0_3 = (*curPos & 0xF) + THRESHOLD + 1;
            temp_v0_2 = temp_v0_2 << 4;

            var_a1    = temp_v0 | temp_v0_2;
            temp_v1_2 = var_a1 + temp_v0_3;
            curPos++;

            // Copy data from window into output.
            for (; var_a1 < (s16)temp_v1_2; var_a1++, outPos++)
            {
                temp_v0_3 = g_Lzss_Window[var_a1 & (N - 1)];

                g_Lzss_Window[windowOffset] = temp_v0_3;
                *outPos = temp_v0_3;

                windowOffset++;
                windowOffset &= (N - 1);
            }
        }
    }

    g_Lzss_CurPosition = curPos;
    g_Lzss_OutPosition = outPos;
    g_Lzss_WindowOffset = windowOffset;
    g_Lzss_EndPosition = endPos;
    g_Lzss_CurFlag = flag;
}
