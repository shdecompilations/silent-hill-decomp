#include <libapi.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/chara/dms.h"
#include "bodyprog/events/event_data.h"
#include "bodyprog/gfx/map_effects.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player/player.h"
#include "main/rng.h"

// TODO: .bss is emitted in order of first declaration, right now our headers
// declare externs in semi-random order though.
// For now we'll declare them before headers in the order they appear in the
// map .bss, later can hopefully improve order in headers instead.
extern e_MapIdx                  g_ParticleMapIdx0;
extern e_MapIdx                  g_ParticleMapIdx1;
extern s32                       __pad_bss_800D0544[2];
extern VECTOR3                   g_CameraPositionTarget;
extern s32                       __pad_bss_800D0558;
extern VECTOR3                   g_CameraLookAtTarget;
extern u8                        g_Cutscene_MapMsgAudioIdx;
extern s8                        __pad_bss_800D0569[3];
extern s32                       g_Particle_SpeedX;
extern s32                       g_Particle_SpeedZ;
extern s32                       sharedData_800DFB6C_0_s00;
extern s32                       sharedData_800DFB70_0_s00;
extern s_MapHdr_field_4C         sharedData_800DFB7C_0_s00[];
extern s_BloodSplat              g_Effect_BloodSplats[];
extern s8                        __pad_bss_800D1648[4];
extern struct _ParticleVectors   g_ParticleVectors0;
extern s32                       __pad_bss_800D1678;
extern struct _ParticleVectors   g_ParticleVectors1;
extern s32                       sharedData_800E0CA8_0_s00;
extern s32                       sharedData_800E0CAC_0_s00;
extern u32                       sharedData_800E0CB0_0_s00;
extern u16                       sharedData_800E0CB4_0_s00;
extern u16                       sharedData_800E0CB6_0_s00;
extern u16                       sharedData_800E0CB8_0_s00;
extern s8                        sharedData_800E0CBA_0_s00;
extern s8                        __pad_bss_800D16BB;
extern struct _CollisionResult   sharedData_800E39BC_0_s00;
extern s32                       sharedData_800E39D8_0_s00;
extern q19_12                    g_Player_MoveSpeed;
extern s32                       sharedData_800D16E0_2_s01;
extern u8                        sharedData_800D16E4_2_s01;
extern s8                        __pad_bss_800D16E5[7];
extern struct _WorldObjectPose   g_WorldObject_Key;
extern struct _WorldObjectPose   g_WorldObject_Item;
extern struct _WorldObjectPose   g_WorldObject_SavePad;
extern u8                        D_800D177C;
extern s8                        __pad_bss_800D177D[15];
extern struct _WorldObjectModel  g_CommonWorldObjects[];

#include "maps/map2/map2_s01.h"
#include "maps/common/characters/dahlia.h"
#include "maps/common/particle.h"
#include "maps/common/characters/player.h"

VECTOR3 g_CameraPositionTarget;
VECTOR3 g_CameraLookAtTarget;
u8      g_Cutscene_MapMsgAudioIdx;

s32 __pad_bss_800D0544[2];
s32 __pad_bss_800D0558;
s8  __pad_bss_800D0569[3];
s8  __pad_bss_800D1648[4];
s32 __pad_bss_800D1678;
s8  __pad_bss_800D16BB;
s8  __pad_bss_800D16E5[7];

#include "../src/maps/common/particles/particle.c"

#include "../src/maps/common/characters/player.c"

#include "../src/maps/common/characters/dahlia.c"

#include "../src/maps/common/chara_util.c" // 0x800CDDA0

#include "../src/maps/common/map_util.c" // 0x800CE7B0

#include "maps/common/shared/Map_RoomBgmInit_CheckCond.h" // 0x800CE858

#include "maps/common/shared/Map_RoomBgmInit_0_s02_CondTrue.h" // 0x800CE88C

#include "maps/common/shared/Map_RoomBgmInit_0_s02_CondFalse.h" // 0x800CE908

void GameBoot_LoadScreen_StageString(void) {} // 0x800CEB94

void func_800CEB9C(void) // 0x800CEB9C
{
    VECTOR3 sfxPos = { MAP_POINTS[g_MapEventData->mapPointIdx].positionX, Q12(-1.2f), MAP_POINTS[g_MapEventData->mapPointIdx].positionZ };

    Event_DisplayMapMsgWithSfx(MapMsgIdx_DoorJammed, Sfx_DoorJammed, &sfxPos);
}

void func_800CEC30(void) // 0x800CEC30
{
    VECTOR3 sfxPos = { MAP_POINTS[g_MapEventData->mapPointIdx].positionX, Q12(-1.2f), MAP_POINTS[g_MapEventData->mapPointIdx].positionZ };

    if (g_MapEventData->mapPointIdx == 2)
    {
        if (Savegame_EventFlagGet(EventFlag_180))
        {
            Event_DisplayMapMsgWithSfx(38, Sfx_Unk1486, &sfxPos);

            if (g_SysWork.sysState == SysState_Gameplay)
            {
                Savegame_EventFlagSet(EventFlag_180);
            }
        }
        else
        {
            Event_DisplayMapMsgWithSfx(MapMsgIdx_DoorLocked, Sfx_Unk1486, &sfxPos);
        }
    }
    else
    {
        Event_DisplayMapMsgWithSfx(MapMsgIdx_DoorLocked, Sfx_DoorLocked, &sfxPos);
    }
}

const char* MAP_MESSAGES[] = {
    #include "maps/common/shared/map_msg_common.h"
#if VERSION_REGION_IS(NTSC)
    /* 15 */ "~J1(1.9)\tWere_you_ringing_that_bell? ",
    /* 16 */ "~J1(5.3)\tI've_been_expecting_you. ~N\n\t\t\tIt_was_foretold_by_Gyromancy. ",
    /* 17 */ "~J1(1.8)\tWhat_are_you_talking_about? ~E ",
    /* 18 */ "~J1(2.2)\tI_knew_you'd_come. ",
    /* 19 */ "~J1(2.4)\tYou_want_the_girl,_right? ",
    /* 20 */ "~J1(0.9)\tThe_girl? ",
    /* 21 */ "~J1(1.7)\tYou're_talking_about_Cheryl? ",
    /* 22 */ "~J1(2.8)\tI_see_everything. ~E ",
    /* 23 */ "~J0(2.1)\tYou_know_something!? ~N\n\t\t\tTell_me! ~E ",
    /* 24 */ "~J0(1.1)\tStay_back! ~E ",
    /* 25 */ "~J1(4.0)\tNothing_is_to_be_gained_from ~N\n\t\t\tfloundering_about_at_random.  ",
    /* 26 */ "~J1(2.3)\tYou_must_follow_the_path. ",
    /* 27 */ "~J1(4.0)\tThe_path_of_the_hermit, ~N\n\t\t\tconcealed_by_Flauros... ",
    /* 28 */ "~J1(3.6)\tWhat? ~N\n\t\t\tWhat_are_you_talking_about? ~E ",
    /* 29 */ "~J0(4.0)\tHere,_the_Flauros, ~N\n\t\t\ta_cage_of_peace. ",
    /* 30 */ "~J0(3.3)\tIt_can_break_through ~N\n\t\t\tthe_walls_of_darkness, ",
    /* 31 */ "~J0(3.8)\tand_counteract_the_wrath_of ~N\n\t\t\tthe_underworld. ",
    /* 32 */ "~J0(3.0)\tThese_will_help_you. ~E ",
    /* 33 */ "~J0(4.2)\tMake_haste_to_the_hospital ~N\n\t\t\tbefore_it's_too_late. ~E ",
    /* 34 */ "~J0(2.4)\tWait! ~N\n\t\t\tDon't_go_yet! ~E ",
    /* 35 */ "\tThere_is_a_ ~C2 [Flauros] ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 36 */ "\tThere_is_a_ ~C2 Drawbridge_key ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 37 */ "\tBible. ~E ",
    /* 38 */ "\tIt_won't_open. ",
    /* 39 */ "\tIt_must_be_locked ~N\n\tfrom_the_other_side. ~E ",
    /* 40 */ "\tThere's_a_collection_box. ~N\n\tNothing_inside. ~E ",
    /* 41 */ "\tThere's_a_newspaper. ~N\n\tNothing_special. ~E ",
    /* 42 */ "\tThere's_a_picture_on_the_wall. ~N\n\tNothing_special. ~E ",
    /* 43 */ "\tThere's_a_candle_stand. ~N\n\tNothing_special. ~E "
#elif VERSION_REGION_IS(NTSCJ)
    /* 15 */ "~J1(1.9)\t鐘を鳴らしていたのは　あんたなのか？ ",
    /* 16 */ "~J1(5.3)\tあなたが来ることはわかっていたわ ~N\n\t\t\t旋歩占いに出ていたからね ",
    /* 17 */ "~J1(1.8)\t何だって？ ~E ",
    /* 18 */ "~J1(2.2)\t待っていたのよ ",
    /* 19 */ "~J1(2.4)\tあの子を探しているのでしょう？ ",
    /* 20 */ "~J1(0.9)\tあの子？ ",
    /* 21 */ "~J1(1.7)\tシェリルのことか？ ",
    /* 22 */ "~J1(2.8)\tあたしにはすべてわかっているわ ~E ",
    /* 23 */ "~J0(2.1)\t何か知っているのか！？ ~N\n\t\t\t教えてくれ！ ~E ",
    /* 24 */ "~J0(1.1) ~D\t近づかないで！ ~E ",
    /* 25 */ "~J1(4.0)\t闇雲にあがいたところで決して ~N\n\t\t\t成就することはないわ ",
    /* 26 */ "~J1(2.3)\tたどるべき道があるのよ ",
    /* 27 */ "~J1(4.0)\tフラウロスに秘められた隠者の道よ ",
    /* 28 */ "~J1(3.6)\t何だ？ ~N\n\t\t\t何を言ってるんだ！？ ~E ",
    /* 29 */ "~J0(4.0)\tフラウロス ~N\n\t\t\t静寂の牢獄よ ",
    /* 30 */ "~J0(3.3)\t宵闇の壁を打ち砕き ",
    /* 31 */ "~J0(3.8)\t泥土の怨念を妨げるもの ",
    /* 32 */ "~J0(3.0)\tきっとこれが必要になるわ ~E ",
    /* 33 */ "~J0(4.2)\t病院へ行きなさい ~N\n\t\t\t手遅れにしたくないならばね ~E ",
    /* 34 */ "~J0(2.4)\t待て！ ~N\n\t\t\tまだ聞きたいことが…… ~E ",
    /* 35 */ "\t~C2 “フラウロス” ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 36 */ "\t~C2 跳ね橋の鍵 ~C7 がある ~N\n\t取りますか？ ~S4 ",
    /* 37 */ "\t聖書が置かれている ~E ",
    /* 38 */ "\t……開かない ~N\n\t向こう側から鍵をかけたのか…… ~E ",
    /* 39 */ "\t募金箱が置かれている ~N\n\t中には何も入っていない ~E ",
    /* 40 */ "\t新聞が置かれている ~N\n\t特に気になることはない ~E ",
    /* 41 */ "\t絵が飾ってある ~N\n\t特に気になることはない ~E ",
    /* 42 */ "\tロウソク立てがある ~N\n\t特に気になることはない ~E "
#endif
};

s_EventData MAP_EVENTS[] = {
#include "map_events.h"
};

u16 g_Cutscene_MapMsgAudioCmds[] = {
    0x104E, 0x1039, 0x1051, 0x104C, 0x1047, 0x1055, 0x1050, 0x1046, 0x1049, 0x1052,
    0x103E, 0x104B, 0x103C, 0x1042, 0x1040, 0x1043, 0x103F, 0x1044, 0x103A, 0x104A,
};

q19_12 g_Cutscene_Timer = NO_VALUE;

void func_800CED44(void) // 0x800CED44
{
    u32 pickupType;
    s32 eventFlagIdx;

    pickupType   = CommonPickupItemId_FirstAidKit;
    eventFlagIdx = 0;

    switch (g_MapEventData->mapPointIdx)
    {
        case 25:
            pickupType   = CommonPickupItemId_HealthDrink;
            eventFlagIdx = EventFlag_M2S01_HealthDrink;
            break;
    }

    Event_CommonItemTake(pickupType, eventFlagIdx);
}

void func_800CED88(void) // 0x800CED88
{
    typedef enum _EventState
    {
        EventState_Skip = 31
    } e_EventState;

    s32             step;
    s_SubCharacter* dahlia;

    #define dahliaChara (&g_SysWork.npcs[0])
    #define playerChara (&g_SysWork.playerWork.player)

    if (Pad_ButtonClickedBind(skip) &&
        g_SysWork.sysStateSteps[0] > 0 && g_SysWork.sysStateSteps[0] < EventState_Skip)
    {
        SysWork_StateStepSet(0, EventState_Skip);
    }

    step = g_SysWork.sysStateSteps[0];
    switch (step)
    {
        case 0:
            Player_ControlFreeze();
            Fs_QueueStartRead(FILE_ANIM_CHRC_DMS, FS_BUFFER_15);
            Fs_QueueWaitForEmpty();
            Dms_HeaderFixOffsets((s_DmsHeader*)FS_BUFFER_15);
            Chara_Load(0, Chara_Dahlia, &g_SysWork.npcBoneCoordBuffer[0], CHARA_FORCE_FREE_ALL, NULL, NULL);
            Chara_ProcessLoads();
            Chara_Spawn(Chara_Dahlia, 0, Q12(20.0f), Q12(23.5f), 0, 3);
            Chara_MovementReset(dahliaChara);

            g_Cutscene_Timer = Q12(0.0f);
            CutsceneBorder_ForceShow();
            g_SysWork.sysFlags |= SysFlag_CutsceneActive;

            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, playerChara, 51, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, dahliaChara, 0, false);
            func_8003D03C();

            Savegame_EventFlagSet(EventFlag_182);
            g_Cutscene_MapMsgAudioIdx = 0;

            SysWork_StateStepIncrement(0);

        case 1:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.0f), false);
            break;

        case 2:
            Event_DisplayMapMsgWithAudio(15, &g_Cutscene_MapMsgAudioIdx, g_Cutscene_MapMsgAudioCmds);
            break;

        case 3:
            Event_DisplayMapMsgWithAudio(18, &g_Cutscene_MapMsgAudioIdx, g_Cutscene_MapMsgAudioCmds);
            break;

        case 4:
            Event_DisplayMapMsgWithAudio(23, &g_Cutscene_MapMsgAudioIdx, g_Cutscene_MapMsgAudioCmds);
            Event_WaitTimer(Q12(1.2f), false);
            break;

        case 5:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, playerChara, 113, false);
            SysWork_StateStepIncrement(0);

        case 6:
            Event_DisplayMapMsgWithAudio(23, &g_Cutscene_MapMsgAudioIdx, g_Cutscene_MapMsgAudioCmds);

            if (g_Cutscene_Timer < Q12(1.0f))
            {
                g_Cutscene_Timer = Q12(1.0f);
                break;
            }

            g_Cutscene_Timer += Q12_MULT_PRECISE(g_DeltaTime, Q12(5.0f));
            if (g_Cutscene_Timer > Q12(23.0f))
            {
                g_Cutscene_Timer = Q12(23.0f);
            }
            break;

        case 7:
            Event_DisplayMapMsgWithAudio(0x18, &g_Cutscene_MapMsgAudioIdx, g_Cutscene_MapMsgAudioCmds);

            if (g_Cutscene_Timer < Q12(1.0f))
            {
                g_Cutscene_Timer = Q12(1.0f);
                break;
            }

            g_Cutscene_Timer += Q12_MULT_PRECISE(g_DeltaTime, Q12(5.0f));
            if (g_Cutscene_Timer > Q12(23.0f))
            {
                g_Cutscene_Timer = Q12(23.0f);
            }
            break;

        case 8:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimReset, playerChara, 0, false);
            Event_WaitTimer(Q12(0.8f), false);
            break;

        case 9:
            Event_DisplayMapMsgWithAudio(25, &g_Cutscene_MapMsgAudioIdx, g_Cutscene_MapMsgAudioCmds);
            break;

        case 10:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, dahliaChara, 9, false);
            SysWork_StateStepIncrement(0);

        case 11:
            Event_WaitTimer(Q12(0.5f), false);
            break;

        case 12:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AwaitAnimEnd, dahliaChara, 0, false);
            Event_DisplayMapMsgWithAudio(29, &g_Cutscene_MapMsgAudioIdx, g_Cutscene_MapMsgAudioCmds);
            break;

        case 13:
            Event_WaitTimer(Q12(2.0f), false);
            Event_DisplayMapMsgWithAudio(29, &g_Cutscene_MapMsgAudioIdx, g_Cutscene_MapMsgAudioCmds);
            break;

        case 14:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, dahliaChara, 10, false);
            SysWork_StateStepIncrement(0);

        case 15:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AwaitAnimEnd, dahliaChara, 0, false);
            Event_DisplayMapMsgWithAudio(29, &g_Cutscene_MapMsgAudioIdx, g_Cutscene_MapMsgAudioCmds);
            break;
        case 16:
            SysWork_StateStepIncrement(0);

        case 17:
            Event_DisplayMapMsgWithAudio(29, &g_Cutscene_MapMsgAudioIdx, g_Cutscene_MapMsgAudioCmds);
            break;
        case 18:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, dahliaChara, 8, false);
            SysWork_StateStepIncrement(0);

        case 19:
            Event_DisplayMapMsgWithAudio(33, &g_Cutscene_MapMsgAudioIdx, g_Cutscene_MapMsgAudioCmds);

            if (g_Cutscene_Timer >= Q12(142.0f))
            {
                g_Cutscene_Timer += Q12_MULT_PRECISE(g_DeltaTime, Q12(10.0f));

                if (g_Cutscene_Timer > Q12(165.0f))
                {
                    g_Cutscene_Timer = Q12(165.0f);
                }
                break;
            }

            g_Cutscene_Timer = Q12(142.0f);
            break;

        case 20:
            Event_WaitTimer(Q12(0.3f), false);

            if (g_Cutscene_Timer < Q12(142.0f))
            {
                g_Cutscene_Timer = Q12(142.0f);
                break;
            }

            g_Cutscene_Timer += Q12_MULT_PRECISE(g_DeltaTime, Q12(10.0f));
            if (g_Cutscene_Timer > Q12(165.0f))
            {
                g_Cutscene_Timer = Q12(165.0f);
            }
            break;

        case 21:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, dahliaChara, 1, false);
            SysWork_StateStepIncrement(0);

        case 22:
            Event_WaitTimer(Q12(0.8f), false);

            if (g_Cutscene_Timer >= Q12(166.0f))
            {
                g_Cutscene_Timer += Q12_MULT_PRECISE(g_DeltaTime, Q12(15.0f));

                if (g_Cutscene_Timer > Q12(180.0f))
                {
                    g_Cutscene_Timer = Q12(180.0f);
                }
                break;
            }

            g_Cutscene_Timer = Q12(166.0f);
            break;

        case 23:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, playerChara, 111, false);
            SysWork_StateStepIncrement(0);

        case 24:
            Event_DisplayMapMsgWithAudio(34, &g_Cutscene_MapMsgAudioIdx, g_Cutscene_MapMsgAudioCmds);

            if (g_Cutscene_Timer < Q12(166.0f))
            {
                g_Cutscene_Timer = Q12(166.0f);
                break;
            }

            g_Cutscene_Timer += Q12_MULT_PRECISE(g_DeltaTime, Q12(15.0f));
            if (g_Cutscene_Timer > Q12(253.0f))
            {
                g_Cutscene_Timer = Q12(253.0f);
            }
            break;

        case 25:
            if (g_Cutscene_Timer < Q12(166.0f))
            {
                g_Cutscene_Timer = Q12(166.0f);
            }
            else
            {
                g_Cutscene_Timer += Q12_MULT_PRECISE(g_DeltaTime, Q12(15.0f));
                if (g_Cutscene_Timer > Q12(253.0f))
                {
                    g_Cutscene_Timer = Q12(253.0f);
                    SysWork_StateStepIncrement(0);
                }
            }
            break;

        case 26:
            Event_WaitTimer(Q12(0.8f), false);
            break;

        case 27:
            Sfx_WithFlagsPlay(Sfx_Unk1323, &Q12_VECTOR3(12.0f, -1.2f, 24.0f), Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 28:
            Event_WaitTimer(Q12(1.2f), false);
            break;

        case 29:
            Sfx_WithFlagsPlay(Sfx_Unk1324, &Q12_VECTOR3(12.0f, -1.2f, 24.0f), Q8(0.5f), SfxFlag_None);
            Chara_ModelCharaIdClear(dahliaChara, 0, 0);
            SysWork_StateStepIncrement(0);

        case 30:
            Event_WaitTimer(Q12(0.5f), false);
            break;

        case EventState_Skip:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);

            if (g_SysWork.sysStateSteps[0] != EventState_Skip)
            {
                g_Cutscene_Timer = Q12(253.0f);
            }
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            vcReturnPreAutoCamWork(true);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);

            Savegame_EventFlagSet(EventFlag_179);
            Savegame_EventFlagSet(EventFlag_176);
            Savegame_EventFlagSet(EventFlag_MapMark_FogCentralTown_HospitalCircle);
            Savegame_EventFlagSet(EventFlag_MapMark_AltCentralTown_HospitalCircle);

            Chara_ModelCharaIdClear(dahliaChara, 0, 0);

            g_Cutscene_Timer = NO_VALUE;

            SD_Call(19);
            func_8003D01C();
            break;
    }

    if (step < 15)
    {
        if (step >= 10)
        {
            if (g_Cutscene_Timer < Q12(24.0f))
            {
                g_Cutscene_Timer = Q12(24.0f);
            }
            else
            {
                g_Cutscene_Timer += Q12_MULT_PRECISE(g_DeltaTime, Q12(10.0f));
                if (g_Cutscene_Timer > Q12(141.0f))
                {
                    g_Cutscene_Timer = Q12(141.0f);
                }
            }

            if (step > 13)
            {
                dahlia = dahliaChara;
                if ((dahlia->model.anim.time - Q12(Chara_AnimStartKeyframeIdxGet(dahlia))) <= Q12(30.0f) &&
                    (dahlia->model.anim.time - Q12(Chara_AnimStartKeyframeIdxGet(dahlia))) >= Q12(21.0f))
                {
                    Savegame_EventFlagSet(EventFlag_176);
                }
            }
        }
    }

    if (g_Cutscene_Timer >= Q12(0.0f))
    {
        Dms_CharacterTransformGet(&playerChara->position, (SVECTOR3*)&playerChara->rotation, "HERO", g_Cutscene_Timer, (s_DmsHeader*)FS_BUFFER_15);
        Dms_CharacterTransformGet(&dahliaChara->position, (SVECTOR3*)&dahliaChara->rotation, "DAHLIA", g_Cutscene_Timer, (s_DmsHeader*)FS_BUFFER_15);
        vcChangeProjectionValue(Dms_CameraTargetsGet(&g_CameraPositionTarget, &g_CameraLookAtTarget, NULL, g_Cutscene_Timer, (s_DmsHeader*)FS_BUFFER_15));
        vcUserCamTarget(&g_CameraPositionTarget, NULL, true);
        vcUserWatchTarget(&g_CameraLookAtTarget, NULL, true);
    }
}

void func_800CF798(void) // 0x800CF798
{
    Event_ItemTake(InvItemId_Flauros, DEFAULT_PICKUP_ITEM_COUNT, EventFlag_M2S01_PickupFlauros, 35);
}

void func_800CF7C4(void) // 0x800CF7C4
{
    Event_ItemTake(InvItemId_DrawbridgeKey, DEFAULT_PICKUP_ITEM_COUNT, EventFlag_M2S01_PickupDrawbridgeKey, 36);
}

s_WorldObjectPose  g_WorldObject_Key;
s_WorldObjectPose  g_WorldObject_Item;
s_WorldObjectPose  g_WorldObject_SavePad;
u8                 D_800D177C;
s8                 __pad_bss_800D177D[15];
s_WorldObjectModel g_CommonWorldObjects[6];

s_Pose g_CommonWorldObjectPoses[] = {
    { { Q12(15.5f), Q12(-1.12f), Q12(25.25f) }, { Q12_ANGLE(0.0f), Q12_ANGLE(0.0f), Q12_ANGLE(0.0f) } },
};

void Map_WorldObjectsInit(void)
{
    WorldEnv_PresetsSet(1, 1);

    WorldObject_PosePositionInit(&g_WorldObject_Item, "ITEM_HID", 19.8289f, -1.3633f, 24.0735f);
    WorldObject_Init(&g_WorldObject_Key, "KEY_HIDE", 20.073f, -0.858f, 24.1469f, 0.0f, 23.0f, 0.0f);
    WorldObject_Init(&g_WorldObject_SavePad, g_CommonWorldObjectNames[1], 18.3345f, -0.9102f, 14.1985f, 0.0f, 10.5f, 0.0f);

    D_800D177C = 0;

    WorldObject_ModelNameSet(&g_CommonWorldObjects[0], g_CommonWorldObjectNames[2]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[1], g_CommonWorldObjectNames[3]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[2], g_CommonWorldObjectNames[4]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[3], g_CommonWorldObjectNames[5]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[4], g_CommonWorldObjectNames[6]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[5], g_CommonWorldObjectNames[7]);
}

void Map_WorldObjectsUpdate(void) // 0x800CF938
{
    if (PLAYER_IN_MAP_CHUNK(0, 0))
    {
            if (Savegame_EventFlagGet(EventFlag_176) && !Savegame_EventFlagGet(EventFlag_M2S01_PickupFlauros))
            {
                WorldObjects_Add(&g_WorldObject_Item.object, &g_WorldObject_Item.position, &SVECTOR3_Zero);
            }

            if (!Savegame_EventFlagGet(EventFlag_M2S01_PickupDrawbridgeKey))
            {
                WorldObjects_Add(&g_WorldObject_Key.object, &g_WorldObject_Key.position, &g_WorldObject_Key.rotation);
                Savegame_EventFlagClear(EventFlag_MapMark_OldTown_BalkanChurchCheck);
            }
            else
            {
                Savegame_EventFlagSet(EventFlag_MapMark_OldTown_BalkanChurchCheck);
            }

            WorldObjects_Add(&g_WorldObject_SavePad.object, &g_WorldObject_SavePad.position, &g_WorldObject_SavePad.rotation);
    }

    if (D_800D177C == 0)
    {
        if (!Savegame_EventFlagGet(EventFlag_M2S01_PickupDrawbridgeKey))
        {
            if (PLAYER_NEAR_POS(vx, 20.0f, 0.7f) && PLAYER_NEAR_POS(vz, 23.9f, 0.5f))
            {
                Event_CameraPositionSet(NULL, Q12(19.57f), Q12(-2.91f), Q12(23.91f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
                Event_CameraLookAtSet(NULL, Q12(20.69f), Q12(0.92f), Q12(24.22f), Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f), true);
                D_800D177C = 1;
            }
        }
    }
    else
    {
        // NOTE: This macro uses `>` comparison, but this part of the code apparently uses `>=`
        // which would require a separate macro or slightly larger number to compensate.
        if (!(PLAYER_NEAR_POS(vx, 20.0f, 1.2004f) && PLAYER_NEAR_POS(vz, 23.9f, 1.0004f)))
        {
            vcReturnPreAutoCamWork(true);
            D_800D177C = 0;
        }
    }

    if (PLAYER_IN_MAP_CHUNK(0, 0))
    {
        if (!Savegame_EventFlagGet(EventFlag_M2S01_HealthDrink))
        {
            WorldObjects_Add(&g_CommonWorldObjects[1], &g_CommonWorldObjectPoses[0].position, &g_CommonWorldObjectPoses[0].rotation);
        }
    }
}
