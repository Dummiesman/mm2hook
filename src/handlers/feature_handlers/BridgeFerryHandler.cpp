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

int BridgeFerryHandler::IsVisibleFerry(gfxViewport const& viewport)
{
    auto inst = reinterpret_cast<lvlInstance*>(this);
    int visibleResult = inst->lvlInstance::IsVisible(viewport);
    return max(visibleResult, 1);
}

void BridgeFerryHandler::DrawFerry(int lod) {
    reinterpret_cast<dgBangerInstance*>(this)->dgBangerInstance::Draw(lod);
}

void BridgeFerryHandler::InitBridge(const char* name, const MM2::Matrix34& matrix)
{
    // Mark bridges as static to achieve two things
    // - Ignore the nodraw distance
    // - Move them to the start of the instance list so shadows work
    // As far as I'm aware this has no side effects
    reinterpret_cast<gizBridge*>(this)->SetFlag(lvlInstance::INST_STATIC);
    reinterpret_cast<gizBridge*>(this)->Init(name, matrix);
}

void BridgeFerryHandler::DrawBridge(int lod)
{
    // draw the base model
    reinterpret_cast<dgBangerInstance*>(this)->dgBangerInstance::Draw(lod);

    // cloud shadow
    auto bridge = reinterpret_cast<gizBridge*>(this);
    auto geometry = bridge->GetGeom(lod, 0);
    if (geometry && (geometry->Flags & (vglCloudMapEnable.get() * 2)) != 0)
    {
        geometry->DrawOrthoMapped(bridge->GetShader(bridge->GetVariant()), vglCloudMapTexture, 0.0078125f, vglCloudMapEnable);
    }
}

void BridgeFerryHandler::Install() {
    InstallCallback("gizBridge::Init", "Mark instance as static.",
        &InitBridge, {
            cb::call(0x577C44),
            cb::call(0x577C91),
        }
    );

    // revert bridges/ferries to how they were in the betas
    InstallVTableHook("Bridge/Ferry: Cull", &Cull, {
            0x5B6008, // gizBridgeMgr::Cull
            0x5B61FC, // gizFerryMgr::Cull
        });
    InstallVTableHook("Ferry: IsVisible", &IsVisibleFerry, {
            0x5B6188, // lvlInstance::IsVisible
        });
    InstallVTableHook("Ferry: Draw", &DrawFerry, {
        0x5B61AC, // gizFerry::Draw
        });
    InstallVTableHook("Bridge: Draw", &DrawBridge, {
        0x5B5FB8, // gizBridge::Draw
        });
}