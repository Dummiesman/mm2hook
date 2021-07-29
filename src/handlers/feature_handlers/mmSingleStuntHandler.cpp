#include "mmSingleStuntHandler.h"

using namespace MM2;

/*
    mmSingleStuntHandler
*/

void mmSingleStuntHandler::SetPriority(int a1) {
    auto soundBase = reinterpret_cast<AudSoundBase*>(this);
    soundBase->Load("arrest", 7, false);
    soundBase->SetVolume(1.f);
    soundBase->SetPriority(a1);
}

void mmSingleStuntHandler::Install() {
    InstallCallback("mmSingleStunt::InitGameObjects", "Implements arrest wav sound in crash course.",
        &SetPriority, {
            cb::call(0x4166EE),
        }
    );
}

