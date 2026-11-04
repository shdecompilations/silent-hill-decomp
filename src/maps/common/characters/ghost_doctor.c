#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "maps/common/characters/ghost_doctor.h"

#define ghostDocProps ghostDoc->properties.npc

void GhostDoctor_Update(s_SubCharacter* ghostDoc, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords) // 0x800D8AF0
{
    if (ghostDoc->model.charaId != Chara_GhostDoctor)
    {
        GhostDoctor_Init(ghostDoc);
    }

    GhostDoctor_CoordRotationUpdate(ghostDoc, boneCoords);
    GhostDoctor_CoordTransformUpdate(ghostDoc, boneCoords);
    GhostDoctor_AnimUpdate(ghostDoc, anmHdr, boneCoords);
}

void GhostDoctor_AnimUpdate(s_SubCharacter* ghostDoc, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords) // 0x800D8B64
{
    Chara_AnimUpdate(ghostDoc, anmHdr, boneCoords, GHOST_DOCTOR_ANIM_INFOS);
}

void GhostDoctor_CoordTransformUpdate(s_SubCharacter* ghostDoc, GsCOORDINATE2* boneCoords) // 0x800D8BAC
{
    boneCoords[GhostDoctorBone_Root].coord.t[0] = Q12_TO_Q8(ghostDoc->position.vx);
    boneCoords[GhostDoctorBone_Root].coord.t[1] = Q12_TO_Q8(ghostDoc->position.vy);
    boneCoords[GhostDoctorBone_Root].coord.t[2] = Q12_TO_Q8(ghostDoc->position.vz);
}

void GhostDoctor_Init(s_SubCharacter* ghostDoc) // 0x800D8BE0
{
    Chara_CollisionReset(ghostDoc);
}

void GhostDoctor_CoordRotationUpdate(s_SubCharacter* ghostDoc, GsCOORDINATE2* boneCoords) // 0x800D8C00
{
    if (ghostDocProps.controlState == GhostDoctorControl_Still)
    {
        if (ghostDoc->model.stateStep == 0)
        {
            ghostDoc->model.anim.status = ANIM_STATUS(GhostDoctorAnim_1, false);
            ghostDoc->model.stateStep++;
        }

        Chara_AnimStateReset(ghostDoc);
    }

    boneCoords->flg = false;
    Math_RotMatrixZxyNegGte(&ghostDoc->rotation, &boneCoords->coord);
}
