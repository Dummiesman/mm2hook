#pragma once
#include <modules\phys.h>

namespace MM2
{
    // Forward declarations
    class phBoundSphere;

    // External declarations


    // Class definitions
    class phBoundSphere : public phBound
    {
        float Radius;
        phMaterial Material;
    };
    ASSERT_SIZEOF(phBoundSphere, 0x80);

    // Lua initialization

}