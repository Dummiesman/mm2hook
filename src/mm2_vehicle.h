#pragma once
#include "mm2_common.h"

#include <modules\vehicle.h>
#include <modules\mmcityinfo.h>

namespace MM2
{
    // Forward declarations

    // External declarations
    template<>
    void luaAddModule<module_vehicle>(LuaState L) {
        luaBind<vehWheel>(L);
        luaBind<vehAero>(L);
        luaBind<vehGyro>(L);
        luaBind<vehAxle>(L);
        luaBind<vehSuspension>(L);
        luaBind<vehStuck>(L);
        luaBind<vehDrivetrain>(L);
        luaBind<vehSiren>(L);
        luaBind<vehBreakable>(L);
        luaBind<vehBreakableMgr>(L);
        luaBind<vehSplash>(L);
        luaBind<vehCar>(L);
        luaBind<vehDamageImpactInfo>(L);
        luaBind<vehCarDamage>(L);
        luaBind<vehCarModel>(L);
        luaBind<vehEngine>(L);
        luaBind<vehCarSim>(L);
        luaBind<vehTransmission>(L);
        luaBind<vehTrailer>(L);
        luaBind<vehTrailerInstance>(L);

        luaBind<vehEngineSampleWrapper>(L);
        luaBind<vehEngineAudio>(L);

        luaBind<vehCarAudio>(L);
        luaBind<vehNitroCarAudio>(L);
        luaBind<vehSemiCarAudio>(L);
        luaBind<vehPoliceCarAudio>(L);
        luaBind<vehCarAudioContainer>(L);

        luaBind<vehBound>(L);

        luaBind<mmVehInfo>(L);
        luaBind<mmVehList>(L);
    }
}