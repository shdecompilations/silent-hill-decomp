#ifndef _BODYPROG_GFX_MATERIALS_H
#define _BODYPROG_GFX_MATERIALS_H

void Lm_TransparentPrimSet(s_LmHeader* lmHdr, bool isTransparent);

s32 Lm_MaterialCountGet(bool (*filterFunc)(s_Material* mat), s_LmHeader* lmHdr);

/** @unused No references. */
void func_800563E8(s_LmHeader* lmHdr, s32 arg1, s32 arg2, s32 arg3);

void Lm_MaterialFileIdxApply(s_LmHeader* lmHdr, e_FsFile fileIdx, s_FsImageDesc* image, s32 blendMode);

void Lm_MaterialFsImageApply1(s_LmHeader* lmHdr, char* newStr, s_FsImageDesc* image, s32 blendMode);

bool Lm_MaterialFsImageApply(s_LmHeader* lmHdr, char* fileName, s_FsImageDesc* image, s32 blendMode);

void Material_FsImageApply(s_Material* mat, s_FsImageDesc* image, s32 blendMode);

/** @brief @unused Sets a model's material blending mode. */
void Lm_MaterialBlendModeSet(s_LmHeader* lmHdr, s_FsImageDesc* images, s8 unused, s32 startIdx, s32 blendMode);

void Lm_MaterialsLoadWithFilter(s_LmHeader* lmHdr, s_ActiveChunkTextures* activeTexs, bool (*filterFunc)(s_Material* mat), e_FsFile fileIdx, s32 blendMode);

/** Checks if LM textures are loaded? */
bool Lm_IsTextureLoaded(s_LmHeader* lmHdr);

void Lm_MaterialFlagsApply(s_LmHeader* lmHdr);

void Model_MaterialFlagsApply(s_ModelHeader* modelHdr, s32 matIdx, const s_Material* mat, s32 matFlags);

void Lm_MaterialRefCountDec(s_LmHeader* lmHdr);

s32 LmHeader_ModelCountGet(s_LmHeader* lmHdr);

void Bone_ModelAssign(s_Bone* bone, s_LmHeader* lmHdr, s32 modelIdx);

bool Lm_ModelFind(s_WorldObjectModel* model, s_LmHeader* lmHdr, s_WorldObjectMetadata* metadata);

void StringCopy(char* prevStr, char* newStr);

#endif
