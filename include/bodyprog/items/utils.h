#ifndef _BODYPROG_ITEMS_UTILS_H
#define _BODYPROG_ITEMS_UTILS_H

/** @brief Used to correct aim animations when exiting the inventory screen.
 *
 * If removed, when the user enters the inventory between the transition of the
 * aim animation to the idle animation, then changes the weapon to another
 * type (e.g. from a gun weapon to a melee weapon), Harry will continue
 * aiming until he interacts with something or starts running.
 *
 * @param weaponAttack @unused
 */
void Inventory_ExitAnimEquippedItemUpdate(u8* weaponAttack);

/** Returns `true` if player has usable ammo in inventory (i.e. ammo + gun needed
 * for it, or gun with ammo loaded) for the shotgun and the hunting rifle.
 *
 * @note Related to the ending unlockable if out of ammo. However, that
 * ending requires the handgun to be out of ammo too. This function is also triggered
 * during the boss fight.
 */
bool Inventory_GunsAvailableCheck(bool unused);

/** @brief Used to check if the Hyper Blaster can be added to the inventory.
 *
 * @return -1 if it already exists in the inventory,
 *          1 if the Konami gun controller is connected or the savegame flag is set,
 *          0 otherwise.
 */
s32 Inventory_HyperBlasterUnlockTest(void);

/** @brief Called by status screen to check if Hyper Blaster can be used?
 *
 * @return 2 if the savegame flag is set,
 *         1 if the Konami gun controller is connected,
 *         0 otherwise.
 */
s32 Inventory_HyperBlasterFunctionalTest(void);

/** @note Rough name. */
s32 Game_HyperBlasterBeamColorGet(void);

/** `flags` might not be a bitfield. */
void func_8004C564(u8 flags, s8 weaponAttack);

void Game_TimerUpdate(void);

#endif
