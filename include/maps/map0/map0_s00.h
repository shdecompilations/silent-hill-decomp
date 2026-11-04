#ifndef _MAPS_MAP0_S00_H
#define _MAPS_MAP0_S00_H

#define HAS_PlayerState_Unk51
#define HAS_PlayerState_Unk131
#define HAS_PlayerState_Reset
#define HAS_PlayerState_WalkForward
#define HAS_PlayerState_RunForward
#define HAS_PlayerState_WalkBackward
#define HAS_PlayerState_TurnRight
#define HAS_PlayerState_TurnLeft
#define HAS_PlayerState_Unk74
#define HAS_PlayerState_Unk84

#define HAS_PARTICLE_CASE_1
#define PARTICLE_CASE_COUNT 1

#define HAS_Chara_Cheryl
#define MAP_CHARA_COUNT 1

#define MAP_ROOM_MIN_X Q12(-320.0f)
#define MAP_ROOM_MAX_X Q12(240.0f)
#define MAP_ROOM_MIN_Z Q12(-240.0f)
#define MAP_ROOM_MAX_Z Q12(400.0f)
#define MAP_HAS_SECONDARY_GRID

#define MAP_FIELD_4C_COUNT 200
#define MAP_BLOOD_SPLAT_COUNT_MAX 150

// PAL has an extra common message at index 3, so every message after it is one higher than in USA.
#if VERSION_REGION_IS(PAL)
    #define MAP_MSG(idx) ((idx) >= 3 ? (idx) + 1 : (idx))
#else
    #define MAP_MSG(idx) (idx)
#endif

#include "maps/common/shared.h"

typedef struct
{
    s16   field_0;
    q3_12 field_2; // Angle.
} s_800DFB44;

typedef struct
{
    u8 field_0;
    u8 field_1;
    u8 field_2;
} s_800DFB48;

typedef struct
{
    s32 field_0;
} s_800E0C74;

/** `bool` */
extern u8 D_800DD593;

/** `bool` */
extern u8 D_800DD594;

/** Time. */
extern q19_12 D_800DFAB8;

extern u16 g_Cutscene_MapMsgAudioCmds[];

extern u8 D_800DFAC2;

extern bool g_WarpCamera0;

extern u16 D_800DFAC8;

extern s32 D_800DFACC;

/** Distance. */
extern q19_12 D_800DFAD0;

extern s32 D_800DFAD4;

extern bool g_WarpCamera;

extern q19_12 D_800DFADC;

extern VECTOR3 D_800DFAE0[];  // }  TODO might be one big table.
extern VECTOR3 D_800DFB28[3]; // }

extern u8 g_Cutscene_MapMsgAudioIdx;

extern s32 D_800DFB40;

extern s_800DFB44 D_800DFB44;

extern s_800DFB48 D_800DFB48;

extern s32 D_800DFB54;

extern q19_12 D_800DFB5C;

extern u8 D_800DFB60;

extern u8 D_800DFB61;

extern u8 D_800DF2F8;

extern s16 D_800DF300[];

extern s16 D_800E0C64;

extern s_800E0C74 D_800E0C74;

extern VECTOR3 D_800E32DC[4];

extern u8 D_800E34EC;

extern u8 D_800E39AC;

// Wheelchair and its spinning wheel in the alley?
extern s_WorldObjectModel D_800E3A5C[2];
extern VECTOR3            D_800E3A9C;
extern SVECTOR3           D_800E3AAC;

/** Open alley gate event? */
void func_800D9610(void);

/** @brief Handles the cutscene event in which Harry wakes up from the car crash to find Cheryl missing. */
void MapEvent_CutsceneOpening(void);

/** @brief Handles the cutscene event in which Harry hears Cheryl's footsteps for the first time and moves toward their
 * source.
 */
void MapEvent_CutsceneCherylFootsteps0(void);

/** @brief Handles the cutscene event in which Harry hears Cheryl's footsteps for the first time and moves toward their
 * source.
 */
void MapEvent_CutsceneCherylFootsteps1(void);

/** @brief Handles the cutscene event in which Harry hears Cheryl's footsteps for the first time and moves toward their
 * source.
 */
void MapEvent_CutsceneCherylFootsteps2(void);

// Plays distant Cheryl footstep SFX?
void Cheryl_DistantFootstepSfxPlay(void);

/** @brief Handles the cutscene event in which Harry spots Cheryl in the fog for the first time and begins to follow
 * her.
 */
void MapEvent_CutsceneCherylSpotted(void);

/** @brief Handles the cutscene event in which Harry is redirected to searching for Cheryl after the player attempts to
 * run off in the wrong direction.
 */
void MapEvent_CutsceneCherylRedirect0(void);

/** @brief Handles the cutscene event in which Harry is redirected to searching for Cheryl after the player attempts to
 * run off in the wrong direction.
 */
void MapEvent_CutsceneCherylRedirect1(void);

/** @brief Handles the cutscene event in which Harry is redirected to searching for Cheryl after the player attempts to
 * run off in the wrong direction.
 */
void MapEvent_CutsceneCherylRedirect2(void);

/** @brief Handles the cutscene event in which Harry is redirected to searching for Cheryl after the player attempts to
 * run off in the wrong direction.
 */
void MapEvent_CutsceneCherylRedirect3(void);

/** @brief Handles the cutscene event in which Harry spots Cheryl in the fog for the second time and follows her into
 * the alley.
 */
void MapEvent_CutsceneCherylIntoTheAlley(void);

// TODO: Which cutscene?
void func_800DB26C(void);

/** @brief Handles the cutscene event in which the alley gets darker and Harry lights a match. */
void MapEven_CutsceneAlleyGetsDarker(void);

// Dead end cutscene
void func_800DB870(void);

/** @brief Handles the cutscene event in which Harry spots the dead body on a chain link fence and is promptly ambushed
 * by Grey Children.
 */
void MapEvent_CutsceneAlleyNightmare(void);

// Stops sound effects and warps the player to the floor.
void func_800DBE00(void);

void Map_WorldObjectsInit(void);

void Map_WorldObjectsUpdate(void);

/** @brief Handles the map event which spawns Grey Children to ambush Harry. */
void MapEvent_GreyChildrenSpawn(void);

// Alley rail camera setup events?

void func_800DC33C(void);

void func_800DC694(void);

void func_800DC8D8(void);

void func_800DCA30(void);

void func_800DCC54(void);

s32 func_800DCDA8(void);

s32 func_800DCF38(s32);

void func_800DD0CC(void);

#endif
