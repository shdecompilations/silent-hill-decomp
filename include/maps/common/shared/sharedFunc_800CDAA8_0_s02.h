#include "bodyprog/bodyprog.h"
#include "bodyprog/player/player.h"

// TODO:
// - Rename `sharedFunc_800CDAA8_0_s02` -> `sharedFunc_800D0E34_0_s00`, g_Player_MoveSpeed` -> `sharedData_800E39DC_0_s00`

// Keyframe indices used for each state
// Maps can override these by defining the keyframe beforehand, only indices that change in certain maps are included here.
// (most likely whatever tool they used to create the `g_MapOverlayHdr.harryMapAnimInfos` data also output a header defining these for each map)
#ifndef KEYFRAME_PlayerState_Unk59
    #define KEYFRAME_PlayerState_Unk59 927
#endif

#ifndef KEYFRAME_PlayerState_Unk60
    #define KEYFRAME_PlayerState_Unk60 942
#endif

#ifndef KEYFRAME_PlayerState_Unk69
    #define KEYFRAME_PlayerState_Unk69 984
#endif

#ifndef KEYFRAME_PlayerState_Unk87
    #define KEYFRAME_PlayerState_Unk87 873
#endif

#ifndef KEYFRAME_PlayerState_Unk88
    #define KEYFRAME_PlayerState_Unk88 910
#endif

#ifndef KEYFRAME_PlayerState_Unk111
    #define KEYFRAME_PlayerState_Unk111 678
#endif

#ifndef KEYFRAME_PlayerState_Unk113
    #define KEYFRAME_PlayerState_Unk113 1084
#endif

#ifndef KEYFRAME_PlayerState_Unk114
    #define KEYFRAME_PlayerState_Unk114 932
#endif

#ifndef KEYFRAME_PlayerState_Unk117
    #define KEYFRAME_PlayerState_Unk117 723
#endif

#ifndef KEYFRAME_PlayerState_Unk122
    #define KEYFRAME_PlayerState_Unk122 961
#endif

#ifndef KEYFRAME_PlayerState_Unk123
    #define KEYFRAME_PlayerState_Unk123 693
#endif

#ifndef KEYFRAME_PlayerState_Unk146
    #define KEYFRAME_PlayerState_Unk146 1074
#endif

#ifndef KEYFRAME_PlayerState_Unk147
    #define KEYFRAME_PlayerState_Unk147 1108
#endif

#ifndef KEYFRAME_PlayerState_Unk153
    #define KEYFRAME_PlayerState_Unk153 1036
#endif

#ifndef KEYFRAME_PlayerState_Unk162
    #define KEYFRAME_PlayerState_Unk162 1237
#endif

#ifndef KEYFRAME_PlayerState_Unk167
    #define KEYFRAME_PlayerState_Unk167 1070
#endif

// All maps have 52
#ifndef HAS_PlayerState_Reset
    #define HAS_PlayerState_Reset
#endif

// Very similar to `func_80071968_Switch1`
static inline void sharedFunc_800CDAA8_0_s02_Switch(void)
{
    if (g_SysWork.playerWork.extra.state == PlayerState_Reset)
    {
        func_8003D01C();

        switch (g_SysWork.playerCombat.weaponAttack)
        {
            case WEAPON_ATTACK(EquippedWeaponId_KitchenKnife, AttackInputType_Tap):
            case WEAPON_ATTACK(EquippedWeaponId_SteelPipe,    AttackInputType_Tap):
            case WEAPON_ATTACK(EquippedWeaponId_RockDrill,    AttackInputType_Tap):
            case WEAPON_ATTACK(EquippedWeaponId_Hammer,       AttackInputType_Tap):
            case WEAPON_ATTACK(EquippedWeaponId_Chainsaw,     AttackInputType_Tap):
            case WEAPON_ATTACK(EquippedWeaponId_Katana,       AttackInputType_Tap):
            case WEAPON_ATTACK(EquippedWeaponId_Axe,          AttackInputType_Tap):
            case WEAPON_ATTACK(EquippedWeaponId_KitchenKnife, AttackInputType_Hold):
            case WEAPON_ATTACK(EquippedWeaponId_SteelPipe,    AttackInputType_Hold):
            case WEAPON_ATTACK(EquippedWeaponId_RockDrill,    AttackInputType_Hold):
            case WEAPON_ATTACK(EquippedWeaponId_Hammer,       AttackInputType_Hold):
            case WEAPON_ATTACK(EquippedWeaponId_Chainsaw,     AttackInputType_Hold):
            case WEAPON_ATTACK(EquippedWeaponId_Katana,       AttackInputType_Hold):
            case WEAPON_ATTACK(EquippedWeaponId_Axe,          AttackInputType_Hold):
            case WEAPON_ATTACK(EquippedWeaponId_KitchenKnife, AttackInputType_Multitap):
            case WEAPON_ATTACK(EquippedWeaponId_SteelPipe,    AttackInputType_Multitap):
            case WEAPON_ATTACK(EquippedWeaponId_RockDrill,    AttackInputType_Multitap):
            case WEAPON_ATTACK(EquippedWeaponId_Hammer,       AttackInputType_Multitap):
            case WEAPON_ATTACK(EquippedWeaponId_Chainsaw,     AttackInputType_Multitap):
            case WEAPON_ATTACK(EquippedWeaponId_Katana,       AttackInputType_Multitap):
            case WEAPON_ATTACK(EquippedWeaponId_Axe,          AttackInputType_Multitap):
                WorldGfx_CharaMeshSwap(Chara_Harry, MESH_SWAP_STATUS(HarrySwappableMesh_RightHand, HarryVariantMesh_HandMelee));
                break;

            case WEAPON_ATTACK(EquippedWeaponId_Handgun,      AttackInputType_Tap):
            case WEAPON_ATTACK(EquippedWeaponId_Shotgun,      AttackInputType_Tap):
            case WEAPON_ATTACK(EquippedWeaponId_HyperBlaster, AttackInputType_Tap):
                WorldGfx_CharaMeshSwap(Chara_Harry, MESH_SWAP_STATUS(HarrySwappableMesh_RightHand, HarryVariantMesh_RightHandGun));
                break;

            case WEAPON_ATTACK(EquippedWeaponId_HuntingRifle, AttackInputType_Tap):
                WorldGfx_CharaMeshSwap(Chara_Harry, MESH_SWAP_STATUS(HarrySwappableMesh_RightHand, HarryVariantMesh_RightHandRifle));
                break;
        }
    }
    else
    {
        func_8003D03C();
        WorldGfx_CharaMeshSwap(Chara_Harry, MESH_SWAP_STATUS(HarrySwappableMesh_RightHand, HarryVariantMesh_RightHandEmpty));
    }
}

// Slight change in `PlayerState_WalkBackward` version
static inline void sharedFunc_800CDAA8_0_s02_Switch_Unk85(s_PlayerExtra* extra)
{
    if (g_SysWork.playerWork.extra.state == PlayerState_Reset)
    {
        func_8003D01C();

        switch (g_SysWork.playerCombat.weaponAttack)
        {
            case WEAPON_ATTACK(EquippedWeaponId_KitchenKnife, AttackInputType_Tap):
            case WEAPON_ATTACK(EquippedWeaponId_SteelPipe,    AttackInputType_Tap):
            case WEAPON_ATTACK(EquippedWeaponId_RockDrill,    AttackInputType_Tap):
            case WEAPON_ATTACK(EquippedWeaponId_Hammer,       AttackInputType_Tap):
            case WEAPON_ATTACK(EquippedWeaponId_Chainsaw,     AttackInputType_Tap):
            case WEAPON_ATTACK(EquippedWeaponId_Katana,       AttackInputType_Tap):
            case WEAPON_ATTACK(EquippedWeaponId_Axe,          AttackInputType_Tap):
            case WEAPON_ATTACK(EquippedWeaponId_KitchenKnife, AttackInputType_Hold):
            case WEAPON_ATTACK(EquippedWeaponId_SteelPipe,    AttackInputType_Hold):
            case WEAPON_ATTACK(EquippedWeaponId_RockDrill,    AttackInputType_Hold):
            case WEAPON_ATTACK(EquippedWeaponId_Hammer,       AttackInputType_Hold):
            case WEAPON_ATTACK(EquippedWeaponId_Chainsaw,     AttackInputType_Hold):
            case WEAPON_ATTACK(EquippedWeaponId_Katana,       AttackInputType_Hold):
            case WEAPON_ATTACK(EquippedWeaponId_Axe,          AttackInputType_Hold):
            case WEAPON_ATTACK(EquippedWeaponId_KitchenKnife, AttackInputType_Multitap):
            case WEAPON_ATTACK(EquippedWeaponId_SteelPipe,    AttackInputType_Multitap):
            case WEAPON_ATTACK(EquippedWeaponId_RockDrill,    AttackInputType_Multitap):
            case WEAPON_ATTACK(EquippedWeaponId_Hammer,       AttackInputType_Multitap):
            case WEAPON_ATTACK(EquippedWeaponId_Chainsaw,     AttackInputType_Multitap):
            case WEAPON_ATTACK(EquippedWeaponId_Katana,       AttackInputType_Multitap):
            case WEAPON_ATTACK(EquippedWeaponId_Axe,          AttackInputType_Multitap):
                WorldGfx_CharaMeshSwap(Chara_Harry, MESH_SWAP_STATUS(HarrySwappableMesh_RightHand, HarryVariantMesh_HandMelee));
                break;

            case WEAPON_ATTACK(EquippedWeaponId_Handgun,      AttackInputType_Tap):
            case WEAPON_ATTACK(EquippedWeaponId_Shotgun,      AttackInputType_Tap):
            case WEAPON_ATTACK(EquippedWeaponId_HyperBlaster, AttackInputType_Tap):
                WorldGfx_CharaMeshSwap(Chara_Harry, MESH_SWAP_STATUS(HarrySwappableMesh_RightHand, HarryVariantMesh_RightHandGun));
                break;

            case WEAPON_ATTACK(EquippedWeaponId_HuntingRifle, AttackInputType_Tap):
                WorldGfx_CharaMeshSwap(Chara_Harry, MESH_SWAP_STATUS(HarrySwappableMesh_RightHand, HarryVariantMesh_RightHandRifle));
                break;
        }
    }
    else
    {
        func_8003D03C();

        if (extra->model.anim.keyframeIdx >= 958)
        {
            WorldGfx_CharaMeshSwap(Chara_Harry, MESH_SWAP_STATUS(HarrySwappableMesh_RightHand, HarryVariantMesh_5));
        }
        else
        {
            WorldGfx_CharaMeshSwap(Chara_Harry, MESH_SWAP_STATUS(HarrySwappableMesh_RightHand, HarryVariantMesh_RightHandEmpty));
        }
    }
}

void sharedFunc_800CDAA8_0_s02(s_SubCharacter* player, s_PlayerExtra* extra, GsCOORDINATE2* boneCoords)
{
    s_CollisionSurface surface;
    e_SfxId            footstepSfxId;
    s8                 pitch0;
    s8                 pitch1;
    s32                newMoveSpeed;
    q19_12             headingAngle;
    u16                keyframeIdxMax;
    s32                keyframeIdx;

    #define playerChara g_SysWork.playerWork.player
    #define playerProps playerChara.properties.player

    headingAngle = Q12_ANGLE(0.0f);
    keyframeIdx  = 0;

    g_Player_MoveSpeed = Q12(0.0f);
    sharedData_800E39D8_0_s00 = 0;

    player->properties.player.field_10C                            >>= 1;
    playerProps.gasWeaponPowerTimer = Q12(0.0f);

    func_8004C564(0, -1);

    player->properties.player.afkTimer        = Q12(0.0f);
    player->properties.player.exhaustionTimer = Q12(0.0f);

    switch (g_SysWork.playerWork.extra.state)
    {
#ifdef HAS_PlayerState_Unk51
        case PlayerState_Unk51:
            if (playerProps.moveSpeed != Q12(0.0f))
            {
                playerProps.moveSpeed -= TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f)) * 2;
                if (playerProps.moveSpeed < Q12(0.0f))
                {
                    playerProps.moveSpeed = Q12(0.0f);
                }
            }

            sharedData_800E39D8_0_s00 = 0;
            g_Player_MoveSpeed = playerProps.moveSpeed;

            func_8007FC48(player, extra, ANIM_STATUS(200, false));

            if (extra->model.controlState == 0)
            {
                extra->model.controlState++;
            }

            switch (g_Player_CutsceneState)
            {
                case PlayerCutsceneState_TurnRight:
                    Player_ExtraStateSet(player, extra, PlayerState_TurnRight);
                    break;

                case PlayerCutsceneState_TurnLeft:
                    Player_ExtraStateSet(player, extra, PlayerState_TurnLeft);
                    break;
            }

            Player_AnimStateReset(player, extra);

            headingAngle = playerProps.headingAngle = Q12_ANGLE(0.0f);
            break;
#endif

#ifdef HAS_PlayerState_Unk131
        case PlayerState_Unk131:
            if (playerProps.moveSpeed != Q12(0.0f))
            {
                playerProps.moveSpeed -= TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f)) * 2;
                if (playerProps.moveSpeed < Q12(0.0f))
                {
                    playerProps.moveSpeed = Q12(0.0f);
                }
            }

            sharedData_800E39D8_0_s00 = 0;
            g_Player_MoveSpeed = playerProps.moveSpeed;
            func_8007FB94(player, extra, ANIM_STATUS(200, false));

            if (extra->model.controlState == 0)
            {
                extra->model.controlState++;
            }

            switch (g_Player_CutsceneState)
            {
                case PlayerCutsceneState_TurnRight:
                    Player_ExtraStateSet(player, extra, PlayerState_TurnRight);
                    break;

                case PlayerCutsceneState_TurnLeft:
                    Player_ExtraStateSet(player, extra, PlayerState_TurnLeft);
                    break;
            }

            Player_AnimStateReset(player, extra);

            headingAngle = playerProps.headingAngle = Q12_ANGLE(0.0f);
            break;
#endif

#ifdef HAS_PlayerState_Reset
        case PlayerState_Reset:
            if (playerProps.moveSpeed != Q12(0.0f))
            {
                playerProps.moveSpeed -= TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f)) * 2;
                if (playerProps.moveSpeed < Q12(0.0f))
                {
                    playerProps.moveSpeed = Q12(0.0f);
                }
            }

            sharedData_800E39D8_0_s00 = 0;
            g_Player_MoveSpeed = playerProps.moveSpeed;

            Model_AnimStatusSet(&extra->model, HarryAnim_Idle, false);
            Model_AnimStatusSet(&player->model, HarryAnim_Idle, false);

            if (extra->model.controlState == 0)
            {
                extra->model.controlState++;
            }

            switch (g_Player_CutsceneState)
            {
                case PlayerCutsceneState_TurnRight:
                    Player_ExtraStateSet(player, extra, PlayerState_TurnRight);
                    break;

                case PlayerCutsceneState_TurnLeft:
                    Player_ExtraStateSet(player, extra, PlayerState_TurnLeft);
                    break;
            }

            Player_AnimStateReset(player, extra);

            headingAngle = playerProps.headingAngle = Q12_ANGLE(0.0f);
            break;
#endif

#ifdef HAS_PlayerState_WalkForward
        case PlayerState_WalkForward:
            if (playerProps.moveSpeed > Q12(1.4f))
            {
                playerProps.moveSpeed -= TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f)) * 2;
                if (playerProps.moveSpeed < Q12(1.4f))
                {
                    playerProps.moveSpeed = Q12(1.4f);
                }
            }
            else if (playerProps.moveSpeed < Q12(1.4f))
            {
                if (player->model.anim.keyframeIdx >= 2)
                {
                    playerProps.moveSpeed += TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f));
                }

                playerProps.moveSpeed = CLAMP(playerProps.moveSpeed,
                                              Q12(0.0f),
                                              Q12(1.4f));
            }

            g_Player_MoveSpeed = playerProps.moveSpeed;

            Model_AnimStatusSet(&extra->model, HarryAnim_WalkForward, false);
            Model_AnimStatusSet(&player->model, HarryAnim_WalkForward, false);

            if (extra->model.controlState == 0)
            {
                extra->model.controlState++;
            }

            switch (g_Player_CutsceneState)
            {
                case PlayerCutsceneState_RunForward:
                    Player_ExtraStateSet(player, extra, PlayerState_RunForward);
                    break;

                case PlayerCutsceneState_TurnRight:
                    Player_ExtraStateSet(player, extra, PlayerState_TurnRight);
                    break;

                case PlayerCutsceneState_TurnLeft:
                    Player_ExtraStateSet(player, extra, PlayerState_TurnLeft);
                    break;
            }
            
            Player_AnimStateReset(player, extra);

            headingAngle = playerProps.headingAngle = Q12_ANGLE(0.0f);
            break;
#endif

#ifdef HAS_PlayerState_RunForward
        case PlayerState_RunForward:
            if (playerProps.moveSpeed > Q12(4.0f))
            {
                playerProps.moveSpeed -= TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f));
                if (playerProps.moveSpeed < Q12(4.0f))
                {
                    playerProps.moveSpeed = Q12(4.0f);
                }
            }
            else if (playerProps.moveSpeed < Q12(4.0f))
            {
                playerProps.moveSpeed += TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.75f));
                playerProps.moveSpeed =
                    CLAMP(playerProps.moveSpeed, Q12(0.0f), Q12(4.0f));
            }

            g_Player_MoveSpeed = playerProps.moveSpeed;

            Model_AnimStatusSet(&extra->model, HarryAnim_RunForward, false);
            Model_AnimStatusSet(&player->model, HarryAnim_RunForward, false);

            if (extra->model.controlState == 0)
            {
                extra->model.controlState++;
            }

            switch (g_Player_CutsceneState)
            {
                case PlayerCutsceneState_WalkForward:
                    Player_ExtraStateSet(player, extra, PlayerState_WalkForward);
                    break;

                case PlayerCutsceneState_TurnRight:
                    Player_ExtraStateSet(player, extra, PlayerState_TurnRight);
                    break;

                case PlayerCutsceneState_TurnLeft:
                    Player_ExtraStateSet(player, extra, PlayerState_TurnLeft);
                    break;
            }
            
            Player_AnimStateReset(player, extra);

            headingAngle =
            playerProps.headingAngle = Q12_ANGLE(0.0f);
            break;
#endif

#ifdef HAS_PlayerState_WalkBackward
        case PlayerState_WalkBackward:
            g_Player_MoveSpeed = playerProps.moveSpeed;

            Model_AnimStatusSet(&extra->model, HarryAnim_WalkBackward, false);
            Model_AnimStatusSet(&player->model, HarryAnim_WalkBackward, false);

            if (extra->model.controlState == 0)
            {
                extra->model.controlState++;
            }
            
            Player_AnimStateReset(player, extra);

            headingAngle = playerProps.headingAngle = Q12_ANGLE(180.0f);
            break;
#endif

#ifdef HAS_PlayerState_TurnRight
        case PlayerState_TurnRight:
            if (playerProps.moveSpeed != Q12(0.0f))
            {
                playerProps.moveSpeed -= TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f)) * 2;
                if (playerProps.moveSpeed < Q12(0.0f))
                {
                    playerProps.moveSpeed = Q12(0.0f);
                }
            }

            g_Player_MoveSpeed = playerProps.moveSpeed;
            sharedData_800E39D8_0_s00 = g_DeltaTime * 7;

            Model_AnimStatusSet(&extra->model, HarryAnim_TurnRight, false);
            Model_AnimStatusSet(&player->model, HarryAnim_TurnRight, false);

            if (extra->model.controlState == 0)
            {
                extra->model.controlState++;
            }

            switch (g_Player_CutsceneState)
            {
                case PlayerCutsceneState_RunForward:
                    Player_ExtraStateSet(player, extra, PlayerState_RunForward);
                    break;

                case PlayerCutsceneState_WalkForward:
                    Player_ExtraStateSet(player, extra, PlayerState_WalkForward);
                    break;

                case PlayerCutsceneState_TurnLeft:
                    Player_ExtraStateSet(player, extra, PlayerState_TurnLeft);
                    break;
            }
            
            Player_AnimStateReset(player, extra);

            headingAngle             = 
            playerProps.headingAngle = Q12_ANGLE(0.0f);
            g_Player_FlexRotationY   = Q12_ANGLE(-28.2f);
            break;
#endif

#ifdef HAS_PlayerState_TurnLeft
        case PlayerState_TurnLeft:
            if (playerProps.moveSpeed != Q12(0.0f))
            {
                playerProps.moveSpeed -= TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f)) * 2;
                if (playerProps.moveSpeed < Q12(0.0f))
                {
                    playerProps.moveSpeed = Q12(0.0f);
                }
            }

            g_Player_MoveSpeed = playerProps.moveSpeed;
            sharedData_800E39D8_0_s00 = g_DeltaTime * -7;

            Model_AnimStatusSet(&extra->model, HarryAnim_TurnLeft, false);
            Model_AnimStatusSet(&player->model, HarryAnim_TurnLeft, false);

            if (extra->model.controlState == 0)
            {
                extra->model.controlState++;
            }

            switch (g_Player_CutsceneState)
            {
                case PlayerCutsceneState_RunForward:
                    Player_ExtraStateSet(player, extra, PlayerState_RunForward);
                    break;

                case PlayerCutsceneState_WalkForward:
                    Player_ExtraStateSet(player, extra, PlayerState_WalkForward);
                    break;

                case PlayerCutsceneState_TurnRight:
                    Player_ExtraStateSet(player, extra, PlayerState_TurnRight);
                    break;
            }
            
            Player_AnimStateReset(player, extra);

            headingAngle             = 
            playerProps.headingAngle = Q12_ANGLE(0.0f);
            g_Player_FlexRotationY   = Q12_ANGLE(28.2f);
            break;
#endif

#ifdef HAS_PlayerState_Unk59
        case PlayerState_Unk59:
            func_8007FB94(player, extra, ANIM_STATUS(150, false));
            keyframeIdx = KEYFRAME_PlayerState_Unk59;
            
            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk60
        case PlayerState_Unk60:
            func_8007FB94(player, extra, ANIM_STATUS(150, true));
            keyframeIdx = KEYFRAME_PlayerState_Unk60;
            
            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk66
        case PlayerState_Unk66:
            func_8007FB94(player, extra, ANIM_STATUS(156, false));
            
            Player_AnimStateReset(player, extra);

            sharedFunc_800CDAA8_0_s02_Switch();
            break;
#endif

#ifdef HAS_PlayerState_Unk70
        case PlayerState_Unk70:
            func_8007FB94(player, extra, ANIM_STATUS(171, false));
            
            Player_AnimStateReset(player, extra);
#ifdef MAP0_S01
            func_8003D03C();
#endif
            break;
#endif

#ifdef HAS_PlayerState_Unk71
        case PlayerState_Unk71:
            func_8007FC48(player, extra, ANIM_STATUS(171, true));
            
            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk73
        case PlayerState_Unk73:
            func_8007FB94(player, extra, ANIM_STATUS(170, true));
            
            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk74
        case PlayerState_Unk74:
            func_8007FB94(player, extra, ANIM_STATUS(175, false));
            
            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk83
        case PlayerState_Unk83:
            func_8007FB94(player, extra, ANIM_STATUS(177, true));
            
            Player_AnimStateReset(player, extra);

            sharedFunc_800CDAA8_0_s02_Switch();
            break;
#endif

#ifdef HAS_PlayerState_Unk84
        case PlayerState_Unk84:
            keyframeIdxMax = 983; // TODO: Should probably be `keyframeIdx` instead like other cases, but doesn't match with that?
            func_8007FB94(player, extra, ANIM_STATUS(178, false));
            playerProps.moveSpeed = Q12(0.0f);

            if (Player_AnimPlaybackStateGet() == 1 && g_SysWork.enablePlayerMatchAnim == false)
            {
                g_SysWork.enablePlayerMatchAnim = true;
                Sfx_WithFlagsPlay(Sfx_Unk1356, &player->position, Q8(0.5f), SfxFlag_None);
            }

            if (player->model.anim.keyframeIdx >= keyframeIdxMax)
            {
                func_8003D03C();
            }
            break;
#endif

#ifdef HAS_PlayerState_Unk85
        case PlayerState_Unk85:
            func_8007FB94(player, extra, ANIM_STATUS(180, true));
#ifdef MAP1_S03
            func_8003D03C();
            WorldGfx_CharaMeshSwap(Chara_Harry, MESH_SWAP_STATUS(HarrySwappableMesh_RightHand, HarryVariantMesh_RightHandEmpty));
#else
            keyframeIdx = 949;
            sharedFunc_800CDAA8_0_s02_Switch_Unk85(extra);
#endif
            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk86
        case PlayerState_Unk86:
            func_8007FB94(player, extra, ANIM_STATUS(181, false));
            
            Player_AnimStateReset(player, extra);

            sharedFunc_800CDAA8_0_s02_Switch();
            break;
#endif

#ifdef HAS_PlayerState_Unk87
        case PlayerState_Unk87:
            func_8007FC48(player, extra, ANIM_STATUS(179, false));
            keyframeIdx = KEYFRAME_PlayerState_Unk87;
            
            Player_AnimStateReset(player, extra);

            sharedFunc_800CDAA8_0_s02_Switch();
            break;
#endif

#ifdef HAS_PlayerState_Unk88
        case PlayerState_Unk88:
            func_8007FC48(player, extra, ANIM_STATUS(179, true));
            keyframeIdx = KEYFRAME_PlayerState_Unk88;
            
            Player_AnimStateReset(player, extra);

            sharedFunc_800CDAA8_0_s02_Switch();
            break;
#endif

#ifdef HAS_PlayerState_Unk94
        case PlayerState_Unk94:
            func_8007FB94(player, extra, ANIM_STATUS(178, false));
            
            Player_AnimStateReset(player, extra);

            sharedFunc_800CDAA8_0_s02_Switch();
            break;
#endif

#ifdef HAS_PlayerState_Unk95
        case PlayerState_Unk95:
            func_8007FB94(player, extra, ANIM_STATUS(184, false));
            
            Player_AnimStateReset(player, extra);

            sharedFunc_800CDAA8_0_s02_Switch();
            break;
#endif

#ifdef HAS_PlayerState_Unk97
        case PlayerState_Unk97:
            if (player->model.anim.keyframeIdx >= 1065)
            {
                Savegame_EventFlagSet(EventFlag_99);
            }

            if (player->model.anim.keyframeIdx < 1090)
            {
                func_8003D03C();
            }
            else
            {
                func_8003D01C();
            }

            func_8007FB94(player, extra, ANIM_STATUS(185, false));
            keyframeIdx = 1062;
            
            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk98
        case PlayerState_Unk98:
            func_8007FB94(player, extra, ANIM_STATUS(185, true));
            keyframeIdx = 1062;
            
            Player_AnimStateReset(player, extra);

            if (player->model.anim.keyframeIdx >= 1147)
            {
                Savegame_EventFlagClear(EventFlag_99);
            }
            break;
#endif

#ifdef HAS_PlayerState_Unk99
        case PlayerState_Unk99:
            func_8007FB94(player, extra, ANIM_STATUS(186, true));
            
            Player_AnimStateReset(player, extra);

            sharedFunc_800CDAA8_0_s02_Switch();
            break;
#endif

#ifdef HAS_PlayerState_Unk100
        case PlayerState_Unk100:
            func_8007FB94(player, extra, ANIM_STATUS(186, false));
            
            Player_AnimStateReset(player, extra);

            sharedFunc_800CDAA8_0_s02_Switch();
            break;
#endif
#if defined(HAS_PlayerState_Unk69) || defined(HAS_PlayerState_Unk105)
        case PlayerState_Unk69:
        case PlayerState_Unk105:
            func_8007FC48(player, extra, 0x14B);

            keyframeIdx = KEYFRAME_PlayerState_Unk69;
            
            Player_AnimStateReset(player, extra);

            sharedFunc_800CDAA8_0_s02_Switch();
            break;
#endif

#ifdef HAS_PlayerState_Unk72
        case PlayerState_Unk72:
            func_8007FB94(player, extra, ANIM_STATUS(170, false));
            
            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk81
        case PlayerState_Unk81:
            keyframeIdx = 998;
            if (player->model.anim.keyframeIdx >= (keyframeIdx + 3))
            {
                if (player->model.anim.keyframeIdx <= (keyframeIdx + 24))
                {
                    if (playerProps.moveSpeed > Q12(0.2875f))
                    {
                        playerProps.moveSpeed -= TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f)) * 2;
                        if (playerProps.moveSpeed < Q12(0.2875f))
                        {
                            playerProps.moveSpeed = Q12(0.2875f);
                        }
                    }
                    else if (playerProps.moveSpeed < Q12(0.2875f))
                    {
                        if (player->model.anim.keyframeIdx > 998)
                        {
                            playerProps.moveSpeed += TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f));
                        }
                        playerProps.moveSpeed = CLAMP(playerProps.moveSpeed,
                                                      Q12(0.0f),
                                                      Q12(0.2875f));
                    }
                }
                else if (playerProps.moveSpeed != Q12(0.0f))
                {
                    playerProps.moveSpeed -= TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f)) * 2;
                    if (playerProps.moveSpeed < Q12(0.0f))
                    {
                        playerProps.moveSpeed = Q12(0.0f);
                    }
                }
            }

            g_Player_MoveSpeed = (q19_12)playerProps.moveSpeed;
            func_8007FC48(player, extra, ANIM_STATUS(180, false));
            headingAngle = Q12_ANGLE(180.0f);
            
            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk106
        case PlayerState_Unk106:
            func_8007FB94(player, extra, ANIM_STATUS(191, false));
            
            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk107
        case PlayerState_Unk107:
            func_8007FB94(player, extra, ANIM_STATUS(192, false));
            
            Player_AnimStateReset(player, extra);

            sharedFunc_800CDAA8_0_s02_Switch();
            break;
#endif

#ifdef HAS_PlayerState_Unk108
        case PlayerState_Unk108:
            func_8007FB94(player, extra, ANIM_STATUS(191, true));
            
            Player_AnimStateReset(player, extra);

            sharedFunc_800CDAA8_0_s02_Switch();
            break;
#endif

#ifdef HAS_PlayerState_Unk109
        case PlayerState_Unk109:
            func_8007FB94(player, extra, ANIM_STATUS(192, true));
            
            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk114
        case PlayerState_Unk114:
            func_8007FB94(player, extra, ANIM_STATUS(195, false));
            keyframeIdx = KEYFRAME_PlayerState_Unk114;
            
            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk115
        case PlayerState_Unk115:
            func_8007FB94(player, extra, ANIM_STATUS(195, true));
            keyframeIdx = 699;

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk116
        case PlayerState_Unk116:
            func_8007FC48(player, extra, ANIM_STATUS(196, false));

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk118
        case PlayerState_Unk118:
            func_8007FB94(player, extra, ANIM_STATUS(197, false));
            keyframeIdx = 750;

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk119
        case PlayerState_Unk119:
            func_8007FB94(player, extra, ANIM_STATUS(197, true));

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk120
        case PlayerState_Unk120:
            func_8007FB94(player, extra, ANIM_STATUS(198, false));
            keyframeIdx = 779;

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk121
        case PlayerState_Unk121:
            func_8007FB94(player, extra, ANIM_STATUS(198, true));
            keyframeIdx = 807;

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk122
        case PlayerState_Unk122:
#ifdef MAP1_S03
            func_8007FB94(player, extra, ANIM_STATUS(199, false));
#else
            func_8007FC48(player, extra, ANIM_STATUS(199, false));
#endif
            keyframeIdx = KEYFRAME_PlayerState_Unk122;

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk124
        case PlayerState_Unk124:
            func_8007FC48(player, extra, ANIM_STATUS(200, true));
            keyframeIdx = 846;

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk125
        case PlayerState_Unk125:
            func_8007FC48(player, extra, ANIM_STATUS(201, false));

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk126
        case PlayerState_Unk126:
            func_8007FC48(player, extra, ANIM_STATUS(201, true));
            keyframeIdx = 980;

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk127
        case PlayerState_Unk127:
            func_8007FC48(player, extra, ANIM_STATUS(202, false));
            keyframeIdx = 1002;

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk130
        case PlayerState_Unk130:
            func_8007FC48(player, extra, ANIM_STATUS(203, true));
            keyframeIdx = 967;

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk132
        case PlayerState_Unk132:
            func_8007FB94(player, extra, 343);

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk76
        case PlayerState_Unk76:
            func_8007FC48(player, extra, 351);
            keyframeIdx = 827;

            Player_AnimStateReset(player, extra);

            if (player->model.anim.keyframeIdx > (keyframeIdx + 33) &&
                player->model.anim.keyframeIdx < (keyframeIdx + 46))
            {
                func_8003D03C();
            }
            else
            {
                func_8003D01C();
            }
            break;
#endif

#ifdef HAS_PlayerState_Unk77
        case PlayerState_Unk77:
            func_8007FC48(player, extra, 352);

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk78
        case PlayerState_Unk78:
            func_8007FC48(player, extra, 353);

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk89
        case PlayerState_Unk89:
            func_8007FC48(player, extra, 363);
            keyframeIdx = 948;

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk90
        case PlayerState_Unk90:
            func_8007FC48(player, extra, 344);

            Player_AnimStateReset(player, extra);

            func_8003D03C();
            break;
#endif

#ifdef HAS_PlayerState_Unk91
        case PlayerState_Unk91:
            func_8007FB94(player, extra, 365);
            keyframeIdx = 1027;

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk92
        case PlayerState_Unk92:
            func_8007FB94(player, extra, 366);

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk93
        case PlayerState_Unk93:
            func_8007FB94(player, extra, 367);
            keyframeIdx = 1055;

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk110
        case PlayerState_Unk110:
            func_8007FB94(player, extra, 386);

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk111
        case PlayerState_Unk111:
            func_8007FB94(player, extra, ANIM_STATUS(193, true));
            keyframeIdx = KEYFRAME_PlayerState_Unk111;

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk113
        case PlayerState_Unk113:
            func_8007FB94(player, extra, ANIM_STATUS(194, true));
            keyframeIdx = KEYFRAME_PlayerState_Unk113;

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk117
        case PlayerState_Unk117:
            func_8007FC48(player, extra, ANIM_STATUS(196, true));
            keyframeIdx = KEYFRAME_PlayerState_Unk117;

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk123
        case PlayerState_Unk123:
            func_8007FB94(player, extra, ANIM_STATUS(199, true));
            keyframeIdx = KEYFRAME_PlayerState_Unk123;

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk128
        case PlayerState_Unk128:
            func_8007FC48(player, extra, ANIM_STATUS(202, true));

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk129
        case PlayerState_Unk129:
            func_8007FC48(player, extra, ANIM_STATUS(203, false));

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk133
        case PlayerState_Unk133:
            func_8007FB94(player, extra, ANIM_STATUS(204, false));
            keyframeIdx = 1039;

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk134
        case PlayerState_Unk134:
            func_8007FB94(player, extra, ANIM_STATUS(204, true));

            Player_AnimStateReset(player, extra);

            sharedFunc_800CDAA8_0_s02_Switch();

            if (player->model.anim.status & 1 << 0)
            {
                s_SubCharacter* new_var; // @hack Permuter find, maybe an inline here?
                extra->model.anim.time       = Q12(g_MapOverlayHdr.harryMapAnimInfos[player->model.anim.status - 76].startKeyframeIdx) + (new_var = g_SysWork.npcs)[0].model.anim.time - Q12(261.0f);
                player->model.anim.time = Q12(g_MapOverlayHdr.harryMapAnimInfos[player->model.anim.status - 76].startKeyframeIdx) + new_var[0].model.anim.time - Q12(261.0f);

                Player_AnimStateReset(player, extra);

                player->model.anim.keyframeIdx = FP_FROM(player->model.anim.time, Q12_SHIFT);
                extra->model.anim.keyframeIdx       = FP_FROM(extra->model.anim.time, Q12_SHIFT);
            }
            break;
#endif

#ifdef HAS_PlayerState_Unk135
        case PlayerState_Unk135:
            func_8007FC48(player, extra, ANIM_STATUS(205, false));

            Player_AnimStateReset(player, extra);

            sharedFunc_800CDAA8_0_s02_Switch();
            break;
#endif

#ifdef HAS_PlayerState_Unk136
        case PlayerState_Unk136:
            func_8007FB94(player, extra, ANIM_STATUS(205, true));

            Player_AnimStateReset(player, extra);

            sharedFunc_800CDAA8_0_s02_Switch();
            break;
#endif

#ifdef HAS_PlayerState_Unk137
        case PlayerState_Unk137:
            func_8007FB94(player, extra, ANIM_STATUS(206, false));

            Player_AnimStateReset(player, extra);

            sharedFunc_800CDAA8_0_s02_Switch();
            break;
#endif

#ifdef HAS_PlayerState_Unk138
        case PlayerState_Unk138:
            func_8007FB94(player, extra, ANIM_STATUS(206, true));
            keyframeIdx = 728;

            Player_AnimStateReset(player, extra);

            sharedFunc_800CDAA8_0_s02_Switch();
            break;
#endif

#ifdef HAS_PlayerState_Unk139
        case PlayerState_Unk139:
            func_8007FC48(player, extra, ANIM_STATUS(207, false));

            Player_AnimStateReset(player, extra);

            sharedFunc_800CDAA8_0_s02_Switch();
            break;
#endif

#ifdef HAS_PlayerState_Unk140
        case PlayerState_Unk140:
            func_8007FC48(player, extra, ANIM_STATUS(207, true));
            keyframeIdx = 982;

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk141
        case PlayerState_Unk141:
            func_8007FB94(player, extra, ANIM_STATUS(208, false));
            keyframeIdx = 1066;

            Player_AnimStateReset(player, extra);

            sharedFunc_800CDAA8_0_s02_Switch();
            break;
#endif

#ifdef HAS_PlayerState_Unk142
        case PlayerState_Unk142:
            func_8007FB94(player, extra, ANIM_STATUS(208, true));
            keyframeIdx = 1016;

            Player_AnimStateReset(player, extra);

            sharedFunc_800CDAA8_0_s02_Switch();
            break;
#endif

#ifdef HAS_PlayerState_Unk143
        case PlayerState_Unk143:
            func_8007FB94(player, extra, ANIM_STATUS(209, false));

            Player_AnimStateReset(player, extra);


            sharedFunc_800CDAA8_0_s02_Switch();
            break;
#endif

#ifdef HAS_PlayerState_Unk144
        case PlayerState_Unk144:
            func_8007FC48(player, extra, ANIM_STATUS(209, true));
            Player_AnimStateReset(player, extra);

            sharedFunc_800CDAA8_0_s02_Switch();
            break;
#endif

#ifdef HAS_PlayerState_Unk145
        case PlayerState_Unk145:
            func_8007FC48(player, extra, ANIM_STATUS(210, false));

            Player_AnimStateReset(player, extra);

            sharedFunc_800CDAA8_0_s02_Switch();
            break;
#endif

#ifdef HAS_PlayerState_Unk146
        case PlayerState_Unk146:
            func_8007FB94(player, extra, ANIM_STATUS(210, true));
            keyframeIdx = KEYFRAME_PlayerState_Unk146;

            Player_AnimStateReset(player, extra);

            sharedFunc_800CDAA8_0_s02_Switch();
            break;
#endif

#ifdef HAS_PlayerState_Unk147
        case PlayerState_Unk147:
            func_8007FC48(player, extra, ANIM_STATUS(211, false));
            keyframeIdx = KEYFRAME_PlayerState_Unk147;

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk148
        case PlayerState_Unk148:
            func_8007FB94(player, extra, ANIM_STATUS(211, true));

            Player_AnimStateReset(player, extra);

            sharedFunc_800CDAA8_0_s02_Switch();
            break;
#endif

#ifdef HAS_PlayerState_Unk149
        case PlayerState_Unk149:
            func_8007FB94(player, extra, ANIM_STATUS(212, false));
            keyframeIdx = 728;

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk150
        case PlayerState_Unk150:
            func_8007FC48(player, extra, ANIM_STATUS(212, true));
            keyframeIdx = 793;

            Player_AnimStateReset(player, extra);

            sharedFunc_800CDAA8_0_s02_Switch();
            break;
#endif

#ifdef HAS_PlayerState_Unk151
        case PlayerState_Unk151:
            keyframeIdx = 1106;

            if (player->model.anim.keyframeIdx == (keyframeIdx + 11) ||
                player->model.anim.keyframeIdx == (keyframeIdx + 7))
            {
                if (!(playerProps.flags & PlayerFlag_Unk6))
                {
                    func_8005F6B0(&g_SysWork.npcs[1], &Q12_VECTOR3(14.94f, -0.05f, 102.0f), 3, 3);
                    playerProps.flags |= PlayerFlag_Unk6;
                }
            }

            if (player->model.anim.keyframeIdx == (keyframeIdx + 8) ||
                player->model.anim.keyframeIdx == (keyframeIdx + 19))
            {
                playerProps.flags &= ~PlayerFlag_Unk6;
            }

            func_8007FB94(player, extra, ANIM_STATUS(213, false));

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk152
        case PlayerState_Unk152:
            func_8007FC48(player, extra, ANIM_STATUS(213, true));

            Player_AnimStateReset(player, extra);

            sharedFunc_800CDAA8_0_s02_Switch();
            break;
#endif

#ifdef HAS_PlayerState_Unk153
        case PlayerState_Unk153:
            func_8007FB94(player, extra, ANIM_STATUS(214, false));
            keyframeIdx = KEYFRAME_PlayerState_Unk153;

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk154
        case PlayerState_Unk154:
            keyframeIdx = 1006;

            if (player->model.anim.keyframeIdx > keyframeIdx + 72)
            {
                func_8003D03C();
            }
            else
            {
                func_8003D01C();
            }

            func_8007FB94(player, extra, ANIM_STATUS(214, true));

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk155
        case PlayerState_Unk155:
            func_8007FB94(player, extra, ANIM_STATUS(215, false));
            keyframeIdx = 678;

            Player_AnimStateReset(player, extra);

            sharedFunc_800CDAA8_0_s02_Switch();
            break;
#endif

#ifdef HAS_PlayerState_Unk156
        case PlayerState_Unk156:
            func_8007FB94(player, extra, ANIM_STATUS(215, true));
            keyframeIdx = 733;

            Player_AnimStateReset(player, extra);

            sharedFunc_800CDAA8_0_s02_Switch();
            break;
#endif

#ifdef HAS_PlayerState_Unk157
        case PlayerState_Unk157:
            func_8007FB94(player, extra, ANIM_STATUS(216, false));
            keyframeIdx = 793;

            Player_AnimStateReset(player, extra);

            sharedFunc_800CDAA8_0_s02_Switch();
            break;
#endif

#ifdef HAS_PlayerState_Unk158
        case PlayerState_Unk158:
            func_8007FB94(player, extra, ANIM_STATUS(216, true));
            keyframeIdx = 834;

            Player_AnimStateReset(player, extra);

            sharedFunc_800CDAA8_0_s02_Switch();
            break;
#endif

#ifdef HAS_PlayerState_Unk159
        case PlayerState_Unk159:
            func_8007FC48(player, extra, ANIM_STATUS(217, false));
            keyframeIdx = 170;

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk160
        case PlayerState_Unk160:
            func_8007FC48(player, extra, ANIM_STATUS(217, true));
            keyframeIdx = 1126;

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk161
        case PlayerState_Unk161:
            func_8007FC48(player, extra, ANIM_STATUS(218, false));
            keyframeIdx = 1207;

            Player_AnimStateReset(player, extra);

            if (player->model.anim.keyframeIdx == 1236)
            {
                Player_ExtraStateSet(player, extra, PlayerState_Unk189);
            }
            break;
#endif

#ifdef HAS_PlayerState_Unk169
        case PlayerState_Unk169:
            keyframeIdx = 864;

            if (player->model.anim.keyframeIdx <= keyframeIdx + 19)
            {
                func_8003D01C();
            }
            else
            {
                func_8003D03C();
            }

            func_8007FB94(player, extra, ANIM_STATUS(222, false));

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk175
        case PlayerState_Unk175:
            func_8007FB94(player, extra, ANIM_STATUS(225, false));

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk186
        case PlayerState_Unk186:
            func_8007FB94(player, extra, ANIM_STATUS(230, true));
#ifdef MAP5_S02
            keyframeIdx = 889;
#endif
            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk164
        case PlayerState_Unk164:
            func_8007FC48(player, extra, ANIM_STATUS(219, true));

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk165
        case PlayerState_Unk165:
            keyframeIdx = 953;

            if (player->model.anim.keyframeIdx == keyframeIdx + 5)
            {
                if (!(playerProps.flags & PlayerFlag_Unk6))
                {
                    func_8006342C(EquippedWeaponId_Handgun, Q12_ANGLE(73.125f), player->rotation.vy, boneCoords);
                    func_8005F6B0(&g_SysWork.npcs[1], &Q12_VECTOR3(140.39f, -0.55f, 22.76f), 3, 3);
                    playerProps.flags |= PlayerFlag_Unk6;
                }
            }

            if (player->model.anim.keyframeIdx == (keyframeIdx + 22))
            {
                playerProps.flags &= ~PlayerFlag_Unk6;
            }

            func_8007FC48(player, extra, ANIM_STATUS(220, false)); // TODO: Demagic

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk166
        case PlayerState_Unk166:
            func_8007FB94(player, extra, ANIM_STATUS(220, true));

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk187
        case PlayerState_Unk187:
            func_8007FB94(player, extra, ANIM_STATUS(231, false));

            Player_AnimStateReset(player, extra);

            sharedFunc_800CDAA8_0_s02_Switch();
            break;
#endif

#ifdef HAS_PlayerState_Unk189
        case PlayerState_Unk189:
            func_8007FC48(player, extra, ANIM_STATUS(232, false));

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk162
        case PlayerState_Unk162:
            func_8007FB94(player, extra, ANIM_STATUS(218, true));
            keyframeIdx = KEYFRAME_PlayerState_Unk162;

            Player_AnimStateReset(player, extra);

            sharedFunc_800CDAA8_0_s02_Switch();
            break;
#endif

#ifdef HAS_PlayerState_Unk163
        case PlayerState_Unk163:
            func_8007FC48(player, extra, ANIM_STATUS(219, false));

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk167
        case PlayerState_Unk167:
            func_8007FB94(player, extra, ANIM_STATUS(221, false));
            keyframeIdx = KEYFRAME_PlayerState_Unk167;

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk168
        case PlayerState_Unk168:
            func_8007FB94(player, extra, ANIM_STATUS(221, true));
            keyframeIdx = 1363;

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk171
        case PlayerState_Unk171:
            func_8007FB94(player, extra, ANIM_STATUS(223, false));
            keyframeIdx = 937;

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk172
        case PlayerState_Unk172:
            func_8007FC48(player, extra, ANIM_STATUS(223, true));
            keyframeIdx = 961;

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk173
        case PlayerState_Unk173:
            func_8007FC48(player, extra, ANIM_STATUS(224, false));

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk174
        case PlayerState_Unk174:
            func_8007FC48(player, extra, ANIM_STATUS(224, true));

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk176
        case PlayerState_Unk176:
            keyframeIdx = 1087;
            func_8003D01C();
            func_8007FB94(player, extra, ANIM_STATUS(225, true));

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk177
        case PlayerState_Unk177:
            func_8003D01C();
            func_8007FB94(player, extra, ANIM_STATUS(226, false));

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk184
        case PlayerState_Unk184:
            func_8003D01C();
            func_8007FB94(player, extra, ANIM_STATUS(229, true));

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk178
        case PlayerState_Unk178:
            func_8003D01C();
            func_8007FB94(player, extra, ANIM_STATUS(226, true));

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk179
        case PlayerState_Unk179:
            keyframeIdx = 1198;

            if (player->model.anim.keyframeIdx >= keyframeIdx + 23)
            {
                func_8003D01C();
            }
            else
            {
                func_8003D03C();
            }

            func_8007FB94(player, extra, ANIM_STATUS(227, false));

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk180
        case PlayerState_Unk180:
            func_8003D01C();
            func_8007FB94(player, extra, ANIM_STATUS(227, true));

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk181
        case PlayerState_Unk181:
            func_8003D01C();
            func_8007FC48(player, extra, ANIM_STATUS(228, false));

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk182
        case PlayerState_Unk182:
            func_8007FC48(player, extra, ANIM_STATUS(228, true));

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk183
        case PlayerState_Unk183:
            func_8007FC48(player, extra, ANIM_STATUS(229, false));
            keyframeIdx = 1358;

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk185
        case PlayerState_Unk185:
            func_8007FC48(player, extra, ANIM_STATUS(230, false));
            keyframeIdx = 1363;

            Player_AnimStateReset(player, extra);
            break;
#endif

#ifdef HAS_PlayerState_Unk170
        case PlayerState_Unk170:
            func_8007FB94(player, extra, ANIM_STATUS(222, true));

            Player_AnimStateReset(player, extra);

            keyframeIdx = 1411;

            if (player->model.anim.keyframeIdx < 1417)
            {
                func_8003D01C();
            }
            else
            {
                func_8003D03C();
            }
            break;
#endif

#ifdef HAS_PlayerState_Unk188
        case PlayerState_Unk188:
            func_8007FB94(player, extra, ANIM_STATUS(231, true));

            Player_AnimStateReset(player, extra);

            sharedFunc_800CDAA8_0_s02_Switch();
            break;
#endif
    }

    Collision_SurfaceGet(&surface, player->position.vx, player->position.vz);
    Player_FootstepSfxGet(surface.floorType, &footstepSfxId, &pitch0, &pitch1);

    switch (g_SysWork.playerWork.extra.state)
    {
#ifdef HAS_PlayerState_Reset
        case PlayerState_Reset:
            if (playerProps.flags & PlayerFlag_Moving)
            {
                if (player->model.anim.status == ANIM_STATUS(HarryAnim_Idle, false))
                {
                    Sfx_WithPitchPlay(footstepSfxId, &player->position, Q8(0.25f), pitch0);

                    player->properties.player.field_10C                  = pitch0 + 16;
                    playerProps.flags &= ~PlayerFlag_Moving;
                }
            }
            break;
#endif

#ifdef HAS_PlayerState_WalkForward
        case PlayerState_WalkForward:
            Player_FootstepSfxPlay(5, player, 18, 6, footstepSfxId, pitch0);
            playerProps.flags |= PlayerFlag_Moving;
            break;
#endif

#ifdef HAS_PlayerState_RunForward
        case PlayerState_RunForward:
            if (Player_FootstepSfxPlay(7, player, 31, 41, footstepSfxId, pitch1) != false)
            {
                player->properties.player.runStepSfxCount++;
            }

            playerProps.flags |= PlayerFlag_Moving;
            break;
#endif

#ifdef HAS_PlayerState_TurnRight
        case PlayerState_TurnRight:
            Player_FootstepSfxPlay(27, player, 204, 200, footstepSfxId, pitch0);
            playerProps.flags &= ~PlayerFlag_Moving;
            break;
#endif

#ifdef HAS_PlayerState_TurnLeft
        case PlayerState_TurnLeft:
            Player_FootstepSfxPlay(25, player, 187, 191, footstepSfxId, pitch0);
            playerProps.flags &= ~PlayerFlag_Moving;
            break;
#endif

#ifdef HAS_PlayerState_Unk59
        case PlayerState_Unk59:
            Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 5, 0, footstepSfxId, pitch0);
            break;
#endif

#ifdef HAS_PlayerState_Unk60
        case PlayerState_Unk60:
            Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 7, 0, footstepSfxId, pitch0);
            break;
#endif

#ifdef HAS_PlayerState_Unk76
        case PlayerState_Unk76:
            if (ANIM_STATUS_IS_ACTIVE(player->model.anim.status))
            {
                if (player->model.anim.keyframeIdx < (keyframeIdx + 31))
                {
                    func_80071620(player->model.anim.status, player, keyframeIdx + 1, Sfx_Unk1386);
                }
                else
                {
                    func_80071620(player->model.anim.status, player, keyframeIdx + 33, Sfx_Unk1369);
                }
            }
            break;
#endif

#ifdef HAS_PlayerState_Unk81
        case PlayerState_Unk81:
            if ((keyframeIdx + 19) >= player->model.anim.keyframeIdx)
            {
                Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 31, keyframeIdx + 19, footstepSfxId, pitch0);
            }
            else
            {
                Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 31, keyframeIdx + 40, footstepSfxId, pitch0);
            }

            playerProps.flags &= ~PlayerFlag_Moving;
            break;
#endif

#ifdef HAS_PlayerState_Unk87
        case PlayerState_Unk87:
            Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 35, keyframeIdx + 17, Sfx_Unk1423, pitch0);
            playerProps.flags &= ~PlayerFlag_Moving;
            break;
#endif

#ifdef HAS_PlayerState_Unk88
        case PlayerState_Unk88:
            Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 15, keyframeIdx + 31, Sfx_Unk1423, pitch0);
            playerProps.flags &= ~PlayerFlag_Moving;
            break;
#endif

#ifdef HAS_PlayerState_Unk91
        case PlayerState_Unk91:
            Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 6, keyframeIdx + 6, footstepSfxId, pitch0);
            playerProps.flags &= ~PlayerFlag_Moving;
            break;
#endif

#ifdef HAS_PlayerState_Unk92
        case PlayerState_Unk92:
            Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 9, keyframeIdx + 9, footstepSfxId, pitch0);
            playerProps.flags &= ~PlayerFlag_Moving;
            break;
#endif

#ifdef HAS_PlayerState_Unk93
        case PlayerState_Unk93:
            if (ANIM_STATUS_IS_ACTIVE(player->model.anim.status))
            {
                func_80071620(player->model.anim.status, player, keyframeIdx + 6, Sfx_Unk1385);
                Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 18, 0, footstepSfxId, pitch0);
                playerProps.flags &= ~PlayerFlag_Moving;
            }
            break;
#endif

#ifdef HAS_PlayerState_Unk89
        case PlayerState_Unk89:
            if ((keyframeIdx + 5) >= player->model.anim.keyframeIdx)
            {
                Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 5, keyframeIdx + 16, footstepSfxId, pitch0);
            }
            else
            {
                Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 74, keyframeIdx + 77, footstepSfxId, pitch0);
            }

            playerProps.flags &= ~PlayerFlag_Moving;
            break;
#endif

#ifdef HAS_PlayerState_Unk97
        case PlayerState_Unk97:
            if ((keyframeIdx + 4) >= player->model.anim.keyframeIdx)
            {
                Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 10, keyframeIdx + 4, footstepSfxId, pitch0);
            }
            else
            {
                Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 10, keyframeIdx + 13, footstepSfxId, pitch0);
            }

            func_80071620(player->model.anim.status, player, keyframeIdx + 26, Sfx_Unk1456);
            break;
#endif

#ifdef HAS_PlayerState_Unk105
        case PlayerState_Unk105:
            Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 5, keyframeIdx + 12, footstepSfxId, pitch0);
            playerProps.flags &= ~PlayerFlag_Moving;
            break;
#endif

#ifdef HAS_PlayerState_Unk113
        case PlayerState_Unk113:
            if ((keyframeIdx + 5) >= player->model.anim.keyframeIdx)
            {
                Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 14, keyframeIdx + 5, footstepSfxId, pitch0);
            }
            else
            {
                Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 14, keyframeIdx + 21, footstepSfxId, pitch0);
            }

            playerProps.flags &= ~PlayerFlag_Moving;
            break;
#endif

#ifdef HAS_PlayerState_Unk111
        case PlayerState_Unk111:
            if (ANIM_STATUS_IS_ACTIVE(player->model.anim.status))
            {
                Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 11, keyframeIdx + 21, footstepSfxId, pitch0);
            }

            playerProps.flags &= ~PlayerFlag_Moving;
            break;
#endif

#ifdef HAS_PlayerState_Unk114
        case PlayerState_Unk114:
            if ((keyframeIdx + 8) >= player->model.anim.keyframeIdx)
            {
                Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 13, keyframeIdx + 8, footstepSfxId, pitch0);
            }
            else
            {
                Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 13, keyframeIdx + 18, footstepSfxId, pitch0);
            }

            playerProps.flags &= ~PlayerFlag_Moving;
            break;
#endif

#ifdef HAS_PlayerState_Unk115
        case PlayerState_Unk115:
            if ((keyframeIdx + 9) >= player->model.anim.keyframeIdx)
            {
                Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 9, keyframeIdx + 33, footstepSfxId, pitch0);
            }
            else
            {
                if (keyframeIdx + 39 >= player->model.anim.keyframeIdx)
                {
                    Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 39, keyframeIdx + 33, footstepSfxId, pitch0);
                }
                else
                {
                    Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 46, keyframeIdx + 42, footstepSfxId, pitch0);
                }
            }
            break;
#endif

#ifdef HAS_PlayerState_Unk117
        case PlayerState_Unk117:
            Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 34, 0, footstepSfxId, pitch0);
            break;
#endif

#ifdef HAS_PlayerState_Unk118
        case PlayerState_Unk118:
            if ((keyframeIdx + 8) >= player->model.anim.keyframeIdx)
            {
                Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 12, keyframeIdx + 8, footstepSfxId, pitch0);
            }
            else
            {
                Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 12, keyframeIdx + 16, footstepSfxId, pitch0);
            }

            if (ANIM_STATUS_IS_ACTIVE(player->model.anim.status))
            {
                func_80071620(player->model.anim.status, player, keyframeIdx + 9, Sfx_Unk1539);
            }
            break;
#endif

#ifdef HAS_PlayerState_Unk120
        case PlayerState_Unk120:
            if ((keyframeIdx + 5) >= player->model.anim.keyframeIdx)
            {
                Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 17, keyframeIdx + 5, footstepSfxId, pitch0);
            }
            else
            {
                Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 17, keyframeIdx + 25, footstepSfxId, pitch0);
            }
            break;
#endif

#ifdef HAS_PlayerState_Unk121
        case PlayerState_Unk121:
            if ((keyframeIdx + 5) >= player->model.anim.keyframeIdx)
            {
                Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 5, keyframeIdx + 12, footstepSfxId, pitch0);
            }
            else if ((keyframeIdx + 23) >= player->model.anim.keyframeIdx)
            {
                Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 23, keyframeIdx + 12, footstepSfxId, pitch0);
            }
            else
            {
                Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 5, keyframeIdx + 29, footstepSfxId, pitch0);
            }
            break;
#endif

#ifdef HAS_PlayerState_Unk122
        case PlayerState_Unk122:
            if ((keyframeIdx + 9) >= player->model.anim.keyframeIdx)
            {
                Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 14, keyframeIdx + 9, footstepSfxId, pitch0);
            }
            else
            {
                Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 14, keyframeIdx + 19, footstepSfxId, pitch0);
            }
            break;
#endif

#ifdef HAS_PlayerState_Unk123
        case PlayerState_Unk123:
            if ((keyframeIdx + 9) >= player->model.anim.keyframeIdx)
            {
                Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 14, keyframeIdx + 9, footstepSfxId, pitch0);
            }
            else
            {
                Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 14, keyframeIdx + 19, footstepSfxId, pitch0);
            }
            break;
#endif

#ifdef HAS_PlayerState_Unk124
        case PlayerState_Unk124:
            if (Player_FootstepSfxPlay(player->model.anim.status, player, 80, 87, footstepSfxId, pitch1))
            {
                player->properties.player.runStepSfxCount++;
            }
            break;
#endif

#ifdef HAS_PlayerState_Unk126
        case PlayerState_Unk126:
            if ((keyframeIdx + 9) >= player->model.anim.keyframeIdx)
            {
                Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 16, keyframeIdx + 9, footstepSfxId, pitch0);
            }
            else
            {
                Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 16, keyframeIdx + 22, footstepSfxId, pitch0);
            }
            break;
#endif

#ifdef HAS_PlayerState_Unk127
        case PlayerState_Unk127:
            if ((keyframeIdx + 5) >= player->model.anim.keyframeIdx)
            {
                Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 17, keyframeIdx + 5, footstepSfxId, pitch0);
            }
            else
            {
                Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 17, keyframeIdx + 26, footstepSfxId, pitch0);
            }
            break;
#endif

#ifdef HAS_PlayerState_Unk133
        case PlayerState_Unk133:
            Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 5, keyframeIdx + 5, footstepSfxId, pitch0);
            break;
#endif

#ifdef HAS_PlayerState_Unk138
        case PlayerState_Unk138:
            if (ANIM_STATUS_IS_ACTIVE(player->model.anim.status))
            {
                Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 18, keyframeIdx + 14, footstepSfxId, pitch0);
            }
            break;
#endif

#ifdef HAS_PlayerState_Unk140
        case PlayerState_Unk140:
            Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 25, keyframeIdx + 25, footstepSfxId, pitch0);
            break;
#endif

#ifdef HAS_PlayerState_Unk141
        case PlayerState_Unk141:
            Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 46, keyframeIdx + 5, footstepSfxId, pitch0);
            break;
#endif

#ifdef HAS_PlayerState_Unk142
        case PlayerState_Unk142:
            Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 10, keyframeIdx + 10, footstepSfxId, pitch0);
            break;
#endif

#ifdef HAS_PlayerState_Unk130
        case PlayerState_Unk130:
            Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 14, keyframeIdx + 14, footstepSfxId, pitch0);
            break;
#endif

#ifdef HAS_PlayerState_Unk146
        case PlayerState_Unk146:
            Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 16, keyframeIdx + 10, footstepSfxId, pitch0);
            break;
#endif

#ifdef HAS_PlayerState_Unk147
        case PlayerState_Unk147:
            Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 11, keyframeIdx + 11, footstepSfxId, pitch0);
            break;
#endif

#ifdef HAS_PlayerState_Unk149
        case PlayerState_Unk149:
            if (player->model.anim.keyframeIdx < (keyframeIdx + 2))
            {
                func_80071620(player->model.anim.status, player, keyframeIdx, Sfx_Unk1626);
            }
            else
            {
                if (player->model.anim.keyframeIdx < (keyframeIdx + 8))
                {
                    if (func_80071620(player->model.anim.status, player, keyframeIdx + 6, Q12(0.4f)) != false)
                    {
                        func_800892DC(8, 0x30); // Hex?
                    }
                }
                else
                {
                    if (player->model.anim.keyframeIdx < (keyframeIdx + 41))
                    {
                        func_80071620(player->model.anim.status, player, keyframeIdx + 3, Sfx_Unk1627);
                    }
                    else
                    {
                        func_80071620(player->model.anim.status, player, keyframeIdx + 42, Sfx_Unk1628);
                    }
                }
            }

            if ((keyframeIdx + 43) >= player->model.anim.keyframeIdx)
            {
                Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 55, keyframeIdx + 43, footstepSfxId, pitch0);
            }
            else
            {
                Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 55, keyframeIdx + 59, footstepSfxId, pitch0);
            }
            break;
#endif

#ifdef HAS_PlayerState_Unk150
        case PlayerState_Unk150:
            if (func_80071620(player->model.anim.status, player, keyframeIdx + 21, Q12(0.4f)) != false)
            {
                func_800892DC(8, 0x30); // Hex?
            }

            if (keyframeIdx + 8 >= player->model.anim.keyframeIdx)
            {
                Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 8, keyframeIdx + 11, footstepSfxId, pitch0);
            }
            else if ((keyframeIdx + 14) >= player->model.anim.keyframeIdx)
            {
                Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 14, keyframeIdx + 11, footstepSfxId, pitch0);
            }
            else
            {
                Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 8, keyframeIdx + 16, footstepSfxId, pitch0);
            }

            break;
#endif

#ifdef HAS_PlayerState_Unk151
        case PlayerState_Unk151:
            if (ANIM_STATUS_IS_ACTIVE(player->model.anim.status))
            {
                if ((keyframeIdx + 7) >= player->model.anim.keyframeIdx)
                {
                    func_80071620(player->model.anim.status, player, keyframeIdx + 7, Sfx_Unk1619);
                }
                else
                {
                    func_80071620(player->model.anim.status, player, keyframeIdx + 11, Sfx_Unk1619);
                }

                Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 16, keyframeIdx + 16, footstepSfxId, pitch0);
            }
            break;
#endif

#ifdef HAS_PlayerState_Unk153
        case PlayerState_Unk153:
            Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 5, keyframeIdx + 12, footstepSfxId, pitch0);
            break;
#endif

#ifdef HAS_PlayerState_Unk154
        case PlayerState_Unk154:
            Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 7, keyframeIdx + 21, footstepSfxId, pitch0);
            break;
#endif

#ifdef HAS_PlayerState_Unk155
        case PlayerState_Unk155:
            if (player->model.anim.keyframeIdx < 34)
            {
                if (keyframeIdx + 10 >= player->model.anim.keyframeIdx)
                {
                    Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 10, keyframeIdx + 16, footstepSfxId, pitch0);
                }
                else
                {
                    if (keyframeIdx + 21 >= player->model.anim.keyframeIdx)
                    {
                        Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 21, keyframeIdx + 16, footstepSfxId, pitch0);
                    }
                    else
                    {
                        Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 28, keyframeIdx + 25, footstepSfxId, pitch0);
                    }
                }
            }
            else if ((keyframeIdx + 16) >= player->model.anim.keyframeIdx)
            {
                Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 34, keyframeIdx + 38, footstepSfxId, pitch0);
            }
            else
            {
                Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 42, keyframeIdx + 46, footstepSfxId, pitch0);
            }
            break;
#endif

#ifdef HAS_PlayerState_Unk156
        case PlayerState_Unk156:
            if (player->model.anim.keyframeIdx < 59)
            {
                if ((keyframeIdx + 9) >= player->model.anim.keyframeIdx)
                {
                    Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 9, keyframeIdx + 22, footstepSfxId, pitch0);
                }
                else
                {
                    if ((keyframeIdx + 30) >= player->model.anim.keyframeIdx)
                    {
                        Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 30, keyframeIdx + 22, footstepSfxId, pitch0);
                    }
                    else
                    {
                        Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 52, keyframeIdx + 42, footstepSfxId, pitch0);
                    }
                }
            }
            else
            {
                Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 59, keyframeIdx + 59, footstepSfxId, pitch1);
            }
            break;
#endif

#ifdef HAS_PlayerState_Unk157
        case PlayerState_Unk157:
            Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 33, keyframeIdx + 25, footstepSfxId, pitch1);
            break;
#endif

#ifdef HAS_PlayerState_Unk158
        case PlayerState_Unk158:
            Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 12, keyframeIdx + 5, footstepSfxId, pitch1);
            break;
#endif

#ifdef HAS_PlayerState_Unk159
        case PlayerState_Unk159:
            Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 11, 0, footstepSfxId, pitch1);
            break;
#endif

#ifdef HAS_PlayerState_Unk160
        case PlayerState_Unk160:
            if ((keyframeIdx + 33) >= player->model.anim.keyframeIdx)
            {
                Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 42, keyframeIdx + 33, footstepSfxId, pitch0);
            }
            else
            {
                Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 42, keyframeIdx + 55, footstepSfxId, pitch0);
            }
            break;
#endif

#ifdef HAS_PlayerState_Unk161
        case PlayerState_Unk161:
            Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 24, keyframeIdx + 22, footstepSfxId, pitch0);
            break;
#endif

#ifdef HAS_PlayerState_Unk175
        case PlayerState_Unk175:
            Player_FootstepSfxPlay(player->model.anim.status, player, 18, 6, footstepSfxId, pitch0);
            break;
#endif

#ifdef HAS_PlayerState_Unk186
        case PlayerState_Unk186:
            Player_FootstepSfxPlay(player->model.anim.status, player, 0, keyframeIdx + 12, footstepSfxId, pitch0);
            playerProps.flags |= PlayerFlag_Moving;
            break;
#endif

#ifdef HAS_PlayerState_Unk162
        case PlayerState_Unk162:
            if (ANIM_STATUS_IS_ACTIVE(player->model.anim.status))
            {
#ifdef MAP7_S03
                func_80071620(player->model.anim.status, player, keyframeIdx + 60, Sfx_Unk1671);
#else
                func_80071620(player->model.anim.status, player, keyframeIdx + 60, Sfx_Unk1623);
#endif
            }
            break;
#endif

#ifdef HAS_PlayerState_Unk163
        case PlayerState_Unk163:
            Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 6, 0, footstepSfxId, pitch0);
            break;
#endif

#ifdef HAS_PlayerState_Unk165
        case PlayerState_Unk165:
            func_80071620(player->model.anim.status, player, keyframeIdx + 5, Sfx_Unk1283);
            break;
#endif

#ifdef HAS_PlayerState_Unk167
        case PlayerState_Unk167:
            Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 10, 0, footstepSfxId, pitch0);
            break;
#endif

#ifdef HAS_PlayerState_Unk168
        case PlayerState_Unk168:
            Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 38, keyframeIdx + 22, footstepSfxId, pitch0);
            break;
#endif

#ifdef HAS_PlayerState_Unk172
        case PlayerState_Unk172:
            Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 29, 0, footstepSfxId, pitch0);
            break;
#endif

#ifdef HAS_PlayerState_Unk176
        case PlayerState_Unk176:
            Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 8, keyframeIdx + 19, footstepSfxId, pitch0);
            break;
#endif

#ifdef HAS_PlayerState_Unk179
        case PlayerState_Unk179:
            Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 8, keyframeIdx + 44, footstepSfxId, pitch0);
            break;
#endif

#ifdef HAS_PlayerState_Unk183
        case PlayerState_Unk183:
            if ((keyframeIdx + 11) >= player->model.anim.keyframeIdx)
            {
                Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 28, keyframeIdx + 11, footstepSfxId, pitch0);
            }
            else
            {
                Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 28, keyframeIdx + 55, footstepSfxId, pitch0);
            }
            break;
#endif

#ifdef HAS_PlayerState_Unk171
        case PlayerState_Unk171:
            Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 9, keyframeIdx + 14, footstepSfxId, pitch0);
            break;
#endif

#ifdef HAS_PlayerState_Unk185
        case PlayerState_Unk185:
            Player_FootstepSfxPlay(player->model.anim.status, player, keyframeIdx + 38, keyframeIdx + 22, footstepSfxId, pitch0);
            break;
#endif

#ifdef HAS_PlayerState_Unk170
        case PlayerState_Unk170:
            func_80071620(player->model.anim.status, player, keyframeIdx + 7, Sfx_Unk1629);
            break;
#endif
        default:
            playerProps.flags &= ~PlayerFlag_Moving;
            break;
    }

    newMoveSpeed = g_Player_MoveSpeed;

    player->rotation.vy  = Q12_ANGLE_NORM_U((player->rotation.vy + (sharedData_800E39D8_0_s00 >> 4)) + Q12_ANGLE(360.0f));
    player->headingAngle = Q12_ANGLE_NORM_U(player->rotation.vy + headingAngle);
    player->moveSpeed    = newMoveSpeed;
    player->fallSpeed   += g_GravitySpeed;

    boneCoords->flg = false;
    Math_RotMatrixZxyNegGte(&player->rotation, &boneCoords[HarryBone_Root].coord);

    #undef playerChara
    #undef playerProps
}
