#include "lvlSkyHandler.h"

using namespace MM2;

/*
    lvlSkyHandler
*/

void lvlSkyHandler::ResetRot() {
    lvlSky* sky = cityLevel::Sky.ptr();
    if (sky != nullptr)
        sky->SetRotation(0.0f);
}

void lvlSkyHandler::Install() {
    //Overwrite SetMIPMapEnv call since it does nothing
    //and the game will crash without a .sky file when atetmpting to call it
    InstallPatch({ 0xEB, 0x0F }, {
        0x465226,
        });

    InstallCallback("mmGame::Reset", "Fixes sky rotation on city reset.",
        &ResetRot, {
            cb::jmp(0x413DE3),
        }
    );
}