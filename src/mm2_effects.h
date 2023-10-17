#pragma once
#include <modules\effects.h>

namespace MM2
{
    // Forward declarations

    //Lua initialization
    template<>
    void luaAddModule<module_effects>(LuaState L) {
        luaBind<asMeshCardVertex>(L);
        luaBind<asBirthRule>(L);
        luaBind<asParticles>(L);
        luaBind<asLineSparks>(L);
        luaBind<fxWaterSpout>(L);
    }
}