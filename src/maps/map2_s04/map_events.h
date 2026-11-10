    // [0]
    {
        .completeFlag = EventFlag_M2S04_ShotgunShells0,
        .triggerType       = EventTriggerType_TouchFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 24,
        .sysState          = SysState_EventCallback,
        .eventParam        = 2, // `func_800CCF9C`
    },
    // [1]
    {
        .completeFlag = EventFlag_M2S04_HandgunBullets0,
        .triggerType       = EventTriggerType_TouchFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 25,
        .sysState          = SysState_EventCallback,
        .eventParam        = 2, // `func_800CCF9C`
    },
    // [2]
    {
        .completeFlag = EventFlag_M2S04_HandgunBullets1,
        .triggerType       = EventTriggerType_TouchFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 26,
        .sysState          = SysState_EventCallback,
        .eventParam        = 2, // `func_800CCF9C`
    },
    // [3]
    {
        .completeFlag = EventFlag_M2S04_ShotgunShells1,
        .triggerType       = EventTriggerType_TouchFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 27,
        .sysState          = SysState_EventCallback,
        .eventParam        = 2, // `func_800CCF9C`
    },
    // [4]
    {
        .completeFlag = EventFlag_M2S04_HandgunBullets2,
        .triggerType       = EventTriggerType_TouchFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 28,
        .sysState          = SysState_EventCallback,
        .eventParam        = 2, // `func_800CCF9C`
    },
    // [5]
    {
        .completeFlag = EventFlag_M2S04_HandgunBullets3,
        .triggerType       = EventTriggerType_TouchFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 29,
        .sysState          = SysState_EventCallback,
        .eventParam        = 2, // `func_800CCF9C`
    },
    // [6]
    {
        .completeFlag = EventFlag_M2S04_RifleShells,
        .triggerType       = EventTriggerType_TouchFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 30,
        .sysState          = SysState_EventCallback,
        .eventParam        = 2, // `func_800CCF9C`
    },
    // [7]
    {
        .triggerType    = EventTriggerType_TouchObbFacing,
        .activationType = EventActivationType_Button,
        .mapPointIdx    = 10,
        .sysState       = SysState_SaveMenu1,
        .eventParam     = 9,
    },
    // [8]
    {
        .triggerType    = EventTriggerType_TouchObbFacing,
        .activationType = EventActivationType_Button,
        .mapPointIdx    = 11,
        .sysState       = SysState_SaveMenu1,
        .eventParam     = 9,
    },
    // [9]
    {
        .triggerType    = EventTriggerType_TouchObbFacing,
        .activationType = EventActivationType_Button,
        .mapPointIdx    = 20,
        .sysState       = SysState_SaveMenu1,
        .eventParam     = 9,
    },
    // [10]
    {
        .triggerType    = EventTriggerType_TouchObbFacing,
        .activationType = EventActivationType_Button,
        .mapPointIdx    = 21,
        .sysState       = SysState_SaveMenu1,
        .eventParam     = 9,
    },
    // [11]
    {
        .completeFlag = EventFlag_M1S02_PickupShotgun,
        .triggerType       = EventTriggerType_TouchObbFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 22,
        .sysState          = SysState_EventCallback,
        .eventParam        = 6, // `MapEvent_ShotgunTake`
    },
    // [12]
    {
        .completeFlag = EventFlag_M1S02_PickupShotgun,
        .triggerType       = EventTriggerType_TouchObbFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 23,
        .sysState          = SysState_EventCallback,
        .eventParam        = 6, // `MapEvent_ShotgunTake`
    },
    // [13]
    {
        .triggerType    = EventTriggerType_TouchObbFacing,
        .activationType = EventActivationType_Button,
        .mapPointIdx    = 13,
        .sysState       = SysState_ReadMessage,
        .eventParam     = MAP_MSG(24),
    },
    // [14]
    {
        .completeFlag = EventFlag_M2S00_PickupMap,
        .triggerType       = EventTriggerType_TouchObbFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 12,
        .sysState          = SysState_EventCallback,
        .eventParam        = 5, // `MapEvent_PaperMapTake`
    },
    // [15]
    {
        .triggerType     = EventTriggerType_TouchObbFacing,
        .activationType  = EventActivationType_Button,
        .mapPointIdx     = 4,
        .sysState        = SysState_LoadOverlay,
        .eventParam      = 9,
        .sfxPairIdx = SfxPairIdx_2,
        .mapIdx          = MapIdx_MAP2_S02,
    },
    // [16]
    {
        .triggerType    = EventTriggerType_TouchObbFacing,
        .activationType = EventActivationType_Button,
        .mapPointIdx    = 7,
        .sysState       = SysState_EventCallback,
        .eventParam     = 3, // `func_800CD050`
    },
    // [17]
    {
        .triggerType    = EventTriggerType_TouchObbFacing,
        .activationType = EventActivationType_Button,
        .mapPointIdx    = 8,
        .sysState       = SysState_EventCallback,
        .eventParam     = 4, // `MapEvent_PoliceChalkboardInspect`
    },
    // [18]
    {
        .triggerType     = EventTriggerType_TouchObbFacing,
        .activationType  = EventActivationType_Button,
        .mapPointIdx     = 2,
        .sysState        = SysState_LoadRoom,
        .eventParam      = 5,
        .sfxPairIdx = SfxPairIdx_1,
    },
    // [19]
    {
        .triggerType     = EventTriggerType_TouchObbFacing,
        .activationType  = EventActivationType_Button,
        .mapPointIdx     = 5,
        .sysState        = SysState_LoadRoom,
        .eventParam      = 2,
        .sfxPairIdx = SfxPairIdx_1,
    },
    // [20]
    {
        .triggerType     = EventTriggerType_TouchObbFacing,
        .activationType  = EventActivationType_Button,
        .mapPointIdx     = 3,
        .sysState        = SysState_EventCallback,
        .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
    },
    // [21]
    {
        .triggerType     = EventTriggerType_TouchObbFacing,
        .activationType  = EventActivationType_Button,
        .mapPointIdx     = 6,
        .sysState        = SysState_EventCallback,
        .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
    },
    // [22]
    {
        .triggerType     = EventTriggerType_TouchObbFacing,
        .activationType  = EventActivationType_Button,
        .mapPointIdx     = 16,
        .sysState        = SysState_LoadOverlay,
        .eventParam      = 17,
        .sfxPairIdx = SfxPairIdx_13,
        .mapIdx          = MapIdx_MAP4_S02,
    },
    // [23]
    {
        .triggerType     = EventTriggerType_TouchObbFacing,
        .activationType  = EventActivationType_Button,
        .mapPointIdx     = 18,
        .sysState        = SysState_EventCallback,
        .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
    },
    // [24]
    {
        .triggerType     = EventTriggerType_TouchObbFacing,
        .activationType  = EventActivationType_Button,
        .mapPointIdx     = 19,
        .sysState        = SysState_EventCallback,
        .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
    },
    // [25]
    {
        .triggerType    = EventTriggerType_TouchObbFacing,
        .activationType = EventActivationType_Button,
        .mapPointIdx    = 14,
        .sysState       = SysState_ReadMessage,
        .eventParam     = MAP_MSG(25),
    },
    // [26]
    {
        .triggerType    = EventTriggerType_TouchObbFacing,
        .activationType = EventActivationType_Button,
        .mapPointIdx    = 15,
        .sysState       = SysState_ReadMessage,
        .eventParam     = MAP_MSG(25),
    },
    // [27]
    {
        .triggerType = EventTriggerType_EndOfArray,
    },
