#pragma once
#include <mm2_common.h>

namespace MM2
{
    // Forward declarations
    class aiRailSet;

    // External declarations
    extern class aiPath;

    // Class definitions
    class aiRailSet {
    private:
        byte _buffer[0xA0];
    protected:
        static hook::Field<0x98, float> _speed;
        static hook::Field<0x14, float> _roadDist;
        static hook::Field<0x2C, short> _curLane;
        static hook::Field<0x3C, aiPath*> _curLink;
        static hook::Field<0x40, short> _side;
        static hook::Field<0x44, float> _accelFactor;
        static hook::Field<0x48, float> _exheedLimit;
    public:
        aiRailSet()
        {
            hook::Thunk<0x556D50>::Call<void>(this);
        }

        ~aiRailSet()
        {
            hook::Thunk<0x556E10>::Call<void>(this);
        }
        int GetRoadSide() const
        {
            return _side.get(this);
        }

        float GetSpeed() const
        {
            return _speed.get(this);
        }

        float GetAccelFactor() const
        {
            return _accelFactor.get(this);
        }

        void SetAccelFactor(float value)
        {
            _accelFactor.set(this, value);
        }

        aiPath* GetCurrentLink()
        {
            return _curLink.get(this);
        }

        int GetCurrentLane() const
        {
            return _curLane.get(this);
        }

        float GetExheedLimit() const
        {
            return _exheedLimit.get(this);
        }

        float GetRoadDist() const
        {
            return _roadDist.get(this);
        }

        void SetRoadDist(float dist)
        {
            _roadDist.set(this, dist);
        }
    };
}