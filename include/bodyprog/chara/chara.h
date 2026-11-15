#ifndef _BODYPROG_CHARA_CHARA_H
#define _BODYPROG_CHARA_CHARA_H

#include "bodyprog/chara/anim.h"
#include "bodyprog/chara/combat.h"
#include "bodyprog/chara/model.h"
#include "bodyprog/chara/properties.h"
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

/** @brief Checks if a character has given flags set.
 *
 * @param chara Character to check (`s_SubCharacter`).
 * @param flag Flag to check (`e_CharaFlags`).
 * @return `true` if the flags are set, `false` otherwise.
 */
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

/** @brief Clears a character's damage parameters.
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
 * @param attack Packed weapona attack to set. See `WEAPON_ATTACK`.
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

/** @brief Sets a character's animation.
 *
 * @param chara Character to set the animation for.
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
