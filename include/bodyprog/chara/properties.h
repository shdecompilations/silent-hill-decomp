#ifndef _BODYPROG_CHARA_PROPERTIES_H
#define _BODYPROG_CHARA_PROPERTIES_H

#include "bodyprog/chara/combat.h"

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

/** @brief Player character properties. */
typedef struct _PropsPlayer
{
    /* 0xE8  */ q19_12 afkTimer; // Increments every tick for 10 seconds before AFK anim starts.
    /* 0xEC  */ q19_12 floorHeight;
    /* 0xF0  */ q19_12 field_F0;
    /* 0xF4  */ q19_12 field_F4; // Angle related to X axis flex rotation.
    /* 0xF8  */ s32    runStepSfxCount;
    /* 0xFC  */ q19_12 exhaustionTimer;
    /* 0x100 */ q19_12 field_100;    // Angle?
    /* 0x104 */ q19_12 field_104;    // Distance?
    /* 0x108 */ q19_12 runDistance;
    /* 0x10C */ u8     field_10C;    // Player SFX pitch?
    /* 0x10D */ u8     field_10D;    // @unused? Set to 0 or 1.
    /* 0x10E */ s8     __pad_10E[2];
    /* 0x110 */ q19_12 timer_110; // Increases when `flags & CharaFlag_Unk4` is set, reset when reaches `D_800C45EC`.
    /* 0x114 */ q19_12 gasWeaponPowerTimer; /** Timer for the Chainsaw and Rock Drill power. */
    /* 0x118 */ s16    field_118; // q3_12?
    /* 0x11A */ s8     __pad_11A[2];
    /* 0x11C */ s32    flags; /** `e_PlayerFlags` */
    /* 0x120 */ q3_12  quickTurnHeadingAngle; /** Target quick turn heading angle. */
    /* 0x122 */ q3_12  field_122; // Specially used when aiming an enemy. Y angle delta to target? 
    /* 0x124 */ q3_12  headingAngle;
    /* 0x126 */ q3_12  moveSpeed; // Used to indicate how much the player should move foward. Seems to be squared.
} s_PropsPlayer;

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

#endif
