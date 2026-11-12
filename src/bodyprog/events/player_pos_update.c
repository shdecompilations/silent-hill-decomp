#include "bodyprog/game.h"

#include <psyq/libetc.h>
#include <psyq/libpad.h>
#include <psyq/strings.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/events/player_pos_update.h"
#include "bodyprog/math/math.h"

void Game_PlayerMapPositionSet(s_MapPoint2d* mapPoint) // 0x800371E8
{
    q19_12 headingAngle;

    headingAngle = Q12_ANGLE_FROM_Q8(mapPoint->triggerParam0);
    Math_SVectorSet(&PLAYER_CHARA().rotation, Q12_ANGLE(0.0f), headingAngle, Q12_ANGLE(0.0f));

    PLAYER_CHARA().position.vy = Q12(0.0f);
    PLAYER_CHARA().position.vx = mapPoint->positionX;
    PLAYER_CHARA().position.vz = mapPoint->positionZ;

    if (mapPoint->triggerParam1 >= 2)
    {
        PLAYER_CHARA().position.vx += Q12_MULT_FLOAT_PRECISE(Math_Sin(headingAngle), 0.4f);
        PLAYER_CHARA().position.vz += Q12_MULT_FLOAT_PRECISE(Math_Cos(headingAngle), 0.4f);
    }

    g_SysWork.loadingScreenIdx = mapPoint->loadingScreenId;

    if (mapPoint->paperMapIdx == PaperMapIdx_24)
    {
        g_SavegamePtr->paperMapIdx = PaperMapIdx_OtherPlaces;
    }
    else if (mapPoint->paperMapIdx != PaperMapIdx_OtherPlaces)
    {
        g_SavegamePtr->paperMapIdx = mapPoint->paperMapIdx;
    }

    g_SysWork.cameraAngleY = headingAngle;

    func_8007E9C4();
    Game_MapRoomIdxUpdate();
}

void Game_PlayerHeightUpdate(void) // 0x80037334
{
    s_CollisionSurface surface;

    if (g_MapOverlayHdr.updateWorldObjects != NULL)
    {
        g_MapOverlayHdr.updateWorldObjects();
    }

    Collision_SurfaceGet(&surface, PLAYER_CHARA().position.vx, PLAYER_CHARA().position.vz);
    PLAYER_CHARA().position.vy = surface.floorHeight;
}
