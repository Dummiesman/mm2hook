#include "vehSemiCarAudioBugfixHandler.h"

using namespace MM2;

static ConfigValue<bool> cfgAirbrakeFix("AirbrakeFix", true);

/*
    vehSemiCarAudioBugfixHandler
*/

bool canPlayAirBlowSound = false;

void vehSemiCarAudioBugfixHandler::UpdateAirBlow()
{
    auto carAudio = reinterpret_cast<vehCarAudio*>(this);
    auto carsim = carAudio->getCarSim();

    //only do this sound for player vehicles
    //if done on all vehicles, cheaters in multiplayer
    //will just absolutely SPAM the airbrake sound
    auto player = mmGameManager::Instance.get()->getPlayerSafe();
    if (player == nullptr)
        return;
    if (player->getCar()->getCarSim() != carsim)
        return;

    float speed = carsim->GetSpeedMPH();
    bool isBraking = carsim->GetBrake() > 0.1f;

    //if we're going >5mph, set the sound flag
    if (speed > 5.f) {
        canPlayAirBlowSound = true;
    }

    //if we're going <1mph, play the sound
    if (speed < 1.f) {
        if (isBraking && canPlayAirBlowSound) {
            auto airBrakeSound = *getPtr<AudSoundBase*>(this, 0x13C);
            if (airBrakeSound != nullptr)
                airBrakeSound->PlayOnce(-1.f, -1.f);
        }
        canPlayAirBlowSound = false;
    }
}

void vehSemiCarAudioBugfixHandler::UpdateReverse()
{
    auto carAudio = reinterpret_cast<vehCarAudio*>(this);
    auto carsim = carAudio->getCarSim();
    auto reverseSound = *getPtr<AudSoundBase*>(this, 0x138);
    auto transmission = carsim->GetTransmission();
    float throttle = carsim->GetEngine()->GetThrottleInput();
    float speed = carsim->GetSpeedMPH();

    if (transmission->IsAuto() && reverseSound != nullptr) {
        if (transmission->GetGear() == 0) {
            if (throttle > 0.f || speed >= 1.f) {
                if (!reverseSound->IsPlaying())
                    reverseSound->PlayLoop(-1.f, -1.f);
            }
            else {
                if (reverseSound->IsPlaying())
                    reverseSound->Stop();
            }
        }
        else {
            if (reverseSound->IsPlaying())
                reverseSound->Stop();
        }
    }
    if (!transmission->IsAuto() && reverseSound != nullptr) {
        if (transmission->GetGear() == 0) {
            if (!reverseSound->IsPlaying())
                reverseSound->PlayLoop(-1.f, -1.f);
        }
        else {
            if (reverseSound->IsPlaying())
                reverseSound->Stop();
        }
    }
}

void vehSemiCarAudioBugfixHandler::Init(MM2::vehCarSim* carsim, MM2::vehCarDamage* cardamage, char* basename, bool a5, bool a6, bool a7)
{
    //set some things in the semi audio class
    *getPtr<int>(this, 0x138) = 0;
    *getPtr<int>(this, 0x13C) = 0;
    *getPtr<float>(this, 0x130) = 0.87f;
    *getPtr<float>(this, 0x134) = 0.9f;
    *getPtr<int>(this, 0x140) = 0xFFFFFFFF;
    *getPtr<int>(this, 0x144) = 0xFFFFFFFF;

    //call vehSemiCarAudio::Load, to load airbrake
    string_buf<128> semiDataName("%s_semidata", basename);
    bool semiDataExists = datAssetManager::Exists("aud\\cardata\\shared", (LPCSTR)semiDataName, "csv");

    //if custom semi data exists, load that. Otherwise load default.
    if (semiDataExists) {
        Displayf("Loading custom semi data: %s", (LPCSTR)semiDataName);
        hook::Thunk<0x4DCD70>::Call<void>(this, (LPCSTR)semiDataName);
    }
    else {
        Displayf("Loading DEFAULT semi data, can't find custom semi data: %s", (LPCSTR)semiDataName);
        hook::Thunk<0x4DCD70>::Call<void>(this, "semidata");
    }

    //call back to vehCarAudio::Init
    hook::Thunk<0x4DB900>::Call<void>(this, carsim, cardamage, basename, a5, a6, a7);
}

void vehSemiCarAudioBugfixHandler::SetNon3DParams()
{
    bool isStereo = AudManagerBase::Instance.get()->getIsStereo();
    auto airBrakeSound = *getPtr<AudSoundBase*>(this, 0x13C);
    auto reverseSound = *getPtr<AudSoundBase*>(this, 0x138);
    float airBrakeVolume = *getPtr<float>(this, 0x134);
    float reverseVolume = *getPtr<float>(this, 0x130);
    float pan = *getPtr<float>(this, 0x7C);

    //setup reverse sound
    if (reverseSound != nullptr) {
        reverseSound->SetVolume(reverseVolume);
        reverseSound->SetFrequency(1.0f);
        if (isStereo)
            reverseSound->SetPan(pan, -1);
    }

    //setup air brake
    if (airBrakeSound != nullptr) {
        airBrakeSound->SetVolume(airBrakeVolume);
        airBrakeSound->SetFrequency(1.0f);
        if (isStereo)
            airBrakeSound->SetPan(pan, -1);
    }

    //call back to vehCarAudio::SetNon3DParams
    hook::Thunk<0x4DC240>::Call<void>(this);
}

void vehSemiCarAudioBugfixHandler::Install()
{
    if (vehCarModel::mm1StyleTransmission) {
        InstallCallback("vehSemiCarAudio::Init", "Fix semi reverse audio.",
            &UpdateReverse, {
                cb::call(0x4DCB35),
                cb::call(0x4DCB5A),
            }
        );
    }

    if (!cfgAirbrakeFix.Get())
        return;

    InstallCallback("vehSemiCarAudio::Init", "Allow custom sounds for air brake audio.",
        &Init, {
            cb::call(0x4DC99A)
        }
    );

    InstallCallback("vehSemiCarAudio::Init", "Fix semi air brake audio.",
        &UpdateAirBlow, {
            cb::call(0x4DCB61),
            cb::call(0x4DCB3C)
        }
    );

    InstallVTableHook("vehSemiCarAudio::SetNon3DParams",
        &SetNon3DParams, {
            0x5B31C4
        }
    );
}