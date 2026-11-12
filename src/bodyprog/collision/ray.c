#include "bodyprog/game.h"
#include "inline_no_dmpsx.h"

#include <psyq/libapi.h>
#include <psyq/strings.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/collision/ray.h"
#include "bodyprog/collision/collision.h"
#include "bodyprog/math/math.h"
#include "main/rng.h"

#define g_RayQueryCache (s_RayQuery*)PSX_SCRATCH

bool Ray_GeomTraceGet(s_RayTrace* trace, const VECTOR3* from, const VECTOR3* to) // 0x8006D90C
{
    s32         prevScratchAddr;
    s_RayQuery* query;
    VECTOR3     offset; // Q19.12

    // Compute end offset.
    offset.vx = to->vx - from->vx;
    offset.vy = to->vy - from->vy;
    offset.vz = to->vz - from->vz;
    
    // Run ray trace.
    trace->hasHit = false;
    if (Ray_TraceSetup(g_RayQueryCache, false, Q8(0.0f), from, &offset, Q12(0.0f), Q12(0.0f), NULL, 0))
    {
        prevScratchAddr = SetSp((s32)PSX_SCRATCH_ADDR(0x3D8));

        query         = g_RayQueryCache;
        trace->hasHit = Ray_TraceRun(trace, query);

        SetSp(prevScratchAddr);
    }

    // Set missed ray if no obstacle was hit.
    if (!trace->hasHit)
    {
        Ray_MissSet(trace, from, &offset, query->rayDistance);
    }

    return trace->hasHit;
}

bool Ray_GeomAndCharaTraceGet(s_RayTrace* trace, const VECTOR3* from, const VECTOR3* offset,
                              const s_SubCharacter* excludedChara) // 0x8006DA08
{
    s32              collCharaCount;
    s32              prevScratchAddr;
    s_RayQuery*      query;
    s_SubCharacter** collCharas;

    // Get collidable characters.
    collCharas = Collision_CollidableCharasGet(&collCharaCount, excludedChara, false);

    // Run ray trace.
    trace->hasHit = false;
    if (Ray_TraceSetup(g_RayQueryCache, false, Q8(0.0f), from, offset, Q12(0.0f), Q12(0.0f), collCharas, collCharaCount))
    {
        prevScratchAddr = SetSp((s32)PSX_SCRATCH_ADDR(0x3D8));

        query         = g_RayQueryCache;
        trace->hasHit = Ray_TraceRun(trace, query);

        SetSp(prevScratchAddr);
    }

    // Set missed ray if no obstacle was hit.
    if (!trace->hasHit)
    {
        Ray_MissSet(trace, from, offset, query->rayDistance);
    }

    return trace->hasHit;
}

void Ray_MissSet(s_RayTrace* trace, const VECTOR3* from, const VECTOR3* offset, q23_8 hitDist) // 0x8006DAE4
{
    trace->hasHit       = false;
    trace->floorType    = FloorType_Default;
    trace->target.vx    = from->vx + offset->vx;
    trace->target.vy    = from->vy + offset->vy;
    trace->target.vz    = from->vz + offset->vz;
    trace->character    = NULL;
    trace->hitDistance  = Q8_TO_Q12(hitDist);
    trace->floorHeight  = Q12(1.875f); // @bug? Awkward value suggests `Q8(DEFAULT_FLOOR_HEIGHT)` may have been typed by mistake.
    trace->headingAngle = Q12_ANGLE(0.0f);
}

bool Ray_GeomAndCharaTraceGetAlt(s_RayTrace* trace, const VECTOR3* from, const VECTOR3* offset,
                                 s_SubCharacter* excludedChara) // 0x8006DB3C
{
    s32              collCharaCount;
    s32              prevScratchAddr;
    s_RayQuery*      query;
    s_SubCharacter** collCharas;

    // Get collidable characters.
    collCharas = Collision_CollidableCharasGet(&collCharaCount, excludedChara, true);

    // Run ray trace.
    trace->hasHit = false;
    if (Ray_TraceSetup(g_RayQueryCache, true, Q8(0.0f), from, offset, Q12(0.0f), Q12(0.0f), collCharas, collCharaCount))
    {
        prevScratchAddr = SetSp((s32)PSX_SCRATCH_ADDR(0x3D8));

        query         = g_RayQueryCache;
        trace->hasHit = Ray_TraceRun(trace, query);

        SetSp(prevScratchAddr);
    }

    // Set missed ray if no obstacle was hit.
    if (!trace->hasHit)
    {
        Ray_MissSet(trace, from, offset, query->rayDistance);
    }

    return trace->hasHit;
}

bool func_8006DC18(s_RayTrace* trace, const VECTOR3* from, const VECTOR3* offset) // 0x8006DC18
{
    s32         prevScratchAddr;
    s_RayQuery* query;

    trace->hasHit = false;
    if (Ray_TraceSetup((s32)PSX_SCRATCH, true, Q8(0.3f), from, offset, Q12(0.0f), Q12(0.0f), NULL, 0))
    {
        prevScratchAddr = SetSp((s32)PSX_SCRATCH_ADDR(0x3D8));

        query         = g_RayQueryCache;
        trace->hasHit = Ray_TraceRun(trace, query);

        SetSp(prevScratchAddr);
    }

    if (!trace->hasHit)
    {
        Ray_MissSet(trace, from, offset, query->rayDistance);
    }

    return trace->hasHit;
}

bool Ray_TraceSetup(s_RayQuery* query, bool ignoreFloors, q7_8 radiusOffset, const VECTOR3* from, const VECTOR3* offset,
                    q19_12 unusedX, q19_12 unusedZ,
                    s_SubCharacter** collCharas, s32 collCharaCount)
{
    if (offset->vx == Q12(0.0f) && offset->vz == Q12(0.0f))
    {
        return false;
    }

    query->ignoreFloors      = ignoreFloors;
    query->triggerFlags = g_ActiveCollisionTriggers.flags;
    query->radius       = radiusOffset;
    query->hitDistance  = SHRT_MAX;
    query->character    = NULL;

    query->from.vx = Q12_TO_Q8(from->vx);
    query->from.vy = Q12_TO_Q8(from->vy);
    query->from.vz = Q12_TO_Q8(from->vz);

    query->offset.vx = Q12_TO_Q8(offset->vx);
    query->offset.vy = Q12_TO_Q8(offset->vy);
    query->offset.vz = Q12_TO_Q8(offset->vz);

    query->field_3C = query->from.vx + query->offset.vx;

    query->field_4C = Q12_TO_Q8(unusedX);
    query->field_4E = Q12_TO_Q8(unusedZ);

    query->field_40 = query->from.vy + query->offset.vy;
    query->field_44 = query->from.vz + query->offset.vz;

    query->rayDistance = Math_Vector2MagCalc(query->offset.vx, query->offset.vz);
    if (query->rayDistance == Q8(0.0f))
    {
        return false;
    }

    query->direction.vx = Q12_DIV(query->offset.vx, query->rayDistance);
    query->direction.vz = Q12_DIV(query->offset.vz, query->rayDistance);

    if (query->offset.vy < 0)
    {
        query->field_5E = query->from.vy + query->field_4E;
        query->field_60 = query->field_40 + query->field_4E;
    }
    else
    {
        query->field_60 = query->from.vy + query->field_4E;
        query->field_5E = query->field_40 + query->field_4E;
    }

    query->collidableCharas     = collCharas;
    query->collidableCharaCount = collCharaCount;

    return true;
}

bool Ray_TraceRun(s_RayTrace* trace, s_RayQuery* query) // 0x8006DEB0
{
    s32                  collDataIdx;
    s32                  temp_lo;
    s_IpdCollisionData*  collData;
    s_SubCharacter**     curCollChara;
    s_IpdCollisionData** collDataPtrs;
    s_IpdCollisionData** curCollData;
    s_RayQuery_8C*       curUnk;

    // Run through collision data.
    collDataPtrs = WorldMap_ActiveChunksCollisionDataGet(&collDataIdx);
    for (curCollData = collDataPtrs; curCollData < &collDataPtrs[collDataIdx]; curCollData++)
    {
        collData = *curCollData;

        if (collData->polygonSurfaceCount != 0 ||
            collData->polygonCount        != 0 ||
            collData->cylinderCount       != 0)
        {
            func_8006E0AC(query, collData);
            Collision_ShapePassReset(collData);

            for (curUnk = &query->field_8C; curUnk < &query->field_8C[query->field_88]; curUnk++)
            {
                temp_lo = curUnk->field_2 * query->cellCountX;
                func_8006E53C(query, &collData->shapeAllocs[temp_lo + curUnk->field_0], collData);
            }
        }
    }

    // Run through collidable characters.
    for (curCollChara = query->collidableCharas;
         curCollChara < &query->collidableCharas[query->collidableCharaCount];
         curCollChara++)
    {
        func_8006EE0C(&query->field_6C, query->ignoreFloors, *curCollChara);
        func_8006EEB8(query, *curCollChara);
    }

    if (query->hitDistance != SHRT_MAX)
    {
        trace->target.vx    = Q8_TO_Q12(query->target.vx);
        trace->target.vy    = Q8_TO_Q12(query->target.vy);
        trace->target.vz    = Q8_TO_Q12(query->target.vz);
        trace->character    = query->character;
        trace->hitDistance  = Q8_TO_Q12(query->hitDistance);
        trace->floorHeight  = Q8_TO_Q12(query->floorHeight);
        trace->headingAngle = ratan2(query->offsetX, query->offsetZ);
        trace->floorType    = query->floorType;
        return true;
    }

    return false;
}

void func_8006E0AC(s_RayQuery* query, const s_IpdCollisionData* collData) // 0x8006E0AC
{
    query->field_6C.positionX         = collData->positionX;
    query->field_6C.positionZ         = collData->positionZ;
    query->field_6C.u.unk.offset.vx   = query->from.vx - query->field_6C.positionX;
    query->field_6C.u.unk.offset.vz   = query->from.vz - query->field_6C.positionZ;
    query->field_6C.u.unk.offset_C.vx = query->field_6C.u.unk.offset.vx + query->offset.vx;
    query->field_6C.u.unk.offset_C.vz = query->field_6C.u.unk.offset.vz + query->offset.vz;
    query->cellCountX                 = collData->cellCountX;
    query->cellCountZ                 = collData->cellCountZ;
    query->cellSize                   = collData->cellSize;

    // TODO: Use `DVECTOR_XZ`?
    func_8006E150(&query->field_6C, ((DVECTOR*)&query->offset)[0], ((DVECTOR*)&query->offset)[1]);
}

void func_8006E150(s_func_8006E490* arg0, DVECTOR arg1, DVECTOR arg2) // 0x8006E150
{
    DVECTOR subroutine_arg4;
    VECTOR  sp18;
    VECTOR  pos; // TODO: Wrong struct, `pad` is also used.
    s16     temp_lo;
    s16     temp_lo_2;
    s32     temp_a0_3;
    s32     temp_lo_4;
    s32     temp_t0;
    s32     temp_t1;
    s32     var_a2;
    s32     var_a3;
    u32     flags;

    flags = OrientationFlags_None;
    arg0->field_1C = Q8(0.0f);

    if (arg0->field_8.vx < 0 && arg0->field_C < 0 &&
        arg0->field_8.vy < 0 && arg0->field_E < 0)
    {
        return;
    }

    temp_lo   = arg0->field_10 * arg0->field_18;
    temp_lo_2 = arg0->field_14 * arg0->field_18;

    if (arg0->field_8.vx >= temp_lo && arg0->field_C >= temp_lo &&
        arg0->field_8.vy >= temp_lo_2 && arg0->field_E >= temp_lo_2)
    {
        return;
    }

    subroutine_arg4 = arg0->field_8;

    sp18.vz  = 0;
    sp18.pad = 0;

    sp18.vx = Q12(arg0->field_10);
    sp18.vy = Q12(arg0->field_14);

    if (arg1.vx < 0)
    {
        flags             |= OrientationFlags_InvertX;
        subroutine_arg4.vx = -subroutine_arg4.vx;

        arg1.vx = -arg1.vx;
        sp18.vz = -sp18.vx;
        sp18.vx = 0;
    }

    if (arg2.vx < 0)
    {
        flags             |= OrientationFlags_InvertZ;
        subroutine_arg4.vy = -subroutine_arg4.vy;
        arg2.vx            = -arg2.vx;
        sp18.pad           = -sp18.vy;
        sp18.vy            = 0;
    }

    if (arg1.vx < arg2.vx)
    {
        flags |= OrientationFlags_SwapXz;

        temp_a0_3          = subroutine_arg4.vx;
        subroutine_arg4.vx = subroutine_arg4.vy;
        subroutine_arg4.vy = temp_a0_3;

        temp_a0_3 = arg1.vx;
        arg1.vx   = arg2.vx;
        arg2.vx   = temp_a0_3;

        temp_a0_3 = sp18.vz;
        sp18.vz   = sp18.pad;
        sp18.pad  = temp_a0_3;
        temp_a0_3 = sp18.vx;
        sp18.vx   = sp18.vy;
        sp18.vy   = temp_a0_3;
    }

    if (subroutine_arg4.vx + arg1.vx < Q12_MULT(arg0->field_18, sp18.vx))
    {
        sp18.vx = Q12(subroutine_arg4.vx + arg1.vx) / arg0->field_18;
    }

    pos.vx = Q12(subroutine_arg4.vx) / arg0->field_18;
    pos.vy = Q12(subroutine_arg4.vy) / arg0->field_18;
    pos.vz = Q12(1.0f);

    pos.pad  = Q12(arg2.vx) / arg1.vx;
    temp_lo_4 = Q12_MULT(pos.pad, Q12_FRACT(pos.vx));

    if (FP_FROM(sp18.vx, Q12_SHIFT) < FP_FROM(pos.vx, Q12_SHIFT))
    {
        return;
    }

    do
    {
        func_8006E490(arg0, flags, pos.vx, pos.vy);

        temp_t0 = pos.vy;
        temp_t1 = pos.vx;
        var_a3  = temp_t0 + pos.pad;
        var_a2  = temp_t1 + Q12(1.0f);

        pos.vy = var_a3;
        pos.vx = var_a2;

        if (FP_FROM(temp_t0, Q12_SHIFT) < FP_FROM(var_a3, Q12_SHIFT))
        {
            if (Q12_FRACT(var_a3) < temp_lo_4)
            {
                func_8006E490(arg0, flags, var_a2, temp_t0);
            }
            else
            {
                func_8006E490(arg0, flags, temp_t1, var_a3);
            }
        }
    }
    while (arg0->field_1C < Q8(0.08f) && FP_FROM(pos.vx, Q12_SHIFT) <= FP_FROM(sp18.vx, Q12_SHIFT));
}

void func_8006E490(s_func_8006E490* arg0, u32 flags, q19_12 posX, q19_12 posZ) // 0x8006E490
{
    q19_12 tempPosX;

    if (flags & OrientationFlags_SwapXz)
    {
        tempPosX = posX;
        posX     = posZ;
        posZ     = tempPosX;
    }

    if (flags & OrientationFlags_InvertZ)
    {
        posZ = -posZ;
    }

    if (flags & OrientationFlags_InvertX)
    {
        posX = -posX;
    }

    posX = FP_FROM(posX, Q12_SHIFT);
    posZ = FP_FROM(posZ, Q12_SHIFT);
    if (posX >= Q12(0.0f) && posX < arg0->field_10 &&
        posZ >= Q12(0.0f) && posZ < arg0->field_14)
    {
        arg0->field_20[arg0->field_1C].vx = posX;
        arg0->field_20[arg0->field_1C].vz = posZ;
        arg0->field_1C++;
    }
}

void func_8006E53C(s_RayQuery* query, s_IpdShapeAlloc* shapeAlloc, s_IpdCollisionData* collData) // 0x8006E53C
{
    s32            i;
    bool           hasSurface;
    bool           cond1;
    bool           cond2;
    s32            surfaceIdx0;
    s32            surfaceIdx1;
    s32            shapeIdx;
    s_IpdCylinder* curCylinder;
    s_IpdPolygon*  curPolygon;

    // Run through allocated cell shapes.
    for (i = shapeAlloc[0].shapeLutIdx; i < shapeAlloc[1].shapeLutIdx; i++)
    {
        shapeIdx = collData->shapeLut[i];

        // Check if shape was already analyzed.
        if (collData->shapePassStampBuffer[shapeIdx] > collData->shapePassCounter)
        {
            continue;
        }
        collData->shapePassStampBuffer[shapeIdx] = collData->shapePassCounter + 1;

        // Handle polygon.
        if (shapeIdx < collData->polygonCount)
        {
            // Check if polygon is collidable.
            curPolygon = &collData->polygons[shapeIdx];
            if (CHECK_FLAG_IDX(query->triggerFlags, (curPolygon->triggerFlagIdxHigh << 2) | curPolygon->triggerFlagIdxLow, false))
            {
                continue;
            }

            // Get surfaces.
            surfaceIdx0 = curPolygon->surfaceIdx0;
            surfaceIdx1 = curPolygon->surfaceIdx1;
            hasSurface  = surfaceIdx0 != UCHAR_MAX && surfaceIdx1 != UCHAR_MAX;

            if (query->ignoreFloors == true)
            {
                if (hasSurface)
                {
                    continue;
                }
            }
            else
            {
                cond1 = false;
                cond2 = false;

                if (surfaceIdx0 == UCHAR_MAX ||
                    collData->polygonSurfaces[surfaceIdx0].floorType == FloorType_Default ||
                    collData->polygonSurfaces[surfaceIdx0].floorType == FloorType_None)
                {
                    cond1 = true;
                }

                if (surfaceIdx1 == UCHAR_MAX ||
                    collData->polygonSurfaces[surfaceIdx1].floorType == FloorType_Default ||
                    collData->polygonSurfaces[surfaceIdx1].floorType == FloorType_None)
                {
                    cond2 = true;
                }

                if (cond1 && cond2)
                {
                    continue;
                }
            }

            func_8006E78C(query, curPolygon, collData->polygonSplitVertices, collData->polygonSurfaces, hasSurface);
        }
        // Handle cylinder.
        else
        {
            curCylinder = &collData->cylinders[shapeIdx - collData->polygonCount];

            if (CHECK_FLAG_IDX(query->triggerFlags, curCylinder->triggerFlagIdx, true) &&
                (query->ignoreFloors == true ||
                    (curCylinder->floorType != FloorType_Default &&
                    curCylinder->floorType != FloorType_None)) &&
                curCylinder->radius >= query->radius)
            {
                func_8006EB8C(query, curCylinder);
            }
        }
    }
}

void func_8006E78C(s_RayQuery* query, s_IpdPolygon* polygon, SVECTOR3* splitVerts, s_IpdSurface* surfaces, bool hasSurface) // 0x8006E78C
{
    SVECTOR   sp0;
    SVECTOR   sp8;
    SVECTOR3  maybeSplitDiff; // Q23.8
    q23_8     hitDist;
    q19_12    offsetY;
    q19_12    offsetX;
    q23_8     maybeRayStep;
    s32       floorType; // `e_FloorType`
    SVECTOR3* splitVert1;
    SVECTOR3* splitVert0;
    s32       unkXz;
    s32       var_v1;

    floorType = FloorType_Default;
    splitVert1 = &splitVerts[polygon->splitVertexIdx1];
    splitVert0 = &splitVerts[polygon->splitVertexIdx0];

    if (query->field_5E >= splitVert1->vy || query->field_5E >= splitVert0->vy)
    {
        if (polygon->surfaceIdx0 != UCHAR_MAX)
        {
            floorType = surfaces[polygon->surfaceIdx0].floorType;
        }
        if (polygon->surfaceIdx1 != UCHAR_MAX)
        {
            floorType = surfaces[polygon->surfaceIdx1].floorType;
        }

        unkXz = PACKED_XZ16(query->direction.vx, query->direction.vz);
        gte_ldR11R12(unkXz);
        gte_ldR13R21(unkXz);
        gte_ldvxy0(PACKED_XZ16(splitVert1->vx - query->field_6C.u.unk.offset.vx, splitVert1->vz - query->field_6C.u.unk.offset.vz));
        gte_ldvz0();
        gte_rtv0();
        gte_stMAC12(&sp0);

        gte_ldvxy0(PACKED_XZ16(splitVert0->vx - query->field_6C.u.unk.offset.vx, splitVert0->vz - query->field_6C.u.unk.offset.vz));
        gte_ldvz0();
        gte_rtv0();
        gte_stMAC12(&sp8);

        if ((sp0.vy & 0x8000) != (sp8.vy & 0x8000))
        {
            if (query->ignoreFloors == true)
            {
                gte_ldsxy3(0, *(s32*)&sp0.vx, *(s32*)&sp8.vx);
                gte_nclip();

                if (gte_stMAC0() >= 0)
                {
                    if (polygon->surfaceIdx0 != UCHAR_MAX)
                    {
                        return;
                    }
                }
                else
                {
                    if (polygon->surfaceIdx1 != UCHAR_MAX)
                    {
                        return;
                    }
                }
            }

            if (sp0.vy != sp8.vy)
            {
                var_v1 = ((sp0.vy << 12) / (sp0.vy - sp8.vy));
                hitDist = (((sp8.vx - sp0.vx) * var_v1) >> 12) + sp0.vx;
                if (hitDist >= 0 && query->rayDistance >= hitDist)
                {
                    gte_lddp(var_v1);
                    gte_ldsv3_(splitVert0->vx - splitVert1->vx, splitVert0->vy - splitVert1->vy, splitVert0->vz - splitVert1->vz);
                    gte_gpf12();
                    gte_stsv(&maybeSplitDiff);

                    maybeSplitDiff.vx += splitVert1->vx;
                    maybeSplitDiff.vy += splitVert1->vy;
                    maybeSplitDiff.vz += splitVert1->vz;

                    maybeRayStep = query->from.vy + query->field_4E;
                    if (query->offset.vy != Q8(0.0f))
                    {
                        maybeRayStep += (query->offset.vy * hitDist) / query->rayDistance;
                    }

                    if (maybeRayStep >= maybeSplitDiff.vy && hitDist < query->hitDistance)
                    {
                        offsetY =  polygon->offsetY;
                        offsetX = -polygon->offsetX;
                        if (query->ignoreFloors != true && hasSurface && (sp8.vy - sp0.vy) > 0)
                        {
                            offsetY = -offsetY;
                            offsetX = polygon->offsetX;
                        }

                        query->hitDistance = hitDist;
                        query->target.vx   = maybeSplitDiff.vx + query->field_6C.positionX;
                        query->target.vy   = maybeRayStep - query->field_4E;
                        query->target.vz   = maybeSplitDiff.vz + query->field_6C.positionZ;
                        query->floorHeight = maybeSplitDiff.vy;
                        query->offsetX     = offsetY;
                        query->offsetZ     = offsetX;
                        query->character   = NULL;
                        query->floorType   = floorType;
                    }
                }
            }
        }
    }
}

void func_8006EB8C(s_RayQuery* query, s_IpdCylinder* cylinder) // 0x8006EB8C
{
    SVECTOR sp10;
    SVECTOR sp18; // Q7.8
    q7_8    hitDist;
    q23_8   temp_v0;
    q7_8    radius;
    s32     temp_v1;

    radius = cylinder->radius;
    if (query->field_5E <= cylinder->offset.vy)
    {
        return;
    }

    temp_v1 = PACKED_XZ16(query->direction.vx, query->direction.vz);
    gte_ldR11R12(temp_v1);
    gte_ldR13R21(temp_v1);
    gte_ldvxy0(PACKED_XZ16(cylinder->offset.vx - query->field_6C.u.unk.offset.vx, cylinder->offset.vz - query->field_6C.u.unk.offset.vz));
    gte_ldvz0();
    gte_rtv0();
    gte_stMAC12(&sp10);

    if (-radius < sp10.vx && sp10.vx < (query->rayDistance + radius) &&
        -radius < sp10.vy && sp10.vy < radius)
    {
        temp_v0   = SquareRoot0(SQUARE(radius) - SQUARE(sp10.vy));
        hitDist = sp10.vx - temp_v0;

        if (hitDist >= -temp_v0 && query->rayDistance >= hitDist && hitDist < query->hitDistance)
        {
            gte_lddp(Q12(hitDist) / query->rayDistance);
            gte_ldsv3_(query->offset.vx, query->offset.vy, query->offset.vz);
            gte_gpf12();
            gte_stsv(&sp18);

            if ((sp18.vy + query->from.vy + query->field_4E) >= cylinder->offset.vy)
            {
                query->hitDistance = hitDist;
                query->target.vx   = sp18.vx + query->field_6C.u.unk.offset.vx + query->field_6C.positionX;
                query->target.vy   = sp18.vy + query->from.vy;
                query->target.vz   = sp18.vz + query->field_6C.u.unk.offset.vz + query->field_6C.positionZ;
                query->floorHeight = cylinder->offset.vy;
                query->offsetX     = (sp18.vx + query->field_6C.u.unk.offset.vx) - cylinder->offset.vx;
                query->offsetZ     = (sp18.vz + query->field_6C.u.unk.offset.vz) - cylinder->offset.vz;
                query->character   = NULL;
                query->floorType   = cylinder->floorType;
            }
        }
    }
}

void func_8006EE0C(s_RayQuery_6C* arg0, bool ignoreFloors, const s_SubCharacter* chara) // 0x8006EE0C
{
    q19_12 offsetX;
    q19_12 offsetZ;
    q19_12 top;

    if (ignoreFloors == true)
    {
        arg0->u.shape.radius = Q12_TO_Q8(chara->collision.cylinder.outerRadius);
        offsetX              = chara->collision.shapeOffsets.cylinder.vx;
        offsetZ              = chara->collision.shapeOffsets.cylinder.vz;
        top                  = chara->position.vy + chara->collision.box.bottomExtent;
    }
    else
    {
        arg0->u.shape.radius = Q12_TO_Q8(chara->collision.cylinder.innerRadius);
        offsetX              = chara->collision.shapeOffsets.box.vx;
        offsetZ              = chara->collision.shapeOffsets.box.vz;
        top                  = chara->position.vy + chara->collision.box.height;
    }

    arg0->u.shape.topHeight   = Q12_TO_Q8(top);
    arg0->positionX           = Q12_TO_Q8(chara->position.vx + offsetX);
    arg0->positionZ           = Q12_TO_Q8(chara->position.vz + offsetZ);
    arg0->u.shape.floorHeight = Q12_TO_Q8(chara->position.vy + chara->collision.box.topExtent);
}

void func_8006EEB8(s_RayQuery* query, s_SubCharacter* chara) // 0x8006EEB8
{
    VECTOR3 offset; // Q23.8
    s32     bound;
    q3_12   hitAlpha;
    q3_12   clampedRayDist;
    q23_8   x1;
    q23_8   z1;
    q23_8   x0;
    q23_8   z0;
    q19_12  var_v1;

    if (query->from.vx <= query->field_3C)
    {
        x0 = query->from.vx;
        z0 = query->field_3C;
    }
    else
    {
        x0 = query->field_3C;
        z0 = query->from.vx;
    }

    if (query->from.vz <= query->field_44)
    {
        z1 = query->from.vz;
        x1 = query->field_44;
    }
    else
    {
        z1 = query->field_44;
        x1 = query->from.vz;
    }

    bound = query->field_6C.u.shape.radius;
    if ((query->field_6C.positionX + bound) < x0 ||
         z0 < (query->field_6C.positionX - bound))
    {
        return;
    }

    if ((query->field_6C.positionZ + bound) < z1 || x1 < (query->field_6C.positionZ - bound) ||
        ((query->from.vy + query->field_4E) < query->field_6C.u.shape.floorHeight && (query->field_40 + query->field_4E) < query->field_6C.u.shape.floorHeight) ||
        ((query->from.vy + query->field_4C) > query->field_6C.u.shape.topHeight && query->field_6C.u.shape.topHeight < (query->field_40 + query->field_4C)))
    {
        return;
    }

    hitAlpha = Collision_HitAlphaGet(PACKED_MOVE_DIRECTION(query->direction), query->rayDistance,
                                     query->field_6C.positionX - query->from.vx,
                                     query->field_6C.positionZ - query->from.vz,
                                     bound);
    if (hitAlpha == NO_VALUE)
    {
        return;
    }

    clampedRayDist = Q12_MULT(query->rayDistance, hitAlpha);
    if (clampedRayDist >= query->hitDistance)
    {
        return;
    }

    offset.vy = query->from.vy + (Q12_MULT(query->offset.vy, hitAlpha));
    if ((offset.vy + query->field_4E) < query->field_6C.u.shape.floorHeight ||
        query->field_6C.u.shape.topHeight < (offset.vy + query->field_4C))
    {
        if (query->offset.vy == Q8(0.0f))
        {
            return;
        }

        if ((offset.vy + query->field_4E) < query->field_6C.u.shape.floorHeight)
        {
            var_v1 = Q12(query->field_6C.u.shape.floorHeight - (query->from.vy + query->field_4E)) / query->offset.vy;
            if (var_v1 > Q12(1.0f))
            {
                return;
            }
            offset.vy = query->field_6C.u.shape.floorHeight - query->field_4E;
        }
        else
        {
            var_v1 = Q12(query->field_6C.u.shape.topHeight - (query->from.vy + query->field_4C)) / query->offset.vy;
            if (var_v1 > Q12(1.0f))
            {
                return;
            }
            offset.vy = query->field_6C.u.shape.topHeight - query->field_4C;
        }

        offset.vx = query->from.vx + Q12_MULT(query->offset.vx, var_v1);
        offset.vz = query->from.vz + Q12_MULT(query->offset.vz, var_v1);
        if ((SQUARE(query->field_6C.positionX - offset.vx) + SQUARE(query->field_6C.positionZ - offset.vz)) >= SQUARE(query->field_6C.u.shape.radius))
        {
            return;
        }
    }
    else
    {
        offset.vx = query->from.vx + Q12_MULT(query->offset.vx, hitAlpha);
        offset.vz = query->from.vz + Q12_MULT(query->offset.vz, hitAlpha);
    }

    query->hitDistance = clampedRayDist;
    query->target.vx   = offset.vx;
    query->target.vy   = offset.vy;
    query->target.vz   = offset.vz;
    query->floorHeight = query->field_6C.u.shape.floorHeight;
    query->offsetX     = offset.vx - query->field_6C.positionX;
    query->offsetZ     = offset.vz - query->field_6C.positionZ;
    query->character   = chara;
    query->floorType   = FloorType_Default;
}
