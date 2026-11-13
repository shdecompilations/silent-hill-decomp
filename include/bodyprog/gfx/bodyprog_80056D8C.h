#ifndef _BODYPROG_BODYPROG80056D8C_H
#define _BODYPROG_BODYPROG80056D8C_H

/** @brief Struct used by many functions involved with GTE. Kept at `PSX_SCRATCH_ADDR` (possibly only temporarily). */
typedef struct _GteScratchData
{
    /* 0x0   */ DVECTOR screenXy_0[90];
    /* 0x168 */ u16     screenZ_168[18];
    /* 0x18C */ s16     field_18C[99]; // The size changed from 150 due to the addition of `field_252`. Not sure if this is correct.
    /* 0x252 */ u8      field_252[102];
    /* 0x2B8 */ u8      field_2B8[200]; // Size likely incorrect.

                union
                {
                    MATRIX field_0;
                    struct
                    {
                        /* 0x0  */ s32     field_0;
                        /* 0x4  */ s32     field_4;
                        /* 0x8  */ CVECTOR field_8;
                        /* 0xC  */ CVECTOR field_C;
                        /* 0x10 */ u8      field_10; // `faceIdxs`.
                        /* 0x11 */ u8      field_11;
                        /* 0x12 */ u8      field_12;
                        /* 0x13 */ u8      field_13;
                        /* 0x14 */ u8      field_14;
                        /* 0x15 */ u8      field_15;
                        /* 0x16 */ u8      field_16;
                        /* 0x17 */ u8      field_17;
                        /* 0x18 */ s32     field_18;
                        /* 0x1C */ s32     field_1C;
                    } s_0;
    /* 0x380 */ } field_380;

    /* 0x3A0 */ s_Normal field_3A0;
    /* 0x3A4 */ DVECTOR  screenPos_3A4;
    /* 0x3A8 */ s32      depthP_3A8;
    /* 0x3AC */ SVECTOR  field_3AC;
    /* 0x3B4 */ u8       unk_3B4[36];
    /* 0x3D8 */ CVECTOR  field_3D8; // Written by `func_8005A42C`.
    /* 0x3DC */ DVECTOR  screenPos_3DC;
    /* 0x3E0 */ s32      depthP_3E0;
    /* 0x3E4 */ s16      rotMatrix_3E4[3][3]; // Truncated `MATRIX` without the `long t[3];` transfer vector?
} s_GteScratchData;

typedef struct
{
    /* 0x0   */ DVECTOR  screenXy_0[90];
    /* 0x168 */ s16      screenZ_168[18];
    /* 0x18C */ s16      field_18C[72];
    /* 0x21C */ s32      field_21C[39]; // Used as `VECTOR3`?
    /* 0x2B8 */ u8       field_2B8[200];
    /* 0x380 */ MATRIX   field_380;
    /* 0x3A0 */ s_Normal field_3A0;
    /* 0x3A4 */ DVECTOR  screenPos_3A4;
    /* 0x3A8 */ s32      depthP_3A8;
    /* 0x3AC */ SVECTOR  field_3AC;
    /* 0x3B4 */ u8       unk_3B4[36];
    /* 0x3D8 */ CVECTOR  field_3D8;

                 // Different functions access different data at 0x3DC onwards.
                 // Union works for it, but also possible those functions just took different `s_GteScratchData` structs.
                 union
                 {
                     struct
                     {
                         /* 0x3DC */ DVECTOR screenPos_3DC;
                         /* 0x3E0 */ s32     depthP_3E0;
                         /* 0x3E4 */ s16     rotMatrix_3E4[3][3]; // Truncated `MATRIX` without the `long t[3];` transfer vector?
                     } vertex;

                     struct
                     {
                         /* 0x3DC */ s_Normal field_3DC;
                         /* 0x3E0 */ SVECTOR  field_3E0[3];
                     } normal;

                     struct
                     {
                         /* 0x0 */ u8  field_0;
                         /* 0x1 */ u8  field_1;
                         /* 0x2 */ u8  field_2;
                         /* 0x3 */ u8  field_3;
                         /* 0x4 */ u8  field_4;
                         /* 0x5 */ u8  field_5;
                         /* 0x6 */ u8  field_6;
                         /* 0x7 */ u8  field_7;
                         /* 0x8 */ s32 field_8;
                     } s_1;
    /* 0x3DC */ } u;
} s_GteScratchData2;

/** @brief Draws a 2D fog overlay quad. */
void Gfx_FogOverlayQuadDraw(s16 x0, s16 y0, s16 x1, s16 y1, s32 arg4, s32 arg5, GsOT* ot, s32 otShift);

/** Draws 3D IPD chunk models? */
void func_80057090(s_ModelInfo* modelInfo, GsOT* otTag, s32 otShift, MATRIX* viewMat, MATRIX* worldMat, u16 clutY);

s32 func_800571D0(u32 arg0);

void WorldEnvRender_LightTransform(MATRIX* worldMat, q19_12 alpha, SVECTOR* arg2, VECTOR3* arg3);

void func_80057344(s_ModelInfo* modelInfo, GsOT_TAG* otTag, s32 otShift, MATRIX* mat);

void func_800574D4(s_MeshHeader* meshHdr, s_GteScratchData* scratchData);

void func_8005759C(s_MeshHeader* meshHdr, s_GteScratchData* scratchData, s32 vertOffset, s32 normalOffset);

void func_80057658(s_MeshHeader* meshHdr, s32 offset, s_GteScratchData* scratchData, SVECTOR3* arg3, SVECTOR* arg4);

void func_80057A3C(s_MeshHeader* meshHdr, s32 offset, s_GteScratchData* scratchData, SVECTOR3* lightVec);

void func_80057B7C(s_MeshHeader* meshHdr, s32 offset, s_GteScratchData* scratchData, MATRIX* mat);

/** Main quad drawing func? */
void Gfx_MeshDraw(s_MeshHeader* meshHdr, s_GteScratchData* scratchData, GsOT_TAG* tag, s32 arg3);

/** TODO: Unknown `arg3` type. */
void func_80059D50(s32 arg0, s_ModelInfo* modelInfo, MATRIX* viewMat, s32 otShift, GsOT_TAG* tag);

void func_80059E34(u32 arg0, s_MeshHeader* meshHdr, s_GteScratchData* scratchData, s32 otShift, GsOT_TAG* tag);

void func_8005A21C(s_ModelInfo* modelInfo, GsOT_TAG* otTag, s32 otShift, MATRIX* viewMat);

/** @brief Computes a fog-shaded version of `D_800C4190` color using `arg1` as the distance factor?
 *  Stores the result at 0x3D8 into `arg0`.
 */
void func_8005A42C(s_GteScratchData* scratchData, q19_12 alpha);

void func_8005A478(s_GteScratchData* scratchData, q19_12 alpha);

/** `scratchData` is unused. */
void func_8005A838(s_GteScratchData* scratchData, q19_12 scale);

void func_8005A900(s_MeshHeader* meshHdr, s32 offset, s_GteScratchData* scratchData, MATRIX* viewMat);

u8 func_8005AA08(s_MeshHeader* meshHdr, s32 arg1, s_GteScratchData2* scratchData);

void func_8005AC50(s_MeshHeader* meshHdr, s_GteScratchData2* scratchData, GsOT_TAG* ot, bool otShift);

#endif
