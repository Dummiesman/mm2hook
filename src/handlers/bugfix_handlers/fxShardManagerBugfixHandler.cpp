#include "fxShardManagerBugfixHandler.h"

using namespace MM2;

/*
    fxShardManagerBugfixHandler
*/

void fxShardManagerBugfixHandler::Install()
{
    InstallCallback("fxShardManager::Drraw", "Fix crashes with cars with low material count.",
        &fxShardManager::Draw, {
            cb::jmp(0x4602D0)
        }
    );
}
