#include "game.h"

#if VERSION_IS(JAP0)
    const s32 g_bodyProg = 0xCC010000;
#elif VERSION_IS(JAP1)
    const s32 g_bodyProg = 0x03FF0000;
#elif VERSION_IS(JAP2)
    const s32 g_bodyProg = 0x01000400;
#else
    const s32 g_bodyProg = 0x430800;
#endif
