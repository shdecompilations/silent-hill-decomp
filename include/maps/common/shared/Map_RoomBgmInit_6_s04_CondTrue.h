void Map_RoomBgmInit_CondTrue(void)
{
    Bgm_SongChange(Savegame_EventFlagGet(EventFlag_440) ? 39 : 37);
}
