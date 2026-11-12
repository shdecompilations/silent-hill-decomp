#ifndef _LIBKPAD_H
#define _LIBKPAD_H

// Konami library for handling DualShock vibration?
// Built with 2.7.2-970404 / 2.7.2-cdk compiler.

/** Based on LIBOVR.PDF `Buffer Data Format` section. */
typedef enum _PadTerminalType
{
    PadTerminalType_Mouse               = 1,
    PadTerminalType_16ButtonAnalog      = 2,
    PadTerminalType_GunControllerKonami = 3,
    PadTerminalType_16Button            = 4,
    PadTerminalType_AnalogJoystick      = 5,
    PadTerminalType_GunControllerNamco  = 6,
    PadTerminalType_AnalogController    = 7,
    PadTerminalType_MultitapAdapter     = 8
} e_PadTerminalType;

/** @brief Pad kinds used by the `s_KPad_Handler` functions. */
typedef enum _KPadDeviceType
{
    KPadDeviceType_None            = 0,
    KPadDeviceType_DualShock       = 1, /** Expanded protocol pad with a small on/off and a large variable motor. */
    KPadDeviceType_Ctp1SingleMotor = 2, /** Non-expanded Sony pad, driven like the one motor SCPH-1180. */
    KPadDeviceType_StandardPad     = 3,
    KPadDeviceType_Other           = 4  /** Any other non-expanded device, e.g. mouse, NeGcon or light gun. */
} e_KPadDeviceType;

/** @brief Describes one pad actuator. Unread fields may include what `PadInfoAct` reports. */
typedef struct
{
    u32 bits_0_0         : 8;
    u32 bits_0_8         : 1;
    u32 currentDrain_0_9 : 7; // libpad current drain units, 10 for an on/off motor and 20 for a variable one.
    u32 dataOffset_0_16  : 3; // First byte in `s_KPadWork::actuatorData_4` driving this actuator.
    u32 bits_0_19        : 5; // Always 1, possibly the actuator function (vibration).
    u32 dataSize_0_24    : 3; // Data bytes, 0 for an on/off motor that takes a single byte.
    u32 bits_0_27        : 5; // 2 for on/off motors, 1 for variable speed.
} s_KPad_ActuatorInfo;
STATIC_ASSERT_SIZEOF(s_KPad_ActuatorInfo, 4);

typedef struct
{
    s_KPad_ActuatorInfo info_0;
    s32                 strength_4; // Requested level, left-aligned to the top byte.
} s_KPad_Channel;
STATIC_ASSERT_SIZEOF(s_KPad_Channel, 8);

/** @brief Links of an `s_KPad_Effect` list. Also used on its own as a list head in `s_KPadWork`. */
typedef struct
{
    struct _KPad_Effect* next_0;
    struct _KPad_Effect* prev_4;
} s_KPad_EffectList;
STATIC_ASSERT_SIZEOF(s_KPad_EffectList, 8);

/**
 * @brief Active vibration effect, kept in `s_KPadWork::effects_18` sorted by descending priority.
 * libkpad only reads `links_0` and `priority_8`, everything after is state owned by the caller.
 */
typedef struct _KPad_Effect
{
    s_KPad_EffectList      links_0;
    s32                    priority_8;
    s32                    id_C;
    struct _VibScriptStep* step_10;
    u32                    elapsed_14_0    : 16; // Ticks spent in the current step.
    u32                    strength_14_16  : 8;  // Level the effect was started with, `0x80` is 1.0.
    u32                    channel_14_24   : 7;
    u32                    hasOutput_14_31 : 1;  // `output_18` is valid and gets applied to the channel.
    u32                    output_18;
    u16                    tag_1C;
    u16                    mask_1E;
} s_KPad_Effect;
STATIC_ASSERT_SIZEOF(s_KPad_Effect, 32);

/** @brief Status bits for the KPad system. */
typedef struct
{
    u32 port_0_0         : 8; // Pad port: bit 4 selects port 1 or 2, bits 0-1 the multitap slot.
    u32 strength_0_8     : 8; // Actuator strength, `0x80` is 1.0.
    u32 isInit_0_16      : 1;
    u32 isPaused_0_17    : 1;
    u32 isEnabled_0_18   : 1;
    u32 actCount_0_19    : 3; // Number of actuator bytes passed to `PadSetAct`.
    u32 isAlignDone_0_22 : 1; // Set once `PadSetActAlign` succeeds.
    u32 isActSet_0_23    : 1; // Set once `PadSetAct` was called, cleared to resend.
    u32 deviceType_0_24  : 8; // `e_KPadDeviceType` of the connected pad.
} s_KPad_Status;
STATIC_ASSERT_SIZEOF(s_KPad_Status, 4);

/** @brief Tracks entries in the `s_KPadWork::channels_C` list. */
typedef struct
{
    s32 field_0    : 16;
    u8 count_2     : 8; // Number of active entries in `s_KPadWork::channels_C`.
    u32 capacity_3 : 8; // Number of entries allocated in `s_KPadWork::channels_C`.
} s_KPad_ChannelInfo;
STATIC_ASSERT_SIZEOF(s_KPad_ChannelInfo, 4);

// Konami vibration library state.
typedef struct
{
    s_KPad_Status      status_0;
    s32                actuatorData_4;
    s_KPad_ChannelInfo channelInfo_8;
    s_KPad_Channel*    channels_C;
    s_KPad_EffectList  freeEffects_10; // Only uses `next_0`.
    s_KPad_EffectList  effects_18;     // Sorted by descending `priority_8`.
} s_KPadWork;
STATIC_ASSERT_SIZEOF(s_KPadWork, 32);

/** @brief Head of `kpad`'s active effect list, as an `s_KPad_Effect`. Only the links are valid. */
#define KPAD_EFFECTS_HEAD(kpad) \
    ((s_KPad_Effect*)&(kpad)->effects_18)

/** @brief Handler function definition, called with the pad state to detect and set up the connected pad. */
typedef struct _KPad_Handler
{
    struct _KPad_Handler* next_0;
    u8                    deviceType_4; // `e_KPadDeviceType` this handler detects.
    u8                    isDualMotor_5; // Assumed name, never read/written, only set to 1 for DualShock handler.
    u8                    numMotors_6;   // Assumed name, never read/written, value set matches channel count `func_C` handles.
    // 1 byte padding.
    s32                   alignData_8; // Data passed to `PadSetActAlign`.
    s32                   (*func_C)(s_KPadWork* work, s32 padState, s32 padInfoCurId, s32 padInfoCurExId);
} s_KPad_Handler;
STATIC_ASSERT_SIZEOF(s_KPad_Handler, 16);

bool KPad_WorkInit(s_KPadWork* work, u32 port);

s32 KPad_VibrationResume(s_KPadWork* work);

s32 KPad_VibrationPause(s_KPadWork* work);

s32 KPad_VibrationEnable(s_KPadWork* work);

s32 KPad_VibrationDisable(s_KPadWork* work);

s32 KPad_ChannelsInit(s_KPadWork* work, s_KPad_Channel* arg1, s32 arg2);

bool KPad_Handler_NoPad(s_KPadWork* work, s32 padState, s32 padInfoCurId, s32 padInfoCurExId);

bool KPad_Handler_Ctp1(s_KPadWork* work, s32 padState, s32 padInfoCurId, s32 padInfoCurExId);

s_KPad_Handler* KPad_HandlerList_Init(void);

s_KPad_Handler* KPad_HandlerList_Find(s_KPad_Handler* node, s32 key);

s32 KPad_DeviceDetect(s_KPadWork* work, s_KPad_Handler* node);

bool KPad_ActuatorSetup(s_KPadWork* work, s32 padState, s_KPad_Handler* head);

bool KPad_ActuatorDataUpdate(s_KPadWork* work);

bool KPad_HandlerList_AddCtp1SingleMotor(s_KPad_Handler* node);

bool KPad_Handler_Ctp1SingleMotor(s_KPadWork* work, s32 padState, s32 padInfoCurId, s32 padInfoCurExId);

bool KPad_HandlerList_AddDualShock(s_KPad_Handler* node);

bool KPad_Handler_DualShock(s_KPadWork* work, s32 padState, s32 padInfoCurId, s32 padInfoCurExId);

bool KPad_EffectListInit(s_KPadWork* work, s_KPad_Effect* node, s32 count);

bool KPad_EffectRemove(s_KPadWork* work, s_KPad_Effect* node);

/** @brief @unused Frees the lowest priority effect if the free list is empty and `priority` is not below it. */
bool KPad_EffectEvictLowest(s_KPadWork* work, s16 priority);

s_KPad_Effect* KPad_EffectAdd(s_KPadWork* work, s32 key);

s_KPad_Effect* KPad_EffectListHeadGet(s_KPadWork* work);

s_KPad_Effect* KPad_EffectNextGet(s_KPad_Effect* node);

s_KPad_Effect* KPad_EffectPrevGet(s_KPad_Effect* node);

#endif
