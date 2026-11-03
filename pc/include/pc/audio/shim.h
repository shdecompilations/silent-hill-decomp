#pragma once
/**
 * Audio shim — replaces libspu / libsd / libsnd on PC.
 *
 * PSX audio path: SEQ (KDT) → libsd → SPU voices → mixer
 * PC path:        KDT parser → SDL_mixer / miniaudio → SDL audio device
 *
 * We preserve the SFX ID namespace (sfx_id_enum.h) so game code is unchanged.
 */

#include "decomp/types.h"
#include "bodyprog/sound/sfx_id_enum.h"

#ifdef PC_PORT

typedef enum _PcAudioBackend {
    PcAudioBackend_SDLMixer = 0,
    PcAudioBackend_MiniAudio = 1,
} e_PcAudioBackend;

void PcAudio_Init(e_PcAudioBackend backend);
void PcAudio_Shutdown(void);
void PcAudio_Tick(void); // call once per VSync

// SFX — mirrors Sd_SfxPlay / SD_Call
void PcSfx_Play(s32 sfxId, q19_12 posX, q19_12 posZ, q8_8 volume);
void PcSfx_Stop(s32 sfxId);
void PcSfx_SetPos(s32 sfxId, q19_12 x, q19_12 z);

// VAB — sample bank loading (1ST/BASE.VAB etc.)
bool PcVab_Load(const char* path, int vabId);
void PcVab_Unload(int vabId);

// KDT — MIDI-like sequence playback
typedef struct _PcKdtTrack PcKdtTrack;
PcKdtTrack* PcKdt_Load(const char* path);
void PcKdt_Play(PcKdtTrack* track, bool loop);
void PcKdt_Stop(PcKdtTrack* track);
void PcKdt_SetVolume(PcKdtTrack* track, int vol); // 0..127

// XA streaming (HILL. / XA/)
void PcXa_Play(int xaChannel, int sector);
void PcXa_Stop(void);
bool PcXa_IsPlaying(void);

#else
#endif
