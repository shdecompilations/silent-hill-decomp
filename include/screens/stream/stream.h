#ifndef _SCREENS_STREAM_H
#define _SCREENS_STREAM_H

#include "common.h"

#include <psyq/libcd.h>
#include <psyq/libgpu.h>

#define RING_SIZE  23
#define MOVIE_WAIT 2000
#define PPW(x)     (((x) * 3) / 2)

extern int StCdIntrFlag; // Not included in SDK docs/headers, but movie player sample code (and moviesys) uses it?

extern s_FsImageDesc g_MemCardWarningImg;

typedef struct _DECENV
{
    /* 0x0  */ u_long* vlcbuf[2];
    /* 0x8  */ s32     vlcid;
    /* 0xC  */ u16*    imgbuf;
    /* 0x10 */ RECT    rect[2];
    /* 0x20 */ s32     rectid;
    /* 0x24 */ RECT    slice;
    /* 0x2C */ s32     isdone;
} DECENV;

typedef struct _MOVIE_STR
{
    /* 0x0     */ CdlLOC loc;
    /* 0x4     */ DECENV dec;
    /* 0x34    */ s32    rewindSwitch; // Or `Clear_Flag`.
    /* 0x38    */ s32    width;
    /* 0x3C    */ s32    height;
    /* 0x40    */ u16    imgbuf0[5760];
    /* 0x2D40  */ u16    imgbuf1[5760];
    /* 0x5A40  */ u_long sect_buff[11776];
    /* 0x11240 */ u_long vlcbuf0[14336];
    /* 0x1F240 */ u_long vlcbuf1[14336];
} MOVIE_STR;

// Customized `StHEADER`?
typedef struct _CDSECTOR
{
    u16    id;
    u16    type;
    u16    secCount;
    u16    nSectors;
    u_long frameCount;
    u_long frameSize;
    u16    width;
    u16    height;
    u_long headm;
    u_long headv;
    u_long user;
} CDSECTOR;

// =========================
// MOVIE GAME STATE HANDLERS
// =========================

/** @brief Updates intro movie fade-in game state. */
void GameState_MovieIntroFadeIn_Update(void);

/** @brief Updates intro movie game state. */
void GameState_MovieIntro_Update(void);

/** @brief Updates opening movie game state. */
void GameState_MovieOpening_Update(void);

/** @brief Updates movie exit game state. */
void GameState_ExitMovie_Update(void);

/** @brief @unused Updates debug movie player game state. The movie to play is decided by `LStickLeft`/`LStickRight`. */
void GameState_DebugMoviePlayer_Update(void);

/** @brief Updates alternative intro movie game state. */
void GameState_MovieIntroAlternate_Update(void);

// ==========
// `MOVIESYS`
// ==========

void    open_main(s32 file_idx, s16 num_frames);
void    movie_main(char* file_name, s32 f_size, s32 sector);
void    strSetDefDecEnv(DECENV* dec, s32 x0, s32 y0, s32 x1, s32 y1);
void    strInit(CdlLOC* loc, void (*callback)());
void    strCallback(void);
void    strKickCD(CdlLOC* loc);
s32     strNextVlc(DECENV* dec);
u_long* strNext(DECENV* dec);
void    strSync(DECENV* dec);

#endif
