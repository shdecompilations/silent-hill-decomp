#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player/player.h"
#include "main/rng.h"
#include "maps/shared.h"
#include "maps/characters/lisa.h"

/** AI code for `Chara_Lisa`
 *
 * Included in:
 *  MAP3_S04
 *  MAP4_S04
 *  MAP7_S00
 *  MAP7_S01
 *  MAP7_S02
 */

#define lisaProps (lisa->properties.npc)

/** Addresses
 * MAP3_S04: 0x800D0888
 * MAP4_S04: 0x800CFEA0
 * MAP7_S00: 0x800CEA38
 * MAP7_S01: 0x800D4DFC
 * MAP7_S02: 0x800D5998
 */
void Lisa_Update(s_SubCharacter* lisa, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords)
{
    if (lisa->model.controlState == LisaControl_None)
    {
        Lisa_Init(lisa);
    }

    Lisa_AnimStateUpdate(lisa, boneCoords);
    Lisa_MovementUpdate(lisa, boneCoords);
    Lisa_AnimUpdate(lisa, anmHdr, boneCoords);
}

/** Addresses
 * MAP3_S04: 0x800D08FC
 * MAP4_S04: 0x800CFF14
 * MAP7_S00: 0x800CEAAC
 * MAP7_S01: 0x800D4E70
 * MAP7_S02: 0x800D5A0C
 */
void Lisa_AnimUpdate(s_SubCharacter* lisa, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords)
{
    Chara_AnimUpdate(lisa, anmHdr, boneCoords, LISA_ANIM_INFOS);
}

/** Addresses
 * MAP3_S04: 0x800D0944
 * MAP4_S04: 0x800CFF5C
 * MAP7_S00: 0x800CEAF4
 * MAP7_S01: 0x800D4EB8
 * MAP7_S02: 0x800D5A54
 */
void Lisa_MovementUpdate(s_SubCharacter* lisa, GsCOORDINATE2* boneCoords)
{
    VECTOR3 unused;
    VECTOR3 offset;
    q19_12  moveSpeed;
    q3_12   headingAngle;
    q19_12  moveDist;
    s32     scaleRestoreShift;
    u32     scaleReduceShift;

    unused       = lisa->position;
    moveSpeed    = lisa->moveSpeed;
    headingAngle = lisa->headingAngle;
    moveDist      = Q12_MULT_PRECISE(moveSpeed, g_DeltaTime);

    scaleRestoreShift = OVERFLOW_GUARD(moveDist);
    scaleReduceShift  = scaleRestoreShift >> 1;

    offset.vx = (u32)Q12_MULT_PRECISE(moveDist >> scaleReduceShift, Math_Sin(headingAngle) >> scaleReduceShift) << scaleRestoreShift;
    offset.vz = (u32)Q12_MULT_PRECISE(moveDist >> scaleReduceShift, Math_Cos(headingAngle) >> scaleReduceShift) << scaleRestoreShift;
    offset.vy = Q12_MULT_PRECISE(lisa->fallSpeed, g_DeltaTime);

    lisa->position.vx += offset.vx;
    lisa->position.vy  = Q12(0.0f);
    lisa->position.vz += offset.vz;

    boneCoords[LisaBone_Root].coord.t[0] = Q12_TO_Q8(lisa->position.vx);
    boneCoords[LisaBone_Root].coord.t[1] = Q12_TO_Q8(lisa->position.vy);
    boneCoords[LisaBone_Root].coord.t[2] = Q12_TO_Q8(lisa->position.vz);
}

/** Addresses
 * MAP3_S04: 0x800D0ADC
 * MAP4_S04: 0x800D00F4
 * MAP7_S00: 0x800CEC8C
 * MAP7_S01: 0x800D5050
 * MAP7_S02: 0x800D5BEC
 */
void Lisa_AnimStateUpdate(s_SubCharacter* lisa, GsCOORDINATE2* boneCoords)
{
    s_CollisionSurface surface;
    e_SfxId            sfxId;
    s8                 pitch0;
    s8                 pitch1;

    switch (lisaProps.controlState)
    {
        case LisaControl_None:
            break;

        case LisaControl_1:
            if (lisaProps.moveSpeed > Q12(1.25f))
            {
                lisaProps.moveSpeed -= TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.5f));
                if (lisaProps.moveSpeed < Q12(1.25f))
                {
                    lisaProps.moveSpeed = Q12(1.25f);
                }
            }
            else if (lisaProps.moveSpeed < Q12(1.25f))
            {
                lisaProps.moveSpeed += TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f));
                lisaProps.moveSpeed  = CLAMP(lisaProps.moveSpeed, 0, Q12(1.25f));
            }

            Model_AnimStatusSet(&lisa->model, LisaAnim_2, false);
            Chara_AnimStateReset(lisa);
            break;

        case LisaControl_5:
        case LisaControl_20:
            if (lisaProps.moveSpeed)
            {
                lisaProps.moveSpeed -= TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12(0.4f)) * 2;
                if (lisaProps.moveSpeed < Q12(0.0f))
                {
                    lisaProps.moveSpeed = Q12(0.0f);
                }
            }

            if (lisaProps.controlState == LisaControl_5)
            {
                Model_AnimStatusKeyframeSet(lisa->model, LisaAnim_1, true, LISA_ANIM_INFOS, 0);
            }
            else
            {
                Model_AnimStatusSet(&lisa->model, LisaAnim_1, true);
            }

            Chara_AnimStateReset(lisa);
            break;

        case LisaControl_6:
            Model_AnimStatusSet(&lisa->model, LisaAnim_3, false);
            Chara_AnimStateReset(lisa);
            break;

        case LisaControl_7:
            Model_AnimStatusSet(&lisa->model, LisaAnim_4, false);

            if (lisa->model.anim.keyframeIdx == 72)
            {
                lisaProps.controlState = LisaControl_20;
                lisa->model.stateStep  = 0;
            }
            break;

        case LisaControl_8:
            Model_AnimStatusSet(&lisa->model, LisaAnim_5, false);
            Chara_AnimStateReset(lisa);
            break;

        case LisaControl_9:
            Model_AnimStatusKeyframeSet(lisa->model, LisaAnim_6, true, LISA_ANIM_INFOS, 0);
            Chara_AnimStateReset(lisa);
            break;

        case LisaControl_10:
            Model_AnimStatusSet(&lisa->model, LisaAnim_7, false);
            Chara_AnimStateReset(lisa);
            break;

        case LisaControl_11:
            Model_AnimStatusKeyframeSet(lisa->model, LisaAnim_8, true, LISA_ANIM_INFOS, 0);
            Chara_AnimStateReset(lisa);
            break;

        case LisaControl_12:
            Model_AnimStatusKeyframeSet(lisa->model, LisaAnim_9, true, LISA_ANIM_INFOS, 0);
            Chara_AnimStateReset(lisa);
            break;

        case LisaControl_13:
            Model_AnimStatusSet(&lisa->model, LisaAnim_10, false);
            Chara_AnimStateReset(lisa);
            break;

        case LisaControl_14:
            Model_AnimStatusKeyframeSet(lisa->model, LisaAnim_11, true, LISA_ANIM_INFOS, 0);
            break;

        case LisaControl_15:
            Model_AnimStatusSet(&lisa->model, LisaAnim_12, false);
            break;

        case LisaControl_16:
            Model_AnimStatusSet(&lisa->model, LisaAnim_13, false);
            Chara_AnimStateReset(lisa);
            break;

        case LisaControl_17:
            Model_AnimStatusKeyframeSet(lisa->model, LisaAnim_14, true, LISA_ANIM_INFOS, 0);
            Chara_AnimStateReset(lisa);
            break;

        case LisaControl_18:
            Model_AnimStatusKeyframeSet(lisa->model, LisaAnim_15, true, LISA_ANIM_INFOS, 0);
            Chara_AnimStateReset(lisa);
            break;

        case LisaControl_19:
            Model_AnimStatusSet(&lisa->model, LisaAnim_16, false);
            Chara_AnimStateReset(lisa);
            break;

        case LisaControl_21:
            Model_AnimStatusSet(&lisa->model, LisaAnim_17, false);
            Chara_AnimStateReset(lisa);
            break;
    }

    Collision_SurfaceGet(&surface, lisa->position.vx, lisa->position.vz);

    Player_FootstepSfxGet(surface.floorType, &sfxId, &pitch0, &pitch1);
    sfxId = Sfx_Unk1529;

    switch (lisaProps.controlState)
    {
        case LisaControl_1:
            Npc_FootstepSoundPlay(ANIM_STATUS(LisaAnim_2, true), lisa, 14, 31, sfxId, pitch0);
            break;

        case LisaControl_8:
            if (lisa->model.anim.keyframeIdx <= 125)
            {
                Npc_FootstepSoundPlay(ANIM_STATUS(LisaAnim_5, true), lisa, 125, 138, sfxId, pitch0);
            }
            else if (lisa->model.anim.keyframeIdx <= 153)
            {
                Npc_FootstepSoundPlay(ANIM_STATUS(LisaAnim_5, true), lisa, 153, 138, sfxId, pitch0);
            }
            else
            {
                Npc_FootstepSoundPlay(ANIM_STATUS(LisaAnim_5, true), lisa, 189, 184, sfxId, pitch0);
            }
            break;

        case LisaControl_12:
            if (lisa->model.anim.keyframeIdx <= 258)
            {
                // @bug From the other `keyframeIdx` checks here, should this be 258 instead of 263?
                Npc_FootstepSoundPlay(ANIM_STATUS(LisaAnim_9, true), lisa, 263, 258, sfxId, pitch1);
            }
            else
            {
                Npc_FootstepSoundPlay(ANIM_STATUS(LisaAnim_9, true), lisa, 263, 266, sfxId, pitch1);
            }
            break;

        case LisaControl_13:
            if (lisa->model.anim.keyframeIdx <= 332)
            {
                Npc_FootstepSoundPlay(ANIM_STATUS(LisaAnim_10, true), lisa, 332, 337, sfxId, pitch0);
            }
            else if (lisa->model.anim.keyframeIdx <= 344)
            {
                Npc_FootstepSoundPlay(ANIM_STATUS(LisaAnim_10, true), lisa, 344, 337, sfxId, pitch0);
            }
            else
            {
                Npc_FootstepSoundPlay(ANIM_STATUS(LisaAnim_10, true), lisa, 352, 348, sfxId, pitch0);
            }
            break;

        case LisaControl_11:
            Npc_FootstepSoundPlay(ANIM_STATUS(LisaAnim_8, true), lisa, 240, 240, sfxId, pitch0);
            break;

        case LisaControl_15:
#if defined(MAP7_S00)
            Npc_SfxPlay(ANIM_STATUS(LisaAnim_12, true), lisa, 408, Sfx_Unk1639);
#endif
            Npc_FootstepSoundPlay(ANIM_STATUS(LisaAnim_12, true), lisa, 406, 406, sfxId, pitch0);
            break;

        case LisaControl_16:
            if (lisa->model.anim.keyframeIdx <= 423)
            {
                Npc_FootstepSoundPlay(ANIM_STATUS(LisaAnim_13, true), lisa, 423, 425, sfxId, pitch0);
            }
            else if (lisa->model.anim.keyframeIdx <= 429)
            {
                Npc_FootstepSoundPlay(ANIM_STATUS(LisaAnim_13, true), lisa, 429, 425, sfxId, pitch0);
            }
            else
            {
                Npc_FootstepSoundPlay(ANIM_STATUS(LisaAnim_13, true), lisa, 435, 432, sfxId, pitch0);
            }
            break;

        case LisaControl_17:
            if (lisa->model.anim.keyframeIdx <= 460)
            {
                Npc_FootstepSoundPlay(ANIM_STATUS(LisaAnim_14, true), lisa, 460, 477, sfxId, pitch0);
            }
            else if (lisa->model.anim.keyframeIdx <= 497)
            {
                Npc_FootstepSoundPlay(ANIM_STATUS(LisaAnim_14, true), lisa, 497, 477, sfxId, pitch0);
            }
            else
            {
                Npc_FootstepSoundPlay(ANIM_STATUS(LisaAnim_14, true), lisa, 536, 516, sfxId, pitch0);
            }
            break;

        case LisaControl_18:
            if (lisa->model.anim.keyframeIdx <= 551)
            {
                Npc_FootstepSoundPlay(ANIM_STATUS(LisaAnim_15, true), lisa, 551, 555, sfxId, pitch0);
            }
            else if (lisa->model.anim.keyframeIdx <= 559)
            {
                Npc_FootstepSoundPlay(ANIM_STATUS(LisaAnim_15, true), lisa, 559, 555, sfxId, pitch0);
            }
            else
            {
                Npc_FootstepSoundPlay(ANIM_STATUS(LisaAnim_15, true), lisa, 564, 564, sfxId, pitch0);
            }
            break;
    }

    lisa->rotation.vy  = Q12_ANGLE_ABS(lisa->rotation.vy + Q8_TO_Q4(sharedData_800D6BB8_3_s04));
    lisa->headingAngle = lisa->rotation.vy;
    lisa->moveSpeed    = lisaProps.moveSpeed;
    lisa->fallSpeed   += g_GravitySpeed;

    boneCoords->flg = false;
    Math_RotMatrixZxyNegGte(&lisa->rotation, &boneCoords->coord);
}

/** Addresses
 * MAP3_S04: 0x800D1350
 * MAP4_S04: 0x800D0968
 * MAP7_S00: 0x800CF514
 * MAP7_S01: 0x800D58C4
 * MAP7_S02: 0x800D6460
 */
void Lisa_Init(s_SubCharacter* lisa)
{
    Chara_CollisionReset(lisa);
    sharedData_800D6BB8_3_s04 = 0;
}
