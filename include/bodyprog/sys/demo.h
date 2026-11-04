#ifndef _BODYPROG_DEMO_H
#define _BODYPROG_DEMO_H

// ======
// ENUMS
// ======

/** @brief Demo states. */
typedef enum _DemoState
{
    DemoState_Exit = -1,
    DemoState_None = 0,
    DemoState_Step = 1
} e_DemoState;

// ========
// STRUCTS
// ========

/** @brief Demo game state data. Stored in `MISC/DEMO****.DAT` files. */
typedef struct _DemoStateData
{
    /* 0x0   */ s_OptionsConfig config;
    /* 0x38  */ s8              __pad_38[200];
    /* 0x100 */ s_Savegame      savegame;
    /* 0x37C */ s8              __pad_37C[1148];
    /* 0x7F8 */ u32             frameCount;
    /* 0x7FC */ u16             randSeed;
} s_DemoState;
STATIC_ASSERT_SIZEOF(s_DemoState, 2048);

/** @brief Per-frame demo playback data. Stored in `MISC/PLAY****.DAT` files. */
typedef struct _DemoPlaybackFrame
{
    /* 0x0 */ s_AnalogController analogController;
    /* 0x8 */ s8                 expectedGameState; /** Expected value of `g_GameWork.gameState` before `analogController` is processed.
                                                     * If it doesn't match, `Demo_Update` displays `STEP ERROR` and stops reading the demo.
                                                     */
    /* 0x9 */ u8                 videoPresentInterval;
    /* 0xA */ s8                 field_A[2]; /** @unused? In DAT files, often contains pairs of the same value. */
    /* 0xC */ u32                randSeed;
} s_DemoPlaybackFrame;
STATIC_ASSERT_SIZEOF(s_DemoPlaybackFrame, 16);

/** @brief Associates a demo ID with `MISC/PLAY****.DAT` and `MISC/DEMO****.DAT` files. */
typedef struct _DemoFileInfo
{
    /* 0x0 */ s16  demoFileId;           /** `MISC/DEMO****.DAT`. See `s_DemoState`. */
    /* 0x2 */ s16  playFileId;           /** `MISC/PLAY****.DAT`. See `s_DemoPlaybackFrame`. */
    /* 0x4 */ bool (*canPlayDemo)(void); /** Optional funcptr. Returns `true` if this demo is eligible to be played (unused in retail demos). */
} s_DemoFileInfo;
STATIC_ASSERT_SIZEOF(s_DemoFileInfo, 8);

// ========
// GLOBALS
// ========

#define g_Demo_ActiveState ((s_DemoState*)0x800FDE00)

/** `Demo_FrameCount` */
extern s32 g_Demo_FrameCount;

// TODO: Make local. Used in `Demo_Update`.
extern char D_8002B2D8[]; // "STEP ERROR:[H:"
extern char D_8002B2E8[]; // "]/[M:"
extern char D_8002B2F0[]; // "]"

extern s_DemoPlaybackFrame* g_Demo_PlaybackFrames;
extern s32                  g_Demo_DemoId;
extern u16                  g_Demo_RandSeed;
extern s32                  g_Demo_DemoFileIdx;
extern s32                  g_Demo_PlayFileIdx;
extern s32                  __pad_bss_800C4848[2];
extern s_OptionsConfig      g_Demo_OptionsConfigBackup;
extern u32                  g_Demo_PrevRandSeed;
extern u32                  g_Demo_RandSeedBackup;
extern s_DemoPlaybackFrame* g_Demo_ActivePlaybackFrame;
extern s32                  g_Demo_DemoStep;
extern s32                  g_Demo_VideoPresentInterval;
extern bool                 g_Demo_IsLoadingChunks;

// ==========
// FUNCTIONS
// ==========

bool Demo_SequenceAdvance(s32 incAmount);

void Demo_DemoDataRead(void);

void Demo_PlayDataRead(void);

bool Demo_PlayFileBufferSetup(void);

void Demo_DemoFileSavegameUpdate(void);

void Demo_GameGlobalsUpdate(void);

void Demo_GameGlobalsRestore(void);

void Demo_GameRandSeedUpdate(void);

void Demo_GameRandSeedRestore(void);

void Demo_Start(void);

void Demo_Stop(void);

bool func_8008F434(s32 arg0);

/** @brief Gets the active demo state.
 *
 * @param gameState Active game state (`e_GameState`).
 * @return Active demo state `(e_DemoState`).
 */
s32 Demo_StateGet(s32 gameState);

void Demo_ExitDemo(void);

bool func_8008F520(void);

/** Generates the backup random demo seed and stores it in `Demo_RandSeedBackup`. */
void Demo_DemoRandSeedBackup(void);

void Demo_DemoRandSeedRestore(void);

void Demo_DemoRandSeedAdvance(void);

bool Demo_Update(void);

bool Demo_ControllerDataUpdate(void);

bool Demo_PresentIntervalUpdate(void);

bool Demo_GameRandSeedSet(void);

bool func_8008F914(s32 posX, s32 posZ);

#endif
