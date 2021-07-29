#include "gizFerryHandler.h"

using namespace MM2;

static ConfigValue<float> cfgFerrySpeedMultiplier("FerrySpeedMultiplier", 5.0f);

/*
    gizFerryHandler
*/

void gizFerryHandler::SetSpeed(float value) {
    value *= cfgFerrySpeedMultiplier;

    hook::Thunk<0x579520>::Call<void>(this, value);
}

void gizFerryHandler::Install() {
    InstallCallback("gizFerry:SetSpeed", "Allows a speed modifier to be applied to ferry speeds.",
        &SetSpeed, {
            cb::call(0x579951), // gizFerryMgr::ApplyTuning
        }
    );

}