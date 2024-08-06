#pragma once
#include <modules\vehicle.h>

namespace MM2
{
    // Forward declarations


    // External declarations


    // Class definitions
    class vehEngineAudio
    {
    private:
        int SampleCount;
        vehEngineSampleWrapper** Samples;
    public:
        ANGEL_ALLOCATOR

        int GetNumSamples() const
        {
            return SampleCount;
        }

        vehEngineSampleWrapper* GetSample(int index) const
        {
            if (index < 0 || index >= SampleCount)
                return nullptr;
            return Samples[index];
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass< vehEngineAudio>("vehEngineAudio")
                .addPropertyReadOnly("NumSamples", &GetNumSamples)
                .addFunction("GetSample", &GetSample)
                .endClass();
        }
    };

    // Lua initialization

}