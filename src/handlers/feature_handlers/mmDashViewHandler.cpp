#include "mmDashViewHandler.h"

using namespace MM2;

static ConfigValue<bool> cfgEnableHeadBobbing("EnableHeadBobbing", true);

static ConfigValue<float> cfgHeadBobOffsetScaleY("HeadBobOffsetScaleY", 0.0125f);
static ConfigValue<float> cfgHeadBobOffsetScaleZ("HeadBobOffsetScaleZ", 0.0125f);

static ConfigValue<float> cfgHeadBobSteeringFactor("HeadBobSteeringFactor", 0.001875f);

static ConfigValue<float> cfgHeadBobSteeringSpeedFactor
("HeadBobSteeringSpeedFactor", 1.125f);

static ConfigValue<float> cfgHeadBobVelocityScaleY("HeadBobVelocityScaleY", 0.00075f);
static ConfigValue<float> cfgHeadBobVelocityScaleZ("HeadBobVelocityScaleZ", 0.00725f);

static ConfigValue<float> cfgHeadBobMultiplierY("HeadBobMultiplierY", 1.0f);
static ConfigValue<float> cfgHeadBobMultiplierZ("HeadBobMultiplierZ", 1.0f);

/*
    mmDashViewHandler
*/

static Matrix34 sm_DashOffset;

void mmDashViewHandler::UpdateCS() {
    auto dashCam = getPtr<Matrix34>(this, 0x18);
    auto player = *getPtr<mmPlayer*>(this, 0x80);

    auto car = player->GetCar();
    auto carModel = car->GetModel();

    sm_DashOffset = carModel->GetMatrix(&sm_DashOffset);

    auto steering = *getPtr<float>(player, 0x2264);
    auto wheelFact = *getPtr<float>(this, 0x400);

    Vector3 velocity = carModel->GetVelocity();

    auto velY = (velocity.Y * cfgHeadBobVelocityScaleY);
    auto velZ = (velocity.Z - (velocity.Y + velocity.X)) * -cfgHeadBobVelocityScaleZ;

    auto bodyRoll = -(steering * wheelFact) * (cfgHeadBobSteeringFactor * (cfgHeadBobSteeringSpeedFactor * velZ));

    auto headBobY = ((sm_DashOffset.m31 - dashCam->m31) * -cfgHeadBobOffsetScaleY) + velY + bodyRoll;
    auto headBobZ = ((sm_DashOffset.m32 - dashCam->m32) * -cfgHeadBobOffsetScaleZ) * velZ;

    dashCam->m31 += (headBobY * cfgHeadBobMultiplierY);
    dashCam->m32 += (headBobZ * cfgHeadBobMultiplierZ);

    hook::Thunk<0x4A3370>::Call<void>(this);
}

void mmDashViewHandler::FileIO(datParser* parser) {
    //call original FileIO
    hook::Thunk<0x4315D0>::Call<void>(this, parser);

    //add missing things
    parser->AddValue("MaxSpeed", getPtr<float>(this, 0x5D0), 1);
    parser->AddValue("MaxRPM", getPtr<float>(this, 0x5D4), 1);
}

void mmDashViewHandler::Install() {
    if (cfgEnableHeadBobbing) {
        InstallCallback("mmDashView::Update", "Allows for a custom implementation of head-bobbing in dashboards.",
            &UpdateCS, {
                cb::call(0x430F87), // replaces call to asLinearCS::Update
            }
        );
    }

    // add missing variables from MM1
    InstallVTableHook("mmDashView::FileIO", &FileIO, {
        0x5B0D90
        });
}