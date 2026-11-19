#include "bodyprog/bodyprog.h"

s_EventData MAP_EVENTS[13] = {
    // [0]
    {
        .requiredFlag = EventFlag_587,
        .completeFlag = EventFlag_588,
        .triggerType  = EventTriggerType_Tick,
        .sysState     = SysState_EventCallback,
        .eventParam   = 7, // `func_800E9AC8`
    },
    // [1]
    {
        .requiredFlag    = EventFlag_583,
        .completeFlag    = EventFlag_589,
        .triggerType     = EventTriggerType_Tick,
        .sysState        = SysState_Fmv,
        .eventParam      = 3,
        .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
    },
    // [2]
    {
        .requiredFlag = EventFlag_584,
        .completeFlag = EventFlag_590,
        .triggerType  = EventTriggerType_Tick,
        .sysState     = SysState_EventCallback,
        .eventParam   = 6, // `func_800E3E84`
    },
    // [3]
    {
        .requiredFlag    = EventFlag_585,
        .completeFlag    = EventFlag_590,
        .triggerType     = EventTriggerType_Tick,
        .sysState        = SysState_Fmv,
        .eventParam      = 2,
        .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
    },
    // [4]
    {
        .requiredFlag    = EventFlag_586,
        .completeFlag    = EventFlag_590,
        .triggerType     = EventTriggerType_Tick,
        .sysState        = SysState_Fmv,
        .eventParam      = 1,
        .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
    },
    // [5]
    {
        .requiredFlag    = EventFlag_590,
        .completeFlag    = EventFlag_589,
        .triggerType     = EventTriggerType_Tick,
        .sysState        = SysState_Fmv,
        .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
    },
    // [6]
    {
        .requiredFlag = EventFlag_589,
        .triggerType  = EventTriggerType_Tick,
        .sysState     = SysState_EventCallback,
        .eventParam   = 8, // `func_800E9C28`
    },
    // [7]
    {
        .completeFlag = EventFlag_577,
        .triggerType  = EventTriggerType_Tick,
        .sysState     = SysState_EventCallback,
        .eventParam   = 2, // `func_800E3390`
    },
    // [8]
    {
        .requiredFlag = EventFlag_578,
        .completeFlag = EventFlag_579,
        .triggerType  = EventTriggerType_Tick,
        .sysState     = SysState_EventCallback,
        .eventParam   = 3, // `func_800E3B6C`
    },
    // [9]
    {
        .requiredFlag    = EventFlag_579,
        .completeFlag    = EventFlag_580,
        .triggerType     = EventTriggerType_Tick,
        .sysState        = SysState_Fmv,
        .eventParam      = 4,
        .transitionFlags = AreaTransitionFlag_SkipFadeIn,
    },
    // [10]
    {
        .requiredFlag = EventFlag_580,
        .triggerType  = EventTriggerType_Tick,
        .sysState     = SysState_EventCallback,
        .eventParam   = 4, // `func_800E3C48`
    },
    // [11]
    {
        .requiredFlag = EventFlag_582,
        .triggerType  = EventTriggerType_Tick,
        .sysState     = SysState_EventCallback,
        .eventParam   = 5, // `func_800E3D18`
    },
    // [12]
    {
        .triggerType = EventTriggerType_EndOfArray,
    },
};
