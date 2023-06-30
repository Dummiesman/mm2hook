#pragma once
#include <mm2_common.h>

namespace MM2
{
    // Forward declarations
    class aiGoal;

    // External declarations
    
    // Class definitions
    class aiGoal {
    private:
        int Ticks;
    public:
        virtual void Reset()            PURE;
        virtual void Update()           PURE;
    };
}