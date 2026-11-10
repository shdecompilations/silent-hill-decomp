#ifndef _BODYPROG_ITEMS_CREDITSINIT_H
#define _BODYPROG_ITEMS_CREDITSINIT_H

#include "main/fsqueue.h"
#include "screens/stf_roll/credits.h"

extern s32 D_800C48F0;

extern s_CreditTextState   g_CreditTextState;
extern s_CreditText3dState g_CreditText3dState;

void GameFs_StfRollBinLoad(void);

void func_800917C0(void* arg0, s32 arg1, s_FsImageDesc* img);

void func_8009185C(s16 arg0, s16 arg1, s32 arg2, q19_12 arg3, q19_12 arg4, q19_12 arg5, q19_12 arg6, q19_12 arg7, s32 arg8, s32 arg9);

#endif
