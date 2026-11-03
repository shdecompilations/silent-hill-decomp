#include "pc/audio/shim.h"
#ifdef PC_PORT
#include <SDL2/SDL.h>
#include <stdio.h>
#include <string.h>

static bool s_AudioInit = false;
static SDL_AudioDeviceID s_Dev = 0;

void PcAudio_Init(e_PcAudioBackend backend) {
    (void)backend;
    if (s_AudioInit) return;
    SDL_AudioSpec want = {0}, have = {0};
    want.freq = 44100;
    want.format = AUDIO_S16SYS;
    want.channels = 2;
    want.samples = 1024;
    s_Dev = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);
    if (s_Dev) {
        SDL_PauseAudioDevice(s_Dev, 0);
        printf("PcAudio: %d Hz %d ch\n", have.freq, have.channels);
    } else {
        printf("PcAudio: no device (%s), audio stubbed\n", SDL_GetError());
    }
    s_AudioInit = true;
}

void PcAudio_Shutdown(void) {
    if (s_Dev) SDL_CloseAudioDevice(s_Dev);
    s_AudioInit = false;
}

void PcAudio_Tick(void) {}

void PcSfx_Play(s32 sfxId, q19_12 posX, q19_12 posZ, q8_8 volume) {
    (void)posX; (void)posZ; (void)volume;
    // printf("SFX %d\n", sfxId);
}
void PcSfx_Stop(s32 sfxId) { (void)sfxId; }
void PcSfx_SetPos(s32 sfxId, q19_12 x, q19_12 z) { (void)sfxId; (void)x; (void)z; }

bool PcVab_Load(const char* path, int vabId) { (void)path; (void)vabId; return true; }
void PcVab_Unload(int vabId) { (void)vabId; }

struct _PcKdtTrack { int dummy; };
PcKdtTrack* PcKdt_Load(const char* path) { (void)path; static PcKdtTrack t; return &t; }
void PcKdt_Play(PcKdtTrack* track, bool loop) { (void)track; (void)loop; }
void PcKdt_Stop(PcKdtTrack* track) { (void)track; }
void PcKdt_SetVolume(PcKdtTrack* track, int vol) { (void)track; (void)vol; }

void PcXa_Play(int xaChannel, int sector) { (void)xaChannel; (void)sector; }
void PcXa_Stop(void) {}
bool PcXa_IsPlaying(void) { return false; }

#endif
