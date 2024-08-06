#include "vehSplashHandler.h"

using namespace MM2;

/*
    vehSplashHandler
*/

void vehSplashHandler::ConstructNode()
{
    hook::Thunk<0x4A0CE0>::Call<void>(this); // asNode::asNode
    auto splash = reinterpret_cast<vehSplash*>(this);
    splash->setActive(false);
}

void vehSplashHandler::Install()
{
    InstallCallback("vehSplash::vehSplash", "Fix floating-by-default cars.",
        &ConstructNode, {
            cb::call(0x4D6A1F),
        }
    );
}