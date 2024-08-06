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

hook::Type<bool> sdl_SolidColor(0x62B08E);

static hook::Type<bool> sm_PerRoomLighting = 0x5C5720;

/*
    cityLevelHandler
*/

int cityLevelHandler::city_numRooms = 0;
int cityLevelHandler::city_currentRoom = 0;

const double cosNum = 1.570796;

/* PSDL shading fix */

static void setLightDirectionInv(Vector3* out, float heading, float pitch) {
    out->X = -(cosf(heading) * cosf(pitch));
    out->Y = -(sinf(pitch));
    out->Z = -(sinf(heading) * cosf(pitch));
}

void cityLevelHandler::UpdateLighting()
{
    // Originally the game calls UpdateLighting() here without first setting up the lights
    reinterpret_cast<cityLevel*>(lvlLevel::GetSingleton())->SetupLighting(Vector3(1.0, 1.0, 1.0));
    sdlCommon::UpdateLighting();

    // Calculate SDL lighting color
    // TODO: fix lighting quality not being taken into account (harder than it sounds)
    auto timeWeather = cityLevel::GetCurrentLighting();
    Vector3 keyDir, fill1Dir, fill2Dir;
    Vector3 lighting = Vector3(0.0f, 0.0f, 0.0f);

    Vector4 ambient;
    ambient.UnpackColorBGRA(timeWeather->Ambient);
    lighting += static_cast<Vector3>(ambient);

    setLightDirectionInv(&keyDir, timeWeather->KeyHeading, timeWeather->KeyPitch);
    setLightDirectionInv(&fill1Dir, timeWeather->Fill1Heading, timeWeather->Fill1Pitch);
    setLightDirectionInv(&fill2Dir, timeWeather->Fill2Heading, timeWeather->Fill2Pitch);

    Vector3 normal = Vector3::YAXIS;
    lighting += timeWeather->KeyColor * fmaxf(0.0f, normal.Dot(keyDir));
    lighting += timeWeather->Fill1Color * fmaxf(0.0f, normal.Dot(fill1Dir));
    lighting += timeWeather->Fill2Color * fmaxf(0.0f, normal.Dot(fill2Dir));

    lighting.X = fminf(1.0f, fmaxf(0.0f, lighting.X));
    lighting.Y = fminf(1.0f, fmaxf(0.0f, lighting.Y));
    lighting.Z = fminf(1.0f, fmaxf(0.0f, lighting.Z));
    
    vglHandler::vglResultColor.r = byte(lighting.X * 255.0f);
    vglHandler::vglResultColor.g = byte(lighting.Y * 255.0f);
    vglHandler::vglResultColor.b = byte(lighting.Z * 255.0f);
    vglHandler::vglResultColor.a = 255;
}

void cityLevelHandler::LightmapDelete(void* data)
{
    // Hooks the lmap data delete
    hook::StaticThunk<0x577380>::Call<void>(data);

    // Set perRoomLighting to TRUE when this happens as the LMAP exists
    sm_PerRoomLighting.set(true);
}

void cityLevelHandler::DrawRooms(const gfxViewport* viewport, unsigned int p2, LPVOID roomRecs, int numRooms)
{
    cityLevelHandler::city_numRooms = numRooms;
    cityLevelHandler::city_currentRoom = 0;
    UpdateLighting();

    auto level = reinterpret_cast<cityLevel*>(this);
    level->DrawRooms(viewport, p2, roomRecs, numRooms);
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

    // Also not set by default
    sdl_SolidColor = (lod <= 1);

    LogFile::Format("[cityLevel::SetObjectDetail]: '%s'\n"
        " - OBJ { %.4f, %.4f, %.4f, %.4f }\n"
        " - SDL { %.4f, %.4f, %.4f }\n"
        " - PED { %.4f }\n",
        lodLevelNames[lod],
        objNoDrawThresh, objVLowThresh, objLowThresh, objMedThresh,
        sdlVLowThresh, sdlLowThresh, sdlMedThresh,
        pedDrawThreshold);
}

MM2::gfxTexture* cityLevelHandler::GetEnvMap(int roomNumber, Vector3 const& position, float& intensity)
{
    auto level = reinterpret_cast<cityLevel*>(this);
    auto room = level->GetRoomInfo(roomNumber);

    if (room->Flags & 2)
    {
        // UNDERGROUND
        intensity = 0.0f;
        return nullptr; 
    }
    else 
    {
        return level->cityLevel::GetEnvMap(roomNumber, position, intensity);
    }
}

void cityLevelHandler::Install() {
    InstallVTableHook("cityLevel::SetObjectDetail", &SetObjectDetail, {
        0x5B16E0
    });

    InstallVTableHook("cityLevel::GetEnvMap", &GetEnvMap, {
        0x5B16F8
    });

    InstallCallback("cityLevel::DrawRooms", "Custom implementation to allow for getting the number of rooms.",
        &DrawRooms, {
            cb::call(0x445798), // cityLevel::Draw
        }
    );

    InstallCallback("cityLevel::Load", "Fix lighting being delayed by one load.",
        &UpdateLighting, {
            cb::call(0x444260),
        }
    );

    InstallCallback("cityLevel::Load - operator delete", "Fix PerRoomLighting never getting reset to true",
        &LightmapDelete, {
            cb::call(0x4447B8),
        }
    );

    // JMP over weird SetLight code with per room lighting
    InstallPatch({
        0xEB,
        }, {
            0x4458AD, // cityLevel::DrawRooms
        });

    // moves ped lod threshold to writable memory
    mem::write(0x54BC3D + 2, &ped_LodThreshold);
    mem::write(0x57B6CE + 2, &ped_LodThreshold);

    // move shadow render threshold from furthest possible -> L lod
    mem::write(0x445E23 + 2, obj_LowThresh.ptr());
    mem::write(0x445DC6 + 2, obj_LowThresh.ptr());
}

