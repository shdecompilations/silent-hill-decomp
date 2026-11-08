#include "game.h"

#include <psyq/libetc.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "bodyprog/sys/memcard.h"
#include "bodyprog/screen/data.h"
#include "bodyprog/screen/fade.h"
#include "bodyprog/text/text_draw.h"
#include "bodyprog/game_boot/game_boot.h"
#include "screens/saveload.h"

#define SAVE_FLASH_TIMER_MAX 40
#define SLOT_COLUMN_OFFSET   150
#define SLOT_ROW_OFFSET      20

// TODO: Not sure if calling them "layers" is accurate. -- Sezz
#define LAYER_24 24
#define LAYER_28 28
#define LAYER_32 32
#define LAYER_36 36

#if !VERSION_REGION_IS(NTSCJ)
const s32 pad = 0;
#endif

// TODO: Temporary hack while JAP data is unmigrated, remove this once data is brought over.
#if VERSION_REGION_IS(NTSCJ)
    #define SAVE_BSS extern
#else
    #define SAVE_BSS static
#endif

#if VERSION_REGION_IS(NTSCJ)
// Strings live in the overlay rodata, in the part included from assembly.
char* g_Savegame_SaveLocationNames[] = {
    (char*)0x801E2714,
    (char*)0x801E270C,
    (char*)0x801E2704,
    (char*)0x801E26F8,
    (char*)0x801E26E8,
    (char*)0x801E26D8,
    (char*)0x801E26C8,
    (char*)0x801E26C0,
    (char*)0x801E26B4,
    (char*)0x801E26AC,
    (char*)0x801E26A0,
    (char*)0x801E2690,
    (char*)0x801E2684,
    (char*)0x801E267C,
    (char*)0x801E266C,
    (char*)0x801E2664,
    (char*)0x801E265C,
    (char*)0x801E2654,
    (char*)0x801E2644,
    (char*)0x801E2638,
    (char*)0x801E2630,
    (char*)0x801E2628,
    (char*)0x801E2620,
    (char*)0x801E2614,
    (char*)0x801E2604
};
#else
char* g_Savegame_SaveLocationNames[] = {
    "Anywhere",
    "Cafe",
    "Bus",
    "Store",
    "Infirmary",
    "Doghouse",
    "Gordon",
    "Church",
    "Garage",
    "Police",
    "Reception",
    "Room_302",
    "Director's",
    "Jewelry_shop",
    "Pool_hall",
    "Antique_shop",
    "Theme_park",
    "Boat",
    "Bridge",
    "Motel",
    "Lighthouse",
    "Sewer",
    "Nowhere",
    "Child's_room",
    "Next_fear"
};
#endif

/** @brief Reduntant code. Some overwrite status.
 * Indicates if the player is or have overwritten
 * a save.
 *
 * This is used in a switch statement from
 * `SaveScreen_WriteOptionsStepDraw` with two cases:
 ** 0: Prepares to draw overwritting options message.
 ** 1: Draws overwritting options message.
 * However, the preparation is unnecessary as the only
 * useful thing it does is assigning the value of `0`
 * to `g_SaveScreen_MemCardStateDisplay`, the other
 * two things the code does are useless, most noticiably
 * it assign itself the value of `1`, however, this
 * is useless as the case 0 have fallthrough to the case 1.
 */
s32 g_SaveScreen_OverwriteActive = 0;

/** @brief Reduntant code. Some display state status.
 * Indicates if memory card or a save state is
 * or have been display.
 *
 * This is used in a switch statement from
 * `SaveScreen_MemCardStateDraw` with two cases:
 ** 0: Prepares to draw process status message.
 ** 1: Draws process status message.
 * However, the preparation is unnecessary as the only
 * useful thing it does is assigning the value of `0`
 * to `g_SaveScreen_OverwriteActive` and updates some
 * global variables in case the memory card have been
 * disconnected during the load of a save, the other
 * two things the code does are useless. It assign itself
 * the value of `1`, however, the case 0 have fallthrough
 * to case 1 and it assign to `D_801E7554` the value
 * of the string index to be display, but it only used as
 * an unnecessary double check for updating the message
 * that indicates a save have been done successfully.
 */
s32 g_SaveScreen_MemCardStateDisplay = 0;

/** @brief Dead code
 * Some boolean statuses for each save slot.
 *
 * This seems to be related to the color of the
 * borders of the file, but appears to do nothing
 * as the code only defines it as 1 or 0.
 * It might also be buggy as it constantly oscillates
 * between 0 and 1. For example:
 * in `SaveScreen_SavesSlotDraw`, the code has a conditional
 * that writes 1, but a little later
 * in the same function, it writes 1 again
 * without any conditional.
 */
s16 D_801E7514[MEMCARD_SLOT_COUNT_MAX] = { false, false };

/** @brief Broken code.
 * This is used in some way to determine the y position
 * of some save/memcard states strings from `e_SavegameEntryType`.
 * This code works in a strange way as it is multiplied by 20
 * and then it get 20 added, each time the function
 * `SaveScreen_SavesSlotDraw` is run it get +1 added to it's value until
 * it reaches 5 or if one of the elements to be draw is the last
 * element of the memory card.
 */
s16 D_801E7518[MEMCARD_SLOT_COUNT_MAX] = { 0, 0 };

s32 g_SaveScreen_State = SaveScreenState_None; /** `e_SaveScreenState` */

/** @brief Timer used when displaying the text after finishing any of the following states:
 * - Formatting
 * - Saving
 * - Loading
 * - Error
 */
s32 g_SaveScreen_MemCardStateTextTimer = 0;

// Only used in `GameState_LoadSavegameScreen_Update`.
void (*g_GameState_SaveScreen_Funcs[])() = {
    SaveScreen_Init,
    SaveScreen_LogicUpdate,
    SaveScreen_FormatCard,
    SaveScreen_SaveGame,
    SaveScreen_LoadSave,
    SaveScreen_Continue
};

bool g_SaveScreen_IsFormatting = false; // `false` - Overwrite, `true` - Format.

bool g_SaveScreen_IsNewSaveSelected = false; // `false` - User has `New save` selected, `true` - User has a save selected.

// Only used in `GameState_AutoLoadSavegame_Update`.
void (*g_GameState_AutoLoadSavegame_Funcs[])() = {
    SaveScreen_Init,
    SaveScreen_AutoLoadSaveSelect,
    SaveScreen_LoadSave,
    SaveScreen_Continue
};

SAVE_BSS s32 D_801E7560; // Unused/Pad.

/** @brief @unused Dead code.
 * Only used when booting the save screen where both
 * values are defined as 0, the memory card is being
 * loaded, or the user has the last save selected in
 * the slot (where it asks if the value corresponding
 * to the slot is 0 to then overwrite it with 1).
 *
 * Used in: `SaveScreen_MemCardInfoReset` and `SaveScreen_SavesSlotDraw`.
 */
SAVE_BSS s32 D_801E7564[MEMCARD_SLOT_COUNT_MAX];

/** @brief @unused Dead code.
 * Constantly updates changing the value from 0 up to
 * the amount of files available in the slot.
 *
 * Code indicate that it is related the draw of file borders color.
 */
SAVE_BSS s16 D_801E756C[MEMCARD_SLOT_COUNT_MAX];

/** @brief Count of saves hidden as the user scrolls down
 * through the displayed saves in the slot.
 */
SAVE_BSS s16 g_SaveScreen_HiddenSaves[MEMCARD_SLOT_COUNT_MAX];

/** @brief @unused Dead code.
 * The only usage it is given is to check it is in `SaveScreen_SavesSlotDraw`
 * where it is used to check if the value is not the same as
 * `g_SaveScreen_HiddenSaves` if the condition is true then it assigns
 * `D_801E7514` a true value. However, the only instance where a value
 * is assigned is when booting the save screen when it get assigned
 * the value of -1 so it's always realizing the conditional.
 * (TODO: JPN makes use of it in `SaveScreen_SavesSlotDraw` - comment may need 
 * updating)
 */
SAVE_BSS s16 D_801E7574[MEMCARD_SLOT_COUNT_MAX];

#if VERSION_REGION_IS(NTSCJ)
/** @brief Savegame index whose location name glyphs are currently rendered for each
 * visible row of each slot, `NO_VALUE` if none. Lets `SaveScreen_SaveLocationDraw`
 * only render a row's glyphs when the element shown in it changes.
 */
SAVE_BSS s16 g_SaveScreen_LocationGlyphSaveIdx[MEMCARD_SLOT_COUNT_MAX][5];
#endif

/** @brief Stores the index of the element selected in each
 * slot based on what is visually available in the slot.
 *
 * For example: if the user has 9 savegames in the slot and
 * wants to create a new, separate savegame, the index of the
 * `New Save` element will be 4 and not 9 (index starting from 0)
 * as it is the fifth element visually available in the slot.
 */
SAVE_BSS s16 g_SaveScreen_VisualElementIdx[MEMCARD_SLOT_COUNT_MAX];

SAVE_BSS s8 D_801E757C[8]; // Unused/Pad.

/** @brief @unused Dead code in USA. (TODO: JPN makes more use of it) */
SAVE_BSS u8 D_801E7584[MEMCARD_SLOT_COUNT_MAX][SAVEGAME_COUNT_MAX];

/** @brief Stores the index of the last save done.
 *
 * Depending on the slot where the last save was done
 * the value representing the other slot is turn into -1.
 */
SAVE_BSS s8 g_SaveScreen_LastSaveIdx[MEMCARD_SLOT_COUNT_MAX];

/** @brief Boolean. State if save information should be displayed. */
SAVE_BSS s8 g_SaveScreen_DisplaySaveInfo;

SAVE_BSS s8 D_801E76D1; // Unused/Pad.

SAVE_BSS u8 g_SaveScreen_IsMemCardNotInserted[MEMCARD_SLOT_COUNT_MAX];

SAVE_BSS s8 g_SaveScreen_SaveFlashTimer;

SAVE_BSS u8 g_SaveScreen_IsNextFearMode;

SAVE_BSS u8 g_SaveScreen_IsGameSaving;

#if VERSION_REGION_IS(NTSCJ)
INCLUDE_RODATA("screens/saveload/nonmatchings/saveload", D_801E2600);

#endif

void SaveScreen_ScreenInfoClear(void) // 0x801E2D8C
{
    s32 i;
    s32 j;

#if VERSION_REGION_IS(NTSCJ)
    const char* digitStr = "０１２３４５６７８９";
    const char* glyphStrs[2] = {
        "差込口１　差込口２　",
        "ファイル　　ファイル　　はい　　　いいえ"
    };
#endif

    func_80037124();

    g_SaveScreen_OverwriteActive     = 0;
    g_SaveScreen_MemCardStateDisplay = 0;
    g_SaveScreen_SaveFlashTimer      = SAVE_FLASH_TIMER_MAX;
    g_SaveScreen_IsGameSaving        = 0;

    for (i = 0; i < MEMCARD_SLOT_COUNT_MAX; i++)
    {
        g_SaveScreen_IsMemCardNotInserted[i] = true;
        g_SaveScreen_LastSaveIdx[i]          = NO_VALUE;
        D_801E7564[i]                        = 0;
        D_801E756C[i]                        = 0;
        g_SaveScreen_HiddenSaves[i]          = NO_VALUE;
        D_801E7574[i]                        = NO_VALUE;
        g_SaveScreen_VisualElementIdx[i]     = 0;
        D_801E7514[i]                        = true;
        D_801E7518[i]                        = 0;

#if VERSION_REGION_IS(NTSCJ)
        for (j = 0; j < 5; j++)
        {
            g_SaveScreen_LocationGlyphSaveIdx[i][j] = NO_VALUE;
        }
#endif

        for (j = 0; j < SAVEGAME_COUNT_MAX; j++)
        {
            D_801E7584[i][j] = 0;
        }

        g_SaveScreen_DisplaySaveInfo = true;
    }

#if VERSION_REGION_IS(NTSCJ)
    D_800C3920 = 10;
    func_8004C918(digitStr,     1, 0, 13);

    func_8004C918(glyphStrs[0], 1, 0, 9);

    D_800C3920 = 20;
    func_8004C918(glyphStrs[1], 1, 0, 12);
#endif
}

void SaveScreen_SlotStrAndBottomRectDraw(void) // 0x801E2EBC
{
    s_Line2d line;
    s32      i;

#if VERSION_REGION_IS(NTSCJ)
    GsOT*     ot;
    POLY_FT4* poly;
#else
    const DVECTOR SLOT_STR_POS_TABLE[] =
    {
        { 59, 16 },
        { 209, 16 }
    };

    const char* SLOT_STRS[] = {
        "SLOT1",
        "SLOT2"
    };
#endif

#if VERSION_REGION_IS(NTSCJ)
    ot = &g_OrderingTable2[g_ActiveBufferIdx];

    // Draw SLOT1/SLOT2 label sprites.
    for (i = 0; i < MEMCARD_SLOT_COUNT_MAX; i++)
    {
        poly = (POLY_FT4*)GsOUT_PACKET_P;
        setPolyFT4(poly);
        poly->tpage = getTPage(0, 0, 128, 256);
        poly->clut  = getClut(304, 510);
        
        setRGB0(poly, 0x80, 0x80, 0x80);

        if (i != 0)
        {
            setUV4(poly, 120, 224, 120, 240,
                         168, 224, 168, 240);
            setXY4(poly, -96, -96, -96, -80,
                         -48, -96, -48, -80);
        }
        else
        {
            setUV4(poly, 180, 224, 180, 240,
                         228, 224, 228, 240);
            setXY4(poly, 52,  -96, 52,  -80,
                         100, -96, 100, -80);
        }

        addPrim((u8*)ot->org + LAYER_24, poly);
        GsOUT_PACKET_P = (u8*)poly + sizeof(POLY_FT4);
    }
#else
    Gfx_StringColorSet(StringColorId_White);

    for (i = 0; i < MEMCARD_SLOT_COUNT_MAX; i++)
    {
        Gfx_StringPositionSet(SLOT_STR_POS_TABLE[i].vx, SLOT_STR_POS_TABLE[i].vy);
        Gfx_StringDraw(SLOT_STRS[i], 50);
    }
#endif

    line.vertex0.vx = -136;
    line.vertex0.vy = 60;
    line.vertex1.vx = 272;
    line.vertex1.vy = 40;
    SaveScreen_RectSaveInfoDraw(&line);
}

void SaveScreen_SelectedElementIdxUpdate(s32 slotIdx, s32 unused) // 0x801E2F90
{
    g_SaveScreen_VisualElementIdx[slotIdx] = g_SlotElementSelectedIdx[slotIdx] - g_SaveScreen_HiddenSaves[slotIdx];
}

void SaveScreen_FileIdxDraw(s32 saveIdx, s32 slotIdx, s32 fileId, s32 entryType) // 0x801E2FCC
{
    #define OFFSET_X             150
    #define FILE_STR_MARGIN_X    (SCREEN_WIDTH / 10)
    #define FILE_ID_STR_MARGIN_X (FILE_STR_MARGIN_X + 50)
    #define POS_Y                35

#if VERSION_REGION_IS(NTSCJ)
    GsOT* ot = &g_OrderingTable2[g_ActiveBufferIdx];
#else
    const char* FILE_STR = "FILE";
#endif

    if (saveIdx == g_SlotElementSelectedIdx[slotIdx] && entryType >= SavegameEntryType_OutOfBlocks)
    {
#if VERSION_REGION_IS(NTSCJ)
        RECT      rect;
        POLY_FT4* poly;

        if (fileId / 10 != 0)
        {
            setRECT(&rect, ((fileId / 10) * 3) + 158, 16, 3, 16);
            MoveImage(&rect, (slotIdx * 18) + 268, 480);
        }
        else
        {
            setRECT(&rect, (slotIdx * 18) + 268, 480, 3, 16);
            ClearImage(&rect, 0, 0, 0);
        }

        setRECT(&rect, ((fileId % 10) * 3) + 158, 16, 3, 16);
        MoveImage(&rect, (slotIdx * 18) + 271, 480);

        poly = (POLY_FT4*)GsOUT_PACKET_P;
        setPolyFT4(poly);
        poly->tpage = getTPage(0, 0, 256, 256);
        poly->clut  = getClut(304, 510);

        setRGB0(poly, 0x80, 0x80, 0x80);
        setUV4(poly, slotIdx * 72,        224, slotIdx * 72,        240,
                     (slotIdx * 72) + 72, 224, (slotIdx * 72) + 72, 240);
        setXY4(poly, (slotIdx * OFFSET_X) - 130, -78, (slotIdx * OFFSET_X) - 130, -62,
                     (slotIdx * OFFSET_X) - 58,  -78, (slotIdx * OFFSET_X) - 58,  -62);

        addPrim((u8*)ot->org + LAYER_24, poly);
        GsOUT_PACKET_P = (u8*)poly + sizeof(POLY_FT4);
#else
        Gfx_StringColorSet(StringColorId_White);

        // Draw "FILE" string.
        Gfx_StringPositionSet((slotIdx * OFFSET_X) + FILE_STR_MARGIN_X, POS_Y);
        Gfx_StringDraw(FILE_STR, 50);

        // Draw file ID string.
        Gfx_StringPositionSet((slotIdx * OFFSET_X) + FILE_ID_STR_MARGIN_X, POS_Y);
        Gfx_StringDrawInt(1, fileId);
#endif
    }

    #undef OFFSET_X
    #undef FILE_STR_MARGIN_X
    #undef FILE_ID_STR_MARGIN_X
    #undef POS_Y
}

bool SaveScreen_NextFearModeSave(s_MemCard_SaveMetadata* saveEntry) // 0x801E3078
{
    if (saveEntry != NULL && saveEntry->isNextFearMode)
    {
        Gfx_StringColorSet(StringColorId_Gold);
        return true;
    }

    Gfx_StringColorSet(StringColorId_White);
    return false;
}

void SaveScreen_SaveLocationDraw(s_SaveScreenElement* saveEntry, s32 saveIdx, s32 slotIdx) // 0x801E30C4
{
    #define OFFSET_X 150
    #define MARGIN_X 90
    #define OFFSET_Y (SCREEN_HEIGHT / 12)
    #define MARGIN_Y 53

    s32 nameIdx = saveEntry->locationId;
    s32 selectedSaveIdx;
    s16 hiddenSaveCount;

#if !VERSION_REGION_IS(NTSCJ)
    s32 colorId;

    const u8 X_OFFSETS[] = {
        82, 37,  30, 44,
        81, 81,  61, 61,
        59, 51,  84, 81,
        81, 109, 75, 111,
        97, 38,  55, 45,
        95, 48,  71, 102,
        77
    };
#endif

    hiddenSaveCount = g_SaveScreen_HiddenSaves[slotIdx];

    if (saveIdx >= hiddenSaveCount && (hiddenSaveCount + 4) >= saveIdx)
    {
        selectedSaveIdx = saveIdx - hiddenSaveCount;

#if VERSION_REGION_IS(NTSCJ)
        if (g_SaveScreen_LocationGlyphSaveIdx[slotIdx][selectedSaveIdx] != saveIdx)
        {
            g_SaveScreen_LocationGlyphSaveIdx[slotIdx][selectedSaveIdx] = saveIdx;

            D_800C3920 = (s32)strlen(g_Savegame_SaveLocationNames[nameIdx]) / 2;
            func_8004C918(g_Savegame_SaveLocationNames[nameIdx], (slotIdx * 5) + selectedSaveIdx, 0, 10);
        }
#else
        SaveScreen_NextFearModeSave(saveEntry->saveMetadata);

        if (g_SaveScreen_IsGameSaving != 0 && g_SelectedSaveSlotIdx == slotIdx && g_SlotElementSelectedIdx[slotIdx] == saveIdx)
        {
            if (g_SaveScreen_IsNextFearMode)
            {
                colorId = StringColorId_Gold;
            }
            else
            {
                colorId = StringColorId_White;
            }
            Gfx_StringColorSet(colorId);
        }

        Gfx_StringPositionSet(((slotIdx * OFFSET_X) + MARGIN_X) - (X_OFFSETS[nameIdx] / 2),
                              (selectedSaveIdx * OFFSET_Y) + MARGIN_Y);
        Gfx_StringDraw(g_Savegame_SaveLocationNames[nameIdx], 50);
#endif
    }

    #undef OFFSET_X
    #undef MARGIN_X
    #undef OFFSET_Y
    #undef MARGIN_Y
}

void SaveScreen_SaveBorder(s_SaveScreenElement* saveEntry, s_SaveScreenElement* nextSaveEntry, s32 saveIdx, s32 slotIdx) // 0x801E326C
{
    if (saveIdx == 0)
    {
        D_801E756C[slotIdx] = 0;
    }

    if (saveIdx < g_SaveScreen_HiddenSaves[slotIdx] || (g_SaveScreen_HiddenSaves[slotIdx] + 4) < saveIdx)
    {
        if (D_801E756C[slotIdx] != saveEntry->fileIdx)
        {
            D_801E756C[slotIdx] = saveEntry->fileIdx;
        }
    }
    else
    {
        SaveScreen_SaveBorderDraw(saveEntry, nextSaveEntry, saveIdx, slotIdx);
    }
}

void SaveScreen_SavesSlotDraw(s_SaveScreenElement* saveEntry, s32 saveIdx, s32 slotIdx) // 0x801E3304
{
#if VERSION_REGION_IS(NTSCJ)
    RECT rect;
    s32 rowIdx;
    s32 isNextFearMode;
#endif

    s32 i;
    s32 entryType;

    const char* DIALOG_STRS[11] = {
#if VERSION_REGION_IS(NTSCJ)
        "~C7　メモリーカードが　　差されていません　",
        "~C7　メモリーカードが　初期化されていません",
        "~C7　メモリーカードに　　異常があります　　",
        "~C7　メモリーカードを　　チェックしています",
        "~C7　空きブロックが　　　不足しています　　",
        "~C7ファイルがありません",
        "~C7データがありません　",
        "~C7ファイルが壊れてます",
        "~C7　　　　　　　　　　",
        "~C7セーブできます　　　",
        "~C7新規ファイルにセーブ"
#else
        "\7MEMORY_CARD\nis_not_inserted",
        "\7MEMORY_CARD\nis_\x01not_\x01""formatted",
        "\7MEMORY_CARD\nmay_be_broken",
        "\7Now_checking\nMEMORY_CARD",
        "\7Out_of_blocks",
        "\7No_data_file",
        "\7No_data_file",
        "\7____Fil""\x01""e_\x01\x01is_\x01\x01""da\x01ma\x01g\x01""ed",
        "\7",
        "\7________\x01New_save",
        "\7____Crea\x01t\x01""e_\x01n\x01""e\x01w_\x01""fi\x01le"
#endif
    };

    entryType = saveEntry->type;

    if (g_SelectedSaveSlotIdx == slotIdx && saveIdx == 0 && entryType >= SavegameEntryType_CorruptedSave)
    {
        SaveScreen_SlotBorder();
    }

    if (saveIdx == 0)
    {
        SaveScreen_SaveFlash();

        g_SaveScreen_SaveFlashTimer++;
        g_SaveScreen_SaveFlashTimer = CLAMP(g_SaveScreen_SaveFlashTimer, 0, SAVE_FLASH_TIMER_MAX);

        if (entryType != SavegameEntryType_LoadMemCard)
        {
            D_801E7518[slotIdx] = 0;
        }

        if (g_SaveScreen_HiddenSaves[slotIdx] == NO_VALUE)
        {
            g_SaveScreen_HiddenSaves[slotIdx] = 0;
        }

        if (g_SaveScreen_IsMemCardNotInserted[slotIdx])
        {
#if VERSION_REGION_IS(NTSCJ)
            if (entryType != SavegameEntryType_LoadMemCard)
            {
                D_801E7564[slotIdx] = 0;
            }
#endif

            if (g_SlotElementSelectedIdx[slotIdx] == (g_Savegame_ElementCount1[slotIdx] - 2))
            {
                g_SaveScreen_HiddenSaves[slotIdx] = g_SlotElementSelectedIdx[slotIdx] - 3;
            }
            else
            {
                g_SaveScreen_HiddenSaves[slotIdx] = g_SlotElementSelectedIdx[slotIdx] - 2;
            }
        }

        if (g_Savegame_ElementCount1[slotIdx] < 6 || g_SlotElementSelectedIdx[slotIdx] == 0)
        {
            g_SaveScreen_HiddenSaves[slotIdx] = 0;
        }
        else
        {
            if ((g_Savegame_ElementCount1[slotIdx] - 2) < g_SlotElementSelectedIdx[slotIdx])
            {
                g_SaveScreen_HiddenSaves[slotIdx] = g_Savegame_ElementCount1[slotIdx] - 5;
            }
            else
            {
                while ((g_SlotElementSelectedIdx[slotIdx] - g_SaveScreen_HiddenSaves[slotIdx]) >= 4)
                {
                    g_SaveScreen_HiddenSaves[slotIdx]++;
                }

                while ((g_SlotElementSelectedIdx[slotIdx] - 1) < g_SaveScreen_HiddenSaves[slotIdx])
                {
                    g_SaveScreen_HiddenSaves[slotIdx]--;
                }
            }
        }

        if (g_SaveScreen_HiddenSaves[slotIdx] != D_801E7574[slotIdx])
        {
            D_801E7514[slotIdx] = true;
        }
    }

    switch (entryType)
    {
        case SavegameEntryType_UnformattedMemCard:
            g_SaveScreen_IsMemCardNotInserted[slotIdx] = false;

            if (g_GameWork.gameState == GameState_SaveScreen)
            {
                SaveScreen_SlotStatusMsgShineDraw(slotIdx);
            }

            break;

        case SavegameEntryType_LoadMemCard:
            g_SaveScreen_IsMemCardNotInserted[slotIdx] = true;

#if VERSION_REGION_IS(NTSCJ)
            for (i = 0; i < 5; i++)
            {
                g_SaveScreen_LocationGlyphSaveIdx[slotIdx][i] = NO_VALUE;
            }
#endif

            for (i = 0; i < SAVEGAME_COUNT_MAX; i++)
            {
                D_801E7584[slotIdx][i] = 0;
            }

#if !VERSION_REGION_IS(NTSCJ)
            g_SaveScreen_HiddenSaves[slotIdx] = NO_VALUE;
#endif
            break;

        case SavegameEntryType_CorruptedSave:
        case SavegameEntryType_Save:
        case SavegameEntryType_NewSave:
        case SavegameEntryType_NewFile:
            g_SaveScreen_IsMemCardNotInserted[slotIdx] = false;

            if (saveIdx == 0)
            {
                SaveScreen_NavigationDraw(slotIdx, g_Savegame_ElementCount1[slotIdx], g_SlotElementSelectedIdx[slotIdx], g_SaveScreen_VisualElementIdx[slotIdx]);
            }
            break;
    }

    if (saveIdx < g_SaveScreen_HiddenSaves[slotIdx] || ((g_SaveScreen_HiddenSaves[slotIdx] + 4) < saveIdx))
    {
        return;
    }

#if VERSION_REGION_IS(NTSCJ)
    // Glyph rows for this slot are cached in VRAM, only redraw them when the visible entries change.
    if (D_801E7514[slotIdx] != 0 || D_801E7584[slotIdx][saveIdx] != (u8)entryType)
    {
        if (D_801E7514[slotIdx] == 0 && D_801E7518[slotIdx] == 0)
        {
            D_801E7564[slotIdx] = 0;
        }

        if (D_801E7564[slotIdx] == 0)
        {
            func_801E4334(entryType, saveIdx, slotIdx, DIALOG_STRS[entryType]);
            D_801E7574[slotIdx] = g_SaveScreen_HiddenSaves[slotIdx];
        }
        else
        {
            if (g_SaveScreen_HiddenSaves[slotIdx] > D_801E7574[slotIdx])
            {
                // Scrolled down, shift the cached rows up.
                if (saveIdx == g_SaveScreen_HiddenSaves[slotIdx])
                {
                    for (rowIdx = 1; rowIdx < 5; rowIdx++)
                    {
                        setRECT(&rect, ((rowIdx >> 1) << 6) + ((rowIdx & 1) * 30), (slotIdx != 0) ? 480 : 16, 30, 16);
                        MoveImage(&rect, (((rowIdx - 1) >> 1) << 6) + (((rowIdx - 1) & 1) * 30), (slotIdx != 0) ? 480 : 16);

                        g_SaveScreen_LocationGlyphSaveIdx[slotIdx][rowIdx - 1] = g_SaveScreen_LocationGlyphSaveIdx[slotIdx][rowIdx];
                    }
                }

                if (g_SaveScreen_HiddenSaves[slotIdx] == (saveIdx - 4))
                {
                    func_801E4334(entryType, saveIdx, slotIdx, DIALOG_STRS[entryType]);
                    D_801E7574[slotIdx] = g_SaveScreen_HiddenSaves[slotIdx];
                    D_801E7514[slotIdx] = 0;

                    g_SaveScreen_LocationGlyphSaveIdx[slotIdx][saveIdx - g_SaveScreen_HiddenSaves[slotIdx]] = NO_VALUE;
                }
            }
            else if (g_SaveScreen_HiddenSaves[slotIdx] < D_801E7574[slotIdx])
            {
                // Scrolled up, shift the cached rows down.
                if (saveIdx == g_SaveScreen_HiddenSaves[slotIdx])
                {
                    for (rowIdx = 3; rowIdx >= 0; rowIdx--)
                    {
                        setRECT(&rect, ((rowIdx >> 1) << 6) + ((rowIdx & 1) * 30), (slotIdx != 0) ? 480 : 16, 30, 16);
                        MoveImage(&rect, (((rowIdx + 1) >> 1) << 6) + (((rowIdx + 1) & 1) * 30), (slotIdx != 0) ? 480 : 16);

                        g_SaveScreen_LocationGlyphSaveIdx[slotIdx][rowIdx + 1] = g_SaveScreen_LocationGlyphSaveIdx[slotIdx][rowIdx];
                    }

                    g_SaveScreen_LocationGlyphSaveIdx[slotIdx][saveIdx - g_SaveScreen_HiddenSaves[slotIdx]] = NO_VALUE;

                    func_801E4334(entryType, saveIdx, slotIdx, DIALOG_STRS[entryType]);
                    D_801E7574[slotIdx] = g_SaveScreen_HiddenSaves[slotIdx];
                    D_801E7514[slotIdx] = 0;
                }
            }
        }

        D_801E7584[slotIdx][saveIdx] = entryType;
    }

    isNextFearMode = SaveScreen_NextFearModeSave(saveEntry->saveMetadata);

    if (g_SaveScreen_IsGameSaving != 0 && g_SelectedSaveSlotIdx == slotIdx && g_SlotElementSelectedIdx[slotIdx] == saveIdx)
    {
        isNextFearMode = g_SaveScreen_IsNextFearMode;
    }

    if (entryType == SavegameEntryType_Save)
    {
        func_801E4128(slotIdx, saveIdx - g_SaveScreen_HiddenSaves[slotIdx],
                      (s32)strlen(g_Savegame_SaveLocationNames[saveEntry->locationId]) / 2, isNextFearMode);
    }
    else
    {
        func_8004C394(DIALOG_STRS[entryType], slotIdx, saveIdx - g_SaveScreen_HiddenSaves[slotIdx], entryType);
    }
#else
    D_801E7514[slotIdx] = true;
    Gfx_StringColorSet(StringColorId_White);

    // Draw memory card message string.
    switch (entryType)
    {
        case SavegameEntryType_NoMemCard:
        case SavegameEntryType_UnformattedMemCard:
        case SavegameEntryType_CorruptedMemCard:
        case SavegameEntryType_LoadMemCard:
            Gfx_StringPositionSet((slotIdx * SLOT_COLUMN_OFFSET) + 22, 82);
            break;

        case SavegameEntryType_NoDataInMemCard:
        case SavegameEntryType_Unk6:
            Gfx_StringPositionSet((slotIdx * SLOT_COLUMN_OFFSET) + 38, 90);
            break;

        case SavegameEntryType_OutOfBlocks:
            Gfx_StringPositionSet((slotIdx * SLOT_COLUMN_OFFSET) + 32, 90);
            break;

        default:
            Gfx_StringPositionSet((slotIdx * SLOT_COLUMN_OFFSET) + 6, (D_801E7518[slotIdx] * 20) + 53);
            break;
    }
    Gfx_StringDraw(DIALOG_STRS[entryType], 50);
#endif

    // Draw memory card message box.
    if (entryType < SavegameEntryType_CorruptedSave)
    {
        SaveScreen_SlotStatusMsgDraw(slotIdx, entryType);
    }

    D_801E7518[slotIdx]++;
    if (D_801E7518[slotIdx] == 5 || saveIdx == (g_Savegame_ElementCount1[slotIdx] - 1))
    {
        if (D_801E7564[slotIdx] == 0)
        {
            D_801E7564[slotIdx] = 1;
        }
        D_801E7514[slotIdx] = false;

        if (entryType != SavegameEntryType_LoadMemCard)
        {
            D_801E7518[slotIdx] = 0;
        }
    }
}

#if VERSION_REGION_IS(NTSCJ)
INCLUDE_ASM("screens/saveload/nonmatchings/saveload", func_801E4128);

INCLUDE_ASM("screens/saveload/nonmatchings/saveload", func_801E4334);
#endif

void SaveScreen_MemCardStateDraw(s32 g_SaveScreen_SaveScreenState, s32 memCardState) // 0x801E3910
{
    s32          strIdx;
    SAVE_BSS s32 D_801E7554;

    const char* DIALOG_STRS[] = {
#if VERSION_REGION_IS(NTSCJ)
        "　　　　　　　　　　　　　　　　　　　　",
        "~C7メモリーカードが抜かれました　　　　　　",
        "~C7メモリーカードを初期化しています　　　　",
        "~C7セーブ中です　　　　　　　　　　　　　　",
        "~C7新規ファイルを作れませんでした　　　　　",
        "~C7セーブが終了しました　　　　　　　　　　",
        "~C7セーブに失敗しました　　　　　　　　　　",
        "~C7ロードしようとしたデータは存在しなかった",
        "~C7データが壊れています　　　　　　　　　　",
        "~C7ロードに失敗しました　　　　　　　　　　",
        "~C7ロードが終了しました　　　　　　　　　　",
        "~C7ロード中です　　　　　　　　　　　　　　"
#else
        " ",
        "\7You_\x01\x01removed_\x01\x01the_\x01\x01MEMORY_\x01\x01""CARD!",
        "\7Now_formatting...",
        "\7Now_saving...",
        "\7Unable_to_create_a_new_file.",
        "\7""Finished_saving.",
        "\7""Failed_to_save!",
        "\7The_data_is_not_found!",
        "\7The_data_is_damaged!",
        "\7""Failed_to_load!",
        "\7""Finished_loading.",
        "\7Now_loading..."
#endif
    };

#if VERSION_REGION_IS(NTSCJ)
    const u8 GLYPH_COUNTS[] = {
        0, 14, 16, 6, 15, 10, 10, 20, 10, 10, 10, 6
    };
#else
    const s16 X_OFFSETS[] = {
        0,   268,
        141, 107,
        223, 136,
        119, 182,
        170, 117,
        143, 114
    };

#endif

    switch (g_SaveScreen_SaveScreenState)
    {
        case SaveScreenState_Format:
            strIdx = (memCardState == g_SaveScreen_SaveScreenState) ? 1 : 2;
            break;

        case SaveScreenState_Save:
            switch (memCardState)
            {
                case MemCardWorkResult_NotConnected:
                    strIdx = 1;
                    break;

                case MemCardWorkResult_Success:
                    strIdx = 3;
                    break;

                case MemCardWorkResult_FileCreateError:
                    strIdx = 4;
                    break;

                case MemCardWorkResult_FileIoComplete:
                    strIdx = 5;
                    break;

                case MemCardWorkResult_FileIoError:
                    strIdx = 6;
                    break;

                default:
                    strIdx = 0;
                    break;
            }
            break;

        case SaveScreenState_Load:
            switch (memCardState)
            {
                // Now loading.
                case MemCardWorkResult_Success:
                    strIdx = 11;
                    break;

                case MemCardWorkResult_DamagedData:
                    strIdx = 8;
                    break;

                case MemCardWorkResult_FileIoError:
                    strIdx = 9;
                    break;

                case MemCardWorkResult_FileIoComplete:
                    strIdx = 10;
                    break;

                case MemCardWorkResult_NotConnected:
                    strIdx = 1;
                    break;

                case MemCardWorkResult_Full:
                    strIdx = 1;
                    break;

                default:
                    strIdx = 0;
                    break;
            }
            break;

        default:
            strIdx = 0;
            break;
    }

    switch (g_SaveScreen_MemCardStateDisplay)
    {
        case 0:
            g_SaveScreen_OverwriteActive = 0;
            D_801E7554 = 0;

            if (strIdx == 1)
            {
                Game_StateStepSet(2, 0);
                g_SaveScreen_HiddenSaves[g_SelectedSaveSlotIdx]      = NO_VALUE;
                g_SaveScreen_VisualElementIdx[g_SelectedSaveSlotIdx] = NO_VALUE;
            }

#if VERSION_REGION_IS(NTSCJ)
            D_800C3920 = 20;
            func_8004C918(DIALOG_STRS[strIdx] + 3, 1, 0, 3);
#endif

            D_801E7554 = strIdx;
            g_SaveScreen_MemCardStateDisplay++;

#if VERSION_REGION_IS(NTSCJ)
            if (strIdx == 5)
            {
                D_801E7564[g_SelectedSaveSlotIdx]                   = 0;
                g_SaveScreen_LastSaveIdx[g_SelectedSaveSlotIdx]     = g_SlotElementSelectedIdx[g_SelectedSaveSlotIdx];
                g_SaveScreen_LastSaveIdx[1 - g_SelectedSaveSlotIdx] = NO_VALUE;
                g_SaveScreen_IsGameSaving                           = 0;
                g_SaveScreen_SaveFlashTimer                         = 0;
            }
#endif

        case 1:
            if (g_SaveScreen_SaveScreenState == SaveScreenState_Save && memCardState == MemCardWorkResult_Success)
            {
                g_SaveScreen_IsGameSaving = memCardState;
            }

#if VERSION_REGION_IS(NTSCJ)
            if (D_801E7554 == strIdx)
            {
                func_8004C394(DIALOG_STRS[strIdx], 2, GLYPH_COUNTS[strIdx], strIdx + 11);

                if (strIdx == 5)
                {
                    g_SaveScreen_LocationGlyphSaveIdx[g_SelectedSaveSlotIdx][g_SaveScreen_VisualElementIdx[g_SelectedSaveSlotIdx]] = NO_VALUE;
                }
            }
            else
            {
                g_SaveScreen_MemCardStateDisplay--;
                D_801E7564[g_SelectedSaveSlotIdx] = 0;
            }
#else
            // Finished saving and `D_801E7554` isn't required string.
            if (strIdx == 5 && D_801E7554 != strIdx)
            {
                g_SaveScreen_LastSaveIdx[g_SelectedSaveSlotIdx]     = g_SlotElementSelectedIdx[g_SelectedSaveSlotIdx];
                g_SaveScreen_LastSaveIdx[1 - g_SelectedSaveSlotIdx] = NO_VALUE;
                g_SaveScreen_SaveFlashTimer                         = 0;
            }

            D_801E7554 = strIdx;
            Gfx_StringPositionSet(160 - (X_OFFSETS[strIdx] >> 1), 186);
            Gfx_StringDraw(DIALOG_STRS[strIdx], DEFAULT_MAP_MESSAGE_LENGTH);

            // Finished saving.
            if (strIdx == 5)
            {
                g_SaveScreen_IsGameSaving = 0;
            }
#endif
            break;
    }
}

void SaveScreen_WriteOptionsStepDraw(s32 stringIdx, bool optionSelected) // 0x801E3C44
{
    SAVE_BSS bool D_801E7558;
    s8       color;
    u32      time;
    GsOT*    ot;
    POLY_F4* poly;
#if VERSION_REGION_IS(NTSCJ)
    POLY_FT4* glyphPoly;
    s32       i;

    const char* const DIALOG_STRS[] = {
        "~C7上書きしてもよろしいですか？　　　　　　",
        "~C7初期化してもよろしいですか？　　　　　　"
    };

    const u8 GLYPH_COUNTS[] = {
        14, 14
    };
#else
    const char* const DIALOG_STRS[] = {
        "\7Is_it_OK_to_overwrite?",
        "\7Is_it_OK_to_format?",
        "\7Yes__________No"
    };

    const u8 X_OFFSETS[] = {
        180, 154
    };
#endif

    g_SaveScreen_DisplaySaveInfo = false;
    time                         = (u8)g_SysWork.gameStateCounter % 64;
    ot                           = &g_OrderingTable2[g_ActiveBufferIdx];

    switch (g_SaveScreen_OverwriteActive)
    {
        case 0:
            g_SaveScreen_MemCardStateDisplay = 0;
#if VERSION_REGION_IS(NTSCJ)
            D_800C3920 = 20;
            func_8004C918(DIALOG_STRS[stringIdx != 0] + 3, 1, 0, 3);
            D_801E7558 = stringIdx != 0;
            g_SaveScreen_OverwriteActive++;
#else
            D_801E7558 = stringIdx != 0;
            g_SaveScreen_OverwriteActive = 1;
#endif

        case 1:
#if VERSION_REGION_IS(NTSCJ)
            if (D_801E7558 == (stringIdx != 0))
            {
                func_8004C394(DIALOG_STRS[D_801E7558], 3, GLYPH_COUNTS[D_801E7558], -1);
            }
            else
            {
                g_SaveScreen_OverwriteActive--;
            }

            for (i = 0; i < 2; i++)
            {
                glyphPoly = (POLY_FT4*)GsOUT_PACKET_P;
                setPolyFT4(glyphPoly);
                glyphPoly->tpage = getTPage(0, 0, 256, 256);
                glyphPoly->clut  = getClut(304, 510);
                setRGB0(glyphPoly, 128, 128, 128);

                if (i != 0)
                {
                    setUV4(glyphPoly, 144, 224, 144, 240,
                                      168, 224, 168, 240);
                    setXY4(glyphPoly, -54, 82, -54, 98,
                                      -30, 82, -30, 98);
                }
                else
                {
                    setUV4(glyphPoly, 204, 224, 204, 240,
                                      240, 224, 240, 240);
                    setXY4(glyphPoly, 24, 82, 24, 98,
                                      60, 82, 60, 98);
                }

                addPrim((u8*)ot->org + LAYER_24, glyphPoly);
                GsOUT_PACKET_P = (u8*)glyphPoly + sizeof(POLY_FT4);
            }
#else
            Gfx_StringPositionSet(160 - (X_OFFSETS[stringIdx] / 2), 178);
            Gfx_StringDraw(DIALOG_STRS[stringIdx], DEFAULT_MAP_MESSAGE_LENGTH);
            Gfx_StringPositionSet(104, 196);
            Gfx_StringDraw(DIALOG_STRS[2], DEFAULT_MAP_MESSAGE_LENGTH);
#endif

            poly = (POLY_F4*)GsOUT_PACKET_P;
            setPolyF4(poly);
            setSemiTrans(poly, true);

            if (time < 32)
            {
                color = (time * 2) + 32;
                setRGB0(poly, color, color, 32);
            }
            else
            {
                color = 96 - ((time - 32) * 2);
                setRGB0(poly, color, color, 32);
            }

            if (optionSelected)
            {
                // Player has "YES" selected.
                setXY4(poly, -66, 82, -66, 98, -18, 82, -18, 98);
            }
            else
            {
                // Player has "NO" selected.
                setXY4(poly, 18, 82, 18, 98, 66, 82, 66, 98);
            }

            GsOUT_PACKET_P = (u8*)poly + sizeof(POLY_F4);
            addPrim((u8*)ot->org + LAYER_28, poly);

            Gfx_Primitive2dTextureSet(0, 0, 7, 1);
            break;
    }
}

void SaveScreen_SaveFlash(void) // 0x801E3E78
{
    s32      slotIdx;
    q19_12   sinFlashTimer;
    s8       color;
    u32      rowIdx;
    GsOT*    ot;
    POLY_F4* poly;

    ot      = &g_OrderingTable2[g_ActiveBufferIdx];
    slotIdx = (g_SaveScreen_LastSaveIdx[0] == NO_VALUE) ? 1 : 0;
    rowIdx  = g_SaveScreen_LastSaveIdx[slotIdx] - g_SaveScreen_HiddenSaves[slotIdx];

    if (rowIdx < 5)
    {
        sinFlashTimer  = Math_Sin((g_SaveScreen_SaveFlashTimer << 10) / SAVE_FLASH_TIMER_MAX);
        poly = (POLY_F4*)GsOUT_PACKET_P;
        setPolyF4(poly);
        setSemiTrans(poly, true);

        color = ~FP_FROM(sinFlashTimer * 255, Q12_SHIFT);
        setRGB0(poly, color, color, color);

        setXY4(poly,
               (slotIdx * SLOT_COLUMN_OFFSET) - 130, (rowIdx * SLOT_ROW_OFFSET) - 62,
               (slotIdx * SLOT_COLUMN_OFFSET) - 130, (rowIdx * SLOT_ROW_OFFSET) - 43,
               (slotIdx * SLOT_COLUMN_OFFSET) - 11,  (rowIdx * SLOT_ROW_OFFSET) - 62,
               (slotIdx * SLOT_COLUMN_OFFSET) - 11,  (rowIdx * SLOT_ROW_OFFSET) - 43);

        GsOUT_PACKET_P = (u8*)poly + sizeof(POLY_F4);
        addPrim((u8*)ot->org + LAYER_24, poly);

        Gfx_Primitive2dTextureSet(0, 0, 6, 1);
    }
}

void SaveScreen_SlotBorder(void) // 0x801E4010
{
    #define BORDER_LINE_COUNT  5
    #define BORDER_PIXEL_WIDTH 2

    GsOT*    ot;
    LINE_F2* borderLine;
    POLY_G4* glowPoly;
    s32      i;
    s32      j;

    // Lines for green border around slot. Each line is doubled for 2-pixel width.
#if VERSION_REGION_IS(NTSCJ)
    const s_Line2d BORDER_LINES[BORDER_LINE_COUNT][BORDER_PIXEL_WIDTH] = {
        // Top-left.
        {
            { { -103, -88 }, { -146, -88 } },
            { { -103, -87 }, { -146, -87 } }
        },
        // Top-right.
        {
            { { -46, -88 }, { -2, -88 } },
            { { -46, -87 }, { -2, -87 } }
        },
        // Left.
        {
            { { -146, -88 }, { -146, 42 } },
            { { -145, -87 }, { -145, 41 } }
        },
        // Right.
        {
            { { -2, -88 }, { -2, 42 } },
            { { -3, -87 }, { -3, 41 } }
        },
        // Bottom.
        {
            { { -146, 42 }, { -2, 42 } },
            { { -145, 41 }, { -3, 41 } }
        }
    };
#else
    const s_Line2d BORDER_LINES[BORDER_LINE_COUNT][BORDER_PIXEL_WIDTH] = {
        // Top-left.
        {
            { { -107, -88 }, { -146, -88 } },
            { { -107, -87 }, { -146, -87 } }
        },
        // Top-right.
        {
            { { -42, -88 }, { -2, -88 } },
            { { -42, -87 }, { -2, -87 } }
        },
        // Left.
        {
            { { -146, -88 }, { -146, 42 } },
            { { -145, -87 }, { -145, 41 } }
        },
        // Right.
        {
            { { -2, -88 }, { -2, 42 } },
            { { -3, -87 }, { -3, 41 } }
        },
        // Bottom.
        {
            { { -146, 42 }, { -2, 42 } },
            { { -145, 41 }, { -3, 41 } }
        }
    };
#endif

    // Polygons for green glow of border around slot. Each line has 2 quads for inner and outer glow.
#if VERSION_REGION_IS(NTSCJ)
    const s_Quad2d BORDER_GLOW_QUADS[BORDER_LINE_COUNT][BORDER_PIXEL_WIDTH] = {
        // Top-left.
        {
            { { -102, -88 }, { -102, -92 }, { -146, -88 }, { -150, -92 } },
            { { -102, -87 }, { -102, -83 }, { -146, -87 }, { -142, -83 } }
        },
        // Top-right.
        {
            { { -46, -88 }, { -46, -92 }, { -2, -88 }, { 2,  -92 } },
            { { -46, -87 }, { -46, -83 }, { -2, -87 }, { -6, -83 } }
        },
        // Left.
        {
            { { -146, -88 }, { -150, -92 }, { -146, 42 }, { -150, 46 } },
            { { -145, -87 }, { -141, -83 }, { -145, 41 }, { -141, 37 } }
        },
        // Right.
        {
            { { -2, -88 }, { 2,  -92 }, { -2, 42 }, {  2, 46 } },
            { { -3, -87 }, { -7, -83 }, { -3, 41 }, { -7, 37 } }
        },
        // Bottom.
        {
            { { -146, 42 }, { -150, 46 }, { -2, 42 }, {  2, 46 } },
            { { -145, 41 }, { -141, 37 }, { -3, 41 }, { -7, 37 } }
        }
    };
#else
    const s_Quad2d BORDER_GLOW_QUADS[BORDER_LINE_COUNT][BORDER_PIXEL_WIDTH] = {
        // Top-left.
        {
            { { -106, -88 }, { -106, -92 }, { -146, -88 }, { -150, -92 } },
            { { -106, -87 }, { -106, -83 }, { -146, -87 }, { -142, -83 } }
        },
        // Top-right.
        {
            { { -42, -88 }, { -42, -92 }, { -2, -88 }, { 2,  -92 } },
            { { -42, -87 }, { -42, -83 }, { -2, -87 }, { -6, -83 } }
        },
        // Left.
        {
            { { -146, -88 }, { -150, -92 }, { -146, 42 }, { -150, 46 } },
            { { -145, -87 }, { -141, -83 }, { -145, 41 }, { -141, 37 } }
        },
        // Right.
        {
            { { -2, -88 }, { 2,  -92 }, { -2, 42 }, {  2, 46 } },
            { { -3, -87 }, { -7, -83 }, { -3, 41 }, { -7, 37 } }
        },
        // Bottom.
        {
            { { -146, 42 }, { -150, 46 }, { -2, 42 }, {  2, 46 } },
            { { -145, 41 }, { -141, 37 }, { -3, 41 }, { -7, 37 } }
        }
    };
#endif

    ot = &g_OrderingTable2[g_ActiveBufferIdx];

    // Draw green border around slot.
    for (i = 0; i < BORDER_PIXEL_WIDTH; i++)
    {
        for (j = 0; j < BORDER_LINE_COUNT; j++)
        {
            borderLine = (LINE_F2*)GsOUT_PACKET_P;
            setLineF2(borderLine);

            setRGB0(borderLine, 0, 255, 0);

            setXY2(borderLine,
                   BORDER_LINES[j][i].vertex0.vx + (g_SelectedSaveSlotIdx * SLOT_COLUMN_OFFSET), BORDER_LINES[j][i].vertex0.vy,
                   BORDER_LINES[j][i].vertex1.vx + (g_SelectedSaveSlotIdx * SLOT_COLUMN_OFFSET), BORDER_LINES[j][i].vertex1.vy);

            addPrim((u8*)ot->org + LAYER_28, borderLine);
            GsOUT_PACKET_P = (u8*)borderLine + sizeof(LINE_F2);
        }
    }

    // Draw green border glow around slot.
    for (i = 0; i < BORDER_PIXEL_WIDTH; i++)
    {
        for (j = 0; j < BORDER_LINE_COUNT; j++)
        {
            glowPoly = (POLY_G4*)GsOUT_PACKET_P;
            setPolyG4(glowPoly);
            setSemiTrans(glowPoly, true);

            setRGB0(glowPoly, 0, 128, 0);
            setRGB1(glowPoly, 0, 0, 0);
            setRGB2(glowPoly, 0, 128, 0);
            setRGB3(glowPoly, 0, 0, 0);

            setXY4(glowPoly,
                   BORDER_GLOW_QUADS[j][i].vertex0.vx + (g_SelectedSaveSlotIdx * SLOT_COLUMN_OFFSET), BORDER_GLOW_QUADS[j][i].vertex0.vy,
                   BORDER_GLOW_QUADS[j][i].vertex1.vx + (g_SelectedSaveSlotIdx * SLOT_COLUMN_OFFSET), BORDER_GLOW_QUADS[j][i].vertex1.vy,
                   BORDER_GLOW_QUADS[j][i].vertex2.vx + (g_SelectedSaveSlotIdx * SLOT_COLUMN_OFFSET), BORDER_GLOW_QUADS[j][i].vertex2.vy,
                   BORDER_GLOW_QUADS[j][i].vertex3.vx + (g_SelectedSaveSlotIdx * SLOT_COLUMN_OFFSET), BORDER_GLOW_QUADS[j][i].vertex3.vy);

            addPrim((u8*)ot->org + LAYER_28, glowPoly);
            GsOUT_PACKET_P = (u8*)glowPoly + sizeof(POLY_G4);
        }
    }

    Gfx_Primitive2dTextureSet(0, 0, 7, 1);

    #undef BORDER_LINE_COUNT
    #undef BORDER_PIXEL_WIDTH
}

void SaveScreen_SlotStatusMsgShineDraw(s32 slotIdx) // 0x801E43C8
{
    GsOT*    ot;
    s16      colorTimer;
    s32      temp;
    s8       color;
    POLY_F4* poly;

    temp       = (u8)g_SysWork.gameStateCounter % 64;
    colorTimer = temp;
    ot         = &g_OrderingTable2[g_ActiveBufferIdx];

    if (g_SelectedSaveSlotIdx == slotIdx)
    {
        poly = (POLY_F4*)GsOUT_PACKET_P;
        setPolyF4(poly);
        setSemiTrans(poly, true);

        if ((u16)colorTimer < 32)
        {
            color = (colorTimer * 2) + 32;
            setRGB0(poly, color, color, 32);
        }
        else
        {
            color = Q8_COLOR(0.375f) - ((colorTimer - 32) * 2);
            setRGB0(poly, color, color, 32);
        }

        setXY4(poly,
               (slotIdx * SLOT_COLUMN_OFFSET) - 142, -35, (slotIdx * SLOT_COLUMN_OFFSET) - 142, 2,
               (slotIdx * SLOT_COLUMN_OFFSET) - 6,   -35, (slotIdx * SLOT_COLUMN_OFFSET) - 6,   2);

        GsOUT_PACKET_P = (u8*)poly + sizeof(POLY_F4);
        addPrim((u8*)ot->org + LAYER_28, poly);

        Gfx_Primitive2dTextureSet(0, 0, 7, 1);
    }
}

void SaveScreen_NavigationDraw(s32 slotIdx, s32 saveCount, s32 selectedSaveIdx, s32 selectedSaveOffsetY) // 0x801E451C
{
    #define SCROLL_BAR_THUMB_RECT_COUNT 2
    #define SCROLL_BAR_ARROW_COUNT      2
    #define SCROLL_BAR_OFFSET_Y         60

    GsOT*    ot;
    POLY_F4* thumbPoly;
    POLY_F4* highlightPoly;
    POLY_G4* trackPoly;
    POLY_G3* arrowPoly;
    POLY_F4* unkPoly;
    s32      trackOffsetX;
    s32      thumbOffsetY;
    s32      thumbOffsetTopY;
    u32      thumbOffsetBottomY;
    s32      thumbOffsetX;
    s32      arrowOffsetX;
    u8       highlightColor;
    s32      i;
    s32      j;

    u32 selectedSaveHighlightTimer = (u8)g_SysWork.gameStateCounter % 64;

    const s_Quad2d SCROLL_BAR_TRACK_QUADS[] = {
        { { 0, 0 }, { 0, 96 }, { 4, 0 }, { 4, 96 } }, // Left half.
        { { 8, 0 }, { 8, 96 }, { 4, 0 }, { 4, 96 } }  // Right half.
    };

    const s_Triangle2d SCROLL_BAR_ARROW_TRIS[MEMCARD_SLOT_COUNT_MAX][SCROLL_BAR_ARROW_COUNT] = {
        // Up arrows.
        {
            { { 4, -1 }, { -1, 7 }, { 8, 7 } }, // Slot 1 up arrow.
            { { 5,  1 }, {  0, 8 }, { 9, 8 } }  // Slot 2 up arrow.
        },
        // Down arrows.
        {
            { { 4, 96 }, { 0, 88 }, { 8, 88 } }, // Slot 1 down arrow.
            { { 5, 97 }, { 1, 89 }, { 9, 89 } }  // Slot 2 down arrow.
        }
    };

    const u8 THUMB_Y_OFFESTS[] = {
        79, 40, 27, 20, 16, 14, 12, 10,
        9, 8, 8, 7, 7, 6, 6, 5,
        5, 5, 5, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 3, 3, 3, 3, 3, 3,
        3, 3, 3, 3, 3, 3, 3, 3,
        3, 3, 3, 3, 3, 3, 3, 3,
        3, 3, 3, 3, 3, 3, 3, 3,
        3, 3, 3, 3, 3, 3, 3, 3,
        3, 3, 3, 3, 3, 3, 3, 3,
        3, 3, 3, 3, 3, 3, 3, 3,
        3, 3, 3, 3, 3, 3, 3, 3,
        3, 3, 3, 3, 3, 3, 3, 3,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0
    };

    ot = &g_OrderingTable2[g_ActiveBufferIdx];

    if (g_SaveScreen_DisplaySaveInfo == true && slotIdx == g_SelectedSaveSlotIdx)
    {
        SaveScreen_ElementInfoDraw(slotIdx, selectedSaveIdx);
    }

    // Draw scroll bar thumb.
    if (saveCount != 0)
    {
        thumbOffsetY       = ((selectedSaveIdx * 79) / saveCount) + 8;
        thumbOffsetBottomY = THUMB_Y_OFFESTS[saveCount - 1];

        // Inner and outer rectangles.
        for (i = 0; i < SCROLL_BAR_THUMB_RECT_COUNT; i++)
        {
            thumbPoly = (POLY_F4*)GsOUT_PACKET_P;
            setPolyF4(thumbPoly);

            // Inner rectangle color.
            if (i != 0)
            {
                setRGB0(thumbPoly, 0x70, 0x70, 0x70);
            }
            // Outer rectangle color.
            else
            {
                setRGB0(thumbPoly, 0x50, 0x50, 0x50);
            }

            // Reference track vertices for accurate thumb.
            thumbOffsetX    = (slotIdx * SLOT_COLUMN_OFFSET) - 139;
            thumbOffsetTopY = i - SCROLL_BAR_OFFSET_Y;
            setXY4(thumbPoly,
                   (SCROLL_BAR_TRACK_QUADS[0].vertex0.vx + thumbOffsetX) + i,  (SCROLL_BAR_TRACK_QUADS[0].vertex0.vy + thumbOffsetY) + thumbOffsetTopY,
                   (SCROLL_BAR_TRACK_QUADS[0].vertex0.vx + thumbOffsetX) + i, ((SCROLL_BAR_TRACK_QUADS[0].vertex0.vy + thumbOffsetY) + thumbOffsetBottomY) - (i + SCROLL_BAR_OFFSET_Y),
                   (SCROLL_BAR_TRACK_QUADS[1].vertex0.vx + thumbOffsetX) - i,  (SCROLL_BAR_TRACK_QUADS[1].vertex0.vy + thumbOffsetY) + thumbOffsetTopY,
                   (SCROLL_BAR_TRACK_QUADS[1].vertex0.vx + thumbOffsetX) - i, ((SCROLL_BAR_TRACK_QUADS[1].vertex0.vy + thumbOffsetY) + thumbOffsetBottomY) - (i + SCROLL_BAR_OFFSET_Y));

            // Ensure inner rectangle is on top.
            addPrim(((u8*)ot->org + LAYER_28) - (i * 4), thumbPoly);
            GsOUT_PACKET_P = (u8*)thumbPoly + sizeof(POLY_F4);
        }
    }

    // Draw selected save highlight.
    if (g_SelectedSaveSlotIdx == slotIdx)
    {
        highlightPoly = (POLY_F4*)GsOUT_PACKET_P;
        setPolyF4(highlightPoly);
        setSemiTrans(highlightPoly, true);

        // Fade in and out.
        if (selectedSaveHighlightTimer < 0x20u)
        {
            highlightColor = (selectedSaveHighlightTimer * 2) + 0x20;
            setRGB0(highlightPoly, highlightColor, highlightColor, 0x20);
        }
        else
        {
            highlightColor = 0x60 - ((selectedSaveHighlightTimer - 0x20) * 2);
            setRGB0(highlightPoly, highlightColor, highlightColor, 0x20);
        }

        setXY4(highlightPoly,
               (slotIdx * SLOT_COLUMN_OFFSET) - 130, (selectedSaveOffsetY * SLOT_ROW_OFFSET) - 62,
               (slotIdx * SLOT_COLUMN_OFFSET) - 130, (selectedSaveOffsetY * SLOT_ROW_OFFSET) - 43,
               (slotIdx * SLOT_COLUMN_OFFSET) - 10,  (selectedSaveOffsetY * SLOT_ROW_OFFSET) - 62,
               (slotIdx * SLOT_COLUMN_OFFSET) - 10,  (selectedSaveOffsetY * SLOT_ROW_OFFSET) - 43);

        addPrim((u8*)ot->org + LAYER_28, highlightPoly);
        GsOUT_PACKET_P = (u8*)highlightPoly + sizeof(POLY_F4);

        Gfx_Primitive2dTextureSet(0, 0, 7, 1);
    }

    // Draw scroll bar arrows.
    for (i = 0; i < SCROLL_BAR_ARROW_COUNT; i++)
    {
        for (j = 0; j < 1; j++)
        {
            arrowPoly = (POLY_G3*)GsOUT_PACKET_P;
            setPolyG3(arrowPoly);

            setRGB0(arrowPoly, Q8_COLOR(0.625f), Q8_COLOR(0.625f), Q8_COLOR(0.625f));
            setRGB1(arrowPoly, Q8_COLOR(0.625f), Q8_COLOR(0.625f), Q8_COLOR(0.625f));
            setRGB2(arrowPoly, Q8_COLOR(0.625f), Q8_COLOR(0.625f), Q8_COLOR(0.625f));

            arrowOffsetX = (slotIdx * SLOT_COLUMN_OFFSET) - 139;
            setXY3(arrowPoly,
                   SCROLL_BAR_ARROW_TRIS[i][j].vertex0.vx + arrowOffsetX, SCROLL_BAR_ARROW_TRIS[i][j].vertex0.vy - SCROLL_BAR_OFFSET_Y,
                   SCROLL_BAR_ARROW_TRIS[i][j].vertex1.vx + arrowOffsetX, SCROLL_BAR_ARROW_TRIS[i][j].vertex1.vy - SCROLL_BAR_OFFSET_Y,
                   SCROLL_BAR_ARROW_TRIS[i][j].vertex2.vx + arrowOffsetX, SCROLL_BAR_ARROW_TRIS[i][j].vertex2.vy - SCROLL_BAR_OFFSET_Y);

            addPrim((u8*)ot->org + LAYER_28, arrowPoly);
            GsOUT_PACKET_P = (u8*)arrowPoly + sizeof(POLY_G3);
        }
    }

    // Draw scroll bar track.
    for (i = 0; i < 2; i++)
    {
        trackPoly = (POLY_G4*)GsOUT_PACKET_P;
        setPolyG4(trackPoly);

        setRGB0(trackPoly, 32, 32, 32);
        setRGB1(trackPoly, 32, 32, 32);
        setRGB2(trackPoly, 128,   128,   128);
        setRGB3(trackPoly, 128,   128,   128);

        trackOffsetX = (slotIdx * SLOT_COLUMN_OFFSET) - 139;
        setXY4(trackPoly,
               SCROLL_BAR_TRACK_QUADS[i].vertex0.vx + trackOffsetX, SCROLL_BAR_TRACK_QUADS[i].vertex0.vy - SCROLL_BAR_OFFSET_Y,
               SCROLL_BAR_TRACK_QUADS[i].vertex1.vx + trackOffsetX, SCROLL_BAR_TRACK_QUADS[i].vertex1.vy - SCROLL_BAR_OFFSET_Y,
               SCROLL_BAR_TRACK_QUADS[i].vertex2.vx + trackOffsetX, SCROLL_BAR_TRACK_QUADS[i].vertex2.vy - SCROLL_BAR_OFFSET_Y,
               SCROLL_BAR_TRACK_QUADS[i].vertex3.vx + trackOffsetX, SCROLL_BAR_TRACK_QUADS[i].vertex3.vy - SCROLL_BAR_OFFSET_Y);

        addPrim((u8*)ot->org + LAYER_32, trackPoly);
        GsOUT_PACKET_P = (u8*)trackPoly + sizeof(POLY_G4);
    }

    // Draw transparent background.
    unkPoly = (POLY_F4*)GsOUT_PACKET_P;
    setPolyF4(unkPoly);
    setSemiTrans(unkPoly, true);

    setRGB0(unkPoly, Q8_COLOR(0.1875f), Q8_COLOR(0.1875f), Q8_COLOR(0.1875f));

    setXY4(unkPoly,
           (slotIdx * SLOT_COLUMN_OFFSET) - 139, -81,
           (slotIdx * SLOT_COLUMN_OFFSET) - 139,  37,
           (slotIdx * SLOT_COLUMN_OFFSET) - 9,   -81,
           (slotIdx * SLOT_COLUMN_OFFSET) - 9,    37);

    addPrim((u8*)ot->org + LAYER_32, unkPoly);
    GsOUT_PACKET_P = (u8*)unkPoly + sizeof(POLY_F4);

    Gfx_Primitive2dTextureSet(0, 0, 8, 1);

    #undef SCROLL_BAR_THUMB_RECT_COUNT
    #undef SCROLL_BAR_ARROW_COUNT
    #undef SCROLL_BAR_OFFSET_Y
}

void SaveScreen_SaveBorderDraw(s_SaveScreenElement* saveEntry, s_SaveScreenElement* nextSaveEntry, s32 saveIdx, s32 slotIdx) // 0x801E4D90
{
    GsOT* ot;
    u32   entryIdx         = saveEntry->elementIdx;
    s32   fileColorIdx     = saveEntry->fileIdx + 1;
    s32   nextFileColorIdx = nextSaveEntry->fileIdx + 1;
    s16   tileOffsetX1;
    s16   tileOffsetX0;
    s32   borderLineOffsetX;
    s32   borderLineOffsetY;
    s32   rowIdx;
    s32   i;
    s32   firstLineIdx;

    s_Line2d entryBorderLines[] = {
        { { -131, -62 }, { -11, -62 } },
        { { -131, -43 }, { -11, -43 } },
        { { -131, -62 }, { -131, -44 } },
        { { -11,  -62 }, { -11, -44 } }
    };

    s_PrimColor fileColors[] = {
        { 0x20, 0xA0, 0x20, 0x00 },
        { 0x60, 0xA0, 0x20, 0x00 },
        { 0xA0, 0xA0, 0x20, 0x00 },
        { 0xA0, 0x80, 0x20, 0x00 },
        { 0xA0, 0x60, 0x20, 0x00 },
        { 0xA0, 0x20, 0x20, 0x00 },
        { 0xA0, 0x20, 0x60, 0x00 },
        { 0xA0, 0x20, 0x80, 0x00 },
        { 0xA0, 0x20, 0xA0, 0x00 },
        { 0x80, 0x20, 0xA0, 0x00 },
        { 0x60, 0x20, 0xA0, 0x00 },
        { 0x20, 0x20, 0xA0, 0x00 },
        { 0x20, 0x40, 0xA0, 0x00 },
        { 0x20, 0x60, 0xA0, 0x00 },
        { 0x20, 0xA0, 0xA0, 0x00 }
    };

    LINE_F2* borderLine;
    TILE*    cornerTile;

    ot     = &g_OrderingTable2[g_ActiveBufferIdx];
    rowIdx = saveIdx - g_SaveScreen_HiddenSaves[slotIdx];

    // Draw border around savegame entry.
    firstLineIdx = rowIdx ? (entryIdx > 0) : 0;
    for (i = firstLineIdx; i < 4; i++)
    {
        borderLine = (LINE_F2*)GsOUT_PACKET_P;
        setLineF2(borderLine);
        setRGB0(borderLine, fileColors[fileColorIdx - 1].r, fileColors[fileColorIdx - 1].g, fileColors[fileColorIdx - 1].b);

        borderLineOffsetX = slotIdx * SLOT_COLUMN_OFFSET;
        borderLineOffsetY = rowIdx  * SLOT_ROW_OFFSET;

        // Regular entry.
        if (firstLineIdx == 0)
        {
            setXY2(borderLine,
                   entryBorderLines[i].vertex0.vx + borderLineOffsetX, entryBorderLines[i].vertex0.vy + borderLineOffsetY,
                   entryBorderLines[i].vertex1.vx + borderLineOffsetX, entryBorderLines[i].vertex1.vy + borderLineOffsetY);
        }
        // Entry at boundary between files.
        else
        {
            setXY2(borderLine,
                   entryBorderLines[i].vertex0.vx + borderLineOffsetX, (entryBorderLines[i].vertex0.vy + borderLineOffsetY) - ((i + (s32)((u32)i >> 31)) >> 1),
                   entryBorderLines[i].vertex1.vx + borderLineOffsetX,  entryBorderLines[i].vertex1.vy + borderLineOffsetY);
        }

        addPrim((u8*)ot->org + LAYER_24, borderLine);
        GsOUT_PACKET_P = (u8*)borderLine + sizeof(LINE_F2);
    }

    // Draw file start corner markers.
    if (entryIdx == 0)
    {
        for (i = 0; i < 2; i++)
        {
            cornerTile = (TILE*)GsOUT_PACKET_P;
            setTile(cornerTile);

            setRGB0(cornerTile, fileColors[fileColorIdx - 1].r, fileColors[fileColorIdx - 1].g, fileColors[fileColorIdx - 1].b);

            tileOffsetX0 = (slotIdx * SLOT_COLUMN_OFFSET) - 131;
            setXY0(cornerTile, tileOffsetX0 + (i * 117), (rowIdx * 20) - 62);
            setWH(cornerTile, 4, 4);

            addPrim((u8*)ot->org + LAYER_24, cornerTile);
            GsOUT_PACKET_P = (u8*)cornerTile + sizeof(TILE);
        }
    }

    // Draw file end corner markers.
    if (fileColorIdx != nextFileColorIdx || (saveIdx + 1) == g_Savegame_ElementCount1[slotIdx])
    {
        for (i = 0; i < 2; i += 1)
        {
            cornerTile = (TILE*)GsOUT_PACKET_P;
            setTile(cornerTile);

            setRGB0(cornerTile, fileColors[fileColorIdx - 1].r, fileColors[fileColorIdx - 1].g, fileColors[fileColorIdx - 1].b);

            tileOffsetX1 = (slotIdx * SLOT_COLUMN_OFFSET) - 131;
            setXY0(cornerTile, tileOffsetX1 + (i * 117), (rowIdx * 20) - 46);
            setWH(cornerTile, 4, 4);

            addPrim((u8*)ot->org + LAYER_24, cornerTile);
            GsOUT_PACKET_P = (u8*)cornerTile + sizeof(TILE);
        }
    }

    if (D_801E756C[slotIdx] != fileColorIdx)
    {
        D_801E756C[slotIdx] = fileColorIdx;
    }
}

void SaveScreen_SlotStatusMsgDraw(s32 slotIdx, s32 entryType) // 0x801E52D8
{
    const s_ColoredLine2d COLORED_LINES[MEMCARD_SLOT_COUNT_MAX] = {
        // Red line.
        {
            { { -142, -33 }, { 136, 33 } },
            255, 0, 0,
            0,
        },
        // Green line.
        {
            { { -142, -33 }, { 136, 33 } },
            0, 255, 0,
            0
        }
    };

    const s_LineBorder BORDER_LINES = {
        {
            { { -144, -36 }, { -4,  -36 } },
            { { -144,  2  }, { -4,   2  } },
            { { -144, -36 }, { -144, 2  } },
            { { -4,   -36 }, { -4,   2  } }
        }
    };

    const s_QuadBorder BORDER_GLOW_QUADS = {
        {
            { { -144, -36 }, { -148, -40 }, { -4,  -36 }, { 0,   -40 } },
            { { -144,  2  }, { -148,  6  }, { -4,   2  }, { 0,    6  } },
            { { -144, -36 }, { -148, -40 }, { -144, 2  }, { -148, 6  } },
            { { -4,   -36 }, {  0,   -40 }, { -4,   2  }, { 0,    6  } }
        }
    };

    if (entryType == SavegameEntryType_UnformattedMemCard && g_GameWork.gameState == GameState_SaveScreen)
    {
        SaveScreen_SlotStatusMsgStepDraw(&BORDER_LINES, &BORDER_GLOW_QUADS, &COLORED_LINES[1], slotIdx);
    }
    else
    {
        SaveScreen_SlotStatusMsgStepDraw(&BORDER_LINES, &BORDER_GLOW_QUADS, &COLORED_LINES[0], slotIdx);
    }
}

void SaveScreen_SlotStatusMsgStepDraw(s_LineBorder* borderLines, s_QuadBorder* borderGlowQuads, s_ColoredLine2d* coloredLine, s32 slotIdx) // 0x801E54DC
{
    s32       i;
    GsOT*     ot;
    LINE_F2*  borderLine;
    POLY_G4*  glowPoly;
    POLY_F4*  unkPoly;
    s_Line2d* glowLine;

    ot = &g_OrderingTable2[g_ActiveBufferIdx];

    // Draw border around message.
    for (i = 0; i < 4; i++)
    {
        borderLine = (LINE_F2*)GsOUT_PACKET_P;
        setLineF2(borderLine);
        setRGB0(borderLine, coloredLine->r, coloredLine->g, coloredLine->b);

        setXY2(borderLine,
               borderLines->lines[i].vertex0.vx + (slotIdx * SLOT_COLUMN_OFFSET), borderLines->lines[i].vertex0.vy,
               borderLines->lines[i].vertex1.vx + (slotIdx * SLOT_COLUMN_OFFSET), borderLines->lines[i].vertex1.vy);

        addPrim((u8*)ot->org + LAYER_28, borderLine);
        GsOUT_PACKET_P = (u8*)borderLine + sizeof(LINE_F2);
    }

    // Draw border glow around message.
    for (i = 0; i < 4; i++)
    {
        glowPoly = (POLY_G4*)GsOUT_PACKET_P;
        setPolyG4(glowPoly);
        setSemiTrans(glowPoly, true);

        setRGB0(glowPoly, coloredLine->r / 2, coloredLine->g / 2, coloredLine->b / 2);
        setRGB1(glowPoly, 0, 0, 0);
        setRGB2(glowPoly, coloredLine->r / 2, coloredLine->g / 2, coloredLine->b / 2);
        setRGB3(glowPoly, 0, 0, 0);

        glowLine = (s_Line2d*)&borderGlowQuads->quads[i].vertex2;
        setXY4(glowPoly,
               borderGlowQuads->quads[i].vertex0.vx + (slotIdx * SLOT_COLUMN_OFFSET), borderGlowQuads->quads[i].vertex0.vy,
               borderGlowQuads->quads[i].vertex1.vx + (slotIdx * SLOT_COLUMN_OFFSET), borderGlowQuads->quads[i].vertex1.vy,
               borderGlowQuads->quads[i].vertex2.vx + (slotIdx * SLOT_COLUMN_OFFSET), glowLine->vertex0.vy,
               glowLine->vertex1.vx + (slotIdx * SLOT_COLUMN_OFFSET), glowLine->vertex1.vy);

        addPrim((u8*)ot->org + LAYER_32, glowPoly);
        GsOUT_PACKET_P = (u8*)glowPoly + sizeof(POLY_G4);
    }

    Gfx_Primitive2dTextureSet(0, 0, 5, 1);

    // Draw (???)'
    unkPoly = (POLY_F4*)GsOUT_PACKET_P;
    setPolyF4(unkPoly);
    setSemiTrans(unkPoly, true);

    setRGB0(unkPoly, Q8_COLOR(0.1875f), Q8_COLOR(0.1875f), Q8_COLOR(0.1875f));

    setXY4(unkPoly,
           coloredLine->line.vertex0.vx + (slotIdx * SLOT_COLUMN_OFFSET),                                    coloredLine->line.vertex0.vy,
           coloredLine->line.vertex0.vx + (slotIdx * SLOT_COLUMN_OFFSET),                                    coloredLine->line.vertex0.vy + coloredLine->line.vertex1.vy,
           coloredLine->line.vertex0.vx + coloredLine->line.vertex1.vx + (slotIdx * SLOT_COLUMN_OFFSET), coloredLine->line.vertex0.vy,
           coloredLine->line.vertex0.vx + coloredLine->line.vertex1.vx + (slotIdx * SLOT_COLUMN_OFFSET), coloredLine->line.vertex0.vy + coloredLine->line.vertex1.vy);

    addPrim((u8*)ot->org + LAYER_32, unkPoly);
    GsOUT_PACKET_P = (u8*)unkPoly + sizeof(POLY_F4);

    Gfx_Primitive2dTextureSet(0, 0, 8, 1);
}

void SaveScreen_RectSaveInfoDraw(s_Line2d* line) // 0x801E5898
{
    s32      i;
    GsOT*    ot;
    POLY_F3* poly_f3;
    POLY_F4* poly_f4;
    LINE_G2* line_g2;

    // Adjusted lines?
    DVECTOR adjLines[] = {
        { line->vertex0.vx,                            line->vertex0.vy },
        { (line->vertex0.vx + line->vertex1.vx) - 8, line->vertex0.vy },
        { line->vertex0.vx + line->vertex1.vx,       line->vertex0.vy + 8},
        { line->vertex0.vx + line->vertex1.vx,       line->vertex0.vy + line->vertex1.vy },
        { line->vertex0.vx + 8,                        line->vertex0.vy + line->vertex1.vy },
        { line->vertex0.vx,                            (line->vertex0.vy + line->vertex1.vy) - 8 }
    };

    s_Triangle2d tris[] = {
        {
            { line->vertex0.vx + line->vertex1.vx,       line->vertex0.vy },
            { (line->vertex0.vx + line->vertex1.vx) - 8, line->vertex0.vy },
            { line->vertex0.vx + line->vertex1.vx,       line->vertex0.vy + 8 },
        },
        {
            { line->vertex0.vx,     line->vertex0.vy + line->vertex1.vy },
            { line->vertex0.vx + 8, line->vertex0.vy + line->vertex1.vy },
            { line->vertex0.vx,     (line->vertex0.vy + line->vertex1.vy) - 8 }
        }
    };

    ot = &g_OrderingTable2[g_ActiveBufferIdx];

    for (i = 0; i < 2; i++)
    {
        poly_f3 = (POLY_F3*)GsOUT_PACKET_P;
        setPolyF3(poly_f3);

        setSemiTrans(poly_f3, true);
        setRGB0(poly_f3, Q8_COLOR(0.1875f), Q8_COLOR(0.1875f), Q8_COLOR(0.1875f));

        setXY3(poly_f3,
               tris[i].vertex0.vx, tris[i].vertex0.vy,
               tris[i].vertex1.vx, tris[i].vertex1.vy,
               tris[i].vertex2.vx, tris[i].vertex2.vy);

        addPrim((u8*)ot->org + LAYER_32, poly_f3);
        GsOUT_PACKET_P  = (u8*)poly_f3;
        GsOUT_PACKET_P += sizeof(POLY_F3);
    }

    Gfx_Primitive2dTextureSet(0, 0, 8, 2);

    poly_f4 = (POLY_F4*)GsOUT_PACKET_P;
    setPolyF4(poly_f4);
    setSemiTrans(poly_f4, true);

    setRGB0(poly_f4, Q8_COLOR(0.1875f), Q8_COLOR(0.1875f), Q8_COLOR(0.1875f));

    setXY4(poly_f4,
           line->vertex0.vx,                      line->vertex0.vy,
           line->vertex0.vx,                      line->vertex0.vy + line->vertex1.vy,
           line->vertex0.vx + line->vertex1.vx, line->vertex0.vy,
           line->vertex0.vx + line->vertex1.vx, line->vertex0.vy + line->vertex1.vy);

    addPrim((u8*)ot->org + LAYER_36, poly_f4);
    GsOUT_PACKET_P = (u8*)poly_f4 + sizeof(POLY_F4);

    Gfx_Primitive2dTextureSet(0, 0, 9, 1);

    for (i = 0; i < 6; i++)
    {
        line_g2 = (LINE_G2*)GsOUT_PACKET_P;
        setLineG2(line_g2);

        if (i & 0x1)
        {
            setRGB0(line_g2, Q8_COLOR(0.625f),  128,    64);
            setRGB1(line_g2, Q8_COLOR(0.6875f), Q8_COLOR(0.6875f), Q8_COLOR(0.6875f));
        }
        else
        {
            setRGB0(line_g2, Q8_COLOR(0.6875f), Q8_COLOR(0.6875f), Q8_COLOR(0.6875f));
            setRGB1(line_g2, Q8_COLOR(0.625f),  128,    64);
        }

        setXY2(line_g2,
               adjLines[i].vx, adjLines[i].vy,
               adjLines[(i + 1) % 6].vx,adjLines[(i + 1) % 6].vy);

        addPrim((u8*)ot->org + LAYER_28, line_g2);
        GsOUT_PACKET_P = (u8*)line_g2 + sizeof(LINE_G2);
    }
}

void SaveScreen_ElementInfoDraw(s32 slotIdx, s32 selectedSaveIdx) // 0x801E5E18
{
    const char* const labelStrs[] = {
#if VERSION_REGION_IS(NTSCJ)
        "data",
        "save",
        "time",
#else
        "Data",
        "Save",
        "Time",
#endif
        "hyper blaster",
        "power up",
#if !VERSION_REGION_IS(NTSCJ)
        "You_need_1_free_block\n__to_create_a_new_file."
#endif
    };

    GsOT*                   ot;
    s32                     saveId;
    s32                     mins;
    s32                     beamColorFlag;
    s32                     sec;
    s32                     hours;
    s32                     saveDataIdx;
    s32                     i;
    s32                     digitCount;
    u32                     hyperBlasterBeamColor;
#if VERSION_EQUAL_OR_NEWER(USA)
    u32                     timeInSec;
#else
    s32                     timeInSec;
#endif
    s_MemCard_SaveMetadata* ptr;
    POLY_G4*                poly;

    ot = &g_OrderingTable2[g_ActiveBufferIdx];

    g_MemCard_ActiveMemCardSlotSaves = MemCard_ActiveMemCardSlotGet(slotIdx);

#if !VERSION_REGION_IS(NTSCJ)
    if (g_MemCard_ActiveMemCardSlotSaves[selectedSaveIdx].type == SavegameEntryType_NewFile)
    {
        Gfx_StringColorSet(StringColorId_White);
        Gfx_StringPositionSet(66, 178);
        Gfx_StringDraw(labelStrs[5], 50);
    }
    else
#endif
    if (g_MemCard_ActiveMemCardSlotSaves[selectedSaveIdx].type == SavegameEntryType_Save)
    {
        saveId      = g_MemCard_ActiveMemCardSlotSaves[selectedSaveIdx].savegameCount;
        saveDataIdx = g_MemCard_ActiveMemCardSlotSaves[selectedSaveIdx].elementIdx + 1;

        saveId = CLAMP(saveId, 0, 999);

        ptr = g_MemCard_ActiveMemCardSlotSaves[selectedSaveIdx].saveMetadata;

#if VERSION_EQUAL_OR_NEWER(USA)
        timeInSec = FP_FROM(ptr->gameplayTimer, Q12_SHIFT);
#else
        timeInSec = FP_FROM((s32)ptr->gameplayTimer, Q12_SHIFT);
#endif

        hours = (timeInSec / 3600) + (ptr->add290Hours * 290);
        mins = (timeInSec / 60) % 60;
        sec  = timeInSec % 60;

        hyperBlasterBeamColor = ptr->pickedUpSpecialItemCount;

        Gfx_StringColorSet(StringColorId_White);
        Gfx_StringPositionSet(40, 178);
        Gfx_StringDraw(labelStrs[0], 5);

        digitCount = (saveDataIdx < 10) ? 1 : 0;

        Gfx_StringPositionSet((digitCount * 5) + 92, 178);
        Gfx_StringDrawInt(2, saveDataIdx);

        Gfx_StringPositionSet(40, 196);
        Gfx_StringDraw(labelStrs[1], 5);

        digitCount = (saveId < 10) ? 1 : 0;

        Gfx_StringPositionSet((digitCount * 5) + 92, 196);
        Gfx_StringDrawInt(2, saveId);

        Gfx_StringPositionSet(128, 178);
        Gfx_StringDraw(labelStrs[2], 5);

        digitCount = 0;
        if (hours < 100)
        {
            if (hours >= 10)
            {
                digitCount = 1;
            }
            else
            {
                digitCount = 2;
            }
        }

        Gfx_StringPositionSet((digitCount * 10) + 176, 178);
        Gfx_StringDrawInt(3, hours);
        Gfx_StringDraw("_:_", 3);

        digitCount = (mins < 10) ? 1 : 0;

        Gfx_StringPositionSet((digitCount * 10) + 220, 178);
        Gfx_StringDrawInt(2, mins);
        Gfx_StringDraw("_:_", 3);

        digitCount = (sec < 10) ? 1 : 0;

        Gfx_StringPositionSet((digitCount * 10) + 254, 178);
        Gfx_StringDrawInt(2, sec);

        // Check if no special hyper blaster beam color is unlocked.
        if (!(hyperBlasterBeamColor & 0x18))
        {
            return;
        }

        Text_Debug_PositionSet(152, 194);
        Text_Debug_Draw(labelStrs[3]);
        Text_Debug_PositionSet(172, 202);
        Text_Debug_Draw(labelStrs[4]);

        for (i = 0; i < 2; i++)
        {
            poly = (POLY_G4*)GsOUT_PACKET_P;
            setPolyG4(poly);
            setSemiTrans(poly, true);

            beamColorFlag = hyperBlasterBeamColor & 0x10;

            if (i != 0)
            {
                setRGB0(poly, (beamColorFlag > 0) ? 0 : 255, 255, 0);
                setRGB2(poly, (beamColorFlag > 0) ? 0 : 255, 255, 0);
                setRGB1(poly, 0, 0, 0);
                setRGB3(poly, 0, 0, 0);
                setXY4(poly, -30, 89, -30, 93, 120, 89, 120, 93);
            }
            else
            {
                setRGB0(poly, 0, 0, 0);
                setRGB2(poly, 0, 0, 0);
                setRGB1(poly, (beamColorFlag > 0) ? 0 : 255, 255, 0);
                setRGB3(poly, (beamColorFlag > 0) ? 0 : 255, 255, 0);
                setXY4(poly, -30, 85, -30, 89, 120, 85, 120, 89);
            }

            GsOUT_PACKET_P = (u8*)poly + sizeof(POLY_G4);
            addPrim((u8*)ot->org + LAYER_28, poly);

            Gfx_Primitive2dTextureSet(0, 0, 7, 1);
        }
    }
}

void GameState_LoadSavegameScreen_Update(void) // 0x801E6320
{
    g_SaveScreen_DisplaySaveInfo = true;

    if (g_GameWork.gameStateSteps[0] == 0)
    {
        g_GameState_SaveScreen_Funcs[0]();
    }

    g_GameState_SaveScreen_Funcs[g_GameWork.gameStateSteps[0]]();

    SaveScreen_BackgroundAndInfoDraw();
    SaveScreen_MemCardState();
    SaveScreen_ScreenDraw();

    if (g_GameWork.gameState == GameState_SaveScreen)
    {
        Bgm_MenuUpdate();
    }
}

void SaveScreen_Init(void) // 0x801E63C0
{
    if (g_GameWork.gameStatePrev == GameState_MainMenu)
    {
        VSync(SyncMode_Wait8);
    }

    g_IntervalVBlanks = 1;
    Screen_FadeStart(true, true, false);

    g_GameWork.background2dColor.r = 0;
    g_GameWork.background2dColor.g = 0;
    g_GameWork.background2dColor.b = 0;

    g_SaveScreen_IsLoadError = false;
    if (g_GameWork.gameState == GameState_AutoLoadSavegame || g_GameWork.gameState == GameState_LoadSavegameScreen)
    {
        if (g_SaveScreen_IsInSaveScreen)
        {
            g_MemCard_AllMemCardsStatus = 0;
        }
    }

    g_SaveScreen_MemCardStateTextTimer = 0;
    g_SaveScreen_IsInSaveScreen        = g_GameWork.gameState == GameState_SaveScreen;

    SaveScreen_ScreenInfoClear();
    Game_StateStepIncrement(0);
}

void SaveScreen_LogicUpdate(void) // 0x801E649C
{
    s32                  gameStateStep = g_GameWork.gameStateSteps[1];
    s_SaveScreenElement* saveEntry;
    SAVE_BSS bool          isSaveWriteOptionSelected;

    switch (gameStateStep)
    {
        case 0:
            if (MemCard_ElementsUpdate() == false)
            {
                break;
            }

            // Memory cards are inserted and user is moving between slots.
            if (g_Savegame_ElementCount0[0] != 0 && g_Savegame_ElementCount0[1] != 0 &&
                Pad_ButtonClicked(ControllerFlag_LStickHighRight | ControllerFlag_LStickHighLeft))
            {
                g_SelectedSaveSlotIdx ^= 1;
                SD_Call(Sfx_MenuMove);
            }

            // A memory card is inserted.
            if (g_MemCard_TotalElementsCount > 0)
            {
                g_SaveScreen_IsFormatting        = false;
                g_SaveScreen_IsNewSaveSelected   = false;
                g_MemCard_ActiveMemCardSlotSaves = MemCard_ActiveMemCardSlotGet(g_SelectedSaveSlotIdx);

                // Move down savegame entry.
                if (Pad_ButtonPulsed(ControllerFlag_LStickHighUp))
                {
                    if (g_SlotElementSelectedIdx[g_SelectedSaveSlotIdx] != 0)
                    {
                        g_SlotElementSelectedIdx[g_SelectedSaveSlotIdx]--;
                        SD_Call(Sfx_MenuMove);
                    }
                }

                // Move up savegame entry.
                if (Pad_ButtonPulsed(ControllerFlag_LStickHighDown))
                {
                    if (g_SlotElementSelectedIdx[g_SelectedSaveSlotIdx] < g_Savegame_ElementCount0[g_SelectedSaveSlotIdx] - 1)
                    {
                        g_SlotElementSelectedIdx[g_SelectedSaveSlotIdx]++;
                        SD_Call(Sfx_MenuMove);
                    }
                }

                saveEntry                        = &g_MemCard_ActiveMemCardSlotSaves[g_SlotElementSelectedIdx[g_SelectedSaveSlotIdx]];
                g_MemCard_ActiveMemCardSlotSaves = saveEntry;
                g_SelectedDeviceId               = saveEntry->deviceId;
                g_SelectedFileIdx                = saveEntry->fileIdx;
                g_Savegame_SelectedElementIdx    = saveEntry->elementIdx;

                // Show "Yes/No" option.
                if (g_SaveScreen_SaveScreenState == SaveScreenState_Save)
                {
                    if (saveEntry->totalSavegameCount == 31600)
                    {
                        g_SaveScreen_IsFormatting = true;
                    }

                    // Defines if the selected element is the `New Save` option.
                    // @bug While an edge case, reaching the maximum makes it
                    // impossible to select or overwrite files.
                    if (saveEntry->totalSavegameCount >= 1 && saveEntry->totalSavegameCount <= 31099)
                    {
                        g_SaveScreen_IsNewSaveSelected = true;
                    }
                }

                // Overwrite or format savegame entry.
                if (Pad_ButtonClickedBind(enter))
                {
                    if (g_SaveScreen_IsFormatting | g_SaveScreen_IsNewSaveSelected)
                    {
                        isSaveWriteOptionSelected    = false;
                        Game_StateStepIncrement(1);
                    }
                    else
                    {
                        Game_StateStepSet(0, g_GameWork.gameStateSteps[0] + g_SaveScreen_SaveScreenState);
                    }

                    SD_Call(Sfx_MenuConfirm);
                }
            }

            // Exit save screen.
#if VERSION_EQUAL_OR_NEWER(JAP1)
            if (!Pad_ButtonClickedBind(enter))
#endif
            if Pad_ButtonClickedBind(cancel)
            {
                Screen_FadeStart(false, false, false);
                Game_StateStepSet(1, 2);
                SD_Call(Sfx_MenuCancel);

                if (g_GameWork.gameStatePrev == GameState_InventoryScreen)
                {
                    SD_Call(23);
                    GameFs_TitleGfxLoad();
                    GameFs_StreamBinSeek();
                }
            }
            break;

        case 1:
            if (MemCard_ElementsUpdate() == false)
            {
                break;
            }

            g_SaveScreen_MemCardStateTextTimer = 0;

            if (Pad_ButtonClicked(ControllerFlag_LStickHighLeft))
            {
                isSaveWriteOptionSelected = gameStateStep;
                SD_Call(Sfx_MenuMove);
            }

            if (Pad_ButtonClicked(ControllerFlag_LStickHighRight))
            {
                isSaveWriteOptionSelected = false;
                SD_Call(Sfx_MenuMove);
            }

            if (Pad_ButtonClickedBind(enter))
            {
                if (!isSaveWriteOptionSelected)
                {
                    Game_StateStepSet(0, gameStateStep);
                }
                else
                {
                    Game_StateStepSet(0, 2 + g_SaveScreen_IsNewSaveSelected);
                }
                SD_Call(Sfx_MenuConfirm);
            }

            // Cancel overwrite.
            if (Pad_ButtonClickedBind(cancel))
            {
                Game_StateStepSet(1, 0);
                SD_Call(Sfx_MenuCancel);
            }

            SaveScreen_WriteOptionsDraw(g_SaveScreen_IsFormatting, isSaveWriteOptionSelected);
            break;

        case 2:
            if (Screen_IsFadeFinished())
            {
                g_GameWork.background2dColor.r = 0;
                g_GameWork.background2dColor.g = 0;
                g_GameWork.background2dColor.b = 0;

                Fs_QueueWaitForEmpty();

                // Unused or cut debug feature?
                // This redirects to the status screen/inventory if the user
                // was in the status screen/inventory before saving.
                // However, it is impossible to access the save screen from there.
                if (g_GameWork.gameStatePrev == GameState_InventoryScreen)
                {
                    Game_StateSetNext(g_GameWork.gameStatePrev);
                }
                else
                {
                    if (g_SaveScreen_SaveScreenState == gameStateStep)
                    {
                        MemCard_SysDisable();
                        Game_StateSetNext(GameState_InGame);
                    }
                    else
                    {
                        Game_StateSetNext(GameState_MainMenu);
                    }
                }
            }
            break;
    }
}

#if VERSION_IS(EUR)
INCLUDE_ASM("screens/saveload/nonmatchings/saveload", SaveScreen_FormatCard);
#else
void SaveScreen_FormatCard(void) // 0x801E69E8
{
    #define STR_TIMER_MAX (TICKS_PER_SECOND / 2)

    // Handle memory card format state.
    switch (g_GameWork.gameStateSteps[1])
    {
        case 0:
            g_SaveScreen_State                 = SaveScreenState_Format;
            g_SaveScreen_MemCardStateTextTimer = STR_TIMER_MAX;
            Game_StateStepSet(1, 1);

        case 1:
            Game_StateStepIncrement(1);
            break;

        case 2:
            MemCard_ProcessSet(MemCardGameProcessId_Format, g_SelectedDeviceId, 0, 0);
            Game_StateStepIncrement(1);
            break;

        case 3:
            switch (MemCard_LastMemCardResultGet())
            {
                case MemCardWorkResult_FileIoError:
                    g_SaveScreen_MemCardStateTextTimer = STR_TIMER_MAX;
                    Game_StateStepSet(0, 1);
                    break;

                case MemCardWorkResult_FileIoComplete:
                    Game_StateStepIncrement(0);
                    break;
            }
            break;
    }

    #undef STR_TIMER_MAX
}
#endif

#if VERSION_IS(EUR)
INCLUDE_ASM("screens/saveload/nonmatchings/saveload", SaveScreen_SaveGame);
#else
void SaveScreen_SaveGame(void) // 0x801E6B18
{
    s_MemCard_SaveMetadata* saveEntry;

    // Handle save state.
    switch (g_GameWork.gameStateSteps[1])
    {
        case 0:
            g_SaveScreen_State = SaveScreenState_Save;
            saveEntry          = MemCard_SaveMetadataGet(g_SelectedDeviceId, g_SelectedFileIdx, g_Savegame_SelectedElementIdx);

            if (g_SavegamePtr->locationId == SaveLocationId_NextFear)
            {
                saveEntry->savegameCount = 0;
            }
            else
            {
                g_SavegamePtr->savegameCount++;
                saveEntry->savegameCount = g_SavegamePtr->savegameCount;
            }

            saveEntry->locationId    = g_SavegamePtr->locationId;
            saveEntry->gameplayTimer = g_SavegamePtr->gameplayTimer;

            g_SaveScreen_IsNextFearMode = saveEntry->isNextFearMode;

            saveEntry->isNextFearMode           = g_SavegamePtr->isNextFearMode;
            saveEntry->add290Hours              = g_SavegamePtr->add290Hours;
            saveEntry->pickedUpSpecialItemCount = g_SavegamePtr->pickedUpSpecialItemCount;

            MemCard_ProcessSet(MemCardGameProcessId_Save_Game, g_SelectedDeviceId, g_SelectedFileIdx, g_Savegame_SelectedElementIdx);
            Game_StateStepIncrement(1);

        case 1:
            switch (MemCard_LastMemCardResultGet())
            {
                default:
                    Game_StateStepSet(0, 1);
                    break;

                case MemCardWorkResult_Success:
                    g_SaveScreen_MemCardStateTextTimer = 30;
                    break;

                case MemCardWorkResult_FileIoComplete:
                    MemCard_ProcessSet(MemCardGameProcessId_Save_Settings, g_SelectedDeviceId, g_SelectedFileIdx, 0);
                    Game_StateStepIncrement(1);
                    break;
            }
            break;

        case 2:
            switch (MemCard_LastMemCardResultGet())
            {
                case MemCardWorkResult_Success:
                    g_SaveScreen_MemCardStateTextTimer = 30;
                    break;

                case MemCardWorkResult_FileIoComplete:
                    g_GameWork.autosave = g_GameWork.savegame;

                default:
                    Game_StateStepSet(0, 1);
                    break;
            }
            break;
    }
}
#endif

#if VERSION_IS(EUR)
INCLUDE_ASM("screens/saveload/nonmatchings/saveload", SaveScreen_LoadSave);
#else
void SaveScreen_LoadSave(void) // 0x801E6DB0
{
    s32 memCardStateResult;

    // Handle load state.
    switch (g_GameWork.gameStateSteps[1])
    {
        case 0:
            g_SaveScreen_State = SaveScreenState_Load;

            MemCard_ProcessSet(MemCardGameProcessId_Load_Settings, g_SelectedDeviceId, g_SelectedFileIdx, g_Savegame_SelectedElementIdx);
            Game_StateStepIncrement(1);

        case 1:
            memCardStateResult = MemCard_LastMemCardResultGet();
            if (memCardStateResult != MemCardWorkResult_Success)
            {
                if (memCardStateResult != MemCardWorkResult_FileIoComplete)
                {
                    g_SaveScreen_IsLoadError = true;
                    Game_StateStepSet(0, 1);
                    break;
                }

                MemCard_ProcessSet(MemCardGameProcessId_Load_Game, g_SelectedDeviceId, 0, 0);
                Game_StateStepIncrement(1);
                break;
            }

            g_SaveScreen_MemCardStateTextTimer = 30;
            break;

        case 2:
            memCardStateResult = MemCard_LastMemCardResultGet();
            if (memCardStateResult == MemCardWorkResult_Success)
            {
                g_SaveScreen_MemCardStateTextTimer = 30;
                break;
            }

            if (memCardStateResult != MemCardWorkResult_FileIoComplete)
            {
                g_SaveScreen_IsLoadError = true;
                Game_StateStepSet(0, 1);
                break;
            }

            g_SaveScreen_MemCardStateTextTimer = 300;

            Game_StateStepIncrement(0);
            break;
    }
}
#endif

void SaveScreen_Continue(void) // 0x801E6F38
{
    // Handle continue state.
    switch (g_GameWork.gameStateSteps[1])
    {
        case 0:
            MemCard_SysDisable();

            D_800A97D7          = 1;
            g_GameWork.autosave = g_GameWork.savegame;

            GameBoot_WorldInit();

            g_SysWork.processFlags = ProcessFlag_LoadSave;

            GameBoot_MapLoad(g_SavegamePtr->mapIdx);
            Screen_FadeStart(true, false, false);
            Game_StateStepIncrement(1);
            break;

        case 1:
        {
            if (Screen_IsFadeFinished())
            {
                Fs_QueueWaitForEmpty();
                Settings_ScreenAndVolUpdate();
                Game_StateSetNext(GameState_MainLoadScreen);
            }
            break;
        }
    }
}

void SaveScreen_BackgroundAndInfoDraw(void) // 0x801E709C
{
    // Draw "SLOT 1"/"SLOT 2" strings and bottom transparent rectangle.
    SaveScreen_SlotStrAndBottomRectDraw();

    // Draws background image.
    Screen_BackgroundImgDraw(&g_ItemInspectionImg);
}

void SaveScreen_ScreenDraw(void) // 0x801E70C8
{
    s32                  i;
    s32                  j;
    s_SaveScreenElement* nextSaveEntry;

    // Run through save slots.
    for (i = 0; i < MEMCARD_SLOT_COUNT_MAX; i++)
    {
        g_MemCard_ActiveMemCardSlotSaves = MemCard_ActiveMemCardSlotGet(i);

        // Run through savegame entries.
        for (j = 0; j < (s32)g_Savegame_ElementCount1[i]; j++)
        {
            if (g_MemCard_ActiveMemCardSlotSaves->totalSavegameCount >= 0)
            {
                SaveScreen_FileIdxDraw(j, i, g_MemCard_ActiveMemCardSlotSaves->fileIdx + 1, g_MemCard_ActiveMemCardSlotSaves->type);
            }

            SaveScreen_SavesSlotDraw(g_MemCard_ActiveMemCardSlotSaves, j, i);

            if (g_MemCard_ActiveMemCardSlotSaves->type >= SavegameEntryType_CorruptedSave)
            {
                nextSaveEntry = g_MemCard_ActiveMemCardSlotSaves;
                if (j != g_Savegame_ElementCount1[i])
                {
                    nextSaveEntry++;
                }

                SaveScreen_SaveBorder(g_MemCard_ActiveMemCardSlotSaves, nextSaveEntry, j, i);
            }

            if (g_MemCard_ActiveMemCardSlotSaves->type == SavegameEntryType_Save)
            {
                SaveScreen_SaveLocationDraw(g_MemCard_ActiveMemCardSlotSaves, j, i);
            }

            g_MemCard_ActiveMemCardSlotSaves++;
        }

        if (g_GameWork.gameState == GameState_AutoLoadSavegame)
        {
            SaveScreen_SelectedElementIdxUpdate(i, g_MemCard_SavegameCount);
        }
        else
        {
            SaveScreen_SelectedElementIdxUpdate(i, g_MemCard_TotalElementsCount);
        }
    }
}

void SaveScreen_MemCardState(void) // 0x801E7244
{
    if (g_SaveScreen_MemCardStateTextTimer <= 0)
    {
        return;
    }

    g_SaveScreen_DisplaySaveInfo = false;
    g_SaveScreen_MemCardStateTextTimer--;

    switch (g_SaveScreen_State)
    {
        case SaveScreenState_Format:
            SaveScreen_MemCardStateDraw(g_SaveScreen_State, MemCard_StatusGet(g_MemCard_AllMemCardsStatus, g_SelectedDeviceId));
            break;

        case SaveScreenState_Save:
        case SaveScreenState_Load:
            SaveScreen_MemCardStateDraw(g_SaveScreen_State, MemCard_LastMemCardResultGet());
            break;
    }
}

void SaveScreen_WriteOptionsDraw(s32 stringIdx, bool optionSelected) // 0x801E72DC
{
    SaveScreen_WriteOptionsStepDraw(stringIdx, optionSelected);
}

void GameState_AutoLoadSavegame_Update(void) // 0x801E72FC
{
    if (g_GameWork.gameStateSteps[0] == 0)
    {
        g_GameState_AutoLoadSavegame_Funcs[0]();
    }

    g_GameState_AutoLoadSavegame_Funcs[g_GameWork.gameStateSteps[0]]();

    SaveScreen_BackgroundAndInfoDraw();
    SaveScreen_MemCardState();
    SaveScreen_ScreenDraw();
}

void SaveScreen_AutoLoadSaveSelect(void) // 0x801E737C
{
    // Return if memory cards not ready yet, will rerun on next tick.
    if (MemCard_ElementsUpdate() == false)
    {
        return;
    }

    // No savegames or load error, switch to manual load screen.
    if (g_MemCard_SavegameCount == 0 || g_SaveScreen_IsLoadError || MemCard_ElementsUpdate() == false)
    {
        g_GameWork.gameState = GameState_LoadSavegameScreen;

        if (g_GameWork.gameStatePrev == GameState_KcetLogo)
        {
            g_GameWork.gameStateSteps[0] = 1;
            g_GameWork.gameStateSteps[1] = g_GameWork.gameStatePrev;

            Screen_FadeStart(false, false, false);

            GameFs_TitleGfxLoad();
        }

        return;
    }

    // Setup globals for `g_SelectedSaveSlotIdx`.
    g_MemCard_ActiveMemCardSlotSaves = MemCard_ActiveMemCardSlotGet(g_SelectedSaveSlotIdx);
    g_MemCard_ActiveMemCardSlotSaves = &g_MemCard_ActiveMemCardSlotSaves[g_SlotElementSelectedIdx[g_SelectedSaveSlotIdx]];
    g_SelectedDeviceId               = g_MemCard_ActiveMemCardSlotSaves->deviceId;
    g_SelectedFileIdx                = g_MemCard_ActiveMemCardSlotSaves->fileIdx;
    g_Savegame_SelectedElementIdx    = g_MemCard_ActiveMemCardSlotSaves->elementIdx;

    // Advance to next autoload state.
    Game_StateStepSet(0, g_GameWork.gameStateSteps[0] + 1);
}

