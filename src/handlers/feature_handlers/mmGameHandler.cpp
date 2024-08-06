#include "mmGameHandler.h"

#include "mmPlayerHandler.h"
#include "gfxPipelineHandler.h"
#include "cityTimeWeatherLightingHandler.h"

#include "..\events\dispatcher.h"


using namespace MM2;

/*
    mmGameHandler
*/

static ConfigValue<bool> cfgGtaStyleHornSiren("GTAStyleHornSiren", false);
static ConfigValue<float> cfgHornSirenThreshold("HornSirenThreshold", 0.15f);
static ConfigValue<bool> cfgMm1StyleAutoReverse("MM1StyleAutoReverse", false);

static float horn_lastPressTime = 0.f;
static float horn_lastReleaseTime = 0.f;
static float horn_holdTime = 0.f;
static float horn_sirenThreshold = 0.15f;
static bool lastHornButtonState = false;

void mmGameHandler::SendChatMessage(char *message) {
    LogFile::Format("Got chat message: %s\n", message);

    //send to dispatcher
    GameEventDispatcher::onChatMessage(message);
}

void mmGameHandler::InitWeather(void) {
    // should've already been initialized, but juuuust in case...
    if (cityTimeWeatherLightingHandler::TimeWeather == nullptr) {
        Warningf("Couldn't initialize weather using TimeWeather, loading defaults...");
        get<mmGame>()->InitWeather();
    }
}

void mmGameHandler::UpdateSteeringBrakes(void) {
    auto game = reinterpret_cast<mmGame*>(this);
    auto player = game->GetPlayer();
    auto car = player->GetCar();
    auto carsim = car->GetCarSim();
    
    
    auto transmission = carsim->GetTransmission();
    int currentGear = transmission->GetGear();

    auto replayman = mmReplayManager::Instance;
    auto input = GameInputPtr;

    int *vehCarPtr = *getPtr<int*>(player, 0x284);
    
    //int *pedalsSwapped = getPtr<int>(gameInputPtr, 0x1D4); // swaps throttle and brake inputs if true

    float throttleLimit = *getPtr<float>(this, 0x40C);
    float v2 = *getPtr<float>(this, 0x68);
    float v3 = *getPtr<float>(this, 0x6C);
    float speedMPH = carsim->GetSpeedMPH();
    float brakes = replayman->GetBrakes();
    float throttle = replayman->GetThrottle();
    float steering = replayman->GetSteering();
    float handbrakes = replayman->GetHandBrakes();

    carsim->SetBrake(brakes);
    carsim->SetHandbrake(handbrakes);
    player->SetSteering(steering);

    auto engine = carsim->GetEngine();
    if (currentGear >= 2)
        engine->SetThrottleInput(min(throttle, throttleLimit));
    else
        engine->SetThrottleInput(throttle);

    // handle auto reverse
    if (transmission->IsAuto() && input->AutoReverseEnabled()) {
        if (currentGear > 0) {
            if (speedMPH <= v3 && brakes >= v2 && throttle <= 0.1f) {
                input->SwapBrakeAndThrottleInput(true);
                transmission->SetReverse();
            }
        }
        else if (currentGear == 0 && input->PedalsSwapped()) {
            if (speedMPH <= v3 && brakes >= v2 && throttle <= 0.1f) {
                input->SwapBrakeAndThrottleInput(false);
                transmission->SetForward();
            }
        }
        else if (currentGear == 0 && !input->PedalsSwapped()) {
            if (speedMPH <= v3 && brakes >= v2 && throttle <= 0.1f) {
                input->SwapBrakeAndThrottleInput(true);
            }
        }
        
        // reset throttle and brake inputs when the vehicle is destroyed
        auto curDamage = car->GetCarDamage()->GetCurDamage();
        auto maxDamage = car->GetCarDamage()->GetMaxDamage();

        if (curDamage >= maxDamage && input->PedalsSwapped()) {
            input->SwapBrakeAndThrottleInput(false);
        }
    }
}

void mmGameHandler::UpdateHorn(bool a1) {
    auto game = reinterpret_cast<mmGame*>(this);
    auto player = game->GetPlayer();
    auto car = player->GetCar();
    auto siren = car->GetSiren();
    auto audio = car->GetCarAudioContainerPtr();
    auto model = car->GetModel();
    auto lightbar0 = model->GetGenBreakableMgr()->Get(1);
    auto lightbar1 = model->GetGenBreakableMgr()->Get(2);

    auto policeAudio = audio->GetPoliceCarAudioPtr();
    char* vehName = car->GetCarDamage()->GetName();

    bool isSirenActive = siren->IsActive();
    bool isVehiclePolice = audio->IsPolice(vehName);

    bool cancelHornInput = horn_holdTime < horn_sirenThreshold && isVehiclePolice;

    //button state updating
    bool buttonReleasedThisFrame = a1 != lastHornButtonState && !a1;
    bool buttonPressedThisFrame = a1 != lastHornButtonState && a1;
    if (buttonPressedThisFrame)
    {
        horn_lastPressTime = datTimeManager::ElapsedTime;
    }
    else if (buttonReleasedThisFrame)
    {
        horn_lastReleaseTime = datTimeManager::ElapsedTime;
        horn_holdTime = 0.f;
    }
    if (a1)
        horn_holdTime += datTimeManager::Seconds;
    lastHornButtonState = a1;

    //update police audio
    if (isVehiclePolice && siren != nullptr && !player->IsMaxDamaged()) {
        if ((lightbar0 == nullptr || (lightbar0 != nullptr && lightbar0->isAttached)) ||
            (lightbar1 == nullptr || (lightbar1 != nullptr && lightbar1->isAttached))) {
            if (buttonReleasedThisFrame && (horn_lastReleaseTime - horn_lastPressTime) < horn_sirenThreshold) {
                if (siren->IsActive()) {
                    siren->SetActive(false);
                    audio->StopSiren();
                }
                else
                {
                    siren->SetActive(true);
                    audio->StartSiren();
                }
            }
        }
    }

    //update horn audio
    if (a1) {
        if (!isVehiclePolice)
            audio->PlayHorn();
        else if (!cancelHornInput)
            audio->PlayHorn();
    }
    else {
        audio->StopHorn();
    }
}

void mmGameHandler::Install() {
    InstallPatch("Increases chat buffer size.", { 60 }, {
        0x4E68B5,
        0x4E68B9,
        0x50BBCF,
    });

    InstallCallback("mmGame::SendChatMessage", "Passes any chat messages to the handler.",
        &SendChatMessage, {
            cb::jmp(0x414EB6),
        }
    );

    InstallCallback("mmGame::InitWeather", "Allows for more control over weather initialization.",
        &InitWeather, {
            cb::call(0x4131C0), // mmGame::Init
        }
    );

    if (cfgMm1StyleAutoReverse.Get()) {
        InstallCallback("mmGame::UpdateSteeringBrakes", "Improves auto reverse system.",
            &UpdateSteeringBrakes, {
                cb::call(0x413EED),
                cb::call(0x413F29),
                cb::call(0x413F4C),
            }
        );
    }

    horn_sirenThreshold = cfgHornSirenThreshold.Get();
    if (cfgGtaStyleHornSiren.Get()) {
        InstallCallback("mmGame::UpdateHorn", "Implements GTA-style horn/siren",
            &UpdateHorn, {
                cb::call(0x413F22),
                cb::call(0x414691),
            }
        );
    }
}

