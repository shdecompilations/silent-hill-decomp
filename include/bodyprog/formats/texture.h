#ifndef _BODYPROG_FORMATS_TEXTURE_H
#define _BODYPROG_FORMATS_TEXTURE_H

#include "main/fs_queue.h"

/** @brief Model texture. */
typedef struct _Texture
{
    /* 0x0  */ s_FsImageDesc imageDesc;
    /* 0x8  */ u_Filename    name;
    /* 0x10 */ u32           queueIdx;
    /* 0x14 */ s8            refCount;
} s_Texture;

#endif
