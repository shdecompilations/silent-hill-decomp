    // [0]
    {
        .completeFlag = EventFlag_237,
        .triggerType       = EventTriggerType_Tick,
        .sysState          = SysState_EventCallback,
        .eventParam        = 5, // `func_800D0608`
    },
    // [1]
    {
        .requiredFlag = EventFlag_237,
        .completeFlag = EventFlag_238,
        .triggerType       = EventTriggerType_Tick,
        .sysState          = SysState_LoadRoom,
        .eventParam        = 1,
        .mapIdx            = MapIdx_MAP0_S02,
    },
    // [2]
    {
        .triggerType     = EventTriggerType_TouchObbFacing,
        .activationType  = EventActivationType_Button,
        .mapPointIdx     = 6,
        .sysState        = SysState_EventCallback,
        .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
    },
    // [3]
    {
        .triggerType     = EventTriggerType_TouchObbFacing,
        .activationType  = EventActivationType_Button,
        .mapPointIdx     = 7,
        .sysState        = SysState_EventCallback,
        .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
    },
    // [4]
    {
        .triggerType     = EventTriggerType_TouchObbFacing,
        .activationType  = EventActivationType_Button,
        .mapPointIdx     = 8,
        .sysState        = SysState_EventCallback,
        .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
    },
    // [5]
    {
        .triggerType     = EventTriggerType_TouchObbFacing,
        .activationType  = EventActivationType_Button,
        .mapPointIdx     = 9,
        .sysState        = SysState_EventCallback,
        .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
    },
    // [6]
    {
        .triggerType     = EventTriggerType_TouchObbFacing,
        .activationType  = EventActivationType_Button,
        .mapPointIdx     = 10,
        .sysState        = SysState_EventCallback,
        .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
    },
    // [7]
    {
        .triggerType     = EventTriggerType_TouchObbFacing,
        .activationType  = EventActivationType_Button,
        .mapPointIdx     = 12,
        .sysState        = SysState_EventCallback,
        .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
    },
    // [8]
    {
        .triggerType     = EventTriggerType_TouchObbFacing,
        .activationType  = EventActivationType_Button,
        .mapPointIdx     = 13,
        .sysState        = SysState_EventCallback,
        .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
    },
    // [9]
    {
        .requiredFlag = EventFlag_233,
        .triggerType       = EventTriggerType_TouchObbFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 4,
        .sysState          = SysState_EventCallback,
        .eventParam        = 1, // `MapEvent_DoorLocked`
        .transitionFlags   = AreaTransitionFlag_UnfreezeWorld,
    },
    // [10]
    {
        .triggerType     = EventTriggerType_TouchObbFacing,
        .activationType  = EventActivationType_Button,
        .mapPointIdx     = 3,
        .sysState        = SysState_LoadRoom,
        .eventParam      = 4,
        .sfxPairIdx = SfxPairIdx_13,
        .setCameraRail      = 1,
    },
    // [11]
    {
        .triggerType     = EventTriggerType_TouchObbFacing,
        .activationType  = EventActivationType_Button,
        .mapPointIdx     = 4,
        .sysState        = SysState_LoadRoom,
        .eventParam      = 3,
        .sfxPairIdx = SfxPairIdx_13,
        .setCameraRail      = 1,
    },
    // [12]
    {
        .requiredFlag = EventFlag_234,
        .triggerType       = EventTriggerType_TouchObbFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 11,
        .sysState          = SysState_EventCallback,
        .eventParam        = 1, // `MapEvent_DoorLocked`
        .transitionFlags   = AreaTransitionFlag_UnfreezeWorld,
    },
    // [13]
    {
        .requiredFlag = EventFlag_236,
        .triggerType       = EventTriggerType_TouchObbFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 5,
        .sysState          = SysState_LoadRoom,
        .eventParam        = 11,
        .sfxPairIdx        = SfxPairIdx_13,
        .setCameraRail     = true,
    },
    // [14]
    {
        .requiredFlag = EventFlag_236,
        .triggerType       = EventTriggerType_TouchObbFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 11,
        .sysState          = SysState_LoadRoom,
        .eventParam        = 5,
        .sfxPairIdx        = SfxPairIdx_13,
        .setCameraRail     = true,
    },
    // [15]
    {
        .completeFlag = EventFlag_235,
        .triggerType       = EventTriggerType_TouchObbFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 5,
        .sysState          = SysState_LoadRoom,
        .eventParam        = 11,
        .transitionFlags   = AreaTransitionFlag_SkipFadeIn,
        .sfxPairIdx        = SfxPairIdx_13,
        .setCameraRail     = true,
    },
    // [16]
    {
        .requiredFlag = EventFlag_235,
        .completeFlag = EventFlag_236,
        .triggerType       = EventTriggerType_Tick,
        .sysState          = SysState_EventCallback,
        .eventParam        = 4, // `func_800D03FC`
    },
    // [17]
    {
        .triggerType = EventTriggerType_TouchAabb,
        .mapPointIdx = 14,
        .sysState    = SysState_LoadOverlay,
        .eventParam  = 15,
        .mapIdx      = MapIdx_MAP3_S03,
    },
    // [18]
    {
        .triggerType    = EventTriggerType_TouchObbFacing,
        .activationType = EventActivationType_Button,
        .mapPointIdx    = 1,
        .sysState       = SysState_ReadMessage,
        .eventParam     = 15,
    },
    // [19]
    {
        .triggerType    = EventTriggerType_TouchObbFacing,
        .activationType = EventActivationType_Button,
        .mapPointIdx    = 2,
        .sysState       = SysState_ReadMessage,
        .eventParam     = 15,
    },
    // [20]
    {
        .triggerType = EventTriggerType_EndOfArray,
    },
