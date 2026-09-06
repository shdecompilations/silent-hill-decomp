#ifndef _BODYPROG_ITEMS_H
#define _BODYPROG_ITEMS_H

#define INV_ITEM_COUNT_MAX  40
#define INV_ITEM_GROUP_SIZE 32 /** Number of `e_InvItemId`s per `e_InvItemGroup`. */

#define DEFAULT_PICKUP_ITEM_COUNT      1
#define HANDGUN_AMMO_PICKUP_ITEM_COUNT 15
#define SHOTGUN_AMMO_PICKUP_ITEM_COUNT 6
#define RIFLE_AMMO_PICKUP_ITEM_COUNT   6

/** @brief Inventory item model load flags. */
typedef enum _InvItemLoadFlags
{
    InvItemLoadFlag_None     = 0,
    InvItemLoadFlag_Texture0 = 1 << 0,
    InvItemLoadFlag_Texture1 = 1 << 1,
    InvItemLoadFlag_Texture2 = 1 << 2,
    InvItemLoadFlag_Texture3 = 1 << 3,
    InvItemLoadFlag_Texture4 = 1 << 4,
    InvItemLoadFlag_Texture5 = 1 << 5,
    InvItemLoadFlag_Texture6 = 1 << 6,
    InvItemLoadFlag_Model    = 1 << 7
} e_InvItemLoadFlags;

/** @brief Inventory command IDs. */
typedef enum _InvCmdId
{
    InvCmdId_UseHealth     = 0, /** Text is "Use", but this one is used exclusively on health items. */
    InvCmdId_Use           = 1,
    InvCmdId_Equip         = 2,
    InvCmdId_Unequip       = 3,
    InvCmdId_EquipReload   = 4,
    InvCmdId_UnequipReload = 5,
    InvCmdId_OnOff         = 6,
    InvCmdId_Reload        = 7,
    InvCmdId_Look          = 8,
    InvCmdId_UseLook       = 9,
    InvCmdId_Unk10         = 10,
    InvCmdId_Unk11         = 11 // Flashlight in daytime?
} e_InvCmdId;

/** @brief Inventory item groups. Every 32nd item ID is treated as a separate group by some code. */
typedef enum _InvItemGroup
{
    InvItemGroup_None          = 0,
    InvItemGroup_HealthItems   = 1,
    InvItemGroup_Keys          = 2,
    InvItemGroup_PuzzleItems   = 3,
    InvItemGroup_MeleeWeapons  = 4,
    InvItemGroup_GunWeapons    = 5,
    InvItemGroup_GunAmmo       = 6,
    InvItemGroup_PortableItems = 7
} e_InvItemGroup;

/** @brief Inventory item IDs. */
typedef enum _InvItemId
{
    // Group 0 (None)
    InvItemId_Empty                 = NO_VALUE,
    InvItemId_Unequipped            = 0,

    // Group 1 (Health Items)
    InvItemId_HealthDrink           = 32,
    InvItemId_FirstAidKit           = 33,
    InvItemId_Ampoule               = 34,

    // Group 2 (Keys)
    InvItemId_LobbyKey              = 64,
    InvItemId_HouseKey              = 65,
    InvItemId_KeyOfLion             = 66,
    InvItemId_KeyOfWoodman          = 67,
    InvItemId_KeyOfScarecrow        = 68,
    InvItemId_LibraryReserveKey     = 69,
    InvItemId_ClassroomKey          = 70,
    InvItemId_KGordonKey            = 71,
    InvItemId_DrawbridgeKey         = 72,
    InvItemId_BasementKey           = 73,
    InvItemId_BasementStoreroomKey  = 74,
    InvItemId_ExaminationRoomKey    = 75,
    InvItemId_AntiqueShopKey        = 76,
    InvItemId_SewerKey              = 77,
    InvItemId_KeyOfOphiel           = 78,
    InvItemId_KeyOfHagith           = 79,
    InvItemId_KeyOfPhaleg           = 80,
    InvItemId_KeyOfBethor           = 81,
    InvItemId_KeyOfAratron          = 82,
    InvItemId_NoteToSchool          = 83,
    InvItemId_NoteDoghouse          = 84,
    InvItemId_PictureCard           = 85,

    InvItemId_SewerExitKey          = 87,
    InvItemId_ChannelingStone       = 88,

    // Group 3 (Puzzle Items)
    InvItemId_Chemical              = 96,
    InvItemId_GoldMedallion         = 97,
    InvItemId_SilverMedallion       = 98,
    InvItemId_RubberBall            = 99,
    InvItemId_Flauros               = 100,
    InvItemId_PlasticBottle         = 101,
    InvItemId_UnknownLiquid         = 102,
    InvItemId_PlateOfTurtle         = 103,
    InvItemId_PlateOfHatter         = 104,
    InvItemId_PlateOfCat            = 105,
    InvItemId_PlateOfQueen          = 106,
    InvItemId_BloodPack             = 107,
    InvItemId_DisinfectingAlcohol   = 108,
    InvItemId_Lighter               = 109,
    InvItemId_VideoTape             = 110,

    InvItemId_KaufmannKey           = 112,
    InvItemId_Receipt               = 113,
    InvItemId_SafeKey               = 114,
    InvItemId_Magnet                = 115,
    InvItemId_MotorcycleKey         = 116,
    InvItemId_BirdCageKey           = 117,
    InvItemId_Pliers                = 118,
    InvItemId_Screwdriver           = 119,
    InvItemId_Camera                = 120,
    InvItemId_RingOfContract        = 121,
    InvItemId_StoneOfTime           = 122,
    InvItemId_AmuletOfSolomon       = 123,
    InvItemId_CrestOfMercury        = 124,
    InvItemId_Ankh                  = 125,
    InvItemId_DaggerOfMelchior      = 126,
    InvItemId_DiskOfOuroboros       = 127,

    // Group 4 (Melee Weapons)
    InvItemId_KitchenKnife          = 128,
    InvItemId_SteelPipe             = 129,
    InvItemId_RockDrill             = 130,

    InvItemId_Hammer                = 132,
    InvItemId_Chainsaw              = 133,
    InvItemId_Katana                = 134,
    InvItemId_Axe                   = 135,

    // Group 5 (Guns)
    InvItemId_Handgun               = 160,
    InvItemId_HuntingRifle          = 161,
    InvItemId_Shotgun               = 162,
    InvItemId_HyperBlaster          = 163,

    InvItemId_CutscenePhone         = 164,
    InvItemId_CutsceneFlauros       = 165,
    InvItemId_CutsceneAglaophotis   = 166,
    InvItemId_CutscenePlasticBottle = 167,
    InvItemId_CutsceneBaby          = 168,
    InvItemId_CutsceneBloodPack     = 169,

    // Group 6 (Gun Ammo)
    InvItemId_HandgunBullets        = 192,
    InvItemId_RifleShells           = 193,
    InvItemId_ShotgunShells         = 194,

    // Group 7 (Portable Items)
    InvItemId_Flashlight            = 224,
    InvItemId_PocketRadio           = 225,
    InvItemId_GasolineTank          = 226
} e_InvItemId;

/** @brief Common pickup item IDs. */
typedef enum _CommonPickupItemId
{
    CommonPickupItemId_FirstAidKit    = 0,
    CommonPickupItemId_HealthDrink    = 1,
    CommonPickupItemId_Ampoule        = 2,
    CommonPickupItemId_HandgunBullets = 3,
    CommonPickupItemId_RifleShells    = 4,
    CommonPickupItemId_ShotgunShells  = 5
} e_CommonPickupItemId;

/** @brief Equipped weapon IDs. Derivative of `e_InvItemId`.
 *
 * TODO: Maybe just "Weapon ID", "equipable item ID", "[something else] item ID"?
 */
typedef enum _EquippedWeaponId
{
    EquippedWeaponId_KitchenKnife   = 0,
    EquippedWeaponId_SteelPipe      = 1,
    EquippedWeaponId_RockDrill      = 2,
    EquippedWeaponId_Unk3           = 3,
    EquippedWeaponId_Hammer         = 4,
    EquippedWeaponId_Chainsaw       = 5,
    EquippedWeaponId_Katana         = 6,
    EquippedWeaponId_Axe            = 7,
    EquippedWeaponId_Kick           = 8,
    EquippedWeaponId_Stomp          = 9,

    EquippedWeaponId_Unk31          = 31, // Larval Stalker attack.
    EquippedWeaponId_Handgun        = 32,
    EquippedWeaponId_HuntingRifle   = 33,
    EquippedWeaponId_Shotgun        = 34,
    EquippedWeaponId_HyperBlaster   = 35,

    EquippedWeaponId_Unk37          = 37, // Split Head attack.

    EquippedWeaponId_Unk44          = 44, // } HangedScratcher attack.
    EquippedWeaponId_Unk45          = 45, // }

    EquippedWeaponId_Unk48          = 48, // } Stalker attack.
    EquippedWeaponId_Unk49          = 49, // }

    EquippedWeaponId_Unk56          = 56, // Puppet Nurse attack.

    EquippedWeaponId_Unk59          = 59, // Float Stinger attack.

    EquippedWeaponId_Unk61          = 61, // Twinfeeler attack.

    EquippedWeaponId_Unk63          = 63, // Cybil or Monster Cybil attack.

    EquippedWeaponId_HandgunBullets = 64, // Monster Cybil attack?
    EquippedWeaponId_RifleShells    = 65, // Monster Cybil attack?
    EquippedWeaponId_ShotgunShells  = 66, // Monster Cybil attack?

    EquippedWeaponId_Unk69          = 69, // Bloodsucker attack.
    EquippedWeaponId_Unk70          = 70, // Kaufmann attack on Dahlia?

    EquippedWeaponId_Flashlight     = 96,
    EquippedWeaponId_PocketRadio    = 97,
    EquippedWeaponId_GasolineTank   = 98
} e_EquippedWeaponId;

/** @brief Special inventory item toggle flags. */
typedef enum _ItemToggleFlags
{
    ItemToggleFlag_RadioOn       = 1 << 0,
    ItemToggleFlag_FlashlightOff = 1 << 1
} e_ItemToggleFlags;

/** @brief Inventory item entry. */
typedef struct _InventoryItem
{
    /* 0x0 */ u8 id;      /** `e_InvItemId` */
    /* 0x1 */ u8 count;
    /* 0x2 */ u8 command; /** `e_InvCmdId` */
    /* 0x3 */ u8 field_3; // Some sort of index?
} s_InventoryItem;
STATIC_ASSERT_SIZEOF(s_InventoryItem, 4);

/** @brief Gets the `e_InvItemGroup` for an `e_InvItemId`.
 * Divides the item ID by 32 (`INV_ITEM_GROUP_SIZE`), using a bitwise shift to match.
 *
 * @param itemId Item ID to process.
 * @return Inventory item group.
 */
#define INV_ITEM_GROUP(itemId) \
    ((itemId) >> 5)

/** @brief Gets the index of an `e_InvItemId` inside the group it belongs to.
 * Modulos the item ID by 32 (`INV_ITEM_GROUP_SIZE`), using AND to match.
 * E.g. `itemId` 65 would be index 1, group 2.
 *
 * @param itemId Item ID to process.
 * @return Inventory item group.
 */
#define INV_ITEM_GROUP_ID(itemId) \
    ((itemId) & 0x1F)

/** @brief Gets the `e_InvItemId` of the ammo for a given weapon item.
 *
 * @param itemId Weapon item ID to process.
 * @return Inventory item group.
 */
#define INV_WEAPON_AMMO_ID(weaponId) \
    ((weaponId) + INV_ITEM_GROUP_SIZE)

/** @brief Gets the `e_InvItemId` of the weapon for a given ammo item.
 *
 * @param ammoId Ammo ID to process.
 * @return Inventory item group.
 */
#define INV_AMMO_WEAPON_ID(ammoId) \
    ((ammoId) - INV_ITEM_GROUP_SIZE)

#endif
