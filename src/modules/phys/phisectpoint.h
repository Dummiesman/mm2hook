#pragma once
#include <modules\phys.h>

namespace MM2
{
    // Forward declarations
    struct phIntersectionPoint;

    // External declarations


    // Class definitions

    struct phIntersectionPoint {
        Vector3 Point;
        Vector3 Normal;
        float Distance;
        float SomeDotProduct;
        byte IntersectResult;
    };
    ASSERT_SIZEOF(phIntersectionPoint, 0x24);


    // Lua initialization

}