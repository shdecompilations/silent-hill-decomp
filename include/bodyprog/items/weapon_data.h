#ifndef _BODYPROG_ITEMS_WEAPONDATA_H
#define _BODYPROG_ITEMS_WEAPONDATA_H

/** @brief Attack types. Packed with an `e_AttackInputType` by `WEAPON_ATTACK` to index into `g_Items_WeaponAttackInfos`.
 *
 * @note Attacks here are associated with the `e_InvItemId` enum. Game code adds 128 to attack types from player weapons
 * (0-7, 32-35, and 96-98) to find the `e_InvItemId` for a given attack.
 */
typedef enum _AttackType
{
    AttackType_KitchenKnife         = 0,
    AttackType_SteelPipe            = 1,
    AttackType_RockDrill            = 2,
    AttackType_Unk3                 = 3,  /** @unused Has stats, but InvItemId 131 has no ID and a blank name. Possibly "nightstick" mentioned in some protos. */
    AttackType_Hammer               = 4,
    AttackType_Chainsaw             = 5,
    AttackType_Katana               = 6,
    AttackType_Axe                  = 7,
    AttackType_Kick                 = 8,
    AttackType_Stomp                = 9,

    AttackType_Unk31                = 31, /** @unused Possibly another kind of fighting attack, like kick/stomp. Gets associated with those in `func_80071968_Switch0`. */
    AttackType_Handgun              = 32,
    AttackType_HuntingRifle         = 33,
    AttackType_Shotgun              = 34,
    AttackType_HyperBlaster         = 35,
    AttackType_Unk36                = 36, /** @unused Unused gun. Might be an automatic-firing gun based on having `interval` set in the attack info table. */

    AttackType_Unk40                = 40,
    AttackType_Unk41                = 41,
    AttackType_Unk42                = 42,
    AttackType_Groaner              = 43, /** Groaner jump attack. */
    AttackType_HangedScratcher0     = 44,
    AttackType_HangedScratcher1     = 45,
    AttackType_Unk46                = 46,
    AttackType_SplitHeadInstantKill = 47,
    AttackType_Stalker              = 48,
    AttackType_StalkerLegGrab       = 49,
    AttackType_Unk50                = 50,
    AttackType_LarvalStalkerGrab    = 51, /** @unused? Set from `LarvalStalkerControl_Unused5`. */
    AttackType_LarvalStalkerDrain   = 52, /** @unused? Damage while grabbed. Set from `LarvalStalkerControl_Unused5`. */
    AttackType_Creeper              = 53,
    AttackType_RomperGrab           = 54, /** Pins the player. */
    AttackType_PuppetNurseGrab      = 56, /** Torso grab. Used by Puppet Nurse and Puppet Doctor. */
    AttackType_PuppetNurse          = 57,
    AttackType_PuppetDoctor         = 58,
    AttackType_Floatstinger         = 59,
    AttackType_FloatstingerAcid     = 60,
    AttackType_Twinfeeler           = 61,
    AttackType_TwinfeelerAcid       = 62,
    AttackType_CybilHandgun         = 63, /** Used by Cybil and Monster Cybil. */
    AttackType_MonsterCybil0        = 64,
    AttackType_MonsterCybil1        = 65,
    AttackType_MonsterCybilNeckGrab = 66,
    AttackType_Unk67                = 67, /** Effect in the map7_s03 boss room. Damages on contact. */
    AttackType_Unknown23            = 68, /** `Chara_Unknown23` attack. Pushes the player away. */
    AttackType_Bloodsucker          = 69,
    AttackType_KaufmannHandgun      = 70  // Muzzle flash only, never hit-tested?
} e_AttackType;

/** @brief Attack info types. */
typedef enum _AttackInfoType
{
    AttackInfoType_Unk0      = 0,
    AttackInfoType_Melee     = 1, /** Melee hitting every frame. */
    AttackInfoType_Melee1Hit = 2, /** Melee stopping on first hit. */
    AttackInfoType_Gunshot   = 3,
    AttackInfoType_Enemy0    = 4, // } TODO: Better naming.
    AttackInfoType_Enemy1    = 5, // }
    AttackInfoType_Enemy2    = 6  // }
} e_AttackInfoType;

/** @brief Melee attack hitbox.
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
    /* 0x2 */ u8    shotCountMax;    /** 0 is treated as 1. */
    /* 0x3 */ u8    pelletCount; /** Rays trace per shot. */
    /* 0x4 */ u8    inaccuracy;
    /* 0x5 */ u8    spread;      /** Low nibble: `D_800AFD1C` index, high nibble: step count. */
} s_AttackShot;

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
    /* 0xF  */ u8    hitKeyframeCount;    /** SH2 name: `atk_end`. Number of keyframes for which hit checks stay active for after `hitStartKeyframe`. */
    /* 0x10 */ u8    type;                /** `e_AttackInfoType` | SH2 name: `kind`. */
    /* 0x11 */ u8    hurtSfxType;         /** SH2 name: `sd`. Player hurt SFX type. TODO: Enum. */
    /* 0x12 */ u8    impactSfxGroup;      /** SH2 name: `eff`. Impact SFX per target, uses values 0-4. */
    /* 0x13 */ s8    __pad_13;
    /* 0x14 */ u32*  unk_14;              /** Some pointer. All `g_Items_WeaponAttackInfos` entries point to `D_800AD4C4`. */
} s_AttackInfo;

extern u8 g_Items_GunsMaxLoadAmmo[36]; // Max loaded ammo that a weapon can hold. 0x800AD4A0

extern s_AttackInfo g_Items_WeaponAttackInfos[70];

/** @brief Initializer for `s_AttackInfo::hitCheck` as a melee hit box.
 *
 * @param sizeX X hitbox span.
 * @param sizeY Y hitbox span.
 * @param sizeZ Z hitbox span.
 */
#define ATTACK_FIGHT(sizeX, sizeY, sizeZ) \
    { Q12(sizeX), Q12(sizeY), Q12(sizeZ) }

/** @brief Initializer for `s_AttackInfo::hitCheck` as gunshot parameters.
 * The union initializer fills `fight`, so the `s_AttackShot` fields are packed into its three halfwords.
 *
 * @param interval Time between shots. 0: single shot. @unused Only weapon that sets this is Unk36, which could be a removed automatic-firing weapon?
 * @param shotsMax Shots per attack (0 is treated as 1).
 * @param pelletCount Rays traced per shot.
 * @param inaccuracy Aim error scale.
 * @param spreadIdx `D_800AFD1C` index for the pellet spread angle. Range: `[0, 15].
 * @param spreadSteps Pellet spread step count. Range: `[0, 15]`.
 */
#define ATTACK_SHOT(interval, shotsMax, pelletCount, inaccuracy, spreadIdx, spreadSteps) \
    { Q12(interval), (shotsMax) | ((pelletCount) << 8), (inaccuracy) | ((spreadIdx) << 8) | ((spreadSteps) << 12) }

/** @brief Packs a melee weapon attack containing a weapon ID and attack input type.
 *
 * @param weaponId Weapon ID.
 * @param attackInputType Attack input type.
 * @return Packed weapon attack containing a weapon ID and attack input type.
 */
#define WEAPON_ATTACK(weaponId, attackInputType) \
    ((weaponId) + ((attackInputType) * 10))

/** @brief Retrieves the melee weapon ID from a packed weapon attack.
 *
 * @param weaponAttack Packed weapon attack containing a weapon ID and attack input type.
 * @return Weapon ID.
 */
#define WEAPON_ATTACK_ID_GET(weaponAttack) \
    ((weaponAttack) % 10)

#endif
