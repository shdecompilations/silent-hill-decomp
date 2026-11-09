#ifndef _GAME_H
#define _GAME_H

#include "bodyprog/events/event_flags.h"
#include "gpu.h"
#include "types.h"
#include "libkpad.h"

// TODO: Includes will have to be cleaned up once everything is organised project-wide and this header becomes
// deprecated.
#include "bodyprog/chara/anim.h"
#include "bodyprog/chara/chara.h"
#include "bodyprog/chara/model.h"
#include "bodyprog/events/bgm_update.h"
#include "bodyprog/formats/anm.h"
#include "bodyprog/items.h"
#include "bodyprog/savegame.h"
#include "bodyprog/screen/data.h"
#include "bodyprog/screen/vsync.h"
#include "bodyprog/sys/joy.h"
#include "maps/common/characters/harry.h"

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
 * For use with `s_PlayerExtra::disabledAnimBoneFlags`.
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

/* Macros for comparing map chunk indexes against a map X/Z position.
 *
 * SH1 game world is split to 40x40 meter chunks, terrain loading code and filenames of IPDs (eg. `THR01FF.IPD` for X 1, Z -1)
 * use a rounded-down grid where chunk 0 is anything between 0 to 40, chunk -1 is anything between -40 to 0.
 *
 * The code used in map event/object spawning code number them differently, since these divide positions with
 * C's truncating division which would give 0 for anything between -40 to 40 (making chunk "0" count as 80 units).
 * To workaround that the code adds 1 if position is positive, or subtracts 1 for negative (or 0).
 * This workaround prevents the actual compiled code from seeing any position as chunk 0 (while other parts of game code do
 * make use of chunk 0), and makes code for any checks for chunk 0 or above +1 of the actual index being checked.
 *
 * To make our code that uses these macros consistent with rest of game code, we make our macro adjust the
 * index being checked for, so the source code using the macros can pass in rounded-down chunk indexes, and that'll
 * handle adding 1 to any index that's positive/zero.
 * This lets it match the original compiled ASM for positive chunk checks (that showed it checking +1 of the actual chunk index),
 * but lets our C code make use of the same indexes used in the rest of the game.
 */

/** @brief Map event chunk index of world coordinate `pos`, never returns chunk 0, see above. */
#define MAP_CHUNK_IDX(pos) \
    (((pos) / Q12(40.0f)) + (((pos) > Q12(0.0f)) ? 1 : -1))

/** @brief Converts rounded-down terrain chunk grid index `n` to its equivalent `MAP_CHUNK_IDX` value. */
#define MAP_CHUNK_IDX_FROM_GRID(n) \
    (((n) >= 0) ? ((n) + 1) : (n))

#define PLAYER_IN_MAP_CHUNK_X(x) \
    (MAP_CHUNK_IDX(g_SysWork.playerWork.player.position.vx) == MAP_CHUNK_IDX_FROM_GRID(x))

#define PLAYER_IN_MAP_CHUNK_Z(z) \
    (MAP_CHUNK_IDX(g_SysWork.playerWork.player.position.vz) == MAP_CHUNK_IDX_FROM_GRID(z))

/** @brief Checks if the player is in terrain chunk (`x`, `z`). */
#define PLAYER_IN_MAP_CHUNK(x, z) \
    (PLAYER_IN_MAP_CHUNK_X(x) && PLAYER_IN_MAP_CHUNK_Z(z))

#define PLAYER_NEAR_POS(comp, base, tol)                                                                                                             \
    (((g_SysWork.playerWork.player.position.comp - Q12(base)) >= Q12(0.0f)) ? ((g_SysWork.playerWork.player.position.comp - Q12(base)) < Q12(tol)) : \
                                                                              ((Q12(base) - g_SysWork.playerWork.player.position.comp) < Q12(tol)))

#define HAS_FLAG(ptr, idx) \
    ((((u32*)ptr)[(idx) >> 5] >> ((idx) & 0x1F)) & (1 << 0))

#define SET_FLAG(ptr, idx) \
    ((((u32*)ptr)[(idx) >> 5] |= (1 << 0) << ((idx) & 0x1F)))

#define CLEAR_FLAG(ptr, idx) \
    ((((u32*)ptr)[(idx) >> 5] &= ~((1 << 0) << ((idx) & 0x1F))))

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
    SysFlag_SetCameraRail  = 1 << 6,
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

    // TODO: The following are only used in `currentEndingFlags`.
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
#if !VERSION_REGION_IS(PAL)
    /* 0x5BC */ s8                 unused_5BC[28];    // @unused Debug data?
#else
    /* 0x5BC */ s8                 unused_5BC[36];
    /* 0x5E0 */ s8                 loadedPalItemLang; // Language index of the loaded item language overlay, +1 of the actual language ID so the default 0 is treated as not loaded.
    /* 0x5E1 */ s8                 __pad_5E1[3];
#endif
} s_GameWork;
#if !VERSION_REGION_IS(PAL)
    STATIC_ASSERT_SIZEOF(s_GameWork, 1496);
#else
    STATIC_ASSERT_SIZEOF(s_GameWork, 1508);
#endif

/** @brief Extra player character info. */
typedef struct _PlayerExtra
{
    /* 0x0  */ s_Model     model;                 /** Manages upper half body's animations (torso, arms, head). */
    /* 0x18 */ s32         disabledAnimBoneFlags; /** Created using `BITMASK_RANGE`. */
    /* 0x1C */ s32         state;                 /** `e_PlayerState` */
    /* 0x20 */ s32         upperBodyState;        /** `e_PlayerUpperBodyState` */
    /* 0x24 */ s32         lowerBodyState;        /** `e_PlayerLowerBodyState` */
    /* 0x28 */ e_InvItemId lastUsedItem;          /** Holds the last item ID used from inventory when the player is inside an item trigger area. */
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

// TODO: Move world env structs to `map_effects.h.
//============================================================================

/** @brief World environment preset. Contains fog, lighting and tint settings. */
typedef struct _WorldEnvPreset
{
    /* 0x0  */ u8      flags;                  /** `e_WorldEnvFlags` */
    /* 0x1  */ u8      lightLevelMax;          /** Upper limit on the computed light level of lit vertices. `0xFF` for no limit. */
    /* 0x2  */ u8      lightMode;              /** `e_WorldEnvLightMode` */
    /* 0x3  */ s8      __pad_3;
    /* 0x4  */ q3_12   spotLightIntensity;
    /* 0x6  */ q3_12   worldLightIntensity;
    /* 0x8  */ q3_12   worldTintR;
    /* 0xA  */ q3_12   worldTintG;
    /* 0xC  */ q3_12   worldTintB;
    /* 0xE  */ u8      fogMode;                /** `e_WorldEnvFogMode` */
    /* 0x10 */ q19_12  fogDistance;
    /* 0x14 */ CVECTOR fogColor;
    /* 0x18 */ u8      enableTintLightOverlap; /** `bool` */
    /* 0x19 */ CVECTOR pointLightTint;         /** Volumetric point light color. */
    /* 0x1D */ CVECTOR worldTint;              /** Subtractive. */
    /* 0x21 */ CVECTOR snowColor;              /** Snow particle color. `Particle_SnowDraw` uses only the red value in maps with header `field_16 == 1`. } Both multiply a dim additive texture, colors in the default env presets likely make very little difference? */
    /* 0x25 */ CVECTOR rainColor;              /** Rain particle color. `Particle_RainDraw` uses only the red value in maps with header `field_16 == 1`. } */
               // 3 bytes of padding.
} s_WorldEnvPreset;
STATIC_ASSERT_SIZEOF(s_WorldEnvPreset, 44);

/** @brief World environment state. */
typedef struct _WorldEnvState
{
    /* 0x0  */ s_WorldEnvPreset params;
    /* 0x2C */ q3_12            flashlightLensFlareIntensity;
    /* 0x2E */ q3_12            brightnessIntensity;
    /* 0x30 */ q19_12           fogDistance;
} s_WorldEnvState;
STATIC_ASSERT_SIZEOF(s_WorldEnvState, 52);

/** @brief Game-side world environment, lighting and fog presets in use, and flashlight state.
 *
 * Each frame `WorldEnv_Update` works out `current` in two steps:
 * 1. `transition` fades from the previous presets (`transitionFrom`) toward the new ones (`transitionTo`).
 * 2. `current` is a mix of the flashlight off and on entries of `transition`, weighted by `flashlightIntensity`.
 *
 * `WorldEnv_Apply` then copies `current` into `g_WorldEnvRenderWork` for drawing.
 *
 * The `transition***` arrays are indexed by the flashlight state: 0 off, 1 on.
 */
typedef struct _WorldEnvWork
{
    /* 0x0   */ s32          progressType;            /** `e_PrimitiveType` of `progressValue`. `PrimitiveType_None`: no transition. */
    /* 0x4   */ s8*          progressValue;           /** Value that drives the transition. Usually overlay data. */
    /* 0x8   */ q19_12       progressStart;           /** `progressValue` at which the transition starts. */
    /* 0xC   */ q19_12       progressEnd;             /** `progressValue` at which the transition completes. */
    /* 0x10  */ q19_12       visibilityDistance;      /** @unused Result of `func_8003FEC0`, never read. */
    /* 0x14  */ u8           flashEffect;             /** `bool` | Trigger world flash effect (used for weapons). */
    /* 0x15  */ u8           isFlashlightOn;          /** `bool` */
    /* 0x16  */ u8           isFlashlightUnavailable; /** `bool` | Set when both presets are the same. Forces the flashlight off. */
                // 1 byte of padding.
    /* 0x18  */ q3_12        flashlightIntensity;
                // 2 bytes of padding.
    /* 0x1C  */ s_WorldEnvState transition[2];        /** Current blend of `transitionFrom` and `transitionTo`. */
    /* 0x84  */ s_WorldEnvState transitionTo[2];      /** Most recently selected presets. */
    /* 0xEC  */ s_WorldEnvState transitionFrom[2];    /** Copy of `transition` taken when a transition starts. */
    /* 0x154 */ s_WorldEnvState current;              /** Final blended state. Read by gameplay code for the environment flags. */
} s_WorldEnvWork;
STATIC_ASSERT_SIZEOF(s_WorldEnvWork, 392);

//============================================================================

/** @brief Main system workspace. Stores key engine data. */
typedef struct _SysWork
{
    /* 0x0      */ s8               unused_0[8];          /** @unused */
    /* 0x8      */ e_SysState       sysState;
    /* 0xC      */ s32              sysStateSteps[3];     /** Temp data used by current `sysState`. Can be another state ID or other data. */
    /* 0x10     */ bool             isMsgStringSet;       /** Indicates if string have been loaded and is going (or it is) being display. */
    /* 0x1C     */ s32              gameStateCounter;     /** Frame counter for the current `g_GameWork.gameState`. Reset when `gameState` is changed. */
    /* 0x1C     */ s32              gameStateStepCounter; /** Frame counter for the current `g_GameWork.gameStateSteps[0]`. Reset when `gameStateSteps[0]` is changed. */
    /* 0x24     */ s32              sysStateCounter;      /** Frame counter for the current `sysState`. Reset when `sysState` is changed. */
    /* 0x28     */ s32              sysStateStepData[2];  /** Temporary data for `sysStateSteps[0]` and `sysStateSteps[1]`.
                                                           * Data at an index is cleared when that step index is changed. Holds a timer or other data.
                                                           */
    /* 0x30     */ s32              cutsceneBorderState;  /** `e_CutsceneBorderState` */
    /* 0x34     */ s8               __pad_34[4];
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
    /* 0x2294   */ s8               __pad_2294[4];
    /* 0x2298   */ e_ProcessFlags   processFlags;
    /* 0x229C   */ s32              unused_229C; /** @unused Set to `NO_VALUE` when the player has been initalized and 0 when the player changes areas. Beyond that it's dead code. */
    /* 0x22A0   */ e_BgmStatusFlags bgmStatusFlags;
    /* 0x22A4   */ e_SysFlags       sysFlags;         // `SysFlag_SetCameraRail` passed as "use through door cam" flag in `vcSetFirstCamWork`.
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
    /* 0x2359   */ s8               __pad_2359[1];
    /* 0x235A   */ u8               playerStopFlags;       /** `e_PlayerStopFlags` */
                   // 1 byte of padding.
    /* 0x235C   */ GsCOORDINATE2*   lightBoneCoord;
    /* 0x2360   */ VECTOR3          lightPosition;      // } Often set to DMS cutscene data.
    /* 0x236C   */ GsCOORDINATE2*   lensFlareBoneCoord; // }
    /* 0x2370   */ SVECTOR          lightRotation;      // }
    /* 0x2378   */ q3_12            lightIntensity;
    /* 0x237A   */ q3_12            cameraAngleY;
    /* 0x237C   */ q3_12            cameraAngleZ;
    /* 0x237E   */ s8               __pad_237E[2];
    /* 0x2380   */ q19_12           cameraRadiusXz;
    /* 0x2384   */ q19_12           cameraY;
    /* 0x2388   */ s_WorldEnvWork   worldEnv;
    /* 0x2510   */ s_KPad_Handler*  kpadHandlers; /** Head of the kpad handler list from `KPad_HandlerList_Init`. */
    /* 0x2514   */ s_KPadWork       kpad;
    /* 0x2534   */ s_KPad_Channel   kpadChannels[2];
    /* 0x2544   */ s_KPad_Effect    kpadEffects[16];
    /* 0x2744   */ s8               unk_2744[4];
    /* 0x2748   */ q3_12            bgmLayerVolumes[BGM_LAYER_COUNT]; // Last index value is not a layer, but some sort of timer. See `Bgm_LayersUpdate`.
                   // 2 bytes of padding.
    /* 0x275C   */ q23_8            field_275C; // } SFX volumes?
    /* 0x2760   */ q23_8            field_2760; // }
    /* 0x2764   */ q23_8            field_2764; // }
} s_SysWork;
STATIC_ASSERT_SIZEOF(s_SysWork, 10088);

extern void* g_OvlDynamic;
extern void* g_OvlBodyprog;
#if VERSION_REGION_IS(PAL)
extern void* g_OvlItemLang;
#endif

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
        step                         =
        g_GameWork.gameStateSteps[2] = stateStep;
    }

    return step;
}

/** @brief Increments one of the three game state step counters.
 *
 * The steps form a hierarchy used by the `gameState` machines:
 *   [0] = state step, [1] = sub-step, [2] = sub-sub-step.
 *
 * @note Incrementing a step cascades a reset downward: changing a higher 
 * level invalidates the steps nested within it, so all levels lower
 * than `stepIdx` are reset to 0.
 * Incrementing [0] additionally clears the `gameStateStepCounter` frame counter.
 *
 * @param stepIdx Step index to increment: 0, 1, or 2.
 */
static inline void Game_StateStepIncrement(s32 stepIdx)
{    
    if (stepIdx == 0)
    {
        s32 step = g_GameWork.gameStateSteps[0];

        g_SysWork.gameStateStepCounter = 0;
        g_GameWork.gameStateSteps[1]   = 0;
        g_GameWork.gameStateSteps[2]   = 0;
        g_GameWork.gameStateSteps[0]   = step + 1;
    }
    else if (stepIdx == 1)
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

#endif
