#include "aiPoliceOfficerFeatureHandler.h"
#include "Wanted_Common.h"

using namespace MM2;

/*
    aiPoliceOfficerFeatureHandler
*/

void aiPoliceOfficerFeatureHandler::DetectPerpetrator() {
    mmGameManager *mgr = mmGameManager::Instance;
    auto player = mgr->getGame()->GetPlayer();

    if (*getPtr<int>(player, 0x2258) || Wanted_Common::bustedTimer > 4.f)
        return;

    //call original
    hook::Thunk<0x53DFD0>::Call<void>(this);
}

void aiPoliceOfficerFeatureHandler::Install() {
    InstallCallback("aiPoliceOfficer::DetectPerpetrator", "Doesn't make cops detect the player once they cross the finish line.",
        &DetectPerpetrator, {
            cb::call(0x53DC91),
        }
    );
}

