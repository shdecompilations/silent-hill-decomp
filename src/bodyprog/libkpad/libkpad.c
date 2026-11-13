#include "common.h"
#include "bodyprog/libkpad/libkpad.h"

#include <psyq/libgte.h>
#include <psyq/libpad.h>
#include <psyq/stdio.h>

#include "bodyprog/math/math.h"

/** KPad library (assumed name), code used by SH1 to handle different kinds of vibration/rumble supported PS1 pads.
 * Unlike SH1's GCC 2.8.1 this was compiled with GCC 2.7.2. Likely built separate to SH1 and included as a library.
 *
 * Library is initialized with `KPad_WorkInit` which zeroes the passed `s_KPadWork` pointer.
 * `KPad_ChannelsInit` and `KPad_EffectListInit` are called afterward with pointers for channel/effect data to be zeroed.
 * Lastly `KPad_HandlerList_Init` is called which links some basic controller handlers into the handler list:
 * - g_KPad_HandlerNoPad
 * - g_KPad_HandlerStandardPad
 * - g_KPad_HandlerCtp1
 *
 * After initialization, games can then optionally link in support for either single-motor controllers (DualAnalog) or dual-motor (DualShock), or both.
 * via `KPad_HandlerList_AddCtp1SingleMotor` and `KPad_HandlerList_AddDualShock`, which links in the following handlers:
 * - g_KPad_HandlerCtp1SingleMotor
 * - g_KPad_HandlerDualShock
 *
 * Handler list works by `KPad_DeviceDetect` passing the pad through each entry in the list, until a handler decides it's a pad that it should handle.
 * The handler can then pass actuator data to the controller using the right protocol, and returns true to prevent later handlers from running on the pad.
 * Optional handlers above get installed before the regular handlers, so StandardPad/Ctp1 handlers would only execute if both DualShock/SingleMotor handlers refused it.
 *
 * In SH1, the order of the handlers should end up as
 * - g_KPad_HandlerNoPad
 * - g_KPad_HandlerDualShock
 * - g_KPad_HandlerCtp1SingleMotor
 * - g_KPad_HandlerStandardPad
 * - g_KPad_HandlerCtp1
 *
 * TODO: Describe more of the effect list side, add function @briefs.
 */

// Data passed to `PadSetActAlign`
u8 g_KPad_DualShockAlignData[6] = { 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xFF };

s_KPad_Handler g_KPad_HandlerNoPad = {
    .deviceType_4 = KPadDeviceType_None,
    .func_C       = KPad_Handler_NoPad, // Handles `PadStateDiscon` / `PadStateFindPad`
};

// Never claims a pad itself, `KPad_Handler_Ctp1` sets this type for digital/analog pads that no motor handler took.
// Entry only gives `KPad_HandlerList_Find` a match for that type, no align data so `KPad_ActuatorSetup` skips alignment.
s_KPad_Handler g_KPad_HandlerStandardPad = {
    .deviceType_4 = KPadDeviceType_StandardPad,
    .func_C       = NULL,
};

s_KPad_Handler g_KPad_HandlerCtp1 = {
    .deviceType_4 = KPadDeviceType_Other,
    .func_C       = KPad_Handler_Ctp1, // Handles `PadStateFindCTP1`
};

s_KPad_Handler g_KPad_HandlerCtp1SingleMotor = {
    .deviceType_4  = KPadDeviceType_Ctp1SingleMotor,
    .isDualMotor_5 = 0,
    .numMotors_6   = 1,
    .func_C        = KPad_Handler_Ctp1SingleMotor,
};

s_KPad_Handler g_KPad_HandlerDualShock = {
    .deviceType_4  = KPadDeviceType_DualShock,
    .isDualMotor_5 = 1,
    .numMotors_6   = 2,
    .alignData_8   = g_KPad_DualShockAlignData,
    .func_C        = KPad_Handler_DualShock,
};

bool KPad_WorkInit(s_KPadWork* work, u32 port) // 0x8009E198
{
    s_KPad_Status status;
    s32           ret;
    const s32     strength = 0x80;

    ret = work != NULL;
    if (ret)
    {
        s16 result = ret;
        port      &= 0x13;

        work->actuatorData_4        = 0;
        work->channelInfo_8         = (s_KPad_ChannelInfo){ 0 };
        work->channels_C            = NULL;
        work->freeEffects_10.next_0 = NULL;

        status = (s_KPad_Status){
            .port_0_0         = 0,
            .strength_0_8     = strength,
            .isInit_0_16      = 1,
            .isPaused_0_17    = 1,
            .isEnabled_0_18   = 1,
            .actCount_0_19    = 0,
            .isAlignDone_0_22 = 0,
            .isActSet_0_23    = 0,
            .deviceType_0_24  = KPadDeviceType_None,
        };

        ret             = port;
        status.port_0_0 = ret;

        work->status_0 = status;

        result = (u8)result;
        return result; // @hack This is close to the pattern used in other functions below, 
                       // but doesn't match with the same `ret = result` line they use, strange.
    }

    return ret;
}

s32 KPad_VibrationResume(s_KPadWork* work) // 0x8009E230
{
    s_KPad_Status status;
    s32           ret;

    status = work->status_0;
    ret    = status.isPaused_0_17;
    if (ret)
    {
        s16 result = ret;

        ret                  = ret & 2;
        status.isActSet_0_23 = ret;
        status.isPaused_0_17 = 0;
        work->status_0       = status;

        result = (u8)result;
        ret    = result;
    }

    return ret;
}

s32 KPad_VibrationPause(s_KPadWork* work) // 0x8009E268
{
    s_KPad_Status status;
    s32           ret;

    status = work->status_0;
    ret    = status.isPaused_0_17 ^ 1;
    if (ret)
    {
        s16 result = ret;

        ret                  = ret & 2;
        status.isActSet_0_23 = ret;
        status.isPaused_0_17 = 1;
        work->status_0       = status;

        result = (u8)result;
        ret    = result;
    }

    return ret;
}

s32 KPad_VibrationEnable(s_KPadWork* work) // 0x8009E2A0
{
    s_KPad_Status status;
    s32           ret;

    status = work->status_0;
    ret    = status.isEnabled_0_18 ^ 1;
    if (ret)
    {
        s16 result = ret;

        ret                   = ret & 2;
        status.isActSet_0_23  = ret;
        status.isEnabled_0_18 = 1;
        work->status_0        = status;

        result = (u8)result;
        ret    = result;
    }

    return ret;
}

s32 KPad_VibrationDisable(s_KPadWork* work) // 0x8009E2D8
{
    s_KPad_Status status;
    s32           ret;

    status = work->status_0;
    ret    = status.isEnabled_0_18;
    if (ret)
    {
        s16 result = ret;

        ret                   = ret & 2;
        status.isActSet_0_23  = ret;
        status.isEnabled_0_18 = 0;
        work->status_0        = status;

        result = (u8)result;
        ret    = result;
    }

    return ret;
}

s32 KPad_ChannelsInit(s_KPadWork* work, s_KPad_Channel* arg1, s32 arg2) // 0x8009E310
{
    s_KPad_ActuatorInfo tmp;
    s32                 ret;

    ret = work != NULL;
    if (ret == 0)
    {
        return ret;
    }

    ret = work->status_0.isInit_0_16;
    if (ret != 0)
    {
        if (arg1 == NULL)
        {
            arg2 = 0;
        }

        work->channels_C               = arg1;
        work->channelInfo_8.capacity_3 = arg2;

        while (arg2 > 0)
        {
            tmp.bits_0_0         = 0;
            tmp.bits_0_8         = 0;
            tmp.currentDrain_0_9 = 0;
            tmp.dataOffset_0_16  = 0;
            tmp.bits_0_19        = 0;
            tmp.dataSize_0_24    = 0;
            tmp.bits_0_27        = 0;
            arg2--;
            arg1->info_0     = tmp;
            arg1->strength_4 = 0;
            arg1++;
        }
    }

    return ret;
}

bool KPad_Handler_NoPad(s_KPadWork* work, s32 padState, s32 padInfoCurId, s32 padInfoCurExId) // 0x8009E3B0
{
    s_KPad_Status tmp;

    tmp = work->status_0;

    if (padState == PadStateDiscon || padState == PadStateFindPad)
    {
        if (tmp.deviceType_0_24 != KPadDeviceType_None)
        {
            tmp.deviceType_0_24  = KPadDeviceType_None;
            tmp.actCount_0_19    = 0;
            tmp.isActSet_0_23    = 0;
            tmp.isAlignDone_0_22 = 0;

            if (padState == PadStateDiscon)
            {
                tmp.isEnabled_0_18 = 0;
            }

            work->actuatorData_4        = 0;
            work->channelInfo_8.field_0 = 0;
            work->channelInfo_8.count_2 = 0;
            work->status_0              = tmp;
        }

        return true;
    }

    if (tmp.deviceType_0_24 == KPadDeviceType_None)
    {
        return false;
    }

    return tmp.deviceType_0_24 != KPadDeviceType_StandardPad;
}

bool KPad_Handler_Ctp1(s_KPadWork* work, s32 padState, s32 padInfoCurId, s32 padInfoCurExId) // 0x8009E438
{
    s_KPad_Status tmp;
    bool          isChanged;

    tmp = work->status_0;

    // "Check for controller connection with controllers other than DUAL SHOCK (Complete the acquisition of controller information)".
    if (padState == PadStateFindCTP1)
    {
        if (padInfoCurId == PadTerminalType_16Button |
            padInfoCurId == PadTerminalType_AnalogJoystick |
            padInfoCurId == PadTerminalType_AnalogController) // `|` Dev mistake?
        {
            isChanged           = tmp.deviceType_0_24 != KPadDeviceType_StandardPad;
            tmp.deviceType_0_24 = KPadDeviceType_StandardPad;
        }
        else
        {
            isChanged           = tmp.deviceType_0_24 != KPadDeviceType_Other;
            tmp.deviceType_0_24 = KPadDeviceType_Other;
        }
    }
    else
    {
        isChanged           = tmp.deviceType_0_24 != KPadDeviceType_None;
        tmp.deviceType_0_24 = KPadDeviceType_None;
    }

    if (isChanged)
    {
        work->actuatorData_4        = 0;
        work->channelInfo_8.field_0 = 0;
        work->channelInfo_8.count_2 = 0;
        tmp.actCount_0_19    = 0;
        tmp.isActSet_0_23    = 0;
        tmp.isAlignDone_0_22 = 0;
        work->status_0              = tmp;
    }

    return true;
}

s_KPad_Handler* KPad_HandlerList_Init(void) // 0x8009E4F8
{
    g_KPad_HandlerNoPad.next_0       = &g_KPad_HandlerStandardPad;
    g_KPad_HandlerStandardPad.next_0 = &g_KPad_HandlerCtp1;
    g_KPad_HandlerCtp1.next_0        = NULL;

    return &g_KPad_HandlerNoPad;
}

s_KPad_Handler* KPad_HandlerList_Find(s_KPad_Handler* node, s32 key) // 0x8009E520
{
    for (; node; node = node->next_0)
    {
        if (node->deviceType_4 == key)
        {
            return node;
        }
    }

    return NULL;
}

s32 KPad_DeviceDetect(s_KPadWork* work, s_KPad_Handler* node) // 0x8009E550
{
    s32 padState;
    s32 padInfoCurId;
    s32 padInfoCurExId;
    u8  padPort;

    padPort        = work->status_0.port_0_0;
    padState       = PadGetState(padPort);
    padInfoCurId   = PadInfoMode(padPort, InfoModeCurID, 0);
    padInfoCurExId = PadInfoMode(padPort, InfoModeCurExID, 0);

    while (node)
    {
        if (node->func_C != NULL && node->func_C(work, padState, padInfoCurId, padInfoCurExId) == 1)
        {
            break;
        }

        node = node->next_0;
    }

    return padState;
}

bool KPad_ActuatorSetup(s_KPadWork* work, s32 padState, s_KPad_Handler* head) // 0x8009E61C
{
    s_KPad_Status status;
    u8*           pattern;
    u8*           act;
    s32           length;

    status = work->status_0;

    if (!status.isActSet_0_23)
    {
        pattern = (u8*)&work->actuatorData_4;
        if (status.isEnabled_0_18 && !status.isPaused_0_17)
        {
            length = status.actCount_0_19;
        }
        else
        {
            length = 0;
        }
        status.isActSet_0_23 = 1;
        PadSetAct(status.port_0_0, pattern, length);
    }

    if (!status.isAlignDone_0_22)
    {
        head = KPad_HandlerList_Find(head, status.deviceType_0_24);
        act  = (head == NULL) ? NULL : head->alignData_8;
        if (act == NULL)
        {
            status.isAlignDone_0_22 = true;
        }
        else if (padState == PadStateStable)
        {
            status.isAlignDone_0_22 = PadSetActAlign(status.port_0_0, act);
        }
    }

    work->status_0 = status;

    return status.isAlignDone_0_22;
}

bool KPad_ActuatorDataUpdate(s_KPadWork* work) // 0x8009E718
{
    s_KPad_Status   status;
    u32             packedVal;
    u32             scale;
    s32             outLen;
    s32             outIdx;
    s32             i;
    u64             tmp;
    u8              inLen;
    s_KPad_Channel* in;
    u8*             out;

    status = work->status_0;
    if (!(status.isEnabled_0_18 && !status.isPaused_0_17))
    {
        return false;
    }

    // 0-128, but seems to only ever be set to 128.
    scale = status.strength_0_8;

    in  = work->channels_C;
    out = (u8*)&work->actuatorData_4;
    for (i = work->channelInfo_8.count_2; i != 0; i--)
    {
        // Contains up to 4 actuator data bytes packed into a `u32`, even though there are only two actuators max.
        packedVal = in->strength_4;
        outLen    = in->info_0.dataSize_0_24;

        // Scale original actuator values by `[0.0f, 1.0f]` (`[0, 128]`). However, scale seems to only ever be 1.0f, so this is useless?
        tmp = ((u64)packedVal * scale) >> 7;
        if (tmp > UINT_MAX)
        {
            // If overflowed, max out every actuator.
            packedVal = UINT_MAX;
        }
        else
        {
            packedVal = tmp;
        }

        outIdx = in->info_0.dataOffset_0_16;

        // Ensure there's data for at least 1 actuator.
        if (outLen == 0)
        {
            outLen = 1;
            if (packedVal != 0)
            {
                // If any actuator values were not zero, enable at least the first one.
                // The other bytes will be ignored because outLen = 1.
                // Note that it's actuator 0 in a dualshock/dual analog that only takes 0 or 1,
                // but outIdx could be non-zero, though it's unclear if it ever is.
                packedVal = 0x01FFFFFF;
            }
        }

        // Change outLen bytes of actuator data starting with byte outIdx.
        while (outLen != 0)
        {
            // Get high byte and store it in actuator data.
            out[outIdx] = packedVal >> 24;
            outLen--;
            outIdx++;

            // Shift next byte to high byte.
            packedVal <<= 8;
        }

        in++;
    }

    return true;
}

bool KPad_HandlerList_AddCtp1SingleMotor(s_KPad_Handler* node) // 0x8009E7D8
{
    s_KPad_Handler* curNode;

    for (curNode = node; curNode; curNode = curNode->next_0)
    {
        if (curNode == &g_KPad_HandlerCtp1SingleMotor)
        {
            return false;
        }
    }

    curNode                              = node->next_0;
    node->next_0                         = &g_KPad_HandlerCtp1SingleMotor;
    g_KPad_HandlerCtp1SingleMotor.next_0 = curNode;

    return true;
}

bool KPad_Handler_Ctp1SingleMotor(s_KPadWork* work, s32 padState, s32 padInfoCurId, s32 padInfoCurExId) // 0x8009E82C
{
    s32                 actCount;
    s_KPad_Channel*     cur;
    s_KPad_ActuatorInfo tmp;
    s32                 i;
    s32                 count;
    s_KPad_Status       status;
    u8                  unused[24];

    if (padState == PadStateFindCTP1 && ((0xB0u >> padInfoCurId) & 0x1))
    {
        work->actuatorData_4        = 0x40;
        work->channelInfo_8.field_0 = 0;
        count                       = work->channelInfo_8.capacity_3;

        status                  = work->status_0;
        cur                     = work->channels_C;
        status.deviceType_0_24  = KPadDeviceType_Ctp1SingleMotor;
        status.isActSet_0_23    = 0;
        status.isAlignDone_0_22 = 0;
        actCount                = 0;
        i                       = 1;

        if (count < 2)
        {
            i = count;
        }
        if (i > 0)
        {
            tmp.bits_0_0         = 1;
            tmp.bits_0_8         = 0;
            tmp.currentDrain_0_9 = 10;
            tmp.dataOffset_0_16  = 1;
            tmp.dataSize_0_24    = 0;
            tmp.bits_0_19        = 1;
            tmp.bits_0_27        = 2;
            cur->info_0         = tmp;
            actCount            = 2;
            i                   = 1;
        }

        work->channelInfo_8.count_2 = i;
        status.actCount_0_19        = actCount;
        work->status_0              = status;
        tmp                         = (s_KPad_ActuatorInfo){ 0 };
        cur++;

        while (i < count)
        {
            cur->info_0 = tmp;
            cur++;
            i++;
        };

        return true;
    }

    return false;
}

bool KPad_HandlerList_AddDualShock(s_KPad_Handler* node) // 0x8009E97C
{
    s_KPad_Handler* curNode;

    for (curNode = node; curNode; curNode = curNode->next_0)
    {
        if (curNode == &g_KPad_HandlerDualShock)
        {
            return false;
        }
    }

    curNode                        = node->next_0;
    node->next_0                   = &g_KPad_HandlerDualShock;
    g_KPad_HandlerDualShock.next_0 = curNode;

    return true;
}

bool KPad_Handler_DualShock(s_KPadWork* work, s32 padState, s32 padInfoCurId, s32 padInfoCurExId) // 0x8009E9D0
{
    s_KPad_Status       status;
    s_KPad_Channel*     cur;
    s_KPad_ActuatorInfo tmp;
    s32                 actCount;
    s32                 channelsCapacity;
    s32                 i;
    u32                 mask = (1 << padState);
    u8                  unused[24];

    status = work->status_0;

    if (mask & ((1 << PadStateReqInfo) | (1 << PadStateExecCmd)))
    {
        return status.deviceType_0_24 == KPadDeviceType_DualShock;
    }
    if (padState != PadStateStable)
    {
        return false;
    }

    if (((0x90u >> padInfoCurExId) & 0x1))
    {
        status.deviceType_0_24  = KPadDeviceType_DualShock;
        status.isActSet_0_23    = 0;
        status.isAlignDone_0_22 = 0;

        cur                        = work->channels_C;
        channelsCapacity            = work->channelInfo_8.capacity_3;
        work->actuatorData_4        = 0;
        work->channelInfo_8.field_0 = 0;

        actCount = 0;
        i        = 2;

        if (channelsCapacity < 3)
        {
            i = channelsCapacity;
        }

        if (i > 0)
        {
            tmp.bits_0_0         = 2;
            tmp.bits_0_8         = 0;
            tmp.currentDrain_0_9 = 10;
            tmp.dataOffset_0_16  = 0;
            tmp.dataSize_0_24    = 0;
            tmp.bits_0_19        = 1;
            tmp.bits_0_27        = 2;
            cur->info_0        = tmp;
            actCount            = 1;
        }

        if (i >= 2)
        {
            tmp.bits_0_0         = 3;
            tmp.bits_0_8         = 0;
            tmp.currentDrain_0_9 = 20;
            tmp.dataOffset_0_16  = 1;
            tmp.dataSize_0_24    = 1;
            tmp.bits_0_19        = 1;
            tmp.bits_0_27        = 1;
            cur++;
            cur->info_0 = tmp;

            actCount = 2;
            i        = 2;
        }

        work->channelInfo_8.count_2 = i;
        status.actCount_0_19        = actCount;
        work->status_0              = status;
        tmp                         = (s_KPad_ActuatorInfo){ 0 };
        cur++;

        while (i < channelsCapacity)
        {
            cur->info_0 = tmp;
            cur++;
            i++;
        };

        return true;
    }

    return false;
}

bool KPad_EffectListInit(s_KPadWork* work, s_KPad_Effect* node, s32 count) // 0x8009EBB8
{
    s_KPad_Effect* cur;
    s_KPad_Effect* head;

    if (count >= 128)
    {
        return false;
    }

    if (count <= 0 || work == NULL)
    {
        return false;
    }

    if (node == NULL)
    {
        return false;
    }

    head = NULL;
    cur  = node;

    for (; count > 0; count--)
    {
        cur->links_0.next_0 = head;
        cur->links_0.prev_4 = NULL;
        head                = cur++;
    }

    work->freeEffects_10.next_0 = head;
    work->freeEffects_10.prev_4 = NULL;

    work->effects_18.next_0 = KPAD_EFFECTS_HEAD(work);
    work->effects_18.prev_4 = KPAD_EFFECTS_HEAD(work);

    return true;
}

bool KPad_EffectRemove(s_KPadWork* work, s_KPad_Effect* node) // 0x8009EC1C
{
    s_KPad_Effect* next;
    s_KPad_Effect* prev;

    next = node->links_0.next_0;
    prev = node->links_0.prev_4;

    // @bug `node == NULL` check happens after already trying to read from `node`
    if (node == NULL)
    {
        return false;
    }

    if (node == KPAD_EFFECTS_HEAD(work))
    {
        return false;
    }

    if (node->links_0.prev_4 == NULL)
    {
        return false;
    }

    // Remove node from doubly-linked list by making its neighbors skip over it.
    prev->links_0.next_0 = next;
    next->links_0.prev_4 = prev;

    // Add node to front of the free list.
    next                        = work->freeEffects_10.next_0;
    work->freeEffects_10.next_0 = node;
    node->links_0.next_0        = next;

    return true;
}

bool KPad_EffectEvictLowest(s_KPadWork* work, s16 priority) // 0x8009EC64
{
    s32            priority32;
    s_KPad_Effect* lowest;
    s_KPad_Effect* head;
    s_KPad_Effect* node;
    s_KPad_Effect* cur;
    bool           canEvict;

    head       = KPAD_EFFECTS_HEAD(work);
    node       = work->freeEffects_10.next_0;
    priority32 = priority;
    lowest     = work->effects_18.prev_4;

    // @bug `lowest == head` is only true when the effect list is empty, `!=` was likely intended.
    canEvict = ((!node) & (lowest == head) & (priority32 >= lowest->priority_8)) != 0;
    if (!canEvict)
    {
        return canEvict;
    }

    // Remove `lowest` from the effect list.
    cur                  = lowest->links_0.prev_4;
    cur->links_0.next_0  = head;
    head->links_0.prev_4 = cur;

    // Insert in front of `node`.
    work->freeEffects_10.next_0 = lowest;
    lowest->links_0.next_0      = node;

    return true;
}

s_KPad_Effect* KPad_EffectAdd(s_KPadWork* work, s32 key) // 0x8009ECCC
{
    s_KPad_Effect* head;
    s_KPad_Effect* node;
    s_KPad_Effect* next;
    s_KPad_Effect* prev;
    s_KPad_Effect* tmp;
    s_KPad_Effect* cur;
    s32            midpoint;

    node = work->freeEffects_10.next_0;
    head = KPAD_EFFECTS_HEAD(work);

    if (node == NULL)
    {
        return NULL;
    }

    cur                         = node->links_0.next_0;
    next                        = head->links_0.next_0;
    work->freeEffects_10.next_0 = cur;

    tmp  = work->effects_18.prev_4;
    prev = tmp;

    midpoint = (next->priority_8 + prev->priority_8) >> 1;
    if (midpoint <= key)
    {
        prev = head;
        cur  = next;

        while (cur != head && key < cur->priority_8)
        {
            prev = cur;
            cur  = prev->links_0.next_0;
        }
    }
    else
    {
        cur = head;
    }

    cur->links_0.prev_4  = node;
    prev->links_0.next_0 = node;
    node->links_0.next_0 = cur;

    tmp                  = prev;
    node->links_0.prev_4 = tmp;

    node->priority_8 = key;

    return node;
}

s_KPad_Effect* KPad_EffectListHeadGet(s_KPadWork* work) // 0x8009ED74
{
    return KPAD_EFFECTS_HEAD(work);
}

s_KPad_Effect* KPad_EffectNextGet(s_KPad_Effect* node) // 0x8009ED7C
{
    if (node != NULL)
    {
        return node->links_0.next_0;
    }

    return NULL;
}

s_KPad_Effect* KPad_EffectPrevGet(s_KPad_Effect* node) // 0x8009ED90
{
    if (node != NULL)
    {
        return node->links_0.prev_4;
    }

    return NULL;
}
