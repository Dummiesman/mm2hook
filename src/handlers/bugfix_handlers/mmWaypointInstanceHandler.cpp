#include "mmWaypointInstanceHandler.h"

using namespace MM2;

/*
    mmWaypointInstanceHandler
*/

void mmWaypointInstanceHandler::Draw(int lod)
{
    return reinterpret_cast<mmBillInstance*>(this)->mmBillInstance::Draw(lod);
}

void mmWaypointInstanceHandler::Install() {
    Installf("Installing checkpoint instance count leak fix.");
    InstallPatch({
        0x90, 0x90, 0x90, 0x90, 0x90,
        0x90, 0x90, 0x90,
        0x90, 0x90, 0x90, 0x90, 0x90
    }, {
        0x43F5C6,
    });

    Installf("Installing waypoint billboard fix.");
    InstallVTableHook("mmBillInstance::Draw",
        &Draw, {
            0x5B12EC, // mmBillInstance::Draw
        }
    );
}