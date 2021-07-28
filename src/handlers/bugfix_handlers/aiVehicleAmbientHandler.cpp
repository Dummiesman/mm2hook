#include "mpConsistencyHandler.h"

using namespace MM2;

/*
    aiVehicleAmbientHandler
*/

void aiVehicleAmbientHandler::Install() {
    // fixes traffic vehicles using a different color
    // than their respective breakables
    InstallPatch({ 0x90, 0x90, 0x90, 0x90, 0x90 }, {
        0x5513E2,
        });

}