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

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<phIntersectionPoint>("phIntersectionPoint")
                .addVariableRef("Point", &phIntersectionPoint::Point, false)
                .addVariableRef("Normal", &phIntersectionPoint::Normal, false)
                .addVariableRef("Distance", &phIntersectionPoint::Distance, false)
                .addVariableRef("Dot", &phIntersectionPoint::SomeDotProduct, false)
                .endClass();
        }
    };
    ASSERT_SIZEOF(phIntersectionPoint, 0x24);


    // Lua initialization

}