#include "mmSingleCircuitHandler.h"

using namespace MM2;

/*
    mmSingleCircuitHandler
*/

void mmSingleCircuitHandler::SetPriority(int a1) {
    auto soundBase = reinterpret_cast<AudSoundBase*>(this);
    soundBase->Load("arrest", 5, false);
    soundBase->SetVolume(1.f);
    soundBase->SetPriority(a1);
}

void mmSingleCircuitHandler::Install() {
    InstallCallback("mmSingleCircuit::InitGameObjects", "Implements arrest wav sound in circuit race.",
        &SetPriority, {
            cb::call(0x41C9EF),
        }
    );
}

