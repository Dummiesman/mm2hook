#include "vehBreakableMgrHandler.h"

using namespace MM2;

/*
    vehBreakableMgrHandler
*/

static ConfigValue<bool> cfgBreakReflections("ReflectionsOnBreakables", true);

void vehBreakableMgrHandler::Install() {
    vehBreakableMgr::EnableReflections = cfgBreakReflections.Get();
    InstallCallback("vehBreakableMgr::Draw", "Draws reflections on breakables.",
        &vehBreakableMgr::Draw, {
            cb::call(0x4CE1B7), // vehCarModel::Draw
            cb::call(0x552220), // aiVehicleInstance::Draw
        }
    );
}