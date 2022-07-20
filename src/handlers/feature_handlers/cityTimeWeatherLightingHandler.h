#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

using namespace MM2;

const int NUM_TIMEWEATHERS = 16;

struct TimeWeatherInfo {
    MM2::cityTimeWeatherLighting* data;

    bool ShowHeadlights;
    bool ShowLightGlows;

    float FlatColorIntensity;
    float WeatherFriction;

    char ReflectionMap[32];
    char ShadowMap[32];
    char GlowName[32];

    void Update() {
        data->ComputeAmbientLightLevels();
    }

    void FileIO(datParser& parser) {
        parser.AddValue("Headlights", &ShowHeadlights);
        parser.AddValue("LightGlows", &ShowLightGlows);

        parser.AddValue("FlatColorIntensity", &FlatColorIntensity);
        parser.AddValue("WeatherFriction", &WeatherFriction);

        parser.AddString("ReflectionMap", &ReflectionMap);
        parser.AddString("ShadowMap", &ShadowMap);

        parser.AddString("GlowName", &GlowName);
    }

    void SetDefaults(dgStatePack* statePack, int index) {
        data = &$::timeWeathers[index];

        ShowHeadlights = (statePack->TimeOfDay >= 2 || statePack->WeatherType == 2);
        ShowLightGlows = statePack->TimeOfDay == 3;

        FlatColorIntensity = (statePack->TimeOfDay == 3) ? 0.5f : 1.0f;

        WeatherFriction = (statePack->WeatherType == 3)
            ? ((statePack->TimeOfDay == 3) ? 0.75f : 0.8f)
            : 1.0f;

        strcpy_s(ReflectionMap, "refl_dc");
        strcpy_s(ShadowMap, (statePack->TimeOfDay == 3) ? "shadmap_nite" : "shadmap_day");
        strcpy_s(GlowName, "s_yel_glow");
    }

    void Apply() {
        static hook::Type<gfxTexture*> g_GlowTexture = 0x62767C;
        static hook::Type<gfxTexture*> g_ReflectionMap = 0x628914;

        static hook::Type<float> g_FlatColorIntensity = 0x5C9DA0;
        static hook::Type<float> g_WeatherFriction = 0x5CF6B8;

        aiMap::GetInstance()->showHeadlights = ShowHeadlights;
        vehCar::sm_DrawHeadlights = ShowHeadlights;
        vehCarModel::ShowHeadlights = ShowHeadlights;

        g_FlatColorIntensity = FlatColorIntensity;
        g_WeatherFriction = WeatherFriction;

        if (MMSTATE->WeatherType == 3) {
            // jump to the part of mmGame::InitWeather that sets up birth rules
            hook::StaticThunk<0x4133D6>::Call<void>();
        }

        if (!useSoftware)
            g_ReflectionMap = gfxGetTexture(ReflectionMap);

        if (ShowLightGlows)
            g_GlowTexture = gfxGetTexture(GlowName);

        vglSetCloudMap(ShadowMap);
    }

    void ApplyFlatColor() {
        static hook::Type<float> g_FlatColorIntensity = 0x5C9DA0;

        g_FlatColorIntensity = FlatColorIntensity;
    }
};


class cityTimeWeatherLightingHandler {
public:
    static TimeWeatherInfo g_TimeWeathers[NUM_TIMEWEATHERS];
    static TimeWeatherInfo* TimeWeather;

    static void Reset();

    static void LoadCityTimeWeatherLighting();

    void FileIO(MM2::datParser &parser);

    static void Install();
};

