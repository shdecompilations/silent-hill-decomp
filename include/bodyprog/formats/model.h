#ifndef _BODYPROG_FORMATS_MODEL_H
#define _BODYPROG_FORMATS_MODEL_H

/** @brief Model flags. */
typedef enum _ModelFlags
{
    ModelFlag_None = 0,
    ModelFlag_0    = 1 << 0, // Related to view matrix reset?
    ModelFlag_31   = 1 << 31 // Related to some kind of bone reset?
} e_ModelFlags;

/** @brief Geometry primitive. */
typedef struct _Primitive
{
    /* 0x0  */ u8  u0;
    /* 0x1  */ u8  v0;
    /* 0x2  */ u16 bits0;
    /* 0x4  */ u8  u1;
    /* 0x5  */ u8  v1;
               union
               {
                         struct
                         {
                             /* 0x6    */ u8 field_6;           // Set to `s_Material::field_E`.
                             /* 0x6+8  */ s8 materialIdx   : 7;
                             /* 0x6+17 */ u8 isTransparent : 1; /** `bool` */
                         } bits;
               /* 0x0 */ u16 flags; // @hack `func_8005AC50` accesses `isTransparent` above with some weird shifts, haven't found how to make it work with bitfield yet.
    /* 0x6  */ } bits1;
    /* 0x8  */ u8 u2;
    /* 0x9  */ u8 v2;
    /* 0xA  */ u8 u3;
    /* 0xB  */ u8 v3;
    /* 0xC  */ u8 faceIdxs[4];
    /* 0x10 */ u8 field_10[4]; // Normal indices?
} s_Primitive;

/** @brief Mesh data header. */
typedef struct _MeshHeader
{
    /* 0x0  */ u8           primitiveCount;
    /* 0x1  */ u8           vertexCount;
    /* 0x2  */ u8           normalCount;
    /* 0x3  */ u8           unkCount_3;
    /* 0x4  */ s_Primitive* primitives;
    /* 0x8  */ DVECTOR*     verticesXy;
    /* 0xC  */ s16*         verticesZ;
    /* 0x10 */ s_Normal*    normals;
    /* 0x14 */ u8*          unkPtr_14;
} s_MeshHeader;

/** @brief Model data header. */
typedef struct _ModelHeader
{
    /* 0x0   */ u_Filename    name;
    /* 0x8   */ u8            meshCount;
    /* 0x9   */ u8            vertexOffset;
    /* 0xA   */ u8            normalOffset;
    /* 0xB+0 */ u8            field_B_0 : 1;
    /* 0xB+1 */ u8            field_B_1 : 3; // Value used in `func_800571D0` switch.
    /* 0xB+4 */ u8            field_B_4 : 2;
    /* 0xB+6 */ u8            unk_B_6   : 2;
    /* 0xC   */ s_MeshHeader* meshHdrs;
} s_ModelHeader;

/** @brief Model info. */
typedef struct _ModelInfo
{
    /* 0x0 */ s32            flags; // `e_ModelFlags` | Visible bone flags? Reset? Only flags 0 and 31 are used.
    /* 0x4 */ GsCOORDINATE2* coord;
    /* 0x8 */ s_ModelHeader* modelHdr;
    /* 0xC */ s32            modelIdx;
} s_ModelInfo;

#endif
