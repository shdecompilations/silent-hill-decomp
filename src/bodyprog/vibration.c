#include "bodyprog/game.h"

#include <psyq/libpad.h>
#include <psyq/strings.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/screen/data.h"
#include "bodyprog/math/math.h"
#include "main/fs_queue.h"

// TODO: Move to `bodyprog/libkpad` or `bodyprog/sys`.

u8 D_800AFD04 = 0;
u8 D_800AFD05 = 0;
// 2 bytes of padding.

bool (*g_VibScript_StepHandlers[])(s_KPadWork* kpadWork, s_KPad_Effect* effect, const s_VibScriptStep* step, u32* param) =
{
    VibScript_Handler_Start,
    VibScript_Handler_Chain,
    VibScript_Handler_Waveform,
    VibScript_Handler_Command,
    VibScript_Handler_Pulse
};

/** @brief Vibration scripts: each entry is a step, `Vibration_Play` starts the script at an index. */
const s_VibScriptStep g_VibScript_Steps[] =
{
    // System commands.
    /*  0 */ VIBSCRIPT_STEP_COMMAND(Pause, NULL),
    /*  1 */ VIBSCRIPT_STEP_COMMAND(Resume, NULL),
    /*  2 */ VIBSCRIPT_STEP_COMMAND(Disable, NULL),
    /*  3 */ VIBSCRIPT_STEP_COMMAND(Enable, NULL),
    /*  4 */ VIBSCRIPT_STEP_COMMAND(ClearAll, NULL),

    // Vibration chain entrypoints.
    /*  5 */ VIBSCRIPT_STEP_START(0, 5, &g_VibScript_Steps[22]),
    /*  6 */ VIBSCRIPT_STEP_START(1, 6, &g_VibScript_Steps[25]),
    /*  7 */ VIBSCRIPT_STEP_START(1, 7, &g_VibScript_Steps[26]),
    /*  8 */ VIBSCRIPT_STEP_START(1, 8, &g_VibScript_Steps[27]),
    /*  9 */ VIBSCRIPT_STEP_START(1, 9, &g_VibScript_Steps[23]),
    /* 10 */ VIBSCRIPT_STEP_START(1, 10, &g_VibScript_Steps[24]),
    /* 11 */ VIBSCRIPT_STEP_CHAIN(&g_VibScript_Steps[6], &g_VibScript_Steps[5]),
    /* 12 */ VIBSCRIPT_STEP_START(1, 12, &g_VibScript_Steps[31]),
    /* 13 */ VIBSCRIPT_STEP_START(1, 12, &g_VibScript_Steps[34]),
    /* 14 */ VIBSCRIPT_STEP_START(1, 12, &g_VibScript_Steps[37]),
    /* 15 */ VIBSCRIPT_STEP_START(1, 12, &g_VibScript_Steps[40]),
    /* 16 */ VIBSCRIPT_STEP_START(1, 12, &g_VibScript_Steps[43]),
    /* 17 */ VIBSCRIPT_STEP_START(1, 12, &g_VibScript_Steps[46]),
    /* 18 */ VIBSCRIPT_STEP_START(1, 18, &g_VibScript_Steps[28]),
    /* 19 */ VIBSCRIPT_STEP_START(1, 19, &g_VibScript_Steps[29]),
    /* 20 */ VIBSCRIPT_STEP_PULSE(0, NO_VALUE, 20, -255, NULL),
    /* 21 */ VIBSCRIPT_STEP_PULSE(1, NO_VALUE, 21, -255, NULL),

    // Waveforms for channel 1 (variable speed motor), one byte per sample.
    /* 22 */ VIBSCRIPT_STEP_WAVEFORM(1, NO_VALUE, 45,  9,  1, 0, VIBSCRIPT_SAMPLES_U8(0xA8, 0xE0, 0xE0, 0xC4, 0xA8, 0x54, 0, 0, 0, 0, 0, 0), NULL),
    /* 23 */ VIBSCRIPT_STEP_WAVEFORM(1, NO_VALUE, 384, 48, 1, 0, VIBSCRIPT_SAMPLES_U8(0x50, 0x28, 0x3C, 0x14, 0x46, 0x1E, 0x32, 0x0A, 0x50, 0, 0, 0), NULL),
    /* 24 */ VIBSCRIPT_STEP_WAVEFORM(1, NO_VALUE, 144, 24, 1, 0, VIBSCRIPT_SAMPLES_U8(0xC0, 0x60, 0x90, 0x30, 0xA8, 0x48, 0xC0, 0), NULL),
    /* 25 */ VIBSCRIPT_STEP_WAVEFORM(1, NO_VALUE, 90,  15, 1, 0, VIBSCRIPT_SAMPLES_U8(0xC8, 0xC8, 0xC8, 0xAF, 0x96, 0x4B, 0, 0), NULL),
    /* 26 */ VIBSCRIPT_STEP_WAVEFORM(1, NO_VALUE, 150, 15, 1, 1, VIBSCRIPT_SAMPLES_U8(0xA0, 0xA0, 0x8C, 0x46, 0x3C, 0x32, 0x28, 0x1E, 0x14, 0x0A, 0, 0), NULL),
    /* 27 */ VIBSCRIPT_STEP_WAVEFORM(1, NO_VALUE, 96,  12, 1, 0, VIBSCRIPT_SAMPLES_U8(0xFF, 0xFF, 0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0), NULL),
    /* 28 */ VIBSCRIPT_STEP_WAVEFORM(1, NO_VALUE, 96,  12, 1, 0, VIBSCRIPT_SAMPLES_U8(0xFF, 0xFF, 0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0), NULL),
    /* 29 */ VIBSCRIPT_STEP_WAVEFORM(1, NO_VALUE, 75,  15, 1, 0, VIBSCRIPT_SAMPLES_U8(0x80, 0x80, 0, 0x80, 0x80, 0, 0, 0), NULL),
    /* 30 */ VIBSCRIPT_STEP_EMPTY(),

    // Waveforms for channel 0 (on/off motor), one bit per sample, bit 0 of each byte first.
    /* 31 */ VIBSCRIPT_STEP_WAVEFORM(0, NO_VALUE, 600, 15, 0, 0, VIBSCRIPT_SAMPLES_BITS(VIBSCRIPT_BITS8(0, 0, 0, 0, 1, 0, 0, 0), VIBSCRIPT_BITS8(0, 0, 0, 0, 0, 0, 1, 1), 0, 0, 0, 0, 0, 0), NULL),
    /* 32 */ VIBSCRIPT_STEP_EMPTY(),
    /* 33 */ VIBSCRIPT_STEP_EMPTY(),
    /* 34 */ VIBSCRIPT_STEP_WAVEFORM(0, NO_VALUE, 480, 15, 0, 0, VIBSCRIPT_SAMPLES_BITS(VIBSCRIPT_BITS8(0, 0, 0, 0, 1, 0, 0, 0), VIBSCRIPT_BITS8(0, 1, 1, 0, 0, 0, 0, 0), 0, 0), NULL),
    /* 35 */ VIBSCRIPT_STEP_EMPTY(),
    /* 36 */ VIBSCRIPT_STEP_EMPTY(),
    /* 37 */ VIBSCRIPT_STEP_WAVEFORM(0, NO_VALUE, 420, 15, 0, 0, VIBSCRIPT_SAMPLES_BITS(VIBSCRIPT_BITS8(0, 0, 0, 1, 0, 0, 0, 0), VIBSCRIPT_BITS8(1, 1, 0, 0, 0, 0, 0, 0), 0, 0), NULL),
    /* 38 */ VIBSCRIPT_STEP_EMPTY(),
    /* 39 */ VIBSCRIPT_STEP_EMPTY(),
    /* 40 */ VIBSCRIPT_STEP_WAVEFORM(0, NO_VALUE, 360, 15, 0, 0, VIBSCRIPT_SAMPLES_BITS(VIBSCRIPT_BITS8(0, 0, 1, 0, 0, 0, 0, 1), VIBSCRIPT_BITS8(1, 0, 0, 0, 0, 0, 0, 0), 0, 0), NULL),
    /* 41 */ VIBSCRIPT_STEP_EMPTY(),
    /* 42 */ VIBSCRIPT_STEP_EMPTY(),
    /* 43 */ VIBSCRIPT_STEP_WAVEFORM(0, NO_VALUE, 270, 15, 0, 0, VIBSCRIPT_SAMPLES_BITS(VIBSCRIPT_BITS8(0, 0, 1, 0, 0, 0, 0, 1), VIBSCRIPT_BITS8(1, 0, 0, 0, 0, 0, 0, 0), 0, 0), NULL),
    /* 44 */ VIBSCRIPT_STEP_EMPTY(),
    /* 45 */ VIBSCRIPT_STEP_EMPTY(),
    /* 46 */ VIBSCRIPT_STEP_WAVEFORM(0, NO_VALUE, 2880, 15, 0, 0, VIBSCRIPT_SAMPLES_BITS(VIBSCRIPT_BITS8(0, 0, 1, 0, 0, 0, 0, 1), VIBSCRIPT_BITS8(1, 0, 0, 0, 0, 0, 0, 0), 0, VIBSCRIPT_BITS8(0, 0, 0, 1, 0, 0, 0, 0), VIBSCRIPT_BITS8(1, 1, 0, 0, 0, 0, 0, 0), 0, 0, VIBSCRIPT_BITS8(1, 0, 0, 0, 0, 1, 1, 0), 0, 0, 0, VIBSCRIPT_BITS8(1, 0, 0, 0, 0, 0, 0, 1), VIBSCRIPT_BITS8(1, 0, 0, 0, 0, 0, 0, 0), 0, 0, 0, VIBSCRIPT_BITS8(1, 0, 0, 0, 0, 0, 0, 1), VIBSCRIPT_BITS8(1, 0, 0, 0, 0, 0, 0, 0), 0, 0, 0, 0, VIBSCRIPT_BITS8(1, 0, 0, 0, 0, 0, 0, 0), 0), &g_VibScript_Steps[47]),
    /* 47 */ VIBSCRIPT_STEP_WAVEFORM(0, NO_VALUE, 30,   15, 0, 0, VIBSCRIPT_SAMPLES_BITS(0, 0, 0, 0), &g_VibScript_Steps[47]),
};

void Vibration_AnalogModeSet(bool isAnalog) // 0x80089090
{
    Vibration_KPadAnalogModeSet(&g_SysWork.kpad, isAnalog);
}

void Vibration_Init(void) // 0x800890B8
{
    KPad_WorkInit(&g_SysWork.kpad, 0);
    KPad_ChannelsInit(&g_SysWork.kpad, g_SysWork.kpadChannels, ARRAY_SIZE(g_SysWork.kpadChannels));
    KPad_EffectListInit(&g_SysWork.kpad, g_SysWork.kpadEffects, ARRAY_SIZE(g_SysWork.kpadEffects));

    g_SysWork.kpadHandlers = KPad_HandlerList_Init();

    KPad_HandlerList_AddCtp1SingleMotor(g_SysWork.kpadHandlers);
    KPad_HandlerList_AddDualShock(g_SysWork.kpadHandlers);
}

s32 Vibration_Update(void) // 0x80089128
{
    s_KPad_Handler* handlers;
    s32             vblanks;
    s32             effectCount;
    u16             mask;
    s_KPad_Effect*  head;
    s_KPad_Effect*  curr;
    s_KPad_Effect*  next;
    s_KPadWork*     kpadWork;

    kpadWork = &g_SysWork.kpad;
    handlers = g_SysWork.kpadHandlers;

    if (!kpadWork->status_0.isInit_0_16)
    {
        Vibration_Init();
    }

    if (g_GameWork.config.vibrationEnabled)
    {
        KPad_VibrationEnable(kpadWork);
        kpadWork->status_0.strength_0_8 = 128; // Full strength.
    }
    else
    {
        KPad_VibrationDisable(kpadWork);
        kpadWork->status_0.strength_0_8 = 0;
    }

    if (PadChkVsync())
    {
        KPad_ActuatorSetup(kpadWork, KPad_DeviceDetect(kpadWork, handlers), handlers);
    }

    effectCount = 0;

    kpadWork->channels_C[0].strength_4 = 0;
    kpadWork->channels_C[1].strength_4 = 0;

    head = KPad_EffectListHeadGet(kpadWork);
    curr = KPad_EffectNextGet(head);

    vblanks = !kpadWork->status_0.isPaused_0_17 ? g_VBlanks : 0;

    mask = 0xFFFF;

    for (; curr != head; curr = next, effectCount++)
    {
        next = KPad_EffectNextGet(curr);

        if (!VibScript_EffectUpdate(kpadWork, curr, mask, vblanks))
        {
            KPad_EffectRemove(kpadWork, curr);
        }
    }

    KPad_ActuatorDataUpdate(kpadWork);

    return effectCount;
}

void Vibration_Play(s32 scriptIdx) // 0x800892A4
{
    VibScript_Play(&g_SysWork.kpad, &g_VibScript_Steps[scriptIdx], 0x80);
}

void Vibration_PlayStrength(s32 scriptIdx, u8 strength) // 0x800892DC
{
    VibScript_Play(&g_SysWork.kpad, &g_VibScript_Steps[scriptIdx], strength);
}

void Vibration_GasWeaponEngineUpdate(bool isChainsaw) // 0x80089314
{
    s32 interval;
    s32 step;

    D_800AFD05 += g_VBlanks;

#if VERSION_REGION_IS(PAL)
    interval = isChainsaw ? 5 : 20;
#else
    interval = isChainsaw ? 6 : 24;
#endif

    if (D_800AFD05 >= interval)
    {
        step = Rng_RandQ12();
        if (isChainsaw)
        {
            step = Q12_MULT(step, 16) + 20;
        }
        else
        {
            step = Q12_MULT(step, 20) + 60;
        }

        D_800AFD04 = (D_800AFD04 + step) & 0x7F;
    }

    Vibration_PlayStrength(21, D_800AFD04 + 32);
}

void Vibration_DamageRumble(q19_12 damage) // 0x800893D0
{
    u8 strength;

    if (damage > Q12(25.0f))
    {
        strength = 200;
    }
    else if (damage < Q12(5.0f))
    {
        strength = 100;
    }
    else
    {
        strength = ((damage + Q12(15.0f)) / Q12(5.0f)) * 25;
    }

    Vibration_PlayStrength(10, strength);
}

void Vibration_StumbleRumble(void) // 0x8008944C
{
    Vibration_PlayStrength(19, 0xA0);
}

void Vibration_LandingRumble(void) // 0x80089470
{
    Vibration_PlayStrength(19, 0xA0);
}

void Vibration_EnemyGrabRumble(void) // 0x80089494
{
    Vibration_PlayStrength(9, 0xA0);
}

void Vibration_RumbleStrength(s32 strength) // 0x800894B8
{
    Vibration_PlayStrength(9, strength);
}

void Vibration_StrongRumble(void) // 0x800894DC
{
    Vibration_PlayStrength(9, 0xE0);
}

void Vibration_Rumble(void) // 0x80089500
{
    Vibration_PlayStrength(9, 0xA0);
}

bool Vibration_KPadAnalogModeSet(s_KPadWork* kpadWork, bool isAnalog) // 0x80089524
{
    s32         padState;
    bool        cond;
    u8          socket;
    s_KPadWork* temp;

    socket = kpadWork->status_0.port_0_0;
    PadInfoMode(socket, InfoModeCurExID, 0);

    padState = PadGetState(socket);
    cond     = PadInfoMode(socket, InfoModeCurExOffs, 0) == isAnalog;
    if (!cond && padState == PadStateStable)
    {
        PadSetMainMode(socket, isAnalog, 0);

        cond = PadSetMainMode(socket, 1, 0);

        temp                            = &g_SysWork.kpad;
        temp->status_0.isActSet_0_23    = 0;
        temp->status_0.isAlignDone_0_22 = 0;
    }

    return cond;
}

s32 VibScript_Play(s_KPadWork* kpadWork, const s_VibScriptStep* step, u8 strength) // 0x800895E4
{
    u32 localStrength;

    localStrength = strength;

    if (step == NULL)
    {
        return 0;
    }

    return g_VibScript_StepHandlers[step->data_4.opcode_4](kpadWork, 0, step, &localStrength);
}

bool VibScript_EffectUpdate(s_KPadWork* kpadWork, s_KPad_Effect* effect, u16 mask, u32 ticks) // 0x80089644
{
    const s_VibScriptStep* step;

#if VERSION_REGION_IS(PAL)
    ticks *= 6;
#else
    ticks *= 5;
#endif

    effect->output_18       = 0;
    effect->hasOutput_14_31 = 0;

    while (true)
    {
        step = effect->step_10;
        if (step == NULL || !g_VibScript_StepHandlers[step->data_4.opcode_4](kpadWork, effect, step, &ticks))
        {
            break;
        }
    }

    if (effect->hasOutput_14_31 && (mask & effect->mask_1E))
    {
        s_KPad_Channel* channel;

        channel = &kpadWork->channels_C[effect->channel_14_24];
        if (channel->strength_4 < effect->output_18)
        {
            channel->strength_4 = effect->output_18;
        }
    }

    return effect->step_10 != NULL;
}

bool VibScript_Handler_Command(s_KPadWork* kpadWork, s_KPad_Effect* effect, const s_VibScriptStep* step, u32* strength) // 0x8008973C
{
    if (kpadWork == NULL || effect != NULL || step == NULL || step->data_4.command.opcode_4 != VibScriptOp_Command)
    {
        return false;
    }

    switch (step->data_4.command.command_5)
    {
        case VibScriptCommand_Enable:
            KPad_VibrationEnable(kpadWork);
            break;

        case VibScriptCommand_Disable:
            KPad_VibrationDisable(kpadWork);
            break;

        case VibScriptCommand_Pause:
            KPad_VibrationPause(kpadWork);
            break;

        case VibScriptCommand_Resume:
            KPad_VibrationResume(kpadWork);
            break;

        case VibScriptCommand_ClearAll:
            VibScript_EffectsClear(kpadWork);
            break;

        case VibScriptCommand_RemoveByMask:
            VibScript_EffectsRemoveByMask(kpadWork, step->data_4.command.arg_6, step->data_4.command.callback_C);
            break;

        case VibScriptCommand_RemoveByTag:
            VibScript_EffectsRemoveByTag(kpadWork, step->data_4.command.arg_6, step->data_4.command.callback_C);
            break;

        case VibScriptCommand_RemoveById:
            VibScript_EffectsRemoveById(kpadWork, step->data_4.command.id_8);
            break;
    }

    return true;
}

void VibScript_EffectsClear(s_KPadWork* kpadWork) // 0x80089840
{
    s_KPad_Effect* next;
    s_KPad_Effect* curr;
    s_KPad_Effect* head;

    if (kpadWork == NULL)
    {
        return;
    }

    curr = kpadWork->effects_18.next_0;
    head = KPAD_EFFECTS_HEAD(kpadWork);

    while (curr != head)
    {
        next = curr->links_0.next_0;
        KPad_EffectRemove(kpadWork, curr);
        curr = next;
    }
}

void VibScript_EffectsRemoveByMask(s_KPadWork* kpadWork, u16 arg, s32 (*testFunc)(u16 mask, s32 arg)) // 0x8008989C
{
    s_KPad_Effect* next;
    s_KPad_Effect* head;
    s_KPad_Effect* curr;

    if (kpadWork == NULL)
    {
        return;
    }

    curr = kpadWork->effects_18.next_0;
    head = KPAD_EFFECTS_HEAD(kpadWork);

    while (curr != head)
    {
        next = curr->links_0.next_0;

        if (testFunc(curr->mask_1E, arg) != 0)
        {
            KPad_EffectRemove(kpadWork, curr);
        }

        curr = next;
    }
}

void VibScript_EffectsRemoveByTag(s_KPadWork* kpadWork, u16 arg, s32 (*testFunc)(u16 tag, s32 arg)) // 0x8008992C
{
    s_KPad_Effect* next;
    s_KPad_Effect* head;
    s_KPad_Effect* curr;

    if (kpadWork == NULL)
    {
        return;
    }

    curr = kpadWork->effects_18.next_0;
    head = KPAD_EFFECTS_HEAD(kpadWork);

    while (curr != head)
    {
        next = curr->links_0.next_0;

        if (testFunc(curr->tag_1C, arg) != 0)
        {
            KPad_EffectRemove(kpadWork, curr);
        }

        curr = next;
    }
}

void VibScript_EffectsRemoveById(s_KPadWork* kpadWork, s32 id) // 0x800899BC
{
    s_KPad_Effect* next;
    s_KPad_Effect* head;
    s_KPad_Effect* curr;

    if (kpadWork == NULL)
    {
        return;
    }

    curr = kpadWork->effects_18.next_0;
    head = KPAD_EFFECTS_HEAD(kpadWork);

    while (curr != head)
    {
        next = curr->links_0.next_0;

        if (curr->id_C == id)
        {
            KPad_EffectRemove(kpadWork, curr);
        }

        curr = next;
    }
}

bool VibScript_Handler_Start(s_KPadWork* kpadWork, s_KPad_Effect* effect, const s_VibScriptStep* step, u32* strength) // 0x80089A30
{
    s32 id;

    if (kpadWork == NULL || step == NULL || step->data_4.opcode_4 != VibScriptOp_Start)
    {
        return false;
    }

    id = step->data_4.start.id_8;

    if (step->data_4.start.skipIfActive_5_5 || step->data_4.start.replaceActive_5_6)
    {
        s_KPad_Effect* curr;
        s_KPad_Effect* head;

        curr = kpadWork->effects_18.next_0;
        head = KPAD_EFFECTS_HEAD(kpadWork);

        if (step->data_4.start.skipIfActive_5_5)
        {
            while (curr != head)
            {
                if (curr->id_C == id)
                {
                    if (effect != NULL)
                    {
                        effect->step_10 = NULL;
                    }

                    return false;
                }

                curr = curr->links_0.next_0;
            }
        }
        else
        {
            s_KPad_Effect* next;

            while (curr != head)
            {
                next = curr->links_0.next_0;
                if ((effect == NULL || curr != effect) && curr->id_C == id)
                {
                    KPad_EffectRemove(kpadWork, curr);
                }
                curr = next;
            }
        }
    }

    if (effect == NULL)
    {
        effect = KPad_EffectAdd(kpadWork, step->data_4.start.priority_C);
        if (effect == NULL)
        {
            return false;
        }
        effect->strength_14_16 = *strength;
    }

    effect->step_10      = step->next_0;
    effect->elapsed_14_0 = 0;
    effect->id_C         = id;

    if (step->data_4.start.hasTag_5_7)
    {
        effect->tag_1C = step->data_4.start.tag_6;
    }

    return true;
}

bool VibScript_Handler_Chain(s_KPadWork* kpadWork, s_KPad_Effect* effect, const s_VibScriptStep* step, u32* strength) // 0x80089BB8
{
    s_KPad_Effect* neighbor;
    s_KPad_Effect* spawned;
    u32            priority;

    if (kpadWork == NULL || step == NULL)
    {
        return false;
    }

    if (step->data_4.opcode_4 != VibScriptOp_Chain)
    {
        return false;
    }

    if (effect == NULL)
    {
        priority = step->data_4.chain.priority_C;
        effect   = KPad_EffectAdd(kpadWork, priority);
        if (effect == NULL)
        {
            return false;
        }
        effect->strength_14_16 = *strength;
    }
    else if (effect->step_10 != step)
    {
        effect->step_10 = NULL;
        return false;
    }
    else
    {
        priority = effect->priority_8;
    }

    effect->step_10      = step->next_0;
    effect->elapsed_14_0 = 0;

    if (step->data_4.chain.hasTag_5_7)
    {
        effect->tag_1C = step->data_4.chain.tag_6;
    }

    spawned = KPad_EffectAdd(kpadWork, priority);
    if (spawned != NULL)
    {
        // Unlink from the position `KPad_EffectAdd` chose.
        neighbor                                 = spawned->links_0.next_0;
        neighbor->links_0.prev_4                 = spawned->links_0.prev_4;
        neighbor->links_0.prev_4->links_0.next_0 = neighbor;

        // Relink directly after `effect`.
        neighbor                 = effect->links_0.next_0;
        spawned->links_0.prev_4  = effect;
        spawned->links_0.next_0  = neighbor;
        neighbor->links_0.prev_4 = spawned;
        effect->links_0.next_0   = spawned;

        spawned->step_10        = step->data_4.chain.spawned_8;
        spawned->elapsed_14_0   = 0;
        spawned->id_C           = effect->id_C;
        spawned->strength_14_16 = effect->strength_14_16;
        spawned->tag_1C         = effect->tag_1C;
    }

    return true;
}

bool VibScript_Handler_Pulse(s_KPadWork* kpadWork, s_KPad_Effect* effect, const s_VibScriptStep* step, u32* strength) // 0x80089D0C
{
    if (kpadWork == NULL || step == NULL || step->data_4.pulse.opcode_4 != VibScriptOp_Pulse)
    {
        return false;
    }

    if (effect != NULL)
    {
        effect->channel_14_24   = step->data_4.pulse.channel_5 & 0x7F;
        effect->mask_1E         = step->data_4.pulse.mask_6;
        effect->step_10         = step->next_0;
        effect->hasOutput_14_31 = 1;
        effect->output_18       = effect->strength_14_16 << 24;
        return false;
    }

    effect = KPad_EffectAdd(kpadWork, step->data_4.pulse.priority_C);
    if (effect == NULL)
    {
        return false;
    }

    effect->step_10        = step;
    effect->strength_14_16 = *strength;
    effect->id_C           = step->data_4.pulse.id_8;
    return true;
}

bool VibScript_Handler_Waveform(s_KPadWork* kpadWork, s_KPad_Effect* effect, const s_VibScriptStep* step, u32* ticksLeft) // 0x80089DF0
{
    u32 ticks;
    u32 elapsed;
    u32 duration;

    if (step == NULL || effect == NULL || step->data_4.opcode_4 != VibScriptOp_Waveform)
    {
        return false;
    }

    if (effect->step_10 != step)
    {
        effect->step_10 = NULL;
        return false;
    }

    elapsed = effect->elapsed_14_0;

    if (!effect->hasOutput_14_31)
    {
        u32 output;
        u32 strength;
        u8* samples;
        u32 ticksPerSample;
        u32 bytesPerSample;
        s32 interpolate;
        u32 sampleIdx;
        u32 nextIdx;
        u32 frac;
        u32 sample;
        u32 nextSample;
        u64 scaled;

        effect->hasOutput_14_31 = 1;

        strength       = effect->strength_14_16;
        nextSample     = step->data_4.waveform.channel_5 & 0x7F; // @hack Channel, held in `nextSample` for a match.
        samples        = step->data_4.waveform.samples_8;
        ticksPerSample = step->data_4.waveform.ticksPerSample_C_16;
        bytesPerSample = step->data_4.waveform.bytesPerSample_C_28;
        interpolate    = step->data_4.waveform.interpolate_C_31;

        effect->channel_14_24 = nextSample;
        effect->mask_1E       = step->data_4.waveform.mask_6;

        if (ticksPerSample == 0)
        {
            sampleIdx = elapsed;
            frac      = 0;
        }
        else
        {
            sampleIdx = elapsed / ticksPerSample;
            frac      = elapsed % ticksPerSample;
        }

        if (!interpolate)
        {
            frac = 0;
        }

        if (bytesPerSample == 0)
        {
            nextIdx    = sampleIdx + 1;
            sample     = (samples[sampleIdx >> 3] >> (sampleIdx & 7)) & 1;
            sample     = -sample;
            nextSample = (samples[nextIdx >> 3] >> (nextIdx & 7)) & 1;
            nextSample = -nextSample;
        }
        else
        {
            u32 i;
            s32 shift;

            if (bytesPerSample >= 5)
            {
                bytesPerSample = 4;
            }

            sampleIdx = sampleIdx * bytesPerSample;

            sample     = 0;
            nextSample = 0;

            for (i = bytesPerSample, shift = 24, nextIdx = sampleIdx + i; i != 0;
                 i--, shift -= 8, sampleIdx++, nextIdx++)
            {
                sample     += samples[sampleIdx] << shift;
                nextSample += samples[nextIdx] << shift;
            }
        }

        if (frac == 0)
        {
            output = sample;
        }
        else
        {
            u32 weight;
            u32 sampleWeighted;
            u32 nextSampleWeighted;

            weight             = ((UINT_MAX / ticksPerSample) + 1) * frac;
            sampleWeighted     = (sample * (u64)-weight) >> 32;
            nextSampleWeighted = (nextSample * (u64)weight) >> 32;
            output             = nextSampleWeighted + sampleWeighted;
        }

        scaled = (u64)output * strength >> 7;

        if (scaled > UINT_MAX)
        {
            output = UINT_MAX;
        }
        else
        {
            output = scaled;
        }

        effect->output_18 = output;
    }

    ticks    = *ticksLeft;
    duration = step->data_4.waveform.duration_C_0;
    elapsed += ticks;

    if (elapsed >= duration)
    {
        effect->step_10      = step->next_0;
        effect->elapsed_14_0 = 0;
        ticks                = elapsed - duration;
        *ticksLeft           = ticks;

        return true;
    }

    effect->elapsed_14_0 = elapsed;
    *ticksLeft           = 0;

    return false;
}
