#include "aiPathHandler.h"

using namespace MM2;

/*
    aiPathHandler
*/

void aiPathHandler::UpdatePedestrians(void) {
    auto path = reinterpret_cast<aiPath*>(this);
    aiPedestrianHandler::UpdateAttempts = 0;

    path->UpdatePedestrians();
}

void aiPathHandler::Install() {
    InstallCallback("aiPath::UpdatePedestrians", "Limits the number of update attempts for pedestrians.",
        &UpdatePedestrians, {
            cb::call(0x536FE0), // aiMap::Update
        }
    );
}