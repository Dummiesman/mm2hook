#include "mmInterfaceFeatureHandler.h"

using namespace MM2;

/*
    mmInterfaceFeatureHandler
*/

void mmInterfaceFeatureHandler::ShowMain(int a2)
{
    // set instance
    auto intf = reinterpret_cast<mmInterface*>(this);
    mmInterface::SetInstance(intf);

    // call original
    hook::Thunk<0x40A140>::Call<void>(this, a2);
}

void mmInterfaceFeatureHandler::Destruct()
{
    // call original
    hook::Thunk<0x4094C0>::Call<void>(this);

    // clear interface instance
    mmInterface::ClearInstance();
}

void mmInterfaceFeatureHandler::Install() {
    InstallCallback("mmInterface::ShowMain", "Hook to store an instance of mmInterface.",
        &ShowMain, {
            cb::call(0x40182C),
        }
    );

    InstallCallback("mmInterface::~mmInterface", "Hook to store an instance of mmInterface.",
        &Destruct, {
            cb::call(0x4124D6),
        }
    );
}
