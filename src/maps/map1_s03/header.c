#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "maps/map1/map1_s03.h"
#include "maps/common/particle.h"
#include "maps/common/characters/player.h"
#include "maps/common/characters/creeper.h"
#include "maps/common/characters/locker_dead_body.h"
#include "maps/common/characters/stalker.h"

extern s_AnimInfo HARRY_M1S03_ANIM_INFOS[];

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
    /* 0  */ MapEvent_DoorJammed,
    /* 1  */ MapEvent_DoorLocked,
    /* 2  */ MapEvent_DoorUnlocked,
    /* 3  */ MapEvent_LibraryReserveKeyUse,
    /* 4  */ MapEvent_ClassroomKeyUse,
    /* 5  */ MapEvent_CommonItemTake,
    /* 6  */ MapEvent_RoofDrainPuzzleInteract0,
    /* 7  */ MapEvent_RoofDrainPuzzleInteract1,
    /* 8  */ MapEvent_DrainageValveInteract,
    /* 9  */ MapEvent_TheMonsterLurksBookRead,
    /* 10 */ MapEvent_SplitHeadFairyTaleBookRead,
    /* 11 */ MapEvent_OpenedCatLockerInspect,
    /* 12 */ MapEvent_UnopenedCatLockerInspect,
    /* 13 */ MapEvent_LockerDeadBodyCutscene,
    /* 14 */ MapEvent_LibraryReserveKeyItemTake,
    /* 15 */ MapEvent_RubberBallUse,
    /* 16 */ func_800DCDDC
};

const s_MapOverlayHdr g_MapOverlayHdr = {
    .mapInfo                        = &MAP_INFOS[MapType_SU],
    .mapRoomIdxGet              = Map_RoomIdxGet,
    .unused_8                          = 0,
    .func_C                           = sharedFunc_800D929C_0_s00,
    .bgmEvent                      = Map_RoomBgmInit,
    .bgmCmd                        = BgmCmd_Track11,
    .ambienceIdx               = 8,
    .field_16                         = 2,
    .field_17                         = 0,
    .loadingScreenFuncs            = g_LoadScreenFuncs,
    .mapPoints           = MAP_POINTS,
    .mapEventFuncs                 = g_MapEventFuncs,
    .mapEvents                     = MAP_EVENTS,
    .npcBoneCoordBuffer                         = &g_SysWork.npcBoneCoordBuffer[0],
    .loadableItems                 = LOADABLE_INVENTORY_ITEMS,
    .mapMessages                   = MAP_MESSAGES,
    .harryMapAnimInfos                     = HARRY_M1S03_ANIM_INFOS,
    .field_38                         = g_MapHeaderTable_38,
    .initWorldObjects              = Map_WorldObjectsInit,
    .updateWorldObjects            = Map_WorldObjectsUpdate,
    .func_44                          = NULL,
    .npcSpawnEvent                 = func_800DE624,
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
    .field_94                         = &sharedData_800E30C8_1_s02,
    .func_98                          = sharedFunc_800CBDA8_1_s02,
    .func_9C                          = sharedFunc_800CCE60_1_s02,
    .ptr_A0                           = D_800E3A40,
    .func_A4                          = func_800CE164,
    .func_A8                          = NULL,
    .func_AC                          = NULL,
    .func_B0                          = NULL,
    .func_B4                          = sharedFunc_800CE688_1_s03,
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
    .charaMovementReset                         = NULL,
    .charaAnimReset                         = NULL,
    .charaAnimStateSet                         = NULL,
    .charaControlStateReset                         = NULL,
    .charaAnimLock                         = NULL,
    .charaAnimIsLocked                         = NULL,
    .charaAnimUnlock                         = NULL,
    .charaAnimPlaybackStateGet                         = NULL,
    .charaPathWaypointExecute                         = NULL,
    .charaVisibleSet                         = NULL,
    .charaInvisibleSet                         = NULL,
    .npcFootstepSoundPlay                         = NULL,
    .npcSfxPlay                         = NULL,
    .charaAnimStartKeyframeIdxGet      = NULL,
    .charaCollisionReset                         = NULL,
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
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, Stalker_Update,
        Stalker_Update, Stalker_Update, NULL, Creeper_Update, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, LockerDeadBody_Update },
    .charaGroupIds = { Chara_GreyChildOrMumbler, Chara_Creeper, Chara_LockerDeadBody },
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
