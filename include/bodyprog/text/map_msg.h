#ifndef _BODYPROG_EVENTS_MAP_MSG_H
#define _BODYPROG_EVENTS_MAP_MSG_H

#define MAP_MSG_UNSKIPPABLE_AUDIO_TYPE_FLAG (1 << 0)
#define DEFAULT_MAP_MESSAGE_LENGTH          99


#if VERSION_REGION_IS(NTSC) || VERSION_REGION_IS(PAL)
    #define MAP_MESSAGE_DISPLAY_ALL_LENGTH 400 /** Long string length is used to display a whole message instantly without a rollout. */
#elif VERSION_REGION_IS(NTSCJ)
    #define MAP_MESSAGE_DISPLAY_ALL_LENGTH 200 /** Long string length is used to display a whole message instantly without a rollout. */
#endif

/** @brief Map message indices.
 *
 * @note Each map has its own messages, with the first 15 hardcoded to be the same.
 */
typedef enum _MapMsgIdx
{
    MapMsgIdx_Yes               = 0,
    MapMsgIdx_No                = 1,
    MapMsgIdx_SaveGame          = 2, /** "Someday, someone may experience..." */
#if VERSION_REGION_IS(PAL)
    MapMsgIdx_SaveGame2         = 3, /** PAL only. Contains latter part of `MapMsgIdx_SaveGame` in languages where it doesn't fit, otherwise "{E}". */
#endif
    MapMsgIdx_NoMap,
    MapMsgIdx_TooDarkForMap,
    MapMsgIdx_FirstAidSelect,
    MapMsgIdx_HealthDrinkSelect,
    MapMsgIdx_AmpouleSelect,
    MapMsgIdx_HandgunAmmoSelect,
    MapMsgIdx_RifleAmmoSelect,
    MapMsgIdx_ShotgunAmmoSelect,
    MapMsgIdx_DoorJammed,
    MapMsgIdx_DoorLocked,
    MapMsgIdx_DoorUnlocked,
    MapMsgIdx_NowMaking /** @unused? */
} e_MapMsgIdx;

/** @brief Map message return codes. Returned by `Gfx_MapMsg_StringDraw`. */
typedef enum _MapMsgReturnCode
{
    MapMsgReturnCode_None        = 0,
    MapMsgReturnCode_Terminate   = 1,
    MapMsgReturnCode_Select2     = 2,
    MapMsgReturnCode_Select3     = 3,
    MapMsgReturnCode_YesOrNo     = 4,
    MapMsgReturnCode_DisplayAll  = 20,
    MapMsgReturnCode_SetByT      = 88,
    MapMsgReturnCode_AlignCenter = 99
} e_MapMsgReturnCode;

/** @brief Map message states.
 *
 * Return states used by `MapMsg_Draw`.
 */
typedef enum _MapMsgState
{
    MapMsgState_Finish       = NO_VALUE, /** Initial setup complete, cutscene timer complete, or input to continue from user received. */
    MapMsgState_Idle         = 0,        /** Continue displaying message. */
    MapMsgState_SelectEntry0 = 1,        /** First entry selected in selection dialog. */
    MapMsgState_SelectEntry1 = 2,        /** Second entry selected in selection dialog. */
    MapMsgState_SelectEntry2 = 3         /** Third entry selected in selection dialog. */
} e_MapMsgState;

/** @brief Map message audio types. */
typedef enum _MapMsgAudioType
{
    MapMsgAudioType_None                 = 0,
    MapMsgAudioType_VoiceClipUnskippable = 1, /** Set by `~J0`. For unskippable cutscenes with individual audio files per message page. */
    MapMsgAudioType_VoiceClipSkippable   = 2, /** Set by `~J1`. For skippable cutscenes with individual audio files per message page. */
    MapMsgAudioType_VoiceStream          = 3  /** Set by `~J2`. For unskippable cutscenes with a single audio file (e.g. video tape cutscene). */
} e_MapMsgAudioType;

typedef struct _MapMsgSelect
{
    /* 0x0 */ s8 maxIdx;
    /* 0x1 */ u8 selectedEntryIdx;
} s_MapMsgSelect;

extern s_MapMsgSelect g_MapMsg_Select;
extern u8             g_MapMsg_AudioType; /** `e_MapMsgAudioType` */
extern s8             g_MapMsg_SelectCancelIdx;

s32 Gfx_MapMsg_Draw(s32 mapMsgIdx);

s32 Gfx_MapMsg_SelectionUpdate(u8 mapMsgIdx, s32* displayLength);

/** @unused */
void func_80036E48(u16* arg0, s16* arg1);

void func_8003708C(s16* ptr0, u16* ptr1);

void func_80037124(void);

#endif
