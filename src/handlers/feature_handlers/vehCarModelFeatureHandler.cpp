#include "vehCarModelFeatureHandler.h"

using namespace MM2;

static ConfigValue<bool> cfgMm1StyleTransmission("MM1StyleTransmission", false);
static ConfigValue<bool> cfgEnable3DDamage("3DDamage", true);
static ConfigValue<bool> cfgCarShadows("3DShadows", false);

/*
    vehCarModelFeatureHandler
*/

void vehCarModelFeatureHandler::Draw(int a1) {
    auto model = reinterpret_cast<vehCarModel*>(this);
    model->vehCarModel::Draw(a1);
}

void vehCarModelFeatureHandler::ModStaticDraw(modShader* a1) {
    auto mod = reinterpret_cast<modStatic*>(this);
    hook::Type<gfxTexture *> g_ReflectionMap = 0x628914;
    bool isSoftware = *(bool*)0x6830D4;

    //convert world matrix for reflection drawing
    Matrix44 worldMatrix = gfxRenderState::GetWorldMatrix();
    Matrix34 envInput = Matrix34();
    worldMatrix.ToMatrix34(envInput);

    //draw breakable
    mod->Draw(a1);

    //draw reflections
    auto state = &MMSTATE;
    if (g_ReflectionMap != nullptr && !isSoftware && state->EnableReflections) {
        modShader::BeginEnvMap(g_ReflectionMap, envInput);
        mod->DrawEnvMapped(a1, g_ReflectionMap, 1.0f);
        modShader::EndEnvMap();
    }
}

void vehCarModelFeatureHandler::ApplyImpact(vehDamageImpactInfo* a1)
{
    auto damage = reinterpret_cast<vehCarDamage*>(this);
    hook::Thunk<0x4CB140>::Call<void>(this, a1); // Call original

    if (a1->Damage >= damage->GetImpactThreshold() && (damage->GetCar()->GetCarSim()->GetSpeedMPH() >= 10.0f || a1->OtherCollider->GetICS()))
    {
        auto damage3d = damage->GetCar()->GetModel()->GetDamage3D();
        if (damage3d)
        {
            damage3d->Impact(a1->LocalPosition);
        }
    }
}

void vehCarModelFeatureHandler::SetVariant(int a1)
{
    auto model = reinterpret_cast<vehCarModel*>(this);
    model->vehCarModel::SetVariant(a1);
}

void vehCarModelFeatureHandler::DrawGlow() {
    auto model = reinterpret_cast<vehCarModel*>(this);
    model->vehCarModel::DrawGlow();
}

void vehCarModelFeatureHandler::DrawShadow()
{
    auto model = reinterpret_cast<vehCarModel*>(this);
    model->vehCarModel::DrawShadow();
}

void vehCarModelFeatureHandler::Install() {
    InstallPatch({ 0x60, 0x1 }, {
        0x42BB6E + 1, // Change size of vehCarModel on allocation
    });

    InstallPatch({ 0x60, 0x1 }, {
        0x4CDFE0 + 1, // Change size of vehCarModel on SizeOf
    });

    InstallCallback("vehCarModel::Init", "Use rewritten vehCarModel init.",
        &vehCarModel::Init, {
            cb::call(0x42BE86),
        }
    );

    InstallCallback("vehCarModel::EjectOneShot", "add more mechanical breakables.",
        &vehCarModel::EjectOneshot, {
            cb::call(0x4CAE16),
        }
    );

    InstallCallback("vehCarModel::ClearDamage", "reset 3d damage on ClearDamage.",
        &vehCarModel::ClearDamage, {
            cb::call(0x42C464),
        }
    );

    InstallCallback("vehCarDamage::ApplyImpact", "Add 3d damage.",
        &ApplyImpact, {
            cb::call(0x4CAFE0),
            cb::call(0x4CB12C),
        }
    );

    InstallVTableHook("vehCarModel::SetVariant",
        &SetVariant, {
            0x5B2CC4,
        }
    );

    InstallVTableHook("vehCarModel::Draw",
        &Draw, {
            0x5B2CDC,
        }
    );

    InstallVTableHook("vehCarModel::DrawGlow",
        &DrawGlow, {
            0x5B2CE8
        }
    );

    InstallVTableHook("vehCarModel::DrawShadow",
        &DrawShadow, {
            0x5B2CE0
        }
    );

    ConfigValue<bool> cfgEnableSpinningWheels("EnableSpinningWheels", true);
    ConfigValue<bool> cfgPartReflections("ReflectionsOnCarParts", false);
    ConfigValue<bool> cfgHeadlightFlashing("EnableHeadlightFlashing", true);
    ConfigValue<bool> cfgNfsMwStyleTotaledCar("NFSMWStyleTotaledCar", false);
    ConfigValue<bool> cfgBreakableRenderTweak("BreakableRenderTweak", false);
    ConfigValue<int> cfgSirenStyle("SirenStyle", 0);
    ConfigValue<int> cfgHeadlightStyle("HeadlightStyle", 0);
    ConfigValue<float> cfgSirenCycleRate("SirenCycle", 0.25f);
    ConfigValue<float> cfgHeadlightFlashingSpeed("HeadlightFlashingSpeed", 42.411503f);

    vehCarModel::EnableSpinningWheels = cfgEnableSpinningWheels.Get();
    vehCarModel::EnableHeadlightFlashing = cfgHeadlightFlashing.Get();
    vehCarModel::SirenType = cfgSirenStyle.Get();
    vehCarModel::HeadlightType = cfgHeadlightStyle.Get();
    vehCarModel::SirenCycle = cfgSirenCycleRate.Get();
    vehCarModel::MWStyleTotaledCar = cfgNfsMwStyleTotaledCar.Get();
    vehCarModel::HeadlightFlashingSpeed = cfgHeadlightFlashingSpeed.Get();

    vehCarModel::PartReflections = cfgPartReflections.Get();
    vehCarModel::WheelReflections = vehCarModel::PartReflections;

    vehCarModel::mm1StyleTransmission = cfgMm1StyleTransmission.Get();
    vehCarModel::breakableRenderTweak = cfgBreakableRenderTweak.Get();
    
    vehCarModel::Enable3DDamage = cfgEnable3DDamage.Get();
    vehCarModel::Enable3DShadows = cfgCarShadows.Get();
}

