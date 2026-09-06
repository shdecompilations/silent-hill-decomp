#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "main/rng.h"
#include "maps/map7/map7_s03.h"
#include "maps/characters/incubus.h"
#include "maps/unk_m3s03.h"

#define incubusProps      incubus->properties.incubus
#define localIncubusProps localIncubus->properties.incubus

bool func_800DD964(void) // 0x800DD964
{
    u8 isPlayerDead;

    Player_DisableDamage(&isPlayerDead, false);
    return isPlayerDead;
}

void func_800DD98C(bool disableDamage) // 0x800DD98C
{
    u8 isPlayerDead;

    Player_DisableDamage(&isPlayerDead, disableDamage);
}

void func_800DD9B0(s_SubCharacter* incubus) // 0x800DD9B0
{
    if (incubus->model.controlState != IncubusControl_None)
    {
        incubus->model.controlState = IncubusControl_2;
        incubus->model.stateStep    = 0;
    }
    else
    {
        incubus->model.stateStep = 2;
    }
}

void func_800DD9D4(s_SubCharacter* incubus) // 0x800DD9D4
{
    if (incubus->model.controlState != IncubusControl_None)
    {
        incubus->model.controlState = IncubusControl_10;
        incubus->model.stateStep    = 0;
    }
    else
    {
        incubus->model.stateStep = 10;
    }
}

void func_800DD9F8(s_SubCharacter* incubus) // 0x800DD9F8
{
    if (incubus->model.controlState != IncubusControl_None)
    {
        incubus->model.controlState = IncubusControl_3;
        incubus->model.stateStep    = 0;
    }
    else
    {
        incubus->model.stateStep = 3;
    }
}

bool Incubus_Init(s_SubCharacter* incubus, GsCOORDINATE2* boneCoords) // 0x800DDA1C
{
    u8              activeStateStep;
    s_SubCharacter* localIncubus; // TODO: Not sure why this is needed here, could be an inline in this func and others.

    incubus->model.anim.alpha = Q12(0.0f);
    localIncubus              = incubus;

    // Set starting health.
    if (g_SavegamePtr->gameDifficulty == GameDifficulty_Easy)
    {
        incubus->health = Q12(30000.0f);
    }
    else
    {
        incubus->health = Q12(40000.0f);
    }

    incubus->moveSpeed                          = Q12(0.0f);
    incubus->headingAngle                       = incubus->rotation.vy;
    incubus->collision.cylinder.radius          = Q12(0.3f);
    incubus->collision.state                    = CharaCollisionState_4;
    incubus->collision.shapeOffsets.cylinder.vx = Q12(0.0f);
    incubus->collision.shapeOffsets.cylinder.vz = Q12(0.0f);
    incubus->flags                             |= CharaFlag_Hit;

    localIncubusProps.someState_F0 = 0;
    localIncubusProps.field_EC = 0;

    if (g_SavegamePtr->gameDifficulty == GameDifficulty_Hard)
    {
        localIncubusProps.bossFightTimer_F4 = Q12(300.0f);
    }
    else
    {
        localIncubusProps.bossFightTimer_F4 = Q12(30.0f);
    }

    activeStateStep = incubus->model.stateStep;
    if (activeStateStep != IncubusControl_None)
    {
        incubus->model.stateStep    = 0;
        incubus->model.controlState = activeStateStep;
    }
    else
    {
        incubus->model.controlState = IncubusControl_1;
        incubus->model.stateStep    = 0;
    }

    Chara_AnimSet(incubus, ANIM_STATUS(IncubusAnim_3, false), 338);
    ModelAnim_AnimInfoSet(&incubus->model.anim, INCUBUS_ANIM_INFOS);

    Chara_DamageClear(incubus);
    incubus->flags |= CharaFlag_NoRadioStatic;

    return true;
}

void func_800DDB3C(s_SubCharacter* incubus, GsCOORDINATE2* boneCoords)
{
    func_800DD62C(&incubus->position, incubus, boneCoords);
}

void func_800DDB68(s_SubCharacter* incubus, s32 soundIdx) // 0x800DDB68
{
    Sfx_WithFlagsPlay(D_800EC8C8[soundIdx].id, &incubus->position, D_800EC8C8[soundIdx].volume_2.val16, SfxFlag_None);
}

s32 func_800DDBA4(s32 idx) // 0x800DDBA4
{
    return D_800EC8FC[idx];
}

void func_800DDBBC(s_SubCharacter* incubus) // 0x800DDBBC
{
    q19_12 newHealth;

    if (incubusProps.someState_F0 == 0)
    {
        if (incubusProps.bossFightTimer_F4 < Q12(0.0f))
        {
            incubus->health = Q12(0.0f);
            incubus->damage.amount = 1;
        }

        if (!Inventory_GunsAvailableCheck(false))
        {
            incubusProps.bossFightTimer_F4 -= g_DeltaTime;
        }

        if (!(incubus->flags & CharaFlag_Hit))
        {
            incubus->damage.amount *= 10;
        }

        if (incubus->damage.amount > Q12(0.0f))
        {
            newHealth = incubus->health - incubus->damage.amount;
            if (newHealth < Q12(0.0f))
            {
                newHealth = Q12(0.0f);
            }
            incubus->health = newHealth;

            if (newHealth < Q12(20.0f) && func_800DD964() == 0)
            {
                incubus->health             = Q12(0.0f);
                incubus->model.controlState = IncubusControl_12;
                incubus->model.stateStep    = 0;
                incubusProps.someState_F0++;
            }
        }
    }

    Chara_DamageClear(incubus);
}

void func_800DDCC4(s_SubCharacter* incubus) // 0x800DDCC4
{
    s16 temp_v0;
    s16 var_v0;
    s16 var_v0_2;
    s32 tmp;

    #define playerChara g_SysWork.playerWork.player

    temp_v0 = Math_AngleNormalizeSigned(ratan2(playerChara.position.vx - incubus->position.vx,
                                               playerChara.position.vz - incubus->position.vz) -
                            incubus->rotation.vy);
    var_v0  = ABS(temp_v0);

    if (var_v0 > Q12_ANGLE(10.0f))
    {
        var_v0_2 = Q12_ANGLE(120.0f);
        tmp      = Q12_MULT_PRECISE(g_DeltaTime, var_v0_2);
        if (temp_v0 > 0)
        {
            incubus->rotation.vy += tmp;
        }
        else
        {
            incubus->rotation.vy -= tmp;
        }
    }

    #undef playerChara
}

void func_800DDDB0(s_SubCharacter* incubus) // 0x800DDDB0
{
    if (incubus->model.stateStep == 0)
    {
        incubus->model.anim.status = ANIM_STATUS(IncubusAnim_1, false);
        incubus->model.stateStep++;
    }
}

void func_800DDDD8(s_SubCharacter* incubus) // 0x800DDDD8
{
    s_SubCharacter* localIncubus;

    localIncubus = incubus;

    if (incubus->model.stateStep == 0)
    {
        incubus->model.anim.status = ANIM_STATUS(IncubusAnim_2, false);
        incubusProps.timer_E8      = Q12(0.0f);
        incubus->model.stateStep++;
        return;
    }

    switch (incubus->model.stateStep)
    {
        case 1:
            if (incubusProps.timer_E8 >= Q12(1.5f))
            {
                incubus->model.stateStep = 2;
            }
            break;

        case 2:
            if (incubusProps.timer_E8 >= Q12(2.5f))
            {
                incubus->model.stateStep = 3;
            }
            break;

        case 3:
            if (incubusProps.timer_E8 >= Q12(3.5f))
            {
                incubus->model.stateStep = 4;
            }
            break;

        case 4:
            if (incubusProps.timer_E8 >= Q12(4.5f))
            {
                incubus->model.stateStep = 5;
            }
            break;
    }

    if (incubus->model.anim.status == ANIM_STATUS(4, false))
    {
        incubus->model.controlState = IncubusControl_5;
        incubus->model.stateStep    = 0;
    }

    localIncubusProps.timer_E8 += g_DeltaTime;
}

void func_800DDEEC(s_SubCharacter* incubus) // 0x800DDEEC
{
    if (incubus->model.stateStep == 0)
    {
        incubus->model.anim.status = ANIM_STATUS(IncubusAnim_4, false);
        incubus->model.stateStep++;
    }
}

void func_800DDF14(s_SubCharacter* incubus) // 0x800DDF14
{
    if (incubus->model.stateStep == 0)
    {
        incubus->model.anim.status = ANIM_STATUS(IncubusAnim_4, false);
        incubus->model.stateStep++;
    }
}

void func_800DDF3C(s_SubCharacter* incubus, GsCOORDINATE2* boneCoords) // 0x800DDF3C
{
    MATRIX          transformMat;
    VECTOR3         pos;
    q3_12           angleDeltaToPlayer;
    s_SubCharacter* localIncubus;

    #define playerChara g_SysWork.playerWork.player

    localIncubus = incubus;

    if (incubus->model.stateStep == 0)
    {
        incubus->model.stateStep++;
        incubusProps.timer_E8 = Q12(1.5f);
        return;
    }

    if (boneCoords == NULL)
    {
        func_800DDCC4(incubus);
    }

    switch (incubus->model.stateStep)
    {
        case 1:
            if (boneCoords)
            {
                Vw_CoordHierarchyMatrixCompute(&boneCoords[IncubusBone_2], &transformMat);
                pos.vx = Q8_TO_Q12(transformMat.t[0]);
                pos.vy = Q8_TO_Q12(transformMat.t[1]);
                pos.vz = Q8_TO_Q12(transformMat.t[2]);

                if (!(Rng_Rand16() & 0xF0))
                {
                    func_800DD240(&pos);
                }

                if (localIncubusProps.timer_E8 < Q12(0.0f))
                {
                    localIncubusProps.timer_E8 = Q12(2.5f);
                    incubus->model.stateStep++;
                }

                localIncubusProps.timer_E8 -= g_DeltaTime;
            }
            break;

        case 2:
            if (boneCoords)
            {
                Vw_CoordHierarchyMatrixCompute(&boneCoords[IncubusBone_2], &transformMat);
                pos.vx = Q8_TO_Q12(transformMat.t[0]);
                pos.vy = Q8_TO_Q12(transformMat.t[1]);
                pos.vz = Q8_TO_Q12(transformMat.t[2]);

                if (!(Rng_Rand16() & 0xF0))
                {
                    func_800DD240(&pos);
                }

                if (!(Rng_Rand16() & 0x10))
                {
                    func_800DD260(&pos, &pos);
                }

                if (!(Rng_Rand16() & 0xFC))
                {
                    func_800DD464(&pos);
                }

                angleDeltaToPlayer = abs(Math_AngleNormalizeSigned(Math_AngleBetweenPositionsGet(incubus->position, playerChara.position) -
                                                       incubus->rotation.vy));

                if (localIncubusProps.timer_E8 < Q12(0.0f) && angleDeltaToPlayer < Q12_ANGLE(12.0f))
                {
                    localIncubusProps.timer_E8 = Q12(0.3f);
                    incubus->model.stateStep++;
                    break;
                }

                localIncubusProps.timer_E8 -= g_DeltaTime;
            }
            break;

        case 3:
            if (boneCoords)
            {
                Vw_CoordHierarchyMatrixCompute(&boneCoords[IncubusBone_2], &transformMat);
                pos.vx = Q8_TO_Q12(transformMat.t[0]);
                pos.vy = Q8_TO_Q12(transformMat.t[1]);
                pos.vz = Q8_TO_Q12(transformMat.t[2]);

                if (!(Rng_Rand16() & 0x10))
                {
                    func_800DD3D4(&pos, 0, 0, 0);
                }

                if (!(Rng_Rand16() & 0xFC))
                {
                    func_800DD464(&pos);
                    func_800DD240(&pos);
                }

                if (localIncubusProps.timer_E8 < Q12(0.0f))
                {
                    incubus->model.stateStep++;
                    localIncubusProps.timer_E8 = Q12(2.0f);
                }

                localIncubusProps.timer_E8 -= g_DeltaTime;
            }
            break;

        case 4:
            if (boneCoords)
            {
                Vw_CoordHierarchyMatrixCompute(&boneCoords[IncubusBone_2], &transformMat);
                pos.vx = Q8_TO_Q12(transformMat.t[0]);
                pos.vy = Q8_TO_Q12(transformMat.t[1]);
                pos.vz = Q8_TO_Q12(transformMat.t[2]);

                if (!(Rng_Rand16() & 0x50))
                {
                    func_800DD3D4(&pos, 0, 0, 0);
                }

                if (!(Rng_Rand16() & 0xFC))
                {
                    func_800DD464(&pos);
                }

                if (localIncubusProps.timer_E8 < Q12(0.0f))
                {
                    incubus->model.stateStep++;
                }

                localIncubusProps.timer_E8 -= g_DeltaTime;
            }
            break;

        case 5:
            incubus->model.controlState = IncubusControl_11;
            incubus->model.stateStep    = 0;
            break;
    }

    #undef playerChara
}

void func_800DE2A4(s_SubCharacter* incubus, GsCOORDINATE2* boneCoords) // 0x800DE2A4
{
    MATRIX          transformMat;
    VECTOR3         pos;
    q3_12           angleDeltaToPlayer;
    s_SubCharacter* localIncubus;

    #define playerChara g_SysWork.playerWork.player

    localIncubus = incubus;

    if (incubus->model.stateStep == 0)
    {
        incubusProps.timer_E8 = Q12(1.5f);
        incubus->flags       &= ~CharaFlag_Hit;
        incubus->model.stateStep++;
        return;
    }

    if (boneCoords == NULL)
    {
        func_800DDCC4(incubus);
    }

    switch (incubus->model.stateStep)
    {
        case 1:
            if (boneCoords)
            {
                Vw_CoordHierarchyMatrixCompute(&boneCoords[IncubusBone_2], &transformMat);
                pos.vx = Q8_TO_Q12(transformMat.t[0]);
                pos.vy = Q8_TO_Q12(transformMat.t[1]);
                pos.vz = Q8_TO_Q12(transformMat.t[2]);

                if (!(Rng_Rand16() & 0xF0))
                {
                    func_800DD240(&pos);
                }

                if (localIncubusProps.timer_E8 < Q12(0.0f))
                {
                    if (g_SavegamePtr->gameDifficulty == GameDifficulty_Hard)
                    {
                        localIncubusProps.timer_E8 = Q12(1.0f);
                    }
                    else
                    {
                        localIncubusProps.timer_E8 = Q12(2.5f);
                    }

                    incubus->model.stateStep++;
                }

                localIncubusProps.timer_E8 -= g_DeltaTime;
            }
            break;

        case 2:
            if (boneCoords)
            {
                Vw_CoordHierarchyMatrixCompute(&boneCoords[IncubusBone_2], &transformMat);
                pos.vx = Q8_TO_Q12(transformMat.t[0]);
                pos.vy = Q8_TO_Q12(transformMat.t[1]);
                pos.vz = Q8_TO_Q12(transformMat.t[2]);

                if (!(Rng_Rand16() & 0xF0))
                {
                    func_800DD240(&pos);
                }

                if (!(Rng_Rand16() & 0x10))
                {
                    func_800DD260(&pos, &pos);
                    func_800DAB18(&pos, 0, 0, Q12(4.5f));
                }

                if (!(Rng_Rand16() & 0xFC))
                {
                    func_800DD464(&pos);
                }

                angleDeltaToPlayer = abs(Math_AngleNormalizeSigned(Math_AngleBetweenPositionsGet(incubus->position, playerChara.position) -
                                                       incubus->rotation.vy));

                if (localIncubusProps.timer_E8 < Q12(0.0f) && angleDeltaToPlayer < Q12_ANGLE(12.0f))
                {
                    localIncubusProps.timer_E8 = Q12(0.3f);
                    incubus->model.stateStep++;
                    break;
                }

                localIncubusProps.timer_E8 -= g_DeltaTime;
            }
            break;

        case 3:
            if (boneCoords)
            {
                Vw_CoordHierarchyMatrixCompute(&boneCoords[IncubusBone_2], &transformMat);
                pos.vx = Q8_TO_Q12(transformMat.t[0]);
                pos.vy = Q8_TO_Q12(transformMat.t[1]);
                pos.vz = Q8_TO_Q12(transformMat.t[2]);

                if (!(Rng_Rand16() & 0x10))
                {
                    func_800DD3D4(&pos, D_800F48A8.velocityX_3C, 0, D_800F48A8.velocityZ_40);
                }

                if (!(Rng_Rand16() & 0xFC))
                {
                    func_800DD464(&pos);
                    func_800DD240(&pos);
                }

                if (localIncubusProps.timer_E8 < Q12(0.0f))
                {
                    incubus->model.stateStep++;

                    if (g_SavegamePtr->gameDifficulty == GameDifficulty_Hard)
                    {
                        localIncubusProps.timer_E8 = Q12(3.0f);
                    }
                    else
                    {
                        localIncubusProps.timer_E8 = Q12(2.0f);
                    }
                }

                localIncubusProps.timer_E8 -= g_DeltaTime;
            }
            break;

        case 4:
            if (boneCoords)
            {
                Vw_CoordHierarchyMatrixCompute(&boneCoords[IncubusBone_2], &transformMat);
                pos.vx = Q8_TO_Q12(transformMat.t[0]);
                pos.vy = Q8_TO_Q12(transformMat.t[1]);
                pos.vz = Q8_TO_Q12(transformMat.t[2]);

                if (!(Rng_Rand16() & 0x51))
                {
                    func_800DD3D4(&pos, D_800F48A8.velocityX_3C, Q12(0.0f), D_800F48A8.velocityZ_40);
                }

                if (!(Rng_Rand16() & 0xFC))
                {
                    func_800DD464(&pos);
                }

                if (localIncubusProps.timer_E8 < Q12(0.0f))
                {
                    incubus->model.stateStep++;
                }

                localIncubusProps.timer_E8 -= g_DeltaTime;
            }
            break;

        case 5:
            incubus->model.controlState = IncubusControl_11;
            incubus->model.stateStep    = 0;
            incubus->flags             |= CharaFlag_Hit;
            break;
    }

    #undef playerChara
}

void func_800DE68C(s_SubCharacter* incubus, GsCOORDINATE2* boneCoords) // 0x800DE68C
{
    MATRIX          transformMat;
    VECTOR3         pos;
    VECTOR3         playerPos;
    q3_12           angleDeltaToPlayer;
    s_SubCharacter* localIncubus;

    #define playerChara g_SysWork.playerWork.player

    localIncubus = incubus;

    if (incubus->model.stateStep == 0)
    {
        incubus->model.stateStep++;
        incubusProps.timer_E8 = Q12(1.5f);
        return;
    }

    if (boneCoords == NULL)
    {
        func_800DDCC4(incubus);
    }

    switch (incubus->model.stateStep)
    {
        case 1:
            if (boneCoords)
            {
                Vw_CoordHierarchyMatrixCompute(&boneCoords[IncubusBone_2], &transformMat);
                pos.vx = Q8_TO_Q12(transformMat.t[0]);
                pos.vy = Q8_TO_Q12(transformMat.t[1]);
                pos.vz = Q8_TO_Q12(transformMat.t[2]);

                if (!(Rng_Rand16() & 0xF0))
                {
                    func_800DD240(&pos);
                }

                if (localIncubusProps.timer_E8 < Q12(0.0f))
                {
                    localIncubusProps.timer_E8 = Q12(2.5f);
                    incubus->model.stateStep++;
                }

                localIncubusProps.timer_E8 -= g_DeltaTime;
            }
            break;

        case 2:
            if (boneCoords)
            {
                Vw_CoordHierarchyMatrixCompute(&boneCoords[IncubusBone_2], &transformMat);
                pos.vx = Q8_TO_Q12(transformMat.t[0]);
                pos.vy = Q8_TO_Q12(transformMat.t[1]);
                pos.vz = Q8_TO_Q12(transformMat.t[2]);

                if (!(Rng_Rand16() & 0xF0))
                {
                    func_800DD240(&pos);
                }

                if (!(Rng_Rand16() & 0x10))
                {
                    func_800DD2C8(&pos, &pos);
                }

                if (!(Rng_Rand16() & 0xFC))
                {
                    func_800DD464(&pos);
                }

                angleDeltaToPlayer = abs(Math_AngleNormalizeSigned(Math_AngleBetweenPositionsGet(incubus->position, playerChara.position) -
                                                       incubus->rotation.vy));

                if (localIncubusProps.timer_E8 < Q12(0.0f) && angleDeltaToPlayer < Q12_ANGLE(12.0f))
                {
                    localIncubusProps.timer_E8 = Q12(0.3f);
                    incubus->model.stateStep++;
                    break;
                }
                localIncubusProps.timer_E8 -= g_DeltaTime;
            }
            break;

        case 3:
            if (boneCoords)
            {
                s_D_800F48A8* ptr;
                Vw_CoordHierarchyMatrixCompute(&boneCoords[IncubusBone_2], &transformMat);
                pos.vx = Q8_TO_Q12(transformMat.t[0]);
                pos.vy = Q8_TO_Q12(transformMat.t[1]);
                pos.vz = Q8_TO_Q12(transformMat.t[2]);

                ptr          = &D_800F48A8;
                playerPos.vx = playerChara.position.vx + ptr->velocityX_3C;
                playerPos.vy = playerChara.position.vy;
                playerPos.vz = playerChara.position.vz + ptr->velocityZ_40;

                if (!(Rng_Rand16() & 0x10))
                {
                    func_800DD32C(&pos, &playerPos);
                }

                if (!(Rng_Rand16() & 0xFC))
                {
                    func_800DD464(&pos);
                    func_800DD240(&pos);
                }

                if (localIncubusProps.timer_E8 < Q12(0.0f))
                {
                    incubus->model.stateStep++;
                    localIncubusProps.timer_E8 = Q12(2.0f);
                }

                localIncubusProps.timer_E8 -= g_DeltaTime;
            }
            break;

        case 4:
            if (boneCoords)
            {
                s_D_800F48A8* ptr;
                Vw_CoordHierarchyMatrixCompute(&boneCoords[IncubusBone_2], &transformMat);
                pos.vx = Q8_TO_Q12(transformMat.t[0]);
                pos.vy = Q8_TO_Q12(transformMat.t[1]);
                pos.vz = Q8_TO_Q12(transformMat.t[2]);

                ptr          = &D_800F48A8;
                playerPos.vx = playerChara.position.vx + ptr->velocityX_3C;
                playerPos.vy = playerChara.position.vy;
                playerPos.vz = playerChara.position.vz + ptr->velocityZ_40;

                if (!(Rng_Rand16() & 0x51))
                {
                    func_800DD32C(&pos, &playerPos);
                }

                if (!(Rng_Rand16() & 0xFC))
                {
                    func_800DD464(&pos);
                }

                if (localIncubusProps.timer_E8 < Q12(0.0f))
                {
                    incubus->model.stateStep++;
                }

                localIncubusProps.timer_E8 -= g_DeltaTime;
            }
            break;

        case 5:
            incubus->model.controlState = IncubusControl_11;
            incubus->model.stateStep    = 0;
            break;
    }

    #undef playerChara
}

void func_800DEA54(s_SubCharacter* incubus, GsCOORDINATE2* boneCoords) // 0x800DEA54
{
    func_800DDB3C(incubus, boneCoords);

    incubus->model.controlState           = IncubusControl_11;
    incubus->model.stateStep              = 0;
    incubus->properties.incubus.field_EC |= 1 << 2;
}

q19_12 func_800DEA90(void) // 0x800DEA90
{
    q19_12 result;

    if (g_SavegamePtr->gameDifficulty == GameDifficulty_Hard)
    {
        result = Q12(2.0f);
    }
    else
    {
        result = Q12(5.0f);
    }

    return result + Rng_GenerateInt(0, Q12(2.0f) - 1);
}

void func_800DEAF4(s_SubCharacter* incubus) // 0x800DEAF4
{
    if (incubus->model.stateStep == 0)
    {
        incubusProps.timer_E8 = func_800DEA90();
        incubus->model.stateStep++;
        return;
    }

    func_800DDCC4(incubus);

    // Handle state step.
    switch (incubus->model.stateStep)
    {
        case 1:
            if (incubusProps.timer_E8 <= Q12(0.0f))
            {
                incubus->model.stateStep = 2;
            }
            break;

        case 2:
            incubus->model.controlState = IncubusControl_7;
            incubus->model.stateStep    = 0;
            break;
    }

    incubusProps.timer_E8 -= g_DeltaTime;
}

void func_800DEBA8(s_SubCharacter* incubus) // 0x800DEBA8
{
    s_SubCharacter* localIncubus = incubus;

    if (incubus->model.stateStep == 0)
    {
        if (incubus->model.anim.status != ANIM_STATUS(IncubusAnim_3, true))
        {
            incubus->model.anim.status = ANIM_STATUS(IncubusAnim_3, false);
        }

        incubusProps.timer_E8 = Q12(0.5f);
        incubus->model.stateStep++;
    }
    else
    {
        if (incubusProps.timer_E8 < Q12(0.0f))
        {
            Savegame_EventFlagSet(EventFlag_578);
            incubus->model.controlState = IncubusControl_13;
            incubus->model.stateStep    = 0;
        }

        localIncubusProps.timer_E8 -= g_DeltaTime;
    }
}

void func_800DEC38(s_SubCharacter* incubus) // 0x800DEC38
{
    if (incubus->model.stateStep == 0)
    {
        if (incubus->model.anim.status != ANIM_STATUS(IncubusAnim_3, true))
        {
            incubus->model.anim.status = ANIM_STATUS(IncubusAnim_3, false);
        }

        incubus->model.stateStep++;
    }
}

void func_800DEC74(s_SubCharacter* incubus, GsCOORDINATE2* boneCoords) // 0x800DEC74
{
    switch (incubus->model.controlState)
    {
        case IncubusControl_1:
            break;

        case IncubusControl_2:
            func_800DDDB0(incubus);
            break;

        case IncubusControl_3:
            func_800DDDD8(incubus);
            break;

        case IncubusControl_4:
            func_800DDEEC(incubus);
            break;

        case IncubusControl_5:
            func_800DDF14(incubus);
            break;

        case IncubusControl_6:
            func_800DDF3C(incubus, 0);
            break;

        case IncubusControl_7:
            func_800DE2A4(incubus, 0);
            break;

        case IncubusControl_8:
            func_800DE68C(incubus, 0);
            break;

        case IncubusControl_11:
            func_800DEAF4(incubus);
            break;

        case IncubusControl_10:
            func_800DEA54(incubus, boneCoords);
            break;

        case IncubusControl_12:
            func_800DEBA8(incubus);
            break;

        case IncubusControl_13:
            func_800DEC38(incubus);
            break;
    }
}

void func_800DED68(s_SubCharacter* incubus, GsCOORDINATE2* boneCoords) // 0x800DED68
{
    MATRIX transformMat;

    func_800DB608();

    switch (incubus->model.controlState)
    {
        case IncubusControl_6:
            func_800DDF3C(incubus, boneCoords);
            break;

        case IncubusControl_7:
            func_800DE2A4(incubus, boneCoords);
            break;

        case IncubusControl_8:
            func_800DE68C(incubus, boneCoords);
            break;
    }

    if (incubus->model.anim.status != ANIM_STATUS(IncubusAnim_3, true))
    {
        Vw_CoordHierarchyMatrixCompute(&boneCoords[IncubusBone_2], &transformMat);
        Vc_LookAtPositionYSet(Q12_MULT_FLOAT_PRECISE(Q8_TO_Q12(transformMat.t[1]), 0.65f));
    }
}

void func_800DEE44(s_SubCharacter* incubus) // 0x800DEE44
{
    s_CollisionResult collResult;

    incubus->fallSpeed += g_GravitySpeed;
    Chara_MovementUpdate0(incubus, &collResult);
    incubus->rotation.vy = Math_AngleNormalizeSigned(incubus->rotation.vy);
}

void func_800DEE90(s_SubCharacter* incubus, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords) // 0x800DEE90
{
    s32 prevSfxIdx;
    s32 sfxIdx;

    #define playbackFunc INCUBUS_ANIM_INFOS[incubus->model.anim.status].playbackFunc

    Math_MatrixTransform(&incubus->position, &incubus->rotation, boneCoords);

    prevSfxIdx = func_800DDBA4(FP_FROM(incubus->model.anim.time, Q12_SHIFT));
    if (incubus->model.anim.status != ANIM_STATUS(IncubusAnim_Still, false))
    {
        playbackFunc(&incubus->model, anmHdr, boneCoords, &playbackFunc);
    }

    sfxIdx = func_800DDBA4(FP_FROM(incubus->model.anim.time, Q12_SHIFT));
    if (sfxIdx != 13 && sfxIdx != prevSfxIdx)
    {
        func_800DDB68(incubus, sfxIdx);
    }

    #undef playbackFunc
}

void func_800DEF50(s_SubCharacter* incubus, GsCOORDINATE2* boneCoords) // 0x800DEF50
{
    MATRIX transformMat;
    q19_12 offsetY;
    q19_12 offsetX;
    q19_12 offsetZ;

    Vw_CoordHierarchyMatrixCompute(&boneCoords[IncubusBone_2], &transformMat);
    offsetX = Q8_TO_Q12(transformMat.t[0]) - incubus->position.vx;
    offsetY = Q8_TO_Q12(transformMat.t[1]) - incubus->position.vy;
    offsetZ = Q8_TO_Q12(transformMat.t[2]) - incubus->position.vz;

    incubus->collision.cylinder.radius  = Q12(0.5f);
    incubus->collision.cylinder.field_2 = Q12(0.5f);
    incubus->collision.box.top          = offsetY - Q12(0.25f);
    incubus->collision.box.bottom       = offsetY;
    incubus->collision.box.height       = offsetY + Q12(0.25f);
    incubus->collision.box.offsetY      = offsetY;

    sharedFunc_800CD920_3_s03(incubus, offsetX, offsetZ);

    incubus->collision.shapeOffsets.box.vx = incubus->collision.shapeOffsets.cylinder.vx;
    incubus->collision.shapeOffsets.box.vz = incubus->collision.shapeOffsets.cylinder.vz;
}

void func_800DEFE8(s_SubCharacter* incubus, GsCOORDINATE2* boneCoords) // 0x800DEFE8
{
    q19_12 posY;

    if (incubus->model.anim.flags & AnimFlag_Visible)
    {
        func_800DEF50(incubus, boneCoords);
        return;
    }

    posY = incubus->position.vy;

    incubus->collision.box.bottom  = posY;
    incubus->collision.box.height  = posY;
    incubus->collision.box.top     = posY - Q12(1.0f);
    incubus->collision.box.offsetY = posY - Q12(0.5f);
}

void func_800DF044(s_SubCharacter* incubus, GsCOORDINATE2* boneCoords) // 0x800DF044
{
    if (incubus->model.stateStep == 0)
    {
        func_800DEC74(incubus, boneCoords);
    }
}

void func_800DF074(s_SubCharacter* incubus) // 0x800DF074
{
    u8 controlState;

    controlState = incubus->model.controlState;
    if (controlState != IncubusControl_2 &&
        controlState != IncubusControl_4 &&
        controlState != IncubusControl_13 &&
        controlState != IncubusControl_3 &&
        controlState != IncubusControl_5 &&
        (incubus->properties.incubus.field_EC & (1 << 2)))
    {
        func_800DD6CC();
    }
}

void Incubus_Update(s_SubCharacter* incubus, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords) // 0x800DF0D8
{
    if ((incubus->model.controlState != IncubusControl_None ||
         Incubus_Init(incubus, boneCoords)) &&
        incubus->model.controlState != IncubusControl_1)
    {
        if (g_DeltaTime != Q12(0.0f))
        {
            func_800DDBBC(incubus);
            func_800DEC74(incubus, boneCoords);
            func_800DF044(incubus, boneCoords);
            func_800DEE44(incubus);
            func_800DEE90(incubus, anmHdr, boneCoords);
            func_800DEFE8(incubus, boneCoords);
            func_800DED68(incubus, boneCoords);
            func_800DF074(incubus);
            func_800DD98C(incubus->flags & CharaFlag_Unk2);
        }
        else
        {
            func_800DEE90(incubus, anmHdr, boneCoords);
            func_800DB608();
            func_800DF074(incubus);
        }
    }
}
