#ifndef _SCREENS_SAVELOAD_H
#define _SCREENS_SAVELOAD_H

// ============
// ENUMERATORS
// ============

/** @brief Used by `g_SaveScreen_State`. */
typedef enum _SaveScreenState
{
    SaveScreenState_None   = 0,
    SaveScreenState_Format = 1,
    SaveScreenState_Save   = 2,
    SaveScreenState_Load   = 3
} e_SaveScreenState;

// ==========
// FUNCTIONS
// ==========

/** @brief Clears all variable states used by the save screen. */
void SaveScreen_ScreenInfoClear(void);

/** @brief Draws the bottom information rectangle.
 *
 * @note SH2 counterpart is name `mcDrawStatus`,
 * but a noticiable difference between both games is
 * that SH2 do not feature a background rectangle.
 */
void SaveScreen_SlotStrAndBottomRectDraw(void);

/** @brief Updates `g_SaveScreen_VisualElementIdx` based on the slot indicated.
 *
 * @param slotIdx Memory Card slot to update.
 * @param unused Intended to getting pass the total amount of elements in the slot.
 */
void SaveScreen_SelectedElementIdxUpdate(s32 slotIdx, s32 unused);

/** @brief Draws the "FILE X" string. */
void SaveScreen_FileIdxDraw(s32 saveIdx, s32 slotIdx, s32 fileId, s32 entryType);

/** @brief Update color of the save string to be drawn
 * and returns a boolean value stating if the save
 * is in "Next Fear" mode.
 *
 * @param saveEntry Save metadata entry being processed.
 * @return Returns true or false depending if the save is in "Next Fear" mode or not.
 */
bool SaveScreen_NextFearModeSave(s_MemCard_SaveMetadata* saveEntry);

/** @brief Draws location of saves. */
void SaveScreen_SaveLocationDraw(s_SaveScreenElement* saveEntry, s32 saveIdx, s32 slotIdx);

/** @brief Draws saves borders. */
void SaveScreen_SaveBorder(s_SaveScreenElement* saveEntry, s_SaveScreenElement* nextSaveEntry, s32 saveIdx, s32 slotIdx);

/** @brief Draws saves, information and slots. */
void SaveScreen_SavesSlotDraw(s_SaveScreenElement* saveEntry, s32 saveIdx, s32 slotIdx);

#if VERSION_REGION_IS(NTSCJ)
void func_801E4128(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_801E4334(s32 arg0, s32 saveIdx, s32 slotIdx, u8* str);
#endif

/** @brief Draws memory card state.
 *
 * @param saveScreenState `e_SaveScreenState`
 * @param memCardState `e_MemCardResult`
 */
void SaveScreen_MemCardStateDraw(s32 saveScreenState, s32 memCardState);

/** @brief Draws options writing options when the
 * player is overwriting a save or is formatting
 * the memory card.
 *
 * @param stringIdx String index used to determine to display the overwrite or format status options.
 * @param optionSelected Option currently selected by the player.
 */
void SaveScreen_WriteOptionsStepDraw(s32 stringIdx, bool optionSelected);

/** @brief Draws the flash that overlap saves after saving. */
void SaveScreen_SaveFlash(void); // 0x801E3E78

/** @brief Draws slot border.
 *
 * @note SH2 counterpart is name `mcDrawFrame`,
 * but in constrast with SH1 it draws the upper
 * text that indicates the slot.
 */
void SaveScreen_SlotBorder(void); // 0x801E4010

/** @brief Draws the shine which appears during an unusual
 * state where the memory card is "not formatted" and
 * `g_GameWork.gameState` is `GameState_SaveScreen`.
 *
 * @note Intended to be used alongside `SaveScreen_SlotStatusMsgDraw`.
 */
void SaveScreen_SlotStatusMsgShineDraw(s32 slotIdx);

/** @brief Draws slot's transparent background, scroll bar and selected element shine. */
void SaveScreen_NavigationDraw(s32 slotIdx, s32 saveCount, s32 selectedSaveIdx, s32 selectedSaveOffsetY);

/** @brief Draws a flash on the selected save menu entry after saving sucessfully. */
void Savegame_SaveEntryFlashDraw(void);

void SaveScreen_SaveBorderDraw(s_SaveScreenElement* saveEntry, s_SaveScreenElement* nextSaveEntry, s32 saveIdx, s32 slotIdx); // 0x801E4D90

/** @brief Defines parameters used to draw the message
 * box use when the memory card is in an unavailable state.
 *
 * For example: When loading the memory card or the memory card is not inserted.
 */
void SaveScreen_SlotStatusMsgDraw(s32 slotIdx, s32 entryType);

/** @brief Draws the message box use when the memory card
 * is in an unavailable state.
 */
void SaveScreen_SlotStatusMsgStepDraw(s_LineBorder* borderLines, s_QuadBorder* borderGlowQuads, s_ColoredLine2d* coloredLine, s32 slotIdx);

/** @brief Draws bottom information rectangle. */
void SaveScreen_RectSaveInfoDraw(s_Line2d* line); // 0x801E5898

/** @brief Draws save information or new save text in the bottom rectangle. */
void SaveScreen_ElementInfoDraw(s32 slotIdx, s32 selectedSaveIdx); // 0x801E5E18

/** @brief Updates the save screen. */
void GameState_LoadSavegameScreen_Update(void);

/** @brief Initialize save screen. */
void SaveScreen_Init(void);

/** Crucial for working with the overlay.
 *
 * `g_GameWork.gameStateSteps[1]` defines the state:
 * 0 - The user can freely move through the save menu.
 * 1 - Triggers the overwrite save.
 * 2 - Player leaves the save screen.
 */
void SaveScreen_LogicUpdate(void);

void SaveScreen_FormatCard(void);

void SaveScreen_SaveGame(void);

void SaveScreen_LoadSave(void);

void SaveScreen_Continue(void);

/** @brief Draws background and triggers save info draw. */
void SaveScreen_BackgroundAndInfoDraw(void);

/** @brief Draws background and triggers save info draw. */
void SaveScreen_ScreenDraw(void);

/** @brief Handles the text that shows when formatting, saving, or loading a file.
 *
 *  Used in: `GameState_LoadSavegameScreen_Update` and `GameState_AutoLoadSavegame_Update`.
 */
void SaveScreen_MemCardState(void);

void SaveScreen_WriteOptionsDraw(s32 stringIdx, bool optionSelected);

/** @brief Updates the death load screen. */
void GameState_AutoLoadSavegame_Update(void);

void SaveScreen_AutoLoadSaveSelect(void);

#endif
