#ifndef _BODYPROG_GFX_WATER_H
#define _BODYPROG_GFX_WATER_H

/** @brief Lens flare workspace.
 *
 * @note Field names are taken from SH2, which has an anonymous struct that matches almost exactly.
 */
typedef struct _LensFlareWork
{
    /* 0x0  */ u8      flare_inhibit_f;           /** `bool` */
    /* 0x1  */ u8      water_inhibit_f;           /** `bool` */
    /* 0x2  */ u8      lfl_sync_draw_func_exec_f; /** `bool` */
    /* 0x3  */ u8      draw_center_f;             /** `bool` | @unused */
    /* 0x4  */ s8      unk_4[4];
    /* 0x8  */ s16     tgt_l_eff_rate;
    /* 0xA  */ s16     now_l_eff_rate;            /** Moves halfway to `tgt_l_eff_rate` each frame. */
    /* 0xC  */ VECTOR3 scr_l_pos;                 /** Light position projected to the screen. */
    /* 0x18 */ s8      unk_18[4];
    /* 0x1C */ s32     scr_l_ang_xy;
    /* 0x20 */ q19_12  scr_l_ang_z;
} s_LensFlareWork;

extern s_LensFlareWork g_LensFlareWork;

void shLensFlareInit(void);

void func_8008D438(void);

void func_8008D448(void);

void func_8008D454(void);

void func_8008D464(void);

/** Light function. */
void shLensFlareExec(q3_12 light_intensity, SVECTOR* rot, VECTOR3* pos, s_WaterZone* waterZones);

// Named from SH2 symbols.
void shLensFlareSetLightSeed(VECTOR3* scr_l_pos, s16 scr_l_ang_z);

// Named from SH2 symbols.
void shLensFlareDraw(void);

// Named from SH2 symbols (func code doesn't match SH2, but the purpose/caller does)
bool shLensFlareLightCenterIsVisible(void);

// Named from SH2 symbols.
q19_12 shLensFlareMakeEffectTargetRate(q3_12 light_eff_pow, q19_12 scr_l_pos_z, q19_12 scr_l_ang_z);

// Named from SH2 symbols.
void shLensFlareEffect_Draw(bool center_visible_f, q19_12 now_l_eff_rate,
                            VECTOR3* scr_l_pos, q19_12 scr_l_ang_xy, q19_12 scr_l_ang_z);

/** @brief Gets the water zone at a given position.
 *
 * @param posX X position.
 * @param posZ Z position.
 * @param waterZones Water zones to query.
 * @return Water zone at the given position.
 */
s_WaterZone* Map_WaterZoneGet(q27_4 posX, q27_4 posZ, s_WaterZone* waterZones);

void func_8008E5B4(void);

void func_8008E794(VECTOR3* posXz, q3_12 angle, q19_12 posY);

void func_8008EA68(SVECTOR* arg0, VECTOR3* posXz, q19_12 posY);

#endif
