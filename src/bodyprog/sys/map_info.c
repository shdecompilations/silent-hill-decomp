#include "bodyprog/bodyprog.h"
#include "bodyprog/math/math.h"
#include "game.h"

s_MapEffectsInfo MAP_EFFECTS_INFOS[21] = {
    {
        .flags                  = SpecialEnvEventFlags_None,
        .field_1                = 0xFF,
        .gfxEnum                = UnkGfxEnum_0,
        .field_3                = 0,
        .spotLightIntensity     = Q12(0.0f),
        .worldLightIntensity    = Q12(1.15f),
        .worldTintR             = Q12(1.0f),
        .worldTintG             = Q12(1.0f),
        .worldTintB             = Q12(1.0f),
        .field_E                = 0,
        .fogDistance            = Q12(100.0f),
        .fogColor               = { 60, 60, 68, 0 },
        .enableTintLightOverlap = false,
        .pointLightTint         = { 0, 0, 0, 0 },
        .worldTint              = { 0, 0, 0, 0 },
        .field_21               = { 64, 64, 64, 0 },
        .field_25               = { 12, 12, 28, 0 }
    },
    {
        .flags                  = SpecialEnvEventFlags_None,
        .field_1                = 0xFF,
        .gfxEnum                = UnkGfxEnum_0,
        .field_3                = 0,
        .spotLightIntensity     = Q12(0.0f),
        .worldLightIntensity    = Q12(1.1f),
        .worldTintR             = Q12(0.95f),
        .worldTintG             = Q12(1.0f),
        .worldTintB             = Q12(1.08f),
        .field_E                = 1,
        .fogDistance            = Q12(13.5f),
        .fogColor               = { 108, 100, 116, 0 },
        .enableTintLightOverlap = false,
        .pointLightTint         = { 0, 0, 0, 0 },
        .worldTint              = { 0, 0, 0, 0 },
        .field_21               = { 48, 48, 48, 0 },
        .field_25               = { 14, 14, 30, 0 }
    },
    {
        .flags                  = SpecialEnvEventFlags_None,
        .field_1                = 0xFF,
        .gfxEnum                = UnkGfxEnum_2,
        .field_3                = 0,
        .spotLightIntensity     = Q12(0.6f),
        .worldLightIntensity    = Q12(0.6f),
        .worldTintR             = Q12(0.9f),
        .worldTintG             = Q12(1.0f),
        .worldTintB             = Q12(1.05f),
        .field_E                = 1,
        .fogDistance            = Q12(14.0f),
        .fogColor               = { 100, 92, 108, 0 },
        .enableTintLightOverlap = false,
        .pointLightTint         = { 0, 0, 0, 0 },
        .worldTint              = { 0, 0, 0, 0 },
        .field_21               = { 48, 48, 48, 0 },
        .field_25               = { 13, 13, 29, 0 }
    },
    {
        .flags                  = (SpecialEnvEventFlags_FlashlightAllowed | SpecialEnvEventFlags_EnableBrightness | SpecialEnvEventFlags_EnableLensflare),
        .field_1                = 0xFF,
        .gfxEnum                = UnkGfxEnum_1,
        .field_3                = 0,
        .spotLightIntensity     = Q12(1.3f),
        .worldLightIntensity    = Q12(0.0f),
        .worldTintR             = Q12(0.95f),
        .worldTintG             = Q12(1.0f),
        .worldTintB             = Q12(1.08f),
        .field_E                = 0,
        .fogDistance            = Q12(CHUNK_SIZE),
        .fogColor               = { 0, 0, 0, 0 },
        .enableTintLightOverlap = false,
        .pointLightTint         = { 0, 0, 0, 0 },
        .worldTint              = { 0, 0, 0, 0 },
        .field_21               = { 48, 48, 48, 0 },
        .field_25               = { 12, 12, 12, 0 }
    },
    {
        .flags                  = (SpecialEnvEventFlags_FlashlightAllowed | SpecialEnvEventFlags_EnableBrightness | SpecialEnvEventFlags_EnableLensflare),
        .field_1                = 0xFF,
        .gfxEnum                = UnkGfxEnum_1,
        .field_3                = 0,
        .spotLightIntensity     = Q12(0.7f),
        .worldLightIntensity    = Q12(0.0f),
        .worldTintR             = Q12(0.9f),
        .worldTintG             = Q12(1.0f),
        .worldTintB             = Q12(1.05f),
        .field_E                = 0,
        .fogDistance            = Q12(CHUNK_SIZE),
        .fogColor               = { 0, 0, 0, 0 },
        .enableTintLightOverlap = false,
        .pointLightTint         = { 0, 0, 0, 0 },
        .worldTint              = { 0, 0, 0, 0 },
        .field_21               = { 48, 48, 48, 0 },
        .field_25               = { 12, 12, 12, 0 }
    },
    {
        .flags                  = (SpecialEnvEventFlags_FlashlightAllowed | SpecialEnvEventFlags_EnableBrightness | SpecialEnvEventFlags_EnableLensflare),
        .field_1                = 0xFF,
        .gfxEnum                = UnkGfxEnum_1,
        .field_3                = 0,
        .spotLightIntensity     = Q12(1.3f),
        .worldLightIntensity    = Q12(0.1f),
        .worldTintR             = Q12(0.95f),
        .worldTintG             = Q12(1.0f),
        .worldTintB             = Q12(1.08f),
        .field_E                = 3,
        .fogDistance            = Q12(13.5f),
        .fogColor               = { 68, 60, 76, 0 },
        .enableTintLightOverlap = true,
        .pointLightTint         = { 24, 16, 32, 0 },
        .worldTint              = { 85, 75, 75, 0 },
        .field_21               = { 32, 32, 40, 0 },
        .field_25               = { 12, 12, 28, 0 }
    },
    {
        .flags                  = (SpecialEnvEventFlags_DarkEnvironment | SpecialEnvEventFlags_FlashlightAllowed | SpecialEnvEventFlags_EnableBrightness),
        .field_1                = 0xFF,
        .gfxEnum                = UnkGfxEnum_0,
        .field_3                = 0,
        .spotLightIntensity     = Q12(0.0f),
        .worldLightIntensity    = Q12(0.44f),
        .worldTintR             = Q12(0.4f),
        .worldTintG             = Q12(0.7f),
        .worldTintB             = Q12(1.0f),
        .field_E                = 1,
        .fogDistance            = Q12(14.0f),
        .fogColor               = { 0, 0, 0, 0 },
        .enableTintLightOverlap = false,
        .pointLightTint         = { 0, 0, 0, 0 },
        .worldTint              = { 0, 0, 0, 0 },
        .field_21               = { 16, 16, 24, 0 },
        .field_25               = { 8, 8, 20, 0 }
    },
    {
        .flags                  = (SpecialEnvEventFlags_DarkEnvironment | SpecialEnvEventFlags_FlashlightAllowed | SpecialEnvEventFlags_EnableBrightness),
        .field_1                = 0xFF,
        .gfxEnum                = UnkGfxEnum_0,
        .field_3                = 0,
        .spotLightIntensity     = Q12(0.0f),
        .worldLightIntensity    = Q12(0.44f),
        .worldTintR             = Q12(0.4f),
        .worldTintG             = Q12(0.7f),
        .worldTintB             = Q12(1.0f),
        .field_E                = 1,
        .fogDistance            = Q12(9.29987f),
        .fogColor               = { 0, 0, 0, 0 },
        .enableTintLightOverlap = false,
        .pointLightTint         = { 0, 0, 0, 0 },
        .worldTint              = { 0, 0, 0, 0 },
        .field_21               = { 16, 16, 24, 0 },
        .field_25               = { 8, 8, 20, 0 }
    },
    {
        .flags                  = (SpecialEnvEventFlags_FlashlightAllowed | SpecialEnvEventFlags_EnableBrightness),
        .field_1                = 0x50,
        .gfxEnum                = UnkGfxEnum_1,
        .field_3                = 0,
        .spotLightIntensity     = Q12(0.35f),
        .worldLightIntensity    = Q12(0.4f),
        .worldTintR             = Q12(0.7f),
        .worldTintG             = Q12(0.8f),
        .worldTintB             = Q12(1.0f),
        .field_E                = 1,
        .fogDistance            = Q12(CHUNK_SIZE),
        .fogColor               = { 0, 0, 0, 0 },
        .enableTintLightOverlap = false,
        .pointLightTint         = { 0, 0, 0, 0 },
        .worldTint              = { 0, 0, 0, 0 },
        .field_21               = { 64, 64, 64, 0 },
        .field_25               = { 12, 12, 12, 0 }
    },
    {
        .flags                  = (SpecialEnvEventFlags_FlashlightAllowed | SpecialEnvEventFlags_EnableBrightness),
        .field_1                = 0xFF,
        .gfxEnum                = UnkGfxEnum_1,
        .field_3                = 0,
        .spotLightIntensity     = Q12(1.8f),
        .worldLightIntensity    = Q12(0.15f),
        .worldTintR             = Q12(1.0f),
        .worldTintG             = Q12(0.7f),
        .worldTintB             = Q12(0.6f),
        .field_E                = 0,
        .fogDistance            = Q12(CHUNK_SIZE),
        .fogColor               = { 0, 0, 0, 0 },
        .enableTintLightOverlap = false,
        .pointLightTint         = { 0, 0, 0, 0 },
        .worldTint              = { 0, 0, 0, 0 },
        .field_21               = { 64, 32, 16, 0 },
        .field_25               = { 24, 12, 12, 0 }
    },
    {
        .flags                  = (SpecialEnvEventFlags_FlashlightAllowed | SpecialEnvEventFlags_EnableBrightness | SpecialEnvEventFlags_UseLighter),
        .field_1                = 0x92,
        .gfxEnum                = UnkGfxEnum_1,
        .field_3                = 0,
        .spotLightIntensity     = Q12(0.5f),
        .worldLightIntensity    = Q12(0.0f),
        .worldTintR             = Q12(1.0f),
        .worldTintG             = Q12(0.8f),
        .worldTintB             = Q12(0.6f),
        .field_E                = 0,
        .fogDistance            = Q12(CHUNK_SIZE),
        .fogColor               = { 0, 0, 0, 0 },
        .enableTintLightOverlap = false,
        .pointLightTint         = { 0, 0, 0, 0 },
        .worldTint              = { 0, 0, 0, 0 },
        .field_21               = { 32, 24, 16, 0 },
        .field_25               = { 18, 8, 8, 0 }
    },
    {
        .flags                  = (SpecialEnvEventFlags_FlashlightAllowed | SpecialEnvEventFlags_EnableBrightness),
        .field_1                = 0xFF,
        .gfxEnum                = UnkGfxEnum_1,
        .field_3                = 0,
        .spotLightIntensity     = Q12(1.8f),
        .worldLightIntensity    = Q12(0.15f),
        .worldTintR             = Q12(1.3f),
        .worldTintG             = Q12(0.4f),
        .worldTintB             = Q12(0.2f),
        .field_E                = 0,
        .fogDistance            = Q12(CHUNK_SIZE),
        .fogColor               = { 0, 0, 0, 0 },
        .enableTintLightOverlap = false,
        .pointLightTint         = { 0, 0, 0, 0 },
        .worldTint              = { 0, 0, 0, 0 },
        .field_21               = { 64, 32, 16, 0 },
        .field_25               = { 24, 12, 12, 0 }
    },
    {
        .flags                  = (SpecialEnvEventFlags_FlashlightAllowed | SpecialEnvEventFlags_EnableBrightness | SpecialEnvEventFlags_EnableLensflare),
        .field_1                = 0xFF,
        .gfxEnum                = UnkGfxEnum_1,
        .field_3                = 0,
        .spotLightIntensity     = Q12(1.3f),
        .worldLightIntensity    = Q12(0.1f),
        .worldTintR             = Q12(0.95f),
        .worldTintG             = Q12(1.0f),
        .worldTintB             = Q12(1.08f),
        .field_E                = 0,
        .fogDistance            = Q12(CHUNK_SIZE),
        .fogColor               = { 0, 0, 0, 0 },
        .enableTintLightOverlap = false,
        .pointLightTint         = { 0, 0, 0, 0 },
        .worldTint              = { 0, 0, 0, 0 },
        .field_21               = { 48, 48, 48, 0 },
        .field_25               = { 12, 12, 12, 0 }
    },
    {
        .flags                  = SpecialEnvEventFlags_None,
        .field_1                = 0xFF,
        .gfxEnum                = UnkGfxEnum_0,
        .field_3                = 0,
        .spotLightIntensity     = Q12(0.0f),
        .worldLightIntensity    = Q12(0.78f),
        .worldTintR             = Q12(0.74f),
        .worldTintG             = Q12(0.85f),
        .worldTintB             = Q12(1.0f),
        .field_E                = 1,
        .fogDistance            = Q12(13.5f),
        .fogColor               = { 92, 84, 100, 0 },
        .enableTintLightOverlap = false,
        .pointLightTint         = { 0, 0, 0, 0 },
        .worldTint              = { 0, 0, 0, 0 },
        .field_21               = { 16, 16, 24, 0 },
        .field_25               = { 8, 8, 20, 0 }
    },
    {
        .flags                  = (SpecialEnvEventFlags_FlashlightAllowed | SpecialEnvEventFlags_EnableBrightness | SpecialEnvEventFlags_EnableLensflare),
        .field_1                = 0x38,
        .gfxEnum                = UnkGfxEnum_1,
        .field_3                = 0,
        .spotLightIntensity     = Q12(0.7f),
        .worldLightIntensity    = Q12(0.05f),
        .worldTintR             = Q12(1.1f),
        .worldTintG             = Q12(0.6f),
        .worldTintB             = Q12(0.65f),
        .field_E                = 0,
        .fogDistance            = Q12(CHUNK_SIZE),
        .fogColor               = { 0, 0, 0, 0 },
        .enableTintLightOverlap = false,
        .pointLightTint         = { 0, 0, 0, 0 },
        .worldTint              = { 0, 0, 0, 0 },
        .field_21               = { 48, 48, 48, 0 },
        .field_25               = { 12, 12, 12, 0 }
    },
    {
        .flags                  = (SpecialEnvEventFlags_FlashlightAllowed | SpecialEnvEventFlags_EnableBrightness),
        .field_1                = 0xC4,
        .gfxEnum                = UnkGfxEnum_1,
        .field_3                = 0,
        .spotLightIntensity     = Q12(1.2f),
        .worldLightIntensity    = Q12(0.2f),
        .worldTintR             = Q12(0.7f),
        .worldTintG             = Q12(0.45f),
        .worldTintB             = Q12(0.45f),
        .field_E                = 0,
        .fogDistance            = Q12(CHUNK_SIZE),
        .fogColor               = { 0, 0, 0, 0 },
        .enableTintLightOverlap = false,
        .pointLightTint         = { 0, 0, 0, 0 },
        .worldTint              = { 0, 0, 0, 0 },
        .field_21               = { 64, 32, 16, 0 },
        .field_25               = { 24, 12, 12, 0 }
    },
    {
        .flags                  = (SpecialEnvEventFlags_FlashlightAllowed | SpecialEnvEventFlags_EnableBrightness),
        .field_1                = 0xFF,
        .gfxEnum                = UnkGfxEnum_1,
        .field_3                = 0,
        .spotLightIntensity     = Q12(1.4f),
        .worldLightIntensity    = Q12(0.05f),
        .worldTintR             = Q12(0.85f),
        .worldTintG             = Q12(0.6f),
        .worldTintB             = Q12(0.6f),
        .field_E                = 0,
        .fogDistance            = Q12(CHUNK_SIZE),
        .fogColor               = { 0, 0, 0, 0 },
        .enableTintLightOverlap = false,
        .pointLightTint         = { 0, 0, 0, 0 },
        .worldTint              = { 0, 0, 0, 0 },
        .field_21               = { 64, 32, 16, 0 },
        .field_25               = { 24, 12, 12, 0 }
    },
    {
        .flags                  = SpecialEnvEventFlags_None,
        .field_1                = 0xFF,
        .gfxEnum                = UnkGfxEnum_0,
        .field_3                = 0,
        .spotLightIntensity     = Q12(0.0f),
        .worldLightIntensity    = Q12(1.0f),
        .worldTintR             = Q12(0.95f),
        .worldTintG             = Q12(0.9f),
        .worldTintB             = Q12(0.9f),
        .field_E                = 1,
        .fogDistance            = Q12(6.5f),
        .fogColor               = { 108, 100, 100, 0 },
        .enableTintLightOverlap = false,
        .pointLightTint         = { 0, 0, 0, 0 },
        .worldTint              = { 0, 0, 0, 0 },
        .field_21               = { 48, 48, 48, 0 },
        .field_25               = { 14, 14, 30, 0 }
    },
    {
        .flags                  = (SpecialEnvEventFlags_FlashlightAllowed | SpecialEnvEventFlags_EnableBrightness | SpecialEnvEventFlags_EnableLensflare),
        .field_1                = 0xFF,
        .gfxEnum                = UnkGfxEnum_1,
        .field_3                = 0,
        .spotLightIntensity     = Q12(2.5f),
        .worldLightIntensity    = Q12(0.15f),
        .worldTintR             = Q12(1.0f),
        .worldTintG             = Q12(0.7f),
        .worldTintB             = Q12(0.7f),
        .field_E                = 0,
        .fogDistance            = Q12(CHUNK_SIZE),
        .fogColor               = { 0, 0, 0, 0 },
        .enableTintLightOverlap = false,
        .pointLightTint         = { 0, 0, 0, 0 },
        .worldTint              = { 0, 0, 0, 0 },
        .field_21               = { 64, 32, 16, 0 },
        .field_25               = { 24, 12, 12, 0 }
    },
    {
        .flags                  = (SpecialEnvEventFlags_DarkEnvironment | SpecialEnvEventFlags_FlashlightAllowed | SpecialEnvEventFlags_EnableBrightness),
        .field_1                = 0xFF,
        .gfxEnum                = UnkGfxEnum_0,
        .field_3                = 0,
        .spotLightIntensity     = 0,
        .worldLightIntensity    = Q12(0.55f),
        .worldTintR             = Q12(0.5f),
        .worldTintG             = Q12(0.88f),
        .worldTintB             = Q12(1.25f),
        .field_E                = 1,
        .fogDistance            = Q12(15.0f),
        .fogColor               = { 0, 0, 0, 0 },
        .enableTintLightOverlap = false,
        .pointLightTint         = { 0, 0, 0, 0 },
        .worldTint              = { 0, 0, 0, 0 },
        .field_21               = { 16, 16, 24, 0 },
        .field_25               = { 8, 8, 20, 0 }
    },
    {
        .flags                  = (SpecialEnvEventFlags_FlashlightAllowed | SpecialEnvEventFlags_EnableBrightness | SpecialEnvEventFlags_EnableLensflare),
        .field_1                = 0xFF,
        .gfxEnum                = UnkGfxEnum_1,
        .field_3                = 0,
        .spotLightIntensity     = Q12(0.8f),
        .worldLightIntensity    = Q12(0.2f),
        .worldTintR             = Q12(0.95f),
        .worldTintG             = Q12(1.0f),
        .worldTintB             = Q12(1.08f),
        .field_E                = 0,
        .fogDistance            = Q12(CHUNK_SIZE),
        .fogColor               = { 0, 0, 0, 0 },
        .enableTintLightOverlap = false,
        .pointLightTint         = { 0, 0, 0, 0 },
        .worldTint              = { 0, 0, 0, 0 },
        .field_21               = { 48, 48, 48, 0 },
        .field_25               = { 12, 12, 12, 0 }
    }
};

const static s_WaterZone WATER_LIGHT_ZONES_0[] = {
    { true, 8, Q4(-120.0f), Q4(80.0f), Q4(-120.0f), Q4(40.0f) },
#if !VERSION_IS(JAP0)
    { true, 8, Q4(0.0f),    Q4(40.0f), Q4(0.0f),    Q4(80.0f) },
#endif
    {}
};

const static s_WaterZone WATER_LIGHT_ZONES_1[] = {
    { true, 8, Q4(-160.0f), Q4(40.0f),  Q4(-160.0f), Q4(160.0f) },
    { true, 8, Q4(-160.0f), Q4(160.0f), Q4(-160.0f), Q4(40.0f)  },
    {}
};

const static s_SpeedZone SPEED_ZONES_THR[] = {
    { SpeedZoneType_Fast,   Q4(-320.0f), Q4(280.0f), Q4(-240.0f), Q4(200.0f) },
    { SpeedZoneType_Fast,   Q4(-40.0f),  Q4(40.0f),  Q4(200.0f),  Q4(240.0f) },
    { SpeedZoneType_Normal, Q4(-280.0f), Q4(-80.0f), Q4(200.0f),  Q4(400.0f) },
    { NO_VALUE,             Q4(0.0f),    Q4(0.0f),   Q4(0.0f),    Q4(0.0f)   }
};

const static s_SpeedZone SPEED_ZONES_SP[] = {
    { SpeedZoneType_Fast,   Q4(-240.0f), Q4(80.0f),  Q4(-120.0f), Q4(40.0f)  },
    { SpeedZoneType_Fast,   Q4(-40.0f),  Q4(80.0f),  Q4(40.0f),   Q4(120.0f) },
    { SpeedZoneType_Normal, Q4(-160.0f), Q4(-80.0f), Q4(80.0f),   Q4(240.0f) },
    { SpeedZoneType_Normal, Q4(80.0f),   Q4(200.0f), Q4(-120.0f), Q4(160.0f) },
    { NO_VALUE,             Q4(0.0f),    Q4(0.0f),   Q4(0.0f),    Q4(0.0f)   }
};

// 2 bytes of padding.

const static s_SpeedZone SPEED_ZONES_RSR[] = {
    { SpeedZoneType_Fast,   Q4(-120.0f), Q4(80.0f),   Q4(-200.0f), Q4(40.0f) },
    { SpeedZoneType_Normal, Q4(-200.0f), Q4(-120.0f), Q4(-40.0f),  Q4(0.0f)  },
    { NO_VALUE,             Q4(0.0f),    Q4(0.0f),    Q4(0.0f),    Q4(0.0f)  }
};

// 2 bytes of padding.

const static s_SpeedZone SPEED_ZONES_RSU[] = {
    { SpeedZoneType_Fast,   Q4(-200.0f), Q4(0.0f),   Q4(40.0f),  Q4(160.0f) },
    { SpeedZoneType_Normal, Q4(-200.0f), Q4(-80.0f), Q4(-80.0f), Q4(40.0f)  },
    { NO_VALUE,             Q4(0.0f),    Q4(0.0f),   Q4(0.0f),   Q4(0.0f)   }
};

// 2 bytes of padding.

const static s_SpeedZone SPEED_ZONES_AP[] = {
    { SpeedZoneType_Fast,   Q4(-80.0f), Q4(40.0f), Q4(-120.0f), Q4(40.0f)  },
    { SpeedZoneType_Normal, Q4(0.0f),   Q4(40.0f), Q4(80.0f),   Q4(120.0f) },
    { NO_VALUE,             Q4(0.0f),   Q4(0.0f),  Q4(0.0f),    Q4(0.0f)   }
};

// 2 bytes of padding.

const static s_SpeedZone SPEED_ZONES_DR[] = {
    { SpeedZoneType_Normal, Q4(-800.0f), Q4(840.0f), Q4(-800.0f), Q4(840.0f) },
    { NO_VALUE,             Q4(0.0f),    Q4(0.0f),   Q4(0.0f),    Q4(0.0f)   }
};

const static s_SpeedZone SPEED_ZONES_SC[] = {
    { SpeedZoneType_Normal, Q4(-120.0f), Q4(-40.0f), Q4(40.0f),  Q4(80.0f)  },
    { SpeedZoneType_Normal, Q4(-40.0f),  Q4(0.0f),   Q4(160.0f), Q4(200.0f) },
    { SpeedZoneType_Normal, Q4(-40.0f),  Q4(0.0f),   Q4(0.0f),   Q4(40.0f)  },
    { SpeedZoneType_Normal, Q4(40.0f),   Q4(120.0f), Q4(40.0f),  Q4(80.0f)  },
    { SpeedZoneType_Normal, Q4(120.0f),  Q4(160.0f), Q4(0.0f),   Q4(40.0f)  },
    { SpeedZoneType_Normal, Q4(120.0f),  Q4(160.0f), Q4(160.0f), Q4(200.0f) },
    { SpeedZoneType_Normal, Q4(-80.0f),  Q4(0.0f),   Q4(-80.0f), Q4(0.0f)   },
    { SpeedZoneType_Normal, Q4(40.0f),   Q4(80.0f),  Q4(-40.0f), Q4(0.0f)   },
    { NO_VALUE,             Q4(0.0f),    Q4(0.0f),   Q4(0.0f),   Q4(0.0f)   }
};

// 2 bytes of padding.

const static s_SpeedZone SPEED_ZONES_SU[] = {
    { SpeedZoneType_Normal, Q4(-120.0f), Q4(-40.0f), Q4(40.0f),  Q4(80.0f)  },
    { SpeedZoneType_Normal, Q4(-40.0f),  Q4(0.0f),   Q4(160.0f), Q4(200.0f) },
    { SpeedZoneType_Normal, Q4(-40.0f),  Q4(0.0f),   Q4(0.0f),   Q4(40.0f)  },
    { SpeedZoneType_Normal, Q4(40.0f),   Q4(120.0f), Q4(40.0f),  Q4(80.0f)  },
    { SpeedZoneType_Normal, Q4(120.0f),  Q4(160.0f), Q4(0.0f),   Q4(40.0f)  },
    { SpeedZoneType_Normal, Q4(120.0f),  Q4(160.0f), Q4(160.0f), Q4(200.0f) },
    { SpeedZoneType_Normal, Q4(-80.0f),  Q4(0.0f),   Q4(-80.0f), Q4(0.0f)   },
    { SpeedZoneType_Normal, Q4(0.0f),    Q4(80.0f),  Q4(-40.0f), Q4(0.0f)   },
    { NO_VALUE,             Q4(0.0f),    Q4(0.0f),   Q4(0.0f),   Q4(0.0f)   }
};

// 2 bytes of padding.

const static s_SpeedZone SPEED_ZONES_HP[] = {
    { SpeedZoneType_Normal, Q4(0.0f), Q4(40.0f), Q4(120.0f), Q4(160.0f) },
    { SpeedZoneType_Normal, Q4(0.0f), Q4(40.0f), Q4(0.0f),   Q4(40.0f)  },
    { NO_VALUE,             Q4(0.0f), Q4(0.0f),  Q4(0.0f),   Q4(0.0f)   }
};

// 2 bytes of padding.

const static s_SpeedZone SPEED_ZONES_HU[] = {
    { SpeedZoneType_Normal, Q4(80.0f),   Q4(120.0f),  Q4(120.0f),  Q4(160.0f)  },
    { SpeedZoneType_Normal, Q4(0.0f),    Q4(40.0f),   Q4(0.0f),    Q4(40.0f)   },
    { SpeedZoneType_Normal, Q4(120.0f),  Q4(160.0f),  Q4(0.0f),    Q4(40.0f)   },
    { SpeedZoneType_Normal, Q4(-120.0f), Q4(0.0f),    Q4(80.0f),   Q4(120.0f)  },
    { SpeedZoneType_Normal, Q4(-80.0f),  Q4(0.0f),    Q4(-120.0f), Q4(-80.0f)  },
    { SpeedZoneType_Normal, Q4(-160.0f), Q4(-120.0f), Q4(-80.0f),  Q4(-40.0f)  },
    { SpeedZoneType_Normal, Q4(-160.0f), Q4(-120.0f), Q4(-160.0f), Q4(-120.0f) },
    { SpeedZoneType_Normal, Q4(0.0f),    Q4(120.0f),  Q4(-160.0f), Q4(-120.0f) },
    { SpeedZoneType_Normal, Q4(80.0f),   Q4(160.0f),  Q4(-120.0f), Q4(-80.0f)  },
    { SpeedZoneType_Normal, Q4(40.0f),   Q4(80.0f),   Q4(-80.0f),  Q4(-40.0f)  },
    { NO_VALUE,             Q4(0.0f),    Q4(0.0f),    Q4(0.0f),    Q4(0.0f)    }
};

// 2 bytes of padding.

const static s_SpeedZone SPEED_ZONES_ER[] = {
    { SpeedZoneType_Normal, Q4(-80.0f),  Q4(-40.0f), Q4(-40.0f),  Q4(0.0f)    },
    { SpeedZoneType_Normal, Q4(-120.0f), Q4(-40.0f), Q4(-120.0f), Q4(-80.0f)  },
    { SpeedZoneType_Normal, Q4(-120.0f), Q4(-80.0f), Q4(-160.0f), Q4(-120.0f) },
    { SpeedZoneType_Normal, Q4(160.0f),  Q4(200.0f), Q4(-40.0f),  Q4(0.0f)    },
    { SpeedZoneType_Normal, Q4(80.0f),   Q4(160.0f), Q4(-120.0f), Q4(-80.0f)  },
    { NO_VALUE,             Q4(0.0f),    Q4(0.0f),   Q4(0.0f),    Q4(0.0f)    }
};

const static s_SpeedZone SPEED_ZONES_XXX[] = {
    { SpeedZoneType_Fast, Q4(-800.0f), Q4(840.0f), Q4(-800.0f), Q4(840.0f) },
    { NO_VALUE,           Q4(0.0f),    Q4(0.0f),   Q4(0.0f),    Q4(0.0f)   }
};

const s_MapInfo MAP_INFOS[MapType_Count] = {
    { FILE_BG_THR_GLB_PLM, "THR", MapFlag_FourActiveChunks,                   NULL,                SPEED_ZONES_THR },
    { NO_VALUE,            "SC",  MapFlag_TwoActiveChunks | MapFlag_Interior, NULL,                SPEED_ZONES_SC  },
    { NO_VALUE,            "SU",  MapFlag_TwoActiveChunks | MapFlag_Interior, NULL,                SPEED_ZONES_SU  },
    { FILE_BG_SPR_GLB_PLM, "SPR", MapFlag_FourActiveChunks,                   NULL,                SPEED_ZONES_SP  },
    { FILE_BG_SPU_GLB_PLM, "SPU", MapFlag_FourActiveChunks,                   NULL,                SPEED_ZONES_SP  },
    { FILE_BG_RSR_GLB_PLM, "RSR", MapFlag_FourActiveChunks,                   NULL,                SPEED_ZONES_RSR },
    { FILE_BG_RSU_GLB_PLM, "RSU", MapFlag_FourActiveChunks,                   NULL,                SPEED_ZONES_RSU },
    { FILE_BG_APR_GLB_PLM, "APR", MapFlag_FourActiveChunks,                   NULL,                SPEED_ZONES_AP  },
    { FILE_BG_APU_GLB_PLM, "APU", MapFlag_FourActiveChunks,                   NULL,                SPEED_ZONES_AP  },
    { NO_VALUE,            "ER",  MapFlag_TwoActiveChunks | MapFlag_Interior, NULL,                SPEED_ZONES_ER  },
    { NO_VALUE,            "ER",  MapFlag_TwoActiveChunks | MapFlag_Interior, NULL,                SPEED_ZONES_ER  },
    { FILE_BG_DR_GLB_PLM,  "DR",  MapFlag_FourActiveChunks,                   WATER_LIGHT_ZONES_0, SPEED_ZONES_DR  },
    { FILE_BG_DRU_GLB_PLM, "DRU", MapFlag_FourActiveChunks,                   WATER_LIGHT_ZONES_1, SPEED_ZONES_DR  },
    { NO_VALUE,            "HP",  MapFlag_TwoActiveChunks | MapFlag_Interior, NULL,                SPEED_ZONES_HP  },
    { NO_VALUE,            "HU",  MapFlag_TwoActiveChunks | MapFlag_Interior, NULL,                SPEED_ZONES_HU  },
    { NO_VALUE,            "XXX", MapFlag_3,                                  NULL,                SPEED_ZONES_XXX }
};
