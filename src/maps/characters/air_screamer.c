#include "bodyprog/bodyprog.h"
#include "bodyprog/events/npc_main.h"
#include "bodyprog/game_boot/fs_chara_anim.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player.h"
#include "main/rng.h"
#include "maps/shared.h"
#include "maps/characters/air_screamer.h"

// NOTES:
// - M0S01 includes some extra functions missing from other maps, but also removes the body of most `AirScreamer_Control_X` functions.
//  (also seems to remove the funcs that those removed control funcs would have called into)
//  See `#ifdef MAP0_S01` and `#ifndef MAP0_S01` blocks below.
//
// - M0S01 also includes an odd `Model_AnimDurationGet` func in between some of the AirScreamer funcs.
//  Seems to get used in some event code instead of being called by AirScreamer functions.
//  Might indicate a split there where M0S01 included some small .c file?

#define airScreamerProps airScreamer->properties.airScreamer

void AirScreamer_Update(s_SubCharacter* airScreamer, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords)
{
    sharedFunc_800D21E4_0_s01(anmHdr, boneCoords);
    sharedFunc_800D2200_0_s01(airScreamer);

    AirScreamer_Init(airScreamer);

    sharedFunc_800D2274_0_s01(airScreamer);
    sharedFunc_800D2390_0_s01(airScreamer);
    AirScreamer_ControlUpdate(airScreamer);
    sharedFunc_800D62D8_0_s01(airScreamer);
    sharedFunc_800D7AB0_0_s01(airScreamer);
    sharedFunc_800D7EBC_0_s01(airScreamer);
    sharedFunc_800D81B0_0_s01(airScreamer);
}

bool sharedFunc_800D21E4_0_s01(s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords)
{
    sharedData_800E21D0_0_s01.anmHdr_4 = anmHdr;
    sharedData_800E21D0_0_s01.coords_8 = boneCoords;
    sharedData_800E21D0_0_s01.flags  = false;
    return true;
}

void sharedFunc_800D2200_0_s01(s_SubCharacter* airScreamer)
{
    s32 count;

    count = func_800808AC(airScreamer->position.vx, airScreamer->position.vz);
    if (count == 7)
    {
        return;
    }

    if (count < 8)
    {
        if (count != 0)
        {
            airScreamerProps.groundHeight = Collision_GroundHeightGet(airScreamer->position.vx, airScreamer->position.vz);
        }
    }
    else if (count != 12)
    {
        airScreamerProps.groundHeight = Collision_GroundHeightGet(airScreamer->position.vx, airScreamer->position.vz);
    }
}

bool sharedFunc_800D2274_0_s01(s_SubCharacter* airScreamer)
{
    s32              i;
    u32              flags;
    s_func_800D2E04* data;

    flags = airScreamerProps.flags;

    if (!(flags & AirScreamerFlag_16))
    {
        if (flags & (AirScreamerFlag_3 | AirScreamerFlag_6))
        {
            flags &= ~(AirScreamerFlag_3 | AirScreamerFlag_6);
            if (airScreamer->health <= Q12(0.0f))
            {
                flags |= AirScreamerFlag_6;
            }
            else
            {
                flags |= AirScreamerFlag_3;
            }
        }

        if (flags & AirScreamerFlag_28)
        {
            flags &= ~(AirScreamerFlag_2 |
                       AirScreamerFlag_3 |
                       AirScreamerFlag_4 |
                       AirScreamerFlag_5 |
                       AirScreamerFlag_6);
        }
        else if (flags & AirScreamerFlag_6)
        {
            flags &= ~(AirScreamerFlag_2 |
                       AirScreamerFlag_3 |
                       AirScreamerFlag_4 |
                       AirScreamerFlag_5);
            Sd_SfxStop(sharedData_800CAA98_0_s01.sfxVolumes_CE8[2].id);
        }

        // Run through flags defining which SFX to play.
        data = &sharedData_800CAA98_0_s01;
        for (i = 0; i < ARRAY_SIZE(data->sfxVolumes_CE8); i++)
        {
            if (flags & (1 << i))
            {
                Sfx_WithFlagsPlay(data->sfxVolumes_CE8[i].id, &airScreamer->position, data->sfxVolumes_CE8[i].volume_2.val8, SfxFlag_None);
            }
        }
    }

    return true;
}

void sharedFunc_800D2364_0_s01(s_SubCharacter* airScreamer)
{
    Sfx_WithFlagsPlay(Sfx_Unk1590, &airScreamer->position, Q8(0.5f), SfxFlag_None);
}

#define BIT_MASK(bit) \
    (1 << (bit))

#define BIT_GET(var, val, bit) \
    ((var) = ((u32)(val) >> (bit)), (var) &= 0x1)

#define BIT_SET(val, bit) \
    (((val) |  BIT_MASK(bit)))

#define BIT_CLEAR(val, bit) \
    (((val) & ~BIT_MASK(bit)))

static inline u32 BitAssign(u32 val, u32 bit, bool cond)
{
    u32 newVal;

    if (cond)
    {
        newVal = BIT_SET(val, bit);
    }
    else
    {
        newVal = BIT_CLEAR(val, bit);
    }

    return newVal;
}

static inline u32 MaskAssign(u32 val, u32 mask, bool cond)
{
    u32 newVal;

    if (cond)
    {
        newVal = val | mask;
    }
    else
    {
        newVal = val & ~mask;
    }

    return newVal;
}

bool sharedFunc_800D2390_0_s01(s_SubCharacter* airScreamer)
{
    s32    bit0;
    s32    bit4;
    s32    bit5;
    s32    bit7;
    s32    sp20;
    s32    sp24;
    s32    sp28;
    bool   cond0;
    s32    bit14;
    s32    sp34;
    s32    bit15;
    s32    bit27;
    s32    cond1;
    s32    bit3;
    q19_12 posX;
    q19_12 posZ;
    s32    temp_s0_2;
    bool   cond2;
    s32    bit22;
    s32    flags;
    s32    bit6;
    s32    bit2;
    s32    bit1;
    s32    temp_s6;
    s32    temp_v0;
    s32    temp_v0_2;
    s32    var_s3;
    s32    var_s4;
    s32    var_s7;
    s32    var_v0;
    s32    var_v0_20;
    s32    var_v1_3;
    s32    bit21;
    s32    var_a0;
    s32    new_var2;
    s32    new_var4;

    sp28 = 0;

    posX  = airScreamer->position.vx;
    flags = airScreamerProps.flags;
    posZ  = airScreamer->position.vz;

    temp_v0 = sharedData_800CAA98_0_s01.field_380[39][airScreamer->model.anim.keyframeIdx];

    BIT_GET(bit1, temp_v0, 1);
    BIT_GET(bit2, temp_v0, 2);
    BIT_GET(bit3, temp_v0, 3);
    BIT_GET(bit4, temp_v0, 4);
    BIT_GET(bit5, temp_v0, 5);
    BIT_GET(bit6, temp_v0, 6);
    BIT_GET(bit7, temp_v0, 7);
    BIT_GET(bit0, temp_v0, 0);

    temp_s0_2 = func_800808AC(posX, posZ);

    cond2 = temp_s0_2 == 7;
    if (airScreamer->position.vy >= (Collision_GroundHeightGet(posX, posZ) - Q12(0.15f)))
    {
        sp28 = !cond2;
    }

    cond0 = false;
    if (temp_s0_2 == 12 || temp_s0_2 == 0)
    {
        cond0 = true;
    }

    BIT_GET(bit27, flags, 27);

    cond1 = false;
    if (flags & PlayerFlag_SfxActive || cond0)
    {
        cond1 = true;
    }

    BIT_GET(bit21, flags, 21);
    BIT_GET(bit22, flags, 22);
    BIT_GET(bit14, flags, 14);
    BIT_GET(bit15, flags, 15);

    temp_v0_2 = flags & (PlayerFlag_Unk6 | PlayerFlag_Unk28);
    sp34      = temp_v0_2;
    sp34      = temp_v0_2 != 0;
    sp20      = 0;

    temp_s6 = (bit0 == 0) & (bit1 == 0);

    if (bit6 == 0)
    {
        sp20 = bit1 & 0x1;
    }

    var_s7 = 0;

    sp24 = bit1 & bit6;

    if (bit2 == 0)
    {
        var_s7 = temp_s6 & 0x1;
    }

    var_s3 = 0;

    if (bit2 != 0)
    {
        var_s3 = temp_s6 & 0x1;
    }

    var_s4 = 0;

    if (airScreamer->position.vy < (Collision_GroundHeightGet(airScreamer->position.vx, airScreamer->position.vz) - Q12(0.1f)))
    {
        switch (func_800808AC(airScreamer->position.vx, airScreamer->position.vz))
        {
            case 7:
            case 0:
            case 12:
                break;

            default:
                if (!bit21)
                {
                    var_v0 = var_s7 & 0x1;
                }
                else
                {
                    var_v0 = 0;
                }

                var_s4 = temp_s6 & var_v0;

                if (!bit22)
                {
                    var_s4 = temp_s6 & (var_v0 | var_s3);
                }
                break;
        }
    }

    // TODO: Weird `e_AirScreamerFlags` operations.

    flags = BitAssign(flags, 13, bit0 & sp28);
    flags = BitAssign(flags, 17, bit0);
    flags = BitAssign(flags, 18, !bit1);
    flags = BitAssign(flags, 19, sp20);
    flags = BitAssign(flags, 20, sp24);
    flags = BitAssign(flags, 21, var_s7);
    flags = BitAssign(flags, 22, var_s3);
    flags = BitAssign(flags, 23, !bit3 ? (temp_s6 & 1) : 0);
    flags = BitAssign(flags, 24, (bit3 == 1) ? (temp_s6 & 1) : 0);
    flags = BitAssign(flags, 25, bit4);
    flags = BitAssign(flags, 26, bit5);
    flags = BitAssign(flags, 27, bit7);
    flags = BIT_CLEAR(BitAssign(flags, 15, bit14), 14);
    var_a0 = BitAssign(flags, 28, sp34);

    new_var4 = AirScreamerFlag_7;

    if (bit27 == 0)
    {
        var_v1_3 = bit7;
        var_v1_3 = var_v1_3 & sp20;
    }
    else
    {
        var_v1_3 = 0;
    }

    var_v1_3 = MaskAssign(var_a0, new_var4, var_v1_3);

    new_var2 = AirScreamerFlag_8;
    flags  = MaskAssign(var_v1_3, new_var2, (bit27 == 0) ? ((bit7 & 0x1) & sp24) : 0);
    var_v1_3 = BitAssign(flags, 9, var_s4);

    var_a0 = AirScreamerFlag_10;
    if (!cond0 && !cond1)
    {
        var_v0_20 = bit0;
        var_v0_20 = var_v0_20 & sp28;
    }
    else
    {
        var_v0_20 = 0;
    }

    flags = BIT_CLEAR(MaskAssign(var_v1_3, var_a0, var_v0_20), 4);
    flags &= ~AirScreamerFlag_5;
    flags &= ~AirScreamerFlag_6;
    flags = BIT_CLEAR(BitAssign(flags, 3, !bit15 ? (bit14 & (1 << 0)) : 0), 2);
    flags &= ~AirScreamerFlag_0;
    flags &= ~AirScreamerFlag_1;

    airScreamerProps.flags = flags;
    return true;
}

bool AirScreamer_Init(s_SubCharacter* airScreamer)
{
    #define BASE_HEALTH                 Q12(380.0f)
    #define AIR_SCREAMER_RAND_MAX       100
    #define NIGHT_FLUTTER_RAND_MAX      300
    #define RAND_MAX_HEALTH_BONUS_EASY -100
    #define RAND_MAX_HEALTH_BONUS_HARD  100

    // Return early if already initialized.
    if (airScreamer->model.controlState != AirScreamerControl_None)
    {
        return false;
    }

    // Set base health with random variation, accounting for differences between Air Screamer and Night Flutter.
    if (airScreamer->model.charaId == Chara_NightFlutter)
    {
        airScreamer->health = BASE_HEALTH + (Rng_RandQ12() * NIGHT_FLUTTER_RAND_MAX);
    }
    else
    {
        airScreamer->health = BASE_HEALTH + (Rng_RandQ12() * AIR_SCREAMER_RAND_MAX);
    }

    // Set health according to game difficulty.
    switch (g_SavegamePtr->gameDifficulty)
    {
        case GameDifficulty_Easy:
            airScreamer->health += Rng_RandQ12() * RAND_MAX_HEALTH_BONUS_EASY;
            break;

        case GameDifficulty_Normal:
            break;

        case GameDifficulty_Hard:
            airScreamer->health += Rng_RandQ12() * RAND_MAX_HEALTH_BONUS_HARD;
            break;
    }

    sharedFunc_800D2BF4_0_s01(airScreamer);
    AirScreamer_Control_0(airScreamer);
    sharedFunc_800D2390_0_s01(airScreamer);
    sharedFunc_800D2390_0_s01(airScreamer);
    sharedFunc_800D2B00_0_s01(airScreamer);
    sharedFunc_800D2B10_0_s01(airScreamer);
    sharedFunc_800D2B28_0_s01(airScreamer);
    sharedFunc_800D2B4C_0_s01(airScreamer);
    AirScreamer_GroundWarp(airScreamer);
    sharedFunc_800D2BE4_0_s01(airScreamer);

    return true;

    #undef BASE_HEALTH
    #undef AIR_SCREAMER_RAND_MAX
    #undef NIGHT_FLUTTER_RAND_MAX
    #undef RAND_MAX_HEALTH_BONUS_EASY
    #undef RAND_MAX_HEALTH_BONUS_HARD
}

void sharedFunc_800D2B00_0_s01(s_SubCharacter* airScreamer)
{
    airScreamer->rotation.vx  = Q12_ANGLE(0.0f);
    airScreamer->rotation.vz  = Q12_ANGLE(0.0f);
    airScreamer->angleToTarget        = Q12_ANGLE(0.0f);
}

void sharedFunc_800D2B10_0_s01(s_SubCharacter* airScreamer)
{
    airScreamer->damage.position.vx = Q12(0.0f);
    airScreamer->damage.position.vy = Q12(0.0f);
    airScreamer->damage.position.vz = Q12(0.0f);
    airScreamer->damage.amount      = Q12(0.0f);
    airScreamerProps.field_F4            = 0;
}

void sharedFunc_800D2B28_0_s01(s_SubCharacter* airScreamer)
{
    airScreamer->moveSpeed           = Q12(0.0f);
    airScreamer->fallSpeed           = Q12(0.0f);
    airScreamer->rotationSpeed.vx    = Q12_ANGLE(0.0f);
    airScreamer->rotationSpeed.vy    = Q12_ANGLE(0.0f);
    airScreamer->rotationSpeed.vz    = Q12_ANGLE(0.0f);
    airScreamer->angleToTargetRotationSpeed = Q12_ANGLE(0.0f);
    airScreamer->headingAngle        = airScreamer->rotation.vy;
}

void sharedFunc_800D2B4C_0_s01(s_SubCharacter* airScreamer)
{
    q19_12         deltaTime;
    s_AnmHeader*   anmHdr;
    GsCOORDINATE2* coords;

    deltaTime = g_DeltaTime;

    anmHdr = sharedData_800E21D0_0_s01.anmHdr_4;
    coords = sharedData_800E21D0_0_s01.coords_8;

    g_DeltaTime = Q12(0.0f);
    func_80044950(airScreamer, anmHdr, coords);
    g_DeltaTime = deltaTime;

    sharedFunc_800D7560_0_s01(airScreamer);
    sharedFunc_800D82B8_0_s01(airScreamer);
}

void AirScreamer_GroundWarp(s_SubCharacter* airScreamer)
{
    airScreamer->position.vy = Collision_GroundHeightGet(airScreamer->position.vx, airScreamer->position.vz);
}

void sharedFunc_800D2BE4_0_s01(s_SubCharacter* airScreamer)
{
    airScreamerProps.field_EC = 0;
    airScreamerProps.field_F0 = 0;
    airScreamerProps.field_F2 = 0;
}

void sharedFunc_800D2BF4_0_s01(s_SubCharacter* airScreamer)
{
    ModelAnim_AnimInfoSet(&airScreamer->model.anim, &sharedData_800CAA98_0_s01.animInfo_0);
}

s32 AirScreamer_DamageTake(s_SubCharacter* airScreamer, q19_12 mult)
{
    q19_12 damage;
    q19_12 injuredDamage;
    s32    animStatus;
    s32    attack;
    u32    damageType;
    u8     temp_a1;
    q19_12 angle;

    damage     = airScreamer->damage.amount;
    animStatus = airScreamer->model.anim.status;
    attack     = airScreamer->attackReceived;
    damageType = AirScreamerHit_None;

    if (airScreamerProps.field_E8_0 == 3)
    {
        switch (attack)
        {
            case WEAPON_ATTACK(EquippedWeaponId_SteelPipe, AttackInputType_Tap):
            case WEAPON_ATTACK(EquippedWeaponId_Hammer,    AttackInputType_Tap):
                damage += damage / 2;
                break;

            case WEAPON_ATTACK(EquippedWeaponId_HuntingRifle, AttackInputType_Tap):
            case WEAPON_ATTACK(EquippedWeaponId_Shotgun,      AttackInputType_Tap):
                damage /= 2;
                break;

            default:
                break;
        }
    }

    if (airScreamer->health > Q12(0.0f))
    {
        if (animStatus == ANIM_STATUS(AirScreamerAnim_HoverInjured, true))
        {
            injuredDamage = Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 10.0f);
            if (injuredDamage < airScreamer->health)
            {
                airScreamer->health -= injuredDamage;
            }
            else
            {
                airScreamer->health = Q12(0.0f);
            }
        }

        airScreamer->damage.amount = Q12(0.0f);

        angle  = mult; // @hack
        damage = Q12_MULT_PRECISE(damage, angle);
        if (damage < airScreamer->health)
        {
            airScreamer->health -= damage;
        }
        else
        {
            airScreamer->health = Q12(0.0f);
        }
    }

    if (damage > Q12(0.0f) || airScreamer->health <= Q12(0.0f))
    {
        temp_a1 = D_800AD4C8[attack].field_10;
        angle   = Q12_ANGLE_NORM_S(g_SysWork.playerWork.player.rotation.vy - airScreamer->rotation.vy);

#ifdef MAP0_S01
        if (airScreamer->health <= Q12(0.0f))
#else
        if (airScreamer->health <= Q12(120.0f))
#endif
        {
            if (damage > Q12(5.0f))
            {
                damageType = AirScreamerHit_Kill;
            }
            else
            {
                damageType = AirScreamerHit_Stun;
            }
        }
        else if (airScreamerProps.field_E8_0 != 3 && temp_a1 == 2 && Math_CheckSignedRange(angle, Q12_ANGLE(120.0f)))
        {
            damageType = AirScreamerHit_Rear;
        }
        else if (airScreamerProps.field_E8_0 == 3)
        {
            if (damage > Q12(45.0f) || temp_a1 == 1)
            {
                damageType = AirScreamerHit_Recoil;
            }
        }
        else
        {
            if (damage > Q12(20.0f) || temp_a1 == 1)
            {
                damageType = AirScreamerHit_Recoil;
            }
        }
    }

    return damageType;
}

bool sharedFunc_800D2E04_0_s01(s_SubCharacter* airScreamer, VECTOR3* inVec, q19_12* outDist, q19_12* outAngle)
{
    u8     idxInfo;
    s32    idx;
    q19_12 angle1;
    q19_12 deltaX;
    q19_12 deltaZ;
    q19_12 dist0;
    q19_12 angle0;
    q19_12 angle;
    q19_12 dist;

    idxInfo = g_SysWork.field_2388.field_154.effectsInfo.field_0.field_0;
    idx     = (idxInfo & (1 << 1)) ? ((idxInfo & (1 << 0)) ? 2 : 3) : (sharedFunc_800D4A80_0_s01(airScreamer) == 3);

    deltaX = inVec->vx - airScreamer->position.vx;
    deltaZ = inVec->vz - airScreamer->position.vz;

    dist0  = (u16)sharedData_800CAA98_0_s01.properties_D14[idx].val16[0];
    angle0 = (u16)sharedData_800CAA98_0_s01.properties_D14[idx].val16[1];

    angle = Q12_ANGLE_NORM_S(ratan2(deltaX, deltaZ) - (airScreamer->rotation.vy + airScreamer->angleToTarget));
    dist  = SquareRoot12(Q12_SQUARE_PRECISE(deltaX) + Q12_SQUARE_PRECISE(deltaZ));

    if (outDist != NULL)
    {
        *outDist = dist;
    }
    if (outAngle != NULL)
    {
        *outAngle = angle;
    }

    angle1 = angle + angle0;
    if (((u32)angle0 * 2) >= angle1 && dist0 >= dist)
    {
        sharedData_800DE170_0_s01.vx = airScreamer->position.vx;
        sharedData_800DE170_0_s01.vy = airScreamer->position.vy + airScreamer->collision.box.top; // Head offset.
        sharedData_800DE170_0_s01.vz = airScreamer->position.vz;
        return !Ray_TraceQuery(&sharedData_800E2330_0_s01, &sharedData_800DE170_0_s01, inVec);
    }

    return false;
}

bool sharedFunc_800D2FB4_0_s01(s_SubCharacter* airScreamer, VECTOR3* playerPos, s32 playerField10C)
{
    q3_12  rotY;
    q19_12 offsetY;
    q19_12 offsetZ;
    s32    idx;
    q19_12 dist;
    q19_12 var_s1;
    u64    roughDist;

    if (airScreamerProps.flags & (AirScreamerFlag_19 | AirScreamerFlag_20))
    {
        idx = 2;
    }
    else
    {
        // TODO: Does `ANIM_STATUS_IS_ACTIVE` fit?
        if ((airScreamer->model.anim.status | 0x1) == 27 ||
            (airScreamer->model.anim.status | 0x1) == 29)
        {
            idx = 0;
        }
        else
        {
            idx = 1;
        }
    }

    var_s1  = sharedData_800CAA98_0_s01.field_D24[0][idx][0];
    dist = sharedData_800CAA98_0_s01.field_D24[0][idx][1];

    switch (g_SavegamePtr->gameDifficulty)
    {
        case GameDifficulty_Easy:
            break;

        case GameDifficulty_Normal:
            var_s1 += Q12(0.5f);
            break;

        case GameDifficulty_Hard:
            var_s1 += Q12(1.0f);
            break;
    }

    rotY    = airScreamer->rotation.vy;
    offsetY = playerPos->vx - (airScreamer->position.vx + Q12_MULT_PRECISE(dist, Math_Sin(rotY)));
    offsetZ = playerPos->vz - (airScreamer->position.vz + Q12_MULT_PRECISE(dist, Math_Cos(rotY)));

    roughDist = Q12_SQUARE_PRECISE(offsetY) +
                Q12_SQUARE_PRECISE(offsetZ);
    if (roughDist > Q12_SQUARE_PRECISE(Q12_MULT_PRECISE(var_s1, playerField10C)))
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool sharedFunc_800D31D0_0_s01(s_SubCharacter* airScreamer, const VECTOR3* pos, s32 arg2)
{
    q3_12  offsetHeadingAngle;
    q19_12 offsetDist;
    q19_12 offsetX;
    q19_12 offsetZ;
    q19_12 var_s1;
    q51_12 roughDist;
    q52_12 distMax;
    s32    idx;

    idx        = sharedData_800CAA98_0_s01.field_D3C[airScreamerProps.field_E8_4][airScreamerProps.field_E8_8];
    var_s1     = sharedData_800CAA98_0_s01.field_D24[1][idx][0];
    offsetDist = sharedData_800CAA98_0_s01.field_D24[1][idx][1];

    switch (g_SavegamePtr->gameDifficulty)
    {
        case GameDifficulty_Easy:
            break;

        case GameDifficulty_Normal:
            var_s1 += Q12(0.5f);
            break;

        case GameDifficulty_Hard:
            var_s1 += Q12(1.0f);
            break;
    }

    if (airScreamer->model.controlState == AirScreamerControl_StandIdle ||
        airScreamer->model.controlState == AirScreamerControl_9)
    {
        var_s1 += Q12(1.0f);
    }

    offsetHeadingAngle = airScreamer->rotation.vy;
    offsetX            = pos->vx - (airScreamer->position.vx + Q12_MULT_PRECISE(offsetDist, Math_Sin(offsetHeadingAngle)));
    offsetZ            = pos->vz - (airScreamer->position.vz + Q12_MULT_PRECISE(offsetDist, Math_Cos(offsetHeadingAngle)));

    roughDist = Q12_SQUARE_PRECISE(offsetX) +
                Q12_SQUARE_PRECISE(offsetZ);
    distMax   = Q12_SQUARE_PRECISE(var_s1);
    distMax   = Q12_MULT_PRECISE(distMax, arg2);
    if (roughDist > distMax)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool sharedFunc_800D3430_0_s01(s_SubCharacter* airScreamer, q19_12* dist, q19_12* angle)
{
    bool cond;

    g_AirScreamer_HeadTargetPosition.vx = g_SysWork.playerWork.player.position.vx;
    g_AirScreamer_HeadTargetPosition.vy = g_SysWork.playerWork.player.position.vy + g_SysWork.playerWork.player.collision.box.top;
    g_AirScreamer_HeadTargetPosition.vz = g_SysWork.playerWork.player.position.vz;

    cond = sharedFunc_800D2E04_0_s01(airScreamer, &g_AirScreamer_HeadTargetPosition, dist, angle);
    if (cond)
    {
        airScreamerProps.headTargetPosition = g_AirScreamer_HeadTargetPosition;
    }

    if (dist != NULL)
    {
        *dist = Math_Distance2dGet(&airScreamer->position, &airScreamerProps.headTargetPosition);
    }

    if (angle != NULL)
    {
        *angle = func_80080478(&airScreamer->position, &airScreamerProps.headTargetPosition);
    }

    return cond;
}

bool sharedFunc_800D3508_0_s01(s_SubCharacter* airScreamer, q19_12* dist)
{
    q19_12 angleToPlayer;
    q19_12 angleToPlayerCpy;
    q19_12 offsetDist;
    q19_12 offsetX;
    u32    offsetZ;
    bool   cond;
    // TODO: Fix messy local var reuse.

    #define playerProps g_SysWork.playerWork.player.properties.player

    offsetDist = playerProps.field_10C << 8;
    if (dist != NULL)
    {
        *dist = offsetDist;
    }

    cond = sharedFunc_800D2FB4_0_s01(airScreamer, &g_SysWork.playerWork.player.position, offsetDist);
    if (cond)
    {
        angleToPlayer    = Math_AngleBetweenPositionsGet(airScreamer->position, g_SysWork.playerWork.player.position);
        angleToPlayerCpy = angleToPlayer;

        offsetZ = g_SysWork.field_2388.field_154.effectsInfo.field_0.field_00[0];
        if (!(offsetZ & 0x2))
        {
            // @hack Permuter find.
            offsetZ = 0;
            while (offsetZ) {};

            do
            {
                sharedFunc_800D4A80_0_s01(airScreamer);
            }
            while (false);
        }

        angleToPlayer = Q12_MULT_PRECISE(offsetDist, Math_Sin(angleToPlayerCpy));
        offsetX = angleToPlayer;
        offsetZ = Q12_MULT_PRECISE(offsetDist, Math_Cos(angleToPlayerCpy));

        airScreamerProps.headTargetPosition.vx = airScreamer->position.vx + offsetX;
        airScreamerProps.headTargetPosition.vy = g_SysWork.playerWork.player.position.vy + g_SysWork.playerWork.player.collision.box.offsetY;
        airScreamerProps.headTargetPosition.vz = airScreamer->position.vz + offsetZ;
    }

    return cond;

    #undef playerProps
}

bool sharedFunc_800D3630_0_s01(s_SubCharacter* airScreamer, q19_12* dist)
{
    bool   cond;
    q19_12 angleToPlayer;
    q19_12 offsetDist;
    q19_12 offsetX;
    q19_12 offsetZ;

    offsetDist = sharedFunc_800D3814_0_s01(airScreamer);
    if (dist != NULL)
    {
        *dist = offsetDist;
    }

    // Set target position for pecking?
    cond = sharedFunc_800D31D0_0_s01(airScreamer, &g_SysWork.playerWork.player.position, offsetDist);
    if (cond)
    {
        // Compute extra offset distance.
        offsetDist = MAX(offsetDist, Q12(1.5f));

        // Compute extra offset.
        angleToPlayer = Math_AngleBetweenPositionsGet(airScreamer->position, g_SysWork.playerWork.player.position);
        offsetX = Q12_MULT_PRECISE(offsetDist, Math_Sin(angleToPlayer));
        offsetZ = Q12_MULT_PRECISE(offsetDist, Math_Cos(angleToPlayer));

        // Set target position slightly ahead of Air Screamer.
        airScreamerProps.headTargetPosition.vx = airScreamer->position.vx + offsetX;
        airScreamerProps.headTargetPosition.vy = g_SysWork.playerWork.player.position.vy + g_SysWork.playerWork.player.collision.box.offsetY;
        airScreamerProps.headTargetPosition.vz = airScreamer->position.vz + offsetZ;
    }

    return cond;
}

s32 sharedFunc_800D3758_0_s01(s_SubCharacter* airScreamer, q19_12* outDist, q19_12* outAngle, q19_12 dist, s32* arg4)
{
    s32  cond1;
    s32  cond0;
    bool cond;
    s32  flags;

    cond = sharedFunc_800D3430_0_s01(airScreamer, outDist, outAngle);
    if (cond)
    {
        sharedData_800DE190_0_s01 = airScreamerProps.headTargetPosition;
    }

    cond0 = sharedFunc_800D3630_0_s01(airScreamer, dist);
    cond1 = sharedFunc_800D3508_0_s01(airScreamer, arg4);

    if (cond)
    {
        airScreamerProps.headTargetPosition = sharedData_800DE190_0_s01;
    }

    flags = (cond != false) * 4;
    if (cond0)
    {
        flags |= 1 << 1;
    }

    return flags | (cond1 != false);
}

s32 sharedFunc_800D3814_0_s01(s_SubCharacter* airScreamer)
{
    s32 headingAngleDelta;
    s32 invDist;
    s32 dist;
    s32 distProp8;
    s32 headingAngleTo;
    s32 headingAngleFrom;

    headingAngleTo = func_80080478(&g_SysWork.playerWork.player.position, &airScreamer->position);
    headingAngleFrom = g_SysWork.playerWork.player.rotation.vy;

    invDist = Q12(8.0f) - Math_Distance2dGet(&g_SysWork.playerWork.player.position, &airScreamer->position);
    if (invDist < Q12(0.0f))
    {
        invDist = Q12(0.0f);
    }
    else
    {
        invDist = invDist >> 3;
    }

    headingAngleDelta = ABS_32(Q12_ANGLE_NORM_S(headingAngleTo - headingAngleFrom));
    if (invDist >= headingAngleDelta)
    {
        dist = (invDist - headingAngleDelta) * 4;
    }
    else
    {
        dist = Q12(0.0f);
    }

    if (g_Player_IsAiming)
    {
        distProp8 = func_8007FD2C();
        if (distProp8 > Q12(2.0f))
        {
            dist += Q12(2.0f);
        }
        else
        {
            dist += distProp8;
        }

        if (airScreamer == &g_SysWork.npcs[g_SysWork.targetNpcIdx])
        {
            dist += Q12(1.0f);
        }
    }
    else
    {
        dist += Q12(0.5f);
    }

    return dist;
}

bool AirScreamer_ControlUpdate(s_SubCharacter* airScreamer)
{
    q20_12 someTime;
    q19_12 deltaTime;
    void (*controlFunc)(s_SubCharacter*);

    deltaTime = g_DeltaTime;
    if (deltaTime < Q12(0.0f))
    {
        deltaTime = Q12(0.0f);
    }

    someTime = airScreamerProps.timer_120;
    if (someTime < deltaTime)
    {
        airScreamerProps.timer_120 = Q12(0.0f);
    }
    else
    {
        airScreamerProps.timer_120 = someTime - deltaTime;
    }

    sharedData_800E21D0_0_s01.field_14C.flags = sharedFunc_800D3758_0_s01(airScreamer,
                                                                          &sharedData_800E21D0_0_s01.distance_150,
                                                                          &sharedData_800E21D0_0_s01.angle_154,
                                                                          &sharedData_800E21D0_0_s01.field_158,
                                                                          &sharedData_800E21D0_0_s01.field_15C);

    if (airScreamerProps.field_E8_0 == 3)
    {
        airScreamerProps.field_E8_8 = 0;
    }

    // Handle control state.
    controlFunc = g_AirScreamer_ControlFuncs[airScreamer->model.controlState];
    if (controlFunc)
    {
        controlFunc(airScreamer);
    }

    return true;
}

#ifdef MAP0_S01
void func_800D39F4(s_SubCharacter* airScreamer) // 0x800D39F4
{
    q19_12 animTime;

    // TODO: `Chara_AnimSet` doesn't match?
    airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_HoverVariable, true);
    animTime = func_80044918(&airScreamer->model.anim)->startKeyframeIdx;
    airScreamer->model.stateStep = 7;
    airScreamer->model.anim.keyframeIdx = animTime;
    airScreamer->model.anim.time = FP_TO(animTime, Q12_SHIFT);
}

void func_800D3A3C(s_SubCharacter* airScreamer) // 0x800D3A3C
{
    q19_12 animTime;
    s32    animMetadataIdx;

    animMetadataIdx = g_CharaAnimDataIdxs[airScreamer->model.charaId];
    AirScreamer_Update(airScreamer, (&g_CharaModelAnimsData[animMetadataIdx])->activeAnmHdr, (&g_CharaModelAnimsData[animMetadataIdx])->boneCoords);

    airScreamer->model.anim.status      = ANIM_STATUS(AirScreamerAnim_Glide, true);
    animTime                            = func_80044918(&airScreamer->model.anim)->startKeyframeIdx;
    airScreamer->model.stateStep        = 3;
    airScreamer->model.anim.keyframeIdx = animTime;
    airScreamer->model.anim.time        = FP_TO(animTime, Q12_SHIFT);
}

void func_800D3AC0(s_SubCharacter* airScreamer) // 0x800D3AC0
{
    // Handle state step.
    switch(airScreamer->model.stateStep)
    {
        case 1:
        case 3:
        case 5:
            airScreamer->model.stateStep++;
            break;

        default:
            break;
    }
}
#endif

void AirScreamer_Control_0(s_SubCharacter* airScreamer)
{
    s32  keyframeIdx;
    s32  var0;
    bool var1;
    s32  var2;
    u32  controlState;
    u32  stateStep;
    s32  field40;
    u32  animStatus;

    field40 = airScreamer->field_40;
    switch (field40 % 0x3)
    {
        case 0:
        default:
            var2 = 0;
            break;

        case 1:
            var2 = 1;
            break;

        case 2:
            var2 = 2;
            break;
    }

    switch(field40 - (field40 / 2) * 2)
    {
        case 0:
        default:
            var1 = true;
            break;

        case 1:
            var1 = false;
            break;
    }

    stateStep = 0;
    switch (airScreamer->model.stateStep)
    {
#ifdef MAP0_S01
        case 12:
        default:
            var0 = 4;
            var1 = true;
            var2 = 3;
            animStatus = NO_VALUE;
            controlState = AirScreamerControl_1;
            break;

        case 13:
            animStatus = ANIM_STATUS(AirScreamerAnim_HoverVariable, true);

            if (g_SavegamePtr->mapIdx == MapIdx_MAP0_S01)
            {
                var0 = 2;
                var1 = true;
                var2 = 2;
                animStatus = ANIM_STATUS(AirScreamerAnim_HoverBiteAttack, true);
                controlState = AirScreamerControl_46;
            }
            else
            {
                var0 = 2;

                if (airScreamer->model.anim.status == ANIM_STATUS(AirScreamerAnim_HoverVariable, true))
                {
                    animStatus = NO_VALUE;
                }

                controlState = AirScreamerControl_18;
            }
            break;
#else
        default:
            var0 = 1;
            animStatus   = ANIM_STATUS(AirScreamerAnim_StandIdle, true);
            controlState = AirScreamerControl_StandIdle;
            break;

        case 0:
        case 2:
            var0 = 1;
            animStatus   = ANIM_STATUS(AirScreamerAnim_WalkForward, true);
            controlState = AirScreamerControl_5;
            break;

        case 3:
            var0 = 1;
            animStatus   = ANIM_STATUS(AirScreamerAnim_HoverVariable, true);
            controlState = AirScreamerControl_19;
            break;

        case 4:
            var0 = 0;
            animStatus   = ANIM_STATUS(AirScreamerAnim_WalkForward, true);
            controlState = AirScreamerControl_4;
            stateStep    = 64;
            break;

        case 5:
            var0 = 0;
            animStatus = ANIM_STATUS(AirScreamerAnim_HoverVariable, true);
            controlState = AirScreamerControl_18;
            stateStep = 64;
            break;

        case 6:
            var0 = 0;
            animStatus = ANIM_STATUS(AirScreamerAnim_Glide, true);
            var2 = 2;
            controlState = AirScreamerControl_Glide;
            stateStep = 64;
            break;

        case 8:
            var0 = 1;
            animStatus = ANIM_STATUS(AirScreamerAnim_StandBiteAttackEnd, true);
            var2 = 4;
            controlState = AirScreamerControl_9;
            break;

        case 9:
            var0 = 3;
            animStatus = ANIM_STATUS(AirScreamerAnim_HoverVariable, true);
            controlState = AirScreamerControl_19;
            var2 = 2;
            airScreamer->health *= 4;
            break;

        case 11:
            var0 = 0;
            animStatus = ANIM_STATUS(AirScreamerAnim_Glide, true);
            controlState = AirScreamerControl_Glide;
            stateStep = 65;
            var2 = 2;
            break;

        case 7:
            var0 = 0;
            animStatus = ANIM_STATUS(AirScreamerAnim_Glide, true);
            controlState = AirScreamerControl_Glide;
            stateStep = 66;
            var2 = 2;
            break;

        case 10:
            var0 = 0;
            animStatus = ANIM_STATUS(AirScreamerAnim_Glide, true);
            controlState = AirScreamerControl_Glide;
            stateStep = 67;
            var2 = 2;
            airScreamer->health -= airScreamer->health >> 2; // `/ 4`.
            break;
#endif
    }

    airScreamer->model.controlState = controlState;
    airScreamer->model.stateStep = stateStep;
    airScreamerProps.flags = AirScreamerFlag_None;
    airScreamerProps.field_E8_0 = var0;
    airScreamerProps.field_E8_4 = var1;
    airScreamerProps.field_E8_8 = var2;

    if (controlState == AirScreamerControl_1)
    {
        airScreamerProps.flags = AirScreamerFlag_16;
    }

    if (animStatus != NO_VALUE && animStatus != airScreamer->model.anim.status)
    {
        airScreamer->model.anim.status      = animStatus;
        keyframeIdx                               = func_80044918(&airScreamer->model.anim)->startKeyframeIdx;
        airScreamer->model.anim.alpha       = Q12(0.0f);
        airScreamer->model.anim.keyframeIdx = keyframeIdx;
        airScreamer->model.anim.time        = FP_TO(keyframeIdx, Q12_SHIFT);
    }
}

void AirScreamer_Control_1(s_SubCharacter* airScreamer)
{
    u32  keyframeIdx;
    bool setAnim;

    sharedData_800E21D0_0_s01.flags |= 1 << 31;
    setAnim                            = false;

    // Handle state step.
    switch (airScreamer->model.stateStep)
    {
        case 0:
            setAnim                              = true;
            airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_StandIdle, true);
            airScreamer->model.stateStep   = 1;

        case 1:
            airScreamerProps.flags |= AirScreamerFlag_16;
            break;

        case 2:
            setAnim                              = true;
            airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_StandIdleToGlide, true);
            airScreamer->model.stateStep   = 3;

        case 3:
            airScreamerProps.flags |= AirScreamerFlag_16;
            break;

        case 4:
            setAnim                              = true;
            airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_GlideToHover, true);
            airScreamer->model.stateStep   = 5;

        case 5:
            airScreamerProps.flags &= ~AirScreamerFlag_16;
            break;

        case 6:
            if (airScreamer->model.anim.status == ANIM_STATUS(AirScreamerAnim_HoverVariable, true))
            {
                airScreamer->model.stateStep = 7;
            }
            break;

        case 7:
            airScreamer->model.controlState = AirScreamerControl_None;
            airScreamer->model.stateStep    = 13;
            AirScreamer_Control_0(airScreamer);
            break;
    }

    AirScreamer_DamageTake(airScreamer, Q12(0.0f));

    if (setAnim)
    {
        keyframeIdx                               = func_80044918(&airScreamer->model.anim)->startKeyframeIdx;
        airScreamer->model.anim.keyframeIdx = keyframeIdx;
        airScreamer->model.anim.time        = Q12(keyframeIdx);
    }
}

void sharedFunc_800D3DFC_0_s01(s_SubCharacter* airScreamer)
{
    q19_12 chance;
    s32    spawnFlags; // `e_SpawnFlags`

    if (airScreamer->field_40 < 32)
    {
        spawnFlags = g_MapOverlayHdr.charaSpawnInfos[0][airScreamer->field_40].flags;
        switch (spawnFlags)
        {
            case SpawnFlag_0:
            case SpawnFlag_3:
            case SpawnFlag_0 | SpawnFlag_3:
                chance = Q12(0.0f);
                break;

            case SpawnFlag_1 | SpawnFlag_3:
                chance = Q12(1.0f);
                break;

            case SpawnFlag_1:
            case SpawnFlag_0 | SpawnFlag_1:
                chance = Q12(0.3f);
                break;

            default:
                chance = Q12(0.7f);
                break;
        }

        if (Rng_RandQ12() >= chance)
        {
            Savegame_EnemyStateUpdate(airScreamer);
        }
    }
}

void AirScreamer_Control_2(s_SubCharacter* airScreamer)
{
    q19_12 damage;
    bool   cond1;
    bool   isBelowGround;
    bool   temp_s3;
    u32    animStatus;
    s32    animStatus12;
    bool   activeAnimStatus;

    animStatus   = airScreamer->model.anim.status;
    animStatus12 = ANIM_STATUS(AirScreamerAnim_DeathThroes, true);

    sharedFunc_800D5638_0_s01(airScreamer);

    activeAnimStatus = ANIM_STATUS(ANIM_STATUS_IDX_GET(animStatus), true);

    // Handle state step.
    switch ((u32)airScreamer->model.stateStep)
    {
        case 0:
            temp_s3       = sharedFunc_800D5F00_0_s01(airScreamer);
            isBelowGround = false;
            cond1         = airScreamer->position.vy >= Q12(8.0f);

            if (func_800808AC(airScreamer->position.vx, airScreamer->position.vz) == 7)
            {
                isBelowGround = airScreamer->position.vy >= Collision_GroundHeightGet(airScreamer->position.vx, airScreamer->position.vz);
            }

            if (cond1)
            {
                AirScreamer_DamageTake(airScreamer, Q12(0.0f));

                airScreamer->position.vx = g_SysWork.playerWork.player.position.vx + Q12(100.0f);
                airScreamer->position.vz = g_SysWork.playerWork.player.position.vz + Q12(100.0f);

                sharedFunc_800D3DFC_0_s01(airScreamer);
                break;
            }

            if (isBelowGround)
            {
                AirScreamer_DamageTake(airScreamer, Q12(0.0f));
                sharedFunc_800D2364_0_s01(airScreamer);

                airScreamer->position.vx = g_SysWork.playerWork.player.position.vx + Q12(100.0f);
                airScreamer->position.vz = g_SysWork.playerWork.player.position.vz + Q12(100.0f);

                sharedFunc_800D3DFC_0_s01(airScreamer);
                break;
            }

            if (airScreamer->health <= Q12(0.0f))
            {
                AirScreamer_DamageTake(airScreamer, Q12(0.0f));

                if (animStatus == ANIM_STATUS(AirScreamerAnim_Stun, true) && temp_s3 == true)
                {
                    airScreamer->health = NO_VALUE;

                    func_800622B8(3, airScreamer, ANIM_STATUS(AirScreamerAnim_StandToStun, true), 2);

                    airScreamer->model.stateStep = 1;

                    sharedFunc_800D3DFC_0_s01(airScreamer);
                    break;
                }
            }
            else
            {
                airScreamer->flags |= CharaFlag_Unk2;

                if (AirScreamer_DamageTake(airScreamer, Q12(1.0f)) == AirScreamerHit_Kill)
                {
                    if (airScreamer->health <= Q12(0.0f))
                    {
                        airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_Death, false);
                        airScreamerProps.flags          |= AirScreamerFlag_6;
                        break;
                    }

                    airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_DeathThroes, false);
                    airScreamer->model.stateStep   = 2;
                    airScreamerProps.flags        |= AirScreamerFlag_3;
                    break;
                }
#ifdef MAP0_S01
                damage = g_DeltaTime * 10;
                if (damage < airScreamer->health)
                {
                    airScreamer->health -= damage;
                }
                else if (airScreamer->health > Q12(0.0f))
                {
                    airScreamer->health = Q12(0.0f);
                }
#endif
            }
            break;

        case 1:
            AirScreamer_DamageTake(airScreamer, Q12(0.0f));
            break;

        case 2:
            AirScreamer_DamageTake(airScreamer, Q12(0.5f));

            if (activeAnimStatus != animStatus12)
            {
                airScreamer->model.stateStep = 0;
            }
            break;
    }
}

void AirScreamer_ControlStandIdle(s_SubCharacter* airScreamer)
{
#ifndef MAP0_S01
    bool cond;

    if (airScreamer->model.stateStep == 0)
    {
        sharedFunc_800DDF74_2_s00(airScreamer, Q12(0.1f), airScreamer->rotation.vy);
        airScreamer->model.stateStep = 1;
    }

    sharedFunc_800DF2D0_2_s00(airScreamer);

    switch (AirScreamer_DamageTake(airScreamer, Q12(1.0f)))
    {
        case AirScreamerHit_None:
            if (sharedData_800E21D0_0_s01.field_14C.bits.field_14C_2)
            {
                if (!airScreamerProps.field_E8_8)
                {
                    cond = sharedFunc_800D3814_0_s01(airScreamer) >= Q12(2.0f);
                }
                else
                {
                    cond = true;
                }

                if (cond)
                {
                    airScreamer->model.controlState = AirScreamerControl_10;
                    airScreamer->model.stateStep    = 0;
                    airScreamer->flags             |= CharaFlag_Hit;
                    airScreamerProps.flags         |= AirScreamerFlag_4;
                }
            }
            else
            {
                if (airScreamerProps.flags & AirScreamerFlag_31)
                {
                    airScreamer->model.controlState = AirScreamerControl_7;
                    airScreamer->model.stateStep    = 0;
                }
            }
            break;

        case AirScreamerHit_Recoil:
        case AirScreamerHit_Rear:
            airScreamer->model.controlState = AirScreamerControl_Recoil;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            break;

        case AirScreamerHit_Stun:
        case AirScreamerHit_Kill:
            airScreamer->model.controlState = AirScreamerControl_Stun;
            airScreamer->model.stateStep    = 0;

            if (airScreamer->health <= Q12(0.0f))
            {
                airScreamerProps.flags |= AirScreamerFlag_6;
            }
            else
            {
                airScreamerProps.flags |= AirScreamerFlag_3;
            }
            break;
    }
#endif
}

void AirScreamer_Control_4(s_SubCharacter* airScreamer)
{
#ifndef MAP0_S01
    s32    temp_fp;
    q19_12 distToTarget;
    s32    temp_s1_2;
    bool   temp_s5;
    bool   temp_s6;
    bool   temp_s7;
    s32    temp_v0;
    s32    var_s3;
    bool   cond;
    bool   var_v0_2;
    s32    animStatus;
    bool   new_var;

    animStatus = airScreamer->model.anim.status;
    var_s3     = 0;
    cond       = false;
    temp_fp    = sharedFunc_800D4A80_0_s01(airScreamer);

    var_v0_2 = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_1;
    new_var  = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_2;
    temp_s6  = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_0;

    temp_s5 = var_v0_2;
    temp_s7 = new_var;

    // Handle state step.
    switch (airScreamer->model.stateStep)
    {
        case 64:
            sharedFunc_800DD4A4_2_s00(airScreamer);
            airScreamer->model.stateStep = 1;
            break;

        case 0:
            sharedFunc_800DDF74_2_s00(airScreamer, Q12(4.0f), airScreamer->rotation.vy);
            sharedFunc_800DE034_2_s00(airScreamer, &airScreamerProps.targetPosition, Q12(3.0f));
            airScreamer->model.stateStep = 1;
            break;

        case 1:
            cond                             = true;
            airScreamerProps.timer_120       = Q12(8.0f);
            airScreamer->model.stateStep = 2;

        case 2:
            distToTarget = Math_Distance2dGet(&airScreamer->position, &airScreamerProps.targetPosition);
            temp_s1_2    = Q12_ANGLE_NORM_S(func_80080478(&airScreamer->position, &airScreamerProps.targetPosition) - airScreamer->rotation.vy);
            temp_v0      = sharedFunc_800DC598_2_s00(airScreamer);

            if (temp_v0 == 1)
            {
                airScreamer->model.stateStep = 3;
            }
            else if (temp_v0 == 2)
            {
                airScreamer->model.stateStep = 5;
            }
            else if ((distToTarget < Q12(1.0f) && (temp_s1_2 > -57 && temp_s1_2 < 57)) ||
                     airScreamerProps.flags < 0)
            {
                airScreamer->model.stateStep = 0;
            }
            else if (airScreamerProps.timer_120 == Q12(0.0f))
            {
                airScreamer->model.stateStep = 1;
            }
            else if (cond)
            {
                if (sharedFunc_800DC200_2_s00(airScreamer) && distToTarget > Q12(6.0f))
                {
                    airScreamer->model.stateStep = 5;
                }
                else if (sharedFunc_800DC30C_2_s00(airScreamer))
                {
                    if (distToTarget >= Q12(1.5f) || Rng_RandQ12() >= Q12(0.5f))
                    {
                        // @hack This check should be `if (diff >= Q12(-1.0f) && diff < Q12(1.0f))`,
                        // but that results in `sltiu 0x2000` instead of the `li 0x2000/sltu` needed.
                        if (Math_CheckSignedRange(airScreamerProps.targetPosition.vy - airScreamer->position.vy, Q12(1.0f)))
                        {
                            airScreamer->model.stateStep = 3;
                        }
                    }
                    else
                    {
                        airScreamer->model.stateStep = 3;
                    }
                }
            }
            break;

        case 3:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_WalkForward, true) ||
                animStatus == ANIM_STATUS(AirScreamerAnim_StandIdle, true))
            {
                var_s3                               = 1;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_StandIdleToHover, false);
                airScreamer->model.stateStep   = 4;
            }
            break;

        case 4:
            var_s3 = 1;
            break;

        case 5:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_WalkForward, true) ||
                animStatus == ANIM_STATUS(AirScreamerAnim_StandIdle, true))
            {
                var_s3                               = 2;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_StandIdleToGlide, false);
                airScreamer->model.stateStep   = 6;
            }
            break;

        case 6:
            var_s3 = 2;
            break;
    }

    sharedFunc_800DF448_2_s00(airScreamer, false);

    switch (AirScreamer_DamageTake(airScreamer, Q12(1.0f)))
    {
        case AirScreamerHit_None:
            switch (var_s3)
            {
                case 0:
                    if (temp_s5 | temp_s6 | temp_s7)
                    {
                        airScreamer->model.controlState = AirScreamerControl_6;
                        airScreamer->model.stateStep    = 0;
                    }
                    break;

                case 1:
                    if (temp_fp == 2)
                    {
                        if (temp_s5 | temp_s6 | temp_s7)
                        {
                            airScreamer->model.controlState = AirScreamerControl_20;
                        }
                        else
                        {
                            airScreamer->model.controlState = AirScreamerControl_18;
                        }

                        airScreamer->model.stateStep = 0;
                    }
                    break;

                case 2:
                    if (temp_fp == 3)
                    {
                        if (temp_s5 | temp_s6 | temp_s7)
                        {
                            airScreamer->model.controlState = AirScreamerControl_35;
                        }
                        else
                        {
                            airScreamer->model.controlState = AirScreamerControl_Glide;
                        }

                        airScreamer->model.stateStep = 0;
                    }
                    break;
            }
            break;

        case AirScreamerHit_Recoil:
        case AirScreamerHit_Rear:
            airScreamer->model.controlState = AirScreamerControl_Recoil;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            break;

        case AirScreamerHit_Stun:
        case AirScreamerHit_Kill:
            airScreamer->model.controlState = AirScreamerControl_Stun;
            airScreamer->model.stateStep    = 0;

            if (airScreamer->health <= Q12(0.0f))
            {
                airScreamerProps.flags |= AirScreamerFlag_6;
            }
            else
            {
                airScreamerProps.flags |= AirScreamerFlag_3;
            }
            break;
    }
#endif
}

void AirScreamer_Control_5(s_SubCharacter* airScreamer)
{
#ifndef MAP0_S01
    s32    animStatus;
    bool   field14C_0;
    bool   field14C_1;
    bool   field14C_2;
    bool   new_var1;
    bool   new_var2;
    bool   new_var3;
    s32    switchCond;
    bool   cond;
    s32    temp_fp;
    s32    new_var;
    q19_12 angleFieldF8;
    q19_12 distFieldF8;
    s32    temp_v0;

    animStatus = airScreamer->model.anim.status;
    switchCond = 0;
    cond       = false;

    new_var    = sharedFunc_800D4A80_0_s01(airScreamer);
    new_var1   = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_0;
    new_var2   = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_1;
    new_var3   = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_2;
    temp_fp    = new_var;
    field14C_0 = new_var1;
    field14C_1 = new_var2;
    field14C_2 = new_var3;

    // Handle state step.
    switch (airScreamer->model.stateStep)
    {
        case 0:
            sharedFunc_800DDE14_2_s00(airScreamer);
            airScreamer->model.stateStep = 1;
            break;

        case 1:
            cond                             = true;
            airScreamerProps.timer_120       = Q12(6.0f);
            airScreamer->model.stateStep = 2;

        case 2:
            distFieldF8  = Math_Distance2dGet(&airScreamer->position, &airScreamerProps.targetPosition);
            angleFieldF8 = Q12_ANGLE_NORM_S(func_80080478(&airScreamer->position, &airScreamerProps.targetPosition) - airScreamer->rotation.vy);

            temp_v0 = sharedFunc_800DC598_2_s00(airScreamer);
            if (temp_v0 == 1)
            {
                airScreamer->model.stateStep = 3;
            }
            else if (temp_v0 == 2)
            {
                airScreamer->model.stateStep = 5;
            }
            else if ((distFieldF8 < Q12(0.5f) && (angleFieldF8 >= Q12_ANGLE(-5.0f) && angleFieldF8 <= Q12_ANGLE(5.0f))) ||
                     (airScreamerProps.flags & AirScreamerFlag_31))
            {
                airScreamer->model.stateStep = 0;
            }
            else if (airScreamerProps.timer_120 == Q12(0.0f))
            {
                airScreamer->model.stateStep = 1;
            }
            else if (cond)
            {
                if (sharedFunc_800DC200_2_s00(airScreamer) && distFieldF8 > Q12(6.0f))
                {
                    airScreamer->model.stateStep = 5;
                }
                else if (sharedFunc_800DC30C_2_s00(airScreamer))
                {
                    // @hack This check should be `if (diff >= Q12(-1.0f) && diff < Q12(1.0f))`,
                    // but that results in `sltiu 0x2000` instead of the `li 0x2000/sltu` needed.
                    if (Math_CheckSignedRange(airScreamerProps.targetPosition.vy - airScreamer->position.vy, Q12(1.0f)))
                    {
                        airScreamer->model.stateStep = 3;
                    }
                }
            }
            break;

        case 3:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_WalkForward, true) ||
                animStatus == ANIM_STATUS(AirScreamerAnim_StandIdle, true))
            {
                switchCond                           = 1;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_StandIdleToHover, false);
                airScreamer->model.stateStep   = 4;
            }
            break;

        case 4:
            switchCond = 1;
            break;

        case 5:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_WalkForward, true) ||
                animStatus == ANIM_STATUS(AirScreamerAnim_StandIdle, true))
            {
                switchCond                           = 2;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_StandIdleToGlide, false);
                airScreamer->model.stateStep   = 6;
            }
            break;

        case 6:
            switchCond = 2;
            break;
    }

    sharedFunc_800DF448_2_s00(airScreamer, false);

    switch (AirScreamer_DamageTake(airScreamer, Q12(1.0f)))
    {
        case AirScreamerHit_None:
            switch (switchCond)
            {
                case 0:
                    if (field14C_1 | field14C_0 | field14C_2)
                    {
                        airScreamer->model.controlState = AirScreamerControl_6;
                        airScreamer->model.stateStep    = 0;
                    }
                    break;

                case 1:
                    if (temp_fp == 2)
                    {
                        if (field14C_1 | field14C_0 | field14C_2)
                        {
                            airScreamer->model.controlState = AirScreamerControl_20;
                        }
                        else
                        {
                            airScreamer->model.controlState = AirScreamerControl_19;
                        }
                        airScreamer->model.stateStep = 0;
                    }
                    break;

                case 2:
                    if (temp_fp == 3)
                    {
                        if (field14C_1 | field14C_0 | field14C_2)
                        {
                            airScreamer->model.controlState = AirScreamerControl_35;
                        }
                        else
                        {
                            airScreamer->model.controlState = AirScreamerControl_34;
                        }

                        airScreamer->model.stateStep = 0;
                    }
                    break;
            }
            break;

        case AirScreamerHit_Recoil:
        case AirScreamerHit_Rear:
            airScreamer->model.controlState = AirScreamerControl_Recoil;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            break;

        case AirScreamerHit_Stun:
        case AirScreamerHit_Kill:
            airScreamer->model.controlState = AirScreamerControl_Stun;
            airScreamer->model.stateStep    = 0;

            if (airScreamer->health <= Q12(0.0f))
            {
                airScreamerProps.flags |= AirScreamerFlag_6;
            }
            else
            {
                airScreamerProps.flags |= AirScreamerFlag_3;
            }
            break;
    }
#endif
}

// Note: Very close match to `AirScreamer_Control_7` other than some checks being removed.
// Changes here should be copied over there.

void AirScreamer_Control_6(s_SubCharacter* airScreamer)
{
#ifndef MAP0_S01
    s32  animStatus;
    s32  switchCond;
    bool cond;
    s32  temp_fp;
    s32  temp_fp_tmp;
    s32  field15C;
    bool field14C_0;
    bool field14C_1;
    bool field14C_2;
    bool field14C_0_tmp;
    bool field14C_1_tmp;
    bool field14C_2_tmp;

    animStatus     = airScreamer->model.anim.status;
    switchCond     = 0;
    cond           = false;
    temp_fp_tmp    = sharedFunc_800D4A80_0_s01(airScreamer);
    field15C       = sharedData_800E21D0_0_s01.field_15C;
    field14C_0_tmp = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_0;
    field14C_1_tmp = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_1;
    field14C_2_tmp = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_2;
    temp_fp        = temp_fp_tmp;
    field14C_0     = field14C_0_tmp;
    field14C_1     = field14C_1_tmp;
    field14C_2     = field14C_2_tmp;

    // Handle state step.
    switch (airScreamer->model.stateStep)
    {
        case 0:
            airScreamerProps.timer_120 = Q12(6.0f);

        case 1:
            sharedFunc_800DE034_2_s00(airScreamer, &airScreamerProps.headTargetPosition, Q12(1.5f));

            if (!sharedFunc_800DC30C_2_s00(airScreamer))
            {
                airScreamer->model.stateStep = 2;
            }
            else if (Rng_RandQ12() >= Q12(0.3f))
            {
                airScreamer->model.stateStep = 2;
            }
            else
            {
                airScreamer->model.stateStep = 4;
            }
            break;

        case 2:
            cond = true;

        case 3:
            if (sharedFunc_800DC598_2_s00(airScreamer))
            {
                airScreamer->model.stateStep = 4;
                break;
            }

            if (Math_Distance2dGet(&airScreamer->position, &airScreamerProps.targetPosition) < Q12(1.0f))
            {
                airScreamer->model.stateStep = 1;
            }
            else if (airScreamerProps.timer_120 == Q12(0.0f))
            {
                if (cond)
                {
                    switchCond = 1;
                }

                airScreamer->model.stateStep = 0;
            }
            else if (field14C_0 || field14C_1 || field14C_2)
            {
                airScreamer->model.stateStep = 3;

                switchCond = 0;

                if (field15C > 32)
                {
                    s32 var_v0 = Q12_ANGLE_NORM_S(func_80080478(&airScreamer->position, &airScreamerProps.headTargetPosition) - airScreamer->rotation.vy);
                    sharedFunc_800DDF74_2_s00(airScreamer, Q12(1.5f), (var_v0 / 4) + airScreamer->rotation.vy);
                    airScreamerProps.timer_120 = Q12(6.0f);
                }
            }
            break;

        case 4:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_WalkForward, true) ||
                animStatus == ANIM_STATUS(AirScreamerAnim_StandIdle, true))
            {
                switchCond                           = 2;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_StandIdleToHover, false);

                if (field14C_2 || field14C_1)
                {
                    airScreamer->model.stateStep = 7;
                }
                else
                {
                    airScreamer->model.stateStep = 6;
                }
            }
            else if (field14C_2 || field14C_1)
            {
                airScreamer->model.stateStep = 5;
            }
            break;

        case 5:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_WalkForward, true) ||
                animStatus == ANIM_STATUS(AirScreamerAnim_StandIdle, true))
            {
                switchCond                           = 3;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_StandIdleToHover, false);
                airScreamer->model.stateStep   = 7;
            }
            break;

        case 6:
            if (field14C_2 || field14C_1)
            {
                switchCond                       = 3;
                airScreamer->model.stateStep = 7;
            }
            else
            {
                switchCond = 2;
            }
            break;

        case 7:
            switchCond = 3;
            break;
    }

    sharedFunc_800DF448_2_s00(airScreamer, false);

    // Handle damage.
    switch (AirScreamer_DamageTake(airScreamer, Q12(1.0f)))
    {
        case AirScreamerHit_None:
            switch (switchCond)
            {
                case 0:
                case 1:
                    if (field14C_2 || field14C_1)
                    {
                        airScreamer->model.controlState = AirScreamerControl_7;
                        airScreamer->model.stateStep    = 0;
                    }
                    else if (switchCond == 1 && Rng_TestProbability(Q12(0.5f)))
                    {
                        s32 randVal = Rng_RandQ12();
                        s32 var_a0  = Q12(0.0f);

                        if (airScreamerProps.field_E8_8 == 1)
                        {
                            var_a0 = Q12(0.1f);
                        }

                        if (randVal < Q12(0.7f) - (var_a0 * 2))
                        {
                            airScreamer->model.controlState = AirScreamerControl_15;
                            airScreamer->model.stateStep    = 0;
                        }
                    }
                    break;

                case 2:
                    if (temp_fp == 2)
                    {
                        airScreamer->model.controlState = AirScreamerControl_20;
                        airScreamer->model.stateStep    = 0;
                    }
                    break;

                case 3:
                    if (temp_fp == 2)
                    {
                        airScreamer->model.controlState = AirScreamerControl_21;
                        airScreamer->model.stateStep    = 0;
                    }
                    break;
            }
            break;

        case AirScreamerHit_Recoil:
        case AirScreamerHit_Rear:
            airScreamer->model.controlState = AirScreamerControl_Recoil;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            break;

        case AirScreamerHit_Stun:
        case AirScreamerHit_Kill:
            airScreamer->model.controlState = AirScreamerControl_Stun;
            airScreamer->model.stateStep    = 0;
            if (airScreamer->health <= Q12(0.0f))
            {
                airScreamerProps.flags |= AirScreamerFlag_6;
            }
            else
            {
                airScreamerProps.flags |= AirScreamerFlag_3;
            }
            break;
    }
#endif
}

// Note: Very close match to `AirScreamer_Control_6` other than some checks being removed.
// Changes here should be copied over there.
void AirScreamer_Control_7(s_SubCharacter* airScreamer)
{
#ifndef MAP0_S01
    s32  animStatus;
    s32  switchCond;
    bool var_s5;
    s32  temp_fp;
    s32  temp_fp_tmp;
    s32  field15C;
    bool field14C_0;
    bool field14C_1;
    bool field14C_2;
    bool field14C_0_tmp;
    bool field14C_1_tmp;
    bool field14C_2_tmp;

    animStatus     = airScreamer->model.anim.status;
    switchCond     = 0;
    var_s5         = false;
    temp_fp_tmp    = sharedFunc_800D4A80_0_s01(airScreamer);
    field15C       = sharedData_800E21D0_0_s01.field_15C;
    field14C_0_tmp = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_0;
    field14C_1_tmp = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_1;
    field14C_2_tmp = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_2;
    field14C_0     = field14C_0_tmp;
    field14C_1     = field14C_1_tmp;
    temp_fp        = temp_fp_tmp;
    field14C_2     = field14C_2_tmp;

    // Handle state step.
    switch (airScreamer->model.stateStep)
    {
        case 0:
            airScreamerProps.timer_120 = Q12(6.0f);

        case 1:
            sharedFunc_800DE034_2_s00(airScreamer, &airScreamerProps.headTargetPosition, Q12(2.0f));

            if (!sharedFunc_800DC30C_2_s00(airScreamer))
            {
                airScreamer->model.stateStep = 2;
            }
            else if (Rng_RandQ12() >= Q12(0.5f))
            {
                airScreamer->model.stateStep = 2;
            }
            else
            {
                airScreamer->model.stateStep = 4;
            }
            break;

        case 2:
            var_s5 = true;

        case 3:
            if (sharedFunc_800DC598_2_s00(airScreamer))
            {
                airScreamer->model.stateStep = 4;
                break;
            }

            if (Math_Distance2dGet(&airScreamer->position, &airScreamerProps.targetPosition) < Q12(1.0f))
            {
                airScreamer->model.stateStep = 1;
            }
            else if (airScreamerProps.timer_120 == Q12(0.0f))
            {
                if (var_s5)
                {
                    switchCond = 1;
                }

                airScreamer->model.stateStep = 0;
            }
            else if (field14C_2 || field14C_0 || field14C_1)
            {
                airScreamer->model.stateStep = 3;

                if (field15C > 32)
                {
                    s32 var_v0 = Q12_ANGLE_NORM_S(func_80080478(&airScreamer->position, &airScreamerProps.headTargetPosition) - airScreamer->rotation.vy);
                    sharedFunc_800DDF74_2_s00(airScreamer, Q12(1.5f), (var_v0 / 4) + airScreamer->rotation.vy);
                    airScreamerProps.timer_120 = Q12(6.0f);
                }
            }
            break;

        case 4:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_WalkForward, true) || animStatus == ANIM_STATUS(AirScreamerAnim_StandIdle, true))
            {
                switchCond                           = 2;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_StandIdleToHover, false);

                if (field14C_1 || field14C_0)
                {
                    airScreamer->model.stateStep = 7;
                }
                else
                {
                    airScreamer->model.stateStep = 6;
                }
            }
            else if (field14C_2 || field14C_1 || field14C_0)
            {
                airScreamer->model.stateStep = 5;
            }
            break;

        case 5:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_WalkForward, true) || animStatus == ANIM_STATUS(AirScreamerAnim_StandIdle, true))
            {
                switchCond                           = 3;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_StandIdleToHover, false);
                airScreamer->model.stateStep   = 7;
            }
            break;

        case 6:
            if (field14C_2 || field14C_1 || field14C_0)
            {
                switchCond                       = 3;
                airScreamer->model.stateStep = 7;
            }
            else
            {
                switchCond = 2;
            }
            break;

        case 7:
            switchCond = 3;
            break;
    }

    sharedFunc_800DF448_2_s00(airScreamer, false);

    switch (AirScreamer_DamageTake(airScreamer, Q12(1.0f)))
    {
        case AirScreamerHit_None:
            switch (switchCond)
            {
                case 0:
                case 1:
                    if (field14C_2)
                    {
                        airScreamer->model.controlState = AirScreamerControl_8;
                        airScreamer->model.stateStep    = 0;
                    }
                    else if (switchCond == 1)
                    {
                        s32 randVal = Rng_RandQ12();

                        s32 var_a0 = Q12(0.0f);
                        if (airScreamerProps.field_E8_8 == 1)
                        {
                            var_a0 = Q12(0.1f);
                        }

                        if (randVal < Q12(0.7f) - (var_a0 * 2))
                        {
                            airScreamer->model.controlState = AirScreamerControl_6;
                            airScreamer->model.stateStep    = 0;
                        }
                    }
                    break;

                case 2:
                    if (temp_fp == 2)
                    {
                        if (!field14C_2)
                        {
                            airScreamer->model.controlState = AirScreamerControl_20;
                        }
                        else
                        {
                            airScreamer->model.controlState = AirScreamerControl_22;
                        }

                        airScreamer->model.stateStep = 0;
                    }
                    break;

                case 3:
                    if (temp_fp == 2)
                    {
                        if (field14C_2)
                        {
                            airScreamer->model.controlState = AirScreamerControl_22;
                        }
                        else
                        {
                            airScreamer->model.controlState = AirScreamerControl_21;
                        }

                        airScreamer->model.stateStep = 0;
                    }
                    break;
            }
            break;

        case AirScreamerHit_Recoil:
        case AirScreamerHit_Rear:
            airScreamer->model.controlState = AirScreamerControl_Recoil;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            break;

        case AirScreamerHit_Stun:
        case AirScreamerHit_Kill:
            airScreamer->model.controlState = AirScreamerControl_Stun;
            airScreamer->model.stateStep    = 0;

            if (airScreamer->health <= Q12(0.0f))
            {
                airScreamerProps.flags |= AirScreamerFlag_6;
            }
            else
            {
                airScreamerProps.flags |= AirScreamerFlag_3;
            }
            break;
    }
#endif
}

void AirScreamer_Control_8(s_SubCharacter* airScreamer)
{
#ifndef MAP0_S01
    s32    animStatus;
    s32    switchCond1;
    q19_12 var_s7;
    s32    sp10;
    q19_12 unkDist;
    q19_12 unkAngle;
    bool   field14C_2;
    s32    temp_s3;
    s32    switchCond0;
    s32    chance0;
    s32    chance1;

    animStatus  = airScreamer->model.anim.status;
    switchCond1 = 0;
    var_s7      = Q12(0.0f);
    sp10        = sharedFunc_800D4A80_0_s01(airScreamer);
    unkDist     = sharedData_800E21D0_0_s01.distance_150;
    unkAngle    = sharedData_800E21D0_0_s01.angle_154;
    field14C_2  = sharedData_800E21D0_0_s01.field_14C.bits.field_14C_2;

    if (airScreamerProps.field_E8_4)
    {
        var_s7 = Q12(0.1f);
    }

    temp_s3     = sharedFunc_800DC598_2_s00(airScreamer);
    switchCond0 = 0;

    // Handle state step.
    switch (airScreamer->model.stateStep)
    {
        case 0:
            switchCond1                = 1;
            airScreamerProps.timer_120 = Q12(2.0f);
            airScreamer->flags     |= CharaFlag_Hit;

        case 1:
            sharedFunc_800DDF74_2_s00(airScreamer, unkDist / 2, unkAngle);

            if (!sharedFunc_800DC30C_2_s00(airScreamer) || Rng_RandQ12() >= Q12(0.3f))
            {
                if (Rng_TestProbability(Q12(0.2f) + (var_s7 * 2)))
                {
                    airScreamer->model.stateStep = 2;
                }
                else
                {
                    airScreamer->model.stateStep = 3;
                }
            }
            else
            {
                airScreamer->model.stateStep = 4;
            }
            break;

        case 2:
            switchCond0 = 1;

            if (temp_s3 != 0)
            {
                airScreamer->model.stateStep = 4;
            }
            else if (airScreamerProps.timer_120 != Q12(0.0f))
            {
                if (Math_Distance2dGet(&airScreamer->position, &airScreamerProps.targetPosition) < Q12(1.0f))
                {
                    airScreamer->model.stateStep = 1;
                }
            }
            else
            {
                airScreamer->model.stateStep = 0;
            }
            break;

        case 3:
            sharedFunc_800DDF74_2_s00(airScreamer, unkDist / 2, unkAngle);

            #define angleDiff Q12_ANGLE_NORM_S(unkAngle - airScreamer->rotation.vy)

            if (temp_s3 != 0)
            {
                airScreamer->model.stateStep = 4;
            }
            else if (airScreamerProps.timer_120 == Q12(0.0f))
            {
                airScreamer->model.stateStep = 0;
            }
            else if (angleDiff >= Q12_ANGLE(-10.0f) && angleDiff < Q12_ANGLE(10.0f))
            {
                switchCond0 = 2;
            }
            break;

            #undef angleDiff

        case 4:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_WalkForward, true) ||
                animStatus == ANIM_STATUS(AirScreamerAnim_StandIdle, true))
            {
                switchCond1                          = 2;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_StandIdleToHover, false);
                airScreamer->model.stateStep   = 5;
            }
            break;

        case 5:
            switchCond1 = 2;
            break;
    }

    switch (switchCond0)
    {
        case 1:
            sharedFunc_800DF448_2_s00(airScreamer, false);
            break;

        case 2:
            sharedFunc_800DF2D0_2_s00(airScreamer);
            break;

        default:
            sharedFunc_800DF60C_2_s00(airScreamer);
            break;
    }

    switch (AirScreamer_DamageTake(airScreamer, Q12(1.0f)))
    {
        case AirScreamerHit_None:
            switch (switchCond1)
            {
                case 0:
                    if (field14C_2 == 0)
                    {
                        airScreamer->model.controlState = AirScreamerControl_7;
                        airScreamer->model.stateStep    = 0;
                    }
                    break;

                case 1:
                    chance0 = sharedFunc_800DC894_2_s00(airScreamer, unkDist);
                    chance1 = sharedFunc_800DC6E4_2_s00(airScreamer, unkDist);

                    if (Rng_TestProbability(chance0))
                    {
                        if (airScreamerProps.field_E8_0 == 3)
                        {
                            sharedFunc_800DD13C_2_s00(airScreamer, airScreamer->field_40 + 1, Q12(0.6f));
                        }

                        airScreamer->model.controlState = AirScreamerControl_10;
                        airScreamer->model.stateStep    = 0;
                        airScreamerProps.flags         |= AirScreamerFlag_4;
                    }
                    else if (Rng_TestProbability(chance1))
                    {
                        airScreamer->model.controlState = AirScreamerControl_14;
                        airScreamer->model.stateStep    = 0;
                        airScreamerProps.flags         |= AirScreamerFlag_4;
                    }
                    break;

                case 2:
                    if (sp10 == switchCond1)
                    {
                        airScreamer->model.controlState = AirScreamerControl_22;
                        airScreamer->model.stateStep    = 0;
                    }
                    break;
            }
            break;

        case AirScreamerHit_Recoil:
        case AirScreamerHit_Rear:
            airScreamer->model.controlState = AirScreamerControl_Recoil;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            break;

        case AirScreamerHit_Stun:
        case AirScreamerHit_Kill:
            airScreamer->model.controlState = AirScreamerControl_Stun;
            airScreamer->model.stateStep    = 0;

            if (airScreamer->health <= Q12(0.0f))
            {
                airScreamerProps.flags |= AirScreamerFlag_6;
            }
            else
            {
                airScreamerProps.flags |= AirScreamerFlag_3;
            }
            break;
    }
#endif
}

void AirScreamer_Control_9(s_SubCharacter* airScreamer)
{
#ifndef MAP0_S01
    bool cond0;
    bool cond1;
    u32  temp_v1;
    s32  animStatus;

    cond0      = false;
    animStatus = airScreamer->model.anim.status;

    // Handle state step.
    switch ((u32)airScreamer->model.stateStep)
    {
        case 0:
            sharedFunc_800DDF74_2_s00(airScreamer, Q12(0.1f), airScreamer->rotation.vy);
            airScreamer->model.stateStep = 7;
            airScreamerProps.timer_120       = Q12(56.0f);
            break;

        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            if (airScreamerProps.timer_120 == Q12(0.0f))
            {
                cond0 = true;
            }
            else if ((((u32)airScreamer->model.stateStep - 1) * Q12(8.0f)) >= airScreamerProps.timer_120 &&
                     animStatus == ANIM_STATUS(AirScreamerAnim_StandBiteAttackEnd, true))
            {
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_StandBiteAttackStart, false);
                airScreamer->model.stateStep   = 8;
                airScreamerProps.flags        |= AirScreamerFlag_2;
            }
            break;

        case 8:
            airScreamerProps.timer_120 += g_DeltaTime;

            if (animStatus == ANIM_STATUS(AirScreamerAnim_StandBiteAttackEnd, true))
            {
                temp_v1 = Rng_RandQ12() * 12;
                if (temp_v1 < airScreamerProps.timer_120)
                {
                    airScreamerProps.timer_120 -= temp_v1;
                }
                else
                {
                    airScreamerProps.timer_120 = Q12(0.0f);
                }

                airScreamer->model.stateStep = ((u32)airScreamerProps.timer_120 / Q12(8.0f)) + 1;
            }
            break;
    }

    sharedFunc_800DF2D0_2_s00(airScreamer);

    switch (AirScreamer_DamageTake(airScreamer, Q12(1.0f)))
    {
        case AirScreamerHit_None:
            switch (cond0)
            {
                case 0:
                    if (ANIM_STATUS_IS_ACTIVE(animStatus))
                    {
                        if (sharedData_800E21D0_0_s01.field_14C.bits.field_14C_2)
                        {
                            if (!airScreamerProps.field_E8_8)
                            {
                                cond1 = sharedFunc_800D3814_0_s01(airScreamer) >= Q12(2.0f);
                            }
                            else
                            {
                                cond1 = true;
                            }

                            if (cond1)
                            {
                                airScreamer->model.anim.status  = ANIM_STATUS(AirScreamerAnim_StandIdle, false);
                                airScreamer->model.controlState = AirScreamerControl_10;
                                airScreamer->model.stateStep    = 0;
                                airScreamer->flags             |= CharaFlag_Hit;
                                airScreamerProps.flags         |= AirScreamerFlag_4;
                                airScreamerProps.field_E8_8          = 3;
                            }
                        }
                        else if (airScreamerProps.flags & AirScreamerFlag_31 ||
                                 sharedData_800E21D0_0_s01.field_14C.bits.field_14C_1 ||
                                 sharedData_800E21D0_0_s01.field_15C > Q12(10.0f))
                        {
                            airScreamer->model.anim.status  = ANIM_STATUS(AirScreamerAnim_StandIdle, false);
                            airScreamer->model.controlState = AirScreamerControl_7;
                            airScreamer->model.stateStep    = 0;
                            airScreamerProps.field_E8_8          = 3;
                        }
                    }
                    break;

                case 1:
                    airScreamer->model.anim.status  = ANIM_STATUS(AirScreamerAnim_StandIdle, false);
                    airScreamer->model.controlState = AirScreamerControl_15;
                    airScreamer->model.stateStep    = 0;
                    sharedFunc_800D3DFC_0_s01(airScreamer);
                    break;
            }
            break;

        case AirScreamerHit_Recoil:
        case AirScreamerHit_Rear:
            airScreamer->model.controlState = AirScreamerControl_Recoil;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            return;

        case AirScreamerHit_Stun:
        case AirScreamerHit_Kill:
            airScreamer->model.controlState = AirScreamerControl_Stun;
            airScreamer->model.stateStep    = 0;

            if (airScreamer->health <= Q12(0.0f))
            {
                airScreamerProps.flags |= AirScreamerFlag_6;
            }
            else
            {
                airScreamerProps.flags |= AirScreamerFlag_3;
            }
            break;
    }
#endif
}

void AirScreamer_Control_10(s_SubCharacter* airScreamer)
{
#ifndef MAP0_S01
    s32    animStatus;
    s32    sp14;
    s32    sp18;
    q19_12 height;
    s32    temp_s7;
    s32    temp_fp;
    q19_12 unkDist;
    s32    temp_s1_2;
    q19_12 playerPosY;
    s32    temp_s4_2;
    s32    temp_s5;
    s32    temp_s6;
    s32    switchCond;
    bool   new_var;

    animStatus = airScreamer->model.anim.status;
    sp14       = sharedFunc_800D4A80_0_s01(airScreamer);
    new_var    = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_2;
    sp18       = new_var;
    unkDist    = sharedData_800E21D0_0_s01.distance_150;

    sharedFunc_800DE11C_2_s00(airScreamer);
    switchCond = 0;

    // Handle state step.
    switch (airScreamer->model.stateStep)
    {
        case 0:
            temp_s5 = unkDist + (Rng_RandQ12() * 4);

            if (sharedFunc_800DC200_2_s00(airScreamer) && temp_s5 > Q12(8.0f))
            {
                airScreamer->model.stateStep = 4;
            }
            else if (!sharedFunc_800DC30C_2_s00(airScreamer) || temp_s5 <= Q12(4.0f) && Rng_RandQ12() >= Q12(0.5f))
            {
                airScreamer->model.stateStep = 1;
            }
            else
            {
                airScreamer->model.stateStep = 2;
            }

            airScreamerProps.timer_120 = Q12(4.0f);
            break;

        case 1:

            temp_s1_2 = sharedFunc_800DC598_2_s00(airScreamer);
            temp_s5   = Q12(0.6f);
            temp_fp   = Math_Distance2dGet(&airScreamer->position, &airScreamerProps.targetPosition);

            if (temp_s1_2 == 1)
            {
                airScreamer->model.stateStep = 2;
            }
            else if (temp_s1_2 == 2)
            {
                airScreamer->model.stateStep = 4;
            }
            else if (temp_fp < Q12(1.1f))
            {
                temp_s6   = airScreamer->position.vy + airScreamer->collision.box.top + Q12(0.1f);
                playerPosY = g_SysWork.playerWork.player.position.vy;
                height = g_SysWork.playerWork.player.collision.box.height;
                temp_s7   = g_SysWork.playerWork.player.collision.box.top;

                temp_s4_2 = Q12_ANGLE_NORM_S(func_80080478(&airScreamer->position, &airScreamerProps.targetPosition) - airScreamer->rotation.vy);

                if (sharedFunc_800DC30C_2_s00(airScreamer))
                {
                    if ((playerPosY + height) < temp_s6 || temp_s6 < (playerPosY + temp_s7))
                    {
                        airScreamer->model.stateStep = 2;
                        break;
                    }
                }

                if (temp_fp < temp_s5)
                {
                    if (temp_s4_2 > -0x101 && temp_s4_2 < 0x100)
                    {
                        switchCond = 1;
                    }
                }
            }
            else if (airScreamerProps.timer_120 == Q12(0.0f))
            {
                airScreamer->model.stateStep = 0;
            }
            break;

        case 2:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_StandIdle, true) ||
                animStatus == ANIM_STATUS(AirScreamerAnim_WalkForward, true))
            {
                switchCond                           = 2;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_StandIdleToHover, false);
                airScreamer->model.stateStep   = 3;
            }
            break;

        case 3:
            switchCond = 2;
            break;

        case 4:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_StandIdle, true) ||
                animStatus == ANIM_STATUS(AirScreamerAnim_WalkForward, true))
            {
                switchCond                           = 3;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_StandIdleToGlide, false);
                airScreamer->model.stateStep   = 5;
            }
            break;

        case 5:
            switchCond = 3;
            break;
    }

    sharedFunc_800DF448_2_s00(airScreamer, 1);

    // Handle damage.
    switch (AirScreamer_DamageTake(airScreamer, Q12(1.0f)))
    {
        case AirScreamerHit_None:
            switch (switchCond)
            {
                case 0:
                    if (!sharedFunc_800DC67C_2_s00(airScreamer))
                    {
                        if (sp18 == 0)
                        {
                            airScreamer->model.controlState = AirScreamerControl_13;
                            airScreamer->model.stateStep    = 0;
                        }
                    }
                    else
                    {
                        airScreamer->model.controlState = AirScreamerControl_14;
                        airScreamer->model.stateStep    = 0;
                    }
                    break;

                case 1:
                    if (!Chara_HasFlag(&g_SysWork.playerWork.player, CharaFlag_Unk4) &&
                        g_SysWork.npcIdxs[0] == NO_VALUE &&
                        g_SysWork.npcIdxs[1] == NO_VALUE)
                    {
                        if (animStatus == ANIM_STATUS(AirScreamerAnim_WalkForward, true) ||
                            animStatus == ANIM_STATUS(AirScreamerAnim_StandIdle, true))
                        {
                            airScreamer->model.controlState = AirScreamerControl_12;
                            airScreamer->model.stateStep    = 0;
                        }
                    }
                    else
                    {
                        airScreamer->model.controlState = AirScreamerControl_11;
                        airScreamer->model.stateStep    = 0;
                    }
                    break;

                case 2:
                    if (sp14 == 2)
                    {
                        airScreamer->model.controlState = AirScreamerControl_23;
                        airScreamer->model.stateStep    = 0;
                    }
                    break;

                case 3:
                    if (sp14 == 3)
                    {
                        airScreamer->model.controlState = AirScreamerControl_38;
                        airScreamer->model.stateStep    = 0;
                    }
                    break;
            }
            break;

        case AirScreamerHit_Recoil:
        case AirScreamerHit_Rear:
            airScreamer->model.controlState = AirScreamerControl_Recoil;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            break;

        case AirScreamerHit_Stun:
        case AirScreamerHit_Kill:
            airScreamer->model.controlState = AirScreamerControl_Stun;
            airScreamer->model.stateStep    = 0;

            if (airScreamer->health <= Q12(0.0f))
            {
                airScreamerProps.flags |= AirScreamerFlag_6;
            }
            else
            {
                airScreamerProps.flags |= AirScreamerFlag_3;
            }
            break;
    }
#endif
}

void AirScreamer_Control_11(s_SubCharacter* airScreamer)
{
#ifndef MAP0_S01
    s32 bit3;
    s32 f150;
    s32 idx;
    s32 switch2Idx;
    u32 stateStep;
    s32 animStatus;
    s16 stateStep1;

    animStatus = airScreamer->model.anim.status;
    idx        = sharedFunc_800D4A80_0_s01(airScreamer);
    switch2Idx = 0;

    bit3       = sharedData_800E21D0_0_s01.field_14C.bits.field_14C_2;
    stateStep  = airScreamer->model.stateStep;
    f150       = sharedData_800E21D0_0_s01.distance_150;
    stateStep1 = 1;

    // Handle state step.
    switch (stateStep)
    {
        case 0:
            if (!sharedFunc_800DC30C_2_s00(airScreamer) || Rng_RandQ12() >= Q12_ANGLE(252.0f))
            {
                airScreamerProps.timer_120       = Q12(2.0f);
                airScreamer->model.stateStep = stateStep1;
            }
            else
            {
                airScreamer->model.stateStep = 2;
            }
            break;

        case 1:
            if (sharedFunc_800DC598_2_s00(airScreamer))
            {
                airScreamer->model.stateStep = 2;
            }
            break;

        case 2:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_WalkForward, true) ||
                animStatus == ANIM_STATUS(AirScreamerAnim_StandIdle, true))
            {
                switch2Idx                           = 1;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_StandIdleToHover, false);
                airScreamer->model.stateStep   = 3;
            }
            break;

        case 3:
            switch2Idx = 1;
            break;
    }

    sharedFunc_800DE1F8_2_s00(airScreamer);
    sharedFunc_800DF710_2_s00(airScreamer);

    switch (AirScreamer_DamageTake(airScreamer, Q12(1.0f)))
    {
        case AirScreamerHit_None:
            switch (switch2Idx)
            {
                case 0:
                    if (airScreamerProps.timer_120 == Q12(0.0f) || f150 > Q12(2.0f))
                    {
                        if (bit3 == 0)
                        {
                            airScreamer->model.controlState = AirScreamerControl_13;
                        }
                        else
                        {
                            airScreamer->model.controlState = AirScreamerControl_10;
                        }

                        airScreamer->model.stateStep = 0;
                    }
                    break;

                case 1:
                    if (idx == 2)
                    {
                        if (bit3 != 0)
                        {
                            airScreamer->model.controlState = AirScreamerControl_24;
                        }
                        else
                        {
                            airScreamer->model.controlState = AirScreamerControl_26;
                        }

                        airScreamer->model.stateStep = 0;
                    }
                    break;

                default:
                    return;
            }
            break;

        case AirScreamerHit_Recoil:
        case AirScreamerHit_Rear:
            airScreamer->model.controlState = AirScreamerControl_Recoil;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            return;

        case AirScreamerHit_Stun:
        case AirScreamerHit_Kill:
            airScreamer->model.controlState = AirScreamerControl_Stun;
            airScreamer->model.stateStep    = 0;

            if (airScreamer->health <= Q12(0.0f))
            {
                airScreamerProps.flags |= AirScreamerFlag_6;
            }
            else
            {
                airScreamerProps.flags |= AirScreamerFlag_3;
            }
            break;
    }
#endif
}

void AirScreamer_Control_12(s_SubCharacter* airScreamer)
{
#ifndef MAP0_S01
    bool cond0;
    bool cond1;
    s32  animStatus;

    cond0      = false;
    cond1      = false;
    animStatus = airScreamer->model.anim.status;

    switch (airScreamer->model.stateStep)
    {
        case 0:
            airScreamer->model.stateStep = 1;

        case 1:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_WalkForward, true) ||
                animStatus == ANIM_STATUS(AirScreamerAnim_StandIdle, true))
            {
                airScreamer->model.stateStep   = 2;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_StandBiteAttack, false);
            }
            break;

        case 2:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_StandBiteAttack, true) &&
                airScreamer->model.anim.keyframeIdx < 8192)
            {
                airScreamer->model.stateStep = 3;
                airScreamerProps.flags      |= AirScreamerFlag_5;
            }
            break;

        case 3:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_WalkForward, true) ||
                animStatus == ANIM_STATUS(AirScreamerAnim_StandIdle, true))
            {
                cond0 = true;
            }
            else if (airScreamer->field_44.field_8 == NO_VALUE)
            {
                cond0                            = true;
                cond1                            = true;
                airScreamer->model.stateStep = 4;
            }
            break;

        case 4:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_WalkForward, true) ||
                animStatus == ANIM_STATUS(AirScreamerAnim_StandIdle, true))
            {
                cond0 = true;
                cond1 = true;
            }
            break;
    }

    sharedFunc_800DF358_2_s00(airScreamer);

    switch (AirScreamer_DamageTake(airScreamer, Q12(1.2f)))
    {
        case AirScreamerHit_None:
            if (cond0)
            {
                if (airScreamerProps.field_E8_8 == 3)
                {
                    airScreamerProps.field_E8_8 = 2;

                    if (cond1 && Rng_TestProbability(Q12(0.5f)))
                    {
                        airScreamer->model.controlState = AirScreamerControl_11;
                    }
                    else
                    {
                        airScreamer->model.controlState = AirScreamerControl_10;
                    }
                }
                else
                {
                    if (cond1)
                    {
                        airScreamer->model.controlState = AirScreamerControl_11;
                    }
                    else if (Rng_TestProbability(Q12(0.5f)))
                    {
                        airScreamer->model.controlState = AirScreamerControl_11;
                    }
                    else
                    {
                        airScreamer->model.controlState = AirScreamerControl_10;
                    }
                }

                airScreamer->model.stateStep = 0;
            }
            break;

        case AirScreamerHit_Recoil:
        case AirScreamerHit_Rear:
            airScreamer->model.controlState = AirScreamerControl_Recoil;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            break;

        case AirScreamerHit_Stun:
        case AirScreamerHit_Kill:
            airScreamer->model.controlState = AirScreamerControl_Stun;
            airScreamer->model.stateStep    = 0;

            if (airScreamer->health <= Q12(0.0f))
            {
                airScreamerProps.flags |= AirScreamerFlag_6;
            }
            else
            {
                airScreamerProps.flags |= AirScreamerFlag_3;
            }
            break;
    }
#endif
}

void AirScreamer_Control_13(s_SubCharacter* airScreamer)
{
#ifndef MAP0_S01
    bool field14C_2;
    bool field14C_1;
    bool new_var2;
    bool new_var3;
    s32  switchCond;
    s32  animStatus;
    s32  temp_s4;
    s32  new_var;

    animStatus = airScreamer->model.anim.status;
    switchCond = 0;
    new_var    = sharedFunc_800D4A80_0_s01(airScreamer);
    new_var3   = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_1;
    new_var2   = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_2;
    field14C_1 = new_var3;
    temp_s4    = new_var;
    field14C_2 = new_var2;

    switch (airScreamer->model.stateStep)
    {
        case 0:
            sharedFunc_800DE1F8_2_s00(airScreamer);

        case 1:
            airScreamerProps.timer_120 = Q12(4.0f);
            if (sharedFunc_800DC30C_2_s00(airScreamer) && Rng_TestProbability(Q12(0.4f)))
            {
                airScreamer->model.stateStep = 3;
            }
            else
            {
                airScreamer->model.stateStep = 2;
            }
            break;

        case 2:
            if (sharedFunc_800DC598_2_s00(airScreamer))
            {
                airScreamer->model.stateStep = 3;
                break;
            }

            if (Math_Distance2dGet(&airScreamer->position, &airScreamerProps.targetPosition) < Q12(0.5f))
            {
                sharedFunc_800DE034_2_s00(airScreamer, &airScreamerProps.headTargetPosition, Q12(2.0f));
            }
            else if (airScreamerProps.timer_120 == Q12(0.0f))
            {
                sharedFunc_800DE034_2_s00(airScreamer, &airScreamerProps.headTargetPosition, Q12(2.0f));
                airScreamer->model.stateStep = 1;
            }
            break;

        case 3:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_WalkForward, true) ||
                animStatus == ANIM_STATUS(AirScreamerAnim_StandIdle, true))
            {
                switchCond                           = 1;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_StandIdleToHover, false);
                airScreamer->model.stateStep   = 4;
            }
            break;

        case 4:
            switchCond = 1;
            break;
    }

    sharedFunc_800DF448_2_s00(airScreamer, false);

    switch (AirScreamer_DamageTake(airScreamer, Q12(1.0f)))
    {
        case AirScreamerHit_None:
            switch (switchCond)
            {
                case 0:
                    if (!sharedFunc_800DC67C_2_s00(airScreamer))
                    {
                        if (field14C_2 == 0)
                        {
                            if (airScreamerProps.timer_120 == Q12(0.0f) && !field14C_1 && Rng_TestProbability(Q12(0.5f)))
                            {
                                airScreamer->model.controlState = AirScreamerControl_7;
                                airScreamer->model.stateStep    = 0;
                            }
                        }
                        else
                        {
                            airScreamer->model.controlState = AirScreamerControl_10;
                            airScreamer->model.stateStep    = 0;
                        }
                    }
                    else
                    {
                        airScreamer->model.controlState = AirScreamerControl_14;
                        airScreamer->model.stateStep    = 0;
                    }
                    break;

                case 1:
                    if (temp_s4 == 2)
                    {
                        if (field14C_2 != 0)
                        {
                            airScreamer->model.controlState = AirScreamerControl_23;
                        }
                        else
                        {
                            airScreamer->model.controlState = AirScreamerControl_26;
                        }

                        airScreamer->model.stateStep = 0;
                    }
                    break;
            }
            break;

        case AirScreamerHit_Recoil:
        case AirScreamerHit_Rear:
            airScreamer->model.controlState = AirScreamerControl_Recoil;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            break;

        case AirScreamerHit_Stun:
        case AirScreamerHit_Kill:
            airScreamer->model.controlState = AirScreamerControl_Stun;
            airScreamer->model.stateStep    = 0;

            if (airScreamer->health <= Q12(0.0f))
            {
                airScreamerProps.flags |= AirScreamerFlag_6;
            }
            else
            {
                airScreamerProps.flags |= AirScreamerFlag_3;
            }
            break;
    }
#endif
}

void AirScreamer_Control_14(s_SubCharacter* airScreamer)
{
#ifndef MAP0_S01
    s32    animStatus;
    s32    switchCond;
    s32    temp_s7;
    s32    temp_s7_tmp;
    q19_12 dist;
    q19_12 angle;
    bool   field14C_2;
    bool   field14C_2_tmp;
    q19_12 val2_0 = Q12(2.0f); // @hack Used to fix register order, couldn't find how to remove this yet.
    q19_12 distFieldF8;
    s32    var_s6;             // Possibly `bool`.
    q19_12 angleDelta;
    s32    temp_v0;

    animStatus     = airScreamer->model.anim.status;
    switchCond     = 0;
    temp_s7_tmp    = sharedFunc_800D4A80_0_s01(airScreamer);
    dist           = sharedData_800E21D0_0_s01.distance_150;
    angle          = sharedData_800E21D0_0_s01.angle_154;
    field14C_2_tmp = sharedData_800E21D0_0_s01.field_14C.bits.field_14C_2;
    temp_s7        = temp_s7_tmp;
    field14C_2     = field14C_2_tmp;
    distFieldF8    = Math_Distance2dGet(&airScreamer->position, &airScreamerProps.targetPosition);
    var_s6         = 0;

    switch (airScreamer->model.stateStep)
    {
        case 0:
            var_s6      = sharedFunc_800DE578_2_s00(airScreamer);
            distFieldF8 = Math_Distance2dGet(&airScreamer->position, &airScreamerProps.targetPosition);
            angleDelta  = Q12_ANGLE_NORM_S(angle - airScreamer->rotation.vy);

            if (sharedFunc_800DC200_2_s00(airScreamer) &&
                (Math_CheckSignedRange(angleDelta, Q12_ANGLE(120.0f)) ||
                 (dist < Q12(5.0f) && Math_CheckSignedRange(angleDelta, Q12_ANGLE(60.0f)))))
            {
                airScreamer->model.stateStep = 4;
            }
            else if (!sharedFunc_800DC30C_2_s00(airScreamer) ||
                     (dist >= val2_0 && Rng_RandQ12() >= Q12(0.4f)))
            {
                airScreamerProps.timer_120       = Q12(2.0f);
                airScreamer->model.stateStep = 1;
            }
            else
            {
                temp_v0                          = 1;
                airScreamer->model.stateStep = temp_v0 * 2; // @hack Copy of `temp_v0 == 1` hack below for them to be folded together.
            }
            break;

        case 1:
            temp_v0 = sharedFunc_800DC598_2_s00(airScreamer);
            if (temp_v0 == 1)
            {
                airScreamer->model.stateStep = temp_v0 * 2; // @hack This should just be `= 2`, but that causes extra `li v0, 2` to be emitted here?
            }
            else if (temp_v0 == 2)
            {
                airScreamer->model.stateStep = 4;
            }
            else if (distFieldF8 < Q12(2.0f) || (airScreamerProps.flags & AirScreamerFlag_31))
            {
                var_s6 = sharedFunc_800DE578_2_s00(airScreamer);
            }
            else if (airScreamerProps.timer_120 == Q12(0.0f))
            {
                airScreamer->model.stateStep = 0;
            }
            break;

        case 2:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_StandIdle, true) ||
                animStatus == ANIM_STATUS(AirScreamerAnim_WalkForward, true))
            {
                switchCond                           = 1;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_StandIdleToHover, false);
                airScreamer->model.stateStep   = 3;
            }
            break;

        case 3:
            switchCond = 1;
            break;

        case 4:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_StandIdle, true) ||
                animStatus == ANIM_STATUS(AirScreamerAnim_WalkForward, true))
            {
                switchCond                           = 2;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_StandIdleToGlide, false);
                airScreamer->model.stateStep   = 5;
            }
            break;

        case 5:
            switchCond = 2;
            break;
    }

    sharedFunc_800DF448_2_s00(airScreamer, true);

    switch (AirScreamer_DamageTake(airScreamer, Q12(1.0f)))
    {
        case AirScreamerHit_None:
            switch (switchCond)
            {
                case 0:
                    distFieldF8 = Math_Distance2dGet(&airScreamer->position, &airScreamerProps.targetPosition);

                    if (var_s6 || (((field14C_2 != 0 && dist < Q12(15.0f)) || dist < Q12(4.0f)) && distFieldF8 < Q12(1.5f)))
                    {
                        airScreamer->model.controlState = AirScreamerControl_10;
                        airScreamer->model.stateStep    = 0;
                        airScreamerProps.field_E8_8         = 3;
                        airScreamerProps.flags         |= AirScreamerFlag_4;
                    }
                    else if (dist > Q12(30.0f) && distFieldF8 < Q12(1.0f))
                    {
                        airScreamer->model.controlState = AirScreamerControl_7;
                        airScreamer->model.stateStep    = 0;
                    }
                    break;

                case 1:
                    if (temp_s7 == 2)
                    {
                        airScreamer->model.controlState = AirScreamerControl_27;
                        airScreamer->model.stateStep    = 0;
                    }
                    break;

                case 2:
                    if (temp_s7 == 3)
                    {
                        airScreamer->model.controlState = AirScreamerControl_42;
                        airScreamer->model.stateStep    = 0;
                    }
                    break;
            }
            break;

        case AirScreamerHit_Recoil:
        case AirScreamerHit_Rear:
            airScreamer->model.controlState = AirScreamerControl_Recoil;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            break;

        case AirScreamerHit_Stun:
        case AirScreamerHit_Kill:
            airScreamer->model.controlState = AirScreamerControl_Stun;
            airScreamer->model.stateStep    = 0;

            if (airScreamer->health <= Q12(0.0f))
            {
                airScreamerProps.flags |= AirScreamerFlag_6;
            }
            else
            {
                airScreamerProps.flags |= AirScreamerFlag_3;
            }
            break;
    }
#endif
}

void AirScreamer_Control_15(s_SubCharacter* airScreamer)
{
#ifndef MAP0_S01
    sharedFunc_800DDF74_2_s00(airScreamer, Q12(0.5f), airScreamer->rotation.vy);
    sharedFunc_800DF2D0_2_s00(airScreamer);
    airScreamer->flags &= ~CharaFlag_Hit;

    // Handle damage.
    switch (AirScreamer_DamageTake(airScreamer, Q12(1.0f)))
    {
        case AirScreamerHit_None:
            switch ((u32)airScreamerProps.field_E8_0)
            {
                case 0:
                    airScreamer->model.controlState = AirScreamerControl_4;
                    airScreamer->model.stateStep    = 0;
                    break;

                case 1:
                    airScreamer->model.controlState = AirScreamerControl_5;
                    airScreamer->model.stateStep    = 0;
                    break;

                case 3:
                    airScreamer->model.controlState = AirScreamerControl_5;
                    airScreamer->model.stateStep    = 0;
                    break;
            }
            break;

        case AirScreamerHit_Recoil:
        case AirScreamerHit_Rear:
            airScreamer->model.controlState = AirScreamerControl_Recoil;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            break;

        case AirScreamerHit_Stun:
        case AirScreamerHit_Kill:
            airScreamer->model.controlState = AirScreamerControl_Stun;
            airScreamer->model.stateStep    = 0;

            if (airScreamer->health <= Q12(0.0f))
            {
                airScreamerProps.flags |= AirScreamerFlag_6;
            }
            else
            {
                airScreamerProps.flags |= AirScreamerFlag_3;
            }
            break;
    }
#endif
}

void AirScreamer_ControlRecoil(s_SubCharacter* airScreamer)
{
#ifndef MAP0_S01
    s32    animStatus;
    bool   field14C_2;
    bool   field14C_2_tmp;
    bool   cond;
    s32    var_a0;
    q19_12 var_s1;

    animStatus             = airScreamer->model.anim.status;
    field14C_2_tmp         = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_2;
    airScreamer->flags |= CharaFlag_Hit;
    field14C_2             = field14C_2_tmp;
    cond                   = false;

    // Handle state step.
    switch ((u32)airScreamer->model.stateStep)
    {
        case 0:
            if (field14C_2)
            {
                sharedFunc_800DE11C_2_s00(airScreamer);
            }
            else
            {
                sharedFunc_800DDF74_2_s00(airScreamer, Q12(4.0f), func_80080478(&airScreamer->position, &g_SysWork.playerWork.player.position));
                airScreamerProps.headTargetPosition = airScreamerProps.targetPosition;
            }

            airScreamer->model.stateStep = 1;

        case 1:
            if (ANIM_STATUS_IS_ACTIVE(animStatus))
            {
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_StandRecoil, false);
                airScreamer->model.stateStep   = 2;
            }
            break;

        case 2:
            if (animStatus != ANIM_STATUS(AirScreamerAnim_StandRecoil, false))
            {
                airScreamer->model.stateStep = 3;
                airScreamerProps.flags      |= AirScreamerFlag_3;
            }
            break;

        case 3:
            if (animStatus != ANIM_STATUS(AirScreamerAnim_StandRecoil, true))
            {
                cond = true;
            }
            break;
    }

    sharedFunc_800DF80C_2_s00(airScreamer);

    switch (AirScreamer_DamageTake(airScreamer, Q12(0.6f)))
    {
        case AirScreamerHit_None:
        case AirScreamerHit_Recoil:
        case AirScreamerHit_Rear:
            if (!cond)
            {
                break;
            }

            var_s1 = airScreamerProps.field_E8_4 ? Q12(0.1f) : Q12(0.0f);

            if (airScreamerProps.field_E8_0 == 3)
            {
                sharedFunc_800DD13C_2_s00(airScreamer, airScreamer->field_40 + 1, Q12(0.7f));
            }

            if (field14C_2)
            {
                if (airScreamerProps.field_E8_8 == 5)
                {
                    airScreamer->model.controlState = AirScreamerControl_14;
                    airScreamer->model.stateStep    = 0;

                    if (Rng_TestProbability(Q12_DIV(airScreamer->health, Q12(380.0f)) + (var_s1 * 2)))
                    {
                        airScreamerProps.field_E8_8 = 3;
                    }
                }
                else
                {
                    airScreamer->model.controlState = AirScreamerControl_10;
                    airScreamer->model.stateStep    = 0;

                    // TODO: `Rng_TestProbability` doesn't fit?
                    if (Rng_RandQ12() <= (Q12_DIV(airScreamer->health, Q12(380.0f)) + (var_s1 * 2)))
                    {
                        airScreamerProps.field_E8_8 = 3;
                    }
                    else
                    {
                        airScreamerProps.field_E8_8 = 5;
                    }
                }
                break;
            }

            switch (airScreamerProps.field_E8_8)
            {
                case 3:
                    airScreamer->model.controlState = AirScreamerControl_13;
                    airScreamer->model.stateStep    = 0;

                    if (Rng_TestProbability(Q12(0.5f) - (var_s1 * 3)))
                    {
                        airScreamerProps.field_E8_8 = 2;
                    }
                    break;

                case 2:
                    airScreamer->model.controlState = AirScreamerControl_7;
                    airScreamer->model.stateStep    = 0;

                    // TODO: Does `Rng_TestProbability` fit?
                    if (Rng_RandQ12() > (Q12_DIV(airScreamer->health, Q12(380.0f)) + (var_s1 * 2)))
                    {
                        airScreamerProps.field_E8_8 = 5;
                    }
                    else if (Rng_TestProbability(Q12(0.3f) + (var_s1 * 3)))
                    {
                        airScreamerProps.field_E8_8 = 3;
                    }
                    break;

                case 5:
                    airScreamer->model.controlState = AirScreamerControl_14;
                    airScreamer->model.stateStep    = 0;

                    if (Rng_TestProbability(Q12_DIV(airScreamer->health, Q12(380.0f))))
                    {
                        airScreamerProps.field_E8_8 = 3;
                    }
                    break;

                case 1:
                case 4:
                    if (Rng_TestProbability(Q12(0.2f) + (var_s1 * 5)))
                    {
                        airScreamer->model.controlState = AirScreamerControl_7;
                    }
                    else
                    {
                        airScreamer->model.controlState = AirScreamerControl_14;
                    }

                    airScreamer->model.stateStep = 0;

                    if (Rng_TestProbability(Q12(0.5f) + (var_s1 * 3)))
                    {
                        var_a0 = 2;
                    }
                    else
                    {
                        var_a0 = 5;
                    }

                    airScreamerProps.field_E8_8 = var_a0;
                    break;

                case 0:
                default:
                    if (airScreamerProps.field_E8_0 == 3)
                    {
                        var_s1 = Q12(1.0f);
                    }

                    if (Rng_TestProbability(Q12(0.4f) + (var_s1 * 3)))
                    {
                        airScreamer->model.controlState = AirScreamerControl_7;
                    }
                    else
                    {
                        airScreamer->model.controlState = AirScreamerControl_14;
                    }

                    airScreamer->model.stateStep = 0;

                    if (Rng_TestProbability(Q12(0.6f) + (var_s1 * 2)))
                    {
                        var_a0 = 2;
                    }
                    else
                    {
                        var_a0 = 5;
                    }

                    airScreamerProps.field_E8_8 = var_a0;
                    break;
            }
            break;

        case AirScreamerHit_Stun:
        case AirScreamerHit_Kill:
            airScreamer->model.controlState = AirScreamerControl_Stun;
            airScreamer->model.stateStep    = 0;

            if (airScreamer->health <= Q12(0.0f))
            {
                airScreamerProps.flags |= AirScreamerFlag_6;
            }
            else
            {
                airScreamerProps.flags |= AirScreamerFlag_3;
            }
            break;
    }
#endif
}

void AirScreamer_ControlStun(s_SubCharacter* airScreamer)
{
#ifndef MAP0_S01
    bool cond;
    s32  animStatus;
    u32  stateStep;

    cond = false;

    stateStep              = airScreamer->model.stateStep;
    animStatus             = airScreamer->model.anim.status;
    airScreamer->flags |= CharaFlag_Hit;

    // Handle state step.
    switch (stateStep)
    {
        case 0:
            if (ANIM_STATUS_IS_ACTIVE(animStatus))
            {
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_StandToStun, false);
                airScreamer->model.stateStep   = 1;
            }
            break;

        case 1:
            if (animStatus != ANIM_STATUS(AirScreamerAnim_StandToStun, false))
            {
                airScreamer->model.stateStep = 2;
                airScreamerProps.flags      |= AirScreamerFlag_3;
            }
            break;

        case 2:
            if (animStatus != ANIM_STATUS(AirScreamerAnim_StandToStun, true))
            {
                cond = true;
            }
            break;
    }

    sharedFunc_800DF8A0_2_s00(airScreamer);
    AirScreamer_DamageTake(airScreamer, Q12(0.6f));

    if (cond)
    {
        airScreamer->model.controlState = AirScreamerControl_2;
        airScreamer->model.stateStep    = 0;
        airScreamerProps.field_E8_8         = 1;
    }
#endif
}

void AirScreamer_Control_18(s_SubCharacter* airScreamer)
{
#ifndef MAP0_S01
    s32    animStatus;
    s32    switchCond;
    s32    sp10;
    bool   field14C_0;
    bool   field14C_1;
    bool   field14C_2;
    bool   field14C_0_tmp;
    bool   field14C_1_tmp;
    bool   field14C_2_tmp;
    q19_12 distToTarget;
    q19_12 angleDeltaToTarget;
    s32    temp_s5;
    bool   cond; // TODO: Why does moving this decl cause mismatch?

    animStatus = airScreamer->model.anim.status;
    switchCond = 0;
    sp10       = sharedFunc_800D4A80_0_s01(airScreamer);
    cond       = false;

    field14C_0_tmp = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_0;
    field14C_1_tmp = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_1;
    field14C_2_tmp = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_2;
    field14C_0     = field14C_0_tmp;
    field14C_1     = field14C_1_tmp;
    field14C_2     = field14C_2_tmp;

    temp_s5 = AirScreamer_HasLandedCheck(airScreamer);

    switch (airScreamer->model.stateStep)
    {
        case 64:
            sharedFunc_800DD4EC_2_s00(airScreamer);
            airScreamer->model.stateStep = 1;
            break;

        case 0:
            sharedFunc_800D529C_0_s01(airScreamer, Q12(5.0f), airScreamer->rotation.vy);
            sharedFunc_800DE6A8_2_s00(airScreamer, &airScreamerProps.targetPosition, Q12(4.0f));
            airScreamer->model.stateStep = 1;
            break;

        case 1:
            cond                             = true;
            airScreamerProps.timer_120       = Q12(6.0f);
            airScreamer->model.stateStep = 2;

        case 2:
            distToTarget       = Math_Distance2dGet(&airScreamer->position, &airScreamerProps.targetPosition);
            angleDeltaToTarget = Q12_ANGLE_NORM_S(func_80080478(&airScreamer->position, &airScreamerProps.targetPosition) - airScreamer->rotation.vy);

            if (!temp_s5)
            {
                if ((distToTarget < Q12(1.0f) && (angleDeltaToTarget >= Q12_ANGLE(-5.0f) && angleDeltaToTarget <= Q12_ANGLE(5.0f))) ||
                    (airScreamerProps.flags & AirScreamerFlag_31))
                {
                    airScreamer->model.stateStep = 0;
                }
                else
                {
                    if (!airScreamerProps.timer_120)
                    {
                        airScreamer->model.stateStep = 1;
                    }
                    else if (cond)
                    {
                        if (sharedFunc_800DC200_2_s00(airScreamer) && distToTarget > Q12(7.0f))
                        {
                            airScreamer->model.stateStep = 5;
                        }
                        else if (sharedFunc_800DC3BC_2_s00(airScreamer) && distToTarget < Q12(3.0f) && Rng_TestProbability(Q12(0.7f)))
                        {
                            airScreamer->model.stateStep = 3;
                        }
                    }
                }
            }
            else
            {
                airScreamer->model.stateStep = 3;
            }
            break;

        case 3:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_HoverVariable, true))
            {
                switchCond                           = 1;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_HoverConstant, false);
                airScreamer->model.stateStep   = 4;
            }
            break;

        case 4:
            switchCond = 1;
            break;

        case 5:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_HoverVariable, true))
            {
                switchCond                           = 2;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_HoverToGlide, false);
                airScreamer->model.stateStep   = 6;
            }
            break;

        case 6:
            switchCond = 2;
            break;
    }

    sharedFunc_800D57C8_0_s01(airScreamer);

    switch (AirScreamer_DamageTake(airScreamer, Q12(1.0f)))
    {
        case AirScreamerHit_None:
            switch (switchCond)
            {
                case 0:
                    if ((field14C_1 | field14C_0 | field14C_2) != 0)
                    {
                        airScreamer->model.controlState = AirScreamerControl_20;
                        airScreamer->model.stateStep    = 0;
                    }
                    break;

                case 1:
                    if (animStatus == ANIM_STATUS(AirScreamerAnim_StandIdle, true) ||
                        animStatus == ANIM_STATUS(AirScreamerAnim_WalkForward, true))
                    {
                        if ((field14C_1 | field14C_0 | field14C_2) == 0)
                        {
                            airScreamer->model.controlState = AirScreamerControl_4;
                        }
                        else
                        {
                            airScreamer->model.controlState = AirScreamerControl_6;
                        }

                        airScreamer->model.stateStep = 0;
                        break;
                    }

                    if (AirScreamer_IsGroundedCheck(airScreamer) != false)
                    {
                        airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_StandIdle, false);
                    }
                    break;

                case 2:
                    if (sp10 == 3)
                    {
                        if ((field14C_1 | field14C_0 | field14C_2) == 0)
                        {
                            airScreamer->model.controlState = AirScreamerControl_Glide;
                        }
                        else
                        {
                            airScreamer->model.controlState = AirScreamerControl_35;
                        }

                        airScreamer->model.stateStep = 0;
                    }
                    break;
            }
            break;

        case AirScreamerHit_Recoil:
            airScreamer->model.controlState = AirScreamerControl_29;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            break;

        case AirScreamerHit_Rear:
            airScreamer->model.controlState = AirScreamerControl_30;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            break;

        case AirScreamerHit_Stun:
        case AirScreamerHit_Kill:
            airScreamer->model.controlState = AirScreamerControl_32;
            airScreamer->model.stateStep    = 0;

            if (airScreamer->health <= Q12(0.0f))
            {
                airScreamerProps.flags |= AirScreamerFlag_6;
            }
            else
            {
                airScreamerProps.flags |= AirScreamerFlag_3;
            }
            break;
    }
#endif
}

void AirScreamer_Control_19(s_SubCharacter* airScreamer)
{
#ifndef MAP0_S01
    s32    animStatus;
    bool   field14C_0;
    bool   field14C_1;
    bool   field14C_2;
    bool   new_var4;
    bool   new_var2;
    bool   new_var3;
    q19_12 distFieldF8;
    q19_12 angFieldF8;
    s32    switchCond;
    s32    sp10; // Possibly `bool`.
    s32    sp10_temp;
    bool   cond;
    bool   temp_s5;

    animStatus = airScreamer->model.anim.status;
    switchCond = 0;

    // @hack These flags/bitfields always need weird variable juggling, is there a way to remove it?
    sp10_temp  = sharedFunc_800D4A80_0_s01(airScreamer);
    new_var4   = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_0;
    new_var2   = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_1;
    new_var3   = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_2;
    sp10       = sp10_temp;
    cond       = false;
    field14C_0 = new_var4;
    field14C_1 = new_var2;
    field14C_2 = new_var3;

    temp_s5 = AirScreamer_HasLandedCheck(airScreamer);

    switch (airScreamer->model.stateStep)
    {
        case 0:
            sharedFunc_800DE7E0_2_s00(airScreamer);
            airScreamer->model.stateStep = 1;
            break;

        case 1:
            cond                             = true;
            airScreamerProps.timer_120       = Q12(6.0f);
            airScreamer->model.stateStep = 2;

        case 2:
            distFieldF8 = Math_Distance2dGet(&airScreamer->position, &airScreamerProps.targetPosition);
            angFieldF8  = Q12_ANGLE_NORM_S(func_80080478(&airScreamer->position, &airScreamerProps.targetPosition) - airScreamer->rotation.vy);

            if (!temp_s5)
            {
                if ((distFieldF8 < Q12(0.5f) && (angFieldF8 >= Q12_ANGLE(-5.0f) && angFieldF8 <= Q12_ANGLE(5.0f))) ||
                    (airScreamerProps.flags & AirScreamerFlag_31))
                {
                    airScreamer->model.stateStep = 0;
                }
                else if (airScreamerProps.timer_120 == Q12(0.0f))
                {
                    airScreamer->model.stateStep = 1;
                }
                else if (cond)
                {
                    if (sharedFunc_800DC200_2_s00(airScreamer) && distFieldF8 > Q12(6.0f))
                    {
                        airScreamer->model.stateStep = 5;
                    }
                    else if (sharedFunc_800DC3BC_2_s00(airScreamer) && distFieldF8 < Q12(2.0f) && Rng_TestProbability(Q12(0.5f)))
                    {
                        airScreamer->model.stateStep = 3;
                    }
                }
            }
            else
            {
                airScreamer->model.stateStep = 3;
            }
            break;

        case 3:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_HoverVariable, true))
            {
                switchCond                           = 1;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_HoverConstant, false);
                airScreamer->model.stateStep   = 4;
            }
            break;

        case 4:
            switchCond = 1;
            break;

        case 5:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_HoverVariable, true))
            {
                switchCond                           = 2;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_StandIdleToGlide, false);
                airScreamer->model.stateStep   = 6;
            }
            break;

        case 6:
            switchCond = 2;
            break;
    }

    sharedFunc_800D57C8_0_s01(airScreamer);

    switch (AirScreamer_DamageTake(airScreamer, Q12(1.0f)))
    {
        case AirScreamerHit_None:
            switch (switchCond)
            {
                case 0:
                    if (field14C_1 | field14C_0 | field14C_2)
                    {
                        airScreamer->model.controlState = AirScreamerControl_20;
                        airScreamer->model.stateStep    = 0;
                    }
                    break;

                case 1:
                    if (animStatus == ANIM_STATUS(AirScreamerAnim_StandIdle, true) ||
                        animStatus == ANIM_STATUS(AirScreamerAnim_WalkForward, true))
                    {
                        if (field14C_1 | field14C_0 | field14C_2)
                        {
                            airScreamer->model.controlState = AirScreamerControl_6;
                        }
                        else
                        {
                            airScreamer->model.controlState = AirScreamerControl_5;
                        }

                        airScreamer->model.stateStep = 0;
                    }
                    else if (AirScreamer_IsGroundedCheck(airScreamer))
                    {
                        airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_StandIdle, false);
                    }
                    break;

                case 2:
                    if (sp10 == 3)
                    {
                        if (field14C_1 | field14C_0 | field14C_2)
                        {
                            airScreamer->model.controlState = AirScreamerControl_35;
                        }
                        else
                        {
                            airScreamer->model.controlState = AirScreamerControl_34;
                        }

                        airScreamer->model.stateStep = 0;
                    }
                    break;
            }
            break;

        case AirScreamerHit_Recoil:
            airScreamer->model.controlState = AirScreamerControl_29;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            break;

        case AirScreamerHit_Rear:
            airScreamer->model.controlState = AirScreamerControl_30;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            break;

        case AirScreamerHit_Stun:
        case AirScreamerHit_Kill:
            airScreamer->model.controlState = AirScreamerControl_32;
            airScreamer->model.stateStep    = 0;

            if (airScreamer->health <= Q12(0.0f))
            {
                airScreamerProps.flags |= AirScreamerFlag_6;
            }
            else
            {
                airScreamerProps.flags |= AirScreamerFlag_3;
            }
            break;
    }
#endif
}

void AirScreamer_Control_20(s_SubCharacter* airScreamer)
{
#ifndef MAP0_S01
    s32    animStatus;
    q19_12 angle0;
    bool   field14C_0;
    bool   field14C_1;
    bool   field14C_2;
    bool   field14C_0_tmp;
    bool   field14C_1_tmp;
    bool   field14C_2_tmp;
    bool   temp_a0;
    bool   condModelStateIs2;
    s32    switchCond;
    q19_12 rngAngle0;
    q19_12 angleSubtract;
    q19_12 angle1;
    s32    damageType;

    animStatus = airScreamer->model.anim.status;
    angle0     = sharedData_800E21D0_0_s01.field_15C;

    field14C_0_tmp    = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_0;
    field14C_1_tmp    = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_1;
    field14C_2_tmp    = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_2;
    field14C_0        = field14C_0_tmp;
    field14C_1        = field14C_1_tmp;
    field14C_2        = field14C_2_tmp;
    temp_a0           = AirScreamer_HasLandedCheck(airScreamer);
    condModelStateIs2 = switchCond = 0;

    switch (airScreamer->model.stateStep)
    {
        case 0:
            airScreamerProps.timer_120 = Q12(6.0f);

        case 1:
            sharedFunc_800DE6A8_2_s00(airScreamer, &airScreamerProps.headTargetPosition, Q12(1.5f));

            if (!sharedFunc_800DC3BC_2_s00(airScreamer) || Rng_RandQ12() >= Q12_ANGLE(72.0f))
            {
                airScreamer->model.stateStep = 2;
            }
            else
            {
                airScreamer->model.stateStep = 4;
            }
            break;

        case 2:
            condModelStateIs2 = true;

        case 3:
            if (temp_a0)
            {
                airScreamer->model.stateStep = 4;
                break;
            }

            if (Math_Distance2dGet(&airScreamer->position, &airScreamerProps.targetPosition) < Q12(1.0f))
            {

                airScreamer->model.stateStep = 1;
            }
            else if (!airScreamerProps.timer_120)
            {
                if (condModelStateIs2)
                {
                    switchCond = 1;
                }

                airScreamer->model.stateStep = 0;
            }
            else if (field14C_0 || field14C_1 || field14C_2)
            {
                airScreamer->model.stateStep = 3;

                if (angle0 > (Q12_ANGLE(2.9f)))
                {
                    angle1 = func_80080478(&airScreamer->position, &airScreamerProps.headTargetPosition) - airScreamer->rotation.vy;
                    angle1 = Q12_ANGLE_NORM_S(angle1);
                    if (angle1 < Q12_ANGLE(0.0f))
                    {
                        angle1 += Q12_ANGLE(0.3f);
                    }

                    sharedFunc_800D529C_0_s01(airScreamer, Q12(1.5f), (angle1 >> 2) + airScreamer->rotation.vy);
                    airScreamerProps.timer_120 = Q12(6.0f);
                }
            }
            break;

        case 4:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_HoverVariable, true))
            {
                switchCond                           = 2;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_HoverConstant, false);

                if (field14C_2 || field14C_1)
                {
                    airScreamer->model.stateStep = 7;
                }
                else
                {
                    airScreamer->model.stateStep = 6;
                }
            }
            else if (field14C_2 || field14C_1)
            {
                airScreamer->model.stateStep = 5;
            }
            break;

        case 5:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_HoverVariable, true))
            {
                switchCond                           = 3;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_HoverConstant, false);
                airScreamer->model.stateStep   = 7;
            }
            break;

        case 6:
            switchCond = 3;

            if (field14C_2 || field14C_1)
            {
                airScreamer->model.stateStep = 7;
            }
            else
            {
                switchCond = 2;
            }
            break;

        case 7:
            switchCond = 3;
            break;
    }

    sharedFunc_800D57C8_0_s01(airScreamer);

    damageType = AirScreamer_DamageTake(airScreamer, Q12(1.0f));
    switch (damageType)
    {
        case 0:
            switch (switchCond)
            {
                case 0:
                case 1:
                    if (!field14C_2 && !field14C_1)
                    {
                        if (switchCond == 1)
                        {
                            rngAngle0     = Rng_RandQ12();
                            angleSubtract = Q12_ANGLE(0.0f);

                            if (airScreamerProps.field_E8_8 == switchCond)
                            {
                                angleSubtract = Q12_ANGLE(36.0f);
                            }

                            if (rngAngle0 < (Q12_ANGLE(252.0f) - (angleSubtract * 2)))
                            {
                                airScreamer->model.controlState = AirScreamerControl_28;
                                airScreamer->model.stateStep    = 0;
                            }
                        }
                    }
                    else
                    {
                        airScreamer->model.controlState = AirScreamerControl_21;
                        airScreamer->model.stateStep    = 0;
                    }
                    break;

                case 2:
                    if (animStatus == ANIM_STATUS(AirScreamerAnim_StandIdle, true) ||
                        animStatus == ANIM_STATUS(AirScreamerAnim_WalkForward, true))
                    {
                        airScreamer->model.controlState = AirScreamerControl_6;
                        airScreamer->model.stateStep    = 0;
                    }
                    else if (AirScreamer_IsGroundedCheck(airScreamer))
                    {
                        airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_StandIdle, false);
                    }
                    break;

                case 3:
                    if (animStatus == ANIM_STATUS(AirScreamerAnim_StandIdle, true) ||
                        animStatus == ANIM_STATUS(AirScreamerAnim_WalkForward, true))
                    {
                        airScreamer->model.controlState = AirScreamerControl_7;
                        airScreamer->model.stateStep    = 0;
                    }
                    else if (AirScreamer_IsGroundedCheck(airScreamer))
                    {
                        airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_StandIdle, false);
                    }
                    break;
            }
            break;

        case 1:
            airScreamer->model.controlState = AirScreamerControl_29;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            break;

        case 2:
            airScreamer->model.controlState = AirScreamerControl_30;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            break;

        case 3:
        case 4:
            airScreamer->model.controlState = AirScreamerControl_32;
            airScreamer->model.stateStep    = 0;

            if (airScreamer->health <= Q12(0.0f))
            {
                airScreamerProps.flags |= AirScreamerFlag_6;
            }
            else
            {
                airScreamerProps.flags |= AirScreamerFlag_3;
            }
            break;
    }
#endif
}

void AirScreamer_Control_21(s_SubCharacter* airScreamer)
{
#ifndef MAP0_S01
    s32    switch3;
    bool   condModelStateIs2;
    s32    animStatus;
    q19_12 angle0;
    bool   field14C_0;
    bool   field14C_1;
    bool   field14C_2;
    bool   field14C_0_tmp;
    bool   field14C_1_tmp;
    bool   field14C_2_tmp;
    bool   temp_a0;
    q19_12 rngAngle0;
    q19_12 angleSubtract;
    q19_12 angle1;

    switch3           = 0;
    condModelStateIs2 = false;
    animStatus        = airScreamer->model.anim.status;
    angle0            = sharedData_800E21D0_0_s01.field_15C;

    field14C_0_tmp = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_0;
    field14C_1_tmp = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_1;
    field14C_2_tmp = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_2;
    field14C_0     = field14C_0_tmp;
    field14C_1     = field14C_1_tmp;
    field14C_2     = field14C_2_tmp;
    temp_a0        = AirScreamer_HasLandedCheck(airScreamer);

    // Handle state step.
    switch (airScreamer->model.stateStep)
    {
        case 0:
            airScreamerProps.timer_120 = Q12(6.0f);

        case 1:
            sharedFunc_800DE6A8_2_s00(airScreamer, &airScreamerProps.headTargetPosition, Q12(2.0f));

            if (!sharedFunc_800DC3BC_2_s00(airScreamer) || Rng_RandQ12() >= Q12_ANGLE(36.0f))
            {
                airScreamer->model.stateStep = 2;
            }
            else
            {
                airScreamer->model.stateStep = 4;
            }
            break;

        case 2:
            condModelStateIs2 = true;

        case 3:
            if (temp_a0)
            {
                airScreamer->model.stateStep = 4;
                break;
            }

            if (Math_Distance2dGet(&airScreamer->position, &airScreamerProps.targetPosition) < Q12(1.0f))
            {
                if (condModelStateIs2)
                {
                    switch3 = 1;
                }

                airScreamer->model.stateStep = 1;
            }
            else if (airScreamerProps.timer_120 == Q12(0.0f))
            {
                airScreamer->model.stateStep = 0;
            }
            else if (field14C_2 || field14C_0 || field14C_1)
            {
                airScreamer->model.stateStep = 3;

                if (angle0 > (Q12_ANGLE(2.9f)))
                {
                    angle1 = func_80080478(&airScreamer->position, &airScreamerProps.headTargetPosition) - airScreamer->rotation.vy;
                    angle1 = Q12_ANGLE_NORM_S(angle1);
                    if (angle1 < Q12_ANGLE(0.0f))
                    {
                        angle1 += Q12_ANGLE(0.3f);
                    }

                    sharedFunc_800D529C_0_s01(airScreamer, Q12(2.0f), (angle1 >> 2) + airScreamer->rotation.vy);
                    airScreamerProps.timer_120 = Q12(6.0f);
                }
            }
            break;

        case 4:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_HoverVariable, true))
            {
                switch3                              = 2;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_HoverConstant, false);

                if (field14C_1 || field14C_0)
                {
                    airScreamer->model.stateStep = 7;
                }
                else
                {
                    airScreamer->model.stateStep = 6;
                }
            }
            else if (field14C_2 || field14C_1 || field14C_0)
            {
                airScreamer->model.stateStep = 5;
            }
            break;

        case 5:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_HoverVariable, true))
            {
                switch3                              = 3;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_HoverConstant, false);
                airScreamer->model.stateStep   = 7;
            }
            break;

        case 6:
            switch3 = 3;
            if (field14C_2 || field14C_1 || field14C_0)
            {
                airScreamer->model.stateStep = 7;
            }
            else
            {
                switch3 = 2;
            }
            break;

        case 7:
            switch3 = 3;
            break;
    }

    sharedFunc_800D57C8_0_s01(airScreamer);

    // Handle damage.
    switch (AirScreamer_DamageTake(airScreamer, Q12(1.0f)))
    {
        case AirScreamerHit_None:
            switch (switch3)
            {
                case 0:
                case 1:
                    if (field14C_2 == 0)
                    {
                        if (switch3 == 1)
                        {
                            rngAngle0     = Rng_RandQ12();
                            angleSubtract = Q12_ANGLE(0.0f);

                            if (airScreamerProps.field_E8_8 == switch3)
                            {
                                angleSubtract = Q12_ANGLE(36.0f);
                            }

                            if (rngAngle0 < (Q12_ANGLE(252.0f) - (angleSubtract * 2)))
                            {
                                airScreamer->model.controlState = AirScreamerControl_20;
                                airScreamer->model.stateStep    = 0;
                            }
                        }
                    }
                    else
                    {
                        airScreamer->model.controlState = AirScreamerControl_22;
                        airScreamer->model.stateStep    = 0;
                    }
                    break;

                case 2:
                    if (animStatus == ANIM_STATUS(AirScreamerAnim_StandIdle, true) ||
                        animStatus == ANIM_STATUS(AirScreamerAnim_WalkForward, true))
                    {
                        if (!field14C_2)
                        {
                            airScreamer->model.controlState = AirScreamerControl_6;
                        }
                        else
                        {
                            airScreamer->model.controlState = AirScreamerControl_8;
                        }

                        airScreamer->model.stateStep = 0;
                    }
                    else if (AirScreamer_IsGroundedCheck(airScreamer))
                    {
                        airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_StandIdle, false);
                    }
                    break;

                case 3:
                    if (animStatus == ANIM_STATUS(AirScreamerAnim_StandIdle, true) ||
                        animStatus == ANIM_STATUS(AirScreamerAnim_WalkForward, true))
                    {
                        if (field14C_2)
                        {
                            airScreamer->model.controlState = AirScreamerControl_8;
                        }
                        else
                        {
                            airScreamer->model.controlState = AirScreamerControl_7;
                        }

                        airScreamer->model.stateStep = 0;
                    }
                    else if (AirScreamer_IsGroundedCheck(airScreamer))
                    {
                        airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_StandIdle, false);
                    }
                    break;
            }
            break;

        case AirScreamerHit_Recoil:
            airScreamer->model.controlState = AirScreamerControl_29;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            break;

        case AirScreamerHit_Rear:
            airScreamer->model.controlState = AirScreamerControl_30;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            break;

        case AirScreamerHit_Stun:
        case AirScreamerHit_Kill:
            airScreamer->model.controlState = AirScreamerControl_32;
            airScreamer->model.stateStep    = 0;

            if (airScreamer->health <= Q12(0.0f))
            {
                airScreamerProps.flags |= AirScreamerFlag_6;
            }
            else
            {
                airScreamerProps.flags |= AirScreamerFlag_3;
            }
            break;
    }
#endif
}

void AirScreamer_Control_22(s_SubCharacter* airScreamer)
{
#ifndef MAP0_S01
    s32    switchCond;
    s32    animStatus;
    q19_12 dist0;
    q19_12 angle0;
    bool   field14C_2;
    q19_12 chance0;
    q19_12 chance1;
    bool   temp_s3;
    s32    var_s5;
    q19_12 angleAdd;

    switchCond = 0;
    angleAdd   = Q12_ANGLE(0.0f);
    animStatus = airScreamer->model.anim.status;
    dist0      = sharedData_800E21D0_0_s01.distance_150;
    angle0     = sharedData_800E21D0_0_s01.angle_154;
    field14C_2 = sharedData_800E21D0_0_s01.field_14C.bits.field_14C_2;

    if (airScreamerProps.field_E8_4)
    {
        angleAdd = Q12(0.1f);
    }

    temp_s3 = AirScreamer_HasLandedCheck(airScreamer);
    var_s5  = 0;

    // Handle state step.
    switch (airScreamer->model.stateStep)
    {
        case 0:
            switchCond                 = 1;
            airScreamerProps.timer_120 = Q12(2.0f);
            airScreamer->flags     |= CharaFlag_Hit;

        case 1:
            sharedFunc_800D529C_0_s01(airScreamer, dist0 / 2, angle0);

            if (Rng_TestProbability((angleAdd * 2) + Q12_ANGLE(72.0f)))
            {
                airScreamer->model.stateStep = 2;
            }
            else
            {
                airScreamer->model.stateStep = 3;
            }

        default:
            break;

        case 2:
            var_s5 = 1;

            if (!temp_s3)
            {
                if (airScreamerProps.timer_120 != Q12(0.0f))
                {
                    if (Math_Distance2dGet(&airScreamer->position, &airScreamerProps.targetPosition) < Q12(1.0f))
                    {
                        airScreamer->model.stateStep = 1;
                    }
                }
                else
                {
                    airScreamer->model.stateStep = 0;
                }
            }
            else
            {
                airScreamer->model.stateStep = 4;
            }
            break;

        case 3:
            sharedFunc_800D529C_0_s01(airScreamer, dist0 / 2, angle0);

            if (temp_s3)
            {
                airScreamer->model.stateStep = 4;
            }
            else if (airScreamerProps.timer_120 == Q12(0.0f))
            {
                airScreamer->model.stateStep = 0;
            }
            else if (Q12_ANGLE_NORM_S(angle0 - airScreamer->rotation.vy) >= Q12_ANGLE(-10.0f) &&
                     Q12_ANGLE_NORM_S(angle0 - airScreamer->rotation.vy) < Q12_ANGLE(10.0f))
            {
                var_s5 = 2;
            }
            break;

        case 4:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_HoverVariable, true))
            {
                switchCond                           = 2;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_HoverConstant, false);
                airScreamer->model.stateStep   = 5;
            }
            break;

        case 5:
            switchCond = 2;
            break;
    }

    if (var_s5 == 1)
    {
        sharedFunc_800D57C8_0_s01(airScreamer);
    }
    else
    {
        sharedFunc_800D598C_0_s01(airScreamer);
    }

    switch (AirScreamer_DamageTake(airScreamer, Q12(1.0f)))
    {
        case AirScreamerHit_None:
            switch (switchCond)
            {
                case 0:
                    if (field14C_2 == 0)
                    {
                        airScreamer->model.controlState = AirScreamerControl_21;
                        airScreamer->model.stateStep    = 0;
                    }
                    break;

                case 2:
                    if (animStatus == ANIM_STATUS(AirScreamerAnim_StandIdle, true) ||
                        animStatus == ANIM_STATUS(AirScreamerAnim_WalkForward, true))
                    {
                        airScreamer->model.controlState = AirScreamerControl_8;
                        airScreamer->model.stateStep    = 0;
                    }
                    else if (AirScreamer_IsGroundedCheck(airScreamer))
                    {
                        airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_StandIdle, false);
                    }
                    break;

                case 1:
                    chance0 = sharedFunc_800DC894_2_s00(airScreamer, dist0);
                    chance1 = sharedFunc_800DC6E4_2_s00(airScreamer, dist0);
                    if (Rng_TestProbability(chance0))
                    {
                        if (airScreamerProps.field_E8_0 == 3)
                        {
                            sharedFunc_800DD13C_2_s00(airScreamer, airScreamer->field_40 + 1, Q12(0.6f));
                        }

                        airScreamer->model.controlState = AirScreamerControl_23;
                        airScreamer->model.stateStep    = 0;
                        airScreamerProps.flags         |= AirScreamerFlag_4;
                    }
                    else if (Rng_TestProbability(chance1))
                    {
                        airScreamer->model.controlState = AirScreamerControl_27;
                        airScreamer->model.stateStep    = 0;
                        airScreamerProps.flags         |= AirScreamerFlag_4;
                    }
                    break;
            }
            break;

        case AirScreamerHit_Recoil:
            airScreamer->model.controlState = AirScreamerControl_29;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            break;

        case AirScreamerHit_Rear:
            airScreamer->model.controlState = AirScreamerControl_30;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            break;

        case AirScreamerHit_Stun:
        case AirScreamerHit_Kill:
            airScreamer->model.controlState = AirScreamerControl_32;
            airScreamer->model.stateStep    = 0;

            if (airScreamer->health <= Q12(0.0f))
            {
                airScreamerProps.flags |= AirScreamerFlag_6;
            }
            else
            {
                airScreamerProps.flags |= AirScreamerFlag_3;
            }
            break;
    }
#endif
}

void AirScreamer_Control_23(s_SubCharacter* airScreamer)
{
#ifndef MAP0_S01
    s32    temp_fp;
    s32    temp_fp_tmp;
    bool   field14C_2;
    bool   field14C_2_tmp;
    bool   temp_s6;
    q19_12 dist0;
    q19_12 dist1;
    q19_12 posDiffAngle;
    q19_12 distTest;
    s32    switchCond;
    s32    animStatus;

    animStatus = airScreamer->model.anim.status;
    switchCond = 0;

    temp_fp_tmp    = sharedFunc_800D4A80_0_s01(airScreamer);
    dist0          = sharedData_800E21D0_0_s01.distance_150;
    field14C_2_tmp = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_2;
    temp_fp        = temp_fp_tmp;
    field14C_2     = field14C_2_tmp;
    temp_s6        = AirScreamer_HasLandedCheck(airScreamer);

    sharedFunc_800D53AC_0_s01(airScreamer);
    switch (airScreamer->model.stateStep)
    {
        case 0:
            #define angleDiff Q12_ANGLE_NORM_S(g_SysWork.playerWork.player.rotation.vy - airScreamer->rotation.vy)

            distTest = dist0 + (Rng_RandQ12() * 4);
            if (sharedFunc_800DC200_2_s00(airScreamer) && distTest > Q12(8.0f))
            {
                airScreamer->model.stateStep = 4;
            }
            else if (!sharedFunc_800DC3BC_2_s00(airScreamer) || distTest >= Q12(4.0f) ||
                     (angleDiff < Q12_ANGLE(-30.0f) || angleDiff >= Q12_ANGLE(30.0f)) ||
                     Rng_RandQ12() >= Q12(0.2f))
            {
                airScreamer->model.stateStep = 1;
            }
            else
            {
                airScreamer->model.stateStep = 2;
            }

            airScreamerProps.timer_120 = Q12(4.0f);
            break;

#undef angleDiff

        case 1:
            distTest = NO_VALUE;
            if (airScreamerProps.targetPosition.vy - airScreamer->position.vy > Q12(-0.2f) &&
                airScreamerProps.targetPosition.vy - airScreamer->position.vy < Q12(0.8f))
            {
                distTest = airScreamer->collision.cylinder.radius + Q12(0.15f);
            }

            dist1        = Math_Distance2dGet(&airScreamer->position, &airScreamerProps.targetPosition);
            posDiffAngle = Q12_ANGLE_NORM_S(func_80080478(&airScreamer->position, &airScreamerProps.targetPosition) - airScreamer->rotation.vy);

            if (temp_s6)
            {
                airScreamer->model.stateStep = 2;
            }
            else if (dist1 < distTest && (posDiffAngle >= Q12_ANGLE(-22.5f) && posDiffAngle < Q12_ANGLE(22.5f)))
            {
                switchCond = 1;
            }
            else if (airScreamerProps.timer_120 == Q12(0.0f))
            {
                airScreamer->model.stateStep = 0;
            }
            break;

        case 2:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_HoverVariable, true))
            {
                switchCond                           = 2;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_HoverConstant, false);
                airScreamer->model.stateStep   = 3;
            }
            break;

        case 3:
            switchCond = 2;
            break;

        case 4:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_HoverVariable, true))
            {
                switchCond                           = 3;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_HoverToGlide, false);
                airScreamer->model.stateStep   = 5;
            }
            break;

        case 5:
            switchCond = 3;
            break;
    }

    sharedFunc_800D57C8_0_s01(airScreamer);

    switch (AirScreamer_DamageTake(airScreamer, Q12(1.0f)))
    {
        case AirScreamerHit_None:
            switch (switchCond)
            {
                case 0:
                    if (!sharedFunc_800DC67C_2_s00(airScreamer))
                    {
                        if (field14C_2 == 0)
                        {
                            airScreamer->model.controlState = AirScreamerControl_26;
                            airScreamer->model.stateStep    = 0;
                        }
                    }
                    else
                    {
                        airScreamer->model.controlState = AirScreamerControl_27;
                        airScreamer->model.stateStep    = 0;
                    }
                    break;

                case 1:
                    if (!Chara_HasFlag(&g_SysWork.playerWork.player, CharaFlag_Unk4) &&
                        g_SysWork.npcIdxs[0] == NO_VALUE &&
                        g_SysWork.npcIdxs[1] == NO_VALUE)
                    {
                        if (animStatus == ANIM_STATUS(AirScreamerAnim_HoverVariable, true))
                        {
                            airScreamer->model.controlState = AirScreamerControl_25;
                            airScreamer->model.stateStep    = 0;
                        }
                    }
                    else
                    {
                        airScreamer->model.controlState = AirScreamerControl_24;
                        airScreamer->model.stateStep    = 0;
                    }
                    break;

                case 2:
                    if (animStatus == ANIM_STATUS(AirScreamerAnim_StandIdle, true) ||
                        animStatus == ANIM_STATUS(AirScreamerAnim_WalkForward, true))
                    {
                        sharedFunc_800DE11C_2_s00(airScreamer);
                        airScreamer->model.controlState = AirScreamerControl_10;
                        airScreamer->model.stateStep    = 0;
                    }
                    else if (AirScreamer_IsGroundedCheck(airScreamer))
                    {
                        airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_StandIdle, false);
                    }
                    break;

                case 3:
                    if (temp_fp == switchCond)
                    {
                        airScreamer->model.controlState = AirScreamerControl_38;
                        airScreamer->model.stateStep    = 0;
                    }
                    break;
            }
            break;

        case AirScreamerHit_Recoil:
            airScreamer->model.controlState = AirScreamerControl_29;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            break;

        case AirScreamerHit_Rear:
            airScreamer->model.controlState = AirScreamerControl_30;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            break;

        case AirScreamerHit_Stun:
        case AirScreamerHit_Kill:
            airScreamer->model.controlState = AirScreamerControl_32;
            airScreamer->model.stateStep    = 0;

            if (airScreamer->health <= Q12(0.0f))
            {
                airScreamerProps.flags |= AirScreamerFlag_6;
            }
            else
            {
                airScreamerProps.flags |= AirScreamerFlag_3;
            }
            break;
    }
#endif
}

void AirScreamer_Control_24(s_SubCharacter* airScreamer)
{
#ifndef MAP0_S01
    bool temp_a0;
    bool unkField14C;
    s32  unkField150;
    s32  switchCond;
    s32  animStatus;

    switchCond  = 0;
    animStatus  = airScreamer->model.anim.status;
    unkField150 = sharedData_800E21D0_0_s01.distance_150;
    unkField14C = sharedData_800E21D0_0_s01.field_14C.bits.field_14C_2;
    temp_a0     = AirScreamer_HasLandedCheck(airScreamer);

    switch (airScreamer->model.stateStep)
    {
        case 0:
            airScreamerProps.timer_120 = Q12(2.0f);

            if (Rng_TestProbability(Q12(0.7f)))
            {
                switchCond = 2;
                sharedFunc_800D529C_0_s01(airScreamer, Q12(8.0f), airScreamer->rotation.vy);
                airScreamer->model.stateStep = 4;
            }
            else
            {
                airScreamer->model.stateStep = 1;
            }
            break;

        case 1:
            if (temp_a0)
            {
                airScreamer->model.stateStep = 2;
            }
            break;

        case 4:
            switchCond = 2;

            if (temp_a0)
            {
                airScreamer->model.stateStep = 2;
            }
            else if (airScreamer->moveSpeed > Q12(1.5f))
            {
                airScreamer->model.stateStep = 5;
            }
            else if (Math_Distance2dGet(&airScreamer->position, &airScreamerProps.targetPosition) < Q12(3.0f) ||
                     airScreamerProps.timer_120 == Q12(0.0f))
            {
                airScreamer->model.stateStep = 1;
            }
            break;

        case 2:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_HoverVariable, true))
            {
                switchCond                           = 1;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_HoverConstant, false);
                airScreamer->model.stateStep   = 3;
            }
            break;

        case 3:
            switchCond = 1;
            break;

        case 5:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_HoverVariable, true))
            {
                switchCond                           = 3;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_HoverToGlide, false);
                airScreamer->model.stateStep   = 6;
            }
            break;

        case 6:
            switchCond = 3;
            break;
    }

    if (switchCond >= 2 && switchCond < 4)
    {
        sharedFunc_800D57C8_0_s01(airScreamer);
    }
    else
    {
        sharedFunc_800DEBCC_2_s00(airScreamer);
        sharedFunc_800D5B10_0_s01(airScreamer);
    }

    switch (AirScreamer_DamageTake(airScreamer, Q12(1.0f)))
    {
        case AirScreamerHit_None:
            switch (switchCond)
            {
                case 0:
                    if (airScreamerProps.timer_120 == Q12(0.0f) || unkField150 > Q12(2.0f))
                    {
                        if (!unkField14C)
                        {
                            airScreamer->model.controlState = AirScreamerControl_26;
                        }
                        else
                        {
                            airScreamer->model.controlState = AirScreamerControl_23;
                        }

                        airScreamer->model.stateStep = 0;
                    }
                    break;

                case 1:
                    if (animStatus == ANIM_STATUS(AirScreamerAnim_StandIdle, true) ||
                        animStatus == ANIM_STATUS(AirScreamerAnim_WalkForward, true))
                    {
                        if (!unkField14C)
                        {
                            airScreamer->model.controlState = AirScreamerControl_13;
                        }
                        else
                        {
                            airScreamer->model.controlState = AirScreamerControl_11;
                        }

                        airScreamer->model.stateStep = 0;
                    }
                    else if (AirScreamer_IsGroundedCheck(airScreamer))
                    {
                        airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_StandIdle, false);
                    }
                    break;

                case 2:
                    break;

                case 3:
                    if (sharedFunc_800D4A80_0_s01(airScreamer) == switchCond)
                    {
                        airScreamer->model.controlState = AirScreamerControl_39;
                        airScreamer->model.stateStep    = 0;
                    }
                    break;
            }
            break;

        case AirScreamerHit_Recoil:
            airScreamer->model.controlState = AirScreamerControl_29;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            break;

        case AirScreamerHit_Rear:
            airScreamer->model.controlState = AirScreamerControl_30;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            break;

        case AirScreamerHit_Stun:
        case AirScreamerHit_Kill:
            airScreamer->model.controlState = AirScreamerControl_32;
            airScreamer->model.stateStep    = 0;

            if (airScreamer->health <= Q12(0.0f))
            {
                airScreamerProps.flags |= AirScreamerFlag_6;
            }
            else
            {
                airScreamerProps.flags |= AirScreamerFlag_3;
            }
            break;
    }
#endif
}

void AirScreamer_Control_25(s_SubCharacter* airScreamer)
{
#ifndef MAP0_S01
    bool var_s1;
    bool var_s2;
    s32  animStatus;

    var_s1     = false;
    var_s2     = false;
    animStatus = airScreamer->model.anim.status;

    switch (airScreamer->model.stateStep)
    {
        case 0:
            airScreamer->model.stateStep = 1;

        case 1:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_HoverVariable, true))
            {
                airScreamer->model.stateStep   = 2;
                airScreamer->model.anim.status = 4;
            }
            break;

        case 2:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_HoverBiteAttack, true) &&
                airScreamer->model.anim.keyframeIdx < 8192)
            {
                airScreamer->model.stateStep = 3;
                airScreamerProps.flags      |= 1 << 5;
            }
            break;

        case 3:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_HoverVariable, true))
            {
                var_s1 = true;
            }
            else if (airScreamer->field_44.field_8 == NO_VALUE)
            {
                var_s1                           = true;
                var_s2                           = true;
                airScreamer->model.stateStep = 4;
            }
            break;

        case 4:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_HoverVariable, true))
            {
                var_s1 = true;
                var_s2 = true;
            }
            break;
    }

    sharedFunc_800D53AC_0_s01(airScreamer);
    sharedFunc_800D5C90_0_s01(airScreamer);

    switch (AirScreamer_DamageTake(airScreamer, Q12(1.2f)))
    {
        case AirScreamerHit_None:
            if (var_s1)
            {
                if (airScreamerProps.field_E8_8 == 3)
                {
                    airScreamerProps.field_E8_8 = 2;

                    if (var_s2 && Rng_TestProbability(Q12(0.5f)))
                    {
                        airScreamer->model.controlState = AirScreamerControl_24;
                    }
                    else
                    {
                        airScreamer->model.controlState = AirScreamerControl_23;
                    }
                }
                else
                {
                    if (var_s2)
                    {
                        airScreamer->model.controlState = AirScreamerControl_24;
                    }
                    else if (Rng_TestProbability(Q12(0.5f)))
                    {
                        airScreamer->model.controlState = AirScreamerControl_24;
                    }
                    else
                    {
                        airScreamer->model.controlState = AirScreamerControl_23;
                    }
                }
                airScreamer->model.stateStep = 0;
            }
            break;

        case AirScreamerHit_Recoil:
            airScreamer->model.controlState = AirScreamerControl_29;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= 1 << 3;
            break;

        case AirScreamerHit_Rear:
            airScreamer->model.controlState = AirScreamerControl_30;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= 1 << 3;
            break;

        case AirScreamerHit_Stun:
        case AirScreamerHit_Kill:
            airScreamer->model.controlState = AirScreamerControl_32;
            airScreamer->model.stateStep    = 0;
            if (airScreamer->health <= 0)
            {
                airScreamerProps.flags |= 1 << 6;
            }
            else
            {
                airScreamerProps.flags |= 1 << 3;
            }
            break;
    }
#endif
}

void AirScreamer_Control_26(s_SubCharacter* airScreamer)
{
#ifndef MAP0_S01
    s32  animStatus;
    bool field14C_2;
    bool field14C_1;
    bool new_var;
    bool new_var2;
    bool temp_a0;
    bool switchCond;

    switchCond = false;
    animStatus = airScreamer->model.anim.status;

    new_var    = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_1;
    new_var2   = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_2;
    field14C_1 = new_var;
    field14C_2 = new_var2;

    temp_a0 = AirScreamer_HasLandedCheck(airScreamer);

    // Handle state step.
    switch (airScreamer->model.stateStep)
    {
        case 0:
            sharedFunc_800DEBCC_2_s00(airScreamer);

        case 1:
            airScreamerProps.timer_120       = Q12(4.0f);
            airScreamer->model.stateStep = 2;
            break;

        case 2:
            if (temp_a0)
            {
                airScreamer->model.stateStep = 3;
            }
            else if (Math_Distance2dGet(&airScreamer->position, &airScreamerProps.targetPosition) < Q12(0.5f))
            {
                sharedFunc_800DE6A8_2_s00(airScreamer, &airScreamerProps.headTargetPosition, Q12(2.5f));
            }
            else if (airScreamerProps.timer_120 == Q12(0.0f))
            {
                sharedFunc_800DE6A8_2_s00(airScreamer, &airScreamerProps.headTargetPosition, Q12(2.5f));
                airScreamer->model.stateStep = 1;
            }
            break;

        case 3:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_HoverVariable, true))
            {
                switchCond                           = true;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_HoverConstant, false);
                airScreamer->model.stateStep   = 4;
            }
            break;

        case 4:
            switchCond = true;
            break;
    }

    sharedFunc_800D57C8_0_s01(airScreamer);

    // Handle damage.
    switch (AirScreamer_DamageTake(airScreamer, Q12(1.0f)))
    {
        case AirScreamerHit_None:
            switch (switchCond)
            {
                case false:
                    if (sharedFunc_800DC67C_2_s00(airScreamer))
                    {
                        airScreamer->model.controlState = AirScreamerControl_27;
                    }
                    else if (!field14C_2)
                    {
                        if (airScreamerProps.timer_120 == Q12(0.0f) && !field14C_1 &&
                            Rng_TestProbability(Q12(0.5f)))
                        {
                            airScreamer->model.controlState = AirScreamerControl_21;
                        }
                        else
                        {
                            break;
                        }
                    }
                    else
                    {
                        airScreamer->model.controlState = AirScreamerControl_23;
                    }

                    airScreamer->model.stateStep = 0;
                    break;

                case true:
                    if (animStatus == ANIM_STATUS(AirScreamerAnim_StandIdle, true) ||
                        animStatus == ANIM_STATUS(AirScreamerAnim_WalkForward, true))
                    {
                        if (field14C_2)
                        {
                            airScreamer->model.controlState = AirScreamerControl_10;
                        }
                        else
                        {
                            airScreamer->model.controlState = AirScreamerControl_13;
                        }

                        airScreamer->model.stateStep = 0;
                    }
                    else if (AirScreamer_IsGroundedCheck(airScreamer))
                    {
                        airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_StandIdle, false);
                    }
                    break;
            }
            break;

        case AirScreamerHit_Recoil:
            airScreamer->model.controlState = AirScreamerControl_29;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            break;

        case AirScreamerHit_Rear:
            airScreamer->model.controlState = AirScreamerControl_30;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            break;

        case AirScreamerHit_Stun:
        case AirScreamerHit_Kill:
            airScreamer->model.controlState = AirScreamerControl_32;
            airScreamer->model.stateStep    = 0;

            if (airScreamer->health <= Q12(0.0f))
            {
                airScreamerProps.flags |= AirScreamerFlag_6;
            }
            else
            {
                airScreamerProps.flags |= AirScreamerFlag_3;
            }
            break;
    }
#endif
}

void AirScreamer_Control_27(s_SubCharacter* airScreamer)
{
#ifndef MAP0_S01
    s32    animStatus;
    s32    switchCond;
    s32    temp_fp;
    bool   field14C_2;
    bool   field14C_2_tmp;
    q19_12 dist;
    q19_12 angle;
    q19_12 distFieldF8;
    s32    var_s6; // Possibly `bool`
    bool   cond;
    q19_12 angleDiff;

    animStatus     = airScreamer->model.anim.status;
    switchCond     = 0;
    temp_fp        = sharedFunc_800D4A80_0_s01(airScreamer);
    field14C_2_tmp = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_2;
    dist           = sharedData_800E21D0_0_s01.distance_150;
    angle          = sharedData_800E21D0_0_s01.angle_154;
    field14C_2     = field14C_2_tmp;

    distFieldF8 = Math_Distance2dGet(&airScreamer->position, &airScreamerProps.targetPosition);
    var_s6      = 0;
    cond        = AirScreamer_HasLandedCheck(airScreamer);

    // Handle state step.
    switch (airScreamer->model.stateStep)
    {
        case 0:
            var_s6      = sharedFunc_800DEC64_2_s00(airScreamer);
            distFieldF8 = Math_Distance2dGet(&airScreamer->position, &airScreamerProps.targetPosition);
            angleDiff   = Q12_ANGLE_NORM_S(angle - airScreamer->rotation.vy);

            if (sharedFunc_800DC200_2_s00(airScreamer) && airScreamer->moveSpeed > Q12(1.5f) &&
                (Math_CheckSignedRange(angleDiff, Q12_ANGLE(120.0f)) || ((dist < Q12(5.0f)) && Math_CheckSignedRange(angleDiff, Q12_ANGLE(60.0f)))))
            {
                airScreamer->model.stateStep = 4;
            }
            else
            {
                airScreamerProps.timer_120 = Q12(2.0f);

                airScreamer->model.stateStep = 1;
            }
            break;

        case 1:
            if (cond)
            {
                airScreamer->model.stateStep = 2;
            }
            else if (distFieldF8 < Q12(2.0f) || (airScreamerProps.flags & AirScreamerFlag_31))
            {
                var_s6 = sharedFunc_800DEC64_2_s00(airScreamer);
            }
            else if (airScreamerProps.timer_120 == Q12(0.0f))
            {
                airScreamer->model.stateStep = 0;
            }
            break;

        case 2:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_HoverVariable, true))
            {
                switchCond                           = 1;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_HoverConstant, false);
                airScreamer->model.stateStep   = 3;
            }
            break;

        case 3:
            switchCond = 1;
            break;

        case 4:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_HoverVariable, true))
            {
                switchCond                           = 2;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_HoverToGlide, false);
                airScreamer->model.stateStep   = 5;
            }
            break;

        case 5:
            switchCond = 2;
            break;
    }

    sharedFunc_800D57C8_0_s01(airScreamer);

    // Handle damage.
    switch (AirScreamer_DamageTake(airScreamer, Q12(1.0f)))
    {
        case AirScreamerHit_None:
            switch (switchCond)
            {
                case 0:
                    if (var_s6 || ((field14C_2 && dist < Q12(15.0f)) || dist < Q12(4.0f)) && distFieldF8 < Q12(1.5f))
                    {
                        airScreamer->model.controlState = AirScreamerControl_23;
                        airScreamer->model.stateStep    = 0;
                        airScreamerProps.field_E8_8         = 3;
                        airScreamerProps.flags         |= AirScreamerFlag_4;
                    }
                    else if (dist > Q12(30.0f) && distFieldF8 < Q12(1.0f))
                    {
                        airScreamer->model.controlState = AirScreamerControl_21;
                        airScreamer->model.stateStep    = 0;
                    }
                    break;

                case 1:
                    if (animStatus == ANIM_STATUS(AirScreamerAnim_StandIdle, true) || animStatus == ANIM_STATUS(AirScreamerAnim_WalkForward, true))
                    {
                        airScreamer->model.controlState = AirScreamerControl_14;
                        airScreamer->model.stateStep    = 0;
                    }
                    else if (AirScreamer_IsGroundedCheck(airScreamer))
                    {
                        airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_StandIdle, false);
                    }
                    break;

                case 2:
                    if (temp_fp == 3)
                    {
                        airScreamer->model.controlState = AirScreamerControl_42;
                        airScreamer->model.stateStep    = 0;
                    }
                    break;
            }
            break;

        case AirScreamerHit_Recoil:
            airScreamer->model.controlState = AirScreamerControl_29;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            break;

        case AirScreamerHit_Rear:
            airScreamer->model.controlState = AirScreamerControl_30;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            break;

        case AirScreamerHit_Stun:
        case AirScreamerHit_Kill:
            airScreamer->model.controlState = AirScreamerControl_32;
            airScreamer->model.stateStep    = 0;

            if (airScreamer->health <= Q12(0.0f))
            {
                airScreamerProps.flags |= AirScreamerFlag_6;
            }
            else
            {
                airScreamerProps.flags |= AirScreamerFlag_3;
            }
            break;
    }
#endif
}

void AirScreamer_Control_28(s_SubCharacter* airScreamer)
{
#ifndef MAP0_S01
    sharedFunc_800D529C_0_s01(airScreamer, Q12(0.5f), airScreamer->rotation.vy);
    sharedFunc_800D598C_0_s01(airScreamer);
    airScreamer->flags &= ~CharaFlag_Hit;

    switch (AirScreamer_DamageTake(airScreamer, Q12(1.0f)))
    {
        case AirScreamerHit_None:
            // @hack Explicit `u32` cast needed for match.
            switch ((u32)airScreamerProps.field_E8_0)
            {
                case 0:
                    airScreamer->model.controlState = AirScreamerControl_18;
                    airScreamer->model.stateStep    = 0;
                    break;

                case 1:
                case 3:
                    airScreamer->model.controlState = AirScreamerControl_19;
                    airScreamer->model.stateStep    = 0;
                    break;
            }
            break;

        case AirScreamerHit_Recoil:
            airScreamer->model.controlState = AirScreamerControl_29;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            return;

        case AirScreamerHit_Rear:
            airScreamer->model.controlState = AirScreamerControl_30;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            break;

        case AirScreamerHit_Stun:
        case AirScreamerHit_Kill:
            airScreamer->model.controlState = AirScreamerControl_32;
            airScreamer->model.stateStep    = 0;

            if (airScreamer->health <= Q12(0.0f))
            {
                airScreamerProps.flags |= AirScreamerFlag_6;
            }
            else
            {
                airScreamerProps.flags |= AirScreamerFlag_3;
            }
            break;
    }
#endif
}

void AirScreamer_Control_29(s_SubCharacter* airScreamer)
{
#ifndef MAP0_S01
    s32  temp_s2;
    s32  var_a0;
    s32  var_s1;
    s32  var_s1_2;
    u32  animStatus;
    u32  stateStep;
    bool new_var;

    animStatus             = airScreamer->model.anim.status;
    new_var                = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_2;
    var_s1                 = 0;
    temp_s2                = new_var;
    stateStep              = airScreamer->model.stateStep;
    airScreamer->flags |= CharaFlag_Hit;

    // Handle state step.
    switch (stateStep)
    {
        case 0:
            if (ANIM_STATUS_IS_ACTIVE(animStatus))
            {
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_HoverRecoil, false);
                airScreamer->model.stateStep   = 1;
            }
            break;

        case 1:
            if (animStatus != ANIM_STATUS(AirScreamerAnim_HoverRecoil, false))
            {
                airScreamer->model.stateStep = 2;
                airScreamerProps.flags      |= AirScreamerFlag_3;
            }
            break;

        case 2:
            if (animStatus != ANIM_STATUS(AirScreamerAnim_HoverRecoil, true))
            {
                var_s1 = 1;
            }
            break;
    }

    sharedFunc_800D5D80_0_s01(airScreamer);

    // Handle damage.
    switch (AirScreamer_DamageTake(airScreamer, Q12(0.6f)))
    {
        case AirScreamerHit_None:
        case AirScreamerHit_Recoil:
            if (var_s1 != 0)
            {
                var_s1_2 = airScreamerProps.field_E8_4 ? Q12(0.1f) : Q12(0.0f);

                if (airScreamerProps.field_E8_0 == 3)
                {
                    sharedFunc_800DD13C_2_s00(airScreamer, airScreamer->field_40 + 1, Q12(0.7f));
                }

                if (temp_s2 != 0)
                {
                    if (airScreamerProps.field_E8_8 == 5)
                    {
                        airScreamer->model.controlState = AirScreamerControl_27;
                        airScreamer->model.stateStep    = 0;

                        if (Rng_TestProbability(Q12_DIV(airScreamer->health, Q12(380.0f)) + (var_s1_2 * 2)))
                        {
                            airScreamerProps.field_E8_8 = 3;
                        }
                        break;
                    }

                    airScreamer->model.controlState = AirScreamerControl_23;
                    airScreamer->model.stateStep    = 0;

                    if (Rng_RandQ12() > (Q12_DIV(airScreamer->health, Q12(380.0f)) + (var_s1_2 * 2)))
                    {
                        airScreamerProps.field_E8_8 = 5;
                    }
                    else
                    {
                        airScreamerProps.field_E8_8 = 3;
                    }
                    break;
                }

                switch (airScreamerProps.field_E8_8)
                {
                    case 3:
                        airScreamer->model.controlState = AirScreamerControl_26;
                        airScreamer->model.stateStep    = 0;

                        if (Rng_TestProbability(Q12(0.5f) - (var_s1_2 * 3)))
                        {
                            airScreamerProps.field_E8_8 = 2;
                        }
                        break;

                    case 2:
                        airScreamer->model.controlState = AirScreamerControl_21;
                        airScreamer->model.stateStep    = 0;

                        // TODO: Will `Rng_TestProbability` fit?
                        if (Rng_RandQ12() > (Q12_DIV(airScreamer->health, Q12(380.0f)) + (var_s1_2 * 2)))
                        {
                            airScreamerProps.field_E8_8 = 5;
                        }
                        else if (Rng_TestProbability((var_s1_2 * 3) + Q12(0.3f)))
                        {
                            airScreamerProps.field_E8_8 = 3;
                        }
                        break;

                    case 5:
                        airScreamer->model.controlState = AirScreamerControl_27;
                        airScreamer->model.stateStep    = 0;

                        if (Rng_TestProbability(Q12_DIV(airScreamer->health, Q12(380.0f))))
                        {
                            airScreamerProps.field_E8_8 = 3;
                        }
                        break;

                    case 1:
                    case 4:
                        // TODO: Will `Rng_TestProbability` fit?
                        if (Rng_RandQ12() >= ((var_s1_2 * 5) + Q12(0.2f)))
                        {
                            airScreamer->model.controlState = AirScreamerControl_27;
                        }
                        else
                        {
                            airScreamer->model.controlState = AirScreamerControl_21;
                        }

                        airScreamer->model.stateStep = 0;

                        if (Rng_TestProbability((var_s1_2 * 3) + Q12(0.5f)))
                        {
                            var_a0 = 2;
                        }
                        else
                        {
                            var_a0 = 5;
                        }

                        airScreamerProps.field_E8_8 = var_a0;
                        break;

                    default:
                    case 0:
                        if (airScreamerProps.field_E8_0 == 3)
                        {
                            var_s1_2 = Q12(1.0f);
                        }

                        if (Rng_RandQ12() >= ((var_s1_2 * 3) + Q12(0.4f)))
                        {
                            airScreamer->model.controlState = AirScreamerControl_27;
                        }
                        else
                        {
                            airScreamer->model.controlState = AirScreamerControl_21;
                        }

                        airScreamer->model.stateStep = 0;

                        if (Rng_TestProbability((var_s1_2 * 2) + Q12(0.6f)))
                        {
                            var_a0 = 2;
                        }
                        else
                        {
                            var_a0 = 5;
                        }

                        airScreamerProps.field_E8_8 = var_a0;
                        break;
                }
            }
            break;

        case AirScreamerHit_Rear:
            airScreamer->model.controlState = AirScreamerControl_30;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            break;

        case AirScreamerHit_Stun:
        case AirScreamerHit_Kill:
            airScreamer->model.controlState = AirScreamerControl_32;
            airScreamer->model.stateStep    = 0;

            if (airScreamer->health <= Q12(0.0f))
            {
                airScreamerProps.flags |= AirScreamerFlag_6;
            }
            else
            {
                airScreamerProps.flags |= AirScreamerFlag_3;
            }
            break;
    }
#endif
}

void AirScreamer_Control_30(s_SubCharacter* airScreamer)
{
#ifndef MAP0_S01
    bool cond;
    s32  animStatus;

    cond                = false;
    animStatus          = airScreamer->model.anim.status;
    airScreamer->flags |= CharaFlag_Hit;

    switch ((u32)airScreamer->model.stateStep)
    {
        case 0:
            if (ANIM_STATUS_IS_ACTIVE(animStatus))
            {
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_HoverInjuredBiteAttack, false);
                airScreamer->model.stateStep   = 1;
            }
            break;

        case 1:
            if (animStatus != ANIM_STATUS(AirScreamerAnim_HoverInjuredBiteAttack, false))
            {
                airScreamer->model.stateStep = 2;
                airScreamerProps.flags      |= AirScreamerFlag_3;
            }
            break;

        case 2:
            if (animStatus != ANIM_STATUS(AirScreamerAnim_HoverInjuredBiteAttack, true))
            {
                cond = true;
            }
            break;
    }

    sharedFunc_800D5E14_0_s01(airScreamer);

    switch (AirScreamer_DamageTake(airScreamer, Q12(0.6f)))
    {
        case AirScreamerHit_None:
        case AirScreamerHit_Recoil:
        case AirScreamerHit_Rear:
            if (cond)
            {
                airScreamer->model.controlState = AirScreamerControl_31;
                airScreamer->model.stateStep    = 0;
            }
            break;

        case AirScreamerHit_Stun:
        case AirScreamerHit_Kill:
            airScreamer->model.controlState = AirScreamerControl_32;
            airScreamer->model.stateStep    = 0;

            if (airScreamer->health <= Q12(0.0f))
            {
                airScreamerProps.flags |= AirScreamerFlag_6;
            }
            else
            {
                airScreamerProps.flags |= AirScreamerFlag_3;
            }
            break;
    }
#endif
}

void AirScreamer_Control_31(s_SubCharacter* airScreamer)
{
#ifndef MAP0_S01
    switch (airScreamer->model.stateStep)
    {
        case 0:
            airScreamerProps.timer_120 = (Rng_RandQ12() * 2) + Q12(0.5f);

            sharedFunc_800D529C_0_s01(airScreamer,
                                      Q12(4.0f),
                                      airScreamer->rotation.vy + Q12_MULT_PRECISE(Rng_RandQ12() - Q12_ANGLE(180.0f), Q12_ANGLE(45.0f)));

            airScreamer->model.stateStep = 1;
            break;

        case 1:
            if (airScreamerProps.timer_120 == Q12(0.0f) ||
                Math_Distance2dGet(&airScreamer->position, &airScreamerProps.targetPosition) < Q12(2.0f))
            {
                airScreamer->model.stateStep = 0;
            }
            break;
    }

    sharedFunc_800E012C_2_s00(airScreamer);

    switch (AirScreamer_DamageTake(airScreamer, Q12(0.6f)))
    {
        case AirScreamerHit_None:
            break;

        case AirScreamerHit_Recoil:
        case AirScreamerHit_Rear:
            airScreamerProps.flags |= AirScreamerFlag_3;
            break;

        case AirScreamerHit_Stun:
        case AirScreamerHit_Kill:
            airScreamer->model.controlState = AirScreamerControl_32;
            airScreamer->model.stateStep    = 0;
            if (airScreamer->health <= Q12(0.0f))
            {
                airScreamerProps.flags |= AirScreamerFlag_6;
            }
            else
            {
                airScreamerProps.flags |= AirScreamerFlag_3;
            }
            break;
    }
#endif
}

void AirScreamer_Control_32(s_SubCharacter* airScreamer)
{
#ifndef MAP0_S01
    bool cond;
    s32  animStatus;

    cond       = false;
    animStatus = airScreamer->model.anim.status;

    airScreamer->flags |= CharaFlag_Hit;

    switch ((u32)airScreamer->model.stateStep)
    {
        case 0:
            if (ANIM_STATUS_IS_ACTIVE(animStatus))
            {
                if (animStatus == ANIM_STATUS(HarryAnim_RunRightStumble, true) ||
                    animStatus == ANIM_STATUS(HarryAnim_RunForwardWallStopRight, true))
                {
                    airScreamer->model.anim.status = ANIM_STATUS(HarryAnim_RunRightWallStop, false);
                }
                else
                {
                    airScreamer->model.anim.status = ANIM_STATUS(HarryAnim_RunForwardWallStopLeft, false);
                }

                airScreamer->model.stateStep = 1;
            }
            break;

        case 1:
            if (animStatus != ANIM_STATUS(HarryAnim_RunForwardWallStopLeft, false) &&
                animStatus != ANIM_STATUS(HarryAnim_RunRightWallStop, false))
            {
                airScreamer->model.stateStep             = 2;
                airScreamerProps.flags                  |= AirScreamerFlag_3;
            }
            break;

        case 2:
            if (animStatus != ANIM_STATUS(HarryAnim_RunForwardWallStopLeft, true) &&
                animStatus != ANIM_STATUS(HarryAnim_RunRightWallStop, true))
            {
                cond = true;
            }
            break;
    }

    sharedFunc_800D5E14_0_s01(airScreamer);
    AirScreamer_DamageTake(airScreamer, Q12(0.6f));

    if (cond)
    {
        airScreamer->model.controlState = AirScreamerControl_2;
        airScreamer->model.stateStep    = 0;

        airScreamerProps.field_E8_8 = 1;
    }
#endif
}

void AirScreamer_ControlGlide(s_SubCharacter* airScreamer)
{
#ifndef MAP0_S01
    s32  animStatus;
    s32  switchCond;
    s32  sp10;
    bool cond;
    bool field14C_0;
    bool field14C_1;
    bool field14C_2;
    bool field14C_0_tmp;
    bool field14C_1_tmp;
    bool field14C_2_tmp;
    s32  temp_s4;
    s32  distToTarget;
    s32  angleDeltaToTarget;

    animStatus = airScreamer->model.anim.status;
    switchCond = 0;
    sp10       = sharedFunc_800D4A80_0_s01(airScreamer);
    cond       = 0;

    field14C_0_tmp = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_0;
    field14C_1_tmp = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_1;
    field14C_2_tmp = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_2;

    field14C_0 = field14C_0_tmp;
    field14C_1 = field14C_1_tmp;
    field14C_2 = field14C_2_tmp;

    temp_s4 = sharedFunc_800DC438_2_s00(airScreamer);

    switch (airScreamer->model.stateStep)
    {
        case 64:
            sharedFunc_800DD534_2_s00(airScreamer);
            airScreamer->model.stateStep = 1;
            break;

        case 0:
            cond = true;

            airScreamerProps.timer_120 = Q12(5.0f);
            sharedFunc_800DEC84_2_s00(airScreamer, Q12(6.0f), airScreamer->rotation.vy);
            sharedFunc_800DECA4_2_s00(airScreamer, &airScreamerProps.targetPosition, Q12(5.0f));

            airScreamer->model.stateStep = 1;

        case 1:
            distToTarget       = Math_Distance2dGet(&airScreamer->position, &airScreamerProps.targetPosition);
            angleDeltaToTarget = Q12_ANGLE_NORM_S(func_80080478(&airScreamer->position, &airScreamerProps.targetPosition) - airScreamer->rotation.vy);

            if (temp_s4 == 2)
            {
                airScreamer->model.stateStep = 2;
            }
            else if (temp_s4 == 1 || (sharedFunc_800DC30C_2_s00(airScreamer) &&
                                      cond == true &&
                                      airScreamerProps.flags & AirScreamerFlag_31 &&
                                      (distToTarget < Q12(2.0f) || Math_CheckSignedRange(angleDeltaToTarget, Q12_ANGLE(60.0f)))))
            {
                airScreamer->model.stateStep = 4;
            }
            else if (sharedFunc_800DC3BC_2_s00(airScreamer) &&
                     cond == true &&
                     distToTarget < Q12(4.0f) &&
                     (angleDeltaToTarget >= Q12_ANGLE(-30.0f) && angleDeltaToTarget <= Q12_ANGLE(30.0f)) &&
                     Rng_TestProbability(Q12(0.7f)))
            {
                airScreamer->model.stateStep = 2;
            }
            else if (airScreamerProps.timer_120 == Q12(0.0f) || distToTarget < Q12(3.0f))
            {
                airScreamer->model.stateStep = 0;
            }
            break;

        case 2:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_Glide, true))
            {
                switchCond                           = 1;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_GlideToWalkForward, false);
                airScreamer->model.stateStep   = 3;
            }
            break;

        case 3:
            switchCond = 1;
            break;

        case 4:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_Glide, true))
            {
                switchCond                           = 2;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_GlideToHover, false);
                airScreamer->model.stateStep   = 5;
            }
            break;

        case 5:
            switchCond = 2;
            break;

        case 65:
            sharedFunc_800DDA80_2_s00(airScreamer);
            airScreamer->model.stateStep = 1;
            break;

        case 66:
            sharedFunc_800DD588_2_s00(airScreamer);
            airScreamer->model.stateStep = 1;
            break;

        case 67:
            sharedFunc_800DD834_2_s00(airScreamer);
            airScreamer->model.stateStep = 1;
            airScreamerProps.flags      |= AirScreamerFlag_12;
            break;
    }

    sharedFunc_800E021C_2_s00(airScreamer, 0, 1);

    switch (AirScreamer_DamageTake(airScreamer, Q12(1.0f)))
    {
        case AirScreamerHit_None:
            switch (switchCond)
            {
                case 0:
                    if (field14C_1 | field14C_0 | field14C_2)
                    {
                        airScreamer->model.controlState = AirScreamerControl_35;
                        airScreamer->model.stateStep    = 0;
                    }
                    break;

                case 1:
                    if (sp10 == 1)
                    {
                        if (!(field14C_1 | field14C_0 | field14C_2))
                        {
                            airScreamer->model.controlState = AirScreamerControl_4;
                            airScreamer->model.stateStep    = 0;
                        }
                        else
                        {
                            airScreamer->model.controlState = AirScreamerControl_6;
                            airScreamer->model.stateStep    = 0;
                        }
                    }
                    break;

                case 2:
                    if (sp10 == 2)
                    {
                        if (!(field14C_1 | field14C_0 | field14C_2))
                        {
                            airScreamer->model.controlState = AirScreamerControl_18;
                            airScreamer->model.stateStep    = 0;
                        }
                        else
                        {
                            airScreamer->model.controlState = AirScreamerControl_20;
                            airScreamer->model.stateStep    = 0;
                        }
                    }
                    break;
            }
            break;

        case AirScreamerHit_Recoil:
        case AirScreamerHit_Rear:
            airScreamer->model.controlState = AirScreamerControl_44;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            break;

        case AirScreamerHit_Stun:
        case AirScreamerHit_Kill:
            airScreamer->model.controlState = AirScreamerControl_45;
            airScreamer->model.stateStep    = 0;

            if (airScreamer->health <= Q12(0.0f))
            {
                airScreamerProps.flags |= AirScreamerFlag_6;
            }
            else
            {
                airScreamerProps.flags |= AirScreamerFlag_3;
            }
            break;
    }
#endif
}

void AirScreamer_Control_34(s_SubCharacter* airScreamer)
{
#ifndef MAP0_S01
    s32    animStatus;
    s32    switchCond;
    s32    sp10;
    bool   cond;
    bool   field14C_0;
    bool   field14C_1;
    bool   field14C_2;
    bool   field14C_0_tmp;
    bool   field14C_1_tmp;
    bool   field14C_2_tmp;
    s32    temp_s4;
    q19_12 distFieldF8;
    q19_12 angFieldF8;

    animStatus     = airScreamer->model.anim.status;
    switchCond     = 0;
    sp10           = sharedFunc_800D4A80_0_s01(airScreamer);
    cond           = false;
    field14C_0_tmp = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_0;
    field14C_1_tmp = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_1;
    field14C_2_tmp = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_2;
    field14C_0     = field14C_0_tmp;
    field14C_1     = field14C_1_tmp;
    field14C_2     = field14C_2_tmp;
    temp_s4        = sharedFunc_800DC438_2_s00(airScreamer);

    switch (airScreamer->model.stateStep)
    {
        case 0:
            cond                       = 1;
            airScreamerProps.timer_120 = Q12(5.0f);
            sharedFunc_800DECC4_2_s00(airScreamer);
            airScreamer->model.stateStep = 1;

        case 1:
            distFieldF8 = Math_Distance2dGet(&airScreamer->position, &airScreamerProps.targetPosition);
            angFieldF8  = Q12_ANGLE_NORM_S(func_80080478(&airScreamer->position, &airScreamerProps.targetPosition) - airScreamer->rotation.vy);
            if (temp_s4 == 2)
            {
                airScreamer->model.stateStep = temp_s4;
            }
            else if (temp_s4 == 1 ||
                     (sharedFunc_800DC30C_2_s00(airScreamer) && cond == true &&
                      airScreamerProps.flags & AirScreamerFlag_31 &&
                      (distFieldF8 < Q12(2.0f) || Math_CheckSignedRange(angFieldF8, Q12_ANGLE(60.0f))))) // @hack Should be `angFieldF8 >= Q12_ANGLE(-60.0f) && angFieldF8 < Q12_ANGLE(60.0f)`
            {
                airScreamer->model.stateStep = 4;
            }
            else if (sharedFunc_800DC3BC_2_s00(airScreamer) && cond == true && distFieldF8 < Q12(4.0f) &&
                     (angFieldF8 >= Q12_ANGLE(-30.0f) && angFieldF8 <= Q12_ANGLE(30.0f)) && Rng_TestProbability(Q12(0.7f)))
            {
                airScreamer->model.stateStep = 2;
            }
            else if (airScreamerProps.timer_120 == Q12(0.0f) || distFieldF8 < Q12(2.0f))
            {
                airScreamer->model.stateStep = 0;
            }
            break;

        case 2:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_Glide, true))
            {
                switchCond                           = 1;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_GlideToWalkForward, false);
                airScreamer->model.stateStep   = 3;
            }
            break;

        case 3:
            switchCond = 1;
            break;

        case 4:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_Glide, true))
            {
                switchCond                           = 2;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_GlideToHover, false);
                airScreamer->model.stateStep   = 5;
            }
            break;

        case 5:
            switchCond = 2;
            break;
    }

    sharedFunc_800E021C_2_s00(airScreamer, 0, 1);
    switch (AirScreamer_DamageTake(airScreamer, Q12(1.0f)))
    {
        case AirScreamerHit_None:
            switch (switchCond)
            {
                case 0:
                    if ((field14C_1 | field14C_0 | field14C_2) != 0)
                    {
                        airScreamer->model.controlState = AirScreamerControl_35;
                        airScreamer->model.stateStep    = 0;
                    }
                    break;

                case 1:
                    if (sp10 == switchCond)
                    {
                        airScreamer->model.controlState = (field14C_1 | field14C_0 | field14C_2) ? 6 : 5;
                        airScreamer->model.stateStep    = 0;
                    }
                    break;

                case 2:
                    if (sp10 == switchCond)
                    {
                        airScreamer->model.controlState = (field14C_1 | field14C_0 | field14C_2) ? 20 : 19;
                        airScreamer->model.stateStep    = 0;
                    }
                    break;
            }
            break;

        case AirScreamerHit_Recoil:
        case AirScreamerHit_Rear:
            airScreamer->model.controlState = AirScreamerControl_44;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            break;

        case AirScreamerHit_Stun:
        case AirScreamerHit_Kill:
            airScreamer->model.controlState = AirScreamerControl_45;
            airScreamer->model.stateStep    = 0;

            if (airScreamer->health <= Q12(0.0f))
            {
                airScreamerProps.flags |= AirScreamerFlag_6;
            }
            else
            {
                airScreamerProps.flags |= AirScreamerFlag_3;
            }
            break;
    }
#endif
}

void AirScreamer_Control_35(s_SubCharacter* airScreamer)
{
#ifndef MAP0_S01
    s32  temp_a0;
    s32  temp_a1;
    s32  temp_s3;
    s32  temp_s4;
    s32  temp_s5;
    s32  temp_s6;
    s32  temp_s7;
    s32  var_a0;
    s32  var_a1;
    s32  unkAngleDelta;
    s32  var_s1;
    s32  animStatus;
    bool new_var;
    bool new_var2;
    bool new_var3;

    animStatus = airScreamer->model.anim.status;

    var_s1 = 0;

    temp_s7 = sharedFunc_800D4A80_0_s01(airScreamer);

    temp_s6 = sharedData_800E21D0_0_s01.field_15C;

    new_var  = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_1;
    new_var2 = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_2;
    new_var3 = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_0;

    temp_s5 = new_var3;
    temp_s4 = new_var;
    temp_s3 = new_var2;

    temp_a0 = sharedFunc_800DC438_2_s00(airScreamer);
    var_a1  = 0;

    // Handle state step.
    switch (airScreamer->model.stateStep)
    {
        case 0:
            airScreamerProps.timer_120 = Q12(6.0f);

        case 1:
            sharedFunc_800DECA4_2_s00(airScreamer, &airScreamerProps.headTargetPosition, Q12(2.0f));

            if (sharedFunc_800DC30C_2_s00(airScreamer) && Rng_TestProbability(Q12(0.1f)))
            {
                airScreamer->model.stateStep = 8;
            }
            else
            {
                airScreamer->model.stateStep = 2;
            }
            break;

        case 2:
            var_a1 = 1;

        case 3:
            if (temp_a0 == 2)
            {
                airScreamer->model.stateStep = 4;
            }
            else
            {
                if (temp_a0 == 1)
                {
                    airScreamer->model.stateStep = 8;
                }
                else if (airScreamerProps.timer_120 == Q12(0.0f))
                {
                    if (var_a1 != 0)
                    {
                        var_s1 = 1;
                    }
                    airScreamer->model.stateStep = 0;
                }
                else if (Math_Distance2dGet(&airScreamer->position, &airScreamerProps.targetPosition) < Q12(1.0f))
                {
                    airScreamer->model.stateStep = 1;
                }
                else if (temp_s5 || temp_s4 || temp_s3)
                {
                    airScreamer->model.stateStep = 3;
                    if (temp_s6 >= 33)
                    {

                        unkAngleDelta = Q12_ANGLE_NORM_S(func_80080478(&airScreamer->position, &airScreamerProps.headTargetPosition) - airScreamer->rotation.vy);
                        if (unkAngleDelta < Q12_ANGLE(0.0f))
                        {
                            unkAngleDelta += Q12_ANGLE(0.3f);
                        }

                        sharedFunc_800DEC84_2_s00(airScreamer, Q12(2.0f), (unkAngleDelta >> 2) + airScreamer->rotation.vy);
                        airScreamerProps.timer_120 = Q12(6.0f);
                    }
                }
            }
            break;

        case 4:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_Glide, true))
            {
                var_s1                               = 2;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_GlideToWalkForward, false);

                if (!temp_s3 && !temp_s4)
                {
                    airScreamer->model.stateStep = 6;
                }
                else
                {
                    airScreamer->model.stateStep = 7;
                }
            }
            else if (temp_s3 || temp_s4)
            {
                airScreamer->model.stateStep = 5;
            }
            break;

        case 5:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_Glide, true))
            {
                var_s1                               = 3;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_GlideToWalkForward, false);
                airScreamer->model.stateStep   = 7;
            }
            break;

        case 6:
            if (temp_s3 || temp_s4)
            {
                var_s1                           = 3;
                airScreamer->model.stateStep = 7;
            }
            else
            {
                var_s1 = 2;
            }
            break;

        case 7:
            var_s1 = 3;
            break;

        case 8:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_Glide, true))
            {
                var_s1                               = 4;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_GlideToHover, false);

                if (!temp_s3 && !temp_s4)
                {
                    airScreamer->model.stateStep = 10;
                }
                else
                {
                    airScreamer->model.stateStep = 11;
                }
            }
            else if (temp_s3 || temp_s4)
            {
                airScreamer->model.stateStep = 9;
            }
            break;

        case 9:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_Glide, true))
            {
                var_s1                               = 5;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_GlideToHover, false);
                airScreamer->model.stateStep   = 11;
            }
            break;

        case 10:
            if (temp_s3 || temp_s4)
            {
                var_s1                           = 5;
                airScreamer->model.stateStep = 11;
            }
            else
            {
                var_s1 = 4;
            }
            break;

        case 11:
            var_s1 = 5;
            break;
    }

    sharedFunc_800E021C_2_s00(airScreamer, 0, 2);

    switch (AirScreamer_DamageTake(airScreamer, Q12(1.0f)))
    {
        case AirScreamerHit_None:
            switch (var_s1)
            {
                case 0:
                case 1:
                    if (!temp_s3 && !temp_s4)
                    {
                        if (var_s1 == 1)
                        {
                            temp_a1 = Rng_RandQ12();

                            if (airScreamerProps.field_E8_8 == var_s1)
                            {
                                var_a0 = Q12(0.1f);
                            }
                            else
                            {
                                var_a0 = 0;
                            }

                            if (temp_a1 < (Q12(0.7f) - (var_a0 * 2)))
                            {
                                airScreamer->model.controlState = AirScreamerControl_43;
                                airScreamer->model.stateStep    = 0;
                            }
                        }
                    }
                    else
                    {
                        airScreamer->model.controlState = AirScreamerControl_36;
                        airScreamer->model.stateStep    = 0;
                    }
                    break;

                case 2:
                    if (temp_s7 == 1)
                    {
                        airScreamer->model.controlState = AirScreamerControl_6;
                        airScreamer->model.stateStep    = 0;
                    }
                    break;

                case 3:
                    if (temp_s7 == 1)
                    {
                        airScreamer->model.controlState = AirScreamerControl_7;
                        airScreamer->model.stateStep    = 0;
                    }
                    break;

                case 4:
                    if (temp_s7 == 2)
                    {
                        airScreamer->model.controlState = AirScreamerControl_20;
                        airScreamer->model.stateStep    = 0;
                    }
                    break;

                case 5:
                    if (temp_s7 == 2)
                    {
                        airScreamer->model.controlState = AirScreamerControl_21;
                        airScreamer->model.stateStep    = 0;
                    }
                    break;
            }
            break;

        case AirScreamerHit_Recoil:
        case AirScreamerHit_Rear:
            airScreamer->model.controlState = AirScreamerControl_44;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            break;

        case AirScreamerHit_Stun:
        case AirScreamerHit_Kill:
            airScreamer->model.controlState = AirScreamerControl_45;
            airScreamer->model.stateStep    = 0;

            if (airScreamer->health <= Q12(0.0f))
            {
                airScreamerProps.flags |= AirScreamerFlag_6;
            }
            else
            {
                airScreamerProps.flags |= AirScreamerFlag_3;
            }
            break;
    }
#endif
}

void AirScreamer_Control_36(s_SubCharacter* airScreamer)
{
#ifndef MAP0_S01
    s32  animStatus;
    s32  switchCond;
    s32  temp_fp;
    s32  field15C;
    bool field14C_0;
    bool field14C_1;
    bool field14C_2;
    bool field14C_0_tmp;
    bool field14C_1_tmp;
    bool field14C_2_tmp;
    bool cond;
    s32  temp_a0;

    animStatus     = airScreamer->model.anim.status;
    switchCond     = 0;
    temp_fp        = sharedFunc_800D4A80_0_s01(airScreamer);
    field15C       = sharedData_800E21D0_0_s01.field_15C;
    field14C_0_tmp = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_0;
    field14C_1_tmp = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_1;
    field14C_2_tmp = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_2;

    field14C_0 = field14C_0_tmp;
    field14C_1 = field14C_1_tmp;
    field14C_2 = field14C_2_tmp;

    cond    = false;
    temp_a0 = sharedFunc_800DC438_2_s00(airScreamer);

    // Handle state step.
    switch (airScreamer->model.stateStep)
    {
        case 0:
            if (sharedFunc_800DC30C_2_s00(airScreamer) && Rng_TestProbability(Q12(0.1f)))
            {
                airScreamer->model.stateStep = 8;
            }
            else
            {
                airScreamer->model.stateStep = 2;
            }

        case 1:
            airScreamerProps.timer_120 = Q12(6.0f);
            sharedFunc_800DECA4_2_s00(airScreamer, &airScreamerProps.headTargetPosition, Q12(3.0f));
            airScreamer->model.stateStep = 2;
            break;

        case 2:
            cond = true;

        case 3:
            if (temp_a0 == 2)
            {
                airScreamer->model.stateStep = 4;
            }
            else if (temp_a0 == 1)
            {
                airScreamer->model.stateStep = 8;
            }
            else if (Math_Distance2dGet(&airScreamer->position, &airScreamerProps.targetPosition) < Q12(1.0f))
            {
                if (cond)
                {
                    switchCond = 1;
                }

                airScreamer->model.stateStep = 0;
            }
            else if (airScreamerProps.timer_120 == Q12(0.0f))
            {
                airScreamer->model.stateStep = 1;
            }
            else if (field14C_2 || field14C_0 || field14C_1)
            {
                airScreamer->model.stateStep = 3;

                if (field15C > 32)
                {
                    q19_12 angleDiff = Q12_ANGLE_NORM_S(func_80080478(&airScreamer->position, &airScreamerProps.headTargetPosition) - airScreamer->rotation.vy);
                    sharedFunc_800DEC84_2_s00(airScreamer, Q12(3.0f), (angleDiff / 4) + airScreamer->rotation.vy);
                    airScreamerProps.timer_120 = Q12(6.0f);
                }
            }
            break;

        case 4:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_Glide, true))
            {
                switchCond                           = 2;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_GlideToWalkForward, false);

                if (!field14C_1 && !field14C_0)
                {
                    airScreamer->model.stateStep = 6;
                }
                else
                {
                    airScreamer->model.stateStep = 7;
                }
            }
            else if (field14C_2 || field14C_1 || field14C_0)
            {
                airScreamer->model.stateStep = 5;
            }
            break;

        case 5:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_Glide, true))
            {
                switchCond                           = 3;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_GlideToWalkForward, false);
                airScreamer->model.stateStep   = 7;
            }
            break;

        case 6:
            switchCond = 3;

            if (field14C_2 || field14C_1 || field14C_0)
            {
                airScreamer->model.stateStep = 7;
            }
            else
            {
                switchCond = 2;
            }
            break;

        case 7:
            switchCond = 3;
            break;

        case 8:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_Glide, true))
            {
                switchCond                           = 4;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_GlideToHover, false);

                if (!field14C_1 && !field14C_0)
                {
                    airScreamer->model.stateStep = 10;
                }
                else
                {
                    airScreamer->model.stateStep = 11;
                }
            }
            else if (field14C_2 || field14C_1 || field14C_0)
            {
                airScreamer->model.stateStep = 9;
            }
            break;

        case 9:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_Glide, true))
            {
                switchCond                           = 5;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_GlideToHover, false);
                airScreamer->model.stateStep   = 11;
            }
            break;

        case 10:
            switchCond = 5;

            if (field14C_2 || field14C_1 || field14C_0)
            {
                airScreamer->model.stateStep = 11;
            }
            else
            {
                switchCond = 4;
            }
            break;

        case 11:
            switchCond = 5;
            break;
    }

    sharedFunc_800E021C_2_s00(airScreamer, 1, 1);

    // Handle damage.
    switch (AirScreamer_DamageTake(airScreamer, Q12(1.0f)))
    {
        case AirScreamerHit_None:
            switch (switchCond)
            {
                case 0:
                case 1:
                    if (field14C_2)
                    {
                        airScreamer->model.controlState = AirScreamerControl_37;
                        airScreamer->model.stateStep    = 0;
                    }
                    else if (switchCond == 1)
                    {
                        s32 rand   = Rng_RandQ12();
                        s32 var_a0 = 0;

                        if (airScreamerProps.field_E8_8 == 1)
                        {
                            var_a0 = Q12(0.1f);
                        }

                        if (rand < (Q12(0.7f) - (var_a0 * 2)))
                        {
                            airScreamer->model.controlState = AirScreamerControl_35;
                            airScreamer->model.stateStep    = 0;
                        }
                    }
                    break;

                case 2:
                    if (temp_fp == 1)
                    {
                        if (field14C_2)
                        {
                            airScreamer->model.controlState = AirScreamerControl_8;
                            airScreamer->model.stateStep    = 0;
                        }
                        else
                        {
                            airScreamer->model.controlState = AirScreamerControl_6;
                            airScreamer->model.stateStep    = 0;
                        }
                    }
                    break;

                case 3:
                    if (temp_fp == 1)
                    {
                        if (field14C_2)
                        {
                            airScreamer->model.controlState = AirScreamerControl_8;
                            airScreamer->model.stateStep    = 0;
                        }
                        else
                        {
                            airScreamer->model.controlState = AirScreamerControl_7;
                            airScreamer->model.stateStep    = 0;
                        }
                    }
                    break;

                case 4:
                    if (temp_fp == 2)
                    {
                        if (field14C_2)
                        {
                            airScreamer->model.controlState = AirScreamerControl_22;
                            airScreamer->model.stateStep    = 0;
                        }
                        else
                        {
                            airScreamer->model.controlState = AirScreamerControl_20;
                            airScreamer->model.stateStep    = 0;
                        }
                    }
                    break;

                case 5:
                    if (temp_fp == 2)
                    {
                        if (field14C_2)
                        {
                            airScreamer->model.controlState = AirScreamerControl_22;
                            airScreamer->model.stateStep    = 0;
                        }
                        else
                        {
                            airScreamer->model.controlState = AirScreamerControl_21;
                            airScreamer->model.stateStep    = 0;
                        }
                    }
                    break;
            }
            break;

        case AirScreamerHit_Recoil:
        case AirScreamerHit_Rear:
            airScreamer->model.controlState = AirScreamerControl_44;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            break;

        case AirScreamerHit_Stun:
        case AirScreamerHit_Kill:
            airScreamer->model.controlState = AirScreamerControl_45;
            airScreamer->model.stateStep    = 0;

            if (airScreamer->health <= Q12(0.0f))
            {
                airScreamerProps.flags |= AirScreamerFlag_6;
            }
            else
            {
                airScreamerProps.flags |= AirScreamerFlag_3;
            }
            break;
    }
#endif
}

void AirScreamer_Control_37(s_SubCharacter* airScreamer)
{
#ifndef MAP0_S01
    s32    temp_a0;
    q19_12 chance1;
    q19_12 dist;
    s32    temp_s4;
    bool   field14C;
    q19_12 angle;
    s32    switchCond;
    q19_12 chance0;
    s32    animStatus;
    s32    new_var;

    switchCond = 0;
    animStatus = airScreamer->model.anim.status;
    new_var    = sharedFunc_800D4A80_0_s01(airScreamer);
    dist       = sharedData_800E21D0_0_s01.distance_150;
    angle      = sharedData_800E21D0_0_s01.angle_154;
    field14C   = sharedData_800E21D0_0_s01.field_14C.bits.field_14C_2;
    temp_s4    = new_var;
    temp_a0    = sharedFunc_800DC438_2_s00(airScreamer);

    // Handle state step.
    switch (airScreamer->model.stateStep)
    {
        case 0:
            switchCond                 = 1;
            airScreamerProps.timer_120 = Q12(2.0f);
            airScreamer->flags     |= CharaFlag_Hit;

        case 1:
            sharedFunc_800DECA4_2_s00(airScreamer, &airScreamerProps.headTargetPosition, Q12(4.0f));
            airScreamer->model.stateStep = 2;
            break;

        case 2:
            if (temp_a0 == 2)
            {
                airScreamer->model.stateStep = 3;
            }
            else if (temp_a0 == 1)
            {
                airScreamer->model.stateStep = 5;
            }
            else if (airScreamerProps.timer_120 == Q12(0.0f))
            {
                airScreamer->model.stateStep = 0;
            }
            else if (Math_Distance2dGet(&airScreamer->position, &airScreamerProps.targetPosition) < Q12(1.0f))
            {
                airScreamer->model.stateStep = 1;
            }
            break;

        case 3:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_Glide, true))
            {
                switchCond                           = 2;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_GlideToWalkForward, false);
                airScreamer->model.stateStep   = 4;
            }
            break;

        case 4:
            switchCond = 2;
            break;

        case 5:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_Glide, true))
            {
                switchCond                           = 3;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_GlideToHover, false);
                airScreamer->model.stateStep   = 6;
            }
            break;

        case 6:
            switchCond = 3;
            break;
    }

    sharedFunc_800E021C_2_s00(airScreamer, 2, 0);

    // Handle damage.
    switch (AirScreamer_DamageTake(airScreamer, Q12(1.0f)))
    {
        case AirScreamerHit_None:
            switch (switchCond)
            {
                case 0:
                    if (!field14C)
                    {
                        airScreamer->model.controlState = AirScreamerControl_36;
                        airScreamer->model.stateStep    = 0;
                    }
                    break;

                case 2:
                    if (temp_s4 == 1)
                    {
                        airScreamer->model.controlState = AirScreamerControl_8;
                        airScreamer->model.stateStep    = 0;
                    }
                    break;

                case 3:
                    if (temp_s4 == 2)
                    {
                        airScreamer->model.controlState = AirScreamerControl_22;
                        airScreamer->model.stateStep    = 0;
                    }
                    break;

                case 1:
                    chance0 = sharedFunc_800DC894_2_s00(airScreamer, dist);

                    chance1 = sharedFunc_800DC6E4_2_s00(airScreamer, dist);
                    if (Q12_ANGLE_NORM_S(angle - airScreamer->rotation.vy) >= Q12_ANGLE(-22.5f) &&
                        Q12_ANGLE_NORM_S(angle - airScreamer->rotation.vy) < Q12_ANGLE(22.5f) &&
                        dist > Q12(4.0f) && dist < Q12(8.0f))
                    {
                        chance0 += Q12(0.2f);
                    }

                    if (Rng_TestProbability(chance0))
                    {
                        if (airScreamerProps.field_E8_0 == 3)
                        {
                            sharedFunc_800DD13C_2_s00(airScreamer, airScreamer->field_40 + 1, Q12(0.6f));
                        }

                        airScreamer->model.controlState = AirScreamerControl_38;
                        airScreamer->model.stateStep    = 0;
                        airScreamerProps.flags         |= AirScreamerFlag_4;
                    }
                    else if (Rng_TestProbability(chance1))
                    {
                        airScreamer->model.controlState = AirScreamerControl_42;
                        airScreamer->model.stateStep    = 0;
                        airScreamerProps.flags         |= AirScreamerFlag_4;
                    }
                    break;
            }
            break;

        case AirScreamerHit_Recoil:
        case AirScreamerHit_Rear:
            airScreamer->model.controlState = AirScreamerControl_44;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            break;

        case AirScreamerHit_Stun:
        case AirScreamerHit_Kill:
            airScreamer->model.controlState = AirScreamerControl_45;
            airScreamer->model.stateStep    = 0;

            if (airScreamer->health <= Q12(0.0f))
            {
                airScreamerProps.flags |= AirScreamerFlag_6;
            }
            else
            {
                airScreamerProps.flags |= AirScreamerFlag_3;
            }
            break;
    }
#endif
}

void AirScreamer_Control_38(s_SubCharacter* airScreamer)
{
#ifndef MAP0_S01
    s32           sp10;
    s32           temp_s0;
    s32           temp_s0_2;
    s32           temp_s0_4;
    s32           stateStep;
    s32           temp_s5;
    s32           temp_v0;
    s32           temp_v1_2;
    s32           var_s4;
    s32           temp_s7;
    s32           var_v0_2;
    bool          new_var;
    s32           temp_s6;

    temp_s7 = airScreamer->model.anim.status;
    var_s4  = 0;

    temp_v0 = sharedFunc_800D4A80_0_s01(airScreamer);

    new_var = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_2;
    temp_s0 = sharedData_800E21D0_0_s01.distance_150;
    sp10    = new_var;

    stateStep = sharedFunc_800DC438_2_s00(airScreamer);
    temp_s6   = sharedFunc_800DEE24_2_s00(airScreamer);

    switch (airScreamer->model.stateStep)
    {
        case 0:
            temp_s0_2 = temp_s0 + (Rng_RandQ12() * 4);

            if (!sharedFunc_800DC30C_2_s00(airScreamer) || temp_s0_2 >= Q12(3.0f) || Rng_RandQ12() >= 0x333)
            {
                airScreamer->model.stateStep = 1;
            }
            else
            {
                airScreamer->model.stateStep = 4;
            }
            airScreamerProps.timer_120 = Q12(4.0f);
            break;

        case 1:
            temp_s5   = airScreamerProps.targetPosition.vy - airScreamer->position.vy;
            temp_s0_4 = Math_Distance2dGet(&airScreamer->position, &airScreamerProps.targetPosition);
            temp_v1_2 = Q12_ANGLE_NORM_S(func_80080478(&airScreamer->position, &airScreamerProps.targetPosition) - airScreamer->rotation.vy);

            if (stateStep == 2)
            {
                airScreamer->model.stateStep = stateStep;
            }
            else if (stateStep == 1)
            {
                airScreamer->model.stateStep = 4;
            }
            else if ((temp_s5 >= -0x333 && temp_s5 <= 0x333) &&
                     (temp_s6 > 0x599 && temp_s6 < 0x8CD) &&
                     (temp_v1_2 >= -0x80 && temp_v1_2 <= 0x80))
            {
                var_s4 = 1;
            }
            else
            {
                // @hack This check should be `if ((temp_v1_2 >= -0x400 && temp_v1_2 < 0x400) && temp_s0_4 < Q12(4.0f))`,
                if (Math_CheckSignedRange(temp_v1_2, 0x400) && (temp_s0_4 < Q12(4.0f)))
                {
                    var_s4 = 2;
                }
                else if (airScreamerProps.timer_120 == Q12(0.0f))
                {
                    airScreamer->model.stateStep = 0;
                }
            }
            break;

        case 2:
            if (temp_s7 == 35)
            {
                var_s4                               = 3;
                airScreamer->model.anim.status = 32;
                airScreamer->model.stateStep   = 3;
            }
            break;

        case 3:
            var_s4 = 3;
            break;

        case 4:
            if (temp_s7 == 35)
            {
                var_s4                               = 4;
                airScreamer->model.anim.status = 30;
                airScreamer->model.stateStep   = 5;
            }
            break;

        case 5:
            var_s4 = 4;
            break;
    }

    sharedFunc_800E021C_2_s00(airScreamer, 2, 0);

    switch (AirScreamer_DamageTake(airScreamer, Q12(1.0f)))
    {
        case AirScreamerHit_None:
            switch (var_s4)
            {
                case 0:
                case 2:
                    if (!sharedFunc_800DC67C_2_s00(airScreamer))
                    {
                        var_v0_2 = 41;
                        if (sp10 != 0)
                        {
                            if (var_s4 == 2)
                            {
                                airScreamer->model.controlState = AirScreamerControl_39;
                                airScreamer->model.stateStep    = 0;
                            }
                            break;
                        }
                    }
                    else
                    {
                        var_v0_2 = 42;
                    }

                    airScreamer->model.controlState = var_v0_2;
                    airScreamer->model.stateStep    = 0;
                    break;

                case 1:
                    if (!Chara_HasFlag(&g_SysWork.playerWork.player, CharaFlag_Unk4) &&
                        (g_SysWork.npcIdxs[0] == NO_VALUE && g_SysWork.npcIdxs[1] == NO_VALUE) &&
                        temp_s7 == 0x23)
                    {
                        airScreamer->model.controlState = AirScreamerControl_40;
                        airScreamer->model.stateStep    = 0;
                    }
                    break;

                case 3:
                    if (temp_v0 == 1)
                    {
                        sharedFunc_800DE11C_2_s00(airScreamer);
                        airScreamer->model.controlState = AirScreamerControl_10;
                        airScreamer->model.stateStep    = 0;
                    }
                    break;

                case 4:
                    if (temp_v0 == 2)
                    {
                        airScreamer->model.controlState = AirScreamerControl_23;
                        airScreamer->model.stateStep    = 0;
                    }
                    break;
            }
            break;

        case AirScreamerHit_Recoil:
        case AirScreamerHit_Rear:
            airScreamer->model.controlState = AirScreamerControl_44;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            break;

        case AirScreamerHit_Stun:
        case AirScreamerHit_Kill:
            airScreamer->model.controlState = AirScreamerControl_45;
            airScreamer->model.stateStep    = 0;
            if (airScreamer->health <= Q12(0.0f))
            {
                airScreamerProps.flags |= AirScreamerFlag_6;
            }
            else
            {
                airScreamerProps.flags |= AirScreamerFlag_3;
            }
            break;
    }
#endif
}

void AirScreamer_Control_39(s_SubCharacter* airScreamer)
{
#ifndef MAP0_S01
    s32  sp10;
    s32  temp_a0;
    s32  temp_s0_2;
    s32  temp_s3;
    s32  temp_s5;
    s32  temp_s7;
    s32  temp_v0;
    s32  var_s4;
    s32  temp_s6;
    bool new_var;
    s32  temp_s8;

    temp_s6 = airScreamer->model.anim.status;
    var_s4  = 0;
    temp_v0 = sharedFunc_800D4A80_0_s01(airScreamer);
    new_var = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_2;
    sp10    = new_var;

    temp_s5   = sharedData_800E21D0_0_s01.distance_150;
    temp_s7   = sharedData_800E21D0_0_s01.angle_154;
    temp_s8   = temp_v0;
    temp_s3   = Math_Distance2dGet(&airScreamer->position, &airScreamerProps.targetPosition);
    temp_s0_2 = Q12_ANGLE_NORM_S(func_80080478(&airScreamer->position, &airScreamerProps.targetPosition) - airScreamer->rotation.vy);
    temp_a0   = sharedFunc_800DC438_2_s00(airScreamer);

    switch (airScreamer->model.stateStep)
    {
        case 0:
            sharedFunc_800DF24C_2_s00(airScreamer);
            Math_Distance2dGet(&airScreamer->position, &airScreamerProps.targetPosition);

            if (temp_s5 > Q12(6.0f))
            {
                temp_s3 = Q12(6.0f);
            }
            else if (temp_s5 > Q12(2.0f))
            {
                temp_s3 = Q12(12.0f) - temp_s5;
            }
            else
            {
                temp_s3 = Q12(10.0f);
            }

            temp_s3  += Rng_RandQ12() * 2;
            temp_s0_2 = Q12_ANGLE_NORM_S((temp_s7 + 0x800) - airScreamer->rotation.vy) / 2 + airScreamer->rotation.vy;

            sharedFunc_800DEC84_2_s00(airScreamer, temp_s3, temp_s0_2 + (Rng_RandQ12() - 0x800) / 8);
            airScreamerProps.timer_120       = Q12(5.0f);
            airScreamer->model.stateStep = 1;
            break;

        case 1:
            if (temp_a0 == 2)
            {
                airScreamer->model.stateStep = 2;
            }
            else if (temp_a0 == 1)
            {
                airScreamer->model.stateStep = 4;
            }
            else if (temp_s3 < Q12(2.0f) || Math_CheckSignedRange(temp_s0_2, 0x400) ||
                     airScreamerProps.timer_120 == Q12(0.0f))
            {
                var_s4 = 1;
            }
            break;

        case 2:
            if (temp_s6 == 35)
            {
                var_s4                               = 2;
                airScreamer->model.anim.status = 32;
                airScreamer->model.stateStep   = 3;
            }
            break;

        case 3:
            var_s4 = 2;
            break;

        case 4:
            if (temp_s6 == 35)
            {
                var_s4                               = 3;
                airScreamer->model.anim.status = 30;
                airScreamer->model.stateStep   = 5;
            }
            break;

        case 5:
            var_s4 = 3;
            break;
    }

    sharedFunc_800E021C_2_s00(airScreamer, 2, 0);

    switch (AirScreamer_DamageTake(airScreamer, Q12(1.0f)))
    {
        case AirScreamerHit_None:
            switch (var_s4)
            {
                case 0:
                case 1:
                    if (sp10 != 0)
                    {
                        if (temp_s5 > Q12(7.0f))
                        {
                            airScreamer->model.controlState = AirScreamerControl_38;
                            airScreamer->model.stateStep    = 0;
                            break;
                        }
                    }

                    if (var_s4 == 1)
                    {
                        if (airScreamerProps.field_E8_8 != 3)
                        {
                            airScreamerProps.field_E8_8 = 2;
                        }
                        airScreamer->model.controlState = AirScreamerControl_41;
                        airScreamer->model.stateStep    = 0;
                    }
                    break;

                case 2:
                    if (temp_s8 == 1)
                    {
                        airScreamer->model.controlState = AirScreamerControl_13;
                        airScreamer->model.stateStep    = 0;
                    }
                    break;

                case 3:
                    if (temp_s8 == 2)
                    {
                        airScreamer->model.controlState = AirScreamerControl_26;
                        airScreamer->model.stateStep    = 0;
                    }
                    break;
            }
            break;

        case AirScreamerHit_Recoil:
        case AirScreamerHit_Rear:
            airScreamer->model.controlState = AirScreamerControl_44;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            break;

        case AirScreamerHit_Stun:
        case AirScreamerHit_Kill:
            airScreamer->model.controlState = AirScreamerControl_45;
            airScreamer->model.stateStep    = 0;

            if (airScreamer->health <= Q12(0.0f))
            {
                airScreamerProps.flags |= AirScreamerFlag_6;
            }
            else
            {
                airScreamerProps.flags |= AirScreamerFlag_3;
            }
            break;
    }
#endif
}

void AirScreamer_Control_40(s_SubCharacter* airScreamer)
{
#ifndef MAP0_S01
    bool cond0;
    bool cond1;
    s32  animStatus;

    cond0 = false;
    cond1 = false;

    // Handle state step.
    animStatus = airScreamer->model.anim.status;
    switch (airScreamer->model.stateStep)
    {
        case 0:
            airScreamer->model.stateStep = 1;

        case 1:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_Glide, true))
            {
                airScreamer->model.stateStep   = 2;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_GlideScratchAttack, false);
            }
            break;

        case 2:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_GlideScratchAttack, true) &&
                airScreamer->model.anim.keyframeIdx < 8192)
            {
                airScreamer->model.stateStep = 3;
                airScreamerProps.flags      |= AirScreamerFlag_5;
            }
            break;

        case 3:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_Glide, true))
            {
                cond0 = true;
            }
            else if (airScreamer->field_44.field_8 == NO_VALUE)
            {
                cond0                            = true;
                cond1                            = true;
                airScreamer->model.stateStep = 4;
            }
            break;

        case 4:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_Glide, true))
            {
                cond0 = true;
                cond1 = true;
            }
            break;
    }

    sharedFunc_800DEE24_2_s00(airScreamer);
    sharedFunc_800E0514_2_s00(airScreamer);

    // Handle damage.
    switch (AirScreamer_DamageTake(airScreamer, Q12(1.2f)))
    {
        case AirScreamerHit_None:
            if (cond0)
            {
                if (airScreamerProps.field_E8_8 == 5 && cond1)
                {
                    airScreamerProps.field_E8_8         = 1;
                    airScreamer->model.controlState = AirScreamerControl_42;
                }
                else
                {
                    airScreamer->model.controlState = AirScreamerControl_39;
                }

                airScreamer->model.stateStep = 0;
            }
            break;

        case AirScreamerHit_Recoil:
        case AirScreamerHit_Rear:
            airScreamer->model.controlState = AirScreamerControl_44;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            break;

        case AirScreamerHit_Stun:
        case AirScreamerHit_Kill:
            airScreamer->model.controlState = AirScreamerControl_45;
            airScreamer->model.stateStep    = 0;

            if (airScreamer->health <= Q12(0.0f))
            {
                airScreamerProps.flags |= AirScreamerFlag_6;
            }
            else
            {
                airScreamerProps.flags |= AirScreamerFlag_3;
            }
            break;
    }
#endif
}

void AirScreamer_Control_41(s_SubCharacter* airScreamer)
{
#ifndef MAP0_S01
    s32    animStatus;
    s32    switchCond;
    s32    temp_s6;
    s32    temp_s6_tmp;
    bool   field14C_2;
    bool   field14C_2_tmp;
    s32    temp_s4;
    q19_12 unkDist;
    q19_12 unkDeltaAngle;

    animStatus = airScreamer->model.anim.status;
    switchCond = 0;

    // @hack Weird variable juggling needed for match, is there another way to do this?
    temp_s6_tmp    = sharedFunc_800D4A80_0_s01(airScreamer);
    field14C_2_tmp = sharedData_800E21D0_0_s01.field_14C.bits.field_14C_2;
    temp_s6        = temp_s6_tmp;
    field14C_2     = field14C_2_tmp;
    temp_s4        = sharedFunc_800DC438_2_s00(airScreamer);

    // Handle state step.
    switch (airScreamer->model.stateStep)
    {
        case 0:
            sharedFunc_800DF22C_2_s00(airScreamer);

        case 1:
            airScreamerProps.timer_120       = Q12(4.0f);
            airScreamer->model.stateStep = 2;
            break;

        case 2:
            unkDist       = Math_Distance2dGet(&airScreamer->position, &airScreamerProps.targetPosition);
            unkDeltaAngle = Q12_ANGLE_NORM_S(func_80080478(&airScreamer->position, &airScreamerProps.targetPosition) - airScreamer->rotation.vy);

            if (temp_s4 == 2)
            {
                airScreamer->model.stateStep = 3;
            }
            else if (temp_s4 == 1)
            {
                airScreamer->model.stateStep = 5;
            }
            else if (unkDist < Q12(0.5f))
            {
                if (sharedFunc_800DC30C_2_s00(airScreamer) && Rng_TestProbability(Q12(0.3f)))
                {
                    airScreamer->model.stateStep = 5;
                }
                else
                {
                    switchCond = 1;
                }
            }
            else if (unkDist < Q12(4.0f) && Math_CheckSignedRange(unkDeltaAngle, Q12_ANGLE(90.0f))) // @hack Should be `angFieldF8 >= Q12_ANGLE(-90.0f) && angFieldF8 < Q12_ANGLE(90.0f)`
            {
                switchCond = 2;
            }

            if (airScreamerProps.timer_120 == Q12(0.0f))
            {
                sharedFunc_800DECA4_2_s00(airScreamer, &airScreamerProps.headTargetPosition, Q12(2.5f));
                airScreamer->model.stateStep = 1;
            }
            break;

        case 3:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_Glide, true))
            {
                switchCond                           = 3;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_GlideToWalkForward, false);
                airScreamer->model.stateStep   = 4;
            }
            break;

        case 4:
            switchCond = 3;
            break;

        case 5:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_Glide, true))
            {
                switchCond                           = 4;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_GlideToHover, false);
                airScreamer->model.stateStep   = 6;
            }
            break;

        case 6:
            switchCond = 4;
            break;
    }

    sharedFunc_800E021C_2_s00(airScreamer, 1, 0);

    switch (AirScreamer_DamageTake(airScreamer, Q12(1.0f)))
    {
        case AirScreamerHit_None:
            switch (switchCond)
            {
                case AirScreamerHit_None:
                case AirScreamerHit_Recoil:
                case AirScreamerHit_Rear:
                    if (sharedFunc_800DC67C_2_s00(airScreamer))
                    {
                        airScreamer->model.controlState = AirScreamerControl_42;
                    }
                    else if (field14C_2)
                    {
                        airScreamer->model.controlState = AirScreamerControl_38;
                    }
                    else if (switchCond == 2)
                    {
                        airScreamer->model.controlState = AirScreamerControl_39;
                    }
                    else if (switchCond == 1)
                    {
                        airScreamer->model.controlState = AirScreamerControl_36;
                    }
                    else
                    {
                        break;
                    }

                    airScreamer->model.stateStep = 0;
                    break;

                case AirScreamerHit_Stun:
                    if (temp_s6 == 1)
                    {
                        if (!field14C_2)
                        {
                            airScreamer->model.controlState = AirScreamerControl_13;
                        }
                        else
                        {
                            airScreamer->model.controlState = AirScreamerControl_10;
                        }
                        airScreamer->model.stateStep = 0;
                    }
                    break;

                case AirScreamerHit_Kill:
                    if (temp_s6 == 2)
                    {
                        if (field14C_2)
                        {
                            airScreamer->model.controlState = AirScreamerControl_23;
                        }
                        else
                        {
                            airScreamer->model.controlState = AirScreamerControl_26;
                        }
                        airScreamer->model.stateStep = 0;
                    }
                    break;
            }
            break;

        case AirScreamerHit_Recoil:
        case AirScreamerHit_Rear:
            airScreamer->model.controlState = AirScreamerControl_44;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            break;

        case AirScreamerHit_Stun:
        case AirScreamerHit_Kill:
            airScreamer->model.controlState = AirScreamerControl_45;
            airScreamer->model.stateStep    = 0;

            if (airScreamer->health <= Q12(0.0f))
            {
                airScreamerProps.flags |= AirScreamerFlag_6;
            }
            else
            {
                airScreamerProps.flags |= AirScreamerFlag_3;
            }
            break;
    }
#endif
}

void AirScreamer_Control_42(s_SubCharacter* airScreamer)
{
#ifndef MAP0_S01
    s32    temp_a0;
    bool   field14C;
    s32    angleDelta;
    q19_12 angle;
    q19_12 dist;
    s32    temp_s7;
    s32    switchCond;
    q19_12 var_s3;
    s32    var_s5; // Possibly `bool`
    s32    animStatus;
    s32    new_var;

    animStatus = airScreamer->model.anim.status;
    switchCond = 0;
    new_var    = sharedFunc_800D4A80_0_s01(airScreamer);
    dist       = sharedData_800E21D0_0_s01.distance_150;
    angle      = sharedData_800E21D0_0_s01.angle_154;
    field14C   = sharedData_800E21D0_0_s01.field_14C.bits.field_14C_2;
    temp_s7    = new_var;
    var_s3     = Math_Distance2dGet(&airScreamer->position, &airScreamerProps.targetPosition);
    temp_a0    = sharedFunc_800DC438_2_s00(airScreamer);
    var_s5     = 0;

    // Handle state step.
    switch (airScreamer->model.stateStep)
    {
        case 0:
            var_s5     = sharedFunc_800DF24C_2_s00(airScreamer);
            var_s3     = Math_Distance2dGet(&airScreamer->position, &airScreamerProps.targetPosition);
            angleDelta = Q12_ANGLE_NORM_S(angle - airScreamer->rotation.vy);
            if (!sharedFunc_800DC30C_2_s00(airScreamer) || (angleDelta < Q12_ANGLE(-30.0f) || angleDelta >= Q12_ANGLE(30.0f)))
            {
                airScreamerProps.timer_120       = Q12(2.0f);
                airScreamer->model.stateStep = 1;
            }
            else
            {
                airScreamer->model.stateStep = 4;
            }
            break;

        case 1:
            if (temp_a0 == 2)
            {
                airScreamer->model.stateStep = 2;
            }
            else if (temp_a0 == 1)
            {
                airScreamer->model.stateStep = 4;
            }
            else if (var_s3 < Q12(2.0f) || (airScreamerProps.flags & AirScreamerFlag_31))
            {
                var_s5 = sharedFunc_800DF24C_2_s00(airScreamer);
            }
            else if (airScreamerProps.timer_120 == Q12(0.0f))
            {
                airScreamer->model.stateStep = 0;
            }
            break;

        case 2:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_Glide, true))
            {
                switchCond                           = 1;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_GlideToWalkForward, false);
                airScreamer->model.stateStep   = 3;
            }
            break;

        case 3:
            switchCond = 1;
            break;

        case 4:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_Glide, true))
            {
                switchCond                           = 2;
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_GlideToHover, false);
                airScreamer->model.stateStep   = 5;
            }
            break;

        case 5:
            switchCond = 2;
            break;
    }

    sharedFunc_800E021C_2_s00(airScreamer, 2, 0);

    // Handle damage.
    switch (AirScreamer_DamageTake(airScreamer, Q12(1.0f)))
    {
        case AirScreamerHit_None:
            switch (switchCond)
            {
                case 0:
                    if (var_s5 || (((field14C != 0 && dist < Q12(15.0f)) || dist < Q12(4.0f)) && var_s3 < Q12(1.5f)))
                    {
                        airScreamer->model.controlState = AirScreamerControl_38;
                        airScreamer->model.stateStep    = 0;
                        airScreamerProps.field_E8_8         = 3;
                        airScreamerProps.flags         |= AirScreamerFlag_4;
                    }
                    else if (dist > Q12(30.0f) && var_s3 < Q12(1.0f))
                    {
                        airScreamer->model.controlState = AirScreamerControl_36;
                        airScreamer->model.stateStep    = 0;
                    }
                    return;

                case 1:
                    if (temp_s7 == 1)
                    {
                        airScreamer->model.controlState = AirScreamerControl_14;
                        airScreamer->model.stateStep    = 0;
                    }
                    break;

                case 2:
                    if (temp_s7 == 2)
                    {
                        airScreamer->model.controlState = AirScreamerControl_27;
                        airScreamer->model.stateStep    = 0;
                    }
                    break;
            }
            break;

        case AirScreamerHit_Recoil:
        case AirScreamerHit_Rear:
            airScreamer->model.controlState = AirScreamerControl_44;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            break;

        case AirScreamerHit_Stun:
        case AirScreamerHit_Kill:
            airScreamer->model.controlState = AirScreamerControl_45;
            airScreamer->model.stateStep    = 0;

            if (airScreamer->health <= Q12(0.0f))
            {
                airScreamerProps.flags |= AirScreamerFlag_6;
            }
            else
            {
                airScreamerProps.flags |= AirScreamerFlag_3;
            }
            break;
    }
#endif
}

void AirScreamer_Control_43(s_SubCharacter* airScreamer)
{
#ifndef MAP0_S01
    sharedFunc_800DEC84_2_s00(airScreamer, Q12(0.5f), airScreamer->rotation.vy);
    sharedFunc_800E021C_2_s00(airScreamer, 1, 1);
    airScreamer->flags &= ~CharaFlag_Hit;

    // Handle damage.
    switch (AirScreamer_DamageTake(airScreamer, Q12(1.0f)))
    {
        case AirScreamerHit_None:
            // @hack Explicit `u32` cast needed for match.
            switch ((u32)airScreamerProps.field_E8_0)
            {
                case 0:
                    airScreamer->model.controlState = AirScreamerControl_Glide;
                    airScreamer->model.stateStep    = 0;
                    break;

                case 1:
                case 3:
                    airScreamer->model.controlState = AirScreamerControl_34;
                    airScreamer->model.stateStep    = 0;
                    break;
            }
            break;

        case AirScreamerHit_Recoil:
        case AirScreamerHit_Rear:
            airScreamer->model.controlState = AirScreamerControl_44;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            break;

        case AirScreamerHit_Stun:
        case AirScreamerHit_Kill:
            airScreamer->model.controlState = AirScreamerControl_45;
            airScreamer->model.stateStep    = 0;

            if (airScreamer->health <= Q12(0.0f))
            {
                airScreamerProps.flags |= AirScreamerFlag_6;
            }
            else
            {
                airScreamerProps.flags |= AirScreamerFlag_3;
            }
            break;
    }
#endif
}

void AirScreamer_Control_44(s_SubCharacter* airScreamer)
{
#ifndef MAP0_S01
    s32    animStatus;
    bool   field14C_2;
    bool   field14C_2_tmp;
    bool   cond;
    s32    var_a0;
    q19_12 var_s1;

    animStatus             = airScreamer->model.anim.status;
    field14C_2_tmp         = sharedData_800E21D0_0_s01.field_14C.bits32.field_14C_2;
    airScreamer->flags |= CharaFlag_Hit;
    field14C_2             = field14C_2_tmp;
    cond                   = false;

    // Handle state step. @hack Explicit `u32` cast needed for match.
    switch ((u32)airScreamer->model.stateStep)
    {
        case 0:
            if (ANIM_STATUS_IS_ACTIVE(animStatus))
            {
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_GlideRecoil, false);
                airScreamer->model.stateStep   = 1;
            }
            break;

        case 1:
            if (animStatus != ANIM_STATUS(AirScreamerAnim_GlideRecoil, false))
            {
                airScreamer->model.stateStep = 2;
                airScreamerProps.flags      |= AirScreamerFlag_3;
            }
            break;

        case 2:
            if (animStatus != ANIM_STATUS(AirScreamerAnim_GlideRecoil, true))
            {
                cond = true;
            }
            break;
    }

    sharedFunc_800E041C_2_s00(airScreamer);

    // Handle damage.
    switch (AirScreamer_DamageTake(airScreamer, Q12(0.6f)))
    {
        case AirScreamerHit_None:
        case AirScreamerHit_Recoil:
        case AirScreamerHit_Rear:
            if (!cond)
            {
                break;
            }

            var_s1 = airScreamerProps.field_E8_4 ? Q12(0.1f) : Q12(0.0f);

            if (airScreamerProps.field_E8_0 == 3)
            {
                sharedFunc_800DD13C_2_s00(airScreamer, airScreamer->field_40 + 1, Q12(0.7f));
            }

            if (field14C_2)
            {
                if (airScreamerProps.field_E8_8 == 5)
                {
                    airScreamer->model.controlState = AirScreamerControl_42;
                    airScreamer->model.stateStep    = 0;

                    if (Rng_TestProbability(Q12_DIV(airScreamer->health, Q12(380.0f)) + (var_s1 * 2)))
                    {
                        airScreamerProps.field_E8_8 = 3;
                    }
                    break;
                }
                else
                {
                    airScreamer->model.controlState = AirScreamerControl_38;
                    airScreamer->model.stateStep    = 0;

                    // TODO: Can `Rng_TestProbability` fit?
                    if (Rng_RandQ12() <= (Q12_DIV(airScreamer->health, Q12(380.0f)) + (var_s1 * 2)))
                    {
                        airScreamerProps.field_E8_8 = 3;
                    }
                    else
                    {
                        airScreamerProps.field_E8_8 = 5;
                    }
                }
                break;
            }

            switch (airScreamerProps.field_E8_8)
            {
                case 3:
                    airScreamer->model.controlState = AirScreamerControl_41;
                    airScreamer->model.stateStep    = 0;

                    if (Rng_TestProbability(((Q12(0.5f) - (var_s1 * 3)))))
                    {
                        airScreamerProps.field_E8_8 = 2;
                    }
                    break;

                case 2:
                    airScreamer->model.controlState = AirScreamerControl_36;
                    airScreamer->model.stateStep    = 0;

                    if (Rng_RandQ12() > (Q12_DIV(airScreamer->health, Q12(380.0f)) + (var_s1 * 2)))
                    {
                        airScreamerProps.field_E8_8 = 5;
                    }
                    else if (Rng_TestProbability(Q12(0.3f) + (var_s1 * 3)))
                    {
                        airScreamerProps.field_E8_8 = 3;
                    }
                    break;

                case 5:
                    airScreamer->model.controlState = AirScreamerControl_42;
                    airScreamer->model.stateStep    = 0;

                    if (Rng_TestProbability(Q12_DIV(airScreamer->health, Q12(380.0f))))
                    {
                        airScreamerProps.field_E8_8 = 3;
                    }
                    break;

                case 1:
                case 4:
                    if (Rng_TestProbability(Q12(0.2f) + (var_s1 * 5)))
                    {
                        airScreamer->model.controlState = AirScreamerControl_36;
                    }
                    else
                    {
                        airScreamer->model.controlState = AirScreamerControl_42;
                    }

                    airScreamer->model.stateStep = 0;

                    if (Rng_TestProbability(Q12(0.5f) + (var_s1 * 3)))
                    {
                        var_a0 = 2;
                    }
                    else
                    {
                        var_a0 = 5;
                    }

                    airScreamerProps.field_E8_8 = var_a0;
                    break;

                case 0:
                default:
                    if (airScreamerProps.field_E8_0 == 3)
                    {
                        var_s1 = Q12(1.0f);
                    }

                    if (Rng_TestProbability(Q12(0.4f) + (var_s1 * 3)))
                    {
                        airScreamer->model.controlState = AirScreamerControl_36;
                    }
                    else
                    {
                        airScreamer->model.controlState = AirScreamerControl_42;
                    }

                    airScreamer->model.stateStep = 0;

                    if (Rng_TestProbability(Q12(0.6f) + (var_s1 * 2)))
                    {
                        var_a0 = 2;
                    }
                    else
                    {
                        var_a0 = 5;
                    }

                    airScreamerProps.field_E8_8 = var_a0;
                    break;
            }
            break;

        case AirScreamerHit_Stun:
        case AirScreamerHit_Kill:
            airScreamer->model.controlState = AirScreamerControl_45;
            airScreamer->model.stateStep    = 0;

            if (airScreamer->health <= Q12(0.0f))
            {
                airScreamerProps.flags |= AirScreamerFlag_6;
            }
            else
            {
                airScreamerProps.flags |= AirScreamerFlag_3;
            }
            break;
    }
#endif
}

void AirScreamer_Control_45(s_SubCharacter* airScreamer)
{
#ifndef MAP0_S01
    bool cond;
    s32  animStatus;

    #define airScreamerFlags airScreamerProps

    cond       = false;
    animStatus = airScreamer->model.anim.status;

    airScreamer->flags |= CharaFlag_Hit;

    // Handle state step. TODO: Cast necessary?
    switch ((u32)airScreamer->model.stateStep)
    {
        case 0:
            if (ANIM_STATUS_IS_ACTIVE(animStatus))
            {
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_GlideToStun, false);
                airScreamer->model.stateStep   = 1;
            }
            break;

        case 1:
            if (animStatus != ANIM_STATUS(AirScreamerAnim_GlideToStun, false))
            {
                airScreamer->model.stateStep = 2;
                airScreamerFlags.flags      |= AirScreamerFlag_3;
            }
            break;

        case 2:
            if (animStatus != ANIM_STATUS(AirScreamerAnim_GlideToStun, true))
            {
                cond = true;
            }
            break;
    }

    if (sharedFunc_800D4A80_0_s01(airScreamer) == 3)
    {
        sharedFunc_800E04B0_2_s00(airScreamer);
    }
    else
    {
        sharedFunc_800D5638_0_s01(airScreamer);
    }

    AirScreamer_DamageTake(airScreamer, Q12(0.6f));

    if (cond)
    {
        airScreamer->model.controlState = AirScreamerControl_2;
        airScreamer->model.stateStep    = 0;
        airScreamerFlags.field_E8_8         = 1;
    }
#endif
}

// Control 46+ are only included in MAP0_S01

void AirScreamer_Control_46(s_SubCharacter* airScreamer)
{
#ifdef MAP0_S01
    s32 animStatus;

    animStatus = airScreamer->model.anim.status;

    if (airScreamer->model.stateStep != 1)
    {
        if (airScreamer->model.stateStep == 0 &&
            animStatus == ANIM_STATUS(AirScreamerAnim_HoverBiteAttack, true) &&
            airScreamer->model.anim.keyframeIdx < 8204)
        {
            airScreamerProps.timer_120       = Q12(4.0f);
            airScreamer->model.stateStep = 1;
            airScreamerProps.flags      |= AirScreamerFlag_5;
            airScreamer->flags          |= CharaFlag_Hit;
        }
    }
    else if (ANIM_STATUS(ANIM_STATUS_IDX_GET(animStatus), true) == ANIM_STATUS(AirScreamerAnim_HoverVariable, true))
    {
        airScreamer->model.stateStep = 2;
        airScreamerProps.timer_120       = Q12(4.0f);
    }

    sharedFunc_800D529C_0_s01(airScreamer, Q12(1.0f), func_80080478(&airScreamer->position, &g_SysWork.playerWork.player.position));
    sharedFunc_800D598C_0_s01(airScreamer);

    switch (AirScreamer_DamageTake(airScreamer, Q12(1.0f)))
    {
        case AirScreamerHit_None:
            if (airScreamerProps.timer_120 == Q12(0.0f) ||
                airScreamer == &g_SysWork.npcs[g_SysWork.targetNpcIdx] ||
                Math_Distance2dGet(&airScreamer->position, &g_SysWork.playerWork.player.position) > Q12(6.5f))
            {
                airScreamer->model.controlState = AirScreamerControl_47;
                airScreamer->model.stateStep    = 0;
                airScreamerProps.flags         |= AirScreamerFlag_4;
            }
            break;

        default:
            break;

        case AirScreamerHit_Recoil:
        case AirScreamerHit_Rear:
            airScreamer->model.controlState = AirScreamerControl_50;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            break;

        case AirScreamerHit_Stun:
        case AirScreamerHit_Kill:
            airScreamer->model.controlState = AirScreamerControl_51;
            airScreamer->model.stateStep    = 0;

            if (airScreamer->health <= Q12(0.0f))
            {
                airScreamerProps.flags |= AirScreamerFlag_6;
            }
            else
            {
                airScreamerProps.flags |= AirScreamerFlag_3;
            }
            break;
    }
#endif
}

void AirScreamer_Control_47(s_SubCharacter* airScreamer)
{
#ifdef MAP0_S01
    q19_12 dist0;
    q19_12 deltaY;
    q19_12 unkAngleDelta;
    q19_12 dist;

    // Handle state step.
    switch (airScreamer->model.stateStep)
    {
        case 0:
            airScreamerProps.timer_120       = Q12(1.5f);
            airScreamer->model.stateStep = 1;
            break;

        case 1:
            if (Math_Distance2dGet(&airScreamer->position, &airScreamerProps.targetPosition) < Q12(0.5f))
            {
                airScreamer->model.stateStep = 0;
            }
            break;
    }

    sharedFunc_800D53AC_0_s01(airScreamer);
    sharedFunc_800D57C8_0_s01(airScreamer);

    switch (AirScreamer_DamageTake(airScreamer, Q12(1.0f)))
    {
        case AirScreamerHit_None:
            dist0  = NO_VALUE;
            deltaY = airScreamerProps.targetPosition.vy - airScreamer->position.vy;
            if (deltaY > Q12(-0.2f) && deltaY < Q12(0.8f))
            {
                dist0 = airScreamer->collision.cylinder.radius + Q12(0.15f);
            }

            dist = Math_Distance2dGet(&airScreamer->position, &airScreamerProps.targetPosition);
            if (dist < dist0)
            {
                unkAngleDelta = Q12_ANGLE_NORM_S(func_80080478(&airScreamer->position, &airScreamerProps.targetPosition) - airScreamer->rotation.vy);
                if (unkAngleDelta >= Q12_ANGLE(-8.0f) && unkAngleDelta < Q12_ANGLE(8.0f))
                {
                    airScreamer->model.controlState = AirScreamerControl_49;
                    airScreamer->model.stateStep    = 0;
                }
            }
            break;

        case AirScreamerHit_Recoil:
        case AirScreamerHit_Rear:
            airScreamer->model.controlState = AirScreamerControl_50;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            break;

        case AirScreamerHit_Stun:
        case AirScreamerHit_Kill:
            airScreamer->model.controlState = AirScreamerControl_51;
            airScreamer->model.stateStep    = 0;

            if (airScreamer->health <= Q12(0.0f))
            {
                airScreamerProps.flags |= AirScreamerFlag_6;
            }
            else
            {
                airScreamerProps.flags |= AirScreamerFlag_3;
            }
            break;
    }
#endif
}

void AirScreamer_Control_48(s_SubCharacter* airScreamer)
{
#ifdef MAP0_S01
    if (!airScreamer->model.stateStep)
    {
        airScreamerProps.timer_120       = Q12(2.0f);
        airScreamer->model.stateStep = 1;
    }

    sharedFunc_800D53AC_0_s01(airScreamer);
    sharedFunc_800D5B10_0_s01(airScreamer);

    switch (AirScreamer_DamageTake(airScreamer, Q12(1.0f)))
    {
        case AirScreamerHit_None:
            if (airScreamerProps.timer_120 == Q12(0.0f) ||
                Math_Distance2dGet(&airScreamer->position, &g_SysWork.playerWork.player.position) > Q12(2.0f))
            {
                airScreamer->model.controlState = AirScreamerControl_47;
                airScreamer->model.stateStep    = 0;
                break;
            }

        default:
            break;

        case AirScreamerHit_Recoil:
        case AirScreamerHit_Rear:
            airScreamer->model.controlState = AirScreamerControl_50;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            break;

        case AirScreamerHit_Stun:
        case AirScreamerHit_Kill:
            airScreamer->model.controlState = AirScreamerControl_51;
            airScreamer->model.stateStep    = 0;

            if (airScreamer->health <= Q12(0.0f))
            {
                airScreamerProps.flags |= AirScreamerFlag_6;
            }
            else
            {
                airScreamerProps.flags |= AirScreamerFlag_3;
            }
            break;
    }
#endif
}

void AirScreamer_Control_49(s_SubCharacter* airScreamer)
{
#ifdef MAP0_S01
    bool cond0;
    bool cond1;
    s32  animStatus;

    cond0      = false;
    animStatus = airScreamer->model.anim.status;
    cond1      = false;

    // Handle state step.
    switch (airScreamer->model.stateStep)
    {
        case 0:
            airScreamer->model.stateStep = 1;

        case 1:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_HoverVariable, true))
            {
                airScreamer->model.stateStep   = 2;
                airScreamer->model.anim.status = AirScreamerAnim_StandToStun;
            }
            break;

        case 2:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_HoverBiteAttack, true) && airScreamer->model.anim.keyframeIdx < 8192)
            {
                airScreamer->model.stateStep = 3;
                airScreamerProps.flags      |= AirScreamerFlag_5;
            }
            break;

        case 3:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_HoverVariable, true))
            {
                cond0 = true;
            }
            else if (airScreamer->field_44.field_8 == NO_VALUE)
            {
                cond0                            = true;
                cond1                            = true;
                airScreamer->model.stateStep = 4;
            }
            break;

        case 4:
            if (animStatus == ANIM_STATUS(AirScreamerAnim_HoverVariable, true))
            {
                cond0 = true;
                cond1 = true;
            }
            break;
    }

    sharedFunc_800D53AC_0_s01(airScreamer);
    sharedFunc_800D5C90_0_s01(airScreamer);

    switch (AirScreamer_DamageTake(airScreamer, Q12(1.2f)))
    {
        case AirScreamerHit_None:
            if (cond0)
            {
                if (!cond1)
                {
                    if (Rng_RandQ12() >= Q12_ANGLE(180.0f))
                    {
                        airScreamer->model.controlState = AirScreamerControl_47;
                    }
                    else
                    {
                        airScreamer->model.controlState = AirScreamerControl_48;
                    }
                }
                else
                {
                    airScreamer->model.controlState = AirScreamerControl_48;
                }

                airScreamer->model.stateStep = 0;
                break;
            }

        default:
            break;

        case AirScreamerHit_Recoil:
        case AirScreamerHit_Rear:
            airScreamer->model.controlState = AirScreamerControl_50;
            airScreamer->model.stateStep    = 0;
            airScreamerProps.flags         |= AirScreamerFlag_3;
            break;

        case AirScreamerHit_Stun:
        case AirScreamerHit_Kill:
            airScreamer->model.controlState = AirScreamerControl_51;
            airScreamer->model.stateStep    = 0;

            if (airScreamer->health <= Q12(0.0f))
            {
                airScreamerProps.flags |= AirScreamerFlag_6;
            }
            else
            {
                airScreamerProps.flags |= AirScreamerFlag_3;
            }
            break;
    }
#endif
}

void AirScreamer_Control_50(s_SubCharacter* airScreamer)
{
#ifdef MAP0_S01
    s32  damageType;
    s32  animStatus;
    u32  stateStep;
    bool cond;

    stateStep  = airScreamer->model.stateStep;
    animStatus = airScreamer->model.anim.status;
    cond       = false;

    // Handle state step.
    switch (stateStep)
    {
        case 0:
            if (ANIM_STATUS_IS_ACTIVE(animStatus))
            {
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_HoverRecoil, false);
                airScreamer->model.stateStep   = 1;
            }
            break;

        case 1:
            if (animStatus != ANIM_STATUS(AirScreamerAnim_HoverRecoil, false))
            {
                airScreamer->model.stateStep = 2;
                airScreamerProps.flags      |= AirScreamerFlag_3;
            }
            break;

        case 2:
            if (animStatus != ANIM_STATUS(AirScreamerAnim_HoverRecoil, true))
            {
                cond = true;
            }
            break;
    }

    sharedFunc_800D5D80_0_s01(airScreamer);

    switch (AirScreamer_DamageTake(airScreamer, Q12(0.6f)))
    {
        case AirScreamerHit_None:
        case AirScreamerHit_Recoil:
        case AirScreamerHit_Rear:
            if (cond)
            {
                airScreamer->model.controlState = AirScreamerControl_47;
                airScreamer->model.stateStep    = 0;
                airScreamerProps.field_E8_8         = 3;
            }
            break;

        case AirScreamerHit_Stun:
        case AirScreamerHit_Kill:
            airScreamer->model.controlState = AirScreamerControl_51;
            airScreamer->model.stateStep    = 0;

            if (airScreamer->health <= Q12(0.0f))
            {
                airScreamerProps.flags |= AirScreamerFlag_6;
            }
            else
            {
                airScreamerProps.flags |= AirScreamerFlag_3;
            }
            break;
    }
#endif
}

void AirScreamer_Control_51(s_SubCharacter* airScreamer)
{
#ifdef MAP0_S01
    s32  animStatus;
    u32  stateStep;
    bool cond;

    stateStep  = airScreamer->model.stateStep;
    animStatus = airScreamer->model.anim.status;
    cond       = false;

    // Handle state step.
    switch (stateStep)
    {
        case 0:
            if (ANIM_STATUS_IS_ACTIVE(animStatus))
            {
                airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_HoverToStun, false);
                airScreamer->model.stateStep   = 1;
            }
            break;

        case 1:
            if (animStatus != ANIM_STATUS(AirScreamerAnim_HoverToStun, false))
            {
                airScreamer->model.stateStep = 2;
                airScreamerProps.flags      |= AirScreamerFlag_3;
            }
            break;

        case 2:
            if (animStatus != ANIM_STATUS(AirScreamerAnim_HoverToStun, true))
            {
                cond = true;
            }
            break;
    }

    sharedFunc_800D5E14_0_s01(airScreamer);
    AirScreamer_DamageTake(airScreamer, Q12(0.6f));

    if (cond)
    {
        airScreamer->model.controlState = AirScreamerControl_2;
        airScreamer->model.stateStep    = 0;
        airScreamerProps.field_E8_8         = 1;
    }
#endif
}

s32 sharedFunc_800D4A80_0_s01(s_SubCharacter* airScreamer)
{
    s32 flags;
    s32 result;

    flags  = airScreamerProps.flags;
    result = 0;
    if (sharedData_800E21D0_0_s01.flags >= 0 && airScreamer->health >= Q12(0.0f))
    {
        result = 4;
        if (!(flags & AirScreamerFlag_17))
        {
            result = 1;
            if (!(flags & AirScreamerFlag_18))
            {
                result = 2;
                if (!(flags & AirScreamerFlag_19))
                {
                    result = NO_VALUE;
                    if (flags & AirScreamerFlag_20)
                    {
                        result = 3;
                    }
                }
            }
        }
    }

    return result;
}

#ifndef MAP0_S01

bool sharedFunc_800DBF88_2_s00(s_SubCharacter* airScreamer, q19_12 arg1)
{
    q19_12   posX;
    q19_12   posY;
    q19_12   posZ;
    q19_12   posDeltaX;
    q19_12   posDeltaY;
    q19_12   posDeltaZ;
    q19_12   charaFieldC8;
    s32      unkPosY;
    VECTOR3* unkVec;

    if (sharedData_800E21D0_0_s01.flags & (1 << 27))
    {
        unkVec = &airScreamerProps.targetPosition2;
    }
    else
    {
        unkVec = &airScreamerProps.targetPosition;
    }

    posX = airScreamer->position.vx;
    posY = airScreamer->position.vy;
    posZ = airScreamer->position.vz;

    posDeltaX = unkVec->vx - posX;
    posDeltaY = unkVec->vy - posY;
    posDeltaZ = unkVec->vz - posZ;

    unkPosY = unkVec->vy;

    charaFieldC8 = airScreamer->collision.box.top;

    func_8006F250(sharedData_800E2370_0_s01, posX, posZ, posDeltaX, posDeltaZ);

    if (arg1 >= sharedData_800E2370_0_s01[0] && (Collision_GroundHeightGet(posX, posZ) + charaFieldC8) >= sharedData_800E2370_0_s01[1])
    {
        if (posDeltaY < Q12(0.0f))
        {
            if ((posY + charaFieldC8) < sharedData_800E2370_0_s01[1])
            {
                return true;
            }
        }
        else
        {
            if ((unkPosY + charaFieldC8) < sharedData_800E2370_0_s01[1])
            {
                return true;
            }
        }
    }

    return false;
}

bool AirScreamer_IsGroundedCheck(s_SubCharacter* airScreamer)
{
    return (Collision_GroundHeightGet(airScreamer->position.vx, airScreamer->position.vz) - Q12(0.2f)) < airScreamer->position.vy;
}

bool sharedFunc_800DC0E4_2_s00(s_SubCharacter* airScreamer, q19_12 moveSpeedMult)
{
    q19_12 headingAngle;
    q19_12 moveSpeed;
    q19_12 posX;
    q19_12 posZ;

    moveSpeed    = airScreamer->moveSpeed;
    posX         = airScreamer->position.vx;
    posZ         = airScreamer->position.vz;
    headingAngle = airScreamer->headingAngle;

    moveSpeed = Q12_MULT_PRECISE(moveSpeed, moveSpeedMult);
    posX     += Q12_MULT_PRECISE(Math_Sin(headingAngle), moveSpeed);
    posZ     += Q12_MULT_PRECISE(Math_Cos(headingAngle), moveSpeed);

    switch (func_800808AC(posX, posZ))
    {
        case 0:
        case 7:
        case 12:
            return true;
    }

    return (Collision_GroundHeightGet(posX, posZ) + Q12(1.2f)) > airScreamer->position.vy;
}

bool sharedFunc_800DC200_2_s00(s_SubCharacter* airScreamer)
{
    if ((g_SysWork.field_2388.field_154.effectsInfo.field_0.field_00[0] & 0x1) &&
        (g_SavegamePtr->gameDifficulty <= GameDifficulty_Normal || airScreamer->model.charaId == Chara_AirScreamer))
    {
        return false;
    }

    sharedData_800F216C_2_s00.vx = airScreamer->position.vx + Q12_MULT_PRECISE(Math_Sin(airScreamer->rotation.vy), Q12(2.0f));
    sharedData_800F216C_2_s00.vy = airScreamer->position.vy + Q12(0.5f);
    sharedData_800F216C_2_s00.vz = airScreamer->position.vz + Q12_MULT_PRECISE(Math_Cos(airScreamer->rotation.vy), Q12(2.0f));

    return sharedFunc_800D4AEC_0_s01(airScreamer, NULL, &sharedData_800F216C_2_s00, NULL) != 0;
}

bool sharedFunc_800DC30C_2_s00(s_SubCharacter* airScreamer)
{
    func_8006F250(sharedData_800E2370_0_s01, airScreamer->position.vx, airScreamer->position.vz, Q12(0.0f), Q12(0.0f));

    switch (func_800808AC(airScreamer->position.vx, airScreamer->position.vz))
    {
        case 0:
        case 7:
        case 12:
            return true;
    }

    return (Collision_GroundHeightGet(airScreamer->position.vx, airScreamer->position.vz) + (airScreamer->collision.box.top * 2)) >= sharedData_800E2370_0_s01[1];
}

bool sharedFunc_800DC3BC_2_s00(s_SubCharacter* airScreamer)
{
    switch (func_800808AC(airScreamer->position.vx, airScreamer->position.vz))
    {
        case 0:
        case 7:
        case 12:
            return false;
    }

    return Collision_GroundHeightGet(airScreamer->position.vx, airScreamer->position.vz) <= Q12(3.0f);
}

s32 sharedFunc_800DC438_2_s00(s_SubCharacter* airScreamer)
{
    s32 stateStep;

    stateStep = 0;
    if (airScreamer->model.anim.status != ANIM_STATUS(AirScreamerAnim_Glide, true))
    {
        return stateStep;
    }

    if (!(airScreamerProps.flags & AirScreamerFlag_29) &&
        (!(g_SysWork.field_2388.field_154.effectsInfo.field_0.field_00[0] & (1 << 0)) ||
         (g_SavegamePtr->gameDifficulty > GameDifficulty_Normal && airScreamer->model.charaId != Chara_AirScreamer)))
    {
        if (sharedFunc_800DC3BC_2_s00(airScreamer) && (AirScreamer_IsGroundedCheck(airScreamer) || sharedFunc_800DBF88_2_s00(airScreamer, Q12(2.0f))))
        {
            if (sharedFunc_800DC0E4_2_s00(airScreamer, Q12(1.0f)))
            {
                stateStep = 1;
            }
            else
            {
                stateStep = 2;
            }
        }
    }
    else
    {
        stateStep = 1;
    }

    return stateStep;
}

bool AirScreamer_HasLandedCheck(s_SubCharacter* airScreamer)
{
    return (airScreamer->model.anim.status == ANIM_STATUS(AirScreamerAnim_HoverVariable, true) || airScreamer->model.anim.status == ANIM_STATUS(AirScreamerAnim_HoverConstant, true)) &&
           sharedFunc_800DC3BC_2_s00(airScreamer) &&
           ((airScreamer->fallSpeed >= 0 && AirScreamer_IsGroundedCheck(airScreamer)) || sharedFunc_800DBF88_2_s00(airScreamer, Q12(1.0f)));
}

s32 sharedFunc_800DC598_2_s00(s_SubCharacter* airScreamer)
{
    s32 ret;
    u8  animStatus;

    ret        = 0;
    animStatus = airScreamer->model.anim.status;

    if (animStatus == ANIM_STATUS(AirScreamerAnim_WalkForward, true) ||
        animStatus == ANIM_STATUS(AirScreamerAnim_StandIdle, true))
    {
        if (airScreamer->position.vy < (Collision_GroundHeightGet(airScreamer->position.vx, airScreamer->position.vz) - Q12(0.7f)))
        {
            if (sharedFunc_800DC200_2_s00(airScreamer) && airScreamer->moveSpeed > Q12(1.5f))
            {
                ret = 2;
            }
            else
            {
                func_8006F250(sharedData_800E2370_0_s01, airScreamer->position.vx, airScreamer->position.vz, 0, 0);

                if (sharedData_800E2370_0_s01[1] < ((airScreamer->position.vy + airScreamer->collision.box.top) - Q12(2.0f)))
                {
                    ret = 1;
                }
            }
        }
    }

    return ret;
}

bool sharedFunc_800DC67C_2_s00(s_SubCharacter* airScreamer)
{
    if (airScreamerProps.field_E8_0 == 1)
    {
        func_800803FC(&sharedData_800F217C_2_s00, airScreamer->field_40);
        return Math_Distance2dGet(&airScreamer->position, &sharedData_800F217C_2_s00) > Q12(CHUNK_SIZE);
    }

    return false;
}

q19_12 sharedFunc_800DC6E4_2_s00(s_SubCharacter* airScreamer, q19_12 arg1)
{
    q19_12 chance;
    q19_12 mult;

    mult = airScreamerProps.field_E8_4 ? Q12(0.1f) : Q12(0.0f);

    chance = Q12(0.0f);
    switch ((u32)airScreamerProps.field_E8_8)
    {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
            break;

        case 5:
            chance  = MAX((Q12(10.0f) - arg1) / 128, Q12(0.0f));
            chance += (sharedFunc_800D3814_0_s01(airScreamer) / 5) - (mult * 2);
            break;
    }

    if (airScreamer->health > Q12(300.0f))
    {
        chance /= 2;
    }

    switch ((u32)airScreamerProps.field_E8_0)
    {
        case 3:
            chance = Q12(0.0f);
            break;

        case 1:
            // TODO: Not sure what this is trying to do, getting index into `npcs` in strange way?
            if (!((g_SysWork.npcs - airScreamer) & 0x1))
            {
                chance = Q12(0.0f);
            }
            else
            {
                chance /= 2;
            }
            break;

        case 0:
            if (!((g_SysWork.npcs - airScreamer) & 0x1))
            {
                chance /= 2;
            }
            break;
    }

    return chance;
}

q19_12 sharedFunc_800DC894_2_s00(s_SubCharacter* airScreamer, q19_12 dist)
{
    q19_12 extraChance;
    q19_12 baseChance;

    extraChance = airScreamerProps.field_E8_4 ? Q12(0.1f) : Q12(0.0f);

    baseChance = Q12(0.0f);
    switch (airScreamerProps.field_E8_8)
    {
        case 3:
            baseChance = Q12(1.0f);
            break;

        case 2:
            baseChance = Q12(0.7f) + (extraChance * 2);
            break;

        case 0:
            // @unused ASM includes an `ADDIU 0x7F` which then gets overwritten.
            // Divide by 128 is assumed code which also uses `ADDIU 0x7F`.
            // Likely the rest of divide/MAX code is optimized out since `var_s0` is immediately overwritten.
            // (also possible last line was meant to be `var_s0 +=` like in case 1 below, bug?
            baseChance = MAX((Q12(10.0f) - dist) / 128, Q12(0.0f));
            baseChance = Q12(0.4f) + (extraChance * 2);
            break;

        case 5:
            baseChance = Q12(0.2f) + (extraChance * 4);
            break;

        case 1:
        case 4:
            baseChance  = MAX((Q12(10.0f) - dist) / 64, Q12(0.0f));
            baseChance += Q12(0.4f) + (extraChance * 2);
            break;
    }

    return baseChance + (sharedFunc_800D3814_0_s01(airScreamer) - Q12(1.0f)) / 8;
}

#endif

bool sharedFunc_800D4AEC_0_s01(s_SubCharacter* airScreamer, VECTOR3* arg1, VECTOR3* arg2, VECTOR3* arg3)
{
    bool     hasLosHit;
    s32      temp_v1;
    s32      prevCollState;
    q19_12   groundHeight;
    q19_12   groundOffset;
    q19_12   offsetX;
    q19_12   offsetZ;
    s32      var_a0;
    s32      i;
    s32      var_v1;
    bool     hasHit;
    q19_12   posX;
    q19_12   posY;
    q19_12   posZ;
    s32      temp;
    s32      temp2;
    s32      temp3;
    VECTOR3* var_s0;

    if (arg1 == NULL)
    {
        var_s0 = &airScreamer->position;
    }
    else
    {
        var_s0 = arg1;
    }

    posX = var_s0->vx;
    posY = var_s0->vy;
    posZ = var_s0->vz;

    offsetX = arg2->vx - posX;
    offsetZ = arg2->vz - posZ;

    sharedData_800DE1A0_0_s01.vx = offsetX;
    sharedData_800DE1A0_0_s01.vz = offsetZ;

    if (sharedFunc_800D4A80_0_s01(airScreamer) == 1)
    {
        i                            = 0;
        sharedData_800DE1A0_0_s01.vy = Q12(0.0f);
    }
    else
    {
        i                            = arg2->vy - posY;
        sharedData_800DE1A0_0_s01.vy = i;
    }

    prevCollState = g_SysWork.playerWork.player.collision.state;

    g_SysWork.playerWork.player.collision.state = CharaCollisionState_Ignore;

    hasLosHit = Ray_LosHitCheck(&sharedData_800E2330_0_s01, var_s0, &sharedData_800DE1A0_0_s01, airScreamer);

    g_SysWork.playerWork.player.collision.state = prevCollState;
    offsetX                                     = sharedData_800E2330_0_s01.target.vx - posX;
    offsetZ                                     = sharedData_800E2330_0_s01.target.vz - posZ;

    func_8006F250(sharedData_800E2370_0_s01, posX, posZ, offsetX, offsetZ);

    var_a0    = sharedData_800E2370_0_s01[1];
    temp_v1   = airScreamer->collision.box.top;
    hasHit = true;

    if (i < 0 && var_a0 < (posY + temp_v1))
    {
        if ((sharedData_800E2330_0_s01.target.vy + temp_v1) < var_a0)
        {
            sharedData_800E2330_0_s01.target.vy = var_a0 - temp_v1;
        }
    }
    else if ((sharedData_800E2330_0_s01.target.vy + temp_v1) < var_a0)
    {
        temp = sharedData_800E2370_0_s01[0];

        if (temp < sharedData_800E2330_0_s01.hitDistance)
        {
            if (sharedData_800E2330_0_s01.hitDistance == Q12(0.0f))
            {
                var_v1 = 0;
            }
            else
            {
                var_v1 = FP_TO(temp, Q12_SHIFT) / sharedData_800E2330_0_s01.hitDistance;
            }

            hasLosHit  = true;
            temp3 = Q12_MULT_PRECISE(offsetX, var_v1);
            temp2 = Q12_MULT_PRECISE(i, var_v1);

            sharedData_800E2330_0_s01.hitDistance  = temp;
            sharedData_800E2330_0_s01.hasHit       = hasHit;
            sharedData_800E2330_0_s01.target.vx    = temp3 + posX;
            sharedData_800E2330_0_s01.target.vy    = temp2 + posY;
            sharedData_800E2330_0_s01.groundHeight = sharedData_800E2330_0_s01.target.vy;

            temp2                                 = Q12_MULT_PRECISE(offsetZ, var_v1);
            sharedData_800E2330_0_s01.target.vz  = temp2 + posZ;
            sharedData_800E2330_0_s01.target.vy += Q12(0.0f); // @hack
        }
    }

    if (arg3 != NULL)
    {
        *arg3 = sharedData_800E2330_0_s01.target;

        if (hasLosHit)
        {
            if (sharedFunc_800D5274_0_s01() < sharedData_800E2330_0_s01.groundHeight)
            {
                groundOffset                         = (Rng_RandQ12() / 2) + Q12(1.5f);
                sharedData_800E2330_0_s01.target.vy = sharedData_800E2330_0_s01.groundHeight - groundOffset;
            }
            else
            {
                groundHeight  = Collision_GroundHeightGet(airScreamer->position.vx, airScreamer->position.vz);
                groundOffset  = (Rng_RandQ12() / 2) + Q12(1.5f);
                groundHeight -= groundOffset;

                groundOffset                         = (Rng_RandQ12() / 2) - Q12(0.25f);
                sharedData_800E2330_0_s01.target.vy = airScreamer->position.vy - groundOffset;

                if (groundHeight < sharedData_800E2330_0_s01.target.vy)
                {
                    sharedData_800E2330_0_s01.target.vy = groundHeight;
                }
            }
        }
    }

    return !hasLosHit;
}

void sharedFunc_800D4E84_0_s01(s_SubCharacter* airScreamer)
{
    q19_12   distToTarget;
    q19_12   posX;
    q19_12   posY;
    q19_12   posZ;
    s32      targetPosY;
    q19_12   dist2;
    s32      new_var;
    VECTOR3* pos;
    s32      rotY;
    q19_12   temp_a0;
    q19_12   dist5;
    s32      temp_hi;
    q19_12   dist1;
    q19_12   angleDeltaToTarget;
    q19_12   angleToTarget;
    q19_12   dist3;
    s32      temp_v0_3;
    s32      temp_v0_4;
    s32      dist4;
    s32      dist2_1_2;
    q19_12   curAngle;
    s32      i;
    s32      var_s5;
    s32      var_s7;

    dist3 = Q12(8.0f);

    pos = &airScreamer->position;

    distToTarget  = Math_Distance2dGet(pos, &airScreamerProps.targetPosition);
    angleToTarget = func_80080478(pos, &airScreamerProps.targetPosition);

    rotY = airScreamer->rotation.vy;
    posX = airScreamer->position.vx;
    posY = airScreamer->position.vy;
    posZ = airScreamer->position.vz;

    angleDeltaToTarget = Q12_ANGLE_NORM_S(angleToTarget - rotY);
    targetPosY         = airScreamerProps.targetPosition.vy;

    if (sharedFunc_800D4AEC_0_s01(airScreamer, NULL, &airScreamerProps.targetPosition, &g_AirScreamer_TargetPosition2))
    {
        airScreamerProps.targetPosition2 = g_AirScreamer_TargetPosition2;
        return;
    }

    sharedData_800E21D0_0_s01.flags |= (1 << 27);

    if (angleDeltaToTarget > Q12_ANGLE(-11.34f) && angleDeltaToTarget < Q12_ANGLE(11.34f))
    {
        sharedData_800DE1C0_0_s01.vx = Math_Sin(rotY) * 2;
        sharedData_800DE1C0_0_s01.vy = Q12(0.0f);
        sharedData_800DE1C0_0_s01.vz = Math_Cos(rotY) * 2;

        if (Ray_LosHitCheck(&sharedData_800E2330_0_s01, pos, &sharedData_800DE1C0_0_s01, airScreamer) &&
            sharedFunc_800D5274_0_s01() < sharedData_800E2330_0_s01.groundHeight)
        {
            airScreamerProps.targetPosition2.vx = sharedData_800E2330_0_s01.target.vx;
            airScreamerProps.targetPosition2.vy = sharedData_800E2330_0_s01.groundHeight - Q12(1.5f);
            airScreamerProps.targetPosition2.vz = sharedData_800E2330_0_s01.target.vz;
            return;
        }
    }

    dist2 = distToTarget - Math_Distance2dGet(&airScreamer->position, &g_AirScreamer_TargetPosition2) + dist3;

    var_s5 = INT_MAX;
    var_s7 = Q12(1.0f);

    for (i = 0, curAngle = (rotY + angleDeltaToTarget / 2) - Q12_ANGLE(90.0f); i < 9; i++, curAngle += Q12_ANGLE(22.5f))
    {
        g_AirScreamer_TargetPosition2.vx = posX + Q12_MULT_PRECISE(Math_Sin(curAngle), Q12(8.0f));
        g_AirScreamer_TargetPosition2.vy = targetPosY;
        g_AirScreamer_TargetPosition2.vz = posZ + Q12_MULT_PRECISE(Math_Cos(curAngle), Q12(8.0f));

        new_var = dist2;
        if (!sharedFunc_800D4AEC_0_s01(airScreamer, NULL, &g_AirScreamer_TargetPosition2, &g_AirScreamer_TargetPosition2))
        {
            dist1 = Math_Distance2dGet(&airScreamer->position, &g_AirScreamer_TargetPosition2);
        }
        else
        {
            dist1 = Q12(8.0f);
        }

        dist3   = Math_Distance2dGet(&g_AirScreamer_TargetPosition2, &airScreamerProps.targetPosition);
        temp_a0 = Q12_MULT_PRECISE(new_var - dist1, dist3);

        temp_v0_3 = Q12_ANGLE_NORM_S(curAngle - rotY);

        temp_v0_4 = temp_v0_3 >> 31;
        temp_hi   = (temp_v0_3 ^ temp_v0_4) - temp_v0_4;
        dist2_1_2 = temp_hi;

        if (temp_a0 < var_s5 || (temp_a0 == var_s5 && dist2_1_2 < var_s7))
        {
            var_s5                        = temp_a0;
            var_s7                        = dist2_1_2;
            airScreamerProps.targetPosition2 = g_AirScreamer_TargetPosition2;
        }
    }

    dist1 = Math_Distance2dGet(&airScreamerProps.targetPosition2, &airScreamer->position);
    dist4 = Math_Distance2dGet(&airScreamerProps.targetPosition2, &airScreamerProps.targetPosition);

    dist5 = dist1 + dist4;
    if (dist5 == Q12(0.0f))
    {
        airScreamerProps.targetPosition2.vy = targetPosY;
        return;
    }

    airScreamerProps.targetPosition2.vy = FP_TO(Q12_MULT_PRECISE(targetPosY, dist1) +
                                                 Q12_MULT_PRECISE(posY, dist4),
                                             Q12_SHIFT) /
                                       dist5;

    sharedFunc_800D4AEC_0_s01(airScreamer, NULL, &g_AirScreamer_TargetPosition2, &g_AirScreamer_TargetPosition2);
}

q19_12 sharedFunc_800D5274_0_s01(void)
{
    return MIN(Q12(-3.0f), g_SysWork.playerWork.player.position.vy - Q12(2.0f));
}

#ifndef MAP0_S01
void sharedFunc_800DD13C_2_s00(s_SubCharacter* airScreamer, s32 npcSlot, q19_12 spawnChance)
{
    u32 flags;
    s32 i;
    s32 counter;
    s32 bitsSet;

    bitsSet = 0;
    flags   = g_SysWork.npcFlags;
    counter = 0;

    if (spawnChance < Rng_RandQ12())
    {
        return;
    }

    if (g_VBlanks >= 4)
    {
        return;
    }

    // @bug This loops 32 times, but `npcs` only has 6 entries. Only accesses `npcs` when bit is set inside `flags_2290` first.
    for (i = 0; i < 32; i++)
    {
        if (!(flags & (1 << i)))
        {
            continue;
        }

        bitsSet++;

        // Check if character is Air Screamer.
        if (g_SysWork.npcs[i].model.charaId != airScreamer->model.charaId)
        {
            continue;
        }

        switch (g_SysWork.npcs[i].model.controlState)
        {
            case AirScreamerControl_2:
            case AirScreamerControl_Stun:
            case AirScreamerControl_30:
            case AirScreamerControl_32:
            case AirScreamerControl_45:
                counter += 2;
                break;

            default:
                counter += 3;
                break;
        }
    }

    // If free slot in `npcs` exists and some counter is not over 5 (idle, aggro? currently animating?)
    if (bitsSet < 32)
    {
        counter = (counter + 2) / 3;
        if (counter < 5)
        {
            s32 selectNpcSlot = npcSlot & 0x1F;
            CLEAR_FLAG(g_SysWork.field_228C, selectNpcSlot);

            Chara_Spawn(airScreamer->model.charaId,
                        selectNpcSlot,
                        g_SysWork.playerWork.player.position.vx + Q12(20.0f),
                        g_SysWork.playerWork.player.position.vz, Q12_ANGLE(0.0f),
                        10);
        }
    }
}

void sharedFunc_800DD2C4_2_s00(s_SubCharacter* airScreamer, s32 arg1, s32 arg2)
{
    q19_12 playerPosX;
    q19_12 playerPosZ;
    q19_12 newPosX;
    q19_12 newPosZ;
    q19_12 dist;
    q19_12 headingAngle;
    q19_12 angleStep;
    q19_12 unkDeltaAngle;
    s32    i;

    newPosX    = airScreamer->position.vx;
    newPosZ    = airScreamer->position.vz;
    playerPosX = g_SysWork.playerWork.player.position.vx;
    playerPosZ = g_SysWork.playerWork.player.position.vz;

    unkDeltaAngle = Q12_ANGLE_NORM_S(airScreamer->rotation.vy - ratan2(newPosX - playerPosX, newPosZ - playerPosZ));
    headingAngle  = airScreamer->rotation.vy + Q12_ANGLE(180.0f);
    angleStep     = unkDeltaAngle > Q12_ANGLE(0.0f);
    angleStep     = (arg2 != angleStep) ? Q12_ANGLE(45.0f) : Q12_ANGLE(-45.0f);

    airScreamerProps.targetPosition.vx = playerPosX;
    airScreamerProps.targetPosition.vz = playerPosZ;

    for (i = 8; i > 0; i--, headingAngle += angleStep)
    {
        dist    = Q12_MULT_PRECISE(Rng_RandQ12(), Q12(6.0f)) + Q12(19.0f);
        newPosX = playerPosX + Q12_MULT_PRECISE(dist, Math_Sin(headingAngle));
        newPosZ = playerPosZ + Q12_MULT_PRECISE(dist, Math_Cos(headingAngle));

        unkDeltaAngle = Collision_GroundHeightGet(newPosX, newPosZ);
        unkDeltaAngle = MIN(unkDeltaAngle, 0);

        airScreamer->position.vy           = unkDeltaAngle + arg1;
        airScreamer->position.vx           = newPosX;
        airScreamer->position.vz           = newPosZ;
        airScreamerProps.targetPosition.vy = airScreamer->position.vy;

        if (func_8008F914(newPosX, newPosZ) && func_800808AC(newPosX, newPosZ) &&
            sharedFunc_800D4AEC_0_s01(airScreamer, NULL, &airScreamerProps.targetPosition, NULL))
        {
            break;
        }
    }

    if (i == 0)
    {
        airScreamer->position.vx = playerPosX + Q12(50.0f);
        airScreamer->position.vy = sharedFunc_800D5274_0_s01() * 2;
        airScreamer->position.vz = playerPosZ + Q12(50.0f);
    }
}

void sharedFunc_800DD4A4_2_s00(s_SubCharacter* airScreamer)
{
    sharedFunc_800DD2C4_2_s00(airScreamer, Q12(0.0f), 0);
    sharedFunc_800DDF74_2_s00(airScreamer, Q12(30.0f), airScreamer->rotation.vy);
    airScreamerProps.timer_120 = Q12(15.0f);
}

void sharedFunc_800DD4EC_2_s00(s_SubCharacter* airScreamer)
{
    sharedFunc_800DD2C4_2_s00(airScreamer, Q12(-2.2f), 0);
    sharedFunc_800D529C_0_s01(airScreamer, Q12(30.0f), airScreamer->rotation.vy);
    airScreamerProps.timer_120 = Q12(12.0f);
}

void sharedFunc_800DD534_2_s00(s_SubCharacter* airScreamer)
{
    sharedFunc_800DD2C4_2_s00(airScreamer, Q12(-2.2f), 1);
    sharedFunc_800DEC84_2_s00(airScreamer, Q12(30.0f), airScreamer->rotation.vy);

    airScreamer->moveSpeed  = sharedData_800CAA98_0_s01.field_380[9][0];
    airScreamerProps.timer_120 = Q12(10.0f);
}

void sharedFunc_800DD588_2_s00(s_SubCharacter* airScreamer)
{
    #define ANGLE_STEP_COUNT 16

    q19_12 posX;
    q19_12 posY;
    q19_12 posZ;
    q19_12 rotY;
    q19_12 curPosX;
    q19_12 curPosY;
    q19_12 curPosZ;
    q19_12 curAngle;
    q19_12 newAngle;
    q19_12 angleStep;
    q19_12 sinRotY;
    s32    i;

    posX = g_SysWork.playerWork.player.position.vx;
    posY = g_SysWork.playerWork.player.position.vy;
    posZ = g_SysWork.playerWork.player.position.vz;
    rotY = g_SysWork.playerWork.player.rotation.vy;

    if (Rng_TestProbability(Q12(0.5f)))
    {
        angleStep = Q12_ANGLE(360.0f / ANGLE_STEP_COUNT);
    }
    else
    {
        angleStep = Q12_ANGLE(-(360.0f / ANGLE_STEP_COUNT));
    }

    newAngle   = Rng_RandQ12() - Q12_ANGLE(180.0f);
    newAngle >>= 5;
    newAngle  += Q12_ANGLE(180.0f);

    // Probe points around position.
    curAngle = rotY + newAngle;
    for (i = 0; i < ANGLE_STEP_COUNT; i++, curAngle += angleStep)
    {
        if (Q12_ANGLE_NORM_S(rotY - curAngle) >= Q12_ANGLE(-90.0f) &&
            Q12_ANGLE_NORM_S(rotY - curAngle) < Q12_ANGLE(90.0f))
        {
            continue;
        }

        curPosX = posX + Q12_MULT_PRECISE(Math_Sin(curAngle), Q12(20.0f));
        curPosZ = posZ + Q12_MULT_PRECISE(Math_Cos(curAngle), Q12(20.0f));
        curPosY = Collision_GroundHeightGet(curPosX, curPosZ) - Q12(2.0f);

        if (curPosY < (posY - Q12(2.0f)) || posY < curPosY || curPosY < sharedFunc_800D5274_0_s01() ||
            !func_8008F914(curPosX, curPosZ) || !func_800808AC(curPosX, curPosZ))
        {
            continue;
        }

        airScreamer->position.vx = curPosX;
        airScreamer->position.vy = curPosY;
        airScreamer->position.vz = curPosZ;

        newAngle                    = ((Rng_RandQ12() - Q12_ANGLE(180.0f)) >> 4) + Q12_ANGLE(180.0f);
        airScreamer->rotation.vy = curAngle + newAngle;
        sinRotY                     = Math_Sin(airScreamer->rotation.vy);

        airScreamerProps.targetPosition.vy = curPosY;
        airScreamerProps.targetPosition.vx = curPosX + Q12_MULT_PRECISE(sinRotY, Q12(30.0f));
        airScreamerProps.targetPosition.vz = curPosX + Q12_MULT_PRECISE(Math_Cos(airScreamer->rotation.vy), Q12(30.0f));

        if (sharedFunc_800D4AEC_0_s01(airScreamer, NULL, &airScreamerProps.targetPosition, NULL))
        {
            break;
        }
    }

    if (i == ANGLE_STEP_COUNT)
    {
        airScreamer->position.vx = posX + Q12(50.0f);
        airScreamer->position.vy = sharedFunc_800D5274_0_s01() * 2;
        airScreamer->position.vz = posZ + Q12(50.0f);
        return;
    }

    airScreamer->moveSpeed  = sharedData_800CAA98_0_s01.field_380[9][0];
    airScreamerProps.timer_120 = Q12(10.0f);

    #undef ANGLE_STEP_COUNT
}

void sharedFunc_800DD834_2_s00(s_SubCharacter* airScreamer)
{
    #define ANGLE_STEP_COUNT 16
    #define RADIUS           Q12(30.0f)

    q19_12 posX;
    q19_12 posY;
    q19_12 posZ;
    q19_12 newAngle;
    q19_12 curPosX;
    q19_12 curPosY;
    q19_12 curPosZ;
    q19_12 curAngle;
    q19_12 sinRotY;
    s32    i;

    posX = g_SysWork.playerWork.player.position.vx;
    posY = g_SysWork.playerWork.player.position.vy;
    posZ = g_SysWork.playerWork.player.position.vz;

    curAngle = Rng_RandQ12();
    for (i = 0; i < ANGLE_STEP_COUNT; i++, curAngle += Q12_ANGLE(360.0f / ANGLE_STEP_COUNT))
    {
        curPosX = posX + Q12_MULT_PRECISE(Math_Sin(curAngle), Q12(20.0f));
        curPosZ = posZ + Q12_MULT_PRECISE(Math_Cos(curAngle), Q12(20.0f));
        curPosY = Collision_GroundHeightGet(curPosX, curPosZ) - Q12(2.0f);

        if (curPosY < (posY - Q12(2.0f)) || posY < curPosY || curPosY < sharedFunc_800D5274_0_s01() ||
            !func_8008F914(curPosX, curPosZ) || !func_800808AC(curPosX, curPosZ))
        {
            continue;
        }

        airScreamer->position.vx = curPosX;
        airScreamer->position.vy = curPosY;
        airScreamer->position.vz = curPosZ;

        newAngle                    = ((Rng_RandQ12() - Q12_ANGLE(180.0f)) >> 4) + Q12_ANGLE(180.0f);
        airScreamer->rotation.vy = curAngle + newAngle;

        sinRotY                               = Math_Sin(airScreamer->rotation.vy);
        airScreamerProps.targetPosition.vy = curPosY;
        airScreamerProps.targetPosition.vx = curPosX + Q12_MULT_PRECISE(sinRotY, RADIUS);
        airScreamerProps.targetPosition.vz = curPosX + Q12_MULT_PRECISE(Math_Cos(airScreamer->rotation.vy), RADIUS);

        if (sharedFunc_800D4AEC_0_s01(airScreamer, NULL, &airScreamerProps.targetPosition, NULL))
        {
            break;
        }
    }

    if (i == ANGLE_STEP_COUNT)
    {
        airScreamer->position.vx = posX + Q12(50.0f);
        airScreamer->position.vy = sharedFunc_800D5274_0_s01();
        airScreamer->position.vz = posZ + Q12(50.0f);
        return;
    }

    airScreamer->moveSpeed  = sharedData_800CAA98_0_s01.field_380[9][0];
    airScreamerProps.timer_120 = Q12(10.0f);

    #undef ANGLE_STEP_COUNT
    #undef RADIUS
}

void sharedFunc_800DDA80_2_s00(s_SubCharacter* airScreamer)
{
    q19_12 posX;
    q19_12 posZ;
    q19_12 playerPosZ;
    q19_12 playerPosX;
    q19_12 quadrantAngle;
    s32    var_a0;
    s32    quadrant;
    q19_12 newPosX;
    q19_12 newPosZ;
    s32    var_v1;

    quadrant = Quadrant_South;

    posX = airScreamer->position.vx;
    posZ = airScreamer->position.vz;

    playerPosX = g_SysWork.playerWork.player.position.vx;
    playerPosZ = g_SysWork.playerWork.player.position.vz;

    newPosX = posX;
    newPosZ = posZ;

    if (playerPosX > Q12(16.0f))
    {
        var_v1 = 2;
    }
    else
    {
        var_v1 = !(playerPosX < Q12(-8.0f));
    }

    if (playerPosZ <= Q12(8.0f))
    {
        var_a0 = !(playerPosZ < Q12(-8.0f));
    }
    else
    {
        var_a0 = 2;
    }

    switch (var_v1 + (var_a0 * 3))
    {
        case 8:
            quadrant = !((playerPosX - Q12(16.0f)) > (playerPosZ - Q12(8.0f))) * 2;
            break;

        case 6:
            if ((Q12(-8.0f) - playerPosX) > (playerPosZ - Q12(8.0f)))
            {
                quadrant = Quadrant_South;
            }
            else
            {
                quadrant = Quadrant_East;
            }
            break;

        case 7:
            quadrant = Quadrant_South;
            break;

        case 5:
            quadrant = Quadrant_West;
            break;

        case 4:
            quadrantAngle = Q12_FRACT(g_SysWork.playerWork.player.rotation.vy - Q12_ANGLE(45.0f));
            switch (quadrantAngle / Q12_ANGLE(90.0f))
            {
                case 1:
                    quadrant = Quadrant_East;
                    break;

                case 2:
                    quadrant = Quadrant_South;
                    break;

                case 3:
                    quadrant = Quadrant_West;
                    break;

                default:
                case 0:
                    quadrant = Quadrant_North;
                    break;
            }
            break;

        case 3:
            quadrant = Quadrant_East;
            break;

        case 1:
            quadrant = Quadrant_North;
            break;

        case 2:
            if ((playerPosX - Q12(16.0f)) > (Q12(-8.0f) - playerPosZ))
            {
                quadrant = Quadrant_North;
            }
            else
            {
                quadrant = Quadrant_West;
            }
            break;

        case 0:
            if ((Q12(-8.0f) - playerPosX) > (Q12(-8.0f) - playerPosZ))
            {
                quadrant = Quadrant_North;
            }
            else
            {
                quadrant = Quadrant_East;
            }
            break;
    }

    switch (quadrant)
    {
        case Quadrant_South:
        case Quadrant_North:
            newPosX = (posX + playerPosX) / 2;
            if (newPosX > Q12(16.0f))
            {
                newPosX = Q12(16.0f);
            }
            else if (newPosX < Q12(-8.0f))
            {
                newPosX = Q12(-8.0f);
            }

            newPosZ = Q12(400.0f) - Q12_SQUARE_PRECISE(newPosX - playerPosX);
            if (newPosZ < Q12(0.0f))
            {
                newPosZ = playerPosZ;

                if (newPosX < playerPosX)
                {
                    quadrant = Quadrant_West;
                }
                else
                {
                    quadrant = Quadrant_East;
                }
            }
            else
            {
                newPosZ = SquareRoot12(newPosZ);
                if (quadrant == Quadrant_South)
                {
                    newPosZ = newPosZ + playerPosZ;
                }
                else
                {
                    newPosZ = playerPosZ - newPosZ;
                }
            }
            break;

        case Quadrant_West:
        case Quadrant_East:
            newPosZ = (posZ + playerPosZ) / 2;
            if (newPosZ > Q12(8.0f))
            {
                newPosZ = Q12(8.0f);
            }
            else if (newPosZ < Q12(-8.0f))
            {
                newPosZ = Q12(-8.0f);
            }

            newPosX = Q12(400.0f) - Q12_SQUARE_PRECISE(newPosZ - playerPosZ);

            if (newPosX < 0)
            {
                newPosX  = playerPosX;
                quadrant = (newPosZ < playerPosZ) ^ 1;
            }
            else
            {
                newPosX = SquareRoot12(newPosX);
                if (quadrant == Quadrant_West)
                {
                    newPosX = newPosX + playerPosX;
                }
                else
                {
                    newPosX = playerPosX - newPosX;
                }
            }
            break;
    }

    switch (quadrant)
    {
        case Quadrant_South:
            airScreamer->rotation.vy = Q12_ANGLE(180.0f);
            airScreamer->position.vy = Q12(-2.0f);
            break;

        case Quadrant_North:
            airScreamer->rotation.vy = Q12_ANGLE(0.0f);

        default:
            airScreamer->position.vy = Q12(-2.0f);
            break;

        case Quadrant_West:
            airScreamer->rotation.vy = Q12_ANGLE(270.0f);
            airScreamer->position.vy = Q12(-2.0f);
            break;

        case Quadrant_East:
            airScreamer->rotation.vy = Q12_ANGLE(90.0f);
            airScreamer->position.vy = Q12(-2.0f);
            break;
    }

    airScreamer->moveSpeed   = sharedData_800CAA98_0_s01.field_380[9][0];
    airScreamer->position.vx = newPosX;
    airScreamer->position.vz = newPosZ;
    airScreamerProps.timer_120  = Q12(10.0f);

    sharedFunc_800DEC84_2_s00(airScreamer, Q12(30.0f), airScreamer->rotation.vy + ((Rng_RandQ12() - Q12_ANGLE(180.0f)) >> 7));
    sharedFunc_800D4E84_0_s01(airScreamer);
}

void sharedFunc_800DDE14_2_s00(s_SubCharacter* airScreamer)
{
    q19_12 angle;
    q19_12 radius;
    q19_12 distMin;
    q19_12 distMax;

    if (airScreamerProps.field_E8_4)
    {
        distMax = Q12(5.0f);
        distMin = Q12(3.0f);
    }
    else
    {
        distMax = Q12(4.0f);
        distMin = Q12(2.0f);
    }

    func_800803FC(&sharedData_800F21BC_2_s00, airScreamer->field_40);

    radius = Math_Distance2dGet(&sharedData_800F21BC_2_s00, &airScreamer->position);

    angle  = func_80080478(&sharedData_800F21BC_2_s00, &airScreamer->position);
    angle += Q12_MULT_PRECISE(Rng_RandQ12() - Q12_ANGLE(180.0f), Q12_ANGLE(120.0f));

    radius = (radius - distMin) / 2;

    sharedData_800F21BC_2_s00.vx += Q12_MULT_PRECISE(radius, Math_Sin(angle));
    sharedData_800F21BC_2_s00.vz += Q12_MULT_PRECISE(radius, Math_Cos(angle));

    sharedFunc_800DE034_2_s00(airScreamer, &sharedData_800F21BC_2_s00, distMax);
    sharedFunc_800D4E84_0_s01(airScreamer);
}

void sharedFunc_800DDF74_2_s00(s_SubCharacter* airScreamer, q19_12 dist, q19_12 headingAngle)
{
    q19_12 newPosX;
    q19_12 newPosZ;
    q19_12 newPosY;

    newPosX = airScreamer->position.vx;
    newPosZ = airScreamer->position.vz;

    newPosX += Q12_MULT_PRECISE(dist, Math_Sin(headingAngle));
    newPosZ += Q12_MULT_PRECISE(dist, Math_Cos(headingAngle));
    newPosY  = Collision_GroundHeightGet(newPosX, newPosZ);

    airScreamerProps.targetPosition.vx = newPosX;
    airScreamerProps.targetPosition.vy = newPosY;
    airScreamerProps.targetPosition.vz = newPosZ;
    sharedFunc_800D4E84_0_s01(airScreamer);
}

void sharedFunc_800DE034_2_s00(s_SubCharacter* airScreamer, VECTOR3* vec, q19_12 dist)
{
    q19_12 scaledRadius;
    q19_12 angle;
    q19_12 posX;
    q19_12 posY;
    q19_12 posZ;

    scaledRadius = Q12_MULT_PRECISE(dist, Rng_RandQ12());
    angle        = Rng_RandQ12();
    posX         = vec->vx + Q12_MULT_PRECISE(scaledRadius, Math_Sin(angle));
    posZ         = vec->vz + Q12_MULT_PRECISE(scaledRadius, Math_Cos(angle));

    posY                                  = Collision_GroundHeightGet(posX, posZ);
    airScreamerProps.targetPosition.vx = posX;
    airScreamerProps.targetPosition.vy = posY;
    airScreamerProps.targetPosition.vz = posZ;

    sharedFunc_800D4E84_0_s01(airScreamer);
}

void sharedFunc_800DE11C_2_s00(s_SubCharacter* airScreamer)
{
    q19_12 playerPosX;
    q19_12 playerPosZ;
    q19_12 newPosX;
    q19_12 newPosY;
    q19_12 newPosZ;
    q19_12 headingAngle;
    q19_12 dist;

    playerPosX = g_SysWork.playerWork.player.position.vx;
    playerPosZ = g_SysWork.playerWork.player.position.vz;
    dist       = g_SysWork.playerWork.player.collision.cylinder.radius;

    headingAngle = Math_AngleBetweenPositionsGet(g_SysWork.playerWork.player.position, airScreamer->position);
    newPosX      = playerPosX + Q12_MULT_PRECISE(dist, Math_Sin(headingAngle));
    newPosZ      = playerPosZ + Q12_MULT_PRECISE(dist, Math_Cos(headingAngle));
    newPosY      = Collision_GroundHeightGet(newPosX, newPosZ);

    // TODO: Work out which character this is for. Tracking projected player position?
    airScreamerProps.targetPosition.vx = newPosX;
    airScreamerProps.targetPosition.vy = newPosY;
    airScreamerProps.targetPosition.vz = newPosZ;

    sharedFunc_800D4E84_0_s01(airScreamer);
}

void sharedFunc_800DE1F8_2_s00(s_SubCharacter* airScreamer) // 0x800DE514
{
    q19_12 newPosX;
    q19_12 newPosY;
    q19_12 newPosZ;

    newPosX = airScreamerProps.headTargetPosition.vx;
    newPosZ = airScreamerProps.headTargetPosition.vz;
    newPosY = Collision_GroundHeightGet(newPosX, newPosZ);

    airScreamerProps.targetPosition.vx = newPosX;
    airScreamerProps.targetPosition.vy = newPosY;
    airScreamerProps.targetPosition.vz = newPosZ;
    sharedFunc_800D4E84_0_s01(airScreamer);
}

bool sharedFunc_800DE250_2_s00(s_SubCharacter* airScreamer)
{
    q19_12 posX;
    q19_12 posZ;
    q19_12 groundHeight;
    q19_12 unkPosX;
    q19_12 unkPosZ;
    q19_12 deltaX;
    q19_12 deltaZ;
    q19_12 newPosZ;
    q19_12 newPosX;
    q19_12 targetPosX;
    q19_12 targetPosY;
    q19_12 angleToUnk;
    s32    sp28;
    bool   retCond;
    q19_12 angleStep;
    bool   cond;
    q19_12 unkDist;
    u32    cond1;
    q19_12 targetPosZ;
    q19_12 groundHeight1;
    q19_12 curAngle;
    s32    i;
    q19_12 dist;
    q19_12 distToUnk1;
    q19_12 unkDist1;

    targetPosX = Q12(0.0f);
    targetPosZ = Q12(0.0f);
    targetPosY = Q12(0.0f);
    retCond    = false;
    dist       = NO_VALUE;

    posX         = airScreamer->position.vx;
    posZ         = airScreamer->position.vz;
    groundHeight = airScreamerProps.groundHeight;

    unkPosX = airScreamerProps.headTargetPosition.vx;
    unkPosZ = airScreamerProps.headTargetPosition.vz;

    cond = sharedFunc_800D4A80_0_s01(airScreamer) == 1;

    deltaX     = posX - unkPosX;
    deltaZ     = posZ - unkPosZ;
    angleToUnk = ratan2(deltaX, deltaZ);

    sp28    = SquareRoot12(Q12_SQUARE_PRECISE(deltaX) +
                           Q12_SQUARE_PRECISE(deltaZ));
    unkDist = (Rng_RandQ12() * 4) + Q12(4.0f);

    if (Rng_TestProbability(Q12(0.5f)))
    {
        angleStep = Q12_ANGLE(22.5f);
    }
    else
    {
        angleStep = Q12_ANGLE(-22.5f);
    }

    for (i = 0, curAngle = angleToUnk; i < 16; i++, curAngle += angleStep)
    {
        // TODO: Same as
        // if (Q12_ANGLE_NORM_S(var_s3 - sp24) >= Q12_ANGLE(150.0f) || Q12_ANGLE_NORM_S(var_s3 - sp24) < Q12_ANGLE(-150.0f))
        // but that uses `sltiu` instead of `sltu`.
        cond1 = Q12_ANGLE(300.0f) - 1;
        cond1 = cond1 < (Q12_ANGLE_NORM_S(curAngle - angleToUnk) + Q12_ANGLE(150.0f));
        if (!cond1)
        {
            deltaX  = Q12_MULT_PRECISE(unkDist, Math_Sin(curAngle));
            newPosZ = Q12_MULT_PRECISE(unkDist, Math_Cos(curAngle));

            newPosX                      = posX + deltaX;
            sharedData_800F21CC_2_s00.vx = newPosX;

            newPosZ                     += posZ;
            sharedData_800F21CC_2_s00.vz = newPosZ;

            if (!cond)
            {
                groundHeight1 = Collision_GroundHeightGet(newPosX, newPosZ);
                if (groundHeight < groundHeight1)
                {
                    groundHeight1 = groundHeight;
                }

                groundHeight1 -= Q12(1.5f) + (Rng_RandQ12() / 2);
                if (groundHeight1 < sharedFunc_800D5274_0_s01())
                {
                    groundHeight1 = sharedFunc_800D5274_0_s01();
                }
                sharedData_800F21CC_2_s00.vy = groundHeight1;
            }

            sharedFunc_800D4AEC_0_s01(airScreamer, NULL, &sharedData_800F21CC_2_s00, &sharedData_800F21CC_2_s00);

            distToUnk1 = Math_Distance2dGet(&airScreamerProps.headTargetPosition, &sharedData_800F21CC_2_s00);
            unkDist1   = distToUnk1 + (Math_Distance2dGet(&airScreamer->position, &sharedData_800F21CC_2_s00) * 2);

            if (dist < unkDist1)
            {
                deltaZ  = distToUnk1;
                retCond = deltaZ < sp28;
                dist    = unkDist1 + Q12(0.1f);

                targetPosX = sharedData_800F21CC_2_s00.vx;
                targetPosY = sharedData_800F21CC_2_s00.vy;
                targetPosZ = sharedData_800F21CC_2_s00.vz;
            }
        }
    }

    // Set new target position.
    airScreamerProps.targetPosition.vx = targetPosX;
    airScreamerProps.targetPosition.vz = targetPosZ;
    if (cond)
    {
        airScreamerProps.targetPosition.vy = Collision_GroundHeightGet(targetPosX, targetPosZ);
    }
    else
    {
        airScreamerProps.targetPosition.vy = targetPosY;
    }

    sharedFunc_800D4E84_0_s01(airScreamer);

    return retCond;
}

bool sharedFunc_800DE578_2_s00(s_SubCharacter* chara)
{
    return sharedFunc_800DE250_2_s00(chara);
}

#endif

void sharedFunc_800D529C_0_s01(s_SubCharacter* airScreamer, q19_12 radius, q19_12 headingAngle)
{
    q19_12 newPosX;
    q19_12 newPosY;
    q19_12 newPosZ;

    newPosX = airScreamer->position.vx;
    newPosZ = airScreamer->position.vz;

    newPosX = newPosX + Q12_MULT_PRECISE(radius, Math_Sin(headingAngle));
    newPosZ = newPosZ + Q12_MULT_PRECISE(radius, Math_Cos(headingAngle));

    newPosY  = Collision_GroundHeightGet(newPosX, newPosZ);
    newPosY  = MIN(airScreamerProps.groundHeight, newPosY);
    newPosY -= Q12(1.5f) + Rng_RandQ12() / 2;
    if (newPosY < sharedFunc_800D5274_0_s01())
    {
        newPosY = sharedFunc_800D5274_0_s01();
    }

    airScreamerProps.targetPosition.vx = newPosX;
    airScreamerProps.targetPosition.vy = newPosY;
    airScreamerProps.targetPosition.vz = newPosZ;

    sharedFunc_800D4E84_0_s01(airScreamer);
}

#ifndef MAP0_S01

void sharedFunc_800DE6A8_2_s00(s_SubCharacter* airScreamer, VECTOR3* vec, q19_12 dist)
{
    q19_12 scaledRadius;
    q19_12 headingAngle;
    q19_12 newPosX;
    q19_12 newPosY;
    q19_12 newPosZ;

    scaledRadius = Q12_MULT_PRECISE(dist, Rng_RandQ12());
    headingAngle = Rng_RandQ12();
    newPosX      = vec->vx + Q12_MULT_PRECISE(scaledRadius, Math_Sin(headingAngle));
    newPosZ      = vec->vz + Q12_MULT_PRECISE(scaledRadius, Math_Cos(headingAngle));

    newPosY  = Collision_GroundHeightGet(newPosX, newPosZ);
    newPosY  = MIN(airScreamerProps.groundHeight, newPosY);
    newPosY -= Q12(1.5f) + (Rng_RandQ12() / 2);

    // `posY = MAX(posY, sharedFunc_800D5274_0_s01())`? Doesn't currently match.
    if (newPosY < sharedFunc_800D5274_0_s01())
    {
        newPosY = sharedFunc_800D5274_0_s01();
    }

    airScreamerProps.targetPosition.vx = newPosX;
    airScreamerProps.targetPosition.vy = newPosY;
    airScreamerProps.targetPosition.vz = newPosZ;

    sharedFunc_800D4E84_0_s01(airScreamer);
}

void sharedFunc_800DE7E0_2_s00(s_SubCharacter* airScreamer)
{
    q19_12 angle;
    q19_12 radius;
    q19_12 distMin;
    q19_12 distMax;

    if (airScreamerProps.field_E8_4)
    {
        distMax = Q12(6.0f);
        distMin = Q12(4.0f);
    }
    else
    {
        distMax = Q12(5.0f);
        distMin = Q12(3.0f);
    }

    func_800803FC(&sharedData_800F21DC_2_s00, airScreamer->field_40);

    radius = Math_Distance2dGet(&sharedData_800F21DC_2_s00, &airScreamer->position);

    angle  = func_80080478(&sharedData_800F21DC_2_s00, &airScreamer->position);
    angle += Q12_MULT_PRECISE(Rng_RandQ12() - Q12_ANGLE(180.0f), Q12_ANGLE(120.0f));

    radius = (radius - distMin) / 2;

    sharedData_800F21DC_2_s00.vx += Q12_MULT_PRECISE(radius, Math_Sin(angle));
    sharedData_800F21DC_2_s00.vz += Q12_MULT_PRECISE(radius, Math_Cos(angle));

    sharedFunc_800DE6A8_2_s00(airScreamer, &sharedData_800F21DC_2_s00, distMax);
    sharedFunc_800D4E84_0_s01(airScreamer);
}

#endif

void sharedFunc_800D53AC_0_s01(s_SubCharacter* airScreamer)
{
    #define PLAYER_ANGLE_RANGE       Q12_ANGLE(60.0f)
    #define AIR_SCREAMER_ANGLE_RANGE Q12_ANGLE(90.0f)

    q19_12 targetPosX;
    q19_12 targetPosY;
    q19_12 targetPosZ;
    q19_12 groundHeight;
    q19_12 playerPosX;
    q19_12 playerPosY;
    q19_12 playerPosZ;
    q19_12 playerHeadingAngle;
    q19_12 playerRadius;
    q19_12 charaPosX;
    q19_12 charaPosY;
    q19_12 charaPosZ;
    q19_12 charaHeadingAngle;
    q19_12 charaRadius;
    q19_12 deltaX;
    q19_12 deltaZ;
    q19_12 distToPlayer;
    q19_12 angleToPlayer;
    q19_12 angleDeltaToPlayer;

    // Set player parameters.
    playerPosX         = g_SysWork.playerWork.player.position.vx;
    playerPosY         = g_SysWork.playerWork.player.position.vy;
    playerPosZ         = g_SysWork.playerWork.player.position.vz;
    playerHeadingAngle = g_SysWork.playerWork.player.rotation.vy;
    playerRadius       = g_SysWork.playerWork.player.collision.cylinder.radius;

    // Set Air Screamer parameters.
    charaPosX         = airScreamer->position.vx;
    charaPosY         = airScreamer->position.vy;
    charaPosZ         = airScreamer->position.vz;
    charaHeadingAngle = airScreamer->rotation.vy;
    charaRadius       = airScreamer->collision.cylinder.radius;

    // Compute relations.
    deltaX        = playerPosX - charaPosX;
    deltaZ        = playerPosZ - charaPosZ;
    distToPlayer  = SquareRoot12(Q12_MULT_PRECISE(deltaX, deltaX) + Q12_MULT_PRECISE(deltaZ, deltaZ));
    angleToPlayer = ratan2(deltaX, deltaZ);

    // Adjust angle depending on distance and heading. Some sort of avoidance handling?
    angleDeltaToPlayer = Q12_ANGLE_NORM_S(angleToPlayer - charaHeadingAngle);
    if ((playerRadius + (charaRadius * 2)) < distToPlayer && Math_CheckSignedRange(angleDeltaToPlayer, PLAYER_ANGLE_RANGE))
    {
        angleToPlayer += (angleDeltaToPlayer > Q12_ANGLE(0.0f)) ? Q12_ANGLE(-60.0f) : Q12_ANGLE(60.0f);
    }
    else
    {
        angleToPlayer += Q12_ANGLE(180.0f); // Turn away?
    }

    // Compute target position based on player heading angle.
    if (Math_CheckSignedRange(Q12_ANGLE_NORM_S(angleToPlayer - playerHeadingAngle), AIR_SCREAMER_ANGLE_RANGE))
    {
        targetPosX = playerPosX + Q12_MULT_PRECISE(playerRadius, Math_Sin(angleToPlayer));
        targetPosY = playerPosY + g_SysWork.playerWork.player.collision.box.offsetY;
        targetPosZ = playerPosZ + Q12_MULT_PRECISE(playerRadius, Math_Cos(angleToPlayer));
    }
    else
    {
        targetPosX = playerPosX + Q12_MULT_PRECISE(playerRadius / 4, Math_Sin(angleToPlayer));
        targetPosY = playerPosY + g_SysWork.playerWork.player.collision.box.top;
        targetPosZ = playerPosZ + Q12_MULT_PRECISE(playerRadius / 4, Math_Cos(angleToPlayer));
    }

    // Clamp to ground. TODO: Doesn't match with `MAX` macro.
    groundHeight = Collision_GroundHeightGet(targetPosX, targetPosZ);
    groundHeight = MIN(airScreamerProps.groundHeight, groundHeight);
    if (groundHeight < sharedFunc_800D5274_0_s01())
    {
        groundHeight = sharedFunc_800D5274_0_s01();
    }
    targetPosY = MAX(groundHeight - Q12(2.0f), targetPosY);

    // Set Air Screamer position to target.
    airScreamerProps.targetPosition.vx = targetPosX;
    airScreamerProps.targetPosition.vy = targetPosY;
    airScreamerProps.targetPosition.vz = targetPosZ;

    // Additional processing.
    sharedFunc_800D4E84_0_s01(airScreamer);

    #undef PLAYER_ANGLE_RANGE
    #undef AIR_SCREAMER_ANGLE_RANGE
}

#ifndef MAP0_S01

void sharedFunc_800DEBCC_2_s00(s_SubCharacter* airScreamer)
{
    q19_12 newPosX;
    q19_12 groundHeight;
    q19_12 newPosZ;

    newPosX = airScreamerProps.headTargetPosition.vx;
    newPosZ = airScreamerProps.headTargetPosition.vz;

    groundHeight  = Collision_GroundHeightGet(newPosX, newPosZ);
    groundHeight  = MIN(airScreamerProps.groundHeight, groundHeight);
    groundHeight -= Q12(1.7f);
    if (groundHeight < sharedFunc_800D5274_0_s01())
    {
        groundHeight = sharedFunc_800D5274_0_s01();
    }

    // TODO: Work out which character this is for.
    airScreamerProps.targetPosition.vx = newPosX;
    airScreamerProps.targetPosition.vy = groundHeight;
    airScreamerProps.targetPosition.vz = newPosZ;

    sharedFunc_800D4E84_0_s01(airScreamer);
}

bool sharedFunc_800DEC64_2_s00(s_SubCharacter* airScreamer)
{
    return sharedFunc_800DE250_2_s00(airScreamer);
}

void sharedFunc_800DEC84_2_s00(s_SubCharacter* airScreamer, q19_12 radius, q19_12 angle)
{
    sharedFunc_800D529C_0_s01(airScreamer, radius, angle);
}

void sharedFunc_800DECA4_2_s00(s_SubCharacter* airScreamer, VECTOR3* vec, q19_12 dist)
{
    sharedFunc_800DE6A8_2_s00(airScreamer, vec, dist);
}

void sharedFunc_800DECC4_2_s00(s_SubCharacter* airScreamer)
{
    q19_12 angle;
    q19_12 radius;
    q19_12 distMin;
    q19_12 distMax;

    if (airScreamerProps.field_E8_4)
    {
        distMax = Q12(8.0f);
        distMin = Q12(5.0f);
    }
    else
    {
        distMax = Q12(6.0f);
        distMin = Q12(4.0f);
    }

    func_800803FC(&sharedData_800F21EC_2_s00, airScreamer->field_40);

    radius = Math_Distance2dGet(&sharedData_800F21EC_2_s00, &airScreamer->position);

    angle  = func_80080478(&sharedData_800F21EC_2_s00, &airScreamer->position);
    angle += Q12_MULT_PRECISE(Rng_RandQ12() - Q12_ANGLE(180.0f), Q12_ANGLE(120.0f));

    radius = (radius - distMin) / 2;

    sharedData_800F21EC_2_s00.vx += Q12_MULT_PRECISE(radius, Math_Sin(angle));
    sharedData_800F21EC_2_s00.vz += Q12_MULT_PRECISE(radius, Math_Cos(angle));

    sharedFunc_800DECA4_2_s00(airScreamer, &sharedData_800F21EC_2_s00, distMax);
    sharedFunc_800D4E84_0_s01(airScreamer);
}

s32 sharedFunc_800DEE24_2_s00(s_SubCharacter* airScreamer)
{
    q19_12 playerPosX;
    q19_12 playerPosY;
    q19_12 playerPosZ;
    q19_12 posX;
    q19_12 posZ;
    q19_12 moveSpeed;
    q19_12 rotY;
    q19_12 playerheadingAngle;
    q19_12 playerOffsetZ;
    q19_12 newPlayerPosZ;
    q19_12 newPlayerPosX;
    q19_12 unkAngle;
    q19_12 playerOffsetX;
    q19_12 var_s2;
    q19_12 playerMoveSpeed;
    q19_12 groundHeight;

    playerheadingAngle = g_SysWork.playerWork.player.headingAngle;
    playerMoveSpeed    = g_SysWork.playerWork.player.moveSpeed;

    playerPosX = g_SysWork.playerWork.player.position.vx;
    playerPosY = g_SysWork.playerWork.player.position.vy;
    playerPosZ = g_SysWork.playerWork.player.position.vz;

    posX = airScreamer->position.vx;
    posZ = airScreamer->position.vz;

    rotY      = airScreamer->rotation.vy;
    moveSpeed = airScreamer->moveSpeed;

    playerOffsetX = Q12_MULT_PRECISE(playerMoveSpeed, Math_Sin(playerheadingAngle));
    playerOffsetZ = Q12_MULT_PRECISE(playerMoveSpeed, Math_Cos(playerheadingAngle));

    sharedData_800F21FC_2_s00.vy = Q12(0.0f);
    sharedData_800F21FC_2_s00.vx = Q12_MULT_PRECISE(g_DeltaTime, playerOffsetX);
    sharedData_800F21FC_2_s00.vz = Q12_MULT_PRECISE(g_DeltaTime, playerOffsetZ);

    if (g_DeltaTime != Q12(0.0f) &&
        Collision_WallDetect(&g_AirScreamer_CollisionResult, &sharedData_800F21FC_2_s00, &g_SysWork.playerWork.player))
    {
        playerOffsetX = g_AirScreamer_CollisionResult.offset.vx;
        playerOffsetZ = g_AirScreamer_CollisionResult.offset.vz;

        playerMoveSpeed    = SquareRoot12(Q12_SQUARE_PRECISE(playerOffsetX) +
                                          Q12_SQUARE_PRECISE(playerOffsetZ));
        playerMoveSpeed    = FP_TO(playerMoveSpeed, Q12_SHIFT) / g_DeltaTime;
        playerheadingAngle = ratan2(playerOffsetX, playerOffsetZ);
    }

    unkAngle = Math_Cos(Q12_ANGLE_NORM_S(rotY - playerheadingAngle));

    playerOffsetX = playerPosX - posX;
    playerOffsetZ = playerPosZ - posZ;

    var_s2   = moveSpeed - Q12_MULT_PRECISE(playerMoveSpeed, unkAngle);
    unkAngle = Q12_ANGLE_NORM_S(ratan2(playerOffsetX, playerOffsetZ) - rotY);

    if (unkAngle >= Q12_ANGLE(-60.0f) && unkAngle < Q12_ANGLE(60.0f))
    {
        playerOffsetZ = SquareRoot12(Q12_SQUARE_PRECISE(playerOffsetX) +
                                     Q12_SQUARE_PRECISE(playerOffsetZ));

        if (var_s2 < Q12(1.0f))
        {
            var_s2 = Q12(1.0f);
        }

        var_s2        = FP_TO(playerOffsetZ, Q12_SHIFT) / var_s2;
        playerOffsetZ = Q12_MULT_PRECISE(playerMoveSpeed, var_s2);
        playerOffsetX = Q12_MULT_PRECISE(playerOffsetZ, Math_Sin(playerheadingAngle));
        playerOffsetZ = Q12_MULT_PRECISE(playerOffsetZ, Math_Cos(playerheadingAngle));
    }
    else
    {
        var_s2        = Q12(10.0f);
        playerOffsetX = Q12(0.0f);
        playerOffsetZ = Q12(0.0f);
    }

    newPlayerPosX = playerPosX + playerOffsetX;
    newPlayerPosZ = playerPosZ + playerOffsetZ;
    playerOffsetX = playerPosY + g_SysWork.playerWork.player.collision.box.top;
    playerOffsetZ = Collision_GroundHeightGet(newPlayerPosX, newPlayerPosZ);

    groundHeight = airScreamerProps.groundHeight;
    if (groundHeight < playerOffsetZ)
    {
        playerOffsetZ = groundHeight;
    }

    if (playerOffsetZ < sharedFunc_800D5274_0_s01())
    {
        playerOffsetZ = sharedFunc_800D5274_0_s01();
    }

    groundHeight = playerOffsetZ - Q12(2.0f);
    if (playerOffsetX < groundHeight)
    {
        playerOffsetX = groundHeight;
    }

    airScreamerProps.targetPosition.vx = newPlayerPosX;
    airScreamerProps.targetPosition.vy = playerOffsetX;
    airScreamerProps.targetPosition.vz = newPlayerPosZ;

    sharedFunc_800D4E84_0_s01(airScreamer);

    return var_s2;
}

void sharedFunc_800DF22C_2_s00(s_SubCharacter* airScreamer)
{
    sharedFunc_800DEBCC_2_s00(airScreamer);
}

bool sharedFunc_800DF24C_2_s00(s_SubCharacter* airScreamer)
{
    return sharedFunc_800DE250_2_s00(airScreamer);
}

#endif

void sharedFunc_800D5638_0_s01(s_SubCharacter* airScreamer)
{
    s32                          idx0;
    s_sharedData_800E21D0_0_s01* dst;
    s_func_800D2E04*             src;

    src = &sharedData_800CAA98_0_s01;
    dst = &sharedData_800E21D0_0_s01;

    idx0                   = 3;
    dst->field_B4[idx0][2] = 0;

    idx0                   = 0;
    dst->field_B4[idx0][2] = src->field_380[2][0];
    dst->field_B4[idx0][1] = src->field_380[2][1];

    idx0                   = 1;
    dst->field_B4[idx0][2] = src->field_380[11][0];
    dst->field_B4[idx0][1] = src->field_380[11][1];

    idx0                   = 3;
    dst->field_B4[idx0][1] = src->field_380[30][1];

    sharedFunc_800D5E78_0_s01(airScreamer, Q12_ANGLE(0.0f));
}

#ifndef MAP0_S01

void sharedFunc_800DF2D0_2_s00(s_SubCharacter* airScreamer)
{
    q19_12                       angleDiff;
    s32                          idx0;
    s_sharedData_800E21D0_0_s01* dst;
    s_func_800D2E04*             src;

    angleDiff = Q12_ANGLE_NORM_S(func_80080478(&airScreamer->position, &airScreamerProps.targetPosition) - airScreamer->rotation.vy);

    src = &sharedData_800CAA98_0_s01;
    dst = &sharedData_800E21D0_0_s01;

    idx0                   = 3;
    dst->field_B4[idx0][2] = 0;

    idx0                   = 0;
    dst->field_B4[idx0][2] = src->field_380[2][0];
    dst->field_B4[idx0][1] = src->field_380[2][1];

    idx0                   = 1;
    dst->field_B4[idx0][2] = src->field_380[11][0];
    dst->field_B4[idx0][1] = src->field_380[11][1];

    idx0                   = 3;
    dst->field_B4[idx0][1] = src->field_380[30][1];

    sharedFunc_800D5E78_0_s01(airScreamer, angleDiff);
}

void sharedFunc_800DF358_2_s00(s_SubCharacter* airScreamer)
{
    q19_12           angle0;
    s32              element0;
    s32              element1;
    s32              idx;
    s_func_800D2E04* ptr;

    angle0   = func_80080478(&airScreamer->position, &airScreamerProps.targetPosition);
    angle0   = Q12_ANGLE_NORM_S(angle0 - airScreamer->rotation.vy);
    element0 = sharedData_800CAA98_0_s01.field_380[30][0] / 2;
    element1 = sharedData_800CAA98_0_s01.field_380[30][1];

    if (angle0 <= Q12_ANGLE(0.5f))
    {
        element1 = -element1;
        if (angle0 < Q12_ANGLE(-0.5f))
        {
            element0 = -element0;
        }
        else
        {
            element1 = 0;
            element0 = 0;
        }
    }

    sharedData_800E21D0_0_s01.field_B4[0][3] = 0;
    sharedData_800E21D0_0_s01.field_B4[0][0] = 1;

    ptr = &sharedData_800CAA98_0_s01;

    idx                                        = 1;
    sharedData_800E21D0_0_s01.field_B4[0][2]   = ptr->field_380[2][0];
    sharedData_800E21D0_0_s01.field_B4[0][1]   = ptr->field_380[2][1];
    sharedData_800E21D0_0_s01.field_B4[idx][2] = sharedData_800CAA98_0_s01.field_380[11][0];
    sharedData_800E21D0_0_s01.field_B4[idx][1] = sharedData_800CAA98_0_s01.field_380[11][1];
    sharedData_800E21D0_0_s01.field_B4[idx][3] = 0;
    sharedData_800E21D0_0_s01.field_B4[idx][0] = 1;

    idx                                        = 3;
    sharedData_800E21D0_0_s01.field_B4[idx][2] = element0;
    sharedData_800E21D0_0_s01.field_B4[idx][1] = element1;
    sharedData_800E21D0_0_s01.field_B4[idx][3] = angle0;
    sharedData_800E21D0_0_s01.field_B4[idx][0] = 1;

    sharedFunc_800D5E78_0_s01(airScreamer, angle0 / 2);
}

void sharedFunc_800DF448_2_s00(s_SubCharacter* airScreamer, bool cond)
{
    s32              temp_a1;
    s32              temp_s2;
    s32              temp_t3;
    s32              idx1;
    s32              var_a2;
    s32              var_t1;
    s32              var_t2;
    u32              var_v1;
    s32              var_v1_2;
    s32              temp;
    s32              temp2;
    s32              temp3;
    s32              temp4;
    s32              idx;
    s_func_800D2E04* ptr;

    temp_s2 = func_80080478(&airScreamer->position, &airScreamerProps.targetPosition2);
    temp_t3 = Math_Distance2dGet(&airScreamer->position, &airScreamerProps.targetPosition2);
    temp_s2 = Q12_ANGLE_NORM_S(temp_s2 - airScreamer->rotation.vy);

    temp_a1 = airScreamer->model.anim.status | 1;

    if (temp_a1 == 0x2D)
    {
        var_a2 = 0xC;
    }
    else if (temp_a1 == 0x31)
    {
        var_a2 = 0xD;
    }
    else
    {
        var_a2 = 0xB;
    }

    if (temp_a1 == 0x31)
    {
        idx1   = 3;
        var_v1_2 = 0x1F;
    }
    else if (airScreamer->collision.cylinder.radius + 0xCC >= temp_t3)
    {
        idx1   = 2;
        var_v1_2 = 0x1E;
    }
    else
    {
        if (cond)
        {
            var_v1 = 0x155;
        }
        else
        {
            var_v1 = 0x2AA;
        }

        if (temp_s2 + var_v1 < var_v1 * 2)
        {
            if (cond)
            {
                idx1   = 3;
                var_v1_2 = 0x1F;
            }
            else
            {
                idx1   = 0;
                var_v1_2 = 0x1C;
            }
        }
        else
        {
            idx1   = 2;
            var_v1_2 = 0x1E;
        }
    }

    var_t1 = sharedData_800CAA98_0_s01.field_380[var_v1_2][0];
    var_t2 = sharedData_800CAA98_0_s01.field_380[var_v1_2][1];

    if (temp_s2 < 6)
    {
        if (temp_s2 < -5)
        {
            var_t2 = -var_t2;
            var_t1 = -var_t1;
        }
        else
        {
            var_t2 = 0;
            var_t1 = 0;
        }
    }

    temp  = sharedData_800CAA98_0_s01.field_380[idx1][0];
    temp2 = sharedData_800CAA98_0_s01.field_380[idx1][1];

    sharedData_800E21D0_0_s01.field_B4[0][1] = temp2;
    sharedData_800E21D0_0_s01.field_B4[0][2] = temp;

    if (temp_a1 != 49)
    {
        sharedData_800E21D0_0_s01.field_B4[0][3] = temp_t3;
        sharedData_800E21D0_0_s01.field_B4[0][0] = 1;
    }

    ptr = &sharedData_800CAA98_0_s01;

    temp3 = ptr->field_380[var_a2][0];
    temp4 = ptr->field_380[var_a2][1];

    idx                                        = 3;
    sharedData_800E21D0_0_s01.field_B4[idx][2] = var_t1;
    sharedData_800E21D0_0_s01.field_B4[idx][1] = var_t2;
    sharedData_800E21D0_0_s01.field_B4[idx][3] = temp_s2;
    sharedData_800E21D0_0_s01.field_B4[idx][0] = 1;

    idx                                        = 1;
    sharedData_800E21D0_0_s01.field_B4[idx][2] = temp3;
    sharedData_800E21D0_0_s01.field_B4[idx][1] = temp4;

    sharedFunc_800D5E78_0_s01(airScreamer, temp_s2 / 2);
}

void sharedFunc_800DF60C_2_s00(s_SubCharacter* airScreamer)
{
    s32              temp_s0;
    s32              temp_t1;
    s32              var_t2;
    s32              var_t3;
    s32              idx1;
    s32              temp3;
    s32              temp4;
    s32              idx0;
    s_func_800D2E04* ptr;

    temp_s0 = airScreamer->model.anim.status | 1;
    temp_t1 = func_80080478(&airScreamer->position, &airScreamerProps.targetPosition);
    temp_t1 = Q12_ANGLE_NORM_S(temp_t1 - airScreamer->rotation.vy);

    if (temp_s0 == 45)
    {
        idx1 = 12;
    }
    else
    {
        idx1 = 11;
    }

    var_t2 = sharedData_800CAA98_0_s01.field_380[30][0];
    var_t3 = sharedData_800CAA98_0_s01.field_380[30][1];

    if (temp_t1 < 6)
    {
        if (temp_t1 < -5)
        {
            var_t3 = -var_t3;
            var_t2 = -var_t2;
        }
        else
        {
            var_t3 = 0;
            var_t2 = 0;
        }
    }

    ptr = &sharedData_800CAA98_0_s01;

    sharedData_800E21D0_0_s01.field_B4[0][3] = 0;
    sharedData_800E21D0_0_s01.field_B4[0][0] = 1;
    sharedData_800E21D0_0_s01.field_B4[0][2] = ptr->field_380[2][0];
    sharedData_800E21D0_0_s01.field_B4[0][1] = ptr->field_380[2][1];

    temp3 = ptr->field_380[idx1][0];
    temp4 = ptr->field_380[idx1][1];

    idx0                                        = 3;
    sharedData_800E21D0_0_s01.field_B4[idx0][2] = var_t2;
    sharedData_800E21D0_0_s01.field_B4[idx0][1] = var_t3;
    sharedData_800E21D0_0_s01.field_B4[idx0][3] = temp_t1;
    sharedData_800E21D0_0_s01.field_B4[idx0][0] = 1;

    idx0                                        = 1;
    sharedData_800E21D0_0_s01.field_B4[idx0][2] = temp3;
    sharedData_800E21D0_0_s01.field_B4[idx0][1] = temp4;

    sharedFunc_800D5E78_0_s01(airScreamer, temp_t1 / 2);
}

void sharedFunc_800DF710_2_s00(s_SubCharacter* airScreamer)
{
    q19_12           angle0;
    q19_12           angle1;
    s32              idx;
    s32              animStatus;
    s32              animIdx;
    s32              element0;
    s32              element1;
    s32              element4;
    s32              element3;
    s32              element2;
    s_func_800D2E04* ptr;

    animStatus = airScreamer->model.anim.status | 1; // TODO: Use macro for this.
    angle0     = func_80080478(&airScreamer->position, &airScreamerProps.targetPosition);
    angle0     = Q12_ANGLE_NORM_S(angle0 - airScreamer->rotation.vy);

    if (animStatus == ANIM_STATUS(AirScreamerAnim_StandIdleToHover, true))
    {
        animIdx = 12;
    }
    else
    {
        animIdx = 11;
    }

    element0 = sharedData_800CAA98_0_s01.field_380[29][0];
    element1 = sharedData_800CAA98_0_s01.field_380[29][1];

    if (angle0 <= Q12_ANGLE(0.5f))
    {
        element1 = -element1;
        if (angle0 < Q12_ANGLE(-0.5f))
        {
            element0 = -element0;
        }
        else
        {
            element1 = 0;
            element0 = 0;
        }
    }

    element2 = sharedData_800CAA98_0_s01.field_380[1][0];

    idx                                        = 0;
    sharedData_800E21D0_0_s01.field_B4[idx][2] = element2;
    sharedData_800E21D0_0_s01.field_B4[idx][1] = sharedData_800CAA98_0_s01.field_380[1][1];

    ptr = &sharedData_800CAA98_0_s01;

    idx                                        = 1;
    element3                                   = ptr->field_380[animIdx][0];
    element4                                   = ptr->field_380[animIdx][1];
    sharedData_800E21D0_0_s01.field_B4[idx][2] = element3;
    sharedData_800E21D0_0_s01.field_B4[idx][1] = element4;

    idx                                        = 3;
    sharedData_800E21D0_0_s01.field_B4[idx][2] = element0;
    sharedData_800E21D0_0_s01.field_B4[idx][1] = element1;
    sharedData_800E21D0_0_s01.field_B4[idx][3] = angle0;
    sharedData_800E21D0_0_s01.field_B4[idx][0] = 1;

    sharedFunc_800D5E78_0_s01(airScreamer, angle0 / 2);
}

void sharedFunc_800DF80C_2_s00(s_SubCharacter* airScreamer)
{
    q19_12           angle0;
    q19_12           angle1;
    s32              idx;
    s_func_800D2E04* src;

    angle0 = func_80080478(&airScreamer->position, &airScreamerProps.targetPosition);
    angle1 = Q12_ANGLE_NORM_S(angle0 - airScreamer->rotation.vy);

    src = &sharedData_800CAA98_0_s01;

    idx                                        = 3;
    sharedData_800E21D0_0_s01.field_B4[idx][2] = 0;

    idx                                      = 0;
    sharedData_800E21D0_0_s01.field_B4[0][2] = src->field_380[2][0];
    sharedData_800E21D0_0_s01.field_B4[0][1] = src->field_380[2][1];

    idx                                        = 1;
    sharedData_800E21D0_0_s01.field_B4[idx][2] = src->field_380[11][0];
    sharedData_800E21D0_0_s01.field_B4[idx][1] = src->field_380[11][1];

    idx                                        = 3;
    sharedData_800E21D0_0_s01.field_B4[idx][1] = src->field_380[30][1];

    if (angle1 < Q12_ANGLE(0.0f))
    {
        angle1 += Q12_ANGLE(0.3f);
    }

    sharedFunc_800D5E78_0_s01(airScreamer, angle1 >> 2);
}

void sharedFunc_800DF8A0_2_s00(s_SubCharacter* airScreamer)
{
    s32              var_a3;
    s32              idx0;
    s32              idx1;
    s32              temp3;
    s32              temp4;
    s_func_800D2E04* ptr;

    if (airScreamer->model.anim.status == ANIM_STATUS(AirScreamerAnim_StandToStun, true) &&
        airScreamer->model.anim.time > Q12(64.0f))
    {
        idx0   = 4;
        var_a3 = 32;
    }
    else
    {
        idx0   = 2;
        var_a3 = 30;
    }

    temp3 = sharedData_800CAA98_0_s01.field_380[idx0][0];
    temp4 = sharedData_800CAA98_0_s01.field_380[idx0][1];

    sharedData_800E21D0_0_s01.field_B4[0][2] = temp3;
    sharedData_800E21D0_0_s01.field_B4[0][1] = temp4;

    ptr = &sharedData_800CAA98_0_s01;

    idx1                                        = 1;
    sharedData_800E21D0_0_s01.field_B4[idx1][2] = ptr->field_380[11][0];
    sharedData_800E21D0_0_s01.field_B4[idx1][1] = ptr->field_380[11][1];

    idx1                                        = 3;
    sharedData_800E21D0_0_s01.field_B4[idx1][1] = ptr->field_380[var_a3][1];
    sharedData_800E21D0_0_s01.field_B4[idx1][2] = 0;

    sharedFunc_800D5E78_0_s01(airScreamer, 0);
}

#endif

s32 sharedFunc_800D569C_0_s01(s_SubCharacter* airScreamer, q19_12 vecY, q19_12 dist)
{
    q19_12 posY;
    s32    prevGroundHeight;
    q19_12 groundHeight;
    q19_12 vecYCpy;
    q19_12 distCpy;
    q19_12 vec_x;
    q19_12 vec_z;

    vecYCpy      = vecY;
    posY         = airScreamer->position.vy;
    distCpy      = dist;
    groundHeight = Collision_GroundHeightGet(airScreamer->position.vx, airScreamer->position.vz);

    // TODO: Not used as ground height in this func?
    prevGroundHeight = airScreamerProps.groundHeight;

    vec_x = airScreamerProps.targetPosition2.vx;
    vec_z = airScreamerProps.targetPosition2.vz;

    if (prevGroundHeight < groundHeight)
    {
        groundHeight = prevGroundHeight;
    }

    groundHeight -= Q12(1.7f);
    prevGroundHeight         = func_800808AC(vec_x, vec_z); // Collision type? This returns `caseVar` from `Collision_Fill`.
    if (prevGroundHeight == 12 || prevGroundHeight == 0 || prevGroundHeight == 7)
    {
        distCpy = Q12(0.0f);
    }

    if (posY < groundHeight)
    {
        groundHeight = posY;
    }

    if (distCpy > Q12(12.0f))
    {
        vecYCpy = groundHeight;
    }
    else if (distCpy > Q12(4.0f))
    {
        prevGroundHeight    = Q12_DIV(distCpy - Q12(4.0f), Q12(8.0f));
        vecYCpy += Q12_MULT_PRECISE((groundHeight - vecYCpy), prevGroundHeight);
    }

    if (vecYCpy < sharedFunc_800D5274_0_s01())
    {
        vecYCpy = sharedFunc_800D5274_0_s01();
    }

    return vecYCpy;
}

void sharedFunc_800D57C8_0_s01(s_SubCharacter* airScreamer)
{
    q19_12           angle;
    q19_12           dist;
    s32              posY;
    s32              idx0;
    s32              idx1;
    s32              idx2;
    s32              idx3;
    s32              animStatus0;
    bool             var_t4;
    s32              animStatus;
    s32              element0;
    s32              element1;
    s32              element3;
    s32              element2;
    s32              element4;
    s32              element5;
    s32              tmp;
    VECTOR3*         pos0;
    VECTOR3*         pos;
    s_func_800D2E04* ptr;

    pos   = &airScreamer->position;
    pos0  = &airScreamerProps.targetPosition2;
    dist  = Math_Distance2dGet(pos, pos0);
    angle = Q12_ANGLE_NORM_S(func_80080478(pos, pos0) - airScreamer->rotation.vy);

    tmp    = sharedFunc_800D569C_0_s01(airScreamer, pos0->vy, dist);
    var_t4 = 0;
    posY   = tmp - airScreamer->position.vy;

    if (dist > (airScreamer->collision.cylinder.radius + Q12(0.05f)) && (angle + Q12_ANGLE(45.0f)) < (u32)Q12_ANGLE(90.0f))
    {
        idx0 = 5;
        idx1 = 33;
    }
    else
    {
        idx0 = 7;
        idx1 = 35;
    }

    animStatus = airScreamer->model.anim.status;

    animStatus0 = ANIM_STATUS(AirScreamerAnim_StandIdleToHover, true);
    if (animStatus == animStatus0)
    {
        idx2 = 17;
    }
    else if (animStatus < animStatus0 ||
             animStatus >= ANIM_STATUS(AirScreamerAnim_28, false) ||
             animStatus <  ANIM_STATUS(AirScreamerAnim_HoverConstant, false))
    {
        var_t4 = 1;
        idx2   = 14;

        if (posY <= Q12(0.05f))
        {
            idx2 = 15;
            if (posY < Q12(-0.05f))
            {
                idx2 = 16;
            }
        }
    }
    else
    {
        idx2 = 18;
    }

    element0 = sharedData_800CAA98_0_s01.field_380[idx1][0];
    element1 = sharedData_800CAA98_0_s01.field_380[idx1][1];
    if (angle <= Q12_ANGLE(0.5f))
    {
        element1 = -element1;
        if (angle < Q12_ANGLE(-0.5f))
        {
            element0 = -element0;
        }
        else
        {
            element1 = 0;

            element0 = 0;
        }
    }

    ptr = &sharedData_800CAA98_0_s01;

    element2 = ptr->field_380[idx0][0];
    element3 = ptr->field_380[idx0][1];

    idx3                                        = 0;
    sharedData_800E21D0_0_s01.field_B4[idx3][2] = element2;
    sharedData_800E21D0_0_s01.field_B4[idx3][1] = element3;
    sharedData_800E21D0_0_s01.field_B4[idx3][3] = dist;
    sharedData_800E21D0_0_s01.field_B4[idx3][0] = 1;

    element4 = ptr->field_380[idx2][0];
    element5 = ptr->field_380[idx2][1];

    idx3                                        = 1;
    sharedData_800E21D0_0_s01.field_B4[idx3][2] = element4;
    sharedData_800E21D0_0_s01.field_B4[idx3][1] = element5;
    if (var_t4)
    {
        sharedData_800E21D0_0_s01.field_B4[idx3][3] = posY;
        sharedData_800E21D0_0_s01.field_B4[idx3][0] = 1;
    }

    idx3                                        = 3;
    sharedData_800E21D0_0_s01.field_B4[idx3][2] = element0;
    sharedData_800E21D0_0_s01.field_B4[idx3][1] = element1;
    sharedData_800E21D0_0_s01.field_B4[idx3][3] = angle;
    sharedData_800E21D0_0_s01.field_B4[idx3][0] = 1;

    idx0 = angle / 2;
    sharedFunc_800D5E78_0_s01(airScreamer, idx0);
}

void sharedFunc_800D598C_0_s01(s_SubCharacter* airScreamer)
{
    q19_12           angle;
    q19_12           dist;
    s32              posY;
    s32              idx0;
    s32              idx1;
    s32              idx2;
    s32              idx3;
    s32              animStatus0;
    bool             var_t4;
    s32              animStatus;
    s32              element0;
    s32              element1;
    s32              element3;
    s32              element2;
    s32              element4;
    s32              element5;
    s32              tmp;
    VECTOR3*         pos0;
    VECTOR3*         pos;
    s_func_800D2E04* ptr;

    pos    = &airScreamer->position;
    pos0   = &airScreamerProps.targetPosition; // `sharedFunc_800D57C8_0_s01` uses `field_110`.
    dist   = Math_Distance2dGet(pos, pos0);
    angle  = Q12_ANGLE_NORM_S(func_80080478(pos, pos0) - airScreamer->rotation.vy);
    tmp    = sharedFunc_800D569C_0_s01(airScreamer, airScreamerProps.targetPosition.vy, dist); // `sharedFunc_800D57C8_0_s01` uses `pos0->vy`. Here it's accessed from chara (full offset).
    var_t4 = 0;
    posY   = tmp - airScreamer->position.vy;

#if 0
    if (dist > (chara->collision.cylinder.radius + Q12(0.05f)) && (angle + Q12_ANGLE(45.0f)) < (u32)Q12_ANGLE(90.0f))
    {
        idx0 = 5;
        idx1 = 33;
    }
    else
    {
        idx0 = 7;
        idx1 = 35;
    }
#endif
    animStatus = airScreamer->model.anim.status;

    animStatus0 = ANIM_STATUS(AirScreamerAnim_StandIdleToHover, true);
    if (animStatus == animStatus0)
    {
        idx2 = 17;
    }
    else if (animStatus < animStatus0 ||
             animStatus >= ANIM_STATUS(AirScreamerAnim_28, false) ||
             animStatus <  ANIM_STATUS(AirScreamerAnim_HoverConstant, false))
    {
        var_t4 = 1;
        idx2   = 14;

        if (posY <= Q12(0.05f))
        {
            idx2 = 15;
            if (posY < Q12(-0.05f))
            {
                idx2 = 16;
            }
        }
    }
    else
    {
        idx2 = 18;
    }

    element0 = sharedData_800CAA98_0_s01.field_380[35][0]; // Hardcoded 35 instead of `idx1`.
    element1 = sharedData_800CAA98_0_s01.field_380[35][1];

    if (angle <= Q12_ANGLE(0.5f))
    {
        element1 = -element1;
        if (angle < Q12_ANGLE(-0.5f))
        {
            element0 = -element0;
        }
        else
        {
            element1 = 0;

            element0 = 0;
        }
    }

    ptr = &sharedData_800CAA98_0_s01;

    element2 = ptr->field_380[7][0]; // Hardcoded 7 instead of `idx0`.
    element3 = ptr->field_380[7][1];

    idx3                                        = 0;
    sharedData_800E21D0_0_s01.field_B4[idx3][2] = element2;
    sharedData_800E21D0_0_s01.field_B4[idx3][1] = element3;
    sharedData_800E21D0_0_s01.field_B4[idx3][3] = 0;
    sharedData_800E21D0_0_s01.field_B4[idx3][0] = 1;

    element4 = ptr->field_380[idx2][0];
    element5 = ptr->field_380[idx2][1];

    idx3                                        = 1;
    sharedData_800E21D0_0_s01.field_B4[idx3][2] = element4;
    sharedData_800E21D0_0_s01.field_B4[idx3][1] = element5;
    if (var_t4)
    {
        sharedData_800E21D0_0_s01.field_B4[idx3][3] = posY;
        sharedData_800E21D0_0_s01.field_B4[idx3][0] = 1;
    }

    idx3                                        = 3;
    sharedData_800E21D0_0_s01.field_B4[idx3][2] = element0;
    sharedData_800E21D0_0_s01.field_B4[idx3][1] = element1;
    sharedData_800E21D0_0_s01.field_B4[idx3][3] = angle;
    sharedData_800E21D0_0_s01.field_B4[idx3][0] = 1;

    idx0 = angle / 2;
    sharedFunc_800D5E78_0_s01(airScreamer, idx0);
}

void sharedFunc_800D5B10_0_s01(s_SubCharacter* airScreamer)
{
    q19_12           angle;
    q19_12           dist;
    q19_12           posY;
    s32              idx0;
    s32              idx1;
    s32              idx2;
    s32              idx3;
    s32              animStatus0;
    bool             cond;
    s32              animStatus;
    s32              element0;
    s32              element1;
    s32              element3;
    s32              element2;
    s32              element4;
    s32              element5;
    q19_12           tmp;
    VECTOR3*         pos0;
    VECTOR3*         pos;
    s_func_800D2E04* ptr;

    pos   = &airScreamer->position;
    pos0  = &airScreamerProps.targetPosition;
    dist  = Math_Distance2dGet(pos, pos0);
    angle = Q12_ANGLE_NORM_S(func_80080478(pos, pos0) - airScreamer->rotation.vy);

    tmp        = sharedFunc_800D569C_0_s01(airScreamer, airScreamerProps.targetPosition.vy, dist);
    cond       = false;
    posY       = tmp - airScreamer->position.vy;
    animStatus = airScreamer->model.anim.status;

    animStatus0 = ANIM_STATUS(AirScreamerAnim_StandIdleToHover, true);
    if (animStatus == animStatus0)
    {
        idx2 = 17;
    }
    else if (animStatus < animStatus0 || animStatus >= ANIM_STATUS(AirScreamerAnim_28, false) || animStatus < ANIM_STATUS(AirScreamerAnim_HoverConstant, false))
    {
        cond = true;
        idx2 = 14;

        if (posY <= Q12(0.05f))
        {
            idx2 = 15;
            if (posY < Q12(-0.05f))
            {
                idx2 = 16;
            }
        }
    }
    else
    {
        idx2 = 18;
    }

    element0 = sharedData_800CAA98_0_s01.field_380[34][0];
    element1 = sharedData_800CAA98_0_s01.field_380[34][1];

    if (angle <= Q12_ANGLE(0.5f))
    {
        element1 = -element1;
        if (angle < Q12_ANGLE(-0.5f))
        {
            element0 = -element0;
        }
        else
        {
            element1 = 0;

            element0 = 0;
        }
    }

    ptr = &sharedData_800CAA98_0_s01;

    element2 = ptr->field_380[6][0];
    element3 = ptr->field_380[6][1];

    idx3                                        = 0;
    sharedData_800E21D0_0_s01.field_B4[idx3][2] = element2;
    sharedData_800E21D0_0_s01.field_B4[idx3][1] = element3;

    element4 = ptr->field_380[idx2][0];
    element5 = ptr->field_380[idx2][1];

    idx3                                        = 1;
    sharedData_800E21D0_0_s01.field_B4[idx3][2] = element4;
    sharedData_800E21D0_0_s01.field_B4[idx3][1] = element5;
    if (cond)
    {
        sharedData_800E21D0_0_s01.field_B4[idx3][3] = posY;
        sharedData_800E21D0_0_s01.field_B4[idx3][0] = 1;
    }

    idx3                                        = 3;
    sharedData_800E21D0_0_s01.field_B4[idx3][2] = element0;
    sharedData_800E21D0_0_s01.field_B4[idx3][1] = element1;
    sharedData_800E21D0_0_s01.field_B4[idx3][3] = angle;
    sharedData_800E21D0_0_s01.field_B4[idx3][0] =
        tmp = 1;

    sharedFunc_800D5E78_0_s01(airScreamer, angle / 2);
}

void sharedFunc_800D5C90_0_s01(s_SubCharacter* airScreamer)
{
    s32              angle0;
    s32              angle1;
    s32              element0;
    s32              element1;
    s32              idx;
    s_func_800D2E04* ptr;

    angle0 = func_80080478(&airScreamer->position, &airScreamerProps.targetPosition);
    angle1 = Q12_ANGLE_NORM_S(angle0 - airScreamer->rotation.vy);

    element0 = sharedData_800CAA98_0_s01.field_380[35][0] / 2;
    element1 = sharedData_800CAA98_0_s01.field_380[35][1];

    if (angle1 <= Q12_ANGLE(0.5f))
    {
        element1 = -element1;
        if (angle1 < Q12_ANGLE(-0.5f))
        {
            element0 = -element0;
        }
        else
        {
            element1 = 0;
            element0 = 0;
        }
    }

    ptr = &sharedData_800CAA98_0_s01;

    idx                                        = 0;
    sharedData_800E21D0_0_s01.field_B4[idx][3] = 0;
    sharedData_800E21D0_0_s01.field_B4[idx][0] = 1;
    sharedData_800E21D0_0_s01.field_B4[idx][2] = ptr->field_380[7][0];
    sharedData_800E21D0_0_s01.field_B4[idx][1] = ptr->field_380[7][1];

    idx                                        = 1;
    sharedData_800E21D0_0_s01.field_B4[idx][3] = 0;
    sharedData_800E21D0_0_s01.field_B4[idx][0] = 1;
    sharedData_800E21D0_0_s01.field_B4[idx][2] = ptr->field_380[15][0];
    sharedData_800E21D0_0_s01.field_B4[idx][1] = ptr->field_380[15][1];

    idx                                        = 3;
    sharedData_800E21D0_0_s01.field_B4[idx][2] = element0;
    sharedData_800E21D0_0_s01.field_B4[idx][1] = element1;
    sharedData_800E21D0_0_s01.field_B4[idx][3] = angle1;
    sharedData_800E21D0_0_s01.field_B4[idx][0] = 1;

    sharedFunc_800D5E78_0_s01(airScreamer, angle1 / 2);
}

void sharedFunc_800D5D80_0_s01(s_SubCharacter* airScreamer)
{
    q19_12                       angle0;
    q19_12                       angle1;
    s32                          idx;
    s_sharedData_800E21D0_0_s01* base;
    s_func_800D2E04*             src;

    angle0 = func_80080478(&airScreamer->position, &airScreamerProps.targetPosition);
    angle1 = Q12_ANGLE_NORM_S(angle0 - airScreamer->rotation.vy);
    src    = &sharedData_800CAA98_0_s01;
    base   = &sharedData_800E21D0_0_s01;

    idx                    = 0;
    base->field_B4[idx][2] = src->field_380[7][0];
    base->field_B4[idx][1] = src->field_380[7][1];

    idx                    = 1;
    base->field_B4[idx][2] = src->field_380[20][0];
    base->field_B4[idx][1] = src->field_380[20][1];

    idx                    = 3;
    base->field_B4[idx][2] = 0;
    base->field_B4[idx][1] = src->field_380[35][1];

    if (angle1 < Q12_ANGLE(0.0f))
    {
        angle1 += Q12_ANGLE(0.3f);
    }

    sharedFunc_800D5E78_0_s01(airScreamer, angle1 >> 2);
}

void sharedFunc_800D5E14_0_s01(s_SubCharacter* airScreamer)
{
    s32                          idx;
    s_sharedData_800E21D0_0_s01* base;
    s_func_800D2E04*             src;

    src  = &sharedData_800CAA98_0_s01;
    base = &sharedData_800E21D0_0_s01;

    idx                    = 0;
    base->field_B4[idx][2] = src->field_380[7][0];
    base->field_B4[idx][1] = src->field_380[7][1];

    idx                    = 1;
    base->field_B4[idx][2] = src->field_380[19][0];
    base->field_B4[idx][1] = src->field_380[19][1];

    idx                    = 3;
    base->field_B4[idx][2] = 0;
    base->field_B4[idx][1] = src->field_380[35][1];

    sharedFunc_800D5E78_0_s01(airScreamer, 0);
}

#ifndef MAP0_S01

void sharedFunc_800E012C_2_s00(s_SubCharacter* airScreamer)
{
    q19_12 angleDeltaToPlayer;
    s32    var_t0;
    s32    var_t1;
    s32    temp_a0;
    s32    idx;
    s32*   ptr;

    angleDeltaToPlayer = func_80080478(&airScreamer->position, &airScreamerProps.targetPosition);
    angleDeltaToPlayer = Q12_ANGLE_NORM_S(angleDeltaToPlayer - airScreamer->rotation.vy);

    idx     = 33;
    temp_a0 = sharedData_800CAA98_0_s01.field_380[idx][0];
    var_t1  = sharedData_800CAA98_0_s01.field_380[idx][1];
    var_t0  = temp_a0 / 2;

    if (angleDeltaToPlayer <= Q12_ANGLE(0.5f))
    {
        if (angleDeltaToPlayer < Q12_ANGLE(-0.5f))
        {
            var_t1 = -var_t1;
            var_t0 = -var_t0;
        }
        else
        {
            var_t1 = 0;
            var_t0 = 0;
        }
    }

    sharedData_800E21D0_0_s01.field_B4[0][2] = sharedData_800CAA98_0_s01.field_380[5][0] / 2;
    sharedData_800E21D0_0_s01.field_B4[0][1] = sharedData_800CAA98_0_s01.field_380[5][1];

    idx                                        = 3;
    sharedData_800E21D0_0_s01.field_B4[idx][2] = var_t0;
    sharedData_800E21D0_0_s01.field_B4[idx][1] = var_t1;
    sharedData_800E21D0_0_s01.field_B4[idx][3] = angleDeltaToPlayer;
    sharedData_800E21D0_0_s01.field_B4[idx][0] = 1;

    ptr = sharedData_800CAA98_0_s01.field_380[19];

    idx                                        = 1;
    sharedData_800E21D0_0_s01.field_B4[idx][2] = ptr[0];
    sharedData_800E21D0_0_s01.field_B4[idx][1] = ptr[1];

    sharedFunc_800D5E78_0_s01(airScreamer, angleDeltaToPlayer / 2);
}

void sharedFunc_800E021C_2_s00(s_SubCharacter* airScreamer, s32 arg1, s32 arg2)
{
    VECTOR3*         pos;
    s32              temp_a1;
    s32              angleToUnk;
    s32              temp_t2;
    s32              temp_v1;
    s32              var_a2;
    s32              var_a3;
    s32              var_t0;
    s32              var_t3;
    s32              var_v1;
    s32              var_a0;
    s32              idx;
    s32              temp;
    s32              temp2;
    s32              temp3;
    s32              temp4;
    s_func_800D2E04* ptr;

    pos = &airScreamerProps.targetPosition2;

    angleToUnk = func_80080478(&airScreamer->position, pos);
    temp       = Math_Distance2dGet(&airScreamer->position, pos);
    temp_t2    = sharedFunc_800D569C_0_s01(airScreamer, pos->vy, temp);
    angleToUnk = Q12_ANGLE_NORM_S(angleToUnk - airScreamer->rotation.vy);

    temp_t2 -= airScreamer->position.vy;
    temp_a1  = airScreamer->model.anim.status | 0x1; // TODO: Macro.

    var_t3 = 0;

    if (angleToUnk >= Q12_ANGLE(-45.0f) && angleToUnk <= Q12_ANGLE(45.0f))
    {
        var_v1 = 0;
    }
    else
    {
        if (angleToUnk >= Q12_ANGLE(-135.0f) && angleToUnk <= Q12_ANGLE(135.0f))
        {
            var_v1 = 1;
        }
        else
        {
            var_v1 = 2;
        }
    }

    arg1 &= 3;
    arg2 &= 3;

    var_a0  = sharedData_800EEAC4_2_s00[var_v1][arg1][arg2];
    temp_v1 = var_a0 + 0x1C;

    switch (temp_a1)
    {
        case 33:
            var_a0 = 10;
            var_a2 = 25;
            break;

        case 49:
            var_a2 = 24;
            break;

        default:
            var_t3 = 1;

            if (temp_t2 >= 205)
            {
                var_a2 = 21;
            }
            else
            {
                if (temp_t2 < -204)
                {
                    var_a2 = 23;
                }
                else
                {
                    var_a2 = 22;
                }
            }
            break;
    }

    var_a3 = sharedData_800CAA98_0_s01.field_380[temp_v1][0];
    var_t0 = sharedData_800CAA98_0_s01.field_380[temp_v1][1];

    if (angleToUnk <= Q12_ANGLE(0.5f))
    {
        if (angleToUnk < Q12_ANGLE(-0.5f))
        {
            var_t0 = -var_t0;
            var_a3 = -var_a3;
        }
        else
        {
            var_t0 = 0;
            var_a3 = 0;
        }
    }

    temp  = sharedData_800CAA98_0_s01.field_380[var_a0][0];
    temp2 = sharedData_800CAA98_0_s01.field_380[var_a0][1];

    sharedData_800E21D0_0_s01.field_B4[0][2] = temp;
    sharedData_800E21D0_0_s01.field_B4[0][1] = temp2;

    ptr   = &sharedData_800CAA98_0_s01;
    temp3 = ptr->field_380[var_a2][0];
    temp4 = ptr->field_380[var_a2][1];

    idx                                        = 1;
    sharedData_800E21D0_0_s01.field_B4[idx][2] = temp3;
    sharedData_800E21D0_0_s01.field_B4[idx][1] = temp4;

    if (var_t3 != 0)
    {
        sharedData_800E21D0_0_s01.field_B4[idx][3] = temp_t2;
        sharedData_800E21D0_0_s01.field_B4[idx][0] = 1;
    }

    do
    {
    } while (false); // @hack

    idx                                        = 3;
    sharedData_800E21D0_0_s01.field_B4[idx][2] = var_a3;
    sharedData_800E21D0_0_s01.field_B4[idx][1] = var_t0;
    sharedData_800E21D0_0_s01.field_B4[idx][3] = angleToUnk;
    sharedData_800E21D0_0_s01.field_B4[idx][0] = 1;

    sharedFunc_800D5E78_0_s01(airScreamer, angleToUnk / 2);
}

void sharedFunc_800E041C_2_s00(s_SubCharacter* airScreamer)
{
    q19_12           angle0;
    q19_12           angle1;
    s32              idx;
    s_func_800D2E04* src;

    angle0 = func_80080478(&airScreamer->position, &airScreamerProps.targetPosition);
    angle1 = Q12_ANGLE_NORM_S(angle0 - airScreamer->rotation.vy);

    src = &sharedData_800CAA98_0_s01;

    idx                                        = 3;
    sharedData_800E21D0_0_s01.field_B4[idx][2] = 0;

    idx                                      = 0;
    sharedData_800E21D0_0_s01.field_B4[0][2] = src->field_380[9][0];
    sharedData_800E21D0_0_s01.field_B4[0][1] = src->field_380[9][1];

    idx                                        = 1;
    sharedData_800E21D0_0_s01.field_B4[idx][2] = src->field_380[27][0];
    sharedData_800E21D0_0_s01.field_B4[idx][1] = src->field_380[27][1];

    idx                                        = 3;
    sharedData_800E21D0_0_s01.field_B4[idx][1] = src->field_380[37][1];

    if (angle1 < Q12_ANGLE(0.0f))
    {
        angle1 += Q12_ANGLE(0.3f);
    }

    sharedFunc_800D5E78_0_s01(airScreamer, angle1 >> 2);
}

void sharedFunc_800E04B0_2_s00(s_SubCharacter* chara)
{
    s32              idx;
    s_func_800D2E04* src;

    src = &sharedData_800CAA98_0_s01;

    idx                                      = 0;
    sharedData_800E21D0_0_s01.field_B4[0][2] = src->field_380[9][0];
    sharedData_800E21D0_0_s01.field_B4[0][1] = src->field_380[9][1];

    idx                                        = 3;
    sharedData_800E21D0_0_s01.field_B4[idx][2] = 0;

    idx                                        = 1;
    sharedData_800E21D0_0_s01.field_B4[idx][2] = src->field_380[26][0];
    sharedData_800E21D0_0_s01.field_B4[idx][1] = src->field_380[26][1];

    idx                                        = 3;
    sharedData_800E21D0_0_s01.field_B4[idx][1] = src->field_380[37][1];

    sharedFunc_800D5E78_0_s01(chara, 0);
}

void sharedFunc_800E0514_2_s00(s_SubCharacter* airScreamer)
{
    q19_12           angleDeltaToTarget;
    q19_12           deltaY;
    s32              var_t5;
    s32              var_t6;
    s32              temp_a0;
    s32              idx;
    s32              idx2;
    s_func_800D2E04* ptr;

    deltaY = airScreamerProps.targetPosition.vy - airScreamer->position.vy;

    angleDeltaToTarget = func_80080478(&airScreamer->position, &airScreamerProps.targetPosition);
    angleDeltaToTarget = Q12_ANGLE_NORM_S(angleDeltaToTarget - airScreamer->rotation.vy);

    temp_a0 = sharedData_800CAA98_0_s01.field_380[37][0];
    var_t6  = sharedData_800CAA98_0_s01.field_380[37][1];
    var_t5  = temp_a0 / 2;

    if (angleDeltaToTarget <= Q12_ANGLE(0.5f))
    {
        if (angleDeltaToTarget < Q12_ANGLE(-0.5f))
        {
            var_t6 = -var_t6;
            var_t5 = -var_t5;
        }
        else
        {
            var_t6 = 0;
            var_t5 = 0;
        }
    }

    idx2                                        = 1;
    sharedData_800E21D0_0_s01.field_B4[idx2][3] = deltaY;
    sharedData_800E21D0_0_s01.field_B4[idx2][0] = 1;

    idx                                        = 3;
    sharedData_800E21D0_0_s01.field_B4[idx][2] = var_t5;
    sharedData_800E21D0_0_s01.field_B4[idx][1] = var_t6;
    sharedData_800E21D0_0_s01.field_B4[idx][3] = angleDeltaToTarget;
    sharedData_800E21D0_0_s01.field_B4[idx][0] = 1;

    ptr = &sharedData_800CAA98_0_s01;

    sharedData_800E21D0_0_s01.field_B4[0][2] = ptr->field_380[9][0];
    sharedData_800E21D0_0_s01.field_B4[0][1] = ptr->field_380[9][1];

    sharedData_800E21D0_0_s01.field_B4[idx2][2] = ptr->field_380[22][0];
    sharedData_800E21D0_0_s01.field_B4[idx2][1] = ptr->field_380[22][1];

    sharedFunc_800D5E78_0_s01(airScreamer, angleDeltaToTarget / 2);
}

#endif

void sharedFunc_800D5E78_0_s01(s_SubCharacter* airScreamer, q19_12 angle) // 0x800D5E78
{
    q19_12                       angleDelta;
    q19_12                       angleCpy;
    q19_12                       angle0;
    s_sharedData_800E21D0_0_s01* base;
    s32                          idx0;

    angleCpy = angle;
    if (angleCpy > Q12_ANGLE(60.0f))
    {
        angleCpy = Q12_ANGLE(60.0f);
    }
    else if (angleCpy < Q12_ANGLE(-60.0f))
    {
        angleCpy = Q12_ANGLE(-60.0f);
    }

    angleDelta = angleCpy - airScreamer->angleToTarget;
    if (angleDelta > Q12_ANGLE(0.5f))
    {
        angleDelta -= Q12_ANGLE(0.5f);
    }
    else if (angleDelta >= Q12_ANGLE(-0.5f))
    {
        angleDelta = Q12_ANGLE(0.0f);
    }
    else
    {
        angleDelta += Q12_ANGLE(0.5f);
    }

    angle0 = Q12_ANGLE(90.0f);
    if (angleDelta <= Q12_ANGLE(0.0f))
    {
        angle0 = Q12_ANGLE(0.0f);
        if (angleDelta < Q12_ANGLE(0.0f))
        {
            angle0 = Q12_ANGLE(-90.0f);
        }
    }

    base                    = &sharedData_800E21D0_0_s01;
    idx0                    = 5;
    base->field_B4[idx0][2] = angle0;
    base->field_B4[idx0][1] = 0x1800;
    base->field_B4[idx0][3] = angleDelta;
    base->field_B4[idx0][0] = 1;
}

extern VECTOR3 sharedData_800DE1D0_0_s01;

bool sharedFunc_800D5F00_0_s01(s_SubCharacter* const airScreamer)
{
    q19_12 posX;
    q19_12 posY;
    q19_12 posZ;
    q19_12 groundHeight;
    s32    temp_s0;
    s32    temp_v0_3;
    s32    var_fp;
    s32    i;
    s32    var_s3;
    s32    var_s4;
    s32    var_s5;
    s32    var_s6;
    s32    var_s7;
    s32    var_v1;
    s32    temp;

    posX = airScreamer->position.vx;
    posY = airScreamer->position.vy;
    posZ = airScreamer->position.vz;

    groundHeight = Collision_GroundHeightGet(posX, posZ);

    if (airScreamer->moveSpeed != Q12(0.0f))
    {
        return false;
    }

    if (posY < groundHeight)
    {
        return false;
    }

    if (airScreamer->fallSpeed < Q12(0.0f))
    {
        return false;
    }

    if (airScreamer->rotationSpeed.vx != Q12_ANGLE(0.0f))
    {
        return false;
    }
    if (airScreamer->rotationSpeed.vy != Q12_ANGLE(0.0f))
    {
        return false;
    }
    if (airScreamer->rotationSpeed.vz != Q12_ANGLE(0.0f))
    {
        return false;
    }

    if (airScreamer->angleToTargetRotationSpeed != Q12_ANGLE(0.0f))
    {
        return false;
    }

    if (airScreamerProps.field_EC != 0)
    {
        return false;
    }

    var_s7 = 0;
    var_s6 = 0;
    var_fp = 1;

    for (var_s4 = 0x200; var_s4 <= 0x800; var_s4 += 0x800, var_fp++)
    {
        var_s5 = groundHeight + (var_fp << 7);

        for (i = 0, var_s3 = 0; i < 8; i++, var_s3 += 0x200)
        {
            temp_s0   = posX + Q12_MULT_PRECISE(var_s4, Math_Sin(var_s3));
            temp_v0_3 = Collision_GroundHeightGet(temp_s0, Q12_MULT_PRECISE(var_s4, Math_Cos(var_s3)) + posZ);

            if (var_s5 < temp_v0_3)
            {
                var_s7 = (1 << i);
                var_s6 = 1;
                var_s5 = temp_v0_3;
            }
            else if (temp_v0_3 == var_s5)
            {
                var_s7 |= (1 << i);
                var_s6 += 1;
            }
        }

        if (var_s6)
        {
            var_v1 = Q12_MULT(Rng_RandQ12(), var_s6);

            for (i = 0; i < 8; i++)
            {
                temp = 1 << i;
                if (!(var_s7 & temp))
                {
                    continue;
                }

                if (!var_v1)
                {
                    break;
                }

                var_v1--;
            }

            var_s3 = i << 9;
            break;
        }
    }

    if (var_s6 == 0)
    {
        return true;
    }

    sharedData_800E21D0_0_s01.flags |= 0x20000000;
    var_s4                             = Q12_MULT_PRECISE(g_DeltaTime, Q12(0.5f));

    sharedData_800DE1D0_0_s01.vx = Q12_MULT_PRECISE(var_s4, Math_Sin(var_s3));
    sharedData_800DE1D0_0_s01.vy = var_s4;
    sharedData_800DE1D0_0_s01.vz = Q12_MULT_PRECISE(var_s4, Math_Cos(var_s3));

    sharedFunc_800D81D0_0_s01(airScreamer);

    airScreamer->collision.cylinder.radius = Q12(0.0f);

    Collision_WallDetect(&g_AirScreamer_CollisionResult, &sharedData_800DE1D0_0_s01, airScreamer);

    sharedFunc_800D8244_0_s01(airScreamer);

    posX += g_AirScreamer_CollisionResult.offset.vx;
    posY += g_AirScreamer_CollisionResult.offset.vy;
    posZ += g_AirScreamer_CollisionResult.offset.vz;

    groundHeight = Collision_GroundHeightGet(posX, posZ);

    posY = MIN(groundHeight, posY);

    airScreamer->position.vx = posX;
    airScreamer->position.vy = posY;
    airScreamer->position.vz = posZ;

    return false;
}

// Returns index. Called in `AirScreamer_Update`.
bool sharedFunc_800D62D8_0_s01(s_SubCharacter* airScreamer)
{
    if (sharedFunc_800D4A80_0_s01(airScreamer) != 0 && !(sharedData_800E21D0_0_s01.flags & (1 << 29)))
    {
        sharedFunc_800D63A4_0_s01(airScreamer);
        sharedFunc_800D6600_0_s01(airScreamer);
        sharedFunc_800D633C_0_s01(airScreamer);
    }

    sharedFunc_800D7560_0_s01(airScreamer);
    return true;
}

void sharedFunc_800D633C_0_s01(s_SubCharacter* airScreamer)
{
    q19_12 posY;

    func_8006F250(&sharedData_800E2370_0_s01, airScreamer->position.vx, airScreamer->position.vz, Q12(0.0f), Q12(0.0f));

    posY = sharedData_800E2370_0_s01[1] - airScreamer->collision.box.top;
    if (airScreamer->position.vy < posY)
    {
        airScreamer->position.vy = posY;
    }
}

void sharedFunc_800D63A4_0_s01(s_SubCharacter* airScreamer)
{
    q3_12  headingAngle;
    q19_12 moveSpeed;
    q19_12 posX;
    q19_12 posY;
    q19_12 posZ;
    s32    temp_s0;
    s32    magSqr;

    sharedFunc_800D6EC4_0_s01(airScreamer);

    headingAngle                 = airScreamer->rotation.vy;
    moveSpeed                    = airScreamer->moveSpeed;
    posY                         = airScreamer->fallSpeed;
    airScreamer->headingAngle = headingAngle;

    sharedData_800DE1F0_0_s01.vx = Q12_MULT_PRECISE(moveSpeed, Math_Sin(headingAngle));
    sharedData_800DE1F0_0_s01.vy = posY;
    sharedData_800DE1F0_0_s01.vz = Q12_MULT_PRECISE(moveSpeed, Math_Cos(headingAngle));

    sharedData_800DE1E0_0_s01.vx = Q12_MULT_PRECISE(g_DeltaTime, sharedData_800DE1F0_0_s01.vx);
    sharedData_800DE1E0_0_s01.vy = Q12_MULT_PRECISE(g_DeltaTime, sharedData_800DE1F0_0_s01.vy);
    sharedData_800DE1E0_0_s01.vz = Q12_MULT_PRECISE(g_DeltaTime, sharedData_800DE1F0_0_s01.vz);

    temp_s0 = sharedFunc_800D6A60_0_s01(&g_AirScreamer_CollisionResult.offset,
                                        &sharedData_800DE1F0_0_s01,
                                        airScreamer,
                                        sharedFunc_800D7440_0_s01(&g_AirScreamer_CollisionResult, &sharedData_800DE1E0_0_s01, airScreamer),
                                        &sharedData_800E21D0_0_s01.field_11C);

    airScreamerProps.flags &= ~AirScreamerFlag_29;

    // @hack Permuter find.
    temp_s0++;
    temp_s0--;

    if (sharedFunc_800D4A80_0_s01(airScreamer) == 3)
    {
        posX = sharedData_800E21D0_0_s01.field_11C.vx;
        posZ = sharedData_800E21D0_0_s01.field_11C.vz;

        magSqr = Q12_MULT_PRECISE(posX, posX) + Q12_MULT_PRECISE(posZ, posZ);
        if (magSqr < Q12(0.49f))
        {
            airScreamerProps.flags |= AirScreamerFlag_29;
        }
    }

    airScreamer->fallSpeed = sharedData_800E21D0_0_s01.field_B4[6][3];
    sharedFunc_800D6C7C_0_s01(&sharedData_800DE1E0_0_s01, airScreamer, temp_s0, &sharedData_800E21D0_0_s01.field_134);
}

void sharedFunc_800D6600_0_s01(s_SubCharacter* airScreamer)
{
    q19_12 newPosX;
    q19_12 sp1C;
    q19_12 newPosY;
    q19_12 angle0;
    q19_12 angle2;
    q19_12 angle1;
    q19_12 newPosZ;
    q19_12 posX;
    q19_12 posY;
    q19_12 posZ;
    q20_12 angle3;
    s32    temp;

    newPosX = Q12_MULT_PRECISE(airScreamer->damage.position.vx, Q12(3.0f));
    newPosY = Q12_MULT_PRECISE(airScreamer->damage.position.vy, Q12(3.0f));
    newPosZ = Q12_MULT_PRECISE(airScreamer->damage.position.vz, Q12(3.0f));

    airScreamer->damage.position.vx = Q12(0.0f);
    airScreamer->damage.position.vy = Q12(0.0f);
    airScreamer->damage.position.vz = Q12(0.0f);

    sp1C = SquareRoot12(Q12_SQUARE_PRECISE(newPosX) +
                        Q12_SQUARE_PRECISE(newPosY) +
                        Q12_SQUARE_PRECISE(newPosZ));

    angle2 = airScreamerProps.field_EC;
    angle0 = airScreamerProps.field_F0;
    angle1 = airScreamerProps.field_F2;

    if (angle2 == Q12_ANGLE(0.0f))
    {
        posZ = Q12(0.0f);
        posY = Q12(0.0f);
        posX = Q12(0.0f);
    }
    else
    {
        posY   = Q12_MULT_PRECISE(angle2, Math_Sin(angle0));
        angle0 = Q12_MULT_PRECISE(angle2, Math_Cos(angle0));

        if (angle0 < Q12_ANGLE(0.0f))
        {
            angle0  = -angle0;
            angle1 ^= 0x800;
        }

        posX = Q12_MULT_PRECISE(angle0, Math_Sin(angle1));
        posZ = Q12_MULT_PRECISE(angle0, Math_Cos(angle1));
    }

    if (sp1C > 0)
    {
        posX += newPosX;
        posY += newPosY;
        posZ += newPosZ;
    }

    if (!(posX | posY | posZ))
    {
        airScreamerProps.field_EC = Q12_ANGLE(0.0f); // } Presumably angles.
        airScreamerProps.field_F0 = Q12_ANGLE(0.0f); // }
        airScreamerProps.field_F2 = airScreamer->rotation.vy;
        return;
    }

    sharedData_800DE210_0_s01.vx = posX;
    sharedData_800DE210_0_s01.vy = posY;
    sharedData_800DE210_0_s01.vz = posZ;

    sharedData_800DE200_0_s01.vx = Q12_MULT_PRECISE(g_DeltaTime, posX);
    sharedData_800DE200_0_s01.vy = Q12_MULT_PRECISE(g_DeltaTime, posY);
    sharedData_800DE200_0_s01.vz = Q12_MULT_PRECISE(g_DeltaTime, posZ);

    temp = sharedFunc_800D7440_0_s01(&g_AirScreamer_CollisionResult, &sharedData_800DE200_0_s01, airScreamer);
    temp = sharedFunc_800D6A60_0_s01(&g_AirScreamer_CollisionResult.offset, &sharedData_800DE210_0_s01, airScreamer, temp, &sharedData_800E21D0_0_s01.field_128);

    sharedFunc_800D6C7C_0_s01(&sharedData_800DE200_0_s01, airScreamer, temp, &sharedData_800E21D0_0_s01.field_140);

    angle1 = ratan2(posX, posZ);
    angle0 = Q12_SQUARE_PRECISE(posX) +
             Q12_SQUARE_PRECISE(posZ);
    angle2 = angle0 + Q12_SQUARE_PRECISE(posY);
    angle0 = SquareRoot12(angle0);
    angle2 = SquareRoot12(angle2);
    angle0 = ratan2(posY, angle0);
    angle3 = Q12_MULT_PRECISE(g_DeltaTime, Q12(6.0f));

    if (angle2 < angle3)
    {
        angle2 = Q12_ANGLE(0.0f);
    }
    else
    {
        angle2 = angle2 - angle3;
    }

    airScreamerProps.field_EC = angle2;
    airScreamerProps.field_F0 = angle0;
    airScreamerProps.field_F2 = angle1;
}

s32 sharedFunc_800D6A60_0_s01(VECTOR3* offset, VECTOR3* vec1, s_SubCharacter* airScreamer, s32 arg3, VECTOR3* vec2)
{
    q19_12   offsetX;
    q19_12   offsetZ;
    s32      vec0Y;
    s32      vec1X;
    s32      vec1Y;
    s32      vec1Z;
    s32      vec2X;
    s32      vec2Z;
    q19_12   posY;
    q19_12   groundHeight;
    s32      newVec1Y;
    s32      newVec2Y;
    s32      time;
    s32      ret;
    s32      retCode1;
    s32      flags;
    VECTOR3* localOffset; // Q19.12

    flags   = airScreamerProps.flags;
    localOffset = offset;

    offsetX = localOffset->vx;
    offsetZ = localOffset->vz;

    ret = arg3;

    airScreamer->position.vx += offsetX;
    airScreamer->position.vz += offsetZ;

    if (ret == 0)
    {
        flags   &= ~AirScreamerFlag_31;
        vec1X    = 0;
        vec2->vx = vec1->vx;
        vec1Z    = 0;
        vec2->vz = vec1->vz;
    }
    else
    {
        flags |= AirScreamerFlag_31;
        time   = Q12(4096.0f) / g_DeltaTime;

        if (!(offsetX | offsetZ))
        {
            vec2X    = Q12(0.0f);
            vec2Z    = Q12(0.0f);
            vec2->vx = Q12(0.0f);
            vec2->vz = Q12(0.0f);
        }
        else
        {
            vec2X    = Q12_MULT_PRECISE(offsetX, time);
            vec2->vx = vec2X;
            vec2Z    = Q12_MULT_PRECISE(offsetZ, time);
            vec2->vz = vec2Z;
        }

        vec1X = vec2X - vec1->vx;
        vec1Z = vec2Z - vec1->vz;
    }

    vec1->vx = vec1X;
    vec1->vz = vec1Z;
    posY     = airScreamer->position.vy;
    vec1Y    = vec1->vy;
    vec0Y    = localOffset->vy;

    groundHeight                        = Collision_GroundHeightGet(airScreamer->position.vx, airScreamer->position.vz);
    sharedData_800E21D0_0_s01.field_114 = groundHeight;
    retCode1                            = 1;

    if (airScreamer->collision.state == CharaCollisionState_Ignore)
    {
        newVec2Y = Q12(0.0f);
        newVec1Y = Q12(0.0f);
    }
    else if (posY >= groundHeight && vec0Y >= Q12(0.0f))
    {
        flags   &= ~AirScreamerFlag_30;
        posY     = groundHeight;
        newVec2Y = Q12(0.0f);
        newVec1Y = Q12(0.0f);
    }
    else
    {
        posY += vec0Y;
        if (posY >= groundHeight)
        {
            flags   |= AirScreamerFlag_30;
            ret      = retCode1;
            newVec2Y = Q12(0.0f);
            newVec1Y = -vec1Y;
            posY     = groundHeight;
        }
        else
        {
            flags   &= ~AirScreamerFlag_30;
            newVec2Y = vec1Y;
            newVec1Y = Q12(0.0f);
        }
    }

    airScreamer->position.vy = posY;
    vec2->vy                 = newVec2Y;
    vec1->vy                 = newVec1Y;
    airScreamerProps.flags   = flags;
    return ret;
}

void sharedFunc_800D6C7C_0_s01(VECTOR* arg0, s_SubCharacter* airScreamer, s32 arg2, VECTOR3* arg3)
{
    s32    sqr;
    q19_12 offsetX;
    q19_12 offsetY;
    q19_12 offsetZ;

    if (arg2 == 0)
    {
        offsetX = Q12(0.0f);
        offsetY = Q12(0.0f);
        offsetZ = Q12(0.0f);
    }
    else
    {
        offsetX = arg0->vx;
        offsetY = arg0->vy;
        offsetZ = arg0->vz;

        sqr  = Q12_MULT_PRECISE(offsetX, offsetX);
        sqr += Q12_MULT_PRECISE(offsetY, offsetY);
        sqr += Q12_MULT_PRECISE(offsetZ, offsetZ);
        sqr  = SquareRoot12(sqr);

        airScreamer->damage.amount += Math_PreservedSignSubtract(sqr, Q12(5.0f));
        if (sqr != Q12(0.0f))
        {
            sqr = Q12(Math_PreservedSignSubtract(sqr, Q12(6.0f))) / sqr;
        }

        offsetX = Q12_MULT_PRECISE(offsetX, sqr);
        offsetY = Q12_MULT_PRECISE(offsetY, sqr);
        offsetZ = Q12_MULT_PRECISE(offsetZ, sqr);
        if (offsetX | offsetY | offsetZ)
        {
            airScreamer->damage.position.vx += Q12_MULT_PRECISE(offsetX, Q12(0.8f));
            airScreamer->damage.position.vy += Q12_MULT_PRECISE(offsetY, Q12(0.8f));
            airScreamer->damage.position.vz += Q12_MULT_PRECISE(offsetZ, Q12(0.8f));
        }
    }

    arg3->vx = offsetX;
    arg3->vy = offsetY;
    arg3->vz = offsetZ;
}

void sharedFunc_800D6EC4_0_s01(s_SubCharacter* airScreamer)
{
    s32    rotSpeedX;
    s32    targetRotSpeed;
    s32    rotSpeedZ;
    s32    element2;
    q19_12 rotSpeedCpy;

    sharedData_800E21D0_0_s01.flags &= ~0x2B; // TODO: What flags are these?

    rotSpeedZ  = sharedData_800E21D0_0_s01.field_B4[0][1];
    element2  = sharedData_800E21D0_0_s01.field_B4[0][2];
    rotSpeedCpy = airScreamer->moveSpeed;
    if (sharedData_800E21D0_0_s01.field_B4[0][0])
    {
        sharedData_800E21D0_0_s01.field_B4[0][0] = 0;
        rotSpeedCpy                                = sharedFunc_800D71F0_0_s01(rotSpeedCpy, rotSpeedZ, element2, sharedData_800E21D0_0_s01.field_B4[0][3]);
    }
    else
    {
        rotSpeedCpy = sharedFunc_800D7120_0_s01(rotSpeedCpy, rotSpeedZ, element2);
    }
    airScreamer->moveSpeed = rotSpeedCpy;

    rotSpeedCpy = airScreamer->fallSpeed;
    rotSpeedZ  = sharedData_800E21D0_0_s01.field_B4[1][1];
    element2  = sharedData_800E21D0_0_s01.field_B4[1][2];
    if (sharedData_800E21D0_0_s01.field_B4[1][0])
    {
        sharedData_800E21D0_0_s01.field_B4[1][0] = 0;
        rotSpeedCpy                                = sharedFunc_800D71F0_0_s01(rotSpeedCpy, rotSpeedZ, element2, sharedData_800E21D0_0_s01.field_B4[1][3]);
    }
    else
    {
        rotSpeedCpy = sharedFunc_800D7120_0_s01(rotSpeedCpy, rotSpeedZ, element2);
    }
    airScreamer->fallSpeed = rotSpeedCpy;

    rotSpeedCpy = airScreamer->rotationSpeed.vy;
    rotSpeedZ  = sharedData_800E21D0_0_s01.field_B4[3][1];
    element2  = sharedData_800E21D0_0_s01.field_B4[3][2];
    if (sharedData_800E21D0_0_s01.field_B4[3][0])
    {

        rotSpeedCpy = sharedFunc_800D71F0_0_s01(rotSpeedCpy, rotSpeedZ, element2, sharedData_800E21D0_0_s01.field_B4[3][3]);
    }
    else
    {
        rotSpeedCpy = sharedFunc_800D7120_0_s01(rotSpeedCpy, rotSpeedZ, element2);
    }
    airScreamer->rotationSpeed.vy = rotSpeedCpy;

    airScreamer->rotation.vy += Q12_MULT_PRECISE(g_DeltaTime, rotSpeedCpy);
    if (sharedFunc_800D4A80_0_s01(airScreamer) == 3)
    {
        sharedFunc_800D72E8_0_s01(airScreamer, airScreamer->fallSpeed, airScreamer->rotationSpeed.vy);
    }
    else
    {
        sharedFunc_800D72E8_0_s01(airScreamer, 0, Q12_ANGLE(0.0f));
    }

    rotSpeedX                     = sharedFunc_800D71F0_0_s01(airScreamer->rotationSpeed.vx, sharedData_800E21D0_0_s01.field_B4[2][1], sharedData_800E21D0_0_s01.field_B4[2][2], sharedData_800E21D0_0_s01.field_B4[2][3]);
    rotSpeedCpy                   = rotSpeedX;
    airScreamer->rotationSpeed.vx = rotSpeedX;
    airScreamer->rotation.vx     += Q12_MULT_PRECISE(g_DeltaTime, rotSpeedCpy);
    rotSpeedZ                     = sharedFunc_800D71F0_0_s01(airScreamer->rotationSpeed.vz, sharedData_800E21D0_0_s01.field_B4[4][1], sharedData_800E21D0_0_s01.field_B4[4][2], sharedData_800E21D0_0_s01.field_B4[4][3]);
    rotSpeedCpy                   = rotSpeedZ;
    airScreamer->rotationSpeed.vz = rotSpeedCpy;

    airScreamer->rotation.vz        += Q12_MULT_PRECISE(g_DeltaTime, rotSpeedCpy);
    rotSpeedCpy                      = sharedFunc_800D71F0_0_s01(airScreamer->angleToTargetRotationSpeed, sharedData_800E21D0_0_s01.field_B4[5][1], sharedData_800E21D0_0_s01.field_B4[5][2], sharedData_800E21D0_0_s01.field_B4[5][3]);
    targetRotSpeed                   = rotSpeedCpy;
    airScreamer->angleToTargetRotationSpeed = targetRotSpeed;
    airScreamer->angleToTarget      += Q12_MULT_PRECISE(g_DeltaTime, targetRotSpeed);
}

q19_12 sharedFunc_800D7120_0_s01(q19_12 moveSpeed, s32 arg1, s32 arg2)
{
    s32 temp_t0;
    s32 temp_v1;
    s32 var_a1;
    s32 adjMoveSpeed;
    s32 temp_t1;
    s32 temp_a0;
    s32 new_var2;

    var_a1       = arg1;
    adjMoveSpeed = moveSpeed;
    temp_v1      = arg2 - adjMoveSpeed;

    if (var_a1 != 0 && temp_v1 != 0)
    {
        temp_t0  = temp_v1 < 1;
        temp_t1  = temp_v1 >= 1;
        new_var2 = arg2;

        // TODO: Can `temp_t0` / `temp_t1` be merged here somehow?
        if (temp_v1 != 0 & (var_a1 > 0 ? temp_t0 : temp_t1))
        {
            var_a1 = -var_a1;
        }

        // TODO: Related to `MoveSpeedUpdate`/`APPROACH` macros?
        if (var_a1 > 0)
        {
            if (temp_v1 > 0)
            {
                adjMoveSpeed += Q12_MULT_PRECISE(g_DeltaTime, var_a1);
                if (new_var2 < adjMoveSpeed)
                {
                    adjMoveSpeed = new_var2;
                }
            }
        }
        else if (temp_v1 < 0)
        {
            adjMoveSpeed += Q12_MULT_PRECISE(g_DeltaTime, var_a1);
            if (adjMoveSpeed < new_var2)
            {
                adjMoveSpeed = new_var2;
            }
        }
    }

    return adjMoveSpeed;
}

q19_12 sharedFunc_800D71F0_0_s01(q19_12 moveSpeed, s32 arg1, s32 arg2, s32 arg3)
{
    s32 var_v1;
    s32 arg2Sqr;

    if (arg1 == 0)
    {
        return moveSpeed;
    }

    if (arg2 != 0)
    {
        if (arg1 < 0)
        {
            arg1 = -arg1;
        }

        arg2Sqr  = Q12_MULT_PRECISE(arg2, arg2) << Q12_SHIFT;
        arg2Sqr /= arg1 * 2;

        if (arg3 <= 0)
        {
            arg3 = -arg3;
        }

        if (arg3 < arg2Sqr)
        {
            var_v1 = SquareRoot12(Q12_MULT_PRECISE(arg3, arg1 * 2));
            if (var_v1 == 0)
            {
                var_v1 = 1;
            }

            if (arg2 > 0)
            {
                if (var_v1 < arg2)
                {
                    arg2 = var_v1;
                }
            }
            else
            {
                if (arg2 < -var_v1)
                {
                    arg2 = -var_v1;
                }
            }
        }
    }

    return sharedFunc_800D7120_0_s01(moveSpeed, arg1, arg2);
}

static void inline sharedFunc_800D72E8_0_s01_subfunc(q19_12 angle, s32 idx)
{
    q19_12 angle1;

    if (angle > Q12_ANGLE(0.5f))
    {
        angle -= Q12_ANGLE(0.5f);
    }
    else
    {
        if (angle < Q12_ANGLE(-0.5f))
        {
            angle += Q12_ANGLE(0.5f);
        }
        else
        {
            angle = Q12_ANGLE(0.0f);
        }
    }

    if (angle > Q12_ANGLE(0.0f))
    {
        angle1 = Q12_ANGLE(120.0f);
    }
    else if (angle < Q12_ANGLE(0.0f))
    {
        angle1 = Q12_ANGLE(-120.0f);
    }
    else
    {
        angle1 = Q12_ANGLE(0.0f);
    }

    sharedData_800E21D0_0_s01.field_B4[idx][2] = angle1;
    sharedData_800E21D0_0_s01.field_B4[idx][1] = Q12(1.3333f);
    sharedData_800E21D0_0_s01.field_B4[idx][3] = angle;
    sharedData_800E21D0_0_s01.field_B4[idx][0] = 1;
}

void sharedFunc_800D72E8_0_s01(s_SubCharacter* airScreamer, q19_12 angle0, q19_12 rotSpeedY)
{
    s32 angle1;

    // TODO: Not sure what these are multiplying with. Different speeds?
    angle1 = Q12_MULT_PRECISE(angle0, Q12(-0.02f));
    if (angle1 > Q12_ANGLE(45.0f))
    {
        angle1 = Q12_ANGLE(45.0f);
    }
    else if (angle1 < Q12_ANGLE(-45.0f))
    {
        angle1 = Q12_ANGLE(-45.0f);
    }
    sharedFunc_800D72E8_0_s01_subfunc(angle1 - airScreamer->rotation.vx, 2);

    angle1 = Q12_MULT_PRECISE(rotSpeedY, Q12(0.15f));
    if (angle1 > Q12_ANGLE(45.0f))
    {
        angle1 = Q12_ANGLE(45.0f);
    }
    else if (angle1 < Q12_ANGLE(-45.0f))
    {
        angle1 = Q12_ANGLE(-45.0f);
    }
    sharedFunc_800D72E8_0_s01_subfunc(angle1 - airScreamer->rotation.vz, 4);
}

s32 sharedFunc_800D7440_0_s01(s_CollisionResult* collResult, VECTOR* offset, s_SubCharacter* airScreamer)
{
    q19_12             groundHeight;
    q19_12             posY;
    s32                wallResponse;
    s32                cond;
    s_CollisionResult* localCollResult;

    localCollResult = collResult;

    sharedFunc_800D81D0_0_s01(airScreamer);
    sharedFunc_800D8714_0_s01(airScreamer, airScreamer->moveSpeed, airScreamer->headingAngle);
    sharedFunc_800D87FC_0_s01(airScreamer);

    wallResponse = Collision_WallDetect(localCollResult, offset, airScreamer);

    posY         = airScreamer->position.vy;
    groundHeight = Collision_GroundHeightGet(airScreamer->position.vx + localCollResult->offset.vx,
                                             airScreamer->position.vz + localCollResult->offset.vz);

    if (sharedFunc_800D4A80_0_s01(airScreamer) != 4)
    {
        if (offset->vy > Q12(0.0f) && groundHeight < (posY - Q12(0.5f)))
        {
            localCollResult->offset.vx = Q12(0.0f);
            localCollResult->offset.vz = Q12(0.0f);
        }
    }
    else if (groundHeight < posY)
    {
        localCollResult->offset.vx = Q12(0.0f);
        localCollResult->offset.vz = Q12(0.0f);
    }

    if ((FP_FROM(offset->vx, Q4_SHIFT) != FP_FROM(localCollResult->offset.vx, Q4_SHIFT)) ||
        (FP_FROM(offset->vz, Q4_SHIFT) != FP_FROM(localCollResult->offset.vz, Q4_SHIFT)))
    {
        wallResponse = 1;
    }

    sharedFunc_800D8244_0_s01(airScreamer);
    return wallResponse;
}

void sharedFunc_800D7560_0_s01(s_SubCharacter* airScreamer)
{
    q19_12   headingAngle;
    q19_12   sinHeadingAngle;
    q19_12   cosHeadingAngle;
    q19_12   offsetX;
    q19_12   offsetZ;
    MATRIX*  mat;
    SVECTOR* rot;

    mat                                       = (s32*)(sharedData_800E21D0_0_s01.coords_8) + 1;
    *(s32*)sharedData_800E21D0_0_s01.coords_8 = 0;
    rot                                       = &airScreamer->rotation;
    Math_RotMatrixZxyNegGte(rot, mat);

    if (airScreamerProps.field_E8_0 == 3)
    {
        ScaleMatrix(mat, &sharedData_800DD870_0_s01);
    }
    else if (airScreamerProps.flags & AirScreamerFlag_12)
    {
        ScaleMatrix(mat, &sharedData_800DD880_0_s01);
    }

    headingAngle = rot->vy;
    offsetZ      = Q12(0.0f);
    if (sharedData_800E21D0_0_s01.flags < 0)
    {
        offsetX = Q12(0.0f);
    }
    else
    {
        sinHeadingAngle = Math_Sin(headingAngle);
        offsetX         = Q12_MULT_PRECISE(sinHeadingAngle, Q12(-0.17f));

        cosHeadingAngle = Math_Cos(headingAngle);
        offsetZ         = Q12_MULT_PRECISE(cosHeadingAngle, Q12(-0.17f));
    }

    mat->t[0] = Q12_TO_Q8(airScreamer->position.vx + offsetX);
    mat->t[1] = Q12_TO_Q8(airScreamer->position.vy);
    mat->t[2] = Q12_TO_Q8(airScreamer->position.vz + offsetZ);
}

void sharedFunc_800D76A0_0_s01(s_SubCharacter* airScreamer)
{
    if (airScreamer->model.anim.status == ANIM_STATUS(AirScreamerAnim_StandIdle, true) ||
        airScreamer->model.anim.status == ANIM_STATUS(AirScreamerAnim_StandBiteAttackStart, true) ||
        airScreamer->model.anim.status == ANIM_STATUS(AirScreamerAnim_StandBiteAttackEnd, true))
    {
        if (sharedFunc_800D77D0_0_s01(airScreamer) != Q12(0.0f))
        {
            airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_WalkForward, false);
        }
    }
    else if (airScreamer->model.anim.status == ANIM_STATUS(AirScreamerAnim_WalkForward, true))
    {
        if (sharedFunc_800D77D0_0_s01(airScreamer) == Q12(0.0f))
        {
            airScreamer->model.anim.status = ANIM_STATUS(AirScreamerAnim_StandIdle, false);
        }
    }
}

q19_12 sharedFunc_800D7714_0_s01(s_SubCharacter* airScreamer)
{
    bool   isNotControlState1;
    q19_12 ret;

    isNotControlState1 = airScreamer->model.controlState != AirScreamerControl_1;

    switch (airScreamer->model.anim.status)
    {
        case ANIM_STATUS(AirScreamerAnim_StandIdle, true):
            ret = Q12(30.0f);
            if (!isNotControlState1)
            {
                ret = Q12(15.0f);
            }
            break;

        case ANIM_STATUS(AirScreamerAnim_HoverVariable, false):
            ret = Q12(24.0f);
            if (!isNotControlState1)
            {
                ret = Q12(30.0f);
            }
            break;

        case ANIM_STATUS(AirScreamerAnim_Glide, false):
            ret = Q12(40.0f);
            if (!isNotControlState1)
            {
                ret = Q12(60.0f);
            }
            break;

        case ANIM_STATUS(AirScreamerAnim_StandIdleToGlide, false):
            ret = Q12(72.0f);
            if (!isNotControlState1)
            {
                ret = Q12(30.0f);
            }
            break;

        case ANIM_STATUS(AirScreamerAnim_StandIdleToGlide, true):
            ret = Q12(40.0f);
            if (!isNotControlState1)
            {
                ret = Q12(30.0f);
            }
            break;

        case ANIM_STATUS(AirScreamerAnim_GlideToHover, false):
            ret = Q12(72.0f);
            if (!isNotControlState1)
            {
                ret = Q12(30.0f);
            }
            break;

        case ANIM_STATUS(AirScreamerAnim_GlideToHover, true):
            ret = Q12(18.0f);
            if (!isNotControlState1)
            {
                ret = Q12(20.0f);
            }
            break;

        default:
            ret = Q12(10.0f);
    }

    return ret;
}

q19_12 sharedFunc_800D77D0_0_s01(s_SubCharacter* airScreamer)
{
    q19_12 fallSpeed;
    bool   isNotControlState1;
    q19_12 rot;
    q19_12 moveSpeed;
    q19_12 speed_2;
    q19_12 temp_lo;
    q19_12 distToGround;
    q19_12 speed1;
    q20_12 absRot;
    q20_12 absSpeed;
    q19_12 ret;
    q19_12 ret_3;
    q19_12 var_v1;
    s32    animStatus;
    q19_12 animTime;

    animStatus         = airScreamer->model.anim.status;
    isNotControlState1 = airScreamer->model.controlState != AirScreamerControl_1;

    switch (animStatus)
    {
        case ANIM_STATUS(AirScreamerAnim_Stun, true):
            if (!(airScreamer->model.anim.keyframeIdx == 96) < 1 & airScreamer->health < 1)
            {
                return Q12(0.0f);
            }
            else
            {
                return Q12(5.0f);
            }
            break;

        case ANIM_STATUS(AirScreamerAnim_StandBiteAttackStart, true):
        case ANIM_STATUS(AirScreamerAnim_StandBiteAttackEnd, true):
        case ANIM_STATUS(AirScreamerAnim_StandIdle, true):
        case ANIM_STATUS(AirScreamerAnim_WalkForward, true):
            moveSpeed = airScreamer->moveSpeed * Q12(0.01f);
            rot       = airScreamer->rotationSpeed.vy * Q12(0.044f);

            if (!(moveSpeed | rot))
            {
                return Q12(0.0f);
            }

            if (moveSpeed < Q12(0.0f))
            {
                absSpeed = -moveSpeed;
            }
            else
            {
                absSpeed = moveSpeed;
            }

            if (rot < Q12_ANGLE(0.0f))
            {
                absRot = -rot;
            }
            else
            {
                absRot = rot;
            }

            if (absRot < absSpeed)
            {
                if (absSpeed < Q12(20.0f))
                {
                    speed1 = Q12(20.0f);
                }
                else
                {
                    speed1 = moveSpeed;
                }
            }
            else
            {
                if (absRot < Q12(20.0f))
                {
                    speed1 = Q12(20.0f);
                }
                else
                {
                    speed1 = rot;
                }
            }
            return speed1;

        case ANIM_STATUS(AirScreamerAnim_HoverVariable, true):
            if (isNotControlState1)
            {
                return Q12(24.0f);
            }

            return Q12_MULT_PRECISE(airScreamer->fallSpeed, Q12(-2.0f)) + Q12(20.0f);

        case ANIM_STATUS(AirScreamerAnim_Glide, true):
            if (isNotControlState1)
            {
                return Q12(40.0f);
            }

            fallSpeed = airScreamer->fallSpeed;
            var_v1  = airScreamer->moveSpeed - Q12(4.0f);

            ret  = Q12(20.0f);
            ret += Q12_MULT_PRECISE(ABS(var_v1), Q12(1.5f));
            ret += Q12_MULT_PRECISE(fallSpeed, Q12(-1.5f));
            return ret;

        case ANIM_STATUS(AirScreamerAnim_HoverInjuredBiteAttack, true):
        case ANIM_STATUS(AirScreamerAnim_HoverToStun, true):
        case ANIM_STATUS(AirScreamerAnim_GlideToStun, true):
        case ANIM_STATUS(AirScreamerAnim_GlideToWalkForward, true):
            distToGround = Collision_GroundHeightGet(airScreamer->position.vx, airScreamer->position.vz) -
                           airScreamer->position.vy;
            speed1       = Q12(20.0f);

            if (distToGround > Q12(0.0f))
            {
                speed_2  = airScreamer->fallSpeed;
                animTime = airScreamer->model.anim.time;

                switch (animStatus)
                {
                    case ANIM_STATUS(AirScreamerAnim_GlideToWalkForward, true):
                        ret_3 = Q12(333.0f);
                        break;

                    case ANIM_STATUS(AirScreamerAnim_HoverToStun, true):
                        ret_3 = Q12(189.0f);
                        break;

                    case ANIM_STATUS(AirScreamerAnim_GlideToStun, true):
                        ret_3 = Q12(215.0f);
                        break;

                    case ANIM_STATUS(AirScreamerAnim_HoverToStun, false):
                    default:
                        ret_3 = Q12(171.0f);
                        break;
                }

                temp_lo = FP_TO(ret_3 - animTime, Q12_SHIFT) / distToGround;
                speed1  = Q12_MULT_PRECISE(speed_2, temp_lo);
            }

            if (speed1 < Q12(0.3f))
            {
                speed1 = Q12(0.3f);
            }
            if (speed1 > Q12(30.0f))
            {
                speed1 = Q12(30.0f);
            }
            return speed1;
    }

    return Q12(10.0f);
}

bool sharedFunc_800D7AB0_0_s01(s_SubCharacter* airScreamer)
{
    GsCOORDINATE2* coords;
    s_AnmHeader*   animHdr;

    animHdr = sharedData_800E21D0_0_s01.anmHdr_4;
    coords  = sharedData_800E21D0_0_s01.coords_8;

    sharedFunc_800D76A0_0_s01(airScreamer);
    func_80044950(airScreamer, animHdr, coords);
    sharedFunc_800D7B14_0_s01(airScreamer, coords);

    return true;
}

void sharedFunc_800D7B14_0_s01(s_SubCharacter* chara, GsCOORDINATE2* boneCoords)
{
    q19_12 bendAngle;
    q3_12  axisBlend;
    q19_12 cosScaledAngle;
    q19_12 sinScaledAngle;
    q19_12 scaledAngle;

    if (chara->model.anim.flags & AnimFlag_Unlocked)
    {
        bendAngle = chara->angleToTarget;

        PushMatrix();
        sharedData_800DE220_0_s01.vx = Q12(0.0f);
        sharedData_800DE220_0_s01.vz = Q12(0.0f);
        sharedData_800DE220_0_s01.vy = Q12_MULT_FLOAT_PRECISE(bendAngle, 0.3f);
        Math_RotMatrixXyzGte(&sharedData_800DE220_0_s01, &sharedData_800DE230_0_s01);
        MulMatrix(&boneCoords[10].coord, &sharedData_800DE230_0_s01);

        scaledAngle = Q12_MULT_FLOAT_PRECISE(bendAngle, 0.5f);

        cosScaledAngle                    = Math_Cos(scaledAngle);
        sinScaledAngle                    = Math_Sin(scaledAngle);
        sharedData_800DE230_0_s01.m[0][0] = cosScaledAngle;

        // `axisBlend = sin(theta) * 1 / sqrt(2)` -> off-diagonal axis contribution.
        axisBlend                         = Q12_MULT_FLOAT_PRECISE(sinScaledAngle, 0.7071f); // `1 / sqrt(2)`
        sharedData_800DE230_0_s01.m[1][0] = axisBlend;
        sharedData_800DE230_0_s01.m[0][2] = axisBlend;
        sharedData_800DE230_0_s01.m[2][0] = -axisBlend;
        sharedData_800DE230_0_s01.m[0][1] = -axisBlend;

        // `axisBlend = (cos(theta) + 1) * 0.5` -> positive diagonal blend.
        axisBlend                         = Q12_MULT_FLOAT_PRECISE(cosScaledAngle + Q12(1.0f), 0.5f);
        sharedData_800DE230_0_s01.m[2][2] = axisBlend;
        sharedData_800DE230_0_s01.m[1][1] = axisBlend;

        // `axisBlend = (1 - cos(theta)) * 0.5` -> complementary diagonal blend.
        axisBlend                         = Q12_MULT_FLOAT_PRECISE(Q12(1.0f) - cosScaledAngle, 0.5f);
        sharedData_800DE230_0_s01.m[2][1] = axisBlend;
        sharedData_800DE230_0_s01.m[1][2] = axisBlend;

        MulMatrix(&boneCoords[9].coord, &sharedData_800DE230_0_s01);

        scaledAngle = Q12_MULT_FLOAT_PRECISE(bendAngle, 0.2f);

        cosScaledAngle                    = Math_Cos(scaledAngle);
        sinScaledAngle                    = Math_Sin(scaledAngle);
        sharedData_800DE230_0_s01.m[0][0] = cosScaledAngle;

        axisBlend                         = Q12_MULT_FLOAT_PRECISE(sinScaledAngle, 0.7071f); // `1 / sqrt(2)`
        sharedData_800DE230_0_s01.m[1][0] = axisBlend;
        sharedData_800DE230_0_s01.m[0][2] = axisBlend;
        sharedData_800DE230_0_s01.m[2][0] = -axisBlend;
        sharedData_800DE230_0_s01.m[0][1] = -axisBlend;

        axisBlend                         = Q12_MULT_FLOAT_PRECISE(cosScaledAngle + Q12(1.0f), 0.5f);
        sharedData_800DE230_0_s01.m[2][2] = axisBlend;
        sharedData_800DE230_0_s01.m[1][1] = axisBlend;

        axisBlend                         = Q12_MULT_FLOAT_PRECISE(Q12(1.0f) - cosScaledAngle, 0.5f);
        sharedData_800DE230_0_s01.m[2][1] = axisBlend;
        sharedData_800DE230_0_s01.m[1][2] = axisBlend;
        MulMatrix(&boneCoords[1].coord, &sharedData_800DE230_0_s01);

        scaledAngle = Q12_MULT_FLOAT_PRECISE(bendAngle, 0.2f);

        cosScaledAngle                    = Math_Cos(scaledAngle);
        sinScaledAngle                    = Math_Sin(scaledAngle);
        sharedData_800DE230_0_s01.m[0][0] = cosScaledAngle;

        axisBlend                         = Q12_MULT_FLOAT_PRECISE(sinScaledAngle, 0.7071f);
        sharedData_800DE230_0_s01.m[2][0] = axisBlend;
        sharedData_800DE230_0_s01.m[0][1] = axisBlend;
        sharedData_800DE230_0_s01.m[1][0] = -axisBlend;
        sharedData_800DE230_0_s01.m[0][2] = -axisBlend;

        axisBlend                         = Q12_MULT_FLOAT_PRECISE(cosScaledAngle + Q12(1.0f), 0.5f);
        sharedData_800DE230_0_s01.m[2][2] = axisBlend;
        sharedData_800DE230_0_s01.m[1][1] = axisBlend;

        axisBlend                         = Q12_MULT_FLOAT_PRECISE(Q12(1.0f) - cosScaledAngle, 0.5f);
        sharedData_800DE230_0_s01.m[2][1] = axisBlend;
        sharedData_800DE230_0_s01.m[1][2] = axisBlend;

        MulMatrix(&boneCoords[2].coord, &sharedData_800DE230_0_s01);
        PopMatrix();
    }
}

// TODO: Why is this only included in MAP0_S01?
// Might indicate there a split here where map0_s01 included another .c file?
#ifdef MAP0_S01
q19_12 Model_AnimDurationGet(s_Model* model) // 0x800D7E88
{
    return Anim_DurationGet(model, func_80044918(&model->anim));
}
#endif

bool sharedFunc_800D7EBC_0_s01(s_SubCharacter* airScreamer)
{
    s32                temp_v0;
    s32                temp_v1_2;
    s32                animStatus;
    VECTOR3*           vec;
    GsCOORDINATE2*     coords;
    s_SubCharacter_44* temp_s1;

    animStatus = airScreamer->model.anim.status;
    temp_s1    = &airScreamer->field_44;

    if (!Chara_HasFlag(&g_SysWork.playerWork.player, CharaFlag_Unk4) &&
        g_SysWork.npcIdxs[0] == NO_VALUE && g_SysWork.npcIdxs[1] == NO_VALUE &&
        airScreamer->model.controlState != AirScreamerControl_12 && airScreamer->model.controlState != AirScreamerControl_25 &&
        airScreamer->model.controlState != AirScreamerControl_40 && airScreamer->model.controlState != AirScreamerControl_49)
    {
        airScreamer->field_44.field_0 = 0;
    }
    else
    {
        vec    = sharedData_800E21D0_0_s01.matrices_C;
        coords = sharedData_800E21D0_0_s01.coords_8;

        switch (animStatus)
        {
            case ANIM_STATUS(AirScreamerAnim_HoverBiteAttack, true):
            case ANIM_STATUS(AirScreamerAnim_StandBiteAttack, true):
                if (temp_s1->field_0 == 0)
                {
                    if (animStatus == ANIM_STATUS(AirScreamerAnim_StandBiteAttack, true))
                    {
                        temp_s1->field_2 = 40;
                    }
                    else
                    {
                        temp_s1->field_2 = 41;
                    }

                    temp_s1->field_0 = 1;
                }

                func_800805BC(&vec[0], sharedData_800CAA98_0_s01.ptr_D48[1], &coords[sharedData_800CAA98_0_s01.ptr_D48[1]->pad], 2);

                temp_v0           = func_80080478(&vec[0], &vec[1]);
                temp_v1_2         = temp_v0 >> 16;
                temp_s1->field_18 = *vec;
                temp_s1->field_C  = temp_v0;
                temp_s1->field_E  = temp_v1_2;

                if (func_8008A3E0(airScreamer) != 0)
                {
                    airScreamerProps.flags |= AirScreamerFlag_0;
                }
                break;

            case ANIM_STATUS(AirScreamerAnim_GlideScratchAttack, true):
                if (temp_s1->field_0 == 0)
                {
                    temp_s1->field_2 = 42;
                    temp_s1->field_0 = 1;
                }

                func_800805BC(vec, sharedData_800CAA98_0_s01.ptr_D48[2], &coords[sharedData_800CAA98_0_s01.ptr_D48[2]->pad], 2);
                func_800805BC(&vec[2], sharedData_800CAA98_0_s01.ptr_D48[3], &coords[sharedData_800CAA98_0_s01.ptr_D48[3]->pad], 2);

                vec[0].vx = (vec[0].vx + vec[2].vx) / 2;
                vec[0].vy = (vec[0].vy + vec[2].vy) / 2;
                vec[0].vz = (vec[0].vz + vec[2].vz) / 2;
                vec[1].vx = (vec[1].vx + vec[3].vx) / 2;
                vec[1].vy = (vec[1].vy + vec[3].vy) / 2;
                vec[1].vz = (vec[1].vz + vec[3].vz) / 2;

                temp_v0   = func_80080478(vec, &vec[1]);
                temp_v1_2 = temp_v0 >> 16;

                temp_s1->field_18 = sharedData_800E21D0_0_s01.matrices_C[0];
                temp_s1->field_C  = temp_v0;
                temp_s1->field_E  = temp_v1_2;

                if (func_8008A3E0(airScreamer) != 0)
                {
                    airScreamerProps.flags |= AirScreamerFlag_1;
                }
                break;
        }
    }

    return true;
}

bool sharedFunc_800D81B0_0_s01(s_SubCharacter* airScreamer)
{
    sharedFunc_800D82B8_0_s01(airScreamer);
    return true;
}

void sharedFunc_800D81D0_0_s01(s_SubCharacter* airScreamer)
{
    sharedData_800E21D0_0_s01.box          = airScreamer->collision.box;
    sharedData_800E21D0_0_s01.cylinder     = airScreamer->collision.cylinder;
    sharedData_800E21D0_0_s01.shapeOffsets = airScreamer->collision.shapeOffsets;
}

void sharedFunc_800D8244_0_s01(s_SubCharacter* airScreamer)
{
    airScreamer->collision.box          = sharedData_800E21D0_0_s01.box;
    airScreamer->collision.cylinder     = sharedData_800E21D0_0_s01.cylinder;
    airScreamer->collision.shapeOffsets = sharedData_800E21D0_0_s01.shapeOffsets;
}
void sharedFunc_800D82B8_0_s01(s_SubCharacter* airScreamer)
{
    s32            sp10;
    s32            idx2;
    s32            temp_a0_2;
    s32            offsetY;
    s32            idx0;
    s32            height;
    s32            i;
    s32            top;
    s32            idx1;
    q19_12         var_t1;
    s16            bottom0;
    q3_12          radius1;
    s16            bottom1;
    q3_12          radius0;
    s32            temp;
    s32            temp2;
    q19_12         posX;
    q19_12         posY;
    q19_12         posZ;
    GsCOORDINATE2* coords;
    SVECTOR*       temp_a0;
    SVECTOR*       temp_s5;
    VECTOR3*       var_t0_2;

    sp10 = sharedFunc_800D4A80_0_s01(airScreamer);
    if (sp10 != 0)
    {
        idx0 = 0;
        idx1 = 1;

        temp_s5 = sharedData_800CAA98_0_s01.ptr_D48[4];
        coords  = sharedData_800E21D0_0_s01.coords_8;

        temp_a0  = &temp_s5[1];
        height   = temp_a0->pad;
        idx2     = temp_s5->pad;
        var_t0_2 = &sharedData_800E21D0_0_s01.matrices_C;

        for (i = 1; height != NO_VALUE; i++)
        {
            if (height != idx2)
            {
                func_800805BC(&var_t0_2[idx0], &temp_s5[idx0], &coords[idx2], idx1 - idx0);
                idx2 = height;
                idx0 = idx1;
            }

            idx1++;
            height = temp_s5[idx1].pad;
        }

        func_800805BC(&var_t0_2[idx0], &temp_s5[idx0], &coords[idx2], idx1 - idx0);

        var_t1 = 0;
        height = INT_MAX + 1;
        top    = INT_MAX;

        posX = airScreamer->position.vx;
        posY = airScreamer->position.vy;
        posZ = airScreamer->position.vz;

        for (i = i - 1; i >= 0; i--)
        {
            temp  = var_t0_2[i].vx - posX;
            idx0  = var_t0_2[i].vy - posY;
            temp2 = var_t0_2[i].vz - posZ;

            temp_a0_2 = Q12_SQUARE_PRECISE(temp) +
                        Q12_SQUARE_PRECISE(temp2);
            if (var_t1 < temp_a0_2)
            {
                var_t1 = temp_a0_2;
            }

            if (height < idx0)
            {
                height = idx0;
            }

            if (idx0 < top)
            {
                top = idx0;
            }
        }

        var_t1  = SquareRoot12(var_t1) >> 1;
        offsetY = (height + top) / 2;

        airScreamer->collision.cylinder.field_2 = var_t1;

        height += (height - offsetY) >> 2; // `/ 4`.
        top    += (top    - offsetY) >> 2; // `/ 4`.

        airScreamer->collision.box.offsetY = offsetY;
        airScreamer->collision.box.field_8 = offsetY;

        airScreamer->collision.box.height = height;
        airScreamer->collision.box.top    = top;

        bottom0 = sharedData_800CAA98_0_s01.field_D70[sp10][0];
        radius1 = sharedData_800CAA98_0_s01.field_D70[sp10][1];

        airScreamer->collision.state           = CharaCollisionState_Npc;
        airScreamer->collision.box.bottom      = bottom0;
        airScreamer->collision.cylinder.radius = radius1;
    }
    else
    {
        bottom1 = sharedData_800CAA98_0_s01.field_D70[0][0];
        radius0 = sharedData_800CAA98_0_s01.field_D70[0][1];

        airScreamer->collision.cylinder.radius  = radius0;
        airScreamer->collision.cylinder.field_2 = Q12(0.0f);

        airScreamer->collision.box.height  = Q12(0.0f);
        airScreamer->collision.box.top     = Q12(0.0f);
        airScreamer->collision.box.offsetY = Q12(0.0f);
        airScreamer->collision.box.field_8 = Q12(0.0f);
        airScreamer->collision.box.bottom  = bottom1;
        airScreamer->collision.state       = CharaCollisionState_Ignore;
    }

    if (airScreamerProps.field_E8_0 == 3)
    {
        airScreamer->collision.box.bottom = Q12_MULT_PRECISE(airScreamer->collision.box.bottom, Q12(1.125f));
        airScreamer->collision.cylinder.field_2 = Q12_MULT_PRECISE(airScreamer->collision.cylinder.field_2, Q12(1.125f));
    }

    if (airScreamerProps.flags & AirScreamerFlag_12)
    {
        airScreamer->collision.box.bottom = Q12_MULT_PRECISE(airScreamer->collision.box.bottom, Q12(0.875f));
        airScreamer->collision.cylinder.field_2 = Q12_MULT_PRECISE(airScreamer->collision.cylinder.field_2, Q12(0.875f));
    }

    airScreamer->collision.shapeOffsets.box.vx      = Q12(0.0f);
    airScreamer->collision.shapeOffsets.box.vz      = Q12(0.0f);
    airScreamer->collision.shapeOffsets.cylinder.vx = Q12(0.0f);
    airScreamer->collision.shapeOffsets.cylinder.vz = Q12(0.0f);
}

void sharedFunc_800D8714_0_s01(s_SubCharacter* airScreamer, q19_12 moveSpeed, q19_12 headingAngle)
{
    q19_12           adjHeadingAngle;
    q19_12           cylinderOffsetDist;
    q19_12           moveDistBase;
    s32              idx;
    q19_12           cylinderOffsetX;
    q19_12           cylinderOffsetZ;
    s_func_800D2E04* base;

    adjHeadingAngle                   = headingAngle;
    idx                               = sharedFunc_800D4A80_0_s01(airScreamer);
    base                              = &sharedData_800CAA98_0_s01;
    moveDistBase                      = base->field_D5C[idx][1];
    airScreamer->collision.box.bottom = base->field_D5C[idx][0];

    if (moveSpeed == Q12(0.0f))
    {
        airScreamer->collision.shapeOffsets.cylinder.vx = Q12(0.0f);
        airScreamer->collision.shapeOffsets.cylinder.vz = Q12(0.0f);
        airScreamer->collision.cylinder.radius          = moveDistBase;
        return;
    }

    // Compute cylinder offset distance.
    cylinderOffsetDist = moveDistBase - Q12(0.4f);
    if (cylinderOffsetDist < Q12(0.0f))
    {
        cylinderOffsetDist = Q12(0.0f);
    }

    // Reverse heading angle if moving backward.
    if (moveSpeed < Q12(0.0f))
    {
        adjHeadingAngle ^= Q12_ANGLE(180.0f);
    }

    cylinderOffsetX = Q12_MULT_PRECISE(cylinderOffsetDist, Math_Sin(adjHeadingAngle));
    cylinderOffsetZ = Q12_MULT_PRECISE(cylinderOffsetDist, Math_Cos(adjHeadingAngle));

    airScreamer->collision.shapeOffsets.cylinder.vx = cylinderOffsetX;
    airScreamer->collision.shapeOffsets.cylinder.vz = cylinderOffsetZ;
    airScreamer->collision.cylinder.radius          = Q12(0.4f);
}

void sharedFunc_800D87FC_0_s01(s_SubCharacter* airScreamer) {}

void sharedFunc_800D8804_0_s01(void)
{
    u8 unknown[16];
}

#undef airScreamerProps
