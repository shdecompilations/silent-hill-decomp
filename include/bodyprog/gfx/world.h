#ifndef _BODYPROG_GFX_WORLD_H
#define _BODYPROG_GFX_WORLD_H

#include "bodyprog/chara/model.h"
#include "bodyprog/formats/lm.h"
#include "bodyprog/formats/model.h"
#include "bodyprog/gfx/world_object.h"
#include "bodyprog/map/map.h"
#include "bodyprog/view/structs.h"

/** @brief World GFX workspace. */
typedef struct _WorldGfxWork
{
    /* 0x0    */ s_MapInfo*                mapInfo;
    /* 0x4    */ u8                        useStoredPoint; /** `bool` */
    /* 0x5    */ s8                        __pad[3];
    /* 0x8    */ VECTOR3                   ipdSamplePoint; /** Used by IPD logic to sample which chunks to load or unload. */
    /* 0x14   */ u8*                       charaLmBuffer;
    /* 0x18   */ s_CharaModel*             registeredCharaModels[Chara_Count];
    /* 0xCC   */ s_CharaModel              charaModels[CHARA_GROUP_COUNT];
    /* 0x164C */ s_CharaModel              harryModel;
    /* 0x1BAC */ s_HeldItem                heldItem; /** Item held by the player. */
    /* 0x1BD8 */ const s_CollisionTrigger* collisionTriggers;
    /* 0x1BDC */ VC_CAMERA_INTINFO         vcCameraInternalInfo; /** Debug camera info. */
    /* 0x1BE4 */ s_LmHeader                itemLmHdr;
    /* 0x1BF4 */ u8                        itemLmData[4096 - sizeof(s_LmHeader)]; // 4kb allocated for 2.75kb game files.
    /* 0x2BE4 */ s32                       itemLmQueueIdx;
    /* 0x2BE8 */ s32                       objectCount;                     /** `objects` size. */
    /* 0x2BEC */ s_WorldObject             objects[WORLD_OBJECT_COUNT_MAX]; /** World objects to draw. */
} s_WorldGfxWork;
STATIC_ASSERT_SIZEOF(s_WorldGfxWork, 11708);

/** @brief World fog info. */
typedef struct _Fog
{
    /* 0x0  */ s32     nearDistance;
    /* 0x4  */ q23_8   farDistance; // "DrawDistanmce" in SHME, "has no effect when fog is disabled".
    /* 0x8  */ s32     depthShift;  // "FogThing1" from SHME. Affects the distance where fog begins.
    /* 0xC  */ s32     intensity;   // "FogThing2" from SHME. Affects the distance where fog begins.
    /* 0x10 */ CVECTOR color;
} s_Fog;

// Related to `s_PointLight`.
typedef struct
{
    VECTOR3 field_0[2][1];
} s_WorldEnvRenderWork_84;

/** @brief Dynamic point light. */
typedef struct _PointLight
{
    /* 0x0  */ s32               field_0; // Depth for something related about the lighting. The value used here is meant to be used in `gte_lddqa`.
    /* 0x4  */ q3_12             lensFlareIntensity;
    /* 0x8  */ q19_12            intensity;
    /* 0xC  */ SVECTOR           direction; /** Q3.12 | TODO: Also a rotation. */
    /* 0x14 */ VECTOR3           position;  /** Q19.12 */
    /* 0x20 */ SVECTOR           rotation;  /** Q3.12 */
    /* 0x28 */ SVECTOR           field_28;  // Q8 light position for matrix?
    /* 0x30 */ SVECTOR           field_30;  // Light offset?
    /* 0x38 */ s_WorldEnvRenderWork_84 field_38[3];
} s_PointLight;

/** @brief World environment render workspace.
 *
 * Holds fog distances and ramps, lighting and color parameters, water zone references, and other per-map environmental
 * data used for world drawing. Most of it is set each frame from `g_SysWork.worldEnv.current` by `WorldEnv_Apply`.
 */
typedef struct _WorldEnvRenderWork
{
    /* 0x0   */ u8           lightMode;              /** `e_WorldEnvLightMode` */
    /* 0x1   */ u8           isFogEnabled;           /** `bool` */
    /* 0x2   */ u8           enableTintLightOverlap; /** `bool` | Allows overlapping the world tint and volumetric light color.
                                                      * @note Specifically modifies the volumetric light color that the point light generates.
                                                      * The color it applies to the enviroment doesn't change.
                                                      */
    /* 0x3   */ u8           lightLevelMax;          /** Upper limit on the computed light level of lit vertices. */
    /* 0x4   */ s_WaterZone* waterZones;
    /* 0x8   */ s32          screenBrightness;
    /* 0xC   */ s_Fog        fog;
    /* 0x20  */ q19_12       field_20; // Map lighting.
    /* 0x24  */ CVECTOR      field_24; // Character color lighting.
    /* 0x28  */ CVECTOR      worldTintColor;
    /* 0x2C  */ MATRIX       colorMat;
    /* 0x4C  */ s_PointLight light;
    /* 0xCC  */ u8           fogRamp[128]; // Fog-related values based on `fog.nearDistance`/`fog.farDistance`.
    /* 0x14C */ u16          clutY;
} s_WorldEnvRenderWork;

#endif
