#include "vehWheelHandler.h"

using namespace MM2;

/*
    vehWheelHandler
*/

static ConfigValue<bool> cfgWheelWobble("PhysicalWheelWobble", false);

float vehWheelHandler::GetBumpDisplacement(float a1)
{
    //call original
    float displacement = hook::Thunk<0x4D3440>::Call<float>(this, a1);

    //get vars
    float wheelWobble = *getPtr<float>(this, 0x218);
    float totalWheelAngle = *getPtr<float>(this, 0x1E4);
    float wobbleLimit = *getPtr<float>(this, 0x74);

    //no need to calculate if the vehicle isn't set up for this
    if (wobbleLimit == 0.f) {
        return displacement;
    }

    //calculate wobble factor
    float wheelAngleAbs = fmodf(fabsf(totalWheelAngle), 6.28f);
    float wheelAngleSub = wheelAngleAbs;
    if (wheelAngleAbs > 3.14f) {
        wheelAngleSub = 3.14f - (wheelAngleAbs - 3.14f);
    }
    float wheelWobbleFactor = (wheelAngleSub / 3.14f) * wobbleLimit;

    //return displacement - wobble
    float dispSubtraction = fabsf(wheelWobble) * wheelWobbleFactor;
    return displacement - dispSubtraction;
}

void vehWheelHandler::Install()
{
    InstallCallback("vehWheel::Init", "Use rewritten vehWheel init.",
        &vehWheel::Init, {
            cb::call(0x4CBC0A),
            cb::call(0x4CBC41),
            cb::call(0x4CBC7D),
            cb::call(0x4CBCBB),
            cb::call(0x4D74B8),
            cb::call(0x4D74E9),
            cb::call(0x4D751A),
            cb::call(0x4D754B),
        }
    );

    if (!cfgWheelWobble.Get())
        return;

    InstallCallback("vehWheel::ComputeDwtdw", "Implementation of physical wheel wobbling.",
        &GetBumpDisplacement, {
            cb::call(0x4D2EDA), // vehWheel::ComputeDwtdw
        }
    );
}

