#ifndef _BODYPROG_GFX_WORLDOBJECT_H
#define _BODYPROG_GFX_WORLDOBJECT_H

#include "bodyprog/chara/model.h"
#include "bodyprog/formats/model.h"
#include "main/fsqueue.h"

#define WORLD_OBJECT_COUNT_MAX 29

/** @brief World object metadata. */
typedef struct _WorldObjectMetadata
{
    /* 0x0 */ u_Filename name;
    /* 0x8 */ s8         unused_8;      /** Unused, dead code. Set to 0 in `WorldObject_ModelNameSet` and never referenced again. */
    /* 0x9 */ s8         modelLocation; /** `e_WorldModelLocation` */
} s_WorldObjectMetadata;

/** @brief World object model. TODO: Rename to "static object"? Conceptually it's what this is in modern terms. */
typedef struct _WorldObjectModel
{
    /* 0x0  */ s_ModelInfo           modelInfo;
    /* 0x10 */ s_WorldObjectMetadata metadata;
} s_WorldObjectModel;
STATIC_ASSERT_SIZEOF(s_WorldObjectModel, 28);

/** @brief Geometry-space world object to draw. */
typedef struct _WorldObject
{
    /* 0x0    */ s_WorldObjectModel* model;
    /* 0x4+0  */ s32                 positionX  : 18;
    /* 0x4+18 */ s32                 positionY  : 14;
    /* 0x8+0  */ s32                 positionZ  : 18;
    /* 0x8+18 */ s32                 __pad_8_18 : 14;
    /* 0xC+0  */ s32                 rotationX  : 10;
    /* 0xC+10 */ s32                 rotationY  : 12;
    /* 0xC+22 */ s32                 rotationZ  : 10;
} s_WorldObject;
STATIC_ASSERT_SIZEOF(s_WorldObject, 16);

/** @brief Hand-held player item. */
typedef struct _HeldItem
{
    /* 0x0  */ s32           itemId; /** `e_InvItemId` */
    /* 0x4  */ s32           queueIdx;
    /* 0x8  */ char*         textureName;
    /* 0xC  */ s_FsImageDesc imageDesc;
    /* 0x14 */ s_LmHeader*   lmHdr;
    /* 0x18 */ s_Bone        bone;
} s_HeldItem;
STATIC_ASSERT_SIZEOF(s_HeldItem, 44);

#endif
