void MapEvent_DoorJammed(void)
{
    VECTOR3 sfxPos = { MAP_POINTS[g_MapEventData->mapPointIdx].positionX, Q12(-1.2f), MAP_POINTS[g_MapEventData->mapPointIdx].positionZ };

    Event_DisplayMapMsgWithSfx(MapMsgIdx_DoorJammed, Sfx_DoorJammed, &sfxPos);
}
