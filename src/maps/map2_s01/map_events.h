    // [0]
    {
        .completeFlag = EventFlag_M2S01_HealthDrink,
        .triggerType       = EventTriggerType_TouchFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 25,
        .sysState          = SysState_EventCallback,
        .eventParam        = 2, // `func_800CED44`
    },
    // [1]
    {
        .completeFlag = EventFlag_181,
        .triggerType       = EventTriggerType_Tick,
        .sysState          = SysState_Fmv,
        .eventParam        = 12,
        .transitionFlags   = AreaTransitionFlag_SkipFadeIn,
    },
    // [2]
    {
        .requiredFlag = EventFlag_181,
        .completeFlag = EventFlag_179,
        .triggerType       = EventTriggerType_Tick,
        .sysState          = SysState_EventCallback,
        .eventParam        = 3, // `func_800CED88`
    },
    // [3]
    {
        .completeFlag = EventFlag_M2S01_PickupFlauros,
        .triggerType       = EventTriggerType_TouchObbFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 1,
        .sysState          = SysState_EventCallback,
        .eventParam        = 4, // `func_800CF798`
    },
    // [4]
    {
        .completeFlag = EventFlag_M2S01_PickupDrawbridgeKey,
        .triggerType       = EventTriggerType_TouchObbFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 1,
        .sysState          = SysState_EventCallback,
        .eventParam        = 5, // `func_800CF7C4`
    },
    // [5]
    {
        .triggerType     = EventTriggerType_TouchObbFacing,
        .activationType  = EventActivationType_Button,
        .mapPointIdx     = 8,
        .sysState        = SysState_ReadMessage,
        .eventParam      = 37,
        .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
    },
    // [6]
    {
        .triggerType     = EventTriggerType_TouchObbFacing,
        .activationType  = EventActivationType_Button,
        .mapPointIdx     = 9,
        .sysState        = SysState_ReadMessage,
        .eventParam      = 37,
        .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
    },
    // [7]
    {
        .completeFlag = EventFlag_MapMark_OldTown_BalkanChurchCheck,
        .triggerType       = EventTriggerType_TouchObbFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 5,
        .sysState          = SysState_LoadOverlay,
        .eventParam        = 7,
        .sfxPairIdx        = SfxPairIdx_2,
        .mapIdx            = MapIdx_MAP2_S00,
    },
    // [8]
    {
        .triggerType    = EventTriggerType_TouchObbFacing,
        .activationType = EventActivationType_Button,
        .mapPointIdx    = 10,
        .sysState       = SysState_SaveMenu1,
        .eventParam     = 7,
    },
    // [9]
    {
        .completeFlag = EventFlag_180,
        .triggerType       = EventTriggerType_TouchObbFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 2,
        .sysState          = SysState_EventCallback,
        .eventParam        = 1, // `func_800CEC30`
        .transitionFlags   = AreaTransitionFlag_UnfreezeWorld,
    },
    // [10]
    {
        .triggerType     = EventTriggerType_TouchObbFacing,
        .activationType  = EventActivationType_Button,
        .mapPointIdx     = 2,
        .sysState        = SysState_EventCallback,
        .eventParam      = 1, // `func_800CEC30`
        .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
    },
    // [11]
    {
        .triggerType     = EventTriggerType_TouchObbFacing,
        .activationType  = EventActivationType_Button,
        .mapPointIdx     = 3,
        .sysState        = SysState_EventCallback,
        .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
    },
    // [12]
    {
        .triggerType     = EventTriggerType_TouchObbFacing,
        .activationType  = EventActivationType_Button,
        .mapPointIdx     = 4,
        .sysState        = SysState_EventCallback,
        .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
    },
    // [13]
    {
        .triggerType    = EventTriggerType_TouchObbFacing,
        .activationType = EventActivationType_Button,
        .mapPointIdx    = 11,
        .sysState       = SysState_ReadMessage,
        .eventParam     = MAP_MSG(40),
    },
    // [14]
    {
        .triggerType    = EventTriggerType_TouchObbFacing,
        .activationType = EventActivationType_Button,
        .mapPointIdx    = 12,
        .sysState       = SysState_ReadMessage,
        .eventParam     = MAP_MSG(41),
    },
    // [15]
    {
        .triggerType    = EventTriggerType_TouchObbFacing,
        .activationType = EventActivationType_Button,
        .mapPointIdx    = 13,
        .sysState       = SysState_ReadMessage,
        .eventParam     = MAP_MSG(42),
    },
    // [16]
    {
        .triggerType    = EventTriggerType_TouchObbFacing,
        .activationType = EventActivationType_Button,
        .mapPointIdx    = 14,
        .sysState       = SysState_ReadMessage,
        .eventParam     = MAP_MSG(42),
    },
    // [17]
    {
        .triggerType    = EventTriggerType_TouchObbFacing,
        .activationType = EventActivationType_Button,
        .mapPointIdx    = 15,
        .sysState       = SysState_ReadMessage,
        .eventParam     = MAP_MSG(42),
    },
    // [18]
    {
        .triggerType    = EventTriggerType_TouchObbFacing,
        .activationType = EventActivationType_Button,
        .mapPointIdx    = 16,
        .sysState       = SysState_ReadMessage,
        .eventParam     = MAP_MSG(42),
    },
    // [19]
    {
        .triggerType    = EventTriggerType_TouchObbFacing,
        .activationType = EventActivationType_Button,
        .mapPointIdx    = 17,
        .sysState       = SysState_ReadMessage,
        .eventParam     = MAP_MSG(42),
    },
    // [20]
    {
        .triggerType    = EventTriggerType_TouchObbFacing,
        .activationType = EventActivationType_Button,
        .mapPointIdx    = 18,
        .sysState       = SysState_ReadMessage,
        .eventParam     = MAP_MSG(42),
    },
    // [21]
    {
        .triggerType    = EventTriggerType_TouchObbFacing,
        .activationType = EventActivationType_Button,
        .mapPointIdx    = 19,
        .sysState       = SysState_ReadMessage,
        .eventParam     = MAP_MSG(43),
    },
    // [22]
    {
        .triggerType    = EventTriggerType_TouchObbFacing,
        .activationType = EventActivationType_Button,
        .mapPointIdx    = 20,
        .sysState       = SysState_ReadMessage,
        .eventParam     = MAP_MSG(43),
    },
    // [23]
    {
        .triggerType    = EventTriggerType_TouchObbFacing,
        .activationType = EventActivationType_Button,
        .mapPointIdx    = 21,
        .sysState       = SysState_ReadMessage,
        .eventParam     = MAP_MSG(43),
    },
    // [24]
    {
        .triggerType    = EventTriggerType_TouchObbFacing,
        .activationType = EventActivationType_Button,
        .mapPointIdx    = 22,
        .sysState       = SysState_ReadMessage,
        .eventParam     = MAP_MSG(43),
    },
    // [25]
    {
        .triggerType    = EventTriggerType_TouchObbFacing,
        .activationType = EventActivationType_Button,
        .mapPointIdx    = 23,
        .sysState       = SysState_ReadMessage,
        .eventParam     = MAP_MSG(43),
    },
    // [26]
    {
        .triggerType    = EventTriggerType_TouchObbFacing,
        .activationType = EventActivationType_Button,
        .mapPointIdx    = 24,
        .sysState       = SysState_ReadMessage,
        .eventParam     = MAP_MSG(43),
    },
    // [27]
    {
        .triggerType = EventTriggerType_EndOfArray,
    },
