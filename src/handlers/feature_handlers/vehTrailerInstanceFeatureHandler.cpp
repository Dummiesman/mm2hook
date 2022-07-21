#include "vehTrailerInstanceFeatureHandler.h"

using namespace MM2;

static ConfigValue<bool> cfgMm1StyleTransmission("MM1StyleTransmission", false);

/*
    vehTrailerInstanceFeatureHandler
*/
Matrix34 trailerMatrix = Matrix34();

void vehTrailerInstanceFeatureHandler::DrawPartReflections(modStatic* a1, Matrix34* a2, modShader* a3) {
    hook::Type<gfxTexture*> g_ReflectionMap = 0x628914;
    bool isSoftware = *(bool*)0x6830D4;

    //convert world matrix for reflection drawing
    Matrix44 worldMatrix = gfxRenderState::GetWorldMatrix();
    Matrix34 envInput = Matrix34();
    worldMatrix.ToMatrix34(envInput);

    //draw trailer
    a1->Draw(a3);

    //draw reflections
    auto state = &MMSTATE;
    if (g_ReflectionMap != nullptr && !isSoftware && state->EnableReflections) {
        modShader::BeginEnvMap(g_ReflectionMap, envInput);
        a1->DrawEnvMapped(a3, g_ReflectionMap, 1.0f);
        modShader::EndEnvMap();
    }
}

void vehTrailerInstanceFeatureHandler::DrawPart(int a1, int a2, Matrix34* a3, modShader* a4) {
    auto inst = reinterpret_cast<vehTrailerInstance*>(this);

    modStatic* part = inst->GetGeom(a1, a2);
    if (part != nullptr) {
        //setup renderer
        gfxRenderState::SetWorldMatrix(*a3);

        //draw
        if (vehCarModel::PartReflections && a1 == 3)
            DrawPartReflections(part, a3, a4);
        else
            part->Draw(a4);
    }
}

void vehTrailerInstanceFeatureHandler::Draw(int a1) {
    auto inst = reinterpret_cast<vehTrailerInstance*>(this);
    auto trailer = inst->getTrailer();
    auto trailerMtx = inst->GetMatrix(&trailerMatrix);

    //get our shader set
    int shaderSet = *getPtr<int>(this, 24);
    auto shaders = inst->GetShader(shaderSet);

    //get spinning wheels
    modStatic* tswhl0 = inst->GetGeom(a1, vehTrailerInstance::TSWHL0_GEOM_ID);
    modStatic* tswhl1 = inst->GetGeom(a1, vehTrailerInstance::TSWHL1_GEOM_ID);
    modStatic* tswhl2 = inst->GetGeom(a1, vehTrailerInstance::TSWHL2_GEOM_ID);
    modStatic* tswhl3 = inst->GetGeom(a1, vehTrailerInstance::TSWHL3_GEOM_ID);
    modStatic* tswhl4 = inst->GetGeom(a1, vehTrailerInstance::TSWHL4_GEOM_ID);
    modStatic* tswhl5 = inst->GetGeom(a1, vehTrailerInstance::TSWHL5_GEOM_ID);

    vehWheel* wheels[4] = { trailer->getWheel(0), trailer->getWheel(1), trailer->getWheel(2), trailer->getWheel(3) };
    modStatic* sWhlGeometries[4] = { tswhl0, tswhl1, tswhl2, tswhl3 };
    int sWhlIds[4] = { vehTrailerInstance::TSWHL0_GEOM_ID, vehTrailerInstance::TSWHL1_GEOM_ID, vehTrailerInstance::TSWHL2_GEOM_ID, vehTrailerInstance::TSWHL3_GEOM_ID };
    int whlIds[4] = { vehTrailerInstance::TWHL0_GEOM_ID, vehTrailerInstance::TWHL1_GEOM_ID, vehTrailerInstance::TWHL2_GEOM_ID, vehTrailerInstance::TWHL3_GEOM_ID };

    //draw trailer
    DrawPart(a1, 0, &trailerMtx, shaders);

    //draw (s)whl0-4
    for (int i = 0; i < 4; i++) {
        auto wheel = wheels[i];
        if (fabs(wheel->GetRotationRate()) > 26.f && sWhlGeometries[i] != nullptr && vehCarModel::EnableSpinningWheels)
        {
            DrawPart(a1, sWhlIds[i], &wheel->GetMatrix(), shaders);
        }
        else 
        {
            DrawPart(a1, whlIds[i], &wheel->GetMatrix(), shaders);
        }
    }

    if (fabs(wheels[2]->GetRotationRate()) > 26.f && tswhl4 != nullptr && vehCarModel::EnableSpinningWheels)
    {
        DrawTwhl4(a1, vehTrailerInstance::TSWHL4_GEOM_ID, &wheels[2]->GetMatrix(), shaders);
    }
    else {
        DrawTwhl4(a1, vehTrailerInstance::TWHL4_GEOM_ID, &wheels[2]->GetMatrix(), shaders);
    }

    if (fabs(wheels[3]->GetRotationRate()) > 26.f && tswhl5 != nullptr && vehCarModel::EnableSpinningWheels)
    {
        DrawTwhl5(a1, vehTrailerInstance::TSWHL5_GEOM_ID, &wheels[3]->GetMatrix(), shaders);
    }
    else {
        DrawTwhl5(a1, vehTrailerInstance::TWHL5_GEOM_ID, &wheels[3]->GetMatrix(), shaders);
    }
}

void vehTrailerInstanceFeatureHandler::DrawTwhl4(int a1, int a2, Matrix34* a3, modShader* a4) {
    auto inst = reinterpret_cast<vehTrailerInstance*>(this);
    auto trailer = inst->getTrailer();
    auto carsim = trailer->getCarSim();

    auto trailerMtx = inst->GetMatrix(&trailerMatrix);

    float offsetX = carsim->TrailerBackBackLeftWheelPosDiff.Y * trailerMtx.m10 + carsim->TrailerBackBackLeftWheelPosDiff.Z * trailerMtx.m20 + carsim->TrailerBackBackLeftWheelPosDiff.X * trailerMtx.m00;
    float offsetY = carsim->TrailerBackBackLeftWheelPosDiff.Y * trailerMtx.m11 + carsim->TrailerBackBackLeftWheelPosDiff.Z * trailerMtx.m21 + carsim->TrailerBackBackLeftWheelPosDiff.X * trailerMtx.m01;
    float offsetZ = carsim->TrailerBackBackLeftWheelPosDiff.Y * trailerMtx.m12 + carsim->TrailerBackBackLeftWheelPosDiff.Z * trailerMtx.m22 + carsim->TrailerBackBackLeftWheelPosDiff.X * trailerMtx.m02;
    a3->m30 += offsetX;
    a3->m31 += offsetY;
    a3->m32 += offsetZ;

    DrawPart(a1, a2, a3, a4);
}

void vehTrailerInstanceFeatureHandler::DrawTwhl5(int a1, int a2, Matrix34* a3, modShader* a4) {
    auto inst = reinterpret_cast<vehTrailerInstance*>(this);
    auto trailer = inst->getTrailer();
    auto carsim = trailer->getCarSim();

    auto trailerMtx = inst->GetMatrix(&trailerMatrix);

    float offsetX = carsim->TrailerBackBackRightWheelPosDiff.Y * trailerMtx.m10 + carsim->TrailerBackBackRightWheelPosDiff.Z * trailerMtx.m20 + carsim->TrailerBackBackRightWheelPosDiff.X * trailerMtx.m00;
    float offsetY = carsim->TrailerBackBackRightWheelPosDiff.Y * trailerMtx.m11 + carsim->TrailerBackBackRightWheelPosDiff.Z * trailerMtx.m21 + carsim->TrailerBackBackRightWheelPosDiff.X * trailerMtx.m01;
    float offsetZ = carsim->TrailerBackBackRightWheelPosDiff.Y * trailerMtx.m12 + carsim->TrailerBackBackRightWheelPosDiff.Z * trailerMtx.m22 + carsim->TrailerBackBackRightWheelPosDiff.X * trailerMtx.m02;
    a3->m30 += offsetX;
    a3->m31 += offsetY;
    a3->m32 += offsetZ;

    DrawPart(a1, a2, a3, a4);
}

void vehTrailerInstanceFeatureHandler::DrawGlow() {
    auto inst = reinterpret_cast<vehTrailerInstance*>(this);
    //don't draw trailer lights if it's broken
    if (inst->getTrailer()->getJoint()->IsBroken())
        return;

    //get vars
    auto carsim = inst->getTrailer()->getCarSim();
    float brakeInput = carsim->GetBrake();
    int gear = carsim->GetTransmission()->GetGear();

    //setup renderer
    inst->GetMatrix(&trailerMatrix);
    gfxRenderState::SetWorldMatrix(trailerMatrix);

    //get our shader set
    int shaderSet = *getPtr<int>(this, 24);
    auto shaders = inst->GetShader(shaderSet);

    //get lights
    modStatic* tlight = inst->GetGeomBase(vehTrailerInstance::TLIGHT_GEOM_ID)->GetHighestLOD();
    modStatic* rlight = inst->GetGeomBase(vehTrailerInstance::RLIGHT_GEOM_ID)->GetHighestLOD();
    modStatic* blight = inst->GetGeomBase(vehTrailerInstance::BLIGHT_GEOM_ID)->GetHighestLOD();
    modStatic* hlight = inst->GetGeomBase(vehTrailerInstance::HLIGHT_GEOM_ID)->GetHighestLOD();
    modStatic* slight0 = inst->GetGeomBase(vehTrailerInstance::SLIGHT0_GEOM_ID)->GetHighestLOD();
    modStatic* slight1 = inst->GetGeomBase(vehTrailerInstance::SLIGHT1_GEOM_ID)->GetHighestLOD();
    modStatic* siren0 = inst->GetGeomBase(vehTrailerInstance::SIREN0_GEOM_ID)->GetHighestLOD();
    modStatic* siren1 = inst->GetGeomBase(vehTrailerInstance::SIREN1_GEOM_ID)->GetHighestLOD();
    modStatic* tslight0 = inst->GetGeomBase(vehTrailerInstance::TSLIGHT0_GEOM_ID)->GetHighestLOD();
    modStatic* tslight1 = inst->GetGeomBase(vehTrailerInstance::TSLIGHT1_GEOM_ID)->GetHighestLOD();

    if (cfgMm1StyleTransmission.Get()) {
        auto throttle = carsim->GetEngine()->GetThrottleInput();
        auto speedMPH = carsim->GetSpeedMPH();
        auto transmission = carsim->GetTransmission();

        //draw rlight
        if (rlight != nullptr && gear == 0) {
            if (transmission->IsAuto()) {
                if (throttle > 0.f || speedMPH >= 1.f)
                    rlight->Draw(shaders);
            }
            else {
                rlight->Draw(shaders);
            }
        }
    }
    else {
        //draw rlight
        if (rlight != nullptr && gear == 0) {
            rlight->Draw(shaders);
        }
    }

    //draw blight
    if (blight != nullptr && brakeInput > 0.1) {
        blight->Draw(shaders);
    }

    //draw tlight
    if (tlight != nullptr) {
        //draw night copy
        if (vehCarModel::ShowHeadlights)
            tlight->Draw(shaders);

        //draw brake input copy
        if (brakeInput > 0.1) {
            tlight->Draw(shaders);
        }
    }

    //draw hlight
    if (hlight != nullptr) {
        if (vehCarModel::ShowHeadlights)
            hlight->Draw(shaders);
    }

    //check signal clock
    bool drawSignal = fmod(datTimeManager::ElapsedTime, 1.f) > 0.5f;
    //draw stuff!
    if (drawSignal) {
        if (vehCarModel::LeftSignalLightState || vehCarModel::HazardLightsState) {
            if (slight0 != nullptr)
                slight0->Draw(shaders);
            if (tslight0 != nullptr)
                tslight0->Draw(shaders);
        }
        if (vehCarModel::RightSignalLightState || vehCarModel::HazardLightsState) {
            if (slight1 != nullptr)
                slight1->Draw(shaders);
            if (tslight1 != nullptr)
                tslight1->Draw(shaders);
        }
    }

    if (!vehCarModel::LeftSignalLightState && !vehCarModel::HazardLightsState) {
        if (tslight0 != nullptr) {
            //draw night copy
            if (vehCarModel::ShowHeadlights)
                tslight0->Draw(shaders);

            //draw brake input copy
            if (brakeInput > 0.1) {
                tslight0->Draw(shaders);
            }
        }
    }

    if (!vehCarModel::RightSignalLightState && !vehCarModel::HazardLightsState) {
        if (tslight1 != nullptr) {
            //draw night copy
            if (vehCarModel::ShowHeadlights)
                tslight1->Draw(shaders);

            //draw brake input copy
            if (brakeInput > 0.1) {
                tslight1->Draw(shaders);
            }
        }
    }

    //get vehSiren since vehTrailer doesn't have one by default
    auto mgr = *mmGameManager::Instance;
    auto game = mgr->getGame();
    auto player = game->getPlayer();
    auto car = player->getCar();
    auto siren = car->GetSiren();

    //draw siren
    if (siren != nullptr && siren->Active) {
        int sirenStage = fmod(datTimeManager::ElapsedTime, 2 * vehCarModel::SirenCycle) >= vehCarModel::SirenCycle ? 1 : 0;
        if (sirenStage == 0 && siren0 != nullptr) {
            siren0->Draw(shaders);
        }
        else if (sirenStage == 1 && siren1 != nullptr) {
            siren1->Draw(shaders);
        }
    }
}

void vehTrailerInstanceFeatureHandler::AddGeomHook(const char* pkgName, const char* name, int flags) {
    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, name, flags);
    gfxPacket::gfxForceLVERTEX = true;
    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, "rlight", flags);
    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, "blight", flags);
    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, "hlight", flags);
    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, "slight0", flags);
    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, "slight1", flags);
    gfxPacket::gfxForceLVERTEX = false;

    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, "siren0", flags);
    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, "siren1", flags);
    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, "twhl4", flags);
    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, "twhl5", flags);
    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, "tswhl0", flags);
    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, "tswhl1", flags);
    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, "tswhl2", flags);
    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, "tswhl3", flags);
    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, "tswhl4", flags);
    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, "tswhl5", flags);

    gfxPacket::gfxForceLVERTEX = true;
    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, "tslight0", flags);
    hook::Thunk<0x463BA0>::Call<int>(this, pkgName, "tslight1", flags);
    gfxPacket::gfxForceLVERTEX = false;
}

void vehTrailerInstanceFeatureHandler::Install() {
    InstallCallback("vehTrailerInstance::Init", "Adds more lights geometries.",
        &AddGeomHook, {
            cb::call(0x4D7E79),
        }
    );

    InstallVTableHook("vehTrailerInstance::Draw",
        &Draw, {
            0x5B2FB0,
        }
    );

    // adds custom light rendering, which adds proper brake lights,
    // reverse lights, signal lights, siren lights and night lights
    InstallVTableHook("vehTrailerInstance::DrawGlow",
        &DrawGlow, {
            0x5B2FBC,
        }
    );
}