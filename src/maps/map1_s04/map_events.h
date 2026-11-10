    // [0]
    {
        .completeFlag = EventFlag_59,
        .triggerType       = EventTriggerType_Tick,
        .sysState          = SysState_ReadMessage,
        .eventParam        = 15,
        .transitionFlags   = AreaTransitionFlag_UnfreezeWorld,
    },
    // [1]
    {
        .triggerType     = EventTriggerType_Tick,
        .sysState        = SysState_EventCallback,
        .eventParam      = 1, // `func_800CCA2C`
        .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
    },
    // [2]
    {
        .triggerType = EventTriggerType_EndOfArray,
    },
