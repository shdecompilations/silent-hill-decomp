#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "maps/common/unk_m3s03.h"

// Utility functions used by PuppetNurse/Twinfeeler/Incubus.

q19_12 sharedFunc_800CD6B0_3_s03(MATRIX* mat, s32 matCount, VECTOR3* center)
{
    s32    i;
    q19_12 posX;
    q19_12 posZ;
    q23_8  minXTemp;
    q23_8  minX;
    q23_8  maxX;
    q23_8  minZTemp;
    q23_8  minZ;
    q23_8  maxZ;
    q19_12 radius;

    maxX =
    minX =
    posX = mat->t[0];

    maxZ =
    minZ =
    posZ = mat->t[2];

    mat++;

    for (i = 1; i < matCount; i++, mat++)
    {
        posX = mat->t[0];
        posZ = mat->t[2];

        // `minX = MIN(posX, minX)`
        minXTemp = minX;
        if (minXTemp >= posX)
        {
            minXTemp = posX;
        }
        minX = minXTemp;
        maxX = MAX(posX, maxX);

        // `minZ = MIN(posZ, minZ)`
        minZTemp = minZ;
        if (minZTemp >= posZ)
        {
            minZTemp = posZ;
        }
        minZ = minZTemp;
        maxZ = MAX(posZ, maxZ);
    }

    posX   = maxX - minX;
    posX   = Q8_TO_Q12(posX);
    posZ   = maxZ - minZ;
    posZ   = Q8_TO_Q12(posZ);
    radius = SquareRoot12(Q12_SQUARE_PRECISE(posX) +
                          Q12_SQUARE_PRECISE(posZ));

    posX       = Q8_TO_Q12(minX) + (posX / 2);
    posZ       = Q8_TO_Q12(minZ) + (posZ / 2);
    center->vx = posX;
    center->vz = posZ;
    return radius / 2;
}

void sharedFunc_800CD7F8_3_s03(s_SubCharacter* chara, q19_12 offsetX, q19_12 offsetZ, q19_12 range)
{
    q19_12 extraOffsetX;
    q19_12 extraOffsetZ;
    q19_12 extraDistSqr;
    q19_12 rootAdjOffsetSqr;
    q19_12 scale;
    q19_12 scaledExtraOffsetX;
    q19_12 scaledExtraOffsetZ;

    // Compute extra offset.
    extraOffsetX = chara->collision.shapeOffsets.cylinder.vx - offsetX;
    extraOffsetZ = chara->collision.shapeOffsets.cylinder.vz - offsetZ;

    // Apply extra offset if its distance exceeds a threshold.
    extraDistSqr = Q12_SQUARE_PRECISE(extraOffsetX) +
                   Q12_SQUARE_PRECISE(extraOffsetZ);
    if (extraDistSqr > Q12_MULT_PRECISE(range, 4))
    {
        // Compute scale.
        rootAdjOffsetSqr = SquareRoot12(extraDistSqr);
        scale            = Q12(rootAdjOffsetSqr - 128) / rootAdjOffsetSqr;

        // Adjust extra X offset.
        scaledExtraOffsetX = scale * extraOffsetX;
        if (scaledExtraOffsetX < Q12(0.0f))
        {
            scaledExtraOffsetX += Q12_CLAMPED(1.0f);
        }
        extraOffsetX = FP_FROM(scaledExtraOffsetX, Q12_SHIFT);

        // Adjust extra Z offset.
        scaledExtraOffsetZ = scale * extraOffsetZ;
        if (scaledExtraOffsetZ < Q12(0.0f))
        {
            scaledExtraOffsetZ += Q12_CLAMPED(1.0f);
        }
        extraOffsetZ = FP_FROM(scaledExtraOffsetZ, Q12_SHIFT);

        // Apply extra offset.
        offsetX += extraOffsetX;
        offsetZ += extraOffsetZ;
    }

    // Set translation offset.
    chara->collision.shapeOffsets.cylinder.vx = offsetX;
    chara->collision.shapeOffsets.cylinder.vz = offsetZ;
}

void sharedFunc_800CD920_3_s03(s_SubCharacter* chara, q19_12 offsetX, q19_12 offsetZ)
{
    sharedFunc_800CD7F8_3_s03(chara, offsetX, offsetZ, Q12(1.0f));
}

q19_12 sharedFunc_800CD940_3_s03(q19_12 pos0, q19_12 pos1)
{
    q19_12 delta;
    q19_12 absDelta;
    q19_12 result;

    delta    = pos0 - pos1;
    absDelta = ABS(delta);
    if (absDelta > Q12(1.0f / 32.0f))
    {
        if (delta <= 0)
        {
            pos0 = pos1 - Q12(1.0f / 32.0f);
        }
        else
        {
            pos0 = pos1 + Q12(1.0f / 32.0f);
        }
    }

    if (pos0 <= Q12(0.35f))
    {
        result = pos0;
    }
    else
    {
        result = Q12(0.35f);
    }

    return result;
}
