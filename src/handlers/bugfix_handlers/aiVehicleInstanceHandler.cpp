#include "aiVehicleInstanceHandler.h"

using namespace MM2;

static ConfigValue<bool> cfgUseAllTrafficColors("UseAllTrafficColors", true);

/*
    aiVehicleInstanceHandler
*/

void aiVehicleInstanceHandler::Reset()
{
    *getPtr<byte>(this, 0x1A) = 0;
    hook::Thunk<0x552100>::Call<void>(this); // call original
}

void aiVehicleInstanceHandler::Install()
{
    // fixes four ways persisting after level reset
    InstallVTableHook("aiVehicleInstance::Reset",
        &Reset, {
            0x5B590C,
        }
    );

    if (cfgUseAllTrafficColors) {
        InstallPatch("Allows traffic to use all color variants.", { 0x90 }, {
            0x55213F,
            });
    }
}