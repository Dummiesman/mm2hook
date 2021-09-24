#pragma once
#include "mm2_common.h"
#include "mm2_base.h"
#include "mm2_rgl.h"

#include <modules\effects.h>

namespace MM2
{
    // Forward declarations

    //Lua initialization
    template<>
    void luaAddModule<module_particle>(LuaState L) {
        luaBind<asMeshCardVertex>(L);
        luaBind<asBirthRule>(L);
        luaBind<asParticles>(L);
    }
}