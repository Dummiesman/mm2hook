#include "InitMapHandler.h"
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

using namespace MM2;

/*
    InitMapHandler
*/

void InitMapHandler::Install()
{
    InstallCallback("InitMap", "Allow loading multiple map files.",
        &datStack::InitDebugSymbols, {
            cb::call(0x4C7120),
            cb::call(0x4C7476),
        }
    );

    InstallCallback("datStack::LookupAddress", "Use custom extension with the addition of DbgHelp.",
        &datStack::LookupAddress, {
            cb::call(0x4C7550),
            cb::call(0x4C76D6),
            cb::call(0x4C773C),
            cb::call(0x4C7F82),
            cb::call(0x576F42),
            cb::call(0x577152),
        }
    );
}
