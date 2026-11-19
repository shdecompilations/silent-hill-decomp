#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"

s_AnimInfo HARRY_M2S04_ANIM_INFOS[] = {
    { Anim_BlendLinear, ANIM_STATUS(38, false), false, ANIM_STATUS(38, true), { Q12(10.0f) }, NO_VALUE, 676 },
    { Anim_PlayOnce, ANIM_STATUS(38, true), false, ANIM_STATUS(38, true), { Q12(10.0f) }, 676, 677 },
    { Anim_BlendLinear, ANIM_STATUS(39, false), false, ANIM_STATUS(39, true), { Q12(10.0f) }, NO_VALUE, 678 },
    { Anim_PlayOnce, ANIM_STATUS(39, true), false, ANIM_STATUS(39, true), { Q12(15.0f) }, 678, 688 },
    { Anim_BlendLinear, ANIM_STATUS(40, false), false, ANIM_STATUS(40, true), { Q12(10.0f) }, NO_VALUE, 689 },
    { Anim_PlayOnce, ANIM_STATUS(40, true), false, ANIM_STATUS(40, true), { Q12(10.0f) }, 689, 724 },
    { Anim_BlendLinear, ANIM_STATUS(41, false), false, ANIM_STATUS(41, true), { Q12(10.0f) }, NO_VALUE, 729 },
    { Anim_PlayOnce, ANIM_STATUS(41, true), false, ANIM_STATUS(41, true), { Q12(15.0f) }, 729, 743 },
    { Anim_BlendLinear, ANIM_STATUS(42, false), false, ANIM_STATUS(42, true), { Q12(10.0f) }, NO_VALUE, 748 },
    { Anim_PlayOnce, ANIM_STATUS(42, true), false, ANIM_STATUS(42, true), { Q12(15.0f) }, 748, 762 },
    { Anim_BlendLinear, ANIM_STATUS(43, false), false, ANIM_STATUS(43, true), { Q12(10.0f) }, NO_VALUE, 769 },
    { Anim_PlayOnce, ANIM_STATUS(43, true), false, ANIM_STATUS(43, true), { Q12(10.0f) }, 769, 781 },
    { Anim_BlendLinear, ANIM_STATUS(44, false), false, ANIM_STATUS(44, true), { Q12(10.0f) }, NO_VALUE, 788 },
    { Anim_PlayOnce, ANIM_STATUS(44, true), false, ANIM_STATUS(44, true), { Q12(10.0f) }, 788, 800 },
    { Anim_BlendLinear, ANIM_STATUS(45, false), false, ANIM_STATUS(45, true), { Q12(10.0f) }, NO_VALUE, 485 },
    { Anim_PlayOnce, ANIM_STATUS(45, true), false, ANIM_STATUS(45, true), { Q12(20.0f) }, 485, 502 },
    {}
};

s_UnkStruct3_Mo g_MapHeaderTable_38[] = {
    { 19788, 0, 676, 677 },
    { 20302, 200, 678, 688 },
    { 20816, 202, 689, 724 },
    { 21330, 210, 725, 743 },
    { 21844, 211, 744, 762 },
    { 22358, 212, 763, 781 },
    { 22872, 213, 782, 800 },
    { 23386, 500, 485, 502 },
    {}
};

u8 LOADABLE_INVENTORY_ITEMS[] = {
    InvItemId_HealthDrink,
    InvItemId_FirstAidKit,
    InvItemId_Ampoule,
    InvItemId_KitchenKnife,
    InvItemId_SteelPipe,
    InvItemId_Hammer,
    InvItemId_Chainsaw,
    InvItemId_Axe,
    InvItemId_Handgun,
    InvItemId_HuntingRifle,
    InvItemId_Shotgun,
    InvItemId_HyperBlaster,
    InvItemId_HandgunBullets,
    InvItemId_RifleShells,
    InvItemId_ShotgunShells,
    InvItemId_Flashlight,
    InvItemId_PocketRadio,
    InvItemId_HouseKey,
    InvItemId_KeyOfLion,
    InvItemId_KeyOfWoodman,
    InvItemId_KeyOfScarecrow,
    InvItemId_KGordonKey,
    InvItemId_DrawbridgeKey,
    InvItemId_NoteToSchool,
    InvItemId_NoteDoghouse,
    InvItemId_Flauros,
    InvItemId_AntiqueShopKey,
    InvItemId_PlasticBottle,
    InvItemId_UnknownLiquid,
    InvItemId_VideoTape,
    InvItemId_RockDrill,
    InvItemId_GasolineTank,
    InvItemId_ChannelingStone,
    InvItemId_Katana,
    InvItemId_Unequipped
};

u8 sharedData_800DD584_0_s00 = 0;

q19_12 sharedData_800DD598_0_s00 = Q12_ANGLE(0.0f);

u8 g_Player_PrevWeaponAttack = 255;
