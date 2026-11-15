#ifndef _BODYPROG_ITEMS_UTILS_H
#define _BODYPROG_ITEMS_UTILS_H

/** @brief Commands for controlling chainsaw/rock-drill looping sound effects, used with `Game_GasWeaponSfxUpdate`. */
typedef enum _GasWeaponSfxCmd
{
    GasWeaponSfxCmd_Reset    = NO_VALUE, /** Stops all gas weapon sounds and clears state. */
    GasWeaponSfxCmd_Start    = 0,        /** Starts playing `Sfx_ChainsawRun` or `Sfx_RockDrillStart`, called partway through startup anim. */
    GasWeaponSfxCmd_StartEnd = 1,        /** Plays `Sfx_RockDrillRun` for the rock drill, once startup anim has finished. */
    GasWeaponSfxCmd_WindDown = 2,        /** Winds `Sfx_RockDrillRun` down in volume and pitch, then stops all sounds, when the weapon runs out of power? */
    GasWeaponSfxCmd_Cancel   = 3,        /** Stops `Sfx_RockDrillStart` and empties the power timer. */
    GasWeaponSfxCmd_Update   = 4         /** Keeps the weapon's sounds looping, restarts them after a map change. */
} e_GasWeaponSfxCmd;

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

/** @brief Drives the chainsaw and rock drill sounds.
 *
 * @param weaponAttack Equipped weapon attack.
 * @param cmd `e_GasWeaponSfxCmd`
 */
void Game_GasWeaponSfxUpdate(u8 weaponAttack, s8 cmd);

void Game_TimerUpdate(void);

#endif
