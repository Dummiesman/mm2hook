#include "mmGameHandler.h"

#include "mmPlayerHandler.h"
#include "gfxPipelineHandler.h"
#include "cityTimeWeatherLightingHandler.h"

#include "..\events\dispatcher.h"


using namespace MM2;

/*
    mmGameHandler
*/

hook::Type<float> bridgeSpeed(0x5DBFA4);
hook::Type<float> bridgeAngle(0x5DBFA8);

static ConfigValue<bool> cfgGtaStyleHornSiren("GTAStyleHornSiren", false);
static ConfigValue<float> cfgHornSirenThreshold("HornSirenThreshold", 0.15f);
static ConfigValue<bool> cfgMm1StyleAutoReverse("MM1StyleAutoReverse", false);

static float horn_lastPressTime = 0.f;
static float horn_lastReleaseTime = 0.f;
static float horn_holdTime = 0.f;
static float horn_sirenThreshold = 0.15f;
static bool lastHornButtonState = false;

void mmGameHandler::SendChatMessage(char *message) {
    if (gfxPipelineHandler::g_bConsoleOpen) {
        if (MM2Lua::IsEnabled())
            MM2Lua::SendCommand(message);

        // return normal chatbox behavior
        gfxPipelineHandler::g_bConsoleOpen = false;
    }
    else {
        LogFile::Format("Got chat message: %s\n", message);

        //handle custom commands (TODO: some "RegisterCommand" thing?)
        if (!strcmp(message, "/freecam")) {
            mmGameManager *mgr = mmGameManager::Instance;
            auto gamePtr = (mgr != NULL) ? mgr->getGame() : NULL;
            auto playerPtr = (gamePtr != NULL) ? gamePtr->getPlayer() : NULL;

            if (gamePtr != NULL && playerPtr != NULL)
            {
                auto playerPosition = playerPtr->getCar()->getModel()->GetPosition();
                auto freecam = playerPtr->getFreecam();

                //setcam and update view
                playerPtr->getCamView()->SetCam(freecam);
                freecam->UpdateView();

                //set freecam pos if >=100m away
                if (playerPosition.Dist(*freecam->getPosition()) >= 100.f) {
                    freecam->SetPosition(&playerPosition);
                }
            }
        }
        if (!strcmp(message, "/fuzz")) {
            mmGameManager *mgr = mmGameManager::Instance;
            auto gamePtr = (mgr != NULL) ? mgr->getGame() : NULL;
            auto playerPtr = (gamePtr != NULL) ? gamePtr->getPlayer() : NULL;

            if (gamePtr != NULL && playerPtr != NULL)
            {
                //ShowAllCops
                *getPtr<BOOL>(playerPtr->getHudmap(), 0x38) = !*getPtr<BOOL>(playerPtr->getHudmap(), 0x38);
            }
        }
        if (!strcmp(message, "/grav")) {
            dgPhysManager::Gravity.set(dgPhysManager::Gravity.get() == -19.6f ? -9.8f : -19.6f);
        }
        if (!strcmp(message, "/slide")) {
            if (MMSTATE->WeatherType == 3 && MMSTATE->TimeOfDay == 3)
                vehWheel::WeatherFriction.set(vehWheel::WeatherFriction.get() == 0.75f ? -0.02f : 0.75f);
            else if (MMSTATE->WeatherType == 3 && MMSTATE->TimeOfDay != 3)
                vehWheel::WeatherFriction.set(vehWheel::WeatherFriction.get() == 0.8f ? -0.03f : 0.8f);
            else
                vehWheel::WeatherFriction.set(vehWheel::WeatherFriction.get() == 1.f ? -0.04f : 1.f);
        }
        if (!strcmp(message, "/bridge")) {
            bridgeSpeed.set(bridgeSpeed.get() == 0.05f ? 2.5f : 0.05f);
            bridgeAngle.set(bridgeAngle.get() == 0.471238941f ? 1.f : 0.471238941f);
        }
        if (!strcmp(message, "/fly")) {
           mmPlayerHandler::playerCanFly = !mmPlayerHandler::playerCanFly;
        }
        if (!strcmp(message, "/nodamage")) {
            mmGameManager *mgr = mmGameManager::Instance;
            auto gamePtr = (mgr != NULL) ? mgr->getGame() : NULL;
            auto playerPtr = (gamePtr != NULL) ? gamePtr->getPlayer() : NULL;

            if (gamePtr != NULL && playerPtr != NULL)
            {
                auto carDamage = playerPtr->getCar()->getCarDamage();
                if (carDamage->getImpactThreshold() < 1001500.f)
                    carDamage->setImpactThreshold(carDamage->getImpactThreshold() + 1001500.f);
            }
        }
        if (!strcmp(message, "/damage")) {
            mmGameManager *mgr = mmGameManager::Instance;
            auto gamePtr = (mgr != NULL) ? mgr->getGame() : NULL;
            auto playerPtr = (gamePtr != NULL) ? gamePtr->getPlayer() : NULL;

            if (gamePtr != NULL && playerPtr != NULL)
            {
                auto carDamage = playerPtr->getCar()->getCarDamage();
                if (carDamage->getImpactThreshold() >= 1001500.f)
                    carDamage->setImpactThreshold(carDamage->getImpactThreshold() - 1001500.f);
            }
        }

        //send to dispatcher
        GameEventDispatcher::onChatMessage(message);
    }
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
    auto player = game->getPlayer();
    auto car = player->getCar();
    auto carsim = car->getCarSim();
    auto engine = carsim->getEngine();
    auto transmission = carsim->getTransmission();
    auto curDamage = car->getCarDamage()->getCurDamage();
    auto maxDamage = car->getCarDamage()->getMaxDamage();
    auto inst = mmReplayManager::Instance;

    void *gameInputPtr = *reinterpret_cast<void **>(0x6B1CF0); // pointer to mmInput

    int *vehCarPtr = *getPtr<int*>(player, 0x284);
    int reverse = *getPtr<int>(vehCarPtr, 0x304);
    int autoReverse = *getPtr<int>(gameInputPtr, 0x18C);
    int *pedalsSwapped = getPtr<int>(gameInputPtr, 0x1D4); // swaps throttle and brake inputs if true

    float v1 = *getPtr<float>(this, 0x40C);
    float v2 = *getPtr<float>(this, 0x68);
    float v3 = *getPtr<float>(this, 0x6C);
    float speedMPH = carsim->getSpeedMPH();
    float brakes = inst->GetBrakes();
    float throttle = inst->GetThrottle();
    float steering = inst->GetSteering();
    float handbrakes = inst->GetHandBrakes();

    carsim->setBrake(brakes);
    carsim->setHandbrake(handbrakes);
    player->SetSteering(steering);

    if (NETMGR->getInSession() && reverse >= 2) {
        if (throttle >= 0.f) {
            if (throttle > v1)
                throttle = v1;
            engine->setThrottleInput(throttle);
        }
        else {
            engine->setThrottleInput(0.f);
        }
    }
    else {
        engine->setThrottleInput(throttle);
    }

    if (transmission->IsAuto() && autoReverse) {
        if (reverse) {
            if (speedMPH <= v3 && brakes >= v2 && throttle <= 0.1f) {
                *pedalsSwapped = true;
                transmission->SetReverse();
            }
        }
        else if (!reverse && *pedalsSwapped) {
            if (speedMPH <= v3 && brakes >= v2 && throttle <= 0.1f) {
                *pedalsSwapped = false;
                transmission->SetForward();
            }
        }
        else if (!reverse && !*pedalsSwapped) {
            if (speedMPH <= v3 && brakes >= v2 && throttle <= 0.1f) {
                *pedalsSwapped = true;
            }
        }
        // reset throttle and brake inputs when the vehicle is destroyed
        if (curDamage >= maxDamage && *pedalsSwapped)
            *pedalsSwapped = false;
    }
}

void mmGameHandler::UpdateHorn(bool a1) {
    auto game = reinterpret_cast<mmGame*>(this);
    auto player = game->getPlayer();
    auto car = player->getCar();
    auto siren = car->getSiren();
    auto audio = car->getAudio();
    auto model = car->getModel();
    auto lightbar0 = model->getGenBreakableMgr()->Get(1);
    auto lightbar1 = model->getGenBreakableMgr()->Get(2);

    auto policeAudio = audio->GetPoliceCarAudioPtr();
    char* vehName = car->getCarDamage()->GetName();

    bool isSirenActive = siren->Active;
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
                if (siren->Active) {
                    siren->Active = false;
                    audio->StopSiren();
                }
                else
                {
                    siren->Active = true;
                    audio->StartSiren();
                }
            }
        }
    }

    //update horn audio
    if (isVehiclePolice)
    {
        auto hornSound = *getPtr<AudSoundBase*>(policeAudio, 0x10C);
        if (hornSound->IsPlaying() != a1) {
            if (a1) {
                if (!cancelHornInput)
                    hornSound->PlayLoop(-1.f, -1.f);
            }
            else {
                hornSound->Stop();
            }
        }
    }
    else
    {
        if (a1) {
            audio->PlayHorn();
        }
        else
        {
            audio->StopHorn();
        }
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

    //removes the "SpecialBound" (INST) spawn limitation (Flag 32), leaving only Standard flag
    /*
    InstallPatch("Enables spawning in INST rooms.", { 0x04 }, {
        0x413C1C,
        });
        */

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

