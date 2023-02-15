#include <events\dispatcher.h>
#include "cityTimeWeatherLightingHandler.h"

using namespace MM2;

/*
    cityTimeWeatherLightingHandler
*/

TimeWeatherInfo cityTimeWeatherLightingHandler::g_TimeWeathers[NUM_TIMEWEATHERS];
TimeWeatherInfo* cityTimeWeatherLightingHandler::TimeWeather = nullptr;

static hook::Type<int> TimeWeatherIdx = 0x62B068;

void InitTimeWeathers() {
    TimeWeatherInfo* timeWeather = cityTimeWeatherLightingHandler::g_TimeWeathers;
    dgStatePack* statePack = dgStatePack::Instance;

    for (int i = 0; i < NUM_TIMEWEATHERS; i++) {
        timeWeather->SetDefaults(statePack, i);
        timeWeather++;
    }

    // reset for the iterator
    cityTimeWeatherLightingHandler::TimeWeather = nullptr;
}

void NextTimeWeather(char* buffer, const char* format, int idx) {
    sprintf(buffer, format, idx);

    if (cityTimeWeatherLightingHandler::TimeWeather != nullptr)
        cityTimeWeatherLightingHandler::TimeWeather->Update();

    // set the next TimeWeather for FileIO to reference
    cityTimeWeatherLightingHandler::TimeWeather = &cityTimeWeatherLightingHandler::g_TimeWeathers[idx];
}

// cannot be 'bool' or else EAX will be corrupted!
BOOL CanDrawNightTrafficGlows() {
    if (cityTimeWeatherLightingHandler::TimeWeather != nullptr)
        return cityTimeWeatherLightingHandler::TimeWeather->ShowLightGlows;

    return (dgStatePack::Instance->TimeOfDay >= 2);
}

void cityTimeWeatherLightingHandler::Reset() {
    if (cityTimeWeatherLightingHandler::TimeWeather != nullptr) {
        TimeWeather->FlatColorIntensity = 1.0;
        TimeWeather->ApplyFlatColor();
    }
}

void cityTimeWeatherLightingHandler::LoadCityTimeWeatherLighting() {
    InitTimeWeathers();

    // LoadCityTimeWeatherLighting
    hook::StaticThunk<0x443530>::Call<void>();

    TimeWeather = &g_TimeWeathers[TimeWeatherIdx];
    TimeWeather->Apply();
}

void cityTimeWeatherLightingHandler::FileIO(datParser &parser) {
    // cityTimeWeatherLighting::FileIO
    hook::Thunk<0x443440>::Call<void>(this, &parser);

    // apply to the active TimeWeatherInfo
    if (TimeWeather != nullptr)
        TimeWeather->FileIO(parser);
}

void cityTimeWeatherLightingHandler::Install() {
    InstallCallback("LoadCityTimeWeatherLighting", "Allows for more control over city lighting initialization.",
        &LoadCityTimeWeatherLighting, {
            cb::call(0x44425B), // cityLevel::Load
        }
    );

    InstallPatch({
        0xEB, 0x50, // jmp 444241
        0x90,       // nop
    }, {
        0x4441EF, // cityLevel::Load
    });

    InstallPatch("aiTrafficLightInstance::DrawGlow code injection", {
        0xE8, 0xCD, 0xCD, 0xCD, 0xCD,   // call <DEADCODE>
        0x90, 0x90,                     // nop(2)

        0x0F, 0xBF, 0x4E, 0x3C,         // movsx ecx, word ptr [esi+3Ch]
    }, {
        0x53CABC, // aiTrafficLightInstance::DrawGlow
    });

    InstallCallback(&CanDrawNightTrafficGlows, {
        cb::call(0x53CABC),
    }, "aiTrafficLightInstance::DrawGlow code implementation");

    /*
        LoadCityTimeWeatherLighting hooks
    */

    // use 'sprintf' like an iterator ;)
    InstallCallback(&NextTimeWeather, {
        cb::call(0x443564),
    }, "Custom iterator in LoadCityTimeWeatherLighting.");

    // inject our custom properties into the .lt file parser
    InstallCallback(&FileIO, {
        cb::call(0x443584),
    }, "Custom FileIO for cityTimeWeatherLighting.");

    GameEventDispatcher::RegisterStateEndCallback(Reset);
}

