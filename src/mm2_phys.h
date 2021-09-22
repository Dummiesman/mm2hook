#pragma once
#include "mm2_common.h"

#include <modules\phys.h>
#include <modules\banger.h>

namespace MM2
{
    template<>
    void luaAddModule<module_phys>(LuaState L) {
        luaBind<phMaterial>(L);
        luaBind<LuaRaycastResult>(L);
        luaBind<phInertialCS>(L);
        luaBind<dgPhysManager>(L);
        luaBind<phJoint>(L);
        luaBind<dgTrailerJoint>(L);
        luaBind<dgPhysEntity>(L);
        luaBind<dgBangerData>(L);
        luaBind<dgBangerInstance>(L);
    }
}