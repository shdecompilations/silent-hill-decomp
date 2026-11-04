#ifndef _BODYPROG_LIBKPAD_VIBRATION_H
#define _BODYPROG_LIBKPAD_VIBRATION_H

// Vibration scripts:
// - Started by index into `g_VibScript_Steps`. Start, Chain and Pulse steps allocate an effect in the libkpad effect list.
// - Each frame, every effect runs its steps until one has to wait for ticks to pass.
// - Waveform and Pulse steps set the effect's motor level, which raises the pad channel's level if higher.
// - The effect is freed once its script ends.
// - Command steps call into libkpad to enable/disable/pause/resume/remove effects.

// Script step initializers, see `s_VibScriptStep`. `next` is the step to run afterwards.
#define VIBSCRIPT_STEP_COMMAND(cmd, next) \
    { next, { .command = { VibScriptOp_Command, VibScriptCommand_##cmd, 0, 0, 0 } } }

#define VIBSCRIPT_STEP_START(skipIfActive, id, next) \
    { next, { .start = { VibScriptOp_Start, 0, skipIfActive, 0, 0, 0, id, 0 } } }

#define VIBSCRIPT_STEP_EMPTY() \
    VIBSCRIPT_STEP_START(0, 0, NULL)

#define VIBSCRIPT_STEP_CHAIN(spawned, next) \
    { next, { .chain = { VibScriptOp_Chain, 0, 0, 0, spawned, 0 } } }

#define VIBSCRIPT_STEP_WAVEFORM(channel, mask, duration, ticksPerSample, bytesPerSample, interpolate, samples, next) \
    { next, { .waveform = { VibScriptOp_Waveform, channel, mask, samples, duration, ticksPerSample, bytesPerSample, interpolate } } }

#define VIBSCRIPT_STEP_PULSE(channel, mask, id, priority, next) \
    { next, { .pulse = { VibScriptOp_Pulse, channel, mask, id, priority } } }

// Waveform sample data for `VIBSCRIPT_STEP_WAVEFORM`. Byte samples are motor levels (`0x80` is 1.0).
// 1-bit samples are on/off, bit 0 of each byte is the first sample.
// (trailing zeros added to pad each array to original size, compiler doesn't seem to insert it for us...)
#define VIBSCRIPT_SAMPLES_U8(...) \
    ((const u8[]){ __VA_ARGS__ })

#define VIBSCRIPT_SAMPLES_BITS(...) \
    ((const u8[]){ __VA_ARGS__ })

#define VIBSCRIPT_BITS8(b0, b1, b2, b3, b4, b5, b6, b7) \
    ((b0) | ((b1) << 1) | ((b2) << 2) | ((b3) << 3) | ((b4) << 4) | ((b5) << 5) | ((b6) << 6) | ((b7) << 7))

/** @brief Step opcodes, index into the handler table `g_VibScript_StepHandlers`. */
typedef enum _VibScriptOp
{
    VibScriptOp_Start    = 0, /** Starts an effect that runs the script following this step. */
    VibScriptOp_Chain    = 1, /** Starts an effect and spawns a second one from another script. */
    VibScriptOp_Waveform = 2, /** Plays sample data on a motor for a fixed number of ticks. */
    VibScriptOp_Command  = 3, /** Changes global state or removes effects, see `e_VibScriptCommand`. */
    VibScriptOp_Pulse    = 4  /** Sets a motor level for a single tick. */
} e_VibScriptOp;

/** @brief Selects the action of an `VibScriptOp_Command` step. */
typedef enum _VibScriptCommand
{
    VibScriptCommand_Enable       = 0,
    VibScriptCommand_Disable      = 1,
    VibScriptCommand_Pause        = 2,
    VibScriptCommand_Resume       = 3,
    VibScriptCommand_ClearAll     = 4,
    VibScriptCommand_RemoveByMask = 5, // Removes effects for which the `callback_C(mask, arg_6)` returns non-zero.
    VibScriptCommand_RemoveByTag  = 6, // Same as above, tested against the effect tag instead of its mask.
    VibScriptCommand_RemoveById   = 7  // Removes effects with an id equal to `value_8`.
} e_VibScriptCommand;

/** @brief One step of a vibration script. Scripts are chains of steps linked through `next_0`.
 *
 * What the fields after `opcode_4` mean depends on the opcode, so each opcode has its own view in `data_4`.
 * `opcode_4` repeats in every view as the compiler needs it to be part of the structs for a match.
 */
typedef struct _VibScriptStep
{
    struct _VibScriptStep* next_0; // Step to run once this one completes, `NULL` ends the script.

    union
    {
        u8 opcode_4;

        // `VibScriptOp_Start`
        struct
        {
            u8  opcode_4;
            u8  unk_5_0           : 5;
            u32 skipIfActive_5_5  : 1; // Do nothing if an effect with the same `id_8` is already running.
            u32 replaceActive_5_6 : 1; // Remove any running effects with the same `id_8` first.
            u32 hasTag_5_7        : 1; // Copy `tag_6` to the effect.
            u16 tag_6;
            s32 id_8;
            u32 priority_C;
        } start;

        // `VibScriptOp_Chain`
        struct
        {
            u8                opcode_4;
            u8                unk_5      : 7;
            u32               hasTag_5_7 : 1; // Copy `tag_6` to the effect.
            u16               tag_6;
            struct _VibScriptStep* spawned_8;      // Script for the second effect, which gets the id of the first.
            u32               priority_C;
        } chain;

        // `VibScriptOp_Waveform`
        struct
        {
            u8  opcode_4;
            u8  channel_5; // Bits 0-6: motor index into `s_KPadWork::channels_C`.
            u16 mask_6;    // Effect output only applies when a caller's mask intersects this, `NO_VALUE` for any.
            u8* samples_8;
            u32 duration_C_0        : 16; // Total length in ticks.
            u32 ticksPerSample_C_16 : 12;
            u32 bytesPerSample_C_28 : 3;  // 0: 1-bit samples packed LSB first, else big-endian bytes (max 4).
            u32 interpolate_C_31    : 1;  // Blend between neighbouring samples instead of holding each one.
        } waveform;

        // `VibScriptOp_Command`
        struct
        {
            u8  opcode_4;
            u8  command_5; // `e_VibScriptCommand`.
            u16 arg_6;
            s32 value_8;
            u32 callback_C; // `s32 (*)(u16, s32)`.
        } command;

        // `VibScriptOp_Pulse`
        struct
        {
            u8  opcode_4;
            u8  channel_5; // Bits 0-6: motor index into `s_KPadWork::channels_C`.
            u16 mask_6;
            s32 id_8;
            u32 priority_C;
        } pulse;
    } data_4;
} s_VibScriptStep;
STATIC_ASSERT_SIZEOF(s_VibScriptStep, 16);

/** Might retrun `bool`. */
void func_80089090(s32 arg0);

// Related to libkpad.
void func_800890B8(void);

s32 func_80089128(void);

void func_800892A4(s32 idx);

void func_800892DC(s32 idx, u8 arg1);

void func_80089314(s32 arg0);

void func_800893D0(q19_12 arg0);

void func_8008944C(void);

void func_80089470(void);

void func_80089494(void);

void func_800894B8(s32 arg0);

void func_800894DC(void);

void func_80089500(void);

/** Handles initial gamepad init. */
bool func_80089524(s_KPadWork* kpadWork, s32 padInfoMode);

s32 func_800895E4(s_KPadWork* kpadWork, s_VibScriptStep* step, u8 arg2);

bool func_80089644(s_KPadWork* kpadWork, s_KPad_Effect* effect, s32 arg2, u32 arg3);

bool func_80089A30(s_KPadWork* kpadWork, s_KPad_Effect* effect, s_VibScriptStep* step, u32* arg3);

bool func_80089BB8(s_KPadWork* kpadWork, s_KPad_Effect* effect, s_VibScriptStep* step, u32* arg3);

bool func_80089DF0(s_KPadWork* kpadWork, s_KPad_Effect* effect, s_VibScriptStep* step, u32* arg3);

bool func_8008973C(s_KPadWork* kpadWork, s_KPad_Effect* effect, s_VibScriptStep* step, u32* arg3);

void func_80089840(s_KPadWork* kpadWork);

void func_8008989C(s_KPadWork* kpadWork, u16 arg1, s32 (*arg2)(u16, s32));

void func_8008992C(s_KPadWork* kpadWork, u16 arg1, s32 (*arg2)(u16, s32));

void func_800899BC(s_KPadWork* kpadWork, s32 arg1);

bool func_80089D0C(s_KPadWork* kpadWork, s_KPad_Effect* effect, s_VibScriptStep* step, u32* arg3);

#endif
