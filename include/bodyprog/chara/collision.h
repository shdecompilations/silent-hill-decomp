#ifndef _BODYPROG_CHARA_COLLISION_H
#define _BODYPROG_CHARA_COLLISION_H

// TODO: These structs are conceptually different from the ones in `bodyprog/collision/chara.h`. Need to distinguish
// them more clearly.

/** @brief Character collision states. */
typedef enum _CharaCollisionState
{
    CharaCollisionState_Ignore  = 0,
    CharaCollisionState_Player  = 1,
    CharaCollisionState_2       = 2,
    CharaCollisionState_Npc     = 3,
    CharaCollisionState_4       = 4,
    CharaCollisionState_Default = 5
} e_CharaCollisionState;

/** @brief Character collision box. */
typedef struct _CharaBox
{
    /* 0x0 */ q3_12 topExtent;
    /* 0x2 */ q3_12 bottomExtent;
    /* 0x4 */ q3_12 height;
    /* 0x6 */ q3_12 offsetY;
    // TODO: Maybe nested inner and outer XZ extents. See `s_CharaCylinder`.
    /* 0x8 */ q3_12 field_8; // Outer XZ extent? Always nagative, might be unsigned? `s_CharaCylinder::outerRadius` is set to this.
    /* 0xA */ q3_12 field_A; // Innter XZ extent? `s_CharaCylinder::innerRadius` is set to this.
} s_CharaBox;

// TODO: Too similar to `s_CharaCollisionCylinder`. Maybe more like character detection ranges?
/** @brief Character collision cylinder. */
typedef struct _CharaCylinder
{
    /* 0x0 */ q3_12 outerRadius;
    /* 0x2 */ q3_12 innerRadius;
} s_CharaCylinder;

/** @brief Character shape offsets for `s_CharaBox` and `s_CharaCylinder`. */
typedef struct _CharaShapeOffsets
{
    /* 0x0 */ DVECTOR_XZ box;
    /* 0x4 */ DVECTOR_XZ cylinder;
} s_CharaShapeOffsets;

/** @brief Character keyframe collision info. */
typedef struct _CharaKeyframe
{
    /* 0x0 */ s_CharaBox          box;
    /* 0xC */ s_CharaShapeOffsets shapeOffsets;
} s_CharaKeyframe;

/** @brief Recorded character-to-character collision.
 * Similar to `s_CharaCollisionCylinder` but with extra `passStamp` field.
 */
typedef struct _CharaCollisionRecord
{
    /* 0x0  */ VECTOR3 position;     /** Q19.12 */
    /* 0xC  */ q3_12   bottomExtent;
    /* 0xE  */ q3_12   topExtent;
    /* 0x10 */ q3_12   radius;
    /* 0x12 */ s8      state;        /** `e_CharaCollisionState` */
    /* 0x13 */ u8      passStamp;    /** Tracks visited characters per query pass. */
} s_CharaCollisionRecord;

/** @brief Character collision info for the active tick. */
typedef struct _CharaCollision
{
    /* 0x0    */ s_CharaBox              box;
    /* 0xC    */ s_CharaCylinder         cylinder;
    /* 0x10   */ s_CharaShapeOffsets     shapeOffsets;         // Translation data?
    /* 0x18+0 */ u8                      passStamp;            /** Same usage as `s_IpdCollisionData::shapePassStampBuffer`. */
    /* 0x19+0 */ s8                      state            : 4; /** `e_CharaCollisionState` */
    /* 0x19+4 */ u8                      charaRecordCount : 4; /** `charaRecords` array size. */
    /* 0x1C   */ s_CharaCollisionRecord* charaRecords;
} s_CharaCollision;

#endif
