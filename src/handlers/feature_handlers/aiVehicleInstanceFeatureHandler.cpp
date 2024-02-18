#include "aiVehicleInstanceFeatureHandler.h"

using namespace MM2;

static ConfigValue<int> cfgAmbientHeadlightStyle("AmbientHeadlightStyle", 0);
static ConfigValue<bool> cfgAmbientShadows("3DShadows", false);

/*
    aiVehicleInstanceFeatureHandler
*/

void aiVehicleInstanceFeatureHandler::Draw(int lod)
{
    reinterpret_cast<aiVehicleInstance*>(this)->aiVehicleInstance::Draw(lod);
}

void aiVehicleInstanceFeatureHandler::DrawGlow()
{
    reinterpret_cast<aiVehicleInstance*>(this)->aiVehicleInstance::DrawGlow();
}

void aiVehicleInstanceFeatureHandler::DrawShadow()
{
    // call original
    hook::Thunk<0x552CC0>::Call<void>(this);
    
    auto inst = reinterpret_cast<aiVehicleInstance*>(this);
    auto timeWeather = cityLevel::GetCurrentLighting();

    if (MMSTATE->TimeOfDay == 3 || lvlLevel::GetSingleton()->GetRoomInfo(inst->GetRoomId())->Flags & static_cast<int>(RoomFlags::Subterranean))
        return;

    bool prevLighting = gfxRenderState::SetLighting(true);

    //get shaders
    auto shaders = inst->GetShader(inst->GetVariant());

    //get model
    modStatic* model = inst->GetGeomBase(0)->GetHighestLOD();

    if (model != nullptr)
    {
        Matrix34 shadowMatrix, dummyMatrix;
        Matrix34 instanceMatrix = inst->GetMatrix(dummyMatrix);

        if (lvlInstance::ComputeShadowProjectionMatrix(shadowMatrix, inst->GetRoomId(), timeWeather->KeyPitch, timeWeather->KeyHeading, instanceMatrix))
        {
            gfxRenderState::SetWorldMatrix(shadowMatrix);
            model->DrawShadowed(shaders, ComputeShadowIntensity(timeWeather->KeyColor));
        }
    }

    gfxRenderState::SetLighting(prevLighting);
}

void aiVehicleInstanceFeatureHandler::AddGeomHook(const char* pkgName, const char* name, int flags) {

    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, name, flags);
    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, "blight", flags);
    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, "plighton", flags);
    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, "plightoff", flags);
    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, "swhl0", flags);
    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, "swhl1", flags);
    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, "swhl2", flags);
    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, "swhl3", flags);
    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, "swhl4", flags);
    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, "swhl5", flags);
    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, "tslight0", flags);
    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, "tslight1", flags);
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

    if (cfgAmbientShadows.Get()) {
        InstallVTableHook("aiVehicleInstance::DrawShadow",
            &DrawShadow, {
                0x5B593C
            }
        );
    }

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

