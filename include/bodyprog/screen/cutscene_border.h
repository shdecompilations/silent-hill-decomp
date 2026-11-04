#ifndef _BODYPROG_SCREEN_CUTSCENEBORDER_H
#define _BODYPROG_SCREEN_CUTSCENEBORDER_H

/** @brief Cutscene border states used by `g_SysWork.cutsceneBorderState` */
typedef enum _CutsceneBorderState
{
    CutsceneBorderState_Reset        = 0,
    CutsceneBorderState_None         = 1,

    CutsceneBorderState_Active       = 1 << 4,                         /** Packing mask. */
    CutsceneBorderState_FadeInStart  = CutsceneBorderState_Active + 2, /** 18. */
    CutsceneBorderState_FadingIn     = CutsceneBorderState_Active + 3, /** 19. */
    CutsceneBorderState_ForceShow    = CutsceneBorderState_Active + 4, /** 20. */
    CutsceneBorderState_Shown        = CutsceneBorderState_Active + 5, /** 21. */
    CutsceneBorderState_FadeOutStart = CutsceneBorderState_Active + 6, /** 22. */
    CutsceneBorderState_FadingOut    = CutsceneBorderState_Active + 7, /** 23. */
} e_CutsceneBorderState;

/** @brief Resets cutscene borders. */
#define CutsceneBorder_Reset() \
    g_SysWork.cutsceneBorderState = CutsceneBorderState_Reset

/** @brief Forces cutscene borders to show. */
#define CutsceneBorder_ForceShow() \
    g_SysWork.cutsceneBorderState = CutsceneBorderState_ForceShow

/** @brief Starts cutscene border fade-in. */
#define CutsceneBorder_FadeInStart() \
    g_SysWork.cutsceneBorderState = CutsceneBorderState_FadeInStart

/** @brief Starts cutscene border fade-out. */
#define CutsceneBorder_FadeOutStart() \
    g_SysWork.cutsceneBorderState = CutsceneBorderState_FadeOutStart

/** @brief Updates the cutscene border.
 *
 * Scratch: https://decomp.me/scratch/ztPzJ
 */
void Screen_CutsceneCameraStateUpdate(void);

#endif
