#include "vehCarAudioHandler.h"

using namespace MM2;

/*
    vehCarAudioHandler
*/

static ConfigValue<float> cfgAirborneTimerThresh("AirborneTimerThreshold", 1.1);
static ConfigValue<float> cfgAirborneSpeedThresh("AirborneSpeedThreshold", 45.0);
static ConfigValue<bool> cfgEnableAirborneCheck("TweakableAirborneCheck", false);
float carAirborneTimer = 0.0f;

void vehCarAudioHandler::DestructEngineSampleWrapper()
{
    auto wrapper = reinterpret_cast<vehEngineSampleWrapper*>(this);
    wrapper->DeallocateSoundPtr();
}

void vehCarAudioHandler::DestructSurfaceAudioData()
{
    // By default the game does not deallocate the audio
    auto data = reinterpret_cast<vehSurfaceAudioData*>(this);

    if (data->NumSkidSamples > 0)
    {
        for (int i = 0; i < data->NumSkidSamples; i++)
        {
            delete data->SkidWaveData[i].SoundBasePtr;
        }
        delete data->SkidWaveData;
    }
    delete data->SurfaceSoundBasePtr;
}

bool vehCarAudioHandler::IsAirBorne()
{
    return carAirborneTimer > cfgAirborneTimerThresh.Get();
}

void vehCarAudioHandler::Reset() 
{
    carAirborneTimer = 0.0f;

    // call original
    hook::Thunk<0x4DBE30>::Call<void>(this);
}

void vehCarAudioHandler::Update() 
{
    float elapsedTime = datTimeManager::Seconds;
    auto carAudio = reinterpret_cast<vehCarAudio*>(this);
    auto carSim = carAudio->GetCarSim();

    // grab only forward/sideways velocity, ignore vertical
    Vector3 vehicleVelo = carSim->GetInstance()->GetVelocity();
    float vehicleMph = sqrtf((vehicleVelo.X * vehicleVelo.X) + (vehicleVelo.Z * vehicleVelo.Z)) * 2.23694f;

    //update timer
    if (carSim->OnGround() || carSim->BottomedOut() || vehicleMph < cfgAirborneSpeedThresh.Get())
    {
        carAirborneTimer = 0.0f;
    }
    else
    {
        carAirborneTimer += elapsedTime;
    }

    // call original
    hook::Thunk<0x4DC320>::Call<void>(this);
}

void vehCarAudioHandler::Install() 
{
    /*
    InstallCallback("vehEngineSampleWrapper::~vehEngineSampleWrapper", "Deallocate audio when destroying engine sample.",
        &DestructEngineSampleWrapper, {
            cb::jmp(0x4DF570)
        }
    );

    InstallCallback("vehSurfaceAudioData::~vehSurfaceAudioData", "Deallocate audio when destroying surface data.",
        &DestructSurfaceAudioData, {
            cb::jmp(0x4DFD80)
        }
    );
    */

    // Experimental "is airborne" check (maybe this should be a feature handler or removed instead?)
    if (!cfgEnableAirborneCheck.Get())
        return;

    InstallCallback("vehCarAudio::IsAirBorne", "Better method of vehicle airborne checking.",
        &IsAirBorne, {
            cb::jmp(0x4D16F9),
            cb::jmp(0x4D1705),
            cb::jmp(0x4D1711),
            cb::jmp(0x4D171F),
        }
    );

    InstallCallback("vehCarAudio::Reset", "Better method of vehicle airborne checking.",
        &Reset, {
            cb::call(0x4D19CA),
        }
    );

    InstallVTableHook("vehCarAudio::Update",
        &Update, {
            0x5B319C
        }
    );
}