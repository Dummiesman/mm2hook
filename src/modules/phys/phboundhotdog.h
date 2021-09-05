#pragma once
#include <modules\phys\phmaterial.h>
#include <modules\phys\phbound.h>

namespace MM2
{
    // Forward declarations
    class phBoundHotdog;

    // External declarations
    extern class phMaterial;

    // Class definitions
    class phBoundHotdog : public phBound
    {
        float Radius;
        float Height;
        phMaterial Material;
    };
    ASSERT_SIZEOF(phBoundHotdog, 0x84);


    // Lua initialization

}