#include "vehCarSimHandler.h"

using namespace MM2;

/*
    vehCarSimHandler
*/

void vehCarSimHandler::Install()
{
    InstallPatch({ 0x90 }, {
        0x42BB4B + 1, // Change size of vehCarSim on allocation
    });

    InstallCallback("vehCarSim::Init", "Use our own init function.",
        &vehCarSim::Init, {
            cb::call(0x403C21),
            cb::call(0x42BE75),
            cb::call(0x43C573),
            cb::call(0x43C6DF),
        }
    );
}

