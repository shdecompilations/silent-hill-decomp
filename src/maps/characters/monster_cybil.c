#include <psyq/gtemac.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/chara/bone.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player/player.h"

#include "maps/characters/cybil.h"
#include "maps/characters/monster_cybil.h"

// Weird access pattern using `npcs` array instead of function param.
#define monsterCybilChara g_SysWork.npcs[0]
#define monsterCybilProps monsterCybilChara.properties.npc

const s32 D_800CB118[24] =
{
    Q12(18.0f),
    Q12(18.0f),
    Q12(18.0f),
    Q12(18.0f),
    Q12(18.0f),
    Q12(16.0f),
    Q12(16.0f),
    Q12(17.0f),
    Q12(17.0f),
    Q12(18.0f),
    Q12(18.0f),
    Q12(18.0f),
    Q12(18.0f),
    Q12(18.0f),
    Q12(18.0f),
    Q12(18.0f),
    Q12(16.0f),
    Q12(16.0f),
    Q12(17.0f),
    Q12(17.0f),
    Q12(18.0f),
    Q12(18.0f),
    Q12(18.0f),
    Q12(18.0f)
};

const s32 D_800CB178[24] =
{
    Q12(28.0f),
    Q12(28.0f),
    Q12(28.0f),
    Q12(28.0f),
    Q12(28.0f),
    Q12(24.0f),
    Q12(24.0f),
    Q12(26.0f),
    Q12(26.0f),
    Q12(28.0f),
    Q12(28.0f),
    Q12(28.0f),
    Q12(28.0f),
    Q12(28.0f),
    Q12(28.0f),
    Q12(28.0f),
    Q12(24.0f),
    Q12(24.0f),
    Q12(26.0f),
    Q12(26.0f),
    Q12(28.0f),
    Q12(28.0f),
    Q12(28.0f),
    Q12(28.0f)
};

const s16 D_800CB1D8[24] =
{
    Q12(0.8f),
    Q12(0.7f),
    Q12(0.6f),
    Q12(0.5f),
    Q12(0.5f),
    Q12(0.7f),
    Q12(0.7f),
    Q12(0.7f),
    Q12(0.7f),
    Q12(0.7f),
    Q12(0.7f),
    Q12(0.7f),
    Q12(0.6f),
    Q12(0.6f),
    Q12(0.5f),
    Q12(0.5f),
    Q12(0.8f),
    Q12(0.7f),
    Q12(0.7f),
    Q12(0.8f),
    Q12(0.8f),
    Q12(0.8f),
    Q12(0.8f),
    Q12(0.8f)
};

const s16 D_800CB208[24] =
{
    Q12(1.2f),
    Q12(1.0f),
    Q12(0.8f),
    Q12(0.6f),
    Q12(0.6f),
    Q12(1.0f),
    Q12(1.0f),
    Q12(1.0f),
    Q12(1.0f),
    Q12(1.0f),
    Q12(1.0f),
    Q12(1.0f),
    Q12(0.8f),
    Q12(0.8f),
    Q12(0.6f),
    Q12(0.6f),
    Q12(1.2f),
    Q12(1.0f),
    Q12(1.0f),
    Q12(1.2f),
    Q12(1.2f),
    Q12(1.2f),
    Q12(1.2f),
    Q12(1.2f)
};

const s16 D_800CB238[11] =
{
    Q12_ANGLE(0.0f),
    Q12_ANGLE(-11.3f),
    Q12_ANGLE(-14.1f),
    Q12_ANGLE(-5.7f),
    Q12_ANGLE(-2.11f),
    Q12_ANGLE(-2.11f),
    Q12_ANGLE(-2.11f),
    Q12_ANGLE(-5.7f),
    Q12_ANGLE(-4.3f),
    Q12_ANGLE(0.0f),
    Q12_ANGLE(0.0f)
};

const s32 D_800CB250[10] =
{
    Q12(4.0f),
    Q12(6.5f),
    Q12(2.5f),
    Q12(-4.0f),
    Q12(-6.5f),
    Q12(-2.5f),
    Q12(0.8f),
    Q12(3.0f),
    Q12(-0.8f),
    Q12(-3.0f)
};

const s32 D_800CB278[10] =
{
    Q12(5.2f),
    Q12(-0.85f),
    Q12(-6.05f),
    Q12(-5.2f),
    Q12(0.85f),
    Q12(6.05f),
    Q12(3.05f),
    Q12(-0.8f),
    Q12(-3.05f),
    Q12(0.8f)
};

const s16 D_800CB2A0[10] =
{
    Q12_ANGLE(37.6f),
    Q12_ANGLE(97.5f),
    Q12_ANGLE(157.5f),
    Q12_ANGLE(217.6f),
    Q12_ANGLE(277.5f),
    Q12_ANGLE(337.5f),
    Q12_ANGLE(15.1f),
    Q12_ANGLE(105.1f),
    Q12_ANGLE(195.1f),
    Q12_ANGLE(285.1f)
};

const s32 D_800CB2B4[10] =
{
    Q12(3.7764f),
    Q12(6.147f),
    Q12(2.372f),
    Q12(-3.7767f),
    Q12(-6.1473f),
    Q12(-2.3721f),
    Q12(0.9075f),
    Q12(3.3804f),
    Q12(-0.9075f),
    Q12(-3.3804f)
};

const s32 D_800CB2DC[10] =
{
    Q12(4.9163f),
    Q12(-0.807f),
    Q12(-5.728f),
    Q12(-4.9166f),
    Q12(0.8067f),
    Q12(5.7276f),
    Q12(3.3804f),
    Q12(-0.9075f),
    Q12(-3.3804f),
    Q12(0.9075f)
};

const s32 D_800CB304[12][2] =
{
    { Q12(0.0f), Q12(7.0f) },
    { Q12(4.0f), Q12(6.0f) },
    { Q12(6.0f), Q12(4.0f) },
    { Q12(7.0f), Q12(0.5f) },
    { Q12(6.0f), Q12(-4.0f) },
    { Q12(4.0f), Q12(-6.0f) },
    { Q12(0.0f), Q12(-7.0f) },
    { Q12(-4.0f), Q12(-6.0f) },
    { Q12(-6.0f), Q12(-4.0f) },
    { Q12(-7.0f), Q12(-0.5f) },
    { Q12(-6.0f), Q12(4.0f) },
    { Q12(-4.0f), Q12(6.0f) }
};

const s32 D_800CB364[8][2] =
{
    { Q12(0.0f), Q12(4.0f) },
    { Q12(4.0f), Q12(4.0f) },
    { Q12(4.0f), Q12(0.0f) },
    { Q12(4.0f), Q12(-4.0f) },
    { Q12(0.0f), Q12(-4.0f) },
    { Q12(-4.0f), Q12(-4.0f) },
    { Q12(-4.0f), Q12(0.0f) },
    { Q12(-4.0f), Q12(4.0f) }
};

const s32 D_800CB3A4[4][2] =
{
    { Q12(0.0f), Q12(2.5f) },
    { Q12(2.5f), Q12(0.0f) },
    { Q12(0.0f), Q12(-2.5f) },
    { Q12(-2.5f), Q12(0.0f) }
};

void func_800D87B0(s_SubCharacter* monsterCybil) // 0x800D87B0
{
    if (monsterCybil->properties.npc.moveSpeed != Q12(0.0f))
    {
        monsterCybil->properties.npc.moveSpeed -= Q12(0.4f);
        if (monsterCybil->properties.npc.moveSpeed < Q12(0.0f))
        {
            monsterCybil->properties.npc.moveSpeed = Q12(0.0f);
        }
    }

    if (monsterCybil->model.stateStep == 0)
    {
        monsterCybilProps.moveSpeed = Q12(1.5f);
        Model_AnimStatusSet(&monsterCybil->model, 9, false);
    }

    if (monsterCybil->model.anim.keyframeIdx == D_800EA856)
    {
        monsterCybilProps.field_EC    = 1;
        monsterCybil->model.stateStep = 0;
        monsterCybilProps.field_122   = Q12_ANGLE(0.0f);
    }
}

void func_800D8848(s_Model* modelUpper) // 0x800D8848
{
    if (modelUpper->stateStep == 0)
    {
        modelUpper->anim.status = ANIM_STATUS(MonsterCybilAnim_9, false);
        modelUpper->stateStep++;
    }

    if (modelUpper->anim.keyframeIdx == D_800EA856)
    {
        monsterCybilProps.field_EE = 1;
        modelUpper->stateStep      = 0;
    }
}

s32 func_800D8898(s_AnimInfo* animInfo) // 0x800D8898
{
    if (animInfo->status == ANIM_STATUS(MonsterCybilAnim_1, true))
    {
        if (monsterCybilProps.field_EC == 0)
        {
            return 0;
        }

        if (monsterCybilProps.field_116 == 1)
        {
            return D_800CB118[animInfo->startKeyframeIdx];
        }
        else
        {
            return D_800CB178[animInfo->startKeyframeIdx];
        }
    }

    return 0;
}

void MonsterCybil_Update(s_SubCharacter* monsterCybil, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords) // 0x800D8908
{
    s_Model* modelUpper;

    modelUpper = &g_MonsterCybil_ExtraModel;

    if (monsterCybil->model.controlState == MonsterCybilControl_None)
    {
        MonsterCybil_Init(monsterCybil, modelUpper);
    }

    if (g_DeltaTime != Q12(0.0f))
    {
        func_800D8B14(monsterCybil, modelUpper);
        func_800D8D7C(monsterCybil, modelUpper, boneCoords);
        func_800D9790(monsterCybil, modelUpper);
        func_800D99E4(monsterCybil, modelUpper, anmHdr, boneCoords);
        func_800DB4CC(monsterCybil, modelUpper, boneCoords);
        func_800D9AAC(monsterCybil, modelUpper);
    }
}

void MonsterCybil_Init(s_SubCharacter* monsterCybil, s_Model* modelUpper) // 0x800D89CC
{
    monsterCybil->model.controlState++;
    modelUpper->controlState++;
    monsterCybil->model.stateStep = 0;
    modelUpper->stateStep         = 0;

    monsterCybil->collision.state   = CharaCollisionState_Npc;
    monsterCybilProps.field_11A     = Q12_ANGLE(90.0f);
    monsterCybil->model.anim.flags |= AnimFlag_Visible | AnimFlag_Unlocked;
    monsterCybil->flags            |= CharaFlag_NoRadioStatic | CharaFlag_Hit;

    sharedData_800D16E4_2_s01 = 0;
    modelUpper->anim.flags   |= AnimFlag_Visible | AnimFlag_Unlocked;
    monsterCybil->health      = Q12(4000.0f);

    monsterCybilProps.field_104 = 10;
    monsterCybilProps.field_FE  = 0;
    monsterCybilProps.field_106 = Q12(0.0f);
    monsterCybilProps.freeze  = Q12(0.0f);
    monsterCybilProps.field_108 = Q12(0.0f);
    monsterCybilProps.field_10C = Q12(0.0f);
    monsterCybilProps.resetControlState  = Q12(0.0f);
    monsterCybilProps.field_116 = 0;
    monsterCybilProps.field_F4  = 0;

    WorldGfx_CharaMeshSwap(Chara_MonsterCybil, MESH_SWAP_STATUS(1, 1));
}

void func_800D8A90(s_SubCharacter* monsterCybil) // 0x800D8A90
{
    monsterCybil->collision.cylinder.outerRadius = Q12(0.3f);
    monsterCybil->collision.cylinder.innerRadius = DEFAULT_PLAYER_CYLINDER_INNER_RADIUS;
    monsterCybil->collision.box.topExtent        = DEFAULT_PLAYER_BOX_TOP;
    monsterCybil->collision.box.offsetY          = Q12(-1.05f);
    monsterCybil->collision.box.field_8          = Q12(-1.45f);

    monsterCybil->health = Q12(4000.0f);

    monsterCybil->collision.box.bottomExtent         = Q12(0.0f);
    monsterCybil->collision.box.height               = Q12(0.0f);
    monsterCybil->collision.shapeOffsets.cylinder.vz = Q12(0.0f);
    monsterCybil->collision.shapeOffsets.cylinder.vx = Q12(0.0f);
    monsterCybil->collision.shapeOffsets.box.vz      = Q12(0.0f);
    monsterCybil->collision.shapeOffsets.box.vx      = Q12(0.0f);

    monsterCybil->model.stateStep = 0;

    monsterCybilProps.field_115 = 0;

    g_MonsterCybil_ExtraModel.stateStep = 0;

    monsterCybilProps.field_104 = 10;
    monsterCybilProps.field_FE  = 0;
    monsterCybilProps.field_106 = Q12(0.0f);
    monsterCybilProps.freeze  = 0;
    monsterCybilProps.field_108 = Q12(0.0f);
    monsterCybilProps.field_10C = Q12(0.0f);
    monsterCybilProps.resetControlState  = Q12(0.0f);
}

void func_800D8B14(s_SubCharacter* monsterCybil, s_Model* modelUpper) // 0x800D8B14
{
    #define playerChara g_SysWork.playerWork.player

    u8  sp10;
    s16 sp12;

    if (monsterCybilProps.resetControlState != Q12(0.0f))
    {
        monsterCybilProps.resetControlState -= g_DeltaTime * 15;
    }

    monsterCybilProps.resetControlState = CLAMP(monsterCybilProps.resetControlState, Q12(0.0f), INT_MAX);

    Player_DisableDamage(&sp10, false);

    if (sp10 != 0)
    {
        return;
    }

    if (monsterCybilProps.controlState < MonsterCybilControl_13)
    {
        if (monsterCybilProps.controlState > MonsterCybilControl_2)
        {
            return;
        }
    }

    if (monsterCybilProps.field_EE == 9 ||
        monsterCybilProps.field_EE == 11)
    {
        monsterCybil->damage.amount = Q12(0.0f);
    }
    else
    {
        if (monsterCybilProps.field_EE != 12)
        {
            monsterCybil->damage.amount = monsterCybil->damage.amount >> 1;
        }

        if (monsterCybil->damage.amount != Q12(0.0f))
        {
            monsterCybilProps.resetControlState += monsterCybil->damage.amount;

            monsterCybil->health       -= monsterCybil->damage.amount;
            monsterCybil->damage.amount = Q12(0.0f);

            monsterCybilProps.field_122 = Q12_ANGLE_NORM_U(
                ratan2(Q12_TO_Q8(monsterCybil->position.vx - playerChara.position.vx),
                       Q12_TO_Q8(monsterCybil->position.vz - playerChara.position.vz)) +
                Q12(1.0f));

            Math_ShortestAngleGet(monsterCybil->rotation.vy, monsterCybilProps.field_122, &sp12);

            monsterCybilProps.field_122 = sp12;

            if (monsterCybil->health <= Q12(0.0f))
            {
                Player_DisableDamage(&sp10, true);
                Savegame_EventFlagSet(EventFlag_446);
                func_8004C564(0, NO_VALUE);
            }
            else
            {
                if (monsterCybilProps.resetControlState >= Q12(800.0f))
                {
                    if (monsterCybilProps.field_EC != 12)
                    {
                        monsterCybilProps.field_EC    = 9;
                        monsterCybil->model.stateStep = 0;
                    }
                }

                if (monsterCybilProps.resetControlState >= Q12(800.0f) ||
                    monsterCybilProps.field_EE == 12)
                {
                    monsterCybilProps.field_EE = 9;
                    modelUpper->stateStep      = 0;
                    monsterCybilProps.resetControlState = Q12(0.0f);
                }
            }

            monsterCybilProps.field_116 = 0;
            monsterCybilProps.field_10C = Q12(0.0f);
        }
    }

    #undef playerChara
}

void func_800D8D7C(s_SubCharacter* monsterCybil, s_Model* modelUpper, GsCOORDINATE2* boneCoords) // 0x800D8D7C
{
    enum _SfxId sfxId;
    s8          pitch0;
    s8          pitch1;
    q19_12      angle0;
    q19_12      moveSpeed;

    #define playerChara g_SysWork.playerWork.player

    monsterCybilProps.distanceToPlayer = Math_Vector2MagCalcSafeQ8(playerChara.position.vx - monsterCybil->position.vx,
                                                                   playerChara.position.vz - monsterCybil->position.vz);

    monsterCybilProps.field_120 = func_800DB6FC(&monsterCybil->position);
    if (monsterCybilProps.field_120)
    {
        monsterCybilProps.field_108 += g_DeltaTime;
    }
    else
    {
        monsterCybilProps.field_108 = Q12(0.0f);
    }

    monsterCybilProps.field_106++;

    switch (monsterCybilProps.controlState)
    {
        case MonsterCybilControl_None:
            func_800D9AB4(monsterCybil, modelUpper, boneCoords);
            break;

        case MonsterCybilControl_1:
            func_800DA9C8(monsterCybil, modelUpper, boneCoords);
            break;

        case MonsterCybilControl_4:
            monsterCybilProps.field_11A = Q12_ANGLE(90.0f);

            Model_AnimStatusKeyframeSet(monsterCybil->model, 4, true, MONSTER_CYBIL_ANIM_INFOS, 0);
            Model_AnimStatusKeyframeSet(*modelUpper, 4, true, MONSTER_CYBIL_ANIM_INFOS, 0);

            if (monsterCybil->model.anim.keyframeIdx >= 66)
            {
                WorldGfx_CharaMeshSwap(Chara_MonsterCybil, MESH_SWAP_STATUS(CybilSwappableMesh_None, CybilVariantMesh_1));
            }
            else
            {
                WorldGfx_CharaMeshSwap(Chara_MonsterCybil, MESH_SWAP_STATUS(CybilSwappableMesh_None, CybilVariantMesh_2));
            }

            monsterCybilProps.moveSpeed = Q12(0.0f);

            if (Savegame_EventFlagGet(EventFlag_445))
            {
                monsterCybilProps.field_F4 = 1;
                monsterCybilProps.controlState = 1;
                monsterCybilProps.field_EC = 1;
                monsterCybilProps.field_EE = 1;

                monsterCybil->model.stateStep = 0;
                modelUpper->stateStep         = 0;
            }
            break;

        case MonsterCybilControl_5:
            if (monsterCybil->model.stateStep == 0)
            {
                modelUpper->stateStep = 0;
                Model_AnimStatusKeyframeSet(monsterCybil->model, 14, true, MONSTER_CYBIL_ANIM_INFOS, 0);
            }

            Model_AnimStatusKeyframeSet(*modelUpper, 14, true, MONSTER_CYBIL_ANIM_INFOS, 0);
            monsterCybilProps.moveSpeed = Q12(0.0f);
            Chara_AnimStateReset(monsterCybil);
            break;

        case MonsterCybilControl_6:
            if (!monsterCybil->model.stateStep)
            {
                modelUpper->stateStep = 0;
                Model_AnimStatusSet(&monsterCybil->model, 15, false);
            }

            Model_AnimStatusSet(modelUpper, 15, false);
            monsterCybilProps.moveSpeed = Q12(0.0f);
            Chara_AnimStateReset(monsterCybil);

            if (monsterCybil->model.anim.keyframeIdx == 343)
            {
                monsterCybilProps.controlState = MonsterCybilControl_12;
                monsterCybil->model.stateStep  = 0;
                modelUpper->stateStep          = 0;
            }
            break;

        case MonsterCybilControl_12:
            Model_AnimStatusKeyframeSet(monsterCybil->model, 21, true, MONSTER_CYBIL_ANIM_INFOS, 0);
            Model_AnimStatusKeyframeSet(*modelUpper, 21, true, MONSTER_CYBIL_ANIM_INFOS, 0);

            monsterCybilProps.moveSpeed = Q12(0.0f);
            Chara_AnimStateReset(monsterCybil);
            break;

        case MonsterCybilControl_9:
            if (!monsterCybil->model.stateStep)
            {
                modelUpper->stateStep = 0;
                Model_AnimStatusKeyframeSet(monsterCybil->model, 20, true, MONSTER_CYBIL_ANIM_INFOS, 0);
            }

            Model_AnimStatusKeyframeSet(*modelUpper, 20, true, MONSTER_CYBIL_ANIM_INFOS, 0);
            WorldGfx_CharaMeshSwap(Chara_MonsterCybil, MESH_SWAP_STATUS(CybilSwappableMesh_None, CybilVariantMesh_1));
            Chara_AnimStateReset(monsterCybil);
            break;

        case MonsterCybilControl_10:
            if (!monsterCybil->model.stateStep)
            {
                modelUpper->stateStep = 0;
                Model_AnimStatusKeyframeSet(monsterCybil->model, 18, true, MONSTER_CYBIL_ANIM_INFOS, 0);
            }

            Model_AnimStatusKeyframeSet(*modelUpper, 18, true, MONSTER_CYBIL_ANIM_INFOS, 0);

            if (modelUpper->anim.keyframeIdx >= 29)
            {
                WorldGfx_CharaMeshSwap(Chara_MonsterCybil, MESH_SWAP_STATUS(CybilSwappableMesh_None, CybilVariantMesh_2));
            }
            else
            {
                WorldGfx_CharaMeshSwap(Chara_MonsterCybil, MESH_SWAP_STATUS(CybilSwappableMesh_None, CybilVariantMesh_1));
            }

            Chara_AnimStateReset(monsterCybil);
            break;

        case MonsterCybilControl_11:
            if (monsterCybil->model.stateStep == 0)
            {
                modelUpper->stateStep = 0;
                Model_AnimStatusKeyframeSet(monsterCybil->model, 19, true, MONSTER_CYBIL_ANIM_INFOS, 0);
            }

            Model_AnimStatusKeyframeSet(*modelUpper, 19, true, MONSTER_CYBIL_ANIM_INFOS, 0);
            WorldGfx_CharaMeshSwap(Chara_MonsterCybil, MESH_SWAP_STATUS(CybilSwappableMesh_None, CybilVariantMesh_2));
            Chara_AnimStateReset(monsterCybil);

            if (!monsterCybil->model.stateStep)
            {
                modelUpper->stateStep = 0;
            }
            break;

        case MonsterCybilControl_7:
            if (!monsterCybil->model.stateStep)
            {
                modelUpper->stateStep = 0;
                Model_AnimStatusKeyframeSet(monsterCybil->model, 16, true, MONSTER_CYBIL_ANIM_INFOS, 0);
            }

            Model_AnimStatusKeyframeSet(*modelUpper, 16, true, MONSTER_CYBIL_ANIM_INFOS, 0);
            break;

        case MonsterCybilControl_8:
            if (!monsterCybil->model.stateStep)
            {
                modelUpper->stateStep = 0;
                Model_AnimStatusKeyframeSet(monsterCybil->model, 17, true, MONSTER_CYBIL_ANIM_INFOS, 0);
            }

            Model_AnimStatusKeyframeSet(*modelUpper, 17, true, MONSTER_CYBIL_ANIM_INFOS, 0);
            break;
    }

    Player_FootstepSfxGet(11, &sfxId, &pitch0, &pitch1);

    switch (monsterCybilProps.controlState)
    {
        case MonsterCybilControl_9:
            Npc_FootstepSoundPlay(41, monsterCybil, 5, 16, sfxId, pitch0);
            break;

        case MonsterCybilControl_10:
            Npc_SfxPlay(37, monsterCybil, 29, Sfx_Unk1620);
            break;

        case MonsterCybilControl_11:
            if (monsterCybil->model.anim.keyframeIdx == 38)
            {
                func_8006342C(EquippedWeaponId_Unk63, Q12_ANGLE(90.0f), monsterCybil->rotation.vy, g_SysWork.npcBoneCoordBuffer);
            }

            Npc_SfxPlay(39, monsterCybil, 38, Sfx_Unk1622);
            break;

        case MonsterCybilControl_4:
            Npc_SfxPlay(9, monsterCybil, 69, Sfx_Unk1621);
            break;

        case MonsterCybilControl_5:
            if (monsterCybil->model.anim.keyframeIdx < 312)
            {
                Npc_SfxPlay(29, monsterCybil, 289, Sfx_Unk1632);
            }
            else
            {
                Npc_SfxPlay(29, monsterCybil, 313, Sfx_Unk1631);
            }
            break;
    }

    if (monsterCybilProps.field_EC == 1)
    {
        Npc_FootstepSoundPlay(3, monsterCybil, 5, 16, sfxId, pitch0);
    }

    switch (monsterCybilProps.field_EE)
    {
        case 2:
            if (ANIM_STATUS_IS_ACTIVE(modelUpper->anim.status) && modelUpper->anim.keyframeIdx >= 29 && !sharedData_800D16E4_2_s01)
            {
                Sfx_WithFlagsPlay(Sfx_Unk1620, &monsterCybil->position, Q8(0.25f), SfxFlag_None);
                sharedData_800D16E4_2_s01 = 1;
            }
            else if (modelUpper->anim.keyframeIdx < 29)
            {
                sharedData_800D16E4_2_s01 = 0;
            }
            break;

        case 3:
            if (ANIM_STATUS_IS_ACTIVE(modelUpper->anim.status) && modelUpper->anim.keyframeIdx >= 38 && !sharedData_800D16E4_2_s01)
            {
                Sfx_WithFlagsPlay(Sfx_Unk1622, &monsterCybil->position, Q8(0.25f), SfxFlag_None);
                sharedData_800D16E4_2_s01 = 1;
            }

            else if (modelUpper->anim.keyframeIdx < 38)
            {
                sharedData_800D16E4_2_s01 = 0;
            }
            break;

        case 4:
            break;
    }

    moveSpeed                  = monsterCybilProps.moveSpeed;
    angle0                     = monsterCybilProps.field_122;
    monsterCybil->headingAngle = Q12_ANGLE_ABS(angle0 + monsterCybil->rotation.vy);

    monsterCybil->moveSpeed  = moveSpeed;
    monsterCybil->fallSpeed += g_GravitySpeed;

    boneCoords->flg = false;
    Math_RotMatrixZxyNegGte(&monsterCybil->rotation, &boneCoords->coord);

    #undef playerChara
}

void func_800D9790(s_SubCharacter* monsterCybil, s_Model* modelUpper) // 0x800D9790
{
    s_CollisionResult collResult;
    VECTOR3           pos;    // Q19.12
    VECTOR3           offset; // Q19.12
    q19_12            headingAngle;
    q19_12            moveSpeed;
    s32               shift0;
    s32               shift1;

    pos          = monsterCybil->position;
    headingAngle = monsterCybil->headingAngle;

    moveSpeed = Q12_MULT_PRECISE(monsterCybil->moveSpeed, g_DeltaTime);
    shift0 = (moveSpeed < -0x7FFF || moveSpeed > 0x7FFF) * 4;
    shift1 = shift0 >> 1;

    offset.vx = Q12_MULT_PRECISE(moveSpeed >> shift1, Math_Sin(headingAngle) >> shift1) << shift0;
    offset.vz = Q12_MULT_PRECISE(moveSpeed >> shift1, Math_Cos(headingAngle) >> shift1) << shift0;
    offset.vy = Q12_MULT_PRECISE(monsterCybil->fallSpeed, g_DeltaTime);

    switch (monsterCybilProps.field_EC)
    {
        case 11:
            if (ANIM_STATUS_IS_ACTIVE(monsterCybil->model.anim.status))
            {
                monsterCybil->collision.cylinder.outerRadius = Q12(0.3f) - (((monsterCybil->model.anim.keyframeIdx - 216) * Q12(3.0f)) / 100);
            }

            Collision_WallDetect(&collResult, &offset, monsterCybil);

            monsterCybil->position.vx += collResult.offset.vx;
            monsterCybil->position.vz += collResult.offset.vz;
            monsterCybil->fallSpeed    = Q12(0.0f);
            break;

        case 12:
            monsterCybil->collision.cylinder.outerRadius = Q12(0.0f);
            monsterCybil->position.vx                   += offset.vx;
            monsterCybil->position.vz                   += offset.vz;
            monsterCybil->fallSpeed                      = Q12(0.0f);
            break;

        default:
            monsterCybil->collision.cylinder.outerRadius     = Q12(0.3f);
            monsterCybil->collision.shapeOffsets.cylinder.vz = Q12(0.0f);
            monsterCybil->collision.shapeOffsets.cylinder.vx = Q12(0.0f);
            monsterCybil->collision.shapeOffsets.box.vz      = Q12(0.0f);
            monsterCybil->collision.shapeOffsets.box.vx      = Q12(0.0f);

            Collision_WallDetect(&collResult, &offset, monsterCybil);

            monsterCybil->position.vx += collResult.offset.vx;
            monsterCybil->position.vy  = Q12(0.0f);
            monsterCybil->fallSpeed    = Q12(0.0f);
            monsterCybil->position.vz += collResult.offset.vz;
            break;
    }
}

void func_800D99E4(s_SubCharacter* monsterCybil, s_Model* modelUpper, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords) // 0x800D99E4
{
    s_AnimInfo* animInfo;

    boneCoords[MonsterCybilBone_Root].coord.t[0] = Q12_TO_Q8(monsterCybil->position.vx);
    boneCoords[MonsterCybilBone_Root].coord.t[1] = Q12_TO_Q8(monsterCybil->position.vy);
    boneCoords[MonsterCybilBone_Root].coord.t[2] = Q12_TO_Q8(monsterCybil->position.vz);

    // TODO: Cybil has same skeleton structure as Harry? Check and make separate mask macros just for Cybil.
    anmHdr->activeBoneFlags = HARRY_LOWER_BODY_BONE_MASK;

    animInfo = &MONSTER_CYBIL_ANIM_INFOS[monsterCybil->model.anim.status];
    animInfo->playbackFunc(&monsterCybil->model, anmHdr, boneCoords, animInfo);

    anmHdr->activeBoneFlags = HARRY_UPPER_BODY_BONE_MASK;

    animInfo = &MONSTER_CYBIL_ANIM_INFOS[modelUpper->anim.status];
    animInfo->playbackFunc(modelUpper, anmHdr, boneCoords, animInfo);
}

void func_800D9AAC(s_SubCharacter* monsterCybil, s_Model* modelUpper) {}                         // 0x800D9AAC

void func_800D9AB4(s_SubCharacter* monsterCybil, s_Model* modelUpper, GsCOORDINATE2* boneCoords) // 0x800D9AB4
{
    s16 temp_v0;

    #define playerChara g_SysWork.playerWork.player

    switch (monsterCybilProps.field_EC)
    {
        case 0:
            break;

        case 1:
            if (monsterCybil->model.stateStep == 0)
            {
                monsterCybil->model.anim.status = ANIM_STATUS(MonsterCybilAnim_1, false);
                monsterCybil->model.stateStep++;
            }

            func_800DC018(monsterCybil);

            if (playerChara.health > Q12(0.0f) && monsterCybilProps.field_116 == 0)
            {
                if (monsterCybilProps.distanceToPlayer > Q12(0.6f))
                {
                    func_800DB748(monsterCybil);
                }

                monsterCybilProps.moveSpeed = Q12(0.0f);

                if (monsterCybil->model.anim.status == 3)
                {
                    if ((u16)monsterCybil->model.anim.keyframeIdx < 24)
                    {
                        monsterCybilProps.moveSpeed = D_800CB208[monsterCybil->model.anim.keyframeIdx];
                    }
                }

                if (monsterCybilProps.field_120 == 0)
                {
                    monsterCybilProps.moveSpeed <<= 1;

                    if (monsterCybilProps.field_106 > 60)
                    {
                        monsterCybilProps.field_106 = 0;

                        if ((Rng_Rand16() % 10) > 4 && monsterCybilProps.distanceToPlayer > Q12(4.0f))
                        {
                            monsterCybilProps.field_FE = func_800DB930();

                            if (monsterCybilProps.field_FE != NO_VALUE)
                            {
                                monsterCybilProps.field_EC    = 12;
                                monsterCybilProps.field_EE    = 12;
                                monsterCybil->model.stateStep = 0;
                                modelUpper->stateStep         = 0;
                            }
                        }
                        else
                        {
                            func_800DBA48(monsterCybil);
                        }
                    }
                }

                if (monsterCybilProps.field_104 <= 0 || monsterCybil->health < Q12(2000.0f))
                {
                    Savegame_EventFlagSet(EventFlag_444);

                    monsterCybilProps.controlState = MonsterCybilControl_3;
                    monsterCybil->model.stateStep  = 0;
                    modelUpper->stateStep          = 0;
                    monsterCybilProps.field_F4     = 0;
                    return;
                }
            }
            else if (monsterCybilProps.field_116 == 1)
            {
                func_800DBE5C(monsterCybil);

                if (monsterCybilProps.field_104 <= 0 || monsterCybil->health < Q12(2000.0f))
                {
                    Savegame_EventFlagSet(EventFlag_444);

                    monsterCybilProps.controlState = MonsterCybilControl_3;
                    monsterCybil->model.stateStep  = 0;
                    modelUpper->stateStep          = 0;
                    monsterCybilProps.field_F4     = 0;
                    return;
                }
            }
            else
            {
                monsterCybilProps.moveSpeed = Q12(0.0f);
                if (monsterCybil->model.anim.status == 3)
                {
                    if ((u16)monsterCybil->model.anim.keyframeIdx < 24)
                    {
                        monsterCybilProps.moveSpeed = D_800CB208[monsterCybil->model.anim.keyframeIdx];
                    }
                }
            }

            monsterCybilProps.field_122 = Q12_ANGLE(0.0f);
            break;

        case 2:
            if (monsterCybil->model.stateStep == 0)
            {
                monsterCybil->model.anim.status = 4;
                monsterCybil->model.stateStep++;
            }

            monsterCybilProps.moveSpeed = Q12(0.0f);
            break;

        case 3:
            if (monsterCybil->model.stateStep == 0)
            {
                monsterCybil->model.anim.status = 6;
                monsterCybil->model.stateStep++;
            }

            monsterCybilProps.moveSpeed = Q12(0.0f);
            break;

        case 8:
            monsterCybilProps.moveSpeed = Q12(0.0f);

            if (monsterCybil->model.stateStep == 0)
            {
                monsterCybil->model.anim.status = 16;
                monsterCybil->model.stateStep++;
            }

            if (monsterCybil->model.anim.keyframeIdx == D_800EA836)
            {
                monsterCybilProps.field_EC    = 1;
                monsterCybil->model.stateStep = 0;
            }
            break;

        case 9:
            func_800D87B0(monsterCybil);
            break;

        case 10:
            if (monsterCybil->model.stateStep == 0)
            {
                monsterCybil->model.anim.status = 20;
                monsterCybil->model.stateStep++;
            }

            monsterCybilProps.moveSpeed = Q12(0.0f);
            break;

        case 11:
            if (monsterCybil->model.stateStep == 0)
            {
                monsterCybil->model.anim.status = 23;
                monsterCybil->model.stateStep++;
                monsterCybil->model.anim.time        = FP_TO(MONSTER_CYBIL_ANIM_INFOS[23].endKeyframeIdx, Q12_SHIFT);
                monsterCybil->model.anim.keyframeIdx = MONSTER_CYBIL_ANIM_INFOS[23].endKeyframeIdx;
            }

            if (monsterCybil->properties.npc.moveSpeed != Q12(0.0f))
            {
                monsterCybil->properties.npc.moveSpeed = monsterCybil->properties.npc.moveSpeed - Q12(0.08f);
                if (monsterCybil->properties.npc.moveSpeed < Q12(0.0f))
                {
                    monsterCybil->properties.npc.moveSpeed = Q12(0.0f);
                }
            }

            if (monsterCybil->model.anim.keyframeIdx == (D_800EA896 - 4) || monsterCybil->model.anim.keyframeIdx == (D_800EA896 - 5))
            {
                monsterCybilProps.moveSpeed = Q12(0.6f);
            }

            if (monsterCybil->model.anim.keyframeIdx <= (D_800EA896 - 7))
            {
                monsterCybil->position.vy += g_GravitySpeed >> 2;
            }
            else if ((D_800EA896 - 4) >= monsterCybil->model.anim.keyframeIdx)
            {
                monsterCybil->position.vy += g_GravitySpeed >> 4;
            }
            else
            {
                monsterCybil->position.vy = monsterCybilProps.field_100 - Q12(0.45f);
            }

            if (monsterCybil->position.vy > Q12(0.0f))
            {
                monsterCybil->position.vy = Q12(0.0f);
            }

            monsterCybil->rotation.vy = (monsterCybilProps.field_FE < 6) ? (D_800CB2A0[monsterCybilProps.field_FE] - Q12_ANGLE(180.0f)) : D_800CB2A0[monsterCybilProps.field_FE];

            temp_v0 = 1;

            if (monsterCybil->model.anim.keyframeIdx == D_800EA894)
            {
                monsterCybilProps.field_EC    = temp_v0;
                monsterCybil->model.stateStep = 0;
            }
            break;

        case 12:
            monsterCybil->position.vx = D_800CB2B4[monsterCybilProps.field_FE] + Q12(20.0f);
            monsterCybil->position.vy = monsterCybilProps.field_100 - Q12(0.45f);
            monsterCybil->position.vz = D_800CB2DC[monsterCybilProps.field_FE] + Q12(100.0f);

            monsterCybil->rotation.vy = (monsterCybilProps.field_FE < 6) ? (D_800CB2A0[monsterCybilProps.field_FE] - Q12_ANGLE(180.0f)) : D_800CB2A0[monsterCybilProps.field_FE];

            if (monsterCybil->model.anim.keyframeIdx >= MONSTER_CYBIL_ANIM_INFOS[25].startKeyframeIdx &&
                MONSTER_CYBIL_ANIM_INFOS[25].endKeyframeIdx >= monsterCybil->model.anim.keyframeIdx)
            {
                monsterCybil->collision.shapeOffsets.box.vx = Q12_MULT(Math_Sin(monsterCybil->rotation.vy), Q12(-0.46f));
                monsterCybil->collision.shapeOffsets.box.vz = Q12_MULT(Math_Cos(monsterCybil->rotation.vy), Q12(-0.46f));
            }

            if (monsterCybil->model.stateStep == 0)
            {
                monsterCybil->model.anim.status = ANIM_STATUS(MonsterCybilAnim_12, false);
                monsterCybil->model.stateStep++;
            }

            monsterCybilProps.moveSpeed = Q12(0.0f);
            break;
    }

    switch (monsterCybilProps.field_EE)
    {
        case 0:
            break;

        case 1:
            if (modelUpper->stateStep == 0)
            {
                modelUpper->anim.status = ANIM_STATUS(MonsterCybilAnim_1, false);
                modelUpper->stateStep++;
            }

            if (modelUpper->anim.status == ANIM_STATUS(MonsterCybilAnim_1, true))
            {
                modelUpper->anim.time        = monsterCybil->model.anim.time;
                modelUpper->anim.keyframeIdx = monsterCybil->model.anim.keyframeIdx;
            }

            if (playerChara.health > 0 && monsterCybilProps.field_116 == 0)
            {
                if (monsterCybilProps.distanceToPlayer < Q12(0.8f) &&
                    (g_SysWork.playerWork.extra.state > 50 || g_SysWork.playerWork.extra.state < 47))
                {
                    if (monsterCybilProps.field_EE == 1)
                    {
                        monsterCybilProps.field_EC    = 8;
                        monsterCybilProps.field_EE    = 8;
                        monsterCybil->model.stateStep = 0;
                        modelUpper->stateStep         = 0;
                    }
                }
                else if (monsterCybilProps.field_108 >= Q12(3.0f) &&
                         (!func_800DB81C(monsterCybil) || g_SysWork.playerWork.extra.state == 47 ||
                          g_SysWork.playerWork.extra.state == 48))
                {
                    if (monsterCybilProps.distanceToPlayer <= Q12(4.0f))
                    {
                        monsterCybilProps.field_EC    = 2;
                        monsterCybil->model.stateStep = 0;
                    }

                    monsterCybilProps.field_EE = 2;
                    modelUpper->stateStep      = 0;
                }
            }
            break;

        case 2:
            if (modelUpper->stateStep == 0)
            {
                monsterCybilProps.freeze = Q12(0.0f);
                D_800EBB48                 = Rng_Rand16() % Q12(1.0f);

                if (modelUpper->stateStep == 0)
                {
                    modelUpper->anim.status = ANIM_STATUS(MonsterCybilAnim_2, false);
                    modelUpper->stateStep++;
                }
            }

            func_800DB748(monsterCybil);

            monsterCybilProps.freeze += g_DeltaTime;

            D_800ED570.vx = Q8_TO_Q12(boneCoords[10].workm.t[0]);
            D_800ED570.vy = Q8_TO_Q12(boneCoords[10].workm.t[1]);
            D_800ED570.vz = Q8_TO_Q12(boneCoords[10].workm.t[2]);

            if (modelUpper->anim.keyframeIdx >= 29)
            {
                WorldGfx_CharaMeshSwap(Chara_MonsterCybil, MESH_SWAP_STATUS(CybilSwappableMesh_None, CybilVariantMesh_2));
            }

            if (modelUpper->anim.keyframeIdx == D_800EA776 && D_800EBB48 < monsterCybilProps.freeze)
            {
                if (monsterCybilProps.field_120 != 0 &&
                    !(playerChara.flags & (1 << 3)))
                {
                    monsterCybilProps.field_114 = 0;
                    monsterCybilProps.field_EE  = 3;
                    modelUpper->stateStep       = 0;

                    if (monsterCybilProps.field_EC == 2)
                    {
                        monsterCybilProps.field_EC    = 3;
                        monsterCybil->model.stateStep = 0;
                    }
                }
                else
                {
                    monsterCybilProps.field_EE = 10;
                    modelUpper->stateStep      = 0;

                    if (monsterCybilProps.field_EC != 1)
                    {
                        monsterCybilProps.field_EC    = 10;
                        monsterCybil->model.stateStep = 0;
                    }
                }
            }
            break;

        case 3:
            if (modelUpper->stateStep == 0)
            {
                monsterCybilProps.freeze  = Q12(0.0f);
                monsterCybilProps.field_108 = Q12(0.0f);
                monsterCybilProps.field_104--;
                monsterCybilProps.field_114++;

                D_800EBB4A = Rng_Rand16() % Q12(1.0f);
            }

            D_800ED570.vx = Q8_TO_Q12(boneCoords[10].workm.t[0]);
            D_800ED570.vy = Q8_TO_Q12(boneCoords[10].workm.t[1]);
            D_800ED570.vz = Q8_TO_Q12(boneCoords[10].workm.t[2]);

            if (modelUpper->anim.keyframeIdx >= (MONSTER_CYBIL_ANIM_INFOS[7].startKeyframeIdx + 2) &&
                (MONSTER_CYBIL_ANIM_INFOS[7].startKeyframeIdx + 4) >= modelUpper->anim.keyframeIdx &&
                monsterCybilProps.field_115 == 0)
            {
                monsterCybil->model.anim.baseAnimInfos = MONSTER_CYBIL_ANIM_INFOS;
                monsterCybil->field_44.field_0         = 1;

                // TODO: What's weapon attack 63?
                func_8006342C(EquippedWeaponId_Unk63, monsterCybilProps.field_11A, monsterCybil->rotation.vy, g_SysWork.npcBoneCoordBuffer);

                if (func_8008A0E4(monsterCybil->field_44.field_0,
                                  WEAPON_ATTACK(EquippedWeaponId_Unk63, AttackInputType_Tap),
                                  monsterCybil,
                                  &D_800ED570,
                                  &playerChara,
                                  monsterCybil->rotation.vy,
                                  monsterCybilProps.field_11A) != NO_VALUE)
                {
                    monsterCybilProps.field_115 = 1;
                    monsterCybilProps.field_116 = 1;
                }
            }
            else
            {
                monsterCybil->field_44.field_0 = 0;
            }

            if (modelUpper->anim.keyframeIdx >= (MONSTER_CYBIL_ANIM_INFOS[7].startKeyframeIdx + 4))
            {
                func_800DB748(monsterCybil);
            }

            monsterCybilProps.freeze += g_DeltaTime;

            if (modelUpper->stateStep == 0)
            {
                modelUpper->anim.status = ANIM_STATUS(MonsterCybilAnim_3, false);
                modelUpper->stateStep++;
            }

            if (ANIM_STATUS_IS_ACTIVE(monsterCybil->model.anim.status) && modelUpper->anim.keyframeIdx == MONSTER_CYBIL_ANIM_INFOS[7].endKeyframeIdx)
            {
                if (monsterCybilProps.field_120 == 0 || monsterCybilProps.field_104 == 0 ||
                    monsterCybilProps.field_114 == 3 || monsterCybilProps.field_115 != 0 ||
                    func_800DB81C(monsterCybil) == true)
                {
                    monsterCybilProps.field_115 = 0;
                    monsterCybilProps.field_EE  = 10;
                    modelUpper->stateStep       = 0;

                    if (monsterCybilProps.field_EC != 1)
                    {
                        monsterCybilProps.field_EC    = 10;
                        monsterCybil->model.stateStep = 0;
                    }
                }
                else if ((D_800EBB4A + Q12(1.5f)) < monsterCybilProps.freeze)
                {
                    modelUpper->stateStep      = 0;
                    monsterCybilProps.freeze = Q12(0.0f);

                    if (monsterCybilProps.field_EC == 3)
                    {
                        monsterCybil->model.stateStep = 0;
                    }
                }
            }
            break;

        case 8:
            func_800DB748(monsterCybil);

            D_800ED570.vx = Q8_TO_Q12(boneCoords[10].workm.t[0]);
            D_800ED570.vy = Q8_TO_Q12(boneCoords[10].workm.t[1]);
            D_800ED570.vz = Q8_TO_Q12(boneCoords[10].workm.t[2]);

            if (modelUpper->stateStep == 0)
            {
                modelUpper->anim.status = ANIM_STATUS(MonsterCybilAnim_8, false);
                modelUpper->stateStep++;
            }

            if (modelUpper->anim.keyframeIdx > 169 && modelUpper->anim.keyframeIdx < 173)
            {
                monsterCybil->model.anim.baseAnimInfos = MONSTER_CYBIL_ANIM_INFOS;
                monsterCybil->field_44.field_0         = 1;
            }
            else
            {
                monsterCybil->field_44.field_0 = 0;
            }

            if (func_8008A0E4(monsterCybil->field_44.field_0, WEAPON_ATTACK(EquippedWeaponId_HandgunBullets, AttackInputType_Tap), monsterCybil, &D_800ED570, &playerChara, monsterCybil->rotation.vy, 0x400) != -1)
            {
                playerChara.damage.position.vz = Q12(1.0f);

                if (sharedData_800D16E4_2_s01 == 0)
                {
                    Sfx_WithFlagsPlay(Sfx_Unk1630, &monsterCybil->position, Q8(0.25f), SfxFlag_None);
                }

                sharedData_800D16E4_2_s01   = 1;
                monsterCybilProps.field_116 = 1;
            }

            if (modelUpper->anim.keyframeIdx & 1 && modelUpper->anim.keyframeIdx == D_800EA836)
            {
                monsterCybilProps.field_EE = 1;
                modelUpper->stateStep      = 0;
                sharedData_800D16E4_2_s01  = 0;
            }
            break;

        case 9:
            func_800D8848(modelUpper);

            if (modelUpper->anim.keyframeIdx == D_800EA856 && monsterCybilProps.field_EC == 12)
            {
                monsterCybilProps.field_EC                  = 11;
                monsterCybilProps.field_EE                  = 11;
                monsterCybil->model.stateStep               = 0;
                modelUpper->stateStep                       = 0;
                monsterCybil->collision.shapeOffsets.box.vz = Q12(0.0f);
                monsterCybil->collision.shapeOffsets.box.vx = Q12(0.0f);
            }
            break;

        case 10:
            if (modelUpper->stateStep == 0)
            {
                modelUpper->anim.status = ANIM_STATUS(MonsterCybilAnim_10, false);
                modelUpper->stateStep++;
            }

            if (ANIM_STATUS_IS_ACTIVE(modelUpper->anim.status) && modelUpper->anim.keyframeIdx < 209)
            {
                WorldGfx_CharaMeshSwap(Chara_MonsterCybil, MESH_SWAP_STATUS(CybilSwappableMesh_None, CybilVariantMesh_1));
            }

            if (modelUpper->anim.keyframeIdx == MONSTER_CYBIL_ANIM_INFOS[modelUpper->anim.status].startKeyframeIdx)
            {
                monsterCybilProps.field_EE = 1;
                modelUpper->stateStep      = 0;

                if (monsterCybilProps.field_EC == 10)
                {
                    monsterCybilProps.field_EC    = 1;
                    monsterCybil->model.stateStep = 0;
                }
            }
            break;

        case 11:
            if (modelUpper->stateStep == 0)
            {
                modelUpper->anim.status = ANIM_STATUS(MonsterCybilAnim_11, true);
                modelUpper->stateStep++;
                modelUpper->anim.time        = FP_TO(MONSTER_CYBIL_ANIM_INFOS[23].endKeyframeIdx, Q12_SHIFT);
                modelUpper->anim.keyframeIdx = MONSTER_CYBIL_ANIM_INFOS[23].endKeyframeIdx;
            }

            if (modelUpper->anim.keyframeIdx == D_800EA894)
            {
                monsterCybilProps.field_EE = 1;
                modelUpper->stateStep      = 0;
            }
            break;

        case 12:
            monsterCybilProps.freeze += g_DeltaTime;

            if (monsterCybil->model.stateStep == 0)
            {
                Sfx_WithFlagsPlay(Sfx_Unk1618, &monsterCybil->position, Q8(0.25f), SfxFlag_None);
            }

            if (modelUpper->stateStep == 0)
            {
                modelUpper->anim.status = 0x18;
                modelUpper->stateStep++;
            }

            if (monsterCybilProps.freeze  >= Q12(10.0f) ||
                monsterCybilProps.field_108 >= Q12(6.0f))
            {
                monsterCybilProps.field_EC    = 11;
                monsterCybilProps.field_EE    = 11;
                monsterCybil->model.stateStep = 0;
                modelUpper->stateStep         = 0;
                monsterCybilProps.freeze    = Q12(0.0f);
            }
            break;
    }

    #undef playerChara
}

void func_800DA9C8(s_SubCharacter* monsterCybil, s_Model* modelUpper, GsCOORDINATE2* boneCoords) // 0x800DA9C8
{
    q19_12 angle;
    s32    temp_v0_2;
    s32    var_a1;
    s32    var_s3;

    #define playerChara g_SysWork.playerWork.player

    switch (monsterCybilProps.field_EC)
    {
        case 0:
            break;

        case 1:
            if (monsterCybil->model.stateStep == 0)
            {
                monsterCybil->model.anim.status = 3;
                monsterCybil->model.stateStep++;
                monsterCybil->model.anim.time        = MONSTER_CYBIL_ANIM_INFOS[3].startKeyframeIdx + Q12(3.0f);
                monsterCybil->model.anim.keyframeIdx = MONSTER_CYBIL_ANIM_INFOS[3].startKeyframeIdx + 3;
            }

            func_800DC018(monsterCybil);

            if (playerChara.health > Q12(0.0f) &&
                monsterCybilProps.field_116 == 0)
            {
                monsterCybilProps.moveSpeed = Q12(0.0f);

                if (monsterCybilProps.distanceToPlayer > Q12(0.6f))
                {
                    func_800DB748(monsterCybil);
                }

                if (monsterCybil->model.anim.status == 3)
                {
                    if ((u16)monsterCybil->model.anim.keyframeIdx < 24)
                    {
                        monsterCybilProps.moveSpeed = D_800CB208[monsterCybil->model.anim.keyframeIdx];
                    }
                }

                if (monsterCybilProps.field_120 == 0)
                {
                    monsterCybilProps.moveSpeed <<= 1;

                    if (monsterCybilProps.field_106 > 60)
                    {
                        monsterCybilProps.field_106 = 0;
                        func_800DBA48(monsterCybil);
                    }
                }

                if (func_800DBD64(monsterCybil))
                {
                    monsterCybilProps.field_10C += g_DeltaTime;
                }
                else
                {
                    monsterCybilProps.field_10C = Q12(0.0f);
                }

                if (monsterCybilProps.field_10C > Q12(3.0f))
                {
                    monsterCybilProps.field_116 = 1;
                    monsterCybilProps.field_10C = Q12(0.0f);
                }
            }
            else if (monsterCybilProps.field_116 == 1)
            {
                func_800DBE5C(monsterCybil);
            }
            else
            {
                monsterCybilProps.moveSpeed = Q12(0.0f);

                if (monsterCybil->model.anim.status == ANIM_STATUS(MonsterCybilAnim_1, true))
                {
                    if ((u16)monsterCybil->model.anim.keyframeIdx < 24)
                    {
                        monsterCybilProps.moveSpeed = D_800CB208[monsterCybil->model.anim.keyframeIdx];
                    }
                }
            }

            monsterCybilProps.field_122 = Q12_ANGLE(0.0f);
            break;

        case 5:
            if (monsterCybil->model.stateStep == 0)
            {
                monsterCybil->model.anim.status = ANIM_STATUS(MonsterCybilAnim_5, false);
                monsterCybil->model.stateStep++;
            }

            func_800DB748(monsterCybil);

            if (monsterCybil->model.anim.keyframeIdx >= (D_800EA7D4 + 10))
            {
                monsterCybilProps.moveSpeed = Q12(0.7f);
            }
            break;

        case 6:
            var_s3 = 0;
            var_a1 = 0;

            if (monsterCybil->model.stateStep == 0)
            {
                monsterCybil->model.anim.status = ANIM_STATUS(MonsterCybilAnim_6, false);
                monsterCybil->model.stateStep++;
            }

            if (monsterCybil->model.anim.status & 1)
            {
                monsterCybil->model.anim.time        = playerChara.model.anim.time - Q12(832.0f);
                monsterCybil->model.anim.keyframeIdx = playerChara.model.anim.keyframeIdx - 832;
            }

            monsterCybilProps.moveSpeed = Q12(0.0f);
            angle                       = Q12_FRACT((u16)monsterCybil->rotation.vy + Q12(1.5f));

            switch (g_SysWork.playerWork.extra.state)
            {
                case 32:
                    var_s3 = playerChara.position.vx + Q12_MULT(Math_Sin(angle), Q12(0.62f));
                    var_a1 = playerChara.position.vz + Q12_MULT(Math_Cos(angle), Q12(0.62f));
                    break;

                case 33:
                    var_s3 = playerChara.position.vx + Q12_MULT(Math_Sin(angle), Q12(0.8f));
                    var_a1 = playerChara.position.vz + Q12_MULT(Math_Cos(angle), Q12(0.8f));
                    break;
            }

            if (g_SysWork.playerWork.extra.state > 31 &&
                g_SysWork.playerWork.extra.state < 34)
            {
                if (var_s3 < monsterCybil->position.vx)
                {
                    monsterCybil->position.vx -= Q12(1.0f / 128.0f);
                }
                else if (monsterCybil->position.vx < var_s3)
                {
                    monsterCybil->position.vx += Q12(1.0f / 128.0f);
                }

                if (var_a1 < monsterCybil->position.vz)
                {
                    monsterCybil->position.vz = monsterCybil->position.vz - Q12(1.0f / 128.0f);
                }
                else if (monsterCybil->position.vz < var_a1)
                {
                    monsterCybil->position.vz = monsterCybil->position.vz + Q12(1.0f / 128.0f);
                }
            }
            break;

        case 7:
            if (monsterCybil->model.stateStep == 0)
            {
                monsterCybil->model.anim.status = ANIM_STATUS(MonsterCybilAnim_7, false);
                monsterCybil->model.stateStep++;
            }

            if (monsterCybil->model.anim.keyframeIdx == D_800EA816)
            {
                monsterCybilProps.field_EC    = 1;
                monsterCybil->model.stateStep = 0;
                monsterCybil->position.vx    += Q12_MULT(Math_Sin(monsterCybil->rotation.vy + Q12_ANGLE(33.75f)), Q12(0.1f));
                monsterCybil->position.vz    += Q12_MULT(Math_Cos(monsterCybil->rotation.vy + Q12_ANGLE(33.75f)), Q12(0.1f));
            }
            break;

        case 8:
            monsterCybilProps.moveSpeed = Q12(0.0f);
            if (monsterCybil->model.stateStep == 0)
            {
                monsterCybil->model.anim.status = ANIM_STATUS(MonsterCybilAnim_8, false);
                monsterCybil->model.stateStep++;
            }
            break;

        case 9:
            func_800D87B0(monsterCybil);
            break;

        case 12:
            break;
    }

    switch (monsterCybilProps.field_EE)
    {
        case 0:
            break;

        case 1:
            if (modelUpper->stateStep == 0)
            {
                modelUpper->anim.status = ANIM_STATUS(MonsterCybilAnim_1, true);
                modelUpper->stateStep++;
                modelUpper->anim.time        = MONSTER_CYBIL_ANIM_INFOS[3].startKeyframeIdx + Q12(3.0f);
                modelUpper->anim.keyframeIdx = MONSTER_CYBIL_ANIM_INFOS[3].startKeyframeIdx + 3;
            }

            if (modelUpper->anim.status == ANIM_STATUS(MonsterCybilAnim_1, true))
            {
                modelUpper->anim.time        = monsterCybil->model.anim.time;
                modelUpper->anim.keyframeIdx = monsterCybil->model.anim.keyframeIdx;
            }

            if (playerChara.health > Q12(0.0f) &&
                monsterCybilProps.field_116 == 0 &&
                monsterCybilProps.distanceToPlayer < Q12(0.8f) &&
                monsterCybilProps.field_106 > 30)
            {
                if (Rng_GenerateInt(0, 7) >= 5)
                {
                    if (monsterCybilProps.field_EE == 1)
                    {
                        monsterCybilProps.field_EC    = 8;
                        monsterCybilProps.field_EE    = 8;
                        monsterCybil->model.stateStep = 0;
                        modelUpper->stateStep         = 0;
                    }
                }
                else
                {
                    if (monsterCybilProps.field_EE == 1)
                    {
                        monsterCybilProps.field_EC    = 5;
                        monsterCybilProps.field_EE    = 5;
                        monsterCybil->model.stateStep = 0;
                        modelUpper->stateStep         = 0;
                    }
                }
            }
            break;

        case 5:
            D_800ED570.vx = Q8_TO_Q12(boneCoords[10].workm.t[0]);
            D_800ED570.vy = Q8_TO_Q12(boneCoords[10].workm.t[1]);
            D_800ED570.vz = Q8_TO_Q12(boneCoords[10].workm.t[2]);

            if (modelUpper->stateStep == 0)
            {
                modelUpper->anim.status = ANIM_STATUS(MonsterCybilAnim_5, false);
                modelUpper->stateStep++;
            }

            if (modelUpper->anim.keyframeIdx == MONSTER_CYBIL_ANIM_INFOS[11].endKeyframeIdx)
            {
                monsterCybil->model.anim.baseAnimInfos = MONSTER_CYBIL_ANIM_INFOS;
                monsterCybil->field_44.field_0         = 1;
            }
            else
            {
                monsterCybil->field_44.field_0 = 0;
            }

            temp_v0_2 = func_8008A0E4(monsterCybil->field_44.field_0,
                                      WEAPON_ATTACK(EquippedWeaponId_ShotgunShells, AttackInputType_Tap),
                                      monsterCybil,
                                      &D_800ED570,
                                      &playerChara,
                                      monsterCybil->rotation.vy,
                                      Q12_ANGLE(90.0f));

            if (temp_v0_2 != NO_VALUE)
            {
                playerChara.field_40 = monsterCybil->field_40;
            }

            if (modelUpper->anim.keyframeIdx == D_800EA7D6)
            {
                if (temp_v0_2 != NO_VALUE)
                {
                    monsterCybilProps.field_EE = 6;
                    monsterCybilProps.field_EC = 6;
                }
                else
                {
                    monsterCybilProps.field_EE = 7;
                    monsterCybilProps.field_EC = 7;
                }

                modelUpper->stateStep         = 0;
                monsterCybil->model.stateStep = 0;
                monsterCybilProps.moveSpeed   = Q12(0.0f);
                monsterCybilProps.freeze    = Q12(0.0f);
            }
            break;

        case 6:
            if (modelUpper->stateStep == 0)
            {
                modelUpper->anim.status = ANIM_STATUS(MonsterCybilAnim_6, false);
                modelUpper->stateStep++;
            }

            if (ANIM_STATUS_IS_ACTIVE(modelUpper->anim.status))
            {
                modelUpper->anim.time        = playerChara.model.anim.time - Q12(832.0f);
                modelUpper->anim.keyframeIdx = playerChara.model.anim.keyframeIdx - 832;
            }

            playerChara.damage.amount = g_DeltaTime * 4;

            if (playerChara.field_40 == NO_VALUE ||
                playerChara.health <= Q12(0.0f) ||
                playerChara.attackReceived == NO_VALUE)
            {
                monsterCybilProps.field_EE    = 7;
                monsterCybilProps.field_EC    = 7;
                modelUpper->stateStep         = 0;
                monsterCybil->model.stateStep = 0;
                playerChara.damage.amount     = Q12(0.0f);
            }

            if (ANIM_STATUS_IS_ACTIVE(modelUpper->anim.status))
            {
                if (modelUpper->anim.keyframeIdx == 101)
                {
                    monsterCybilProps.field_118 = Q12(0.0f);
                }
                else
                {
                    monsterCybilProps.field_118 = D_800CB238[modelUpper->anim.keyframeIdx - 91] +
                                                  (((D_800CB238[modelUpper->anim.keyframeIdx - 90] - D_800CB238[modelUpper->anim.keyframeIdx - 91]) * Q12_FRACT((u16)modelUpper->anim.time)) / Q12(1.0f));
                }
            }
            break;

        case 7:
            if (modelUpper->stateStep == 0)
            {
                modelUpper->anim.status = ANIM_STATUS(MonsterCybilAnim_7, false);
                modelUpper->stateStep++;
            }

            if (modelUpper->anim.keyframeIdx == D_800EA816)
            {
                monsterCybilProps.field_EE  = 1;
                modelUpper->stateStep       = 0;
                monsterCybilProps.field_106 = 0;
                monsterCybilProps.field_116 = 1;
            }
            break;

        case 8:
            func_800DB748(monsterCybil);

            D_800ED570.vx = Q8_TO_Q12(boneCoords[10].workm.t[0]);
            D_800ED570.vy = Q8_TO_Q12(boneCoords[10].workm.t[1]);
            D_800ED570.vz = Q8_TO_Q12(boneCoords[10].workm.t[2]);

            if (modelUpper->stateStep == 0)
            {
                modelUpper->anim.status = ANIM_STATUS(MonsterCybilAnim_8, false);
                modelUpper->stateStep++;
            }

            if (modelUpper->anim.keyframeIdx > 169 &&
                modelUpper->anim.keyframeIdx < 173)
            {
                monsterCybil->model.anim.baseAnimInfos = MONSTER_CYBIL_ANIM_INFOS;
                monsterCybil->field_44.field_0         = 1;
            }
            else
            {
                monsterCybil->field_44.field_0 = 0;
            }

            if (func_8008A0E4(monsterCybil->field_44.field_0,
                              WEAPON_ATTACK(EquippedWeaponId_RifleShells, AttackInputType_Tap),
                              monsterCybil,
                              &D_800ED570,
                              &playerChara,
                              monsterCybil->rotation.vy,
                              Q12_ANGLE(90.0f)) != NO_VALUE)
            {
                playerChara.damage.position.vz = Q12(1.0f);

                if (sharedData_800D16E4_2_s01 == 0)
                {
                    Sfx_WithFlagsPlay(Sfx_Unk1630, &monsterCybil->position, Q8(0.25f), SfxFlag_None);
                }

                sharedData_800D16E4_2_s01   = 1;
                monsterCybilProps.field_116 = 1;
            }

            if (ANIM_STATUS_IS_ACTIVE(modelUpper->anim.keyframeIdx) && modelUpper->anim.keyframeIdx == D_800EA836)
            {
                monsterCybilProps.field_EE    = 1;
                monsterCybilProps.field_EC    = 1;
                modelUpper->stateStep         = 0;
                monsterCybil->model.stateStep = 0;
                sharedData_800D16E4_2_s01     = 0;
            }
            break;

        case 9:
            func_800D8848(modelUpper);
            break;

        case 12:
            if (modelUpper->stateStep == 0)
            {
                modelUpper->anim.status = ANIM_STATUS(MonsterCybilAnim_12, false);
                modelUpper->stateStep++;
            }
            break;
    }

    #undef playerChara
}

void func_800DB4CC(s_SubCharacter* monsterCybil, s_Model* modelUpper, GsCOORDINATE2* boneCoords) // 0x800DB4CC
{
    q3_12  angle;
    q19_12 range;
    q19_12 delta;

    #define playerChara g_SysWork.playerWork.player

    delta = TIMESTEP_SCALE_30_FPS(g_DeltaTime, 32);

    switch (monsterCybilProps.field_EE)
    {
        case 9:
        case 11:
            if (monsterCybilProps.field_EC == 12)
            {
                D_800ED560.vy = -224;
                D_800ED560.vz = -136;
                TransMatrix(&boneCoords[1].coord, &D_800ED560);
            }

            if (monsterCybilProps.field_EC == 11 &&
                modelUpper->anim.status == ANIM_STATUS(MonsterCybilAnim_9, true))
            {
                D_800ED560.vy = -224;
                D_800ED560.vz = -136;
                TransMatrix(&boneCoords[1].coord, &D_800ED560);
            }
            break;

        case 10:
            range = delta >> 1;
            if (range >= (monsterCybilProps.field_11A - Q12_ANGLE(90.0f)))
            {
                if ((monsterCybilProps.field_11A - Q12_ANGLE(90.0f)) >= -range)
                {
                    monsterCybilProps.field_11A = Q12_ANGLE(90.0f);
                }
                else
                {
                    monsterCybilProps.field_11A += range;
                }
            }
            else
            {
                monsterCybilProps.field_11A -= delta;
            }

            Math_RotMatrixZ(monsterCybilProps.field_11A - Q12_ANGLE(90.0f), &boneCoords[8].coord);
            break;

        case 2:
        case 3:
            angle = ratan2(Math_Vector2MagCalc((playerChara.position.vx - D_800ED570.vx) >> 4, (playerChara.position.vz - D_800ED570.vz) >> 4),
                           (playerChara.position.vy + playerChara.collision.box.offsetY - D_800ED570.vy) >> 4);

            if ((angle + delta) < monsterCybilProps.field_11A)
            {
                monsterCybilProps.field_11A -= delta;
            }
            else
            {
                monsterCybilProps.field_11A = angle;
            }

            Math_RotMatrixZ(monsterCybilProps.field_11A - Q12_ANGLE(90.0f), &boneCoords[8].coord);
            break;

        case 6:
            Bone_ExtraRotationSet(&boneCoords[1], 0, 0, -monsterCybilProps.field_118);
            break;
    }

    #undef playerChara
}

bool func_800DB6FC(VECTOR3* pos) // 0x800DB6FC
{
    return Vw_AabbVisibleInScreenCheck(pos->vx - Q12(0.5f), pos->vx + Q12(0.5f),
                                       pos->vy - Q12(1.7f), pos->vy + Q12(0.1f),
                                       pos->vz - Q12(0.5f), pos->vz + Q12(0.5f));
}

void func_800DB748(s_SubCharacter* monsterCybil) // 0x800DB748
{
    q3_12 angleToPlayer;
    q3_12 shortestAngle;

    #define playerChara g_SysWork.playerWork.player

    // TODO: Not sure if `Q12_TO_Q8` makes sense here, maybe this was just a divide by 16 for some reason.
    angleToPlayer = Q12_ANGLE_ABS(ratan2(Q12_TO_Q8(playerChara.position.vx - monsterCybil->position.vx),
                                         Q12_TO_Q8(playerChara.position.vz - monsterCybil->position.vz)));

    Math_ShortestAngleGet(monsterCybil->rotation.vy, angleToPlayer, &shortestAngle);

    if (ABS(shortestAngle) < Q12_ANGLE(11.3f))
    {
        monsterCybil->rotation.vy = angleToPlayer;
    }
    else if (shortestAngle > Q12_ANGLE(0.0f))
    {
        monsterCybil->rotation.vy += Q12_ANGLE(11.3f);
    }
    else
    {
        monsterCybil->rotation.vy -= Q12_ANGLE(11.3f);
    }
}

bool func_800DB81C(s_SubCharacter* monsterCybil) // 0x800DB81C
{
    s_RayTrace trace;
    VECTOR3    dir; // Q19.12

    dir.vy = Q12_MULT(D_800AD4C8[63].field_0, Math_Cos(monsterCybilProps.field_11A));
    dir.vx = Q12_MULT(Q12_MULT(D_800AD4C8[63].field_0, Math_Sin(monsterCybilProps.field_11A)), Math_Sin(monsterCybil->rotation.vy));
    dir.vz = Q12_MULT(Q12_MULT(D_800AD4C8[63].field_0, Math_Sin(monsterCybilProps.field_11A)), Math_Cos(monsterCybil->rotation.vy));

    if (Ray_GeomAndCharaTraceGet(&trace, &D_800ED570, &dir, monsterCybil) && trace.character == NULL)
    {
        return true;
    }

    return false;
}

s32 func_800DB930(void) // 0x800DB930
{
    VECTOR3 sp10;
    q19_12  dist;
    q19_12  shortestDist;
    s32     foundIdx;
    s32     i;

    #define playerChara g_SysWork.playerWork.player

    shortestDist = Q12(20.0f);
    foundIdx     = NO_VALUE;

    for (i = 0; i < 10; i++)
    {
        sp10.vx = D_800CB2B4[i] + Q12(20.0f);
        sp10.vy = Q12(-0.5f);
        sp10.vz = D_800CB2DC[i] + Q12(100.0f);

        if (func_800DB6FC(&sp10))
        {
            continue;
        }

        dist = Math_Vector2MagCalcSafeQ8(playerChara.position.vx - sp10.vx,
                                         playerChara.position.vz - sp10.vz);
        if (dist < shortestDist)
        {
            shortestDist = dist;
            foundIdx     = i;
        }
    }

    return foundIdx;
}

s32 func_800DBA48(s_SubCharacter* monsterCybil) // 0x800DBA48
{
    // TODO: Decl order matters for some reason.
    VECTOR sp10;
    q19_12 dist;
    s32    var_s0;
    s32    count;
    q19_12 shortestDist;
    s32    foundIdx;
    q19_12 distMax;
    s32    i;
    q19_12 dist0;

    #define playerChara g_SysWork.playerWork.player

    dist0 = Math_Vector2MagCalcSafeQ8(playerChara.position.vx - Q12(20.0f),
                                      playerChara.position.vz - Q12(100.0f));
    if (dist0 > Q12(6.5f))
    {
        var_s0 = 0;
    }
    else if (dist0 > Q12(3.2f))
    {
        var_s0 = 1;
    }
    else
    {
        var_s0 = 2;
    }

    shortestDist = Q12(20.0f);
    dist         = Q12(20.0f);

    foundIdx = NO_VALUE;
    distMax  = monsterCybilProps.distanceToPlayer;
    count    = (3 - var_s0) * 4;

    for (i = 0; i < count; i++)
    {
        switch (var_s0)
        {
            case 0:
                sp10.vx = D_800CB304[i][0] + Q12(20.0f);
                sp10.vy = Q12(0.0f);
                sp10.vz = D_800CB304[i][1] + Q12(100.0f);
                break;

            case 1:
                sp10.vx = D_800CB364[i][0] + Q12(20.0f);
                sp10.vy = Q12(0.0f);
                sp10.vz = D_800CB364[i][1] + Q12(100.0f);
                break;

            case 2:
                sp10.vx = D_800CB3A4[i][0] + Q12(20.0f);
                sp10.vy = Q12(0.0f);
                sp10.vz = D_800CB3A4[i][1] + Q12(100.0f);
                break;
        }

        if (!func_800DB6FC(&sp10))
        {
            dist = Math_Vector2MagCalcSafeQ8(playerChara.position.vx - sp10.vx,
                                             playerChara.position.vz - sp10.vz);
            if (dist < shortestDist)
            {
                foundIdx = i;
            }
        }
    }

    if (foundIdx == NO_VALUE)
    {
        return 0;
    }

    if (dist < distMax)
    {
        switch (var_s0)
        {
            case 0:
                sp10.vx = D_800CB304[foundIdx][0] + Q12(20.0f);
                sp10.vz = D_800CB304[foundIdx][1] + Q12(100.0f);
                break;

            case 1:
                sp10.vx = D_800CB364[foundIdx][0] + Q12(20.0f);
                sp10.vz = D_800CB364[foundIdx][1] + Q12(100.0f);
                break;

            case 2:
                sp10.vx = D_800CB3A4[foundIdx][0] + Q12(20.0f);
                sp10.vz = D_800CB3A4[foundIdx][1] + Q12(100.0f);
                break;
        }

        monsterCybil->position.vx = sp10.vx;
        monsterCybil->position.vz = sp10.vz;
        func_800DB748(monsterCybil);
        return 1;
    }

    return 2;

    #undef playerChara
}

bool func_800DBD64(s_SubCharacter* monsterCybil) // 0x800DBD64
{
    q19_12 shortestDist;
    q19_12 dist;
    q19_12 posX;
    q19_12 posZ;
    s32    i;

    if (monsterCybilProps.distanceToPlayer > Q12(2.0f))
    {
        return false;
    }

    shortestDist = Q12(6.0f);

    for (i = 0; i < 10; i++)
    {
        posX = monsterCybil->position.vx - Q12(20.0f);
        posZ = monsterCybil->position.vz - Q12(100.0f);
        dist = Math_Vector2MagCalcSafeQ8(D_800CB250[i] - posX, D_800CB278[i] - posZ);
        if (shortestDist > dist)
        {
            shortestDist = dist;
        }
    }

    return shortestDist < monsterCybilProps.distanceToPlayer;
}

void func_800DBE5C(s_SubCharacter* monsterCybil)
{
    q3_12  angle;
    q19_12 unkAngle1;
    q3_12  unkAngle0;

    if (monsterCybilProps.controlState == MonsterCybilControl_1)
    {
        unkAngle1 = ((Rng_Rand16() & 0x1) * Q12_ANGLE(180.0f)) - Q12_ANGLE(90.0f);
    }
    else
    {
        unkAngle1 = ((Rng_Rand16() & 0x1) * Q12_ANGLE(180.0f)) - Q12_ANGLE(45.0f);
    }

    // TODO: Use macro.
    unkAngle0 = Q12_ANGLE_NORM_U(unkAngle1 + ratan2(Q12_TO_Q8(monsterCybil->position.vx - Q12(20.0f)), Q12_TO_Q8(monsterCybil->position.vz - Q12(100.0f))) + Q12(1.0f));

    if (monsterCybil->model.anim.status == ANIM_STATUS(MonsterCybilAnim_1, true))
    {
        if (monsterCybil->model.anim.keyframeIdx >= 0 &&
            monsterCybil->model.anim.keyframeIdx <= 23)
        {
            monsterCybilProps.moveSpeed = D_800CB1D8[monsterCybil->model.anim.keyframeIdx];
        }
    }

    Math_ShortestAngleGet(monsterCybil->rotation.vy, unkAngle0, &angle);
    if (ABS(angle) < Q12_ANGLE(5.625f))
    {
        monsterCybil->rotation.vy = unkAngle0;
    }
    else if (angle > Q12_ANGLE(0.0f))
    {
        monsterCybil->rotation.vy += Q12_ANGLE(5.625f);
    }
    else
    {
        monsterCybil->rotation.vy -= Q12_ANGLE(5.625f);
    }

    monsterCybil->rotation.vy += (Rng_Rand16() % Q12_ANGLE(11.25f)) - Q12_ANGLE(5.625f);

    monsterCybilProps.field_10C += g_DeltaTime;
    if (monsterCybilProps.field_10C > Q12(5.0f))
    {
        monsterCybilProps.field_116 = 0;
        monsterCybilProps.field_10C = Q12(0.0f);
    }
}

void func_800DC018(s_SubCharacter* monsterCybil) // 0x800DC018
{
    s_RayTrace trace;
    VECTOR3    from; // Q19.12
    VECTOR3    to;   // Q19.12
    q19_12     newRotY;

    switch (monsterCybilProps.field_116)
    {
        case 2:
            monsterCybilProps.field_106 += TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12_ANGLE(11.25f / 4.0f));
            monsterCybilProps.field_106  = CLAMP(monsterCybilProps.field_106,
                                                 Q12_ANGLE(0.0f),
                                                 Q12_ANGLE(135.0f));

            newRotY = monsterCybil->rotation.vy;
            if (monsterCybilProps.field_106 < Q12_ANGLE(90.0f))
            {
                newRotY += TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12_ANGLE(11.25f / 4.0f));
            }
            monsterCybil->rotation.vy = newRotY;

            if (monsterCybilProps.field_106 == Q12_ANGLE(135.0f))
            {
                monsterCybilProps.field_106 = Q12_ANGLE(0.0f);
                monsterCybilProps.field_116 = 0;
            }
            break;

        case 3:
            monsterCybilProps.field_106 -= TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12_ANGLE(11.25f / 4.0f));
            monsterCybilProps.field_106  = CLAMP(monsterCybilProps.field_106,
                                                 Q12_ANGLE(-135.0f),
                                                 Q12_ANGLE(0.0f));

            newRotY = monsterCybil->rotation.vy;
            if (monsterCybilProps.field_106 > Q12_ANGLE(-90.0f))
            {
                newRotY -= TIMESTEP_SCALE_30_FPS(g_DeltaTime, Q12_ANGLE(11.25f / 4.0f));
            }
            monsterCybil->rotation.vy = newRotY;

            if (monsterCybilProps.field_106 == Q12_ANGLE(-135.0f))
            {
                monsterCybilProps.field_106 = Q12_ANGLE(0.0f);
                monsterCybilProps.field_116 = 0;
            }
            break;

        default:
            from.vx = monsterCybil->position.vx;
            from.vy = monsterCybil->position.vy;
            from.vz = monsterCybil->position.vz;

            to.vx = monsterCybil->position.vx + Q12_MULT(Math_Sin(monsterCybil->rotation.vy), Q12(0.3f));
            to.vy = from.vy;
            to.vz = monsterCybil->position.vz + Q12_MULT(Math_Cos(monsterCybil->rotation.vy), Q12(0.3f));

            if (Ray_GeomTraceGet(&trace, &from, &to))
            {
                monsterCybilProps.field_116 = Rng_GenerateUInt(2, 3);
                monsterCybilProps.field_106 = Q12_ANGLE(0.0f);
            }
            break;
    }
}
