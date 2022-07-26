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

        int GetPathCount() const {
            return pathCount;
        }

        inline aiPath* GetPath(int id) const {
            if (id >= pathCount)
                return nullptr;
            return paths[id];
        }

        int GetID() const {
            return id;
        }

        int GetRoomId() const {
            return roomId;
        }

        Vector3 GetCenter() const {
            return center;
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<aiIntersection>("aiIntersection")
                .addFunction("GetPath", &GetPath)
                .addPropertyReadOnly("ID", &GetID)
                .addPropertyReadOnly("RoomId", &GetRoomId)
                .addPropertyReadOnly("Center", &GetCenter)
                .addPropertyReadOnly("NumPaths", &GetPathCount)
                .endClass();
        }
    };

    ASSERT_SIZEOF(aiIntersection, 0x2C);
}