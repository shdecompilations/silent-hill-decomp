#ifndef _BODYPROG_SAVEGAME_H
#define _BODYPROG_SAVEGAME_H

#include "bodyprog/chara/chara.h"
#include "bodyprog/items.h"
#include "bodyprog/sys/joy.h"

/** @brief Savegame data. */
typedef struct _Savegame
{
    /* 0x0      */ s_InventoryItem items[INV_ITEM_COUNT_MAX];
    /* 0xA0     */ s8              field_A0;
    /* 0xA1     */ s8              unused_A1[3]; /** @unused */
    /* 0xA4     */ s8              mapIdx;       /** `e_MapIdx` | Index to map overlay `.BIN` files. */
    /* 0xA5     */ s8              mapRoomIdx;   /** Index to local map geometry `.IPD` files. */
    /* 0xA6     */ s16             savegameCount;
    /* 0xA8     */ s8              locationId;                  /** `e_SaveLocationId` */
    /* 0xA9     */ u8              paperMapIdx;                 /** `e_PaperMapIdx` | Index of the paper map displayed when opening the map screen. */
    /* 0xAA     */ u8              equippedWeapon;              /** `e_InvItemId` | Affects the visible player weapon model. */
    /* 0xAB     */ u8              invSlotCount;                /** Item slots. */
    /* 0xAC     */ u32             itemToggleFlags;             /** `e_ItemToggleFlags` */
    /* 0xB0     */ s32             mapEnemyStates[45];          /** Flags indicating the enemy states in a given map.
                                                                 * All set to 1 by default. As soon as they are killed (not just stunned),
                                                                 * set to 0 based on a currently unknown index value.
                                                                 */
    /* 0x164    */ s32             paperMapFlags;               // See Sparagas' `HasMapsFlags` struct for details of every bit.
    /* 0x168    */ u32             eventFlags[52];              // Can be accessed through `Savegame_EventFlagGet` / `Savegame_EventFlagSet`, only tested a few, but seems all are related to events and pick-up flags
                                                                // grouped by location and not item types.
                                                                // Also includes map marking flags - see Sparagas' `MapMarkingsFlags` struct for details of every bit.
    /* 0x238    */ q19_12          healthSaturation;            /** Range: [0, 300]. Ampoules give extra stored health. If the player loses health, it will be slowly restored. */
    /* 0x23C    */ s16             pickedUpItemCount;
    /* 0x23E    */ s8              unused_23E;         /** @unused */
    /* 0x23F    */ u8              inventoryItemFlags; /** `e_InvItemFlags` */
    /* 0x240    */ q19_12          playerHealth;       /** Default: `Q12(100.0f)` */
    /* 0x244    */ q19_12          playerPositionX;
    /* 0x248    */ q3_12           playerRotationY;  /** Range [0, 0.999755859375], positive Z: 0, clockwise rotation. It can be multiplied by 360 to get degrees. */
    /* 0x24A    */ u8              clearGameCount;   /** Range [0, 99] */
    /* 0x24B    */ u8              clearGameEndings; /** `e_GameEndingFlags` */
    /* 0x24C    */ q19_12          playerPositionZ;
    /* 0x250    */ q20_12          gameplayTimer;
    /* 0x254    */ q20_12          runDistance;
    /* 0x258    */ q20_12          walkDistance;
    /* 0x25C+0  */ u8              isNextFearMode           : 1; /** `bool` | Makes savegame entry text gold. */
    /* 0x25C+1  */ u8              add290Hours              : 2; /** Adds 290 hours per 1 bit, i.e. 290, 580, 870. */
    /* 0x25C+3  */ u8              pickedUpSpecialItemCount : 5; /** Red/None: 0?, Yellow: 8, Green: 16, @unused Rainbow: 24. */
                                                                 /** Sparagas' investigations indicate this variable should be
                                                                  * two different variables. However, splitting it causes minor
                                                                  * mismatches in some functions.
                                                                  *
                                                                  * The first 3 bits indicate the number of special items the
                                                                  * player has picked up, and the last 2 bits indicate the color of the Hyper
                                                                  * Blaster beam.
                                                                  *
                                                                  * Belek666 suggests that some functions specifically access this field as 5 bits.
                                                                  *
                                                                  * The the odd access results in a bug where the results screen will count more collected
                                                                  * special items than normal by additionally reading one of the two bits
                                                                  * for the Hyper Blaster beam color.
                                                                  */
    /* 0x25D    */ u8              meleeKillCount;
    /* 0x25E    */ u8              meleeKillCountB; // Can't be packed if used as `u16`.
    /* 0x25F    */ u8              rangedKillCount;
    /* 0x260+0  */ u32             field_260      : 28;
    /* 0x260+28 */ s32             gameDifficulty : 4;  /** `e_GameDifficulty` */
    /* 0x264    */ u16             firedShotCount;      /** Missed shot count = firedShotCount - (closeRangeShotCount + midRangeShotCount + longRangeShotCount). */
    /* 0x266    */ u16             closeRangeShotCount; /** Only hits counted. */
    /* 0x268    */ u16             midRangeShotCount;   /** Only hits counted. */
    /* 0x26A    */ u16             longRangeShotCount;  /** Only hits counted. */
    /* 0x26C    */ u16             field_26C;
    /* 0x26E    */ u16             field_26E; // Related to enemy kills.
    /* 0x270    */ u16             field_270;
    /* 0x272    */ u16             field_272;
    /* 0x274    */ u16             field_274;
    /* 0x276    */ u16             field_276;
    /* 0x278    */ u16             field_278;
    /* 0x27A    */ s8              currentEndingFlags; /** `e_GameEndingFlags` | Ending reached in the current session.
                                                        * Similar to `clearGameEndings`, but the value is replaced with the current ending instead of ORed.
                                                        * Used to identify the latest ending for the ranking screen.
                                                        */
    /* 0x27B    */ u8              continueCount;
} s_Savegame;
STATIC_ASSERT_SIZEOF(s_Savegame, 636);

/** @brief User options configuration. */
typedef struct _OptionsConfig
{
    /* 0x0  */ s_ControllerConfig controllerConfig;
    /* 0x1C */ s8                 screenPositionX;     /** Range: [-11, 11], default: 0. */
    /* 0x1D */ s8                 screenPositionY;     /** Range: [-8, 8], default: 0. */
    /* 0x1E */ u8                 soundType;           /** `bool` | Stereo: `false`, Monaural: `true`, default: Stereo. */
    /* 0x1F */ u8                 volumeBgm;           /** Range: [0, 128] with steps of 8, default: 16. */
    /* 0x20 */ u8                 volumeSe;            /** Range: [0, 128] with steps of 8, default: 16. */
    /* 0x21 */ u8                 vibrationEnabled;    /** `bool` | Off: 0, On: 128, default: On. */
    /* 0x22 */ u8                 brightness;          /** Range: [0, 7], default: 3. */
    /* 0x23 */ u8                 extraWeaponCtrl;     /** `bool` | Switch: `false`, Press: `true`, default: Press. */
    /* 0x24 */ u8                 extraBloodColor;     /** `e_BloodColor` | Default: Normal. */
    /* 0x25 */ s8                 autoLoad;            /** `bool` | Off: `false`, On: `true`, default: Off. */
    /* 0x26 */ s8                 unused_26;           /** @unused */
    /* 0x27 */ u8                 extraOptionsEnabled; /** Holds unlocked option flags. */
    /* 0x28 */ s8                 extraViewCtrl;       /** `bool` | Normal: `false`, Reverse: `true`, default: Normal. */
    /* 0x29 */ s8                 extraViewMode;       /** `bool` | Normal: `false`, Self View: `true`, default: Normal. */
    /* 0x2A */ s8                 extraRetreatTurn;    /** `bool` | Normal: `false`, Reverse: `true`, default: Normal. */
    /* 0x2B */ s8                 extraWalkRunCtrl;    /** `bool` | Normal: `false`, Reverse: `true`, default: Normal. */
    /* 0x2C */ s8                 extraAutoAiming;     /** `bool` | On: `false`, Off: `true`, default: On. */
    /* 0x2D */ s8                 extraBulletAdjust;   /** x1-x6: Range [0, 5], default: x1. */
    /* 0x2E */ u16                seenGameOverTips[1]; /** Bitfield tracking seen game-over tips. Each bit corresponds to a tip index (0–14), set bits indicate seen tips. Resets after picking all 15. */
    /* 0x30 */ s8                 unk_30[4];
    /* 0x34 */ u32                palLanguageId;
} s_OptionsConfig;
STATIC_ASSERT_SIZEOF(s_OptionsConfig, 56);

extern s_Savegame* const g_SavegamePtr;

/** @brief Gets an event flag state from the savegame event flags array.
 *
 * @param flagIdx Event flag index.
 * @return Event flag state (`bool`).
 */
#define Savegame_EventFlagGet(flagIdx) \
    (g_SavegamePtr->eventFlags[(flagIdx) >> 5] & (1 << ((flagIdx) & 0x1F)))

/** @brief Gets an event flag state from the savegame event flags array.
 *
 * @note This alternate version shifts the flags array value by the flag index for some reason
 * and is required for some matches.
 *
 * @param flagIdx Event flag index.
 * @return Event flag state (`bool`).
 */
#define Savegame_EventFlagGetAlt(flagIdx) \
    ((g_SavegamePtr->eventFlags[(flagIdx) >> 5] >> ((flagIdx) & 0x1F)) & (1 << 0))

/** @brief Clears an event flag state in the savegame event flags array.
 *
 * @param flagIdx Event flag index.
 */
#define Savegame_EventFlagClear(flagIdx) \
    (g_SavegamePtr->eventFlags[(flagIdx) >> 5] &= ~(1 << ((flagIdx) & 0x1F)))

/** @brief Sets an event flag state in the savegame event flags array.
 *
 * @param flagIdx Event flag index.
 */
#define Savegame_EventFlagSet(flagIdx) \
    (g_SavegamePtr->eventFlags[(flagIdx) >> 5] |= 1 << ((flagIdx) & 0x1F))

/** @brief Sets an event flag state in the savegame event flags array.
 *
 * @note Some map event code only seems to work with this inline version.
 *
 * @param flagIdx Event flag index.
 */
static inline void Savegame_EventFlagSetAlt(u32 flagIdx)
{
    s16 localIdx;
    s16 localBit;

    localIdx = flagIdx / 32;
    localBit = flagIdx % 32;

    g_SavegamePtr->eventFlags[localIdx] |= 1 << localBit;
}

#endif
