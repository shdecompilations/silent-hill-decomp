#include "bodyprog/bodyprog.h"

s_EventData MAP_EVENTS[4] = {
    // [0]
    {
        .completeFlag = EventFlag_130,
        .triggerType       = EventTriggerType_Tick,
        .sysState          = SysState_EventCallback,
        .eventParam        = 2, // `func_800D49AC`
    },
    // [1]
    {
        .requiredFlag = EventFlag_131,
        .completeFlag = EventFlag_132,
        .triggerType       = EventTriggerType_Tick,
        .sysState          = SysState_EventCallback,
        .eventParam        = 3, // `func_800D4D1C`
    },
    // [2]
    {
        .requiredFlag = EventFlag_132,
        .triggerType       = EventTriggerType_Tick,
        .sysState          = SysState_LoadOverlay,
        .eventParam        = 1,
        .transitionFlags   = AreaTransitionFlag_SkipAmbienceUpdate,
        .mapIdx            = MapIdx_MAP1_S06,
    },
    // [3]
    {
        .triggerType = EventTriggerType_EndOfArray,
    },
};
