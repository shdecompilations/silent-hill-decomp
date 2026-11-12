#include "bodyprog/bodyprog.h"
#include "bodyprog/sys/chara_file_infos.h"
#include "bodyprog/math/math.h"
#include "game.h"

s_CharaFileInfo CHARA_FILE_INFOS[] = {
/* `Chara_` name       { `animFileIdx`          `modelFileIdx`         `textureFileIdx`       `field_6`  `materialBlendMode`      `field_8`    `cameraAnchor`          `cameraOffsetY`     } */
/* None             */ {  NO_VALUE,              FILE_1ST_2ZANKO80_TIM, FILE_1ST_2ZANKO80_TIM, Q8(0.0f),  BlendMode_Average,       NULL,        CameraAnchor_Character, Q8(0.0f)           },
/* Harry            */ {  FILE_ANIM_HB_BASE_ANM, FILE_CHARA_HERO_ILM,   FILE_CHARA_HERO_TIM,   Q8(0.25f), BlendMode_Additive,      NULL,        CameraAnchor_Character, Q8(-0.7f)          },
/* AirScreamer      */ {  FILE_ANIM_BIRD_ANM,    FILE_CHARA_BIRD_ILM,   FILE_CHARA_REBIRD_TIM, Q8(0.25f), BlendMode_Subtractive,   &D_800A90B4, CameraAnchor_Character, Q8(-0.6f)          },
/* NightFlutter     */ {  FILE_ANIM_BIRD_ANM,    FILE_CHARA_BD2_ILM,    FILE_CHARA_BD2_TIM,    Q8(0.25f), BlendMode_Subtractive,   &D_800A90B4, CameraAnchor_Character, Q8(-0.6f)          },
/* Groaner          */ {  FILE_ANIM_DOG_ANM,     FILE_CHARA_DOG_ILM,    FILE_CHARA_DOG_TIM,    Q8(0.25f), BlendMode_Subtractive,   NULL,        CameraAnchor_Character, Q8(-0.7f)          },
/* Wormhead         */ {  FILE_ANIM_DOG_ANM,     FILE_CHARA_DG2_ILM,    FILE_CHARA_DG2_TIM,    Q8(0.25f), BlendMode_Subtractive,   NULL,        CameraAnchor_Character, Q8(-0.7f)          },
/* LarvalStalker    */ {  FILE_ANIM_CLD1_ANM,    FILE_CHARA_CLD1_ILM,   FILE_CHARA_CLD1_TIM,   Q8(0.25f), BlendMode_Subtractive,   NULL,        CameraAnchor_Character, Q8(-0.5f)          },
/* Stalker          */ {  FILE_ANIM_CLD2_ANM,    FILE_CHARA_CLD2_ILM,   FILE_CHARA_CLD2_TIM,   Q8(0.25f), BlendMode_Subtractive,   NULL,        CameraAnchor_Character, Q8(-0.8f)          },
/* GreyChild        */ {  FILE_ANIM_CLD2_ANM,    FILE_CHARA_CLD3_ILM,   FILE_CHARA_CLD3_TIM,   Q8(0.25f), BlendMode_Additive,      NULL,        CameraAnchor_Character, Q8(-0.8f)          },
/* Mumbler          */ {  FILE_ANIM_CLD2_ANM,    FILE_CHARA_CLD4_ILM,   FILE_CHARA_CLD4_TIM,   Q8(0.25f), BlendMode_Additive,      NULL,        CameraAnchor_Character, Q8(-0.8f)          },
/* HangedScratcher  */ {  FILE_ANIM_SLT_ANM,     FILE_CHARA_SLT_ILM,    FILE_CHARA_SLT_TIM,    Q8(0.25f), BlendMode_Subtractive,   NULL,        CameraAnchor_Character, Q8(-0.6f)          },
/* Creeper          */ {  FILE_ANIM_COC_ANM,     FILE_CHARA_COC_ILM,    FILE_CHARA_COC_TIM,    Q8(0.25f), BlendMode_Subtractive,   NULL,        CameraAnchor_Character, Q8(-0.5f)          },
/* Romper           */ {  FILE_ANIM_JACK_ANM,    FILE_CHARA_JACK_ILM,   FILE_CHARA_JACK_TIM,   Q8(0.3f),  BlendMode_Subtractive,   &D_800A90A4, CameraAnchor_Floor,     Q8(-0.8f)          },
/* Chicken          */ {  FILE_ANIM_CKN_ANM,     FILE_CHARA_CKN_ILM,    FILE_CHARA_CKN_TIM,    Q8(0.0f),  BlendMode_Subtractive,   NULL,        CameraAnchor_Character, Q8(-0.9f)          }, /** @unused */
/* SplitHead        */ {  FILE_ANIM_FAT_ANM,     FILE_CHARA_FAT_ILM,    FILE_CHARA_FAT_TIM,    Q8(0.0f),  BlendMode_Subtractive,   NULL,        CameraAnchor_Character, Q8(-0.7f)          },
/* Floatstinger     */ {  FILE_ANIM_MTH_ANM,     FILE_CHARA_MTH_ILM,    FILE_CHARA_MOTH_TIM,   Q8(0.0f),  BlendMode_Additive,      NULL,        CameraAnchor_Character, Q8(-2.3f)          },
/* PuppetNurse      */ {  FILE_ANIM_PRS_ANM,     FILE_CHARA_PRS_ILM,    FILE_CHARA_PRS_TIM,    Q8(0.25f), BlendMode_Subtractive,   NULL,        CameraAnchor_Character, Q8(-0.9f)          },
/* DummyNurse       */ {  FILE_ANIM_DUMMY_ANM,   FILE_CHARA_DUMMY_ILM,  NO_VALUE,              Q8(0.25f), BlendMode_Subtractive,   NULL,        CameraAnchor_Character, Q8(-0.9f)          },
/* PuppetDoctor     */ {  FILE_ANIM_PRS_ANM,     FILE_CHARA_PRSD_ILM,   FILE_CHARA_PRSD_TIM,   Q8(0.25f), BlendMode_Subtractive,   NULL,        CameraAnchor_Character, Q8(-0.9f)          },
/* DummyDoctor      */ {  FILE_ANIM_DUMMY_ANM,   FILE_CHARA_DUMMY_ILM,  NO_VALUE,              Q8(0.25f), BlendMode_Subtractive,   NULL,        CameraAnchor_Character, Q8(-0.9f)          },
/* Twinfeeler       */ {  FILE_ANIM_WRM_ANM,     FILE_CHARA_WRM_ILM,    FILE_CHARA_WORM_TIM,   Q8(0.0f),  BlendMode_Subtractive,   NULL,        CameraAnchor_Character, Q8(-0.7f)          },
/* Bloodsucker      */ {  FILE_ANIM_ROD_ANM,     FILE_CHARA_ROD_ILM,    FILE_CHARA_ROD_TIM,    Q8(0.0f),  BlendMode_Subtractive,   NULL,        CameraAnchor_Character, Q8(-0.7f)          },
/* Incubus          */ {  FILE_ANIM_BOS_ANM,     FILE_CHARA_BOS_ILM,    FILE_CHARA_BOS_TIM,    Q8(0.0f),  BlendMode_Additive,      NULL,        CameraAnchor_Camera,    Q8(0.0f)           },
/* Unknown23        */ {  FILE_ANIM_MAR_ANM,     FILE_CHARA_MAR_ILM,    FILE_CHARA_MAR_TIM,    Q8(0.0f),  BlendMode_Average,       NULL,        CameraAnchor_Camera,    Q8(0.0f)           },
/* MonsterCybil     */ {  FILE_ANIM_MSB_ANM,     FILE_CHARA_MSB_ILM,    FILE_CHARA_MSB_TIM,    Q8(0.25f), BlendMode_Subtractive,   NULL,        CameraAnchor_Character, Q8(-1.35f)         },
/* LockerDeadBody   */ {  FILE_ANIM_DEAD_ANM,    FILE_CHARA_DEAD_ILM,   FILE_CHARA_DEAD_TIM,   Q8(0.25f), BlendMode_Subtractive,   NULL,        CameraAnchor_Character, Q8(-0.5f)          },
/* Cybil            */ {  FILE_ANIM_SBL_ANM,     FILE_CHARA_SIBYL_ILM,  FILE_CHARA_SIBYL_TIM,  Q8(0.3f),  BlendMode_Subtractive,   NULL,        CameraAnchor_Character, Q8(-0.5f)          },
/* EndingCybil      */ {  FILE_ANIM_SBL2_ANM,    FILE_CHARA_SIBYL_ILM,  FILE_CHARA_SIBYL_TIM,  Q8(0.25f), BlendMode_Subtractive,   NULL,        CameraAnchor_Character, Q8(-0.5f)          },
/* Cheryl           */ {  FILE_ANIM_SRL_ANM,     FILE_CHARA_SRL_ILM,    FILE_CHARA_SRL_TIM,    Q8(0.25f), BlendMode_Subtractive,   NULL,        CameraAnchor_Character, Q8(-0.5f)          },
/* Cat              */ {  FILE_ANIM_CAT_ANM,     FILE_CHARA_CAT_ILM,    FILE_CHARA_CAT_TIM,    Q8(0.25f), BlendMode_Subtractive,   NULL,        CameraAnchor_Character, Q8(-0.5f)          },
/* Dahlia           */ {  FILE_ANIM_DA_ANM,      FILE_CHARA_DARIA_ILM,  FILE_CHARA_DARIA_TIM,  Q8(0.25f), BlendMode_Additive,      NULL,        CameraAnchor_Character, Q8(-0.5f)          },
/* EndingDahlia     */ {  FILE_ANIM_DA2_ANM,     FILE_CHARA_DARIA_ILM,  FILE_CHARA_DARIA_TIM,  Q8(0.25f), BlendMode_Additive,      NULL,        CameraAnchor_Character, Q8(-0.5f)          },
/* Lisa             */ {  FILE_ANIM_LS_ANM,      FILE_CHARA_LISA_ILM,   FILE_CHARA_LISA_TIM,   Q8(0.25f), BlendMode_Subtractive,   NULL,        CameraAnchor_Character, Q8(-0.5f)          },
/* BloodyLisa       */ {  FILE_ANIM_BLS_ANM,     FILE_CHARA_BLISA_ILM,  FILE_CHARA_BLISA_TIM,  Q8(0.25f), BlendMode_Subtractive,   NULL,        CameraAnchor_Character, Q8(-0.5f)          },
/* Alessa           */ {  FILE_ANIM_AR_ANM,      FILE_CHARA_AR_ILM,     FILE_CHARA_AR_TIM,     Q8(0.25f), BlendMode_Additive,      NULL,        CameraAnchor_Character, Q8(-0.5f)          },
/* GhostChildAlessa */ {  FILE_ANIM_TAR_ANM,     FILE_CHARA_TAR_ILM,    FILE_CHARA_TAR_TIM,    Q8(0.25f), BlendMode_Additive,      NULL,        CameraAnchor_Character, Q8(-0.5f)          },
/* Incubator        */ {  FILE_ANIM_MAR_ANM,     FILE_CHARA_MAR_ILM,    FILE_CHARA_MAR_TIM,    Q8(0.25f), BlendMode_Additive,      NULL,        CameraAnchor_Character, Q8(-0.5f)          },
/* BloodyIncubator  */ {  FILE_ANIM_BAR_ANM,     FILE_CHARA_BAR_ILM,    FILE_CHARA_BAR_TIM,    Q8(0.25f), BlendMode_Additive,      NULL,        CameraAnchor_Character, Q8(-0.5f)          },
/* Kaufmann         */ {  FILE_ANIM_KAU_ANM,     FILE_CHARA_KAU_ILM,    FILE_CHARA_KAU_TIM,    Q8(0.25f), BlendMode_Additive,      NULL,        CameraAnchor_Character, Q8(-0.5f)          },
/* EndingKaufmann   */ {  FILE_ANIM_KAU2_ANM,    FILE_CHARA_KAU_ILM,    FILE_CHARA_KAU_TIM,    Q8(0.25f), BlendMode_Additive,      NULL,        CameraAnchor_Character, Q8(-0.5f)          },
/* Flauros          */ {  FILE_ANIM_BFLU_ANM,    FILE_CHARA_BFLU_ILM,   FILE_CHARA_BFLU_TIM,   Q8(0.25f), BlendMode_Additive,      NULL,        CameraAnchor_Character, Q8(0.0f)           },
/* LittleIncubus    */ {  FILE_ANIM_LITL_ANM,    FILE_CHARA_LITL_ILM,   FILE_CHARA_LITL_TIM,   Q8(0.25f), BlendMode_Additive,      NULL,        CameraAnchor_Character, Q8(0.0f)           },
/* GhostDoctor      */ {  FILE_ANIM_DOC_ANM,     FILE_CHARA_DOC_ILM,    FILE_CHARA_DOC_TIM,    Q8(0.25f), BlendMode_Additive,      NULL,        CameraAnchor_Character, Q8(0.0f)           },
/* Parasite         */ {  FILE_ANIM_ICU_ANM,     FILE_CHARA_ICU_ILM,    FILE_CHARA_ICU_TIM,    Q8(0.25f), BlendMode_Additive,      NULL,        CameraAnchor_Character, Q8(0.0f)           },
/* Padlock          */ {  FILE_1ST_2ZANKO80_TIM, FILE_1ST_2ZANKO80_TIM, FILE_1ST_2ZANKO80_TIM, Q8(0.0f),  BlendMode_Average,       NULL,        CameraAnchor_Character, Q8(0.0f)           }
};
