#ifndef _BODYPROG_MAP_MAP_H
#define _BODYPROG_MAP_MAP_H

#include "bodyprog/chara/anim.h"
#include "bodyprog/chara/chara.h"
#include "bodyprog/collision/collision.h"
#include "bodyprog/formats/anm.h"
#include "bodyprog/view/structs.h"

#define CAMERA_PATH_COUNT_MAX       100
#define COLLISION_TRIGGER_COUNT_MAX 200

/** @brief Map flags. */
typedef enum _MapFlags
{
    MapFlag_FourActiveChunks = 0,      /** Used by exterior maps. */
    MapFlag_OneActiveChunk   = 1 << 0,
    MapFlag_TwoActiveChunks  = 1 << 1,
    MapFlag_Interior         = 1 << 2,
    MapFlag_3                = 1 << 3  /** @unused Unused map type `XXX` has this flag. */
} e_MapFlags;

/** @brief Playable map indices. Used for binary overlays. */
typedef enum _MapIdx
{
    MapIdx_MAP0_S00 = 0,
    MapIdx_MAP0_S01 = 1,
    MapIdx_MAP0_S02 = 2,
    MapIdx_MAP1_S00 = 3,
    MapIdx_MAP1_S01 = 4,
    MapIdx_MAP1_S02 = 5,
    MapIdx_MAP1_S03 = 6,
    MapIdx_MAP1_S04 = 7,
    MapIdx_MAP1_S05 = 8,
    MapIdx_MAP1_S06 = 9,
    MapIdx_MAP2_S00 = 10,
    MapIdx_MAP2_S01 = 11,
    MapIdx_MAP2_S02 = 12,
    MapIdx_MAP2_S03 = 13,
    MapIdx_MAP2_S04 = 14,
    MapIdx_MAP3_S00 = 15,
    MapIdx_MAP3_S01 = 16,
    MapIdx_MAP3_S02 = 17,
    MapIdx_MAP3_S03 = 18,
    MapIdx_MAP3_S04 = 19,
    MapIdx_MAP3_S05 = 20,
    MapIdx_MAP3_S06 = 21,
    MapIdx_MAP4_S00 = 22,
    MapIdx_MAP4_S01 = 23,
    MapIdx_MAP4_S02 = 24,
    MapIdx_MAP4_S03 = 25,
    MapIdx_MAP4_S04 = 26,
    MapIdx_MAP4_S05 = 27,
    MapIdx_MAP4_S06 = 28,
    MapIdx_MAP5_S00 = 29,
    MapIdx_MAP5_S01 = 30,
    MapIdx_MAP5_S02 = 31,
    MapIdx_MAP5_S03 = 32,
    MapIdx_MAP6_S00 = 33,
    MapIdx_MAP6_S01 = 34,
    MapIdx_MAP6_S02 = 35,
    MapIdx_MAP6_S03 = 36,
    MapIdx_MAP6_S04 = 37,
    MapIdx_MAP6_S05 = 38,
    MapIdx_MAP7_S00 = 39,
    MapIdx_MAP7_S01 = 40,
    MapIdx_MAP7_S02 = 41,
    MapIdx_MAP7_S03 = 42,
    MapIdx_MAPT_S00 = 43, // } @unused Empty test maps. Only some code references remain and `HB_MTS00.ANM`/`HB_MTX00.ANM` anim files.
    MapIdx_MAPX_S00 = 44  // }
} e_MapIdx;

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

/** @brief Used as index into `MAP_INFOS` array. 
 *
 * @note
 * R: "Real"
 * U: "Underworld"
 */
typedef enum _MapType
{
    MapType_THR = 0,  /** Old Silent Hill. */
    MapType_SC  = 1,  /** Real School. */
    MapType_SU  = 2,  /** Alt. School. */
    MapType_SPR = 3,  /** Real Central Town. */
    MapType_SPU = 4,  /** Alt. Central Town */
    MapType_RSR = 5,  /** Real Resort. */
    MapType_RSU = 6,  /** Alt. Resort. */
    MapType_APR = 7,  /** @unused Real Amusement Park. */
    MapType_APU = 8,  /** Alt. Amusement Park. */
    MapType_ER  = 9,  /** Nowhere. */
    MapType_ER2 = 10, /** Bonus Old Town areas. */
    MapType_DR  = 11, /** Real Sewers. */
    MapType_DRU = 12, /** Alt. Sewers. */
    MapType_HP  = 13, /** Real Hospital. */
    MapType_HU  = 14, /** Alt. Hospital. */
    MapType_XXX = 15, /** @unused Test map? */

    MapType_Count
} e_MapType;

/** @brief Speed zone types for player movement speed modulation. */
typedef enum _SpeedZoneType
{
    SpeedZoneType_Slow   = 0,
    SpeedZoneType_Normal = 1,
    SpeedZoneType_Fast   = 2
} e_SpeedZoneType;

/** @brief Event activation types. */
typedef enum _EventActivationType
{
    EventActivationType_None      = 0, /** No activation conditions other than event flag/trigger checks. */
    EventActivationType_Exclusive = 1, /** Prevents other events from being triggered while the event is active. */
    EventActivationType_Button    = 2, /** Requires a button press. */
    EventActivationType_Item      = 3  /** Requires an inventory item. */
} e_EventActivationType;

/** @brief Event trigger types. */
typedef enum _EventTriggerType
{
    EventTriggerType_EndOfArray     = NO_VALUE,
    EventTriggerType_Tick           = 0, /** Skips trigger/activation type checks. Always activates if required event flags are set. Skips processing later events until flags deactivate it. */
    EventTriggerType_TouchAabb      = 1, /** Player has collided with an AABB. */
    EventTriggerType_TouchFacing    = 2, /** Player collided with a trigger is facing toward it. */
    EventTriggerType_TouchObbFacing = 3, /** Player collided with an OBB and is facing toward it. */
    EventTriggerType_TouchObb       = 4  /** Player collided with an OBB. No facing requirement. */
} e_EventTriggerType;

/** @brief Area transition flags. Used by some events to indicate specific cutscene behavior. */
typedef enum _AreaTransitionFlags
{
    AreaTransitionFlag_None               = 0,
    AreaTransitionFlag_UnfreezeWorld      = 1 << 0, /** TODO: Dual purpose: `SysState_ReadMessage` unfreezes the world if set, while `GameBoot_WorldStartup` calls `Map_WorldClear`? */
    AreaTransitionFlag_SkipFadeIn         = 1 << 1, /** Skips fade-in when returning to `GameState_InGame`. */
    AreaTransitionFlag_SkipAmbientSfxInit = 1 << 2  /** TODO: Skips the `Sd_AmbientSfxInit` call in `GameBoot_WorldStartup`? */
} e_AreaTransitionFlags;

/** @brief World map model data locations. */
typedef enum _WorldModelLocation
{
    WorldModelLocation_None   = 0,
    WorldModelLocation_Global = 1,
    WorldModelLocation_Chunk1 = 2,
    WorldModelLocation_Chunk2 = 3,
    WorldModelLocation_Chunk3 = 4,
    WorldModelLocation_Chunk4 = 5,
    WorldModelLocation_Unk6   = 6
} e_WorldModelLocation;

// Something related to enemies attacks.
typedef struct
{
    union
    {
        q19_12 vx_0; // Displacement offset.
        struct
        {
            s16 field_0;
            s16 field_2;
        } s_0;
        struct
        {
            u16 field_0;
            u16 field_2;
        } s_1;
    } field_0;
    union
    {
        q19_12 vz_4;
        struct
        {
            s16 field_0;
            s16 field_2;
        } s_0;
    } field_4;
    q3_12 vy_8;
    u8    field_A;
    u8    field_B; // Flags?
    union
    {
        s32 field_0;
        struct
        {
            q3_12 field_0; // Angle.
            q3_12 field_2; // Angle.
        } s_0;
        struct
        {
            u8 field_0;
            u8 field_1;
            u8 field_2;
            u8 field_3;
        } s_1;
        struct
        {
            q4_12 field_0;
            u16   field_2;
        } s_2;
    } field_C;
    union
    {
        q20_12 field_0; // Timer.
        struct
        {
            s16 field_0;
            s16 field_2;
        } s_0;
        struct
        {
            s16 field_0;
            u8  field_2;
            u8  field_3;
        } s_1;
        struct
        {
            u8 field_0;
            u8 field_1;
            u8 field_2;
            u8 field_3;
        } s_2;
        struct
        {
            u16 field_0;
            u16 field_2;
        } s_3;
    } field_10;
} s_MapHdr_field_4C;
STATIC_ASSERT_SIZEOF(s_MapHdr_field_4C, 20);

/** @brief Speed zone. Defines a volume used for player speed modulation. */
typedef struct _SpeedZone
{
    /* 0x0 */ s8   type; /** `e_SpeedZoneType` */
              // 1 byte of padding.
    /* 0x2 */ q11_4 minX;
    /* 0x4 */ q11_4 maxX;
    /* 0x6 */ q11_4 minZ;
    /* 0x/ */ q11_4 maxZ;
} s_SpeedZone;

/** @brief Water zone. Defines a volume used for visual water effects. */
typedef struct _WaterZone
{
    /* 0x0 */ u8    isEnabled; /** `bool` */
              // 1 byte of padding.
    /* 0x2 */ q11_4 illumination;
    /* 0x4 */ q11_4 minX;
    /* 0x6 */ q11_4 maxX;
    /* 0x8 */ q11_4 minZ;
    /* 0xA */ q11_4 maxZ;
} s_WaterZone;

/** @brief Map GFX info. */
typedef struct _MapInfo
{
    /* 0x0 */ s16                plmFileIdx;
    /* 0x2 */ char               tag[4];
    /* 0x6 */ u8                 flags; /** `e_MapFlags` */
              // 1 byte of padding.
    /* 0x8 */ const s_WaterZone* waterZones;
    /* 0xC */ const s_SpeedZone* speedZones;
} s_MapInfo;

typedef struct
{
    u8    unk_0;
    u8    field_1;
    u8    field_2;
    u8    unk_3;
    u8    field_4;
    u8    field_5;
    u8    field_6;
    u8    unk_7;
    u8    unk_8[2];
    s16   field_A;
    u8    unk_C[2];
    s16   field_E;
    q3_12 field_10;
    u8    unk_12[4];
    s16   field_16;
    u8    unk_18[8];
    s32   field_20;
    s32   field_24;
} s_MapOverlayHdr_5C;

typedef struct
{
    u8    field_0;
    u8    field_1;
    u8    field_2;
    u8    field_3;
    s16   field_4;
    s16   field_6;
    s16   field_8;
    s16   field_A;
    s16   field_C;
    s16   field_E;
    q3_12 field_10;
    s16   field_12;
    s32   field_14;
    s32   field_18;
    s32   field_1C;
} s_MapOverlayHdr_7C;
STATIC_ASSERT_SIZEOF(s_MapOverlayHdr_7C, 32);

// Related to water particles.
typedef struct
{
    s32    field_0[4];
    q3_12  field_10[4];
    q19_12 field_18[4];
    q4_12  field_28[4];
    u8     field_30[4];
    q3_12  field_34[4]; // Y angles.
    q4_12  field_3C[4];
    q4_12  field_44[4];
    q3_12  field_4C[4];
    u16    field_54[4];
    q4_12  field_5C[4];
    q4_12  field_64[4];
    q3_12  field_6c[4];
    s16    field_74; // Particle count?
    s8     __pad_76[2];
    u8     field_78; // Active index for above arrays.
} s_MapOverlayHdr_94;

/** @brief Contains X/Z coordinates and optional 4 bytes of data.
 * Map headers include an array of these, into which `s_EventData` includes an index.
 */
typedef struct _MapPoint2d
{
    /* 0x0    */ q19_12 positionX;
    /* 0x4+0  */ u32    paperMapIdx     : 5; /** `e_PaperMapIdx` */
    /* 0x4+5  */ u32    field_4_5       : 4;
    /* 0x4+9  */ u32    loadingScreenId : 3; /** `e_LoadingScreenId` */
    /* 0x4+12 */ u32    unused_4_12     : 4; /** @unused Always set to 0. */
    /* 0x4+16 */ q24_8  triggerParam0   : 8; // Usually a `Q8_ANGLE`.
    /* 0x4+24 */ u32    triggerParam1   : 8;
    /* 0x8    */ q19_12 positionZ;
} s_MapPoint2d;
STATIC_ASSERT_SIZEOF(s_MapPoint2d, 12);

/** @brief Event data. TODO: Known as `Trigger` in SilentHillMapExaminer: https://github.com/ItEndsWithTens/SilentHillMapExaminer/blob/master/src/SHME.ExternalTool.Guts/Trigger.cs */
typedef struct _EventData
{
    /* 0x0    */ s16 requiredFlag;        /** `e_EventFlag` | Must be set for an event to trigger (or `EventFlag_None`). */
    /* 0x2    */ s16 completeFlag;        /** `e_EventFlag` | Set on event completion. The event is skipped if this flag is already set. */
    /* 0x4+0  */ s8  triggerType    : 4;  /** `e_EventTriggerType` */
    /* 0x4+4  */ u8  activationType : 4;  /** `e_EventActivationType` */
    /* 0x5    */ u8  mapPointIdx;         /** Index into `g_MapOverlayHdr.mapPoints`. */
    /* 0x6    */ u8  requiredItemId;      /** `e_InvItemId` | ID of the item the player must use from the item screen. */
    /* 0x7    */ u8  __pad_7;
    /* 0x8+0  */ u32 sysState        : 5; /** `e_SysState` | System state used by the event. */
    /* 0x8+5  */ u32 eventParam      : 8; /** Can be an ID of a `MapMsg`, sound effect, index into `mapEventFuncs`, or index into `mapPoints` for `areaLoad` events. */
    /* 0x8+8  */ u32 transitionFlags : 6; /** `e_AreaTransitionFlags` */
    /* 0x8+13 */ u32 sfxPairIdx      : 5; /** `e_SfxPairIdx` | Index into `SFX_PAIRS`. */
    /* 0x8+19 */ u32 setCameraRail   : 1; /** `bool` | Always `true`. */
    /* 0x8+24 */ u32 mapIdx          : 6;
} s_EventData;
STATIC_ASSERT_SIZEOF(s_EventData, 12);

/** @brief Special map-specific Harry anim data. */
typedef struct
{
    s16   status; /** Packed anim status. See `s_ModelAnim::status`. */
    s16   status_2; /** Packed anim status. See `s_ModelAnim::status`. */
    q3_12 time;   /** Fixed-point anim time. */
    s16   keyframeIdx_6;
} s_UnkStruct3_Mo;
STATIC_ASSERT_SIZEOF(s_UnkStruct3_Mo, 8);

/** Guessed based on in-debugger observation during gameplay.
 * Everything is inited to 0xFFFF and some data is written when the player is hit by monsters.
 * Might be more generic particles/decals struct.
 */
typedef struct _BloodSplat
{
    s16 field_0; // Maybe an array of `u16`s? Contains indices.
} s_BloodSplat;

/** @brief Character spawn info. */
typedef struct _SpawnInfo
{
    /* 0x0   */ q19_12 positionX;
    /* 0x4   */ s8     charaId; /** `e_CharaId` */
    /* 0x5   */ q0_8   rotationY;
    /* 0x6   */ s8     flags;                 /** `e_SpawnFlags` | Copied to `stateStep` in `s_Model`, with `controlState = 0`. */
    /* 0x7+0 */ s32    gameDifficultyMin : 4; /** `e_GameDifficulty` | Minimum difficulty required for successful spawn. */
    /* 0x8   */ q19_12 positionZ;
} s_SpawnInfo;
STATIC_ASSERT_SIZEOF(s_SpawnInfo, 12);

/** TODO: `g_MapOverlayHdr` is part of the map overlay BIN files. Maybe should be moved to `maps/shared.h`.
 * If field has a comment that lists only certain maps, it means all others set this field to 0.
 * func(?) means the signature is unknown and a default void() was selected for now.
 */
typedef struct _MapOverlayHdr
{
    /* 0x0   */ s_MapInfo*             mapInfo;
    /* 0x4   */ u8                     (*mapRoomIdxGet)(q19_12 posX, q19_12 posY); // Called by `Game_MapRoomIdxUpdate`.
    /* 0x8   */ s8                     unused_8;
    /* 0xC   */ s32                    (*func_C)(); /** @unused */
                // 3 bytes of padding.
    /* 0x10  */ void                   (*bgmEvent)(bool);
    /* 0x14  */ s8                     bgmCmd;
    /* 0x15  */ u8                     ambientAudioIdx; // Ambient file index from `g_AmbientVabTaskLoadCmds`.
    /* 0x16  */ s8                     field_16;        // Set ambient tint and draw distance.
                                                        // A value of 3 sets the map to night.
                                                        // A value of 2 will sets the map to use a tint used during the hallaway
                                                        // intro.
                                                        // Giving this and next variable a name will require further investigation
                                                        // of `particle.c` code.
    /* 0x17  */ s8                     field_17;        // Set weather to play. Rain, heavy rain, or snow.
    /* 0x18  */ void                   (**loadingScreenFuncs)();
    /* 0x1C  */ s_MapPoint2d*          mapPoints;
    /* 0x20  */ void                   (**mapEventFuncs)(); /** Points to array of event functions. */
    /* 0x24  */ s_EventData*           mapEvents;
    /* 0x28  */ GsCOORDINATE2*         npcBoneCoordBuffer; /** Dynamic NPC bone coord buffer used as backup for `s_SysWork::npcBoneCoordBuffer`.
                                                            * In practice, this is always set to `g_SysWork.npcBoneCoordBuffer`,
                                                            * effectively forming a ring buffer. */
    /* 0x2C  */ u8*                    loadableItems;
    /* 0x30  */ const char**           mapMessages;
    /* 0x34  */ s_AnimInfo*            harryMapAnimInfos; /** Map-specific anim infos for Harry (for anims 38+). */
    /* 0x38  */ s_UnkStruct3_Mo*       field_38; // Array of 40?
    /* 0x3C  */ void                   (*initWorldObjects)(void);
    /* 0x40  */ void                   (*updateWorldObjects)(void);
    /* 0x44  */ void                   (*func_44)();
    /* 0x48  */ void                   (*npcSpawnEvent)();
    /* 0x4C  */ s_MapHdr_field_4C*     unkTable1_4C; // Related to collision?
    /* 0x50  */ s16                    unkTable1Count_50;
                // 2 bytes of padding.
    /* 0x54  */ s_BloodSplat*          bloodSplats;
    /* 0x58  */ s16                    bloodSplatCount;
                // 2 bytes of padding.
    /* 0x5C  */ s_MapOverlayHdr_5C* field_5C;

    /* 0x60  */ void                   (*func_60)(s32 idx, bool arg1);
    /* 0x64  */ s32                    (*func_64)(POLY_FT4** poly, s32);
    /* 0x68  */ s32                    (*func_68)(POLY_FT4** poly, s32);
    /* 0x6C  */ void                   (*func_6C)(); /** @unused */
    /* 0x70  */ s32                    (*func_70)(POLY_FT4** poly, s32);
    /* 0x74  */ void                   (*func_74)(); /** @unused */
    /* 0x78  */ s32                    (*func_78)(POLY_FT4** poly, s32);
    /* 0x7C  */ s_MapOverlayHdr_7C*    field_7C; // Only map1_s01, map6_s04.
    /* 0x80  */ void                   (*func_80)();
    /* 0x84  */ s32                    (*func_84)(POLY_FT4** poly, s32);
    /* 0x88  */ s32*                   func_88; /** @unused */           // Only map0_s01, map7_s01.
    /* 0x8C  */ s32                    (*func_8C)(POLY_FT4** poly, s32); // Only map0_s01, map7_s01.
    /* 0x90  */ s32                    (*func_90)(POLY_FT4** poly, s32); // Only map5_s00.
    /* 0x94  */ s_MapOverlayHdr_94*    field_94;                         // Only map1_s02, map1_s03.
    /* 0x98  */ s32                    (*func_98)(POLY_FT4** poly, s32); // Only map1_s02, map1_s03.
    /* 0x9C  */ void                   (*func_9C)();                     // Only map1_s02, map1_s03.
    /* 0xA0  */ void*                  ptr_A0;                           // M1S03 only, pointer to `s_800E3A40` array.
    /* 0xA4  */ s32                    (*func_A4)(POLY_FT4** poly, s32); // Only map1_s03.
    /* 0xA8  */ void                   (*func_A8)();                     // Only map6_s02.
    /* 0xAC  */ s32                    (*func_AC)(POLY_FT4** poly, s32); // Only map4_s03, map4_s05.
    /* 0xB0  */ s32                    (*func_B0)(POLY_FT4** poly, s32); // Only map4_s03, map4_s05.
    /* 0xB4  */ s32                    (*func_B4)(POLY_FT4** poly, s32); // Only map1_s03, map4_s05, map6_s01, map6_s02, map5_s01.
    /* 0xB8  */ void                   (*func_B8)(s_SubCharacter* chara, s_PlayerExtra* extra, GsCOORDINATE2* coords);
    /* 0xBC  */ void                   (*func_BC)(s_SubCharacter* chara, s_PlayerExtra* extra, GsCOORDINATE2* coords);
    /* 0xC0  */ void                   (*playerCutsceneWeaponUnequip)(); /** @unused */
    /* 0xC4  */ void                   (*playerMatchArmAnimDisable)(); /** @unused */
    /* 0xC8  */ void                   (*playerControlFreeze)();
    /* 0xCC  */ void                   (*playerControlUnfreeze)(bool setIdle);
    /* 0xD0  */ bool                   (*playerPathWaypointExecute)(s32 playerExtraState, VECTOR3* vec, q3_12 angle, s32 vecCount); // 0x800C964C
    /* 0xD4  */ s32                    (*playerAnimStateSet)(s32 playerExtraState); // This only changes the state of player extra data, but gets used by same function that calls `charaAnimStateSet` when player chara is passed.
                                                                                    // @hack Points to `Player_AnimStateSet` which has `void` return type, but changing this funcptr causes mismatch in `Event_CharaAnimCmdExecute`.
    /* 0xD8  */ void                   (*playerAnimReset)();
    /* 0xDC  */ void                   (*playerAnimLock)(void);
    /* 0xE0  */ void                   (*playerAnimIsLocked)(void);
    /* 0xE4  */ void                   (*playerAnimUnlock)(void);
    /* 0xE8  */ s32                    (*playerAnimPlaybackStateGet)(void);
    /* 0xEC  */ bool                   (*playerMoveSpeedIsZero)(void);
    /* 0xF0  */ void                   (*playerMoveSpeedClear)(void); /** @unused */
    /* 0xF4  */ void                   (*playerFallBackward)(void); /** @unused */
    /* 0xF8  */ void                   (*playerStub_F8)(); /** @unused */
    /* 0xFC  */ void                   (*playerDamageFeetFront)(void); /** @unused */
    /* 0x100 */ void                   (*func_100)(); /** @unused */
    /* 0x104 */ void                   (*func_104)(); /** @unused */
    /* 0x108 */ s32                    (*func_108)();
    /* 0x10C */ void                   (*playerEmptyWeaponHandSet)(); /** @unused */
    /* 0x110 */ void                   (*playerWeaponAttackRestore)(); /** @unused */
    /* 0x114 */ s32                    (*playerPropertyField10DGet)(); /** @unused */
    /* 0x118 */ void                   (*func_118)(); /** @unused */
    /* 0x11C */ void                   (*charaMovementReset)(s_SubCharacter* npc); /** @unused */
    /* 0x120 */ void                   (*charaAnimReset)(); /** @unused */
    /* 0x124 */ void                   (*charaAnimStateSet)(s_SubCharacter* chara, q19_12 afkTime);
    /* 0x128 */ s32                    (*charaControlStateReset)(s_SubCharacter* player); // TODO: Not only used by the player.
    /* 0x12C */ s32                    (*charaAnimLock)(s_SubCharacter* chara);
    /* 0x130 */ void                   (*charaAnimIsLocked)(s_SubCharacter* chara);
    /* 0x134 */ s32                    (*charaAnimUnlock)(s_SubCharacter* chara);
    /* 0x138 */ s32                    (*charaAnimPlaybackStateGet)(s_SubCharacter* chara);
    /* 0x13C */ bool                   (*charaPathWaypointExecute)(s_SubCharacter* chara, s32 arg1, VECTOR3* arg2In, s32 angleIn, s32 arg4);
    /* 0x140 */ void                   (*charaVisibleSet)(s_SubCharacter* chara);
    /* 0x144 */ void                   (*charaInvisibleSet)(s_SubCharacter* chara);
    /* 0x148 */ bool                   (*npcFootstepSoundPlay)(s32 animStatus, s_SubCharacter* chara, s32 keyframeIdx0, s32 keyframeIdx1, s32 sfxId, s32 pitch); /** @unused */
    /* 0x14C */ bool                   (*npcSfxPlay)(s32 animStatus, s_SubCharacter* chara, s32 keyframeIdx, s32 sfxId); /** @unused */
    /* 0x150 */ s32                    (*charaAnimStartKeyframeIdxGet)(s_SubCharacter* chara); /** @unused */
    /* 0x154 */ void                   (*charaCollisionReset)(s_SubCharacter* chara); /** @unused */
    /* 0x158 */ void                   (*splitHeadMovementOffsetSet)(q19_12 x, q19_12 z); // Only map1_s05, map1_s06.
    /* 0x15C */ void                   (*func_15C)(); /** @unused Loads number pad. */ // Only map5_s01.
    /* 0x160 */ void                   (*func_160)(); /** @unused */ // Only map5_s01.
    /* 0x164 */ void                   (*func_164)(); /** @unused */ // Only map5_s01.
    /* 0x168 */ void                   (*particleSystemUpdate)(s32 unused, s32 mapId, s32);
    /* 0x16C */ void                   (*particleEnviromentSet)(s8, u32);
    /* 0x170 */ void                   (*func_170)(); /** @unused */
    /* 0x174 */ void                   (*func_174)(); /** @unused */
    /* 0x178 */ void                   (*particleHyperBlasterBeamDraw)(VECTOR3* vec0, q19_12* rotX, q19_12* rotY);
    /* 0x17C */ void                   (*particleBeamDraw)(const VECTOR3* from, const VECTOR3* to);
    /* 0x180 */ void                   (*particleSoundStop)(); /** @unused */

    /* 0x184 */ q19_12*                particleWindSpeedX;
    /* 0x188 */ q19_12*                particleWindSpeedZ;
    /* 0x18C */ s32*                   data_18C;
    /* 0x190 */ s32*                   data_190;
    /* 0x194 */ void                   (*charaUpdateFuncs[Chara_Count])(s_SubCharacter* chara, s_AnmHeader* anmHdr, GsCOORDINATE2* coords); /** Guessed params. Funcptrs for each `e_CharaId`, set to 0 for IDs not included in the map overlay. Called by `Game_NpcUpdate`. */
    /* 0x248 */ s8                     charaGroupIds[CHARA_GROUP_COUNT]; /** `e_CharaId` values where if `s_SpawnInfo::charaId == Chara_None`, `charaGroupIds[0]` is used for `charaSpawnInfos[0]` and `charaGroupIds[1]` for `charaSpawnInfos[1]`. */
    /* 0x24C */ s_SpawnInfo            charaSpawnInfos[2][16];           /** Array of character type/position/flags. `flags == SpawnFlags_None` are unused slots? Read by `Game_NpcRoomInitSpawn`. */
    /* 0x3CC */ VC_ROAD_DATA           cameraPaths[CAMERA_PATH_COUNT_MAX];
    /* 0xD2C */ s_CollisionTrigger     collisionTriggers[COLLISION_TRIGGER_COUNT_MAX];
} s_MapOverlayHdr;
STATIC_ASSERT_SIZEOF(s_MapOverlayHdr, 4172);

extern const s_MapInfo MAP_INFOS[MapType_Count];

/** @brief Checks if a paper map has been collected.
 *
 * @param paperMapIdx Paper map index.
 * @return `true` if the paper map has been collected, `false` otherwise.
 */
#define HAS_PAPER_MAP(paperMapIdx) \
    ((((u32*)&g_SavegamePtr->paperMapFlags)[(paperMapIdx) / 32] >> ((paperMapIdx) % 32)) & (1 << 0))

#endif
