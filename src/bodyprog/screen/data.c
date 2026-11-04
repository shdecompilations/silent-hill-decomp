#include "bodyprog/bodyprog.h"
#include "bodyprog/screen/data.h"
#include "bodyprog/math/math.h"

q19_12 g_ScreenFadeTimestep;
s32    g_VBlanks;
s32    g_UncappedVBlanks;

s32 __pad_bss_800B5C3C;

GsOT_TAG g_OtTags0[2][16];
q19_12   g_DeltaTime;
GsOT_TAG g_OtTags1[2][ORDERING_TABLE_SIZE];

s32 __pad_bss_800B9CC4;

q19_12 g_GravitySpeed;
s32    g_TickCount;

// SH2 symbols indicate this should be in `vc_main.c`, but the file already has its data segment assigned.
// It's possible this is part of another split as there is also some inbetween data in
// the bss data segment related to the view camera system.
VC_ROAD_DATA vcNullRoadArray[] = {
    {
        .lim_sw         = { Q8(-30.0f), Q8(30.0f), Q8(-30.0f), Q8(30.0f) },
        .lim_rd         = { Q8(-30.0f), Q8(30.0f), Q8(-30.0f), Q8(30.0f) },
        .flags          = VC_RD_NOFLAG,
        .area_size_type = VC_AREA_WIDE,
        .rd_type        = VC_RD_TYPE_ROAD,
        .mv_y_type      = VC_MV_SELF_VIEW,
        .lim_rd_max_hy  = Q4(6.0f),
        .lim_rd_min_hy  = Q4(-6.0f),
        .ofs_watch_hy   = Q4(0.5f),
        .lens_flare     = LensFlareType_Custom,
        .cam_mv_type    = VC_MV_CHASE,
        .fix_ang_x      = Q8_ANGLE(0.0f),
        .fix_ang_y      = Q8_ANGLE(0.0f)
    },
    {
        .lim_sw         = { Q8(0.0f), Q8(0.0f), Q8(0.0f), Q8(0.0f) },
        .lim_rd         = { Q8(0.0f), Q8(0.0f), Q8(0.0f), Q8(0.0f) },
        .flags          = VC_RD_END_DATA_F,
        .area_size_type = VC_AREA_TINY,
        .rd_type        = VC_RD_TYPE_ROAD,
        .mv_y_type      = VC_MV_CHASE,
        .lim_rd_max_hy  = Q4(0.0f),
        .lim_rd_min_hy  = Q4(0.0f),
        .ofs_watch_hy   = Q4(0.0f),
        .lens_flare     = LensFlareType_Default,
        .cam_mv_type    = VC_MV_CHASE,
        .fix_ang_x      = Q8_ANGLE(0.0f),
        .fix_ang_y      = Q8_ANGLE(0.0f)
    }
};

GsOT g_OrderingTable0[2] = {
    { 11, &g_OtTags1[0][1], 0, 0, 0 },
    { 11, &g_OtTags1[1][1], 0, 0, 0 }
};

GsOT g_OrderingTable1[2] = {
    { 9, (GsOT_TAG*)FS_BUFFER_1, 0, 0, 0 },
    { 9, (GsOT_TAG*)0x801E2E00, 0, 0, 0 }
};

GsOT g_OrderingTable2[2] = {
    { 4, &g_OtTags0[0][0], 0, 0, 0 },
    { 4, &g_OtTags0[1][0], 0, 0, 0 }
};

q19_12 g_DeltaTimeRaw    = TIMESTEP_60_FPS;
s32    g_IntervalVBlanks = 1;

#if VERSION_REGION_IS(PAL)
// PAL image layout: different VRAM positions, `g_Font16AtlasImg` moved, and no separate death tip image.
s_FsImageDesc g_KonamiLogoImg              = { { 0,   12  }, 0,    0,   1088, 511 };
s_FsImageDesc g_KcetLogoImg                = { { 0,   14  }, 0,    0,   1104, 511 };
s_FsImageDesc g_MemCardWarningImg          = { { 0,   28  }, 0,    0,   1120, 511 };
s_FsImageDesc g_Font16AtlasImg             = { { 0,   12  }, 0,    128, 816,  255 };
s_FsImageDesc g_TitleImg                   = { { 0,   14  }, 0,    0,   832,  511 };
s_FsImageDesc g_PaperMapImg                = { { 1,   5   }, 0,    16,  1088, 511 };
s_FsImageDesc g_PaperMapMarkingAtlasImg    = { { 0,   7   }, 32,   0,   480,  255 };
s_FsImageDesc g_ItemInspectionImg          = { { 1,   5   }, 0,    16,  1088, 511 };
s_FsImageDesc D_800A9A04                   = { { 1,   21  }, 0,    0,   832,  511 };
s_FsImageDesc g_LoadingScreenImg           = { { 0,   5   }, 0,    16,  928,  448 };
s_FsImageDesc g_ControllerButtonAtlasImg   = { { 0,   7   }, 32,   0,   912,  496 };
s_FsImageDesc g_BrightnessScreenImg0       = { { 0,   28  }, 0,    16,  928,  448 };
s_FsImageDesc g_BrightnessScreenImg1       = { { 0,   30  }, 0,    16,  928,  448 };
s_FsImageDesc g_HealthPortraitImg          = { { 1,   31  }, 12,   0,   832,  511 };
s_FsImageDesc g_InventoryKeyItemTextureImg = { { 0,   14  }, 0,    0,   912,  480 };
s_FsImageDesc g_FirstAidKitItemTextureImg  = { { 0,   15  }, 0,    0,   928,  480 };
s_FsImageDesc D_800A9074                   = { { 0,   13  }, 32,   0,   896,  480 };
s_FsImageDesc D_800A907C                   = { { 0,   5   }, 0,    128, 896,  488 };
s_FsImageDesc D_800A9084                   = { { 0,   11  }, 0,    0,   944,  480 };
s_FsImageDesc D_800A908C                   = { { 0,   13  }, 0,    0,   928,  432 }; // } Effect textures.
s_FsImageDesc D_800A9094                   = { { 0,   13  }, 0,    128, 928,  440 }; // }
s_FsImageDesc g_Font24AtlasImg             = { { 0,   31  }, 0,    0,   960,  496 };
#else
s_FsImageDesc g_Font16AtlasImg             = { { 0,   16  }, 0,    240, 304, 511 };
s_FsImageDesc g_KonamiLogoImg              = { { 0,   12  }, 0,    0,   0,   0   };
s_FsImageDesc g_KcetLogoImg                = { { 0,   14  }, 0,    0,   0,   1   };
s_FsImageDesc g_MemCardWarningImg          = { { 0,   28  }, 0,    0,   0,   2   };
s_FsImageDesc g_TitleImg                   = { { 1,   13  }, 32,   0,   224, 15  };
s_FsImageDesc g_PaperMapImg                = { { 1,   5   }, 0,    16,  224, 15  };
s_FsImageDesc g_PaperMapMarkingAtlasImg    = { { 0,   7   }, 32,   0,   480, 255 };
s_FsImageDesc g_ItemInspectionImg          = { { 1,   5   }, 0,    16,  224, 14  };
s_FsImageDesc g_LoadingScreenImg           = { { 0,   5   }, 0,    16,  288, 0   }; // @unused? Uses a floor texture.
s_FsImageDesc g_ControllerButtonAtlasImg   = { { 0,   7   }, 32,   0,   464, 0   };
s_FsImageDesc g_BrightnessScreenImg0       = { { 0,   28  }, 0,    16,  288, 0   };
s_FsImageDesc g_BrightnessScreenImg1       = { { 0,   30  }, 0,    16,  288, 0   };
s_FsImageDesc g_DeathTipImg                = { { 0,   30  }, 0,    16,  288, 0   };
s_FsImageDesc g_HealthPortraitImg          = { { 1,   31  }, 12,   103, 224, 13  };
s_FsImageDesc g_InventoryKeyItemTextureImg = { { 0,   14  }, 0,    0,   160, 0   };
s_FsImageDesc g_FirstAidKitItemTextureImg  = { { 0,   15  }, 0,    0,   176, 0   };
s_FsImageDesc D_800A9074                   = { { 0,   13  }, 32,   0,   240, 0   };
s_FsImageDesc D_800A907C                   = { { 0,   5   }, 0,    128, 240, 8   };
s_FsImageDesc D_800A9084                   = { { 0,   11  }, 0,    0,   304, 0   };
s_FsImageDesc D_800A908C                   = { { 0,   13  }, 0,    0,   224, 0   }; // } Effect textures.
s_FsImageDesc D_800A9094                   = { { 0,   13  }, 0,    128, 224, 8   }; // }
s_FsImageDesc g_Font24AtlasImg             = { { 0,   31  }, 0,    0,   960, 496 };
#endif
/* Not sure if these are really `s_FsImageDesc`. Seems plausible but not sure about negative values here. */
s_FsImageDesc D_800A90A4                   = { { 0,   0   }, 35,   0,   35,  3   };
s_FsImageDesc D_800A90AC                   = { { 0,   0   }, 0,    0,   0,  -1   }; // @unused?
s_FsImageDesc D_800A90B4                   = { { 0,   0   }, 15,   0,  -53,  10  };
s_FsImageDesc D_800A90BC                   = { { 0,   0   }, 76,   0,   140, 2   }; // } @unused?
s_FsImageDesc D_800A90C4                   = { { 168, 0   }, 246,  255, 7,   13  }; // }
s_FsImageDesc D_800A90CC                   = { { 153, 0   }, 94,   0,   35,  13  }; // }
s_FsImageDesc D_800A90D4                   = { { 205, 255 }, 122,  0,   56,  12  }; // }
s_FsImageDesc D_800A90DC                   = { { 168, 0   }, 246,  255,-7,   15  }; // }
s_FsImageDesc D_800A90E4                   = { { 153, 0   }, 94,   0,  -38,  15  }; // }
s_FsImageDesc D_800A90EC                   = { { 51,  0   }, 125,  0,  -56,  14  }; // }
s_FsImageDesc D_800A90F4                   = { { 0,   0   }, 0,    0,   0,   -1  }; // }
