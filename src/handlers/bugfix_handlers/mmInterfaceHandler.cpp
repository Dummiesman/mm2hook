#include "mmInterfaceHandler.h"

using namespace MM2;

/*
    mmInterfaceHandler
*/

hook::Type<char[80]> currentPlayerVehicle(0x6B1B28);

void mmInterfaceHandler::PlayerResolveCars() {
    //call original
    hook::Thunk<0x40FE20>::Call<void>(this);

    //null out currentPlayerVehicle if this vehicle is missing
    //the code after PlayerResolveCars in PlayerSetState will then reset to vpbug
    if (!datAssetManager::Exists("tune", currentPlayerVehicle, "info")) {
        currentPlayerVehicle[0] = NULL;
    }
}

void mmInterfaceHandler::Install() {
    InstallCallback("mmInterface::PlayerSetState", "Fixes game crashes in the vehicle select menu when the selected vehicle is missing.",
        &PlayerResolveCars, {
            cb::call(0x040E256),
        }
    );
}
