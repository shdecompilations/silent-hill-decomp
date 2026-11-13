#ifndef _BODYPROG_GFX_TEXTUREUTILS_H
#define _BODYPROG_GFX_TEXTUREUTILS_H

#include "bodyprog/formats/lm.h"
#include "bodyprog/formats/texture.h"
#include "bodyprog/map/terrain.h"

void Texture_Init(s_Texture* tex, char* texName, u8 tPage0, u8 tPage1, s32 u, s32 v, s16 clutX, s16 clutY);

/** `arg4` unused. */
s_Texture* Texture_Get(s_Material* mat, s_ActiveChunkTextures* activeTexs, void* fsBuf9, e_FsFile fileIdx, s32 arg4);

void Texture_RefCountReset(s_Texture* tex);

/** @unused */
void func_8005B378(s_Texture* tex, char* arg1);

void Texture_RefClear(s_Texture* tex);

void Material_TimFileNameGet(char* filename, s_Material* mat);

void func_8005B424(VECTOR3* vec0, const VECTOR3* vec1);

void Textures_ActiveTex_CountReset(s_ActiveChunkTextures* activeTexs);

void Textures_ActiveTex_PutTextures(s_ActiveChunkTextures* activeTexs, s_Texture* texs, s32 texIdx);

s_Texture* Textures_ActiveTex_FindTexture(char* texName, s_ActiveChunkTextures* activeTexs);

#endif
