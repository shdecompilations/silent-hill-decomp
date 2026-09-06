#include "game.h"

#include "bodyprog/bodyprog.h"
#include "bodyprog/screen/screen_data.h"
#include "bodyprog/screen/screen_draw.h"
#include "bodyprog/item_screens.h"
#include "bodyprog/player.h"
#include "bodyprog/math/math.h"

static const s32 __pad_rodata_80025E90 = 0;
s32 __pad_bss_800C3964;

void Inventory_ExitAnimEquippedItemUpdate(u8* weaponId) // 0x8004C088
{
    s8       weaponAttack; // Packed weapon attack. See `WEAPON_ATTACK`.
    s32      playerState;
    s_Model* modelPtr3;
    s_Model* extraModelPtr2;
    s_Model* extraModelPtr0;
    s_Model* extraModelPtr1;
    s_Model* modelPtr0;
    s_Model* modelPtr1;

    #define playerChara g_SysWork.playerWork.player
    #define playerExtra g_SysWork.playerWork.extra
    #define playerProps playerChara.properties.player

    Game_PlayerMovementsReset();

    playerState = playerExtra.state;

    if (playerExtra.state < PlayerState_Idle &&
        playerState >= PlayerState_None)
    {
        if (g_Player_WeaponAttack != g_SysWork.playerCombat.weaponAttack)
        {
            g_SysWork.playerCombat.isAiming = false;
        }

        weaponId = WEAPON_ATTACK_ID_GET(g_Player_WeaponAttack);
        if (weaponId == EquippedWeaponId_Chainsaw &&
            g_SysWork.playerCombat.weaponAttack != weaponId)
        {
            goto Inventory_ExitAnimEquippedItemUpdate_block; // TODO: Remove goto if possible.
        }

        if (weaponId == EquippedWeaponId_RockDrill &&
            g_SysWork.playerCombat.weaponAttack != WEAPON_ATTACK(weaponId, AttackInputType_Tap))
        {
        Inventory_ExitAnimEquippedItemUpdate_block:
            playerChara.field_44.field_0    = 0;
            playerProps.gasWeaponPowerTimer = Q12(0.0f);
        }

        switch (playerExtra.lowerBodyState)
        {
            case PlayerLowerBodyState_QuickTurnRight:
            case PlayerLowerBodyState_QuickTurnLeft:
            case PlayerLowerBodyState_JumpBackward:
            case PlayerLowerBodyState_AimQuickTurnRight:
            case PlayerLowerBodyState_AimQuickTurnLeft:
                break;

            default:

                if (playerExtra.state == PlayerState_Combat &&
                    g_Player_WeaponAttack != g_SysWork.playerCombat.weaponAttack)
                {
                    playerExtra.state = PlayerState_None;
                    playerProps.flags = PlayerFlag_None;
                }

                if (playerExtra.lowerBodyState >= PlayerLowerBodyState_Aim &&
                    g_SysWork.playerCombat.weaponAttack != NO_VALUE &&
                    g_Player_WeaponAttack == g_SysWork.playerCombat.weaponAttack)
                {
                    extraModelPtr0 = &playerExtra.model;
                    if (extraModelPtr0->anim.status >= ANIM_STATUS(33, false))
                    {
                        modelPtr0                      = &playerChara.model;
                        extraModelPtr0->anim.status   -= 10;
                        playerChara.model.controlState = 0;
                        playerChara.model.stateStep    = 0;
                        playerExtra.model.controlState = 0;
                        playerExtra.model.stateStep    = 0;
                        modelPtr0->anim.status        -= 10;
                    }
                }
                else
                {
                    modelPtr1      = &playerChara.model;
                    extraModelPtr1 = &playerExtra.model;

                    modelPtr1->anim.status           = ANIM_STATUS(HarryAnim_TransitionToStill, true);
                    modelPtr1->anim.keyframeIdx      = 0;
                    extraModelPtr1->anim.status      = ANIM_STATUS(HarryAnim_TransitionToStill, true);
                    extraModelPtr1->anim.keyframeIdx = 0;

                    playerChara.model.controlState = 0;
                    playerChara.model.stateStep    = 0;
                    playerExtra.upperBodyState     = PlayerUpperBodyState_None;
                    playerExtra.lowerBodyState     = PlayerLowerBodyState_None;
                    playerExtra.model.controlState = 0;
                    playerExtra.model.stateStep    = 0;
                }

                playerProps.moveSpeed = Q12(0.0f);
                playerProps.afkTimer         = Q12(0.0f);
        }
    }

    modelPtr3      = &playerChara.model;
    extraModelPtr2 = &playerExtra.model;

    // Set animation alpha.
    modelPtr3->anim.alpha      = Q12(1.0f);
    extraModelPtr2->anim.alpha = Q12(1.0f);

    // Disable upper body bones.
    playerExtra.disabledAnimBones = HARRY_UPPER_BODY_BONE_MASK;

    // Set animation time.
    modelPtr3->anim.time      = Q12(modelPtr3->anim.keyframeIdx);
    extraModelPtr2->anim.time = Q12(extraModelPtr2->anim.keyframeIdx);

    Anim_BoneUpdate((s_AnmHeader*)FS_BUFFER_0, g_SysWork.playerBoneCoords, modelPtr3->anim.keyframeIdx, modelPtr3->anim.keyframeIdx, Q12(1.0f));

    // Re-enable upper body bones, disable lower body bones.
    playerExtra.disabledAnimBones = HARRY_LOWER_BODY_BONE_MASK;

    Anim_BoneUpdate((s_AnmHeader*)FS_BUFFER_0, g_SysWork.playerBoneCoords, extraModelPtr2->anim.keyframeIdx, extraModelPtr2->anim.keyframeIdx, Q12(1.0f));
    func_8004C040();

    #undef playerChara
    #undef playerExtra
    #undef playerProps
}

bool Inventory_GunsAvailableCheck(bool unused) // 0x8004C328
{
    u32 itemIdx;
    u8  itemId;
    u32 hasHuntingRifle;
    u32 hasShotgun;

    itemIdx         = 0;
    hasHuntingRifle = false;
    hasShotgun      = false;

    for (; (itemId = g_SavegamePtr->items[itemIdx].id) != InvItemId_Unequipped; itemIdx++)
    {
        if (itemId == InvItemId_HuntingRifle)
        {
            hasHuntingRifle = true;
        }

        if (itemId == InvItemId_Shotgun)
        {
            hasShotgun = true;
        }

        if (g_SysWork.playerCombat.weaponInventoryIdx != NO_VALUE && itemIdx == g_SysWork.playerCombat.weaponInventoryIdx)
        {
            if (itemId >= InvItemId_Handgun && itemId <= InvItemId_Shotgun)
            {
                if (g_SysWork.playerCombat.currentWeaponAmmo != 0)
                {
                    return true;
                }
            }
            continue;
        }

        if (itemId == InvItemId_Handgun || itemId == InvItemId_HandgunBullets)
        {
            if (g_SavegamePtr->items[itemIdx].count != 0)
            {
                return true;
            }
            continue;
        }

        // Checks if player has ammo.
        switch (itemId)
        {
            case InvItemId_HuntingRifle:
            case InvItemId_Shotgun:
                if (g_SavegamePtr->items[itemIdx].count != 0)
                {
                    return true;
                }
                break;

            case InvItemId_RifleShells:
                if (hasHuntingRifle)
                {
                    if (g_SavegamePtr->items[itemIdx].count != 0)
                    {
                        return true;
                    }
                }
                break;

            case InvItemId_ShotgunShells:
                if (hasShotgun)
                {
                    if (g_SavegamePtr->items[itemIdx].count != 0)
                    {
                        return true;
                    }
                }
                break;
        }
    }

    return false;
}

s32 Inventory_HyperBlasterUnlockTest(void) // 0x8004C45C
{
    s32 i;

    for (i = 0; i < INV_ITEM_COUNT_MAX; i++)
    {
        // Already in inventory, can't add new one.
        if (g_SavegamePtr->items[i].id == InvItemId_HyperBlaster)
        {
            return NO_VALUE;
        }
    }

    if (g_SavegamePtr->mapIdx > MapIdx_MAP0_S00)
    {
        // Konami gun controller connected.
        if (g_GameWork.controllers[1].analogController.status         == 0 &&
            g_GameWork.controllers[1].analogController.receivedBytes == 1 &&
            g_GameWork.controllers[1].analogController.terminalType  == PadTerminalType_GunControllerKonami)
        {
            return 1;
        }

        // Game completed with some condition met?
        if (g_SavegamePtr->clearGameCount != 0 && (g_SavegamePtr->clearGameEndings & GameEndingFlag_Ufo))
        {
            return 1;
        }
    }

    return 0;
}

s32 Inventory_HyperBlasterFunctionalTest(void) // 0x8004C4F8
{
    if (g_SavegamePtr->mapIdx > MapIdx_MAP0_S00)
    {
        if ((g_SavegamePtr->clearGameEndings & GameEndingFlag_Ufo) != 0)
        {
            // Game completed with Ufo ending.
            return 2;
        }

        // Returns 1 if controller port 2 has Konami gun controller connected.
        return g_GameWork.controllers[1].analogController.status         == 0 &&
               g_GameWork.controllers[1].analogController.receivedBytes == 1 &&
               g_GameWork.controllers[1].analogController.terminalType  == PadTerminalType_GunControllerKonami;
    }

    // Neither of the conditions has been completed.
    return 0;
}

s32 Game_HyperBlasterBeamColorGet(void) // 0x8004C54C
{
    return g_SavegamePtr->pickedUpSpecialItemCount >> 3;
}

void func_8004C564(u8 arg0, s8 weaponAttack) // 0x8004C564
{
    s32 temp_v1;
    static s8 D_800C3960;
    static s8 D_800C3961;
    static s8 D_800C3962;
    static u8 D_800C3963;

    switch (weaponAttack)
    {
        case NO_VALUE:
            D_800C3960 = g_SavegamePtr->mapIdx;
            D_800C3962 = 0;
            D_800C3963 = 0;
            D_800C3961 = 32;

            func_8008B398();

            g_SysWork.playerWork.player.properties.player.gasWeaponPowerTimer = Q12(0.0f);
            break;

        case 0:
            D_800C3960 = g_SavegamePtr->mapIdx;
            D_800C3962 = 0;
            D_800C3961 = 32;

            func_8008B438(arg0 != (1 << 1), 32, 0);
            func_8008B3E4(Q8(0.0f));

            if (arg0 != (1 << 1))
            {
                D_800C3963 |= 1 << 0;
            }
            else
            {
                D_800C3963 |= 1 << 1;
            }
            break;

        case 1:
            if (arg0 == (1 << 1))
            {
                func_8008B40C((u8)D_800C3961, D_800C3962);
                D_800C3963 |= 1 << 2;
            }
            break;

        case 2:
            if (D_800C3961 != 0)
            {
                D_800C3961 -= ((g_DeltaTime / 68) == 0) ? 1 : (g_DeltaTime / 68);
                D_800C3962  = D_800C3961 - 32;
                D_800C3961  = CLAMP(D_800C3961, 0, 32);

                func_8008B438(arg0 != 2, 0, 0);
                func_8008B3E4(NO_VALUE);

                if (arg0 == 2)
                {
                    func_8008B40C((u8)D_800C3961, D_800C3962);
                }

                if (D_800C3961 == 0)
                {
                    func_8008B398();
                    D_800C3963                                                                    = 0;
                    D_800C3962                                                                    = 0;
                    g_SysWork.playerWork.player.properties.player.gasWeaponPowerTimer = Q12(0.0f);
                    g_SysWork.playerWork.player.field_44.field_0                             = 0;
                }
            }
            break;

        case 3:
            if (D_800C3963 & (1 << 1))
            {
                if (!(D_800C3963 & (1 << 2)))
                {
                    func_8008B438(0, 0, 0);
                    func_8008B3E4(Q8(0.0f));

                    g_SysWork.playerWork.player.properties.player.gasWeaponPowerTimer = Q12(0.0f);
                    D_800C3963                                                              -= 2;
                }
            }
            break;

        case 4:
            if (g_SavegamePtr->mapIdx != D_800C3960)
            {
                D_800C3960 = g_SavegamePtr->mapIdx;
                D_800C3963 = 0;
                D_800C3962 = 0;
                D_800C3961 = 32;

                temp_v1 = arg0;
                if (temp_v1 != (1 << 1))
                {
                    D_800C3963 = 1 << 0;
                }
                else
                {
                    D_800C3963 = temp_v1;
                }

                if (arg0 == (1 << 1))
                {
                    D_800C3963 |= 1 << 2;
                }
            }

            func_8008B438(arg0 != 2, (u8)D_800C3961, D_800C3962);

            if (arg0 == (1 << 1))
            {
                func_8008B40C((u8)D_800C3961, D_800C3962);
            }

            func_8008B3E4(NO_VALUE);
            break;
    }
}

void Game_TimerUpdate(void) // 0x8004C8DC
{
    #define TIME_290_HOURS        Q12((290.0f * 60.0f) * 60.0f)
    #define TIME_130_HOURS        Q12((130.0f * 60.0f) * 60.0f)
    #define TIME_290_OVERFLOW_MAX 3 // `add290Hours` has max value of 3.

    g_SavegamePtr->gameplayTimer += g_DeltaTimeRaw;
    if (g_SavegamePtr->gameplayTimer >= TIME_290_HOURS)
    {
        if (g_SavegamePtr->add290Hours < TIME_290_OVERFLOW_MAX)
        {
            g_SavegamePtr->add290Hours++;
            g_SavegamePtr->gameplayTimer += (UINT_MAX - TIME_290_HOURS) + 1; // Wrap timer to 0 using unsigned overflow.
        }
        else
        {
            g_SavegamePtr->gameplayTimer = TIME_290_HOURS - 1;
        }
    }

    if (g_SavegamePtr->add290Hours == TIME_290_OVERFLOW_MAX)
    {
        g_SavegamePtr->gameplayTimer = CLAMP(g_SavegamePtr->gameplayTimer, 1, TIME_130_HOURS);
    }

    #undef TIME_290_HOURS
    #undef TIME_130_HOURS
    #undef TIME_290_OVERFLOW_MAX
}
