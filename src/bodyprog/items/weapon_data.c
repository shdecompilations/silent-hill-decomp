#include "bodyprog/game.h"

#include "bodyprog/bodyprog.h"
#include "bodyprog/items/weapon_data.h"

u8 g_Items_GunsMaxLoadAmmo[36] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    15, 6, 6, NO_VALUE
};

static u32 D_800AD4C4 = 0;

/** @brief Weapon attack definitions, indexed by `WEAPON_ATTACK` value. 
 *
 * @note "Silent Hill Perfect Navigation Book" (JP) lists some weapon damage numbers that match these closely (Page 14+), but has some slight differences.
 * Unsure if there's any english-language guides with similar info.
 *
 * TODO: Identify `UnkXX` attacks, `HandgunBullets` / `RifleShells` / `ShotgunShells` may be misnamed.
*/
s_AttackInfo g_Items_WeaponAttackInfos[70] = {
/* Weapon attack            { `rangeMax`, `rangeMin`, `damage`, `hitCheck`,                           `shock`,    `hitStartKeyframe`, `hitKeyframeCount`, `type`,                   `hurtSfxType`, `impactSfxGroup`, `pad`, `unk_14`    } */
/* KitchenKnife          */ { Q12(0.4f),  Q12(0.1f),  130,      ATTACK_FIGHT(0.5f, 0.5f, 0.2f),       Q12(0.0f),  3,                  3,                  AttackInfoType_Melee,     -1,            2,                0,     &D_800AD4C4 },
/* SteelPipe             */ { Q12(1.0f),  Q12(-0.1f), 70,       ATTACK_FIGHT(0.4f, 0.5f, 0.3f),       Q12(1.0f),  7,                  4,                  AttackInfoType_Melee1Hit, -1,            3,                0,     &D_800AD4C4 },
/* RockDrill             */ { Q12(1.0f),  Q12(0.6f),  200,      ATTACK_FIGHT(0.5f, 0.5f, 0.3f),       Q12(1.0f),  4,                  6,                  AttackInfoType_Melee,     -1,            1,                0,     &D_800AD4C4 },
/* Unk3                  */ { Q12(0.8f),  Q12(0.3f),  15,       ATTACK_FIGHT(0.0f, 0.0f, 0.0f),       Q12(0.5f),  13,                 7,                  AttackInfoType_Melee1Hit, -1,            2,                0,     &D_800AD4C4 },
/* Hammer                */ { Q12(1.2f),  Q12(0.2f),  280,      ATTACK_FIGHT(0.4f, 0.5f, 0.3f),       Q12(1.0f),  7,                  4,                  AttackInfoType_Melee1Hit, -1,            3,                0,     &D_800AD4C4 },
/* Chainsaw              */ { Q12(1.0f),  Q12(0.1f),  250,      ATTACK_FIGHT(0.5f, 0.5f, 0.4f),       Q12(0.25f), 6,                  5,                  AttackInfoType_Melee,     -1,            2,                0,     &D_800AD4C4 },
/* Katana                */ { Q12(1.2f),  Q12(0.1f),  1000,     ATTACK_FIGHT(0.4f, 0.4f, 0.4f),       Q12(0.0f),  0,                  6,                  AttackInfoType_Melee,     -1,            2,                0,     &D_800AD4C4 },
/* Axe                   */ { Q12(0.4f),  Q12(0.4f),  240,      ATTACK_FIGHT(0.3f, 0.4f, 0.5f),       Q12(0.0f),  6,                  3,                  AttackInfoType_Melee,     -1,            2,                0,     &D_800AD4C4 },
/* Kick                  */ { Q12(0.5f),  Q12(0.0f),  100,      ATTACK_FIGHT(0.2f, 0.3f, 0.4f),       Q12(0.25f), 13,                 7,                  AttackInfoType_Melee1Hit, -1,            2,                0,     &D_800AD4C4 },
/* Stomp                 */ { Q12(0.1f),  Q12(0.0f),  100,      ATTACK_FIGHT(0.2f, 0.3f, 0.4f),       Q12(0.0f),  8,                  5,                  AttackInfoType_Melee1Hit, -1,            3,                0,     &D_800AD4C4 },
/* KitchenKnife Hold     */ { Q12(0.5f),  Q12(0.2f),  120,      ATTACK_FIGHT(0.3f, 0.3f, 0.2f),       Q12(0.0f),  0,                  4,                  AttackInfoType_Melee,     -1,            2,                0,     &D_800AD4C4 },
/* SteelPipe Hold        */ { Q12(1.0f),  Q12(-0.1f), 90,       ATTACK_FIGHT(0.4f, 0.2f, 0.3f),       Q12(1.0f),  12,                 6,                  AttackInfoType_Melee1Hit, -1,            4,                0,     &D_800AD4C4 },
/* RockDrill Hold        */ { Q12(1.0f),  Q12(0.5f),  20,       ATTACK_FIGHT(0.4f, 0.4f, 0.4f),       Q12(1.0f),  4,                  6,                  AttackInfoType_Melee,     -1,            1,                0,     &D_800AD4C4 },
/* Unk3 Hold             */ { Q12(0.8f),  Q12(0.3f),  15,       ATTACK_FIGHT(0.0f, 0.0f, 0.0f),       Q12(0.5f),  13,                 7,                  AttackInfoType_Melee1Hit, -1,            3,                0,     &D_800AD4C4 },
/* Hammer Hold           */ { Q12(1.2f),  Q12(0.2f),  360,      ATTACK_FIGHT(0.4f, 0.2f, 0.3f),       Q12(1.25f), 12,                 6,                  AttackInfoType_Melee1Hit, -1,            4,                0,     &D_800AD4C4 },
/* Chainsaw Hold         */ { Q12(1.0f),  Q12(0.1f),  250,      ATTACK_FIGHT(0.5f, 0.5f, 0.4f),       Q12(0.25f), 6,                  9,                  AttackInfoType_Melee,     -1,            2,                0,     &D_800AD4C4 },
/* Katana Hold           */ { Q12(1.2f),  Q12(0.1f),  1000,     ATTACK_FIGHT(0.4f, 0.4f, 0.4f),       Q12(0.0f),  7,                  6,                  AttackInfoType_Melee,     -1,            2,                0,     &D_800AD4C4 },
/* Axe Hold              */ { Q12(0.4f),  Q12(0.4f),  270,      ATTACK_FIGHT(0.4f, 0.4f, 0.5f),       Q12(0.0f),  6,                  4,                  AttackInfoType_Melee,     -1,            2,                0,     &D_800AD4C4 },
/* Kick Hold             */ { Q12(0.0f),  Q12(0.0f),  0,        ATTACK_FIGHT(0.0f, 0.0f, 0.0f),       Q12(0.0f),  0,                  0,                  AttackInfoType_Unk0,      -1,            0,                0,     &D_800AD4C4 },
/* Stomp Hold            */ { Q12(0.0f),  Q12(0.0f),  0,        ATTACK_FIGHT(0.0f, 0.0f, 0.0f),       Q12(0.0f),  0,                  0,                  AttackInfoType_Unk0,      -1,            0,                0,     &D_800AD4C4 },
/* KitchenKnife Multitap */ { Q12(0.4f),  Q12(0.1f),  130,      ATTACK_FIGHT(0.4f, 0.4f, 0.2f),       Q12(0.0f),  1,                  4,                  AttackInfoType_Melee,     -1,            2,                0,     &D_800AD4C4 },
/* SteelPipe Multitap    */ { Q12(1.0f),  Q12(-0.1f), 50,       ATTACK_FIGHT(0.4f, 0.5f, 0.3f),       Q12(1.0f),  5,                  3,                  AttackInfoType_Melee1Hit, -1,            3,                0,     &D_800AD4C4 },
/* RockDrill Multitap    */ { Q12(1.0f),  Q12(0.5f),  16,       ATTACK_FIGHT(0.4f, 0.4f, 0.4f),       Q12(0.75f), 4,                  6,                  AttackInfoType_Melee,     -1,            1,                0,     &D_800AD4C4 },
/* Unk3 Multitap         */ { Q12(0.8f),  Q12(0.3f),  12,       ATTACK_FIGHT(0.0f, 0.0f, 0.0f),       Q12(0.25f), 4,                  6,                  AttackInfoType_Melee1Hit, -1,            2,                0,     &D_800AD4C4 },
/* Hammer Multitap       */ { Q12(1.2f),  Q12(0.2f),  200,      ATTACK_FIGHT(0.4f, 0.5f, 0.3f),       Q12(1.0f),  4,                  4,                  AttackInfoType_Melee1Hit, -1,            3,                0,     &D_800AD4C4 },
/* Chainsaw Multitap     */ { Q12(1.0f),  Q12(0.1f),  250,      ATTACK_FIGHT(0.5f, 0.5f, 0.4f),       Q12(0.25f), 4,                  5,                  AttackInfoType_Melee,     -1,            2,                0,     &D_800AD4C4 },
/* Katana Multitap       */ { Q12(1.2f),  Q12(0.1f),  1000,     ATTACK_FIGHT(0.4f, 0.4f, 0.4f),       Q12(0.0f),  2,                  5,                  AttackInfoType_Melee,     -1,            2,                0,     &D_800AD4C4 },
/* Axe Multitap          */ { Q12(0.4f),  Q12(0.4f),  160,      ATTACK_FIGHT(0.3f, 0.4f, 0.5f),       Q12(0.0f),  2,                  4,                  AttackInfoType_Melee,     -1,            2,                0,     &D_800AD4C4 },
/* Kick Multitap         */ { Q12(0.5f),  Q12(0.3f),  1,        ATTACK_FIGHT(0.0f, 0.0f, 0.0f),       Q12(0.0f),  0,                  5,                  AttackInfoType_Enemy2,    -1,            0,                0,     &D_800AD4C4 },
/* Stomp Multitap        */ { Q12(0.5f),  Q12(0.3f),  1,        ATTACK_FIGHT(0.0f, 0.0f, 0.0f),       Q12(0.0f),  0,                  5,                  AttackInfoType_Enemy2,    -1,            0,                0,     &D_800AD4C4 },
/* Unk30                 */ { Q12(0.5f),  Q12(0.3f),  1,        ATTACK_FIGHT(0.0f, 0.0f, 0.0f),       Q12(0.0f),  0,                  5,                  AttackInfoType_Enemy2,    -1,            0,                0,     &D_800AD4C4 },
/* Unk31                 */ { Q12(0.0f),  Q12(0.0f),  0,        ATTACK_FIGHT(0.0f, 0.0f, 0.0f),       Q12(0.0f),  0,                  0,                  AttackInfoType_Unk0,      0,             0,                0,     &D_800AD4C4 },
/* Handgun               */ { Q12(12.0f), Q12(0.0f),  100,      ATTACK_SHOT(0.0f, 0, 1,  68,  0,  0), Q12(0.0f),  0,                  5,                  AttackInfoType_Gunshot,   0,             3,                0,     &D_800AD4C4 },
/* HuntingRifle          */ { Q12(15.0f), Q12(0.0f),  350,      ATTACK_SHOT(0.0f, 0, 1,  34,  0,  0), Q12(0.5f),  0,                  5,                  AttackInfoType_Gunshot,   0,             3,                0,     &D_800AD4C4 },
/* Shotgun               */ { Q12(12.0f), Q12(0.0f),  50,       ATTACK_SHOT(0.0f, 0, 12, 170, 12, 5), Q12(0.25f), 0,                  5,                  AttackInfoType_Gunshot,   0,             2,                0,     &D_800AD4C4 },
/* HyperBlaster          */ { Q12(15.0f), Q12(0.0f),  50,       ATTACK_SHOT(0.0f, 0, 1,  0,   0,  0), Q12(0.0f),  0,                  3,                  AttackInfoType_Gunshot,   0,             2,                0,     &D_800AD4C4 },
/* Unk36                 */ { Q12(12.0f), Q12(0.0f),  40,       ATTACK_SHOT(0.2f, 0, 4,  147, 9,  4), Q12(0.0f),  2,                  5,                  AttackInfoType_Gunshot,   0,             2,                0,     &D_800AD4C4 },
/* Unk37                 */ { Q12(0.0f),  Q12(0.0f),  0,        ATTACK_SHOT(0.0f, 0, 1,  0,   0,  0), Q12(0.0f),  0,                  0,                  AttackInfoType_Unk0,      0,             0,                0,     &D_800AD4C4 },
/* Unk38                 */ { Q12(0.0f),  Q12(0.0f),  0,        ATTACK_SHOT(0.0f, 0, 1,  0,   0,  0), Q12(0.0f),  0,                  0,                  AttackInfoType_Unk0,      0,             0,                0,     &D_800AD4C4 },
/* Unk39                 */ { Q12(0.0f),  Q12(0.0f),  0,        ATTACK_SHOT(0.0f, 1, 1,  0,   0,  0), Q12(0.0f),  0,                  0,                  AttackInfoType_Unk0,      0,             0,                0,     &D_800AD4C4 },
/* Unk40                 */ { Q12(0.3f),  Q12(0.0f),  22,       ATTACK_FIGHT(0.0f, 0.0f, 0.0f),       Q12(0.0f),  2,                  7,                  AttackInfoType_Melee1Hit, 5,             4,                0,     &D_800AD4C4 },
/* Unk41                 */ { Q12(0.3f),  Q12(0.0f),  18,       ATTACK_FIGHT(0.0f, 0.0f, 0.0f),       Q12(0.0f),  2,                  7,                  AttackInfoType_Melee1Hit, 5,             4,                0,     &D_800AD4C4 },
/* Unk42                 */ { Q12(0.3f),  Q12(0.0f),  10,       ATTACK_FIGHT(0.2f, 0.3f, 0.2f),       Q12(0.0f),  10,                 8,                  AttackInfoType_Melee1Hit, 2,             3,                0,     &D_800AD4C4 },
/* Unk43                 */ { Q12(0.4f),  Q12(0.1f),  10,       ATTACK_FIGHT(0.3f, 0.3f, 0.0f),       Q12(0.25f), 8,                  2,                  AttackInfoType_Melee1Hit, 2,             2,                0,     &D_800AD4C4 },
/* Unk44                 */ { Q12(0.4f),  Q12(0.1f),  16,       ATTACK_FIGHT(0.0f, 0.0f, 0.0f),       Q12(0.0f),  0,                  99,                 AttackInfoType_Enemy2,    5,             3,                0,     &D_800AD4C4 },
/* Unk45                 */ { Q12(1.3f),  Q12(0.4f),  16,       ATTACK_FIGHT(0.4f, 0.2f, 0.0f),       Q12(0.0f),  8,                  7,                  AttackInfoType_Melee1Hit, 2,             4,                0,     &D_800AD4C4 },
/* Unk46                 */ { Q12(0.0f),  Q12(0.0f),  0,        ATTACK_FIGHT(0.0f, 0.0f, 0.0f),       Q12(0.0f),  0,                  99,                 AttackInfoType_Melee1Hit, 0,             0,                0,     &D_800AD4C4 },
/* Unk47                 */ { Q12(0.0f),  Q12(0.0f),  0,        ATTACK_FIGHT(0.0f, 0.0f, 0.0f),       Q12(0.0f),  0,                  99,                 AttackInfoType_Enemy0,    0,             0,                0,     &D_800AD4C4 },
/* Unk48                 */ { Q12(0.9f),  Q12(0.7f),  15,       ATTACK_FIGHT(0.2f, 0.2f, 0.0f),       Q12(0.0f),  12,                 3,                  AttackInfoType_Melee,     2,             2,                0,     &D_800AD4C4 },
/* Unk49                 */ { Q12(0.6f),  Q12(0.3f),  0,        ATTACK_FIGHT(0.4f, 0.4f, 0.0f),       Q12(0.0f),  11,                 5,                  AttackInfoType_Enemy1,    5,             0,                0,     &D_800AD4C4 },
/* Unk50                 */ { Q12(0.0f),  Q12(0.0f),  12,       ATTACK_FIGHT(0.0f, 0.0f, 0.0f),       Q12(0.0f),  0,                  99,                 AttackInfoType_Enemy0,    0,             2,                0,     &D_800AD4C4 },
/* Unk51                 */ { Q12(0.4f),  Q12(0.0f),  0,        ATTACK_FIGHT(0.3f, 0.3f, 0.0f),       Q12(0.0f),  0,                  12,                 AttackInfoType_Enemy1,    0,             0,                0,     &D_800AD4C4 },
/* Unk52                 */ { Q12(0.5f),  Q12(0.0f),  5,        ATTACK_FIGHT(0.0f, 0.0f, 0.0f),       Q12(0.0f),  0,                  99,                 AttackInfoType_Enemy0,    0,             2,                0,     &D_800AD4C4 },
/* Unk53                 */ { Q12(0.4f),  Q12(0.2f),  6,        ATTACK_FIGHT(0.1f, 0.1f, 0.0f),       Q12(0.0f),  5,                  2,                  AttackInfoType_Enemy0,    5,             2,                0,     &D_800AD4C4 },
/* Unk54                 */ { Q12(0.9f),  Q12(0.4f),  0,        ATTACK_FIGHT(0.4f, 0.6f, 0.0f),       Q12(0.0f),  3,                  2,                  AttackInfoType_Enemy1,    1,             0,                0,     &D_800AD4C4 },
/* Unk55                 */ { Q12(0.0f),  Q12(0.0f),  10,       ATTACK_FIGHT(0.0f, 0.0f, 0.0f),       Q12(0.0f),  0,                  99,                 AttackInfoType_Enemy0,    2,             0,                0,     &D_800AD4C4 },
/* Unk56                 */ { Q12(0.0f),  Q12(0.0f),  0,        ATTACK_FIGHT(0.2f, 0.2f, 0.2f),       Q12(0.0f),  0,                  99,                 AttackInfoType_Enemy1,    5,             0,                0,     &D_800AD4C4 },
/* Unk57                 */ { Q12(0.0f),  Q12(0.0f),  25,       ATTACK_FIGHT(0.2f, 0.2f, 0.2f),       Q12(0.25f), 17,                 5,                  AttackInfoType_Melee,     2,             2,                0,     &D_800AD4C4 },
/* Unk58                 */ { Q12(0.0f),  Q12(0.0f),  33,       ATTACK_FIGHT(0.2f, 0.2f, 0.2f),       Q12(0.25f), 17,                 5,                  AttackInfoType_Melee,     2,             2,                0,     &D_800AD4C4 },
/* Unk59                 */ { Q12(0.2f),  Q12(0.0f),  18,       ATTACK_FIGHT(0.2f, 0.2f, 0.0f),       Q12(0.0f),  0,                  99,                 AttackInfoType_Melee1Hit, 2,             4,                0,     &D_800AD4C4 },
/* Unk60                 */ { Q12(0.0f),  Q12(0.0f),  0,        ATTACK_FIGHT(0.0f, 0.0f, 0.0f),       Q12(0.0f),  0,                  99,                 AttackInfoType_Enemy2,    4,             0,                0,     &D_800AD4C4 },
/* Unk61                 */ { Q12(0.4f),  Q12(0.4f),  10,       ATTACK_FIGHT(0.6f, 0.6f, 0.6f),       Q12(1.75f), 0,                  99,                 AttackInfoType_Melee1Hit, 5,             0,                0,     &D_800AD4C4 },
/* Unk62                 */ { Q12(0.0f),  Q12(0.0f),  0,        ATTACK_FIGHT(0.0f, 0.0f, 0.0f),       Q12(0.0f),  0,                  99,                 AttackInfoType_Enemy2,    4,             0,                0,     &D_800AD4C4 },
/* Unk63                 */ { Q12(15.0f), Q12(0.0f),  60,       ATTACK_SHOT(0.0f, 0, 1, 39, 0, 0),    Q12(0.25f), 1,                  2,                  AttackInfoType_Gunshot,   4,             3,                0,     &D_800AD4C4 },
/* HandgunBullets        */ { Q12(0.4f),  Q12(0.0f),  15,       ATTACK_FIGHT(0.0f, 0.0f, 0.0f),       Q12(0.25f), 11,                 3,                  AttackInfoType_Melee1Hit, 2,             0,                0,     &D_800AD4C4 },
/* RifleShells           */ { Q12(0.5f),  Q12(0.0f),  25,       ATTACK_FIGHT(0.0f, 0.0f, 0.0f),       Q12(0.25f), 11,                 3,                  AttackInfoType_Melee1Hit, 1,             0,                0,     &D_800AD4C4 },
/* ShotgunShells         */ { Q12(0.4f),  Q12(0.0f),  1,        ATTACK_FIGHT(0.4f, 0.4f, 0.4f),       Q12(0.0f),  0,                  99,                 AttackInfoType_Enemy1,    5,             0,                0,     &D_800AD4C4 },
/* Unk67                 */ { Q12(0.0f),  Q12(0.0f),  0,        ATTACK_FIGHT(0.0f, 0.0f, 0.0f),       Q12(0.0f),  0,                  99,                 AttackInfoType_Enemy2,    4,             0,                0,     &D_800AD4C4 },
/* Unk68                 */ { Q12(0.0f),  Q12(0.0f),  0,        ATTACK_FIGHT(0.0f, 0.0f, 0.0f),       Q12(0.0f),  0,                  99,                 AttackInfoType_Enemy2,    1,             0,                0,     &D_800AD4C4 },
/* Unk69                 */ { Q12(0.1f),  Q12(0.0f),  6,        ATTACK_FIGHT(0.1f, 0.1f, 0.0f),       Q12(0.0f),  0,                  99,                 AttackInfoType_Enemy2,    2,             2,                0,     &D_800AD4C4 },
};
