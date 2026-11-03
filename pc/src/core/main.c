/**
 * PC entry point — replaces PSX crt0 + main()'s ResetCallback/CdInit path.
 *
 * Flow:
 *   1. SDL_Init + window + renderer (via pc/gpu)
 *   2. PcFs_Init(assetsRoot) — locate extracted game data
 *   3. PcAudio_Init()
 *   4. Run the decompiled MainLoop() from src/bodyprog/sys/game_main.c
 *      (which already contains the state machine: Konami logo → menu → in-game)
 *   5. Pump SDL events → translate to s_ControllerData (replaces libpad)
 */

#include "pc/core/shiftable.h"
#include "pc/fs/pc_fs.h"
#include "pc/gpu/shim.h"
#include "pc/audio/shim.h"

#include "bodyprog/bodyprog.h"
#include "bodyprog/sys/game_main.h"
#include "main/fileinfo.h"
#include "bodyprog/screen/screen_draw.h"
#include <SDL2/SDL.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Provided by linkage against src/main/main.c + src/bodyprog/sys/game_main.c
extern void MainLoop(void);
extern s_SysWork g_SysWork;
extern s_GameWork g_GameWork;

static bool s_Running = true;
static SDL_Window* s_Window = NULL;

static void handle_args(int argc, char** argv, const char** outAssetsRoot) {
    *outAssetsRoot = "assets/USA";
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--assets") == 0 && i + 1 < argc) {
            *outAssetsRoot = argv[++i];
        } else if (strcmp(argv[i], "--help") == 0) {
            printf("Usage: %s [--assets assets/USA] [--windowed] [--scale N]\n", argv[0]);
            exit(0);
        }
    }
}

static void sdl_to_pad(SDL_Event* ev) {
    // Translate SDL keyboard → s_ControllerData (g_SysWork.controllers / g_Controller0)
    // Mirrors sys/joy.c Joy_Update() but for host input.
    // We write directly into g_GameWork.controllers so the existing Joy_Update
    // can still run (it will just see our pre-filled values).
    //
    // PSX pad bits (from libpad.h): PADLup, PADLdown, etc. → keep same values.
    extern s_ControllerData* g_Controller0;
    if (!g_Controller0) return;

    // Minimal mapping: WASD + Space/Esc handled in PcInput_Tick()
    (void)ev;
}

int main(int argc, char** argv) {
    const char* assetsRoot = NULL;
    handle_args(argc, argv, &assetsRoot);

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER | SDL_INIT_TIMER) != 0) {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* win = SDL_CreateWindow("Silent Hill — PC Recompilation",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        960, 720, // 3x PSX 320x240
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!win) {
        fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError());
        return 1;
    }
    s_Window = win;

    PcGfx_Init(960, 720);
    PcFs_Init(assetsRoot);
    PcAudio_Init(0);
    PcGte_Init();

    // Load BODYPROG overlay into heap (was at 0x80024B60)
    // On PSX this is Fs_QueueStartRead(FILE_1ST_BODYPROG_BIN, FS_BUFFER_0)
    // On PC we do it synchronously at boot so MainLoop() can run.
    {
        s_PcOverlay* bodyprog = &g_PcOverlays[PcOverlay_Bodyprog];
        bodyprog->id = PcOverlay_Bodyprog;
        // If assets exist, load the real binary and fixup pointers;
        // otherwise allocate a dummy so the engine can at least init.
        size_t sz = 0;
        void* data = PcFs_LoadFile(FILE_1ST_BODYPROG_BIN, &sz);
        if (data) {
            bodyprog->base = data;
            bodyprog->size = sz;
            sh_overlay_fixup(bodyprog);
            printf("Loaded BODYPROG.BIN: %zu bytes at %p (VRAM was 0x80024B60)\n", sz, data);
        } else {
            printf("Warning: BODYPROG.BIN not found under %s — running with stubs\n", assetsRoot);
            bodyprog->base = calloc(1, 0xA0000);
            bodyprog->size = 0xA0000;
        }
    }

    // Initial game state — mirrors main.c's boot path but without CD init
    // The PSX main() does Fs_QueueInitialize() + VSync + ResetGraph;
    // we synthesize that:
    Fs_QueueInitialize();
    g_GameWork.gameState = 0; // GameState_Init

    // Main loop — was `while (1) { MainLoop(); VSync(0); }` on PSX
    // On PC: Cap to 60 Hz via SDL_Delay, pump events.
    Uint64 freq = SDL_GetPerformanceFrequency();
    Uint64 last = SDL_GetPerformanceCounter();
    const double targetDt = 1.0 / 60.0;

    while (s_Running) {
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) s_Running = false;
            if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_ESCAPE) {
                // Map to PSX Start for pause
            }
            sdl_to_pad(&ev);
        }

        Uint64 now = SDL_GetPerformanceCounter();
        double dt = (double)(now - last) / (double)freq;
        if (dt < targetDt) {
            SDL_Delay((Uint32)((targetDt - dt) * 1000.0));
            continue;
        }
        last = now;

        // Tick subsystems (was Fs_QueueUpdate + VSync)
        PcFs_Update();
        PcAudio_Tick();

        // Run one frame of game logic — this is the decompiled engine
        PcGfx_BeginFrame();
        MainLoop();
        PcGfx_EndFrame();

        // Present (was PutDispEnv / DrawSync)
        SDL_Delay(1); // yield
    }

    PcAudio_Shutdown();
    PcGfx_Shutdown();
    PcFs_Shutdown();
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
