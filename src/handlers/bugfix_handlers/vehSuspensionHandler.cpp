#include "vehSuspensionHandler.h"

using namespace MM2;

/*
    vehSuspensionBugfixHandler
*/

void vehSuspensionBugfixHandler::Install()
{
    InstallCallback("vehSuspension::Init", "Prevent suspension forever disappearing after ex. vehicle re-init.",
        &vehSuspension::Init, {
            cb::call(0x4CBD61),
            cb::call(0x4CBD7A),
            cb::call(0x4CBD8D),
            cb::call(0x4CBDA6),
            cb::call(0x4CBDBF),
            cb::call(0x4CBDD8),
            cb::call(0x4CBDEB),
            cb::call(0x4CBE04),
            cb::call(0x4CBE17),
            cb::call(0x4CBE30)
        }
    );
}