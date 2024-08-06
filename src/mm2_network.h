#pragma once
#include "mm2_common.h"
#include "mm2_data.h"

#include <modules\mmnetwork.h>

namespace MM2 {
    template<>
    void luaAddModule<module_network>(LuaState L) {        
        luaBind<LuaSessionInfo>(L);
        luaBind<mmNetworkPlayerData>(L);
        luaBind<asNetwork>(L);
        luaBind<asNetObject>(L);
        luaBind<mmNetObject>(L);
    }
}