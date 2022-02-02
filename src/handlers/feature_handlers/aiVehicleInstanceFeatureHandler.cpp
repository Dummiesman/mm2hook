#include "aiVehicleInstanceFeatureHandler.h"

using namespace MM2;

/*
    Model Index Constants
*/
const int HLIGHT_GEOM_ID = 2;
const int TLIGHT_GEOM_ID = 3;
const int SLIGHT0_GEOM_ID = 4;
const int SLIGHT1_GEOM_ID = 5;
const int BLIGHT_GEOM_ID = 18;
const int PLIGHTON_GEOM_ID = 19;
const int PLIGHTOFF_GEOM_ID = 20;
const int TSLIGHT0_GEOM_ID = 21;
const int TSLIGHT1_GEOM_ID = 22;


static ConfigValue<int> cfgAmbientHeadlightStyle("AmbientHeadlightStyle", 0);

/*
    aiVehicleInstanceFeatureHandler
*/
Matrix34 aiVehicleMatrix = Matrix34();
int ambientHeadlightStyle = 0;

void aiVehicleInstanceFeatureHandler::Draw(int a1) {
    auto inst = reinterpret_cast<aiVehicleInstance*>(this);
    auto geomID = inst->GetGeomIndex() - 1;
    auto geomSet = lvlInstance::GetGeomTableEntry(geomID);

    //setup renderer
    Matrix34 carMatrix = inst->GetMatrix(&aiVehicleMatrix);
    gfxRenderState::SetWorldMatrix(carMatrix);

    //get our shader set
    auto shaderSet = *getPtr<signed short>(this, 0x1E);
    auto shaders = geomSet->pShaders[shaderSet];

    //get objects
    modStatic* plighton = lvlInstance::GetGeomTableEntry(geomID + PLIGHTOFF_GEOM_ID)->GetHighLOD();
    modStatic* plightoff = lvlInstance::GetGeomTableEntry(geomID + PLIGHTON_GEOM_ID)->GetHighLOD();

    if (plighton != nullptr) {
        if (aiMap::Instance->showHeadlights)
            inst->DrawPart(plighton, &carMatrix, shaders, *getPtr<int>(this, 6));
    }
    if (plightoff != nullptr) {
        if (!aiMap::Instance->showHeadlights)
            inst->DrawPart(plightoff, &carMatrix, shaders, *getPtr<int>(this, 6));
    }

    //call original
    hook::Thunk<0x552160>::Call<void>(this, a1);
}

void aiVehicleInstanceFeatureHandler::DrawGlow() {
    auto inst = reinterpret_cast<aiVehicleInstance*>(this);
    auto geomID = inst->GetGeomIndex() - 1;
    auto geomSet = lvlInstance::GetGeomTableEntry(geomID);

    //setup renderer
    Matrix34 carMatrix = inst->GetMatrix(&aiVehicleMatrix);
    gfxRenderState::SetWorldMatrix(carMatrix);

    //get our shader set
    auto shaderSet = *getPtr<signed short>(this, 0x1E);
    auto shaders = geomSet->pShaders[shaderSet];

    //get objects
    modStatic* hlight = lvlInstance::GetGeomTableEntry(geomID + HLIGHT_GEOM_ID)->GetHighestLOD();
    modStatic* tlight = lvlInstance::GetGeomTableEntry(geomID + TLIGHT_GEOM_ID)->GetHighestLOD();
    modStatic* slight0 = lvlInstance::GetGeomTableEntry(geomID + SLIGHT0_GEOM_ID)->GetHighestLOD();
    modStatic* slight1 = lvlInstance::GetGeomTableEntry(geomID + SLIGHT1_GEOM_ID)->GetHighestLOD();
    modStatic* blight = lvlInstance::GetGeomTableEntry(geomID + BLIGHT_GEOM_ID)->GetHighestLOD();
    modStatic* tslight0 = lvlInstance::GetGeomTableEntry(geomID + TSLIGHT0_GEOM_ID)->GetHighestLOD();
    modStatic* tslight1 = lvlInstance::GetGeomTableEntry(geomID + TSLIGHT1_GEOM_ID)->GetHighestLOD();

    //get lights stuff
    float accel = inst->getSpline()->getRailSet()->getAccelFactor();
    float speed = inst->getSpline()->getCurSpeed();

    byte toggleSignal = *getPtr<byte>(this, 0x1A);
    int signalDelayTime = *getPtr<int>(this, 0x18); // adjusts the delay time for signal lights among traffic vehicles

    //draw blight
    if (blight != nullptr) {
        if (accel < 0.f || speed == 0.f)
            blight->Draw(shaders);
    }

    //draw tlight
    if (tlight != nullptr) {
        //draw brake copy
        if (accel < 0.f || speed == 0.f)
            tlight->Draw(shaders);
        //draw headlight copy
        if (aiMap::Instance->showHeadlights)
            tlight->Draw(shaders);
    }

    //draw signals
    if (toggleSignal & 1) {
        if ((aiVehicleManager::SignalClock + signalDelayTime) & 8) {
            if (slight0 != nullptr)
                slight0->Draw(shaders);
            if (tslight0 != nullptr)
                tslight0->Draw(shaders);
        }
    }
    else {
        if (tslight0 != nullptr) {
            //draw brake copy
            if (accel < 0.f || speed == 0.f)
                tslight0->Draw(shaders);
            //draw headlight copy
            if (aiMap::Instance->showHeadlights)
                tslight0->Draw(shaders);
        }
    }

    if (toggleSignal & 2) {
        if ((aiVehicleManager::SignalClock + signalDelayTime) & 8) {
            if (slight1 != nullptr)
                slight1->Draw(shaders);
            if (tslight1 != nullptr)
                tslight1->Draw(shaders);
        }
    }
    else {
        if (tslight1 != nullptr) {
            //draw brake copy
            if (accel < 0.f || speed == 0.f)
                tslight1->Draw(shaders);
            //draw headlight copy
            if (aiMap::Instance->showHeadlights)
                tslight1->Draw(shaders);
        }
    }

    //draw headlights
    if (ambientHeadlightStyle < 3) {
        if (ambientHeadlightStyle == 0 || ambientHeadlightStyle == 2) {
            //MM2 headlights
            if (aiMap::Instance->showHeadlights) {
                //call original
                hook::Thunk<0x552930>::Call<void>(this);
            }
        }
        if (ambientHeadlightStyle == 1 || ambientHeadlightStyle == 2) {
            //MM1 headlights
            gfxRenderState::SetWorldMatrix(carMatrix);

            if (hlight != nullptr && aiMap::Instance->showHeadlights) {
                hlight->Draw(shaders);
            }
        }
    }
}

void aiVehicleInstanceFeatureHandler::ModStaticDraw(modShader* a1) {
    auto mod = reinterpret_cast<modStatic*>(this);
    hook::Type<gfxTexture*> g_ReflectionMap = 0x628914;
    bool isSoftware = *(bool*)0x6830D4;

    //convert world matrix for reflection drawing
    Matrix44* worldMatrix = gfxRenderState::sm_World;
    Matrix34 envInput = Matrix34();
    worldMatrix->ToMatrix34(envInput);

    //draw car part
    mod->Draw(a1);

    //draw reflections
    auto state = &MMSTATE;
    if (g_ReflectionMap != nullptr && !isSoftware && state->EnableReflections) {
        modShader::BeginEnvMap(g_ReflectionMap, envInput);
        mod->DrawEnvMapped(a1, g_ReflectionMap, 1.0f);
        modShader::EndEnvMap();
    }
}

void aiVehicleInstanceFeatureHandler::AddGeomHook(const char* pkgName, const char* name, int flags) {

    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, name, flags);
    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, "blight", flags);
    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, "plighton", flags);
    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, "plightoff", flags);
    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, "tslight0", flags);
    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, "tslight1", flags);
}

void aiVehicleInstanceFeatureHandler::Install() {
    InstallCallback("aiVehicleInstance::aiVehicleInstance", "Adds brake light and pop-up lights geometries.",
        &AddGeomHook, {
            cb::call(0x551F2F),
        }
    );

    if (vehCarModel::PartReflections) {
        InstallCallback("aiVehicleInstance::DrawPart", "Draws reflections on car parts.",
            &ModStaticDraw, {
                cb::call(0x55291F), // aiVehicleInstance::DrawPart
            }
        );
    }

    InstallVTableHook("aiVehicleInstance::Draw",
        &Draw, {
            0x5B5938
        }
    );

    ambientHeadlightStyle = cfgAmbientHeadlightStyle.Get();
    InstallVTableHook("aiVehicleInstance::DrawGlow",
        &DrawGlow, {
            0x5B5944
        }
    );

    // removes Angels tlight
    InstallPatch({ 0xEB }, {
        0x552995,
    });

    // removes Angels slight0
    InstallPatch({ 0xEB }, {
        0x5529F2,
    });

    // removes Angels slight1
    InstallPatch({ 0xEB }, {
        0x552A2E,
    });
}

