#include "common.h"

#include <psyq/libapi.h>

#include "bodyprog/libsd/libsd.h"

bool sd_interrupt_start_flag = false;
s16  sd_keyoff_mode          = 0;
// 2 bytes of padding.
bool sd_mono_st_flag         = false;
s32  sd_reverb_mode          = 0;
u32  body_partly_size        = 0;

#include "smf_tables.h"

s32 sd_reserved_voice = 24;
u32 spu_reverb_sw     = 0;

u32 spu_ch_tbl[24] = {
    1 << 0,
    1 << 1,
    1 << 2,
    1 << 3,
    1 << 4,
    1 << 5,
    1 << 6,
    1 << 7,
    1 << 8,
    1 << 9,
    1 << 10,
    1 << 11,
    1 << 12,
    1 << 13,
    1 << 14,
    1 << 15,
    1 << 16,
    1 << 17,
    1 << 18,
    1 << 19,
    1 << 20,
    1 << 21,
    1 << 22,
    1 << 23
};

bool sd_int_flag    = false;
bool sd_int_flag2   = false;
bool smf_start_flag = false;
s32  sd_timer_sync  = 0;
u32  timer_count[5] = { 0x1999, 0x2000, 0x4000, 0xFFFF, 0x1A80 };
bool sd_timer_flag  = false;
s32  time_flag      = 0xC0;
s32  smf_file_no    = 0;
u32  print_start    = 0;
char eof_char[3]    = { 0xFF, 0x2F, 0x00 };
// 1 byte of padding.
s32  chantype[18]   = { 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 1, 1, 2, 0, 0, 0 };

extern bool sd_timer_flag; // TODO: Only used in this file.

bool smf_timer(void) // 0x800A6D18
{
    if (!sd_interrupt_start_flag || sd_int_flag)
    {
        return true;
    }

    if (!sd_int_flag2)
    {
        sd_int_flag2 = true;
        midi_smf_main();

        if (sd_timer_sync >= 11)
        {
            midi_vsync();
            SdAutoKeyOffCheck();
            sd_timer_sync = 0;
        }

        sd_int_flag2 = false;
        sd_timer_sync++;
    }

    return false;
}

void smf_timer_set(void) // 0x800A6DC0
{
    if (!sd_timer_flag)
    {
        sd_timer_flag = true;

        EnterCriticalSection();
        sd_timer_event = OpenEvent(RCntCNT2, EvSpINT, EvMdINTR, smf_timer);
        EnableEvent(sd_timer_event);
        SetRCnt(RCntCNT2, 7328, RCntMdINTR); // ~30Hz?
        StartRCnt(RCntCNT2);
        ExitCriticalSection();

        sd_int_flag   = false;
        sd_timer_flag = false;
    }
}

void smf_timer_end(void) // 0x800A6E58
{
    sd_timer_flag = true;

    EnterCriticalSection();
    StopRCnt(RCntCNT2);
    DisableEvent(sd_timer_event);
    CloseEvent(sd_timer_event);
    ExitCriticalSection();

    sd_timer_flag = false;
    sd_int_flag   = false;
}

void smf_timer_stop(void) // 0x800A6EC8
{
    sd_timer_flag = true;

    EnterCriticalSection();
    StopRCnt(RCntCNT2);
    ExitCriticalSection();

    sd_timer_flag = false;
    sd_int_flag   = false;
}

void smf_vsync(void) // 0x800A6F14
{
    if (sd_int_flag2)
    {
        return;
    }

    sd_int_flag2 = true;

    if (smf_start_flag)
    {
        midi_smf_main();
        midi_smf_main();
        midi_smf_main();
        midi_smf_main();
        midi_smf_main();
        midi_smf_main();
        midi_smf_main();
        midi_smf_main();
        midi_smf_main();
        midi_smf_main();
    }

    midi_vsync();
    SdAutoKeyOffCheck();
    sd_int_flag2 = false;
}
