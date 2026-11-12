#ifndef _BODYPROG_SYS_WARMBOOT_H
#define _BODYPROG_SYS_WARMBOOT_H

/** @brief Checks for a specific combination of inputs to determine if a soft reboot should take place.
 *
 * @return `ResetType_WarmBoot` if a soft reboot should take place, `ResetType_None` otherwise.
 * @note OPM counterpart of this function also returns `4` which is used to kill game's process.
 */
s32 MainLoop_ShouldWarmReset(void);

/** @brief Handles a warm game reboot. */
void Game_WarmBoot(void);

#endif
