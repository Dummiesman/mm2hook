#include "mmArrowHandler.h"

using namespace MM2;

static ConfigValue<bool> cfgHudArrowStyles("EnableHudArrowStyles", true);

/*
    mmArrowHandler
*/

void mmArrowHandler::SetShape(LPCSTR modelName, LPCSTR dirName, bool useLVertex, Vector3* a4) {
    auto form = reinterpret_cast<asMeshSetForm*>(this);

    if (dgStatePack::Instance->GameMode == Blitz)
        form->SetShape("hudarrow_blitz01", "geometry", 0);
    else if (dgStatePack::Instance->GameMode == CrashCourse)
        form->SetShape("hudarrow_cc01", "geometry", 0);
    else
        form->SetShape("hudarrow01", "geometry", 0);
}

void mmArrowHandler::Install()
{
    if (cfgHudArrowStyles.Get()) {
        InstallCallback("mmArrow::mmArrow", "Enables the unused hud arrows for blitz and crash course game modes.",
            &SetShape, {
                cb::call(0x42E6BA),
            }
        );
    }
}

