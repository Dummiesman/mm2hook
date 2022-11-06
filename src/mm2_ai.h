#pragma once
#include "mm2_common.h"

#include <modules\ai.h>
#include <modules\city.h>

namespace MM2
{
    //External declarations

    template<>
    void luaAddModule<module_ai>(LuaState L) {
        luaBind<aiPath>(L);
        luaBind<aiIntersection>(L);
        luaBind<aiCityData>(L);
        luaBind<aiPoliceForce>(L);
        luaBind<aiMapStats>(L);
        luaBind<aiObstacle>(L);
        luaBind<aiVehicle>(L);
        luaBind<aiVehicleSpline>(L);
        luaBind<aiVehicleAmbient>(L);
        luaBind<aiVehiclePhysics>(L);
        luaBind<aiRouteRacer>(L);
        luaBind<aiCTFRacer>(L);
        luaBind<aiPoliceOfficer>(L);
        luaBind<mcHookman>(L);
        luaBind<aiVehicleData>(L);
        luaBind<aiVehicleInstance>(L);
        luaBind<aiVehicleManager>(L);
        luaBind<aiPedAudio>(L);
        luaBind<aiMap>(L);
        luaBind<lvlAiMap>(L);
        luaBind<lvlAiRoad>(L);
        luaBind<aiTrafficLightInstance>(L);
        luaBind<aiTrafficLightSet>(L);
    }
}