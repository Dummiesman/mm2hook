#include "aiVehicleInstanceHandler.h"

using namespace MM2;

/*
    aiVehicleInstanceHandler
*/

void aiVehicleInstanceHandler::Reset()
{
    *getPtr<byte>(this, 0x1A) = 0;
    hook::Thunk<0x552100>::Call<void>(this); // call original
}

void aiVehicleInstanceHandler::Install()
{
    // fixes four ways persisting after level reset
    InstallVTableHook("aiVehicleInstance::Reset",
        &Reset, {
            0x5B590C,
        }
    );
}