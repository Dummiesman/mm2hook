#include "aiPedestrianHandler.h"

using namespace MM2;

static ConfigValue<int> cfgMaxPedUpdateAttempts("MaxPedUpdateAttempts", 256);

/*
    aiPedestrianHandler
*/

int aiPedestrianHandler::UpdateAttempts = 0;

void aiPedestrianHandler::Update(void) {
    if (aiPedestrianHandler::UpdateAttempts < cfgMaxPedUpdateAttempts) {
        ++aiPedestrianHandler::UpdateAttempts;
        $::aiPedestrian::Update(this);
    }
}

void aiPedestrianHandler::Install() {
    InstallCallback("aiPedestrian::Update", "Limits the number of update attempts for a pedestrian.",
        &Update, {
            cb::call(0x544191), // aiPath::UpdatePedestrians
        }
    );
}