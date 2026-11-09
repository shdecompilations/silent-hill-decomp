#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "maps/common/characters/parasite.h"

void Parasite_Update(s_SubCharacter* parasite, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords) // 0x800DE1CC
{
    s_AnimInfo* animInfo;

    if (parasite->model.controlState == ParasiteControl_None)
    {
        parasite->model.anim.alpha   = Q12(0.0f);
        parasite->model.controlState = ParasiteControl_1;
        parasite->model.stateStep    = 0;
        Chara_AnimSet(parasite, ANIM_STATUS(ParasiteAnim_1, true), 0);
    }

    Math_MatrixTransform(&parasite->position, &parasite->rotation, boneCoords);

    animInfo = &PARASITE_ANIM_INFOS[parasite->model.anim.status];
    animInfo->playbackFunc(&parasite->model, anmHdr, boneCoords, animInfo);
}
