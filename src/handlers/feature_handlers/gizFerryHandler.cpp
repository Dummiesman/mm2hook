#include "gizFerryHandler.h"

using namespace MM2;

static ConfigValue<float> cfgFerrySpeedMultiplier("FerrySpeedMultiplier", 5.0f);

/*
    gizFerryHandler
*/

void gizFerryHandler::SetSpeed(float value) {
    value *= cfgFerrySpeedMultiplier;
    reinterpret_cast<gizFerry*>(this)->SetSpeed(value);
}

void gizFerryHandler::Install() {
    InstallCallback("gizFerry:SetSpeed", "Allows a speed modifier to be applied to ferry speeds.",
        &SetSpeed, {
            cb::call(0x579951), // gizFerryMgr::ApplyTuning
        }
    );

}