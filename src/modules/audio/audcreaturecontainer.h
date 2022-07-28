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
    private:
        byte _buffer[0x14];
    public:
        AGE_API void PlayImpactReaction(float force) { hook::Thunk<0x510CC0>::Call<void>(this, force); }
        AGE_API void PlayAvoidanceReaction(float a1) { hook::Thunk<0x510CF0>::Call<void>(this, a1); }
    };

    ASSERT_SIZEOF(AudCreatureContainer, 0x74);
}