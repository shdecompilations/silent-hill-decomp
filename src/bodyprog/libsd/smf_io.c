#include "bodyprog/game.h"

#include <psyq/libspu.h>

extern SpuVoiceAttr s_attr; // @hack BSS

#include "bodyprog/bodyprog.h"
#include "bodyprog/libsd/libsd.h"

SpuVoiceAttr s_attr;
s32  __pad_bss_800C7730[2];
MIDI smf_midi[2 * 16];
MIDI smf_midi_sound_off;
s32  __pad_bss_800C8314;
PORT smf_port[24];
s32  sd_timer_event;

void set_note_on(s16 arg0, u8 arg1, u8 arg2, s16 arg3, s16 arg4) // 0x800A397C
{
    func_800485B0(arg0, arg1, arg2, arg3, arg4); // Nullsub.
}

void set_midi_info(s32 type, u8 midiChannel, u32 value) // 0x800A39B8
{
    func_800485B8(type, midiChannel, value); // Nullsub.
}

u16 Note2Pitch(s16 note, s16 cent, s16 sample_note, s16 sample_cent) // 0x800A39D8
{
    s16 total_cent;
    s16 cent_offset;
    s16 note_diff;
    s32 pitch_steps;
    s16 final_note;
    s32 temp;
    s16 abs_diff;

    total_cent = cent + sample_cent;

    pitch_steps = (total_cent < 0) ? (total_cent + 127) : total_cent;

    pitch_steps = (pitch_steps >> 7);
    final_note  = note + pitch_steps;

    temp        = total_cent;
    cent_offset = temp - (pitch_steps << 7);
    note_diff   = final_note - sample_note;

    if (note_diff >= 0)
    {
        return PitchTbl[note_diff % 12][cent_offset] << (note_diff / 12);
    }
    else
    {
        abs_diff = -note_diff;
        return PitchTbl[(12 - (abs_diff % 12)) % 12][cent_offset] >> ((abs_diff + 11) / 12);
    }
}

void tre_calc(PORT* p) // 0x800A3B20
{
    s32 vol;
    s32 tre_data;
    s8  tre_char;

    if (p->tredm_36 == 0)
    {
        return;
    }

    if (p->trehc_32 != p->trehs_39)
    {
        p->trehc_32++;
    }
    else
    {
        if (p->trecc_38 >= p->trecs_3A)
        {
            p->tred_34 = p->tredm_36;
        }
        else
        {
            if (p->trecc_38 != 0)
            {
                p->tred_34 += p->tread_3C;
            }
            else
            {
                p->tred_34 = p->tread_3C;
            }
            p->trecc_38++;
        }

        p->trec_33 += p->trecad_3B;
        p->trecadw2_31 = 0;

        if (p->trec_33 < 0)
        {
            tre_char = -p->trec_33 * 2;
            if (tre_char < 0)
            {
                tre_char = -tre_char;
            }

            tre_data = p->tred_34 * tre_char;
            if (tre_data > 0)
            {
                tre_data = -tre_data;
            }
        }
        else
        {
            tre_char = p->trec_33 * 2;
            if (tre_char < 0)
            {
                tre_char = -tre_char;
            }

            tre_data = p->tred_34 * tre_char;
            if (tre_data < 0)
            {
                tre_data = -tre_data;
            }
        }

        if (tre_data != 0)
        {
            p->tre_data_3E = tre_data / 0x100;
        }
        else
        {
            p->tre_data_3E = 0;
        }
    }

    if (p->tremoro_wk_4C == p->tre_data_3E)
    {
        return;
    }

    p->tremoro_wk_4C = p->tre_data_3E;

    s_attr.mask  = SPU_VOICE_VOLL | SPU_VOICE_VOLR | SPU_VOICE_VOLMODEL | SPU_VOICE_VOLMODER;
    s_attr.voice = spu_ch_tbl[p->vc_0];

    vol = p->l_vol_C + p->tre_data_3E;
    if (vol < 0)
    {
        vol = 0;
    }
    else if (vol > 0x3FFF)
    {
        vol = 0x3FFF;
    }
    s_attr.volume.left = vol;

    vol = p->r_vol_E + p->tre_data_3E;
    if (vol < 0)
    {
        vol = 0;
    }
    else if (vol > 0x3FFF)
    {
        vol = 0x3FFF;
    }
    s_attr.volume.right = vol;

    s_attr.volmode.left  = 0;
    s_attr.volmode.right = 0;
    SpuSetVoiceAttr(&s_attr);
}

void vib_calc(PORT* p) // 0x800A3D30
{
    s32 vib_data;
    s8  vib_char;

    if (p->vibdm_26 != 0)
    {
        if (p->vibhc_22 != p->vibhs_29)
        {
            p->vibhc_22++;
            return;
        }

        if (p->vibcc_28 >= p->vibcs_2A)
        {
            p->vibd_24 = p->vibdm_26;
        }
        else
        {
            if (p->vibcc_28 != 0)
            {
                p->vibd_24 += p->vibad_2C;
            }
            else
            {
                p->vibd_24 = p->vibad_2C;
            }
            p->vibcc_28++;
        }

        p->vibc_23 += p->vibcad_2B;
        p->vibcadw2_21 = 0;

        if (p->vibc_23 < 0)
        {
            vib_char = -p->vibc_23 * 2;
            if (vib_char < 0)
            {
                vib_char = -vib_char;
            }

            vib_data = p->vibd_24 * vib_char;
            if (vib_data > 0)
            {
                vib_data = -vib_data;
            }
        }
        else
        {
            vib_char = p->vibc_23 * 2;
            if (vib_char < 0)
            {
                vib_char = -vib_char;
            }

            vib_data = p->vibd_24 * vib_char;
            if (vib_data < 0)
            {
                vib_data = -vib_data;
            }
        }

        if (vib_data != 0)
        {
            p->vib_data_2E = vib_data / 0x400;
            return;
        }

        p->vib_data_2E = 0;
    }
}

void random_calc(PORT* p) // 0x800A3E70
{
    extern u8 random_tbl[];

    s8 c_data;

    if (!p->rdms_43)
    {
        return;
    }

    p->rdmc_44 += p->rdms_43;

    if (p->rdmc_44 < 0x7F && p->rdms_43 != 127)
    {
        return;
    }

    p->rdmc_44 = p->rdmc_44 & 0x7F;

    c_data = random_tbl[p->rdmo_42];
    p->rdmo_42++;

    if (c_data)
    {
        p->rdmd_40 = (c_data * p->rdmdm_45) / 4;
    }
    else
    {
        p->rdmd_40 = 0;
    }
}

void volume_calc(PORT* p, MIDI* mp) // 0x800A3F14
{
    s32 sd_seq_play_no;
    s32 r_vol;
    s32 pan;
    s32 l_vol;
    s32 scale;

    sd_seq_play_no = smf_song[p->midi_ch_3 >> 4].sd_seq_vab_id_508;
    pan            = 0x40;

    if (!((u8)sd_mono_st_flag))
    {
        pan = ((u8)vab_h[sd_seq_play_no].mpan_1B + p->ppan_12 + p->tpan_13 + mp->pan_1) - 0xC0; // `mpan_1B` `s8` to `u8`.
        if (mp->key_pan_22 != 0)
        {
            s32 temp = ((s32)(p->note_6 * mp->key_pan_22) >> 6) + 0x40;
            pan      = (pan + (temp - mp->key_pan_22)) - 0x40;
        }
    }

    if (pan < 0)
    {
        pan = 0;
    }
    if (pan > 0x7F)
    {
        pan = 0x7F;
    }

    p->pan_14 = pan;

    // `mvol_18` `s8` to `u8`.
    l_vol = ((u8)vab_h[sd_seq_play_no].mvol_18 * smf_song[p->midi_ch_3 >> 4].midi_master_vol_538 * mp->express_5 * mp->mvol_3) >> 14;
    l_vol = l_vol * p->pvol_10;
    l_vol = (l_vol * p->tvol_11) >> 14;

    scale = 2;

    if (pan >= 0x40)
    {
        if (pan == 0x40)
        {
            pan   = l_vol;
            r_vol = l_vol;
        }
        else
        {
            r_vol = l_vol;
            l_vol = ((0x40 - (pan & 0x3F)) * (l_vol * scale)) >> 7;
        }
    }
    else
    {
        r_vol = (pan * (l_vol * scale)) >> 7;
    }

    p->l_vol_C = (l_vol * (p->velo_1A & 0x7F)) >> 7;
    p->r_vol_E = (r_vol * (p->velo_1A & 0x7F)) >> 7;

    if (mp->vol_mode_11 >= 0x40u)
    {
        p->l_vol_C = (p->l_vol_C * p->l_vol_C) >> 14;
        p->r_vol_E = (p->r_vol_E * p->r_vol_E) >> 14;
    }
}

void smf_vol_set(s32 ch, s32 vc, s32 l_vol, s32 r_vol) // 0x800A4150
{
    s_attr.mask  = SPU_VOICE_VOLL | SPU_VOICE_VOLR | SPU_VOICE_VOLMODEL | SPU_VOICE_VOLMODER;
    s_attr.voice = spu_ch_tbl[vc];

    if (l_vol | r_vol)
    {
        s_attr.volume.left  = (l_vol * smf_song[ch >> 4].sd_seq_mvoll_50C) >> 7;
        s_attr.volume.right = (r_vol * smf_song[ch >> 4].sd_seq_mvolr_50E) >> 7;
        if (smf_midi[ch].wide_flag_21 || smf_song[ch >> 4].seq_wide_flag_534)
        {
            s_attr.volume.right = -s_attr.volume.right;
        }
    }
    else
    {
        s_attr.volume.right = 0;
        s_attr.volume.left  = 0;
    }

    s_attr.volmode.left  = 0;
    s_attr.volmode.right = 0;

    SpuSetVoiceAttr(&s_attr);
}

void master_vol_set(void) // 0x800A4260
{
    s32 i;

    sd_int_flag = true;

    for (i = 0; i < sd_reserved_voice; i++)
    {
        volume_calc(&smf_port[i], &smf_midi[smf_port[i].midi_ch_3]);
        smf_vol_set(smf_port[i].midi_ch_3, i, smf_port[i].l_vol_C, smf_port[i].r_vol_E);
    }

    sd_int_flag = false;
}

void seq_master_vol_set(s32 access_num) // 0x800A4314
{
    s32 vc;

    for (vc = 0; vc < sd_reserved_voice; vc++)
    {
        if ((smf_port[vc].midi_ch_3 >> 4) == access_num)
        {
            smf_vol_set(smf_port[vc].midi_ch_3, vc, smf_port[vc].l_vol_C, smf_port[vc].r_vol_E);
        }
    }
}

void toremoro_set(void) // 0x800A439C
{
    s16 tre_data;
    s32 vc;

    for (vc = 0; vc < sd_reserved_voice; vc++)
    {
        tre_data = smf_port[vc].tre_data_3E;
        if (smf_port[vc].tremoro_wk_4C != tre_data)
        {
            smf_vol_set(smf_port[vc].midi_ch_3, vc, smf_port[vc].l_vol_C + tre_data, smf_port[vc].r_vol_E + tre_data);
        }
    }
}

s32 pitch_bend_calc(PORT* p, u32 pit) // 0x800A441C
{
    u16 bendMultiplier = 2; // @hack Needed to get regalloc order correct, not included in PS2 `AUDIO.IRX` syms.
    s16 pitch;

    if (pit < 0x40u)
    {
        if (!p->bend_min_1D)
        {
            return 0;
        }
        else
        {
            pitch = -((p->bend_min_1D * bendMultiplier) * (0x40 - pit));
        }
    }
    else if (pit == 0x40)
    {
        return 0;
    }
    else
    {
        if (!p->bend_max_1C)
        {
            return 0;
        }
        else
        {
            pitch = (short)(p->bend_max_1C * bendMultiplier) * (pit - 0x3F);
        }
    }

    return pitch;
}

void pitch_calc(PORT* p, s32 forceSpuUpdate) // 0x800A4494
{
    MIDI* m;
    s16   pitch;
    s32   note;

    m = &smf_midi[p->midi_ch_3];
    if (p->vibdm_26 != 0 || m->mod_2 != 0 || m->porta_28 != 0 || p->rdms_43 != 0 || m->pbend_7 != p->pbend_wk_4E || forceSpuUpdate)
    {
        p->pbend_wk_4E = m->pbend_7;

        pitch = p->rdmd_40 + (p->vib_data_2E + ((u16)m->mod_depth_1C + (u16)m->porta_depth_2A));
        pitch += (p->note_wk_8 << 7) + pitch_bend_calc(p, m->pbend_7);
        note = pitch << 16;

        s_attr.mask  = SPU_VOICE_PITCH;
        s_attr.voice = spu_ch_tbl[p->vc_0];
        s_attr.pitch = Note2Pitch(note >> 23, pitch & 0x7F, p->center_1E, p->shift_1F);
        SpuSetVoiceAttr(&s_attr);
    }

    if (p->vibdm_26 != 0 && SpuGetKeyStatus(spu_ch_tbl[p->vc_0]) == SPU_OFF)
    {
        p->vibdm_26    = 0;
        p->vib_data_2E = 0;
    }
}

void midi_mod(MIDI* p) // 0x800A4608
{
    if (p->mod_2 != 0)
    {
        if (p->mod_w_14 != 0)
        {
            if ((p->mod_depth_1C + p->mod_add_1A) < p->mod_limit_1E)
            {
                p->mod_depth_1C += p->mod_add_1A;
            }
            else
            {
                p->mod_w_14     = 0;
                p->mod_depth_1C = p->mod_limit_1E;
            }
        }
        else if ((p->mod_depth_1C - p->mod_add_1A) > -p->mod_limit_1E)
        {
            p->mod_depth_1C -= p->mod_add_1A;
        }
        else
        {
            p->mod_w_14     = 1;
            p->mod_depth_1C = -p->mod_limit_1E;
        }
    }
    else
    {
        p->mod_depth_1C = 0;
    }
}

void midi_porta(MIDI* p) // 0x800A46B8
{
    if (p->porta_28 != 0)
    {
        if (p->porta_wk_30 != 0)
        {
            if ((p->porta_depth_2A + p->porta_add_2C) < p->porta_limit_2E)
            {
                p->porta_depth_2A += p->porta_add_2C;
            }
            else
            {
                p->porta_depth_2A = p->porta_limit_2E;
            }
        }
        else
        {
            if ((p->porta_add_2C - p->porta_depth_2A) < p->porta_limit_2E)
            {
                p->porta_depth_2A -= p->porta_add_2C;
            }
            else
            {
                p->porta_depth_2A = -p->porta_limit_2E;
            }
        }
    }
}

void replay_reverb_set(s16 acc) // 0x800A4748
{
    SpuReverbAttr reverb;

    if (smf_song[acc].seq_rev_set_flag_532 != 0)
    {
        if (smf_song[acc].seq_rev_set_flag_532 & (1 << 0))
        {
            reverb.mask = SPU_REV_DEPTHL | SPU_REV_DEPTHR;
            if (smf_song[acc].seq_rev_set_flag_532 << 8 < smf_song[acc].seq_reverb_depth_536)
            {
                reverb.depth.left = reverb.depth.right = smf_song[acc].seq_rev_set_flag_532 << 8;
            }
            else
            {
                reverb.depth.left = reverb.depth.right = smf_song[acc].seq_reverb_depth_536;
                smf_song[acc].seq_rev_set_flag_532     = 0;
                return; // @bug Returns before setting `seq_reverb_depth_536` as reverb? Was fixed in later PS2 `soundcd.irx`.
            }

            SpuSetReverbModeParam(&reverb);
            SpuSetReverb(1);
        }

        smf_song[acc].seq_rev_set_flag_532 += 2;
    }
}

void midi_vsync(void) // 0x800A4838
{
    s32 i;
    s32 ch;

    for (i = 0; i < 2; i++)
    {
        if (smf_song[i].sd_seq_stat_50A == SEQ_PLAY)
        {
            for (ch = 0; ch < 16; ch++)
            {
                midi_mod(&smf_midi[ch + (i * 16)]);
                midi_porta(&smf_midi[ch + (i * 16)]);
            }
        }
    }

    for (ch = 0; ch < sd_reserved_voice; ch++)
    {
        u8 midi_num = smf_port[ch].midi_ch_3;
        if (midi_num < 32 && smf_song[midi_num >> 4].sd_seq_stat_50A == SEQ_PLAY)
        {
            vib_calc(&smf_port[ch]);
            tre_calc(&smf_port[ch]);
            random_calc(&smf_port[ch]);
            pitch_calc(&smf_port[ch], 0);

            if (smf_song[(u8)smf_port[ch].midi_ch_3 >> 4].seq_vol_set_flag_530 != 0)
            {
                smf_vol_set(smf_port[ch].midi_ch_3, ch, smf_port[ch].l_vol_C, smf_port[ch].r_vol_E);
            }
        }
    }

    for (i = 0; i < 2; i++)
    {
        if (smf_song[i].sd_seq_stat_50A == SEQ_PLAY)
        {
            replay_reverb_set(i);
            if (smf_song[i].seq_vol_set_flag_530 != 0)
            {
                smf_song[i].seq_vol_set_flag_530 = 0;
            }
        }
    }
}

void sound_seq_off(s32 access_num) // 0x800A4A34
{
    MIDI* m;
    s32   vo;
    u32   voice;
    s32   stat;
    u32   chan;

    voice = 0;

    smf_song[access_num].midi_master_vol_538  = 127;
    smf_song[access_num].seq_vol_set_flag_530 = 0;
    smf_song[access_num].seq_rev_set_flag_532 = 0;
    smf_song[access_num].seq_reverb_depth_536 = 0;

    for (vo = 0; vo < 32; vo++)
    {
        SMF* track = &smf_song[access_num].tracks_0[vo];

        track->mf_eof_flag_20         = 1;
        track->running_status_flag_24 = 0xFF;
        track->mf_data_loc_0          = 0;
        track->mf_loop_point_4        = 0;
        track->mf_loop_count_22       = 0;
        track->mf_delta_time_1C       = 0;
        track->mf_track_length_8      = 0;
        track->mf_track_size_C        = 0;
        track->mf_tempo_14            = 114;
        track->mf_tempo2_16           = 114;
        track->mf_repeat_ptr_10       = 0;
        track->time_hosei_18          = 0;
        track->ti_flag_23             = 0;
        track->status_value_25        = 0;
        track->midi_ch_27             = access_num * 0x10;
    }

    for (vo = 0; vo < sd_reserved_voice; vo++)
    {
        PORT* port = &smf_port[vo];

        if ((port->midi_ch_3 >> 4) != access_num)
        {
            continue;
        }

        smf_vol_set(0, vo, 0, 0);

        port->vc_0        = vo;
        port->stat_16     = 0;
        port->pan_14      = 64;
        port->pedal_1B    = 0;
        port->vibc_23     = 0;
        port->vibhc_22    = 0;
        port->vibcc_28    = 0;
        port->vibd_24     = 0;
        port->vibdm_26    = 0;
        port->vib_data_2E = 0;
        port->trec_33     = 0;
        port->trehc_32    = 0;
        port->trecc_38    = 0;
        port->tred_34     = 0;
        port->tredm_36    = 0;
        port->tre_data_3E = 0;
        port->rdmd_40     = 0;
        port->rdmo_42     = 0;
        port->rdms_43     = 0;
        port->rdmc_44     = 0;
        port->rdmdm_45    = 0;
        port->pbend_wk_4E = 64;
        port->pbend_50    = 64;

        chan = spu_ch_tbl[vo];
        voice |= chan;

        rr_off(vo);

        do
        {
            SpuSetKey(SPU_OFF, chan);
            stat = SpuGetKeyStatus(chan);
        }
        while (stat != SPU_OFF_ENV_ON && stat != SPU_OFF);
    }

    for (vo = 0; vo < 16; vo++)
    {
        m = &smf_midi[(access_num * 16) + vo];

        m->mvol_3         = 127;
        m->before_note_13 = 60;
        m->pan_1          = 64;
        m->pedal_6        = 0;
        m->pbend_7        = 64;
        m->express_5      = 127;
        m->l_vol_8        = 127;
        m->r_vol_C        = 127;
        m->mod_2          = 0;
        m->mod_depth_1C   = 0;
        m->mod_speed_18   = 0;
        m->mod_limit_1E   = 0;
        m->mod_mode_20    = 0;
        m->pitch_32       = 0;
        m->rev_depth_24   = 0;
        m->wide_flag_21   = 0;
        m->vibc_37        = 0;
        m->vibhc_36       = 0;
        m->vibcc_38       = 0;
        m->vibd_3C        = 0;
        m->vibdm_3E       = 0;
        m->trec_47        = 0;
        m->trehc_46       = 0;
        m->trecc_50       = 0;
        m->tred_48        = 0;
        m->tredm_4A       = 0;
        m->rdmd_54        = 0;
        m->rdmo_56        = 0;
        m->rdms_57        = 0;
        m->rdmc_58        = 0;
        m->rdmdm_59       = 0;
        m->bend_mode_10   = 0;
        m->vol_mode_11    = 0;
        m->porta_28       = 0;
        m->mode_12        = 0;
        m->key_pan_22     = 0;
        m->bank_change_5A = 127;
    }

    SpuSetKey(SPU_OFF, voice);
}

void sound_off(void) // 0x800A4D20
{
    u32 voice;
    s32 stat;
    s32 vo;
    u32 voices;

    voices = 0;

    for (vo = 0; vo < 2; vo++)
    {
        sound_seq_off(vo);
    }

    for (vo = 0; vo < sd_reserved_voice; vo++)
    {
        smf_vol_set(0, vo, 0, 0);
        smf_port[vo].vc_0        = vo;
        smf_port[vo].stat_16     = 0;
        smf_port[vo].pan_14      = 0x40;
        smf_port[vo].pbend_wk_4E = 0x40;
        smf_port[vo].pbend_50    = 0x40;

        voice = spu_ch_tbl[vo];
        voices |= voice;
        rr_off(vo);

        do
        {
            SpuSetKey(SPU_OFF, voice);
            stat = SpuGetKeyStatus(voice);
        }
        while (stat != SPU_OFF_ENV_ON && stat != SPU_OFF);
    }

    smf_midi_sound_off.mvol_3    = 0x7F;
    smf_midi_sound_off.pan_1     = 0x40;
    smf_midi_sound_off.pbend_7   = 0x40;
    smf_midi_sound_off.pedal_6   = 0;
    smf_midi_sound_off.express_5 = 0x7F;
    smf_midi_sound_off.l_vol_8   = 0x7F;
    smf_midi_sound_off.r_vol_C   = 0x7F;
    smf_midi_sound_off.mod_2     = 0;
    smf_midi_sound_off.pitch_32  = 0;
    smf_midi_sound_off.porta_28  = 0;

    do
    {
        SpuSetKey(SPU_OFF, voices);
        stat = SpuGetKeyStatus(voices);
    }
    while (stat != SPU_OFF_ENV_ON && stat != SPU_OFF);
}

void vol_calc(void) {} // 0x800A4E90

void adsr_set(s32 vo, PORT* p) // 0x800A4E98
{
    s_attr.mask   = SPU_VOICE_ADSR_AMODE | SPU_VOICE_ADSR_ADSR1 | SPU_VOICE_ADSR_ADSR2;
    s_attr.voice  = spu_ch_tbl[vo];
    s_attr.adsr1  = p->adsr1_46;
    s_attr.adsr2  = p->adsr2_48;
    s_attr.a_mode = p->a_mode_4A;
    SpuSetVoiceAttr(&s_attr);
}

void rr_off(s32 vo) // 0x800A4F08
{
    s_attr.mask   = SPU_VOICE_ADSR_AMODE | SPU_VOICE_ADSR_ADSR1 | SPU_VOICE_ADSR_ADSR2;
    s_attr.voice  = spu_ch_tbl[vo];
    s_attr.adsr1  = 0;
    s_attr.adsr2  = 0;
    s_attr.a_mode = 5;
    SpuSetVoiceAttr(&s_attr);
}

s16 voice_check(s32 chan, s32 note, bool flag) // 0x800A4F64
{
    s32 stat;
    s16 vo;

    vo = 0;
    do
    {
        if (SpuGetKeyStatus(spu_ch_tbl[vo]) == SPU_OFF)
        {
            return vo;
        }
        vo++;
    }
    while (vo < sd_reserved_voice);

    vo = 0;
    do
    {
        stat = SpuGetKeyStatus(spu_ch_tbl[vo]);
        if (stat == SPU_OFF_ENV_ON || stat == SPU_OFF)
        {
            return vo;
        }
        vo++;
    }
    while (vo < sd_reserved_voice);

    if (flag)
    {
        return NO_VALUE;
    }

    vo = 0;
    do
    {
        if (chan == smf_port[vo].midi_ch_3 && smf_port[vo].note_6 == note)
        {
            return vo;
        }
        vo++;
    }
    while (vo < sd_reserved_voice);

    vo = 0;
    do
    {
        if (smf_port[vo].stat_16 == 0)
        {
            return vo;
        }
        vo++;
    }
    while (vo < sd_reserved_voice);

    // @hack Dead code needed for match from permuter.
    if (stat && vo)
    {
    }

    return NO_VALUE;
}

void key_on(u8 chan, u8 c1, u8 c2) // 0x800A5158
{
    u16       tone;
    s32       sp28;
    u16       progNo;
    SD_VAB_H* vab1;
    SD_VAB_H* vab0;
    ProgAtr*  prog;
    s32       vabId;
    s32       sp48;
    MIDI*     midi;
    PORT*     port;
    VagAtr*   sd_vag_atr;
    s16       vo;
    s16       temp_s0_5;
    s32       stat;
    s32       i;
    u32       vag_addr;
    u16       note;
    u16*      addr_p;

    midi   = &smf_midi[chan];
    progNo = midi->prog_no_0;

    if (midi->bank_change_5A > 16)
    {
        vabId = smf_song[chan >> 4].sd_seq_vab_id_508;
    }
    else
    {
        vabId = midi->bank_change_5A;
    }

    vab0 = vab_h[vabId].vh_addr_4;
    vab1 = vab_h[vabId].vh_addr_4;
    sp28 = 0;

    for (i = 0; i < progNo; i++)
    {
        if (vab0->vab_prog[i].tones != 0)
        {
            sp28++;
        }
    }
    prog = &vab1->vab_prog[progNo];
    note = c1;

    for (tone = 0; tone < prog->tones; tone++)
    {
        sd_vag_atr = &vab1->vag_atr[(sp28 * 16) + tone];
        sp48       = 0;

        if (sd_vag_atr->vag != 0 && c1 >= sd_vag_atr->min && sd_vag_atr->max >= c1)
        {
            vo = NO_VALUE;
            if (midi->mode_12 != 0)
            {
                vo = 0;
                while (true)
                {
                    if (chan == smf_port[vo].midi_ch_3)
                    {
                        break;
                    }

                    vo++;

                    if (vo >= sd_reserved_voice)
                    {
                        vo = NO_VALUE;
                        break;
                    }
                }

                if (vo != NO_VALUE)
                {
                    if (smf_port[vo].stat_16 == 0)
                    {
                        rr_off(vo);
                        do
                        {
                            SpuSetKey(0, spu_ch_tbl[vo]);
                            stat = SpuGetKeyStatus(spu_ch_tbl[vo]);
                        }
                        while (stat != 2 && stat != 0);
                    }
                    else
                    {
                        sp48 = 1;
                    }
                }
                else
                {
                    vo = voice_check(chan, c1, true);
                }

                if (midi->porta_28 != 0)
                {
                    note              = (midi->before_note_13 & 0x7F);
                    midi->porta_depth_2A = 0;

                    if (midi->before_note_13 < c1)
                    {
                        midi->porta_wk_30    = 1;
                        midi->porta_limit_2E = (c1 - midi->before_note_13) << 7;
                        midi->porta_add_2C   = (midi->porta_limit_2E * 4) / midi->porta_28;
                    }
                    else if (midi->before_note_13 == c1)
                    {
                        midi->porta_add_2C = 0;
                        note            = c1;
                    }
                    else
                    {
                        midi->porta_wk_30    = 0;
                        midi->porta_limit_2E = (midi->before_note_13 - c1) << 7;
                        midi->porta_add_2C   = (midi->porta_limit_2E * 4) / midi->porta_28;
                    }
                }
                else
                {
                    midi->porta_depth_2A = 0;
                }
            }
            else
            {
                if (midi->porta_28 != 0)
                {
                    note              = midi->before_note_13 & 0x7F;
                    midi->porta_depth_2A = 0;

                    if (midi->before_note_13 < c1)
                    {
                        midi->porta_wk_30    = 1;
                        midi->porta_limit_2E = (c1 - midi->before_note_13) << 7;
                        midi->porta_add_2C   = ((midi->porta_limit_2E * 4) / midi->porta_28);
                    }
                    else if (midi->before_note_13 == c1)
                    {
                        midi->porta_add_2C = 0;
                        note            = c1;
                    }
                    else
                    {
                        midi->porta_wk_30    = 0;
                        midi->porta_limit_2E = (midi->before_note_13 - c1) << 7;
                        midi->porta_add_2C   = (midi->porta_limit_2E * 4) / midi->porta_28;
                    }
                }
                else
                {
                    midi->porta_depth_2A = 0;
                }
            }

            if (vo == NO_VALUE)
            {
                vo = voice_check(chan, c1, false);
                if (vo == NO_VALUE)
                {
                    continue;
                }
            }

            port = &smf_port[vo];
            if (midi->mode_12 == 0)
            {
                rr_off(vo);
                do
                {
                    SpuSetKey(0, spu_ch_tbl[vo]);
                    stat = SpuGetKeyStatus(spu_ch_tbl[vo]);
                }
                while (stat != 2 && stat != 0);
            }

            for (i = 0, vag_addr = 0, addr_p = (u8*)vab_h[vabId].vh_addr_4 + (vab0->vab_h.ps * 512) + 2080;
                i < sd_vag_atr->vag;
                addr_p++, i++)
            {
                vag_addr += *addr_p;
            }

            s_attr.mask          = 0x6019F;
            s_attr.volmode.left  = 0;
            s_attr.volmode.right = 0;
            s_attr.voice         = spu_ch_tbl[vo];
            vag_addr             = vag_addr * 8;
            s_attr.addr          = vab_h[vabId].vb_start_addr_10 + vag_addr;
            s_attr.adsr1         = sd_vag_atr->adsr1;
            port->adsr1_46          = s_attr.adsr1;
            s_attr.adsr2         = sd_vag_atr->adsr2;
            port->adsr2_48          = s_attr.adsr2;

            if (!(sd_vag_atr->adsr1 & 0x80))
            {
                s_attr.a_mode = 1;
            }
            else
            {
                s_attr.a_mode = 5;
            }

            port->a_mode_4A      = s_attr.a_mode;
            midi->before_note_13 = c1;
            midi->velo_4         = c2;
            port->center_1E      = sd_vag_atr->center;
            port->shift_1F       = sd_vag_atr->shift;
            port->bend_min_1D    = sd_vag_atr->pbmin;
            port->bend_max_1C    = sd_vag_atr->pbmax;
            port->vc_0           = vo;
            port->prog_2         = progNo;
            port->tone_4         = tone;
            port->note_6         = note & 0x7F;
            port->note_wk_8      = note & 0x7F;
            port->midi_ch_3      = chan;
            port->stat_16        = 1;
            midi->mod_depth_1C   = 0;
            port->pvol_10        = prog->mvol;
            port->ppan_12        = prog->mpan;
            port->tvol_11        = sd_vag_atr->vol;
            port->tpan_13        = sd_vag_atr->pan;
            port->velo_1A        = c2;
            port->vibdm_26       = 0;

            if (midi->vibdm_3E != 0)
            {
                port->vibc_23    = 0;
                port->vibhc_22   = 0;
                port->vibcc_28   = 0;
                port->vibhs_29   = midi->vibhs_39;
                port->vibcad_2B  = midi->vibcad_3B;
                port->vibad_2C   = midi->vibad_40;
                port->vibdm_26   = midi->vibdm_3E;
                port->vibcs_2A   = midi->vibcs_3A;
                port->vibcadw_20 = midi->vibcadw_34;
            }

            port->vibd_24     = 0;
            port->vib_data_2E = 0;
            port->tredm_36    = 0;

            if (midi->tredm_4A != 0)
            {
                port->trec_33    = 0;
                port->trehc_32   = 0;
                port->trecc_38   = 0;
                port->trehs_39   = midi->trehs_51;
                port->trecad_3B  = midi->trecad_53;
                port->tread_3C   = midi->tread_4C;
                port->tredm_36   = midi->tredm_4A;
                port->trecs_3A   = midi->trecs_52;
                port->trecadw_30 = midi->trecadw_44;
            }

            port->tred_34     = 0;
            port->tre_data_3E = 0;
            port->rdmdm_45    = 0;

            if (midi->rdms_57 != 0)
            {
                port->rdmd_40  = midi->rdmd_54;
                port->rdms_43  = midi->rdms_57;
                port->rdmc_44  = midi->rdmc_58;
                port->rdmdm_45 = midi->rdmdm_59;
            }

            volume_calc(&smf_port[vo], &smf_midi[chan]);

            s_attr.volume.left  = (port->l_vol_C * smf_song[chan >> 4].sd_seq_mvoll_50C) >> 7;
            s_attr.volume.right = (port->r_vol_E * smf_song[chan >> 4].sd_seq_mvolr_50E) >> 7;

            if (midi->wide_flag_21 != 0 || smf_song[chan >> 4].seq_wide_flag_534 != 0)
            {
                s_attr.volume.right = -s_attr.volume.right;
            }

            port->pedal_1B    = midi->pedal_6;
            port->note_6      = c1 & 0x7F;
            port->pbend_50    = (u16)midi->pbend_7;
            port->pbend_wk_4E = 0xFFFF;

            if (midi->bend_mode_10 < 0x40u)
            {
                temp_s0_5 = port->vib_data_2E + (midi->mod_depth_1C + midi->porta_depth_2A);
                temp_s0_5 += (port->note_wk_8 << 7) + pitch_bend_calc(port, midi->pbend_7);
                s_attr.pitch = Note2Pitch(temp_s0_5 >> 7, temp_s0_5 & 0x7F, sd_vag_atr->center, sd_vag_atr->shift);
            }
            else
            {
                midi->pbend_7   = 0x40;
                s_attr.pitch = Note2Pitch(note, 0, sd_vag_atr->center, sd_vag_atr->shift);
            }

            set_note_on(vo, c1, chan, s_attr.volume.left, s_attr.volume.right);

            if (sp48 == 0 && (s_attr.volume.right | s_attr.volume.left) != 0)
            {
                do
                {
                    SpuSetKeyOnWithAttr(&s_attr);
                }
                while (SpuGetKeyStatus(spu_ch_tbl[vo] == 1) == 0);
            }

            if (midi->rev_depth_24 == 0)
            {
                if (sd_vag_atr->mode & (1 << 2))
                {
                    while (!(SpuGetReverbVoice() & spu_ch_tbl[vo]))
                    {
                        SpuSetReverbVoice(true, spu_ch_tbl[vo]);
                    }
                }
                else
                {
                    while (SpuGetReverbVoice() & spu_ch_tbl[vo])
                    {
                        SpuSetReverbVoice(false, spu_ch_tbl[vo]);
                    }
                }
            }
            else if (midi->rev_depth_24 != 1)
            {
                while (!(SpuGetReverbVoice() & spu_ch_tbl[vo]))
                {
                    SpuSetReverbVoice(true, spu_ch_tbl[vo]);
                }
            }
            else
            {
                while (SpuGetReverbVoice() & spu_ch_tbl[vo])
                {
                    SpuSetReverbVoice(false, spu_ch_tbl[vo]);
                }
            }
        }
    }
}

void key_off(u8 chan, u8 c1, u8 c2)
{
    s32 stat;
    s32 vo;
    u32 voices;

    voices = 0;

    for (vo = 0; vo < sd_reserved_voice; vo++)
    {
        if (chan == smf_port[vo].midi_ch_3 && c1 == smf_port[vo].note_6)
        {
            if (smf_port[vo].pedal_1B == 0)
            {
                adsr_set(vo, &smf_port[vo]);
                voices |= spu_ch_tbl[vo];

                do
                {
                    SpuSetKey(SPU_OFF, spu_ch_tbl[vo]);
                    stat = SpuGetKeyStatus(spu_ch_tbl[vo]);
                }
                while (stat != SPU_OFF_ENV_ON && stat != SPU_OFF);

                smf_port[vo].stat_16 = 0;
            }
            else
            {
                smf_port[vo].velo_1A |= 1 << 7;
            }
        }
    }

    if (voices != 0)
    {
        do
        {
            SpuSetKey(SPU_OFF, voices);
            stat = SpuGetKeyStatus(voices);
        }
        while (stat != SPU_OFF_ENV_ON && stat != SPU_OFF);
    }
}

void key_press(u8 chan, u8 c1, u8 c2) {} // 0x800A5DCC

VagAtr* get_vab_tone(MIDI* p, u16 tone, u8 chan) // 0x800A5DD4
{
    SD_VAB_H* vab;

    if (p->bank_change_5A > 16)
    {
        vab = vab_h[smf_song[chan >> 4].sd_seq_vab_id_508].vh_addr_4;
    }
    else
    {
        vab = vab_h[p->bank_change_5A].vh_addr_4;
    }

    return &vab->vag_atr[(p->prog_no_0 * 16) + tone];
}

void smf_data_entry(MIDI* p, u8 chan)
{
    SpuReverbAttr rev_attr; // Used in `AUDIO.IRX` but unused here.
    VagAtr*       vp;
    s32           i;

    if (p->nrpn_msb_26 == 0x10 && p->nrpn_lsb_25 < 0xE)
    {
        for (i = 0; i < 16; i++)
        {
            vp = get_vab_tone(p, i, chan);
            switch (p->nrpn_lsb_25)
            {
                case 0:
                    break;

                case 1:
                    vp->mode = p->data_entry_27;
                    break;

                case 2:
                    vp->min = p->data_entry_27;
                    break;

                case 3:
                    vp->max = p->data_entry_27;
                    break;

                case 4:
                    vp->adsr1 = (vp->adsr1 & 0xFF) | ((0x7F - p->data_entry_27) << 8);
                    break;

                case 5:
                    vp->adsr1 = (vp->adsr1 & 0xFF) | ((0x7F - p->data_entry_27) << 8) | 0x8000;
                    break;

                case 6:
                    vp->adsr1 = (vp->adsr1 & 0xFF0F) | ((p->data_entry_27 * 2) & 0xF0);
                    break;

                case 7:
                    vp->adsr1 = (vp->adsr1 & 0xFFF0) | ((p->data_entry_27 >> 3));
                    break;

                case 8:
                    vp->adsr2 = (vp->adsr2 & 0x603F) | ((0x7F - p->data_entry_27) << 6);
                    break;

                case 9:
                    vp->adsr2 = (vp->adsr2 & 0x603F) | ((0x7F - p->data_entry_27) << 6) | 0x8000;
                    break;

                case 10:
                    vp->adsr2 = (vp->adsr2 & 0xFFC0) | ((0x7F - p->data_entry_27) >> 3);
                    break;

                case 11:
                    vp->adsr2 = (vp->adsr2 & 0xFFC0) | ((0x7F - p->data_entry_27) >> 3) | 0x20;
                    break;

                case 12:
                    if (p->data_entry_27 <= 0x40)
                    {
                        vp->adsr2 |= 0x4000;
                    }
                    else
                    {
                        vp->adsr2 &= 0xBFFF;
                    }
                    break;

                case 13:
                    vp->adsr1 = vp->reserved[0];
                    vp->adsr2 = vp->reserved[1];
                    break;
            }
        }
    }
    else
    {
        vp = get_vab_tone(p, p->nrpn_msb_26 & 0xF, chan);
        switch (p->nrpn_lsb_25)
        {
            case 0:
                break;

            case 1:
                vp->mode = p->data_entry_27;
                break;

            case 2:
                vp->min = p->data_entry_27;
                break;

            case 3:
                vp->max = p->data_entry_27;
                break;

            case 4:
                vp->adsr1 = (vp->adsr1 & 0xFF) | ((0x7F - p->data_entry_27) << 8);
                break;

            case 5:
                vp->adsr1 = (vp->adsr1 & 0xFF) | ((0x7F - p->data_entry_27) << 8) | 0x8000;
                break;

            case 6:
                vp->adsr1 = (vp->adsr1 & 0xFF0F) | ((p->data_entry_27 * 2) & 0xF0);
                break;

            case 7:
                vp->adsr1 = (vp->adsr1 & 0xFFF0) | (p->data_entry_27 >> 3);
                break;

            case 8:
                vp->adsr2 = (vp->adsr2 & 0x603F) | ((0x7F - p->data_entry_27) << 6);
                break;

            case 9:
                vp->adsr2 = (vp->adsr2 & 0x603F) | ((0x7F - p->data_entry_27) << 6) | 0x8000;
                break;

            case 10:
                vp->adsr2 = (vp->adsr2 & 0xFFC0) | ((0x7F - p->data_entry_27) >> 3);
                break;

            case 11:
                vp->adsr2 = (vp->adsr2 & 0xFFC0) | ((0x7F - p->data_entry_27) >> 3) | 0x20;
                break;

            case 12:
                if (p->data_entry_27 <= 0x40)
                {
                    vp->adsr2 |= 0x4000;
                }
                else
                {
                    vp->adsr2 &= 0xBFFF;
                }
                break;

            case 13:
                vp->adsr1 = vp->reserved[0];
                vp->adsr2 = vp->reserved[1];
                return;

            case 23:
                p->before_note_13 = p->data_entry_27;
                break;
        }
    }
}

/** Note: Requires maspsx --expand-div flag to build matching. */
void control_change(u8 chan, u8 c1, u8 c2)
{
    s32   vol_flag;
    u32   port;
    MIDI* p;
    s32   stat;
    s32   i;
    s32   vc;

    p        = &smf_midi[chan];
    vol_flag = 0;
    port     = 0;

    switch (c1)
    {
        case 0x0:
            p->bank_change_5A = c2;
            break;

        case 0x1:
            if (p->mod_mode_20 < 0x40)
            {
                p->mod_2        = c2;
                p->mod_limit_1E = c2 << 1;
                if (p->mod_speed_18 != 0 && p->mod_limit_1E != 0)
                {
                    p->mod_add_1A = ((p->mod_limit_1E * 4) / p->mod_speed_18);
                }
                else
                {
                    p->mod_depth_1C = 0;
                    for (i = 0; i < sd_reserved_voice; i++)
                    {
                        if (smf_port[i].midi_ch_3 == chan)
                        {
                            pitch_calc(&smf_port[i], 1);
                        }
                    }
                }

                if (!c2 || p->mod_add_1A == 0)
                {
                    p->mod_depth_1C = 0;
                    for (i = 0; i < sd_reserved_voice; i++)
                    {
                        if (smf_port[i].midi_ch_3 == chan)
                        {
                            pitch_calc(&smf_port[i], 1);
                        }
                    }
                }
            }
            else
            {
                p->mod_2        = c2;
                p->mod_speed_18 = c2;
                if (p->mod_speed_18 != 0 && p->mod_limit_1E != 0)
                {
                    p->mod_add_1A = (p->mod_limit_1E / p->mod_speed_18) * 2;
                }
                else
                {
                    p->mod_depth_1C = 0;
                    for (i = 0; i < sd_reserved_voice; i++)
                    {
                        if (smf_port[i].midi_ch_3 == chan)
                        {
                            pitch_calc(&smf_port[i], 1);
                        }
                    }
                }

                if (!c2 || p->mod_add_1A == 0)
                {
                    p->mod_depth_1C = 0;
                    for (i = 0; i < sd_reserved_voice; i++)
                    {
                        if (smf_port[i].midi_ch_3 == chan)
                        {
                            pitch_calc(&smf_port[i], 1);
                        }
                    }
                }
            }

            set_midi_info(SD_MIDI_MOD, chan, p->mod_2);
            break;

        case 0x2:
            if (p->mod_mode_20 < 0x40)
            {
                p->mod_speed_18 = c2;
            }
            else
            {
                p->mod_limit_1E = c2 << 1;
            }
            break;

        case 0x3:
            p->mod_mode_20 = c2;
            break;

        case 0x4:
            p->bend_mode_10 = c2;
            break;

        case 0x5:
            p->porta_28 = c2;
            break;

        case 0x7:
            p->mvol_3 = c2;
            vol_flag  = 1;
            set_midi_info(SD_MIDI_VOL, chan, p->mvol_3);
            break;

        case 0xA:
            if (c2)
            {
                p->pan_1 = c2;
            }
            else
            {
                p->pan_1 = 1;
            }

            vol_flag = 1;
            set_midi_info(SD_MIDI_PAN, chan, p->pan_1);
            break;

        case 0xB:
            p->express_5 = c2;
            vol_flag     = 1;
            set_midi_info(SD_MIDI_EXP, chan, p->express_5);
            break;

        case 0xC:
            p->vol_mode_11 = c2;
            vol_flag       = 1;
            break;

        case 0xD:
            p->rdms_57 = c2;
            for (vc = 0; vc < sd_reserved_voice; vc++)
            {
                if (chan == smf_port[vc].midi_ch_3)
                {
                    smf_port[vc].rdms_43 = p->rdms_57;
                }
            }
            break;

        case 0xE:
            p->rdmdm_59 = c2;

            for (vc = 0; vc < sd_reserved_voice; vc++)
            {
                if (chan == smf_port[vc].midi_ch_3)
                {
                    smf_port[vc].rdmdm_45 = p->rdmdm_59;
                }
            }
            break;

        case 0xF:
            p->wide_flag_21 = c2;
            break;

        case 0x10:
            p->key_pan_22 = c2;
            break;

        case 0x14:
            p->vibhs_39 = c2 << 1;
            break;

        case 0x15:
            p->vibcad_3B = c2;
            break;

        case 0x16:
            p->vibdm_3E   = c2 << 4;
            p->vibd_3C    = c2 << 4;
            p->vibcs_3A   = 0;
            p->vibcadw_34 = 1;
            break;

        case 0x17:
            p->vibcs_3A = c2;
            if (c2 != 0)
            {
                if (p->vibdm_3E != 0)
                {
                    p->vibad_40 = p->vibdm_3E / c2;
                    if (p->vibad_40 == 0)
                    {
                        p->vibad_40 = 1;
                    }
                }
            }
            break;

        case 0x19:
            p->trehs_51 = c2 << 1;
            break;

        case 0x1A:
            p->trecad_53 = c2;
            break;

        case 0x1B:
            p->tredm_4A   = c2 << 7;
            p->tred_48    = c2 << 7;
            p->trecs_52   = 0;
            p->trecadw_44 = 1;
            break;

        case 0x1C:
            p->trecs_52 = c2;
            if (c2 != 0)
            {
                if (p->tredm_4A != 0)
                {
                    p->tread_4C = p->tredm_4A / c2;
                    if (p->tread_4C == 0)
                    {
                        p->tread_4C = 1;
                    }
                }
            }
            break;

        case 0x1E:
            smf_song[chan >> 4].midi_master_vol_538 = c2;
            seq_master_vol_set(chan >> 4);
            break;

        case 0x40:
            for (vc = 0; vc < sd_reserved_voice; vc++)
            {
                if (chan == smf_port[vc].midi_ch_3)
                {
                    if (c2 < 0x40)
                    {
                        for (i = 0; i < sd_reserved_voice; i++)
                        {
                            if (smf_port[i].midi_ch_3 == chan && smf_port[i].pedal_1B != 0 && smf_port[i].velo_1A & 0x80)
                            {
                                port |= spu_ch_tbl[i];

                                do
                                {
                                    SpuSetKey(SPU_OFF, spu_ch_tbl[i]);
                                    stat = SpuGetKeyStatus(spu_ch_tbl[i]);
                                }
                                while (stat != SPU_OFF_ENV_ON && stat != SPU_OFF);

                                smf_port[i].pedal_1B = 0;
                                smf_port[i].stat_16  = 0;
                            }
                        }
                        smf_port[vc].pedal_1B = 0;
                    }
                    else
                    {
                        smf_port[vc].pedal_1B = 1;
                    }
                }
            }

            if (c2 < 0x40)
            {
                smf_midi[chan].pedal_6 = 0;
            }
            else
            {
                smf_midi[chan].pedal_6 = 1;
            }
            break;

        case 0x5B:
            p->rev_depth_24 = c2 & 0x7F;
            break;

        case 0x6:
            p->data_entry_27 = c2;

            if (p->nrpn_msb_26 != 0x14 && p->nrpn_msb_26 != 0x1E)
            {
                smf_data_entry(p, chan);
            }
            break;

        case 0x62:
            p->nrpn_lsb_25 = c2;
            break;

        case 0x63:
            p->nrpn_msb_26 = c2;
            break;

        case 0x76:
            if (c2 < 0x40)
            {
                if (c2 <= 0x20)
                {
                    smf_song[chan >> 4].mf_seq_beat_51C   = 0;
                    smf_song[chan >> 4].smf_beat_stat_52E = c2;
                }
                else
                {
                    smf_song[chan >> 4].smf_beat_stat_52E = c2 - 0x21;
                }
            }
            else
            {
                smf_song[chan >> 4].smf_control_stat_52F = c2;
            }
            break;

        case 0x77:
            tone_adsr_back(smf_song[chan >> 4].sd_seq_vab_id_508);
            break;

        case 0x78:
        case 0x79:
        case 0x7B:
            sound_seq_off(chan >> 4);
            break;

        case 0x7E:
            p->mode_12 = c2;
            break;
    }

    if (port != 0)
    {
        do
        {
            SpuSetKey(SPU_OFF, port);
            stat = SpuGetKeyStatus(port);
        }
        while (stat != SPU_OFF_ENV_ON && stat != SPU_OFF);
    }

    if (vol_flag != 0)
    {
        for (vc = 0; vc < sd_reserved_voice; vc++)
        {
            if (chan == smf_port[vc].midi_ch_3 && smf_port[vc].stat_16 != 0)
            {
                volume_calc(&smf_port[vc], &smf_midi[chan]);
                smf_vol_set(chan, vc, smf_port[vc].l_vol_C, smf_port[vc].r_vol_E);
            }
        }
    }
}

void program_change(u8 chan, u8 c1) // 0x800A6C2C
{
    smf_midi[chan].prog_no_0 = c1;
}

void chan_press(u8 chan, u8 c1) {} // 0x800A6C58

void pitch_bend(u8 chan, s32 c1, u8 c2)
{
    smf_midi[chan].pbend_7 = c2 & 0x7F;
    set_midi_info(SD_MIDI_BEND, chan, c2);
}

void control_code_set(s32 seq_access_num) {} // 0x800A6CB0
