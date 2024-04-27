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

void BridgeFerryHandler::Cull() {}

void BridgeFerryHandler::Draw(int lod) {
    reinterpret_cast<dgBangerInstance*>(this)->dgBangerInstance::Draw(lod);
}

int BridgeFerryHandler::InitHook(const char* a1, const Matrix34& a2, int a3)
{
    // Set INST_STATIC flag to allow shadow rendering, and bypass the render distance cutoff
    auto inst = reinterpret_cast<dgUnhitBangerInstance*>(this);
    int retval = inst->Init(a1, a2, a3);
    inst->SetFlags((inst->GetFlags() & ~64) | 1024); // unset shadow since it breaks 3D shadows, and set INST_STATIC
    return retval;
}

void BridgeFerryHandler::Install() {
    // revert bridges/ferries to how they were in the betas
    InstallVTableHook("Bridge/Ferry: Cull", &Cull, {
            0x5B6008, // gizBridgeMgr::Cull
            0x5B61FC, // gizFerryMgr::Cull
        });

    InstallVTableHook("Bridge/Ferry: Draw", &Draw, {
        0x5B5FB8, // gizBridge::Draw
        0x5B61AC, // gizFerry::Draw
        });


    InstallCallback("Bridge/Ferry: Init", 
        &InitHook, {
            //cb::call(0x57944A), // Ferries cannot use this because they move rooms more than once. x| (static inst chain only works once)
            cb::call(0x577570)
        });
}