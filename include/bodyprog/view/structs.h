#ifndef _BODYPROG_VIEW_STRUCTS_H
#define _BODYPROG_VIEW_STRUCTS_H

#include "bodyprog/view/enums.h"

#define CAMERA_PATH_COLL_COUNT_MAX 10

/** @brief 2D camera path area constraint on the XZ plane. */
typedef struct _VC_LIMIT_AREA
{
    /* 0x0 */ q11_4 min_hx;
    /* 0x2 */ q11_4 max_hx;
    /* 0x4 */ q11_4 min_hz;
    /* 0x6 */ q11_4 max_hz;
} VC_LIMIT_AREA;

/** @brief Internal camera info. */
typedef struct _VC_CAMERA_INTINFO
{
    /* 0x0 */ u32   mode;        /** Mode state step. */
    /* 0x4 */ u8    mv_smooth;   /** `VC_CAM_MV_TYPE` */
              // 1 byte of padding.
    /* 0x6 */ q3_12 ev_cam_rate; /** Camera elevation rate. */
} VC_CAMERA_INTINFO;

/** @brief Camera look-at move parameters.
 * TODO: These don't seem to be angles like the names would suggest, but distances don't fit either.
 */
typedef struct _VC_WATCH_MV_PARAM
{
    /* 0x0 */ q19_12 ang_accel_x;   /** Angle acceleration on X axis. */
    /* 0x4 */ q19_12 ang_accel_y;   /** Angle acceleration on Y axis. */
    /* 0x8 */ q3_12  max_ang_spd_x; /** Max angle speed on X axis. */
    /* 0xA */ q3_12  max_ang_spd_y; /** Max angle speed on Y axis. */
} VC_WATCH_MV_PARAM;

/** @brief Camera move parameters. */
typedef struct _VC_CAM_MV_PARAM
{
    /* 0x0 */ q19_12 accel_xz;   /** Speed acceleration on XZ plane. */
    /* 0x4 */ q19_12 accel_y;    /** Speed acceleration on Y axis. */
    /* 0x8 */ q19_12 max_spd_xz; /** Max speed on XZ plane. */
    /* 0xC */ q19_12 max_spd_y;  /** Max speed on Y axis. */
} VC_CAM_MV_PARAM;

/** @brief Camera path data.
 *
 * @note In SH2, the `.cam` files contain this struct, while in SH1 this is part of `s_MapOverlayHdr`.
 */
typedef struct _VC_ROAD_DATA
{
    /* 0x0     */ VC_LIMIT_AREA     lim_sw;
    /* 0x8     */ VC_LIMIT_AREA     lim_rd;
    /* 0x10+0  */ VC_ROAD_FLAGS     flags          : 8; /** `VC_ROAD_FLAGS` | Camera path flags. */
    /* 0x11+0  */ VC_AREA_SIZE_TYPE area_size_type : 2;
    /* 0x11+8  */ VC_ROAD_TYPE      rd_type        : 3; /** Path type. */
    /* 0x11+10 */ u32               mv_y_type      : 3; /** `VC_CAM_MV_TYPE` */
    /* 0x12+0  */ q27_4             lim_rd_max_hy  : 8; /** In SH2, `max_hy` and `min_hy` are part of `VC_LIMIT_AREA`.In SH1, these are separate for some reason. */
    /* 0x13+0  */ q27_4             lim_rd_min_hy  : 8;
    /* 0x14+0  */ q27_4             ofs_watch_hy   : 8;
    /* 0x14+8  */ u32               lens_flare     : 4; /** `e_LensFlareType` | TODO: Unsure, rough name. */
    /* 0x14+12 */ s16               cam_mv_type    : 4; /** `VC_CAM_MV_TYPE` */
    /* 0x16    */ q0_7              fix_ang_x;          /** @note Part of union in SH2 `VC_ROAD_DATA`. */
    /* 0x17    */ q0_7              fix_ang_y;
} VC_ROAD_DATA;

/** @brief Rail camera parementers. */
typedef struct _VC_THROUGH_DOOR_CAM_PARAM
{
    /* 0x0  */ u8      active_f;               /** `bool` | Active flag. */
               // 3 bytes of padding.
    /* 0x4  */ q19_12  timer;
    /* 0x8  */ q3_12   rail_ang_y;             /** Rail Y angle. */
               // 2 bytes of padding.
    /* 0xC  */ VECTOR3 rail_sta_pos;           /** Rail start position. */
    /* 0x18 */ q19_12  rail_sta_to_chara_dist; /** Distance from rail start position to locked-on character position. */
} VC_THROUGH_DOOR_CAM_PARAM;

/** @brief Nearby camera path collision. */
typedef struct _VC_NEAR_ROAD_DATA
{
    /* 0x0 */  VC_ROAD_DATA* road_p;              /** Path associated with the collision. */
    /* 0x4 */  u8            rd_dir_type;         /** `VC_ROAD_DIR_TYPE` */
    /* 0x5 */  u8            use_priority;        /** Usage priority in case of overlap. Higher values take precedence. */
               // 2 bytes of padding.
    /* 0x8 */  q19_12        chara2road_sum_dist; /** Character to path distance. */
    /* 0xC */  q19_12        chara2road_vec_x;    /** Character to path distance on X axis. */
    /* 0x10 */ q19_12        chara2road_vec_z;    /** Character to path distance on Z axis. */
    /* 0x14 */ VC_LIMIT_AREA rd;                  /** Camera path constraint on XZ plane. */
    /* 0x1C */ VC_LIMIT_AREA sw;                  /** Switch constraint on XZ plane. */
} VC_NEAR_ROAD_DATA;

/** @brief Camera workspace. */
typedef struct _VC_WORK
{
    /* 0x0   */ u8                        view_cam_active_f;                         /** `bool` */
    /* 0x4   */ VC_ROAD_DATA*             vc_road_ary_list;                          /** Camera paths for the active map. */
    /* 0x8   */ u32                       flags;                                     /** `VC_FLAGS` */
    /* 0xC   */ u8                        through_door_activate_init_f;              /** `bool` | Camera rail activation flag? */
                // 3 bytes of padding.
    /* 0x10  */ VC_THROUGH_DOOR_CAM_PARAM through_door;                              /** Active rail camera data? */
    /* 0x1C  */ q3_12                     scr_half_ang_wy;
    /* 0x2E  */ q3_12                     scr_half_ang_wx;
    /* 0x30  */ q3_12                     geom_screen_dist;                          /** Related to `GsSetProjection`/`g_GameSys.gs_y_res_58A`. */
                // 2 bytes of padding.
    /* 0x34  */ VC_CAM_MV_PARAM           user_cam_mv_prm;                           /** Look parameters? */
    /* 0x44  */ VECTOR3                   cam_tgt_pos;                               /** Target camera position. */
    /* 0x50  */ VECTOR3                   cam_pos;                                   /** Q19.12 | Camera position. */
    /* 0x5C  */ q3_12                     cam_mv_ang_y;                              /** Angular velocity on the Y axis. */
                // 2 bytes of padding.
    /* 0x60  */ VECTOR3                   cam_velo;                                  /** Q19.12 | Camera velocity. */
    /* 0x6C  */ q19_12                    old_cam_excl_area_r;                       /** Previous exclusion area radius. */
    /* 0x70  */ VC_WATCH_MV_PARAM         user_watch_mv_prm;
    /* 0x7C  */ VECTOR3                   watch_tgt_pos;                             /** Q19.12 | Target look-at position. */
    /* 0x88  */ q19_12                    watch_tgt_max_y;                           /** Max look-at Y offset. */
    /* 0x8C  */ q3_12                     watch_tgt_ang_z;                           /** Target look-at Z angle. */
    /* 0x8E  */ SVECTOR                   cam_mat_ang;                               /** Matrix rotation. */
                // 2 bytes of padding.
    /* 0x98  */ MATRIX                    cam_mat;                                   /** Matrix. */
    /* 0xB8  */ SVECTOR                   ofs_cam_ang;                               /** Offset rotation. */
    /* 0xC0  */ SVECTOR                   ofs_cam_ang_spd;                           /** Offset rotational speed. */
    /* 0xC8  */ SVECTOR                   base_cam_ang;                              /** Base rotation. */
    /* 0xD0  */ s8                        unk_D0[8];                                 // TODO: Possibly unused or debug data?
    /* 0xD8  */ u8                        updateLookAtPoint;                         /** `bool` */
                // 3 bytes of padding.
    /* 0xDC  */ MATRIX                    lookAtMat;
    /* 0xFC  */ u8                        updateLookAtMat;                           /** `bool` */
                // 1 byte of padding.
    /* 0xFE  */ q3_12                     cam_chara2ideal_ang_y;
    /* 0x100 */ VECTOR3                   cam_tgt_velo;                              /** Target velocity. */
    /* 0x10C */ q3_12                     cam_tgt_mv_ang_y;                          /** Target Y angles. */
                // 2 bytes of padding.
    /* 0x110 */ q19_12                    cam_tgt_spd;                               /** Target speed. */
    /* 0x114 */ VECTOR3                   chara_pos;                                 /** Q19.12 | Locked-on character position. */
    /* 0x120 */ q19_12                    chara_bottom_y;                            /** Locked-on character bottom height. */
    /* 0x124 */ q19_12                    chara_top_y;                               /** Locked-on character top height. */
    /* 0x128 */ q19_12                    chara_center_y;                            /** Locked-on character center height. */
    /* 0x12C */ q19_12                    chara_grnd_y;                              /** Locked-on character height from the floor? */
    /* 0x130 */ VECTOR3                   chara_head_pos;                            /** Q19.12 | Locked-on character head position. */
    /* 0x13C */ q19_12                    chara_mv_spd;                              /** Locked-on character movement speed. */
    /* 0x140 */ q3_12                     chara_mv_ang_y;                            /** Locked-on character heading angle. */
    /* 0x142 */ q3_12                     chara_ang_spd_y;                           /** Locked-on character heading angle angular speed. */
    /* 0x144 */ q3_12                     chara_eye_ang_y;                           /** Locked-on character look heading angle. */
    /* 0x146 */ q3_12                     chara_eye_ang_wy;                          /** @unused */
    /* 0x148 */ q19_12                    chara_watch_xz_r;                          /** Locked-on character radius on the XZ plane. */
    /* 0x14C */ VC_NEAR_ROAD_DATA         near_road_ary[CAMERA_PATH_COLL_COUNT_MAX]; /** Nearby camera path collisions. */
    /* 0x2B4 */ q19_12                    near_road_suu;                             /** Count of valid `near_road_ary` entries. */
    /* 0x2B8 */ VC_NEAR_ROAD_DATA         cur_near_road;                             /** Closest camera path? */
    /* 0x2DC */ struct _SubCharacter*     nearest_enemy;                             /** Closest enemy character. */
    /* 0x2E0 */ q19_12                    nearest_enemy_xz_dist;                     /** Distance to the closest enemy character on the XZ plane. */
    /* 0x2E4 */ q19_12                    watch_pos_y;                               /** Guessed name. Camera look-at Y position base? Combined with another value later. */
} VC_WORK;

/** @brief Camera matrix. */
typedef struct _VbRVIEW
{
    /* 0x0  */ VECTOR3        vp;
    /* 0xC  */ VECTOR3        vr;
    /* 0x18 */ s32            rz;
    /* 0x1C */ GsCOORDINATE2* super;
} VbRVIEW;

/** @brief Camera view renderer workspace. */
typedef struct _VW_VIEW_WORK
{
    /* 0x0  */ VbRVIEW       rview;
    /* 0x20 */ GsCOORDINATE2 vwcoord;  /** Look-at point coord. */
    /* 0x70 */ VECTOR3       worldpos; /** Q19.12 | Camera world position. */
    /* 0x7C */ SVECTOR       worldang; /** Q3.12 | Camera world rotation. */
} VW_VIEW_WORK;

/** @brief Camera view cull data. */
typedef struct _CameraCullData
{
    /* 0x0   */ MATRIX  modelMat;
    /* 0x20  */ SVECTOR field_20[8];
    /* 0x60  */ VECTOR3 field_60[8];
    /* 0xC0  */ s32     field_C0;
    /* 0xC4  */ SVECTOR field_C4[10];
    /* 0x114 */ s32     field_114;
    /* 0x118 */ DVECTOR field_118[24];
    /* 0x178 */ s32     field_178;
} s_CameraCullData;

/** @brief 3x3 screen region occupancy flags. */
typedef struct _CameraScreenRegionFlags
{
   /* 0x0 */ u8 flags[3][3]; /** `bool` */
} s_CameraScreenRegionFlags;

#endif
