#pragma once
#include <mm2_common.h>
#include "audimpactdata.h"

namespace MM2
{
    // Forward declarations
    class AudImpact;

    // External declarations

    // Class definitions
    class AudImpact {
    private:
        int LastPlayedImpactID;
        bool Is3D;
        int Ticks;
        int LastTicks;
        int NumImpactDatas;
        AudImpactData** ImpactDatas;
        float Attenuation;
        float Pan;
    public:
        AGE_API void Play(float force, int impactId)        { hook::Thunk<0x511860>::Call<void>(this, force, impactId);   }
    };
}