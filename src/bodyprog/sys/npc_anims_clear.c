#include "game.h"

#include "bodyprog/bodyprog.h"
#include "bodyprog/sys/npc_anims_clear.h"
#include "bodyprog/game_boot/fs_chara_anim.h"

void Anim_CharaTypeAnimInfoClear(void)
{
    bzero(&g_CharaModelAnimsData[1], sizeof(s_CharaAnimData) * (CHARA_GROUP_COUNT - 1));
}
