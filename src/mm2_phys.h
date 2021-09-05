#pragma once
#include "mm2_common.h"

#include <modules\phys.h>

namespace MM2
{
    template<>
    void luaAddModule<module_phys>(LuaState L) {
        luaBind<phMaterial>(L);
        luaBind<phIntersectionPoint>(L);
        luaBind<phInertialCS>(L);
        luaBind<dgPhysManager>(L);
        luaBind<phJoint>(L);
        luaBind<dgTrailerJoint>(L);
        luaBind<dgPhysEntity>(L);
    }
}