#include "game.h"

#include "bodyprog/bodyprog.h"
#include "bodyprog/collision/chara.h"
#include "bodyprog/collision/collision.h"
#include "bodyprog/math/math.h"
#include "main/rng.h"

bool func_80070320(void) // 0x80070320
{
    s32 i;

    for (i = 0; i < ARRAY_SIZE(g_SysWork.npcIdxs); i++)
    {
        if (g_SysWork.npcIdxs[i] != NO_VALUE)
        {
            return true;
        }
    }

    return false;
}

q19_12 func_80070360(s_SubCharacter* chara, q19_12 someDist, q3_12 arg2) // 0x80070360
{
    q25_6  offsetToPlayerX;
    q25_6  offsetToPlayerZ;
    q19_12 distToPlayer;
    q19_12 result;

    distToPlayer = someDist;
    if (distToPlayer == Q12(0.0f))
    {
        offsetToPlayerX = Q12_TO_Q6(PLAYER_CHARA().position.vx - chara->position.vx);
        offsetToPlayerZ = Q12_TO_Q6(PLAYER_CHARA().position.vz - chara->position.vz);
        distToPlayer    = Q6_TO_Q12(Math_Vector2MagCalc(offsetToPlayerX, offsetToPlayerZ));
    }

    // TODO: Why `>> 8`? Odd type conversion?
    result = Q12_MULT(arg2, PLAYER_PROPS().field_10C) - (distToPlayer >> 8);
    if (result < Q12(0.0f))
    {
        result = Q12(0.0f);
    }
    return result;
}

void Collision_CharaCollisionSet(s_SubCharacter* chara, s_CharaKeyframe* keyframe0, s_CharaKeyframe* keyframe1) // 0x80070400
{
    q19_12 alpha;
    q19_12 invAlpha;

    // Compute alpha.
    if (ANIM_STATUS_IS_ACTIVE(chara->model.anim.status))
    {
        alpha = Q12_FRACT(chara->model.anim.time);
    }
    else
    {
        alpha = chara->model.anim.alpha;
    }
    invAlpha = Q12(1.0f) - alpha;

    // Set interpolated collision shapes for active frame.
    chara->collision.box.topExtent            = FP_FROM((keyframe0->box.topExtent            * invAlpha) + (keyframe1->box.topExtent            * alpha), Q12_SHIFT);
    chara->collision.box.bottomExtent         = FP_FROM((keyframe0->box.bottomExtent         * invAlpha) + (keyframe1->box.bottomExtent         * alpha), Q12_SHIFT);
    chara->collision.box.height               = FP_FROM((keyframe0->box.height               * invAlpha) + (keyframe1->box.height               * alpha), Q12_SHIFT);
    chara->collision.box.offsetY              = FP_FROM((keyframe0->box.offsetY              * invAlpha) + (keyframe1->box.offsetY              * alpha), Q12_SHIFT);
    chara->collision.shapeOffsets.cylinder.vx = FP_FROM((keyframe0->shapeOffsets.cylinder.vx * invAlpha) + (keyframe1->shapeOffsets.cylinder.vx * alpha), Q12_SHIFT);
    chara->collision.shapeOffsets.cylinder.vz = FP_FROM((keyframe0->shapeOffsets.cylinder.vz * invAlpha) + (keyframe1->shapeOffsets.cylinder.vz * alpha), Q12_SHIFT);
    chara->collision.cylinder.outerRadius     = FP_FROM((keyframe0->box.field_8              * invAlpha) + (keyframe1->box.field_8              * alpha), Q12_SHIFT);
    chara->collision.shapeOffsets.box.vx      = FP_FROM((keyframe0->shapeOffsets.box.vx      * invAlpha) + (keyframe1->shapeOffsets.box.vx      * alpha), Q12_SHIFT);
    chara->collision.shapeOffsets.box.vz      = FP_FROM((keyframe0->shapeOffsets.box.vz      * invAlpha) + (keyframe1->shapeOffsets.box.vz      * alpha), Q12_SHIFT);
    chara->collision.cylinder.innerRadius     = FP_FROM((keyframe0->box.field_A              * invAlpha) + (keyframe1->box.field_A              * alpha), Q12_SHIFT);
}

void Chara_ModelBoneScaleSet(GsCOORDINATE2* boneCoords, s32 boneIdx, q19_12 scaleX, q19_12 scaleY, q19_12 scaleZ) // 0x800705E4
{
    q3_12 scale[3];
    s32   i;
    s32   j;

    #define boneCoord boneCoords[boneIdx]

    // Set scale.
    scale[0] = scaleX;
    scale[1] = scaleY;
    scale[2] = scaleZ;

    // Run through scale components.
    for (i = 0; i < ARRAY_SIZE(scale); i++)
    {
        // Ignore scale component of 1.
        if (scale[i] == Q12(1.0f))
        {
            continue;
        }

        // Apply scale to bone matrix.
        for (j = 0; j < ARRAY_SIZE(scale); j++)
        {
            boneCoord.coord.m[j][i] = Q12_MULT_PRECISE(scale[i], boneCoord.coord.m[j][i]);
        }
    }

    boneCoords->flg = false;

    #undef boneCoord
}
