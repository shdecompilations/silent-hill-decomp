#include "game.h"

#include "bodyprog/bodyprog.h"
#include "bodyprog/gfx/map_effects.h"
#include "bodyprog/item_screens.h"
#include "bodyprog/math/math.h"
#include "bodyprog/player.h"
#include "bodyprog/ranking.h"
#include "bodyprog/screen/screen_data.h"
#include "bodyprog/screen/screen_data.h"
#include "bodyprog/sys/joy.h"
#include "bodyprog/text/text_draw.h"
#include "main/rng.h"

const s32 __pad_rodata_800262F8 = 0;

GsF_LIGHT g_Items_Lights[7][2];

GsF_LIGHT D_800C3A88[4];

GsF_LIGHT D_800C3AC8[2];

GsCOORDINATE2 D_800C3AE8; // 0x800C3AE8

SVECTOR3 D_800C3B38; // 0x800C3B38

s16 __pad_bss_800C3B3E[5];

VbRVIEW D_800C3B48; // 0x800C3B48

DVECTOR D_800C3B68[4][4]; // 0x800C3BE8 - Type assumed.

s32 g_Inventory_ScrollTransitionTimer; // 0x800AE1A4

s32 D_800C3BA8;

s32 D_800C3BAC;

s32 __pad_bss_800C3BB0[2];

u8 g_Item_MapLoadableItems[48]; // 0x800C3BB8

GsCOORD2PARAM g_Items_Transforms[DISPLAYED_ITEM_COUNT_MAX]; // 0x800C3BE8

GsDOBJ2 g_Items_ItemsModelData[9]; // 0x800C3D78

GsDOBJ2 D_800C3E08;

s32 D_800C3E18[7]; // 0x800C3E18

s32 g_Inventory_EquippedItemIdx; // 0x800C3E34

s32 __pad_bss_800C3E38[2];

u8 D_800C3E40;

s8 __pad_bss_800C3E41[7];

GsCOORDINATE2 g_Items_Coords[DISPLAYED_ITEM_COUNT_MAX]; // 0x800C3E48

#include "item_rotations.h"

static u8 g_Items_GunsMaxLoadAmmo[36] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    15, 6, 6, NO_VALUE
};

u32 D_800AD4C4 = 0;

// Weapon attack data?
s_800AD4C8 D_800AD4C8[70] = {
    { Q12(0.4f),  Q12(0.1f),      0x0082, 0, 8, 0, Chara_GreyChild, 0x33, 0x03, Q12(0.0f), 3, 3, 1, -1, 2, 0, &D_800AD4C4 },
    { Q12(1.0f),  Q12(15.9f) + 1, 0x0046, 102, 6, 0, Chara_GreyChild, 0xCC, 0x04, Q12(1.0f), 7, 4, 2, -1, 3, 0, &D_800AD4C4 },
    { Q12(1.0f),  Q12(0.6f),      0x00C8, 0, 8, 0, Chara_GreyChild, 0xCC, 0x04, Q12(1.0f), 4, 6, 1, -1, 1, 0, &D_800AD4C4 },
    { Q12(0.8f),  Q12(0.3f),      0x000F, 0, 0, 0, Chara_None, 0x00, 0x00, Q12(0.5f), 13, 7, 2, -1, 2, 0, &D_800AD4C4 },
    { Q12(1.2f),  Q12(0.2f),      0x0118, 102, 6, 0, Chara_GreyChild, 0xCC, 0x04, Q12(1.0f), 7, 4, 2, -1, 3, 0, &D_800AD4C4 },
    { Q12(1.0f),  Q12(0.1f),      0x00FA, 0, 8, 0, Chara_GreyChild, 0x66, 0x06, Q12(0.25f), 6, 5, 1, -1, 2, 0, &D_800AD4C4 },
    { Q12(1.2f),  Q12(0.1f),      0x03E8, 102, 6, 102, Chara_LarvalStalker, 0x66, 0x06, Q12(0.0f), 0, 6, 1, -1, 2, 0, &D_800AD4C4 },
    { Q12(0.4f),  Q12(0.4f),      0x00F0, -52, 4, 102, Chara_LarvalStalker, 0x00, 0x08, Q12(0.0f), 6, 3, 1, -1, 2, 0, &D_800AD4C4 },
    { Q12(0.5f),  Q12(0.0f),      0x0064, 51, 3, 204, Chara_Groaner, 0x66, 0x06, Q12(0.25f), 13, 7, 2, -1, 2, 0, &D_800AD4C4 },
    { Q12(0.1f),  Q12(0.0f),      0x0064, 51, 3, 204, Chara_Groaner, 0x66, 0x06, Q12(0.0f), 8, 5, 2, -1, 3, 0, &D_800AD4C4 },
    { Q12(0.5f),  Q12(0.2f),      0x0078, -52, 4, 204, Chara_Groaner, 0x33, 0x03, Q12(0.0f), 0, 4, 1, -1, 2, 0, &D_800AD4C4 },
    { Q12(1.0f),  Q12(15.9f) + 1, 0x005A, 102, 6, 51, Chara_NightFlutter, 0xCC, 0x04, Q12(1.0f), 12, 6, 2, -1, 4, 0, &D_800AD4C4 },
    { Q12(1.0f),  Q12(0.5f),      0x0014, 102, 6, 102, Chara_LarvalStalker, 0x66, 0x06, Q12(1.0f), 4, 6, 1, -1, 1, 0, &D_800AD4C4 },
    { Q12(0.8f),  Q12(0.3f),      0x000F, 0, 0, 0, Chara_None, 0x00, 0x00, Q12(0.5f), 13, 7, 2, -1, 3, 0, &D_800AD4C4 },
    { Q12(1.2f),  Q12(0.2f),      0x0168, 102, 6, 51, Chara_NightFlutter, 0xCC, 0x04, Q12(1.25f), 12, 6, 2, -1, 4, 0, &D_800AD4C4 },
    { Q12(1.0f),  Q12(0.1f),      0x00FA, 0, 8, 0, Chara_GreyChild, 0x66, 0x06, Q12(0.25f), 6, 9, 1, -1, 2, 0, &D_800AD4C4 },
    { Q12(1.2f),  Q12(0.1f),      0x03E8, 102, 6, 102, Chara_LarvalStalker, 0x66, 0x06, Q12(0.0f), 7, 6, 1, -1, 2, 0, &D_800AD4C4 },
    { Q12(0.4f),  Q12(0.4f),      0x010E, 102, 6, 102, Chara_LarvalStalker, 0x00, 0x08, Q12(0.0f), 6, 4, 1, -1, 2, 0, &D_800AD4C4 },
    { Q12(0.0f),  Q12(0.0f),      0x0000, 0, 0, 0, Chara_None, 0x00, 0x00, Q12(0.0f), 0, 0, 0, -1, 0, 0, &D_800AD4C4 },
    { Q12(0.0f),  Q12(0.0f),      0x0000, 0, 0, 0, Chara_None, 0x00, 0x00, Q12(0.0f), 0, 0, 0, -1, 0, 0, &D_800AD4C4 },
    { Q12(0.4f),  Q12(0.1f),      0x0082, 102, 6, 102, Chara_LarvalStalker, 0x33, 0x03, Q12(0.0f), 1, 4, 1, -1, 2, 0, &D_800AD4C4 },
    { Q12(1.0f),  Q12(15.9f) + 1, 0x0032, 102, 6, 0, Chara_GreyChild, 0xCC, 0x04, Q12(1.0f), 5, 3, 2, -1, 3, 0, &D_800AD4C4 },
    { Q12(1.0f),  Q12(0.5f),      0x0010, 102, 6, 102, Chara_LarvalStalker, 0x66, 0x06, Q12(0.75f), 4, 6, 1, -1, 1, 0, &D_800AD4C4 },
    { Q12(0.8f),  Q12(0.3f),      0x000C, 0, 0, 0, Chara_None, 0x00, 0x00, Q12(0.25f), 4, 6, 2, -1, 2, 0, &D_800AD4C4 },
    { Q12(1.2f),  Q12(0.2f),      0x00C8, 102, 6, 0, Chara_GreyChild, 0xCC, 0x04, Q12(1.0f), 4, 4, 2, -1, 3, 0, &D_800AD4C4 },
    { Q12(1.0f),  Q12(0.1f),      0x00FA, 0, 8, 0, Chara_GreyChild, 0x66, 0x06, Q12(0.25f), 4, 5, 1, -1, 2, 0, &D_800AD4C4 },
    { Q12(1.2f),  Q12(0.1f),      0x03E8, 102, 6, 102, Chara_LarvalStalker, 0x66, 0x06, Q12(0.0f), 2, 5, 1, -1, 2, 0, &D_800AD4C4 },
    { Q12(0.4f),  Q12(0.4f),      0x00A0, -52, 4, 102, Chara_LarvalStalker, 0x00, 0x08, Q12(0.0f), 2, 4, 1, -1, 2, 0, &D_800AD4C4 },
    { Q12(0.5f),  Q12(0.3f),      0x0001, 0, 0, 0, Chara_None, 0x00, 0x00, Q12(0.0f), 0, 5, 6, -1, 0, 0, &D_800AD4C4 },
    { Q12(0.5f),  Q12(0.3f),      0x0001, 0, 0, 0, Chara_None, 0x00, 0x00, Q12(0.0f), 0, 5, 6, -1, 0, 0, &D_800AD4C4 },
    { Q12(0.5f),  Q12(0.3f),      0x0001, 0, 0, 0, Chara_None, 0x00, 0x00, Q12(0.0f), 0, 5, 6, -1, 0, 0, &D_800AD4C4 },
    { Q12(0.0f),  Q12(0.0f),      0x0000, 0, 0, 0, Chara_None, 0x00, 0x00, Q12(0.0f), 0, 0, 0, 0, 0, 0, &D_800AD4C4 },
    { Q12(12.0f), Q12(0.0f),      0x0064, 0, 0, 0, Chara_Harry, 0x44, 0x00, Q12(0.0f), 0, 5, 3, 0, 3, 0, &D_800AD4C4 },
    { Q12(15.0f), Q12(0.0f),      0x015E, 0, 0, 0, Chara_Harry, 0x22, 0x00, Q12(0.5f), 0, 5, 3, 0, 3, 0, &D_800AD4C4 },
    { Q12(12.0f), Q12(0.0f),      0x0032, 0, 0, 0, Chara_Romper, 0xAA, 0x5C, Q12(0.25f), 0, 5, 3, 0, 2, 0, &D_800AD4C4 },
    { Q12(15.0f), Q12(0.0f),      0x0032, 0, 0, 0, Chara_Harry, 0x00, 0x00, Q12(0.0f), 0, 3, 3, 0, 2, 0, &D_800AD4C4 },
    { Q12(12.0f), Q12(0.0f),      0x0028, 51, 3, 0, Chara_Groaner, 0x93, 0x49, Q12(0.0f), 2, 5, 3, 0, 2, 0, &D_800AD4C4 },
    { Q12(0.0f),  Q12(0.0f),      0x0000, 0, 0, 0, Chara_Harry, 0x00, 0x00, Q12(0.0f), 0, 0, 0, 0, 0, 0, &D_800AD4C4 },
    { Q12(0.0f),  Q12(0.0f),      0x0000, 0, 0, 0, Chara_Harry, 0x00, 0x00, Q12(0.0f), 0, 0, 0, 0, 0, 0, &D_800AD4C4 },
    { Q12(0.0f),  Q12(0.0f),      0x0000, 0, 0, 1, Chara_Harry, 0x00, 0x00, Q12(0.0f), 0, 0, 0, 0, 0, 0, &D_800AD4C4 },
    { Q12(0.3f),  Q12(0.0f),      0x0016, 0, 0, 0, Chara_None, 0x00, 0x00, Q12(0.0f), 2, 7, 2, 5, 4, 0, &D_800AD4C4 },
    { Q12(0.3f),  Q12(0.0f),      0x0012, 0, 0, 0, Chara_None, 0x00, 0x00, Q12(0.0f), 2, 7, 2, 5, 4, 0, &D_800AD4C4 },
    { Q12(0.3f),  Q12(0.0f),      0x000A, 51, 3, 204, Chara_Groaner, 0x33, 0x03, Q12(0.0f), 10, 8, 2, 2, 3, 0, &D_800AD4C4 },
    { Q12(0.4f),  Q12(0.1f),      0x000A, -52, 4, 204, Chara_Groaner, 0x00, 0x00, Q12(0.25f), 8, 2, 2, 2, 2, 0, &D_800AD4C4 },
    { Q12(0.4f),  Q12(0.1f),      0x0010, 0, 0, 0, Chara_None, 0x00, 0x00, Q12(0.0f), 0, 99, 6, 5, 3, 0, &D_800AD4C4 },
    { Q12(1.3f),  Q12(0.4f),      0x0010, 102, 6, 51, Chara_NightFlutter, 0x00, 0x00, Q12(0.0f), 8, 7, 2, 2, 4, 0, &D_800AD4C4 },
    { Q12(0.0f),  Q12(0.0f),      0x0000, 0, 0, 0, Chara_None, 0x00, 0x00, Q12(0.0f), 0, 99, 2, 0, 0, 0, &D_800AD4C4 },
    { Q12(0.0f),  Q12(0.0f),      0x0000, 0, 0, 0, Chara_None, 0x00, 0x00, Q12(0.0f), 0, 99, 4, 0, 0, 0, &D_800AD4C4 },
    { Q12(0.9f),  Q12(0.7f),      0x000F, 51, 3, 51, Chara_NightFlutter, 0x00, 0x00, Q12(0.0f), 12, 3, 1, 2, 2, 0, &D_800AD4C4 },
    { Q12(0.6f),  Q12(0.3f),      0x0000, 102, 6, 102, Chara_LarvalStalker, 0x00, 0x00, Q12(0.0f), 11, 5, 5, 5, 0, 0, &D_800AD4C4 },
    { Q12(0.0f),  Q12(0.0f),      0x000C, 0, 0, 0, Chara_None, 0x00, 0x00, Q12(0.0f), 0, 99, 4, 0, 2, 0, &D_800AD4C4 },
    { Q12(0.4f),  Q12(0.0f),      0x0000, -52, 4, 204, Chara_Groaner, 0x00, 0x00, Q12(0.0f), 0, 12, 5, 0, 0, 0, &D_800AD4C4 },
    { Q12(0.5f),  Q12(0.0f),      0x0005, 0, 0, 0, Chara_None, 0x00, 0x00, Q12(0.0f), 0, 99, 4, 0, 2, 0, &D_800AD4C4 },
    { Q12(0.4f),  Q12(0.2f),      0x0006, -103, 1, 153, Chara_Harry, 0x00, 0x00, Q12(0.0f), 5, 2, 4, 5, 2, 0, &D_800AD4C4 },
    { Q12(0.9f),  Q12(0.4f),      0x0000, 102, 6, 153, Chara_Mumbler, 0x00, 0x00, Q12(0.0f), 3, 2, 5, 1, 0, 0, &D_800AD4C4 },
    { Q12(0.0f),  Q12(0.0f),      0x000A, 0, 0, 0, Chara_None, 0x00, 0x00, Q12(0.0f), 0, 99, 4, 2, 0, 0, &D_800AD4C4 },
    { Q12(0.0f),  Q12(0.0f),      0x0000, 51, 3, 51, Chara_NightFlutter, 0x33, 0x03, Q12(0.0f), 0, 99, 5, 5, 0, 0, &D_800AD4C4 },
    { Q12(0.0f),  Q12(0.0f),      0x0019, 51, 3, 51, Chara_NightFlutter, 0x33, 0x03, Q12(0.25f), 17, 5, 1, 2, 2, 0, &D_800AD4C4 },
    { Q12(0.0f),  Q12(0.0f),      0x0021, 51, 3, 51, Chara_NightFlutter, 0x33, 0x03, Q12(0.25f), 17, 5, 1, 2, 2, 0, &D_800AD4C4 },
    { Q12(0.2f),  Q12(0.0f),      0x0012, 51, 3, 51, Chara_NightFlutter, 0x00, 0x00, Q12(0.0f), 0, 99, 2, 2, 4, 0, &D_800AD4C4 },
    { Q12(0.0f),  Q12(0.0f),      0x0000, 0, 0, 0, Chara_None, 0x00, 0x00, Q12(0.0f), 0, 99, 6, 4, 0, 0, &D_800AD4C4 },
    { Q12(0.4f),  Q12(0.4f),      0x000A, -103, 9, 153, Chara_Mumbler, 0x99, 0x09, Q12(1.75f), 0, 99, 2, 5, 0, 0, &D_800AD4C4 },
    { Q12(0.0f),  Q12(0.0f),      0x0000, 0, 0, 0, Chara_None, 0x00, 0x00, Q12(0.0f), 0, 99, 6, 4, 0, 0, &D_800AD4C4 },
    { Q12(15.0f), Q12(0.0f),      0x003C, 0, 0, 0, Chara_Harry, 0x27, 0x00, Q12(0.25f), 1, 2, 3, 4, 3, 0, &D_800AD4C4 },
    { Q12(0.4f),  Q12(0.0f),      0x000F, 0, 0, 0, Chara_None, 0x00, 0x00, Q12(0.25f), 11, 3, 2, 2, 0, 0, &D_800AD4C4 },
    { Q12(0.5f),  Q12(0.0f),      0x0019, 0, 0, 0, Chara_None, 0x00, 0x00, Q12(0.25f), 11, 3, 2, 1, 0, 0, &D_800AD4C4 },
    { Q12(0.4f),  Q12(0.0f),      0x0001, 102, 6, 102, Chara_LarvalStalker, 0x66, 0x06, Q12(0.0f), 0, 99, 5, 5, 0, 0, &D_800AD4C4 },
    { Q12(0.0f),  Q12(0.0f),      0x0000, 0, 0, 0, Chara_None, 0x00, 0x00, Q12(0.0f), 0, 99, 6, 4, 0, 0, &D_800AD4C4 },
    { Q12(0.0f),  Q12(0.0f),      0x0000, 0, 0, 0, Chara_None, 0x00, 0x00, Q12(0.0f), 0, 99, 6, 1, 0, 0, &D_800AD4C4 },
    { Q12(0.1f),  Q12(0.0f),      0x0006, -103, 1, 153, Chara_Harry, 0x00, 0x00, Q12(0.0f), 0, 99, 6, 2, 2, 0, &D_800AD4C4 },
};

s32 g_Inventory_CmdSelectedIdx = 0;
u32 g_Inventory_SelectionId    = 0;

const char* INV_ITEM_NAMES[] = {
#if VERSION_REGION_IS(NTSCJ)
    "栄養剤",
    "携帯用救急キット",
    "アンプル",
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    "ロビーの鍵",
    "民家の鍵",
    "［ライオン］の鍵",
    "［きこり］の鍵",
    "［かかし］の鍵",
    "書庫の鍵",
    "教室の鍵",
    "Ｋ・ゴードンの鍵",
    "跳ね橋の鍵",
    "病院地下の鍵",
    "地下倉庫の鍵",
    "診察室の鍵",
    "骨董屋の鍵",
    "下水道通路の鍵",
    "［オフィエル］の鍵",
    "［ハギト］の鍵",
    "［ファレグ］の鍵",
    "［ベトール］の鍵",
    "［アラトロン］の鍵",
    "「学校へ」と書かれた紙片",
    "「犬小屋」と書かれた紙片",
    "絵札",
    "　",
    "下水道出口の鍵",
    "チャネリングストーン",
    NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    "化学薬品",
    "金のメダリオン",
    "銀のメダリオン",
    "ゴムボール",
    "“フラウロス”",
    "ペットボトル",
    "液体（詳細不明）",
    "［海亀］のプレート",
    "［帽子屋］のプレート",
    "［猫］のプレート",
    "［女王］のプレート",
    "輸血用血液パック",
    "消毒用アルコール",
    "ライター",
    "ビデオテープ",
    "　",
    "カウフマンの鍵",
    "領収証",
    "金庫の鍵",
    "磁石",
    "バイクの鍵",
    "鳥カゴの鍵",
    "ペンチ",
    "ドライバー",
    "カメラ",
    "契約の指輪",
    "時の石",
    "ソロモンの護符",
    "メルクリウスの紋章",
    "アンク",
    "メルキオールの小剣",
    "ウロボロスの円環",
    "調理用ナイフ",
    "鉄パイプ",
    "小型削岩機",
    "警棒",
    "ハンマー",
    "チェーンソー",
    "日本刀",
    "斧",
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,
    "ハンドガン",
    "狩猟用ライフル",
    "ショットガン",
    "ハイパーブラスター",
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    "ハンドガンの弾",
    "ライフルの弾",
    "ショットガンの弾",
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    "ライト",
    "ポケットラジオ",
    "ガソリン" 
#else
    "Health_drink(s)",
    "First_aid_kit(s)",
    "Ampoule(s)",
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    "Lobby_key",
    "House_key",
    "Key_of_[Lion]",
    "Key_of_[Woodman]",
    "Key_of_[Scarecrow]",
    "Library_reserve_key",
    "Classroom_key",
    "K._Gordon_key",
    "Drawbridge_key",
    "Basement_key",
    "Basement_storeroom_key",
    "Examination_room_key",
    "Antique_shop_key",
    "Sewer_key",
    "Key_of_[Ophiel]",
    "Key_of_[Hagith]",
    "Key_of_[Phaleg]",
    "Key_of_[Bethor]",
    "Key_of_[Aratron]",
    "A_note_[To_school]",
    "A_note_[Doghouse]",
    "Picture_card",
    " ",
    "Sewer_exit_key",
    "Channeling_stone",
    NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    "Chemical",
    "Gold_medallion",
    "Silver_medallion",
    "Rubber_ball",
    "[Flauros]",
    "Plastic_bottle",
    "Unknown_liquid",
    "Plate_of_[Turtle]",
    "Plate_of_[Hatter]",
    "Plate_of_[Cat]",
    "Plate_of_[Queen]",
    "Blood_pack",
    "Disinfecting_alcohol",
    "Lighter",
    "Video_tape",
    " ",
    "Kaufmann_key",
    "Receipt",
    "Safe_key",
    "Magnet",
    "Motorcycle_key",
    "Bird_cage_key",
    "Pliers",
    "Screwdriver",
    "Camera",
    "Ring_of_contract",
    "Stone_of_time",
    "Amulet_of_Solomon",
    "Crest_of_Mercury",
    "Ankh",
    "Dagger_of_Melchior",
    "Disk_of_Ouroboros",
    "Kitchen_knife",
    "Steel_pipe",
    "Rock_drill",
    " ",
    "Hammer",
    "Chainsaw",
    "Katana",
    "Axe",
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,
    "Handgun",
    "Hunting_rifle",
    "Shotgun",
    "Hyper_Blaster",
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    "Handgun_bullets",
    "Rifle_shells",
    "Shotgun_shells",
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    "Flashlight",
    "Pocket_radio",
    "Gasoline_tank"
#endif
};

const char* g_ItemDescriptions[] = {
#if VERSION_REGION_IS(NTSCJ)
    "栄養を補給し体力を回復させる~N効果は低め",
    "傷を治療し体力を回復させる~N効果は中程度",
    "痛みを鎮静化し体力を回復させる~N効果は高めで一定時間持続する",
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    "ロビーへの扉を開ける鍵。~Nロッカー室で見つけた。",
    "ある民家の正面玄関の鍵~N犬小屋の中に隠されていた",
    "民家の庭へ行く扉の鍵~Nパトカーのトランクの中にあった",
    "民家の庭へ行く扉の鍵~Nバスケットボールのコートに落ちていた",
    "民家の庭へ行く扉の鍵~N倒木を渡った先の郵便箱に置かれていた",
    "学校２階の書庫の鍵~Nロッカー室で拾った",
    "屋上の排水孔の奥にひっかかっていた~Nタグのついた学校の教室の鍵",
    "タグにＫ．ゴードンと記されている~N学校のボイラー室で拾った",
    "跳ね橋の開閉を制御する機械を~N作動させるための鍵　教会で手に入れた",
    "病院の地下へ行くための扉を開ける~N１階会議室のテーブルの上にあった",
    "手術室で手に入れた~N病院の地下にある倉庫の鍵",
    "病院地下２階の病室で見つけた~N診察室の扉を開ける鍵",
    "病院でダリアが置いていった鍵~Nタグに店の名前が書いてある",
    "下水道内の通路の扉を開けるための鍵",
    "［オフィエル］の文字が刻まれた鍵",
    "［ハギト］の文字が刻まれた鍵",
    "［ファレグ］の文字が刻まれた鍵",
    "［ベトール］の文字が刻まれた鍵",
    "［アラトロン］の文字が刻まれた鍵",
    "シェリルの字で「学校へ」と~N書かれたスケッチブックの紙片",
#if VERSION_EQUAL_OR_NEWER(JAP1)
    "シェリルの字で「ＬＥＶＩＮ通り」~N「犬小屋」と書かれたスケッチブックの紙片",
#else
    "シェリルの字で「ＬＥＶＩＮ通り」~N\n	「犬小屋」と書かれたスケッチブックの紙片",
#endif
    "鍵が描かれた長方形のカード~N教室の机の上に置かれていた",
    "　",
    "下水道でリゾート街への出口へ行くための~N扉を開ける鍵　水の中に落ちていた",
    "不思議な力を持った神秘の石~Nどこかで使うと何かが起きる……？",
    NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    "実験用具室で手に入れた　ラベルに~N［濃塩酸／取扱い注意］と書かれている",
    "手の形をした彫像に握られていた~N表面に時計台の絵が彫られている",
    "音楽室で拾った~N表面に時計台の絵が彫られている",
    "子どもが遊びに使う柔らかい~N蛍光ピンクのゴムのボール",
    "教会で手に入れた三角錐状のオブジェ~N使用方法・用途共に不明",
    "病院の厨房で手に入れたミネラル~Nウォーターの空のプラスチックボトル",
    "病院院長室の床で割れていた~Nガラス瓶から手に入れた赤い液体",
    "表面に「にせ海亀」が描かれた~N青色の四角いプレート",
    "表面に「帽子屋」が描かれた~N緑色の四角いプレート",
    "表面に「チェシャ猫」が描かれた~N黄色の四角いプレート",
    "表面に「ハートの女王」が描かれた~N赤色の四角いプレート",
    "病院倉庫で手に入れた~N２００ｍｌ入り血液のパック",
    "ボトル入りの消毒に使われるアルコール~N病院の集中治療室に置かれていた",
    "金属製のオイルライター~N２０１号室に置いてあった",
    "病院地下２階で見つけた~Nラベルには何も書いていない",
    "このアイテムは存在しない",
    "カウフマンが落としたと思われる~Nタグに「３」と書かれている",
    "カウフマンが落としたと思われる~N雑貨屋のレシート",
    "金庫を開けるための鍵~N雑貨屋インディアンランナーで見つけた",
    "細長いヒモが結ばれている棒磁石~Nモーテルの事務室に置いてあった",
    "古びているバイクの鍵~Nモーテルの客室の床下に落ちていた",
    "柄の部分に鳥カゴが彫られている",
    "金属部品・針金の加工に使われる道具~N錆びてはいるが使えないことはない",
    "ネジを回すのに使われる道具~N錆びてはいるが使えないことはない",
    "フラッシュ付きのカメラ",
    "材質不明の無骨で頑丈そうな指輪",
#if VERSION_EQUAL_OR_NEWER(JAP1)
    "星座の絵がかけられた柱にはまっていた~N時計の文字盤のような模様がある大きな石",
#else
    "星座の絵がかけられた柱にはまっていた~N\n	時計の文字盤のような模様がある大きな石",
#endif
    "古代の魔術師の名が冠された~N六つの頂点を持つ星型の護符",
    "水銀を意味する蛇をかたどった~N錬金術のシンボル",
    "楕円と十字からなる護符",
    "賢者の名前が冠された古びた小剣",
    "自らの尾をかむ蛇の円盤状の魔除け",
    "料理に使うナイフ~N扱いにくく攻撃力も低いがないよりはまし",
    "鉄製の長さ約１ｍのパイプ~N破壊力はいまいちだが攻撃範囲は広い",
    "道路工事などに使われる杭を打ちだし~N岩石に穴を開けるための機械",
    "シビルから受け取った警棒。",
    "災害時用の鉄製の重いハンマー~N破壊力は大きいが扱いが難しい",
    "樹木の伐採などに使われる動力付きのこぎり~N攻撃力は極めて高い",
    "日本の伝統的な片刃の長剣",
    "片手で扱える小型の斧~N攻撃力は中程度",
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,
    "シビルに渡された小型の拳銃~N最大装填数１５発",
    "狩猟用の大型の銃~N最大装填数６発",
    "広い範囲を攻撃出来る銃~N最大装填数６発",
    "赤外線照準装置付きの高性能拳銃~N弾数無制限",
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    "ハンドガンに用いられる弾",
    "ライフルに用いられる弾",
    "ショットガンに用いられる弾",
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    "ワンタッチでＯＮ／ＯＦＦの操作が可能~N胸ポケットに入れてある",
    "携帯用の小型ラジオ~N怪物が近付くとノイズが鳴り出す",
    "チェーンソーや削岩機を動かすために必要~Nガソリンスタンドのガレージに置いてあった"
#else
    "Supplies_nutrition_to\n\t\t\t\t\trecover_a_small_amount_of\n\t\t\t\t\tstamina.",
    "Heals_injury_to_provide\n\t\t\t\t\tmoderate_stamina_recovery.",
    "Relieve_pain_to_recover\n\t\t\t\t\tstamina_to_high.\n\t\t\t\t\tEffect_lasts_for_a_while.",
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    "The_key_to_open\n\t\t\t\t\tthe_door_to_the_lobby.\n\t\t\t\t\tI_found_it_in_the_locker_room.",
    "The_house_key\n\t\t\t\t\thidden_in_the_dog_house.",
    "The_key_to_the_garden_of\n\t\t\t\t\tthe_house._Found_in_patrol\n\t\t\t\t\tcar's_trunk.",
    "The_key_to_the_garden_of\n\t\t\t\t\tthe_house._Found_in_the\n\t\t\t\t\tbasketball_court.",
    "The_key_to_the_garden_of\n\t\t\t\t\tthe_house._Found_in_mailbox\n\t\t\t\t\tblocked_by_a_fallen_tree.",
    "The_key_to_the_school's_2nd\n\t\t\t\t\tfloor_library_reserves.\n\t\t\t\t\tPicked_up_in_the_locker_room.",
    "The_tagged_classroom_key\n\t\t\t\t\tfound_hanging_in_the_drainpipe\n\t\t\t\t\ton_the_roof.",
    "The_tag_says,_[K._Gordon;]\n\t\t\t\t\tfound_in_the_school's\n\t\t\t\t\tboiler_room.",
    "Key_to_operate_the_drawbridge.\n\t\t\t\t\tFound_in_the_church.",
    "The_hospital_basement\n\t\t\t\t\tdoor_key,_found_on_the_table\n\t\t\t\t\tin_1st_floor_conference_room.",
    "The_hospital_basement\n\t\t\t\t\tstoreroom_key_found\n\t\t\t\t\tin_the_operating_room.",
    "Found_in_patient_room\n\t\t\t\t\tin_the_2nd_floor_basement.",
    "The_tagged_key_with_a_shop\n\t\t\t\t\tname_on_it_that_Dahlia_left\n\t\t\t\t\tat_the_hospital.",
    "Sewer_passageway_door_key.",
    "The_word_[Ophiel]_is\n\t\t\t\t\tengraved_on_the_key.",
    "The_word_[Hagith]_is\n\t\t\t\t\tengraved_on_the_key.",
    "The_word_[Phaleg]_is\n\t\t\t\t\tengraved_on_the_key.",
    "The_word_[Bethor]_is\n\t\t\t\t\tengraved_on_the_key.",
    "The_word_[Aratron]_is\n\t\t\t\t\tengraved_on_the_key.",
    "[To_school]_is_written\n\t\t\t\t\tin_Cheryl's_handwriting.",
    "[Doghouse]_and_[Levin_st.]\n\t\t\t\t\tare_written_in\n\t\t\t\t\tCheryl's_handwriting.",
    "A_rectangular_card_with\n\t\t\t\t\ta_picture_of_a_key._Found_on\n\t\t\t\t\ta_desk_in_a_classroom.",
    " ",
    "Key_to_exit_from_the_sewer\n\t\t\t\t\tto_the_resort_area,_which_was\n\t\t\t\t\tdropped_in_the_water.",
    "A_mystical_stone_possessing\n\t\t\t\t\tpower._If_used_somewhere,\n\t\t\t\t\tthe_result_will_be...?",
    NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    "The_label_reads,_[Concentrated\n\t\t\t\t\thydrochloric_acid.]_Found_in\n\t\t\t\t\tthe_lab_equipment_room.",
    "A_picture_of_a_clock_tower\n\t\t\t\t\tis_engraved_on_the_surface.\n\t\t\t\t\tFound_in_the_chemistry_lab.",
    "A_picture_of_a_clock_tower\n\t\t\t\t\tis_engraved_on_the_surface.\n\t\t\t\t\tFound_in_the_music_room.",
    "Florescent_pink_rubber_ball\n\t\t\t\t\tused_by_children.",
    "Pyramid_shaped_object_found\n\t\t\t\t\tin_the_church.\n\t\t\t\t\tPurpose_and_use_unknown.",
    "Empty_bottle_found\n\t\t\t\t\tin_the_kitchen\n\t\t\t\t\tof_the_hospital.",
    "Red_liquid_found_in\n\t\t\t\t\ta_broken_vial_in_the\n\t\t\t\t\tHospital_Director's_room.",
    "Square,_blue_plate\n\t\t\t\t\twith_a_picture_of\n\t\t\t\t\ta_[Mock_Turtle.]",
    "Square,_green_plate\n\t\t\t\t\twith_a_picture_of\n\t\t\t\t\ta_[Mad_Hatter.]",
    "Square,_yellow_plate\n\t\t\t\t\twith_a_picture_of\n\t\t\t\t\ta_[Cheshire_Cat.]",
    "Square,_red_plate\n\t\t\t\t\twith_a_picture_of\n\t\t\t\t\ta_[Queen_of_Hearts.]",
    "200ml_blood_pack.\n\t\t\t\t\tFound_in_the_hospital\n\t\t\t\t\tstorage_room.",
    "Isopropyl_alcohol_bottle,\n\t\t\t\t\tfound_in_ICU_at_the_hospital.",
    "Oil_lighter_found_in_Room_201.",
    "Nothing_on_the_label.\n\t\t\t\t\tFound_on_2nd_floor\n\t\t\t\t\thospital_basement.",
    " ",
    "A_[3]_is_written_on_the_tag.\n\t\t\t\t\tThe_key_was_thought_to_have\n\t\t\t\t\tbeen_dropped_by_Kaufmann.",
    "Receipt_from_General_Store,\n\t\t\t\t\tthought_to_have_been_dropped\n\t\t\t\t\tby_Kaufmann.",
    "Key_to_the_safe,\n\t\t\t\t\tfound_at_Indian_Runner\n\t\t\t\t\tGeneral_Store.",
    "Long,_thin_magnetic_rod\n\t\t\t\t\twith_a_string_attached,\n\t\t\t\t\tin_the_motel_office.",
    "Key_to_an_old_motorcycle,\n\t\t\t\t\twhich_was_dropped_between_the\n\t\t\t\t\tfloorboards_in_the_motel_room.",
    "Bird_cage_key_with\n\t\t\t\t\tengraved_handle.",
    "Tool_to_use_on\n\t\t\t\t\tmetal_and_wires.\n\t\t\t\t\tRusted_but_usable.",
    "Tool_for_screws.\n\t\t\t\t\tRusted,_but_usable.",
    "Camera_with_flash.",
    "Rustic,_hefty-looking_ring\n\t\t\t\t\tof_unknown_material.",
    "Large_stone_with_clock-face\n\t\t\t\t\tdesign_on_pillar_with_astrology\n\t\t\t\t\tchart_hanging_from_it.",
    "The_amulet_is_a_star_shaped\n\t\t\t\t\tobject_with_6_corners_named\n\t\t\t\t\tafter_the_ancient_magician.",
    "Snake_in_the_symbol_of\n\t\t\t\t\tmercury_signifies_alchemy.",
    "Elliptical_talisman\n\t\t\t\t\twith_a_cross_inside.",
    "The_ancient_dagger\n\t\t\t\t\tnamed_after_the_Magi.",
    "Ring_of_a_snake_biting\n\t\t\t\t\tit's_own_tail.",
    "Hard_to_use,\n\t\t\t\t\tbut_better_than_nothing.",
    "3'_long_steel_pipe.\n\t\t\t\t\tLong_range_but_of_limited_use.",
    "A_tool_used_in_road_construction\n\t\t\t\t\tto_create_holes_in_the_road_for\n\t\t\t\t\tpoles.",
    " ",
    "Emergency_hammer.\n\t\t\t\t\tHighly_damaging,\n\t\t\t\t\tbut_hard_to_use.",
    "Power_tool_used_for_cutting\n\t\t\t\t\tdown_trees,_etc.,_highly_deadly.",
    "Traditional_single_edged\n\t\t\t\t\tJapanese_blade.",
    "Small_one_hand_axe.\n\t\t\t\t\tModerately_effective\n\t\t\t\t\tas_a_weapon.",
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,
    "Handgun_received_from_Cybil.\n\t\t\t\t\tHolds_up_to_15_rounds.",
    "Holds_up_to_6_rounds.",
    "Fires_in_a_wide_radius.\n\t\t\t\t\tHolds_up_to_6_rounds.",
    "High-powered_handgun\n\t\t\t\t\twith_infrared_scope\n\t\t\t\t\tand_unlimited_bullets.",
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    "Ammo_for_the_handgun.",
    "Ammo_for_the_hunting_rifle.",
    "Ammo_for_the_shotgun.",
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    "One_touch_on/off_switch.\n\t\t\t\t\tIn_the_chest_pocket.",
    "Portable_radio_that\n\t\t\t\t\temits_static\n\t\t\t\t\twhen_monsters_are_near.",
    "Fuel_for_chainsaws_and\n\t\t\t\t\trock_drills."
#endif
};

// TODO: `Items_` subsystem globals and funcs could be part of `Inventory_` instead, not sure yet.

s32 D_800AE178                                 = 0;
s32 g_Inventory_SelectedItemIdx                = 0;
s32 g_Items_DisplayedCount                     = 0;
u8  g_Inventory_EquippedItem                   = 0;
u8  D_800AE185                                 = 0;
u8  D_800AE186                                 = 0;
u8  D_800AE187                                 = InvItemId_Unequipped;  /** `e_InvItemId` | Related to displaying items. */
u32 D_800AE188                                 = 0;
u16 D_800AE18C                                 = 0; // } Used by JAP0-only code.
u16 D_800AE18E                                 = 0; // }
s32 D_800AE190                                 = 0;
s16 g_Inventory_HealthStatusScanlineTimer      = 0;
s16 g_Inventory_HealthStatusColorGradientTimer = 0;
s16 D_800AE198                                 = 0;
s16 g_Inventory_HealthStatusScanlinePosition   = -300;
s32 g_Inventory_ItemNameTimer                  = 0;
s32 g_Inventory_DescriptionRollTimer           = 0;
s32 g_Inventory_ScrollTransitionTimer          = 0;
s16 D_800AE1A8                                 = 0;

// 2 bytes of padding.

s32    g_Items_PickupAnimState = 0;
q19_12 g_Items_PickupScale     = Q12(0.0f);

void Gfx_ItemScreens_DrawInit(u32* selectedItemId) // 0x8004F764
{
    #define LABEL_COUNT 8

    GsDOBJ2* curObj;
    s32      i;

    DVECTOR LABEL_STR_POS_TABLE[LABEL_COUNT] = {
#if VERSION_REGION_IS(NTSCJ)
        { 118, -80 },
        { 144, 288 },
        { 36, 288 },
        { 240, 288 },
        { 218, -80 },
        { 40, -80 },
        { 16, 184 },
        { 16, 200 }
#elif VERSION_REGION_IS(NTSC)
        { 118, -80 },
        { 144, 304 },
        { 36, 304 },
        { 240, 304 },
        { 218, -80 },
        { 40, -80 },
        { 16, 184 },
        { 16, 200 }
#endif
    };

    char* LABEL_STRS[LABEL_COUNT] = {
        "Equipment",
        "Exit",
        "Option",
        "Map",
        "Command",
        "Status",
        "No.",
        "Name:"
    };

    Gfx_StringColorSet(StringColorId_White);

    // In inventory.
    if (g_GameWork.gameStateSteps[1] < 21)
    {
        for (i = 0; i < ARRAY_SIZE(LABEL_STR_POS_TABLE); i++)
        {
            Gfx_StringPositionSet(LABEL_STR_POS_TABLE[i].vx, LABEL_STR_POS_TABLE[i].vy);
            Gfx_StringDraw(LABEL_STRS[i], 10);
        }

        Inventory_PlayerItemScroll(selectedItemId);

        // Player items.
        for (i = 0, curObj = &g_Items_ItemsModelData[0]; i < 7; i++, curObj++)
        {
            if (D_800C3E18[i] == NO_VALUE)
            {
                continue;
            }
                if (g_SavegamePtr->items[D_800C3E18[i]].id == (u8)InvItemId_Empty)
                {
                    continue;
                }

            g_Items_Transforms[i].rotate.vx = INV_ITEM_ROTATIONS[g_SavegamePtr->items[D_800C3E18[i]].id - 32].vx;
            g_Items_Transforms[i].rotate.vz = INV_ITEM_ROTATIONS[g_SavegamePtr->items[D_800C3E18[i]].id - 32].vy;

            ItemScreen_ItemRotate(&g_Items_Coords[i].param->rotate, &g_Items_Coords[i]);
            func_800548D8(i);
            GsSetFlatLight(0, &g_Items_Lights[i][0]);
            GsSetFlatLight(1, &g_Items_Lights[i][1]);
            func_8004BD74(i, curObj, 0);
        }

        // Equipped item.
        if (g_SavegamePtr->items[g_Inventory_EquippedItemIdx].id != (u8)InvItemId_Empty && 
            g_Inventory_EquippedItemIdx != NO_VALUE)
        {
            g_Items_Transforms[7].rotate.vx = INV_ITEM_ROTATIONS[g_SavegamePtr->items[g_Inventory_EquippedItemIdx].id - 32].vx;
            g_Items_Transforms[7].rotate.vz = INV_ITEM_ROTATIONS[g_SavegamePtr->items[g_Inventory_EquippedItemIdx].id - 32].vy;

            ItemScreen_ItemRotate(&g_Items_Coords[7].param->rotate, &g_Items_Coords[7]);
            func_800548D8(7);
            GsSetFlatLight(0, &D_800C3A88[0]);
            GsSetFlatLight(1, &D_800C3A88[1]);
            func_8004BD74(7, curObj, 0);
        }

        Gfx_Inventory_ItemDescriptionDraw(selectedItemId);
        Gfx_Inventory_2dBackgroundDraw(selectedItemId);
    }
    else
    {
        Gfx_Results_ItemsPosition(g_SavegamePtr->clearGameCount);
        Gfx_Results_ItemsDisplay(g_SavegamePtr->clearGameCount);
        Ranking_DisplayInfo(selectedItemId); // TODO: Takes no args in `credits.c`?
    }

    func_8004FB0C();
}

void func_8004FB0C(void) // 0x8004FB0C
{
    GsOT*    ot;
    POLY_F4* poly;

    ot   = &g_OrderingTable2[g_ActiveBufferIdx];
    poly = (POLY_F4*)GsOUT_PACKET_P;

    setPolyF4(poly);
    setRGB0(poly, Q8_COLOR(0.0f), Q8_COLOR(0.0f), Q8_COLOR(0.0f));
    setXY4(poly,
           -160, -224,
           -160, -222,
            160, -224,
            160, -222);
    addPrim(ot->org, poly);

    GsOUT_PACKET_P = (PACKET*)poly + sizeof(POLY_F4);
}

/** Draws many 2D menu elements.
 * The background behind the text in the upper
 * and lower part of the inventory screen, the
 * box used to indicate what the user has selected,
 * and triggers the drawing of the health status.
 */
void Gfx_Inventory_2dBackgroundDraw(s32* arg0) // 0x8004FBCC
{
    s32       temp_t3;
    s32       temp_v1;
    s32       k;
    s32       i;
    s32       j;
    s32       var_t5;
    s32       var_t6;
    s32       new_var;
    s32       spA0;
    GsOT*     ot0;
    GsOT*     ot1;
    POLY_G4*  poly_g4;
    POLY_FT4* poly_ft4;
    LINE_G2*  line_g2;

    s_Inventory_SelectionOutlineVector selectionOutlineInner[] = {
        { { -32, -52 }, { 64, 128 } },
        { { -48, -200 }, { 96, 144 } },
    #if VERSION_REGION_IS(NTSCJ)
        { { -48, 169 }, { 96, 24 } },
        { { -144, 169 }, { 96, 24 } },
        { { 48, 169 }, { 96, 24 } },
    #elif VERSION_REGION_IS(NTSC)
        { { -48, 185 }, { 96, 24 } },
        { { -144, 185 }, { 96, 24 } },
        { { 48, 185 }, { 96, 24 } },
    #endif
        { { 48, -200 }, { 96, 128 } },
        { { 48, -200 }, { 96, 128 } },
        { { -144, -200 }, { 96, 156 } },
        { { -48, -200 }, { 192, 256 } }
    };

    s_Inventory_SelectionOutlineVector selectionOutlineCorner[] = {
        { { -34, -56 }, { 34, 80 } },
        { { -50, -204 }, { 50, -52 } },
    #if VERSION_REGION_IS(NTSCJ)
        { { -50, 165 }, { 50, 197 } },
        { { -146, 165 }, { -46, 197 } },
        { { 46, 165 }, { 146, 197 } },
    #elif VERSION_REGION_IS(NTSC)
        { { -50, 181 }, { 50, 213 } },
        { { -146, 181 }, { -46, 213 } },
        { { 46, 181 }, { 146, 213 } },
    #endif
        { { 46, -204 }, { 146, -68 } },
        { { 45, -204 }, { 146, -68 } },
        { { -146, -204 }, { -46, -40 } },
        { { -50, -204 }, { 146, 60 } }
    };

    ot1 = &g_OrderingTable2[g_ActiveBufferIdx];
    ot0 = &g_OrderingTable0[g_ActiveBufferIdx];

    temp_v1 = g_SysWork.gameStateCounter & 0x7F; // `% 128`

    if (temp_v1 < 0x40)
    {
        spA0 = temp_v1;
    }
    else
    {
        spA0 = 0x7F - temp_v1;
    }

    Gfx_Inventory_HealthStatusDraw();
    if (g_Inventory_SelectionBordersDraw == 8)
    {
        switch (*arg0)
        {
            case 1:
                var_t6 = 0;
                var_t5 = 1;
                break;

            case 2:
                var_t6 = 1;
                var_t5 = 1;
                break;

            case 3:
                var_t6 = 1;
                var_t5 = 0;
                break;

            case 4:
                var_t6 = 1;
                var_t5 = 2;
                break;

            case 5:
            case 6:
                var_t6 = 0;
                var_t5 = 2;
                break;

            case 7:
                var_t6 = 0;
                var_t5 = 0;
                break;

            default:
                var_t5 = NO_VALUE;
                var_t6 = NO_VALUE;
                break;
        }
    }
    else
    {
        var_t5 = NO_VALUE;
        var_t6 = NO_VALUE;
    }

    if (g_GameWork.gameStateSteps[1] != 20)
    {
        for (i = 0; i < 2; i++)
        {
            for (j = 0; j < 3; j++)
            {
                for (k = 0; k < 2; k++)
                {
                    poly_g4 = (POLY_G4*)GsOUT_PACKET_P;
                    setPolyG4(poly_g4);

                    if (k != 0)
                    {
                        switch (g_SavegamePtr->gameDifficulty)
                        {
                            case GameDifficulty_Easy:
                                if (i == var_t6 && j == var_t5)
                                {
                                    setRGB0(poly_g4, spA0 - 0x80, 0xC0, spA0 * 3);
                                    setRGB2(poly_g4, spA0 - 0x80, 0xC0, spA0 * 3);
                                }
                                else
                                {
                                    setRGB0(poly_g4, 0x80, 0xC0, 0);
                                    setRGB2(poly_g4, 0x80, 0xC0, 0);
                                }
                                break;

                            case GameDifficulty_Normal:
                                if (i == var_t6 && j == var_t5)
                                {
                                    setRGB0(poly_g4, spA0 * 3, spA0 - 0x80, 0xC0);
                                    setRGB2(poly_g4, spA0 * 3, spA0 - 0x80, 0xC0);
                                }
                                else
                                {
                                    setRGB0(poly_g4, 0, 0x80, 0xC0);
                                    setRGB2(poly_g4, 0, 0x80, 0xC0);
                                }
                                break;

                            case GameDifficulty_Hard:
                                if (i == var_t6 && j == var_t5)
                                {
                                    setRGB0(poly_g4, spA0 - 0x80, spA0 * 3, 0xC0);
                                    setRGB2(poly_g4, spA0 - 0x80, spA0 * 3, 0xC0);
                                }
                                else
                                {
                                    setRGB0(poly_g4, 0x80, 0, 0xC0);
                                    setRGB2(poly_g4, 0x80, 0, 0xC0);
                                }
                                break;
                        }

                        setRGB1(poly_g4, 0, 0, 0);
                        setRGB3(poly_g4, 0, 0, 0);
                    }
                    else
                    {
                        switch (g_SavegamePtr->gameDifficulty)
                        {
                            case GameDifficulty_Easy:
                                if (i == var_t6 && j == var_t5)
                                {
                                    setRGB1(poly_g4, spA0 - 0x80, 0xC0, spA0 * 3);
                                    setRGB3(poly_g4, spA0 - 0x80, 0xC0, spA0 * 3);
                                }
                                else
                                {
                                    setRGB1(poly_g4, 0x80, 0xC0, 0);
                                    setRGB3(poly_g4, 0x80, 0xC0, 0);
                                }
                                break;

                            case GameDifficulty_Normal:
                                if (i == var_t6 && j == var_t5)
                                {
                                    setRGB1(poly_g4, spA0 * 3, spA0 - 0x80, 0xC0);
                                    setRGB3(poly_g4, spA0 * 3, spA0 - 0x80, 0xC0);
                                }
                                else
                                {
                                    setRGB1(poly_g4, 0, 0x80, 0xC0);
                                    setRGB3(poly_g4, 0, 0x80, 0xC0);
                                }
                                break;

                            case GameDifficulty_Hard:
                                if (i == var_t6 && j == var_t5)
                                {
                                    setRGB1(poly_g4, spA0 - 0x80, spA0 * 3, 0xC0);
                                    setRGB3(poly_g4, spA0 - 0x80, spA0 * 3, 0xC0);
                                }
                                else
                                {
                                    setRGB1(poly_g4, 0x80, 0, 0xC0);
                                    setRGB3(poly_g4, 0x80, 0, 0xC0);
                                }
                                break;
                        }

                        setRGB0(poly_g4, 0, 0, 0);
                        setRGB2(poly_g4, 0, 0, 0);
                    }

                    if (i != 0)
                    {
                        setXY4(poly_g4,
#if VERSION_REGION_IS(NTSCJ)
                               -0x8C + (0x60 * j), 0xB0 + (6 * k),
                               -0x8C + (0x60 * j), 0xB6 + (6 * k),
                               -0x34 + (0x60 * j), 0xB0 + (6 * k),
                               -0x34 + (0x60 * j), 0xB6 + (6 * k));
#elif VERSION_REGION_IS(NTSC)
                               -0x8C + (0x60 * j), 0xC0 + (6 * k),
                               -0x8C + (0x60 * j), 0xC6 + (6 * k),
                               -0x34 + (0x60 * j), 0xC0 + (6 * k),
                               -0x34 + (0x60 * j), 0xC6 + (6 * k));
#endif
                    }
                    else
                    {
                        setXY4(poly_g4,
                               -0x8C + (0x60 * j), -0xC0 + (6 * k),
                               -0x8C + (0x60 * j), -0xBA + (6 * k),
                               -0x34 + (0x60 * j), -0xC0 + (6 * k),
                               -0x34 + (0x60 * j), -0xBA + (6 * k));
                    }

                    addPrim(&ot1->org[7], poly_g4);
                    GsOUT_PACKET_P = (PACKET*)poly_g4 + sizeof(POLY_G4);
                }
            }
        }
    }

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 4; j++)
        {
            D_800C3B68[3 - i][j].vx = D_800C3B68[2 - i][j].vx;
            D_800C3B68[3 - i][j].vy = D_800C3B68[2 - i][j].vy;
        }
    }

    temp_t3   = (Math_Cos(g_Inventory_SelectionBordersDraw << 7) * Math_Cos(g_Inventory_SelectionBordersDraw << 7) * 16) >> 16;

    D_800C3B68[0][0].vx = selectionOutlineInner[*arg0].field_0.vx +
    FP_FROM((new_var = selectionOutlineInner[g_Inventory_PrevSelectionId].field_0.vx - selectionOutlineInner[*arg0].field_0.vx) * temp_t3, Q12_SHIFT);

    D_800C3B68[0][0].vy = selectionOutlineInner[*arg0].field_0.vy +
        FP_FROM((selectionOutlineInner[g_Inventory_PrevSelectionId].field_0.vy - selectionOutlineInner[*arg0].field_0.vy) * temp_t3, Q12_SHIFT);

    D_800C3B68[0][1].vx = D_800C3B68[0][0].vx;

    D_800C3B68[0][1].vy = selectionOutlineInner[*arg0].field_0.vy + selectionOutlineInner[*arg0].field_4.vy +
        FP_FROM(((selectionOutlineInner[g_Inventory_PrevSelectionId].field_0.vy + selectionOutlineInner[g_Inventory_PrevSelectionId].field_4.vy) -
        (selectionOutlineInner[*arg0].field_0.vy + selectionOutlineInner[*arg0].field_4.vy)) * temp_t3, Q12_SHIFT);

    D_800C3B68[0][2].vx = selectionOutlineInner[*arg0].field_0.vx + selectionOutlineInner[*arg0].field_4.vx +
        FP_FROM(((selectionOutlineInner[g_Inventory_PrevSelectionId].field_0.vx + selectionOutlineInner[g_Inventory_PrevSelectionId].field_4.vx) -
        (selectionOutlineInner[*arg0].field_0.vx + selectionOutlineInner[*arg0].field_4.vx)) * temp_t3, Q12_SHIFT);
    D_800C3B68[0][3].vx = D_800C3B68[0][2].vx;
    D_800C3B68[0][2].vy = D_800C3B68[0][1].vy;
    D_800C3B68[0][3].vy = D_800C3B68[0][0].vy;

    if (*arg0 == 8 || g_GameWork.gameStateSteps[1] == 0xF)
    {
        poly_g4 = (POLY_G4*)GsOUT_PACKET_P;

        setPolyG4(poly_g4);
        setSemiTrans(poly_g4, true);

        if (D_800AE190 < 0x21)
        {
            setRGB0(poly_g4,
                    (D_800AE190 >= 16) ? 0xFE : (D_800AE190 * 16),
                    (D_800AE190 >= 16) ? 0xFE : (D_800AE190 * 16),
                    (D_800AE190 >= 16) ? 0xFE : (D_800AE190 * 16));
            setRGB1(poly_g4,
                    (D_800AE190 >= 32) ? 0xFE : (D_800AE190 * 8),
                    (D_800AE190 >= 32) ? 0xFE : (D_800AE190 * 8),
                    (D_800AE190 >= 32) ? 0xFE : (D_800AE190 * 8));
            setRGB2(poly_g4,
                    (D_800AE190 >= 8) ? 0xFE : (D_800AE190 * 32),
                    (D_800AE190 >= 8) ? 0xFE : (D_800AE190 * 32),
                    (D_800AE190 >= 8) ? 0xFE : (D_800AE190 * 32));
            setRGB3(poly_g4,
                    (D_800AE190 >= 16) ? 0xFE : (D_800AE190 * 16),
                    (D_800AE190 >= 16) ? 0xFE : (D_800AE190 * 16),
                    (D_800AE190 >= 16) ? 0xFE : (D_800AE190 * 16));
        }
        else
        {
            setRGB0(poly_g4,
                    (D_800AE190 < 0x30) ? ((0x30 - D_800AE190) * 16) : 0,
                    (D_800AE190 < 0x30) ? ((0x30 - D_800AE190) * 16) : 0,
                    (D_800AE190 < 0x30) ? ((0x30 - D_800AE190) * 16) : 0);
            setRGB1(poly_g4,
                    (D_800AE190 < 0x40) ? ((0x40 - D_800AE190) * 8) : 0,
                    (D_800AE190 < 0x40) ? ((0x40 - D_800AE190) * 8) : 0,
                    (D_800AE190 < 0x40) ? ((0x40 - D_800AE190) * 8) : 0);
            setRGB2(poly_g4,
                    (D_800AE190 < 0x28) ? ((0x28 - D_800AE190) * 32) : 0,
                    (D_800AE190 < 0x28) ? ((0x28 - D_800AE190) * 32) : 0,
                    (D_800AE190 < 0x28) ? ((0x28 - D_800AE190) * 32) : 0);
            setRGB3(poly_g4,
                    (D_800AE190 < 0x30) ? ((0x30 - D_800AE190) * 16) : 0,
                    (D_800AE190 < 0x30) ? ((0x30 - D_800AE190) * 16) : 0,
                    (D_800AE190 < 0x30) ? ((0x30 - D_800AE190) * 16) : 0);
        }

        setXY4(poly_g4,
               D_800C3B68[3][0].vx + 1, D_800C3B68[3][0].vy + 2,
               D_800C3B68[3][1].vx + 1, D_800C3B68[3][1].vy,
               D_800C3B68[3][3].vx,     D_800C3B68[3][3].vy + 2,
               D_800C3B68[3][3].vx,     D_800C3B68[3][1].vy);

        addPrim(&ot1->org[5], poly_g4);
        GsOUT_PACKET_P = (PACKET*)poly_g4 + sizeof(POLY_G4);

        Gfx_Primitive2dTextureSet(0, 0, 5, 2);

        if ((D_800AE190 >= 0x21 && *arg0 == 8) || (D_800AE190 < 0x21 && g_GameWork.gameStateSteps[1] == 0xF))
        {
            for (i = 0; i < 2; i++)
            {
                poly_ft4 = (POLY_FT4*)GsOUT_PACKET_P;
                setPolyFT4(poly_ft4);
                poly_ft4->clut = getClut(g_ItemInspectionImg.clutX, g_ItemInspectionImg.clutY);
                setRGB0(poly_ft4, 0x80, 0x80, 0x80);

                if (i != 0)
                {
                    poly_ft4->tpage = 134;
                    setUV4(poly_ft4,
                           0x20, 0x10,
                           0x20, 0xFF,
                           0xC0, 0x10,
                           0xC0, 0xFF);
                    setXY4(poly_ft4,
                           (u16)(D_800C3B68[3][0].vx + (((-(D_800C3B68[3][0].vx + 4) + D_800C3B68[3][3].vx) >> 1) + 1)), D_800C3B68[3][0].vy + 2,
                           (u16)(D_800C3B68[3][1].vx + (((-(D_800C3B68[3][1].vx + 4) + D_800C3B68[3][3].vx) >> 1) + 1)), D_800C3B68[3][1].vy,
                           D_800C3B68[3][3].vx,                                                                          D_800C3B68[3][3].vy + 2,
                           D_800C3B68[3][3].vx,                                                                          D_800C3B68[3][1].vy);
                }
                else
                {
                    poly_ft4->tpage = 0x85;
                    setUV4(poly_ft4,
                           0,    0x10,
                           0,    0xFF,
                           0xA0, 0x10,
                           0xA0, 0xFF);
                    setXY4(poly_ft4,
                           D_800C3B68[3][0].vx + 1,                                                                      D_800C3B68[3][0].vy + 2,
                           D_800C3B68[3][1].vx + 1,                                                                      D_800C3B68[3][1].vy,
                           (u16)(D_800C3B68[3][0].vx + ((-(D_800C3B68[3][0].vx + 4) + (D_800C3B68[3][3].vx) >> 1) + 2)), D_800C3B68[3][3].vy + 2,
                           (u16)(D_800C3B68[3][1].vx + ((-(D_800C3B68[3][1].vx + 4) + (D_800C3B68[3][3].vx) >> 1) + 2)), D_800C3B68[3][1].vy);
                }

                addPrim(&ot1->org[5], poly_ft4);
                GsOUT_PACKET_P = (PACKET*)poly_ft4 + sizeof(POLY_FT4);
            }
        }
    }

    for (j = 0; j < 4; j++)
    {
        for (i = 0; i < 4; i++)
        {
            line_g2 = (LINE_G2*)GsOUT_PACKET_P;
            setLineG2(line_g2);

            setRGB0(line_g2, 0x60 - (0x20 * j), 0x60 - (0x20 * j), 0xFF);
            setRGB1(line_g2, 0x60 - (0x20 * j), 0x60 - (0x20 * j), 0xFF);

            new_var = (u16)D_800C3B68[j][i].vx;

            setXY2(line_g2,
                   new_var, D_800C3B68[j][i].vy,
                   D_800C3B68[j][(i + 1) & 0x3].vx, D_800C3B68[j][(i + 1) & 0x3].vy);

            addPrim(&ot0->org[7], line_g2);
            GsOUT_PACKET_P = (PACKET*)line_g2 + sizeof(LINE_G2);
        }

        for (i = 1; i < 4; i += 2)
        {
            line_g2 = (LINE_G2*)GsOUT_PACKET_P;
            setLineG2(line_g2);

            setRGB0(line_g2, 0x60 - (0x20 * j), 0x60 - (0x20 * j), 0xFF);
            setRGB1(line_g2, 0x60 - (0x20 * j), 0x60 - (0x20 * j), 0xFF);
            setXY2(line_g2,
                   D_800C3B68[j][i].vx,             D_800C3B68[j][i].vy + 1,
                   D_800C3B68[j][(i + 1) & 0x3].vx, D_800C3B68[j][(i + 1) & 0x3].vy + 1);

            addPrim(&ot1->org[7], line_g2);
            GsOUT_PACKET_P = (PACKET*)line_g2 + sizeof(LINE_G2);
        }
    }

    if (g_Inventory_SelectionBordersDraw != 8)
    {
        return;
    }

    for (i = 0; i < 6; i++)
    {
        line_g2 = (LINE_G2*)GsOUT_PACKET_P;
        setLineG2(line_g2);

        setRGB0(line_g2, -0x50 - spA0, -0x50 - spA0, 0xFF);
        setRGB1(line_g2, spA0 + 0x70, spA0 + 0x70, 0xFF);

        if (i / 3)
        {
            setXY2(line_g2, selectionOutlineCorner[*arg0].field_4.vx,
                   (i == 4) ? (selectionOutlineCorner[*arg0].field_4.vy - 1) : selectionOutlineCorner[*arg0].field_4.vy,
                   (i != 5) ? (selectionOutlineCorner[*arg0].field_4.vx - (selectionOutlineInner[*arg0].field_4.vx >> 1)) : selectionOutlineCorner[*arg0].field_4.vx,
                   (i == 5) ? (selectionOutlineCorner[*arg0].field_4.vy - (selectionOutlineInner[*arg0].field_4.vy >> 1)) :
                   (i == 4) ? (selectionOutlineCorner[*arg0].field_4.vy - 1) : selectionOutlineCorner[*arg0].field_4.vy);
        }
        else
        {
            setXY2(line_g2, selectionOutlineCorner[*arg0].field_0.vx,
                   (i == 1) ? (selectionOutlineCorner[*arg0].field_0.vy - 1) : selectionOutlineCorner[*arg0].field_0.vy,
                   (i != 2) ? (selectionOutlineCorner[*arg0].field_0.vx + (selectionOutlineInner[*arg0].field_4.vx >> 1)) : selectionOutlineCorner[*arg0].field_0.vx,
                   (i == 2) ? (selectionOutlineCorner[*arg0].field_0.vy + (selectionOutlineInner[*arg0].field_4.vy >> 1)) :
                   (i == 1) ? (selectionOutlineCorner[*arg0].field_0.vy - 1) : selectionOutlineCorner[*arg0].field_0.vy);
        }

        addPrim(&ot0->org[7], line_g2);
        GsOUT_PACKET_P = (PACKET*)line_g2 + sizeof(LINE_G2);
    }
}

static inline s16 GetUvOrRandom(void)
{
    if (D_800AE198 == 1)
    {
        D_800AE1A8 = Rng_GenerateInt(0, 133);
        return D_800AE1A8;
    }
    else
    {
        return D_800AE1A8 & 0xFF;
    }
}

void Gfx_Inventory_HealthStatusDraw(void) // 0x80051020
{
    s32       tempOverlayY;
    s16       overlayYPos;
    s32       health;
    s32       healthSegment;
    s16       healthStage;
    s32       i;
    u8        greenVal;
    u8        redVal;
    GsOT*     ot;
    POLY_FT4* poly_ft4;
    POLY_G4*  poly_g4;
    DR_TPAGE* tPage;

    u8 D_80027F04[] = {
        0xFF, 0x00, 0x00, 0x00,
        0xFF, 0xA0, 0x00, 0x00,
        0xA0, 0xFF, 0x00, 0x00,
        0x00, 0xFF, 0x00, 0x00
    };

    ot     = &g_OrderingTable0[g_ActiveBufferIdx];
    health = g_SysWork.playerWork.player.health;

    if (health < Q12(10.0f))
    {
        healthStage = 3;
    }
    else if (health < Q12(50.0f))
    {
        healthStage = 2;
    }
    else
    {
        healthStage = 1;
    }

    healthStage                          += g_Inventory_HealthStatusScanlineTimer;
    g_Inventory_HealthStatusScanlineTimer = healthStage % 164;

    for (i = 0; i < 3; i++)
    {
        if (i != 1 || g_Inventory_HealthStatusScanlineTimer >= 30)
        {
            if (i == 2)
            {
                if (g_SysWork.playerWork.player.health != Q12(100.0f) &&
                    ((Rng_Rand16() % ((g_SysWork.playerWork.player.health >> 13) + 2) == 0) || D_800AE198 != 0))
                {
                    D_800AE198++;

                    if (D_800AE198 == 4)
                    {
                        D_800AE198 = 0;
                    }
                }
                else
                {
                    continue;
                }
            }

            poly_ft4 = (POLY_FT4*)GsOUT_PACKET_P;
            setPolyFT4(poly_ft4);
            poly_ft4->tpage = 159;
            poly_ft4->clut  = getClut(g_HealthPortraitImg.clutX, g_HealthPortraitImg.clutY);
            setRGB0(poly_ft4, 0x80, 0x80, 0x80);

            switch (i)
            {
                case 0:
                    setUV4(poly_ft4,
                           40, 103,
                           40, 237,
                           124, 103,
                           124, 237);
                    setXY4(poly_ft4,
                           -138, -184,
                           -138, -50,
                           -54, -184,
                           -54, -50);
                    break;

                case 1:
                    setUV4(poly_ft4,
                           40,  g_Inventory_HealthStatusScanlineTimer + 73,
                           40,  g_Inventory_HealthStatusScanlineTimer + 75,
                           124, g_Inventory_HealthStatusScanlineTimer + 73,
                           124, g_Inventory_HealthStatusScanlineTimer + 75);
                    setXY4(poly_ft4,
                           -136, g_Inventory_HealthStatusScanlineTimer - 214,
                           -136, g_Inventory_HealthStatusScanlineTimer - 212,
                           -52,  g_Inventory_HealthStatusScanlineTimer - 214,
                           -52,  g_Inventory_HealthStatusScanlineTimer - 212);

                    if (g_Inventory_HealthStatusColorGradientTimer == 0)
                    {
                        g_Inventory_HealthStatusScanlinePosition = poly_ft4->y0;
                    }
                    break;

                case 2:
                    setUV4(poly_ft4,
                           40,  GetUvOrRandom() + 103,
                           40,  D_800AE1A8 + 105,
                           124, D_800AE1A8 + 103,
                           124, D_800AE1A8 + 105);
                    setXY4(poly_ft4,
                           -136, D_800AE1A8 - 184,
                           -136, D_800AE1A8 - 182,
                           -52,  D_800AE1A8 - 184,
                           -52,  D_800AE1A8 - 182);
                    break;
            }

            addPrim((u8*)ot->org - (-8156 + (i * 4)), poly_ft4);
            GsOUT_PACKET_P = (PACKET*)poly_ft4 + sizeof(POLY_FT4);
        }
    }

    poly_g4 = (POLY_G4*)GsOUT_PACKET_P;

    setPolyG4(poly_g4);
    setSemiTrans(poly_g4, true);
    setRGB0(poly_g4, 255, 255, 255);
    setRGB1(poly_g4, 0, 0, 0);
    setRGB2(poly_g4, 255, 255, 255);
    setRGB3(poly_g4, 0, 0, 0);
    setXY4(poly_g4,
           -136, g_Inventory_HealthStatusScanlineTimer - 212,
           -136, g_Inventory_HealthStatusScanlineTimer - 182,
           -54,  g_Inventory_HealthStatusScanlineTimer - 212,
           -54,  g_Inventory_HealthStatusScanlineTimer - 182);

    tempOverlayY = poly_g4->y0 - 2;

    if (tempOverlayY >= -214)
    {
        if (tempOverlayY >= (g_Inventory_HealthStatusScanlinePosition - (g_Inventory_HealthStatusColorGradientTimer >> 1)))
        {
            overlayYPos = tempOverlayY;
        }
        else
        {
            overlayYPos = -52;
        }
    }
    else
    {
        overlayYPos = -52;
    }

    addPrim(&ot->org[2036], poly_g4);
    GsOUT_PACKET_P = (PACKET*)poly_g4 + sizeof(POLY_G4);

    tPage = (DR_TPAGE*)GsOUT_PACKET_P;
    setDrawTPage(tPage, 0, 1, getTPageN(0, 2, 0, 0));
    addPrim(&ot->org[2036], tPage);
    GsOUT_PACKET_P = (PACKET*)tPage + sizeof(DR_TPAGE);

    if (g_Inventory_HealthStatusScanlinePosition != -300)
    {
        healthSegment = FP_FROM(g_SysWork.playerWork.player.health, Q12_SHIFT);

        if (healthSegment < 39)
        {
            redVal   = D_80027F04[0];
            greenVal = D_80027F04[1];
        }
        else if (healthSegment < 0x41)
        {
            redVal   = D_80027F04[4];
            greenVal = D_80027F04[5];
        }
        else if (healthSegment < 0x55)
        {
            redVal   = D_80027F04[8];
            greenVal = D_80027F04[9];
        }
        else
        {
            redVal   = D_80027F04[12];
            greenVal = D_80027F04[13];
        }

        for (i = 0; i < 2; i++)
        {
            poly_g4 = (POLY_G4*)GsOUT_PACKET_P;

            setPolyG4(poly_g4);
            setRGB0(poly_g4,
                    redVal   - ((redVal   * g_Inventory_HealthStatusColorGradientTimer) >> 6),
                    greenVal - ((greenVal * g_Inventory_HealthStatusColorGradientTimer) >> 6),
                    0);
            setRGB1(poly_g4, 0, 0, 0);
            setRGB2(poly_g4,
                    redVal   - ((redVal   * g_Inventory_HealthStatusColorGradientTimer) >> 6),
                    greenVal - ((greenVal * g_Inventory_HealthStatusColorGradientTimer) >> 6),
                    0);
            setRGB3(poly_g4, 0, 0, 0);

            if (i != 0)
            {
                setXY4(poly_g4,
                    -136, ((g_Inventory_HealthStatusScanlinePosition - (g_Inventory_HealthStatusColorGradientTimer >> 1)) < -182) ?
                          -182 :
                          (g_Inventory_HealthStatusScanlinePosition - (g_Inventory_HealthStatusColorGradientTimer >> 1)),
                    -136, poly_g4->y0 - 16,
                    -56,  ((g_Inventory_HealthStatusScanlinePosition - (g_Inventory_HealthStatusColorGradientTimer >> 1)) < -182) ?
                          -182 :
                          (g_Inventory_HealthStatusScanlinePosition - (g_Inventory_HealthStatusColorGradientTimer >> 1)),
                    -56,  poly_g4->y0 - 16);
            }
            else
            {
                setXY4(poly_g4,
                    -136, ((g_Inventory_HealthStatusScanlinePosition - (g_Inventory_HealthStatusColorGradientTimer >> 1)) < -182) ?
                          -182 :
                          (g_Inventory_HealthStatusScanlinePosition - (g_Inventory_HealthStatusColorGradientTimer >> 1)),
                    -136, overlayYPos,
                    -56,  ((g_Inventory_HealthStatusScanlinePosition - (g_Inventory_HealthStatusColorGradientTimer >> 1)) < -182) ?
                          -182 :
                          (g_Inventory_HealthStatusScanlinePosition - (g_Inventory_HealthStatusColorGradientTimer >> 1)),
                    -56,  overlayYPos);
            }

            addPrim(&ot->org[2040], poly_g4);
            GsOUT_PACKET_P = (PACKET*)poly_g4 + sizeof(POLY_G4);
        }
    }

    if (g_SysWork.playerWork.player.health < Q12(10.0f))
    {
        healthStage = 3;
    }
    else if (g_SysWork.playerWork.player.health < Q12(50.0f))
    {
        healthStage = 2;
    }
    else
    {
        healthStage = 1;
    }

    g_Inventory_HealthStatusColorGradientTimer = healthStage + g_Inventory_HealthStatusColorGradientTimer;

    if (g_Inventory_HealthStatusColorGradientTimer >= 64)
    {
        g_Inventory_HealthStatusColorGradientTimer = 0;
        g_Inventory_HealthStatusScanlinePosition   = -300;
    }
}

#if VERSION_REGION_IS(NTSCJ)
INCLUDE_ASM("bodyprog/nonmatchings/items/item_screens_3", Gfx_Inventory_ItemDescriptionDraw);
#else
void Gfx_Inventory_ItemDescriptionDraw(s32* selectedItemId) // 0x8005192C
{
    s32 temp;
    s32 i;
    s32 idx;

    DVECTOR stringPos = { 208, 200 };

    char* D_80027F14[] = {
        "Can't_use_it_here.",
        "Too_dark_to_look_at\n\t\tthe_item_here."
    };

    char* D_80027F94[] = {
        "Stock:",
        "==On==",
        "==Off==",
        "==Use_OK==",
        "==Use_OK?==",
        "==Use_NG==",
        "Fuel:"
    };

    switch (*selectedItemId)
    {
        case 0:
        case 5:
        case 7:
        case 8:
            idx = g_SysWork.invItemSelectedIdx;
            break;

        default:
            idx = g_SysWork.playerCombat.weaponInventoryIdx;
            break;
    }

    if (D_800AE185 != INV_ITEM_GROUP(g_SavegamePtr->items[idx].id) - 1 ||
        D_800AE186 != INV_ITEM_GROUP_ID(g_SavegamePtr->items[idx].id))
    {
        SysWork_StateStepSet(1, 0);
        g_Inventory_ItemNameTimer        = 0;
        g_Inventory_DescriptionRollTimer = 0;
        D_800AE18C                       = 0;
        D_800AE18E                       = 0;
        D_800AE178                       = 0;
    }

    if (idx != NO_VALUE && *selectedItemId != 2 && *selectedItemId != 3 && *selectedItemId != 4)
    {
        for (i = 0; i < g_SavegamePtr->invSlotCount; i++)
        {
            if (i == idx)
            {
                if (idx + 1 >= 10)
                {
                    Gfx_StringPositionSet(45, 184);
                }
                else
                {
                    Gfx_StringPositionSet(55, 184);
                }

                Gfx_StringDrawInt(2, i + 1);
                i = g_SavegamePtr->invSlotCount;
            }
        }
    }

    if (INV_ITEM_GROUP(g_Inventory_EquippedItem) == InvItemGroup_GunWeapons &&
        g_Inventory_EquippedItem != InvItemId_HyperBlaster)
    {
        Gfx_StringPositionSet(122, 30);
        Gfx_StringDraw(D_80027F94[0], 10);

        for (i = 0; i < g_SavegamePtr->invSlotCount; i++)
        {
            if (g_Inventory_EquippedItem == g_SavegamePtr->items[i].id)
            {
                if (g_SavegamePtr->items[i].count >= 10)
                {
                    Gfx_StringPositionSet(178, 30);
                }
                else
                {
                    Gfx_StringPositionSet(188, 30);
                }
                Gfx_StringDrawInt(2, g_SavegamePtr->items[i].count);
            }
        }
    }

    temp = *selectedItemId;

    if (temp < 0 || (temp >= 2 && (*selectedItemId >= 9 || temp < 5)) ||
        g_SavegamePtr->items[idx].id == (u8)InvItemId_Empty)
    {
        return;
    }

    switch (g_SavegamePtr->items[idx].id)
    {
        case InvItemId_Flashlight:
            Gfx_StringPositionSet(stringPos.vx, stringPos.vy);
            if (!Game_FlashlightIsOn())
            {
                Gfx_StringDraw(D_80027F94[2], 10);
            }
            else
            {
                Gfx_StringDraw(D_80027F94[1], 10);
            }
            break;

        case InvItemId_PocketRadio:
            Gfx_StringPositionSet(stringPos.vx, stringPos.vy);
            if (g_SavegamePtr->itemToggleFlags & ItemToggleFlag_RadioOn)
            {
                Gfx_StringDraw(D_80027F94[1], 10);
            }
            else
            {
                Gfx_StringDraw(D_80027F94[2], 10);
            }
            break;

        case InvItemId_HyperBlaster:
            Gfx_StringPositionSet(stringPos.vx - 16, stringPos.vy);
            switch (Inventory_HyperBlasterFunctionalTest())
            {
                case 2: // If Hyper Blaster connected (Port 1) or player has unlocked it.
                    Gfx_StringDraw(D_80027F94[3], 10);
                    break;

                case 1: // If Hyper Blaster connected (Port 2).
                    Gfx_StringDraw(D_80027F94[4], 10);
                    break;

                case 0: // If Hyper Blaster is in inventory, but needs to be unlocked by finishing the game.
                    Gfx_StringDraw(D_80027F94[5], 10);
                    break;
            }
            break;

        case InvItemId_HealthDrink:
        case InvItemId_FirstAidKit:
        case InvItemId_Ampoule:
        case InvItemId_Handgun:
        case InvItemId_HuntingRifle:
        case InvItemId_Shotgun:
        case InvItemId_HandgunBullets:
        case InvItemId_RifleShells:
        case InvItemId_ShotgunShells:
            Gfx_StringPositionSet(stringPos.vx, stringPos.vy);
            Gfx_StringDraw(D_80027F94[0], 10);
            if (g_SavegamePtr->items[idx].id != (u8)InvItemId_Empty)
            {
                if (g_SavegamePtr->items[idx].count >= 100)
                {
                    Gfx_StringPositionSet(260, 200);
                }
                else if (g_SavegamePtr->items[idx].count >= 10)
                {
                    Gfx_StringPositionSet(270, 200);
                }
                else
                {
                    Gfx_StringPositionSet(280, 200);
                }
                Gfx_StringDrawInt(3, g_SavegamePtr->items[idx].count);
            }
            break;
    }

    Gfx_StringColorSet(StringColorId_White);

    D_800AE185 = INV_ITEM_GROUP(g_SavegamePtr->items[idx].id) - 1;
    D_800AE186 = INV_ITEM_GROUP_ID(g_SavegamePtr->items[idx].id);

    switch (g_SysWork.sysStateSteps[1])
    {
        case 0:
            g_Inventory_ItemNameTimer++;
            Gfx_StringPositionSet(68, 200);
            if (Gfx_StringDraw(INV_ITEM_NAMES[g_SavegamePtr->items[idx].id - 32], g_Inventory_ItemNameTimer) == true)
            {
                g_Inventory_ItemNameTimer   = 100;
                SysWork_StateStepIncrement(1);
            }
            break;

        case 1:
            g_Inventory_DescriptionRollTimer += 2;
            Gfx_StringPositionSet(68, 200);
            Gfx_StringDraw(INV_ITEM_NAMES[g_SavegamePtr->items[idx].id - 32], 100);

            if (idx == g_Inventory_SelectedItemIdx)
            {
                g_Inventory_ItemNameTimer = 0;
            }

            Gfx_StringPositionSet(30, 232);

            if (Gfx_StringDraw(g_ItemDescriptions[g_SavegamePtr->items[idx].id - 32], g_Inventory_DescriptionRollTimer) == true)
            {
                g_Inventory_DescriptionRollTimer = 100;
                SysWork_StateStepIncrement(1);
            }
            break;

        case 2:
        case 3:
        case 4:
            Gfx_StringPositionSet(68, 200);
            Gfx_StringDraw(INV_ITEM_NAMES[g_SavegamePtr->items[idx].id - 32], 100);
            Gfx_StringPositionSet(30, 232);

            switch (g_SysWork.sysStateSteps[1])
            {
                case 2:
                    Gfx_StringDraw(g_ItemDescriptions[g_SavegamePtr->items[idx].id - 32], 100);
                    break;

                case 3:
                    Gfx_StringDraw(D_80027F14[0], 100);
                    break;

                case 4:
                    Gfx_StringDraw(D_80027F14[1], 100);
                    break;
            }

            if (idx == g_Inventory_SelectedItemIdx)
            {
                g_Inventory_ItemNameTimer = 0;
            }
            break;
    }
}
#endif

#if VERSION_REGION_IS(NTSCJ)
INCLUDE_RODATA("bodyprog/nonmatchings/items/item_screens_3", D_80027C6C);

INCLUDE_RODATA("bodyprog/nonmatchings/items/item_screens_3", D_80027C74);

INCLUDE_RODATA("bodyprog/nonmatchings/items/item_screens_3", D_80027C7C);

INCLUDE_RODATA("bodyprog/nonmatchings/items/item_screens_3", D_80027C84);

INCLUDE_RODATA("bodyprog/nonmatchings/items/item_screens_3", D_80027C90);

INCLUDE_RODATA("bodyprog/nonmatchings/items/item_screens_3", D_80027C9C);

INCLUDE_RODATA("bodyprog/nonmatchings/items/item_screens_3", D_80027CA8);

INCLUDE_RODATA("bodyprog/nonmatchings/items/item_screens_3", D_80027CB0);

void func_80053898(s32 arg0, s32 arg1) // JPN0 0x80053898
{
    GsOT* ot;
    SPRT* sprt;

    ot   = &g_OrderingTable2[g_ActiveBufferIdx];
    sprt = GsOUT_PACKET_P;

    setSprt(sprt);

    setRGBC0(sprt, 128, 128, 128, PRIM_RECT | RECT_TEXTURE);
    *(u32*)&sprt->w = (arg0 * 12) + (16 << 16); // setWHFast(sprt, (arg0 * 12), 16);
    setXY0Fast(sprt, -92, 88);

    if (arg1 & 1)
    {
        setUV0AndClutSum(sprt, 0, 224, 0x7F93);
    }
    else
    {
        setUV0AndClutSum(sprt, 0, 16, 0x7F93);
    }

    addPrim(&ot->org[6], sprt);

    GsOUT_PACKET_P = &sprt[1];

    Gfx_Primitive2dTextureSet(0, (arg1 & 0x1) ? 256 : 0, 6, 1);
}

void func_800539A0(s32 arg0, s32 arg1) // JPN0 0x800539A0
{
    GsOT* ot;
    SPRT* sprt;

    ot   = &g_OrderingTable2[g_ActiveBufferIdx];
    sprt = GsOUT_PACKET_P;

    setSprt(sprt);

    setRGBC0(sprt, 128, 128, 128, PRIM_RECT | RECT_TEXTURE);

    if (arg0 != 0)
    {
        *(u32*)&sprt->w = (D_800AE18E * 12) + (16 << 16); // setWHFast(sprt, (D_800AE18E * 12), 16);
    }
    else
    {
        *(u32*)&sprt->w = (D_800AE18C * 12) + (16 << 16); // setWHFast(sprt, (D_800AE18C * 12), 16);
    }

    setXY0Fast(sprt, -120, 116 + (arg0 * 20));

    if (arg1 & 1)
    {
        setUV0AndClutSum(sprt, 0, 224, 0x7F93);
    }
    else
    {
        setUV0AndClutSum(sprt, 0, 16, 0x7F93);
    }

    addPrim(&ot->org[6], sprt);

    GsOUT_PACKET_P = &sprt[1];

    Gfx_Primitive2dTextureSet(64 + (arg0 << 6), (arg1 & 1) ? 256 : 0, 6, 1);
}
#endif

void Gfx_Primitive2dTextureSet(s32 x, s32 y, s32 otIdx, s32 abr) // 0x80052088
{
    GsOT*     ot0;
    GsOT*     ot1;
    s32       idx   = g_ActiveBufferIdx;
    DR_TPAGE* tPage = (DR_TPAGE*)GsOUT_PACKET_P;

    setDrawTPage(tPage, 0, 1, getTPage(0, abr, x, y));

    ot1 = &g_OrderingTable2[idx];
    ot0 = &g_OrderingTable0[idx];

    if (abr < 4)
    {
        addPrim(&ot1->org[otIdx], tPage);
    }
    else
    {
        addPrim(&ot0->org[otIdx], tPage);
    }

    GsOUT_PACKET_P = (PACKET*)tPage + sizeof(DR_TPAGE);
}

void Gfx_Results_ItemsDisplay() // 0x800521A8
{
    GsDOBJ2* ptr;
    s32      i;

    for (i = 0, ptr = g_Items_ItemsModelData; i < 6; i++, ptr++)
    {
        if ((D_800C3E40 >> i) & (1 << 0))
        {
            ItemScreen_ItemRotate(&g_Items_Coords[i].param->rotate, &g_Items_Coords[i]);
            func_800548D8(i);
            GsSetFlatLight(0, &g_Items_Lights[i][0]);
            GsSetFlatLight(1, &g_Items_Lights[i][1]);
            func_8004BD74(i, ptr, 3);
        }
    }
}

void Gfx_Results_ItemsPosition() // 0x8005227C
{
    #define DISPLAY_ITEM_COUNT 6

    s32 i;

    const SVECTOR OFFSETS[DISPLAY_ITEM_COUNT] = {
        { Q8(0.0f),      Q8(-3.25f), Q8(20.0f), Q8(0.0f) },
        { Q8(-3.03125f), Q8(-1.5f),  Q8(20.0f), Q8(0.0f) },
        { Q8(3.03125f),  Q8(-1.5f),  Q8(20.0f), Q8(0.0f) },
        { Q8(-3.03125f), Q8(2.0f),   Q8(20.0f), Q8(0.0f) },
        { Q8(3.03125f),  Q8(2.0f),   Q8(20.0f), Q8(0.0f) },
        { Q8(0.0f),      Q8(3.75f),  Q8(20.0f), Q8(0.0f) }
    };

    if (g_SavegamePtr->clearGameCount == 0)
    {
        g_SavegamePtr->clearGameCount = 1;
    }

    for (i = 0; i < DISPLAY_ITEM_COUNT; i++)
    {
        if ((D_800C3E40 >> i) & (1 << 0))
        {
            g_Items_Coords[i].coord.t[0]     = OFFSETS[i].vx;
            g_Items_Coords[i].coord.t[1]     = OFFSETS[i].vy;
            g_Items_Coords[i].coord.t[2]     = OFFSETS[i].vz;
            g_Items_Transforms[i].rotate.vx  = Q12_ANGLE(45.0f);
            g_Items_Transforms[i].rotate.vz  = Q12_ANGLE(45.0f);
            g_Items_Transforms[i].scale.vz   = Q12(1.0f);
            g_Items_Transforms[i].scale.vy   = Q12(1.0f);
            g_Items_Transforms[i].scale.vx   = Q12(1.0f);
            g_Items_Transforms[i].rotate.vy += Q12_ANGLE(0.75f);
        }
    }
}

void Inventory_PlayerItemScroll(u32* selectedItemId) // 0x800523D8
{
    s32          curWeaponAmmo;
    s32          totalWeaponAmmo;
    s32          someCount;
    s32          temp_hi;
    s32          temp_ret;
    s32          temp_ret_2;
    s32          temp_ret_3;
    s32          i;
    s32          j;
    s32          k;
    s32          l;
    s32          var_s3;
    s32          var_t3;
    s32          temp;
    s32          temp2;
    s32          temp3;
    s32          temp4;
    s32          temp5;
    register s32 v0 asm("v0"); // @hack
    s_GameWork*  ptr;
    s_GameWork*  ptr2;
    s32*         ptr3;

    switch (g_GameWork.gameStateSteps[1])
    {
        case 1:
            for (i = 0; i < 7; i++)
            {
                g_Items_Transforms[i].scale.vz = Q12(1.0f);
                g_Items_Transforms[i].scale.vy = Q12(1.0f);
                g_Items_Transforms[i].scale.vx = Q12(1.0f);
            }

            if (g_SysWork.invItemSelectedIdx == g_Inventory_SelectedItemIdx)
            {
                for (i = 0; i < 7; i++)
                {
                    if (D_800C3E18[i] != -1)
                    {
                        if (D_800C3E18[i] == g_SysWork.invItemSelectedIdx)
                        {
                            g_Items_Transforms[i].scale.vz = Q12(1.0f);
                            g_Items_Transforms[i].scale.vy = Q12(1.0f);
                            g_Items_Transforms[i].scale.vx = Q12(1.0f);
                            g_Items_Coords[i].coord.t[1]   = Q8(0.25f);
                            g_Items_Coords[i].coord.t[0]   = Q8(0.0f);
                            g_Items_Coords[i].coord.t[2]   = Q8(-4.0f);
                        }
                        else
                        {
                            if (ABS(D_800C3E18[i] - g_SysWork.invItemSelectedIdx) >= (g_SavegamePtr->invSlotCount >> 1))
                            {
                                if (g_SysWork.invItemSelectedIdx < D_800C3E18[i])
                                {
                                    var_s3 = (D_800C3E18[i] - g_SysWork.invItemSelectedIdx) - g_SavegamePtr->invSlotCount;
                                }
                                else
                                {
                                    var_s3 = g_SavegamePtr->invSlotCount - (g_SysWork.invItemSelectedIdx - D_800C3E18[i]);
                                }
                            }
                            else
                            {
                                var_s3 = D_800C3E18[i] - g_SysWork.invItemSelectedIdx;
                            }

                            temp_ret = Math_Sin((var_s3 << 10) / 3);
                            temp_hi  = temp_ret / 3;

                            g_Items_Coords[i].coord.t[1] = Q8(0.25f);
                            g_Items_Coords[i].coord.t[0] = temp_hi;

                            g_Items_Coords[i].coord.t[2]    = ABS(Math_Sin((var_s3 << 10) >> 2)) - Q8(4.0f);
                            g_Items_Transforms[i].rotate.vy = -(var_s3 << 10) / 3;
                        }
                    }
                }
            }
            else
            {
                g_Inventory_ScrollTransitionTimer++;

                if ((g_SysWork.invItemSelectedIdx == (g_Inventory_SelectedItemIdx + 1)) ||
                    ((g_Inventory_SelectedItemIdx - g_SysWork.invItemSelectedIdx) == (g_SavegamePtr->invSlotCount - 1)))
                {
                    g_Inventory_ScrollTransitionTimer = -g_Inventory_ScrollTransitionTimer;
                }

                if (ABS((s32)g_Inventory_ScrollTransitionTimer) == 8)
                {
                    g_Inventory_ScrollTransitionTimer = 0;
                    g_Inventory_SelectedItemIdx       = g_SysWork.invItemSelectedIdx;
                }

                for (i = 0; i < 7; i++)
                {
                    if (D_800C3E18[i] != NO_VALUE)
                    {
                        if (ABS(D_800C3E18[i] - g_Inventory_SelectedItemIdx) >= g_SavegamePtr->invSlotCount >> 1)
                        {
                            if (D_800C3E18[i] == g_Inventory_SelectedItemIdx)
                            {
                                var_s3 = 0;
                            }
                            else if (g_Inventory_SelectedItemIdx < D_800C3E18[i])
                            {
                                var_s3 = (D_800C3E18[i] - g_Inventory_SelectedItemIdx) - g_SavegamePtr->invSlotCount;
                            }
                            else
                            {
                                var_s3 = g_SavegamePtr->invSlotCount - (g_Inventory_SelectedItemIdx - D_800C3E18[i]);
                            }
                        }
                        else
                        {
                            var_s3 = D_800C3E18[i] - g_Inventory_SelectedItemIdx;
                        }

                        if (g_Inventory_ScrollTransitionTimer != 0)
                        {
                            temp_ret_2 = Math_Sin(((var_s3 << 10) + (g_Inventory_ScrollTransitionTimer << 7)) / 3);
                            temp_hi    = temp_ret_2 / 3;

                            g_Items_Coords[i].coord.t[1] = Q8(0.25f);
                            g_Items_Coords[i].coord.t[0] = temp_hi;

                            g_Items_Coords[i].coord.t[2]    = ABS(Math_Sin(((var_s3 << 10) + (g_Inventory_ScrollTransitionTimer << 7)) >> 2)) - Q8(4.0f);
                            g_Items_Transforms[i].rotate.vy = -(((var_s3 << 10) / 3) + ((g_Inventory_ScrollTransitionTimer << 7) / 3));
                        }
                        else
                        {
                            if (D_800C3E18[i] == g_Inventory_SelectedItemIdx)
                            {
                                g_Items_Coords[i].coord.t[1] = Q8(0.25f);
                                g_Items_Coords[i].coord.t[0] = Q8(0.0f);
                                g_Items_Coords[i].coord.t[2] = Q8(-4.0f);
                            }
                            else
                            {
                                temp_ret_3 = Math_Sin((var_s3 << 10) / 3);
                                temp_hi    = temp_ret_3 / 3;

                                g_Items_Coords[i].coord.t[1] = Q8(0.25f);
                                g_Items_Coords[i].coord.t[0] = temp_hi;

                                g_Items_Coords[i].coord.t[2]    = ABS(Math_Sin((var_s3 << 10) / 4)) - Q8(4.0f);
                                g_Items_Transforms[i].rotate.vy = -(var_s3 << 10) / 3;
                            }
                        }

                        if (var_s3 < -3 || var_s3 > 3)
                        {
                            g_Items_Coords[i].coord.t[0] = Q8(CHUNK_SIZE);
                        }
                    }
                }
                g_Inventory_ScrollTransitionTimer = ABS(g_Inventory_ScrollTransitionTimer);
            }
            break;

        case 5: // Equip weapon.
            if (g_Inventory_ScrollTransitionTimer == 0)
            {
                g_SysWork.playerCombat.weaponAttack       = g_SavegamePtr->items[g_SysWork.invItemSelectedIdx].id + InvItemId_KitchenKnife;
                g_SysWork.playerCombat.weaponInventoryIdx = g_SysWork.invItemSelectedIdx & 0xFF;
                D_800C3E18[7]                             = g_SysWork.invItemSelectedIdx;

                for (k = 0; k < INV_ITEM_COUNT_MAX; k++)
                {
                    if (g_SavegamePtr->items[D_800C3E18[7]].id == g_Item_MapLoadableItems[k])
                    {
                        Gfx_Items_Display((s_TmdFile*)FS_BUFFER_8, 7, k);
                        func_8005487C(7);
                        k = INV_ITEM_COUNT_MAX;
                    }
                }

                if (INV_ITEM_GROUP(g_SavegamePtr->items[g_Inventory_EquippedItemIdx].id) == InvItemGroup_GunWeapons)
                {
                    g_SysWork.playerCombat.currentWeaponAmmo = g_SavegamePtr->items[g_Inventory_EquippedItemIdx].count;
                    g_SysWork.playerCombat.totalWeaponAmmo   = 0;

                    for (i = 0; i < g_SavegamePtr->invSlotCount; i++)
                    {
                        if (g_SavegamePtr->items[i].id == INV_WEAPON_AMMO_ID(g_SavegamePtr->items[g_Inventory_EquippedItemIdx].id))
                        {
                            g_SysWork.playerCombat.totalWeaponAmmo = g_SavegamePtr->items[i].count;
                        }
                    }
                }
                else
                {
                    g_SysWork.playerCombat.currentWeaponAmmo = 1;
                }
            }

            g_Inventory_ScrollTransitionTimer++;

            if (g_Inventory_ScrollTransitionTimer == 9)
            {
                g_Inventory_EquippedItem                  = g_SavegamePtr->items[g_SysWork.invItemSelectedIdx].id;
                g_SavegamePtr->equippedWeapon             = g_Inventory_EquippedItem;
                g_Inventory_ScrollTransitionTimer         = 0;
                g_SysWork.playerCombat.weaponAttack       = g_Inventory_EquippedItem + InvItemId_KitchenKnife;
                g_SysWork.playerCombat.weaponInventoryIdx = g_SysWork.invItemSelectedIdx;

                *selectedItemId              = 0;
                temp3                        = g_GameWork.gameStateSteps[2];
                D_800AE188                   = 0;
                g_GameWork.gameStateSteps[1] = 1;
                D_800C3BA8                   = temp3;

                func_8004EF48();
                func_8004C564(0, NO_VALUE);
            }
            else
            {
                g_Items_Transforms[7].scale.vz = g_Inventory_ScrollTransitionTimer << 9;
                g_Items_Transforms[7].scale.vy = g_Inventory_ScrollTransitionTimer << 9;
                g_Items_Transforms[7].scale.vx = g_Inventory_ScrollTransitionTimer << 9;
            }

            g_Items_Coords[7].coord.t[0] = Q8(0.0f);
            g_Items_Coords[7].coord.t[1] = Q8(-2.5f);
            g_Items_Coords[7].coord.t[2] = Q8(0.0f);
            break;

        case 6: // Unequip weapon.
            g_Inventory_ScrollTransitionTimer++;

            if (g_Inventory_ScrollTransitionTimer == 9)
            {
                g_Inventory_EquippedItem                  = InvItemId_Unequipped;
                g_SavegamePtr->equippedWeapon             = InvItemId_Unequipped;
                g_SysWork.playerCombat.weaponAttack       = NO_VALUE;
                g_SysWork.playerCombat.weaponInventoryIdx = NO_VALUE;
                g_Inventory_ScrollTransitionTimer         = 0;

                temp = g_SysWork.invItemSelectedIdx;
                if (temp == NO_VALUE)
                {
                    D_800AE188 = 0;
                }

                *selectedItemId              = 0;
                temp3                        = g_GameWork.gameStateSteps[2];
                v0                           = 1;
                g_GameWork.gameStateSteps[1] = v0;
                D_800C3BA8                   = temp3;

                func_8004EF48();
                func_8004C564(0, NO_VALUE);
            }
            else
            {
                g_Items_Transforms[7].scale.vz = Q12(1.0f) - (g_Inventory_ScrollTransitionTimer << 9);
                g_Items_Transforms[7].scale.vy = Q12(1.0f) - (g_Inventory_ScrollTransitionTimer << 9);
                g_Items_Transforms[7].scale.vx = Q12(1.0f) - (g_Inventory_ScrollTransitionTimer << 9);
            }

            g_Items_Coords[7].coord.t[0] = Q8(0.0f);
            g_Items_Coords[7].coord.t[1] = Q8(-2.5f);
            g_Items_Coords[7].coord.t[2] = Q8(0.0f);
            break;

        case 7: // Reload equipped weapon.
            if (g_Inventory_ScrollTransitionTimer == 0)
            {
                D_800C3BAC      = NO_VALUE;
                curWeaponAmmo   = g_SysWork.playerCombat.currentWeaponAmmo;
                totalWeaponAmmo = g_SysWork.playerCombat.totalWeaponAmmo;

                Items_AmmoReloadCompute(&curWeaponAmmo, &totalWeaponAmmo, g_SysWork.playerCombat.weaponAttack);

                g_SysWork.playerCombat.totalWeaponAmmo                                = totalWeaponAmmo;
                g_SysWork.playerCombat.currentWeaponAmmo                              = curWeaponAmmo;
                g_SavegamePtr->items[g_SysWork.playerCombat.weaponInventoryIdx].count = curWeaponAmmo;

                for (j = 0; j < g_SavegamePtr->invSlotCount; j++)
                {
                    if (g_SavegamePtr->items[j].id == INV_WEAPON_AMMO_ID(g_SavegamePtr->items[g_SysWork.playerCombat.weaponInventoryIdx].id))
                    {
                        g_SavegamePtr->items[j].count = g_SysWork.playerCombat.totalWeaponAmmo;
                        D_800C3BAC                        = j;
                        j                                 = g_SavegamePtr->invSlotCount;
                    }
                }
            }

            if (g_SavegamePtr->items[g_SysWork.invItemSelectedIdx].count == 0 &&
                INV_ITEM_GROUP(g_SavegamePtr->items[g_SysWork.invItemSelectedIdx].id) == InvItemGroup_GunAmmo)
            {
                g_Inventory_ScrollTransitionTimer++;
                *selectedItemId = 0;

                if (g_Inventory_ScrollTransitionTimer == 9)
                {
                    g_SavegamePtr->items[g_SysWork.invItemSelectedIdx].id = InvItemId_Empty;

                    ptr = &g_GameWork;
                    do
                    {
                        g_Inventory_ScrollTransitionTimer = 0;
                        temp4                        = ptr->gameStateSteps[2];
                        g_GameWork.gameStateSteps[1] = 1;
                        D_800C3BA8                   = temp4;
                    }
                    while (0); // @hack

                    func_8004EF48();
                }
                else
                {
                    for (j = 0; j < 7; j++)
                    {
                        if (D_800C3E18[j] == g_SysWork.invItemSelectedIdx)
                        {
                            g_Items_Transforms[j].scale.vz = Q12(1.0f) - (g_Inventory_ScrollTransitionTimer << 9);
                            g_Items_Transforms[j].scale.vy = Q12(1.0f) - (g_Inventory_ScrollTransitionTimer << 9);
                            g_Items_Transforms[j].scale.vx = Q12(1.0f) - (g_Inventory_ScrollTransitionTimer << 9);
                        }
                    }
                }
            }
            else if (g_SavegamePtr->items[g_SysWork.playerCombat.weaponInventoryIdx + 1].count == 0 &&
                     D_800C3BAC != NO_VALUE)
            {
                *selectedItemId = 0;
                g_Inventory_ScrollTransitionTimer++;

                if (g_Inventory_ScrollTransitionTimer == 9)
                {
                    g_SavegamePtr->items[g_SysWork.playerCombat.weaponInventoryIdx + 1].id = InvItemId_Empty;
                    g_Inventory_ScrollTransitionTimer = 0;

                    func_8004EF48();

                    *selectedItemId              = 0;
                    D_800AE188                   = 0;
                    temp5                        = g_GameWork.gameStateSteps[2];
                    g_GameWork.gameStateSteps[1] = 1;
                    D_800C3BA8                   = temp5;
                }
                else
                {
                    for (i = 0; i < 7; i++)
                    {
                        if (D_800C3E18[i] == (g_SysWork.playerCombat.weaponInventoryIdx + 1))
                        {
                            g_Items_Transforms[i].scale.vz = Q12(1.0f) - (g_Inventory_ScrollTransitionTimer << 9);
                            g_Items_Transforms[i].scale.vy = Q12(1.0f) - (g_Inventory_ScrollTransitionTimer << 9);
                            g_Items_Transforms[i].scale.vx = Q12(1.0f) - (g_Inventory_ScrollTransitionTimer << 9);
                        }
                    }
                }
            }
            else
            {
                *selectedItemId = 0;

                g_Inventory_ScrollTransitionTimer++;
                if (g_Inventory_ScrollTransitionTimer == 9)
                {
                    g_Inventory_ScrollTransitionTimer = 0;
                    *selectedItemId                   = 0;
                    D_800AE188                        = 0;
                    temp5                             = g_GameWork.gameStateSteps[2];
                    g_GameWork.gameStateSteps[1]      = 1;
                    D_800C3BA8                        = temp5;
                }
            }
            break;

        case 8: // Reload unequipped weapon.
            var_t3 = NO_VALUE;
            if (g_Inventory_ScrollTransitionTimer == 0)
            {
                D_800C3BAC = NO_VALUE;

                if (INV_ITEM_GROUP(g_SavegamePtr->items[g_SysWork.invItemSelectedIdx].id) == InvItemGroup_GunAmmo)
                {
                    for (l = 0; l < g_SavegamePtr->invSlotCount; l++)
                    {
                        if (g_SavegamePtr->items[l].id == INV_AMMO_WEAPON_ID(g_SavegamePtr->items[g_SysWork.invItemSelectedIdx].id))
                        {
                            someCount = g_Items_GunsMaxLoadAmmo[g_SavegamePtr->items[g_SysWork.invItemSelectedIdx].id - 160] -
                                        g_SavegamePtr->items[l].count;
                            if (g_SavegamePtr->items[g_SysWork.invItemSelectedIdx].count >= someCount)
                            {
                                g_SavegamePtr->items[l].count                            += someCount;
                                g_SavegamePtr->items[g_SysWork.invItemSelectedIdx].count -= someCount;
                            }
                            else
                            {
                                g_SavegamePtr->items[l].count                           += g_SavegamePtr->items[g_SysWork.invItemSelectedIdx].count;
                                g_SavegamePtr->items[g_SysWork.invItemSelectedIdx].count = 0;
                            }

                            if (g_SavegamePtr->items[l].id == (g_SysWork.playerCombat.weaponAttack + InvItemId_KitchenKnife))
                            {
                                g_SysWork.playerCombat.currentWeaponAmmo = g_SavegamePtr->items[l].count;
                                g_SysWork.playerCombat.totalWeaponAmmo   = g_SavegamePtr->items[g_SysWork.invItemSelectedIdx].count;
                            }
                        }
                    }
                }
                else
                {
                    for (l = 0; l < g_SavegamePtr->invSlotCount; l++)
                    {
                        if (g_SavegamePtr->items[l].id == INV_WEAPON_AMMO_ID(g_SavegamePtr->items[g_SysWork.invItemSelectedIdx].id))
                        {
                            D_800C3BAC = l;

                            someCount = g_Items_GunsMaxLoadAmmo[g_SavegamePtr->items[g_SysWork.invItemSelectedIdx].id - 128] -
                                        g_SavegamePtr->items[g_SysWork.invItemSelectedIdx].count;
                            if (g_SavegamePtr->items[l].count >= someCount)
                            {
                                g_SavegamePtr->items[g_SysWork.invItemSelectedIdx].count += someCount;
                                g_SavegamePtr->items[l].count                            -= someCount;
                            }
                            else
                            {
                                var_t3                                                      = l;
                                g_SavegamePtr->items[g_SysWork.invItemSelectedIdx].count += g_SavegamePtr->items[l].count;
                                g_SavegamePtr->items[l].count                             = 0;
                            }
                        }
                    }
                }
            }

            if ((g_SavegamePtr->items[g_SysWork.invItemSelectedIdx].count == 0 &&
                 INV_ITEM_GROUP(g_SavegamePtr->items[g_SysWork.invItemSelectedIdx].id) == InvItemGroup_GunAmmo) ||
                var_t3 >= 0 && g_SavegamePtr->items[var_t3].count == 0)
            {
                g_Inventory_ScrollTransitionTimer++;
                *selectedItemId = 0;

                if (g_Inventory_ScrollTransitionTimer == 9)
                {
                    g_SavegamePtr->items[g_SysWork.invItemSelectedIdx].id = InvItemId_Empty;
                    ptr2                                                    = &g_GameWork;

                    do
                    {
                        g_Inventory_ScrollTransitionTimer = 0;
                        temp4                             = ptr2->gameStateSteps[2];
                        g_GameWork.gameStateSteps[1]      = 1;
                        D_800C3BA8                        = temp4;
                    }
                    while (0); // @hack

                    func_8004EF48();
                }
                else
                {
                    for (l = 0; l < 7; l++)
                    {
                        if (D_800C3E18[l] == g_SysWork.invItemSelectedIdx)
                        {
                            g_Items_Transforms[l].scale.vz = Q12(1.0f) - (g_Inventory_ScrollTransitionTimer << 9);
                            g_Items_Transforms[l].scale.vy = Q12(1.0f) - (g_Inventory_ScrollTransitionTimer << 9);
                            g_Items_Transforms[l].scale.vx = Q12(1.0f) - (g_Inventory_ScrollTransitionTimer << 9);
                        }
                    }
                }
            }
            else if (g_SavegamePtr->items[g_SysWork.invItemSelectedIdx + 1].count == 0 && D_800C3BAC != NO_VALUE)
            {
                *selectedItemId = 0;
                g_Inventory_ScrollTransitionTimer++;

                if (g_Inventory_ScrollTransitionTimer == 9)
                {
                    g_SavegamePtr->items[g_SysWork.invItemSelectedIdx + 1].id = InvItemId_Empty;
                    g_Inventory_ScrollTransitionTimer = 0;

                    func_8004EF48();

                    *selectedItemId              = 0;
                    D_800AE188                   = 0;
                    temp5                        = g_GameWork.gameStateSteps[2];
                    g_GameWork.gameStateSteps[1] = 1;
                    D_800C3BA8                   = temp5;
                }
                else
                {
                    for (l = 0; l < 7; l++)
                    {
                        if (D_800C3E18[l] == (g_SysWork.invItemSelectedIdx + 1))
                        {
                            g_Items_Transforms[l].scale.vz = Q12(1.0f) - (g_Inventory_ScrollTransitionTimer << 9);
                            g_Items_Transforms[l].scale.vy = Q12(1.0f) - (g_Inventory_ScrollTransitionTimer << 9);
                            g_Items_Transforms[l].scale.vx = Q12(1.0f) - (g_Inventory_ScrollTransitionTimer << 9);
                        }
                    }
                }
            }
            else
            {
                *selectedItemId = 0;
                g_Inventory_ScrollTransitionTimer++;

                if (g_Inventory_ScrollTransitionTimer == 9)
                {
                    g_Inventory_ScrollTransitionTimer = 0;
                    *selectedItemId                   = 0;
                    D_800AE188                        = 0;
                    temp5                             = g_GameWork.gameStateSteps[2];
                    g_GameWork.gameStateSteps[1]      = 1;
                    D_800C3BA8                        = temp5;
                }
            }
            break;

        case 9: // Apply health.
            if (g_Inventory_ScrollTransitionTimer == 0)
            {
                switch (g_SavegamePtr->items[g_SysWork.invItemSelectedIdx].id)
                {
                    case InvItemId_HealthDrink:
                    case InvItemId_FirstAidKit:
                    case InvItemId_Ampoule:
                        switch (g_SavegamePtr->items[g_SysWork.invItemSelectedIdx].id)
                        {
                            case InvItemId_FirstAidKit:
                                g_SysWork.playerWork.player.health += Q12(80.0f);
                                break;

                            case InvItemId_HealthDrink:
                                g_SysWork.playerWork.player.health += Q12(40.0f);
                                break;

                            case InvItemId_Ampoule:
                                g_SysWork.playerWork.player.health += Q12(100.0f);
                                g_SavegamePtr->healthSaturation     = Q12(300.0f);
                                break;
                        }

                        Sd_SfxPlay(Sfx_Unk1325, Q8(-0.25f), Q8(0.25f));
                        g_SysWork.playerWork.player.health = CLAMP(g_SysWork.playerWork.player.health, Q12(0.0f), Q12(100.0f));
                        g_SavegamePtr->items[g_SysWork.invItemSelectedIdx].count--;
                        break;
                }
            }

            if (g_SavegamePtr->items[g_SysWork.invItemSelectedIdx].count == 0)
            {
                g_Inventory_ScrollTransitionTimer++;

                if (g_Inventory_ScrollTransitionTimer == 9)
                {
                    g_SavegamePtr->items[g_SysWork.invItemSelectedIdx].id = InvItemId_Empty;

                    g_Inventory_ScrollTransitionTimer = 0;

                    *selectedItemId                 = 0;
                    D_800AE188                      = 0;
                    temp4                           = g_GameWork.gameStateSteps[2];
                    g_GameWork.gameStateSteps[1] = 1;
                    D_800C3BA8                      = temp4;

                    func_8004EF48();
                }
                else
                {
                    for (i = 0; i < 7; i++)
                    {
                        if (D_800C3E18[i] == g_SysWork.invItemSelectedIdx)
                        {
                            g_Items_Transforms[i].scale.vz = Q12(1.0f) - (g_Inventory_ScrollTransitionTimer << 9);
                            g_Items_Transforms[i].scale.vy = Q12(1.0f) - (g_Inventory_ScrollTransitionTimer << 9);
                            g_Items_Transforms[i].scale.vx = Q12(1.0f) - (g_Inventory_ScrollTransitionTimer << 9);
                        }
                    }
                }
            }
            else
            {
                g_Inventory_ScrollTransitionTimer++;

                if (g_Inventory_ScrollTransitionTimer == 9)
                {
                    g_Inventory_ScrollTransitionTimer = 0;
                    *selectedItemId                   = 0;
                    D_800AE188                        = 0;

                    temp5                           = g_GameWork.gameStateSteps[2];
                    g_GameWork.gameStateSteps[1] = 1;
                    D_800C3BA8                      = temp5;
                }
            }
            break;

        case 11:
            if (g_Inventory_ScrollTransitionTimer == 0 &&
                ((g_SavegamePtr->items[(u8)g_SysWork.invItemSelectedIdx].id >= InvItemId_LobbyKey &&
                  g_SavegamePtr->items[(u8)g_SysWork.invItemSelectedIdx].id <  InvItemId_KitchenKnife) ||
                 g_SavegamePtr->items[(u8)g_SysWork.invItemSelectedIdx].id == InvItemId_GasolineTank))
            {
                g_SavegamePtr->items[(u8)g_SysWork.invItemSelectedIdx].count = 0;
            }

            g_Inventory_ScrollTransitionTimer++;

            if (g_SavegamePtr->items[g_SysWork.invItemSelectedIdx].count == 0)
            {
                if (g_Inventory_ScrollTransitionTimer == 9)
                {
                    g_SavegamePtr->items[g_SysWork.invItemSelectedIdx].id = 0xFF;
                    g_Inventory_ScrollTransitionTimer                       = 0;
                    *selectedItemId                                         = 0;
                    D_800AE188                                              = 0;

                    temp3                        = g_GameWork.gameStateSteps[2];
                    g_GameWork.gameStateSteps[1] = 17;
                    D_800C3BA8                   = temp3;

                    for (i = 0; i < 7; i++)
                    {
                        if (D_800C3E18[i] == g_SysWork.invItemSelectedIdx)
                        {
                            g_SavegamePtr->items[D_800C3E18[i]].id = InvItemId_Empty;

                            i = 7;
                        }
                    }
                }
                else
                {
                    for (i = 0; i < 7; i++)
                    {
                        if (D_800C3E18[i] == g_SysWork.invItemSelectedIdx)
                        {
                            g_Items_Transforms[i].scale.vz = Q12(1.0f) - (g_Inventory_ScrollTransitionTimer << 9);
                            g_Items_Transforms[i].scale.vy = Q12(1.0f) - (g_Inventory_ScrollTransitionTimer << 9);
                            g_Items_Transforms[i].scale.vx = Q12(1.0f) - (g_Inventory_ScrollTransitionTimer << 9);
                        }
                    }
                }
            }
            break;

        case 13:
            D_800AE190++;
            g_Inventory_ScrollTransitionTimer++;
            D_800AE190                        = CLAMP(D_800AE190, 0, 0x20);
            g_Inventory_ScrollTransitionTimer = CLAMP(g_Inventory_ScrollTransitionTimer, 0, 0x20);

            if (g_Inventory_ScrollTransitionTimer == 0x20)
            {
                if (Fs_QueueGetLength() == 0)
                {
                    Game_StateStepSet(1, 14);
                }
            }
            break;

        case 14:
            D_800AE190++;
            D_800AE190 = CLAMP(D_800AE190, 0, 0x40);

            if (g_Controller0->buttonFlags.clicked & (g_GameWorkPtr->config.controllerConfig.action |
                                                 g_GameWorkPtr->config.controllerConfig.cancel))
            {
                if (D_800AE190 == 0x40)
                {
                    g_Inventory_ScrollTransitionTimer = 0;
                    g_Inventory_SelectionBordersDraw  = 0;
                    Game_StateStepSet(1, 15);
                    D_800AE190 = 0;
                }
            }
            break;

        case 15:
            D_800AE190 += 2;
            g_Inventory_ScrollTransitionTimer++;
            g_Inventory_ScrollTransitionTimer = CLAMP(g_Inventory_ScrollTransitionTimer, 0, 9);

            if (D_800AE190 > 0x20)
            {
                g_Inventory_ScrollTransitionTimer = 0;
                temp2                             = g_GameWork.gameStateSteps[2];
                g_GameWork.gameStateSteps[1]      = 1;
                ptr3                              = &D_800C3BA8;

                do {} while (false); // @hack

                g_GameWork.gameStateSteps[2]     = 0;
                *ptr3                            = temp2;
                *selectedItemId                  = 0;
                D_800AE188                       = 0;
                D_800AE190                       = 0;
                g_Inventory_SelectionBordersDraw = 0;
            }
            break;
    }

    for (i = 0; i < 7; i++)
    {
        if (D_800C3E18[i] == g_SysWork.invItemSelectedIdx)
        {
            g_Items_Transforms[i].rotate.vy -= 0x10;
        }
    }

    g_Items_Transforms[7].rotate.vy += 0x20;
}

void func_800539A4(s32 scrollDirection, s32 arg1) // 0x800539A4
{
    #define RIGHT_SCROLL 7
    #define LEFT_SCROLL -7

    s32 sp10[10];
    s32 i;
    s32 var_a3_2;
    s32 var_s0;

    var_s0 = 0;

    for (i = 0; i < ARRAY_SIZE(sp10); i++)
    {
        sp10[i] = D_800C3E18[i];
    }

    var_a3_2 = (scrollDirection != 0) ? LEFT_SCROLL : RIGHT_SCROLL;
    var_a3_2 = (arg1 + var_a3_2 + g_SavegamePtr->invSlotCount) % g_SavegamePtr->invSlotCount;

    for (i = 0; i < 7; i++)
    {
        if (sp10[i] == var_a3_2)
        {
            var_s0 = i;
            i      = 7;
        }
    }

    sp10[var_s0] = arg1;

    if (g_SavegamePtr->items[arg1].id != (u8)InvItemId_Empty)
    {
        for (i = 0; i < INV_ITEM_COUNT_MAX; i++)
        {
            if (g_SavegamePtr->items[arg1].id == g_Item_MapLoadableItems[i])
            {
                Gfx_Items_Display(FS_BUFFER_8, var_s0, i);
                func_8005487C(var_s0);
                i = INV_ITEM_COUNT_MAX;
            }
        }
    }

    for (i = 0; i < ARRAY_SIZE(sp10); i++)
    {
        D_800C3E18[i] = sp10[i];
    }
}

/** @note For better comprehension related to file handling
 * of inventory items.
 *
 * - `GameFs_UniqueItemModelLoad` loads model of items individually.
 * - `GameFs_MapItemsModelLoad` and `GameFs_MapItemsTextureLoad` load packs of textures and
 *   models of items, though while `GameFs_UniqueItemModelLoad` seems to load
 *   them individually based on what is being passed as argument,
 * - `GameFs_MapItemsModelLoad` and `GameFs_MapItemsTextureLoad` load the packs based on the
 *   map being loaded.
 */

void GameFs_UniqueItemModelLoad(u8 itemId) // 0x80053B08
{
    switch (itemId)
    {
        case InvItemId_HealthDrink:
            Fs_QueueStartRead(FILE_ITEM_UNQ21_TMD, FS_BUFFER_5);
            break;

        case InvItemId_Ampoule:
            Fs_QueueStartRead(FILE_ITEM_UNQ22_TMD, FS_BUFFER_5);
            break;

        case InvItemId_HouseKey:
            Fs_QueueStartRead(FILE_ITEM_UNQ41_TMD, FS_BUFFER_5);
            break;

        case InvItemId_KeyOfLion:
            Fs_QueueStartRead(FILE_ITEM_UNQ42_TMD, FS_BUFFER_5);
            break;

        case InvItemId_KeyOfWoodman:
            Fs_QueueStartRead(FILE_ITEM_UNQ43_TMD, FS_BUFFER_5);
            break;

        case InvItemId_KeyOfScarecrow:
            Fs_QueueStartRead(FILE_ITEM_UNQ44_TMD, FS_BUFFER_5);
            break;

        case InvItemId_LibraryReserveKey:
            Fs_QueueStartRead(FILE_ITEM_UNQ45_TMD, FS_BUFFER_5);
            break;

        case InvItemId_ClassroomKey:
            Fs_QueueStartRead(FILE_ITEM_UNQ46_TMD, FS_BUFFER_5);
            break;

        case InvItemId_KGordonKey:
            Fs_QueueStartRead(FILE_ITEM_UNQ47_TMD, FS_BUFFER_5);
            break;

        case InvItemId_DrawbridgeKey:
            Fs_QueueStartRead(FILE_ITEM_UNQ48_TMD, FS_BUFFER_5);
            break;

        case InvItemId_BasementKey:
            Fs_QueueStartRead(FILE_ITEM_UNQ49_TMD, FS_BUFFER_5);
            break;

        case InvItemId_BasementStoreroomKey:
            Fs_QueueStartRead(FILE_ITEM_UNQ4A_TMD, FS_BUFFER_5);
            break;

        case InvItemId_ExaminationRoomKey:
            Fs_QueueStartRead(FILE_ITEM_UNQ4B_TMD, FS_BUFFER_5);
            break;

        case InvItemId_AntiqueShopKey:
            Fs_QueueStartRead(FILE_ITEM_UNQ4C_TMD, FS_BUFFER_5);
            break;

        case InvItemId_SewerKey:
        case InvItemId_SewerExitKey:
            Fs_QueueStartRead(FILE_ITEM_UNQ4D_TMD, FS_BUFFER_5);
            break;

        case InvItemId_KeyOfOphiel:
            Fs_QueueStartRead(FILE_ITEM_UNQ4E_TMD, FS_BUFFER_5);
            break;

        case InvItemId_KeyOfHagith:
            Fs_QueueStartRead(FILE_ITEM_UNQ4F_TMD, FS_BUFFER_5);
            break;

        case InvItemId_KeyOfPhaleg:
            Fs_QueueStartRead(FILE_ITEM_UNQ50_TMD, FS_BUFFER_5);
            break;

        case InvItemId_KeyOfBethor:
            Fs_QueueStartRead(FILE_ITEM_UNQ51_TMD, FS_BUFFER_5);
            break;

        case InvItemId_KeyOfAratron:
            Fs_QueueStartRead(FILE_ITEM_UNQ52_TMD, FS_BUFFER_5);
            break;

        case InvItemId_NoteToSchool:
            Fs_QueueStartRead(FILE_ITEM_UNQ53_TMD, FS_BUFFER_5);
            break;

        case InvItemId_NoteDoghouse:
            Fs_QueueStartRead(FILE_ITEM_UNQ54_TMD, FS_BUFFER_5);
            break;

        case InvItemId_PictureCard:
            Fs_QueueStartRead(FILE_ITEM_UNQ55_TMD, FS_BUFFER_5);
            break;

        case InvItemId_ChannelingStone:
            Fs_QueueStartRead(FILE_ITEM_UNQ56_TMD, FS_BUFFER_5);
            break;

        case InvItemId_Chemical:
            Fs_QueueStartRead(FILE_ITEM_UNQ60_TMD, FS_BUFFER_5);
            break;

        case InvItemId_GoldMedallion:
            Fs_QueueStartRead(FILE_ITEM_UNQ61_TMD, FS_BUFFER_5);
            break;

        case InvItemId_SilverMedallion:
            Fs_QueueStartRead(FILE_ITEM_UNQ62_TMD, FS_BUFFER_5);
            break;

        case InvItemId_RubberBall:
            Fs_QueueStartRead(FILE_ITEM_UNQ63_TMD, FS_BUFFER_5);
            break;

        case InvItemId_Flauros:
            Fs_QueueStartRead(FILE_ITEM_UNQ64_TMD, FS_BUFFER_5);
            break;

        case InvItemId_PlasticBottle:
            Fs_QueueStartRead(FILE_ITEM_UNQ65_TMD, FS_BUFFER_5);
            break;

        case InvItemId_UnknownLiquid:
            Fs_QueueStartRead(FILE_ITEM_UNQ66_TMD, FS_BUFFER_5);
            break;

        case InvItemId_PlateOfHatter:
            Fs_QueueStartRead(FILE_ITEM_UNQ67_TMD, FS_BUFFER_5);
            break;

        case InvItemId_PlateOfCat:
            Fs_QueueStartRead(FILE_ITEM_UNQ68_TMD, FS_BUFFER_5);
            break;

        case InvItemId_PlateOfQueen:
            Fs_QueueStartRead(FILE_ITEM_UNQ69_TMD, FS_BUFFER_5);
            break;

        case InvItemId_PlateOfTurtle:
            Fs_QueueStartRead(FILE_ITEM_UNQ6A_TMD, FS_BUFFER_5);
            break;

        case InvItemId_BloodPack:
            Fs_QueueStartRead(FILE_ITEM_UNQ6B_TMD, FS_BUFFER_5);
            break;

        case InvItemId_DisinfectingAlcohol:
            Fs_QueueStartRead(FILE_ITEM_UNQ6C_TMD, FS_BUFFER_5);
            break;

        case InvItemId_Lighter:
            Fs_QueueStartRead(FILE_ITEM_UNQ6D_TMD, FS_BUFFER_5);
            break;

        case InvItemId_VideoTape:
            Fs_QueueStartRead(FILE_ITEM_UNQ6E_TMD, FS_BUFFER_5);
            break;

        case InvItemId_KaufmannKey:
            Fs_QueueStartRead(FILE_ITEM_UNQ70_TMD, FS_BUFFER_5);
            break;

        case InvItemId_Receipt:
            Fs_QueueStartRead(FILE_ITEM_UNQ71_TMD, FS_BUFFER_5);
            break;

        case InvItemId_SafeKey:
            Fs_QueueStartRead(FILE_ITEM_UNQ72_TMD, FS_BUFFER_5);
            break;

        case InvItemId_Magnet:
            Fs_QueueStartRead(FILE_ITEM_UNQ73_TMD, FS_BUFFER_5);
            break;

        case InvItemId_MotorcycleKey:
            Fs_QueueStartRead(FILE_ITEM_UNQ74_TMD, FS_BUFFER_5);
            break;

        case InvItemId_BirdCageKey:
            Fs_QueueStartRead(FILE_ITEM_UNQ75_TMD, FS_BUFFER_5);
            break;

        case InvItemId_Pliers:
            Fs_QueueStartRead(FILE_ITEM_UNQ76_TMD, FS_BUFFER_5);
            break;

        case InvItemId_Screwdriver:
            Fs_QueueStartRead(FILE_ITEM_UNQ77_TMD, FS_BUFFER_5);
            break;

        case InvItemId_Camera:
            Fs_QueueStartRead(FILE_ITEM_UNQ78_TMD, FS_BUFFER_5);
            break;

        case InvItemId_RingOfContract:
            Fs_QueueStartRead(FILE_ITEM_UNQ79_TMD, FS_BUFFER_5);
            break;

        case InvItemId_StoneOfTime:
            Fs_QueueStartRead(FILE_ITEM_UNQ7A_TMD, FS_BUFFER_5);
            break;

        case InvItemId_AmuletOfSolomon:
            Fs_QueueStartRead(FILE_ITEM_UNQ7B_TMD, FS_BUFFER_5);
            break;

        case InvItemId_CrestOfMercury:
            Fs_QueueStartRead(FILE_ITEM_UNQ7C_TMD, FS_BUFFER_5);
            break;

        case InvItemId_Ankh:
            Fs_QueueStartRead(FILE_ITEM_UNQ7D_TMD, FS_BUFFER_5);
            break;

        case InvItemId_DaggerOfMelchior:
            Fs_QueueStartRead(FILE_ITEM_UNQ7E_TMD, FS_BUFFER_5);
            break;

        case InvItemId_DiskOfOuroboros:
            Fs_QueueStartRead(FILE_ITEM_UNQ7F_TMD, FS_BUFFER_5);
            break;

        case InvItemId_KitchenKnife:
            Fs_QueueStartRead(FILE_ITEM_UNQ80_TMD, FS_BUFFER_5);
            break;

        case InvItemId_SteelPipe:
            Fs_QueueStartRead(FILE_ITEM_UNQ81_TMD, FS_BUFFER_5);
            break;

        case InvItemId_Hammer:
            Fs_QueueStartRead(FILE_ITEM_UNQ82_TMD, FS_BUFFER_5);
            break;

        case InvItemId_Chainsaw:
            Fs_QueueStartRead(FILE_ITEM_UNQ83_TMD, FS_BUFFER_5);
            break;

        case InvItemId_Axe:
            Fs_QueueStartRead(FILE_ITEM_UNQ84_TMD, FS_BUFFER_5);
            break;

        case InvItemId_RockDrill:
            Fs_QueueStartRead(FILE_ITEM_UNQ85_TMD, FS_BUFFER_5);
            break;

        case InvItemId_Katana:
            Fs_QueueStartRead(FILE_ITEM_UNQ86_TMD, FS_BUFFER_5);
            break;

        case InvItemId_Handgun:
            Fs_QueueStartRead(FILE_ITEM_UNQA0_TMD, FS_BUFFER_5);
            break;

        case InvItemId_HuntingRifle:
            Fs_QueueStartRead(FILE_ITEM_UNQA1_TMD, FS_BUFFER_5);
            break;

        case InvItemId_Shotgun:
            Fs_QueueStartRead(FILE_ITEM_UNQA2_TMD, FS_BUFFER_5);
            break;

        case InvItemId_HyperBlaster:
            Fs_QueueStartRead(FILE_ITEM_UNQA3_TMD, FS_BUFFER_5);
            break;

        case InvItemId_HandgunBullets:
            Fs_QueueStartRead(FILE_ITEM_UNQC0_TMD, FS_BUFFER_5);
            break;

        case InvItemId_RifleShells:
            Fs_QueueStartRead(FILE_ITEM_UNQC1_TMD, FS_BUFFER_5);
            break;

        case InvItemId_ShotgunShells:
            Fs_QueueStartRead(FILE_ITEM_UNQC2_TMD, FS_BUFFER_5);
            break;

        case InvItemId_Flashlight:
            Fs_QueueStartRead(FILE_ITEM_UNQE0_TMD, FS_BUFFER_5);
            break;

        case InvItemId_PocketRadio:
            Fs_QueueStartRead(FILE_ITEM_UNQE1_TMD, FS_BUFFER_5);
            break;

        case InvItemId_GasolineTank:
            Fs_QueueStartRead(FILE_ITEM_UNQE2_TMD, FS_BUFFER_5);
            break;

        default:
            Fs_QueueStartRead(FILE_ITEM_UNQ20_TMD, FS_BUFFER_5);
            break;
    }
}

void GameFs_Tim00TIMLoad(void) // 0x80053dA0
{
    if (g_SysWork.invItemLoadFlags & InvItemLoadFlag_Texture0)
    {
        return;
    }

    Fs_QueueStartReadTim(FILE_ITEM_TIM00_TIM, FS_BUFFER_1, &g_FirstAidKitItemTextureImg);
    g_SysWork.invItemLoadFlags |= InvItemLoadFlag_Texture0;
}

void GameFs_MapItemsModelLoad(u32 mapId) // 0x80053DFC
{
    #define loadFlags g_SysWork.invItemLoadFlags

    if (!(loadFlags & InvItemLoadFlag_Model))
    {
        Fs_QueueStartReadTim(FILE_ITEM_TIM07_TIM, FS_BUFFER_1, &D_800A9074);
        loadFlags |= InvItemLoadFlag_Model;
    }

    switch (mapId)
    {
        case MapIdx_MAP1_S00:
        case MapIdx_MAP1_S01:
        case MapIdx_MAP1_S02:
        case MapIdx_MAP1_S03:
        case MapIdx_MAP1_S05:
            if (!(loadFlags & InvItemLoadFlag_Texture1))
            {
                Fs_QueueStartReadTim(FILE_ITEM_TIM01_TIM, FS_BUFFER_1, &g_InventoryKeyItemTextureImg);
                loadFlags = (loadFlags & (InvItemLoadFlag_Texture0 | InvItemLoadFlag_Model)) | InvItemLoadFlag_Texture1;
            }
            break;

        case MapIdx_MAP0_S01:
        case MapIdx_MAP0_S02:
        case MapIdx_MAP1_S06:
        case MapIdx_MAP2_S00:
        case MapIdx_MAP2_S01:
        case MapIdx_MAP2_S02:
        case MapIdx_MAP2_S04:
        case MapIdx_MAP4_S00:
        case MapIdx_MAP4_S01:
        case MapIdx_MAP4_S02:
        case MapIdx_MAP4_S03:
        case MapIdx_MAP4_S05:
            if (!(loadFlags & InvItemLoadFlag_Texture2))
            {
                Fs_QueueStartReadTim(FILE_ITEM_TIM02_TIM, FS_BUFFER_1, &g_InventoryKeyItemTextureImg);
                loadFlags = (loadFlags & (InvItemLoadFlag_Texture0 | InvItemLoadFlag_Model)) | InvItemLoadFlag_Texture2;
            }
            break;

        case MapIdx_MAP3_S00:
        case MapIdx_MAP3_S01:
        case MapIdx_MAP3_S02:
        case MapIdx_MAP3_S03:
        case MapIdx_MAP3_S04:
        case MapIdx_MAP3_S05:
        case MapIdx_MAP3_S06:
        case MapIdx_MAP4_S04:
            if (!(loadFlags & InvItemLoadFlag_Texture3))
            {
                Fs_QueueStartReadTim(FILE_ITEM_TIM03_TIM, FS_BUFFER_1, &g_InventoryKeyItemTextureImg);
                loadFlags = (loadFlags & (InvItemLoadFlag_Texture0 | InvItemLoadFlag_Model)) | InvItemLoadFlag_Texture3;
            }
            break;

        case MapIdx_MAP5_S00:
        case MapIdx_MAP5_S01:
        case MapIdx_MAP5_S02:
        case MapIdx_MAP5_S03:
        case MapIdx_MAP6_S00:
        case MapIdx_MAP6_S01:
        case MapIdx_MAP6_S02:
        case MapIdx_MAP6_S03:
        case MapIdx_MAP6_S04:
            if (!(loadFlags & InvItemLoadFlag_Texture4))
            {
                Fs_QueueStartReadTim(FILE_ITEM_TIM04_TIM, FS_BUFFER_1, &g_InventoryKeyItemTextureImg);
                loadFlags = (loadFlags & (InvItemLoadFlag_Texture0 | InvItemLoadFlag_Model)) | InvItemLoadFlag_Texture4;
            }
            break;

        case MapIdx_MAP7_S00:
        case MapIdx_MAP7_S01:
            if (!(loadFlags & InvItemLoadFlag_Texture5))
            {
                Fs_QueueStartReadTim(FILE_ITEM_TIM05_TIM, FS_BUFFER_1, &g_InventoryKeyItemTextureImg);
                loadFlags = (loadFlags & (InvItemLoadFlag_Texture0 | InvItemLoadFlag_Model)) | InvItemLoadFlag_Texture5;
            }
            break;

        case MapIdx_MAP7_S02:
        case MapIdx_MAP7_S03:
            if (!(loadFlags & InvItemLoadFlag_Texture6))
            {
                Fs_QueueStartReadTim(FILE_ITEM_TIM06_TIM, FS_BUFFER_1, &g_InventoryKeyItemTextureImg);
                loadFlags = (loadFlags & (InvItemLoadFlag_Texture0 | InvItemLoadFlag_Model)) | InvItemLoadFlag_Texture6;
            }
            break;

        case MapIdx_MAP0_S00:
            break;
    }

    #undef loadFlags
}

void GameFs_MapItemsTextureLoad(s32 mapId) // 0x80054024
{
    switch (mapId)
    {
        case MapIdx_MAP0_S00:
            Fs_QueueStartRead(FILE_ITEM_IT_000_TMD, FS_BUFFER_8);
            break;

        case MapIdx_MAP0_S01:
        case MapIdx_MAP0_S02:
        case MapIdx_MAP1_S06:
        case MapIdx_MAP2_S00:
        case MapIdx_MAP2_S01:
        case MapIdx_MAP2_S02:
        case MapIdx_MAP2_S04:
        case MapIdx_MAP4_S00:
        case MapIdx_MAP4_S01:
        case MapIdx_MAP4_S02:
        case MapIdx_MAP4_S03:
        case MapIdx_MAP4_S05:
            Fs_QueueStartRead(FILE_ITEM_IT_001_TMD, FS_BUFFER_8);
            break;

        case MapIdx_MAP1_S00:
        case MapIdx_MAP1_S01:
        case MapIdx_MAP1_S02:
        case MapIdx_MAP1_S03:
        case MapIdx_MAP1_S05:
            Fs_QueueStartRead(FILE_ITEM_IT_002_TMD, FS_BUFFER_8);
            break;

        case MapIdx_MAP3_S00:
        case MapIdx_MAP3_S01:
        case MapIdx_MAP3_S02:
        case MapIdx_MAP3_S03:
        case MapIdx_MAP3_S04:
        case MapIdx_MAP3_S05:
        case MapIdx_MAP3_S06:
        case MapIdx_MAP4_S04:
            Fs_QueueStartRead(FILE_ITEM_IT_003_TMD, FS_BUFFER_8);
            break;

        case MapIdx_MAP5_S00:
        case MapIdx_MAP5_S01:
        case MapIdx_MAP5_S02:
        case MapIdx_MAP5_S03:
        case MapIdx_MAP6_S00:
        case MapIdx_MAP6_S01:
        case MapIdx_MAP6_S02:
        case MapIdx_MAP6_S03:
        case MapIdx_MAP6_S04:
            Fs_QueueStartRead(FILE_ITEM_IT_004_TMD, FS_BUFFER_8);
            break;

        case MapIdx_MAP7_S00:
        case MapIdx_MAP7_S01:
            Fs_QueueStartRead(FILE_ITEM_IT_005_TMD, FS_BUFFER_8);
            break;

        case MapIdx_MAP7_S02:
        case MapIdx_MAP7_S03:
            Fs_QueueStartRead(FILE_ITEM_IT_006_TMD, FS_BUFFER_8);
            break;
    }
}

void func_800540A4(s8 arg0) // 0x800540A4
{
    s32 i;
    s32 j;

    if (g_SysWork.playerCombat.weaponInventoryIdx != NO_VALUE)
    {
        g_SavegamePtr->items[g_SysWork.playerCombat.weaponInventoryIdx].count = g_SysWork.playerCombat.currentWeaponAmmo;
    }

    for (i = 0; i < g_SavegamePtr->invSlotCount; i++)
    {
        if (INV_ITEM_GROUP(g_SavegamePtr->items[i].id) == InvItemGroup_GunAmmo &&
            g_SavegamePtr->items[i].id == INV_WEAPON_AMMO_ID(g_SavegamePtr->items[g_SysWork.playerCombat.weaponInventoryIdx].id))
        {
            g_SavegamePtr->items[i].count = g_SysWork.playerCombat.totalWeaponAmmo;
            if (g_SavegamePtr->items[i].count == 0)
            {
                g_SavegamePtr->items[i].id = InvItemId_Empty;
            }

            g_SavegamePtr->invSlotCount = func_8004F190(g_SavegamePtr);
            break;
        }
    }

    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            D_800C3B68[i][j].vy = 0;
            D_800C3B68[i][j].vx = 0;
        }
    }

    D_800AE18C = 0;
    D_800AE18E = 0;

    Fs_QueueStartReadTim(FILE_ITEM_HEROPIC2_TIM, FS_BUFFER_1, &g_HealthPortraitImg);
}

void Gfx_Items_Draw(void) // 0x80054200
{
    s32  temp_s5;
    s32  i;
    s32  saveItemsIdx;
    s32  invItemsIdx;

    func_8004BFE8();

    D_800AE190 = 0;

    ItemScreen_CamSet(&D_800C3B48, &D_800C3AE8, &D_800C3B38, 0);

    // Define position, rotation, and scale of inventory item initially equipped by player.
    for (i = 0; i < DISPLAYED_ITEM_COUNT_MAX; i++)
    {
        D_800C3E18[i] = NO_VALUE;

        g_Items_Transforms[i].scale.vz  = Q12(1.0f);
        g_Items_Transforms[i].scale.vy  = Q12(1.0f);
        g_Items_Transforms[i].scale.vx  = Q12(1.0f);
        g_Items_Transforms[i].rotate.vz = Q12_ANGLE(0.0f);
        g_Items_Transforms[i].rotate.vy = Q12_ANGLE(0.0f);
        g_Items_Transforms[i].rotate.vx = Q12_ANGLE(0.0f);
        g_Items_Transforms[i].trans.vz  = 0;
        g_Items_Transforms[i].trans.vy  = 0;
        g_Items_Transforms[i].trans.vx  = 0;
    }

    GameFs_TmdDataAlloc(FS_BUFFER_8);

    temp_s5 = (g_SysWork.invItemSelectedIdx - 3 + g_SavegamePtr->invSlotCount) % g_SavegamePtr->invSlotCount;

    // Sscreen is inventory.
    if (g_GameWork.gameStateSteps[1] < 21)
    {
        for (invItemsIdx = 0; invItemsIdx < 7; invItemsIdx++)
        {
            D_800C3E18[invItemsIdx] = (temp_s5 + invItemsIdx) % g_SavegamePtr->invSlotCount;

            if (g_SavegamePtr->items[D_800C3E18[invItemsIdx]].id == (u8)InvItemId_Empty)
            {
                continue;
            }

            for (saveItemsIdx = 0; saveItemsIdx < INV_ITEM_COUNT_MAX; saveItemsIdx++)
            {
                if (g_SavegamePtr->items[D_800C3E18[invItemsIdx]].id == g_Item_MapLoadableItems[saveItemsIdx])
                {
                    Gfx_Items_Display(FS_BUFFER_8, invItemsIdx, saveItemsIdx);
                    func_8005487C(invItemsIdx);

                    saveItemsIdx = INV_ITEM_COUNT_MAX;
                }
            }
        }

        g_Inventory_EquippedItem = g_SavegamePtr->equippedWeapon;

        for (i = 0; i < g_SavegamePtr->invSlotCount; i++)
        {
            if (g_SavegamePtr->items[i].id == g_Inventory_EquippedItem)
            {
                D_800C3E18[7] = i;

                i = g_SavegamePtr->invSlotCount;
            }
        }

        // Player has something equipped.
        if (g_Inventory_EquippedItemIdx != NO_VALUE)
        {
            for (saveItemsIdx = 0; saveItemsIdx < INV_ITEM_COUNT_MAX; saveItemsIdx++)
            {
                if (g_Inventory_EquippedItem == g_Item_MapLoadableItems[saveItemsIdx])
                {
                    Gfx_Items_Display(FS_BUFFER_8, 7, saveItemsIdx);
                    func_8005487C(7);

                    saveItemsIdx = INV_ITEM_COUNT_MAX;
                }
            }
        }

        g_Items_Coords[7].coord.t[0] = Q8(0.0f);
        g_Items_Coords[7].coord.t[1] = Q8(-2.5);
        g_Items_Coords[7].coord.t[2] = Q8(0.0f);
    }
    else
    {
        // TODO: Make `const`?
        u8 ITEM_IDS[] = {
            InvItemId_GasolineTank,
            InvItemId_Chainsaw,
            InvItemId_RockDrill,
            InvItemId_Katana,
            InvItemId_ChannelingStone,
            InvItemId_HyperBlaster
        };

        if (g_SavegamePtr->clearGameCount == 0)
        {
            g_SavegamePtr->clearGameCount = 1;
        }

        for (invItemsIdx = 0; invItemsIdx < 6; invItemsIdx++)
        {
            D_800C3E18[invItemsIdx] = invItemsIdx;

            for (saveItemsIdx = 0; saveItemsIdx < INV_ITEM_COUNT_MAX; saveItemsIdx++)
            {
                if (ITEM_IDS[invItemsIdx] == g_Item_MapLoadableItems[saveItemsIdx])
                {
                    Gfx_Items_Display(FS_BUFFER_8, invItemsIdx, saveItemsIdx);
                    func_8005487C(invItemsIdx);

                    saveItemsIdx = INV_ITEM_COUNT_MAX;
                }
            }
        }
    }
}

void Gfx_Items_DrawInit(void) // 0x80054558
{
    s32 i;

    g_Items_DisplayedCount                     = 0;
    D_800AE187                                 = InvItemId_Unequipped;
    g_Inventory_HealthStatusScanlineTimer      = 32;
    g_Inventory_HealthStatusColorGradientTimer = 0;
    D_800AE198                                 = 0;
    g_Inventory_HealthStatusScanlinePosition   = -300;
    g_Inventory_ItemNameTimer                  = 0;
    g_Inventory_DescriptionRollTimer           = 0;
    g_Inventory_ScrollTransitionTimer          = 0;

    for (i = 0; g_MapOverlayHdr.loadableItems[i] != 0; i++)
    {
        g_Item_MapLoadableItems[i] = g_MapOverlayHdr.loadableItems[i];
    }

    g_Item_MapLoadableItems[i] = 0;

    Gfx_Items_Draw();
    Gfx_Items_SetAmbientLighting();
}

void Inventory_ExitAnimFixes(void) // 0x80054634
{
    u8 weaponAttack;

    weaponAttack = (u8)g_SysWork.playerCombat.weaponAttack;

    g_SavegamePtr->equippedWeapon = g_Inventory_EquippedItem;
    if (g_SavegamePtr->equippedWeapon)
    {
        g_SysWork.playerCombat.weaponAttack = WEAPON_ATTACK(g_SavegamePtr->equippedWeapon + InvItemId_KitchenKnife, AttackInputType_Tap);
    }
    else
    {
        g_SysWork.playerCombat.weaponAttack = NO_VALUE;
        g_SysWork.playerCombat.isAiming     = false;
    }

    Gfx_PlayerHeldItemAttach(g_SysWork.playerCombat.weaponAttack);
    Inventory_ExitAnimEquippedItemUpdate(&weaponAttack);
}

void Gfx_PlayerHeldItemAttach(u8 weaponAttack) // 0x800546A8
{
    switch (weaponAttack)
    {
        case EquippedWeaponId_KitchenKnife:
            WorldGfx_CharaMeshSwap(Chara_Harry, MESH_SWAP_STATUS(HarrySwappableMesh_LeftHand, HarryVariantMesh_HandMelee));
            break;

        case EquippedWeaponId_SteelPipe:
        case EquippedWeaponId_Hammer:
        case EquippedWeaponId_Chainsaw:
        case EquippedWeaponId_Katana:
        case EquippedWeaponId_Axe:
        case EquippedWeaponId_RockDrill:
            WorldGfx_CharaMeshSwap(Chara_Harry, MESH_SWAP_STATUS(HarrySwappableMesh_LeftHand, HarryVariantMesh_HandMelee));
            break;

        case EquippedWeaponId_Handgun:
        case EquippedWeaponId_HuntingRifle:
        case EquippedWeaponId_Shotgun:
            WorldGfx_CharaMeshSwap(Chara_Harry, MESH_SWAP_STATUS(HarrySwappableMesh_RightHand, HarryVariantMesh_RightHandGun));
            break;

        default:
            WorldGfx_CharaMeshSwap(Chara_Harry, MESH_SWAP_STATUS(HarrySwappableMesh_None, HarryVariantMesh_RightHandEmpty));
            break;
    }
}

void Gfx_Items_Display(s_TmdFile* tmd, s32 displayItemIdx, s32 loadableItemIdx)
{
    u8                 itemId;
    GsDOBJ2*           ptr;
    struct TMD_STRUCT* models;

    models = tmd->models;

    GsLinkObject4((u32)&models[loadableItemIdx], &g_Items_ItemsModelData[displayItemIdx], 0);

    ptr         = &g_Items_ItemsModelData[displayItemIdx];
    ptr->coord2 = &g_Items_Coords[displayItemIdx];

    if (D_800AE187 != InvItemId_Unequipped)
    {
        itemId = D_800AE187;
    }
    else
    {
        itemId = g_MapOverlayHdr.loadableItems[loadableItemIdx];
    }

    switch (itemId)
    {
        case InvItemId_HealthDrink:
        case InvItemId_FirstAidKit:
        case InvItemId_Ampoule:
        case InvItemId_KitchenKnife:
        case InvItemId_SteelPipe:
        case InvItemId_Hammer:
        case InvItemId_Chainsaw:
        case InvItemId_Handgun:
        case InvItemId_HuntingRifle:
        case InvItemId_Shotgun:
        case InvItemId_HandgunBullets:
        case InvItemId_RifleShells:
        case InvItemId_ShotgunShells:
        case InvItemId_Flashlight:
        case InvItemId_PocketRadio:
        case InvItemId_Axe:
            ptr->attribute = 0;
            break;

        default:
            ptr->attribute = 0x40000000;
            break;
    }

    if (g_Items_DisplayedCount < DISPLAYED_ITEM_COUNT_MAX)
    {
        g_Items_DisplayedCount++;
    }
}

void func_8005487C(s32 arg0) // 0x8005487C
{
    // Initializes coordinate system base. `base->coord` is set to identity matrix (`GsIDMATRIX`), `super->sub` is set to base.
    GsInitCoordinate2(NULL, &g_Items_Coords[arg0]);

    g_Items_Coords[arg0].param = (GsCOORD2PARAM*) &g_Items_Transforms[arg0];
}

void func_800548D8(s32 idx) // 0x800548D8
{
    g_Items_Lights[idx][0].vx = g_Items_Coords[idx].coord.t[0];
    g_Items_Lights[idx][0].vy = g_Items_Coords[idx].coord.t[1];
    g_Items_Lights[idx][0].vz = g_Items_Coords[idx].coord.t[2] + 20000;
}

void Gfx_Items_SetAmbientLighting(void) // 0x80054928
{
    s32 i;

    for (i = 0; i < 10; i++)
    {
        g_Items_Lights[i][0].r  = NO_VALUE;
        g_Items_Lights[i][0].g  = NO_VALUE;
        g_Items_Lights[i][0].b  = NO_VALUE;
        g_Items_Lights[i][1].vx = Q12(1.0f);
        g_Items_Lights[i][1].vy = Q12(0.0f);
        g_Items_Lights[i][1].vz = Q12(0.0f);
        g_Items_Lights[i][1].r  = NO_VALUE;
        g_Items_Lights[i][1].g  = NO_VALUE;
        g_Items_Lights[i][1].b  = NO_VALUE;
    }

    GsSetAmbient(1024, 1024, 1024);
    GsSetLightMode(1);
}

// Similar to `Gfx_Items_SetAmbientLighting`.
void func_800549A0(void) // 0x800549A0
{
    #define IDX 9

    g_Items_Lights[IDX][0].r  = NO_VALUE;
    g_Items_Lights[IDX][1].vx = Q12(1.0f);
    g_Items_Lights[IDX][0].g  = NO_VALUE;
    g_Items_Lights[IDX][0].b  = NO_VALUE;
    g_Items_Lights[IDX][1].r  = NO_VALUE;
    g_Items_Lights[IDX][1].g  = NO_VALUE;
    g_Items_Lights[IDX][1].b  = NO_VALUE;
    g_Items_Lights[IDX][1].vy = Q12(0.0f);
    g_Items_Lights[IDX][1].vz = Q12(0.0f);

    GsSetAmbient(2048, 2048, 2048);
    GsSetLightMode(1);
}

void func_80054A04(u8 itemId) // 0x80054A04
{
    D_800AE187              = itemId;
    g_Items_DisplayedCount  = 0;
    g_Items_PickupAnimState = 0;
    g_Items_PickupScale     = Q12(0.0f);

    D_800C3E18[9]                   = NO_VALUE;
    g_Items_Transforms[9].rotate.vz = Q12_ANGLE(0.0f);
    g_Items_Transforms[9].rotate.vy = Q12_ANGLE(0.0f);
    g_Items_Transforms[9].rotate.vx = Q12_ANGLE(0.0f);
    g_Items_Transforms[9].trans.vz  = 0;
    g_Items_Transforms[9].trans.vy  = 0;
    g_Items_Transforms[9].trans.vx  = 0;

    GameFs_TmdDataAlloc(FS_BUFFER_5);

    D_800C3E18[9] = 0;

    Gfx_Items_Display(FS_BUFFER_5, 9, 0);
    func_8005487C(9);

    g_Items_Transforms[9].scale.vz = Q12(1.0f);
    g_Items_Transforms[9].scale.vy = Q12(1.0f);
    g_Items_Transforms[9].scale.vx = Q12(1.0f);

    func_800549A0();
    ItemScreen_CamSet(&D_800C3B48, &D_800C3AE8, &D_800C3B38, 0);
}

bool Gfx_PickupItemAnimate(u8 itemId) // 0x80054AD8
{
    q19_12         scale;
    s16            rotX;
    s16            rotZ;
    GsDOBJ2*       obj;
    GsCOORD2PARAM* transform;

    g_Items_Coords[9].coord.t[1] = Q8(0.25f);
    g_Items_Coords[9].coord.t[0] = Q8(0.0f);
    g_Items_Coords[9].coord.t[2] = Q8(-32.6875);

    switch (g_Items_PickupAnimState)
    {
        case 0: // Expand without rotation?
            if (g_Items_PickupScale >= Q12(0.5f))
            {
                g_Items_PickupAnimState = 1;
            }

            transform = &g_Items_Transforms[0];
            scale     = g_Items_PickupScale << 12;

            rotX = INV_ITEM_ROTATIONS[itemId - 32].vx;
            rotZ = INV_ITEM_ROTATIONS[itemId - 32].vy;

            // Double scale via `<< 12` then `>> 11`.
            transform[9].scale.vz = scale >> 11;
            transform[9].scale.vy = scale >> 11;
            transform[9].scale.vx = scale >> 11;

            transform[9].rotate.vx = rotX;
            transform[9].rotate.vy = Q12_ANGLE(0.0f);
            transform[9].rotate.vz = rotZ;
            break;

        case 1: // Snap to full scale?
            g_Items_Transforms[9].scale.vz = Q12(1.0f);
            g_Items_Transforms[9].scale.vy = Q12(1.0f);
            g_Items_Transforms[9].scale.vx = Q12(1.0f);
            g_Items_PickupAnimState = 2;
            break;
    }

    // Scale.
    g_Items_PickupScale += g_DeltaTimeRaw * 2;
    g_Items_PickupScale  = CLAMP(g_Items_PickupScale, Q12(0.0f), Q12(0.5f));

    PushMatrix();
    func_8004BBF4(&D_800C3B48, &D_800C3AE8, &D_800C3B38);

    obj = &D_800C3E08;

    ItemScreen_ItemRotate(&g_Items_Coords[9].param->rotate, &g_Items_Coords[9]);

    // Rotate 180 degrees per second.
    g_Items_Transforms[9].rotate.vy += g_DeltaTimeRaw >> 1;

    func_800548D8(9);
    GsSetFlatLight(0, &D_800C3AC8[0]);
    GsSetFlatLight(1, &D_800C3AC8[1]);
    func_8004BD74(9, obj, 2);
    PopMatrix();

    return g_Items_PickupAnimState > 0;
}

void Inventory_AddSpecialItem(u8 itemId, u8 itemCount) // 0x80054CAC
{
    switch (itemId)
    {
        case InvItemId_RockDrill:
        case InvItemId_Chainsaw:
        case InvItemId_Katana:
        case InvItemId_HyperBlaster:
        case InvItemId_GasolineTank:
            g_SavegamePtr->pickedUpSpecialItemCount++;
            break;

        case InvItemId_ChannelingStone:
            if (!(g_SavegamePtr->inventoryItemFlags & InvItemFlag_ChannelingStone))
            {
                g_SavegamePtr->pickedUpSpecialItemCount++;
                g_SavegamePtr->inventoryItemFlags |= InvItemFlag_ChannelingStone; // `specialItemFlags`?
            }
            break;

        case InvItemId_PlateOfTurtle:
            if (!(g_SavegamePtr->inventoryItemFlags & InvItemFlag_PlateOfTurtle))
            {
                g_SavegamePtr->pickedUpItemCount++;
                g_SavegamePtr->inventoryItemFlags |= InvItemFlag_PlateOfTurtle;
            }
            break;

        case InvItemId_PlateOfHatter:
            if (!(g_SavegamePtr->inventoryItemFlags & InvItemFlag_PlateOfHatter))
            {
                g_SavegamePtr->pickedUpItemCount++;
                g_SavegamePtr->inventoryItemFlags |= InvItemFlag_PlateOfHatter;
            }
            break;

        case InvItemId_PlateOfCat:
            if (!(g_SavegamePtr->inventoryItemFlags & InvItemFlag_PlateOfCat))
            {
                g_SavegamePtr->pickedUpItemCount++;
                g_SavegamePtr->inventoryItemFlags |= InvItemFlag_PlateOfCat;
            }
            break;

        case InvItemId_PlateOfQueen:
            if (!(g_SavegamePtr->inventoryItemFlags & InvItemFlag_PlateOfQueen))
            {
                g_SavegamePtr->pickedUpItemCount++;
                g_SavegamePtr->inventoryItemFlags |= InvItemFlag_PlateOfQueen;
            }
            break;

        case InvItemId_Lighter:
            if (!(g_SavegamePtr->inventoryItemFlags & InvItemFlag_Lighter))
            {
                g_SavegamePtr->pickedUpItemCount++;
                g_SavegamePtr->inventoryItemFlags |= InvItemFlag_Lighter;
            }
            break;

        case InvItemId_VideoTape:
            if (!(g_SavegamePtr->inventoryItemFlags & InvItemFlag_VideoTape))
            {
                g_SavegamePtr->pickedUpItemCount++;
                g_SavegamePtr->inventoryItemFlags |= InvItemFlag_VideoTape;
            }
            break;

        case InvItemId_Camera:
            if (!(g_SavegamePtr->inventoryItemFlags & InvItemFlag_Camera))
            {
                g_SavegamePtr->pickedUpItemCount++;
                g_SavegamePtr->inventoryItemFlags |= InvItemFlag_Camera;
            }
            break;

        default:
            g_SavegamePtr->pickedUpItemCount++;
            break;
    }

    g_SavegamePtr->items[g_SavegamePtr->invSlotCount].id    = itemId;
    g_SavegamePtr->items[g_SavegamePtr->invSlotCount].count = itemCount;
    g_SavegamePtr->invSlotCount                             = func_8004F190(g_SavegamePtr);
}

void Items_AmmoReloadCompute(s32* currentAmmo, s32* availableAmmo, u8 gunIdx) // 0x80054FC0
{
    s32 missingAmmo;

    missingAmmo = g_Items_GunsMaxLoadAmmo[gunIdx] - *currentAmmo;
    if (missingAmmo == 0)
    {
        return;
    }

    if (*availableAmmo >= missingAmmo)
    {
        *currentAmmo   += missingAmmo;
        *availableAmmo -= missingAmmo;
        return;
    }
    else
    {
        *currentAmmo  += *availableAmmo;
        *availableAmmo = 0;
        return;
    }
}

// Likely the first pad is supposed to be generated during file linking, while the second is inserted at
// `bodyprog_80055028.c` directly.
const s32 rodataPad_80028524 = 0;
const s32 rodataPad_80028528 = 0;
 