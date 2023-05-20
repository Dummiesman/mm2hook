#pragma once
#include "mm2_common.h"
#include "mm2_gfx.h"
#include "mm2_base.h"

#include <modules\model.h>
#include <modules\mmeffects\meshform.h>

namespace MM2
{
    // Forward declarations

    // External declarations
    
    // Lua initialization
    template<>
    void luaAddModule<module_model>(LuaState L) {
        luaBind<modShader>(L);
        luaBind<modStatic>(L);
        luaBind<asMeshSetForm>(L);

        //LuaBinding(L).addFunction("GetPivot", &GetPivot);
    }
}