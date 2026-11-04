#include "game.h"
#include "inline_no_dmpsx.h"

#include <psyq/gtemac.h>
#include <psyq/libapi.h>
#include <psyq/strings.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "bodyprog/screen/screen_data.h"
#include "bodyprog/screen/screen_draw.h"
#include "bodyprog/item_screens.h"
#include "bodyprog/player.h"
#include "bodyprog/sound/sound_system.h"
#include "main/rng.h"

s32 Chara_NpcIdxGet(s_SubCharacter* chara) // 0x8005C7D0
{
    s32             i;
    s_SubCharacter* curNpc;
    s_SubCharacter* player;

    i = 0;

    if (chara == &g_SysWork.playerWork.player)
    {
        return ARRAY_SIZE(g_SysWork.npcs);
    }

    curNpc = &g_SysWork.npcs[0];
    player = chara;
    for (i = 0; i < ARRAY_SIZE(g_SysWork.npcs); i++, curNpc++)
    {
        if (player == curNpc)
        {
            return i;
        }
    }

    return NO_VALUE;
}

void Chara_CollisionShapeOffsetsUpdate(s_CharaShapeOffsets* offsets, s_SubCharacter* chara) // 0x8005C814
{
    q3_12 boxOffsetX;
    q3_12 boxOffsetZ;
    q3_12 cylinderOffsetX;
    q3_12 cylinderOffsetZ;
    q3_12 sinRotY;
    q3_12 cosRotY;

    // Get relative shape offsets.
    boxOffsetX      = offsets->box.vx;
    boxOffsetZ      = offsets->box.vz;
    cylinderOffsetX = offsets->cylinder.vx;
    cylinderOffsetZ = offsets->cylinder.vz;

    // Compute XZ plane heading sine and cosine.
    cosRotY = Math_Cos(chara->rotation.vy);
    sinRotY = Math_Sin(chara->rotation.vy);

    // Update shape offsets.
    chara->collision.shapeOffsets.box.vx      = FP_FROM(( boxOffsetX      * cosRotY) + (boxOffsetZ      * sinRotY), Q12_SHIFT);
    chara->collision.shapeOffsets.box.vz      = FP_FROM((-boxOffsetX      * sinRotY) + (boxOffsetZ      * cosRotY), Q12_SHIFT);
    chara->collision.shapeOffsets.cylinder.vx = FP_FROM(( cylinderOffsetX * cosRotY) + (cylinderOffsetZ * sinRotY), Q12_SHIFT);
    chara->collision.shapeOffsets.cylinder.vz = FP_FROM((-cylinderOffsetX * sinRotY) + (cylinderOffsetZ * cosRotY), Q12_SHIFT);
}

s32 Chara_MovementUpdate0(s_SubCharacter* chara, s_CollisionResult* collResult) // 0x8005C944
{
    s_CollisionResult newCollResult;
    VECTOR3           offset;
    q3_12             headingAngle;
    q19_12            moveSpeed;
    s32               moveDist;
    s32               scaleRestoreShift;
    s32               scaleReduceShift;
    s32               temp_v0;
    q19_12            sinHeadingAngle;
    s32               wallResponse;

    headingAngle      = chara->headingAngle;
    moveSpeed         = Q12_MULT_PRECISE(g_DeltaTime, chara->moveSpeed);
    scaleRestoreShift = OVERFLOW_GUARD(moveSpeed);
    scaleReduceShift  = scaleRestoreShift >> 1;

    sinHeadingAngle = Math_Sin(headingAngle);
    moveDist        = moveSpeed >> scaleReduceShift;
    temp_v0         = sinHeadingAngle >> scaleReduceShift;

    offset.vx = Q12_MULT_PRECISE(moveDist, temp_v0) << scaleRestoreShift;
    offset.vz = Q12_MULT_PRECISE(moveDist, Math_Cos(headingAngle) >> scaleReduceShift) << scaleRestoreShift;
    offset.vy = Q12_MULT_PRECISE(g_DeltaTime, chara->fallSpeed);

    wallResponse = Collision_WallDetect(&newCollResult, &offset, chara);

    chara->position.vx += newCollResult.offset.vx;
    chara->position.vy += newCollResult.offset.vy;
    chara->position.vz += newCollResult.offset.vz;

    // Warp to floor.
    if (chara->position.vy > newCollResult.surface.floorHeight)
    {
        chara->position.vy = newCollResult.surface.floorHeight;
        chara->fallSpeed   = Q12(0.0f);
    }

    if (collResult != NULL)
    {
        *collResult = newCollResult;
    }

    return wallResponse;
}

s32 Chara_MovementUpdate1(s_SubCharacter* chara, s_CollisionResult* collResult, q3_12 offsetX, q3_12 offsetZ) // 0x8005CB20
{
    s_CollisionResult newCollResult;
    VECTOR3           offset; // Q19.12
    q19_12            headingAngle;
    q19_12            moveSpeed;
    s32               moveDist;
    s32               scaleRestoreShift;
    s32               scaleReduceShift;
    s32               temp_v0_2;
    s32               temp_v0_4;
    q19_12            sinHeadingAngle;
    s32               wallResponse;

    headingAngle      = chara->headingAngle;
    moveSpeed         = Q12_MULT_PRECISE(g_DeltaTime, chara->moveSpeed);
    scaleRestoreShift = OVERFLOW_GUARD(moveSpeed);
    scaleReduceShift  = scaleRestoreShift >> 1;

    sinHeadingAngle = Math_Sin(headingAngle);
    moveDist        = moveSpeed >> scaleReduceShift;
    temp_v0_2       = sinHeadingAngle >> scaleReduceShift;
    
    offset.vx       = Q12_MULT_PRECISE(moveDist, temp_v0_2) << scaleRestoreShift;
    temp_v0_4       = Math_Cos(headingAngle) >> scaleReduceShift;
    offset.vz       = Q12_MULT_PRECISE(moveDist, temp_v0_4) << scaleRestoreShift;
    sinHeadingAngle = chara->fallSpeed;
    
    offset.vx += offsetX;
    offset.vy  = Q12_MULT_PRECISE(g_DeltaTime, sinHeadingAngle);
    offset.vz += offsetZ;

    wallResponse = Collision_WallDetect(&newCollResult, &offset, chara);

    chara->position.vx += newCollResult.offset.vx;
    chara->position.vy += newCollResult.offset.vy;
    chara->position.vz += newCollResult.offset.vz;

    if (chara->position.vy > newCollResult.surface.floorHeight)
    {
        chara->position.vy = newCollResult.surface.floorHeight;
        chara->fallSpeed   = Q12(0.0f);
    }

    if (collResult != NULL)
    {
        *collResult = newCollResult;
    }

    return wallResponse;
}

void func_8005CD38(s32* npcIdx, q3_12* angle, const VECTOR3* pos, q3_12 angleMin, q19_12 distMin, s32 mode) // 0x8005CD38
{
    VECTOR3    from;   // Q19.12
    s_RayTrace trace;
    VECTOR3    offset; // Q19.12
    VECTOR3    curPos; // Q19.12
    VECTOR3    boxPos; // Q19.12
    s32        npcIdxs[ARRAY_SIZE(g_SysWork.npcs)];
    q3_12      angles0[ARRAY_SIZE(g_SysWork.npcs)];
    q3_12      angles2[ARRAY_SIZE(g_SysWork.npcs)];
    q19_12     angles1[ARRAY_SIZE(g_SysWork.npcs)];
    q19_12     posX;
    q19_12     posY;
    q19_12     posZ;
    s32        swap;
    s32        i;
    s32        j;
    s32        count;

    posX = pos->vx;
    posY = pos->vy;
    posZ = pos->vz;
    from = (VECTOR3){ posX, posY, posZ };

    count = 0;

    *npcIdx = NO_VALUE;

    // Run through NPCs.
    for (i = 0; i < ARRAY_SIZE(g_SysWork.npcs); i++)
    {
        #define curNpc g_SysWork.npcs[i]

        // Check if NPC is valid.
        if (curNpc.model.charaId == Chara_None || curNpc.health < Q12(0.0f))
        {
            continue;
        }

        // Define character box position.
        boxPos.vx = curNpc.position.vx + curNpc.collision.shapeOffsets.box.vx;
        boxPos.vy = curNpc.position.vy + curNpc.collision.box.offsetY;
        boxPos.vz = curNpc.position.vz + curNpc.collision.shapeOffsets.box.vz;

        if (mode < 3)
        {
            if (Math_SqrMagCalcToQ6(distMin) < (Math_SqrMagCalcToQ6(posX - boxPos.vx) +
                                                Math_SqrMagCalcToQ6(posY - boxPos.vy) +
                                                Math_SqrMagCalcToQ6(posZ - boxPos.vz)))
            {
                continue;
            }

            angles2[count] = ratan2(boxPos.vx - posX, boxPos.vz - posZ);
            if (angleMin < ABS(Math_AngleNormalizeSigned(g_SysWork.playerWork.player.rotation.vy - angles2[count])))
            {
                continue;
            }

            if (mode != 0)
            {
                if (g_SysWork.targetNpcIdx == i)
                {
                    continue;
                }

                angles2[count] = Math_AngleNormalizeSigned(angles2[count] - g_SysWork.playerWork.player.angleToTarget);

                if (mode == 1 && angles2[count] < 0)
                {
                    continue;
                }

                if (mode == 2 && angles2[count] > 0)
                {
                    continue;
                }
            }
        }
        else
        {
            curPos.vx = posX + Q12_MULT(angleMin, Math_Sin(g_SysWork.playerWork.player.rotation.vy));
            curPos.vz = posZ + Q12_MULT(angleMin, Math_Cos(g_SysWork.playerWork.player.rotation.vy));
            if (Math_SqrMagCalcToQ6(distMin) < (Math_SqrMagCalcToQ6(curPos.vx - boxPos.vx) +
                                                Math_SqrMagCalcToQ6(curPos.vz - boxPos.vz)))
            {
                continue;
            }

            angles2[count] = Math_AngleNormalizeSigned(ratan2(boxPos.vx - posX, boxPos.vz - posZ) - g_SysWork.playerWork.player.rotation.vy);

            if (mode != 3)
            {
                angles1[count] = Math_Vector2MagCalcSafeQ6(boxPos.vx - posX, boxPos.vz - posZ);
            }

            #undef curNpc
        }

        angles0[count] = ratan2(Math_Vector2MagCalcSafeQ6(boxPos.vx - posX, boxPos.vz - posZ), boxPos.vy - posY);
        if (angles0[count] < Q12_ANGLE(0.0f))
        {
            continue;
        }

        if (angles0[count] > Q12_ANGLE(180.0f))
        {
            continue;
        }

        if (mode < 1 || mode >= 3)
        {
            if (g_SysWork.targetNpcIdx == i)
            {
                *npcIdx = i;
                *angle = angles0[count];
                return;
            }
        }

        npcIdxs[count] = i;
        count++;
    }

    if (count == 0)
    {
        return;
    }

    for (i = 0; i < count; i++)
    {
        for (j = i + 1; j < count; j++)
        {
            if ((!Chara_HasFlag(&g_SysWork.npcs[npcIdxs[i]], 1 << 1) && !Chara_HasFlag(&g_SysWork.npcs[npcIdxs[j]], 1 << 1)) ||
                ( Chara_HasFlag(&g_SysWork.npcs[npcIdxs[i]], 1 << 1) &&  Chara_HasFlag(&g_SysWork.npcs[npcIdxs[j]], 1 << 1)))
            {
                switch (mode)
                {
                    case 0:
                    case 3:
                        if (ABS(angles2[i]) > ABS(angles2[j]))
                        {
                            break;
                        }
                        continue;

                    case 1:
                        if (angles2[i] > angles2[j])
                        {
                            break;
                        }
                        continue;

                    case 2:
                        if (angles2[i] < angles2[j])
                        {
                            break;
                        }
                        continue;

                    case 4:
                        if ((Math_SqrMagCalcToQ6(angles1[i]) * abs(angles2[i])) > (Math_SqrMagCalcToQ6(angles1[j]) * abs(angles2[j])))
                        {
                            break;
                        }
                        continue;

                    case 5:
                        if (angles1[i] > angles1[j])
                        {
                            break;
                        }
                        continue;

                    default:
                        continue;
                }
            }
            else if (!Chara_HasFlag(&g_SysWork.npcs[npcIdxs[i]], 2))
            {
                continue;
            }

            if (mode > 3 && mode < 6)
            {
                swap       = angles1[i];
                angles1[i] = angles1[j];
                angles1[j] = swap;
            }

            swap       = angles2[i];
            angles2[i] = angles2[j];
            angles2[j] = swap;

            swap       = angles0[i];
            angles0[i] = angles0[j];
            angles0[j] = swap;

            swap       = npcIdxs[i];
            npcIdxs[i] = npcIdxs[j];
            npcIdxs[j] = swap;
        }

        j         = npcIdxs[i];
        offset.vx = (g_SysWork.npcs[j].position.vx + g_SysWork.npcs[j].collision.shapeOffsets.box.vx) - posX;
        offset.vy = (g_SysWork.npcs[j].position.vy + g_SysWork.npcs[j].collision.box.offsetY)         - posY;
        offset.vz = (g_SysWork.npcs[j].position.vz + g_SysWork.npcs[j].collision.shapeOffsets.box.vz) - posZ;

        if (Ray_GeomAndCharaTraceGet(&trace, &from, &offset, &g_SysWork.playerWork.player) &&
            trace.character == &g_SysWork.npcs[j])
        {
            break;
        }
    }

    if (i < count)
    {
        *npcIdx = npcIdxs[i];
        *angle  = angles0[i];
    }
}

bool func_8005D50C(s32* targetNpcIdx, q3_12* rotToTargetX, q3_12* rotToTargetY, const VECTOR3* attackPos,
                   u32 npcIdx, q19_12 angleConstraint) // 0x8005D50C
{
    s_RayTrace trace;
    VECTOR3    offsetToTarget; // Q19.12
    q3_12      rotToTargetX0;
    q3_12      rotToTargetY0;
    q3_12      curRotToTargetY;
    q3_12      curRotToTargetX;
    q19_12     targetDist;
    q19_12     curDistToTarget;
    s32        i;

    #define npc g_SysWork.npcs[npcIdx]

    // Check if NPC index is valid.
    if (npcIdx >= ARRAY_SIZE(g_SysWork.npcs))
    {
        return false;
    }

    offsetToTarget.vx = (npc.position.vx + npc.collision.shapeOffsets.box.vx) - attackPos->vx;
    offsetToTarget.vy = (npc.position.vy + npc.collision.box.offsetY)         - attackPos->vy;
    offsetToTarget.vz = (npc.position.vz + npc.collision.shapeOffsets.box.vz) - attackPos->vz;

    // Compute rotation to target.
    targetDist    = Math_Vector2MagCalcSafeQ6(offsetToTarget.vx, offsetToTarget.vz);
    rotToTargetY0 = ratan2(offsetToTarget.vx, offsetToTarget.vz);
    rotToTargetX0 = ratan2(targetDist, offsetToTarget.vy);

    *targetNpcIdx = npcIdx;
    *rotToTargetX = rotToTargetX0;
    *rotToTargetY = rotToTargetY0;

    // Run through NPCs to find closer targetable NPC.
    for (i = 0; i < ARRAY_SIZE(g_SysWork.npcs); i++)
    {
        #define curNpc g_SysWork.npcs[i]

        // Check if NPC is valid.
        if (curNpc.model.charaId == Chara_None || curNpc.health < Q12(0.0f) || i == npcIdx)
        {
            continue;
        }

        // Compute offset.
        offsetToTarget.vx = (curNpc.position.vx + curNpc.collision.shapeOffsets.box.vx) - attackPos->vx;
        offsetToTarget.vy = (curNpc.position.vy + curNpc.collision.box.offsetY)         - attackPos->vy;
        offsetToTarget.vz = (curNpc.position.vz + curNpc.collision.shapeOffsets.box.vz) - attackPos->vz;

        // Check if Y-axis rotation to new target is within range.
        curRotToTargetY = ratan2(offsetToTarget.vx, offsetToTarget.vz);
        if (angleConstraint < ABS(Math_AngleNormalizeSigned(rotToTargetY0 - curRotToTargetY)))
        {
            continue;
        }

        // Check if distance is closer.
        curDistToTarget = Math_Vector2MagCalcSafeQ6(offsetToTarget.vx, offsetToTarget.vz);
        if (targetDist < curDistToTarget)
        {
            continue;
        }

        // Check if X-axis rotation to new target is within range.
        curRotToTargetX = ratan2(curDistToTarget, offsetToTarget.vy);
        if (angleConstraint < ABS(Math_AngleNormalizeSigned(rotToTargetX0 - curRotToTargetX)))
        {
            continue;
        }

        // Track closer targetable NPC.
        if (Ray_GeomAndCharaTraceGet(&trace, attackPos, &offsetToTarget, &g_SysWork.playerWork.player) &&
            trace.character == &g_SysWork.npcs[i])
        {
            *targetNpcIdx = i;
            *rotToTargetX = curRotToTargetX;
            targetDist    = curDistToTarget;
            *rotToTargetY = curRotToTargetY;
        }

        #undef curNpc
    }

    return true;

    #undef npc
}
