#include "vehTrailerFeatureHandler.h"

using namespace MM2;

/*
    vehTrailerFeatureHandler
*/

void vehTrailerFeatureHandler::Install() {
    InstallCallback("vehTrailer::Init", "Reads TWHL4/5 MTX files",
        &vehTrailer::Init, {
            cb::call(0x42C023),
        }
    );
}

