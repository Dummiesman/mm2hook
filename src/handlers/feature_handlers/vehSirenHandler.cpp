#include "vehSirenHandler.h"

using namespace MM2;

/*
    vehSirenHandler
*/

void vehSirenHandler::Update() {
    auto siren = reinterpret_cast<vehSiren*>(this);
    siren->vehSiren::Update();
}

void vehSirenHandler::Reset() {
    auto siren = reinterpret_cast<vehSiren*>(this);
    siren->vehSiren::Reset();
}

void vehSirenHandler::SizeOf() {
    hook::StaticThunk<0x577360>::Call<vehSiren*>(0x164);
}

void vehSirenHandler::Install() {
    InstallCallback("vehSiren::Update", "Use our vehSiren update.",
        &Update, {
            cb::call(0x42C920),
        }
    );

    InstallCallback("vehSiren::Reset", "Deactivates siren lights upon reset.",
        &Reset, {
            cb::call(0x42C3A6),
        }
    );

    InstallCallback("vehSiren::SizeOf", "Change size of vehSiren on vehicle initialization.",
        &SizeOf, {
            cb::call(0x42BE30),
        }
    );

    //jmp out ltLightPool destructor
    InstallPatch({ 0xEB }, {
        0x4D6638,
    });

    //don't draw Angels siren lights
    InstallPatch({
        0xE9, 0xD1, 0x0, 0x0, 0x0,
        0x90,
    }, {
        0x4D68C1,
    });

    ConfigValue<float> cfgSirenRotationSpeed("SirenRotationSpeed", 3.1415927f);

    vehSiren::SirenRotationSpeed = cfgSirenRotationSpeed.Get();
}

