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

        AGE_API static void SetTempOversampling(bool a1, int a2);
        AGE_API static void FixedFrame(float fps, unsigned int overSamples);
        AGE_API static void RealTime(float step);
        AGE_API static void Reset();
        AGE_API static void Update();

        static void BindLua(LuaState L);
    };    
}