#ifndef _BODYPROG_EVENTS_EVENTSUTIL_H
#define _BODYPROG_EVENTS_EVENTSUTIL_H

/** @brief Background texture commands. */
typedef enum _BgTextureCmd
{
    BgTextureCmd_QueueRead          = 0,
    BgTextureCmd_AwaitLoad          = 1, // Increments `sysStateSteps[0]`/`sysStateSteps[1]` if file is loaded.
    BgTextureCmd_Draw               = 2,
    BgTextureCmd_StoreVram          = 3,
    BgTextureCmd_QueueReadSecondary = 4,
    BgTextureCmd_DrawSecondary      = 5,
    BgTextureCmd_RestoreVram        = 6,
    BgTextureCmd_Auto               = 7, // Command is kept inside `sysStateSteps[2]` which is then incremented as texture is loaded/drawn?
    BgTextureCmd_8                  = 8, // TODO: Queues load as either primary or secondary, depending on whether primary is in use? (`g_SysWork.sysStateSteps[2] != 0`)
                                         // `Event_BgTextureCmd` has weird code that treats this as a different cmd by changing `activeCmd`, but seems that would skip `BgTextureCmd_QueueRead` for primary texture load?
} e_BgTextureCmd;

/** @brief Character animation commands. */
typedef enum _CharaAnimCmd
{
    CharaAnimCmd_SetState     = 0,
    CharaAnimCmd_AwaitAnimEnd = 1, // Calls `Event_SysStateStepIncrement` when an animation reaches its end.
    CharaAnimCmd_AnimLock     = 2,
    CharaAnimCmd_AnimUnlock   = 3,
    CharaAnimCmd_AnimReset    = 4  // TODO: Unsure on name, could be a forced stop.
} e_CharaAnimCmd;

/** @brief Inventory item commands. */
typedef enum _InvItemCmd
{
    InvItemCmd_QueueLoad = 0,
    InvItemCmd_AwaitLoad = 1,
    InvItemCmd_Nop       = 2, // Does nothing? `activeCmd` is changed to 3, but then switch has no entry for `3`.
    InvItemCmd_AddItem   = 3, // Finalize + add to inventory (`activeCmd` changes to 2, which adds to inventory).
    InvItemCmd_4         = 4, // Auto, no inventory add? | @unused No caller passes this?
    InvItemCmd_5         = 5, // @unused No caller passes this?
    InvItemCmd_6         = 6, // Auto + add to inventory | @unused No caller passes thi, but code checks it.
} e_InvItemCmd;

/** @brief Paper map commands. */
typedef enum _PaperMapCmd
{
    PaperMapCmd_Load   = 0, /** Stores screen VRAM and loads paper map textures. */
    PaperMapCmd_Draw   = 1, /** @unused | TODO: No events use this cmd, how do they draw the map? */
    PaperMapCmd_Unload = 2, /** Restores VRAM and resets the screen. */

    PaperMapCmd_Hack   = -1 /** @hack Forces the enum as `s32`. */
} e_PaperMapCmd;

/** @brief Screen fade commands. */
typedef enum _ScreenFadeCmd
{
    ScreenFadeCmd_Start = 0,
    ScreenFadeCmd_Wait  = 1,
    ScreenFadeCmd_Auto  = 2 /** Uses `sysStateSteps[2]` as the command. */
} e_ScreenFadeCmd;

/** @brief Screen fade types. */
typedef enum _ScreenFadeType
{
    ScreenFadeType_Black           = 0,
    ScreenFadeType_White           = 1,
    ScreenFadeType_ScreenBorders   = 2, // TODO: Investigate. Seems to enable borders around screen?
    ScreenFadeType_CutsceneBorders = 3, // TODO: Investigate. Same as `ScreenBorders` but also sets cutscene flag?

    ScreenFadeType_Hack            = -1 /** @hack Forces the enum as `s32`. */
} e_ScreenFadeType;

/** @brief Increments the event state step to use on next tick.
 *
 * @param incSubStep If `true`, increments `sysStateSteps[1]`, otherwise increments `sysStateSteps[0]`.
 */
void Event_SysStateStepIncrement(bool incSubStep);

/** @brief Sets the event state step to a specific value for the next tick.
 *
 * @param setSubStep If `true`, sets `sysStateSteps[1]`, otherwise sets `sysStateSteps[0]`.
 * @param sysStateStep The value to set the state step to.
 */
void Event_SysStateStepSet(bool setSubStep, s32 sysStateStep);

/** @brief Waits for either the player to stop moving or a 1 second timeout, then increments `sysStateSteps[0]`.
 *
 * @note Used after `Player_ControlFreeze()` to ensure the player has come to a halt before continuing the event sequence.
 */
void Event_WaitPlayerStop(void);

/** @brief Waits for a specified delay before incrementing the event state step.
 *
 * @note Uses `g_SysWork.sysStateStepData[1]` to store/increment timer.
 *
 * @param delay Duration to wait before advancing.
 * @param incSubStep If `true`, increments `sysStateSteps[1]`, otherwise increments `sysStateSteps[0]`.
 */
void Event_WaitTimer(q19_12 delay, bool incSubStep);

/** @brief Updates character states during events and cutscenes.
 *
 * @param cmd Character animation command.
 * @param chara Character to update.
 * @param animState Animation state.
 * @param setSubStep If `true`, sets `sysStateSteps[1]`, otherwise sets `sysStateSteps[0]`.
 */
void Event_CharaAnimCmdExecute(e_CharaAnimCmd cmd, s_SubCharacter* chara, s32 animState, bool incSubStep);

/** @brief Sets `sysStateSteps` depending on whether `eventFlagIdx` flag is set.
 *
 * @param eventFlagIdx Index of the event flag to check.
 * @param stepTrue Step to use if flag is set.
 * @param stepFalse Step to use if flag is not set.
 * @param setSubStep If `true`, sets `sysStateSteps[1]`, otherwise sets `sysStateSteps[0]`.
 */
void Event_SysStateBranchOnFlag(e_EventFlag eventFlagIdx, s32 stepTrue, s32 stepFalse, bool setSubStep);

/** @brief Displays a selection menu and sets `sysStateSteps` depending on the chosen value.
 *
 * @param hasSelection `true` if waiting for a selection, `false` if `sysStateSteps` increments after displaying.
 * @param mapMsgIdx Index of the map message to display.
 * @param step0 Step to use if selection #0 is chosen.
 * @param step1 Step to use if selection #1 is chosen.
 * @param step2 Step to use if selection #2 is chosen.
 * @param incSubStep If `true`, sets `sysStateSteps[1]`, otherwise sets `sysStateSteps[0]`.
 */
void Event_DisplayMapMsg(bool hasSelection, s32 mapMsgIdx, s32 step0, s32 step1, s32 step2, bool incSubStep);

/** @brief Executes a screen fade or cutscene border transition command.
 *
 * @note Two phases:
 *   - Start the transition.
 *   - Poll until complete.
 * Fade types 0-1 perform actual screen fades (to/from black or white) using `Screen_FadeStart`.
 * Fade types 2-3 use cutscene letterbox borders instead of a screen fade? Type 3 additionally
 * sets `SysFlag_CutsceneActive`.
 *
 * @param cmd Screen fade command to execute.
 * @param fadeOut If `true`, fades to black/white or shows borders, otherwise fades from black/white or hides borders.
 * @param fadeType Transition type.
 * @param fadeTimestep Fade speed.
 * @param incSubStep If `true`, increments `sysStateSteps[1]`, otherwise increments `sysStateSteps[0]`.
 */
void Event_ScreenFadeCmd(e_ScreenFadeCmd cmd, bool fadeOut, e_ScreenFadeType fadeType, q19_12 fadeTimestep, bool incSubStep);

/** @brief Handles background texture loading and drawing. Optionally allows layering two textures.
 *
 * @param cmd Background texture command.
 * @param texFileIdx Texture file index of the background to load and draw.
 * @param incSubStep Increment sub-step.
 */
void Event_BgTextureCmd(e_BgTextureCmd cmd, e_FsFile texFileIdx, bool incSubStep);

/** @brief Handles an inventory item model load and optionally adds the item to the inventory.
 *
 * @note Three main phases:
 *   - Queue model load.
 *   - Await load completion.
 *   - Finalize, optionally adding the item to inventory.
 * TODO: Commands 0-3 execute a specific phase directly. Commands 4-6 resume from `sysStateSteps[2]`?
 *
 * @param cmd Inventory item command to execute.
 * @param itemId Inventory item to load and add.
 * @param itemCount Number of items to add (only used by `InvItemCmd_AddItem` and `InvItemCmd_6`).
 * @param incSubStep If `true`, increments `sysStateSteps[1]`, otherwise increments `sysStateSteps[0]`.
 */
void Event_InvItemCmd(e_InvItemCmd stateStep, e_InvItemId itemId, s32 itemCount, bool incSubStep);

/** @brief Sets a waypoint position for a character path.
 *
 * @param isAbs If `true`, uses `posX`/`posZ` as absolute coordinates. If `false`, it uses them as offsets from the player's current position.
 * @param charaSlotIdx Character slot index. 0 = player, 1 = NPC. @note Paths for NPCs are still offset from player position.
 * @param waypointIdx Index of the waypoint along the path.
 * @param headingAngle Heading angle for the character on this path.
 * @param posX X position or offset from the player, depending on `isAbsolute`.
 * @param posZ Z position or offset from the player, depending on `isAbsolute`.
 */
void Event_PathWaypointSet(bool isAbs, s32 charaSlotIdx, s32 waypointIdx, q3_12 headingAngle, q19_12 posX, q19_12 posZ);

/** @brief Executes the player's walk path and increments `sysStateSteps` on completion.
 *
 * @param animId Animation to use while walking.
 * @param waypointCount Number of waypoints in the path.
 * @param incSubStep If `true`, increments `sysStateSteps[1]`, otherwise increments `sysStateSteps[0]`.
 */
void Event_PathWaypointExecutePlayer(s32 animId, s32 waypointCount, bool incSubStep);

/** @brief Executes an NPC's walk path and increments `sysStateSteps` on completion.
 *
 * @param chara Character to move.
 * @param animId Animation to use while walking.
 * @param waypointCount Number of waypoints in the path.
 * @param incSubStep If `true`, increments `sysStateSteps[1]`, otherwise increments `sysStateSteps[0]`.
 */
void Event_PathWaypointExecuteChara(s_SubCharacter* chara, s32 animId, s32 waypointCount, bool incSubStep);

/** @brief Executes an NPC's walk path.
 *
 * @note Same as `Event_PathWaypointExecuteChara`, but doesn't affect `sysStateSteps`.
 * Used when event code doesn't need to wait for the waypoint to be reached?
 *
 * @param chara Character to move.
 * @param animId Animation to use while walking.
 * @param waypointCount Number of waypoints in the path.
 */
void Event_PathWaypointExecuteCharaNoWait(s_SubCharacter* chara, s32 animId, s32 waypointCount);

/** @brief Handles paper map loading, drawing, and cleanup.
 *
 * @param cmd Paper map command to execute.
 * @param paperMapIdx Index for `g_PaperMapFileIdxs` and `g_PaperMapMarkingFileIdxs` to select the area map.
 */
void Event_PaperMapCmd(e_PaperMapCmd cmd, s32 paperMapIdx);

/** @brief Resets a tween timer slot to `Q12(0.0f)`.
 *
 * @param timerIdx Timer slot index.
 */
void Event_TweenReset(s32 timerIdx);

/** @brief Computes a linear tween from `Q12(0.0f)` to `target` over `duration`.
 *
 * @param target Target value at the end of the tween.
 * @param duration Tween duration.
 * @param timerIdx Timer slot index.
 * @return Interpolated value based on the elapsed time.
 */
q19_12 Event_TweenLinear(q19_12 target, q19_12 duration, s32 timerIdx);

/** @brief @unused Computes a sine-based tween over a given angle range.
 *
 * @param amp Amplitude multiplier applied to the sine-based result.
 * @param startAngle Starting angle for the sine function.
 * @param sweepAngle Angular span swept over the duration.
 * @param duration Total tween duration in delta time.
 * @param timerIdx Timer slot index.
 * @return `amp * sin(startAngle + ((sweepAngle * elapsed) / duration))`.
 */
q19_12 Event_TweenSine(q19_12 amp, q3_12 startAngle, q3_12 sweepAngle, q19_12 duration, s32 timerIdx);

/** @brief Displays a map message with dialog audio and increments the audio index.
 *
 * @param mapMsgIdx Index of the message to display.
 * @param audioIdx Index of the dialog audio command to play.
 * @param audioCmds Dialog audio commands.
 */
void Event_DisplayMapMsgWithAudio(s32 mapMsgIdx, u8* audioIdx, const u16* audioCmds);

/** @brief Sets the camera position target.
 *
 * @param pos Target position (Q19.12).
 * @param offsetOrPosX If `pos` is valid, X offset for `pos`. If `pos` is `NULL`, X target position.
 * @param offsetOrPosY If `pos` is valid, Y offset for `pos`. If `pos` is `NULL`, Y target position.
 * @param offsetOrPosZ If `pos` is valid, Z offset for `pos`. If `pos` is `NULL`, Z target position.
 * @param accelX X acceleration.
 * @param accelY X acceleration.
 * @param speedXMax Max X speed.
 * @param speedYMax Max Y speed.
 * @param warp If `true`, warp to the position target, otherwise transition over time.
 */
void Event_CameraPositionSet(VECTOR3* pos,
                             q19_12 offsetOrPosX, q19_12 offsetOrPosY, q19_12 offsetOrPosZ,
                             q19_12 accelXz, q19_12 accelY, q19_12 speedXzMax, q19_12 speedYMax,
                             bool warp);

/** @brief Sets the camera rotation and look-at position targets.
 *
 * @param lookAt Target look-at position (Q19.12).
 * @param lookAtOffsetOrPosX If `lookAt` is valid, X offset for `lookAt`. If `lookAt` is `NULL`, X target look-at position.
 * @param lookAtOffsetOrPosY If `lookAt` is valid, X offset for `lookAt`. If `lookAt` is `NULL`, Y target look-at position.
 * @param lookAtOffsetOrPosZ If `lookAt` is valid, Z offset for `lookAt`. If `lookAt` is `NULL`, Z target look-at position.
 * @param angularAccelX Angular acceleration on the X axis.
 * @param angularAccelY Angular acceleration on the Y axis.
 * @param angularSpeedXMax Max angular speed on the X axis.
 * @param angularSpeedYMax Max angular speed on the Y axis.
 * @param warp If `true`, warp to the look-at target, otherwise transition over time.
 */
void Event_CameraLookAtSet(VECTOR3* lookAt,
                           q19_12 lookAtOffsetOrPosX, q19_12 lookAtOffsetOrPosY, q19_12 lookAtOffsetOrPosZ,
                           q19_12 angularAccelX, q19_12 angularAccelY, q19_12 angularSpeedXMax, q19_12 angularSpeedYMax,
                           bool warp);

/** @brief Starts a character animation.
 * Increments `sysStateSteps[0]` when the animation has finished.
 *
 * @param chara Character to update.
 * @param animState Animation state.
 */
void Event_CharaAnimPlayToEnd(s_SubCharacter* chara, s32 animState);

/** @brief Unlocks/resumes an already playing character animation.
 * Increments `sysStateSteps[0]` when the animation has finished.
 *
 * @param chara Character to update.
 */
void Event_CharaAnimUnlockPlayToEnd(s_SubCharacter* chara);

/** @brief Starts a screen fade and begins loading a background texture.
 * Increments `sysStateSteps[0]` when the fade has completed.
 *
 * @param texFileIdx Texture file index to of the background load.
 * @param fadeTimestep Image fade timestep.
 */
void Event_BgTextureLoadFadeIn(e_FsFile texFileIdx, q19_12 fadeTimestep);

/** @brief Fades the screen while loading a background texture.
 * When the texture is loaded, it draws it to screen and increments `sysStateSteps[0]`.
 *
 * @param texFileIdx Texture file index of the background to load.
 * @param fadeTimestep0 Image fade timestep 0. TODO
 * @param fadeTimestep1 Image fade timestep 1. TODO
 */
void Event_BgTextureFadeIn(e_FsFile texFileIdx, q19_12 fadeTimestep0, q19_12 fadeTimestep1);

/** @brief Fades out a background texture and returns to gameplay.
 *
 * Counterpart to `Event_BgTextureFadeIn`. Continues drawing the current background
 * texture while fading to black, then starts a fade from black to reveal the
 * gameplay scene and advances `sysStateSteps[0]`.
 *
 * @param fadeTimestepFromBlack Fade speed for the reveal.
 * @param fadeTimestepToBlack Fade speed for the hide.
 */
void Event_BgTextureFadeOut(s32 fadeTimestepFromBlack, q19_12 fadeTimestepToBlack);

/** @brief Displays a map message with SFX.
 *
 * @param mapMsgIdx Index of the map message to display.
 * @param sfxId ID of the SFX to play.
 * @param sfxPos SFX position.
 */
void Event_DisplayMapMsgWithSfx(s32 mapMsgIdx, e_SfxId sfxId, VECTOR3* sfxPos);

/** @brief @unused Freezes the game and fades in a background texture.
 * The image remains on the screen until a button is pressed by the user.
 *
 * @param texFileIdx Texture file index of the background to display.
 * @param fadeTimestep0 Image fade timestep 0. TODO: Specify.
 * @param fadeTimestep1 Image fade timestep 1. TODO: Specify.
 */
void Event_DisplayBgTexture(e_FsFile texFileIdx, q19_12 fadeTimestep0, q19_12 fadeTimestep1);

/** @brief Displays a message with a background texture.
 *
 * @param texFileIdx Texture file index of the background to display.
 * @param fadeTimestep0 TODO: Specify.
 * @param fadeTimestep1 TODO: Specify.
 * @param mapMsgIdx Index of the map message to display.
 */
void Event_DisplayMapMsgWithBg(e_FsFile texFileIdx, q19_12 fadeTimestep0, q19_12 fadeTimestep1, s32 mapMsgIdx);

/** @brief Displays a message with a background texture that is dimmed after reading the first message.
 *
 * @param texFileIdx Texture file index of the background to display.
 * @param fadeTimestep0 TODO: Specify.
 * @param fadeTimestep1 TODO: Specify.
 * @param mapMsgIdx0 Index of the first map message to display.
 * @param mapMsgIdx1 Index of the second map message to display.
 */
void Event_DisplayMapMsgWithDimmedBg(e_FsFile texFileIdx, q19_12 fadeTimestep0, q19_12 fadeTimestep1, s32 mapMsgIdx0, s32 mapMsgIdx1);

/** @brief Handles the state steps of an item pickup event.
 *
 * @param itemId ID of the inventory item to be picked up.
 * @param itemCount Number of items.
 * @param eventFlagIdx Index of the event flag to set.
 * @param mapMsgIdx Index of the map message to display.
 */
void Event_ItemTake(e_InvItemId itemId, s32 itemCount, e_EventFlag eventFlagIdx, s32 mapMsgIdx);

/** @brief Handles a common item pickup event.
 *
 * @param pickupType Common pickup item ID of the item to be picked up (`e_CommonPickupItemId`).
 * @param eventFlagIdx Index of the event flag to set.
 */
void Event_CommonItemTake(u32 pickupType, e_EventFlag eventFlagIdx);

/** @brief Handles a paper map pickup event.
 *
 * @param paperMapFlagIdx Flag index of the paper map to be picked up.
 * @param eventFlagIdx Index of the event flag to set.
 * @param mapMsgIdx Index of the map message to display.
 */
void Event_PaperMapTake(s32 paperMapFlagIdx, e_EventFlag eventFlagIdx, s32 mapMsgIdx);

#endif
