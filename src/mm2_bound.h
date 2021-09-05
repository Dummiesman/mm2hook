#pragma once
#include "mm2_common.h"
#include "mm2_phys.h"

namespace MM2
{
    // Extern declarations
    extern class phMaterial;
    extern class phImpactBase;
    extern struct phSegment;
    extern struct phIntersection;
    extern struct phIntersectionPoint;

    // Forward declarations
    

    // Class definitions


    template<>
    void luaAddModule<module_bound>(LuaState L) {
        luaBind<phBound>(L);
    }
}