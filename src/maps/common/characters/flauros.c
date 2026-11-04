#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "maps/common/characters/flauros.h"

void Flauros_Update(s_SubCharacter* flauros, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords) // 0x800DE0C4
{
    s32         i;
    q19_12      scale;
    s_AnimInfo* animInfo;

    if (flauros->model.controlState == FlaurosControl_None)
    {
        flauros->model.anim.alpha   = Q12(0.0f);
        flauros->model.controlState = FlaurosControl_1;
        flauros->model.stateStep    = 0;
        Chara_AnimSet(flauros, ANIM_STATUS(FlaurosAnim_1, true), 0);
    }

    if (flauros->model.anim.time <= Q12(21.0f))
    {
        if (flauros->model.anim.time > Q12(13.0f))
        {
            scale = (flauros->model.anim.time - Q12(13.0f)) >> 3;
        }
        else
        {
            scale = Q12(0.0f);
        }
    }
    else
    {
        scale = Q12(1.0f);
    }

    Math_MatrixTransform(&flauros->position, &flauros->rotation, boneCoords);

    animInfo = &FLAUROS_ANIM_INFOS[flauros->model.anim.status];
    animInfo->playbackFunc(&flauros->model, anmHdr, boneCoords, animInfo);

    // TODO: Are these the Flauros' light rays?
    for (i = 6; i < 11; i++)
    {
        Chara_ModelBoneScaleSet(boneCoords, i, scale, scale, scale);
    }
}
