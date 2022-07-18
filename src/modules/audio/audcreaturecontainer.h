#pragma once
#include <mm2_common.h>
#include "aud3dobject.h"

namespace MM2
{
    // Forward declarations
    class AudCreatureContainer;

    // External declarations

    // Class definitions
    class AudCreatureContainer : public Aud3DObject {
    public:
        AGE_API void PlayImpactReaction(float a1) { hook::Thunk<0x510CC0>::Call<void>(this, a1); }
        AGE_API void PlayAvoidanceReaction(float a1) { hook::Thunk<0x510CF0>::Call<void>(this, a1); }
    };
}