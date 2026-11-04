void Map_RoomBgmInit_CondTrue(void)
{
    Bgm_SongChange(!Savegame_EventFlagGet(EventFlag_237) ? 8 : 2);
}
