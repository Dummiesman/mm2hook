#include "mmHudMapHandler.h"

using namespace MM2;

/*
    mmHudMapHandler
*/

bool mmHudMapHandler::CanActivateMap() {
    //0x3D is set to 1 when the PKG isn't found
    return *getPtr<byte>(this, 0x3D) != 1;
}

void mmHudMapHandler::Activate() {
    if (!CanActivateMap())
        return;

    //forward to mmHudMap::Activate
    hook::Thunk<0x42EEE0>::Call<void>(this);
}

void mmHudMapHandler::SetMapMode(int mode) {
    if (!CanActivateMap())
        return;

    //forward to mmHudMap::SetMapMode
    hook::Thunk<0x42EF30>::Call<void>(this, mode);
}

void mmHudMapHandler::Install() {
    InstallCallback("mmHudMap::Activate", "Fixes crashes when attempting to activate a nonexistant hudmap.",
        &Activate, {
            cb::call(0x42A306), // mmPopup::DisablePU
        }
    );
    InstallCallback("mmHudMap::SetMapMode", "Fixes crashes when attempting to activate a nonexistant hudmap.",
        &SetMapMode, {
            cb::call(0x42EE98), // mmHudMap::Reset
            cb::call(0x43204E), // mmViewMgr::SetViewSetting
        }
    );
}