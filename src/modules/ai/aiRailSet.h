#pragma once
#include <mm2_common.h>

namespace MM2
{
    // Forward declarations
    class aiRailSet;

    // External declarations


    // Class definitions
    class aiRailSet {
    protected:
        static hook::Field<0x98, float> _speed;
        static hook::Field<0x44, float> _accelFactor;
    public:
        float GetSpeed() const
        {
            return _speed.get(this);
        }

        float GetAccelFactor() const
        {
            return _accelFactor.get(this);
        }
    };
}