#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "maps/map7/map7_s00.h"
#include "maps/common/particle.h"
#include "maps/common/characters/player.h"
#include "maps/common/characters/lisa.h"

extern s_AnimInfo HARRY_M7S00_ANIM_INFOS[];

void (*g_LoadScreenFuncs[])() = {
    NULL,
    GameBoot_LoadScreen_PlayerRun,
    GameBoot_LoadScreen_BackgroundImg,
    GameBoot_LoadScreen_StageString
};

s_MapPoint2d MAP_POINTS[] = {
#include "map_points.h"
};

void (*g_MapEventFuncs[])() = {
    MapEvent_DoorJammed,
    MapEvent_DoorLocked,
    func_800D0AC4,
    func_800D0B64,
    func_800D1604
};

const s_MapOverlayHdr g_MapOverlayHdr = {
    .mapInfo                        = &MAP_INFOS[MapType_ER2],
    .mapRoomIdxGet              = Map_RoomIdxGet,
    .unused_8                          = NO_VALUE,
    .func_C                           = sharedFunc_800D929C_0_s00,
    .bgmEvent                      = func_800CFFE8,
    .bgmCmd                        = BgmCmd_Track10,
    .ambienceIdx               = 36,
    .field_16                         = 2,
    .field_17                         = 6,
    .loadingScreenFuncs            = g_LoadScreenFuncs,
    .mapPoints           = MAP_POINTS,
    .mapEventFuncs                 = g_MapEventFuncs,
    .mapEvents                     = MAP_EVENTS,
    .npcBoneCoordBuffer                         = &g_SysWork.npcBoneCoordBuffer[0],
    .loadableItems                 = LOADABLE_INVENTORY_ITEMS,
    .mapMessages                   = MAP_MESSAGES,
    .harryMapAnimInfos                     = HARRY_M7S00_ANIM_INFOS,
    .field_38                         = g_MapHeaderTable_38,
    .initWorldObjects              = Map_WorldObjectsInit,
    .updateWorldObjects            = Map_WorldObjectsUpdate,
    .func_44                          = NULL,
    .npcSpawnEvent                 = NULL,
    .unkTable1_4C                     = sharedData_800DFB7C_0_s00,
    .unkTable1Count_50                = ARRAY_SIZE(sharedData_800DFB7C_0_s00),
    .bloodSplats                   = g_Effect_BloodSplats,
    .bloodSplatCount               = ARRAY_SIZE(g_Effect_BloodSplats),
    .field_5C                         = NULL,
    .func_60                          = NULL,
    .func_64                          = NULL,
    .func_68                          = NULL,
    .func_6C                          = NULL,
    .func_70                          = NULL,
    .func_74                          = NULL,
    .func_78                          = NULL,
    .field_7C                         = NULL,
    .func_80                          = NULL,
    .func_84                          = NULL,
    .func_88                          = NULL,
    .func_8C                          = NULL,
    .func_90                          = NULL,
    .field_94                         = NULL,
    .func_98                          = NULL,
    .func_9C                          = NULL,
    .ptr_A0                           = NULL,
    .func_A4                          = NULL,
    .func_A8                          = NULL,
    .func_AC                          = NULL,
    .func_B0                          = NULL,
    .func_B4                          = NULL,
    .func_B8                          = sharedFunc_800CDAA8_0_s02,
    .func_BC                          = sharedFunc_800D1C38_0_s00,
    .playerCutsceneWeaponUnequip                          = Player_CutsceneWeaponUnequip,
    .playerMatchArmAnimDisable     = Player_MatchArmAnimDisable,
    .playerControlFreeze           = Player_ControlFreeze,
    .playerControlUnfreeze         = Player_ControlUnfreeze,
    .playerPathWaypointExecute                          = Player_PathWaypointExecute,
    .playerAnimStateSet                          = Player_AnimStateSet,
    .playerAnimReset                          = Player_AnimReset,
    .playerAnimLock                = Player_AnimLock,
    .playerAnimIsLocked            = Player_AnimIsLocked,
    .playerAnimUnlock              = Player_AnimUnlock,
    .playerAnimPlaybackStateGet                          = Player_AnimPlaybackStateGet,
    .playerMoveSpeedIsZero          = Player_MoveSpeedIsZero,
    .playerMoveSpeedClear           = Player_MoveSpeedClear,
    .playerFallBackward            = Player_FallBackward,
    .playerStub_F8                          = Player_Stub_F8,
    .playerDamageFeetFront         = Player_DamageFeetFront,
    .func_100                         = sharedFunc_800D2E8C_0_s00,
    .func_104                         = sharedFunc_800D2E6C_0_s00,
    .func_108                         = sharedFunc_800D2E94_0_s00,
    .playerEmptyWeaponHandSet                         = Player_EmptyWeaponHandSet,
    .playerWeaponAttackRestore                         = Player_WeaponAttackRestore,
    .playerPropertyField10DGet                         = Player_PropertyField10DGet,
    .func_118                         = sharedFunc_800D2E9C_0_s00,
    .charaMovementReset                         = Chara_MovementReset,
    .charaAnimReset                         = Chara_AnimReset,
    .charaAnimStateSet                         = Chara_AnimStateSet,
    .charaControlStateReset                         = Chara_ControlStateReset,
    .charaAnimLock                         = Chara_AnimLock,
    .charaAnimIsLocked                         = Chara_AnimIsLocked,
    .charaAnimUnlock                         = Chara_AnimUnlock,
    .charaAnimPlaybackStateGet                         = Chara_AnimPlaybackStateGet,
    .charaPathWaypointExecute                         = Chara_PathWaypointExecute,
    .charaVisibleSet                         = Chara_VisibleSet,
    .charaInvisibleSet                         = Chara_InvisibleSet,
    .npcFootstepSoundPlay                         = Npc_FootstepSoundPlay,
    .npcSfxPlay                         = Npc_SfxPlay,
    .charaAnimStartKeyframeIdxGet      = Chara_AnimStartKeyframeIdxGet,
    .charaCollisionReset                         = Chara_CollisionReset,
    .splitHeadMovementOffsetSet                         = NULL,
    .func_15C                         = NULL,
    .func_160                         = NULL,
    .func_164                         = NULL,
    .particleSystemUpdate              = Particle_SystemUpdate,
    .particleEnviromentSet                = Particle_EnvironmentSet,
    .func_170                         = sharedFunc_800D0A60_0_s00,
    .func_174                         = sharedFunc_800D0B18_0_s00,
    .particleHyperBlasterBeamDraw = Particle_HyperBlasterBeamDraw,
    .particleBeamDraw             = Particle_BeamDraw,
    .particleSoundStop            = Particle_SoundStop,
    .particleWindSpeedX                   = &g_Particle_SpeedX,
    .particleWindSpeedZ                   = &g_Particle_SpeedZ,
    .data_18C                         = &sharedData_800DFB6C_0_s00,
    .data_190                         = &sharedData_800DFB70_0_s00,
    .charaUpdateFuncs             = {
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        Lisa_Update },
    .charaGroupIds = { Chara_Lisa },
    .charaSpawnInfos   = {
        #include "chara_spawns.h"
    },
    .cameraPaths = {
        #include "camera_paths.h"
    },
    .collisionTriggers = {
        #include "collision_triggers.h"
    },
};
