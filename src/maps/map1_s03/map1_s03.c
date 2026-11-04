#include "inline_no_dmpsx.h"

#include <psyq/gtemac.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/chara/dms.h"
#include "bodyprog/events/event_data.h"
#include "bodyprog/gfx/map_effects.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player/player.h"
#include "bodyprog/item_screens.h"
#include "bodyprog/sound/sound_system.h"
#include "main/rng.h"
#include "maps/map1/map1_s03.h"
#include "maps/particle.h"
#include "maps/characters/creeper.h"
#include "maps/characters/locker_dead_body.h"
#include "maps/characters/player.h"
#include "maps/characters/stalker.h"

#include "../src/maps/particle_water.c"

void func_800CDCE0(s32 arg0, s32 arg1, s32 arg2) // 0x800CDCE0
{
    s16 tmp0;
    s16 offsetX; // } Implied Q8.
    s16 offsetZ; // }
    s32 zOff2;
    s32 i;

    switch (D_800E3A40[arg1].idx_14)
    {
        case 0:
            sharedData_800DFB7C_0_s00[arg0].field_0.vx_0 = D_800E3A40[arg1].vx_0;
            sharedData_800DFB7C_0_s00[arg0].field_4.vz_4 = D_800E3A40[arg1].vz_4;
            break;

        case 1:
            // TODO: Not sure if this fits any RNG macro?
            tmp0 = (Rng_Rand16() % D_800E3A40[arg1].field_C) - (D_800E3A40[arg1].field_C >> 1);
            sharedData_800DFB7C_0_s00[arg0].field_0.vx_0 = D_800E3A40[arg1].vx_0 + FP_FROM((tmp0 * Math_Sin(D_800E3A40[arg1].field_A)), Q12_SHIFT);
            zOff2 = FP_FROM((tmp0 * Math_Cos(D_800E3A40[arg1].field_A)), Q12_SHIFT);
            sharedData_800DFB7C_0_s00[arg0].field_4.vz_4 = D_800E3A40[arg1].vz_4 + zOff2;
            break;

        case 2:
            for (i = 0; i < 8; i++)
            {
                offsetX = Rng_GenerateInt(-D_800E3A40[arg1].field_C, D_800E3A40[arg1].field_C - 1);
                offsetZ = Rng_GenerateInt(-D_800E3A40[arg1].field_C, D_800E3A40[arg1].field_C - 1);
                if ((SQUARE(offsetX) + SQUARE(offsetZ)) < (D_800E3A40[arg1].field_C * D_800E3A40[arg1].field_C))
                {
                    break;
                }
            }

            if (i == 8)
            {
                offsetZ = 0;
                offsetX = 0;
            }

            sharedData_800DFB7C_0_s00[arg0].field_0.vx_0 = D_800E3A40[arg1].vx_0 + offsetX;
            sharedData_800DFB7C_0_s00[arg0].field_4.vz_4 = D_800E3A40[arg1].vz_4 + offsetZ;
            break;

        case 3:
            offsetX = Rng_GenerateUInt(0, Q12_ANGLE(360.0f) - 1);
            sharedData_800DFB7C_0_s00[arg0].field_0.vx_0 = D_800E3A40[arg1].vx_0 + FP_FROM((D_800E3A40[arg1].field_C * Math_Sin(offsetX)), Q12_SHIFT);
            zOff2 = FP_FROM((D_800E3A40[arg1].field_C * Math_Cos(offsetX)), Q12_SHIFT);
            sharedData_800DFB7C_0_s00[arg0].field_4.vz_4 = D_800E3A40[arg1].vz_4 + zOff2;
            break;

        case 4:
            offsetX = Rng_AddGeneratedUInt(D_800E3A40[arg1].field_A, Q12_ANGLE(-90.0f), Q12_ANGLE(90.0f) - 1);
            sharedData_800DFB7C_0_s00[arg0].field_0.vx_0 = D_800E3A40[arg1].vx_0 + FP_FROM((D_800E3A40[arg1].field_C * Math_Sin(offsetX)), Q12_SHIFT);
            zOff2 = FP_FROM((D_800E3A40[arg1].field_C * Math_Cos(offsetX)), Q12_SHIFT);
            sharedData_800DFB7C_0_s00[arg0].field_4.vz_4 = D_800E3A40[arg1].vz_4 + zOff2;
            break;
    }

    sharedData_800DFB7C_0_s00[arg0].vy_8 = D_800E3A40[arg1].field_8;

    if (arg2 == 0)
    {
        sharedData_800DFB7C_0_s00[arg0].field_C.s_0.field_0 = Rng_GenerateInt(0, D_800E3A40[arg1].field_10 - 1);
    }
    else
    {
        sharedData_800DFB7C_0_s00[arg0].field_C.s_0.field_0 = sharedData_800DFB7C_0_s00[arg0].field_C.field_0 - D_800E3A40[arg1].field_10;
    }

    sharedData_800DFB7C_0_s00[arg0].field_C.s_1.field_2  = Rng_GenerateUInt(0, 3);
    sharedData_800DFB7C_0_s00[arg0].field_C.s_1.field_3  = Rng_GenerateUInt(96, 159);
    sharedData_800DFB7C_0_s00[arg0].field_10.s_2.field_0 = arg1;
}

void func_800CE0CC(s32 arg0) // 0x800CE0CC
{
    s32 idx;
    s32 i;

    for (i = 0; i < D_800E3A40[arg0].field_12; i++)
    {
        idx = func_8005E7E0(28);
        if (idx == NO_VALUE)
        {
            break;
        }

        func_800CDCE0(idx, arg0, 0);
    }
}

bool func_800CE164(POLY_FT4** poly, s32 idx) // 0x800CE164
{
    typedef struct
    {
        s_func_8005E89C field_0;
        SVECTOR         field_12C;
        DVECTOR         field_134;
        s32             field_138;
    } s_ScratchData;

    s_ScratchData* scratch = PSX_SCRATCH;
    s32            idx2;
    s32            var_lo;
    s32            var_a2;
    s32            var_a1;
    s32            rgbColor;

    sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_0 += g_DeltaTime;

    idx2 = sharedData_800DFB7C_0_s00[idx].field_10.s_2.field_0;
    if ((u16)sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_0 > D_800E3A40[idx2].field_10)
    {
        func_800CDCE0(idx, idx2, 1);
        return false;
    }

    Math_SetSVectorFastSum(&scratch->field_12C,
                           Q12_TO_Q8(sharedData_800DFB7C_0_s00[idx].field_0.vx_0) - (u16)scratch->field_0.field_0.vx,
                           Q12_TO_Q8((s32)sharedData_800DFB7C_0_s00[idx].vy_8) - scratch->field_0.field_0.vy,
                           Q12_TO_Q8(sharedData_800DFB7C_0_s00[idx].field_4.vz_4) - scratch->field_0.field_0.vz);

    gte_ldv0(&scratch->field_12C);
    gte_rtps();
    gte_stsxy(&scratch->field_134.vx);
    gte_stsz(&scratch->field_138);

    var_lo = FP_TO((u16)sharedData_800DFB7C_0_s00[idx].field_C.s_0.field_0, Q12_SHIFT) / D_800E3A40[idx2].field_10;

    setPolyFT4(*poly);

    var_a2 = (FP_FROM(Q12_MULT_PRECISE(D_800E3A40[idx2].field_E * sharedData_800DFB7C_0_s00[idx].field_C.s_1.field_3, Math_Sin(var_lo >> 2)), Q12_SHIFT) * scratch->field_0.field_2C) / scratch->field_138;
    var_a1 = (var_a2 * (0x800 - ABS(scratch->field_0.field_30.vx))) >> 10;

    setXY0Fast(*poly, (u16)scratch->field_134.vx - var_a2, scratch->field_134.vy - var_a1);
    setXY1Fast(*poly, (u16)scratch->field_134.vx + var_a2, scratch->field_134.vy - var_a1);
    setXY2Fast(*poly, (u16)scratch->field_134.vx - var_a2, scratch->field_134.vy);
    setXY3Fast(*poly, (u16)scratch->field_134.vx + var_a2, scratch->field_134.vy);

    *(u32*)&(*poly)->u0 = ((sharedData_800DFB7C_0_s00[idx].field_C.s_1.field_2 << 13) + 0xE0000);
    *(u32*)&(*poly)->u1 = ((sharedData_800DFB7C_0_s00[idx].field_C.s_1.field_2 << 13) + 0x2D003F);
    *(u16*)&(*poly)->u2 = ((sharedData_800DFB7C_0_s00[idx].field_C.s_1.field_2 << 13) | 0x1F00);
    *(u16*)&(*poly)->u3 = ((sharedData_800DFB7C_0_s00[idx].field_C.s_1.field_2 << 13) | 0x1F3F);

    setSemiTrans(*poly, true);

    if (var_lo < 0x800)
    {
        rgbColor = Q12_MULT_PRECISE((var_lo * 2), 0x80);
    }
    else
    {
        rgbColor = 0x80 - Q12_MULT_PRECISE(((var_lo - 0x800) * 2), 0x80);
    }

    rgbColor = (rgbColor * func_80055D78(sharedData_800DFB7C_0_s00[idx].field_0.vx_0, sharedData_800DFB7C_0_s00[idx].vy_8, sharedData_800DFB7C_0_s00[idx].field_4.vz_4)) >> 7;

    setRGB0Fast(*poly, rgbColor, rgbColor, rgbColor);

    addPrim(&g_OrderingTable0[g_ActiveBufferIdx].org[(scratch->field_138 + 256) >> 3], *poly);

    *poly += 1;

    return true;
}

#include "../src/maps/unk_draw_m1s03.c"

#include "../src/maps/particle.c"

#include "../src/maps/characters/player.c"

#include "../src/maps/characters/stalker.c" // 0x800D2DC0
#include "../src/maps/characters/creeper.c" // 0x800D8010

// TODO: Move to separate split.
#include "../src/maps/characters/locker_dead_body.c" // 0x800DA284

#include "../src/maps/map_util.c" // 0x800DA424

#include "maps/shared/Map_RoomBgmInit_1_s02.h" // 0x800DA4D4

void GameBoot_LoadScreen_StageString(void) {}

#include "maps/shared/MapEvent_DoorJammed.h" // 0x800DA638

#include "maps/shared/MapEvent_DoorLocked.h" // 0x800DA6CC

#include "maps/shared/MapEvent_DoorUnlocked.h" // 0x800DA760

void MapEvent_LibraryReserveKeyUse(void) // 0x800DA7F4
{
    VECTOR3 sfxPos = {
        MAP_POINTS[g_MapEventData->mapPointIdx].positionX,
        Q12(-1.2f),
        MAP_POINTS[g_MapEventData->mapPointIdx].positionZ
    };

    Player_ItemRemove(InvItemId_LibraryReserveKey, 1);
    Event_DisplayMapMsgWithSfx(MAP_MSG(37), Sfx_UseKey, &sfxPos); // "Used the Library reverse key."
    Savegame_EventFlagSet(EventFlag_MapMark_AltSchool2F_LibraryReserveArrows);
}

void MapEvent_ClassroomKeyUse(void) // 0x800DA8B0
{
    VECTOR3 sfxPos = {
        MAP_POINTS[g_MapEventData->mapPointIdx].positionX,
        Q12(-1.2f),
        MAP_POINTS[g_MapEventData->mapPointIdx].positionZ
    };

    Player_ItemRemove(InvItemId_ClassroomKey, 1);
    Event_DisplayMapMsgWithSfx(MAP_MSG(38), Sfx_UseKey, &sfxPos); // "Used classroom key."
}

const char* MAP_MESSAGES[] = {
    #include "maps/shared/map_msg_common.h"
#if VERSION_REGION_IS(NTSC)
    /* 15 */ "\tWhat's_this? ~E ",
    /* 16 */ "\t[The_Monster_Lurks] ~N\n\tis_the_book's_title. ~E ",
    /* 17 */ "~C3\tChapter_3: ~N\n\t\t__[Manifestation_of_Delusions] ",
    /* 18 */ "~C3\t...Poltergeists_are_among_these. ~N\n\t\tNegative_emotions,_like_fear, ",
    /* 19 */ "~C3\tworry_or_stress_manifest ~N\n\t\tinto_external_energy_with ~N\n\t\tphysical_effects. ",
    /* 20 */ "~C3\tNightmares_have,_in_some_cases, ~N\n\t\tbeen_shown_to_trigger_them. ",
    /* 21 */ "~C3\tHowever,_such_phenomena_do_not ~N\n\t\tappear_to_happen_to_just_anyone. ",
    /* 22 */ "~C3\tAlthough_it's_not_clear_why, ~N\n\t\tadolescents,_especially_girls, ~N\n\t\tare_prone_to_such_occurrences. ~E ",
    /* 23 */ "~C3\tHearing_this,_the_hunter ~N\n\t\tarmed_with_bow_and_arrow_said, ~N\n\t\t[I_will_kill_the_lizard.] ",
    /* 24 */ "~C3\tBut_upon_meeting_his_opponent, ~N\n\t\the_held_back,_taunting, ~N\n\t\t[Who's_afraid_of_a_reptile?] ",
    /* 25 */ "~C3\tAt_this,_the_furious_lizard ~N\n\t\thissed,_[I'll_swallow_you_up ~N\n\t\tin_a_single_bite!] ",
    /* 26 */ "~C3\tThen_the_huge_creature ~N\n\t\tattacked,_jaws_opened_wide. ",
    /* 27 */ "~C3\tThis_was_what_the_man_wanted. ~N\n\t\tCalmly_drawing_his_bow,_he_shot ~N\n\t\tinto_the_lizard's_gaping_mouth. ",
    /* 28 */ "~C3\tEffortlessly,_the_arrow_flew, ~N\n\t\tpiercing_the_defenseless_maw, ",
    /* 29 */ "~C3\tand_the_lizard_fell_down_dead. ~E ",
    /* 30 */ "\tThis_is_from_an_old_fairy_tale. ~N\n\tI_remember_reading_it_as_a_kid. ~E ",
    /* 31 */ "\tThere's_a_drainage_valve. ~N\n\tNothing_unusual. ~E ",
    /* 32 */ "\tThere's_a_hanging_key. ~N\n\tIt's_just_out_of_reach. ~E ",
    /* 33 */ "\tThe_key_went_down_the_drain. ~E ",
    /* 34 */ "\tThe_ball_is_stuffed ~N\n\tinside_the_pipe. ~E ",
    /* 35 */ "\tNothing_special. ~E ",
    /* 36 */ "\tUsed_the_ ~C2 Rubber_ball ~C7 . ~E ",
    /* 37 */ "\tUsed_the_ ~C2 Library_reserve_key ~C7 . ~E ",
    /* 38 */ "\tUsed_the_ ~C2 Classroom_key ~C7 . ~E ",
    /* 39 */ "\tNo_useful_books. ~E ",
    /* 40 */ "\tThere_is_a_drainage_valve. ~N\n\tDo_you_want_to_open_it? ~S4 ",
    /* 41 */ "\tIt_drains_into_the_wrong_hole. ~N\n\tMaybe_I_could_plug ~N\n\tthat_up_somehow. ~E ",
    /* 42 */ "\tNothing_useful_in_the_locker. ~E ",
    /* 43 */ "\tNothing_inside. ~E ",
    /* 44 */ "\tThere_is_a_ ~C2 Library_reserve_key ~C7 . ~N\n\tTake_it? ~S4 ",
    /* 45 */ "\tA_body_is_slumped_over. ~E ",
    /* 46 */ "~J0(1.0)\tWhat? ~E ",
    /* 47 */ "~J0(2.5)\tThat_light!? ~E "
#elif VERSION_REGION_IS(NTSCJ)
    /* 15 */ "\tこれは……？ ~E ",
    /* 16 */ "\t“潜在する怪物”……本の題名だったのか ~E ",
    /* 17 */ "~C3\t◇第三章：妄想の顕在化 ",
    /* 18 */ "~C3\tまた、ポルターガイストも、 ~N\n\t\tその一種と考えられている。 ",
    /* 19 */ "~C3\t極度の不安、恐怖、ストレス等のネガティブ ~N\n\t\tな感情が、物理作用を持つ外部的なエネル ~N\n\t\tギーとして発露するのだ。 ",
    /* 20 */ "~C3\t睡眠中に悪夢が引き金となって起こった ~N\n\t\tと思われるケースも確認されている。 ",
    /* 21 */ "~C3\tしかし、これらの事象は誰にでも ~N\n\t\t起こり得るものではない。 ",
    /* 22 */ "~C3\t生来の素養が大きく関わっているが、 ~N\n\t\t思春期の少年少女、とりわけ少女の ~N\n\t\t身の回りで起こる場合が多い。 ~E ",
    /* 23 */ "~C3\tそれを聞いた弓矢をかついだ旅人は ~N\n\t\t「それなら私がトカゲをやっつけよう」 ~N\n\t\tと言いました。 ",
    /* 24 */ "~C3\t旅人はトカゲに会うといきなりやっつけよ ~N\n\t\tうとはしないで、悪口を言い出しました。 ",
    /* 25 */ "~C3\t「なんだい、こんなへなちょこトカゲ、 ~N\n\t\t　ちっとも怖くない」 ",
    /* 26 */ "~C3\tそれを聞いてトカゲは怒りました。 ~N\n\t\t「おまえも一口に呑み込んでやる！」 ",
    /* 27 */ "~C3\tトカゲは旅人めがけて大きな口を開けて ~N\n\t\tとびかかりました。 ",
    /* 28 */ "~C3\t実は旅人はこれを待っていたのです。 ~N\n\t\t旅人は落ち着いて弓に矢をつがえると、 ~N\n\t\tその口めがけてひゅうと射ました。 ",
    /* 29 */ "~C3\t見事、矢はトカゲの口の中へ、固いうろこ ~N\n\t\tのない口の中へと飛んでいき、お腹の中に ~N\n\t\tぶっすりとささりました。 ",
    /* 30 */ "~C3\tそれにやられてトカゲはどうと倒れると死 ~N\n\t\tんでしまいました。 ~E ",
    /* 31 */ "~L1\t外国のおとぎ話だな ~N\n\t子供の頃読んだことがある ~E ",
    /* 32 */ "\t排水バルブがある ~N\n\t特に変わったところはない ~E ",
    /* 33 */ "\t鍵が引っかかっている ~N\n\t指を伸ばしても届きそうにない ~E ",
    /* 34 */ "\t鍵は流れていったようだ ~E ",
    /* 35 */ "\t排水孔に ~C2 ゴムボール ~C7 が詰め込んである ~E ",
    /* 36 */ "\t普通の排水孔だ ~N\n\t特に変わった様子はない ~E ",
    /* 37 */ "\t~C2 ゴムボール ~C7 で排水孔をふさいだ ~E ",
    /* 38 */ "\t~C2 書庫の鍵 ~C7 を使った ~E ",
    /* 39 */ "\t~C2 教室の鍵 ~C7 を使った ~E ",
    /* 40 */ "\t役に立ちそうな本はない ~E ",
    /* 41 */ "\t排水バルブがある ~N\n\t開けますか？ ~S4 ",
    /* 42 */ "\t手前の穴に流れていってしまう ~N\n\t……何かでふさげないか？ ~E ",
    /* 43 */ "\tロッカーの中に役に ~N\n\t立ちそうな物は入ってない ~E ",
    /* 44 */ "\t\tロッカーの中には何もない ~E ",
    /* 45 */ "\t\t~C2 書庫の鍵 ~C7 がある ~N\n\t\t取りますか？ ~S4 ",
    /* 46 */ "\t死体が横たわっている ~E ",
    /* 47 */ "~J0(1.0)\t何だ？ ~E ",
    /* 48 */ "~J0(2.5)\tあの光は！？ ~E"
#endif
};

void MapEvent_CommonItemTake(void) // 0x800DA950
{
    u32 pickupType;
    s32 eventFlagIdx;

    pickupType   = CommonPickupItemId_FirstAidKit;
    eventFlagIdx = 0;

    switch (g_MapEventData->mapPointIdx)
    {
        case 84:
            pickupType   = CommonPickupItemId_ShotgunShells;
            eventFlagIdx = EventFlag_M1S03_ShotgunShells0;
            break;

        case 85:
            pickupType   = CommonPickupItemId_HandgunBullets;
            eventFlagIdx = EventFlag_M1S03_HandgunBullets0;
            break;

        case 86:
            pickupType   = CommonPickupItemId_HealthDrink;
            eventFlagIdx = EventFlag_M1S03_HealthDrink0;
            break;

        case 87:
            pickupType   = CommonPickupItemId_HandgunBullets;
            eventFlagIdx = EventFlag_M1S03_HandgunBullets1;
            break;

        case 88:
            pickupType   = CommonPickupItemId_HandgunBullets;
            eventFlagIdx = EventFlag_M1S03_HandgunBullets2;
            break;

        case 89:
            pickupType   = CommonPickupItemId_ShotgunShells;
            eventFlagIdx = EventFlag_M1S03_ShotgunShells1;
            break;

        case 90:
            pickupType   = CommonPickupItemId_HandgunBullets;
            eventFlagIdx = EventFlag_M1S03_HandgunBullets3;
            break;

        case 91:
            pickupType   = CommonPickupItemId_HealthDrink;
            eventFlagIdx = EventFlag_M1S03_HealthDrink1;
            break;

        case 92:
            pickupType   = CommonPickupItemId_HealthDrink;
            eventFlagIdx = EventFlag_M1S03_HealthDrink2;
            break;

        case 93:
            pickupType   = CommonPickupItemId_FirstAidKit;
            eventFlagIdx = EventFlag_M1S03_FirstAidKit;
            break;
    }

    Event_CommonItemTake(pickupType, eventFlagIdx);
}

void MapEvent_RoofDrainPuzzleInteract0(void) // 0x800DAA28
{
    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            Event_CharaAnimPlayToEnd(&g_SysWork.playerWork.player, 59);
            break;

        case 3:
            if (g_MapEventData->mapPointIdx == 19 ||
                g_MapEventData->mapPointIdx == 20)
            {
                if (g_MapEventData->activationType == EventActivationType_Item)
                {
                    Savegame_EventFlagSet(EventFlag_M1S02_UsedRubberBall);
                    Event_DisplayMapMsg(false, MAP_MSG(36), false, false, 0, false); // "Used rubber ball."
                }
                else
                {
                    Event_DisplayMapMsg(false, MAP_MSG(35), false, false, 0, false); // "Nothing special."
                }
            }
            else if (g_MapEventData->mapPointIdx == 17 ||
                     g_MapEventData->mapPointIdx == 18)
            {
                if (Savegame_EventFlagGet(EventFlag_M1S02_PickupClassroomKey))
                {
                    Event_DisplayMapMsg(false, MAP_MSG(35), false, false, 0, false); // "Nothing special."
                }
                else
                {

                    Event_DisplayMapMsg(false, MAP_MSG(33), false, false, 0, false); // "There's a hanging key."
                }
            }
            break;

        case 4:
            Event_CharaAnimPlayToEnd(&g_SysWork.playerWork.player, 60);
            break;

        default:
            // Restore player control.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;
    }
}

void MapEvent_RoofDrainPuzzleInteract1(void) // 0x800DABC4
{
    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();

            if (g_MapEventData->mapPointIdx == 17 ||
                g_MapEventData->mapPointIdx == 18)
            {
                if (Savegame_EventFlagGet(EventFlag_M1S03_KeyDownTheDrain))
                {
                    Event_BgTextureCmd(false, FILE_TIM_HOLEKEY1_TIM, false);
                }
                else
                {
                    Event_BgTextureCmd(false, FILE_TIM_HOLEKEY0_TIM, false);
                    Savegame_EventFlagSet(EventFlag_112);
                }
            }
            else if (g_MapEventData->mapPointIdx == 19 ||
                     g_MapEventData->mapPointIdx == 20)
            {
                if (g_MapEventData->activationType == EventActivationType_Item ||
                    Savegame_EventFlagGet(EventFlag_M1S02_UsedRubberBall))
                {
                    Event_BgTextureCmd(false, FILE_TIM_BALLHOLE_TIM, false);
                }
                else
                {
                    Event_BgTextureCmd(false, FILE_TIM_BALLHOL1_TIM, false);
                }
            }

            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            Event_CharaAnimPlayToEnd(&g_SysWork.playerWork.player, 59);
            break;

        case 3:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 0, 0, false);
            SysWork_StateStepIncrement(0);

        case 4:
            Event_BgTextureCmd(true, 0, false);
            break;

        case 5:
            Event_ScreenFadeCmd(ScreenFadeCmd_Wait, true, 0, 0, false);
            break;

        case 6:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, 0, false);
            break;

        case 7:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);

            // Rubber ball.
            if (g_MapEventData->mapPointIdx == 19 ||
                g_MapEventData->mapPointIdx == 20)
            {
                if (g_MapEventData->activationType == EventActivationType_Item)
                {
                    Savegame_EventFlagSet(EventFlag_M1S02_UsedRubberBall);
                    Event_DisplayMapMsg(false, MAP_MSG(36), false, false, 0, false);
                }
                else
                {
                    if (Savegame_EventFlagGet(EventFlag_M1S02_UsedRubberBall))
                    {
                        Event_DisplayMapMsg(false, MAP_MSG(34), false, false, 0, false); // "The ball is stuffed inside the pipe."
                    }
                    else
                    {
                        Event_DisplayMapMsg(false, MAP_MSG(35), false, false, 0, false); // "Nothing special."
                    }
                }
            }
            // Key in drain.
            else
            {
                if (Savegame_EventFlagGet(EventFlag_M1S02_PickupClassroomKey))
                {
                    Event_DisplayMapMsg(false, MAP_MSG(35), false, false, 0, false); // "Nothing special."
                }
                else
                {
                    if (!Savegame_EventFlagGet(EventFlag_M1S03_KeyDownTheDrain))
                    {
                        Event_DisplayMapMsg(false, MAP_MSG(32), false, false, 0, false); // "There's a hanging key."
                    }
                    else
                    {
                        Event_DisplayMapMsg(false, MAP_MSG(33), false, false, 0, false); // "The key went down the drain."
                    }
                }
            }
            break;

        case 8:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, 0, false);
            break;

        case 9:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, 0, false);
            break;

        case 10:
            Event_CharaAnimPlayToEnd(&g_SysWork.playerWork.player, 60);
            break;

        default:
            // Restore player control.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;
    }
}

const VECTOR3 D_800CBAA4 = { /** Q19.12 */
    Q12(-10.5f),
    Q12(-0.5f),
    Q12(-12.9f)
};

const VECTOR3 D_800CBAB0 = { /** Q19.12 */
    Q12(-10.2f),
    Q12(0.0f),
    Q12(-13.35f)
};

void MapEvent_DrainageValveInteract(void) // 0x800DAF18
{
    VECTOR3 lightPos;  // Q19.12
    VECTOR3 lookAtPos; // Q19.12
    VECTOR3 sp48;
    MATRIX  sp58;
    VECTOR  sp78;
    s32     balance;
    s32     temp_s2;
    s32     temp_v1_9;
    s32     i;
    s32     var_v0;
    s32     vol;
    s32     lightPosIdx;

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();

            D_800E20FC = Q12_ANGLE(0.0f);
            D_800E2100 = 0;

            Savegame_EventFlagSet(EventFlag_MapMark_AltSchoolRF_ValveSign);

            D_800E2101 = 0;
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            Event_DisplayMapMsg(true, MAP_MSG(40), 4, 3, 0, false); // "There is a drainage valve."
            break;

        case 3:
            // Restore player control.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;

        case 4:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 2, 0, false);

            sharedData_800E30C8_1_s02.field_0[0]  = Q12(-10.2);
            sharedData_800E30C8_1_s02.field_10[0] = Q12(-0.35f);
            sharedData_800E30C8_1_s02.field_18[0] = Q12(-13.35f);
            sharedData_800E30C8_1_s02.field_28[0] = Q12(0.45f);
            sharedData_800E30C8_1_s02.field_30[0] = 1;
            sharedData_800E30C8_1_s02.field_3C[0] = Q12(0.18f);
            sharedData_800E30C8_1_s02.field_44[0] = Q12(0.25f);
            sharedData_800E30C8_1_s02.field_4C[0] = Q12(1.5f);
            sharedData_800E30C8_1_s02.field_5C[0] = Q12(0.18f);
            sharedData_800E30C8_1_s02.field_64[0] = Q12(0.25f);
            sharedData_800E30C8_1_s02.field_6c[0] = Q12(0.0f);

            sharedData_800E30C8_1_s02.field_0[1]  = Q12(-10.25f);
            sharedData_800E30C8_1_s02.field_10[1] = Q12(0.1f);
            sharedData_800E30C8_1_s02.field_18[1] = Q12(-12.98f);
            sharedData_800E30C8_1_s02.field_28[1] = Q12(4.92f);
            sharedData_800E30C8_1_s02.field_30[1] = 0;
            sharedData_800E30C8_1_s02.field_34[1] = Q12_ANGLE(180.0f);
            sharedData_800E30C8_1_s02.field_3C[1] = Q12(0.3f);
            sharedData_800E30C8_1_s02.field_44[1] = Q12(0.5f);
            sharedData_800E30C8_1_s02.field_4C[1] = Q12(0.6f);
            sharedData_800E30C8_1_s02.field_5C[1] = Q12(0.3f);
            sharedData_800E30C8_1_s02.field_64[1] = Q12(0.5f);
            sharedData_800E30C8_1_s02.field_6c[1] = Q12(0.8f);

            D_800E3A40[0].vx_0     = Q12(-10.2f);
            D_800E3A40[0].field_8  = Q12(0.12f);
            D_800E3A40[0].field_C  = Q12(0.08f);
            D_800E3A40[0].vz_4     = Q12(-13.35f);
            D_800E3A40[0].idx_14   = 2;
            D_800E3A40[0].field_12 = 40;
            D_800E3A40[0].field_E  = Q12(0.25f);
            D_800E3A40[0].field_10 = Q12(0.3f);

            D_800E3A40[1].vx_0     = Q12(-10.25f);
            D_800E3A40[1].vz_4     = Q12(-17.72f);
            D_800E3A40[1].field_C  = Q12(0.05f);
            D_800E3A40[1].field_12 = 40;

            if (Savegame_EventFlagGet(EventFlag_M1S02_UsedRubberBall))
            {
                sharedData_800E30C8_1_s02.field_0[2]  = Q12(-10.1f);
                sharedData_800E30C8_1_s02.field_10[2] = Q12(0.13f);
                sharedData_800E30C8_1_s02.field_28[2] = Q12(6.82f);
                sharedData_800E30C8_1_s02.field_34[2] = Q12_ANGLE(270.0f);
                sharedData_800E30C8_1_s02.field_74    = 150;
                sharedData_800E30C8_1_s02.field_18[2] = Q12(-17.75f);
                sharedData_800E30C8_1_s02.field_30[2] = 0;
                sharedData_800E30C8_1_s02.field_3C[2] = Q12(0.3f);
                sharedData_800E30C8_1_s02.field_44[2] = Q12(0.5f);
                sharedData_800E30C8_1_s02.field_4C[2] = Q12(0.6f);
                sharedData_800E30C8_1_s02.field_5C[2] = Q12(0.3f);
                sharedData_800E30C8_1_s02.field_64[2] = Q12(0.5f);
                sharedData_800E30C8_1_s02.field_6c[2] = Q12(0.8f);
                sharedData_800E30C8_1_s02.field_78    = 3;

                D_800E3A40[1].field_8  = Q12(0.22f);
                D_800E3A40[1].idx_14   = 4;
                D_800E3A40[1].field_A  = 0;
                D_800E3A40[1].field_E  = Q12(0.2f);
                D_800E3A40[1].field_10 = Q12(0.35f);
                D_800E3A40[2].field_C  = Q12(0.06f);
                D_800E3A40[2].vx_0     = Q12(-16.86f);
                D_800E3A40[2].vz_4     = Q12(-17.75f);
                D_800E3A40[2].field_8  = Q12(0.2f);
                D_800E3A40[2].idx_14   = 3;
                D_800E3A40[2].field_12 = 60;
                D_800E3A40[2].field_E  = Q12(0.2f);
                D_800E3A40[2].field_10 = Q12(0.2f);
            }
            else
            {
                sharedData_800E30C8_1_s02.field_74 = 80;
                sharedData_800E30C8_1_s02.field_78 = 2;
                D_800E3A40[1].field_8              = Q12(0.2f);
                D_800E3A40[1].idx_14               = 3;
                D_800E3A40[1].field_E              = Q12(0.15f);
                D_800E3A40[1].field_10             = Q12(0.2f);
            }

            sharedData_800E30C8_1_s02.field_28[sharedData_800E30C8_1_s02.field_78] = Q12(0.5f);
            sharedData_800E30C8_1_s02.field_10[sharedData_800E30C8_1_s02.field_78] = Q12(0.16f);
            sharedData_800E30C8_1_s02.field_3C[sharedData_800E30C8_1_s02.field_78] = Q12(0.1f);
            sharedData_800E30C8_1_s02.field_5C[sharedData_800E30C8_1_s02.field_78] = Q12(0.12f);
            sharedData_800E30C8_1_s02.field_64[sharedData_800E30C8_1_s02.field_78] = Q12(0.3f);

            // Warp player.
            g_SysWork.playerWork.player.position.vx = Q12(-11.45f);
            g_SysWork.playerWork.player.position.vz = Q12(-12.95f);
            g_SysWork.playerWork.player.rotation.vy = Q12(0.25f);

            func_8003D03C();
            Player_EmptyWeaponHandSet();

            // Warp camera.
            Event_CameraPositionSet(NULL,
                                    Q12(-12.38f), Q12(-1.2f), Q12(-14.07f),
                                    Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                    true);
            Event_CameraLookAtSet(NULL,
                                  Q12(-9.24f), Q12(-0.76f), Q12(-11.63f),
                                  Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                  true);

            SysWork_StateStepIncrement(0);

        case 5:
            Event_WaitTimer(Q12(1.5f), false);
            break;

        case 6:
            D_800E20FE = 0;
            SysWork_StateStepIncrement(0);

        case 7:
            D_800E20FE++;
            Event_CharaAnimPlayToEnd(&g_SysWork.playerWork.player, 107);
            break;

        case 8:
            sharedFunc_800CBC98_1_s02();
            func_800CE0CC(0);
            func_800CE0CC(1);

            if (Savegame_EventFlagGet(EventFlag_M1S02_UsedRubberBall))
            {
                func_800CE0CC(2);
            }

            Model_AnimFlagsClear(&g_SysWork.playerWork.player.model, AnimFlag_Visible);

            func_8008D438();
            Event_CameraPositionSet(NULL,
                                    D_800E1F88[0].vx, D_800E1F88[0].vy, D_800E1F88[0].vz,
                                    Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                    true);
            Event_CameraLookAtSet(NULL,
                                  D_800E1FC4[0].vx, D_800E1FC4[0].vy, D_800E1FC4[0].vz,
                                  Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                  true);

            D_800E20FA = 0;
            SysWork_StateStepIncrement(0);

        case 9:
            D_800E20FA += g_DeltaTime;

            for (i = 1; i < 6; i++)
            {
                if (D_800E20FA < D_800E1F7C[i])
                {
                    break;
                }
            }

            if (i == 6 || (!Savegame_EventFlagGet(EventFlag_M1S02_UsedRubberBall) && i > 3))
            {
                SysWork_StateStepIncrement(0);
            }
            else
            {
                lightPosIdx = i >> 1;

                if (Savegame_EventFlagGet(EventFlag_M1S02_UsedRubberBall))
                {
                    lightPosIdx += 2;
                }

                if (i & 0x1)
                {
                    lightPos.vx = D_800E1F88[lightPosIdx].vx;
                    lightPos.vy = D_800E1F88[lightPosIdx].vy;
                    lightPos.vz = D_800E1F88[lightPosIdx].vz;

                    lookAtPos.vx = D_800E1FC4[lightPosIdx].vx;
                    lookAtPos.vy = D_800E1FC4[lightPosIdx].vy;
                    lookAtPos.vz = D_800E1FC4[lightPosIdx].vz;
                }
                else
                {
                    lightPos.vx = D_800E1F88[lightPosIdx - 1].vx + (((D_800E1F88[lightPosIdx].vx - D_800E1F88[lightPosIdx - 1].vx) * (D_800E20FA - D_800E1F7C[i - 1])) / (D_800E1F7C[i] - D_800E1F7C[i - 1]));
                    lightPos.vy = D_800E1F88[lightPosIdx - 1].vy + (((D_800E1F88[lightPosIdx].vy - D_800E1F88[lightPosIdx - 1].vy) * (D_800E20FA - D_800E1F7C[i - 1])) / (D_800E1F7C[i] - D_800E1F7C[i - 1]));
                    lightPos.vz = D_800E1F88[lightPosIdx - 1].vz + (((D_800E1F88[lightPosIdx].vz - D_800E1F88[lightPosIdx - 1].vz) * (D_800E20FA - D_800E1F7C[i - 1])) / (D_800E1F7C[i] - D_800E1F7C[i - 1]));

                    lookAtPos.vx = D_800E1FC4[lightPosIdx - 1].vx + (((D_800E1FC4[lightPosIdx].vx - D_800E1FC4[lightPosIdx - 1].vx) * (D_800E20FA - D_800E1F7C[i - 1])) / (D_800E1F7C[i] - D_800E1F7C[i - 1]));
                    lookAtPos.vy = D_800E1FC4[lightPosIdx - 1].vy + (((D_800E1FC4[lightPosIdx].vy - D_800E1FC4[lightPosIdx - 1].vy) * (D_800E20FA - D_800E1F7C[i - 1])) / (D_800E1F7C[i] - D_800E1F7C[i - 1]));
                    lookAtPos.vz = D_800E1FC4[lightPosIdx - 1].vz + (((D_800E1FC4[lightPosIdx].vz - D_800E1FC4[lightPosIdx - 1].vz) * (D_800E20FA - D_800E1F7C[i - 1])) / (D_800E1F7C[i] - D_800E1F7C[i - 1]));
                }

                // Warp camera.
                Event_CameraPositionSet(&lightPos,
                                        Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                        Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                        true);
                Event_CameraLookAtSet(&lookAtPos,
                                      Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                      Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                      true);

                g_SysWork.lensFlareBoneCoord = NULL;
                g_SysWork.lightRotation.vx   = -ratan2(lookAtPos.vy - lightPos.vy,
                                                       Math_Vector2MagCalcSafeQ6(lookAtPos.vx - lightPos.vx,
                                                                                 lookAtPos.vz - lightPos.vz));

                g_SysWork.lightRotation.vy = ratan2(lookAtPos.vx - lightPos.vx, lookAtPos.vz - lightPos.vz);
                g_SysWork.lightRotation.vz = Q12_ANGLE(0.0f);
                g_SysWork.lightBoneCoord   = NULL;
                g_SysWork.lightPosition.vx = lightPos.vx;
                g_SysWork.lightPosition.vy = MAX(Q12(-1.5f), lightPos.vy);
                g_SysWork.lightPosition.vz = lightPos.vz;
            }
            break;

        case 10:
            if (Savegame_EventFlagGet(EventFlag_M1S02_UsedRubberBall))
            {
                SysWork_StateStepIncrement(0);
            }
            else
            {
                Event_DisplayMapMsg(false, MAP_MSG(41), false, false, 0, false); // "It draains into the wrong hole."
            }
            break;

        case 11:
            Model_AnimFlagsSet(&g_SysWork.playerWork.player.model, AnimFlag_Visible);

            func_8008D448();
            Game_FlashlightAttributesFix();

            // Warp camera.
            Event_CameraPositionSet(NULL,
                                    Q12(-12.38f), Q12(-1.2f), Q12(-14.07f),
                                    Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                    true);
            Event_CameraLookAtSet(NULL,
                                  Q12(-9.24f), Q12(-0.76f), Q12(-11.63f),
                                  Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                  true);

            sharedFunc_800CBD58_1_s02();

            if (Savegame_EventFlagGet(EventFlag_M1S02_UsedRubberBall))
            {
                Savegame_EventFlagSet(EventFlag_M1S03_KeyDownTheDrain);
            }

            D_800E2101 = 0;
            D_800E20FE = 0;
            SysWork_StateStepIncrement(0);

        case 12:
            D_800E20FE++;
            Event_CharaAnimPlayToEnd(&g_SysWork.playerWork.player, 108);
            break;

        default:
            // Restore player control.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, 0, false);
            vcReturnPreAutoCamWork(true);
            Player_WeaponAttackRestore();
            break;
    }

    if (g_SysWork.sysStateSteps[0] == 7 || g_SysWork.sysStateSteps[0] == 12)
    {
        Vw_CoordHierarchyMatrixCompute(&g_SysWork.playerBoneCoords[HarryBone_RightHand], &sp58);

        gte_SetRotMatrix(&sp58);
        gte_SetTransMatrix(&sp58);
        gte_ldv0(&D_800E1F74);
        gte_rt();
        gte_stlvnl(&sp78);

        temp_s2 = ratan2((sp78.vy * 16) + Q12(0.7f), (sp78.vz * 16) + Q12(12.92f));
        if (D_800E20FE > 2)
        {
            var_v0 = FP_FROM(g_SysWork.playerWork.player.model.anim.time, Q12_SHIFT) - Player_AnimGetSomething();
        }
        else
        {
            var_v0 = 0;
        }

        if (var_v0 >= 6 && var_v0 < 37)
        {
            if ((g_SysWork.sysStateSteps[0] == 7  && Math_AngleNormalizeSigned(D_800E20F8 - temp_s2) < Q12_ANGLE(0.0f)) ||
                (g_SysWork.sysStateSteps[0] == 12 && Math_AngleNormalizeSigned(D_800E20F8 - temp_s2) > Q12_ANGLE(0.0f)))
            {
                if (D_800E2101 == 0)
                {
                    Sfx_WithFlagsPlay(Sfx_Unk1463, &D_800CBAA4, Q8(0.5f), SfxFlag_None);
                    D_800E2101++;
                }
                else if (!(D_800E2101 & 0x1) && D_800E2101 < 8)
                {
                    Sfx_WithFlagsPlay(Sfx_Unk1462, &D_800CBAA4, Q8(0.5f), SfxFlag_None);

                    if (D_800E2101 == 2)
                    {
                        if (g_SysWork.sysStateSteps[0] == 12)
                        {
                            Sd_SfxStop(Sfx_Unk1465);
                            D_800E2100 = 0;
                        }
                        else if (g_SysWork.sysStateSteps[0] == 7)
                        {
                            SD_Call(Sfx_Unk1465);
                            D_800E2100 = 1;
                        }
                    }

                    D_800E2101++;
                }

                g_WorldObject5.rotation.vx += Math_AngleNormalizeSigned(D_800E20F8 - temp_s2);

                if (D_800E2100 != 0)
                {
                    D_800E20FC -= Math_AngleNormalizeSigned(D_800E20F8 - temp_s2);
                    if (D_800E20FC < Q12_ANGLE(0.0f))
                    {
                        D_800E20FC = Q12_ANGLE(0.0f);
                    }
                    if (D_800E20FC > Q12_ANGLE(358.6f))
                    {
                        D_800E20FC = Q12_ANGLE(358.6f);
                    }
                }
            }
            else if (D_800E2101 & 1)
            {
                D_800E2101++;
            }
        }

        D_800E20F8 = temp_s2;
    }

    if (D_800E2100 != 0)
    {
        balance = Vc_StereoBalanceGet(&D_800CBAB0) >> 1;
        vwGetViewPosition(&sp48);

        temp_v1_9 = Math_Vector3MagCalcSafe((((g_SysWork.playerWork.player.position.vx + sp48.vx) >> 1) + Q12(10.2f)),
                                            ((g_SysWork.playerWork.player.position.vy + sp48.vy) >> 1),
                                            (((g_SysWork.playerWork.player.position.vz + sp48.vz) >> 1) + Q12(13.35f)));

        if (temp_v1_9 < Q12(1.0f))
        {
            vol = D_800E20FC;
        }
        else
        {
            vol = temp_v1_9;
            if (vol > Q12(17.0f))
            {
                vol = 0;
            }
            else
            {
                vol = Q12_MULT_PRECISE(D_800E20FC, Q12(1.0f) - Q12_DIV(vol - Q12(1.0f),  Q12(16.0f)));
            }
        }

        vol = ~(vol >> 4);
        Sd_SfxAttributesUpdate(Sfx_Unk1465, balance, vol, 0);
    }
}

void MapEvent_TheMonsterLurksBookRead(void) // 0x800DBEC8
{
    typedef enum _EventState
    {
        EventState_BookFirstTime = 3,
        EventState_BookAgain     = 6
    } e_EventState;

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            Event_SysStateBranchOnFlag(EventFlag_M1S03_MonsterLurksBookRead, EventState_BookAgain, EventState_BookFirstTime, false);
            break;

        case 3:
            Event_DisplayMapMsg(false, 15, false, false, 0, false); // "What's this?"
            break;

        case 4:
            Event_WaitTimer(Q12(0.6f), false);
            break;

        case 5:
            Event_DisplayMapMsg(false, 16, false, false, 0, false); // "The Monster Lurks is the book's title."
            break;

        case EventState_BookAgain:
            Event_BgTextureCmd(BgTextureCmd_QueueRead, FILE_TIM_LMONSTER_TIM, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, false, false, false);
            SysWork_StateStepIncrement(0);

        case 7:
            Event_BgTextureCmd(BgTextureCmd_AwaitLoad, 0, false);
            break;

        case 8:
            Event_ScreenFadeCmd(ScreenFadeCmd_Wait, true, false, false, false);
            break;

        case 9:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, false, false, false);
            break;

        case 10:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_DisplayMapMsg(false, 17, false, false, 0, false); // "Chapter3: ..."
            break;

        case 11:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, false, false, false);
            break;

        default:
            // Restore player control.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);

            Savegame_EventFlagSet(EventFlag_M1S03_MonsterLurksBookRead);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, false, false, false);
            break;
    }
}

void MapEvent_SplitHeadFairyTaleBookRead(void) // 0x800DC0F8
{
    typedef enum _EventState
    {
        EventState_Flag117True  = NO_VALUE,
        EventState_Flag117False = 8,
    } e_EventState;

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_BgTextureCmd(BgTextureCmd_QueueRead, FILE_TIM_LIBRBOOK_TIM, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 0, 0, false);
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            Event_ScreenFadeCmd(ScreenFadeCmd_Wait, true, 0, 0, false);
            break;

        case 3:
            Event_BgTextureCmd(BgTextureCmd_AwaitLoad, 0, false);
            break;

        case 4:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, 0, false);
            break;

        case 5:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_DisplayMapMsg(false, 23, false, false, 0, false); // "Hearing this, the hunter armed with bow..."
            break;

        case 6:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, 0, false);
            break;

        case 7:
            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, 0, false);
            Event_SysStateBranchOnFlag(EventFlag_117, EventState_Flag117True, EventState_Flag117False, false);
            break;

        case EventState_Flag117False:
            Event_ScreenFadeCmd(ScreenFadeCmd_Wait, false, 0, 0, false);
            break;

        case 9:
            Event_DisplayMapMsg(false, MAP_MSG(30), false, false, 0, false); // "This is from an old fairy tale."
            break;

        default: // `EventState_Flag117True`
            // Restore player control.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);

            Savegame_EventFlagSet(EventFlag_117);
            break;
    }
}

void MapEvent_OpenedCatLockerInspect(void) // 0x800DC2E4
{
    Event_DisplayMapMsgWithBg(FILE_TIM_LOCKER1_TIM, Q12(2.5f), Q12(2.5f), MAP_MSG(43)); // "Nothing inside."
}

void MapEvent_UnopenedCatLockerInspect(void) // 0x800DC310
{
    typedef enum _EventState
    {
        EventState_LockerOpenWait = 9,
        EventState_LockerOpen     = 10,
        EventState_Skip           = 13
    } e_EventState;

    if ((g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.skip) &&
        g_SysWork.sysStateSteps[0] >= 3 && g_SysWork.sysStateSteps[0] < 11)
    {
        ScreenFade_ResetTimestep();
        Savegame_EventFlagSet(EventFlag_101);
        Savegame_EventFlagSet(EventFlag_102);
        Savegame_EventFlagSet(EventFlag_M1S03_LockerOpened);
        SysWork_StateStepSet(0, EventState_Skip);
    }

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();

            Savegame_EventFlagSet(EventFlag_100);
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            // Warp player.
            g_SysWork.playerWork.player.position.vx = Q12(-59.36f);
            g_SysWork.playerWork.player.position.vz = Q12(18.37f);
            g_SysWork.playerWork.player.moveSpeed   = Q12(0.0f);
            g_SysWork.playerWork.player.rotation.vy = Q12_ANGLE(90.0f);

            // Warp camera.
            Event_CameraPositionSet(NULL,
                                    Q12(-60.16f), Q12(-2.7f), Q12(19.76f),
                                    Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                    true);
            Event_CameraLookAtSet(NULL,
                                  Q12(-58.13f), Q12(-0.04f), Q12(17.57f),
                                  Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                  true);

            func_8003D03C();
            Player_EmptyWeaponHandSet();
            SysWork_StateStepIncrement(0);

        case 3:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 2, 0, false);
            break;

        case 4:
            Event_WaitTimer(Q12(1.0f), false);
            break;

        case 5:
            Savegame_EventFlagSet(EventFlag_101);
            SysWork_StateStepIncrement(0);

        case 6:
            Event_CharaAnimPlayToEnd(&g_SysWork.playerWork.player, 85);
            break;

        case 7:
            Savegame_EventFlagSet(EventFlag_102);
            Event_CharaAnimCmdExecute(CharaAnimCmd_SetState, &g_SysWork.playerWork.player, 86, false);
            SysWork_StateStepIncrement(0);

        case 8:
            Event_WaitTimer(Q12(2.0f), false);
            break;

        case EventState_LockerOpenWait:
            Event_SysStateBranchOnFlag(EventFlag_M1S03_LockerOpened, EventState_LockerOpen, EventState_LockerOpenWait, false);
            break;

        case EventState_LockerOpen:
            Event_BgTextureFadeIn(FILE_TIM_LOCKER1_TIM, Q12(3.0f), Q12(1.5f));
            break;

        case 11:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_DisplayMapMsg(false, MAP_MSG(43), 0, 0, 0, false); // "Nothing inside."
            break;

        case 12:
            Event_BgTextureFadeOut(Q12(1.5f), Q12(3.0f));
            break;

        case EventState_Skip:
            Event_CharaAnimCmdExecute(CharaAnimCmd_AnimReset, &g_SysWork.playerWork.player, 0, false);
            Savegame_EventFlagSet(EventFlag_104);

            // Warp camera.
            Event_CameraPositionSet(NULL,
                                    Q12(-60.63f), Q12(-4.14f), Q12(17.42f),
                                    Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                    true);
            Event_CameraLookAtSet(NULL,
                                  Q12(-60.0f), Q12(0.0f), Q12(18.5f),
                                  Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                  true);

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, 0, false);
            SysWork_StateStepIncrement(0);
            func_8003D01C();
            Player_WeaponAttackRestore();
            break;

        default:
            // Restore player control.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);

            if (ScreenFade_IsFinished())
            {
                Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 0, 0, false);
            }
            break;
    }
}

void MapEvent_LockerDeadBodyCutscene(void) // 0x800DC780
{
    typedef enum _EventState
    {
        EventState_Flag106False = 4,
        EventState_Flag106True  = 5
    } e_EventState;

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            SysWork_StateStepIncrement(0);

        case 1:
            Savegame_EventFlagSet(EventFlag_105);

            // Warp camera.
            Event_CameraPositionSet(NULL,
                                    Q12(-59.93f), Q12(0.0f), Q12(20.18f),
                                    Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                    true);
            Event_CameraLookAtSet(NULL,
                                  Q12(-62.99f), Q12(-2.29f), Q12(21.37f),
                                  Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                  true);

            // Warp player.
            g_SysWork.playerWork.player.rotation.vy = ratan2(Q12(-61.4f) - g_SysWork.playerWork.player.position.vx,
                                                             Q12(20.8f)  - g_SysWork.playerWork.player.position.vz);

            func_8008D438();
            Model_AnimFlagsClear(&g_SysWork.playerWork.player.model, AnimFlag_Visible);
            SysWork_StateStepIncrement(0);

        case 2:
            Event_WaitTimer(Q12(0.7f), false);
            break;

        case 3:
            Sfx_WithFlagsPlay(Sfx_Unk1466, &Q12_VECTOR3(-60.3f, 0.0f, 20.2f), Q8(0.5f), SfxFlag_None);
            SysWork_StateStepIncrement(0);

        case EventState_Flag106False:
            Event_SysStateBranchOnFlag(EventFlag_106, EventState_Flag106True, EventState_Flag106False, false);
            break;

        case EventState_Flag106True:
            Event_WaitTimer(Q12(0.8f), false);
            break;

        default:
            // Restore player control.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);

            Savegame_EventFlagSet(EventFlag_107);
            Model_AnimFlagsSet(&g_SysWork.playerWork.player.model, AnimFlag_Visible);
            func_8008D448();
            break;
    }
}

void MapEvent_LibraryReserveKeyItemTake(void) // 0x800DC9A0
{
    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_InvItemCmd(InvItemCmd_QueueLoad, InvItemId_LibraryReserveKey, 0, false);
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            Event_CharaAnimPlayToEnd(&g_SysWork.playerWork.player, 59);
            break;

        case 3:
            Event_InvItemCmd(InvItemCmd_AwaitLoad, InvItemId_LibraryReserveKey, 0, false);
            break;

        case 4:
            Savegame_EventFlagSet(EventFlag_M1S03_PickupLibraryReserveKey);
            if (Gfx_PickupItemAnimate(InvItemId_LibraryReserveKey))
            {
                Event_DisplayMapMsg(true, MAP_MSG(44), 5, 6, 0, false); // "There is a Library Reserve Key. Take it?"
            }
            if (g_SysWork.sysStateSteps[0] == 6)
            {
                Savegame_EventFlagClear(EventFlag_M1S03_PickupLibraryReserveKey);
            }
            break;

        case 5:
            Event_InvItemCmd(InvItemCmd_AddItem, InvItemId_LibraryReserveKey, 1, false);
            SysWork_StateStepIncrement(0);

        case 6:
            Event_CharaAnimPlayToEnd(&g_SysWork.playerWork.player, 60);
            break;

        default:
            // Restore player control.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;
    }
}

void MapEvent_RubberBallUse(void) // 0x800DCB54
{
    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            Event_BgTextureCmd(BgTextureCmd_QueueRead, FILE_TIM_BALLHOLE_TIM, false);
            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            Event_CharaAnimPlayToEnd(&g_SysWork.playerWork.player, 59);
            break;

        case 3:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, 0, false);
            break;

        case 4:
            Sfx_WithFlagsPlay(Sfx_Unk1461, &Q12_VECTOR3(-10.5f, 0.0f, -17.5f), Q8(0.5f), SfxFlag_None);
            Savegame_EventFlagSet(EventFlag_M1S02_UsedRubberBall);
            SysWork_StateStepIncrement(0);

        case 5:
            Event_BgTextureCmd(BgTextureCmd_AwaitLoad, 0, false);
            break;

        case 6:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, 0, false);
            break;

        case 7:
            Event_DisplayMapMsg(false, MAP_MSG(36), 0, 0, 0, false); // "Used the rubber ball."
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            break;

        case 8:
            Event_BgTextureCmd(BgTextureCmd_Draw, 0, false);
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, 0, 0, false);
            break;

        case 9:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, 0, false);
            break;

        case 10:
            Event_CharaAnimPlayToEnd(&g_SysWork.playerWork.player, 60);
            break;

        default:
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);
            break;
    }
}

#include "maps/shared/Event_CutsceneTimerAdvance.h" // 0x800DCD44

void func_800DCDDC(void) // 0x800DCDDC
{
    s32 pitch;
    s32 tmp0;
    s32 tmp1;
    s32 cos0;
    s32 sin0;

    tmp0 = D_800E201C;
    tmp1 = tmp0 >> 2;
    sin0 = Math_Sin(tmp1);
    cos0 = Math_Cos(tmp0 >> 4);
    pitch = ((sin0 * 2) + cos0 + Math_Cos(Math_Sin(tmp1 * 2))) >> 9;

    if (g_SysWork.sysStateSteps[0] >= 10)
    {
        D_800E2018 -= Q12_MULT_PRECISE(g_DeltaTime, Q12(0.5f));
        if (D_800E2018 < 0)
        {
            D_800E2018 = 0;
        }

        Sd_SfxAttributesUpdate(Sfx_Unk1467, 0, ~(D_800E2018 >> 4), pitch);
    }
    else if (g_SysWork.sysStateSteps[0] > 0)
    {
        D_800E2018 += Q12_MULT_PRECISE(g_DeltaTime, Q12(0.25f));
        if (D_800E2018 > Q12(0.9961f))
        {
            D_800E2018 = Q12(0.9961f);
        }

        Sd_SfxAttributesUpdate(Sfx_Unk1467, 0, ~(D_800E2018 >> 4), pitch);
    }
    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();

            D_800E2102 = 0;

            Fs_QueueStartReadTim(FILE_TIM_ENBAN_TIM, FS_BUFFER_1, &D_800E2004);
            Fs_QueueStartRead(FILE_ANIM_UFO1_DMS, FS_BUFFER_13);

            g_Cutscene_Timer = NO_VALUE;
            ScreenFade_ResetTimestep();

            SD_Call(Sfx_Unk1467);
            Sd_SfxAttributesUpdate(Sfx_Unk1467, 0, Q8_CLAMPED(1.0f), 0);

            D_800E2018 = 0;
            D_800E201C = 0;

            SysWork_StateStepIncrement(0);

        case 1:
            Event_WaitPlayerStop();
            break;

        case 2:
            if (Fs_QueueChunksLoad())
            {
                SysWork_StateStepIncrement(0);
            }
            break;

        case 3:
            g_Cutscene_Timer = Q12(0.0f);

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, true, 3, 0, false);
            Dms_HeaderFixOffsets((s_DmsHeader*)FS_BUFFER_13);
            func_8003D03C();
            Player_EmptyWeaponHandSet();
            Game_TurnFlashlightOn();
            SysWork_StateStepIncrement(0);

        case 4:
            Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, 0, 0, false);

        case 5:
            Event_DisplayMapMsgWithAudio(MAP_MSG(46), &D_800E2102, D_800E2000); // "What?"
            break;

        case 6:
            Event_CutsceneTimerAdvance(&g_Cutscene_Timer, Q12(40.0f), Q12(0.0f), Q12(30.0f), true, true);
            sharedFunc_800CE5D4_1_s03(&D_800E200C, Q12(0.5f), Q12(0.05f), 0);
            break;
            
        case 7:
            Event_DisplayMapMsg(false, MAP_MSG(47), 0, 0, 0, false); // "That light!?"
            sharedFunc_800CE5D4_1_s03(&D_800E200C, Q12(0.5f), Q12(0.05f), 0);
            break;

        case 8:
            Vc_SetLookAtMatFromBoneCoord(&g_SysWork.playerBoneCoords[HarryBone_Head],
                                         &SVECTOR3_Zero, &Q12_VECTOR3(-13.91f, -11.5f, 22.11f));
            sharedFunc_800CE5D4_1_s03(&D_800E200C, Q12(0.5f), Q12(0.05f), 0);
            Event_WaitTimer(Q12(2.5f), false);
            break;

        case 9:
            sharedFunc_800CE5D4_1_s03(&D_800E200C, Q12(0.5f), Q12(0.05f), 0);

            g_SysWork.sysStateStepData[0] += g_DeltaTime;
            if (g_SysWork.sysStateStepData[0] > Q12(4.0f))
            {
                D_800E200C.vx += Q12_MULT_PRECISE(g_DeltaTime, Q12(-1.2f));
                D_800E200C.vy += Q12_MULT_PRECISE(g_DeltaTime, Q12(-0.4f));
            }

            if (g_SysWork.sysStateStepData[0] > Q12(6.0f))
            {
                SysWork_StateStepIncrement(0);
            }
            break;

        case 10:
            Event_WaitTimer(Q12(2.5f), false);
            break;

        default:
            // Restore player control.
            Player_ControlUnfreeze(false);
            SysWork_StateSetNext(SysState_Gameplay);

            Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, 2, 0, false);
            vcReturnPreAutoCamWork(true);

            Savegame_EventFlagSet(EventFlag_456);
            Event_InvItemCmd(InvItemCmd_AddItem, InvItemId_ChannelingStone, 1, false);
            func_8003D01C();
            Player_WeaponAttackRestore();

            g_Cutscene_Timer = NO_VALUE;

            Sd_SfxStop(Sfx_Unk1467);
            break;
    }

    D_800E201C += g_DeltaTime;
    if (g_Cutscene_Timer >= Q12(0.0f))
    {
        Dms_CharacterTransformGet(&g_SysWork.playerWork.player.position, &g_SysWork.playerWork.player.rotation, "HERO", g_Cutscene_Timer, (s_DmsHeader*)FS_BUFFER_13);
        vcChangeProjectionValue(Dms_CameraTargetsGet(&g_Cutscene_CameraPositionTarget, &g_Cutscene_CameraLookAtTarget, NULL, g_Cutscene_Timer, (s_DmsHeader*)FS_BUFFER_13));
        vcUserCamTarget(&g_Cutscene_CameraPositionTarget, NULL, true);
        vcUserWatchTarget(&g_Cutscene_CameraLookAtTarget, NULL, true);
    }
}

void Map_WorldObjectsInit(void) // 0x800DD3AC
{
    D_800E62D0 = 0;
    D_800E638C = 0;

    WorldObject_PlacementInit(&g_WorldObject6, "BALL_HID", -10.25f, 0.2f, -17.75f);
    WorldObject_Init(&g_WorldObject5, "VALVE_HI", -10.49f, -0.7f, -12.92f, 0.0f, 0.0f, 0.0f);
    WorldObject_Init(&g_WorldObject0, "TOBIRA00", -58.81f, -1.4f, 18.02f, 0.0f, 0.0f, 0.0f);
    WorldObject_Init(&g_WorldObject1, "TOBIRA01", -58.81f, -1.4f, 18.02f, 0.0f, 0.0f, 0.0f);
    WorldObject_Init(&g_WorldObject2, "TOBIRA2_", -61.0f, -1.05f, 20.97f, 0.0f, 0.0f, 0.0f);
    WorldObject_PlacementInit(&g_WorldObject3, "HAKO1_HI", -58.81f, -1.4f, 18.02f);
    WorldObject_PlacementInit(&g_WorldObject4, "HAKO2_HI", -61.0f, -0.9f, 20.97f);
    WorldObject_Init(&g_WorldObject7, "KEY_HIDE", -60.3184f, -0.00732422f, 20.23f, 0.0f, 53.2f, 5.3f);

    if (g_SavegamePtr->gameDifficulty == GameDifficulty_Easy)
    {
        Chara_SpawnFlagsSet(Chara_GreyChildOrMumbler, 1, CharaSpawnFlag_None);
        Chara_SpawnFlagsSet(Chara_GreyChildOrMumbler, 5, CharaSpawnFlag_None);
        Chara_SpawnFlagsSet(Chara_GreyChildOrMumbler, 6, CharaSpawnFlag_None);
        Chara_SpawnFlagsSet(Chara_GreyChildOrMumbler, 7, CharaSpawnFlag_None);
        Chara_SpawnFlagsSet(Chara_Creeper, 5, CharaSpawnFlag_None);
        Chara_SpawnFlagsSet(Chara_Creeper, 6, CharaSpawnFlag_None);
    }

    WorldObject_ModelNameSet(&g_CommonWorldObjects[0], g_CommonWorldObjectNames[2]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[1], g_CommonWorldObjectNames[3]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[2], g_CommonWorldObjectNames[4]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[3], g_CommonWorldObjectNames[5]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[4], g_CommonWorldObjectNames[6]);
    WorldObject_ModelNameSet(&g_CommonWorldObjects[5], g_CommonWorldObjectNames[7]);
}

void Map_WorldObjectsUpdate(void) // 0x800DD688
{
    s32 x;
    MAP_CHUNK_CHECK_VARIABLE_DECL();

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -1, 0, 0) && PLAYER_IN_MAP_CHUNK(vz, 1, -1, 0, 0))
    {
        WorldObjects_Add(&g_WorldObject5.object, &g_WorldObject5.position, &g_WorldObject5.rotation);

        if (Savegame_EventFlagGet(EventFlag_M1S02_UsedRubberBall))
        {
            WorldObjects_Add(&g_WorldObject6.object, &g_WorldObject6.position, &SVECTOR3_Zero);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -2, -1, -2) && PLAYER_IN_MAP_CHUNK(vz, 0, 0, -1, 1))
    {
            func_800DE828();

            if (Savegame_EventFlagGet(EventFlag_107) && !Savegame_EventFlagGet(EventFlag_M1S03_PickupLibraryReserveKey))
            {
                WorldObjects_Add(&g_WorldObject7.object, &g_WorldObject7.position, &g_WorldObject7.rotation);
            }

            if (g_SysWork.playerWork.player.position.vx < Q12(-57.5f) &&
                g_SysWork.playerWork.player.position.vz < Q12(21.6f))
            {
                if (!Savegame_EventFlagGet(EventFlag_100))
                {
                    if (g_SysWork.playerWork.player.position.vx > Q12(-60.0f))
                    {
                        if (D_800E62D0 == 2)
                        {
                            D_800E62D0 = 0;
                        }

                        // Warp camera.
                        Event_CameraPositionSet(NULL,
                                                Q12(-61.18f), Q12(-3.0f), g_SysWork.playerWork.player.position.vz + Q12(0.49f),
                                                Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                                D_800E62D0 == 0);
                        Event_CameraLookAtSet(NULL,
                                              Q12(-58.48f), Q12(-0.31f), g_SysWork.playerWork.player.position.vz - Q12(0.72f),
                                              Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                              D_800E62D0 == 0);

                        D_800E62D0 = 1;
                    }
                    else
                    {
                        Event_CameraPositionSet(NULL,
                                                Q12(-58.48f), Q12(-1.88f), Q12(22.3f),
                                                Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                                true);
                        Event_CameraLookAtSet(NULL,
                                              Q12(-60.88f), Q12(-1.23f), Q12(19.16f),
                                              Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                              true);
                        D_800E62D0 = 2;
                    }
                }
                else if (Savegame_EventFlagGet(EventFlag_107))
                {
                    if (g_SysWork.playerWork.player.position.vz > Q12(18.6f))
                    {
                        // Warp camera.
                        Event_CameraPositionSet(NULL,
                                               Q12(-60.27f), Q12(-0.41f), g_SysWork.playerWork.player.position.vz - Q12(2.25f),
                                               Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                               D_800E62D0 == 0);
                        Event_CameraLookAtSet(NULL,
                                              Q12(-59.75f), Q12(-1.09f), g_SysWork.playerWork.player.position.vz + Q12(1.65f),
                                              Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                              D_800E62D0 == 0);

                        D_800E62D0 = 1;
                    }
                    else
                    {
                        D_800E62D0 = 0;

                        // Warp camera.
                        Event_CameraPositionSet(NULL,
                                                Q12(-60.15f), Q12(-3.3f), Q12(20.52f),
                                                Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                                true);
                        Event_CameraLookAtSet(NULL,
                                              Q12(-59.98f), Q12(-0.85f), Q12(17.36f),
                                              Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                              true);
                    }
                }
            }
            else
            {
                if (D_800E62D0 == 2)
                {
                    D_800E62D0 = 1;
                }

                vcReturnPreAutoCamWork(D_800E62D0);
                D_800E62D0 = 0;
            }
    }

    if ( Savegame_EventFlagGet(EventFlag_104) &&
        !Savegame_EventFlagGet(EventFlag_105))
    {
        // Warp camera.
        Event_CameraPositionSet(NULL,
                                Q12(-60.63f), Q12(-4.14f), Q12(17.42f),
                                Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                true);
        if (g_SysWork.playerWork.player.position.vz < Q12(18.5f))
        {
            Event_CameraLookAtSet(NULL,
                                  Q12(-60.0f), Q12(0.0f), Q12(18.5f),
                                  Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                  false);
        }
        else
        {
            Event_CameraLookAtSet(NULL,
                                  Q12(-60.0f), Q12(0.0f), g_SysWork.playerWork.player.position.vz,
                                  Q12(0.0f), Q12(0.0f), Q12(0.0f), Q12(0.0f),
                                  false);
        }

        D_800E62D0 = 0;
    }

    if (!Savegame_EventFlagGet(EventFlag_53) &&
        PLAYER_IN_MAP_CHUNK(vx, 1, -3, -1, -3) && PLAYER_IN_MAP_CHUNK(vz, 1, 4, -1, 4))
    {
        x = g_SysWork.playerWork.player.position.vx;
        if (x < Q12(-100.0f))
        {
            D_800E638C += g_DeltaTime;
        }
        else if (x < Q12(-99.0f))
        {
            D_800E638C += Q12_MULT_PRECISE(g_DeltaTime, (x + Q12(100.0f)));
        }

        if ((D_800E6388 == 0 && D_800E638C > Q12(2.5f)) ||
            (D_800E6388 == 1 && D_800E638C > Q12(4.0f)) ||
            (D_800E6388 == 2 && D_800E638C > Q12(4.6f)) ||
            (D_800E6388 == 3 && D_800E638C > Q12(7.4f)) ||
            (D_800E6388 == 4 && D_800E638C > Q12(8.6f)) ||
            (D_800E6388 == 5 && D_800E638C > Q12(9.3f)))
        {
            Sfx_WithFlagsPlay(Sfx_Unk1468, &Q12_VECTOR3(-96.1f, -1.2f, 141.8f), 0xFF, SfxFlag_NoDistAtten);
            D_800E6388++;
        }
    }
    else if (D_800E6388)
    {
        Savegame_EventFlagSet(EventFlag_53);
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -4, -1, -4) && PLAYER_IN_MAP_CHUNK(vz, 1, 3, -1, 3))
    {
        if (!Savegame_EventFlagGet(EventFlag_M1S03_ShotgunShells0))
        {
            WorldObjects_Add(&g_CommonWorldObjects[5], &g_CommonWorldObjectPoses[0].position, &g_CommonWorldObjectPoses[0].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -4, -1, -4) && PLAYER_IN_MAP_CHUNK(vz, 1, 4, -1, 4))
    {
        if (!Savegame_EventFlagGet(EventFlag_M1S03_HandgunBullets0))
        {
            WorldObjects_Add(&g_CommonWorldObjects[3], &g_CommonWorldObjectPoses[1].position, &g_CommonWorldObjectPoses[1].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -1, 0, 0) && PLAYER_IN_MAP_CHUNK(vz, 1, 2, -1, 2))
    {
        if (!Savegame_EventFlagGet(EventFlag_M1S03_HealthDrink0))
        {
            WorldObjects_Add(&g_CommonWorldObjects[1], &g_CommonWorldObjectPoses[2].position, &g_CommonWorldObjectPoses[2].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -4, -1, -4) && PLAYER_IN_MAP_CHUNK(vz, 1, 2, -1, 2))
    {
        if (!Savegame_EventFlagGet(EventFlag_M1S03_HandgunBullets1))
        {
            WorldObjects_Add(&g_CommonWorldObjects[3], &g_CommonWorldObjectPoses[3].position, &g_CommonWorldObjectPoses[3].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -4, -1, -4) && PLAYER_IN_MAP_CHUNK(vz, 1, 2, -1, 2))
    {
        if (!Savegame_EventFlagGet(EventFlag_M1S03_HandgunBullets2))
        {
            WorldObjects_Add(&g_CommonWorldObjects[3], &g_CommonWorldObjectPoses[4].position, &g_CommonWorldObjectPoses[4].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -4, -1, -4) && PLAYER_IN_MAP_CHUNK(vz, 1, 2, -1, 2))
    {
        if (!Savegame_EventFlagGet(EventFlag_M1S03_ShotgunShells1))
        {
            WorldObjects_Add(&g_CommonWorldObjects[5], &g_CommonWorldObjectPoses[5].position, &g_CommonWorldObjectPoses[5].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -2, -1, -2) && PLAYER_IN_MAP_CHUNK(vz, 1, 4, -1, 4))
    {
        if (!Savegame_EventFlagGet(EventFlag_M1S03_HandgunBullets3))
        {
            WorldObjects_Add(&g_CommonWorldObjects[3], &g_CommonWorldObjectPoses[6].position, &g_CommonWorldObjectPoses[6].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -3, -1, -3) && PLAYER_IN_MAP_CHUNK(vz, 1, 2, -1, 2))
    {
        if (!Savegame_EventFlagGet(EventFlag_M1S03_HealthDrink1))
        {
            WorldObjects_Add(&g_CommonWorldObjects[1], &g_CommonWorldObjectPoses[7].position, &g_CommonWorldObjectPoses[7].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -2, -1, -2) && PLAYER_IN_MAP_CHUNK(vz, 1, 2, -1, 2))
    {
        if (!Savegame_EventFlagGet(EventFlag_M1S03_HealthDrink2))
        {
            WorldObjects_Add(&g_CommonWorldObjects[1], &g_CommonWorldObjectPoses[8].position, &g_CommonWorldObjectPoses[8].rotation);
        }
    }

    if (PLAYER_IN_MAP_CHUNK(vx, 1, -3, -1, -3) && PLAYER_IN_MAP_CHUNK(vz, 1, 4, -1, 4))
    {
        if (!Savegame_EventFlagGet(EventFlag_M1S03_FirstAidKit))
        {
            WorldObjects_Add(&g_CommonWorldObjects[0], &g_CommonWorldObjectPoses[9].position, &g_CommonWorldObjectPoses[9].rotation);
        }
    }
}

void func_800DE624(void) // 0x800DE624
{
    q19_12 playerHealth;

    if (Savegame_EventFlagGet(EventFlag_M1S02_PickupClassroomKey) && g_SavegamePtr->gameDifficulty != GameDifficulty_Easy)
    {
        Chara_SpawnFlagsSet(Chara_GreyChildOrMumbler, 9, CharaSpawnFlag_0 | CharaSpawnFlag_1);
        Chara_SpawnFlagsSet(Chara_GreyChildOrMumbler, 10, CharaSpawnFlag_0 | CharaSpawnFlag_1);
    }

    if (Savegame_EventFlagGet(EventFlag_M1S03_PickupLibraryReserveKey))
    {
        if (g_SavegamePtr->gameDifficulty != GameDifficulty_Easy)
        {
            Chara_SpawnFlagsSet(Chara_GreyChildOrMumbler, 12, CharaSpawnFlag_0 | CharaSpawnFlag_1);
        }

        if (g_SavegamePtr->gameDifficulty == GameDifficulty_Hard)
        {
            Chara_SpawnFlagsSet(Chara_GreyChildOrMumbler, 13, CharaSpawnFlag_0 | CharaSpawnFlag_1);
            Chara_SpawnFlagsSet(Chara_GreyChildOrMumbler, 11, CharaSpawnFlag_0 | CharaSpawnFlag_1 | CharaSpawnFlag_2);
        }

        Chara_SpawnFlagsSet(Chara_GreyChildOrMumbler, 14, CharaSpawnFlag_0 | CharaSpawnFlag_1 | CharaSpawnFlag_3);
        Chara_SpawnFlagsSet(Chara_GreyChildOrMumbler, 15, CharaSpawnFlag_0 | CharaSpawnFlag_1);
    }

    playerHealth = g_SysWork.playerWork.player.health;
    if (g_SavegamePtr->gameDifficulty == GameDifficulty_Easy)
    {
        playerHealth -= Q12(50.0f);
    }

    if (g_SavegamePtr->gameDifficulty == GameDifficulty_Normal)
    {
        playerHealth -= Q12(25.0f);
    }

    if (playerHealth > Q12(20.0f))
    {
        Chara_SpawnFlagsSet(Chara_Creeper, 0, CharaSpawnFlag_0 | CharaSpawnFlag_1);
    }
    else
    {
        Chara_SpawnFlagsSet(Chara_Creeper, 0, CharaSpawnFlag_None);
    }

    if (playerHealth > Q12(60.0f))
    {
        Chara_SpawnFlagsSet(Chara_Creeper, 1, CharaSpawnFlag_0 | CharaSpawnFlag_1);
        Chara_SpawnFlagsSet(Chara_Creeper, 2, CharaSpawnFlag_0 | CharaSpawnFlag_1);
    }
    else
    {
        Chara_SpawnFlagsSet(Chara_Creeper, 1, CharaSpawnFlag_None);
        Chara_SpawnFlagsSet(Chara_Creeper, 2, CharaSpawnFlag_None);
    }

    if (playerHealth > Q12(80.0f))
    {
        Chara_SpawnFlagsSet(Chara_Creeper, 3, CharaSpawnFlag_0 | CharaSpawnFlag_1);
        Chara_SpawnFlagsSet(Chara_Creeper, 4, CharaSpawnFlag_0 | CharaSpawnFlag_1);
    }
    else
    {
        Chara_SpawnFlagsSet(Chara_Creeper, 3, CharaSpawnFlag_None);
        Chara_SpawnFlagsSet(Chara_Creeper, 4, CharaSpawnFlag_None);
    }
}

void func_800DE828(void) // 0x800DE828
{
    s32 tmp0;
    s32 tmp1;

    const static VECTOR3 sfxPos = VECTOR3(-58.81f, -1.4f, 18.02f);

    if (g_DeltaTime == Q12(0.0f))
    {
        if (Savegame_EventFlagGet(EventFlag_105))
        {
            WorldObjects_Add(&g_WorldObject4.object, &g_WorldObject4.position, &SVECTOR3_Zero);
        }

        if (Savegame_EventFlagGet(EventFlag_101) && Savegame_EventFlagGet(EventFlag_102))
        {
            WorldObjects_Add(&g_WorldObject3.object, &g_WorldObject3.position, &SVECTOR3_Zero);
        }

        WorldObjects_Add(&g_WorldObject0.object, &g_WorldObject0.position, &g_WorldObject0.rotation);
        WorldObjects_Add(&g_WorldObject1.object, &g_WorldObject1.position, &g_WorldObject1.rotation);
        WorldObjects_Add(&g_WorldObject2.object, &g_WorldObject2.position, &g_WorldObject2.rotation);
    }
    else
    {
        if ( Savegame_EventFlagGet(EventFlag_105) &&
            !Savegame_EventFlagGet(EventFlag_106))
        {
            if (D_800E20EE == 6)
            {
                func_800892A4(18);
                Sfx_WithFlagsPlay(Sfx_Unk1460, &g_WorldObject2.position, Q8_CLAMPED(1.0f), SfxFlag_NoDistAtten);

                D_800E20E8 = Q12_ANGLE(-11.3f);
                D_800E20EE = 7;
                D_800E20EC = 0;
            }

            g_WorldObject2.rotation.vy += D_800E20E8;

            if (D_800E20E8 == Q12_ANGLE(-11.3f))
            {
                if (g_WorldObject2.rotation.vy < Q12_ANGLE(-168.8f))
                {
                    g_WorldObject2.rotation.vy = Q12_ANGLE(-168.8f);
                    D_800E20E8 = Q12_ANGLE(2.9f);

                    Sfx_WithFlagsPlay(Sfx_Unk1434, &g_WorldObject2.position, Q8_CLAMPED(1.0f), SfxFlag_NoDistAtten);
                }
            }
            else if ((g_WorldObject2.rotation.vy < Q12_ANGLE(-168.8f) && D_800E20E8 < 0) ||
                     (g_WorldObject2.rotation.vy >= Q12_ANGLE(-157.45f) && D_800E20E8 > 0))
            {
                tmp1 = -D_800E20E8 >> 1;
                D_800E20E8 = tmp1;
                if (ABS(tmp1) == 1)
                {
                    D_800E20E8 = 0;
                }
            }

            if (D_800E20E8 == 0)
            {
                Savegame_EventFlagSet(EventFlag_106);
            }

            WorldObjects_Add(&g_WorldObject4.object, &g_WorldObject4.position, &SVECTOR3_Zero);
        }
        else if (Savegame_EventFlagGet(EventFlag_106))
        {
            g_WorldObject2.rotation.vy = Q12_ANGLE(-157.4f);
            WorldObjects_Add(&g_WorldObject4.object, &g_WorldObject4.position, &SVECTOR3_Zero);
        }

        if (!Savegame_EventFlagGet(EventFlag_101) || !Savegame_EventFlagGet(EventFlag_102))
        {
            if (D_800E20E8 == 0)
            {
                if (!Rng_GenerateUInt(0, 31)) // 1 in 32 chance.
                {
                    D_800E20E8 = -Rng_GenerateUInt(2, 9);
                }
            }

            if (D_800E20E8 != 0)
            {
                if (ABS(D_800E20E8) == 1)
                {
                    g_WorldObject1.rotation.vy += D_800E20E8;
                }
                g_WorldObject1.rotation.vy += D_800E20E8;

                if (D_800E20E8 > 0)
                {
                    if (g_WorldObject1.rotation.vy > 0)
                    {
                        Sfx_WithFlagsPlay(Sfx_Unk1435, &sfxPos, Q8(0.5f), SfxFlag_None);
                        g_WorldObject1.rotation.vy = 0;

                        if (D_800E20E8 >= 8 && !Rng_GenerateUInt(0, 3)) // 1 in 4 chance.
                        {
                            D_800E20E8 = 0;
                        }
                        else
                        {
                            D_800E20E8 = (Rng_GenerateUInt(0, 3) - D_800E20E8) + 1;
                        }
                        D_800E20E8 = MIN(D_800E20E8, 0);
                    }
                }
                else if (g_WorldObject1.rotation.vy < Q12_ANGLE(-1.8f))
                {
                    Sfx_WithFlagsPlay(Sfx_Unk1435, &sfxPos, Q8(0.5f), SfxFlag_None);

                    D_800E20E8 = ~Rng_GenerateUInt(0, 3) - D_800E20E8;
                    D_800E20E8 = MAX(D_800E20E8, 1);
                }
            }
        }
        else if (!Savegame_EventFlagGet(EventFlag_M1S03_LockerOpened) || D_800E20EE == 5)
        {
            switch (D_800E20EE)
            {
                case 0:
                    D_800E20E8 = Q12_ANGLE(-1.45f);
                    D_800E20EC = 0;
                    D_800E20EE = 1;

                case 1:
                    g_WorldObject1.rotation.vy += D_800E20E8;
                    if (g_WorldObject1.rotation.vy <  Q12_ANGLE(-2.9f))
                    {
                        func_800892DC(18, 0x60);
                        Sfx_WithFlagsPlay(Sfx_Unk1434, &sfxPos, Q8(0.7657f), SfxFlag_NoDistAtten);

                        g_WorldObject1.rotation.vy = Q12_ANGLE(-2.9f);
                        D_800E20E8 = Q12_ANGLE(1.45f);
                        D_800E20EE = 2;
                    }
                    break;

                case 2:
                    D_800E20E8 -= 2;
                    g_WorldObject1.rotation.vy += D_800E20E8;

                    if (g_WorldObject1.rotation.vy > 0)
                    {
                        Sfx_WithFlagsPlay(Sfx_Unk1434, &sfxPos, Q8(0.7657f), SfxFlag_NoDistAtten);
                        do
                        {
                            D_800E20E8 = Q12(-10.0f);
                            g_WorldObject1.rotation.vy = 0;
                            D_800E20EE = 3;
                        }
                        while (0); // @hack permuter find.
                    }
                    break;

                case 3:
                    D_800E20EC += g_DeltaTime;
                    if (D_800E20EC > Q12(3.0f))
                    {
                        D_800E20EE = 4;
                    }
                    break;

                case 4:
                case 5:
                    D_800E20E8 += Q12_MULT_PRECISE(g_DeltaTime, Q12(4.0f));
                    if (D_800E20E8 <= Q12(-2.0f))
                    {
                        tmp0 = D_800E20E8;
                    }
                    else
                    {
                        tmp0 = Q12(-2.0f);
                    }

                    D_800E20E8 = tmp0;
                    g_WorldObject1.rotation.vy += FP_FROM(tmp0, Q12_SHIFT);

                    WorldObjects_Add(&g_WorldObject3.object, &g_WorldObject3.position, &SVECTOR3_Zero);

                    if (D_800E20EE == 4)
                    {
                        if (g_WorldObject1.rotation.vy < Q12_ANGLE(-5.7f))
                        {
                            Sfx_WithFlagsPlay(Sfx_Unk1436, &g_WorldObject1.position, Q8(0.5f), SfxFlag_None);
                            D_800E20EE = 5;
                        }
                    }

                    if (g_WorldObject1.rotation.vy < Q12_ANGLE(-39.4f))
                    {
                        Savegame_EventFlagSet(EventFlag_M1S03_LockerOpened);
                    }

                    if (Savegame_EventFlagGet(EventFlag_104) || ScreenFade_IsFinished() ||
                        g_WorldObject1.rotation.vy < Q12_ANGLE(-67.5f))
                    {
                        g_WorldObject1.rotation.vy = Q12_ANGLE(-67.5f);
                        D_800E20EE = 6;
                    }
                    break;

                default:
                    break;
            }
        }
        else
        {
            g_WorldObject1.rotation.vy = Q12_ANGLE(-157.5f);
            WorldObjects_Add(&g_WorldObject3.object, &g_WorldObject3.position, &SVECTOR3_Zero);

            if (!Savegame_EventFlagGet(EventFlag_105))
            {
                D_800E20EE = 6;
            }
        }

        g_WorldObject0.rotation.vy = g_WorldObject1.rotation.vy;
        WorldObjects_Add(&g_WorldObject0.object, &g_WorldObject0.position, &g_WorldObject0.rotation);
        WorldObjects_Add(&g_WorldObject1.object, &g_WorldObject1.position, &g_WorldObject1.rotation);
        WorldObjects_Add(&g_WorldObject2.object, &g_WorldObject2.position, &g_WorldObject2.rotation);
    }
}
