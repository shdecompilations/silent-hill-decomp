#include "bodyprog/bodyprog.h"

s_EventData MAP_EVENTS[53] = {
    // [0]
    {
        .completeFlag = EventFlag_M3S04_HealthDrink0,
        .triggerType       = EventTriggerType_TouchFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 52,
        .sysState          = SysState_EventCallback,
        .eventParam        = 2, // `func_800D2470`
    },
    // [1]
    {
        .completeFlag = EventFlag_M3S04_HealthDrink1,
        .triggerType       = EventTriggerType_TouchObbFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 53,
        .sysState          = SysState_EventCallback,
        .eventParam        = 2, // `func_800D2470`
    },
    // [2]
    {
        .completeFlag = EventFlag_M3S04_HealthDrink2,
        .triggerType       = EventTriggerType_TouchObbFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 54,
        .sysState          = SysState_EventCallback,
        .eventParam        = 2, // `func_800D2470`
    },
    // [3]
    {
        .completeFlag = EventFlag_M3S04_HealthDrink3,
        .triggerType       = EventTriggerType_TouchObbFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 55,
        .sysState          = SysState_EventCallback,
        .eventParam        = 2, // `func_800D2470`
    },
    // [4]
    {
        .completeFlag = EventFlag_M3S04_HandgunBullets,
        .triggerType       = EventTriggerType_TouchFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 56,
        .sysState          = SysState_EventCallback,
        .eventParam        = 2, // `func_800D2470`
    },
    // [5]
    {
        .requiredFlag = EventFlag_292,
        .completeFlag = EventFlag_293,
        .triggerType       = EventTriggerType_Tick,
        .sysState          = SysState_Fmv,
        .eventParam        = 10,
        .transitionFlags   = AreaTransitionFlag_SkipFadeIn,
    },
    // [6]
    {
        .requiredFlag = EventFlag_293,
        .completeFlag = EventFlag_294,
        .triggerType       = EventTriggerType_Tick,
        .sysState          = SysState_EventCallback,
        .eventParam        = 4, // `func_800D2668`
    },
    // [7]
    {
        .requiredFlag = EventFlag_294,
        .triggerType       = EventTriggerType_Tick,
        .sysState          = SysState_LoadOverlay,
        .eventParam        = 39,
        .transitionFlags   = AreaTransitionFlag_SkipAmbienceUpdate,
        .mapIdx            = MapIdx_MAP3_S06,
    },
    // [8]
    {
        .triggerType    = EventTriggerType_TouchObbFacing,
        .activationType = EventActivationType_Button,
        .mapPointIdx    = 38,
        .sysState       = SysState_SaveMenu1,
        .eventParam     = 12,
    },
    // [9]
    {
        .completeFlag = EventFlag_M3S04_PickupPlateOfQueen,
        .triggerType       = EventTriggerType_TouchObbFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 37,
        .sysState          = SysState_EventCallback,
        .eventParam        = 5, // `func_800D2E58`
    },
    // [10]
    {
        .requiredFlag = EventFlag_258,
        .completeFlag = EventFlag_292,
        .triggerType       = EventTriggerType_TouchObbFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 5,
        .sysState          = SysState_LoadRoom,
        .eventParam        = 4,
        .sfxPairIdx        = SfxPairIdx_12,
        .mapIdx            = MapIdx_MAP5_S02,
    },
    // [11]
    {
        .requiredFlag = EventFlag_M3S05_PickupExaminationRoomKey,
        .completeFlag = EventFlag_258,
        .triggerType       = EventTriggerType_TouchObbFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 5,
        .sysState          = SysState_EventCallback,
        .eventParam        = 3, // `func_800D250C`
    },
    // [12]
    {
        .requiredFlag = EventFlag_M3S05_PickupExaminationRoomKey,
        .completeFlag = EventFlag_258,
        .triggerType       = EventTriggerType_TouchObbFacing,
        .activationType    = EventActivationType_Item,
        .mapPointIdx       = 5,
        .requiredItemId    = InvItemId_ExaminationRoomKey,
        .sysState          = SysState_EventCallback,
        .eventParam        = 3, // `func_800D250C`
    },
    // [13]
    {
        .completeFlag = EventFlag_1474,
        .triggerType       = EventTriggerType_TouchObbFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 5,
        .sysState          = SysState_EventCallback,
        .eventParam        = 1, // `func_800D21C4`
        .transitionFlags   = AreaTransitionFlag_UnfreezeWorld,
    },
    // [14]
    {
        .triggerType    = EventTriggerType_TouchObbFacing,
        .activationType = EventActivationType_Button,
        .mapPointIdx    = 40,
        .sysState       = SysState_EventCallback,
        .eventParam     = 6, // `sharedFunc_800D15F0_3_s01`
    },
    // [15]
    {
        .requiredFlag = EventFlag_216,
        .completeFlag = EventFlag_220,
        .triggerType       = EventTriggerType_Tick,
        .sysState          = SysState_LoadOverlay,
        .eventParam        = 43,
        .mapIdx            = MapIdx_MAP3_S05,
    },
    // [16]
    {
        .requiredFlag = EventFlag_217,
        .completeFlag = EventFlag_221,
        .triggerType       = EventTriggerType_Tick,
        .sysState          = SysState_LoadRoom,
        .eventParam        = 17,
    },
    // [17]
    {
        .requiredFlag = EventFlag_218,
        .completeFlag = EventFlag_222,
        .triggerType       = EventTriggerType_Tick,
        .sysState          = SysState_LoadOverlay,
        .eventParam        = 41,
        .mapIdx            = MapIdx_MAP3_S03,
    },
    // [18]
    {
        .requiredFlag = EventFlag_219,
        .completeFlag = EventFlag_223,
        .triggerType       = EventTriggerType_Tick,
        .sysState          = SysState_LoadOverlay,
        .eventParam        = 42,
        .mapIdx            = MapIdx_MAP3_S03,
    },
    // [19]
    {
        .completeFlag = EventFlag_213,
        .triggerType       = EventTriggerType_TouchObbFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 17,
        .sysState          = SysState_LoadRoom,
        .eventParam        = 36,
        .sfxPairIdx        = SfxPairIdx_7,
    },
    // [20]
    {
        .completeFlag = EventFlag_213,
        .triggerType       = EventTriggerType_TouchObbFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 18,
        .sysState          = SysState_LoadRoom,
        .eventParam        = 36,
        .sfxPairIdx        = SfxPairIdx_7,
    },
    // [21]
    {
        .completeFlag = EventFlag_1477,
        .triggerType       = EventTriggerType_TouchObbFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 6,
        .sysState          = SysState_LoadRoom,
        .eventParam        = 26,
        .sfxPairIdx        = SfxPairIdx_12,
        .setCameraRail     = true,
    },
    // [22]
    {
        .completeFlag = EventFlag_1477,
        .triggerType       = EventTriggerType_TouchObbFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 26,
        .sysState          = SysState_LoadRoom,
        .eventParam        = 6,
        .sfxPairIdx        = SfxPairIdx_12,
    },
    // [23]
    {
        .completeFlag = EventFlag_1479,
        .triggerType       = EventTriggerType_TouchObbFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 7,
        .sysState          = SysState_LoadRoom,
        .eventParam        = 24,
        .sfxPairIdx        = SfxPairIdx_12,
        .setCameraRail     = true,
    },
    // [24]
    {
        .completeFlag = EventFlag_1479,
        .triggerType       = EventTriggerType_TouchObbFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 24,
        .sysState          = SysState_LoadRoom,
        .eventParam        = 7,
        .sfxPairIdx        = SfxPairIdx_12,
    },
    // [25]
    {
        .completeFlag = EventFlag_1481,
        .triggerType       = EventTriggerType_TouchObbFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 8,
        .sysState          = SysState_LoadRoom,
        .eventParam        = 23,
        .sfxPairIdx        = SfxPairIdx_12,
        .setCameraRail     = true,
    },
    // [26]
    {
        .completeFlag = EventFlag_1481,
        .triggerType       = EventTriggerType_TouchObbFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 23,
        .sysState          = SysState_LoadRoom,
        .eventParam        = 8,
        .sfxPairIdx        = SfxPairIdx_12,
    },
    // [27]
    {
        .completeFlag = EventFlag_1482,
        .triggerType       = EventTriggerType_TouchObbFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 22,
        .sysState          = SysState_EventCallback,
        .transitionFlags   = AreaTransitionFlag_UnfreezeWorld,
    },
    // [28]
    {
        .completeFlag = EventFlag_1485,
        .triggerType       = EventTriggerType_TouchObbFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 12,
        .sysState          = SysState_LoadRoom,
        .eventParam        = 21,
        .sfxPairIdx        = SfxPairIdx_13,
        .setCameraRail     = true,
    },
    // [29]
    {
        .completeFlag = EventFlag_1485,
        .triggerType       = EventTriggerType_TouchObbFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 21,
        .sysState          = SysState_LoadRoom,
        .eventParam        = 12,
        .sfxPairIdx        = SfxPairIdx_13,
    },
    // [30]
    {
        .completeFlag = EventFlag_1487,
        .triggerType       = EventTriggerType_TouchObbFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 13,
        .sysState          = SysState_LoadRoom,
        .eventParam        = 20,
        .sfxPairIdx        = SfxPairIdx_12,
        .setCameraRail     = true,
    },
    // [31]
    {
        .completeFlag = EventFlag_1487,
        .triggerType       = EventTriggerType_TouchObbFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 20,
        .sysState          = SysState_LoadRoom,
        .eventParam        = 13,
        .sfxPairIdx        = SfxPairIdx_12,
    },
    // [32]
    {
        .completeFlag = EventFlag_1489,
        .triggerType       = EventTriggerType_TouchObbFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 14,
        .sysState          = SysState_LoadRoom,
        .eventParam        = 19,
        .sfxPairIdx        = SfxPairIdx_14,
        .setCameraRail     = true,
    },
    // [33]
    {
        .completeFlag = EventFlag_1489,
        .triggerType       = EventTriggerType_TouchObbFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 19,
        .sysState          = SysState_LoadRoom,
        .eventParam        = 14,
        .sfxPairIdx        = SfxPairIdx_14,
    },
    // [34]
    {
        .completeFlag = EventFlag_1491,
        .triggerType       = EventTriggerType_TouchObbFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 25,
        .sysState          = SysState_LoadRoom,
        .eventParam        = 30,
        .sfxPairIdx        = SfxPairIdx_13,
        .setCameraRail     = true,
    },
    // [35]
    {
        .completeFlag = EventFlag_1491,
        .triggerType       = EventTriggerType_TouchObbFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 30,
        .sysState          = SysState_LoadRoom,
        .eventParam        = 25,
        .sfxPairIdx        = SfxPairIdx_13,
        .setCameraRail     = true,
    },
    // [36]
    {
        .completeFlag = EventFlag_MapMark_FogHospitalBF_StairsArrows,
        .triggerType       = EventTriggerType_TouchObbFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 27,
        .sysState          = SysState_EventCallback,
        .transitionFlags   = AreaTransitionFlag_UnfreezeWorld,
    },
    // [37]
    {
        .completeFlag = EventFlag_1494,
        .triggerType       = EventTriggerType_TouchObbFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 32,
        .sysState          = SysState_EventCallback,
        .transitionFlags   = AreaTransitionFlag_UnfreezeWorld,
    },
    // [38]
    {
        .completeFlag = EventFlag_1496,
        .triggerType       = EventTriggerType_TouchObbFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 31,
        .sysState          = SysState_EventCallback,
        .transitionFlags   = AreaTransitionFlag_UnfreezeWorld,
    },
    // [39]
    {
        .completeFlag = EventFlag_MapMark_AltHospital1F_StairsArrows,
        .triggerType       = EventTriggerType_TouchObbFacing,
        .activationType    = EventActivationType_Button,
        .mapPointIdx       = 33,
        .sysState          = SysState_EventCallback,
        .transitionFlags   = AreaTransitionFlag_UnfreezeWorld,
    },
    // [40]
    {
        .triggerType = EventTriggerType_TouchAabb,
        .mapPointIdx = 34,
        .sysState    = SysState_LoadOverlay,
        .eventParam  = 35,
        .mapIdx      = MapIdx_MAP3_S03,
    },
    // [41]
    {
        .triggerType = EventTriggerType_TouchAabb,
        .mapPointIdx = 15,
        .sysState    = SysState_LoadOverlay,
        .eventParam  = 16,
        .mapIdx      = MapIdx_MAP3_S05,
    },
    // [42]
    {
        .triggerType     = EventTriggerType_TouchObbFacing,
        .activationType  = EventActivationType_Button,
        .mapPointIdx     = 2,
        .sysState        = SysState_EventCallback,
        .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
    },
    // [43]
    {
        .triggerType     = EventTriggerType_TouchObbFacing,
        .activationType  = EventActivationType_Button,
        .mapPointIdx     = 3,
        .sysState        = SysState_EventCallback,
        .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
    },
    // [44]
    {
        .triggerType    = EventTriggerType_TouchObbFacing,
        .activationType = EventActivationType_Button,
        .mapPointIdx    = 44,
        .sysState       = SysState_ReadMessage,
        .eventParam     = 49,
    },
    // [45]
    {
        .triggerType    = EventTriggerType_TouchObbFacing,
        .activationType = EventActivationType_Button,
        .mapPointIdx    = 46,
        .sysState       = SysState_ReadMessage,
        .eventParam     = 50,
    },
    // [46]
    {
        .triggerType    = EventTriggerType_TouchObbFacing,
        .activationType = EventActivationType_Button,
        .mapPointIdx    = 45,
        .sysState       = SysState_ReadMessage,
        .eventParam     = 50,
    },
    // [47]
    {
        .triggerType    = EventTriggerType_TouchObbFacing,
        .activationType = EventActivationType_Button,
        .mapPointIdx    = 47,
        .sysState       = SysState_ReadMessage,
        .eventParam     = 51,
    },
    // [48]
    {
        .triggerType    = EventTriggerType_TouchObbFacing,
        .activationType = EventActivationType_Button,
        .mapPointIdx    = 48,
        .sysState       = SysState_ReadMessage,
        .eventParam     = 53,
    },
    // [49]
    {
        .triggerType    = EventTriggerType_TouchObbFacing,
        .activationType = EventActivationType_Button,
        .mapPointIdx    = 49,
        .sysState       = SysState_ReadMessage,
        .eventParam     = 52,
    },
    // [50]
    {
        .triggerType    = EventTriggerType_TouchObbFacing,
        .activationType = EventActivationType_Button,
        .mapPointIdx    = 50,
        .sysState       = SysState_ReadMessage,
        .eventParam     = 50,
    },
    // [51]
    {
        .triggerType    = EventTriggerType_TouchObbFacing,
        .activationType = EventActivationType_Button,
        .mapPointIdx    = 51,
        .sysState       = SysState_ReadMessage,
        .eventParam     = 50,
    },
    // [52]
    {
        .triggerType = EventTriggerType_EndOfArray,
    },
};
