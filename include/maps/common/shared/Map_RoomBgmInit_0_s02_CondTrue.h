void Map_RoomBgmInit_CondTrue(void)
{
    switch (g_SavegamePtr->mapRoomIdx)
    {
        case 5:
            Bgm_SongChange(BgmCmd_Track6);
            break;

        case 18:
            Bgm_SongChange(!Savegame_EventFlagGet(EventFlag_379) ? BgmCmd_Track29 : BgmCmd_Track7);
            break;

        case 17:
            Bgm_SongChange(BgmCmd_Track7);
            break;
    }
}
