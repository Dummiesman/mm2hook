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
        float NormalizedDistance;
        float Penetration;
        byte IntersectResult;

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<phIntersectionPoint>("phIntersectionPoint")
                .addVariableRef("Point", &phIntersectionPoint::Point, false)
                .addVariableRef("Normal", &phIntersectionPoint::Normal, false)
                .addVariableRef("Penetration", &phIntersectionPoint::Penetration, false)
                .addVariableRef("NormalizedDistance", &phIntersectionPoint::NormalizedDistance, false)
                .endClass();
        }
    };
    ASSERT_SIZEOF(phIntersectionPoint, 0x24);


    // Lua initialization

}