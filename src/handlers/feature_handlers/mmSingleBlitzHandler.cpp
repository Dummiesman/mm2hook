#include "mmSingleBlitzHandler.h"

using namespace MM2;

/*
    mmSingleBlitzHandler
*/

void mmSingleBlitzHandler::SetPriority(int a1) {
    auto soundBase = reinterpret_cast<AudSoundBase*>(this);
    soundBase->Load("arrest", 7, false);
    soundBase->SetVolume(1.f);
    soundBase->SetPriority(a1);
}

void mmSingleBlitzHandler::Install() {
    InstallCallback("mmSingleBlitz::InitGameObjects", "Implements arrest wav sound in blitz race.",
        &SetPriority, {
            cb::call(0x41B30A),
        }
    );
}

