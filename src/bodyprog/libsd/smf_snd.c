#include "common.h"

#include <psyq/libspu.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/libsd.h"

VAB_H        vab_h[SD_VAB_SLOTS];
u8           sd_vb_malloc_rec[136];
s16          sd_seq_loop_mode;
s16          __pad_bss_800C7662;
s32          sd_tick_mode;
SD_SPU_ALLOC sd_spu_alloc[SD_ALLOC_SLOTS];

void tone_adsr_mem(s16 vab_id) // 0x8009EDA4
{
    SD_VAB_H* vabData;
    VagAtr*   vagAtr;
    VabHdr*   vabHdr;
    s32       vagIdx;
    s32       progIdx;

    vabData = vab_h[vab_id].vh_addr_4;
    vabHdr  = &vabData->vab_h;

    for (progIdx = 0; progIdx < vabHdr->ps; progIdx++)
    {
        for (vagIdx = 0; vagIdx < 16; vagIdx++)
        {
            vagAtr = &vabData->vag_atr[(progIdx * 16) + vagIdx];

            vagAtr->reserved[0] = vagAtr->adsr1;
            vagAtr->reserved[1] = vagAtr->adsr2;
        }
    }
}

void tone_adsr_back(s16 vab_id) // 0x8009EE30
{
    SD_VAB_H* vabData;
    VagAtr*   vagAtr;
    VabHdr*   vabHdr;
    s32       vagIdx;
    s32       progIdx;

    vabData = vab_h[vab_id].vh_addr_4;
    vabHdr  = &vabData->vab_h;

    for (progIdx = 0; progIdx < vabHdr->ps; progIdx++)
    {
        for (vagIdx = 0; vagIdx < 16; vagIdx++)
        {
            vagAtr = &vabData->vag_atr[(progIdx * 16) + vagIdx];

            vagAtr->adsr1 = vagAtr->reserved[0];
            vagAtr->adsr2 = vagAtr->reserved[1];
        }
    }
}

void sd_alloc_sort(void) // 0x8009EEBC
{
    s32 lastSlotIdx;
    s32 frontIdx;
    s32 backIdx;

    for (lastSlotIdx = 0; lastSlotIdx < SD_ALLOC_SLOTS; lastSlotIdx++)
    {
        if (sd_spu_alloc[lastSlotIdx].addr_0 == 0)
        {
            break;
        }
    }

    for (frontIdx = 0; frontIdx < lastSlotIdx; frontIdx++)
    {
        for (backIdx = lastSlotIdx - 1; frontIdx < backIdx; backIdx--)
        {
            if (sd_spu_alloc[frontIdx].addr_0 > sd_spu_alloc[backIdx].addr_0)
            {
                SD_SPU_ALLOC temp;
                temp.size_4 = sd_spu_alloc[frontIdx].size_4;
                temp.addr_0 = sd_spu_alloc[frontIdx].addr_0;

                sd_spu_alloc[frontIdx].size_4 = sd_spu_alloc[backIdx].size_4;
                sd_spu_alloc[frontIdx].addr_0 = sd_spu_alloc[backIdx].addr_0;

                sd_spu_alloc[backIdx].size_4 = temp.size_4;
                sd_spu_alloc[backIdx].addr_0 = temp.addr_0;
            }
        }
    }
}

s32 SdSpuMalloc(s32 size)
{
    s32 i;
    s32 bytesToNextAlloc;
    u32 addr;

    addr = 0x1010;

    if (sd_spu_alloc[0].addr_0 == 0)
    {
        if (addr + size < 0x80000 - sd_reverb_area_size[sd_reverb_mode])
        {
            sd_spu_alloc[0].addr_0 = addr;
            sd_spu_alloc[0].size_4 = size;

            sd_alloc_sort();
            return addr;
        }

        return -1;
    }

    for (i = 0; i < 16; i++)
    {
        if (sd_spu_alloc[i].size_4 == 0)
        {
            if ((addr + size) < (0x80000 - sd_reverb_area_size[sd_reverb_mode]))
            {
                sd_spu_alloc[i].addr_0 = addr;
                sd_spu_alloc[i].size_4 = size;
                sd_alloc_sort();
                return addr;
            }

            return -1u;
        }

        if (sd_spu_alloc[i + 1].size_4 != 0)
        {
            bytesToNextAlloc = sd_spu_alloc[i + 1].addr_0 - (addr + sd_spu_alloc[i].size_4);

            // If there's enough free space between this block and the next alloc to fit `size` bytes, allocate new block in that gap.
            if (bytesToNextAlloc >= size)
            {
                bytesToNextAlloc = sd_spu_alloc[i].size_4; // Not sure why it replaces the var here...

                for (; i < 16; i++)
                {
                    if (sd_spu_alloc[i].size_4 != 0)
                    {
                        continue;
                    }
                    sd_spu_alloc[i].addr_0 = addr + bytesToNextAlloc; // Set addr to end of this alloc (blockStart + blockSize)
                    sd_spu_alloc[i].size_4 = size;
                    break;
                }

                if (i != 0x10)
                {
                    sd_alloc_sort();
                    return addr;
                }

                return -1u;
            }
        }

        addr = sd_spu_alloc[i].addr_0 + sd_spu_alloc[i].size_4;
    }

    sd_alloc_sort();
    return addr;
}

u32 SdSpuMallocWithStartAddr(u32 addr, s32 size) // 0x8009F120
{
    s32 i;
    u32 end_addr;

    if (addr < 0x1010 || addr >= 0x80000 || (addr + size) >= 0x80000)
    {
        return -1;
    }

    if (sd_spu_alloc[0].addr_0 == 0)
    {
        if ((addr + size) < (0x80000 - sd_reverb_area_size[sd_reverb_mode]))
        {
            sd_spu_alloc[0].addr_0 = addr;
            sd_spu_alloc[0].size_4 = size;

            sd_alloc_sort();
            return addr;
        }

        return -1;
    }

    for (i = 0; i < 16; i++)
    {
        if (i == 0 && addr < sd_spu_alloc[i].addr_0)
        {
            if (sd_spu_alloc[i].addr_0 < addr + size)
            {
                return -1;
            }

            for (; i < 16; i++)
            {
                if (sd_spu_alloc[i].size_4 != 0)
                {
                    continue;
                }

                sd_spu_alloc[i].addr_0 = addr;
                sd_spu_alloc[i].size_4 = size;
                break;
            }

            if (i != 16)
            {
                break;
            }
            else
            {
                return -1;
            }
        }
        else
        {
            if (addr < sd_spu_alloc[i].addr_0)
            {
                continue;
            }
        }

        if (sd_spu_alloc[i].addr_0 == addr)
        {
            sd_spu_alloc[i].addr_0 = addr;
            sd_spu_alloc[i].size_4 = size;
            return addr;
        }

        if (sd_spu_alloc[i + 1].size_4 == 0)
        {
            if ((addr + size) < (0x80000 - sd_reverb_area_size[sd_reverb_mode]))
            {
                sd_spu_alloc[i + 1].addr_0 = addr;
                sd_spu_alloc[i + 1].size_4 = size;
                break;
            }

            return -1;
        }

        if (sd_spu_alloc[i].addr_0 < addr && addr < sd_spu_alloc[i + 1].addr_0)
        {
            if (addr < sd_spu_alloc[i].addr_0 + sd_spu_alloc[i].size_4)
            {
                return -1;
            }

            end_addr = addr;
            end_addr = end_addr + size;

            if (sd_spu_alloc[i + 1].addr_0 < end_addr)
            {
                return -1;
            }

            for (; i < 16; i++)
            {
                if (sd_spu_alloc[i].size_4 != 0)
                {
                    continue;
                }

                sd_spu_alloc[i].addr_0 = addr;
                sd_spu_alloc[i].size_4 = size;
                break;
            }

            if (i != 16)
            {
                break;
            }
            else
            {
                return -1;
            }
        }
    }

    sd_alloc_sort();
    return addr;
}

void SdSpuFree(u32 addr) // 0x8009F364
{
    s32 i;

    for (i = 0; i < SD_ALLOC_SLOTS; i++)
    {
        if (sd_spu_alloc[i].addr_0 != addr)
        {
            continue;
        }

        // TODO: could this be moved out of the for loop above?
        for (; i < SD_ALLOC_SLOTS; i++)
        {
            sd_spu_alloc[i].size_4 = sd_spu_alloc[i + 1].size_4;
            sd_spu_alloc[i].addr_0 = sd_spu_alloc[i + 1].addr_0;
            if (!sd_spu_alloc[i].addr_0)
                break;
        }
        sd_alloc_sort();
        return;
    }
}

void SdWorkInit(void) // 0x8009F400
{
    s32 i;

    STATIC_ASSERT(SD_VAB_SLOTS == SD_ALLOC_SLOTS, SD_VAB_SLOTS_must_equal_SD_ALLOC_SLOTS);

    for (i = 0; i < SD_VAB_SLOTS; i++)
    {
        vab_h[i].vab_id      = NO_VALUE;
        vab_h[i].mvoll_19      = 0x7F;
        vab_h[i].mvolr_1A      = 0x7F;
        vab_h[i].mpan_1B       = 0x40;
        sd_spu_alloc[i].size_4 = 0;
        sd_spu_alloc[i].addr_0 = 0;
    }

    for (i = 0; i < 2; i++)
    {
        smf_song[i].sd_seq_vab_id_508 = NO_VALUE;
        smf_song[i].sd_seq_stat_50A   = SEQ_NON;
        smf_song[i].sd_seq_mvolr_50E  = 0x7F;
        smf_song[i].sd_seq_mvoll_50C  = 0x7F;
    }

    sd_tick_mode = 4096;
}

void SdInit(void) // 0x8009F490
{
    SpuInit();
    sound_off();
    SdWorkInit();
    SpuInitMalloc(16, &sd_vb_malloc_rec);
}

void SdStart(void) // 0x8009F4D0
{
    if (sd_tick_mode > 0)
    {
        if (sd_tick_mode < 4)
        {
            smf_timer_set();
        }
    }

    sd_interrupt_start_flag = true;
}

void SdStart2(void) // 0x8009F510
{
    SdStart();
}

void SdSetTickMode(s32 tick_mode) // 0x8009F530
{
    sd_tick_mode = tick_mode;
}

void SdSeqCalledTbyT(void) // 0x8009F53C
{
    if (sd_interrupt_start_flag)
    {
        smf_vsync();
    }
}

void SdSetStereo(void) // 0x8009F568
{
    sd_mono_st_flag = false;
}

void SdSetMono(void) // 0x8009F574
{
    sd_mono_st_flag = true;
}

char SdSetReservedVoice(char voices) // 0x8009F584
{
    if (voices >= 0x19)
    {
        return -1;
    }
    if (voices == 0)
    {
        return -1;
    }

    sd_reserved_voice = voices;
    return voices;
}

void SdSetTableSize(void* arg0, s32 arg1, s32 arg2) {} // 0x8009F5B8

void SdEnd(void) // 0x8009F5C0
{
    s32 i;

    sound_off();

    if (sd_tick_mode > 0)
    {
        if (sd_tick_mode < 4)
        {
            smf_timer_end();
        }
    }

    for (i = 0; i < SD_VAB_SLOTS; i++)
    {
        if (vab_h[i].vab_id >= 0)
        {
            SdSpuFree(vab_h[i].vb_start_addr_10);
        }
    }

    SdWorkInit();
}

void SdQuit(void) // 0x8009F64C
{
    sound_off();
    SdWorkInit();
    SpuQuit();
}

void SdSetSerialAttr(char s_num, char attr, char mode) // 0x8009F67C
{
    SpuCommonAttr spu_attr;

    if (s_num == SS_SERIAL_A)
    {
        if (attr == SS_MIX)
        {
            spu_attr.mask   = SPU_COMMON_CDMIX;
            spu_attr.cd.mix = mode;
        }
        else // SS_REV
        {
            spu_attr.mask      = SPU_COMMON_CDREV;
            spu_attr.cd.reverb = mode;
        }
    }
    else // SS_SERIAL_B
    {
        if (attr == SS_MIX)
        {
            spu_attr.mask    = SPU_COMMON_EXTMIX;
            spu_attr.ext.mix = mode;
        }
        else // SS_REV
        {
            spu_attr.mask       = SPU_COMMON_EXTREV;
            spu_attr.ext.reverb = mode;
        }
    }
    SpuSetCommonAttr(&spu_attr);
}

void SdSetSerialVol(s16 s_num, s16 voll, s16 volr) // 0x8009F700
{
    // TODO: libsnd SsSetSerialVol uses char for s_num
    // callers also seem to pass char, but only matches with s16 right now?

    SpuCommonAttr attr;

    s32 v_left  = voll << 8;
    s32 v_right = volr << 8;

    if (s_num == SS_SERIAL_A)
    {
        attr.mask            = SPU_COMMON_CDVOLL | SPU_COMMON_CDVOLR;
        attr.cd.volume.left  = v_left;
        attr.cd.volume.right = v_right;
    }
    else // SS_SERIAL_B
    {
        attr.mask             = SPU_COMMON_EXTVOLL | SPU_COMMON_EXTVOLR;
        attr.ext.volume.left  = v_left;
        attr.ext.volume.right = v_right;
    }

    SpuSetCommonAttr(&attr);
}

void SdSetMVol(s16 voll, s16 volr) // 0x8009F75C
{
    SpuCommonAttr attr;

    attr.mask           = SPU_COMMON_MVOLL | SPU_COMMON_MVOLR | SPU_COMMON_MVOLMODEL | SPU_COMMON_MVOLMODER;
    attr.mvol.left      = voll << 7;
    attr.mvol.right     = volr << 7;
    attr.mvolmode.left  = 0;
    attr.mvolmode.right = 0;

    SpuSetCommonAttr(&attr);
}

s16 SdVabOpenHead(u8* addr, s16 vabid) // 0x8009F79C
{
    VAB_H*  p;
    s16     i;
    VabHdr* sd_vab_h;

    body_partly_size = 0;

    if (*addr != 0x70)
    {
        return -1;
    }

    i = 0;
    if (vabid == -1)
    {
        while (true)
        {
            if (vab_h[i].vab_id == -1)
            {
                break;
            }

            i++;

            if (i >= 0x10)
            {
                return -1;
            }
        }
    }
    else
    {
        i = vabid;

        if (vab_h[i].vab_id != -1)
        {
            SdSpuFree(vab_h[i].vb_start_addr_10);
        }
    }

    p        = &vab_h[i];
    sd_vab_h = (VabHdr*)addr;

    p->vab_id         = i;
    p->vh_addr_4        = (SD_VAB_H*)sd_vab_h;
    p->vh_size_8        = 0xA20 + (sd_vab_h->ps * 0x200);
    p->vb_size_14       = sd_vab_h->fsize - p->vh_size_8;
    p->mvol_18          = sd_vab_h->mvol;
    p->mpan_1B          = sd_vab_h->pan;
    p->vb_start_addr_10 = SdSpuMalloc(p->vb_size_14);

    if (p->vb_start_addr_10 == -1)
    {
        return -1;
    }

    tone_adsr_mem(i);
    return i;
}

s16 SdVabOpenHeadSticky(u8* addr, s16 vabid, s32 sbaddr) // 0x8009F91C
{
    VAB_H*  p;
    s16     i;
    VabHdr* sd_vab_h;

    i                = 0;
    body_partly_size = 0;

    if (*addr != 0x70)
    {
        return -1;
    }

    sd_vab_h = (VabHdr*)addr;

    if (vabid == -1)
    {
        while (true)
        {
            if (vab_h[i].vab_id == -1)
            {
                break;
            }

            i++;

            if (i >= 0x10)
            {
                return -1;
            }
        }
    }
    else
    {
        i = vabid;
        if (i >= 0x10)
        {
            return -1;
        }

        if (vab_h[i].vab_id != -1)
        {
            SdSpuFree(vab_h[i].vb_start_addr_10);
        }
    }

    p = &vab_h[i];

    p->vab_id         = i;
    p->vh_addr_4        = (SD_VAB_H*)addr;
    p->vh_size_8        = 0xA20 + (sd_vab_h->ps * 0x200);
    p->vb_start_addr_10 = sbaddr;
    p->vb_size_14       = sd_vab_h->fsize - p->vh_size_8;
    p->mvol_18          = sd_vab_h->mvol;
    p->mpan_1B          = sd_vab_h->pan;
    p->vb_start_addr_10 = SdSpuMallocWithStartAddr(sbaddr, p->vb_size_14);

    if (p->vb_start_addr_10 == -1)
    {
        return -1;
    }

    tone_adsr_mem(i);
    return i;
}

s16 SdVabFakeHead(u8* addr, s16 vabid, u32 sbaddr) // 0x8009FAC0
{
    VAB_H*  p;
    VabHdr* sd_vab_h;
    s16     i;

    if (*addr != 0x70)
    {
        return -1;
    }

    i = 0;

    if (vabid == -1)
    {
        while (true)
        {
            if (vab_h[i].vab_id == -1)
            {
                break;
            }

            i++;

            if (i >= 0x10)
            {
                return -1;
            }
        }
    }
    else
    {
        i = vabid;
    }

    p        = &vab_h[i];
    sd_vab_h = (VabHdr*)addr;

    p->vab_id         = i;
    p->vh_addr_4        = (SD_VAB_H*)sd_vab_h;
    p->vh_size_8        = 0xA20 + (sd_vab_h->ps * 0x200);
    p->vb_size_14       = sd_vab_h->fsize - p->vh_size_8;
    p->mvol_18          = sd_vab_h->mvol;
    p->mpan_1B          = sd_vab_h->pan;
    p->vb_start_addr_10 = sbaddr;

    return i;
}

s32 SdVbOpenOne(u8* addr, s32 sbaddr, s32 sbsize, s16 vabid) // 0x8009FBAC
{
    VAB_H* p;
    s16    i;

    body_partly_size = 0;
    i                = 0;

    if (vabid == -1)
    {
        while (true)
        {
            if (vab_h[i].vab_id == -1)
            {
                break;
            }

            i++;

            if (i >= 16)
            {
                return -1;
            }
        }
    }
    else
    {
        i = vabid;
        if (vab_h[i].vab_id != -1)
        {
            SdSpuFree(vab_h[i].vb_start_addr_10);
        }
    }

    p             = &vab_h[i];
    p->vb_size_14 = sbsize;
    p->vab_id   = i;
    p->vh_addr_4  = (SD_VAB_H*)addr;
    p->vh_size_8  = sbsize;
    p->mvol_18    = 127;
    p->vb_size_14 = sbsize;
    p->mpan_1B    = 64;

    p->vb_start_addr_10 = sbaddr;
    p->vb_start_addr_10 = SdSpuMallocWithStartAddr(sbaddr, p->vb_size_14);

    if (p->vb_start_addr_10 == -1)
    {
        return -1;
    }

    SpuSetTransferStartAddr(p->vb_start_addr_10);
    if (SpuWrite(addr, p->vb_size_14) != p->vb_size_14)
    {
        return -1;
    }

    p->vb_addr_C = addr;
    return i;
}

s16 SdVabTransBody(u8* addr, s16 vabid) // 0x8009FD38
{
    s32 vab_h_id;

    if (vabid < 0)
    {
        return -1;
    }

    vab_h_id = vab_h[vabid].vab_id;
    if (vab_h_id >= 0 && vab_h_id == vabid)
    {
        SpuSetTransferStartAddr(vab_h[vabid].vb_start_addr_10);
        if (SpuWrite(addr, vab_h[vabid].vb_size_14) == vab_h[vabid].vb_size_14)
        {
            vab_h[vabid].vb_addr_C = addr;
            return vab_h_id;
        }
    }

    return -1;
}

s16 SdVabTransBodyPartly(u8* addr, u32 bufsize, s16 vabid) // 0x8009FDDC
{
    s16 vab_h_id;
    s32 retval;

    if (vabid < 0)
    {
        return -1;
    }

    vab_h_id = vab_h[vabid].vab_id;
    retval   = -1;

    if (vab_h_id >= 0 && vab_h_id == vabid)
    {
        SpuSetTransferStartAddr(vab_h[vabid].vb_start_addr_10 + body_partly_size);
        if (vab_h[vabid].vb_size_14 - body_partly_size < bufsize)
        {
            bufsize = vab_h[vabid].vb_size_14 - body_partly_size;
        }
        if (SpuWrite(addr, bufsize) != bufsize)
        {
            return -1;
        }

        body_partly_size += bufsize;
        retval = vab_h_id;

        if (body_partly_size < vab_h[vabid].vb_size_14)
        {
            return -2;
        }

        return vab_h_id; // Maybe was meant to be return retval but devs just returned vab_h_id?
    }
    return retval;
}

s16 SdVabTransCompleted(s16 immediateFlag) // 0x8009FEC4
{
    if (!immediateFlag)
    {
        return SpuIsTransferCompleted(0);
    }

    return SpuIsTransferCompleted(1);
}

void SdVabClose(s16 vab_id) // 0x8009FF00
{
    if (vab_h[vab_id].vab_id != -1)
    {
        SdSpuFree(vab_h[vab_id].vb_start_addr_10);
        vab_h[vab_id].vab_id = -1;
    }
}

void SdSetAutoKeyOffMode(s16 mode) // 0x8009FF64
{
    sd_keyoff_mode = mode;
}

void SdAutoKeyOffCheck(void) // 0x8009FF70
{
    s32 stat;
    s32 vo;
    u32 voices;

    voices = 0;

    if (sd_keyoff_mode == 0)
    {
        for (vo = 0; vo < sd_reserved_voice; vo++)
        {
            if (smf_port[vo].stat_16 != 0 && SpuGetKeyStatus(spu_ch_tbl[vo]) == SPU_ON_ENV_OFF)
            {
                if (smf_port[vo].stat_16 >= 2u)
                {
                    voices |= spu_ch_tbl[vo];

                    do
                    {
                        SpuSetKey(SPU_OFF, spu_ch_tbl[vo]);
                        stat = SpuGetKeyStatus(spu_ch_tbl[vo]);
                    }
                    while (stat != SPU_OFF_ENV_ON && stat != SPU_OFF);

                    smf_port[vo].stat_16 = 0;

                    func_800485C0(vo);
                }
                else
                {
                    smf_port[vo].stat_16++;
                }
            }
        }
    }

    if (voices != 0)
    {
        SpuSetKey(SPU_OFF, voices);
    }
}

s16 SdSeqOpen(s32* addr, s16 vab_id) // 0x800A00A4
{
    s32 i;

    sd_int_flag = true;

    if (*addr != SD_MAGIC_SEQp && *addr != SD_MAGIC_MThd && *addr != SD_MAGIC_KDT && *addr != SD_MAGIC_KDT1)
    {
        return -1;
    }

    for (i = 0; i < 2; i++)
    {
        if (smf_song[i].sd_seq_vab_id_508 == -1)
        {
            smf_song[i].sd_seq_stat_50A       = SEQ_STOP;
            smf_song[i].sd_seq_start_addr_514 = addr;
            smf_song[i].sd_seq_vab_id_508     = vab_id;

            sd_int_flag = false;
            return i;
        }
    }

    sd_int_flag = false;
    return -1;
}

s16 SdSeqOpenWithAccNum(s32* addr, s16 vab_id, s16 acc_num) // 0x800A0154
{
    sd_int_flag = true;

    if (*addr != SD_MAGIC_SEQp && *addr != SD_MAGIC_MThd && *addr != SD_MAGIC_KDT && *addr != SD_MAGIC_KDT1)
    {
        return -1;
    }

    if (smf_song[acc_num].sd_seq_vab_id_508 == -1)
    {
        smf_song[acc_num].sd_seq_stat_50A       = SEQ_STOP;
        smf_song[acc_num].sd_seq_start_addr_514 = addr;
        smf_song[acc_num].sd_seq_vab_id_508     = vab_id;

        sd_int_flag = false;
        return acc_num;
    }

    sd_int_flag = false;
    return -1;
}

void SdSeqPlay(s16 seq_access_num, u8 play_mode, s16 l_count) // 0x800A0210
{
    if (seq_access_num == -1)
    {
        return;
    }

    sd_int_flag = true;

    if (smf_song[seq_access_num].sd_seq_vab_id_508 == -1)
    {
        sd_int_flag = false;
        return;
    }

    sd_seq_loop_mode = l_count;

    smf_song[seq_access_num].mf_data_size_518 = 0x10000;
    smf_song[seq_access_num].mf_data_ptr_504  = smf_song[seq_access_num].sd_seq_start_addr_514;

    midi_file_out(seq_access_num);

    if (!play_mode)
    {
        smf_song[seq_access_num].sd_seq_stat_50A = SEQ_PAUSE;
    }
    else
    {
        smf_start_flag = true;

        smf_song[seq_access_num].sd_seq_stat_50A = SEQ_PLAY;
    }

    sd_int_flag = false;
}

void SdSeqStop(s16 seq_access_num) // 0x800A02D8
{
    if (seq_access_num == NO_VALUE)
    {
        return;
    }

    if (smf_song[seq_access_num].sd_seq_vab_id_508 != NO_VALUE)
    {
        sd_int_flag = true;

        midi_smf_stop(seq_access_num);
        sound_seq_off(seq_access_num);
        smf_song[seq_access_num].sd_seq_mvolr_50E = 127;
        smf_song[seq_access_num].sd_seq_mvoll_50C = 127;
        smf_song[seq_access_num].sd_seq_stat_50A  = SEQ_STOP;

        sd_int_flag = false;
    }
}

void SdSeqClose(s16 seq_access_num) // 0x800A037C
{
    if (seq_access_num == NO_VALUE)
    {
        return;
    }

    if (smf_song[seq_access_num].sd_seq_vab_id_508 != NO_VALUE)
    {
        sd_int_flag = true;

        tone_adsr_back(smf_song[seq_access_num].sd_seq_vab_id_508);
        smf_song[seq_access_num].sd_seq_vab_id_508 = NO_VALUE;
        smf_song[seq_access_num].sd_seq_stat_50A   = SEQ_NON;
        smf_song[seq_access_num].sd_seq_mvolr_50E  = 127;
        smf_song[seq_access_num].sd_seq_mvoll_50C  = 127;

        sd_int_flag = false;
    }
}

void SdSeqPause(s16 seq_access_num) // 0x800A0418
{
    SpuVoiceAttr s_attr;
    s32          vc;

    if (seq_access_num == NO_VALUE)
    {
        return;
    }

    sd_int_flag = true;

    for (vc = 0; vc < sd_reserved_voice; vc++)
    {
        if ((smf_port[vc].midi_ch_3 >> 4) == seq_access_num && smf_port[vc].stat_16 != 0)
        {
            s_attr.mask         = (SPU_VOICE_VOLL | SPU_VOICE_VOLR);
            s_attr.voice        = spu_ch_tbl[vc];
            s_attr.volume.left  = 0;
            s_attr.volume.right = 0;
            SpuSetVoiceAttr(&s_attr);
        }
    }

    smf_song[seq_access_num].sd_seq_stat_50A = SEQ_PAUSE;

    sd_int_flag = false;
}

void SdSeqReplay(s16 seq_access_num) // 0x800A0534
{
    SpuVoiceAttr  s_attr;
    SpuReverbAttr rev_attr;
    s32           vc;
    u32           temp_v1;

    if (seq_access_num == NO_VALUE)
    {
        return;
    }

    sd_int_flag = true;

    for (vc = 0; vc < sd_reserved_voice; vc++)
    {
        temp_v1 = (smf_port[vc].midi_ch_3 >> 4);
        if (temp_v1 == seq_access_num && smf_port[vc].stat_16 != 0)
        {
            s_attr.mask         = (SPU_VOICE_VOLL | SPU_VOICE_VOLR);
            s_attr.voice        = spu_ch_tbl[vc];
            s_attr.volume.left  = ((smf_port[vc].l_vol_C * smf_song[temp_v1].sd_seq_mvoll_50C) >> 7);
            s_attr.volume.right = ((smf_port[vc].r_vol_E * smf_song[temp_v1].sd_seq_mvolr_50E) >> 7);
            SpuSetVoiceAttr(&s_attr);
        }
    }

    if (smf_song[seq_access_num].seq_reverb_depth_536 != 0)
    {
        rev_attr.mask        = (SPU_REV_DEPTHL | SPU_REV_DEPTHR);
        rev_attr.depth.right = 0;
        rev_attr.depth.left  = 0;
        SpuSetReverbModeParam(&rev_attr);
        SpuSetReverb(1);
        smf_song[seq_access_num].seq_rev_set_flag_532 = 1;
    }

    smf_song[seq_access_num].sd_seq_stat_50A = SEQ_PLAY;
    control_code_set(seq_access_num);

    sd_int_flag = false;
}

void SdSeqSetVol(s16 seq_access_num, s16 voll, s16 volr) // 0x800A06F0
{
    smf_song[seq_access_num].sd_seq_mvoll_50C = voll & 0x7F;
    smf_song[seq_access_num].sd_seq_mvolr_50E = volr & 0x7F;
    if (smf_song[seq_access_num].sd_seq_stat_50A != SEQ_STOP)
    {
        smf_song[seq_access_num].seq_vol_set_flag_530 = 1;
    }
}

void SdSeqGetVol(s16 seq_access_num, s16* voll, s16* volr) // 0x800A074C
{
    *voll = smf_song[seq_access_num].sd_seq_mvoll_50C;
    *volr = smf_song[seq_access_num].sd_seq_mvolr_50E;
}

void SdUtFlush(void) // 0x800A0794
{
    SdAutoKeyOffCheck();
}

void SdUtReverbOn(void) // 0x800A07B4
{
    SpuReserveReverbWorkArea(SPU_ON);
    SpuSetReverb(SPU_ON);
}

void SdUtReverbOff(void) // 0x800A07DC
{
    sd_reverb_mode = 0;
    SpuSetReverb(SPU_OFF);
    SpuReserveReverbWorkArea(SPU_OFF);
}

s16 SdUtSetReverbType(s16 type) // 0x800A080C
{
    SpuReverbAttr r_attr;

    r_attr.mask = SPU_REV_MODE;
    r_attr.mode = type;

    if (SpuSetReverbModeParam(&r_attr))
    {
        return -1;
    }

    sd_reverb_mode = type;
    return type;
}

void SdUtSetReverbDepth(s16 ldepth, s16 rdepth) // 0x800A085C
{
    SpuReverbAttr r_attr;

    r_attr.mask        = SPU_REV_DEPTHL | SPU_REV_DEPTHR;
    r_attr.depth.left  = (ldepth << 0x10) >> 8;
    r_attr.depth.right = (rdepth << 0x10) >> 8;

    SpuSetReverbModeParam(&r_attr);
}

void SdSetRVol(s16 ldepth, s16 rdepth) // 0x800A089C
{
    SpuReverbAttr r_attr;

    r_attr.mask        = SPU_REV_DEPTHL | SPU_REV_DEPTHR;
    r_attr.depth.left  = (ldepth << 0x10) >> 8;
    r_attr.depth.right = (rdepth << 0x10) >> 8;

    SpuSetReverbModeParam(&r_attr);
}

void SdUtSEAllKeyOff(void) // 0x800A08DC
{
    s32 i;
    s32 stat   = SPU_OFF;
    u32 voices = 0;

    for (i = 0; i < sd_reserved_voice; i++)
    {
        if (smf_port[i].midi_ch_3 == 0x20)
        {
            voices |= spu_ch_tbl[i];
            smf_port[i].stat_16 = 0;
            smf_port[i].note_6  = 0;
            SpuSetKey(SPU_OFF, spu_ch_tbl[i]);
            adsr_set(i, &smf_port[i]);
            rr_off(i);
            do
            {
                SpuSetKey(SPU_OFF, spu_ch_tbl[i]);
                stat = SpuGetKeyStatus(spu_ch_tbl[i]);
            }
            while (stat != SPU_OFF_ENV_ON && stat != SPU_OFF);
        }
    }

    if (stat != SPU_OFF)
    {
        SpuSetKey(SPU_OFF, voices);
    }
}

void SdUtAllKeyOff(s16 mode) // 0x800A09E8
{
    s32 i;
    u32 voices = 0;

    for (i = 0; i < sd_reserved_voice; i++)
    {
        voices |= spu_ch_tbl[i];
    }

    SpuSetKey(SPU_OFF, voices);
}

s32 SdUtGetVabHdr(s16 vabId, VabHdr* vabhdrptr) // 0x800A0A40
{
    u8* dest;
    u8* src;
    u32 i;

    if (vab_h[vabId].vab_id == -1)
    {
        return -1;
    }

    dest = (u8*)vabhdrptr;
    src  = (u8*)vab_h[vabId].vh_addr_4;

    for (i = 0; i < sizeof(VabHdr); i++)
    {
        *dest++ = *src++;
    }

    return 0;
}

s32 SdVoKeyOn(s32 vab_pro, s32 pitch, u16 voll, u16 volr) // 0x800A0AA0
{
    ProgAtr*  sd_vab_prog;
    SD_VAB_H* sd_vh;
    VagAtr*   sd_vag_atr;
    s32       prog;
    s32       vc;
    s32       tone;
    s32       note;
    s32       voice;
    s32       pc;
    s32       c;
    s16       vabid;

    prog  = vab_pro & 0x7F;
    vabid = vab_pro >> 8;
    note  = pitch >> 8;

    sd_vh = vab_h[vabid].vh_addr_4;

    sd_int_flag = true;

    c = 0;
    for (pc = 0; pc < prog; pc++)
    {
        if (sd_vh->vab_prog[pc].tones != 0)
        {
            c++;
        }
    }

    sd_vab_prog = &sd_vh->vab_prog[prog];

    for (tone = 0; tone < sd_vab_prog->tones; tone++)
    {
        sd_vag_atr = &sd_vh->vag_atr[(c * 16) + tone];

        if (sd_vag_atr->vag == 0 || sd_vag_atr->min > note || sd_vag_atr->max < note)
        {
            continue;
        }

        vc = 0;

        while (SpuGetKeyStatus(spu_ch_tbl[vc]) != SPU_OFF)
        {
            if (++vc > (sd_reserved_voice - 1))
            {
                vc = -1;
                break;
            }
        }

        voice = vc << 16;
        if (vc == -1)
        {
            vc = 0;

            while (SmfGetPort(vc)->stat_16 != 0)
            {
                if (++vc > (sd_reserved_voice - 1))
                {
                    vc = -1;
                    break;
                }
            }

            voice = vc << 16;
        }

        if (vc != -1)
        {
            vc = SdUtKeyOnV((voice >> 16), vabid, prog, tone, note, pitch & 0xFF, voll, volr);
        }
    }

    sd_int_flag = false;
    return vc;
}

void SdVoKeyOff(s32 vab_pro, s32 pitch) // 0x800A0CFC
{
    s32 i;
    u32 voices = 0;

    sd_int_flag = true;

    for (i = 0; i < sd_reserved_voice; i++)
    {
        // Of the lower 16 bits of vab_pro, the upper 8 bits are used for VAB id, and the lower 8 bits specify a program number
        // Of the lower 16 bits of pitch, the upper 8 bits specify a key number in MIDI standard.
        // To specify a finer pitch, specify a key number in the lower 8 bits of pitch in 1/128 semitones.
        if (smf_port[i].stat_16 != 0 &&
            smf_port[i].midi_ch_3 == 0x20 &&
            smf_port[i].note_6 == (pitch >> 8) &&
            smf_port[i].vab_id_52 == (vab_pro >> 8) &&
            smf_port[i].prog_2 == (vab_pro & 0x7F))
        {
            voices |= spu_ch_tbl[i];

            smf_port[i].stat_16 = 0;
            smf_port[i].note_6  = 0;

            adsr_set(i, &smf_port[i]);
            SpuSetKey(SPU_OFF, spu_ch_tbl[i]);
            adsr_set(i, &smf_port[i]);
            SpuSetKey(SPU_OFF, spu_ch_tbl[i]);

            voices |= spu_ch_tbl[i];
        }
    }

    SpuSetKey(SPU_OFF, voices);

    sd_int_flag = false;
}

void SdVoKeyOffWithRROff(s32 vab_pro, s32 pitch) // 0x800A0E40
{
    s32 i;
    u32 voices = 0;

    sd_int_flag = true;

    for (i = 0; i < sd_reserved_voice; i++)
    {
        // Of the lower 16 bits of vab_pro, the upper 8 bits are used for VAB id, and the lower 8 bits specify a program number
        // Of the lower 16 bits of pitch, the upper 8 bits specify a key number in MIDI standard.
        // To specify a finer pitch, specify a key number in the lower 8 bits of pitch in 1/128 semitones.
        if (smf_port[i].stat_16 != 0 &&
            smf_port[i].midi_ch_3 == 32 &&
            smf_port[i].note_6 == (pitch >> 8) &&
            smf_port[i].vab_id_52 == (vab_pro >> 8) &&
            smf_port[i].prog_2 == (vab_pro & 0x7F))
        {
            voices |= spu_ch_tbl[i];

            smf_port[i].stat_16 = 0;
            smf_port[i].note_6  = 0;

            adsr_set(i, &smf_port[i]);
            rr_off(i);
            SpuSetKey(SPU_OFF, spu_ch_tbl[i]);
            SpuSetKey(SPU_OFF, spu_ch_tbl[i]);

            voices |= spu_ch_tbl[i];
        }
    }

    SpuSetKey(SPU_OFF, voices);

    sd_int_flag = false;
}

s16 SdUtKeyOnV(s16 voice, s16 vabid, s16 prog, s16 tone, s16 note, s16 fine, s16 voll, s16 volr) // 0x800A0F80
{
    SpuVoiceAttr s_attr;
    ProgAtr*     sd_vab_prog;
    SD_VAB_H*    sd_vh;
    VagAtr*      sd_vag_atr;
    s32          vo;
    s32          master_vol_temp;
    s32          stat;
    s32          i;
    s32          r_vol;
    s32          vag_offset;
    s32          l_vol;
    s32          pan;
    u16*         addr_p;
    u8           master_vol_byte;
    u16          adsr1_raw;
    s32          note_base;
    s32          volume_scale;
    s32          note_value;

    if (tone >= 0x10 || prog >= 0x80 || note >= 0x80)
    {
        return -1;
    }

    sd_int_flag = true;

    vo = voice;
    if ((sd_reserved_voice - 1) < vo)
    {
        vo = -1;
    }

    if (vo != -1)
    {
        sd_vh       = vab_h[vabid].vh_addr_4;
        sd_vab_prog = &sd_vh->vab_prog[prog];
        note_base   = note << 8;
        note_value  = note_base + fine;
        sd_vag_atr  = &sd_vh->vag_atr[(prog * 16) + tone];

        if (sd_vag_atr->vag == 0)
        {
            return -1;
        }

        if (note >= sd_vag_atr->min && sd_vag_atr->max >= note)
        {
            rr_off(vo);
            do
            {
                SpuSetKey(0, spu_ch_tbl[vo]);
                stat = SpuGetKeyStatus(spu_ch_tbl[vo]);
            }
            while (stat != 2 && stat != 0);

            addr_p = (u8*)vab_h[vabid].vh_addr_4 + (sd_vh->vab_h.ps << 9) + 0x820;

            for (vag_offset = 0, i = 0; i < sd_vag_atr->vag; i++)
            {
                vag_offset += *addr_p++;
            }

            s_attr.mask = 0x7019F;

            s_attr.volmode.left  = 0;
            s_attr.volmode.right = 0;
            vag_offset           = (vag_offset * 8);
            s_attr.voice         = spu_ch_tbl[vo];
            s_attr.addr          = vab_h[vabid].vb_start_addr_10 + vag_offset;

            s_attr.adsr1          = sd_vag_atr->adsr1;
            smf_port[vo].adsr1_46 = s_attr.adsr1;
            s_attr.adsr2          = sd_vag_atr->adsr2;
            smf_port[vo].adsr2_48 = s_attr.adsr2;

            adsr1_raw        = sd_vag_atr->adsr1;
            s_attr.a_mode    = !(adsr1_raw & 0x80) ? 1 : 5;
            s_attr.loop_addr = s_attr.addr;

            smf_port[vo].center_1E   = sd_vag_atr->center;
            smf_port[vo].shift_1F    = sd_vag_atr->shift;
            smf_port[vo].bend_min_1D = sd_vag_atr->pbmin;
            smf_port[vo].bend_max_1C = sd_vag_atr->pbmax;
            smf_port[vo].vc_0        = vo;
            smf_port[vo].prog_2      = prog;
            smf_port[vo].tone_4      = tone;
            smf_port[vo].note_6      = note_value >> 8;

            smf_port[vo].midi_ch_3 = 0x20;
            smf_port[vo].stat_16   = 1;
            smf_port[vo].pvol_10   = sd_vab_prog->mvol;
            smf_port[vo].ppan_12   = sd_vab_prog->mpan;
            smf_port[vo].tvol_11   = sd_vag_atr->vol;
            smf_port[vo].tpan_13   = sd_vag_atr->pan;
            smf_port[vo].vab_id_52 = vabid;

            pan = ((u8)vab_h[vabid].mpan_1B + smf_port[vo].ppan_12 + smf_port[vo].tpan_13) - 0x80;
            if (pan < 0)
            {
                pan = 0;
            }
            if (pan > 0x7F)
            {
                pan = 0x7F;
            }
            smf_port[vo].pan_14 = pan;

            master_vol_temp = (u8)vab_h[vabid].mvol_18;
            master_vol_temp = ((master_vol_temp * smf_port[vo].pvol_10) * smf_port[vo].tvol_11) >> 7;
            volume_scale    = 2;

            if (pan >= 0x40)
            {
                r_vol = master_vol_temp;
                l_vol = ((0x40 - (pan & 0x3F)) * (r_vol * volume_scale)) >> 7;
            }
            else
            {
                l_vol = master_vol_temp;
                r_vol = pan;
                r_vol = (r_vol * (l_vol * volume_scale)) >> 7;
            }
            master_vol_byte = vab_h[vabid].mvol_18;

            l_vol                = ((l_vol * master_vol_byte) >> 7);
            smf_port[vo].l_vol_C = (l_vol * voll) >> 7;

            r_vol                = ((r_vol * master_vol_byte) >> 7);
            smf_port[vo].r_vol_E = (r_vol * volr) >> 7;

            if (sd_mono_st_flag)
            {
                l_vol                = (smf_port[vo].l_vol_C + smf_port[vo].r_vol_E) >> 1;
                smf_port[vo].r_vol_E = l_vol;
                smf_port[vo].l_vol_C = l_vol;
            }

            s_attr.volume.left  = smf_port[vo].l_vol_C;
            s_attr.volume.right = smf_port[vo].r_vol_E;
            s_attr.pitch        = Note2Pitch(note, fine, sd_vag_atr->center, sd_vag_atr->shift);

            do
            {
                SpuSetKeyOnWithAttr(&s_attr);
            }
            while (SpuGetKeyStatus(spu_ch_tbl[vo] == 1) == 0);

            if (sd_vag_atr->mode & (1 << 2))
            {
                SpuSetReverbVoice(true, spu_ch_tbl[vo]);
            }
            else
            {
                SpuSetReverbVoice(false, spu_ch_tbl[vo]);
            }
        }
        else
        {
            vo = NO_VALUE;
        }
    }

    sd_int_flag = false;
    return vo;
}

s16 SdUtKeyOn(s16 vabid, s16 prog, s16 tone, s16 note, s16 fine, s16 voll, s16 volr) // 0x800A1534
{
    s32 vc;
    s32 voice;

    vc = 0;

    sd_int_flag = true;

    while (SpuGetKeyStatus(spu_ch_tbl[vc]) != SPU_OFF)
    {
        if (++vc > (sd_reserved_voice - 1))
        {
            vc = NO_VALUE;
            break;
        }
    }

    voice = vc << 0x10;
    if (vc == NO_VALUE)
    {
        vc = 0;

        while (SmfGetPort(vc)->stat_16 != 0)
        {
            if (++vc > (sd_reserved_voice - 1))
            {
                vc = NO_VALUE;
                break;
            }
        }

        voice = vc << 16;
    }

    if (vc != NO_VALUE)
    {
        vc = SdUtKeyOnV((voice >> 16), vabid, prog, tone, note, fine, voll, volr);
    }

    sd_int_flag = false;
    return vc;
}

s16 SdVbKeyOn(s16 vabid, s32 voice, s16 center, s16 shift, s16 note, s16 voll, s16 volr) // 0x800A16C0
{
    SpuVoiceAttr s_attr;
    s32          stat;

    sd_int_flag = true;

    if (vabid != NO_VALUE && voice < 24)
    {
        rr_off(voice);

        do
        {
            SpuSetKey(SPU_OFF, spu_ch_tbl[voice]);
            stat = SpuGetKeyStatus(spu_ch_tbl[voice]);
        }
        while (stat != SPU_OFF_ENV_ON && stat != SPU_OFF);

        s_attr.mask = SPU_VOICE_VOLL | SPU_VOICE_VOLR | SPU_VOICE_VOLMODEL | SPU_VOICE_VOLMODER |
                      SPU_VOICE_PITCH | SPU_VOICE_WDSA |
                      SPU_VOICE_ADSR_AMODE | SPU_VOICE_ADSR_SMODE | SPU_VOICE_ADSR_RMODE | SPU_VOICE_ADSR_AR |
                      SPU_VOICE_ADSR_DR | SPU_VOICE_ADSR_SR | SPU_VOICE_ADSR_RR | SPU_VOICE_ADSR_SL |
                      SPU_VOICE_LSAX;

        s_attr.a_mode        = 1;
        s_attr.s_mode        = 1;
        s_attr.r_mode        = 3;
        s_attr.ar            = 0;
        s_attr.dr            = 0;
        s_attr.sr            = 0;
        s_attr.rr            = 0;
        s_attr.sl            = 0;
        s_attr.volmode.left  = 0;
        s_attr.volmode.right = 0;
        s_attr.voice         = spu_ch_tbl[voice];
        s_attr.addr          = vab_h[vabid].vb_start_addr_10;

        smf_port[voice].vc_0      = voice;
        smf_port[voice].midi_ch_3 = 32;
        smf_port[voice].stat_16   = 1;
        smf_port[voice].center_1E = center;
        smf_port[voice].shift_1F  = shift;
        smf_port[voice].note_6    = note;
        smf_port[voice].pvol_10   = 127;
        smf_port[voice].ppan_12   = 64;
        smf_port[voice].tpan_13   = 64;
        smf_port[voice].tvol_11   = 127;
        smf_port[voice].vab_id_52 = vabid;
        smf_port[voice].l_vol_C   = voll << 7;
        smf_port[voice].r_vol_E   = volr << 7;

        s_attr.volume.left  = smf_port[voice].l_vol_C;
        s_attr.volume.right = smf_port[voice].r_vol_E;
        s_attr.pitch        = Note2Pitch(0, note, center, shift);

        do
        {
            SpuSetKeyOnWithAttr(&s_attr);
        }
        while (SpuGetKeyStatus(spu_ch_tbl[voice] == 1) == SPU_OFF);
    }

    sd_int_flag = false;
    return voice;
}

s32 SdUtKeyOffV(s16 vo) // 0x800A18F4
{
    s32 stat;
    u32 port;

    sd_int_flag = true;
    if (smf_port[vo].stat_16 != 0 && smf_port[vo].midi_ch_3 == 32)
    {
        port = spu_ch_tbl[vo];

        smf_port[vo].stat_16 = 0;
        smf_port[vo].note_6  = 0;

        adsr_set(vo, &smf_port[vo]);
        SpuSetKey(SPU_OFF, spu_ch_tbl[vo]);
        do
        {
            adsr_set(vo, &smf_port[vo]);
            SpuSetKey(SPU_OFF, port);
            stat = SpuGetKeyStatus(port);
        }
        while (stat != SPU_OFF_ENV_ON && stat != SPU_OFF);
    }
    else
    {
        sd_int_flag = false;
        return -1;
    }

    sd_int_flag = false;
    return 0;
}

s32 SdUtKeyOffVWithRROff(s16 vo) // 0x800A1A18
{
    s32 stat;
    u32 port;

    sd_int_flag = true;
    if (smf_port[vo].stat_16 != 0 && smf_port[vo].midi_ch_3 == 32)
    {
        port = spu_ch_tbl[vo];

        smf_port[vo].stat_16 = 0;
        smf_port[vo].note_6  = 0;

        adsr_set(vo, &smf_port[vo]);
        rr_off(vo);
        SpuSetKey(SPU_OFF, spu_ch_tbl[vo]);
        do
        {
            SpuSetKey(SPU_OFF, port);
            stat = SpuGetKeyStatus(port);
        }
        while (stat != SPU_OFF_ENV_ON && stat != SPU_OFF);
    }
    else
    {
        sd_int_flag = false;
        return -1;
    }

    sd_int_flag = false;
    return 0;
}

s16 SdGetSeqStatus(s16 access_num) // 0x800A1B14
{
    if (smf_song[access_num].sd_seq_stat_50A == SEQ_PLAY && midi_smf_stat(access_num) == SEQ_END)
    {
        smf_song[access_num].sd_seq_stat_50A = SEQ_END;
    }

    return smf_song[access_num].sd_seq_stat_50A;
}

s32 SdUtSetDetVVol(s16 vc, s16 voll, s16 volr) // 0x800A1BD0
{
    SpuVoiceAttr s_attr;

    s_attr.mask         = SPU_VOICE_VOLL | SPU_VOICE_VOLR;
    s_attr.voice        = spu_ch_tbl[vc];
    s_attr.volume.left  = voll;
    s_attr.volume.right = volr;

    SpuSetVoiceAttr(&s_attr);
    return 0;
}

s32 SdUtSetVVol(s16 vc, s16 voll, s16 volr) // 0x800A1C1C
{
    SpuVoiceAttr s_attr;

    s_attr.mask         = SPU_VOICE_VOLL | SPU_VOICE_VOLR;
    s_attr.voice        = spu_ch_tbl[vc];
    s_attr.volume.left  = (voll & 0x7F) << 7;
    s_attr.volume.right = (volr & 0x7F) << 7;

    SpuSetVoiceAttr(&s_attr);
    return 0;
}

s32 SdUtGetDetVVol(s16 vc, u16* voll, u16* volr) // 0x800A1C78
{
    SpuVoiceAttr s_attr;

    s_attr.mask  = 0;
    s_attr.voice = spu_ch_tbl[vc];

    SpuGetVoiceAttr(&s_attr);

    *voll = s_attr.volume.left;
    *volr = s_attr.volume.right;
    return 0;
}

s32 SdUtGetVVol(s16 vc, u16* voll, u16* volr) // 0x800A1CE8
{
    SpuVoiceAttr s_attr;

    s_attr.mask  = 0;
    s_attr.voice = spu_ch_tbl[vc];

    SpuGetVoiceAttr(&s_attr);

    *voll = s_attr.volume.left >> 7;
    *volr = s_attr.volume.right >> 7;
    return 0;
}

u16 SdGetTempo(s16 seq_access_num) // 0x800A1D68
{
    return smf_song[seq_access_num].tracks_0[0].mf_tempo2_16;
}

void SdSetTempo(s16 seq_access_num, s16 tempo) // 0x800A1DA4
{
    s32 a;

    for (a = 0; a < smf_song[seq_access_num].mf_tracks_526; a++)
    {
        smf_song[seq_access_num].tracks_0[a].mf_tempo2_16 = tempo;
        smf_song[seq_access_num].tracks_0[a].mf_tempo_14  = tempo;
    }
}

void SdSetSeqWide(s16 seq_access_num, u16 seq_wide) // 0x800A1E18
{
    smf_song[seq_access_num].seq_wide_flag_534 = seq_wide;
}

u8 SdGetMidiVol(s16 seq_access_num, s16 midi_ch) // 0x800A1E50
{
    return smf_midi[midi_ch + (seq_access_num * 16)].mvol_3;
}

void SdSetMidiVol(s16 seq_access_num, s16 midi_ch, s32 vol) // 0x800A1E90
{
    control_change(midi_ch + (seq_access_num * 16), 7, vol & 0x7F);
}

void SdSetMidiExpress(s16 seq_access_num, s16 midi_ch, s32 expression) // 0x800A1EC4
{
    control_change(midi_ch + (seq_access_num * 16), 11, expression & 0x7F);
}

u8 SdGetMidiExpress(s16 seq_access_num, s16 midi_ch) // 0x800A1EF8
{
    return smf_midi[midi_ch + (seq_access_num * 16)].express_5;
}

u8 SdGetMidiPan(s16 seq_access_num, s16 midi_ch) // 0x800A1F38
{
    return smf_midi[midi_ch + (seq_access_num * 16)].pan_1;
}

void SdSetMidiPan(s16 seq_access_num, s16 midi_ch, s32 pan) // 0x800A1F78
{
    control_change(midi_ch + (seq_access_num * 16), 10, pan & 0x7F);
}

u8 SdGetMidiPitchBendFine(s16 seq_access_num, s16 midi_ch) // 0x800A1FAC
{
    return smf_midi[midi_ch + (seq_access_num * 16)].pbend_7;
}

s32 SdSetMidiPitchBendFine(s16 seq_access_num, s16 midi_ch, u8 pitchBendFine) // 0x800A1FEC
{
    smf_midi[midi_ch + (seq_access_num * 16)].pbend_7 = pitchBendFine & 0x7F;
    return 0;
}

s32 SdGetTrackTranspause(void) // 0x800A2030
{
    return 0;
}

s32 SdSetTrackTranspause(void) // 0x800A2038
{
    return 0;
}

s32 SdGetTrackMute(s16 seq_access_num, s32 midi_ch) // 0x800A2040
{
    return smf_song[seq_access_num].sd_seq_track_mute_510 & spu_ch_tbl[midi_ch];
}

s32 SdSetTrackMute(s16 seq_access_num, s32 midi_ch) // 0x800A2090
{
    smf_song[seq_access_num].sd_seq_track_mute_510 |= spu_ch_tbl[midi_ch];
    return 0;
}

s32 SdGetSeqControlStatus(s16 seq_access_num) // 0x800A20EC
{
    if (seq_access_num >= 0)
    {
        return smf_song[seq_access_num].smf_control_stat_52F;
    }

    return -1;
}

s16 SdGetSeqPlayStatus(s32 access_num) // 0x800A2134
{
    if (smf_song[access_num].sd_seq_stat_50A == SEQ_PLAY && midi_smf_stat(access_num) == SEQ_END)
    {
        smf_song[access_num].sd_seq_stat_50A = SEQ_END;
    }

    return smf_song[access_num].sd_seq_stat_50A;
}

u32 SdGetSeqBeat(s16 seq_access_num) // 0x800A21E0
{
    if (seq_access_num >= 0)
    {
        if (smf_song[seq_access_num].mf_seq_beat_51C == 0)
        {
            return 0;
        }

        return smf_song[seq_access_num].mf_seq_beat_51C / 60;
    }

    return -1;
}

s32 SdGetSeqBeat2(s16 seq_access_num) // 0x800A224C
{
    if (seq_access_num >= 0)
    {
        return smf_song[seq_access_num].smf_beat_stat_52E;
    }

    return -1;
}

void SsSetMVol(s16 voll, s16 volr) // 0x800A2294
{
    SdSetMVol(voll, volr);
}

void SsEnd(void) // 0x800A22C0
{
    SdEnd();
}

void SsSetSerialAttr(char s_num, char attr, char mode) // 0x800A22E0
{
    SdSetSerialAttr(s_num, attr, mode);
}

void SsSetSerialVol(char s_num, s16 voll, s16 volr) // 0x800A2308
{
    SdSetSerialVol(s_num, voll, volr);
}

void SsUtAllKeyOff(s16 mode) // 0x800A2338
{
    SdUtAllKeyOff(mode);
}
