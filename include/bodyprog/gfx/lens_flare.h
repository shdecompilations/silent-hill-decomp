#ifndef _BODYPROG_GFX_LENSFLARE_H
#define _BODYPROG_GFX_LENSFLARE_H

/** @brief Lens flare workspace.
 *
 * @note Field names are taken from SH2, which has an anonymous struct that matches almost exactly.
 */
typedef struct _LensFlareWork
{
    /* 0x0  */ u8      flare_inhibit_f;           /** `bool` | Disable lens flare. */
    /* 0x1  */ u8      water_inhibit_f;           /** `bool` | Disable water shimmer. */
    /* 0x2  */ u8      lfl_sync_draw_func_exec_f; /** `bool` | Has drawn lens flare. */
    /* 0x3  */ u8      draw_center_f;             /** `bool` | @unused */
    /* 0x4  */ s8      __pad_4[4];
    /* 0x8  */ q3_12   tgt_l_eff_rate;
    /* 0xA  */ q3_12   now_l_eff_rate;            /** Moves halfway to `tgt_l_eff_rate` each frame. */
    /* 0xC  */ VECTOR3 scr_l_pos;                 /** Light position projected to the screen. */
    /* 0x18 */ s8      __pad_18[4];
    /* 0x1C */ s32     scr_l_ang_xy;
    /* 0x20 */ q19_12  scr_l_ang_z;
} s_LensFlareWork;

extern s_LensFlareWork g_LensFlareWork;

/** @brief Initializes `g_LensFlareWork`. */
void shLensFlareInit(void);

/** @brief Disables the lens flare effect. */
void LensFlare_LensFlareDisable(void);

/** @brief Enables the lens flare effect. */
void LensFlare_LensFlareEnable(void);

/** @brief Disables the water shimmer effect. */
void LensFlare_WaterShimmerDisable(void);

/** @brief Enables the water shimmer effect. */
void LensFlare_WaterShimmerEnable(void);

/** @brief Sets up the lens flare and water shimmer effects.
 *
 * @note Name from SH2.
 *
 * @param light_intensity Light intensity.
 * @param rot Light rotation.
 * @param pos Light position.
 * @param waterZones Reference water zones.
 */
void shLensFlareExec(q3_12 light_intensity, SVECTOR* rot, VECTOR3* pos, s_WaterZone* waterZones);

// Named from SH2 symbols.
void shLensFlareSetLightSeed(VECTOR3* scr_l_pos, q3_12 scr_l_ang_z);

/** @brief Draws a lens flare effect if it wasn't already drawn.
 *
 * @note Name from SH2.
 */
void shLensFlareDraw(void);

/** @brief Checks if the light source is visible and a lens flare can be drawn.
 *
 * @note Name from SH2. The code was changed for that game.
 *
 * @return `true` if the light source is visible, `false` otherwise.
 */
bool shLensFlareLightCenterIsVisible(void);

// Named from SH2 symbols.
q19_12 shLensFlareMakeEffectTargetRate(q3_12 light_eff_pow, q19_12 scr_l_pos_z, q19_12 scr_l_ang_z);

/** @brief Draws a lens flare effect
 *
 * @note Name from SH2.
 *
 * @param TODO
 */
void shLensFlareEffect_Draw(bool center_visible_f, q19_12 now_l_eff_rate,
                            VECTOR3* scr_l_pos, q19_12 scr_l_ang_xy, q19_12 scr_l_ang_z);

/** @brief Gets the water zone at a given position for a water shimmer effect.
 *
 * @param posX X position.
 * @param posZ Z position.
 * @param waterZones Water zones to query.
 * @return Water zone at the given position.
 */
s_WaterZone* LensFlare_WaterZoneGet(q27_4 posX, q27_4 posZ, s_WaterZone* waterZones);

void func_8008E5B4(void);

void func_8008E794(VECTOR3* pos, q3_12 angle, q19_12 posY);

void func_8008EA68(SVECTOR* rot, VECTOR3* posXz, q19_12 posY);

#endif
