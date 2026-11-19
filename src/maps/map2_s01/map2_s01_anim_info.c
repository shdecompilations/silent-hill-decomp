#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"

s_AnimInfo HARRY_M2S01_ANIM_INFOS[] = {
    { Anim_BlendLinear, ANIM_STATUS(38, false), false, ANIM_STATUS(38, true), { Q12(10.0f) }, NO_VALUE, 676 },
    { Anim_PlayOnce, ANIM_STATUS(38, true), false, ANIM_STATUS(38, true), { Q12(10.0f) }, 676, 677},
    { Anim_BlendLinear, ANIM_STATUS(39, false), false, ANIM_STATUS(39, true), { Q12(10.0f) }, NO_VALUE, 678 },
    { Anim_PlayOnce, ANIM_STATUS(39, true), false, ANIM_STATUS(39, true), { Q12(16.0f) }, 678, 722 },
    { Anim_BlendLinear, ANIM_STATUS(40, false), false, ANIM_STATUS(40, true), { Q12(10.0f) }, NO_VALUE, 723 },
    { Anim_PlayOnce, ANIM_STATUS(40, true), false, ANIM_STATUS(40, true), { Q12(5.0f) }, 723, 744 },
    { Anim_BlendLinear, ANIM_STATUS(41, false), false, ANIM_STATUS(41, true), { Q12(10.0f) }, NO_VALUE, 745 },
    { Anim_PlayLoop, ANIM_STATUS(41, true), false, NO_VALUE, { Q12(2.0f) }, 745, 748 },
    { Anim_BlendLinear, ANIM_STATUS(42, false), false, ANIM_STATUS(42, true), { Q12(10.0f) }, NO_VALUE, 485 },
    { Anim_PlayOnce, ANIM_STATUS(42, true), false, ANIM_STATUS(42, true), { Q12(20.0f) }, 485, 502 },
    {}
};

s_UnkStruct3_Mo g_MapHeaderTable_38[] = {
    { 19788, 0, 676, 677 },
    { 20302, 387, 678, 722 },
    { 20816, 389, 723, 744 },
    { 21330, 400, 745, 748 },
    { 21844, 500, 485, 502 },
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

/* Slight differences from anim info in Chara_Dahlia.h
s_AnimInfo DAHLIA_ANIM_INFOS[] = {
    { Anim_BlendLinear, ANIM_STATUS(0, false), false, ANIM_STATUS(0, false),{ Q12(0.0f) }, NO_VALUE, 0 },
    { Anim_PlayLoop, ANIM_STATUS(0, true), false, NO_VALUE,{ Q12(30.0f) }, NO_VALUE, 1 },
    { Anim_BlendLinear, ANIM_STATUS(1, false), false, ANIM_STATUS(1, true),{ Q12(10.0f) }, NO_VALUE, 0 },
    { Anim_PlayLoop, ANIM_STATUS(1, true), false, NO_VALUE,{ Q12(5.0f) }, 0, 15  },
    { Anim_BlendLinear, ANIM_STATUS(2, false), false, ANIM_STATUS(2, true),{ Q12(10.0f) }, NO_VALUE, 16 },
    { Anim_PlayLoop, ANIM_STATUS(2, true), false, NO_VALUE,{ Q12(15.0f) }, 16, 44 },
    { Anim_BlendLinear, ANIM_STATUS(3, false), false, ANIM_STATUS(3, true),{ Q12(10.0f) }, NO_VALUE, 45 },
    { Anim_PlayOnce, ANIM_STATUS(3, true), false, ANIM_STATUS(3, true),{ Q12(15.0f) }, 45, 85 },
    { Anim_BlendLinear, ANIM_STATUS(4, false), false, ANIM_STATUS(4, true),{ Q12(5.0f) }, NO_VALUE, 86 },
    { Anim_PlayOnce, ANIM_STATUS(4, true), false, ANIM_STATUS(4, true),{ Q12(4.2f) }, 86, 109 },
    { Anim_BlendLinear, ANIM_STATUS(5, false), false, ANIM_STATUS(5, true),{ Q12(10.0f) }, NO_VALUE, 110 },
    { Anim_PlayOnce, ANIM_STATUS(5, true), false, ANIM_STATUS(5, true),{ Q12(7.0f) }, 110, 175 },
    { Anim_BlendLinear, ANIM_STATUS(6, false), false, ANIM_STATUS(6, true),{ Q12(10.0f) }, NO_VALUE, 176 },
    { Anim_PlayOnce, ANIM_STATUS(6, true), false, ANIM_STATUS(6, true),{ Q12(7.0f) }, 176, 226 },
    { Anim_BlendLinear, ANIM_STATUS(7, false), false, ANIM_STATUS(7, true),{ Q12(15.0f) }, NO_VALUE, 227 },
    { Anim_PlayOnce, ANIM_STATUS(7, true), false, ANIM_STATUS(7, true),{ Q12(10.0f) }, 227, 247 },
    { Anim_BlendLinear, ANIM_STATUS(8, false), false, ANIM_STATUS(8, true),{ Q12(10.0f) }, NO_VALUE, 248 },
    { Anim_PlayLoop, ANIM_STATUS(8, true), false, NO_VALUE,{ Q12(5.0f) }, 248, 263 },
    { Anim_BlendLinear, ANIM_STATUS(9, false), false, ANIM_STATUS(9, true),{ Q12(10.0f) }, NO_VALUE, 264 },
    { Anim_PlayOnce, ANIM_STATUS(9, true), false, ANIM_STATUS(9, true),{ Q12(6.0f) }, 264, 304 },
    { Anim_BlendLinear, ANIM_STATUS(10, false), false, ANIM_STATUS(10, true),{ Q12(10.0f) }, NO_VALUE, 305 },
    { Anim_PlayOnce, ANIM_STATUS(10, true), false, ANIM_STATUS(10, true),{ Q12(10.0f) }, 305, 335 },
    { Anim_BlendLinear, ANIM_STATUS(11, false), false, ANIM_STATUS(11, true),{ Q12(10.0f) }, NO_VALUE, 336 },
    { Anim_PlayOnce, ANIM_STATUS(11, true), false, ANIM_STATUS(11, true),{ Q12(10.0f) }, 336, 349 },
    { Anim_BlendLinear, ANIM_STATUS(12, false), false, ANIM_STATUS(12, true),{ Q12(10.0f) }, NO_VALUE, 350 },
    { Anim_PlayLoop, ANIM_STATUS(12, true), false, NO_VALUE,{ Q12(10.0f) }, 350, 369 },
    { Anim_BlendLinear, ANIM_STATUS(13, false), false, ANIM_STATUS(13, true),{ Q12(10.0f) }, NO_VALUE, 370 },
    { Anim_PlayOnce, ANIM_STATUS(13, true), false, ANIM_STATUS(13, true),{ Q12(10.0f) }, 370, 395 },
    { Anim_BlendLinear, ANIM_STATUS(14, false), false, ANIM_STATUS(14, true),{ Q12(10.0f) }, NO_VALUE, 16 },
    { Anim_PlayOnce, ANIM_STATUS(14, true), false, ANIM_STATUS(14, true),{ Q12(15.0f) }, 16, 80 },
    { Anim_BlendLinear, ANIM_STATUS(15, false), false, ANIM_STATUS(15, true),{ Q12(10.0f) }, NO_VALUE, 81 },
    { Anim_PlayOnce, ANIM_STATUS(15, true), false, ANIM_STATUS(15, true),{ Q12(10.0f) }, 81, 96 },
    { Anim_BlendLinear, ANIM_STATUS(16, false), false, ANIM_STATUS(16, true),{ Q12(10.0f) }, NO_VALUE, 97 },
    { Anim_PlayOnce, ANIM_STATUS(16, true), false, ANIM_STATUS(16, true),{ Q12(10.0f) }, 97, 103 },
    { Anim_BlendLinear, ANIM_STATUS(17, false), false, ANIM_STATUS(17, true),{ Q12(10.0f) }, NO_VALUE, 104 },
    { Anim_PlayOnce, ANIM_STATUS(17, true), false, ANIM_STATUS(17, true),{ Q12(10.0f) }, 104, 139 },
    { Anim_BlendLinear, ANIM_STATUS(18, false), false, ANIM_STATUS(18, true),{ Q12(10.0f) }, NO_VALUE, 140 },
    { Anim_PlayLoop, ANIM_STATUS(18, true), false, NO_VALUE,{ Q12(10.0f) }, 140, 155 },
    { Anim_BlendLinear, ANIM_STATUS(19, false), false, ANIM_STATUS(19, true),{ Q12(10.0f) }, NO_VALUE, 156 },
    { Anim_PlayOnce, ANIM_STATUS(19, true), false, ANIM_STATUS(19, true),{ Q12(10.0f) }, 156, 193 },
    { Anim_BlendLinear, ANIM_STATUS(20, false), false, ANIM_STATUS(20, true),{ Q12(10.0f) }, NO_VALUE, 194 },
    { Anim_PlayLoop, ANIM_STATUS(20, true), false, NO_VALUE,{ Q12(10.0f) }, 194, 209 },
    { Anim_BlendLinear, ANIM_STATUS(21, false), false, ANIM_STATUS(21, true),{ Q12(10.0f) }, NO_VALUE, 210 },
    { Anim_PlayOnce, ANIM_STATUS(21, true), false, ANIM_STATUS(21, true),{ Q12(10.0f) }, 210, 228 },
    { Anim_BlendLinear, ANIM_STATUS(22, false), false, ANIM_STATUS(22, true),{ Q12(10.0f) }, NO_VALUE, 229 },
    { Anim_PlayLoop, ANIM_STATUS(22, true), false, NO_VALUE,{ Q12(10.0f) }, 229, 244 },
    { Anim_BlendLinear, ANIM_STATUS(23, false), false, ANIM_STATUS(23, true),{ Q12(10.0f) }, NO_VALUE, 245 },
    { Anim_PlayOnce, ANIM_STATUS(23, true), false, ANIM_STATUS(23, true),{ Q12(10.0f) }, 245, 259 },
    { Anim_BlendLinear, ANIM_STATUS(24, false), false, ANIM_STATUS(24, true),{ Q12(10.0f) }, NO_VALUE, 260 },
    { Anim_PlayOnce, ANIM_STATUS(24, true), false, ANIM_STATUS(24, true),{ Q12(10.0f) }, 260, 296 },
    { Anim_BlendLinear, ANIM_STATUS(25, false), false, ANIM_STATUS(25, true),{ Q12(10.0f) }, NO_VALUE, 260 },
    { Anim_PlayOnce, ANIM_STATUS(25, true), false, ANIM_STATUS(24, true),{ Q12(0.0f) }, 260, 296}
};
*/