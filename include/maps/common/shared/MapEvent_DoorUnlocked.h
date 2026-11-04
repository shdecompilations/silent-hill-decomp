void MapEvent_DoorUnlocked(void)
{
    VECTOR3 sfxPos = { MAP_POINTS[g_MapEventData->mapPointIdx].positionX, Q12(-1.2f), MAP_POINTS[g_MapEventData->mapPointIdx].positionZ };

    Event_DisplayMapMsgWithSfx(MapMsgIdx_DoorUnlocked, Sfx_DoorUnlocked, &sfxPos);
}
