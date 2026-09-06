#include "game.h"
#include "inline_no_dmpsx.h"

#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "bodyprog/screen/screen_data.h"
#include "bodyprog/screen/screen_draw.h"
#include "main/fsqueue.h"

s8* D_800C15B0;

// ========================================
// BONES AND SKELETONS
// ========================================

void func_80044F14(GsCOORDINATE2* coord, q3_12 rotZ, q3_12 rotX, q19_12 rotY) // 0x80044F14
{
    SVECTOR* rot; // Q3.12
    MATRIX*  rotMat;
    
    rot    = PSX_SCRATCH;
    rotMat = PSX_SCRATCH_ADDR(sizeof(SVECTOR));
    
    rot->vz = rotZ;
    rot->vy = rotY;
    rot->vx = rotX;

    Math_RotMatrixZxyNegGte(rot, rotMat);
    MulMatrix(&coord->coord, rotMat);
}

s8 Bone_ModelIdxGet(s8* ptr, bool reset) // 0x80044F6C
{
    // TODO: These two are probably part of a bigger struct.
    // `boneMeshIdx` is not referenced directly anywhere else.
    // It must be reset somewhere at some point.
    #define boneIdxs    D_800C15B0
    #define boneMeshIdx D_800C15B4

    if (reset)
    {
        boneIdxs = ptr;
    }

    // LM has <= 1 model.
    if (boneIdxs[0] != -3)
    {
        // -2 for 0 models, 0 for 1 model.
        boneMeshIdx = boneIdxs[0];
        boneIdxs++;
    }
    // Increment `boneMeshIdx` and break loop when equal to number of models.
    else if (++boneMeshIdx >= (boneIdxs[1] - 1))
    {
        boneIdxs++;
    }

    return boneMeshIdx;
}

void Skeleton_Init(s_Skeleton* skel, s_BoneNode* boneHierarchy, u8 boneCount) // 0x80044FE0
{
    skel->boneHierarchy = boneHierarchy;
    skel->boneCount     = boneCount;
    skel->boneIdx       = 0;
    skel->field_2       = 1;
    skel->bones_4       = NULL;

    func_80045014(skel);
}

void func_80045014(s_Skeleton* skel) // 0x80045014
{
    s_BoneNode* curBoneNode;

    // Traverse bone hierarchy and clear flags.
    for (curBoneNode = &skel->boneHierarchy[0]; curBoneNode < &skel->boneHierarchy[skel->boneCount]; curBoneNode++)
    {
        curBoneNode->bone.modelInfo.field_0 = 0;
    }
}

void func_8004506C(s_Skeleton* skel, s_LmHeader* lmHdr) // 0x8004506C
{
    u8  sp10[4]; // Size unsure, this could be larger.
    s32 modelCount;

    modelCount = LmHeader_ModelCountGet(lmHdr);
    sp10[0]    = 0;

    switch (modelCount)
    {
        case 0:
            sp10[0] = BoneHierarchy_End;
            break;

        case 1:
            sp10[0] = 0;
            sp10[1] = BoneHierarchy_End;
            break;

        default:
            sp10[1] = BoneHierarchy_MultiModel;
            sp10[2] = LmHeader_ModelCountGet(lmHdr) - 1;
            sp10[3] = BoneHierarchy_End;
            break;
    }

    func_80045108(skel, lmHdr, (s8*)sp10, false);
}

void func_80045108(s_Skeleton* skel, s_LmHeader* lmHdr, s8* arg2, bool cond) // 0x80045108
{
    s_BoneNode*  curBoneNode;
    s_BoneNode** curBoneNodeOrd;
    s32          boneIdx;

    if (!cond)
    {
        skel->boneIdx = 0;
        skel->bones_4 = NULL;
    }

    boneIdx = skel->boneIdx;
    Skeleton_BoneModelAssign(skel, lmHdr, arg2);

    curBoneNodeOrd = &skel->bones_4;
    while (*curBoneNodeOrd != NULL)
    {
        curBoneNode    = *curBoneNodeOrd;
        curBoneNodeOrd = &curBoneNode->next;
    }

    // `Skeleton_BoneModelAssign` increments `boneIdx`.
    func_80045258(curBoneNodeOrd, &skel->boneHierarchy[boneIdx], skel->boneIdx - boneIdx, lmHdr);
    func_800453E8(skel, false);
}

void Skeleton_BoneModelAssign(s_Skeleton* skel, s_LmHeader* lmHdr, s8* arg2) // 0x800451B0
{
    s32 modelIdx;

    modelIdx = Bone_ModelIdxGet(arg2, true);
    while (modelIdx != BoneHierarchy_End)
    {
        Bone_ModelAssign(&skel->boneHierarchy[skel->boneIdx].bone, lmHdr, modelIdx);

        skel->boneIdx++;
        modelIdx = Bone_ModelIdxGet(arg2, false);
    }
}

void func_80045258(s_BoneNode** boneOrd, s_BoneNode* boneHierarchy, s32 boneIdx, s_LmHeader* lmHdr) // 0x80045258
{
    s_BoneNode* curBoneNode;
    u8*         curObjOrd;

    // Run through model order.
    for (curObjOrd = lmHdr->modelOrder; curObjOrd < &lmHdr->modelOrder[lmHdr->modelCount]; curObjOrd++)
    {
        // Run through partial bone hierarchy.
        for (curBoneNode = boneHierarchy; curBoneNode < &boneHierarchy[boneIdx]; curBoneNode++)
        {
            if (curBoneNode->bone.modelInfo.modelIdx == *curObjOrd)
            {
                *boneOrd = curBoneNode;
                boneOrd  = &curBoneNode->next;
            }
        }
    }

    *boneOrd = NULL;
}

void func_800452EC(s_Skeleton* skel) // 0x800452EC
{
    s32            boneIdxTens;
    s32            boneIdx;
    u32            boneIdxOnes;
    s_BoneNode*    curBoneNode;
    s_ModelHeader* modelHdr;

    curBoneNode = skel->bones_4;
    while (curBoneNode != NULL)
    {
        modelHdr = curBoneNode->bone.modelInfo.modelHdr;

        // Extract bone index from first two `char`s of model name.
        boneIdxOnes = modelHdr->name.str[1] - '0';
        boneIdxTens = modelHdr->name.str[0] - '0';
        if (boneIdxOnes < 10 && boneIdxTens >= 0 && boneIdxTens < 10)
        {
            boneIdx = (boneIdxTens * 10) + boneIdxOnes;
        }
        else
        {
            boneIdx = 0;
        }

        curBoneNode->bone.idx = boneIdx;
        curBoneNode           = curBoneNode->next;
    }
}

void func_80045360(s_Skeleton* skel, s8* arg1) // 0x80045360
{
    s32 i;
    s32 boneIdx;

    for (boneIdx = Bone_ModelIdxGet(arg1, true), i = 0;
         boneIdx != -2;
         i++)
    {
        skel->boneHierarchy[i].bone.idx = boneIdx;
        boneIdx                         = Bone_ModelIdxGet(arg1, false);
    }
}

void func_800453E8(s_Skeleton* skel, bool cond) // 0x800453E8
{
    s_BoneNode* curBoneNode;

    // Traverse bone hierarchy and set flags according to `cond`.
    for (curBoneNode = &skel->boneHierarchy[0]; curBoneNode < &skel->boneHierarchy[skel->boneCount]; curBoneNode++)
    {
        if (cond)
        {
            curBoneNode->bone.modelInfo.field_0 &= ~(1 << 31);
        }
        else
        {
            curBoneNode->bone.modelInfo.field_0 |= 1 << 31;
        }
    }
}

void func_80045468(s_Skeleton* skel, s32* arg1, bool cond) // 0x80045468
{
    s_BoneNode* boneHierarchy;
    s32         modelIdx;

    boneHierarchy = skel->boneHierarchy;

    // Traverse bone hierarchy and set flag 31 according to some condition.
    modelIdx = Bone_ModelIdxGet(arg1, true);
    while (modelIdx != -2)
    {
        if (cond)
        {
            boneHierarchy[modelIdx].bone.modelInfo.field_0 &= ~(1 << 31);
        }
        else
        {
            boneHierarchy[modelIdx].bone.modelInfo.field_0 |= 1 << 31;
        }

        modelIdx = Bone_ModelIdxGet(arg1, false);
    }
}

extern s_WorldEnvWork const g_WorldEnvWork;

void func_80045534(s_Skeleton* skel, GsOT* ot, s32 otShift, GsCOORDINATE2* boneCoords, q3_12 arg4, u16 clutY, s_FsImageDesc* images) // 0x80045534
{
    MATRIX         viewMat;
    MATRIX         worldMat;
    DVECTOR        sp60;
    s32            temp_a1;
    s16            var_fp;
    s16            var_s3;
    s16            var_s4;
    s16            var_s5;
    s16            var_s6;
    s16            var_s7;
    s32            temp_a0;
    s32            temp_s0;
    s32            temp_s1_2;
    s32            temp_s1_3;
    s32            temp_s1_4;
    s32            temp_v1;
    s32            clutY1;
    s32            var_s2;
    s32            var_s3_2;
    s32            var_v0_2;
    s32            var_v0_4;
    s32            var_v0_5;
    s_FsImageDesc* curImage;
    s_BoneNode*    curBoneNode;

    var_s5 = SHRT_MAX;
    var_s6 = SHRT_MAX;
    var_s4 = SHRT_MAX;
    var_s7 = -SHRT_MAX;
    var_fp = -SHRT_MAX;
    var_s3 = -SHRT_MAX;

    if (skel->field_2 == 0)
    {
        return;
    }

    clutY1 = NO_VALUE;

    if (images != NULL)
    {
        for (curImage = images; curImage->clutY != NO_VALUE; curImage++)
        {
            if (clutY1 != curImage->clutY)
            {
                clutY1 = curImage->clutY;
                Vw_CoordToViewSpaceMatrix(&boneCoords[clutY1], &viewMat);
                SetRotMatrix(&viewMat);
                SetTransMatrix(&viewMat);
            }

            gte_ldv0(curImage);
            gte_rtps();
            gte_stsxy(&sp60);
            temp_a1 = gte_stSZ3();

            if (sp60.vx < var_s5)
            {
                var_s5 = sp60.vx;
            }

            if (var_s7 < sp60.vx)
            {
                var_s7 = sp60.vx;
            }

            if (sp60.vy < var_s6)
            {
                var_s6 = sp60.vy;
            }

            if (var_fp < sp60.vy)
            {
                var_fp = sp60.vy;
            }

            if (temp_a1 < var_s4)
            {
                var_s4 = temp_a1;
            }

            if (var_s3 < temp_a1)
            {
                var_s3 = temp_a1;
            }
        }
    }

    // Run through bone hierarchy. TODO: What are these bones?
    for (curBoneNode = skel->bones_4; curBoneNode != NULL; curBoneNode = curBoneNode->next)
    {
        if (curBoneNode->bone.modelInfo.field_0 >= 0)
        {
            Vw_CoordToWorldAndViewMatrices(&boneCoords[(u8)curBoneNode->bone.idx], &worldMat, &viewMat);

            if (curBoneNode->bone.modelInfo.field_0 & (1 << 0))
            {
                viewMat.m[2][2]         = 0;
                *(s32*)&viewMat.m[2][0] = 0;
                *(s32*)&viewMat.m[1][1] = 0;
                *(s32*)&viewMat.m[0][2] = 0;
                *(s32*)&viewMat.m[0][0] = 0;
            }

            func_80057090(&curBoneNode->bone.modelInfo, ot, otShift, &viewMat, &worldMat, clutY);

            if (g_WorldEnvWork.isFogEnabled)
            {
                gte_SetRotMatrix(&viewMat);
                gte_SetTransMatrix(&viewMat);
                gte_ldvxy0_Zero();
                gte_ldvz0();
                gte_rtps();
                gte_stsxy(&sp60);
                temp_a1 = gte_stSZ3();

                if (sp60.vx < var_s5)
                {
                    var_s5 = sp60.vx;
                }

                if (var_s7 < sp60.vx)
                {
                    var_s7 = sp60.vx;
                }

                if (sp60.vy < var_s6)
                {
                    var_s6 = sp60.vy;
                }

                if (var_fp < sp60.vy)
                {
                    var_fp = sp60.vy;
                }

                if (temp_a1 < var_s4)
                {
                    var_s4 = temp_a1;
                }

                if (var_s3 < temp_a1)
                {
                    var_s3 = temp_a1;
                }
            }
        }
    }

    if (g_WorldEnvWork.isFogEnabled)
    {
        temp_s1_2 = g_SysWork.playerBoneCoords[1].coord.t[1];
        temp_s1_2 = CLAMP(temp_s1_2, Q8(-2.0f), Q8(0.0f));

        temp_s1_2 += Q12_TO_Q8(g_SysWork.playerWork.player.position.vy);
        temp_s1_3  = Math_MulFixed(Q12_TO_Q8(g_SysWork.playerWork.player.position.vx), GsWSMATRIX.m[2][0], Q12_SHIFT);
        temp_s0    = Math_MulFixed(temp_s1_2, GsWSMATRIX.m[2][1], Q12_SHIFT);
        temp_s1_4  = ((temp_s1_3 + temp_s0) + Math_MulFixed(Q12_TO_Q8(g_SysWork.playerWork.player.position.vz), GsWSMATRIX.m[2][2], Q12_SHIFT)) + GsWSMATRIX.t[2];

        var_s3_2 = (var_s4 + var_s3) >> 1;
        temp_v1  = var_s3_2 - ((var_s3 - var_s4) >> 1);

        temp_a0 = temp_v1 - 0x33;

        if (temp_s1_4 >= (temp_v1 - 0x41A))
        {
            if (temp_s1_4 < (temp_v1 - 0x1A))
            {
                var_v0_2  = temp_s1_4 + 0x3E7;
                var_v0_2 -= temp_a0;
                var_v0_2  = Q12(var_v0_2);

                if (var_v0_2 < 0)
                {
                    var_v0_2 += 0x3FF;
                }

                var_s3_2 += Math_MulFixed(temp_a0 - var_s3_2, var_v0_2 >> 10, Q12_SHIFT);
            }
            else if (temp_s1_4 < temp_a0)
            {
                var_s3_2 = temp_a0;
            }
            else if (temp_s1_4 < var_s3_2)
            {
                var_s3_2 = temp_s1_4;
            }
        }

        var_s3_2 = MAX(var_s3_2, 4);

        var_s2 = Q8_TO_Q12(var_s4) - arg4;
        var_s2 = MAX(var_s2, 0);

        clutY1   = ReadGeomScreen();
        var_v0_4 = Q12_TO_Q8(arg4) * clutY1;

        if (var_s4 >= 5)
        {
            var_v0_5 = (var_v0_4 / var_s4) + 2;
        }
        else
        {
            var_v0_5 = (var_v0_4 / 4) + 2;
        }

        Gfx_FogOverlayQuadDraw(var_s5 - var_v0_5, var_s6 - var_v0_5, var_s7 + var_v0_5, var_fp + var_v0_5, var_s3_2 * 16, var_s2, ot, otShift);
    }
}
