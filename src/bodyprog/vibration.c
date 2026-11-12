#include "bodyprog/game.h"

#include <psyq/libpad.h>
#include <psyq/strings.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/screen/data.h"
#include "bodyprog/math/math.h"
#include "main/fsqueue.h"

// TODO: Move to `bodyprog/libkpad`.

u8 D_800AFD04 = 0;
u8 D_800AFD05 = 0;
// 2 bytes of padding.

bool (*g_VibScript_StepHandlers[])(s_KPadWork* arg0, s_KPad_Effect* arg1, s_VibScriptStep* ptr, u32* arg3) =
{
    func_80089A30,
    func_80089BB8,
    func_80089DF0,
    func_8008973C,
    func_80089D0C
};

/** @brief Vibration scripts: each entry is a step, `func_800892A4` starts the script at an index. */
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

void func_80089090(s32 arg0) // 0x80089090
{
    func_80089524(&g_SysWork.kpad, arg0);
}

void func_800890B8(void) // 0x800890B8
{
    KPad_WorkInit(&g_SysWork.kpad, 0);
    KPad_ChannelsInit(&g_SysWork.kpad, g_SysWork.kpadChannels, ARRAY_SIZE(g_SysWork.kpadChannels));
    KPad_EffectListInit(&g_SysWork.kpad, g_SysWork.kpadEffects, ARRAY_SIZE(g_SysWork.kpadEffects));

    g_SysWork.kpadHandlers = KPad_HandlerList_Init();

    KPad_HandlerList_AddCtp1SingleMotor(g_SysWork.kpadHandlers);
    KPad_HandlerList_AddDualShock(g_SysWork.kpadHandlers);
}

s32 func_80089128(void) // 0x80089128
{
    s_KPad_Handler* temp_s0;
    s32             var_s3;
    s32             var_s5;
    s32             var_s6;
    s_KPad_Effect*  temp_v0;
    s_KPad_Effect*  var_s0;
    s_KPad_Effect*  temp_s1;
    s_KPadWork*     var_s2;

    var_s2 = &g_SysWork.kpad;

    temp_s0 = g_SysWork.kpadHandlers;

    if (!var_s2->status_0.isInit_0_16)
    {
        func_800890B8();
    }

    if (g_GameWork.config.vibrationEnabled)
    {
        KPad_VibrationEnable(var_s2);
        var_s2->status_0.strength_0_8 = 128; // Full strength.
    }
    else
    {
        KPad_VibrationDisable(var_s2);
        var_s2->status_0.strength_0_8 = 0;
    }

    if (PadChkVsync())
    {
        KPad_ActuatorSetup(var_s2, KPad_DeviceDetect(var_s2, temp_s0), temp_s0);
    }

    var_s5 = 0;

    var_s2->channels_C[0].strength_4 = 0;
    var_s2->channels_C[1].strength_4 = 0;

    temp_v0 = KPad_EffectListHeadGet(var_s2);
    var_s0  = KPad_EffectNextGet(temp_v0);

    if (!var_s2->status_0.isPaused_0_17)
    {
        var_s3 = g_VBlanks;
    }
    else
    {
        var_s3 = 0;
    }

    var_s6 = 0xFFFF;

    for (; var_s0 != temp_v0; var_s5++)
    {
        temp_s1 = KPad_EffectNextGet(var_s0);

        if (!func_80089644(var_s2, var_s0, var_s6 & 0xFFFF, var_s3))
        {
            KPad_EffectRemove(var_s2, var_s0);
        }

        var_s0 = temp_s1;
    }

    KPad_ActuatorDataUpdate(var_s2);

    return var_s5;
}

void func_800892A4(s32 idx) // 0x800892A4
{
    func_800895E4(&g_SysWork.kpad, &g_VibScript_Steps[idx], 0x80);
}

void func_800892DC(s32 idx, u8 arg1) // 0x800892DC
{
    func_800895E4(&g_SysWork.kpad, &g_VibScript_Steps[idx], arg1);
}

void func_80089314(s32 arg0) // 0x80089314
{
    s32 var0;
    s32 var1;

#if VERSION_REGION_IS(PAL)
    var0        = 20;
#else
    var0        = 24;
#endif

    D_800AFD05 += g_VBlanks;

    if (arg0 != 0)
    {
#if VERSION_REGION_IS(PAL)
        var0 = 5;
#else
        var0 = 6;
#endif
    }

    if ((s32)D_800AFD05 >= var0)
    {
        var0 = Rng_RandQ12();
        if (arg0 != 0)
        {
            var1 = ((var0 * 16) >> 12) + 20;
        }
        else
        {
            var1 = ((var0 * 20) >> 12) + 60;
        }

        D_800AFD04 = (D_800AFD04 + var1) & 0x7F;
    }

    func_800892DC(21, D_800AFD04 + 32);
}

void func_800893D0(q19_12 arg0) // 0x800893D0
{
    u32 var;

    if (arg0 > Q12(25.0f))
    {
        var = 200;
    }
    else if (arg0 < Q12(5.0f))
    {
        var = 100;
    }
    else
    {
        var = ((arg0 + Q12(15.0f)) / Q12(5.0f)) * 25;
    }

    func_800892DC(10, var);
}

void func_8008944C(void) // 0x8008944C
{
    func_800892DC(19, 0xA0);
}

void func_80089470(void) // 0x80089470
{
    func_800892DC(19, 0xA0);
}

void func_80089494(void) // 0x80089494
{
    func_800892DC(9, 0xA0);
}

void func_800894B8(s32 arg0) // 0x800894B8
{
    func_800892DC(9, arg0);
}

void func_800894DC(void) // 0x800894DC
{
    func_800892DC(9, 0xE0);
}

void func_80089500(void) // 0x80089500
{
    func_800892DC(9, 0xA0);
}

bool func_80089524(s_KPadWork* kpadWork, s32 padInfoMode) // 0x80089524
{
    s32         padState;
    bool        cond;
    u8          socket;
    s_KPadWork* temp;

    socket = kpadWork->status_0.port_0_0;
    PadInfoMode(socket, 2, 0);

    padState = PadGetState(socket);
    cond     = PadInfoMode(socket, 3, 0) == padInfoMode;
    if (!cond && padState == 6)
    {
        PadSetMainMode(socket, padInfoMode, 0);

        cond = PadSetMainMode(socket, 1, 0);

        temp                            = &g_SysWork.kpad;
        temp->status_0.isActSet_0_23    = 0;
        temp->status_0.isAlignDone_0_22 = 0;
    }

    return cond;
}

s32 func_800895E4(s_KPadWork* kpadWork, s_VibScriptStep* step, u8 arg2) // 0x800895E4
{
    u32 sp10;

    sp10 = arg2;

    if (step == NULL)
    {
        return 0;
    }

    return g_VibScript_StepHandlers[step->data_4.opcode_4](kpadWork, 0, step, &sp10);
}

bool func_80089644(s_KPadWork* kpadWork, s_KPad_Effect* effect, s32 arg2, u32 arg3) // 0x80089644
{
    u32              var1;
    s_KPad_Channel*  ptr0;
    s_VibScriptStep* ptr1;

#if VERSION_REGION_IS(PAL)
    arg3 *= 6;
#else
    arg3 *= 5;
#endif

    effect->output_18       = 0;
    effect->hasOutput_14_31 = 0;
    while (true)
    {
        ptr1 = effect->step_10;
        if (!ptr1)
        {
            break;
        }

        if (g_VibScript_StepHandlers[ptr1->data_4.opcode_4](kpadWork, effect, ptr1, &arg3) == 0)
        {
            break;
        }
    }

    if (effect->hasOutput_14_31 && (arg2 & effect->mask_1E))
    {
        var1 = effect->output_18;
        ptr0 = &kpadWork->channels_C[effect->channel_14_24];
        if (ptr0->strength_4 < var1)
        {
            ptr0->strength_4 = var1;
        }
    }

    return effect->step_10 != NULL;
}

bool func_8008973C(s_KPadWork* kpadWork, s_KPad_Effect* effect, s_VibScriptStep* step, u32* arg3)
{
    if (!kpadWork || effect)
    {
        return false;
    }

    if (step == NULL || step->data_4.command.opcode_4 != VibScriptOp_Command)
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
            func_80089840(kpadWork);
            break;

        case VibScriptCommand_RemoveByMask:
            func_8008989C(kpadWork, step->data_4.command.arg_6, step->data_4.command.callback_C);
            break;

        case VibScriptCommand_RemoveByTag:
            func_8008992C(kpadWork, step->data_4.command.arg_6, step->data_4.command.callback_C);
            break;

        case VibScriptCommand_RemoveById:
            func_800899BC(kpadWork, step->data_4.command.value_8);
            break;
    }

    return true;
}

void func_80089840(s_KPadWork* kpadWork) // 0x80089840
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

void func_8008989C(s_KPadWork* kpadWork, u16 arg1, s32 (*arg2)(u16, s32)) // 0x8008989C
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

        if (arg2(curr->mask_1E, arg1) != 0)
        {
            KPad_EffectRemove(kpadWork, curr);
        }

        curr = next;
    }
}

void func_8008992C(s_KPadWork* kpadWork, u16 arg1, s32 (*arg2)(u16, s32)) // 0x8008992C
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

        if (arg2(curr->tag_1C, arg1) != 0)
        {
            KPad_EffectRemove(kpadWork, curr);
        }

        curr = next;
    }
}

void func_800899BC(s_KPadWork* kpadWork, s32 effect) // 0x800899BC
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

        if (curr->id_C == effect)
        {
            KPad_EffectRemove(kpadWork, curr);
        }

        curr = next;
    }
}

bool func_80089A30(s_KPadWork* kpadWork, s_KPad_Effect* effect, s_VibScriptStep* step, u32* arg3) // 0x80089A30
{
    s32            temp_s3;
    s_KPad_Effect* next;
    s_KPad_Effect* head;
    s_KPad_Effect* current;
    s_KPad_Effect* ptr;

    ptr = effect;

    if (kpadWork == NULL)
    {
        return false;
    }

    if (step == NULL)
    {
        return false;
    }

    if (step->data_4.opcode_4 != VibScriptOp_Start)
    {
        return false;
    }

    temp_s3 = step->data_4.start.id_8;

    if (step->data_4.start.skipIfActive_5_5 || step->data_4.start.replaceActive_5_6)
    {
        current = kpadWork->effects_18.next_0;
        head    = KPAD_EFFECTS_HEAD(kpadWork);

        if (step->data_4.start.skipIfActive_5_5)
        {
            while (current != head)
            {
                if (current->id_C == temp_s3)
                {
                    if (ptr != NULL)
                    {
                        ptr->step_10 = NULL;
                    }

                    return false;
                }

                current = current->links_0.next_0;
            }
        }
        else
        {
            while (current != head)
            {
                next = current->links_0.next_0;
                if ((ptr == NULL || current != ptr) && current->id_C == temp_s3)
                {
                    KPad_EffectRemove(kpadWork, current);
                }
                current = next;
            }
        }
    }

    if (ptr == NULL)
    {
        ptr = KPad_EffectAdd(kpadWork, step->data_4.start.priority_C);
        if (ptr == NULL)
        {
            return false;
        }
        ptr->strength_14_16 = *arg3;
    }

    ptr->step_10   = step->next_0;
    ptr->elapsed_14_0 = 0;
    ptr->id_C    = temp_s3;

    if (step->data_4.start.hasTag_5_7)
    {
        ptr->tag_1C = step->data_4.start.tag_6;
    }

    return true;
}

bool func_80089BB8(s_KPadWork* kpadWork, s_KPad_Effect* effect, s_VibScriptStep* step, u32* arg3) // 0x80089BB8
{
    s_KPad_Effect* temp_a0;
    s_KPad_Effect* temp_v0;
    u32            var_s1;

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
        var_s1 = step->data_4.chain.priority_C;
        effect   = KPad_EffectAdd(kpadWork, var_s1);
        if (effect == NULL)
        {
            return false;
        }
        effect->strength_14_16 = *arg3;
    }
    else
    {
        if (effect->step_10 != step)
        {
            effect->step_10 = NULL;
            return false;
        }
        else
        {
            var_s1 = effect->priority_8;
        }
    }

    effect->step_10   = step->next_0;
    effect->elapsed_14_0 = 0;

    if (step->data_4.chain.hasTag_5_7)
    {
        effect->tag_1C = step->data_4.chain.tag_6;
    }

    temp_v0 = KPad_EffectAdd(kpadWork, var_s1);
    if (temp_v0 != NULL)
    {
        temp_a0                                 = temp_v0->links_0.next_0;
        temp_a0->links_0.prev_4                 = temp_v0->links_0.prev_4;
        temp_a0->links_0.prev_4->links_0.next_0 = temp_a0;

        temp_a0                 = effect->links_0.next_0;
        temp_v0->links_0.prev_4 = effect;
        temp_v0->links_0.next_0 = temp_a0;
        temp_a0->links_0.prev_4 = temp_v0;

        effect->links_0.next_0 = temp_v0;

        temp_v0->step_10    = step->data_4.chain.spawned_8;
        temp_v0->elapsed_14_0  = 0;
        temp_v0->id_C     = effect->id_C;
        temp_v0->strength_14_16 = effect->strength_14_16;
        temp_v0->tag_1C    = effect->tag_1C;
    }

    return true;
}

bool func_80089D0C(s_KPadWork* kpadWork, s_KPad_Effect* effect, s_VibScriptStep* step, u32* arg3) // 0x80089D0C
{
    if (kpadWork == NULL || step == NULL)
    {
        return false;
    }

    if (step->data_4.pulse.opcode_4 == VibScriptOp_Pulse)
    {
        if (effect != NULL)
        {
            effect->channel_14_24 = step->data_4.pulse.channel_5 & 0x7F;
            effect->mask_1E    = step->data_4.pulse.mask_6;
            effect->step_10    = step->next_0;
            effect->hasOutput_14_31 = 1;
            effect->output_18    = effect->strength_14_16 << 24;
            return false;
        }
        else
        {
            effect = KPad_EffectAdd(kpadWork, step->data_4.pulse.priority_C);
            if (effect != NULL)
            {
                effect->step_10    = step;
                effect->strength_14_16 = *arg3;
                effect->id_C     = step->data_4.pulse.id_8;
                return true;
            }
        }
    }

    return false;
}

bool func_80089DF0(s_KPadWork* kpadWork, s_KPad_Effect* effect, s_VibScriptStep* step, u32* arg3) // 0x80089DF0
{
    u32 temp_a0;
    s32 temp_a0_2;
    u32 temp_lo;
    u8* temp_t5;
    s32 var_a1_2;
    u32 var_t1;
    u32 var_t3;
    u32 temp_t6;
    u32 temp_v1_3;
    u32 var_t0;
    u32 temp_t2;
    u32 temp_t4;
    u32 var_a1;
    u32 var_a2;
    u32 var_t7;
    u32 temp_s0;
    s32 temp;
    s32 temp2;
    u32 temp3;
    u32 temp4;
    u64 temp5;

    if (step == NULL || effect == NULL || step->data_4.opcode_4 != VibScriptOp_Waveform)
    {
        return false;
    }

    if (effect->step_10 != step)
    {
        effect->step_10 = NULL;
        return false;
    }

    temp_t6 = effect->elapsed_14_0;

    if (!effect->hasOutput_14_31)
    {
        effect->hasOutput_14_31 = 1;

        temp_s0   = effect->strength_14_16;
        var_t1    = step->data_4.waveform.channel_5 & 0x7F;
        temp_t5   = step->data_4.waveform.samples_8;
        temp_t4   = step->data_4.waveform.ticksPerSample_C_16;
        var_a1    = step->data_4.waveform.bytesPerSample_C_28;
        temp_a0_2 = step->data_4.waveform.interpolate_C_31;

        effect->channel_14_24 = var_t1;
        effect->mask_1E    = step->data_4.waveform.mask_6;

        if (temp_t4 == 0)
        {
            var_t0 = temp_t6;
            var_t7 = 0;
        }
        else
        {
            var_t0 = (temp_t6 / temp_t4);
            var_t7 = temp_t6 % temp_t4;
        }

        if (temp_a0_2 == 0)
        {
            var_t7 = 0;
        }

        if (var_a1 == 0)
        {
            temp_t2 = var_t0 + 1;
            temp    = (temp_t5[var_t0 >> 3] >> (var_t0 & 7));
            var_t3  = temp & 1;
            var_t3  = -var_t3;

            temp2  = temp_t5[temp_t2 >> 3] >> (temp_t2 & 7);
            var_t1 = temp2 & 1;
            var_t1 = -var_t1;
        }
        else
        {
            if (var_a1 >= 5)
            {
                var_a1 = 4;
            }

            var_t0 = var_t0 * var_a1;

            var_t3 = 0;
            var_t1 = 0;

            for (var_a2 = var_a1, var_a1_2 = 0x18, temp_t2 = var_t0 + var_a2; var_a2 != 0;
                 var_a2--, var_a1_2 -= 8, var_t0++, temp_t2++)
            {
                var_t3 += temp_t5[var_t0] << var_a1_2;
                var_t1 += temp_t5[temp_t2] << var_a1_2;
            }
        }

        if (var_t7 == 0)
        {
            temp_a0 = var_t3;
        }
        else
        {
            temp_lo = ((UINT_MAX / temp_t4) + 1) * var_t7;
            temp3   = (var_t3 * (u64)-temp_lo) >> 32;
            temp4   = (var_t1 * (u64)temp_lo) >> 32;
            temp_a0 = temp4 + temp3;
        }

        temp5 = (u64)temp_a0 * temp_s0 >> 7;

        if (temp5 > UINT_MAX)
        {
            temp_a0 = UINT_MAX;
        }
        else
        {
            temp_a0 = temp5;
        }

        effect->output_18 = temp_a0;
    }

    temp_a0   = *arg3;
    temp_v1_3 = step->data_4.waveform.duration_C_0;
    temp_t6   = temp_t6 + temp_a0;

    if (temp_t6 >= temp_v1_3)
    {
        effect->step_10   = step->next_0;
        effect->elapsed_14_0 = 0;
        temp_a0          = temp_t6 - temp_v1_3;
        *arg3            = temp_a0;

        return true;
    }

    effect->elapsed_14_0 = temp_t6;
    *arg3            = 0;

    return false;
}
