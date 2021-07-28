#include "asMeshCardInfoHandler.h"

using namespace MM2;

/*
    asMeshCardInfoHandler
*/

void asMeshCardInfoHandler::Install()
{
    InstallCallback("asMeshCardInfo::Draw", "Scales particles correctly based on current cull mode.",
        &asMeshCardInfo::Draw, {
            cb::jmp(0x461770),
        }
    );
}