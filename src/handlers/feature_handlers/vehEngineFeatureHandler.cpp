#include "vehEngineFeatureHandler.h"

using namespace MM2;

/*
    vehEngineFeatureHandler
*/

static ConfigValue<bool> cfgPhysicalEngineDamage("PhysicalEngineDamage", false);

float vehEngineFeatureHandler::CalcTorque(float a1) {
    auto engine = reinterpret_cast<vehEngine*>(this);

    auto carDamage = reinterpret_cast<vehCarModel*>(engine->GetCarSim()->GetInstance())->GetCar()->GetCarDamage();
    float damagePercent = (carDamage->GetCurDamage() - carDamage->GetMedDamage()) / (carDamage->GetMaxDamage() - carDamage->GetMedDamage());
    
    //clamp from 0-70%
    damagePercent = fmaxf(0.0f, fminf(damagePercent, 0.7f));
    float invDamagePercent = 1.f - damagePercent;

    float torque = hook::Thunk<0x4D8ED0>::Call<float>(this, a1);
    return torque * invDamagePercent;
}

void vehEngineFeatureHandler::Install(void) {
    if (!cfgPhysicalEngineDamage.Get())
        return;

    InstallCallback("vehEngine::CalcTorque", "Overwrite torque calculation for engine damage",
        &CalcTorque, {
            cb::call(0x4D8F65),
        }
    );
}

