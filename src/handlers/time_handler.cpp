#include "time_handler.h"

#include <events\dispatcher.h>
#include <modules\data\timemgr.h>

using namespace MM2;

/*
    TimeHandler
*/
void TimeHandler::Update(void) {
    GameEventDispatcher::onTick();

    // pass control back to MM2
    datTimeManager::Update();
}

void TimeHandler::Install() {
    InstallCallback("datTimeManager::Update", "Intercepts the call to update each tick.",
        &Update, {
            cb::call(0x401A2F),
        }
    );
}
