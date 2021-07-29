#include "cityLevelHandler.h"
#include "vglHandler.h"

using namespace MM2;

// ==========================
// Pointer hooks
// ==========================

static float ped_LodThreshold = 1225.f;

hook::Type<float> obj_NoDrawThresh(0x5C571C); // default: 300.0

hook::Type<float> obj_VLowThresh(0x5C6658); // default: 200.0
hook::Type<float> obj_LowThresh(0x5C665C); // default: 100.0
hook::Type<float> obj_MedThresh(0x5C6660); // default: 40.0

hook::Type<float> sdl_VLowThresh(0x5C5708);  // default: 300.0
hook::Type<float> sdl_LowThresh(0x5C570C);  // default: 100.0
hook::Type<float> sdl_MedThresh(0x5C5710);  // default: 50.0

hook::Type<int> timeOfDay(0x62B068);

/*
    cityLevelHandler
*/

int cityLevelHandler::city_numRooms = 0;
int cityLevelHandler::city_currentRoom = 0;

const double cosNum = 1.570796;

/* PSDL shading fix */
Vector3 vglAmbient;
Vector3 vglKeyColor;
Vector3 vglFill1Color;
Vector3 vglFill2Color;
Vector3 vglShadedColor;


static Vector3 addPitch(Vector3* vec, float pitch) {
    pitch = (float)fmod(pitch, 3.14159);
    bool pitchIsZero = (pitch >= 0.0f);

    return {
        (float)((!pitchIsZero) ? fmaxf(vec->X, 0) * cos(pitch + cosNum) : 0.0f),
        (float)((!pitchIsZero) ? fmaxf(vec->Y, 0) * cos(pitch + cosNum) : 0.0f),
        (float)((!pitchIsZero) ? fmaxf(vec->Z, 0) * cos(pitch + cosNum) : 0.0f),
    };
}

static float normalize(float value) {
    if (value >= 2.0f)
        value = 1.0f;

    return (value > 1.0f) ? (value - (value - 1.0f)) : value;
}

static Vector3 intToColor(int value) {
    return {
        (float)((char)((value & 0xFF0000) >> 16) / 256.0f),
        (float)((char)((value & 0xFF00) >> 8) / 256.0f),
        (float)((char)((value & 0xFF)) / 256.0f),
    };
}


void cityLevelHandler::DrawRooms(const gfxViewport* viewport, unsigned int p2, LPVOID roomRecs, int numRooms)
{
    cityLevelHandler::city_numRooms = numRooms;
    cityLevelHandler::city_currentRoom = 0;

    auto level = reinterpret_cast<cityLevel*>(this);
    level->DrawRooms(viewport, p2, roomRecs, numRooms);
}

void cityLevelHandler::PostUpdate() {
    // update the SDL lighting
    sdlCommon::UpdateLighting();

    // update our shaded lighting
    // TODO: fix lighting quality not being taken into account (harder than it sounds)
    auto timeWeather = $::timeWeathers.ptr(timeOfDay);

    vglKeyColor = addPitch(&timeWeather->KeyColor, timeWeather->KeyPitch);
    vglFill1Color = addPitch(&timeWeather->Fill1Color, timeWeather->Fill1Pitch);
    vglFill2Color = addPitch(&timeWeather->Fill2Color, timeWeather->Fill2Pitch);

    // convert the ambient to a vector3 for better accuracy
    vglAmbient = intToColor(timeWeather->Ambient);

    // compute le values
    vglShadedColor = {
        normalize(vglKeyColor.X + vglFill1Color.X + vglFill2Color.X + vglAmbient.X),
        normalize(vglKeyColor.Y + vglFill1Color.Y + vglFill2Color.Y + vglAmbient.Y),
        normalize(vglKeyColor.Z + vglFill1Color.Z + vglFill2Color.Z + vglAmbient.Z),
    };

    vglHandler::vglResultColor.r = byte(vglShadedColor.X * 255.999f);
    vglHandler::vglResultColor.g = byte(vglShadedColor.Y * 255.999f);
    vglHandler::vglResultColor.b = byte(vglShadedColor.Z * 255.999f);
    vglHandler::vglResultColor.a = 255;
}

void cityLevelHandler::SetObjectDetail(int lod) {
    // Default MM2 values (leaving this here for reference)
    // static float lodLevels[4][4] = {
    //     { 200.0f, 150.0f,  70.0f, 20.0f }, // Low
    //     { 250.0f, 175.0f,  90.0f, 30.0f }, // Medium
    //     { 300.0f, 200.0f, 100.0f, 40.0f }, // High
    //     { 300.0f, 200.0f, 130.0f, 70.0f }, // Very high
    // };

    static const char *lodLevelNames[4] = {
        "Low",
        "Medium",
        "High",
        "Very High",
    };

    // 'Low' now uses 'Medium' values, 'Medium' uses 'High', etc.
    // 'Very high' now renders a bit farther than before, but not to an extreme.
    // Performance drops are to be expected until bugs can be ironed out.
    // Poor PVS optimization seems to be the reason why values were so low.
    static float lodLevels[4][4] = {
        { 250.0f, 175.0f,  90.0f, 30.0f }, // Low       (Default: Medium)
        { 300.0f, 200.0f, 100.0f, 40.0f }, // Medium    (Default: High)
        { 300.0f, 200.0f, 130.0f, 70.0f }, // High      (Default: Very High)
        { 5000.0f, 200.0f, 150.0f, 100.0f }, // Very high (NEW)
    };

    if (lod == 3) {
        HookConfig::GetProperty("UltraLOD_NoDrawThreshold", lodLevels[lod][0]);
        HookConfig::GetProperty("UltraLOD_VeryLowThreshold", lodLevels[lod][1]);
        HookConfig::GetProperty("UltraLOD_LowThreshold", lodLevels[lod][2]);
        HookConfig::GetProperty("UltraLOD_MediumThreshold", lodLevels[lod][3]);
    }

    // Using temporary variables so we don't need to constantly access pointers

    float objNoDrawThresh = lodLevels[lod][0]; // VL: <VLowThresh> - <NoDrawThresh>
    float objVLowThresh = lodLevels[lod][1]; // L: <LowThresh> - <VLowThresh>
    float objLowThresh = lodLevels[lod][2]; // M: <MedThresh> - <LowThresh>
    float objMedThresh = lodLevels[lod][3]; // H: 0.0 - <MedThresh>

    obj_NoDrawThresh = objNoDrawThresh;
    obj_VLowThresh = objVLowThresh;
    obj_LowThresh = objLowThresh;
    obj_MedThresh = objMedThresh;

    // By default, the game doesn't set these values based on the detail level
    // They are hardcoded to what is considered 'High' by default,
    // however this is now known as 'Medium' (lod = 1; see above)
    //
    // 'Medium' and below (default 'High') uses the defaults.
    float sdlVLowThresh = (lod > 1) ? (objVLowThresh + 100.0f) : 300.0f;
    float sdlLowThresh = (lod > 1) ? (objLowThresh + 25.0f) : 100.0f;
    float sdlMedThresh = (lod > 1) ? (objMedThresh + 10.0f) : 50.0f;

    sdl_VLowThresh = sdlVLowThresh;
    sdl_LowThresh = sdlLowThresh;
    sdl_MedThresh = sdlMedThresh;

    // By default the game doesn't set this based on the detail level
    float pedDrawThreshold = powf(HookConfig::GetProperty("PedestrianLod", 35.5f), 2) * (lod + 1);
    ped_LodThreshold = pedDrawThreshold;

    LogFile::Format("[cityLevel::SetObjectDetail]: '%s'\n"
        " - OBJ { %.4f, %.4f, %.4f, %.4f }\n"
        " - SDL { %.4f, %.4f, %.4f }\n"
        " - PED { %.4f }\n",
        lodLevelNames[lod],
        objNoDrawThresh, objVLowThresh, objLowThresh, objMedThresh,
        sdlVLowThresh, sdlLowThresh, sdlMedThresh,
        pedDrawThreshold);
}

void cityLevelHandler::Install() {
    InstallVTableHook("cityLevel::SetObjectDetail", &SetObjectDetail, {
        0x5B16E0
    });

    InstallCallback("cityLevel::DrawRooms", "Custom implementation to allow for getting the number of rooms.",
        &DrawRooms, {
            cb::call(0x445798), // cityLevel::Draw
        }
    );

    InstallCallback("cityLevel::Update", "Adds PostUpdate handler.",
        &PostUpdate, {
            cb::jmp(0x4452D0), // jump to PostUpdate at the very end
        }
    );

    // moves ped lod threshold to writable memory
    mem::write(0x54BC3D + 2, &ped_LodThreshold);
    mem::write(0x57B6CE + 2, &ped_LodThreshold);
}

