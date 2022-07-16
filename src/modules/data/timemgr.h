#pragma once
#include <mm2_common.h>

namespace MM2
{
    // Forward declarations
    class datTimeManager;

    // External declarations


    // Class definitions
    class datTimeManager {
    public:
        static hook::Type<float> InvSeconds;
        static hook::Type<float> FPS;
        static hook::Type<float> Seconds;
        static hook::Type<float> ActualSeconds;
        static hook::Type<float> ElapsedTime;
        static hook::Type<unsigned int> FrameCount;
        static hook::Type<Timer> Time;

        AGE_API static void Reset(void);
        AGE_API static void Update(void);

        static void BindLua(LuaState L);
    };    
}