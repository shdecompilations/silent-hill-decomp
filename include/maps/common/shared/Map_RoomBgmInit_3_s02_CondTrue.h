void Map_RoomBgmInit_CondTrue(void)
{
    Bgm_SongChange(!Savegame_EventFlagGet(EventFlag_237) ? BgmCmd_Track8 : BgmCmd_Track2);
}
