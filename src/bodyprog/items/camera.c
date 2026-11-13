#include "bodyprog/game.h"
#include "inline_no_dmpsx.h"

#include <psyq/gtemac.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/items/camera.h"
#include "bodyprog/screen/data.h"
#include "bodyprog/items/item_screens.h"
#include "bodyprog/math/math.h"

GsCOORD2PARAM g_ItemScreen_CameraTransform;
s8            g_ItemScreen_PlayerWeaponAttack;

s8 __pad_bss_800C3951[3];

s32 g_ItemScreen_ViewDistance;
s32 g_ItemScreen_GeomOffsetX;
s32 g_ItemScreen_GeomOffsetY;

// In USA, this function is called at the start of `MainLoop`.
// NTSC-J splits this function into 3 and moves calls into the `HP_SAFE1` overlay.
#if VERSION_REGION_IS(NTSC) || VERSION_REGION_IS(PAL)
void ItemScreen_TmdGsFCallInit(void) // 0x8004BB10
{
    // Based on `libgs.h` `jt_init4` function?
    // Only seems to affect TMD rendering which is exclusive to item screen.

    // Gouraud triangle.
    GsFCALL4.g3[GsDivMODE_NDIV][GsLMODE_FOG]  = GsTMDfastG3LFG;

    // Textured gouraud triangle.
    GsFCALL4.tg3[GsDivMODE_NDIV][GsLMODE_FOG] = GsTMDfastTG3LFG;

    // Gouraud quad.
    GsFCALL4.g4[GsDivMODE_NDIV][GsLMODE_FOG]  = GsTMDfastG4LFG;

    // Textured gouraud quad.
    GsFCALL4.tg4[GsDivMODE_NDIV][GsLMODE_FOG] = GsTMDfastTG4LFG;
}
#elif VERSION_REGION_IS(NTSCJ)
bool ItemScreen_TmdGsFCallInitTG3(void) // JPN0 0x8004CB54
{
    GsFCALL4.tg3[GsDivMODE_NDIV][GsLMODE_FOG] = GsTMDfastTG3LFG;
    return false;
}

void ItemScreen_TmdGsFCallInitG3G4(void) // JPN0 0x8004CB6C
{
    GsFCALL4.g3[GsDivMODE_NDIV][GsLMODE_FOG] = GsTMDfastG3LFG;
    GsFCALL4.g4[GsDivMODE_NDIV][GsLMODE_FOG] = GsTMDfastG4LFG;
}

void ItemScreen_TmdGsFCallInitTG4(void) // JPN0 0x8004CB90
{
    GsFCALL4.tg4[GsDivMODE_NDIV][GsLMODE_FOG] = GsTMDfastTG4LFG;
}
#endif

void ItemScreen_CameraSet(VbRVIEW* camView, GsCOORDINATE2* camCoord, SVECTOR3* itemRot, s32 unused1) // 0x8004BB4C
{
    // Set camera view.
    camView->vr.vz = 10;
    camView->vp.vx = 0;
    camView->vp.vy = 0;
    camView->vp.vz = 0;
    camView->vr.vx = 0;
    camView->vr.vy = 0;
    camView->rz    = 0;
    camView->super = camCoord;

    // Set camera coord.
    camCoord->coord.t[2] = Q12(-2.5f);
    camCoord->super      = NULL;
    camCoord->coord.t[0] = Q12(0.0f);
    camCoord->coord.t[1] = Q12(0.0f);

    // Clear item rotation.
    itemRot->vx = Q12_ANGLE(0.0f);
    itemRot->vy = Q12_ANGLE(0.0f);
    itemRot->vz = Q12_ANGLE(0.0f);

    // Set camera transform.
    g_ItemScreen_CameraTransform.scale.vz  = Q12(1.0f);
    g_ItemScreen_CameraTransform.scale.vy  = Q12(1.0f);
    g_ItemScreen_CameraTransform.scale.vx  = Q12(1.0f);
    g_ItemScreen_CameraTransform.rotate.vz = Q12_ANGLE(0.0f);
    g_ItemScreen_CameraTransform.rotate.vy = Q12_ANGLE(0.0f);
    g_ItemScreen_CameraTransform.rotate.vx = Q12_ANGLE(0.0f);
    g_ItemScreen_CameraTransform.trans.vz  = Q12(0.0f);
    g_ItemScreen_CameraTransform.trans.vy  = Q12(0.0f);
    g_ItemScreen_CameraTransform.trans.vx  = Q12(0.0f);

    // Assign transform to camera coord.
    camCoord->param = &g_ItemScreen_CameraTransform;

    // Rotate item.
    ItemScreen_ItemRotate((SVECTOR*)itemRot, camCoord);
    vbSetRefView(camView);
}

void func_8004BBF4(VbRVIEW* camView, GsCOORDINATE2* camCoord, SVECTOR* camRot) // 0x8004BBF4
{
    u16     prevRotX;
    VECTOR  vec;
    SVECTOR sVec;

    prevRotX   = camRot->vx;
    camRot->vx = Q12_ANGLE(0.0f);
    ItemScreen_ItemRotate(camRot, camCoord);
    camRot->vx = prevRotX;
    ItemScreen_ItemRotate(camRot, camCoord);

    sVec.vx = 0;
    sVec.vy = 0;
    sVec.vz = 0;

    gte_ApplyMatrix(&camCoord->coord, &sVec, &vec);
    vbSetRefView(camView);
}

void GameFs_TmdDataAlloc(s32* buf) // 0x8004BCBC
{
    GsMapModelingData((unsigned long*)&buf[1]);
}

void ItemScreen_ItemRotate(const SVECTOR* itemRot, GsCOORDINATE2* itemCoord) // 0x8004BCDC
{
    MATRIX mat;

    mat.t[0] = itemCoord->coord.t[0];
    mat.t[1] = itemCoord->coord.t[1];
    mat.t[2] = itemCoord->coord.t[2];

    Math_RotMatrixZxyNegGte(itemRot, &mat);
    itemCoord->coord = mat;

    ScaleMatrix(&itemCoord->coord, &itemCoord->param->scale);
    itemCoord->flg = false;
}

void func_8004BD74(s32 displayItemIdx, GsDOBJ2* obj, s32 arg2)  // 0x8004BD74
{
    MATRIX viewMat;
    MATRIX localToScreenMat;
    MATRIX worldMat;
    s32    i;
    s32    j;

    Vw_CoordToWorldAndViewMatrices(obj->coord2, &worldMat, &viewMat);
    localToScreenMat = viewMat;

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            viewMat.m[i][j] = Q12(viewMat.m[i][j]) / g_Items_Transforms[displayItemIdx].scale.vx;
        }
    }

    if (arg2 != 3 && displayItemIdx < 7)
    {
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 3; j++)
            {
                viewMat.m[i][j] -= Q12_MULT(viewMat.m[i][j],
                                            Math_Sin(DIV_FAST(g_Items_Coords[displayItemIdx].coord.t[2] + Q8(4.0f), 4)));
            }
        }
    }

    GsSetLightMatrix(&viewMat);
    GsSetLsMatrix(&localToScreenMat);

    if (arg2 == 2)
    {
        GsClearOt(0, 0, &g_OrderingTable1[g_ActiveBufferIdx]);
        GsSortOt(&g_OrderingTable1[g_ActiveBufferIdx], &g_OrderingTable0[g_ActiveBufferIdx]);
        GsSortObject4J(obj, &g_OrderingTable1[g_ActiveBufferIdx], 1, (u32*)PSX_SCRATCH);
    }
    else
    {
        GsSortObject4J(obj, &g_OrderingTable0[g_ActiveBufferIdx], 1, (u32*)PSX_SCRATCH);
    }
}

/** Removing this causes the models of items to appear farther from the camera.
 * `Gfx_Items_ViewPointAdjustment`?
 */
void func_8004BFE8(void) // 0x8004BFE8
{
    // Save constant rotation matrix in stack.
    PushMatrix();

    // Read distance from viewpoint to screen.
    g_ItemScreen_ViewDistance = ReadGeomScreen();

    // Read GTE offset value.
    ReadGeomOffset(&g_ItemScreen_GeomOffsetX, &g_ItemScreen_GeomOffsetY);

    // Set distance between projection plane and viewpoint. Results in FOV change.
    GsSetProjection(1000);

    g_ItemScreen_PlayerWeaponAttack = PLAYER_COMBAT().weaponAttack;
}

/** Possible failsafe?
 * Used when exiting the inventory screen or going into options and map menus
 * through the inventory.
 *
 * Removing it doesn't affect the game.
 *
 * @note From a member of the PS Decomp Discord:
 * "[The function] essentially just tries to restore projection/offset settings
 * and the transform matrix to what they were in the main game, but I'm guessing
 * the game sets them at the start of every frame anyway, so it doesn't really
 * achieve anything."
 */
void func_8004C040(void) // 0x8004C040
{
    // Reset constant rotation matrix from stack.
    PopMatrix();

    GsSetProjection(g_ItemScreen_ViewDistance);
    SetGeomOffset(g_ItemScreen_GeomOffsetX, g_ItemScreen_GeomOffsetY);
}
