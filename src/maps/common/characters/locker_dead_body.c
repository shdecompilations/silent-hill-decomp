#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "maps/common/characters/locker_dead_body.h"

void LockerDeadBody_Update(s_SubCharacter* deadBody, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords)
{
    s_AnimInfo* animInfo;

    if (deadBody->model.controlState == 0)
    {
        deadBody->model.anim.alpha = Q12(0.0f);
        Chara_AnimSet(deadBody, ANIM_STATUS(LockerDeadBodyAnim_2, true), 0);
        deadBody->model.controlState++;
        deadBody->position.vy = Q12(0.0f);
    }

    if (!Savegame_EventFlagGet(EventFlag_105))
    {
        deadBody->model.anim.flags &= ~AnimFlag_Visible;
        deadBody->collision.state   = CharaCollisionState_Ignore;
        return;
    }

    deadBody->model.anim.flags |= AnimFlag_Visible;

    if (!Savegame_EventFlagGet(EventFlag_106))
    {
        if (deadBody->model.anim.status == ANIM_STATUS(LockerDeadBodyAnim_2, true))
        {
            deadBody->model.anim.status = ANIM_STATUS(LockerDeadBodyAnim_1, true);
        }

        deadBody->collision.state = CharaCollisionState_Ignore;
    }
    else
    {
        Chara_AnimSet(deadBody, ANIM_STATUS(LockerDeadBodyAnim_3, true), 16);
        deadBody->collision.state = CharaCollisionState_Npc;
    }

    Math_MatrixTransform(&deadBody->position, &deadBody->rotation, boneCoords);

    animInfo = &LOCKER_DEAD_BODY_ANIM_INFOS[deadBody->model.anim.status];
    animInfo->playbackFunc(&deadBody->model, anmHdr, boneCoords, animInfo);

    deadBody->collision.box.topExtent            = Q12(-0.3f);
    deadBody->collision.shapeOffsets.cylinder.vx = Q12(0.06f);
    deadBody->collision.shapeOffsets.cylinder.vz = Q12(0.93f);
    deadBody->collision.box.bottomExtent         = Q12(0.0f);
    deadBody->collision.box.height               = Q12(0.0f);
    deadBody->collision.box.offsetY              = Q12(0.0f);
    deadBody->collision.shapeOffsets.box.vx      = Q12(0.0f);
    deadBody->collision.shapeOffsets.box.vz      = Q12(0.0f);
    deadBody->collision.cylinder.innerRadius     = Q12(0.0f);
    deadBody->collision.cylinder.outerRadius     = Q12(0.33f);

    Chara_CollisionShapeOffsetsUpdate(&deadBody->collision.shapeOffsets, deadBody);
}
