#include "inline_no_dmpsx.h"

#include <psyq/gtemac.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player/player.h"
#include "main/rng.h"
#include "maps/common/shared.h"
#include "maps/common/characters/bloodsucker.h"

#define bloodsuckerProps bloodsucker->properties.bloodsucker

void Bloodsucker_Update(s_SubCharacter* bloodsucker, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords)
{
    u32 animStatusDiv2;
    s32 stateStepMul2;

    // Initialize.
    if (bloodsucker->model.controlState == BloodsuckerControl_None)
    {
        Bloodsucker_Init(bloodsucker);
    }

    // Handle control state.
    switch (bloodsucker->model.controlState)
    {
        case BloodsuckerControl_1:
            Bloodsucker_Control_1(bloodsucker);
            break;

        case BloodsuckerControl_2:
            Bloodsucker_Control_2(bloodsucker);
            break;

        case BloodsuckerControl_3:
            Bloodsucker_Control_3(bloodsucker);
            break;

        case BloodsuckerControl_4:
            Bloodsucker_Control_4(bloodsucker);
            break;
    }

    sharedFunc_800D0F28_3_s03(bloodsucker, anmHdr, boneCoords);

    // Reset flags if ???
    if (g_SysWork.bgmStatusFlags & BgmStatusFlag_6)
    {
        bloodsuckerProps.flags &= ~BloodsuckerFlag_0;
        bloodsuckerProps.flags &= ~BloodsuckerFlag_1;
    }

    if (bloodsucker->model.stateStep != 0)
    {
        return;
    }

    if (!(bloodsuckerProps.flags & BloodsuckerFlag_0))
    {
        SD_Call(Sfx_Unk1525);
        bloodsuckerProps.flags |= BloodsuckerFlag_0;
    }

    Sfx_WithFalloffAndPitchPlay(Sfx_Unk1525, &bloodsucker->position, bloodsuckerProps.timer_EC >> 5, Q12(16.0f), 0);

    if (bloodsuckerProps.timer_EC < bloodsuckerProps.timer_F0)
    {
        bloodsuckerProps.timer_EC += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.5f);
        if (bloodsuckerProps.timer_EC > bloodsuckerProps.timer_F0)
        {
            bloodsuckerProps.timer_EC = bloodsuckerProps.timer_F0;
        }
    }
    else if (bloodsuckerProps.timer_EC > bloodsuckerProps.timer_F0)
    {
        bloodsuckerProps.timer_EC -= Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 0.5f);
        if (bloodsuckerProps.timer_EC < bloodsuckerProps.timer_F0)
        {
            bloodsuckerProps.timer_EC = bloodsuckerProps.timer_F0;
        }
    }

    animStatusDiv2 = bloodsucker->model.anim.status / 2;
    stateStepMul2  = bloodsucker->model.stateStep * 2;

    // SFX timer state handling. TODO: Inspect behavior in-game.
    if (animStatusDiv2 == ((stateStepMul2 + 23) / 2) || animStatusDiv2 == ((stateStepMul2 + 17) / 2))
    {
        if (!(bloodsuckerProps.flags & BloodsuckerFlag_1))
        {
            bloodsuckerProps.flags |= BloodsuckerFlag_1;
            SD_Call(Sfx_Unk1527);
        }

        bloodsuckerProps.timer_F4 += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 2.0f);
        if (bloodsuckerProps.timer_F4 > Q12(1.0f))
        {
            bloodsuckerProps.timer_F4 = Q12(1.0f);
        }

        Sfx_WithFalloffAndPitchPlay(Sfx_Unk1527, &bloodsucker->position, bloodsuckerProps.timer_F4 >> 5, Q12(16.0f), 0);
    }
    else if (bloodsuckerProps.flags & BloodsuckerFlag_1)
    {
        bloodsuckerProps.timer_F4 -= Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 2.0f);
        if (bloodsuckerProps.timer_F4 < Q12(0.0f))
        {
            bloodsuckerProps.timer_F4 = Q12(0.0f);
            bloodsuckerProps.flags &= ~BloodsuckerFlag_1;
            Sd_SfxStop(Sfx_Unk1527);
        }
    }
}

static inline void Bloodsucker_AnimUpdateFromStep(s_SubCharacter* chara)
{
    u32         stateStep;
    s_Savegame* save;

    switch (chara->model.stateStep)
    {
        case 17:
            chara->model.controlState     = 1;
            chara->model.stateStep = 0;
            Chara_AnimSet(chara, ANIM_STATUS(BloodsuckerAnim_7, true), 81);
            return;

#ifdef MAP7_S02 // MAP7_S02 skips rest of function.
    }
#else
        case 18:
            chara->health       = 1;
            chara->model.controlState = 2;
            break;

        case 19:
        case 20:
            chara->model.controlState = 2;
            break;

        default:
            return;
    }

    save = g_SavegamePtr; // TODO: Odd pointer copy, might be some inline flag check func?

    // Anim-related?
    stateStep                  = chara->model.stateStep - 18;
    chara->model.stateStep = stateStep;

    // TODO: `Savegame_EventFlagGet(EventFlag_250)`
    if (!(save->eventFlags[7] & (1 << 26)))
    {
        chara->model.anim.status = (stateStep * 2) + 23;
    }
    else
    {
        chara->model.anim.status = (stateStep * 2) + 17;
    }

    chara->model.anim.time        = Q12(BLOODSUCKER_ANIM_INFOS[chara->model.anim.status].startKeyframeIdx);
    chara->model.anim.keyframeIdx = BLOODSUCKER_ANIM_INFOS[chara->model.anim.status].startKeyframeIdx;
#endif
}

void Bloodsucker_Init(s_SubCharacter* bloodsucker)
{
    s32 i;

    Chara_PropsClear(bloodsucker);
    bloodsucker->collision.state = CharaCollisionState_4;
    bloodsucker->headingAngle    = bloodsucker->rotation.vy;

    Bloodsucker_AnimUpdateFromStep(bloodsucker);
    ModelAnim_AnimInfoSet(&bloodsucker->model.anim, BLOODSUCKER_ANIM_INFOS);
    Chara_DamageClear(bloodsucker);
}

void Bloodsucker_Control_1(s_SubCharacter* bloodsucker)
{
    bloodsucker->model.anim.time = Q12(81.0f) + bloodsuckerProps.timer_E8;
}

void Bloodsucker_Control_2(s_SubCharacter* bloodsucker)
{
#ifdef MAP3_S03
    if (bloodsucker->model.anim.status == ((bloodsucker->model.stateStep * 2) + 9))
    {
        bloodsucker->model.anim.status = (bloodsucker->model.stateStep * 2) + 22;
    }

    if (PLAYER_CHARA().position.vx < Q12(-140.75f))
    {
        bloodsucker->model.controlState = BloodsuckerControl_3;
    }

    bloodsuckerProps.timer_F0 = Q12(0.3f);
#endif
}

void Bloodsucker_Control_3(s_SubCharacter* bloodsucker)
{
    if (bloodsucker->model.anim.status == ((bloodsucker->model.stateStep * 2) + 23))
    {
        bloodsucker->model.anim.status = (bloodsucker->model.stateStep * 2) + 8;
    }

    if (PLAYER_CHARA().position.vx > Q12(-140.5f))
    {
        bloodsucker->model.controlState = BloodsuckerControl_2;
    }

    bloodsuckerProps.timer_F0 = Q12(1.0f);
}

void Bloodsucker_Control_4(s_SubCharacter* bloodsucker)
{
#ifdef MAP3_S03
    if (bloodsucker->model.anim.status != ((bloodsucker->model.stateStep * 2) + 2) &&
        bloodsucker->model.anim.status != ((bloodsucker->model.stateStep * 2) + 3) &&
        bloodsucker->model.anim.status != ((bloodsucker->model.stateStep * 2) + 16) &&
        bloodsucker->model.anim.status != ((bloodsucker->model.stateStep * 2) + 17))
    {
        bloodsucker->model.anim.status = (bloodsucker->model.stateStep * 2) + 2;
    }

    if (ANIM_STATUS_IS_ACTIVE(bloodsucker->model.anim.status))
    {
        bloodsuckerProps.timer_F0 = Q12(0.3f);
    }
    else
    {
        bloodsuckerProps.timer_F0 = Q12(0.75f);
    }
#endif
}

void sharedFunc_800D0F28_3_s03(s_SubCharacter* bloodsucker, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords)
{
    typedef struct
    {
        MATRIX   field_0;
        SVECTOR3 field_20;
        u8       unk_26[2];
        VECTOR3  field_28;
        u8       unk_34[4];
        VECTOR3  field_38;
    } s_sharedFunc_800D0F28_3_s03;

    s_AnimInfo*                  animInfo;
    s_sharedFunc_800D0F28_3_s03* scratchData;

    scratchData = PSX_SCRATCH;

    Math_MatrixTransform(&bloodsucker->position, &bloodsucker->rotation, boneCoords);
    animInfo = &BLOODSUCKER_ANIM_INFOS[bloodsucker->model.anim.status];
    animInfo->playbackFunc(&bloodsucker->model, anmHdr, boneCoords, animInfo);
    Vw_CoordHierarchyMatrixCompute(&boneCoords[14], &scratchData->field_0);

    gte_SetRotMatrix(&scratchData->field_0);
    gte_SetTransMatrix(&scratchData->field_0);

    Math_SetSVectorFast(&scratchData->field_20, 0, 0, 0);

    gte_ldv0(&scratchData->field_20);
    gte_rt();
    gte_stlvnl(&scratchData->field_28);

    scratchData->field_28.vx = Q8_TO_Q12(scratchData->field_28.vx);
    scratchData->field_28.vy = Q8_TO_Q12(scratchData->field_28.vy);
    scratchData->field_28.vz = Q8_TO_Q12(scratchData->field_28.vz);

    bloodsucker->collision.box.topExtent            = scratchData->field_28.vy - Q12(0.05f);
    bloodsucker->collision.box.bottomExtent         = scratchData->field_28.vy + Q12(0.05f);
    bloodsucker->collision.box.height               = scratchData->field_28.vy + Q12(0.05f);
    bloodsucker->collision.box.offsetY              = scratchData->field_28.vy;
    bloodsucker->collision.shapeOffsets.box.vx      = scratchData->field_28.vx - bloodsucker->position.vx;
    bloodsucker->collision.shapeOffsets.box.vz      = scratchData->field_28.vz - bloodsucker->position.vz;
    bloodsucker->collision.cylinder.innerRadius     = Q12(0.05f);
    bloodsucker->collision.shapeOffsets.cylinder.vx = scratchData->field_28.vx - bloodsucker->position.vx;
    bloodsucker->collision.shapeOffsets.cylinder.vz = scratchData->field_28.vz - bloodsucker->position.vz;
    bloodsucker->collision.cylinder.outerRadius     = Q12(0.05f);
    Math_SetSVectorFast(&scratchData->field_20, 0, -22, 0);

    gte_ldv0(&scratchData->field_20);
    gte_rt();
    gte_stlvnl(&scratchData->field_28);

    scratchData->field_28.vx = Q8_TO_Q12(scratchData->field_28.vx);
    scratchData->field_28.vy = Q8_TO_Q12(scratchData->field_28.vy);
    scratchData->field_28.vz = Q8_TO_Q12(scratchData->field_28.vz);
    scratchData->field_38.vx = scratchData->field_28.vx;
    scratchData->field_38.vy = scratchData->field_28.vy;
    scratchData->field_38.vz = scratchData->field_28.vz;

    bloodsucker->attack.state = 1;

    func_8008A0E4(1, WEAPON_ATTACK(AttackType_Bloodsucker, AttackInputType_Tap), bloodsucker, &scratchData->field_38, &PLAYER_CHARA(), Q12_ANGLE(90.0f), Q12_ANGLE(90.0f));
}
