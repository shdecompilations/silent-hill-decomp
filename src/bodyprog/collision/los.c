#include "game.h"

#include <psyq/strings.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/collision/collision.h"
#include "bodyprog/world/effects.h"
#include "bodyprog/math/math.h"
#include "main/rng.h"

q19_12 func_8006F99C(s_SubCharacter* chara, q19_12 dist, q3_12 headingAngle) // 0x8006F99C
{
    q3_12 curAngleOffset;
    q3_12 angleOffset;
    s32   i;

    // TODO: What are these bit shifts?
    angleOffset = NO_VALUE;
    for (i = 0; i < 15; i++)
    {
        if (i == 0)
        {
            curAngleOffset = Rng_GenerateUInt(-32, 31);
        }
        else if (i & 0x1)
        {
            curAngleOffset = (256 << ((i + 1) >> 1)) + Rng_GenerateUInt(0, 63);
        }
        else
        {
            curAngleOffset = -(256 << (i >> 1)) - Rng_GenerateUInt(0, 63);
        }

        if (angleOffset != NO_VALUE)
        {
            if (ABS(angleOffset) < ABS(curAngleOffset))
            {
                continue;
            }
        }

        if (!Los_CharaHitCheck(chara, dist, curAngleOffset + headingAngle))
        {
            angleOffset = curAngleOffset;
        }
    }

    if (angleOffset != NO_VALUE)
    {
        return Math_AngleNormalizeSigned(angleOffset + headingAngle);
    }

    return Q12_ANGLE(360.0f);
}

q19_12 Los_CharaHeadingAngleGet(s_SubCharacter* chara, q19_12 dist, q19_12 toX, q19_12 toZ, q3_12 spanAngle, bool isClockwise) // 0x8006FAFC
{
    #define SPAN_STEP_COUNT 3

    s16    spanAngleStep;
    q3_12  curAngle;
    q19_12 curToZ;
    q19_12 curToX;
    q25_6  curOffsetX;
    q25_6  curOffsetZ;
    q25_6  curDist;
    s32    i;
    q19_12 shortestOrLongestDist;
    s32    stepCount;
    q3_12  unkAngle;

    // Define if distance should track minimum or maximum.
    shortestOrLongestDist = Q12(0.0f);
    if (isClockwise)
    {
        shortestOrLongestDist = INT_MAX;
    }

    spanAngleStep = spanAngle / SPAN_STEP_COUNT;
    unkAngle = Q12_ANGLE(-360.0f);

    // Define step count.
    stepCount = 7;
    if (spanAngle == Q12_ANGLE(360.0f))
    {
        stepCount = 12;
    }

    // Run through steps in span.
    for (i = 0; i < stepCount; i++)
    {
        if (spanAngle == Q12_ANGLE(360.0f))
        {
            curAngle = Q12((i * 30) + (Rng_Rand16() % 30)) / 360;
        }
        else
        {
            curAngle = (chara->rotation.vy +
                        ((i - SPAN_STEP_COUNT) * spanAngleStep) +
                        ((Rng_Rand16() % spanAngleStep) >> 1)) -
                       (spanAngleStep >> 2);
        }

        curToX = chara->position.vx + Q12_MULT(dist, Math_Sin(curAngle));
        curToZ = chara->position.vz + Q12_MULT(dist, Math_Cos(curAngle));

        if (!Los_CharaToTargetHitCheck(chara, curToX, chara->position.vy, curToZ))
        {
            curOffsetX = Q12_TO_Q6(toX - curToX);
            curOffsetZ = Q12_TO_Q6(toZ - curToZ);
            curDist    = SQUARE(curOffsetX) + SQUARE(curOffsetZ);

            if ((!isClockwise && (shortestOrLongestDist < curDist)) ||
                ( isClockwise && (curDist               < shortestOrLongestDist)))
            {
                shortestOrLongestDist = curDist;
                unkAngle              = curAngle;
            }
        }
    }

    if (unkAngle != Q12_ANGLE(-360.0f))
    {
        return Math_AngleNormalizeSigned(unkAngle);
    }
    return Q12_ANGLE(360.0f);

    #undef SPAN_STEP_COUNT
}

bool func_8006FD90(s_SubCharacter* chara, s32 count, q19_12 baseDistMax, q19_12 distStep) // 0x8006FD90
{
    VECTOR3 sp10;
    VECTOR3 sp20;
    VECTOR3 pos;    // Q19.12
    VECTOR3 offset; // Q19.12
    s32     i;
    q19_12  dist;
    q19_12  distMult;
    q19_12  distMax;

    if (Math_AngleNormalizeSigned(ratan2(PLAYER_CHARA().position.vx - chara->position.vx,
                                         PLAYER_CHARA().position.vz - chara->position.vz) -
                                  chara->rotation.vy) < Q12_ANGLE(0.0f))
    {
        distMult = (Math_AngleNormalizeSigned(ratan2(PLAYER_CHARA().position.vx - chara->position.vx,
                                                     PLAYER_CHARA().position.vz - chara->position.vz) -
                                              chara->rotation.vy) * 2) + Q12_ANGLE(360.0f);
    }
    else
    {
        distMult = (Q12_ANGLE(180.0f) - Math_AngleNormalizeSigned((ratan2(PLAYER_CHARA().position.vx - chara->position.vx,
                                                                          PLAYER_CHARA().position.vz - chara->position.vz) -
                                                                   chara->rotation.vy))) * 2;
    }

    for (i = distMult; count > 0; count--)
    {
        distMult = Q12_MULT_PRECISE(distMult, i);
    }

    dist = Math_Vector2MagCalcSafeQ6(PLAYER_CHARA().position.vx - chara->position.vx,
                                     PLAYER_CHARA().position.vz - chara->position.vz);
    distMax = baseDistMax + Q12_MULT_PRECISE(distStep, distMult);
    if (distMax < dist)
    {
        return false;
    }

    pos.vx = chara->position.vx;
    pos.vz = chara->position.vz;

    offset.vx = PLAYER_CHARA().position.vx - chara->position.vx;
    offset.vz = PLAYER_CHARA().position.vz - chara->position.vz;

    if (!WorldEnv_IsInDarkness() && WorldEnv_IsFlashlightAllowed())
    {
        offset.vy = Q12(0.0f);
        pos.vy    = PLAYER_CHARA().position.vy + PLAYER_CHARA().collision.box.topExtent;
    }
    else
    {
        pos.vy    = chara->position.vy + chara->collision.box.offsetY;
        offset.vy = (PLAYER_CHARA().position.vy + PLAYER_CHARA().collision.box.offsetY) -
                    (chara->position.vy - chara->collision.box.offsetY);
    }

    // TODO: Maybe `sp10` is not `VECTOR3`. Might need to rewrite this whole function if it's `s_RayTrace`?
    return !Ray_GeomAndCharaTraceGet(&sp10, &pos, &offset, chara) || sp20.vx != Q12(0.0f);
}

bool Los_CharaToTargetHitCheck(s_SubCharacter* fromChara, q19_12 toX, q19_12 toY, q19_12 toZ)
{
    s_RayTrace trace;
    VECTOR3    offset; // Q19.12

    offset.vx = toX - fromChara->position.vx;
    offset.vy = toY - fromChara->position.vy;
    offset.vz = toZ - fromChara->position.vz;

    return Ray_GeomAndCharaTraceGetAlt(&trace, &fromChara->position, &offset, fromChara);
}

bool Los_CharaToCharaTargetCheck(s_SubCharacter* fromChara, q19_12 toX, q19_12 toY, q19_12 toZ) // 0x80070084
{
    s_RayTrace trace;
    VECTOR3    offset; // Q19.12
    bool       isCharaMissed;

    offset.vx = toX - fromChara->position.vx;
    offset.vy = toY - fromChara->position.vy;
    offset.vz = toZ - fromChara->position.vz;

    isCharaMissed = false;
    if (Ray_GeomAndCharaTraceGetAlt(&trace, &fromChara->position, &offset, fromChara))
    {
        isCharaMissed = trace.character == NULL;
    }
    return isCharaMissed;
}

bool Los_NpcToPlayerHitCheck(s_SubCharacter* fromNpc, s_SubCharacter* toPlayer) // 0x800700F8
{
    s_RayTrace trace;
    VECTOR3    from;   // Q19.12
    VECTOR3    offset; // Q19.12

    from = fromNpc->position;

    offset.vx = toPlayer->position.vx - fromNpc->position.vx;
    offset.vy = Q12(-0.1f);
    offset.vz = toPlayer->position.vz - fromNpc->position.vz;

    return Ray_GeomAndCharaTraceGetAlt(&trace, &from, &offset, fromNpc) && trace.character == NULL;
}

bool Los_CharaToCharaDistCheck(s_SubCharacter* fromChara, q19_12 dist, q3_12 headingAngle) // 0x80070184
{
    q19_12 toX;
    q19_12 toY;
    q19_12 toZ;
    q19_12 sinHeadingAngle;
    q19_12 cosHeadingAngle;

    sinHeadingAngle = Math_Sin(headingAngle);
    toX             = fromChara->position.vx + Q12_MULT(dist, sinHeadingAngle);

    cosHeadingAngle = Math_Cos(headingAngle);
    toY             = fromChara->position.vy;
    toZ             = fromChara->position.vz + Q12_MULT(dist, cosHeadingAngle);

    return Los_CharaToCharaTargetCheck(fromChara, toX, toY, toZ);
}

bool Los_CharaToCharaFrontHitCheck(s_SubCharacter* fromChara, q19_12 dist) // 0x80070208
{
    s_RayTrace trace;
    VECTOR3    offset; // Q19.12
    bool       hasHit;

    offset.vx = Q12_MULT(dist, Math_Sin(fromChara->rotation.vy));
    offset.vy = Q12(0.0f);
    offset.vz = Q12_MULT(dist, Math_Cos(fromChara->rotation.vy));

    hasHit = false;
    if (Ray_GeomAndCharaTraceGetAlt(&trace, &fromChara->position, &offset, fromChara))
    {
        hasHit = trace.character > NULL;
    }
    return hasHit;
}

bool Los_CharaHitCheck(s_SubCharacter* fromChara, q19_12 dist, q3_12 headingAngle) // 0x8007029C
{
    s_RayTrace trace;
    VECTOR3    offset; // Q19.12

    offset.vx = Q12_MULT(dist, Math_Sin(headingAngle));
    offset.vy = Q12(0.0f);
    offset.vz = Q12_MULT(dist, Math_Cos(headingAngle));

    return Ray_GeomAndCharaTraceGetAlt(&trace, &fromChara->position, &offset, fromChara);
}
