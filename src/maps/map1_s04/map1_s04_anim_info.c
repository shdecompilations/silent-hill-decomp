#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"

s_AnimInfo HARRY_M1S04_ANIM_INFOS[] = {
    { Anim_BlendLinear, ANIM_STATUS(38, false), false, ANIM_STATUS(38, true), { Q12(10.0f) }, NO_VALUE, 676},
    { Anim_PlayOnce, ANIM_STATUS(38, true), false, ANIM_STATUS(38, true), { Q12(10.0f) }, 676, 677},
    { Anim_BlendLinear, ANIM_STATUS(54, false), false, ANIM_STATUS(54, true), { Q12(10.0f) }, NO_VALUE, 485},
    { Anim_PlayOnce, ANIM_STATUS(54, true), false, ANIM_STATUS(54, true), { Q12(20.0f) }, 485, 502},
    {}
};

s_UnkStruct3_Mo g_MapHeaderTable_38[2] = {
    { 19788, 0, 676, 677, },
    {},
};

u8 LOADABLE_INVENTORY_ITEMS[] = {
    InvItemId_Unequipped
};

u8 sharedData_800DD584_0_s00 = 0;

q19_12 sharedData_800DD598_0_s00 = Q12_ANGLE(0.0f);

u8 g_Player_PrevWeaponAttack = 255;
