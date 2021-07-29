#include "mmSingleRaceHandler.h"

using namespace MM2;

/*
    mmSingleRaceHandler
*/

void mmSingleRaceHandler::QueueCopVoice(float a1) {
    //play damage out voice
    auto rsPtr = AudManager::Instance->GetRaceSpeechPtr();
    if (rsPtr != nullptr)
        rsPtr->PlayDamagePenalty();
}

void mmSingleRaceHandler::SetPriority(int a1) {
    auto soundBase = reinterpret_cast<AudSoundBase*>(this);
    soundBase->Load("arrest", 6, false);
    soundBase->SetVolume(1.f);
    soundBase->SetPriority(a1);
}

void mmSingleRaceHandler::Install() {
    InstallCallback("mmSingleRace::UpdateGame", "Plays damage out voices in checkpoint race.",
        &QueueCopVoice, {
            cb::call(0x41E9EF),
        }
    );

    InstallCallback("mmSingleRace::InitGameObjects", "Implements arrest wav sound in checkpoint race.",
        &SetPriority, {
            cb::call(0x41E48F),
        }
    );

    InstallPatch("Skips Aud3DObjectManager check, since we aren't using it.", { 0x90, 0x90 }, {
        0x41E9E8,
    });
}

