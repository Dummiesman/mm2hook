#include "vehCarAudioContainerHandler.h"

using namespace MM2;

/*
    vehCarAudioContainerHandler
*/

char defaultCitySirenFile[64] = "sfpolicesiren";

void vehCarAudioContainerHandler::SetSirenCSVName(LPCSTR name) {
    string_buf<80> buffer("%spolicesiren", MMSTATE->CityName);

    LPCSTR szSirenName = (datAssetManager::Exists("aud\\cardata\\player", buffer, "csv")) ? buffer : defaultCitySirenFile;

    LogFile::Format("SirenCSVName: %s\n", szSirenName);

    vehCarAudioContainer::SetSirenCSVName(szSirenName);
}

void vehCarAudioContainerHandler::Install() {
    HookConfig::GetProperty("DefaultCitySirenFile", defaultCitySirenFile, sizeof(defaultCitySirenFile));

    InstallCallback("vehCarAudioContainer::SetSirenCSVName", "Allows for custom sirens in addon cities.",
        &SetSirenCSVName, {
            cb::call(0x412783),
            cb::call(0x412772),
        }
    );
}