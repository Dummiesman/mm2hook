#include "ltLensFlareHandler.h"

using namespace MM2;

/*
    ltLensFlareHandler
*/

static ConfigValue<bool> cfgEnableLensFlare("LensFlare", false);

void ltLensFlareHandler::Install() {
    if (cfgEnableLensFlare.Get()) {
        InstallPatch("Enables lens flares in a semi broken fashion.", { 0x90, 0x90, 0x90, 0x90, 0x90 }, {
            0x59C1EC,
        });
    }
}

