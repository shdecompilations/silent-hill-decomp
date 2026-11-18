#include "bodyprog/bodyprog.h"
#include "bodyprog/events/bgm_update.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player/player.h"
#include "main/rng.h"
#include "maps/map0/map0_s01.h"
#include "maps/common/characters/cybil.h"
#include "maps/common/particle.h"
#include "maps/common/characters/air_screamer.h"
#include "maps/common/characters/player.h"
#include "inline_no_dmpsx.h"
#include <psyq/gtemac.h>

#include "../src/maps/common/particles/glass.c"

#include "../src/maps/common/particles/particle.c"

#include "../src/maps/common/characters/player.c"

#include "../src/maps/common/characters/air_screamer.c" // 0x800D2170
#include "../src/maps/common/characters/cybil.c"

#include "../src/maps/common/chara_util.c" // 0x800D9B0C

#include "../src/maps/common/map_util.c" // 0x800DA51C

#include "../src/maps/common/bgm/bgm_THR.c" // 0x800DA778

void GameBoot_LoadScreen_StageString(void) // 0x800DA8A4
{
    Screen_FadeStart(false, true, false);

    Text_Debug_PositionSet(100, 100);
    Text_Debug_Draw("STAGE 0-1");
    GameBoot_LoadScreen_BackgroundImg();
}

#include "maps/common/shared/Event_CutsceneTimerAdvance.h" // 0x800DA8E8
