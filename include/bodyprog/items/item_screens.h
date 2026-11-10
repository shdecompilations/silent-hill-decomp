#ifndef _BODYPROG_ITEMSCREENS_H
#define _BODYPROG_ITEMSCREENS_H

/** @brief Inventory and result screen system. */

// ==========
// CONSTANTS
// ==========

#define DISPLAYED_ITEM_COUNT_MAX 10

// ======
// ENUMS
// ======

/** @brief Inventory item flags. */
typedef enum _InvItemFlags
{
    InvItemFlag_PlateOfTurtle   = 1 << 0,
    InvItemFlag_PlateOfHatter   = 1 << 1,
    InvItemFlag_PlateOfCat      = 1 << 2,
    InvItemFlag_PlateOfQueen    = 1 << 3,
    InvItemFlag_Lighter         = 1 << 4,
    InvItemFlag_Camera          = 1 << 5,
    InvItemFlag_VideoTape       = 1 << 6,
    InvItemFlag_ChannelingStone = 1 << 7
} e_InvItemFlags;

/** @brief Inventory selection ID. */
typedef enum _InvSelectionId
{
    InvSelectionId_Item            = 0,
    InvSelectionId_EquippedItem    = 1,
    InvSelectionId_Exit            = 2,
    InvSelectionId_Settings        = 3,
    InvSelectionId_Map             = 4,
    InvSelectionId_ItemCmd         = 5,
    InvSelectionId_EquippedItemCmd = 6,
    InvSelectionId_Health          = 7, // Used for animation when user selects health item.
    InvSelectionId_Examine         = 8
} e_InvSelectionId;

// ========
// STRUCTS
// ========

typedef struct _ResultStringOffset
{
    u8 x0_0;
    u8 y0_1;
    u8 x1_2;
    u8 y1_3;
} s_ResultStringOffset;
STATIC_ASSERT_SIZEOF(s_ResultStringOffset, 4);

typedef struct
{
    DVECTOR field_0;
    DVECTOR field_4;
} s_Inventory_SelectionOutlineVector;

typedef struct
{
    u32* ptr_0;
    u32* ptr_4;
    s16  field_8;
    s16  field_A;
} s_800AEDBC;

typedef struct
{
    u8 field_0;
    u8 field_1;
    u8 field_2;
} s_800AE710;

// ========
// GLOBALS
// ========

extern s32 g_Inventory_CmdSelectedIdx; // 0x800ADB58

/** @brief Defines what the user has selected in the inventory.
 * `e_InvSelectionId`
 *
 * @note
 * `g_Inventory_PrevSelectionId` has the same behaviour, with
 * the main difference being that it can be modified in memory
 * while `g_Inventory_PrevSelectionId` is constantly updated to the same value
 * as this variable.
 */
extern u32 g_Inventory_SelectionId; /** `e_InvSelectionId` */ // 0x800ADB5C

/** @brief Has user started scrolling. Dead code?
 * Only used when the user starts to scrolling through the inventory, but as soon
 * as it's assigned to 1, `Gfx_Inventory_ItemDescriptionDraw` makes it 0 again.
 * Serves no purpose beyond this.
 */
extern s32 D_800AE178;

extern s32 g_Inventory_SelectedItemIdx; // 0x800AE17C

/** @brief Loaded item count. Dead code?
 * Appears to count the number of times the user has scrolled in the inventory.
 * Tied to the function which assigns models. Caps at 10.
 * If the user opens any item screen (i.e. inventory or results), the counter restarts.
 * Serves no purpose beyond this.
 */
extern s32 g_Items_DisplayedCount; // 0x800AE180

/** `e_InvItemId` */
extern u8 g_Inventory_EquippedItem; // 0x800AE184

extern s32 D_800AE190;

/** Timer used for the black scanline gradient in the health status effect. */
extern s16 g_Inventory_HealthStatusScanlineTimer; // 0x800AE194

/** Timer used for the colored gradient in the health status effect. */
extern s16 g_Inventory_HealthStatusColorGradientTimer; // 0x800AE196

extern s16 D_800AE198; // 0x800AE198

extern s16 g_Inventory_HealthStatusScanlinePosition; // 0x800AE19A

extern s32 g_Inventory_ItemNameTimer; // 0x800AE19C

/** @brief Timer used for displaying item descriptions in the inventory. */
extern s32 g_Inventory_DescriptionRollTimer; // 0x800AE1A0

// ====================
// GLOBALS (BSS; Hack)
// ====================
// To match the order of the BSS segment, extern declarations
// are required in a predetermined order.
// This is done until a way to replicate `common`
// segment behavior is found.

// cam.c

extern GsCOORD2PARAM g_ItemScreen_CameraTransform;

/** Packed weapon attack. See `WEAPON_ATTACK`. */
extern s8 g_ItemScreen_PlayerWeaponAttack;

extern s8 __pad_bss_800C3951[3];

extern s32 g_ItemScreen_ViewDistance;

extern s32 g_ItemScreen_GeomOffsetX;

extern s32 g_ItemScreen_GeomOffsetY;

// 2.c

extern bool g_Inventory_IsUpClicked;
extern bool g_Inventory_IsDownClicked;
extern bool g_Inventory_IsLeftClicked;
extern bool g_Inventory_IsRightClicked;
extern bool g_Inventory_IsUpPulsed;
extern bool g_Inventory_IsDownPulsed;
extern bool g_Inventory_IsLeftPulsed;
extern bool g_Inventory_IsRightPulsed;
extern bool g_Inventory_IsLeftHeld;
extern bool g_Inventory_IsRightHeld;

/** @brief Used for smooth inventory scrolling if a left/right input is held. */
extern bool g_Inventory_IsScrolling;

/** `e_GameDifficulty` */
extern s32 g_Inventory_GameDifficulty;

/** @brief Timer used for the corner borders of the selection box in the inventory.
 *
 * If set to 8, borders will be drawn. If not, it increments until it reaches 8.
 */
extern s32 g_Inventory_SelectionBordersDraw;

/** @brief Used for drawing the transition of the corner borders' position when changing
 * the selected option in the inventory. Indicates where the box is moving to.
 *
 * `e_InvSelectionId`
 *
 * Difficult to notice, but this shares the same value as
 * `g_Inventory_SelectionId`, with the difference of a minor delay.
 */
extern s32 g_Inventory_PrevSelectionId;

extern s32 __pad_bss_800C39A0[2];

// 3.c

extern GsF_LIGHT g_Items_Lights[7][2];

extern GsF_LIGHT D_800C3A88[4];

extern GsF_LIGHT D_800C3AC8[2];

extern GsCOORDINATE2 g_ItemScreen_CameraCoord;

/** Used for displaying the item models.
 * Could also be camera values. @unused Dead code? Only set to 0 by `ItemScreen_CameraSet`.
 */
extern SVECTOR3 g_ItemScreen_CameraRotation;

extern s16 __pad_bss_800C3B3E[5];

extern VbRVIEW g_ItemScreen_CameraView;

/** Holds the coords for displaying selection box around items and options. */
extern DVECTOR D_800C3B68[4][4]; // Type assumed.

/** @brief Timer used for the transition of selected items
* when scrolling through the inventory.
*/
extern s32 g_Inventory_ScrollTransitionTimer;

extern s32 D_800C3BA8;

extern s32 D_800C3BAC;

extern s32 __pad_bss_800C3BB0[2];

extern u8 g_Item_MapLoadableItems[48];

extern GsCOORD2PARAM g_Items_Transforms[DISPLAYED_ITEM_COUNT_MAX];

/** Stores item model data. */
extern GsDOBJ2 g_Items_ItemsModelData[9];

extern GsDOBJ2 D_800C3E08;

/** Used to organize the items displaying in the inventory.
 * 7 is the max number of visible items.
 */
extern s32 D_800C3E18[7];

/** This value is based on the index of the equipped item.
 * If the player has nothing equipped, set to `NO_VALUE`.
 */
extern s32 g_Inventory_EquippedItemIdx;

extern s32 __pad_bss_800C3E38[2];

// TODO: Unsure about name, depends on certain extra options being enabled.
extern u8 g_Ranking_ExtraItemFlags;

extern s8 __pad_bss_800C3E41[7];

extern GsCOORDINATE2 g_Items_Coords[DISPLAYED_ITEM_COUNT_MAX];

// ==========
// FUNCTIONS
// ==========

/** Appears to set the camera for items. `itemRot` is always cleared to the identity rotation. */
void ItemScreen_CameraSet(VbRVIEW* camView, GsCOORDINATE2* camCoord, SVECTOR3* itemRot, s32 unused1); // 0x8004BB4C

void GameFs_TmdDataAlloc(s32* buf); // 0x8004BCBC

/** @brief Used for item rotation and scrolling in the inventory.
 *
 * Name might be inaccurate as it also appears to be used for the camera.
 */
void ItemScreen_ItemRotate(const SVECTOR* itemRot, GsCOORDINATE2* itemCoord); // 0x8004BCDC

void func_8004BD74(s32 displayItemIdx, GsDOBJ2* obj, s32 arg2); // 0x8004BD74

void func_8004BFE8(void); // 0x8004BFE8

void func_8004C040(void); // 0x8004C040

/** @brief Used to correct aim animations when exiting the inventory screen.
 *
 * If removed, when the user enters the inventory between the transition of the
 * aim animation to the idle animation, then changes the weapon to another
 * type (e.g. from a gun weapon to a melee weapon), Harry will continue
 * aiming until he interacts with something or starts running.
 *
 * @param weaponAttack @unused
 */
void Inventory_ExitAnimEquippedItemUpdate(u8* weaponAttack); // 0x8004C088

/** Returns `true` if player has usable ammo in inventory (i.e. ammo + gun needed
 * for it, or gun with ammo loaded) for the shotgun and the hunting rifle.
 *
 * @note Related to the ending unlockable if out of ammo. However, that
 * ending requires the handgun to be out of ammo too. This function is also triggered
 * during the boss fight.
 */
bool Inventory_GunsAvailableCheck(bool unused); // 0x8004C328

/** @brief Used to check if the Hyper Blaster can be added to the inventory.
 *
 * @return -1 if it already exists in the inventory,
 *          1 if the Konami gun controller is connected or the savegame flag is set,
 *          0 otherwise.
 */
s32 Inventory_HyperBlasterUnlockTest(void); // 0x8004C45C

/** @brief Called by status screen to check if Hyper Blaster can be used?
 *
 * @return 2 if the savegame flag is set,
 *         1 if the Konami gun controller is connected,
 *         0 otherwise.
 */
s32 Inventory_HyperBlasterFunctionalTest(void); // 0x8004C4F8

/** @note Rough name. */
s32 Game_HyperBlasterBeamColorGet(void); // 0x8004C54C

void Game_TimerUpdate(void); // 0x8004C8DC

void GameState_ItemScreens_Update(void); // 0x8004C9B0

void Gfx_Results_Save(void); // 0x8004D1A0

/** Function mainly responsible for handling the inventory screen. */
void Inventory_Update(void); // 0x8004D518

/** @brief Used for environmental item interactions such as keys, puzzle objects, or
 * events like the Cybil boss fight.
 *
 * @note This is where the Cybil boss fight skip bug occurs. The function is in
 * charge of triggering the event of Harry throwing the unknown liquid over
 * Cybil, however, the code doesn't check if the NPC that is being interacted
 * with is Cybil, allowing the player to trigger the bug by interacting with
 * the first NPC loaded in `g_SysWork`.
 */
void Inventory_ItemUse(s32 arg0); // 0x8004E6D4

void Gfx_Inventory_CmdOptionsDraw(void); // 0x8004E864

void Gfx_Inventory_ScrollArrowsDraw(s32* invSelectionId); // 0x8004EC7C

bool Player_ItemRemove(u8 itemId, u8 count);

/** Sets the possible interaction commands for all items in the inventory. */
void Inventory_ItemCommandSet();

// Returns inventory item slot count.
s32 func_8004F190(s_Savegame* save);

/** @unused */
void func_8004F10C(s32* arg0);

/** @brief Draws the two messages that indicate when the player can't open
 * the map in the inventory.
 */
void Inventory_UnavailableMapTextDraw(s32 strIdx); // 0x8004F57C

void Inventory_DirectionalInputSet(void); // 0x8004F5DC

void Gfx_ItemScreens_DrawInit(u32* selectedItemId);

/** Draws some rectangle. */
void func_8004FB0C(void);

void Gfx_Inventory_HealthStatusDraw(void);

void Gfx_Inventory_ItemDescriptionDraw(s32* selectedItemId);

/** Name could be inaccurate.
 * Breaking this function call makes items no longer rotate,
 * and trying to scroll through the items will softlock the game.
 * If the user breaks the function before entering the
 * inventory, all items will be at the center of the screen
 * and the equiped item will appear frozen in its base pose.
 * If the user doesn't try scrolling through items, they
 * will be able to navigate through other functions. However,
 * messing with commands also will softlock the game.
 */
void Inventory_PlayerItemScroll(u32* selectedItemId);

void Gfx_Results_ItemsDisplay(); // TODO: Might take s32 argument?

void Gfx_Results_ItemsPosition(); // TODO: Might take s32 argument?

#if VERSION_IS(JAP0)
void func_80053898(s32 arg0, s32 arg1);
void func_800539A0(s32 arg0, s32 arg1);
#endif

void Gfx_Primitive2dTextureSet(s32 x, s32 y, s32 otIdx, s32 abr);

/** @brief Toggles the visibility of object while the inventory scrolls.
 *
 * Used in: `Inventory_Update`
 */
void func_800539A4(s32 arg0, s32 arg1);

/** Related to file loading of inventory elements.
 * Loads Harry's potrait for the health status image.
 */
void func_800540A4(s8 arg0);

/** Related to drawing items. */
void Gfx_Items_Draw(void); // 0x80054200

/** Rough name. Part of this function resets values related to visual elements of the inventory. */
void Gfx_Items_DrawInit(void); // 0x80054558

/** Used for correct reset of aiming animations when exiting the inventory screen.
 *
 * Used in: `GameState_ItemScreens_Update`
 *
 * This calls two other functions which reset Harry's animations when exiting the inventory screen.
 * If removed, the player softlocks in the following scenario: Harry aims a weapon,
 * the user enters the inventory, unequips the weapon, and exits the inventory.
 * This results in Harry being unable to move and use wrong animations.
 */
void Inventory_ExitAnimFixes(void); // 0x80054634

/** Very rough param names. */
void Gfx_Items_Display(s_TmdFile* tmd, s32 displayItemIdx, s32 loadableItemIdx); // 0x80054720

/** Something related to items lighting. */
void func_800548D8(s32 idx); // 0x800548D8

void Gfx_Items_SetAmbientLighting(void); // 0x80054928

void Inventory_AddSpecialItem(u8 itemId, u8 itemCount); // 0x80054CAC

#endif
