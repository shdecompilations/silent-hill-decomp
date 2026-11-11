#include "bodyprog/bodyprog.h"
#include "bodyprog/player/player.h"
#include "maps/common/characters/player.h"

#if defined(MAP1_S04) || defined(MAP1_S05) || defined(MAP2_S01) || defined(MAP2_S03) || defined(MAP2_S04) || defined(MAP3_S02) || defined(MAP4_S00) || defined(MAP4_S06) || defined(MAP6_S05)
// .bss data in maps we've matched .bss for
// As more maps get matched we can hopefully find proper order for these.
s32    sharedData_800E39D8_0_s00;
q19_12 g_Player_MoveSpeed;
#endif

#if defined(MAP2_S01)
s_CollisionResult sharedData_800E39BC_0_s00;
#endif

// Player-related functions kept inside map overlay for some reason.
//
// TODO:
//  - This should be a separate .c file/split in each map overlay, but for now #including this .c works.
//  - sharedData and structs that are only used in this file need to be moved from `shared.h` into `player.h`
//
// NOTES:
//  - All functions are declared in every map, but some function bodies are only included in specific maps.
//  - `sharedFunc_800CDAA8_0_s02` is the huge PlayerState function, probably best to keep that as separate file.

// NOTE: Huge PlayerState anim function, kept as separate file for now.
#include "maps/common/shared/sharedFunc_800CDAA8_0_s02.h" // 0x800CBAE4

void sharedFunc_800D1C38_0_s00(s_SubCharacter* player, s_PlayerExtra* extra, GsCOORDINATE2* boneCoords)
{
    s_CollisionSurface surface;
    VECTOR3            offset;
    q19_12             headingAngle;
    q3_12              moveOffsetAlphaZ;
    q19_12             moveOffsetZ;
    q19_12             moveDist;
    q19_12             moveOffsetX;
    q3_12              moveOffsetAlphaX;
    s32                scaleRestoreShift;
    u32                scaleReduceShift;
    q19_12             moveSpeed;
    bool               cond;
    q3_12              adjMoveOffsetX;
    q3_12              adjMoveOffsetZ;

    cond = false;
    if (PLAYER_EXTRA().state < PlayerState_Unk58)
    {
        cond = true;
    }

#if defined(MAP0_S00)
    else if (PLAYER_EXTRA().state == PlayerState_Unk74)
    {
        cond = g_SavegamePtr->mapIdx == MapIdx_MAP1_S02;
    }

#elif defined(MAP1_S01) || defined(MAP1_S02) || defined(MAP1_S03) || defined(MAP1_S06) || \
      defined(MAP2_S00) || defined(MAP3_S01) || defined(MAP3_S03) || defined(MAP3_S04) || \
      defined(MAP3_S05) || defined(MAP4_S03) || defined(MAP4_S05) || defined(MAP5_S00) || \
      defined(MAP5_S02) || defined(MAP5_S03) || defined(MAP7_S01) || defined(MAP7_S02) || \
      defined(MAP7_S03)

    else
    {
        switch (PLAYER_EXTRA().state)
        {
#ifdef HAS_PlayerState_Unk128
            case PlayerState_Unk128:
                cond = true;
                break;
#endif

#ifdef HAS_PlayerState_Unk129
            case PlayerState_Unk129:
                cond = true;
                break;
#endif

#ifdef HAS_PlayerState_Unk109
            case PlayerState_Unk109:
                cond = true;
                break;
#endif

#ifdef HAS_PlayerState_Unk176
            case PlayerState_Unk176:
                cond = true;
                break;
#endif

#ifdef HAS_PlayerState_Unk59
            case PlayerState_Unk59:
                cond = true;
                break;
#endif

#ifdef HAS_PlayerState_Unk60
            case PlayerState_Unk60:
                cond = true;
                break;
#endif

#ifdef HAS_PlayerState_Unk81
            case PlayerState_Unk81:
                cond = true;
                break;
#endif

#ifdef HAS_PlayerState_Unk74
            case PlayerState_Unk74:
                cond = (g_SavegamePtr->mapIdx == 5);
                break;
#endif

            default:
                cond = false;
                break;
        }
    }
#endif

#if defined(MAP0_S01)
    if (PLAYER_EXTRA().state == PlayerState_WalkForward)
    {
        cond = false;
    }
#endif

    if (cond)
    {
        // TODO: Similar block found in `Player_PositionUpdate`. Maybe an inline?

        Collision_SurfaceGet(&surface, player->position.vx, player->position.vz);

        // Compute speed displacement.
        moveOffsetX = Math_Sin(player->headingAngle);
        moveOffsetX = Q12_MULT(player->moveSpeed, moveOffsetX);
        moveOffsetZ = Math_Cos(player->headingAngle);
        moveOffsetZ = Q12_MULT(player->moveSpeed, moveOffsetZ);

        // Compute displacement alpha from floor slope.
        moveOffsetAlphaX = Math_Cos(DIV_FAST(ABS(surface.slopeGradientX), 8));
        moveOffsetAlphaZ = Math_Cos(DIV_FAST(ABS(surface.slopeGradientZ), 8));

        // Compute adjusted displacement.
        adjMoveOffsetX = Q12_MULT(Q12_MULT(moveOffsetX, moveOffsetAlphaX), moveOffsetAlphaX);
        adjMoveOffsetZ = Q12_MULT(Q12_MULT(moveOffsetZ, moveOffsetAlphaZ), moveOffsetAlphaZ);
    }
    else
    {
        adjMoveOffsetX = Q12_MULT(player->moveSpeed, Math_Sin(player->headingAngle));
        adjMoveOffsetZ = Q12_MULT(player->moveSpeed, Math_Cos(player->headingAngle));
    }

    if (player->moveSpeed >= Q12(0.0f))
    {
        player->moveSpeed = Math_Vector2MagCalc(adjMoveOffsetX, adjMoveOffsetZ);
    }
    else
    {
        player->moveSpeed = -Math_Vector2MagCalc(adjMoveOffsetX, adjMoveOffsetZ);
    }

    moveSpeed    = player->moveSpeed;
    headingAngle = player->headingAngle;
    moveDist     = Q12_MULT_PRECISE(moveSpeed, g_DeltaTime);

    scaleRestoreShift = OVERFLOW_GUARD(moveDist);
    scaleReduceShift  = scaleRestoreShift >> 1;

    offset.vx = Q12_MULT_PRECISE(moveDist >> scaleReduceShift, Math_Sin(headingAngle) >> scaleReduceShift) << scaleRestoreShift;
    offset.vz = Q12_MULT_PRECISE(moveDist >> scaleReduceShift, Math_Cos(headingAngle) >> scaleReduceShift) << scaleRestoreShift;
    offset.vy = Q12_MULT_PRECISE(player->fallSpeed, g_DeltaTime);

    if (cond)
    {
        // Most maps use `g_Player_CollisionResult` from bodyprog, but some use struct from inside map overlay.
#if defined(MAP0_S00) || defined(MAP2_S01) || defined(MAP3_S00) || defined(MAP3_S06) || \
    defined(MAP5_S02) || defined(MAP5_S03) || defined(MAP6_S01) || defined(MAP6_S04) || \
    defined(MAP7_S02) || defined(MAP7_S03)
    #define UnkStruct sharedData_800E39BC_0_s00
#else
    #define UnkStruct g_Player_CollisionResult
#endif
        Collision_WallDetect(&UnkStruct, &offset, player);
        player->position.vx += UnkStruct.offset.vx;
        player->position.vy += UnkStruct.offset.vy;
        player->position.vz += UnkStruct.offset.vz;

        if (UnkStruct.surface.floorType == FloorType_Default)
        {
            UnkStruct.surface.floorHeight = player->properties.player.floorHeight;
        }

        if (player->position.vy > UnkStruct.surface.floorHeight)
        {
            player->position.vy = UnkStruct.surface.floorHeight;
            player->fallSpeed   = Q12(0.0f);
        }
    }
    else
    {
        player->position.vx += offset.vx;
        player->position.vz += offset.vz;

        if (PLAYER_EXTRA().state < PlayerState_Unk87 ||
            (PLAYER_EXTRA().state >= PlayerState_Unk89 &&
             PLAYER_EXTRA().state != PlayerState_Unk106))
        {
            player->position.vy = Q12(0.0f);
        }

        player->fallSpeed = Q12(0.0f);
    }

    if (g_DeltaTime == Q12(0.0f))
    {
        player->rotationSpeed.vy = Q12_ANGLE(0.0f);
    }
    else
    {
        player->rotationSpeed.vy = FP_TO(sharedData_800E39D8_0_s00, Q8_SHIFT) / g_DeltaTime;
    }

    boneCoords[HarryBone_Root].coord.t[0] = Q12_TO_Q8(player->position.vx);
    boneCoords[HarryBone_Root].coord.t[1] = Q12_TO_Q8(player->position.vy);
    boneCoords[HarryBone_Root].coord.t[2] = Q12_TO_Q8(player->position.vz);
}

void Player_CutsceneWeaponUnequip(void)
{
    PLAYER_COMBAT().weaponAttack = NO_VALUE;
    g_SavegamePtr->equippedWeapon       = InvItemId_Unequipped;

    Player_ControlFreeze();
    Player_AnimStateSet(PlayerState_Unk84);
}

void Player_MatchArmAnimDisable(void)
{
    g_SysWork.enablePlayerMatchAnim = false;
}

void Player_ControlFreeze(void)
{
    s_PlayerExtra*  playerExtra;
    s_SubCharacter* playerChara;

    playerExtra = &PLAYER_EXTRA();
    playerChara = &PLAYER_CHARA();

    g_Player_PrevWeaponAttack = NO_VALUE;

    g_Player_CutsceneState               = PlayerCutsceneState_RunForward;
    D_800C4588                       = 0;
    g_Player_DisableControl          = true;
    g_Player_IsInWalkToRunTransition = false;

    g_Player_MoveStickMag = Q7(0.0f);

    playerChara->properties.player.afkTimer        = Q12(0.0f);
    playerChara->properties.player.runStepSfxCount = 0;
    playerChara->properties.player.exhaustionTimer = Q12(0.0f);

    Player_ExtraStateSet(playerChara, playerExtra, PlayerState_Reset);

    g_Player_IsShooting          = false;
    g_Player_IsAttacking         = false;
    g_Player_IsHoldAttack        = false;
    g_Player_IsAiming            = false;
    g_Player_IsRunning           = false;
    g_Player_IsMovingBackward    = false;
    g_Player_IsMovingForward     = false;
    g_Player_IsSteppingRightTap  = false;
    g_Player_IsSteppingRightHold = false;
    g_Player_IsTurningRight      = false;
    g_Player_IsSteppingLeftTap   = false;
    g_Player_IsSteppingLeftHold  = false;
    g_Player_IsTurningLeft       = false;

    sharedData_800E39D8_0_s00 = 0;

    playerChara->collision.cylinder.innerRadius  = Q12(0.0f);
    PLAYER_PROPS().flags                        |= PlayerFlag_Unk4 | PlayerFlag_Unk5;
    playerChara->flags                          |= CharaFlag_Unk4;
    playerChara->collision.state                 = CharaCollisionState_4;

    if (PLAYER_COMBAT().weaponAttack < WEAPON_ATTACK(EquippedWeaponId_Handgun, AttackInputType_Tap))
    {
        PLAYER_COMBAT().weaponAttack = WEAPON_ATTACK_ID_GET(PLAYER_COMBAT().weaponAttack);
    }
}

void Player_ControlUnfreeze(bool setIdle)
{
    s_SubCharacter* player;
    s_PlayerExtra*  extra;

    g_Player_DisableControl = false;

    extra  = &PLAYER_EXTRA();
    player = &PLAYER_CHARA();

    if (setIdle)
    {
        PLAYER_EXTRA().state          = PlayerState_None;
        PLAYER_EXTRA().upperBodyState = PlayerUpperBodyState_None;
        PLAYER_EXTRA().lowerBodyState = PlayerLowerBodyState_None;

        // TODO: Will `Chara_AnimSet` work here?
        player->model.anim.status      = ANIM_STATUS(HarryAnim_Idle, true);
        player->model.anim.keyframeIdx = 503;
        extra->model.anim.status       = ANIM_STATUS(HarryAnim_Idle, true);
        extra->model.anim.keyframeIdx  = 503;
        player->model.anim.time        = Q12(503);
        player->model.anim.alpha       = Q12(1.0f);
        extra->model.anim.time         = Q12(503);
        extra->model.anim.alpha        = Q12(1.0f);
    }
    else
    {
        Player_ExtraStateSet(player, extra, PlayerState_None);
    }

    player->attackReceived = NO_VALUE;
    player->field_40          = NO_VALUE;
    g_SysWork.npcIdxs[1] = NO_VALUE;
    g_SysWork.npcIdxs[0] = NO_VALUE;

#if !defined(MAP0_S00) && !defined(MAP0_S01)
    func_8003D01C();
#endif

    g_Player_IsShooting          = false;
    g_Player_IsAttacking         = false;
    g_Player_IsHoldAttack        = false;
    g_Player_IsAiming            = false;
    g_Player_IsRunning           = false;
    g_Player_IsMovingBackward    = false;
    g_Player_IsMovingForward     = false;
    g_Player_IsSteppingRightTap  = false;
    g_Player_IsSteppingRightHold = false;
    g_Player_IsTurningRight      = false;
    g_Player_IsSteppingLeftTap   = false;
    g_Player_IsSteppingLeftHold  = false;
    g_Player_IsTurningLeft       = false;

    PLAYER_COMBAT().isAiming = false;

    player->collision.cylinder.innerRadius = DEFAULT_PLAYER_CYLINDER_INNER_RADIUS;
    Player_CollisionReset();
    PLAYER_PROPS().flags &= ~(PlayerFlag_Unk2 |
                              PlayerFlag_SfxActive |
                              PlayerFlag_DamageReceived |
                              PlayerFlag_Moving);

    player->flags          &= ~CharaFlag_Unk4;
    player->collision.state = CharaCollisionState_Npc;

    Player_AnimFlagsSet(AnimFlag_Unlocked);

    g_Player_PrevPosition.vx = player->position.vx;
    g_Player_PrevPosition.vz = player->position.vz;

#if !defined(MAP0_S00)
    g_Player_FlexRotationY = Q12_ANGLE(0.0f);
    g_Player_FlexRotationX = Q12_ANGLE(0.0f);
#endif
}

// Very similar to `Chara_PathWaypointExecute`.
bool Player_PathWaypointExecute(s32 playerExtraState, VECTOR3* vec, q3_12 angle, s32 vecCount)
{
#if defined(MAP0_S00) || defined(MAP1_S02) || defined(MAP3_S05) || defined(MAP4_S05) || \
    defined(MAP7_S00) || defined(MAP7_S01)

    #define ANGLE_THRESHOLD (Q12_ANGLE(360.0) >> 6) // 360 / 64 = 5.625 degrees.

    VECTOR3*        localVec;
    s32             playerVecDist;
    s16             playerRotDelta;
    s32             angleStep;
    s_SubCharacter* playerChara;
    s_PlayerExtra*  playerExtra;

    angle       = Q12_ANGLE_ABS(angle);
    playerExtra = &PLAYER_EXTRA();
    playerChara = &PLAYER_CHARA();
    localVec    = vec;

    g_Player_FlexRotationY = Q12_ANGLE(0.0f);

    switch (D_800C4588)
    {
        case 0:
            sharedData_800DD5A4_0_s00 = 0;
            D_800C4588                = 1;
            break;

        case 1:
            if (localVec->vx == playerChara->position.vx && localVec->vz == playerChara->position.vz)
            {
                D_800C4588 = 6;
                break;
            }

            sharedData_800E39E0_0_s00 = Q12_ANGLE_ABS(ratan2(localVec->vx - playerChara->position.vx, localVec->vz - playerChara->position.vz));
            Math_ShortestAngleGet(playerChara->rotation.vy, sharedData_800E39E0_0_s00, &playerRotDelta);

            if (ABS(playerRotDelta) < ANGLE_THRESHOLD)
            {
                playerChara->rotation.vy = sharedData_800E39E0_0_s00;
                D_800C4588                  = 3;
                break;
            }

            if (playerRotDelta < Q12_ANGLE(0.0f))
            {
                g_Player_CutsceneState = PlayerCutsceneState_TurnLeft;
                Player_ExtraStateSet(playerChara, playerExtra, PlayerState_TurnLeft);
                D_800C4588 = 2;
            }
            else
            {
                g_Player_CutsceneState = PlayerCutsceneState_TurnRight;
                Player_ExtraStateSet(playerChara, playerExtra, PlayerState_TurnRight);
                D_800C4588 = 2;
            }

            break;

        case 2:
            Math_ShortestAngleGet(playerChara->rotation.vy, sharedData_800E39E0_0_s00, &playerRotDelta);
            if (ABS(playerRotDelta) < ANGLE_THRESHOLD)
            {
                playerChara->rotation.vy = sharedData_800E39E0_0_s00;
                D_800C4588                  = 3;
            }
            break;

        case 3:
            playerVecDist = SquareRoot0(Q12_2D_DISTANCE_SQR(localVec[0], playerChara->position));
            if (ABS((int)playerVecDist) < Q8(0.15f)) // @hack Needs to be `int` for `ABS` to match?
            {
                Player_ExtraStateSet(playerChara, playerExtra, PlayerState_Reset);
                D_800C4588 = 8;
                break;
            }

            Player_ExtraStateSet(playerChara, playerExtra, playerExtraState);
            switch (playerExtraState)
            {
                case PlayerState_RunForward:
                    g_Player_CutsceneState = PlayerCutsceneState_RunForward;
                    break;

                case PlayerState_WalkForward:
                    g_Player_CutsceneState = PlayerCutsceneState_WalkForward;
                    break;
            }

            D_800C4588 = 4;
            break;

        case 4:
            sharedData_800E39E0_0_s00 = Q12_ANGLE_ABS(ratan2(localVec[sharedData_800DD5A4_0_s00].vx -
                                                            playerChara->position.vx, localVec[sharedData_800DD5A4_0_s00].vz -
                                                            playerChara->position.vz));
            if (sharedData_800DD5A4_0_s00 + 1 < vecCount)
            {
                sharedData_800E39E2_0_s00 = Q12_ANGLE_ABS(ratan2(localVec[sharedData_800DD5A4_0_s00 + 1].vx -
                                                                playerChara->position.vx, localVec[sharedData_800DD5A4_0_s00 + 1].vz -
                                                                playerChara->position.vz));
            }
            else
            {
                sharedData_800E39E2_0_s00 = sharedData_800E39E0_0_s00;
            }

            sharedData_800DD5A0_0_s00 = SquareRoot0(Q12_2D_DISTANCE_SQR(localVec[sharedData_800DD5A4_0_s00], playerChara->position));
            D_800C4588                = 5;

            Math_ShortestAngleGet(playerChara->rotation.vy, sharedData_800E39E2_0_s00, &g_Player_FlexRotationY);

            g_Player_FlexRotationY = CLAMP(g_Player_FlexRotationY, Q12_ANGLE(-45.0f), Q12_ANGLE(45.0f));

            break;

        case 5:
            if (sharedData_800DD5A4_0_s00 + 1 < vecCount)
            {
                sharedData_800E39E2_0_s00 = ratan2(localVec[sharedData_800DD5A4_0_s00 + 1].vx -
                                                   playerChara->position.vx, localVec[sharedData_800DD5A4_0_s00 + 1].vz -
                                                   playerChara->position.vz);
            }
            else
            {
                sharedData_800E39E2_0_s00 = ratan2(localVec[sharedData_800DD5A4_0_s00].vx -
                                                   playerChara->position.vx, localVec[sharedData_800DD5A4_0_s00].vz -
                                                   playerChara->position.vz);
            }

            sharedData_800E39E2_0_s00 = Q12_ANGLE_ABS(sharedData_800E39E2_0_s00);

            Math_ShortestAngleGet(playerChara->rotation.vy, sharedData_800E39E2_0_s00, &playerRotDelta);

            // Turn toward.
            angleStep = TIMESTEP_SCALE_30_FPS(g_DeltaTime, (playerRotDelta < Q12_ANGLE(45.0f)) ? Q12_ANGLE(2.9f) : Q12_ANGLE(22.5f));

            if (ABS(playerRotDelta) < ANGLE_THRESHOLD)
            {
                playerChara->rotation.vy = sharedData_800E39E2_0_s00;
            }
            else if (playerRotDelta < Q12_ANGLE(0.0f))
            {
                playerChara->rotation.vy -= angleStep;
            }
            else
            {
                playerChara->rotation.vy += angleStep;
            }

            playerChara->rotation.vy = Q12_ANGLE_ABS(playerChara->rotation.vy);

            playerVecDist = SquareRoot0(Q12_2D_DISTANCE_SQR(localVec[sharedData_800DD5A4_0_s00], playerChara->position));

            // Uses `playerExtraState` index to work out distance to check against? Odd.
            if (playerVecDist < Q12_TO_Q8((playerExtraState - 53) * Q12(0.15f) + Q12(0.15f)) ||
                (sharedData_800DD5A0_0_s00 < playerVecDist && sharedData_800DD5A4_0_s00 != vecCount - 1))
            {
                sharedData_800DD5A4_0_s00++;

                playerVecDist = SquareRoot0(Q12_2D_DISTANCE_SQR(localVec[sharedData_800DD5A4_0_s00], playerChara->position));

                if (sharedData_800DD5A4_0_s00 == vecCount)
                {
                    D_800C4588 = 6;
                }
                else
                {
                    D_800C4588 = 4;
                }
            }

            sharedData_800DD5A0_0_s00 = playerVecDist;

            Math_ShortestAngleGet(playerChara->rotation.vy, sharedData_800E39E2_0_s00, &g_Player_FlexRotationY);
            g_Player_FlexRotationY = CLAMP(g_Player_FlexRotationY, Q12_ANGLE(-45.0f), Q12_ANGLE(45.0f));
            break;

        case 6:
            Math_ShortestAngleGet(playerChara->rotation.vy, angle, &playerRotDelta);
            if (ABS(playerRotDelta) < ANGLE_THRESHOLD)
            {
                playerChara->rotation.vy = angle;
                D_800C4588                  = 8;
                break;
            }

            if (playerRotDelta < Q12_ANGLE(0.0f))
            {
                g_Player_CutsceneState = PlayerCutsceneState_TurnLeft;
                Player_ExtraStateSet(playerChara, playerExtra, PlayerState_TurnLeft);
            }
            else
            {
                g_Player_CutsceneState = PlayerCutsceneState_TurnRight;
                Player_ExtraStateSet(playerChara, playerExtra, PlayerState_TurnRight);
            }

            D_800C4588 = 7;
            break;

        case 7:
            Math_ShortestAngleGet(playerChara->rotation.vy, angle, &playerRotDelta);
            if (ABS(playerRotDelta) < ANGLE_THRESHOLD)
            {
                playerChara->rotation.vy = angle;
                Player_ExtraStateSet(playerChara, playerExtra, PlayerState_Reset);
                g_Player_CutsceneState = PlayerCutsceneState_RunForward;
                D_800C4588 = 8;
            }
            break;

        case 8:
            g_Player_AnimResetRequest = true;
            sharedData_800E39E0_0_s00 = 0;
            D_800C4588                = 0;
            PLAYER_PROPS().moveSpeed  = Q12(0.0f);
            return true;
    }
#endif
    return false;
}

void Player_AnimStateSet(s32 playerExtraState)
{
    s_SubCharacter* player;
    s_PlayerExtra*  extra;

    extra  = &PLAYER_EXTRA();
    player = &PLAYER_CHARA();

    PLAYER_PROPS().moveSpeed = Q12(0.0f);

    g_Player_AnimResetRequest = false;

    switch (playerExtraState)
    {
        case PlayerState_RunForward:
            g_Player_CutsceneState = PlayerCutsceneState_RunForward;
            break;

        case PlayerState_WalkForward:
            g_Player_CutsceneState = PlayerCutsceneState_WalkForward;
            break;

        case PlayerState_TurnRight:
            g_Player_CutsceneState = PlayerCutsceneState_TurnRight;
            break;

        case PlayerState_TurnLeft:
            g_Player_CutsceneState = PlayerCutsceneState_TurnLeft;
            break;
    }

    Player_ExtraStateSet(player, extra, playerExtraState);
}

void Player_AnimReset(void)
{
    g_Player_AnimResetRequest++;
}

void Player_AnimLock(void)
{
    Player_AnimFlagsClear(AnimFlag_Unlocked);
}

s32 Player_AnimIsLocked(void)
{
    return ~(PLAYER_CHARA().model.anim.flags & AnimFlag_Unlocked);
}

void Player_AnimUnlock(void)
{
    Player_AnimFlagsSet(AnimFlag_Unlocked);
}

s32 Player_AnimPlaybackStateGet(void)
{
    s_Model*    model;
    s_AnimInfo* animInfo;

    // NOTE: There are 37 base anims for Harry. 38 and beyond are map-specific.
    model    = &PLAYER_CHARA().model;
    animInfo = &g_MapOverlayHdr.harryMapAnimInfos[model->anim.status - ANIM_STATUS(38, false)];

    if (animInfo->playbackFunc == Anim_PlayOnce)
    {
        // Check if anim is in progress or finished.
        if (Anim_DurationGet(model, animInfo) > Q12(0.0f))
        {
            return (model->anim.keyframeIdx == animInfo->endKeyframeIdx) ? AnimPlaybackState_End :
                                                                           AnimPlaybackState_Active;
        }
        else
        {
            return (model->anim.keyframeIdx == animInfo->startKeyframeIdx) ? AnimPlaybackState_End :
                                                                             AnimPlaybackState_Active;
        }
    }

    if (animInfo->playbackFunc == Anim_BlendLinear)
    {
        return AnimPlaybackState_Blend;
    }
    else
    {
        return AnimPlaybackState_Invalid;
    }
}

bool Player_MoveSpeedIsZero(void)
{
    return PLAYER_PROPS().moveSpeed == Q12(0.0f);
}

void Player_MoveSpeedClear(void)
{
    PLAYER_PROPS().moveSpeed = Q12(0.0f);
}

void sharedFunc_800D2E6C_0_s00(void)
{
#if defined(MAP0_S01)
    PLAYER_COMBAT().weaponAttack = WEAPON_ATTACK(EquippedWeaponId_Handgun, AttackInputType_Tap);
    WorldGfx_PlayerPrevHeldItem(&PLAYER_COMBAT());

    PLAYER_COMBAT().weaponAttack = NO_VALUE;

    func_8003D03C();
#endif
}

void Player_FallBackward(void)
{
#if defined(MAP0_S01)
    s_SubCharacter* player;
    s_PlayerExtra*  playerExtra;

    player      = &PLAYER_CHARA();
    playerExtra = &PLAYER_EXTRA();

    PLAYER_PROPS().moveSpeed    = Q12(2.3f);
    PLAYER_PROPS().headingAngle = Q12_ANGLE(180.0f);
    g_Player_HeadingAngle       = Q12_ANGLE(180.0f);

    Player_ExtraStateSet(player, playerExtra, PlayerState_FallBackward);
#endif
}

void Player_Stub_F8(void) {}

void Player_DamageFeetFront(void)
{
#if defined(MAP6_S04)
    s_SubCharacter* playerChara;
    s_PlayerExtra*  playerExtra;

    playerChara = &PLAYER_CHARA();
    playerExtra = &PLAYER_EXTRA();

    Sfx_WithFlagsPlay(Sfx_HarryFallGasp, &playerChara->position, Q8(1.0f / 8.0f), SfxFlag_None);

    Player_ExtraStateSet(playerChara, playerExtra, PlayerState_DamageFeetFront);
#endif
}

void sharedFunc_800D2E8C_0_s00(q19_12 posX, q19_12 posZ, VECTOR3* vec)
{
#if defined(MAP1_S05)
    q19_12          angle;
    s32             vecSqr;
    s32             playerExtraState;
    s_SubCharacter* playerChara;
    s_PlayerExtra*  playerExtra;

    playerChara = &PLAYER_CHARA();
    playerExtra = &PLAYER_EXTRA();

    if (vec->vx == 0 && vec->vz == 0)
    {
        if (D_800D587C != 0)
        {
            D_800D587C = 0;
        }
    }
    else
    {
        PLAYER_PROPS().field_118 = Q12_ANGLE_ABS(ratan2(vec->vx, vec->vz));
    }

    if (posX == Q12(0.0f) && posZ == Q12(0.0f))
    {
        return;
    }

    D_800C45B0.vx = posX;
    D_800C45B0.vz = posZ;

    if (g_SysWork.npcs[0].health <= Q12(0.0f) ||
        g_Player_IsInWalkToRunTransition || playerChara->health <= Q12(0.0f))
    {
        return;
    }

    vecSqr      = SQUARE(vec->vx) + SQUARE(vec->vz);
    D_800D587C += SquareRoot0(vecSqr);

    if (vecSqr > Q12(0.75f))
    {
        if (PLAYER_EXTRA().state <  PlayerState_DamagePushBack ||
            PLAYER_EXTRA().state >= PlayerState_Unk31)
        {
            angle = Q12_ANGLE_ABS(Q12_ANGLE_ABS(ratan2(vec->vx, vec->vz)) - (u16)playerChara->rotation.vy);

            if (D_800D587C > Q12(1.75f))
            {
                if (PLAYER_EXTRA().state >= 23 &&
                    PLAYER_EXTRA().state <  27)
                {
                    playerChara->model.stateStep    = 0;
                    playerChara->model.controlState = 0;
                    playerExtra->model.stateStep    = 0;
                    playerExtra->model.controlState = 0;
                }

                if (angle >= Q12_ANGLE(90.0f) && angle < Q12_ANGLE(270.0f))
                {
                    playerExtraState = PlayerState_DamagePushFront;
                }
                else
                {
                    playerExtraState = PlayerState_DamagePushBack;
                }

                Player_ExtraStateSet(playerChara, playerExtra, playerExtraState);
                Sfx_WithFlagsPlay(Sfx_Unk1326, &playerChara->position, Q8(1.0f / 8.0f), SfxFlag_None);
            }
            else if ((PLAYER_EXTRA().state <  PlayerState_DamageTorsoBack ||
                      PLAYER_EXTRA().state >= PlayerState_DamageFeetFront) &&
                     PLAYER_EXTRA().state != PlayerState_DamagePushBack &&
                     PLAYER_EXTRA().state != PlayerState_DamagePushFront)
            {
                if (angle >= Q12_ANGLE(45.0f) && angle < Q12_ANGLE(135.0f))
                {
                    playerExtraState = PlayerState_DamageTorsoLeft;
                }
                else if (angle >= Q12_ANGLE(135.0f) && angle < Q12_ANGLE(225.0f))
                {
                    playerExtraState = PlayerState_DamageTorsoFront;
                }
                else if (angle >= Q12_ANGLE(225.0f) && angle < Q12_ANGLE(315.0f))
                {
                    playerExtraState = PlayerState_DamageTorsoRight;
                }
                else
                {
                    playerExtraState = PlayerState_DamageTorsoBack;
                }

                Player_ExtraStateSet(playerChara, playerExtra, playerExtraState);
            }

            PLAYER_PROPS().flags &= ~PlayerFlag_Unk12;

            playerChara->properties.player.afkTimer = Q12(0.0f);
            playerChara->properties.player.field_F4 = 0;
            PLAYER_COMBAT().isAiming         = false;
            playerChara->field_44.field_0           = NO_VALUE;

            PLAYER_PROPS().flags &= ~PlayerFlag_Unk9;

            playerChara->field_44.field_0 = NO_VALUE; // Redundant set needed for match.
        }
    }

    if (vecSqr > Q12(1.75f))
    {
        playerChara->damage.amount = SquareRoot0(vecSqr) * 4;
    }
    else
    {
        playerChara->damage.amount = SquareRoot0(vecSqr) * 64;
    }

    if (!(PLAYER_PROPS().flags & PlayerFlag_DamageReceived) &&
        vecSqr > Q12(0.75f) && vecSqr <= Q12(1.75f))
    {
        Sfx_WithFlagsPlay(Sfx_Unk1327, &playerChara->position, Q8(1.0f / 8.0f), SfxFlag_None);

        playerChara->properties.player.field_10C = 64;
        PLAYER_PROPS().flags |= PlayerFlag_DamageReceived;
    }

    if (playerChara->damage.amount != Q12(0.0f))
    {
        PLAYER_PROPS().flags &= ~PlayerFlag_Unk2;

        playerChara->health -= playerChara->damage.amount;
        func_800893D0(playerChara->damage.amount);
        playerChara->damage.amount = Q12(0.0f);
    }
#endif
}

bool sharedFunc_800D2E94_0_s00(void)
{
#if defined(MAP2_S00)
    s_SubCharacter* npc;
    q19_12          playerDist;

    npc = &g_SysWork.npcs[5];

    if (PLAYER_CHARA().position.vx < Q12(-255.0f) &&
        PLAYER_CHARA().position.vz > Q12(-110.0f) &&
        PLAYER_CHARA().position.vz < Q12(-100.0f))
    {
        playerDist = SquareRoot0(SQUARE(Q12(-262.0f) - PLAYER_CHARA().position.vx) +
                                 SQUARE(Q12(-104.0f) - PLAYER_CHARA().position.vz));

        if (npc->model.controlState == 0 || npc->model.stateStep == 0)
        {
            npc->health                             = Q12(400.0f);
            npc->model.controlState++;
            npc->collision.box.topExtent            = Q12(-0.2f);
            npc->collision.box.bottomExtent         = Q12(0.2f);
            npc->collision.cylinder.outerRadius     = Q12(0.05f);
            npc->collision.cylinder.innerRadius     = Q12(0.5f);
            npc->field_40                           = 0;
            npc->collision.box.offsetY              = Q12(0.0f);
            npc->collision.box.field_8              = Q12(0.0f);
            npc->collision.shapeOffsets.cylinder.vz = Q12(0.0f);
            npc->collision.shapeOffsets.cylinder.vx = Q12(0.0f);
            npc->collision.shapeOffsets.box.vz      = Q12(0.0f);
            npc->collision.shapeOffsets.box.vx      = Q12(0.0f);
            npc->position.vx                        = Q12(-262.0f);
            npc->position.vy                        = DEFAULT_PLAYER_BOX_OFFSET_Y;
            npc->position.vz                        = Q12(-104.0f);
            npc->collision.state                    = CharaCollisionState_Npc;
            npc->model.stateStep++;
            npc->flags                             |= CharaFlag_Hit;
            npc->model.anim.flags                  &= ~(AnimFlag_Visible | AnimFlag_Unlocked);
        }

        // TODO: `else` branch is duplicated here, is there some way to merge them? Decompiler used `goto`.
        if (npc->health > Q12(0.0f))
        {
            if (playerDist < Q12(2.0f) &&
                Savegame_EventFlagGet(EventFlag_167) && !Savegame_EventFlagGet(EventFlag_168) &&
                PLAYER_COMBAT().weaponAttack >= WEAPON_ATTACK(EquippedWeaponId_KitchenKnife, AttackInputType_Tap))
            {
                npc->model.charaId = Chara_Padlock;
            }
            else
            {
                if (npc->health <= Q12(0.0f))
                {
                    if (Savegame_EventFlagGet(EventFlag_167) && npc->model.charaId == Chara_Padlock)
                    {
                        npc->flags |= CharaFlag_PadlockBroken;
                        Savegame_EventFlagSet(EventFlag_168);
                    }
                }

                npc->model.charaId = Chara_None;
            }
        }
        else
        {
            if (Savegame_EventFlagGet(EventFlag_167) && npc->model.charaId == Chara_Padlock)
            {
                npc->flags |= CharaFlag_PadlockBroken;
                Savegame_EventFlagSet(EventFlag_168);
            }

            npc->model.charaId = Chara_None;
        }

        if (npc->damage.amount != Q12(0.0f))
        {
            if (WEAPON_ATTACK_ID_GET(PLAYER_COMBAT().weaponAttack) == EquippedWeaponId_KitchenKnife)
            {
                npc->damage.amount >>= 1;
            }

            // Apply `damageReceived` to character health.
            if (PLAYER_COMBAT().weaponAttack >= WEAPON_ATTACK(EquippedWeaponId_Handgun, AttackInputType_Tap))
            {
                npc->health = NO_VALUE;
            }
            else
            {
                npc->health -= npc->damage.amount;
            }

            npc->damage.amount = Q12(0.0f);
        }

        return true;
    }

    npc->model.controlState = 0;
    npc->model.stateStep    = 0;
    npc->model.charaId      = Chara_None;
    npc->health             = Q12(0.0f);
#endif
    return false;
}

/** @brief Checks if two angles are within the forward arc.
 *
 * @param angle0 First angle.
 * @param angle1 Second angle.
 * @return `true` if the angles are within the forward arc, `false` otherwise.
 */
static inline bool Math_AngleFrontCheck(q4_12 angle0, q4_12 angle1)
{
    q19_12 angleDelta = Q12_ANGLE_ABS(angle0 - angle1);
    return angleDelta < Q12_ANGLE(90.0f) || angleDelta >= Q12_ANGLE(270.0f);
}

void sharedFunc_800D2E9C_0_s00(q19_12* offsetX, q19_12* offsetZ, q3_12* angle)
{
#if defined(MAP2_S00) || defined(MAP2_S02) || defined(MAP4_S02) || defined(MAP5_S01) || defined(MAP6_S00)
    VECTOR3 vec; // Q19.12
    q3_12   headingAngle;
    q3_12   angle0;
    bool    isInFront;

    PLAYER_PROPS().moveSpeed = Q12(0.0f);
    PLAYER_CHARA().headingAngle                = Q12_ANGLE(0.0f);
    isInFront                                               = Math_AngleFrontCheck(*angle, PLAYER_CHARA().rotation.vy);

    // @hack Permuter find, needed for match.
    angle--;
    angle++;

    if (!isInFront)
    {
        D_800C4610.vx = *offsetX + FP_FROM(( Math_Cos(*angle) * -0x16C) + (Math_Sin(*angle) * -0x6B0), Q12_SHIFT);
        D_800C4610.vz = *offsetZ + FP_FROM((-Math_Sin(*angle) * -0x16C) + (Math_Cos(*angle) * -0x6B0), Q12_SHIFT);
    }
    else
    {
        D_800C4610.vx = *offsetX + FP_FROM(( Math_Cos(*angle) * -0xCB) + (Math_Sin(*angle) * 0x3C6), Q12_SHIFT);
        D_800C4610.vz = *offsetZ + FP_FROM((-Math_Sin(*angle) * -0xCB) + (Math_Cos(*angle) * 0x3C6), Q12_SHIFT);
    }

    headingAngle = PLAYER_CHARA().headingAngle;
    vec.vx       = Math_Sin(headingAngle); // @unused
    vec.vx       = Q12(0.0f);
    vec.vz       = Math_Cos(headingAngle); // @unused
    vec.vz       = Q12(0.0f);

    vec.vx = D_800C4610.vx - PLAYER_CHARA().position.vx;
    vec.vz = D_800C4610.vz - PLAYER_CHARA().position.vz;
    vec.vy = Q12_MULT_PRECISE(PLAYER_CHARA().fallSpeed, g_DeltaTime);

    Collision_WallDetect(&g_Player_CollisionResult, &vec, &PLAYER_CHARA());

    D_800C4610.vx = PLAYER_CHARA().position.vx + g_Player_CollisionResult.offset.vx;
    D_800C4610.vz = PLAYER_CHARA().position.vz + g_Player_CollisionResult.offset.vz;

    // TODO: Convert hex to float or fraction.
    if (!isInFront)
    {
        angle0   = *angle + Q12_ANGLE(180.0f);
        *offsetX = D_800C4610.vx + FP_FROM(( Math_Cos(angle0) * 0x16C) + (Math_Sin(angle0) * 0x6B0), Q12_SHIFT);
        *offsetZ = D_800C4610.vz + FP_FROM((-Math_Sin(angle0) * 0x16C) + (Math_Cos(angle0) * 0x6B0), Q12_SHIFT);
    }
    else
    {
        angle0   = *angle;
        *offsetX = D_800C4610.vx + FP_FROM(( Math_Cos(angle0) * 0xCB) + (Math_Sin(angle0) * -0x3C6), Q12_SHIFT);
        *offsetZ = D_800C4610.vz + FP_FROM((-Math_Sin(angle0) * 0xCB) + (Math_Cos(angle0) * -0x3C6), Q12_SHIFT);
    }
#endif
}

s32 Player_PropertyField10DGet(void)
{
    return PLAYER_PROPS().field_10D;
}

void Player_EmptyWeaponHandSet(void)
{
    u8 prevWeaponAttack;

    prevWeaponAttack                    = PLAYER_COMBAT().weaponAttack;
    PLAYER_COMBAT().weaponAttack = NO_VALUE;
    g_Player_PrevWeaponAttack           = prevWeaponAttack;

    WorldGfx_CharaMeshSwap(Chara_Harry, MESH_SWAP_STATUS(HarrySwappableMesh_RightHand, HarryVariantMesh_RightHandEmpty));
}

void Player_WeaponAttackRestore(void)
{
    PLAYER_COMBAT().weaponAttack = g_Player_PrevWeaponAttack;
}

