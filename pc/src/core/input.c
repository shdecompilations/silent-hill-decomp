#include <SDL2/SDL.h>
#include "bodyprog/sys/joy.h"
#include "game.h"

// Map SDL keyboard / gamepad to s_ControllerData (libpad + bodyprog/joy.h)
// This replaces the PSX pad polling in sys/joy.c on PC.

void PcInput_Tick(void) {
    extern s_ControllerData* g_Controller0;
    extern s_ControllerData* g_Controller1;
    if (!g_Controller0) return;

    const Uint8* keys = SDL_GetKeyboardState(NULL);
    s_ControllerData* pad = g_Controller0;

    // Clear previous frame's clicked
    pad->buttonFlags.clicked = 0;

    // D-Pad → WASD / arrows (libpad bits)
    u32 held = 0;
    if (keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP])    held |= (1<<12); // PADLup
    if (keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN])  held |= (1<<13); // PADLdown
    if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT])  held |= (1<<14); // PADLleft
    if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT]) held |= (1<<15); // PADLright
    if (keys[SDL_SCANCODE_SPACE])  held |= (1<<5);  // PADRright (action)
    if (keys[SDL_SCANCODE_LSHIFT]) held |= (1<<10); // PADL1 (run)
    if (keys[SDL_SCANCODE_E])      held |= (1<<11); // PADR1 (aim)
    if (keys[SDL_SCANCODE_Q])      held |= (1<<10); // PADL1
    if (keys[SDL_SCANCODE_ESCAPE]) held |= (1<<3);  // PADstart
    if (keys[SDL_SCANCODE_TAB])    held |= (1<<0);  // PADselect (map)

    // Analog sticks — map right stick to camera, left to movement
    // PSX had no analog in v1.1, but bodyprog_80089090.c uses kpad for analog
    SDL_GameController* gc = NULL;
    for (int i = 0; i < SDL_NumJoysticks(); i++) {
        if (SDL_IsGameController(i)) { gc = SDL_GameControllerOpen(i); break; }
    }
    if (gc) {
        int lx = SDL_GameControllerGetAxis(gc, SDL_CONTROLLER_AXIS_LEFTX);
        int ly = SDL_GameControllerGetAxis(gc, SDL_CONTROLLER_AXIS_LEFTY);
        // Deadzone
        if (abs(lx) > 8000) {
            if (lx < 0) held |= (1<<14); else held |= (1<<15);
        }
        if (abs(ly) > 8000) {
            if (ly < 0) held |= (1<<12); else held |= (1<<13);
        }
        pad->leftStickX = lx / 256;
        pad->leftStickY = ly / 256;

        if (SDL_GameControllerGetButton(gc, SDL_CONTROLLER_BUTTON_A)) held |= (1<<5);
        if (SDL_GameControllerGetButton(gc, SDL_CONTROLLER_BUTTON_START)) held |= (1<<3);
    }

    pad->buttonFlags.held = held;
    // pressedThisFrame = held & ~prevHeld — PSX did this in joy.c
    static u32 prevHeld = 0;
    pad->buttonFlags.clicked = held & ~prevHeld;
    prevHeld = held;
}
