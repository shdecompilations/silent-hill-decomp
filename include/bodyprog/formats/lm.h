#ifndef _BODYPROG_FORMATS_LM_H
#define _BODYPROG_FORMATS_LM_H

#include "bodyprog/formats/model.h"
#include "bodyprog/formats/texture.h"

#define LM_HEADER_MAGIC '0'
#define LM_VERSION      6

/** @brief LM file material. */
typedef struct _Material
{
    /* 0x0  */ u_Filename name; // Texture name?
    /* 0x8  */ s_Texture* texture;
    /* 0xC  */ u8         field_C; /** `bool` | Loaded/applied flag? Marks as having image t-page and blend mode set. */
    /* 0xD  */ s8         __pad_D;
    /* 0xE  */ u8         field_E; // Packed t-page, blend mode data..
    /* 0xF  */ u8         field_F;
    /* 0x10 */ u16        field_10; // Packed CLUT position?
    /* 0x12 */ u16        field_12;
               union
               {
                   u8  u8[2];
                   u16 u16;
    /* 0x14 */ } field_14; // UV
               union
               {
                   u8  u8[2];
                   u16 u16;
    /* 0x16 */ } field_16; // UV. Min or max, not sure.
} s_Material;
STATIC_ASSERT_SIZEOF(s_Material, 24);

/** @brief LM file header. */
typedef struct _LmHeader
{
    /* 0x0  */ u8             magic;    /** Equal to `LM_HEADER_MAGIC` in valid file. */
    /* 0x1  */ u8             version;  /** Equal to `LM_VERSION` in valid file. */
    /* 0x2  */ u8             isLoaded; /** `bool` */
    /* 0x3  */ u8             materialCount;
    /* 0x4  */ s_Material*    materials;
    /* 0x8  */ u8             modelCount;
    /* 0x9  */ s8             __pad_9[3];
    /* 0xC  */ s_ModelHeader* modelHdrs;
    /* 0x10 */ u8*            modelOrder;
} s_LmHeader;

#endif
