#ifndef _BODYPROG_CHARA_CHARA_H
#define _BODYPROG_CHARA_CHARA_H

#include "bodyprog/chara/anim.h"
#include "bodyprog/chara/model.h"
#include "bodyprog/math/math.h"
#include "bodyprog/sys/chara_file_infos.h"
#include "main/fs_queue.h"

#define NPC_COUNT_MAX        6
#define NPC_BONE_COUNT_MAX   10 * NPC_COUNT_MAX
#define CHARA_GROUP_COUNT    4                  /** While up to 6 NPCs and a player can exist in the game world, only 4 different character
                                                 * types (including the player) can be loaded at a time.
                                                 */
#define CHARA_FORCE_FREE_ALL 0xFF               /** `Chara_Load` can force free already loaded models to make room for new ones. */
#define CHARA_LAST_ENEMY_ID  Chara_MonsterCybil /** `e_CharaId` of the last common enemy character. */

/** @brief Returns the `s_SubCharacter` instance for the player character.
 *
 * @note Kept as function-style macro to prevent macro from replacing anything named the same, if we decide to rename these.
 *       e.g. if we had `#define playerChara ...`, that would break things that use `s_SubCharacter* playerChara`
 *       but `#define playerChara() ...` would only replace function-style uses.
*/
#define PLAYER_CHARA() (g_SysWork.playerWork.player)

/** @brief Returns the player character properties. */
#define PLAYER_PROPS() (g_SysWork.playerWork.player.properties.player)

/** @brief Returns the player character extra data. */
#define PLAYER_EXTRA() (g_SysWork.playerWork.extra)

/** @brief Returns the player combat data. */
#define PLAYER_COMBAT() (g_SysWork.playerCombat)

/** @brief Returns the `s_SubCharacter` instance for a given NPC index. */
#define NPC_CHARA(index) (g_SysWork.npcs[(index)])

/** @brief Character flags. Used by `s_SubCharacter::flags`. */
typedef enum _CharaFlags
{
    CharaFlag_None          = 0,
    CharaFlag_PadlockBroken = 1 << 0, /** Only used by Padlock character. */
    CharaFlag_Unk2          = 1 << 1, // Targeted by player?
    CharaFlag_Hit           = 1 << 2, // Associated with recoil-causing hit?
    CharaFlag_Unk4          = 1 << 3,
    CharaFlag_Unk5          = 1 << 4, // Camera-related. Only used by Stalker? Maybe only for alley scenario?
    CharaFlag_Damaged       = 1 << 5,
    CharaFlag_Dead          = 1 << 6, // Unure.
    CharaFlag_Unk8          = 1 << 7,
    CharaFlag_NoRadioStatic = 1 << 8
} e_CharaFlags;

/** @brief Character group flags. Used by `s_SysWork::charaGroupFlags`. */
typedef enum _CharaGroupFlags
{
    CharaGroupFlag_None = 0,
    CharaGroupFlag_0    = 1 << 0,
    CharaGroupFlag_1    = 1 << 1
} e_CharaGroupFlags;

/** @brief Character collision states. */
typedef enum _CharaCollisionState
{
    CharaCollisionState_Ignore  = 0,
    CharaCollisionState_Player  = 1,
    CharaCollisionState_2       = 2,
    CharaCollisionState_Npc     = 3,
    CharaCollisionState_4       = 4,
    CharaCollisionState_Default = 5
} e_CharaCollisionState;

/** @brief Character IDs.
 *
 * @note The `CHARA_FILE_INFOS` array associates each character ID with asset files. */
typedef enum _CharaId
{
    Chara_None             = 0,

    /** Player characters. */

    Chara_Harry            = 1,

    /** Enemy characters. */

    Chara_AirScreamer      = 2,
    Chara_NightFlutter     = 3,
    Chara_Groaner          = 4,
    Chara_Wormhead         = 5,
    Chara_LarvalStalker    = 6,
    Chara_Stalker          = 7,
    Chara_GreyChild        = 8,
    Chara_Mumbler          = 9,
    Chara_HangedScratcher  = 10,
    Chara_Creeper          = 11,
    Chara_Romper           = 12,
    Chara_Chicken          = 13, /** @unused */
    Chara_SplitHead        = 14,
    Chara_Floatstinger     = 15,
    Chara_PuppetNurse      = 16,
    Chara_DummyNurse       = 17, /** Uses dummy anim file without model/texture, but the same update funcptr as `Chara_PuppetNurse`. */
    Chara_PuppetDoctor     = 18,
    Chara_DummyDoctor      = 19, /** Uses dummy anim file without model/texture, but the same update funcptr as `Chara_PuppetDoctor`. */
    Chara_Twinfeeler       = 20,
    Chara_Bloodsucker      = 21,
    Chara_Incubus          = 22,
    Chara_Unknown23        = 23,
    Chara_MonsterCybil     = 24, /** Last common enemy character. Used as `CHARA_LAST_ENEMY_ID`. */

    /** Cutscene characters. */

    Chara_LockerDeadBody   = 25,
    Chara_Cybil            = 26,
    Chara_EndingCybil      = 27,
    Chara_Cheryl           = 28,
    Chara_Cat              = 29,
    Chara_Dahlia           = 30,
    Chara_EndingDahlia     = 31,
    Chara_Lisa             = 32,
    Chara_BloodyLisa       = 33,
    Chara_Alessa           = 34,
    Chara_GhostChildAlessa = 35,
    Chara_Incubator        = 36,
    Chara_BloodyIncubator  = 37,
    Chara_Kaufmann         = 38,
    Chara_EndingKaufmann   = 39,
    Chara_Flauros          = 40,
    Chara_LittleIncubus    = 41,
    Chara_GhostDoctor      = 42,
    Chara_Parasite         = 43,

    /** Special characters. */

    Chara_Padlock          = 44,

    Chara_Count,

/** @brief Grey Child enemy is replaced by the Mumbler in non-USA releases. */
#if VERSION_REGION_IS(NTSC)
    Chara_GreyChildOrMumbler = Chara_GreyChild,
#else
    Chara_GreyChildOrMumbler = Chara_Mumbler,
#endif

    Chara_Hack = NO_VALUE // @hack Force enum to be treated as `s32`.
} e_CharaId;

/** @brief Character damage info. */
typedef struct _CharaDamage
{
    /* 0x0 */ VECTOR3 position;
    /* 0xC */ q19_12  amount;
} s_CharaDamage;

typedef union
{
    s32 val32;
    s16 val16[2];
    s8  val8[4];
} u_Property;

/** @brief Temporary struct. */
typedef struct _PropsDummy
{
    u_Property properties_E8[16];
} s_PropsDummy;
STATIC_ASSERT_SIZEOF(s_PropsDummy, 64);

/** @brief Player character properties. */
typedef struct _PropsPlayer
{
    /* 0xE8  */ q19_12        afkTimer; // Increments every tick for 10 seconds before AFK anim starts.
    /* 0xEC  */ q19_12        floorHeight;
    /* 0xF0  */ q19_12        field_F0;
    /* 0xF4  */ q19_12        field_F4; // Angle related to X axis flex rotation.
    /* 0xF8  */ s32           runStepSfxCount;
    /* 0xFC  */ q19_12        exhaustionTimer;
    /* 0x100 */ q19_12        field_100;    // Angle?
    /* 0x104 */ q19_12        field_104;    // Distance?
    /* 0x108 */ q19_12        runDistance;
    /* 0x10C */ u8            field_10C;    // Player SFX pitch?
    /* 0x10D */ u8            field_10D;    // @unused? Set to 0 or 1.
    /* 0x10E */ s8            __pad_10E[2];
    /* 0x110 */ q19_12        timer_110; // Increases when `flags & CharaFlag_Unk4` is set, reset when reaches `D_800C45EC`.
    /* 0x114 */ q19_12        gasWeaponPowerTimer; // Timer for the rock drill and chainsaw power.
    /* 0x118 */ s16           field_118; // q3_12?
    /* 0x11A */ s8            __pad_11A[2];
    /* 0x11C */ s32           flags; /** `e_PlayerFlags` */
    /* 0x120 */ q3_12         quickTurnHeadingAngle; /** Target quick turn heading angle. */
    /* 0x122 */ q3_12         field_122; // Specially used when aiming an enemy. Y angle delta to target? 
    /* 0x124 */ q3_12         headingAngle;
    /* 0x126 */ q3_12         moveSpeed; // Used to indicate how much the player should move foward. Seems to be squared.
} s_PropsPlayer;
STATIC_ASSERT_SIZEOF(s_PropsPlayer, 64);

// TODO: Unsure if this struct is puppet doctor specific or shared with all characterss. Pointer gets set at puppetDoc+0x124.
typedef struct
{
    /* 0x0  */ q19_12      health_0;
    /* 0x4  */ s32         field_4;
    /* 0x8  */ s32         field_8;
    /* 0xC  */ s32         field_C;
    /* 0x10 */ s8          unk_10[8];
    /* 0x18 */ q19_12      field_18; // Angle?
    /* 0x1C */ s32         idx_1C;
    /* 0x20 */ s32         field_20;
    /* 0x24 */ s_AnimInfo* animInfo_24;
    /* 0x28 */ s8          unk_28[4];
    /* 0x2C */ q19_12      field_2C;
    /* 0x30 */ s8          unk_30[4];
} s_800D5710;
STATIC_ASSERT_SIZEOF(s_800D5710, 52);

// Used by Alessa, Bloody Incubator, BloodyLisa, Cheryl, Cybil, Dahlia, Ghost Child Alessa, Ghost Doctor, Incubator,
// Kaufmann, Lisa.
typedef struct _PropsNpc
{
    /* 0xE8  */ s32    controlState; /** `e_*Control` */
    /* 0xEC  */ s16    field_EC;
    /* 0xEE  */ s16    field_EE;
    /* 0xF0  */ s32    freeze; // `bool`
    /* 0xF4  */ s32    field_F4;
    /* 0xF8  */ s32    resetControlState; // `bool`
    /* 0xFC  */ s8     __pad_FC[2];
    /* 0xFE  */ s16    field_FE; // Index.
    /* 0x100 */ q19_12 field_100;
    /* 0x104 */ s16    field_104;
    /* 0x106 */ q3_12  field_106;
    /* 0x108 */ q19_12 field_108; // Timer.
    /* 0x10C */ q19_12 field_10C; // Timer.
    /* 0x110 */ q19_12 distanceToPlayer;
    /* 0x114 */ u8     field_114;
    /* 0x115 */ u8     field_115;
    /* 0x116 */ u8     field_116;
    /* 0x117 */ s8     __pad_117;
    /* 0x118 */ q3_12  field_118; // Angle.
    /* 0x11A */ q3_12  field_11A; // Angle.
    /* 0x11C */ s32    flags; /** `e_*Flags` */
    /* 0x120 */ s32    field_120; // Kaufmann: set once his gunshot effect has started.
    /* 0x124 */ q3_12  moveDistance_124;
    /* 0x126 */ q3_12  moveSpeed;
} s_PropsNpc;
STATIC_ASSERT_SIZEOF(s_PropsNpc, 64);

/** @brief Properties used by `Chara_PathWaypointExecute` while an NPC walks an event path.
 TODO: Most fields here likely overwrite existing properties on that NPC at same offset, should note down which ones.
*/
typedef struct _PropsNpcPath
{
    /* 0xE8  */ s32 controlState; /** `e_*Control` */
    /* 0xEC  */ s32 pathStep;
    /* 0xF0  */ s32 freeze;       /** `bool` */
    /* 0xF4  */ s32 field_F4;
    /* 0xF8  */ s32 resetControlState;
    /* 0xFC  */ s32 field_FC;     // Set alongside `controlState`.
    /* 0x100 */ s8  unk_100[36];
    /* 0x124 */ s32 field_124;    // `moveDistance_124` and `moveSpeed` together.
} s_PropsNpcPath;
STATIC_ASSERT_SIZEOF(s_PropsNpcPath, 64);

/** @brief Air Screamer or Night Flutter character properties. */
typedef struct _PropsAirScreamer
{
    /* 0xE8+0  */ u32     field_E8_0  : 4; // `AirScreamerHit_None` step.
    /* 0xE8+4  */ u32     field_E8_4  : 4; /** `bool` */
    /* 0xE8+8  */ u32     field_E8_8  : 4;
    /* 0xE8+12 */ u32     __pad_E8_12 : 20;
    /* 0xEC    */ s32     field_EC;
    /* 0xF0    */ s16     field_F0; // } Maybe 2D offset like in Creeper properties? Must check.
    /* 0xF2    */ s16     field_F2; // }
    /* 0xF4    */ s32     field_F4;
    /* 0xF8    */ VECTOR3 targetPosition;     /** Q19.12 */
    /* 0x104   */ VECTOR3 headTargetPosition; /** Q19.12 */
    /* 0x110   */ VECTOR3 targetPosition2;    /** Q19.12 | TODO: Unknown purpose. */
    /* 0x11C   */ s32     flags; /** `e_AirScreamerFlags` */
    /* 0x120   */ q19_12  timer_120;
    /* 0x124   */ q19_12  floorHeight;
} s_PropsAirScreamer;

/** @brief Bloodsucker character properties. */
typedef struct _PropsBloodsucker
{
    /* 0xE8  */ q19_12 timer_E8;
    /* 0xEC  */ q19_12 timer_EC;
    /* 0xF0  */ q19_12 timer_F0;
    /* 0xF4  */ q19_12 timer_F4;
    /* 0xF8  */ s8     unused_F8[36]; /** @unused Probably explicit filler fields originally. */
    /* 0x11C */ s32    flags;         /** `e_BloodsuckerFlags` */
} s_PropsBloodsucker;

/** @brief Cat character properties. */
typedef struct _PropsCat
{
    /* 0xE8 */ u8 field_E8; // `bool`
} s_PropsCat;

/** @brief Creeper character properties. */
typedef struct _PropsCreeper
{
    /* 0xE8  */ u16    flags; /** `e_CreeperFlags` */
    /* 0xEA  */ s8     __pad_EA[2];
    /* 0xEC  */ q3_12  collisionOffsetX;
    /* 0xEE  */ q3_12  collisionOffsetZ;
    /* 0xF0  */ q19_12 attackTimer;
    /* 0xF4  */ q19_12 targetPositionX;
    /* 0xF8  */ q19_12 targetPositionZ;
    /* 0xFC  */ q19_12 homePositionX;
    /* 0x100 */ q19_12 homePositionZ;
    /* 0x104 */ q19_12 chirpTimer;
    /* 0x108 */ q3_12  angleToTarget;
    /* 0x10A */ s16    animStatus_10A; // TODO: Purpose unclear.
    /* 0x10C */ q4_12  moveSpeed;
} s_PropsCreeper;
STATIC_ASSERT_SIZEOF(s_PropsCreeper, 40);

/** @brief Floatstinger character properties. */
typedef struct _PropsFloatstinger
{
    /* 0xE8  */ s16        flags_E8;
    /* 0xEA  */ s8         unk_EA[2];
    /* 0xEC  */ q3_12      field_EC; // `vy` angle.
    /* 0xEE  */ q3_12      field_EE; // Angle.
    /* 0xF0  */ q19_12     field_F0; // Timer.
    /* 0xF4  */ q3_12      field_F4; // Angle.
    /* 0xF6  */ s8         __pad_F6[2];
    /* 0xF8  */ u16        field_F8;
    /* 0xFA  */ u16        field_FA;
    /* 0xFC  */ q4_12      field_FC; // Timer?
    /* 0xFE  */ s8         __pad_FE[2];
    /* 0x100 */ q19_12     field_100; // Timer.
    /* 0x104 */ u8         field_104;
    /* 0x105 */ s8         __pad_105;
    /* 0x106 */ s16        field_106;
    /* 0x108 */ q19_12     field_108; // Damage related.
    /* 0x10C */ q3_12      field_10C; // Angle, backup of `field_F4`?
    /* 0x10E */ q3_12      field_10E; // Timer?
    /* 0x110 */ s8         unk_110[24];
} s_PropsFloatstinger;
STATIC_ASSERT_SIZEOF(s_PropsFloatstinger, 64);

/** @brief Groaner character properties. */
typedef struct _PropsGroaner
{
    /* 0xE8  */ u_Property flags; /** `e_GroanerFlags` TODO: One weird exception where it's accessed as `s32`. */
    /* 0xEC  */ q3_12      targetHeadingAngle;
    /* 0xEE  */ q3_12      flexAngle;
    /* 0xF0  */ q3_12      field_F0; // } XZ offset?
    /* 0xF2  */ q3_12      field_F2; // }
    /* 0xF4  */ q19_12     targetPositionX;
    /* 0xF8  */ q19_12     targetPositionZ;
    /* 0xFC  */ q3_12      angleToTarget;
    /* 0xFE  */ q3_12      field_FE;
    /* 0x100 */ u16        relKeyframeIdx_100;
    /* 0x102 */ s8         __pad_102[2];
    /* 0x104 */ q19_12     timer_104;
    /* 0x108 */ u32        field_108;
    /* 0x10C */ q3_12      timer_10C; // SFX timer?
    /* 0x10E */ u8         field_10E; // } Sound states?
    /* 0x10F */ u8         field_10F; // }
    /* 0x110 */ u8         playLeftFootstepSfx;  /** `bool` */
    /* 0x111 */ u8         playRightFootstepSfx; /** `bool` */
    /* 0x112 */ s8         __pad_112[2];
    /* 0x114 */ q3_12      field_114; // Move speed coefficient?
} s_PropsGroaner;

/** @brief Hanged Scratcher character properties. */
typedef struct _PropsHangedScratcher
{
    /* 0xE8  */ s16    flags; /** `e_HangedScratcherFlags` */
    /* 0xEA  */ q4_12  timer_EA;
    /* 0xEC  */ q3_12  targetHeadingAngle_EC;
    /* 0xEE  */ u8     field_EE;
    /* 0xEF  */ s8     __pad_EF;
    /* 0xF0  */ q3_12  offsetX_F0; // } Offsets passed to `Chara_MovementUpdate1`
    /* 0xF2  */ q3_12  offsetZ_F2; // }
    /* 0xF4  */ q19_12 positionX_F4;
    /* 0xF8  */ q19_12 positionZ_F8;
    /* 0xFC  */ q3_12  field_FC;
    /* 0xFE  */ s8     __pad_FE[2];
    /* 0x100 */ q3_12  timer_100;
    /* 0x102 */ u8     field_102;
    /* 0x103 */ u8     field_103;
    /* 0x104 */ s32    field_104;
    /* 0x108 */ q3_12  timer_108;
    /* 0x10A */ s8     __pad_10A[2];
    /* 0x10C */ q4_12  radiusMax_10C; // } Used as `Chara_MoveSpeedUpdate` limit param, TODO: rename?
    /* 0x10E */ q4_12  radiusMin_10E; // }
} s_PropsHangedScratcher;

/** @brief Incubus character properties, shared with Unknown23? */
typedef struct _PropsIncubus
{
    /* 0xE8 */ q19_12 timer_E8;
    /* 0xEC */ s32    field_EC; // Flags?
    /* 0xF0 */ s32    someState_F0;
    /* 0xF4 */ q19_12 bossFightTimer_F4;
    /* 0xF8 */ s8     __pad_F8[48];
} s_PropsIncubus;

/** @brief Larval Stalker character properties. */
typedef struct _PropsLarvalStalker
{
    /* 0xE8  */ u16        flags_E8; /** `e_LarvalStalkerFlags` */
    /* 0xEA  */ u8         field_EA;
    /* 0xEB  */ s8         __pad_EB;
    /* 0xEC  */ q19_12     timer_EC;
    /* 0xF0  */ q20_12     timer_F0;
    /* 0xF4  */ s16        keyframeIdx_F4; // Relative keyframe?
    /* 0xF6  */ s16        keyframeIdx_F6; // Relative keyframe?
    /* 0xF8  */ q19_12     targetPositionX;
    /* 0xFC  */ q19_12     targetPositionZ;
    /* 0x100 */ q3_12      angle_100;
    /* 0x102 */ q3_12      angle_102;
    /* 0x104 */ q19_12     animTime_104;
    /* 0x108 */ q3_12      angle_108;
    /* 0x10A */ q4_12      timer_10A;
    /* 0x10C */ u_Property field_10C;
    /* 0x110 */ VECTOR3    field_110;
    /* 0x11C */ s32        flags_11C;
    /* 0x120 */ u_Property field_120;
    /* 0x124 */ s16        field_124;
    /* 0x126 */ q3_12      moveSpeed;
} s_PropsLarvalStalker;
STATIC_ASSERT_SIZEOF(s_PropsLarvalStalker, 64);

/** @brief Monster Cybil character properties. */
typedef struct _PropsMonsterCybil
{
    /* 0xE8  */ s32    controlState; /** `e_MonsterCybilControl` */
    /* 0xEC  */ s16    field_EC;
    /* 0xEE  */ s16    field_EE;          // Anim index (or full anim status?).
    /* 0xF0  */ q19_12 freeze;            // Timer.
    /* 0xF4  */ s32    field_F4;
    /* 0xF8  */ q19_12 resetControlState; // Timer.
    /* 0xFC  */ s8     __pad_FC[2];
    /* 0xFE  */ s16    field_FE;          // Index.
    /* 0x100 */ q19_12 field_100;
    /* 0x104 */ s16    field_104;
    /* 0x106 */ q3_12  field_106;         // Angle or counter (`func_800D8D7C`)? May change usage depending on state step?
    /* 0x108 */ q19_12 field_108;         // Timer.
    /* 0x10C */ q19_12 field_10C;         // Timer.
    /* 0x110 */ q19_12 distanceToPlayer;
    /* 0x114 */ u8     field_114;
    /* 0x115 */ u8     field_115;
    /* 0x116 */ u8     field_116;
    /* 0x117 */ s8     __pad_117;
    /* 0x118 */ q3_12  field_118;         // Angle.
    /* 0x11A */ q3_12  field_11A;         // Angle.
    /* 0x11C */ s32    flags;
    /* 0x120 */ s16    field_120;         // `bool`
    /* 0x122 */ q3_12  field_122;         // Angle to player.
    /* 0x124 */ q3_12  moveDistance_124;
    /* 0x126 */ q3_12  moveSpeed;
} s_PropsMonsterCybil;
STATIC_ASSERT_SIZEOF(s_PropsMonsterCybil, 64);

/** @brief Puppet Nurse or Puppet Doctor character properties. */
typedef struct _PropsPuppetNurse
{
    /* 0xE8  */ VECTOR3       position_E8; /** Q19.12 */
    /* 0xF4  */ s_CharaDamage damage;
    /* 0x104 */ q19_12        field_104; // Timer.
    /* 0x108 */ q19_12        field_108; // `vx` position?
    /* 0x10C */ q19_12        field_10C; // `vz` position?
    /* 0x110 */ q19_12        moveSpeed;
    /* 0x114 */ q19_12        field_114; // Damage related.
    /* 0x118 */ u8            field_118; // Used in switch, values 0 - 2.
    /* 0x119 */ u8            modelVariantIdx;
    /* 0x11A */ q3_12         field_11A; // Timer.
    /* 0x11C */ q3_12         field_11C; // Heading angle.
    /* 0x11E */ s16           field_11E; // `bool`
    /* 0x120 */ q3_12         field_120;
    /* 0x122 */ u16           flags_122; /** `e_PuppetNurseFlags` */
    /* 0x124 */ s_800D5710*   field_124;
} s_PropsPuppetNurse;
STATIC_ASSERT_SIZEOF(s_PropsPuppetNurse, 64);

/** @brief Romper character properties. */
typedef struct _PropsRomper
{
    /* 0xE8  */ s32    flags; /** `e_RomperFlags` */
    /* 0xEC  */ q3_12  angle_EC; // Target heading angle?
    /* 0xEE  */ s16    field_EE;
    /* 0xF0  */ q3_12  field_F0; // Move speed accumulation for this tick.
    /* 0xF2  */ q3_12  rotationY_F2;
    /* 0xF4  */ q19_12 field_F4; // Relative anim time?
    /* 0xF8  */ q3_12  movementOffsetX;
    /* 0xFA  */ q3_12  movementOffsetZ;
    /* 0xFC  */ q19_12 targetPositionX_FC;
    /* 0x100 */ q19_12 targetPositionZ_100;
    /* 0x104 */ s32    field_104;
    /* 0x108 */ q19_12 positionX_108;
    /* 0x10C */ u8     field_10C; // Relative keyframe index?
    /* 0x10D */ s8     __pad_10D;
    /* 0x10E */ u16    field_10E;
    /* 0x110 */ q19_12 positionZ_110;
    /* 0x114 */ u8     field_114;
    /* 0x115 */ u8     field_115;
    /* 0x116 */ q3_12  field_116;
    /* 0x118 */ q3_12  timer_118;
    /* 0x11A */ u8     field_11A;
    /* 0x11B */ s8     __pad_11B;
    /* 0x11C */ q3_12  timer_11C;
    /* 0x11E */ s8     __pad_11E[2];
    /* 0x120 */ q19_12 distance_120;
    /* 0x124 */ q19_12 field_124; // Move speed step?
} s_PropsRomper;

/** @brief Split Head character properties. */
typedef struct _PropsSplitHead
{
    /* 0xE8  */ u16     flags; /** `e_SplitHeadFlags` */
    /* 0xEA  */ u8      field_EA;
    /* 0xEB  */ s8      __pad_EB;
    /* 0xEC  */ q3_12   angle_EC;
    /* 0xEE  */ q3_12   field_EE;
    /* 0xF0  */ q4_12   angle_F0;
    /* 0xF2  */ q4_12   timer_F2;
    /* 0xF4  */ q4_12   timer_F4;
    /* 0xF8  */ s8      __pad_F8[2];
    /* 0xF8  */ q19_12  animTime; // Set to `Q12(201.0f)` if not walking forward.
    /* 0xFC  */ s32     field_FC;
    /* 0x100 */ s32     field_100;
    /* 0x104 */ s8      unk_104[4];
    /* 0x108 */ u8      field_108[4];
    /* 0x10C */ q19_12  field_10C;
    /* 0x110 */ VECTOR3 field_110;
    /* 0x11C */ s32     flags_11C; /** `e_SplitHeadFlags` */
    /* 0x120 */ s8      unk_120[4];
    /* 0x124 */ s16     field_124;
    /* 0x126 */ q3_12   moveSpeed;
} s_PropsSplitHead;
STATIC_ASSERT_SIZEOF(s_PropsSplitHead, 64);

/** @brief Stalker character properties. */
typedef struct _PropsStalker
{
    /* 0xE8  */ s16    flags; /** `e_StalkerFlags` */
    /* 0xEA  */ s8     __pad_EA[2];
    /* 0xEC  */ q3_12  offset_EC;
    /* 0xEE  */ q3_12  offset_EE;
    /* 0xF0  */ q19_12 targetPositionX;
    /* 0xF4  */ q19_12 targetPositionZ;
    /* 0xF8  */ q19_12 timer_F8;
    /* 0xFC  */ s16    keyframeIdx_FC;    // Or anim status?? Seems to be used as both.
    /* 0xFE  */ s16    relKeyframeIdx_FE; // Unsure.
    /* 0x100 */ q3_12  targetHeadingAngle;
    /* 0x102 */ s16    sfxId_102;
    /* 0x104 */ q19_12 relAnimTime_104;
    /* 0x108 */ q4_12  timer_108;
    /* 0x10A */ u8     field_10A;
    /* 0x10B */ s8     __pad_10B;
    /* 0x10C */ q19_12 timer_10C;
    /* 0x110 */ q19_12 health_110;
    /* 0x114 */ q3_12  angle_114;
    /* 0x116 */ q4_12  timer_116;
} s_PropsStalker;

/** @brief Twinfeeler character properties. */
typedef struct _PropsTwinfeeler
{
    // TODO: Weird `field_E8` access.
    /* 0xE8  */ u_Property    field_E8;
    /* 0xE8  */ //q3_12         sfxTimer_E8;
    /* 0xE8  */ //q4_12         field_EA;
    /* 0xEC  */ s_CharaDamage damage;
    /* 0xFC  */ q19_12        digTimer;
    /* 0x100 */ q19_12        spawnPositionX; /** @unused */
    /* 0x104 */ q19_12        spawnPositionZ; /** @unused */
    /* 0x108 */ q19_12        prevMoveSpeed;
    /* 0x10C */ q19_12        accumulatedDamage;
    /* 0x110 */ s16           field_110; /** @unused */
    /* 0x112 */ s8            __pad_112[2];
    /* 0x114 */ u32           flags;     /** `e_TwinfeelerFlags` */
    /* 0x118 */ u16           field_118; /** `bool` */
    /* 0x11A */ s8            __pad_11A[2];
    /* 0x11C */ q19_12        prevHealth;
    /* 0x124 */ s8            __pad_120[8];
} s_PropsTwinfeeler;
STATIC_ASSERT_SIZEOF(s_PropsTwinfeeler, 64);

/** @brief State of the attack a character is currently performing.
 *
 * @note SH2 keeps similar data to this inside `shBattleInfo`, and also seemed 
 * to move some SubCharacter fields (`flags`, `field_40`, `attackReceived`) inside it.
 */
typedef struct _CharaAttack
{
    /* 0x0  */ s16     state;        /** 0: idle, 1: requested, 2: first active tick, 3: hit checks running, `NO_VALUE`: finished.
                                       * Reset to 0 by `func_8008A3AC` when `CharaFlag_Unk8` is clear. */
    /* 0x2  */ u8      weaponAttack; /** Packed `WEAPON_ATTACK` being performed. Index into `g_Items_WeaponAttackInfos`. */
    /* 0x3  */ u8      shotsLeft;    /** Gunshots only. Set to 100 on start, decremented per shot. Attack ends at 0. */
    /* 0x4  */ u8      shotsFired;   /** Gunshots only. Compared against `s_AttackShot::shotCountMax`. */
    /* 0x5  */ s8      __pad_5[3];
    /* 0x8  */ s32     hitFlags;     /** Bit `1 << npcIdx` per NPC hit by this attack, `NO_VALUE` (all bits set) if the player is hit. */
    /* 0xC  */ q3_12   aimYaw;
    /* 0xE  */ q3_12   aimPitch;     /** Measured from vertical, 90 degrees is level. */
    /* 0x10 */ q3_12   prevAimYaw;
    /* 0x12 */ q3_12   prevAimPitch;
    /* 0x14 */ q19_12  hitCheckTime; /** Animation time that hit checks have been run up to, clamped to the attack's hit window. */
    /* 0x18 */ VECTOR3 origin;       /** Q19.12 | Attack origin / ray start for gunshots. */
    /* 0x24 */ VECTOR3 segStart[2];  /** Q19.12 | Hit segment start, `origin` + direction * `rangeMin`. [0]: this tick, [1]: previous tick. */
    /* 0x3C */ VECTOR3 prevOrigin;   /** Q19.12 */
    /* 0x48 */ VECTOR3 segVec[2];    /** Q19.12 | Hit segment, direction * (`rangeMax` - `rangeMin`). [0]: this tick, [1]: previous tick. */
    /* 0x60 */ VECTOR3 field_60;     /** Q19.12 | Read by `func_8008B714` as the offset from `prevOrigin` to the previous segment end,
                                       * but never written, always 0 after the `bzero` during character spawn? */
} s_CharaAttack;
STATIC_ASSERT_SIZEOF(s_CharaAttack, 108);

/** @brief Character collision box. */
typedef struct _CharaBox
{
    /* 0x0 */ q3_12 topExtent;
    /* 0x2 */ q3_12 bottomExtent;
    /* 0x4 */ q3_12 height;
    /* 0x6 */ q3_12 offsetY;
    // TODO: Maybe nested inner and outer XZ extents. See `s_CharaCylinder`.
    /* 0x8 */ q3_12 field_8; // Outer XZ extent? Always nagative, might be unsigned? `s_CharaCylinder::outerRadius` is set to this.
    /* 0xA */ q3_12 field_A; // Innter XZ extent? `s_CharaCylinder::innerRadius` is set to this.
} s_CharaBox;
STATIC_ASSERT_SIZEOF(s_CharaBox, 12);

// TODO: Too similar to `s_CharaCollisionCylinder`. Maybe more like character detection ranges?
/** @brief Character collision cylinder. */
typedef struct _CharaCylinder
{
    /* 0x0 */ q3_12 outerRadius;
    /* 0x2 */ q3_12 innerRadius;
} s_CharaCylinder;
STATIC_ASSERT_SIZEOF(s_CharaCylinder, 4);

/** @brief Character shape offsets for `s_CharaBox` and `s_CharaCylinder`. */
typedef struct _CharaShapeOffsets
{
    /* 0x0 */ DVECTOR_XZ box;
    /* 0x4 */ DVECTOR_XZ cylinder;
} s_CharaShapeOffsets;
STATIC_ASSERT_SIZEOF(s_CharaShapeOffsets, 8);

/** @brief Character keyframe collision info. */
typedef struct _CharaKeyframe
{
    /* 0x0 */ s_CharaBox          box;
    /* 0xC */ s_CharaShapeOffsets shapeOffsets;
} s_CharaKeyframe;

/** @brief Recorded character-to-character collision.
 * Similar to `s_CharaCollisionCylinder` but with extra `passStamp` field.
 */
typedef struct _CharaCollisionRecord
{
    /* 0x0  */ VECTOR3 position;     /** Q19.12 */
    /* 0xC  */ q3_12   bottomExtent;
    /* 0xE  */ q3_12   topExtent;
    /* 0x10 */ q3_12   radius;
    /* 0x12 */ s8      state;        /** `e_CharaCollisionState` */
    /* 0x13 */ u8      passStamp;    /** Tracks visited characters per query pass. */
} s_CharaCollisionRecord;

/** @brief Character collision info for the active tick. */
typedef struct _CharaCollision
{
    /* 0x0    */ s_CharaBox              box;
    /* 0xC    */ s_CharaCylinder         cylinder;
    /* 0x10   */ s_CharaShapeOffsets     shapeOffsets;         // Translation data?
    /* 0x18+0 */ u8                      passStamp;            /** Same usage as `s_IpdCollisionData::shapePassStampBuffer`. */
    /* 0x19+0 */ s8                      state            : 4; /** `e_CharaCollisionState` */
    /* 0x19+4 */ u8                      charaRecordCount : 4; /** `charaRecords` array size. */
    /* 0x1C   */ s_CharaCollisionRecord* charaRecords;
} s_CharaCollision;

/** @brief Character info. */
typedef struct _SubCharacter
{
    /* 0x0  */ s_Model           model;    // In player: Manage the half lower part of Harry's body animations (legs and feet).
    /* 0x18 */ VECTOR3           position; /** Q19.12 */
    /* 0x24 */ SVECTOR3          rotation; /** Q3.12 */
    /* 0x2A */ q3_12             angleToTarget;
    /* 0x2C */ SVECTOR3          rotationSpeed;              /** Q3.12 | Rotation speed for `rotation`. */
    /* 0x32 */ q3_12             angleToTargetRotationSpeed; /** Rotation speed for `angleToTarget`. */
    /* 0x34 */ q19_12            fallSpeed;
    /* 0x38 */ q19_12            moveSpeed;
    /* 0x3C */ q3_12             headingAngle;
    /* 0x3E */ s16               flags;          /** `e_CharaFlags` */
    /* 0x40 */ s8                field_40;       // In player: Index of the NPC attacking the player. Spawn index for Air Screamer?
                                                 // In NPCs: Unknown. `Game_NpcRoomInitSpawn` sugests it indicates the NPC index in `s_Savegame::mapEnemyStates`.
    /* 0x41 */ s8                attackReceived; // Packed weapon attack indicating what attack has been performed to the character. See `WEAPON_ATTACK`.
               // 2 bytes of padding.
    /* 0x44 */ s_CharaAttack     attack;
    /* 0xB0 */ q19_12            health;
    /* 0xB4 */ s_CharaDamage     damage;
    /* 0xC4 */ u16               deathTimer;     // Part of `shBattleInfo` struct in SH2, may use something similar here.
    /* 0xC6 */ q3_12             timer_C6;       // Some sort of timer. Written to by `LarvalStalker_Update`.
    /* 0xC8 */ s_CharaCollision  collision;
               union
               {
                   s_PropsDummy           dummy;
                   s_PropsPlayer          player;
                   s_PropsNpc             npc;
                   s_PropsNpcPath         npcPath;
               
                   s_PropsAirScreamer     airScreamer;
                   s_PropsBloodsucker     bloodsucker;
                   s_PropsCat             cat;
                   s_PropsCreeper         creeper;
                   s_PropsFloatstinger    floatstinger;
                   s_PropsGroaner         groaner;
                   s_PropsHangedScratcher hangedScratcher;
                   s_PropsIncubus         incubus;
                   s_PropsLarvalStalker   larvalStalker;
                   s_PropsMonsterCybil    monsterCybil;
                   s_PropsPuppetNurse     puppetNurse;
                   s_PropsRomper          romper;
                   s_PropsSplitHead       splitHead;
                   s_PropsStalker         stalker;
                   s_PropsTwinfeeler      twinfeeler;
    /* 0xE8 */ } properties;
} s_SubCharacter;
STATIC_ASSERT_SIZEOF(s_SubCharacter, 296);

/** @brief Sets the collision shapes of a character from keyframe collision data.
 *
 * @param chara Character to update (`s_SubCharacter`).
 * @param keyframe Keyframe collision data (`s_CharaKeyframe`).
 */
#define Chara_CollisionSet(chara, keyframe)                                        \
{                                                                                  \
    s32 __temp;                                                                    \
    s32 __temp2;                                                                   \
                                                                                   \
    chara->collision.box.topExtent = keyframe.box.topExtent;                       \
                                                                                   \
    __temp                            = keyframe.box.bottomExtent;                 \
    chara->collision.box.bottomExtent = __temp;                                    \
    chara->collision.box.height       = keyframe.box.height;                       \
                                                                                   \
    __temp                                    = keyframe.box.offsetY;              \
    chara->collision.box.offsetY              = __temp;                            \
    chara->collision.shapeOffsets.cylinder.vx = keyframe.shapeOffsets.cylinder.vx; \
                                                                                   \
    __temp                                    = keyframe.shapeOffsets.cylinder.vz; \
    chara->collision.shapeOffsets.cylinder.vz = __temp;                            \
    chara->collision.cylinder.outerRadius     = keyframe.box.field_8;              \
    chara->collision.shapeOffsets.box.vx      = keyframe.shapeOffsets.box.vx;      \
                                                                                   \
    __temp                               = keyframe.shapeOffsets.box.vz;           \
    chara->collision.shapeOffsets.box.vz = __temp;                                 \
                                                                                   \
    __temp2                               = keyframe.box.field_A;                  \
    chara->collision.cylinder.innerRadius = __temp2;                               \
}

/** @brief Checks if the `s_SubCharacter*` has the given `flags` value set. */
#define Chara_HasFlag(chara, flag) \
    ((chara)->flags & (flag))

/** @brief Clears a character's properties. TODO: Declare `i` here.
 *
 * @param chara Character to update.
 */
#define Chara_PropsClear(chara)                             \
    for (i = 0; i < 16; i++)                                \
    {                                                       \
        chara->properties.dummy.properties_E8[i].val32 = 0; \
    }

/** @brief Clears a character's damage field.
 *
 * @param chara Character to update.
 */
#define Chara_DamageClear(chara)             \
    (chara)->damage.amount      = Q12(0.0f); \
    (chara)->damage.position.vz = Q12(0.0f); \
    (chara)->damage.position.vy = Q12(0.0f); \
    (chara)->damage.position.vx = Q12(0.0f)

/** @brief Sets a character's received attack type.
 *
 * TODO: Could make this use `WEAPON_ATTACK` macro and take the values needed for that instead.
 * Maybe devs did similar thing, which is why this separate macro/inline is needed for a match?
 *
 * @param chara Character to update.
 * @param attack Attack type to set.
 */
#define Chara_AttackReceivedSet(chara, attack) \
    (chara)->attackReceived = (attack)

/** @brief Gets a character's received attack type.
 *
 * @param chara Character to update.
 */
#define Chara_AttackReceivedGet(chara) \
    (chara)->attackReceived

/** @brief Updates an NPC's animation if the `freeze` flag isn't currently set.
 *
 * @note This is only for NPCs which use the `s_PropsNpc` part of the `s_SubCharacter::properties` union.
 *
 * @param chara Character to update.
 * @param anmHdr Character animation data.
 * @param boneCoords Character model bone coords.
 * @param animInfos Character animation infos.
 */
#define Chara_AnimUpdate(chara, anmHdr, boneCoords, animInfos)                     \
{                                                                                  \
    if (!(chara)->properties.npc.freeze)                                           \
    {                                                                              \
        s_AnimInfo* __animInfo = &(animInfos)[(chara)->model.anim.status];         \
                                                                                   \
        __animInfo->playbackFunc(&(chara)->model, anmHdr, boneCoords, __animInfo); \
    }                                                                              \
}

/** @brief Sets the animation of a character.
 *
 * @param chara Character to set animation for. TODO: Maybe should take `s_ModelAnim` instead? If fits better, also rename to `Anim_Set`.
 * @param animStatus Packed anim status. See `s_ModelAnim::status`.
 * @param keyframeIdx Active keyframe index.
 */
static inline void Chara_AnimSet(s_SubCharacter* chara, s32 animStatus, s32 keyframeIdx)
{
    chara->model.anim.status      = animStatus;
    chara->model.anim.time        = Q12(keyframeIdx);
    chara->model.anim.keyframeIdx = keyframeIdx;
}

/** @brief Resets an NPC's control state to `*_None` if the control subsystem was flagged for a reset.
 *
 * @note This is only for NPCs which use the `s_PropsNpc` part of the properties union.
 *
 * @param chara Character to update.
 */
static inline void Chara_AnimStateReset(s_SubCharacter* chara)
{
    if (chara->properties.npc.resetControlState)
    {
        chara->properties.npc.controlState      = 0;
        chara->model.stateStep                  = 0;
        chara->properties.npc.resetControlState = false;
    }
}

#endif
