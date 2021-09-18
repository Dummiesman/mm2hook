#include "aiPoliceForceHandler.h"

using namespace MM2;

/*
    aiPoliceForceHandler
*/

void aiPoliceForceHandler::Reset(void) {
    // reset number of cops pursuing player
    // fixes incorrect music bug
    vehPoliceCarAudio::iNumCopsPursuingPlayer = 0;
    reinterpret_cast<aiPoliceForce*>(this)->Reset();
}

void aiPoliceForceHandler::Install() {
    InstallCallback("aiPoliceForce::Reset", "Resets the number of cops pursuing the player upon reset.",
        &Reset, {
            cb::call(0x536AAE),
            cb::call(0x550ECA),
        }
    );
}