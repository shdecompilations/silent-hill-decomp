#ifndef _COMMON_H
#define _COMMON_H

#include "include_asm.h"
#include "types.h"
#include "version.h"

#define PAD_RODATA()

#define PSX_SCRATCH ((void*)0x1F800000)

#define PSX_SCRATCH_ADDR(offset) ((void*)(((u8*)PSX_SCRATCH) + (offset)))

/** @brief Computes the size of an array.
 *
 * @param arr Array.
 * @return Element count.
 */
#define ARRAY_SIZE(arr) \
    (s32)(sizeof(arr) / sizeof((arr)[0]))

/** @brief Creates a bitmask with a contiguous range of bits set.
 * For use with `s_PlayerExtra::disabledAnimBoneFlags`.
 *
 * Generates an `unsigned int` mask with all bits in the range `[fromInclusive, toInclusive]` set.
 *
 * For example:
 * - `BITMASK_RANGE(0, 2)` -> 0b000...0111 (decimal 7).
 * - `BITMASK_RANGE(4, 11)` -> 0b000...111111110000 (decimal 4080).
 *
 * @param fromInclusive Index of the lowest bit to set (0 = least significant bit).
 * @param toInclusive Index of the highest bit to set.
 * @return Bitmask with the specified range of bits set to 1 (`unsigned int`).
 */
#define BITMASK_RANGE(fromInclusive, toInclusive) \
    (((~0u << (fromInclusive)) & ~(~0u << ((toInclusive) + 1))))

#define HAS_FLAG(flags, idx) \
    ((((u32*)flags)[(idx) >> 5] >> ((idx) & 0x1F)) & (1 << 0))

#define SET_FLAG(flags, idx) \
    ((((u32*)flags)[(idx) >> 5] |= (1 << 0) << ((idx) & 0x1F)))

#define CLEAR_FLAG(flags, idx) \
    ((((u32*)flags)[(idx) >> 5] &= ~((1 << 0) << ((idx) & 0x1F))))

#define ALIGN(x, a) \
    (((u32)(x) + ((a) - 1)) & ~((a) - 1))

#define SECTION(x) \
    __attribute__((section(x)))

#define STATIC_ASSERT(cond, msg) \
    typedef char static_assertion_##msg[(cond) ? 1 : -1]

#define STATIC_ASSERT_SIZEOF(type, size) \
    typedef char static_assertion_sizeof_##type[(sizeof(type) == (size)) ? 1 : -1]

#endif
