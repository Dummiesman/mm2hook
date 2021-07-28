#include "mmCDPlayerHandler.h"

using namespace MM2;

/*
    mmCDPlayerHandler
*/

void mmCDPlayerHandler::Install() {
    //nop out jump
    InstallPatch({ 0x90, 0x90 }, {
        0x42CEC4,
        });

    //force font to 24
    InstallPatch({ 0x18 }, {
        0x42CEC7,
        });
}