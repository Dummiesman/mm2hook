#pragma once
#include <mm2_common.h>
#include "aiGoal.h"

namespace MM2
{
    // Forward declarations
    class aiGoalAvoidPlayer;

    // External declarations
    extern class aiRailSet;
    extern class aiVehicleAmbient;
    extern class aiVehicleSpline;

    // Class definitions
    class aiGoalAvoidPlayer : public aiGoal {
    private:
        bool pdotFlag;
        float Side;
        float Heading;
        float PlayerSideReactDist;
        aiRailSet* RailSet;
        aiVehicleAmbient* Vehicle;
    public:
        virtual void Reset() override                       { hook::Thunk<0x56AD50>::Call<void>(this); }
        virtual void Update() override                      { hook::Thunk<0x56AF50>::Call<void>(this); }

        void AvoidPlayer();
    };

    ASSERT_SIZEOF(aiGoalAvoidPlayer, 0x20);
}