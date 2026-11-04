#ifndef _MAPS_CHARACTERS_CYBIL_H
#define _MAPS_CHARACTERS_CYBIL_H

/** @brief Cybil character control states. */
typedef enum _CybilControl
{
    CybilControl_None = 0,
    CybilControl_1    = 1,
    CybilControl_2    = 2,
    CybilControl_3    = 3,
    CybilControl_4    = 4,
    CybilControl_5    = 5,
    CybilControl_6    = 6,
    CybilControl_7    = 7,
    CybilControl_8    = 8,
    CybilControl_9    = 9,
    CybilControl_10   = 10,
    CybilControl_11   = 11,
    CybilControl_12   = 12,
    CybilControl_13   = 13,
    CybilControl_14   = 14,
    CybilControl_15   = 15,
    CybilControl_16   = 16,
    CybilControl_17   = 17,
    CybilControl_18   = 18,
    CybilControl_19   = 19,
    CybilControl_20   = 20,
    CybilControl_21   = 21,
    CybilControl_22   = 22,
    CybilControl_23   = 23,
    CybilControl_24   = 24,
    CybilControl_25   = 25,
    CybilControl_26   = 26,
    CybilControl_27   = 27,
    CybilControl_28   = 28,
    CybilControl_29   = 29,
    CybilControl_30   = 30
} e_CybilControl;

/** @brief Cybil character animation indices. */
typedef enum _CybilAnim
{
    CybilAnim_Still = 0,
    CybilAnim_1     = 1,
    CybilAnim_2     = 2,
    CybilAnim_3     = 3,
    CybilAnim_4     = 4,
    CybilAnim_5     = 5,
    CybilAnim_6     = 6,
    CybilAnim_7     = 7,
    CybilAnim_8     = 8,
    CybilAnim_9     = 9,
    CybilAnim_10    = 10,
    CybilAnim_11    = 11,
    CybilAnim_12    = 12,
    CybilAnim_13    = 13,
    CybilAnim_14    = 14,
    CybilAnim_15    = 15,
    CybilAnim_16    = 16,
    CybilAnim_17    = 17,
    CybilAnim_18    = 18,
    CybilAnim_19    = 19,
    CybilAnim_20    = 20,
    CybilAnim_21    = 21,
    CybilAnim_22    = 22,
    CybilAnim_23    = 23,
    CybilAnim_24    = 24,
    CybilAnim_25    = 25,
    CybilAnim_26    = 26,
    CybilAnim_27    = 27,
    CybilAnim_28    = 28,
    CybilAnim_29    = 29,
    CybilAnim_30    = 30,
    CybilAnim_31    = 31
} e_CybilAnim;

/** @brief Cybil character bone indices. */
typedef enum _CybilBone
{
    CybilBone_Root = 0
} e_CybilBone;

/** @brief Cybil character swappable mesh IDs. */
typedef enum _CybilSwappableMesh
{
    CybilSwappableMesh_None = 0,
    CybilSwappableMesh_1    = 1,
    CybilSwappableMesh_2    = 2
} e_CybilSwappableMesh;

/** @brief Cybil character variant mesh IDs. */
typedef enum _CybilVariantMesh
{
    CybilVariantMesh_None = 0,
    CybilVariantMesh_1    = 1,
    CybilVariantMesh_2    = 2
} e_CybilVariantMesh;

/** @brief Cybil character anim infos. */
extern s_AnimInfo CYBIL_ANIM_INFOS[];

/*s_AnimInfo CYBIL_ANIM_INFOS[] = {
    { Anim_BlendLinear, ANIM_STATUS(CybilAnim_Still,  false), false, ANIM_STATUS(CybilAnim_Still,  false), { Q12(0.0f)   }, NO_VALUE, 0   },
    { Anim_PlayLoop, ANIM_STATUS(CybilAnim_Still,  true),  false, NO_VALUE,               { Q12(30.0f)  }, NO_VALUE, 1   },
    { Anim_BlendLinear, ANIM_STATUS(CybilAnim_1,  false), false, ANIM_STATUS(CybilAnim_1,  true),  { Q12(5.0f)   }, NO_VALUE, 0   },
    { Anim_PlayLoop, ANIM_STATUS(CybilAnim_1,  true),  false, NO_VALUE,               { Q12(5.0f)   }, 0,        15  },
    { Anim_BlendLinear, ANIM_STATUS(CybilAnim_2,  false), false, ANIM_STATUS(CybilAnim_2,  true),  { Q12(5.0f)   }, NO_VALUE, 16  },
    { Anim_PlayLoop, ANIM_STATUS(CybilAnim_2,  true),  false, ANIM_STATUS(CybilAnim_2,  true),  { Q12(1.0f)   }, 16,       21  },
    { Anim_BlendLinear, ANIM_STATUS(CybilAnim_3,  false), false, ANIM_STATUS(CybilAnim_3,  true),  { Q12(100.0f) }, NO_VALUE, 22  },
    { Anim_PlayLoop, ANIM_STATUS(CybilAnim_3,  true),  false, NO_VALUE,               { Q12(20.0f)  }, 22,       49  },
    { Anim_BlendLinear, ANIM_STATUS(CybilAnim_4,  false), false, ANIM_STATUS(CybilAnim_4,  true),  { Q12(5.0f)   }, NO_VALUE, 50  },
    { Anim_PlayLoop, ANIM_STATUS(CybilAnim_4,  true),  false, ANIM_STATUS(CybilAnim_4,  true),  { Q12(15)  }, 50,       62  },
    { Anim_BlendLinear, ANIM_STATUS(CybilAnim_5,  false), false, ANIM_STATUS(CybilAnim_5,  true),  { Q12(5)   }, NO_VALUE, 63  },
    { Anim_PlayLoop, ANIM_STATUS(CybilAnim_5,  true),  false, ANIM_STATUS(CybilAnim_5,  true),  { Q12(15.0f)  }, 63,       75  },
    { Anim_BlendLinear, ANIM_STATUS(CybilAnim_6,  false), false, ANIM_STATUS(CybilAnim_6,  true),  { Q12(5.0f)   }, NO_VALUE, 76  },
    { Anim_PlayLoop, ANIM_STATUS(CybilAnim_6,  true),  false, NO_VALUE,               { Q12(1.0f)   }, 76,       79  },
    { Anim_BlendLinear, ANIM_STATUS(CybilAnim_7,  false), false, ANIM_STATUS(CybilAnim_7,  true),  { Q12(10.0f)  }, NO_VALUE, 80  },
    { Anim_PlayOnce, ANIM_STATUS(CybilAnim_7,  true),  false, ANIM_STATUS(CybilAnim_7,  true),  { Q12(3.0f)   }, 80,       108 },
    { Anim_BlendLinear, ANIM_STATUS(CybilAnim_8,  false), false, ANIM_STATUS(CybilAnim_8,  true),  { Q12(10.0f)  }, NO_VALUE, 109 },
    { Anim_PlayOnce, ANIM_STATUS(CybilAnim_8,  true),  false, ANIM_STATUS(CybilAnim_8,  true),  { Q12(10.0f)  }, 109,      122 },
    { Anim_BlendLinear, ANIM_STATUS(CybilAnim_9,  false), false, ANIM_STATUS(CybilAnim_9,  true),  { Q12(10.0f)  }, NO_VALUE, 123 },
    { Anim_PlayOnce, ANIM_STATUS(CybilAnim_9,  true),  false, ANIM_STATUS(CybilAnim_9,  true),  { Q12(18.0f)  }, 123,      151 },
    { Anim_BlendLinear, ANIM_STATUS(CybilAnim_10, false), false, ANIM_STATUS(CybilAnim_10, true),  { Q12(10.0f)  }, NO_VALUE, 152 },
    { Anim_PlayOnce, ANIM_STATUS(CybilAnim_10, true),  false, ANIM_STATUS(CybilAnim_10, true),  { Q12(7.0f)   }, 152,      162 },
    { Anim_BlendLinear, ANIM_STATUS(CybilAnim_11, false), false, ANIM_STATUS(CybilAnim_11, true),  { Q12(10.0f)  }, NO_VALUE, 163 },
    { Anim_PlayOnce, ANIM_STATUS(CybilAnim_11, true),  false, ANIM_STATUS(CybilAnim_11, true),  { Q12(12.0f)  }, 163,      173 },
    { Anim_BlendLinear, ANIM_STATUS(CybilAnim_12, false), false, ANIM_STATUS(CybilAnim_12, true),  { Q12(10.0f)  }, NO_VALUE, 174 },
    { Anim_PlayLoop, ANIM_STATUS(CybilAnim_12, true),  false, NO_VALUE,               { Q12(1.0f)   }, 174,      180 },
    { Anim_BlendLinear, ANIM_STATUS(CybilAnim_13, false), false, ANIM_STATUS(CybilAnim_13, true),  { Q12(10.0f)  }, NO_VALUE, 181 },
    { Anim_PlayOnce, ANIM_STATUS(CybilAnim_13, true),  false, ANIM_STATUS(CybilAnim_13, true),  { Q12(6.0f)   }, 181,      228 },
    { Anim_BlendLinear, ANIM_STATUS(CybilAnim_14, false), false, ANIM_STATUS(CybilAnim_14, true),  { Q12(10.0f)  }, NO_VALUE, 229 },
    { Anim_PlayOnce, ANIM_STATUS(CybilAnim_14, true),  false, ANIM_STATUS(CybilAnim_14, true),  { Q12(10.0f)  }, 229,      249 },
    { Anim_BlendLinear, ANIM_STATUS(CybilAnim_15, false), false, ANIM_STATUS(CybilAnim_15, true),  { Q12(10.0f)  }, NO_VALUE, 250 },
    { Anim_PlayOnce, ANIM_STATUS(CybilAnim_15, true),  false, ANIM_STATUS(CybilAnim_15, true),  { Q12(10.0f)  }, 250,      270 },
    { Anim_BlendLinear, ANIM_STATUS(CybilAnim_16, false), false, ANIM_STATUS(CybilAnim_16, true),  { Q12(10.0f)  }, NO_VALUE, 271 },
#if defined(MAP6_S01)
    { Anim_PlayOnce, ANIM_STATUS(CybilAnim_16, true),  false, ANIM_STATUS(CybilAnim_16, true),  { Q12(10.0f)  }, 271,      276 },
    { Anim_BlendLinear, ANIM_STATUS(CybilAnim_17, false), false, ANIM_STATUS(CybilAnim_17, true),  { Q12(10.0f)  }, NO_VALUE, 284 },
    { Anim_PlayOnce, ANIM_STATUS(CybilAnim_17, true),  false, ANIM_STATUS(CybilAnim_17, true),  { Q12(10.0f)  }, 284,      292 },
#elif defined(MAP7_S03)
    { Anim_PlayOnce, ANIM_STATUS(CybilAnim_16, true),  false, ANIM_STATUS(CybilAnim_16, true),  { Q12(10.0f) }, 271,       281 },
    { Anim_BlendLinear, ANIM_STATUS(CybilAnim_17, false), false, ANIM_STATUS(CybilAnim_17, true),  { Q12(10.0f) }, NO_VALUE,  282 },
    { Anim_PlayOnce, ANIM_STATUS(CybilAnim_17, true),  false, ANIM_STATUS(CybilAnim_17, true),  { Q12(10.0f) }, 282,       292 },
#endif
    { Anim_BlendLinear, ANIM_STATUS(CybilAnim_18, false), false, ANIM_STATUS(CybilAnim_18, true),  { Q12(10.0f)  }, NO_VALUE, 293 },
    { Anim_PlayOnce, ANIM_STATUS(CybilAnim_18, true),  false, ANIM_STATUS(CybilAnim_18, true),  { Q12(10.0f)  }, 293,      302 },
    { Anim_BlendLinear, ANIM_STATUS(CybilAnim_19, false), false, ANIM_STATUS(CybilAnim_19, true),  { Q12(10.0f)  }, NO_VALUE, 303 },
    { Anim_PlayOnce, ANIM_STATUS(CybilAnim_19, true),  false, ANIM_STATUS(CybilAnim_19, true),  { Q12(10.0f)  }, 303,      368 },
    { Anim_BlendLinear, ANIM_STATUS(CybilAnim_20, false), false, ANIM_STATUS(CybilAnim_20, true),  { Q12(10.0f)  }, NO_VALUE, 369 },
    { Anim_PlayOnce, ANIM_STATUS(CybilAnim_20, true),  false, ANIM_STATUS(CybilAnim_20, true),  { Q12(10.0f)  }, 369,      403 },
    { Anim_BlendLinear, ANIM_STATUS(CybilAnim_21, false), false, ANIM_STATUS(CybilAnim_21, true),  { Q12(5.0f)   }, NO_VALUE, 0   },
    { Anim_PlayLoop, ANIM_STATUS(CybilAnim_21, true),  false, NO_VALUE,               { Q12(20.0f)  }, 0,        21  },
    { Anim_BlendLinear, ANIM_STATUS(CybilAnim_22, false), false, ANIM_STATUS(CybilAnim_22, true),  { Q12(10.0f)  }, NO_VALUE, 22  },
    { Anim_PlayOnce, ANIM_STATUS(CybilAnim_22, true),  false, ANIM_STATUS(CybilAnim_22, true),  { Q12(10.0f)  }, 22,       104 },
    { Anim_BlendLinear, ANIM_STATUS(CybilAnim_23, false), false, ANIM_STATUS(CybilAnim_23, true),  { Q12(10.0f)  }, NO_VALUE, 105 },
    { Anim_PlayOnce, ANIM_STATUS(CybilAnim_23, true),  false, ANIM_STATUS(CybilAnim_23, true),  { Q12(10.0f)  }, 105,      144 },
    { Anim_BlendLinear, ANIM_STATUS(CybilAnim_24, false), false, ANIM_STATUS(CybilAnim_24, true),  { Q12(10.0f)  }, NO_VALUE, 155 },
    { Anim_PlayOnce, ANIM_STATUS(CybilAnim_24, true),  false, ANIM_STATUS(CybilAnim_24, true),  { Q12(10.0f)  }, 155,      184 },
    { Anim_BlendLinear, ANIM_STATUS(CybilAnim_25, false), false, ANIM_STATUS(CybilAnim_25, true),  { Q12(10.0f)  }, NO_VALUE, 185 },
    { Anim_PlayOnce, ANIM_STATUS(CybilAnim_25, true),  false, ANIM_STATUS(CybilAnim_25, true),  { Q12(10.0f)  }, 185,      278 },
    { Anim_BlendLinear, ANIM_STATUS(CybilAnim_26, false), false, ANIM_STATUS(CybilAnim_26, true),  { Q12(10.0f)  }, NO_VALUE, 279 },
    { Anim_PlayOnce, ANIM_STATUS(CybilAnim_26, true),  false, ANIM_STATUS(CybilAnim_26, true),  { Q12(10.0f)  }, 279,      295 },
    { Anim_BlendLinear, ANIM_STATUS(CybilAnim_27, false), false, ANIM_STATUS(CybilAnim_27, true),  { Q12(10.0f)  }, NO_VALUE, 296 },
    { Anim_PlayOnce, ANIM_STATUS(CybilAnim_27, true),  false, ANIM_STATUS(CybilAnim_27, true),  { Q12(10.0f)  }, 296,      310 },
    { Anim_BlendLinear, ANIM_STATUS(CybilAnim_28, false), false, ANIM_STATUS(CybilAnim_28, true),  { Q12(10.0f)  }, NO_VALUE, 311 },
    { Anim_PlayOnce, ANIM_STATUS(CybilAnim_28, true),  false, ANIM_STATUS(CybilAnim_28, true),  { Q12(10.0f)  }, 311,      324 },
    { Anim_BlendLinear, ANIM_STATUS(CybilAnim_29, false), false, ANIM_STATUS(CybilAnim_29, true),  { Q12(10.0f)  }, NO_VALUE, 325 },
    { Anim_PlayOnce, ANIM_STATUS(CybilAnim_29, true),  false, ANIM_STATUS(CybilAnim_29, true),  { Q12(10.0f)  }, 325,      355 },
    { Anim_BlendLinear, ANIM_STATUS(CybilAnim_30, false), false, ANIM_STATUS(CybilAnim_30, true),  { Q12(10.0f)  }, NO_VALUE, 167 },
    { Anim_PlayOnce, ANIM_STATUS(CybilAnim_30, true),  false, ANIM_STATUS(CybilAnim_30, true),  { Q12(10.0f)  }, 167,      184 },
    { Anim_BlendLinear, ANIM_STATUS(CybilAnim_31, false), false, ANIM_STATUS(CybilAnim_31, true),  { Q12(5.0f)   }, NO_VALUE, 356 },
    { Anim_PlayLoop, ANIM_STATUS(CybilAnim_31, true),  false, NO_VALUE,               { Q12(2.0f)   }, 356,      371 }
};*/

// Two variables used by `Cybil_Init`.
extern s32 sharedData_800E2378_0_s01;
extern s32 sharedData_800E237C_0_s01;

void Cybil_Update(s_SubCharacter* cybil, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);
void Cybil_AnimUpdate(s_SubCharacter* cybil, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);
void Cybil_MovementUpdate(s_SubCharacter* cybil, GsCOORDINATE2* boneCoords);
void Cybil_AnimStateUpdate(s_SubCharacter* cybil, GsCOORDINATE2* boneCoords);
void Cybil_Init(s_SubCharacter* cybil);

#endif
