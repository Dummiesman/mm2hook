#include "mmExternalViewHandler.h"

using namespace MM2;

/*
    mmExternalViewHandler
*/

void mmExternalViewHandler::Install() {
    ConfigValue<bool> cfgEnableMouseBar("EnableMouseBar", false);

    if (cfgEnableMouseBar.Get()) {
        InstallPatch("Enables showing up the mouse bar for all input devices.", { 0x90, 0x90 }, {
            0x431A56,
        });
    }
}

