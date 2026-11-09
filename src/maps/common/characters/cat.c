#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "main/rng.h"
#include "maps/common/characters/cat.h"

#define catProps cat->properties.cat

void Cat_Update(s_SubCharacter* cat, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords) // 0x800D6D40
{
    s_AnimInfo* animInfo;
    bool        cond;

    if (cat->model.controlState == CatControl_None)
    {
        cat->model.controlState     = CatControl_1;
        cat->model.anim.status      = ANIM_STATUS(CatAnim_IdleToJump, true);
        cat->model.anim.time        = Q12(7.0f);
        cat->model.anim.alpha       = Q12(0.0f);
        cat->model.stateStep        = 0;
        cat->model.anim.keyframeIdx = 7;
        cat->position.vy            = Q12(0.0f);
        catProps.field_E8           = 0;
    }

    if (cat->model.stateStep == 0)
    {
        if (cat->model.controlState == CatControl_2)
        {
            cat->model.anim.status      = ANIM_STATUS(CatAnim_Jump, true);
            cat->model.anim.time        = Q12(7.0f);
            cat->model.anim.keyframeIdx = 7;
        }
        else if (cat->model.controlState == CatControl_3)
        {
            cat->model.anim.status      = ANIM_STATUS(CatAnim_Run, true);
            cat->model.anim.time        = Q12(23.0f);
            cat->model.anim.keyframeIdx = 23;
        }

        cat->model.stateStep++;
    }

    Math_MatrixTransform(&cat->position, &cat->rotation, boneCoords);

    animInfo = &CAT_ANIM_INFOS[cat->model.anim.status];
    animInfo->playbackFunc(&cat->model, anmHdr, boneCoords, animInfo);

    cond = false;
    if (cat->model.anim.status == ANIM_STATUS(CatAnim_Jump, true))
    {
        if ((catProps.field_E8 == 0 && ANIM_TIME_RANGE_CHECK(cat->model.anim.time, 20, 22)) ||
            (catProps.field_E8 != 0 && ANIM_TIME_RANGE_CHECK(cat->model.anim.time, 25, 27)))
        {
            cond = true;
        }
    }
    else
    {
        if ((catProps.field_E8 == 0 && ANIM_TIME_RANGE_CHECK(cat->model.anim.time, 29, 31)) ||
            (catProps.field_E8 != 0 && ANIM_TIME_RANGE_CHECK(cat->model.anim.time, 36, 38)))
        {
            cond = true;
        }
    }

    if (cond)
    {
        Sfx_WithPitchPlay(Sfx_CatMeow, &cat->position, Q8(0.5f), Rng_GenerateUInt(-7, 8));
        catProps.field_E8 ^= 1;
    }
}
