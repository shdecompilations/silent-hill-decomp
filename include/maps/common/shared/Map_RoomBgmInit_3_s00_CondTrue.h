void Map_RoomBgmInit_CondTrue(void)
{
    #define OVERLAY_VAL_ON  0x10
    #define OVERLAY_VAL_OFF 0x19

    s32 mapOverlayVal;

    mapOverlayVal = !Savegame_EventFlagGet(EventFlag_295) ? OVERLAY_VAL_OFF : OVERLAY_VAL_ON;
    Bgm_SongChange(mapOverlayVal);
}
