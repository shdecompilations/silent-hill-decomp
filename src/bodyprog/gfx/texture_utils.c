#include "bodyprog/game.h"
#include "inline_no_dmpsx.h"

#include <psyq/gtemac.h>
#include <psyq/libapi.h>
#include <psyq/strings.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/gfx/texture_utils.h"
#include "bodyprog/math/math.h"
#include "bodyprog/screen/data.h"
#include "bodyprog/screen/draw.h"
#include "bodyprog/items/item_screens.h"
#include "bodyprog/player/player.h"
#include "bodyprog/sound/sd_call.h"
#include "main/rng.h"

void Texture_Init(s_Texture* tex, char* texName, u8 tPage0, u8 tPage1, s32 u, s32 v, s16 clutX, s16 clutY) // 0x8005B1A0
{
    tex->imageDesc.tPage[0] = tPage0;
    tex->imageDesc.tPage[1] = tPage1;
    tex->imageDesc.u        = u;
    tex->imageDesc.v        = v;
    tex->imageDesc.clutX    = clutX;
    tex->imageDesc.clutY    = clutY;

    StringCopy(tex->name.str, texName);

    tex->refCount = 0;
    tex->queueIdx = NO_VALUE;
}

s_Texture* Texture_Get(s_Material* mat, s_ActiveChunkTextures* activeTexs, void* fsBuffer9, e_FsFile fileIdx, s32 arg4)
{
    char       filename[12];
    char       debugStr[16];
    s32        fileId;
    s32        i;
    s32        smallestQueueIdx;
    u32        queueIdx;
    s_Texture* curTex;
    s_Texture* foundTex;

    smallestQueueIdx = INT_MAX;
    mat->texture     = NULL;
    foundTex         = NULL;

    for (i = 0; i < activeTexs->count; i++)
    {
        curTex = activeTexs->textures[i];

        if (!COMPARE_FILENAMES(&mat->name, &curTex->name))
        {
            mat->texture = curTex;
            curTex->refCount++;
            return curTex;
        }

        queueIdx = curTex->queueIdx;
        if ((s32)queueIdx < smallestQueueIdx && curTex->refCount == 0)
        {
            smallestQueueIdx = queueIdx;
            foundTex       = curTex;
        }
    }

    if (foundTex == NULL)
    {
        return NULL;
    }

    Material_TimFileNameGet(filename, mat);
    fileId = Fs_FindNextFile(filename, 0, fileIdx);
    if (fileId == NO_VALUE)
    {
        // Failed to find file. Log filename to screen.
        debugStr[12] = 0;
        Text_Debug_PositionSet(100, 80);
        strncpy(debugStr, filename, 12);

#if VERSION_EQUAL_OR_OLDER(PROTO_981216)
        // Code seen in 98-12-16 build.
        Text_Debug_Draw(debugStr);
#endif
    }

    foundTex->queueIdx = Fs_QueueStartReadTim(fileId, fsBuffer9, &foundTex->imageDesc);
    foundTex->refCount++;
    foundTex->name = mat->name;

    return foundTex;
}

void Texture_RefCountReset(s_Texture* tex) // 0x8005B370
{
    tex->refCount = 0;
}

void func_8005B378(s_Texture* tex, char* arg1) // 0x8005B378
{
    tex->refCount = 1;
    tex->queueIdx = 0;
    StringCopy(tex->name.str, arg1);
}

void Texture_RefClear(s_Texture* tex) // 0x8005B3A4
{
    tex->name.u32[1] = 0;
    tex->name.u32[0] = 0;

    tex->refCount = 0;
    tex->queueIdx = NO_VALUE;
}

void Material_TimFileNameGet(char* filename, s_Material* mat) // 0x8005B3BC
{
    char filenameCpy[12];

    // Some inline `memcpy`/`bcopy`/`strncpy`? those use `lwl`/`lwr`/`swl`/`swr` instead though
    // Example: casting `filename`/`arg1` to `u32*` and using `memcpy` does generate `lw`/`sw`,
    // but not in same order as this, guess it's some custom inline/macro instead.
    *(u32*)&filenameCpy[0] = *(u32*)&mat->name.str[0];
    *(u32*)&filenameCpy[4] = *(u32*)&mat->name.str[4];
    *(u32*)&filenameCpy[8] = 0;

    strcat(filenameCpy, ".TIM");

    *(u32*)&filename[0] = *(u32*)&filenameCpy[0];
    *(u32*)&filename[4] = *(u32*)&filenameCpy[4];
    *(u32*)&filename[8] = *(u32*)&filenameCpy[8];
}

void func_8005B424(VECTOR3* vec0, const VECTOR3* vec1) // 0x8005B424
{
    typedef struct { char pad[12]; } Unk_cast;

    vec0->vz = 0;
    vec0->vy = 0;
    vec0->vx = 0;

    if (vec1 == NULL)
    {
        return;
    }

    *((Unk_cast*)vec0) = *((Unk_cast*)vec1);
}

void Textures_ActiveTex_CountReset(s_ActiveChunkTextures* activeTexs) // 0x8005B46C
{
    activeTexs->count = 0;
}

void Textures_ActiveTex_PutTextures(s_ActiveChunkTextures* activeTexs, s_Texture* texs, s32 texIdx) // 0x8005B474
{
    s_Texture*  curTex;
    s_Texture** texEntries;

    texEntries = activeTexs->textures;
    for (curTex = &texs[0]; curTex < &texs[texIdx];)
    {
        *texEntries++ = curTex++;
        activeTexs->count++;
    }
}

s_Texture* Textures_ActiveTex_FindTexture(char* texName, s_ActiveChunkTextures* activeTexs) // 0x8005B4BC
{
    char       targetTexName[8];
    s32        i;
    s_Texture* tex;

    StringCopy(targetTexName, texName);

    for (i = 0; i < activeTexs->count; i++)
    {
        tex = activeTexs->textures[i];
        if (tex->queueIdx != NO_VALUE && !COMPARE_FILENAMES(targetTexName, &tex->name))
        {
            return tex;
        }
    }

    return NULL;
}
