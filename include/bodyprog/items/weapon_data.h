#ifndef _BODYPROG_ITEMS_WEAPONDATA_H
#define _BODYPROG_ITEMS_WEAPONDATA_H

/** @brief Melee attack box.
 *
 * SH2 name: `shBattleFight`.
 */
typedef struct _AttackFight
{
    /* 0x0 */ q4_12 sizeX; /** SH2 name: `test_a`. */
    /* 0x2 */ q4_12 sizeY; /** SH2 name: `test_b`. */
    /* 0x4 */ q4_12 sizeZ; /** SH2 name: `test_c`. */
} s_AttackFight;

/** @brief Gunshot attack parameters.
 *
 * SH2 name: `shBattleShot`.
 */
typedef struct _AttackShot
{
    /* 0x0 */ q4_12 interval;    /** Animation time between shots. */
    /* 0x2 */ u8    maxShots;    /** 0 is treated as 1. */
    /* 0x3 */ u8    pelletCount; /** Rays trace per shot. */
    /* 0x4 */ u8    inaccuracy;
    /* 0x5 */ u8    spread;      /** Low nibble: `D_800AFD1C` index, high nibble: step count. */
} s_AttackShot;

typedef enum _AttackInfoType
{
    AttackInfoType_Unk0      = 0,
    AttackInfoType_Melee     = 1, /** Melee hitting every frame. */
    AttackInfoType_Melee1Hit = 2, /** Melee stopping on first hit. */
    AttackInfoType_Gunshot   = 3,
    AttackInfoType_Enemy0    = 4, // } TODO: Better naming.
    AttackInfoType_Enemy1    = 5, // }
    AttackInfoType_Enemy2    = 6, // }
} e_AttackInfoType;

/** @brief Attack definition, indexed by `WEAPON_ATTACK` value.
 *
 * @note Struct contents match SH2's `shAttackInfo` struct very closely, though with reordered fields likely because
 * some PS2 compilers reorder structs to improve packing.
 * The SH2 `shAttackInfo` struct even has some unused fields (such as `sd` and `eff`, set to 0),
 * which is a good indication they were a holdover from SH1. The code in SH2 that uses this struct seems to have been
 * reworked into a kind of attack queue system for that game and doesn't resemble anything from SH1.
*/
typedef struct _AttackInfo
{
    /* 0x0  */ q4_12 rangeMax;            /** SH2 name: `max_range`. */
    /* 0x2  */ q3_12 rangeMin;            /** SH2 name: `min_range`. */
    /* 0x4  */ u16   damage;              /** SH2 name: `ap`. Game code converts to Q12 later. */
    /* 0x6  */ union
               {
                   s_AttackFight fight;
                   s_AttackShot  shot;
               } hitCheck;                /** SH2 name: `hit_check`. `shot` when `type` is `AttackInfoType_Gunshot`, otherwise `fight`. */
    /* 0xC  */ q4_12 shock;               /** SH2 name: `sp`. Scales the push applied to `s_CharaDamage::position`. */
    /* 0xE  */ u8    hitStartKeyframe;    /** SH2 name: `atk_start`. Keyframe into the attack animation where hit checks begin. */
    /* 0xF  */ u8    hitKeyframeCount;    /** SH2 name: `atk_end`. How many keyframes hit checks stay active for after `hitStartKeyframe`. 99: rest of the animation. */
    /* 0x10 */ u8    type;                /** `e_AttackInfoType` | SH2 name: `kind`. */
    /* 0x11 */ u8    hurtSfxType;         /** SH2 name: `sd`. Player hurt SFX type. TODO: Enum. */
    /* 0x12 */ u8    impactSfxGroup;      /** SH2 name: `eff`. Impact SFX per target, uses values 0-4. */
    /* 0x13 */ s8    __pad_13;
    /* 0x14 */ u32*  unk_14;              /** Some pointer. All `g_Items_WeaponAttackInfos` entries point to `D_800AD4C4`. */
} s_AttackInfo;

/** @brief Initializer for `s_AttackInfo::hitCheck` as a melee hit box.
 *
 * @param sizeX Hit box X size.
 * @param sizeY Hit box Y size.
 * @param sizeZ Hit box Z size.
 */
#define ATTACK_FIGHT(sizeX, sizeY, sizeZ) \
    { Q12(sizeX), Q12(sizeY), Q12(sizeZ) }

/** @brief Initializer for `s_AttackInfo::hitCheck` as gunshot parameters.
 * The union initializer fills `fight`, so the `s_AttackShot` fields are packed into its three halfwords.
 *
 * @param interval    Time between shots. 0: single shot. @unused Only weapon that sets this is Unk36, which could be a removed automatic-firing weapon?
 * @param maxShots    Shots per attack, 0 is treated as 1.
 * @param pelletCount Rays traced per shot.
 * @param inaccuracy  Aim error scale.
 * @param spreadIdx   `D_800AFD1C` index for the pellet spread angle (0-15).
 * @param spreadSteps Pellet spread step count (0-15).
 */
#define ATTACK_SHOT(interval, maxShots, pelletCount, inaccuracy, spreadIdx, spreadSteps) \
    { Q12(interval), (maxShots) | ((pelletCount) << 8), (inaccuracy) | ((spreadIdx) << 8) | ((spreadSteps) << 12) }

extern u8 g_Items_GunsMaxLoadAmmo[36]; // Max loaded ammo that a weapon can hold. 0x800AD4A0

extern s_AttackInfo g_Items_WeaponAttackInfos[70];

#endif
