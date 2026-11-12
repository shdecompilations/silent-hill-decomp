#ifndef _BODYPROG_CHARAFILEINFOS_H
#define _BODYPROG_CHARAFILEINFOS_H

#include "main/fsqueue.h"

/** @brief Character file info.
 * Holds file IDs of anim/model/texture for each `e_CharaId` along with some data used in VC camera code.
 */
typedef struct _CharaFileInfo
{
    /* 0x0    */ s16            animFileIdx;
    /* 0x2    */ s16            modelFileIdx;
    /* 0x4+0  */ s16            textureFileIdx    : 16;
    /* 0x4+16 */ q8_8           field_6           : 10;
    /* 0x4+26 */ u16            materialBlendMode : 6; /** `e_BlendMode` */
    /* 0x8    */ s_FsImageDesc* field_8;               // TODO: Extra texture pointer? Usually `NULL` in `CHARA_FILE_INFOS`.
    /* 0xC+0  */ u16            cameraAnchor  : 2;     /** `e_CameraAnchor` */
    /* 0xC+2  */ q19_12         cameraOffsetY : 14;
                 // 2 bytes of padding.
} s_CharaFileInfo;

/** Array containg file IDs used for each `e_CharaId`, used in `Fs_QueueStartReadAnm`. */
extern s_CharaFileInfo CHARA_FILE_INFOS[45]; // TODO: Use `Chara_Count` again somehow.

#endif
