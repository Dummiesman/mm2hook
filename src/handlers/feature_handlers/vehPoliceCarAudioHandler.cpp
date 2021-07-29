#include "vehPoliceCarAudioHandler.h"

using namespace MM2;

/*
    vehPoliceCarAudioHandler
*/

void vehPoliceCarAudioHandler::InitSirenAudio(vehCarSim* a1, vehCarDamage* a2, LPCSTR basename, LPCSTR sirenCsvFile, bool a5) {
    string_buf<80> buffer("%s_siren", basename);

    // override the global city one if a custom siren exists
    if (datAssetManager::Exists("aud\\cardata\\player", buffer, "csv"))
        sirenCsvFile = buffer;

    // vehPoliceCarAudio::Init
    hook::Thunk<0x4D46F0>::Call<void>(this, a1, a2, basename, sirenCsvFile, a5);
}

void vehPoliceCarAudioHandler::Reset() {
    auto policeAudio = reinterpret_cast<vehPoliceCarAudio*>(this);
    policeAudio->StopSiren();

    //call original
    hook::Thunk<0x4D5290>::Call<void>(this);
}

void vehPoliceCarAudioHandler::Install() {
    InstallCallback("vehPoliceCarAudio::Init", "Allows vehicles to use their own custom sirens instead of default ones for each city.",
        &InitSirenAudio, {
            cb::call(0x4D44A3),
        }
    );

    InstallCallback("vehPoliceCarAudio::Reset", "Deactivates siren sounds upon reset.",
        &Reset, {
            cb::call(0x4D19D6),
        }
    );
}