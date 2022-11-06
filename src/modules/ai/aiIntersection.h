#pragma once
#include <mm2_common.h>
#include "aiVehicle.h"
#include "aiPath.h"
#include "aiTrafficLight.h"

namespace MM2
{
    // Forward declarations
    class aiIntersection;

    // External declarations


    // Class definitions

    class aiIntersection {
    private:
        aiPath * *paths;
        int16_t pathCount;
        aiVehicle *prevVeh;
        aiVehicle *nextVeh;
        uint16_t roomId;
        uint16_t id;
        Vector3 center;
        aiTrafficLightSet *trafficLights;
        aiObstacle *vehicles;
        aiObstacle *bangers;
    public:
        aiIntersection(void)                                DONOTCALL;
        aiIntersection(const aiIntersection &&)             DONOTCALL;

        AGE_API int NumSources() const;
        AGE_API int NumSinks() const;

        int GetPathCount() const;
        aiPath* GetPath(int id) const;
        int GetId() const;
        int GetRoomId() const;
        Vector3 GetCenter() const;

        static void BindLua(LuaState L);
    };

    ASSERT_SIZEOF(aiIntersection, 0x2C);
}