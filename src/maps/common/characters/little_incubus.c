#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "maps/common/characters/little_incubus.h"

void LittleIncubus_Update(s_SubCharacter* incubus, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords) // 0x800D5BC8
{
    q19_12      scale;
    q19_12      clampedScale;
    s_AnimInfo* animInfo;

    if (incubus->model.controlState == LitteIncubusControl_None)
    {
        incubus->model.anim.alpha   = Q12(0.0f);
        incubus->model.controlState = LitteIncubusControl_1;
        incubus->model.stateStep    = 0;
        Chara_AnimSet(incubus, ANIM_STATUS(LitteIncubusAnim_1, true), 0);

        D_800EDA00 = 0;
    }

    D_800EDA00 += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 10.0f);
    clampedScale = FP_TO(D_800EDA00, Q12_SHIFT) / Q12(80.0f);

    scale = clampedScale;
    if (clampedScale > Q12(1.0f))
    {
        clampedScale = Q12(1.0f);
    }
    scale = Math_Sin(clampedScale >> 2);

    Math_MatrixTransform(&incubus->position, &incubus->rotation, boneCoords);

    animInfo = &LITTLE_INCUBUS_ANIM_INFOS[incubus->model.anim.status];
    animInfo->playbackFunc(&incubus->model, anmHdr, boneCoords, animInfo);

    Chara_ModelBoneScaleSet(boneCoords, LittleIncubusBone_1, scale, scale, scale);
    Chara_ModelBoneScaleSet(boneCoords, LittleIncubusBone_7, scale, scale, scale);
}
