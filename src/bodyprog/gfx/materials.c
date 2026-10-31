#include "game.h"
#include "inline_no_dmpsx.h"

#include <psyq/gtemac.h>
#include <psyq/libapi.h>
#include <psyq/strings.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "bodyprog/screen/screen_data.h"
#include "bodyprog/screen/screen_draw.h"
#include "bodyprog/item_screens.h"
#include "bodyprog/player.h"
#include "bodyprog/sound/sound_system.h"
#include "main/rng.h"

void Lm_TransparentPrimSet(s_LmHeader* lmHdr, bool isTransparent) // 0x80056244
{
    s_ModelHeader* modelHdrs;
    s_ModelHeader* curModelHdr;
    s_MeshHeader*  curMeshHdr;
    s_Primitive*   prim;

    modelHdrs = lmHdr->modelHdrs;

    for (curModelHdr = &modelHdrs[0]; curModelHdr < &modelHdrs[lmHdr->modelCount]; curModelHdr++)
    {
        for (curMeshHdr = &curModelHdr->meshHdrs[0]; curMeshHdr < &curModelHdr->meshHdrs[curModelHdr->meshCount]; curMeshHdr++)
        {
            for (prim = &curMeshHdr->primitives[0]; prim < &curMeshHdr->primitives[curMeshHdr->primitiveCount]; prim++)
            {
                prim->bits1.bits.isTransparent = isTransparent;
            }
        }
    }
}

s32 Lm_MaterialCountGet(bool (*filterFunc)(s_Material* mat), s_LmHeader* lmHdr) // 0x80056348
{
    s32         count;
    s_Material* curMat;

    count = 0;
    for (curMat = lmHdr->materials; curMat < (lmHdr->materials + lmHdr->materialCount); curMat++)
    {
        if (filterFunc(curMat))
        {
            count++;
        }
    }

    return count;
}

void func_800563E8(s_LmHeader* lmHdr, s32 arg1, s32 arg2, s32 arg3) // 0x800563E8
{
    s32         i;
    s_Material* curMat;

    if (arg2 < 0)
    {
        arg2 += 15;
    }

    for (i = 0, curMat = &lmHdr->materials[0];
         i < lmHdr->materialCount;
         i++, curMat++)
    {
        // TODO: Bitfield stuff? Doesn't seem to match other uses of `field_E`/`field_10` we've seen though.
        u8  temp_a0 = curMat->field_E;
        u16 temp_v1 = curMat->field_10;

        curMat->field_E  = ((temp_a0 + arg1) & 0x1F) | (temp_a0 & 0xE0);
        curMat->field_10 = ((temp_v1 + (arg2 >> 4)) & 0x3F) | ((temp_v1 + (arg3 << 6)) & 0x7FC0);
    }
}

void Lm_MaterialFileIdxApply(s_LmHeader* lmHdr, e_FsFile fileIdx, s_FsImageDesc* image, s32 blendMode) // 0x80056464
{
    char  sp10[8];
    char  sp18[16];
    char* sp10Ptr;
    char* sp18Ptr;

    // Probably a `memset`.
    *(int*)sp10 = *(int*)(sp10 + 4) = 0;

    Fs_GetFileName(sp18, fileIdx);

    sp10Ptr = sp10;
    sp18Ptr = sp18;

    while (sp10Ptr < &sp10[ARRAY_SIZE(sp10)] && *sp18Ptr != '.')
    {
        *sp10Ptr++ = *sp18Ptr++;
    }

    Lm_MaterialFsImageApply(lmHdr, sp10, image, blendMode);
}

void Lm_MaterialFsImageApply1(s_LmHeader* lmHdr, char* newStr, s_FsImageDesc* image, s32 blendMode) // 0x80056504
{
    char strCpy[8];

    StringCopy(strCpy, newStr);
    Lm_MaterialFsImageApply(lmHdr, strCpy, image, blendMode);
}

bool Lm_MaterialFsImageApply(s_LmHeader* lmHdr, char* fileName, s_FsImageDesc* image, s32 blendMode) // 0x80056558
{
    s_Material* curMat;

    // Run through materials.
    for (curMat = &lmHdr->materials[0];
         curMat < &lmHdr->materials[lmHdr->materialCount];
         curMat++)
    {
        if (!COMPARE_FILENAMES(&curMat->name, fileName))
        {
            curMat->field_C = true;
            Material_FsImageApply(curMat, image, blendMode);
            return true;
        }
    }

    return false;
}

void Material_FsImageApply(s_Material* mat, s_FsImageDesc* image, s32 blendMode) // 0x8005660C
{
    s32 coeff;

    coeff = 4;
    switch (image->tPage[0])
    {
        default:
        case 0:
            break;

        case 1:
            coeff = 2;
            break;

        case 2:
            coeff = 1;
            break;
    }

    mat->field_14.u8[0] = image->u * coeff;
    mat->field_14.u8[1] = image->v;

    // Set GPU flags.
    mat->field_E  = ((image->tPage[0] & 0x3) << 7) | // X bits of texture page.
                    ((blendMode & 0x3) << 5) |       // Semi-transparency blend mode.
                    (image->tPage[1] & (1 << 4)) |   // dither/texture-depth flag.
                    (image->tPage[1] & 0xF);         // Y bits + color depth.
    mat->field_10 = (image->clutY << 6) |
                    ((image->clutX >> 4) & 0x3F);
}

void Lm_MaterialBlendModeSet(s_LmHeader* lmHdr, s_FsImageDesc* images, s8 unused, s32 startIdx, s32 blendMode) // 0x800566B4
{
    char           filename[16];
    s32            i;
    s_Material*    curMat;
    s_FsImageDesc* curImage;

    // Loop could be using `&image[i]`/`&arg0->field_4[i]` instead? Wasn't able to make that match though.
    for (i = 0, curImage = images, curMat = lmHdr->materials;
         i < lmHdr->materialCount;
         i++, curMat++, curImage++)
    {
        Material_TimFileNameGet(filename, curMat);
        Fs_QueueStartReadTim(Fs_FindNextFile(filename, 0, startIdx), FS_BUFFER_9, curImage);
        Material_FsImageApply(curMat, curImage, blendMode);
    }
}

void Lm_MaterialsLoadWithFilter(s_LmHeader* lmHdr, s_ActiveChunkTextures* activeTexs, bool (*filterFunc)(s_Material* mat), e_FsFile fileIdx, s32 blendMode) // 0x80056774
{
    s_Material* curMat;

    for (curMat = &lmHdr->materials[0]; curMat < &lmHdr->materials[lmHdr->materialCount]; curMat++)
    {
        if (!curMat->field_C && curMat->texture == NULL &&
            (filterFunc == NULL || filterFunc(curMat)))
        {
            curMat->texture = Texture_Get(curMat, activeTexs, FS_BUFFER_9, fileIdx, blendMode);
            if (curMat->texture != NULL)
            {
                Material_FsImageApply(curMat, &curMat->texture->imageDesc, blendMode);
            }
        }
    }
}

bool Lm_IsTextureLoaded(s_LmHeader* lmHdr) // 0x80056888
{
    s_Material* curMat;

    if (!lmHdr->isLoaded)
    {
        return false;
    }

    // Run through materials.
    for (curMat = &lmHdr->materials[0]; curMat < &lmHdr->materials[lmHdr->materialCount]; curMat++)
    {
        if (curMat->field_C)
        {
            continue;
        }

        if (curMat->texture == NULL)
        {
            return false;
        }

        if (!Fs_QueueIsEntryLoaded(curMat->texture->queueIdx))
        {
            return false;
        }
    }

    return true;
}

void Lm_MaterialFlagsApply(s_LmHeader* lmHdr) // 0x80056954
{
    s32         i;
    s32         j;
    s32         matFlags;
    s_Material* curMat;

    for (i = 0, curMat = lmHdr->materials; i < lmHdr->materialCount; i++, curMat++)
    {
        matFlags = (curMat->field_E != curMat->field_F) ? MaterialFlag_0 : MaterialFlag_None;

        if (curMat->field_10 != curMat->field_12)
        {
            matFlags |= MaterialFlag_1;
        }
        if (curMat->field_14.u16 != curMat->field_16.u16)
        {
            matFlags |= MaterialFlag_2;
        }

        if (matFlags != 0)
        {
            for (j = 0; j < lmHdr->modelCount; j++)
            {
                if (lmHdr->magic != LM_HEADER_MAGIC)
                {
                    continue;
                }

                Model_MaterialFlagsApply(&lmHdr->modelHdrs[j], i, curMat, matFlags);
            }

            curMat->field_F        = curMat->field_E;
            curMat->field_12       = curMat->field_10;
            curMat->field_16.u8[0] = curMat->field_14.u8[0];
            curMat->field_16.u8[1] = curMat->field_14.u8[1];
        }
    }
}

void Model_MaterialFlagsApply(s_ModelHeader* modelHdr, s32 matIdx, const s_Material* mat, s32 matFlags) // 0x80056A88
{
    u16           field_14;
    u16           field_16;
    s_MeshHeader* curMeshHdr;
    s_Primitive*  curPrim;

    // Run through meshes.
    for (curMeshHdr = modelHdr->meshHdrs; curMeshHdr < &modelHdr->meshHdrs[modelHdr->meshCount]; curMeshHdr++)
    {
        // Run through primitives.
        for (curPrim = curMeshHdr->primitives; curPrim < &curMeshHdr->primitives[curMeshHdr->primitiveCount]; curPrim++)
        {
            // No material.
            if (curPrim->bits1.bits.materialIdx == NO_VALUE)
            {
                curPrim->bits1.bits.field_6 = 32;
            }

            // Apply material flags.
            if (curPrim->bits1.bits.materialIdx == matIdx)
            {
                if (matFlags & MaterialFlag_0)
                {
                    curPrim->bits1.bits.field_6 = mat->field_E;
                }
                if (matFlags & MaterialFlag_1)
                {
                    curPrim->bits0 = mat->field_10 + (curPrim->bits0 - mat->field_12);
                }
                if (matFlags & MaterialFlag_2)
                {
                    field_16            = mat->field_16.u16;
                    field_14            = mat->field_14.u16;
                    *(u16*)&curPrim->u0 = field_14 + (*(u16*)&curPrim->u0 - field_16);
                    *(u16*)&curPrim->u1 = field_14 + (*(u16*)&curPrim->u1 - field_16);
                    *(u16*)&curPrim->u2 = field_14 + (*(u16*)&curPrim->u2 - field_16);
                    *(u16*)&curPrim->u3 = field_14 + (*(u16*)&curPrim->u3 - field_16);
                }
            }
        }
    }
}

void Lm_MaterialRefCountDec(s_LmHeader* lmHdr) // 0x80056BF8
{
    s_Material* curMat;
    s_Texture*  tex;

    // Run through materials.
    for (curMat = &lmHdr->materials[0]; curMat < &lmHdr->materials[lmHdr->materialCount]; curMat++)
    {
        tex = curMat->texture;
        if (tex != NULL)
        {
            tex->refCount--;
            if (tex->refCount < 0)
            {
                tex->refCount = 0;
            }

            curMat->texture = NULL;
        }
    }
}

s32 LmHeader_ModelCountGet(s_LmHeader* lmHdr) // 0x80056C80
{
    return lmHdr->modelCount;
}

void Bone_ModelAssign(s_Bone* bone, s_LmHeader* lmHdr, s32 modelIdx)
{
    s_ModelHeader* modelHdr;

    modelHdr                 = lmHdr->modelHdrs;
    bone->modelInfo.modelIdx = modelIdx;

    if (lmHdr->magic == LM_HEADER_MAGIC)
    {
        bone->modelInfo.modelHdr = &modelHdr[modelIdx];
    }
}

bool Lm_ModelFind(s_WorldObjectModel* model, s_LmHeader* lmHdr, s_WorldObjectMetadata* metadata) // 0x80056CB4
{
    u_Filename     targetModelName;
    s32            modelHdrCount;
    bool           result;
    s32            i;
    s_ModelHeader* modelHdr;

    result = false;

    StringCopy(targetModelName.str, metadata->name.str);

    modelHdrCount = lmHdr->modelCount;

    if (lmHdr->magic == LM_HEADER_MAGIC)
    {
        // Run through model headers.
        for (i = 0,
             modelHdr = &lmHdr->modelHdrs[0];
             i < modelHdrCount; i++, modelHdr++)
        {
            if (!COMPARE_FILENAMES(&modelHdr->name.str, &targetModelName.str))
            {
                result                    = true;
                model->modelInfo.modelIdx = i;
                model->modelInfo.modelHdr = modelHdr;
            }
        }
    }

    return result;
}

void StringCopy(char* prevStr, char* newStr) // 0x80056D64
{
    *(s32*)&prevStr[4] = 0;
    *(s32*)&prevStr[0] = 0;
    strncpy(prevStr, newStr, 8);
}
