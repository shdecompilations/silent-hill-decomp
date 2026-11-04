#include "bodyprog/bodyprog.h"

#include "bodyprog/events/event_data.h"

const s32 __pad_rodata_800252B8 = 0;

/** @brief SFX pairs.
 *
 * @note Used when loading areas. Maybe facilitates things like the
 * opening and closing door SFX when the player moves between rooms. */
const s_AreaLoadSfx SFX_PAIRS[25] = {
    { Sfx_Base, Sfx_Base },
    { Sfx_DoorOpen0, Sfx_DoorClose0 },
    { Sfx_Unk1323, Sfx_Unk1324 },
    { Sfx_Unk1418, Sfx_Base },
    { Sfx_Unk1354, Sfx_Base },
    { Sfx_Unk1387, Sfx_Base },
    { Sfx_Unk1391, Sfx_Base },
    { Sfx_Unk1521, Sfx_Base },
    { Sfx_Unk1458, Sfx_Unk1459 },
    { Sfx_Unk1604, Sfx_Unk1605 },
    { Sfx_Unk1609, Sfx_Unk1610 },
    { Sfx_Unk1396, Sfx_Unk1397 },
    { Sfx_Unk1336, Sfx_Unk1337 },
    { Sfx_Unk1338, Sfx_Unk1339 },
    { Sfx_Unk1340, Sfx_Unk1341 },
    { Sfx_Unk1601, Sfx_Unk1602 },
    { Sfx_Unk1490, Sfx_Unk1491 },
    { Sfx_Unk1431, Sfx_Unk1432 },
    { Sfx_Unk1398, Sfx_Unk1399 },
    { Sfx_Unk1504, Sfx_Unk1505 },
    { Sfx_DoorOpen0, Sfx_Base },
    { Sfx_Unk1323, Sfx_Base },
    { Sfx_Base, Sfx_Unk1324 },
    { Sfx_Unk1351, Sfx_Unk1352 },
    { Sfx_Unk1487, Sfx_Base }
};

/** @brief Relative paper map TIM file indices. */
s8 g_PaperMapFileIdxs[] = {
    0, 0, 1, 1, 9, 12, 10, 11,
    13, 12, 10, 11, 13, 2, 3, 4,
    8, 5, 6, 7, 8, 5, 6, 7
};

/** @brief Relative paper map marking TIM file indices. */
s8 g_PaperMapMarkingFileIdxs[] = {
    255, 0, 1, 1, 1, 4, 4, 4,
    4, 4, 4, 4, 4, 2, 2, 2,
    3, 3, 3, 3, 3, 3, 3, 3
};

// TODO: Make enum for access?
char* g_CommonWorldObjectNames[8] = {
    NULL,
    "PAD_NEAR",
    "AIDKIT_N",
    "DRINK_NE",
    "AMPULE_N",
    "BULLET_N",
    "SHELL_NE",
    "SHOT_NEA"
};

/** @note Related to images display. */
#if !VERSION_REGION_IS(PAL)
s_FsImageDesc D_800A9A04             = { .tPage = { 1, 21 }, .clutX = 224, .clutY = 13 };
#endif
bool          g_IsLoadingFinished             = false;
s32           g_MapEventSysState     = SysState_Gameplay;
u32           g_MapEventParam        = 0;
s32           g_MapEventLastUsedItem = 0;
s32           g_RadioPitchState      = 0;

const s32 __pad_rodata_80025374 = 0;
