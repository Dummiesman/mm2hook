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

int mmInterfaceFeatureHandler::ShowcaseMenuStateHook(int menuId)
{
    // Hook the MenuState call in mmInterface because the showcase menu treats *any button* as done
    // This lets people add more buttons to the menu
    auto manager = reinterpret_cast<MenuManager*>(this);
    int state = manager->MenuState(menuId);
    int widget = manager->GetCurrentMenu()->GetWidgetID();
    if (widget == 0) return state; // The 'DONE' button is being used, return its result
    return 0; // This will prevent menu switching
}

void mmInterfaceFeatureHandler::Install() {
    InstallCallback("MenuManager::MenuState", "Hook to allow extra buttons on the vehicle showcase.",
        &ShowcaseMenuStateHook, {
            cb::call(0x40ADB7),
            cb::call(0x40ADC8)
        }
    );

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
