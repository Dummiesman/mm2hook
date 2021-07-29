#include "BridgeFerryHandler.h"

using namespace MM2;

/*
    BridgeFerryHandler
*/
/*
    By default, bridges are treated as "Cullables" instead of "Drawables" (MM2 is weird)

    Before the patch, Cull calls dgBangerInstance::Draw, and Draw does nothing.
    With the patch, these 2 functions are swapped around, so Draw calls dgBangerInstance::Draw, and Cull does nothing.

    Problem solved, right? Nope. Cull is called by gizBridgeMgr::Cull, where as Draw is called by cityLevel::DrawRooms.

    gizBridgeMgr has it's own maximum draw distance for bridges, so gizBridgeMgr::Cull draws them fine at range.
    But cityLevel::DrawRooms uses the prop lod distances, so the bridges end up disappearing a lot sooner.

    A "solution" is to manually set the LOD distance values (see cityLevel::SetObjectDetail).
    But that would cause everything to be drawn further, and decrase FPS.
    It also seems to create rendering artifacts when set too high.
*/

void BridgeFerryHandler::Cull(int lod) {
    // TODO: Make this do something?
}

void BridgeFerryHandler::Draw(int lod) {
    reinterpret_cast<dgBangerInstance*>(this)->Draw(lod);
}

void BridgeFerryHandler::Install() {
    // revert bridges/ferries to how they were in the betas
    InstallCallback("Bridge/Ferry: Cull", "Quick'n dirty fix for fullbright bridges/ferries.",
        &Cull, {
            cb::call(0x5780BC), // gizBridgeMgr::Cull
            cb::call(0x5798F0), // gizFerryMgr::Cull
        }
    );

    InstallVTableHook("Bridge/Ferry: Draw", &Draw, {
        0x5B5FB8, // gizBridge::Draw
        0x5B61AC, // gizFerry::Draw
        });
}