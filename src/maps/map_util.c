#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"

// TODO: 
// - `Map_RoomIdxGet` uses defines from the `mapX_sXX.h` headers, the map .c that includes this file handles including that atm.
//   To split this .c properly it'd need to handle including the right map header though.
//
// - `Map_RoomBgmInit` should probably be part of this file too.
//   Right now it has tons of differences between maps though. Need to try and find a way to merge them.
//
// - `GameBoot_LoadScreen_StageString` could also be part of this as it seems to always come after `Map_RoomBgmInit`.
//
// - Might be a cleaner way to handle the `Map_RoomIdxGet` map differences?

s32 sharedFunc_800D929C_0_s00(void)
{
    return D_800A999C;
}

#if !defined(M2CTX) // Inlines cause issues with M2C context.

#define CheckRange(axis, low, high)    \
({                                     \
    s32 val = (axis);                  \
    (low) <= (val) && (val) <= (high); \
})

#define CheckNotInRange(axis, low, high)  \
({                                        \
    s32 val = (axis);                     \
    !((low) <= (val) && (val) <= (high)); \
})

#ifdef MAP5_S01
static inline u32 GetXIdx(q19_12 posX, q19_12 posZ)
{
    if (CheckRange(posX + Q12(38.5f), Q12(-16.5f), Q12(16.5f)) && posZ > Q12(-160.0f))
    {
        if (posZ < Q12(-120.0f) || (posZ < Q12(-24.0f) && posX < Q12(-32.0f)) ||
            CheckRange(posX + Q12(40.0f), Q12(-12.0f), Q12(12.0f)))
        {
            return 1;
        }
    }

    if (CheckRange(posX + Q12(-38.0f), Q12(-6.0f), Q12(6.0f)) && posZ < Q12(-40.0f))
    {
        return 2;
    }

    return 0;
}
#else
static inline s32 GetXIdx(q19_12 posX)
{
    if (CheckRange(posX + Q12(240.0f), Q12(-24.0f), Q12(24.0f)))
    {
        return 1;
    }

    if (CheckRange(posX + Q12(120.0f), Q12(-24.0f), Q12(24.0f)))
    {
        return 2;
    }

    if (CheckRange(posX + Q12(16.0f), Q12(0.0f), Q12(40.0f)))
    {
        return 3;
    }

    if (CheckRange(posX - Q12(120.0f), Q12(-24.0f), Q12(24.0f)))
    {
        return 4;
    }

    return 0;
}
#endif

static inline s32 GetYIdx(q19_12 posX, q19_12 posY)
{
    if (CheckRange(posY + Q12(202.0f), Q12(-18.0f), Q12(18.0f)))
    {
        return 1;
    }

    if (CheckRange(posY + Q12(79.0f), Q12(-15.0f), Q12(15.0f)) && posX < Q12(160.0f))
    {
        return 2;
    }

    if (CheckRange(posY + Q12(16.0f), 0, Q12(32.0f)))
    {
        return 3;
    }

    if (CheckRange(posY - Q12(124.0f), Q12(-20.0f), Q12(20.0f)))
    {
        return 4;
    }

    return 0;
}

u8 Map_RoomIdxGet(q19_12 posX, q19_12 posZ)
{
#if defined(MAP6_S04) || defined(MAP6_S05)
    s32 result;

    if (posZ > Q12(80.0f))
    {
        result = 5;
    }
    else if (Q12_SQUARE_PRECISE(posX + Q12(16.0f)) + Q12_SQUARE_PRECISE(posZ + Q12(64.0f)) < Q12(SQUARE(18)))
    {
        result = 3;
    }
    else if (Q12_SQUARE_PRECISE(posX + Q12(-14.0f)) + Q12_SQUARE_PRECISE(posZ + Q12(28.0f)) < Q12(SQUARE(18)))
    {
        result = 2;
    }
    else if (Q12_SQUARE_PRECISE(posX + Q12(14.0f)) + Q12_SQUARE_PRECISE(posZ + Q12(28.0f)) < Q12(SQUARE(5)))
    {
        result = 4;
    }
    else
    {
        result = 1;
    }

    return result;
#elif defined(MAP6_S03)
    s32 ret;

    if (posX < Q12(27.0f))
    {
        ret = 17;
        if (posX <= Q12(13.0f))
        {
            ret = 15;
            if (posX > Q12(-13.0f))
            {
                ret = 16;
            }
        }
    }
    else if (posX < Q12(56.0f))
    {
        ret = 18;
    }
    else
    {
        if (posX < Q12(72.0f))
        {
            ret = 19;
        }
        else
        {
            ret = 20;
        }
    }

    return ret;
#else
    extern u8 MAP_ROOM_IDXS[];

    s32 xIdx;
    s32 yIdx;
    u8  res = 0;

    // TODO: Used in map0_s00/map0_s01/map2_s00/map2_s03, maybe some kind of outdoor/exterior grid?
#ifdef MAP_HAS_SECONDARY_GRID
    extern u8 sharedData_800DF2DC_0_s00[];

    // Check if coordinates are within primary grid bounds.
    if (posX < Q12(-320.0f) || posX >= Q12(240.0f) ||
        posZ < Q12(-240.0f) || posZ >= Q12(200.0f))
    {
        res = 0;
    }
    else
    {
        // Determine XY indices in primary grid.
        xIdx = GetXIdx(posX);
        yIdx = GetYIdx(posX, posZ);
        res  = sharedData_800DF2DC_0_s00[(xIdx * 5) + yIdx];
    }
#elif defined(MAP2_S02) || defined(MAP4_S00) || defined(MAP4_S02) || defined(MAP4_S03) || \
      defined(MAP4_S05) || defined(MAP4_S06)
    if (CheckRange(posX + Q12(120.0f), Q12(-14.0f), Q12(14.0f)) && posZ < Q12(40.0f))
    {
        xIdx = 1;
    }
    else if (CheckRange(posX + Q12(60.0f), Q12(-9.0f), Q12(9.0f)) &&
             CheckRange(posZ + Q12(40.0f), Q12(-34.0f), Q12(34.0f)))
    {
        xIdx = 2;
    }
    else if (CheckRange(posX + Q12(0.0f), Q12(-14.0f), Q12(14.0f)) && posZ > Q12(-80.0f))
    {
        xIdx = 3;
    }
    else
    {
        xIdx = 0;
    }

    if (CheckNotInRange(posX + Q12(40.0f), Q12(-120.0f), Q12(120.0f)))
    {
        yIdx = 0;
    }
    else if (CheckRange(posZ + Q12(80.0f), Q12(-14.0f), Q12(14.0f)))
    {
        yIdx = 1;
    }
    else if (CheckRange(posZ + Q12(0.0f), Q12(-14.0f), Q12(14.0f)))
    {
        yIdx = 2;
    }
    else
    {
        yIdx = 0;
    }

    res = sharedData_800ED430_2_s02[(xIdx * 3) + yIdx];
#elif defined(MAP5_S00)
    res = 0;

    if (posX >= Q12(-40.0f))
    {
        if (CheckRange(posX, Q12(16.0), Q12(24.0)) && posZ > Q12(-8.0))
        {
            res = (posZ < Q12(48.0)) ? 2 : 1;
        }
        else if (CheckRange(posZ + Q12(12.0), Q12(-4.0), Q12(4.0)) && CheckRange(posX - Q12(40.0), Q12(-8.0), Q12(8.0)))
        {
            res = 3;
        }
        else if (CheckRange(posX, Q12(48.0), Q12(56.0)) && CheckRange(posZ + Q12(40.0), Q12(-8.0), Q12(8.0)))
        {
            res = 4;
        }
    }
    else if (CheckRange(posX, Q12(-64.0f), Q12(-56.0f)))
    {
        if (posZ > Q12(-8.0))
        {
            res = 9;
        }
        else if (CheckRange(posZ + Q12(52.0), Q12(-36.0), Q12(36.0)))
        {
            res = 8;
        }
    }
    else if (posX > Q12(-64.0) && posZ < Q12(-88.0))
    {
        res = 10;
    }
    else if (CheckRange(posZ, Q12(-94.0), Q12(-90.0)) && posX > Q12(-89.0))
    {
        res = 11;
    }
    else if (CheckRange(posX, Q12(-96.0), Q12(-88.0)) && CheckRange(posZ + Q12(46.0), Q12(-30.0), Q12(30.0)))
    {
        res = 13;
    }
#elif defined(MAP5_S01)
    xIdx = GetXIdx(posX, posZ);

    // TODO: Could this be a `GetYIdx` inline like one above? No luck changing it yet.
    yIdx = 0;
    if (CheckRange(posZ + Q12(163.0f), Q12(-18.0f), Q12(18.0f)))
    {
        yIdx = 1;
    }
    else if (posX > Q12(-40.0f))
    {
        if (posX > Q12(40.0f))
        {
            if (CheckRange(posZ + Q12(36.0f), Q12(-16.0f), Q12(16.0f)))
            {
                yIdx = 2;
            }
        }
        else if (CheckRange(posZ + Q12(43.5f), Q12(-12.0f), Q12(12.0f)))
        {
            yIdx = 2;
        }
    }

    res = sharedData_800ED430_2_s02[(xIdx * 3) + yIdx];
#elif defined(MAP6_S00) || defined(MAP6_S02)
    res = 0;

    // @hack Weird temp vars needed for match.
    yIdx = Q12(-127.0f);
    if (CheckRange(posZ + yIdx, Q12(-17.0f), Q12(17.0f)) &&
        ((yIdx = posX + Q12(192.0f), xIdx = Q12(8.0f) - 1, (u32)yIdx > xIdx) || posZ < Q12(130.0f)) &&
        (posX < Q12(-166.0f) || posZ > Q12(112.0f)))
    {
        res = 3;
    }
    else if (CheckRange(posZ + Q12(-8.5f), Q12(-31.5f), Q12(31.5f)) && posX < Q12(-159.5f))
    {
        res = 16;
    }
    else if (posZ < Q12(0.0f) && posX < Q12(-80.0f))
    {
        xIdx = Collision_FloorHeightGet(posX, posZ);

        if (posX < Q12(-160.0f))
        {
            res = (xIdx > Q12(1.0f)) ? 17 : 18;
        }
        else if (posX >= Q12(-148.0f))
        {
            res = (xIdx > Q12(1.0f)) ? 18 : 19;
        }
        else
        {
            res = 18;
        }
    }
#endif

    // If no match in primary grid, try fallback grid.
    if (res == 0)
    {
#define MAP_ROOM_STRIDE_Z ((MAP_ROOM_MAX_Z - MAP_ROOM_MIN_Z) / Q12(CHUNK_SIZE))

        if (posX < MAP_ROOM_MIN_X || posX >= MAP_ROOM_MAX_X ||
            posZ < MAP_ROOM_MIN_Z || posZ >= MAP_ROOM_MAX_Z)
        {
            // TODO: Not sure why these two maps need this.
            #if defined(MAP6_S00) || defined(MAP6_S02)
                return 0;
            #else
                res = 0;
            #endif
        }
        else
        {
            posX = (posX - MAP_ROOM_MIN_X) / Q12(CHUNK_SIZE);
            posZ = (posZ - MAP_ROOM_MIN_Z) / Q12(CHUNK_SIZE);

            res = MAP_ROOM_IDXS[(posX * MAP_ROOM_STRIDE_Z) + posZ];
#ifdef MAP_HAS_SECONDARY_GRID
            if (res == 0)
            {
                res = 26;
            }
#endif
        }

#undef MAP_ROOM_STRIDE_Z
    }

    return res;
#endif
}
#endif
