#include "mpConsistencyHandler.h"

using namespace MM2;

/*
    mpConsistencyHandler
*/

// Makes multiplayer AIMAP behavior match singleplayer
void mpConsistencyHandler::Install() {
    // removes mmGame.EnableAIMAP check
    InstallPatch({ 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }, {
        0x412ACB,
        });

    // removes mmMultiRoam second AIMAP
    InstallPatch({ 0xEB }, {
        0x427661,
        });

    // removes mmMultiBlitz second AIMAP
    InstallPatch({ 0xEB }, {
        0x420084,
        });

    // removes mmMultiRace second AIMAP
    InstallPatch({ 0xEB }, {
        0x4283A8,
        });

    // removes mmMultiCircuit second AIMAP
    InstallPatch({ 0xEB }, {
        0x421CE4,
        });

    // removes mmMultiCR second AIMAP
    InstallPatch({ 0xEB }, {
        0x4238BE,
        });
}