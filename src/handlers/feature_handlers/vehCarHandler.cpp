#include "vehCarHandler.h"

using namespace MM2;

/*
    vehCarHandler
*/

static ConfigValue<bool> cfgVehicleDebug("VehicleDebug", "vehicleDebug", false);
static ConfigValue<bool> cfgPhysicalEngineDamage("PhysicalEngineDamage", true);

void vehCarHandler::InitCar(LPCSTR vehName, int a2, int a3, bool a4, bool a5) {
    Displayf("Initializing vehicle (\"%s\", %d, %d, %s, %s)", vehName, a2, a3, bool_str(a4), bool_str(a5));
    get<vehCar>()->Init(vehName, a2, a3, a4, a5);
}

const phBound * vehCarHandler::GetModelBound(int a1) {
    auto result = hook::Thunk<0x4648C0>::Call<const phBound *>(this, a1);

    if (result == NULL)
        Errorf(">>> COULD NOT RETRIEVE VEHICLE BOUND (%d) !!! <<<", a1);

    return result;
}

void vehCarHandler::InitCarAudio(LPCSTR a1, BOOL a2) {
    auto car = reinterpret_cast<vehCar*>(this);
    char *vehName = car->getCarDamage()->GetName();
    int flagsId = VehicleListPtr->GetVehicleInfo(vehName)->GetFlags();

    // debug if enabled
    if (cfgVehicleDebug.Get()) {
        Displayf("Loading vehicle audio (\"%s\", %d)", a1, a2);
    }

    //Automatic vehtypes system
    bool vehicleHasSiren = false;
    if (car->getSiren() != nullptr) {
        vehicleHasSiren = car->getSiren()->HasLights && car->getSiren()->LightCount > 0;
    }

    if (vehicleHasSiren || flagsId == 8 && !vehCarAudioContainer::IsPolice(a1)) {
        Displayf("%s has a lightbar, but is not in the vehtypes file. Adding it.");
        string_buf<128> sirenBuffer("%s,ENDOFDATA", a1);
        vehCarAudioContainer::RegisterPoliceNames(NULL, (LPCSTR)sirenBuffer);
    }

    string_buf<128> semiDataName("%s_semidata", a1);
    bool semiDataExists = datAssetManager::Exists("aud\\cardata\\shared", (LPCSTR)semiDataName, "csv");
    if (semiDataExists && !vehCarAudioContainer::IsSemiOrBus(a1)) {
        Displayf("%s has semidata, but is not in the vehtypes file. Adding it.");
        string_buf<128> semiBuffer("%s,ENDOFDATA", a1);
        vehCarAudioContainer::RegisterSemiNames(NULL, (LPCSTR)semiBuffer);
    }

    //pass back to original function
    car->InitAudio(a1, a2);
}

void vehCarHandler::Mm1StyleTransmission() {
    auto car = reinterpret_cast<vehCar*>(this);
    auto carsim = car->getCarSim();
    auto engine = carsim->getEngine();
    auto drivetrain = carsim->getDrivetrain();
    auto transmission = carsim->getTransmission();
    auto curDamage = car->getCarDamage()->getCurDamage();
    auto maxDamage = car->getCarDamage()->getMaxDamage();

    if (curDamage < maxDamage) {
        if (transmission->IsAuto()) {
            if (carsim->getSpeedMPH() >= 1.f && carsim->OnGround()) {
                if (engine->getThrottleInput() < 0.1f && transmission->getGear() != 1)
                    engine->setThrottleInput(0.1f);
            }
            // activate Handbrake if car goes under 1mph (P gear)
            if (carsim->getSpeedMPH() < 1.f && engine->getThrottleInput() < 0.1f) {
                carsim->setHandbrake(1.f);
            }
        }
        else {
            if (carsim->getBrake() < 0.1f && carsim->getHandbrake() < 0.1f) {
                if (engine->getThrottleInput() < 0.1f && transmission->getGear() != 1)
                    engine->setThrottleInput(0.1f);
            }
        }
    }
    // setting up this case for crash course
    // fixes ai cops and opponents have no brakes if they're damaged out
    if (curDamage >= maxDamage) {
        carsim->setBrake(1.f);
    }

    // attach drive train if we just hit throttle
    // fixes the short delay that happens before the car moves
    if (engine->getThrottleInput() > 0.f) {
        drivetrain->Attach();
    }
}

void vehCarHandler::Update() {
    auto car = reinterpret_cast<vehCar*>(this);
    auto siren = car->getSiren();
    auto audio = car->getAudio();
    auto model = car->getModel();
    auto damage = car->getCarDamage();
    auto engine = car->getCarSim()->getEngine();
    auto lightbar0 = model->getGenBreakableMgr()->Get(1);
    auto lightbar1 = model->getGenBreakableMgr()->Get(2);

    if ((lightbar0 != nullptr && !lightbar0->isAttached) ||
        (lightbar1 != nullptr && !lightbar1->isAttached)) {
        if (siren != nullptr && siren->Active) {
            siren->Active = false;
            audio->StopSiren();
        }
    }

    if (vehCarModel::mm1StyleTransmission) {
        vehCarHandler::Mm1StyleTransmission();
    }

    if (cfgPhysicalEngineDamage.Get()) {
        float damagePercent = (damage->getCurDamage() - damage->getMedDamage()) / (damage->getMaxDamage() - damage->getMedDamage());
        if (damagePercent > 0.f && engine->getThrottleInput() > 0.1f) {
            engine->setCurrentTorque((engine->getCurrentTorque() + (engine->getCurrentRPM() * 0.001f)) - (damagePercent * (engine->getMaxHorsePower() * 0.018f)));
        }
    }

    // call original
    hook::Thunk<0x42C690>::Call<void>(this);
}

void vehCarHandler::Install(void) {
    InstallCallback("vehCar::InitAudio", "Enables debugging for vehicle initialization, and automatic vehtypes handling.",
        &InitCarAudio, {
            cb::call(0x55943A), // aiVehiclePhysics::Init
            cb::call(0x404090), // mmPlayer::Init
            cb::call(0x43C540), // mmNetObject::Init
        }
    );

    if (cfgVehicleDebug) {
        InstallCallback("vehCar::InitAudio", "Enables debugging for vehicle initialization.",
            &InitCarAudio, {
                cb::call(0x55943A), // aiVehiclePhysics::Init
                cb::call(0x404090), // mmPlayer::Init
                cb::call(0x43C540), // mmNetObject::Init
            }
        );

        InstallVTableHook("vehCarModel::GetBound",
            &GetModelBound, {
                0x5B2D14
            }
        );
    }

    InstallVTableHook("vehCar::Update",
        &Update, {
            0x5B0BB8,
        }
    );

    if (vehCarModel::mm1StyleTransmission) {
        // deactivate auto Handbrake system
        InstallPatch({ 0xD8, 0x1D, 0x3C, 0x4, 0x5B, 0x0 }, {
            0x405C81,
        });
    }
}

