#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "bodyprog/text/text_draw.h"
#include "bodyprog/sound/sound_system.h"
#include "main/rng.h"

// TODO: .bss is emitted in order of first declaration, right now our headers
// declare externs in semi-random order though.
// For now we'll declare them before headers in the order they appear in the 
// map .bss, later can hopefully improve order in headers instead.
extern s32                     D_800CD768;
extern s32                     D_800CD76C;
extern s32                     D_800CD770;
extern s32                     D_800CD774;
extern e_MapIdx                g_ParticleMapIdx0;
extern e_MapIdx                g_ParticleMapIdx1;
extern s32                     g_Particle_SpeedX;
extern s32                     g_Particle_SpeedZ;
extern s32                     sharedData_800DFB6C_0_s00;
extern s32                     sharedData_800DFB70_0_s00;
extern s32                     __pad_bss_800CD790[2];
extern s_MapHdr_field_4C       sharedData_800DFB7C_0_s00[];
extern s_BloodSplat            g_Effect_BloodSplats[];
extern s32                     __pad_bss_800CDFCC[3];
extern struct _ParticleVectors g_ParticleVectors0;
extern s32                     __pad_bss_800CE004;
extern struct _ParticleVectors g_ParticleVectors1;
extern s32                     sharedData_800E0CA8_0_s00;
extern s32                     sharedData_800E0CAC_0_s00;
extern u32                     sharedData_800E0CB0_0_s00;
extern u16                     sharedData_800E0CB4_0_s00;
extern u16                     sharedData_800E0CB6_0_s00;
extern u16                     sharedData_800E0CB8_0_s00;
extern s8                      sharedData_800E0CBA_0_s00;
extern s8                      __pad_bss_800CE047;
extern s32                     sharedData_800E39D8_0_s00;
extern q19_12                  g_Player_MoveSpeed;

#include "maps/map1/map1_s04.h"
#include "maps/common/particle.h"
#include "inline_no_dmpsx.h"

#include <psyq/gtemac.h>

#include "maps/common/characters/player.h"

s32 D_800CD768;
s32 D_800CD76C;
s32 D_800CD770;
s32 D_800CD774;

s32 __pad_bss_800CD790[2];
s32 __pad_bss_800CDFCC[3];
s32 __pad_bss_800CE004;
s8  __pad_bss_800CE047;

#include "../src/maps/common/particles/particle.c"

#include "../src/maps/common/characters/player.c"

#include "../src/maps/common/map_util.c" // 0x800CC818

void Map_RoomBgmInit(bool arg0) // 0x800CC8C8
{
    s32 fadeSpeed;
    s32 mapRoomIdx;
    s32 bgmFlags;

    mapRoomIdx = g_SavegamePtr->mapRoomIdx;
    bgmFlags     = D_800CCF5C[mapRoomIdx];
    fadeSpeed     = Q12(0.1f);

    switch (mapRoomIdx)
    {
        case 23:
            bgmFlags = 1;
            if (Savegame_EventFlagGet(EventFlag_MapMark_AltSchool1F_CourtyardQuestion))
            {
                bgmFlags = 0x1FE;
                if (Savegame_EventFlagGet(EventFlag_MapMark_308) || Savegame_EventFlagGet(EventFlag_MapMark_AltSchool1F_CourtyardTopArrows))
                {
                    bgmFlags = 0x10;
                }
            }
            break;

        case 38:
            if (Savegame_EventFlagGet(EventFlag_100) && !Savegame_EventFlagGet(EventFlag_107))
            {
                bgmFlags = 1;
                fadeSpeed = Q12(0.3f);
            }
            break;

        case 5:
            if (Savegame_EventFlagGet(EventFlag_121))
            {
                fadeSpeed = Q12(240.0f);
                if (Savegame_EventFlagGet(EventFlag_96))
                {
                    bgmFlags |= 8;
                }
                else
                {
                    bgmFlags = 1;
                }
            }
            break;

        case 11:
            Savegame_EventFlagClear(EventFlag_121);
            break;

        case 7:
            if (!Savegame_EventFlagGet(EventFlag_98) && Savegame_EventFlagGet(EventFlag_122))
            {
                fadeSpeed = Q12(240.0f);
                bgmFlags = 1;
            }
            break;
    }

    Bgm_LayersUpdate(bgmFlags, fadeSpeed, &D_800CCF54);
}

void GameBoot_LoadScreen_StageString(void) {} // 0x800CCA24

const char* MAP_MESSAGES[] = {
    #include "maps/common/shared/map_msg_common.inc"
#if VERSION_REGION_IS(NTSC)
    /* 15 */ "\tNO_STAGE! ~E "
#elif VERSION_REGION_IS(NTSCJ)
    /* 15 */ "\tこのステージは存在しません ~E "
#endif
};

s_EventData MAP_EVENTS[] = {
#include "map_events.h"
};

// TODO: change to `e_SfxId`
u16 D_800CD038[23][40] = {
    {
        0x100D, 0x1023, 0x1024, 0x101E, 0x1017, 0x101F, 0x1022, 0x1001, 0x1028, 0x102A,
        0x1004, 0x1005, 0x104D, 0x1041, 0x1037, 0x1054, 0x1053, 0x104F, 0x103D, 0x1045,
        0x1048,
    },
    {
        0x1003, 0x1010, 0x1009, 0x101D, 0x101B, 0x1030, 0x1019, 0x1008, 0x102F, 0x1033,
        0x1015, 0x1021, 0x1018, 0x1007, 0x1027, 0x1026, 0x102E, 0x1034, 0x102C, 0x102D,
        0x100F, 0x100A, 0x1016, 0x1032, 0x100C, 0x1012, 0x102B, 0x101A, 0x1006, 0x1014,
        0x1029, 0x1020, 0x100B, 0x1025, 0x100E, 0x1013, 0x1011, 0x1031, 0x101C, 0x1002,
    },
    {
        0x104E, 0x1039, 0x1051, 0x104C, 0x1047, 0x1055, 0x1050, 0x1046, 0x1049, 0x1052,
        0x103E, 0x104B, 0x103C, 0x1042, 0x1040, 0x1043, 0x103F, 0x1044, 0x103A, 0x104A,
    },
    {
        0x10FC, 0x10FF, 0x10CD, 0x10BA, 0x1073, 0x108A, 0x10EC, 0x1076, 0x109B, 0x10C8,
        0x105C, 0x10A5, 0x1089, 0x108D, 0x10C0, 0x1088, 0x10B5, 0x1072, 0x10D5, 0x10A7,
        0x10CA, 0x1087, 0x10A4, 0x1067, 0x10D9, 0x10A3, 0x10B6, 0x105F, 0x10B9, 0x10BE,
        0x109E, 0x10B2, 0x10E0, 0x10B1,
    },
    {
        0x107C, 0x10EB, 0x1086, 0x10DC, 0x105E, 0x10B0, 0x107D, 0x10D4, 0x1075, 0x10D0,
        0x1079, 0x1095, 0x1071, 0x109D, 0x106A, 0x10F6, 0x10AD, 0x105B, 0x10DD, 0x10AC,
        0x1066, 0x1065, 0x1090, 0x106E, 0x10B8, 0x10BC, 0x10CF, 0x10FB, 0x1094, 0x109A,
        0x109F, 0x1097,
    },
    {
        0x10DF, 0x10B7, 0x10F5, 0x1083, 0x10AB, 0x10EA, 0x1074, 0x1058, 0x107F, 0x1077,
        0x1082, 0x1059, 0x1056, 0x105A, 0x1085, 0x10F1, 0x107A, 0x1093, 0x10DB,
    },
    {
        0x10D8, 0x10E7, 0x10ED, 0x108F, 0x106B, 0x10E3, 0x108B, 0x1092, 0x10DA, 0x10C1,
        0x10B4, 0x10A2, 0x10A1, 0x10C7, 0x10E6, 0x10AF, 0x10AA, 0x10B3, 0x107E, 0x1081,
        0x10C9, 0x10A9, 0x105D, 0x108C, 0x106D, 0x10F4, 0x10A6, 0x109C, 0x106C, 0x10CC,
        0x10FA, 0x10E9, 0x1069, 0x10D7, 0x10CB, 0x1091, 0x10D6, 0x10C6, 0x10A8, 0x10C5,
    },
    {
        0x10BD, 0x1080, 0x107B, 0x1084, 0x10E2, 0x10DE, 0x10E5, 0x10E1, 0x10CE, 0x10E8,
        0x10FE, 0x1068, 0x10C4, 0x10BB, 0x10F9, 0x10D1, 0x10C3, 0x1078, 0x10F3, 0x10F2,
        0x10F8, 0x10D3, 0x1057, 0x10BF, 0x10D2, 0x1070, 0x1064, 0x1099, 0x106F, 0x1062,
        0x108E, 0x10F7, 0x1060, 0x10FD, 0x10F0, 0x10C2,
    },
    {
        0x1063, 0x10A0, 0x10AE, 0x1098, 0x1061, 0x10EF, 0x10EE, 0x1158, 0x1157, 0x112F,
        0x1175,
    },
    {
        0x113B, 0x1156, 0x115A, 0x114A, 0x1135, 0x1159, 0x1108, 0x1152, 0x1134, 0x1128,
        0x1107, 0x1126, 0x1133, 0x1112, 0x1101, 0x1104, 0x112D, 0x1116, 0x1137, 0x1125,
        0x111C, 0x110B, 0x1106, 0x1102, 0x110D, 0x1114, 0x1111, 0x110C, 0x1144, 0x1103,
        0x1105, 0x113E, 0x110A,
    },
    {
        0x116D, 0x1162, 0x1117, 0x1145, 0x1161, 0x1131, 0x1136, 0x1164, 0x1151, 0x116B,
        0x115F, 0x1140, 0x1177, 0x113D, 0x114B, 0x111F, 0x1139, 0x1172, 0x114C, 0x1132,
        0x1143, 0x1150, 0x1163, 0x111D, 0x1165, 0x1142, 0x1129, 0x111A, 0x1124, 0x112E,
        0x1141, 0x116F,
    },
    {
        0x1100, 0x116E, 0x1168, 0x1138, 0x113C, 0x1153, 0x1171, 0x1176, 0x115B, 0x1149,
        0x112B, 0x113F, 0x1167, 0x1160, 0x112A, 0x1155, 0x1173, 0x11C6, 0x11C3, 0x11AE,
        0x1185, 0x11D5, 0x11A3, 0x11C4, 0x119B, 0x11AC, 0x1198, 0x11CF, 0x118E, 0x1191,
        0x11B1, 0x1181, 0x11C0, 0x11D4, 0x11D6, 0x11CB,
    },
    {
        0x11CE, 0x11CA, 0x11BF, 0x11C2, 0x11B5, 0x11D3, 0x119D, 0x1187, 0x11C9, 0x11A9,
        0x11B0, 0x1186, 0x11BB, 0x118B, 0x1197, 0x11BA, 0x1193, 0x11A2, 0x1183, 0x119F,
        0x1190, 0x1182, 0x117C, 0x119A, 0x11B4, 0x11A8, 0x11B7, 0x11A5, 0x11CD, 0x1196,
        0x11A0, 0x11D1, 0x119E, 0x11B6, 0x11B9, 0x1179, 0x11B8, 0x11A1, 0x1180, 0x117B,
    },
    {
        0x1195, 0x1184, 0x11A4, 0x117A, 0x1192, 0x117E, 0x1178, 0x1194, 0x118D, 0x118F,
        0x117D, 0x1189, 0x11A7, 0x1199, 0x11BE, 0x11AD, 0x118C, 0x1188, 0x11C8, 0x11AB,
        0x117F, 0x11B2, 0x11BD, 0x11A6, 0x11AA, 0x11C7, 0x11C1, 0x119C, 0x11C5,
    },
    {
        0x11AF, 0x118A, 0x11B3, 0x11CC, 0x11D2, 0x11D0, 0x11BC, 0x11D7, 0x11D9, 0x1220,
        0x122C, 0x1206, 0x11E0, 0x11EA, 0x11DF, 0x1215, 0x1217, 0x11E9, 0x120E, 0x1203,
        0x11EF, 0x120A, 0x1209, 0x121B, 0x11FF, 0x1216, 0x120D, 0x11E4, 0x1208, 0x11ED,
        0x121F,
    },
    {
        0x1219, 0x1222, 0x11E3, 0x121D, 0x11F2, 0x122D, 0x1231, 0x121A, 0x1224, 0x122B,
        0x1221, 0x1213, 0x11F4, 0x1230, 0x1212, 0x120C, 0x1228, 0x11E6, 0x1227, 0x1202,
        0x11DE, 0x1218, 0x11EE, 0x11E5, 0x11E2, 0x1205, 0x11DD, 0x11FA, 0x11D8, 0x11FE,
        0x121E, 0x11DC, 0x1211, 0x122A,
    },
    {
        0x1226, 0x122F, 0x11E7, 0x1223, 0x11F5, 0x1204, 0x121C, 0x1201, 0x11F8, 0x11F0,
        0x11F7, 0x11E8, 0x11FD, 0x11DB, 0x1225, 0x11FC, 0x11F1, 0x1210, 0x11FB, 0x11F3,
        0x122E, 0x1214, 0x11F9,
    },
    {
        0x120F, 0x1229, 0x120B, 0x1207, 0x11E1, 0x1239, 0x124B, 0x1249, 0x1248, 0x1253,
        0x1243,
    },
    {
        0x1252, 0x1245, 0x124E, 0x123E, 0x123D, 0x124D, 0x1247, 0x1251, 0x1240, 0x1241,
        0x124A, 0x123F, 0x1236, 0x1250, 0x1244, 0x123B, 0x124C, 0x123C, 0x123A, 0x1242,
        0x1246, 0x124F,
    },
    {
        0x1268, 0x12BA, 0x1278, 0x128F, 0x127D, 0x129F, 0x12AA, 0x1283, 0x126E, 0x1271,
        0x126F, 0x12A1, 0x129D, 0x126A, 0x12A6, 0x12A0, 0x12A8, 0x12A9, 0x125D, 0x12B4,
        0x1269, 0x1289, 0x1286, 0x125F, 0x12C9, 0x1270, 0x1260, 0x127C, 0x12C6,
    },
    {
        0x1274, 0x128E, 0x1273, 0x1262, 0x129C, 0x125C, 0x12BC, 0x12A7, 0x12C7, 0x12CB,
        0x126D, 0x12A5, 0x12B3, 0x1275, 0x12AF, 0x1299, 0x12C4, 0x1280, 0x12CD, 0x12C5,
        0x128C, 0x127A, 0x129B, 0x1282, 0x1263, 0x1288, 0x1292, 0x1281, 0x129E, 0x126B,
        0x1295, 0x1298, 0x1277, 0x1276, 0x1265, 0x127F,
    },
    {
        0x12B2, 0x12AE, 0x12CC, 0x12C2, 0x12C1, 0x12B9, 0x1279, 0x1290, 0x12AD, 0x12A3,
        0x1285, 0x1297, 0x128D, 0x12C0, 0x12B1, 0x12B8, 0x12AC, 0x12A4, 0x1259, 0x12A2,
        0x12BF, 0x128B, 0x12BB, 0x127E, 0x12D0, 0x1287, 0x1294, 0x1296, 0x12B7, 0x1284,
        0x1261, 0x1257,
    },
    {
        0x12B0, 0x12C8, 0x12C3, 0x12B6, 0x12CA, 0x12AB, 0x12BE, 0x12B5, 0x1293, 0x12BD,
        0x12D2, 0x12D1, 0x1272, 0x12CE, 0x12CF,
    },
};

void func_800CCA2C(void) // 0x800CCA2C
{
    s32 temp_s0;

    switch (g_SysWork.sysStateSteps[0])
    {
        case 0:
            Player_ControlFreeze();
            CutsceneBorder_ForceShow();
            SysWork_StateStepIncrement(0);

        case 1:
            if (Pad_ButtonClicked(ControllerFlag_R3))
            {
                D_800CD768 = 0;
                D_800CD76C = 0;
            }

            if (Pad_ButtonPulsed(ControllerFlag_LStickHighDown))
            {
                if (D_800CD768 == 0)
                {
                    D_800CD768 = 23;
                }
                D_800CD768--;
            }

            if (Pad_ButtonPulsed(ControllerFlag_LStickHighUp))
            {
                D_800CD768++;
                if (D_800CD768 == 23)
                {
                    D_800CD768 = 0;
                }
            }

            if (Pad_ButtonPulsed(ControllerFlag_LStickHighLeft))
            {
                if (D_800CD76C != 0)
                {
                    D_800CD76C--;
                }
            }

            if (Pad_ButtonPulsed(ControllerFlag_LStickHighRight))
            {
                if (D_800CD76C != 39 && D_800CD038[D_800CD768][D_800CD76C + 1] != 0)
                {
                    D_800CD76C++;
                }
            }

            if (Pad_ButtonClicked(ControllerFlag_Circle))
            {
                SD_Call(D_800CD038[D_800CD768][D_800CD76C]);
                D_800CD770 = 0;
                D_800CD774 = 1;
            }

            temp_s0 = D_800CD774;
            if (temp_s0 == 1 && Sd_AudioStreamingCheck() != AudioStreamingState_XaPlaying)
            {
                D_800CD774 = 2;
            }

            if (D_800CD774 == 2 && Sd_AudioStreamingCheck() == AudioStreamingState_XaPlaying)
            {
                D_800CD770 += g_DeltaTime;

                if (Pad_ButtonClicked(ControllerFlag_Cross))
                {
                    D_800CD774 = 0;
                }
            }

            Text_Debug_PositionSet(50, 50);

#if VERSION_EQUAL_OR_OLDER(PROTO_981216)
            // Code seen in 98-12-16 build.
            Text_Debug_Draw("VS 0:");
            Text_Debug_Draw(Text_Debug_IntToString(3, D_800CD768));
            Text_Debug_Draw("\n");
            Text_Debug_Draw("VS 1:");
            Text_Debug_Draw(Text_Debug_IntToString(3, D_800CD76C));
            Text_Debug_Draw("\n");
            Text_Debug_Draw("SDST:");
            Text_Debug_Draw(Text_Debug_IntToString(3, Sd_AudioStreamingCheck()));
            Text_Debug_Draw("\n");

            if (D_800CD774 != 2)
            {
                Text_Debug_Draw("TIME:");
                // 98-12-16 calls into `Text_Debug_DrawQ12` which is missing in retail. That func then calls `Text_Debug_DrawDecimal`.
                Text_Debug_DrawQ12(6, 2, D_800CD770);
                Text_Debug_Draw("\n");
            }
#endif

            if (Pad_ButtonClickedBind(skip))
            {
                SysWork_StateStepIncrement(0);
            }
            break;

        case 2:
            if (Pad_ButtonClicked(ControllerFlag_Circle))
            {
                SD_Call(D_800CD768_tbl[D_800CD768]);
            }

            if (Pad_ButtonClicked(ControllerFlag_Cross))
            {
                Sd_SfxStop(D_800CD768_tbl[D_800CD768]);
            }

            if (Pad_ButtonPulsed(ControllerFlag_LStickHighDown))
            {
                if (D_800CD768 == 0)
                {
                    D_800CD768 = 4;
                }
                D_800CD768--;
            }

            if (Pad_ButtonPulsed(ControllerFlag_LStickHighUp))
            {
                D_800CD768++;
                if (D_800CD768 == 4)
                {
                    D_800CD768 = 0;
                }
            }
            break;
    }
}

void Map_WorldObjectsInit(void) {} // 0x800CCE30

void Map_WorldObjectsUpdate(void) // 0x800CCE38
{
    if (Pad_ButtonClicked(ControllerFlag_R1))
    {
        SD_Call(Sfx_UseKey);
    }

    if (Pad_ButtonClicked(ControllerFlag_R2))
    {
        SD_Call(Sfx_DoorUnlocked);
    }

    if (Pad_ButtonClicked(ControllerFlag_R3))
    {
        SD_Call(Sfx_Unk1343);
    }
}