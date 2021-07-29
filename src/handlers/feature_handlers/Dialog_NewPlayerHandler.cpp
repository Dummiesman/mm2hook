#include "Dialog_NewPlayerHandler.h"

using namespace MM2;

/*
    Dialog_NewPlayerHandler
*/

void Dialog_NewPlayerHandler::Install() {
    InstallPatch("Enables tooltips in the new player dialog.", { 0x90, 0x90, 0x90 }, {
        0x4FD823,
    });
}

