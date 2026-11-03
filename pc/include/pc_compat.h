#pragma once
/**
 * pc_compat.h — forced include for every PC build file.
 *
 * This is added via `-include pc/include/pc_compat.h` in CMake, so it
 * runs before any original header. It redefines PSX-specific macros to
 * be PC-safe, without needing to shadow psyq headers.
 *
 * On PSX build (no PC_PORT), this header is never included.
 */

#ifdef PC_PORT

// ---------------------------------------------------------------------------
// Section placement — on PSX this puts data into .sdata/.rodata at fixed VRAM.
// On PC we want normal placement so the linker can relocate.
// ---------------------------------------------------------------------------
#undef SECTION
#define SECTION(x)  // empty on PC

// Scratchpad — PSX fast RAM at 0x1F800000, not available on PC
#undef PSX_SCRATCH
#undef PSX_SCRATCH_ADDR
#define PSX_SCRATCH ((void*)0)
#define PSX_SCRATCH_ADDR(off) ((void*)(uintptr_t)(off))

// GTE cop2 inline asm — replaced by C fallbacks in pc/src/gpu/shim.c
// The original headers use __asm__ volatile("cop2 ...") which won't compile on x86.
#ifndef __mips__
#define cop2_asm_stub() do {} while(0)
#endif

// Prevent inclusion of PSX assembly helpers that use MIPS-specific directives
#define _PSX_ASM_GUARD

// Make VRAM addresses relocatable
#include "pc/core/shiftable.h"

#endif // PC_PORT
