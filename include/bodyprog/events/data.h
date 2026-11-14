#ifndef _BODYPROG_EVENTS_EVENTDATA_H
#define _BODYPROG_EVENTS_EVENTDATA_H

/** @brief SFX pair used for area transitions (e.g. door opening and closing). */
typedef struct _AreaLoadSfx
{
    u16 sfx_0;
    u16 sfx_2;
} s_AreaLoadSfx;

extern const s_AreaLoadSfx SFX_PAIRS[25];

extern s8 g_PaperMapFileIdxs[];

extern s8 g_PaperMapMarkingFileIdxs[];

extern char* g_CommonWorldObjectNames[8];

extern s_FsImageDesc D_800A9A04;

extern bool g_IsLoadingFinished;

extern s32 g_MapEventSysState; /** `e_InvItemId` */

extern u32 g_MapEventParam;

extern s32 g_MapEventLastUsedItem; /** `e_InvItemId` */

/** Radio pitch state based on the distance from the player to an enemy. Range: `[0, 3]`. */
extern s32 g_RadioPitchState;

#endif
