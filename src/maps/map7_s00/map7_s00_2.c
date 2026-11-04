#include "bodyprog/bodyprog.h"
#include "bodyprog/chara/dms.h"
#include "bodyprog/events/event_data.h"
#include "bodyprog/gfx/map_effects.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player/player.h"
#include "bodyprog/sound/sound_system.h"
#include "main/rng.h"
#include "maps/map7/map7_s00.h"
#include "maps/common/characters/lisa.h"
#include "maps/common/particle.h"
#include "maps/common/characters/player.h"

#include "../src/maps/common/particles/particle.c"

#include "../src/maps/common/characters/player.c"

#include "../src/maps/common/characters/lisa.c"

#include "../src/maps/common/chara_util.c" // 0x800CF538

#include "../src/maps/common/map_util.c" // 0x800CFF40

void func_800CFFE8(s32 arg0) // 0x800CFFE8
{
    (arg0 != 0) ? func_800D0050() : sharedFunc_800D0110_7_s00();
}

void func_800D001C(s32 arg0) // 0x800D001C
{
    (arg0 != 0) ? func_800D00D0() : sharedFunc_800D0110_7_s00();
}

void func_800D0050(void) // 0x800D0050
{
    s32 bgmIdx; // `e_BgmCmd`
    u32 temp_v1;

    if (g_SavegamePtr->mapRoomIdx == 3)
    {
        bgmIdx = BgmCmd_Track15;
    }
    else if (Savegame_EventFlagGet(EventFlag_481))
    {
        if (!Savegame_EventFlagGet(EventFlag_574))
        {
            Savegame_EventFlagSet(EventFlag_574);
            bgmIdx = BgmCmd_UpdateTrack;
        }
        else
        {
            bgmIdx = BgmCmd_Track22;
        }
    }
    else
    {
        bgmIdx = BgmCmd_Track14;
    }

    Bgm_SongChange(bgmIdx);
}

void func_800D00D0(void) // 0x800D00D0
{
    Bgm_SongChange(Savegame_EventFlagGet(EventFlag_M5S03_KaufmannHasMotorcycleVial) ? 40 : 35);
}

#include "maps/common/shared/sharedFunc_800D0110_7_s00.h" // 0x800D0110

void GameBoot_LoadScreen_StageString(void) {}

#include "maps/common/shared/MapEvent_DoorJammed.h" // 0x800D099C

#include "maps/common/shared/MapEvent_DoorLocked.h" // 0x800D0A30

void func_800D0AC4(void) {}

#include "maps/common/shared/Event_CutsceneTimerAdvance.h" // 0x800D0ACC

const char* MAP_MESSAGES[] = {
    #include "maps/common/shared/map_msg_common.h"
#if VERSION_REGION_IS(NTSC)
    /* 15 */ "~J0(1.2)\t~E ",
    /* 16 */ "~J1(1.2)\tLisa? ",
    /* 17 */ "~J1(3.8)\tWhat_happened? ~N\n\t\t\tWhere's_Alessa_and_Dahlia? ",
    /* 18 */ "~J1(1.6)\tHarry,_listen. ",
    /* 19 */ "~J1(2.8)\tSomething_you_said_before ~N\n\t\t\thas_been_bothering_me. ",
    /* 20 */ "~J1(2.6)\tI_just_can't_get_it_out_of_my_head. ",
    /* 21 */ "~J1(1.6)\tWhat_is_it_Lisa? ",
    /* 22 */ "~J1(2.5)\tSo_I_went_to_look ~N\n\t\t\tin_the_basement. ",
    /* 23 */ "~J1(3.0)\tEven_though_I_was_scared ~N\n\t\t\tas_hell. ",
    /* 24 */ "~J1(3.1)\tLike_you_said, ~N\n\t\t\tthere_were_these_creepy_rooms. ",
    /* 25 */ "~J1(3.2) \tBut_nothing_really_unusual ~N\n\t\t\tdown_there. ",
    /* 26 */ "~J1(3.6)\tBut_while_I_was_down_there, ~N\n\t\t\tI_got_this_weird_feeling. ",
    /* 27 */ "~J1(2.2)\tLike_I'd_been_there_before. ",
    /* 28 */ "~J1(5.0)\tLike_something_happened_there, ~N\n\t\t\tbut_I_can't_quite_remember ~N\n\t\t\tsomehow. ",
    /* 29 */ "~J1(1.8)\tWhat_was_it? ~E ",
    /* 30 */ "~J0(2.4)\tHarry..._help_me... ",
    /* 31 */ "~J0(3.2)\tI'm_so_scared. ~N\n\t\t\tI_can't_take_this. ",
    /* 32 */ "~J0(2.0)\tIt's_only_a_temporary_thing. ",
    /* 33 */ "~J0(2.7)\tYou're_in_shock_from ~N\n\t\t\twhen_you_were_knocked_out. ",
    /* 34 */ "~J0(2.7)\tDon't_fret_about_it, ~N\n\t\t\tyou'll_remember_after_a_while... ~E ",
    /* 35 */ "~J0(1.5)\tNo. ",
    /* 36 */ "~J0(2.0)\tYou_don't_understand... ~E ",
    /* 37 */ "~J0(2.8)\tWait! ~N\n\t\t\tWhere_do_you_think ~N\n\t\t\tyou're_going? ~E ",
    /* 38 */ "~J0(2.0)\tLisa... ",
    /* 39 */ "~J0(3.0)\tDamn!__What's_that? ~E ",
    /* 40 */ "~J0(2.0)\tThat_sound.... ",
    /* 41 */ "~J0(4.0)\tFrom_down_there... ~N\n\t\t\tIt's_coming_from_the_basement... ~E ",
    /* 42 */ "~J0(2.4)\tI'm_being_summoned... ~E ",
    /* 43 */ "~J0(2.1)\tWhat_happened_in_this_town? ",
    /* 44 */ "~J0(2.7)\tWhat_could_be_making_things ~N\n\t\t\tlike_this? ",
    /* 45 */ "~J0(4.3)\tI_have_a_feeling_if_I_take ~N\n\t\t\tthe_elevator_down_I'll_find_it. ~E ",
    /* 46 */ "~J0(1.5)\t_ ~E ",
    /* 47 */ "\tNothing_unusual. ~E ",
    /* 48 */ "\tNothing_helpful. ~E "
#elif VERSION_REGION_IS(NTSCJ)
    /* 15 */ "~J0(1.2)\t~E ",
    /* 16 */ "~J1(1.2)\tリサ ",
    /* 17 */ "~J1(3.8)\tここは……？ ~N\n\t\t\tダリアとアレッサはどこに行ったんだ？ ",
    /* 18 */ "~J1(1.6)\tハリー…… ",
    /* 19 */ "~J1(2.8)\tあたしね……あなたと話してから ~N\n\t\t\tひどく気になることがあって…… ",
    /* 20 */ "~J1(2.6)\t頭の片隅に引っかかって離れないの ",
    /* 21 */ "~J1(1.6)\t……リサ？ ",
    /* 22 */ "~J1(2.5)\tだから……地下を見に行ったのよ ",
    /* 23 */ "~J1(3.0)\t怖くて……行きたくなかった ~N\n\t\t\tでも…… ",
    /* 24 */ "~J1(3.1)\tあなたが言っていたように ~N\n\t\t\t奇妙な部屋があったわ ",
    /* 25 */ "~J1(3.2)\tだけど特に変わった様子はなかった ",
    /* 26 */ "~J1(3.6)\tでもね……そこで変な感じがしたのよ ",
    /* 27 */ "~J1(2.2)\tあの部屋は初めてじゃないような ~N\n\t\t\t気がしたの…… ",
    /* 28 */ "~J1(5.0)\tあそこにあたしの覚えていない ~N\n\t\t\t何かがあるというの？ ",
    /* 29 */ "~J1(2.2)\tあたしは　なぜ…… ~E ",
    /* 30 */ "~J0(2.4)\tハリー……助けて…… ",
    /* 31 */ "~J0(3.2)\tひどく怖いのよ…… ~N\n\t\t\t耐えられないわ ",
    /* 32 */ "~J0(2.0)\t……一時的なものだろう ",
    /* 33 */ "~J0(2.7)\t気を失った時のショックのせいだ ",
    /* 34 */ "~J0(2.7)\t心配しなくても　時間が経てば思い出す…… ~E ",
    /* 35 */ "~J0(1.5)\t駄目よ ",
    /* 36 */ "~J0(2.0)\tあなたにはわからないわ ~E ",
    /* 37 */ "~J0(2.8)\t待て！ ~N\n\t\t\tいったいどうしたんだ？ ~E ",
    /* 38 */ "~J0(2.0)\tリサ…… ",
    /* 39 */ "~J0(3.0)\tくそっ……いったい ~N\n\t\t\tどうなってるんだ？ ~E ",
    /* 40 */ "~J0(2.0)\tこの音…… ",
    /* 41 */ "~J0(4.0)\t足下からだ…… ~N\n\t\t\t地下から　響いてきている…… ~E ",
    /* 42 */ "~J0(2.4)\tわたしを呼んでいるのか…… ~E ",
    /* 43 */ "~J0(2.1)\tこの町で何が起こったのか ",
    /* 44 */ "~J0(2.7)\tなぜ　こんなことになってしまったのか ",
    /* 45 */ "~J0(4.3)\tこれを降りれば ~N\n\t\t\tはっきりするような気がする…… ~E ",
    /* 46 */ "~J0(1.5)\t　 ~E ",
    /* 47 */ "\t特に気になるものはない ~E ",
    /* 48 */ "\t役に立ちそうなものはない ~E "
#endif
};

void func_800D0B64(void) // 0x800D0B64
{
    typedef enum _EventState
    {
        EventState_Skip = 27
    } e_EventState;

    #define playerChara g_SysWork.playerWork.player
    #define lisaChara g_SysWork.npcs[0]

    // Skip.
    if ((g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.skip) &&
        g_SysWork.sysStateSteps[0] > 0 && g_SysWork.sysStateSteps[0] < EventState_Skip)
    {
        SysWork_StateStepSet(0, EventState_Skip);
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Savegame_EventFlagClear(EventFlag_476);
            func_8003A16C();

            g_Cutscene_MapMsgAudioIdx = 0;
            D_800D31D8 = 0;

            Savegame_EventFlagSet(EventFlag_476);
            Fs_QueueStartRead(FILE_ANIM_HSPTL6_DMS, FS_BUFFER_11);
            Fs_QueueWaitForEmpty();
            Dms_HeaderFixOffsets((s_DmsHeader*)FS_BUFFER_11);

            g_Cutscene_Timer             = Q12(0.0f);
            g_SysWork.lightBoneCoord     = NULL;
            g_SysWork.lensFlareBoneCoord = NULL;
            g_SysWork.lightIntensity     = Q12(0.5f);

            Math_Vector3Set(&g_SysWork.lightPosition, Q12(18.7f), Q12(-1.9f), Q12(-137.8f));
            Math_SetSVectorFast(&g_SysWork.lightRotation, Q12_ANGLE(-34.0f), Q12_ANGLE(-100.0f), 0);
            func_8008D438();

            CutsceneBorder_ForceShow();

            ScreenFade_ResetTimestep();

            g_SysWork.sysFlags |= SysFlag_CutsceneActive;

            Chara_Spawn(Chara_Lisa, 0, Q12(17.4f), Q12(139.1f), 0, 3);
            SysWork_StateStepIncrement(0);

            SD_Call(Sfx_Unk1640);

            D_800D4362 = 0;

            func_8003D03C();
            Player_EmptyWeaponHandSet();
            break;

        case 1:
            Event_DisplayMapMsgWithAudio(15, &g_Cutscene_MapMsgAudioIdx, &g_Cutscene_MapMsgAudioCmds);
            break;

        case 2:
            Event_WaitTimer(Q12(1.5f), false);
            break;

        case 3:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &playerChara, 135, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &lisaChara, 9, false);
            SysWork_StateStepIncrement(0);

        case 4:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, Q12(0.8f), false);
            break;

        case 5:
            Event_DisplayMapMsgWithAudio(16, &g_Cutscene_MapMsgAudioIdx, &g_Cutscene_MapMsgAudioCmds);
            break;

        case 6:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &lisaChara, 14, false);
            SysWork_StateStepIncrement(0);

        case 7:
            Event_DisplayMapMsgWithAudio(30, &g_Cutscene_MapMsgAudioIdx, &g_Cutscene_MapMsgAudioCmds);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(2.5f), Q12(1.0f), Q12(44.0f), false, true);
            break;

        case 8:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &playerChara, 136, false);
            SysWork_StateStepIncrement(0);

        case 9:
            Event_DisplayMapMsgWithAudio(30, &g_Cutscene_MapMsgAudioIdx, &g_Cutscene_MapMsgAudioCmds);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(2.5f), Q12(1.0f), Q12(62.0f), true, false);
            break;

        case 10:
            Event_WaitTimer(Q12(0.4f), false);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(2.5f), Q12(1.0f), Q12(62.0f), true, false);
            break;

        case 11:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &lisaChara, 15, false);
            Savegame_EventFlagSet(EventFlag_474);
            SysWork_StateStepIncrement(0);

        case 12:
            Event_DisplayMapMsgWithAudio(35, &g_Cutscene_MapMsgAudioIdx, &g_Cutscene_MapMsgAudioCmds);
            Event_WaitTimer(Q12(0.5f), false);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(2.5f), Q12(1.0f), Q12(65.0f), true, false);
            break;

        case 13:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &playerChara, 137, false);
            SysWork_StateStepIncrement(0);

        case 14:
            Event_DisplayMapMsgWithAudio(35, &g_Cutscene_MapMsgAudioIdx, &g_Cutscene_MapMsgAudioCmds);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(2.5f), Q12(1.0f), Q12(65.0f), true, false);
            break;

        case 15:
            SysWork_StateStepIncrement(0);

        case 16:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &playerChara, 138, false);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &lisaChara, 16, false);
            SysWork_StateStepIncrement(0);

        case 17:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(7.5f), Q12(65.0f), Q12(81.0f), true, true);
            Event_DisplayMapMsgWithAudio(37, &g_Cutscene_MapMsgAudioIdx, &g_Cutscene_MapMsgAudioCmds);
            break;

        case 18:
            Chara_ModelCharaIdClear(&lisaChara, 0, 0);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &playerChara, 139, false);

            Math_Vector3Set(&g_SysWork.lightPosition, Q12(16.3f), Q12(-1.55f), Q12(-138.6f));

            Math_SetSVectorFast(&g_SysWork.lightRotation, Q12_ANGLE(-18.0f), Q12_ANGLE(65.0f), Q12_ANGLE(0.0f));

            Sfx_WithFlagsPlay(Sfx_DoorOpen0, &Q12_VECTOR3(14.6f, -1.2f, -138.5f), Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 19:
            Event_DisplayMapMsgWithAudio(37, &g_Cutscene_MapMsgAudioIdx, &g_Cutscene_MapMsgAudioCmds);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(8.0f), Q12(82.0f), Q12(105.0f), true, false);
            break;

        case 20:
            Sfx_WithFlagsPlay(Sfx_DoorClose0, &Q12_VECTOR3(14.6f, -1.2f, -138.5f), Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case 21:
            Event_WaitTimer(Q12(2.5f), false);
            break;
        case 22:
            Event_DisplayMapMsg(false, 38, 0, 0, 0, false);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(8.0f), Q12(82.0f), Q12(105.0f), true, false);
            break;

        case 23:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(8.0f), Q12(82.0f), Q12(105.0f), true, true);
            break;

        case 24:
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &playerChara, 51, false);
            g_SysWork.lightIntensity = Q12(0.6f);
            SysWork_StateStepIncrement(0);

        case 25:
            Event_DisplayMapMsg(false, 40, 0, 0, 0, false);
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(6.0f), Q12(106.0f), Q12(150.0f), true, false);
            break;

        case 26:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(6.0f), Q12(106.0f), Q12(150.0f), true, true);
            break;

        case EventState_Skip:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.0f), false);
            break;

        case 28:
            g_Cutscene_Timer = Q12(150.0f);
            SysWork_StateStepIncrement(0);
            break;

        default:
            Player_ControlUnfreeze(true);
            SysWork_StateSetNext(SysState_Gameplay);

            vcReturnPreAutoCamWork(true);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, Q12(0.0f), false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, Q12(0.0f), false);
            func_8008D448();
            Game_FlashlightAttribsSet();

            g_SysWork.lightIntensity = Q12(1.0f);

            Savegame_EventFlagSet(EventFlag_471);
            Chara_ModelCharaIdClear(&lisaChara, 0, 0);

            g_Cutscene_Timer = NO_VALUE;

            SD_Call(19);

            D_800D4362 = Q12(1.0f);

            func_8003D01C();
            Player_WeaponAttackRestore();
            playerChara.position.vx = Q12(17.8f);
            playerChara.position.vz = Q12(-137.7f);
            break;
    }

    if (g_Cutscene_Timer >= Q12(0.0f))
    {
        Dms_CharacterTransformGet(&playerChara.position, &playerChara.rotation, "HERO", g_Cutscene_Timer, (s_DmsHeader*)FS_BUFFER_11);

        if (g_SysWork.sysStateSteps[0] < 18)
        {
            Dms_CharacterTransformGet(&lisaChara.position, &lisaChara.rotation, "LISA", g_Cutscene_Timer, (s_DmsHeader*)FS_BUFFER_11);
        }

        vcChangeProjectionValue(Dms_CameraTargetsGet(&g_Cutscene_CameraPosition, &g_Cutscene_CameraLookAt, NULL, g_Cutscene_Timer, (s_DmsHeader*)FS_BUFFER_11));
        vcUserCamTarget(&g_Cutscene_CameraPosition, NULL, true);
        vcUserWatchTarget(&g_Cutscene_CameraLookAt, NULL, true);

        if (g_SysWork.sysStateSteps[0] == 1 || g_SysWork.sysStateSteps[0] == 2)
        {
            D_800D4362 += g_DeltaTime >> 1;
            if (D_800D4362 > Q12(0.25f))
            {
                D_800D4362 = Q12(0.25f);
            }
        }

        if (g_SysWork.sysStateSteps[0] >= 10)
        {
            D_800D4362 += Q12_MULT_PRECISE(Rng_GenerateUInt(Q12(0.5f), Q12_CLAMPED(1.0f)), g_DeltaTime) >> 3;
            if (D_800D4362 > Q12(1.0f))
            {
                D_800D4362 = Q12(1.0f);
            }
        }

        Sfx_WithFalloffAndPitchPlay(Sfx_Unk1640, &Q12_VECTOR3(17.8f, 2.0f, -139.1f), D_800D4362 >> 5, Q12(32.0f), Rng_GenerateUInt(-8, 7));
    }
}

void func_800D1604(void) // 0x800D1604
{
    typedef struct
    {
        SPRT*     field_0;
        DR_TPAGE* field_4;
        DR_STP*   field_8;
        s8        unk_C[8];
        TILE*     field_14;
        s32       field_18;
    } s_func_800D1604;

    s32              temp_s0_2;
    s32              temp_s0_3;
    s32              temp_s1;
    s32              temp_s1_2;
    s32              temp_v0;
    s32              temp_v1;
    s32              j;
    s32              i;
    s32              var_t5;
    s32              temp;
    s_func_800D1604* ptr;

    ptr = PSX_SCRATCH;

    ptr->field_18 = g_ActiveBufferIdx;

    if (g_SysWork.sysStateSteps[0] >= 7)
    {
        temp_v0 = Screen_FadeInProgressGet();
        temp_v1 = (g_GameWork.config.brightness * 8) + 4;

        ptr->field_14 = GsOUT_PACKET_P;

        for (i = 0; i < 4; i++)
        {
            j    = i & 0x1;
            temp = Q12_MULT_PRECISE(temp_v1, temp_v0);

            setRGBC0(ptr->field_14, temp, temp, temp, PRIM_RECT | RECT_BLEND);
            setXY0Fast(ptr->field_14, (j << 8) - (SCREEN_WIDTH / 2), -(FRAMEBUFFER_HEIGHT_PROGRESSIVE / 2));

            setWH(ptr->field_14, (j != 0) ? 64 : 256, FRAMEBUFFER_HEIGHT_PROGRESSIVE);

            addPrimFast(&g_OrderingTable2[g_ActiveBufferIdx].org[3], ptr->field_14, 3);
            ptr->field_14++;
            ptr->field_4 = ptr->field_14;
            setDrawTPage(ptr->field_4, 0, 0, getTPage(2, (i >= 2) ? 2 : 1, ((ptr->field_18 << 4) + (j << 2)) << 6, (((ptr->field_18 << 4) >> 4) & 0x1) << 8));
            AddPrim(&g_OrderingTable2[g_ActiveBufferIdx].org[3], ptr->field_4);
            ptr->field_4++;

            ptr->field_14 = ptr->field_4;
        }

        GsOUT_PACKET_P = ptr->field_14;
    }

    ptr->field_0 = GsOUT_PACKET_P;

    for (j = 0; j < 2; j++)
    {
        setRGBC0(ptr->field_0, 128, 128, 128, PRIM_RECT | RECT_BLEND | RECT_TEXTURE);
        setXY0Fast(ptr->field_0, ((j) << 8) - 160, -112);
        setUV0(ptr->field_0, 0, (ptr->field_18 == 0) << 5);
        setWH(ptr->field_0, (j != 0) ? 64 : 256, FRAMEBUFFER_HEIGHT_PROGRESSIVE);
        addPrimFast(&g_OrderingTable2[g_ActiveBufferIdx].org[15], ptr->field_0, 4);
        ptr->field_0++;

        ptr->field_4 = ptr->field_0;

        setDrawTPage(ptr->field_4, 0, 0, getTPage(2, 0, ((ptr->field_18 << 4) + (j << 2)) << 6, (((ptr->field_18 << 4) >> 4) & 1) << 8));
        AddPrim(&g_OrderingTable2[g_ActiveBufferIdx].org[15], ptr->field_4);

        ptr->field_4++;
        ptr->field_0 = ptr->field_4;
    }

    ptr->field_8 = ptr->field_0;

    SetDrawStp(ptr->field_8, 1);
    addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[ORDERING_TABLE_SIZE - 1], ptr->field_8);
    ptr->field_8++;

    SetDrawStp(ptr->field_8, 0);
    addPrim(&g_OrderingTable2[g_ActiveBufferIdx].org[0], ptr->field_8);
    ptr->field_8++;

    GsOUT_PACKET_P = ptr->field_8;

    if (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.skip)
    {
        if (g_SysWork.sysStateSteps[0] >= 3)
        {
            if (g_SysWork.sysStateSteps[0] < 12)
            {
                SysWork_StateStepSet(0, 12);
            }
        }
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 2, 0, false);

            g_Cutscene_MapMsgAudioIdx = 0;
            D_800D31DC[1]    = 0;
            D_800D31DC[0]    = 0;

            Event_TweenReset(0);
            Event_TweenReset(1);
            Event_TweenReset(2);
            Event_TweenReset(3);
            Event_TweenReset(4);
            Event_TweenReset(5);

            // Warp camera.
            Event_CameraPositionSet(NULL, Q12(58.21f), Q12(-7.23f), Q12(-144.62f), 0, 0, 0, 0, true);
            Event_CameraLookAtSet(NULL, Q12(56.92f), Q12(-4.5f), Q12(-141.99f), 0, 0, 0, 0, true);

            // Warp player.
            g_SysWork.playerWork.player.position.vx = Q12(57.45f);
            g_SysWork.playerWork.player.position.vz = Q12(-139.05f);
            g_SysWork.playerWork.player.rotation.vy = Q12(-0.25f);
            Game_TurnFlashlightOn();

            Savegame_EventFlagSet(EventFlag_475);
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            Event_ScreenFadeCmd(ScreenFadeCmd_Wait, true, 2, 0, false);
            break;

        case 6:
            Event_DisplayMapMsgWithAudio(42, &g_Cutscene_MapMsgAudioIdx, &D_800D31C4);

        case 5:
            if (g_SysWork.sysStateSteps[0] == 5)
            {
                Event_WaitTimer(Q12(2.5f), false);
            }

            D_800D31DC[1] = MIN(D_800D31DC[1] + Q12_MULT_PRECISE(g_DeltaTime, Q12(0.32f)), Q12(0.14f));

        case 4:
            if (g_SysWork.sysStateSteps[0] == 4)
            {
                Event_WaitTimer(Q12(0.9f), false);
                var_t5 = 2;
            }
            else
            {
                var_t5 = 4;
            }
            D_800D31DC[0] = MIN(D_800D31DC[0] + Q12_MULT_PRECISE(g_DeltaTime, Q12(0.32f)), Q12(0.14f));

            for (j = 0; j < var_t5; j++)
            {
                if (j & 0x1)
                {
                    D_800D4370[j] -= Q12_MULT_PRECISE(g_DeltaTime, D_800D31DC[j >> 1]);
                    if (D_800D4370[j] < Q12(-145.45f))
                    {
                        D_800D4370[j] = Q12(-145.45f);
                    }
                }
                else
                {
                    D_800D4370[j] += Q12_MULT_PRECISE(g_DeltaTime, D_800D31DC[j >> 1]);
                    if (D_800D4370[j] > Q12(-129.45f))
                    {
                        D_800D4370[j] = Q12(-129.45f);
                    }
                }
            }

        case 3:
            if (g_SysWork.sysStateSteps[0] == 3)
            {
                Event_WaitTimer(0x1333, false);
            }

            temp_s1   = Event_TweenLinear(Q12(-0.37f) - 1, Q12(6.0f), 0) + Q12(58.21f);
            temp_s0_2 = Event_TweenLinear(Q12(4.34f) + 1, Q12(6.0f), 1) - Q12(7.23f);

            Event_CameraPositionSet(NULL, temp_s1, temp_s0_2, Event_TweenLinear(Q12(3.08f) + 1, Q12(6.0f), 2) - Q12(144.62f), 0, 0, 0, 0, false);

            temp_s1_2 = Event_TweenLinear(Q12(-2.25f), Q12(6.0f), 3) + Q12(56.92f);
            temp_s0_3 = Event_TweenLinear(Q12(2.96f) + 1, Q12(6.0f), 4) - Q12(4.5f);

            Event_CameraLookAtSet(NULL, temp_s1_2, temp_s0_3, Event_TweenLinear(Q12(2.5f), Q12(6.0f), 5) - Q12(141.99f), 0, 0, 0, 0, false);
            break;

        case 7:
            // Warp player.
            g_SysWork.playerWork.player.position.vx = Q12(57.0f);
            g_SysWork.playerWork.player.position.vz = Q12(-139.05f);

            // Warp camera.
            Event_CameraPositionSet(NULL, Q12(58.5f), Q12(-2.9f), g_SysWork.playerWork.player.position.vz, 0, 0, 0, 0, true);
            Event_CameraLookAtSet(NULL, g_SysWork.playerWork.player.position.vx - Q12(1.6f), Q12(-0.3f), g_SysWork.playerWork.player.position.vz, 0, 0, 0, 0, true);

            Event_PathWaypointSet(true, 0, 0, Q12_ANGLE(-90.0f), Q12(51.8f), Q12(-139.05f));

            g_SysWork.playerWork.player.rotation.vy = Q12(-0.25f);
            D_800D31DC[1]                                   = 0;
            D_800D31DC[0]                                   = 0;

            SysWork_StateStepIncrement(0);

        case 8:
            Event_PathWaypointExecutePlayer(53, 1, false);
            Event_DisplayMapMsgWithAudio(43, &g_Cutscene_MapMsgAudioIdx, &D_800D31C4);
            Event_CameraLookAtSet(&g_SysWork.playerWork.player.position, Q12(-1.6f), Q12(-0.3f), 0, 0, 0, 0, 0, false);

            g_DeltaTime >>= 2;

            Event_WaitTimer(Q12(6.0f), false);
            break;

        case 9:
            Event_PathWaypointExecutePlayer(53, 1, false);
            Event_DisplayMapMsgWithAudio(43, &g_Cutscene_MapMsgAudioIdx, &D_800D31C4);
            Event_CameraLookAtSet(&g_SysWork.playerWork.player.position, Q12(-1.6f), Q12(-0.3f), 0, 0, 0, 0, 0, false);

            g_DeltaTime >>= 2;
            D_800D31DC[0] = MIN(D_800D31DC[0] + Q12_MULT_PRECISE(g_DeltaTime, Q12(0.55f)), Q12(0.3f));

            for (j = 0; j < 4; j++)
            {
                if (j & 0x1)
                {
                    D_800D4370[j] = D_800D4370[j] + Q12_MULT_PRECISE(g_DeltaTime, D_800D31DC[j >> 1]);
                }
                else
                {
                    D_800D4370[j] = D_800D4370[j] - Q12_MULT_PRECISE(g_DeltaTime, D_800D31DC[j >> 1]);
                }
            }
            break;

        case 10:
            Event_PathWaypointExecutePlayer(53, 1, false);
            Event_CameraLookAtSet(&g_SysWork.playerWork.player.position, Q12(-1.6f), Q12(-0.3f), 0, 0, 0, 0, 0, false);

            g_DeltaTime >>= 2;
            D_800D31DC[0] = MIN(D_800D31DC[0] + Q12_MULT_PRECISE(g_DeltaTime, Q12(0.55f)), Q12(0.3f));

            for (j = 0; j < 4; j++)
            {
                if (j & 0x1)
                {
                    D_800D4370[j] = D_800D4370[j] + Q12_MULT_PRECISE(g_DeltaTime, D_800D31DC[j >> 1]);
                }
                else
                {
                    D_800D4370[j] = D_800D4370[j] - Q12_MULT_PRECISE(g_DeltaTime, D_800D31DC[j >> 1]);
                }
            }

            Event_WaitTimer(Q12(3.0f), false);
            break;

        case 11:
            Event_PathWaypointExecutePlayer(53, 1, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, Q12(0.6f), false);
            Event_DisplayMapMsgWithAudio(46, &g_Cutscene_MapMsgAudioIdx, &D_800D31C4);

            g_DeltaTime >>= 2;
            D_800D31DC[0] = MIN(D_800D31DC[0] + Q12_MULT_PRECISE(g_DeltaTime, 0x428), 0x1EB);

            for (j = 0; j < 4; j++)
            {
                if (j & 1)
                {
                    D_800D4370[j] = D_800D4370[j] + Q12_MULT_PRECISE(g_DeltaTime, D_800D31DC[(j >> 1)]);
                }
                else
                {
                    D_800D4370[j] = D_800D4370[j] - Q12_MULT_PRECISE(g_DeltaTime, D_800D31DC[(j >> 1)]);
                }
            }
            break;

        case 12:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, 0, false);
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            vcReturnPreAutoCamWork(true);
            Savegame_EventFlagSet(EventFlag_472);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, 0, false);

            D_800D4362 = 0;
            break;
    }

    if (g_SysWork.sysStateSteps[0] >= 3)
    {
        D_800D4362 -= g_DeltaTime >> 3;
        if (D_800D4362 < 0)
        {
            D_800D4362 = 0;
        }
    }
}

void Map_WorldObjectsInit(void) // 0x800D2700
{
    s32    i;
    q19_12 z;

    WorldObject_ModelNameSet(&g_WorldObject0[0], "DR3R_HID");
    WorldObject_ModelNameSet(&g_WorldObject0[1], "DR3L_HID");
    WorldObject_ModelNameSet(&g_WorldObject0[2], "DR1R_HID");
    WorldObject_ModelNameSet(&g_WorldObject0[3], "DR1L_HID");
    WorldObject_ModelNameSet(&g_WorldObject0[4], "DR2R_HID");
    WorldObject_ModelNameSet(&g_WorldObject0[5], "DR2L_HID");

    z = Q12(-137.45f);

    for (i = 3; i >= 0; i--)
    {
        D_800D4370[i] = z;
    }

    WorldObject_Init(&g_WorldObject1, g_CommonWorldObjectNames[1], 15.63f, -0.91f, -140.55f, 0.0f, -90.0f, 0.0f);

    if (Savegame_EventFlagGet(EventFlag_471))
    {
        SD_Call(Sfx_Unk1640);
        D_800D4362 = Q12(1.0f);
    }

    Savegame_EventFlagClear(EventFlag_219);

    if (Savegame_EventFlagGet(EventFlag_M2S04_SeenPoliceChalkboard) && Savegame_EventFlagGet(EventFlag_M4S01_InspectedChalice))
    {
        Savegame_EventFlagSet(EventFlag_M7S00_BotanyBookReadable);
    }
}

void Map_WorldObjectsUpdate(void) // 0x800D286C
{
    VECTOR3 pos;
    s32     i;

    MAP_CHUNK_CHECK_VARIABLE_DECL();

    if (PLAYER_IN_MAP_CHUNK(vx, 1, 2, -1, 2) && PLAYER_IN_MAP_CHUNK(vz, 1, -4, -1, -4))
    {
        for (i = 0; i < ARRAY_SIZE(g_WorldObject0); i++)
        {
            pos.vx = Q12(60.8f);
            pos.vy = Q12(0.0f);
            pos.vz = D_800D4370[(i >= 2) ? (i - 2) : i];

            WorldObjects_Add(&g_WorldObject0[i], &pos, &D_800CB61C);
        }

        Sd_SfxAttributesUpdate(Sfx_Unk1640, 0, ((0xFF - (D_800D4362 >> 4)) >= 0x20) ? ~(D_800D4362 >> 4) : 0x20, Rng_GenerateUInt(-8, 7));
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 0, 0, -1, 1) && PLAYER_IN_MAP_CHUNK(vz, 1, -4, -1, -4))
    {
        WorldObjects_Add(&g_WorldObject1.object, &g_WorldObject1.position, &g_WorldObject1.rotation);

        if (Savegame_EventFlagGet(EventFlag_471))
        {
            Sd_SfxAttributesUpdate(Sfx_Unk1640, 0, ((0xFF - (D_800D4362 >> 4)) >= 0x20) ? ~(D_800D4362 >> 4) : 0x20, Rng_GenerateUInt(-8, 7));
        }
    }
}

const SVECTOR3 D_800CB61C = { 0, 0, 0 };
