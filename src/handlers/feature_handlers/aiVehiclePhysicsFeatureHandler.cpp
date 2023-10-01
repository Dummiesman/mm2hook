#include "aiVehiclePhysicsFeatureHandler.h"

using namespace MM2;

/*
    aiVehiclePhysicsFeatureHandler
*/

static ConfigValue<bool> cfgOppsUseAllColors("OpponentsUseAllColors", false);

void aiVehiclePhysicsFeatureHandler::Install()
{
    if (cfgOppsUseAllColors.Get())
    {
        InstallPatch("Allow opponents to use more than four colors.", { 0x90, 0x90, 0x90 }, {
            0x559423,
        });
    }
}