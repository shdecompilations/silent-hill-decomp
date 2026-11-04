#ifndef _BODYPROG_LIBSD_H
#define _BODYPROG_LIBSD_H

#include <psyq/libsnd.h>

/** libsd: konami-customized version of libsnd?
 * Majority of the functions work like the libsnd Ss versions.
 * libref.pdf v4.4 may be useful, though was likely based on an earlier SDK.
 */

#define SD_VOICE_COUNT 24

/** `SD_ALLOC_SLOTS` must equal `SD_VAB_SLOTS` due to `SdWorkInit` loop, but they're mostly unrelated. */

#define SD_ALLOC_SLOTS 16
#define SD_VAB_SLOTS   16

/** MIDI-related header magic. */

#define SD_MAGIC_SEQp 0x53455170
#define SD_MAGIC_MThd 0x6468544D
#define SD_MAGIC_KDT  0x2054444B
#define SD_MAGIC_KDT1 0x3154444B

typedef enum _SMF_STAT
{
    SEQ_NON   = 0,
    SEQ_PLAY  = 1,
    SEQ_STOP  = 2,
    SEQ_END   = 3,
    SEQ_PAUSE = 4
} SMF_STAT;

typedef enum _SMF_MIDI_STAT
{
    SD_MIDI_BEND = 1,
    SD_MIDI_PAN  = 2,
    SD_MIDI_VOL  = 3,
    SD_MIDI_MOD  = 4,
    SD_MIDI_EXP  = 5
} SMF_MIDI_STAT;

typedef enum _SMF_FILE_MODE
{
    SEQ_MODE = 0,
    SMF_MODE = 1,
    KDT_MODE = 2
} SMF_FILE_MODE;

typedef struct _SD_VAB_H
{
    VabHdr  vab_h;
    ProgAtr vab_prog[128];
    VagAtr  vag_atr[256]; // 16 per program.
} SD_VAB_H;

typedef struct _VAB_H
{
    /* 0x0  */ s16       vab_id;
    /* 0x4  */ SD_VAB_H* vh_addr_4;
    /* 0x8  */ s32       vh_size_8;
    /* 0xC  */ u8*       vb_addr_C;
    /* 0x10 */ s32       vb_start_addr_10;
    /* 0x14 */ s32       vb_size_14;
    /* 0x18 */ s8        mvol_18; // These `s8`s are `u8` in `soundcd.irx`.
    /* 0x19 */ s8        mvoll_19;
    /* 0x1A */ s8        mvolr_1A;
    /* 0x1B */ s8        mpan_1B;
} VAB_H;
STATIC_ASSERT_SIZEOF(VAB_H, 28);

typedef struct _Seqhdr
{
    u8  form_0[4];
    u32 size_4;
    u32 tb_8;
    u32 tracks_C;
    u16 tr_size_10[32];
} Seqhdr;
STATIC_ASSERT_SIZEOF(Seqhdr, 80);

typedef struct _SMF
{
    u32 mf_data_loc_0;
    u32 mf_loop_point_4;
    u32 mf_track_length_8;
    u32 mf_track_size_C;
    u32 mf_repeat_ptr_10;
    u16 mf_tempo_14;
    u16 mf_tempo2_16;
    u16 time_hosei_18;
    u16 time_hosei_wk_1A;
    u16 mf_delta_time_1C;
    u16 mf_delta_time_wk_1E;
    u8  mf_eof_flag_20;
    u8  mf_eof_flag_wk_21;
    u8  mf_loop_count_22;
    u8  ti_flag_23;
    u8  running_status_flag_24;
    u8  status_value_25;
    u8  status_value_wk_26;
    u8  midi_ch_27;
} SMF;
STATIC_ASSERT_SIZEOF(SMF, 40);

/** @brief Standard MIDI File */
typedef struct _SMF_SONG
{
    SMF   tracks_0[32];
    u8*   mf_data_ptr2_500;
    u8*   mf_data_ptr_504;
    s16   sd_seq_vab_id_508;
    s16   sd_seq_stat_50A; /** `SMF_STAT` enum */
    s16   sd_seq_mvoll_50C;
    s16   sd_seq_mvolr_50E;
    u32   sd_seq_track_mute_510;
    void* sd_seq_start_addr_514;
    s32   mf_data_size_518;
    u32   mf_seq_beat_51C;
    s32   mf_seq_beat_wk_520;
    u16   mf_format_524;
    u16   mf_tracks_526;
    u16   mf_division_528;
    u16   mf_head_len_52A;
    u8    smf_seq_flag_52C; /** `SMF_FILE_MODE` enum */
    u8    loop_start_flag_52D;
    u8    smf_beat_stat_52E;
    u8    smf_control_stat_52F;
    u16   seq_vol_set_flag_530;
    u16   seq_rev_set_flag_532;
    u16   seq_wide_flag_534;
    u16   seq_reverb_depth_536;
    u16   midi_master_vol_538;
    u8    unk_53A[2];
} SMF_SONG;
STATIC_ASSERT_SIZEOF(SMF_SONG, 1340);

/** @brief `MIDI` fields are mainly changed by `control_change`, which takes in a MIDI control change ID and adjusts the appropriate field.
 * Seems that func accepts some change IDs that are undefined in MIDI specs though...
 */
typedef struct _MIDI
{
    u8  prog_no_0;
    u8  pan_1;
    u8  mod_2;
    u8  mvol_3;
    u8  velo_4;
    u8  express_5;
    u8  pedal_6;
    u8  pbend_7;
    u32 l_vol_8;
    u32 r_vol_C;
    u8  bend_mode_10;
    u8  vol_mode_11;
    u8  mode_12;
    u8  before_note_13;
    u16 mod_w_14;
    u16 mod_time_16;
    u16 mod_speed_18;
    s16 mod_add_1A;
    s16 mod_depth_1C;
    s16 mod_limit_1E;
    u8  mod_mode_20;
    u8  wide_flag_21;
    u16 key_pan_22;
    u8  rev_depth_24;
    u8  nrpn_lsb_25;
    u8  nrpn_msb_26;
    u8  data_entry_27;
    s16 porta_28;
    s16 porta_depth_2A;
    s16 porta_add_2C;
    u16 porta_limit_2E;
    u16 porta_wk_30;
    u16 pitch_32;
    u8  vibcadw_34;
    u8  vibcadw2_35;
    u8  vibhc_36;
    s8  vibc_37;
    u8  vibcc_38;
    u8  vibhs_39;
    u8  vibcs_3A;
    u8  vibcad_3B;
    u16 vibd_3C;
    u16 vibdm_3E;
    u16 vibad_40;
    u16 vib_data_42;
    u8  trecadw_44;
    u8  trecadw2_45;
    u8  trehc_46;
    s8  trec_47;
    u16 tred_48;
    u16 tredm_4A;
    u16 tread_4C;
    u16 tre_data_4E;
    u8  trecc_50;
    u8  trehs_51;
    u8  trecs_52;
    u8  trecad_53;
    u16 rdmd_54;
    u8  rdmo_56;
    u8  rdms_57;
    u8  rdmc_58;
    u8  rdmdm_59;
    u8  bank_change_5A;
    u8  unk_5B[1];
} MIDI;
STATIC_ASSERT_SIZEOF(MIDI, 92);

typedef struct _PORT
{
    u8  vc_0; // `soundcd.irx` has `vab_id` here, and `vc_1` after, but the reads/writes to +0x1 in soundcd are to +0x0 in SH, so this is likely `vc`.
    u8  field_1;
    u8  prog_2;
    u8  midi_ch_3;
    u16 tone_4;
    u16 note_6;
    u16 note_wk_8;
    u16 fine_A;
    u16 l_vol_C;
    u16 r_vol_E;
    u8  pvol_10;
    u8  tvol_11;
    u8  ppan_12;
    u8  tpan_13;
    u16 pan_14;
    u16 stat_16;
    u16 pitch_18;
    u8  velo_1A;
    u8  pedal_1B;
    u8  bend_max_1C;
    u8  bend_min_1D;
    u8  center_1E;
    u8  shift_1F;
    u8  vibcadw_20;
    u8  vibcadw2_21;
    u8  vibhc_22;
    s8  vibc_23;
    u16 vibd_24;
    u16 vibdm_26;
    u8  vibcc_28;
    u8  vibhs_29;
    u8  vibcs_2A;
    u8  vibcad_2B;
    u16 vibad_2C;
    u16 vib_data_2E; // `s16` in `soundcd.irx`?
    u8  trecadw_30;
    u8  trecadw2_31;
    u8  trehc_32;
    s8  trec_33;
    u16 tred_34;
    u16 tredm_36;
    u8  trecc_38;
    u8  trehs_39;
    u8  trecs_3A;
    u8  trecad_3B;
    u16 tread_3C;
    s16 tre_data_3E;
    u16 rdmd_40; // `s16` in `soundcd.irx`?
    u8  rdmo_42;
    u8  rdms_43;
    u8  rdmc_44;
    u8  rdmdm_45;
    u16 adsr1_46;
    u16 adsr2_48;
    u16 a_mode_4A;
    s16 tremoro_wk_4C;
    u16 pbend_wk_4E;
    u16 pbend_50;
    u8  vab_id_52; // `soundcd.irx` accesses this at +0x0 while SH accesses at +0x52, lines up with comment on `vc_0`.
    u8  unk_53[1]; // `core_no` or `vh_mode`?
} PORT;
STATIC_ASSERT_SIZEOF(PORT, 84);

typedef struct _SD_SPU_ALLOC
{
    u32 addr_0;
    s32 size_4;
} SD_SPU_ALLOC;
STATIC_ASSERT_SIZEOF(SD_SPU_ALLOC, 8);

// smf_snd.c BSS
extern VAB_H        vab_h[SD_VAB_SLOTS];
extern u8           sd_vb_malloc_rec[136];
extern s16          sd_seq_loop_mode;
extern s16          __pad_bss_800C7662;
extern s32          sd_tick_mode;
extern SD_SPU_ALLOC sd_spu_alloc[SD_ALLOC_SLOTS];

// smf_io.c BSS
extern s32  __pad_bss_800C7730[2];
extern MIDI smf_midi[2 * 16];   // 2 devices with 16 channels each?
extern MIDI smf_midi_sound_off; // Set by `sound_off`, could be `smf_midi[32]`, but game doesn't use offsets for `[32]`?
extern s32  __pad_bss_800C8314;
extern PORT smf_port[24];
extern s32  sd_timer_event;

// smf_mid.c BSS
extern SMF_SONG smf_song[2];

extern s32  sd_reverb_area_size[10];
extern s32  sd_reverb_mode;
extern s16  sd_keyoff_mode;
extern bool sd_interrupt_start_flag;
extern s32  sd_reserved_voice;
extern bool sd_mono_st_flag;
extern bool sd_int_flag;
extern bool sd_int_flag2;
extern s32  sd_timer_sync;
extern bool smf_start_flag;
extern u32  body_partly_size;
extern s32  smf_file_no;
extern u32  spu_ch_tbl[24];
extern u16  PitchTbl[12][128];

static inline PORT* SmfGetPort(s32 voice)
{
    return &smf_port[voice];
}

// smf_snd.c

void tone_adsr_mem(s16 vab_id);
void tone_adsr_back(s16 vab_id);
void sd_alloc_sort(void);
s32  SdSpuMalloc(s32 size);
u32  SdSpuMallocWithStartAddr(u32, s32);
void SdSpuFree(u32 addr);
void SdWorkInit(void);
void SdInit(void);
void SdStart(void);
void SdStart2(void);
void SdSetTickMode(s32 tick_mode);
void SdSeqCalledTbyT(void);
void SdSetStereo(void);
void SdSetMono(void);
char SdSetReservedVoice(char voices);
void SdSetTableSize(void* arg0, s32 arg1, s32 arg2);
void SdEnd(void);
void SdQuit(void);
void SdSetSerialAttr(char s_num, char attr, char mode);
void SdSetSerialVol(s16 s_num, s16 voll, s16 volr);
void SdSetMVol(s16 voll, s16 volr);
s16  SdVabOpenHead(u8* addr, s16 vabid);
s16  SdVabOpenHeadSticky(u8* addr, s16 vabid, s32 sbaddr);
s16  SdVabFakeHead(u8* addr, s16 vabid, u32 sbaddr);

s16  SdVabTransBody(u8* addr, s16 vabid);
s16  SdVabTransBodyPartly(u8* addr, u32 bufsize, s16 vabid);
s16  SdVabTransCompleted(s16 immediateFlag);
void SdVabClose(s16 vab_id);
void SdSetAutoKeyOffMode(s16 mode);
void SdAutoKeyOffCheck(void);
s16  SdSeqOpen(s32* addr, s16 vab_id);
s16  SdSeqOpenWithAccNum(s32* addr, s16 vab_id, s16 seq_access_num);
void SdSeqPlay(s16 seq_access_num, u8 play_mode, s16 l_count);
void SdSeqStop(s16 seq_access_num);
void SdSeqClose(s16 seq_access_num);
void SdSeqPause(s16 seq_access_num);
void SdSeqReplay(s16 seq_access_num);
void SdSeqSetVol(s16 seq_access_num, s16 voll, s16 volr);
void SdSeqGetVol(s16 seq_access_num, s16* voll, s16* volr);
void SdUtFlush(void);
void SdUtReverbOn(void);
void SdUtReverbOff(void);
s16  SdUtSetReverbType(s16 type);
void SdUtSetReverbDepth(s16 ldepth, s16 rdepth);
void SdSetRVol(s16 ldepth, s16 rdepth);
void SdUtSEAllKeyOff(void);
void SdUtAllKeyOff(s16 mode);
s32  SdUtGetVabHdr(s16 vabId, VabHdr* vabhdrptr);
s32  SdVoKeyOn(s32 vab_pro, s32 pitch, u16 voll, u16 volr);
void SdVoKeyOff(s32 vab_pro, s32 pitch);
void SdVoKeyOffWithRROff(s32 vab_pro, s32 pitch);

s16  SdUtKeyOnV(s16 voice, s16 vabid, s16 prog, s16 tone, s16 note, s16 fine, s16 voll, s16 volr);
s16  SdUtKeyOn(s16 vabid, s16 prog, s16 tone, s16 note, s16 fine, s16 voll, s16 volr);
s16  SdVbKeyOn(s16 vabid, s32 voice, s16 center, s16 shift, s16 note, s16 voll, s16 volr);
s32  SdUtKeyOffV(s16 vo);
s32  SdUtKeyOffVWithRROff(s16 vo);
s16  SdGetSeqStatus(s16 access_num);
s32  SdUtSetDetVVol(s16 vc, s16 voll, s16 volr);
s32  SdUtSetVVol(s16 vc, s16 voll, s16 volr);
s32  SdUtGetDetVVol(s16 vc, u16* voll, u16* volr);
s32  SdUtGetVVol(s16 vc, u16* voll, u16* volr);
u16  SdGetTempo(s16 seq_access_num);
void SdSetTempo(s16 seq_access_num, s16 tempo);
void SdSetSeqWide(s16 seq_access_num, u16 seq_wide);
u8   SdGetMidiVol(s16 seq_access_num, s16 midi_ch);
void SdSetMidiVol(s16 seq_access_num, s16 midi_ch, s32 vol);
void SdSetMidiExpress(s16 seq_access_num, s16 midi_ch, s32 expression);
u8   SdGetMidiExpress(s16 seq_access_num, s16 midi_ch);
u8   SdGetMidiPan(s16 seq_access_num, s16 midi_ch);
void SdSetMidiPan(s16 seq_access_num, s16 midi_ch, s32 pan);
u8   SdGetMidiPitchBendFine(s16 seq_access_num, s16 midi_ch);
s32  SdSetMidiPitchBendFine(s16 seq_access_num, s16 midi_ch, u8 pitchBendFine);
s32  SdGetTrackTranspause(void);
s32  SdSetTrackTranspause(void);
s32  SdGetTrackMute(s16 seq_access_num, s32 midi_ch);
s32  SdSetTrackMute(s16 seq_access_num, s32 midi_ch);
s32  SdGetSeqControlStatus(s16 seq_access_num);
s16  SdGetSeqPlayStatus(s32 access_num); /** Returns `SMF_STAT`. */
u32  SdGetSeqBeat(s16 seq_access_num);
s32  SdGetSeqBeat2(s16 seq_access_num);

// Wrappers for standard `PsyQ Ss*` funcs.

void SsSetMVol(s16 voll, s16 volr);
void SsEnd(void);
void SsSetSerialAttr(char s_num, char attr, char mode);
void SsSetSerialVol(char s_num, s16 voll, s16 volr);
void SsUtAllKeyOff(s16 mode);

// smf_io.c

void set_note_on(s16 arg0, u8 arg1, u8 arg2, s16 arg3, s16 arg4);
void set_midi_info(s32 type, u8 midiChannel, u32 value); /** type = `SMF_MIDI_STAT` */
u16  Note2Pitch(s16, s16, s16, s16);
void tre_calc(PORT* p);
void vib_calc(PORT* p);
void random_calc(PORT* p);
void volume_calc(PORT* p, MIDI* mp);
void smf_vol_set(s32 ch, s32 vc, s32 l_vol, s32 r_vol);
void master_vol_set(void);
void seq_master_vol_set(s32 access_num);
void toremoro_set(void);
s32  pitch_bend_calc(PORT* p, u32 pit);
void pitch_calc(PORT* p, s32 forceSpuUpdate);
void midi_mod(MIDI* p);
void midi_porta(MIDI* p);
void replay_reverb_set(s16 acc);
void midi_vsync(void);
void sound_seq_off(s32 access_num);
void sound_off(void);

void    adsr_set(s32 vo, PORT* p);
void    rr_off(s32 vo);
s16     voice_check(s32 chan, s32 note, bool flag);
void    key_on(u8 chan, u8 c1, u8 c2);
void    key_off(u8 chan, u8 c1, u8 c2);
void    key_press(u8 chan, u8 c1, u8 c2); // Nullsub.
VagAtr* get_vab_tone(MIDI* p, u16 tone, u8 chan);
void    smf_data_entry(MIDI* p, u8 chan);
void    control_change(u8 chan, u8 c1, u8 c2);
void    program_change(u8 chan, u8 c1);
void    chan_press(u8 chan, u8 c1); // Nullsub.
void    pitch_bend(u8 chan, s32 c1, u8 c2);

void control_code_set(s32 seq_access_num);

// smf_main.c

bool smf_timer(void);
void smf_timer_set(void);
void smf_timer_end(void);
void smf_timer_stop(void);
void smf_vsync(void);

// smf_mid.c

s32 MemCmp(u8* src, u8* des, u32 num);
u32 readMThd(u32 loc);
u32 readMTrk(u32 loc);
u32 readEOF(u32 loc);
u32 egetc(SMF* p);
u32 readvarinum(SMF* p);

// `to32bit`/`to16bit`/`len_add` only seem used inside `smf_mid.c`, can probably be removed from header.
u32  to32bit(u8 c1, u8 c2, u8 c3, u8 c4);
u32  to16bit(u8 c1, u8 c2);
u32  read32bit(SMF* p);
u16  read16bit(SMF* p);
u8   readheader(s32 file_no);
void len_add(SMF* p, u32 len);
void metaevent(SMF* p, u8 type);
void sysex(SMF* p);
void chanmessage(SMF* p, u8 status, u8 c1, u8 c2);
u8   readtrack(SMF* p);
u8   readtrack2(SMF* p);
u8   track_head_read(SMF* p);
void delta_time_conv(SMF* p);
u8   midi_file_out(s32 file_no);
u8   midi_smf_main(void);
void midi_smf_stop(s32 access_num);
s16  midi_smf_stat(s32 access_no); /** Returns SMF_STAT. */

#endif
