#include "game.h"

#include <psyq/libetc.h>
#include <psyq/libpad.h>
#include <psyq/strings.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/world/collision_trigger.h"
#include "bodyprog/sys/memcard.h"
#include "bodyprog/screen/draw.h"
#include "bodyprog/text/text_draw.h"
#include "bodyprog/math/math.h"
#include "bodyprog/sound/sound_system.h"
#include "main/fsqueue.h"
#include "main/rng.h"
#include "screens/stream/stream.h"

void World_CollisionTriggersSet(s_MapOverlayHdr* overlayHeader) // 0x80040004
{
    g_WorldGfxWork.collisionTriggers = &overlayHeader->collisionTriggers;
}

void World_NearbyPlayerCollisionTriggersGet(void) // 0x80040014
{
    #define playerChara g_SysWork.playerWork.player

    Collision_NearbyTriggersGet(playerChara.position.vx, playerChara.position.vz, g_WorldGfxWork.collisionTriggers);

    #undef playerChara
};
