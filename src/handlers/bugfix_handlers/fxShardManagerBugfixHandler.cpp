#include "fxShardManagerBugfixHandler.h"

using namespace MM2;

/*
    fxShardManagerBugfixHandler
*/

void fxShardManagerBugfixHandler::Install()
{
    // The fxShardManager::Instances array is an unreferenced MM1 holdover that causes crashes with 41+vehicles
    InstallPatch("Remove fxShardManager instance array.", { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }, {
        0x45FE8B,
    });

    InstallCallback("fxShardManager::Drraw", "Fix crashes with cars with low material count.",
        &fxShardManager::Draw, {
            cb::jmp(0x4602D0)
        }
    );
}
