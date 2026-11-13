#ifndef _BODYPROG_PLAYER_WEAPONANIMDATA_H
#define _BODYPROG_PLAYER_WEAPONANIMDATA_H

typedef struct
{
    /* 0x0 */ u8  unused_0[4];      /** @unused Looks meaningful, but unclear what it could be. */
    /* 0x4 */ s16 startKeyframeIdx;
    /* 0x6 */ s16 endKeyframeIdx;
} s_800C44F0;

extern const s_AnimInfo HARRY_WEAPON_ANIM_INFOS[];

// Something related to keyframes.
extern const s_800C44F0 D_800294F4[];

#endif
