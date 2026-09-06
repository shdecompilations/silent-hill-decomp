#ifndef _GAME_H
#define _GAME_H

#include "event_flags.h"
#include "gpu.h"
#include "types.h"
#include "libkpad.h"

#include "bodyprog/anim.h"
#include "bodyprog/chara/chara.h"
#include "bodyprog/events/bgm_update.h"
#include "bodyprog/formats/anm.h"
#include "bodyprog/items.h"
#include "bodyprog/model.h"
#include "bodyprog/savegame.h"
#include "bodyprog/sys/joy.h"
#include "maps/characters/harry.h"

#define TICKS_PER_SECOND  60 /** Game has a variable timestep with 60 ticks max. */
#define TIMESTEP_30_FPS   Q12(1.0f / (float)(TICKS_PER_SECOND / 2))
#define TIMESTEP_60_FPS   Q12(1.0f / (float)(TICKS_PER_SECOND))

#define SCREEN_WIDTH                   320
#define SCREEN_HEIGHT                  240
#define FRAMEBUFFER_HEIGHT_PROGRESSIVE 224
#define FRAMEBUFFER_HEIGHT_INTERLACED  (FRAMEBUFFER_HEIGHT_PROGRESSIVE * 2)
#define ORDERING_TABLE_SIZE            2048

/** @brief Converts seconds to frames at 60 FPS.
 *
 * @param sec Seconds to convert.
 * @return Frames at 60 FPS.
 */
#define SECONDS_60_FPS(sec) \
    (s32)((sec) * TICKS_PER_SECOND)

/** @brief Packs a weapon attack containing a weapon ID and attack input type.
 *
 * @param weaponId Weapon ID.
 * @param attackInputType Attack input type.
 * @return Packed weapon attack containing a weapon ID and attack input type.
 */
#define WEAPON_ATTACK(weaponId, attackInputType) \
    ((weaponId) + ((attackInputType) * 10))

/** @brief Retrieves the weapon ID from a packed weapon attack.
 *
 * @param weaponAttack Packed weapon attack containing a weapon ID and attack input type.
 * @return Weapon ID.
 */
#define WEAPON_ATTACK_ID_GET(weaponAttack) \
    ((weaponAttack) % 10)

/** @brief Creates a bitmask with a contiguous range of bits set.
 * For use with `s_PlayerExtra::disabledAnimBones`.
 *
 * Generates an `unsigned int` mask with all bits in the range `[fromInclusive, toInclusive]` set.
 *
 * For example:
 * - `BITMASK_RANGE(0, 2)` -> 0b000...0111 (decimal 7).
 * - `BITMASK_RANGE(4, 11)` -> 0b000...111111110000 (decimal 4080).
 *
 * @param fromInclusive Index of the lowest bit to set (0 = least significant bit).
 * @param toInclusive Index of the highest bit to set.
 * @return Bitmask with the specified range of bits set to 1 (`unsigned int`).
 */
#define BITMASK_RANGE(fromInclusive, toInclusive) \
    (((~0u << (fromInclusive)) & ~(~0u << ((toInclusive) + 1))))

#define MAP_CHUNK_CHECK_VARIABLE_DECL() \
    s32 __chunkIdx

/** @hack This macro requires a variable `s32 __chunkIdx` to be declared before using it.
 * The macro `MAP_CHUNK_CHECK_VARIABLE_DECL` declares that variable and must be called before this macro.
 * The first argument is `vx` or `vz`, which is used as the component name in `VECTOR3`.
 * @bug Some maps appear to have a bug where the negative position check will never be true because they check
 * if the chunk index will be a positive number. Seems like they forgot to use `ABS`?
 */
#define PLAYER_IN_MAP_CHUNK(comp, x0, x1, x2, x3)                                                \
    (__chunkIdx = g_SysWork.playerWork.player.position.comp / Q12(40.0f),                        \
     ((g_SysWork.playerWork.player.position.comp >  Q12(0.0f) && (__chunkIdx + (x0)) == (x1)) || \
      (g_SysWork.playerWork.player.position.comp <= Q12(0.0f) && (__chunkIdx + (x2)) == (x3))))

#define PLAYER_NOT_IN_MAP_CHUNK(comp, x0, x1, x2, x3)                                            \
    (__chunkIdx = g_SysWork.playerWork.player.position.comp / Q12(40.0f),                        \
     ((g_SysWork.playerWork.player.position.comp >  Q12(0.0f) && (__chunkIdx + (x0)) != (x1)) || \
      (g_SysWork.playerWork.player.position.comp <= Q12(0.0f) && (__chunkIdx + (x2)) != (x3))))

#define MAP_CHUNK_CHECK_VARIABLE_DECL_2() \
    s32 __chunkIdx2

#define PLAYER_IN_MAP_CHUNK_2(comp, x0, x1, x2, x3)                                              \
    (__chunkIdx2 = g_SysWork.playerWork.player.position.comp / Q12(40.0f),                       \
     ((g_SysWork.playerWork.player.position.comp >  Q12(0.0f) && (__chunkIdx2 + (x0)) < (x1)) || \
      (g_SysWork.playerWork.player.position.comp <= Q12(0.0f) && (__chunkIdx2 + (x2)) < (x3))))

#define PLAYER_NEAR_POS(comp, base, tol)                                                                                                             \
    (((g_SysWork.playerWork.player.position.comp - Q12(base)) >= Q12(0.0f)) ? ((g_SysWork.playerWork.player.position.comp - Q12(base)) < Q12(tol)) : \
                                                                              ((Q12(base) - g_SysWork.playerWork.player.position.comp) < Q12(tol)))

#define MIN_OFFSET(x, neg, pos) \
    ((((x) + (-neg)) <= ((x) + (pos))) ? ((x) - (neg)) : ((x) + (pos)))

#define MAX_OFFSET(x, neg, pos) \
    ((((x) - (neg)) > ((x) + (pos))) ? ((x) - (neg)) : ((x) + (pos)))

/** @brief Packs XZ cell coordinates in a single value.
 *
 * @param x X cell coordinate.
 * @param z Z cell coordinate.
 * @return Packed XZ cell coordinates.
 */
#define CELL_XZ(x, z) \
    ((x) + ((z) << 8))

#define HAS_FLAG(ptr, idx) \
    ((((u32*)ptr)[(idx) >> 5] >> ((idx) & 0x1F)) & (1 << 0))

#define SET_FLAG(ptr, idx) \
    ((((u32*)ptr)[(idx) >> 5] |= (1 << 0) << ((idx) & 0x1F)))

#define CLEAR_FLAG(ptr, idx) \
    ((((u32*)ptr)[(idx) >> 5] &= ~((1 << 0) << ((idx) & 0x1F))))

/** @brief Sync modes used by `DrawSync` and `VSync`. */
typedef enum _SyncMode
{
    SyncMode_Count     = -1,
    SyncMode_Wait      = 0,
    SyncMode_Immediate = 1,
    SyncMode_Wait2     = 2,
    SyncMode_Wait3     = 3,
    SyncMode_Wait8     = 8
} e_SyncMode;

/** @brief Paper map indices. Used for the navigation map screen. */
typedef enum _PaperMapIdx
{
    PaperMapIdx_OtherPlaces    = 0,
    PaperMapIdx_OldTown        = 1,
    PaperMapIdx_FogCentralTown = 2,
    PaperMapIdx_AltCentralTown = 3,
    PaperMapIdx_ResortTown     = 4,
    PaperMapIdx_FogSchoolBF    = 5,
    PaperMapIdx_FogSchool1F    = 6,
    PaperMapIdx_FogSchool2F    = 7,
    PaperMapIdx_FogSchoolRF    = 8,
    PaperMapIdx_AltSchoolBF    = 9,
    PaperMapIdx_AltSchool1F    = 10,
    PaperMapIdx_AltSchool2F    = 11,
    PaperMapIdx_AltSchoolRF    = 12,
    PaperMapIdx_FogSewer1F     = 13,
    PaperMapIdx_FogSewer2F     = 14,
    PaperMapIdx_AltSewer       = 15,
    PaperMapIdx_FogHospitalBF  = 16,
    PaperMapIdx_FogHospital1F  = 17,
    PaperMapIdx_FogHospital2F  = 18,
    PaperMapIdx_FogHospital3F  = 19,
    PaperMapIdx_AltHospitalBF  = 20,
    PaperMapIdx_AltHospital1F  = 21,
    PaperMapIdx_AltHospital2F  = 22,
    PaperMapIdx_AltHospital3F  = 23,
    PaperMapIdx_24             = 24
} e_PaperMapIdx;

/** @brief System flags. TODO: Give this a more specific name. */
typedef enum _SysFlags
{
    SysFlag_None           = 0,
    SysFlag_0              = 1 << 0, /** @unused */
    SysFlag_DemoActive     = 1 << 1,
    SysFlag_2              = 1 << 2, /** @unused */
    SysFlag_CutsceneActive = 1 << 3,
    SysFlag_NoEnemySpawn   = 1 << 4,
    SysFlag_5              = 1 << 5, // Set during cafe cutscene. Something to prevent cutscene softlocks when an enemy is around??
    SysFlag_OnCameraRail   = 1 << 6,
    SysFlag_MenuActive     = 1 << 7,
    SysFlag_DoWarmReset    = 1 << 8,
    SysFlag_9              = 1 << 9, // @unused? Set during cafe cutscene along with `SysFlag_5` and never checked.
    SysFlag_LoadActive     = 1 << 10
} e_SysFlags;

/** @brief System process flags. */
typedef enum _ProcessFlags
{
    ProcessFlag_None              = 0,
    ProcessFlag_RoomTransition    = 1 << 0,
    ProcessFlag_OverlayTransition = 1 << 1,
    ProcessFlag_NewGame           = 1 << 2,
    ProcessFlag_LoadSave          = 1 << 3,
    ProcessFlag_Continue          = 1 << 4,
    ProcessFlag_BootDemo          = 1 << 5
} e_ProcessFlags;

/** @brief State IDs used by the main game loop. The values are used as indices into the `g_GameStateUpdateFuncs` function array. */
typedef enum _GameState
{
    GameState_Init                = 0,
    GameState_KonamiLogo          = 1,
    GameState_KcetLogo            = 2,
    GameState_MovieIntroFadeIn    = 3,
    GameState_AutoLoadSavegame    = 4,
    GameState_MovieIntroAlternate = 5,
    GameState_MovieIntro          = 6,
    GameState_MainMenu            = 7,
    GameState_LoadSavegameScreen  = 8,
    GameState_MovieOpening        = 9,
    GameState_MainLoadScreen      = 10,
    GameState_InGame              = 11,
    GameState_MapEvent            = 12,
    GameState_ExitMovie           = 13,
    GameState_InventoryScreen     = 14,
    GameState_PaperMapScreen      = 15,
    GameState_SaveScreen          = 16,
    GameState_DebugMoviePlayer    = 17,
    GameState_OptionScreen        = 18,
    GameState_LoadStatusScreen    = 19,
    GameState_LoadMapScreen       = 20,
    GameState_Credits             = 21,
    GameState_Unk16               = 22, /** Removed debug menu? Doesn't exist in function array, but `DebugMoviePlayer` state tries to switch to it. */

    GameState_Hack                = NO_VALUE // @hack Force enum to be treated as `s32`.
} e_GameState;

/** @brief State IDs used by `GameState_InGame`.
 *
 * The values are used as indices into the 0x800A9A2C function array.
 */
typedef enum _SysState
{
    SysState_Gameplay       = 0,
    SysState_OptionsMenu    = 1,
    SysState_StatusMenu     = 2,
    SysState_MapScreen      = 3,
    SysState_Fmv            = 4,
    SysState_LoadOverlay    = 5,
    SysState_LoadRoom       = 6, // Alternative: `SysState_LoadArea`.
    SysState_ReadMessage    = 7,
    SysState_SaveMenu0      = 8,
    SysState_SaveMenu1      = 9,
    SysState_EventCallback  = 10,
    SysState_EventSetFlag   = 11,
    SysState_EventPlaySound = 12,
    SysState_GameOver       = 13,
    SysState_GamePaused     = 14,
    SysState_Invalid        = 15 /** Used by `Event_Update` to signal that SysState shouldn't be updated. */
} e_SysState;

/** @brief Attack input types. Packed into a weapon attack using `WEAPON_ATTACK`. */
typedef enum _AttackInputType
{
    AttackInputType_Tap      = 0,
    AttackInputType_Hold     = 1,
    AttackInputType_Multitap = 2
} e_AttackInputType;

/** @brief Game difficulties. */
typedef enum _GameDifficulty
{
    GameDifficulty_Easy   = -1,
    GameDifficulty_Normal = 0,
    GameDifficulty_Hard   = 1
} e_GameDifficulty;

/** @brief Game ending types. */
typedef enum _GameEnding
{
    GameEnding_0        = 0, // @unused? Credits has parameters set up for it, but nothing sets this.
    GameEnding_GoodPlus = 1,
    GameEnding_Good     = 2,
    GameEnding_BadPlus  = 3,
    GameEnding_Bad      = 4,
    GameEnding_Ufo      = 5
} e_GameEnding;

/** @brief Game ending flags. */
typedef enum _GameEndingFlags
{
    GameEndingFlag_GoodPlus = 1 << 0,
    GameEndingFlag_Good     = 1 << 1,
    GameEndingFlag_BadPlus  = 1 << 2,
    GameEndingFlag_Bad      = 1 << 3,
    GameEndingFlag_Ufo      = 1 << 4,

    // The following are only used in `currentEndingFlags`.
    GameEndingFlag_5        = 1 << 5, // Unknown purpose, `Ranking_PrepareSavegame` sets `g_SavegamePtr->currentEndingFlags |= 1 << 5`, nothing checks it?
    GameEndingFlag_6        = 1 << 6, // Set the first time ranking has been seen?
    GameEndingFlag_7        = 1 << 7  // Set if ranking has been seen and ending was UFO?
} e_GameEndingFlags;

/** @brief Game workspace. Stores miscellaneous gameplay-related data. */
typedef struct _GameWork
{
    /* 0x0   */ s_OptionsConfig    config;
    /* 0x38  */ s_ControllerData   controllers[CONTROLLER_COUNT_MAX];
    /* 0x90  */ s_Savegame         autosave;
    /* 0x30C */ s_Savegame         savegame;
    /* 0x588 */ u16                gsScreenWidth;
    /* 0x58A */ u16                gsScreenHeight;
    /* 0x58C */ s_PrimColor        background2dColor;
    /* 0x590 */ e_GameState        gameStatePrev;
    /* 0x594 */ e_GameState        gameState;
    /* 0x598 */ s32                gameStateSteps[3]; /** Sub-state steps used by the current `gameState`. Can be other state IDs or data. */
    /* 0x5A4 */ s8                 unk_5A4[4];        // @unused?
    /* 0x5A8 */ s32                field_5A8;
    /* 0x5AC */ s32                field_5AC;
    /* 0x5B0 */ s8                 __pad_5B0;
    /* 0x5B1 */ s8                 mapAnimIdx;
    /* 0x5B2 */ s8                 bgmIdx;            /** `BgmCmd` | Currently player background music track. */
    /* 0x5B4 */ s8                 ambientIdx;        /** Index of `g_AmbientVabTaskLoad`. */
    /* 0x5B4 */ s_AnalogController rawController;
    /* 0x5BC */ s8                 unused_5BC[28];    // @unused Debug data?
} s_GameWork;
STATIC_ASSERT_SIZEOF(s_GameWork, 1496);

/** @brief Extra player character info. */
typedef struct _PlayerExtra
{
    /* 0x0  */ s_Model     model;             /** Manages upper half body's animations (torso, arms, head). */
    /* 0x18 */ s32         disabledAnimBones; /** Bitfield of disabled animation bones. Can be created using the `BITMASK_RANGE` macro. */
    /* 0x1C */ s32         state;             /** `e_PlayerState` */
    /* 0x20 */ s32         upperBodyState;    /** `e_PlayerUpperBodyState` */
    /* 0x24 */ s32         lowerBodyState;    /** `e_PlayerLowerBodyState` */
    /* 0x28 */ e_InvItemId lastUsedItem;      /** Holds the last item ID used from inventory when the player is inside an item trigger area. */
} s_PlayerExtra;
STATIC_ASSERT_SIZEOF(s_PlayerExtra, 44);

/** @brief Player workspace.
 *
 * Possible original name: `shPlayerWork`.
 */
typedef struct _PlayerWork
{
    /* 0x0   */ s_SubCharacter player;
    /* 0x128 */ s_PlayerExtra  extra;
} s_PlayerWork;
STATIC_ASSERT_SIZEOF(s_PlayerWork, 340);

/** @brief Player combat info. */
typedef struct _PlayerCombat
{
    /* 0x0  */ VECTOR3 attackPosition; /** Q19.12 */
    /* 0xC  */ s8      __pad[3];
    /* 0xF  */ s8      weaponAttack; /** Packed weapon attack. See `WEAPON_ATTACK`. */
    /* 0x10 */ u8      currentWeaponAmmo;
    /* 0x11 */ u8      totalWeaponAmmo;
    /* 0x12 */ s8      weaponInventoryIdx; /** Index of the currently equipped weapon in the inventory. */
    /* 0x13 */ u8      isAiming;           /** `bool` */
} s_PlayerCombat;
STATIC_ASSERT_SIZEOF(s_PlayerCombat, 20);

/** @brief Map effects info. */
typedef struct _MapEffectsInfo
{
    /* 0x0  */ union
               {
                   s32 field_0; // Flags?
                   u8  field_00[4];
               } field_0;
    /* 0x4  */ q3_12   field_4; // Alpha.
    /* 0x6  */ q3_12   field_6; // World tint color intensity.
    /* 0x8  */ q3_12   worldTintR;
    /* 0xA  */ q3_12   worldTintG;
    /* 0xC  */ q3_12   worldTintB;
    /* 0xE  */ u8      field_E; // Fog enabled if not set to 0, `Gfx_FogParametersSet` checks for values 0/1/2/3.
                                // Sets the transparent grey layer overlaid on characters and the enviroment.
    /* 0x10 */ q19_12  fogDistance;
    /* 0x14 */ CVECTOR fogColor;
    /* 0x18 */ u8      enableTintLightOverlap; /** `bool` */
    /* 0x19 */ CVECTOR pointLightTint;         /** Volumetric point light color. */
    /* 0x1D */ CVECTOR worldTint;              /** Subtractive. */
    /* 0x21 */ CVECTOR field_21;               // Particle effect related. Only the first value affects snow transparency.
    /* 0x25 */ CVECTOR field_25;
               // 3 bytes of padding.
} s_MapEffectsInfo;
STATIC_ASSERT_SIZEOF(s_MapEffectsInfo, 44);

typedef struct
{
    /* 0x0  */ s_MapEffectsInfo effectsInfo;
    /* 0x2C */ q3_12            flashlightLensFlareIntensity;
    /* 0x2E */ q3_12            brightnessIntensity;
    /* 0x30 */ q19_12           fogDistance;
} s_StructUnk3;
STATIC_ASSERT_SIZEOF(s_StructUnk3, 52);

// Current enviroment effects information.
typedef struct
{
    /* 0x0   */ s32          primitiveType; /** `e_PrimitiveType` */
    /* 0x4   */ s8*          field_4;       /** Points to different types of data depending on `field_0`. */
    /* 0x8   */ q19_12       field_8;
    /* 0xC   */ q19_12       field_C;
    /* 0x10  */ s32          field_10;
    /* 0x14  */ u8           field_14;                /** `bool` */
    /* 0x15  */ u8           isFlashlightOn;          /** `bool` */
    /* 0x16  */ u8           isFlashlightUnavailable; /** `bool` */
                // 1 byte of padding.
    /* 0x18  */ q3_12        flashlightIntensity;
                // 2 bytes of padding.
    /* 0x1C  */ s_StructUnk3 field_1C[2];
    /* 0x84  */ s_StructUnk3 field_84[2];
    /* 0xEC  */ s_StructUnk3 field_EC[2];
    /* 0x154 */ s_StructUnk3 field_154;
} s_SysWork_2388;
STATIC_ASSERT_SIZEOF(s_SysWork_2388, 392);

/** @brief Main system workspace. Stores key engine data. */
typedef struct _SysWork
{
    /* 0x0      */ s8               unused_0[8];          /** @unused */
    /* 0x8      */ e_SysState       sysState;
    /* 0xC      */ s32              sysStateSteps[3];     /** Temp data used by current `sysState`. Can be another state ID or other data. */
    /* 0x10     */ bool             isMgsStringSet;       /** Indicates if string have been loaded and is going (or it is) being display. */
    /* 0x1C     */ s32              gameStateCounter;     /** Frame counter for the current `g_GameWork.gameState`. Reset when `gameState` is changed. */
    /* 0x1C     */ s32              gameStateStepCounter; /** Frame counter for the current `g_GameWork.gameStateSteps[0]`. Reset when `gameStateSteps[0]` is changed. */
    /* 0x24     */ s32              sysStateCounter;      /** Frame counter for the current `sysState`. Reset when `sysState` is changed. */
    /* 0x28     */ s32              sysStateStepData[2];  /** Temporary data for `sysStateSteps[0]` and `sysStateSteps[1]`.
                                                           * Data at an index is cleared when that step index is changed. Holds a timer or other data.
                                                           */
    /* 0x30     */ s32              cutsceneBorderState;  /** `e_CutsceneBorderState` */
    /* 0x34     */ s8               unused_34[4];         /** @unused */
    /* 0x38     */ s_PlayerCombat   playerCombat;
    /* 0x4C     */ s_PlayerWork     playerWork;
    /* 0x1A0    */ s_SubCharacter   npcs[NPC_COUNT_MAX];
    /* 0x890    */ GsCOORDINATE2    playerBoneCoords[HarryBone_Count];
    /* 0xE30    */ GsCOORDINATE2    unkCoords_E30[5];                       // Might be part of previous array for 5 extra coords which go unused.
    /* 0xFC0    */ GsCOORDINATE2    npcBoneCoordBuffer[NPC_BONE_COUNT_MAX]; /** Contiguous NPC bone coord buffer. */
    /* 0x2280   */ s8               npcFlagId;                              // 1-based NPC ID for `npcFlags`.
    /* 0x2281   */ s8               loadingScreenIdx;
    /* 0x2282   */ s8               areaTransitionFlags;                /** `e_AreaTransitionFlags` */
    /* 0x2283   */ s8               sfxPairIdx;                         /** `e_SfxPairIdx` | Index into `SFX_PAIRS`. */
    /* 0x2284   */ u16              charaGroupFlags[CHARA_GROUP_COUNT]; /** `e_CharaGroupFlags` */
                                                                        // Enabling a flag for Larval Stalkers causes them to die.
    /* 0x228C   */ s32              field_228C[1];  // Spawn flags for enemy characters?
    /* 0x2290   */ s32              npcFlags;       // Flags related to NPCs. Each bit corresponds to an `npcs` array entry.
    /* 0x2294   */ s8               unused_2294[4]; /** @unused */
    /* 0x2298   */ e_ProcessFlags   processFlags;
    /* 0x229C   */ s32              unused_229C; /** @unused Set to `NO_VALUE` when the player has been initalized and 0 when the player changes areas. Beyond that it's dead code. */
    /* 0x22A0   */ e_BgmStatusFlags bgmStatusFlags;
    /* 0x22A4   */ e_SysFlags       sysFlags;         // `SysFlag_OnCameraRail` passed as "use through door cam" flag in `vcSetFirstCamWork`.
    /* 0x22A8   */ GsCOORDINATE2    coord_22A8;       // For particles only?
    /* 0x22F8   */ GsCOORDINATE2    coord_22F8;       // Likely related to above.
    /* 0x2348+0 */ s8               field_2348   : 8; // Related to particles.
    /* 0x2349+0 */ s8               field_2349   : 8; // Particle spawn multiplier?
    /* 0x234A+0 */ u8               field_234A   : 8; /** `bool` */
    /* 0x234B+0 */ u8               field_234B_0 : 4; /** `bool` | Related to particles. Used to trigger SFX? */
    /* 0x234B+4 */ u8               field_234B_4 : 4; // Related to particles.
    /* 0x234C   */ q19_12           mapMsgTimer;
    /* 0x2350+0 */ u8               enableHalfHeightGlyphs : 4; /** `bool` */
    /* 0x2350+4 */ u8               silentYesSelection     : 4; /** `bool` */
    /* 0x2351+0 */ u32              invItemSelectedIdx     : 8;
    /* 0x2352+0 */ u32              invItemLoadFlags       : 8; /** `e_InvItemLoadFlags` */
    /* 0x2353   */ s8               targetNpcIdx;               /** Index of the NPC in `npcs` being targeted by the player. */
    /* 0x2354   */ s8               npcIdxs[CHARA_GROUP_COUNT];
    /* 0x2358   */ u8               enablePlayerMatchAnim; /** `bool` | Activates the animation performed by Harry when lighting a match at the beginning of the game. */
    /* 0x2359   */ s8               unused_2359;           /** @unused */
    /* 0x235A   */ u8               playerStopFlags;       /** `e_PlayerStopFlags` */
                   // 1 byte of padding.
    /* 0x235C   */ GsCOORDINATE2*   lightBoneCoord;
    /* 0x2360   */ VECTOR3          lightPosition;      // } Often set to DMS cutscene data.
    /* 0x236C   */ GsCOORDINATE2*   lensFlareBoneCoord; // }
    /* 0x2370   */ SVECTOR          lightRotation;      // }
    /* 0x2378   */ q3_12            lightIntensity;
    /* 0x237A   */ q3_12            cameraAngleY;
    /* 0x237C   */ q3_12            cameraAngleZ;
    /* 0x237E   */ s16              field_237E;
    /* 0x2380   */ q19_12           cameraRadiusXz;
    /* 0x2384   */ q19_12           cameraY;
    /* 0x2388   */ s_SysWork_2388   field_2388;
    /* 0x2510   */ s32              field_2510;
    /* 0x2514   */ s_SysWork_2514   field_2514;
    /* 0x254C   */ s8               field_254C[508];                  /** Used through indirect pointer calls. Tied to `libkpad`.*/
    /* 0x2748   */ q3_12            bgmLayerVolumes[BGM_LAYER_COUNT]; // Last index value is not a layer, but some sort of timer. See `Bgm_LayersUpdate`.
                   // 2 bytes of padding.
    /* 0x275C   */ q23_8            field_275C; // } SFX volumes?
    /* 0x2760   */ q23_8            field_2760; // }
    /* 0x2764   */ q23_8            field_2764; // }
} s_SysWork;
STATIC_ASSERT_SIZEOF(s_SysWork, 10088);

extern void* g_OvlBodyprog;
extern void* g_OvlDynamic;

extern s_SysWork         g_SysWork;
extern s_GameWork        g_GameWork;
extern s_GameWork* const g_GameWorkConst;
extern s_GameWork* const g_GameWorkPtr;

/** @brief Sets `sysState` in `g_SysWork` for the next tick.
 *
 * @param sysState System state to set.
 * @return New system state.
 */
static inline s32 SysWork_StateSetNext(e_SysState sysState)
{
    s32 state;

    state                         =
    g_SysWork.sysState            = sysState;
    g_SysWork.sysStateCounter     = 0;
    g_SysWork.sysStateSteps[0]    = 0;
    g_SysWork.sysStateStepData[0] = 0;
    g_SysWork.sysStateSteps[1]    = 0;
    g_SysWork.sysStateStepData[1] = 0;
    g_SysWork.sysStateSteps[2]    = 0;
    return state;
}

/** @brief Increments `sysStateStep` in `g_SysWork` for the next tick.
 *
 * @param stepIdx Index of the `sysStateStep` to increment.
 */
static inline void SysWork_StateStepIncrement(s32 stepIdx)
{
    if (stepIdx == 0)
    {
        g_SysWork.sysStateStepData[0] = 0;
        g_SysWork.sysStateSteps[1]    = 0;
        g_SysWork.sysStateStepData[1] = 0;
        g_SysWork.sysStateSteps[2]    = 0;
        g_SysWork.sysStateSteps[0]++;
    }
    else if (stepIdx == 1)
    {
        g_SysWork.sysStateStepData[1] = 0;
        g_SysWork.sysStateSteps[2]    = 0;
        g_SysWork.sysStateSteps[1]++;
    }
    else
    {
        g_SysWork.sysStateSteps[2]++;
    }
}

/** @brief Sets `sysStateStep[stepIdx]` in `g_SysWork` for the next tick.
 *
 * @param stepIdx The `sysStateStep` index to change.
 * @param sysStateStep System state step to set.
 * @return New system state step.
 */
static inline s32 SysWork_StateStepSet(s32 stepIdx, s32 sysStateStep)
{
    s32 step;

    if (stepIdx == 0)
    {
        step                          =
        g_SysWork.sysStateSteps[0]    = sysStateStep;
        g_SysWork.sysStateStepData[0] = 0;
        g_SysWork.sysStateSteps[1]    = 0;
        g_SysWork.sysStateStepData[1] = 0;
        g_SysWork.sysStateSteps[2]    = 0;
    }
    else if (stepIdx == 1)
    {
        step                          =
        g_SysWork.sysStateSteps[1]    = sysStateStep;
        g_SysWork.sysStateStepData[1] = 0;
        g_SysWork.sysStateSteps[2]    = 0;
    }
    else
    {
        step                       =
        g_SysWork.sysStateSteps[2] = sysStateStep;
    }

    return step;
}

/** @brief Resets `sysStateStep` in `g_SysWork` for the next tick. */
static inline void SysWork_StateStepReset()
{
    g_SysWork.sysStateSteps[0]    = NO_VALUE;
    g_SysWork.sysStateStepData[0] = 0;
    g_SysWork.sysStateSteps[1]    = 0;
    g_SysWork.sysStateStepData[1] = 0;
    g_SysWork.sysStateSteps[2]    = 0;
}

/** @brief Sets an NPC flag in the `g_SysWork.npcFlags` bitfield.
 *
 * @param flagIdx Index of the NPC flag to set.
 */
static inline void SysWork_NpcFlagSet(s32 flagIdx)
{
    g_SysWork.npcFlags |= 1 << flagIdx;
}

/** @brief Clears an NPC flag in the `g_SysWork.npcFlags` bitfield.
 *
 * @param flagIdx Index of the NPC flag to clear.
 */
static inline void SysWork_NpcFlagClear(s32 flagIdx)
{
    CLEAR_FLAG(&g_SysWork.npcFlags, flagIdx);
}

/** @brief Sets one of the three game state step counters.
 *
 * The steps form a hierarchy used by the game's game state state machines:
 * - [0] = state step
 * - [1] = sub-step,
 * - [2] = sub-sub-step.
 *
 * @note Writing a step cascades a reset downward: changing a higher
 * level invalidates the steps nested beneath it, so all levels lower
 * than `stepIdx` are reset to 0.
 * Setting [0] additionally clears the `gameStateStepCounter` frame counter.
 *
 * @param stepIdx Step index to set (0, 1, or 2).
 * @param stateStep New value for the index.
 * @return Value written (`== stateStep`).
 */
static inline s32 Game_StateStepSet(s32 stepIdx, s32 stateStep)
{
    s32 step;

    if (stepIdx == 0)
    {
        step                           = 
        g_GameWork.gameStateSteps[0]   = stateStep;
        g_SysWork.gameStateStepCounter = 0;
        g_GameWork.gameStateSteps[1]   = 0;
        g_GameWork.gameStateSteps[2]   = 0;
    }
    else if (stepIdx == 1)
    {
        step                         = 
        g_GameWork.gameStateSteps[1] = stateStep;
        g_GameWork.gameStateSteps[2] = 0;
    }
    else
    {
        step = g_GameWork.gameStateSteps[2] = stateStep;
    }

    return step;
}

/** @brief Increments one of the three game-state step counters.
 *
 * The steps form a hierarchy used by the games gameState state 
 * machines:
 *   [0] = state step, [1] = sub-step, [2] = sub-sub-step.
 *
 * @note Incrementing a step cascades a reset downward: changing a higher 
 * level invalidates the steps nested beneath it, so all levels lower
 * than `stepIdx` are reset to 0.
 * Incrementing [0] additionally clears the `gameStateStepCounter` frame counter.
 *
 * @param stepIdx    The step index to increment: 0, 1, or 2.
 */
static inline void Game_StateStepIncrement(s32 stepIdx)
{    
    if(stepIdx == 0)
    {
        s32 step = g_GameWork.gameStateSteps[0];

        g_SysWork.gameStateStepCounter = 0;
        g_GameWork.gameStateSteps[1]   = 0;
        g_GameWork.gameStateSteps[2]   = 0;
        g_GameWork.gameStateSteps[0]   = step + 1;
    }
    else if(stepIdx == 1)
    {
        g_GameWork.gameStateSteps[1]++;
        g_GameWork.gameStateSteps[2] = 0;
    }
    else
    {
        g_GameWork.gameStateSteps[2]++;
    }
}

/** @brief Sets the game state to be used for the next tick all resets all state steps.
 *
 * Records the outgoing state as `gameStatePrev`, sets `gameState` as the new 
 * state, and clears all state steps for the new state to have a clean slate.
 *
 * `gameStateCounter` and `gameStateStepCounter` are also cleared and the sys state is changed to `SysState_Gameplay`.
 *
 * @note Changed from inline to macro to fix some stubborn functions.
 *
 * @param gameState New game state to enter.
 */
#define Game_StateSetNext(newGameState)          \
{                                                \
    e_GameState prevState;                       \
    prevState = g_GameWork.gameState;            \
                                                 \
    g_GameWork.gameStateSteps[0] = prevState;    \
    g_GameWork.gameState         = newGameState; \
    g_SysWork.gameStateCounter   = 0;            \
    g_GameWork.gameStatePrev     = prevState;    \
                                                 \
    Game_StateStepSet(0, 0);                     \
    SysWork_StateSetNext(SysState_Gameplay);     \
}

 /** @brief Returns the GameState to the previously used state and resets all state steps.
 *
 * Records the outgoing state as `gameStatePrev`, sets `gameState` to the previous 
 * `gameStatePrev` value, and clears all state-steps for the new state to have a clean slate.
 *
 * @note `gameStateCounter` and `gameStateStepCounter` are also cleared, and SysState is changed to `SysState_Gameplay`.
 */
#define Game_StateSetPrevious()                              \
{                                                            \
    e_GameState prevState;                                   \
    prevState = g_GameWork.gameState;                        \
                                                             \
    g_GameWork.gameStateSteps[0] = prevState;                \
    g_GameWork.gameState         = g_GameWork.gameStatePrev; \
    g_SysWork.gameStateCounter   = 0;                        \
    g_GameWork.gameStatePrev     = prevState;                \
                                                             \
    Game_StateStepSet(0, 0);                                 \
    SysWork_StateSetNext(SysState_Gameplay);                 \
}

static inline void Game_StateSetPreviousA()
{
    e_GameState curState;
    curState = g_GameWork.gameState;
    
    g_GameWork.gameStateSteps[0] = curState;
    g_GameWork.gameState         = g_GameWork.gameStatePrev;
    g_SysWork.gameStateCounter   = 0;
    g_GameWork.gameStatePrev     = curState;
    
    Game_StateStepSet(0, 0);
    SysWork_StateSetNext(SysState_Gameplay);
}

#endif
