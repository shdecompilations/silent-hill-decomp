#include "bodyprog/game.h"

#include <psyq/libetc.h>
#include <psyq/libpad.h>
#include <psyq/strings.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/events/event_data.h"
#include "bodyprog/events/events_main.h"
#include "bodyprog/world/effects.h"
#include "bodyprog/screen/data.h"
#include "bodyprog/math/math.h"

void Event_Update(bool disableButtonEvents) // 0x800373CC
{
    s_MapPoint2d* mapPoint;
    s_EventData*  mapEvent;
    q19_12        pointPosX;
    q19_12        pointPosZ;
    q19_12        pointRadiusX;
    q19_12        pointRadiusZ;
    s32           i;

    void Event_ItemTriggersClear() // 0x80037388
    {
        s32 i;

        for (i = 0; i < 5; i++)
        {
            g_ItemTriggerItemIds[i] = NO_VALUE;
            g_ItemEvents[i]  = NULL;
        }
    }

    // `lastUsedItem` is set by `Inventory_ItemUse` when player uses an item that matches one of the item trigger events.
    // If it's set, find its index in `g_ItemTriggerItemIds` and use that to get the corresponding `s_EventData` from `g_ItemEvents`.
    // After processing, the field is cleared and item trigger IDs are reset.
    // (Multi-item events likely repopulate the trigger IDs below based on whichever events are still active?)
    if (PLAYER_EXTRA().lastUsedItem != InvItemId_Unequipped)
    {
        for (i = 0; PLAYER_EXTRA().lastUsedItem != g_ItemTriggerItemIds[i]; i++);

        g_MapEventData         = g_ItemEvents[i];
        g_MapEventLastUsedItem = PLAYER_EXTRA().lastUsedItem;
        g_MapEventSysState     = g_MapEventData->sysState;
        g_MapEventParam        = g_MapEventData->eventParam;

        PLAYER_EXTRA().lastUsedItem = InvItemId_Unequipped;
        Event_ItemTriggersClear();
        return;
    }

    Event_ItemTriggersClear();

    g_MapEventLastUsedItem = InvItemId_Unequipped;

    mapEvent = &g_MapOverlayHdr.mapEvents[-1];

    while (true)
    {
        s32 completeFlag_temp;
        s16 completeFlag;
        s16 requiredFlag;

        mapEvent++;

        if (mapEvent->triggerType == NO_VALUE)
        {
            break;
        }

        // `requiredFlag`: EventFlag that must be set for event to trigger.
        // `completeFlag`: skips triggering event if EventFlag is set, otherwise will set this flag once event has completed.

        // TODO: Can this s32 temp be removed? Trying to set `completeFlag` directly results in `lhu` instead?
        requiredFlag      = mapEvent->requiredFlag;
        completeFlag_temp = mapEvent->completeFlag;
        completeFlag      = completeFlag_temp;

        if (requiredFlag != EventFlag_None && !Savegame_EventFlagGet(requiredFlag))
        {
            continue;
        }

        if (completeFlag != EventFlag_None && Savegame_EventFlagGet(completeFlag) &&
            // Completed map marker events with non-Exclusive activation
            // and non-EventSetFlag state are allowed to re-trigger?
            (completeFlag >= EventFlag_MapMark_Begin &&
            mapEvent->activationType != EventActivationType_Exclusive &&
            mapEvent->sysState != SysState_EventSetFlag) == false)
        {
            continue;
        }

        // `EventTriggerType_Tick` skips trigger/activation checks and executes every tick if flag checks are satisfied.
        // Maybe used for map-load events, and events that should run every frame?
        // Will skip processing any later events until flag checks above disable it.
        if (mapEvent->triggerType == EventTriggerType_Tick)
        {
            g_MapEventData     = mapEvent;
            g_MapEventSysState = mapEvent->sysState;
            g_MapEventParam    = mapEvent->eventParam;
            return;
        }

        // `EventActivationType_Button`: Only continue processing event when action button is pressed and
        // `Player_IsBusy` returns `false`.
        if (mapEvent->activationType == EventActivationType_Button &&
            (!Pad_ButtonClickedBind(action) ||
            disableButtonEvents || Player_IsBusy()))
        {
            continue;
        }

        mapPoint = &g_MapOverlayHdr.mapPoints[mapEvent->mapPointIdx];

        switch (mapEvent->triggerType)
        {
            case EventTriggerType_TouchAabb:
                pointPosX    = mapPoint->positionX;
                pointPosZ    = mapPoint->positionZ;
                pointRadiusX = mapPoint->triggerParam0 * Q12(0.25f);
                pointRadiusZ = mapPoint->triggerParam1 * Q12(0.25f);

                if (ABS(PLAYER_CHARA().position.vx - pointPosX) > pointRadiusX)
                {
                    continue;
                }

                if (ABS(PLAYER_CHARA().position.vz - pointPosZ) > pointRadiusZ)
                {
                    continue;
                }
                break;

            case EventTriggerType_TouchFacing:
                if (!Event_CollideFacingCheck(mapPoint))
                {
                    continue;
                }
                break;

            case EventTriggerType_TouchObbFacing:
                if (!Event_CollideObbFacingCheck(mapPoint))
                {
                    continue;
                }
                break;

            case EventTriggerType_TouchObb:
                if (!Event_CollideObbCheck(mapPoint))
                {
                    continue;
                }
                break;
        }

        // Trigger checks have passed. Check activation type.

        // `EventActivationType_Exclusive`: Skip processing any other events if this event is active.
        if (mapEvent->activationType == EventActivationType_Exclusive && mapEvent == g_MapEventData)
        {
            g_MapEventSysState = SysState_Invalid;
            return;
        }

        // `EventActivationType_Item`: When trigger check has passed (player is in the trigger area).
        // Required item ID for event is stored into `g_ItemTriggerItemIds` and event pointer at `g_ItemEvents`
        // Once player uses an item in the inventory screen, it compares the ID against the ones stored at `g_ItemTriggerItemIds`.
        // If used item ID matches one that event has requested, `extra.lastUsedItem` gets set to the item ID.
        // At the start of this function, if `extra.lastUsedItem` is set, it will locate the `s_EventData` for it from `g_ItemEvents` and run the event.
        if (mapEvent->activationType == EventActivationType_Item)
        {
            for (i = 0; g_ItemTriggerItemIds[i] != NO_VALUE; i++); // @hack

            g_ItemEvents[i]      = mapEvent;
            g_ItemTriggerItemIds[i] = mapEvent->requiredItemId;
            continue;
        }

        // `EventActivationType_Button`: Only allow button activated events when area is lit up?
        if (mapEvent->activationType == EventActivationType_Button)
        {
            if (!WorldEnv_IsWorldLit())
            {
                if (mapEvent->sysState != SysState_LoadOverlay && (mapEvent->sysState != SysState_LoadRoom && mapEvent->eventParam > 1))
                {
                    continue;
                }
            }
        }

        // Trigger and activation checks passed. Run event.

        // If this is `EventSetFlag`, handle setting the flag here and skip running it.
        // (Same as `SysState_EventSetFlag_Update`.)
        if (mapEvent->sysState == SysState_EventSetFlag)
        {
            Savegame_EventFlagSetAlt(mapEvent->completeFlag);
            break;
        }

        // Set `g_MapEventSysState` to the SysState needed for the event to be ran on next tick (`SysState_ReadMessage`/`SaveMenu`/`EventCallFunc`/etc.).
        g_MapEventData     = mapEvent;
        g_MapEventSysState = mapEvent->sysState;
        g_MapEventParam    = mapEvent->eventParam;
        return;
    }

    g_MapEventData     = NULL;
    g_MapEventSysState = SysState_Invalid;
    g_MapEventParam    = 0;
}

bool Event_CollideFacingCheck(s_MapPoint2d* mapPoint) // 0x800378D4
{
    q19_12 deltaX;
    q19_12 deltaZ;
    q3_12  rotY;
    q19_12 deltaRotY;

    static q19_12 D_800A9A20 = Q12(0.0f);
    static q19_12 D_800A9A24 = Q12(0.0f);
    static q19_12 D_800A9A28 = Q12(0.0f);

    if (g_TickCount > D_800A9A20)
    {
        rotY       = PLAYER_CHARA().rotation.vy;
        D_800A9A24 = PLAYER_CHARA().position.vx - DIV_FAST(Math_Sin(rotY), 8);
        D_800A9A28 = PLAYER_CHARA().position.vz - DIV_FAST(Math_Cos(rotY), 8);
        D_800A9A20 = g_TickCount;
    }

    deltaX = mapPoint->positionX - D_800A9A24;
    if (ABS(deltaX) > Q12(0.8f))
    {
        return false;
    }

    deltaZ = mapPoint->positionZ - D_800A9A28;
    if (ABS(deltaZ) > Q12(0.8f))
    {
        return false;
    }

    if ((SQUARE(deltaX) + SQUARE(deltaZ)) > SQUARE(Q12(0.8f)))
    {
        return false;
    }

    deltaRotY = PLAYER_CHARA().rotation.vy - ratan2(deltaX, deltaZ);
    if (deltaRotY >= Q12_ANGLE(180.0f))
    {
        deltaRotY -= Q12_ANGLE(360.0f);
    }

    if (Q12_ANGLE(30.0f) < ABS(deltaRotY))
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Event_CollideObbFacingCheck(s_MapPoint2d* mapPoint) // 0x80037A4C
{
    s32    temp_a0_2;
    s32    temp_a2;
    q19_12 halfCosPlayerRotY;
    s32    temp_s2;
    s32    halfSinRotY;
    s32    temp_s4;
    q19_12 deltaX;
    q19_12 deltaZ;
    s32    temp_v1;
    q19_12 clampedHalfCosPlayerRotY;
    bool   cond;
    s32    scaledSinPlayerRotY;
    s32    scaledCosRotY;

    halfSinRotY   = DIV_FAST(Math_Sin(PLAYER_CHARA().rotation.vy), 2);
    scaledCosRotY = -Math_Cos(Q12_ANGLE_FROM_Q8(mapPoint->triggerParam0)) * mapPoint->triggerParam1;

    clampedHalfCosPlayerRotY = halfSinRotY;

    temp_a0_2 = DIV_FAST(scaledCosRotY, 16);
    deltaX    = mapPoint->positionX - PLAYER_CHARA().position.vx;
    temp_s2   = deltaX - temp_a0_2;
    temp_s4   = deltaX + temp_a0_2;

    clampedHalfCosPlayerRotY = MAX(halfSinRotY, 0);

    if (temp_s4 >= temp_s2)
    {
        cond = clampedHalfCosPlayerRotY < temp_s2;
    }
    else
    {
        cond = clampedHalfCosPlayerRotY < temp_s4;
    }

    if (!cond)
    {
        if (MIN(halfSinRotY, 0) <= MAX(temp_s2, temp_s4))
        {
            halfCosPlayerRotY   = DIV_FAST(Math_Cos(PLAYER_CHARA().rotation.vy), 2);
            scaledSinPlayerRotY = Math_Sin(Q12_ANGLE_FROM_Q8(mapPoint->triggerParam0)) *
                                  mapPoint->triggerParam1;

            clampedHalfCosPlayerRotY = halfCosPlayerRotY;

            temp_a0_2 = DIV_FAST(scaledSinPlayerRotY, 16);
            deltaZ    = mapPoint->positionZ - PLAYER_CHARA().position.vz;
            temp_v1   = deltaZ - temp_a0_2;
            temp_a2   = deltaZ + temp_a0_2;

            clampedHalfCosPlayerRotY = MAX(halfCosPlayerRotY, Q12(0.0f));

            if (temp_a2 >= temp_v1)
            {
                cond = clampedHalfCosPlayerRotY < temp_v1;
            }
            else
            {
                cond = clampedHalfCosPlayerRotY < temp_a2;
            }

            if (!cond)
            {
                if (MIN(halfCosPlayerRotY, 0) <= MAX(temp_v1, temp_a2) &&
                   (((temp_s2 * halfCosPlayerRotY) - (halfSinRotY * temp_v1)) <= 0) && ((temp_s4 * halfCosPlayerRotY) - (halfSinRotY * temp_a2)) >= 0 &&
                   ((-temp_s2 * (temp_a2 - temp_v1)) + ((temp_s4 - temp_s2) * temp_v1)) >= 0)
                {
                    return (((halfSinRotY - temp_s2) * (temp_a2 - temp_v1)) -
                            ((temp_s4 - temp_s2) * (halfCosPlayerRotY - temp_v1))) < 1;
                }
            }
        }
    }

    return false;
}

bool Event_CollideObbCheck(s_MapPoint2d* mapPoint) // 0x80037C5C
{
    q19_12 sinAngle;
    q19_12 cosAngle;
    q19_12 angle;
    q19_12 deltaX;
    q19_12 deltaZ;
    s32    shift8Field_7;
    s32    temp_v0;
    s32    scale;
    u32    temp;

    shift8Field_7 = mapPoint->triggerParam1 << 8;

    deltaX = PLAYER_CHARA().position.vx - mapPoint->positionX;
    if (mapPoint->triggerParam1 << 9 < ABS(deltaX))
    {
        return false;
    }

    deltaZ = PLAYER_CHARA().position.vz - mapPoint->positionZ;
    scale  = 2;
    if ((shift8Field_7 * scale) < ABS(deltaZ))
    {
        return false;
    }

    // TODO: Odd Q8 angle conversion method. `Q12_ANGLE_FROM_Q8` doesn't match here.
    angle    = -(mapPoint->triggerParam0 << 20) >> 16;
    sinAngle = Math_Sin(angle);

    temp = FP_FROM((-deltaX * sinAngle) + (deltaZ * Math_Cos(angle)), Q12_SHIFT);
    if (temp > Q12(4.0f))
    {
        return false;
    }

    cosAngle = Math_Cos(angle);
    temp_v0  = FP_FROM((deltaX * cosAngle) + (deltaZ * Math_Sin(angle)), Q12_SHIFT);
    if (shift8Field_7 < ABS(temp_v0))
    {
        return false;
    }
    else
    {
        return true;
    }
}
