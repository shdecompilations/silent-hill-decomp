#ifndef _BODYPROG_CHARA_UTILS_H
#define _BODYPROG_CHARA_UTILS_H

s32 Chara_NpcIdxGet(s_SubCharacter* chara);

void Chara_CollisionShapeOffsetsUpdate(s_CharaShapeOffsets* offsets, s_SubCharacter* chara);

s32 Chara_MovementUpdate0(s_SubCharacter* chara, s_CollisionResult* collResult);

s32 Chara_MovementUpdate1(s_SubCharacter* chara, s_CollisionResult* collResult, q3_12 offsetX, q3_12 offsetZ);

/** Used for enemy target locking. Arg at index 3 is an angle? */
void func_8005CD38(s32* out0, q3_12* out1, const VECTOR3* pos, q3_12 angleMin, q19_12 distMin, s32 mode);

/** Computes something for a targeted NPC. */
bool func_8005D50C(s32* targetNpcIdx, q3_12* rotToTargetX, q3_12* rotToTargetY, const VECTOR3* attackPos,
                   u32 npcIdx, q19_12 angleConstraint);

#endif
