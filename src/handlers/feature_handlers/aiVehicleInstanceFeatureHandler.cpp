#include "aiVehicleInstanceFeatureHandler.h"

using namespace MM2;

/*
    Model Index Constants
*/
static ConfigValue<int> cfgAmbientHeadlightStyle("AmbientHeadlightStyle", 0);

/*
    aiVehicleInstanceFeatureHandler
*/

void aiVehicleInstanceFeatureHandler::Draw(int lod)
{
    reinterpret_cast<aiVehicleInstance*>(this)->aiVehicleInstance::Draw(lod);
}

void aiVehicleInstanceFeatureHandler::DrawGlow() {
    reinterpret_cast<aiVehicleInstance*>(this)->aiVehicleInstance::DrawGlow();
}

void aiVehicleInstanceFeatureHandler::AddGeomHook(const char* pkgName, const char* name, int flags) {

    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, name, flags);
    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, "blight", flags);
    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, "plighton", flags);
    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, "plightoff", flags);
    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, "tslight0", flags);
    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, "tslight1", flags);
    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, "swhl0", flags);
    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, "swhl1", flags);
    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, "swhl2", flags);
    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, "swhl3", flags);
    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, "swhl4", flags);
    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, "swhl5", flags);
}

void aiVehicleInstanceFeatureHandler::VehicleSpline_DrawId()
{
    reinterpret_cast<aiVehicleSpline*>(this)->aiVehicleSpline::DrawId();
}

void aiVehicleInstanceFeatureHandler::Ambient_DrawId()
{
    reinterpret_cast<aiVehicleAmbient*>(this)->aiVehicleAmbient::DrawId();
}

void aiVehicleInstanceFeatureHandler::Install() {
    InstallCallback("aiVehicleInstance::aiVehicleInstance", "Adds more geometries.",
        &AddGeomHook, {
            cb::call(0x551F2F),
        }
    );

    InstallVTableHook("aiVehicleInstance::Draw",
        &Draw, {
            0x5B5938
        }
    );

    aiVehicleInstance::AmbientHeadlightStyle = cfgAmbientHeadlightStyle.Get();
    InstallVTableHook("aiVehicleInstance::DrawGlow",
        &DrawGlow, {
            0x5B5944
        }
    );

    InstallVTableHook("aiVehicleAmbient::DrawId",
        &Ambient_DrawId, {
            0x5B58E8
        }
    );
    InstallVTableHook("aiVehicleSpline::DrawId",
        &VehicleSpline_DrawId, {
            0x5B5B68
        }
    );
}

